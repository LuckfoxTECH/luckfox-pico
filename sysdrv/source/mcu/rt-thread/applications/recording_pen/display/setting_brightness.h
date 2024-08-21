/*
 * Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-14     Jair Wu      First version
 *
 */

#ifndef __SETTING_BRIGHTNESS_H__
#define __SETTING_BRIGHTNESS_H__

#include "setting_page.h"

#define BRIGHTNESS_MAX         5
#define BRIGHTNESS_MIN         1
#define BRIGHTNESS_HW_MAX      100
#define BRIGHTNESS_HW_MIN      20

void setting_brightness_init(int p_page);
void setting_brightness_deinit(void);
void setting_brightness_key(size_t type, size_t value);

#endif
