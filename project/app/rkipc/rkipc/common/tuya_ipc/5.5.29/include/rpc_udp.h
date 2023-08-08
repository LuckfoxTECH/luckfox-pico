/**
 * @file rpc_udp.h
 * @brief Common process - udp rpc
 * @version 0.1
 * @date 2020-03-16
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __RPC_UDP_H__
#define __RPC_UDP_H__

#include "base_rpc_com.h"
#include "hashmap.h"
#include "json_rpc.h"
#include "tuya_cloud_types.h"
#include "tuya_os_adapter.h"
#include "uni_thread.h"

#define OPRT_RPC_CALL_TIMEOUT -10
#define OPRT_RPC_CALL_ERROR -11
#define RPC_DEFAULT_BUFSIZE 256

typedef enum {
	RPC_CLIENT,
	RPC_SERVER,
	RPC_LOOPIF,
	RPC_CUSTOM,
} RPC_TYPE_E;

typedef enum {
	RPC_BUFSIZE_SET_CMD,
	RPC_CUSTOM_HOST_CMD,
	RPC_KEY_ENABLE_CMD,
	RPC_KEY_SET_CMD,
	RPC_METHOD_UNREGISTER_CMD,
	RPC_EXIT_CMD,
	RPC_CALL_RETRY_CMD,
} RPC_CMD_E;

typedef struct {
	INT_T udp_fd;
	UINT_T flag;
	THRD_HANDLE thread;
	RPC_TYPE_E type;
	BRC_HAND_T brchd;
	MUTEX_HANDLE mutex;
	UINT_T ipaddr;
	UINT_T port;
	UINT8_T key[16];
	map_t method_list;
	UINT16_T buffer_size;
	UINT8_T *buffer;
	UINT8_T default_buffer[RPC_DEFAULT_BUFSIZE];
} TUYA_RPC_S;

/**
 * @brief callback of rpc method
 *
 * @param[in] request refer to JRPC_MSG_S
 * @param[in] arg callback argument
 * @param[in] peer_ip refer to UNW_IP_ADDR_T
 * @param[out] reply refer to JRPC_MSG_S
 *
 */
typedef VOID (*RPC_METHOD_CB)(IN JRPC_MSG_S *request, IN VOID *arg, IN UNW_IP_ADDR_T peer_ip,
                              OUT JRPC_MSG_S *reply);

/**
 * @brief callback of rpc return
 *
 * @param[in] op_ret return code,refer to tuya_error_code.h
 * @param[in] result reference to JRPC_MSG_S
 * @param[in] prv_data
 *
 */
typedef VOID (*RPC_RET_CB)(IN OPERATE_RET op_ret, IN JRPC_MSG_S *result, IN VOID *prv_data);

/**
 * @brief tuya udp rpc init
 *
 * @param[in] rpc  udp rpc description
 * @param[in] type reference to RPC_TYPE_E
 * @param[in] host ip:port
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_rpc_init(TUYA_RPC_S *rpc, RPC_TYPE_E type, UINT8_T *host);

/**
 * @brief create udp rpc task
 *
 * @param[in] rpc  udp rpc description
 * @param[in] stack_size task size
 * @param[in] prio task prio
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_rpc_run(TUYA_RPC_S *rpc, UINT_T stack_size, UINT_T prio);

/**
 * @brief udp rpc method register
 *
 * @param[in] rpc  udp rpc description
 * @param[in] name key name
 * @param[in] cb method callback
 * @param[in] arg callback argument
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_rpc_method_register(TUYA_RPC_S *rpc, CHAR_T *name, RPC_METHOD_CB cb, VOID *arg);

/**
 * @brief rpc sync
 *
 * @param[in] rpc udp rpc description
 * @param[in] requset reference to JRPC_MSG_S
 * @param[in] result reference to JRPC_MSG_S
 * @param[in] timeout
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_rpc_call_sync(TUYA_RPC_S *rpc, JRPC_MSG_S *requset, JRPC_MSG_S *result,
                               UINT_T timeout);

/**
 * @brief rpc control
 *
 * @param[in] rpc  udp rpc description
 * @param[in] cmd control command
 * @param[in] arg contol argument
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_rpc_control(TUYA_RPC_S *rpc, UINT8_T cmd, VOID *arg);

#endif
