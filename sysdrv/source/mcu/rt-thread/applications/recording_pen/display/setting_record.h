/*
 * Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-04-26     Jair Wu      First version
 *
 */

#ifndef __SETTING_RECORD_H__
#define __SETTING_RECORD_H__

#include "setting_page.h"

void setting_record_init(int p_page);
void setting_record_deinit(void);
void setting_record_key(size_t type, size_t value);

#endif
