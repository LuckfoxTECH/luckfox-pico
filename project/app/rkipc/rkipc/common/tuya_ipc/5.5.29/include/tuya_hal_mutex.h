/**
 * @file tuya_hal_mutex.h
 * @brief Common process - Initialization
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */
#ifndef _TUYA_HAL_MUTEX_H
#define _TUYA_HAL_MUTEX_H

#include "tuya_os_adapter.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create mutex
 *
 * @param[out] pMutexHandle: mutex handle
 *
 * @note This API is used to create mutex.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_hal_mutex_create_init(MUTEX_HANDLE *pMutexHandle);

/**
 * @brief Lock mutex
 *
 * @param[in] mutexHandle: mutex handle
 *
 * @note This API is used to lock mutex.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_hal_mutex_lock(const MUTEX_HANDLE mutexHandle);

/**
 * @brief Unlock mutex
 *
 * @param[in] mutexHandle: mutex handle
 *
 * @note This API is used to unlock mutex.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_hal_mutex_unlock(const MUTEX_HANDLE mutexHandle);

/**
 * @brief Release mutex
 *
 * @param[in] mutexHandle: mutex handle
 *
 * @note This API is used to release mutex.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_hal_mutex_release(const MUTEX_HANDLE mutexHandle);

/**
 * @brief Mutex interface initialization
 *
 * @param VOID
 *
 * @note This API is used for initialization of mutex interface.
 *
 * @return VOID
 */
VOID_T tuya_os_mutex_intf_init(VOID_T);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
