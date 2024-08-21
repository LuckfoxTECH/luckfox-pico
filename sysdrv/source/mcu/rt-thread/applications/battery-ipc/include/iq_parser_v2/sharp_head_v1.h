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

#ifndef __CALIBDBV2_SHARP_HEADER_V1_H__
#define __CALIBDBV2_SHARP_HEADER_V1_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

///////////////////////////sharp//////////////////////////////////////

typedef struct CalibDbV2_SharpV1_Luma_s {
    // M4_ARRAY_DESC("luma_point", "f32", M4_SIZE(1,8), M4_RANGE(0,255), "[0,16,32,64,96,160,224,256]", M4_DIGIT(1), M4_DYNAMIC(0))
    float luma_point[8];
    // M4_ARRAY_DESC("luma_sigma", "f32", M4_SIZE(1,8), M4_RANGE(0,255), "[8, 10, 10, 12, 14, 12, 12, 10]", M4_DIGIT(1), M4_DYNAMIC(0))
    float luma_sigma[8];
    // M4_ARRAY_DESC("mf_clip_pos", "f32", M4_SIZE(1,8), M4_RANGE(0,255), "[8, 16, 20, 32.0000 46, 46, 46, 40,]", M4_DIGIT(1), M4_DYNAMIC(0))
    float mf_clip_pos[8];
    // M4_ARRAY_DESC("mf_clip_neg", "f32", M4_SIZE(1,8), M4_RANGE(0,255), "[-0.5, -0.6, -0.6 -0.8, -0.8,-0.8,-0.8, -0.5]", M4_DIGIT(2), M4_DYNAMIC(0))
    float mf_clip_neg[8];
    // M4_ARRAY_DESC("hf_clip", "f32", M4_SIZE(1,8), M4_RANGE(0,255), "[10, 20, 30, 56, 56, 56, 56, 30]", M4_DIGIT(1), M4_DYNAMIC(0))
    float hf_clip[8];
} CalibDbV2_SharpV1_Luma_t;

typedef struct CalibDbV2_SharpV1_T_ISO_s {
    // M4_NUMBER_MARK_DESC("iso", "f32", M4_RANGE(50, 204800), "50", M4_DIGIT(1), "index2")
    float iso;
    // M4_NUMBER_DESC("hratio", "f32", M4_RANGE(1.0, 2.0), "2.0", M4_DIGIT(2))
    float hratio;
    // M4_NUMBER_DESC("lratio", "f32", M4_RANGE(0.0, 1.0), "1.0", M4_DIGIT(2))
    float lratio;
    // M4_NUMBER_DESC("mf_sharp_ratio", "f32", M4_RANGE(0.0, 8.0), "3.0", M4_DIGIT(2))
    float mf_sharp_ratio;
    // M4_NUMBER_DESC("hf_sharp_ratio", "f32", M4_RANGE(0.0, 8.0), "5.0", M4_DIGIT(2))
    float hf_sharp_ratio;

    // M4_NUMBER_DESC("pbf_gain", "f32", M4_RANGE(0.0, 2.0), "0.5", M4_DIGIT(2))
    float pbf_gain;
    // M4_NUMBER_DESC("pbf_ratio", "f32", M4_RANGE(0.0, 1.0), "0.5", M4_DIGIT(2))
    float pbf_ratio;
    // M4_NUMBER_DESC("pbf_add", "f32", M4_RANGE(0.0, 255.0), "0.0", M4_DIGIT(2))
    float pbf_add;

    // M4_NUMBER_DESC("mbf_gain", "f32", M4_RANGE(0.0, 2.0), "0.5", M4_DIGIT(2))
    float mbf_gain;
    // M4_NUMBER_DESC("mbf_add", "f32", M4_RANGE(0.0, 255.0), "0.0", M4_DIGIT(2))
    float mbf_add;

    // M4_NUMBER_DESC("hbf_gain", "f32", M4_RANGE(0.0, 2.0), "0.5", M4_DIGIT(2))
    float hbf_gain;
    // M4_NUMBER_DESC("hbf_ratio", "f32", M4_RANGE(0.0, 1.0), "0.5", M4_DIGIT(2))
    float hbf_ratio;
    // M4_NUMBER_DESC("hbf_add", "f32", M4_RANGE(0.0, 255.0), "0.0", M4_DIGIT(2))
    float hbf_add;

    // M4_NUMBER_DESC("local_sharp_strength", "f32", M4_RANGE(0.0, 255.0), "0.5", M4_DIGIT(2))
    float local_sharp_strength;

    // M4_NUMBER_DESC("pbf_coeff_percent", "f32", M4_RANGE(0.0, 1.0), "0.5", M4_DIGIT(2))
    float pbf_coeff_percent;
    // M4_NUMBER_DESC("rf_m_coeff_percent", "f32", M4_RANGE(0.0, 1.0), "0.5", M4_DIGIT(2))
    float rf_m_coeff_percent;
    // M4_NUMBER_DESC("rf_h_coeff_percent", "f32", M4_RANGE(0.0, 1.0), "0.5", M4_DIGIT(2))
    float rf_h_coeff_percent;
    // M4_NUMBER_DESC("hbf_coeff_percent", "f32", M4_RANGE(0.0, 1.0), "0.5", M4_DIGIT(2))
    float hbf_coeff_percent;

    // M4_ARRAY_TABLE_DESC("luma_para", "array_table_ui", "none")
    CalibDbV2_SharpV1_Luma_t luma_para;
} CalibDbV2_SharpV1_T_ISO_t;

typedef struct CalibDbV2_SharpV1_T_Set_s {
    // M4_STRING_MARK_DESC("SNR_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "LSNR",M4_DYNAMIC(0), "index1")
    char *SNR_Mode;
    // M4_STRING_DESC("Sensor_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "lcg", M4_DYNAMIC(0))
    char *Sensor_Mode;
    // M4_STRUCT_LIST_DESC("Tuning_ISO", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_SharpV1_T_ISO_t *Tuning_ISO;
    int Tuning_ISO_len;
} CalibDbV2_SharpV1_T_Set_t;

typedef struct CalibDbV2_SharpV1_Kernel_s {
    // M4_ARRAY_DESC("gauss_luma_coeff", "f32", M4_SIZE(3,3), M4_RANGE(0,1), "[0.0625,0.125,0.0625,0.125,0.25,0.125,0.0625,0.125,0.0625]", M4_DIGIT(6), M4_DYNAMIC(0))
    float gauss_luma_coeff[9];
    // M4_ARRAY_DESC("pbf_coeff_l", "f32", M4_SIZE(3,3), M4_RANGE(0,1), "[0.15625,0.25,0.15625,0.25,0.375,0.25,0.15625,0.25,0.15625]",M4_DIGIT(6), M4_DYNAMIC(0))
    float pbf_coeff_l[9];
    // M4_ARRAY_DESC("pbf_coeff_h", "f32", M4_SIZE(3,3), M4_RANGE(0,1), "[0.15625,0.25,0.15625,0.25,0.375,0.25,0.15625,0.25,0.15625]",M4_DIGIT(6), M4_DYNAMIC(0))
    float pbf_coeff_h[9];
    // M4_ARRAY_DESC("rf_m_coeff_l", "f32", M4_SIZE(5,5), M4_RANGE(0,1), "[0.023438,0.03125,0.039063,0.03125,0.023438,0.03125,0.046875,0.054688,0.046875,0.03125,0.039063,0.054688,0.09375,0.054688,0.039063,0.03125,0.046875,0.054688,0.046875,0.03125,0.023438,0.03125,0.039063,0.03125,0.023438]",M4_DIGIT(6), M4_DYNAMIC(0))
    float rf_m_coeff_l[25];
    // M4_ARRAY_DESC("rf_m_coeff_h", "f32", M4_SIZE(5,5), M4_RANGE(0,1), "[0.023438,0.03125,0.039063,0.03125,0.023438,0.03125,0.046875,0.054688,0.046875,0.03125,0.039063,0.054688,0.09375,0.054688,0.039063,0.03125,0.046875,0.054688,0.046875,0.03125,0.023438,0.03125,0.039063,0.03125,0.023438]",M4_DIGIT(6), M4_DYNAMIC(0))
    float rf_m_coeff_h[25];
    // M4_ARRAY_DESC("rf_m_coeff_h", "f32", M4_SIZE(17,13), M4_RANGE(0,1), "0.004",M4_DIGIT(6), M4_DYNAMIC(0),M4_HIDE(1))
    float mbf_coeff[221];
    // M4_ARRAY_DESC("rf_h_coeff_l", "f32", M4_SIZE(5,5), M4_RANGE(0,1), "[0,0.015625,0.023438,0.015625,0,0.015625,0.0625,0.101563,0.0625,0.015625,0.023438,0.101563,0.125,0.101563,0.023438,0.015625,0.0625,0.101563,0.0625,0.015625,0,0.015625,0.023438,0.015625,0]",M4_DIGIT(6), M4_DYNAMIC(0))
    float rf_h_coeff_l[25];
    // M4_ARRAY_DESC("rf_h_coeff_h", "f32", M4_SIZE(5,5), M4_RANGE(0,1), "[0,0.015625,0.023438,0.015625,0,0.015625,0.0625,0.101563,0.0625,0.015625,0.023438,0.101563,0.125,0.101563,0.023438,0.015625,0.0625,0.101563,0.0625,0.015625,0,0.015625,0.023438,0.015625,0]",M4_DIGIT(6), M4_DYNAMIC(0))
    float rf_h_coeff_h[25];
    // M4_ARRAY_DESC("hbf_coeff_l", "f32", M4_SIZE(3,3), M4_RANGE(0,1), "[0.15625,0.25,0.15625,0.25,0.375,0.25,0.15625,0.25,0.15625]",M4_DIGIT(6), M4_DYNAMIC(0))
    float hbf_coeff_l[9];
    // M4_ARRAY_DESC("hbf_coeff_h", "f32", M4_SIZE(3,3), M4_RANGE(0,1), "[0.15625,0.25,0.15625,0.25,0.375,0.25,0.15625,0.25,0.15625]",M4_DIGIT(6), M4_DYNAMIC(0))
    float hbf_coeff_h[9];
} CalibDbV2_SharpV1_Kernel_t;


typedef struct CalibDbV2_SharpV1_Tuning_s {
    // M4_BOOL_DESC("enable", "1")
    bool enable;
    // M4_STRUCT_DESC("kernel_coeff", "normal_ui_style")
    CalibDbV2_SharpV1_Kernel_t kernel_coeff;
    // M4_STRUCT_LIST_DESC("Setting", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_SharpV1_T_Set_t *Setting;
    int Setting_len;

} CalibDbV2_SharpV1_Tuning_t;

typedef struct CalibDbV2_SharpV1_s {
    // M4_STRING_DESC("Version", M4_SIZE(1,1), M4_RANGE(0, 64), "V1", M4_DYNAMIC(0))
    char *Version;
    // M4_STRUCT_DESC("TuningPara", "normal_ui_style")
    CalibDbV2_SharpV1_Tuning_t TuningPara;
} CalibDbV2_SharpV1_t;

RKAIQ_END_DECLARE

#endif

