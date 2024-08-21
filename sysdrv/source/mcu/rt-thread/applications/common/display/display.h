/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    display.h
  * @version V0.1
  * @brief   display
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-08-01     Tony.zheng      first implementation
  *
  ******************************************************************************
  */

#ifndef __DISPLAY_H__
#define __DISPLAY_H__
#include <rtthread.h>

/* Macro define */
#undef  MIN
#define MIN(X, Y)           ((X)<(Y)?(X):(Y))
#undef  MAX
#define MAX(X, Y)           ((X)>(Y)?(X):(Y))
#undef  ABS
#define ABS(X)              (((X) < 0) ? (-(X)) : (X))

#define GAMMA_RED           1.0
#define GAMMA_GREEN         1.0
#define GAMMA_BLUE          1.0
#define FORMAT_RGB_332      0
#define FORMAT_BGR_233      1
#define FORMAT_INDEX8       2

#define WSCALE              1
#define HSCALE              1

#if defined(RT_USING_VOP)
/**
 * Global data struct for olpc display demo
 */
struct rt_display_lut
{
    rt_uint8_t  winId;
    rt_uint8_t  format;
    rt_uint32_t *lut;
    rt_uint32_t size;
};
#endif

struct rt_display_config
{
    struct rt_display_config *next;

    rt_uint8_t  *fb;
    rt_uint32_t fblen;
    rt_uint8_t  format;

    rt_uint16_t x;
    rt_uint16_t y;
    rt_uint16_t w;
    rt_uint16_t h;

#if defined(RT_USING_VOP)
    rt_uint8_t  winId;
    rt_uint8_t  zpos;
    rt_uint32_t *lut;
    rt_uint32_t lutsize;
    rt_uint32_t new_lut;
    rt_uint32_t hide_win;
    rt_uint32_t colorkey;

    rt_uint8_t  alphaEn;
    rt_uint8_t  alphaMode;
    rt_uint8_t  alphaPreMul;
    rt_uint8_t  globalAlphaValue;

    rt_uint16_t xVir;
    rt_uint16_t xLoopOffset;
    rt_uint16_t yLoopOffset;

    rt_uint16_t ylast;
#endif
};

typedef enum
{
    WIN_BOTTOM_LAYER = 0,
#if defined(RT_USING_VOP)
    WIN_MIDDLE_LAYER,
    WIN_TOP_LAYER,
#endif
    WIN_MAX_LAYER
} WINLAYER_ID;

struct rt_display_mq_t
{
    rt_uint8_t cfgsta;                              // config status(bit 2-0), 0: unused, 1 used;
    struct rt_display_config win[WIN_MAX_LAYER];    // layers(bottom --> top): 0 --> 1 --> 2
    rt_err_t (*disp_finish)(void);
};

struct rt_display_data
{
    rt_device_t device;
    rt_mq_t     disp_mq;
    struct rt_device_graphic_info info;
    rt_uint16_t blval;
    rt_uint8_t  enable_cnt;

#if defined(RT_USING_VOP)
    struct rt_display_lut lut[3];
#endif
};
typedef struct rt_display_data *rt_display_data_t;

#if defined(RT_USING_VOP)
/**
 * color palette for RGB332
 */
extern uint32_t bpp_lut[256];
#endif

/**
 * Screen clear.
 */
rt_err_t rt_display_screen_clear(void);

/**
 * Get global display data struct.
 */
rt_display_data_t rt_display_get_disp(void);


/**
 * Get value of backlight.
 */
rt_uint16_t rt_display_backlight_get(void);

/**
 * backlight set (0 ~ 100).
 */
rt_err_t rt_display_backlight_set(rt_uint16_t val);

/**
 * Win layers close
 */
void rt_display_win_layer_close(rt_uint8_t winid);

#endif
