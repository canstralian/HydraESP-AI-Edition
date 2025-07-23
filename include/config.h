#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Hardware Configuration - CYD ST7789 Display
#define TFT_WIDTH  320
#define TFT_HEIGHT 240
#define TFT_CS     15
#define TFT_DC     2
#define TFT_RST    -1
#define TFT_BL     21
#define TFT_MOSI   13
#define TFT_CLK    14

// Touch pins (if available)
#define TOUCH_CS   33
#define TOUCH_IRQ  36

// Status LED
#define STATUS_LED_PIN 22

// I2C pins for sensors
#define I2C_SDA 21
#define I2C_SCL 22

// SD Card pins
#define SD_CS   5
#define SD_MOSI 23
#define SD_MISO 19
#define SD_CLK  18

// System Configuration
#define SYSTEM_TASK_STACK_SIZE 4096
#define UI_TASK_STACK_SIZE     8192
#define AI_TASK_STACK_SIZE     6144
#define SCAN_TASK_STACK_SIZE   4096

// Task Priorities
#define SYSTEM_TASK_PRIORITY   1
#define SCAN_TASK_PRIORITY     1
#define UI_TASK_PRIORITY       2
#define AI_TASK_PRIORITY       3

// Timing Configuration (in milliseconds)
#define UI_UPDATE_INTERVAL     30
#define AI_UPDATE_INTERVAL     200
#define SCAN_INTERVAL          5000
#define SYSTEM_MONITOR_INTERVAL 1000

// AI State Thresholds
#define HIGH_WIFI_ACTIVITY_THRESHOLD  10
#define STRONG_BLE_SIGNAL_THRESHOLD   -50
#define LOW_MEMORY_THRESHOLD          10240

// WiFi & BLE Configuration
#define MAX_WIFI_NETWORKS  50
#define MAX_BLE_DEVICES    30
#define SCAN_TIME_SECONDS  10

#endif // CONFIG_H