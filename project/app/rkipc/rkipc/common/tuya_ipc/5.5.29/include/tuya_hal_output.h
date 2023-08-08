/**
 * @file tuya_hal_output.h
 * @brief Common process - Initialization
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */
#ifndef _TUYA_HAL_OUTPUT_H
#define _TUYA_HAL_OUTPUT_H

#include "tuya_os_adapter.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Output log information
 *
 * @param[in] str: log information
 *
 * @note This API is used for outputing log information
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
VOID_T tuya_hal_output_log(CONST CHAR_T *str);

/**
 * @brief Close log port
 *
 * @param VOID
 *
 * @note This API is used for closing log port.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_hal_log_close(VOID_T);

/**
 * @brief Open log port
 *
 * @param VOID
 *
 * @note This API is used for openning log port.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_hal_log_open(VOID_T);

/**
 * @brief Output interface initialization
 *
 * @param VOID
 *
 * @note This API is used for initialization of output interface.
 *
 * @return VOID
 */
VOID_T tuya_os_output_intf_init(VOID_T);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
