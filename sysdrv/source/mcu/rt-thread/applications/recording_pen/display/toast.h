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

#ifndef __TOAST_H__
#define __TOAST_H__

#include "recpen.h"

void toast_create(char *text, uint32_t ms);
void toast_refresh(void);
void toast_destory(void);

#endif
