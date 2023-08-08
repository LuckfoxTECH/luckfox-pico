/**
 * @file tuya_hal_ble_mesh_prov.h
 * @brief ble mesh provisioner - Initialization
 * @version 0.1
 * @date 2021-04-19
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */
#ifndef __TUYA_HAL_BLE_MESH_PROV_H__
#define __TUYA_HAL_BLE_MESH_PROV_H__

#include "tuya_os_adapter.h"

#if defined(TY_BT_MOD) && (TY_BT_MOD == 1)

#include "tuya_cloud_types.h"
#include <stdbool.h>
#include <stdint.h>

OPERATE_RET tuya_hal_bt_mesh_prov_init(ty_ble_mesh_prov_param_t *p);

OPERATE_RET tuya_hal_bt_mesh_prov_deinit(VOID_T);

OPERATE_RET tuya_hal_bt_mesh_prov_set_info(ty_ble_mesh_info_t *info);

OPERATE_RET tuya_hal_bt_mesh_prov_set_addr(UINT16_T mesh_addr);

OPERATE_RET tuya_hal_bt_mesh_prov_scan(BOOL_T enable);

OPERATE_RET tuya_hal_bt_mesh_prov_del_dev_with_addr(UINT16_T mesh_addr, UINT8_T dev_key[16]);

OPERATE_RET tuya_hal_bt_mesh_prov_send_data(UINT_T opcode, UINT16_T mesh_addr, UINT8_T *data,
                                            UINT_T length, UINT8_T dev_key[16]);

OPERATE_RET tuya_hal_bt_mesh_prov_node_reset(VOID_T);

VOID_T tuya_os_ble_mesh_prov_intf_init(VOID_T);

#endif

#endif
