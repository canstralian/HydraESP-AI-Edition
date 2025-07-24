/**
 * @file main.cpp  
 * @brief HydraESP AI Edition - Main entry point
 * @author HydraESP Team
 * @version 2.0
 * 
 * ESP32-S3 based Ponagotchi-style AI companion with:
 * - LVGL animated UI with emotional faces
 * - WiFi/BLE network scanning and analysis
 * - TinyML-based behavioral inference
 * - FreeRTOS multitasking architecture
 * - PSRAM optimization for low memory pressure
 */

#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SPIFFS.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>

#include "config.h"
#include "ai_states.h"
#include "system_monitor.h"

// Task handles for FreeRTOS
TaskHandle_t ui_task_handle = NULL;
TaskHandle_t ai_task_handle = NULL;
TaskHandle_t scan_task_handle = NULL;
TaskHandle_t system_task_handle = NULL;

// Global state and synchronization
ai_state_t current_ai_state = AI_STATE_IDLE;
sensor_data_t global_sensor_data = {0};
SemaphoreHandle_t sensor_data_mutex = NULL;
QueueHandle_t ai_state_queue = NULL;

// Forward declarations
void ui_task(void* parameter);
void ai_task(void* parameter);
void scan_task(void* parameter);
void system_task(void* parameter);
bool initialize_hardware(void);
bool initialize_storage(void);
void create_tasks(void);

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n" + String('=', 50));
    Serial.println("🧠 HydraESP AI Edition v2.0");
    Serial.println("ESP32-S3 Ponagotchi-Style AI Companion");
    Serial.println(String('=', 50) + "\n");
    
    // Initialize hardware components
    if (!initialize_hardware()) {
        Serial.println("❌ Hardware initialization failed!");
        ESP.restart();
    }
    
    // Initialize storage systems  
    if (!initialize_storage()) {
        Serial.println("❌ Storage initialization failed!");
        ESP.restart();
    }
    
    // Initialize system monitoring
    if (!system_monitor_init()) {
        Serial.println("❌ System monitor initialization failed!");
        ESP.restart();
    }
    
    // Create FreeRTOS synchronization objects
    sensor_data_mutex = xSemaphoreCreateMutex();
    ai_state_queue = xQueueCreate(10, sizeof(ai_state_t));
    
    if (sensor_data_mutex == NULL || ai_state_queue == NULL) {
        Serial.println("❌ Failed to create synchronization objects!");
        ESP.restart();
    }
    
    // Create and start FreeRTOS tasks
    create_tasks();
    
    Serial.println("✅ HydraESP AI Edition initialized successfully!");
    Serial.println("🚀 All systems operational\n");
}

void loop() {
    // Main loop is handled by FreeRTOS tasks
    // Keep this minimal to avoid blocking
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    // Optional: Watchdog reset or system health check
    if (system_monitor_is_critical()) {
        Serial.println("⚠️  System in critical state - considering restart");
        // Could implement graceful shutdown or restart logic here
    }
}

/**
 * @brief Initialize all hardware components
 * @return true on success, false on failure
 */
bool initialize_hardware(void) {
    Serial.println("🔧 Initializing hardware components...");
    
    // Initialize status LED
    pinMode(STATUS_LED_PIN, OUTPUT);
    digitalWrite(STATUS_LED_PIN, HIGH); // Turn on during init
    
    // Initialize SPI for display and SD card
    SPI.begin(TFT_CLK, -1, TFT_MOSI, -1);
    Serial.println("✅ SPI initialized");
    
    // Initialize I2C for sensors (if needed)
    Wire.begin(I2C_SDA, I2C_SCL);
    Serial.println("✅ I2C initialized");
    
    // Initialize WiFi in station mode
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    Serial.println("✅ WiFi initialized in station mode");
    
    // Initialize Bluetooth
    if (!btStart()) {
        Serial.println("❌ Bluetooth initialization failed");
        return false;
    }
    Serial.println("✅ Bluetooth initialized");
    
    // Check PSRAM availability
    if (psramFound()) {
        Serial.printf("✅ PSRAM initialized: %d KB available\n", 
                     ESP.getPsramSize() / 1024);
    } else {
        Serial.println("⚠️  PSRAM not found - using internal RAM only");
    }
    
    digitalWrite(STATUS_LED_PIN, LOW); // Turn off after init
    return true;
}

/**
 * @brief Initialize storage systems (SD card and SPIFFS)  
 * @return true on success, false on failure
 */
bool initialize_storage(void) {
    Serial.println("💾 Initializing storage systems...");
    
    // Initialize SPIFFS for internal storage
    if (!SPIFFS.begin(true)) {
        Serial.println("❌ SPIFFS initialization failed");
        return false;
    }
    Serial.printf("✅ SPIFFS initialized: %d KB total, %d KB used\n",
                  SPIFFS.totalBytes() / 1024, SPIFFS.usedBytes() / 1024);
    
    // Initialize SD card (optional - don't fail if not present)
    if (SD.begin(SD_CS)) {
        uint64_t cardSize = SD.cardSize() / (1024 * 1024);
        Serial.printf("✅ SD Card initialized: %lluMB\n", cardSize);
        
        // Create log directory if it doesn't exist
        if (!SD.exists("/logs")) {
            SD.mkdir("/logs");
            Serial.println("📁 Created /logs directory");
        }
        
        global_sensor_data.sd_card_present = true;
    } else {
        Serial.println("⚠️  SD Card not found - logging to SPIFFS only");
        global_sensor_data.sd_card_present = false;
    }
    
    return true;
}

/**
 * @brief Create and start all FreeRTOS tasks
 */
void create_tasks(void) {
    Serial.println("🚀 Creating FreeRTOS tasks...");
    
    // Create UI task (highest priority for smooth animations)
    xTaskCreatePinnedToCore(
        ui_task,                    // Task function
        "UI_Task",                  // Task name
        UI_TASK_STACK_SIZE,         // Stack size
        NULL,                       // Parameters
        UI_TASK_PRIORITY,           // Priority
        &ui_task_handle,            // Task handle
        1                           // Core 1 (APP CPU)
    );
    Serial.println("✅ UI Task created on Core 1");
    
    // Create AI inference task  
    xTaskCreatePinnedToCore(
        ai_task,
        "AI_Task", 
        AI_TASK_STACK_SIZE,
        NULL,
        AI_TASK_PRIORITY,
        &ai_task_handle,
        0                           // Core 0 (PRO CPU)
    );
    Serial.println("✅ AI Task created on Core 0");
    
    // Create network scanning task
    xTaskCreatePinnedToCore(
        scan_task,
        "Scan_Task",
        SCAN_TASK_STACK_SIZE, 
        NULL,
        SCAN_TASK_PRIORITY,
        &scan_task_handle,
        0                           // Core 0 (PRO CPU)
    );
    Serial.println("✅ Scan Task created on Core 0");
    
    // Create system monitoring task
    xTaskCreatePinnedToCore(
        system_task,
        "System_Task",
        SYSTEM_TASK_STACK_SIZE,
        NULL, 
        SYSTEM_TASK_PRIORITY,
        &system_task_handle,
        0                           // Core 0 (PRO CPU)  
    );
    Serial.println("✅ System Task created on Core 0");
    
    Serial.println("🎯 All tasks created successfully!");
    Serial.println("📊 Task distribution:");
    Serial.println("   Core 0 (PRO): AI, Scan, System tasks");
    Serial.println("   Core 1 (APP): UI task");
}

// Task implementations are in separate files
extern void ui_task(void* parameter);
extern void ai_task(void* parameter);  
extern void scan_task(void* parameter);
extern void system_task(void* parameter);