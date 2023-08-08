/**
 * @file uni_thread.h
 * @brief tuya thread module
 * @version 1.0
 * @date 2019-10-30
 *
 * @copyright Copyright (c) tuya.inc 2019
 *
 */

#ifndef _UNI_THREAD_H
#define _UNI_THREAD_H

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief max length of thread name
 *
 */
#define MAX_THREAD_NAME_LEN 16

/**
 * @brief thread process function
 *
 */
typedef VOID (*P_THRD_FUNC)(PVOID_T pArg);

/**
 * @brief thread handle
 *
 */
typedef PVOID_T THRD_HANDLE;

/**
 * @brief thread construct function
 *
 */
typedef VOID (*P_CONSTRUCT_FUNC)(VOID);

/**
 * @brief thread extract function
 *
 */
typedef VOID (*P_EXTRACT_FUNC)(VOID); // thread extract

/**
 * @brief thread stack size
 *
 */
typedef unsigned int STACK_SIZE;

/**
 * @brief thread running status
 *
 */
typedef INT_T THRD_STA;
#define STATE_EMPTY 0   // thread is inited
#define STATE_RUNNING 1 // thread is running
#define STATE_STOP 2    // thread is stop
#define STATE_DELETE 3  // thread delete status

/**
 * @brief thread priority
 *
 */
typedef unsigned int TRD_PRI;
#define TRD_PRIO_0 5 // high
#define TRD_PRIO_1 4
#define TRD_PRIO_2 3
#define TRD_PRIO_3 2
#define TRD_PRIO_4 1
#define TRD_PRIO_5 0
#define TRD_PRIO_6 0 // low

/**
 * @brief thread parameters
 *
 */
typedef struct {
	STACK_SIZE stackDepth; // stack size
	TRD_PRI priority;      // thread priority
	CHAR_T *thrdname;      // thread name
} THRD_PARAM_S;

/**
 * @brief create and start a tuya sdk thread
 *
 * @param[in] enter: the function called before the thread process called.can be null
 * @param[in] exit: the function called after the thread process called.can be null
 * @param[in] pThrdFunc: the main thread process function
 * @param[in] pThrdFuncArg: the args of the pThrdFunc.can be null
 * @param[in] thrd_param: the param of creating a thread
 * @param[out] pThrdHandle: the tuya sdk thread context
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET CreateAndStart(OUT THRD_HANDLE *pThrdHandle, IN CONST P_CONSTRUCT_FUNC enter,
                           IN CONST P_EXTRACT_FUNC exit, IN CONST P_THRD_FUNC pThrdFunc,
                           IN CONST PVOID_T pThrdFuncArg, IN CONST THRD_PARAM_S *thrd_param);

/**
 * @brief stop and free a tuya sdk thread
 *
 * @param[in] thrdHandle: the input thread context
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET DeleteThrdHandle(IN CONST THRD_HANDLE thrdHandle);

/**
 * @brief check the function caller is in the input thread context
 *
 * @param[in] thrdHandle: the input thread context
 * @param[in] bl: run in self space
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ThreadRunSelfSpace(IN CONST THRD_HANDLE thrdHandle, OUT BOOL_T *bl);

/**
 * @brief get the thread context running status
 *
 * @param[in] thrdHandle: the input thread context
 * @return the thread status
 */
THRD_STA GetThrdSta(IN CONST THRD_HANDLE thrdHandle);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
