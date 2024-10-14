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

#ifndef _RK_AIQ_TYPE_ABAYERTNRV23_ALGO_INT_H_
#define _RK_AIQ_TYPE_ABAYERTNRV23_ALGO_INT_H_

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "rk_aiq_comm.h"
#include "RkAiqCalibDbTypes.h"
#include "abayertnrV23/rk_aiq_types_abayertnr_algo_v23.h"
#include "bayertnr_head_v23.h"
#include "bayertnr_uapi_head_v23.h"


//RKAIQ_BEGIN_DECLARE

#define ABAYERTNRV23_RECALCULATE_DELTA_ISO (10)
#define RK_BAYERNR_V23_MAX_ISO_NUM (CALIBDB_MAX_ISO_LEVEL)

typedef enum Abayertnr_result_V23_e {
    ABAYERTNRV23_RET_SUCCESS             = 0,   // this has to be 0, if clauses rely on it
    ABAYERTNRV23_RET_FAILURE             = 1,   // process failure
    ABAYERTNRV23_RET_INVALID_PARM        = 2,   // invalid parameter
    ABAYERTNRV23_RET_WRONG_CONFIG        = 3,   // feature not supported
    ABAYERTNRV23_RET_BUSY                = 4,   // there's already something going on...
    ABAYERTNRV23_RET_CANCELED            = 5,   // operation canceled
    ABAYERTNRV23_RET_OUTOFMEM            = 6,   // out of memory
    ABAYERTNRV23_RET_OUTOFRANGE          = 7,   // parameter/value out of range
    ABAYERTNRV23_RET_NULL_POINTER        = 8,   // the/one/all parameter(s) is a(are) NULL pointer(s)
    ABAYERTNRV23_RET_DIVISION_BY_ZERO    = 9,   // a divisor equals ZERO
    ABAYERTNRV23_RET_NO_INPUTIMAGE       = 10   // no input image
} Abayertnr_result_V23_t;

typedef enum Abayertnr_State_V23_e {
    ABAYERTNRV23_STATE_INVALID           = 0,                   /**< initialization value */
    ABAYERTNRV23_STATE_INITIALIZED       = 1,                   /**< instance is created, but not initialized */
    ABAYERTNRV23_STATE_STOPPED           = 2,                   /**< instance is confiured (ready to start) or stopped */
    ABAYERTNRV23_STATE_RUNNING           = 3,                   /**< instance is running (processes frames) */
    ABAYERTNRV23_STATE_LOCKED            = 4,                   /**< instance is locked (for taking snapshots) */
    ABAYERTNRV23_STATE_MAX                                      /**< max */
} Abayertnr_State_V23_t;

typedef enum Abayertnr_OPMode_V23_e {
    ABAYERTNRV23_OP_MODE_INVALID           = 0,                   /**< initialization value */
    ABAYERTNRV23_OP_MODE_AUTO              = 1,                   /**< instance is created, but not initialized */
    ABAYERTNRV23_OP_MODE_MANUAL            = 2,                   /**< instance is confiured (ready to start) or stopped */
    ABAYERTNRV23_OP_MODE_REG_MANUAL        = 3,
    ABAYERTNRV23_OP_MODE_MAX                                      /**< max */
} Abayertnr_OPMode_V23_t;

typedef enum Abayertnr_ParamMode_V23_e {
    ABAYERTNRV23_PARAM_MODE_INVALID           = 0,
    ABAYERTNRV23_PARAM_MODE_NORMAL          = 1,                   /**< initialization value */
    ABAYERTNRV23_PARAM_MODE_HDR              = 2,                   /**< instance is created, but not initialized */
    ABAYERTNRV23_PARAM_MODE_GRAY            = 3,                   /**< instance is confiured (ready to start) or stopped */
    ABAYERTNRV23_PARAM_MODE_MAX                                      /**< max */
} Abayertnr_ParamMode_V23_t;



#if 0
typedef struct RK_Bayertnr_Params_V23_Select_s
{
    bool enable;

    //calib
    int lumapoint[16];
    int sigma[16];
    int lumapoint2[16];
    int lo_sigma[16];
    int hi_sigma[16];

    //tuning
    uint8_t thumbds_w;
    uint8_t thumbds_h;

    bool lo_enable;
    bool hi_enable;
    bool lo_med_en;
    bool lo_gsbay_en;
    bool lo_gslum_en;
    bool hi_med_en;
    bool hi_gslum_en;

    bool trans_en;

    bool wgt_use_mode;
    bool wgt_mge_mode;
    bool hi_guass;
    bool kl_guass;

    bool global_pk_en;
    int global_pksq;

    float lo_filter_strength;
    float hi_filter_strength;
    float soft_threshold_ratio;

    float lo_clipwgt;
    float hi_wgt_comp;
    int   hidif_th;

    float lo_filter_rat0;
    int   lo_filter_thed0;

    int hi_filter_abs_ctrl;
    int hi_filter_filt_bay;
    int hi_filter_filt_avg;
    int hi_filter_filt_mode;

    float hi_filter_rat0;
    int   hi_filter_thed0;
    float hi_filter_rat1;
    int   hi_filter_thed1;


    int   guass_guide_coeff0;
    int   guass_guide_coeff1;
    int   guass_guide_coeff2;
    int   guass_guide_coeff3;

} RK_Bayertnr_Params_V23_Select_t;
#endif

typedef struct RK_Bayertnr_Params_V23_s
{
    bool enable;
    float iso[RK_BAYERNR_V23_MAX_ISO_NUM];
    RK_Bayertnr_Params_V23_Select_t bayertnrParamISO[RK_BAYERNR_V23_MAX_ISO_NUM];
} RK_Bayertnr_Params_V23_t;

typedef struct RK_Bayertnr_Params_V23L_s {
    bool enable;
    float iso[RK_BAYERNR_V23_MAX_ISO_NUM];
    RK_Bayertnr_Param_V23L_Select_t bayertnrParamISO[RK_BAYERNR_V23_MAX_ISO_NUM];
} RK_Bayertnr_Params_V23L_t;

typedef struct Abayertnr_Manual_Attr_V23_s
{
    RK_Bayertnr_Params_V23_Select_t st3DSelect;

    RK_Bayertnr_Fix_V23_t st3DFix;
} Abayertnr_Manual_Attr_V23_t;

typedef struct Abayertnr_Manual_Attr_V23L_s {
    RK_Bayertnr_Param_V23L_Select_t st3DSelect;

    RK_Bayertnr_Fix_V23_t st3DFix;
} Abayertnr_Manual_Attr_V23L_t;

typedef struct Abayertnr_Auto_Attr_V23_s
{
    RK_Bayertnr_Params_V23_t st3DParams;
    RK_Bayertnr_Params_V23_Select_t st3DSelect;

} Abayertnr_Auto_Attr_V23_t;

typedef struct Abayertnr_Auto_Attr_V23L_s {
    RK_Bayertnr_Params_V23L_t st3DParams;
    RK_Bayertnr_Param_V23L_Select_t st3DSelect;
} Abayertnr_Auto_Attr_V23L_t;

typedef struct Abayertnr_ProcResult_V23_s {
    int bayernr3DEn;

    //for sw simultaion
#if (RKAIQ_HAVE_BAYERTNR_V23)
    //RK_Bayertnr_Params_V23_Select_t st3DSelect;
#else
    //RK_Bayertnr_Param_V23L_Select_t st3DSelect;
#endif

    //for hw register
    RK_Bayertnr_Fix_V23_t* st3DFix;

} Abayertnr_ProcResult_V23_t;


typedef struct Abayertnr_Config_V23_s {
    Abayertnr_State_V23_t eState;
    Abayertnr_OPMode_V23_t eMode;
} Abayertnr_Config_V23_t;


typedef struct rk_aiq_bayertnr_attrib_v23_s {
    rk_aiq_uapi_sync_t sync;
    Abayertnr_OPMode_V23_t eMode;
    Abayertnr_Auto_Attr_V23_t stAuto;
    Abayertnr_Manual_Attr_V23_t stManual;
} rk_aiq_bayertnr_attrib_v23_t;

typedef struct rk_aiq_bayertnr_attrib_v23L_s {
    rk_aiq_uapi_sync_t sync;
    Abayertnr_OPMode_V23_t eMode;
    Abayertnr_Auto_Attr_V23L_t stAuto;
    Abayertnr_Manual_Attr_V23L_t stManual;
} rk_aiq_bayertnr_attrib_v23L_t;

typedef struct rk_aiq_bayertnr_strength_v23_s {
    rk_aiq_uapi_sync_t sync;
    float percent;
    bool strength_enable;
} rk_aiq_bayertnr_strength_v23_t;



//RKAIQ_END_DECLARE

#endif

