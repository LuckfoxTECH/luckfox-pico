/**
 * @file sys_timer.h
 * @author nzy@tuya.com
 * @brief Common process - Initialization
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */
#ifndef _SYS_TIMER_H
#define _SYS_TIMER_H

#include "tuya_cloud_types.h"
#include "uni_time_queue.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializing the system timer
 *
 * @param VOID
 *
 * @note This API is used for initializing the system timer
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET system_timer_init(void);

/**
 * @brief Add a system timer
 *
 * @param[in] pTimerFunc: the processing function of the timer
 * @param[in] pTimerArg: the parameater of the timer function
 * @param[out] p_timerID: timer id
 *
 * @note This API is used for adding a system timer
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET sys_add_timer(IN CONST P_TIMER_FUNC pTimerFunc, IN CONST PVOID_T pTimerArg,
                          OUT TIMER_ID *p_timerID);

/**
 * @brief Delete the system timer
 *
 * @param[in] timerID: timer id
 *
 * @note This API is used for deleting the system timer
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET sys_delete_timer(IN CONST TIMER_ID timerID);

/**
 * @brief Stop the system timer
 *
 * @param[in] timerID: timer id
 *
 * @note This API is used for stopping the system timer
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET sys_stop_timer(IN CONST TIMER_ID timerID);

/**
 * @brief Identify the system timer is running
 *
 * @param[in] timerID: timer id
 *
 * @note This API is used to identify wheather the system timer is running
 *
 * @return TRUE or FALSE
 */
BOOL_T IsThisSysTimerRun(IN CONST TIMER_ID timer_id);

/**
 * @brief Start the system timer
 *
 * @param[in] timerID: timer id
 * @param[in] timeCycle: timer running cycle
 * @param[in] timer_type: timer type
 *
 * @note This API is used for starting the system timer
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET sys_start_timer(IN CONST TIMER_ID timerID, IN CONST TIME_MS timeCycle,
                            IN CONST TIMER_TYPE timer_type);

/**
 * @brief Trigger the system timer
 *
 * @param[in] timerID: timer id
 *
 * @note This API is used for triggering the system timer instantly.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET sys_trigger_timer(IN CONST TIMER_ID timerID);

/**
 * @brief Release all resource of the system timer
 *
 * @param VOID
 *
 * @note This API is used for releasing all resource of the system timer
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET system_timer_release(void);

/**
 * @brief Set sleep interval of the system timer
 *
 * @param[in] interval: sleep interval(MS) of the system timer
 *
 * @note This API is used for setting the sleep interval(MS) of the system timer
 *
 * @return VOID
 */
VOID system_timer_set_sleep_interval(ULONG_T interval);

/**
 * @brief Get sleep interval of the system timer
 *
 * @param VOID
 *
 * @note This API is used for getting the sleep interval(MS) of the system timer
 *
 * @return the sleep interval of the system timer
 */
ULONG_T system_timer_get_sleep_interval(VOID);

/**
 * @brief Get timer node currently
 *
 * @param VOID
 *
 * @note This API is used for getting the timer node currently.
 *
 * @return the timer node count.
 */
INT_T sys_get_timer_num(VOID);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
