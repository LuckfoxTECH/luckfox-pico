/*
 * bayer2dnr_uapi_head_v2.h
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

#ifndef __CALIBDBV2_UAPI_BAYER_2DNR_HEADER_V2_H__
#define __CALIBDBV2_UAPI_BAYER_2DNR_HEADER_V2_H__

#include "rk_aiq_comm.h"
//#include "RkAiqCalibDbTypes.h"

#define RK_BAYER2DNR_V2_MAX_ISO_NUM  13

RKAIQ_BEGIN_DECLARE


typedef struct RK_Bayer2dnr_Params_V2_Select_s
{
    // M4_BOOL_DESC("enable", "1")
    int enable;
    // M4_BOOL_DESC("gauss_guide", "1")
    int gauss_guide;


    // M4_ARRAY_DESC("lumapoint", "s32", M4_SIZE(1,16), M4_RANGE(0,65535), "[512,1024,1536,2048,3072,4096,5120,6144,7168,8192,9216,10240,11264,12288,13312,14336]", M4_DIGIT(0), M4_DYNAMIC(0))
    int lumapoint[16];
    // M4_ARRAY_DESC("sigma", "s32", M4_SIZE(1,16), M4_RANGE(0,65535), "[256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256]", M4_DIGIT(0), M4_DYNAMIC(0))
    int sigma[16];

    // M4_NUMBER_DESC("filter_strength", "f32", M4_RANGE(0, 16), "0.5", M4_DIGIT(2))
    float filter_strength;
    // M4_NUMBER_DESC("edgesofts", "f32", M4_RANGE(0, 16.0), "1.0", M4_DIGIT(2))
    float edgesofts;
    // M4_NUMBER_DESC("ratio", "f32", M4_RANGE(0, 1.0), "0.2", M4_DIGIT(2))
    float ratio;
    // M4_NUMBER_DESC("weight", "f32", M4_RANGE(0, 1.0), "1.0", M4_DIGIT(2))
    float weight;

    // M4_NUMBER_DESC("pix_diff", "s32", M4_RANGE(0, 16383), "16383", M4_DIGIT(0))
    int pix_diff;
    // M4_NUMBER_DESC("diff_thld", "s32", M4_RANGE(0, 1024), "1024", M4_DIGIT(0))
    int diff_thld;

    // M4_BOOL_DESC("hdrdgain_ctrl_en", "0")
    bool hdrdgain_ctrl_en;
    // M4_NUMBER_DESC("hdr_dgain_scale_s", "f32", M4_RANGE(0, 128.0), "1.0", M4_DIGIT(2))
    float hdr_dgain_scale_s;
    // M4_NUMBER_DESC("hdr_dgain_scale_m", "f32", M4_RANGE(0, 128.0), "1.0", M4_DIGIT(2))
    float hdr_dgain_scale_m;


} RK_Bayer2dnr_Params_V2_Select_t;

typedef struct RK_Bayer2dnr_Params_V2_s
{
    // M4_BOOL_DESC("enable", "1")
    int enable;

    // M4_ARRAY_DESC("lumapoint", "s32", M4_SIZE(1,16), M4_RANGE(0,65535), "0.0", M4_DIGIT(0), M4_DYNAMIC(0))
    int   lumapoint[16];

    // M4_ARRAY_DESC("sigma", "s32", M4_SIZE(13,16), M4_RANGE(0,65535), "0.0", M4_DIGIT(0), M4_DYNAMIC(0))
    int   sigma[RK_BAYER2DNR_V2_MAX_ISO_NUM][16];

    // v2 parse
    // M4_ARRAY_DESC("iso", "f32", M4_SIZE(1,13), M4_RANGE(0,65535), "0.0", M4_DIGIT(0), M4_DYNAMIC(0))
    float iso[RK_BAYER2DNR_V2_MAX_ISO_NUM];
    // M4_ARRAY_DESC("filter_strength", "f32", M4_SIZE(1,13), M4_RANGE(0,65535), "0.0", M4_DIGIT(2), M4_DYNAMIC(0))
    float filter_strength[RK_BAYER2DNR_V2_MAX_ISO_NUM];
    // M4_ARRAY_DESC("edgesofts", "f32", M4_SIZE(1,13), M4_RANGE(0,65535), "0.0", M4_DIGIT(2), M4_DYNAMIC(0))
    float edgesofts[RK_BAYER2DNR_V2_MAX_ISO_NUM];
    // M4_ARRAY_DESC("ratio", "f32", M4_SIZE(1,13), M4_RANGE(0,65535), "0.0", M4_DIGIT(2), M4_DYNAMIC(0))
    float ratio[RK_BAYER2DNR_V2_MAX_ISO_NUM];
    // M4_ARRAY_DESC("weight", "f32", M4_SIZE(1,13), M4_RANGE(0,65535), "0.0", M4_DIGIT(2), M4_DYNAMIC(0))
    float weight[RK_BAYER2DNR_V2_MAX_ISO_NUM];

    // M4_ARRAY_DESC("gauss_guide", "s32", M4_SIZE(1,13), M4_RANGE(0,65535), "0.0", M4_DIGIT(0), M4_DYNAMIC(0))
    int gauss_guide[RK_BAYER2DNR_V2_MAX_ISO_NUM];

    // M4_ARRAY_DESC("pix_diff", "s32", M4_SIZE(1,13), M4_RANGE(0,65535), "0.0", M4_DIGIT(0), M4_DYNAMIC(0))
    int pix_diff[RK_BAYER2DNR_V2_MAX_ISO_NUM];
    // M4_ARRAY_DESC("diff_thld", "s32", M4_SIZE(1,13), M4_RANGE(0,65535), "0.0", M4_DIGIT(0), M4_DYNAMIC(0))
    int diff_thld[RK_BAYER2DNR_V2_MAX_ISO_NUM];


    // M4_BOOL_DESC("hdrdgain_ctrl_en", "0")
    bool hdrdgain_ctrl_en;
    // M4_ARRAY_DESC("hdr_dgain_scale_s", "f32", M4_SIZE(1,13), M4_RANGE(0,65535), "0.0", M4_DIGIT(2), M4_DYNAMIC(0))
    float hdr_dgain_scale_s[RK_BAYER2DNR_V2_MAX_ISO_NUM];
    // M4_ARRAY_DESC("hdr_dgain_scale_m", "f32", M4_SIZE(1,13), M4_RANGE(0,65535), "0.0", M4_DIGIT(2), M4_DYNAMIC(0))
    float hdr_dgain_scale_m[RK_BAYER2DNR_V2_MAX_ISO_NUM];

} RK_Bayer2dnr_Params_V2_t;


typedef struct Abayer2dnr_ExpInfo_V2_s {

    // M4_NUMBER_DESC("hdr_mode", "u8", M4_RANGE(0, 2), "0", M4_DIGIT(0))
    int hdr_mode;

    // M4_NUMBER_DESC("snr_mode", "s8", M4_RANGE(0, 2), "0", M4_DIGIT(0))
    int snr_mode;

    // M4_NUMBER_DESC("gray_mode", "s8", M4_RANGE(0, 1), "0", M4_DIGIT(0))
    int gray_mode;

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
} Abayer2dnr_ExpInfo_V2_t;

typedef struct rk_aiq_bayer2dnr_info_v2_s {
    // M4_ARRAY_TABLE_DESC("sync", "array_table_ui", "none", "1")
    rk_aiq_uapi_sync_t sync;
    // M4_NUMBER_DESC("iso", "u32", M4_RANGE(0, 204800), "50", M4_DIGIT(0), "0", "0")
    int iso;
    // M4_ARRAY_TABLE_DESC("expo_info", "normal_ui_style", "none", "0", "0")
    Abayer2dnr_ExpInfo_V2_t expo_info;
} rk_aiq_bayer2dnr_info_v2_t;



RKAIQ_END_DECLARE

#endif
