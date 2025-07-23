/**
 * @file display_driver.cpp
 * @brief ST7789 display driver with LVGL integration for HydraESP
 */

#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include "config.h"

// TFT_eSPI instance
TFT_eSPI tft = TFT_eSPI();

// LVGL display buffer - use PSRAM if available
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *buf1;
static lv_color_t *buf2;
static lv_disp_drv_t disp_drv;

// Forward declarations
static void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
static void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);

/**
 * @brief Initialize display driver and LVGL
 * @return true on success, false on failure
 */
bool display_driver_init(void) {
    Serial.println("🖥️  Initializing ST7789 display driver...");
    
    // Initialize TFT display
    tft.init();
    tft.setRotation(1); // Landscape mode
    tft.fillScreen(TFT_BLACK);
    
    // Turn on backlight
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
    
    Serial.printf("✅ Display initialized: %dx%d pixels\n", TFT_WIDTH, TFT_HEIGHT);
    
    // Initialize LVGL
    lv_init();
    
    // Allocate display buffers - prefer PSRAM for large buffers
    size_t buffer_size = TFT_WIDTH * TFT_HEIGHT / 10; // 1/10 screen size
    
    if (psramFound()) {
        buf1 = (lv_color_t*)ps_malloc(buffer_size * sizeof(lv_color_t));
        buf2 = (lv_color_t*)ps_malloc(buffer_size * sizeof(lv_color_t));
        Serial.println("✅ Display buffers allocated in PSRAM");
    } else {
        buf1 = (lv_color_t*)malloc(buffer_size * sizeof(lv_color_t));
        buf2 = (lv_color_t*)malloc(buffer_size * sizeof(lv_color_t));
        Serial.println("⚠️  Display buffers allocated in internal RAM");
    }
    
    if (buf1 == NULL || buf2 == NULL) {
        Serial.println("❌ Failed to allocate display buffers");
        return false;
    }
    
    // Initialize display buffer
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, buffer_size);
    
    // Initialize display driver
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = TFT_WIDTH;
    disp_drv.ver_res = TFT_HEIGHT;
    disp_drv.flush_cb = disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
    
    // Initialize input device (touchpad) if available
    #ifdef TOUCH_CS
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    lv_indev_drv_register(&indev_drv);
    Serial.println("✅ Touchpad initialized");
    #endif
    
    Serial.println("🎨 LVGL initialized successfully");
    return true;
}

/**
 * @brief LVGL display flush callback
 */
static void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t*)&color_p->full, w * h, true);
    tft.endWrite();
    
    lv_disp_flush_ready(disp);
}

/**
 * @brief LVGL touchpad read callback (if touch is available)
 */
static void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
    #ifdef TOUCH_CS
    // Implement touch reading logic here
    // For now, just set as not pressed
    data->state = LV_INDEV_STATE_REL;
    #endif
}

/**
 * @brief Update display - call from UI task
 */
void display_driver_update(void) {
    lv_timer_handler();
}

/**
 * @brief Set display brightness
 * @param brightness Brightness level (0-255)
 */
void display_driver_set_brightness(uint8_t brightness) {
    // For simple on/off backlight control
    digitalWrite(TFT_BL, brightness > 127 ? HIGH : LOW);
    
    // TODO: Implement PWM brightness control if needed
    // ledcWrite(backlight_channel, brightness);
}

/**
 * @brief Get display dimensions
 */
void display_driver_get_dimensions(uint16_t* width, uint16_t* height) {
    *width = TFT_WIDTH;
    *height = TFT_HEIGHT;
}