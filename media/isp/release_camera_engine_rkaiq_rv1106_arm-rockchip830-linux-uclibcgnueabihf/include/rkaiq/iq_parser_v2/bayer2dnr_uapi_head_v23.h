/*
 * bayertnr_uapi_head_v23.h
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

#ifndef __CALIBDBV2_BAYER2DNR_UAPI_HEADER_V23_H__
#define __CALIBDBV2_BAYER2DNR_UAPI_HEADER_V23_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

////////////////////////bayernr V23//////////////////////////////////////
#if 1
typedef struct RK_Bayer2dnr_Params_V23_Select_s {
    // v23 version register // edge filter params
    // M4_BOOL_DESC("enable", "1")
    bool enable;


    // M4_ARRAY_DESC("lumapoint", "s32", M4_SIZE(1,16), M4_RANGE(0,65535), "0.0", M4_DIGIT(0), M4_DYNAMIC(0))
    int lumapoint[16];

    // M4_ARRAY_DESC("sigma", "s32", M4_SIZE(1,16), M4_RANGE(0,65535), "0.0", M4_DIGIT(0), M4_DYNAMIC(0))
    int sigma[16];

    // M4_BOOL_DESC("gauss_guide", "1")
    bool gauss_guide;

    // M4_NUMBER_DESC("filter_strength", "f32", M4_RANGE(0, 16), "0.5", M4_DIGIT(2))
    float filter_strength;

    // M4_NUMBER_DESC("edgesofts", "f32", M4_RANGE(0, 16.0), "1.0", M4_DIGIT(2))
    float edgesofts;

    // M4_NUMBER_DESC("ratio", "f32", M4_RANGE(0, 1.0), "0.2", M4_DIGIT(2))
    float ratio;

    // M4_NUMBER_DESC("weight", "f32", M4_RANGE(0, 1.0), "1.0", M4_DIGIT(2))
    float weight;

    // M4_NUMBER_DESC("pix_diff", "u16", M4_RANGE(0, 16383), "16383", M4_DIGIT(0))
    uint16_t pix_diff;
    // M4_NUMBER_DESC("diff_thld", "u16", M4_RANGE(0, 1023), "1023", M4_DIGIT(0))
    uint16_t diff_thld;


    // M4_NUMBER_DESC("trans_mode", "u8", M4_RANGE(0, 3), "0", M4_DIGIT(0))
    uint8_t trans_mode;
    // M4_NUMBER_DESC("trans_offset", "u16", M4_RANGE(0, 8191), "256", M4_DIGIT(0))
    uint16_t trans_offset;
    // M4_NUMBER_DESC("itrans_offset", "u16", M4_RANGE(0, 65535), "32768", M4_DIGIT(0))
    uint16_t itrans_offset;
    // M4_NUMBER_DESC("trans_datmax", "u32", M4_RANGE(0, 1048575), "1048575", M4_DIGIT(0))
    uint32_t trans_datmax;


    // M4_BOOL_DESC("gain_bypass", "1")
    bool gain_bypass;
    // M4_NUMBER_DESC("gain_scale", "f32", M4_RANGE(0, 16.0), "1.0", M4_DIGIT(2))
    float gain_scale;
    // M4_ARRAY_DESC("gain_lumapoint", "u16", M4_SIZE(1,16), M4_RANGE(0,256), "[16,32,48,64,80,96,112,128,144,160,176,192,208,224,240,256]", M4_DIGIT(0), M4_DYNAMIC(0))
    uint16_t gain_lumapoint[16];
    // M4_ARRAY_DESC("gain_adj", "u16", M4_SIZE(1,16), M4_RANGE(0,4095), "[16,32,48,64,80,96,112,128,144,160,176,192,208,224,240,256]", M4_DIGIT(2), M4_DYNAMIC(0))
    uint16_t gain_adj[16];

    // M4_BOOL_DESC("hdrdgain_ctrl_en", "0")
    bool hdrdgain_ctrl_en;
    // M4_NUMBER_DESC("hdr_dgain_scale_s", "f32", M4_RANGE(0, 128.0), "1.0", M4_DIGIT(2))
    float hdr_dgain_scale_s;
    // M4_NUMBER_DESC("hdr_dgain_scale_m", "f32", M4_RANGE(0, 128.0), "1.0", M4_DIGIT(2))
    float hdr_dgain_scale_m;

} RK_Bayer2dnrV23_Params_Select_t;

typedef struct Abayer2dnr_ExpInfo_V23_s {
    // M4_NUMBER_DESC("hdr_mode", "u8", M4_RANGE(0, 2), "0", M4_DIGIT(0))
    int hdr_mode;

    // M4_NUMBER_DESC("snr_mode", "s8", M4_RANGE(0, 2), "0", M4_DIGIT(0))
    int   snr_mode;

    // M4_NUMBER_DESC("gray_mode", "s8", M4_RANGE(0, 1), "0", M4_DIGIT(0))
    int gray_mode;

    // M4_NUMBER_DESC("bayertnr_en", "s8", M4_RANGE(0, 1), "0", M4_DIGIT(0))
    int bayertnr_en;

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

} Abayer2dnr_ExpInfo_V23_t;

typedef struct rk_aiq_bayer2dnr_info_v23_s {
    // M4_ARRAY_TABLE_DESC("sync", "array_table_ui", "none", "1")
    rk_aiq_uapi_sync_t sync;
    // M4_NUMBER_DESC("iso", "u32", M4_RANGE(0, 204800), "50", M4_DIGIT(0), "0", "0")
    int iso;
    // M4_ARRAY_TABLE_DESC("expo_info", "normal_ui_style", "none", "0", "0")
    Abayer2dnr_ExpInfo_V23_t expo_info;
} rk_aiq_bayer2dnr_info_v23_t;


#endif


RKAIQ_END_DECLARE

#endif
