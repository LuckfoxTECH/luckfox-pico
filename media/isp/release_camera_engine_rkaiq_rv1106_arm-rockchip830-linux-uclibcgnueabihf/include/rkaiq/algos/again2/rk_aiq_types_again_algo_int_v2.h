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

#ifndef _RK_AIQ_TYPE_AGAIN_ALGO_INT_V2_H_
#define _RK_AIQ_TYPE_AGAIN_ALGO_INT_V2_H_

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "rk_aiq_comm.h"
#include "RkAiqCalibDbTypes.h"
#include "again2/rk_aiq_types_again_algo_v2.h"
#include "gain_head_v2.h"
#include "gain_uapi_head_v2.h"

// enable write local gain write to ddr
#define RK_GAIN_V2_ENABLE_GAIN2DDR      0

//RKAIQ_BEGIN_DECLARE
#define RK_GAIN_V2_MAX_ISO_NUM  CALIBDB_MAX_ISO_LEVEL


#define AGAINV2_RECALCULATE_DELTA_ISO       (10)

#define     GAIN_BAYERTNR_IN_BITS_INTE          0


#define     GAIN_HDR_MERGE_IN_FIX_BITS_DECI             6
#define     GAIN_HDR_MERGE_IN0_FIX_BITS_INTE            8
#define     GAIN_HDR_MERGE_IN1_FIX_BITS_INTE            8
#define     GAIN_HDR_MERGE_IN2_FIX_BITS_INTE            12


typedef enum Again_result_V2_e {
    AGAINV2_RET_SUCCESS             = 0,   // this has to be 0, if clauses rely on it
    AGAINV2_RET_FAILURE             = 1,   // process failure
    AGAINV2_RET_INVALID_PARM        = 2,   // invalid parameter
    AGAINV2_RET_WRONG_CONFIG        = 3,   // feature not supported
    AGAINV2_RET_BUSY                = 4,   // there's already something going on...
    AGAINV2_RET_CANCELED            = 5,   // operation canceled
    AGAINV2_RET_OUTOFMEM            = 6,   // out of memory
    AGAINV2_RET_OUTOFRANGE          = 7,   // parameter/value out of range
    AGAINV2_RET_NULL_POINTER        = 8,   // the/one/all parameter(s) is a(are) NULL pointer(s)
    AGAINV2_RET_DIVISION_BY_ZERO    = 9,   // a divisor equals ZERO
    AGAINV2_RET_NO_INPUTIMAGE       = 10   // no input image
} Again_result_V2_t;

typedef enum Again_State_V2_e {
    AGAINV2_STATE_INVALID           = 0,                   /**< initialization value */
    AGAINV2_STATE_INITIALIZED       = 1,                   /**< instance is created, but not initialized */
    AGAINV2_STATE_STOPPED           = 2,                   /**< instance is confiured (ready to start) or stopped */
    AGAINV2_STATE_RUNNING           = 3,                   /**< instance is running (processes frames) */
    AGAINV2_STATE_LOCKED            = 4,                   /**< instance is locked (for taking snapshots) */
    AGAINV2_STATE_MAX                                      /**< max */
} Again_State_V2_t;

typedef enum Again_OPMode_V2_e {
    AGAINV2_OP_MODE_INVALID           = 0,                   /**< initialization value */
    AGAINV2_OP_MODE_AUTO              = 1,                   /**< instance is created, but not initialized */
    AGAINV2_OP_MODE_MANUAL            = 2,                   /**< instance is confiured (ready to start) or stopped */
    AGAINV2_OP_MODE_MAX                                      /**< max */
} Again_OPMode_V2_t;

typedef enum Again_ParamMode_V2_e {
    AGAINV2_PARAM_MODE_INVALID           = 0,
    AGAINV2_PARAM_MODE_NORMAL          = 1,                   /**< initialization value */
    AGAINV2_PARAM_MODE_HDR              = 2,                   /**< instance is created, but not initialized */
    AGAINV2_PARAM_MODE_GRAY            = 3,                   /**< instance is confiured (ready to start) or stopped */
    AGAINV2_PARAM_MODE_MAX                                      /**< max */
} Again_ParamMode_V2_t;



#if 0
typedef struct RK_GAIN_Select_V2_s
{
    bool hdrgain_ctrl_enable;
    float hdr_gain_scale_s;
    float hdr_gain_scale_m;

} RK_GAIN_Select_V2_t;
#endif

typedef struct RK_GAIN_Params_V2_s
{   bool hdrgain_ctrl_enable;
    int iso[RK_GAIN_V2_MAX_ISO_NUM];
    RK_GAIN_Select_V2_t iso_params[RK_GAIN_V2_MAX_ISO_NUM];
} RK_GAIN_Params_V2_t;


typedef struct Again_Manual_Attr_V2_s
{
    RK_GAIN_Select_V2_t stSelect;
    RK_GAIN_Fix_V2_t stFix;
} Again_Manual_Attr_V2_t;

typedef struct AgainV2_Auto_Attr_V2_s
{
    //all ISO params and select param

    RK_GAIN_Params_V2_t stParams;
    RK_GAIN_Select_V2_t stSelect;

} Again_Auto_Attr_V2_t;

typedef struct AgainV2_ProcResult_V2_s {
    //for sw simultaion
    //RK_GAIN_Select_V2_t stSelect;

    //for hw register
    RK_GAIN_Fix_V2_t* stFix;

} Again_ProcResult_V2_t;


typedef struct Again_Config_V2_s {
    Again_State_V2_t eState;
    Again_OPMode_V2_t eMode;
} Again_Config_V2_t;


typedef struct rk_aiq_gain_attrib_v2_s {
    rk_aiq_uapi_sync_t sync;
    Again_OPMode_V2_t eMode;
    Again_Auto_Attr_V2_t stAuto;
    Again_Manual_Attr_V2_t stManual;
} rk_aiq_gain_attrib_v2_t;

typedef struct rk_aiq_uapiV2_again_wrtIn_attr_s {
    rk_aiq_uapi_sync_t sync;
    bool enable;
    int mode;
    char path[100];
    int call_cnt;
} rk_aiq_uapiV2_again_wrtIn_attr_t;

//RKAIQ_END_DECLARE

#endif

