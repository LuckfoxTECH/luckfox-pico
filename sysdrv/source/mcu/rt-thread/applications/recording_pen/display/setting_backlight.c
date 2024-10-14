/*
 * Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-14     Jair Wu      First version
 *
 */

#include "setting_backlight.h"

#define DBG_SECTION_NAME    "SETBL"
#define DBG_LEVEL           DBG_INFO
#include "rtdbg.h"

#define SETTING_NUM     2

static int g_pPage = MENU_NONE_PAGE;
static rk_lv_menulist_t *ml = NULL;

static const char title[RECPEN_APP_LANG_MAX][MAX_LABEL_LEN] =
{
    "背光设置",
    "Backlight"
};

static const rk_lv_ml_element_t elements[SETTING_NUM] =
{
    {{"背光时长", "Timeout"},    load_setting, (void *)MENU_SETTING_BL_TIMEOUT_PAGE},
    {{"亮度设置", "Brightness"}, load_setting, (void *)MENU_SETTING_BL_BRIGHTNESS_PAGE},
};

void setting_backlight_key(size_t type, size_t value)
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

void setting_backlight_init(int p_page)
{
    if (ml == NULL)
    {
        if (g_pPage == MENU_NONE_PAGE)
            g_pPage = p_page;
        ml = rk_lv_menulist_create((char *)title[g_app_info.language], lv_scr_act(), NULL, (rk_lv_ml_element_t *)elements, SETTING_NUM);
        rk_lv_menulist_set_size(ml, SCREEN_W, SCREEN_H);
        rk_lv_menulist_set_style(ml, &label_style_font20_hl, &style_bar);
        rk_lv_menulist_set_language(ml, g_app_info.language);
    }
}

void setting_backlight_deinit(void)
{
    if (ml)
    {
        rk_lv_menulist_destroy(ml);
        ml = NULL;
    }
}
