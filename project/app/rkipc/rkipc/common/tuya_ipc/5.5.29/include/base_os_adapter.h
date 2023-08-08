/**
 * @file base_os_adapter.h
 * @brief Common process - Initialization
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */
#ifndef _BASE_OS_ADAPTER_H
#define _BASE_OS_ADAPTER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialization for system interface adaption.
 *
 * @param void
 *
 * @note This API is used for Initialization for system interface adaption.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_os_intf_init(void);

#ifdef __cplusplus
}
#endif

#endif /* _TUYA_OS_ADAPTER_H */
