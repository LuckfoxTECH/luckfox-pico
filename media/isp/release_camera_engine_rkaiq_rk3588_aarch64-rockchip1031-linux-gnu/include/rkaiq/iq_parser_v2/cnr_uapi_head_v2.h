/*
 * cnr_uapi_head_v2.h
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

#ifndef __CALIBDBV2_UAPI_CNRV2_HEADER_H__
#define __CALIBDBV2_UAPI_CNRV2_HEADER_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

#define     RKCNR_V2_SGM_ADJ_TABLE_LEN      13

typedef struct RK_CNR_Params_V2_Select_s
{
    // M4_BOOL_DESC("enable", "1")
    int enable;


    // M4_BOOL_DESC("hf_bypass", "0")
    int hf_bypass;
    // M4_BOOL_DESC("lf_bypass", "0")
    int lf_bypass;

    // M4_NUMBER_DESC("global_gain", "f32", M4_RANGE(0.0, 64.0), "1.0", M4_DIGIT(2), M4_HIDE(0))
    float global_gain;
    // M4_NUMBER_DESC("global_gain_alpha", "f32", M4_RANGE(0.0, 1.0), "0.0", M4_DIGIT(2), M4_HIDE(0))
    float global_gain_alpha;
    // M4_NUMBER_DESC("local_gain_scale", "f32", M4_RANGE(0.0, 128.0), "1.0", M4_DIGIT(2), M4_HIDE(0))
    float local_gain_scale;


    // M4_ARRAY_DESC("gain_adj_strength_ratio", "f32", M4_SIZE(1,13), M4_RANGE(0.0, 255.0), "1.0", M4_DIGIT(2), M4_DYNAMIC(0))
    int gain_adj_strength_ratio[RKCNR_V2_SGM_ADJ_TABLE_LEN];

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


    // M4_NUMBER_DESC("lf_denoise_strength", "f32", M4_RANGE(1.0, 1023.0), "4.0", M4_DIGIT(2))
    float lf_denoise_strength;
    // M4_NUMBER_DESC("lf_color_sat", "f32", M4_RANGE(0.0, 8.0), "2.0", M4_DIGIT(2))
    float lf_color_sat;
    // M4_NUMBER_DESC("lf_denoise_alpha", "f32", M4_RANGE(0.0, 1.0), "1.0", M4_DIGIT(2))
    float lf_denoise_alpha;

    // M4_ARRAY_DESC("kernel_5x5", "f32", M4_SIZE(1,5), M4_RANGE(0.0, 1.0), "[1.0000,0.8825,0.7788,0.6065,0.3679]", M4_DIGIT(6), M4_DYNAMIC(0))
    float kernel_5x5[5];

} RK_CNR_Params_V2_Select_t;

typedef struct AcnrV2_ExpInfo_s {

    // M4_NUMBER_DESC("hdr_mode", "u8", M4_RANGE(0, 2), "0", M4_DIGIT(0))
    int hdr_mode;

    // M4_NUMBER_DESC("snr_mode", "s8", M4_RANGE(0, 2), "0", M4_DIGIT(0))
    int snr_mode;

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
} AcnrV2_ExpInfo_t;

typedef struct rk_aiq_cnr_info_v2_s {
    // M4_ARRAY_TABLE_DESC("sync", "array_table_ui", "none", "1")
    rk_aiq_uapi_sync_t sync;
    // M4_NUMBER_DESC("iso", "u32", M4_RANGE(0, 204800), "50", M4_DIGIT(0), "0", "0")
    int iso;
    // M4_ARRAY_TABLE_DESC("expo_info", "normal_ui_style", "none", "0", "0")
    AcnrV2_ExpInfo_t expo_info;
} rk_aiq_cnr_info_v2_t;


RKAIQ_END_DECLARE

#endif
