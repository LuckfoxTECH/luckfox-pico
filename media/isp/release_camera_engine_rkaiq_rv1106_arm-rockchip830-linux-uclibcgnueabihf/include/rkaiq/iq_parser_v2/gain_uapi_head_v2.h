/*
 * Copyright (c) 2019-2022 Rockchip Eletronics Co., Ltd.
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
 */

#ifndef __CALIBDBV2_GAIN_UAPI_HEADER_V2_H__
#define __CALIBDBV2_GAIN_UAPI_HEADER_V2_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

///////////////////////////gain v2//////////////////////////////////////

typedef struct RK_GAIN_Select_V2_s {
    // M4_BOOL_DESC("hdrgain_ctrl_enable", "1")
    bool hdrgain_ctrl_enable;

    // M4_NUMBER_DESC("hdr_gain_scale_s", "f32", M4_RANGE(0, 128.0), "1.0", M4_DIGIT(2))
    float hdr_gain_scale_s;

    // M4_NUMBER_DESC("hdr_gain_scale_m", "f32", M4_RANGE(0, 128.0), "1.0", M4_DIGIT(2))
    float hdr_gain_scale_m;

} RK_GAIN_Select_V2_t;

typedef struct Again_ExpInfo_V2_s {
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

    // M4_NUMBER_DESC("isoLevelLow", "u8", M4_RANGE(0, 12), "0", M4_DIGIT(0))
    int isoLevelLow;
    // M4_NUMBER_DESC("isoLevelHig", "u8", M4_RANGE(0, 12), "0", M4_DIGIT(0))
    int isoLevelHig;

} Again_ExpInfo_V2_t;

typedef struct rk_aiq_gain_info_v2_s {
    // M4_ARRAY_TABLE_DESC("sync", "array_table_ui", "none", "1")
    rk_aiq_uapi_sync_t sync;
    // M4_NUMBER_DESC("iso", "u32", M4_RANGE(0, 204800), "50", M4_DIGIT(0), "0", "0")
    int iso;
    // M4_ARRAY_TABLE_DESC("expo_info", "normal_ui_style", "none", "0", "0")
    Again_ExpInfo_V2_t expo_info;
} rk_aiq_gain_info_v2_t;



RKAIQ_END_DECLARE

#endif
