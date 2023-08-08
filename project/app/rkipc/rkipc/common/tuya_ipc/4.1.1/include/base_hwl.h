/*
base_hwl.h
Copyright(C),2018-2020, 涂鸦科技 www.tuya.comm
*/

#ifndef _BASE_HWL_H
#define _BASE_HWL_H

#include "tuya_cloud_types.h"
#include "uni_network.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
 *  Function: hwl_bnw_get_ip
 *  Desc:     get wired ethernet ip info
 *  Output:   ip: the ip addr info
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_bnw_get_ip(OUT NW_IP_S *ip);

/***********************************************************
 *  Function: hwl_bnw_station_conn
 *  Desc:     return whether the hardware is connect to internet
 *  Return:   TRUE: connected  FALSE: disconnected
 ***********************************************************/
BOOL_T hwl_bnw_station_conn(VOID);

/***********************************************************
 *  Function: hwl_bnw_set_station_connect
 *  Desc:     if the hardware has a wifi interface, and user
 *            want to connect wifi. tuya-app will notify current
 *            wifi ssid and passwd,and tuya-sdk will nofity
 *            user with this function. user should implement
 *            wifi connect process in this function.
 *  Input:    ssid && passwd
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_bnw_set_station_connect(IN CONST CHAR_T *ssid, IN CONST CHAR_T *passwd);

/***********************************************************
 *  Function: hwl_bnw_need_wifi_cfg
 *  Desc:     if the hardware has a wifi interface, and user
 *            want to connect wifi, then return true.
 *            if the hardware does not have a wifi interface,
 *            or user do not want to connect wifi,return false.
 *  Return:   TRUE: need to connect wifi  FALSE:ignore wifi
 ***********************************************************/
BOOL_T hwl_bnw_need_wifi_cfg(VOID);

/***********************************************************
 *  Function: hwl_bnw_station_get_conn_ap_rssi
 *  Desc:     if the hardware has a wifi interface and wifi is
 *            connected,return wifi rssi. or return 99
 *  Output:   rssi: the return rssi.
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_bnw_station_get_conn_ap_rssi(OUT SCHAR_T *rssi);

/***********************************************************
 *  Function: hwl_bnw_get_mac
 *  Desc:     get wired ethernet mac info
 *  Output:   mac: the mac info.
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_bnw_get_mac(OUT NW_MAC_S *mac);

/***********************************************************
 *  Function: hwl_bnw_set_mac
 *  Desc:     set wired ethernet mac info
 *  Input:    mac: the mac info.
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_bnw_set_mac(IN CONST NW_MAC_S *mac);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
