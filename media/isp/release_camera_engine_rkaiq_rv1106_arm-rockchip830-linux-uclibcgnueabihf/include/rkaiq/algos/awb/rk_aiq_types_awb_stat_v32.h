/*
* rk_aiq_types_awb_stat_v32.h

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

#ifndef __RK_AIQ_TYPE_AWB_STAT_V32_H__
#define __RK_AIQ_TYPE_AWB_STAT_V32_H__

#include "rk_aiq_types_awb_stat_v201.h"
#define RK_AIQ_AWB_MAX_WHITEREGIONS_NUM_V32    4
#if RKAIQ_HAVE_AWB_V32
#define AWB_MULTI_WINDOW_EN
#define AWB_EXCWP_RANGE_STAT_EN
#define AWB_WPNUM2_EN
#endif
#define RK_AIQ_WP_INCLUDE_BIS 6
typedef struct rk_aiq_awb_exc_range_v32_s {
    rk_aiq_awb_exc_range_domain_t domain;//1 uv domain,2 xy domain;
    bool excludeEnable[RK_AIQ_AWB_XY_TYPE_MAX_V201];// 0 disable,1 eanble;
    bool measureEnable;//1 calculate sum r g b value in the range,0 not meausre
    int xu[2];//left right
    int yv[2];//top bottom
    unsigned int weightInculde;
} rk_aiq_awb_exc_range_v32_t;

typedef struct rk_aiq_awb_blc_s {
    bool enable;
    int blc[4];//r gr gb b;
} rk_aiq_awb_blc_t;

typedef struct rk_aiq_awb_stat_cfg_v32_s {
    bool awbEnable;
    bool lscBypEnable;
    bool inputBitIs12Bit;
    bool write2ddrEnable;
    int  write2ddrSelc;
    bool inputShiftEnable;//right shift to 12 bit
    rk_aiq_awb_blc_t blc;
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
    bool multiwindow_en;
    uint8_t frameChoose;
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
    int overexposure_value; //if overexposure_value ==0 ,will auto calc;
    short          icrgb2RYuv_matrix[12];//3*3 matrix 9bit fracitional +0bit integer+1s
    //  3*1 off    4bit fractional +8bit integer+1s
    rk_aiq_awb_rt3dyuv_range_para_t     ic3Dyuv2Range_param[RK_AIQ_AWB_YUV_LS_PARA_NUM];
    rk_aiq_awb_uv_range_para_t            uvRange_param[RK_AIQ_AWB_MAX_WHITEREGIONS_NUM_V32];//A CWF D50 D65 D75 HZ TL84
    rk_aiq_rgb2xy_para_t      rgb2xy_param;
    rk_aiq_awb_xy_range_para_t    xyRange_param[RK_AIQ_AWB_MAX_WHITEREGIONS_NUM_V32];
    //several window in pixel domain
    unsigned short multiwindow[RK_AIQ_AWB_MULTIWINDOW_NUM_V201][4];// 4  windows in pixel domain ,hOffset,vOffser,hSize,vSize;
    //several winow in uv or xy domain
    rk_aiq_awb_exc_range_v32_t excludeWpRange[RK_AIQ_AWB_EXCLUDE_WP_RANGE_NUM];
    //with differernt luma ,the different weight in WP sum
    unsigned char wpDiffwei_y[RK_AIQ_AWBWP_WEIGHT_CURVE_DOT_NUM];//8bit
    unsigned char wpDiffwei_w[RK_AIQ_AWBWP_WEIGHT_CURVE_DOT_NUM];//8bit fix point ,and wpDiffwei_w[4]=wpDiffwei_w[5]=255;
    rk_aiq_awb_xy_type_v201_t xyRangeTypeForWpHist; //for xyRangeT
    unsigned char blkWeight[RK_AIQ_AWB_GRID_NUM_TOTAL];
    rk_aiq_awb_xy_type_v201_t xyRangeTypeForBlkStatistics; //used when blkMeasureMode>BLK_MEASURE_MODE_ALL
    rk_aiq_awb_blk_stat_realwp_ill_e illIdxForBlkStatistics; //blkMeasureMode used when blkMeasureMode>BLK_MEASURE_MODE_ALL
    bool blkStatisticsWithLumaWeightEn;
    int  groupIllIndxCurrent;//for time share
    int  IllIndxSetCurrent[RK_AIQ_AWB_MAX_WHITEREGIONS_NUM_V32];//for time share
    char timeSign[64];
} rk_aiq_awb_stat_cfg_v32_t;

typedef struct rk_aiq_awb_stat_cfg_effect_v32_s {
    rk_aiq_awb_blk_stat_mode_v201_t blkMeasureMode;
    unsigned char lightNum;
    int  groupIllIndxCurrent;//for time share
    int  IllIndxSetCurrent[RK_AIQ_AWB_MAX_WHITEREGIONS_NUM_V32];//for time share
    char timeSign[64];
} rk_aiq_awb_stat_cfg_effect_v32_t;

typedef struct rk_aiq_awb_stat_res_v32_s {
    //method1
    rk_aiq_awb_stat_wp_res_light_v201_t light[RK_AIQ_AWB_MAX_WHITEREGIONS_NUM_V32];
    int WpNo2[RK_AIQ_AWB_MAX_WHITEREGIONS_NUM_V32];
    //method2
    rk_aiq_awb_stat_blk_res_v201_t   blockResult[RK_AIQ_AWB_GRID_NUM_TOTAL];
    //window in xy or uv domain
    rk_aiq_awb_stat_wp_res_v201_t excWpRangeResult[RK_AIQ_AWB_STAT_WP_RANGE_NUM_V201];
    //wpno histogram
    unsigned int WpNoHist[RK_AIQ_AWB_WP_HIST_BIN_NUM];
    rk_aiq_awb_stat_cfg_effect_v32_t  awb_cfg_effect_v32;
    int dbginfo_fd;
} rk_aiq_awb_stat_res_v32_t;

#endif
