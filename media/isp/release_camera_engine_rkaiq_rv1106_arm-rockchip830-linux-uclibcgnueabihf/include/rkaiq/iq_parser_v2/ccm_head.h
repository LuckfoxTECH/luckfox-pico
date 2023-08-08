/*
 * ccm_head.h
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

#ifndef __CALIBDBV2_CCM_HEADER_H__
#define __CALIBDBV2_CCM_HEADER_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

#define CCM_ILLUMINATION_MAX    7
#define CCM_PROFILE_NAME        ( 25U )
#define CCM_ILLUMINATION_NAME   ( 20U )
#define CCM_PROFILES_NUM_MAX    ( 5 )
#define CCM_RESOLUTIONS_NUM_MAX ( 4 )
#define CALIBDB_ISO_NUM         ( 9 )

/*****************************************************************************/
/**
 * @brief   Global ACCM calibration structure of isp2.0
 */
/*****************************************************************************/

typedef struct CalibDbV2_Ccm_Gain_Sat_Curve_s {
    // M4_ARRAY_DESC("gains", "f32", M4_SIZE(1,4), M4_RANGE(0,256), "[4,8,16,32]", M4_DIGIT(0), M4_DYNAMIC(0))
    float gains[4];
    // M4_ARRAY_DESC("sat", "f32", M4_SIZE(1,4), M4_RANGE(0,100), "[100,100,90,50]", M4_DIGIT(1), M4_DYNAMIC(0))
    float sat[4];
} CalibDbV2_Ccm_Gain_Sat_Curve_t;

typedef struct CalibDbV2_Ccm_Gain_Scale_s {
    // M4_ARRAY_DESC("gain", "f32", M4_SIZE(1,9), M4_RANGE(0,256), "[1,2,4,8,16,32,64,128,256]", M4_DIGIT(0), M4_DYNAMIC(0))
    float gain[CALIBDB_ISO_NUM];
    // M4_ARRAY_DESC("scale", "f32", M4_SIZE(1,9), M4_RANGE(0,1), "1", M4_DIGIT(2), M4_DYNAMIC(0))
    float scale[CALIBDB_ISO_NUM];
} CalibDbV2_Ccm_Gain_Scale_t;

typedef struct CalibDbV2_Ccm_Matrix_Para_s {
    // M4_STRING_DESC("name", M4_SIZE(1,1), M4_RANGE(0, 25), "A_100",M4_DYNAMIC(0))
    char* name;
    // M4_STRING_DESC("illumination", M4_SIZE(1,1), M4_RANGE(0, 20), "A",M4_DYNAMIC(0))
    char* illumination;
    // M4_NUMBER_DESC("saturation", "f32", M4_RANGE(0.0, 100.0), "100.0", M4_DIGIT(1))
    float saturation;
    // M4_ARRAY_DESC("ccMatrix", "f32", M4_SIZE(3,3), M4_RANGE(-8,7.992), "[1,0,0,0,1,0,0,0,1]", M4_DIGIT(4), M4_DYNAMIC(0))
    float ccMatrix[9];
    // M4_ARRAY_DESC("ccOffsets", "f32", M4_SIZE(1,3), M4_RANGE(-4095,4095), "0", M4_DIGIT(1), M4_DYNAMIC(0))
    float ccOffsets[3];
} CalibDbV2_Ccm_Matrix_Para_t;

typedef struct CalibDbV2_Ccm_Accm_Cof_Para_s {
    // M4_STRING_DESC("name", M4_SIZE(1,1), M4_RANGE(0, 20), "A",M4_DYNAMIC(0))
    char* name;
    // M4_ARRAY_DESC("awbGain", "f32", M4_SIZE(1,2), M4_RANGE(0,5), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float awbGain[2];
    // M4_NUMBER_DESC("minDist", "f32", M4_RANGE(0.0, 4), "0", M4_DIGIT(4))
    float minDist;
    // M4_STRING_DESC("matrixUsed", M4_SIZE(1,2), M4_RANGE(0, 25), "A_100",M4_DYNAMIC(1))
    char** matrixUsed;
    int matrixUsed_len;
    // M4_ARRAY_TABLE_DESC("gain sat curve", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDbV2_Ccm_Gain_Sat_Curve_t gain_sat_curve;
} CalibDbV2_Ccm_Accm_Cof_Para_t;

typedef struct CalibDbV2_Ccm_Luma_Ccm_s {
    // M4_ARRAY_DESC("RGB2Y para", "f32", M4_SIZE(1,3), M4_RANGE(0,128), "[38 75 15]", M4_DIGIT(0), M4_DYNAMIC(0))
    float rgb2y_para[3];
    // M4_NUMBER_DESC("bound pos bit", "f32", M4_RANGE(4, 10), "8", M4_DIGIT(0))
    float low_bound_pos_bit;  // low & high y alpha adjust
    // M4_ARRAY_DESC("y alpha curve", "f32", M4_SIZE(1,17), M4_RANGE(0,1024), "1024", M4_DIGIT(0), M4_DYNAMIC(0))
    float y_alpha_curve[17];
    // M4_ARRAY_TABLE_DESC("gain alphaScale curve", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDbV2_Ccm_Gain_Scale_t gain_alphaScale_curve;
} CalibDbV2_Ccm_Luma_Ccm_t;

typedef struct CalibDbV2_Ccm_Alp_Sym_Para_s {
    // M4_BOOL_DESC("high Y adjust enable", "1")
    bool highy_adj_en;
    // M4_NUMBER_DESC("bound pos bit", "f32", M4_RANGE(4, 10), "10", M4_DIGIT(0))
    float bound_pos_bit;  // y alpha adjust
    // M4_ARRAY_DESC("y alpha curve", "f32", M4_SIZE(1,17), M4_RANGE(0,1024), "1024", M4_DIGIT(0), M4_DYNAMIC(0))
    float y_alpha_curve[17];
} CalibDbV2_Ccm_Alp_Sym_Para_t;

typedef struct CalibDbV2_Ccm_Alp_Asym_Para_s {
    // M4_NUMBER_DESC("left bound pos bit", "f32", M4_RANGE(3, 11), "10", M4_DIGIT(0))
    float bound_pos_bit;  // low y alpha adjust
    // M4_NUMBER_DESC("right bound pos bit", "f32", M4_RANGE(3, 11), "10", M4_DIGIT(0))
    float right_pos_bit;  // high y alpha adjust
    // M4_ARRAY_DESC("left y alpha curve", "f32", M4_SIZE(1,9), M4_RANGE(0,1024), "1024", M4_DIGIT(0), M4_DYNAMIC(0))
    float y_alpha_left_curve[9];
    // M4_ARRAY_DESC("right y alpha curve", "f32", M4_SIZE(1,9), M4_RANGE(0,1024), "1024", M4_DIGIT(0), M4_DYNAMIC(0))
    float y_alpha_right_curve[9];
} CalibDbV2_Ccm_Alp_Asym_Para_t;

typedef struct CalibDbV2_Ccm_Luma_Ccm_V2_s {
    // M4_ARRAY_DESC("RGB2Y para", "f32", M4_SIZE(1,3), M4_RANGE(0,128), "[38 75 15]", M4_DIGIT(0), M4_DYNAMIC(0))
    float rgb2y_para[3];
    // M4_BOOL_DESC("asym enable", "0")
    bool asym_enable;
    // M4_STRUCT_DESC("y alp sym config", "normal_ui_style")
    CalibDbV2_Ccm_Alp_Sym_Para_t y_alp_sym;
    // M4_STRUCT_DESC("y alp asym config", "normal_ui_style")
    CalibDbV2_Ccm_Alp_Asym_Para_t y_alp_asym;
    // M4_ARRAY_TABLE_DESC("gain alphaScale curve", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDbV2_Ccm_Gain_Scale_t gain_alphaScale_curve;
} CalibDbV2_Ccm_Luma_Ccm_V2_t;

typedef struct CalibDbV2_Ccm_illu_est_Para_s {
    // M4_BOOL_DESC("interp enable", "0")
    bool interp_enable;
    // M4_STRING_DESC("default illu", M4_SIZE(1,1), M4_RANGE(0, 20), "D65",M4_DYNAMIC(0))
    char* default_illu;
    // M4_ARRAY_DESC("weightRB", "f32", M4_SIZE(1,2), M4_RANGE(0,3), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float weightRB[2];
    // M4_NUMBER_DESC("prob limit", "f32", M4_RANGE(0.0, 1), "0.2", M4_DIGIT(6))
    float prob_limit;
    // M4_NUMBER_DESC("frame num", "u8", M4_RANGE(1, 128), "8", M4_DIGIT(0))
    int frame_no;
} CalibDbV2_Ccm_illu_est_Para_t;

typedef struct CalibDbV2_Ccm_Tuning_Para_s {
    // M4_BOOL_DESC("damp enable", "0")
    bool damp_enable;
    // M4_STRUCT_DESC("illuminant estimation", "normal_ui_style")
    CalibDbV2_Ccm_illu_est_Para_t illu_estim;
    // M4_STRUCT_LIST_DESC("aCcmCof", M4_SIZE_DYNAMIC, "normal_ui_style")
    CalibDbV2_Ccm_Accm_Cof_Para_t* aCcmCof;
    int aCcmCof_len;
    // M4_STRUCT_LIST_DESC("matrixAll",  M4_SIZE_DYNAMIC, "normal_ui_style")
    CalibDbV2_Ccm_Matrix_Para_t* matrixAll;
    int matrixAll_len;
} CalibDbV2_Ccm_Tuning_Para_t;

typedef struct CalibDbV2_Ccm_Manual_Para_s {
    // M4_ARRAY_DESC("ccMatrix", "f32", M4_SIZE(3,3), M4_RANGE(-8,7.992), "[1,0,0,0,1,0,0,0,1]", M4_DIGIT(4), M4_DYNAMIC(0))
    float ccMatrix[9];
    // M4_ARRAY_DESC("ccOffsets", "f32", M4_SIZE(1,3), M4_RANGE(-4095,4095), "0", M4_DIGIT(1), M4_DYNAMIC(0))
    float ccOffsets[3];
} CalibDbV2_Ccm_Manual_Para_t;

typedef enum CalibDbV2_Ccm_Op_Mode_t {
    CALIB_CCM_MODE_MANUAL = 1, /**< run manual lens shading correction */
    CALIB_CCM_MODE_AUTO   = 2, /**< run auto lens shading correction */
} CalibDbV2_Ccm_Op_Mode_t;

typedef struct CalibDbV2_Ccm_Control_Para_s {
    // M4_BOOL_DESC("enable", "0")
    bool enable;
    // M4_ENUM_DESC("mode", "CalibDbV2_Ccm_Op_Mode_t", "CALIB_CCM_MODE_AUTO");
    CalibDbV2_Ccm_Op_Mode_t mode;
    // M4_NUMBER_DESC("wbgain tolerance", "f32", M4_RANGE(0.0, 1), "0.1", M4_DIGIT(4))
    float wbgain_tolerance;
    // M4_NUMBER_DESC("gain tolerance", "f32", M4_RANGE(0.0, 1), "0.2", M4_DIGIT(4))
    float gain_tolerance;
} CalibDbV2_Ccm_Control_Para_t;

typedef struct CalibDbV2_Ccm_Enh_para_s {
    // M4_ARRAY_DESC("gains", "f32", M4_SIZE(1,9), M4_RANGE(0,256), "1", M4_DIGIT(0), M4_DYNAMIC(0))
    float gains[9];
    // M4_ARRAY_DESC("enable", "u8", M4_SIZE(1,9),  M4_RANGE(0, 1), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned short enh_adj_en[9];
    // M4_ARRAY_DESC("Enhance ratio max", "f32", M4_SIZE(1,9), M4_RANGE(0, 8), "0", M4_DIGIT(1), M4_DYNAMIC(0))
    float enh_rat_max[9];
} CalibDbV2_Ccm_Enh_para_t;

typedef struct CalibDbV2_Ccm_Enhance_Para_s {
    // M4_ARRAY_TABLE_DESC("Enhance ctrl", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDbV2_Ccm_Enh_para_t enh_ctl;
    // M4_ARRAY_DESC("Enhance RGB2Y para", "u8", M4_SIZE(1,3), M4_RANGE(0,128), "[38 75 15]", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char enh_rgb2y_para[3];
} CalibDbV2_Ccm_Enhance_Para_t;

typedef struct CalibDbV2_Ccm_Para_V2_s {
    // M4_STRUCT_DESC("control", "normal_ui_style")
    CalibDbV2_Ccm_Control_Para_t control;
    // M4_STRUCT_DESC("lumaCCM", "normal_ui_style")
    CalibDbV2_Ccm_Luma_Ccm_t lumaCCM;
    // M4_STRUCT_DESC("manualCCM", "normal_ui_style")
    CalibDbV2_Ccm_Manual_Para_t manualPara;
    // M4_STRUCT_DESC("autoCCM", "normal_ui_style")
    CalibDbV2_Ccm_Tuning_Para_t TuningPara;
} CalibDbV2_Ccm_Para_V2_t;

typedef struct CalibDbV2_Ccm_Para_V32_s {
    // M4_STRUCT_DESC("control", "normal_ui_style")
    CalibDbV2_Ccm_Control_Para_t control;
    // M4_STRUCT_DESC("lumaCCM", "normal_ui_style")
    CalibDbV2_Ccm_Luma_Ccm_V2_t lumaCCM;
    // M4_STRUCT_DESC("enhanceCCM", "normal_ui_style")
    CalibDbV2_Ccm_Enhance_Para_t enhCCM;
    // M4_STRUCT_DESC("manualCCM", "normal_ui_style")
    CalibDbV2_Ccm_Manual_Para_t manualPara;
    // M4_STRUCT_DESC("autoCCM", "normal_ui_style")
    CalibDbV2_Ccm_Tuning_Para_t TuningPara;
} CalibDbV2_Ccm_Para_V32_t;

RKAIQ_END_DECLARE

#endif
