/*********************************************************************************
 *Copyright(C),2017, 涂鸦科技 www.tuya.comm
 *FileName:    tuya_ipc_p2p_common.h
 **********************************************************************************/

#ifndef __TUYA_IPC_P2P_COMMON_H__
#define __TUYA_IPC_P2P_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_error_code.h"
#include "tuya_cloud_types.h"

// ppcs
#define SY_INIT_STR_LEN (128)
#define SY_CRC_KEY_LEN (10)
#define SY_P2P_KEY_LEN (20)
#define SY_SVR_KEY_LEN (20)
#define SY_DID_LEN (30)
#define SY_WAKEUP_SVR_LEN (30)

#define P2P_ID_LEN 25
#define P2P_NAME_LEN 8
#define P2P_PASSWD_LEN 8
#define P2P_GW_LOCAL_KEY_LEN 16 // securt lan dps key len
#define P2P_TYPE_LEN 8

typedef struct {
	CHAR_T p2p_id[P2P_ID_LEN + 1];
	CHAR_T p2p_name[P2P_NAME_LEN + 1];
	CHAR_T p2p_passwd[P2P_PASSWD_LEN + 1];
	CHAR_T gw_local_key[P2P_GW_LOCAL_KEY_LEN + 1];
	VOID *p_reserved;
} TUYA_IPC_P2P_AUTH_S;
/**
 * \brief 尚云认证结构体
 * \struct SY_AUTH_PARAM_S
 */
typedef struct {
	CHAR_T init_str[SY_INIT_STR_LEN];
	CHAR_T crc_key[SY_CRC_KEY_LEN];
	CHAR_T p2p_key[SY_P2P_KEY_LEN];
	CHAR_T did[SY_DID_LEN];
	CHAR_T wakeup_svrkey[SY_SVR_KEY_LEN];

	CHAR_T wakeup_server1[SY_WAKEUP_SVR_LEN];
	CHAR_T wakeup_server2[SY_WAKEUP_SVR_LEN];
	CHAR_T wakeup_server3[SY_WAKEUP_SVR_LEN];
#if (ENABLE_MQTT_P2P == 1)
	INT_T type; // p2p类型，1tutk 2ppcs 4mqtt_p2p 6ppcs+mqtt_p2p......
#endif
} P2P_AUTH_PARAM_S;

/**
 * \brief 音频采样率
 * \enum TRANSFER_AUDIO_SAMPLE_E
 */
typedef enum {
	TY_AUDIO_SAMPLE_8K,
	TY_AUDIO_SAMPLE_11K,
	TY_AUDIO_SAMPLE_12K,
	TY_AUDIO_SAMPLE_16K,
	TY_AUDIO_SAMPLE_22K,
	TY_AUDIO_SAMPLE_24K,
	TY_AUDIO_SAMPLE_32K,
	TY_AUDIO_SAMPLE_44K,
	TY_AUDIO_SAMPLE_48K,
	TY_AUDIO_SAMPLE_96K,
} TRANSFER_AUDIO_SAMPLE_E;

/**
 * \brief 音频位宽
 * \enum TRANSFER_AUDIO_DATABITS_E
 */
typedef enum {
	TY_AUDIO_DATABITS_8,
	TY_AUDIO_DATABITS_16,
} TRANSFER_AUDIO_DATABITS_E;

/**
 * \brief 音频通道
 * \enum TRANSFER_AUDIO_CHANNEL_E
 */
typedef enum {
	TY_AUDIO_CHANNEL_MONO,
	TY_AUDIO_CHANNEL_STERO,
} TRANSFER_AUDIO_CHANNEL_E;

#define P2P_SESSION_DETECH_INTV (10000)                         // session检测时间间隔ms
#define P2P_SESSION_DETECH_COUNT (120)                          // session检测次数
#define P2P_LOGIN_DETECH_CNT (180000 / P2P_SESSION_DETECH_INTV) //连续异常login检测时间间隔

typedef struct P2P_SESSION_DETECH_ {
	UINT_T lstCnt;    //最后一次计数
	UINT_T staticCnt; //满足条件的计数
} P2P_SESSION_DETECH_T;

OPERATE_RET tuya_ipc_p2p_get_auth_param(INOUT P2P_AUTH_PARAM_S *p_auth_param);
OPERATE_RET tuya_ipc_p2p_get_id(INOUT CHAR_T p2p_id[]);
OPERATE_RET tuya_ipc_check_p2p_auth_update();
OPERATE_RET tuya_ipc_p2p_reboot();
OPERATE_RET tuya_ipc_get_p2p_auth(TUYA_IPC_P2P_AUTH_S *pAuth);
OPERATE_RET tuya_ipc_p2p_update_pw(INOUT CHAR_T p2p_pw[]);
OPERATE_RET mqc_p2p_data_rept_v41(IN CONST CHAR_T *devid, IN CONST CHAR_T *pData,
                                  IN CONST INT_T len);

#ifdef __cplusplus
}
#endif

#endif
