/*
 * Copyright: Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * License: SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-20 10:10:00
 * @Company: Rockchip
 * @LastEditTime: 2020-06-29 11:43:45
 * @LastEditors: Do not edit
 * @Description:
 */

#include "main_page.h"
#include "key_test_page.h"

#define DBG_SECTION_NAME    "KEY_PAGE"
#define DBG_LEVEL           DBG_LOG
#include "rtdbg.h"

static lv_obj_t *key_page = NULL;
static lv_obj_t *divide_line = NULL;
static lv_obj_t *company_label = NULL;
static lv_obj_t *item_label = NULL;
static lv_obj_t *pass_btn = NULL;
static lv_obj_t *fail_btn = NULL;

static int g_key_pass;
static int g_key_fail;

#define KEY_NUM_MAX         6
#define KEY_NAME_LEN        12
static char keyInfo[KEY_NUM_MAX][KEY_NAME_LEN] =
{
    "RECORD",
    "UP",
    "DOWN",
    "SETTING",
    "MENU",
    "CANCLE",
};

static lv_obj_t* keyBtn[KEY_NUM_MAX];

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

static void key_btn_event_cb(lv_obj_t * obj, lv_event_t event)
{
    static lv_style_t *style_key_btn;

    if(event == LV_EVENT_CLICKED)
    {
        printf("Clicked\n");

        style_key_btn = (lv_style_t *)lv_obj_get_style(obj);
        style_key_btn->body.main_color = LV_COLOR_GREEN;
        lv_obj_set_style(obj, style_key_btn);

    }
    else if(event == LV_EVENT_VALUE_CHANGED)
    {
        printf("Toggled\n");
    }
}

static int draw_key_btn(void)
{
    int i = 0;
    static lv_style_t style_tv_btn_rel;
    lv_style_copy(&style_tv_btn_rel, &lv_style_btn_rel);

    style_tv_btn_rel.body.opa = LV_OPA_70;
    style_tv_btn_rel.body.border.width = 0;
    style_tv_btn_rel.body.radius = 0;

    static lv_style_t style_tv_btn_pr;
    lv_style_copy(&style_tv_btn_pr, &lv_style_btn_pr);
    style_tv_btn_pr.body.radius = 0;
    style_tv_btn_pr.body.opa = LV_OPA_50;
    style_tv_btn_pr.body.main_color = LV_COLOR_WHITE;
    style_tv_btn_pr.body.grad_color = LV_COLOR_WHITE;
    style_tv_btn_pr.body.border.width = 2;
    style_tv_btn_pr.text.color = LV_COLOR_GRAY;


    for (i = 0; i < KEY_NUM_MAX; i++)
    {
        keyBtn[i] = lv_btn_create(key_page, NULL);
        lv_obj_set_event_cb(keyBtn[i], key_btn_event_cb);

        lv_obj_set_size(keyBtn[i], BTN_W, BTN_H);
        lv_btn_set_style(keyBtn[i], LV_BTN_STYLE_PR,  &style_tv_btn_pr);
        lv_btn_set_style(keyBtn[i], LV_BTN_STYLE_REL,  &style_tv_btn_rel);

        if (i % 2 == 0)
            lv_obj_set_pos(keyBtn[i], 2, TITLE_STR_H + (BTN_H + BTN_Y_GAP) * (i / 2));
        else
            lv_obj_set_pos(keyBtn[i], BTN_W + BTN_X_GAP + 5, TITLE_STR_H + (BTN_H + BTN_Y_GAP) * (i / 2));

        lv_obj_t * label;
        label = lv_label_create(keyBtn[i], NULL);
        lv_label_set_text(label, keyInfo[i]);
        lv_obj_set_style(label, &lv_style_pretty);
    }

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
            memcpy (g_result_info[ITEM_KEY], result, strlen(result));
        }
        else
        {
            LOG_D("Clicked Fail button");
            result = "NG";
            memcpy (g_result_info[ITEM_KEY], result, strlen(result));
        }

        display_pcba_page(MENU_MAIN_PAGE);
    }
}

static int draw_result_btn(void)
{
    lv_obj_t * label;

    /* Create PASS button */
    pass_btn = lv_btn_create(key_page, NULL);
    lv_obj_set_event_cb(pass_btn, btn_event_handler);
    lv_obj_set_size(pass_btn, BTN_W, BTN_H);
    lv_obj_align(pass_btn, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);

    lv_btn_set_style(pass_btn, LV_BTN_STYLE_PR,  &style_tv_btn_pr);
    lv_btn_set_style(pass_btn, LV_BTN_STYLE_REL,  &style_tv_btn_rel);

    /* Create FAIL button */
    fail_btn = lv_btn_create(key_page, NULL);
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
void key_page_init(int p_page)
{
    short int y = 0;
    char str_test_item[64];

    g_key_pass = 0;
    g_key_fail = 0;

    if (!key_page)
    {
        key_page = lv_obj_create(lv_scr_act(), NULL);
        lv_obj_set_size(key_page, SCREEN_W, SCREEN_H);
        lv_obj_set_style(key_page, &label_style_font16);
        lv_obj_align(key_page, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
        lv_obj_invalidate(key_page);

        divide_line = lv_line_create(key_page, NULL);

        company_label = lv_label_create(key_page, NULL);
        lv_label_set_long_mode(company_label, LV_LABEL_LONG_SROLL_CIRC);
        lv_label_set_align(company_label, LV_LABEL_ALIGN_CENTER);
        lv_obj_set_size(company_label, LABEL_W, LABEL_H);
        lv_obj_align(company_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);
        lv_label_set_text(company_label, "Rockchip Co.,Ltd.");
        lv_obj_set_style(company_label, &label_style_font16);

        item_label = lv_label_create(key_page, NULL);
        y = LABEL_H;
        lv_label_set_long_mode(item_label, LV_LABEL_LONG_SROLL_CIRC);
        lv_label_set_align(item_label, LV_LABEL_ALIGN_CENTER);
        lv_label_set_recolor(item_label, true);
        lv_obj_set_size(item_label, LABEL_W, LABEL_H);
        lv_obj_align(item_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);
        lv_obj_set_style(item_label, &label_style_font16);
        rt_snprintf(str_test_item, sizeof(str_test_item), "#00ff00 %s#", pcba_test_item_str[ITEM_KEY]);
        lv_label_set_text(item_label, str_test_item);
    }

    draw_divide_line();

    draw_key_btn();

    draw_result_btn();
}

/**
 * @description:
 * @param {type}
 * @return:
 */
void key_page_deinit(void)
{
    if (key_page)
    {
        /* this function will delete children too */
        lv_obj_del(key_page);
        key_page = NULL;
    }
}

void key_page_key(size_t type, size_t value)
{
    if (type == TYPE_KEY_PRESS)
    {
        switch (value)
        {
        case KEY_RECORD:
            key_btn_event_cb(keyBtn[0], LV_EVENT_CLICKED);
            break;

        case KEY_UP:
            if (g_key_pass == 0)
            {
                key_btn_event_cb(keyBtn[1], LV_EVENT_CLICKED);
                g_key_pass = 1;
            }
            else
            {
                g_key_pass = 0;
                lv_btn_set_state(pass_btn,LV_BTN_STATE_PR);
                btn_event_handler(pass_btn, LV_EVENT_CLICKED);
            }

            break;

        case KEY_DOWN:
            if (g_key_fail == 0)
            {
                key_btn_event_cb(keyBtn[2], LV_EVENT_CLICKED);
                g_key_fail = 1;
            }
            else
            {
                g_key_fail = 0;
                lv_btn_set_state(fail_btn,LV_BTN_STATE_PR);
                btn_event_handler(fail_btn, LV_EVENT_CLICKED);
            }

            break;

        case KEY_MENU:
            key_btn_event_cb(keyBtn[4], LV_EVENT_CLICKED);
        default:
            //TODO:
            break;
        }
    }
}

void key_page_refresh(size_t type, size_t value)
{
}

