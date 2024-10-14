/*
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
#ifndef _SHARP_HEAD_V33_H_
#define _SHARP_HEAD_V33_H_

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

///////////////////////////sharp//////////////////////////////////////
// clang-format off

typedef struct CalibDbV2_SharpV33_Luma_t {
    // M4_ARRAY_DESC("luma_point", "u16", M4_SIZE(1,8), M4_RANGE(0,1024), "[0, 64, 128, 256, 384, 640, 896, 1024]", M4_DIGIT(1), M4_DYNAMIC(0))
    uint16_t luma_point[8];
    // M4_ARRAY_DESC("luma_sigma", "u16", M4_SIZE(1,8), M4_RANGE(0,1023), "[8, 12, 16, 16, 24, 20, 16, 16]", M4_DIGIT(0), M4_DYNAMIC(0))
    uint16_t luma_sigma[8];
    // M4_ARRAY_DESC("hf_clip", "u16", M4_SIZE(1,8), M4_RANGE(0,1023), "[256, 256, 256, 256, 350, 512, 512, 512]", M4_DIGIT(0), M4_DYNAMIC(0))
    uint16_t hf_clip[8];
} CalibDbV2_SharpV33_Luma_t;

typedef struct CalibDbV2_SharpV33_kernel_s {
    // M4_ARRAY_DESC("prefilter_coeff", "f32", M4_SIZE(1,3), M4_RANGE(0,1), "[0.2042,0.1238,0.0751]", M4_DIGIT(4), M4_DYNAMIC(0))
    float prefilter_coeff[3];
    // M4_ARRAY_DESC("GaussianFilter_coeff", "f32", M4_SIZE(1,6), M4_RANGE(0.0,1.0), "[0.2042,0.1238,0.0751,0,0,0]", M4_DIGIT(4), M4_DYNAMIC(0))
    float GaussianFilter_coeff[6];
    // M4_ARRAY_DESC("hfBilateralFilter_coeff", "f32", M4_SIZE(1,3), M4_RANGE(0.0,1.0), "[0.2042,0.1238,0.0751]", M4_DIGIT(4), M4_DYNAMIC(0))
    float hfBilateralFilter_coeff[3];

    // M4_ARRAY_DESC("GaussianFilter_coeff_0", "f32", M4_SIZE(1,6), M4_RANGE(0.0,1.0), "[0.0632,0.0558,0.0492,0.0383,0.0338,0.0232]", M4_DIGIT(4), M4_DYNAMIC(0))
    float GaussianFilter_coeff_0[6];
    // M4_ARRAY_DESC("GaussianFilter_coeff_1", "f32", M4_SIZE(1,6), M4_RANGE(0.0,1.0), "[0.0632,0.0558,0.0492,0.0383,0.0338,0.0232]", M4_DIGIT(4), M4_DYNAMIC(0))
    float GaussianFilter_coeff_1[6];
} CalibDbV2_SharpV33_kernel_t;

typedef struct CalibDbV2_SharpV33_Ksigma_s {
    // M4_NUMBER_MARK_DESC("prefilter_sigma", "f32", M4_RANGE(0.0, 100.0), "1.0", M4_DIGIT(2))
    float prefilter_sigma;
    // M4_NUMBER_MARK_DESC("hfBilateralFilter_sigma", "f32", M4_RANGE(0.0, 100.0), "1.0", M4_DIGIT(2))
    float hfBilateralFilter_sigma;
    // M4_NUMBER_MARK_DESC("GaussianFilter_sigma", "f32", M4_RANGE(0.0, 100.0), "1.5", M4_DIGIT(2))
    float GaussianFilter_sigma;
    // M4_NUMBER_MARK_DESC("GaussianFilter_radius", "u8", M4_RANGE(1, 2), "2", M4_DIGIT(0))
    uint8_t GaussianFilter_radius;

    // M4_NUMBER_MARK_DESC("GaussianFilter_sigma_0", "f32", M4_RANGE(0.0, 100.0), "1.0", M4_DIGIT(2))
    float GaussianFilter_sigma_0;
    // M4_NUMBER_MARK_DESC("GaussianFilter_sigma_1", "f32", M4_RANGE(0.0, 100.0), "1.0", M4_DIGIT(2))
    float GaussianFilter_sigma_1;
    // M4_NUMBER_MARK_DESC("GaussianFilter_radius_0", "u8", M4_RANGE(1, 2), "2", M4_DIGIT(0))
    uint8_t GaussianFilter_radius_0;
    // M4_NUMBER_MARK_DESC("GaussianFilter_radius_1", "u8", M4_RANGE(1, 2), "2", M4_DIGIT(0))
    uint8_t GaussianFilter_radius_1;

} CalibDbV2_SharpV33_Ksigma_t;

typedef struct CalibDbV2_SharpV33_T_ISO_s {
    // M4_NUMBER_MARK_DESC("iso", "f32", M4_RANGE(50, 204800), "50", M4_DIGIT(1), "index2")
    float iso;
    // M4_BOOL_DESC("exgain_bypass", "0")
    bool exgain_bypass;
    // M4_NUMBER_DESC("pbf_gain", "f32", M4_RANGE(0.0, 4.0), "0.5", M4_DIGIT(2))
    float pbf_gain;
    // M4_NUMBER_DESC("pbf_ratio", "f32", M4_RANGE(0.0, 1.0), "0.5", M4_DIGIT(2))
    float pbf_ratio;
    // M4_NUMBER_DESC("pbf_add", "f32", M4_RANGE(0.0, 1023.0), "0.0", M4_DIGIT(2))
    float pbf_add;
    // M4_NUMBER_DESC("gaus_ratio", "f32", M4_RANGE(0.0, 1.0), "0.0", M4_DIGIT(2))
    float gaus_ratio;
    // M4_NUMBER_DESC("sharp_ratio", "f32", M4_RANGE(0.0, 31.275), "8", M4_DIGIT(2))
    float sharp_ratio;

    // M4_NUMBER_DESC("sharp_ratio_0", "f32", M4_RANGE(0.0, 31.275), "4", M4_DIGIT(2))
    float sharp_ratio_0;
    // M4_NUMBER_DESC("sharp_ratio_1", "f32", M4_RANGE(0.0, 31.275), "4", M4_DIGIT(2))
    float sharp_ratio_1;

    // M4_NUMBER_DESC("bf_gain", "f32", M4_RANGE(0.0, 4.0), "0.5", M4_DIGIT(2))
    float bf_gain;
    // M4_NUMBER_DESC("bf_ratio", "f32", M4_RANGE(0.0, 1.0), "0.5", M4_DIGIT(2))
    float bf_ratio;
    // M4_NUMBER_DESC("bf_add", "f32", M4_RANGE(0.0, 1023.0), "0.0", M4_DIGIT(2))
    float bf_add;
    // M4_NUMBER_DESC("global_gain", "f32", M4_RANGE(0.0, 63.0), "1.0", M4_DIGIT(2))
    float global_gain;
    // M4_NUMBER_DESC("global_gain_alpha", "f32", M4_RANGE(0.0, 1.0), "0.0", M4_DIGIT(2))
    float global_gain_alpha;
    // M4_NUMBER_DESC("local_gainscale", "f32", M4_RANGE(0.0, 1.0), "0.0", M4_DIGIT(2))
    float local_gainscale;
    // M4_NUMBER_DESC("global_hf_clip_pos", "u8", M4_RANGE(0, 2), "0", M4_DIGIT(0))
    uint8_t global_hf_clip_pos;
    // M4_NUMBER_DESC("noiseclip_strength", "f32", M4_RANGE(0, 10.0), "1", M4_DIGIT(1))
    float noiseclip_strength;
    // M4_NUMBER_DESC("enhance_bit", "u8", M4_RANGE(0, 9), "3", M4_DIGIT(0))
    uint8_t enhance_bit;
    // M4_BOOL_DESC("noiseclip_mode","0")
    bool noiseclip_mode;
    // M4_NUMBER_DESC("noise_sigma_clip", "u16", M4_RANGE(0, 1023), "1023", M4_DIGIT(0))
    uint16_t noise_sigma_clip;
    // M4_ARRAY_DESC("gain_adj_sharp_strength", "f32", M4_SIZE(1,14), M4_RANGE(0.0,31.275), "[1.0]", M4_DIGIT(3), M4_DYNAMIC(0))
    float gain_adj_sharp_strength[14];
    // M4_ARRAY_DESC("dis_adj_sharp_strength", "f32", M4_SIZE(1,22), M4_RANGE(0.0,1.0), "[1.0]", M4_DIGIT(3), M4_DYNAMIC(0))
    float dis_adj_sharp_strength[22];
    // M4_ARRAY_TABLE_DESC("lumaPara", "array_table_ui", "none")
    CalibDbV2_SharpV33_Luma_t luma_para;
    // M4_ARRAY_TABLE_DESC("kernel_para", "array_table_ui", "none")
    CalibDbV2_SharpV33_kernel_t kernel_para;
    // M4_ARRAY_TABLE_DESC("kernel_sigma", "array_table_ui", "none")
    CalibDbV2_SharpV33_Ksigma_t kernel_sigma;

} CalibDbV2_SharpV33_T_ISO_t;
typedef struct CalibDbV2_SharpV33_T_Set_s {
    // M4_STRING_MARK_DESC("SNR_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "LSNR",M4_DYNAMIC(0), "index1")
    char* SNR_Mode;
    // M4_STRING_DESC("Sensor_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "lcg", M4_DYNAMIC(0))
    char* Sensor_Mode;
    // M4_STRUCT_LIST_DESC("Tuning_ISO", M4_SIZE_DYNAMIC, "double_indx_list")
    CalibDbV2_SharpV33_T_ISO_t* Tuning_ISO;
    int Tuning_ISO_len;
} CalibDbV2_SharpV33_T_Set_t;

typedef struct CalibDbV2_SharpV33_Tuning_s {
    // M4_BOOL_DESC("enable", "1")
    bool enable;
    // M4_BOOL_DESC("sharp_ratio_seperate_en", "0")
    bool sharp_ratio_seperate_en;
    // M4_BOOL_DESC("kernel_sigma_enable", "0")
    bool kernel_sigma_enable;
    // M4_BOOL_DESC("Center_Mode", "0")
    bool Center_Mode;
    // M4_STRUCT_LIST_DESC("Setting", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_SharpV33_T_Set_t* Setting;
    int Setting_len;
} CalibDbV2_SharpV33_Tuning_t;


typedef struct CalibDbV2_SharpV33LT_Luma_t {
    // M4_ARRAY_DESC("luma_point", "u16", M4_SIZE(1,8), M4_RANGE(0,1024), "[0, 64, 128, 256, 384, 640, 896, 1024]", M4_DIGIT(1), M4_DYNAMIC(0))
    uint16_t luma_point[8];
    // M4_ARRAY_DESC("luma_sigma", "u16", M4_SIZE(1,8), M4_RANGE(0,1023), "[8, 12, 16, 16, 24, 20, 16, 16]", M4_DIGIT(0), M4_DYNAMIC(0))
    uint16_t luma_sigma[8];
    // M4_ARRAY_DESC("hf_clip", "u16", M4_SIZE(1,8), M4_RANGE(0,1023), "[256, 256, 256, 256, 350, 512, 512, 512]", M4_DIGIT(0), M4_DYNAMIC(0))
    uint16_t hf_clip[8];
    // M4_ARRAY_DESC("hf_clip_neg", "u16", M4_SIZE(1,8), M4_RANGE(0,1023), "[256, 256, 256, 256, 350, 512, 512, 512]", M4_DIGIT(0), M4_DYNAMIC(0))
    uint16_t hf_clip_neg[8];
    // M4_ARRAY_DESC("local_sharp_strength", "f32", M4_SIZE(1,8), M4_RANGE(0,1023), "[256, 256, 256, 256, 350, 512, 512, 512]", M4_DIGIT(1), M4_DYNAMIC(0))
    float local_sharp_strength[8];
} CalibDbV2_SharpV33LT_Luma_t;

typedef struct CalibDbV2_SharpV33LT_T_ISO_s {
    // M4_NUMBER_MARK_DESC("iso", "f32", M4_RANGE(50, 204800), "50", M4_DIGIT(1), "index2")
    float iso;
    // M4_BOOL_DESC("exgain_bypass", "0")
    bool exgain_bypass;

    // M4_BOOL_DESC("clip_hf_mode", "0")
    bool clip_hf_mode;
    // M4_BOOL_DESC("add_mode", "0")
    bool add_mode;

    // M4_NUMBER_DESC("pbf_gain", "f32", M4_RANGE(0.0, 4.0), "0.5", M4_DIGIT(2))
    float pbf_gain;
    // M4_NUMBER_DESC("pbf_ratio", "f32", M4_RANGE(0.0, 1.0), "0.5", M4_DIGIT(2))
    float pbf_ratio;
    // M4_NUMBER_DESC("pbf_add", "f32", M4_RANGE(0.0, 1023.0), "0.0", M4_DIGIT(2))
    float pbf_add;
    // M4_NUMBER_DESC("gaus_ratio", "f32", M4_RANGE(0.0, 1.0), "0.0", M4_DIGIT(2))
    float gaus_ratio;
    // M4_NUMBER_DESC("sharp_ratio", "f32", M4_RANGE(0.0, 31.275), "8", M4_DIGIT(2))
    float sharp_ratio;

    // M4_NUMBER_DESC("sharp_ratio_0", "f32", M4_RANGE(0.0, 31.275), "4", M4_DIGIT(2))
    float sharp_ratio_0;
    // M4_NUMBER_DESC("sharp_ratio_1", "f32", M4_RANGE(0.0, 31.275), "4", M4_DIGIT(2))
    float sharp_ratio_1;

    // M4_NUMBER_DESC("bf_gain", "f32", M4_RANGE(0.0, 4.0), "0.5", M4_DIGIT(2))
    float bf_gain;
    // M4_NUMBER_DESC("bf_ratio", "f32", M4_RANGE(0.0, 1.0), "0.5", M4_DIGIT(2))
    float bf_ratio;
    // M4_NUMBER_DESC("bf_add", "f32", M4_RANGE(0.0, 1023.0), "0.0", M4_DIGIT(2))
    float bf_add;
    // M4_NUMBER_DESC("global_gain", "f32", M4_RANGE(0.0, 63.0), "1.0", M4_DIGIT(2))
    float global_gain;
    // M4_NUMBER_DESC("global_gain_alpha", "f32", M4_RANGE(0.0, 1.0), "0.0", M4_DIGIT(2))
    float global_gain_alpha;
    // M4_NUMBER_DESC("local_gainscale", "f32", M4_RANGE(0.0, 1.0), "1.0", M4_DIGIT(2))
    float local_gainscale;
#if 0
    // M4_NUMBER_DESC("global_hf_clip_pos", "u8", M4_RANGE(0, 2), "0", M4_DIGIT(0))
    uint8_t global_hf_clip_pos;
    // M4_NUMBER_DESC("noiseclip_strength", "f32", M4_RANGE(0, 10.0), "1", M4_DIGIT(1))
    float noiseclip_strength;
    // M4_NUMBER_DESC("enhance_bit", "u8", M4_RANGE(0, 9), "3", M4_DIGIT(0))
    uint8_t enhance_bit;
    // M4_BOOL_DESC("noiseclip_mode","0")
    bool noiseclip_mode;
    // M4_NUMBER_DESC("noise_sigma_clip", "u16", M4_RANGE(0, 1023), "1023", M4_DIGIT(0))
    uint16_t noise_sigma_clip;
#endif
    // M4_ARRAY_DESC("gain_adj_sharp_strength", "f32", M4_SIZE(1,14), M4_RANGE(0.0,31.275), "[1.0]", M4_DIGIT(3), M4_DYNAMIC(0))
    float gain_adj_sharp_strength[14];
    // M4_ARRAY_DESC("dis_adj_sharp_strength", "f32", M4_SIZE(1,22), M4_RANGE(0.0,1.0), "[1.0]", M4_DIGIT(3), M4_DYNAMIC(0))
    float dis_adj_sharp_strength[22];
    // M4_ARRAY_TABLE_DESC("lumaPara", "array_table_ui", "none")
    CalibDbV2_SharpV33LT_Luma_t luma_para;
    // M4_ARRAY_TABLE_DESC("kernel_para", "array_table_ui", "none")
    CalibDbV2_SharpV33_kernel_t kernel_para;
    // M4_ARRAY_TABLE_DESC("kernel_sigma", "array_table_ui", "none")
    CalibDbV2_SharpV33_Ksigma_t kernel_sigma;

} CalibDbV2_SharpV33LT_T_ISO_t;

typedef struct CalibDbV2_SharpV33LT_T_Set_s {
    // M4_STRING_MARK_DESC("SNR_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "LSNR",M4_DYNAMIC(0), "index1")
    char* SNR_Mode;
    // M4_STRING_DESC("Sensor_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "lcg", M4_DYNAMIC(0))
    char* Sensor_Mode;
    // M4_STRUCT_LIST_DESC("Tuning_ISO", M4_SIZE_DYNAMIC, "double_indx_list")
    CalibDbV2_SharpV33LT_T_ISO_t* Tuning_ISO;
    int Tuning_ISO_len;
} CalibDbV2_SharpV33LT_T_Set_t;

typedef struct CalibDbV2_SharpV33LT_Tuning_s {
    // M4_BOOL_DESC("enable", "1")
    bool enable;
    // M4_BOOL_DESC("sharp_ratio_seperate_en", "0")
    bool sharp_ratio_seperate_en;
    // M4_BOOL_DESC("kernel_sigma_enable", "0")
    bool kernel_sigma_enable;
    // M4_BOOL_DESC("Center_Mode", "0")
    bool Center_Mode;
    // M4_STRUCT_LIST_DESC("Setting", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_SharpV33LT_T_Set_t* Setting;
    int Setting_len;
} CalibDbV2_SharpV33LT_Tuning_t;


typedef struct CalibDbV2_SharpV33_s {
    // M4_STRING_DESC("Version", M4_SIZE(1,1), M4_RANGE(0, 64), "V33", M4_DYNAMIC(0))
    char* Version;
    // M4_STRUCT_DESC("TuningPara", "normal_ui_style")
    CalibDbV2_SharpV33_Tuning_t TuningPara;
} CalibDbV2_SharpV33_t;

typedef struct CalibDbV2_SharpV33Lite_s {
    // M4_STRING_DESC("Version", M4_SIZE(1,1), M4_RANGE(0, 64), "V33", M4_DYNAMIC(0))
    char* Version;
    // M4_STRUCT_DESC("TuningPara", "normal_ui_style")
    CalibDbV2_SharpV33LT_Tuning_t TuningPara;
} CalibDbV2_SharpV33Lite_t;

// clang-format on
RKAIQ_END_DECLARE

#endif
