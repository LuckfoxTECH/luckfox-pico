/*
 * Copyright: Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * License: SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-15 15:59:11
 * @Company: Rockchip
 * @LastEditTime: 2020-06-16 15:21:24
 * @LastEditors: Do not edit
 * @Description:
 */

#include "pcba_app.h"
#include "pcba_public.h"
#include "main_page.h"
#include "screen_test_page.h"
#include "key_test_page.h"
#include "record_test_page.h"
#include "phospk_test_page.h"
#include "emmc_test_page.h"
#include "rtc_test_page.h"
#include "temperate_test_page.h"
#include "battery_test_page.h"
#include "wifi_test_page.h"
#include "bt_test_page.h"

#include "result_page.h"
#define DBG_SECTION_NAME    "PCBA_MAINPAGE"
#define DBG_LEVEL           DBG_LOG
#include "rtdbg.h"

char g_result_info[ITEM_MAX][8];
/* release btn style */
lv_style_t style_tv_btn_rel;
/* press btn style */
lv_style_t style_tv_btn_pr;

static lv_obj_t *index_page = NULL;
static lv_obj_t *divide_line = NULL;

static lv_obj_t *company_label = NULL;
static lv_obj_t *platform_label = NULL;
static lv_obj_t * arc_point = NULL;

struct pcba_test_item_st
{
    int item;
    lv_obj_t  *item_btn;
    const char* btn_label;
    lv_event_cb_t btn_cb;
};

static void screen_test_cb(lv_obj_t * obj, lv_event_t event);
static void key_test_cb(lv_obj_t * obj, lv_event_t event);
static void record_test_cb(lv_obj_t * obj, lv_event_t event);
static void phospk_test_cb(lv_obj_t * obj, lv_event_t event);
static void emmc_test_cb(lv_obj_t * obj, lv_event_t event);
static void batt_test_cb(lv_obj_t * obj, lv_event_t event);
static void temp_test_cb(lv_obj_t * obj, lv_event_t event);
static void rtc_test_cb(lv_obj_t * obj, lv_event_t event);
static void wifi_test_cb(lv_obj_t * obj, lv_event_t event);
static void bt_test_cb(lv_obj_t * obj, lv_event_t event);
static void result_btn_cb(lv_obj_t * obj, lv_event_t event);

struct pcba_test_item_st g_pcba_items[ITEM_MAX] =
{
    {ITEM_SCREEN,   NULL,  "screen test", screen_test_cb},
    {ITEM_KEY,      NULL,     "key test", key_test_cb   },
    {ITEM_RECORD,   NULL,  "record test", record_test_cb},
    {ITEM_PHO_SPK,  NULL,  "phospk test", phospk_test_cb},
    {ITEM_EMMC,     NULL,    "emmc test", emmc_test_cb  },
    {ITEM_BATTERY,  NULL,    "batt test", batt_test_cb  },
    {ITEM_TEMP,     NULL,    "temp test", temp_test_cb  },
    {ITEM_RTC,      NULL,     "rtc test", rtc_test_cb   },
    {ITEM_WIFI,     NULL,    "wifi test", wifi_test_cb  },
    {ITEM_BT,       NULL,      "bt test", bt_test_cb    },
    {ITEM_RESULT,   NULL,       "result", result_btn_cb },
};

const char *pcba_test_item_str[ITEM_MAX] =
{
    "Screen Test",
    "Key Test",
    "Record Test",
    "Pho_Spk Test",
    "eMMC Test",
    "Battery Test",
    "Temperate Test",
    "Rtc Test",
    "Wifi Test",
    "BT Test",
    "PCBA Statistics",
};

static void set_buttn_globe_sylte(void)
{
    /* release btn style */
    lv_style_copy(&style_tv_btn_rel, &lv_style_btn_rel);
    style_tv_btn_rel.body.opa = LV_OPA_70;
    style_tv_btn_rel.body.border.width = 0;
    style_tv_btn_rel.body.radius = 0;

    /* press btn style */
    lv_style_copy(&style_tv_btn_pr, &lv_style_btn_pr);
    style_tv_btn_pr.body.radius = 0;
    style_tv_btn_pr.body.opa = LV_OPA_50;
    style_tv_btn_pr.body.main_color = LV_COLOR_WHITE;
    style_tv_btn_pr.body.grad_color = LV_COLOR_WHITE;
    style_tv_btn_pr.body.border.width = 0;
    style_tv_btn_pr.text.color = LV_COLOR_GRAY;
}

static rt_err_t draw_divide_line(void)
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

    return RT_EOK;
}

static void screen_test_cb(lv_obj_t * obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        LOG_D("Clicked SCREEN TEST");
        LOG_D(">>>>> SCREEN TEST PAGE Enter..");

        index_page_deinit();
        screen_page_init(0);
    }
}

static void key_test_cb(lv_obj_t * obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        LOG_D("Clicked KEY TEST");
        LOG_D(">>>>> KEY TEST PAGE Enter..");

        index_page_deinit();
        key_page_init(0);
    }
}

static void record_test_cb(lv_obj_t * obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        LOG_D("Clicked RECORD TEST");
        LOG_D(">>>>> RECORD TEST PAGE Enter..");

        index_page_deinit();
        record_page_init(0);
    }
}

static void phospk_test_cb(lv_obj_t * obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        LOG_D("Clicked PHONE_SPEAK TEST");
        LOG_D(">>>>> PHONE_SPEAK TEST PAGE Enter..");

        index_page_deinit();
        phospk_page_init(0);
    }
}

static void emmc_test_cb(lv_obj_t * obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        LOG_D("Clicked EMMC TEST");
        LOG_D(">>>>> EMMC TEST PAGE Enter..");

        index_page_deinit();
        emmc_page_init(0);
    }
}

static void batt_test_cb(lv_obj_t * obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        LOG_D("Clicked BATTERY TEST");
        LOG_D(">>>>> BATTERY TEST PAGE Enter..");
        index_page_deinit();
        batt_page_init(0);
    }
}

static void rtc_test_cb(lv_obj_t * obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        LOG_D("Clicked RTC TEST");
        LOG_D(">>>>> RTC TEST PAGE Enter..");
        index_page_deinit();
        rtc_page_init(0);
    }
}

static void temp_test_cb(lv_obj_t * obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        LOG_D("Clicked TEMPERATE TEST");
        LOG_D(">>>>> TEMPERATE TEST PAGE Enter..");

        index_page_deinit();
        temp_page_init(0);
    }
}

static void wifi_test_cb(lv_obj_t * obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        LOG_D("Clicked WIFI TEST");
        LOG_D(">>>>> WIFI TEST PAGE Enter..");

        index_page_deinit();
        wifi_page_init(0);
    }
}

static void bt_test_cb(lv_obj_t * obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        LOG_D("Clicked BT TEST");
        LOG_D(">>>>> BT TEST PAGE Enter..");

        index_page_deinit();
        bt_page_init(0);
    }
}

void draw_test_item_buttons()
{
    int i = -1;
    for (i = ITEM_SCREEN; i < ITEM_RESULT; i++)
    {
        g_pcba_items[i].item_btn = lv_btn_create(index_page, NULL);
        lv_obj_set_event_cb(g_pcba_items[i].item_btn, g_pcba_items[i].btn_cb);
        lv_obj_set_size(g_pcba_items[i].item_btn, BTN_W, BTN_H);
        lv_btn_set_style(g_pcba_items[i].item_btn, LV_BTN_STYLE_PR,  &style_tv_btn_pr);
        lv_btn_set_style(g_pcba_items[i].item_btn, LV_BTN_STYLE_REL,  &style_tv_btn_rel);

        if (i % 2 == 0)
            lv_obj_set_pos(g_pcba_items[i].item_btn, 2, TITLE_STR_H + (BTN_H + BTN_Y_GAP) * (i / 2));
            //lv_obj_align(g_pcba_items[i].item_btn, NULL, LV_ALIGN_IN_TOP_LEFT, BTN_X_GAP, TITLE_STR_H + (BTN_H* i + BTN_Y_GAP));
        else
            //lv_obj_align(btn, NULL, LV_ALIGN_CENTER, 0, -40);
            lv_obj_set_pos(g_pcba_items[i].item_btn, BTN_W + BTN_X_GAP + 5, TITLE_STR_H + (BTN_H + BTN_Y_GAP) * (i / 2));

        lv_obj_t * label;
        label = lv_label_create(g_pcba_items[i].item_btn, NULL);
        lv_label_set_text(label, g_pcba_items[i].btn_label);
        lv_obj_set_style(label, &label_style_font16);
    }
}

static lv_obj_t* rst_btn;
static int g_item_index = 0;
/*
void draw_arc_position(int position)
{
	int arc_x = 0;
	int arc_y = 0;

    LOG_D("%s Enter", __func__);
    static lv_style_t style;

    if (arc_point == NULL)
    {
        LOG_D("%s Create new arc point", __func__);
        lv_style_copy(&style, &lv_style_plain);
        style.line.color = LV_COLOR_RED;
        style.line.width = 5;

        arc_point = lv_arc_create(index_page, NULL);
        lv_obj_set_size(arc_point, 10, 10);
        lv_arc_set_angles(arc_point, 0, 360);
        lv_arc_set_style(arc_point, LV_ARC_STYLE_MAIN, &style);
        lv_obj_invalidate(index_page);
    }

    if (position != ITEM_RESULT)
    {
        if (position % 2 == 0)
        {
            arc_x = BTN_W - 2;
            arc_y = TITLE_STR_H + BTN_H + (BTN_H + BTN_Y_GAP) * (position / 2) - 5;
        }
        else
        {
            arc_x = SCREEN_W - 10;
            arc_y = TITLE_STR_H + BTN_H + (BTN_H + BTN_Y_GAP) * (position / 2) - 5;
        }
    }
    else if (position == ITEM_RESULT)
    {
        arc_x = (SCREEN_W / 3) * 2;
        arc_y = SCREEN_H - 10;
    }

    LOG_D("arc point x : %d, y: %d", arc_x, arc_y);
    lv_obj_set_pos(arc_point, arc_x, arc_y);
}
*/
static void result_btn_cb(lv_obj_t * obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        LOG_D("Clicked PCBA TEST RESULT");
        LOG_D(">>>>> PCBA TEST result page Enter..");

        index_page_deinit();
        result_page_init(0);
    }
}

static lv_obj_t* rst_btn;
static void draw_result_button(void)
{
    int btn_w = SCREEN_W / 3;

    lv_obj_t * label;
    /* RESULT button */
    rst_btn = lv_btn_create(index_page, NULL);
    g_pcba_items[ITEM_RESULT].item_btn = rst_btn;

    lv_obj_set_event_cb(rst_btn, result_btn_cb);
    lv_obj_set_size(rst_btn, btn_w, BTN_H);
    //lv_obj_set_pos(rst_btn, btn_w * 2, SCREEN_H - BTN_H);
    lv_obj_align(rst_btn, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0/*SCREEN_H - BTN_H*/);

    static lv_style_t style_btn_rel;
    lv_style_copy(&style_btn_rel, &lv_style_btn_rel);
    style_btn_rel.body.opa = LV_OPA_70;
    style_btn_rel.body.border.width = 0;
    style_btn_rel.body.radius = 0;

    static lv_style_t style_btn_pr;
    lv_style_copy(&style_btn_pr, &lv_style_btn_pr);
    style_btn_pr.body.radius = 0;
    style_btn_pr.body.opa = LV_OPA_50;
    style_btn_pr.body.main_color = LV_COLOR_WHITE;
    style_btn_pr.body.grad_color = LV_COLOR_WHITE;
    style_btn_pr.body.border.width = 0;
    style_btn_pr.text.color = LV_COLOR_GRAY;

    lv_btn_set_style(rst_btn, LV_BTN_STYLE_PR,  &style_btn_pr);
    lv_btn_set_style(rst_btn, LV_BTN_STYLE_REL,  &style_btn_rel);

    label = lv_label_create(rst_btn, NULL);
    lv_label_set_recolor(label, true);
    lv_label_set_text(label, "#0000ff RESULT#");
    lv_obj_set_style(label, &label_style_font16);
}

/**
 * @description: pcba main page init
 * @param {int page}
 * @return: none
 */
void index_page_init(int p_page)
{
    rt_int16_t y = 0;

    if (!index_page)
    {
        index_page = lv_obj_create(lv_scr_act(), NULL);

        lv_obj_set_size(index_page, SCREEN_W, SCREEN_H);
        lv_obj_set_style(index_page, &page_pcba_style);
        //lv_obj_align(index_page, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
        lv_obj_invalidate(index_page);

        divide_line = lv_line_create(index_page, NULL);

        company_label = lv_label_create(index_page, NULL);
        // y = LABEL_Y_START + LABEL_Y_GAP;
        lv_label_set_long_mode(company_label, LV_LABEL_LONG_SROLL_CIRC);
        lv_label_set_align(company_label, LV_LABEL_ALIGN_CENTER);
        lv_obj_set_size(company_label, LABEL_W, LABEL_H);
        lv_obj_align(company_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);
        lv_label_set_text(company_label, "Rockchip Co.,Ltd.");

        platform_label = lv_label_create(index_page, NULL);
        y = LABEL_H;
        lv_label_set_long_mode(platform_label, LV_LABEL_LONG_SROLL_CIRC);
        lv_label_set_align(platform_label, LV_LABEL_ALIGN_CENTER);
        lv_label_set_recolor(platform_label, true);
        lv_obj_set_size(platform_label, LABEL_W, LABEL_H);
        lv_obj_align(platform_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);
        lv_label_set_text(platform_label, "#00ff00 RK2108 PCBA Test #");
    }

    set_buttn_globe_sylte();
    draw_divide_line();
    draw_test_item_buttons();
    // draw_arc_position(g_item_index);

    draw_result_button();
    lv_btn_set_state(g_pcba_items[g_item_index].item_btn, LV_BTN_STATE_PR);
}

/**
 * @description:
 * @param {type}
 * @return:
 */
void index_page_deinit(void)
{
    if (index_page)
    {
        LOG_D("%s start", __func__);

        /* this function will delete children too */
        lv_obj_del(index_page);
        index_page = NULL;
        arc_point = NULL;
    }
}

void index_page_key(size_t type, size_t value)
{
    if (type == TYPE_KEY_PRESS)
    {
        switch (value)
        {
        case KEY_SELECT:
            LOG_D("KEY SELECT");
            if ((g_item_index + 2) == MENU_RECORD_TEST_PAGE)
            {
                if (rootfs_check())
                    goto NO_ROOTFS;
                pcba_player_stop();
            }

            display_pcba_page(g_item_index + 2) ;
            break;

        case KEY_UP:
            LOG_D("KEY UP");
            lv_btn_set_state(g_pcba_items[g_item_index].item_btn, LV_BTN_STATE_REL);
            --g_item_index;
            LOG_D("g_item_index = %d", g_item_index);
            if (g_item_index < ITEM_SCREEN)
                g_item_index = ITEM_MAX - 1;
            // draw_arc_position(g_item_index);
            lv_btn_set_state(g_pcba_items[g_item_index].item_btn, LV_BTN_STATE_PR);
            break;

        case KEY_DOWN:
            LOG_D("KEY Down");
            lv_btn_set_state(g_pcba_items[g_item_index].item_btn, LV_BTN_STATE_REL);
            ++g_item_index;
            LOG_D("g_item_index = %d", g_item_index);
            if (g_item_index >= ITEM_MAX)
                g_item_index = 0;
            // draw_arc_position(g_item_index);
            lv_btn_set_state(g_pcba_items[g_item_index].item_btn, LV_BTN_STATE_PR);
            break;

        default:
            break;
        }
    }

    return;

NO_ROOTFS:
    LOG_E("No rootfs, please check");
}

void index_page_refresh(size_t type, size_t value)
{
}
