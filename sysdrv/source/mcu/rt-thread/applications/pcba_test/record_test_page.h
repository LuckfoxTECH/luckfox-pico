/*
 * @Copyright: Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * @License: SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-20 10:20:18
 * @Company: Rockchip
 * @LastEditTime: 2020-06-29 11:25:07
 * @LastEditors: Do not edit
 * @Description:
 */

#ifndef __RECORD_TEST_PAGE_H__
#define __RECORD_TEST_PAGE_H__

#include "pcba_def.h"

extern void record_page_init(int p_page);
extern void record_page_deinit(void);
extern void record_page_refresh(size_t type, size_t value);
extern void record_page_key(size_t type, size_t value);

#endif  //__RECORD_TEST_PAGE_H__