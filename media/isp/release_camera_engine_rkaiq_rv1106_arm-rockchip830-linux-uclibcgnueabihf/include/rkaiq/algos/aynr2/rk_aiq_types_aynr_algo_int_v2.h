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

#ifndef _RK_AIQ_TYPE_AYNR_ALGO_INT_V2_H_
#define _RK_AIQ_TYPE_AYNR_ALGO_INT_V2_H_

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "rk_aiq_comm.h"
#include "RkAiqCalibDbTypes.h"
#include "aynr2/rk_aiq_types_aynr_algo_v2.h"
#include "ynr_head_v2.h"


//RKAIQ_BEGIN_DECLARE
#define AYNRV2_RECALCULATE_DELTA_ISO       (10)
#define YNR_V2_ISO_CURVE_POINT_BIT          4
#define YNR_V2_ISO_CURVE_POINT_NUM          ((1 << YNR_V2_ISO_CURVE_POINT_BIT)+1)
#define YNR_V2_SIGMA_BITS                  10
#define YNR_V2_NOISE_SIGMA_FIX_BIT              3
#define LOG2(x)                             (log((double)x)                 / log((double)2))


typedef enum Aynr_result_e {
    AYNR_RET_SUCCESS             = 0,   // this has to be 0, if clauses rely on it
    AYNR_RET_FAILURE             = 1,   // process failure
    AYNR_RET_INVALID_PARM        = 2,   // invalid parameter
    AYNR_RET_WRONG_CONFIG        = 3,   // feature not supported
    AYNR_RET_BUSY                = 4,   // there's already something going on...
    AYNR_RET_CANCELED            = 5,   // operation canceled
    AYNR_RET_OUTOFMEM            = 6,   // out of memory
    AYNR_RET_OUTOFRANGE          = 7,   // parameter/value out of range
    AYNR_RET_NULL_POINTER        = 8,   // the/one/all parameter(s) is a(are) NULL pointer(s)
    AYNR_RET_DIVISION_BY_ZERO    = 9,   // a divisor equals ZERO
    AYNR_RET_NO_INPUTIMAGE       = 10   // no input image
} Aynr_result_t;

typedef enum Aynr_State_e {
    AYNR_STATE_INVALID           = 0,                   /**< initialization value */
    AYNR_STATE_INITIALIZED       = 1,                   /**< instance is created, but not initialized */
    AYNR_STATE_STOPPED           = 2,                   /**< instance is confiured (ready to start) or stopped */
    AYNR_STATE_RUNNING           = 3,                   /**< instance is running (processes frames) */
    AYNR_STATE_LOCKED            = 4,                   /**< instance is locked (for taking snapshots) */
    AYNR_STATE_MAX                                      /**< max */
} Aynr_State_t;

typedef enum Aynr_OPMode_e {
    AYNR_OP_MODE_INVALID           = 0,                   /**< initialization value */
    AYNR_OP_MODE_AUTO              = 1,                   /**< instance is created, but not initialized */
    AYNR_OP_MODE_MANUAL            = 2,                   /**< instance is confiured (ready to start) or stopped */
    AYNR_OP_MODE_MAX                                      /**< max */
} Aynr_OPMode_t;

typedef enum Aynr_ParamMode_e {
    AYNR_PARAM_MODE_INVALID           = 0,
    AYNR_PARAM_MODE_NORMAL          = 1,                   /**< initialization value */
    AYNR_PARAM_MODE_HDR              = 2,                   /**< instance is created, but not initialized */
    AYNR_PARAM_MODE_GRAY            = 3,                   /**< instance is confiured (ready to start) or stopped */
    AYNR_PARAM_MODE_MAX                                      /**< max */
} Aynr_ParamMode_t;

typedef struct Aynr_ExpInfo_s {
    int hdr_mode;
    float arTime[3];
    float arAGain[3];
    float arDGain[3];
    int   arIso[3];
    int   snr_mode;
    int rawWidth;
    int rawHeight;
} Aynr_ExpInfo_t;

typedef struct RK_YNR_Params_V2_Select_s
{
    int enable;
    float ciISO_V2[2];
    float noiseSigma_V2[YNR_V2_ISO_CURVE_POINT_NUM];
    short lumaPoints_V2[YNR_V2_ISO_CURVE_POINT_NUM];

    // low frequency
    int ynr_big_resolution_mode;
    int ynr_global_gain_V2;
    float ynr_rnr_strength_V2[17];
    int ynr_bft3x3_bypass_V2;
    int ynr_lbft5x5_bypass_V2;
    int ynr_lgft3x3_bypass_V2;
    int ynr_flt1x1_bypass_V2;
    int ynr_sft5x5_bypass_V2;
    float ynr_low_bf_V2[2];
    float ynr_low_thred_adj_V2;
    float ynr_low_peak_supress_V2;
    float ynr_low_edge_adj_thresh_V2;
    float ynr_low_center_weight_V2;
    float ynr_low_dist_adj_V2;
    float ynr_low_weight_V2;
    float ynr_low_filt1_strength_V2;
    float ynr_low_filt2_strength_V2;
    float ynr_low_bi_weight_V2;

    // high frequency
    float ynr_base_filter_weight1_V2;
    float ynr_base_filter_weight2_V2;
    float ynr_base_filter_weight3_V2;
    float ynr_high_thred_adj_V2;
    float ynr_high_weight_V2;
    float ynr_direction_weight_V2[8];
    float ynr_hi_min_adj_V2;
    float ynr_hi_edge_thed_V2;
} RK_YNR_Params_V2_Select_t;



typedef struct RK_YNR_Params_V2_s
{
    int enable;
    char version[64];
    float iso[RK_YNR_V2_MAX_ISO_NUM];
    RK_YNR_Params_V2_Select_t arYnrParamsISO[RK_YNR_V2_MAX_ISO_NUM];

} RK_YNR_Params_V2_t;


typedef struct Aynr_Manual_Attr_V2_s
{
    int ynrEn;
    RK_YNR_Params_V2_Select_t stSelect;

} Aynr_Manual_Attr_V2_t;

typedef struct Aynr_Auto_Attr_V2_s
{
    //all ISO params and select param
    int ynrEn;

    RK_YNR_Params_V2_t stParams;
    RK_YNR_Params_V2_Select_t stSelect;

} Aynr_Auto_Attr_V2_t;

typedef struct Aynr_ProcResult_V2_s {
    int ynrEn;

    //for sw simultaion
    RK_YNR_Params_V2_Select_t stSelect;

    //for hw register
    RK_YNR_Fix_V2_t stFix;

    bool isNeedUpdate;

} Aynr_ProcResult_V2_t;


typedef struct Aynr_Config_V2_s {
    Aynr_State_t eState;
    Aynr_OPMode_t eMode;
    int rawHeight;
    int rawWidth;
} Aynr_Config_V2_t;


typedef struct rk_aiq_ynr_attrib_v2_s {
    Aynr_OPMode_t eMode;
    Aynr_Auto_Attr_V2_t stAuto;
    Aynr_Manual_Attr_V2_t stManual;
} rk_aiq_ynr_attrib_v2_t;


typedef struct rk_aiq_ynr_IQPara_V2_s {
    struct list_head* listHead;
} rk_aiq_ynr_IQPara_V2_t;


//calibdb


//RKAIQ_END_DECLARE

#endif

