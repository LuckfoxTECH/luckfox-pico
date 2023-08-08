#ifndef __EVENT_H__
#define __EVENT_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "tuya_ipc_api.h"
#include "tuya_ipc_cloud_storage.h"

#define MAX_TEXT_LEN 32

#define RESOURCE_PIC 1
#define RESOURCE_TEXT 2
#define RESOURCE_VIDEO 4
#define RESOURCE_AUDIO 8

#define AES_KEY_LEN 16

#define ALARM_BASE (0x1)
typedef enum TUYA_ALARM_TYPE_ {
	E_ALARM_MOTION = ALARM_BASE << 0,
	E_ALARM_DOORBELL = ALARM_BASE << 1,
	E_ALARM_DEV_LINK = ALARM_BASE << 2,
	E_ALARM_PASSBY = ALARM_BASE << 3,
	E_ALARM_LINGER = ALARM_BASE << 4,
	E_ALARM_MSG = ALARM_BASE << 5,
	E_ALARM_CONNECTED = ALARM_BASE << 6,
	E_ALARM_UNCONNECTED = ALARM_BASE << 7,
	E_ALARM_REFUSE = ALARM_BASE << 8,
	E_ALARM_HUMAN = ALARM_BASE << 9,
	E_ALARM_CAT = ALARM_BASE << 10,
	E_ALARM_CAR = ALARM_BASE << 11,
	E_ALARM_BABY_CRY = ALARM_BASE << 12,
	E_ALARM_BANG = ALARM_BASE << 13,
	E_ALARM_FACE = ALARM_BASE << 14,
	E_ALARM_ANTIBREAK = ALARM_BASE << 15,
	E_ALARM_RESERVE = ALARM_BASE << 16,
	E_ALARM_IO = ALARM_BASE << 17,
	E_ALARM_LOW_BATTERY = ALARM_BASE << 18,
	E_ALARM_DOORBELL_PRESS = ALARM_BASE << 19,
	E_ALARM_COVER = ALARM_BASE << 20,
} TUYA_ALARM_TYPE_E;

#define ALARM_STORAGE_BASE (0x1)
typedef enum TUYA_ALARM_STORAGE_TYPE_ {
	E_ALARM_SD_STORAGE = ALARM_STORAGE_BASE << 0,
	E_ALARM_CLOUD_STORAGE = ALARM_STORAGE_BASE << 1,
} TUYA_ALARM_STORAGE_TYPE;

typedef struct {
	CHAR_T *media_buf;
	INT_T media_size;
	CHAR_T key[AES_KEY_LEN];
} TUYA_ALARM_MEDIA_T;

typedef struct {
	INT_T text_type; // enum text
	CHAR_T text[MAX_TEXT_LEN];
} TUYA_ALARM_TEXT_T;

typedef struct {
	INT_T type;          //类型，bit位
	INT_T wait_for;      // 0为独立事件，非零为依赖此字段中的type
	INT_T is_notify;     //是否上报消息中心
	TIME_T trigger_time; //触发时间
	TIME_T upload_time;  //预期上报时间， 0为立即上报, -1为等待到事件结束上报
	BOOL_T valid;        // 1 需要处理的, 0 已经处理过。用户需设置为1
	BOOL_T force;        //是否强制上报，与event无关的上报，如封面图
	INT_T resource_type; //参考上面的RESOURCE_xx宏,表明下面的资源有哪些类型
	CHAR_T
	*extra_data; //额外字段，添加到上报信息中，需为去掉大括号的json格式，如"aaa":3，若无写NULL
	CHAR_T *pic_buf;            //图片缓存
	INT_T pic_size;             //图片大小
	TUYA_ALARM_TEXT_T *context; //文字告警的信息，若无写NULL
	INT_T context_cnt;          // TUYA_ALARM_TEXT_T的数量,若无写0
	NOTIFICATION_UNIT_T *media; //音视频的资源信息，若无写NULL
	INT_T media_cnt;            // NOTIFICATION_UNIT_T的数量,若无写0
} TUYA_ALARM_T;

/*开始一个alarm_types的event*/
OPERATE_RET tuya_ipc_start_event(TUYA_ALARM_TYPE_E alarms_types); // start an event

/*开始一个alarm_types的event,开始一个存储事件*/
OPERATE_RET tuya_ipc_start_event_with_storage(TUYA_ALARM_TYPE_E alarms_types, INT_T storage_type,
                                              UINT_T max_duration, EVENT_ID *pevent_id);

/*开始一个alarm_types的event,且此event仅能被开启一次*/
OPERATE_RET tuya_ipc_start_event_once(TUYA_ALARM_TYPE_E alarms_types);

/*结束一个alarm_types的event*/
OPERATE_RET tuya_ipc_stop_event(TUYA_ALARM_TYPE_E alarms_types); // stop an event

/*结束一个alarm_types的event，结束一个存储事件*/
OPERATE_RET tuya_ipc_stop_event_with_storage(TUYA_ALARM_TYPE_E alarms_types, INT_T storage_type,
                                             EVENT_ID event_id);

/*结束一个alarm_types的event，结束一个存储事件，同步接口，事件未结束，阻塞在接口上，超时时间timeout*/
OPERATE_RET tuya_ipc_stop_event_with_storage_sync(TUYA_ALARM_TYPE_E alarms_types,
                                                  INT_T storage_type, EVENT_ID event_id,
                                                  INT_T timeout);

/*查询当前报警类型相关的event是否开启*/
BOOL_T tuya_ipc_event_check_status(TUYA_ALARM_TYPE_E alarms_types);

/*查询存储事件是否结束*/
BOOL_T tuya_ipc_event_check_status_with_storage(INT_T storage_type);

/* 上报一个告警 */
OPERATE_RET tuya_ipc_alarm_event(TUYA_ALARM_T *alarm);

/* 上报一个不基于事件的告警 */
OPERATE_RET tuya_ipc_trigger_alarm_without_event(TUYA_ALARM_T *alarm);

/* 通知event模块设备在线，加速配置获取 */
OPERATE_RET tuya_ipc_event_set_online();

/*
    max_event
   支持的最大数量的event，如视频侦测类的、音频检测类的、门铃类的，同一类型的会按照alarm的优先级上报
    alarms_of_event 每个event支持的alarm type
    alarm_cnt_per_event 每个event支持的最多同时上报的数量（为了方便，统一设置）
*/
OPERATE_RET tuya_ipc_event_module_init(INT_T max_event, INT_T *alarms_of_event,
                                       INT_T alarm_cnt_per_event);

OPERATE_RET tuya_ipc_event_module_exit();

#ifdef __cplusplus
}
#endif

#endif
