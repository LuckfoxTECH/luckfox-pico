// Copyright 2015-2017 Fuzhou Rockchip Electronics Co., Ltd
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __RK_GATT_COMMON_API_H__
#define __RK_GATT_COMMON_API_H__

#include <stdint.h>
#include <stdbool.h>

#include "rk_err.h"
#include "rk_bt_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

// Maximum Transmission Unit used in GATT
#define RK_GATT_DEF_BLE_MTU_SIZE   23   /* relate to GATT_DEF_BLE_MTU_SIZE in stack/gatt_api.h */

// Maximum Transmission Unit allowed in GATT
#define RK_GATT_MAX_MTU_SIZE       517  /* relate to GATT_MAX_MTU_SIZE in stack/gatt_api.h */

/**
 * @brief           This function is called to set local MTU,
 *                  the function is called before BLE connection.
 *
 * @param[in]       mtu: the size of MTU.
 *
 * @return
 *                  - RK_OK: success
 *                  - other: failed
 *
 */
extern osi_err_t rk_ble_gatt_set_local_mtu(uint16_t mtu);

#ifdef __cplusplus
}
#endif

#endif /* __RK_GATT_COMMON_API_H__ */
