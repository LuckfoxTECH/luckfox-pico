/*
 * adrc_head.h
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

#ifndef __CALIBDBV2_ADRC_HEADER_H__
#define __CALIBDBV2_ADRC_HEADER_H__

#include "adehaze_head.h"

RKAIQ_BEGIN_DECLARE

#define ADRC_Y_NUM 17
#define ADRC_STEP_MAX 13

typedef struct AdrcGain_s {
    // M4_ARRAY_DESC("CtrlData", "f32", M4_SIZE(1,100), M4_RANGE(0,10000000), "0",M4_DIGIT(3), M4_DYNAMIC(1))
    float* CtrlData;
    int CtrlData_len;
    // M4_ARRAY_DESC("DrcGain", "f32", M4_SIZE(1,100), M4_RANGE(1,8), "4",M4_DIGIT(2), M4_DYNAMIC(1))
    float* DrcGain;
    int DrcGain_len;
    // M4_ARRAY_DESC("Alpha", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0.2",M4_DIGIT(2), M4_DYNAMIC(1))
    float*            Alpha;
    int Alpha_len;
    // M4_ARRAY_DESC("Clip", "f32", M4_SIZE(1,100), M4_RANGE(0,64), "16",M4_DIGIT(2), M4_DYNAMIC(1))
    float* Clip;
    int Clip_len;
} AdrcGain_t;

typedef struct HighLight_s {
    // M4_ARRAY_DESC("CtrlData", "f32", M4_SIZE(1,100), M4_RANGE(0,10000000), "0",M4_DIGIT(3), M4_DYNAMIC(1))
    float* CtrlData;
    int CtrlData_len;
    // M4_ARRAY_DESC("Strength", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "1",M4_DIGIT(2), M4_DYNAMIC(1))
    float* Strength;
    int Strength_len;
} HighLight_t;

typedef struct LocalData_s {
    // M4_ARRAY_DESC("CtrlData", "f32", M4_SIZE(1,100), M4_RANGE(0,10000000), "0",M4_DIGIT(2), M4_DYNAMIC(1))
    float* CtrlData;
    int CtrlData_len;
    // M4_ARRAY_DESC("LocalWeit", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "1",M4_DIGIT(2), M4_DYNAMIC(1))
    float* LocalWeit;
    int LocalWeit_len;
    // M4_ARRAY_DESC("GlobalContrast", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0",M4_DIGIT(3), M4_DYNAMIC(1))
    float* GlobalContrast;
    int GlobalContrast_len;
    // M4_ARRAY_DESC("LoLitContrast", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0",M4_DIGIT(3), M4_DYNAMIC(1))
    float* LoLitContrast;
    int LoLitContrast_len;
} LocalData_t;

typedef struct local_s {
    // M4_ARRAY_TABLE_DESC("LocalTMOData", "array_table_ui", "none")
    LocalData_t LocalTMOData;
    // M4_NUMBER_DESC("curPixWeit", "f32", M4_RANGE(0,1), "0.37", M4_DIGIT(3))
    float curPixWeit;
    // M4_NUMBER_DESC("preFrameWeit", "f32", M4_RANGE(0,1), "1.0", M4_DIGIT(3))
    float preFrameWeit;
    // M4_NUMBER_DESC("Range_force_sgm", "f32", M4_RANGE(0,1), "0.0", M4_DIGIT(4))
    float Range_force_sgm;
    // M4_NUMBER_DESC("Range_sgm_cur", "f32", M4_RANGE(0,1), "0.125", M4_DIGIT(4))
    float Range_sgm_cur;
    // M4_NUMBER_DESC("Range_sgm_pre", "f32", M4_RANGE(0,1), "0.125", M4_DIGIT(4))
    float Range_sgm_pre;
    // M4_NUMBER_DESC("Space_sgm_cur", "u16", M4_RANGE(0,4095), "4068", M4_DIGIT(0))
    int Space_sgm_cur;
    // M4_NUMBER_DESC("Space_sgm_pre", "u16", M4_RANGE(0,4095), "3968", M4_DIGIT(0))
    int Space_sgm_pre;
} local_t;

typedef enum CompressMode_e {
    COMPRESS_AUTO     = 0,
    COMPRESS_MANUAL   = 1,
} CompressMode_t;

typedef struct Compress_s {
    // M4_ENUM_DESC("Mode", "CompressMode_t", "COMPRESS_AUTO")
    CompressMode_t Mode;
    // M4_ARRAY_MARK_DESC("Manual_curve", "u32", M4_SIZE(1,17),  M4_RANGE(0, 8192), "[0, 558, 1087, 1588, 2063, 2515, 2944, 3353, 3744, 4473, 5139, 5751, 6316, 6838, 7322, 7772, 8192]", M4_DIGIT(0), M4_DYNAMIC(0), "drc_compress_curve")
    uint16_t       Manual_curve[ADRC_Y_NUM];
} Compress_t;

typedef struct CalibDbV2_Adrc_s {
    // M4_BOOL_DESC("Enable", "1")
    bool Enable;
    // M4_ENUM_DESC("CtrlDataType", "CtrlDataType_t", "CTRLDATATYPE_ENVLV")
    CtrlDataType_t CtrlDataType;
    // M4_ARRAY_TABLE_DESC("DrcGain", "array_table_ui", "none")
    AdrcGain_t DrcGain;
    // M4_ARRAY_TABLE_DESC("HiLight", "array_table_ui", "none")
    HighLight_t HiLight;
    // M4_STRUCT_DESC("LocalTMOSetting", "normal_ui_style")
    local_t LocalTMOSetting;
    // M4_STRUCT_DESC("CompressSetting", "normal_ui_style")
    Compress_t CompressSetting;
    // M4_ARRAY_DESC("Scale_y", "u16", M4_SIZE(1,17),  M4_RANGE(0, 2048), "[0,2,20,76,193,381,631,772,919,1066,1211,1479,1700,1863,1968,2024,2048]", M4_DIGIT(0), M4_DYNAMIC(0))
    int Scale_y[ADRC_Y_NUM];
    // M4_NUMBER_DESC("ByPassThr", "f32", M4_RANGE(0,1), "0", M4_DIGIT(4))
    float ByPassThr;
    // M4_NUMBER_DESC("Edge_Weit", "f32",  M4_RANGE(0,1), "1",M4_DIGIT(3))
    float Edge_Weit;
    // M4_BOOL_DESC("OutPutLongFrame", "0")
    bool OutPutLongFrame;
    // M4_NUMBER_DESC("IIR_frame", "u8", M4_RANGE(1,1000), "16", M4_DIGIT(0))
    int IIR_frame;
    // M4_NUMBER_DESC("Tolerance", "f32", M4_RANGE(0,1), "0", M4_DIGIT(3))
    float                  Tolerance;
    // M4_NUMBER_DESC("damp", "f32", M4_RANGE(0,1), "0.9", M4_DIGIT(3))
    float damp;
} CalibDbV2_Adrc_t;

typedef struct CalibDbV2_drc_s {
    // M4_STRUCT_DESC("DrcTuningPara", "normal_ui_style")
    CalibDbV2_Adrc_t DrcTuningPara;
} CalibDbV2_drc_t;

typedef struct LocalDataV2_s {
    // M4_ARRAY_DESC("CtrlData", "f32", M4_SIZE(1,100), M4_RANGE(0,10000000), "0",M4_DIGIT(2), M4_DYNAMIC(1))
    float* CtrlData;
    int CtrlData_len;
    // M4_ARRAY_DESC("LocalWeit", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "1",M4_DIGIT(2), M4_DYNAMIC(1))
    float* LocalWeit;
    int LocalWeit_len;
    // M4_ARRAY_DESC("LocalAutoEnable", "u8", M4_SIZE(1,100), M4_RANGE(0,1), "1",M4_DIGIT(0), M4_DYNAMIC(1))
    int* LocalAutoEnable;
    int LocalAutoEnable_len;
    // M4_ARRAY_DESC("LocalAutoWeit", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0.037477",M4_DIGIT(5), M4_DYNAMIC(1))
    float* LocalAutoWeit;
    int LocalAutoWeit_len;
    // M4_ARRAY_DESC("GlobalContrast", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0",M4_DIGIT(3), M4_DYNAMIC(1))
    float* GlobalContrast;
    int GlobalContrast_len;
    // M4_ARRAY_DESC("LoLitContrast", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0",M4_DIGIT(3), M4_DYNAMIC(1))
    float* LoLitContrast;
    int LoLitContrast_len;
} LocalDataV2_t;

typedef struct localV2_s {
    // M4_ARRAY_TABLE_DESC("LocalData", "array_table_ui", "none")
    LocalDataV2_t LocalData;
    // M4_NUMBER_DESC("curPixWeit", "f32", M4_RANGE(0,1), "0.37", M4_DIGIT(3))
    float curPixWeit;
    // M4_NUMBER_DESC("preFrameWeit", "f32", M4_RANGE(0,1), "1.0", M4_DIGIT(3))
    float preFrameWeit;
    // M4_NUMBER_DESC("Range_force_sgm", "f32", M4_RANGE(0,1), "0.0", M4_DIGIT(4))
    float Range_force_sgm;
    // M4_NUMBER_DESC("Range_sgm_cur", "f32", M4_RANGE(0,1), "0.125", M4_DIGIT(4))
    float Range_sgm_cur;
    // M4_NUMBER_DESC("Range_sgm_pre", "f32", M4_RANGE(0,1), "0.125", M4_DIGIT(4))
    float Range_sgm_pre;
    // M4_NUMBER_DESC("Space_sgm_cur", "u16", M4_RANGE(0,4095), "4068", M4_DIGIT(0))
    int Space_sgm_cur;
    // M4_NUMBER_DESC("Space_sgm_pre", "u16", M4_RANGE(0,4095), "3968", M4_DIGIT(0))
    int Space_sgm_pre;
} localV2_t;

typedef struct CalibDbV2_Adrc_V2_s {
    // M4_BOOL_DESC("Enable", "1")
    bool Enable;
    // M4_ENUM_DESC("CtrlDataType", "CtrlDataType_t", "CTRLDATATYPE_ENVLV")
    CtrlDataType_t CtrlDataType;
    // M4_ARRAY_TABLE_DESC("DrcGain", "array_table_ui", "none")
    AdrcGain_t DrcGain;
    // M4_ARRAY_TABLE_DESC("HiLight", "array_table_ui", "none")
    HighLight_t HiLight;
    // M4_STRUCT_DESC("LocalSetting", "normal_ui_style")
    localV2_t LocalSetting;
    // M4_STRUCT_DESC("CompressSetting", "normal_ui_style")
    Compress_t CompressSetting;
    // M4_ARRAY_DESC("Scale_y", "u16", M4_SIZE(1,17),  M4_RANGE(0, 2048), "[0,2,20,76,193,381,631,772,919,1066,1211,1479,1700,1863,1968,2024,2048]", M4_DIGIT(0), M4_DYNAMIC(0))
    int Scale_y[ADRC_Y_NUM];
    // M4_NUMBER_DESC("ByPassThr", "f32", M4_RANGE(0,1), "0", M4_DIGIT(4))
    float ByPassThr;
    // M4_NUMBER_DESC("Edge_Weit", "f32",  M4_RANGE(0,1), "1",M4_DIGIT(3))
    float Edge_Weit; //sw_drc_edge_scl, range[0,1], step 1/255
    // M4_BOOL_DESC("OutPutLongFrame", "0")
    bool  OutPutLongFrame;  //sw_drc_min_ogain
    // M4_NUMBER_DESC("IIR_frame", "u8", M4_RANGE(1,1000), "16", M4_DIGIT(0))
    int IIR_frame; //sw_drc_iir_frame, range [1, 1000]
    // M4_NUMBER_DESC("Tolerance", "f32", M4_RANGE(0,1), "0", M4_DIGIT(3))
    float                  Tolerance;
    // M4_NUMBER_DESC("damp", "f32", M4_RANGE(0,1), "0.9", M4_DIGIT(3))
    float damp;
} CalibDbV2_Adrc_V2_t;

typedef struct CalibDbV2_drc_V2_s {
    // M4_STRUCT_DESC("DrcTuningPara", "normal_ui_style")
    CalibDbV2_Adrc_V2_t DrcTuningPara;
} CalibDbV2_drc_V2_t;

RKAIQ_END_DECLARE

#endif
