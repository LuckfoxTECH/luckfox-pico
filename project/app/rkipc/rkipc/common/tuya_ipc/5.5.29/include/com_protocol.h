/**
 * @file com_protocol.h
 * @brief Common process - common protocol
 * @version 0.1
 * @date 2017-04-12
 *
 * @copyright Copyright 2015-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef _COM_PROTOCOL_H
#define _COM_PROTOCOL_H
#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief parse data
 *
 * @param[in] cmd refer to DP_CMD_TYPE_E
 * @param[in] data origin data
 * @param[in] len data lemgth
 * @param[out] out_data parse data out
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET parse_data_with_cmd(IN CONST DP_CMD_TYPE_E cmd, IN BYTE_T *data, IN CONST INT_T len,
                                OUT CHAR_T **out_data);

/**
 * @brief pack data
 *
 * @param[in] cmd refer to DP_CMD_TYPE_E
 * @param[in] src origin data
 * @param[in] pro pro
 * @param[in] num num
 * @param[out] out pack out data
 * @param[out] out_len pack data length
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET pack_data_with_cmd(IN CONST DP_CMD_TYPE_E cmd, IN CONST CHAR_T *src,
                               IN CONST UINT_T pro, IN CONST UINT_T num, OUT BYTE_T **out,
                               OUT UINT_T *out_len);

/***********************************************************
 *  Function: parse_data_with_cmd
 *  Input: cmd data len
 *  Output: out_data
 *  Return: OPERATE_RET
 ***********************************************************/

/**
 * @brief parse protocol data
 *
 * @param[in] cmd refer to DP_CMD_TYPE_E
 * @param[in] pv verison
 * @param[in] data origin data
 * @param[in] len data length
 * @param[in] key parse key
 * @param[out] out_data parse out
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_parse_protocol_data(IN CONST DP_CMD_TYPE_E cmd, IN CONST CHAR_T *pv,
                                     IN BYTE_T *data, IN CONST INT_T len, IN CONST CHAR_T *key,
                                     OUT CHAR_T **out_data);

/**
 * @brief pack protocol data
 *
 * @param[in] cmd refer to DP_CMD_TYPE_E
 * @param[in] pv verison
 * @param[in] src origin data
 * @param[in] pro pro
 * @param[in] num num
 * @param[in] key pack key
 * @param[out] out pack out
 * @param[out] out_len pack out length
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_pack_protocol_data(IN CONST DP_CMD_TYPE_E cmd, IN CONST CHAR_T *pv,
                                    IN CONST CHAR_T *src, IN CONST UINT_T pro, IN CONST UINT_T num,
                                    IN UINT_T from, IN BYTE_T *key, OUT CHAR_T **out,
                                    OUT UINT_T *out_len);
#ifdef __cplusplus
}
#endif
#endif
