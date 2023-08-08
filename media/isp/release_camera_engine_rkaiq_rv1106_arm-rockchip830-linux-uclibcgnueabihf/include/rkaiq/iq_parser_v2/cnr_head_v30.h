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

#ifndef __CALIBDBV2_CNRV30_HEADER_H__
#define __CALIBDBV2_CNRV30_HEADER_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

///////////////////////////uvnr//////////////////////////////////////
typedef struct CalibDbV2_CNRV30_T_ISO_s {
    // M4_NUMBER_MARK_DESC("iso", "f32", M4_RANGE(50, 204800), "50", M4_DIGIT(1), "index2")
    float iso;

    ///////////new/////////////
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

} CalibDbV2_CNRV30_T_ISO_t;

typedef struct CalibDbV2_CNRV30_T_Set_s {
    // M4_STRING_MARK_DESC("SNR_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "LSNR",M4_DYNAMIC(0), "index1")
    char *SNR_Mode;
    // M4_STRING_DESC("Sensor_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "lcg", M4_DYNAMIC(0))
    char *Sensor_Mode;
    // M4_STRUCT_LIST_DESC("Tuning_ISO", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_CNRV30_T_ISO_t *Tuning_ISO;
    int Tuning_ISO_len;

} CalibDbV2_CNRV30_T_Set_t;


typedef struct CalibDbV2_CNRV30_Tuning_s {
    // M4_BOOL_DESC("enable", "1")
    bool enable;
    // M4_STRUCT_LIST_DESC("Setting", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_CNRV30_T_Set_t *Setting;
    int Setting_len;
} CalibDbV2_CNRV30_Tuning_t;


typedef struct CalibDbV2_CNRV30_s {
    // M4_STRING_DESC("Version", M4_SIZE(1,1), M4_RANGE(0, 64), "V1", M4_DYNAMIC(0))
    char *Version;
    // M4_STRUCT_DESC("TuningPara", "normal_ui_style")
    CalibDbV2_CNRV30_Tuning_t TuningPara;
} CalibDbV2_CNRV30_t;

RKAIQ_END_DECLARE

#endif
