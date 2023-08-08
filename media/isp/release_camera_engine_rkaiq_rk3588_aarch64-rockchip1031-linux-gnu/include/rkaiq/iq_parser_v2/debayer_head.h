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

#define DEBAYER_ISO_LEN 13

RKAIQ_BEGIN_DECLARE

typedef struct Debayer_Param_Auto_s {
    // M4_ARRAY_DESC("ISO", "u32", M4_SIZE(1,13),  M4_RANGE(50, 204800), "[50,100,200,400,800,1600,3200,6400,12800,25600,51200,102400,204800]", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    int ISO[13];
    // M4_ARRAY_DESC("debayer_gain_offset", "u8",M4_SIZE(1,13),  M4_RANGE(0,15), "4", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    unsigned char debayer_gain_offset[DEBAYER_ISO_LEN];
    // M4_ARRAY_DESC("debayer_offset", "u8",M4_SIZE(1,13),M4_RANGE(0,31), "1", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    unsigned char debayer_offset[DEBAYER_ISO_LEN];
    // M4_ARRAY_DESC("debayer_clip_en", "u8",M4_SIZE(1,13), M4_RANGE(0,1), "1", M4_DYNAMIC(0), 0)
    unsigned char debayer_clip_en[DEBAYER_ISO_LEN];
    // M4_ARRAY_DESC("debayer_filter_g_en", "u8",M4_SIZE(1,13), M4_RANGE(0,1), "1", M4_DYNAMIC(0), 0)
    unsigned char debayer_filter_g_en[DEBAYER_ISO_LEN];
    // M4_ARRAY_DESC("debayer_filter_c_en", "u8",M4_SIZE(1,13), M4_RANGE(0,1), "1", M4_DYNAMIC(0), 0)
    unsigned char debayer_filter_c_en[DEBAYER_ISO_LEN];
    // M4_ARRAY_DESC("debayer_thed0", "u8", M4_SIZE(1,13), M4_RANGE(0,15), "3", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    unsigned char debayer_thed0[DEBAYER_ISO_LEN];
    // M4_ARRAY_DESC("debayer_thed1", "u8", M4_SIZE(1,13), M4_RANGE(0,15), "6", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    unsigned char debayer_thed1[DEBAYER_ISO_LEN];
    // M4_ARRAY_DESC("debayer_dist_scale", "u8", M4_SIZE(1,13), M4_RANGE(0,15), "8", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    unsigned char debayer_dist_scale[DEBAYER_ISO_LEN];
    // M4_ARRAY_DESC("debayer_cnr_strength", "u8", M4_SIZE(1,13), M4_RANGE(0,9), "5", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    unsigned char debayer_cnr_strength[DEBAYER_ISO_LEN];
    // M4_ARRAY_DESC("debayer_shift_num", "u8", M4_SIZE(1,13), M4_RANGE(0,3), "2", M4_DIGIT(0),  M4_DYNAMIC(0),0)
    unsigned char debayer_shift_num[DEBAYER_ISO_LEN];
    // M4_ARRAY_DESC("debayer_sharp_strength", "u8", M4_SIZE(1,13),  M4_RANGE(0, 7), "[4,4,4,4,4,4,4,4,4,4,4,4,4]", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    unsigned char debayer_sharp_strength[DEBAYER_ISO_LEN];
    // M4_ARRAY_DESC("debayer_hf_offset", "u16", M4_SIZE(1,13),  M4_RANGE(0, 65535), "[1,1,1,1,1,1,1,1,1,1,1,1,1]", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    unsigned short debayer_hf_offset[DEBAYER_ISO_LEN];
} Debayer_Param_Auto_t;

typedef struct __Debayer_Param {
    // M4_BOOL_DESC("debayer_en", "1", 0, M4_HIDE(1))
    bool debayer_en;
    // M4_ARRAY_DESC("debayer_filter1", "s8", M4_SIZE(1,5),  M4_RANGE(-128, 128), "[2,-6,0,6,-2]", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    int debayer_filter1[5];
    // M4_ARRAY_DESC("debayer_filter2", "s8", M4_SIZE(1,5),  M4_RANGE(-128, 128), "[2,-4,4,-4,2]", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    int debayer_filter2[5];

    // M4_ARRAY_TABLE_DESC("AutoPara", "array_table_ui", M4_INDEX_DEFAULT)
    Debayer_Param_Auto_t debayer_auto;

} Debayer_Param_t;

typedef struct __debayer {
    // M4_STRUCT_DESC("TuningPara", "normal_ui_style")
    Debayer_Param_t param;
} CalibDbV2_Debayer_t;

RKAIQ_END_DECLARE

#endif
