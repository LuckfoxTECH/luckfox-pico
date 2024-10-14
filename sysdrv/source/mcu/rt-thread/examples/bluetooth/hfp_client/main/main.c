/******************************************************************************
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author              Notes
 * 2019-05-10     mark.huang && xyp   first implementation
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "rk_log.h"

#include "rk_bt.h"
#include "bt_app_core.h"
#include "bt_app_av.h"
#include "rk_bt_main.h"
#include "rk_bt_device.h"
#include "rk_gap_bt_api.h"
#include "rk_hf_client_api.h"
#include "rk_a2dp_api.h"
#include "rk_avrc_api.h"
#include "btHwControl.h"
//#include "driver/i2s.h"

/* event for handler "bt_av_hdl_stack_up */
enum
{
    BT_APP_EVT_STACK_UP = 0,
};

/* handler for bluetooth stack enabled events */
static void bt_av_hdl_stack_evt(uint16_t event, void *p_param);
#ifdef BT_CHIP_AP6212
extern bt_hw_control_t ap6212_hw_control;
bt_hw_control_t *p_hci_if_hfp = &ap6212_hw_control;
#else
//koala
extern bt_hw_control_t koala_hw_control;
bt_hw_control_t *p_hci_if_hfp = &koala_hw_control;
#endif

void _hfp_client()
{
    osi_err_t err;

    rk_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    if ((err = rk_bt_controller_init(&bt_cfg, p_hci_if_hfp)) != OSI_EOK)
    {
        RK_LOGE(BT_AV_TAG, "%s initialize controller failed: %s\n", __func__, osi_err_to_name(err));
        return;
    }

    if ((err = rk_bt_controller_enable(RK_BT_MODE_CLASSIC_BT)) != OSI_EOK)
    {
        RK_LOGE(BT_AV_TAG, "%s enable controller failed: %s\n", __func__, osi_err_to_name(err));
        return;
    }

    if ((err = rk_bluedroid_init()) != OSI_EOK)
    {
        RK_LOGE(BT_AV_TAG, "%s initialize bluedroid failed: %s\n", __func__, osi_err_to_name(err));
        return;
    }

    if ((err = rk_bluedroid_enable()) != OSI_EOK)
    {
        RK_LOGE(BT_AV_TAG, "%s enable bluedroid failed: %s\n", __func__, osi_err_to_name(err));
        return;
    }

    /* create application task */
    bt_app_task_start_up();

    /* Bluetooth device name, connection mode and profile set up */
    bt_app_work_dispatch(bt_av_hdl_stack_evt, BT_APP_EVT_STACK_UP, NULL, 0, NULL);
    /*
     * Set default parameters for Legacy Pairing
     * Use fixed pin code
     */
    rk_bt_pin_type_t pin_type = RK_BT_PIN_TYPE_FIXED;
    rk_bt_pin_code_t pin_code;
    pin_code[0] = '1';
    pin_code[1] = '2';
    pin_code[2] = '3';
    pin_code[3] = '4';
    rk_bt_gap_set_pin(pin_type, 4, pin_code);

}

void bt_app_gap_cb(rk_bt_gap_cb_event_t event, rk_bt_gap_cb_param_t *param)
{
    switch (event)
    {
    case RK_BT_GAP_AUTH_CMPL_EVT:
    {
        if (param->auth_cmpl.stat == RK_BT_STATUS_SUCCESS)
        {
            RK_LOGI(BT_AV_TAG, "authentication success: %s", param->auth_cmpl.device_name);
            rk_log_buffer_hex(BT_AV_TAG, param->auth_cmpl.bda, RK_BD_ADDR_LEN);
        }
        else
        {
            RK_LOGE(BT_AV_TAG, "authentication failed, status:%d", param->auth_cmpl.stat);
        }
        break;
    }

#ifdef CONFIG_BT_SSP_ENABLED
    case RK_BT_GAP_CFM_REQ_EVT:
        RK_LOGI(BT_AV_TAG, "RK_BT_GAP_CFM_REQ_EVT Please compare the numeric value: %d", param->cfm_req.num_val);
        rk_bt_gap_ssp_confirm_reply(param->cfm_req.bda, true);
        break;
    case RK_BT_GAP_KEY_NOTIF_EVT:
        RK_LOGI(BT_AV_TAG, "RK_BT_GAP_KEY_NOTIF_EVT passkey:%d", param->key_notif.passkey);
        break;
    case RK_BT_GAP_KEY_REQ_EVT:
        RK_LOGI(BT_AV_TAG, "RK_BT_GAP_KEY_REQ_EVT Please enter passkey!");
        break;
#endif

    default:
    {
        RK_LOGI(BT_AV_TAG, "event: %d", event);
        break;
    }
    }
    return;
}
static void bt_av_hdl_stack_evt(uint16_t event, void *p_param)
{
    RK_LOGD(BT_AV_TAG, "%s evt %d", __func__, event);
    switch (event)
    {
    case BT_APP_EVT_STACK_UP:
    {
        /* set up device name */
        char *dev_name = "RK_HFP_CLIENT";
        rk_bt_dev_set_device_name(dev_name);

        rk_bt_gap_register_callback(bt_app_gap_cb);

        /* initialize AVRCP controller */
        rk_avrc_ct_init();
        rk_avrc_ct_register_callback(bt_app_rc_ct_cb);
        /* initialize AVRCP target */
        assert(rk_avrc_tg_init() == OSI_EOK);
        rk_avrc_tg_register_callback(bt_app_rc_tg_cb);

        rk_avrc_rn_evt_cap_mask_t evt_set = {0};
        rk_avrc_rn_evt_bit_mask_operation(RK_AVRC_BIT_MASK_OP_SET, &evt_set, RK_AVRC_RN_VOLUME_CHANGE);
        assert(rk_avrc_tg_set_rn_evt_cap(&evt_set) == OSI_EOK);

        /* initialize A2DP sink */
        rk_a2d_register_callback(bt_app_a2d_cb);
        rk_a2d_sink_register_data_callback(bt_app_a2d_data_cb);
        rk_a2d_sink_init();

        /* initialize HFP client */
        rk_hf_client_register_callback(bt_app_hfp_client_cb);
        rk_hf_client_register_data_callback(bt_app_hfp_client_incoming_data_cb_t, bt_app_hfp_client_outgoing_data_cb_t);
        rk_hf_client_init();

        /* set discoverable and connectable mode, wait to be connected */
        rk_bt_gap_set_scan_mode(RK_BT_CONNECTABLE, RK_BT_GENERAL_DISCOVERABLE);
        break;
    }
    default:
        RK_LOGE(BT_AV_TAG, "%s unhandled evt %d", __func__, event);
        break;
    }
}

static rt_thread_t tid1;
void app_main_hfp_sink()
{
    rt_thread_delay(100);
    tid1 = rt_thread_create("_hfp_client",
                            _hfp_client, RT_NULL,
                            2048,
                            25, 10);

    /* 如果获得线程控制块，启动这个线程 */
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

}

MSH_CMD_EXPORT(app_main_hfp_sink, hfp client app test);
