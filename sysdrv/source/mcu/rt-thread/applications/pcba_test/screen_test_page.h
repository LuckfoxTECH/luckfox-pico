/*
 * Copyright: Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * License: SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-15 15:59:22
 * @Company: Rockchip
 * @LastEditTime: 2020-06-19 15:10:49
 * @LastEditors: Do not edit
 * @Description:
 */

#ifndef __SCREEN_TEST_PAGE_H__
#define __SCREEN_TEST_PAGE_H__

#include "pcba_def.h"

extern void screen_page_init(int p_page);
extern void screen_page_deinit(void);
extern void screen_page_refresh(size_t type, size_t value);
extern void screen_page_key(size_t type, size_t value);

#endif  //__SCREEN_TEST_PAGE_H__