/*
 * sharp_uapi_head_v4.h
 *
 *  Copyright (c) 2022 Rockchip Corporation
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

#ifndef __CALIBDBV2_UAPI_SHARPV4_HEADER_H__
#define __CALIBDBV2_UAPI_SHARPV4_HEADER_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

#define RK_SHARP_V4_LUMA_POINT_NUM             8
#define RK_SHARP_V4_PBF_DIAM                    3
#define RK_SHARP_V4_RF_DIAM                     5
#define RK_SHARP_V4_BF_DIAM                     3


typedef struct RK_SHARP_Params_V4_Select_s
{
    // M4_BOOL_DESC("enable", "1")
    int enable;



    // M4_ARRAY_DESC("luma_point", "s16", M4_SIZE(1,8), M4_RANGE(0,1024), "[0, 64, 128, 256, 384, 640, 896, 1024]", M4_DIGIT(0), M4_DYNAMIC(0))
    int16_t luma_point[RK_SHARP_V4_LUMA_POINT_NUM];
    // M4_ARRAY_DESC("luma_sigma", "s16", M4_SIZE(1,8), M4_RANGE(0,1023), "[8, 10, 10, 12, 14, 12, 12, 10]", M4_DIGIT(0), M4_DYNAMIC(0))
    int16_t luma_sigma[RK_SHARP_V4_LUMA_POINT_NUM];


    // M4_NUMBER_DESC("pbf_gain", "f32", M4_RANGE(0.0, 2.0), "0.5", M4_DIGIT(2))
    float pbf_gain;

    // M4_NUMBER_DESC("pbf_add", "f32", M4_RANGE(0.0, 1023.0), "0.0", M4_DIGIT(2))
    float pbf_add;

    // M4_NUMBER_DESC("pbf_ratio", "f32", M4_RANGE(0.0, 1.0), "0.5", M4_DIGIT(2))
    float pbf_ratio;

    // M4_NUMBER_DESC("gaus_ratio", "f32", M4_RANGE(0.0, 1.0), "0.0", M4_DIGIT(2))
    float gaus_ratio;

    // M4_NUMBER_DESC("sharp_ratio", "f32", M4_RANGE(0.0, 32.0), "0.5", M4_DIGIT(2))
    float sharp_ratio;



    // M4_NUMBER_DESC("bf_gain", "f32", M4_RANGE(0.0, 2.0), "0.5", M4_DIGIT(2))
    float bf_gain;
    // M4_NUMBER_DESC("bf_add", "f32", M4_RANGE(0.0, 1023.0), "0.0", M4_DIGIT(2))
    float bf_add;
    // M4_NUMBER_DESC("bf_ratio", "f32", M4_RANGE(0.0, 1.0), "0.5", M4_DIGIT(2))
    float bf_ratio;


    // M4_ARRAY_DESC("hf_clip", "s16", M4_SIZE(1,8), M4_RANGE(0,1023), "[10, 20, 30, 56, 56, 56, 56, 30]", M4_DIGIT(0), M4_DYNAMIC(0))
    int16_t hf_clip[RK_SHARP_V4_LUMA_POINT_NUM];
    // M4_ARRAY_DESC("local_sharp_strength", "s16", M4_SIZE(1,8), M4_RANGE(0,1023), "[1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023]", M4_DIGIT(0), M4_DYNAMIC(0))
    int16_t local_sharp_strength[RK_SHARP_V4_LUMA_POINT_NUM];



    // M4_BOOL_DESC("kernel_sigma_enable", "1")
    int kernel_sigma_enable;
    // M4_ARRAY_DESC("prefilter_sigma", "f32", M4_SIZE(1,1), M4_RANGE(0,100), "[1.0]", M4_DIGIT(2), M4_DYNAMIC(0))
    float prefilter_sigma;
    // M4_ARRAY_DESC("hfBilateralFilter_sigma", "f32", M4_SIZE(1,1), M4_RANGE(0,100), "[1.0]", M4_DIGIT(2), M4_DYNAMIC(0))
    float hfBilateralFilter_sigma;
    // M4_ARRAY_DESC("GaussianFilter_sigma", "f32", M4_SIZE(1,1), M4_RANGE(0,100), "[1.0]", M4_DIGIT(2), M4_DYNAMIC(0))
    float GaussianFilter_sigma;
    // M4_ARRAY_DESC("GaussianFilter_radius", "f32", M4_SIZE(1,1), M4_RANGE(1,2), "[2.0]", M4_DIGIT(0), M4_DYNAMIC(0))
    float GaussianFilter_radius;


    // M4_ARRAY_DESC("prefilter_coeff", "f32", M4_SIZE(1,3), M4_RANGE(0,1), "[0.2042,0.1238,0.0751]", M4_DIGIT(4), M4_DYNAMIC(0))
    float prefilter_coeff[3];
    // M4_ARRAY_DESC("GaussianFilter_coeff", "f32", M4_SIZE(1,6), M4_RANGE(0,1), "[0.2042,0.1238,0.0751]", M4_DIGIT(4), M4_DYNAMIC(0))
    float GaussianFilter_coeff[6];
    // M4_ARRAY_DESC("hfBilateralFilter_coeff", "f32", M4_SIZE(1,3), M4_RANGE(0,1), "[0.2042,0.1238,0.0751]", M4_DIGIT(4), M4_DYNAMIC(0))
    float hfBilateralFilter_coeff[3];





} RK_SHARP_Params_V4_Select_t;



typedef struct Asharp4_ExpInfo_s {

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
    // M4_ARRAY_DESC("iso", "u32", M4_SIZE(1,3), M4_RANGE(0, 204800), "1", M4_DIGIT(0))
    int   arIso[3];

    // M4_NUMBER_DESC("isoLow", "u32", M4_RANGE(0, 204800), "50", M4_DIGIT(0))
    int isoLow;
    // M4_NUMBER_DESC("isoHigh", "u32", M4_RANGE(0, 204800), "50", M4_DIGIT(0))
    int isoHigh;

    // M4_NUMBER_DESC("rawWidth", "s32", M4_RANGE(0, 65535), "0", M4_DIGIT(0))
    int rawWidth;
    // M4_NUMBER_DESC("rawHeight", "s32", M4_RANGE(0, 65535), "0", M4_DIGIT(0))
    int rawHeight;
} Asharp4_ExpInfo_t;

typedef struct rk_aiq_sharp_info_v4_s {
    // M4_ARRAY_TABLE_DESC("sync", "array_table_ui", "none", "1")
    rk_aiq_uapi_sync_t sync;
    // M4_NUMBER_DESC("iso", "u32", M4_RANGE(0, 204800), "50", M4_DIGIT(0), "0", "0")
    int iso;
    // M4_ARRAY_TABLE_DESC("expo_info", "normal_ui_style", "none", "0", "0")
    Asharp4_ExpInfo_t expo_info;
} rk_aiq_sharp_info_v4_t;

RKAIQ_END_DECLARE

#endif

