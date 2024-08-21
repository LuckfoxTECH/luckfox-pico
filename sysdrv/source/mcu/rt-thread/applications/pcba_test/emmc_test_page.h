/*
 * Copyright: Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * License: SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-20 10:30:10
 * @Company: Rockchip
 * @LastEditTime: 2020-05-21 20:53:27
 * @LastEditors: Do not edit
 * @Description:
 */

#ifndef __EMMC_TEST_PAGE_H__
#define __EMMC_TEST_PAGE_H__

#include "pcba_def.h"

extern void emmc_page_init(int p_page);
extern void emmc_page_deinit(void);
extern void emmc_page_refresh(size_t type, size_t value);
extern void emmc_page_key(size_t type, size_t value);

#endif