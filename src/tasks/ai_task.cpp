/**
 * @file ai_task.cpp
 * @brief AI inference task with behavioral analysis
 */

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include "config.h"
#include "ai_states.h"

// External variables
extern ai_state_t current_ai_state;
extern sensor_data_t global_sensor_data;
extern SemaphoreHandle_t sensor_data_mutex;
extern QueueHandle_t ai_state_queue;

// Internal state tracking
static ai_state_t previous_state = AI_STATE_IDLE;
static uint32_t state_duration = 0;
static uint32_t excitement_level = 0;
static uint32_t learning_progress = 0;

// Forward declarations
ai_state_t analyze_behavior(const sensor_data_t* data);
void update_learning_metrics(ai_state_t new_state);
void log_state_change(ai_state_t old_state, ai_state_t new_state);

/**
 * @brief AI Task - performs behavioral inference and state management
 */
void ai_task(void* parameter) {
    Serial.println("🧠 AI Task started");
    
    TickType_t last_wake_time = xTaskGetTickCount();
    sensor_data_t local_sensor_data;
    
    while (true) {
        // Get current sensor data safely
        if (xSemaphoreTake(sensor_data_mutex, pdMS_TO_TICKS(50)) == pdTRUE) {
            memcpy(&local_sensor_data, &global_sensor_data, sizeof(sensor_data_t));
            xSemaphoreGive(sensor_data_mutex);
        } else {
            Serial.println("⚠️  AI Task: Failed to get sensor data");
            vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(AI_UPDATE_INTERVAL));
            continue;
        }
        
        // Perform AI inference
        ai_state_t new_state = analyze_behavior(&local_sensor_data);
        
        // Check if state changed
        if (new_state != current_ai_state) {
            log_state_change(current_ai_state, new_state);
            
            // Send state change to UI task
            if (xQueueSend(ai_state_queue, &new_state, pdMS_TO_TICKS(10)) != pdTRUE) {
                Serial.println("⚠️  Failed to send AI state to UI");
            }
            
            // Update learning metrics
            update_learning_metrics(new_state);
            
            previous_state = current_ai_state;
            current_ai_state = new_state;
            state_duration = 0;
        } else {
            state_duration += AI_UPDATE_INTERVAL;
        }
        
        // Log AI metrics periodically
        static uint32_t last_log_time = 0;
        if (millis() - last_log_time > 30000) { // Every 30 seconds
            Serial.printf("🧠 AI Metrics: State=%s, Duration=%lums, Excitement=%lu, Learning=%lu\n",
                         ai_state_to_string(current_ai_state), state_duration, 
                         excitement_level, learning_progress);
            last_log_time = millis();
        }
        
        // Sleep until next inference cycle
        vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(AI_UPDATE_INTERVAL));
    }
}

/**
 * @brief Analyze sensor data and determine AI behavior state
 */
ai_state_t analyze_behavior(const sensor_data_t* data) {
    // Check for error conditions first
    if (data->free_memory < LOW_MEMORY_THRESHOLD) {
        return AI_STATE_ERROR;
    }
    
    // Check for high activity states
    if (data->wifi_networks_count >= HIGH_WIFI_ACTIVITY_THRESHOLD) {
        excitement_level = min(excitement_level + 5, 100);
        
        if (excitement_level > 80) {
            return AI_STATE_EXCITED;
        } else {
            return AI_STATE_SNIFFING;
        }
    }
    
    // Check for BLE tracking mode
    if (data->ble_devices_count > 5 && data->ble_signal_strength > STRONG_BLE_SIGNAL_THRESHOLD) {
        return AI_STATE_TRACKING;
    }
    
    // Learning state when processing data
    if (current_ai_state == AI_STATE_SNIFFING && state_duration > 5000) {
        learning_progress = min(learning_progress + 10, 100);
        return AI_STATE_LEARNING;
    }
    
    // Sleep state during low activity
    if (data->wifi_networks_count == 0 && data->ble_devices_count == 0 && 
        !data->user_interaction && state_duration > 60000) {
        excitement_level = max((int32_t)excitement_level - 2, 0);
        return AI_STATE_SLEEPING;
    }
    
    // Gradually reduce excitement over time
    if (millis() % 10000 == 0) { // Every 10 seconds
        excitement_level = max((int32_t)excitement_level - 1, 0);
    }
    
    // Default to idle state
    return AI_STATE_IDLE;
}

/**
 * @brief Update learning metrics based on state transitions
 */
void update_learning_metrics(ai_state_t new_state) {
    switch (new_state) {
        case AI_STATE_LEARNING:
            learning_progress += 5;
            break;
            
        case AI_STATE_EXCITED:
            excitement_level = min(excitement_level + 10, 100);
            break;
            
        case AI_STATE_SLEEPING:
            // Reset some metrics during sleep
            if (learning_progress > 20) {
                learning_progress -= 2;
            }
            break;
            
        default:
            break;
    }
    
    // Cap metrics at reasonable values
    learning_progress = min(learning_progress, 100);
    excitement_level = min(excitement_level, 100);
}

/**
 * @brief Log state changes for debugging and analysis
 */
void log_state_change(ai_state_t old_state, ai_state_t new_state) {
    Serial.printf("🧠 AI State Change: %s -> %s (Duration: %lums)\n",
                  ai_state_to_string(old_state),
                  ai_state_to_string(new_state),
                  state_duration);
    
    // TODO: Log to SD card if available for long-term analysis
}