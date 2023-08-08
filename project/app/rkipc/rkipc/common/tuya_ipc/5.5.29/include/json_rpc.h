/**
 * @file lan_rpc.h
 * @brief Common process - json rpc
 * @version 0.1
 * @date 2020-03-16
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __JSON_RPC_H__
#define __JSON_RPC_H__

#include "cJSON.h"
#include "tuya_cloud_types.h"

#define JRPC_PASER_ERROR -32700
#define JPRC_INVALID_REQUEST -32600
#define JPRC_METHOD_NOT_FOUND -32601

typedef enum {
	JRPC_REQUEST = 0,
	JRPC_RESPONSE,
	JRPC_NOTIFICATION,
	JRPC_ERROR,
	JRPC_WRONG_OBJECT
} JRPC_TYPE_E;

typedef enum { JRPC_PARAMS = 0, JRPC_RESULT } JRPC_NODE_TYPE_E;

typedef struct {
	cJSON *json;
	UINT8_T *binary;
	JRPC_TYPE_E type;
} JRPC_MSG_S;

/**
 * @brief create request
 *
 * @param[in] msg refer to JRPC_MSG_S
 * @param[in] method method
 * @param[in] id node id
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET jrpc_create_request(JRPC_MSG_S *msg, UINT8_T *method, INT_T id);

/**
 * @brief create response
 *
 * @param[in] msg refer to JRPC_MSG_S
 * @param[in] id node id
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET jrpc_create_response(JRPC_MSG_S *msg, INT_T id);

/**
 * @brief create error
 *
 * @param[in] msg refer to JRPC_MSG_S
 * @param[in] id node id
 * @param[in] code json int
 * @param[in] message json message
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET jrpc_create_error(JRPC_MSG_S *msg, INT_T id, INT_T code, INT8_T *message);

/**
 * @brief create notification
 *
 * @param[in] msg refer to JRPC_MSG_S
 * @param[in] method refer to JRPC_NODE_TYPE_E
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET jrpc_create_notification(JRPC_MSG_S *msg, UINT8_T *method);

/**
 * @brief write int
 *
 * @param[in] msg refer to JRPC_MSG_S
 * @param[in] type refer to JRPC_NODE_TYPE_E
 * @param[in] key json key
 * @param[in] value json value
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET jrpc_write_int(JRPC_MSG_S *msg, UINT8_T type, UINT8_T *key, INT_T value);

/**
 * @brief write string
 *
 * @param[in] msg refer to JRPC_MSG_S
 * @param[in] type refer to JRPC_NODE_TYPE_E
 * @param[in] key json key
 * @param[in] value json value
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET jrpc_write_string(JRPC_MSG_S *msg, UINT8_T type, UINT8_T *key, UINT8_T *value);

/**
 * @brief read int
 *
 * @param[in] msg refer to JRPC_MSG_S
 * @param[in] type refer to JRPC_NODE_TYPE_E
 * @param[in] key json key
 * @param[out] value json value
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET jrpc_read_int(JRPC_MSG_S *msg, UINT8_T type, UINT8_T *key, INT_T *value);

/**
 * @brief read string
 *
 * @param[in] msg refer to JRPC_MSG_S
 * @param[in] type refer to JRPC_NODE_TYPE_E
 * @param[in] key json key
 * @param[out] value json value
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET jrpc_read_string(JRPC_MSG_S *msg, UINT8_T type, UINT8_T *key, UINT8_T **value);

/**
 * @brief read method
 *
 * @param[in] msg refer to JRPC_MSG_S
 * @param[out] method value
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET jrpc_read_method(JRPC_MSG_S *msg, UINT8_T **method);

/**
 * @brief read error
 *
 * @param[in] msg refer to JRPC_MSG_S
 * @param[out] code error code
 * @param[out] message error message
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET jrpc_read_error(JRPC_MSG_S *msg, INT_T *code, UINT8_T **message);

/**
 * @brief binary write
 *
 * @param[in] msg refer to LAN_RPC_NODE_OUTPUT_CB
 * @param[in] type refer to JRPC_NODE_TYPE_E
 * @param[in] key json key
 * @param[in] binary write value
 * @param[in] size write size
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET jrpc_write_binary(JRPC_MSG_S *msg, UINT8_T type, INT8_T *key, UINT8_T *binary,
                              UINT16_T size);

/**
 * @brief binary read
 *
 * @param[in] msg refer to LAN_RPC_NODE_OUTPUT_CB
 * @param[in] type refer to JRPC_NODE_TYPE_E
 * @param[in] key json key
 * @param[out] binary read value
 * @param[out] size read size
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET jrpc_read_binary(JRPC_MSG_S *msg, UINT8_T type, INT8_T *key, UINT8_T **binary,
                             UINT16_T *size);

/**
 * @brief json rpc delete
 *
 * @param[in] msg refer to JRPC_MSG_S
 *
 */
VOID jrpc_delete(JRPC_MSG_S *msg);

#endif
