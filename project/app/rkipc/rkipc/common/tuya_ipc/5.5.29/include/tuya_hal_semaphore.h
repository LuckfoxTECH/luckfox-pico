/**
 * @file tuya_hal_semaphore.h
 * @brief Common process - Initialization
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */
#ifndef _TUYA_HAL_SEMAPHORE_H
#define _TUYA_HAL_SEMAPHORE_H

#include "tuya_os_adapter.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create semaphore
 *
 * @param[out] pHandle: semaphore handle
 * @param[in] semCnt: semaphore init count
 * @param[in] sem_max: semaphore max count
 *
 * @note This API is used for creating semaphore.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T tuya_hal_semaphore_create_init(SEM_HANDLE *pHandle, CONST UINT_T semCnt,
                                     CONST UINT_T sem_max);

/**
 * @brief Wait semaphore
 *
 * @param[in] semHandle: semaphore handle
 *
 * @note This API is used for waiting semaphore.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T tuya_hal_semaphore_wait(CONST SEM_HANDLE semHandle);

/**
 * @brief Wait semaphore with timeout
 *
 * @param[in] semHandle: semaphore handle
 * @param[in] timeout: timeout(TUYA_OS_ADAPT_SEM_FOREVER: block forever)
 *
 * @note This API is used for waiting semaphore.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T tuya_hal_semaphore_waittimeout(CONST SEM_HANDLE semHandle, unsigned int timeout);

/**
 * @brief Post semaphore
 *
 * @param[in] semHandle: semaphore handle
 *
 * @note This API is used for posting semaphore.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T tuya_hal_semaphore_post(CONST SEM_HANDLE semHandle);

/**
 * @brief Release semaphore
 *
 * @param[in] semHandle: semaphore handle
 *
 * @note This API is used for releasing semaphore.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T tuya_hal_semaphore_release(CONST SEM_HANDLE semHandle);

/**
 * @brief Semaphore interface initialization
 *
 * @param VOID
 *
 * @note This API is used for initialization of semaphore interface.
 *
 * @return VOID
 */
VOID_T tuya_os_semaphore_intf_init(VOID_T);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
