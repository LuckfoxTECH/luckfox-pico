/**
 * @file tuya_iot_base_api.h
 * @brief
 * @author huatuo
 * @version 1.0.0
 * @date 2021-01-12
 * @Copyright Copyright(C),2018-2020, 涂鸦科技 www.tuya.comm
 */

#ifndef __TUYA_IOT_BASE_API_H
#define __TUYA_IOT_BASE_API_H

#include "tuya_cloud_base_defs.h"
#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_types.h"
#include "tuya_iot_com_api.h"
#include "tuya_os_adapter.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief tuya_iot_set_gw_prod_info
 * @desc set tuya-sdk product info (wired version)
 *
 * @param prod_info: tuya-sdk product info
 *
 * @return OPRT_OK: success  Other: fail
 */
OPERATE_RET tuya_iot_set_gw_prod_info(IN CONST GW_PROD_INFO_S *prod_info);

/**
 * @brief tuya_iot_dev_init
 *
 * @param cbs: tuya sdk user callbacks
 * @param firmware_key
 * @param product_key: product key/proudct id,get from tuya open platform
 * @param sw_ver: netlink module software version format:xx.xx.xx (0<=x<=9)
 * @param attr
 * @param attr_num
 *
 * @return OPERATE_RET
 */
OPERATE_RET tuya_iot_dev_init(IN CONST TY_IOT_CBS_S *cbs, IN CONST CHAR_T *firmware_key,
                              IN CONST CHAR_T *product_key, IN CONST CHAR_T *sw_ver,
                              IN CONST DEV_TYPE_T tp, IN CONST GW_ATTACH_ATTR_T *attr,
                              IN CONST UINT_T attr_num);

/**
 * @brief tuya_iot_mcu_dev_init
 * @desc init tuya-sdk (wired version, [mcu] mode)
 *
 * @param cbs: tuya-sdk user callbacks
 * @param product_key: product key
 * @param p_firmware_key
 * @param sw_ver: netlink module version.format:xx.xx.xx (0<=x<=9)
 * @param mcu_sw_ver: mcu version format:xx.xx.xx (0<=x<=9)
 *
 * @return OPRT_OK: success  Other: fail
 */
OPERATE_RET tuya_iot_mcu_dev_init(IN CONST TY_IOT_CBS_S *cbs, IN CONST CHAR_T *product_key,
                                  IN CONST CHAR_T *p_firmware_key, IN CONST CHAR_T *sw_ver,
                                  IN CONST CHAR_T *mcu_sw_ver);

/**
 * @brief tuya_iot_soc_init_param
 * @desc init tuya-sdk (wired version, [soc] mode)
 *
 * @param cbs: tuya-sdk user callbacks
 * @param product_key: product key
 * @param firmware_key: firmware key
 * @param sw_ver: soc version.format:xx.xx.xx (0<=x<=9)
 *
 * @return OPRT_OK: success  Other: fail
 */
OPERATE_RET tuya_iot_soc_init_param(IN CONST TY_IOT_CBS_S *cbs, IN CONST CHAR_T *product_key,
                                    IN CONST CHAR_T *firmware_key, IN CONST CHAR_T *sw_ver);
#define tuya_iot_soc_init(cbs, product_key, sw_ver)                                                \
	tuya_iot_soc_init_param(cbs, product_key, NULL, sw_ver)

/**
 * @brief tuya_iot_gw_unactive
 * @desc unactive this hardware from tuya-cloud
 *
 * @param VOID
 *
 * @return OPRT_OK: success  Other: fail
 */
OPERATE_RET tuya_iot_gw_unactive(VOID);

/**
 * @brief tuya_iot_gw_reset
 *
 * @return OPRT_OK: success  Other: fail
 */
OPERATE_RET tuya_iot_gw_reset(VOID);

#if (defined(WIFI_GW) && (WIFI_GW == 0)) ||                                                        \
    (defined(GW_SUPPORT_WIRED_WIFI) && (GW_SUPPORT_WIRED_WIFI == 1))

/**
 * @brief tuya_iot_reg_get_nw_stat_cb_params
 * @desc enable tuya-sdk network check. sdk will check the
 *       network every 1 second. if network change, sdk
 *       will notify by calling nw_stat_cb
 *
 * @param nw_stat_cb: network change callback
 * @param min_interval_s
 *
 * @return OPRT_OK: success  Other: fail
 */
OPERATE_RET tuya_iot_reg_get_nw_stat_cb_params(IN CONST GET_NW_STAT_CB nw_stat_cb,
                                               IN CONST INT_T min_interval_s);
#define tuya_iot_reg_get_nw_stat_cb(nw_stat_cb) tuya_iot_reg_get_nw_stat_cb_params(nw_stat_cb, 1);

#endif
#ifdef __cplusplus
}
#endif
#endif
