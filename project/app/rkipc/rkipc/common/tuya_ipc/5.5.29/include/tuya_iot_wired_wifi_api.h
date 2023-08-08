/**
file tuya_iot_wired_wifi_api.h
Copyright(C),2017, 涂鸦科技 www.tuya.comm
*/

#ifndef _TUYA_IOT_WIRED_WIFI_API_H
#define _TUYA_IOT_WIRED_WIFI_API_H

#ifdef __cplusplus
extern "C" {
#endif
#if defined(GW_SUPPORT_WIRED_WIFI) && (GW_SUPPORT_WIRED_WIFI == 1)

#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_types.h"
#include "tuya_cloud_wifi_defs.h"
#include "tuya_iot_com_api.h"

#ifdef __TUYA_IOT_WIRED_WIFI_API_GLOBALS
#define __TUYA_IOT_WIRED_WIFI_API_EXT
#else
#define __TUYA_IOT_WIRED_WIFI_API_EXT extern
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/

/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/
/***********************************************************
 *  Function: tuya_iot_set_wf_gw_prod_info
 *  Input: wf_prod_info
 *  Output: none
 *  Return: OPERATE_RET
 ***********************************************************/
__TUYA_IOT_WIRED_WIFI_API_EXT
OPERATE_RET tuya_iot_set_wired_wifi_gw_prod_info(IN CONST WF_GW_PROD_INFO_S *wf_prod_info);

/***********************************************************
 *  Function: tuya_iot_set_wf_gw_debug_info  安全版本调试用的产测信息写入
 *  Input: wf_prod_info
 *  Output: none
 *  Return: OPERATE_RET
 ***********************************************************/
__TUYA_IOT_WIRED_WIFI_API_EXT
OPERATE_RET tuya_iot_set_wired_wifi_gw_debug_info(IN CONST DEBUG_GW_PROD_INFO_S *debug_info);

/***********************************************************
 *  Function: tuya_iot_wired_wifi_dev_init
 *  Input: cfg
 *         cbs->tuya wifi sdk user callbacks,note cbs->dev_ug_cb is useless
 *         product_key->product key/proudct id,get from tuya open platform
 *         wf_sw_ver->wifi module software version format:xx.xx.xx (0<=x<=9)
 *  Output: none
 *  Return: OPERATE_RET
 ***********************************************************/
OPERATE_RET tuya_iot_wired_wifi_dev_init(IN CONST IOT_GW_NET_TYPE_T net_mode,
                                         IN CONST GW_WF_CFG_MTHD_SEL cfg,
                                         IN CONST GW_WF_START_MODE start_mode,
                                         IN CONST TY_IOT_CBS_S *cbs, IN CONST CHAR_T *firmware_key,
                                         IN CONST CHAR_T *product_key, IN CONST CHAR_T *sw_ver,
                                         IN CONST GW_ATTACH_ATTR_T *attr, IN CONST UINT_T attr_num);

/***********************************************************
 *  Function: tuya_iot_wf_mcu_dev_init->The device consists of MCU and WiFi
 *  Input: cfg
 *         cbs->tuya wifi sdk user callbacks
 *         product_key->product key/proudct id,get from tuya open platform
 *         wf_sw_ver->wifi module software version format:xx.xx.xx (0<=x<=9)
 *         mcu_sw_ver->mcu software version format:xx.xx.xx (0<=x<=9)
 *  Output: none
 *  Return: OPERATE_RET
 ***********************************************************/
__TUYA_IOT_WIRED_WIFI_API_EXT
OPERATE_RET
tuya_iot_wired_wifi_mcu_dev_init(IN CONST IOT_GW_NET_TYPE_T net_mode,
                                 IN CONST GW_WF_CFG_MTHD_SEL cfg,
                                 IN CONST GW_WF_START_MODE start_mode, IN CONST TY_IOT_CBS_S *cbs,
                                 IN CONST CHAR_T *p_firmware_key, IN CONST CHAR_T *product_key,
                                 IN CONST CHAR_T *wf_sw_ver, IN CONST CHAR_T *mcu_sw_ver);

/***********************************************************
 *  Function: tuya_iot_wf_soc_init->The devcie consists of wifi soc
 *  Input: cfg
 *         cbs->tuya wifi sdk user callbacks,note cbs->dev_ug_cb is useless
 *         product_key->product key/proudct id,get from tuya open platform
 *         wf_sw_ver->wifi module software version format:xx.xx.xx (0<=x<=9)
 *  Output: none
 *  Return: OPERATE_RET
 ***********************************************************/
#define tuya_iot_wired_wifi_soc_dev_init(net_mode, cfg, start_mode, cbs, product_key, wf_sw_ver)   \
	tuya_iot_wired_wifi_soc_dev_init_param(net_mode, cfg, start_mode, cbs, NULL, product_key,      \
	                                       wf_sw_ver)

__TUYA_IOT_WIRED_WIFI_API_EXT
OPERATE_RET tuya_iot_wired_wifi_soc_dev_init_param(
    IN CONST IOT_GW_NET_TYPE_T net_mode, IN CONST GW_WF_CFG_MTHD_SEL cfg,
    IN CONST GW_WF_START_MODE start_mode, IN CONST TY_IOT_CBS_S *cbs, IN CONST CHAR_T *firmware_key,
    IN CONST CHAR_T *product_key, IN CONST CHAR_T *wf_sw_ver);

/***********************************************************
 *  Function: tuya_iot_reg_get_wf_nw_stat_cb
 *  Input: wf_nw_stat_cb
 *  Output: none
 *  Return: OPERATE_RET
 ***********************************************************/
#define tuya_iot_reg_get_wired_wifi_nw_stat_cb(nw_stat_cb, wf_nw_stat_cb)                          \
	tuya_iot_reg_get_wired_wifi_nw_stat_cb_params(nw_stat_cb, wf_nw_stat_cb, 1)
__TUYA_IOT_WIRED_WIFI_API_EXT
OPERATE_RET tuya_iot_reg_get_wired_wifi_nw_stat_cb_params(IN CONST GET_NW_STAT_CB nw_stat_cb,
                                                          IN CONST GET_WF_NW_STAT_CB wf_nw_stat_cb,
                                                          IN CONST INT_T min_interval_s);

/***********************************************************
 *  Function: tuya_iot_wf_gw_unactive
 *  Input: none
 *  Output: none
 *  Return: OPERATE_RET
 ***********************************************************/
__TUYA_IOT_WIRED_WIFI_API_EXT
OPERATE_RET tuya_iot_wired_wifi_gw_unactive(VOID);

/***********************************************************
 *  Function: tuya_iot_wf_gw_reset
 *  Input: none
 *  Output: none
 *  Return: OPERATE_RET
 ***********************************************************/
__TUYA_IOT_WIRED_WIFI_API_EXT
OPERATE_RET tuya_iot_wired_wifi_gw_reset(VOID);
/***********************************************************
 *  Function: tuya_iot_wf_gw_fast_unactive
 *  Input: none
 *  Output: none
 *  Return: OPERATE_RET
 ***********************************************************/
__TUYA_IOT_WIRED_WIFI_API_EXT
OPERATE_RET tuya_iot_wired_wifi_gw_fast_unactive(IN CONST GW_WF_CFG_MTHD_SEL mthd,
                                                 IN CONST GW_WF_START_MODE wifi_start_mode);

/***********************************************************
 *  Function: tuya_iot_wf_gw_fast_unactive
 *  Input: none
 *  Output: none
 *  Return: OPERATE_RET
 ***********************************************************/
OPERATE_RET tuya_iot_wired_wifi_fast_get_nc_type(GW_WF_NWC_FAST_STAT_T *nc_type);

/***********************************************************
 *  Function: tuya_iot_set_user_def_ap_if
 *  Input: ssid passwd
 *  Output: none
 *  Return: OPERATE_RET
 *  Note:need call before tuya_iot_wf_xxx_init
 ***********************************************************/
__TUYA_IOT_WIRED_WIFI_API_EXT
OPERATE_RET tuya_iot_wired_wifi_set_user_def_ap_if(IN CONST CHAR_T *ssid, IN CONST CHAR_T *passwd);

/***********************************************************
 *  Function: tuya_iot_gw_wf_user_cfg
 *  Input: ssid passwd token
 *  Output: none
 *  Return: OPERATE_RET
 *..Note:use to TY_APP_CFG_WF mode
 ***********************************************************/
__TUYA_IOT_WIRED_WIFI_API_EXT
OPERATE_RET tuya_iot_gw_wired_wifi_user_cfg(IN CONST CHAR_T *ssid, IN CONST CHAR_T *passwd,
                                            IN CONST CHAR_T *token);

/***********************************************************
 *  Function: tuya_iot_gw_wired_wifi_set_net_name
 *  Input: wired_net_name or wifi_net_name or both
 *  Output: none
 *  Return: none
 *..Note:use to TY_APP_CFG_WF mode
 ***********************************************************/
__TUYA_IOT_WIRED_WIFI_API_EXT
VOID tuya_iot_gw_wired_wifi_set_net_name(IN CONST CHAR_T *wired_net_name,
                                         IN CONST CHAR_T *wifi_net_name);

#ifdef __cplusplus
}
#endif

#endif /*_TUYA_IOT_API_H*/
#endif
