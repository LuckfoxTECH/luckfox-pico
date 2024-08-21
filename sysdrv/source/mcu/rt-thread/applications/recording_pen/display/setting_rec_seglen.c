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

#include "setting_rec_seglen.h"

#define DBG_SECTION_NAME    "SETSEG"
#define DBG_LEVEL           DBG_INFO
#include "rtdbg.h"

#define RECORD_SEGLEN_LEVELS    6
#ifdef RECPEN_RECORD_MAX_DURATION
#define RECORD_SEGLEN_OFF       RECPEN_RECORD_MAX_DURATION
#else
#define RECORD_SEGLEN_OFF       0
#endif
#define RECORD_SEGLEN_1         1
#define RECORD_SEGLEN_30        30
#define RECORD_SEGLEN_60        60
#define RECORD_SEGLEN_90        90
#define RECORD_SEGLEN_120       120
#define RECORD_SEGLEN_DEFAULT   RECORD_SEGLEN_OFF

#define SETTING_NUM                 RECORD_SEGLEN_LEVELS

static int g_pPage = MENU_NONE_PAGE;
static rk_lv_menulist_t *ml = NULL;

void set_seglen(void *arg);

static const char title[RECPEN_APP_LANG_MAX][MAX_LABEL_LEN] =
{
    "循环录音",
    "Segment Length"
};

static const rk_lv_ml_element_t elements[SETTING_NUM] =
{
    {{"关闭循环", "Off"}, set_seglen, (void *)RECORD_SEGLEN_OFF},
    {{"1分钟", "1 min"}, set_seglen, (void *)RECORD_SEGLEN_1},
    {{"30分钟", "30 mins"}, set_seglen, (void *)RECORD_SEGLEN_30},
    {{"60分钟", "60 mins"}, set_seglen, (void *)RECORD_SEGLEN_60},
    {{"90分钟", "90 mins"}, set_seglen, (void *)RECORD_SEGLEN_90},
    {{"120分钟", "120 mins"}, set_seglen, (void *)RECORD_SEGLEN_120},
};

void reset_seglen(void)
{
    g_app_info.record_seglen = RECORD_SEGLEN_DEFAULT;
}

int get_seglen(void)
{
    int seglen = g_app_info.record_seglen;

    /* Check if seglen is valid */
    for (int i = 0; i < RECORD_SEGLEN_LEVELS; i++)
    {
        if (seglen == (int)elements[i].arg)
            return seglen;
    }

    return RECORD_SEGLEN_DEFAULT;
}

void set_seglen(void *arg)
{
    int seglen = (int)arg;

    if (g_app_info.record_seglen != seglen)
    {
        g_app_info.record_seglen = seglen;
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

void setting_seglen_key(size_t type, size_t value)
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

void setting_seglen_init(int p_page)
{
    if (ml == NULL)
    {
        int line = 0;
        for (int i = 0; i < RECORD_SEGLEN_LEVELS; i++)
        {
            if (g_app_info.record_seglen == (int)elements[i].arg)
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

void setting_seglen_deinit(void)
{
    if (ml)
    {
        rk_lv_menulist_destroy(ml);
        ml = NULL;
    }
}
