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

#ifndef __RECORDING_PAGE_H__
#define __RECORDING_PAGE_H__

#include "recpen.h"

void recording_page_init(int p_page);
void recording_page_deinit(void);
void recording_page_key(size_t type, size_t value);
void recording_page_refresh(size_t type, size_t value);
void recording_page_err_exit(int info);

#endif
