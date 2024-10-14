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

#include "recording_page.h"

#define DBG_SECTION_NAME    "RECPAGE"
#define DBG_LEVEL           DBG_INFO
#include "rtdbg.h"

static lv_obj_t *recording_page = NULL;
static lv_obj_t *file_name_label = NULL;
static lv_obj_t *record_time_label = NULL;
static enum recpen_app_menu_page g_pPage = MENU_NONE_PAGE;

static rt_err_t draw_media_info(void)
{
    char str[32];
    uint32_t time;

    /* draw current time */
    memset((void *)str, 0x0, 10);
    time = g_app_info.record_time / 1000;
    uint16_t hour = time / 60 / 60;
    time %= 60 * 60;
    uint16_t min = time / 60;
    uint16_t sec = time % 60;
    sprintf(str, "%02d:%02d:%02d", hour, min, sec);

    lv_label_set_text(record_time_label, str);

    return RT_EOK;
}

void recording_page_refresh(size_t type, size_t value)
{
    char file_path[MAX_FILE_NAME];

    switch (type)
    {
    case TYPE_REFRESH:
        if (value & REFRESH_MEDIA_INFO)
            draw_media_info();
        break;
    case TYPE_RECORD_DONE:
        LOG_I("[0x%08x] KEY_RECORD_NEXT", HAL_GetTick());
        recpen_recorder_prepare(file_path);
        lv_label_set_text(file_name_label,
                          &file_path[strlen(RECPEN_RECORD_FILE_DIR) + 1]);
        break;
    default:
        break;
    }
}

void recording_page_err_exit(int info)
{
    if (display_page(g_pPage) == RT_EOK)
        g_pPage = MENU_NONE_PAGE;
}

void recording_page_key(size_t type, size_t value)
{
    if (type == TYPE_KEY_PRESS)
    {
        switch (value)
        {
        case KEY_RECORD:
            recpen_recorder_stop();
            if (display_page(g_pPage) == RT_EOK)
                g_pPage = MENU_NONE_PAGE;
            break;
        default:
            break;
        }
    }
}

void recording_page_init(int p_page)
{
    char file_path[MAX_FILE_NAME];
    int y;

    g_app_info.record_time = 0;
    recpen_recorder_prepare(file_path);
    if (!recording_page)
    {
        if (g_pPage == MENU_NONE_PAGE)
            g_pPage = p_page;

        recording_page = lv_obj_create(lv_scr_act(), NULL);
        lv_obj_add_style(recording_page, LV_OBJ_PART_MAIN, &page_common_style);
        lv_obj_set_size(recording_page, SCREEN_W, SCREEN_H);
        lv_obj_align(recording_page, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
        lv_obj_invalidate(recording_page);

        y = 100;
        file_name_label = lv_label_create(recording_page, NULL);
        lv_label_set_long_mode(file_name_label, LV_LABEL_LONG_SROLL_CIRC);
        lv_label_set_align(file_name_label, LV_LABEL_ALIGN_CENTER);
        lv_obj_set_size(file_name_label, LABEL_W, LABEL_H);
        lv_obj_align(file_name_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);
        lv_label_set_text(file_name_label,
                          &file_path[strlen(RECPEN_RECORD_FILE_DIR) + 1]);
        lv_obj_invalidate(file_name_label);

        y += LABEL_H;
        record_time_label = lv_label_create(recording_page, NULL);
        lv_obj_add_style(record_time_label, LV_OBJ_PART_MAIN, &label_style_font20_hl);
        lv_label_set_long_mode(record_time_label, LV_LABEL_LONG_BREAK);
        lv_label_set_align(record_time_label, LV_LABEL_ALIGN_CENTER);
        lv_label_set_recolor(record_time_label, true);
        lv_obj_set_size(record_time_label, LABEL_W, LABEL_H);
        lv_obj_align(record_time_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);
        draw_media_info();
    }
}

void recording_page_deinit(void)
{
    if (recording_page)
    {
        lv_obj_remove_style(record_time_label, LV_OBJ_PART_MAIN, &label_style_font20_hl);
        lv_obj_remove_style(recording_page, LV_OBJ_PART_MAIN, &page_common_style);

        /* this function will delete children too */
        lv_obj_del(recording_page);
        recording_page = NULL;
    }
}
