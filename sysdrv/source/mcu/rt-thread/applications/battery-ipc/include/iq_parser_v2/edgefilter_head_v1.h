/*
 * edgefilter_head_v1.h
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

#ifndef __CALIBDBV2_EDGE_FILTER_HEADER_V1_H__
#define __CALIBDBV2_EDGE_FILTER_HEADER_V1_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

///////////////////////////edgefilter v1//////////////////////////////////////

typedef struct CalibDbV2_Edgefilter_Luma_s {
    // M4_ARRAY_DESC("luma_point", "f32", M4_SIZE(1, 8), M4_RANGE(0,255), "[0,16,32,64,96,160,224,256]", M4_DIGIT(2), M4_DYNAMIC(0))
    float luma_point[8];
    // M4_ARRAY_DESC("noise_clip", "f32", M4_SIZE(1, 8), M4_RANGE(0,255), "[2.0000,6.0000,8.0000,10.0000,16.0000,16.0000,14.0000,10.0000]", M4_DIGIT(2), M4_DYNAMIC(0))
    float noise_clip[8];
    // M4_ARRAY_DESC("dog_clip_pos", "f32", M4_SIZE(1, 8), M4_RANGE(0,255), "[10.0000,12.0000,12.0000,16.0000,16.0000,12.0000,10.0000,10.0000]", M4_DIGIT(2), M4_DYNAMIC(0))
    float dog_clip_pos[8];
    // M4_ARRAY_DESC("dog_clip_neg", "f32", M4_SIZE(1, 8), M4_RANGE(0,255), "[10.0000,12.0000,12.0000,16.0000,16.0000,12.0000,10.0000,10.0000]", M4_DIGIT(2), M4_DYNAMIC(0))
    float dog_clip_neg[8];
    // M4_ARRAY_DESC("dog_alpha", "f32", M4_SIZE(1, 8), M4_RANGE(0,3), "[0.1000,0.2000,0.2000,0.2000,0.2000,0.2000,0.2000,0.1000]", M4_DIGIT(2), M4_DYNAMIC(0))
    float dog_alpha[8];
} CalibDbV2_Edgefilter_Luma_t;

typedef struct CalibDbV2_Edgefilter_T_ISO_s {
    // M4_NUMBER_MARK_DESC("iso", "f32", M4_RANGE(50, 204800), "50", M4_DIGIT(1), "index2")
    float iso;
    // M4_NUMBER_DESC("edge_thed", "f32", M4_RANGE(1.0, 255.0), "16.0", M4_DIGIT(2))
    float edge_thed;
    // M4_NUMBER_DESC("src_wgt", "f32", M4_RANGE(0, 1.0), "0.0", M4_DIGIT(2))
    float src_wgt;
    // M4_BOOL_DESC("alpha_adp_en", "0")
    bool alpha_adp_en;
    // M4_NUMBER_DESC("local_alpha", "f32", M4_RANGE(0, 1.0), "0.5", M4_DIGIT(2))
    float local_alpha;
    // M4_NUMBER_DESC("global_alpha", "f32", M4_RANGE(0, 1.0), "0.5", M4_DIGIT(2))
    float global_alpha;
    // M4_NUMBER_DESC("dog_kernel_percent", "f32", M4_RANGE(0, 1.0), "0.5", M4_DIGIT(2))
    float dog_kernel_percent;
    // M4_ARRAY_DESC("direct_filter_coeff", "f32", M4_SIZE(1, 5), M4_RANGE(0,1), "[0.1000,0.2000,0.4000,0.2000,0.1000]", M4_DIGIT(6), M4_DYNAMIC(0))
    float direct_filter_coeff[5];
    // M4_ARRAY_TABLE_DESC("luma_para", "array_table_ui", "none")
    CalibDbV2_Edgefilter_Luma_t luma_para;
} CalibDbV2_Edgefilter_T_ISO_t;


typedef struct CalibDbV2_Edgefilter_T_Set_s {
    // M4_STRING_MARK_DESC("SNR_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "LSNR",M4_DYNAMIC(0), "index1")
    char *SNR_Mode;
    // M4_STRING_DESC("Sensor_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "lcg",M4_DYNAMIC(0))
    char *Sensor_Mode;
    // M4_STRUCT_LIST_DESC("Tuning_ISO", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_Edgefilter_T_ISO_t *Tuning_ISO;
    int Tuning_ISO_len;
} CalibDbV2_Edgefilter_T_Set_t;

typedef struct CalibDbV2_Edgefilter_Kernel_s {
    // M4_ARRAY_DESC("dog_kernel_l", "f32", M4_SIZE(5,5), M4_RANGE(0,1), "[-0.0026,-0.1699,-0.6814,-0.1699,-0.0026,-0.1699,0.1134,0.4547,0.1134,-0.1699,-0.6814,0.4547,1.8234,0.4547,-0.6814,-0.1699,0.1134,0.4547,0.1134,-0.1699,-0.0026,-0.1699,-0.6814,-0.1699,-0.0026]", M4_DIGIT(6), M4_DYNAMIC(0))
    float dog_kernel_l[25];
    // M4_ARRAY_DESC("dog_kernel_h", "f32", M4_SIZE(5,5), M4_RANGE(0,1), "[-0.0026,-0.1699,-0.6814,-0.1699,-0.0026,-0.1699,0.1134,0.4547,0.1134,-0.1699,-0.6814,0.4547,1.8234,0.4547,-0.6814,-0.1699,0.1134,0.4547,0.1134,-0.1699,-0.0026,-0.1699,-0.6814,-0.1699,-0.0026]", M4_DIGIT(6), M4_DYNAMIC(0))
    float dog_kernel_h[25];
} CalibDbV2_Edgefilter_Kernel_t;

typedef struct CalibDbV2_Edgefilter_Tuning_s {
    // M4_BOOL_DESC("enable", "1")
    bool enable;
    // M4_STRUCT_DESC("dog_kernel", "normal_ui_style")
    CalibDbV2_Edgefilter_Kernel_t dog_kernel;
    // M4_STRUCT_LIST_DESC("Setting", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_Edgefilter_T_Set_t *Setting;
    int Setting_len;
} CalibDbV2_Edgefilter_Tuning_t;


typedef struct CalibDbV2_Edgefilter_s {
    // M4_STRING_DESC("Version", M4_SIZE(1,1), M4_RANGE(0, 64), "V1", M4_DYNAMIC(0))
    char *Version;
    // M4_STRUCT_DESC("TuningPara", "normal_ui_style")
    CalibDbV2_Edgefilter_Tuning_t TuningPara;
} CalibDbV2_Edgefilter_t;

RKAIQ_END_DECLARE

#endif

