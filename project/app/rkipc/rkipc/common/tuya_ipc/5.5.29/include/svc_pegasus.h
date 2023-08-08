/*============================================================================
*                                                                            *
* Copyright (C) by Tuya Inc                                                  *
* All rights reserved                                                        *
*                                                                            *
* @author  :   wangym Linch                                                  *
* @date    :   2020-03-03                                                    *
* @brief   :   tuya pegasus help device config without password phase        *
* @log     :   first implement by wangym                                     *
=============================================================================*/

#ifndef __SVC_PEGASUS_H__
#define __SVC_PEGASUS_H__

#ifdef __cplusplus
extern "C" {
#endif
/*============================ INCLUDES ======================================*/
#include "pegasus_frame.h"
#include "stdbool.h"
#include "stdint.h"
#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_wifi_defs.h"
#include "tuya_iot_dev_check.h"
#if WIFI_GW
#include "netcfg_module.h"
#include "wifi_netcfg_frame_sniffer.h"
#include "wifi_netcfg_frame_transporter.h"
#endif
/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef enum {
	PEGASUS_CLIENT_MODE_FIRST,
	PEGASUS_CLIENT_MODE_SECOND,
	PEGASUS_SERVER_MODE_FIRST,
	PEGASUS_SERVER_MODE_SECOND,
} pegasus_mode_t;

typedef enum {
	PEGASUS_IDLE,       // 0
	PEGASUS_SEND_REQ,   // 1
	PEGASUS_RECV_REQ,   // 2
	PEGASUS_SEND_ACK,   // 3
	PEGASUS_RECV_ACK,   // 4
	PEGASUS_CALC_KEY,   // 5
	PEGASUS_CHECKING,   // 6
	PEGASUS_SEND_MQTT,  // 7
	PEGASUS_RECV_MQTT,  // 8
	PEGASUS_SEND_CHECK, // 9
	PEGASUS_RECV_CHECK, // 10
	PEGASUS_SEND_CFG,   // 11
	PEGASUS_RECV_CFG,   // 12
	PEGASUS_CONNECT,    // 13
} pegasus_stat_t;

typedef struct {
	bool ack;
	int count;
	uint8_t dev_mac[6];
	uint8_t ap_mac[6];
	char pid[PRODUCT_KEY_LEN + 1];
	char uuid[GW_UUID_LEN + 1];
	uint8_t ra_uuid_hmac[TY_DEV_SHA256_DIGEST_SIZE];
	uint8_t ra_aes[TY_DEV_RANDOM_AES_LEN];
	uint8_t ra_hmac[TY_DEV_SHA256_DIGEST_SIZE];
	uint8_t rb_aes[TY_DEV_RANDOM_AES_LEN];
	uint8_t rb_hmac[TY_DEV_SHA256_DIGEST_SIZE];
	uint8_t uuid_hmac[TY_DEV_SHA256_DIGEST_SIZE];
	uint8_t key[TY_DEV_SESSION_KEY_LEN];
	pegasus_stat_t stat;
	pegasus_mode_t mode;
} pegasus_info_t;

/*============================ PROTOTYPES ====================================*/
int pegasus_server_mqc_handle(ty_cJSON *root_json);
int pegasus_server_second_start(uint32_t timeout_s);
int pegasus_recv_data_handle_cb(void *ptrArgs, uint8_t *buffer, uint16_t length, const int8_t rssi);
int pegasus_server_add_device(char *token, char *uuid);
typedef void (*fnLockChanCb)(void);

#if WIFI_GW
#include "wl_cfg_com.h"
int tuya_scan_all_ap(AP_IF_S **ap_ary, uint8_t *num, uint8_t *oui, AP_IF_S **oui_ap);
int pegasus_beacon_info_get(uint8_t *buf, uint16_t len, char rssi, AP_IF_S *ap_info);
typedef int (*pegasus_second_connect_cb)(char *ssid, char *passwd);
int pegasus_init(IN CONST fnLockChanCb lc_cb, IN CONST FN_NETCFG_CB ncf_cb,
                 ptrWifiNetcfgFrameSniffer_t pSniffer,
                 ptrWifiNetcfgFrameTransporter_t pTransporter);
int pegasus_second_init(pegasus_second_connect_cb connect_cb, AP_IF_S *pegasus_ap);
int pegasus_uninit(void);
WLAN_MANAGEMENT_S *pegasus_find_oui(uint8_t *buf, int buf_len, uint8_t *oui);
#endif

#ifdef __cplusplus
}
#endif

#endif
