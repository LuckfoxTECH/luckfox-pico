/*
 *  Copyright (c) 2022 Rockchip Eletronics Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _RK_AIQ_TYPE_ACAC_ALGO_INT_H_
#define _RK_AIQ_TYPE_ACAC_ALGO_INT_H_

#include <stdint.h>

#include "acac/rk_aiq_types_acac_algo.h"
#include "common/rk_aiq_comm.h"
#include "iq_parser_v2/cac_head.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RKCAC_STRENGTH_TABLE_LEN 22
#define RKAIQ_CAC_MAX_ISO_CNT    14

typedef enum rkaiq_cac_api_op_mode_e {
    RKAIQ_CAC_API_OPMODE_INVALID = 0,
    RKAIQ_CAC_API_OPMODE_AUTO    = 1,
    RKAIQ_CAC_API_OPMODE_MANUAL  = 2,
} rkaiq_cac_api_op_mode_t;

typedef CalibDbV2_Cac_SettingPara_t rkaiq_cac_v10_persist_params_t;
typedef CalibDbV2_Cac_SettingPara_t rkaiq_cac_v11_persist_params_t;
typedef CalibV2_Cac_V03_SettingByIso_t rkaiq_cac_v03_param_selected_t;
typedef CalibV2_Cac_V10_SettingByIso_t rkaiq_cac_v10_param_selected_t;
typedef CalibV2_Cac_V11_SettingByIso_t rkaiq_cac_v11_param_selected_t;

typedef struct rkaiq_cac_v03_api_attr_s {
    rk_aiq_uapi_sync_t sync;
    bool enable;
    rkaiq_cac_api_op_mode_t op_mode;
    uint8_t iso_cnt;
    rkaiq_cac_v03_param_selected_t auto_params[RKAIQ_CAC_MAX_ISO_CNT];
    rkaiq_cac_v03_param_selected_t manual_param;
} rkaiq_cac_v03_api_attr_t;

typedef struct rkaiq_cac_v10_api_attr_s {
    rk_aiq_uapi_sync_t sync;
    bool enable;
    rkaiq_cac_api_op_mode_t op_mode;
    uint8_t iso_cnt;
    rkaiq_cac_v10_persist_params_t persist_params;
    rkaiq_cac_v10_param_selected_t auto_params[RKAIQ_CAC_MAX_ISO_CNT];
    rkaiq_cac_v10_param_selected_t manual_param;
} rkaiq_cac_v10_api_attr_t;

typedef struct rkaiq_cac_v11_api_attr_s {
    rk_aiq_uapi_sync_t sync;
    bool enable;
    rkaiq_cac_api_op_mode_t op_mode;
    uint8_t iso_cnt;
    rkaiq_cac_v11_persist_params_t persist_params;
    rkaiq_cac_v11_param_selected_t auto_params[RKAIQ_CAC_MAX_ISO_CNT];
    rkaiq_cac_v11_param_selected_t manual_param;
} rkaiq_cac_v11_api_attr_t;

#ifdef __cplusplus
}
#endif

#endif
