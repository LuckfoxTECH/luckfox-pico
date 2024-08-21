/**
 * @file lv_conf.h
 *
 */

/*
 * COPY THIS FILE AS `lv_conf.h` NEXT TO the `lvgl` FOLDER
 */

#if 1 /*Set it to "1" to enable content*/

#ifndef LV_CONF_H
#define LV_CONF_H
/* clang-format off */

#include <stdint.h>
#include "rtconfig.h"

/*====================
   Graphical settings
 *====================*/

/* Maximal horizontal and vertical resolution to support by the library.*/
#define LV_HOR_RES_MAX          (RT_LV_HOR_RES)
#define LV_VER_RES_MAX          (RT_LV_VER_RES)

#ifdef RT_LV_HOR_RES
#define LV_HOR_RES RT_LV_HOR_RES
#endif

#ifdef RT_LV_VER_RES
#define LV_VER_RES RT_LV_VER_RES
#endif

/* Color depth:
 * - 1:  1 byte per pixel
 * - 8:  RGB332
 * - 16: RGB565
 * - 32: ARGB8888
 */

#ifdef RT_LV_COLOR_DEPTH
#define LV_COLOR_DEPTH RT_LV_COLOR_DEPTH
#endif

/* Swap the 2 bytes of RGB565 color.
 * Useful if the display has a 8 bit interface (e.g. SPI)*/
#ifdef RT_LV_COLOR_16_SWAP
#define LV_COLOR_16_SWAP   1
#endif

/* 1: Enable screen transparency.
 * Useful for OSD or other overlapping GUIs.
 * Requires `LV_COLOR_DEPTH = 32` colors and the screen's style should be modified: `style.body.opa = ...`*/
#ifdef RT_LV_COLOR_SCREEN_TRANSP
#define LV_COLOR_SCREEN_TRANSP    1
#endif

/*Images pixels with this color will not be drawn (with chroma keying)*/
#define LV_COLOR_TRANSP    LV_COLOR_LIME         /*LV_COLOR_LIME: pure green*/

/* Enable chroma keying for indexed images. */
#ifdef RT_LV_INDEXED_CHROMA
#define LV_INDEXED_CHROMA    1
#else
#define LV_INDEXED_CHROMA    0
#endif

/* Enable anti-aliasing (lines, and radiuses will be smoothed) */
#ifdef RT_LV_ANTIALIAS
#define LV_ANTIALIAS        1
#endif

/* Default display refresh period.
 * Can be changed in the display driver (`lv_disp_drv_t`).*/
#ifdef RT_LV_DISP_DEF_REFR_PERIOD
#define LV_DISP_DEF_REFR_PERIOD      RT_LV_DISP_DEF_REFR_PERIOD      /*[ms]*/
#endif

/* Dot Per Inch: used to initialize default sizes.
 * E.g. a button with width = LV_DPI / 2 -> half inch wide
 * (Not so important, you can adjust it to modify default sizes and spaces)*/
//#define LV_DPI              50     /*[px]*/
#ifdef RT_LV_DPI
#define LV_DPI RT_LV_DPI
#endif

/* The the real width of the display changes some default values:
 * default object sizes, layout of examples, etc.
 * According to the width of the display (hor. res. / dpi)
 * the displays fall in 4 categories.
 * The 4th is extra large which has no upper limit so not listed here
 * The upper limit of the categories are set below in 0.1 inch unit.
 */
#ifdef RT_LV_DISP_SMALL_LIMIT
#define LV_DISP_SMALL_LIMIT  RT_LV_DISP_SMALL_LIMIT
#endif

#ifdef RT_LV_DISP_MEDIUM_LIMIT
#define LV_DISP_MEDIUM_LIMIT RT_LV_DISP_MEDIUM_LIMIT
#endif

#ifdef RT_LV_DISP_LARGE_LIMIT
#define LV_DISP_LARGE_LIMIT  RT_LV_DISP_LARGE_LIMIT
#endif

/* Type of coordinates. Should be `int16_t` (or `int32_t` for extreme cases) */
typedef int16_t lv_coord_t;

/*=========================
   Memory manager settings
 *=========================*/

/* LittelvGL's internal memory manager's settings.
 * The graphical objects and other related data are stored here. */

/* 1: use custom malloc/free, 0: use the built-in `lv_mem_alloc` and `lv_mem_free` */
#define LV_MEM_CUSTOM      0
#if LV_MEM_CUSTOM == 0
/* Size of the memory used by `lv_mem_alloc` in bytes (>= 2kB)*/
#  define LV_MEM_SIZE    (32U * 1024U)

/* Complier prefix for a big array declaration */
#  define LV_MEM_ATTR

/* Set an address for the memory pool instead of allocating it as an array.
 * Can be in external SRAM too. */
#  define LV_MEM_ADR          0

/* Automatically defrag. on free. Defrag. means joining the adjacent free cells. */
#  define LV_MEM_AUTO_DEFRAG  1
#else       /*LV_MEM_CUSTOM*/
#  define LV_MEM_CUSTOM_INCLUDE <stdlib.h>   /*Header for the dynamic memory function*/
#  define LV_MEM_CUSTOM_ALLOC   malloc       /*Wrapper to malloc*/
#  define LV_MEM_CUSTOM_FREE    free         /*Wrapper to free*/
#endif     /*LV_MEM_CUSTOM*/

/* Garbage Collector settings
 * Used if lvgl is binded to higher level language and the memory is managed by that language */
#define LV_ENABLE_GC 0
#if LV_ENABLE_GC != 0
#  define LV_GC_INCLUDE "gc.h"                           /*Include Garbage Collector related things*/
#  define LV_MEM_CUSTOM_REALLOC   your_realloc           /*Wrapper to realloc*/
#  define LV_MEM_CUSTOM_GET_SIZE  your_mem_get_size      /*Wrapper to lv_mem_get_size*/
#endif /* LV_ENABLE_GC */

/*=======================
   Input device settings
 *=======================*/

/* Input device default settings.
 * Can be changed in the Input device driver (`lv_indev_drv_t`)*/

/* Input device read period in milliseconds */
#ifdef RT_LV_INDEV_DEF_READ_PERIOD
#define LV_INDEV_DEF_READ_PERIOD          RT_LV_INDEV_DEF_READ_PERIOD
#endif

/* Drag threshold in pixels */
#ifdef RT_LV_INDEV_DEF_DRAG_LIMIT
#define LV_INDEV_DEF_DRAG_LIMIT           RT_LV_INDEV_DEF_DRAG_LIMIT
#endif

/* Drag throw slow-down in [%]. Greater value -> faster slow-down */
#ifdef RT_LV_INDEV_DEF_DRAG_THROW
#define LV_INDEV_DEF_DRAG_THROW           RT_LV_INDEV_DEF_DRAG_THROW
#endif

/* Long press time in milliseconds.
 * Time to send `LV_EVENT_LONG_PRESSSED`) */
#ifdef RT_LV_INDEV_DEF_LONG_PRESS_TIME
#define LV_INDEV_DEF_LONG_PRESS_TIME      RT_LV_INDEV_DEF_LONG_PRESS_TIME
#endif

/* Repeated trigger period in long press [ms]
 * Time between `LV_EVENT_LONG_PRESSED_REPEAT */
#ifdef RT_LV_INDEV_DEF_LONG_PRESS_REP_TIME
#define LV_INDEV_DEF_LONG_PRESS_REP_TIME  RT_LV_INDEV_DEF_LONG_PRESS_REP_TIME
#endif

/* Gesture threshold in pixels */
#ifdef RT_LV_INDEV_DEF_GESTURE_LIMIT
#define LV_INDEV_DEF_GESTURE_LIMIT        RT_LV_INDEV_DEF_GESTURE_LIMIT
#endif

/* Gesture min velocity at release before swipe (pixels)*/
#ifdef RT_LV_INDEV_DEF_GESTURE_MIN_VELOCITY
#define LV_INDEV_DEF_GESTURE_MIN_VELOCITY RT_LV_INDEV_DEF_GESTURE_MIN_VELOCITY
#endif

/*==================
 * Feature usage
 *==================*/

/*1: Enable the Animations */
#ifdef RT_LV_USE_ANIMATION
#define LV_USE_ANIMATION        1
#endif
#if LV_USE_ANIMATION

/*Declare the type of the user data of animations (can be e.g. `void *`, `int`, `struct`)*/
typedef void *lv_anim_user_data_t;

#endif

/* 1: Enable shadow drawing*/
#ifdef RT_LV_USE_SHADOW
#define LV_USE_SHADOW           1
#endif
#if LV_USE_SHADOW
/* Allow buffering some shadow calculation
 * LV_SHADOW_CACHE_SIZE is the max. shadow size to buffer,
 * where shadow size is `shadow_width + radius`
 * Caching has LV_SHADOW_CACHE_SIZE^2 RAM cost*/
#define LV_SHADOW_CACHE_SIZE    0
#endif

/* 1: Use other blend modes than normal (`LV_BLEND_MODE_...`)*/
#ifdef RT_LV_USE_BLEND_MODES
#define LV_USE_BLEND_MODES      1
#endif

/* 1: Use the `opa_scale` style property to set the opacity of an object and its children at once*/
#ifdef RT_LV_USE_OPA_SCALE
#define LV_USE_OPA_SCALE        1
#endif

/* 1: Use image zoom and rotation*/
#ifdef RT_LV_USE_IMG_TRANSFORM
#define LV_USE_IMG_TRANSFORM    1
#endif

/* 1: Enable object groups (for keyboard/encoder navigation) */
#ifdef RT_LV_USE_GROUP
#define LV_USE_GROUP            1
#endif
#if LV_USE_GROUP
typedef void *lv_group_user_data_t;
#endif  /*LV_USE_GROUP*/

/* 1: Enable GPU interface*/
#ifdef RT_LV_USE_GPU
#define LV_USE_GPU              1   /*Only enables `gpu_fill_cb` and `gpu_blend_cb` in the disp. drv- */
#endif

#define LV_USE_GPU_STM32_DMA2D  0
/*If enabling LV_USE_GPU_STM32_DMA2D, LV_GPU_DMA2D_CMSIS_INCLUDE must be defined to include path of CMSIS header of target processor
e.g. "stm32f769xx.h" or "stm32f429xx.h" */
#define LV_GPU_DMA2D_CMSIS_INCLUDE

/* 1: Enable file system (might be required for images */
#ifdef RT_LV_USE_FILESYSTEM
#define LV_USE_FILESYSTEM       1
#endif
#if LV_USE_FILESYSTEM
/*Declare the type of the user data of file system drivers (can be e.g. `void *`, `int`, `struct`)*/
typedef void *lv_fs_drv_user_data_t;
#endif

/*1: Add a `user_data` to drivers and objects*/
#ifdef RT_LV_USE_USER_DATA
#define LV_USE_USER_DATA        1
#endif

/*1: Show CPU usage and FPS count in the right bottom corner*/
#ifdef RT_LV_USE_PERF_MONITOR
#define LV_USE_PERF_MONITOR     1
#endif

/*1: Use the functions and types from the older API if possible */
#define LV_USE_API_EXTENSION_V6  1
#define LV_USE_API_EXTENSION_V7  1

/*========================
 * Image decoder and cache
 *========================*/

/* 1: Enable indexed (palette) images */
#define LV_IMG_CF_INDEXED       1

/* 1: Enable alpha indexed images */
#define LV_IMG_CF_ALPHA         1

/* Default image cache size. Image caching keeps the images opened.
 * If only the built-in image formats are used there is no real advantage of caching.
 * (I.e. no new image decoder is added)
 * With complex image decoders (e.g. PNG or JPG) caching can save the continuous open/decode of images.
 * However the opened images might consume additional RAM.
 * LV_IMG_CACHE_DEF_SIZE must be >= 1 */
#define LV_IMG_CACHE_DEF_SIZE       1

/*Declare the type of the user data of image decoder (can be e.g. `void *`, `int`, `struct`)*/
typedef void *lv_img_decoder_user_data_t;

/*=====================
 *  Compiler settings
 *====================*/

/* For big endian systems set to 1 */
#define LV_BIG_ENDIAN_SYSTEM    0

/* Define a custom attribute to `lv_tick_inc` function */
#define LV_ATTRIBUTE_TICK_INC

/* Define a custom attribute to `lv_task_handler` function */
#define LV_ATTRIBUTE_TASK_HANDLER

/* Define a custom attribute to `lv_disp_flush_ready` function */
#define LV_ATTRIBUTE_FLUSH_READY

/* With size optimization (-Os) the compiler might not align data to
 * 4 or 8 byte boundary. This alignment will be explicitly applied where needed.
 * E.g. __attribute__((aligned(4))) */
#define LV_ATTRIBUTE_MEM_ALIGN

/* Attribute to mark large constant arrays for example
 * font's bitmaps */
#define LV_ATTRIBUTE_LARGE_CONST

/* Prefix performance critical functions to place them into a faster memory (e.g RAM)
 * Uses 15-20 kB extra memory */
#define LV_ATTRIBUTE_FAST_MEM

/* Export integer constant to binding.
 * This macro is used with constants in the form of LV_<CONST> that
 * should also appear on lvgl binding API such as Micropython
 *
 * The default value just prevents a GCC warning.
 */
#define LV_EXPORT_CONST_INT(int_value) struct _silence_gcc_warning

/* Prefix variables that are used in GPU accelerated operations, often these need to be
 * placed in RAM sections that are DMA accessible */
#define LV_ATTRIBUTE_DMA

/*===================
 *  HAL settings
 *==================*/

/* 1: use a custom tick source.
 * It removes the need to manually update the tick with `lv_tick_inc`) */
#define LV_TICK_CUSTOM     1
#if LV_TICK_CUSTOM == 1
#define LV_TICK_CUSTOM_INCLUDE  "rtthread.h"       /*Header for the sys time function*/
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (rt_tick_get())     /*Expression evaluating to current systime in ms*/
#endif   /*LV_TICK_CUSTOM*/

typedef void *lv_disp_drv_user_data_t;              /*Type of user data in the display driver*/
typedef void *lv_indev_drv_user_data_t;             /*Type of user data in the input device driver*/

/*================
 * Log settings
 *===============*/

/*1: Enable the log module*/
#define LV_USE_LOG      1
#if LV_USE_LOG
/* How important log should be added:
 * LV_LOG_LEVEL_TRACE       A lot of logs to give detailed information
 * LV_LOG_LEVEL_INFO        Log important events
 * LV_LOG_LEVEL_WARN        Log if something unwanted happened but didn't cause a problem
 * LV_LOG_LEVEL_ERROR       Only critical issue, when the system may fail
 * LV_LOG_LEVEL_NONE        Do not log anything
 */
#define LV_LOG_LEVEL    LV_LOG_LEVEL_WARN

/* 1: Print the log with 'printf';
 * 0: user need to register a callback with `lv_log_register_print_cb`*/
#define LV_LOG_PRINTF   1
#endif  /*LV_USE_LOG*/

/*=================
 * Debug settings
 *================*/

/* If Debug is enabled LittelvGL validates the parameters of the functions.
 * If an invalid parameter is found an error log message is printed and
 * the MCU halts at the error. (`LV_USE_LOG` should be enabled)
 * If you are debugging the MCU you can pause
 * the debugger to see exactly where  the issue is.
 *
 * The behavior of asserts can be overwritten by redefining them here.
 * E.g. #define LV_ASSERT_MEM(p)  <my_assert_code>
 */
#define LV_USE_DEBUG        1
#if LV_USE_DEBUG

/*Check if the parameter is NULL. (Quite fast) */
#define LV_USE_ASSERT_NULL      1

/*Checks is the memory is successfully allocated or no. (Quite fast)*/
#define LV_USE_ASSERT_MEM       1

/*Check the integrity of `lv_mem` after critical operations. (Slow)*/
#define LV_USE_ASSERT_MEM_INTEGRITY       0

/* Check the strings.
 * Search for NULL, very long strings, invalid characters, and unnatural repetitions. (Slow)
 * If disabled `LV_USE_ASSERT_NULL` will be performed instead (if it's enabled) */
#define LV_USE_ASSERT_STR       0

/* Check NULL, the object's type and existence (e.g. not deleted). (Quite slow)
 * If disabled `LV_USE_ASSERT_NULL` will be performed instead (if it's enabled) */
#define LV_USE_ASSERT_OBJ       0

/*Check if the styles are properly initialized. (Fast)*/
#define LV_USE_ASSERT_STYLE     1

#endif /*LV_USE_DEBUG*/

/*==================
 *    FONT USAGE
 *===================*/

/* The built-in fonts contains the ASCII range and some Symbols with  4 bit-per-pixel.
 * The symbols are available via `LV_SYMBOL_...` defines
 * More info about fonts: https://docs.lvgl.io/v7/en/html/overview/font.html
 * To create a new font go to: https://lvgl.com/ttf-font-to-c-array
 */

/* Montserrat fonts with bpp = 4
 * https://fonts.google.com/specimen/Montserrat  */
#ifdef RT_LV_FONT_MONTSERRAT_12
#define LV_FONT_MONTSERRAT_12    1
#endif
#ifdef RT_LV_FONT_MONTSERRAT_14
#define LV_FONT_MONTSERRAT_14    1
#endif
#ifdef RT_LV_FONT_MONTSERRAT_16
#define LV_FONT_MONTSERRAT_16    1
#endif
#ifdef RT_LV_FONT_MONTSERRAT_18
#define LV_FONT_MONTSERRAT_18    1
#endif
#ifdef RT_LV_FONT_MONTSERRAT_20
#define LV_FONT_MONTSERRAT_20    1
#endif
#ifdef RT_LV_FONT_MONTSERRAT_22
#define LV_FONT_MONTSERRAT_22    1
#endif
#ifdef RT_LV_FONT_MONTSERRAT_24
#define LV_FONT_MONTSERRAT_24    1
#endif
#ifdef RT_LV_FONT_MONTSERRAT_26
#define LV_FONT_MONTSERRAT_26    1
#endif
#ifdef RT_LV_FONT_MONTSERRAT_28
#define LV_FONT_MONTSERRAT_28    1
#endif
#ifdef RT_LV_FONT_MONTSERRAT_30
#define LV_FONT_MONTSERRAT_30    1
#endif
#ifdef RT_LV_FONT_MONTSERRAT_32
#define LV_FONT_MONTSERRAT_32    1
#endif
#ifdef RT_LV_FONT_MONTSERRAT_34
#define LV_FONT_MONTSERRAT_34    1
#endif
#ifdef RT_LV_FONT_MONTSERRAT_36
#define LV_FONT_MONTSERRAT_36    1
#endif
#ifdef RT_LV_FONT_MONTSERRAT_38
#define LV_FONT_MONTSERRAT_38    1
#endif
#ifdef RT_LV_FONT_MONTSERRAT_40
#define LV_FONT_MONTSERRAT_40    1
#endif
#ifdef RT_LV_FONT_MONTSERRAT_42
#define LV_FONT_MONTSERRAT_42    1
#endif
#ifdef RT_LV_FONT_MONTSERRAT_44
#define LV_FONT_MONTSERRAT_44    1
#endif
#ifdef RT_LV_FONT_MONTSERRAT_46
#define LV_FONT_MONTSERRAT_46    1
#endif
#ifdef RT_LV_FONT_MONTSERRAT_48
#define LV_FONT_MONTSERRAT_48    1
#endif


/* Demonstrate special features */
#define LV_FONT_MONTSERRAT_12_SUBPX      0
#define LV_FONT_MONTSERRAT_28_COMPRESSED 0  /*bpp = 3*/
#define LV_FONT_DEJAVU_16_PERSIAN_HEBREW 0  /*Hebrew, Arabic, PErisan letters and all their forms*/
#define LV_FONT_SIMSUN_16_CJK            0  /*1000 most common CJK radicals*/

/*Pixel perfect monospace font
 * http://pelulamu.net/unscii/ */
#define LV_FONT_UNSCII_8     0

/* Optionally declare your custom fonts here.
 * You can use these fonts as default font too
 * and they will be available globally. E.g.
 * #define LV_FONT_CUSTOM_DECLARE LV_FONT_DECLARE(my_font_1) \
 *                                LV_FONT_DECLARE(my_font_2)
 */
#define LV_FONT_CUSTOM_DECLARE

/* Enable it if you have fonts with a lot of characters.
 * The limit depends on the font size, font face and bpp
 * but with > 10,000 characters if you see issues probably you need to enable it.*/
#define LV_FONT_FMT_TXT_LARGE   0

/* Enables/disables support for compressed fonts. If it's disabled, compressed
 * glyphs cannot be processed by the library and won't be rendered.
 */
#define LV_USE_FONT_COMPRESSED 1

/* Enable subpixel rendering */
#define LV_USE_FONT_SUBPX 1
#if LV_USE_FONT_SUBPX
/* Set the pixel order of the display.
 * Important only if "subpx fonts" are used.
 * With "normal" font it doesn't matter.
 */
#define LV_FONT_SUBPX_BGR    0
#endif

/*Declare the type of the user data of fonts (can be e.g. `void *`, `int`, `struct`)*/
typedef void *lv_font_user_data_t;

/*================
 *  THEME USAGE
 *================*/

/*Always enable at least on theme*/

/* No theme, you can apply your styles as you need
 * No flags. Set LV_THEME_DEFAULT_FLAG 0 */
#define LV_USE_THEME_EMPTY       0

/*Simple to the create your theme based on it
 * No flags. Set LV_THEME_DEFAULT_FLAG 0 */
#define LV_USE_THEME_TEMPLATE    0

/* A fast and impressive theme.
 * Flags:
 * LV_THEME_MATERIAL_FLAG_LIGHT: light theme
 * LV_THEME_MATERIAL_FLAG_DARK: dark theme
 * LV_THEME_MATERIAL_FLAG_NO_TRANSITION: disable transitions (state change animations)
 * LV_THEME_MATERIAL_FLAG_NO_FOCUS: disable indication of focused state)
 * */
#define LV_USE_THEME_MATERIAL    0

/* Mono-color theme for monochrome displays.
 * If LV_THEME_DEFAULT_COLOR_PRIMARY is LV_COLOR_BLACK the
 * texts and borders will be black and the background will be
 * white. Else the colors are inverted.
 * No flags. Set LV_THEME_DEFAULT_FLAG 0 */
#define LV_USE_THEME_MONO        0

/* Copy of material theme for monochrome displays.
*  You can modify the styles as you need
 * No flags. Set LV_THEME_DEFAULT_FLAG 0 */
#define LV_USE_THEME_RTT         1

#define LV_THEME_DEFAULT_INCLUDE            <stdint.h>            /*Include a header for the init. function*/
#define LV_THEME_DEFAULT_INIT               lv_theme_rtt_init     /*Theme init function implemented by user if not use system theme*/
#define LV_THEME_DEFAULT_FLAG               0

#ifdef RT_LV_THEME_DEFAULT_COLOR_PRIMARY
#define LV_THEME_DEFAULT_COLOR_PRIMARY      lv_color_hex(RT_LV_THEME_DEFAULT_COLOR_PRIMARY)
#endif
#ifdef RT_LV_THEME_DEFAULT_COLOR_SECONDARY
#define LV_THEME_DEFAULT_COLOR_SECONDARY    lv_color_hex(RT_LV_THEME_DEFAULT_COLOR_SECONDARY)
#endif

#ifdef RT_LV_THEME_DEFAULT_FONT_SMALL_MONTSERRAT_12
#define LV_THEME_DEFAULT_FONT_SMALL         &lv_font_montserrat_12
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SMALL_MONTSERRAT_14
#define LV_THEME_DEFAULT_FONT_SMALL         &lv_font_montserrat_14
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SMALL_MONTSERRAT_16
#define LV_THEME_DEFAULT_FONT_SMALL         &lv_font_montserrat_16
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SMALL_MONTSERRAT_18
#define LV_THEME_DEFAULT_FONT_SMALL         &lv_font_montserrat_18
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SMALL_MONTSERRAT_20
#define LV_THEME_DEFAULT_FONT_SMALL         &lv_font_montserrat_20
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SMALL_MONTSERRAT_22
#define LV_THEME_DEFAULT_FONT_SMALL         &lv_font_montserrat_22
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SMALL_MONTSERRAT_24
#define LV_THEME_DEFAULT_FONT_SMALL         &lv_font_montserrat_24
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SMALL_MONTSERRAT_26
#define LV_THEME_DEFAULT_FONT_SMALL         &lv_font_montserrat_26
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SMALL_MONTSERRAT_28
#define LV_THEME_DEFAULT_FONT_SMALL         &lv_font_montserrat_28
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SMALL_MONTSERRAT_30
#define LV_THEME_DEFAULT_FONT_SMALL         &lv_font_montserrat_30
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SMALL_MONTSERRAT_32
#define LV_THEME_DEFAULT_FONT_SMALL         &lv_font_montserrat_32
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SMALL_MONTSERRAT_34
#define LV_THEME_DEFAULT_FONT_SMALL         &lv_font_montserrat_34
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SMALL_MONTSERRAT_36
#define LV_THEME_DEFAULT_FONT_SMALL         &lv_font_montserrat_36
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SMALL_MONTSERRAT_38
#define LV_THEME_DEFAULT_FONT_SMALL         &lv_font_montserrat_38
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SMALL_MONTSERRAT_40
#define LV_THEME_DEFAULT_FONT_SMALL         &lv_font_montserrat_40
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SMALL_MONTSERRAT_42
#define LV_THEME_DEFAULT_FONT_SMALL         &lv_font_montserrat_42
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SMALL_MONTSERRAT_44
#define LV_THEME_DEFAULT_FONT_SMALL         &lv_font_montserrat_44
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SMALL_MONTSERRAT_46
#define LV_THEME_DEFAULT_FONT_SMALL         &lv_font_montserrat_46
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SMALL_MONTSERRAT_48
#define LV_THEME_DEFAULT_FONT_SMALL         &lv_font_montserrat_48
#endif

#ifdef RT_LV_THEME_DEFAULT_FONT_NORMAL_MONTSERRAT_12
#define LV_THEME_DEFAULT_FONT_NORMAL         &lv_font_montserrat_12
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_NORMAL_MONTSERRAT_14
#define LV_THEME_DEFAULT_FONT_NORMAL         &lv_font_montserrat_14
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_NORMAL_MONTSERRAT_16
#define LV_THEME_DEFAULT_FONT_NORMAL         &lv_font_montserrat_16
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_NORMAL_MONTSERRAT_18
#define LV_THEME_DEFAULT_FONT_NORMAL         &lv_font_montserrat_18
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_NORMAL_MONTSERRAT_20
#define LV_THEME_DEFAULT_FONT_NORMAL         &lv_font_montserrat_20
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_NORMAL_MONTSERRAT_22
#define LV_THEME_DEFAULT_FONT_NORMAL         &lv_font_montserrat_22
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_NORMAL_MONTSERRAT_24
#define LV_THEME_DEFAULT_FONT_NORMAL         &lv_font_montserrat_24
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_NORMAL_MONTSERRAT_26
#define LV_THEME_DEFAULT_FONT_NORMAL         &lv_font_montserrat_26
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_NORMAL_MONTSERRAT_28
#define LV_THEME_DEFAULT_FONT_NORMAL         &lv_font_montserrat_28
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_NORMAL_MONTSERRAT_30
#define LV_THEME_DEFAULT_FONT_NORMAL         &lv_font_montserrat_30
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_NORMAL_MONTSERRAT_32
#define LV_THEME_DEFAULT_FONT_NORMAL         &lv_font_montserrat_32
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_NORMAL_MONTSERRAT_34
#define LV_THEME_DEFAULT_FONT_NORMAL         &lv_font_montserrat_34
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_NORMAL_MONTSERRAT_36
#define LV_THEME_DEFAULT_FONT_NORMAL         &lv_font_montserrat_36
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_NORMAL_MONTSERRAT_38
#define LV_THEME_DEFAULT_FONT_NORMAL         &lv_font_montserrat_38
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_NORMAL_MONTSERRAT_40
#define LV_THEME_DEFAULT_FONT_NORMAL         &lv_font_montserrat_40
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_NORMAL_MONTSERRAT_42
#define LV_THEME_DEFAULT_FONT_NORMAL         &lv_font_montserrat_42
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_NORMAL_MONTSERRAT_44
#define LV_THEME_DEFAULT_FONT_NORMAL         &lv_font_montserrat_44
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_NORMAL_MONTSERRAT_46
#define LV_THEME_DEFAULT_FONT_NORMAL         &lv_font_montserrat_46
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_NORMAL_MONTSERRAT_48
#define LV_THEME_DEFAULT_FONT_NORMAL         &lv_font_montserrat_48
#endif

#ifdef RT_LV_THEME_DEFAULT_FONT_SUBTITLE_MONTSERRAT_12
#define LV_THEME_DEFAULT_FONT_SUBTITLE         &lv_font_montserrat_12
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SUBTITLE_MONTSERRAT_14
#define LV_THEME_DEFAULT_FONT_SUBTITLE         &lv_font_montserrat_14
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SUBTITLE_MONTSERRAT_16
#define LV_THEME_DEFAULT_FONT_SUBTITLE         &lv_font_montserrat_16
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SUBTITLE_MONTSERRAT_18
#define LV_THEME_DEFAULT_FONT_SUBTITLE         &lv_font_montserrat_18
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SUBTITLE_MONTSERRAT_20
#define LV_THEME_DEFAULT_FONT_SUBTITLE         &lv_font_montserrat_20
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SUBTITLE_MONTSERRAT_22
#define LV_THEME_DEFAULT_FONT_SUBTITLE         &lv_font_montserrat_22
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SUBTITLE_MONTSERRAT_24
#define LV_THEME_DEFAULT_FONT_SUBTITLE         &lv_font_montserrat_24
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SUBTITLE_MONTSERRAT_26
#define LV_THEME_DEFAULT_FONT_SUBTITLE         &lv_font_montserrat_26
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SUBTITLE_MONTSERRAT_28
#define LV_THEME_DEFAULT_FONT_SUBTITLE         &lv_font_montserrat_28
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SUBTITLE_MONTSERRAT_30
#define LV_THEME_DEFAULT_FONT_SUBTITLE         &lv_font_montserrat_30
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SUBTITLE_MONTSERRAT_32
#define LV_THEME_DEFAULT_FONT_SUBTITLE         &lv_font_montserrat_32
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SUBTITLE_MONTSERRAT_34
#define LV_THEME_DEFAULT_FONT_SUBTITLE         &lv_font_montserrat_34
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SUBTITLE_MONTSERRAT_36
#define LV_THEME_DEFAULT_FONT_SUBTITLE         &lv_font_montserrat_36
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SUBTITLE_MONTSERRAT_38
#define LV_THEME_DEFAULT_FONT_SUBTITLE         &lv_font_montserrat_38
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SUBTITLE_MONTSERRAT_40
#define LV_THEME_DEFAULT_FONT_SUBTITLE         &lv_font_montserrat_40
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SUBTITLE_MONTSERRAT_42
#define LV_THEME_DEFAULT_FONT_SUBTITLE         &lv_font_montserrat_42
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SUBTITLE_MONTSERRAT_44
#define LV_THEME_DEFAULT_FONT_SUBTITLE         &lv_font_montserrat_44
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SUBTITLE_MONTSERRAT_46
#define LV_THEME_DEFAULT_FONT_SUBTITLE         &lv_font_montserrat_46
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_SUBTITLE_MONTSERRAT_48
#define LV_THEME_DEFAULT_FONT_SUBTITLE         &lv_font_montserrat_48
#endif


#ifdef RT_LV_THEME_DEFAULT_FONT_TITLE_MONTSERRAT_12
#define LV_THEME_DEFAULT_FONT_TITLE         &lv_font_montserrat_12
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_TITLE_MONTSERRAT_14
#define LV_THEME_DEFAULT_FONT_TITLE         &lv_font_montserrat_14
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_TITLE_MONTSERRAT_16
#define LV_THEME_DEFAULT_FONT_TITLE         &lv_font_montserrat_16
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_TITLE_MONTSERRAT_18
#define LV_THEME_DEFAULT_FONT_TITLE         &lv_font_montserrat_18
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_TITLE_MONTSERRAT_20
#define LV_THEME_DEFAULT_FONT_TITLE         &lv_font_montserrat_20
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_TITLE_MONTSERRAT_22
#define LV_THEME_DEFAULT_FONT_TITLE         &lv_font_montserrat_22
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_TITLE_MONTSERRAT_24
#define LV_THEME_DEFAULT_FONT_TITLE         &lv_font_montserrat_24
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_TITLE_MONTSERRAT_26
#define LV_THEME_DEFAULT_FONT_TITLE         &lv_font_montserrat_26
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_TITLE_MONTSERRAT_28
#define LV_THEME_DEFAULT_FONT_TITLE         &lv_font_montserrat_28
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_TITLE_MONTSERRAT_30
#define LV_THEME_DEFAULT_FONT_TITLE         &lv_font_montserrat_30
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_TITLE_MONTSERRAT_32
#define LV_THEME_DEFAULT_FONT_TITLE         &lv_font_montserrat_32
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_TITLE_MONTSERRAT_34
#define LV_THEME_DEFAULT_FONT_TITLE         &lv_font_montserrat_34
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_TITLE_MONTSERRAT_36
#define LV_THEME_DEFAULT_FONT_TITLE         &lv_font_montserrat_36
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_TITLE_MONTSERRAT_38
#define LV_THEME_DEFAULT_FONT_TITLE         &lv_font_montserrat_38
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_TITLE_MONTSERRAT_40
#define LV_THEME_DEFAULT_FONT_TITLE         &lv_font_montserrat_40
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_TITLE_MONTSERRAT_42
#define LV_THEME_DEFAULT_FONT_TITLE         &lv_font_montserrat_42
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_TITLE_MONTSERRAT_44
#define LV_THEME_DEFAULT_FONT_TITLE         &lv_font_montserrat_44
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_TITLE_MONTSERRAT_46
#define LV_THEME_DEFAULT_FONT_TITLE         &lv_font_montserrat_46
#endif
#ifdef RT_LV_THEME_DEFAULT_FONT_TITLE_MONTSERRAT_48
#define LV_THEME_DEFAULT_FONT_TITLE         &lv_font_montserrat_48
#endif

/*=================
 *  Text settings
 *=================*/

/* Select a character encoding for strings.
 * Your IDE or editor should have the same character encoding
 * - LV_TXT_ENC_UTF8
 * - LV_TXT_ENC_ASCII
 * */
#define LV_TXT_ENC LV_TXT_ENC_UTF8

/*Can break (wrap) texts on these chars*/
#define LV_TXT_BREAK_CHARS                  " ,.;:-_"

/* If a word is at least this long, will break wherever "prettiest"
 * To disable, set to a value <= 0 */
#define LV_TXT_LINE_BREAK_LONG_LEN          1

/* Minimum number of characters in a long word to put on a line before a break.
 * Depends on LV_TXT_LINE_BREAK_LONG_LEN. */
#define LV_TXT_LINE_BREAK_LONG_PRE_MIN_LEN  3

/* Minimum number of characters in a long word to put on a line after a break.
 * Depends on LV_TXT_LINE_BREAK_LONG_LEN. */
#define LV_TXT_LINE_BREAK_LONG_POST_MIN_LEN 3

/* The control character to use for signalling text recoloring. */
#define LV_TXT_COLOR_CMD "#"

/* Support bidirectional texts.
 * Allows mixing Left-to-Right and Right-to-Left texts.
 * The direction will be processed according to the Unicode Bidirectioanl Algorithm:
 * https://www.w3.org/International/articles/inline-bidi-markup/uba-basics*/
#define LV_USE_BIDI     0
#if LV_USE_BIDI
/* Set the default direction. Supported values:
 * `LV_BIDI_DIR_LTR` Left-to-Right
 * `LV_BIDI_DIR_RTL` Right-to-Left
 * `LV_BIDI_DIR_AUTO` detect texts base direction */
#define LV_BIDI_BASE_DIR_DEF  LV_BIDI_DIR_AUTO
#endif

/* Enable Arabic/Persian processing
 * In these languages characters should be replaced with
 * an other form based on their position in the text */
#define LV_USE_ARABIC_PERSIAN_CHARS 0

/*Change the built in (v)snprintf functions*/
#define LV_SPRINTF_CUSTOM   0
#if LV_SPRINTF_CUSTOM
#  define LV_SPRINTF_INCLUDE <stdio.h>
#  define lv_snprintf     snprintf
#  define lv_vsnprintf    vsnprintf
#else   /*!LV_SPRINTF_CUSTOM*/
#  define LV_SPRINTF_DISABLE_FLOAT 1
#endif  /*LV_SPRINTF_CUSTOM*/

/*===================
 *  LV_OBJ SETTINGS
 *==================*/

#if LV_USE_USER_DATA
/*Declare the type of the user data of object (can be e.g. `void *`, `int`, `struct`)*/
typedef void *lv_obj_user_data_t;
/*Provide a function to free user data*/
#define LV_USE_USER_DATA_FREE 0
#if LV_USE_USER_DATA_FREE
#  define LV_USER_DATA_FREE_INCLUDE  "something.h"  /*Header for user data free function*/
/* Function prototype : void user_data_free(lv_obj_t * obj); */
#  define LV_USER_DATA_FREE  (user_data_free)       /*Invoking for user data free function*/
#endif
#endif

/*1: enable `lv_obj_realign()` based on `lv_obj_align()` parameters*/
#define LV_USE_OBJ_REALIGN          1

/* Enable to make the object clickable on a larger area.
 * LV_EXT_CLICK_AREA_OFF or 0: Disable this feature
 * LV_EXT_CLICK_AREA_TINY: The extra area can be adjusted horizontally and vertically (0..255 px)
 * LV_EXT_CLICK_AREA_FULL: The extra area can be adjusted in all 4 directions (-32k..+32k px)
 */
#define LV_USE_EXT_CLICK_AREA  LV_EXT_CLICK_AREA_TINY

/*==================
 *  LV OBJ X USAGE
 *================*/
/*
 * Documentation of the object types: https://docs.lvgl.com/#Object-types
 */

/*Arc (dependencies: -)*/
#ifdef RT_LV_USE_ARC
#define LV_USE_ARC      1
#endif

/*Bar (dependencies: -)*/
#ifdef RT_LV_USE_BAR
#define LV_USE_BAR      1
#endif

/*Button (dependencies: lv_cont*/
#ifdef RT_LV_USE_BTN
#define LV_USE_BTN      1
#endif

/*Button matrix (dependencies: -)*/
#ifdef RT_LV_USE_BTNMATRIX
#define LV_USE_BTNMATRIX     1
#endif

/*Calendar (dependencies: -)*/
#ifdef RT_LV_USE_CALENDAR
#define LV_USE_CALENDAR 1
#endif
#if LV_USE_CALENDAR
#  define LV_CALENDAR_WEEK_STARTS_MONDAY    0
#endif

/*Canvas (dependencies: lv_img)*/
#ifdef RT_LV_USE_CANVAS
#define LV_USE_CANVAS   1
#endif

/*Check box (dependencies: lv_btn, lv_label)*/
#ifdef RT_LV_USE_CHECKBOX
#define LV_USE_CHECKBOX       1
#endif

/*Chart (dependencies: -)*/
#ifdef RT_LV_USE_CHART
#define LV_USE_CHART    1
#endif
#if LV_USE_CHART
#  define LV_CHART_AXIS_TICK_LABEL_MAX_LEN    256
#endif

/*Container (dependencies: -*/
#ifdef RT_LV_USE_CONT
#define LV_USE_CONT     1
#endif

/*Color picker (dependencies: -*/
#ifdef RT_LV_USE_CPICKER
#define LV_USE_CPICKER   1
#endif

/*Drop down list (dependencies: lv_page, lv_label, lv_symbol_def.h)*/
#ifdef RT_LV_USE_DROPDOWN
#define LV_USE_DROPDOWN    1
#endif
#if LV_USE_DROPDOWN != 0
/*Open and close default animation time [ms] (0: no animation)*/
#  define LV_DROPDOWN_DEF_ANIM_TIME     200
#endif

/*Gauge (dependencies:lv_bar, lv_linemeter)*/
#ifdef RT_LV_USE_GAUGE
#define LV_USE_GAUGE    1
#endif

/*Clock (dependencies:lv_bar, lv_linemeter)*/
#ifdef RT_LV_USE_CLOCK
#define LV_USE_CLOCK    1
#endif

/*Image (dependencies: lv_label*/
#ifdef RT_LV_USE_IMG
#define LV_USE_IMG      1
#endif

/*Image Button (dependencies: lv_btn*/
#ifdef RT_LV_USE_IMGBTN
#define LV_USE_IMGBTN   1
#endif
#if LV_USE_IMGBTN
/*1: The imgbtn requires left, mid and right parts and the width can be set freely*/
#  define LV_IMGBTN_TILED 0
#endif

/*Keyboard (dependencies: lv_btnm)*/
#ifdef RT_LV_USE_KEYBOARD
#define LV_USE_KEYBOARD       1
#endif

/*Label (dependencies: -*/
#ifdef RT_LV_USE_LABEL
#define LV_USE_LABEL    1
#endif
#if LV_USE_LABEL != 0
/*Hor, or ver. scroll speed [px/sec] in 'LV_LABEL_LONG_ROLL/ROLL_CIRC' mode*/
#  define LV_LABEL_DEF_SCROLL_SPEED       25

/* Waiting period at beginning/end of animation cycle */
#  define LV_LABEL_WAIT_CHAR_COUNT        3

/*Enable selecting text of the label */
#  define LV_LABEL_TEXT_SEL               0

/*Store extra some info in labels (12 bytes) to speed up drawing of very long texts*/
#  define LV_LABEL_LONG_TXT_HINT          0
#endif

/*LED (dependencies: -)*/
#ifdef RT_LV_USE_LED
#define LV_USE_LED      1
#endif
#if LV_USE_LED
#  define LV_LED_BRIGHT_MIN  120      /*Minimal brightness*/
#  define LV_LED_BRIGHT_MAX  255     /*Maximal brightness*/
#endif

/*Line (dependencies: -*/
#ifdef RT_LV_USE_LINE
#define LV_USE_LINE     1
#endif

/*List (dependencies: lv_page, lv_btn, lv_label, (lv_img optionally for icons ))*/
#ifdef RT_LV_USE_LIST
#define LV_USE_LIST     1
#endif
#if LV_USE_LIST != 0
/*Default animation time of focusing to a list element [ms] (0: no animation)  */
#  define LV_LIST_DEF_ANIM_TIME  100
#endif

/*Line meter (dependencies: *;)*/
#ifdef RT_LV_USE_LINEMETER
#define LV_USE_LINEMETER   1
#endif
#if LV_USE_LINEMETER
/* Draw line more precisely at cost of performance.
 * Useful if there are lot of lines any minor are visible
 * 0: No extra precision
 * 1: Some extra precision
 * 2: Best precision
 */
#  define LV_LINEMETER_PRECISE    0
#endif

/*Mask (dependencies: -)*/
#ifdef RT_LV_USE_OBJMASK
#define LV_USE_OBJMASK  1
#endif

/*Message box (dependencies: lv_rect, lv_btnm, lv_label)*/
#ifdef RT_LV_USE_MSGBOX
#define LV_USE_MSGBOX     1
#endif

/*Page (dependencies: lv_cont)*/
#ifdef RT_LV_USE_PAGE
#define LV_USE_PAGE     1
#endif
#if LV_USE_PAGE != 0
/*Focus default animation time [ms] (0: no animation)*/
#  define LV_PAGE_DEF_ANIM_TIME     400
#endif

/*Preload (dependencies: lv_arc, lv_anim)*/
#ifdef RT_LV_USE_SPINNER
#define LV_USE_SPINNER      1
#endif
#if LV_USE_SPINNER != 0
#  define LV_SPINNER_DEF_ARC_LENGTH   60      /*[deg]*/
#  define LV_SPINNER_DEF_SPIN_TIME    1000    /*[ms]*/
#  define LV_SPINNER_DEF_ANIM         LV_SPINNER_TYPE_SPINNING_ARC
#endif

/*Roller (dependencies: lv_ddlist)*/
#ifdef RT_LV_USE_ROLLER
#define LV_USE_ROLLER    1
#endif
#if LV_USE_ROLLER != 0
/*Focus animation time [ms] (0: no animation)*/
#  define LV_ROLLER_DEF_ANIM_TIME     200

/*Number of extra "pages" when the roller is infinite*/
#  define LV_ROLLER_INF_PAGES         7
#endif

/*Slider (dependencies: lv_bar)*/
#ifdef RT_LV_USE_SLIDER
#define LV_USE_SLIDER    1
#endif

/*Spinbox (dependencies: lv_ta)*/
#ifdef RT_LV_USE_SPINBOX
#define LV_USE_SPINBOX       1
#endif

/*Switch (dependencies: lv_slider)*/
#ifdef RT_LV_USE_SWITCH
#define LV_USE_SWITCH       1
#endif

/*Text area (dependencies: lv_label, lv_page)*/
#ifdef RT_LV_USE_TEXTAREA
#define LV_USE_TEXTAREA       1
#endif
#if LV_USE_TEXTAREA != 0
#  define LV_TEXTAREA_DEF_CURSOR_BLINK_TIME 400     /*ms*/
#  define LV_TEXTAREA_DEF_PWD_SHOW_TIME     1500    /*ms*/
#endif

/*Table (dependencies: lv_label)*/
#ifdef RT_LV_USE_TABLE
#define LV_USE_TABLE    1
#endif
#if LV_USE_TABLE
#  define LV_TABLE_COL_MAX    12
#endif

/*Tab (dependencies: lv_page, lv_btnm)*/
#ifdef RT_LV_USE_TABVIEW
#define LV_USE_TABVIEW      1
#endif
#  if LV_USE_TABVIEW != 0
/*Time of slide animation [ms] (0: no animation)*/
#  define LV_TABVIEW_DEF_ANIM_TIME    300
#endif

/*Tileview (dependencies: lv_page) */
#ifdef RT_LV_USE_TILEVIEW
#define LV_USE_TILEVIEW     1
#endif
#if LV_USE_TILEVIEW
/*Time of slide animation [ms] (0: no animation)*/
#  define LV_TILEVIEW_DEF_ANIM_TIME   300
#endif

/*Window (dependencies: lv_cont, lv_btn, lv_label, lv_img, lv_page)*/
#ifdef RT_LV_USE_WIN
#define LV_USE_WIN      1
#endif

/*==================
 * Non-user section
 *==================*/

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)    /* Disable warnings for Visual Studio*/
#  define _CRT_SECURE_NO_WARNINGS
#endif

/*--END OF LV_CONF_H--*/

/*Be sure every define has a default value*/
#include "lvgl/src/lv_conf_internal.h"

#endif /*LV_CONF_H*/

#endif /*End of "Content enable"*/
