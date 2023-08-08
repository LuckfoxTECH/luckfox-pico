/**
 * @file async_notify.h
 * @author nzy@tuya.com
 * @brief Common process - Initialization
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */
#ifndef _ASYNC_NOTIFY_H
#define _ASYNC_NOTIFY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sys_timer.h"
#include "tuya_cloud_types.h"

// notify handle
typedef PVOID_T NOTIFY_HANDLE;
// notify param
typedef PVOID_T NOTIFY_PARAM;
// notify time(ms)
typedef DWORD_T NOTIFY_TIME_MS;

/**
 * @brief Definition of notify type
 */
typedef enum {
	NOTIFY_FINISH = 0,
	NOTIFY_ERROR,
	NOTIFY_OVERTIME,
} NOTIFY_TYPE_E;

// notify callback function definition
typedef VOID (*P_NOTIFY_FUNC)(IN CONST NOTIFY_TYPE_E type, IN NOTIFY_PARAM param);

/**
 * @brief notify handle info
 */
typedef struct {
	TIMER_ID timer;         // timer
	P_NOTIFY_FUNC func;     // callback function
	NOTIFY_PARAM param;     // param
	NOTIFY_TIME_MS time_ms; // time(ms)
} NOTIFY_HANDLE_S;

/**
 * @brief Create notify handle.
 *
 * @param[out] p_handle: the notify handle
 *
 * @note This API is used for creating the notify handle
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET create_notify_handle(OUT NOTIFY_HANDLE *p_handle);

/**
 * @brief Delete notify handle.
 *
 * @param[in] p_handle: the notify handle
 *
 * @note This API is used for deleting the notify handle
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET delete_notify_handle(IN NOTIFY_HANDLE *p_handle);

/**
 * @brief Register notify handle information.
 *
 * @param[in] handle: the notify handle
 * @param[in] func: the notify callback function
 * @param[in] param: the notify call back param
 * @param[in] time_ms: after time in MS, the notification will be trigged
 *
 * @note This API is used for creating the notify handle
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET register_notify_handle(IN NOTIFY_HANDLE handle, IN CONST P_NOTIFY_FUNC func,
                                   IN CONST NOTIFY_PARAM param, IN CONST NOTIFY_TIME_MS time_ms);
/**
 * @brief Finish notify handle.
 *
 * @param[in] handle: the notify handle
 *
 * @note This API is used for telling the caller the notification is finished.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET notify_handle_finish(IN NOTIFY_HANDLE handle);

/**
 * @brief Finish notify handle.
 *
 * @param[in] handle: the notify handle
 *
 * @note This API is used for telling the caller the notification is error.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET notify_handle_error(IN NOTIFY_HANDLE handle);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*_ASYNC_NOTIFY_H*/
