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

#ifndef __CALIBDBV2_CNRV30_UAPI_HEADER_H__
#define __CALIBDBV2_CNRV30_UAPI_HEADER_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE


typedef struct RK_CNR_Params_V30_Select_s
{
    // M4_BOOL_DESC("enable", "1")
    bool enable;

    // M4_NUMBER_DESC("down_scale_x", "u8", M4_RANGE(2, 8), "8", M4_DIGIT(0))
    uint8_t down_scale_x;
    // M4_NUMBER_DESC("down_scale_y", "u8", M4_RANGE(2, 6), "6", M4_DIGIT(0))
    uint8_t down_scale_y;

    // M4_NUMBER_DESC("thumb_sigma", "f32", M4_RANGE(0.0, 1.0), "0.0039", M4_DIGIT(4))
    float thumb_sigma;
    // M4_NUMBER_DESC("thumb_bf_ratio", "f32", M4_RANGE(0.0, 1.0), "1.0", M4_DIGIT(2))
    float thumb_bf_ratio;

    // M4_NUMBER_DESC("chroma_filter_strength", "f32", M4_RANGE(0.0, 1.0), "0.0078", M4_DIGIT(4))
    float chroma_filter_strength;
    // M4_NUMBER_DESC("chroma_filter_wgt_clip", "f32", M4_RANGE(0.0, 16.0), "1.0", M4_DIGIT(2))
    float chroma_filter_wgt_clip;
    // M4_NUMBER_DESC("anti_chroma_ghost", "f32", M4_RANGE(0.0, 1.0), "0.0313", M4_DIGIT(4))
    float anti_chroma_ghost;
    // M4_NUMBER_DESC("chroma_filter_uv_gain", "f32", M4_RANGE(0.0, 1.0), "0.333", M4_DIGIT(3))
    float chroma_filter_uv_gain;
    // M4_NUMBER_DESC("wgt_slope", "f32", M4_RANGE(0.0, 8.0), "1.0", M4_DIGIT(2))
    float wgt_slope;

    // M4_NUMBER_DESC("gaus_ratio", "f32", M4_RANGE(0.0, 1.0), "1.0", M4_DIGIT(2))
    float gaus_ratio;

    // M4_NUMBER_DESC("bf_sigmaR", "f32", M4_RANGE(0.0, 1.0), "0.0314", M4_DIGIT(4))
    float bf_sigmaR;
    // M4_NUMBER_DESC("bf_uvgain", "f32", M4_RANGE(0.0, 8.0), "3.0", M4_DIGIT(2))
    float bf_uvgain;
    // M4_NUMBER_DESC("bf_ratio", "f32", M4_RANGE(0.0, 1.0), "0.0625", M4_DIGIT(4))
    float bf_ratio;
    // M4_NUMBER_DESC("hbf_wgt_clip", "f32", M4_RANGE(0.0, 1.0), "0.0078", M4_DIGIT(4))
    float hbf_wgt_clip;
    // M4_BOOL_DESC("bf_wgt0_sel", "0")
    bool bf_wgt0_sel;
    // M4_NUMBER_DESC("global_alpha", "f32", M4_RANGE(0.0, 1.0), "1.0", M4_DIGIT(2))
    float global_alpha;

    // M4_NUMBER_DESC("saturation_adj_offset", "f32", M4_RANGE(0.0, 511.0), "0", M4_DIGIT(4))
    float saturation_adj_offset;
    // M4_NUMBER_DESC("saturation_adj_ratio", "f32", M4_RANGE(0.0, 32.0), "0.0", M4_DIGIT(2))
    float saturation_adj_ratio;

    // M4_NUMBER_DESC("global_gain", "f32", M4_RANGE(0.0, 64.0), "1.0", M4_DIGIT(2))
    float global_gain;
    // M4_NUMBER_DESC("global_gain_alpha", "f32", M4_RANGE(0.0, 1.0), "0.0", M4_DIGIT(2))
    float global_gain_alpha;
    // M4_NUMBER_DESC("local_gain_scale", "f32", M4_RANGE(0.0625, 1.0), "1.0", M4_DIGIT(2))
    float local_gain_scale;
    // M4_NUMBER_DESC("global_gain_thumb", "f32", M4_RANGE(0.0, 4.0), "1.0", M4_DIGIT(2))
    float global_gain_thumb;
    // M4_NUMBER_DESC("global_gain_alpha_thumb", "f32", M4_RANGE(8.0, 8.0), "8.0", M4_DIGIT(0))
    float global_gain_alpha_thumb;

    // M4_ARRAY_DESC("gain_adj_strength_ratio", "f32", M4_SIZE(1,13), M4_RANGE(0.0,4.0), "1.0", M4_DIGIT(2), M4_DYNAMIC(0))
    float gain_adj_strength_ratio[13];

    // M4_ARRAY_DESC("thumb_filter_wgt_coeff", "f32", M4_SIZE(1,4), M4_RANGE(0.0,1.0), "1.0,1.0,1.0,1.0", M4_DIGIT(2), M4_DYNAMIC(0))
    float thumb_filter_wgt_coeff[4];
    // M4_ARRAY_DESC("gaus_coeff", "f32", M4_SIZE(1,6), M4_RANGE(0,127), "36,24,16,6,4,1", M4_DIGIT(0), M4_DYNAMIC(0))
    float gaus_coeff[6];

} RK_CNR_Params_V30_Select_t;

typedef struct AcnrV30_ExpInfo_s {
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
} AcnrV30_ExpInfo_t;

typedef struct rk_aiq_cnr_info_v30_s {
    // M4_ARRAY_TABLE_DESC("sync", "array_table_ui", "none", "1")
    rk_aiq_uapi_sync_t sync;
    // M4_NUMBER_DESC("iso", "u32", M4_RANGE(0, 204800), "50", M4_DIGIT(0), "0", "0")
    int iso;
    // M4_ARRAY_TABLE_DESC("expo_info", "normal_ui_style", "none", "0", "0")
    AcnrV30_ExpInfo_t expo_info;
} rk_aiq_cnr_info_v30_t;




RKAIQ_END_DECLARE

#endif
