/**
 * @file direct_connect_tuya.h
 * @brief Common process - tuya activate component
 * @version 0.1
 * @date 2020-05-13
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __DIRECT_CONNECT_TUYA_H__
#define __DIRECT_CONNECT_TUYA_H__

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief callback when received token
 *
 * @param[in] token send from cloud
 *
 */
typedef VOID (*REV_BIND_TOKEN_CB)(IN CONST CHAR_T *token);

/**
 * @brief connect tuya could struct info
 */
typedef struct {
	/** uuid, assigned by TUYA */
	CHAR_T *p_uuid;
	/** key, assigned by TUYA */
	CHAR_T *p_authkey;
	/** region, assigned by TUYA */
	CHAR_T *p_region;
	/** region, assigned by TUYA */
	CHAR_T *p_env;
	/** received token callback */
	REV_BIND_TOKEN_CB rev_token_cb;
} DIRECT_CON_INPUT_S;

/**
 * @brief connect tuya cloud
 *
 * @param[in] p_input connect cloud para
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET direct_connect_tuya_cloud(IN CONST DIRECT_CON_INPUT_S *p_input);

/**
 * @brief check if connect mqtt broker
 *
 * @return 0 not connected,else connected
 */
UINT8_T mqtt_direct_connected();

/**
 * @brief close activate
 *
 */
#if defined(QRCODE_ACTIVE_MODE) && (QRCODE_ACTIVE_MODE == 1)
VOID tuya_qrcode_active_close(VOID);
VOID tuya_qrcode_set_appid(CONST CHAR_T *appid);
#endif

#ifdef __cplusplus
}
#endif
#endif
