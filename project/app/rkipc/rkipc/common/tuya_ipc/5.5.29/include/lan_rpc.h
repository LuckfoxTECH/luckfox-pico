/**
 * @file lan_rpc.h
 * @brief Common process - lan rpc
 * @version 0.1
 * @date 2020-03-16
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __LAN_RPC_H__
#define __LAN_RPC_H__

#include "rpc_udp.h"
#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LAN_RPC_ID_LEN (GW_ID_LEN)
#define LAN_RPC_KEY_LEN (LOCAL_KEY_LEN)
#define LAN_RPC_NODE_TIMEOUT 30
#define LAN_RPC_NODE_ADD 0
#define LAN_RPC_NODE_DEL 1
#define LAN_RPC_NODE_REJOIN 2

typedef enum {
	LAN_RPC_SLAVER,
	LAN_RPC_MASTER,
} LAN_RPC_TYPE;

typedef struct {
	UINT_T ip;
	INT_T seqno;
	USHORT_T cap;
	UINT8_T id[LAN_RPC_ID_LEN + 1];
	UINT8_T key[LAN_RPC_KEY_LEN + 1]; // key是非字符串的
} LAN_RPC_NODE_S;

typedef struct {
	UINT8_T owner;
	UINT8_T stat;
	LAN_RPC_NODE_S node;
} LAN_RPC_MASTER_S;

typedef OPERATE_RET (*LAN_RPC_MASTER_GET_CB)(LAN_RPC_MASTER_S *master);
typedef OPERATE_RET (*LAN_RPC_NODE_STAT_CB)(UINT8_T stat, LAN_RPC_NODE_S *node);
typedef VOID (*LAN_RPC_NODE_OUTPUT_CB)(LAN_RPC_NODE_S *node, UINT16_T num, VOID *arg);

typedef struct {
	UINT8_T type;
	LAN_RPC_NODE_STAT_CB node_stat_cb;
	LAN_RPC_MASTER_GET_CB master_get_cb;
} LAN_RPC_CFG_S;

/**
 * @brief get fd
 *
 * @return fd value
 */
INT_T lan_rpc_fd_get(VOID);

/**
 * @brief check timeout
 *
 */
VOID lan_rpc_node_timeout_check(VOID);

/**
 * @brief node add
 *
 * @param[in] node node value
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET lan_rpc_node_add(LAN_RPC_NODE_S *node);

/**
 * @brief found node by ip
 *
 * @param[in] ip key ip
 * @param[out] node result node
 *
 * @return found TRUE,else false
 */
BOOL_T lan_rpc_node_find_by_ip(UINT_T ip, LAN_RPC_NODE_S *node);

/**
 * @brief found ip by id
 *
 * @param[in] id node id
 * @param[out] ip result ip
 *
 * @return found TRUE,else false
 */
BOOL_T lan_rpc_node_find_ip_by_id(UINT8_T *id, UINT_T *ip);

/**
 * @brief node key set
 *
 * @param[in] id node id
 * @param[in] key key value
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET lan_rpc_node_key_set(UINT8_T *id, UINT8_T *key);

/**
 * @brief node output
 *
 * @param[in] node_output_cb refer to LAN_RPC_NODE_OUTPUT_CB
 * @param[in] arg argument
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET lan_rpc_node_output(LAN_RPC_NODE_OUTPUT_CB node_output_cb, VOID *arg);

/**
 * @brief set session key
 *
 * @param[in] session_key key value
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET lan_rpc_session_key_set(UINT8_T *session_key);

/**
 * @brief rpc init
 *
 * @param[in] cfg refer to LAN_RPC_CFG_S
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET lan_rpc_init(LAN_RPC_CFG_S *cfg);

/**
 * @brief slave rpc sync
 *
 * @param[in] requset refer to JRPC_MSG_S
 * @param[in] result refer to JRPC_MSG_S
 * @param[in] timeout timeout
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET lan_rpc_slave_call_sync(JRPC_MSG_S *requset, JRPC_MSG_S *result, UINT_T timeout);

/**
 * @brief master rpc sync
 *
 * @param[in] id node id
 * @param[in] requset refer to JRPC_MSG_S
 * @param[in] result refer to JRPC_MSG_S
 * @param[in] timeout timeout
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET lan_rpc_master_call_sync(UINT8_T *id, JRPC_MSG_S *requset, JRPC_MSG_S *result,
                                     UINT_T timeout);

/**
 * @brief rpc sync
 *
 * @param[in] id node id
 * @param[in] requset refer to JRPC_MSG_S
 * @param[in] cb callback function
 * @param[in] cb_param callback argument
 * @param[in] timeout timeout
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET lan_rpc_call_async(UINT8_T *id, JRPC_MSG_S *requset, RPC_RET_CB cb, VOID *cb_param,
                               UINT_T timeout);

/**
 * @brief rpc receive
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET lan_rpc_recv(VOID);

/**
 * @brief lan rpc method register
 *
 * @param[in] name key name
 * @param[in] cb callback function
 * @param[in] arg callback argument
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET lan_rpc_method_register(CHAR_T *name, RPC_METHOD_CB cb, VOID *arg);

#ifdef __cplusplus
}
#endif
#endif
