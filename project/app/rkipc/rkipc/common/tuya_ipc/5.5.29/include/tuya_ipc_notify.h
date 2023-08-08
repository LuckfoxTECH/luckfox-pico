/*********************************************************************************
 *Copyright(C),2017, 涂鸦科技 www.tuya.comm
 *FileName:    tuya_ipc_notify.h
 **********************************************************************************/

#ifndef _TUYA_IPC_NOTIFY_H
#define _TUYA_IPC_NOTIFY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <time.h>

#include "tuya_iot_config.h"

#if defined(WIFI_GW) && (WIFI_GW == 1)
#include "tuya_cloud_wifi_defs.h"
#endif

#include "cJSON.h"
#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_error_code.h"
#include "tuya_cloud_types.h"

#define IPC_AES_ENCRYPT_KEY_LEN 16
#define IPC_NOTIFY_INFO_LEN 16

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
	char *data;
	int len;
	char secret_key[IPC_AES_ENCRYPT_KEY_LEN + 1];
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
	NOTIFICATION_NAME_LOW_BATTERY = 18,    /* 低电量*/
	NOTIFICATION_NAME_NOUSE = 19,          /* 忽略*/
	NOTIFICATION_NAME_COVER = 20,          /* 封面 */
	NOTIFICATION_NAME_FULL_BATTERY = 21,   /* 电量满 */
	NOTIFICATION_NAME_USER_IO = 22,        /* 本地IO设备触发-2 */
	NOTIFY_TYPE_MAX
} NOTIFICATION_NAME_E;

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

#define NOTIFICATION_MOTION_DETECTION 115

OPERATE_RET tuya_ipc_notify_init_lock();

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
 * \fn OPERATE_RET tuya_ipc_start_storage
 * \brief start storage
 * \param[in] storage_type: E_ALARM_SD_STORAGE & E_ALARM_CLOUD_STORAGE
 * \return return OPRT_OK if start storage success .other is failed;
 */
OPERATE_RET tuya_ipc_start_storage(INT_T storage_type);

/**
 * \fn OPERATE_RET tuya_ipc_stop_storage
 * \brief stop storage
 * \param[in] storage_type: E_ALARM_SD_STORAGE & E_ALARM_CLOUD_STORAGE
 * \return return OPRT_OK if stop storage success .other is failed;
 */
OPERATE_RET tuya_ipc_stop_storage(INT_T storage_type);

/**
 * \fn OPERATE_RET tuya_ipc_notify_alarm
 * \brief send a editable alarm to tuya cloud and APP
 * \param[in] snap_buffer: address of current snapshot
 * \param[in] snap_size: size fo snapshot, in Byte
 * \param[in] name: editable event type, NOTIFICATION_NAME_E
 * \param[in] is_notify: send to message center if TRUE
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_notify_alarm(CONST CHAR_T *snap_buffer, CONST UINT_T snap_size,
                                  CONST NOTIFICATION_NAME_E name, BOOL_T is_notify);

/**
 * \fn OPERATE_RET tuya_ipc_send_custom_mqtt_msg(IN CONST UINT protocol, IN CONST CHAR *p_data)
 * \brief send customized mqtt message to tuya cloud
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_send_custom_mqtt_msg(IN CONST UINT_T protocol, IN CONST CHAR_T *p_data);

/**
 * \fn OPERATE_RET tuya_ipc_report_living_msg(uint32_t error_code, int force, int timeout)
 * \brief report living message to tuya APP
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_report_living_msg(IN UINT_T error_code, IN UINT_T force, IN UINT_T timeout);
typedef struct {
	CHAR_T cmd[64];            // event
	CHAR_T subVersion[32];     // version
	CHAR_T eventType[64];      // event type
	CHAR_T eventId[64];        // event ID
	INT_T notifyMessageCenter; //是否通知消息中心。0：不通知，1通知
	INT_T bodyLen;             //消息长度
	CHAR_T *bodyData;          //消息体
} TUYA_IPC_AI_EVENT_CTX;
/**
 * \fn OPERATE_RET tuya_ipc_ai_event_notify(TUYA_XVR_AI_EVENT_CTX *ctx);
 * \param[in] ctx : event context;
 * \return success:0.failed:other
 */
OPERATE_RET tuya_ipc_ai_event_notify(TUYA_IPC_AI_EVENT_CTX *ctx);
/**
 * \fn OPERATE_RET tuya_ipc_ai_image_upload(char * imageBuf,UINT_T imageSize,char * result,UINT_T
 * reslut_len); \param[in] result_len : can not less 256,must >=256 \param[in] imageBuf : image
 * buffer address \param[in] imageSize : image size; \param[out] result : image upload callback
 * image \return  \return success:0.failed:other
 */
OPERATE_RET tuya_ipc_ai_image_upload(CHAR_T *imageBuf, UINT_T imageSize, char *result,
                                     UINT_T result_len);

#ifdef __cplusplus
}
#endif

#endif /*_TUYA_IPC_NOTIFY_H*/
