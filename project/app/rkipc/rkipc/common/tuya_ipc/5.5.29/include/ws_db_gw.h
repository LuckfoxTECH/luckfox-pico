/**
 * @file ws_db_gw.h
 * @brief TUYA storage service
 * @version 0.1
 * @date 2019-08-28
 *
 * @copyright Copyright 2019-2021 Tuya Inc. All Rights Reserved.
 */

#ifndef __TUYA_WS_DB_GW_H__
#define __TUYA_WS_DB_GW_H__

#include "gw_intf.h"
#include "tuya_ws_db.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Save base info into storage
 *
 * @param[in] bi Base info, see GW_BASE_IF_S
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET wd_gw_base_if_write(IN GW_BASE_IF_S *bi);

/**
 * @brief Read base info from storage
 *
 * @param[out] bi Base info, see GW_BASE_IF_S
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET wd_gw_base_if_read(OUT GW_BASE_IF_S *bi);

/**
 * @brief Save work state info into storage
 *
 * @param[in] wsm Work state info, see GW_WORK_STAT_MAG_S
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET wd_gw_wsm_write(IN GW_WORK_STAT_MAG_S *wsm);

/**
 * @brief Read work state info from storage
 *
 * @param[out] wsm Work state info, see GW_WORK_STAT_MAG_S
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET wd_gw_wsm_read(OUT GW_WORK_STAT_MAG_S *wsm);

/**
 * @brief Save description info into storage
 *
 * @param[in] ai Description info, see GW_DESC_IF_S
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET wd_gw_desc_if_write(IN GW_DESC_IF_S *di);

/**
 * @brief Read description info from storage
 *
 * @param[out] di Description info, see GW_DESC_IF_S
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET wd_gw_desc_if_read(OUT GW_DESC_IF_S *di);

/**
 * @brief Delete description info from storage
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET wd_gw_desc_if_delete(VOID_T);

/**
 * @brief Save activation info into storage
 *
 * @param[in] ai Activation info, see GW_ACTV_IF_S
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET wd_gw_actv_if_write(IN GW_ACTV_IF_S *ai);

/**
 * @brief Read activation info from storage
 *
 * @param[in] ai Activation info, see GW_ACTV_IF_S
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET wd_gw_actv_if_read(OUT GW_ACTV_IF_S *ai);

/**
 * @brief Save schema info into storage
 *
 * @param[in] s_id Schema ID
 * @param[in] schema Schema buffer
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET wd_gw_schema_write(IN CONST CHAR_T *s_id, IN CONST CHAR_T *schema);

/**
 * @brief Read schema info from storage
 *
 * @param[in] s_id Schema ID
 * @param[out] schema Schema buffer
 *
 * @note Caller shall free schema buffer manualy
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET wd_gw_schema_read(IN CONST CHAR_T *s_id, OUT CHAR_T **schema);

/**
 * @brief Delete schema and its version from storage
 *
 * @param[in] s_id Schema ID
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET wd_gw_schema_delete(IN CONST CHAR_T *s_id);

/**
 * @brief Save schema version into storage
 *
 * @param[in] s_id Schema ID
 * @param[in] ver Schema version buffer
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET wd_gw_schema_write_ver(IN CONST CHAR_T *s_id, IN CONST CHAR_T *ver);

/**
 * @brief Read schema version from storage
 *
 * @param[in] s_id Schema ID
 * @param[out] ver Schema version buffer
 *
 * @note Caller shall free schema version buffer manualy
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET wd_gw_schema_read_ver(IN CONST CHAR_T *s_id, OUT CHAR_T **ver);

#ifdef __cplusplus
}
#endif

#endif // __TUYA_WS_DB_GW_H__
