// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "tuya_ipc.h"
#include "tools.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "tuya_ipc.c"

#define VENDOR_TUYA_LICENSE_ID 254
// Path to save tuya sdk DB files, should be readable, writeable and storable
#define IPC_APP_STORAGE_PATH "/userdata/"
// File with path to download file during OTA
#define IPC_APP_UPGRADE_FILE "/tmp/upgrade.file"
// SD card mount directory
#define IPC_APP_SD_BASE_PATH "/mnt/sdcard/"
// Firmware version displayed on TUYA APP
#define IPC_APP_VERSION "1.2.3"
// TUYA P2P supports 5 users at the same time, include live preview and playback
#define MAX_P2P_USER 5
#define TUYA_DP_DOOR_STATUS 149

typedef struct {
	BOOL_T enabled;
	TRANSFER_VIDEO_CLARITY_TYPE_E live_clarity;
	UINT_T max_users;
	TUYA_CODEC_ID p2p_audio_codec;
} TUYA_APP_P2P_MGR;

static Ring_Buffer_User_Handle_S audio_handle;
static Ring_Buffer_User_Handle_S video_handle;
static TUYA_APP_P2P_MGR s_p2p_mgr = {0};
static IPC_MEDIA_INFO_S s_media_info = {0};
static INT_T s_mqtt_status = 0;
static MEDIA_FRAME_S video_frame, audio_frame;

rk_tuya_ao_create rk_tuya_ao_create_ = NULL;
rk_tuya_ao_write rk_tuya_ao_write_ = NULL;
rk_tuya_ao_destroy rk_tuya_ao_destroy_ = NULL;

void rk_tuya_ao_create_register(rk_tuya_ao_create callback_ptr) {
	rk_tuya_ao_create_ = callback_ptr;
}

void rk_tuya_ao_write_register(rk_tuya_ao_write callback_ptr) { rk_tuya_ao_write_ = callback_ptr; }

void rk_tuya_ao_destroy_register(rk_tuya_ao_destroy callback_ptr) {
	rk_tuya_ao_destroy_ = callback_ptr;
}

STATIC VOID __depereated_online_cb(IN TRANSFER_ONLINE_E status) {
	LOG_INFO("status is %d\n", status);
}

unsigned char ao_data[2048];
int TUYA_APP_Enable_Speaker_CB(int enable) {
	if ((rk_tuya_ao_create_ == NULL) || (rk_tuya_ao_destroy_ == NULL)) {
		LOG_ERROR("rk_tuya_ao_create_ or rk_tuya_ao_destroy_ is null\n");
		return -1;
	}
	if (enable)
		rk_tuya_ao_create_();
	else
		rk_tuya_ao_destroy_();

	return 0;
}

STATIC VOID __TUYA_APP_rev_audio_cb(IN CONST TRANSFER_AUDIO_FRAME_S *p_audio_frame,
                                    IN CONST UINT_T frame_no) {
	// LOG_DEBUG("Rev Audio. size:%u audio_codec:%d audio_sample:%d "
	//           "audio_databits:%d audio_channel:%d\n",
	//           p_audio_frame->buf_len, p_audio_frame->audio_codec, p_audio_frame->audio_sample,
	//           p_audio_frame->audio_databits, p_audio_frame->audio_channel);
	int data_len;
	tuya_g711_decode(TUYA_G711_MU_LAW, (unsigned short *)p_audio_frame->p_audio_buf,
	                 p_audio_frame->buf_len, ao_data, &data_len);
	// LOG_INFO("p_audio_frame->buf_len is %d, data_len is %d\n", p_audio_frame->buf_len, data_len);
	rk_tuya_ao_write_(ao_data, data_len);
}

/* Callback functions for transporting events */
INT_T __TUYA_APP_p2p_event_cb(IN CONST TRANSFER_EVENT_E event, IN CONST PVOID_T args) {
	LOG_INFO("p2p rev event cb=[%d]\n", event);
	if (args == NULL) {
		LOG_DEBUG("p2p rev event args null ");
		return -1;
	}
	switch (event) {
	case TRANS_LIVE_VIDEO_START: {
		C2C_TRANS_CTRL_VIDEO_START *parm = (C2C_TRANS_CTRL_VIDEO_START *)args;
		LOG_INFO("%s: chn[%u] video start\n", get_time_string(), parm->channel);
		FILE *fp;
		fp = fopen("/tmp/rtmp_live", "a");
		if (fp)
			fclose(fp);
		break;
	}
	case TRANS_LIVE_VIDEO_STOP: {
		C2C_TRANS_CTRL_VIDEO_STOP *parm = (C2C_TRANS_CTRL_VIDEO_STOP *)args;
		LOG_INFO("chn[%u] video stop\n", parm->channel);
		unlink("/tmp/rtmp_live");
		break;
	}
	case TRANS_LIVE_AUDIO_START: {
		C2C_TRANS_CTRL_AUDIO_START *parm = (C2C_TRANS_CTRL_AUDIO_START *)args;
		LOG_INFO("chn[%u] audio start\n", parm->channel);
		break;
	}
	case TRANS_LIVE_AUDIO_STOP: {
		C2C_TRANS_CTRL_AUDIO_STOP *parm = (C2C_TRANS_CTRL_AUDIO_STOP *)args;
		LOG_INFO("chn[%u] audio stop\n", parm->channel);
		break;
	}
	case TRANS_SPEAKER_START: {
		LOG_INFO("enbale audio speaker\n");
		TUYA_APP_Enable_Speaker_CB(TRUE);
		break;
	}
	case TRANS_SPEAKER_STOP: {
		LOG_INFO("disable audio speaker\n");
		TUYA_APP_Enable_Speaker_CB(FALSE);
		break;
	}
	case TRANS_LIVE_LOAD_ADJUST: {
		C2C_TRANS_LIVE_LOAD_PARAM_S *quality = (C2C_TRANS_LIVE_LOAD_PARAM_S *)args;
		LOG_INFO("live quality %d -> %d\n", quality->curr_load_level, quality->new_load_level);
		break;
	}
	case TRANS_PLAYBACK_LOAD_ADJUST: {
		C2C_TRANS_PB_LOAD_PARAM_S *quality = (C2C_TRANS_PB_LOAD_PARAM_S *)args;
		LOG_INFO("pb idx:%d quality %d -> %d\n", quality->client_index, quality->curr_load_level,
		         quality->new_load_level);
		break;
	}
	case TRANS_ABILITY_QUERY: {
		C2C_TRANS_QUERY_FIXED_ABI_REQ *pAbiReq;
		pAbiReq = (C2C_TRANS_QUERY_FIXED_ABI_REQ *)args;
		pAbiReq->ability_mask = TY_CMD_QUERY_IPC_FIXED_ABILITY_TYPE_VIDEO |
		                        TY_CMD_QUERY_IPC_FIXED_ABILITY_TYPE_SPEAKER |
		                        TY_CMD_QUERY_IPC_FIXED_ABILITY_TYPE_MIC;
		break;
	}
	case TRANS_PLAYBACK_QUERY_MONTH_SIMPLIFY:
	case TRANS_PLAYBACK_QUERY_DAY_TS:
	case TRANS_PLAYBACK_START_TS:
	case TRANS_PLAYBACK_PAUSE:
	case TRANS_PLAYBACK_RESUME:
	case TRANS_PLAYBACK_MUTE:
	case TRANS_PLAYBACK_UNMUTE:
	case TRANS_PLAYBACK_STOP:
		break;
	case TRANS_LIVE_VIDEO_CLARITY_SET: {
		C2C_TRANS_LIVE_CLARITY_PARAM_S *pParam = (C2C_TRANS_LIVE_CLARITY_PARAM_S *)args;
		LOG_INFO("set clarity:%d\n", pParam->clarity);
		if ((pParam->clarity == TY_VIDEO_CLARITY_STANDARD) ||
		    (pParam->clarity == TY_VIDEO_CLARITY_HIGH)) {
			LOG_INFO("set clarity:%d OK\n", pParam->clarity);
			s_p2p_mgr.live_clarity = pParam->clarity;
		}
		break;
	}
	case TRANS_LIVE_VIDEO_CLARITY_QUERY: {
		C2C_TRANS_LIVE_CLARITY_PARAM_S *pParam = (C2C_TRANS_LIVE_CLARITY_PARAM_S *)args;
		pParam->clarity = s_p2p_mgr.live_clarity;
		LOG_INFO("query larity:%d\n", pParam->clarity);
		break;
	}
	case TRANS_DOWNLOAD_START:
	case TRANS_DOWNLOAD_STOP:
	case TRANS_DOWNLOAD_PAUSE:
	case TRANS_DOWNLOAD_RESUME:
	case TRANS_DOWNLOAD_CANCLE:
		break;
	case TRANS_STREAMING_VIDEO_START: {
		TRANSFER_SOURCE_TYPE_E *pSrcType = (TRANSFER_SOURCE_TYPE_E *)args;
		LOG_INFO("streaming start type %d\n", *pSrcType);
		break;
	}
	case TRANS_STREAMING_VIDEO_STOP: {
		TRANSFER_SOURCE_TYPE_E *pSrcType = (TRANSFER_SOURCE_TYPE_E *)args;
		LOG_INFO("streaming stop type %d\n", *pSrcType);
		break;
	}
	default:
		break;
	}

	return 0;
}

int rk_tuya_low_power_enable() {
	LOG_INFO("tuya low power mode enable\n");
	BOOL_T doorStat = FALSE;
	int ret = 0;
	int server_ip, port;
	int id_len = 64;
	int key_len = 64;
	int wakeup_len = 36;
	int heartbeat_len = 12;
	unsigned char dev_id[64] = {0};
	unsigned char dest_key_buf[64] = {0};
	unsigned char wakeup_data[36] = {0};
	unsigned char heartbeat_data[12] = {0};
	int mqtt_socket_fd = -1;

	// Report sleep status to tuya
	ret = tuya_ipc_dp_report(NULL, TUYA_DP_DOOR_STATUS, PROP_BOOL, &doorStat, 1);
	if (OPRT_OK != ret) {
		// This dp point is very important. The user should repeat call the report
		// interface until the report is successful when it is fail.
		// If it is failure continues, the user needs to check the network
		// connection.
		LOG_ERROR("dp report failed\n");
		return ret;
	}

	// low power server connect
	tuya_ipc_low_power_server_get(&server_ip, &port);
	tuya_ipc_device_id_get(dev_id, &id_len);
	tuya_ipc_local_key_get(dest_key_buf, &key_len);
	LOG_INFO("server_ip is %d, port is %d, dev_id is %s, dest_key_buf is %s\n", server_ip, port,
	         dev_id, dest_key_buf);
#if 0 // wifi do it
	// ret = tuya_ipc_low_power_server_connect(server_ip, port, dev_id, strlen(dev_id), dest_key_buf, strlen(dest_key_buf));
	// if (OPRT_OK != ret) {
	// 	LOG_ERROR("tuya_ipc_low_power_server_connect failed\n");
	// 	return ret;
	// }
	// LOG_INFO("tuya_ipc_low_power_server_connect success\n");

	// // Get fd for server to wakeup
	// mqtt_socket_fd = tuya_ipc_low_power_socket_fd_get();
	// if (-1 == mqtt_socket_fd) {
	// 	LOG_ERROR("tuya_ipc_get_mqtt_socket_fd failed\n");
	// 	return ret;
	// }
	// LOG_INFO("mqtt_socket_fd is %d\n", mqtt_socket_fd);
#endif
	ret = tuya_ipc_low_power_wakeup_data_get(wakeup_data, &wakeup_len);
	if (OPRT_OK != ret) {
		LOG_ERROR("tuya_ipc_get_wakeup_data failed\n");
		return ret;
	}
	LOG_INFO("wakeup_data is {");
	for (int i = 0; i < wakeup_len; i++) {
		printf("%x ", wakeup_data[i]);
	}
	printf("}\n");

	ret = tuya_ipc_low_power_heart_beat_get(heartbeat_data, &heartbeat_len);
	if (OPRT_OK != ret) {
		LOG_ERROR("tuya_ipc_get_heartbeat_data failed\n");
		return ret;
	}
	LOG_INFO("heartbeat_data is {");
	for (int i = 0; i < heartbeat_len; i++) {
		printf("%x ", heartbeat_data[i]);
	}
	printf("}\n");

	ret = set_filter("ICMP", "0");
	LOG_INFO("set_filter ICMP ret is %d\n", ret);
	ret = set_filter("DES", "443");
	LOG_INFO("set_filter DES ret is %d\n", ret);
	ret = set_filter("SRC", "443");
	LOG_INFO("set_filter SRC ret is %d\n", ret);
	ret = enable_filter();
	LOG_INFO("set_filter enable_filter ret is %d\n", ret);

	char cmd[256] = {0};
	sprintf(cmd,
	        "AT+WIFI_HEART_PKT TEXT keepalive DEVID %s KEY %s PERIOD 80000 SERVER 42.192.35.108 "
	        "PORT 443",
	        dev_id, dest_key_buf);
	LOG_INFO("cmd is %s\n", cmd);
	ret = fw_at(cmd);
	LOG_INFO("fw_at AT+WIFI_HEART_PKT ret is %d\n", ret);
	sleep(3);
	ret = fw_at("AT+WIFI_LISTEN_ITVL 10");
	LOG_INFO("fw_at AT+WIFI_LISTEN_ITVL ret is %d\n", ret);
	ret = fw_at("AT+LIGHT_SLEEP 1");
	LOG_INFO("fw_at AT+LIGHT_SLEEP ret is %d\n", ret);
	driver_go_sleep();
	LOG_INFO("driver_go_sleep\n");

	return ret;
}

int rk_tuya_low_power_disable() {
	BOOL_T doorStat = TRUE;
	int ret = 0;

	ret = tuya_ipc_dp_report(NULL, TUYA_DP_DOOR_STATUS, PROP_BOOL, &doorStat, 1);

	return ret;
}

VOID IPC_APP_Net_Status_cb(IN CONST BYTE_T stat) {
	LOG_DEBUG("Net status change to:%d", stat);
	switch (stat) {
#if defined(WIFI_GW) && (WIFI_GW == 1)
	case STAT_CLOUD_CONN:  // for wifi ipc
	case STAT_MQTT_ONLINE: // for low-power wifi ipc
#endif
#if defined(WIFI_GW) && (WIFI_GW == 0)
	case GB_STAT_CLOUD_CONN: // for wired ipc
#endif
	{
		// IPC_APP_Notify_LED_Sound_Status_CB(IPC_MQTT_ONLINE);
		LOG_INFO("---------------%s: mqtt is online---------------\r\n", get_time_string());
		s_mqtt_status = 1;
		break;
	}
	default: {
		break;
	}
	}
}

INT_T IPC_APP_Get_MqttStatus() { return s_mqtt_status; }

VOID IPC_APP_handle_dp_query_objs(IN CONST TY_DP_QUERY_S *dp_query) {
	LOG_DEBUG("tmp do nothing\n");
	// INT_T table_idx = 0;
	// INT_T table_count = ( sizeof(s_dp_table) / sizeof(s_dp_table[0]) );
	// INT_T index = 0;
	// for(index = 0; index < dp_query->cnt; index++)
	// {
	//     if(dp_simulation_filter)
	//     {
	//         if(dp_simulation_filter(dp_query->dpid) == 0)
	//         {
	//             continue;
	//         }
	//     }
	//     for(table_idx = 0; table_idx < table_count; table_idx++)
	//     {
	//         if(s_dp_table[table_idx].dp_id == dp_query->dpid[index])
	//         {
	//             s_dp_table[table_idx].handler(NULL);
	//             break;
	//         }
	//     }
	// }
}

VOID IPC_APP_handle_raw_dp_cmd_objs(IN CONST TY_RECV_RAW_DP_S *dp_rev) {
	LOG_DEBUG("tmp do nothing\n");
	// INT_T table_idx = 0;
	// INT_T table_count = ( sizeof(s_raw_dp_table) / sizeof(s_raw_dp_table[0]) );
	// INT_T index = 0;

	// for(table_idx = 0; table_idx < table_count; table_idx++)
	// {
	//     if(s_raw_dp_table[table_idx].dp_id == dp_rev->dpid)
	//     {
	//         s_raw_dp_table[table_idx].handler(dp_rev);
	//         break;
	//     }
	// }
}

VOID IPC_APP_handle_dp_cmd_objs(IN CONST TY_RECV_OBJ_DP_S *dp_rev) {
	LOG_DEBUG("tmp do nothing\n");
	// TY_OBJ_DP_S *dp_data = (TY_OBJ_DP_S *)(dp_rev->dps);
	// UINT_T cnt = dp_rev->dps_cnt;
	// INT_T table_idx = 0;
	// INT_T table_count = ( sizeof(s_dp_table) / sizeof(s_dp_table[0]) );
	// INT_T index = 0;
	// for(index = 0; index < cnt; index++)
	// {
	//     TY_OBJ_DP_S *p_dp_obj = dp_data + index;

	//     if(dp_simulation_filter)
	//     {
	//         if(dp_simulation_filter(p_dp_obj->dpid) == 0)
	//         {
	//             continue;
	//         }
	//     }

	//     for(table_idx = 0; table_idx < table_count; table_idx++)
	//     {
	//         if(s_dp_table[table_idx].dp_id == p_dp_obj->dpid)
	//         {
	//             s_dp_table[table_idx].handler(p_dp_obj);
	//             break;
	//         }
	//     }
	// }
}

INT_T IPC_APP_Upgrade_Inform_cb(IN CONST FW_UG_S *fw) {
	LOG_DEBUG("Rev Upgrade Info");
	LOG_DEBUG("fw->fw_url:%s", fw->fw_url);
	LOG_DEBUG("fw->sw_ver:%s", fw->sw_ver);
	LOG_DEBUG("fw->file_size:%u", fw->file_size);

	// FILE *p_upgrade_fd = fopen(IPC_APP_UPGRADE_FILE, "w+b");
	// tuya_ipc_upgrade_sdk(fw, __IPC_APP_get_file_data_cb, __IPC_APP_upgrade_notify_cb,
	// (PVOID_T)p_upgrade_fd);

	return 0;
}

/*
Callback when the user clicks on the APP to remove the device
*/
VOID IPC_APP_Reset_System_CB(GW_RESET_TYPE_E type) {
	printf("reset ipc success. please restart the ipc %d\n", type);
	// IPC_APP_Notify_LED_Sound_Status_CB(IPC_RESET_SUCCESS);
	// TODO
	/* Developers need to restart IPC operations */
}

VOID IPC_APP_Restart_Process_CB(VOID) {
	printf("sdk internal restart request. please restart the ipc\n");
	// TODO
	/* Developers need to implement restart operations. Restart the process or restart the device.
	 */
}

IPC_MEDIA_INFO_S *IPC_APP_Get_Media_Info() { return &s_media_info; }

OPERATE_RET TUYA_APP_Enable_P2PTransfer(IN TUYA_IPC_SDK_P2P_S *p2p_infos) {
	if (s_p2p_mgr.enabled == TRUE) {
		LOG_DEBUG("P2P Is Already Inited");
		return OPRT_OK;
	}

	if (p2p_infos == NULL) {
		LOG_DEBUG("Init P2PTransfer fail. Param is null");
		return OPRT_INVALID_PARM;
	}

	if (p2p_infos->enable == FALSE) {
		return OPRT_OK;
	}

	IPC_MEDIA_INFO_S *p_media_info = IPC_APP_Get_Media_Info();
	if (p_media_info == NULL) {
		return OPRT_COM_ERROR;
	}

	LOG_DEBUG("Init P2P With Max Users:%u", p2p_infos->max_p2p_client);

	s_p2p_mgr.enabled = TRUE;
	s_p2p_mgr.max_users = p2p_infos->max_p2p_client;
	s_p2p_mgr.p2p_audio_codec = p_media_info->audio_codec[E_IPC_STREAM_AUDIO_MAIN];

	TUYA_IPC_TRANSFER_VAR_S p2p_var = {0};
	p2p_var.online_cb = __depereated_online_cb;
	p2p_var.on_rev_audio_cb = p2p_infos->rev_audio_cb;
	/*speak data format  app->ipc*/
	p2p_var.rev_audio_codec = p_media_info->audio_codec[E_IPC_STREAM_AUDIO_MAIN];
	p2p_var.audio_sample = p_media_info->audio_sample[E_IPC_STREAM_AUDIO_MAIN];
	p2p_var.audio_databits = p_media_info->audio_databits[E_IPC_STREAM_AUDIO_MAIN];
	p2p_var.audio_channel = p_media_info->audio_channel[E_IPC_STREAM_AUDIO_MAIN];
	/*end*/
	p2p_var.on_event_cb = p2p_infos->transfer_event_cb;
	p2p_var.live_quality = TRANS_LIVE_QUALITY_MAX;
	p2p_var.max_client_num = p2p_infos->max_p2p_client;
	p2p_var.lowpower = p2p_infos->is_lowpower;
	memcpy(&p2p_var.AVInfo, p_media_info, sizeof(IPC_MEDIA_INFO_S));
	tuya_ipc_tranfser_init(&p2p_var);
	LOG_INFO("%s: tuya_ipc_tranfser_init over\n", get_time_string());

	return OPRT_OK;
}

STATIC TUYA_IPC_SDK_RUN_VAR_S g_sdk_run_info = {0};

STATIC VOID respone_dp_value(BYTE_T dp_id, INT_T val);
STATIC VOID respone_dp_bool(BYTE_T dp_id, BOOL_T true_false);
STATIC VOID respone_dp_enum(BYTE_T dp_id, CHAR_T *p_val_enum);
STATIC VOID respone_dp_str(BYTE_T dp_id, CHAR_T *p_val_str);
STATIC VOID handle_DP_SD_STORAGE_ONLY_GET(IN TY_OBJ_DP_S *p_obj_dp);

//------------------------------------------
VOID IPC_APP_upload_all_status(VOID) {
#ifdef TUYA_DP_SLEEP_MODE
	respone_dp_bool(TUYA_DP_SLEEP_MODE, IPC_APP_get_sleep_mode());
#endif

#ifdef TUYA_DP_LIGHT
	respone_dp_bool(TUYA_DP_LIGHT, IPC_APP_get_light_onoff());
#endif

#ifdef TUYA_DP_FLIP
	respone_dp_bool(TUYA_DP_FLIP, IPC_APP_get_flip_onoff());
#endif

#ifdef TUYA_DP_WATERMARK
	respone_dp_bool(TUYA_DP_WATERMARK, IPC_APP_get_watermark_onoff());
#endif

#ifdef TUYA_DP_WDR
	respone_dp_bool(TUYA_DP_WDR, IPC_APP_get_wdr_onoff());
#endif

#ifdef TUYA_DP_NIGHT_MODE
	respone_dp_enum(TUYA_DP_NIGHT_MODE, IPC_APP_get_night_mode());
#endif

#ifdef TUYA_DP_ALARM_FUNCTION
	respone_dp_bool(TUYA_DP_ALARM_FUNCTION, IPC_APP_get_alarm_function_onoff());
#endif

#ifdef TUYA_DP_ALARM_SENSITIVITY
	respone_dp_enum(TUYA_DP_ALARM_SENSITIVITY, IPC_APP_get_alarm_sensitivity());
#endif

#ifdef TUYA_DP_ALARM_ZONE_ENABLE
	respone_dp_bool(TUYA_DP_ALARM_ZONE_ENABLE, IPC_APP_get_alarm_zone_onoff());
#endif

#ifdef TUYA_DP_ALARM_ZONE_DRAW
	respone_dp_str(TUYA_DP_ALARM_ZONE_DRAW, IPC_APP_get_alarm_zone_draw());
#endif

#ifdef TUYA_DP_SD_STATUS_ONLY_GET
	respone_dp_value(TUYA_DP_SD_STATUS_ONLY_GET, IPC_APP_get_sd_status());
#endif

#ifdef TUYA_DP_SD_STORAGE_ONLY_GET
	handle_DP_SD_STORAGE_ONLY_GET(NULL);
#endif

#ifdef TUYA_DP_SD_RECORD_ENABLE
	respone_dp_bool(TUYA_DP_SD_RECORD_ENABLE, IPC_APP_get_sd_record_onoff());
#endif

#ifdef TUYA_DP_SD_RECORD_MODE
	CHAR_T sd_mode[4];
	snprintf(sd_mode, 4, "%d", IPC_APP_get_sd_record_mode());
	respone_dp_enum(TUYA_DP_SD_RECORD_MODE, sd_mode);
#endif

#ifdef TUYA_DP_SD_FORMAT_STATUS_ONLY_GET
	respone_dp_value(TUYA_DP_SD_FORMAT_STATUS_ONLY_GET, 0);
#endif

#ifdef TUYA_DP_BLUB_SWITCH
	respone_dp_bool(TUYA_DP_BLUB_SWITCH, IPC_APP_get_blub_onoff());
#endif

#ifdef TUYA_DP_POWERMODE
	IPC_APP_update_battery_status();
#endif
}

STATIC VOID *tuya_ipc_sdk_mqtt_online_proc(PVOID_T arg) {
	LOG_DEBUG("tuya_ipc_sdk_mqtt_online_proc thread start success\n");
	while (IPC_APP_Get_MqttStatus() == FALSE) {
		usleep(10 * 1000);
	}
	LOG_DEBUG("tuya_ipc_sdk_mqtt_online_proc is start run\n");
	int ret;
	//同步服务器时间
	TIME_T time_utc;
	INT_T time_zone;
	do {
		//需要SDK同步到时间后才能开启下面的业务
		ret = tuya_ipc_get_service_time_force(&time_utc, &time_zone);

	} while (ret != OPRT_OK);
	LOG_INFO("%s: tuya_ipc_get_service_time_force over\n", get_time_string());

	if (FALSE == g_sdk_run_info.quick_start_info.enable) {
		TUYA_APP_Enable_P2PTransfer(&(g_sdk_run_info.p2p_info));
		LOG_INFO("%s: TUYA_APP_Enable_P2PTransfer over\n", get_time_string());
	}

	// if (g_sdk_run_info.local_storage_info.enable) {
	// 	ret = TUYA_APP_Init_Stream_Storage(&(g_sdk_run_info.local_storage_info));
	// 	LOG_DEBUG("local storage init result is %d\n", ret);
	// }

	// if (g_sdk_run_info.cloud_ai_detct_info.enable) {
	// 	ret = TUYA_APP_Enable_AI_Detect();
	// 	LOG_DEBUG("ai detect result is %d\n", ret);
	// }

	// if (g_sdk_run_info.video_msg_info.enable) {
	// 	ret = TUYA_APP_Enable_Video_Msg(&(g_sdk_run_info.video_msg_info));
	// 	LOG_DEBUG("door bell init result is %d\n", ret);
	// }

	// if (g_sdk_run_info.cloud_storage_info.enable) {
	// 	ret = TUYA_APP_Enable_CloudStorage(&(g_sdk_run_info.cloud_storage_info));
	// 	LOG_DEBUG("cloud storage init result is %d\n", ret);
	// }

	IPC_APP_upload_all_status();

	tuya_ipc_upload_skills();
	rk_tuya_low_power_disable();
	LOG_INFO("%s, tuya_ipc_sdk_mqtt_online_proc is end run\n", get_time_string());

	return NULL;
}

OPERATE_RET TUYA_APP_Init_Ring_Buffer(VOID) {
	OPERATE_RET ret = OPRT_OK;

	STATIC BOOL_T s_ring_buffer_inited = FALSE;
	if (s_ring_buffer_inited == TRUE) {
		LOG_DEBUG("The Ring Buffer Is Already Inited");
		return OPRT_OK;
	}

	IPC_STREAM_E ringbuffer_stream_type;
	// CHANNEL_E channel;
	Ring_Buffer_Init_Param_S param = {0};
	for (ringbuffer_stream_type = E_IPC_STREAM_VIDEO_MAIN;
	     ringbuffer_stream_type < E_IPC_STREAM_MAX; ringbuffer_stream_type++) {
		LOG_DEBUG("init ring buffer Channel:%d Enable:%d", ringbuffer_stream_type,
		          s_media_info.channel_enable[ringbuffer_stream_type]);
		if (s_media_info.channel_enable[ringbuffer_stream_type] == TRUE) {
			if (ringbuffer_stream_type >= E_IPC_STREAM_AUDIO_MAIN) {
				param.bitrate = s_media_info.audio_sample[E_IPC_STREAM_AUDIO_MAIN] *
				                s_media_info.audio_databits[E_IPC_STREAM_AUDIO_MAIN] / 1024;
				param.fps = s_media_info.audio_fps[E_IPC_STREAM_AUDIO_MAIN];
				param.max_buffer_seconds = 0;
				param.requestKeyFrameCB = NULL;
				LOG_DEBUG("audio_sample %d, audio_databits %d, audio_fps %d",
				          s_media_info.audio_sample[E_IPC_STREAM_AUDIO_MAIN],
				          s_media_info.audio_databits[E_IPC_STREAM_AUDIO_MAIN],
				          s_media_info.audio_fps[E_IPC_STREAM_AUDIO_MAIN]);
				ret = tuya_ipc_ring_buffer_init(0, 0, ringbuffer_stream_type, &param);
			} else {
				param.bitrate = s_media_info.video_bitrate[ringbuffer_stream_type];
				param.fps = s_media_info.video_fps[ringbuffer_stream_type];
				param.max_buffer_seconds = 0;
				param.requestKeyFrameCB = NULL;
				LOG_DEBUG("video_bitrate %d, video_fps %d",
				          s_media_info.video_bitrate[ringbuffer_stream_type],
				          s_media_info.video_fps[ringbuffer_stream_type]);
				ret = tuya_ipc_ring_buffer_init(0, 0, ringbuffer_stream_type, &param);
			}
			if (ret != 0) {
				LOG_ERROR("init ring buffer fails. %d %d", ringbuffer_stream_type, ret);
				return OPRT_MALLOC_FAILED;
			}
			LOG_DEBUG("init ring buffer success. channel:%d", ringbuffer_stream_type);
		}
	}

	s_ring_buffer_inited = TRUE;

	return OPRT_OK;
}

/* Set audio and video properties */
VOID IPC_APP_Set_Media_Info(IPC_MEDIA_INFO_S *media) {
	memcpy(&s_media_info, media, sizeof(IPC_MEDIA_INFO_S));

	LOG_DEBUG("channel_enable:%d %d %d", s_media_info.channel_enable[0],
	          s_media_info.channel_enable[1], s_media_info.channel_enable[2]);

	LOG_DEBUG("fps:%u", s_media_info.video_fps[E_IPC_STREAM_VIDEO_MAIN]);
	LOG_DEBUG("gop:%u", s_media_info.video_gop[E_IPC_STREAM_VIDEO_MAIN]);
	LOG_DEBUG("bitrate:%u kbps", s_media_info.video_bitrate[E_IPC_STREAM_VIDEO_MAIN]);
	LOG_DEBUG("video_main_width:%u", s_media_info.video_width[E_IPC_STREAM_VIDEO_MAIN]);
	LOG_DEBUG("video_main_height:%u", s_media_info.video_height[E_IPC_STREAM_VIDEO_MAIN]);
	LOG_DEBUG("video_freq:%u", s_media_info.video_freq[E_IPC_STREAM_VIDEO_MAIN]);
	LOG_DEBUG("video_codec:%d", s_media_info.video_codec[E_IPC_STREAM_VIDEO_MAIN]);

	LOG_DEBUG("audio_codec:%d", s_media_info.audio_codec[E_IPC_STREAM_AUDIO_MAIN]);
	LOG_DEBUG("audio_sample:%d", s_media_info.audio_sample[E_IPC_STREAM_AUDIO_MAIN]);
	LOG_DEBUG("audio_databits:%d", s_media_info.audio_databits[E_IPC_STREAM_AUDIO_MAIN]);
	LOG_DEBUG("audio_channel:%d", s_media_info.audio_channel[E_IPC_STREAM_AUDIO_MAIN]);
}

OPERATE_RET tuya_ipc_app_start(IN CONST TUYA_IPC_SDK_RUN_VAR_S *pRunInfo) {
	if (NULL == pRunInfo) {
		LOG_ERROR("start sdk para is NULL\n");
		return OPRT_INVALID_PARM;
	}

	OPERATE_RET ret = 0;
	STATIC BOOL_T s_ipc_sdk_started = FALSE;
	if (TRUE == s_ipc_sdk_started) {
		LOG_DEBUG("IPC SDK has started\n");
		return ret;
	}

	memcpy(&g_sdk_run_info, pRunInfo, SIZEOF(TUYA_IPC_SDK_RUN_VAR_S));

	/* 将码流信息保存到s_media_info，用于P2P的一些回调中匹配。客户可以根据自己的逻辑来实现。此处仅作参考
	 */
	IPC_APP_Set_Media_Info(&(g_sdk_run_info.media_info.media_info));
	LOG_INFO("%s: IPC_APP_Set_Media_Info over\n", get_time_string());

	//低功耗 优先开启P2P
	if (g_sdk_run_info.quick_start_info.enable) {
		int op_ret = TUYA_APP_Enable_P2PTransfer(&(g_sdk_run_info.p2p_info));
		if (op_ret < 0) {
			LOG_ERROR("create p2p start thread is error\n");
			return -1;
		}
		LOG_INFO("%s: TUYA_APP_Enable_P2PTransfer over\n", get_time_string());
	}

	// setup1:创建等待mqtt上线进程，mqtt上线后，再开启与网络相关的业务
	pthread_t mqtt_status_change_handle;
	int op_ret =
	    pthread_create(&mqtt_status_change_handle, NULL, tuya_ipc_sdk_mqtt_online_proc, NULL);
	if (op_ret < 0) {
		LOG_ERROR("create tuya_ipc_sdk_mqtt_online_proc  thread is error\n");
		return -1;
	}

	// setup2:init sdk
	TUYA_IPC_ENV_VAR_S env;
	memset(&env, 0, sizeof(TUYA_IPC_ENV_VAR_S));
	strcpy(env.storage_path, pRunInfo->iot_info.cfg_storage_path);
	strcpy(env.product_key, pRunInfo->iot_info.product_key);
	strcpy(env.uuid, pRunInfo->iot_info.uuid);
	strcpy(env.auth_key, pRunInfo->iot_info.auth_key);
	strcpy(env.dev_sw_version, pRunInfo->iot_info.dev_sw_version);
	strcpy(env.dev_serial_num, "tuya_ipc");
	// TODO:raw
	env.dev_raw_dp_cb = pRunInfo->dp_info.raw_dp_cmd_proc;
	env.dev_obj_dp_cb = pRunInfo->dp_info.common_dp_cmd_proc;
	env.dev_dp_query_cb = pRunInfo->dp_info.dp_query;
	env.status_changed_cb = pRunInfo->net_info.net_status_change_cb;
	env.upgrade_cb_info.upgrade_cb = pRunInfo->upgrade_info.upgrade_cb;
	env.gw_rst_cb = pRunInfo->iot_info.gw_reset_cb;
	env.gw_restart_cb = pRunInfo->iot_info.gw_restart_cb;
#if defined(QRCODE_ACTIVE_MODE) && (QRCODE_ACTIVE_MODE == 1)
	env.qrcode_active_cb = pRunInfo->qrcode_active_cb;
#endif
	env.dev_type = pRunInfo->iot_info.dev_type;
	ret = tuya_ipc_init_sdk(&env);
	if (OPRT_OK != ret) {
		LOG_ERROR("init sdk is error\n");
		return ret;
	}
	LOG_INFO("%s: tuya_ipc_init_sdk over\n", get_time_string());

	//设置日志等级
	tuya_ipc_set_log_attr(pRunInfo->debug_info.log_level, NULL);

	// ring buffer 创建。
	ret = TUYA_APP_Init_Ring_Buffer();
	if (OPRT_OK != ret) {
		LOG_ERROR("create ring buffer is error\n");
		return ret;
	}
	LOG_INFO("%s: TUYA_APP_Init_Ring_Buffer over\n", get_time_string());

	ret = tuya_ipc_start_sdk(pRunInfo->net_info.connect_mode, pRunInfo->debug_info.qrcode_token);
	if (OPRT_OK != ret) {
		LOG_ERROR("start sdk is error\n");
		return ret;
	}
	LOG_INFO("%s: tuya_ipc_start_sdk over\n", get_time_string());

	s_ipc_sdk_started = true;
	LOG_DEBUG("tuya ipc sdk start is complete\n");
	return ret;
}

OPERATE_RET TUYA_IPC_SDK_START(WIFI_INIT_MODE_E connect_mode, CHAR_T *p_token) {
	LOG_DEBUG("SDK Version:%s\r\n", tuya_ipc_get_sdk_info());
	const char *value;
	TUYA_IPC_SDK_RUN_VAR_S ipc_sdk_run_var = {0};

	memset(&ipc_sdk_run_var, 0, sizeof(ipc_sdk_run_var));
	/*certification information(essential)*/
	if (rk_param_get_int("tuya:use_ini_key", 0)) {
		const char *value;
		value = rk_param_get_string("tuya:product_key", NULL);
		strcpy(ipc_sdk_run_var.iot_info.product_key, value);
		value = rk_param_get_string("tuya:uuid", NULL);
		strcpy(ipc_sdk_run_var.iot_info.uuid, value);
		value = rk_param_get_string("tuya:auth_key", NULL);
		strcpy(ipc_sdk_run_var.iot_info.auth_key, value);
	} else {
		int index = 0;
		char *token;
		char vendor_data[256] = {0};
		if (rkvendor_read(VENDOR_TUYA_LICENSE_ID, vendor_data,
		                  sizeof(vendor_data) / sizeof(vendor_data[0]))) {
			LOG_INFO("rkvendor_read fail\n");
			system("aplay /etc/no_key.wav &");
			return -1;
		}
		LOG_INFO("vendor_data is %s\n", vendor_data);
		token = strtok(vendor_data, "\"");
		while (token != NULL) {
			LOG_DEBUG("%s\n", token);
			token = strtok(NULL, "\"");
			if (index == 2)
				strcpy(ipc_sdk_run_var.iot_info.product_key, token);
			if (index == 6)
				strcpy(ipc_sdk_run_var.iot_info.uuid, token);
			if (index == 10)
				strcpy(ipc_sdk_run_var.iot_info.auth_key, token);
			index++;
		}
	}
	LOG_INFO("%s: product_key is %s, uuid is %s, auth_key is %s\n", get_time_string(),
	         ipc_sdk_run_var.iot_info.product_key, ipc_sdk_run_var.iot_info.uuid,
	         ipc_sdk_run_var.iot_info.auth_key);
	strcpy(ipc_sdk_run_var.iot_info.dev_sw_version, IPC_APP_VERSION);
	strcpy(ipc_sdk_run_var.iot_info.cfg_storage_path, IPC_APP_STORAGE_PATH);

	// normal device
	ipc_sdk_run_var.iot_info.dev_type = NORMAL_POWER_DEV;
	// if needed, change to low power device
	// ipc_sdk_run_var.iot_info.dev_type= LOW_POWER_DEV;

	/*connect mode (essential)*/
	ipc_sdk_run_var.net_info.connect_mode = connect_mode;
	ipc_sdk_run_var.net_info.net_status_change_cb = IPC_APP_Net_Status_cb;
	if (p_token) {
		strcpy(ipc_sdk_run_var.debug_info.qrcode_token, p_token);
	}
	/* 0-5, the bigger, the more log */
	ipc_sdk_run_var.debug_info.log_level = 0;
	/*media info (essential)*/
	/* main stream(HD), video configuration*/
	/* NOTE
	FIRST:If the main stream supports multiple video stream configurations, set each item to the
	upper limit of the allowed configuration. SECOND:E_IPC_STREAM_VIDEO_MAIN must exist.It is the
	data source of SDK. please close the E_IPC_STREAM_VIDEO_SUB for only one stream*/
	ipc_sdk_run_var.media_info.media_info.channel_enable[E_IPC_STREAM_VIDEO_MAIN] =
	    TRUE; /* Whether to enable local HD video streaming */
	ipc_sdk_run_var.media_info.media_info.video_fps[E_IPC_STREAM_VIDEO_MAIN] =
	    rk_param_get_int("video.0:dst_frame_rate_num", 30);
	ipc_sdk_run_var.media_info.media_info.video_gop[E_IPC_STREAM_VIDEO_MAIN] =
	    rk_param_get_int("video.0:gop", 50);
	ipc_sdk_run_var.media_info.media_info.video_bitrate[E_IPC_STREAM_VIDEO_MAIN] =
	    TUYA_VIDEO_BITRATE_1_5M; /* Rate limit */
	ipc_sdk_run_var.media_info.media_info.video_width[E_IPC_STREAM_VIDEO_MAIN] =
	    rk_param_get_int("video.0:width", 1920);
	ipc_sdk_run_var.media_info.media_info.video_height[E_IPC_STREAM_VIDEO_MAIN] =
	    rk_param_get_int("video.0:height", 1080);
	ipc_sdk_run_var.media_info.media_info.video_freq[E_IPC_STREAM_VIDEO_MAIN] =
	    90000; /* Clock frequency */
	value = rk_param_get_string("video.0:output_data_type", "H.264");
	if (!strcmp(value, "H.264"))
		ipc_sdk_run_var.media_info.media_info.video_codec[E_IPC_STREAM_VIDEO_MAIN] =
		    TUYA_CODEC_VIDEO_H264;
	else
		ipc_sdk_run_var.media_info.media_info.video_codec[E_IPC_STREAM_VIDEO_MAIN] =
		    TUYA_CODEC_VIDEO_H265;

	// /* substream(HD), video configuration */
	// /* Please note that if the substream supports multiple video stream configurations, please
	// set each item to the upper limit of the allowed configuration. */
	// ipc_sdk_run_var.media_info.media_info.channel_enable[E_IPC_STREAM_VIDEO_SUB] = TRUE;     /*
	// Whether to enable local SD video stream */
	// ipc_sdk_run_var.media_info.media_info.video_fps[E_IPC_STREAM_VIDEO_SUB] = 30;  /* FPS */
	// ipc_sdk_run_var.media_info.media_info.video_gop[E_IPC_STREAM_VIDEO_SUB] = 30;  /* GOP */
	// ipc_sdk_run_var.media_info.media_info.video_bitrate[E_IPC_STREAM_VIDEO_SUB] =
	// TUYA_VIDEO_BITRATE_512K; /* Rate limit */
	// ipc_sdk_run_var.media_info.media_info.video_width[E_IPC_STREAM_VIDEO_SUB] = 640; /* Single
	// frame resolution of width */
	// ipc_sdk_run_var.media_info.media_info.video_height[E_IPC_STREAM_VIDEO_SUB] = 360;/* Single
	// frame resolution of height */
	// ipc_sdk_run_var.media_info.media_info.video_freq[E_IPC_STREAM_VIDEO_SUB] = 90000; /* Clock
	// frequency */ ipc_sdk_run_var.media_info.media_info.video_codec[E_IPC_STREAM_VIDEO_SUB] =
	// TUYA_CODEC_VIDEO_H264; /* Encoding format */

	/* Audio stream configuration.
	Note: The internal P2P preview, cloud storage, and local storage of the SDK are all use
	E_IPC_STREAM_AUDIO_MAIN data. */
	ipc_sdk_run_var.media_info.media_info.channel_enable[E_IPC_STREAM_AUDIO_MAIN] =
	    TRUE; /* Whether to enable local sound collection */
	value = rk_param_get_string("audio.0:encode_type", "G711A");
	if (!strcmp(value, "G711A"))
		ipc_sdk_run_var.media_info.media_info.audio_codec[E_IPC_STREAM_AUDIO_MAIN] =
		    TUYA_CODEC_AUDIO_G711A;
	else if (!strcmp(value, "PCM"))
		ipc_sdk_run_var.media_info.media_info.audio_codec[E_IPC_STREAM_AUDIO_MAIN] =
		    TUYA_CODEC_AUDIO_PCM;
	else
		LOG_ERROR("audio_codec %s is unsupport\n", value);

	int sample_rate = rk_param_get_int("audio.0:sample_rate", 8000);
	if (sample_rate == 8000)
		ipc_sdk_run_var.media_info.media_info.audio_sample[E_IPC_STREAM_AUDIO_MAIN] =
		    TUYA_AUDIO_SAMPLE_8K;
	else if (sample_rate == 16000)
		ipc_sdk_run_var.media_info.media_info.audio_sample[E_IPC_STREAM_AUDIO_MAIN] =
		    TUYA_AUDIO_SAMPLE_16K;
	else
		LOG_ERROR("audio_sample %d is unsupport\n", sample_rate);

	value = rk_param_get_string("audio.0:format", "S16");
	if (!strcmp(value, "S16"))
		ipc_sdk_run_var.media_info.media_info.audio_databits[E_IPC_STREAM_AUDIO_MAIN] =
		    TUYA_AUDIO_DATABITS_16;
	else
		ipc_sdk_run_var.media_info.media_info.audio_databits[E_IPC_STREAM_AUDIO_MAIN] =
		    TUYA_AUDIO_DATABITS_8;

	int channels = rk_param_get_int("audio.0:channels", 2);
	if (channels == 2)
		ipc_sdk_run_var.media_info.media_info.audio_channel[E_IPC_STREAM_AUDIO_MAIN] =
		    TUYA_AUDIO_CHANNEL_STERO;
	else
		ipc_sdk_run_var.media_info.media_info.audio_channel[E_IPC_STREAM_AUDIO_MAIN] =
		    TUYA_AUDIO_CHANNEL_MONO;

	ipc_sdk_run_var.media_info.media_info.audio_fps[E_IPC_STREAM_AUDIO_MAIN] =
	    25; /* Fragments per second */

	// /*local storage (customer whether enable or not)*/
	// ipc_sdk_run_var.local_storage_info.enable = 1;
	// ipc_sdk_run_var.local_storage_info.max_event_num_per_day = 500;
	// ipc_sdk_run_var.local_storage_info.skills = 0;//0 means all skills
	// ipc_sdk_run_var.local_storage_info.sd_status_cb = tuya_ipc_sd_status_upload ;
	// strcpy(ipc_sdk_run_var.local_storage_info.storage_path, IPC_APP_SD_BASE_PATH);

	// /*cloud storage (custome whether enable or not)*/
	// /*if no AES, ipc_sdk_run_var.aes_hw_info.aes_fun.* can equal NULL;*/
	// ipc_sdk_run_var.cloud_storage_info.enable = TRUE;
	// ipc_sdk_run_var.cloud_storage_info.en_audio_record = TRUE;
	// ipc_sdk_run_var.cloud_storage_info.pre_record_time = -1; //set -1 to ignore it. default 2
	// seconds.

	/*p2p function (essential)*/
	ipc_sdk_run_var.p2p_info.enable = TRUE;
	ipc_sdk_run_var.p2p_info.is_lowpower = FALSE;
	ipc_sdk_run_var.p2p_info.max_p2p_client = 5;
	ipc_sdk_run_var.p2p_info.live_mode = TRANS_DEFAULT_HIGH; // TRANS_DEFAULT_STANDARD;
	ipc_sdk_run_var.p2p_info.transfer_event_cb = __TUYA_APP_p2p_event_cb;
	ipc_sdk_run_var.p2p_info.rev_audio_cb = __TUYA_APP_rev_audio_cb;

	// /*AI detect (custome whether enable or not)*/
	// ipc_sdk_run_var.cloud_ai_detct_info.enable = 1;

	// /*door bell (custome whether enable or not)*/
	// ipc_sdk_run_var.video_msg_info.enable = 1;
	// ipc_sdk_run_var.video_msg_info.type = MSG_BOTH;
	// ipc_sdk_run_var.video_msg_info.msg_duration = 10;

	/*dp function(essential)*/
	ipc_sdk_run_var.dp_info.dp_query = IPC_APP_handle_dp_query_objs;
	ipc_sdk_run_var.dp_info.raw_dp_cmd_proc = IPC_APP_handle_raw_dp_cmd_objs;
	ipc_sdk_run_var.dp_info.common_dp_cmd_proc = IPC_APP_handle_dp_cmd_objs;

	/*upgrade function(essential)*/
	ipc_sdk_run_var.upgrade_info.enable = true;
	ipc_sdk_run_var.upgrade_info.upgrade_cb = IPC_APP_Upgrade_Inform_cb;

	ipc_sdk_run_var.iot_info.gw_reset_cb = IPC_APP_Reset_System_CB;
	ipc_sdk_run_var.iot_info.gw_restart_cb = IPC_APP_Restart_Process_CB;

	ipc_sdk_run_var.quick_start_info.enable = false;

	// 	/*QR-active function(essential)*/
	// #if defined(QRCODE_ACTIVE_MODE) && (QRCODE_ACTIVE_MODE == 1)
	// 	ipc_sdk_run_var.qrcode_active_cb = IPC_APP_qrcode_shorturl_cb;
	// #endif

	OPERATE_RET ret;
	ret = tuya_ipc_app_start(&ipc_sdk_run_var);
	if (ret != 0) {
		LOG_DEBUG("ipc sdk start fail,please check run parameter, ret=%d\n", ret);
	}
	LOG_INFO("%s: tuya_ipc_app_start over\n", get_time_string());

	return ret;
}

int rk_tuya_init() {
	fw_at("AT+DEL_KEEPALIVE");
	set_filter("ICMP", "1");
	set_filter("DES", "999");
	set_filter("SRC", "999");
	enable_filter();
	LOG_INFO("%s: DEL_KEEPALIVE over\n", get_time_string());

	WIFI_INIT_MODE_E mode = WIFI_INIT_AUTO;
	OPERATE_RET ret = OPRT_OK;
	ret = TUYA_IPC_SDK_START(mode, NULL);
	if (ret != OPRT_OK) {
		LOG_ERROR("TUYA_IPC_SDK_START fail\n");
		return ret;
	}
	LOG_INFO("%s: TUYA_IPC_SDK_START over\n", get_time_string());

	// IPC_APP_Init_Media_Task();
	audio_handle = tuya_ipc_ring_buffer_open(0, 0, E_IPC_STREAM_AUDIO_MAIN, E_RBUF_WRITE);
	video_handle = tuya_ipc_ring_buffer_open(0, 0, E_IPC_STREAM_VIDEO_MAIN, E_RBUF_WRITE);
	LOG_INFO("%s: tuya_ipc_ring_buffer_open over\n", get_time_string());

	return 0;
}

int rk_tuya_deinit() {
	tuya_ipc_ring_buffer_close(audio_handle);
	tuya_ipc_ring_buffer_close(video_handle);

	return 0;
}

int rk_tuya_put_frame(IN CONST IPC_STREAM_E channel, IN CONST MEDIA_FRAME_S *p_frame) {
	// LOG_DEBUG("Put Frame. Channel:%d type:%d size:%u pts:%llu ts:%llu\n", channel,
	//           p_frame->type, p_frame->size, p_frame->pts, p_frame->timestamp);
	int ret;
	if (!s_mqtt_status)
		return 0;

	if (channel == E_IPC_STREAM_VIDEO_MAIN) {
		ret = tuya_ipc_ring_buffer_append_data(video_handle, p_frame->p_buf, p_frame->size,
		                                       p_frame->type, p_frame->pts);
	} else {
		ret = tuya_ipc_ring_buffer_append_data(audio_handle, p_frame->p_buf, p_frame->size,
		                                       p_frame->type, p_frame->pts);
	}

	if (ret != OPRT_OK) {
		LOG_INFO("Put Frame Fail.%d Channel:%d type:%d size:%u pts:%llu ts:%llu\n", ret, channel,
		         p_frame->type, p_frame->size, p_frame->pts, p_frame->timestamp);
	}

	return ret;
}

int rk_tuya_push_video(unsigned char *buffer, unsigned int buffer_size, int64_t present_time,
                       int nal_type) {
	memset(&video_frame, 0, sizeof(video_frame));
	if (nal_type)
		video_frame.type = E_VIDEO_I_FRAME;
	else
		video_frame.type = E_VIDEO_PB_FRAME;
	video_frame.p_buf = buffer;
	video_frame.size = buffer_size;
	video_frame.pts = present_time;
	rk_tuya_put_frame(E_IPC_STREAM_VIDEO_MAIN, &video_frame);

	return 0;
}

int rk_tuya_push_audio(unsigned char *buffer, unsigned int buffer_size, int64_t present_time) {
	// LOG_INFO("buffer_size is %d\n", buffer_size);
	memset(&audio_frame, 0, sizeof(audio_frame));
	audio_frame.type = E_AUDIO_FRAME;
	audio_frame.p_buf = buffer;
	audio_frame.size = buffer_size;
	audio_frame.pts = present_time;
	rk_tuya_put_frame(E_IPC_STREAM_AUDIO_MAIN, &audio_frame);

	return 0;
}

VOID tuya_ipc_get_snapshot_cb(char *pjbuf, int *size) {
	printf("+++%s\n", __FUNCTION__);
	// get_motion_snapshot(pjbuf,size);
}

VOID tuya_ipc_doorbell_event(char *action) {
	int status = 0;

	if (0 == memcmp(action, "accept", 6)) {
		status = 0;
	} else if (0 == memcmp(action, "stop", 4)) {
		status = 1;
	} else if (0 == memcmp(action, "heartbeat", 9)) {
		status = 2;
	}
	// doorbell_mqtt_handler(status);
	return;
}
