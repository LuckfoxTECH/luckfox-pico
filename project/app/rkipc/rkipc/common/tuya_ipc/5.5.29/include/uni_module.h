/**
 * @file uni_module.h
 * @author nzy@tuya.com
 * @brief Common process - Initialization
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */
#ifndef _UNI_MODULE_H
#define _UNI_MODULE_H

#include "tuya_base_utilities.h"
#include "tuya_cloud_types.h"
#include "uni_msg_queue.h"

#ifdef __cplusplus
extern "C" {
#endif

// the handle of the module
typedef PVOID_T MODULE_HANDLE;

/**
 * @brief Release module handle.
 *
 * @param[in] moduleHandle: the handle of the module
 *
 * @note This API is used for releasing module handle.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ReleaseModule(IN CONST MODULE_HANDLE moduleHandle);

/**
 * @brief Create and start the module
 *
 * @param[out] pModuleHandle: the handle of the module
 * @param[in] enter: the construct function of the module
 * @param[in] exit: the deconstructor function of the module
 * @param[in] thrd_param: the parameaters of the thread
 *
 * @note This API is used for creating and starting the module
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET CreateModuleAndStart(OUT MODULE_HANDLE *pModuleHandle, IN CONST P_CONSTRUCT_FUNC enter,
                                 IN CONST P_EXTRACT_FUNC exit, IN CONST THRD_PARAM_S *thrd_param);

/**
 * @brief Post a message.
 *
 * @param[in] moduleHandle: the handle of the module
 * @param[in] msgID: message id
 * @param[in] pMsgData: message data
 * @param[in] msgDataLen: message data len
 *
 * @note This API is used for posting a message
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ModulePostMessage(IN CONST MODULE_HANDLE moduleHandle, IN CONST MSG_ID msgID,
                              IN CONST P_MSG_DATA pMsgData, IN CONST MSG_DATA_LEN msgDataLen);

/**
 * @brief Post a message with memory alloc
 *
 * @param[in] moduleHandle: the handle of the module
 * @param[in] msgID: message id
 * @param[in] pMsgData: message data
 * @param[in] msgDataLen: message data len
 *
 * @note This API is used for posting a message with memory alloc, the param of message data can be
 * local variable
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ModPostMsgMalloc(IN CONST MODULE_HANDLE moduleHandle, IN CONST MSG_ID msgID,
                             IN CONST P_MSG_DATA pMsgData, IN CONST MSG_DATA_LEN msgDataLen);

/**
 * @brief Post a message instantly
 *
 * @param[in] moduleHandle: the handle of the module
 * @param[in] msgID: message id
 * @param[in] pMsgData: message data
 * @param[in] msgDataLen: message data len
 *
 * @note This API is used for posting a message instantly
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ModulePostInstancyMsg(IN CONST MODULE_HANDLE moduleHandle, IN CONST MSG_ID msgID,
                                  IN CONST P_MSG_DATA pMsgData, IN CONST MSG_DATA_LEN msgDataLen);

/**
 * @brief Post a instant message with memory alloc
 *
 * @param[in] moduleHandle: the handle of the module
 * @param[in] msgID: message id
 * @param[in] pMsgData: message data
 * @param[in] msgDataLen: message data len
 *
 * @note This API is used for posting a instant message with memory alloc, the param of message data
 * can be local variable
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ModPostInstancyMsgMalloc(IN CONST MODULE_HANDLE moduleHandle, IN CONST MSG_ID msgID,
                                     IN CONST P_MSG_DATA pMsgData,
                                     IN CONST MSG_DATA_LEN msgDataLen);

/**
 * @brief Regist callback function for message.
 *
 * @param[in] moduleHandle: the handle of the module
 * @param[in] msg_cb: message callback function
 * @param[out] msg_id: message id
 *
 * @note This API is used for registing message's callback function.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ModuleRegisterMsgCb(IN CONST MODULE_HANDLE moduleHandle, IN CONST MSG_CALLBACK msg_cb,
                                OUT MSG_ID *msg_id);

/**
 * @brief Unregist callback function for message.
 *
 * @param[in] moduleHandle: the handle of the module
 * @param[in] msgID: message id
 *
 * @note This API is used for unRegisting message's callback function.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ModuleUnregisterMsgCb(IN CONST MODULE_HANDLE moduleHandle, IN CONST MSG_ID msgID);

/**
 * @brief Create timer message handle.
 *
 * @param[in] moduleHandle: the handle of the module
 * @param[in] data: message data
 * @param[in] cb: callback function
 * @param[out] tm_msg: timer message info
 *
 * @note This API is used for creating a timer message handle
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET module_cr_tm_msg_hand(IN CONST MODULE_HANDLE moduleHandle, IN CONST VOID *data,
                                  IN CONST TM_MSG_CB cb, OUT TM_MSG_S **tm_msg);

/**
 * @brief Start the timer message.
 *
 * @param[in] tm_msg: timer message info
 * @param[in] timeCycle: cycle time of the timer
 * @param[in] timer_type: timer type, cycle or once
 *
 * @note This API is used for starting the timer message
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET module_start_tm_msg(IN CONST TM_MSG_S *tm_msg, IN CONST TIME_MS timeCycle,
                                IN CONST TIMER_TYPE timer_type);

/**
 * @brief Stop the timer message.
 *
 * @param[in] tm_msg: timer message info
 *
 * @note This API is used for stopping the timer message
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET module_stop_tm_msg(IN CONST TM_MSG_S *tm_msg);

/**
 * @brief Release the timer message.
 *
 * @param[in] tm_msg: timer message info
 *
 * @note This API is used for releasing the timer message
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET module_release_tm_msg(IN CONST TM_MSG_S *tm_msg);

/**
 * @brief Get message node count.
 *
 * @param[in] moduleHandle: the handle of the module
 * @param[out] pMsgNodeNum: the message node count in the module
 *
 * @note This API is used for getting the count of message node.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ModuleGetMsgNodeNum(IN CONST MODULE_HANDLE moduleHandle, OUT PINT_T pMsgNodeNum);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
