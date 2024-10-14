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

#ifndef __AWB_UAPI_HEAD_H__
#define __AWB_UAPI_HEAD_H__


#include "rk_aiq_user_api_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __uapi_wb_mode_t {
  // M4_ENUM_DESC("mode", "opMode_t", "OP_AUTO");
  opMode_t mode;
} uapi_wb_mode_t;

typedef struct __uapi_wb_gain_t {
  // M4_NUMBER_DESC("rgain", "f32", M4_RANGE(0,8), "0", M4_DIGIT(6))
    float rgain;
  // M4_NUMBER_DESC("grgain", "f32", M4_RANGE(0,8), "0", M4_DIGIT(6))
    float grgain;
  // M4_NUMBER_DESC("gbgain", "f32", M4_RANGE(0,8), "0", M4_DIGIT(6))
    float gbgain;
  // M4_NUMBER_DESC("bgain", "f32", M4_RANGE(0,8), "0", M4_DIGIT(6))
    float bgain;
} uapi_wb_gain_t;


#define RK_TOOL_AWB_MAX_WHITEREGIONS_NUM 7
#define RK_TOOL_AWB_MAX_WHITEREGIONS_NUM_V32 4
#define RK_TOOL_AWB_MAX_WHITEREGIONS_NUM2 14
#define RK_TOOL_AWB_STAT_WP_RANGE_NUM_V201 4
#define RK_TOOL_AWB_WP_HIST_BIN_NUM 8
#define RK_TOOL_AWB_XY_TYPE_MAX_V201 2
#define RK_TOOL_AWB_GRID_NUM_TOTAL 225
#define RK_TOOL_AWB_CHANNEL_MAX 4
#define RK_TOOL_MAX_AWB_TIME_SHARE_FRAME_NUM 4
//typedef struct stat3a_lightType_s
typedef struct rk_tool_awb_stat_wp_res_v201_s {
    // M4_NUMBER_DESC("WpNo", "u32", M4_RANGE(-9223372036854775808,9223372036854775807), "0", M4_DIGIT(0), M4_HIDE(0))
    long long WpNo;
    // M4_NUMBER_DESC("RgainValue", "u32", M4_RANGE(-9223372036854775808,9223372036854775807), "0", M4_DIGIT(0), M4_HIDE(0))
    long long RgainValue;
    // M4_NUMBER_DESC("BgainValue", "u32", M4_RANGE(-9223372036854775808,9223372036854775807), "0", M4_DIGIT(0), M4_HIDE(0))
    long long BgainValue;
} rk_tool_awb_stat_wp_res_v201_t;

typedef struct rk_tool_awb_stat_blk_res_v201_s {
    // M4_NUMBER_DESC("WpNo", "u32", M4_RANGE(-9223372036854775808,9223372036854775807), "0", M4_DIGIT(0), M4_HIDE(0))
    long long WpNo;
    // M4_NUMBER_DESC("Rvalue", "u32", M4_RANGE(-9223372036854775808,9223372036854775807), "0", M4_DIGIT(0), M4_HIDE(0))
    long long Rvalue;
    // M4_NUMBER_DESC("Gvalue", "u32", M4_RANGE(-9223372036854775808,9223372036854775807), "0", M4_DIGIT(0), M4_HIDE(0))
    long long Gvalue;
    // M4_NUMBER_DESC("Bvalue", "u32", M4_RANGE(-9223372036854775808,9223372036854775807), "0", M4_DIGIT(0), M4_HIDE(0))
    long long Bvalue;
} rk_tool_awb_stat_blk_res_v201_t;

typedef struct rk_tool_awb_wp_res_light_v201_s {
    // M4_STRUCT_LIST_DESC("xYType", M4_SIZE(1,2), "normal_ui_style")
    rk_tool_awb_stat_wp_res_v201_t xYType[RK_TOOL_AWB_XY_TYPE_MAX_V201];
} rk_tool_awb_wp_res_light_v201_t;

typedef struct rk_tool_awb_stat_res2_v201_s {
    // M4_STRUCT_LIST_DESC("light", M4_SIZE(1,7), "normal_ui_style")
    rk_tool_awb_wp_res_light_v201_t light[RK_TOOL_AWB_MAX_WHITEREGIONS_NUM];
    //method2
    // M4_STRUCT_LIST_DESC("blockResult", M4_SIZE(1,225), "normal_ui_style")
    rk_tool_awb_stat_blk_res_v201_t   blockResult[RK_TOOL_AWB_GRID_NUM_TOTAL];
    // M4_ARRAY_DESC("WpNoHist", "u32", M4_SIZE(1,8), M4_RANGE(0,4294967295), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned int WpNoHist[RK_TOOL_AWB_WP_HIST_BIN_NUM];
} rk_tool_awb_stat_res2_v201_t;


typedef struct rk_tool_awb_stat_res2_v30_s {
    // M4_STRUCT_LIST_DESC("light", M4_SIZE(1,7), "normal_ui_style")
    rk_tool_awb_wp_res_light_v201_t light[RK_TOOL_AWB_MAX_WHITEREGIONS_NUM];
    // M4_ARRAY_DESC("WpNo2", "s32", M4_SIZE(1,8), M4_RANGE(-2147483648,2147483647), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    int WpNo2[RK_TOOL_AWB_MAX_WHITEREGIONS_NUM];
    // M4_STRUCT_LIST_DESC("blockResult", M4_SIZE(1,225), "normal_ui_style")
    rk_tool_awb_stat_blk_res_v201_t   blockResult[RK_TOOL_AWB_GRID_NUM_TOTAL];
    // M4_STRUCT_LIST_DESC("multiwindowLightResult", M4_SIZE(1,4), "normal_ui_style")
    rk_tool_awb_wp_res_light_v201_t multiwindowLightResult[4];
    // M4_STRUCT_LIST_DESC("excWpRangeResult", M4_SIZE(1,4), "normal_ui_style")
    rk_tool_awb_stat_wp_res_v201_t excWpRangeResult[RK_TOOL_AWB_STAT_WP_RANGE_NUM_V201];
    // M4_ARRAY_DESC("WpNoHist", "u32", M4_SIZE(1,8), M4_RANGE(0,4294967295), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned int WpNoHist[RK_TOOL_AWB_WP_HIST_BIN_NUM];
} rk_tool_awb_stat_res2_v30_t;

typedef struct rk_tool_isp_awb_stats_v32_s {
    // M4_STRUCT_LIST_DESC("light", M4_SIZE(1,4), "normal_ui_style")
    rk_tool_awb_wp_res_light_v201_t light[RK_TOOL_AWB_MAX_WHITEREGIONS_NUM_V32];
    // M4_ARRAY_DESC("WpNo2", "s32", M4_SIZE(1,8), M4_RANGE(-2147483648,2147483647), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    int WpNo2[RK_TOOL_AWB_MAX_WHITEREGIONS_NUM_V32];
    // M4_STRUCT_LIST_DESC("blockResult", M4_SIZE(1,225), "normal_ui_style")
    rk_tool_awb_stat_blk_res_v201_t   blockResult[RK_TOOL_AWB_GRID_NUM_TOTAL];
    // M4_STRUCT_LIST_DESC("multiwindowLightResult", M4_SIZE(1,4), "normal_ui_style")
    rk_tool_awb_stat_wp_res_v201_t excWpRangeResult[RK_TOOL_AWB_STAT_WP_RANGE_NUM_V201];
    // M4_ARRAY_DESC("WpNoHist", "u32", M4_SIZE(1,8), M4_RANGE(0,4294967295), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned int WpNoHist[RK_TOOL_AWB_WP_HIST_BIN_NUM];
} rk_tool_isp_awb_stats_v32_t;


typedef struct rk_tool_awb_measure_wp_res_fl_s {
    // M4_ARRAY_DESC("WpNo", "u32", M4_SIZE(1,1), M4_RANGE(0,4294967295), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned int WpNo;
    // M4_ARRAY_DESC("gain", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float gain[RK_TOOL_AWB_CHANNEL_MAX];//mean rgain  value
    // M4_ARRAY_DESC("WpNoHist_2", "s32", M4_SIZE(1,8), M4_RANGE(-2147483648,2147483647), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    int WpNoHist_2[RK_TOOL_AWB_WP_HIST_BIN_NUM]; //uselsess
} rk_tool_awb_measure_wp_res_fl_t;

typedef struct rk_tool_awb_blk_res2_fl_s {
    // M4_ARRAY_DESC("WpNo", "u32", M4_SIZE(1,1), M4_RANGE(0,4294967295), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned int WpNo;
    // M4_ARRAY_DESC("gain", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float gain[RK_TOOL_AWB_CHANNEL_MAX];//mean rgain  value
} rk_tool_awb_blk_res2_fl_t;

typedef struct rk_tool_awb_wp_res_light_fl_s {
    // M4_STRUCT_LIST_DESC("xYType", M4_SIZE(1,2), "normal_ui_style")
    rk_tool_awb_measure_wp_res_fl_t xYType[RK_TOOL_AWB_XY_TYPE_MAX_V201];
} rk_tool_awb_wp_res_light_fl_t;

typedef struct rk_tool_awb_blk_res_fl_s {
    // M4_ARRAY_DESC("R", "f32", M4_SIZE(1,1), M4_RANGE(0,1), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float R;//mean r value and normalize to 0~1
    // M4_ARRAY_DESC("G", "f32", M4_SIZE(1,1), M4_RANGE(0,1), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float G;
    // M4_ARRAY_DESC("B", "f32", M4_SIZE(1,1), M4_RANGE(0,1), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float B;
} rk_tool_awb_blk_res_fl_t;

typedef struct rk_tool_awb_effect_para1_s{
    // M4_ARRAY_DESC("hdrFrameChoose", "s32", M4_SIZE(1,1), M4_RANGE(-2147483648,2147483647), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    int hdrFrameChoose;
}rk_tool_awb_effect_para1_t;

typedef struct rk_tool_awb_stat_res_full_s {
    // M4_ARRAY_DESC("lightNum", "s32", M4_SIZE(1,1), M4_RANGE(-2147483648,2147483647), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    int lightNum;
    // M4_STRUCT_LIST_DESC("light", M4_SIZE(1,14), "normal_ui_style")
    rk_tool_awb_wp_res_light_fl_t light[RK_TOOL_AWB_MAX_WHITEREGIONS_NUM2];
    // M4_BOOL_DESC("lightWpResVaLidIll", "0")
    bool                          lightWpResVaLidIll[RK_TOOL_AWB_MAX_WHITEREGIONS_NUM2];//for time share
    // M4_BOOL_DESC("lightWpResVaLid", "0")
    bool                          lightWpResVaLid;//for time share
    // M4_ARRAY_DESC("WpNo", "s32", M4_SIZE(1,2), M4_RANGE(-2147483648,2147483647), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    int WpNo[RK_TOOL_AWB_XY_TYPE_MAX_V201];//for diff xy type
    // M4_ARRAY_DESC("WpNo2", "s32", M4_SIZE(1,14), M4_RANGE(-2147483648,2147483647), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    int WpNo2[RK_TOOL_AWB_MAX_WHITEREGIONS_NUM2];//for diff illu
    // M4_BOOL_DESC("blkGwResValid", "0")
    bool                    blkGwResValid;
    // M4_BOOL_DESC("blkSgcResVaLid", "0")
    bool                    blkSgcResVaLid;
    // M4_STRUCT_LIST_DESC("blkSgcResult", M4_SIZE(1,225), "normal_ui_style")
    rk_tool_awb_blk_res_fl_t blkSgcResult[RK_TOOL_AWB_GRID_NUM_TOTAL];
    // M4_BOOL_DESC("blkWpResVaLid", "0")
    bool                     blkWpResVaLid;
    // M4_STRUCT_LIST_DESC("blkWpResult", M4_SIZE(1,225), "normal_ui_style")
    rk_tool_awb_blk_res2_fl_t blkWpResult[RK_TOOL_AWB_GRID_NUM_TOTAL];
    // M4_BOOL_DESC("excWpResValid", "0")
    bool excWpResValid;
    // M4_STRUCT_LIST_DESC("excWpRangeResult", M4_SIZE(1,4), "normal_ui_style")
    rk_tool_awb_measure_wp_res_fl_t excWpRangeResult[RK_TOOL_AWB_STAT_WP_RANGE_NUM_V201];
    // M4_STRUCT_DESC("extraLightResult", "normal_ui_style")
    rk_tool_awb_measure_wp_res_fl_t extraLightResult;
    // M4_ARRAY_DESC("WpNoHist", "u32", M4_SIZE(1,8), M4_RANGE(0,4294967295), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned int WpNoHist[RK_TOOL_AWB_WP_HIST_BIN_NUM];
    // M4_STRUCT_DESC("effectHwPara", "normal_ui_style")
    rk_tool_awb_effect_para1_t effectHwPara;
} rk_tool_awb_stat_res_full_t;

typedef struct rk_tool_awb_smart_run_res_s{
    // M4_BOOL_DESC("blc1Stable", "0")
    bool blc1Stable;
    // M4_BOOL_DESC("lvStable", "0")
    bool lvStable;
    // M4_BOOL_DESC("wbgainStable", "0")
    bool wbgainStable;
    // M4_BOOL_DESC("wpDiffweiStable", "0")
    bool wpDiffweiStable;
    // M4_NUMBER_DESC("statisticsStable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char statisticsStable;
    // M4_BOOL_DESC("forceRunAwbFlag", "0")
    bool forceRunAwbFlag;//update by api or prepare;
    // M4_BOOL_DESC("samrtRunAwbFlag2", "0")
    bool samrtRunAwbFlag2;//update by above paras
    // M4_ARRAY_DESC("wbgainMean", "f32", M4_SIZE(4,4), M4_RANGE(0,8), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float wbgainMean[RK_TOOL_MAX_AWB_TIME_SHARE_FRAME_NUM][RK_TOOL_AWB_CHANNEL_MAX];
    // M4_BOOL_DESC("algMethodStable", "0")
    bool algMethodStable;
}rk_tool_awb_smart_run_res_t;

typedef struct rk_tool_awb_illInf_s {
    // M4_STRING_DESC("illName", M4_SIZE(1,100), M4_RANGE(-128, 127), "default", M4_DYNAMIC(0))
    char illName[100];
    // M4_ARRAY_DESC("gainValue", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float gainValue[4];
    // M4_ARRAY_DESC("prob_total", "f32", M4_SIZE(1,1), M4_RANGE(0,1), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float prob_total;
    // M4_ARRAY_DESC("prob_dis", "f32", M4_SIZE(1,1), M4_RANGE(0,1), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float prob_dis;
    // M4_ARRAY_DESC("prob_LV", "f32", M4_SIZE(1,1), M4_RANGE(0,1), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float prob_LV;
    // M4_ARRAY_DESC("weight", "f32", M4_SIZE(1,1), M4_RANGE(0,1), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float weight;
    // M4_ARRAY_DESC("spatialGainValue", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float spatialGainValue[RK_TOOL_AWB_CHANNEL_MAX];
    // M4_ARRAY_DESC("prob_WPNO", "f32", M4_SIZE(1,1), M4_RANGE(0,1), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float prob_WPNO;
    // M4_ARRAY_DESC("staWeight", "f32", M4_SIZE(1,1), M4_RANGE(0,1), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float staWeight;
    // M4_ARRAY_DESC("xyType2Weight", "f32", M4_SIZE(1,1), M4_RANGE(0,1), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float xyType2Weight;
} rk_tool_awb_illInf_t;

typedef struct rk_tool_color_tempture_info_s {
    // M4_BOOL_DESC("valid", "0")
    bool valid;
    // M4_NUMBER_DESC("CCT", "f32", M4_RANGE(0,10000), "5000", M4_DIGIT(0))
    float CCT;
    // M4_NUMBER_DESC("CCRI", "f32", M4_RANGE(-2,2), "0", M4_DIGIT(3))
    float CCRI;
} rk_tool_color_tempture_info_t;

typedef struct rk_tool_awb_strategy_result_s {
    // M4_BOOL_DESC("awbConverged", "0")
    bool awbConverged;
    // M4_STRUCT_DESC("cctGloabl", "normal_ui_style")
    rk_tool_color_tempture_info_t cctGloabl;
    // M4_ARRAY_DESC("lightNum", "s32", M4_SIZE(1,1), M4_RANGE(-2147483648,2147483647), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    int lightNum;
    // M4_STRUCT_DESC("smartAwbRunRes", "normal_ui_style")
    rk_tool_awb_smart_run_res_t smartAwbRunRes;
    // M4_ARRAY_DESC("count", "u32", M4_SIZE(1,1), M4_RANGE(0,4294967295), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned int count;
    // M4_ARRAY_DESC("runInterval", "u32", M4_SIZE(1,1), M4_RANGE(0,4294967295), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    uint32_t runInterval;
    // M4_NUMBER_DESC("tolerance", "f32", M4_RANGE(0,10000), "5000", M4_DIGIT(0))
    float tolerance;
    // M4_ARRAY_DESC("dsRateWt", "u8", M4_SIZE(1,1), M4_RANGE(0,255), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    uint8_t dsRateWh; //downsample 8x8->Pixel(R,G,B)
    // M4_ARRAY_DESC("dsRateHt", "u8", M4_SIZE(1,1), M4_RANGE(0,255), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    uint8_t dsRateHt; //downsample 8x8->Pixel(R,G,B)
    // M4_ARRAY_DESC("width_ds", "u32", M4_SIZE(1,1), M4_RANGE(0,4294967295), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    uint32_t width_ds;
    // M4_ARRAY_DESC("height_ds", "u32", M4_SIZE(1,1), M4_RANGE(0,4294967295), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    uint32_t height_ds;
    // M4_ARRAY_DESC("WPmode", "s32", M4_SIZE(1,1), M4_RANGE(-2147483648,2147483647), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    int WPmode;
    // M4_ARRAY_DESC("WPTotalNUM", "u32", M4_SIZE(1,1), M4_RANGE(0,4294967295), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    uint32_t WPTotalNUM;
    // M4_ARRAY_DESC("WPType", "s32", M4_SIZE(1,1), M4_RANGE(-2147483648,2147483647), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    int WPType;
    // M4_ARRAY_DESC("LVType", "s32", M4_SIZE(1,1), M4_RANGE(-2147483648,2147483647), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    int LVType;
    // M4_ARRAY_DESC("LVValue", "s32", M4_SIZE(1,1), M4_RANGE(-2147483648,2147483647), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    int LVValue;// 8bit/1x/1s
    // M4_NUMBER_DESC("fLVValue", "f32", M4_RANGE(0,255000), "5000", M4_DIGIT(0))
    float fLVValue;
    // M4_ARRAY_DESC("LVLevel", "s32", M4_SIZE(1,1), M4_RANGE(-2147483648,2147483647), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    int LVLevel;
    // M4_STRUCT_LIST_DESC("illInf", M4_SIZE(1,14), "normal_ui_style")
    rk_tool_awb_illInf_t illInf[RK_TOOL_AWB_MAX_WHITEREGIONS_NUM2];//information and measure result in different illuminations
    // M4_ARRAY_DESC("wbGainTepTp3", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float wbGainTepTp3[RK_TOOL_AWB_CHANNEL_MAX];
    // M4_ARRAY_DESC("wbWeiTepTp3", "f32", M4_SIZE(1,1), M4_RANGE(0,100), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float wbWeiTepTp3;//temporal mix statistics
    // M4_ARRAY_DESC("xy_area_type", "u8", M4_SIZE(1,1), M4_RANGE(0,255), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    uint8_t xy_area_type; //enalbe type
    // M4_BOOL_DESC("spaGainEqu2Tem", "0")
    bool spaGainEqu2Tem;
    // M4_BOOL_DESC("sgcGainEqu2Tem", "0")
    bool sgcGainEqu2Tem;
    // M4_ARRAY_DESC("wbGainType3", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float wbGainType3[RK_TOOL_AWB_CHANNEL_MAX];
    // M4_ARRAY_DESC("wbGainType1", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float  wbGainType1[RK_TOOL_AWB_CHANNEL_MAX];
    // M4_ARRAY_DESC("wbWeightType3", "f32", M4_SIZE(1,1), M4_RANGE(0,100), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float wbWeightType3;
    // M4_ARRAY_DESC("wbGainTep", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float  wbGainTep[RK_TOOL_AWB_CHANNEL_MAX];//night gain
    // M4_ARRAY_DESC("wbGainSgc", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float wbGainSgc[RK_TOOL_AWB_CHANNEL_MAX];
    // M4_ARRAY_DESC("wbWeightSgc", "f32", M4_SIZE(1,1), M4_RANGE(0,100), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float wbWeightSgc;
    // M4_ARRAY_DESC("wbGainSpa", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float  wbGainSpa[RK_TOOL_AWB_CHANNEL_MAX];//day gain
    // M4_ARRAY_DESC("wbWeightSpa", "f32", M4_SIZE(1,1), M4_RANGE(0,100), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float wbWeightSpa;
    // M4_ARRAY_DESC("varianceLuma", "f32", M4_SIZE(1,1), M4_RANGE(0,2147483647), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float varianceLuma;
    // M4_ARRAY_DESC("wbGainDampFactor", "f32", M4_SIZE(1,1), M4_RANGE(0,1), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float wbGainDampFactor;
    // M4_BOOL_DESC("clipEnalbe", "0")
    bool clipEnalbe;
    // M4_BOOL_DESC("updateFlag", "0")
    bool updateFlag;
    // M4_ARRAY_DESC("wbGainIntpStrategy", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float wbGainIntpStrategy[RK_TOOL_AWB_CHANNEL_MAX];//gain by interpartition strategy
    // M4_ARRAY_DESC("wbGainClip", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float wbGainClip[RK_TOOL_AWB_CHANNEL_MAX];//gain by clip
    // M4_ARRAY_DESC("wbGainCaga", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float wbGainCaga[RK_TOOL_AWB_CHANNEL_MAX];//gain by  chromatic adatptation gain adjust
    // M4_ARRAY_DESC("wbGainAdjustIn", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float wbGainAdjustIn[RK_TOOL_AWB_CHANNEL_MAX];//gain by color temperature shift
    // M4_ARRAY_DESC("wbGainAdjustOut", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float wbGainAdjust[RK_TOOL_AWB_CHANNEL_MAX];//gain by color temperature shift
    // M4_STRUCT_DESC("cctAdjustIn", "normal_ui_style")
    rk_tool_color_tempture_info_t cctAdjustIn;
    // M4_STRUCT_DESC("cctAdjustOut", "normal_ui_style")
    rk_tool_color_tempture_info_t cctAdjustOut;
    // M4_ARRAY_DESC("wbGainOffset", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float wbGainOffset[RK_TOOL_AWB_CHANNEL_MAX];//gain by gain shift
    // M4_ARRAY_DESC("wbGainSmooth", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float wbGainSmooth[RK_TOOL_AWB_CHANNEL_MAX];//gain by smooth factor
    // M4_ARRAY_DESC("stat3aAwbLastGainOut", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float  stat3aAwbLastGainOut[RK_TOOL_AWB_CHANNEL_MAX];//final gain of last frame
    // M4_ARRAY_DESC("stat3aAwbGainOut", "f32", M4_SIZE(1,4), M4_RANGE(0,8), "1", M4_DIGIT(4), M4_DYNAMIC(0))
    float  stat3aAwbGainOut[RK_TOOL_AWB_CHANNEL_MAX];// final gain
    // M4_ARRAY_DESC("algMethod", "s32", M4_SIZE(1,1), M4_RANGE(0,100), "1", M4_DIGIT(0), M4_DYNAMIC(0))
    int  algMethod;
} rk_tool_awb_strategy_result_t;

typedef struct __uapi_wbV21_log_info_t {
    // M4_STRUCT_DESC("awb_stat", "normal_ui_style")
    rk_tool_awb_stat_res2_v201_t awb_stat;
    // M4_STRUCT_DESC("awb_stat_algo", "normal_ui_style")
    rk_tool_awb_stat_res_full_t awb_stat_algo;
    // M4_STRUCT_DESC("awb_strategy_result", "normal_ui_style")
    rk_tool_awb_strategy_result_t awb_strategy_result;
} uapi_wbV21_log_info_t;

typedef struct __uapi_wbV21_log_t {
    // M4_STRUCT_DESC("uapi_wb_log_info_t", "normal_ui_style")
    uapi_wbV21_log_info_t info;
} uapi_wbV21_log_t;

typedef struct __uapi_wbV30_log_info_t {
    // M4_STRUCT_DESC("awb_stat", "normal_ui_style")
    rk_tool_awb_stat_res2_v30_t awb_stat;
    // M4_STRUCT_DESC("awb_stat_algo", "normal_ui_style")
    rk_tool_awb_stat_res_full_t awb_stat_algo;
    // M4_STRUCT_DESC("awb_strategy_result", "normal_ui_style")
    rk_tool_awb_strategy_result_t awb_strategy_result;
} uapi_wbV30_log_info_t;

typedef struct __uapi_wbV30_log_t {
    // M4_STRUCT_DESC("uapi_wb_log_info_t", "normal_ui_style")
    uapi_wbV30_log_info_t info;
} uapi_wbV30_log_t;

typedef struct __uapi_wbV32_log_info_t {
    // M4_STRUCT_DESC("awb_stat", "normal_ui_style")
    rk_tool_isp_awb_stats_v32_t awb_stat;
    // M4_STRUCT_DESC("awb_stat_algo", "normal_ui_style")
    rk_tool_awb_stat_res_full_t awb_stat_algo;
    // M4_STRUCT_DESC("awb_strategy_result", "normal_ui_style")
    rk_tool_awb_strategy_result_t awb_strategy_result;
} uapi_wbV32_log_info_t;

typedef struct __uapi_wbV32_log_t {
    // M4_STRUCT_DESC("uapi_wb_log_info_t", "normal_ui_style")
    uapi_wbV32_log_info_t info;
} uapi_wbV32_log_t;



#ifdef __cplusplus
}
#endif


#endif  /*__AWB_UAPI_HEAD_H__*/
