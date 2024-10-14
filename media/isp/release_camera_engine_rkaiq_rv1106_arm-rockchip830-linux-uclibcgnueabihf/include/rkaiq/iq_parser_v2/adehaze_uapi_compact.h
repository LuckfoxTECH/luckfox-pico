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

#ifndef __ADEHAZE_UAPI_COMPACT_H__
#define __ADEHAZE_UAPI_COMPACT_H__

#include "adehaze_head.h"

#define CALIBDB_ADEHAZE_ENHANCE_CURVE_KNOTS_NUM (DHAZ_ENHANCE_CURVE_KNOTS_NUM)

typedef DehazeDataV11_t aDehazeDataV21_t;

typedef struct aDehaze_Setting_V21_s {
    // M4_BOOL_DESC("en", "1")
    bool en;
    // M4_BOOL_DESC("air_lc_en", "1")
    bool air_lc_en;
    // M4_NUMBER_DESC("stab_fnum", "f32", M4_RANGE(0,31), "8.0", M4_DIGIT(4))
    float stab_fnum;
    // M4_NUMBER_DESC("sigma", "f32", M4_RANGE(0,255), "6.0", M4_DIGIT(4))
    float sigma;
    // M4_NUMBER_DESC("wt_sigma", "f32", M4_RANGE(0,256), "8.0", M4_DIGIT(4))
    float wt_sigma;
    // M4_NUMBER_DESC("air_sigma", "f32", M4_RANGE(0,255), "120.0", M4_DIGIT(4))
    float air_sigma;
    // M4_NUMBER_DESC("tmax_sigma", "f32", M4_RANGE(0,1), "0.0100", M4_DIGIT(4))
    float tmax_sigma;
    // M4_NUMBER_DESC("pre_wet", "f32", M4_RANGE(0,1), "0.0100", M4_DIGIT(4))
    float pre_wet;
    // M4_STRUCT_DESC("DehazeData", "normal_ui_style")
    aDehazeDataV21_t DehazeData;
} aDehaze_Setting_V21_t;

typedef EnhanceDataV11_t aEnhanceDataV21_t;

typedef struct aEnhance_Setting_V21_s {
    // M4_BOOL_DESC("en", "1")
    bool en;
    // M4_ARRAY_DESC("enhance_curve", "f32", M4_SIZE(1,17),  M4_RANGE(0, 1024), "[0,64,128,192,256,320,384,448,512,576,640,704,768,832,896,960,1023]", M4_DIGIT(0), M4_DYNAMIC(0))
    float enhance_curve[CALIBDB_ADEHAZE_ENHANCE_CURVE_KNOTS_NUM];
    // M4_STRUCT_DESC("EnhanceData", "normal_ui_style")
    aEnhanceDataV21_t EnhanceData;
} aEnhance_Setting_V21_t;

typedef HistDataV11_t aHistDataV21_t;

typedef struct aHist_setting_V21_s {
    // M4_BOOL_DESC("en", "1")
    bool en;
    // M4_BOOL_DESC("hist_para_en", "1")
    bool hist_para_en;
    // M4_STRUCT_DESC("HistData", "normal_ui_style")
    aHistDataV21_t HistData;
} aHist_setting_V21_t;

typedef struct aDehazeAttrData_s {
    // M4_BOOL_DESC("Enable", "1")
    bool Enable;
    // M4_ENUM_DESC("CtrlDataType", "CtrlDataType_t", "CTRLDATATYPE_ENVLV")
    CtrlDataType_t CtrlDataType;
    // M4_NUMBER_DESC("cfg_alpha", "f32", M4_RANGE(0,1), "1", M4_DIGIT(4))
    float cfg_alpha;
    // M4_NUMBER_DESC("ByPassThr", "f32", M4_RANGE(0,1), "0", M4_DIGIT(4))
    float ByPassThr;
    // M4_STRUCT_DESC("dehaze_setting", "normal_ui_style")
    aDehaze_Setting_V21_t dehaze_setting;
    // M4_STRUCT_DESC("enhance_setting", "normal_ui_style")
    aEnhance_Setting_V21_t enhance_setting;
    // M4_STRUCT_DESC("hist_setting", "normal_ui_style")
    aHist_setting_V21_t hist_setting;
} aDehazeAttrData_t;

typedef struct aDehazeAttr_s {
    // M4_STRUCT_DESC("DehazeTuningPara", "normal_ui_style")
    aDehazeAttrData_t DehazeTuningPara;
} aDehazeAttr_t;

typedef mDehazeDataV11_t mDehazeDataV21_t;

typedef struct mDehaze_Setting_V21_s {
    // M4_BOOL_DESC("en", "1")
    bool en;
    // M4_BOOL_DESC("air_lc_en", "1")
    bool air_lc_en;
    // M4_NUMBER_DESC("stab_fnum", "f32", M4_RANGE(0,31), "8.0", M4_DIGIT(4))
    float stab_fnum;
    // M4_NUMBER_DESC("sigma", "f32", M4_RANGE(0,255), "6.0", M4_DIGIT(4))
    float sigma;
    // M4_NUMBER_DESC("wt_sigma", "f32", M4_RANGE(0,256), "8.0", M4_DIGIT(4))
    float wt_sigma;
    // M4_NUMBER_DESC("air_sigma", "f32", M4_RANGE(0,255), "120.0", M4_DIGIT(4))
    float air_sigma;
    // M4_NUMBER_DESC("tmax_sigma", "f32", M4_RANGE(0,1), "0.0100", M4_DIGIT(4))
    float tmax_sigma;
    // M4_NUMBER_DESC("pre_wet", "f32", M4_RANGE(0,1), "0.0100", M4_DIGIT(4))
    float pre_wet;
    // M4_STRUCT_DESC("DehazeData", "normal_ui_style")
    mDehazeDataV21_t DehazeData;
} mDehaze_Setting_V21_t;

typedef mEnhanceDataV11_t mEnhanceDataV21_t;

typedef struct mEnhance_Setting_V21_s {
    // M4_BOOL_DESC("en", "1")
    bool en;
    // M4_ARRAY_DESC("enhance_curve", "f32", M4_SIZE(1,17),  M4_RANGE(0, 1024), "[0,64,128,192,256,320,384,448,512,576,640,704,768,832,896,960,1023]", M4_DIGIT(0), M4_DYNAMIC(0))
    float enhance_curve[CALIBDB_ADEHAZE_ENHANCE_CURVE_KNOTS_NUM];
    // M4_STRUCT_DESC("EnhanceData", "normal_ui_style")
    mEnhanceDataV21_t EnhanceData;
} mEnhance_Setting_V21_t;

typedef mHistDataV11_t mHistDataV21_t;

typedef struct mHist_setting_V21_s {
    // M4_BOOL_DESC("en", "1")
    bool en;
    // M4_BOOL_DESC("hist_para_en", "1")
    bool hist_para_en;
    // M4_STRUCT_DESC("HistData", "normal_ui_style")
    mHistDataV21_t HistData;
} mHist_setting_V21_t;

typedef struct mDehazeAttr_s {
    // M4_BOOL_DESC("Enable", "1")
    bool Enable;
    // M4_NUMBER_DESC("cfg_alpha", "f32", M4_RANGE(0,1), "1", M4_DIGIT(4))
    float cfg_alpha;
    // M4_STRUCT_DESC("dehaze_setting", "normal_ui_style")
    mDehaze_Setting_V21_t dehaze_setting;
    // M4_STRUCT_DESC("enhance_setting", "normal_ui_style")
    mEnhance_Setting_V21_t enhance_setting;
    // M4_STRUCT_DESC("hist_setting", "normal_ui_style")
    mHist_setting_V21_t hist_setting;
} mDehazeAttr_t;

typedef struct EnhanceManuAttr_s {
    bool update;
    int level;  // 0~100
} EnhanceManuAttr_t;

typedef struct DehazeManuAttr_s {
    bool update;
    int level;  // 0~100
} DehazeManuAttr_t;

typedef struct adehaze_sw_V2_s {
    rk_aiq_uapi_sync_t sync;

    dehaze_api_mode_t mode;
    aDehazeAttr_t stAuto;
    mDehazeAttr_t stManual;
    DehazeManuAttr_t stDehazeManu;
    EnhanceManuAttr_t stEnhanceManu;
} adehaze_sw_V2_t;

#endif /*__ADEHAZE_UAPI_COMPACT_H__*/
