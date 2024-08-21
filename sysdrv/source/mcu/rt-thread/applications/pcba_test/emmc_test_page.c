/*
 * Copyright: Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * License: SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-20 10:30:03
 * @Company: Rockchip
 * @LastEditTime: 2020-06-29 11:43:27
 * @LastEditors: Do not edit
 * @Description:
 */

#include "main_page.h"
#include "emmc_test_page.h"

#define DBG_SECTION_NAME    "EMMC_PAGE"
#define DBG_LEVEL           DBG_LOG
#include "rtdbg.h"

static lv_obj_t *emmc_page = NULL;
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

static int disk_capacity(const char *path, char *capacity)
{
    int result;
    int minor = 0;
    long long cap;
    struct statfs buffer;

    int unit_index = 0;
    char *unit_str[] = {"KB", "MB", "GB"};

    result = dfs_statfs(path ? path : NULL, &buffer);
    if (result != 0)
    {
        rt_kprintf("dfs_statfs failed.\n");
        return -1;
    }

    cap = ((long long)buffer.f_bsize) * ((long long)buffer.f_blocks) / 1024LL;
    for (unit_index = 0; unit_index < 2; unit_index ++)
    {
        if (cap < 1024) break;

        minor = (cap % 1024) * 10 / 1024; /* only one decimal point */
        cap = cap / 1024;
    }

    rt_snprintf(capacity, sizeof(capacity), "%ld.%d %s", (unsigned long)cap, minor, unit_str[unit_index]);
    LOG_D("disk total: %ld.%d %s [ %d block, %d bytes per block ]\n",
        (unsigned long)cap, minor, unit_str[unit_index], buffer.f_bfree, buffer.f_bsize);
    return 0;
}

static lv_obj_t *capacity_label;
static lv_obj_t *capacity_value;
static int draw_emmc_info(void)
{
    capacity_label = lv_label_create(emmc_page, NULL);
    lv_label_set_recolor(capacity_label, true);
    lv_label_set_long_mode(capacity_label, LV_LABEL_LONG_EXPAND);
    lv_obj_align(capacity_label, NULL, LV_ALIGN_IN_LEFT_MID, 0, -20);
    lv_label_set_text(capacity_label, "eMMC capacity:");
    lv_obj_set_style(capacity_label, &lv_style_pretty);

    static lv_style_t style1;
    lv_style_copy(&style1, &lv_style_pretty);
    style1.text.color = LV_COLOR_GREEN;

    char emmc_capa[64] =  {0};
    disk_capacity("/sdcard", emmc_capa);

    capacity_value = lv_label_create(emmc_page, NULL);
    lv_label_set_recolor(capacity_value, true);
    lv_label_set_long_mode(capacity_value, LV_LABEL_LONG_EXPAND);
    lv_obj_align(capacity_value, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(capacity_value, emmc_capa);
    lv_obj_set_style(capacity_value, &style1);

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
            memcpy (g_result_info[ITEM_EMMC], result, strlen(result));
        }
        else
        {
            LOG_D("Clicked Fail button");
            result = "NG";
            memcpy (g_result_info[ITEM_EMMC], result, strlen(result));
        }

        emmc_page_deinit();

        display_pcba_page(MENU_MAIN_PAGE);
    }
}

static int draw_result_btn(void)
{
    lv_obj_t * label;

    /* Create PASS button */
    pass_btn = lv_btn_create(emmc_page, NULL);
    lv_obj_set_event_cb(pass_btn, btn_event_handler);
    lv_obj_set_size(pass_btn, BTN_W, BTN_H);
    lv_obj_align(pass_btn, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
    lv_btn_set_style(pass_btn, LV_BTN_STYLE_PR,  &style_tv_btn_pr);
    lv_btn_set_style(pass_btn, LV_BTN_STYLE_REL,  &style_tv_btn_rel);

    /* Create FAIL button */
    fail_btn = lv_btn_create(emmc_page, NULL);
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
 * @description: pcba key test page init
 * @param {int page}
 * @return: none
 */
void emmc_page_init(int p_page)
{
    short int y = 0;
    char str_test_item[64];

    if (!emmc_page)
    {
        emmc_page = lv_obj_create(lv_scr_act(), NULL);

        lv_obj_set_size(emmc_page, SCREEN_W, SCREEN_H);
        lv_obj_set_style(emmc_page, &label_style_font16);
        lv_obj_invalidate(emmc_page);

        divide_line = lv_line_create(emmc_page, NULL);

        company_label = lv_label_create(emmc_page, NULL);
        lv_label_set_long_mode(company_label, LV_LABEL_LONG_SROLL_CIRC);
        lv_label_set_align(company_label, LV_LABEL_ALIGN_CENTER);
        lv_obj_set_style(company_label, &label_style_font16);
        lv_obj_set_size(company_label, LABEL_W, LABEL_H);
        lv_obj_align(company_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);
        lv_label_set_text(company_label, "Rockchip Co.,Ltd.");

        item_label = lv_label_create(emmc_page, NULL);
        y = LABEL_H;
        lv_label_set_long_mode(item_label, LV_LABEL_LONG_SROLL_CIRC);
        lv_label_set_align(item_label, LV_LABEL_ALIGN_CENTER);
        lv_label_set_recolor(item_label, true);
        lv_obj_set_style(item_label, &label_style_font16);
        lv_obj_set_size(item_label, LABEL_W, LABEL_H);
        lv_obj_align(item_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);
        rt_snprintf(str_test_item, sizeof(str_test_item), "#00ff00 %s#", pcba_test_item_str[ITEM_EMMC]);
        lv_label_set_text(item_label, str_test_item);
    }

    draw_divide_line();

    draw_emmc_info();

    draw_result_btn();
}

/**
 * @description:
 * @param {type}
 * @return:
 */
void emmc_page_deinit(void)
{
    if (emmc_page)
    {
        /* this function will delete children too */
        lv_obj_del(emmc_page);
        emmc_page = NULL;
    }
}

void emmc_page_key(size_t type, size_t value)
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

void emmc_page_refresh(size_t type, size_t value)
{
}
