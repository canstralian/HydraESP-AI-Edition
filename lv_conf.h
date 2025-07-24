
/**
 * @file lv_conf.h
 * LVGL configuration file
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#define LV_CONF_INCLUDE_SIMPLE 1

/*====================
   COLOR SETTINGS
 *====================*/
#define LV_COLOR_DEPTH     16
#define LV_COLOR_16_SWAP   0

/*=========================
   MEMORY SETTINGS
 *=========================*/
#define LV_MEM_CUSTOM      1
#define LV_MEM_SIZE        (32U * 1024U)

/*====================
   HAL SETTINGS
 *====================*/
#define LV_DISP_DEF_REFR_PERIOD    30
#define LV_INDEV_DEF_READ_PERIOD   30

/*========================
 * FONT USAGE
 *=======================*/
#define LV_FONT_MONTSERRAT_8     0
#define LV_FONT_MONTSERRAT_10    0
#define LV_FONT_MONTSERRAT_12    1
#define LV_FONT_MONTSERRAT_14    1
#define LV_FONT_MONTSERRAT_16    1
#define LV_FONT_MONTSERRAT_18    0
#define LV_FONT_MONTSERRAT_20    0
#define LV_FONT_MONTSERRAT_22    0
#define LV_FONT_MONTSERRAT_24    0
#define LV_FONT_MONTSERRAT_26    0
#define LV_FONT_MONTSERRAT_28    0
#define LV_FONT_MONTSERRAT_30    0
#define LV_FONT_MONTSERRAT_32    0
#define LV_FONT_MONTSERRAT_34    0
#define LV_FONT_MONTSERRAT_36    0
#define LV_FONT_MONTSERRAT_38    0
#define LV_FONT_MONTSERRAT_40    0
#define LV_FONT_MONTSERRAT_42    0
#define LV_FONT_MONTSERRAT_44    0
#define LV_FONT_MONTSERRAT_46    0
#define LV_FONT_MONTSERRAT_48    1

/*=================
 * TEXT SETTINGS
 *================*/
#define LV_TXT_ENC LV_TXT_ENC_UTF8

/*=========================
 * WIDGET USAGE
 *========================*/
#define LV_USE_ARC        1
#define LV_USE_BAR        1
#define LV_USE_BTN        1
#define LV_USE_BTNMATRIX  1
#define LV_USE_CANVAS     1
#define LV_USE_CHECKBOX   1
#define LV_USE_DROPDOWN   1
#define LV_USE_IMG        1
#define LV_USE_LABEL      1
#define LV_USE_LINE       1
#define LV_USE_ROLLER     1
#define LV_USE_SLIDER     1
#define LV_USE_SWITCH     1
#define LV_USE_TEXTAREA   1
#define LV_USE_TABLE      1

/*==================
 * EXAMPLES
 *==================*/
#define LV_BUILD_EXAMPLES 0

/*==================
 * DEMOS
 *==================*/
#define LV_USE_DEMO_WIDGETS    0
#define LV_USE_DEMO_KEYPAD_AND_ENCODER     0
#define LV_USE_DEMO_BENCHMARK  0
#define LV_USE_DEMO_STRESS     0
#define LV_USE_DEMO_MUSIC      0

#endif /*LV_CONF_H*/
