/***********************************************************
 *  File: base_nw_intf.h
 *  Author: nzy
 *  Date: 20170220
 ***********************************************************/
#ifndef _BASE_NW_INTF_H
#define _BASE_NW_INTF_H
#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_types.h"
#include "tuya_hal_network.h"
#include "tuya_hal_wired.h"

/**
 * @brief bnw_get_ip
 * @desc get the ip address of the wired network interface
 *
 * @param[out] ip: the ip address
 *
 * @return OPRT_OK: success  Other: fail
 *
 * @note this api can used when in wired or wired&wifi mode
 */
OPERATE_RET bnw_get_ip(OUT NW_IP_S *ip);

/**
 * @brief bnw_set_station_connect
 * @desc connect to a special ap
 *
 * @param[in] ssid: the ap ssid
 * @param[in] passwd: the ap password
 *
 * @return OPRT_OK: success  Other: fail
 *
 * @note this api only can used when in wired&wifi mode
 */
OPERATE_RET bnw_set_station_connect(IN const char *ssid, IN const char *passwd);

/**
 * @brief bnw_station_get_conn_ap_rssi
 * @desc get the connected ap rssi infomation
 *
 * @param[OUT] rssi: the ap rssi, rssi=99 if in wired mode, rssi=actually rssi if in wired&wifi mode
 *
 * @return OPRT_OK: success  Other: fail
 *
 * @note this api can used when in wired or wired&wifi mode
 */
OPERATE_RET bnw_station_get_conn_ap_rssi(OUT int8_t *rssi);

/**
 * @brief bnw_get_mac
 * @desc get the mac address of the wired network interface
 *
 * @param[OUT] mac: the mac address
 *
 * @return OPRT_OK: success  Other: fail
 *
 * @note this api can used when in wired or wired&wifi mode
 */
OPERATE_RET bnw_get_mac(OUT NW_MAC_S *mac);

/**
 * @brief bnw_set_mac
 * @desc set the mac address of the wired network interface
 *
 * @param[in] mac: the mac address
 *
 * @return OPRT_OK: success  Other: fail
 *
 * @note this api can used when in wired or wired&wifi mode
 */
OPERATE_RET bnw_set_mac(IN const NW_MAC_S *mac);

/**
 * @brief bnw_if_wired_connect_net
 * @desc get the status of the wired network interface
 *
 * @param[out] status: the connection status, TRUE means connected, FALSE means not connected
 *
 * @return OPRT_OK: success  Other: fail
 *
 * @note this api can used when in wired or wired&wifi mode
 */
OPERATE_RET bnw_if_wired_connect_net(BOOL_T *status);

/**
 * @brief bnw_station_conn
 * @desc get the status of the wifi network interface
 *
 * @return TRUE： connected, FALSE： not connected
 *
 * @note this api can used when in wired or wired&wifi mode
 */
BOOL_T bnw_station_conn(void);

/**
 * @brief bnw_need_wifi_cfg
 * @desc check if need config wifi
 *
 * @return TRUE： connected, FALSE： not connected
 *
 * @note this api can used when in wired or wired&wifi mode,
 * it will used to add a wf_cfg infomation in lan discocery packet,
 * and support set wifi ssid and passwd from lan.
 */
BOOL_T bnw_need_wifi_cfg(void);

#ifdef __cplusplus
}
#endif
#endif
