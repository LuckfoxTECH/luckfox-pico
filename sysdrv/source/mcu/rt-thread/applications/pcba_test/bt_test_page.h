/*
 * Copyright: Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * License: SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-20 10:31:31
 * @Company: Rockchip
 * @LastEditTime: 2020-05-21 10:24:20
 * @LastEditors: Do not edit
 * @Description:
 */

#ifndef __BT_TEST_PAGE_H__
#define __BT_TEST_PAGE_H__

#include "pcba_def.h"

extern void bt_page_init(int p_page);
extern void bt_page_deinit(void);
extern void bt_page_refresh(size_t type, size_t value);
extern void bt_page_key(size_t type, size_t value);

#endif