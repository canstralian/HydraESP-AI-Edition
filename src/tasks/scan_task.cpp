/**
 * @file scan_task.cpp
 * @brief Network scanning task for WiFi and BLE detection
 */

#include <Arduino.h>
#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "config.h"
#include "ai_states.h"

// External variables
extern sensor_data_t global_sensor_data;
extern SemaphoreHandle_t sensor_data_mutex;

// BLE scanning variables
BLEScan* ble_scanner = nullptr;
static std::vector<BLEAdvertisedDevice> ble_devices;

// WiFi scanning results
static int16_t wifi_networks[MAX_WIFI_NETWORKS];
static String wifi_ssids[MAX_WIFI_NETWORKS];

// Forward declarations
void scan_wifi_networks(void);
void scan_ble_devices(void);
void process_scan_results(void);
void log_interesting_networks(void);

/**
 * @brief BLE scan callback class
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        if (ble_devices.size() < MAX_BLE_DEVICES) {
            ble_devices.push_back(advertisedDevice);
        }
    }
};

/**
 * @brief Scan Task - performs WiFi and BLE network scanning
 */
void scan_task(void* parameter) {
    Serial.println("📡 Scan Task started");
    
    // Initialize BLE scanner
    BLEDevice::init("HydraESP-Scanner");
    ble_scanner = BLEDevice::getScan();
    ble_scanner->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    ble_scanner->setActiveScan(true);
    ble_scanner->setInterval(100);
    ble_scanner->setWindow(99);
    
    Serial.println("✅ BLE scanner initialized");
    
    TickType_t last_wake_time = xTaskGetTickCount();
    
    while (true) {
        Serial.println("📡 Starting network scan cycle...");
        
        // Scan WiFi networks
        scan_wifi_networks();
        
        // Scan BLE devices  
        scan_ble_devices();
        
        // Process and update global sensor data
        process_scan_results();
        
        // Log interesting findings
        log_interesting_networks();
        
        Serial.printf("📊 Scan complete: %d WiFi, %d BLE devices\n",
                     global_sensor_data.wifi_networks_count,
                     global_sensor_data.ble_devices_count);
        
        // Sleep until next scan cycle
        vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(SCAN_INTERVAL));
    }
}

/**
 * @brief Scan for WiFi networks
 */
void scan_wifi_networks(void) {
    Serial.println("📶 Scanning WiFi networks...");
    
    // Start WiFi scan
    int network_count = WiFi.scanNetworks(false, true, false, 300);
    
    if (network_count == WIFI_SCAN_FAILED) {
        Serial.println("❌ WiFi scan failed");
        return;
    }
    
    // Store results
    int stored_count = 0;
    int32_t total_rssi = 0;
    
    for (int i = 0; i < network_count && stored_count < MAX_WIFI_NETWORKS; i++) {
        wifi_networks[stored_count] = WiFi.RSSI(i);
        wifi_ssids[stored_count] = WiFi.SSID(i);
        total_rssi += WiFi.RSSI(i);
        stored_count++;
        
        // Log interesting networks (hidden, unusual names, etc.)
        String ssid = WiFi.SSID(i);
        if (ssid.length() == 0 || ssid.indexOf("Hidden") >= 0 || 
            ssid.indexOf("_nomap") >= 0 || WiFi.RSSI(i) > -30) {
            Serial.printf("🎯 Interesting WiFi: '%s' (RSSI: %d dBm)\n", 
                         ssid.c_str(), WiFi.RSSI(i));
        }
    }
    
    // Update global sensor data (thread-safe)
    if (xSemaphoreTake(sensor_data_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        global_sensor_data.wifi_networks_count = stored_count;
        global_sensor_data.wifi_signal_strength = stored_count > 0 ? total_rssi / stored_count : -100;
        xSemaphoreGive(sensor_data_mutex);
    }
    
    // Clean up
    WiFi.scanDelete();
    
    Serial.printf("✅ WiFi scan complete: %d networks found\n", stored_count);
}

/**
 * @brief Scan for BLE devices
 */
void scan_ble_devices(void) {
    Serial.println("📱 Scanning BLE devices...");
    
    // Clear previous results
    ble_devices.clear();
    
    // Perform BLE scan
    BLEScanResults foundDevices = ble_scanner->start(SCAN_TIME_SECONDS, false);
    
    int device_count = foundDevices.getCount();
    int32_t total_rssi = 0;
    
    // Process discovered devices
    for (int i = 0; i < device_count; i++) {
        BLEAdvertisedDevice device = foundDevices.getDevice(i);
        total_rssi += device.getRSSI();
        
        // Log interesting devices
        String device_name = device.getName().c_str();
        if (device_name.length() > 0) {
            Serial.printf("🔵 BLE Device: '%s' (RSSI: %d dBm, Address: %s)\n",
                         device_name.c_str(), device.getRSSI(), 
                         device.getAddress().toString().c_str());
        }
        
        // Check for specific device types
        if (device.haveServiceUUID()) {
            Serial.printf("🎯 BLE Service: %s\n", device.getServiceUUID().toString().c_str());
        }
    }
    
    // Update global sensor data (thread-safe)
    if (xSemaphoreTake(sensor_data_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        global_sensor_data.ble_devices_count = device_count;
        global_sensor_data.ble_signal_strength = device_count > 0 ? total_rssi / device_count : -100;
        xSemaphoreGive(sensor_data_mutex);
    }
    
    // Clear scan results to free memory
    ble_scanner->clearResults();
    
    Serial.printf("✅ BLE scan complete: %d devices found\n", device_count);
}

/**
 * @brief Process scan results and update sensor data
 */
void process_scan_results(void) {
    // Update timestamp for sensor data
    if (xSemaphoreTake(sensor_data_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        global_sensor_data.uptime_seconds = millis() / 1000;
        
        // Trigger user interaction flag if high activity detected
        if (global_sensor_data.wifi_networks_count > HIGH_WIFI_ACTIVITY_THRESHOLD ||
            global_sensor_data.ble_devices_count > 10) {
            global_sensor_data.user_interaction = true;
        } else {
            global_sensor_data.user_interaction = false;
        }
        
        xSemaphoreGive(sensor_data_mutex);
    }
}

/**
 * @brief Log interesting network findings
 */
void log_interesting_networks(void) {
    static uint32_t last_log_time = 0;
    uint32_t current_time = millis();
    
    // Log summary every 60 seconds
    if (current_time - last_log_time > 60000) {
        Serial.println("\n📊 === Network Activity Summary ===");
        Serial.printf("WiFi Networks: %d (Avg RSSI: %d dBm)\n",
                     global_sensor_data.wifi_networks_count,
                     global_sensor_data.wifi_signal_strength);
        Serial.printf("BLE Devices: %d (Avg RSSI: %d dBm)\n",
                     global_sensor_data.ble_devices_count,
                     global_sensor_data.ble_signal_strength);
        Serial.println("=====================================\n");
        
        last_log_time = current_time;
        
        // TODO: Log to SD card for historical analysis
    }
}
/**
 * @file scan_task.cpp
 * @brief Network scanning task for WiFi and BLE
 */

#include <Arduino.h>
#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEAdvertisedDevice.h>
#include <BLEScan.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include "config.h"
#include "ai_states.h"

// External variables
extern sensor_data_t global_sensor_data;
extern SemaphoreHandle_t sensor_data_mutex;

// Local storage for scan results
static int32_t wifi_networks[MAX_WIFI_NETWORKS];
static String wifi_ssids[MAX_WIFI_NETWORKS];
static BLEScan* ble_scanner = nullptr;

// BLE scan callback class
class ScanCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        // Handle BLE device found
        // This will be called for each device found during scan
    }
};

// Forward declarations
void scan_wifi_networks(void);
void scan_ble_devices(void);
void update_sensor_data(int wifi_count, int32_t avg_rssi, int ble_count, int32_t ble_rssi);

/**
 * @brief Scan Task - performs WiFi and BLE scanning
 */
void scan_task(void* parameter) {
    Serial.println("📶 Scan Task started");
    
    // Initialize BLE
    BLEDevice::init("HydraESP");
    ble_scanner = BLEDevice::getScan();
    ble_scanner->setAdvertisedDeviceCallbacks(new ScanCallbacks());
    ble_scanner->setActiveScan(true);
    ble_scanner->setInterval(100);
    ble_scanner->setWindow(99);
    
    TickType_t last_wake_time = xTaskGetTickCount();
    
    while (true) {
        // Perform WiFi scan
        scan_wifi_networks();
        
        // Small delay between scans
        vTaskDelay(pdMS_TO_TICKS(1000));
        
        // Perform BLE scan
        scan_ble_devices();
        
        // Sleep until next scan cycle
        vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(SCAN_INTERVAL));
    }
}

/**
 * @brief Scan for WiFi networks
 */
void scan_wifi_networks(void) {
    Serial.println("📶 Scanning WiFi networks...");
    
    // Start WiFi scan
    int network_count = WiFi.scanNetworks(false, true, false, 300);
    
    if (network_count == WIFI_SCAN_FAILED) {
        Serial.println("❌ WiFi scan failed");
        return;
    }
    
    // Store results
    int stored_count = 0;
    int32_t total_rssi = 0;
    
    for (int i = 0; i < network_count && stored_count < MAX_WIFI_NETWORKS; i++) {
        wifi_networks[stored_count] = WiFi.RSSI(i);
        wifi_ssids[stored_count] = WiFi.SSID(i);
        total_rssi += WiFi.RSSI(i);
        stored_count++;
        
        // Log interesting networks
        String ssid = WiFi.SSID(i);
        if (ssid.length() == 0 || ssid.indexOf("Hidden") >= 0 || 
            ssid.indexOf("_nomap") >= 0 || WiFi.RSSI(i) > -30) {
            Serial.printf("🎯 Interesting WiFi: '%s' (RSSI: %d dBm)\n", 
                         ssid.c_str(), WiFi.RSSI(i));
        }
    }
    
    // Calculate average RSSI
    int32_t avg_rssi = stored_count > 0 ? total_rssi / stored_count : -100;
    
    // Update sensor data (will be combined with BLE data)
    update_sensor_data(stored_count, avg_rssi, 0, -100);
    
    // Clean up
    WiFi.scanDelete();
    
    Serial.printf("✅ WiFi scan complete: %d networks found\n", stored_count);
}

/**
 * @brief Scan for BLE devices
 */
void scan_ble_devices(void) {
    Serial.println("📱 Scanning BLE devices...");
    
    if (!ble_scanner) {
        Serial.println("❌ BLE scanner not initialized");
        return;
    }
    
    // Start BLE scan
    BLEScanResults scan_results = ble_scanner->start(SCAN_TIME_SECONDS, false);
    int device_count = scan_results.getCount();
    
    int32_t strongest_rssi = -100;
    
    for (int i = 0; i < device_count; i++) {
        BLEAdvertisedDevice device = scan_results.getDevice(i);
        int32_t rssi = device.getRSSI();
        
        if (rssi > strongest_rssi) {
            strongest_rssi = rssi;
        }
        
        // Log interesting devices
        if (rssi > -50) {
            Serial.printf("🎯 Strong BLE device: %s (RSSI: %d dBm)\n", 
                         device.getAddress().toString().c_str(), rssi);
        }
    }
    
    // Update sensor data with BLE results
    if (xSemaphoreTake(sensor_data_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        global_sensor_data.ble_devices_count = device_count;
        global_sensor_data.ble_signal_strength = strongest_rssi;
        xSemaphoreGive(sensor_data_mutex);
    }
    
    // Clear scan results
    ble_scanner->clearResults();
    
    Serial.printf("✅ BLE scan complete: %d devices found\n", device_count);
}

/**
 * @brief Update global sensor data with scan results
 */
void update_sensor_data(int wifi_count, int32_t avg_rssi, int ble_count, int32_t ble_rssi) {
    if (xSemaphoreTake(sensor_data_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        global_sensor_data.wifi_networks_count = wifi_count;
        global_sensor_data.wifi_signal_strength = avg_rssi;
        // BLE data will be updated separately in scan_ble_devices
        xSemaphoreGive(sensor_data_mutex);
    }
}
