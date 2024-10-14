/*
 * Copyright: Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * License: SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-20 10:31:55
 * @Company: Rockchip
 * @LastEditTime: 2020-06-29 11:50:25
 * @LastEditors: Do not edit
 * @Description:
 */

#include "main_page.h"
#include "rtc_test_page.h"

#define DBG_SECTION_NAME    "RTC_PAGE"
#define DBG_LEVEL           DBG_LOG
#include "rtdbg.h"

static lv_obj_t *rtc_page = NULL;
static lv_obj_t *divide_line = NULL;
static lv_obj_t *company_label = NULL;
static lv_obj_t *item_label = NULL;
static lv_obj_t *pass_btn = NULL;
static lv_obj_t *fail_btn = NULL;
static lv_task_t *rtc_test_task = NULL;

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
            memcpy (g_result_info[ITEM_RTC], result, strlen(result));
        }
        else
        {
            LOG_D("Clicked Fail button");
            result = "NG";
            memcpy (g_result_info[ITEM_RTC], result, strlen(result));
        }

        lv_task_del(rtc_test_task);
        rtc_test_task = NULL;
        rtc_page_deinit();

        display_pcba_page(MENU_MAIN_PAGE);
    }
}

static int draw_result_btn(void)
{
    lv_obj_t * label;
    /* Create PASS button */
    pass_btn = lv_btn_create(rtc_page, NULL);
    lv_obj_set_event_cb(pass_btn, btn_event_handler);
    lv_obj_set_size(pass_btn, BTN_W, BTN_H);
    lv_obj_align(pass_btn, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
    lv_btn_set_style(pass_btn, LV_BTN_STYLE_PR,  &style_tv_btn_pr);
    lv_btn_set_style(pass_btn, LV_BTN_STYLE_REL,  &style_tv_btn_rel);

    /* Create FAIL button */
    fail_btn = lv_btn_create(rtc_page, NULL);
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

static lv_obj_t *time_label, *time_lb;
static lv_obj_t *date_label, *date_lb;
static int draw_rtc_info(void)
{
    char str[32];

    memset((void *)str, 0x0, sizeof(str));
    rt_snprintf(str, sizeof(str), "%02d-%02d-%02d",
            g_pcba.date->tm_year + 1900,
            g_pcba.date->tm_mon + 1,
            g_pcba.date->tm_mday);

    lv_label_set_text(date_lb, str);

    memset((void *)str, 0x0, sizeof(str));
    rt_snprintf(str, sizeof(str), "%02d:%02d:%02d",
            g_pcba.date->tm_hour,
            g_pcba.date->tm_min,
            g_pcba.date->tm_sec);
    lv_label_set_text(time_lb, str);
    return 0;
}

/**
 * @description: pcba key test page init
 * @param {int page}
 * @return: none
 */
void rtc_page_init(int p_page)
{
    short int y = 0;
    char str_test_item[64];

    if (!rtc_page)
    {
        rtc_page = lv_obj_create(lv_scr_act(), NULL);

        lv_obj_set_size(rtc_page, SCREEN_W, SCREEN_H);
        lv_obj_set_style(rtc_page, &label_style_font16);
        lv_obj_invalidate(rtc_page);

        divide_line = lv_line_create(rtc_page, NULL);

        company_label = lv_label_create(rtc_page, NULL);
        lv_label_set_long_mode(company_label, LV_LABEL_LONG_SROLL_CIRC);
        lv_label_set_align(company_label, LV_LABEL_ALIGN_CENTER);
        lv_obj_set_size(company_label, LABEL_W, LABEL_H);
        lv_obj_set_style(company_label, &label_style_font16);
        lv_obj_align(company_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);
        lv_label_set_text(company_label, "Rockchip Co.,Ltd.");

        item_label = lv_label_create(rtc_page, NULL);
        y = LABEL_H;
        lv_label_set_long_mode(item_label, LV_LABEL_LONG_SROLL_CIRC);
        lv_label_set_align(item_label, LV_LABEL_ALIGN_CENTER);
        lv_label_set_recolor(item_label, true);
        lv_obj_set_size(item_label, LABEL_W, LABEL_H);
        lv_obj_set_style(company_label, &label_style_font16);
        lv_obj_align(item_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);

        rt_snprintf(str_test_item, sizeof(str_test_item), "#00ff00 %s#", pcba_test_item_str[ITEM_RTC]);
        lv_label_set_text(item_label, str_test_item);

        time_label = lv_label_create(rtc_page, NULL);
        lv_label_set_recolor(time_label, true);
        lv_label_set_long_mode(time_label, LV_LABEL_LONG_EXPAND);
        lv_obj_align(time_label, NULL, LV_ALIGN_IN_LEFT_MID, 0, -20);
        lv_label_set_text(time_label, "time: ");
        lv_obj_set_style(time_label, &lv_style_pretty);

        date_label = lv_label_create(rtc_page, NULL);
        lv_label_set_recolor(date_label, true);
        lv_label_set_long_mode(date_label, LV_LABEL_LONG_EXPAND);
        lv_obj_align(date_label, NULL, LV_ALIGN_IN_LEFT_MID, 0, 0);
        lv_label_set_text(date_label, "data: ");
        lv_obj_set_style(date_label, &lv_style_pretty);

        /* time */
        static lv_style_t style1;
        lv_style_copy(&style1, &lv_style_pretty);
        style1.text.color = LV_COLOR_GREEN;

        time_lb = lv_label_create(rtc_page, NULL);
        lv_label_set_long_mode(time_lb, LV_LABEL_LONG_EXPAND);
        lv_obj_align(time_lb, NULL, LV_ALIGN_IN_LEFT_MID, 60, -20);
        lv_obj_set_style(time_lb, &style1);

        /* date */
        date_lb = lv_label_create(rtc_page, NULL);
        lv_label_set_long_mode(date_lb, LV_LABEL_LONG_EXPAND);
        lv_obj_align(date_lb, NULL, LV_ALIGN_IN_LEFT_MID, 60, 0);
        lv_obj_set_style(date_lb, &style1);
    }

    draw_divide_line();

    draw_rtc_info();

    draw_result_btn();
}

/**
 * @description:
 * @param {type}
 * @return:
 */
void rtc_page_deinit(void)
{
    if (rtc_page)
    {
        /* this function will delete children too */
        lv_obj_del(rtc_page);
        rtc_page = NULL;
    }
}

void rtc_page_key(size_t type, size_t value)
{
    LOG_D("%s start", __func__);
    if (type == TYPE_KEY_PRESS)
    {
        switch (value)
        {
        case KEY_UP:
            lv_btn_set_state(pass_btn,LV_BTN_STATE_PR);
            btn_event_handler(pass_btn, LV_EVENT_CLICKED);
            break;

        case KEY_DOWN:
            lv_btn_set_state(fail_btn,LV_BTN_STATE_PR);
            btn_event_handler(fail_btn, LV_EVENT_CLICKED);
            break;

        default:
            break;
        }
    }
}

void rtc_page_refresh(size_t type, size_t value)
{
    switch (type)
    {
    case TYPE_REFRESH:
        if (value & REFRESH_DATE)
        {
            draw_rtc_info();
        }
        break;

    default:
        break;
    }
}
