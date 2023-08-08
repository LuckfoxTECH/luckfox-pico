/**
 * @file tuya_hal_wired.h
 * @brief Common process - Initialization
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */
#ifndef __TUYA_HAL_WIRED_H__
#define __TUYA_HAL_WIRED_H__

#include "tuya_hal_network.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get wired ethernet ip info
 *
 * @param[in] ip: the ip addr info
 *
 * @note This API is used for getting wired ethernet ip info.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_hal_wired_get_ip(NW_IP_S *ip);

/**
 * @brief Check whether the hardware is connected to internet
 *
 * @param VOID
 *
 * @note This API is used to check whether the hardware is connected to internet.
 *
 * @return true on connected, false on disconnected.
 */
BOOL_T tuya_hal_wired_station_conn(VOID);

/**
 * @brief Get wired ethernet mac information
 *
 * @param[out] mac: the mac information
 *
 * @note This API is used for getting wired ethernet mac information
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_hal_wired_get_mac(NW_MAC_S *mac);

/**
 * @brief Set wired ethernet mac information
 *
 * @param[in] mac: the mac information
 *
 * @note This API is used for setting wired ethernet mac information
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_hal_wired_set_mac(const NW_MAC_S *mac);

/**
 * @brief If the hardware has a wifi interface, and user want to connect wifi, tuya-app will notify
 * current wifi ssid and passwd, and tuya-sdk will nofity user with this function. user should
 * implement wifi connect process in this function.
 *
 * @param[in] ssid: ssid of router
 * @param[in] passwd: password of router
 *
 * @note This API is used for setting ssid and password of the router.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_hal_wired_wifi_set_station_connect(const char *ssid, const char *passwd);

/**
 * @brief Check whether the hardware need to config
 *
 * @param VOID
 *
 * @note This API is used for checking whether the hardware need to config.
 * if the hardware has a wifi interface, and user want to connect wifi, then return true.
 * if the hardware does not have a wifi interface, or user do not want to connect wifi, return
 * false.
 *
 * @return TRUE or FALSE
 */
BOOL_T tuya_hal_wired_wifi_need_cfg(VOID);

/**
 * @brief Get wifi rssi
 *
 * @param[out] rssi: wifi rssi
 *
 * @note This API is used for getting wifi rssi.
 * if the hardware has a wifi interface and wifi is connected, return wifi rssi, or return null.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_hal_wired_wifi_station_get_conn_ap_rssi(int8_t *rssi);

/**
 * @brief Get network status
 *
 * @param[out] stat: the status of network
 *
 * @note This API is used for getting network status.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_hal_wired_get_nw_stat(GW_BASE_NW_STAT_T *stat);

/**
 * @brief Get wired connect status
 *
 * @param[out] stat: the status of network
 *
 * @note This API is used for getting wired connect status.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_hal_wired_if_connect_internet(BOOL_T *stat);

/**
 * @brief Wired interface initialization
 *
 * @param VOID
 *
 * @note This API is used for initialization of wired interface.
 *
 * @return VOID
 */
VOID_T tuya_os_wired_intf_init(VOID_T);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
