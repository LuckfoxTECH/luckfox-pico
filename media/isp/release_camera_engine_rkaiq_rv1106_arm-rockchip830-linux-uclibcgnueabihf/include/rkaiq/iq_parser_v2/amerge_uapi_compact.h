/*
 *  Copyright (c) 2023 Rockchip Corporation
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

#ifndef __AMERGE_UAPI_COMPACT_H__
#define __AMERGE_UAPI_COMPACT_H__

#include "amerge_uapi_head.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uapiMergeCurrCtlData_t MergeCurrCtlData_t;

typedef enum merge_OpModeV21_e {
    MERGE_OPMODE_API_OFF = 0,  // run IQ ahdr
    MERGE_OPMODE_MANU    = 1,  // run api manual ahdr
} merge_OpModeV21_t;

typedef mMergeOECurveV10_t mMergeOECurveV21_t;
typedef mMergeMDCurveV10_t mMergeMDCurveV21_t;

typedef struct mmergeAttrV21_s {
    // M4_STRUCT_DESC("OECurve", "normal_ui_style")
    mMergeOECurveV21_t OECurve;
    // M4_STRUCT_DESC("MDCurve", "normal_ui_style")
    mMergeMDCurveV21_t MDCurve;
} mmergeAttrV21_t;

typedef struct mergeAttrV21_s {
    merge_OpModeV21_t opMode;
    mmergeAttrV21_t stManual;
    MergeCurrCtlData_t CtlInfo;
} mergeAttrV21_t;

typedef struct mLongFrameModeDataV30_s {
    // M4_STRUCT_DESC("OECurve", "normal_ui_style")
    mMergeOECurveV21_t OECurve;
    // M4_STRUCT_DESC("MDCurve", "normal_ui_style")
    mMergeMDCurveV21_t MDCurve;
} mLongFrameModeDataV30_t;

typedef mMergeMDCurveV11Short_t mMergeMDCurveV30Short_t;

typedef struct mShortFrameModeDataV30_s {
    // M4_STRUCT_DESC("OECurve", "normal_ui_style")
    mMergeOECurveV21_t OECurve;
    // M4_STRUCT_DESC("MDCurve", "normal_ui_style")
    mMergeMDCurveV30Short_t MDCurve;
} mShortFrameModeDataV30_t;

typedef struct mMergeAttrV30_s {
    // M4_ENUM_DESC("BaseFrm", "MergeBaseFrame_t", "BASEFRAME_LONG")
    MergeBaseFrame_t BaseFrm;
    // M4_STRUCT_DESC("LongFrmModeData", "normal_ui_style")
    mLongFrameModeDataV30_t LongFrmModeData;
    // M4_STRUCT_DESC("ShortFrmModeData", "normal_ui_style")
    mShortFrameModeDataV30_t ShortFrmModeData;
} mMergeAttrV30_t;

typedef struct mergeAttrV30_s {
    merge_OpModeV21_t opMode;
    mMergeAttrV30_t stManual;
    MergeCurrCtlData_t CtlInfo;
} mergeAttrV30_t;

typedef struct mergeAttr_s {
    rk_aiq_uapi_sync_t sync;

    mergeAttrV21_t attrV21;
    mergeAttrV30_t attrV30;
} mergeAttr_t;

typedef mergeAttr_t amerge_attrib_t;

#ifdef __cplusplus
}
#endif

#endif /*__AMERGE_UAPI_COMPACT_H__*/
