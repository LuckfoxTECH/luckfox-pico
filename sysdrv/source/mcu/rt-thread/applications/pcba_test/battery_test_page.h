/*
 * Copyright: Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * License: SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-20 10:30:29
 * @Company: Rockchip
 * @LastEditTime: 2020-06-02 15:46:04
 * @LastEditors: Do not edit
 * @Description:
 */

#ifndef __BATTERY_TEST_PAGE_H__
#define __BATTERY_TEST_PAGE_H__

#include "pcba_def.h"

extern void batt_page_init(int p_page);
extern void batt_page_deinit(void);
extern void batt_page_refresh(size_t type, size_t value);
extern void batt_page_key(size_t type, size_t value);

#endif