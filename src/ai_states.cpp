/**
 * @file ai_states.cpp
 * @brief AI state inference and behavior implementation
 */

#include "ai_states.h"
#include "config.h"

/**
 * @brief Convert AI state enum to human-readable string
 */
const char* ai_state_to_string(ai_state_t state) {
    switch (state) {
        case AI_STATE_IDLE: return "Idle";
        case AI_STATE_SNIFFING: return "Sniffing";
        case AI_STATE_TRACKING: return "Tracking";
        case AI_STATE_LEARNING: return "Learning";
        case AI_STATE_EXCITED: return "Excited";
        case AI_STATE_SLEEPING: return "Sleeping";
        case AI_STATE_ERROR: return "Error";
        case AI_STATE_UPDATING: return "Updating";
        default: return "Unknown";
    }
}

/**
 * @brief AI inference function to determine current state
 */
ai_state_t infer_ai_state(const sensor_data_t* data) {
    if (data == NULL) {
        return AI_STATE_ERROR;}
    
    // Check for system error conditions first
    if (data->free_memory < LOW_MEMORY_THRESHOLD) {
        return AI_STATE_ERROR;
    }
    
    // High activity detection - WiFi networks
    if (data->wifi_networks_count >= HIGH_WIFI_ACTIVITY_THRESHOLD) {
        // Strong signals indicate close proximity activity
        if (data->wifi_signal_strength > -40) {
            return AI_STATE_EXCITED;
        } else {
            return AI_STATE_SNIFFING;
        }
    }
    
    // BLE tracking mode - strong nearby devices
    if (data->ble_devices_count > 5 && data->ble_signal_strength > STRONG_BLE_SIGNAL_THRESHOLD) {
        return AI_STATE_TRACKING;
    }
    
    // Learning mode - moderate activity with recent interaction
    if ((data->wifi_networks_count > 3 || data->ble_devices_count > 2) && 
        data->user_interaction) {
        return AI_STATE_LEARNING;
    }
    
    // Sleep mode - very low activity for extended time
    if (data->wifi_networks_count == 0 && data->ble_devices_count == 0 && 
        !data->user_interaction && data->uptime_seconds > 300) {
        return AI_STATE_SLEEPING;
    }
    
    // System update mode
    if (data->uptime_seconds < 60) {
        return AI_STATE_UPDATING;
    }
    
    // Default idle state
    return AI_STATE_IDLE;
}

/**
 * @brief Get string representation of AI state
 */
const char* ai_state_to_string(ai_state_t state) {
    switch (state) {
        case AI_STATE_IDLE:     return "Idle";
        case AI_STATE_SNIFFING: return "Sniffing";
        case AI_STATE_TRACKING: return "Tracking";
        case AI_STATE_LEARNING: return "Learning";
        case AI_STATE_EXCITED:  return "Excited";
        case AI_STATE_SLEEPING: return "Sleeping";
        case AI_STATE_ERROR:    return "Error";
        case AI_STATE_UPDATING: return "Updating";
        default:                return "Unknown";
    }
}

/**
 * @brief Get mood emoji for current AI state
 */
const char* ai_state_to_emoji(ai_state_t state) {
    switch (state) {
        case AI_STATE_IDLE:     return "😊";
        case AI_STATE_SNIFFING: return "👃";
        case AI_STATE_TRACKING: return "👁️";
        case AI_STATE_LEARNING: return "🧠";
        case AI_STATE_EXCITED:  return "🤩";
        case AI_STATE_SLEEPING: return "😴";
        case AI_STATE_ERROR:    return "💀";
        case AI_STATE_UPDATING: return "🔄";
        default:                return "🤔";
    }
}