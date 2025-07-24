#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

#include <stdint.h>

/**
 * @brief System health metrics structure
 */
typedef struct {
    uint32_t free_heap_size;         // Free heap memory in bytes
    uint32_t free_psram_size;        // Free PSRAM in bytes  
    uint32_t min_free_heap;          // Minimum free heap since boot
    uint8_t  cpu_usage_percent;      // CPU usage percentage
    float    temperature_celsius;     // CPU temperature
    uint32_t uptime_ms;              // System uptime in milliseconds
    uint16_t task_count;             // Number of active tasks
    bool     wifi_connected;         // WiFi connection status
    bool     sd_card_mounted;        // SD card mount status
} system_metrics_t;

/**
 * @brief Initialize system monitoring
 * @return true on success, false on failure
 */
bool system_monitor_init(void);

/**
 * @brief Get current system metrics
 * @param metrics Pointer to store metrics
 */
void system_monitor_get_metrics(system_metrics_t* metrics);

/**
 * @brief Log system metrics to serial and SD card
 */
void system_monitor_log_metrics(void);

/**
 * @brief Check if system is in critical state
 * @return true if system needs attention
 */
bool system_monitor_is_critical(void);

/**
 * @brief Update status LED based on system health
 */
void system_monitor_update_status_led(void);

#endif // SYSTEM_MONITOR_H
#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

#include <Arduino.h>

// System metrics structure
typedef struct {
    uint32_t free_heap_size;
    uint32_t free_psram_size;
    uint32_t min_free_heap;
    uint32_t uptime_ms;
    uint32_t task_count;
    float temperature_celsius;
    uint8_t cpu_usage_percent;
    bool wifi_connected;
    bool sd_card_mounted;
} system_metrics_t;

// Function declarations
bool system_monitor_init(void);
bool system_monitor_is_critical(void);
void system_monitor_get_metrics(system_metrics_t* metrics);

#endif // SYSTEM_MONITOR_H
