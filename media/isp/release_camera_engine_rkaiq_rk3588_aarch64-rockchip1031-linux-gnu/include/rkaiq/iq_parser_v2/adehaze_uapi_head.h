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

#ifndef __ADEHAZE_UAPI_HEAD_H__
#define __ADEHAZE_UAPI_HEAD_H__

#include "adehaze_head.h"

typedef struct mDehazeDataV21_s {
    // M4_NUMBER_DESC("dc_min_th", "f32", M4_RANGE(0,255), "64", M4_DIGIT(4))
    float dc_min_th;
    // M4_NUMBER_DESC("dc_max_th", "f32", M4_RANGE(0,255), "192", M4_DIGIT(4))
    float dc_max_th;
    // M4_NUMBER_DESC("yhist_th", "f32", M4_RANGE(0,255), "249", M4_DIGIT(4))
    float yhist_th;
    // M4_NUMBER_DESC("yblk_th", "f32", M4_RANGE(0,512), "0.002", M4_DIGIT(4))
    float yblk_th;
    // M4_NUMBER_DESC("dark_th", "f32", M4_RANGE(0,255), "250", M4_DIGIT(4))
    float dark_th;
    // M4_NUMBER_DESC("bright_min", "f32", M4_RANGE(0,255), "180", M4_DIGIT(4))
    float bright_min;
    // M4_NUMBER_DESC("bright_max", "f32", M4_RANGE(0,255), "240", M4_DIGIT(4))
    float bright_max;
    // M4_NUMBER_DESC("wt_max", "f32", M4_RANGE(0,1), "0.9", M4_DIGIT(4))
    float wt_max;
    // M4_NUMBER_DESC("air_min", "f32", M4_RANGE(0,255), "200", M4_DIGIT(4))
    float air_min;
    // M4_NUMBER_DESC("air_max", "f32", M4_RANGE(0,255), "250", M4_DIGIT(4))
    float air_max;
    // M4_NUMBER_DESC("tmax_base", "f32", M4_RANGE(0,255), "125", M4_DIGIT(4))
    float tmax_base;
    // M4_NUMBER_DESC("tmax_off", "f32", M4_RANGE(0,1), "0.1", M4_DIGIT(4))
    float tmax_off;
    // M4_NUMBER_DESC("tmax_max", "f32", M4_RANGE(0,1), "0.8", M4_DIGIT(4))
    float tmax_max;
    // M4_NUMBER_DESC("cfg_wt", "f32", M4_RANGE(0,1), "0.8", M4_DIGIT(4))
    float cfg_wt;
    // M4_NUMBER_DESC("cfg_air", "f32", M4_RANGE(0,255), "210", M4_DIGIT(4))
    float cfg_air;
    // M4_NUMBER_DESC("cfg_tmax", "f32", M4_RANGE(0,1), "0.2", M4_DIGIT(4))
    float cfg_tmax;
    // M4_NUMBER_DESC("dc_weitcur", "f32", M4_RANGE(0,1), "1", M4_DIGIT(4))
    float dc_weitcur;
    // M4_NUMBER_DESC("bf_weight", "f32", M4_RANGE(0,1), "0.5", M4_DIGIT(4))
    float bf_weight;
    // M4_NUMBER_DESC("range_sigma", "f32", M4_RANGE(0,1), "0.04", M4_DIGIT(4))
    float range_sigma;
    // M4_NUMBER_DESC("space_sigma_pre", "f32", M4_RANGE(0,1), "0.4", M4_DIGIT(4))
    float space_sigma_pre;
    // M4_NUMBER_DESC("space_sigma_cur", "f32", M4_RANGE(0,1), "0.8", M4_DIGIT(4))
    float space_sigma_cur;
} mDehazeDataV21_t;

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

typedef struct mEnhanceDataV21_s {
    // M4_NUMBER_DESC("enhance_value", "f32", M4_RANGE(0,32), "1", M4_DIGIT(4))
    float enhance_value;
    // M4_NUMBER_DESC("enhance_chroma", "f32", M4_RANGE(0,32), "1", M4_DIGIT(4))
    float enhance_chroma;
} mEnhanceDataV21_t;

typedef struct mEnhance_Setting_V21_s {
    // M4_BOOL_DESC("en", "1")
    bool en;
    // M4_ARRAY_DESC("enhance_curve", "f32", M4_SIZE(1,17),  M4_RANGE(0, 1024), "[0,64,128,192,256,320,384,448,512,576,640,704,768,832,896,960,1023]", M4_DIGIT(0), M4_DYNAMIC(0))
    float enhance_curve[CALIBDB_ADEHAZE_ENHANCE_CURVE_KNOTS_NUM];
    // M4_STRUCT_DESC("EnhanceData", "normal_ui_style")
    mEnhanceDataV21_t EnhanceData;
} mEnhance_Setting_V21_t;

typedef struct mHistDataV21_s {
    // M4_NUMBER_DESC("hist_gratio", "f32", M4_RANGE(0,32), "4", M4_DIGIT(4))
    float hist_gratio;
    // M4_NUMBER_DESC("hist_th_off", "f32", M4_RANGE(0,255), "64", M4_DIGIT(4))
    float hist_th_off;
    // M4_NUMBER_DESC("hist_k", "f32", M4_RANGE(0,8), "2", M4_DIGIT(4))
    float hist_k;
    // M4_NUMBER_DESC("hist_min", "f32", M4_RANGE(0,2), "0.016", M4_DIGIT(4))
    float hist_min;
    // M4_NUMBER_DESC("hist_scale", "f32", M4_RANGE(0,32), "0.09", M4_DIGIT(4))
    float hist_scale;
    // M4_NUMBER_DESC("cfg_gratio", "f32", M4_RANGE(0,32), "2", M4_DIGIT(4))
    float cfg_gratio;
} mHistDataV21_t;

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

typedef struct aDehazeDataV21_s {
    // M4_ARRAY_DESC("CtrlData", "f32", M4_SIZE(1,13), M4_RANGE(0,10000000), "[0, 0.005, 0.01, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 1]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float CtrlData[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("dc_min_th", "f32", M4_SIZE(1,13), M4_RANGE(0,255), "[64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float dc_min_th[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("dc_max_th", "f32", M4_SIZE(1,13), M4_RANGE(0,255), "[192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float dc_max_th[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("yhist_th", "f32", M4_SIZE(1,13), M4_RANGE(0,255), "[249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float yhist_th[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("yblk_th", "f32", M4_SIZE(1,13), M4_RANGE(0,512), "[0.002, 0.002, 0.002, 0.002, 0.002, 0.002, 0.002, 0.002, 0.002, 0.002, 0.002, 0.002, 0.002]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float yblk_th[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("dark_th", "f32", M4_SIZE(1,13), M4_RANGE(0,255), "[250, 250, 250, 250, 250, 250, 250, 250, 250 250, 250, 250, 250]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float dark_th[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("bright_min", "f32", M4_SIZE(1,13), M4_RANGE(0,255), "[180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float bright_min[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("bright_max", "f32", M4_SIZE(1,13), M4_RANGE(0,255), "[240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float bright_max[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("wt_max", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float wt_max[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("air_min", "f32", M4_SIZE(1,13), M4_RANGE(0,255), "[200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float air_min[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("air_max", "f32", M4_SIZE(1,13), M4_RANGE(0,255), "[250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float air_max[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("tmax_base", "f32", M4_SIZE(1,13), M4_RANGE(0,255), "[125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float tmax_base[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("tmax_off", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float tmax_off[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("tmax_max", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float tmax_max[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("cfg_wt", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float cfg_wt[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("cfg_air", "f32", M4_SIZE(1,13), M4_RANGE(0,255), "[210, 210, 210, 210, 210, 210, 210, 210, 210, 210, 210, 210, 210]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float cfg_air[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("cfg_tmax", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float cfg_tmax[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("dc_weitcur", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float dc_weitcur[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("bf_weight", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float bf_weight[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("range_sigma", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0.14, 0.14, 0.14, 0.14, 0.14, 0.14, 0.14, 0.14, 0.14, 0.14, 0.14, 0.14, 0.14]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float range_sigma[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("space_sigma_pre", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0.14, 0.14, 0.14, 0.14, 0.14, 0.14, 0.14, 0.14, 0.14, 0.14, 0.14, 0.14, 0.14]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float space_sigma_pre[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("space_sigma_cur", "f32", M4_SIZE(1,13), M4_RANGE(0,1), "[0.14, 0.14, 0.14, 0.14, 0.14, 0.14, 0.14, 0.14, 0.14, 0.14, 0.14, 0.14, 0.14]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float space_sigma_cur[DHAZ_CTRL_DATA_STEP_MAX];
} aDehazeDataV21_t;

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

typedef struct aEnhanceDataV21_s {
    // M4_ARRAY_DESC("CtrlData", "f32", M4_SIZE(1,13), M4_RANGE(0,10000000), "[0, 0.005, 0.01, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 1]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float CtrlData[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("enhance_value", "f32", M4_SIZE(1,13), M4_RANGE(1,16), "[1,1,1,1,1,1,1,1,1,1,1,1,1]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float enhance_value[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("enhance_chroma", "f32", M4_SIZE(1,13), M4_RANGE(1,16), "[1,1,1,1,1,1,1,1,1,1,1,1,1]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float enhance_chroma[DHAZ_CTRL_DATA_STEP_MAX];
} aEnhanceDataV21_t;

typedef struct aEnhance_Setting_V21_s {
    // M4_BOOL_DESC("en", "1")
    bool en;
    // M4_ARRAY_DESC("enhance_curve", "f32", M4_SIZE(1,17),  M4_RANGE(0, 1024), "[0,64,128,192,256,320,384,448,512,576,640,704,768,832,896,960,1023]", M4_DIGIT(0), M4_DYNAMIC(0))
    float enhance_curve[CALIBDB_ADEHAZE_ENHANCE_CURVE_KNOTS_NUM];
    // M4_STRUCT_DESC("EnhanceData", "normal_ui_style")
    aEnhanceDataV21_t EnhanceData;
} aEnhance_Setting_V21_t;

typedef struct aHistDataV21_s {
    // M4_ARRAY_DESC("CtrlData", "f32", M4_SIZE(1,13), M4_RANGE(0,10000000), "[0, 0.005, 0.01, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 1]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float CtrlData[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("hist_gratio", "f32", M4_SIZE(1,13), M4_RANGE(0,32), "[2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float hist_gratio[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("hist_th_off", "f32", M4_SIZE(1,13), M4_RANGE(0,255), "[64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float hist_th_off[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("hist_k", "f32", M4_SIZE(1,13), M4_RANGE(0,8), "[2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float hist_k[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("hist_min", "f32", M4_SIZE(1,13), M4_RANGE(0,2), "[0.015, 0.015, 0.015, 0.015, 0.015, 0.015, 0.015, 0.015, 0.015, 0.015, 0.015, 0.015, 0.015]",M4_DIGIT(4), M4_DYNAMIC(0))
    float hist_min[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("hist_scale", "f32", M4_SIZE(1,13), M4_RANGE(0,32), "[0.09, 0.09, 0.09, 0.09, 0.09, 0.09, 0.09, 0.09, 0.09, 0.09, 0.09, 0.09, 0.09]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float hist_scale[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("cfg_gratio", "f32", M4_SIZE(1,13), M4_RANGE(0,32), "[2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float cfg_gratio[DHAZ_CTRL_DATA_STEP_MAX];
} aHistDataV21_t;

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

#endif /*__ADEHAZE_UAPI_HEAD_H__*/
