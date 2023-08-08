/**
 * @file tuya_devos_activate.h
 * @brief Common process - Activation
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 */

#ifndef TUYA_DEVOS_ACTIVATE_H
#define TUYA_DEVOS_ACTIVATE_H

#include "tuya_cloud_com_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	UINT_T resved;
} tuya_activate_t;

/**
 * @brief Initialize activation module
 *
 * @param[in] param Init param, see tuya_activate_t
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_devos_activate_init(tuya_activate_t *param);

/**
 * @brief Start activate process
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_devos_activate_start(VOID);

/**
 * @brief Stop activate process
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_devos_activate_stop(VOID);

/**
 * @brief Set user defined features(udf) for activation
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET gw_set_udf(IN CONST CHAR_T *udf);

/**
 * @brief Get user defined features(udf) for activation
 *
 * @return NULL on error
 */
CHAR_T *gw_get_udf(VOID);

/**
 * @brief Set election enable or not
 *
 * @param[in] enable election is supported or not
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET gw_active_set_support_election(BOOL_T enable);

/**
 * @brief Get election enable or not
 *
 * @return TRUE on enable, False on disable
 */
BOOL_T gw_active_get_support_election(VOID);

/**
 * @brief Set extension params for activation
 *
 * @param[in] param Extension params
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET gw_active_set_ext_param(IN CHAR_T *param);

/**
 * @brief Get extension params for activation
 *
 * @return NULL on error
 */
CHAR_T *gw_active_get_ext_param(VOID);

/**
 * @brief Parse activation result
 *
 * @param[in] result Json encoded activation result
 * @param[in] isWifiActive Is activated by Wi-Fi or not
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET gw_active_result_parse(ty_cJSON *result, bool isWifiActive);

/**
 * @brief Set activation state
 *
 * @param[in] stat Work state, see GW_WORK_STAT_T
 */
VOID set_gw_active(GW_WORK_STAT_T stat);

/**
 * @brief Get activation state
 *
 * @return Work state, see GW_WORK_STAT_T
 */
GW_WORK_STAT_T get_gw_active(VOID);

#ifdef __cplusplus
}
#endif

#endif // TUYA_DEVOS_ACTIVATE_H
