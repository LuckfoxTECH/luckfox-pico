/*
 * Copyright: Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * License: SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-20 10:31:42
 * @Company: Rockchip
 * @LastEditTime: 2020-06-29 14:20:00
 * @LastEditors: Do not edit
 * @Description:
 */

#include "main_page.h"
#include "bt_test_page.h"
#include "board_wifibt.h"

#include "wiced_bt_dev.h"
#include "wiced_bt_dev.h"
#include "wiced_bt_cfg.h"

#define DBG_SECTION_NAME    "BT_PAGE"
#define DBG_LEVEL           DBG_LOG
#include "rtdbg.h"

static lv_obj_t *bt_page = NULL;
static lv_obj_t *divide_line = NULL;
static lv_obj_t *company_label = NULL;
static lv_obj_t *item_label = NULL;
static lv_obj_t *pass_btn = NULL;
static lv_obj_t *fail_btn = NULL;
static rt_bool_t bt_test_ok = RT_FALSE;

#ifdef RT_CYPRESS_BT
#include "btm_api.h"
/*
 *  Handle Inquiry result callback from teh stack, format and send event over UART
 */
 static int g_scan_cnt;
void hci_control_inquiry_result_cback( wiced_bt_dev_inquiry_scan_result_t *p_inquiry_result, uint8_t *p_eir_data )
{
    int       i;
    uint8_t   len;
    uint8_t   tx_buf[300];
    uint8_t   *p = tx_buf;

    if ( p_inquiry_result == NULL )
    {
        //WPRINT_APP_INFO(( "inquiry err\n"));
    }
    else
    {
        rt_kprintf("inquiry result: %02x:%02x:%02x:%02x:%02x:%02x\n", \
                        p_inquiry_result->remote_bd_addr[0], \
                        p_inquiry_result->remote_bd_addr[1], \
                        p_inquiry_result->remote_bd_addr[2], \
                        p_inquiry_result->remote_bd_addr[3], \
                        p_inquiry_result->remote_bd_addr[4], \
                        p_inquiry_result->remote_bd_addr[5]
                        );
        g_scan_cnt++;

        for ( i = 0; i < 6; i++ )
            *p++ = p_inquiry_result->remote_bd_addr[5 - i];
        for ( i = 0; i < 3; i++ )
            *p++ = p_inquiry_result->dev_class[2 - i];
        *p++ = p_inquiry_result->rssi;

        // currently callback does not pass the data of the adv data, need to go through the data
        // zero len in the LTV means that there is no more data
        while ( ( len = *p_eir_data ) != 0 )
        {
            // In the HCI event all parameters should fit into 255 bytes
            if ( p + len + 1 > tx_buf + 255 )
            {
                rt_kprintf("Bad data\n");
                break;
            }
            for ( i = 0; i < len + 1; i++ )
                *p++ = *p_eir_data++;
        }
    }
}

/*
 *  Handle Inquiry command received over UART
 */
void hci_control_inquiry( uint8_t enable )
{
    wiced_result_t           result;
    wiced_bt_dev_inq_parms_t params;

    if ( enable )
    {
        rt_kprintf("inquiry enable: %d\n", enable );
        BTM_SetInquiryMode (BTM_INQ_RESULT_EXTENDED);

        memset( &params, 0, sizeof( params ) );

        params.mode             = BTM_GENERAL_INQUIRY;
        params.duration         = 5;
        params.filter_cond_type = BTM_CLR_INQUIRY_FILTER;

        result = wiced_bt_start_inquiry( &params, &hci_control_inquiry_result_cback );
        rt_kprintf("inquiry started:%d\n", result );
    }
    else
    {
        result = wiced_bt_cancel_inquiry( );
        rt_kprintf("cancel inquiry:%d\n", result );
    }
}
#endif

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
static int draw_bt_scan_result(void)
{
    result_label = lv_label_create(bt_page, NULL);
    lv_label_set_recolor(result_label, true);
    lv_label_set_long_mode(result_label, LV_LABEL_LONG_EXPAND);
    lv_obj_align(result_label, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 45);
    lv_label_set_text(result_label, "BT test result:");
    lv_obj_set_style(result_label, &lv_style_pretty);

    return 0;
}

#if 0//def RT_CYPRESS_BT
extern void init_bt_gpio(void);
extern uint32_t bt_rst(void);
extern int open_comm(const char *name);
extern void config_comm(int fd, int speed_baud_rate, char parity, int data_bits, int stop_bits);
extern int flush_comm(int fd);
extern bool snd_rcv(int fd, const uint8_t *inbuf, int inlen, uint8_t * outbuf, int * outlen);
extern bool download_fw(int fd);
int fd;
uint8_t rxBuf[1024];

static void hci_rst()
{
    int cnt;
    const uint8_t hci_reset[] = { 0x01, 0x03, 0x0c, 0x00 };
    snd_rcv(fd, hci_reset, sizeof(hci_reset), rxBuf, &cnt);
}

static void dlhcd()
{
    int cnt;

    rk_bt_init_gpio();
    rk_bt_rst();

    if ((fd = open_comm(BT_UART_NAME)) == -1)
    {
        rt_kprintf("open failed\n");
        return;
    }

    config_comm(fd, 115200, 'n', 8, 1);
    flush_comm(fd);

    rt_thread_mdelay(100);

    hci_rst();

    rt_thread_mdelay(100);
    const uint8_t miniDriver[] = {0x01, 0x2e, 0xfc, 0/*length*/};
    snd_rcv(fd, miniDriver, sizeof(miniDriver), rxBuf, &cnt);

    rt_thread_mdelay(100);
    rt_kprintf("download fw...\n");
    download_fw(fd);

    rt_thread_mdelay(100);
    const uint8_t lunchRam[] = {0x01, 0x4E, 0xFC, 0x04, 0xFF, 0xFF, 0xFF, 0xFF};
    snd_rcv(fd, lunchRam, sizeof(lunchRam), rxBuf, &cnt);

    rt_thread_mdelay(1000);
    hci_rst();
    rt_kprintf("download fw complete.\n");
}
#endif

RT_UNUSED static int bt_device_scan(void)
{
#ifdef RT_CYPRESS_BT
    char dev_name[33] = {0};
    int index = 0, num = 0;
    static lv_style_t style1;
    lv_style_copy(&style1, &lv_style_pretty);
    style1.text.color = LV_COLOR_GREEN;

    lv_obj_t * dev_label = NULL;
    for (index = 0; index < num; index ++)
    {
        // snprintf(dev_name, 32, "%s", &scan_result->info[index].ssid.val[0]);

        dev_label = lv_label_create(bt_page, NULL);
        lv_label_set_recolor(dev_label, true);
        lv_label_set_long_mode(dev_label, LV_LABEL_LONG_EXPAND);
        lv_obj_align(dev_label, NULL, LV_ALIGN_IN_LEFT_MID, 0, (index - 2) * 20);
        lv_label_set_text(dev_label, dev_name);
        lv_obj_set_style(dev_label, &style1);
    }
#else
    LOG_D("Unsupport bt tyte.");
#endif
     return 0;
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
            memcpy (g_result_info[ITEM_BT], result, strlen(result));
        }
        else
        {
            LOG_D("Clicked Fail button");
            result = "NG";
            memcpy (g_result_info[ITEM_BT], result, strlen(result));
        }

        display_pcba_page(MENU_MAIN_PAGE);
    }
}

static int draw_result_btn(void)
{
    lv_obj_t * label;

    /* Create PASS button */
    pass_btn = lv_btn_create(bt_page, NULL);
    lv_obj_set_event_cb(pass_btn, btn_event_handler);
    lv_obj_set_size(pass_btn, BTN_W, BTN_H);
    lv_obj_align(pass_btn, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
    lv_btn_set_style(pass_btn, LV_BTN_STYLE_PR,  &style_tv_btn_pr);
    lv_btn_set_style(pass_btn, LV_BTN_STYLE_REL,  &style_tv_btn_rel);

    /* Create FAIL button */
    fail_btn = lv_btn_create(bt_page, NULL);
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

static rt_thread_t g_bt_task;

static void bt_task(void *arg)
{
    char bt_rst[32];
#ifdef RT_CYPRESS_BT
//    dlhcd();

    hci_control_inquiry(1);

    bt_test_ok = RT_TRUE;

    static lv_style_t style1;
    lv_style_copy(&style1, &lv_style_pretty);
    style1.text.color = LV_COLOR_GREEN;

    lv_obj_t * rst_label = NULL;

    rst_label = lv_label_create(bt_page, NULL);
    lv_label_set_recolor(rst_label, true);
    lv_label_set_long_mode(rst_label, LV_LABEL_LONG_EXPAND);
    lv_obj_align(rst_label, NULL, LV_ALIGN_CENTER, -40, 0);

    lv_obj_set_style(rst_label, &style1);

    int old_val = 0;
    old_val = g_scan_cnt;

    while(1)
    {
        rt_thread_delay(200);
        if (old_val != g_scan_cnt)
        {
            old_val = g_scan_cnt;
            rt_snprintf(bt_rst, sizeof(bt_rst), "scanned: %d", g_scan_cnt);
            lv_label_set_text(rst_label, bt_rst);
        }
    }
#else
    //do nothing...
#endif
//    g_bt_task = NULL;
}

rt_err_t bt_startup_init(void)
{
    rt_thread_t tid;
    tid = rt_thread_create("bt_start",
                           bt_task,
                           RT_NULL,
                           1024,
                           15,
                           10);

    if (tid != RT_NULL)
    {
        LOG_D("Create bt start task OK");
        rt_thread_startup(tid);
        g_bt_task = tid;
        return RT_EOK;
    }

    LOG_E("Create bt start task failed");
    RT_ASSERT(0);

    return -RT_ERROR;
}

/**
 * @description: pcba key test page init
 * @param {int page}
 * @return: none
 */
void bt_page_init(int p_page)
{
    short int y = 0;
    char str_test_item[64];
    g_scan_cnt = 0;

    if (!bt_page)
    {
        bt_page = lv_obj_create(lv_scr_act(), NULL);

        lv_obj_set_size(bt_page, SCREEN_W, SCREEN_H);
        lv_obj_set_style(bt_page, &label_style_font16);
        lv_obj_align(bt_page, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
        lv_obj_invalidate(bt_page);

        divide_line = lv_line_create(bt_page, NULL);

        company_label = lv_label_create(bt_page, NULL);
        lv_label_set_long_mode(company_label, LV_LABEL_LONG_SROLL_CIRC);
        lv_label_set_align(company_label, LV_LABEL_ALIGN_CENTER);
        lv_obj_set_size(company_label, LABEL_W, LABEL_H);
        lv_obj_align(company_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);
        lv_label_set_text(company_label, "Rockchip Co.,Ltd.");
        lv_obj_set_style(company_label, &label_style_font16);

        item_label = lv_label_create(bt_page, NULL);
        y = LABEL_H;
        lv_label_set_long_mode(item_label, LV_LABEL_LONG_SROLL_CIRC);
        lv_label_set_align(item_label, LV_LABEL_ALIGN_CENTER);
        lv_label_set_recolor(item_label, true);
        lv_obj_set_size(item_label, LABEL_W, LABEL_H);
        lv_obj_align(item_label, NULL, LV_ALIGN_IN_TOP_MID, 0, y);
        rt_snprintf(str_test_item, sizeof(str_test_item), "#00ff00 %s#", pcba_test_item_str[ITEM_BT]);
        lv_label_set_text(item_label, str_test_item);
        lv_obj_set_style(item_label, &label_style_font16);
    }

    draw_divide_line();

    draw_result_btn();

    draw_bt_scan_result();
    //bt_device_scan();

    /* create bt start task to scan.*/
    bt_startup_init();
}

/**
 * @description:
 * @param {type}
 * @return:
 */
void bt_page_deinit(void)
{
    if (bt_page)
    {
        /* this function will delete children too */
        lv_obj_del(bt_page);
        bt_page = NULL;
    }

    if (g_bt_task)
        rt_thread_delete(g_bt_task);
    g_bt_task = NULL;
}

void bt_page_key(size_t type, size_t value)
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

void bt_page_refresh(size_t type, size_t value)
{

}