/**
 * @file ffc_app.h
 * @brief Common process - wifi ffc
 * @version 0.1
 * @date 2020-05-13
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TUYA_FFC_APP_H__
#define __TUYA_FFC_APP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define FFC_MASTER 0x01
#define FFC_SLAVER 0x02

typedef enum {
	FFC_INIT_STATE = 0,
	FFC_CONTROL_STATE,
	FFC_BINDING_BEGIN_STATE,
	FFC_BINDING_SUCCESS_STATE,
	FFC_BINDING_FINSH_STATE,
} ffc_cb_state_t;

typedef int (*ffc_status_cb)(ffc_cb_state_t state);
typedef int (*ffc_recv_cb)(uint8_t *data_cmd, uint16_t data_len);

typedef enum {
	FFC_GROUP_0 = 0x00,
	FFC_GROUP_1,
	FFC_GROUP_2,
	FFC_GROUP_3,
} ffc_group_t;

typedef enum {
	FFC_CHANNEL_SCAN_FLAG = 0x01,
	FFC_GROUP_SET_FLAG = 0x02,
} ffc_flag_t;

/**
 * @brief ffc configure struct
 */
typedef struct {
	/** uuid,assigned by TUYA */
	uint8_t *uuid;
	/** auzkey,assigned by TUYA */
	uint8_t auzkey[32 + 1];
	/** mac address */
	uint8_t mac[6];
	/** wifi channel */
	uint8_t channel[13];
	/** channel count */
	uint8_t channel_count;
	/** flag */
	uint32_t flag;
} ffc_cfg_t;

/**
 * @brief ffc operation struct
 */
typedef struct {
	/** new channel set */
	int (*channel_set)(uint8_t new_channel);
	/** one channel get */
	uint8_t (*channel_get)(void);
	/** set frame */
	int (*send_frame)(uint8_t *frame, uint16_t frame_len, uint8_t src_mac[6], uint8_t dst_mac[6]);
} ffc_ops_t;

/**
 * @brief callback define when ffc init
 */
typedef struct {
	/** user status call back */
	ffc_status_cb status_cb;
	/** user received callback */
	ffc_recv_cb recv_cb;
} ffc_cb_t;

/**
 * @brief ffc init function,must called first
 *
 * @param[in] role master or slaver
 * @param[in] status_cb user status callback
 * @param[in] recv_cb user received callback
 *
 * @return OPRT_OK on success. Others error, please refer to tuya_error_code.h
 *
 */
int tuya_iot_wifi_ffc_init(uint8_t role, ffc_status_cb *status_cb, ffc_recv_cb *recv_cb);

/**
 * @brief ffc bind function
 *
 * @param[in] timeout_s time out
 *
 * @return OPRT_OK on success. Others error, please refer to tuya_error_code.h
 *
 */
int tuya_iot_wifi_ffc_bind(uint16_t timeout_s);

/**
 * @brief ffc unbind function
 *
 * @param[in] timeout_s time out
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 *
 */
int tuya_iot_wifi_ffc_unbind(uint16_t timeout_s);

/**
 * @brief ffc send
 *
 * @param[in] data send buf
 * @param[in] len send length
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 *
 */
int tuya_iot_wifi_ffc_send(uint8_t *data, uint16_t len);

/**
 * @brief ffc control
 *
 * @param[in] flag control flag
 * @param[in] data control data
 *
 * @return OPRT_OK on success. Others error, please refer to tuya_error_code.h
 *
 */
int tuya_iot_wifi_ffc_control(int flag, void *data);

#define ffc_init tuya_iot_wifi_ffc_init
#define ffc_bind tuya_iot_wifi_ffc_bind
#define ffc_unbind tuya_iot_wifi_ffc_unbind
#define ffc_send tuya_iot_wifi_ffc_send
#define ffc_control tuya_iot_wifi_ffc_control

#ifdef __cplusplus
}
#endif
#endif
