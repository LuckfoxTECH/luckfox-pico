/*********************************************************************************
 *Copyright(C),2017, 涂鸦科技 www.tuya.comm
 *FileName:    tuya_ipc_api.h
 **********************************************************************************/

#ifndef _TUYA_IPC_API_H
#define _TUYA_IPC_API_H

#ifdef __cplusplus
extern "C" {
#endif
#if defined(WIFI_GW) && (WIFI_GW == 1)
#include "tuya_cloud_wifi_defs.h"
#endif
#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_error_code.h"
#include "tuya_cloud_types.h"
#include "tuya_ipc_skill.h"

#include "cJSON.h"
#include <stdio.h>
#include <time.h>

typedef enum { WIFI_INIT_AUTO, WIFI_INIT_AP, WIFI_INIT_DEBUG, WIFI_INIT_NULL } WIFI_INIT_MODE_E;

typedef VOID (*TUYA_CMD_DISPATCH)(IN INT_T cmd, IN VOID *param);
typedef VOID (*TUYA_QUERY_DISPATCH)(IN INT_T cmd, IN VOID *param);
typedef VOID (*TUYA_RST_INFORM_CB)(GW_RESET_TYPE_E from);
typedef VOID (*TUYA_RESTART_INFORM_CB)(VOID);

#define IPC_STORAGE_PATH_LEN 64
#define IPC_PRODUCT_KEY_LEN 16
#define IPC_UUID_LEN 25
#define IPC_AUTH_KEY_LEN 32
#define IPC_SW_VER_LEN 20
#define IPC_SERIAL_NUM_LEN 32
#define IPC_FACTORY_ID_LEN 4
#define IPC_P2P_ID_LEN 25
#define IPC_AES_ENCRYPT_KEY_LEN 16
#define IPC_NOTIFY_INFO_LEN 16
#define NOTIFICATION_MOTION_DETECTION 115
#define NOTIFICATION_CONTENT_SIZE_MAX (150 * 1024)

/**
 * \brief notification type of the event, currently only JPEG and PNG are supported
 * \enum NOTIFICATION_CONTENT_TYPE_E
 */
typedef enum {
	NOTIFICATION_CONTENT_MP4 = 0, /* mp4 for video, NOT supported now */
	NOTIFICATION_CONTENT_JPEG,    /* jpeg for snapshot */
	NOTIFICATION_CONTENT_PNG,     /* png for snapshot */
	NOTIFICATION_CONTENT_AUDIO,   /* audio for video msg */
	NOTIFICATION_CONTENT_MAX,
} NOTIFICATION_CONTENT_TYPE_E;

typedef struct {
	CHAR_T *data;
	INT_T len;
	CHAR_T secret_key[IPC_AES_ENCRYPT_KEY_LEN + 1];
	NOTIFICATION_CONTENT_TYPE_E type;
} NOTIFICATION_UNIT_T;

typedef enum {
	NOTIFICATION_NAME_MOTION = 0,          /* 移动侦测 */
	NOTIFICATION_NAME_DOORBELL = 1,        /* 门铃按下 */
	NOTIFICATION_NAME_DEV_LINK = 2,        /* IOT设备联动触发 */
	NOTIFICATION_NAME_PASSBY = 3,          /* 正常经过 */
	NOTIFICATION_NAME_LINGER = 4,          /* 异常逗留 */
	NOTIFICATION_NAME_MESSAGE = 5,         /* 留言信息 */
	NOTIFICATION_NAME_CALL_ACCEPT = 6,     /* 门铃接听 */
	NOTIFICATION_NAME_CALL_NOT_ACCEPT = 7, /* 门铃未接听 */
	NOTIFICATION_NAME_CALL_REFUSE = 8,     /* 门铃拒绝接听 */
	NOTIFICATION_NAME_HUMAN = 9,           /* 人形检测 */
	NOTIFICATION_NAME_PCD = 10,            /* 宠物检测 */
	NOTIFICATION_NAME_CAR = 11,            /* 车辆检测 */
	NOTIFICATION_NAME_BABY_CRY = 12,       /* 婴儿哭声 */
	NOTIFICATION_NAME_ABNORMAL_SOUND = 13, /* 声音异响 */
	NOTIFICATION_NAME_FACE = 14,           /* 人脸检测 */
	NOTIFICATION_NAME_ANTIBREAK = 15,      /* 强拆告警 */
	NOTIFICATION_NAME_RECORD_ONLY = 16,    /* 占位，无意义 */
	NOTIFICATION_NAME_IO_ALARM = 17,       /* 本地IO设备触发 */
	NOTIFY_TYPE_MAX
} NOTIFICATION_NAME_E;

typedef enum {
	NOTIFY_TEXT_KEY_POWER,
	NOTIFY_TEXT_KEY_TEMPERATURE,
	NOTIFY_TEXT_KEY_MAX
} NOTIFY_TEXT_KEY_E;

#define TEXT_VALUE_LEN 16
typedef struct {
	NOTIFY_TEXT_KEY_E key;
	CHAR_T value[TEXT_VALUE_LEN];
} TEXT_KV_T;

typedef struct {
	INT_T text_cnt;
	TEXT_KV_T text[0];
} NOTIFY_TEXT_T;

typedef enum {
	DOORBELL_NORMAL, /* 普通门铃，推送图片 */
	DOORBELL_AC,     /* 直供电门铃，推送p2p */
	DOORBELL_TYPE_MAX,
} DOORBELL_TYPE_E;

typedef struct {
	INT_T unit_count;
	NOTIFICATION_NAME_E name;
	CHAR_T *extra_data;               // default:NULL
	CHAR_T type[IPC_NOTIFY_INFO_LEN]; // jpg:"image",video:""
	CHAR_T with[IPC_NOTIFY_INFO_LEN]; // default "resources"
	NOTIFICATION_UNIT_T unit[0];
} NOTIFICATION_DATA_GENERIC_T;

typedef enum {
	E_IPC_UNREGISTERED = 0,
	E_IPC_REGISTERED = 1,
	E_IPC_ACTIVEATED = 2 // after successful register and active process, device is in this status
} IPC_REGISTER_STATUS;
/**
 * \brief Status for IPC
 * \enum IPC_GW_STATUS_E
 */
typedef enum {
	IPC_GW_UNREGISTERED = 0,
	IPC_GW_REGISTERED,
	IPC_GW_ACTIVED,
	IPC_GW_MQTT_ONLINE,
	IPC_GW_MQTT_OFFLINE,
} IPC_GW_STATUS_E;

/**
 * \brief IPC环境变量结构体
 * \struct TUYA_IPC_ENV_VAR_S
 */
typedef struct {
	CHAR_T storage_path[IPC_STORAGE_PATH_LEN +
	                    1]; /* path to store TUYA database files, readable and writable */

	CHAR_T product_key[IPC_PRODUCT_KEY_LEN + 1]; /* one product key(PID) for one product  */
	CHAR_T
	uuid[IPC_UUID_LEN + 1]; /* one uuid-authkey pair for one device, to register on TUYA coud */
	CHAR_T auth_key[IPC_AUTH_KEY_LEN + 1];
	CHAR_T p2p_id[IPC_P2P_ID_LEN + 1]; /* this param is not used now */

	CHAR_T dev_sw_version[IPC_SW_VER_LEN + 1]; /* software version with format x1.x2.x3.x4.x5.x6,
	                                              five dots maximum, zero dots minimum */
	CHAR_T dev_serial_num[IPC_SERIAL_NUM_LEN + 1]; /* this param is not used in IPC now */

	DEV_OBJ_DP_CMD_CB
	dev_obj_dp_cb; /* dp cmd callback function, which hanles cmd from TUYA cloud and APP */
	DEV_DP_QUERY_CB dev_dp_query_cb; /* dp query callback function */

	GW_STATUS_CHANGED_CB status_changed_cb; /* status change callback function for developer, could
	                                           be NULL if not needed */
	GW_UG_INFORM_CB
	gw_ug_cb; /* OTA callback function, triggered by uprading from APP and TUYA cloud */
	TUYA_RST_INFORM_CB
	gw_rst_cb; /* reset callback fucntion, triggered when user unbind device from a account */
	TUYA_RESTART_INFORM_CB gw_restart_cb; /* restart callback function */

	BOOL_T mem_save_mode; /* woking in low memory mode */

	VOID *p_reserved;
} TUYA_IPC_ENV_VAR_S;

#define NOTIFICATION_MOTION_DETECTION 115

/**
 * \brief direct connect mode, compared to wifi EZ mode, get ssid/psw via QR etc.
 * \enum TUYA_IPC_DIRECT_CONNECT_TYPE_E
 */
typedef enum {
	TUYA_IPC_DIRECT_CONNECT_QRCODE = 0,
	TUYA_IPC_DIRECT_CONNECT_MAX,
} TUYA_IPC_DIRECT_CONNECT_TYPE_E;

typedef enum { REGION_MIN = 0, REGION_CN, REGION_EU, REGION_US, REGION_MAX } TUYA_IPC_REGION_E;

typedef enum {
	E_QRCODE_CONN_INVAILD,
	E_QRCODE_CONN_WAIT_TOEKN,
	E_QRCODE_CONN_ACTIVING,
	E_QRCODE_CONN_ACTIVED,
	E_QRCODE_CONN_STATUS_MAX
} TUYA_IPC_QRCODE_CONN_STATUS;

/**
 * \fn CHAR *tuya_ipc_get_sdk_info(VOID)
 * \brief return sdk info, sdk version etc.
 * \return CHAR *
 */
CHAR_T *tuya_ipc_get_sdk_info(VOID);

/**
 * \fn OPERATE_RET tuya_ipc_init_sdk(IN CONST TUYA_IPC_ENV_VAR_S *p_var)
 * \brief initialize tuya sdk for embeded devices
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_init_sdk(IN CONST TUYA_IPC_ENV_VAR_S *p_var);

/**
 * \fn OPERATE_RET tuya_ipc_set_ap_info(IN CONST CHAR_T *ssid, IN CONST CHAR_T *passwd)
 * \brief set ap info when start sdk
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_set_ap_info(IN CONST CHAR_T *ssid, IN CONST CHAR_T *passwd);

/**
 * \fn tuya_ipc_save_ap_info(IN CONST CHAR_T *ssid, IN CONST CHAR_T *passwd)
 * \brief save ap info when get info from app
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_save_ap_info(IN CONST CHAR_T *ssid, IN CONST CHAR_T *passwd);

/**
 * \fn tuya_ipc_reconnect_wifi()
 * \brief reconnect wifi
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_reconnect_wifi();

/**
 * \fn OPERATE_RET tuya_ipc_start_sdk(IN CONST TUYA_WIFI_DEV_INFO_S *p_wifi_info)
 * \brief start ipc as WIFI_DEVICE
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_start_sdk(IN CONST WIFI_INIT_MODE_E wifi_mode, IN CONST char *pToken);

/**
 * \fn OPERATE_RET tuya_ipc_upgrade_sdk(     IN CONST FW_UG_S *fw,\
 *                                     IN CONST GET_FILE_DATA_CB get_file_cb,\
 *                                     IN CONST UPGRADE_NOTIFY_CB upgrd_nofity_cb,\
 *                                     IN PVOID pri_data)
 * \brief OTA, upgrade via TUYA SDK
 * \param[in] fw firmware: infomation
 * \param[in] get_file_cb: callback function during downloading fw
 * \param[in] upgrd_nofity_cb: callback function when downloading fw fininsh
 * \param[in] pri_data: data transferred between callback functions
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_upgrade_sdk(IN CONST FW_UG_S *fw, IN CONST GET_FILE_DATA_CB get_file_cb,
                                 IN CONST UPGRADE_NOTIFY_CB upgrd_nofity_cb, IN PVOID_T pri_data);

/**
 * \fn OPERATE_RET tuya_ipc_get_register_status(VOID)
 * \brief get register status on tuya cloud
 * \return OPERATE_RET
 */
IPC_REGISTER_STATUS tuya_ipc_get_register_status(VOID);

/**
 * \fn OPERATE_RET tuya_ipc_get_service_time/tuya_ipc_get_service_time_force
 * \brief Get UTC time and timezone from Tuya cloud service.
          tuya_ipc_get_service_time_force will block until service time is synchronized.
 * \return return OPRT_OK if service time is avaliable
 */
OPERATE_RET tuya_ipc_get_service_time(OUT TIME_T *time_utc, OUT INT_T *time_zone);
OPERATE_RET tuya_ipc_get_service_time_force(OUT TIME_T *time_utc, OUT INT_T *time_zone);

/**
 * \fn OPERATE_RET tuya_ipc_check_in_dls(IN TIME_T time_utc, OUT BOOL * pIsDls)
 * \brief check if specified utc time is in daylight saving time夏令时
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_check_in_dls(IN TIME_T time_utc, OUT BOOL_T *pIsDls);

/**
 * \fn OPERATE_RET tuya_ipc_get_local_time(IN TIME_T inTime, OUT struct tm *localTime)
 * \brief transfer utc time to local time with timezone. if inTime=0, current utc time will be used
 * automatically. \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_get_local_time(IN TIME_T inTime, OUT struct tm *localTime);

/**
 * \fn OPERATE_RET tuya_ipc_get_utc_tm(OUT struct tm *time_utc)
 * \brief get utc time in tm format
 * \return OPERATE_RET
 */

OPERATE_RET tuya_ipc_get_utc_tm(OUT struct tm *localTime);

/**
 * \fn OPERATE_RET tuya_ipc_get_utc_time(OUT TIME_T *time_utc)
 * \brief get utc time
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_get_utc_time(OUT TIME_T *time_utc);

/**
 * \fn OPERATE_RET tuya_ipc_get_tm_with_timezone_dls(OUT struct tm *localTime)
 * \brief get time with timezone and dls(summer timezone)
 * \      recommended API for OSD
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_get_tm_with_timezone_dls(OUT struct tm *localTime);

/**
 * \fn OOPERATE_RET tuya_ipc_dp_report(IN CONST CHAR_T *dev_id, IN BYTE_T dp_id, IN DP_PROP_TP_E
 * type, IN VOID * pVal, IN CONST UINT_T cnt) \brief report dp(data point) to tuya cloud, always
 * report in async way \return OPERATE_RET
 */

/**
 * \fn OPERATE_RET tuya_ipc_dp_report_async/tuya_ipc_dp_report_sync/tuya_ipc_dp_report
 * \brief report dp to tuya cloud, in aync/sync way.
 *
 *        tuya_ipc_dp_report_async will not report for same dp_id and value
 *        tuya_ipc_dp_report_sync will try for timeout seconds to report
 *        tuya_ipc_dp_report always reports till succeed
 *        tuya_ipc_dp_report_raw_sync report raw data, try for <timeout> senconds
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_dp_report_async(IN CONST CHAR_T *dev_id, IN BYTE_T dp_id, IN DP_PROP_TP_E type,
                                     IN VOID *pVal, IN CONST UINT_T cnt);
OPERATE_RET tuya_ipc_dp_report_sync(IN CONST CHAR_T *dev_id, IN BYTE_T dp_id, IN DP_PROP_TP_E type,
                                    IN VOID *pVal, IN CONST UINT_T cnt, IN CONST UINT_T timeout);
OPERATE_RET tuya_ipc_dp_report(IN CONST CHAR_T *dev_id, IN BYTE_T dp_id, IN DP_PROP_TP_E type,
                               IN VOID *pVal, IN CONST UINT_T cnt);
OPERATE_RET tuya_ipc_dp_report_raw_sync(IN CONST CHAR_T *dev_id, IN BYTE_T dp_id, IN VOID *pVal,
                                        IN CONST UINT_T len, IN CONST UINT_T timeout);

/**
 * \fn OPERATE_RET tuya_ipc_get_wakeup_data(INOUT BYTE *wakeup_data_arr, INOUT UINT *p_len)
 * \brief get wakeup data for low-power ipc, which is decided by tuya SDK and is used to check if
 * device need to be waked up \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_get_wakeup_data(INOUT BYTE_T *wakeup_data_arr, INOUT UINT_T *p_len);

/**
 * \fn OPERATE_RET tuya_ipc_get_heartbeat_data(INOUT BYTE *heartbeat_data_arr, INOUT UINT *p_len)
 * \param[in out] get heartbeat data for loe-power ipc, which is devided by tuya SDK and is used to
 * send to tuya cloud to declaim existence \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_get_heartbeat_data(INOUT BYTE_T *heartbeat_data_arr, INOUT UINT_T *p_len);

/**
 * \fn OPERATE_RET tuya_ipc_qrcode_recon_init(VOID)
 * \brief clear QRcode connect flag,so that you can QRcode again to reconnect wifi
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_qrcode_recon_init(VOID);
/**
 * \fn OPERATE_RET tuya_ipc_direct_connect(IN CONST CHAR *p_str, IN CONST
 * TUYA_IPC_DIRECT_CONNECT_TYPE_E source) \brief use this API to connect to specific wifi and
 * register to tuya cloud, data can be obtain via QR code e.g. \param[in] p_str in format:
 * {"p":"password_of_wifi","s":"ssid_of_wifi","t":"token_to_register"} \param[in] source only
 * TUYA_IPC_DIRECT_CONNECT_QRCODE is supported now \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_direct_connect(IN CONST CHAR_T *p_str,
                                    IN CONST TUYA_IPC_DIRECT_CONNECT_TYPE_E source);

/**
 * \fn OPERATE_RET tuya_ipc_get_mqtt_status(VOID)
 * \brief get mqtt connection status, mqtt is used by tuya SDK to commnicate with tuya cloud
 * \return 0:offline 1:online
 */
OPERATE_RET tuya_ipc_get_mqtt_status(VOID);

/**
 * \fn OPERATE_RET tuya_ipc_book_wakeup_topic(VOID)
 * \brief book wakeup topic foe low-power ipc before power down
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_book_wakeup_topic(VOID);

/**
 * \fn INT tuya_ipc_get_mqtt_socket_fd(VOID)
 * \brief get mqtt socket fd which is used t send heartbeat data after power down
 * \return INT
 */
INT_T tuya_ipc_get_mqtt_socket_fd(VOID);

/**
 * \fn OPERATE_RET tuya_ipc_set_log_attr(IN CONST INT log_level, CHAR *filename)
 * \brief set tuya SDK log level and destination
 * \param[in] log_level 0-5, the bigger, the more log
 * \param[in] filename if need to print to filesystem
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_set_log_attr(IN CONST INT_T log_level, CHAR_T *filename);

/**
 * \fn OPERATE_RET tuya_ipc_send_custom_mqtt_msg(IN CONST UINT protocol, IN CONST CHAR *p_data)
 * \brief send customized mqtt message to tuya cloud
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_send_custom_mqtt_msg(IN CONST UINT_T protocol, IN CONST CHAR_T *p_data);

/**
 * \fn OPERATE_RET tuya_ipc_set_service_time(IN TIME_T new_time_utc)
 * \brief set time of tuya SDK
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_set_service_time(IN TIME_T new_time_utc);

/**
 * \fn OPERATE_RET tuya_ipc_report_living_msg(uint32_t error_code, int force, int timeout)
 * \brief report living message to tuya APP
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_report_living_msg(IN UINT_T error_code, IN UINT_T force, IN UINT_T timeout);

/**
 * \fn OPERATE_RET tuya_ipc_get_free_ram(IN ULONG *free)
 * \brief get free memory of system in KB, only standard linux supported
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_get_free_ram(IN ULONG_T *free);

/**
 * \fn OPERATE_RET tuya_ipc_notify_motion_detect
 * \brief send a motion-detecttion alarm to tuya cloud and APP
 * \param[in] snap_buffer: address of current snapshot
 * \param[in] snap_size: size fo snapshot, in Byte
 * \param[in] type: snapshot file type, jpeg or png
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_notify_motion_detect(IN CONST CHAR_T *snap_buffer, IN CONST UINT_T snap_size,
                                          IN CONST NOTIFICATION_CONTENT_TYPE_E type);

/**
 * \fn OPERATE_RET tuya_ipc_notify_door_bell_press
 * \brief send a doorbell pressing message to tuya cloud and APP
 * \param[in] snap_buffer: address of current snapshot
 * \param[in] snap_size: size fo snapshot, in Byte
 * \param[in] type: snapshot file type, jpeg or png
 * \return OPERATE_RET
 * \NOTE: this API will be abandoned later. Use tuya_ipc_door_bell_press instead
 */
OPERATE_RET tuya_ipc_notify_door_bell_press(IN CONST CHAR_T *snap_buffer, IN CONST UINT_T snap_size,
                                            IN CONST NOTIFICATION_CONTENT_TYPE_E type);

/**
 * \fn OPERATE_RET tuya_ipc_door_bell_press
 * \brief send a doorbell pressing message to tuya cloud and APP
 * \param[in] doorbell_type: DOORBELL_NORMAL or DOORBELL_AC
 * \param[in] snap_buffer: address of current snapshot
 * \param[in] snap_size: size fo snapshot, in Byte
 * \param[in] type: snapshot file type, jpeg or png
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_door_bell_press(IN CONST DOORBELL_TYPE_E doorbell_type,
                                     IN CONST CHAR_T *snap_buffer, IN CONST UINT_T snap_size,
                                     IN CONST NOTIFICATION_CONTENT_TYPE_E type);

/**
 * \fn OPERATE_RET tuya_ipc_notify_with_event
 * \brief send a editable alarm to tuya cloud and APP
 * \param[in] snap_buffer: address of current snapshot
 * \param[in] snap_size: size fo snapshot, in Byte
 * \param[in] type: snapshot file type, jpeg or png
 * \param[in] name: editable event type, NOTIFICATION_NAME_E
 * \return OPERATE_RET
 */

OPERATE_RET tuya_ipc_notify_with_event(IN CONST CHAR_T *snap_buffer, IN CONST UINT_T snap_size,
                                       IN CONST NOTIFICATION_CONTENT_TYPE_E type,
                                       IN CONST NOTIFICATION_NAME_E name);

/**
 * \fn VOID tuya_ipc_skill_enable
 * \brief before tuya_ipc_upload_skills, use this API to enable device skills
 */
OPERATE_RET tuya_ipc_skill_enable(TUYA_IPC_SKILL_E skill, tuya_ipc_skill_param_u *param);

/**
 * \fn VOID tuya_ipc_upload_skills
 * \brief after all module initialization, use this API to update device skills
 */
VOID tuya_ipc_upload_skills(VOID);

/**
 * \fn OPERATE_RET tuya_ipc_upgrade_progress_report
 * \brief send a upgrade progress to tuya cloude and app
 * \param[in] percent: upgrade progress percent , valid value [0,100]
 * \return SUCCESS -- OPERATE_RET , FAIL -- COMM ERROR
 */
OPERATE_RET tuya_ipc_upgrade_progress_report(IN UINT_T percent);

/**
 * \fn OPERATE_RET tuya_ipc_get_qrcode
 * \brief get qrcode short url from tuya cloud
 * \param[in] appId : which app to use the qrcode
 * \param[in] p_short_url:buf to store the url
 * \param[in] len : the size of p_short_url
 * \return SUCCESS -- OPERATE_RET , FAIL -- COMM ERROR
 */
OPERATE_RET tuya_ipc_get_qrcode(CHAR_T *appId, CHAR_T *p_short_url, INT_T len);

/**
 * \fn OPERATE_RET tuya_ipc_set_region
 * \brief set a tmp region to connect to mqtt and get qrcode
 * \param[in] region :     REGION_CN,REGION_EU,REGION_US,
 * \return SUCCESS -- OPERATE_RET , FAIL -- COMM ERROR
 */
OPERATE_RET tuya_ipc_set_region(TUYA_IPC_REGION_E region);

/**
 * \fn TUYA_IPC_QRCODE_CONN_STATUS tuya_ipc_get_qrcode_direct_conn_status
 * \brief get status of qrcode direct connect
 * \return TUYA_IPC_QRCODE_CONN_STATUS
 */
TUYA_IPC_QRCODE_CONN_STATUS tuya_ipc_get_qrcode_direct_conn_status();

OPERATE_RET tuya_ipc_thing_config_start(IN CHAR_T *ssid, CHAR_T *passwd, IN BOOL_T update_token,
                                        IN UINT_T timeout);

/**
 * \fn OPERATE_RET tuya_ipc_report_p2p_msg
 * \brief send a msg to enable p2p func
 * \return SUCCESS -- OPERATE_RET , FAIL -- COMM ERROR
 */
OPERATE_RET tuya_ipc_report_p2p_msg();

VOID tuya_ipc_thing_config_stop();

/**
 * \fn OPERATE_RET tuya_ipc_local_key_get
 * \brief Get signatrue key from ipc sdk.
 * \ make destKeyBuf len >=17;
 * \return return OPRT_OK if get key success
 */

OPERATE_RET tuya_ipc_local_key_get(OUT CHAR_T *destKeybuf, OUT UINT_T *len);

/**
 * \fn OPERATE_RET tuya_ipc_sec_key_get
 * \brief Get wakeup  key from ipc sdk.
 * \ make destKeyBuf len >=17;
 * \return return OPRT_OK if get key success
 */

OPERATE_RET tuya_ipc_sec_key_get(OUT CHAR_T *destKeybuf, OUT UINT_T *len);
/**
 * \fn OPERATE_RET tuya_ipc_low_power_server_addr_get
 * \brief get low power server info
 * \
 * \return return OPRT_OK if get ip、port success .other is failed;
 */

OPERATE_RET tuya_ipc_low_power_server_get(OUT UINT_T *ip, OUT UINT_T *port);

/**
 * \fn OPERATE_RET tuya_ipc_device_id_get
 * \brief get device id
 * \
 * \return return OPRT_OK if get device id success .other is failed;
 */

OPERATE_RET tuya_ipc_device_id_get(IN OUT CHAR_T *devid, IN OUT INT_T *id_len);

#ifdef __cplusplus
}
#endif

#endif /*_TUYA_IPC_API_H*/
