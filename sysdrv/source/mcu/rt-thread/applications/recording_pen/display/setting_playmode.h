/*
 * Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-04-24     Jair Wu      First version
 *
 */

#ifndef __SETTING_PLAYMODE_H__
#define __SETTING_PLAYMODE_H__

#include "setting_page.h"

void setting_playmode_init(int p_page);
void setting_playmode_deinit(void);
void setting_playmode_key(size_t type, size_t value);

#endif