/*
 * rkpostisp_head_v1.h
 *
 *  Copyright (c) 2023 Rockchip Corporation
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

#ifndef __CALIBDBV2_POSTISPV1_HEADER_H__
#define __CALIBDBV2_POSTISPV1_HEADER_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

///////////////////////////postisp v1//////////////////////////////////////
typedef struct CalibDbV2_PostIspV1_T_ISO_s {
    // M4_NUMBER_MARK_DESC("iso", "f32", M4_RANGE(50, 409600), "50", M4_DIGIT(1), "index1")
    float iso;
    // M4_NUMBER_DESC("gain", "f32", M4_RANGE(1.0, 8192.0), "1.0", M4_DIGIT(2))
    float gain;
    // M4_NUMBER_DESC("sigma", "f32", M4_RANGE(0.0, 32.0), "2.0", M4_DIGIT(2))
    float sigma;
    // M4_NUMBER_DESC("shade", "f32", M4_RANGE(-4.0, 4.0), "0.0", M4_DIGIT(2))
    float shade;
    // M4_NUMBER_DESC("sharp", "f32", M4_RANGE(0.0, 4.0), "1.0", M4_DIGIT(2))
    float sharp;
    // M4_NUMBER_DESC("min_luma", "f32", M4_RANGE(0.0, 128.0), "0.0", M4_DIGIT(2))
    float min_luma;
    // M4_NUMBER_DESC("sat_scale", "f32", M4_RANGE(1.0, 3.0), "1", M4_DIGIT(2))
    float sat_scale;
    // M4_NUMBER_DESC("dark_contrast", "f32", M4_RANGE(0.0, 32.0), "0", M4_DIGIT(2))
    float dark_contrast;
    // M4_NUMBER_DESC("ai_ratio", "f32", M4_RANGE(0.0, 1.0), "0", M4_DIGIT(2))
    float ai_ratio;
    // M4_NUMBER_DESC("mot_thresh", "f32", M4_RANGE(0.0, 32.0), "5", M4_DIGIT(2))
    float mot_thresh;
    // M4_NUMBER_DESC("static_thresh", "f32", M4_RANGE(0.0, 32.0), "10", M4_DIGIT(2))
    float static_thresh;
    // M4_NUMBER_DESC("mot_nr_stren", "f32", M4_RANGE(0.0, 2.0), "0", M4_DIGIT(2))
    float mot_nr_stren;
    // M4_ARRAY_DESC("luma_sigma", "f32", M4_SIZE(1,6), M4_RANGE(0,1023), "[3, 3, 3, 3, 3, 3]", M4_DIGIT(0), M4_DYNAMIC(0))
    float luma_sigma[6];

}CalibDbV2_PostIspV1_T_ISO_t;


typedef struct CalibDbV2_PostIspV1_Tunning_s {
    // M4_BOOL_DESC("enable", "1")
    bool enable;
    // M4_NUMBER_DESC("gain_max", "f32", M4_RANGE(0, 8192.0), "1", M4_DIGIT(1))
    float gain_max;
    // M4_NUMBER_DESC("gain_tab_len", "u8", M4_RANGE(1, 13), "13", M4_DIGIT(0))
    uint8_t gain_tab_len;
    // M4_BOOL_DESC("tuning_visual_flag", "0")
    bool tuning_visual_flag;
    // M4_ARRAY_DESC("dynamicSw", "f32", M4_SIZE(1,2), M4_RANGE(0,1024), "[60, 68]", M4_DIGIT(1), M4_DYNAMIC(0))
    float dynamicSw[2];
    // M4_ARRAY_DESC("luma_point", "u16", M4_SIZE(1,6), M4_RANGE(0,1024), "[0, 16, 64, 128, 196, 256]", M4_DIGIT(1), M4_DYNAMIC(0))
    uint16_t luma_point[6];
    // M4_STRUCT_LIST_DESC("Tuning_ISO", M4_SIZE_DYNAMIC, "normal_ui_style")
    CalibDbV2_PostIspV1_T_ISO_t* Tuning_ISO;
    int Tuning_ISO_len;
} CalibDbV2_PostIspV1_Tunning_t;

typedef struct CalibDbV2_PostIspV1_s {
    // M4_STRING_DESC("Version", M4_SIZE(1,1), M4_RANGE(0, 64), "V33", M4_DYNAMIC(0))
    char* Version;
    // M4_STRUCT_DESC("TuningPara", "normal_ui_style")
    CalibDbV2_PostIspV1_Tunning_t TuningPara;
} CalibDbV2_PostIspV1_t;

RKAIQ_END_DECLARE

#endif // __CALIBDBV2_POSTISPV1_HEADER_H__
