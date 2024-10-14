/*
 * Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-03-04     Jair Wu      First version
 *
 */

#ifndef __RECPEN_DISPLAY_H__
#define __RECPEN_DISPLAY_H__
#include "image_info.h"

#ifdef RT_USING_VOP
#define DISPLAY_DEVICE_NAME     "lcd"
#else
#define DISPLAY_DEVICE_NAME     "spi_screen"
#endif

#if defined(RT_LV_HOR_RES) && defined(RT_LV_VER_RES)
#define SCREEN_W                RT_LV_HOR_RES
#define SCREEN_H                RT_LV_VER_RES
#else
#define SCREEN_W                176
#define SCREEN_H                220
#endif

#define STATE_BAR_W             SCREEN_W
#define STATE_BAR_H             60

#define ICON_Y_START            2
#define ICON_W                  16
#define ICON_H                  16
#define ICON_X_GAP              2

#define LABEL_Y_START           (40)
#define LABEL_W                 SCREEN_W
#define LABEL_H                 20
#define LABEL_Y_GAP             4

#define SETTING_LABEL_Y_START   25
#define SETTING_LABEL_W         LABEL_W
#define SETTING_LABEL_H         LABEL_H
#define SETTING_LABEL_GAP       LABEL_Y_GAP

#define RECPEN_COLOR_WHITE      LV_COLOR_MAKE(0xff, 0xff, 0xff)
#define RECPEN_COLOR_LIGHT_GRAY LV_COLOR_MAKE(0xdf, 0xdf, 0xdf)
#define RECPEN_COLOR_BLUE       LV_COLOR_MAKE(0x11, 0xC1, 0xB7)
#define RECPEN_COLOR_LIGHT_BLUE LV_COLOR_MAKE(0x04, 0x91, 0xc6)
#define RECPEN_COLOR_DEEP_BLUE  LV_COLOR_MAKE(0x4b, 0x50, 0x64)
#define RECPEN_COLOR_BLACK      LV_COLOR_MAKE(0x2e, 0x40, 0x50)
#define RECPEN_COLOR_ORINGE     LV_COLOR_MAKE(0xff, 0xa9, 0x80)

#define RECPEN_COLOR_TEXT       RECPEN_COLOR_WHITE
#define RECPEN_COLOR_TEXT_HL    RECPEN_COLOR_ORINGE
#define RECPEN_COLOR_BAR        RECPEN_COLOR_WHITE
#define RECPEN_COLOR_BAR_2      RECPEN_COLOR_LIGHT_GRAY
#define RECPEN_COLOR_BG         RECPEN_COLOR_DEEP_BLUE

extern lv_style_t page_common_style;
extern lv_style_t label_style_font20_letter;
extern lv_style_t label_style_font20;
extern lv_style_t label_style_font20_hl;
extern lv_style_t label_style_font20_sel;
extern lv_style_t label_style_font16;
extern lv_style_t style_bar;

LV_FONT_DECLARE(lv_font_16);
LV_FONT_DECLARE(lv_font_20);

rt_err_t display_page(int p_page);
rt_err_t recpen_display_init(void);
void *lvgl_img_dsc_init(lv_img_dsc_t *dsc, image_info_t *info);

#endif // __RECPEN_DISPLAY_H__
