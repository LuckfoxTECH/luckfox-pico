/**
 * @file tuya_hal_thread.h
 * @brief Common process - Initialization
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */
#ifndef __TUYA_HAL_THREAD_H__
#define __TUYA_HAL_THREAD_H__

#include "tuya_os_adapter.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create thread
 *
 * @param[out] thread: thread handle
 * @param[in] name: thread name
 * @param[in] stack_size: stack size of thread
 * @param[in] priority: priority of thread
 * @param[in] func: the main thread process function
 * @param[in] arg: the args of the func, can be null
 *
 * @note This API is used for creating thread.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_hal_thread_create(THREAD_HANDLE *thread, const char *name, uint32_t stack_size,
                           uint32_t priority, THREAD_FUNC_T func, void *const arg);

/**
 * @brief Terminal thread and release thread resources
 *
 * @param[in] thread: thread handle
 *
 * @note This API is used to terminal thread and release thread resources.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_hal_thread_release(THREAD_HANDLE thread);

/**
 * @brief Check thread is self thread
 *
 * @param[in] thread: thread handle
 * @param[out] is_self: is self thread or not
 *
 * @note This API is used to check thread is self thread.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_hal_thread_is_self(THREAD_HANDLE thread, BOOL_T *is_self);

/**
 * @brief Get the thread stack's watermark
 *
 * @param[in] thread: thread handle
 * @param[out] watermark: watermark in Bytes
 *
 * @note This API is used to get the thread stack's watermark.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_hal_thread_get_watermark(THREAD_HANDLE thread, UINT_T *watermark);

/**
 * @brief Set name of self thread
 *
 * @param[in] name: thread name
 *
 * @note This API is used to set name of self thread.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_hal_thread_set_self_name(const char *name);

/**
 * @brief thread interface initialization
 *
 * @param VOID
 *
 * @note This API is used for initialization of thread interface.
 *
 * @return VOID
 */
VOID_T tuya_os_thread_intf_init(VOID_T);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __TUYA_HAL_THREAD_H__
