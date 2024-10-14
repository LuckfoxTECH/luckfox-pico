/*
 * debayer_head.h
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

#ifndef __CALIBDBV2_DEBAYER_HEAD_H__
#define __CALIBDBV2_DEBAYER_HEAD_H__

#include <rk_aiq_comm.h>

#define DEBAYER_ISO_STEP_MAX 13

RKAIQ_BEGIN_DECLARE

typedef struct __debayer_array {
    // M4_ARRAY_DESC("debayer_filter1", "u32", M4_SIZE(1,9),  M4_RANGE(08, 65536), "[50,100,200,400,800,1600,3200,6400,12800]", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    int ISO[9];
    // M4_ARRAY_DESC("sharp_strength", "u8", M4_SIZE(1,9),  M4_RANGE(0, 255), "[4,4,4,4,4,4,4,4,4]", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    unsigned char sharp_strength[9];
    // M4_ARRAY_DESC("debayer_hf_offset", "u16", M4_SIZE(1,9),  M4_RANGE(0, 255), "[1,1,1,1,1,1,1,1,1]", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    unsigned short debayer_hf_offset[9];
} Debayer_Array_t;

typedef struct __Debayer_Param {
    // M4_BOOL_DESC("debayer_en", "1", 0, M4_HIDE(1))
    bool debayer_en;
    // M4_ARRAY_DESC("debayer_filter1", "s8", M4_SIZE(1,5),  M4_RANGE(-128, 128), "[2,-6,0,6,-2]", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    int debayer_filter1[5];
    // M4_ARRAY_DESC("debayer_filter2", "s8", M4_SIZE(1,5),  M4_RANGE(-128, 128), "[2,-4,4,-4,2]", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    int debayer_filter2[5];
    // M4_NUMBER_DESC("debayer_gain_offset", "u8", M4_RANGE(0,128), "4", M4_DIGIT(0), 0)
    unsigned char debayer_gain_offset;
    // M4_NUMBER_DESC("debayer_offset", "u8", M4_RANGE(0,128), "1", M4_DIGIT(0), 0)
    unsigned char debayer_offset;
    // M4_BOOL_DESC("debayer_clip_en", "1", 0)
    bool debayer_clip_en;
    // M4_BOOL_DESC("debayer_filter_g_en", "1", 0)
    bool debayer_filter_g_en;
    // M4_BOOL_DESC("debayer_filter_c_en", "1", 0)
    bool debayer_filter_c_en;
    // M4_NUMBER_DESC("debayer_thed0", "u8", M4_RANGE(0,128), "3", M4_DIGIT(0), 0)
    unsigned char debayer_thed0;
    // M4_NUMBER_DESC("debayer_thed1", "u8", M4_RANGE(0,128), "6", M4_DIGIT(0), 0)
    unsigned char debayer_thed1;
    // M4_NUMBER_DESC("debayer_dist_scale", "u8", M4_RANGE(0,128), "8", M4_DIGIT(0), 0)
    unsigned char debayer_dist_scale;
    // M4_NUMBER_DESC("debayer_cnr_strength", "u8", M4_RANGE(0,128), "5", M4_DIGIT(0), 0)
    unsigned char debayer_cnr_strength;
    // M4_NUMBER_DESC("debayer_shift_num", "u8", M4_RANGE(0,128), "2", M4_DIGIT(0), 0)
    unsigned char debayer_shift_num;
    // M4_ARRAY_TABLE_DESC("ISO_Params", "array_table_ui", "default")
    Debayer_Array_t array;
} Debayer_Param_t;

typedef struct __debayer {
    // M4_STRUCT_DESC("DebayerTuningPara", "normal_ui_style")
    Debayer_Param_t param;
} CalibDbV2_Debayer_t;

// TODO: add debayer v2 params

typedef struct CalibDbV2_Debayer_GInterp_s {

    // M4_ARRAY_DESC("iso", "f32", M4_SIZE(1,13), M4_RANGE(50, 204800), "50", M4_DIGIT(1), M4_DYNAMIC(0))
    float iso[DEBAYER_ISO_STEP_MAX];

    // M4_ARRAY_DESC("debayer_clip_en", "u8", M4_SIZE(1,13),M4_RANGE(0, 1),"1", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char debayer_clip_en[DEBAYER_ISO_STEP_MAX];

    // M4_ARRAY_DESC("debayer_gain_offset", "u16", M4_SIZE(1,13),M4_RANGE(0,4095), "4", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned short debayer_gain_offset[DEBAYER_ISO_STEP_MAX];

    // M4_ARRAY_DESC("debayer_max_ratio", "u8", M4_SIZE(1,13),M4_RANGE(0,63), "4", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char debayer_max_ratio[DEBAYER_ISO_STEP_MAX];

} CalibDbV2_Debayer_GInterp_t;

typedef struct CalibDbV2_Debayer_GDirectWgt_s {

    // M4_ARRAY_DESC("iso", "f32", M4_SIZE(1,13), M4_RANGE(50, 204800), "50", M4_DIGIT(1), M4_DYNAMIC(0))
    float iso[DEBAYER_ISO_STEP_MAX];

    // M4_ARRAY_DESC("debayer_hf_offset", "u16",M4_SIZE(1,13), M4_RANGE(0, 65535), "1", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned short debayer_hf_offset[DEBAYER_ISO_STEP_MAX];

    // M4_ARRAY_DESC("debayer_thed0", "u8",M4_SIZE(1,13), M4_RANGE(0,15), "3", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char debayer_thed0[DEBAYER_ISO_STEP_MAX];

    // M4_ARRAY_DESC("debayer_thed1", "u8",M4_SIZE(1,13), M4_RANGE(0,15), "6", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char debayer_thed1[DEBAYER_ISO_STEP_MAX];

    // M4_ARRAY_DESC("debayer_dist_scale", "u8",M4_SIZE(1,13), M4_RANGE(0,15), "8", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char debayer_dist_scale[DEBAYER_ISO_STEP_MAX];

    // M4_ARRAY_DESC("debayer_select_thed", "u8",M4_SIZE(1,13), M4_RANGE(0,255), "13", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char debayer_select_thed[DEBAYER_ISO_STEP_MAX];

} CalibDbV2_Debayer_GDirectWgt_t;

typedef struct CalibDbV2_Debayer_GFilter_s {
    // M4_ARRAY_DESC("iso", "f32", M4_SIZE(1,13), M4_RANGE(50, 204800), "50", M4_DIGIT(1), M4_DYNAMIC(0))
    float iso[DEBAYER_ISO_STEP_MAX];

    // M4_ARRAY_DESC("debayer_gfilter_en", "u8",M4_SIZE(1,13), M4_RANGE(0, 1), "1", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char debayer_gfilter_en[DEBAYER_ISO_STEP_MAX];

    // M4_ARRAY_DESC("debayer_gfilter_offset", "u16",M4_SIZE(1,13),  M4_RANGE(0,2047), "1", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned short debayer_gfilter_offset[DEBAYER_ISO_STEP_MAX];

} CalibDbV2_Debayer_GFilter_t;

typedef struct CalibDbV2_Debayer_CFilter_s {

    // M4_ARRAY_DESC("iso", "f32", M4_SIZE(1,13), M4_RANGE(50, 204800), "50", M4_DIGIT(1), M4_DYNAMIC(0))
    float iso[DEBAYER_ISO_STEP_MAX];

    // M4_ARRAY_DESC("debayer_cfilter_en", "u8",M4_SIZE(1,13),M4_RANGE(0, 1), "1", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char debayer_cfilter_en[DEBAYER_ISO_STEP_MAX];

    // M4_ARRAY_DESC("debayer_loggd_offset", "u16",M4_SIZE(1,13), M4_RANGE(0,4095), "1", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned short debayer_loggd_offset[DEBAYER_ISO_STEP_MAX];

    /* C_FILTER_IIR */

    // M4_ARRAY_DESC("debayer_cfilter_str", "f32",M4_SIZE(1,13), M4_RANGE(0,1), "0.0313", M4_DIGIT(4), M4_DYNAMIC(0))
    float  debayer_cfilter_str[DEBAYER_ISO_STEP_MAX];

    // M4_ARRAY_DESC("debayer_wet_clip", "f32",M4_SIZE(1,13), M4_RANGE(0,15.875), "7.25", M4_DIGIT(2), M4_DYNAMIC(0))
    float debayer_wet_clip[DEBAYER_ISO_STEP_MAX];

    // M4_ARRAY_DESC("debayer_wet_ghost", "f32",M4_SIZE(1,13), M4_RANGE(0,0.98), "0.03", M4_DIGIT(2), M4_DYNAMIC(0))
    float debayer_wet_ghost[DEBAYER_ISO_STEP_MAX];

    // M4_ARRAY_DESC("debayer_wgtslope", "f32",M4_SIZE(1,13), M4_RANGE(0,31.992), "0.7", M4_DIGIT(3), M4_DYNAMIC(0))
    float debayer_wgtslope[DEBAYER_ISO_STEP_MAX];

    /* C_FILTER_BF */

    // M4_ARRAY_DESC("debayer_bf_sgm", "f32",M4_SIZE(1,13), M4_RANGE(0,1), "0.0266", M4_DIGIT(4), M4_DYNAMIC(0))
    float debayer_bf_sgm[DEBAYER_ISO_STEP_MAX];

    // M4_ARRAY_DESC("debayer_bf_clip", "u8",M4_SIZE(1,13), M4_RANGE(0,127), "1", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char  debayer_bf_clip[DEBAYER_ISO_STEP_MAX];

    // M4_ARRAY_DESC("debayer_bf_curwgt", "u8",M4_SIZE(1,13), M4_RANGE(0,127), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char  debayer_bf_curwgt[DEBAYER_ISO_STEP_MAX];

    // M4_ARRAY_DESC("debayer_loghf_offset", "u16",M4_SIZE(1,13), M4_RANGE(0,8191), "128", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned short debayer_loghf_offset[DEBAYER_ISO_STEP_MAX];

    /* C_FILTER_ALPHA */

    // M4_ARRAY_DESC("debayer_alpha_offset", "u16",M4_SIZE(1,13), M4_RANGE(0,4095), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned short debayer_alpha_offset[DEBAYER_ISO_STEP_MAX];

    // M4_ARRAY_DESC("debayer_alpha_scale", "f32",M4_SIZE(1,13), M4_RANGE(0,1023.999), "1.0", M4_DIGIT(3), M4_DYNAMIC(0))
    float debayer_alpha_scale[DEBAYER_ISO_STEP_MAX];

    /* C_FILTER_EDGE */
    // M4_ARRAY_DESC("debayer_edge_offset", "u16",M4_SIZE(1,13), M4_RANGE(0,4095), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned short debayer_edge_offset[DEBAYER_ISO_STEP_MAX];

    // M4_ARRAY_DESC("debayer_edge_scale", "f32",M4_SIZE(1,13), M4_RANGE(0,1023.999), "1.0", M4_DIGIT(3), M4_DYNAMIC(0))
    float debayer_edge_scale[DEBAYER_ISO_STEP_MAX];
} CalibDbV2_Debayer_CFilter_t;


typedef struct CalibDbV2_Debayer_Tuning_s {
    // M4_BOOL_DESC("debayer_en", "1")
    bool debayer_en;

    // M4_ARRAY_DESC("g_lowfreq_filter1", "s8", M4_SIZE(1,4),  M4_RANGE(-16, 15), "[-2,2,-4,4]", M4_DIGIT(0), M4_DYNAMIC(0))
    int lowfreq_filter1[4];

    // M4_ARRAY_DESC("g_highfreq_filter2", "s8", M4_SIZE(1,4),  M4_RANGE(-16, 15), "[-1,2,-2,4]", M4_DIGIT(0), M4_DYNAMIC(0))
    int  highfreq_filter2[4];

    // M4_ARRAY_DESC("c_alpha_gaus_coe", "s8", M4_SIZE(1,3),  M4_RANGE(0, 255), "[64,32,16]", M4_DIGIT(0), M4_DYNAMIC(0))
    int  c_alpha_gaus_coe[3];

    // M4_ARRAY_DESC("c_guid_gaus_coe", "s8", M4_SIZE(1,3),  M4_RANGE(0, 255), "[64,32,16]", M4_DIGIT(0), M4_DYNAMIC(0))
    int  c_guid_gaus_coe[3];

    // M4_ARRAY_DESC("c_ce_gaus_coe", "s8", M4_SIZE(1,3),  M4_RANGE(0, 255), "[64,32,16]", M4_DIGIT(0), M4_DYNAMIC(0))
    int  c_ce_gaus_coe[3];

    // M4_ARRAY_TABLE_DESC("g_interp", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDbV2_Debayer_GInterp_t g_interp;

    // M4_ARRAY_TABLE_DESC("g_drctwgt", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDbV2_Debayer_GDirectWgt_t g_drctwgt;

    // M4_ARRAY_TABLE_DESC("g_fliter", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDbV2_Debayer_GFilter_t g_filter;

    // M4_ARRAY_TABLE_DESC("c_fliter", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDbV2_Debayer_CFilter_t c_filter;

} CalibDbV2_Debayer_Tuning_t;



typedef struct CalibDbV2_Debayer_v2_s {
    // M4_STRUCT_DESC("TuningPara", "normal_ui_style")
    CalibDbV2_Debayer_Tuning_t param;
} CalibDbV2_Debayer_v2_t;


RKAIQ_END_DECLARE

#endif
