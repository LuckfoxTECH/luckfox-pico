/*
 * adehaze_head.h
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

#ifndef __CALIBDBV2_ADEHAZE_HEAD_H__
#define __CALIBDBV2_ADEHAZE_HEAD_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

#define DHAZ_ENHANCE_CURVE_KNOTS_NUM 17
#define DHAZ_V11_SIGMA_IDX_NUM       15
#define DHAZ_V11_SIGMA_LUT_NUM       17
#define DHAZ_CTRL_DATA_STEP_MAX      13
#define DHAZ_V12_HIST_WR_CURVE_NUM   17

// dehaze v10
typedef enum CtrlDataType_e {
    CTRLDATATYPE_ENVLV   = 0,
    CTRLDATATYPE_ISO     = 1,
} CtrlDataType_t;

typedef struct DehazeData_s {
    // M4_ARRAY_DESC("CtrlData", "f32", M4_SIZE(1,100), M4_RANGE(0,10000000), "50",M4_DIGIT(4), M4_DYNAMIC(1))
    float* CtrlData;
    int CtrlData_len;
    // M4_ARRAY_DESC("dc_min_th", "f32", M4_SIZE(1,100), M4_RANGE(0,255), "64",M4_DIGIT(4), M4_DYNAMIC(1))
    float* dc_min_th;
    int dc_min_th_len;
    // M4_ARRAY_DESC("dc_max_th", "f32", M4_SIZE(1,100), M4_RANGE(0,255), "192",M4_DIGIT(4), M4_DYNAMIC(1))
    float* dc_max_th;
    int dc_max_th_len;
    // M4_ARRAY_DESC("yhist_th", "f32", M4_SIZE(1,100), M4_RANGE(0,255), "249",M4_DIGIT(4), M4_DYNAMIC(1))
    float* yhist_th;
    int yhist_th_len;
    // M4_ARRAY_DESC("yblk_th", "f32", M4_SIZE(1,100), M4_RANGE(0,512), "0.002",M4_DIGIT(4), M4_DYNAMIC(1))
    float* yblk_th;
    int yblk_th_len;
    // M4_ARRAY_DESC("dark_th", "f32", M4_SIZE(1,100), M4_RANGE(0,255), "250",M4_DIGIT(4), M4_DYNAMIC(1))
    float* dark_th;
    int dark_th_len;
    // M4_ARRAY_DESC("bright_min", "f32", M4_SIZE(1,100), M4_RANGE(0,255), "180",M4_DIGIT(4), M4_DYNAMIC(1))
    float* bright_min;
    int bright_min_len;
    // M4_ARRAY_DESC("bright_max", "f32", M4_SIZE(1,100), M4_RANGE(0,255), "240",M4_DIGIT(4), M4_DYNAMIC(1))
    float* bright_max;
    int bright_max_len;
    // M4_ARRAY_DESC("wt_max", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0.9",M4_DIGIT(4), M4_DYNAMIC(1))
    float* wt_max;
    int wt_max_len;
    // M4_ARRAY_DESC("air_min", "f32", M4_SIZE(1,100), M4_RANGE(0,255), "200",M4_DIGIT(4), M4_DYNAMIC(1))
    float* air_min;
    int air_min_len;
    // M4_ARRAY_DESC("air_max", "f32", M4_SIZE(1,100), M4_RANGE(0,255), "250",M4_DIGIT(4), M4_DYNAMIC(1))
    float* air_max;
    int air_max_len;
    // M4_ARRAY_DESC("tmax_base", "f32", M4_SIZE(1,100), M4_RANGE(0,255), "125",M4_DIGIT(4), M4_DYNAMIC(1))
    float* tmax_base;
    int tmax_base_len;
    // M4_ARRAY_DESC("tmax_off", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0.1",M4_DIGIT(4), M4_DYNAMIC(1))
    float* tmax_off;
    int tmax_off_len;
    // M4_ARRAY_DESC("tmax_max", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0.8",M4_DIGIT(4), M4_DYNAMIC(1))
    float* tmax_max;
    int tmax_max_len;
    // M4_ARRAY_DESC("cfg_wt", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0.8",M4_DIGIT(4), M4_DYNAMIC(1))
    float* cfg_wt;
    int cfg_wt_len;
    // M4_ARRAY_DESC("cfg_air", "f32", M4_SIZE(1,100), M4_RANGE(0,255), "210",M4_DIGIT(4), M4_DYNAMIC(1))
    float* cfg_air;
    int cfg_air_len;
    // M4_ARRAY_DESC("cfg_tmax", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0.2",M4_DIGIT(4), M4_DYNAMIC(1))
    float* cfg_tmax;
    int cfg_tmax_len;
    // M4_ARRAY_DESC("dc_thed", "f32", M4_SIZE(1,100), M4_RANGE(0,255), "2",M4_DIGIT(4), M4_DYNAMIC(1))
    float* dc_thed;
    int dc_thed_len;
    // M4_ARRAY_DESC("dc_weitcur", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "1",M4_DIGIT(4), M4_DYNAMIC(1))
    float* dc_weitcur;
    int dc_weitcur_len;
    // M4_ARRAY_DESC("air_thed", "f32", M4_SIZE(1,100), M4_RANGE(0,255), "2",M4_DIGIT(4), M4_DYNAMIC(1))
    float* air_thed;
    int air_thed_len;
    // M4_ARRAY_DESC("air_weitcur", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0.14",M4_DIGIT(4), M4_DYNAMIC(1))
    float* air_weitcur;
    int air_weitcur_len;
} DehazeData_t;

typedef struct Dehaze_Setting_V10_s {
    // M4_BOOL_DESC("en", "1")
    bool en;
    // M4_NUMBER_DESC("stab_fnum", "f32", M4_RANGE(0,31), "8", M4_DIGIT(0))
    float stab_fnum;
    // M4_NUMBER_DESC("sigma", "f32", M4_RANGE(1,255), "255", M4_DIGIT(0))
    float sigma;
    // M4_NUMBER_DESC("wt_sigma", "f32", M4_RANGE(0,256), "8.0", M4_DIGIT(4))
    float wt_sigma;
    // M4_NUMBER_DESC("air_sigma", "f32", M4_RANGE(4,1024), "120.0", M4_DIGIT(4))
    float air_sigma;
    // M4_NUMBER_DESC("tmax_sigma", "f32", M4_RANGE(0.0004,1), "0.0100", M4_DIGIT(4))
    float tmax_sigma;
    // M4_ARRAY_TABLE_DESC("DehazeData", "array_table_ui", "none")
    DehazeData_t DehazeData;
} Dehaze_Setting_V10_t;

typedef struct EnhanceData_s {
    // M4_ARRAY_DESC("CtrlData", "f32", M4_SIZE(1,100), M4_RANGE(0,10000000), "50",M4_DIGIT(4), M4_DYNAMIC(1))
    float* CtrlData;
    int CtrlData_len;
    // M4_ARRAY_DESC("enhance_value", "f32", M4_SIZE(1,100), M4_RANGE(1,16), "1",M4_DIGIT(4), M4_DYNAMIC(1))
    float* enhance_value;
    int enhance_value_len;
} EnhanceData_t;

typedef struct Enhance_Setting_V10_s {
    // M4_BOOL_DESC("en", "1")
    bool en;
    // M4_ARRAY_TABLE_DESC("EnhanceData", "array_table_ui", "none")
    EnhanceData_t EnhanceData;
} Enhance_Setting_V10_t;

typedef struct HistData_s {
    // M4_ARRAY_DESC("CtrlData", "f32", M4_SIZE(1,100), M4_RANGE(0,10000000), "50",M4_DIGIT(4), M4_DYNAMIC(1))
    float* CtrlData;
    int CtrlData_len;
    // M4_ARRAY_DESC("hist_gratio", "f32", M4_SIZE(1,100), M4_RANGE(0,32), "2",M4_DIGIT(4), M4_DYNAMIC(1))
    float* hist_gratio;
    int hist_gratio_len;
    // M4_ARRAY_DESC("hist_th_off", "f32", M4_SIZE(1,100), M4_RANGE(0,255), "64",M4_DIGIT(4), M4_DYNAMIC(1))
    float* hist_th_off;
    int hist_th_off_len;
    // M4_ARRAY_DESC("hist_k", "f32", M4_SIZE(1,100), M4_RANGE(0,8), "2",M4_DIGIT(4), M4_DYNAMIC(1))
    float* hist_k;
    int hist_k_len;
    // M4_ARRAY_DESC("hist_min", "f32", M4_SIZE(1,100), M4_RANGE(0,2), "0.016",M4_DIGIT(4), M4_DYNAMIC(1))
    float* hist_min;
    int hist_min_len;
    // M4_ARRAY_DESC("hist_scale", "f32", M4_SIZE(1,100), M4_RANGE(0,32), "0.09",M4_DIGIT(4), M4_DYNAMIC(1))
    float* hist_scale;
    int hist_scale_len;
    // M4_ARRAY_DESC("cfg_gratio", "f32", M4_SIZE(1,100), M4_RANGE(0,32), "2",M4_DIGIT(4), M4_DYNAMIC(1))
    float* cfg_gratio;
    int cfg_gratio_len;
} HistData_t;

typedef struct Hist_setting_V10_s {
    // M4_BOOL_DESC("en", "1")
    bool en;
    // M4_BOOL_DESC("hist_para_en", "1")
    bool  hist_para_en;
    // M4_BOOL_DESC("hist_channel", "1")
    bool  hist_channel;
    // M4_ARRAY_TABLE_DESC("HistData", "array_table_ui","none")
    HistData_t HistData;
} Hist_setting_V10_t;

typedef struct CalibDbDehazeV10_s {
    // M4_BOOL_DESC("Enable", "1")
    bool Enable;
    // M4_ENUM_DESC("CtrlDataType", "CtrlDataType_t", "CTRLDATATYPE_ENVLV")
    CtrlDataType_t CtrlDataType;
    // M4_NUMBER_DESC("cfg_alpha", "f32", M4_RANGE(0,1), "1", M4_DIGIT(4))
    float cfg_alpha;
    // M4_NUMBER_DESC("ByPassThr", "f32", M4_RANGE(0,1), "0", M4_DIGIT(4))
    float ByPassThr;
    // M4_STRUCT_DESC("dehaze_setting", "normal_ui_style")
    Dehaze_Setting_V10_t dehaze_setting;
    // M4_STRUCT_DESC("enhance_setting", "normal_ui_style")
    Enhance_Setting_V10_t enhance_setting;
    // M4_STRUCT_DESC("hist_setting", "normal_ui_style")
    Hist_setting_V10_t hist_setting;
} CalibDbDehazeV10_t;

typedef struct CalibDbV2_dehaze_v10_s {
    // M4_STRUCT_DESC("DehazeTuningPara", "normal_ui_style")
    CalibDbDehazeV10_t DehazeTuningPara;
} CalibDbV2_dehaze_v10_t;

// dehaze v11
typedef struct DehazeDataV11_s {
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
} DehazeDataV11_t;

typedef struct Dehaze_Setting_V11_s {
    // M4_BOOL_DESC("en", "1")
    bool en;
    // M4_BOOL_DESC("air_lc_en", "1")
    bool air_lc_en;
    // M4_NUMBER_DESC("stab_fnum", "f32", M4_RANGE(0,31), "8.0", M4_DIGIT(0))
    float stab_fnum;
    // M4_NUMBER_DESC("sigma", "f32", M4_RANGE(1,255), "6.0", M4_DIGIT(0))
    float sigma;
    // M4_NUMBER_DESC("wt_sigma", "f32", M4_RANGE(0,256), "8.0", M4_DIGIT(4))
    float wt_sigma;
    // M4_NUMBER_DESC("air_sigma", "f32", M4_RANGE(4,1024), "120.0", M4_DIGIT(4))
    float air_sigma;
    // M4_NUMBER_DESC("tmax_sigma", "f32", M4_RANGE(0.0004,1), "0.0100", M4_DIGIT(4))
    float tmax_sigma;
    // M4_NUMBER_DESC("pre_wet", "f32", M4_RANGE(0,16), "8", M4_DIGIT(0))
    float pre_wet;
    // M4_ARRAY_TABLE_DESC("DehazeData", "array_table_ui", "none")
    DehazeDataV11_t DehazeData;
} Dehaze_Setting_V11_t;

typedef struct EnhanceDataV11_s {
    // M4_ARRAY_DESC("CtrlData", "f32", M4_SIZE(1,13), M4_RANGE(0,10000000), "[0, 0.005, 0.01, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 1]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float CtrlData[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("enhance_value", "f32", M4_SIZE(1,13), M4_RANGE(1,16), "[1,1,1,1,1,1,1,1,1,1,1,1,1]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float enhance_value[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("enhance_chroma", "f32", M4_SIZE(1,13), M4_RANGE(1,16), "[1,1,1,1,1,1,1,1,1,1,1,1,1]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float enhance_chroma[DHAZ_CTRL_DATA_STEP_MAX];
} EnhanceDataV11_t;

typedef struct Enhance_Setting_V11_s {
    // M4_BOOL_DESC("en", "1")
    bool en;
    // M4_ARRAY_DESC("enhance_curve", "f32", M4_SIZE(1,17),  M4_RANGE(0, 1024), "[0,64,128,192,256,320,384,448,512,576,640,704,768,832,896,960,1023]", M4_DIGIT(0), M4_DYNAMIC(0))
    float enhance_curve[DHAZ_ENHANCE_CURVE_KNOTS_NUM];
    // M4_ARRAY_TABLE_DESC("EnhanceData", "array_table_ui", "none")
    EnhanceDataV11_t EnhanceData;
} Enhance_Setting_V11_t;

typedef struct HistDataV11_s {
    // M4_ARRAY_DESC("CtrlData", "f32", M4_SIZE(1,13), M4_RANGE(0,10000000), "[0, 0.005, 0.01, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 1]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float CtrlData[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("hist_gratio", "f32", M4_SIZE(1,13), M4_RANGE(0,32), "[2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float hist_gratio[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("hist_th_off", "f32", M4_SIZE(1,13), M4_RANGE(0,255), "[64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float hist_th_off[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("hist_k", "f32", M4_SIZE(1,13), M4_RANGE(0,8), "[2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float hist_k[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("hist_min", "f32", M4_SIZE(1,13), M4_RANGE(0,2), "[0.015, 0.015, 0.015, 0.015, 0.015, 0.015, 0.015, 0.015, 0.015, 0.015, 0.015, 0.015, 0.015]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float hist_min[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("hist_scale", "f32", M4_SIZE(1,13), M4_RANGE(0,32), "[0.09, 0.09, 0.09, 0.09, 0.09, 0.09, 0.09, 0.09, 0.09, 0.09, 0.09, 0.09, 0.09]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float hist_scale[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("cfg_gratio", "f32", M4_SIZE(1,13), M4_RANGE(0,32), "[2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float cfg_gratio[DHAZ_CTRL_DATA_STEP_MAX];
} HistDataV11_t;

typedef struct Hist_setting_V11_s {
    // M4_BOOL_DESC("en", "1")
    bool en;
    // M4_BOOL_DESC("hist_para_en", "1")
    bool hist_para_en;
    // M4_ARRAY_TABLE_DESC("HistData", "array_table_ui","none")
    HistDataV11_t HistData;
} Hist_setting_V11_t;

typedef struct CalibDbDehazeV11_s {
    // M4_BOOL_DESC("en", "1")
    bool Enable;
    // M4_ENUM_DESC("CtrlDataType", "CtrlDataType_t", "CTRLDATATYPE_ENVLV")
    CtrlDataType_t CtrlDataType;
    // M4_NUMBER_DESC("cfg_alpha", "f32", M4_RANGE(0,1), "1", M4_DIGIT(4))
    float cfg_alpha;
    // M4_NUMBER_DESC("ByPassThr", "f32", M4_RANGE(0,1), "0", M4_DIGIT(4))
    float ByPassThr;
    // M4_STRUCT_DESC("dehaze_setting", "normal_ui_style")
    Dehaze_Setting_V11_t dehaze_setting;
    // M4_STRUCT_DESC("enhance_setting", "normal_ui_style")
    Enhance_Setting_V11_t enhance_setting;
    // M4_STRUCT_DESC("hist_setting", "normal_ui_style")
    Hist_setting_V11_t hist_setting;
} CalibDbDehazeV11_t;

typedef struct CalibDbV2_dehaze_v11_s {
    // M4_STRUCT_DESC("DehazeTuningPara", "normal_ui_style")
    CalibDbDehazeV11_t DehazeTuningPara;
} CalibDbV2_dehaze_v11_t;

// dehaze v12
typedef struct EnhanceDataV12_s {
    // M4_NUMBER_MARK_DESC("CtrlData", "f32", M4_RANGE(0,1000000000), "0", M4_DIGIT(4), "index1")
    float CtrlData;
    // M4_ARRAY_MARK_DESC("enhance_curve", "f32", M4_SIZE(1,17),  M4_RANGE(0, 1023), "[0, 64, 128, 192, 256, 320, 384, 448, 512, 576, 640, 704, 768, 832, 896, 960, 1023]", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    float enhance_curve[DHAZ_ENHANCE_CURVE_KNOTS_NUM];
    // M4_ARRAY_MARK_DESC("enh_luma", "f32", M4_SIZE(1,17),  M4_RANGE(0, 16), "[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]", M4_DIGIT(2), M4_DYNAMIC(0), 0)
    float enh_luma[DHAZ_ENHANCE_CURVE_KNOTS_NUM];
    // M4_NUMBER_DESC("enhance_value", "f32", M4_RANGE(0,16), "1", M4_DIGIT(4))
    float enhance_value;
    // M4_NUMBER_DESC("enhance_chroma", "f32", M4_RANGE(0,16), "1", M4_DIGIT(4))
    float enhance_chroma;
} EnhanceDataV12_t;

typedef struct Enhance_Setting_V12_s {
    // M4_BOOL_DESC("en", "1")
    bool en;
    // M4_BOOL_DESC("color_deviate_en", "0")
    bool color_deviate_en;
    // M4_BOOL_DESC("enh_luma_en", "0")
    bool enh_luma_en;
    // M4_STRUCT_LIST_DESC("EnhanceData", M4_SIZE(1,13), "normal_ui_style")
    EnhanceDataV12_t EnhanceData[DHAZ_CTRL_DATA_STEP_MAX];
} Enhance_Setting_V12_t;

typedef enum HistWrMode_e {
    HIST_WR_AUTO     = 0,
    HIST_WR_MANUAL   = 1,
    HIST_WR_SEMIAUTO = 2,
} HistWrMode_t;

typedef struct Manual_curve_s {
    // M4_NUMBER_MARK_DESC("CtrlData", "f32", M4_RANGE(0,1000000000), "0", M4_DIGIT(4), "index1")
    float CtrlData;
    // M4_ARRAY_MARK_DESC("curve_x", "u32", M4_SIZE(1,17),  M4_RANGE(0, 1023), "[0, 64, 128, 192, 256, 320, 384, 448, 512, 576, 640, 704, 768, 832, 896, 960, 1023]", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    int curve_x[DHAZ_V12_HIST_WR_CURVE_NUM];
    // M4_ARRAY_MARK_DESC("curve_y", "u32", M4_SIZE(1,17),  M4_RANGE(0, 1023), "[0, 64, 128, 192, 256, 320, 384, 448, 512, 576, 640, 704, 768, 832, 896, 960, 1023]", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    int curve_y[DHAZ_V12_HIST_WR_CURVE_NUM];
} Manual_curve_t;

typedef struct hist_wr_semiauto_s {
    // M4_ARRAY_DESC("CtrlData", "f32", M4_SIZE(1,13), M4_RANGE(0,10000000), "[0, 0.005, 0.01, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 1]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float CtrlData[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("clim0", "f32", M4_SIZE(1,13), M4_RANGE(0.1,32), "[1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float clim0[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("clim1", "f32", M4_SIZE(1,13), M4_RANGE(0.1,32), "[1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]",M4_DIGIT(4), M4_DYNAMIC(0), 0)
    float clim1[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_DESC("dark_th", "f32", M4_SIZE(1,13), M4_RANGE(0,1023), "[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]",M4_DIGIT(0), M4_DYNAMIC(0), 0)
    float dark_th[DHAZ_CTRL_DATA_STEP_MAX];
} hist_wr_semiauto_t;

typedef struct HistWr_s {
    // M4_ENUM_DESC("mode", "HistWrMode_t", "HIST_WR_MANUAL")
    HistWrMode_t mode;
    // M4_STRUCT_LIST_DESC("manual_curve", M4_SIZE(1,13), "normal_ui_style")
    Manual_curve_t manual_curve[DHAZ_CTRL_DATA_STEP_MAX];
    // M4_ARRAY_TABLE_DESC("semiauto_curve", "array_table_ui","none")
    hist_wr_semiauto_t semiauto_curve;
} HistWr_t;

typedef struct Hist_setting_V12_s {
    // M4_BOOL_DESC("en", "1")
    bool en;
    // M4_BOOL_DESC("hist_para_en", "1")
    bool hist_para_en;
    // M4_STRUCT_DESC("hist_wr", "normal_ui_style")
    HistWr_t hist_wr;
    // M4_ARRAY_TABLE_DESC("HistData", "array_table_ui","none")
    HistDataV11_t HistData;
} Hist_setting_V12_t;

typedef struct CalibDbDehazeV12_s {
    // M4_BOOL_DESC("en", "1")
    bool Enable;
    // M4_ENUM_DESC("CtrlDataType", "CtrlDataType_t", "CTRLDATATYPE_ENVLV")
    CtrlDataType_t CtrlDataType;
    // M4_NUMBER_DESC("cfg_alpha", "f32", M4_RANGE(0,1), "1", M4_DIGIT(4))
    float cfg_alpha;
    // M4_NUMBER_DESC("ByPassThr", "f32", M4_RANGE(0,1), "0", M4_DIGIT(4))
    float ByPassThr;
    // M4_STRUCT_DESC("dehaze_setting", "normal_ui_style")
    Dehaze_Setting_V11_t dehaze_setting;
    // M4_STRUCT_DESC("enhance_setting", "normal_ui_style")
    Enhance_Setting_V12_t enhance_setting;
    // M4_STRUCT_DESC("hist_setting", "normal_ui_style")
    Hist_setting_V12_t hist_setting;
} CalibDbDehazeV12_t;

typedef struct CalibDbV2_dehaze_v12_s {
    // M4_STRUCT_DESC("DehazeTuningPara", "normal_ui_style")
    CalibDbDehazeV12_t DehazeTuningPara;
} CalibDbV2_dehaze_v12_t;

RKAIQ_END_DECLARE

#endif
