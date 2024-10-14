/*
 * Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-12 10:33:21
 * @Company: Rockchip
 * @LastEditTime: 2020-06-29 11:57:51
 * @LastEditors: Do not edit
 * @Description:
 */

#include "pcba_app.h"
#include "pcba_disp.h"

#if defined(RT_USING_PCBA_TEST)

#include "main_page.h"
#include "screen_test_page.h"
#include "key_test_page.h"
#include "record_test_page.h"
#include "phospk_test_page.h"
#include "emmc_test_page.h"
#include "rtc_test_page.h"
#include "temperate_test_page.h"
#include "battery_test_page.h"
#include "wifi_test_page.h"
#include "bt_test_page.h"
#include "result_page.h"

#include "result_page.h"

#define DBG_SECTION_NAME    "PCBA_DISPLAY"
#define DBG_LEVEL           DBG_LOG
#include "rtdbg.h"

/* disp_event define */
#define EVENT_REFR_UPDATE           (0x01UL << 0)
#define EVENT_REFR_DONE             (0x01UL << 1)

struct pcba_disp_data
{
    rt_display_data_t disp;
    rt_event_t  disp_event;
    rt_uint32_t cmd;
};
static struct pcba_disp_data *g_pcba_disp_data = RT_NULL;

lv_style_t page_pcba_style;
lv_style_t label_style_font20;
lv_style_t label_style_font20_hl;
lv_style_t label_style_font20_sel;
lv_style_t label_style_font16;


struct lvgl_page_func pcba_page_func[MENU_MAX_PAGE] =
{
    {"MENU_NO_PAGE",            NULL, NULL, NULL, NULL},
    {"MENU_MAIN_PAGE",          index_page_init, index_page_deinit, index_page_refresh, index_page_key},
    {"MENU_SCREEN_TEST_PAGE",   screen_page_init, screen_page_deinit, screen_page_refresh, screen_page_key},
    {"MENU_KEY_TEST_PAGE",      key_page_init, key_page_deinit, key_page_refresh, key_page_key},
    {"MENU_RECORD_TEST_PAGE",   record_page_init, record_page_deinit, record_page_refresh, record_page_key},
    {"MENU_PHONE_SPEAK_TEST_PAGE", phospk_page_init, phospk_page_deinit , phospk_page_refresh, phospk_page_key},
    {"MENU_EMMC_TEST_PAGE",     emmc_page_init, emmc_page_deinit , emmc_page_refresh, emmc_page_key},
    {"MENU_BATTERY_TEST_PAGE",  batt_page_init, batt_page_deinit , batt_page_refresh, batt_page_key},
    {"MENU_TEMPERATE_TEST_PAGE",temp_page_init, temp_page_deinit , temp_page_refresh, temp_page_key},
    {"MENU_RTC_TEST_PAGE",      rtc_page_init, rtc_page_deinit , rtc_page_refresh, rtc_page_key},
    {"MENU_WIFI_TEST_PAGE",     wifi_page_init, wifi_page_deinit , wifi_page_refresh, wifi_page_key},
    {"MENU_BT_TEST_PAGE",       bt_page_init, bt_page_deinit , bt_page_refresh, bt_page_key},
    {"MENU_RESULT_PAGE",        result_page_init, result_page_deinit , result_page_refresh, result_page_key},
};

static void styles_init(void)
{
    lv_style_copy(&page_pcba_style, &lv_style_scr);
    page_pcba_style.body.main_color = LV_COLOR_BLACK;
    page_pcba_style.body.grad_color = LV_COLOR_BLACK;
    page_pcba_style.body.padding.top = LABEL_Y_GAP / 2;
    page_pcba_style.body.padding.bottom = LABEL_Y_GAP / 2;
    page_pcba_style.text.color = LV_COLOR_WHITE;
    page_pcba_style.text.font = &lv_font_20;

    lv_style_copy(&label_style_font20, &lv_style_scr);
    label_style_font20.body.padding.top = LABEL_Y_GAP / 2;
    label_style_font20.body.padding.bottom = LABEL_Y_GAP / 2;
    label_style_font20.text.color = LV_COLOR_WHITE;
    label_style_font20.text.font = &lv_font_20;

    lv_style_copy(&label_style_font20_hl, &label_style_font20);
    label_style_font20_hl.text.color = LV_COLOR_MAKE(0x11, 0xC1, 0xB7);

    lv_style_copy(&label_style_font20_sel, &label_style_font20);
    label_style_font20_sel.body.main_color = LV_COLOR_MAKE(0x33, 0x62, 0x94);
    label_style_font20_sel.body.grad_color = LV_COLOR_MAKE(0x33, 0x62, 0x94);

    lv_style_copy(&label_style_font16, &lv_style_scr);
    label_style_font16.text.color = LV_COLOR_WHITE;
    label_style_font16.text.font = &lv_font_16;
}

rt_err_t display_pcba_page(int p_page)
{
    struct pcba_app_msg send_msg;
    int ret;

    if (p_page != MENU_NONE_PAGE)
    {
        send_msg.type = TYPE_SHOW_MENU;
        send_msg.value = p_page;
        ret = rt_mq_send(g_pcba.display_mq, (void *)&send_msg, sizeof(struct pcba_app_msg));
        if (ret == -RT_EFULL)
        {
            LOG_W("Msg queue full\n");
            return -RT_ERROR;
        }
    }

    return RT_EOK;
}

static rt_err_t lcd_refr_done(void)
{
    return rt_event_send(g_pcba_disp_data->disp_event, EVENT_REFR_DONE);
}

static rt_err_t lcd_refr_request(void)
{
    rt_err_t ret;
    struct rt_display_mq_t disp_mq;
    rt_memset(&disp_mq, 0, sizeof(struct rt_display_mq_t));

    //request refresh display data to Pannel
    disp_mq.disp_finish = lcd_refr_done;
    disp_mq.cfgsta |= (0x01 << 0);
    ret = rt_mq_send(g_pcba_disp_data->disp->disp_mq, &disp_mq, sizeof(struct rt_display_mq_t));
    RT_ASSERT(ret == RT_EOK);

    //wait refresh done
    rt_uint32_t event;
    ret = rt_event_recv(g_pcba_disp_data->disp_event, EVENT_REFR_DONE,
                        RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                        RT_WAITING_FOREVER, &event);
    RT_ASSERT(ret == RT_EOK);

    return RT_EOK;
}

static void display_task(void *arg)
{
    rt_err_t ret;
    struct pcba_app_msg rcv_msg;
    struct pcba_disp_data *disp_data;
    int backlight = 0;

    g_pcba_disp_data = disp_data = (struct pcba_disp_data *)rt_malloc(sizeof(struct pcba_disp_data));
    RT_ASSERT(disp_data != RT_NULL);
    rt_memset((void *)disp_data, 0, sizeof(struct pcba_disp_data));

    disp_data->disp_event = rt_event_create("display_event", RT_IPC_FLAG_FIFO);
    RT_ASSERT(disp_data->disp_event != RT_NULL);

    disp_data->disp = rt_display_get_disp();
    RT_ASSERT(disp_data->disp != RT_NULL);

    /* init littlevGL */
    ret = littlevgl2rtt_init(DISPLAY_DEVICE_NAME);
    RT_ASSERT(ret == RT_EOK);

    styles_init();
    g_pcba.now_page = MENU_MAIN_PAGE;
    pcba_page_func[g_pcba.now_page].init(MENU_NONE_PAGE);
    LOG_D("run here %s: %d", __func__,__LINE__);

    while (1)
    {
        // toast_refresh();
        lv_task_handler();
        if (backlight != g_pcba.backlight)
        {
            if (g_pcba.backlight)
            {
                rt_device_control(disp_data->disp->device, RTGRAPHIC_CTRL_POWERON, NULL);
                lcd_refr_request();
                if (g_pcba.bl_device)
                    rt_device_control(g_pcba.bl_device, RTGRAPHIC_CTRL_POWERON, NULL);
            }
            else
            {
                if (g_pcba.bl_device)
                    rt_device_control(g_pcba.bl_device, RTGRAPHIC_CTRL_POWEROFF, NULL);
                rt_device_control(disp_data->disp->device, RTGRAPHIC_CTRL_POWEROFF, NULL);
            }
            backlight = g_pcba.backlight;
        }
        else if (g_pcba.backlight)
        {
            lcd_refr_request();
        }

        // lvgl_img_dsc_remove_all();
        ret = rt_mq_recv(g_pcba.display_mq, (void *)&rcv_msg, sizeof(struct pcba_app_msg), RT_WAITING_FOREVER);
        if (ret == RT_EOK)
        {
            if (rcv_msg.type != TYPE_REFRESH)
            {
                LOG_D("Disp rev msg:%d %d", rcv_msg.type, rcv_msg.value);
                LOG_D("Current page : %d", g_pcba.now_page);
            }
            switch (rcv_msg.type)
            {
            //#if 1
            case TYPE_SHOW_MENU:
                if (rcv_msg.value > MENU_MAX_PAGE)
                    continue;

                LOG_I("Now page %s -> %s", pcba_page_func[g_pcba.now_page].name, pcba_page_func[rcv_msg.value].name);
                if (!pcba_page_func[g_pcba.now_page].deinit || !pcba_page_func[rcv_msg.value].init)
                {
                    LOG_W("Page func null:%p -> %p, ignore this msg", pcba_page_func[g_pcba.now_page].deinit, pcba_page_func[rcv_msg.value].init);
                    break;
                }
                pcba_page_func[g_pcba.now_page].deinit();
                pcba_page_func[rcv_msg.value].init(g_pcba.now_page);
                g_pcba.now_page = rcv_msg.value;
                LOG_D("change to New page : %d", g_pcba.now_page);
                break;

            case TYPE_KEY_PRESS:
                // if (g_pcba.bl_timer)
                //     rt_timer_start(g_pcba.bl_timer);
                if (g_pcba.backlight == 0)
                {
                    /* Key is invalid if backlight off */
                    g_pcba.backlight = 1;
                    break;
                }

                if (pcba_page_func[g_pcba.now_page].key)
                    pcba_page_func[g_pcba.now_page].key(rcv_msg.type, rcv_msg.value);
                break;

            default:
                // if (rcv_msg.type == TYPE_REFRESH && rcv_msg.value == REFRESH_DISP_TICK)
                //     break;
                if (pcba_page_func[g_pcba.now_page].refresh)
                    pcba_page_func[g_pcba.now_page].refresh(rcv_msg.type, rcv_msg.value);
                break;
            }
        }
    }

    littlevgl2rtt_deinit();

    rt_event_delete(disp_data->disp_event);

    rt_free(disp_data);
}

rt_err_t pcba_display_init(void)
{
    rt_thread_t tid;

    tid = rt_thread_create("pcba_disp",
                           display_task,
                           RT_NULL,
                           PCBA_APP_DISP_TASK_STACK_SIZE,
                           PCBA_APP_DISP_TASK_PRIORITY,
                           10);

    if (tid != RT_NULL)
    {
        LOG_D("Create pcba_disp OK");
        rt_thread_startup(tid);
        return RT_EOK;
    }

    LOG_E("Create pcba_disp failed");
    RT_ASSERT(0);

    return -RT_ERROR;
}


#endif
