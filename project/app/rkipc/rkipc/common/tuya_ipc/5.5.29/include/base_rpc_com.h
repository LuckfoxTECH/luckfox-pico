/**
 * @file base_rpc_com.h
 * @brief Common process - base rpc
 * @version 0.1
 * @date 2018-09-01
 *
 * @copyright Copyright 2018-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef _BASE_RPC_COM_H
#define _BASE_RPC_COM_H

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define COM_KEY_LEN_LMT 16
#define BRC_SET_PRIV_DATA_CMD 0
#define BRC_GET_PRIV_DATA_CMD 1

typedef VOID *BRC_HAND_T;

/**
 * @brief server send callback
 *
 * @param[in] hand refer to BRC_HAND_T
 * @param[in] com_key key
 * @param[in] data send data
 * @param[in] len send length
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
typedef INT_T (*BRC_SEND_FUNC)(IN CONST BRC_HAND_T hand, IN CONST CHAR_T *com_key, IN BYTE_T *data,
                               IN CONST UINT_T len);

/**
 * @brief server received callback
 *
 * @param[in] hand refer to BRC_HAND_T
 * @param[in] com_key key
 * @param[in] input input data
 * @param[in] len data length
 * @param[in] snd_seqno sequence
 * @param[in] enc_key key
 * @param[in] rsp resonsible
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
typedef VOID (*RPC_RECV_CB)(IN BRC_HAND_T hand, IN CONST CHAR_T *com_key, IN CONST VOID *input,
                            IN CONST UINT_T len, IN CONST USHORT_T snd_seqno, IN BYTE_T *enc_key,
                            IN CONST BOOL_T rsp);

/**
 * @brief clinet result callback
 *
 * @param[in] op_ret refer to OPERATE_RET
 * @param[out] output result
 * @param[out] out_len length
 * @param[in] prv_data data
 * @param[in] enc_key key
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
typedef VOID (*RPC_TRANSFER_CB)(IN OPERATE_RET op_ret, OUT VOID *output, OUT UINT_T out_len,
                                IN VOID *prv_data, IN BYTE_T *enc_key);

/**
 * @brief init function
 *
 * @param[in] send refer to BRC_SEND_FUNC
 * @param[in] recv receive callback
 * @param[out] hand refer to BRC_HAND_T
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET cr_and_init_brc_hand(IN BRC_SEND_FUNC send, IN RPC_RECV_CB recv, OUT BRC_HAND_T *hand);

/**
 * @brief data handle
 *
 * @param[in] hand refer to BRC_HAND_T
 * @param[in] com_key key
 * @param[in] data handle data
 * @param[in] len data length
 * @param[in] enc_key key
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET brc_data_recv_process(IN BRC_HAND_T hand, IN CHAR_T *com_key, IN BYTE_T *data,
                                  IN UINT_T len, IN UINT8_T *enc_key);

/**
 * @brief call async
 *
 * @param[in] hand refer to BRC_HAND_T
 * @param[in] com_key key
 * @param[in] input input
 * @param[in] in_len input length
 * @param[in] timeout timeout
 * @param[in] cb refer to RPC_TRANSFER_CB
 * @param[in] pri_data callback argument
 * @param[in] enc_key key
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET rpc_client_call_async(IN BRC_HAND_T hand, IN CHAR_T *com_key, IN VOID *input,
                                  IN UINT_T in_len, IN UINT_T timeout, IN RPC_TRANSFER_CB cb,
                                  IN VOID *pri_data, IN UINT8_T *enc_key);

/**
 * @brief respsond call
 *
 * @param[in] hand refer to BRC_HAND_T
 * @param[in] com_key key
 * @param[in] input input
 * @param[in] in_len input length
 * @param[in] snd_seqno sequence
 * @param[in] enc_key key
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET rpc_serv_respsond_call(IN BRC_HAND_T hand, IN CHAR_T *com_key, IN VOID *input,
                                   IN UINT_T in_len, IN USHORT_T snd_seqno, IN UINT8_T *enc_key);
/**
 * @brief base rpc control
 *
 * @param[in] hand refer to BRC_HAND_T
 * @param[in] cmd control command
 * @param[in] args control argument
 * @param[out] args control argument
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET brc_control(IN BRC_HAND_T hand, IN UCHAR_T cmd, INOUT VOID *args);

#ifdef __cplusplus
}
#endif

#endif
