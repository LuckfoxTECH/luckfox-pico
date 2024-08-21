/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#ifndef __APP_ALPHA_WIN__
#define __APP_ALPHA_WIN__
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

struct g_alpha_win_data_t
{
    rt_uint8_t *fb;
    rt_uint32_t fblen;

    rt_int16_t  mov_xoffset;
};
extern struct g_alpha_win_data_t *g_alpha_win_data;
extern struct refrsh_param_t    alpha_win_refrsh_param;

rt_err_t alpha_win_refresh(struct rt_display_config *wincfg, void *param);

void app_alpha_win_init(void);

#endif
