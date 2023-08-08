/**
 * @file base_event.h
 * @brief tuya simple event module
 * @version 1.0
 * @date 2019-10-30
 *
 * @copyright Copyright (c) tuya.inc 2019
 *
 */

#ifndef __BASE_EVENT_H__
#define __BASE_EVENT_H__

#include "base_event_info.h"
#include "tuya_base_utilities.h"
#include "tuya_iot_config.h"
#include "tuya_os_adapter.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief max length of event name
 *
 */
#ifndef EVENT_NAME_MAX_LEN
#define EVENT_NAME_MAX_LEN (16) // move to tuya_iot_config.h. use kconfig config. default is 16
#endif

/**
 * @brief max length of event description
 *
 */
#define EVENT_DESC_MAX_LEN (32)

/**
 * @brief subscriber type
 *
 */
typedef BYTE_T SUBSCRIBE_TYPE_E;
#define SUBSCRIBE_TYPE_NORMAL                                                                      \
	0 // normal type, dispatch by the subscribe order, remove when unsubscribe
#define SUBSCRIBE_TYPE_EMERGENCY 1 // emergency type, dispatch first, remove when unsubscribe
#define SUBSCRIBE_TYPE_ONETIME                                                                     \
	2 // one time type, dispatch by the subscribe order, remove after first time dispath

/**
 * @brief the event dispatch raw data
 *
 */
typedef struct {
	int type;      // the data type
	int len;       // the data length
	char value[0]; // the data content
} event_raw_data_t;

/**
 * @brief event subscribe callback function type
 *
 */
typedef int (*event_subscribe_cb)(void *data);

/**
 * @brief the subscirbe node
 *
 */
typedef struct {
	char name[EVENT_NAME_MAX_LEN + 1]; // name, used to record the the event info
	char desc[EVENT_DESC_MAX_LEN + 1]; // description, used to record the subscribe info
	SUBSCRIBE_TYPE_E type;             // the subscribe type
	event_subscribe_cb cb;             // the subscribe callback function
	struct tuya_list_head node;        // list node, used to attch to the event node
} subscribe_node_t;

/**
 * @brief the event node
 *
 */
typedef struct {
	MUTEX_HANDLE mutex; // mutex, protection the event publish and subscribe

	char name[EVENT_NAME_MAX_LEN + 1];    // name, the event name
	struct tuya_list_head node;           // list node, used to attach to the event manage module
	struct tuya_list_head subscribe_root; // subscibe root, used to manage the subscriber
} event_node_t;

/**
 * @brief the event manage node
 *
 */
typedef struct {
	int inited;
	MUTEX_HANDLE mutex;                        // mutex, used to protection event manage node
	int event_cnt;                             // current event number
	struct tuya_list_head event_root;          // event root, used to manage the event
	struct tuya_list_head free_subscribe_root; // free subscriber list, used to manage the subscribe
	                                           // which not found the event
} event_manage_t;

/**
 * @brief event initialization
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int ty_event_init(void);

/**
 * @brief: publish event
 *
 * @param[in] name: event name
 * @param[in] data: event data
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int ty_publish_event(const char *name, void *data);

/**
 * @brief: subscribe event
 *
 * @param[in] name: event name
 * @param[in] desc: subscribe description
 * @param[in] cb: subscribe callback function
 * @param[in] type: subscribe type
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int ty_subscribe_event(const char *name, const char *desc, const event_subscribe_cb cb,
                       SUBSCRIBE_TYPE_E type);

/**
 * @brief: unsubscribe event
 *
 * @param[in] name: event name
 * @param[in] desc: subscribe description
 * @param[in] cb: subscribe callback function
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int ty_unsubscribe_event(const char *name, const char *desc, event_subscribe_cb cb);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__BASE_EVENT_H__ */
