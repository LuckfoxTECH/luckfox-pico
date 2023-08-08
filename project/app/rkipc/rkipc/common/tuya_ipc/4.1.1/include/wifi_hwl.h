/*
wifi_hwl.h
Copyright(C),2018-2020, 涂鸦科技 www.tuya.comm
*/

#ifndef _WIFI_HWL_H
#define _WIFI_HWL_H

#include "tuya_cloud_types.h"
#include "uni_network.h"

#ifdef __cplusplus
extern "C" {
#endif

/* tuya sdk definition of wifi ap info */
#define WIFI_SSID_LEN 32   // tuya sdk definition WIFI SSID MAX LEN
#define WIFI_PASSWD_LEN 64 // tuya sdk definition WIFI PASSWD MAX LEN
typedef struct {
	BYTE_T channel;                 // AP channel
	SCHAR_T rssi;                   // AP rssi
	BYTE_T bssid[6];                // AP bssid
	BYTE_T ssid[WIFI_SSID_LEN + 1]; // AP ssid array
	BYTE_T s_len;                   // AP ssid len
	BYTE_T has_key;                 // AP has password
} AP_IF_S;

/***********************************************************
 *  callback function: SNIFFER_CALLBACK
 *  Desc:    when wifi sniffers package from air, notify tuya-sdk
 *           with this callback. the package should include
 *           802.11 protocol header.
 *  Input:   buf: the buf wifi recv
 *  Input:   len: the len of buf
 ***********************************************************/
typedef VOID (*SNIFFER_CALLBACK)(IN CONST BYTE_T *buf, IN CONST USHORT_T len);

/* tuya sdk definition of wifi function type */
typedef enum {
	WF_STATION = 0, // station type
	WF_AP,          // ap type
} WF_IF_E;

/* tuya sdk definition of wifi work mode */
typedef enum {
	WWM_LOWPOWER = 0, // wifi work in lowpower mode
	WWM_SNIFFER,      // wifi work in sniffer mode
	WWM_STATION,      // wifi work in station mode
	WWM_SOFTAP,       // wifi work in ap mode
	WWM_STATIONAP,    // wifi work in station+ap mode
} WF_WK_MD_E;

/* tuya sdk definition of wifi encryption type */
typedef enum {
	WAAM_OPEN = 0,     // open
	WAAM_WEP,          // WEP
	WAAM_WPA_PSK,      // WPA—PSK
	WAAM_WPA2_PSK,     // WPA2—PSK
	WAAM_WPA_WPA2_PSK, // WPA/WPA2
} WF_AP_AUTH_MODE_E;

/* tuya sdk definition of ap config info */
typedef struct {
	BYTE_T ssid[WIFI_SSID_LEN + 1];     // ssid
	BYTE_T s_len;                       // len of ssid
	BYTE_T passwd[WIFI_PASSWD_LEN + 1]; // passwd
	BYTE_T p_len;                       // len of passwd
	BYTE_T chan;                        // channel. default:0
	WF_AP_AUTH_MODE_E md;               // encryption type
	BYTE_T ssid_hidden;                 // ssid hidden  default:0
	BYTE_T max_conn;                    // max sta connect nums default:0
	USHORT_T ms_interval;               // broadcast interval default:0
} WF_AP_CFG_IF_S;

/* tuya sdk definition of wifi station work status */
typedef enum {
	WSS_IDLE = 0,     // not connected
	WSS_CONNECTING,   // connecting wifi
	WSS_PASSWD_WRONG, // passwd not match
	WSS_NO_AP_FOUND,  // ap is not found
	WSS_CONN_FAIL,    // connect fail
	WSS_CONN_SUCCESS, // connect wifi success
	WSS_GOT_IP,       // get ip success
} WF_STATION_STAT_E;

typedef enum {
	WIRELESS_MODE = 0, // Wifi
	WIRED_MODE,        // Ethernet
} WF_NETWORK_MODE_E;
/***********************************************************
 *  Function: hwl_wf_all_ap_scan
 *  Desc:     scan current environment and obtain all the ap
 *            infos in current environment
 *  Output:   ap_ary: current ap info array
 *  Output:   num   : the num of ar_ary
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_all_ap_scan(OUT AP_IF_S **ap_ary, OUT UINT_T *num);

/***********************************************************
 *  Function: hwl_wf_assign_ap_scan
 *  Desc:     scan current environment and obtain the specific
 *            ap info.
 *  Input:    ssid: the specific ssid
 *  Output:   ap  : the ap info
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_assign_ap_scan(IN CONST CHAR_T *ssid, OUT AP_IF_S **ap);

/***********************************************************
 *  Function: hwl_wf_release_ap
 *  Desc:     release the memory malloced in <hwl_wf_all_ap_scan>
 *            and <hwl_wf_assign_ap_scan> if needed. tuya-sdk
 *            will call this function when the ap info is no use.
 *  Input:    ap: the ap info
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_release_ap(IN AP_IF_S *ap);

/***********************************************************
 *  Function: hwl_wf_set_cur_channel
 *  Desc:     set wifi interface work channel
 *  Input:    chan: the channel to set
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_set_cur_channel(IN CONST BYTE_T chan);

/***********************************************************
 *  Function: hwl_wf_get_cur_channel
 *  Desc:     get wifi interface work channel
 *  Input:    chan: the channel wifi works
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_get_cur_channel(OUT BYTE_T *chan);

/***********************************************************
 *  Function: hwl_wf_sniffer_set
 *  Desc:     enable / disable wifi sniffer mode.
 *            if wifi sniffer mode is enabled, wifi recv from
 *            packages from the air, and user shoud send these
 *            packages to tuya-sdk with callback <cb>.
 *  Input:    en: enable or disable
 *  Input:    cb: notify callback
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_sniffer_set(IN CONST BOOL_T en, IN CONST SNIFFER_CALLBACK cb);

/***********************************************************
 *  Function: hwl_wf_get_ip
 *  Desc:     get wifi ip info.when wifi works in
 *            ap+station mode, wifi has two ips.
 *  Input:    wf: wifi function type
 *  Output:   ip: the ip addr info
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_get_ip(IN CONST WF_IF_E wf, OUT NW_IP_S *ip);

/***********************************************************
 *  Function: hwl_wf_set_ip
 *  Desc:     set wifi ip info.when wifi works in
 *            ap+station mode, wifi has two ips.
 *  Input:    wf: wifi function type
 *  Input:    ip: the ip addr info
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_set_ip(IN CONST WF_IF_E wf, IN CONST NW_IP_S *ip);

/***********************************************************
 *  Function: hwl_wf_get_mac
 *  Desc:     get wifi mac info.when wifi works in
 *            ap+station mode, wifi has two macs.
 *  Input:    wf: wifi function type
 *  Output:   mac: the mac info
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_get_mac(IN CONST WF_IF_E wf, OUT NW_MAC_S *mac);

/***********************************************************
 *  Function: hwl_wf_set_mac
 *  Desc:     set wifi mac info.when wifi works in
 *            ap+station mode, wifi has two macs.
 *  Input:    wf: wifi function type
 *  Input:    mac: the mac info
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_set_mac(IN CONST WF_IF_E wf, IN CONST NW_MAC_S *mac);

/***********************************************************
 *  Function: hwl_wf_wk_mode_set
 *  Desc:     set wifi work mode
 *  Input:    mode: wifi work mode
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_wk_mode_set(IN CONST WF_WK_MD_E mode);

/***********************************************************
 *  Function: hwl_wf_wk_mode_get
 *  Desc:     get wifi work mode
 *  Output:   mode: wifi work mode
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_wk_mode_get(OUT WF_WK_MD_E *mode);

/***********************************************************
 *  Function: hwl_wf_station_connect
 *  Desc:     connect wifi with ssid and passwd.
 *  Input:    ssid && passwd
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_station_connect(IN CONST CHAR_T *ssid, IN CONST CHAR_T *passwd);

/***********************************************************
 *  Function: hwl_wf_station_disconnect
 *  Desc:     disconnect wifi from connect ap
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_station_disconnect(VOID);

/***********************************************************
 *  Function: hwl_wf_station_get_conn_ap_rssi
 *  Desc:     get wifi connect rssi
 *  Output:   rssi: the return rssi.
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_station_get_conn_ap_rssi(OUT SCHAR_T *rssi);

/***********************************************************
 *  Function: hwl_wf_station_stat_get
 *  Desc:     get wifi station work status
 *  Output:   stat: the wifi station work status
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_station_stat_get(OUT WF_STATION_STAT_E *stat);

/***********************************************************
 *  Function: hwl_wf_ap_start
 *  Desc:     start a soft ap
 *  Input:    cfg: the soft ap config
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_ap_start(IN CONST WF_AP_CFG_IF_S *cfg);

/***********************************************************
 *  Function: hwl_wf_ap_stop
 *  Desc:     stop a soft ap
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_ap_stop(VOID);

OPERATE_RET hwl_wf_set_country_code(IN CONST CHAR_T *p_country_code);

/***********************************************************
 *  Function: hwl_wf_network_mode
 *  Desc:
 *  Output:  mode 0:wifi     1:ethernet
 *  Return:   OPRT_OK: success   Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_network_mode(OUT WF_NETWORK_MODE_E *mode);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
