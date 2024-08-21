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

#ifndef __SETTING_LANGUAGE_H__
#define __SETTING_LANGUAGE_H__

#include "setting_page.h"

void setting_language_init(int p_page);
void setting_language_deinit(void);
void setting_language_key(size_t type, size_t value);

#endif
