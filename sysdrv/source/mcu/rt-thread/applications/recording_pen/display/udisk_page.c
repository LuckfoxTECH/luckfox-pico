/*
 * Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-04-30     Jair Wu      First version
 *
 */

#include "udisk_page.h"

#define DBG_SECTION_NAME    "UDISKPAGE"
#define DBG_LEVEL           DBG_INFO
#include "rtdbg.h"

extern lv_img_dsc_t img_udisk;
static lv_obj_t *udisk_page;
static lv_obj_t *g_img;
static lv_obj_t *g_label;

static const char udisk_prompt[RECPEN_APP_LANG_MAX][MAX_LABEL_LEN] =
{
    "U盘使用中",
    "Udisk is enable"
};

void udisk_page_init(int p_page)
{
    g_app_info.state = RECPEN_APP_STATE_BUSY;
    udisk_page = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_set_size(udisk_page, SCREEN_W, SCREEN_H);
    lv_obj_add_style(udisk_page, LV_OBJ_PART_MAIN, &page_common_style);
    lv_obj_align(udisk_page, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_invalidate(udisk_page);

    g_img = lv_img_create(udisk_page, NULL);
    lv_obj_set_size(g_img, img_udisk.header.w, img_udisk.header.h);
    lv_obj_align(g_img, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_img_set_src(g_img, &img_udisk);
    lv_obj_invalidate(g_img);

    g_label = lv_label_create(udisk_page, NULL);
    lv_obj_add_style(g_label, LV_OBJ_PART_MAIN, &label_style_font20);
    lv_label_set_long_mode(g_label, LV_LABEL_LONG_BREAK);
    lv_label_set_align(g_label, LV_LABEL_ALIGN_CENTER);
    lv_label_set_text(g_label, udisk_prompt[g_app_info.language]);
    lv_obj_set_size(g_label, LABEL_W, LABEL_H);
    lv_obj_align(g_label, NULL, LV_ALIGN_CENTER, 0, img_udisk.header.h / 2 + LABEL_H);
    lv_obj_invalidate(g_label);
}

void udisk_page_deinit(void)
{
    lv_obj_remove_style(g_label, LV_OBJ_PART_MAIN, &label_style_font20);
    lv_obj_remove_style(udisk_page, LV_OBJ_PART_MAIN, &page_common_style);
    g_app_info.state = RECPEN_APP_STATE_IDLE;
    lv_obj_del(udisk_page);
}
