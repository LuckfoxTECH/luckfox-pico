/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#ifndef __APP_SETTING_MENU__
#define __APP_SETTING_MENU__
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

struct g_setting_menu_data_t
{
    rt_uint8_t *fb;
    rt_uint32_t fblen;

    rt_int16_t  mov_xdir;
    rt_int16_t  mov_ydir;
    rt_int16_t  mov_xoffset;
    rt_int16_t  mov_yoffset;
};
extern struct g_setting_menu_data_t *g_setting_menu_data;
extern struct app_main_touch_cb_t   app_setting_menu_touch_cb;
extern struct refrsh_param_t app_setting_menu_refrsh_param;

rt_err_t app_setting_menu_refrsh(struct rt_display_config *wincfg, void *param);

void app_setting_menu_init(void);

#endif
