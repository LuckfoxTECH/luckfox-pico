
#ifndef __AMERGE_UAPI_HEAD_H__
#define __AMERGE_UAPI_HEAD_H__

#include "amerge_head.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mMergeOECurveV21_s {
    // M4_NUMBER_DESC("Smooth", "f32", M4_RANGE(0,1), "0.4", M4_DIGIT(4))
    float Smooth;
    // M4_NUMBER_DESC("Offset", "f32", M4_RANGE(108,280), "210", M4_DIGIT(4))
    float Offset;
} mMergeOECurveV21_t;

typedef struct mMergeMDCurveV21_s {
    // M4_NUMBER_DESC("LM_smooth", "f32", M4_RANGE(0,1), "0.4", M4_DIGIT(4))
    float LM_smooth;
    // M4_NUMBER_DESC("LM_offset", "f32", M4_RANGE(0,1), "0.38", M4_DIGIT(4))
    float LM_offset;
    // M4_NUMBER_DESC("MS_smooth", "f32", M4_RANGE(0,1), "0.4", M4_DIGIT(4))
    float MS_smooth;
    // M4_NUMBER_DESC("MS_offset", "f32", M4_RANGE(0,1), "0.38", M4_DIGIT(4))
    float MS_offset;
} mMergeMDCurveV21_t;

typedef struct mmergeAttrV21_s {
    // M4_STRUCT_DESC("OECurve", "normal_ui_style")
    mMergeOECurveV21_t OECurve;
    // M4_STRUCT_DESC("MDCurve", "normal_ui_style")
    mMergeMDCurveV21_t MDCurve;
} mmergeAttrV21_t;

typedef struct mLongFrameModeData_s {
    // M4_STRUCT_DESC("OECurve", "normal_ui_style")
    mMergeOECurveV21_t OECurve;
    // M4_STRUCT_DESC("MDCurve", "normal_ui_style")
    mMergeMDCurveV21_t MDCurve;
} mLongFrameModeData_t;

typedef struct mMergeMDCurveV30Short_s {
    // M4_NUMBER_DESC("Coef", "f32", M4_RANGE(0,1), "0.05", M4_DIGIT(4))
    float Coef;
    // M4_NUMBER_DESC("ms_thd0", "f32", M4_RANGE(0,1), "0.0", M4_DIGIT(1))
    float ms_thd0;
    // M4_NUMBER_DESC("lm_thd0", "f32", M4_RANGE(0,1), "0.0", M4_DIGIT(1))
    float lm_thd0;
} mMergeMDCurveV30Short_t;

typedef struct mShortFrameModeData_s {
    // M4_STRUCT_DESC("OECurve", "normal_ui_style")
    mMergeOECurveV21_t OECurve;
    // M4_STRUCT_DESC("MDCurve", "normal_ui_style")
    mMergeMDCurveV30Short_t MDCurve;
} mShortFrameModeData_t;

typedef struct mMergeAttrV30_s {
    // M4_ENUM_DESC("BaseFrm", "MergeBaseFrame_t", "BASEFRAME_LONG")
    MergeBaseFrame_t BaseFrm;
    // M4_STRUCT_DESC("LongFrmModeData", "normal_ui_style")
    mLongFrameModeData_t LongFrmModeData;
    // M4_STRUCT_DESC("ShortFrmModeData", "normal_ui_style")
    mShortFrameModeData_t ShortFrmModeData;
} mMergeAttrV30_t;

typedef struct uapiMergeCurrCtlData_s {
    // M4_NUMBER_DESC("Envlv", "f32", M4_RANGE(0,1), "0", M4_DIGIT(4))
    float Envlv;
    // M4_NUMBER_DESC("ISO", "f32", M4_RANGE(0,10000000000), "0", M4_DIGIT(2))
    float ISO;
    // M4_NUMBER_DESC("MoveCoef", "f32", M4_RANGE(0,1), "0", M4_DIGIT(4))
    float MoveCoef;
} uapiMergeCurrCtlData_t;

#ifdef __cplusplus
}
#endif

#endif  /*__AMERGE_UAPI_HEAD_H__*/
