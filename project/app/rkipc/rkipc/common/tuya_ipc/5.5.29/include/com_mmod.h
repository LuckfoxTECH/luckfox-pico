/**
 * @file com_mmod.h
 * @author nzy@tuya.com
 * @brief Common process - Initialization
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */
#ifndef _COM_MMOD_H
#define _COM_MMOD_H

#include "uni_msg_queue.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Release module handle.
 *
 * @param VOID
 *
 * @note This API is used for releasing module handle.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET cmmod_release(VOID);

/**
 * @brief Init module handle.
 *
 * @param VOID
 *
 * @note This API is used for initializing module handle.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET cmmod_init(VOID);

/**
 * @brief Regist callback function for message.
 *
 * @param[in] msg_cb: message callback function
 * @param[out] msg_id: message id
 *
 * @note This API is used for registing message's callback function.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET cmmod_reg_msgcb(IN CONST MSG_CALLBACK msg_cb, OUT MSG_ID *msg_id);

/**
 * @brief Unregist callback function for message.
 *
 * @param[in] msgID: message id
 *
 * @note This API is used for unRegisting message's callback function.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET cmmod_unreg_msgcb(IN CONST MSG_ID msgID);

/**
 * @brief Post a message.
 *
 * @param[in] msgID: message id
 * @param[in] pMsgData: message data
 * @param[in] msgDataLen: message data len
 *
 * @note This API is used for posting a message
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET cmmod_post_msg(IN CONST MSG_ID msgID, IN CONST P_MSG_DATA pMsgData,
                           IN CONST MSG_DATA_LEN msgDataLen);

/**
 * @brief Post a message with memory alloc
 *
 * @param[in] msgID: message id
 * @param[in] pMsgData: message data
 * @param[in] msgDataLen: message data len
 *
 * @note This API is used for posting a message with memory alloc, the param of message data can be
 * local variable
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET cmmod_post_msg_malloc(IN CONST MSG_ID msgID, IN CONST P_MSG_DATA pMsgData,
                                  IN CONST MSG_DATA_LEN msgDataLen);

/**
 * @brief Post a message instantly
 *
 * @param[in] msgID: message id
 * @param[in] pMsgData: message data
 * @param[in] msgDataLen: message data len
 *
 * @note This API is used for posting a message instantly
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET cmmod_post_instancy_msg(IN CONST MSG_ID msgID, IN CONST P_MSG_DATA pMsgData,
                                    IN CONST MSG_DATA_LEN msgDataLen);

/**
 * @brief Post a instant message with memory alloc
 *
 * @param[in] msgID: message id
 * @param[in] pMsgData: message data
 * @param[in] msgDataLen: message data len
 *
 * @note This API is used for posting a instant message with memory alloc, the param of message data
 * can be local variable
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET cmmod_post_instancy_msg_malloc(IN CONST MSG_ID msgID, IN CONST P_MSG_DATA pMsgData,
                                           IN CONST MSG_DATA_LEN msgDataLen);

/**
 * @brief Create timer message handle.
 *
 * @param[in] cb: callback function
 * @param[in] data: message data
 * @param[out] tm_msg: timer message info
 *
 * @note This API is used for creating a timer message handle
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET cmmod_cr_tm_msg_hand(IN CONST TM_MSG_CB cb, IN CONST VOID *data, OUT TM_MSG_S **tm_msg);

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
OPERATE_RET cmmod_start_tm_msg(IN CONST TM_MSG_S *tm_msg, IN CONST TIME_MS timeCycle,
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
OPERATE_RET cmmod_stop_tm_msg(IN CONST TM_MSG_S *tm_msg);

/**
 * @brief Release the timer message.
 *
 * @param[in] tm_msg: timer message info
 *
 * @note This API is used for releasing the timer message
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET cmmod_release_tm_msg(IN CONST TM_MSG_S *tm_msg);

/**
 * @brief Get message node count.
 *
 * @param VOID
 *
 * @note This API is used for getting the count of message node.
 *
 * @return the count of message node.
 */
INT_T cmmod_get_msg_node_num(VOID);

#ifdef __cplusplus
}
#endif
#endif
