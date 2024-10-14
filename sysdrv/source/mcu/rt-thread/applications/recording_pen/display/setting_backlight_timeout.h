/*
 * Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-04-10     Jair Wu      First version
 *
 */

#ifndef __SETTING_BACKLIGHT_TIMEOUT_H__
#define __SETTING_BACKLIGHT_TIMEOUT_H__

#include "setting_page.h"

void reset_bl_timeout(void);
int get_bl_timeout(void);
void setting_bl_timeout_init(int p_page);
void setting_bl_timeout_deinit(void);
void setting_bl_timeout_key(size_t type, size_t value);

#endif