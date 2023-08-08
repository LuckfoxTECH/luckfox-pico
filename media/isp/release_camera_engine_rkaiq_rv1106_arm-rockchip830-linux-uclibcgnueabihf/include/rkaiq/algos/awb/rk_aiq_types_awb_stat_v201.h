/*
* rk_aiq_types_awb_stat_v201.h

* for rockchip v2.0.0
*
*  Copyright (c) 2019 Rockchip Corporation
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
/* for rockchip v2.0.0*/

#ifndef __RK_AIQ_TYPE_AWB_STAT_V201_H__
#define __RK_AIQ_TYPE_AWB_STAT_V201_H__

#include "rk_aiq_types_awb_stat_v2xx.h"

#ifdef ISP_HW_V30 //for rk3588
#define AWB_MULTI_WINDOW_EN
#define AWB_EXCWP_RANGE_STAT_EN
#define AWB_WPNUM2_EN
#define RK_AIQ_AWB_WP_WEIGHT_BIS_V201 5
#define RK_AIQ_WP_GAIN_FRAC_BIS 9
#else
#define RK_AIQ_AWB_WP_WEIGHT_BIS_V201 6
#define RK_AIQ_WP_GAIN_FRAC_BIS 10
#endif
#define RK_AIQ_AWB_WIDTH 5120
#define RK_AIQ_AWB_HEIGHT 2880
#define RK_AIQ_AWB_STAT_MAX_AREA (RK_AIQ_AWB_WIDTH * RK_AIQ_AWB_HEIGHT)
#define RK_AIQ_AWB_STAT_WP_RANGE_NUM_V201 4
#define RK_AIQ_AWB_MULTIWINDOW_NUM_V201 4
#define RK_AIQ_AWB_WP_HIST_BIN_NUM 8
#define RK_AIQ_WP_GAIN_FRAC_BIS_INTERNAL 8
#define RK_AIQ_AWBWP_WEIGHT_CURVE_DOT_NUM (RK_AIQ_AWB_WP_HIST_BIN_NUM+1)
#define RK_AIQ_AWB_YUV2_THCURVE_DOT_NUM 6
#define RK_AIQ_AWB_YUV2_MATRIX_FRAC_BIT 9
#define RK_AIQ_AWB_YUV2_OFFSET_FRAC_BIT 4
#define RK_AIQ_AWB_YUV2_OUT_FRAC_BIT 4
#define RK_AIQ_AWB_YUV2_OUT_INTEGER_BIT 8

#define RK_AIQ_AWB_PRE_WBGAIN_FRAC_BIT 8

typedef enum rk_aiq_awb_xy_type_v201_e {
    RK_AIQ_AWB_XY_TYPE_NORMAL_V201 = 0,
    RK_AIQ_AWB_XY_TYPE_BIG_V201 = 1,
    RK_AIQ_AWB_XY_TYPE_MAX_V201
} rk_aiq_awb_xy_type_v201_t;

typedef enum rk_aiq_awb_blk_stat_mode_v201_e {
    RK_AIQ_AWB_BLK_STAT_MODE_MIN_V201 = -1,
    RK_AIQ_AWB_BLK_STAT_MODE_ALL_V201 = 0,
    RK_AIQ_AWB_BLK_STAT_MODE_REALWP_V201 = 1,
    RK_AIQ_AWB_BLK_STAT_MODE_MAX_V201
} rk_aiq_awb_blk_stat_mode_v201_t;

typedef struct rk_aiq_awb_exc_range_v201_s {
    rk_aiq_awb_exc_range_domain_t domain;//1 uv domain,2 xy domain;
    bool excludeEnable[RK_AIQ_AWB_XY_TYPE_MAX_V201];// 0 disable,1 eanble;
    bool measureEnable;//1 calculate sum r g b value in the range,0 not meausre
    int xu[2];//left right
    int yv[2];//top bottom
} rk_aiq_awb_exc_range_v201_t;

typedef enum RK_AIQ_AWB_BLK_STAT_REALWP_ILL_S {
    RK_AIQ_AWB_BLK_STAT_REALWP_ILL0 = 0,
    RK_AIQ_AWB_BLK_STAT_REALWP_ILL1 = 1,
    RK_AIQ_AWB_BLK_STAT_REALWP_ILL2 = 2,
    RK_AIQ_AWB_BLK_STAT_REALWP_ILL3 = 3,
    RK_AIQ_AWB_BLK_STAT_REALWP_ILL4 = 4,
    RK_AIQ_AWB_BLK_STAT_REALWP_ILL5 = 5,
    RK_AIQ_AWB_BLK_STAT_REALWP_ILL6 = 6,
    RK_AIQ_AWB_BLK_STAT_REALWP_ALL = 7,
    RK_AIQ_AWB_BLK_STAT_REALWP_MAX
} rk_aiq_awb_blk_stat_realwp_ill_e;

typedef enum rk_aiq_awb_stat_cfg_mode_s {
    RK_AIQ_AWB_STAT_WP_MODE = 0,
    RK_AIQ_AWB_STAT_GW_MODE = 1,
} rk_aiq_awb_stat_cfg_mode_e;

typedef struct rk_aiq_awb_stat_cfg_v201_s {
    bool awbEnable;
    bool lscBypEnable;
    bool uvDetectionEnable[RK_AIQ_AWB_XY_TYPE_MAX_V201];
    bool xyDetectionEnable[RK_AIQ_AWB_XY_TYPE_MAX_V201];
    bool threeDyuvEnable[RK_AIQ_AWB_XY_TYPE_MAX_V201];
    unsigned short threeDyuvIllu[RK_AIQ_AWB_YUV_LS_PARA_NUM];
    bool wpDiffWeiEnable[RK_AIQ_AWB_XY_TYPE_MAX_V201];
    bool blkWeightEnable[RK_AIQ_AWB_XY_TYPE_MAX_V201];//the different weight in WP sum
    bool blkStatisticsEnable;
    rk_aiq_down_scale_mode_t dsMode;
    rk_aiq_awb_blk_stat_mode_v201_t blkMeasureMode;
    bool blk_rtdw_measure_en;//right and down measure eanble
#ifdef AWB_MULTI_WINDOW_EN
    bool multiwindow_en;
    //several window in pixel domain
    unsigned short multiwindow[RK_AIQ_AWB_MULTIWINDOW_NUM_V201][4];//8  windows in pixel domain ,hOffset,vOffser,hSize,vSize;
#endif
    uint8_t frameChoose;//default value is 0,support to choose long frame ,middle frame or short frame when hdr is on
    unsigned short windowSet[4];//hOffset,vOffser,hSiz,vSize;
    unsigned char lightNum;
    unsigned short maxR;
    unsigned short minR;
    unsigned short maxG;
    unsigned short minG;
    unsigned short maxB;
    unsigned short minB;
    unsigned short maxY;
    unsigned short minY;
    uint16_t pre_wbgain_inv_r;//3i+f8;
    uint16_t pre_wbgain_inv_g;
    uint16_t pre_wbgain_inv_b;
    short          icrgb2RYuv_matrix[12];//3*3 matrix 9bit fracitional +0bit integer+1s
    //  3*1 off    4bit fractional +8bit integer+1s
    rk_aiq_awb_rt3dyuv_range_para_t     ic3Dyuv2Range_param[RK_AIQ_AWB_YUV_LS_PARA_NUM];
    rk_aiq_awb_uv_range_para_t            uvRange_param[RK_AIQ_AWB_MAX_WHITEREGIONS_NUM];//A CWF D50 D65 D75 HZ TL84
    rk_aiq_rgb2xy_para_t      rgb2xy_param;
    rk_aiq_awb_xy_range_para_t    xyRange_param[RK_AIQ_AWB_MAX_WHITEREGIONS_NUM];
    //several winow in uv or xy domain
    rk_aiq_awb_exc_range_v201_t excludeWpRange[RK_AIQ_AWB_EXCLUDE_WP_RANGE_NUM];
    //with differernt luma ,the different weight in WP sum
    unsigned char wpDiffwei_y[RK_AIQ_AWBWP_WEIGHT_CURVE_DOT_NUM];//8bit
    unsigned char wpDiffwei_w[RK_AIQ_AWBWP_WEIGHT_CURVE_DOT_NUM];//8bit fix point ,and wpDiffwei_w[4]=wpDiffwei_w[5]=255;
    rk_aiq_awb_xy_type_v201_t xyRangeTypeForWpHist; //for xyRangeT
    unsigned char blkWeight[RK_AIQ_AWB_GRID_NUM_TOTAL];
    rk_aiq_awb_xy_type_v201_t xyRangeTypeForBlkStatistics; //used when blkMeasureMode>BLK_MEASURE_MODE_ALL
    rk_aiq_awb_blk_stat_realwp_ill_e illIdxForBlkStatistics; //blkMeasureMode used when blkMeasureMode>BLK_MEASURE_MODE_ALL
    bool blkStatisticsWithLumaWeightEn;
    int  groupIllIndxCurrent;//not register, for time share
    int  IllIndxSetCurrent[RK_AIQ_AWB_MAX_WHITEREGIONS_NUM];//not register,for time share
    char timeSign[64];//not register,for time share
    rk_aiq_awb_stat_cfg_mode_e statCfgMode; //not register,for blc1 used case

} rk_aiq_awb_stat_cfg_v201_t;

//typedef struct stat3a_lightType_s
typedef struct rk_aiq_awb_stat_wp_res_v201_s {
    long long WpNo;
    long long RgainValue;
    long long BgainValue;
} rk_aiq_awb_stat_wp_res_v201_t;

typedef struct rk_aiq_awb_stat_blk_res_v201_s {
    long long WpNo;
    long long Rvalue;
    long long Gvalue;
    long long Bvalue;
} rk_aiq_awb_stat_blk_res_v201_t;

typedef struct rk_aiq_awb_stat_wp_res_light_v201_s {
    rk_aiq_awb_stat_wp_res_v201_t xYType[RK_AIQ_AWB_XY_TYPE_MAX_V201];
} rk_aiq_awb_stat_wp_res_light_v201_t;

typedef struct rk_aiq_awb_stat_res_v201_s {
    //method1
    rk_aiq_awb_stat_wp_res_light_v201_t light[RK_AIQ_AWB_MAX_WHITEREGIONS_NUM];
#ifdef AWB_WPNUM2_EN
    int WpNo2[RK_AIQ_AWB_MAX_WHITEREGIONS_NUM];
#endif
    //method2
    rk_aiq_awb_stat_blk_res_v201_t   blockResult[RK_AIQ_AWB_GRID_NUM_TOTAL];
#ifdef AWB_MULTI_WINDOW_EN
    //window in pixel domain
    rk_aiq_awb_stat_wp_res_light_v201_t multiwindowLightResult[RK_AIQ_AWB_MAX_WHITEREGIONS_NUM];
#endif
#ifdef AWB_EXCWP_RANGE_STAT_EN
    //window in xy or uv domain
    rk_aiq_awb_stat_wp_res_v201_t excWpRangeResult[RK_AIQ_AWB_STAT_WP_RANGE_NUM_V201];
#endif
    //wpno histogram
    unsigned int WpNoHist[RK_AIQ_AWB_WP_HIST_BIN_NUM];

    rk_aiq_awb_stat_cfg_v201_t  awb_cfg_effect_v201;
} rk_aiq_awb_stat_res_v201_t;


typedef struct rk_aiq_awb_stat_res2_v201_s {
    //method1
    rk_aiq_awb_stat_wp_res_light_v201_t light[RK_AIQ_AWB_MAX_WHITEREGIONS_NUM];
    //method2
    rk_aiq_awb_stat_blk_res_v201_t   blockResult[RK_AIQ_AWB_GRID_NUM_TOTAL];
    //wpno histogram
    unsigned int WpNoHist[RK_AIQ_AWB_WP_HIST_BIN_NUM];
} rk_aiq_awb_stat_res2_v201_t;


typedef struct rk_aiq_awb_stat_res2_v30_s {
    //method1
    rk_aiq_awb_stat_wp_res_light_v201_t light[RK_AIQ_AWB_MAX_WHITEREGIONS_NUM];
    int WpNo2[RK_AIQ_AWB_MAX_WHITEREGIONS_NUM];
    //method2
    rk_aiq_awb_stat_blk_res_v201_t   blockResult[RK_AIQ_AWB_GRID_NUM_TOTAL];
    //window in pixel domain
    rk_aiq_awb_stat_wp_res_light_v201_t multiwindowLightResult[4];
    //window in xy or uv domain
    rk_aiq_awb_stat_wp_res_v201_t excWpRangeResult[RK_AIQ_AWB_STAT_WP_RANGE_NUM_V201];
    //wpno histogram
    unsigned int WpNoHist[RK_AIQ_AWB_WP_HIST_BIN_NUM];
} rk_aiq_awb_stat_res2_v30_t;



#endif
