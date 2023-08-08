/**
 * @file uni_time_queue.h
 * @author nzy@tuya.com
 * @brief Common process - Initialization
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */
#ifndef _UNI_TIME_QUEUE_H
#define _UNI_TIME_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_types.h"

// the handle of the timer queue
typedef PVOID_T TIMER_QUE_HANDLE;

// timer id
typedef WORD_T TIMER_ID; // 定时器ID

// the timer's processing function
typedef VOID (*P_TIMER_FUNC)(UINT_T timerID, PVOID_T pTimerArg);

/**
 * @brief the type of timer
 */
typedef enum {
	TIMER_ONCE = 0,
	TIMER_CYCLE,
} TIMER_TYPE;

/**
 * @brief Create and init the system timer queue
 *
 * @param[in] pTimerQueHandle: the handle of the timer queue
 *
 * @note This API is used for initializing the system timer
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET CreateTimerQueAndInit(OUT TIMER_QUE_HANDLE *pTimerQueHandle);

/**
 * @brief Add timer to queue
 *
 * @param[in] timerQueHandle: the handle of the timer queue
 * @param[in] timeCycle: cycle of the timer
 * @param[in] pTimerFunc: processing function of the timer
 * @param[out] timerID: timer id
 * @param[in] pTimerArg: the parameter of the timer's processing function
 *
 * @note This API is used for adding a timer to the queue
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET AddTimerToQueue(IN CONST TIMER_QUE_HANDLE timerQueHandle, IN CONST TIME_MS timeCycle,
                            IN CONST P_TIMER_FUNC pTimerFunc, OUT TIMER_ID *timerID,
                            IN CONST PVOID_T pTimerArg);

/**
 * @brief Delete timer from queue
 *
 * @param[in] timerQueHandle: the handle of the timer queue
 * @param[in] timerID: timer id
 *
 * @note This API is used for deleting a timer from the queue
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET DeleteTimerFromQueue(IN CONST TIMER_QUE_HANDLE timerQueHandle,
                                 IN CONST TIMER_ID timerID);

/**
 * @brief Start the timer in the queue
 *
 * @param[in] timerQueHandle: the handle of the timer queue
 * @param[in] timerID: timer id
 * @param[in] timeCycle: cycle of the timer
 * @param[in] timer_type: type of the timer
 *
 * @note This API is used for starting the timer in the queue
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET StartTimerFromQueue(IN CONST TIMER_QUE_HANDLE timerQueHandle, IN CONST TIMER_ID timerID,
                                IN CONST TIME_MS timeCycle, IN CONST TIMER_TYPE timer_type);

/**
 * @brief Trigger the timer in the queue
 *
 * @param[in] timerQueHandle: the handle of the timer queue
 * @param[in] timerID: timer id
 *
 * @note This API is used for triggering the timer in the queue instantly.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET TriggerTimerFromQueue(IN CONST TIMER_QUE_HANDLE timerQueHandle,
                                  IN CONST TIMER_ID timerID);

/**
 * @brief Stop the timer in the queue
 *
 * @param[in] timerQueHandle: the handle of the timer queue
 * @param[in] timerID: timer id
 *
 * @note This API is used for stopping the timer in the queue.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET StopTimerFromQueue(IN CONST TIMER_QUE_HANDLE timerQueHandle, IN CONST TIMER_ID timerID);

/**
 * @brief Dispatch all timers in the queue
 *
 * @param[in] timerQueHandle: the handle of the timer queue
 *
 * @note This API is used for dispatching all timers in the queue.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET DispatchTimerFromQuene(IN CONST TIMER_QUE_HANDLE timerQueHandle);

/**
 * @brief Get the timer node count of the queue
 *
 * @param[in] timerQueHandle: the handle of the timer queue
 * @param[out] pTimerNodeNum: the timer node count of the queue
 *
 * @note This API is used for getting the timer node count of the queue
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET GetTimerNodeNum(IN CONST TIMER_QUE_HANDLE timerQueHandle, OUT PINT_T pTimerNodeNum);

/**
 * @brief Release the timer queue
 *
 * @param[in] timerQueHandle: the handle of the timer queue
 *
 * @note This API is used for releasing all resouce of the timer queue
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ReleaseTimerQue(IN CONST TIMER_QUE_HANDLE timerQueHandle);

/**
 * @brief Get an idle timer of the queue
 *
 * @param[in] timerQueHandle: the handle of the timer queue
 *
 * @note This API is used for getting an idle timer of the queue
 *
 * @return the timer id
 */
TIMER_ID GetIdleTimerIdRapid(IN CONST TIMER_QUE_HANDLE timerQueHandle);

/**
 * @brief Identify the timer is running
 *
 * @param[in] timerQueHandle: the handle of the timer queue
 * @param[in] timerID: timer id
 *
 * @note This API is used to identify wheather the timer is running
 *
 * @return TRUE or FALSE
 */
BOOL_T IsThisTimerRun(IN CONST TIMER_QUE_HANDLE timerQueHandle, IN CONST TIMER_ID timer_id);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
