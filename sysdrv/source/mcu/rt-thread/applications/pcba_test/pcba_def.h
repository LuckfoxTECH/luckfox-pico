/*
 * Copyright: Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * License: SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-12 11:51:25
 * @Company: Rockchip
 * @LastEditTime: 2020-05-18 17:21:11
 * @LastEditors: Do not edit
 * @Description:
 */

#ifndef __RECPEN_DISPLAY_H__
#define __RECPEN_DISPLAY_H__

#define DISPLAY_DEVICE_NAME     "spi_screen"

#define SCREEN_W                176
#define SCREEN_H                220

#define STATE_BAR_W             SCREEN_W
#define STATE_BAR_H             40

#define LABEL_Y_START           STATE_BAR_H
#define LABEL_W                 SCREEN_W
#define LABEL_H                 20
#define LABEL_Y_GAP             5

#define TITLE_STR_H             45
#define BTN_W                   84
#define BTN_X_GAP               0
#define BTN_H                   24
#define BTN_Y_GAP               6

#endif