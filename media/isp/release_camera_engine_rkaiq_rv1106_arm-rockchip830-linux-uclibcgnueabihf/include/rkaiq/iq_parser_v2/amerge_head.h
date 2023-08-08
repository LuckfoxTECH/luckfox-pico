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
#include "rk_aiq_comm.h"

#define MERGE_ENVLV_STEP_MAX 13

RKAIQ_BEGIN_DECLARE

// merge V10
typedef struct MergeOECurveV10_s {
    // M4_ARRAY_DESC("CtrlData", "f32", M4_SIZE(1,13), M4_RANGE(0,10000000), "[0, 0.005, 0.01, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 1]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float CtrlData[MERGE_ENVLV_STEP_MAX];
    // M4_ARRAY_DESC("Smooth", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float Smooth[MERGE_ENVLV_STEP_MAX];
    // M4_ARRAY_DESC("Offset", "f32", M4_SIZE(1,13), M4_RANGE(108,300), "[210, 210, 210, 210, 210, 210, 210, 210, 210, 210, 210, 210, 210]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float Offset[MERGE_ENVLV_STEP_MAX];
} MergeOECurveV10_t;

typedef struct MergeMDCurveV10_s {
    // M4_ARRAY_DESC("MoveCoef", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0, 0.005, 0.01, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 1]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float MoveCoef[MERGE_ENVLV_STEP_MAX];
    // M4_ARRAY_DESC("LM_smooth", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float LM_smooth[MERGE_ENVLV_STEP_MAX];
    // M4_ARRAY_DESC("LM_offset", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0.38, 0.38, 0.38, 0.38, 0.38, 0.38, 0.38, 0.38, 0.38, 0.38, 0.38, 0.38, 0.38]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float LM_offset[MERGE_ENVLV_STEP_MAX];
    // M4_ARRAY_DESC("MS_smooth", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float MS_smooth[MERGE_ENVLV_STEP_MAX];
    // M4_ARRAY_DESC("MS_offset", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0.38, 0.38, 0.38, 0.38, 0.38, 0.38, 0.38, 0.38, 0.38, 0.38, 0.38, 0.38, 0.38]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float MS_offset[MERGE_ENVLV_STEP_MAX];
} MergeMDCurveV10_t;

typedef struct MergeV10_s {
    // M4_ENUM_DESC("CtrlDataType", "CtrlDataType_t", "CTRLDATATYPE_ENVLV")
    CtrlDataType_t CtrlDataType;
    // M4_ARRAY_TABLE_DESC("OECurve", "array_table_ui", "none")
    MergeOECurveV10_t OECurve;
    // M4_ARRAY_TABLE_DESC("MDCurve", "array_table_ui", "none")
    MergeMDCurveV10_t MDCurve;
    // M4_NUMBER_DESC("ByPassThr", "f32", M4_RANGE(0,1), "0", M4_DIGIT(4))
    float ByPassThr;
    // M4_NUMBER_DESC("OECurve_damp", "f32", M4_RANGE(0,1), "0.9", M4_DIGIT(4))
    float OECurve_damp;
    // M4_NUMBER_DESC("MDCurveLM_damp", "f32", M4_RANGE(0,1), "0.9", M4_DIGIT(4))
    float MDCurveLM_damp;
    // M4_NUMBER_DESC("MDCurveMS_damp", "f32", M4_RANGE(0,1), "0.9", M4_DIGIT(4))
    float MDCurveMS_damp;
} MergeV10_t;

typedef struct CalibDbV2_merge_v10_s {
    // M4_STRUCT_DESC("MergeTuningPara", "normal_ui_style")
    MergeV10_t MergeTuningPara;
} CalibDbV2_merge_v10_t;

// merge V11
typedef enum MergeBaseFrame_e {
    BASEFRAME_LONG        = 0,
    BASEFRAME_SHORT       = 1,
} MergeBaseFrame_t;

typedef struct LongFrameModeData_s {
    // M4_ARRAY_TABLE_DESC("OECurve", "array_table_ui", "none")
    MergeOECurveV10_t OECurve;
    // M4_ARRAY_TABLE_DESC("MDCurve", "array_table_ui", "none")
    MergeMDCurveV10_t MDCurve;
    // M4_NUMBER_DESC("OECurve_damp", "f32", M4_RANGE(0,1), "0.9", M4_DIGIT(4))
    float OECurve_damp;
    // M4_NUMBER_DESC("MDCurveLM_damp", "f32", M4_RANGE(0,1), "0.9", M4_DIGIT(4))
    float MDCurveLM_damp;
    // M4_NUMBER_DESC("MDCurveMS_damp", "f32", M4_RANGE(0,1), "0.9", M4_DIGIT(4))
    float MDCurveMS_damp;
} LongFrameModeData_t;

typedef struct MergeMDCurveV11Short_s {
    // M4_ARRAY_DESC("MoveCoef", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0, 0.005, 0.01, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 1]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float MoveCoef[MERGE_ENVLV_STEP_MAX];
    // M4_ARRAY_DESC("Coef", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float Coef[MERGE_ENVLV_STEP_MAX];
    // M4_ARRAY_DESC("ms_thd0", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]",M4_DIGIT(1), M4_DYNAMIC(0), 0)
    float ms_thd0[MERGE_ENVLV_STEP_MAX];
    // M4_ARRAY_DESC("lm_thd0", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]",M4_DIGIT(1), M4_DYNAMIC(0), 0)
    float lm_thd0[MERGE_ENVLV_STEP_MAX];
} MergeMDCurveV11Short_t;

typedef struct ShortFrameModeData_s {
    // M4_ARRAY_TABLE_DESC("OECurve", "array_table_ui", "none")
    MergeOECurveV10_t OECurve;
    // M4_ARRAY_TABLE_DESC("MDCurve", "array_table_ui", "none")
    MergeMDCurveV11Short_t MDCurve;
    // M4_NUMBER_DESC("OECurve_damp", "f32", M4_RANGE(0,1), "0.9", M4_DIGIT(4))
    float OECurve_damp;
    // M4_NUMBER_DESC("MDCurve_damp", "f32", M4_RANGE(0,1), "0.9", M4_DIGIT(4))
    float MDCurve_damp;
} ShortFrameModeData_t;

typedef struct MergeV11_s {
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
} MergeV11_t;

typedef struct CalibDbV2_merge_v11_s {
    // M4_STRUCT_DESC("MergeTuningPara", "normal_ui_style")
    MergeV11_t MergeTuningPara;
} CalibDbV2_merge_v11_t;

// merge v12
typedef struct MergeEachChnCurveV12_s {
    // M4_ARRAY_DESC("CtrlData", "f32", M4_SIZE(1,13), M4_RANGE(0,10000000), "[0, 0.005, 0.01, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 1]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float CtrlData[MERGE_ENVLV_STEP_MAX];
    // M4_ARRAY_DESC("Smooth", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float Smooth[MERGE_ENVLV_STEP_MAX];
    // M4_ARRAY_DESC("Offset", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0.38, 0.38, 0.38, 0.38, 0.38, 0.38, 0.38, 0.38, 0.38, 0.38, 0.38, 0.38, 0.38]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float Offset[MERGE_ENVLV_STEP_MAX];
} MergeEachChnCurveV12_t;

typedef struct LongFrameModeDataV12_s {
    // M4_BOOL_DESC("EnableEachChn", "1")
    bool EnableEachChn;
    // M4_ARRAY_TABLE_DESC("OECurve", "array_table_ui", "none")
    MergeOECurveV10_t OECurve;
    // M4_ARRAY_TABLE_DESC("MDCurve", "array_table_ui", "none")
    MergeMDCurveV10_t MDCurve;
    // M4_ARRAY_TABLE_DESC("EachChnCurve", "array_table_ui", "none")
    MergeEachChnCurveV12_t EachChnCurve;
    // M4_NUMBER_DESC("OECurve_damp", "f32", M4_RANGE(0,1), "0.9", M4_DIGIT(4))
    float OECurve_damp;
    // M4_NUMBER_DESC("MDCurveLM_damp", "f32", M4_RANGE(0,1), "0.9", M4_DIGIT(4))
    float MDCurveLM_damp;
    // M4_NUMBER_DESC("MDCurveMS_damp", "f32", M4_RANGE(0,1), "0.9", M4_DIGIT(4))
    float MDCurveMS_damp;
} LongFrameModeDataV12_t;

typedef struct MergeV12_s {
    // M4_ENUM_DESC("BaseFrm", "MergeBaseFrame_t", "BASEFRAME_LONG")
    MergeBaseFrame_t BaseFrm;
    // M4_ENUM_DESC("CtrlDataType", "CtrlDataType_t", "CTRLDATATYPE_ENVLV")
    CtrlDataType_t CtrlDataType;
    // M4_NUMBER_DESC("ByPassThr", "f32", M4_RANGE(0,1), "0", M4_DIGIT(4))
    float ByPassThr;
    // M4_STRUCT_DESC("LongFrmModeData", "normal_ui_style")
    LongFrameModeDataV12_t LongFrmModeData;
    // M4_STRUCT_DESC("ShortFrmModeData", "normal_ui_style")
    ShortFrameModeData_t ShortFrmModeData;
} MergeV12_t;

typedef struct CalibDbV2_merge_v12_s {
    // M4_STRUCT_DESC("MergeTuningPara", "normal_ui_style")
    MergeV12_t MergeTuningPara;
} CalibDbV2_merge_v12_t;

RKAIQ_END_DECLARE

#endif
