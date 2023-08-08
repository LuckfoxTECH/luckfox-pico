/**
 * @file lan_protocol.h
 * @brief Common process - lan protocol
 * @version 0.1
 * @date 2019-08-20
 *
 * @copyright Copyright 2019-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __LAN_PROTOCOL__
#define __LAN_PROTOCOL__

#include "aes_inf.h"
#include "crc32i.h"
#include "mem_pool.h"
#include "mix_method.h"
#include "tuya_cloud_types.h"
#include "uni_log.h"

#ifdef __cplusplus
extern "C" {
#endif

// lan protocol
#define FRM_TP_CFG_WF 1 // only used for ap 3.0 network config
// #define FRM_TP_ACTV 2 // discard
// #define FRM_TP_BIND_DEV 3
// #define FRM_TP_UNBIND_DEV 6
#define FRM_TP_CMD 7
#define FRM_TP_STAT_REPORT 8
#define FRM_TP_HB 9
#define FRM_QUERY_STAT 0x0a
// #define FRM_SSID_QUERY 0x0b // discard
#define FRM_USER_BIND_REQ 0x0c
#define FRM_TP_NEW_CMD 0x0d
#define FRM_ADD_SUB_DEV_CMD 0x0e
#define FRM_CFG_WIFI_INFO 0x0f
#define FRM_QUERY_STAT_NEW 0x10
#define FRM_SCENE_EXEC 0x11
#define FRM_LAN_QUERY_DP 0x12

#define FRM_SECURITY_TYPE3 0x03
#define FRM_SECURITY_TYPE4 0x04
#define FRM_SECURITY_TYPE5 0x05

#define FRM_LAN_EXT_STREAM 0x40
#if defined(ENABLE_LAN_ENCRYPTION) && (ENABLE_LAN_ENCRYPTION == 1)
#define FR_TYPE_ENCRYPTION 0x13
#define FRM_AP_CFG_WF_V40 0x14
#define FR_TYPE_BOARDCAST_LPV34 0x23
#endif

typedef int AP_CFG_ERR_CODE;
#define AP_CFG_SUCC 0
#define AP_CFG_ERR_PCK 1
#define AP_CFG_AP_NOT_FOUND 2
#define AP_CFG_ERR_PASSWD 3
#define AP_CFG_CANT_CONN_AP 4
#define AP_CFG_DHCP_FAILED 5
#define AP_CFG_CONN_CLOUD_FAILED 6
#define AP_CFG_GET_URL_FAILED 7
#define AP_CFG_GW_ACTIVE_FAILED 8
#define AP_CFG_GW_ACTIVE_SUCCESS 9

#pragma pack(1)
/**
 * @brief lan protocol app head
 */
typedef struct {
	UINT_T head; // 0x55aa
	UINT_T fr_num;
	UINT_T fr_type;
	UINT_T len;
	BYTE_T data[0];
} LAN_PRO_HEAD_APP_S;

typedef struct {
	UINT_T crc;
	UINT_T tail; // 0xaa55
} LAN_PRO_TAIL_S;

typedef struct {
	BYTE_T hmac[32];
	UINT_T tail; // 0xaa55
} LAN_PRO_TAIL_V34_S;

/**
 * @brief lan protocol gateway head
 */
typedef struct {
	UINT_T head; // 0x55aa
	UINT_T fr_num;
	UINT_T fr_type;
	UINT_T len;
	UINT_T ret_code;
	BYTE_T data[0];
} LAN_PRO_HEAD_GW_S;
#pragma pack()

#if defined(ENABLE_LAN_ENCRYPTION) && (ENABLE_LAN_ENCRYPTION == 1)
/**
 * @brief decrypt data
 *
 * @param[in] data encry data
 * @param[in] len encry data length
 * @param[in] key decrypt key
 * @param[out] out_data origin data
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET __lan_parse_data(IN BYTE_T *data, IN CONST INT_T len, IN CONST BYTE_T *key,
                             OUT CHAR_T **out_data);

/**
 * @brief encry lan message
 *
 * @param[in] data origin data
 * @param[in] len origin data length
 * @param[in] ec_data encode data
 * @param[in] ec_len encode data length
 * @param[in] key encry key
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET encrpt_lan_msg(IN CONST BYTE_T *data, IN CONST UINT_T len, OUT BYTE_T **ec_data,
                           OUT UINT_T *ec_len, IN CONST BYTE_T *key);
#endif

/**
 * @brief lan protocol send data, the date will package 0x55aa
 *
 * @param[in] fr_num refer to LAN_PRO_HEAD_GW_S
 * @param[in] fr_type refer to LAN_PRO_HEAD_GW_S
 * @param[in] ret_code refer to LAN_PRO_HEAD_GW_S
 * @param[in] data refer to LAN_PRO_HEAD_GW_S
 * @param[in] len refer to LAN_PRO_HEAD_GW_S
 * @param[in] s_len send data length
 *
 * @return send data buf point
 */
BYTE_T *__mlp_gw_send_da(IN CONST UINT_T fr_num, IN CONST UINT_T fr_type, IN CONST UINT_T ret_code,
                         IN CONST BYTE_T *data, IN CONST UINT_T len, OUT UINT_T *s_len);

#ifdef __cplusplus
}
#endif
#endif
