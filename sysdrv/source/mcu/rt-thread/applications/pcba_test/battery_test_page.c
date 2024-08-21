/*
 * Copyright: Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * License: SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-20 10:30:36
 * @Company: Rockchip
 * @LastEditTime: 2020-06-29 11:41:57
 * @LastEditors: Do not edit
 * @Description:
 */

#include "main_page.h"
#include "battery_test_page.h"

#define DBG_SECTION_NAME    "BATTERY_PAGE"
#define DBG_LEVEL           DBG_LOG
#include "rtdbg.h"

static lv_obj_t *batt_page = NULL;
static lv_obj_t *divide_line = NULL;
static lv_obj_t *company_label = NULL;
static lv_obj_t *item_label = NULL;
static lv_obj_t *pass_btn = NULL;
static lv_obj_t *fail_btn = NULL;

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

static lv_obj_t *batt_label, *batt_val_label;
static lv_obj_t *batt_capacity_label, *batt_capacity_val_label;
static lv_obj_t *current_label, *current_val_label;
static lv_obj_t *voltage_label, *voltage_val_label;

static int draw_batt_info(void)
{
    char str[32];
    g_pcba.fuel_gauge = rt_device_find(FUEL_GAUGE_DEVICE_NAME);
    if (!g_pcba.fuel_gauge)
    {
        LOG_W("No fuel gauge\n");
        g_pcba.battery = 0;
    }
    else
    {
        rt_device_control(g_pcba.fuel_gauge, RT_DEVICE_CTRL_FG_GET, &g_pcba.fg_status);
        g_pcba.battery = g_pcba.fg_status.soc;
    }

    rt_memset((void *)str, 0x0, sizeof(str));
    rt_snprintf(str, sizeof(str), "%d%%",g_pcba.battery);
    lv_label_set_text(batt_val_label, str);

    rt_memset((void *)str, 0x0, sizeof(str));
    rt_snprintf(str, sizeof(str), "%ld", g_pcba.fg_status.fcc);
    lv_label_set_text(batt_capacity_val_label, str);

    rt_memset((void *)str, 0x0, sizeof(str));
    rt_snprintf(str, sizeof(str), "%ld", g_pcba.fg_status.current);
    lv_label_set_text(current_val_label, str);

    rt_memset((void *)str, 0x0, sizeof(str));
    rt_snprintf(str, sizeof(str), "%ld", g_pcba.fg_status.voltage);
    lv_label_set_text(voltage_val_label, str);

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
            memcpy (g_result_info[ITEM_BATTERY], result, strlen(result));
        }
        else
        {
            LOG_D("Clicked Fail button");
            result = "NG";
            memcpy (g_result_info[ITEM_BATTERY], result, strlen(result));
        }

        batt_page_deinit();

        display_pcba_page(MENU_MAIN_PAGE);
    }
}

static int draw_result_btn(void)
{
    lv_obj_t * label;

    /* Create PASS button */
    pass_btn = lv_btn_create(batt_page, NULL);
    lv_obj_set_event_cb(pass_btn, btn_event_handler);
    lv_obj_set_size(pass_btn, BTN_W, BTN_H);
    lv_obj_align(pass_btn, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);

    lv_btn_set_style(pass_btn, LV_BTN_STYLE_PR,  &style_tv_btn_pr);
    lv_btn_set_style(pass_btn, LV_BTN_STYLE_REL,  &style_tv_btn_rel);

    /* Create FAIL button */
    fail_btn = lv_btn_create(batt_page, NULL);
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

/**
 * @description: pcba phone & speaker test page init
 * @param {int page}
 * @return: none
 */
void batt_page_init(int p_page)
{
    short int y = 0;
    char str_test_item[64];

    if (!batt_page)
    {
        batt_page = lv_obj_create(lv_scr_act(), NULL);

        lv_obj_set_size(batt_page, SCREEN_W, SCREEN_H);
        lv_obj_set_style(batt_page, &label_style_font16);
        lv_obj_invalidate(batt_page);

        divide_line = lv_line_create(batt_page, NULL);

        company_label = lv_label_create(batt_page, NULL);
        lv_label_set_long_mode(company_label, LV_LABEL_LONG_SROLL_CIRC);
        lv_label_set_align(company_label, LV_LABEL_ALIGN_CENTER);
        lv_obj_set_size(company_label, LABEL_W, LABEL_H);
        lv_obj_set_style(company_label, &label_style_font16);
        lv_obj_align(company_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);
        lv_label_set_text(company_label, "Rockchip Co.,Ltd.");

        item_label = lv_label_create(batt_page, NULL);
        y = LABEL_H;
        lv_label_set_long_mode(item_label, LV_LABEL_LONG_SROLL_CIRC);
        lv_label_set_align(item_label, LV_LABEL_ALIGN_CENTER);
        lv_label_set_recolor(item_label, true);
        lv_obj_set_size(item_label, LABEL_W, LABEL_H);
        lv_obj_set_style(item_label, &label_style_font16);
        lv_obj_align(item_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);
        rt_snprintf(str_test_item, sizeof(str_test_item), "#00ff00 %s#", pcba_test_item_str[ITEM_BATTERY]);
        lv_label_set_text(item_label, (const char *)str_test_item);

        /*battery lable */
        batt_label = lv_label_create(batt_page, NULL);
        lv_label_set_recolor(batt_label, true);
        lv_label_set_long_mode(batt_label, LV_LABEL_LONG_EXPAND);
        lv_obj_align(batt_label, NULL, LV_ALIGN_IN_LEFT_MID, 0, -40);
        lv_label_set_text(batt_label, "battery: ");
        lv_obj_set_style(batt_label, &lv_style_pretty);

        /*battery value lable*/
        batt_val_label = lv_label_create(batt_page, NULL);
        lv_label_set_recolor(batt_val_label, true);
        lv_label_set_long_mode(batt_val_label, LV_LABEL_LONG_EXPAND);
        lv_obj_align(batt_val_label, NULL, LV_ALIGN_IN_LEFT_MID, 80, -40);
        lv_obj_set_style(batt_val_label, &lv_style_pretty);

        /*battery capacity lable*/
        batt_capacity_label = lv_label_create(batt_page, NULL);
        lv_label_set_recolor(batt_capacity_label, true);
        lv_label_set_long_mode(batt_capacity_label, LV_LABEL_LONG_EXPAND);
        lv_obj_align(batt_capacity_label, NULL, LV_ALIGN_IN_LEFT_MID, 0, -20);
        lv_label_set_text(batt_capacity_label, "capacity: ");
        lv_obj_set_style(batt_capacity_label, &lv_style_pretty);

        /*battery capacity value lable*/
        batt_capacity_val_label = lv_label_create(batt_page, NULL);
        lv_label_set_recolor(batt_capacity_val_label, true);
        lv_label_set_long_mode(batt_capacity_val_label, LV_LABEL_LONG_EXPAND);
        lv_obj_align(batt_capacity_val_label, NULL, LV_ALIGN_IN_LEFT_MID, 90, -20);
        lv_obj_set_style(batt_capacity_val_label, &lv_style_pretty);

        /*battery current lable*/
        current_label = lv_label_create(batt_page, NULL);
        lv_label_set_recolor(current_label, true);
        lv_label_set_long_mode(current_label, LV_LABEL_LONG_EXPAND);
        lv_obj_align(current_label, NULL, LV_ALIGN_IN_LEFT_MID, 0, 0);
        lv_label_set_text(current_label, "current: ");
        lv_obj_set_style(current_label, &lv_style_pretty);

        /*battery current value lable*/
        current_val_label = lv_label_create(batt_page, NULL);
        lv_label_set_recolor(current_val_label, true);
        lv_label_set_long_mode(current_val_label, LV_LABEL_LONG_EXPAND);
        lv_obj_align(current_val_label, NULL, LV_ALIGN_IN_LEFT_MID, 80, 0);
        lv_obj_set_style(current_val_label, &lv_style_pretty);

        /*battery voltage lable*/
        voltage_label = lv_label_create(batt_page, NULL);
        lv_label_set_recolor(voltage_label, true);
        lv_label_set_long_mode(voltage_label, LV_LABEL_LONG_EXPAND);
        lv_obj_align(voltage_label, NULL, LV_ALIGN_IN_LEFT_MID, 0, 20);
        lv_label_set_text(voltage_label, "voltage: ");
        lv_obj_set_style(voltage_label, &lv_style_pretty);

        /*battery voltage value lable*/
        voltage_val_label = lv_label_create(batt_page, NULL);
        lv_label_set_recolor(voltage_val_label, true);
        lv_label_set_long_mode(voltage_val_label, LV_LABEL_LONG_EXPAND);
        lv_obj_align(voltage_val_label, NULL, LV_ALIGN_IN_LEFT_MID, 80, 20);
        lv_obj_set_style(voltage_val_label, &lv_style_pretty);

    }

    draw_divide_line();

    /*draw battery value info*/
    draw_batt_info();

    /*draw buttons of result*/
    draw_result_btn();
}

/**
 * @description:
 * @param {type}
 * @return:
 */
void batt_page_deinit(void)
{
    if (batt_page)
    {
        /* this function will delete children too */
        lv_obj_del(batt_page);
        batt_page = NULL;
    }
}

void batt_page_key(size_t type, size_t value)
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

void batt_page_refresh(size_t type, size_t value)
{
    switch (type)
    {
    case TYPE_REFRESH:
        if (value & REFRESH_BAT)
        {
            draw_batt_info();
        }
        break;

    default:
        break;
    }
}

