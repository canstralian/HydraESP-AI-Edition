
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
/**
 * @file lv_conf.h
 * LVGL configuration file
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/*====================
   COLOR SETTINGS
 *====================*/

/* Color depth: 1 (1 byte per pixel), 8 (RGB332), 16 (RGB565), 32 (ARGB8888) */
#define LV_COLOR_DEPTH 16

/*=========================
   MEMORY SETTINGS
 *=========================*/

/* Size of the memory available for `lv_mem_alloc()` in bytes (>= 2kB) */
#define LV_MEM_SIZE (32U * 1024U)      /* [bytes] */

/*====================
   HAL SETTINGS
 *====================*/

/* Default display refresh period. LittlevGL will redraw changed areas with this period time */
#define LV_DISP_DEF_REFR_PERIOD 30      /* [ms] */

/* Input device read period in milliseconds */
#define LV_INDEV_DEF_READ_PERIOD 30     /* [ms] */

/*========================
   FEATURE CONFIGURATION
 *========================*/

/* 1: Enable the Animations */
#define LV_USE_ANIMATION 1

/* 1: Enable shadow drawing on rectangles */
#define LV_USE_SHADOW 1

/* 1: Use other blend modes than normal (`LV_BLEND_MODE_...`) */
#define LV_USE_BLEND_MODES 1

/* 1: Use the `opa_scale` style property to set the opacity of an object and its children at once */
#define LV_USE_OPA_SCALE 1

/* 1: Use image zoom and rotation */
#define LV_USE_IMG_TRANSFORM 1

/* 1: Enable object groups (for keyboard/encoder navigation) */
#define LV_USE_GROUP 1

/* 1: Enable GPU interface */
#define LV_USE_GPU 0

/*================
 * FONT USAGE
 *================*/

/* Montserrat fonts with various sizes and features.
 * The compressed options are more resource efficient. */
#define LV_FONT_MONTSERRAT_8  0
#define LV_FONT_MONTSERRAT_10 0
#define LV_FONT_MONTSERRAT_12 1
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_16 1
#define LV_FONT_MONTSERRAT_18 0
#define LV_FONT_MONTSERRAT_20 0
#define LV_FONT_MONTSERRAT_22 0
#define LV_FONT_MONTSERRAT_24 0
#define LV_FONT_MONTSERRAT_26 0
#define LV_FONT_MONTSERRAT_28 0
#define LV_FONT_MONTSERRAT_30 0
#define LV_FONT_MONTSERRAT_32 0
#define LV_FONT_MONTSERRAT_34 0
#define LV_FONT_MONTSERRAT_36 0
#define LV_FONT_MONTSERRAT_38 0
#define LV_FONT_MONTSERRAT_40 0
#define LV_FONT_MONTSERRAT_42 0
#define LV_FONT_MONTSERRAT_44 0
#define LV_FONT_MONTSERRAT_46 0
#define LV_FONT_MONTSERRAT_48 0

/* Demonstrate special features */
#define LV_FONT_MONTSERRAT_28_COMPRESSED 0  /* bpp = 3 */
#define LV_FONT_DEJAVU_16_PERSIAN_HEBREW 0  /* Hebrew, Arabic, Persian letters and all their forms */
#define LV_FONT_SIMSUN_16_CJK            0  /* 1000 most common CJK radicals */

/*Pixel perfect monospace fonts*/
#define LV_FONT_UNSCII_8  0
#define LV_FONT_UNSCII_16 0

/* Optionally declare custom fonts here.
 * You can use these fonts as default font too and they will be available globally.
 * E.g. #define LV_FONT_CUSTOM_DECLARE   LV_FONT_DECLARE(my_font_1) LV_FONT_DECLARE(my_font_2) */
#define LV_FONT_CUSTOM_DECLARE

/* Always set a default font */
#define LV_FONT_DEFAULT &lv_font_montserrat_14

/* Enable it if you have fonts with a lot of characters.
 * The limit depends on the font size, font face and bpp
 * but with > 10,000 characters if you see issues probably you need to enable it.*/
#define LV_FONT_FMT_TXT_LARGE 0

/*=================
 * TEXT SETTINGS
 *=================*/

/**
 * Select a character encoding for strings.
 * Your IDE or editor should have the same character encoding
 * - LV_TXT_ENC_UTF8
 * - LV_TXT_ENC_ASCII
 */
#define LV_TXT_ENC LV_TXT_ENC_UTF8

/*Can break (wrap) texts on these chars*/
#define LV_TXT_BREAK_CHARS " ,.;:-_"

/* If a word is at least this long, will break wherever "prettiest"
 * To disable, set to a value <= 0 */
#define LV_TXT_LINE_BREAK_LONG_LEN 0

/* Minimum number of characters in a long word to put on a line before a break.
 * Depends on LV_TXT_LINE_BREAK_LONG_LEN. */
#define LV_TXT_LINE_BREAK_LONG_PRE_MIN_LEN 3

/* Minimum number of characters in a long word to put on a line after a break.
 * Depends on LV_TXT_LINE_BREAK_LONG_LEN. */
#define LV_TXT_LINE_BREAK_LONG_POST_MIN_LEN 3

/* The control character to use for signalling text recoloring. */
#define LV_TXT_COLOR_CMD "#"

/* Support bidirectional texts. Allows mixing Left-to-Right and Right-to-Left texts.
 * The direction will be processed according to the Unicode Bidirectional Algorithm:
 * https://www.unicode.org/reports/tr9/*/
#define LV_USE_BIDI 0
#if LV_USE_BIDI
/* Set the default direction. Supported values:
 * `LV_BASE_DIR_LTR` Left-to-Right
 * `LV_BASE_DIR_RTL` Right-to-Left
 * `LV_BASE_DIR_AUTO` detect texts base direction */
#define LV_BIDI_BASE_DIR_DEF LV_BASE_DIR_AUTO
#endif

/* Enable Arabic/Persian processing
 * In these languages characters should be replaced with an other form based on their position in the text */
#define LV_USE_ARABIC_PERSIAN_CHARS 0

/*==================
 * WIDGET USAGE
 *==================*/

/* Documentation of the widgets: https://docs.lvgl.io/latest/en/html/widgets/index.html */

#define LV_USE_ARC        1
#define LV_USE_ANIMIMG    1
#define LV_USE_BAR        1
#define LV_USE_BTN        1
#define LV_USE_BTNMATRIX  1
#define LV_USE_CANVAS     1
#define LV_USE_CHECKBOX   1
#define LV_USE_DROPDOWN   1   /* Requires: lv_label */
#define LV_USE_IMG        1   /* Requires: lv_label */
#define LV_USE_LABEL      1
#define LV_USE_LINE       1
#define LV_USE_ROLLER     1   /* Requires: lv_label */
#define LV_USE_SLIDER     1   /* Requires: lv_bar */
#define LV_USE_SWITCH     1
#define LV_USE_TEXTAREA   1   /* Requires: lv_label */
#define LV_USE_TABLE      1   /* Requires: lv_label */

/*==================
 * EXAMPLES
 *==================*/

/* Enable the examples to be built with the library */
#define LV_BUILD_EXAMPLES 0

/*==================
 * THEME USAGE
 *==================*/

/* A simple, impressive and very complete theme */
#define LV_USE_THEME_DEFAULT 1
#if LV_USE_THEME_DEFAULT

/* 0: Light mode; 1: Dark mode */
#define LV_THEME_DEFAULT_DARK 1

/* 1: Enable grow on press */
#define LV_THEME_DEFAULT_GROW 1

/* Default transition time in [ms] */
#define LV_THEME_DEFAULT_TRANSITION_TIME 80
#endif /*LV_USE_THEME_DEFAULT*/

/* A very simple theme that is a good starting point for a custom theme */
#define LV_USE_THEME_BASIC 1

/* A theme designed for monochrome displays */
#define LV_USE_THEME_MONO 1

/*==================
 * LAYOUTS
 *==================*/

/* A layout similar to Flexbox in CSS. */
#define LV_USE_FLEX 1

/* A layout similar to Grid in CSS. */
#define LV_USE_GRID 1

/*==================
 * 3RD PARTY LIBRARIES
 *==================*/

/*File system interfaces for common APIs */

/*API for fopen, fread, etc */
#define LV_USE_FS_STDIO 0
#if LV_USE_FS_STDIO
#define LV_FS_STDIO_LETTER '\0'     /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
#define LV_FS_STDIO_PATH ""         /*Set the working directory. File/directory paths will be appended to it.*/
#define LV_FS_STDIO_CACHE_SIZE 0    /*>0 to cache this number of bytes in lv_fs_read()*/
#endif

/*API for open, read, etc */
#define LV_USE_FS_POSIX 0
#if LV_USE_FS_POSIX
#define LV_FS_POSIX_LETTER '\0'     /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
#define LV_FS_POSIX_PATH ""         /*Set the working directory. File/directory paths will be appended to it.*/
#define LV_FS_POSIX_CACHE_SIZE 0    /*>0 to cache this number of bytes in lv_fs_read()*/
#endif

/*API for CreateFile, ReadFile, etc*/
#define LV_USE_FS_WIN32 0
#if LV_USE_FS_WIN32
#define LV_FS_WIN32_LETTER '\0'     /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
#define LV_FS_WIN32_PATH ""         /*Set the working directory. File/directory paths will be appended to it.*/
#define LV_FS_WIN32_CACHE_SIZE 0    /*>0 to cache this number of bytes in lv_fs_read()*/
#endif

/*API for FATFS (needs to be added separately). Uses f_open, f_read, etc*/
#define LV_USE_FS_FATFS 0
#if LV_USE_FS_FATFS
#define LV_FS_FATFS_LETTER '\0'     /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
#define LV_FS_FATFS_CACHE_SIZE 0    /*>0 to cache this number of bytes in lv_fs_read()*/
#endif

/*PNG decoder library*/
#define LV_USE_PNG 0

/*BMP decoder library*/
#define LV_USE_BMP 0

/* JPG + split JPG decoder library.
 * Split JPG is a custom format optimized for embedded systems. */
#define LV_USE_SJPG 0

/*GIF decoder library*/
#define LV_USE_GIF 0

/*QR code library*/
#define LV_USE_QRCODE 0

/*FreeType library*/
#define LV_USE_FREETYPE 0
#if LV_USE_FREETYPE
/* FreeType has a lot of possible configurations.
 * Typical values are 16, 64, 256, but it can be any value. */
#define LV_FREETYPE_CACHE_SIZE 16
#endif

/*Rlottie library*/
#define LV_USE_RLOTTIE 0

/*FFmpeg library for image decoding and playing videos
 * Supports all major image formats so do not enable other image decoder with it */
#define LV_USE_FFMPEG 0
#if LV_USE_FFMPEG
#define LV_FFMPEG_AV_DUMP_FORMAT 0
#endif

/*==================
 * OTHERS
 *==================*/

/* 1: Enable API to take snapshot for object */
#define LV_USE_SNAPSHOT 0

/* 1: Enable Monkey test */
#define LV_USE_MONKEY 0

/* 1: Enable grid navigation */
#define LV_USE_GRIDNAV 0

/* 1: Enable lv_obj fragment */
#define LV_USE_FRAGMENT 0

/* 1: Support using images as font in label or span widgets */
#define LV_USE_IMGFONT 0

/* 1: Enable a published subscriber based messaging system */
#define LV_USE_MSG 0

/* 1: Enable Pinyin input method */
/* Requires: lv_keyboard */
#define LV_USE_IME_PINYIN 0
#if LV_USE_IME_PINYIN
/* 1: Use default font */
#define LV_IME_PINYIN_USE_DEFAULT_DICT 1
/* Set the modes (essays) for the input method. */
#define LV_IME_PINYIN_CAND_TEXT_NUM 6
#endif

/*==================
 * EXAMPLES
 *==================*/

/* Enable the examples to be built with the library */
#define LV_BUILD_EXAMPLES 0

/*==================
 * COMPILER SETTINGS
 *==================*/

/* For big endian systems set to 1 */
#define LV_BIG_ENDIAN_SYSTEM 0

/* Define a custom attribute to `lv_tick_inc` function */
#define LV_ATTRIBUTE_TICK_INC

/* Define a custom attribute to `lv_timer_handler` function */
#define LV_ATTRIBUTE_TIMER_HANDLER

/* Define a custom attribute to `lv_disp_flush_ready` function */
#define LV_ATTRIBUTE_FLUSH_READY

/* Required alignment size for buffers */
#define LV_ATTRIBUTE_MEM_ALIGN_SIZE 1

/* Will be added where memories needs to be aligned (with -Os data might not be aligned to boundary by default).
 * E.g. __attribute__((aligned(4))) */
#define LV_ATTRIBUTE_MEM_ALIGN

/* Attribute to mark large constant arrays for example font's bitmaps */
#define LV_ATTRIBUTE_LARGE_CONST

/* Compiler prefix for a big array declaration in RAM */
#define LV_ATTRIBUTE_LARGE_RAM_ARRAY

/* Place performance critical functions into a faster memory (e.g RAM) */
#define LV_ATTRIBUTE_FAST_MEM

/* Prefix variables that are used in GPU accelerated operations, often these need to be placed in RAM sections that are DMA accessible */
#define LV_ATTRIBUTE_DMA

/* Export integer constant to binding. This macro is used with constants in the form of LV_<CONST> that
 * should also appear on LVGL binding API such as Micropython. */
#define LV_EXPORT_CONST_INT(int_value) struct _silence_gcc_warning /* The default value just prevents GCC warning */

/* Extend the default -32k..32k coordinate range to -4M..4M by using int32_t for coordinates instead of int16_t */
#define LV_USE_LARGE_COORD 0

/*==================
 * FONT USAGE
 *==================*/

/* The built-in fonts contains the ASCII range and some Symbols with  4 bit-per-pixel.
 * The symbols are available via `LV_SYMBOL_...` defines
 * More info about fonts: https://docs.lvgl.io/v8/overview/font.html
 * To create a new font go to: https://lvgl.io/tools/fontconverter
 */

/* Montserrat fonts with various sizes and features.
 * A compressed version of Montserrat 14 is also available: LV_FONT_MONTSERRAT_14_COMPRESSED
 * https://fonts.google.com/specimen/Montserrat */

/*Pixel perfect monospace fonts*/

/* Optionally declare custom fonts here.
 * You can use these fonts as default font too and they will be available globally.
 * E.g. #define LV_FONT_CUSTOM_DECLARE   LV_FONT_DECLARE(my_font_1) LV_FONT_DECLARE(my_font_2) */

/*Always set a default font*/

/* Enable handling large font and/or fonts with a lot of characters.
 * The limit depends on the font size, font face and bpp
 * but with > 10,000 characters if you see issues probably you need to enable it.*/

/* Set the pixel order of the display. Physical order of RGB channels. Doesn't matter with "normal" fonts. */
#define LV_FONT_SUBPX_BGR 0    /* 0: RGB; 1:BGR order */

/*=================
 * TEXT SETTINGS
 *=================*/

/**
 * Select a character encoding for strings.
 * Your IDE or editor should have the same character encoding
 * - LV_TXT_ENC_UTF8
 * - LV_TXT_ENC_ASCII
 */

 /*Can break (wrap) texts on these chars*/

/* If a word is at least this long, will break wherever "prettiest"
 * To disable, set to a value <= 0 */

/* Minimum number of characters in a long word to put on a line before a break.
 * Depends on LV_TXT_LINE_BREAK_LONG_LEN. */

/* Minimum number of characters in a long word to put on a line after a break.
 * Depends on LV_TXT_LINE_BREAK_LONG_LEN. */

/* The control character to use for signalling text recoloring. */

/* Support bidirectional texts. Allows mixing Left-to-Right and Right-to-Left texts.
 * The direction will be processed according to the Unicode Bidirectional Algorithm:
 * https://www.unicode.org/reports/tr9/*/

/* Enable Arabic/Persian processing
 * In these languages characters should be replaced with an other form based on their position in the text */

/*===================
 * LV_OBJ SETTINGS
 *===================*/

/*Declare the type of the user data of objects. It can be e.g. `void *`, `int`, `struct`*/
typedef void * lv_obj_user_data_t;

/*Provide a function to free user data*/
#define LV_USE_USER_DATA_FREE 0
#if LV_USE_USER_DATA_FREE
#  define LV_USER_DATA_FREE_INCLUDE  "something.h"  /*Header to include for the prototype of `lv_user_data_free`*/
#  define LV_USER_DATA_FREE  (lv_user_data_free)   /*Function prototype to free user data. See `lv_obj_set_user_data`*/
#endif

/*1: enable `lv_obj_realloc()` to be used. Uses `realloc()` internally*/
#define LV_USE_OBJ_REALLOC 0

/*1: enable `lv_obj_realign()` based on `lv_obj_align()` parameters*/
#define LV_USE_OBJ_REALIGN 1

/* Enable to make the object clickable on a larger area.
 * LV_EXT_CLICK_AREA_OFF or 0: Disable this feature
 * LV_EXT_CLICK_AREA_TINY: The extra area can be adjusted horizontally and vertically (0..255 px)
 * LV_EXT_CLICK_AREA_FULL: The extra area can be adjusted in all 4 directions (-32k..+32k px)
 */
#define LV_USE_EXT_CLICK_AREA  LV_EXT_CLICK_AREA_TINY

/*==================
 * LV OBJ X USAGE
 *==================*/
/*
 * Documentation of the object types: https://docs.lvgl.io/latest/en/html/overview/object.html#object-types
 */

/*Arc (dependencies: -)*/

/*Bar (dependencies: -)*/

/*Button (dependencies: lv_cont*/

/*Button matrix (dependencies: -)*/

/*Calendar (dependencies: -)*/

/*Canvas (dependencies: lv_img)*/

/*Check box (dependencies: lv_btn, lv_label)*/

/*Chart (dependencies: -)*/

/*Color picker (dependencies: -*/

/*Drop down list (dependencies: lv_page, lv_label, lv_symbol_def.h)*/

/*Gauge (dependencies:lv_bar, lv_linemeter)*/

/*Image (dependencies: lv_label*/

/*Image Button (dependencies: lv_btn*/

/*Keyboard (dependencies: lv_btnm)*/

/*Label (dependencies: -*/

/*LED (dependencies: -)*/

/*Line (dependencies: -*/

/*List (dependencies: lv_page, lv_btn, lv_label, (lv_img optionally for icons ))*/

/*Line meter (dependencies: *;)*/

/*Mask (dependencies: -)*/

/*Message box (dependencies: lv_rect, lv_btnm, lv_label)*/

/*Page (dependencies: lv_cont)*/

/*Preload (dependencies: lv_arc, lv_anim)*/

/*Roller (dependencies: lv_ddlist)*/

/*Slider (dependencies: lv_bar)*/

/*Spinbox (dependencies: lv_ta)*/

/*Switch (dependencies: lv_slider)*/

/*Text area (dependencies: lv_label, lv_page)*/

/*Table (dependencies: lv_label)*/

/*Tabview (dependencies: lv_page, lv_btnm)*/

/*Tileview (dependencies: lv_page) */

/*Window (dependencies: lv_cont, lv_btn, lv_label, lv_img, lv_page)*/

/*==================
 * Non-user section
 *==================*/

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)    /* Disable warnings for Visual Studio*/
#  define _CRT_SECURE_NO_WARNINGS
#endif

/*--END OF LV_CONF_H--*/

#endif /*LV_CONF_H*/

/*Be sure every define has a default value*/
#include "src/lv_conf_internal.h"
