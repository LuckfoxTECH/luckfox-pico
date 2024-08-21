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

#include "setting_date.h"

#define DBG_SECTION_NAME    "SETLANG"
#define DBG_LEVEL           DBG_INFO
#include "rtdbg.h"

static lv_obj_t *setting_date;
static lv_obj_t *pick;
static lv_obj_t *setting_date_label[2][5];
static int g_pPage = MENU_NONE_PAGE;
static struct tm date;

static size_t g_select = 0;

static const char label_tag[5][128] =
{
    "Year",
    "Month",
    "Day",
    "Hour",
    "Minute"
};

void setting_date_refresh(void)
{
    char str[5][32];
    int i;

    sprintf(str[0], "%4d", date.tm_year);
    sprintf(str[1], "%02d", date.tm_mon + 1);
    sprintf(str[2], "%02d", date.tm_mday);
    sprintf(str[3], "%02d", date.tm_hour);
    sprintf(str[4], "%02d", date.tm_min);

    for (i = 0; i < 5; i++)
        lv_label_set_text(setting_date_label[1][i], str[i]);

    lv_obj_set_size(pick, lv_obj_get_x(setting_date_label[0][g_select]) -
                    lv_obj_get_x(setting_date_label[1][g_select]) +
                    lv_obj_get_width(setting_date_label[0][g_select]) + SETTING_LABEL_GAP * 2,
                    lv_obj_get_height(setting_date_label[1][g_select]) + SETTING_LABEL_GAP * 2);
    lv_obj_align(pick, setting_date_label[1][g_select], LV_ALIGN_IN_TOP_LEFT, -SETTING_LABEL_GAP, -SETTING_LABEL_GAP);
}

uint8_t month_days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
void setting_date_key(size_t type, size_t value)
{
    if (type == TYPE_KEY_PRESS)
    {
        switch (value)
        {
        case KEY_DOWN:
            switch (g_select)
            {
            case 0:
                date.tm_year++;
                if (date.tm_year > 2120)
                    date.tm_year = 2120;
                if (date.tm_year % 4 == 0)
                    month_days[1] = 29;
                else
                    month_days[1] = 28;
                if (date.tm_mday > month_days[date.tm_mon])
                    date.tm_mday = month_days[date.tm_mon];
                break;
            case 1:
                date.tm_mon++;
                if (date.tm_mon > 11)
                    date.tm_mon = 0;
                if (date.tm_mday > month_days[date.tm_mon])
                    date.tm_mday = month_days[date.tm_mon];
                break;
            case 2:
                date.tm_mday++;
                if (date.tm_mday > month_days[date.tm_mon])
                    date.tm_mday = 1;
                break;
            case 3:
                date.tm_hour++;
                if (date.tm_hour > 23)
                    date.tm_hour = 0;
                break;
            case 4:
                date.tm_min++;
                if (date.tm_min > 59)
                    date.tm_min = 0;
                break;
            default:
                g_select = 0;
                break;
            }
            setting_date_refresh();
            break;
        case KEY_UP:
            switch (g_select)
            {
            case 0:
                date.tm_year--;
                if (date.tm_year < 1920 || date.tm_year > 2120)
                    date.tm_year = 1920;
                if (date.tm_year % 4 == 0)
                    month_days[1] = 29;
                else
                    month_days[1] = 28;
                if (date.tm_mday > month_days[date.tm_mon])
                    date.tm_mday = month_days[date.tm_mon];
                break;
            case 1:
                date.tm_mon--;
                if (date.tm_mon < 0 || date.tm_mon > 11)
                    date.tm_mon = 11;
                if (date.tm_mday > month_days[date.tm_mon])
                    date.tm_mday = month_days[date.tm_mon];
                break;
            case 2:
                date.tm_mday--;
                if (date.tm_mday < 1)
                    date.tm_mday = month_days[date.tm_mon];
                break;
            case 3:
                date.tm_hour--;
                if (date.tm_hour < 0 || date.tm_hour > 23)
                    date.tm_hour = 23;
                break;
            case 4:
                date.tm_min--;
                if (date.tm_min < 0 || date.tm_min > 59)
                    date.tm_min = 59;
                break;
            default:
                g_select = 0;
                break;
            }
            setting_date_refresh();
            break;
        case KEY_SELECT:
            g_select++;
            if (g_select > 4)
                g_select = 0;
            setting_date_refresh();
            break;
        case KEY_CANCEL:
            if (display_page(g_pPage) == RT_EOK)
                g_pPage = MENU_NONE_PAGE;
            break;
        case KEY_OK:
#ifdef RT_USING_RTC
            set_date(date.tm_year, date.tm_mon + 1, date.tm_mday);
            set_time(date.tm_hour, date.tm_min, date.tm_sec);
#endif
            if (display_page(g_pPage) == RT_EOK)
                g_pPage = MENU_NONE_PAGE;
            break;
        default:
            break;
        }
    }
}

void setting_date_init(int p_page)
{
    memcpy((void *)&date, (void *)g_app_info.date, sizeof(struct tm));
    date.tm_year += 1900;
    g_select = 0;
    if (date.tm_year % 4 == 0)
        month_days[1] = 29;
    else
        month_days[1] = 28;
    if (!setting_date)
    {
        if (g_pPage == MENU_NONE_PAGE)
            g_pPage = p_page;
        setting_date = lv_obj_create(lv_scr_act(), NULL);

        lv_obj_set_size(setting_date, SCREEN_W, SCREEN_H);
        lv_obj_add_style(setting_date, LV_OBJ_PART_MAIN, &page_common_style);
        lv_obj_align(setting_date, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

        pick = lv_obj_create(setting_date, NULL);
        lv_obj_add_style(pick, LV_OBJ_PART_MAIN, &style_bar);

        int y = 28;
        for (int i = 0; i < 5; i++)
        {
            setting_date_label[1][i] = lv_label_create(setting_date, NULL);
            lv_obj_add_style(setting_date_label[1][i], LV_OBJ_PART_MAIN, &label_style_font20_letter);
            lv_label_set_long_mode(setting_date_label[1][i], LV_LABEL_LONG_EXPAND);
            lv_label_set_align(setting_date_label[1][i], LV_LABEL_ALIGN_LEFT);
            lv_obj_set_size(setting_date_label[1][i], SETTING_LABEL_W - 40, SETTING_LABEL_H);
            lv_obj_align(setting_date_label[1][i], NULL, LV_ALIGN_IN_TOP_LEFT, 30, y);

            setting_date_label[0][i] = lv_label_create(setting_date, NULL);
            lv_obj_add_style(setting_date_label[0][i], LV_OBJ_PART_MAIN, &label_style_font20_letter);
            lv_label_set_text(setting_date_label[0][i], label_tag[i]);
            lv_label_set_long_mode(setting_date_label[0][i], LV_LABEL_LONG_EXPAND);
            lv_label_set_align(setting_date_label[0][i], LV_LABEL_ALIGN_RIGHT);
            lv_obj_set_size(setting_date_label[0][i], SETTING_LABEL_W - 40, SETTING_LABEL_H);
            lv_obj_align(setting_date_label[0][i], NULL, LV_ALIGN_IN_TOP_RIGHT, -30, y);

            y += SETTING_LABEL_H + SETTING_LABEL_GAP * 2;
            if (i == 2)
                y += SETTING_LABEL_H + SETTING_LABEL_GAP * 2;
        }

        setting_date_refresh();
    }
}

void setting_date_deinit(void)
{
    if (setting_date)
    {
        for (int i = 0; i < 5; i++)
        {
            lv_obj_clean_style_list(setting_date_label[0][i], LV_OBJ_PART_MAIN);
            lv_obj_clean_style_list(setting_date_label[1][i], LV_OBJ_PART_MAIN);
        }
        lv_obj_clean_style_list(setting_date, LV_OBJ_PART_MAIN);

        lv_obj_del(setting_date);
        setting_date = NULL;
    }
}
