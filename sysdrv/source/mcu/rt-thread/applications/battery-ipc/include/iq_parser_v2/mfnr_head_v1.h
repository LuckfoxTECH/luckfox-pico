/*
 * mfnr_head_v1.h
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

#ifndef __CALIBDBV2_MFNR_HEADER_V1_H__
#define __CALIBDBV2_MFNR_HEADER_V1_H__

#include <rk_aiq_comm.h>

RKAIQ_BEGIN_DECLARE

///////////////////////////mfnr//////////////////////////////////////
typedef struct CalibDbV2_MFNR_C_ISO_s {
    // M4_NUMBER_MARK_DESC("iso", "f32", M4_RANGE(50, 204800), "50", M4_DIGIT(1), "index2")
    float iso;

    // M4_ARRAY_DESC("noise_curve", "f32", M4_SIZE(1,5), M4_RANGE(-65535,65535), "[-8.4303162971622996e-013,7.9640341802261105e-009,-2.6912484620871701e-005,3.3049135399760801e-002,1.8211244079810101e+001]", M4_DIGIT(6), M4_DYNAMIC(0))
    float noise_curve[5];
    // M4_NUMBER_DESC("noise_curve_x00", "f32", M4_RANGE(0.0, 4095), "3647", M4_DIGIT(0))
    float noise_curve_x00;
    // M4_ARRAY_DESC("y_lo_noiseprofile", "f32", M4_SIZE(1,4), M4_RANGE(0,1), "[1.00000,0.68993,0.41986,0.24798]", M4_DIGIT(6), M4_DYNAMIC(0))
    float y_lo_noiseprofile[4];
    // M4_ARRAY_DESC("y_hi_noiseprofile", "f32", M4_SIZE(1,4), M4_RANGE(0,1), "[0.77114,0.62293,0.46145,0.00000]", M4_DIGIT(6), M4_DYNAMIC(0))
    float y_hi_noiseprofile[4];
    // M4_ARRAY_DESC("uv_lo_noiseprofile", "f32", M4_SIZE(1,3), M4_RANGE(0,1), "[1.00000,0.68993,0.41986]", M4_DIGIT(6), M4_DYNAMIC(0))
    float uv_lo_noiseprofile[3];
    // M4_ARRAY_DESC("uv_hi_noiseprofile", "f32", M4_SIZE(1,3), M4_RANGE(0,1), "[0.77114,0.62293,0.35740]", M4_DIGIT(6), M4_DYNAMIC(0))
    float uv_hi_noiseprofile[3];
} CalibDbV2_MFNR_C_ISO_t;

typedef struct CalibDbV2_MFNR_C_Set_s {
    // M4_STRING_MARK_DESC("SNR_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "LSNR",M4_DYNAMIC(0), "index1")
    char *SNR_Mode;
    // M4_STRING_DESC("Sensor_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "lcg", M4_DYNAMIC(0))
    char *Sensor_Mode;
    // M4_STRUCT_LIST_DESC("Calib_ISO", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_MFNR_C_ISO_t *Calib_ISO;
    int Calib_ISO_len;
} CalibDbV2_MFNR_C_Set_t;

typedef struct CalibDbV2_MFNR_Calib_s {
    // M4_STRUCT_LIST_DESC("Setting", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_MFNR_C_Set_t *Setting;
    int Setting_len;
} CalibDbV2_MFNR_Calib_t;

typedef struct CalibDbV2_MFNR_Luma_s {
    // M4_ARRAY_DESC("y_lumanrpoint", "f32", M4_SIZE(1,6), M4_RANGE(0,256), "[0.0000,32.0000,64.0000,128.0000,192.0000,256.0000]", M4_DIGIT(0), M4_DYNAMIC(0))
    float y_lumanrpoint[6];
    // M4_ARRAY_DESC("y_lumanrcurve", "f32", M4_SIZE(1,6), M4_RANGE(0,2.0), "[1.0,1.0,1.0,1.0,1.0,1.0]", M4_DIGIT(2), M4_DYNAMIC(0))
    float y_lumanrcurve[6];
    // M4_ARRAY_DESC("uv_lumanrpoint", "f32", M4_SIZE(1,6), M4_RANGE(0,256), "[0.0000,32.0000,64.0000,128.0000,192.0000,256.0000]", M4_DIGIT(0), M4_DYNAMIC(0))
    float uv_lumanrpoint[6];
    // M4_ARRAY_DESC("uv_lumanrcurve", "f32", M4_SIZE(1,6), M4_RANGE(0,2.0), "[1.0,1.0,1.0,1.0,1.0,1.0]", M4_DIGIT(2), M4_DYNAMIC(0))
    float uv_lumanrcurve[6];
} CalibDbV2_MFNR_Luma_t;

typedef struct CalibDbV2_MFNR_Ygfdelta_s {
    // M4_ARRAY_DESC("y_lo_lvl0_gfdelta", "f32", M4_SIZE(1,6), M4_RANGE(0,1.0), "[0.06250,0.03906,0.03906,0.03906,0.03906,0.03906]", M4_DIGIT(6), M4_DYNAMIC(0))
    float y_lo_lvl0_gfdelta[6];
    // M4_ARRAY_DESC("y_hi_lvl0_gfdelta", "f32", M4_SIZE(1,6), M4_RANGE(0,1.0), "[0.06250,0.03906,0.03906,0.03906,0.03906,0.03906]", M4_DIGIT(6), M4_DYNAMIC(0))
    float y_hi_lvl0_gfdelta[6];
    // M4_ARRAY_DESC("y_lo_lvl1_gfdelta", "f32", M4_SIZE(1,3), M4_RANGE(0,1.0), "[0.12500,0.10938,0.10938]", M4_DIGIT(6), M4_DYNAMIC(0))
    float y_lo_lvl1_gfdelta[3];
    // M4_ARRAY_DESC("y_hi_lvl1_gfdelta", "f32", M4_SIZE(1,3), M4_RANGE(0,1.0), "[0.12500,0.10938,0.10938]", M4_DIGIT(6), M4_DYNAMIC(0))
    float y_hi_lvl1_gfdelta[3];
    // M4_ARRAY_DESC("y_lo_lvl2_gfdelta", "f32", M4_SIZE(1,3), M4_RANGE(0,1.0), "[0.12500,0.10938,0.10938]", M4_DIGIT(6), M4_DYNAMIC(0))
    float y_lo_lvl2_gfdelta[3];
    // M4_ARRAY_DESC("y_hi_lvl2_gfdelta", "f32", M4_SIZE(1,3), M4_RANGE(0,1.0), "[0.12500,0.10938,0.10938]", M4_DIGIT(6), M4_DYNAMIC(0))
    float y_hi_lvl2_gfdelta[3];
    // M4_ARRAY_DESC("y_lo_lvl3_gfdelta", "f32", M4_SIZE(1,3), M4_RANGE(0,1.0), "[0.12500,0.10938,0.10938]", M4_DIGIT(6), M4_DYNAMIC(0))
    float y_lo_lvl3_gfdelta[3];
    // M4_ARRAY_DESC("y_hi_lvl3_gfdelta", "f32", M4_SIZE(1,3), M4_RANGE(0,1.0), "[0.12500,0.10938,0.10938]", M4_DIGIT(6), M4_DYNAMIC(0))
    float y_hi_lvl3_gfdelta[3];
} CalibDbV2_MFNR_Ygfdelta_t;

typedef struct CalibDbV2_MFNR_UVgfdelta_s {
    // M4_ARRAY_DESC("uv_lo_lvl0_gfdelta", "f32", M4_SIZE(1,6), M4_RANGE(0,1.0), "[0.06250,0.03906,0.03906,0.03906,0.03906,0.03906]", M4_DIGIT(6), M4_DYNAMIC(0))
    float uv_lo_lvl0_gfdelta[6];
    // M4_ARRAY_DESC("uv_hi_lvl0_gfdelta", "f32", M4_SIZE(1,6), M4_RANGE(0,1.0), "[0.06250,0.03906,0.03906,0.03906,0.03906,0.03906]", M4_DIGIT(6), M4_DYNAMIC(0))
    float uv_hi_lvl0_gfdelta[6];
    // M4_ARRAY_DESC("uv_lo_lvl1_gfdelta", "f32", M4_SIZE(1,3), M4_RANGE(0,1.0), "[0.12500,0.10938,0.10938]", M4_DIGIT(6), M4_DYNAMIC(0))
    float uv_lo_lvl1_gfdelta[3];
    // M4_ARRAY_DESC("uv_hi_lvl1_gfdelta", "f32", M4_SIZE(1,3), M4_RANGE(0,1.0), "[0.12500,0.10938,0.10938]", M4_DIGIT(6), M4_DYNAMIC(0))
    float uv_hi_lvl1_gfdelta[3];
    // M4_ARRAY_DESC("uv_lo_lvl2_gfdelta", "f32", M4_SIZE(1,3), M4_RANGE(0,1.0), "[0.12500,0.10938,0.10938]", M4_DIGIT(6), M4_DYNAMIC(0))
    float uv_lo_lvl2_gfdelta[3];
    // M4_ARRAY_DESC("uv_hi_lvl2_gfdelta", "f32", M4_SIZE(1,3), M4_RANGE(0,1.0), "[0.12500,0.10938,0.10938]", M4_DIGIT(6), M4_DYNAMIC(0))
    float uv_hi_lvl2_gfdelta[3];
} CalibDbV2_MFNR_UVgfdelta_t;

typedef struct CalibDbV2_MFNR_gfsigma_s {
    // M4_ARRAY_DESC("lvl0_gfsigma", "f32", M4_SIZE(1,6), M4_RANGE(0,1.0), "[0.06250,0.03906,0.03906,0.03906,0.03906,0.03906]", M4_DIGIT(6), M4_DYNAMIC(0))
    float lvl0_gfsigma[6];
    // M4_ARRAY_DESC("lvl1_gfsigma", "f32", M4_SIZE(1,3), M4_RANGE(0,1.0), "[0.12500,0.10938,0.10938]", M4_DIGIT(6), M4_DYNAMIC(0))
    float lvl1_gfsigma[3];
    // M4_ARRAY_DESC("lvl2_gfsigma", "f32", M4_SIZE(1,3), M4_RANGE(0,1.0), "[0.12500,0.10938,0.10938]", M4_DIGIT(6), M4_DYNAMIC(0))
    float lvl2_gfsigma[3];
    // M4_ARRAY_DESC("lvl3_gfsigma", "f32", M4_SIZE(1,3), M4_RANGE(0,1.0), "[0.12500,0.10938,0.10938]", M4_DIGIT(6), M4_DYNAMIC(0))
    float lvl3_gfsigma[3];
} CalibDbV2_MFNR_gfsigma_t;

typedef struct CalibDbV2_MFNR_T_ISO_s {
    // M4_NUMBER_MARK_DESC("iso", "f32", M4_RANGE(50, 204800), "50", M4_DIGIT(1), "index2")
    float iso;

    // M4_NUMBER_DESC("weight_limit_y_0", "f32", M4_RANGE(0.0, 255), "64", M4_DIGIT(0))
    float weight_limit_y_0;
    // M4_NUMBER_DESC("weight_limit_y_1", "f32", M4_RANGE(0.0, 255), "64", M4_DIGIT(0))
    float weight_limit_y_1;
    // M4_NUMBER_DESC("weight_limit_y_2", "f32", M4_RANGE(0.0, 255), "64", M4_DIGIT(0))
    float weight_limit_y_2;
    // M4_NUMBER_DESC("weight_limit_y_3", "f32", M4_RANGE(0.0, 255), "64", M4_DIGIT(0))
    float weight_limit_y_3;

    // M4_NUMBER_DESC("weight_limit_uv_0", "f32", M4_RANGE(0.0, 255), "64", M4_DIGIT(0))
    float weight_limit_uv_0;
    // M4_NUMBER_DESC("weight_limit_uv_1", "f32", M4_RANGE(0.0, 255), "64", M4_DIGIT(0))
    float weight_limit_uv_1;
    // M4_NUMBER_DESC("weight_limit_uv_2", "f32", M4_RANGE(0.0, 255), "64", M4_DIGIT(0))
    float weight_limit_uv_2;

    // M4_NUMBER_DESC("ratio_frq_y_l", "f32", M4_RANGE(0.0, 7.9), "0.5", M4_DIGIT(1))
    float ratio_frq_y_l;
    // M4_NUMBER_DESC("ratio_frq_y_h", "f32", M4_RANGE(0.0, 7.9), "2.0", M4_DIGIT(1))
    float ratio_frq_y_h;
    // M4_NUMBER_DESC("ratio_frq_uv_l", "f32", M4_RANGE(0.0, 7.9), "0.5", M4_DIGIT(1))
    float ratio_frq_uv_l;
    // M4_NUMBER_DESC("ratio_frq_uv_h", "f32", M4_RANGE(0.0, 7.9), "2.0", M4_DIGIT(1))
    float ratio_frq_uv_h;

    // M4_NUMBER_DESC("luma_w_in_chroma_0", "f32", M4_RANGE(0.0, 1.0), "1.0", M4_DIGIT(2))
    float luma_w_in_chroma_0;
    // M4_NUMBER_DESC("luma_w_in_chroma_1", "f32", M4_RANGE(0.0, 1.0), "1.0", M4_DIGIT(2))
    float luma_w_in_chroma_1;
    // M4_NUMBER_DESC("luma_w_in_chroma_2", "f32", M4_RANGE(0.0, 1.0), "1.0", M4_DIGIT(2))
    float luma_w_in_chroma_2;

    // M4_NUMBER_DESC("y_lo_bfscale_0", "f32", M4_RANGE(0.0, 8.0), "0.5", M4_DIGIT(2))
    float y_lo_bfscale_0;
    // M4_NUMBER_DESC("y_lo_bfscale_1", "f32", M4_RANGE(0.0, 8.0), "0.5", M4_DIGIT(2))
    float y_lo_bfscale_1;
    // M4_NUMBER_DESC("y_lo_bfscale_2", "f32", M4_RANGE(0.0, 8.0), "0.5", M4_DIGIT(2))
    float y_lo_bfscale_2;
    // M4_NUMBER_DESC("y_lo_bfscale_3", "f32", M4_RANGE(0.0, 8.0), "0.5", M4_DIGIT(2))
    float y_lo_bfscale_3;

    // M4_NUMBER_DESC("y_hi_bfscale_0", "f32", M4_RANGE(0.0, 8.0), "0.5", M4_DIGIT(2))
    float y_hi_bfscale_0;
    // M4_NUMBER_DESC("y_hi_bfscale_1", "f32", M4_RANGE(0.0, 8.0), "0.5", M4_DIGIT(2))
    float y_hi_bfscale_1;
    // M4_NUMBER_DESC("y_hi_bfscale_2", "f32", M4_RANGE(0.0, 8.0), "0.5", M4_DIGIT(2))
    float y_hi_bfscale_2;
    // M4_NUMBER_DESC("y_hi_bfscale_3", "f32", M4_RANGE(0.0, 8.0), "0.5", M4_DIGIT(2))
    float y_hi_bfscale_3;
    // M4_NUMBER_DESC("y_denoisestrength", "f32", M4_RANGE(0.0, 4.0), "1.0", M4_DIGIT(2))
    float y_denoisestrength;

    // M4_NUMBER_DESC("uv_lo_bfscale_0", "f32", M4_RANGE(0.0, 8.0), "0.5", M4_DIGIT(2))
    float uv_lo_bfscale_0;
    // M4_NUMBER_DESC("uv_lo_bfscale_1", "f32", M4_RANGE(0.0, 8.0), "0.5", M4_DIGIT(2))
    float uv_lo_bfscale_1;
    // M4_NUMBER_DESC("uv_lo_bfscale_2", "f32", M4_RANGE(0.0, 8.0), "0.5", M4_DIGIT(2))
    float uv_lo_bfscale_2;

    // M4_NUMBER_DESC("uv_hi_bfscale_0", "f32", M4_RANGE(0.0, 8.0), "0.5", M4_DIGIT(2))
    float uv_hi_bfscale_0;
    // M4_NUMBER_DESC("uv_hi_bfscale_1", "f32", M4_RANGE(0.0, 8.0), "0.5", M4_DIGIT(2))
    float uv_hi_bfscale_1;
    // M4_NUMBER_DESC("uv_hi_bfscale_2", "f32", M4_RANGE(0.0, 8.0), "0.5", M4_DIGIT(2))
    float uv_hi_bfscale_2;
    // M4_NUMBER_DESC("uv_denoisestrength", "f32", M4_RANGE(0.0, 4.0), "1.0", M4_DIGIT(2))
    float uv_denoisestrength;

    // M4_ARRAY_TABLE_DESC("luma_para", "array_table_ui", "none")
    CalibDbV2_MFNR_Luma_t luma_para;
    // M4_ARRAY_TABLE_DESC("y_gfdelta_para", "array_table_ui", "none", M4_HIDE(1))
    CalibDbV2_MFNR_Ygfdelta_t y_gfdelta_para;
    // M4_ARRAY_TABLE_DESC("uv_gfdelta_para", "array_table_ui", "none", M4_HIDE(1))
    CalibDbV2_MFNR_UVgfdelta_t uv_gfdelta_para;
    // M4_ARRAY_TABLE_DESC("gfsigma_para", "array_table_ui", "none", M4_HIDE(1))
    CalibDbV2_MFNR_gfsigma_t gfsigma_para;

} CalibDbV2_MFNR_T_ISO_t;


typedef struct CalibDbV2_MFNR_T_Set_s {
    // M4_STRING_MARK_DESC("SNR_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "LSNR",M4_DYNAMIC(0), "index1")
    char *SNR_Mode;
    // M4_STRING_DESC("Sensor_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "lcg", M4_DYNAMIC(0))
    char *Sensor_Mode;
    // M4_STRUCT_LIST_DESC("Tuning_ISO", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_MFNR_T_ISO_t *Tuning_ISO;
    int Tuning_ISO_len;
} CalibDbV2_MFNR_T_Set_t;

typedef struct CalibDbV2_MFNR_Dynamic_s {
    // M4_BOOL_DESC("Enable", "1")
    bool Enable;
    // M4_NUMBER_DESC("LowTh_iso", "f32", M4_RANGE(0.0, 2.0), "1.0", M4_DIGIT(2))
    float LowTh_iso;
    // M4_NUMBER_DESC("LowTh_time", "f32", M4_RANGE(0.0, 2.0), "1.0", M4_DIGIT(2))
    float LowTh_time;
    // M4_NUMBER_DESC("HighTh_iso", "f32", M4_RANGE(0.0, 2.0), "1.0", M4_DIGIT(2))
    float HighTh_iso;
    // M4_NUMBER_DESC("HighTh_time", "f32", M4_RANGE(0.0, 2.0), "1.0", M4_DIGIT(2))
    float HighTh_time;
} CalibDbV2_MFNR_Dynamic_t;


typedef struct CalibDbV2_MFNR_Motion_ISO_s {
    // M4_NUMBER_DESC("iso", "f32", M4_RANGE(50, 204800), "50", M4_DIGIT(1))
    float iso;
    // M4_NUMBER_DESC("sigmaHScale", "f32", M4_RANGE(0.0, 2.0), "1.0", M4_DIGIT(2))
    float sigmaHScale;
    // M4_NUMBER_DESC("sigmaLScale", "f32", M4_RANGE(0.0, 2.0), "0.5", M4_DIGIT(2))
    float sigmaLScale;
    // M4_NUMBER_DESC("lightClp", "f32", M4_RANGE(0.0, 64), "32", M4_DIGIT(2))
    float lightClp;
    // M4_NUMBER_DESC("uvWeight", "f32", M4_RANGE(0.0, 1.0), "1.0", M4_DIGIT(2))
    float uvWeight;
    // M4_NUMBER_DESC("mfnrSigmaScale", "f32", M4_RANGE(1.0, 2.0), "1.0", M4_DIGIT(2))
    float mfnrSigmaScale;
    // M4_NUMBER_DESC("yuvnrGainScale0", "f32", M4_RANGE(1.0, 2.0), "1.0", M4_DIGIT(2))
    float yuvnrGainScale0;
    // M4_NUMBER_DESC("yuvnrGainScale1", "f32", M4_RANGE(1.0, 2.0), "1.0", M4_DIGIT(2))
    float yuvnrGainScale1;
    // M4_NUMBER_DESC("yuvnrGainScale2", "f32", M4_RANGE(1.0, 2.0), "1.0", M4_DIGIT(2))
    float yuvnrGainScale2;
    // M4_NUMBER_DESC("frame_limit_y", "f32", M4_RANGE(1.0, 255.0), "32.0", M4_DIGIT(2))
    float frame_limit_y;
    // M4_NUMBER_DESC("frame_limit_uv", "f32", M4_RANGE(1.0, 255.0), "32.0", M4_DIGIT(2))
    float frame_limit_uv;
} CalibDbV2_MFNR_Motion_ISO_t;

typedef struct CalibDbV2_MFNR_Motion_s {
    // M4_STRUCT_LIST_DESC("Motion_ISO", M4_SIZE_DYNAMIC, "normal_ui_style")
    CalibDbV2_MFNR_Motion_ISO_t *Motion_ISO;
    int Motion_ISO_len;
} CalibDbV2_MFNR_Motion_t;


typedef struct CalibDbV2_MFNR_Tuning_s {
    // M4_BOOL_DESC("enable", "1")
    bool enable;
    // M4_BOOL_DESC("mode_3to1", "0")
    bool mode_3to1;
    // M4_BOOL_DESC("local_gain_en", "1")
    bool local_gain_en;
    // M4_BOOL_DESC("motion_detect_en", "0")
    bool motion_detect_en;

    // M4_STRUCT_DESC("Dynamic", "normal_ui_style")
    CalibDbV2_MFNR_Dynamic_t Dynamic;
    // M4_STRUCT_DESC("Motion", "normal_ui_style")
    CalibDbV2_MFNR_Motion_t Motion;
    // M4_STRUCT_LIST_DESC("Setting", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_MFNR_T_Set_t *Setting;
    int Setting_len;
} CalibDbV2_MFNR_Tuning_t;


typedef struct CalibDbV2_MFNR_s {
    // M4_STRING_DESC("Version", M4_SIZE(1,1), M4_RANGE(0, 64), "V1", M4_DYNAMIC(0))
    char *Version;
    // M4_STRUCT_DESC("CalibPara", "normal_ui_style")
    CalibDbV2_MFNR_Calib_t CalibPara;
    // M4_STRUCT_DESC("TuningPara", "normal_ui_style")
    CalibDbV2_MFNR_Tuning_t TuningPara;
} CalibDbV2_MFNR_t;

RKAIQ_END_DECLARE

#endif

