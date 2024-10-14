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

#ifndef _RK_AIQ_TYPE_ASHARP_ALGO_INT_V3_H_
#define _RK_AIQ_TYPE_ASHARP_ALGO_INT_V3_H_

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "rk_aiq_comm.h"
#include "RkAiqCalibDbTypes.h"
#include "asharp3/rk_aiq_types_asharp_algo_v3.h"
#include "sharp_head_v3.h"


RKAIQ_BEGIN_DECLARE
#define ASHARPV3_RECALCULATE_DELTA_ISO        (10)
#define RK_SHARP_V3_PBF_DIAM                    3
#define RK_SHARP_V3_RF_DIAM                     3
#define RK_SHARP_V3_BF_DIAM                     3
#define RK_SHARP_V3_AVG_DIAM                    3

#define RK_SHARP_V3_HF_DIAM                     3
#define RK_SHARP_V3_MF_DIAM                     5
#define RK_SHARP_V3_LF_DIAM                     9
#define RK_SHARP_V3_WGT_FILTER_DIAM             5

#define rk_sharp_V3_sharp_ratio_fix_bits        2
#define rk_sharp_V3_gaus_ratio_fix_bits         7
#define rk_sharp_V3_bf_ratio_fix_bits           7
#define rk_sharp_V3_pbfCoeff_fix_bits           7
#define rk_sharp_V3_rfCoeff_fix_bits            7
#define rk_sharp_V3_hbfCoeff_fix_bits           7

#define INTERP_V3(x0, x1, ratio)            ((ratio) * ((x1) - (x0)) + x0)
#define CLIP(a, min_v, max_v)               (((a) < (min_v)) ? (min_v) : (((a) > (max_v)) ? (max_v) : (a)))


#if 1
typedef enum Asharp3_result_e {
    ASHARP3_RET_SUCCESS             = 0,   // this has to be 0, if clauses rely on it
    ASHARP3_RET_FAILURE             = 1,   // process failure
    ASHARP3_RET_INVALID_PARM        = 2,   // invalid parameter
    ASHARP3_RET_WRONG_CONFIG        = 3,   // feature not supported
    ASHARP3_RET_BUSY                = 4,   // there's already something going on...
    ASHARP3_RET_CANCELED            = 5,   // operation canceled
    ASHARP3_RET_OUTOFMEM            = 6,   // out of memory
    ASHARP3_RET_OUTOFRANGE          = 7,   // parameter/value out of range
    ASHARP3_RET_NULL_POINTER        = 8,   // the/one/all parameter(s) is a(are) NULL pointer(s)
    ASHARP3_RET_DIVISION_BY_ZERO    = 9,   // a divisor equals ZERO
    ASHARP3_RET_NO_INPUTIMAGE       = 10   // no input image
} Asharp3_result_t;

typedef enum Asharp3_State_e {
    ASHARP3_STATE_INVALID           = 0,                   /**< initialization value */
    ASHARP3_STATE_INITIALIZED       = 1,                   /**< instance is created, but not initialized */
    ASHARP3_STATE_STOPPED           = 2,                   /**< instance is confiured (ready to start) or stopped */
    ASHARP3_STATE_RUNNING           = 3,                   /**< instance is running (processes frames) */
    ASHARP3_STATE_LOCKED            = 4,                   /**< instance is locked (for taking snapshots) */
    ASHARP3_STATE_MAX                                      /**< max */
} Asharp3_State_t;

typedef enum Asharp3_OPMode_e {
    ASHARP3_OP_MODE_INVALID           = 0,                   /**< initialization value */
    ASHARP3_OP_MODE_AUTO              = 1,                   /**< instance is created, but not initialized */
    ASHARP3_OP_MODE_MANUAL            = 2,                   /**< instance is confiured (ready to start) or stopped */
    ASHARP3_OP_MODE_MAX                                      /**< max */
} Asharp3_OPMode_t;

typedef enum Asharp3_ParamMode_e {
    ASHARP3_PARAM_MODE_INVALID           = 0,
    ASHARP3_PARAM_MODE_NORMAL          = 1,                   /**< initialization value */
    ASHARP3_PARAM_MODE_HDR              = 2,                   /**< instance is created, but not initialized */
    ASHARP3_PARAM_MODE_GRAY            = 3,                   /**< instance is confiured (ready to start) or stopped */
    ASHARP3_PARAM_MODE_MAX                                      /**< max */
} Asharp3_ParamMode_t;

#endif

typedef struct Asharp3_ExpInfo_s {
    int hdr_mode;
    float arTime[3];
    float arAGain[3];
    float arDGain[3];
    int   arIso[3];
    int   snr_mode;
    int rawWidth;
    int rawHeight;
} Asharp3_ExpInfo_t;


typedef struct RK_SHARP_Params_V3_s
{
    int enable;

    int iso[RK_SHARP_V3_MAX_ISO_NUM];
    short luma_point        [RK_SHARP_V3_LUMA_POINT_NUM];
    short luma_sigma        [RK_SHARP_V3_MAX_ISO_NUM][RK_SHARP_V3_LUMA_POINT_NUM];
    float pbf_gain          [RK_SHARP_V3_MAX_ISO_NUM];
    float pbf_add           [RK_SHARP_V3_MAX_ISO_NUM];
    float pbf_ratio         [RK_SHARP_V3_MAX_ISO_NUM];
    float gaus_ratio        [RK_SHARP_V3_MAX_ISO_NUM];
    float sharp_ratio       [RK_SHARP_V3_MAX_ISO_NUM];
    short lum_clip_h        [RK_SHARP_V3_MAX_ISO_NUM][RK_SHARP_V3_LUMA_POINT_NUM];
    float bf_gain           [RK_SHARP_V3_MAX_ISO_NUM];
    float bf_add            [RK_SHARP_V3_MAX_ISO_NUM];
    float bf_ratio          [RK_SHARP_V3_MAX_ISO_NUM];
    short ehf_th            [RK_SHARP_V3_MAX_ISO_NUM][RK_SHARP_V3_LUMA_POINT_NUM];

    float kernel_pre_bila_filter[RK_SHARP_V3_MAX_ISO_NUM][RK_SHARP_V3_PBF_DIAM * RK_SHARP_V3_PBF_DIAM];
    float kernel_range_filter   [RK_SHARP_V3_MAX_ISO_NUM][RK_SHARP_V3_RF_DIAM * RK_SHARP_V3_RF_DIAM];
    float kernel_bila_filter    [RK_SHARP_V3_MAX_ISO_NUM][RK_SHARP_V3_BF_DIAM * RK_SHARP_V3_BF_DIAM];

#if 1
    //////////////////////////////////////////////////////////////////////////
    // test params
    float sharp_ratio_h     [RK_SHARP_V3_MAX_ISO_NUM];
    float sharp_ratio_m     [RK_SHARP_V3_MAX_ISO_NUM];
    float sharp_ratio_l     [RK_SHARP_V3_MAX_ISO_NUM];
    short clip_hf           [RK_SHARP_V3_MAX_ISO_NUM][RK_SHARP_V3_LUMA_POINT_NUM];
    short clip_mf           [RK_SHARP_V3_MAX_ISO_NUM][RK_SHARP_V3_LUMA_POINT_NUM];
    short clip_lf           [RK_SHARP_V3_MAX_ISO_NUM][RK_SHARP_V3_LUMA_POINT_NUM];
    short local_wgt         [RK_SHARP_V3_MAX_ISO_NUM][RK_SHARP_V3_LUMA_POINT_NUM];

    short kernel_hf_filter  [RK_SHARP_V3_MAX_ISO_NUM][RK_SHARP_V3_HF_DIAM * RK_SHARP_V3_HF_DIAM];
    short kernel_mf_filter  [RK_SHARP_V3_MAX_ISO_NUM][RK_SHARP_V3_MF_DIAM * RK_SHARP_V3_MF_DIAM];
    short kernel_lf_filter  [RK_SHARP_V3_MAX_ISO_NUM][RK_SHARP_V3_LF_DIAM * RK_SHARP_V3_LF_DIAM];
#endif

} RK_SHARP_Params_V3_t;

typedef struct RK_SHARP_Params_V3_Select_s
{
    int enable;

    short luma_point    [RK_SHARP_V3_LUMA_POINT_NUM];
    short luma_sigma    [RK_SHARP_V3_LUMA_POINT_NUM];
    float pbf_gain      ;
    float pbf_add       ;
    float pbf_ratio     ;
    float gaus_ratio    ;
    float sharp_ratio   ;
    short lum_clip_h    [RK_SHARP_V3_LUMA_POINT_NUM];
    float bf_gain       ;
    float bf_add        ;
    float bf_ratio      ;
    short ehf_th        [RK_SHARP_V3_LUMA_POINT_NUM];

    float kernel_pre_bila_filter[RK_SHARP_V3_PBF_DIAM * RK_SHARP_V3_PBF_DIAM];
    float kernel_range_filter   [RK_SHARP_V3_RF_DIAM * RK_SHARP_V3_RF_DIAM];
    float kernel_bila_filter    [RK_SHARP_V3_BF_DIAM * RK_SHARP_V3_BF_DIAM];

#if 1
    //////////////////////////////////////////////////////////////////////////
    // test params
    float sharp_ratio_h     ;
    float sharp_ratio_m     ;
    float sharp_ratio_l     ;
    short clip_hf           [RK_SHARP_V3_LUMA_POINT_NUM];
    short clip_mf           [RK_SHARP_V3_LUMA_POINT_NUM];
    short clip_lf           [RK_SHARP_V3_LUMA_POINT_NUM];
    short local_wgt         [RK_SHARP_V3_LUMA_POINT_NUM];

    short kernel_hf_filter  [RK_SHARP_V3_HF_DIAM * RK_SHARP_V3_HF_DIAM];
    short kernel_mf_filter  [RK_SHARP_V3_MF_DIAM * RK_SHARP_V3_MF_DIAM];
    short kernel_lf_filter  [RK_SHARP_V3_LF_DIAM * RK_SHARP_V3_LF_DIAM];
#endif

} RK_SHARP_Params_V3_Select_t;


typedef struct Asharp_Manual_Attr_V3_s
{
    RK_SHARP_Params_V3_Select_t stSelect;

} Asharp_Manual_Attr_V3_t;

typedef struct Asharp_Auto_Attr_V3_s
{
    //all ISO params and select param

    RK_SHARP_Params_V3_t stParams;
    RK_SHARP_Params_V3_Select_t stSelect;

} Asharp_Auto_Attr_V3_t;

typedef struct Asharp_ProcResult_V3_s {
    int sharpEn;

    //for sw simultaion
    //RK_SHARP_Params_V3_Select_t stSelect;

    //for hw register
    RK_SHARP_Fix_V3_t* stFix;
} Asharp_ProcResult_V3_t;


typedef struct Asharp_Config_V3_s {
    Asharp3_State_t eState;
    Asharp3_OPMode_t eMode;
    int rawHeight;
    int rawWidth;
} Asharp_Config_V3_t;


typedef struct rk_aiq_sharp_attrib_v3_s {
    Asharp3_OPMode_t eMode;
    Asharp_Auto_Attr_V3_t stAuto;
    Asharp_Manual_Attr_V3_t stManual;
} rk_aiq_sharp_attrib_v3_t;


typedef struct rk_aiq_sharp_IQPara_V3_s {
    struct list_head* listHead;
} rk_aiq_sharp_IQPara_V3_t;


//calibdb


RKAIQ_END_DECLARE

#endif

