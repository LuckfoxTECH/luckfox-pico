/******************************************************************************
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author              Notes
 * 2019-08-05     mark.huang && xyp   first implementation
 *
 ******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "osi/task.h"
#include "osi/timer.h"
#include "rk_log.h"

#include "rk_bt.h"
#include "bt_app_core.h"
#include "rk_bt_main.h"
#include "rk_bt_device.h"
#include "rk_a2dp_api.h"
#include "rk_avrc_api.h"
#include "rk_gap_bt_api.h"
#include "btHwControl.h"

#define BT_AV_TAG               "BT_AV"

#ifdef BT_CHIP_AP6212
extern bt_hw_control_t ap6212_hw_control;
bt_hw_control_t *p_hci_if_a2d_source = &ap6212_hw_control;
#else
//koala
extern bt_hw_control_t koala_hw_control;
bt_hw_control_t *p_hci_if_a2d_source = &koala_hw_control;
#endif

/* event for handler "bt_av_hdl_stack_up */
enum {
    BT_APP_EVT_STACK_UP = 0,
};

/* A2DP global state */
enum {
    APP_AV_STATE_IDLE,
    APP_AV_STATE_DISCOVERING,
    APP_AV_STATE_DISCOVERED,
    APP_AV_STATE_UNCONNECTED,
    APP_AV_STATE_CONNECTING,
    APP_AV_STATE_CONNECTED,
    APP_AV_STATE_DISCONNECTING,
};

/* sub states of APP_AV_STATE_CONNECTED */
enum {
    APP_AV_MEDIA_STATE_IDLE,
    APP_AV_MEDIA_STATE_STARTING,
    APP_AV_MEDIA_STATE_STARTED,
    APP_AV_MEDIA_STATE_STOPPING,
};

#define BT_APP_HEART_BEAT_EVT                (0xff00)

/// handler for bluetooth stack enabled events
static void bt_av_hdl_stack_evt(uint16_t event, void *p_param);

/// callback function for A2DP source
static void bt_app_a2d_cb(rk_a2d_cb_event_t event, rk_a2d_cb_param_t *param);

/// callback function for A2DP source audio data stream
static int32_t bt_app_a2d_data_cb(uint8_t *data, int32_t len);

static void a2d_app_heart_beat(void *arg);

/// A2DP application state machine
static void bt_app_av_sm_hdlr(uint16_t event, void *param);

/* A2DP application state machine handler for each state */
static void bt_app_av_state_unconnected(uint16_t event, void *param);
static void bt_app_av_state_connecting(uint16_t event, void *param);
static void bt_app_av_state_connected(uint16_t event, void *param);
static void bt_app_av_state_disconnecting(uint16_t event, void *param);

static rk_bd_addr_t s_peer_bda = {0};
static uint8_t s_peer_bdname[RK_BT_GAP_MAX_BDNAME_LEN + 1];
static int s_a2d_state = APP_AV_STATE_IDLE;
static int s_media_state = APP_AV_MEDIA_STATE_IDLE;
static int s_intv_cnt = 0;
static int s_connecting_intv = 0;
static uint32_t s_pkt_cnt = 0;

static osi_timer_t s_tmr;

static char *bda2str(rk_bd_addr_t bda, char *str, size_t size)
{
    if (bda == NULL || str == NULL || size < 18) {
        return NULL;
    }

    uint8_t *p = bda;
    sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
            p[0], p[1], p[2], p[3], p[4], p[5]);
    return str;
}

void _a2dp_source(void *arg)
{
    rk_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

    if (rk_bt_controller_init(&bt_cfg, p_hci_if_a2d_source) != OSI_EOK) {
        RK_LOGE(BT_AV_TAG, "%s initialize controller failed\n", __func__);
        return;
    }

    if (rk_bt_controller_enable(RK_BT_MODE_CLASSIC_BT) != OSI_EOK) {
        RK_LOGE(BT_AV_TAG, "%s enable controller failed\n", __func__);
        return;
    }

    if (rk_bluedroid_init() != OSI_EOK) {
        RK_LOGE(BT_AV_TAG, "%s initialize bluedroid failed\n", __func__);
        return;
    }

    if (rk_bluedroid_enable() != OSI_EOK) {
        RK_LOGE(BT_AV_TAG, "%s enable bluedroid failed\n", __func__);
        return;
    }

    /* create application task */
    bt_app_task_start_up();

    /* Bluetooth device name, connection mode and profile set up */
    bt_app_work_dispatch(bt_av_hdl_stack_evt, BT_APP_EVT_STACK_UP, NULL, 0, NULL);

#ifdef CONFIG_BT_SSP_ENABLED
    /* Set default parameters for Secure Simple Pairing */
    rk_bt_sp_param_t param_type = RK_BT_SP_IOCAP_MODE;
    rk_bt_io_cap_t iocap = RK_BT_IO_CAP_IO;
    rk_bt_gap_set_security_param(param_type, &iocap, sizeof(uint8_t));
#endif

    /*
     * Set default parameters for Legacy Pairing
     * Use variable pin, input pin code when pairing
     */
    rk_bt_pin_type_t pin_type = RK_BT_PIN_TYPE_VARIABLE;
    rk_bt_pin_code_t pin_code;
    rk_bt_gap_set_pin(pin_type, 0, pin_code);
}

static bool get_name_from_eir(uint8_t *eir, uint8_t *bdname, uint8_t *bdname_len)
{
    uint8_t *rmt_bdname = NULL;
    uint8_t rmt_bdname_len = 0;

    if (!eir) {
        return false;
    }

    rmt_bdname = rk_bt_gap_resolve_eir_data(eir, RK_BT_EIR_TYPE_CMPL_LOCAL_NAME, &rmt_bdname_len);
    if (!rmt_bdname) {
        rmt_bdname = rk_bt_gap_resolve_eir_data(eir, RK_BT_EIR_TYPE_SHORT_LOCAL_NAME, &rmt_bdname_len);
    }

    if (rmt_bdname) {
        if (rmt_bdname_len > RK_BT_GAP_MAX_BDNAME_LEN) {
            rmt_bdname_len = RK_BT_GAP_MAX_BDNAME_LEN;
        }

        if (bdname) {
            memcpy(bdname, rmt_bdname, rmt_bdname_len);
            bdname[rmt_bdname_len] = '\0';
        }
        if (bdname_len) {
            *bdname_len = rmt_bdname_len;
        }
        return true;
    }

    return false;
}

static void filter_inquiry_scan_result(rk_bt_gap_cb_param_t *param)
{
    char bda_str[18];
    uint32_t cod = 0;
    int32_t rssi = -129; /* invalid value */
    uint8_t *eir = NULL;
    rk_bt_gap_dev_prop_t *p;

    RK_LOGI(BT_AV_TAG, "Scanned device: %s", bda2str(param->disc_res.bda, bda_str, 18));
    for (int i = 0; i < param->disc_res.num_prop; i++) {
        p = param->disc_res.prop + i;
        switch (p->type) {
        case RK_BT_GAP_DEV_PROP_COD:
            cod = *(uint32_t *)(p->val);
            RK_LOGI(BT_AV_TAG, "--Class of Device: 0x%x", cod);
            break;
        case RK_BT_GAP_DEV_PROP_RSSI:
            rssi = *(int8_t *)(p->val);
            RK_LOGI(BT_AV_TAG, "--RSSI: %d", rssi);
            break;
        case RK_BT_GAP_DEV_PROP_EIR:
            eir = (uint8_t *)(p->val);
            break;
        case RK_BT_GAP_DEV_PROP_BDNAME:
        default:
            break;
        }
    }

    /* search for device with MAJOR service class as "rendering" in COD */
    if (!rk_bt_gap_is_valid_cod(cod) ||
            !(rk_bt_gap_get_cod_srvc(cod) & RK_BT_COD_SRVC_RENDERING)) {
        return;
    }

    /* search for device named "RK_SPEAKER" in its extended inqury response */
    if (eir) {
        get_name_from_eir(eir, s_peer_bdname, NULL);
        if (strcmp((char *)s_peer_bdname, "RK_SPEAKER") != 0) {
            return;
        }

        RK_LOGI(BT_AV_TAG, "Found a target device, address %s, name %s", bda_str, s_peer_bdname);
        s_a2d_state = APP_AV_STATE_DISCOVERED;
        memcpy(s_peer_bda, param->disc_res.bda, RK_BD_ADDR_LEN);
        RK_LOGI(BT_AV_TAG, "Cancel device discovery ...");
        rk_bt_gap_cancel_discovery();
    }
}


void bt_app_gap_cb(rk_bt_gap_cb_event_t event, rk_bt_gap_cb_param_t *param)
{
    switch (event) {
    case RK_BT_GAP_DISC_RES_EVT: {
        filter_inquiry_scan_result(param);
        break;
    }
    case RK_BT_GAP_DISC_STATE_CHANGED_EVT: {
        if (param->disc_st_chg.state == RK_BT_GAP_DISCOVERY_STOPPED) {
            if (s_a2d_state == APP_AV_STATE_DISCOVERED) {
                s_a2d_state = APP_AV_STATE_CONNECTING;
                RK_LOGI(BT_AV_TAG, "Device discovery stopped.");
                RK_LOGI(BT_AV_TAG, "a2dp connecting to peer: %s", s_peer_bdname);
                rk_a2d_source_connect(s_peer_bda);
            } else {
                // not discovered, continue to discover
                RK_LOGI(BT_AV_TAG, "Device discovery failed, continue to discover...");
                rk_bt_gap_start_discovery(RK_BT_INQ_MODE_GENERAL_INQUIRY, 10, 0);
            }
        } else if (param->disc_st_chg.state == RK_BT_GAP_DISCOVERY_STARTED) {
            RK_LOGI(BT_AV_TAG, "Discovery started.");
        }
        break;
    }
    case RK_BT_GAP_RMT_SRVCS_EVT:
    case RK_BT_GAP_RMT_SRVC_REC_EVT:
        break;
    case RK_BT_GAP_AUTH_CMPL_EVT: {
        if (param->auth_cmpl.stat == RK_BT_STATUS_SUCCESS) {
            RK_LOGI(BT_AV_TAG, "authentication success: %s", param->auth_cmpl.device_name);
            rk_log_buffer_hex(BT_AV_TAG, param->auth_cmpl.bda, RK_BD_ADDR_LEN);
        } else {
            RK_LOGE(BT_AV_TAG, "authentication failed, status:%d", param->auth_cmpl.stat);
        }
        break;
    }
    case RK_BT_GAP_PIN_REQ_EVT: {
        RK_LOGI(BT_AV_TAG, "RK_BT_GAP_PIN_REQ_EVT min_16_digit:%d", param->pin_req.min_16_digit);
        if (param->pin_req.min_16_digit) {
            RK_LOGI(BT_AV_TAG, "Input pin code: 0000 0000 0000 0000");
            rk_bt_pin_code_t pin_code = {0};
            rk_bt_gap_pin_reply(param->pin_req.bda, true, 16, pin_code);
        } else {
            RK_LOGI(BT_AV_TAG, "Input pin code: 1234");
            rk_bt_pin_code_t pin_code;
            pin_code[0] = '1';
            pin_code[1] = '2';
            pin_code[2] = '3';
            pin_code[3] = '4';
            rk_bt_gap_pin_reply(param->pin_req.bda, true, 4, pin_code);
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

    default: {
        RK_LOGI(BT_AV_TAG, "event: %d", event);
        break;
    }
    }
    return;
}

static void bt_av_hdl_stack_evt(uint16_t event, void *p_param)
{
    RK_LOGD(BT_AV_TAG, "%s evt %d", __func__, event);
    switch (event) {
    case BT_APP_EVT_STACK_UP: {
        /* set up device name */
        char *dev_name = "RK_A2DP_SRC";
        rk_bt_dev_set_device_name(dev_name);

        /* register GAP callback function */
        rk_bt_gap_register_callback(bt_app_gap_cb);

        /* initialize A2DP source */
        rk_a2d_register_callback(&bt_app_a2d_cb);
        rk_a2d_source_register_data_callback(bt_app_a2d_data_cb);
        rk_a2d_source_init();

        /* set discoverable and connectable mode */
        rk_bt_gap_set_scan_mode(RK_BT_CONNECTABLE, RK_BT_GENERAL_DISCOVERABLE);

        /* start device discovery */
        RK_LOGI(BT_AV_TAG, "Starting device discovery...");
        s_a2d_state = APP_AV_STATE_DISCOVERING;
        rk_bt_gap_start_discovery(RK_BT_INQ_MODE_GENERAL_INQUIRY, 10, 0);

        /* create and start heart beat timer */
        do {
            s_tmr = osi_timer_create("connTmr", a2d_app_heart_beat, NULL, 10000, true);
            osi_timer_start(s_tmr);
        } while (0);
        break;
    }
    default:
        RK_LOGE(BT_AV_TAG, "%s unhandled evt %d", __func__, event);
        break;
    }
}

static void bt_app_a2d_cb(rk_a2d_cb_event_t event, rk_a2d_cb_param_t *param)
{
    bt_app_work_dispatch(bt_app_av_sm_hdlr, event, param, sizeof(rk_a2d_cb_param_t), NULL);
}

static int32_t bt_app_a2d_data_cb(uint8_t *data, int32_t len)
{
    if (len < 0 || data == NULL) {
        return 0;
    }

    // generate random sequence
    int val = rand() % (1 << 16);
    for (int i = 0; i < (len >> 1); i++) {
        data[(i << 1)] = val & 0xff;
        data[(i << 1) + 1] = (val >> 8) & 0xff;
    }

    return len;
}

static void a2d_app_heart_beat(void *arg)
{
    bt_app_work_dispatch(bt_app_av_sm_hdlr, BT_APP_HEART_BEAT_EVT, NULL, 0, NULL);
}

static void bt_app_av_sm_hdlr(uint16_t event, void *param)
{
    RK_LOGI(BT_AV_TAG, "%s state %d, evt 0x%x", __func__, s_a2d_state, event);
    switch (s_a2d_state) {
    case APP_AV_STATE_DISCOVERING:
    case APP_AV_STATE_DISCOVERED:
        break;
    case APP_AV_STATE_UNCONNECTED:
        bt_app_av_state_unconnected(event, param);
        break;
    case APP_AV_STATE_CONNECTING:
        bt_app_av_state_connecting(event, param);
        break;
    case APP_AV_STATE_CONNECTED:
        bt_app_av_state_connected(event, param);
        break;
    case APP_AV_STATE_DISCONNECTING:
        bt_app_av_state_disconnecting(event, param);
        break;
    default:
        RK_LOGE(BT_AV_TAG, "%s invalid state %d", __func__, s_a2d_state);
        break;
    }
}

static void bt_app_av_state_unconnected(uint16_t event, void *param)
{
    switch (event) {
    case RK_A2D_CONNECTION_STATE_EVT:
    case RK_A2D_AUDIO_STATE_EVT:
    case RK_A2D_AUDIO_CFG_EVT:
    case RK_A2D_MEDIA_CTRL_ACK_EVT:
        break;
    case BT_APP_HEART_BEAT_EVT: {
        uint8_t *p = s_peer_bda;
        RK_LOGI(BT_AV_TAG, "a2dp connecting to peer: %02x:%02x:%02x:%02x:%02x:%02x",
                p[0], p[1], p[2], p[3], p[4], p[5]);
        rk_a2d_source_connect(s_peer_bda);
        s_a2d_state = APP_AV_STATE_CONNECTING;
        s_connecting_intv = 0;
        break;
    }
    default:
        RK_LOGE(BT_AV_TAG, "%s unhandled evt %d", __func__, event);
        break;
    }
}

static void bt_app_av_state_connecting(uint16_t event, void *param)
{
    rk_a2d_cb_param_t *a2d = NULL;
    switch (event) {
    case RK_A2D_CONNECTION_STATE_EVT: {
        a2d = (rk_a2d_cb_param_t *)(param);
        if (a2d->conn_stat.state == RK_A2D_CONNECTION_STATE_CONNECTED) {
            RK_LOGI(BT_AV_TAG, "a2dp connected");
            s_a2d_state =  APP_AV_STATE_CONNECTED;
            s_media_state = APP_AV_MEDIA_STATE_IDLE;
            rk_bt_gap_set_scan_mode(RK_BT_NON_CONNECTABLE, RK_BT_NON_DISCOVERABLE);
        } else if (a2d->conn_stat.state == RK_A2D_CONNECTION_STATE_DISCONNECTED) {
            s_a2d_state =  APP_AV_STATE_UNCONNECTED;
        }
        break;
    }
    case RK_A2D_AUDIO_STATE_EVT:
    case RK_A2D_AUDIO_CFG_EVT:
    case RK_A2D_MEDIA_CTRL_ACK_EVT:
        break;
    case BT_APP_HEART_BEAT_EVT:
        if (++s_connecting_intv >= 2) {
            s_a2d_state = APP_AV_STATE_UNCONNECTED;
            s_connecting_intv = 0;
        }
        break;
    default:
        RK_LOGE(BT_AV_TAG, "%s unhandled evt %d", __func__, event);
        break;
    }
}

static void bt_app_av_media_proc(uint16_t event, void *param)
{
    rk_a2d_cb_param_t *a2d = NULL;
    switch (s_media_state) {
    case APP_AV_MEDIA_STATE_IDLE: {
        if (event == BT_APP_HEART_BEAT_EVT) {
            RK_LOGI(BT_AV_TAG, "a2dp media ready checking ...");
            rk_a2d_media_ctrl(RK_A2D_MEDIA_CTRL_CHECK_SRC_RDY);
        } else if (event == RK_A2D_MEDIA_CTRL_ACK_EVT) {
            a2d = (rk_a2d_cb_param_t *)(param);
            if (a2d->media_ctrl_stat.cmd == RK_A2D_MEDIA_CTRL_CHECK_SRC_RDY &&
                    a2d->media_ctrl_stat.status == RK_A2D_MEDIA_CTRL_ACK_SUCCESS) {
                RK_LOGI(BT_AV_TAG, "a2dp media ready, starting ...");
                rk_a2d_media_ctrl(RK_A2D_MEDIA_CTRL_START);
                s_media_state = APP_AV_MEDIA_STATE_STARTING;
            }
        }
        break;
    }
    case APP_AV_MEDIA_STATE_STARTING: {
        if (event == RK_A2D_MEDIA_CTRL_ACK_EVT) {
            a2d = (rk_a2d_cb_param_t *)(param);
            if (a2d->media_ctrl_stat.cmd == RK_A2D_MEDIA_CTRL_START &&
                    a2d->media_ctrl_stat.status == RK_A2D_MEDIA_CTRL_ACK_SUCCESS) {
                RK_LOGI(BT_AV_TAG, "a2dp media start successfully.");
                s_intv_cnt = 0;
                s_media_state = APP_AV_MEDIA_STATE_STARTED;
            } else {
                // not started succesfully, transfer to idle state
                RK_LOGI(BT_AV_TAG, "a2dp media start failed.");
                s_media_state = APP_AV_MEDIA_STATE_IDLE;
            }
        }
        break;
    }
    case APP_AV_MEDIA_STATE_STARTED: {
        if (event == BT_APP_HEART_BEAT_EVT) {
            if (++s_intv_cnt >= 10) {
                RK_LOGI(BT_AV_TAG, "a2dp media stopping...");
                rk_a2d_media_ctrl(RK_A2D_MEDIA_CTRL_STOP);
                s_media_state = APP_AV_MEDIA_STATE_STOPPING;
                s_intv_cnt = 0;
            }
        }
        break;
    }
    case APP_AV_MEDIA_STATE_STOPPING: {
        if (event == RK_A2D_MEDIA_CTRL_ACK_EVT) {
            a2d = (rk_a2d_cb_param_t *)(param);
            if (a2d->media_ctrl_stat.cmd == RK_A2D_MEDIA_CTRL_STOP &&
                    a2d->media_ctrl_stat.status == RK_A2D_MEDIA_CTRL_ACK_SUCCESS) {
                RK_LOGI(BT_AV_TAG, "a2dp media stopped successfully, disconnecting...");
                s_media_state = APP_AV_MEDIA_STATE_IDLE;
                rk_a2d_source_disconnect(s_peer_bda);
                s_a2d_state = APP_AV_STATE_DISCONNECTING;
            } else {
                RK_LOGI(BT_AV_TAG, "a2dp media stopping...");
                rk_a2d_media_ctrl(RK_A2D_MEDIA_CTRL_STOP);
            }
        }
        break;
    }
    }
}

static void bt_app_av_state_connected(uint16_t event, void *param)
{
    rk_a2d_cb_param_t *a2d = NULL;
    switch (event) {
    case RK_A2D_CONNECTION_STATE_EVT: {
        a2d = (rk_a2d_cb_param_t *)(param);
        if (a2d->conn_stat.state == RK_A2D_CONNECTION_STATE_DISCONNECTED) {
            RK_LOGI(BT_AV_TAG, "a2dp disconnected");
            s_a2d_state = APP_AV_STATE_UNCONNECTED;
            rk_bt_gap_set_scan_mode(RK_BT_CONNECTABLE, RK_BT_GENERAL_DISCOVERABLE);
        }
        break;
    }
    case RK_A2D_AUDIO_STATE_EVT: {
        a2d = (rk_a2d_cb_param_t *)(param);
        if (RK_A2D_AUDIO_STATE_STARTED == a2d->audio_stat.state) {
            s_pkt_cnt = 0;
        }
        break;
    }
    case RK_A2D_AUDIO_CFG_EVT:
        // not suppposed to occur for A2DP source
        break;
    case RK_A2D_MEDIA_CTRL_ACK_EVT:
    case BT_APP_HEART_BEAT_EVT: {
        bt_app_av_media_proc(event, param);
        break;
    }
    default:
        RK_LOGE(BT_AV_TAG, "%s unhandled evt %d", __func__, event);
        break;
    }
}

static void bt_app_av_state_disconnecting(uint16_t event, void *param)
{
    rk_a2d_cb_param_t *a2d = NULL;
    switch (event) {
    case RK_A2D_CONNECTION_STATE_EVT: {
        a2d = (rk_a2d_cb_param_t *)(param);
        if (a2d->conn_stat.state == RK_A2D_CONNECTION_STATE_DISCONNECTED) {
            RK_LOGI(BT_AV_TAG, "a2dp disconnected");
            s_a2d_state =  APP_AV_STATE_UNCONNECTED;
            rk_bt_gap_set_scan_mode(RK_BT_CONNECTABLE, RK_BT_GENERAL_DISCOVERABLE);
        }
        break;
    }
    case RK_A2D_AUDIO_STATE_EVT:
    case RK_A2D_AUDIO_CFG_EVT:
    case RK_A2D_MEDIA_CTRL_ACK_EVT:
    case BT_APP_HEART_BEAT_EVT:
        break;
    default:
        RK_LOGE(BT_AV_TAG, "%s unhandled evt %d", __func__, event);
        break;
    }
}
static rt_thread_t tid1;
void app_main_a2dp_source()
{
    rt_thread_delay(100);
    tid1 = rt_thread_create("_a2dp_source",
                            _a2dp_source, RT_NULL,
                            2048,
                            25, 10);

    /* 如果获得线程控制块，启动这个线程 */
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

}

MSH_CMD_EXPORT(app_main_a2dp_source, a2dp source app test);

