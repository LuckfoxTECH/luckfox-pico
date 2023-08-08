/**
 * @file tuya_work_queue.h
 * @brief provide work queue operate api
 * @version 0.1
 * @date 2010-09-09
 *
 * @copyright Copyright 2019-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef _TY_WORK_QUEUE_H
#define _TY_WORK_QUEUE_H

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief call back prototype of work
 */
typedef VOID (*TY_WK_CB)(VOID *data);

/**
 * @brief init ty work queue
 *
 * @param[in] NONE
 *
 * @note This API is used for init work queue
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ty_work_queue_init(VOID);

/**
 * @brief add work to work queue
 *
 * @param[in] cb, call back of work
 * @param[in] data, parameter of call back
 *
 * @note This API is used for add work to work queue
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ty_work_queue_add(IN CONST TY_WK_CB cb, IN CONST VOID *data);

/**
 * @brief get current work number in work queue.
 *
 * @param[in] NONE
 *
 * @note This API is used for get the current work number in work queue.
 *
 * @return current work number in the work queue
 */
UINT_T ty_work_queue_depth(VOID);

#ifdef __cplusplus
}
#endif
#endif
