#ifndef AI_STATES_H
#define AI_STATES_H

/**
 * @brief AI State enumeration for different Ponagotchi behaviors
 */
typedef enum {
    AI_STATE_IDLE = 0,      // Default peaceful state
    AI_STATE_SNIFFING,      // High WiFi activity detected
    AI_STATE_TRACKING,      // Strong BLE devices nearby  
    AI_STATE_LEARNING,      // Processing captured data
    AI_STATE_EXCITED,       // Found interesting networks
    AI_STATE_SLEEPING,      // Low activity mode
    AI_STATE_ERROR,         // System error detected
    AI_STATE_UPDATING       // OTA or system update
} ai_state_t;

/**
 * @brief Structure to hold sensor and network data for AI inference
 */
typedef struct {
    uint16_t wifi_networks_count;    // Number of WiFi networks detected
    int16_t  wifi_signal_strength;   // Average WiFi signal strength
    uint16_t ble_devices_count;      // Number of BLE devices detected
    int16_t  ble_signal_strength;    // Average BLE signal strength
    uint32_t free_memory;            // Available free memory in bytes
    uint32_t uptime_seconds;         // System uptime
    bool     sd_card_present;        // SD card availability
    bool     user_interaction;       // Recent user input detected
} sensor_data_t;

/**
 * @brief AI inference function to determine current state
 * @param data Sensor and network data
 * @return Current AI state
 */
ai_state_t infer_ai_state(const sensor_data_t* data);

/**
 * @brief Get string representation of AI state
 * @param state AI state enumeration
 * @return String description of the state
 */
const char* ai_state_to_string(ai_state_t state);

/**
 * @brief Get mood emoji for current AI state
 * @param state AI state enumeration  
 * @return Unicode emoji character
 */
const char* ai_state_to_emoji(ai_state_t state);

#endif // AI_STATES_H
#ifndef AI_STATES_H
#define AI_STATES_H

#include <Arduino.h>

// AI State enumeration
typedef enum {
    AI_STATE_IDLE = 0,
    AI_STATE_SNIFFING,
    AI_STATE_TRACKING,
    AI_STATE_LEARNING,
    AI_STATE_EXCITED,
    AI_STATE_SLEEPING,
    AI_STATE_ERROR,
    AI_STATE_UPDATING
} ai_state_t;

// Sensor data structure
typedef struct {
    uint32_t wifi_networks_count;
    int32_t wifi_signal_strength;
    uint32_t ble_devices_count;
    int32_t ble_signal_strength;
    uint32_t free_memory;
    uint32_t uptime_seconds;
    bool user_interaction;
    bool sd_card_present;
} sensor_data_t;

// Function declarations
ai_state_t infer_ai_state(const sensor_data_t* data);
const char* ai_state_to_string(ai_state_t state);

#endif // AI_STATES_H
