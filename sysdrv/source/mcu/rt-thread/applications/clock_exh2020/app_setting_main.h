/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#ifndef __APP_SETTING_MAIN__
#define __APP_SETTING_MAIN__
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

/*
 **************************************************************************************************
 *
 * Declaration
 *
 **************************************************************************************************
 */

struct app_setting_main_data_t
{
    rt_uint8_t *fb;
    rt_uint32_t fblen;

    rt_int16_t  mov_dir;
    rt_int16_t  mov_offset;
};
extern struct app_setting_main_data_t *g_setting_main_data;
extern struct app_main_touch_cb_t      app_setting_main_touch_cb;
extern struct refrsh_param_t app_setting_main_refrsh_param;

rt_err_t app_setting_main_refresh(struct rt_display_config *wincfg, void *param);

void app_setting_main_init(void);

#endif
