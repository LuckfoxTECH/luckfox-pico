/**
 * @file tuya_svc_schema_upgrade.h
 * @brief TUYA schema OTA service
 * @version 0.1
 * @date 2020-07-31
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 */

#ifndef __TUYA_SVC_SCHEMA_UPGRADE_H__
#define __TUYA_SVC_SCHEMA_UPGRADE_H__

#include "tuya_cloud_types.h"
#include "tuya_error_code.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EVT_DEV_SCHEMA_UPGRADE "schema.ug.cid"

/**
 * @brief Definition of schema upgrade action
 */
typedef enum tagAction_e {
	/** this action will redo */
	ACTION_CONTINUE,
	/** switch to next action */
	ACTION_NEXT,
	/** action list is complete */
	ACTION_COMPLETE,
	ACTION_MAX
} Action_e;

/**
 * @brief Schema upgrade action
 *
 * @param[in] data ID of sub-device, or NULL for GW
 *
 * @return see Action_e
 */
typedef Action_e (*ACTION_CB)(OUT VOID *data);

/**
 * @brief Initilize schema upgrade framework
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ty_schema_upgrade_init(VOID);

/**
 * @brief Run schema upgrade framework, workloads registered will be executed
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ty_schema_upgrade_active(VOID);

/**
 * @brief Schema upgrade is triggered by MQTT msg
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ty_schema_upgrade_notify(CHAR_T *dev_id);

/**
 * @brief Register schema upgrade workload
 *
 * @param[in] cb_update Callback to upload schema upload status
 * @param[in] cb_get Callback to retrieve schema from cloud
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ty_schema_upgrade_register(ACTION_CB cb_update, ACTION_CB cb_get);

#ifdef __cplusplus
}
#endif

#endif //__TUYA_SVC_SCHEMA_UPGRADE_H__
