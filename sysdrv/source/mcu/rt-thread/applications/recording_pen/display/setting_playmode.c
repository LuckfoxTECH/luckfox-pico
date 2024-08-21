/*
 * Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-04-24     Jair Wu      First version
 *
 */

#include "setting_playmode.h"

#define DBG_SECTION_NAME    "SETMODE"
#define DBG_LEVEL           DBG_INFO
#include "rtdbg.h"

#define SETTING_NUM     3

static int g_pPage = MENU_NONE_PAGE;
static rk_lv_menulist_t *ml = NULL;

void set_playmode(void *arg);

static const char title[RECPEN_APP_LANG_MAX][MAX_LABEL_LEN] =
{
    "播放模式",
    "Playback Mode"
};

static const rk_lv_ml_element_t elements[SETTING_NUM] =
{
    {{"关闭循环", "Play once"}, set_playmode, (void *)PLAYMODE_ONCE},
    {{"单曲循环", "Single cycle"}, set_playmode, (void *)PLAYMODE_SINGLECYCLE},
    {{"列表循环", "List cycle"}, set_playmode, (void *)PLAYMODE_LISTCYCLE}
};

void set_playmode(void *arg)
{
    int mode = (int)arg;

    if (g_app_info.playmode != mode)
    {g_app_info.playmode = mode;
        save_app_info();
        toast_create((char *)elements[mode].str[g_app_info.language], 1500);
    }
}

void setting_playmode_key(size_t type, size_t value)
{
    if (type == TYPE_KEY_PRESS)
    {
        switch (value)
        {
        case KEY_UP:
            rk_lv_menulist_prev(ml);
            break;
        case KEY_DOWN:
            rk_lv_menulist_next(ml);
            break;
        case KEY_CANCEL:
            if (display_page(g_pPage) == RT_EOK)
                g_pPage = MENU_NONE_PAGE;
            break;
        case KEY_OK:
            rk_lv_menulist_excute(ml);
            break;
        default:
            break;
        }
    }
}

void setting_playmode_init(int p_page)
{
    if (ml == NULL)
    {
        int n_mode = g_app_info.playmode;
        if (n_mode > PLAYMODE_MAX || n_mode < PLAYMODE_ONCE)
            g_app_info.playmode = n_mode = PLAYMODE_ONCE;
        if (g_pPage == MENU_NONE_PAGE)
            g_pPage = p_page;
        ml = rk_lv_menulist_create((char *)title[g_app_info.language], lv_scr_act(), NULL, (rk_lv_ml_element_t *)elements, SETTING_NUM);
        rk_lv_menulist_set_size(ml, SCREEN_W, SCREEN_H);
        rk_lv_menulist_set_style(ml, &label_style_font20_hl, &style_bar);
        rk_lv_menulist_set_language(ml, g_app_info.language);
        rk_lv_menulist_select(ml, n_mode);
    }
}

void setting_playmode_deinit(void)
{
    if (ml)
    {
        rk_lv_menulist_destroy(ml);
        ml = NULL;
    }
}
