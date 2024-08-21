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

#ifndef __SETTING_BACKLIGHT_H__
#define __SETTING_BACKLIGHT_H__

#include "setting_page.h"
#include "setting_backlight_timeout.h"
#include "setting_brightness.h"

void setting_backlight_init(int p_page);
void setting_backlight_deinit(void);
void setting_backlight_key(size_t type, size_t value);

#endif
