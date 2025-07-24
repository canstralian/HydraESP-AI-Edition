/**
 * @file ui_task.cpp
 * @brief UI task with LVGL and animated Ponagotchi-style face
 */

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <lvgl.h>
#include "config.h"
#include "ai_states.h"

// External variables
extern ai_state_t current_ai_state;
extern QueueHandle_t ai_state_queue;
extern sensor_data_t global_sensor_data;
extern SemaphoreHandle_t sensor_data_mutex;

// Forward declarations
bool display_driver_init(void);
void display_driver_update(void);
void create_ponagotchi_ui(void);
void update_face_expression(ai_state_t state);
void update_status_bar(void);
void create_main_screen(void);

// LVGL objects
static lv_obj_t* main_screen = NULL;
static lv_obj_t* face_container = NULL;
static lv_obj_t* left_eye = NULL;
static lv_obj_t* right_eye = NULL;
static lv_obj_t* mouth = NULL;
static lv_obj_t* status_label = NULL;
static lv_obj_t* stats_label = NULL;
static lv_obj_t* network_label = NULL;

// Animation variables
static lv_anim_t blink_anim;
static uint32_t last_blink_time = 0;
static uint32_t last_expression_change = 0;

/**
 * @brief UI Task - handles LVGL updates and face animations
 */
void ui_task(void* parameter) {
    Serial.println("🎨 UI Task started");
    
    // Initialize display driver and LVGL
    if (!display_driver_init()) {
        Serial.println("❌ Display driver initialization failed");
        vTaskDelete(NULL);
        return;
    }
    
    // Create Ponagotchi UI
    create_ponagotchi_ui();
    
    ai_state_t new_state;
    TickType_t last_wake_time = xTaskGetTickCount();
    
    while (true) {
        // Check for AI state updates
        if (xQueueReceive(ai_state_queue, &new_state, 0) == pdTRUE) {
            current_ai_state = new_state;
            update_face_expression(new_state);
            last_expression_change = millis();
            
            Serial.printf("🎭 Face expression changed to: %s\n", 
                         ai_state_to_string(new_state));
        }
        
        // Update status information
        update_status_bar();
        
        // Handle automatic blinking animation
        uint32_t current_time = millis();
        if (current_time - last_blink_time > random(2000, 5000)) {
            // Trigger blink animation
            last_blink_time = current_time;
            // TODO: Implement blink animation
        }
        
        // Update LVGL
        display_driver_update();
        
        // Sleep until next update cycle
        vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(UI_UPDATE_INTERVAL));
    }
}

/**
 * @brief Create the main Ponagotchi UI layout
 */
void create_ponagotchi_ui(void) {
    Serial.println("🎨 Creating Ponagotchi UI...");
    
    // Create main screen
    main_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(main_screen, lv_color_black(), 0);
    lv_scr_load(main_screen);
    
    // Create face container (center of screen)
    face_container = lv_obj_create(main_screen);
    lv_obj_set_size(face_container, 200, 150);
    lv_obj_center(face_container);
    lv_obj_set_style_bg_color(face_container, lv_color_black(), 0);
    lv_obj_set_style_border_width(face_container, 0, 0);
    
    // Create eyes
    left_eye = lv_obj_create(face_container);
    lv_obj_set_size(left_eye, 30, 30);
    lv_obj_set_pos(left_eye, 50, 40);
    lv_obj_set_style_bg_color(left_eye, lv_color_white(), 0);
    lv_obj_set_style_radius(left_eye, 15, 0);
    
    right_eye = lv_obj_create(face_container);
    lv_obj_set_size(right_eye, 30, 30);
    lv_obj_set_pos(right_eye, 120, 40);
    lv_obj_set_style_bg_color(right_eye, lv_color_white(), 0);
    lv_obj_set_style_radius(right_eye, 15, 0);
    
    // Create mouth
    mouth = lv_obj_create(face_container);
    lv_obj_set_size(mouth, 60, 20);
    lv_obj_set_pos(mouth, 70, 90);
    lv_obj_set_style_bg_color(mouth, lv_color_white(), 0);
    lv_obj_set_style_radius(mouth, 10, 0);
    
    // Create status label (top of screen)
    status_label = lv_label_create(main_screen);
    lv_label_set_text(status_label, "🧠 HydraESP AI - Idle");
    lv_obj_set_style_text_color(status_label, lv_color_white(), 0);
    lv_obj_set_pos(status_label, 10, 10);
    
    // Create stats label (bottom left)
    stats_label = lv_label_create(main_screen);
    lv_label_set_text(stats_label, "Memory: OK\nUptime: 0s");
    lv_obj_set_style_text_color(stats_label, lv_color_hex(0x00FF00), 0);
    lv_obj_set_style_text_font(stats_label, &lv_font_montserrat_14, 0);
    lv_obj_set_pos(stats_label, 10, 180);
    
    // Create network stats label (bottom right)
    network_label = lv_label_create(main_screen);
    lv_label_set_text(network_label, "WiFi: 0\nBLE: 0");
    lv_obj_set_style_text_color(network_label, lv_color_hex(0x00FFFF), 0);
    lv_obj_set_style_text_font(network_label, &lv_font_montserrat_12, 0);
    lv_obj_set_pos(network_label, 250, 180);
    
    Serial.println("✅ Ponagotchi UI created successfully");
}

/**
 * @brief Update face expression based on AI state
 */
void update_face_expression(ai_state_t state) {
    if (face_container == NULL) return;
    
    switch (state) {
        case AI_STATE_IDLE:
            // Normal round eyes, small smile
            lv_obj_set_size(left_eye, 30, 30);
            lv_obj_set_size(right_eye, 30, 30);
            lv_obj_set_style_bg_color(mouth, lv_color_white(), 0);
            lv_obj_set_size(mouth, 40, 15);
            break;
            
        case AI_STATE_SNIFFING:
            // Wide eyes, focused expression
            lv_obj_set_size(left_eye, 35, 35);
            lv_obj_set_size(right_eye, 35, 35);
            lv_obj_set_style_bg_color(mouth, lv_color_hex(0xFFFF00), 0);
            lv_obj_set_size(mouth, 30, 10);
            break;
            
        case AI_STATE_EXCITED:
            // Big round eyes, big smile
            lv_obj_set_size(left_eye, 40, 40);
            lv_obj_set_size(right_eye, 40, 40);
            lv_obj_set_style_bg_color(mouth, lv_color_hex(0x00FF00), 0);
            lv_obj_set_size(mouth, 70, 25);
            break;
            
        case AI_STATE_LEARNING:
            // Squinted eyes, thinking
            lv_obj_set_size(left_eye, 25, 15);
            lv_obj_set_size(right_eye, 25, 15);
            lv_obj_set_style_bg_color(mouth, lv_color_hex(0xFF8800), 0);
            lv_obj_set_size(mouth, 35, 12);
            break;
            
        case AI_STATE_SLEEPING:
            // Closed eyes, peaceful
            lv_obj_set_size(left_eye, 30, 5);
            lv_obj_set_size(right_eye, 30, 5);
            lv_obj_set_style_bg_color(mouth, lv_color_hex(0x8888FF), 0);
            lv_obj_set_size(mouth, 25, 8);
            break;
            
        case AI_STATE_ERROR:
            // X eyes, frown
            lv_obj_set_size(left_eye, 20, 20);
            lv_obj_set_size(right_eye, 20, 20);
            lv_obj_set_style_bg_color(mouth, lv_color_hex(0xFF0000), 0);
            lv_obj_set_size(mouth, 50, 8);
            break;
            
        default:
            break;
    }
}

/**
 * @brief Update status bar with current information
 */
void update_status_bar(void) {
    if (status_label == NULL || stats_label == NULL || network_label == NULL) return;
    
    // Update main status
    static char status_text[64];
    snprintf(status_text, sizeof(status_text), "%s %s", 
             ai_state_to_emoji(current_ai_state),
             ai_state_to_string(current_ai_state));
    lv_label_set_text(status_label, status_text);
    
    // Update system stats (thread-safe)
    if (xSemaphoreTake(sensor_data_mutex, pdMS_TO_TICKS(10)) == pdTRUE) {
        static char stats_text[64];
        snprintf(stats_text, sizeof(stats_text), 
                "Mem: %luKB\nUp: %lus", 
                global_sensor_data.free_memory / 1024,
                global_sensor_data.uptime_seconds);
        lv_label_set_text(stats_label, stats_text);
        
        // Update network stats
        static char network_text[64];
        snprintf(network_text, sizeof(network_text),
                "WiFi: %d\nBLE: %d",
                global_sensor_data.wifi_networks_count,
                global_sensor_data.ble_devices_count);
        lv_label_set_text(network_label, network_text);
        
        xSemaphoreGive(sensor_data_mutex);
    }
}
/**
 * @file ui_task.cpp
 * @brief UI task with LVGL integration
 */

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include "config.h"
#include "ai_states.h"

// External variables
extern ai_state_t current_ai_state;
extern QueueHandle_t ai_state_queue;

// LVGL objects
static lv_obj_t* main_screen = NULL;
static lv_obj_t* face_label = NULL;
static lv_obj_t* stats_label = NULL;

// TFT instance
TFT_eSPI tft = TFT_eSPI();

// LVGL display buffer
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[TFT_WIDTH * 10];

// Forward declarations
void display_flush(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p);
void create_ponagotchi_ui(void);
void update_face_expression(ai_state_t state);
void update_stats_display(void);

/**
 * @brief UI Task - handles display and animations
 */
void ui_task(void* parameter) {
    Serial.println("🎨 UI Task started");
    
    // Initialize TFT display
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    
    // Initialize LVGL
    lv_init();
    
    // Initialize display buffer
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, TFT_WIDTH * 10);
    
    // Initialize display driver
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = TFT_WIDTH;
    disp_drv.ver_res = TFT_HEIGHT;
    disp_drv.flush_cb = display_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
    
    // Create UI
    create_ponagotchi_ui();
    
    TickType_t last_wake_time = xTaskGetTickCount();
    ai_state_t new_state;
    
    while (true) {
        // Check for AI state updates
        if (xQueueReceive(ai_state_queue, &new_state, 0) == pdTRUE) {
            update_face_expression(new_state);
        }
        
        // Update statistics display
        update_stats_display();
        
        // Handle LVGL tasks
        lv_timer_handler();
        
        // Sleep until next update
        vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(UI_UPDATE_INTERVAL));
    }
}

/**
 * @brief LVGL display flush callback
 */
void display_flush(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t*)&color_p->full, w * h, true);
    tft.endWrite();
    
    lv_disp_flush_ready(disp);
}

/**
 * @brief Create Ponagotchi-style UI
 */
void create_ponagotchi_ui(void) {
    // Create main screen
    main_screen = lv_scr_act();
    lv_obj_set_style_bg_color(main_screen, lv_color_black(), 0);
    
    // Create face label (centered)
    face_label = lv_label_create(main_screen);
    lv_obj_set_style_text_font(face_label, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_color(face_label, lv_color_white(), 0);
    lv_obj_align(face_label, LV_ALIGN_CENTER, 0, -20);
    lv_label_set_text(face_label, "😊");
    
    // Create stats label (bottom)
    stats_label = lv_label_create(main_screen);
    lv_obj_set_style_text_font(stats_label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(stats_label, lv_color_white(), 0);
    lv_obj_align(stats_label, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_label_set_text(stats_label, "Initializing...");
}

/**
 * @brief Update face expression based on AI state
 */
void update_face_expression(ai_state_t state) {
    const char* face = "😊";  // Default idle face
    
    switch (state) {
        case AI_STATE_IDLE:
            face = "😊";
            break;
        case AI_STATE_SNIFFING:
            face = "👃";
            break;
        case AI_STATE_TRACKING:
            face = "👁️";
            break;
        case AI_STATE_LEARNING:
            face = "🧠";
            break;
        case AI_STATE_EXCITED:
            face = "🤩";
            break;
        case AI_STATE_SLEEPING:
            face = "😴";
            break;
        case AI_STATE_ERROR:
            face = "💀";
            break;
        case AI_STATE_UPDATING:
            face = "🔄";
            break;
    }
    
    if (face_label) {
        lv_label_set_text(face_label, face);
    }
}

/**
 * @brief Update statistics display
 */
void update_stats_display(void) {
    static uint32_t last_update = 0;
    
    if (millis() - last_update > 1000) {  // Update every second
        char stats_text[100];
        snprintf(stats_text, sizeof(stats_text), 
                "State: %s | Memory: %d KB | Uptime: %d s",
                ai_state_to_string(current_ai_state),
                ESP.getFreeHeap() / 1024,
                millis() / 1000);
        
        if (stats_label) {
            lv_label_set_text(stats_label, stats_text);
        }
        
        last_update = millis();
    }
}
