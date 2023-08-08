/*
* rk_aiq_awb_api_hw_v200.h

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

#ifndef __RK_AIQ_TYPE_AWB_STAT_V2XX_H__
#define __RK_AIQ_TYPE_AWB_STAT_V2XX_H__
#include "rk_aiq_comm.h"
//#define TIME_SHARE_TEST
#define TIME_SHARE_MODE
#define RK_AIQ_AWB_STAT_YUV_ILL_OFF 7
#define RK_AIQ_AWB_MAX_WHITEREGIONS_NUM       7
#define RK_AIQ_AWB_YUV_LS_PARA_NUM 4
#define RK_AIQ_AWB_GRID_NUM_VERHOR           15
#define RK_AIQ_AWB_GRID_NUM_TOTAL   (RK_AIQ_AWB_GRID_NUM_VERHOR*RK_AIQ_AWB_GRID_NUM_VERHOR)
#define RK_AIQ_AWB_STORE_LS_WPFLAG_NUM 3
#define RK_AIQ_AWB_EXCLUDE_WP_RANGE_NUM 7

#define AWB_UV_RANGE_SLOVE_FRACTION 10
#define AWB_UV_RANGE_SLOVE_SINTER 9

typedef enum rk_aiq_down_scale_mode_e {
    RK_AIQ_AWB_DS_INVALIED = -1,
    RK_AIQ_AWB_DS_4X4 = 0,
    RK_AIQ_AWB_DS_8X8 = 1,
    RK_AIQ_AWB_DS_MAX
} rk_aiq_down_scale_mode_t;//cxf333 repeat

typedef struct rk_aiq_rgb2xy_para_s {
    unsigned short pseudoLuminanceWeight[3];
    short rotationMat[9];
} rk_aiq_rgb2xy_para_t;
typedef struct rk_aiq_awb_xy_range_para_s {
    int NorrangeX[2];//left right
    int NorrangeY[2];//top bottom
    int SperangeX[2];
    int SperangeY[2];
    int SmalrangeX[2];
    int SmalrangeY[2];
} rk_aiq_awb_xy_range_para_t;

typedef struct rk_aiq_awb_uv_range_para_s {
    unsigned short pu_region[5];//9 bit
    unsigned short pv_region[5];
    int slope_inv[4];//10+10 signed 1/k
} rk_aiq_awb_uv_range_para_t;

typedef struct rk_aiq_awb_3dyuv_range_para_s {
    int b_uv;            //17+0 s
    int slope_inv_neg_uv; //9+10s //-1/k
    int slope_factor_uv; //1+14s // 1/(-1/k+k)
    int slope_ydis;        //9+10s //k
    int b_ydis;            //17+0s
    unsigned char ref_u;   //8u
    unsigned char ref_v;   //8u
    unsigned short dis[6];//8+4 u
    unsigned char th[6]; //8u
} rk_aiq_awb_3dyuv_range_para_t;

typedef struct rk_aiq_awb_rt3dyuv_range_para_s {
    unsigned char thcurve_u[6];//0bit fracitonal+8bit integer+0s
    unsigned short thcure_th[6];//4bit fractional+8bit integer+0s
    unsigned short lineP1[3];//4bit fractional+8bit integer+0s
    short vP1P2[3];//4bit fractional+8bit integer+1s
    unsigned char  disP1P2;// /0bit fractional+5bit integer+0s  distance is 2^disP1P2;
} rk_aiq_awb_rt3dyuv_range_para_t;

typedef enum rk_aiq_awb_exc_range_domain_e {
    RK_AIQ_AWB_EXC_RANGE_DOMAIN_UV_INVALID = -1,
    RK_AIQ_AWB_EXC_RANGE_DOMAIN_UV = 0,
    RK_AIQ_AWB_EXC_RANGE_DOMAIN_XY = 1
} rk_aiq_awb_exc_range_domain_t;


typedef enum rk_aiq_awb_bayer_mode_e {
    RK_AIQ_AWB_BAYER_MODE_MIN,
    RK_AIQ_AWB_BAYER_MODE_RGGB = 0,
    RK_AIQ_AWB_BAYER_MODE_GRBG = 1,
    RK_AIQ_AWB_BAYER_MODE_GBRG = 2,
    RK_AIQ_AWB_BAYER_MODE_BGGR = 3,
    RK_AIQ_AWB_BAYER_MODE_MAX
} rk_aiq_awb_bayer_mode_t;


#endif

