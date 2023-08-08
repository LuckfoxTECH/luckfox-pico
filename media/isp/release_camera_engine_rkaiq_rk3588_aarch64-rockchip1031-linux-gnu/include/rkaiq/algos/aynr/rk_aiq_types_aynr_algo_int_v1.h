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

#ifndef _RK_AIQ_TYPE_AYNR_ALGO_INT_V1_H_
#define _RK_AIQ_TYPE_AYNR_ALGO_INT_V1_H_

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "rk_aiq_comm.h"
#include "RkAiqCalibDbTypes.h"
#include "aynr/rk_aiq_types_aynr_algo_v1.h"
#include "ynr_head_v1.h"

RKAIQ_BEGIN_DECLARE

#define ISO_CURVE_POINT_BIT             4
#define ISO_CURVE_POINT_NUM             ((1 << ISO_CURVE_POINT_BIT)+1)
#define RKAIQ_YNR_SIGMA_BITS            10
#define WAVELET_LEVEL_NUM               4
#define GAIN_ENABLE_FLG                 1
#define GAIN_YNR_ENABLE_FLG             (GAIN_ENABLE_FLG & 1)
#define GAIN_YNR_FIX_BITS_DECI          4
#define GAIN_YUVNR_SOURCE_FLG           0

#define YNR_MAX_ISO_STEP_V1 (CALIBDB_NR_SHARP_MAX_ISO_LEVEL)

typedef enum Aynr_Result_V1_e {
    AYNR_RET_V1_SUCCESS             = 0,   // this has to be 0, if clauses rely on it
    AYNR_RET_V1_FAILURE             = 1,   // process failure
    AYNR_RET_V1_INVALID_PARM        = 2,   // invalid parameter
    AYNR_RET_V1_WRONG_CONFIG        = 3,   // feature not supported
    AYNR_RET_V1_BUSY                = 4,   // there's already something going on...
    AYNR_RET_V1_CANCELED            = 5,   // operation canceled
    AYNR_RET_V1_OUTOFMEM            = 6,   // out of memory
    AYNR_RET_V1_OUTOFRANGE          = 7,   // parameter/value out of range
    AYNR_RET_V1_NULL_POINTER        = 8,   // the/one/all parameter(s) is a(are) NULL pointer(s)
    AYNR_RET_V1_DIVISION_BY_ZERO    = 9,   // a divisor equals ZERO
    AYNR_RET_V1_NO_INPUTIMAGE       = 10   // no input image
} Aynr_Result_V1_t;

typedef enum Aynr_State_V1_e {
    AYNR_STATE_V1_INVALID           = 0,                   /**< initialization value */
    AYNR_STATE_V1_INITIALIZED       = 1,                   /**< instance is created, but not initialized */
    AYNR_STATE_V1_STOPPED           = 2,                   /**< instance is confiured (ready to start) or stopped */
    AYNR_STATE_V1_RUNNING           = 3,                   /**< instance is running (processes frames) */
    AYNR_STATE_V1_LOCKED            = 4,                   /**< instance is locked (for taking snapshots) */
    AYNR_STATE_V1_MAX                                      /**< max */
} Aynr_State_V1_t;

typedef enum Aynr_OPMode_V1_e {
    AYNR_OP_MODE_V1_INVALID           = 0,                   /**< initialization value */
    AYNR_OP_MODE_V1_AUTO              = 1,                   /**< instance is created, but not initialized */
    AYNR_OP_MODE_V1_MANUAL            = 2,                   /**< instance is confiured (ready to start) or stopped */
    AYNR_OP_MODE_V1_MAX                                      /**< max */
} Aynr_OPMode_V1_t;

typedef enum Aynr_ParamMode_V1_e {
    AYNR_PARAM_MODE_V1_INVALID           = 0,
    AYNR_PARAM_MODE_V1_NORMAL          = 1,                   /**< initialization value */
    AYNR_PARAM_MODE_V1_HDR              = 2,                   /**< instance is created, but not initialized */
    AYNR_PARAM_MODE_V1_GRAY            = 3,                   /**< instance is confiured (ready to start) or stopped */
    AYNR_PARAM_MODE_V1_MAX                                      /**< max */
} Aynr_ParamMode_V1_t;

typedef struct Aynr_ExpInfo_V1_s {
    int hdr_mode;
    float arTime[3];
    float arAGain[3];
    float arDGain[3];
    int arIso[3];
    int arDcgMode[3];
    int cur_snr_mode;
    int snr_mode;

    float preTime[3];
    float preAGain[3];
    float preDGain[3];
    int preIso[3];
    int preDcgMode[3];
    int pre_snr_mode;

    int mfnr_mode_3to1;
} Aynr_ExpInfo_V1_t;


typedef struct RK_YNR_Params_V1_Select_s
{
#ifndef RK_SIMULATOR_HW
    float iso;
#endif

    float ciISO[12];
    float noiseSigma[ISO_CURVE_POINT_NUM];
    short lumaPoints[ISO_CURVE_POINT_NUM];

    float loFreqNoiseCi[WAVELET_LEVEL_NUM];
    float loFreqDenoiseWeight[WAVELET_LEVEL_NUM];
    float loFreqBfScale[WAVELET_LEVEL_NUM];
    float loFreqLumaNrCurvePoint[6];
    float loFreqLumaNrCurveRatio[6];

    float loFreqDenoiseStrength[2];
    float loFreqDirectionStrength;

    float hiFreqDenoiseWeight[WAVELET_LEVEL_NUM];
    float hiFreqBfScale[WAVELET_LEVEL_NUM];
    float hiFreqEdgeSoftness[WAVELET_LEVEL_NUM];
    float hiFreqLumaNrCurvePoint[6];
    float hiFreqLumaNrCurveRatio[6];
    float hiFreqDenoiseStrength;
    float hiFreqSoftThresholdScale[WAVELET_LEVEL_NUM];

    short radialNoiseCtrPoint[7];
    float radialNoiseCtrRatio[7];

    float lscGainRatioAdjust[WAVELET_LEVEL_NUM];

    float detailThre[6];
    float detailThreRatioLevel[3][6];
    float detailMinAdjDnW;

    float detailThreLevel4[6];
    float detailThreRatioLevel4[6];

    short waveLetCoeffDeltaHi;
    short waveLetCoeffDeltaLo;
    short hiValueThre;
    short loValueThre;
    int   ynr_level4_max_gain;

    char ynr_ver_char[64];
} RK_YNR_Params_V1_Select_t;



typedef struct RK_YNR_Params_V1_s
{
    RK_YNR_Params_V1_Select_t aYnrParamsISO[YNR_MAX_ISO_STEP_V1];

    short rawBit;
    short isoValue;

    char ynr_ver_char[64];

} RK_YNR_Params_V1_t;


typedef struct Aynr_Manual_Attr_V1_s
{
    int ynrEn;
    RK_YNR_Params_V1_Select_t stSelect;

} Aynr_Manual_Attr_V1_t;

typedef struct Aynr_Auto_Attr_V1_s
{
    //all ISO params and select param
    int ynrEn;

    RK_YNR_Params_V1_t stParams;
    RK_YNR_Params_V1_Select_t stSelect;

} Aynr_Auto_Attr_V1_t;

typedef struct Aynr_ProcResult_V1_s {
    int ynrEn;

    //for sw simultaion
    RK_YNR_Params_V1_Select_t stSelect;

    //for hw register
    RK_YNR_Fix_V1_t stFix;

} Aynr_ProcResult_V1_t;


typedef struct Aynr_Config_V1_s {
    Aynr_State_V1_t eState;
    Aynr_OPMode_V1_t eMode;
} Aynr_Config_V1_t;


typedef struct rk_aiq_ynr_attrib_v1_s {
    Aynr_OPMode_V1_t eMode;
    Aynr_Auto_Attr_V1_t stAuto;
    Aynr_Manual_Attr_V1_t stManual;
} rk_aiq_ynr_attrib_v1_t;


typedef struct rk_aiq_ynr_IQPara_V1_s {
    CalibDb_YNR_t stYnrPara;
} rk_aiq_ynr_IQPara_V1_t;


typedef struct rk_aiq_ynr_JsonPara_V1_s {
    CalibDbV2_YnrV1_t ynr_v1;
} rk_aiq_ynr_JsonPara_V1_t;



RKAIQ_END_DECLARE

#endif

