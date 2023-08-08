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

#ifndef _RK_AIQ_TYPE_ABAYERNR_ALGO_INT_V1_H_
#define _RK_AIQ_TYPE_ABAYERNR_ALGO_INT_V1_H_

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "rk_aiq_comm.h"
#include "RkAiqCalibDbTypes.h"
#include "arawnr/rk_aiq_types_abayernr_algo_v1.h"
#include "bayernr_head_v1.h"

//RKAIQ_BEGIN_DECLARE

#define MAX_ISO_STEP CALIBDB_NR_SHARP_MAX_ISO_LEVEL



typedef enum Abayernr_result_v1_e {
    ABAYERNR_RET_V1_SUCCESS             = 0,   // this has to be 0, if clauses rely on it
    ABAYERNR_RET_V1_FAILURE             = 1,   // process failure
    ABAYERNR_RET_V1_INVALID_PARM        = 2,   // invalid parameter
    ABAYERNR_RET_V1_WRONG_CONFIG        = 3,   // feature not supported
    ABAYERNR_RET_V1_BUSY                = 4,   // there's already something going on...
    ABAYERNR_RET_V1_CANCELED            = 5,   // operation canceled
    ABAYERNR_RET_V1_OUTOFMEM            = 6,   // out of memory
    ABAYERNR_RET_V1_OUTOFRANGE          = 7,   // parameter/value out of range
    ABAYERNR_RET_V1_NULL_POINTER        = 8,   // the/one/all parameter(s) is a(are) NULL pointer(s)
    ABAYERNR_RET_V1_DIVISION_BY_ZERO    = 9,   // a divisor equals ZERO
    ABAYERNR_RET_V1_NO_INPUTIMAGE       = 10   // no input image
} Abayernr_result_v1_t;

typedef enum Abayernr_State_V1_e {
    ABAYERNR_STATE_V1_INVALID           = 0,                   /**< initialization value */
    ABAYERNR_STATE_V1_INITIALIZED       = 1,                   /**< instance is created, but not initialized */
    ABAYERNR_STATE_V1_STOPPED           = 2,                   /**< instance is confiured (ready to start) or stopped */
    ABAYERNR_STATE_V1_RUNNING           = 3,                   /**< instance is running (processes frames) */
    ABAYERNR_STATE_V1_LOCKED            = 4,                   /**< instance is locked (for taking snapshots) */
    ABAYERNR_STATE_V1_MAX                                      /**< max */
} Abayernr_State_V1_t;

typedef enum Abayernr_OPMode_V1_e {
    ABAYERNR_OP_MODE_V1_INVALID           = 0,                   /**< initialization value */
    ABAYERNR_OP_MODE_V1_AUTO              = 1,                   /**< instance is created, but not initialized */
    ABAYERNR_OP_MODE_V1_MANUAL            = 2,                   /**< instance is confiured (ready to start) or stopped */
    ABAYERNR_OP_MODE_V1_MAX                                      /**< max */
} Abayernr_OPMode_V1_t;

typedef enum Abayernr_ParamMode_V1_e {
	ABAYERNR_PARAM_MODE_V1_INVALID           = 0, 
    ABAYERNR_PARAM_MODE_V1_NORMAL          = 1,                   /**< initialization value */
    ABAYERNR_PARAM_MODE_V1_HDR              = 2,                   /**< instance is created, but not initialized */
    ABAYERNR_PARAM_MODE_V1_GRAY            = 3,                   /**< instance is confiured (ready to start) or stopped */
    ABAYERNR_PARAM_MODE_V1_MAX                                      /**< max */
} Abayernr_ParamMode_V1_t;

typedef struct Abayernr_ExpInfo_V1_s {
    int hdr_mode;
    float arTime[3];
    float arAGain[3];
    float arDGain[3];
    int   arIso[3];
    int   arDcgMode[3];
    int   cur_snr_mode;
	int   snr_mode;

	float preTime[3];
	float preAGain[3];
	float preDGain[3];
	int preIso[3];
	int   preDcgMode[3];
    int pre_snr_mode;

	int mfnr_mode_3to1;
} Abayernr_ExpInfo_V1_t;


typedef struct RK_Bayernr_Params_V1_s
{
#ifndef RK_SIMULATOR_HW
    float iso[MAX_ISO_STEP];
#endif

    float a[MAX_ISO_STEP];
    float b[MAX_ISO_STEP];

    int halfpatch;
    int halfblock;
    float filtpar[MAX_ISO_STEP];
    float ctrPit[7];

    float luLevel[8];
    float luRatio[MAX_ISO_STEP][8];

    float w[MAX_ISO_STEP][4];

    int peaknoisesigma;
    int sw_rawnr_gauss_en;

    int rgain_offs;
    int rgain_filp;
    int bgain_offs;
    int bgain_filp;

    char bayernr_ver_char[64];

    float bayernr_edgesoftness;
    float bayernr_gauss_weight0;
    float bayernr_gauss_weight1;

    int sw_bayernr_edge_filter_en;
    float sw_bayernr_edge_filter_lumapoint[8];
    float sw_bayernr_edge_filter_wgt[MAX_ISO_STEP][8];
    float sw_bayernr_filter_strength[MAX_ISO_STEP];
    int   sw_bayernr_filter_lumapoint[16];
    int   sw_bayernr_filter_sigma[MAX_ISO_STEP][16];
    float sw_bayernr_filter_edgesofts[MAX_ISO_STEP];
    float sw_bayernr_filter_soft_threshold_ratio[MAX_ISO_STEP];
    float sw_bayernr_filter_out_wgt[MAX_ISO_STEP];
} RK_Bayernr_Params_V1_t;



typedef struct RK_Bayernr_Params_Select_V1_s
{
    float a[3];
    float b[3];
    float t0[3];
    int halfPatch;
    int halfBlock;
    float filtPar[3];
    float ctrPit[7];

    float luLevel[8];
    float luRatio[8];

    float w[4];
    int peaknoisesigma;
    int sw_rawnr_gauss_en;

    int rgain_offs;
    int rgain_filp;
    int bgain_offs;
    int bgain_filp;

    char bayernr_ver_char[64];

    float bayernr_edgesoftness;
    float bayernr_gauss_weight0;
    float bayernr_gauss_weight1;

    int sw_bayernr_edge_filter_en;
    float sw_bayernr_edge_filter_lumapoint[8];
    float sw_bayernr_edge_filter_wgt[8];
    float sw_bayernr_filter_strength;
    int   sw_bayernr_filter_lumapoint[16];
    int   sw_bayernr_filter_sigma[16];
    float sw_bayernr_filter_edgesofts;
    float sw_bayernr_filter_soft_threshold_ratio;
    float sw_bayernr_filter_out_wgt;

    float sw_dgain[3];

    int gausskparsq;
    int sigmaPar;
    int thld_diff;
    int thld_chanelw;
    int pix_diff;
    int log_bypass;
} RK_Bayernr_Params_Select_V1_t;




typedef struct Abayernr_Manual_Attr_V1_s
{
    int bayernrEn;
	RK_Bayernr_Params_Select_V1_t stSelect;
	
} Abayernr_Manual_Attr_V1_t;

typedef struct Abayernr_Auto_Attr_V1_s
{
    //all ISO params and select param
    int bayernrEn;

	RK_Bayernr_Params_V1_t stParams;
    RK_Bayernr_Params_Select_V1_t stSelect;

} Abayernr_Auto_Attr_V1_t;

typedef struct Abayernr_ProcResult_V1_s {
    int bayernrEn;

    //for sw simultaion
    RK_Bayernr_Params_Select_V1_t stSelect;
    //for hw register
    RK_Bayernr_Fix_V1_t stFix;
	
} Abayernr_ProcResult_V1_t;


typedef struct Abayernr_Config_V1_s {
    Abayernr_State_V1_t eState;
    Abayernr_OPMode_V1_t eMode;
} Abayernr_Config_V1_t;


typedef struct rk_aiq_bayernr_attrib_v1_s {
    Abayernr_OPMode_V1_t eMode;
    Abayernr_Auto_Attr_V1_t stAuto;
    Abayernr_Manual_Attr_V1_t stManual;
} rk_aiq_bayernr_attrib_v1_t;


typedef struct rk_aiq_bayernr_IQPara_V1_s {
	CalibDb_BayerNr_t stBayernrPara;
} rk_aiq_bayernr_IQPara_V1_t;

typedef struct rk_aiq_bayernr_JsonPara_V1_s {	
	CalibDbV2_BayerNrV1_t bayernr_v1;
    
} rk_aiq_bayernr_JsonPara_V1_t;

//RKAIQ_END_DECLARE

#endif

