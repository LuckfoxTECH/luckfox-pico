/**
 * @file uni_msg_queue.h
 * @author nzy@tuya.com
 * @brief Common process - Initialization
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */
#ifndef _UNI_MSG_QUEUE_H
#define _UNI_MSG_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sys_timer.h"
#include "tuya_cloud_types.h"
#include "uni_pointer.h"

#define USE_SEM_COUNTING 1
#if !(USE_SEM_COUNTING)
#include "tuya_hal_system.h"
#define PROC_MSG_DELAY 100
#endif

typedef PVOID_T MSG_QUE_HANDLE; // message queue handle

typedef USHORT_T MSG_ID;     // message id
typedef PVOID_T P_MSG_DATA;  // message data
typedef UINT_T MSG_DATA_LEN; // message data lenth

#define UNVALUED_MSGID 0xffff // invalid message id

/**
 * @brief Definition of message type
 */
typedef UINT_T MSG_TYPE;
#define INSTANCY_MESSAGE 0 // instant message type
#define NORMAL_MESSAGE 1   // normal message type

/**
 * @brief Init param of message
 */
typedef struct {
	MSG_ID msgID;            // message id
	P_MSG_DATA pMsgData;     // message data
	MSG_DATA_LEN msgDataLen; // message data len
} MESSAGE, *P_MESSAGE;

/**
 * @brief message list
 */
typedef struct {
	LIST_HEAD listHead; // list head
	MESSAGE msg;        // message info
} MSG_LIST, *P_MSG_LIST;

// message processing callback
typedef VOID (*MSG_CALLBACK)(MESSAGE *msg);

struct s_tm_msg;
typedef VOID (*TM_MSG_CB)(struct s_tm_msg *tm_msg); // timer message callback

/**
 * @brief timer message definition
 */
typedef struct s_tm_msg {
	TIMER_ID timer;              // timer id
	MSG_QUE_HANDLE msgQueHandle; // timer queue handle
	TM_MSG_CB cb;                // timer callback function
	MSG_ID msgID;                // message id
	VOID *data;                  // message data
} TM_MSG_S;

/**
 * @brief message entry definition
 */
typedef struct {
	MSG_ID *mid;         // message id
	MSG_CALLBACK msg_cb; // message callback
} MSG_ENTRY_S;

/**
 * @brief timer message entry definition
 */
typedef struct {
	TM_MSG_S **tm_msg;    // timer message info
	TM_MSG_CB tmm_msg_cb; // timer message callback
} TM_MSG_ENTRY_S;

/**
 * @brief Create and init the message queue
 *
 * @param[in] pMsgQueHandle: the handle of message queue
 *
 * @note This API is used for initializing the message queue
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET CreateMsgQueAndInit(OUT MSG_QUE_HANDLE *pMsgQueHandle);

/**
 * @brief Add message to queue
 *
 * @param[in] msgQueHandle: the handle of the message queue
 * @param[in] msgID: message id
 * @param[in] pMsgData: message data
 * @param[in] pMsgData: message data lenth
 * @param[in] msgType: message type
 *
 * @note This API is used for adding a timer to the queue
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET AddMsgNodeToQueue(IN CONST MSG_QUE_HANDLE msgQueHandle, IN CONST MSG_ID msgID,
                              IN CONST P_MSG_DATA pMsgData, IN CONST MSG_DATA_LEN msgDataLen,
                              IN CONST MSG_TYPE msgType);

/**
 * @brief Get message node from queue
 *
 * @param[in] msgQueHandle: the handle of the message queue
 * @param[in] msgID: message id
 * @param[out] ppMsgListNode: message node
 *
 * @note This API is used for getting message node from the queue
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET GetMsgNodeFromQueue(IN CONST MSG_QUE_HANDLE msgQueHandle, IN CONST MSG_ID msgID,
                                OUT P_MSG_LIST *ppMsgListNode);

/**
 * @brief Get the first message node from queue
 *
 * @param[in] msgQueHandle: the handle of the message queue
 * @param[out] ppMsgListNode: message node
 *
 * @note This API is used for getting the first message node from queue
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET GetFirstMsgFromQueue(IN CONST MSG_QUE_HANDLE msgQueHandle,
                                 OUT P_MSG_LIST *ppMsgListNode);

/**
 * @brief Get the count of message node from queue
 *
 * @param[in] msgQueHandle: the handle of the message queue
 * @param[out] pMsgNodeNum: the count of message node
 *
 * @note This API is used for getting the first message node from queue
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET GetMsgNodeNum(IN CONST MSG_QUE_HANDLE msgQueHandle, OUT PINT_T pMsgNodeNum);

/**
 * @brief Delete and free the message node from queue
 *
 * @param[in] msgQueHandle: the handle of the message queue
 * @param[in] pMsgListNode: the message node
 *
 * @note This API is used to delete and free the message node from queue
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET DelAndFreeMsgNodeFromQueue(IN CONST MSG_QUE_HANDLE msgQueHandle,
                                       IN CONST P_MSG_LIST pMsgListNode);

/**
 * @brief Release the message queue
 *
 * @param[in] msgQueHandle: the handle of the message queue
 *
 * @note This API is used releasing the message queue
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ReleaseMsgQue(IN CONST MSG_QUE_HANDLE msgQueHandle);

/**
 * @brief Post a message.
 *
 * @param[in] msgQueHandle: the handle of the message queue
 * @param[in] msgID: message id
 * @param[in] pMsgData: message data
 * @param[in] msgDataLen: message data len
 *
 * @note This API is used for posting a message
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET PostMessage(IN CONST MSG_QUE_HANDLE msgQueHandle, IN CONST MSG_ID msgID,
                        IN CONST P_MSG_DATA pMsgData, IN CONST MSG_DATA_LEN msgDataLen);

/**
 * @brief Post a message instantly
 *
 * @param[in] msgQueHandle: the handle of the message queue
 * @param[in] msgID: message id
 * @param[in] pMsgData: message data
 * @param[in] msgDataLen: message data len
 *
 * @note This API is used for posting a message instantly
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET PostInstancyMsg(IN CONST MSG_QUE_HANDLE msgQueHandle, IN CONST MSG_ID msgID,
                            IN CONST P_MSG_DATA pMsgData, IN CONST MSG_DATA_LEN msgDataLen);

/**
 * @brief Wait a message.
 *
 * @param[in] msgQueHandle: the handle of the message queue
 * @param[out] ppMsgListNode: message node
 *
 * @note This API is used for waiting a message
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET WaitMessage(IN CONST MSG_QUE_HANDLE msgQueHandle, OUT P_MSG_LIST *ppMsgListNode);

/**
 * @brief Regist callback function for message.
 *
 * @param[in] msgQueHandle: the handle of the message queue
 * @param[in] msg_cb: message callback function
 * @param[out] msg_id: message id
 *
 * @note This API is used for registing message's callback function.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET RegisterMsgCb(IN CONST MSG_QUE_HANDLE msgQueHandle, IN CONST MSG_CALLBACK msg_cb,
                          OUT MSG_ID *msg_id);

/**
 * @brief Unregist callback function for message.
 *
 * @param[in] msgQueHandle: the handle of the message queue
 * @param[in] msgID: message id
 *
 * @note This API is used for unRegisting message's callback function.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET UnregisterMsgCb(IN CONST MSG_QUE_HANDLE msgQueHandle, IN CONST MSG_ID msgID);

/**
 * @brief Message loop processing.
 *
 * @param[in] msgQueHandle: the handle of the message queue
 *
 * @note This API is used for Message loop processing.
 *
 * @return VOID
 */
VOID MessageLoop(IN CONST MSG_QUE_HANDLE msgQueHandle);

/**
 * @brief Create timer message handle.
 *
 * @param[in] msgQueHandle: the handle of the message queue
 * @param[in] data: message data
 * @param[in] cb: callback function
 * @param[out] tm_msg: timer message info
 *
 * @note This API is used for creating a timer message handle
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET create_tm_msg_hand(IN CONST MSG_QUE_HANDLE msgQueHandle, IN CONST VOID *data,
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
OPERATE_RET start_tm_msg(IN CONST TM_MSG_S *tm_msg, IN CONST TIME_MS timeCycle,
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
OPERATE_RET stop_tm_msg(IN CONST TM_MSG_S *tm_msg);

/**
 * @brief Release the timer message.
 *
 * @param[in] tm_msg: timer message info
 *
 * @note This API is used for releasing the timer message
 *
 * @return VOID
 */
VOID release_tm_msg_hand(IN CONST TM_MSG_S *tm_msg);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
