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

#ifndef _RK_AIQ_TYPE_AUVNR_ALGO_INT_V1_H_
#define _RK_AIQ_TYPE_AUVNR_ALGO_INT_V1_H_

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "rk_aiq_comm.h"
#include "RkAiqCalibDbTypes.h"
#include "auvnr/rk_aiq_types_auvnr_algo_v1.h"
#include "uvnr_head_v1.h"


//RKAIQ_BEGIN_DECLARE

#define UVNR_MAX_ISO_STEP (CALIBDB_NR_SHARP_MAX_ISO_LEVEL)

typedef enum Auvnr_result_e {
    AUVNR_RET_SUCCESS             = 0,   // this has to be 0, if clauses rely on it
    AUVNR_RET_FAILURE             = 1,   // process failure
    AUVNR_RET_INVALID_PARM        = 2,   // invalid parameter
    AUVNR_RET_WRONG_CONFIG        = 3,   // feature not supported
    AUVNR_RET_BUSY                = 4,   // there's already something going on...
    AUVNR_RET_CANCELED            = 5,   // operation canceled
    AUVNR_RET_OUTOFMEM            = 6,   // out of memory
    AUVNR_RET_OUTOFRANGE          = 7,   // parameter/value out of range
    AUVNR_RET_NULL_POINTER        = 8,   // the/one/all parameter(s) is a(are) NULL pointer(s)
    AUVNR_RET_DIVISION_BY_ZERO    = 9,   // a divisor equals ZERO
    AUVNR_RET_NO_INPUTIMAGE       = 10   // no input image
} Auvnr_result_t;

typedef enum Auvnr_State_e {
    AUVNR_STATE_INVALID           = 0,                   /**< initialization value */
    AUVNR_STATE_INITIALIZED       = 1,                   /**< instance is created, but not initialized */
    AUVNR_STATE_STOPPED           = 2,                   /**< instance is confiured (ready to start) or stopped */
    AUVNR_STATE_RUNNING           = 3,                   /**< instance is running (processes frames) */
    AUVNR_STATE_LOCKED            = 4,                   /**< instance is locked (for taking snapshots) */
    AUVNR_STATE_MAX                                      /**< max */
} Auvnr_State_t;

typedef enum Auvnr_OPMode_e {
    AUVNR_OP_MODE_INVALID           = 0,                   /**< initialization value */
    AUVNR_OP_MODE_AUTO              = 1,                   /**< instance is created, but not initialized */
    AUVNR_OP_MODE_MANUAL            = 2,                   /**< instance is confiured (ready to start) or stopped */
    AUVNR_OP_MODE_MAX                                      /**< max */
} Auvnr_OPMode_t;

typedef enum Auvnr_ParamMode_e {
	AUVNR_PARAM_MODE_INVALID           = 0, 
    AUVNR_PARAM_MODE_NORMAL          = 1,                   /**< initialization value */
    AUVNR_PARAM_MODE_HDR              = 2,                   /**< instance is created, but not initialized */
    AUVNR_PARAM_MODE_GRAY            = 3,                   /**< instance is confiured (ready to start) or stopped */
    AUVNR_PARAM_MODE_MAX                                      /**< max */
} Auvnr_ParamMode_t;

typedef struct Auvnr_ExpInfo_s {
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
} Auvnr_ExpInfo_t;

typedef struct RK_UVNR_Params_V1_s 
{
	#ifndef RK_SIMULATOR_HW
    float iso[UVNR_MAX_ISO_STEP];
#endif

    char    rkuvnrISO[256];

    //step0:
    float   ratio[UVNR_MAX_ISO_STEP];
    float   offset[UVNR_MAX_ISO_STEP];

    //step1
    int     wStep1[UVNR_MAX_ISO_STEP];
    int     hStep1[UVNR_MAX_ISO_STEP];
    int     meanSize1[UVNR_MAX_ISO_STEP];

    int     nonMed1[4];
    int     medSize1[UVNR_MAX_ISO_STEP];
    float   medRatio1[UVNR_MAX_ISO_STEP];
    int     isMedIIR1[UVNR_MAX_ISO_STEP];

    int     nonBf1[4];
    int     bfSize1[UVNR_MAX_ISO_STEP];
    float   sigmaR1[UVNR_MAX_ISO_STEP];
    float   sigmaD1[UVNR_MAX_ISO_STEP];
    float   uvgain1[UVNR_MAX_ISO_STEP];
    float   bfRatio1[UVNR_MAX_ISO_STEP];
    int     isRowIIR1[UVNR_MAX_ISO_STEP];
    int     isYcopy1[UVNR_MAX_ISO_STEP];

    //step2
    int     block2_ext[4];

    int     wStep2[UVNR_MAX_ISO_STEP];
    int     hStep2[UVNR_MAX_ISO_STEP];
    int     meanSize2[UVNR_MAX_ISO_STEP];

    int     nonMed2[4];
    int     medSize2[UVNR_MAX_ISO_STEP];
    float   medRatio2[UVNR_MAX_ISO_STEP];
    int     isMedIIR2[UVNR_MAX_ISO_STEP];

    int     nonBf2[4];
    int     bfSize2[UVNR_MAX_ISO_STEP];
    float   sigmaR2[UVNR_MAX_ISO_STEP];
    float   sigmaD2[UVNR_MAX_ISO_STEP];
    float   uvgain2[UVNR_MAX_ISO_STEP];
    float   bfRatio2[UVNR_MAX_ISO_STEP];
    int     isRowIIR2[UVNR_MAX_ISO_STEP];
    int     isYcopy2[UVNR_MAX_ISO_STEP];

    //step3
    int     nonBf3[4];
    int     bfSize3[UVNR_MAX_ISO_STEP];
    float   sigmaR3[UVNR_MAX_ISO_STEP];
    float   sigmaD3[UVNR_MAX_ISO_STEP];
    float   uvgain3[UVNR_MAX_ISO_STEP];
    float   bfRatio3[UVNR_MAX_ISO_STEP];
    int     isRowIIR3[UVNR_MAX_ISO_STEP];
    int     isYcopy3[UVNR_MAX_ISO_STEP];

    //kernels
    float   kernel_3x3_table[3];
    float   kernel_5x5_talbe[5];
    float   kernel_9x9_table[8];
    int     kernel_9x9_num;

    //curves
    int     sigmaAdj_x[9];
    float   sigamAdj_y[9];

    int     threAdj_x[9];
    int     threAjd_y[9];
}RK_UVNR_Params_V1_t;

typedef struct RK_UVNR_Params_V1_Select_s 
{
	char    select_iso[256];

    //step0
    float   ratio;
    float   offset;

    //step1
    int     wStep1;
    int     hStep1;
    int     meanSize1;

    int     nonMed1[4];
    int     medSize1;
    float   medRatio1;
    int     isMedIIR1;

    int     nonBf1[4];
    int     bfSize1;
    float   sigmaR1;
    float   sigmaD1;
    float   uvgain1;
    float   bfRatio1;
    int     isRowIIR1;
    int     isYcopy1;

    //step2
    int     block2_ext[4];
    int     wStep2;
    int     hStep2;
    int     meanSize2;

    int     nonMed2[4];
    int     medSize2;
    float   medRatio2;
    int     isMedIIR2;

    int     nonBf2[4];
    int     bfSize2;
    float   sigmaR2;
    float   sigmaD2;
    float   uvgain2;
    float   bfRatio2;
    int     isRowIIR2;
    int     isYcopy2;

    //step3
    int     nonBf3[4];
    int     bfSize3;
    float   sigmaR3;
    float   sigmaD3;
    float   uvgain3;
    float   bfRatio3;
    int     isRowIIR3;
    int     isYcopy3;

    //kernels
    float   kernel_3x3_table[3];
    float   kernel_5x5_table[5];
    float   kernel_9x9_table[8];
    int     kernel_9x9_num;

    //curves
    int     sigmaAdj_x[9];
    float   sigmaAdj_y[9];

    int     threAdj_x[9];
    int     threAdj_y[9];
}RK_UVNR_Params_V1_Select_t;


typedef struct Auvnr_Manual_Attr_V1_s
{
    int uvnrEn;
	RK_UVNR_Params_V1_Select_t stSelect;
	
} Auvnr_Manual_Attr_V1_t;

typedef struct Auvnr_Auto_Attr_V1_s
{
    //all ISO params and select param
    int uvnrEn;

	RK_UVNR_Params_V1_t stParams;
    RK_UVNR_Params_V1_Select_t stSelect;

} Auvnr_Auto_Attr_V1_t;

typedef struct Auvnr_ProcResult_V1_s {
    int uvnrEn;

    //for sw simultaion
    //RK_UVNR_Params_V1_Select_t stSelect;

    //for hw register
    RK_UVNR_Fix_V1_t* stFix;
	
} Auvnr_ProcResult_V1_t;


typedef struct Auvnr_Config_V1_s {
    Auvnr_State_t eState;
    Auvnr_OPMode_t eMode;
	int rawHeight;
	int rawWidth;
} Auvnr_Config_V1_t;


typedef struct rk_aiq_uvnr_attrib_v1_s {
    Auvnr_OPMode_t eMode;
    Auvnr_Auto_Attr_V1_t stAuto;
    Auvnr_Manual_Attr_V1_t stManual;
} rk_aiq_uvnr_attrib_v1_t;


typedef struct rk_aiq_uvnr_IQPara_v1_s {
	CalibDb_UVNR_t stUvnrPara;
} rk_aiq_uvnr_IQPara_v1_t;

typedef struct rk_aiq_uvnr_JsonPara_v1_s {
	CalibDbV2_UVNR_t uvnr_v1;
} rk_aiq_uvnr_JsonPara_v1_t;


//calibdb


//RKAIQ_END_DECLARE

#endif

