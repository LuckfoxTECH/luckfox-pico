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

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "rk_log.h"

#include "bt_app_core.h"
#include "bt_app_av.h"
#include "rk_bt_main.h"
#include "rk_bt_device.h"
#include "rk_gap_bt_api.h"
#include "rk_a2dp_api.h"
#include "rk_avrc_api.h"
#include "osi/mutex.h"
#include "osi/task.h"


//#include "driver/i2s.h"


// AVRCP used transaction label
#define APP_RC_CT_TL_GET_CAPS            (0)
#define APP_RC_CT_TL_GET_META_DATA       (1)
#define APP_RC_CT_TL_RN_TRACK_CHANGE     (2)
#define APP_RC_CT_TL_RN_PLAYBACK_CHANGE  (3)
#define APP_RC_CT_TL_RN_PLAY_POS_CHANGE  (4)

/* a2dp event handler */
static void bt_av_hdl_a2d_evt(uint16_t event, void *p_param);
/* avrc CT event handler */
static void bt_av_hdl_avrc_ct_evt(uint16_t event, void *p_param);
/* avrc TG event handler */
static void bt_av_hdl_avrc_tg_evt(uint16_t event, void *p_param);

static uint32_t s_pkt_cnt = 0;
static rk_a2d_audio_state_t s_audio_state = RK_A2D_AUDIO_STATE_STOPPED;
static const char *s_a2d_conn_state_str[] = {"Disconnected", "Connecting", "Connected", "Disconnecting"};
static const char *s_a2d_audio_state_str[] = {"Suspended", "Stopped", "Started"};
static rk_avrc_rn_evt_cap_mask_t s_avrc_peer_rn_cap;
static osi_mutex_t s_volume_lock;
static osi_task_t s_vcs_task_hdl = NULL;
static uint8_t s_volume = 0;
static bool s_volume_notify;

/* callback for A2DP sink */
void bt_app_a2d_cb(rk_a2d_cb_event_t event, rk_a2d_cb_param_t *param)
{
    switch (event)
    {
    case RK_A2D_CONNECTION_STATE_EVT:
    case RK_A2D_AUDIO_STATE_EVT:
    case RK_A2D_AUDIO_CFG_EVT:
    {
        bt_app_work_dispatch(bt_av_hdl_a2d_evt, event, param, sizeof(rk_a2d_cb_param_t), NULL);
        break;
    }
    default:
        RK_LOGE(BT_AV_TAG, "Invalid A2DP event: %d", event);
        break;
    }
}

void bt_app_a2d_data_cb(const uint8_t *data, uint32_t len)
{

#ifdef OSI_I2S
    size_t bytes_written;
    i2s_write(0, data, len, &bytes_written, portMAX_DELAY);
#endif
    if (++s_pkt_cnt % 100 == 0)
    {
        RK_LOGI(BT_AV_TAG, "Audio packet count %u", s_pkt_cnt);
    }
}

void bt_app_alloc_meta_buffer(rk_avrc_ct_cb_param_t *param)
{
    rk_avrc_ct_cb_param_t *rc = (rk_avrc_ct_cb_param_t *)(param);
    uint8_t *attr_text = (uint8_t *) malloc(rc->meta_rsp.attr_length + 1);
    memcpy(attr_text, rc->meta_rsp.attr_text, rc->meta_rsp.attr_length);
    attr_text[rc->meta_rsp.attr_length] = 0;

    rc->meta_rsp.attr_text = attr_text;
}

void bt_app_rc_ct_cb(rk_avrc_ct_cb_event_t event, rk_avrc_ct_cb_param_t *param)
{
    switch (event)
    {
    case RK_AVRC_CT_METADATA_RSP_EVT:
        bt_app_alloc_meta_buffer(param);
    /* fall through */
    case RK_AVRC_CT_CONNECTION_STATE_EVT:
    case RK_AVRC_CT_PASSTHROUGH_RSP_EVT:
    case RK_AVRC_CT_CHANGE_NOTIFY_EVT:
    case RK_AVRC_CT_REMOTE_FEATURES_EVT:
    case RK_AVRC_CT_GET_RN_CAPABILITIES_RSP_EVT:
    {
        bt_app_work_dispatch(bt_av_hdl_avrc_ct_evt, event, param, sizeof(rk_avrc_ct_cb_param_t), NULL);
        break;
    }
    default:
        RK_LOGE(BT_RC_CT_TAG, "Invalid AVRC event: %d", event);
        break;
    }
}

void bt_app_rc_tg_cb(rk_avrc_tg_cb_event_t event, rk_avrc_tg_cb_param_t *param)
{
    switch (event)
    {
    case RK_AVRC_TG_CONNECTION_STATE_EVT:
    case RK_AVRC_TG_REMOTE_FEATURES_EVT:
    case RK_AVRC_TG_PASSTHROUGH_CMD_EVT:
    case RK_AVRC_TG_SET_ABSOLUTE_VOLUME_CMD_EVT:
    case RK_AVRC_TG_REGISTER_NOTIFICATION_EVT:
        bt_app_work_dispatch(bt_av_hdl_avrc_tg_evt, event, param, sizeof(rk_avrc_tg_cb_param_t), NULL);
        break;
    default:
        RK_LOGE(BT_RC_TG_TAG, "Invalid AVRC event: %d", event);
        break;
    }
}

static void bt_av_hdl_a2d_evt(uint16_t event, void *p_param)
{
    RK_LOGD(BT_AV_TAG, "%s evt %d", __func__, event);
    rk_a2d_cb_param_t *a2d = NULL;
    switch (event)
    {
    case RK_A2D_CONNECTION_STATE_EVT:
    {
        a2d = (rk_a2d_cb_param_t *)(p_param);
        uint8_t *bda = a2d->conn_stat.remote_bda;
        RK_LOGI(BT_AV_TAG, "A2DP connection state: %s, [%02x:%02x:%02x:%02x:%02x:%02x]",
                s_a2d_conn_state_str[a2d->conn_stat.state], bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]);
        if (a2d->conn_stat.state == RK_A2D_CONNECTION_STATE_DISCONNECTED)
        {
            rk_bt_gap_set_scan_mode(RK_BT_CONNECTABLE, RK_BT_GENERAL_DISCOVERABLE);
        }
        else if (a2d->conn_stat.state == RK_A2D_CONNECTION_STATE_CONNECTED)
        {
            rk_bt_gap_set_scan_mode(RK_BT_NON_CONNECTABLE, RK_BT_NON_DISCOVERABLE);
        }
        break;
    }
    case RK_A2D_AUDIO_STATE_EVT:
    {
        a2d = (rk_a2d_cb_param_t *)(p_param);
        RK_LOGI(BT_AV_TAG, "A2DP audio state: %s", s_a2d_audio_state_str[a2d->audio_stat.state]);
        s_audio_state = a2d->audio_stat.state;
        if (RK_A2D_AUDIO_STATE_STARTED == a2d->audio_stat.state)
        {
            s_pkt_cnt = 0;
        }
        break;
    }
    case RK_A2D_AUDIO_CFG_EVT:
    {
        a2d = (rk_a2d_cb_param_t *)(p_param);
        RK_LOGI(BT_AV_TAG, "A2DP audio stream configuration, codec type %d", a2d->audio_cfg.mcc.type);
        // for now only SBC stream is supported
        if (a2d->audio_cfg.mcc.type == RK_A2D_MCT_SBC)
        {
            int sample_rate = 16000;
            char oct0 = a2d->audio_cfg.mcc.cie.sbc[0];
            if (oct0 & (0x01 << 6))
            {
                sample_rate = 32000;
            }
            else if (oct0 & (0x01 << 5))
            {
                sample_rate = 44100;
            }
            else if (oct0 & (0x01 << 4))
            {
                sample_rate = 48000;
            }
#ifdef OSI_I2S
            i2s_set_clk(0, sample_rate, 16, 2);
#endif
            RK_LOGI(BT_AV_TAG, "Configure audio player %x-%x-%x-%x",
                    a2d->audio_cfg.mcc.cie.sbc[0],
                    a2d->audio_cfg.mcc.cie.sbc[1],
                    a2d->audio_cfg.mcc.cie.sbc[2],
                    a2d->audio_cfg.mcc.cie.sbc[3]);
            RK_LOGI(BT_AV_TAG, "Audio player configured, sample rate=%d", sample_rate);
        }
        break;
    }
    default:
        RK_LOGE(BT_AV_TAG, "%s unhandled evt %d", __func__, event);
        break;
    }
}

static void bt_av_new_track(void)
{
    // request metadata
    uint8_t attr_mask = RK_AVRC_MD_ATTR_TITLE | RK_AVRC_MD_ATTR_ARTIST | RK_AVRC_MD_ATTR_ALBUM | RK_AVRC_MD_ATTR_GENRE;
    rk_avrc_ct_send_metadata_cmd(APP_RC_CT_TL_GET_META_DATA, attr_mask);

    // register notification if peer support the event_id
    if (rk_avrc_rn_evt_bit_mask_operation(RK_AVRC_BIT_MASK_OP_TEST, &s_avrc_peer_rn_cap,
                                          RK_AVRC_RN_TRACK_CHANGE))
    {
        rk_avrc_ct_send_register_notification_cmd(APP_RC_CT_TL_RN_TRACK_CHANGE, RK_AVRC_RN_TRACK_CHANGE, 0);
    }
}

static void bt_av_playback_changed(void)
{
    if (rk_avrc_rn_evt_bit_mask_operation(RK_AVRC_BIT_MASK_OP_TEST, &s_avrc_peer_rn_cap,
                                          RK_AVRC_RN_PLAY_STATUS_CHANGE))
    {
        rk_avrc_ct_send_register_notification_cmd(APP_RC_CT_TL_RN_PLAYBACK_CHANGE, RK_AVRC_RN_PLAY_STATUS_CHANGE, 0);
    }
}

static void bt_av_play_pos_changed(void)
{
    if (rk_avrc_rn_evt_bit_mask_operation(RK_AVRC_BIT_MASK_OP_TEST, &s_avrc_peer_rn_cap,
                                          RK_AVRC_RN_PLAY_POS_CHANGED))
    {
        rk_avrc_ct_send_register_notification_cmd(APP_RC_CT_TL_RN_PLAY_POS_CHANGE, RK_AVRC_RN_PLAY_POS_CHANGED, 10);
    }
}

void bt_av_notify_evt_handler(uint8_t event_id, rk_avrc_rn_param_t *event_parameter)
{
    switch (event_id)
    {
    case RK_AVRC_RN_TRACK_CHANGE:
        bt_av_new_track();
        break;
    case RK_AVRC_RN_PLAY_STATUS_CHANGE:
        RK_LOGI(BT_AV_TAG, "Playback status changed: 0x%x", event_parameter->playback);
        bt_av_playback_changed();
        break;
    case RK_AVRC_RN_PLAY_POS_CHANGED:
        RK_LOGI(BT_AV_TAG, "Play position changed: %d-ms", event_parameter->play_pos);
        bt_av_play_pos_changed();
        break;
    }
}

static void bt_av_hdl_avrc_ct_evt(uint16_t event, void *p_param)
{
    RK_LOGD(BT_RC_CT_TAG, "%s evt %d", __func__, event);
    rk_avrc_ct_cb_param_t *rc = (rk_avrc_ct_cb_param_t *)(p_param);
    switch (event)
    {
    case RK_AVRC_CT_CONNECTION_STATE_EVT:
    {
        uint8_t *bda = rc->conn_stat.remote_bda;
        RK_LOGI(BT_RC_CT_TAG, "AVRC conn_state evt: state %d, [%02x:%02x:%02x:%02x:%02x:%02x]",
                rc->conn_stat.connected, bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]);

        if (rc->conn_stat.connected)
        {
            // get remote supported event_ids of peer AVRCP Target
            rk_avrc_ct_send_get_rn_capabilities_cmd(APP_RC_CT_TL_GET_CAPS);
        }
        else
        {
            // clear peer notification capability record
            s_avrc_peer_rn_cap.bits = 0;
        }
        break;
    }
    case RK_AVRC_CT_PASSTHROUGH_RSP_EVT:
    {
        RK_LOGI(BT_RC_CT_TAG, "AVRC passthrough rsp: key_code 0x%x, key_state %d", rc->psth_rsp.key_code, rc->psth_rsp.key_state);
        break;
    }
    case RK_AVRC_CT_METADATA_RSP_EVT:
    {
        RK_LOGI(BT_RC_CT_TAG, "AVRC metadata rsp: attribute id 0x%x, %s", rc->meta_rsp.attr_id, rc->meta_rsp.attr_text);
        free(rc->meta_rsp.attr_text);
        break;
    }
    case RK_AVRC_CT_CHANGE_NOTIFY_EVT:
    {
        RK_LOGI(BT_RC_CT_TAG, "AVRC event notification: %d", rc->change_ntf.event_id);
        bt_av_notify_evt_handler(rc->change_ntf.event_id, &rc->change_ntf.event_parameter);
        break;
    }
    case RK_AVRC_CT_REMOTE_FEATURES_EVT:
    {
        RK_LOGI(BT_RC_CT_TAG, "AVRC remote features %x, TG features %x", rc->rmt_feats.feat_mask, rc->rmt_feats.tg_feat_flag);
        break;
    }
    case RK_AVRC_CT_GET_RN_CAPABILITIES_RSP_EVT:
    {
        RK_LOGI(BT_RC_CT_TAG, "remote rn_cap: count %d, bitmask 0x%x", rc->get_rn_caps_rsp.cap_count,
                rc->get_rn_caps_rsp.evt_set.bits);
        s_avrc_peer_rn_cap.bits = rc->get_rn_caps_rsp.evt_set.bits;
        bt_av_new_track();
        bt_av_playback_changed();
        bt_av_play_pos_changed();
        break;
    }
    default:
        RK_LOGE(BT_RC_CT_TAG, "%s unhandled evt %d", __func__, event);
        break;
    }
}

static void volume_set_by_controller(uint8_t volume)
{
    RK_LOGI(BT_RC_TG_TAG, "Volume is set by remote controller %d%%\n", (uint32_t)volume * 100 / 0x7f);
    osi_mutex_lock(&s_volume_lock, OSI_MAX_TIMEOUT);
    s_volume = volume;
    osi_mutex_unlock(&s_volume_lock);
}

static void volume_set_by_local_host(uint8_t volume)
{
    RK_LOGI(BT_RC_TG_TAG, "Volume is set locally to: %d%%", (uint32_t)volume * 100 / 0x7f);
    osi_mutex_lock(&s_volume_lock, OSI_MAX_TIMEOUT);
    s_volume = volume;
    osi_mutex_unlock(&s_volume_lock);

    if (s_volume_notify)
    {
        rk_avrc_rn_param_t rn_param;
        rn_param.volume = s_volume;
        rk_avrc_tg_send_rn_rsp(RK_AVRC_RN_VOLUME_CHANGE, RK_AVRC_RN_RSP_CHANGED, &rn_param);
        s_volume_notify = false;
    }
}

static void volume_change_simulation(void *arg)
{
    RK_LOGI(BT_RC_TG_TAG, "start volume change simulation");
    osi_mutex_new(&s_volume_lock);
    for (;;)
    {
        rt_thread_delay(10000);

        uint8_t volume = (s_volume + 5) & 0x7f;
        volume_set_by_local_host(volume);
    }
}

static void bt_av_hdl_avrc_tg_evt(uint16_t event, void *p_param)
{
    RK_LOGD(BT_RC_TG_TAG, "%s evt %d", __func__, event);
    rk_avrc_tg_cb_param_t *rc = (rk_avrc_tg_cb_param_t *)(p_param);
    switch (event)
    {
    case RK_AVRC_TG_CONNECTION_STATE_EVT:
    {
        uint8_t *bda = rc->conn_stat.remote_bda;
        RK_LOGI(BT_RC_TG_TAG, "AVRC conn_state evt: state %d, [%02x:%02x:%02x:%02x:%02x:%02x]",
                rc->conn_stat.connected, bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]);
        if (rc->conn_stat.connected)
        {
            // create task to simulate volume change
            osi_task_create(volume_change_simulation, "vcsT", 2048, NULL, 5, &s_vcs_task_hdl, 10);

        }
        else
        {
            osi_task_delete(&s_vcs_task_hdl);
            RK_LOGI(BT_RC_TG_TAG, "Stop volume change simulation");
        }
        break;
    }
    case RK_AVRC_TG_PASSTHROUGH_CMD_EVT:
    {
        RK_LOGI(BT_RC_TG_TAG, "AVRC passthrough cmd: key_code 0x%x, key_state %d", rc->psth_cmd.key_code, rc->psth_cmd.key_state);
        break;
    }
    case RK_AVRC_TG_SET_ABSOLUTE_VOLUME_CMD_EVT:
    {
        RK_LOGI(BT_RC_TG_TAG, "AVRC set absolute volume: %d%%", (int)rc->set_abs_vol.volume * 100 / 0x7f);
        volume_set_by_controller(rc->set_abs_vol.volume);
        break;
    }
    case RK_AVRC_TG_REGISTER_NOTIFICATION_EVT:
    {
        RK_LOGI(BT_RC_TG_TAG, "AVRC register event notification: %d, param: 0x%x", rc->reg_ntf.event_id, rc->reg_ntf.event_parameter);
        if (rc->reg_ntf.event_id == RK_AVRC_RN_VOLUME_CHANGE)
        {
            s_volume_notify = true;
            rk_avrc_rn_param_t rn_param;
            rn_param.volume = s_volume;
            rk_avrc_tg_send_rn_rsp(RK_AVRC_RN_VOLUME_CHANGE, RK_AVRC_RN_RSP_INTERIM, &rn_param);
        }
        break;
    }
    case RK_AVRC_TG_REMOTE_FEATURES_EVT:
    {
        RK_LOGI(BT_RC_TG_TAG, "AVRC remote features %x, CT features %x", rc->rmt_feats.feat_mask, rc->rmt_feats.ct_feat_flag);
        break;
    }
    default:
        RK_LOGE(BT_RC_TG_TAG, "%s unhandled evt %d", __func__, event);
        break;
    }
}
