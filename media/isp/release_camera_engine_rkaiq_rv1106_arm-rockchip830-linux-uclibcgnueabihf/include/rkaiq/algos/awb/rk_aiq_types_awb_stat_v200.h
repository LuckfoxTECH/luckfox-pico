/*
* rk_aiq_types_awb_stat_v200.h

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

#ifndef __RK_AIQ_TYPE_AWB_STAT_V200_H__
#define __RK_AIQ_TYPE_AWB_STAT_V200_H__

#include "rk_aiq_types_awb_stat_v2xx.h"

#define RK_AIQ_AWB_MULTIWINDOW_NUM 8
#define RK_AIQ_AWB_STAT_WP_RANGE_NUM_V200 7

typedef enum rk_aiq_awb_xy_type_v200_e {
    RK_AIQ_AWB_XY_TYPE_NORMAL_V200 = 0,
    RK_AIQ_AWB_XY_TYPE_BIG_V200 = 1,
    RK_AIQ_AWB_XY_TYPE_SMALL_V200 = 2,
    RK_AIQ_AWB_XY_TYPE_MAX_V200
} rk_aiq_awb_xy_type_v200_t;

typedef enum rk_aiq_blk_stat_mode_v200_e {
    RK_AIQ_AWB_BLK_STAT_MODE_MIN_V200 = 0,
    RK_AIQ_AWB_BLK_STAT_MODE_ALL_V200 = 1,
    RK_AIQ_AWB_BLK_STAT_MODE_UVWP_V200 = 2,
    RK_AIQ_AWB_BLK_STAT_MODE_XYWP_V200 = 3,
    RK_AIQ_AWB_BLK_STAT_MODE_MAX_V200
} rk_aiq_blk_stat_mode_v200_t;//cxf333 repeat
typedef struct rk_aiq_awb_exc_range_s {
    rk_aiq_awb_exc_range_domain_t domain;
    bool excludeEnable;// 0 disable,1 eanble;
    bool measureEnable;// 1 calculate sum value in the range,0 don't meausre
    int xu[2];//left right
    int yv[2];//top bottom
} rk_aiq_awb_exc_range_t;

typedef struct rk_aiq_awb_stat_cfg_v200_s {
    bool awbEnable;
    bool lscBypEnable;
    bool uvDetectionEnable;
    bool xyDetectionEnable;
    unsigned short storeWpFlagIllu[RK_AIQ_AWB_STORE_LS_WPFLAG_NUM];
    unsigned short threeDyuvIllu[RK_AIQ_AWB_YUV_LS_PARA_NUM];
    rk_aiq_down_scale_mode_t dsMode;
    rk_aiq_blk_stat_mode_v200_t blkMeasureMode;
    unsigned short blkMeasWpTh[3];
    bool multiwindow_en;
    uint8_t frameChoose;//default value is 0,support to choose long frame ,middle frame or short frame when hdr is on
    uint8_t rgb2yuv_c_range;
    uint8_t rgb2yuv_y_range;
    short rgb2yuv_matrix[9];
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
    rk_aiq_awb_uv_range_para_t         uvRange_param[RK_AIQ_AWB_MAX_WHITEREGIONS_NUM];//A CWF D50 D65 D75 HZ TL84
    rk_aiq_awb_3dyuv_range_para_t       yuvRange_param[RK_AIQ_AWB_YUV_LS_PARA_NUM];
    rk_aiq_rgb2xy_para_t     rgb2xy_param;
    rk_aiq_awb_xy_range_para_t   xyRange_param[RK_AIQ_AWB_MAX_WHITEREGIONS_NUM];
    //several window in pixel domain
    unsigned short multiwindow[RK_AIQ_AWB_MULTIWINDOW_NUM][4];//8  windows in pixel domain ,hOffset,vOffser,hSize,vSize;
    //several winow in uv or xy domain
    rk_aiq_awb_exc_range_t excludeWpRange[RK_AIQ_AWB_EXCLUDE_WP_RANGE_NUM];

    int  groupIllIndxCurrent;//for time share
    int  IllIndxSetCurrent[RK_AIQ_AWB_MAX_WHITEREGIONS_NUM];//for time share
    char timeSign[64];

} rk_aiq_awb_stat_cfg_v200_t;

typedef struct rk_aiq_awb_stat_wp_res_v200_s {
    unsigned int WpNo;
    unsigned int Rvalue;
    unsigned int Gvalue;
    unsigned int Bvalue;
} rk_aiq_awb_stat_wp_res_v200_t;

typedef struct rk_aiq_awb_stat_blk_res_v200_s {
    unsigned int Rvalue;
    unsigned int Gvalue;
    unsigned int Bvalue;
    bool isWP[RK_AIQ_AWB_STORE_LS_WPFLAG_NUM]; //record wp flag in three light sources
} rk_aiq_awb_stat_blk_res_v200_t;

typedef struct rk_aiq_awb_stat_wp_res_light_v200_s {
    rk_aiq_awb_stat_wp_res_v200_t xYType[RK_AIQ_AWB_XY_TYPE_MAX_V200];
} rk_aiq_awb_stat_wp_res_light_v200_t;

typedef struct rk_aiq_awb_stat_res_v200_s {
    rk_aiq_awb_stat_wp_res_light_v200_t light[RK_AIQ_AWB_MAX_WHITEREGIONS_NUM];
    rk_aiq_awb_stat_blk_res_v200_t   blockResult[RK_AIQ_AWB_GRID_NUM_TOTAL];
    //window in pixel domain
    rk_aiq_awb_stat_wp_res_light_v200_t multiwindowLightResult[RK_AIQ_AWB_MAX_WHITEREGIONS_NUM];
    //window in xy or uv domain
    rk_aiq_awb_stat_wp_res_v200_t excWpRangeResult[RK_AIQ_AWB_STAT_WP_RANGE_NUM_V200];

    rk_aiq_awb_stat_cfg_v200_t  awb_cfg_effect_v200;
} rk_aiq_awb_stat_res_v200_t;

#endif
