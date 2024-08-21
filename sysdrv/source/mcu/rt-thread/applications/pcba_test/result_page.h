/*
 * Copyright: Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * License: SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-20 11:49:48
 * @Company: Rockchip
 * @LastEditTime: 2020-06-19 15:10:01
 * @LastEditors: Do not edit
 * @Description:
 */

#ifndef __RESULT_PAGE_H__
#define __RESULT_PAGE_H__

#include "pcba_def.h"

extern void result_page_init(int p_page);
extern void result_page_deinit(void);
extern void result_page_refresh(size_t type, size_t value);
extern void result_page_key(size_t type, size_t value);

#endif  //__RESULT_PAGE_H__