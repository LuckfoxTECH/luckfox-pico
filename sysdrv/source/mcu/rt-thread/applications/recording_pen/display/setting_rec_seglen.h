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

#ifndef __SETTING_REC_SEGMENT_H__
#define __SETTING_REC_SEGMENT_H__

#include "setting_page.h"

void reset_seglen(void);
int get_seglen(void);
void setting_seglen_init(int p_page);
void setting_seglen_deinit(void);
void setting_seglen_key(size_t type, size_t value);

#endif
