/*
 * sharp_head_v1.h
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

#ifndef __CALIBDBV2_SHARPV4_HEADER_H__
#define __CALIBDBV2_SHARPV4_HEADER_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

///////////////////////////sharp//////////////////////////////////////

typedef struct CalibDbV2_SharpV4_ISO_Luma_s {
    // M4_ARRAY_DESC("luma_point", "f32", M4_SIZE(1,8), M4_RANGE(0,1024), "[0, 64, 128, 256, 384, 640, 896, 1024]", M4_DIGIT(1), M4_DYNAMIC(0))
    float luma_point[8];
    // M4_ARRAY_DESC("luma_sigma", "f32", M4_SIZE(1,8), M4_RANGE(0,1023), "[8, 10, 10, 12, 14, 12, 12, 10]", M4_DIGIT(1), M4_DYNAMIC(0))
    float luma_sigma[8];
    // M4_ARRAY_DESC("hf_clip", "f32", M4_SIZE(1,8), M4_RANGE(0,1023), "[10, 20, 30, 56, 56, 56, 56, 30]", M4_DIGIT(1), M4_DYNAMIC(0))
    float hf_clip[8];
    // M4_ARRAY_DESC("local_sharp_strength", "f32", M4_SIZE(1,8), M4_RANGE(0,1023), "[1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023]", M4_DIGIT(1), M4_DYNAMIC(0))
    float local_sharp_strength[8];
} CalibDbV2_SharpV4_ISO_Luma_t;

typedef struct CalibDbV2_SharpV4_kernel_s {

    // M4_ARRAY_DESC("prefilter_coeff", "f32", M4_SIZE(1,3), M4_RANGE(0,1), "[0.2042,0.1238,0.0751]", M4_DIGIT(4), M4_DYNAMIC(0))
    float prefilter_coeff[3];
    // M4_ARRAY_DESC("GaussianFilter_coeff", "f32", M4_SIZE(1,6), M4_RANGE(0,1), "[0.2042,0.1238,0.0751]", M4_DIGIT(4), M4_DYNAMIC(0))
    float GaussianFilter_coeff[6];
    // M4_ARRAY_DESC("hfBilateralFilter_coeff", "f32", M4_SIZE(1,3), M4_RANGE(0,1), "[0.2042,0.1238,0.0751]", M4_DIGIT(4), M4_DYNAMIC(0))
    float hfBilateralFilter_coeff[3];
} CalibDbV2_SharpV4_kernel_t;

typedef struct CalibDbV2_SharpV4_sigma_s {

    // M4_ARRAY_DESC("prefilter_sigma", "f32", M4_SIZE(1,1), M4_RANGE(0,100), "[1.0]", M4_DIGIT(2), M4_DYNAMIC(0))
    float prefilter_sigma;
    // M4_ARRAY_DESC("hfBilateralFilter_sigma", "f32", M4_SIZE(1,1), M4_RANGE(0,100), "[1.0]", M4_DIGIT(2), M4_DYNAMIC(0))
    float hfBilateralFilter_sigma;
    // M4_ARRAY_DESC("GaussianFilter_sigma", "f32", M4_SIZE(1,1), M4_RANGE(0,100), "[1.0]", M4_DIGIT(2), M4_DYNAMIC(0))
    float GaussianFilter_sigma;
    // M4_ARRAY_DESC("GaussianFilter_radius", "f32", M4_SIZE(1,1), M4_RANGE(1,2), "[2.0]", M4_DIGIT(0), M4_DYNAMIC(0))
    float GaussianFilter_radius;

} CalibDbV2_SharpV4_sigma_t;

typedef struct CalibDbV2_SharpV4_Set_ISO_s {
    // M4_NUMBER_MARK_DESC("iso", "f32", M4_RANGE(50, 204800), "50", M4_DIGIT(1), "index2")
    float iso;

    // M4_NUMBER_DESC("pbf_gain", "f32", M4_RANGE(0.0, 2.0), "0.5", M4_DIGIT(2))
    float pbf_gain;
    // M4_NUMBER_DESC("pbf_ratio", "f32", M4_RANGE(0.0, 1.0), "0.5", M4_DIGIT(2))
    float pbf_ratio;
    // M4_NUMBER_DESC("pbf_add", "f32", M4_RANGE(0.0, 1023.0), "0.0", M4_DIGIT(2))
    float pbf_add;

    // M4_NUMBER_DESC("gaus_ratio", "f32", M4_RANGE(0.0, 1.0), "0.0", M4_DIGIT(2))
    float gaus_ratio;

    // M4_NUMBER_DESC("sharp_ratio", "f32", M4_RANGE(0.0, 32.0), "0.5", M4_DIGIT(2))
    float sharp_ratio;


    // M4_NUMBER_DESC("bf_gain", "f32", M4_RANGE(0.0, 2.0), "0.5", M4_DIGIT(2))
    float bf_gain;
    // M4_NUMBER_DESC("bf_ratio", "f32", M4_RANGE(0.0, 1.0), "0.5", M4_DIGIT(2))
    float bf_ratio;
    // M4_NUMBER_DESC("bf_add", "f32", M4_RANGE(0.0, 1023.0), "0.0", M4_DIGIT(2))
    float bf_add;

    // M4_ARRAY_TABLE_DESC("luma_para", "array_table_ui", "none")
    CalibDbV2_SharpV4_ISO_Luma_t luma_para;

    // M4_ARRAY_TABLE_DESC("kernel_para", "array_table_ui", "none")
    CalibDbV2_SharpV4_kernel_t kernel_para;

    // M4_ARRAY_TABLE_DESC("kernel_sigma", "array_table_ui", "none")
    CalibDbV2_SharpV4_sigma_t kernel_sigma;
} CalibDbV2_SharpV4_Set_ISO_t;

typedef struct CalibDbV2_SharpV4_Set_s {
    // M4_STRING_MARK_DESC("SNR_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "LSNR",M4_DYNAMIC(0), "index1")
    char *SNR_Mode;
    // M4_STRING_DESC("Sensor_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "lcg", M4_DYNAMIC(0))
    char *Sensor_Mode;
    // M4_STRUCT_LIST_DESC("Tuning_ISO", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_SharpV4_Set_ISO_t *Tuning_ISO;
    int Tuning_ISO_len;
} CalibDbV2_SharpV4_Set_t;

typedef struct CalibDbV2_SharpV4_Tuning_s {
    // M4_BOOL_DESC("enable", "1")
    bool enable;

    // M4_BOOL_DESC("kernel_sigma_enable", "1")
    bool kernel_sigma_enable;

    // M4_STRUCT_LIST_DESC("Setting", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_SharpV4_Set_t *Setting;
    int Setting_len;

} CalibDbV2_SharpV4_Tuning_t;

typedef struct CalibDbV2_SharpV4_s {
    // M4_STRING_DESC("Version", M4_SIZE(1,1), M4_RANGE(0, 64), "V3", M4_DYNAMIC(0))
    char *Version;
    // M4_STRUCT_DESC("TuningPara", "normal_ui_style")
    CalibDbV2_SharpV4_Tuning_t TuningPara;
} CalibDbV2_SharpV4_t;

RKAIQ_END_DECLARE

#endif

