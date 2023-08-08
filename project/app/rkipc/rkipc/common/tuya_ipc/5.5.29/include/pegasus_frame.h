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

#ifndef __PEGASUS_FRAME_H__
#define __PEGASUS_FRAME_H__

#ifdef __cplusplus
extern "C" {
#endif
/*============================ INCLUDES ======================================*/
#include "stdbool.h"
#include "stdint.h"

/*============================ MACROS ========================================*/
#define PEGASUS_FRAME_LEN_MAX 512
#define PEGASUS_DATA_LEN_MAX 256
#define PEGASUS_MAC_LEN 6

#define WNC_PEGASUS 4

#define TUYA_PEGASUS_BEACON "pegasus"
#define TUYA_PEGASUS_SSID NULL

#define PEGASUS_TYPE_CMD 1
#define PEGASUS_TYPE_PRODUCT_ID 2
#define PEGASUS_TYPE_DEV_ID 3
#define PEGASUS_TYPE_DEV_MAC 4
#define PEGASUS_TYPE_RA_UUID_HMAC 5
#define PEGASUS_TYPE_RA_AES 6
#define PEGASUS_TYPE_RA_HMAC 7
#define PEGASUS_TYPE_RB_AES 8
#define PEGASUS_TYPE_RB_HMAC 9
#define PEGASUS_TYPE_UUID_HMAC 0xa
#define PEGASUS_TYPE_AP_MAC 0xb

#define WFC_TYPE_TAG 253
#define PEGASUS_TYPE_CRC32 254
#define PEGASUS_TYPE_DATA 255

// 1.1 Client Request First Config
#define PEGASUS_CMD_CLIENT_FIRST_REQ 0x00
// 1.1 Client Request Second Config
#define PEGASUS_CMD_CLIENT_SECOND_REQ 0x01

// 1.2 Server Ack to Client Request
#define PEGASUS_CMD_SERVER_FIRST_SECOND_REQ_ACK 0x82

// 2.1 Server Request Check
#define PEGASUS_CMD_SERVER_RECHECK_REQ 0x83
// 2.2 Clien Ack to Server Check
#define PEGASUS_CMD_CLIENT_RECHECK_ACK 0x03
// 2.3 Server Cancel Client Config
/*cancel pegasus netcfg ,go to concurrent共存配网*/
#define PEGASUS_CMD_SERVER_CONFIG_CANCEL 0x84

// 3.1 Server Send ssid&passwd&token First Config
#define PEGASUS_CMD_SERVER_FIRST_CONFIG_RSP 0x80
// 3.1 Server Send ssid&passwd&token Second Config
#define PEGASUS_CMD_SERVER_SECOND_CONFIG_RSP 0x81
// 3.2 Client Ack to Server Send ssid&passwd&token
#define PEGASUS_CMD_CLIENT_NET_CONFIG_ACK 0x02

#define PEGASUS_DATA_TYPE_SSID 1
#define PEGASUS_DATA_TYPE_SECURITY_MODE 2
#define PEGASUS_DATA_TYPE_PASSWD 3
#define PEGASUS_DATA_TYPE_CHANNEL 4
#define PEGASUS_DATA_TYPE_BSSID 5
#define PEGASUS_DATA_TYPE_TOKEN 6
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
extern const uint8_t c_tuya_oui[3];
int pegasus_frame_malloc(uint8_t **frame);
void pegasus_frame_free(uint8_t *frame);
uint32_t pegasus_frame_data_set(uint8_t *frame, uint32_t offset, uint8_t type, uint8_t *data,
                                uint32_t len);
uint32_t pegasus_frame_crc_set(uint8_t *frame, uint32_t offset);
int pegasus_frame_crc_check(uint8_t *frame, int frame_len);

#ifdef __cplusplus
}
#endif

#endif
