// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef _RK_TUYA_IPC_H_
#define _RK_TUYA_IPC_H_

#include "common.h"
#include "vendor_storage.h"

#include "codec/tuya_g711_utils.h"
#include "tuya_cloud_base_defs.h"
#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_error_code.h"
#include "tuya_cloud_types.h"
#include "tuya_ipc_api.h"
#include "tuya_ipc_low_power_api.h"
#include "tuya_ipc_media.h"
#include "tuya_ipc_p2p.h"
#include "tuya_ipc_sdk_init.h"
#include "tuya_ipc_stream_storage.h"
#include "tuya_ipc_video_msg.h"
#include "tuya_ring_buffer.h"

typedef struct {
	BOOL_T enable;         /*enable == false ,other var is invalid*/
	BOOL_T is_lowpower;    /* for pre-link feature */
	UINT_T max_p2p_client; /* p2p clinet max connect number*/
	TRANS_DEFAULT_QUALITY_E
	live_mode; /* for multi-streaming ipc, the default quality for live preview */
	TRANSFER_EVENT_CB transfer_event_cb; /*transfer event callback*/
	;
	TRANSFER_REV_AUDIO_CB rev_audio_cb; /*app to dev audio callback*/
} TUYA_IPC_SDK_P2P_S;

typedef struct {
	BOOL_T enable; /*enable == false ,other var is invalid*/
	CHAR_T storage_path[IPC_STORAGE_PATH_LEN + 1];
	UINT_T max_event_num_per_day;
	UINT_T preview_record_time;
	UINT_T skills; //参考tuya_ipc_stream_storage.h TUYA_IPC_SKIL_BASIC
	SS_SD_STATUS_CHANGED_CB sd_status_cb;
} TUYA_IPC_SDK_LOCAL_STORAGE_S;

typedef struct {
	BOOL_T enable;          /*enable == false ,other var is invalid*/
	BOOL_T en_audio_record; /* default enable */
	INT_T pre_record_time;
} TUYA_IPC_SDK_CLOUD_STORAGE_S;

#if defined(QRCODE_ACTIVE_MODE) && (QRCODE_ACTIVE_MODE == 1)
typedef VOID (*TUAY_QRCODE_ACTIVE_CB)(CHAR_T *shorturl);
#endif

/**
 * the fifth version api
 */
/*** separator begin  ****/
typedef struct {
	CHAR_T product_key[IPC_PRODUCT_KEY_LEN + 1]; /* one product key(PID) for one product  */
	CHAR_T
	uuid[IPC_UUID_LEN + 1]; /* one uuid-authkey pair for one device, to register on TUYA coud */
	CHAR_T auth_key[IPC_AUTH_KEY_LEN + 1];
	CHAR_T dev_sw_version[IPC_SW_VER_LEN + 1]; /* software version with format x1.x2.x3.x4.x5.x6,
	                                              five dots maximum, zero dots minimum */
	CHAR_T cfg_storage_path[IPC_STORAGE_PATH_LEN + 1];
	TUYA_RST_INFORM_CB
	gw_reset_cb; /* reset callback fucntion, triggered when user unbind device from a account */
	TUYA_RESTART_INFORM_CB gw_restart_cb; /* restart callback function */
	TUYA_IPC_DEV_TYPE_E dev_type;         /*dev type ,eg low power device*/
} TUYA_IPC_SDK_BASE_S;

typedef struct {
	WIFI_INIT_MODE_E connect_mode;
	GW_STATUS_CHANGED_CB net_status_change_cb;
} TUYA_IPC_SDK_NET_S;

typedef struct {
	BOOL_T enable; /*enable == false ,other var is invalid*/
} TUYA_IPC_SDK_CLOUD_AI_S;

typedef struct {
	IPC_MEDIA_INFO_S media_info;
} TUYA_IPC_SDK_MEDIA_STREAM_S;
typedef struct {
	DEV_DP_QUERY_CB dp_query;
	DEV_RAW_DP_CMD_CB raw_dp_cmd_proc;
	DEV_OBJ_DP_CMD_CB common_dp_cmd_proc;
} TUYA_IPC_SDK_DP_S;
typedef struct {
	BOOL_T enable;
	TUYA_IPC_SDK_DEV_UPGRADE_INFORM_CB upgrade_cb;
} TUYA_IPC_SDK_DEMO_UPGRADE_S;
typedef struct {
	BOOL_T enable;
} TUYA_IPC_SDK_QUICK_START_S;
typedef struct {
	INT_T log_level;         /*set log level default: error level*/
	CHAR_T qrcode_token[16]; // connect_method !=2 ,qrcode_token invalid
} TUYA_IPC_SDK_DEBUG_S;

typedef struct {
	BOOL_T enable;      /*enable == false ,other var is invalid*/
	MESSAGE_E type;     /* video msg type: video+audio, video only, audio only*/
	INT_T msg_duration; /* the duration of video record */
} TUYA_IPC_SDK_VIDEO_MSG_S;

/**
 * \brief IPC SDK run variable set
 * \struct TUYA_IPC_SDK_RUN_VAR_S
 */
typedef struct {
	TUYA_IPC_SDK_BASE_S iot_info;                    /*sdk base configure information*/
	TUYA_IPC_SDK_MEDIA_STREAM_S media_info;          /*audio video configure information*/
	TUYA_IPC_SDK_NET_S net_info;                     /*net work info*/
	TUYA_IPC_SDK_DEMO_UPGRADE_S upgrade_info;        /*fireware upgrade information*/
	TUYA_IPC_SDK_DP_S dp_info;                       /*date point information*/
	TUYA_IPC_SDK_P2P_S p2p_info;                     /*p2p information*/
	TUYA_IPC_SDK_LOCAL_STORAGE_S local_storage_info; /* sd card storage information*/
	TUYA_IPC_SDK_CLOUD_STORAGE_S cloud_storage_info; /*cloud storage information*/
	TUYA_IPC_SDK_CLOUD_AI_S cloud_ai_detct_info;     /*ai detcet information*/
	TUYA_IPC_SDK_VIDEO_MSG_S video_msg_info;         /*door bell function information*/
	TUYA_IPC_SDK_QUICK_START_S quick_start_info;     /*start way for p2p  */
	TUYA_IPC_SDK_DEBUG_S debug_info;                 /*debug info sets*/
#if defined(QRCODE_ACTIVE_MODE) && (QRCODE_ACTIVE_MODE == 1)
	TUAY_QRCODE_ACTIVE_CB qrcode_active_cb;
#endif
} TUYA_IPC_SDK_RUN_VAR_S;

typedef int (*rk_tuya_ao_create)();
typedef int (*rk_tuya_ao_write)(unsigned char *, int);
typedef int (*rk_tuya_ao_destroy)();
void rk_tuya_ao_create_register(rk_tuya_ao_create callback_ptr);
void rk_tuya_ao_write_register(rk_tuya_ao_write callback_ptr);
void rk_tuya_ao_destroy_register(rk_tuya_ao_destroy callback_ptr);

int rk_tuya_init();
int rk_tuya_deinit();
int rk_tuya_push_video(unsigned char *buffer, unsigned int buffer_size, int64_t present_time,
                       int nal_type);
int rk_tuya_push_audio(unsigned char *buffer, unsigned int buffer_size, int64_t present_time);
int rk_tuya_low_power_enable();
int rk_tuya_low_power_disable();

#endif
