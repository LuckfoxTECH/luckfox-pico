/*
 * bayertnr_uapi_head_v2.h
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

#ifndef __CALIBDBV2_UAPI_BAYER_TNR_HEADER_V2_H__
#define __CALIBDBV2_UAPI_BAYER_TNR_HEADER_V2_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE


typedef struct RK_Bayertnr_Params_V2_Select_s
{
    // M4_BOOL_DESC("enable", "1")
    int enable;
    // M4_BOOL_DESC("lo_enable", "1")
    int lo_enable;
    // M4_BOOL_DESC("hi_enable", "1")
    int hi_enable;
    // M4_BOOL_DESC("lo_med_en", "1")
    int lo_med_en;
    // M4_BOOL_DESC("lo_gsbay_en", "1")
    int lo_gsbay_en;
    // M4_BOOL_DESC("lo_gslum_en", "1")
    int lo_gslum_en;
    // M4_BOOL_DESC("hi_med_en", "1")
    int hi_med_en;
    // M4_BOOL_DESC("hi_gslum_en", "1")
    int hi_gslum_en;


    // M4_ARRAY_DESC("lumapoint", "s32", M4_SIZE(1,16), M4_RANGE(0,65535), "[512,1024,1536,2048,3072,4096,5120,6144,7168,8192,9216,10240,11264,12288,13312,14336]", M4_DIGIT(0), M4_DYNAMIC(0))
    int lumapoint[16];
    // M4_ARRAY_DESC("sigma", "s32", M4_SIZE(1,16), M4_RANGE(0,65535), "[256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256]", M4_DIGIT(0), M4_DYNAMIC(0))
    int sigma[16];

    // M4_ARRAY_DESC("lumapoint2", "s32", M4_SIZE(1,16), M4_RANGE(0,65535), "[512,1024,1536,2048,3072,4096,5120,6144,7168,8192,9216,10240,11264,12288,13312,14336]", M4_DIGIT(0), M4_DYNAMIC(0))
    int lumapoint2[16];
    // M4_ARRAY_DESC("lo_sigma", "s32", M4_SIZE(1,16), M4_RANGE(0,65535), "[32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32]", M4_DIGIT(0), M4_DYNAMIC(0))
    int lo_sigma[16];
    // M4_ARRAY_DESC("hi_sigma", "s32", M4_SIZE(1,16), M4_RANGE(0,65535), "[256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256]", M4_DIGIT(0), M4_DYNAMIC(0))
    int hi_sigma[16];

    // M4_NUMBER_DESC("thumbds", "f32", M4_RANGE(0, 8), "8", M4_DIGIT(2))
    int thumbds;

    // M4_BOOL_DESC("global_pk_en", "1")
    int global_pk_en;
    // M4_NUMBER_DESC("global_pksq", "s32", M4_RANGE(0, 268435455), "1024", M4_DIGIT(0))
    int global_pksq;

    // M4_NUMBER_DESC("lo_filter_strength", "f32", M4_RANGE(0, 16), "1.0", M4_DIGIT(2))
    float lo_filter_strength;
    // M4_NUMBER_DESC("hi_filter_strength", "f32", M4_RANGE(0, 16), "1.0", M4_DIGIT(2))
    float hi_filter_strength;
    // M4_NUMBER_DESC("soft_threshold_ratio", "f32", M4_RANGE(0, 1.0), "0.0", M4_DIGIT(2))
    float soft_threshold_ratio;

    // M4_NUMBER_DESC("clipwgt", "f32", M4_RANGE(0, 1), "0.03215", M4_DIGIT(5))
    float clipwgt;
    // M4_NUMBER_DESC("hi_wgt_comp", "f32", M4_RANGE(0, 1), "0.16", M4_DIGIT(2))
    float hi_wgt_comp;
    // M4_NUMBER_DESC("hidif_th", "f32", M4_RANGE(0, 65535), "32767", M4_DIGIT(0))
    float hidif_th;
} RK_Bayertnr_Params_V2_Select_t;


typedef struct Abayertnr_ExpInfo_V2_s {
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


} Abayertnr_ExpInfo_V2_t;


typedef struct rk_aiq_bayertnr_info_v2_s {
    // M4_ARRAY_TABLE_DESC("sync", "array_table_ui", "none", "1")
    rk_aiq_uapi_sync_t sync;
    // M4_NUMBER_DESC("iso", "u32", M4_RANGE(0, 204800), "50", M4_DIGIT(0), "0", "0")
    int iso;
    // M4_ARRAY_TABLE_DESC("expo_info", "normal_ui_style", "none", "0", "0")
    Abayertnr_ExpInfo_V2_t expo_info;
} rk_aiq_bayertnr_info_v2_t;


RKAIQ_END_DECLARE

#endif
