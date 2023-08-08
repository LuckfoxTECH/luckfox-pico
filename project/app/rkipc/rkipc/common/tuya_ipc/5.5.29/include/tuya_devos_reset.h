/**
 * @file tuya_devos_reset.h
 * @brief Common process - Reset
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 */

#ifndef TUYA_DEVOS_RESET_H
#define TUYA_DEVOS_RESET_H

#include "tuya_cloud_com_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	UINT_T resved;
} tuya_reset_t;

/**
 * @brief Initialize reset module
 *
 * @param[in] param See tuya_reset_t
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_devos_reset_init(tuya_reset_t *param);

/**
 * @brief Faset reset device to register state
 *
 * @param[in] mthd Wi-Fi netconfig method, see GW_WF_CFG_MTHD_SEL
 * @param[in] wifi_start_mode Wi-Fi start mode, see GW_WF_START_MODE
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET gw_fast_unactive(IN CONST GW_WF_CFG_MTHD_SEL mthd,
                             IN CONST GW_WF_START_MODE wifi_start_mode);

/**
 * @brief Reset device to unregister state
 *
 * @param[in] rst_t Reset type, see GW_RESET_S
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET gw_unregister(IN CONST GW_RESET_S *rst_t);

/**
 * @brief Reset device to register state
 *
 * @param[in] rst_t Reset type, see GW_RESET_S
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET gw_unactive(IN CONST GW_RESET_S *rst_t);

/**
 * @brief Get last reset region
 *
 * @param[in] region buffer with length REGION_LEN + 1
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET gw_get_last_reset_region(INOUT CONST CHAR_T *region);

#ifdef __cplusplus
}
#endif

#endif // TUYA_DEVOS_RESET_H
