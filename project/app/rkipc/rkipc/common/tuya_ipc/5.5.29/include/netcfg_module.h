/**
 * @file netcfg_module.h
 * @brief tuya netcfg module
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __NETCFG_MODULE__
#define __NETCFG_MODULE__

#include "tuya_hal_wifi.h"
#include "uni_pointer.h"
#ifdef __cplusplus
extern "C" {
#endif

/*
    配网类型的宏定义由32bits整型表示:
    | high 16bits 	| low 16bits 	|
    |	vendor type	|	cfg type	|

    1)固定前缀netcfg
    2)协议提供方：ty,jd,
    3)链路方式wifi,ble,camera等，品牌
    4)特性后缀，如smartcfg,ap,airkiss

    NETCFG_TY_WIFI_SMARTCFG
    NETCFG_TY_WIFI_AP
    NETCFG_JD_WIFI_SMART
    NETCFG_WX_WIFI_AIRKISS
    NETCFG_TY_BLE
    NETCFG_TY_CAMERA_QR
*/

/*
    配网提供方：
    tuya, jd, weixin

*/
#define VENDOR_TUYA (0x1 << 0)
#define VENDOR_JD (0x1 << 1)
#define VENDOR_WEIXIN (0x1 << 2)
#define VENDOR_AMAZON (0x1 << 3)

#define WIFI_SMARTCFG (0x1 << 0)
#define WIFI_AP (0x1 << 1)
#define BLE (0x1 << 2)
#define CAMERA_QR (0x1 << 3)
#define WIFI_SMARTCFG_COEXIST (0x1 << 4)
#define WIFI_AP_COEXIST (0x1 << 5)
#define WIFI_FFS (0x1 << 6)
#define WIFI_PEGASUS (0x1 << 7) //无感配网
#define API_USER (0x1 << 8)     //用户通过api配网

/*netcfg type definitions*/
// ez
#define NETCFG_TY_WIFI_SMARTCFG ((VENDOR_TUYA << 16) | WIFI_SMARTCFG)
// ez in WF_START_SMART_AP_CONCURRENT
#define NETCFG_TY_WIFI_SMARTCFG_COEXIST ((VENDOR_TUYA << 16) | WIFI_SMARTCFG_COEXIST)
// ap
#define NETCFG_TY_WIFI_AP ((VENDOR_TUYA << 16) | WIFI_AP)
// ap in WF_START_SMART_AP_CONCURRENT
#define NETCFG_TY_WIFI_AP_COEXIST ((VENDOR_TUYA << 16) | WIFI_AP_COEXIST)
// ble
#define NETCFG_TY_BLE ((VENDOR_TUYA << 16) | BLE)
// qr
#define NETCFG_TY_CAMERA_QR ((VENDOR_TUYA << 16) | CAMERA_QR)
// user or uart
#define NETCFG_TY_API_USER ((VENDOR_TUYA << 16) | API_USER)
#define NETCFG_JD_WIFI_SMART ((VENDOR_JD << 16) | WIFI_SMARTCFG)
// wechat airkiss
#define NETCFG_WX_WIFI_AIRKISS ((VENDOR_WEIXIN << 16) | WIFI_SMARTCFG)
// amazon ffs netcfg
#define NETCFG_AMAZON_WIFI_FFS ((VENDOR_AMAZON << 16) | (WIFI_FFS))
// tuya pegasus netcfg
#define NETCFG_TY_WIFI_PEGASUS ((VENDOR_TUYA << 16) | WIFI_PEGASUS)

#define NETCFG_STOP_ALL_CFG_MODULE (0)

#define DEF_AP_SSID_PF "SmartLife"

typedef struct {
	int enable_station_ap;
	WF_AP_CFG_IF_S *pDevApCfg;
} AP_NETCFG_ARGS, *P_AP_NETCFG_ARGS;

#define WL_TOKEN_LEN (16)

/**
 * @brief ssid password and token info
 */

typedef struct {
	/** ssid array */
	uint8_t ssid[WIFI_SSID_LEN + 1];
	/** length of ssid array */
	uint8_t s_len;
	/** password array */
	uint8_t passwd[WIFI_PASSWD_LEN + 1];
	/** length of password array */
	uint8_t p_len;
	/** token array */
	uint8_t token[WL_TOKEN_LEN + 1];
	/** length of token array */
	uint8_t t_len;
} SSID_PASSWORD_TOKEN, *PTR_SSID_PASSWORD_TOKEN;

/**
 * @brief ssid and password info
 */
typedef struct apSsidPasswd_s {
	/** ssid */
	uint8_t *pSsid;
	/** password */
	uint8_t *pPasswd;
} ApSsidPasswd_t, *ptrApSsidPasswd;

/**
 * @brief netcfg finished callback function.
 *
 * @param[in] PTR_SSID_PASSWORD_TOKEN netcfg info: ssid,passwd,token
 * @param[in] type refer to netcfg type definitions
 *
 * @note netcfg finished callback function.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */

typedef int (*FN_NETCFG_CB)(PTR_SSID_PASSWORD_TOKEN info, int type);

/**
 * @brief netcfg start function.
 *
 * @param[in] cb netcfg finished callback function
 * @param[in] args args for start
 * @param[in] type refer to netcfg type definitions
 *
 * @note netcfg start function.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */

typedef int (*FN_NETCFG_START)(FN_NETCFG_CB cb, void *args, int type);

/**
 * @brief netcfg stop function.
 *
 * @param[in] type refer to netcfg type definitions
 *
 * @note netcfg stop function.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */

typedef int (*FN_NETCFG_STOP)(int type);

/**
 * @brief netcfg handler
 */

typedef struct {
	/** node for list */
	LIST_HEAD node;
	/** netcfg type */
	int type;
	/** netcfg start function */
	FN_NETCFG_START start;
	/** netcfg stop function */
	FN_NETCFG_STOP stop;
	/** netcfg finished function */
	FN_NETCFG_CB netcfg_finish_cb;
	/** args for netcfg_finish_cb */
	void *args;
	/** netcfg is started or not */
	bool isStarted;
} NETCFG_HANDLER, *PTR_NETCFG_HANDLER;

/**
 * @brief init netcfg module.
 *
 * @param[in] netcfg_policy netcfg methods,such as WF_START_AP_ONLY, WF_START_SMART_ONLY,
 * WF_START_AP_FIRST,WF_START_SMART_FIRST,WF_START_SMART_AP_CONCURRENT
 * @param[in] pApInfo ap info to be used in ap netcfg
 * @param[in] isApCfg ap netcfg or not, used in WF_START_AP_FIRST
 *
 * @note init netcfg module.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */

int netcfg_module_init(int netcfg_policy, ApSsidPasswd_t pApInfo, bool isApCfg);

/**
 * @brief uninit netcfg module.
 *
 *
 * @note release netcfg module resources.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */

int netcfg_module_uninit();

/**
 * @brief register netcfg type
 *
 * @param[in] type netcfg type,refer to netcfg type definitions
 * @param[in] start callback function to start netcfg
 * @param[in] stop callback function to stop netcfg
 *
 * @note register netcfg module start and stop callback.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */

int netcfg_module_register(int type, FN_NETCFG_START start, FN_NETCFG_STOP stop);

/**
 * @brief unregister netcfg type
 *
 * @param[in] type netcfg type,refer to netcfg type definitions
 *
 * @note register netcfg module start and stop callback.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */

int netcfg_module_unregister(int type);

/**
 * @brief start netcfg type
 *
 * @param[in] type netcfg type,refer to netcfg type definitions
 * @param[in] netcfg_finish_cb callback function when netcfg finished
 * @param[in] args private data for netcfg_finish_cb
 *
 * @note start netcfg type.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int netcfg_start(int type, FN_NETCFG_CB netcfg_finish_cb, void *args);

/**
 * @brief stop netcfg type
 *
 * @param[in] type netcfg type,refer to netcfg type definitions 0 : stop all netcfg type
 *
 * @note stop netcfg type.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */

int netcfg_stop(int type);

/**
 * @brief stop all netcfg type except the specified type
 *
 * @param[in] type netcfg type,refer to netcfg type definitions 0 : stop all netcfg type
 *
 * @note stop all netcfg type except the specified type.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */

int netcfg_stop_other_all(int type);

/*
    fun_desc:
    开启除了type之外所有配网
*/

/**
 * @brief start all netcfg type except the specified type
 *
 * @param[in] type netcfg type,refer to netcfg type definitions 0 : stop all netcfg type
 *
 * @note start all netcfg type except the specified type.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */

int netcfg_start_other_all(int type);

/**
 * @brief set channel switch interval(default:110ms)
 *
 * @param[in] ms_interval unit:ms
 *
 * @note set channel switch interval(default:110ms)
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */

int netcfg_set_transporter_switch_interval(int ms_interval);

#ifdef __cplusplus
}
#endif

#endif
