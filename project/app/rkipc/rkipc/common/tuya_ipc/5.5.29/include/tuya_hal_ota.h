/**
 * @file tuya_hal_ota.h
 * @brief Common process - Initialization
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */
#ifndef _TUYA_HAL_OTA_H_
#define _TUYA_HAL_OTA_H_
#include "tuya_os_adapter.h"
#include <stdbool.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Start upgrade notification
 *
 * @param[in] file_size: file size
 * @param[in] type: ota type
 *
 * @note This API is used for starting upgrade notification
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_hal_ota_start_inform(unsigned int file_size, OTA_TYPE type);

/**
 * @brief Ota data packet processing
 *
 * @param[in] total_len: total lenth of upgrade package
 * @param[in] offset: offset of current data in upgrade package
 * @param[in] data: ota data
 * @param[in] len: ota data length
 * @param[out] remain_len: the length of data that has been distributed internally but not yet
 * processed
 * @param[in] pri_data: reserved data
 *
 * @note This API is used for ota data packet processing
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_hal_ota_data_process(const unsigned int total_len, const unsigned int offset,
                              const unsigned char *data, const unsigned int len,
                              unsigned int *remain_len, void *pri_data);

/**
 * @brief Ota data transfer completion notification
 *
 * @param[in] reset: if need reset or not
 *
 * @note This API is used for ota data transfer completion notification
 * users can do firmware verification and device restart
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_hal_ota_end_inform(bool reset);

/**
 * @brief Ota interface initialization
 *
 * @param VOID
 *
 * @note This API is used for initialization of ota interface.
 *
 * @return VOID
 */
VOID_T tuya_os_ota_intf_init(VOID_T);

#ifdef __cplusplus
}
#endif

#endif
