/**
 * @file system_task.cpp
 * @brief System monitoring and health management task
 */

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <SPIFFS.h>
#include <SD.h>
#include "config.h"
#include "system_monitor.h"

// External variables
extern sensor_data_t global_sensor_data;
extern SemaphoreHandle_t sensor_data_mutex;

// System metrics
static system_metrics_t current_metrics = {0};
static bool system_critical = false;

// Forward declarations
void collect_system_metrics(void);
void update_global_sensor_data(void);
void check_critical_conditions(void);
void manage_memory(void);
void log_system_status(void);

/**
 * @brief System Task - monitors system health and manages resources
 */
void system_task(void* parameter) {
    Serial.println("⚙️  System Task started");
    
    TickType_t last_wake_time = xTaskGetTickCount();
    
    while (true) {
        // Collect system metrics
        collect_system_metrics();
        
        // Update global sensor data with system info
        update_global_sensor_data();
        
        // Check for critical conditions
        check_critical_conditions();
        
        // Perform memory management
        manage_memory();
        
        // Update status LED
        system_monitor_update_status_led();
        
        // Log system status periodically
        log_system_status();
        
        // Sleep until next monitoring cycle
        vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(SYSTEM_MONITOR_INTERVAL));
    }
}

/**
 * @brief Collect comprehensive system metrics
 */
void collect_system_metrics(void) {
    // Memory metrics
    current_metrics.free_heap_size = ESP.getFreeHeap();
    current_metrics.free_psram_size = ESP.getFreePsram();
    current_metrics.min_free_heap = ESP.getMinFreeHeap();
    
    // System uptime
    current_metrics.uptime_ms = millis();
    
    // Task count (FreeRTOS)
    current_metrics.task_count = uxTaskGetNumberOfTasks();
    
    // Temperature (if available - approximation)
    current_metrics.temperature_celsius = temperatureRead();
    
    // CPU usage estimation (rough approximation)
    static uint32_t last_idle_time = 0;
    uint32_t current_idle_time = millis(); // Simplified
    current_metrics.cpu_usage_percent = 50; // Placeholder - would need proper implementation
    
    // Connectivity status
    current_metrics.wifi_connected = WiFi.status() == WL_CONNECTED;
    current_metrics.sd_card_mounted = SD.cardType() != CARD_NONE;
}

/**
 * @brief Update global sensor data with system information
 */
void update_global_sensor_data(void) {
    if (xSemaphoreTake(sensor_data_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        global_sensor_data.free_memory = current_metrics.free_heap_size;
        global_sensor_data.uptime_seconds = current_metrics.uptime_ms / 1000;
        global_sensor_data.sd_card_present = current_metrics.sd_card_mounted;
        
        xSemaphoreGive(sensor_data_mutex);
    }
}

/**
 * @brief Check for critical system conditions
 */
void check_critical_conditions(void) {
    bool new_critical = false;
    
    // Low memory condition
    if (current_metrics.free_heap_size < LOW_MEMORY_THRESHOLD) {
        Serial.printf("⚠️  Critical: Low heap memory (%lu bytes)\n", 
                     current_metrics.free_heap_size);
        new_critical = true;
    }
    
    // High temperature condition
    if (current_metrics.temperature_celsius > 80.0) {
        Serial.printf("⚠️  Critical: High temperature (%.1f°C)\n", 
                     current_metrics.temperature_celsius);
        new_critical = true;
    }
    
    // Task stack overflow detection
    if (current_metrics.task_count > 20) {
        Serial.printf("⚠️  Warning: High task count (%d)\n", current_metrics.task_count);
    }
    
    system_critical = new_critical;
}

/**
 * @brief Perform memory management and cleanup
 */
void manage_memory(void) {
    // Force garbage collection if memory is low
    if (current_metrics.free_heap_size < (LOW_MEMORY_THRESHOLD * 2)) {
        Serial.println("🧹 Performing memory cleanup...");
        
        // Force heap defragmentation (if supported)
        // This is a placeholder - ESP32 doesn't have direct defrag
        
        Serial.printf("🧹 Memory after cleanup: %lu bytes free\n", ESP.getFreeHeap());
    }
    
    // Log memory usage statistics
    static uint32_t last_memory_log = 0;
    if (millis() - last_memory_log > 60000) { // Every minute
        Serial.printf("💾 Memory Stats - Heap: %lu/%lu KB, PSRAM: %lu/%lu KB\n",
                     current_metrics.free_heap_size / 1024,
                     ESP.getHeapSize() / 1024,
                     current_metrics.free_psram_size / 1024,
                     ESP.getPsramSize() / 1024);
        last_memory_log = millis();
    }
}

/**
 * @brief Log system status to serial and storage
 */
void log_system_status(void) {
    static uint32_t last_status_log = 0;
    uint32_t current_time = millis();
    
    // Log detailed status every 5 minutes
    if (current_time - last_status_log > 300000) {
        Serial.println("\n⚙️  === System Status Report ===");
        Serial.printf("Uptime: %lu seconds (%.1f hours)\n", 
                     current_metrics.uptime_ms / 1000,
                     current_metrics.uptime_ms / 3600000.0);
        Serial.printf("Memory: %lu KB free heap, %lu KB free PSRAM\n",
                     current_metrics.free_heap_size / 1024,
                     current_metrics.free_psram_size / 1024);
        Serial.printf("Temperature: %.1f°C\n", current_metrics.temperature_celsius);
        Serial.printf("Tasks: %d active\n", current_metrics.task_count);
        Serial.printf("WiFi: %s, SD Card: %s\n",
                     current_metrics.wifi_connected ? "Connected" : "Disconnected",
                     current_metrics.sd_card_mounted ? "Mounted" : "Not found");
        Serial.printf("System Status: %s\n", system_critical ? "CRITICAL" : "OK");
        Serial.println("================================\n");
        
        last_status_log = current_time;
        
        // TODO: Write to log file on SD card
        if (current_metrics.sd_card_mounted) {
            // Implement SD card logging
        }
    }
}

// Implementation of system_monitor.h functions

/**
 * @brief Initialize system monitoring
 */
bool system_monitor_init(void) {
    Serial.println("⚙️  Initializing system monitor...");
    
    // Initialize status LED
    pinMode(STATUS_LED_PIN, OUTPUT);
    digitalWrite(STATUS_LED_PIN, LOW);
    
    // Set initial metrics
    current_metrics.free_heap_size = ESP.getFreeHeap();
    current_metrics.free_psram_size = ESP.getFreePsram();
    current_metrics.min_free_heap = ESP.getMinFreeHeap();
    
    Serial.println("✅ System monitor initialized");
    return true;
}

/**
 * @brief Get current system metrics
 */
void system_monitor_get_metrics(system_metrics_t* metrics) {
    if (metrics != NULL) {
        memcpy(metrics, &current_metrics, sizeof(system_metrics_t));
    }
}

/**
 * @brief Log system metrics to serial and SD card
 */
void system_monitor_log_metrics(void) {
    log_system_status();
}

/**
 * @brief Check if system is in critical state
 */
bool system_monitor_is_critical(void) {
    return system_critical;
}

/**
 * @brief Update status LED based on system health
 */
void system_monitor_update_status_led(void) {
    static uint32_t last_blink = 0;
    static bool led_state = false;
    uint32_t current_time = millis();
    
    if (system_critical) {
        // Fast blink for critical state
        if (current_time - last_blink > 250) {
            led_state = !led_state;
            digitalWrite(STATUS_LED_PIN, led_state ? HIGH : LOW);
            last_blink = current_time;
        }
    } else {
        // Slow heartbeat for normal operation
        if (current_time - last_blink > 2000) {
            digitalWrite(STATUS_LED_PIN, HIGH);
            delay(50);
            digitalWrite(STATUS_LED_PIN, LOW);
            last_blink = current_time;
        }
    }
}