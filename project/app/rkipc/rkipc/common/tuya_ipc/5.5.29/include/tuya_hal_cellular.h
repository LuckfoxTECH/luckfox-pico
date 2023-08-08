/**
 * @file tuya_hal_cellular.h
 * @brief cellular相关接口
 *
 * @copyright Copyright(C),2018-2020, 涂鸦科技 www.tuya.com
 *
 */

#ifndef __TUYA_HAL_CELLULAR_H__
#define __TUYA_HAL_CELLULAR_H__

#include "tuya_os_adapter.h"

#ifdef __cplusplus
extern "C" {
#endif

VOID_T tuya_os_cellular_intf_init(VOID_T);

OPERATE_RET tuya_hal_cellular_set_imei(CONST CHAR_T imei[15 + 1]);
OPERATE_RET tuya_hal_cellular_get_imei(CHAR_T imei[15 + 1]);
OPERATE_RET tuya_hal_cellular_get_network_state(BOOL_T *state);
OPERATE_RET tuya_hal_cellular_control(uint8_t cmd, void *arg);
OPERATE_RET tuya_hal_cellular_get_iccid(CHAR_T iccid[20 + 1]);
OPERATE_RET tuya_hal_cellular_get_ip(NW_IP_S *ip);
OPERATE_RET tuya_hal_cellular_get_simcard_state(uint8_t *state);
OPERATE_RET tuya_hal_cellular_get_rssi(SCHAR_T *rssi);
OPERATE_RET tuya_hal_cellular_get_test_intf(CELLULAR_TEST_INTF **intf);
BOOL_T tuya_hal_cellular_calibrated(VOID_T);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
