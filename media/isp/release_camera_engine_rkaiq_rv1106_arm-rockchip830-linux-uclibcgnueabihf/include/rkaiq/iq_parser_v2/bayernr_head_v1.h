/*
 * bayernr_head_v1.h
 *
 *  Copyright (c) 2021 Rockchip Corporation
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
 *
 */

#ifndef __CALIBDBV2_BAYERNR_HEADER_V1_H__
#define __CALIBDBV2_BAYERNR_HEADER_V1_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

////////////////////////bayernr V1//////////////////////////////////////
typedef struct CalibDbV2_BayerNrV1_C_ISO_s {
    // M4_NUMBER_MARK_DESC("iso", "f32", M4_RANGE(50, 204800), "50", M4_DIGIT(1), "index2")
    float iso;
    // M4_ARRAY_DESC("luLevelVal", "f32", M4_SIZE(1,8), M4_RANGE(0,255), "0.0", M4_DIGIT(0), M4_DYNAMIC(0))
    float luLevelVal[8];
    // M4_ARRAY_DESC("luRatio", "f32", M4_SIZE(1,8), M4_RANGE(0,15.9), "0.0", M4_DIGIT(1), M4_DYNAMIC(0))
    float luRatio[8];
} CalibDbV2_BayerNrV1_C_ISO_t;

typedef struct CalibDbV2_BayerNrV1_C_Set_s {
    // M4_STRING_MARK_DESC("SNR_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "LSNR",M4_DYNAMIC(0), "index1")
    char *SNR_Mode;
    // M4_STRING_DESC("Sensor_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "lcg", M4_DYNAMIC(0))
    char *Sensor_Mode;
    // M4_STRUCT_LIST_DESC("Calib_ISO", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_BayerNrV1_C_ISO_t *Calib_ISO;
    int Calib_ISO_len;

} CalibDbV2_BayerNrV1_C_Set_t;

typedef struct CalibDbV2_BayerNrV1_Calib_s {
    // M4_STRUCT_LIST_DESC("Setting", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_BayerNrV1_C_Set_t *Setting;
    int Setting_len;
} CalibDbV2_BayerNrV1_Calib_t;

typedef struct CalibDbV2_BayerNrV1_T_ISO_s {
    // M4_NUMBER_MARK_DESC("iso", "f32", M4_RANGE(50, 204800), "50", M4_DIGIT(1), "index2")
    float iso;
    // M4_BOOL_DESC("gauss_en", "1")
    bool gauss_en;
    // M4_NUMBER_DESC("filtPara", "f32", M4_RANGE(0, 15.9), "0.0", M4_DIGIT(1))
    float filtPara;
    // M4_NUMBER_DESC("lamda", "f32", M4_RANGE(0, 16383), "0.0", M4_DIGIT(0))
    float lamda;
    // M4_NUMBER_DESC("fixW0", "f32", M4_RANGE(0, 1.0), "0.0", M4_DIGIT(1))
    float fixW0;
    // M4_NUMBER_DESC("fixW1", "f32", M4_RANGE(0, 1.0), "0.0", M4_DIGIT(1))
    float fixW1;
    // M4_NUMBER_DESC("fixW2", "f32", M4_RANGE(0, 1.0), "0.0", M4_DIGIT(1))
    float fixW2;
    // M4_NUMBER_DESC("fixW3", "f32", M4_RANGE(0, 1.0), "0.0", M4_DIGIT(1))
    float fixW3;
    // M4_NUMBER_DESC("RGainOff", "f32", M4_RANGE(0, 1.0), "0.0", M4_DIGIT(1), M4_HIDE(1))
    float RGainOff;
    // M4_NUMBER_DESC("RGainFilp", "f32", M4_RANGE(0, 1.0), "0.0", M4_DIGIT(1), M4_HIDE(1))
    float RGainFilp;
    // M4_NUMBER_DESC("BGainOff", "f32", M4_RANGE(0, 1.0), "0.0", M4_DIGIT(1), M4_HIDE(1))
    float BGainOff;
    // M4_NUMBER_DESC("BGainFilp", "f32", M4_RANGE(0, 1.0), "0.0", M4_DIGIT(1), M4_HIDE(1))
    float BGainFilp;
} CalibDbV2_BayerNrV1_T_ISO_t;

typedef struct CalibDbV2_BayerNrV1_T_Set_s {
    // M4_STRING_MARK_DESC("SNR_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "LSNR",M4_DYNAMIC(0), "index1")
    char *SNR_Mode;
    // M4_STRING_DESC("Sensor_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "lcg", M4_DYNAMIC(0))
    char *Sensor_Mode;
    // M4_STRUCT_LIST_DESC("Tuning_ISO", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_BayerNrV1_T_ISO_t *Tuning_ISO;
    int Tuning_ISO_len;
} CalibDbV2_BayerNrV1_T_Set_t;

typedef struct CalibDbV2_BayerNrV1_Tuning_s {
    // M4_BOOL_DESC("enable", "1")
    bool enable;
    // M4_STRUCT_LIST_DESC("Setting", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_BayerNrV1_T_Set_t *Setting;
    int Setting_len;
} CalibDbV2_BayerNrV1_Tuning_t;

typedef struct CalibDbV2_BayerNrV1_s {
    // M4_STRING_DESC("Version", M4_SIZE(1,1), M4_RANGE(0, 64), "V1", M4_DYNAMIC(0))
    char *Version;
    // M4_STRUCT_DESC("CalibPara", "normal_ui_style")
    CalibDbV2_BayerNrV1_Calib_t CalibPara;
    // M4_STRUCT_DESC("TuningPara", "normal_ui_style")
    CalibDbV2_BayerNrV1_Tuning_t TuningPara;
} CalibDbV2_BayerNrV1_t;

RKAIQ_END_DECLARE

#endif
