/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#ifndef __APP_DIALOG_BOX_H__
#define __APP_DIALOG_BOX_H__
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

struct g_dialog_box_data_t
{
    rt_uint8_t *fb;
    rt_uint32_t fblen;
    rt_uint8_t  mode;
};
extern struct g_dialog_box_data_t *g_dialog_box_data;
extern struct app_main_touch_cb_t  app_dialog_box_touch_cb;
extern struct refrsh_param_t app_dialog_box_refrsh_param;

rt_err_t app_dialog_box_refrsh(struct rt_display_config *wincfg, void *param);

void app_dialog_box_init(void);

#endif
