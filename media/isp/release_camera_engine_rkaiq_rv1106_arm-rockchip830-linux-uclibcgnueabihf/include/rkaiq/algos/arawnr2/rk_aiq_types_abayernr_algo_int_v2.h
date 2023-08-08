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

#ifndef _RK_AIQ_TYPE_ABAYERNR_ALGO_INT_V2_H_
#define _RK_AIQ_TYPE_ABAYERNR_ALGO_INT_V2_H_

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "rk_aiq_comm.h"
#include "RkAiqCalibDbTypes.h"
#include "arawnr2/rk_aiq_types_abayernr_algo_v2.h"
#include "bayernr_head_v2.h"

//RKAIQ_BEGIN_DECLARE

#define ABAYERNRV2_DELTA_ISO (10)

typedef enum Abayernr_result_e {
    ABAYERNR_RET_SUCCESS             = 0,   // this has to be 0, if clauses rely on it
    ABAYERNR_RET_FAILURE             = 1,   // process failure
    ABAYERNR_RET_INVALID_PARM        = 2,   // invalid parameter
    ABAYERNR_RET_WRONG_CONFIG        = 3,   // feature not supported
    ABAYERNR_RET_BUSY                = 4,   // there's already something going on...
    ABAYERNR_RET_CANCELED            = 5,   // operation canceled
    ABAYERNR_RET_OUTOFMEM            = 6,   // out of memory
    ABAYERNR_RET_OUTOFRANGE          = 7,   // parameter/value out of range
    ABAYERNR_RET_NULL_POINTER        = 8,   // the/one/all parameter(s) is a(are) NULL pointer(s)
    ABAYERNR_RET_DIVISION_BY_ZERO    = 9,   // a divisor equals ZERO
    ABAYERNR_RET_NO_INPUTIMAGE       = 10   // no input image
} Abayernr_result_t;

typedef enum Abayernr_State_e {
    ABAYERNR_STATE_INVALID           = 0,                   /**< initialization value */
    ABAYERNR_STATE_INITIALIZED       = 1,                   /**< instance is created, but not initialized */
    ABAYERNR_STATE_STOPPED           = 2,                   /**< instance is confiured (ready to start) or stopped */
    ABAYERNR_STATE_RUNNING           = 3,                   /**< instance is running (processes frames) */
    ABAYERNR_STATE_LOCKED            = 4,                   /**< instance is locked (for taking snapshots) */
    ABAYERNR_STATE_MAX                                      /**< max */
} Abayernr_State_t;

typedef enum Abayernr_OPMode_e {
    ABAYERNR_OP_MODE_INVALID           = 0,                   /**< initialization value */
    ABAYERNR_OP_MODE_AUTO              = 1,                   /**< instance is created, but not initialized */
    ABAYERNR_OP_MODE_MANUAL            = 2,                   /**< instance is confiured (ready to start) or stopped */
    ABAYERNR_OP_MODE_MAX                                      /**< max */
} Abayernr_OPMode_t;

typedef enum Abayernr_ParamMode_e {
    ABAYERNR_PARAM_MODE_INVALID           = 0,
    ABAYERNR_PARAM_MODE_NORMAL          = 1,                   /**< initialization value */
    ABAYERNR_PARAM_MODE_HDR              = 2,                   /**< instance is created, but not initialized */
    ABAYERNR_PARAM_MODE_GRAY            = 3,                   /**< instance is confiured (ready to start) or stopped */
    ABAYERNR_PARAM_MODE_MAX                                      /**< max */
} Abayernr_ParamMode_t;

typedef struct Abayernr_ExpInfo_s {
    int hdr_mode;
    float arTime[3];
    float arAGain[3];
    float arDGain[3];
    int   arIso[3];
    int   snr_mode;
} Abayernr_ExpInfo_t;



typedef struct RK_Bayernr_2D_Params_V2_s
{
    // bayernr version
    int bayernrv2_2dnr_enable;

    // v2 parse
    float iso[RK_BAYERNR_V2_MAX_ISO_NUM];
    float bayernrv2_edge_filter_lumapoint_r[8];
    float bayernrv2_edge_filter_wgt_r[RK_BAYERNR_V2_MAX_ISO_NUM][8];
    float bayernrv2_filter_strength_r[RK_BAYERNR_V2_MAX_ISO_NUM];
    int   bayernrv2_filter_lumapoint_r[16];
    int   bayernrv2_filter_sigma_r[RK_BAYERNR_V2_MAX_ISO_NUM][16];
    float bayernrv2_filter_edgesofts_r[RK_BAYERNR_V2_MAX_ISO_NUM];
    float bayernrv2_filter_soft_threshold_ratio_r[RK_BAYERNR_V2_MAX_ISO_NUM];
    float bayernrv2_filter_out_wgt_r[RK_BAYERNR_V2_MAX_ISO_NUM];
    int   bayernrv2_gauss_guide_r[RK_BAYERNR_V2_MAX_ISO_NUM];

} RK_Bayernr_2D_Params_V2_t;

typedef struct RK_Bayernr_2D_Params_V2_Select_s
{
    // v2 version register // edge filter params
    int bayernrv2_2dnr_enable;

    float bayernrv2_filter_strength;
    float bayernrv2_filter_soft_threshold_ratio;
    float bayernrv2_filter_out_wgt;
    float bayernrv2_filter_edgesofts;

    int bayernrv2_edge_filter_en;
    int bayernrv2_edge_filter_lumapoint[8];
    int bayernrv2_edge_filter_wgt[8];
    int bayernrv2_gauss_guide;
    int bayernrv2_filter_lumapoint[16];
    int bayernrv2_filter_sigma[16];
    int bayernrv2_bil_gauss_weight[16];
    int bayernrv2_gray_mode;
    int bayernrv2_dgains[3];
    int bayernrv2_thld_diff;
    int bayernrv2_pix_diff;

} RK_Bayernr_2D_Params_V2_Select_t;


typedef struct RK_Bayernr_3D_Params_V2_s
{
    int bayernrv2_tnr_enable;

    float iso[RK_BAYERNR_V2_MAX_ISO_NUM];
    float bayernrv2_tnr_filter_strength_r[RK_BAYERNR_V2_MAX_ISO_NUM];
    float bayernrv2_tnr_lo_clipwgt_r[RK_BAYERNR_V2_MAX_ISO_NUM];
    float bayernrv2_tnr_hi_clipwgt_r[RK_BAYERNR_V2_MAX_ISO_NUM];
    float bayernrv2_tnr_softwgt_r[RK_BAYERNR_V2_MAX_ISO_NUM];

    int   bayernrv2_lumapoint_r[16];
    int   bayernrv2_sigma_r[RK_BAYERNR_V2_MAX_ISO_NUM][16];

} RK_Bayernr_3D_Params_V2_t;


typedef struct RK_Bayernr_3D_Params_V2_Select_s
{
    int bayernrv2_tnr_enable;

    float bayernrv2_tnr_filter_strength;
    float bayernrv2_tnr_lo_clipwgt;
    float bayernrv2_tnr_hi_clipwgt;
    float bayernrv2_tnr_softwgt;

    //below not use yet
    int bayernrv2_tnr_sigratio;
    int bayernrv2_tnr_strength;
    int bayernrv2_tnr_global_pk_en;     // 1 use local pk, 0 use global pk
    int bayernrv2_tnr_global_pksq;

    int bayernrv2_tnr_lumapoint[16];
    int bayernrv2_tnr_sigma[16];
} RK_Bayernr_3D_Params_V2_Select_t;


typedef struct Abayernr_Manual_Attr_V2_s
{
    int bayernr2DEn;
    int bayernr3DEn;
    RK_Bayernr_2D_Params_V2_Select_t st2DSelect;
    RK_Bayernr_3D_Params_V2_Select_t st3DSelect;

} Abayernr_Manual_Attr_V2_t;

typedef struct Abayernr_Auto_Attr_V2_s
{
    //all ISO params and select param
    int bayernr2DEn;
    int bayernr3DEn;

    RK_Bayernr_2D_Params_V2_t st2DParams;
    RK_Bayernr_3D_Params_V2_t st3DParams;
    RK_Bayernr_2D_Params_V2_Select_t st2DSelect;
    RK_Bayernr_3D_Params_V2_Select_t st3DSelect;

} Abayernr_Auto_Attr_V2_t;

typedef struct Abayernr_ProcResult_V2_s {
    int bayernr2DEn;
    int bayernr3DEn;

    //for sw simultaion
    RK_Bayernr_2D_Params_V2_Select_t st2DSelect;
    RK_Bayernr_3D_Params_V2_Select_t st3DSelect;

    //for hw register
    RK_Bayernr_2D_Fix_V2_t st2DFix;
    RK_Bayernr_3D_Fix_V2_t st3DFix;

    bool isNeedUpdate;

} Abayernr_ProcResult_V2_t;


typedef struct Abayernr_Config_V2_s {
    Abayernr_State_t eState;
    Abayernr_OPMode_t eMode;
} Abayernr_Config_V2_t;


typedef struct rk_aiq_bayernr_attrib_v2_s {
    Abayernr_OPMode_t eMode;
    Abayernr_Auto_Attr_V2_t stAuto;
    Abayernr_Manual_Attr_V2_t stManual;
} rk_aiq_bayernr_attrib_v2_t;





typedef struct rk_aiq_bayernr_IQPara_V2_s {
    struct list_head listHead_mode;
} rk_aiq_bayernr_IQPara_V2_t;


typedef struct rk_aiq_bayernr_JsonPara_V2_s {
    CalibDbV2_BayerNrV2_t bayernr_v2;
} rk_aiq_bayernr_JsonPara_V2_t;


//RKAIQ_END_DECLARE

#endif

