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
#ifndef _CALIBDBV2_SHARP_UAPI_HEAD_V33_H_
#define _CALIBDBV2_SHARP_UAPI_HEAD_V33_H_

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE


#define RK_SHARP_V33_LUMA_POINT_NUM (8)

#define RK_SHARP_V33_PBF_DIAM                 3
#define RK_SHARP_V33_RF_DIAM                  5
#define RK_SHARP_V33_BF_DIAM                  3
#define RK_SHARP_V33_AVG_DIAM                 3
#define RK_SHARP_V33_SHARP_ADJ_GAIN_TABLE_LEN 14
#define RK_SHARP_V33_STRENGTH_TABLE_LEN       22
///////////////////////////sharp//////////////////////////////////////
// clang-format off


typedef struct RK_SHARP_Params_V33_Select_s {
    // M4_BOOL_DESC("enable", "1")
    bool enable;
    // M4_BOOL_DESC("kernel_sigma_enable", "0")
    bool kernel_sigma_enable;
    // M4_BOOL_DESC("Center_Mode", "0")
    bool Center_Mode;
    // M4_BOOL_DESC("exgain_bypass", "0")
    bool exgain_bypass;

    // M4_ARRAY_DESC("luma_point", "u16", M4_SIZE(1,8), M4_RANGE(0,1024), "[0, 64, 128, 256, 384, 640, 896, 1024]", M4_DIGIT(1), M4_DYNAMIC(0))
    uint16_t luma_point[8];
    // M4_ARRAY_DESC("luma_sigma", "u16", M4_SIZE(1,8), M4_RANGE(0,1023), "[8, 12, 16, 16, 24, 20, 16, 16]", M4_DIGIT(0), M4_DYNAMIC(0))
    uint16_t luma_sigma[8];
    // M4_ARRAY_DESC("hf_clip", "u16", M4_SIZE(1,8), M4_RANGE(0,1023), "[1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023]", M4_DIGIT(0), M4_DYNAMIC(0))
    uint16_t hf_clip[8];


    // M4_NUMBER_DESC("pbf_gain", "f32", M4_RANGE(0.0, 4.0), "0.5", M4_DIGIT(2))
    float pbf_gain;
    // M4_NUMBER_DESC("pbf_ratio", "f32", M4_RANGE(0.0, 1.0), "0.5", M4_DIGIT(2))
    float pbf_ratio;
    // M4_NUMBER_DESC("pbf_add", "f32", M4_RANGE(0.0, 1023.0), "0.0", M4_DIGIT(2))
    float pbf_add;
    // M4_NUMBER_DESC("gaus_ratio", "f32", M4_RANGE(0.0, 1.0), "0.0", M4_DIGIT(2))
    float gaus_ratio;
    // M4_NUMBER_DESC("sharp_ratio", "f32", M4_RANGE(0.0, 31.275), "0.5", M4_DIGIT(2))
    float sharp_ratio;
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



    // M4_NUMBER_MARK_DESC("prefilter_sigma", "f32", M4_RANGE(0.0, 100.0), "1.0", M4_DIGIT(2))
    float prefilter_sigma;
    // M4_NUMBER_MARK_DESC("hfBilateralFilter_sigma", "f32", M4_RANGE(0.0, 100.0), "1.0", M4_DIGIT(2))
    float hfBilateralFilter_sigma;
    // M4_NUMBER_MARK_DESC("GaussianFilter_sigma", "f32", M4_RANGE(0.0, 100.0), "1.0", M4_DIGIT(2))
    float GaussianFilter_sigma;
    // M4_NUMBER_MARK_DESC("GaussianFilter_radius", "u8", M4_RANGE(1, 2), "2", M4_DIGIT(0))
    uint8_t GaussianFilter_radius;


    // M4_ARRAY_DESC("prefilter_coeff", "f32", M4_SIZE(1,3), M4_RANGE(0,1), "[0.2042,0.1238,0.0751]", M4_DIGIT(4), M4_DYNAMIC(0))
    float prefilter_coeff[3];
    // M4_ARRAY_DESC("GaussianFilter_coeff", "f32", M4_SIZE(1,6), M4_RANGE(0.0,1.0), "[0.2042,0.1238,0.0751]", M4_DIGIT(4), M4_DYNAMIC(0))
    float GaussianFilter_coeff[6];
    // M4_ARRAY_DESC("hfBilateralFilter_coeff", "f32", M4_SIZE(1,3), M4_RANGE(0.0,1.0), "[0.2042,0.1238,0.0751]", M4_DIGIT(4), M4_DYNAMIC(0))
    float hfBilateralFilter_coeff[3];

} RK_SHARP_Params_V33_Select_t;


typedef struct Asharp_ExpInfo_V33_s {
    // M4_NUMBER_DESC("hdr_mode", "u8", M4_RANGE(0, 2), "0", M4_DIGIT(0))
    int hdr_mode;
    // M4_NUMBER_DESC("snr_mode", "s8", M4_RANGE(0, 2), "0", M4_DIGIT(0))
    int   snr_mode;

    // M4_ARRAY_DESC("time", "f32", M4_SIZE(1,3), M4_RANGE(0, 1024), "0.01", M4_DIGIT(6))
    float arTime[3];
    // M4_ARRAY_DESC("again", "f32", M4_SIZE(1,3), M4_RANGE(0, 204800), "1", M4_DIGIT(3))
    float arAGain[3];
    // M4_ARRAY_DESC("dgain", "f32", M4_SIZE(1,3), M4_RANGE(0, 204800), "1", M4_DIGIT(3))
    float arDGain[3];
    // M4_NUMBER_DESC("blc_ob_predgain", "f32", M4_RANGE(0, 204800), "1", M4_DIGIT(3))
    float blc_ob_predgain;
    // M4_ARRAY_DESC("iso", "u32", M4_SIZE(1,3), M4_RANGE(0, 204800), "1", M4_DIGIT(0))
    int   arIso[3];

    // M4_NUMBER_DESC("isoLevelLow", "u8", M4_RANGE(0, 12), "0", M4_DIGIT(0))
    int isoLevelLow;
    // M4_NUMBER_DESC("isoLevelHig", "u8", M4_RANGE(0, 12), "0", M4_DIGIT(0))
    int isoLevelHig;

    // M4_NUMBER_DESC("rawWidth", "s32", M4_RANGE(0, 65535), "0", M4_DIGIT(0))
    int rawWidth;
    // M4_NUMBER_DESC("rawHeight", "s32", M4_RANGE(0, 65535), "0", M4_DIGIT(0))
    int rawHeight;

} Asharp_ExpInfo_V33_t;


typedef struct rk_aiq_sharp_info_v33_s {
    // M4_ARRAY_TABLE_DESC("sync", "array_table_ui", "none", "1")
    rk_aiq_uapi_sync_t sync;
    // M4_NUMBER_DESC("iso", "u32", M4_RANGE(0, 204800), "50", M4_DIGIT(0), "0", "0")
    int iso;
    // M4_ARRAY_TABLE_DESC("expo_info", "normal_ui_style", "none", "0", "0")
    Asharp_ExpInfo_V33_t expo_info;
} rk_aiq_sharp_info_v33_t;





// clang-format off
RKAIQ_END_DECLARE

#endif
