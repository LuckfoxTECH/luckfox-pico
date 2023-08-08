/**
 * @file wf_basic_intf.h
 * @brief basic wifi interface
 * @version 0.1
 * @date 2021-01-28
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef _WF_BASIC_INTF_H
#define _WF_BASIC_INTF_H
#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_types.h"
#include "tuya_hal_network.h"
#include "tuya_hal_wifi.h"

#ifdef __WF_BASIC_INTF_GLOBALS
#define __WF_BASIC_INTF_EXT
#else
#define __WF_BASIC_INTF_EXT extern
#endif

/***********************************************************
*************************macro define***********************
***********************************************************/

/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/

/**
 * @brief do wifi Scan in all channels.
 *
 * @param[out] ap_ary array of ap infomation
 * @param[out] num number of array members
 *
 * @note do all channel scan, must set station mode first.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */

__WF_BASIC_INTF_EXT
OPERATE_RET wf_all_ap_scan(OUT AP_IF_S **ap_ary, OUT UINT_T *num);

/**
 * @brief do wifi Scan with specificed SSID.
 *
 * @param[in] ssid the ssid to be scaned
 * @param[out] ap ap info of the ssid
 *
 * @note must set station mode first.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
__WF_BASIC_INTF_EXT
OPERATE_RET wf_assign_ap_scan(IN CONST CHAR_T *ssid, OUT AP_IF_S **ap);

/**
 * @brief free ap memory.
 *
 * @param[in] ap the ap info to be free
 *
 * @note called after wf_all_ap_scan or wf_assign_ap_scan
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
__WF_BASIC_INTF_EXT
OPERATE_RET wf_release_ap(IN AP_IF_S *ap);

/**
 * @brief set wifi channel.
 *
 * @param[in] chan the chan to be set
 *
 * @note called after set station mode
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */

__WF_BASIC_INTF_EXT
OPERATE_RET wf_set_cur_channel(IN CONST BYTE_T chan);

/**
 * @brief get wifi channel.
 *
 * @param[out] chan pointer that current wifi chan
 *
 * @note called after set station mode
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */

__WF_BASIC_INTF_EXT
OPERATE_RET wf_get_cur_channel(OUT BYTE_T *chan);

/**
 * @brief enable or disable wifi sniffer.
 *
 * @param[in] en enable or disable wifi sniffer
 *
 * @param[in] cb the callback function that will get sniffer data
 *
 * @note called after set sniffer mode
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */

__WF_BASIC_INTF_EXT
OPERATE_RET wf_sniffer_set(IN CONST BOOL_T en, IN CONST SNIFFER_CALLBACK cb);

/**
 * @brief get wifi ip.
 *
 * @param[in] wf station or ap
 *
 * @param[out] ip ip address of station or ap
 *
 * @note called in station mode or ap mode
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
__WF_BASIC_INTF_EXT
OPERATE_RET wf_get_ip(IN CONST WF_IF_E wf, OUT NW_IP_S *ip);

/**
 * @brief get wifi mac address.
 *
 * @param[in] wf station or ap
 *
 * @param[out] mac mac address of station or ap
 *
 * @note called in station mode or ap mode
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
__WF_BASIC_INTF_EXT
OPERATE_RET wf_get_mac(IN CONST WF_IF_E wf, OUT NW_MAC_S *mac);

/**
 * @brief set wifi mac address.
 *
 * @param[in] wf station or ap
 *
 * @param[in] mac mac address of station or ap
 *
 * @note called in station mode or ap mode
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */

__WF_BASIC_INTF_EXT
OPERATE_RET wf_set_mac(IN CONST WF_IF_E wf, IN CONST NW_MAC_S *mac);

/**
 * @brief set wifi work mode.
 *
 * @param[in] mode wifi work mode,such as station, soft ap...
 *
 * @note called before other wifi operation
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
__WF_BASIC_INTF_EXT
OPERATE_RET wf_wk_mode_set(IN CONST WF_WK_MD_E mode);

/**
 * @brief get wifi work mode.
 *
 * @param[out] mode current wifi work mode
 *
 * @note
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
__WF_BASIC_INTF_EXT
OPERATE_RET wf_wk_mode_get(OUT WF_WK_MD_E *mode);

#if defined(ENABLE_AP_FAST_CONNECT) && (ENABLE_AP_FAST_CONNECT == 1)
/**
 * @brief get connect ap info.
 *
 * @param[out] fast_ap_info the ap infomation of the connected ap
 *
 * @note called after wifi connection has established in station mode
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
__WF_BASIC_INTF_EXT
OPERATE_RET wf_get_connected_ap_info(OUT FAST_WF_CONNECTED_AP_INFO_V2_S **fast_ap_info);

/**
 * @brief do fast wifi connect.
 *
 * @param[in] fast_ap_info the ap infomation of the ap to be connected
 *
 * @note called after wf_get_connected_ap_info is called
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
__WF_BASIC_INTF_EXT
OPERATE_RET wf_fast_station_connect(IN FAST_WF_CONNECTED_AP_INFO_V2_S *fast_ap_info);
#endif

/**
 * @brief do wifi connect in station mode.
 *
 * @param[in] ssid the ssid of the ap to be connected
 *
 * @param[in] passwd the passwd of the ap to be connected
 *
 * @note called in station mode
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
__WF_BASIC_INTF_EXT
OPERATE_RET wf_station_connect(IN CONST CHAR_T *ssid, IN CONST CHAR_T *passwd);

/**
 * @brief do wifi disconnect in station mode.
 *
 * @note called in station mode after wf_station_connect
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */

__WF_BASIC_INTF_EXT
OPERATE_RET wf_station_disconnect(VOID);

/**
 * @brief get rssi of the connected ap.
 *
 * @param[out] rssi the rssi of the connected ap
 *
 * @note called after connection has established in station mode
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
__WF_BASIC_INTF_EXT
OPERATE_RET wf_station_get_conn_ap_rssi(OUT SCHAR_T *rssi);

/**
 * @brief get wifi station stat.
 *
 * @param[out] stat the stat in the station mode,refer to WF_STATION_STAT_E
 *
 * @note called after wf_station_connect
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
__WF_BASIC_INTF_EXT
OPERATE_RET wf_station_stat_get(OUT WF_STATION_STAT_E *stat);

/**
 * @brief start ap in ap mode.
 *
 * @param[in] cfg the configuration of the ap
 *
 * @note called in wifi ap mode
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
__WF_BASIC_INTF_EXT
OPERATE_RET wf_ap_start(IN CONST WF_AP_CFG_IF_S *cfg);

/**
 * @brief stop ap in ap mode.
 *
 * @note called in wifi ap mode
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
__WF_BASIC_INTF_EXT
OPERATE_RET wf_ap_stop(VOID);

/**
 * @brief get the ap mac address.
 *
 * @note called in wifi ap mode
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */

__WF_BASIC_INTF_EXT
OPERATE_RET wf_get_ap_mac(INOUT NW_MAC_S *mac);

/**
 * @brief set the wifi country code.
 *
 * @note called in wifi ap mode
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
__WF_BASIC_INTF_EXT
OPERATE_RET wf_set_country_code(IN CONST CHAR_T *p_country_code);

/**
 * @brief get the wifi country code.
 *
 * @note called after wf_set_country_code
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
__WF_BASIC_INTF_EXT
OPERATE_RET wf_get_country_code(IN CONST CHAR_T *p_country_code);

/**
 * @brief set the wifi low power dtim.
 *
 * @note called before enter wifi low power mode
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
__WF_BASIC_INTF_EXT
VOID_T tuya_wifi_set_lps_dtim(const unsigned int dtim);

/**
 * @brief disable the wifi low power mode.
 *
 * @note called when exiting wifi low power mode
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
__WF_BASIC_INTF_EXT
OPERATE_RET tuya_wifi_lp_disable(VOID);

/**
 * @brief enable the wifi low power mode.
 *
 * @note called when entering wifi low power mode
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
__WF_BASIC_INTF_EXT
OPERATE_RET tuya_wifi_lp_enable(VOID);

/**
 * @brief disable the cpu low power mode.
 *
 * @note called when exiting wifi low power mode
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
__WF_BASIC_INTF_EXT
OPERATE_RET tuya_cpu_lp_disable(VOID);

/**
 * @brief enable the cpu low power mode.
 *
 * @note called when entering wifi low power mode
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
__WF_BASIC_INTF_EXT
OPERATE_RET tuya_cpu_lp_enable(VOID);

/**
 * @brief do wifi calibration
 *
 * @note called when test wifi
 *
 * @return true on success. faile on failure
 */

__WF_BASIC_INTF_EXT
BOOL_T wf_rf_calibrated(VOID_T);

#ifdef __cplusplus
}
#endif
#endif
