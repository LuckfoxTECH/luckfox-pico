/*
 * Copyright: Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * License: SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-15 15:59:22
 * @Company: Rockchip
 * @LastEditTime: 2020-06-12 16:54:14
 * @LastEditors: Do not edit
 * @Description:
 */

#ifndef __MAIN_PAGE_H__
#define __MAIN_PAGE_H__

#include "pcba_app.h"

enum pcba_test_items
{
    ITEM_SCREEN,
    ITEM_KEY,
    ITEM_RECORD,
    ITEM_PHO_SPK,   //phone & speaker
    ITEM_EMMC,
    ITEM_BATTERY,
    ITEM_TEMP,      //temperate
    ITEM_RTC,
    ITEM_WIFI,
    ITEM_BT,
    ITEM_RESULT,
    ITEM_MAX,
};

extern char g_result_info[ITEM_MAX][8];
extern const char *pcba_test_item_str[ITEM_MAX];
/* release btn style */
extern lv_style_t style_tv_btn_rel;
/* press btn style */
extern lv_style_t style_tv_btn_pr;

extern void index_page_init(int p_page);
extern void index_page_deinit(void);
extern void index_page_refresh(size_t type, size_t value);
extern void index_page_key(size_t type, size_t value);

#endif //__MAIN_PAGE_H__