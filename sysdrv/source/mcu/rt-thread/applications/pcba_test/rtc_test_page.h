/*
 * @Copyright: Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * @License: SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-20 10:32:07
 * @Company: Rockchip
 * @LastEditTime: 2020-06-29 11:28:39
 * @LastEditors: Do not edit
 * @Description:
 */

#ifndef __RTC_TEST_PAGE_H__
#define __RTC_TEST_PAGE_H__

#include "pcba_def.h"

extern void rtc_page_init(int p_page);
extern void rtc_page_deinit(void);
extern void rtc_page_refresh(size_t type, size_t value);
extern void rtc_page_key(size_t type, size_t value);

#endif  //__RTC_TEST_PAGE_H__