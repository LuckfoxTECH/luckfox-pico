/*
 * Copyright: Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * License: SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-20 11:49:40
 * @Company: Rockchip
 * @LastEditTime: 2020-06-29 11:49:32
 * @LastEditors: Do not edit
 * @Description:
 */

#include "main_page.h"
#include "result_page.h"

#define DBG_SECTION_NAME    "PCBA_RESULT_PAGE"
#define DBG_LEVEL           DBG_LOG
#include "rtdbg.h"

static lv_obj_t *result_page = NULL;
static lv_obj_t *divide_line = NULL;

//static lv_obj_t *company_label = NULL;
static lv_obj_t *platform_label = NULL;

static const char test_item_str[ITEM_MAX][32] =
{
    "screen test :",
    "key test :",
    "record :",
    "pho_spk test :",
    "emmc test :",
    "battery test :",
    "temp test :",
    "rtc test :",
    "wifi test :",
    "bt test :"
};

static int draw_divide_line(void)
{
    static lv_point_t line_points[] = {{0, STATE_BAR_H / 2 - 1}, {STATE_BAR_W, STATE_BAR_H / 2 - 1}};
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

    return RT_EOK;
}

#define TITLE_STR_Y         20
#define ITEM_LABEL_H        18
static int draw_result_info(void)
{
    int idx = 0;
    int label_w = SCREEN_W / 3;
    lv_obj_t *item_label = NULL;
    lv_obj_t *item_rst   = NULL;

    for (idx = 0; idx < ITEM_RESULT; idx++ )
    {
        /* item string label */
        item_label = lv_label_create(result_page, NULL);
        lv_label_set_long_mode(item_label, LV_LABEL_LONG_BREAK);
        lv_label_set_align(item_label, LV_LABEL_ALIGN_LEFT);
        lv_obj_set_size(item_label, (LABEL_W * 2) / 3, ITEM_LABEL_H);

        lv_obj_align(item_label, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
        lv_obj_set_pos(item_label, 5, TITLE_STR_Y + idx * ITEM_LABEL_H );
        lv_label_set_text(item_label, test_item_str[idx]);
        lv_obj_set_style(item_label, &label_style_font16);

        /* result lable */
        item_rst = lv_label_create(result_page, NULL);
        lv_label_set_long_mode(item_rst, LV_LABEL_LONG_BREAK);
        lv_label_set_align(item_rst, LV_LABEL_ALIGN_RIGHT);
        lv_obj_set_size(item_rst, LABEL_W / 3, ITEM_LABEL_H);

        lv_obj_align(item_rst, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
        lv_obj_set_pos(item_rst, label_w * 2, TITLE_STR_Y + idx * ITEM_LABEL_H);
        lv_label_set_text(item_rst, g_result_info[idx]);
    }

    return 0;
}

static lv_obj_t *back_btn;
static void back_btn_event_cb (lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED)
    {
        LOG_D("Clicked btn Back\n");
        display_pcba_page(MENU_MAIN_PAGE);
    }
}

static int draw_back_btn()
{
    int btn_w = SCREEN_W / 3;

    /* back to index page button */
    back_btn = lv_btn_create(result_page, NULL);

    lv_obj_set_event_cb(back_btn, back_btn_event_cb);
    lv_obj_set_size(back_btn, SCREEN_W - (SCREEN_W * 2) / 3, BTN_H);
    lv_obj_set_pos(back_btn, btn_w * 2, SCREEN_H - BTN_H);
    lv_obj_align(back_btn, NULL,LV_ALIGN_IN_BOTTOM_RIGHT,0,0);

    lv_btn_set_style(back_btn, LV_BTN_STYLE_PR,  &style_tv_btn_pr);
    lv_btn_set_style(back_btn, LV_BTN_STYLE_REL,  &style_tv_btn_rel);

    lv_obj_t * label;
    label = lv_label_create(back_btn, NULL);
    lv_label_set_recolor(label, true);
    lv_label_set_text(label, "#ff0000 Back#");
    lv_obj_set_style(label, &lv_style_pretty);

    lv_btn_set_state(back_btn, LV_BTN_STATE_PR);
    return 0;
}

/**
 * @description: pcba result page init
 * @param {int page}
 * @return: none
 */
void result_page_init(int p_page)
{
    short int y = 0;
    char str_test_item[64];
    if (!result_page)
    {
        result_page = lv_obj_create(lv_scr_act(), NULL);

        lv_obj_set_size(result_page, SCREEN_W, SCREEN_H);
        lv_obj_set_style(result_page, &label_style_font16);
        lv_obj_align(result_page, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
        lv_obj_invalidate(result_page);

        divide_line = lv_line_create(result_page, NULL);

        platform_label = lv_label_create(result_page, NULL);
        y = 0;
        lv_label_set_long_mode(platform_label, LV_LABEL_LONG_SROLL_CIRC);
        lv_label_set_align(platform_label, LV_LABEL_ALIGN_CENTER);
        lv_label_set_recolor(platform_label, true);
        lv_obj_set_size(platform_label, LABEL_W, LABEL_H);
        lv_obj_align(platform_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);

        rt_snprintf(str_test_item, sizeof(str_test_item), "#00ff00 %s#", pcba_test_item_str[ITEM_RESULT]);
        lv_label_set_text(platform_label, str_test_item);
    }

    draw_divide_line();

    /* draw all pcba test item result info */
    draw_result_info();

    /* draw back button to index page*/
    draw_back_btn();
}

/**
 * @description:
 * @param {type}
 * @return:
 */
void result_page_deinit(void)
{
    LOG_D("Enter result_page_deinit");
    if (result_page)
    {
        /* this function will delete children too */
        lv_obj_del(result_page);
        result_page = NULL;
    }
}

void result_page_key(size_t type, size_t value)
{
    if (type == TYPE_KEY_PRESS)
    {
        switch (value)
        {
        case KEY_SELECT:
            lv_btn_set_state(back_btn, LV_BTN_STATE_PR);
            back_btn_event_cb(back_btn, LV_EVENT_CLICKED);
            break;

        default:
            break;
        }
    }
}

void result_page_refresh(size_t type, size_t value)
{
}
