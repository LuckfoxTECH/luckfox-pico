/**
 * @file uni_work_queue.h
 * @brief provide work queue operate api
 * @version 0.1
 * @date 2010-09-09
 *
 * @copyright Copyright 2019-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef _UNI_WORK_QUEUE_H
#define _UNI_WORK_QUEUE_H

#include "tuya_base_utilities.h"
#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief handle of work queue
 */
typedef PVOID_T WORK_QUEUE_HANDLE;

/**
 * @brief call back function of work queue
 */
typedef VOID (*WK_CALLBACK)(VOID *data);

/**
 * @brief Create work queue
 *
 * @param[out] phand: pointer of the work queue handle
 * @param[in] stack_size: stack depth of work queue's thread
 * @param[in] pri: thread priority of work queue
 * @param[in] work_num, thread num of work queue
 *
 * @note This API is used for creating work queue
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET work_queue_create(OUT WORK_QUEUE_HANDLE *phand, IN CONST STACK_SIZE stack_size,
                              IN CONST TRD_PRI pri, IN CONST UINT_T work_num);

/**
 * @brief Add a task to work queue
 *
 * @param[in] hand: handle of the work queue
 * @param[in] cb: callback function of task
 * @param[in] data: data for callback function
 *
 * @note This API is used for creating work queue
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET work_queue_add(IN CONST WORK_QUEUE_HANDLE hand, IN CONST WK_CALLBACK cb,
                           IN CONST VOID *data);

/**
 * @brief Check and delete a task from work queue
 *
 * @param[in] hand: handle of the work queue
 * @param[in] cb: callback function of task
 * @param[in] data: data for callback function
 *
 * @note This API is used to check and delete a task from work queue
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET work_queue_check_and_del(IN CONST WORK_QUEUE_HANDLE hand, IN CONST VOID *data);

/**
 * @brief Release a work queue
 *
 * @param[in] hand: handle of the work queue
 *
 * @note This API is used for releasing a work queue
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET work_queue_release(IN CONST WORK_QUEUE_HANDLE hand);

/**
 * @brief Get stack depth of the work queue
 *
 * @param[in] hand: handle of the work queue
 *
 * @note This API is used for getting stack depth of the work queue
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
UINT_T work_queue_get_depth(IN CONST WORK_QUEUE_HANDLE hand);

#ifdef __cplusplus
}
#endif
#endif
