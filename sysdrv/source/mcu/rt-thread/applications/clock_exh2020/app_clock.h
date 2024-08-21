/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#ifndef __APP_CLOCK__
#define __APP_CLOCK__
#include <rtthread.h>

/*
 **************************************************************************************************
 *
 * Macro define
 *
 **************************************************************************************************
 */

/*
 **************************************************************************************************
 *
 * Struct & data define
 *
 **************************************************************************************************
 */

struct app_clock_data_t
{
    rt_uint8_t *bottom_fb;
    rt_uint32_t bottom_fblen;
    rt_uint8_t *top_fb;
    rt_uint32_t top_fblen;
    rt_uint8_t *bak_fb;
    rt_uint32_t bak_fblen;

    rt_int8_t   cur_style;
    rt_int8_t   switch_done;
    rt_int16_t  mov_dir;
    rt_int16_t  mov_offset;
};
extern struct app_clock_data_t    *app_clock_data;
extern struct app_main_touch_cb_t app_clock_touch_cb;
extern struct refrsh_param_t      app_clock_refrsh_param;

// Params
typedef struct
{
    rt_int8_t buf_id;
} clock_design_param_t;

/*
 **************************************************************************************************
 *
 * Declaration
 *
 **************************************************************************************************
 */

/**
 * App clock fast init.
 */
rt_err_t app_clock_win_refresh(struct rt_display_config *wincfg, void *param);

void app_clock_init(void);

void app_clock_update(void);

#endif
