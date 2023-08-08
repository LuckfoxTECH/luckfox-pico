

#ifndef __AMERGE_UAPI_HEAD_H__
#define __AMERGE_UAPI_HEAD_H__

#include "amerge_head.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct uapiMergeCurrCtlData_s {
    // M4_NUMBER_DESC("Envlv", "f32", M4_RANGE(0,1), "0", M4_DIGIT(4))
    float Envlv;
    // M4_NUMBER_DESC("ISO", "f32", M4_RANGE(50,1000000000), "0", M4_DIGIT(4))
    float ISO;
    // M4_NUMBER_DESC("MoveCoef", "f32", M4_RANGE(0,1), "0", M4_DIGIT(4))
    float MoveCoef;
} uapiMergeCurrCtlData_t;

typedef struct mMergeOECurveV10_s {
    // M4_NUMBER_DESC("Smooth", "f32", M4_RANGE(0,1), "0.4", M4_DIGIT(4))
    float Smooth;
    // M4_NUMBER_DESC("Offset", "f32", M4_RANGE(0,1), "210", M4_DIGIT(4))
    float Offset;
} mMergeOECurveV10_t;

typedef struct mMergeMDCurveV10_s {
    // M4_NUMBER_DESC("LM_smooth", "f32", M4_RANGE(0,1), "0.4", M4_DIGIT(4))
    float LM_smooth;
    // M4_NUMBER_DESC("LM_offset", "f32", M4_RANGE(0,1), "0.38", M4_DIGIT(4))
    float LM_offset;
    // M4_NUMBER_DESC("MS_smooth", "f32", M4_RANGE(0,1), "0.4", M4_DIGIT(4))
    float MS_smooth;
    // M4_NUMBER_DESC("MS_offset", "f32", M4_RANGE(0,1), "0.38", M4_DIGIT(4))
    float MS_offset;
} mMergeMDCurveV10_t;

typedef struct mMergeAttrV10_s {
    // M4_STRUCT_DESC("OECurve", "normal_ui_style")
    mMergeOECurveV10_t OECurve;
    // M4_STRUCT_DESC("MDCurve", "normal_ui_style")
    mMergeMDCurveV10_t MDCurve;
} mMergeAttrV10_t;

typedef struct mLongFrameModeData_s {
    // M4_STRUCT_DESC("OECurve", "normal_ui_style")
    mMergeOECurveV10_t OECurve;
    // M4_STRUCT_DESC("MDCurve", "normal_ui_style")
    mMergeMDCurveV10_t MDCurve;
} mLongFrameModeData_t;

typedef struct mMergeMDCurveV11Short_s {
    // M4_NUMBER_DESC("Coef", "f32", M4_RANGE(0,1), "0.05", M4_DIGIT(4))
    float Coef;
    // M4_NUMBER_DESC("ms_thd0", "f32", M4_RANGE(0,1), "0.0", M4_DIGIT(1))
    float ms_thd0;
    // M4_NUMBER_DESC("lm_thd0", "f32", M4_RANGE(0,1), "0.0", M4_DIGIT(1))
    float lm_thd0;
} mMergeMDCurveV11Short_t;

typedef struct mShortFrameModeData_s {
    // M4_STRUCT_DESC("OECurve", "normal_ui_style")
    mMergeOECurveV10_t OECurve;
    // M4_STRUCT_DESC("MDCurve", "normal_ui_style")
    mMergeMDCurveV11Short_t MDCurve;
} mShortFrameModeData_t;

typedef struct mMergeAttrV11_s {
    // M4_ENUM_DESC("BaseFrm", "MergeBaseFrame_t", "BASEFRAME_LONG")
    MergeBaseFrame_t BaseFrm;
    // M4_STRUCT_DESC("LongFrmModeData", "normal_ui_style")
    mLongFrameModeData_t LongFrmModeData;
    // M4_STRUCT_DESC("ShortFrmModeData", "normal_ui_style")
    mShortFrameModeData_t ShortFrmModeData;
} mMergeAttrV11_t;

typedef struct mMergeEachChnCurveV12_s {
    // M4_NUMBER_DESC("Smooth", "f32", M4_RANGE(0,1), "0.4", M4_DIGIT(4))
    float Smooth;
    // M4_NUMBER_DESC("Offset", "f32", M4_RANGE(0,1), "0.38", M4_DIGIT(4))
    float Offset;
} mMergeEachChnCurveV12_t;

typedef struct mLongFrameModeDataV12_s {
    // M4_BOOL_DESC("EnableEachChn", "1")
    bool EnableEachChn;
    // M4_ARRAY_TABLE_DESC("OECurve", "array_table_ui", "none")
    mMergeOECurveV10_t OECurve;
    // M4_ARRAY_TABLE_DESC("MDCurve", "array_table_ui", "none")
    mMergeMDCurveV10_t MDCurve;
    // M4_ARRAY_TABLE_DESC("EachChnCurve", "array_table_ui", "none")
    mMergeEachChnCurveV12_t EachChnCurve;
} mLongFrameModeDataV12_t;

typedef struct mMergeAttrV12_s {
    // M4_ENUM_DESC("BaseFrm", "MergeBaseFrame_t", "BASEFRAME_LONG")
    MergeBaseFrame_t BaseFrm;
    // M4_STRUCT_DESC("LongFrmModeData", "normal_ui_style")
    mLongFrameModeDataV12_t LongFrmModeData;
    // M4_STRUCT_DESC("ShortFrmModeData", "normal_ui_style")
    mShortFrameModeData_t ShortFrmModeData;
} mMergeAttrV12_t;

#ifdef __cplusplus
}
#endif

#endif  /*__AMERGE_UAPI_HEAD_H__*/
