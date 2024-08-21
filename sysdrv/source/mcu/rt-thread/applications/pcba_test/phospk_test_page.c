/*
 * @Copyright: Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * @License: SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-20 10:20:33
 * @Company: Rockchip
 * @LastEditTime: 2020-06-29 11:47:46
 * @LastEditors: Do not edit
 * @Description:
 */

#include "main_page.h"
#include "pcba_public.h"
#include "phospk_test_page.h"

#define DBG_SECTION_NAME    "PHO_SPK_PAGE"
#define DBG_LEVEL           DBG_LOG
#include "rtdbg.h"

static lv_obj_t *phospk_page = NULL;
static lv_obj_t *divide_line = NULL;
static lv_obj_t *company_label = NULL;
static lv_obj_t *item_label = NULL;
static lv_obj_t *pass_btn = NULL;
static lv_obj_t *fail_btn = NULL;

static lv_obj_t *file_label = NULL;
static lv_obj_t *file_index_label = NULL;
static lv_obj_t *cur_time_label = NULL;
static lv_obj_t *tot_time_label = NULL;

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
            memcpy (g_result_info[ITEM_PHO_SPK], result, strlen(result));
        }
        else
        {
            LOG_D("Clicked Fail button");
            result = "NG";
            memcpy (g_result_info[ITEM_PHO_SPK], result, strlen(result));
        }

        display_pcba_page(MENU_MAIN_PAGE);
    }
}

static int draw_result_btn(void)
{
    lv_obj_t * label;

    /* Create PASS button */
    pass_btn = lv_btn_create(phospk_page, NULL);
    lv_obj_set_event_cb(pass_btn, btn_event_handler);
    lv_obj_set_size(pass_btn, BTN_W, BTN_H);
    lv_obj_align(pass_btn, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
    lv_btn_set_style(pass_btn, LV_BTN_STYLE_PR,  &style_tv_btn_pr);
    lv_btn_set_style(pass_btn, LV_BTN_STYLE_REL,  &style_tv_btn_rel);

    /* Create FAIL button */
    fail_btn = lv_btn_create(phospk_page, NULL);
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

static void draw_audio_info()
{
    rt_int16_t y;

    file_label = lv_label_create(phospk_page, NULL);
    y = LABEL_Y_START + LABEL_Y_GAP;
    lv_label_set_long_mode(file_label, LV_LABEL_LONG_SROLL_CIRC);
    lv_label_set_align(file_label, LV_LABEL_ALIGN_CENTER);
    lv_obj_set_size(file_label, LABEL_W, LABEL_H);
    lv_obj_align(file_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);

    file_index_label = lv_label_create(phospk_page, NULL);
    y += LABEL_Y_GAP + LABEL_H;
    lv_label_set_long_mode(file_index_label, LV_LABEL_LONG_EXPAND);
    lv_obj_set_size(file_index_label, LABEL_W, LABEL_H);
    lv_obj_align(file_index_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);

    cur_time_label = lv_label_create(phospk_page, NULL);
    y += LABEL_Y_GAP + LABEL_H;
    lv_label_set_long_mode(cur_time_label, LV_LABEL_LONG_EXPAND);
    lv_obj_set_size(cur_time_label, LABEL_W, LABEL_H);
    lv_obj_align(cur_time_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);
    lv_obj_set_style(cur_time_label, &label_style_font20_hl);

    tot_time_label = lv_label_create(phospk_page, NULL);
    y += LABEL_Y_GAP + LABEL_H;
    lv_label_set_long_mode(tot_time_label, LV_LABEL_LONG_EXPAND);
    lv_obj_set_size(tot_time_label, LABEL_W, LABEL_H);
    lv_obj_align(tot_time_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);
    lv_obj_set_style(tot_time_label, &label_style_font20_hl);
}

/**
 * @description: pcba phone & speaker test page init
 * @param {int page}
 * @return: none
 */
void phospk_page_init(int p_page)
{
    short int y = 0;
    char str_test_item[64];
    LOG_D("%s enter...", __func__);

    if (!phospk_page)
    {
        phospk_page = lv_obj_create(lv_scr_act(), NULL);

        lv_obj_set_size(phospk_page, SCREEN_W, SCREEN_H);
        lv_obj_set_style(phospk_page, &label_style_font16);
        lv_obj_align(phospk_page, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
        lv_obj_invalidate(phospk_page);

        divide_line = lv_line_create(phospk_page, NULL);

        company_label = lv_label_create(phospk_page, NULL);
        lv_label_set_long_mode(company_label, LV_LABEL_LONG_SROLL_CIRC);
        lv_label_set_align(company_label, LV_LABEL_ALIGN_CENTER);
        lv_obj_set_size(company_label, LABEL_W, LABEL_H);
        lv_obj_align(company_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);
        lv_label_set_text(company_label, "Rockchip Co.,Ltd.");
        lv_obj_set_style(company_label, &label_style_font16);

        item_label = lv_label_create(phospk_page, NULL);
        y = LABEL_H;
        lv_label_set_long_mode(item_label, LV_LABEL_LONG_SROLL_CIRC);
        lv_label_set_align(item_label, LV_LABEL_ALIGN_CENTER);
        lv_label_set_recolor(item_label, true);
        lv_obj_set_size(item_label, LABEL_W, LABEL_H);
        lv_obj_align(item_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);
        rt_snprintf(str_test_item, sizeof(str_test_item), "#00ff00 %s#", pcba_test_item_str[ITEM_PHO_SPK]);
        lv_label_set_text(item_label, str_test_item);

        lv_obj_set_style(company_label, &label_style_font16);
    }

    draw_divide_line();

    draw_audio_info();

    draw_result_btn();
}

/**
 * @description:
 * @param {type}
 * @return:
 */
void phospk_page_deinit(void)
{
    if (phospk_page)
    {
        g_pcba.file.total_file = 0;
        /* this function will delete children too */
        lv_obj_del(phospk_page);
        phospk_page = NULL;
    }
}

void phospk_page_key(size_t type, size_t value)
{
    if (type == TYPE_KEY_PRESS)
    {
        switch (value)
        {
            case KEY_PLAY:
                if (rootfs_check())
                    goto NO_ROOTFS;
                switch (g_pcba.player_state)
                {
                case PLAYER_STATE_RUNNING:
                    LOG_I("Player pause");
                    g_pcba.player_state = PLAYER_STATE_PAUSED;
                    player_pause(g_pcba.player);
                    break;
                case PLAYER_STATE_PAUSED:
                    LOG_I("Player resume");
                    g_pcba.player_state = PLAYER_STATE_RUNNING;
                    player_resume(g_pcba.player);
                    break;
                case PLAYER_STATE_IDLE:
                case PLAYER_STATE_ERROR:
                case PLAYER_STATE_STOP:
                default:
                    LOG_I("Player start");
                    g_pcba.player_state = PLAYER_STATE_RUNNING;
                    pcba_player_prepare(g_pcba.file.cur_file_name, 0);
                    break;
                }
            // main_page_refresh(TYPE_REFRESH, REFRESH_PLAY_STATE);
            break;

        case KEY_UP:
            pcba_player_stop();
            lv_btn_set_state(pass_btn,LV_BTN_STATE_PR);
            btn_event_handler(pass_btn, LV_EVENT_CLICKED);
            break;

        case KEY_DOWN:
            pcba_player_stop();
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

static rt_err_t draw_file_info(void)
{
    char str[138];
    rt_int16_t y;

    /* draw file name */
    memset((void *)str, 0x0, sizeof(str));
    if (g_pcba.file.cur_file_name[0] == 0)
        return RT_EOK;

    rt_snprintf(str, sizeof(str), "%s", g_pcba.file.cur_file_name);
    y = LABEL_Y_START + LABEL_Y_GAP;

    lv_label_set_long_mode(file_label, LV_LABEL_LONG_SROLL_CIRC);
    lv_label_set_text(file_label, str);
    int16_t width = lv_obj_get_width(file_label);
    if (width < LABEL_W)
    {
        lv_label_set_long_mode(file_label, LV_LABEL_LONG_BREAK);
        lv_obj_set_size(file_label, LABEL_W, LABEL_H);
        y = lv_obj_get_y(file_label);
        lv_obj_align(file_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);
    }

    /* draw file index */
    memset((void *)str, 0x0, sizeof(str));
    rt_snprintf(str, sizeof(str), "%d/%d", g_pcba.file.cur_file, g_pcba.file.total_file);
    lv_label_set_text(file_index_label, str);
    y = lv_obj_get_y(file_index_label);
    lv_obj_align(file_index_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);

    return RT_EOK;
}

static void draw_audio_time(void)
{
    char str[32];
    rt_int16_t y;
    uint32_t time;

    /* draw current time */
    memset((void *)str, 0x0, sizeof(str));
    if (g_pcba.player_total_time == 0)
        return;

    time = g_pcba.player_cur_time / 1000;
    uint16_t hour = time / 60 / 60;
    time %= 60 * 60;
    uint16_t min = time / 60;
    uint16_t sec = time % 60;
    rt_snprintf(str, sizeof(str), "%02d:%02d:%02d", hour, min, sec);
    lv_label_set_text(cur_time_label, str);
    y = lv_obj_get_y(cur_time_label);
    lv_obj_align(cur_time_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);

    /* draw file duration */
    memset((void *)str, 0x0, sizeof(str));
    time = g_pcba.player_total_time / 1000;
    hour = time / 60 / 60;
    time %= 60 * 60;
    min = time / 60;
    sec = time % 60;
    rt_snprintf(str, sizeof(str), "%02d:%02d:%02d", hour, min, sec);
    lv_label_set_text(tot_time_label, str);
    y = lv_obj_get_y(tot_time_label);
    lv_obj_align(tot_time_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);
}


void phospk_page_refresh(size_t type, size_t value)
{
    switch (type)
    {
    case TYPE_REFRESH:
        if (value & REFRESH_MEDIA_INFO)
            draw_audio_time();
        if (value & REFRESH_FILE_INFO)
            draw_file_info();
        break;

    case TYPE_PLAYBACK_DONE:
        if (g_pcba.player_disable_callback)
        {
            LOG_I("No callback");
            g_pcba.player_disable_callback = 0;
            break;
        }
        LOG_I("Playback callback");
        switch (g_pcba.playmode)
        {
        case PLAYMODE_SINGLECYCLE:
            LOG_I("Signle cycle");
            g_pcba.player_state = PLAYER_STATE_RUNNING;
            pcba_player_prepare(g_pcba.file.cur_file_name, 0);
            phospk_page_refresh(TYPE_REFRESH, REFRESH_PLAY_STATE);
            break;

        case PLAYMODE_ONCE:
        default:
            LOG_I("PLay once");
            g_pcba.player_state = PLAYER_STATE_IDLE;
            phospk_page_refresh(TYPE_REFRESH, REFRESH_PLAY_STATE);
            break;
        }
        break;
    default:
        break;
    }
}

