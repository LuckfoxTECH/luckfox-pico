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

#ifndef __CALIBDBV2_CNR_HEADER_V1_H__
#define __CALIBDBV2_CNR_HEADER_V1_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

///////////////////////////uvnr//////////////////////////////////////
typedef struct CalibDbV2_CNR_T_ISO_s {
    // M4_NUMBER_MARK_DESC("iso", "f32", M4_RANGE(50, 204800), "50", M4_DIGIT(1), "index2")
    float iso;

    // M4_BOOL_DESC("hf_bypass", "0")
    float hf_bypass;
    // M4_BOOL_DESC("lf_bypass", "0")
    float lf_bypass;

    // M4_NUMBER_DESC("cnr_exgain", "f32", M4_RANGE(0, 1.0), "1.0", M4_DIGIT(2), M4_HIDE(1))
    float cnr_exgain;
    // M4_NUMBER_DESC("cnr_g_gain", "f32", M4_RANGE(0, 1.0), "1.0", M4_DIGIT(2), M4_HIDE(1))
    float cnr_g_gain;

    // M4_NUMBER_DESC("color_sat_adj", "f32", M4_RANGE(1, 255.0), "40.0", M4_DIGIT(2))
    float color_sat_adj;
    // M4_NUMBER_DESC("color_sat_adj_alpha", "f32", M4_RANGE(0.0, 1.0), "0.8", M4_DIGIT(2))
    float color_sat_adj_alpha;

    // M4_NUMBER_DESC("hf_spikes_reducion_strength", "f32", M4_RANGE(0.0, 1.0), "0.5", M4_DIGIT(2))
    float hf_spikes_reducion_strength;
    // M4_NUMBER_DESC("hf_denoise_strength", "f32", M4_RANGE(1, 1023.0), "10", M4_DIGIT(2))
    float hf_denoise_strength;
    // M4_NUMBER_DESC("hf_color_sat", "f32", M4_RANGE(0.0, 8.0), "1.5", M4_DIGIT(2))
    float hf_color_sat;
    // M4_NUMBER_DESC("hf_denoise_alpha", "f32", M4_RANGE(0.0, 1.0), "0.0", M4_DIGIT(2))
    float hf_denoise_alpha;
    // M4_NUMBER_DESC("hf_bf_wgt_clip", "u32", M4_RANGE(0, 255), "0", M4_DIGIT(0), M4_HIDE(0))
    int hf_bf_wgt_clip;

    // M4_NUMBER_DESC("thumb_spikes_reducion_strength", "f32", M4_RANGE(0.0, 1.0), "0.2", M4_DIGIT(2))
    float thumb_spikes_reducion_strength;
    // M4_NUMBER_DESC("thumb_denoise_strength", "f32", M4_RANGE(1, 1023), "4.0", M4_DIGIT(2))
    float thumb_denoise_strength;
    // M4_NUMBER_DESC("thumb_color_sat", "f32", M4_RANGE(0.0, 8.0), "4.0", M4_DIGIT(2))
    float thumb_color_sat;


    // M4_NUMBER_DESC("lf_denoise_strength", "f32", M4_RANGE(1, 1023.0), "4.0", M4_DIGIT(2))
    float lf_denoise_strength;
    // M4_NUMBER_DESC("lf_color_sat", "f32", M4_RANGE(0.0, 8.0), "2.0", M4_DIGIT(2))
    float lf_color_sat;
    // M4_NUMBER_DESC("lf_denoise_alpha", "f32", M4_RANGE(0.0, 1.0), "1.0", M4_DIGIT(2))
    float lf_denoise_alpha;

} CalibDbV2_CNR_T_ISO_t;

typedef struct CalibDbV2_CNR_T_Set_s {
    // M4_STRING_MARK_DESC("SNR_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "LSNR",M4_DYNAMIC(0), "index1")
    char *SNR_Mode;
    // M4_STRING_DESC("Sensor_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "lcg", M4_DYNAMIC(0))
    char *Sensor_Mode;
    // M4_STRUCT_LIST_DESC("Tuning_ISO", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_CNR_T_ISO_t *Tuning_ISO;
    int Tuning_ISO_len;

} CalibDbV2_CNR_T_Set_t;

typedef struct CalibDbV2_CNR_Kernel_s {
    // M4_ARRAY_DESC("kernel_5x5", "f32", M4_SIZE(1,5), M4_RANGE(0,1), "[1.0000,0.8825,0.7788,0.6065,0.3679]", M4_DIGIT(6), M4_DYNAMIC(0))
    float kernel_5x5[5];
} CalibDbV2_CNR_Kernel_t;


typedef struct CalibDbV2_CNR_Tuning_t {
    // M4_BOOL_DESC("enable", "1")
    int enable;
    // M4_STRUCT_DESC("Kernel_Coeff", "normal_ui_style")
    CalibDbV2_CNR_Kernel_t Kernel_Coeff;
    // M4_STRUCT_LIST_DESC("Setting", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_CNR_T_Set_t *Setting;
    int Setting_len;
} CalibDbV2_CNR_Tuning_t;


typedef struct CalibDbV2_CNR_s {
    // M4_STRING_DESC("Version", M4_SIZE(1,1), M4_RANGE(0, 64), "V1", M4_DYNAMIC(0))
    char *Version;
    // M4_STRUCT_DESC("TuningPara", "normal_ui_style")
    CalibDbV2_CNR_Tuning_t TuningPara;
} CalibDbV2_CNR_t;

RKAIQ_END_DECLARE

#endif
