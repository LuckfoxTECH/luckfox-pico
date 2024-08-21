/*
 * bayernr_head_v1.h
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

#ifndef __CALIBDBV2_BAYER_TNR_HEADER_V2_H__
#define __CALIBDBV2_BAYER_TNR_HEADER_V2_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

////////////////////////bayernr V2//////////////////////////////////////
typedef struct CalibDbV2_BayerTnrV2_C_ISO_s {
    // M4_NUMBER_MARK_DESC("iso", "f32", M4_RANGE(50, 204800), "50", M4_DIGIT(1), "index2")
    float iso;

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

} CalibDbV2_BayerTnrV2_C_ISO_t;

typedef struct CalibDbV2_BayerTnrV2_C_Set_s {
    // M4_STRING_MARK_DESC("SNR_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "LSNR",M4_DYNAMIC(0), "index1")
    char *SNR_Mode;
    // M4_STRING_DESC("Sensor_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "lcg", M4_DYNAMIC(0))
    char *Sensor_Mode;
    // M4_STRUCT_LIST_DESC("Calib_ISO", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_BayerTnrV2_C_ISO_t *Calib_ISO;
    int Calib_ISO_len;

} CalibDbV2_BayerTnrV2_C_Set_t;

typedef struct CalibDbV2_BayerTnrV2_Calib_s {
    // M4_STRUCT_LIST_DESC("Setting", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_BayerTnrV2_C_Set_t *Setting;
    int Setting_len;
} CalibDbV2_BayerTnrV2_Calib_t;


typedef struct CalibDbV2_BayerTnrV2_T_ISO_s {
    // M4_NUMBER_MARK_DESC("iso", "f32", M4_RANGE(50, 204800), "50", M4_DIGIT(1), "index2")
    float iso;

    // M4_NUMBER_DESC("thumbds", "f32", M4_RANGE(0, 8), "8", M4_DIGIT(2))
    float thumbds;

    // M4_BOOL_DESC("lo_enable", "1")
    float lo_enable;

    // M4_BOOL_DESC("hi_enable", "1")
    float hi_enable;

    // M4_BOOL_DESC("lo_med_en", "1")
    float lo_med_en;
    // M4_BOOL_DESC("lo_gsbay_en", "1")
    float lo_gsbay_en;
    // M4_BOOL_DESC("lo_gslum_en", "1")
    float lo_gslum_en;
    // M4_BOOL_DESC("hi_med_en", "1")
    float hi_med_en;
    // M4_BOOL_DESC("hi_gslum_en", "1")
    float hi_gslum_en;

    // M4_NUMBER_DESC("hi_wgt_comp", "f32", M4_RANGE(0, 1), "0.16", M4_DIGIT(2))
    float hi_wgt_comp;

    // M4_NUMBER_DESC("clipwgt", "f32", M4_RANGE(0, 1), "0.03215", M4_DIGIT(5))
    float clipwgt;

    // M4_BOOL_DESC("global_pk_en", "0")
    float global_pk_en;
    // M4_NUMBER_DESC("global_pksq", "f32", M4_RANGE(0, 268435455), "1024", M4_DIGIT(0))
    float global_pksq;

    // M4_NUMBER_DESC("hidif_th", "f32", M4_RANGE(0, 65535), "32767", M4_DIGIT(0))
    float hidif_th;


    // M4_NUMBER_DESC("lo_filter_strength", "f32", M4_RANGE(0, 16), "1.0", M4_DIGIT(2))
    float lo_filter_strength;

    // M4_NUMBER_DESC("hi_filter_strength", "f32", M4_RANGE(0, 16), "1.0", M4_DIGIT(2))
    float hi_filter_strength;



    // M4_NUMBER_DESC("soft_threshold_ratio", "f32", M4_RANGE(0, 1.0), "0.0", M4_DIGIT(2))
    float soft_threshold_ratio;


} CalibDbV2_BayerTnrV2_T_ISO_t;

typedef struct CalibDbV2_BayerTnrV2_T_Set_s {
    // M4_STRING_MARK_DESC("SNR_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "LSNR",M4_DYNAMIC(0), "index1")
    char *SNR_Mode;
    // M4_STRING_DESC("Sensor_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "lcg", M4_DYNAMIC(0))
    char *Sensor_Mode;
    // M4_STRUCT_LIST_DESC("Tuning_ISO", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_BayerTnrV2_T_ISO_t *Tuning_ISO;
    int Tuning_ISO_len;
} CalibDbV2_BayerTnrV2_T_Set_t;

typedef struct CalibDbV2_BayerTnrV2_Tuning_s {
    // M4_BOOL_DESC("enable", "1")
    bool enable;
    // M4_STRUCT_LIST_DESC("Setting", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_BayerTnrV2_T_Set_t *Setting;
    int Setting_len;
} CalibDbV2_BayerTnrV2_Tuning_t;


typedef struct CalibDbV2_BayerTnrV2_s {
    // M4_STRING_DESC("Version", M4_SIZE(1,1), M4_RANGE(0, 64), "V2", M4_DYNAMIC(0))
    char *Version;
    // M4_STRUCT_DESC("CalibPara", "normal_ui_style")
    CalibDbV2_BayerTnrV2_Calib_t CalibPara;
    // M4_STRUCT_DESC("Bayernr3D", "normal_ui_style")
    CalibDbV2_BayerTnrV2_Tuning_t TuningPara;
} CalibDbV2_BayerTnrV2_t;

RKAIQ_END_DECLARE

#endif
