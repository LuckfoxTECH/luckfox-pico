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

#include "setting_backlight_timeout.h"

#define DBG_SECTION_NAME    "SETBLT"
#define DBG_LEVEL           DBG_INFO
#include "rtdbg.h"

#define BACKLIGHT_TIMEOUT_LEVELS    5
#define BACKLIGHT_TIMEOUT_15        15
#define BACKLIGHT_TIMEOUT_30        30
#define BACKLIGHT_TIMEOUT_60        60
#define BACKLIGHT_TIMEOUT_90        90
#define BACKLIGHT_TIMEOUT_120       120
#define BACKLIGHT_TIMEOUT_DEFAULT   BACKLIGHT_TIMEOUT_30

#define SETTING_NUM                 BACKLIGHT_TIMEOUT_LEVELS

static int g_pPage = MENU_NONE_PAGE;
static rk_lv_menulist_t *ml = NULL;

void set_bl_timeout(void *arg);

static const char title[RECPEN_APP_LANG_MAX][MAX_LABEL_LEN] =
{
    "背光时长",
    "Backlight Timeout"
};

static const rk_lv_ml_element_t elements[SETTING_NUM] =
{
    {{"15秒", "15s"},   set_bl_timeout, (void *)BACKLIGHT_TIMEOUT_15},
    {{"30秒", "30s"},   set_bl_timeout, (void *)BACKLIGHT_TIMEOUT_30},
    {{"60秒", "60s"},   set_bl_timeout, (void *)BACKLIGHT_TIMEOUT_60},
    {{"90秒", "90s"},   set_bl_timeout, (void *)BACKLIGHT_TIMEOUT_90},
    {{"120秒", "120s"}, set_bl_timeout, (void *)BACKLIGHT_TIMEOUT_120},
};

void reset_bl_timeout(void)
{
    g_app_info.backlight_timeout = BACKLIGHT_TIMEOUT_DEFAULT;
}

int get_bl_timeout(void)
{
    int timeout = g_app_info.backlight_timeout;

    /* Check if timeout is valid */
    for (int i = 0; i < BACKLIGHT_TIMEOUT_LEVELS; i++)
    {
        if (timeout == (int)elements[i].arg)
            return timeout;
    }

    return BACKLIGHT_TIMEOUT_DEFAULT;
}

void set_bl_timeout(void *arg)
{
    int timeout = (int)arg;

    if (g_app_info.backlight_timeout != timeout)
    {
        g_app_info.backlight_timeout = timeout;
        save_app_info();
        for (int i = 0; i < SETTING_NUM; i++)
        {
            if (elements[i].arg == arg)
            {
                toast_create((char *)elements[i].str[g_app_info.language], 1500);
                break;
            }
        }
    }
}

void setting_bl_timeout_key(size_t type, size_t value)
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

void setting_bl_timeout_init(int p_page)
{
    if (ml == NULL)
    {
        int n_timeout = g_app_info.backlight_timeout;
        int line = 0;
        for (int i = 0; i < BACKLIGHT_TIMEOUT_LEVELS; i++)
        {
            if (n_timeout == (int)elements[i].arg)
            {
                line = i;
                break;
            }
        }
        if (g_pPage == MENU_NONE_PAGE)
            g_pPage = p_page;
        ml = rk_lv_menulist_create((char *)title[g_app_info.language], lv_scr_act(), NULL, (rk_lv_ml_element_t *)elements, SETTING_NUM);
        rk_lv_menulist_set_size(ml, SCREEN_W, SCREEN_H);
        rk_lv_menulist_set_style(ml, &label_style_font20_hl, &style_bar);
        rk_lv_menulist_set_language(ml, g_app_info.language);
        rk_lv_menulist_select(ml, line);
    }
}

void setting_bl_timeout_deinit(void)
{
    if (ml)
    {
        rk_lv_menulist_destroy(ml);
        ml = NULL;
    }
}
