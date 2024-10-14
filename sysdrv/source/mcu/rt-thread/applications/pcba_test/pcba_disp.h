/*
 * Copyright: Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * License: SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-12 11:25:27
 * @Company: Rockchip
 * @LastEditTime: 2020-05-15 16:37:06
 * @LastEditors: Do not edit
 * @Description:
 */

#ifndef __PCBA_MAIN_PAGE_H__
#define __PCBA_MAIN_PAGE_H__

#include "pcba_app.h"
#include "pcba_def.h"

#define DISPLAY_DEVICE_NAME     "spi_screen"

/* display win layers */
#define LVGL_HOME_GRAY1_WIN     0
#define LVGL_CLOCK_RGB332_WIN   1
#define LVGL_LOGO_RGB565_WIN    2

#define SCREEN_W                176
#define SCREEN_H                220

struct lvgl_page_func
{
    char *name;
    void (*init)(int p_page);
    void (*deinit)(void);
    void (*refresh)(size_t type, size_t value);
    void (*key)(size_t type, size_t value);
};

extern lv_style_t page_pcba_style;
extern lv_style_t label_style_font20;
extern lv_style_t label_style_font20_hl;
extern lv_style_t label_style_font20_sel;
extern lv_style_t label_style_font16;

LV_FONT_DECLARE(lv_font_16);
LV_FONT_DECLARE(lv_font_20);

rt_err_t display_pcba_page(int p_page);
rt_err_t pcba_display_init(void);

#endif