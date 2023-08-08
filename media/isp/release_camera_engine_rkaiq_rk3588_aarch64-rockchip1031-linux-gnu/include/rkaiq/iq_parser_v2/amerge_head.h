/*
 * amerge_head.h
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

#ifndef __CALIBDBV2_AMERGE_HEADER_H__
#define __CALIBDBV2_AMERGE_HEADER_H__

#include "adehaze_head.h"

RKAIQ_BEGIN_DECLARE

typedef struct MergeOECurveV20_s {
    // M4_ARRAY_DESC("CtrlData", "f32", M4_SIZE(1,100), M4_RANGE(0,1000000000), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float* CtrlData;
    int CtrlData_len;
    // M4_ARRAY_DESC("Smooth", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0.4",M4_DIGIT(4), M4_DYNAMIC(1))
    float* Smooth;
    int Smooth_len;
    // M4_ARRAY_DESC("Offset", "f32", M4_SIZE(1,100), M4_RANGE(108,280), "210",M4_DIGIT(4), M4_DYNAMIC(1))
    float* Offset;
    int Offset_len;
} MergeOECurveV20_t;

typedef struct MergeMDCurveV20_s {
    // M4_ARRAY_DESC("MoveCoef", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float* MoveCoef;
    int MoveCoef_len;
    // M4_ARRAY_DESC("LM_smooth", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0.4",M4_DIGIT(4), M4_DYNAMIC(1))
    float* LM_smooth;
    int LM_smooth_len;
    // M4_ARRAY_DESC("LM_offset", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0.38",M4_DIGIT(4), M4_DYNAMIC(1))
    float* LM_offset;
    int LM_offset_len;
    // M4_ARRAY_DESC("MS_smooth", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0.4",M4_DIGIT(4), M4_DYNAMIC(1))
    float* MS_smooth;
    int MS_smooth_len;
    // M4_ARRAY_DESC("MS_offset", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0.38",M4_DIGIT(4), M4_DYNAMIC(1))
    float* MS_offset;
    int MS_offset_len;
} MergeMDCurveV20_t;

typedef struct MergeV20_s {
    // M4_ENUM_DESC("CtrlDataType", "CtrlDataType_t", "CTRLDATATYPE_ENVLV")
    CtrlDataType_t CtrlDataType;
    // M4_ARRAY_TABLE_DESC("OECurve", "array_table_ui", "none")
    MergeOECurveV20_t OECurve;
    // M4_ARRAY_TABLE_DESC("MDCurve", "array_table_ui", "none")
    MergeMDCurveV20_t MDCurve;
    // M4_NUMBER_DESC("ByPassThr", "f32", M4_RANGE(0,1), "0", M4_DIGIT(4))
    float ByPassThr;
    // M4_NUMBER_DESC("OECurve_damp", "f32", M4_RANGE(0,1), "0.9", M4_DIGIT(4))
    float OECurve_damp;
    // M4_NUMBER_DESC("MDCurveLM_damp", "f32", M4_RANGE(0,1), "0.9", M4_DIGIT(4))
    float MDCurveLM_damp;
    // M4_NUMBER_DESC("MDCurveMS_damp", "f32", M4_RANGE(0,1), "0.9", M4_DIGIT(4))
    float MDCurveMS_damp;
} MergeV20_t;

typedef struct CalibDbV2_merge_s {
    // M4_STRUCT_DESC("MergeTuningPara", "normal_ui_style")
    MergeV20_t MergeTuningPara;
} CalibDbV2_merge_t;

typedef enum MergeBaseFrame_e {
    BASEFRAME_LONG        = 0,
    BASEFRAME_SHORT       = 1,
} MergeBaseFrame_t;

typedef struct LongFrameModeData_s {
    // M4_ARRAY_TABLE_DESC("OECurve", "array_table_ui", "none")
    MergeOECurveV20_t OECurve;
    // M4_ARRAY_TABLE_DESC("MDCurve", "array_table_ui", "none")
    MergeMDCurveV20_t MDCurve;
    // M4_NUMBER_DESC("OECurve_damp", "f32", M4_RANGE(0,1), "0.9", M4_DIGIT(4))
    float OECurve_damp;
    // M4_NUMBER_DESC("MDCurveLM_damp", "f32", M4_RANGE(0,1), "0.9", M4_DIGIT(4))
    float MDCurveLM_damp;
    // M4_NUMBER_DESC("MDCurveMS_damp", "f32", M4_RANGE(0,1), "0.9", M4_DIGIT(4))
    float MDCurveMS_damp;
} LongFrameModeData_t;

typedef struct MergeMDCurveV21Short_s {
    // M4_ARRAY_DESC("MoveCoef", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float* MoveCoef;
    int MoveCoef_len;
    // M4_ARRAY_DESC("Coef", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0.05",M4_DIGIT(4), M4_DYNAMIC(1))
    float* Coef;
    int Coef_len;
    // M4_ARRAY_DESC("ms_thd0", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0",M4_DIGIT(1), M4_DYNAMIC(1))
    float* ms_thd0;
    int ms_thd0_len;
    // M4_ARRAY_DESC("lm_thd0", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0",M4_DIGIT(1), M4_DYNAMIC(1))
    float* lm_thd0;
    int lm_thd0_len;
} MergeMDCurveV21Short_t;

typedef struct ShortFrameModeData_s {
    // M4_ARRAY_TABLE_DESC("OECurve", "array_table_ui", "none")
    MergeOECurveV20_t OECurve;
    // M4_ARRAY_TABLE_DESC("MDCurve", "array_table_ui", "none")
    MergeMDCurveV21Short_t MDCurve;
    // M4_NUMBER_DESC("OECurve_damp", "f32", M4_RANGE(0,1), "0.9", M4_DIGIT(4))
    float OECurve_damp;
    // M4_NUMBER_DESC("MDCurve_damp", "f32", M4_RANGE(0,1), "0.9", M4_DIGIT(4))
    float MDCurve_damp;
} ShortFrameModeData_t;

typedef struct MergeV21_s {
    // M4_ENUM_DESC("BaseFrm", "MergeBaseFrame_t", "BASEFRAME_LONG")
    MergeBaseFrame_t BaseFrm;
    // M4_ENUM_DESC("CtrlDataType", "CtrlDataType_t", "CTRLDATATYPE_ENVLV")
    CtrlDataType_t CtrlDataType;
    // M4_NUMBER_DESC("ByPassThr", "f32", M4_RANGE(0,1), "0", M4_DIGIT(4))
    float ByPassThr;
    // M4_STRUCT_DESC("LongFrmModeData", "normal_ui_style")
    LongFrameModeData_t LongFrmModeData;
    // M4_STRUCT_DESC("ShortFrmModeData", "normal_ui_style")
    ShortFrameModeData_t ShortFrmModeData;
} MergeV21_t;

typedef struct CalibDbV2_merge_V2_s {
    // M4_STRUCT_DESC("MergeTuningPara", "normal_ui_style")
    MergeV21_t MergeTuningPara;
} CalibDbV2_merge_V2_t;


RKAIQ_END_DECLARE

#endif
