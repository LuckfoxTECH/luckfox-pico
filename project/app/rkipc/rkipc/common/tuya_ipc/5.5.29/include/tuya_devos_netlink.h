/**
 * @file tuya_devos_netlink.h
 * @brief Common process - Network linkage management
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 */

#ifndef TUYA_DEVOS_NETLINK_H
#define TUYA_DEVOS_NETLINK_H

#include "gw_intf.h"
#include "tuya_cloud_com_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EVT_NETLINK_UP "netlink.up"     // network linkage up
#define EVT_NETLINK_DOWN "netlink.down" // network linkage down

typedef struct {
	UINT_T resved;
} tuya_netlink_t;

/**
 * @brief Initialize network linkage module
 *
 * @param[in] param Init param, see tuya_netlink_t
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_devos_netlink_init(tuya_netlink_t *param);

/**
 * @brief Start network linkage module
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_devos_netlink_start(VOID);

/**
 * @brief Stop network linkage module
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_devos_netlink_stop(VOID);

/**
 * @brief Enable network linkage monitor
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_devos_netlink_monitor_enable(VOID);

/**
 * @brief Disable network linkage monitor
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_devos_netlink_monitor_disable(VOID);

#if defined(WIFI_GW) && (WIFI_GW == 1)
#if defined(GW_SUPPORT_WIRED_WIFI) && (GW_SUPPORT_WIRED_WIFI == 1)
/**
 * @brief Get device network linkage state
 *
 * @param[out] nw_stat Linkage state, see GW_BASE_NW_STAT_T
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET get_base_gw_nw_status(OUT GW_BASE_NW_STAT_T *nw_stat);

/**
 * @brief Get device network state
 *
 * @param[out] nw_stat Network state, see IOT_GW_NET_TYPE_T
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
VOID get_wired_wifi_gw_net_mode(OUT IOT_GW_NET_TYPE_T *net_mode);

/**
 * @brief Get network concurrent state
 *
 * @param[out] nw_stat Concurrent state, see CUR_NET_STATUS_T
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
VOID get_wired_wifi_current_net(OUT CUR_NET_STATUS_T *current_net);

/**
 * @brief Set network concurrent state
 *
 * @param[in] nw_stat Concurrent state, see CUR_NET_STATUS_T
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
VOID set_wired_wifi_current_net(IN CUR_NET_STATUS_T current_net);
#endif
#else
/**
 * @brief Get device network linkage state
 *
 * @param[out] nw_stat Linkage state, see GW_BASE_NW_STAT_T
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET get_base_gw_nw_status(OUT GW_BASE_NW_STAT_T *nw_stat);
#endif

/**
 * @brief Get device network linkage state
 *
 * @param[out] nw_stat Linkage state, see GW_NW_STAT_T
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
GW_NW_STAT_T get_gw_nw_status(VOID);

/**
 * @brief Get device network state
 *
 * @param[out] nw_stat Network state, see GW_WIFI_NW_STAT_E
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET get_wf_gw_nw_status(OUT GW_WIFI_NW_STAT_E *nw_stat);

/**
 * @brief Connect Wi-Fi with given ssid and password
 *
 * @param[in] ssid SSID to connect
 * @param[in] passwd Password of AP
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET gw_wifi_modify_and_conn(IN CONST CHAR_T *ssid, IN CONST CHAR_T *passwd);

/**
 * @brief Get device IP
 *
 * @param[out] p_ip Device IP, see NW_IP_S
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET gw_get_ip(NW_IP_S *p_ip);

/**
 * @brief Get device MAC
 *
 * @param[out] p_mac Device MAC, see NW_MAC_S
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET gw_get_mac(NW_MAC_S *p_mac);

/**
 * @brief Get device RSSI level
 *
 * @param[out] p_rssi Device RSSI level, if(*p_rssi == 99) then do't support get RSSI
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET gw_get_rssi(SCHAR_T *p_rssi);

#ifdef __cplusplus
}
#endif

#endif // TUYA_DEVOS_NETLINK_H
