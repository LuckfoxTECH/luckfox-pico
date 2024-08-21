/*
 * @Copyright: Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * @License: SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-20 10:20:08
 * @Company: Rockchip
 * @LastEditTime: 2020-06-29 11:48:45
 * @LastEditors: Do not edit
 * @Description:
 */

#include "main_page.h"
#include "pcba_public.h"
#include "record_test_page.h"

#define DBG_SECTION_NAME    "RECORD_PAGE"
#define DBG_LEVEL           DBG_LOG
#include "rtdbg.h"

static lv_obj_t *record_page = NULL;
static lv_obj_t *divide_line = NULL;
static lv_obj_t *company_label = NULL;
static lv_obj_t *item_label = NULL;
static lv_obj_t *pass_btn = NULL;
static lv_obj_t *fail_btn = NULL;
static lv_obj_t *file_name_label = NULL;
static lv_obj_t *record_time_label = NULL;

static int draw_divide_line(void)
{
    static lv_point_t line_points[] = {{0, STATE_BAR_H - 1}, {STATE_BAR_W, STATE_BAR_H - 1}};
    static lv_style_t style_line;
    lv_style_copy(&style_line, &lv_style_plain);
    style_line.line.color = LV_COLOR_NAVY;
    style_line.line.width = 1;
    style_line.body.main_color = LV_COLOR_BLACK;
    style_line.body.grad_color = LV_COLOR_BLACK;

    lv_line_set_points(divide_line, line_points, 2);
    lv_line_set_style(divide_line, 0, &style_line);
    lv_obj_align(divide_line, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_invalidate(divide_line);

    return 0;
}

static void btn_event_handler(lv_obj_t * obj, lv_event_t event)
{
    char *result = NULL;
    if (event == LV_EVENT_CLICKED)
    {
        if (obj == pass_btn)
        {
            LOG_D("Clicked Pass button");
            result = "OK";
            memcpy (g_result_info[ITEM_RECORD], result, strlen(result));
        }
        else
        {
            LOG_D("Clicked Fail button");
            result = "NG";
            memcpy (g_result_info[ITEM_RECORD], result, strlen(result));
        }

        display_pcba_page(MENU_MAIN_PAGE);
    }
}

static int draw_result_btn(void)
{
    lv_obj_t * label;

    /* Create PASS button */
    pass_btn = lv_btn_create(record_page, NULL);
    lv_obj_set_event_cb(pass_btn, btn_event_handler);
    lv_obj_set_size(pass_btn, BTN_W, BTN_H);
    lv_obj_align(pass_btn, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
    lv_btn_set_style(pass_btn, LV_BTN_STYLE_PR,  &style_tv_btn_pr);
    lv_btn_set_style(pass_btn, LV_BTN_STYLE_REL,  &style_tv_btn_rel);
    /* Create FAIL button */
    fail_btn = lv_btn_create(record_page, NULL);
    lv_obj_set_event_cb(fail_btn, btn_event_handler);
    lv_obj_set_size(fail_btn, BTN_W, BTN_H);
    lv_obj_align(fail_btn, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);
    lv_btn_set_style(fail_btn, LV_BTN_STYLE_PR,  &style_tv_btn_pr);
    lv_btn_set_style(fail_btn, LV_BTN_STYLE_REL,  &style_tv_btn_rel);

    label = lv_label_create(pass_btn, NULL);
    lv_label_set_recolor(label, true);
    lv_label_set_text(label, "#00ff00 PASS#");
    lv_obj_set_style(label, &lv_style_pretty);

    label = lv_label_create(fail_btn, NULL);
    lv_label_set_recolor(label, true);
    lv_label_set_text(label, "#ff0000 FAIL#");
    lv_obj_set_style(label, &lv_style_pretty);

    return 0;
}

static rt_err_t draw_media_info(void)
{
    char str[32];
    uint32_t time;

    /* draw current time */
    memset((void *)str, 0x0, 10);
    time = g_pcba.record_time / 1000;
    uint16_t hour = time / 60 / 60;
    time %= 60 * 60;
    uint16_t min = time / 60;
    uint16_t sec = time % 60;
    rt_snprintf(str, sizeof(str), "%02d:%02d:%02d", hour, min, sec);

    lv_label_set_text(record_time_label, str);

    return RT_EOK;
}

/**
 * @description: pcba record test page init
 * @param {int page}
 * @return: none
 */
void record_page_init(int p_page)
{
    short int y = 0;
    char file_path[MAX_FILE_NAME];
    char str_test_item[64];
    g_pcba.record_time = 0;
    pcba_recorder_prepare(file_path);

    if (!record_page)
    {
        record_page = lv_obj_create(lv_scr_act(), NULL);

        lv_obj_set_size(record_page, SCREEN_W, SCREEN_H);
        lv_obj_set_style(record_page, &label_style_font16);
        lv_obj_align(record_page, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
        lv_obj_invalidate(record_page);

        divide_line = lv_line_create(record_page, NULL);

        company_label = lv_label_create(record_page, NULL);
        lv_label_set_long_mode(company_label, LV_LABEL_LONG_SROLL_CIRC);
        lv_label_set_align(company_label, LV_LABEL_ALIGN_CENTER);
        lv_obj_set_size(company_label, LABEL_W, LABEL_H);
        lv_obj_align(company_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);
        lv_label_set_text(company_label, "Rockchip Co.,Ltd.");
        lv_obj_set_style(company_label, &label_style_font16);

        item_label = lv_label_create(record_page, NULL);
        y = LABEL_H;
        lv_label_set_long_mode(item_label, LV_LABEL_LONG_SROLL_CIRC);
        lv_label_set_align(item_label, LV_LABEL_ALIGN_CENTER);
        lv_label_set_recolor(item_label, true);
        lv_obj_set_size(item_label, LABEL_W, LABEL_H);
        lv_obj_align(item_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);
        rt_snprintf(str_test_item, sizeof(str_test_item), "#00ff00 %s#", pcba_test_item_str[ITEM_RECORD]);
        lv_label_set_text(item_label, str_test_item);

        lv_obj_set_style(item_label, &label_style_font16);

        y = 100;
        file_name_label = lv_label_create(record_page, NULL);
        lv_label_set_long_mode(file_name_label, LV_LABEL_LONG_SROLL_CIRC);
        lv_label_set_align(file_name_label, LV_LABEL_ALIGN_CENTER);
        lv_obj_set_size(file_name_label, LABEL_W, LABEL_H);
        lv_obj_align(file_name_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);
        lv_label_set_text(file_name_label,
                          &file_path[strlen(RECORD_DIR_PATH) + 1]);
        lv_obj_invalidate(file_name_label);

        y += LABEL_H;
        record_time_label = lv_label_create(record_page, NULL);
        lv_obj_set_style(record_time_label, &label_style_font20_hl);
        lv_label_set_long_mode(record_time_label, LV_LABEL_LONG_BREAK);
        lv_label_set_align(record_time_label, LV_LABEL_ALIGN_CENTER);
        lv_label_set_recolor(record_time_label, true);
        lv_obj_set_size(record_time_label, LABEL_W, LABEL_H);
        lv_obj_align(record_time_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);

        draw_media_info();
    }

    draw_divide_line();
    draw_result_btn();
}

/**
 * @description:
 * @param {type}
 * @return:
 */
void record_page_deinit(void)
{
    if (record_page)
    {
        /* this function will delete children too */
        lv_obj_del(record_page);
        record_page = NULL;
    }
}

void record_page_key(size_t type, size_t value)
{
    if (type == TYPE_KEY_PRESS)
    {
        switch (value)
        {
        case KEY_RECORD:
            if (rootfs_check())
                goto NO_ROOTFS;
            pcba_recorder_stop();

            // if (display_pcba_page(MENU_MAIN_PAGE) == RT_EOK)
            //     g_item_index = MENU_NONE_PAGE;
            break;

        case KEY_UP:
            pcba_recorder_stop();
            lv_btn_set_state(pass_btn,LV_BTN_STATE_PR);
            btn_event_handler(pass_btn, LV_EVENT_CLICKED);
            break;

        case KEY_DOWN:
            pcba_recorder_stop();
            lv_btn_set_state(fail_btn,LV_BTN_STATE_PR);
            btn_event_handler(fail_btn, LV_EVENT_CLICKED);
            break;

        default:
            break;
        }
    }

    return;

NO_ROOTFS:
    LOG_E("No rootfs, please check");
}

void record_page_refresh(size_t type, size_t value)
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

        pcba_recorder_prepare(file_path);
        lv_label_set_text(file_name_label,
                          &file_path[strlen(RECORD_DIR_PATH) + 1]);
        break;
    default:
        break;
    }
}
