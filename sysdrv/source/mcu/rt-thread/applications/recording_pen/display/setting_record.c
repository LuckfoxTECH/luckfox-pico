/*
 * Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-04-26     Jair Wu      First version
 *
 */

#include "setting_record.h"
#include "setting_backlight.h"

#define DBG_SECTION_NAME    "SETREC"
#define DBG_LEVEL           DBG_INFO
#include "rtdbg.h"

enum
{
    SETTING_SUBSECTION,
    SETTING_NUM,
};

static int g_pPage = MENU_NONE_PAGE;
static rk_lv_menulist_t *ml = NULL;

static const char title[RECPEN_APP_LANG_MAX][MAX_LABEL_LEN] =
{
    "录音设置",
    "Record Setting"
};

static const rk_lv_ml_element_t elements[SETTING_NUM] =
{
    {{"循环录音", "Segment Length"}, load_setting, (void *)MENU_SETTING_REC_SEGLEN_PAGE},
};

void setting_record_key(size_t type, size_t value)
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

void setting_record_init(int p_page)
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

void setting_record_deinit(void)
{
    if (ml)
    {
        rk_lv_menulist_destroy(ml);
        ml = NULL;
    }
}
