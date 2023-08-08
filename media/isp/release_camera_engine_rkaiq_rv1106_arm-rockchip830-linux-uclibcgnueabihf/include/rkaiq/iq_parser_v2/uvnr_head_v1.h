/*
 * uvnr_head_v1.h
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

#ifndef __CALIBDBV2_UVNR_HEADER_V1_H__
#define __CALIBDBV2_UVNR_HEADER_V1_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

///////////////////////////uvnr//////////////////////////////////////
typedef struct CalibDbV2_UVNR_T_ISO_s {
    // M4_NUMBER_MARK_DESC("iso", "f32", M4_RANGE(50, 204800), "50", M4_DIGIT(1), "index2")
    float iso;

    // M4_NUMBER_DESC("step0_uvgrad_ratio", "f32", M4_RANGE(1.0, 63.0), "40", M4_DIGIT(0))
    float step0_uvgrad_ratio;
    // M4_NUMBER_DESC("step0_uvgrad_offset", "f32", M4_RANGE(0, 1.0), "0.8", M4_DIGIT(2))
    float step0_uvgrad_offset;

    // M4_NUMBER_DESC("step1_median_ratio", "f32", M4_RANGE(0, 1.0), "0.2", M4_DIGIT(2))
    float step1_median_ratio;
    // M4_NUMBER_DESC("step1_bf_sigmaR", "f32", M4_RANGE(2, 64.0), "10.0", M4_DIGIT(2))
    float step1_bf_sigmaR;
    // M4_NUMBER_DESC("step1_bf_uvgain", "f32", M4_RANGE(0.0, 7.9), "1.5", M4_DIGIT(2))
    float step1_bf_uvgain;
    // M4_NUMBER_DESC("step1_bf_ratio", "f32", M4_RANGE(0.0, 1.0), "0.0", M4_DIGIT(2))
    float step1_bf_ratio;

    // M4_NUMBER_DESC("step2_median_ratio", "f32", M4_RANGE(0, 1.0), "0.2", M4_DIGIT(2))
    float step2_median_ratio;
    // M4_NUMBER_DESC("step2_bf_sigmaR", "f32", M4_RANGE(2, 48.0), "5.0", M4_DIGIT(2))
    float step2_bf_sigmaR;
    // M4_NUMBER_DESC("step2_bf_uvgain", "f32", M4_RANGE(0.0, 7.9), "2.0", M4_DIGIT(2))
    float step2_bf_uvgain;
    // M4_NUMBER_DESC("step2_bf_ratio", "f32", M4_RANGE(0.0, 1.0), "0.0", M4_DIGIT(2), M4_HIDE(1))
    float step2_bf_ratio;

    // M4_NUMBER_DESC("step3_bf_sigmaR", "f32", M4_RANGE(2, 24.0), "5.0", M4_DIGIT(2))
    float step3_bf_sigmaR;
    // M4_NUMBER_DESC("step3_bf_uvgain", "f32", M4_RANGE(0.0, 7.9), "2.0", M4_DIGIT(2))
    float step3_bf_uvgain;
    // M4_NUMBER_DESC("step3_bf_ratio", "f32", M4_RANGE(0.0, 1.0), "1.0", M4_DIGIT(2))
    float step3_bf_ratio;

} CalibDbV2_UVNR_T_ISO_t;

typedef struct CalibDbV2_UVNR_T_Set_s {
    // M4_STRING_MARK_DESC("SNR_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "LSNR",M4_DYNAMIC(0), "index1")
    char *SNR_Mode;
    // M4_STRING_DESC("Sensor_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "lcg", M4_DYNAMIC(0))
    char *Sensor_Mode;
    // M4_STRUCT_LIST_DESC("Tuning_ISO", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_UVNR_T_ISO_t *Tuning_ISO;
    int Tuning_ISO_len;

} CalibDbV2_UVNR_T_Set_t;

typedef struct CalibDbV2_UVNR_Kernel_s {
    // M4_ARRAY_DESC("kernel_3x3", "f32", M4_SIZE(1,3), M4_RANGE(0,1), "[1.0000,0.8825,0.7788]", M4_DIGIT(6), M4_DYNAMIC(0))
    float kernel_3x3[3];
    // M4_ARRAY_DESC("kernel_5x5", "f32", M4_SIZE(1,5), M4_RANGE(0,1), "[1.0000,0.8825,0.7788,0.6065,0.3679]", M4_DIGIT(6), M4_DYNAMIC(0))
    float kernel_5x5[5];
    // M4_ARRAY_DESC("kernel_9x9", "f32", M4_SIZE(1,8), M4_RANGE(0,1), "[1.0000,0.8825,0.7788,0.6065,0.3679,0.1969,0.1353,0.0439]", M4_DIGIT(6), M4_DYNAMIC(0))
    float kernel_9x9[8];
    // M4_NUMBER_DESC("kernel_9x9_num", "s32", M4_RANGE(0, 3), "0", M4_DIGIT(0))
    int kernel_9x9_num;
} CalibDbV2_UVNR_Kernel_t;


typedef struct CalibDbV2_UVNR_Tuning_s {
    // M4_BOOL_DESC("enable", "1")
    int enable;
    // M4_STRUCT_DESC("Kernel_Coeff", "normal_ui_style")
    CalibDbV2_UVNR_Kernel_t Kernel_Coeff;
    // M4_STRUCT_LIST_DESC("Setting", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_UVNR_T_Set_t *Setting;
    int Setting_len;
} CalibDbV2_UVNR_Tuning_t;


typedef struct CalibDbV2_UVNR_s {
    // M4_STRING_DESC("Version", M4_SIZE(1,1), M4_RANGE(0, 64), "V1", M4_DYNAMIC(0))
    char *Version;
    // M4_STRUCT_DESC("TuningPara", "normal_ui_style")
    CalibDbV2_UVNR_Tuning_t TuningPara;
} CalibDbV2_UVNR_t;

RKAIQ_END_DECLARE

#endif
