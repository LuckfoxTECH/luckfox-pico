/*
 * Copyright: Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * License: SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-20 10:31:11
 * @Company: Rockchip
 * @LastEditTime: 2020-06-29 11:51:00
 * @LastEditors: Do not edit
 * @Description:
 */

#include "main_page.h"
#include "wifi_test_page.h"
#include "wlan_dev.h"

#define DBG_SECTION_NAME    "WIFI_PAGE"
#define DBG_LEVEL           DBG_LOG
#include "rtdbg.h"

static lv_obj_t *wifi_page = NULL;
static lv_obj_t *divide_line = NULL;
static lv_obj_t *company_label = NULL;
static lv_obj_t *item_label = NULL;
static lv_obj_t *pass_btn = NULL;
static lv_obj_t *fail_btn = NULL;

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

static lv_obj_t * result_label = NULL;
static int draw_wifi_result(void)
{
    result_label = lv_label_create(wifi_page, NULL);
    lv_label_set_recolor(result_label, true);
    lv_label_set_long_mode(result_label, LV_LABEL_LONG_EXPAND);
    // lv_obj_align(result_label, NULL, LV_ALIGN_IN_LEFT_MID, 0, -20);
    lv_obj_set_pos(result_label, 0, 45);
    lv_label_set_text(result_label, "WiFi scan result:");
    lv_obj_set_style(result_label, &lv_style_pretty);

    return 0;
}

static int wifi_set_mode_scan(void)
{
#ifdef RT_CYPRESS_WIFI
    rt_wlan_set_mode("wlan0", RT_WLAN_STATION);
    rt_thread_delay(200);

    char ssid[33] = {0};
    struct rt_wlan_scan_result *scan_result = RT_NULL;
    /* scan ap info */
    int index, num;
    char *security;
    scan_result = rt_wlan_scan_sync();
    if (scan_result)
    {
        num = scan_result->num;
        rt_kprintf("             SSID                      MAC            security    rssi chn Mbps\n");
        rt_kprintf("------------------------------- -----------------  -------------- ---- --- ----\n");
        for (index = 0; index < num; index ++)
        {
            rt_kprintf("%-32.32s", &scan_result->info[index].ssid.val[0]);
            rt_kprintf("%02x:%02x:%02x:%02x:%02x:%02x  ",
                       scan_result->info[index].bssid[0],
                       scan_result->info[index].bssid[1],
                       scan_result->info[index].bssid[2],
                       scan_result->info[index].bssid[3],
                       scan_result->info[index].bssid[4],
                       scan_result->info[index].bssid[5]
                      );
            switch (scan_result->info[index].security)
            {
            case SECURITY_OPEN:
                security = "OPEN";
                break;
            case SECURITY_WEP_PSK:
                security = "WEP_PSK";
                break;
            case SECURITY_WEP_SHARED:
                security = "WEP_SHARED";
                break;
            case SECURITY_WPA_TKIP_PSK:
                security = "WPA_TKIP_PSK";
                break;
            case SECURITY_WPA_AES_PSK:
                security = "WPA_AES_PSK";
                break;
            case SECURITY_WPA2_AES_PSK:
                security = "WPA2_AES_PSK";
                break;
            case SECURITY_WPA2_TKIP_PSK:
                security = "WPA2_TKIP_PSK";
                break;
            case SECURITY_WPA2_MIXED_PSK:
                security = "WPA2_MIXED_PSK";
                break;
            case SECURITY_WPS_OPEN:
                security = "WPS_OPEN";
                break;
            case SECURITY_WPS_SECURE:
                security = "WPS_SECURE";
                break;
            default:
                security = "UNKNOWN";
                break;
            }
            rt_kprintf("%-14.14s ", security);
            rt_kprintf("%-4d ", scan_result->info[index].rssi);
            rt_kprintf("%3d ", scan_result->info[index].channel);
            rt_kprintf("%4d\n", scan_result->info[index].datarate / 1000000);
        }
    }
    else
    {
        rt_kprintf("wifi scan result is null\n");
        return 0;
    }

    static lv_style_t style1;
    lv_style_copy(&style1, &lv_style_pretty);
    style1.text.color = LV_COLOR_GREEN;

    lv_obj_t * ssid_label = NULL;

    if (num > 6)
        num = 6;    //limit to 5 SSID to show.

    for (index = 0; index < num; index ++)
    {
        snprintf(ssid, 32, "%s", &scan_result->info[index].ssid.val[0]);

        ssid_label = lv_label_create(wifi_page, NULL);
        lv_label_set_recolor(ssid_label, true);
        lv_label_set_long_mode(ssid_label, LV_LABEL_LONG_EXPAND);
        lv_obj_align(ssid_label, NULL, LV_ALIGN_IN_LEFT_MID, 0, (index - 2) * 20);
        lv_label_set_text(ssid_label, ssid);
        lv_obj_set_style(ssid_label, &style1);
    }
    return 0;

#else
    return 0;
#endif
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
            memcpy (g_result_info[ITEM_WIFI], result, strlen(result));
        }
        else
        {
            LOG_D("Clicked Fail button");
            result = "NG";
            memcpy (g_result_info[ITEM_WIFI], result, strlen(result));
        }

        display_pcba_page(MENU_MAIN_PAGE);
    }
}

static int draw_result_btn(void)
{
    lv_obj_t * label;

    /* Create FAIL button */
    pass_btn = lv_btn_create(wifi_page, NULL);
    lv_obj_set_event_cb(pass_btn, btn_event_handler);
    lv_obj_set_size(pass_btn, BTN_W, BTN_H);
    lv_obj_align(pass_btn, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
    lv_btn_set_style(pass_btn, LV_BTN_STYLE_PR,  &style_tv_btn_pr);
    lv_btn_set_style(pass_btn, LV_BTN_STYLE_REL,  &style_tv_btn_rel);

    /* Create FAIL button */
    fail_btn = lv_btn_create(wifi_page, NULL);
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

/**
 * @description: pcba key test page init
 * @param {int page}
 * @return: none
 */
void wifi_page_init(int p_page)
{
    short int y = 0;
    char str_test_item[64];

    if (!wifi_page)
    {
        wifi_page = lv_obj_create(lv_scr_act(), NULL);

        lv_obj_set_size(wifi_page, SCREEN_W, SCREEN_H);
        lv_obj_set_style(wifi_page, &label_style_font16);
        lv_obj_align(wifi_page, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
        lv_obj_invalidate(wifi_page);

        divide_line = lv_line_create(wifi_page, NULL);

        company_label = lv_label_create(wifi_page, NULL);
        lv_label_set_long_mode(company_label, LV_LABEL_LONG_SROLL_CIRC);
        lv_label_set_align(company_label, LV_LABEL_ALIGN_CENTER);
        lv_obj_set_size(company_label, LABEL_W, LABEL_H);
        lv_obj_align(company_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);
        lv_label_set_text(company_label, "Rockchip Co.,Ltd.");
        lv_obj_set_style(company_label, &label_style_font16);

        item_label = lv_label_create(wifi_page, NULL);
        y = LABEL_H;
        lv_label_set_long_mode(item_label, LV_LABEL_LONG_SROLL_CIRC);
        lv_label_set_align(item_label, LV_LABEL_ALIGN_CENTER);
        lv_label_set_recolor(item_label, true);
        lv_obj_set_size(item_label, LABEL_W, LABEL_H);
        lv_obj_align(item_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);
        rt_snprintf(str_test_item, sizeof(str_test_item), "#00ff00 %s#", pcba_test_item_str[ITEM_WIFI]);
        lv_label_set_text(item_label, str_test_item);
        lv_obj_set_style(item_label, &label_style_font16);
    }

    draw_divide_line();

    draw_wifi_result();
    wifi_set_mode_scan();

    draw_result_btn();
}

/**
 * @description:
 * @param {type}
 * @return:
 */
void wifi_page_deinit(void)
{
    if (wifi_page)
    {
        /* this function will delete children too */
        lv_obj_del(wifi_page);
        wifi_page = NULL;
    }
}

void wifi_page_key(size_t type, size_t value)
{
    if (type == TYPE_KEY_PRESS)
    {
        switch (value)
        {
        case KEY_UP:
            lv_btn_set_state(pass_btn, LV_BTN_STATE_PR);
            btn_event_handler(pass_btn, LV_EVENT_CLICKED);
            break;

        case KEY_DOWN:
            lv_btn_set_state(fail_btn, LV_BTN_STATE_PR);
            btn_event_handler(fail_btn, LV_EVENT_CLICKED);
            break;

        default:
            break;
        }
    }
}

void wifi_page_refresh(size_t type, size_t value)
{
}