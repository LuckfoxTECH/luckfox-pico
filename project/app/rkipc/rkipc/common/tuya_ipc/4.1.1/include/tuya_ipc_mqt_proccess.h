/*********************************************************************************
 *Copyright(C),2017, 涂鸦科技 www.tuya.comm
 *FileName:    tuya_ipc_mqt_proccess.h
 **********************************************************************************/

#ifndef __TUYA_IPC_MQT_PROCCESS_H__
#define __TUYA_IPC_MQT_PROCCESS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_types.h"

#define PRO_CLOUD_STORAGE_ORDER_REQ 300 /* 云存储订单 */
#define PRO_3RD_PARTY_STREAMING_REQ 301 /* echo show/chromecast request */
#define PRO_RTC_REQ 302                 /* cloud -> dev */
#define PRO_AI_DETECT_DATA_SYNC_REQ                                                                \
	304 /* 本地AI数据更新，当前用于人脸检测样本数据更新(新增/删除/变更) */
#define PRO_FACE_DETECT_DATA_SYNC 306   /* 人脸识别数据同步通知,门禁设备使用 */
#define PRO_CLOUD_STORAGE_EVENT_REQ 307 /* 联动触发云存储 */
#define PRO_DOORBELL_STATUS_REQ 308     /* 门铃请求被用户处理，接听或者拒绝 */
#define PRO_WIFI_MIGRATE_REQ 801        /* wifi迁移 */

typedef struct {
	CHAR_T devid[25 + 1];
	CHAR_T ssid[32 + 1];
	CHAR_T passwd[128 + 1];
	CHAR_T tid[32 + 1];
	CHAR_T iv[16 + 1];
} TUYA_IPC_WIFI_MIGRATE_S;

typedef enum { WIFI_INFO, WIFI_CONNECT, WIFI_LIST } WIFI_MIGRATE_EVENT_TYPE_E;

typedef VOID (*IPC_ECHO_SHOW_CB)(IN CONST CHAR_T *url);

/**
 * \typedef  typedef VOID (*GW_CLOUD_STORAGE_CB)(IN CONST CHAR* action, IN CONST CHAR* store_mode);
 * \brief 云存储功能开启服务回调回调函数定义
 * \param[in] action: store/abort
 * \param[in] store_mode: event/continue
 */
typedef VOID (*IPC_CLOUD_STORAGE_ORDER_CB)(IN CONST CHAR_T *action, IN CONST CHAR_T *store_mode);
typedef VOID (*IPC_CLOUD_STORAGE_EVENT_CB)(IN CONST CHAR_T *action, IN CONST INT_T duration,
                                           IN CONST CHAR_T *data);

/**
 * \typedef typedef VOID (*GW_CHROMECAST_CB)(IN CONST CHAR* url)
 * \brief CHROMECAST 服务回调函数定义
 * \param[in] url
 */
typedef VOID (*IPC_CHROMECAST_CB)(IN CHAR_T *url);

typedef VOID (*IPC_DOORBELL_STATUS_CB)(IN CHAR_T *action, IN CHAR_T *extra_data);

typedef VOID (*IPC_CLOUD_AI_CB)(IN CONST CHAR_T *action, IN CONST CHAR_T *mode);

typedef VOID (*IPC_LOCAL_AI_CB)(IN CONST CHAR_T *action, IN CONST CHAR_T *face_id_list);

typedef int (*IPC_RTC_MQTT_DATA_CB)(IN CONST CHAR_T *dev_id, IN CONST CHAR_T *buffer,
                                    IN CONST INT_T len);

typedef VOID (*IPC_WIFI_MIGRATE_CB)(IN WIFI_MIGRATE_EVENT_TYPE_E action,
                                    IN TUYA_IPC_WIFI_MIGRATE_S data);

VOID tuya_ipc_mqt_EchoShowCb(IN CONST IPC_ECHO_SHOW_CB echo_show_cb);

VOID tuya_ipc_mqt_ChromeCastCb(IN CONST IPC_CHROMECAST_CB chromecast_cb);

VOID tuya_ipc_mqt_cloud_storage_order_cb(IN CONST IPC_CLOUD_STORAGE_ORDER_CB cloud_storage_cb);

VOID tuya_ipc_mqt_cloud_storage_event_cb(IN CONST IPC_CLOUD_STORAGE_EVENT_CB cb);

VOID tuya_ipc_mqt_rtc_callback(IN CONST IPC_RTC_MQTT_DATA_CB pcbk);

VOID iot_register_extra_mqt_cb(VOID);

VOID tuya_ipc_mqt_local_ai_cb(IN CONST IPC_LOCAL_AI_CB cb);

VOID tuya_ipc_wifi_migrate_cb(IN CONST IPC_WIFI_MIGRATE_CB cb);

typedef struct {
	IPC_ECHO_SHOW_CB echo_show_cb;
	IPC_CLOUD_STORAGE_ORDER_CB cloud_storage_order_cb;
	IPC_CLOUD_STORAGE_EVENT_CB cloud_storage_event_cb;
	IPC_CHROMECAST_CB chromecast_cb;
	IPC_DOORBELL_STATUS_CB doorbell_status_cb;
	IPC_RTC_MQTT_DATA_CB rtc_msg_cb;
	IPC_CLOUD_AI_CB cloud_ai_cb;
	IPC_LOCAL_AI_CB local_ai_cb;
	IPC_WIFI_MIGRATE_CB wifi_migrate_cb;
} IPC_MQT_CTRL_S;

#ifdef __cplusplus
}
#endif

#endif
