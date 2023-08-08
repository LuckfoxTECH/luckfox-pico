/*
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

#ifndef __ADRC_UAPI_HEAD_H__
#define __ADRC_UAPI_HEAD_H__

#include "adrc_head.h"

typedef struct mDrcGain_t {
    // M4_NUMBER_DESC("DrcGain", "f32", M4_RANGE(1,8), "1.00", M4_DIGIT(2))
    float DrcGain;
    // M4_NUMBER_DESC("Alpha", "f32", M4_RANGE(0,1), "0.20", M4_DIGIT(2))
    float Alpha;
    // M4_NUMBER_DESC("Clip", "f32", M4_RANGE(0,64), "16.00", M4_DIGIT(2))
    float Clip;
} mDrcGain_t;

typedef struct mDrcHiLit_s {
    // M4_NUMBER_DESC("Strength", "f32", M4_RANGE(0,1), "1.00", M4_DIGIT(2))
    float Strength;
} mDrcHiLit_t;

typedef struct mLocalDataV21_s {
    // M4_NUMBER_DESC("LocalWeit", "f32", M4_RANGE(0,1), "1.00", M4_DIGIT(2))
    float LocalWeit;
    // M4_NUMBER_DESC("GlobalContrast", "f32", M4_RANGE(0,1), "0.00", M4_DIGIT(3))
    float GlobalContrast;
    // M4_NUMBER_DESC("LoLitContrast", "f32", M4_RANGE(0,1), "0.00", M4_DIGIT(3))
    float LoLitContrast;
} mLocalDataV21_t;

typedef struct mDrcLocalV21_s {
    // M4_STRUCT_DESC("LocalData", "normal_ui_style")
    mLocalDataV21_t LocalData;
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
} mDrcLocalV21_t;

typedef struct mDrcCompress_s {
    // M4_ENUM_DESC("Mode", "CompressMode_t", "COMPRESS_AUTO")
    CompressMode_t Mode;
    // M4_ARRAY_MARK_DESC("Manual_curve", "u32", M4_SIZE(1,17),  M4_RANGE(0, 8192), "[0, 558, 1087, 1588, 2063, 2515, 2944, 3353, 3744, 4473, 5139, 5751, 6316, 6838, 7322, 7772, 8192]", M4_DIGIT(0), M4_DYNAMIC(0), "drc_compress_curve")
    uint16_t Manual_curve[ADRC_Y_NUM];
} mDrcCompress_t;

typedef struct mdrcAttr_V21_s {
    // M4_BOOL_DESC("Enable", "1")
    bool Enable;
    // M4_STRUCT_DESC("DrcGain", "normal_ui_style")
    mDrcGain_t DrcGain;
    // M4_STRUCT_DESC("HiLit", "normal_ui_style")
    mDrcHiLit_t HiLit;
    // M4_STRUCT_DESC("LocalSetting", "normal_ui_style")
    mDrcLocalV21_t LocalSetting;
    // M4_STRUCT_DESC("Compress", "normal_ui_style")
    mDrcCompress_t Compress;
    // M4_ARRAY_DESC("Scale_y", "u16", M4_SIZE(1,17),  M4_RANGE(0, 2048), "[0,2,20,76,193,381,631,772,919,1066,1211,1479,1700,1863,1968,2024,2048]", M4_DIGIT(0), M4_DYNAMIC(0))
    int Scale_y[ADRC_Y_NUM];
    // M4_NUMBER_DESC("Edge_Weit", "f32",  M4_RANGE(0,1), "1",M4_DIGIT(3))
    float Edge_Weit;
    // M4_BOOL_DESC("OutPutLongFrame", "0")
    bool OutPutLongFrame;
    // M4_NUMBER_DESC("IIR_frame", "u8", M4_RANGE(1,1000), "2", M4_DIGIT(0))
    int IIR_frame;
} mdrcAttr_V21_t;

typedef struct mLocalDataV30_s {
    // M4_NUMBER_DESC("LocalWeit", "f32", M4_RANGE(0,1), "1.00", M4_DIGIT(2))
    float LocalWeit;
    // M4_NUMBER_DESC("LocalAutoEnable", "u8", M4_RANGE(0,1), "1", M4_DIGIT(0))
    int LocalAutoEnable;
    // M4_NUMBER_DESC("LocalAutoWeit", "f32", M4_RANGE(0,1), "0.037477", M4_DIGIT(5))
    float LocalAutoWeit;
    // M4_NUMBER_DESC("GlobalContrast", "f32", M4_RANGE(0,1), "0.00", M4_DIGIT(3))
    float GlobalContrast;
    // M4_NUMBER_DESC("LoLitContrast", "f32", M4_RANGE(0,1), "0.00", M4_DIGIT(3))
    float LoLitContrast;
} mLocalDataV30_t;

typedef struct mDrcLocalV30_s {
    // M4_STRUCT_DESC("LocalData", "normal_ui_style")
    mLocalDataV30_t LocalData;
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
} mDrcLocalV30_t;

typedef struct mdrcAttr_V30_s {
    // M4_BOOL_DESC("Enable", "1")
    bool Enable;
    // M4_STRUCT_DESC("DrcGain", "normal_ui_style")
    mDrcGain_t DrcGain;
    // M4_STRUCT_DESC("HiLight", "normal_ui_style")
    mDrcHiLit_t HiLight;
    // M4_STRUCT_DESC("LocalSetting", "normal_ui_style")
    mDrcLocalV30_t LocalSetting;
    // M4_STRUCT_DESC("CompressSetting", "normal_ui_style")
    mDrcCompress_t CompressSetting;
    // M4_ARRAY_DESC("Scale_y", "u16", M4_SIZE(1,17),  M4_RANGE(0, 2048), "[0,2,20,76,193,381,631,772,919,1066,1211,1479,1700,1863,1968,2024,2048]", M4_DIGIT(0), M4_DYNAMIC(0))
    int Scale_y[ADRC_Y_NUM];
    // M4_NUMBER_DESC("Edge_Weit", "f32",  M4_RANGE(0,1), "1",M4_DIGIT(3))
    float Edge_Weit;
    // M4_BOOL_DESC("OutPutLongFrame", "0")
    bool OutPutLongFrame;
    // M4_NUMBER_DESC("IIR_frame", "u8", M4_RANGE(1,1000), "2", M4_DIGIT(0))
    int IIR_frame;
} mdrcAttr_V30_t;

typedef struct stAutoDrcGain_s {
    // M4_ARRAY_DESC("CtrlData", "f32", M4_SIZE(1,13), M4_RANGE(0,10000000), "[0, 0.005, 0.01, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 1]",M4_DIGIT(3), M4_DYNAMIC(0))
    float CtrlData[ADRC_STEP_MAX];
    // M4_ARRAY_DESC("DrcGain", "f32", M4_SIZE(1,13), M4_RANGE(1,8), "[1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]",M4_DIGIT(2), M4_DYNAMIC(0))
    float DrcGain[ADRC_STEP_MAX];
    // M4_ARRAY_DESC("Alpha", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2]",M4_DIGIT(2), M4_DYNAMIC(0))
    float Alpha[ADRC_STEP_MAX];
    // M4_ARRAY_DESC("Clip", "f32", M4_SIZE(1,13), M4_RANGE(0,64), "[16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16]",M4_DIGIT(2), M4_DYNAMIC(0))
    float Clip[ADRC_STEP_MAX];
} stAutoDrcGain_t;

typedef struct stAutoDrcHiLit_s {
    // M4_ARRAY_DESC("CtrlData", "f32", M4_SIZE(1,13), M4_RANGE(0,10000000), "[0, 0.005, 0.01, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 1]",M4_DIGIT(3), M4_DYNAMIC(0))
    float CtrlData[ADRC_STEP_MAX];
    // M4_ARRAY_DESC("Strength", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]",M4_DIGIT(2), M4_DYNAMIC(0))
    float Strength[ADRC_STEP_MAX];
} stAutoDrcHiLit_t;

typedef struct stAutoLocalDataV30_s {
    // M4_ARRAY_DESC("CtrlData", "f32", M4_SIZE(1,13), M4_RANGE(0,10000000), "[0, 0.005, 0.01, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 1]",M4_DIGIT(2), M4_DYNAMIC(0))
    float CtrlData[ADRC_STEP_MAX];
    // M4_ARRAY_DESC("LocalWeit", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]",M4_DIGIT(2), M4_DYNAMIC(0))
    float LocalWeit[ADRC_STEP_MAX];
    // M4_ARRAY_DESC("LocalAutoEnable", "u8", M4_SIZE(1,13), M4_RANGE(0,1), "[1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]",M4_DIGIT(0), M4_DYNAMIC(0))
    int LocalAutoEnable[ADRC_STEP_MAX];
    // M4_ARRAY_DESC("LocalAutoWeit", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0.037477, 0.037477, 0.037477, 0.037477, 0.037477, 0.037477, 0.037477, 0.037477, 0.037477, 0.037477, 0.037477, 0.037477, 0.037477]",M4_DIGIT(5), M4_DYNAMIC(0))
    float LocalAutoWeit[ADRC_STEP_MAX];
    // M4_ARRAY_DESC("GlobalContrast", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]",M4_DIGIT(3), M4_DYNAMIC(0))
    float GlobalContrast[ADRC_STEP_MAX];
    // M4_ARRAY_DESC("LoLitContrast", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]",M4_DIGIT(3), M4_DYNAMIC(0))
    float LoLitContrast[ADRC_STEP_MAX];
} stAutoLocalDataV30_t;

typedef struct stAutoDrcLocalV30_s {
    // M4_STRUCT_DESC("LocalData", "normal_ui_style")
    stAutoLocalDataV30_t LocalData;
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
} stAutoDrcLocalV30_t;

typedef struct adrcAttrData_V30_s {
    // M4_BOOL_DESC("Enable", "1")
    bool Enable;
    // M4_ENUM_DESC("CtrlDataType", "CtrlDataType_t", "CTRLDATATYPE_ENVLV")
    CtrlDataType_t CtrlDataType;
    // M4_STRUCT_DESC("DrcGain", "normal_ui_style")
    stAutoDrcGain_t DrcGain;
    // M4_STRUCT_DESC("HiLight", "normal_ui_style")
    stAutoDrcHiLit_t HiLight;
    // M4_STRUCT_DESC("LocalSetting", "normal_ui_style")
    stAutoDrcLocalV30_t LocalSetting;
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
    float Tolerance;
    // M4_NUMBER_DESC("damp", "f32", M4_RANGE(0,1), "0.9", M4_DIGIT(3))
    float damp;
} adrcAttrData_V30_t;

typedef struct adrcAttr_V30_s {
    // M4_STRUCT_DESC("DrcTuningPara", "normal_ui_style")
    adrcAttrData_V30_t DrcTuningPara;
} adrcAttr_V30_t;

typedef struct stAutoLocalDataV21_s {
    // M4_ARRAY_DESC("CtrlData", "f32", M4_SIZE(1,13), M4_RANGE(0,10000000), "[0, 0.005, 0.01, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 1]",M4_DIGIT(2), M4_DYNAMIC(0))
    float CtrlData[ADRC_STEP_MAX];
    // M4_ARRAY_DESC("LocalWeit", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]",M4_DIGIT(2), M4_DYNAMIC(0))
    float LocalWeit[ADRC_STEP_MAX];
    // M4_ARRAY_DESC("GlobalContrast", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]",M4_DIGIT(3), M4_DYNAMIC(0))
    float GlobalContrast[ADRC_STEP_MAX];
    // M4_ARRAY_DESC("LoLitContrast", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]",M4_DIGIT(3), M4_DYNAMIC(0))
    float LoLitContrast[ADRC_STEP_MAX];
} stAutoLocalDataV21_t;

typedef struct stAutoDrcLocalV21_s {
    // M4_STRUCT_DESC("LocalTMOData", "normal_ui_style")
    stAutoLocalDataV21_t LocalTMOData;
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
} stAutoDrcLocalV21_t;

typedef struct adrcAttrData_V21_s {
    // M4_BOOL_DESC("Enable", "1")
    bool Enable;
    // M4_ENUM_DESC("CtrlDataType", "CtrlDataType_t", "CTRLDATATYPE_ENVLV")
    CtrlDataType_t CtrlDataType;
    // M4_STRUCT_DESC("DrcGain", "normal_ui_style")
    stAutoDrcGain_t DrcGain;
    // M4_STRUCT_DESC("HiLight", "normal_ui_style")
    stAutoDrcHiLit_t HiLight;
    // M4_STRUCT_DESC("LocalTMOSetting", "normal_ui_style")
    stAutoDrcLocalV21_t LocalTMOSetting;
    // M4_STRUCT_DESC("CompressSetting", "normal_ui_style")
    Compress_t CompressSetting;
    // M4_ARRAY_DESC("Scale_y", "u16", M4_SIZE(1,17),  M4_RANGE(0, 2048), "[0,2,20,76,193,381,631,772,919,1066,1211,1479,1700,1863,1968,2024,2048]", M4_DIGIT(0), M4_DYNAMIC(0))
    int Scale_y[ADRC_Y_NUM];
    // M4_NUMBER_DESC("ByPassThr", "f32", M4_RANGE(0,1), "0", M4_DIGIT(4))
    float ByPassThr;
    // M4_NUMBER_DESC("Edge_Weit", "f32",  M4_RANGE(0,1), "1",M4_DIGIT(3))
    float Edge_Weit;  // sw_drc_edge_scl, range[0,1], step 1/255
    // M4_BOOL_DESC("OutPutLongFrame", "0")
    bool OutPutLongFrame;  // sw_drc_min_ogain
    // M4_NUMBER_DESC("IIR_frame", "u8", M4_RANGE(1,1000), "16", M4_DIGIT(0))
    int IIR_frame;  // sw_drc_iir_frame, range [1, 1000]
    // M4_NUMBER_DESC("Tolerance", "f32", M4_RANGE(0,1), "0", M4_DIGIT(3))
    float Tolerance;
    // M4_NUMBER_DESC("damp", "f32", M4_RANGE(0,1), "0.9", M4_DIGIT(3))
    float damp;
} adrcAttrData_V21_t;

typedef struct adrcAttr_V21_s {
    // M4_STRUCT_DESC("DrcTuningPara", "normal_ui_style")
    adrcAttrData_V21_t DrcTuningPara;
} adrcAttr_V21_t;

typedef struct DrcCtrlInfo_s {
    // M4_NUMBER_DESC("EnvLv", "f32", M4_RANGE(0,1), "0", M4_DIGIT(4))
    float EnvLv;
    // M4_NUMBER_DESC("ISO", "f32", M4_RANGE(0,1000000000), "50", M4_DIGIT(2))
    float ISO;
} DrcCtrlInfo_t;

typedef struct DrcInfo_s {
    // M4_STRUCT_DESC("CtrlInfo", "normal_ui_style")
    DrcCtrlInfo_t CtrlInfo;
    // M4_STRUCT_DESC("ValidParamsV21", "normal_ui_style")
    mdrcAttr_V21_t ValidParamsV21;
    // M4_STRUCT_DESC("ValidParamsV30", "normal_ui_style")
    mdrcAttr_V30_t ValidParamsV30;
} DrcInfo_t;

#endif /*__ADRC_UAPI_HEAD_H__*/
