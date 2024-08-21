/*
 * ynr_head_v1.h
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

#ifndef __CALIBDBV2_YNR_HEADER_V1_H__
#define __CALIBDBV2_YNR_HEADER_V1_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

///////////////////////////ynr v1//////////////////////////////////////

typedef struct CalibDbV2_YnrV1_C_ISO_s {
    // M4_NUMBER_MARK_DESC("iso", "f32", M4_RANGE(50, 204800), "50", M4_DIGIT(1), "index2")
    float iso;
    // M4_ARRAY_DESC("sigma_curve", "f32", M4_SIZE(1,5), M4_RANGE(-65535.0, 65535), "[2.0,2.0,1.2,1.2]", M4_DIGIT(6), M4_DYNAMIC(0))
    double sigma_curve[5];
    // M4_ARRAY_DESC("ynr_lci", "f32", M4_SIZE(1,4), M4_RANGE(0.0, 16.0), "[0.9332,0.6865,0.4320,0.2739]", M4_DIGIT(4), M4_DYNAMIC(0))
    float ynr_lci[4];
    // M4_ARRAY_DESC("ynr_lhci", "f32", M4_SIZE(1,4), M4_RANGE(0.0, 16.0), "[0.8322,0.9514,0.7381,0.4824]", M4_DIGIT(4), M4_DYNAMIC(0))
    float ynr_lhci[4];
    // M4_ARRAY_DESC("ynr_hlci", "f32", M4_SIZE(1,4), M4_RANGE(0.0, 16.0), "[0.8451,0.9787,0.7649,0.5108]", M4_DIGIT(4), M4_DYNAMIC(0))
    float ynr_hlci[4];
    // M4_ARRAY_DESC("ynr_hhci", "f32", M4_SIZE(1,4), M4_RANGE(0.0, 16.0), "[0.6986,1.4097,1.2535,0.8366]", M4_DIGIT(4), M4_DYNAMIC(0))
    float ynr_hhci[4];
} CalibDbV2_YnrV1_C_ISO_t;

typedef struct CalibDbV2_YnrV1_C_Set_s {
    // M4_STRING_MARK_DESC("SNR_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "LSNR",M4_DYNAMIC(0), "index1")
    char *SNR_Mode;
    // M4_STRING_DESC("Sensor_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "lcg", M4_DYNAMIC(0))
    char *Sensor_Mode;
    // M4_STRUCT_LIST_DESC("Calib_ISO", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_YnrV1_C_ISO_t *Calib_ISO;
    int Calib_ISO_len;
} CalibDbV2_YnrV1_C_Set_t;

typedef struct CalibDbV2_YnrV1_Calib_s {
    // M4_STRUCT_LIST_DESC("Setting", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_YnrV1_C_Set_t *Setting;
    int Setting_len;
} CalibDbV2_YnrV1_Calib_t;

typedef struct CalibDbV2_YnrV1_Luma_s {
    // M4_ARRAY_DESC("lo_lumaPoint", "f32", M4_SIZE(1,6), M4_RANGE(0,256), "[0.0000,32.0000,64.0000,128.0000,192.0000,256.0000]", M4_DIGIT(0), M4_DYNAMIC(0))
    float lo_lumaPoint[6];
    // M4_ARRAY_DESC("lo_lumaRatio", "f32", M4_SIZE(1,6), M4_RANGE(0,2.0), ""[1.0,1.0,1.0,1.0,1.0,1.0]"", M4_DIGIT(2), M4_DYNAMIC(0))
    float lo_lumaRatio[6];
    // M4_ARRAY_DESC("hi_lumaPoint", "f32", M4_SIZE(1,6), M4_RANGE(0,256), "[0.0000,32.0000,64.0000,128.0000,192.0000,256.0000]", M4_DIGIT(0), M4_DYNAMIC(0))
    float hi_lumaPoint[6];
    // M4_ARRAY_DESC("hi_lumaRatio", "f32", M4_SIZE(1,6), M4_RANGE(0,2.0), ""[1.0,1.0,1.0,1.0,1.0,1.0]"", M4_DIGIT(2), M4_DYNAMIC(0))
    float hi_lumaRatio[6];
} CalibDbV2_YnrV1_Luma_t;

typedef struct CalibDbV2_YnrV1_Hgrad_s {
    // M4_ARRAY_DESC("y_luma_point", "f32", M4_SIZE(1,6), M4_RANGE(0,512), "[64.0000,128.0000,192.0000,256.0000,384.0000,512.0000]", M4_DIGIT(0), M4_DYNAMIC(0))
    float y_luma_point[6];
    // M4_ARRAY_DESC("hgrad_y_level1", "f32", M4_SIZE(1,6), M4_RANGE(0,1.0), "[1.0000,0.6700,0.6700,0.6700,0.6700,0.6700]", M4_DIGIT(0), M4_DYNAMIC(0))
    float hgrad_y_level1[6];
    // M4_ARRAY_DESC("hgrad_y_level2", "f32", M4_SIZE(1,6), M4_RANGE(0,1.0), "[1.0000,0.6700,0.6700,0.6700,0.6700,0.6700]", M4_DIGIT(0), M4_DYNAMIC(0))
    float hgrad_y_level2[6];
    // M4_ARRAY_DESC("hgrad_y_level3", "f32", M4_SIZE(1,6), M4_RANGE(0,1.0), "[1.0000,0.5700,0.5700,0.5700,0.5700,0.5700]", M4_DIGIT(0), M4_DYNAMIC(0))
    float hgrad_y_level3[6];
    // M4_ARRAY_DESC("hgrad_y_level4", "f32", M4_SIZE(1,6), M4_RANGE(0,1.0), "[1.0000,0.5000,0.5000,0.4000,0.3000,0.2000]", M4_DIGIT(0), M4_DYNAMIC(0))
    float hgrad_y_level4[6];
} CalibDbV2_YnrV1_Hgrad_t;


typedef struct CalibDbV2_YnrV1_T_ISO_s {
    // M4_NUMBER_MARK_DESC("iso", "f32", M4_RANGE(50, 204800), "50", M4_DIGIT(1), "index2")
    float iso;

    // M4_NUMBER_DESC("lo_bfScale_1", "f32", M4_RANGE(0.0, 15.9), "1.0", M4_DIGIT(2))
    float lo_bfScale_1;
    // M4_NUMBER_DESC("lo_bfScale_2", "f32", M4_RANGE(0.0, 15.9), "1.0", M4_DIGIT(2))
    float lo_bfScale_2;
    // M4_NUMBER_DESC("lo_bfScale_3", "f32", M4_RANGE(0.0, 15.9), "1.0", M4_DIGIT(2))
    float lo_bfScale_3;
    // M4_NUMBER_DESC("lo_bfScale_4", "f32", M4_RANGE(0.0, 15.9), "1.0", M4_DIGIT(2))
    float lo_bfScale_4;

    // M4_NUMBER_DESC("denoise_weight_1", "f32", M4_RANGE(0.0, 1.0), "0.5", M4_DIGIT(2))
    float denoise_weight_1;
    // M4_NUMBER_DESC("denoise_weight_2", "f32", M4_RANGE(0.0, 1.0), "0.5", M4_DIGIT(2))
    float denoise_weight_2;
    // M4_NUMBER_DESC("denoise_weight_3", "f32", M4_RANGE(0.0, 1.0), "0.5", M4_DIGIT(2))
    float denoise_weight_3;
    // M4_NUMBER_DESC("denoise_weight_4", "f32", M4_RANGE(0.0, 1.0), "0.5", M4_DIGIT(2))
    float denoise_weight_4;

    // M4_NUMBER_DESC("imerge_ratio", "f32", M4_RANGE(0.0, 1.0), "0.4", M4_DIGIT(2))
    float imerge_ratio;
    // M4_NUMBER_DESC("imerge_bound", "f32", M4_RANGE(1.0, 16.0), "1.25", M4_DIGIT(2))
    float imerge_bound;
    // M4_NUMBER_DESC("lo_directionStrength", "f32", M4_RANGE(0.0, 16.0), "0.5", M4_DIGIT(2))
    float lo_directionStrength;

    // M4_NUMBER_DESC("hi_bfScale_1", "f32", M4_RANGE(0.0, 15.9), "1.0", M4_DIGIT(2))
    float hi_bfScale_1;
    // M4_NUMBER_DESC("hi_bfScale_2", "f32", M4_RANGE(0.0, 15.9), "1.0", M4_DIGIT(2))
    float hi_bfScale_2;
    // M4_NUMBER_DESC("hi_bfScale_3", "f32", M4_RANGE(0.0, 15.9), "1.0", M4_DIGIT(2))
    float hi_bfScale_3;
    // M4_NUMBER_DESC("hi_bfScale_4", "f32", M4_RANGE(0.0, 15.9), "1.0", M4_DIGIT(2))
    float hi_bfScale_4;

    // M4_NUMBER_DESC("hi_denoiseWeight_1", "f32", M4_RANGE(0.0, 1.0), "0.5", M4_DIGIT(2))
    float hi_denoiseWeight_1;
    // M4_NUMBER_DESC("hi_denoiseWeight_2", "f32", M4_RANGE(0.0, 1.0), "0.5", M4_DIGIT(2))
    float hi_denoiseWeight_2;
    // M4_NUMBER_DESC("hi_denoiseWeight_3", "f32", M4_RANGE(0.0, 1.0), "0.5", M4_DIGIT(2))
    float hi_denoiseWeight_3;
    // M4_NUMBER_DESC("hi_denoiseWeight_4", "f32", M4_RANGE(0.0, 1.0), "0.5", M4_DIGIT(2))
    float hi_denoiseWeight_4;

    // M4_NUMBER_DESC("hi_soft_thresh_scale_1", "f32", M4_RANGE(0.0, 1.0), "0.1", M4_DIGIT(2))
    float hi_soft_thresh_scale_1;
    // M4_NUMBER_DESC("hi_soft_thresh_scale_2", "f32", M4_RANGE(0.0, 1.0), "0.1", M4_DIGIT(2))
    float hi_soft_thresh_scale_2;
    // M4_NUMBER_DESC("hi_soft_thresh_scale_3", "f32", M4_RANGE(0.0, 1.0), "0.1", M4_DIGIT(2))
    float hi_soft_thresh_scale_3;
    // M4_NUMBER_DESC("hi_soft_thresh_scale_4", "f32", M4_RANGE(0.0, 1.0), "0.1", M4_DIGIT(2))
    float hi_soft_thresh_scale_4;

    // M4_NUMBER_DESC("hi_denoiseStrength", "f32", M4_RANGE(0.0, 63.0), "1.0", M4_DIGIT(2))
    float hi_denoiseStrength;
    // M4_NUMBER_DESC("hi_detailMinAdjDnW", "f32", M4_RANGE(0.0, 1.9), "0.2", M4_DIGIT(2))
    float hi_detailMinAdjDnW;
    // M4_ARRAY_DESC("hwith_d", "f32", M4_SIZE(1,4), M4_RANGE(0,15.9), "[2.0,2.0,1.2,1.2]", M4_DIGIT(6), M4_DYNAMIC(0), M4_HIDE(1))
    float hwith_d[4];

    // M4_ARRAY_TABLE_DESC("luma_para", "array_table_ui", "none")
    CalibDbV2_YnrV1_Luma_t luma_para;
    // M4_ARRAY_TABLE_DESC("hgrad_para", "array_table_ui", "none", M4_HIDE(1))
    CalibDbV2_YnrV1_Hgrad_t hgrad_para;
} CalibDbV2_YnrV1_T_ISO_t;


typedef struct CalibDbV2_YnrV1_T_Set_s {
    // M4_STRING_MARK_DESC("SNR_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "LSNR",M4_DYNAMIC(0), "index1")
    char *SNR_Mode;
    // M4_STRING_DESC("Sensor_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "lcg", M4_DYNAMIC(0))
    char *Sensor_Mode;
    // M4_STRUCT_LIST_DESC("Tuning_ISO", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_YnrV1_T_ISO_t *Tuning_ISO;
    int Tuning_ISO_len;
} CalibDbV2_YnrV1_T_Set_t;

typedef struct CalibDbV2_YnrV1_Tuning_s {
    // M4_BOOL_DESC("enable", "1")
    bool enable;
    // M4_STRUCT_LIST_DESC("Setting", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_YnrV1_T_Set_t *Setting;
    int Setting_len;
} CalibDbV2_YnrV1_Tuning_t;


typedef struct CalibDbV2_YnrV1_s {
    // M4_STRING_DESC("Version", M4_SIZE(1,1), M4_RANGE(0, 64), "V1", M4_DYNAMIC(0))
    char *Version;
    // M4_STRUCT_DESC("CalibPara", "normal_ui_style")
    CalibDbV2_YnrV1_Calib_t CalibPara;
    // M4_STRUCT_DESC("TuningPara", "normal_ui_style")
    CalibDbV2_YnrV1_Tuning_t TuningPara;
} CalibDbV2_YnrV1_t;

RKAIQ_END_DECLARE

#endif
