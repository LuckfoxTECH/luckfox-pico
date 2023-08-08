/**
 * @file tuya_hal_wifi.h
 * @brief Common process - Initialization
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */
#ifndef __TUYA_HAL_WIFI_H__
#define __TUYA_HAL_WIFI_H__

#include "tuya_hal_network.h"
#include "tuya_os_adapter.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief scan current environment and obtain all the ap
 *        infos in current environment
 *
 * @param[out]      ap_ary      current ap info array
 * @param[out]      num         the num of ar_ary
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
OPERATE_RET tuya_hal_wifi_all_ap_scan(AP_IF_S **ap_ary, UINT_T *num);

/**
 * @brief scan current environment and obtain the specific
 *        ap info.
 *
 * @param[in]       ssid        the specific ssid
 * @param[out]      ap          the ap info
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
OPERATE_RET tuya_hal_wifi_assign_ap_scan(CONST SCHAR_T *ssid, AP_IF_S **ap);

/**
 * @brief release the memory malloced in <tuya_hal_wifi_all_ap_scan>
 *        and <tuya_hal_wifi_assign_ap_scan> if needed. tuya-sdk
 *        will call this function when the ap info is no use.
 *
 * @param[in]       ap          the ap info
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
OPERATE_RET tuya_hal_wifi_release_ap(AP_IF_S *ap);

/**
 * @brief set wifi interface work channel
 *
 * @param[in]       chan        the channel to set
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
OPERATE_RET tuya_hal_wifi_set_cur_channel(CONST UCHAR_T chan);

/**
 * @brief get wifi interface work channel
 *
 * @param[out]      chan        the channel wifi works
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
OPERATE_RET tuya_hal_wifi_get_cur_channel(UCHAR_T *chan);

/**
 * @brief enable / disable wifi sniffer mode.
 *        if wifi sniffer mode is enabled, wifi recv from
 *        packages from the air, and user shoud send these
 *        packages to tuya-sdk with callback <cb>.
 *
 * @param[in]       en          enable or disable
 * @param[in]       cb          notify callback
 * @return  OPRT_OK: success  Other: fail
 */
OPERATE_RET tuya_hal_wifi_sniffer_set(CONST BOOL_T en, CONST SNIFFER_CALLBACK cb);

/**
 * @brief get wifi ip info.when wifi works in
 *        ap+station mode, wifi has two ips.
 *
 * @param[in]       wf          wifi function type
 * @param[out]      ip          the ip addr info
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
OPERATE_RET tuya_hal_wifi_get_ip(CONST WF_IF_E wf, NW_IP_S *ip);

/**
 * @brief set wifi mac info.when wifi works in
 *        ap+station mode, wifi has two macs.
 *
 * @param[in]       wf          wifi function type
 * @param[in]       mac         the mac info
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
OPERATE_RET tuya_hal_wifi_set_mac(CONST WF_IF_E wf, CONST NW_MAC_S *mac);

/**
 * @brief get wifi mac info.when wifi works in
 *        ap+station mode, wifi has two macs.
 *
 * @param[in]       wf          wifi function type
 * @param[out]      mac         the mac info
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
OPERATE_RET tuya_hal_wifi_get_mac(CONST WF_IF_E wf, NW_MAC_S *mac);

/**
 * @brief set wifi work mode
 *
 * @param[in]       mode        wifi work mode
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
OPERATE_RET tuya_hal_wifi_set_work_mode(CONST WF_WK_MD_E mode);

/**
 * @brief get wifi work mode
 *
 * @param[out]      mode        wifi work mode
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
OPERATE_RET tuya_hal_wifi_get_work_mode(WF_WK_MD_E *mode);

/**
 * @brief start a soft ap
 *
 * @param[in]       cfg         the soft ap config
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
OPERATE_RET tuya_hal_wifi_ap_start(CONST WF_AP_CFG_IF_S *cfg);

/**
 * @brief stop a soft ap
 *
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
OPERATE_RET tuya_hal_wifi_ap_stop(VOID_T);

/**
 * @brief : get ap info for fast connect
 * @param[out]      fast_ap_info
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
OPERATE_RET tuya_hal_wifi_get_connected_ap_info_v2(FAST_WF_CONNECTED_AP_INFO_V2_S **fast_ap_info);

/**
 * @brief : fast connect
 * @param[in]      fast_ap_info
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
OPERATE_RET tuya_hal_fast_station_connect_v2(CONST FAST_WF_CONNECTED_AP_INFO_V2_S *fast_ap_info);

/**
 * @brief connect wifi with ssid and passwd
 *
 * @param[in]       ssid
 * @param[in]       passwd
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
OPERATE_RET tuya_hal_wifi_station_connect(CONST SCHAR_T *ssid, CONST SCHAR_T *passwd);

/**
 * @brief disconnect wifi from connect ap
 *
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
OPERATE_RET tuya_hal_wifi_station_disconnect(VOID_T);

/**
 * @brief get wifi connect rssi
 *
 * @param[out]      rssi        the return rssi
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
OPERATE_RET tuya_hal_wifi_station_get_conn_ap_rssi(SCHAR_T *rssi);

/**
 * @brief get wifi bssid
 *
 * @param[out]      mac         uplink mac
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
OPERATE_RET tuya_hal_wifi_get_bssid(UCHAR_T *mac);

/**
 * @brief get wifi station work status
 *
 * @param[out]      stat        the wifi station work status
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
OPERATE_RET tuya_hal_wifi_station_get_status(WF_STATION_STAT_E *stat);

/**
 * @brief set wifi country code
 *
 * @param[in]       ccode  country code
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
OPERATE_RET tuya_hal_wifi_set_country_code(CONST COUNTRY_CODE_E ccode);

/**
 * @brief send wifi management
 *
 * @param[in]       buf         pointer to buffer
 * @param[in]       len         length of buffer
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
OPERATE_RET tuya_hal_wifi_send_mgnt(CONST UCHAR_T *buf, CONST UINT_T len);

/**
 * @brief register receive wifi management callback
 *
 * @param[in]       enable
 * @param[in]       recv_cb     receive callback
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
OPERATE_RET tuya_hal_wifi_register_recv_mgnt_callback(CONST bool enable,
                                                      CONST WIFI_REV_MGNT_CB recv_cb);

/***********************************************************
 *  Function: tuya_hal_set_wifi_lp_mode
 *  Input: en dtim
 *  Output: none
 *  Return: 0-succ,other-fail
 ***********************************************************/
OPERATE_RET tuya_hal_set_wifi_lp_mode(CONST BOOL_T en, CONST UINT_T dtim);

/***********************************************************
 *  Function: tuya_hal_wifi_rf_calibrated
 *  Input: none
 *  Output: none
 *  Return:BOOL
 ***********************************************************/
BOOL_T tuya_hal_wifi_rf_calibrated(VOID_T);

/***********************************************************
 *  Function: tuya_hal_wifi_send_beacon
 *
 * @param[in]       channel         beacon's channel
 * @param[in]       ssid            beacon's ssid
 * @param[in]       ssid_len        ssid length
 *
 *  Output: none
 *  Return:BOOL
 ***********************************************************/

OPERATE_RET tuya_hal_wifi_send_beacon(uint8_t channel, uint8_t *ssid, uint8_t ssid_len);

/**
 * @brief Wifi interface initialization
 *
 * @param VOID
 *
 * @note This API is used for initialization of wifi interface.
 *
 * @return VOID
 */
VOID_T tuya_os_wifi_intf_init(VOID_T);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
