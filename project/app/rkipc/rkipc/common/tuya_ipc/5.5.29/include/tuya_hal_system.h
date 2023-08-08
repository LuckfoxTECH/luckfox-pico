/**
 * @file tuya_hal_system.h
 * @brief Common process - Initialization
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */
#ifndef _TUYA_HAL_SYSTEM_H
#define _TUYA_HAL_SYSTEM_H

#include "tuya_os_adapter.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get system ticket count
 *
 * @param VOID
 *
 * @note This API is used to get system ticket count.
 *
 * @return system ticket count
 */
SYS_TICK_T tuya_hal_get_systemtickcount(VOID_T);

/**
 * @brief Get time in MS per system ticket
 *
 * @param VOID
 *
 * @note This API is used to get time in MS per system ticket.
 *
 * @return time in MS
 */
UINT_T tuya_hal_get_tickratems(VOID_T);

/**
 * @brief System sleep
 *
 * @param[in] msTime: time in MS
 *
 * @note This API is used for system sleep.
 *
 * @return VOID
 */
VOID_T tuya_hal_system_sleep(const unsigned long msTime);

/**
 * @brief System reset
 *
 * @param VOID
 *
 * @note This API is used for system reset.
 *
 * @return VOID
 */
VOID_T tuya_hal_system_reset(VOID_T);

/**
 * @brief Watch dog init and start
 *
 * @param[in] timeval: interval time for watch dog detection
 *
 * @note This API is used to init and start watch dog.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
unsigned int tuya_hal_watchdog_init_start(const unsigned int timeval);

/**
 * @brief Watch dog refresh
 *
 * @param VOID
 *
 * @note This API is used to refresh watch dog.
 *
 * @return VOID
 */
VOID_T tuya_hal_watchdog_refresh(VOID_T);

/**
 * @brief Watch dog stop
 *
 * @param VOID
 *
 * @note This API is used to stop watch dog.
 *
 * @return VOID
 */
VOID_T tuya_hal_watchdog_stop(VOID_T);

/**
 * @brief Get free heap size
 *
 * @param VOID
 *
 * @note This API is used for getting free heap size.
 *
 * @return size of free heap
 */
INT_T tuya_hal_system_getheapsize(VOID_T);

/**
 * @brief Get system reset reason
 *
 * @param VOID
 *
 * @note This API is used for getting system reset reason.
 *
 * @return reset reason of system
 */
TY_RST_REASON_E tuya_hal_system_get_rst_info(VOID_T);

/**
 * @brief Get system extend reset reason
 *
 * @param[out] ext_info: extend reset reason information
 *
 * @note This API is used for getting system extend reset reason.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_hal_system_get_rst_ext_info(CHAR_T **ext_info);

/**
 * @brief Get a random number in the specified range
 *
 * @param[in] range: range
 *
 * @note This API is used for getting a random number in the specified range
 *
 * @return a random number in the specified range
 */
int tuya_hal_get_random_data(const unsigned int range);

/**
 * @brief Set the low power mode of CPU
 *
 * @param[in] en: enable switch
 * @param[in] en: cpu sleep mode
 *
 * @note This API is used for setting the low power mode of CPU.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_hal_set_cpu_lp_mode(const BOOL_T en, const TY_CPU_SLEEP_MODE_E mode);

/**
 * @brief System interface initialization
 *
 * @param VOID
 *
 * @note This API is used for initialization of system interface.
 *
 * @return VOID
 */
VOID_T tuya_os_system_intf_init(VOID_T);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
