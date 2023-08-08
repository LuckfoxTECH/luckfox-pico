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

#ifndef _RK_AIQ_TYPE_AGAIN_ALGO_INT_V1_H_
#define _RK_AIQ_TYPE_AGAIN_ALGO_INT_V1_H_

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "rk_aiq_comm.h"
#include "RkAiqCalibDbTypes.h"
#include "again/rk_aiq_types_again_algo.h"

//RKAIQ_BEGIN_DECLARE

#define GAIN_SIGMA_BITS_ACT_V1     (10)

#define GAIN_HDR_MERGE_IN_FIX_BITS_DECI_V1             (6)
#define GAIN_HDR_MERGE_IN2_FIX_BITS_INTE_V1            (12)
#define GAIN_HDR_MERGE_IN0_FIX_BITS_INTE_V1            (8)

#define GAIN_MAX_INTEPORATATION_LUMAPOINT (17)

typedef enum Again_result_e {
    AGAIN_RET_SUCCESS             = 0,   // this has to be 0, if clauses rely on it
    AGAIN_RET_FAILURE             = 1,   // process failure
    AGAIN_RET_INVALID_PARM        = 2,   // invalid parameter
    AGAIN_RET_WRONG_CONFIG        = 3,   // feature not supported
    AGAIN_RET_BUSY                = 4,   // there's already something going on...
    AGAIN_RET_CANCELED            = 5,   // operation canceled
    AGAIN_RET_OUTOFMEM            = 6,   // out of memory
    AGAIN_RET_OUTOFRANGE          = 7,   // parameter/value out of range
    AGAIN_RET_NULL_POINTER        = 8,   // the/one/all parameter(s) is a(are) NULL pointer(s)
    AGAIN_RET_DIVISION_BY_ZERO    = 9,   // a divisor equals ZERO
    AGAIN_RET_NO_INPUTIMAGE       = 10   // no input image
} Again_result_t;

typedef enum Again_State_e {
    AGAIN_STATE_INVALID           = 0,                   /**< initialization value */
    AGAIN_STATE_INITIALIZED       = 1,                   /**< instance is created, but not initialized */
    AGAIN_STATE_STOPPED           = 2,                   /**< instance is confiured (ready to start) or stopped */
    AGAIN_STATE_RUNNING           = 3,                   /**< instance is running (processes frames) */
    AGAIN_STATE_LOCKED            = 4,                   /**< instance is locked (for taking snapshots) */
    AGAIN_STATE_MAX                                      /**< max */
} Again_State_t;

typedef enum Again_OPMode_e {
    AGAIN_OP_MODE_INVALID           = 0,                   /**< initialization value */
    AGAIN_OP_MODE_AUTO              = 1,                   /**< instance is created, but not initialized */
    AGAIN_OP_MODE_MANUAL            = 2,                   /**< instance is confiured (ready to start) or stopped */
    AGAIN_OP_MODE_MAX                                      /**< max */
} Again_OPMode_t;

typedef enum Again_ParamMode_e {
	AGAIN_PARAM_MODE_INVALID           = 0, 
    AGAIN_PARAM_MODE_NORMAL          = 1,                   /**< initialization value */
    AGAIN_PARAM_MODE_HDR              = 2,                   /**< instance is created, but not initialized */
    AGAIN_PARAM_MODE_GRAY            = 3,                   /**< instance is confiured (ready to start) or stopped */
    AGAIN_PARAM_MODE_MAX                                      /**< max */
} Again_ParamMode_t;

typedef struct Again_ExpInfo_s {
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
} Again_ExpInfo_t;

typedef struct RK_GAIN_Params_V1_s 
{

}RK_GAIN_Params_V1_t;

typedef struct RK_GAIN_Params_V1_Select_s 
{	
	unsigned short fix_x_pos_dehaze[GAIN_MAX_INTEPORATATION_LUMAPOINT];
	double noise_sigma_dehaze[GAIN_MAX_INTEPORATATION_LUMAPOINT];

}RK_GAIN_Params_V1_Select_t;


typedef struct Again_Manual_Attr_V1_s
{
    int gainEn;
	RK_GAIN_Params_V1_Select_t stSelect;
	
} Again_Manual_Attr_V1_t;

typedef struct Again_Auto_Attr_V1_s
{
    //all ISO params and select param
    int gainEn;

	RK_GAIN_Params_V1_t stParams;
    RK_GAIN_Params_V1_Select_t stSelect;

} Again_Auto_Attr_V1_t;

typedef struct Again_ProcResult_V1_s {
    int gainEn;

    //for sw simultaion
    RK_GAIN_Params_V1_Select_t stSelect;

    //for hw register
    RK_GAIN_Fix_V1_t stFix;
	
} Again_ProcResult_V1_t;


typedef struct Again_Config_V1_s {
    Again_State_t eState;
    Again_OPMode_t eMode;
} Again_Config_V1_t;


typedef struct rk_aiq_gain_attrib_v1_s {
    Again_OPMode_t eMode;
    Again_Auto_Attr_V1_t stAuto;
    Again_Manual_Attr_V1_t stManual;
} rk_aiq_gain_attrib_v1_t;



//calibdb


//RKAIQ_END_DECLARE

#endif

