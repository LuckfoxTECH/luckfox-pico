/*********************************************************************************
 *Copyright(C),2017, 涂鸦科技 www.tuya.comm
 *FileName:    tuya_ipc_cloud_storage.h
 **********************************************************************************/

#ifndef __TUYA_IPC_CLOUD_STORAGE_H__
#define __TUYA_IPC_CLOUD_STORAGE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_error_code.h"
#include "tuya_cloud_types.h"
#include "tuya_ipc_media.h"

#define MAX_SNAPSHOT_BUFFER_SIZE (100 * 1024) // in BYTE
#define INVALID_EVENT_ID 0xFFFFFFFF
#define MAX_CLOUD_EVENT_DURATION 300 // in second

typedef UINT_T EVENT_ID;

typedef enum { EVENT_NONE, EVENT_ONGOING, EVENT_READY, EVENT_INVALID } EVENT_STATUS_E;

typedef enum {
	EVENT_TYPE_MOTION_DETECT = 0,   /* ipc_event_motion,"ipc_motion"*/
	EVENT_TYPE_DOOR_BELL = 1,       /* ipc_event_doorbell,"ipc_doorbell"*/
	EVENT_TYPE_DEV_LINK = 2,        /* event triggerred by other tuya cloud device*/
	EVENT_TYPE_PASSBY = 3,          /* for doorbell, detect someone passby */
	EVENT_TYPE_LINGER = 4,          /* for doorbell, detect someone linger */
	EVENT_TYPE_LEAVE_MESSAGE = 5,   /* for doorbell, a video message leaved */
	EVENT_TYPE_CALL_ACCEPT = 6,     /* for doorbell, call is accepted */
	EVENT_TYPE_CALL_NOT_ACCEPT = 7, /* for doorbell, call is not accepted */
	EVENT_TYPE_CALL_REFUSE = 8,     /* ipc_event_call_refuse*/
	EVENT_TYPE_HUMAN_DETECT = 9,    /* ipc_event_human,"ipc_human"*/
	EVENT_TYPE_CAT_DETECT = 10,     /* ipc_event_cat,"ipc_cat"*/
	EVENT_TYPE_CAR_DETECT = 11,     /* ipc_event_car,"ipc_car"*/
	EVENT_TYPE_BABY_CRY = 12,       /* ipc_event_baby_cry,"ipc_baby_cry"*/
	EVENT_TYPE_BANG = 13,           /* ipc_event_bang,"ipc_bang"*/
	EVENT_TYPE_FACE = 14,           /* ipc_event_face,"ipc_face"*/
	EVENT_TYPE_ANTIBREAK = 15,      /* ipc_event_antibreak,"ipc_antibreak"*/
	EVENT_TYPE_RECORD_ONLY =
	    16, /* only to start event record, no event reported or will be reported later */
	EVENT_TYPE_IO_ALARM = 17, /* event triggerred by local IO devices */
	EVENT_TYPE_INVALID
} ClOUD_STORAGE_EVENT_TYPE_E;

/* 云存储订单类型 */
typedef enum {
	ClOUD_STORAGE_TYPE_CONTINUE, /* continus record type */
	ClOUD_STORAGE_TYPE_EVENT,  /* event-based type, only record and store audio/video when there is
	                              event happening*/
	ClOUD_STORAGE_TYPE_INVALID /* no cloud storage order exist */
} ClOUD_STORAGE_TYPE_E;

/**
 * \fn OPERATE_RET tuya_ipc_cloud_storage_init
 * \brief initialize cloud storage, which will malloc all needed memory after this
 * \param[in] media_setting is used to decide needed memroy and storage strategy
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_cloud_storage_init(IN IPC_MEDIA_INFO_S *media_setting,
                                        IN AES_HW_CBC_FUNC *aes_func);

/**
 * \fn OPERATE_RET tuya_ipc_cloud_storage_uninit
 * \brief uninit cloud storage, free used memory
 * \param[]
 * \return
 */
VOID tuya_ipc_cloud_storage_uninit(VOID);

/**
 * \fn OPERATE_RET tuya_ipc_cloud_storage_get_store_mode
 * \brief get current sotrage type, based on purchase order
 * \return ClOUD_STORAGE_TYPE_E
 */
ClOUD_STORAGE_TYPE_E tuya_ipc_cloud_storage_get_store_mode(VOID);

/**
 * \fn OPERATE_RET tuya_ipc_cloud_storage_event_start
 * \brief report an event, cloud storage will start and mantain an evnet list
 * \param[in] snapshot_buffer snapshot_size for current event
 * \param[in] type event type
 * \return OPERATE_RET
 * \caution! this API will no longer supported, and currently only motion detect and doorbell type
 * event are supported
 */
OPERATE_RET tuya_ipc_cloud_storage_event_start(IN CHAR_T *snapshot_buffer, IN UINT_T snapshot_size,
                                               IN ClOUD_STORAGE_EVENT_TYPE_E type);

/**
 * \fn OPERATE_RET tuya_ipc_cloud_storage_event_start
 * \brief report the end of event
 * \return OPERATE_RET
 * \caution! this API will no longer supported, and currently both motion detect and doorbell type
 * events will be stopped
 */
OPERATE_RET tuya_ipc_cloud_storage_event_stop(VOID);

/**
 * \fn OPERATE_RET tuya_ipc_cloud_storage_get_event_status
 * \brief get specified type event status
 * \return OPERATE_RET
 */
EVENT_STATUS_E tuya_ipc_cloud_storage_get_event_status(ClOUD_STORAGE_EVENT_TYPE_E type);

/**
 * \fn OPERATE_RET tuya_ipc_cloud_storage_get_event_status_by_id
 * \brief 根据事件id获取当前事件状态。
 * \return OPERATE_RET
 */

EVENT_STATUS_E tuya_ipc_cloud_storage_get_event_status_by_id(EVENT_ID event_id);

/**
 * \fn VOID tuya_ipc_cloud_storage_pause
 * \brief pause cloud storage, used in privacy mode e.g.
 * \return VOID
 */
VOID tuya_ipc_cloud_storage_pause(VOID);

/**
 * \fn VOID tuya_ipc_cloud_storage_resume
 * \brief resume cloud storage which is paused by pause API
 * \return VOID
 */
VOID tuya_ipc_cloud_storage_resume(VOID);

/**
 * \fn OPERATE_RET tuya_ipc_cloud_storage_event_add
 * \brief add a new event
 * \param[in] snapshot_buffer snapshot_size of current evnet
 * \param[in] type event type
 * \param[in] max_duration max duration of the event, not bigger than MAX_CLOUD_EVENT_DURATION
 * \          event will be automaticly stopped when times up, if it's not stopped by delete API
 * \return EVENT_ID unique evnet id
 */
EVENT_ID tuya_ipc_cloud_storage_event_add(CHAR_T *snapshot_buffer, UINT_T snapshot_size,
                                          ClOUD_STORAGE_EVENT_TYPE_E type, UINT_T max_duration);

/**
 * \fn OPERATE_RET tuya_ipc_cloud_storage_event_delete
 * \brief delete(stop) specified event
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_cloud_storage_event_delete(EVENT_ID event_id);

/**
 * \fn OPERATE_RET tuya_ipc_cloud_storage_event_notify
 * \brief
 * 手动触发事件的上报。应用场景：部分事件在add的时候无法确定类型，需要在后续基于场景变化确定type类型
 * \param[in] EVENT_ID 事件唯一ID
 * \param[in] type 事件类型
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_cloud_storage_event_notify(EVENT_ID event_id, ClOUD_STORAGE_EVENT_TYPE_E type);

/**
 * \fn OPERATE_RET tuya_ipc_cloud_storage_set_event_type
 * \brief if event type changed, set to new type.
 * \param[in] EVENT_ID event id
 * \param[in] type type to set
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_cloud_storage_set_event_type(EVENT_ID event_id,
                                                  ClOUD_STORAGE_EVENT_TYPE_E type);

/**
 * \fn OPERATE_RET tuya_ipc_cloud_storage_set_pre_record_time
 * \brief set cloud storage pre record time duration
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_cloud_storage_set_pre_record_time(INT_T pre_record_time);

#ifdef __cplusplus
}
#endif

#endif
