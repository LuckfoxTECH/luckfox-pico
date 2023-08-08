/**
 * @file tuya_devos_netcfg.h
 * @brief Common process - Network configuration management
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 */

#ifndef TUYA_DEVOS_NETCFG_H
#define TUYA_DEVOS_NETCFG_H

#include "lan_protocol.h"
#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_wifi_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EVT_NETCFG_DONE "netcfg.done"

typedef struct {
	UINT_T resved;
} tuya_netcfg_t;

/**
 * @brief Initialize network configuration module
 *
 * @param[in] param Init param, see tuya_netcfg_t
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_devos_netcfg_init(tuya_netcfg_t *param);

/**
 * @brief Start network config
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_devos_netcfg_start(VOID);

/**
 * @brief Stop network config
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_devos_netcfg_stop(VOID);

/**
 * @brief Validate network config status
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_devos_netcfg_validate(VOID);

/**
 * @brief Set Wi-Fi configuration timeout
 *
 * @param[in] timeout_s Timeout(unit s), default 15 min
 */
VOID set_wf_cfg_timeout(UINT_T timeout_s);

/**
 * @brief Set information for AP config
 *
 * @param[in] cfg AP config, see WF_AP_CFG_IF_S
 */
VOID set_ap_cfg_info(WF_AP_CFG_IF_S *cfg);

/**
 * @brief Set network start mode
 *
 * @param[in] wf_start_mode Network start mode, see GW_WF_START_MODE
 */
VOID gw_write_wf_start_mode(GW_WF_START_MODE wf_start_mode);

/**
 * @brief Get network configuration type
 *
 * @param[out] nc_type Network configuration type, see GW_WF_NWC_FAST_STAT_T

 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET gw_fast_get_nc_type(GW_WF_NWC_FAST_STAT_T *nc_type);

/**
 * @brief Set device AP config
 *
 * @param[out] ssid SSID to connect
 * @param[out] passwd Password of AP

 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET set_user_def_ap_if(IN CONST CHAR_T *ssid, IN CONST CHAR_T *passwd);

/**
 * @brief Get device AP config
 *
 * @param[out] ssid SSID to connect
 * @param[out] passwd Password of AP

 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET get_dev_ap_if(OUT CHAR_T *ssid, OUT CHAR_T *passwd);

/**
 * @brief Set maximum of clients that are allowed to connect this device
 *
 * @param[in] max_conn Max count
 */
VOID set_max_sta_conn(IN CONST UCHAR_T max_conn);

/**
 * @brief Set device network info
 *
 * @param[in] ip IP addr.
 * @param[in] gw Gateway addr.
 * @param[in] mask Network mask
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
VOID set_nw_ip(IN CONST CHAR_T *ip, IN CONST CHAR_T *mask, IN CONST CHAR_T *gw);

/**
 * @brief Get network configuration error code
 *
 * @param[out] nw_cfg_err_code Error code, see NW_CFG_ERR_CODE_E
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET get_wf_gw_nw_cfg_err_code(OUT NW_CFG_ERR_CODE_E *nw_cfg_err_code);

/**
 * @brief Configure network with user defined params
 *
 * @param[in] ssid SSID to connect
 * @param[in] passwd Password of AP
 * @param[in] token The token
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET gw_wifi_user_cfg(IN CONST CHAR_T *ssid, IN CONST CHAR_T *passwd,
                             IN CONST CHAR_T *token);

/**
 * @brief Set network configuration token
 *
 * @param[in] token Token got from network configuration
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET gw_user_token_bind(IN CONST CHAR_T *token);

/**
 * @brief Reset work state according to network configuration method
 *
 * @param[in] mthd Network configuration method, see GW_WF_CFG_MTHD_SEL
 * @param[in,out] gw_wsm Work state, see GW_WORK_STAT_MAG_S
 */
VOID gw_reset_nc_type(IN CONST GW_WF_CFG_MTHD_SEL mthd, INOUT GW_WORK_STAT_MAG_S *gw_wsm);

/**
 * @brief Whether in wechat network configuration or not
 *
 * @return TRUE on positive, FALSE on negative
 */
BOOL_T gw_check_wechat_mode(VOID);

/**
 * @brief Error dispatch when network configuration fail
 *
 * @param[in] err_code Error code, see AP_CFG_ERR_CODE
 */
VOID gw_rept_netcfg_type(AP_CFG_ERR_CODE err_code);

/**
 * @brief Connect Wi-Fi with given ssid and password, together with runtime region and env
 *
 * @param[in] ssid SSID to connect
 * @param[in] passwd Password of AP
 * @param[in] region Device runtime region
 * @param[in] env Device runtime environment
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET gw_ble_actived_add_wifi_info(IN CONST CHAR_T *ssid, IN CONST CHAR_T *passwd,
                                         IN CONST CHAR_T *region, IN CONST CHAR_T *env);

#ifdef __cplusplus
}
#endif

#endif // TUYA_DEVOS_NETCFG_H
