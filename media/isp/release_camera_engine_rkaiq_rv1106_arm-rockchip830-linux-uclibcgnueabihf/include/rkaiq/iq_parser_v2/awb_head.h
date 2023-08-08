/*
 * awb_head.h
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

#ifndef __CALIBDBV2_AWB_HEADER_H__
#define __CALIBDBV2_AWB_HEADER_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

#define CALD_AWB_WINDOW_NUM_MAX 8
#define CALD_AWB_EXCRANGE_NUM_MAX 7
#define CALD_AWB_LV_NUM_MAX 16
#define CALD_AWB_ILLUMINATION_NAME       ( 20U )
#define CALD_AWB_GRID_NUM_TOTAL 225
#define CALD_AWB_RGCT_GRID_NUM 9
#define CALD_AWB_BGCRI_GRID_NUM 11

typedef enum CalibDbV2_Awb_Down_Scale_Mode_e {
    CALIB_AWB_DS_4X4 = 0,
    CALIB_AWB_DS_8X8 = 1,
} CalibDbV2_Awb_Down_Scale_Mode_t;

typedef enum CalibDbV2_Awb_Blk_Stat_V21_e {
    CALIB_AWB_BLK_STAT_MODE_AL_V201 = 0,
    CALIB_AWB_BLK_STAT_MODE_WP_V201 = 1,
} CalibDbV2_Awb_Blk_Stat_V21_t;

typedef enum CalibDbV2_Awb_Blk_Stat_V20_e {
    CALIB_AWB_BLK_STAT_MODE_AL_V200 = 1,
    CALIB_AWB_BLK_STAT_MODE_UV_V200 = 2,
    CALIB_AWB_BLK_STAT_MODE_XY_V200 = 3,
} CalibDbV2_Awb_Blk_Stat_V20_t;


typedef enum CalibDbV2_Awb_Ext_Range_Dom_e {
    CALIB_AWB_EXTRA_RANGE_DOMAIN_UV = 0,
    CALIB_AWB_EXTRA_RANGE_DOMAIN_XY = 1
} CalibDbV2_Awb_Ext_Range_Dom_t;

typedef enum CalibDbV2_Awb_Ext_Range_Mode_s {
    CALIB_AWB_EXCLUDE_WP_MODE = 1,
    CALIB_AWB_ETR_LIGHT_SOUR_MODE = 3,
} CalibDbV2_Awb_Ext_Range_Mode_t;

typedef struct CalibDbV2_ExtRange_s {
    // M4_ENUM_DESC("domain", "CalibDbV2_Awb_Ext_Range_Dom_t", "CALIB_AWB_EXTRA_RANGE_DOMAIN_UV")
    CalibDbV2_Awb_Ext_Range_Dom_t domain;/*0uv domain,1 xy domain*/
    // M4_ENUM_DESC("mode", "CalibDbV2_Awb_Ext_Range_Mode_t", "CALIB_AWB_EXCLUDE_WP_MODE")
    CalibDbV2_Awb_Ext_Range_Mode_t mode;
    // M4_ARRAY_DESC("region", "s32", M4_SIZE(1,4), M4_RANGE(-8192, 8191), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    int region[4];
} CalibDbV2_ExtRange_t;

typedef struct CalibDbV2_Yuv3D_Range_Ill_s {
    // M4_NUMBER_DESC("b_uv", "s32", M4_RANGE(-65536,65535), "0", M4_DIGIT(0), M4_HIDE(1))
    int b_uv;           //17+0 s
    // M4_NUMBER_DESC("slope_inv_neg_uv", "s32", M4_RANGE(-262144,262143), "0", M4_DIGIT(0))
    int slope_inv_neg_uv; //9+10s //-1/k
    // M4_NUMBER_DESC("slope_factor_uv", "s32", M4_RANGE(-16384,16383), "0", M4_DIGIT(0), M4_HIDE(1))
    int slope_factor_uv; //1+14s // 1/(-1/k+k)
    // M4_NUMBER_DESC("slope_ydis", "s32", M4_RANGE(-262144,262143), "0", M4_DIGIT(0), M4_HIDE(1))
    int slope_ydis;     //9+10s //k
    // M4_NUMBER_DESC("b_ydis", "s32", M4_RANGE(-65536,65535), "0", M4_DIGIT(0), M4_HIDE(1))
    int b_ydis;         //17+0s
    // M4_NUMBER_DESC("ref_v", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0), M4_HIDE(1))
    unsigned char ref_v;//8u
    // M4_ARRAY_DESC("dis", "u16", M4_SIZE(1,6), M4_RANGE(0,4095), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned short  dis[6];//8+4 u
    // M4_ARRAY_DESC("th", "u8", M4_SIZE(1,6), M4_RANGE(0,255), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char th[6]; //8u
} CalibDbV2_Yuv3D_Range_Ill_t;

typedef struct CalibDbV2_Yuv3D_2_Range_Ill_s {
    // M4_ARRAY_DESC("thcurve_u", "f32", M4_SIZE(1,6), M4_RANGE(0,255), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    float thcurve_u[6];
    // M4_ARRAY_DESC("thcure_th", "f32", M4_SIZE(1,6), M4_RANGE(0,8), "0", M4_DIGIT(4), M4_DYNAMIC(0))
    float thcure_th[6];
    // M4_ARRAY_DESC("lineVector", "f32", M4_SIZE(1,6), M4_RANGE(0,255), "0", M4_DIGIT(4), M4_DYNAMIC(0), M4_HIDE(1))
    float lineVector[6];
    // M4_NUMBER_DESC("disP1P2", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0), M4_HIDE(1))
    unsigned char disP1P2;
} CalibDbV2_Yuv3D_2_Range_Ill_t;

typedef struct CalibDbV2_Tcs_Range_Ill_s {
    // M4_ARRAY_DESC("normal", "f32", M4_SIZE(1,4), M4_RANGE(-8,8), "0", M4_DIGIT(4), M4_DYNAMIC(0))
    float normal[4];
    // M4_ARRAY_DESC("big", "f32", M4_SIZE(1,4), M4_RANGE(-8,8), "0", M4_DIGIT(4), M4_DYNAMIC(0))
    float big[4];
    // M4_ARRAY_DESC("small", "f32", M4_SIZE(1,4), M4_RANGE(-8,8), "0", M4_DIGIT(4), M4_DYNAMIC(0))
    float small[4];
} CalibDbV2_Tcs_Range_Ill_t;

typedef struct CalibDbV2_Tcs_Range_Ill2_s {
    // M4_ARRAY_DESC("normal", "f32", M4_SIZE(1,4), M4_RANGE(-8,8), "0", M4_DIGIT(4), M4_DYNAMIC(0))
    float normal[4];
    // M4_ARRAY_DESC("big", "f32", M4_SIZE(1,4), M4_RANGE(-8,8), "0", M4_DIGIT(4), M4_DYNAMIC(0))
    float big[4];
} CalibDbV2_Tcs_Range_Ill2_t;

typedef struct CalibDbV2_Uv_Range_Ill_s {
    // M4_ARRAY_DESC("u", "f32", M4_SIZE(1,4), M4_RANGE(0,255), "0", M4_DIGIT(1), M4_DYNAMIC(0))
    float  u[4];
    // M4_ARRAY_DESC("v", "f32", M4_SIZE(1,4), M4_RANGE(0,255), "0", M4_DIGIT(1), M4_DYNAMIC(0))
    float  v[4];
} CalibDbV2_Uv_Range_Ill_t;

typedef enum CalibDbV2_Awb_DoorType_e {
    CALIB_AWB_DOOR_TYPE_INDOOR = 1,
    CALIB_AWB_DOOR_TYPE_AMBIGUITY = 2,
    CALIB_AWB_DOOR_TYPE_OUTDOOR = 3,
} CalibDbV2_Awb_DoorType_t;

typedef struct CalibDbV2_Awb_Light_V20_s {
    // M4_STRING_DESC("name", M4_SIZE(1,1), M4_RANGE(0, 16), "default", M4_DYNAMIC(0))
    char* name;
    // M4_ENUM_DESC("doorType", "CalibDbV2_Awb_DoorType_t", "CALIB_AWB_DOOR_TYPE_AMBIGUITY")
    CalibDbV2_Awb_DoorType_t doorType;
    // M4_ARRAY_DESC("standardGainValue", "f32", M4_SIZE(1,4), M4_RANGE(0,4), "1.0", M4_DIGIT(4), M4_DYNAMIC(0))
    float standardGainValue[4];
    // M4_ARRAY_TABLE_DESC("uvRegion", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDbV2_Uv_Range_Ill_t uvRegion;
    // M4_ARRAY_TABLE_DESC("xyRegion", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDbV2_Tcs_Range_Ill_t xyRegion;
    // M4_STRUCT_DESC("yuvRegion", "normal_ui_style")
    CalibDbV2_Yuv3D_Range_Ill_t   yuvRegion;
    // M4_ARRAY_DESC("staWeight", "u8", M4_SIZE(1,16), M4_RANGE(0,100), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char staWeight[CALD_AWB_LV_NUM_MAX];
    // M4_ARRAY_DESC("dayGainLvThSet", "u32", M4_SIZE(1,2), M4_RANGE(0,255000), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned int dayGainLvThSet[2];
    // M4_ARRAY_DESC("defaultDayGainLow", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float defaultDayGainLow[4];//spatial gain
    // M4_ARRAY_DESC("defaultDayGainHigh", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float defaultDayGainHigh[4];
    // M4_BOOL_DESC("xyType2Enable", "1", M4_HIDE(1))
    bool  xyType2Enable;
} CalibDbV2_Awb_Light_V20_t;

typedef enum CalibDb_Window_Mode_e {
    CALIB_AWB_WINDOW_CFG_AUTO = 0,
    CALIB_AWB_WINDOW_CFG_FIXED,
} CalibDb_Window_Mode_t;

typedef struct CalibDbV2_StatWindow_s {
    // M4_ENUM_DESC("mode", "CALIB_AWB_WINDOW_CFG_AUTO", "CALIB_AWB_WINDOW_CFG_AUTO")
    CalibDb_Window_Mode_t mode;
    // M4_ARRAY_DESC("window", "f32", M4_SIZE(1,4), M4_RANGE(0,100), "[0,0,1,1]", M4_DIGIT(2), M4_DYNAMIC(0))
    float window[4];//percent 100% ,between[0,100]
} CalibDbV2_StatWindow_t;

typedef struct CalibDbV2_Awb_Cct_Clip_Cfg_s {
    // M4_BOOL_DESC("enable", "0")
    bool enable;
    // M4_ARRAY_DESC("cct", "f32", M4_SIZE(1,5), M4_RANGE(0,10000), "0", M4_DIGIT(0), M4_DYNAMIC(1))
    float* cct;
    int cct_len;
    // M4_ARRAY_DESC("cri_bound_up", "f32", M4_SIZE(1,5), M4_RANGE(0,1), "0", M4_DIGIT(4), M4_DYNAMIC(1))
    float* cri_bound_up;
    int cri_bound_up_len;
    // M4_ARRAY_DESC("cri_bound_low", "f32", M4_SIZE(1,5), M4_RANGE(0,1), "0", M4_DIGIT(4), M4_DYNAMIC(1))
    float* cri_bound_low;
    int cri_bound_low_len;
} CalibDbV2_Awb_Cct_Clip_Cfg_t;

typedef struct CalibDbV2_Awb_DaylgtClip_Cfg_s {
    // M4_BOOL_DESC("enable", "0")
    bool  enable;
    // M4_NUMBER_DESC("outdoor_cct_min", "f32", M4_RANGE(0,10000), "5000", M4_DIGIT(0))
    float outdoor_cct_min;
} CalibDbV2_Awb_DaylgtClip_Cfg_t;

typedef struct CalibDbV2_Awb_Limit_Range_s {
    // M4_ARRAY_DESC("lumaValue", "f32", M4_SIZE(1,5), M4_RANGE(0,255000), "0", M4_DIGIT(1), M4_DYNAMIC(1))
    float *lumaValue;
    int lumaValue_len;
    // M4_ARRAY_DESC("maxR", "u16", M4_SIZE(1,5), M4_RANGE(0,255), "230", M4_DIGIT(0), M4_DYNAMIC(1))
    unsigned short *maxR;
    int maxR_len;
    // M4_ARRAY_DESC("minR", "u16", M4_SIZE(1,5), M4_RANGE(0,255), "3", M4_DIGIT(0), M4_DYNAMIC(1))
    unsigned short *minR;
    int minR_len;
    // M4_ARRAY_DESC("maxG", "u16", M4_SIZE(1,5), M4_RANGE(0,255), "230", M4_DIGIT(0), M4_DYNAMIC(1))
    unsigned short *maxG;
    int maxG_len;
    // M4_ARRAY_DESC("minG", "u16", M4_SIZE(1,5), M4_RANGE(0,255), "3", M4_DIGIT(0), M4_DYNAMIC(1))
    unsigned short *minG;
    int minG_len;
    // M4_ARRAY_DESC("maxB", "u16", M4_SIZE(1,5), M4_RANGE(0,255), "230", M4_DIGIT(0), M4_DYNAMIC(1))
    unsigned short *maxB;
    int maxB_len;
    // M4_ARRAY_DESC("minB", "u16", M4_SIZE(1,5), M4_RANGE(0,255), "3", M4_DIGIT(0), M4_DYNAMIC(1))
    unsigned short *minB;
    int minB_len;
    // M4_ARRAY_DESC("maxY", "u16", M4_SIZE(1,5), M4_RANGE(0,255), "230", M4_DIGIT(0), M4_DYNAMIC(1))
    unsigned short *maxY;
    int maxY_len;
    // M4_ARRAY_DESC("minY", "u16", M4_SIZE(1,5), M4_RANGE(0,255), "3", M4_DIGIT(0), M4_DYNAMIC(1))
    unsigned short *minY;
    int minY_len;
} CalibDbV2_Awb_Limit_Range_t;

typedef struct CalibDbV2_Rgb2Tcs_s {
    // M4_ARRAY_DESC("pseudoLuminanceWeight", "f32", M4_SIZE(1,3), M4_RANGE(0,1), "0.3", M4_DIGIT(6), M4_DYNAMIC(0))
    float pseudoLuminanceWeight[3];
    // M4_ARRAY_DESC("rotationMat", "f32", M4_SIZE(3,3), M4_RANGE(0,1), "0.3", M4_DIGIT(6), M4_DYNAMIC(0))
    float rotationMat[9];
} CalibDbV2_Rgb2Tcs_t;

typedef struct CalibDbV2_Awb_Sgc_Cblk_s {
    // M4_NUMBER_DESC("index", "u16", M4_RANGE(1,24), "1", M4_DIGIT(0))
    unsigned short index;
    // M4_NUMBER_DESC("meanC", "f32", M4_RANGE(0,255), "0", M4_DIGIT(6))
    float meanC;
    // M4_NUMBER_DESC("meanH", "f32", M4_RANGE(-180,180), "0", M4_DIGIT(6))
    float meanH;
} CalibDbV2_Awb_Sgc_Cblk_t;

typedef struct CalibDbV2_Awb_Sgc_Ls_s {
    // M4_STRING_DESC("name", M4_SIZE(1,1), M4_RANGE(0, 16), "default", M4_DYNAMIC(0))
    char* name;
    // M4_NUMBER_DESC("RGain", "f32", M4_RANGE(0,8), "1", M4_DIGIT(6))
    float RGain;
    // M4_NUMBER_DESC("BGain", "f32", M4_RANGE(0,8), "1", M4_DIGIT(6))
    float BGain;
} CalibDbV2_Awb_Sgc_Ls_t;

typedef struct CalibDbV2_Awb_Sgc_s {
    // M4_BOOL_DESC("enable", "0")
    bool enable;
    // M4_STRUCT_LIST_DESC("colorBlock", M4_SIZE(1,24), "normal_ui_style")
    CalibDbV2_Awb_Sgc_Cblk_t *colorBlock;
    int colorBlock_len;
    // M4_STRUCT_LIST_DESC("lsUsedForEstimation", M4_SIZE(1,7), "normal_ui_style")
    CalibDbV2_Awb_Sgc_Ls_t *lsUsedForEstimation;
    int lsUsedForEstimation_len;
    // M4_NUMBER_DESC("alpha", "f32", M4_RANGE(0,1), "1", M4_DIGIT(4))
    float alpha;
} CalibDbV2_Awb_Sgc_t;

typedef enum CalibDbV2_Awb_Hdr_Fr_Ch_Mode_e {
    CALIB_AWB_HDR_FR_CH_MANUAL = 0,
    CALIB_AWB_HDR_FR_CH_AUTO
} CalibDbV2_Awb_Hdr_Fr_Ch_Mode_t;

typedef struct CalibDbV2_Awb_Hdr_Fr_Ch_s {
    // M4_ENUM_DESC("frameChooseMode", "CalibDbV2_Awb_Hdr_Fr_Ch_Mode_t", "CALIB_AWB_HDR_FR_CH_AUTO")
    CalibDbV2_Awb_Hdr_Fr_Ch_Mode_t frameChooseMode;
    // M4_NUMBER_DESC("frameChoose", "u8", M4_RANGE(0,2), "1", M4_DIGIT(0))
    unsigned char frameChoose;
} CalibDbV2_Awb_Hdr_Fr_Ch_t;

typedef enum CalibDbV2_Awb_Mul_Win_Mode_e {
    CALIB_AWB_WIN_USELESS = 0,
    CALIB_AWB_WIN_EXTRAWP = 1,
    CALIB_AWB_WIN_WEIGHTWP = 2,
} CalibDbV2_Awb_Mul_Win_Mode_t;

typedef struct CalibDbV2_Awb_Mul_Win_s {
    // M4_BOOL_DESC("enable", "1")
    bool  enable;
    // M4_ENUM_DESC("multiwindowMode", "CalibDbV2_Awb_Mul_Win_Mode_t", "CALIB_AWB_WIN_USELESS")
    CalibDbV2_Awb_Mul_Win_Mode_t multiwindowMode;
    // M4_ARRAY_DESC("window", "u16", M4_SIZE(8,4), M4_RANGE(0,8191), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    float window[CALD_AWB_WINDOW_NUM_MAX][4];//8  windows in pixel domain ,hOffset,vOffser,hSize,vSize;
    // M4_ARRAY_DESC("weight", "u16", M4_SIZE(8,4), M4_RANGE(0,10), "0", M4_DIGIT(4), M4_DYNAMIC(0))
    float weight;//weight for 8  windows;
} CalibDbV2_Awb_Mul_Win_t;

typedef struct CalibDbV2_Wb_Awb_Div_WpTh_s {
    //  M4_ARRAY_DESC("lumaValue", "f32", M4_SIZE(1,5), M4_RANGE(0,255000), "0", M4_DIGIT(1), M4_DYNAMIC(1))
    float *lumaValue;
    int lumaValue_len;
    // M4_ARRAY_DESC("low", "f32", M4_SIZE(1,5), M4_RANGE(0,100000), "0", M4_DIGIT(0), M4_DYNAMIC(1))
    float *low;
    int low_len;
    // M4_ARRAY_DESC("high", "f32", M4_SIZE(1,5), M4_RANGE(0,100000), "0", M4_DIGIT(0), M4_DYNAMIC(1))
    float *high;
    int high_len;
} CalibDbV2_Wb_Awb_Div_WpTh_t;

typedef struct CalibDbV2_Wb_Awb_Div_s {
    // M4_NUMBER_DESC("lumaValThLow", "u32", M4_RANGE(0, 255000), "0", M4_DIGIT(0))
    unsigned int lumaValThLow;
    // M4_NUMBER_DESC("lumaValThLow2", "u32", M4_RANGE(0, 255000), "0", M4_DIGIT(0))
    unsigned int lumaValThLow2;
    // M4_NUMBER_DESC("lumaValThHigh", "u32", M4_RANGE(0, 255000), "0", M4_DIGIT(0))
    unsigned int lumaValThHigh;
    // M4_NUMBER_DESC("lumaValThHigh2", "u32", M4_RANGE(0, 255000), "0", M4_DIGIT(0))
    unsigned int lumaValThHigh2;
    // M4_ARRAY_TABLE_DESC("wpNumTh", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDbV2_Wb_Awb_Div_WpTh_t wpNumTh;
} CalibDbV2_Wb_Awb_Div_t;

typedef struct CalibDbV2_Wb_Awb_Caga_s {
    // M4_BOOL_DESC("enable", "1")
    bool enable;
    // M4_ARRAY_DESC("targetGain", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float targetGain[4];//cagaTargetGain
    // M4_NUMBER_DESC("laCalcFactor", "f32", M4_RANGE(0, 255), "0", M4_DIGIT(4))
    float laCalcFactor;
} CalibDbV2_Wb_Awb_Caga_t;

typedef struct CalibDbV2_Wb_Awb_Prob_Calc_Dt_s {
    // M4_NUMBER_DESC("proDis_THH", "f32", M4_RANGE(0, 8), "0", M4_DIGIT(4))
    float proDis_THH; //threshold for distance probality calculation// rename to do
    // M4_NUMBER_DESC("proDis_THL", "f32", M4_RANGE(0, 8), "0", M4_DIGIT(4))
    float proDis_THL;
} CalibDbV2_Wb_Awb_Prob_Calc_Dt_t;

typedef struct CalibDbV2_Wb_Awb_Prob_Calc_Lv_s {
    // M4_NUMBER_DESC("outdoorLumaValThLow", "u32", M4_RANGE(0, 255000), "0", M4_DIGIT(0))
    unsigned int outdoorLumaValThLow;
    // M4_NUMBER_DESC("outdoorLumaValThHigh", "u32", M4_RANGE(0, 255000), "0", M4_DIGIT(0))
    unsigned int outdoorLumaValThHigh;// threshold for luminance probality calculation
} CalibDbV2_Wb_Awb_Prob_Calc_Lv_t;

typedef struct CalibDbV2_Wb_Awb_Prob_Calc_Wp_s {
    // M4_NUMBER_DESC("wpNumPercTh", "f32", M4_RANGE(0, 1), "0", M4_DIGIT(6))
    float  wpNumPercTh;
    // M4_NUMBER_DESC("wpNumPercTh2", "f32", M4_RANGE(0, 1), "0", M4_DIGIT(6))
    float  wpNumPercTh2;
} CalibDbV2_Wb_Awb_Prob_Calc_Wp_t;

typedef struct CalibDbV2_Wb_Awb_Convg_s {
    // M4_NUMBER_DESC("varThforUnDamp", "f32", M4_RANGE(0, 1), "0", M4_DIGIT(4))
    float  varThforUnDamp;
    // M4_NUMBER_DESC("varThforDamp", "f32", M4_RANGE(0, 1), "0", M4_DIGIT(4))
    float  varThforDamp;
} CalibDbV2_Wb_Awb_Convg_t;

typedef struct CalibDbV2_Awb_Tolerance_s {
    // M4_ARRAY_DESC("lumaValue", "f32", M4_SIZE(1,4), M4_RANGE(0,255000), "0", M4_DIGIT(0), M4_DYNAMIC(1))
    float* lumaValue;
    int lumaValue_len;
    // M4_ARRAY_DESC("toleranceValue", "f32", M4_SIZE(1,4), M4_RANGE(0,1), "0", M4_DIGIT(4), M4_DYNAMIC(1))
    float* toleranceValue;
    int toleranceValue_len;
}  CalibDbV2_Awb_Tolerance_t;

typedef struct CalibDbV2_Awb_runinterval_s {
    // M4_ARRAY_DESC("lumaValue", "f32", M4_SIZE(1,4), M4_RANGE(0,255000), "0", M4_DIGIT(0), M4_DYNAMIC(1))
    float *lumaValue;
    int lumaValue_len;
    // M4_ARRAY_DESC("intervalValue", "f32", M4_SIZE(1,4), M4_RANGE(0,255), "0", M4_DIGIT(0), M4_DYNAMIC(1))
    float *intervalValue;
    int intervalValue_len;
}  CalibDbV2_Awb_runinterval_t;

typedef struct CalibDbV2_Awb_DampFactor_s {
    // M4_NUMBER_DESC("dFStep", "f32", M4_RANGE(0, 1), "0", M4_DIGIT(4))
    float dFStep;
    // M4_NUMBER_DESC("dFMin", "f32", M4_RANGE(0, 1), "0", M4_DIGIT(4))
    float dFMin;
    // M4_NUMBER_DESC("dFMax", "f32", M4_RANGE(0, 1), "0", M4_DIGIT(4))
    float dFMax;
    // M4_NUMBER_DESC("lvIIRsize", "u32", M4_RANGE(0, 128), "0", M4_DIGIT(0))
    int lvIIRsize;
    // M4_NUMBER_DESC("lvVarTh", "f32", M4_RANGE(0, 100), "0", M4_DIGIT(4))
    float lvVarTh;
} CalibDbV2_Awb_DampFactor_t;

typedef struct CalibDbV2_Awb_Cct_Lut_Cfg_Lv_s {
    // M4_NUMBER_DESC("lumaValue", "f32", M4_RANGE(0, 255000), "0", M4_DIGIT(0))
    float lumaValue;
    // M4_NUMBER_DESC("ct_grid_num", "s32", M4_RANGE(0, 32), "0", M4_DIGIT(0))
    int ct_grid_num;
    // M4_NUMBER_DESC("cri_grid_num", "s32", M4_RANGE(0, 32), "0", M4_DIGIT(0))
    int cri_grid_num;
    // M4_ARRAY_DESC("ct_in_range", "f32", M4_SIZE(1,2), M4_RANGE(0,10000), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    float ct_in_range[2];//min,max, equal distance sapmle
    // M4_ARRAY_DESC("cri_in_range", "f32", M4_SIZE(1,2), M4_RANGE(-2,2), "0", M4_DIGIT(4), M4_DYNAMIC(0))
    float cri_in_range[2];//min,max
    // M4_ARRAY_DESC("ct_lut_out", "f32", M4_SIZE(9,7), M4_RANGE(0,10000), "0", M4_DIGIT(0), M4_DYNAMIC(1))
    float *ct_lut_out;
    int ct_lut_out_len;
    // M4_ARRAY_DESC("cri_lut_out", "f32", M4_SIZE(9,7), M4_RANGE(-2,2), "0", M4_DIGIT(4), M4_DYNAMIC(1))
    float *cri_lut_out;
    int cri_lut_out_len;
} CalibDbV2_Awb_Cct_Lut_Cfg_Lv_t;

typedef struct CalibDbV2_Awb_GainAdjust_s {
    // M4_BOOL_DESC("enable", "1")
    bool enable;
    // M4_STRUCT_LIST_DESC("lutAll", M4_SIZE(1,8), "normal_ui_style")
    CalibDbV2_Awb_Cct_Lut_Cfg_Lv_t *lutAll;
    int lutAll_len;
} CalibDbV2_Awb_GainAdjust_t;


typedef struct CalibDbV2_Awb_Cct_Lut_Cfg_Lv2_s {
    // M4_NUMBER_DESC("ctlData", "f32", M4_RANGE(0, 255000), "0", M4_DIGIT(1))
    float ctlData;
    // M4_ARRAY_DESC("rgct_in_ds", "f32", M4_SIZE(1,9), M4_RANGE(0,10000), "0", M4_DIGIT(4), M4_DYNAMIC(0))
    float rgct_in_ds[CALD_AWB_RGCT_GRID_NUM];//min,max
    // M4_ARRAY_DESC("bgcri_in_ds", "f32", M4_SIZE(1,11), M4_RANGE(-2,8), "0", M4_DIGIT(4), M4_DYNAMIC(0))
    float bgcri_in_ds[CALD_AWB_BGCRI_GRID_NUM];//max,min
    // M4_ARRAY_DESC("rgct_lut_out", "f32", M4_SIZE(11,9), M4_RANGE(0,10000), "0", M4_DIGIT(4), M4_DYNAMIC(0))
    float rgct_lut_out[CALD_AWB_RGCT_GRID_NUM*CALD_AWB_BGCRI_GRID_NUM];
    // M4_ARRAY_DESC("bgcri_lut_out", "f32", M4_SIZE(11,9), M4_RANGE(-2,8), "0", M4_DIGIT(4), M4_DYNAMIC(0))
    float bgcri_lut_out[CALD_AWB_RGCT_GRID_NUM*CALD_AWB_BGCRI_GRID_NUM];
} CalibDbV2_Awb_Cct_Lut_Cfg_Lv2_t;


typedef enum CalibDbV2_Awb_Ctrl_Dat_Selt_e{
    AWB_CTRL_DATA_ISO = 0,
    AWB_CTRL_DATA_LV = 1,
}CalibDbV2_Awb_Ctrl_Dat_Selt_t;

typedef enum CalibDbV2_Awb_Gain_Adj_Dat_Sl_e{
    AWB_GAIN_ADJ_DATA_GAIN = 0,
    AWB_GAIN_ADJ_DATA_CT = 1,
}CalibDbV2_Awb_Gain_Adj_Dat_Sl_t;

typedef struct CalibDbV2_Awb_GainAdjust2_s {
    // M4_BOOL_DESC("enable", "1")
    bool enable;
    // M4_ENUM_DESC("ctrlDataSelt", "CalibDbV2_Awb_Ctrl_Dat_Selt_t", "AWB_CTRL_DATA_LV")
    CalibDbV2_Awb_Ctrl_Dat_Selt_t ctrlDataSelt;
    // M4_ENUM_DESC("adjDataSelt", "CalibDbV2_Awb_Gain_Adj_Dat_Sl_t", "AWB_GAIN_ADJ_DATA_GAIN")
    CalibDbV2_Awb_Gain_Adj_Dat_Sl_t adjDataSelt;
    // M4_STRUCT_LIST_DESC("lutAll", M4_SIZE(1,8), "normal_ui_style")
    CalibDbV2_Awb_Cct_Lut_Cfg_Lv2_t *lutAll;
    int lutAll_len;
} CalibDbV2_Awb_GainAdjust2_t;


typedef struct CalibDbV2_Awb_Remosaic_Para_s {
    // M4_BOOL_DESC("enable", "1")
    bool enable;
    // M4_BOOL_DESC("applyInvWbGainEnable", "1")
    bool applyInvWbGainEnable;
    // M4_ARRAY_DESC("sensorWbGain", "f32", M4_SIZE(1,4), M4_RANGE(0,4), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float sensorWbGain[4];
} CalibDbV2_Awb_Remosaic_Para_t;

typedef struct CalibDbV2_Awb_xyRg_stb_WpTh_s {
    // M4_ARRAY_DESC("lumaValue", "f32", M4_SIZE(1,5), M4_RANGE(0,255000), "0", M4_DIGIT(1), M4_DYNAMIC(1))
    float *lumaValue;
    int lumaValue_len;
    // M4_ARRAY_DESC("forBigType", "f32", M4_SIZE(1,5), M4_RANGE(0,100000), "0", M4_DIGIT(0), M4_DYNAMIC(1))
    float *forBigType;
    int forBigType_len;
    // M4_ARRAY_DESC("forExtraType", "f32", M4_SIZE(1,5), M4_RANGE(0,100000), "0", M4_DIGIT(0), M4_DYNAMIC(1))
    float *forExtraType;
    int forExtraType_len;
} CalibDbV2_Awb_xyRg_stb_WpTh_t;

typedef struct CalibDbV2_Awb_xyRegion_stable_s {
    // M4_BOOL_DESC("enable", "1")
    bool enable;
    //  M4_ARRAY_TABLE_DESC("wpNumTh", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDbV2_Awb_xyRg_stb_WpTh_t wpNumTh;
    // M4_NUMBER_DESC("xyTypeListSize", "s32", M4_RANGE(0, 65535), "0", M4_DIGIT(0))
    int xyTypeListSize;// xyTypeListSize ==0 will disable this function
    // M4_NUMBER_DESC("varianceLumaTh", "f32", M4_RANGE(0, 100), "0", M4_DIGIT(4))
    float varianceLumaTh;
} CalibDbV2_Awb_xyRegion_stable_t;

typedef struct CalibDbV2_Awb_gain_offset_cfg_s {
    // M4_BOOL_DESC("enable", "1")
    bool enable;
    // M4_ARRAY_DESC("offset", "f32", M4_SIZE(1,4), M4_RANGE(-8,8), "0", M4_DIGIT(4), M4_DYNAMIC(0))
    float offset[4];//rggb
} CalibDbV2_Awb_gain_offset_cfg_t;

typedef struct CalibDbV2_Awb_SmartRun_cfg_s {
    // M4_ARRAY_DESC("lumaValue", "f32", M4_SIZE(1,5), M4_RANGE(0,255000), "0", M4_DIGIT(1), M4_DYNAMIC(1))
    float *lumaValue;
    int lumaValue_len;
    // M4_ARRAY_DESC("lvVarTh", "f32", M4_SIZE(1,5), M4_RANGE(0,255), "0", M4_DIGIT(4), M4_DYNAMIC(1))
    float *lvVarTh;
    int lvVarTh_len;
    // M4_ARRAY_DESC("wbgainAlgDiffTh", "f32", M4_SIZE(1,5), M4_RANGE(0,1), "0", M4_DIGIT(4), M4_DYNAMIC(1))
    float *wbgainAlgDiffTh;
    int wbgainAlgDiffTh_len;
    // M4_ARRAY_DESC("wbgainHwDiffTh", "f32", M4_SIZE(1,5), M4_RANGE(0,1), "0", M4_DIGIT(4), M4_DYNAMIC(1))
    float *wbgainHwDiffTh;
    int wbgainHwDiffTh_len;
} CalibDbV2_Awb_SmartRun_cfg_t;

typedef struct CalibDbV2_Awb_SmartRun_s {
    // M4_BOOL_DESC("enable", "1")
    bool  enable;
    // M4_ARRAY_TABLE_DESC("cfg", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDbV2_Awb_SmartRun_cfg_t cfg;
} CalibDbV2_Awb_SmartRun_t;

typedef struct CalibDbV2_Wb_Awb_Ext_Com_Para_t {
    // M4_STRING_DESC("lightSourceForFirstFrame", M4_SIZE(1,1), M4_RANGE(-128, 127), "default", M4_DYNAMIC(0))
    char *lightSourceForFirstFrame;
    // M4_ARRAY_TABLE_DESC("tolerance", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDbV2_Awb_Tolerance_t tolerance;//wb gain diff th for awb gain update, set 0 to disable this function
    // M4_ARRAY_TABLE_DESC("runInterval", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDbV2_Awb_runinterval_t runInterval;
    // M4_STRUCT_DESC("dampFactor", "normal_ui_style")
    CalibDbV2_Awb_DampFactor_t dampFactor;
    // M4_STRUCT_DESC("wbGainAdjust", "normal_ui_style")
    CalibDbV2_Awb_GainAdjust_t wbGainAdjust;
    // M4_STRUCT_DESC("wbGainDaylightClip", "normal_ui_style")
    CalibDbV2_Awb_DaylgtClip_Cfg_t wbGainDaylightClip;
    // M4_STRUCT_DESC("wbGainClip", "normal_ui_style")
    CalibDbV2_Awb_Cct_Clip_Cfg_t wbGainClip;
    // M4_STRUCT_DESC("division", "normal_ui_style")
    CalibDbV2_Wb_Awb_Div_t division;
    // M4_ARRAY_DESC("defaultNightGain", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float defaultNightGain[4];
    // M4_ARRAY_DESC("lumaValueMatrix", "u32", M4_SIZE(1,16), M4_RANGE(0,255000), "1", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned int lumaValueMatrix[CALD_AWB_LV_NUM_MAX];
    // M4_ARRAY_DESC("defaultNightGainWeight", "u8", M4_SIZE(1,16), M4_RANGE(0,100), "1", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char defaultNightGainWeight[CALD_AWB_LV_NUM_MAX];
    // M4_STRUCT_DESC("probCalcDis", "normal_ui_style")
    CalibDbV2_Wb_Awb_Prob_Calc_Dt_t probCalcDis;
    // M4_STRUCT_DESC("probCalcLv", "normal_ui_style")
    CalibDbV2_Wb_Awb_Prob_Calc_Lv_t probCalcLv;
    // M4_STRUCT_DESC("probCalcWp", "normal_ui_style")
    CalibDbV2_Wb_Awb_Prob_Calc_Wp_t probCalcWp;
    // M4_STRUCT_DESC("converged", "normal_ui_style")
    CalibDbV2_Wb_Awb_Convg_t converged;
    // M4_STRUCT_DESC("xyRegionStableSelection", "normal_ui_style")
    CalibDbV2_Awb_xyRegion_stable_t xyRegionStableSelection;
    // M4_ARRAY_DESC("weightForNightGainCalc", "u8", M4_SIZE(1,16), M4_RANGE(0,100), "1", M4_DIGIT(0), M4_DYNAMIC(1))
    unsigned char *weightForNightGainCalc;
    int weightForNightGainCalc_len;
    // M4_STRUCT_DESC("singleColorProces", "normal_ui_style")
    CalibDbV2_Awb_Sgc_t singleColorProces;
    // M4_ARRAY_DESC("lineRgBg", "f32", M4_SIZE(1,3), M4_RANGE(-2147483648,2147483647), "1", M4_DIGIT(4), M4_DYNAMIC(0), M4_HIDE(1))
    float lineRgBg[3];
    // M4_ARRAY_DESC("lineRgProjCCT", "f32", M4_SIZE(1,3), M4_RANGE(-2147483648,2147483647), "1", M4_DIGIT(4), M4_DYNAMIC(0), M4_HIDE(1))
    float lineRgProjCCT[3];
    // M4_STRUCT_DESC("chrAdpttAdj", "normal_ui_style", M4_HIDE(1))
    CalibDbV2_Wb_Awb_Caga_t chrAdpttAdj;
    // M4_STRUCT_DESC("remosaicCfg", "normal_ui_style")
    CalibDbV2_Awb_Remosaic_Para_t remosaicCfg;
    // M4_STRUCT_DESC("wbGainOffset", "normal_ui_style")
    CalibDbV2_Awb_gain_offset_cfg_t wbGainOffset;
} CalibDbV2_Wb_Awb_Ext_Com_Para_t;

typedef struct CalibDbV2_Wb_Awb_Ext_Para_V32_s {
    // M4_STRING_DESC("lightSourceForFirstFrame", M4_SIZE(1,1), M4_RANGE(-128, 127), "default", M4_DYNAMIC(0))
    char *lightSourceForFirstFrame;
    // M4_STRUCT_DESC("smartRun", "normal_ui_style")
    CalibDbV2_Awb_SmartRun_t smartRun;
    // M4_ARRAY_TABLE_DESC("tolerance", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDbV2_Awb_Tolerance_t tolerance;//wb gain diff th for awb gain update, set 0 to disable this function
    // M4_ARRAY_TABLE_DESC("runInterval", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDbV2_Awb_runinterval_t runInterval;
    // M4_STRUCT_DESC("dampFactor", "normal_ui_style")
    CalibDbV2_Awb_DampFactor_t dampFactor;
    // M4_STRUCT_DESC("wbGainAdjust", "normal_ui_style")
    CalibDbV2_Awb_GainAdjust2_t wbGainAdjust;
    // M4_STRUCT_DESC("wbGainDaylightClip", "normal_ui_style")
    CalibDbV2_Awb_DaylgtClip_Cfg_t wbGainDaylightClip;
    // M4_STRUCT_DESC("wbGainClip", "normal_ui_style")
    CalibDbV2_Awb_Cct_Clip_Cfg_t wbGainClip;
    // M4_STRUCT_DESC("division", "normal_ui_style")
    CalibDbV2_Wb_Awb_Div_t division;
    // M4_ARRAY_DESC("defaultNightGain", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float defaultNightGain[4];
    // M4_ARRAY_DESC("lumaValueMatrix", "u32", M4_SIZE(1,16), M4_RANGE(0,255000), "1", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned int lumaValueMatrix[CALD_AWB_LV_NUM_MAX];
    // M4_ARRAY_DESC("defaultNightGainWeight", "u8", M4_SIZE(1,16), M4_RANGE(0,100), "1", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char defaultNightGainWeight[CALD_AWB_LV_NUM_MAX];
    // M4_STRUCT_DESC("probCalcDis", "normal_ui_style")
    CalibDbV2_Wb_Awb_Prob_Calc_Dt_t probCalcDis;
    // M4_STRUCT_DESC("probCalcLv", "normal_ui_style")
    CalibDbV2_Wb_Awb_Prob_Calc_Lv_t probCalcLv;
    // M4_STRUCT_DESC("probCalcWp", "normal_ui_style")
    CalibDbV2_Wb_Awb_Prob_Calc_Wp_t probCalcWp;
    // M4_STRUCT_DESC("converged", "normal_ui_style")
    CalibDbV2_Wb_Awb_Convg_t converged;
    // M4_STRUCT_DESC("xyRegionStableSelection", "normal_ui_style")
    CalibDbV2_Awb_xyRegion_stable_t xyRegionStableSelection;
    // M4_ARRAY_DESC("weightForNightGainCalc", "u8", M4_SIZE(1,16), M4_RANGE(0,100), "1", M4_DIGIT(0), M4_DYNAMIC(1))
    unsigned char *weightForNightGainCalc;
    int weightForNightGainCalc_len;
    // M4_STRUCT_DESC("singleColorProces", "normal_ui_style")
    CalibDbV2_Awb_Sgc_t singleColorProces;
    // M4_ARRAY_DESC("lineRgBg", "f32", M4_SIZE(1,3), M4_RANGE(-2147483648,2147483647), "1", M4_DIGIT(4), M4_DYNAMIC(0), M4_HIDE(1))
    float lineRgBg[3];
    // M4_ARRAY_DESC("lineRgProjCCT", "f32", M4_SIZE(1,3), M4_RANGE(-2147483648,2147483647), "1", M4_DIGIT(4), M4_DYNAMIC(0), M4_HIDE(1))
    float lineRgProjCCT[3];
    // M4_STRUCT_DESC("remosaicCfg", "normal_ui_style")
    CalibDbV2_Awb_Remosaic_Para_t remosaicCfg;
    // M4_STRUCT_DESC("wbGainOffset", "normal_ui_style")
    CalibDbV2_Awb_gain_offset_cfg_t wbGainOffset;
} CalibDbV2_Wb_Awb_Ext_Para_V32_t;

typedef struct CalibDbV2_Wb_Awb_Para_V20_t {
    // M4_STRUCT_DESC("hdrPara", "normal_ui_style")
    CalibDbV2_Awb_Hdr_Fr_Ch_t hdrPara;
    // M4_BOOL_DESC("lscBypassEnable", "0");
    bool                lscBypassEnable;
    // M4_BOOL_DESC("uvDetectionEnable", "1");
    bool                uvDetectionEnable;
    // M4_BOOL_DESC("xyDetectionEnable", "1");
    bool                xyDetectionEnable;
    // M4_BOOL_DESC("yuvDetectionEnable", "1");
    bool                yuvDetectionEnable;
    // CXF CHECK
    // M4_STRING_DESC("lsUsedForYuvDet", M4_SIZE(1,7), M4_RANGE(-128,127), "light", M4_DYNAMIC(1), M4_HIDE(1));
    char**              lsUsedForYuvDet;
    int                 lsUsedForYuvDet_len;
    // M4_NUMBER_DESC("yuvDetRef_u", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0), M4_HIDE(1))
    unsigned char yuvDetRef_u;          //8u //cxf3333
    // M4_ENUM_DESC("downScaleMode", "CalibDbV2_Awb_Down_Scale_Mode_t","CALIB_AWB_DS_4X4");
    CalibDbV2_Awb_Down_Scale_Mode_t downScaleMode;
    // M4_ENUM_DESC("blkMeasureMode", "CalibDbV2_Awb_Blk_Stat_V20_t","CALIB_AWB_BLK_STAT_MODE_AL_V200");
    CalibDbV2_Awb_Blk_Stat_V20_t blkMeasureMode;
    // M4_STRUCT_DESC("mainWindow", "normal_ui_style")
    CalibDbV2_StatWindow_t mainWindow;
    // M4_ARRAY_TABLE_DESC("limitRange", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDbV2_Awb_Limit_Range_t limitRange;
    // M4_STRUCT_DESC("rgb2TcsPara", "normal_ui_style", M4_HIDE(1))
    CalibDbV2_Rgb2Tcs_t rgb2TcsPara;
    // M4_STRUCT_DESC("multiWindow", "normal_ui_style")
    CalibDbV2_Awb_Mul_Win_t  multiWindow;
    // M4_STRUCT_LIST_DESC("extraWpRange", M4_SIZE(1,7), "normal_ui_style")
    CalibDbV2_ExtRange_t extraWpRange[CALD_AWB_EXCRANGE_NUM_MAX];
    // M4_STRUCT_LIST_DESC("lightSources", M4_SIZE(1,7), "normal_ui_style")
    CalibDbV2_Awb_Light_V20_t* lightSources;
    int   lightSources_len;
} CalibDbV2_Wb_Awb_Para_V20_t;

typedef struct CalibDbV2_Awb_Lu_Wgt_En_Th_s {
    // M4_NUMBER_DESC("wpDiffWeiNoTh", "f32", M4_RANGE(0,1), "0", M4_DIGIT(4))
    float wpDiffWeiNoTh;
    // M4_NUMBER_DESC("wpDiffWeiLvValueTh", "u32", M4_RANGE(0,255000), "0", M4_DIGIT(0))
    unsigned int   wpDiffWeiLvValueTh;
} CalibDbV2_Awb_Lu_Wgt_En_Th_t;

typedef struct CalibDbV2_Awb_Lum_Wgt_Lv_Rto_s {
    // M4_NUMBER_DESC("ratioValue", "f32", M4_RANGE(0,1), "0", M4_DIGIT(4))
    float ratioValue;
    // M4_ARRAY_DESC("weight", "f32", M4_SIZE(1,9), M4_RANGE(0,1), "0", M4_DIGIT(4), M4_DYNAMIC(0))
    float weight[9] ;
} CalibDbV2_Awb_Lum_Wgt_Lv_Rto_t;

typedef struct CalibDbV2_Awb_Luma_Weight_Lv_s {
    // M4_NUMBER_DESC("LvValue", "f32", M4_RANGE(0,255000), "0", M4_DIGIT(0))
    float LvValue;
    // M4_STRUCT_LIST_DESC("ratioSet", M4_SIZE(1,4), "normal_ui_style")
    CalibDbV2_Awb_Lum_Wgt_Lv_Rto_t* ratioSet;
    int ratioSet_len;
} CalibDbV2_Awb_Luma_Weight_Lv_t;

typedef struct CalibDbV2_Awb_Luma_Weight_s {
    //with differernt luma ,the different weight in WP sum
    // M4_BOOL_DESC("enable", "0");
    bool enable;
    // M4_STRUCT_DESC("wpDiffWeiEnableTh", "normal_ui_style")
    CalibDbV2_Awb_Lu_Wgt_En_Th_t wpDiffWeiEnableTh;
    // M4_ARRAY_DESC("wpDiffwei_y", "u8", M4_SIZE(1,9), M4_RANGE(0,255), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char wpDiffwei_y[9];
    // M4_ARRAY_DESC("perfectBin", "u8", M4_SIZE(1,8), M4_RANGE(0,1), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char  perfectBin[8];// true means the luma is appropriate
    // M4_STRUCT_LIST_DESC("wpDiffWeightLvSet", M4_SIZE(1,4), "normal_ui_style")
    CalibDbV2_Awb_Luma_Weight_Lv_t* wpDiffWeightLvSet;
    int wpDiffWeightLvSet_len;
} CalibDbV2_Awb_Luma_Weight_t;

typedef struct CalibDbV2_Awb_Light_V21_s {
    // M4_STRING_DESC("name", M4_SIZE(1,1), M4_RANGE(0, 32), "default", M4_DYNAMIC(0))
    char* name;
    // M4_ENUM_DESC("doorType", "CalibDbV2_Awb_DoorType_t", "CALIB_AWB_DOOR_TYPE_AMBIGUITY")
    CalibDbV2_Awb_DoorType_t doorType;
    // M4_ARRAY_DESC("standardGainValue", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "0", M4_DIGIT(4), M4_DYNAMIC(0))
    float standardGainValue[4];
    // M4_ARRAY_TABLE_DESC("uvRegion", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDbV2_Uv_Range_Ill_t uvRegion;
    // M4_ARRAY_TABLE_DESC("xyRegion", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDbV2_Tcs_Range_Ill2_t xyRegion;
    // M4_STRUCT_DESC("rtYuvRegion", "normal_ui_style")
    CalibDbV2_Yuv3D_2_Range_Ill_t rtYuvRegion;
    // M4_ARRAY_DESC("staWeight", "u8", M4_SIZE(1,16), M4_RANGE(0,100), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char staWeight[CALD_AWB_LV_NUM_MAX];
    // M4_ARRAY_DESC("dayGainLvThSet", "u8", M4_SIZE(1,2), M4_RANGE(0,255000), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned int dayGainLvThSet[2];
    // M4_ARRAY_DESC("defaultDayGainLow", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float defaultDayGainLow[4];//spatial gain
    // M4_ARRAY_DESC("defaultDayGainHigh", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float defaultDayGainHigh[4];
    // M4_NUMBER_DESC("xyType2Enable", "u8", M4_RANGE(0,1), "1", M4_DIGIT(0), M4_HIDE(1))
    unsigned char xyType2Enable;
} CalibDbV2_Awb_Light_V21_t;

typedef struct CalibDbV2_Wb_Awb_Para_V21_t {
    // M4_STRUCT_DESC("hdrPara", "normal_ui_style")
    CalibDbV2_Awb_Hdr_Fr_Ch_t hdrPara;//hdrFrameChoose;
    // M4_BOOL_DESC("lscBypassEnable", "0");
    bool                lscBypassEnable;
    // M4_BOOL_DESC("uvDetectionEnable", "0");
    bool                uvDetectionEnable;
    // M4_BOOL_DESC("xyDetectionEnable", "0");
    bool                xyDetectionEnable;
    // M4_BOOL_DESC("yuvDetectionEnable", "0");
    bool                yuvDetectionEnable;
    // M4_STRING_DESC("lsUsedForYuvDet", M4_SIZE(1,7), M4_RANGE(0, 16), "default", M4_DYNAMIC(1), M4_HIDE(1))
    char** lsUsedForYuvDet;
    int                 lsUsedForYuvDet_len;
    // M4_BOOL_DESC("blkStatisticsEnable", "0");
    bool                blkStatisticsEnable;
    // M4_ENUM_DESC("downScaleMode", "CalibDbV2_Awb_Down_Scale_Mode_t", "CALIB_AWB_DS_8X8")
    CalibDbV2_Awb_Down_Scale_Mode_t       downScaleMode;
    // M4_ENUM_DESC("blkMeasureMode", "CalibDbV2_Awb_Blk_Stat_V21_t", "CALIB_AWB_BLK_STAT_MODE_WP_V201")
    CalibDbV2_Awb_Blk_Stat_V21_t      blkMeasureMode;
    // M4_STRUCT_DESC("mainWindow", "normal_ui_style")
    CalibDbV2_StatWindow_t mainWindow;
    // M4_ARRAY_TABLE_DESC("limitRange", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDbV2_Awb_Limit_Range_t limitRange;
    // M4_STRUCT_DESC("rgb2TcsPara", "normal_ui_style", M4_HIDE(1))
    CalibDbV2_Rgb2Tcs_t rgb2TcsPara;
    // M4_ARRAY_DESC("rgb2RotationYuvMat", "f32", M4_SIZE(4,4), M4_RANGE(-255,255), "0", M4_DIGIT(6), M4_DYNAMIC(0), M4_HIDE(1))
    float rgb2RotationYuvMat[16];
    //several winow in uv or xy domain
    // M4_STRUCT_LIST_DESC("extraWpRange", M4_SIZE(1,7), "normal_ui_style")
    CalibDbV2_ExtRange_t extraWpRange[CALD_AWB_EXCRANGE_NUM_MAX];
    // M4_STRUCT_DESC("wpDiffLumaWeight", "normal_ui_style")
    CalibDbV2_Awb_Luma_Weight_t wpDiffLumaWeight;
    // M4_BOOL_DESC("wpDiffBlkWeiEnable", "0");
    bool           wpDiffBlkWeiEnable;
    // M4_ARRAY_DESC("wpDiffBlkWeight", "u16", M4_SIZE(15,15), M4_RANGE(0,63), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned short wpDiffBlkWeight[CALD_AWB_GRID_NUM_TOTAL];
    // M4_STRUCT_LIST_DESC("lightSources", M4_SIZE(1,7), "normal_ui_style")
    CalibDbV2_Awb_Light_V21_t* lightSources;
    int lightSources_len;
} CalibDbV2_Wb_Awb_Para_V21_t;

typedef enum CalibDbV2_Wb_Mwb_Mode_e {
    CALIB_MWB_MODE_CCT                  = 1,        /**< run manual white balance by cct*/
    CALIB_MWB_MODE_WBGAIN               = 2,        /**< run manual white balance by wbgain*/
    CALIB_MWB_MODE_SCENE                = 3,       /**< run manual white balance by scene selection*/
} CalibDbV2_Wb_Mwb_Mode_t;

typedef enum CalibDbV2_Wb_Mwb_Scene_e {
    CALIB_WB_SCENE_INCANDESCENT = 0,
    CALIB_WB_SCENE_FLUORESCENT,
    CALIB_WB_SCENE_WARM_FLUORESCENT,
    CALIB_WB_SCENE_DAYLIGHT,
    CALIB_WB_SCENE_CLOUDY_DAYLIGHT,
    CALIB_WB_SCENE_TWILIGHT,
    CALIB_WB_SCENE_SHADE
} CalibDbV2_Wb_Mwb_Scene_t;

typedef struct CalibDbV2_Awb_Cct_s {
    // M4_NUMBER_DESC("CCT", "f32", M4_RANGE(0,10000), "5000", M4_DIGIT(0))
    float CCT;
    // CXF CHECK
    // M4_NUMBER_DESC("CCRI", "f32", M4_RANGE(-2,2), "0", M4_DIGIT(3))
    float CCRI;
} CalibDbV2_Awb_Cct_t;
typedef struct CalibDbV2_Wb_Mwb_Cfg_Para_s {
    // M4_ARRAY_DESC("mwbGain", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1.0", M4_DIGIT(4), M4_DYNAMIC(0))
    float mwbGain[4];
    // M4_ENUM_DESC("scene", "CalibDbV2_Wb_Mwb_Scene_t", "CALIB_WB_SCENE_DAYLIGHT");
    CalibDbV2_Wb_Mwb_Scene_t scene;
    // M4_STRUCT_DESC("cct", "normal_ui_style")
    CalibDbV2_Awb_Cct_t cct;
} CalibDbV2_Wb_Mwb_Cfg_Para_t;

typedef struct CalibDbV2_Wb_Mwb_Para_s {
    // M4_ENUM_DESC("mode", "CalibDbV2_Wb_Mwb_Mode_t", "CALIB_MWB_MODE_WBGAIN");
    CalibDbV2_Wb_Mwb_Mode_t mode;
    // M4_STRUCT_DESC("cfg", "normal_ui_style")
    CalibDbV2_Wb_Mwb_Cfg_Para_t cfg;
} CalibDbV2_Wb_Mwb_Para_t;

typedef enum CalibDbV2_Wb_Op_Mode_e {
    CALIB_WB_MODE_MANUAL                      = 0,        /**< run manual white balance */
    CALIB_WB_MODE_AUTO                        = 1,        /**< run auto white balance */
} CalibDbV2_Wb_Op_Mode_t;

typedef struct CalibDbV2_Wb_Para_t {
    // M4_BOOL_DESC("byPass", "0");
    bool byPass;
    // M4_ENUM_DESC("mode", "CalibDbV2_Wb_Op_Mode_t", "CALIB_WB_MODE_AUTO");
    CalibDbV2_Wb_Op_Mode_t mode;//replace awbEnable
} CalibDbV2_Wb_Para_t;

typedef struct CalibDbV2_Wb_Para_V20_s {
    // M4_STRUCT_DESC("control", "normal_ui_style")
    CalibDbV2_Wb_Para_t control;
    // M4_STRUCT_DESC("manualPara", "normal_ui_style")
    CalibDbV2_Wb_Mwb_Para_t manualPara;
    // M4_STRUCT_DESC("autoPara", "normal_ui_style")
    CalibDbV2_Wb_Awb_Para_V20_t autoPara;
    // M4_STRUCT_DESC("autoExtPara", "normal_ui_style")
    CalibDbV2_Wb_Awb_Ext_Com_Para_t autoExtPara;
} CalibDbV2_Wb_Para_V20_t;

typedef struct CalibDbV2_Wb_Para_V21_s {
    // M4_STRUCT_DESC("control", "normal_ui_style")
    CalibDbV2_Wb_Para_t control;
    // M4_STRUCT_DESC("manualPara", "normal_ui_style")
    CalibDbV2_Wb_Mwb_Para_t manualPara;
    // M4_STRUCT_DESC("autoPara", "normal_ui_style")
    CalibDbV2_Wb_Awb_Para_V21_t autoPara;
    // M4_STRUCT_DESC("autoExtPara", "normal_ui_style")
    CalibDbV2_Wb_Awb_Ext_Com_Para_t autoExtPara;
} CalibDbV2_Wb_Para_V21_t;

typedef enum CalibDbV2_Awb_Raw_Select_Mode_s {
    CALIB_AWB_INPUT_RAW_FIXED = 0,
    CALIB_AWB_INPUT_RAW_AUTO = 1,
    CALIB_AWB_INPUT_BAYERNR = 3,
    CALIB_AWB_INPUT_DRC = 4,
} CalibDbV2_Awb_Raw_Select_Mode_e;

typedef struct CalibDbV2_Awb_Raw_Select_s {
    // M4_ENUM_DESC("frameChooseMode", "CalibDbV2_Awb_Raw_Select_Mode_e", "CALIB_AWB_INPUT_BAYERNR")
    CalibDbV2_Awb_Raw_Select_Mode_e frameChooseMode;
    // M4_NUMBER_DESC("frameChoose", "u8", M4_RANGE(0,2), "1", M4_DIGIT(0))
    unsigned char frameChoose;
} CalibDbV2_Awb_Raw_Select_t;
typedef struct CalibDbV2_ExtR_Wei_V32_s {
    // M4_ARRAY_DESC("lumaValue", "f32", M4_SIZE(1,5), M4_RANGE(0,255000), "0", M4_DIGIT(1), M4_DYNAMIC(1))
    float* lumaValue;
    int lumaValue_len;
    // M4_ARRAY_DESC("weight", "f32", M4_SIZE(1,5), M4_RANGE(0,5), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float* weight;
    int weight_len;
} CalibDbV2_ExtR_Wei_V32_t;

typedef struct CalibDbV2_ExtRange_V32_s {
    // M4_ENUM_DESC("domain", "CalibDbV2_Awb_Ext_Range_Dom_t", "CALIB_AWB_EXTRA_RANGE_DOMAIN_UV")
    CalibDbV2_Awb_Ext_Range_Dom_t domain;/*0uv domain,1 xy domain*/
    // M4_ENUM_DESC("mode", "CalibDbV2_Awb_Ext_Range_Mode_t", "CALIB_AWB_EXCLUDE_WP_MODE")
    CalibDbV2_Awb_Ext_Range_Mode_t mode;
    // M4_ARRAY_DESC("region", "s32", M4_SIZE(1,4), M4_RANGE(-8192, 8191), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    int region[4];
    // M4_ARRAY_TABLE_DESC("weightInculde", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDbV2_ExtR_Wei_V32_t weightInculde;
} CalibDbV2_ExtRange_V32_t;

typedef struct CalibDbV2_Awb_offset_data_s {
    // M4_ARRAY_DESC("ISO", "f32", M4_SIZE(1,100), M4_RANGE(0,10000000), "50",M4_DIGIT(4), M4_DYNAMIC(1))
    float* ISO;
    int ISO_len;
    // M4_ARRAY_DESC("R_Channel", "f32", M4_SIZE(1,100), M4_RANGE(0,4095), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float* R_Channel;
    int R_Channel_len;
    // M4_ARRAY_DESC("Gr_Channel", "f32", M4_SIZE(1,100), M4_RANGE(0,4095), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float* Gr_Channel;
    int Gr_Channel_len;
    // M4_ARRAY_DESC("Gb_Channel", "f32", M4_SIZE(1,100), M4_RANGE(0,4095), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float* Gb_Channel;
    int Gb_Channel_len;
    // M4_ARRAY_DESC("B_Channel", "f32", M4_SIZE(1,100), M4_RANGE(0,4095), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float* B_Channel;
    int B_Channel_len;
} CalibDbV2_Awb_offset_data_t;

typedef struct CalibDbV2_Awb_Blc_s {
    // M4_BOOL_DESC("enable", "0");
    bool enable;
    // M4_ARRAY_TABLE_DESC("offset", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDbV2_Awb_offset_data_t offset;//r gr gb b
} CalibDbV2_Awb_Blc_t;

typedef struct CalibDbV2_Awb_Limit_Range_V32_s {
    // M4_ARRAY_DESC("lumaValue", "f32", M4_SIZE(1,5), M4_RANGE(0,255000), "0", M4_DIGIT(1), M4_DYNAMIC(1))
    float* lumaValue;
    int lumaValue_len;
    // M4_ARRAY_DESC("maxR", "f32", M4_SIZE(1,5), M4_RANGE(0,255), "0", M4_DIGIT(4), M4_DYNAMIC(1))
    float* maxR;
    int maxR_len;
    // M4_ARRAY_DESC("minR", "f32", M4_SIZE(1,5), M4_RANGE(0,255), "0", M4_DIGIT(4), M4_DYNAMIC(1))
    float * minR;
    int minR_len;
    // M4_ARRAY_DESC("maxG", "f32", M4_SIZE(1,5), M4_RANGE(0,255), "0", M4_DIGIT(4), M4_DYNAMIC(1))
    float * maxG;
    int maxG_len;
    // M4_ARRAY_DESC("minG", "f32", M4_SIZE(1,5), M4_RANGE(0,255), "0", M4_DIGIT(4), M4_DYNAMIC(1))
    float * minG;
    int minG_len;
    // M4_ARRAY_DESC("maxB", "f32", M4_SIZE(1,5), M4_RANGE(0,255), "0", M4_DIGIT(4), M4_DYNAMIC(1))
    float *maxB;
    int maxB_len;
    // M4_ARRAY_DESC("minB", "f32", M4_SIZE(1,5), M4_RANGE(0,255), "0", M4_DIGIT(4), M4_DYNAMIC(1))
    float * minB;
    int minB_len;
    // M4_ARRAY_DESC("maxY", "f32", M4_SIZE(1,5), M4_RANGE(0,255), "0", M4_DIGIT(4), M4_DYNAMIC(1))
    float * maxY;
    int maxY_len;
    // M4_ARRAY_DESC("minY", "f32", M4_SIZE(1,5), M4_RANGE(0,255), "0", M4_DIGIT(4), M4_DYNAMIC(1))
    float *minY;
    int minY_len;
} CalibDbV2_Awb_Limit_Range_V32_t;

typedef enum CalibDbV2_Wb_Awb_EarlAct_mdoe_e {
    CALIB_AWB_EARLACT_XYREG_AUTO = 0,
    CALIB_AWB_EARLACT_XYREG_FIXED,
} CalibDbV2_Wb_Awb_EarlAct_mdoe_t;

typedef struct CalibDbV2_Tcs_Range_Ill3_s {
    // M4_ARRAY_DESC("normal", "s32", M4_SIZE(1,4), M4_RANGE(-8192,8192), "0", M4_DIGIT(0),
    // M4_DYNAMIC(0))
    int normal[4];
    // M4_ARRAY_DESC("big", "s32", M4_SIZE(1,4), M4_RANGE(-8192,8192), "0", M4_DIGIT(0),
    // M4_DYNAMIC(0))
    int big[4];
} CalibDbV2_Tcs_Range_Ill3_t;

typedef struct CalibDbV2_Wb_Awb_EarlAct_s {
    // M4_BOOL_DESC("enable", "1");
    bool enable;
    // M4_ENUM_DESC("mode", "CalibDbV2_Wb_Awb_EarlAct_mdoe_t", "CALIB_AWB_EARLACT_XYREG_AUTO")
    CalibDbV2_Wb_Awb_EarlAct_mdoe_t mode;
    // M4_STRUCT_LIST_DESC("xyRegion", M4_SIZE(1,4), "normal_ui_style")
    CalibDbV2_Tcs_Range_Ill3_t xyRegion[4];
} CalibDbV2_Wb_Awb_EarlAct_t;

typedef struct CalibDbV2_Awb_Light_V32_s {
    // M4_STRING_DESC("name", M4_SIZE(1,1), M4_RANGE(0, 32), "default", M4_DYNAMIC(0))
    char* name;
    // M4_ENUM_DESC("doorType", "CalibDbV2_Awb_DoorType_t", "CALIB_AWB_DOOR_TYPE_AMBIGUITY")
    CalibDbV2_Awb_DoorType_t doorType;
    // M4_ARRAY_DESC("standardGainValue", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "0", M4_DIGIT(4),
    // M4_DYNAMIC(0))
    float standardGainValue[4];
    // M4_ARRAY_TABLE_DESC("uvRegion", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDbV2_Uv_Range_Ill_t uvRegion;
    // M4_ARRAY_TABLE_DESC("xyRegion", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDbV2_Tcs_Range_Ill2_t xyRegion;
    // M4_STRUCT_DESC("rtYuvRegion", "normal_ui_style")
    CalibDbV2_Yuv3D_2_Range_Ill_t rtYuvRegion;
    // M4_ARRAY_DESC("staWeight", "u8", M4_SIZE(1,16), M4_RANGE(0,100), "0", M4_DIGIT(0),
    // M4_DYNAMIC(0))
    unsigned char staWeight[CALD_AWB_LV_NUM_MAX];
    // M4_ARRAY_DESC("dayGainLvThSet", "u8", M4_SIZE(1,2), M4_RANGE(0,255000), "0", M4_DIGIT(0),
    // M4_DYNAMIC(0))
    unsigned int dayGainLvThSet[2];
    // M4_ARRAY_DESC("defaultDayGainLow", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1", M4_DIGIT(4),
    // M4_DYNAMIC(0))
    float defaultDayGainLow[4];  // spatial gain
    // M4_ARRAY_DESC("defaultDayGainHigh", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1", M4_DIGIT(4),
    // M4_DYNAMIC(0))
    float defaultDayGainHigh[4];
    // M4_ARRAY_TABLE_DESC("weight", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDbV2_ExtR_Wei_V32_t weight;

} CalibDbV2_Awb_Light_V32_t;

typedef struct CalibDbV2_Wb_Awb_Para_V32_t {
    // M4_STRUCT_DESC("rawSelectPara", "normal_ui_style")
    CalibDbV2_Awb_Raw_Select_t rawSelectPara;//hdrFrameChoose;
    // M4_STRUCT_DESC("blc2ForAwb", "normal_ui_style")
    CalibDbV2_Awb_Blc_t blc2ForAwb;
    // M4_BOOL_DESC("lscBypass", "0");
    bool                lscBypass;
    // M4_BOOL_DESC("uvDetectionEnable", "0");
    bool                uvDetectionEnable;
    // M4_BOOL_DESC("xyDetectionEnable", "0");
    bool                xyDetectionEnable;
    // M4_BOOL_DESC("yuvDetectionEnable", "0");
    bool                yuvDetectionEnable;
    // M4_BOOL_DESC("blkStatisticsEnable", "0");
    bool                blkStatisticsEnable;
    // M4_ENUM_DESC("downScaleMode", "CalibDbV2_Awb_Down_Scale_Mode_t", "CALIB_AWB_DS_8X8")
    CalibDbV2_Awb_Down_Scale_Mode_t       downScaleMode;
    // M4_ENUM_DESC("blkMeasureMode", "CalibDbV2_Awb_Blk_Stat_V21_t", "CALIB_AWB_BLK_STAT_MODE_WP_V201")
    CalibDbV2_Awb_Blk_Stat_V21_t      blkMeasureMode;
    // M4_STRUCT_DESC("mainWindow", "normal_ui_style")
    CalibDbV2_StatWindow_t mainWindow;
    // M4_ARRAY_TABLE_DESC("limitRange", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDbV2_Awb_Limit_Range_V32_t limitRange;
    // M4_STRUCT_DESC("rgb2TcsPara", "normal_ui_style", M4_HIDE(1))
    CalibDbV2_Rgb2Tcs_t rgb2TcsPara;
    // M4_ARRAY_DESC("rgb2RotationYuvMat", "f32", M4_SIZE(4,4), M4_RANGE(-255,255), "0", M4_DIGIT(6), M4_DYNAMIC(0), M4_HIDE(1))
    float rgb2RotationYuvMat[16];
    //several winow in uv or xy domain
    // M4_STRUCT_LIST_DESC("extraWpRange", M4_SIZE(1,7), "normal_ui_style")
    CalibDbV2_ExtRange_V32_t extraWpRange[CALD_AWB_EXCRANGE_NUM_MAX];
    // M4_STRUCT_DESC("wpDiffLumaWeight", "normal_ui_style")
    CalibDbV2_Awb_Luma_Weight_t wpDiffLumaWeight;
    // M4_BOOL_DESC("wpDiffBlkWeiEnable", "0");
    bool           wpDiffBlkWeiEnable;
    // M4_ARRAY_DESC("wpDiffBlkWeight", "u16", M4_SIZE(15,15), M4_RANGE(0,63), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned short wpDiffBlkWeight[CALD_AWB_GRID_NUM_TOTAL];
    // M4_STRUCT_LIST_DESC("lightSources", M4_SIZE(1,7), "normal_ui_style")
    CalibDbV2_Awb_Light_V32_t* lightSources;
    int lightSources_len;
    // M4_STRUCT_DESC("earlierAwbAct", "normal_ui_style")
    CalibDbV2_Wb_Awb_EarlAct_t earlierAwbAct;

} CalibDbV2_Wb_Awb_Para_V32_t;




typedef struct CalibDbV2_Wb_Para_V32_s {
    // M4_STRUCT_DESC("control", "normal_ui_style")
    CalibDbV2_Wb_Para_t control;
    // M4_STRUCT_DESC("manualPara", "normal_ui_style")
    CalibDbV2_Wb_Mwb_Para_t manualPara;
    // M4_STRUCT_DESC("autoPara", "normal_ui_style")
    CalibDbV2_Wb_Awb_Para_V32_t autoPara;
    // M4_STRUCT_DESC("autoExtPara", "normal_ui_style")
    CalibDbV2_Wb_Awb_Ext_Para_V32_t autoExtPara;
} CalibDbV2_Wb_Para_V32_t;

RKAIQ_END_DECLARE

#endif
