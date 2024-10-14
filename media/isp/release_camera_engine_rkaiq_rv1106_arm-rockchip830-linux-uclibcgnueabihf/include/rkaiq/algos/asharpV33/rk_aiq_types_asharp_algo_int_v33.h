/*
 *rk_aiq_types_asharp_algo_int.h
 *
 *  Copyright (c) 2022 Rockchip Corporation
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

#ifndef _RK_AIQ_TYPES_ASHARP_ALOG_INT_V33_H_
#define _RK_AIQ_TYPES_ASHARP_ALOG_INT_V33_H_

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "RkAiqCalibDbTypes.h"
#include "asharpV33/rk_aiq_types_asharp_algo_v33.h"
#include "rk_aiq_comm.h"
#include "sharp_head_v33.h"
#include "sharp_uapi_head_v33.h"



#define ASHARPV33_RECALCULATE_DELTA_ISO       (10)
#define RK_SHARP_V33_MAX_ISO_NUM    CALIBDB_MAX_ISO_LEVEL


#define RK_SHARP_V33_SHARP_RATIO_FIX_BITS 2
#define RK_SHARP_V33_GAUS_RATIO_FIX_BITS  7
#define RK_SHARP_V33_BF_RATIO_FIX_BITS    7
#define RK_SHARP_V33_PBFCOEFF_FIX_BITS    7
#define RK_SHARP_V33_RFCOEFF_FIX_BITS     7
#define RK_SHARP_V33_HBFCOEFF_FIX_BITS    7

#define RK_SHARP_V33_GLOBAL_GAIN_FIX_BITS       4
#define RK_SHARP_V33_GLOBAL_GAIN_ALPHA_FIX_BITS 3
#define RK_SHARP_V33_LOCAL_GAIN_FIX_BITS        4
#define RK_SHARP_V33_LOCAL_GAIN_SACLE_FIX_BITS  7
#define RK_SHARP_V33_ADJ_GAIN_FIX_BITS          10
#define RK_SHARP_V33_STRENGTH_TABLE_FIX_BITS    7

#define INTERP_V4(x0, x1, ratio) ((ratio) * ((x1) - (x0)) + x0)
#define CLIP(a, min_v, max_v)    (((a) < (min_v)) ? (min_v) : (((a) > (max_v)) ? (max_v) : (a)))

typedef enum Asharp_result_V33_e {
    ASHARP_V33_RET_SUCCESS          = 0,  // this has to be 0, if clauses rely on it
    ASHARP_V33_RET_FAILURE          = 1,  // process failure
    ASHARP_V33_RET_INVALID_PARM     = 2,  // invalid parameter
    ASHARP_V33_RET_WRONG_CONFIG     = 3,  // feature not supported
    ASHARP_V33_RET_BUSY             = 4,  // there's already something going on...
    ASHARP_V33_RET_CANCELED         = 5,  // operation canceled
    ASHARP_V33_RET_OUTOFMEM         = 6,  // out of memory
    ASHARP_V33_RET_OUTOFRANGE       = 7,  // parameter/value out of range
    ASHARP_V33_RET_NULL_POINTER     = 8,  // the/one/all parameter(s) is a(are) NULL pointer(s)
    ASHARP_V33_RET_DIVISION_BY_ZERO = 9,  // a divisor equals ZERO
    ASHARP_V33_RET_NO_INPUTIMAGE    = 10  // no input image
} Asharp_result_V33_t;

typedef enum Asharp_State_V33_e {
    ASHARP_V33_STATE_INVALID     = 0, /**< initialization value */
    ASHARP_V33_STATE_INITIALIZED = 1, /**< instance is created, but not initialized */
    ASHARP_V33_STATE_STOPPED     = 2, /**< instance is confiured (ready to start) or stopped */
    ASHARP_V33_STATE_RUNNING     = 3, /**< instance is running (processes frames) */
    ASHARP_V33_STATE_LOCKED      = 4, /**< instance is locked (for taking snapshots) */
    ASHARP_V33_STATE_MAX              /**< max */
} Asharp_State_V33_t;

typedef enum Asharp_OPMode_V33_e {
    ASHARP_V33_OP_MODE_INVALID    = 0, /**< initialization value */
    ASHARP_V33_OP_MODE_AUTO       = 1, /**< instance is created, but not initialized */
    ASHARP_V33_OP_MODE_MANUAL     = 2, /**< instance is confiured (ready to start) or stopped */
    ASHARP_V33_OP_MODE_REG_MANUAL = 3,
    ASHARP_V33_OP_MODE_MAX /**< max */
} Asharp_OPMode_V33_t;

typedef enum Asharp_ParamMode_V33_e {
    ASHARP_V33_PARAM_MODE_INVALID = 0,
    ASHARP_V33_PARAM_MODE_NORMAL  = 1, /**< initialization value */
    ASHARP_V33_PARAM_MODE_HDR     = 2, /**< instance is created, but not initialized */
    ASHARP_V33_PARAM_MODE_GRAY    = 3, /**< instance is confiured (ready to start) or stopped */
    ASHARP_V33_PARAM_MODE_MAX          /**< max */
} Asharp_ParamMode_V33_t;



#if 0
// sharp params related to ISO
typedef struct RK_SHARP_V33_Select_ISO_s {
    int luma_sigma[RK_SHARP_V33_LUMA_POINT_NUM];
    int luma_point[RK_SHARP_V33_LUMA_POINT_NUM];
    float pbf_gain;
    float pbf_add;
    float pbf_ratio;
    float gaus_ratio;
    float sharp_ratio;
    int lum_clip_h[RK_SHARP_V33_LUMA_POINT_NUM];
    float bf_gain;
    float bf_add;
    float bf_ratio;
    int global_clip_pos;

    float prefilter_coeff[RK_SHARP_V33_PBF_DIAM * RK_SHARP_V33_PBF_DIAM];
    float GaussianFilter_coeff[RK_SHARP_V33_RF_DIAM * RK_SHARP_V33_RF_DIAM];
    float hfBilateralFilter_coeff[RK_SHARP_V33_BF_DIAM * RK_SHARP_V33_BF_DIAM];

    float global_gain;
    float global_gain_alpha;
    float local_gainscale;

    float gain_adj_sharp_strength[RK_SHARP_V33_SHARP_ADJ_GAIN_TABLE_LEN];
    int exgain_bypass;

    float dis_adj_sharp_strength[RK_SHARP_V33_STRENGTH_TABLE_LEN];

    float noiseclip_strength;
    int enhance_bit;
    int noiseclip_mode;
    int noise_sigma_clip;

    float prefilter_sigma;
    float hfBilateralFilter_sigma;
    float GaussianFilter_sigma;
    float GaussianFilter_radius;

} RK_SHARP_V33_Select_ISO_t;

// sharp params select
typedef struct RK_SHARP_Params_V33_Select_s {
    int enable;
    int kernel_sigma_enable;

    int center_mode;
    int center_x;
    int center_y;

    RK_SHARP_V33_Select_ISO_t sharpParamsSelectISO;

} RK_SHARP_Params_V33_Select_t;
#endif


// sharp params
typedef struct RK_SHARP_Params_V33_s {
    int enable;
    int sharp_ratio_seperate_en;
    int kernel_sigma_enable;
    char version[64];


    int Center_Mode;
    int center_x;
    int center_y;

    int iso[RK_SHARP_V33_MAX_ISO_NUM];
    RK_SHARP_Params_V33_Select_t sharpParamsISO[RK_SHARP_V33_MAX_ISO_NUM];

} RK_SHARP_Params_V33_t;

typedef struct Asharp_Manual_Attr_V33_s {
    RK_SHARP_Params_V33_Select_t stSelect;

    RK_SHARP_Fix_V33_t stFix;

} Asharp_Manual_Attr_V33_t;

typedef struct Asharp_Auto_Attr_V33_s {
    // all ISO params and select param

    RK_SHARP_Params_V33_t stParams;
    RK_SHARP_Params_V33_Select_t stSelect;

} Asharp_Auto_Attr_V33_t;

// for isp32 lite version
typedef struct RK_SHARP_Params_V33LT_s {
    int enable;
    int sharp_ratio_seperate_en;
    int kernel_sigma_enable;
    char version[64];

    int Center_Mode;
    int center_x;
    int center_y;

    int iso[RK_SHARP_V33_MAX_ISO_NUM];
    RK_SHARP_Params_V33LT_Select_t sharpParamsISO[RK_SHARP_V33_MAX_ISO_NUM];

} RK_SHARP_Params_V33LT_t;

typedef struct Asharp_Manual_Attr_V33LT_s {
    RK_SHARP_Params_V33LT_Select_t stSelect;

    RK_SHARP_Fix_V33_t stFix;

} Asharp_Manual_Attr_V33LT_t;

typedef struct Asharp_Auto_Attr_V33LT_s {
    // all ISO params and select param

    RK_SHARP_Params_V33LT_t stParams;
    RK_SHARP_Params_V33LT_Select_t stSelect;

} Asharp_Auto_Attr_V33LT_t;

typedef struct Asharp_ProcResult_V33_s {
#if RKAIQ_HAVE_SHARP_V33
    // for sw simultaion
    // RK_SHARP_Params_V33_Select_t stSelect;
#else
    // for isp32 lite version
    // for sw simultaion
    //RK_SHARP_Params_V33LT_Select_t stSelect;
#endif

    // for hw register
    RK_SHARP_Fix_V33_t* stFix;
} Asharp_ProcResult_V33_t;

typedef struct Asharp_Config_V33_s {
    Asharp_State_V33_t eState;
    Asharp_OPMode_V33_t eMode;
    int rawHeight;
    int rawWidth;
} Asharp_Config_V33_t;

typedef struct rk_aiq_sharp_attrib_v33_s {
    rk_aiq_uapi_sync_t sync;

    Asharp_OPMode_V33_t eMode;
    Asharp_Auto_Attr_V33_t stAuto;
    Asharp_Manual_Attr_V33_t stManual;
} rk_aiq_sharp_attrib_v33_t;

typedef struct rk_aiq_sharp_attrib_v33LT_s {
    rk_aiq_uapi_sync_t sync;

    Asharp_OPMode_V33_t eMode;
    Asharp_Auto_Attr_V33LT_t stAuto;
    Asharp_Manual_Attr_V33LT_t stManual;
} rk_aiq_sharp_attrib_v33LT_t;

typedef struct rk_aiq_sharp_strength_v33_s {
    rk_aiq_uapi_sync_t sync;

    float percent;
    bool strength_enable;
} rk_aiq_sharp_strength_v33_t;




#endif
