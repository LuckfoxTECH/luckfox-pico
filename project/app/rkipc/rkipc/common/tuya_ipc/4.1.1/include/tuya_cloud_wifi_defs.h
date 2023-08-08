/*
tuya_cloud_wifi_defs.h
Copyright(C),2018-2020, 涂鸦科技 www.tuya.comm
*/

#ifndef TUYA_CLOUD_WIFI_DEFS_H
#define TUYA_CLOUD_WIFI_DEFS_H

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* tuya sdk definition of wifi work mode */
typedef BYTE_T GW_WF_CFG_MTHD_SEL; // wifi config method select
#define GWCM_OLD 0                 // do not have low power mode
#define GWCM_LOW_POWER 1           // with low power mode
#define GWCM_SPCL_MODE 2           // special with low power mode
#define GWCM_OLD_PROD 3            // GWCM_OLD mode with product

/* tuya sdk definition of wifi start mode */
typedef BYTE_T GW_WF_START_MODE;
#define WF_START_AP_ONLY 0     // only have ap-cfg mode
#define WF_START_SMART_ONLY 1  // only have smart-cfg mode
#define WF_START_AP_FIRST 2    // have both ap-cfg and smart-cfg. default is ap-cfg mode
#define WF_START_SMART_FIRST 3 // have both ap-cfg and smart-cfg. default is smart-cfg mode

/* tuya sdk definition of wifi-config status */
typedef BYTE_T GW_WF_NWC_STAT_T;
#define GWNS_LOWPOWER 0    // current in low power mode
#define GWNS_UNCFG_SMC 1   // current in smart-cfg mode
#define GWNS_UNCFG_AP 2    // current in ap-cfg mode
#define GWNS_TY_SMARTCFG 3 // already get ssid && passwd from smart-cfg mode
#define GWNS_TY_AP 4       // already get ssid && passwd from ap-cfg mode
#define GWNS_WECHAT_AK 5   // already get ssid && passwd from wechat mode
#define GWNS_OTHER_CFG                                                                             \
	6 // already get ssid && passwd from other mode.for example:uart、bluetooth、qrcode and so on
#define GWNS_NO_NEED_CFG 7 // no need config wifi
/* tuya sdk definition of wifi-net status */
typedef BYTE_T GW_WIFI_NW_STAT_E;
#define STAT_LOW_POWER 0     // idle status,use to external config network
#define STAT_UNPROVISION 1   // smart config status
#define STAT_AP_STA_UNCFG 2  // ap WIFI config status
#define STAT_AP_STA_DISC 3   // ap WIFI already config,station disconnect
#define STAT_AP_STA_CONN 4   // ap station mode,station connect
#define STAT_STA_DISC 5      // only station mode,disconnect
#define STAT_STA_CONN 6      // station mode connect
#define STAT_CLOUD_CONN 7    // cloud connect
#define STAT_AP_CLOUD_CONN 8 // cloud connect and ap start
#define STAT_REG_FAIL 9      // register fail
#define STAT_OFFLINE 10      // offline
#define STAT_MQTT_ONLINE 11
#define STAT_MQTT_OFFLINE 12
#define STAT_REG_FAIL_WITH_BINDED 13 // dev has binded

/* tuya sdk definition of wifi-reset type */
typedef BYTE_T WF_RESET_TP_T;
#define WRT_SMT_CFG 0
#define WRT_AP 1
#define WRT_AUTO 2

/* tuya-sdk product info (wifi version) */
typedef struct {
	CHAR_T *uuid;      // strlen(uuid) <= 16,must not be null
	CHAR_T *auth_key;  // strlen(auth_key) <= 32,must not be null
	CHAR_T *ap_ssid;   // strlen(ap_ssid) <= 16,if ap_ssid is null ,then the default ssid is
	                   // Smartlife_xxxx
	CHAR_T *ap_passwd; // strlen(ap_passwd) <= 16,default null
} WF_GW_PROD_INFO_S;

typedef struct {
	BOOL_T has_auth;   // authFlag
	CHAR_T *ap_ssid;   // strlen(ap_ssid) <= 16,if ap_ssid is null ,then the default ssid is
	                   // Smartlife_xxxx
	CHAR_T *ap_passwd; // strlen(ap_passwd) <= 16,default null
} DEBUG_GW_PROD_INFO_S;

/***********************************************************
 *  callback function: GET_WF_NW_STAT_CB
 *  Desc:    tuya-sdk network check callback (wifi version)
 *  Input:   stat: curr network status
 ***********************************************************/
typedef VOID (*GET_WF_NW_STAT_CB)(IN CONST GW_WIFI_NW_STAT_E stat);

#ifdef __cplusplus
}
#endif

#endif // TUYA_CLOUD_WIFI_DEFS_H
