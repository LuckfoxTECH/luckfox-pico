/*
 * Copyright (c) 2019-2022 Rockchip Eletronics Co., Ltd.
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
#ifndef __CALIBDBV2_ABLC_UAPI_HEADER_V32_H__
#define __CALIBDBV2_ABLC_UAPI_HEADER_V32_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE
typedef struct AblcSelect_V32_s {
    // M4_BOOL_DESC("enable", "1")
    bool enable;
    // M4_NUMBER_DESC("blc_r", "f32", M4_RANGE(0,4095), "0",M4_DIGIT(4))
    float blc_r;
    // M4_NUMBER_DESC("blc_gr", "f32", M4_RANGE(0,4095), "0",M4_DIGIT(4))
    float blc_gr;
    // M4_NUMBER_DESC("blc_gb", "f32", M4_RANGE(0,4095), "0",M4_DIGIT(4))
    float blc_gb;
    // M4_NUMBER_DESC("blc_b", "f32", M4_RANGE(0,4095), "0",M4_DIGIT(4))
    float blc_b;
} AblcSelect_V32_t;

typedef struct AblcOBSelect_V32_s {
    // M4_BOOL_DESC("enable", "1")
    bool enable;
    // M4_NUMBER_DESC("ob_offset", "f32", M4_RANGE(0,4095), "0",M4_DIGIT(4))
    float ob_offset;
    // M4_NUMBER_DESC("ob_predgain", "f32", M4_RANGE(0,4095), "0",M4_DIGIT(4))
    float ob_predgain;
} AblcOBSelect_V32_t;

typedef struct AblcManualSetting_s {
    // M4_STRUCT_DESC("blc0_para", "normal_ui_style")
    AblcSelect_V32_t blc0_para;

    // M4_STRUCT_DESC("blc1_para", "normal_ui_style")
    AblcSelect_V32_t blc1_para;

    // M4_STRUCT_DESC("blc_ob_para", "normal_ui_style")
    AblcOBSelect_V32_t blc_ob_para;
} AblcManualSetting_t;

typedef struct AblcExpInfo_V32_s {
    // M4_NUMBER_DESC("hdr_mode", "u8", M4_RANGE(0, 2), "0", M4_DIGIT(0))
    int hdr_mode;

    // M4_NUMBER_DESC("snr_mode", "s8", M4_RANGE(0, 2), "0", M4_DIGIT(0))
    int snr_mode;

    // M4_NUMBER_DESC("bayertnr_en", "s8", M4_RANGE(0, 1), "0", M4_DIGIT(0))
    int bayertnr_en;

    // M4_ARRAY_DESC("time", "f32", M4_SIZE(1,3), M4_RANGE(0, 1024), "0.01", M4_DIGIT(6))
    float arTime[3];
    // M4_ARRAY_DESC("again", "f32", M4_SIZE(1,3), M4_RANGE(0, 204800), "1", M4_DIGIT(3))
    float arAGain[3];
    // M4_ARRAY_DESC("dgain", "f32", M4_SIZE(1,3), M4_RANGE(0, 204800), "1", M4_DIGIT(3))
    float arDGain[3];
    // M4_ARRAY_DESC("iso", "u32", M4_SIZE(1,3), M4_RANGE(0, 204800), "1", M4_DIGIT(0))
    int   arIso[3];

    // M4_NUMBER_DESC("isoLevelLow", "u8", M4_RANGE(0, 12), "0", M4_DIGIT(0))
    int isoLevelLow;
    // M4_NUMBER_DESC("isoLevelHig", "u8", M4_RANGE(0, 12), "0", M4_DIGIT(0))
    int isoLevelHig;

} AblcExpInfo_V32_t;

typedef struct rk_aiq_blc_info_v32_s {
    // M4_ARRAY_TABLE_DESC("sync", "array_table_ui", "none", "1")
    rk_aiq_uapi_sync_t sync;
    // M4_NUMBER_DESC("iso", "u32", M4_RANGE(0, 204800), "50", M4_DIGIT(0), "0", "0")
    int iso;
    // M4_ARRAY_TABLE_DESC("expo_info", "normal_ui_style", "none", "0", "0")
    AblcExpInfo_V32_t expo_info;
} rk_aiq_blc_info_v32_t;

RKAIQ_END_DECLARE

#endif
