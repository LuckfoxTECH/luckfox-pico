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

#ifndef __BT_APP_AV_H__
#define __BT_APP_AV_H__

#include <stdint.h>
#include "rk_hf_client_api.h"
#include "rk_a2dp_api.h"
#include "rk_avrc_api.h"

#define BT_AV_TAG               "BT_AV"
#define BT_RC_TG_TAG            "RCTG"
#define BT_RC_CT_TAG            "RCCT"


/**
 * @brief     callback function for A2DP sink
 */
void bt_app_a2d_cb(rk_a2d_cb_event_t event, rk_a2d_cb_param_t *param);

/**
 * @brief     callback function for A2DP sink audio data stream
 */
void bt_app_a2d_data_cb(const uint8_t *data, uint32_t len);

/**
 * @brief     callback function for HFP Client
 */
void bt_app_hfp_client_cb(rk_hf_client_cb_event_t event, rk_hf_client_cb_param_t *param);

/**
 * @brief     callback function for HFP Client data in
 */
void bt_app_hfp_client_incoming_data_cb_t(const uint8_t *buf, uint32_t len);

/**
 * @brief     callback function for HFP Client data out
 */
uint32_t bt_app_hfp_client_outgoing_data_cb_t(uint8_t *buf, uint32_t len);

/**
 * @brief     callback function for AVRCP controller
 */
void bt_app_rc_ct_cb(rk_avrc_ct_cb_event_t event, rk_avrc_ct_cb_param_t *param);

/**
 * @brief     callback function for AVRCP target
 */
void bt_app_rc_tg_cb(rk_avrc_tg_cb_event_t event, rk_avrc_tg_cb_param_t *param);

#endif /* __BT_APP_AV_H__*/
