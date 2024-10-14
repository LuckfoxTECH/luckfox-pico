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

#include "setting_brightness.h"

#define DBG_SECTION_NAME    "SETBRT"
#define DBG_LEVEL           DBG_INFO
#include "rtdbg.h"

static lv_obj_t *setting_brightness;
static lv_obj_t *setting_brightness_label;
static lv_obj_t *setting_brightness_bar;
static int g_pPage = MENU_NONE_PAGE;
static int new_brightness;

static inline int brightness_remap_to_hw(int value)
{
    int sf_range;
    int hw_range;
    int brightness;

    if (value > BRIGHTNESS_MAX)
        value = BRIGHTNESS_MAX;
    if (value < BRIGHTNESS_MIN)
        value = BRIGHTNESS_MIN;
    sf_range = BRIGHTNESS_MAX - BRIGHTNESS_MIN;
    hw_range = BRIGHTNESS_HW_MAX - BRIGHTNESS_HW_MIN;
    brightness = ((float)(value - BRIGHTNESS_MIN) / sf_range * hw_range + BRIGHTNESS_HW_MIN);

    return brightness;
}

static inline int brightness_remap_to_sf(int value)
{
    int sf_range;
    int hw_range;
    int brightness;

    if (value > BRIGHTNESS_HW_MAX)
        value = BRIGHTNESS_HW_MAX;
    if (value < BRIGHTNESS_HW_MIN)
        value = BRIGHTNESS_HW_MIN;
    sf_range = BRIGHTNESS_MAX - BRIGHTNESS_MIN;
    hw_range = BRIGHTNESS_HW_MAX - BRIGHTNESS_HW_MIN;
    brightness = ((float)(value - BRIGHTNESS_HW_MIN) / hw_range * sf_range + BRIGHTNESS_MIN);

    return brightness;
}

static void setting_brightness_refresh(void)
{
    char str[32] = {0};

    snprintf(str, sizeof(str), "%d%c", new_brightness, '\0');
    lv_label_set_text(setting_brightness_label, str);
    lv_bar_set_value(setting_brightness_bar, new_brightness, LV_ANIM_OFF);
}

void setting_brightness_key(size_t type, size_t value)
{
    int brightness;

    if (type == TYPE_KEY_PRESS)
    {
        switch (value)
        {
        case KEY_DOWN:
            if (new_brightness < BRIGHTNESS_MAX)
            {
                new_brightness++;
                setting_brightness_refresh();
                /* preview backlight setting */
                brightness = brightness_remap_to_hw(new_brightness);
                if (g_app_info.bl_device)
                {
                    rt_device_control(g_app_info.bl_device,
                                      RTGRAPHIC_CTRL_RECT_UPDATE,
                                      &brightness);
                }
            }
            break;
        case KEY_UP:
            if (new_brightness > BRIGHTNESS_MIN)
            {
                new_brightness--;
                setting_brightness_refresh();
                /* preview backlight setting */
                brightness = brightness_remap_to_hw(new_brightness);
                if (g_app_info.bl_device)
                {
                    rt_device_control(g_app_info.bl_device,
                                      RTGRAPHIC_CTRL_RECT_UPDATE,
                                      &brightness);
                }
            }
            break;
        case KEY_CANCEL:
            if (display_page(g_pPage) == RT_EOK)
            {
                /* reset backlight setting */
                if (g_app_info.bl_device)
                {
                    rt_device_control(g_app_info.bl_device,
                                      RTGRAPHIC_CTRL_RECT_UPDATE,
                                      &g_app_info.brightness);
                }
                g_pPage = MENU_NONE_PAGE;
            }
            break;
        case KEY_OK:
            if (g_app_info.brightness != new_brightness)
            {
                /* save backlight setting */
                g_app_info.brightness = brightness_remap_to_hw(new_brightness);
                save_app_info();
            }
            if (display_page(g_pPage) == RT_EOK)
                g_pPage = MENU_NONE_PAGE;
            break;
        default:
            break;
        }
    }
}

void setting_brightness_init(int p_page)
{
    if (!setting_brightness)
    {
        new_brightness = brightness_remap_to_sf(g_app_info.brightness);
        if (g_pPage == MENU_NONE_PAGE)
            g_pPage = p_page;
        setting_brightness = lv_obj_create(lv_scr_act(), NULL);
        lv_obj_set_size(setting_brightness, SCREEN_W, SCREEN_H);
        lv_obj_add_style(setting_brightness, LV_OBJ_PART_MAIN, &page_common_style);
        lv_obj_align(setting_brightness, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

        setting_brightness_label = lv_label_create(setting_brightness, NULL);
        lv_label_set_long_mode(setting_brightness_label, LV_LABEL_LONG_BREAK);
        lv_label_set_align(setting_brightness_label, LV_LABEL_ALIGN_CENTER);
        lv_obj_set_size(setting_brightness_label, SETTING_LABEL_W, SETTING_LABEL_H);
        lv_obj_align(setting_brightness_label, NULL, LV_ALIGN_CENTER, 0, SETTING_LABEL_H);

        setting_brightness_bar = lv_bar_create(setting_brightness, NULL);
        lv_obj_set_size(setting_brightness_bar, SETTING_LABEL_W / 2, SETTING_LABEL_H);
        lv_obj_align(setting_brightness_bar, NULL, LV_ALIGN_CENTER, 0, 0);
        lv_bar_set_range(setting_brightness_bar, BRIGHTNESS_MIN, BRIGHTNESS_MAX);
        setting_brightness_refresh();
    }
}

void setting_brightness_deinit(void)
{
    if (setting_brightness)
    {
        lv_obj_clean_style_list(setting_brightness, LV_OBJ_PART_MAIN);

        lv_obj_del(setting_brightness);
        setting_brightness = NULL;
    }
}
