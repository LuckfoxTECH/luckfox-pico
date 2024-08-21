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

#include "setting_page.h"
#include "setting_backlight.h"
#include "setting_rec_seglen.h"

#define DBG_SECTION_NAME    "SETPAGE"
#define DBG_LEVEL           DBG_INFO
#include "rtdbg.h"

enum
{
    SETTING_DELETE,
    SETTING_PLAYMODE,
    SETTING_RECORD,
    SETTING_DATA,
    SETTING_LANGUAGE,
    SETTING_BACKLIGHT,
    SETTING_FORMAT,
    SETTING_RESTORE,
    SETTING_NUM,
};

static void format_disk(void *arg);
static void restore_factory(void *arg);
static void unlink_file(void *arg);

static int g_pPage = MENU_NONE_PAGE;
static rk_lv_menulist_t *ml = NULL;

static const char title[RECPEN_APP_LANG_MAX][MAX_LABEL_LEN] =
{
    "设置",
    "Setting"
};

static const rk_lv_ml_element_t elements[SETTING_NUM] =
{
    {{"删除文件",       "Delete File"},     unlink_file},
    {{"播放模式",       "Playback Mode"},   load_setting,   (void *)MENU_SETTING_PLAYMODE_PAGE},
    {{"录音设置",       "Record Setting"},  load_setting,   (void *)MENU_SETTING_RECORD_PAGE},
    {{"日期",           "Date"},            load_setting,   (void *)MENU_SETTING_DATE_PAGE},
    {{"语言",           "Language"},        load_setting,   (void *)MENU_SETTING_LANGUAGE_PAGE},
    {{"背光设置",       "Backlight"},       load_setting,   (void *)MENU_SETTING_BACKLIGHT_PAGE},
    {{"格式化",         "Format"},          format_disk},
    {{"恢复出厂设置",   "Restore Setting"}, restore_factory},
};

void load_setting(void *arg)
{
    display_page((int)arg);
}

static void do_format_disk(int ret)
{
    if (ret == DIALOG_BOX_CONFIRM && rt_device_find("sd0"))
    {
        dfs_mkfs("elm", "sd0");
        mkdir(RECORD_DIR_PATH, 0);
        memset(&g_app_info.file, 0x0, sizeof(struct recpen_file_info));
        sprintf(g_app_info.file.cur_file_name, "No recording file");
        g_app_info.player_total_time = 0;
        g_app_info.player_cur_time = 0;
    }
}

static void format_disk(void *arg)
{
    dialog_box_init(elements[SETTING_FORMAT].str[g_app_info.language],
                    do_format_disk);
}

static void do_restore_factory(int ret)
{
    if (ret == DIALOG_BOX_CONFIRM)
    {
        LOG_I("Restore factory setting");
        g_app_info.language = RECPEN_APP_LANG_CN;
        g_app_info.playmode = PLAYMODE_ONCE;
        g_app_info.brightness = BRIGHTNESS_HW_MAX;
        reset_seglen();
        reset_bl_timeout();
        save_app_info();
        display_page(MENU_SETTING_PAGE);
        /* Reset brightness after page change */
        if (g_app_info.bl_device)
        {
            rt_device_control(g_app_info.bl_device,
                              RTGRAPHIC_CTRL_RECT_UPDATE,
                              &g_app_info.brightness);
        }
    }
}

static void restore_factory(void *arg)
{
    dialog_box_init(elements[SETTING_RESTORE].str[g_app_info.language],
                    do_restore_factory);
}

static void do_unlink_file(int ret)
{
    if (ret == DIALOG_BOX_CONFIRM)
    {
        LOG_I("Remove %s", g_app_info.file.cur_file_name);
        remove_file(RECORD_DIR_PATH, g_app_info.file.cur_file_name);
    }
}

static void unlink_file(void *arg)
{
    if (g_app_info.file.total_file > 0)
    {
        char str[256];
        sprintf(str, "%s \n\"%s\"?%c",
                elements[SETTING_DELETE].str[g_app_info.language],
                g_app_info.file.cur_file_name,
                '\0');
        dialog_box_init(str, do_unlink_file);
    }
    else
    {
        toast_create("No file\0", 1500);
    }
}

void setting_page_key(size_t type, size_t value)
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

void setting_page_init(int p_page)
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

void setting_page_deinit(void)
{
    if (ml)
    {
        rk_lv_menulist_destroy(ml);
        ml = NULL;
    }
}
