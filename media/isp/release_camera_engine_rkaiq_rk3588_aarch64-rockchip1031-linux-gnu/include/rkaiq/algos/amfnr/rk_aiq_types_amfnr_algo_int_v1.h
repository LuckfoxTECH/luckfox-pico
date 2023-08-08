/*
 * rk_aiq_types_amfnr_algo_int_v1.h
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

#ifndef _RK_AIQ_TYPE_AMFNR_ALGO_INT_V1_H_
#define _RK_AIQ_TYPE_AMFNR_ALGO_INT_V1_H_

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "rk_aiq_comm.h"
#include "RkAiqCalibDbTypes.h"
#include "amfnr/rk_aiq_types_amfnr_algo_v1.h"
#include "mfnr_head_v1.h"

RKAIQ_BEGIN_DECLARE

/************mfnr***************/
#define WAVELET_LEVEL_NUM               4
#define MAX_INTEPORATATION_LUMAPOINT    17
#define POLT_ORDER                  4
#define MAX_BACK_REF_NUM            1
#define LIMIT_RANGE_LEFT_BIT8       16
#define LIMIT_RANGE_RIGHT_BIT8      236
#define Y_CALIBRATION_BITS          12
#define Y_SIGMA_TABLE_BITS          8
#define Y_SIGMA_TABLE_BITS_DEHAZE   8
#define MFNR_POLYORDER              POLT_ORDER
#define MFNR_MAX_GAUS_SIZE          10
#define MFNR_DIR_NUM                2
#define DIR_LO                      0
#define DIR_HI                      1
#define MFNR_MAX_LVL_UV             3
#define MFNR_MAX_LVL                4
#define MFNR_MAX_LVL_DEPTH          ((MFNR_MAX_LVL_UV + 1 > MFNR_MAX_LVL) ? (MFNR_MAX_LVL_UV + 1) : MFNR_MAX_LVL)
#define MFNR_DELTA_SQR_EXP_POINT    17
#define LUMANRCURVE_STEP            6
#define MFNR_TILE_W                 16
#define MFNR_TILE_H                 128
#define MFNR_F_INTE_SIGMA           6



#define MFNR_MAX_ISO_STEP_V1 (CALIBDB_NR_SHARP_MAX_ISO_LEVEL)

typedef enum Amfnr_Result_V1_e {
    AMFNR_RET_V1_SUCCESS             = 0,   // this has to be 0, if clauses rely on it
    AMFNR_RET_V1_FAILURE             = 1,   // process failure
    AMFNR_RET_V1_INVALID_PARM        = 2,   // invalid parameter
    AMFNR_RET_V1_WRONG_CONFIG        = 3,   // feature not supported
    AMFNR_RET_V1_BUSY                = 4,   // there's already something going on...
    AMFNR_RET_V1_CANCELED            = 5,   // operation canceled
    AMFNR_RET_V1_OUTOFMEM            = 6,   // out of memory
    AMFNR_RET_V1_OUTOFRANGE          = 7,   // parameter/value out of range
    AMFNR_RET_V1_NULL_POINTER        = 8,   // the/one/all parameter(s) is a(are) NULL pointer(s)
    AMFNR_RET_V1_DIVISION_BY_ZERO    = 9,   // a divisor equals ZERO
    AMFNR_RET_V1_NO_INPUTIMAGE       = 10   // no input image
} Amfnr_Result_V1_t;

typedef enum Amfnr_State_V1_e {
    AMFNR_STATE_V1_INVALID           = 0,                   /**< initialization value */
    AMFNR_STATE_V1_INITIALIZED       = 1,                   /**< instance is created, but not initialized */
    AMFNR_STATE_V1_STOPPED           = 2,                   /**< instance is confiured (ready to start) or stopped */
    AMFNR_STATE_V1_RUNNING           = 3,                   /**< instance is running (processes frames) */
    AMFNR_STATE_V1_LOCKED            = 4,                   /**< instance is locked (for taking snapshots) */
    AMFNR_STATE_V1_MAX                                      /**< max */
} Amfnr_State_V1_t;

typedef enum Amfnr_OPMode_V1_e {
    AMFNR_OP_MODE_V1_INVALID           = 0,                   /**< initialization value */
    AMFNR_OP_MODE_V1_AUTO              = 1,                   /**< instance is created, but not initialized */
    AMFNR_OP_MODE_V1_MANUAL            = 2,                   /**< instance is confiured (ready to start) or stopped */
    AMFNR_OP_MODE_V1_MAX                                      /**< max */
} Amfnr_OPMode_V1_t;

typedef enum Amfnr_ParamMode_V1_e {
    AMFNR_PARAM_MODE_V1_INVALID           = 0,
    AMFNR_PARAM_MODE_V1_NORMAL          = 1,                   /**< initialization value */
    AMFNR_PARAM_MODE_V1_HDR              = 2,                   /**< instance is created, but not initialized */
    AMFNR_PARAM_MODE_V1_GRAY            = 3,                   /**< instance is confiured (ready to start) or stopped */
    AMFNR_PARAM_MODE_V1_MAX                                      /**< max */
} Amfnr_ParamMode_V1_t;

typedef struct Amfnr_ExpInfo_V1_s {
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
} Amfnr_ExpInfo_V1_t;


typedef struct RK_MFNR_Params_V1_Select_s
{

    int back_ref_num;

    int weight_limit_y[MFNR_MAX_LVL_DEPTH];
    int weight_limit_uv[MFNR_MAX_LVL_UV];
    double ratio_frq[4];
    double luma_w_in_chroma[MFNR_MAX_LVL_UV];
    double awb_uv_ratio[2];

    double ci                   [MFNR_DIR_NUM][MFNR_MAX_LVL_DEPTH];
    double dnweight             [MFNR_DIR_NUM][MFNR_MAX_LVL_DEPTH];
    double scale                    [MFNR_DIR_NUM][MFNR_MAX_LVL_DEPTH];
    double lumanrpoint          [MFNR_DIR_NUM][LUMANRCURVE_STEP];
    double lumanrcurve          [MFNR_DIR_NUM][LUMANRCURVE_STEP];
    double dnstr                    [MFNR_DIR_NUM];
    double gfdelta              [MFNR_DIR_NUM][MFNR_MAX_LVL_DEPTH][MFNR_MAX_GAUS_SIZE];

    double ci_uv                    [MFNR_DIR_NUM][MFNR_MAX_LVL_UV];
    double dnweight_uv          [MFNR_DIR_NUM][MFNR_MAX_LVL_UV];
    double scale_uv             [MFNR_DIR_NUM][MFNR_MAX_LVL_UV];
    double lumanrpoint_uv       [MFNR_DIR_NUM][LUMANRCURVE_STEP];
    double lumanrcurve_uv       [MFNR_DIR_NUM][LUMANRCURVE_STEP];
    double dnstr_uv             [MFNR_DIR_NUM];
    double gfdelta_uv           [MFNR_DIR_NUM][MFNR_MAX_LVL_UV][MFNR_MAX_GAUS_SIZE];

    double gfsigma              [MFNR_MAX_LVL][MFNR_MAX_GAUS_SIZE];

    double noise_sigma          [1 << Y_CALIBRATION_BITS];
    double noise_sigma_sample   [MAX_INTEPORATATION_LUMAPOINT];
    double noise_sigma_dehaze   [MAX_INTEPORATATION_LUMAPOINT];
    unsigned short  fix_x_pos           [MAX_INTEPORATATION_LUMAPOINT];
    unsigned short  fix_x_pos_dehaze    [MAX_INTEPORATATION_LUMAPOINT];

    float mfnr_sigma_scale;

} RK_MFNR_Params_V1_Select_t;



typedef struct RK_MFNR_Params_V1_s
{
#ifndef RK_SIMULATOR_HW
    float iso[MFNR_MAX_ISO_STEP_V1];
#endif

    int back_ref_num;

    int weight_limit_y  [MFNR_MAX_ISO_STEP_V1][MFNR_MAX_LVL_DEPTH];
    int weight_limit_uv [MFNR_MAX_ISO_STEP_V1][MFNR_MAX_LVL_UV];
    double ratio_frq        [MFNR_MAX_ISO_STEP_V1][4];
    double luma_w_in_chroma[MFNR_MAX_ISO_STEP_V1][MFNR_MAX_LVL_UV];
    double awb_uv_ratio [4][2];
    double curve        [MFNR_MAX_ISO_STEP_V1][MFNR_POLYORDER + 1];
    double curve_x0     [MFNR_MAX_ISO_STEP_V1];

    double ci           [MFNR_MAX_ISO_STEP_V1][MFNR_DIR_NUM][MFNR_MAX_LVL_DEPTH];
    double dnweight     [MFNR_MAX_ISO_STEP_V1][MFNR_DIR_NUM][MFNR_MAX_LVL_DEPTH];
    double scale            [MFNR_MAX_ISO_STEP_V1][MFNR_DIR_NUM][MFNR_MAX_LVL_DEPTH];
    double lumanrpoint  [MFNR_MAX_ISO_STEP_V1][MFNR_DIR_NUM][LUMANRCURVE_STEP];
    double lumanrcurve  [MFNR_MAX_ISO_STEP_V1][MFNR_DIR_NUM][LUMANRCURVE_STEP];
    double dnstr            [MFNR_MAX_ISO_STEP_V1][MFNR_DIR_NUM];
    double gfdelta      [MFNR_MAX_ISO_STEP_V1][MFNR_DIR_NUM][MFNR_MAX_LVL_DEPTH][MFNR_MAX_GAUS_SIZE];

    double ci_uv            [MFNR_MAX_ISO_STEP_V1][MFNR_DIR_NUM][MFNR_MAX_LVL_UV];
    double dnweight_uv  [MFNR_MAX_ISO_STEP_V1][MFNR_DIR_NUM][MFNR_MAX_LVL_UV];
    double scale_uv     [MFNR_MAX_ISO_STEP_V1][MFNR_DIR_NUM][MFNR_MAX_LVL_UV];
    double lumanrpoint_uv[MFNR_MAX_ISO_STEP_V1][MFNR_DIR_NUM][LUMANRCURVE_STEP];
    double lumanrcurve_uv[MFNR_MAX_ISO_STEP_V1][MFNR_DIR_NUM][LUMANRCURVE_STEP];
    double dnstr_uv     [MFNR_MAX_ISO_STEP_V1][MFNR_DIR_NUM];
    double gfdelta_uv   [MFNR_MAX_ISO_STEP_V1][MFNR_DIR_NUM][MFNR_MAX_LVL_UV][MFNR_MAX_GAUS_SIZE];

    double gfsigma      [MFNR_MAX_ISO_STEP_V1][MFNR_MAX_LVL][MFNR_MAX_GAUS_SIZE];

    double noise_sigma  [MFNR_MAX_ISO_STEP_V1][1 << Y_CALIBRATION_BITS];
    float mfnr_sigma_scale[MFNR_MAX_ISO_STEP_V1];
    int motion_detection_enable;
    char mfnr_ver_char;

} RK_MFNR_Params_V1_t;


typedef struct RK_MFNR_Dynamic_V1_s {
    int enable;
    float lowth_iso;
    float lowth_time;
    float highth_iso;
    float highth_time;
    int mfnr_enable_state;
} RK_MFNR_Dynamic_V1_t;


typedef struct Amfnr_Manual_Attr_V1_s
{
    int mfnrEn;
    RK_MFNR_Params_V1_Select_t stSelect;

    RK_MFNR_Dynamic_V1_t stMfnr_dynamic;

} Amfnr_Manual_Attr_V1_t;

typedef struct Amfnr_Auto_Attr_V1_s
{
    //all ISO params and select param
    int mfnrEn;

    RK_MFNR_Params_V1_t stParams;
    RK_MFNR_Params_V1_Select_t stSelect;

    RK_MFNR_Dynamic_V1_t stMfnr_dynamic;
    CalibDb_MFNR_Motion_t  stMotion;

} Amfnr_Auto_Attr_V1_t;

typedef struct Amfnr_ProcResult_V1_s {
    int mfnrEn;

    //for sw simultaion
    RK_MFNR_Params_V1_Select_t stSelect;

    //for hw register
    RK_MFNR_Fix_V1_t stFix;

    CalibDb_MFNR_Motion_t  stMotion;

} Amfnr_ProcResult_V1_t;


typedef struct Amfnr_Config_V1_s {
    Amfnr_State_V1_t eState;
    Amfnr_OPMode_V1_t eMode;
} Amfnr_Config_V1_t;


typedef struct rk_aiq_mfnr_attrib_v1_s {
    Amfnr_OPMode_V1_t eMode;
    Amfnr_Auto_Attr_V1_t stAuto;
    Amfnr_Manual_Attr_V1_t stManual;
} rk_aiq_mfnr_attrib_v1_t;


typedef struct rk_aiq_mfnr_IQPara_V1_s {
    CalibDb_MFNR_t stMfnrPara;
} rk_aiq_mfnr_IQPara_V1_t;


typedef struct rk_aiq_mfnr_JsonPara_V1_s {
    CalibDbV2_MFNR_t mfnr_v1;
} rk_aiq_mfnr_JsonPara_V1_t;



RKAIQ_END_DECLARE

#endif

