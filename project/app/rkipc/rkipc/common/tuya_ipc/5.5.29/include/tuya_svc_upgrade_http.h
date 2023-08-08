/**
 * @file tuya_svc_upgrade_http.h
 * @brief TUYA OTA service
 * @version 0.1
 * @date 2019-08-28
 *
 * @copyright Copyright 2019-2021 Tuya Inc. All Rights Reserved.
 */

#ifndef __TUYA_SVC_UPGRADE_HTTP_H__
#define __TUYA_SVC_UPGRADE_HTTP_H__

#include "tuya_base_utilities.h"
#include "tuya_svc_upgrade.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Retrieve silent upgrade info from cloud
 *
 * @param[in] sub_id ID of sub-device, or NULL for GW
 * @param[out] result JSON encoded data
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET http_device_upgrade_info_get_silent(IN CONST CHAR_T *sub_id, OUT ty_cJSON **result);

/**
 * @brief Retrieve default upgrade info from cloud
 *
 * @param[in] dev_id ID of sub-device, or NULL for GW
 * @param[in] tp OTA channel, see DEV_TYPE_T
 * @param[out] result JSON encoded data
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET http_device_upgrade_info_get(const CHAR_T *dev_id, const DEV_TYPE_T tp,
                                         ty_cJSON **result);

/**
 * @brief Sync upgrade status to cloud
 *
 * @param[in] dev_id ID of sub-device, or NULL for GW
 * @param[in] tp OTA channel, see DEV_TYPE_T
 * @param[in] status Upgrade status, see TI_UPGRD_STAT_S
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET http_device_upgrade_stat_update(const CHAR_T *dev_id, const DEV_TYPE_T tp,
                                            const BYTE_T status);

/**
 * @brief Retrieve lastest schema from cloud
 *
 * @param[in] schemaId ID of local schema
 * @param[in] version Version of local schema
 * @param[in] nodeId ID of sub-device, or NULL for GW
 * @param[out] result JSON encoded data
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET http_device_schema_newest_get(const char *schemaId, const char *version,
                                          const char *nodeId, ty_cJSON **result);

/**
 * @brief Sync schema upgrade result to cloud
 *
 * @param[in] schemaId ID of local schema
 * @param[in] version Version of local schema
 * @param[in] nodeIds IDs of sub-device, or NULL for GW
 * @param[in] result Upgrade result
 * @param[in] result Reason
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET http_device_schema_version_update(const char *schemaId, const char *version,
                                              ty_cJSON *nodeIds, const char *result,
                                              const char *reason);

#ifdef __cplusplus
}
#endif

#endif
