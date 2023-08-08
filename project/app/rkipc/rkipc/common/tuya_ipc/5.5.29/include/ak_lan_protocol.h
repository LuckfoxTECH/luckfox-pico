/**
 * @file ak_lan_protocol.h
 * @brief Common process - airkiss lan protocol
 * @version 0.1
 * @date 2017-04-18
 *
 * @copyright Copyright 2014-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef _AK_LAN_PROTOCOL_H
#define _AK_LAN_PROTOCOL_H
#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief lan fin inform
 *
 * @param[in] random pid
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ak_lan_nwc_fin_inform(IN CONST BYTE_T random);

/**
 * @brief lan disc init
 *
 * @param[in] appid pid
 * @param[in] dev_id dev id
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ak_lan_disc_init(IN CONST CHAR_T *appid, IN CONST CHAR_T *dev_id);

/**
 * @brief lan disc package send
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ak_lan_disc_pack_send(VOID);

/**
 * @brief lan disc package send
 *
 * @param[in] pArg argument
 *
 */
VOID ak_lan_rev_active_task(PVOID_T pArg);

/**
 * @brief get status
 *
 * @return status
 */
INT_T get_ak_active_status(VOID);

#ifdef __cplusplus
}
#endif
#endif
