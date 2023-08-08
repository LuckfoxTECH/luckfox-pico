/**
 * @file tuya_devos_utils.h
 * @brief Utilities of DevOS
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 */

#ifndef TUYA_DEVOS_UTILS_H
#define TUYA_DEVOS_UTILS_H

#include "gw_intf.h"
#include "tuya_cloud_com_defs.h"
#include "ty_cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Load summer timezone talbe
 *
 * @param[in] tm_zone Summer timezone table
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET gw_load_sum_time_zone_tbl(IN CONST CHAR_T *tm_zone);

/**
 * @brief Get DP flow control rules
 *
 * @return See DP_REPT_FLOW_CTRL_UNIT, NULL on error
 */
DP_REPT_FLOW_CTRL_UNIT *get_all_dp_flow_ctrl_rule(VOID);

/**
 * @brief Parse DP flow control rules
 *
 * @param[in] result Json encoded flow control rules
 *
 * @return Buffer of flow control rules, NULL on error
 */
CHAR_T *gw_dp_flow_ctrl_cfg_proc(ty_cJSON *result);

/**
 * @brief Get serial number
 *
 * @return Buffer of serial number, NULL on error
 */
CHAR_T *tuya_get_serialno(VOID);

/**
 * @brief Set CPU lowpower mode
 *
 * @param[in] lp_enable CPU lowpower is enable or not
 */
VOID tuya_cpu_set_lp_mode(BOOL_T lp_enable);

/**
 * @brief Get CPU lowpower mode
 *
 * @return TRUE on lowpower, FALSE on normal
 */
BOOL_T tuya_cpu_get_lp_mode(VOID);

#if defined(TY_RTC) && (TY_RTC == 1)
/**
 * @brief Initialize rtc module
 *
 * @param[in] timeval Time interval to update local time
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ty_init_rtc(INT_T timeval);
#endif

/**
 * @brief Set DevOS extension state
 *
 * @param[in] ext_stat Extension state, see GW_EXT_STAT_T
 */
VOID set_gw_ext_stat(IN CONST GW_EXT_STAT_T ext_stat);

/**
 * @brief Get DevOS extension state
 *
 * @return Extension state, see GW_EXT_STAT_T
 */
GW_EXT_STAT_T get_gw_ext_stat(VOID);

/**
 * @brief Set DevOS event handlers
 *
 * @param[in] cbs Handler of event, see TY_IOT_CBS_S
 */
VOID gw_register_cbs(IN CONST TY_IOT_CBS_S *cbs);

#if defined(WIFI_GW) && (WIFI_GW == 1)
/**
 * @brief Get Wi-Fi country code
 *
 * @return Country code, see TUYA_COUNTRY_CODE_E
 */
TUYA_COUNTRY_CODE_E tuya_wifi_get_country_code(VOID_T);
#endif

#ifdef __cplusplus
}
#endif

#endif // TUYA_DEVOS_UTILS_H
