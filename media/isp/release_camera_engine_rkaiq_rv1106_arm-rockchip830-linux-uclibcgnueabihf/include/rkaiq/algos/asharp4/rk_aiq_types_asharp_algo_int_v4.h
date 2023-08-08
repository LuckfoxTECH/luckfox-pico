/*
 *rk_aiq_types_alsc_algo_int.h
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

#ifndef _RK_AIQ_TYPE_ASHARP_ALGO_INT_V4_H_
#define _RK_AIQ_TYPE_ASHARP_ALGO_INT_V4_H_

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "rk_aiq_comm.h"
#include "RkAiqCalibDbTypes.h"
#include "asharp4/rk_aiq_types_asharp_algo_v4.h"
#include "sharp_head_v4.h"


RKAIQ_BEGIN_DECLARE
#define RK_SHARP_V4_MAX_ISO_NUM             CALIBDB_MAX_ISO_LEVEL

#define RK_SHARP_V4_LUMA_POINT_NUM             (8)

#define ASHARPV4_RECALCULATE_DELTA_ISO        (10)
#define RK_SHARPV4_PBF_DIAM                    3
#define RK_SHARPV4_RF_DIAM                     5
#define RK_SHARPV4_BF_DIAM                     3
#define RK_SHARPV4_AVG_DIAM                    3

#define rk_sharpV4_sharp_ratio_fix_bits        2
#define rk_sharpV4_gaus_ratio_fix_bits         7
#define rk_sharpV4_bf_ratio_fix_bits           7
#define rk_sharpV4_pbfCoeff_fix_bits           7
#define rk_sharpV4_rfCoeff_fix_bits            7
#define rk_sharpV4_hbfCoeff_fix_bits           7

#define INTERP_V4(x0, x1, ratio)            ((ratio) * ((x1) - (x0)) + x0)
#define CLIP(a, min_v, max_v)               (((a) < (min_v)) ? (min_v) : (((a) > (max_v)) ? (max_v) : (a)))


#if 1
typedef enum Asharp4_result_e {
    ASHARP4_RET_SUCCESS             = 0,   // this has to be 0, if clauses rely on it
    ASHARP4_RET_FAILURE             = 1,   // process failure
    ASHARP4_RET_INVALID_PARM        = 2,   // invalid parameter
    ASHARP4_RET_WRONG_CONFIG        = 3,   // feature not supported
    ASHARP4_RET_BUSY                = 4,   // there's already something going on...
    ASHARP4_RET_CANCELED            = 5,   // operation canceled
    ASHARP4_RET_OUTOFMEM            = 6,   // out of memory
    ASHARP4_RET_OUTOFRANGE          = 7,   // parameter/value out of range
    ASHARP4_RET_NULL_POINTER        = 8,   // the/one/all parameter(s) is a(are) NULL pointer(s)
    ASHARP4_RET_DIVISION_BY_ZERO    = 9,   // a divisor equals ZERO
    ASHARP4_RET_NO_INPUTIMAGE       = 10   // no input image
} Asharp4_result_t;

typedef enum Asharp4_State_e {
    ASHARP4_STATE_INVALID           = 0,                   /**< initialization value */
    ASHARP4_STATE_INITIALIZED       = 1,                   /**< instance is created, but not initialized */
    ASHARP4_STATE_STOPPED           = 2,                   /**< instance is confiured (ready to start) or stopped */
    ASHARP4_STATE_RUNNING           = 3,                   /**< instance is running (processes frames) */
    ASHARP4_STATE_LOCKED            = 4,                   /**< instance is locked (for taking snapshots) */
    ASHARP4_STATE_MAX                                      /**< max */
} Asharp4_State_t;

typedef enum Asharp4_OPMode_e {
    ASHARP4_OP_MODE_INVALID           = 0,                   /**< initialization value */
    ASHARP4_OP_MODE_AUTO              = 1,                   /**< instance is created, but not initialized */
    ASHARP4_OP_MODE_MANUAL            = 2,                   /**< instance is confiured (ready to start) or stopped */
    ASHARP4_OP_MODE_REG_MANUAL        = 3,
    ASHARP4_OP_MODE_MAX                                      /**< max */
} Asharp4_OPMode_t;

typedef enum Asharp4_ParamMode_e {
    ASHARP4_PARAM_MODE_INVALID           = 0,
    ASHARP4_PARAM_MODE_NORMAL          = 1,                   /**< initialization value */
    ASHARP4_PARAM_MODE_HDR              = 2,                   /**< instance is created, but not initialized */
    ASHARP4_PARAM_MODE_GRAY            = 3,                   /**< instance is confiured (ready to start) or stopped */
    ASHARP4_PARAM_MODE_MAX                                      /**< max */
} Asharp4_ParamMode_t;

#endif

typedef struct Asharp4_ExpInfo_s {
    int hdr_mode;
    float arTime[3];
    float arAGain[3];
    float arDGain[3];
    int   arIso[3];
    int   snr_mode;
    int rawWidth;
    int rawHeight;
} Asharp4_ExpInfo_t;


typedef struct RK_SHARP_Params_V4_s
{
    int enable;
    int kernel_sigma_enable;

    int iso[RK_SHARP_V4_MAX_ISO_NUM];
    short luma_point        [RK_SHARP_V4_LUMA_POINT_NUM];
    short luma_sigma        [RK_SHARP_V4_MAX_ISO_NUM][RK_SHARP_V4_LUMA_POINT_NUM];
    float pbf_gain          [RK_SHARP_V4_MAX_ISO_NUM];
    float pbf_add           [RK_SHARP_V4_MAX_ISO_NUM];
    float pbf_ratio         [RK_SHARP_V4_MAX_ISO_NUM];
    float gaus_ratio        [RK_SHARP_V4_MAX_ISO_NUM];
    float sharp_ratio       [RK_SHARP_V4_MAX_ISO_NUM];
    short hf_clip        [RK_SHARP_V4_MAX_ISO_NUM][RK_SHARP_V4_LUMA_POINT_NUM];
    float bf_gain           [RK_SHARP_V4_MAX_ISO_NUM];
    float bf_add            [RK_SHARP_V4_MAX_ISO_NUM];
    float bf_ratio          [RK_SHARP_V4_MAX_ISO_NUM];
    short local_sharp_strength  [RK_SHARP_V4_MAX_ISO_NUM][RK_SHARP_V4_LUMA_POINT_NUM];

    float prefilter_coeff[RK_SHARP_V4_MAX_ISO_NUM][RK_SHARPV4_PBF_DIAM * RK_SHARPV4_PBF_DIAM];
    float GaussianFilter_coeff   [RK_SHARP_V4_MAX_ISO_NUM][RK_SHARPV4_RF_DIAM * RK_SHARPV4_RF_DIAM];
    float hfBilateralFilter_coeff    [RK_SHARP_V4_MAX_ISO_NUM][RK_SHARPV4_BF_DIAM * RK_SHARPV4_BF_DIAM];

    float prefilter_sigma[RK_SHARP_V4_MAX_ISO_NUM];
    float GaussianFilter_sigma[RK_SHARP_V4_MAX_ISO_NUM];
    float GaussianFilter_radius[RK_SHARP_V4_MAX_ISO_NUM];
    float hfBilateralFilter_sigma[RK_SHARP_V4_MAX_ISO_NUM];


} RK_SHARP_Params_V4_t;

typedef struct RK_SHARP_Params_V4_Select_s
{
    int enable;
    int kernel_sigma_enable;

    short luma_point    [RK_SHARP_V4_LUMA_POINT_NUM];
    short luma_sigma    [RK_SHARP_V4_LUMA_POINT_NUM];
    float pbf_gain      ;
    float pbf_add       ;
    float pbf_ratio     ;
    float gaus_ratio    ;
    float sharp_ratio   ;
    short hf_clip       [RK_SHARP_V4_LUMA_POINT_NUM];
    float bf_gain       ;
    float bf_add        ;
    float bf_ratio      ;
    short local_sharp_strength        [RK_SHARP_V4_LUMA_POINT_NUM];

    float prefilter_coeff[RK_SHARPV4_PBF_DIAM * RK_SHARPV4_PBF_DIAM];
    float GaussianFilter_coeff   [RK_SHARPV4_RF_DIAM * RK_SHARPV4_RF_DIAM];
    float hfBilateralFilter_coeff    [RK_SHARPV4_BF_DIAM * RK_SHARPV4_BF_DIAM];

    float prefilter_sigma;
    float GaussianFilter_sigma;
    float GaussianFilter_radius;
    float hfBilateralFilter_sigma;

} RK_SHARP_Params_V4_Select_t;


typedef struct Asharp_Manual_Attr_V4_s
{
    RK_SHARP_Params_V4_Select_t stSelect;

    RK_SHARP_Fix_V4_t stFix;

} Asharp_Manual_Attr_V4_t;

typedef struct Asharp_Auto_Attr_V4_s
{
    //all ISO params and select param

    RK_SHARP_Params_V4_t stParams;
    RK_SHARP_Params_V4_Select_t stSelect;

} Asharp_Auto_Attr_V4_t;

typedef struct Asharp_ProcResult_V4_s {
    int sharpEn;

    //for sw simultaion
    RK_SHARP_Params_V4_Select_t stSelect;

    //for hw register
    RK_SHARP_Fix_V4_t stFix;

    bool isNeedUpdate;

} Asharp_ProcResult_V4_t;


typedef struct Asharp_Config_V4_s {
    Asharp4_State_t eState;
    Asharp4_OPMode_t eMode;
    int rawHeight;
    int rawWidth;
} Asharp_Config_V4_t;


typedef struct rk_aiq_sharp_attrib_v4_s {
    rk_aiq_uapi_sync_t sync;

    Asharp4_OPMode_t eMode;
    Asharp_Auto_Attr_V4_t stAuto;
    Asharp_Manual_Attr_V4_t stManual;
} rk_aiq_sharp_attrib_v4_t;


typedef struct rk_aiq_sharp_strength_v4_s {
    rk_aiq_uapi_sync_t sync;

    float percent;
} rk_aiq_sharp_strength_v4_t;



//calibdb


RKAIQ_END_DECLARE

#endif

