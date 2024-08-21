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

#include "main_page.h"

#define DBG_SECTION_NAME    "MAINPAGE"
#define DBG_LEVEL           DBG_INFO
#include "rtdbg.h"

extern lv_img_dsc_t icon_playonce;
extern lv_img_dsc_t icon_singlecycle;
extern lv_img_dsc_t icon_listcycle;
extern lv_img_dsc_t icon_play;
extern lv_img_dsc_t icon_pause;
extern lv_img_dsc_t icon_stop;
extern lv_img_dsc_t icon_bat0;
extern lv_img_dsc_t icon_bat25;
extern lv_img_dsc_t icon_bat50;
extern lv_img_dsc_t icon_bat75;
extern lv_img_dsc_t icon_bat100;
extern lv_img_dsc_t icon_bt;
extern lv_img_dsc_t icon_wifi;
extern lv_img_dsc_t icon_prev;
extern lv_img_dsc_t icon_next;
extern lv_img_dsc_t icon_menu;

static lv_img_dsc_t *play_mode_img;

static lv_obj_t *main_page = NULL;

static lv_obj_t *btn_menu = NULL;

static lv_obj_t *bt_icon = NULL;
static lv_obj_t *wifi_icon = NULL;
static lv_obj_t *bat_icon = NULL;
static lv_obj_t *play_icon = NULL;
static lv_obj_t *prev_icon = NULL;
static lv_obj_t *next_icon = NULL;
static lv_obj_t *menu_icon = NULL;
static lv_obj_t *playmode_icon = NULL;

static lv_obj_t *file_label = NULL;
static lv_obj_t *file_index_label = NULL;
static lv_obj_t *cur_time_label = NULL;
static lv_obj_t *time_label = NULL;

static lv_obj_t *date_time_label = NULL;

static lv_obj_t *duration_bar;

static rt_err_t draw_playmode_icon(void)
{
    size_t value = g_app_info.playmode;
    if (value == PLAYMODE_SINGLECYCLE)
        play_mode_img = &icon_singlecycle;
    else if (value == PLAYMODE_LISTCYCLE)
        play_mode_img = &icon_listcycle;
    else
        play_mode_img = &icon_playonce;

    lv_obj_set_size(playmode_icon, ICON_W, ICON_H);
    lv_obj_align(playmode_icon, NULL, LV_ALIGN_CENTER,
                 (SCREEN_W / 2 - 20), 20);
    lv_img_set_src(playmode_icon, play_mode_img);
    lv_obj_invalidate(playmode_icon);

    return RT_EOK;
}

static rt_err_t draw_bat_icon(void)
{
    lv_img_dsc_t *bat_img_dsc;
    size_t value = g_app_info.bat;
    if (value >= 95)
        bat_img_dsc = &icon_bat100;
    else if (value >= 75)
        bat_img_dsc = &icon_bat75;
    else if (value >= 50)
        bat_img_dsc = &icon_bat50;
    else if (value >= 25)
        bat_img_dsc = &icon_bat25;
    else
        bat_img_dsc = &icon_bat0;

    lv_obj_set_size(bat_icon, bat_img_dsc->header.w, bat_img_dsc->header.h);
    lv_obj_align(bat_icon, NULL, LV_ALIGN_IN_TOP_RIGHT,
                 -ICON_X_GAP, ICON_Y_START + (ICON_H - bat_img_dsc->header.h) / 2);
    lv_img_set_src(bat_icon, bat_img_dsc);
    lv_obj_invalidate(bat_icon);

    return RT_EOK;
}

static rt_err_t draw_bt_icon(void)
{
    lv_obj_set_size(bt_icon, icon_bt.header.w, icon_bt.header.h);
    lv_obj_align(bt_icon, NULL, LV_ALIGN_IN_TOP_RIGHT,
                 -ICON_X_GAP - (ICON_W + ICON_X_GAP + (ICON_W - icon_bt.header.w) / 2),
                 ICON_Y_START + (ICON_H - icon_bt.header.h) / 2);
    lv_img_set_src(bt_icon, &icon_bt);
    lv_obj_invalidate(bt_icon);

    return RT_EOK;
}

static rt_err_t draw_wifi_icon(void)
{
    lv_obj_set_size(wifi_icon, icon_wifi.header.w, icon_wifi.header.h);
    lv_obj_align(wifi_icon, NULL, LV_ALIGN_IN_TOP_RIGHT,
                 -ICON_X_GAP - ((ICON_W + ICON_X_GAP) * 2 + (ICON_W - icon_wifi.header.w) / 2),
                 ICON_Y_START + (ICON_H - icon_wifi.header.h) / 2);
    lv_img_set_src(wifi_icon, &icon_wifi);
    lv_obj_invalidate(wifi_icon);

    return RT_EOK;
}

static rt_err_t draw_btns(void)
{
    lv_obj_set_size(prev_icon, icon_prev.header.w, icon_prev.header.h);
    lv_obj_align(prev_icon, NULL, LV_ALIGN_IN_BOTTOM_LEFT, (44 - icon_prev.header.w) / 2, -20);
    lv_img_set_src(prev_icon, &icon_prev);
    lv_obj_invalidate(prev_icon);

    lv_obj_set_size(next_icon, icon_next.header.w, icon_next.header.h);
    lv_obj_align(next_icon, NULL, LV_ALIGN_IN_BOTTOM_LEFT, (44 - icon_next.header.w) / 2 + 44 * 2, -20);
    lv_img_set_src(next_icon, &icon_next);
    lv_obj_invalidate(next_icon);

    lv_obj_set_size(menu_icon, icon_menu.header.w, icon_menu.header.h);
    lv_obj_align(menu_icon, NULL, LV_ALIGN_IN_BOTTOM_LEFT, (44 - icon_menu.header.w) / 2 + 44 * 3, -20);
    lv_img_set_src(menu_icon, &icon_menu);
    lv_obj_invalidate(menu_icon);

    return RT_EOK;
}

static rt_err_t draw_play_icon(void)
{
    lv_img_dsc_t *play_state_img_dsc;
    player_state_t state = g_app_info.player_state;
    switch (state)
    {
    case PLAYER_STATE_RUNNING:
        play_state_img_dsc = &icon_play;
        break;
    case PLAYER_STATE_PAUSED:
        play_state_img_dsc = &icon_pause;
        break;
    case PLAYER_STATE_IDLE:
    case PLAYER_STATE_ERROR:
    case PLAYER_STATE_STOP:
    default:
        play_state_img_dsc = &icon_stop;
        break;
    }
    lv_obj_set_size(play_icon, play_state_img_dsc->header.w, play_state_img_dsc->header.h);
    lv_obj_align(play_icon, NULL, LV_ALIGN_IN_BOTTOM_LEFT, (44 - play_state_img_dsc->header.w) / 2 + 44, -20);
    lv_img_set_src(play_icon, play_state_img_dsc);
    lv_obj_invalidate(play_icon);

    return RT_EOK;
}

static rt_err_t draw_btn_menu(void)
{
    lv_obj_set_size(btn_menu, STATE_BAR_W, STATE_BAR_H);
    lv_obj_align(btn_menu, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
    lv_obj_invalidate(btn_menu);

    return RT_EOK;
}

static rt_err_t draw_file_info(void)
{
    char str[138];
    rt_int16_t y;

    /* draw file name */
    memset((void *)str, 0x0, sizeof(str));
    sprintf(str, "%s", g_app_info.file.cur_file_name);
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
    sprintf(str, "%d/%d", g_app_info.file.cur_file, g_app_info.file.total_file);
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
    uint32_t max_time = g_app_info.player_total_time / 1000;
    uint32_t cur_time = g_app_info.player_cur_time / 1000;

    /* draw current time */
    memset((void *)str, 0x0, sizeof(str));
    time = g_app_info.player_cur_time / 1000;
    uint16_t min = time / 60;
    uint16_t sec = time % 60;
    sprintf(str, "%02d:%02d", min, sec);
    lv_label_set_text(cur_time_label, str);
    y = lv_obj_get_y(cur_time_label);
    lv_obj_align(cur_time_label, NULL, LV_ALIGN_IN_TOP_LEFT, 10, y);

    /* draw file duration */
    memset((void *)str, 0x0, sizeof(str));
    time = g_app_info.player_total_time / 1000;
    min = time / 60;
    sec = time % 60;
    sprintf(str, "%02d:%02d", min, sec);
    lv_label_set_text(time_label, str);
    y = lv_obj_get_y(time_label);
    lv_obj_align(time_label, NULL, LV_ALIGN_IN_TOP_RIGHT, -10, y);

    if (max_time > 32767)
    {
        cur_time = 32767.0 / max_time * cur_time;
        max_time = 32767;
    }
    lv_bar_set_range(duration_bar, 0, max_time);
    lv_bar_set_value(duration_bar, cur_time, LV_ANIM_OFF);
}

static void draw_date(void)
{
    char str[32];

    memset((void *)str, 0x0, sizeof(str));
    sprintf(str, "%02d:%02d:%02d",
            g_app_info.date->tm_hour,
            g_app_info.date->tm_min,
            g_app_info.date->tm_sec);
    lv_label_set_text(date_time_label, str);
    lv_obj_align(date_time_label, NULL, LV_ALIGN_IN_TOP_LEFT, ICON_X_GAP, 0);
}

void main_page_key(size_t type, size_t value)
{
    if (type == TYPE_KEY_PRESS)
    {
        switch (value)
        {
        case KEY_RECORD:
            if (rootfs_check())
                goto NO_ROOTFS;
            recpen_player_stop();
            display_page(MENU_REC_PAGE);
            break;
        case KEY_DOWN:
            if (rootfs_check())
                goto NO_ROOTFS;
            recpen_player_stop();
            if (recpen_next_file() == RT_EOK)
            {
                g_app_info.player_total_time = recpen_player_get_total_time(g_app_info.file.cur_file_name);
                g_app_info.player_cur_time = 0;
                main_page_refresh(TYPE_REFRESH, REFRESH_PLAY_STATE | REFRESH_MEDIA_INFO | REFRESH_FILE_INFO);
            }
            else
            {
                toast_create("No file\0", 1500);
            }
            break;
        case KEY_UP:
            if (rootfs_check())
                goto NO_ROOTFS;
            recpen_player_stop();
            if (recpen_prev_file() == RT_EOK)
            {
                g_app_info.player_total_time = recpen_player_get_total_time(g_app_info.file.cur_file_name);
                g_app_info.player_cur_time = 0;
                main_page_refresh(TYPE_REFRESH, REFRESH_PLAY_STATE | REFRESH_MEDIA_INFO | REFRESH_FILE_INFO);
            }
            else
            {
                toast_create("No file\0", 1500);
            }
            break;
        case KEY_MENU:
            recpen_player_stop();
            display_page(MENU_SETTING_PAGE);
            break;
        case KEY_PLAY:
            if (rootfs_check())
                goto NO_ROOTFS;
            switch (g_app_info.player_state)
            {
            case PLAYER_STATE_RUNNING:
                LOG_I("Player pause");
                g_app_info.player_state = PLAYER_STATE_PAUSED;
                player_pause(g_app_info.player);
                break;
            case PLAYER_STATE_PAUSED:
                LOG_I("Player resume");
                g_app_info.player_state = PLAYER_STATE_RUNNING;
                player_resume(g_app_info.player);
                break;
            case PLAYER_STATE_IDLE:
            case PLAYER_STATE_ERROR:
            case PLAYER_STATE_STOP:
            default:
                LOG_I("Player start");
                g_app_info.player_state = PLAYER_STATE_RUNNING;
                recpen_player_prepare(g_app_info.file.cur_file_name, 0);
                break;
            }
            main_page_refresh(TYPE_REFRESH, REFRESH_PLAY_STATE);
            break;
        default:
            break;
        }
    }

    return;
NO_ROOTFS:
    toast_create("No rootfs, please check", 3000);
}

void main_page_refresh(size_t type, size_t value)
{
    switch (type)
    {
    case TYPE_REFRESH:
        if (value & REFRESH_DATE)
            draw_date();
        if (value & REFRESH_WIFI)
            draw_wifi_icon();
        if (value & REFRESH_BT)
            draw_bt_icon();
        if (value & REFRESH_BAT)
            draw_bat_icon();
        if (value & REFRESH_PLAY_STATE)
            draw_play_icon();
        if (value & REFRESH_MEDIA_INFO)
            draw_audio_time();
        if (value & REFRESH_FILE_INFO)
            draw_file_info();
        lv_obj_invalidate(btn_menu);
        break;
    case TYPE_PLAYBACK_DONE:
        LOG_I("Playback callback");
        switch (g_app_info.playmode)
        {
        case PLAYMODE_SINGLECYCLE:
            LOG_I("Signle cycle");
            g_app_info.player_state = PLAYER_STATE_RUNNING;
            recpen_player_prepare(g_app_info.file.cur_file_name, 0);
            main_page_refresh(TYPE_REFRESH, REFRESH_PLAY_STATE);
            break;
        case PLAYMODE_LISTCYCLE:
            LOG_I("List cycle");
            if (recpen_next_file() == RT_EOK)
            {
                g_app_info.player_cur_time = 0;
                g_app_info.player_total_time = recpen_player_get_total_time(g_app_info.file.cur_file_name);
                g_app_info.player_state = PLAYER_STATE_RUNNING;
                main_page_refresh(TYPE_REFRESH, REFRESH_PLAY_STATE | REFRESH_MEDIA_INFO | REFRESH_FILE_INFO);
                rt_thread_mdelay(200);
                recpen_player_prepare(g_app_info.file.cur_file_name, 0);
            }
            break;
        case PLAYMODE_ONCE:
        default:
            LOG_I("PLay once");
            g_app_info.player_state = PLAYER_STATE_IDLE;
            main_page_refresh(TYPE_REFRESH, REFRESH_PLAY_STATE);
            break;
        }
        break;
    default:
        break;
    }
}

void main_page_init(int p_page)
{
    rt_int16_t y;

    if (!main_page)
    {
        main_page = lv_obj_create(lv_scr_act(), NULL);
        lv_obj_add_style(main_page, LV_OBJ_PART_MAIN, &page_common_style);

        lv_obj_set_size(main_page, SCREEN_W, SCREEN_H);
        lv_obj_align(main_page, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
        lv_obj_invalidate(main_page);

        btn_menu = lv_obj_create(main_page, NULL);
        lv_obj_add_style(btn_menu, LV_OBJ_PART_MAIN, &style_bar);

        bt_icon = lv_img_create(main_page, NULL);
        wifi_icon = lv_img_create(main_page, NULL);
        bat_icon = lv_img_create(main_page, NULL);
        play_icon = lv_img_create(main_page, NULL);
        prev_icon = lv_img_create(main_page, NULL);
        next_icon = lv_img_create(main_page, NULL);
        menu_icon = lv_img_create(main_page, NULL);
        playmode_icon = lv_img_create(main_page, NULL);

        y = LABEL_Y_START + LABEL_Y_GAP;
        file_label = lv_label_create(main_page, NULL);
        lv_label_set_long_mode(file_label, LV_LABEL_LONG_SROLL_CIRC);
        lv_label_set_align(file_label, LV_LABEL_ALIGN_CENTER);
        lv_obj_set_size(file_label, LABEL_W, LABEL_H);
        lv_obj_align(file_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);

        y += (LABEL_Y_GAP + LABEL_H) * 2;
        file_index_label = lv_label_create(main_page, NULL);
        lv_label_set_long_mode(file_index_label, LV_LABEL_LONG_EXPAND);
        lv_obj_set_size(file_index_label, LABEL_W, LABEL_H);
        lv_obj_align(file_index_label, NULL, LV_ALIGN_IN_TOP_MID,
                     (SCREEN_W / 2 - 10), y);

        duration_bar = lv_bar_create(main_page, NULL);
        y += LABEL_Y_GAP + LABEL_H;
        lv_obj_set_size(duration_bar, SETTING_LABEL_W - 20, 3);
        lv_obj_align(duration_bar, NULL, LV_ALIGN_IN_TOP_MID, 0, y + 20);

        y += LABEL_Y_GAP + LABEL_H;
        cur_time_label = lv_label_create(main_page, NULL);
        lv_label_set_long_mode(cur_time_label, LV_LABEL_LONG_EXPAND);
        lv_obj_set_size(cur_time_label, LABEL_W, LABEL_H);
        lv_obj_align(cur_time_label, NULL, LV_ALIGN_IN_TOP_LEFT, 0, y);

        time_label = lv_label_create(main_page, NULL);
        lv_label_set_long_mode(time_label, LV_LABEL_LONG_EXPAND);
        lv_obj_set_size(time_label, LABEL_W, LABEL_H);
        lv_obj_align(time_label, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, y);

        date_time_label = lv_label_create(main_page, NULL);
        lv_label_set_long_mode(date_time_label, LV_LABEL_LONG_EXPAND);
        lv_obj_add_style(date_time_label, LV_OBJ_PART_MAIN, &label_style_font16);
    }
    draw_btn_menu();
    draw_bt_icon();
    draw_wifi_icon();
    draw_btns();
    draw_bat_icon();
    draw_play_icon();
    draw_playmode_icon();
    draw_file_info();
    draw_audio_time();
    draw_date();
}

void main_page_deinit(void)
{
    if (main_page)
    {
        lv_obj_remove_style(date_time_label, LV_OBJ_PART_MAIN, &label_style_font16);
        lv_obj_remove_style(main_page, LV_OBJ_PART_MAIN, &page_common_style);
        lv_obj_remove_style(btn_menu, LV_OBJ_PART_MAIN, &style_bar);

        /* this function will delete children too */
        lv_obj_del(main_page);
        main_page = NULL;
    }
}
