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

#ifndef _RK_AIQ_TYPE_ANR_ALGO_INT_H_
#define _RK_AIQ_TYPE_ANR_ALGO_INT_H_

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "rk_aiq_comm.h"
#include "RkAiqCalibDbTypes.h"
#include "anr/rk_aiq_types_anr_algo.h"
#include "bayernr_head_v1.h"
#include "mfnr_head_v1.h"
#include "ynr_head_v1.h"
#include "uvnr_head_v1.h"

RKAIQ_BEGIN_DECLARE


#define MAX_ISO_STEP CALIBDB_NR_SHARP_MAX_ISO_LEVEL
#define MFNR_MAX_WEIGHT_LIMIT_UV (255)


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



/************ynr***************/
#define ISO_CURVE_POINT_BIT             4
#define ISO_CURVE_POINT_NUM             ((1 << ISO_CURVE_POINT_BIT)+1)
#define RKAIQ_YNR_SIGMA_BITS            10
#define WAVELET_LEVEL_NUM               4
#define GAIN_ENABLE_FLG                 1
#define GAIN_YNR_ENABLE_FLG             (GAIN_ENABLE_FLG & 1)
#define GAIN_YNR_FIX_BITS_DECI          4
#define GAIN_YUVNR_SOURCE_FLG           0



typedef struct RKAnr_Bayernr_Params_s
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
} RKAnr_Bayernr_Params_t;



typedef struct RKAnr_Bayernr_Params_Select_s
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
} RKAnr_Bayernr_Params_Select_t;


typedef struct RKAnr_Mfnr_Params_s
{
#ifndef RK_SIMULATOR_HW
    float iso[MAX_ISO_STEP];
#endif

    int back_ref_num;

    int weight_limit_y  [MAX_ISO_STEP][MFNR_MAX_LVL_DEPTH];
    int weight_limit_uv [MAX_ISO_STEP][MFNR_MAX_LVL_UV];
    double ratio_frq        [MAX_ISO_STEP][4];
    double luma_w_in_chroma[MAX_ISO_STEP][MFNR_MAX_LVL_UV];
    double awb_uv_ratio [4][2];
    double curve        [MAX_ISO_STEP][MFNR_POLYORDER + 1];
    double curve_x0     [MAX_ISO_STEP];

    double ci           [MAX_ISO_STEP][MFNR_DIR_NUM][MFNR_MAX_LVL_DEPTH];
    double dnweight     [MAX_ISO_STEP][MFNR_DIR_NUM][MFNR_MAX_LVL_DEPTH];
    double scale            [MAX_ISO_STEP][MFNR_DIR_NUM][MFNR_MAX_LVL_DEPTH];
    double lumanrpoint  [MAX_ISO_STEP][MFNR_DIR_NUM][LUMANRCURVE_STEP];
    double lumanrcurve  [MAX_ISO_STEP][MFNR_DIR_NUM][LUMANRCURVE_STEP];
    double dnstr            [MAX_ISO_STEP][MFNR_DIR_NUM];
    double gfdelta      [MAX_ISO_STEP][MFNR_DIR_NUM][MFNR_MAX_LVL_DEPTH][MFNR_MAX_GAUS_SIZE];

    double ci_uv            [MAX_ISO_STEP][MFNR_DIR_NUM][MFNR_MAX_LVL_UV];
    double dnweight_uv  [MAX_ISO_STEP][MFNR_DIR_NUM][MFNR_MAX_LVL_UV];
    double scale_uv     [MAX_ISO_STEP][MFNR_DIR_NUM][MFNR_MAX_LVL_UV];
    double lumanrpoint_uv[MAX_ISO_STEP][MFNR_DIR_NUM][LUMANRCURVE_STEP];
    double lumanrcurve_uv[MAX_ISO_STEP][MFNR_DIR_NUM][LUMANRCURVE_STEP];
    double dnstr_uv     [MAX_ISO_STEP][MFNR_DIR_NUM];
    double gfdelta_uv   [MAX_ISO_STEP][MFNR_DIR_NUM][MFNR_MAX_LVL_UV][MFNR_MAX_GAUS_SIZE];

    double gfsigma      [MAX_ISO_STEP][MFNR_MAX_LVL][MFNR_MAX_GAUS_SIZE];

    double noise_sigma  [MAX_ISO_STEP][1 << Y_CALIBRATION_BITS];
    float mfnr_sigma_scale[MAX_ISO_STEP];
	int motion_detection_enable;
    char mfnr_ver_char;
} RKAnr_Mfnr_Params_t;

typedef struct RKAnr_Mfnr_Params_Select_s
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


} RKAnr_Mfnr_Params_Select_t;

typedef struct RKAnr_Uvnr_Params_s
{
#ifndef RK_SIMULATOR_HW
    float iso[MAX_ISO_STEP];
#endif


    char    rkuvnrISO[256];

    //step0:
    float   ratio[MAX_ISO_STEP];
    float   offset[MAX_ISO_STEP];

    //step1
    int     wStep1[MAX_ISO_STEP];
    int     hStep1[MAX_ISO_STEP];
    int     meanSize1[MAX_ISO_STEP];

    int     nonMed1[4];
    int     medSize1[MAX_ISO_STEP];
    float   medRatio1[MAX_ISO_STEP];
    int     isMedIIR1[MAX_ISO_STEP];

    int     nonBf1[4];
    int     bfSize1[MAX_ISO_STEP];
    float   sigmaR1[MAX_ISO_STEP];
    float   sigmaD1[MAX_ISO_STEP];
    float   uvgain1[MAX_ISO_STEP];
    float   bfRatio1[MAX_ISO_STEP];
    int     isRowIIR1[MAX_ISO_STEP];
    int     isYcopy1[MAX_ISO_STEP];

    //step2
    int     block2_ext[4];

    int     wStep2[MAX_ISO_STEP];
    int     hStep2[MAX_ISO_STEP];
    int     meanSize2[MAX_ISO_STEP];

    int     nonMed2[4];
    int     medSize2[MAX_ISO_STEP];
    float   medRatio2[MAX_ISO_STEP];
    int     isMedIIR2[MAX_ISO_STEP];

    int     nonBf2[4];
    int     bfSize2[MAX_ISO_STEP];
    float   sigmaR2[MAX_ISO_STEP];
    float   sigmaD2[MAX_ISO_STEP];
    float   uvgain2[MAX_ISO_STEP];
    float   bfRatio2[MAX_ISO_STEP];
    int     isRowIIR2[MAX_ISO_STEP];
    int     isYcopy2[MAX_ISO_STEP];

    //step3
    int     nonBf3[4];
    int     bfSize3[MAX_ISO_STEP];
    float   sigmaR3[MAX_ISO_STEP];
    float   sigmaD3[MAX_ISO_STEP];
    float   uvgain3[MAX_ISO_STEP];
    float   bfRatio3[MAX_ISO_STEP];
    int     isRowIIR3[MAX_ISO_STEP];
    int     isYcopy3[MAX_ISO_STEP];

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

} RKAnr_Uvnr_Params_t;

typedef struct RKAnr_Uvnr_Params_Select_s
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

} RKAnr_Uvnr_Params_Select_t;

typedef struct RKAnr_Ynr_Params_Select_s
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
} RKAnr_Ynr_Params_Select_t;

//
typedef struct RKAnr_Ynr_Params_s
{
    RKAnr_Ynr_Params_Select_t aYnrParamsISO[MAX_ISO_STEP];

    short rawBit;
    short isoValue;

    char ynr_ver_char[64];
} RKAnr_Ynr_Params_t;

typedef enum ANRresult_e {
    ANR_RET_SUCCESS             = 0,   // this has to be 0, if clauses rely on it
    ANR_RET_FAILURE             = 1,   // process failure
    ANR_RET_INVALID_PARM        = 2,   // invalid parameter
    ANR_RET_WRONG_CONFIG        = 3,   // feature not supported
    ANR_RET_BUSY                = 4,   // there's already something going on...
    ANR_RET_CANCELED            = 5,   // operation canceled
    ANR_RET_OUTOFMEM            = 6,   // out of memory
    ANR_RET_OUTOFRANGE          = 7,   // parameter/value out of range
    ANR_RET_NULL_POINTER        = 8,   // the/one/all parameter(s) is a(are) NULL pointer(s)
    ANR_RET_DIVISION_BY_ZERO    = 9,   // a divisor equals ZERO
    ANR_RET_NO_INPUTIMAGE       = 10   // no input image
} ANRresult_t;

typedef enum ANRState_e {
    ANR_STATE_INVALID           = 0,                   /**< initialization value */
    ANR_STATE_INITIALIZED       = 1,                   /**< instance is created, but not initialized */
    ANR_STATE_STOPPED           = 2,                   /**< instance is confiured (ready to start) or stopped */
    ANR_STATE_RUNNING           = 3,                   /**< instance is running (processes frames) */
    ANR_STATE_LOCKED            = 4,                   /**< instance is locked (for taking snapshots) */
    ANR_STATE_MAX                                      /**< max */
} ANRState_t;

typedef enum ANROPMode_e {
    ANR_OP_MODE_INVALID           = 0,                   /**< initialization value */
    ANR_OP_MODE_AUTO              = 1,                   /**< instance is created, but not initialized */
    ANR_OP_MODE_MANUAL            = 2,                   /**< instance is confiured (ready to start) or stopped */
    ANR_OP_MODE_MAX                                      /**< max */
} ANROPMode_t;

typedef enum ANRParamMode_e {
	ANR_PARAM_MODE_INVALID           = 0, 
    ANR_PARAM_MODE_NORMAL          = 1,                   /**< initialization value */
    ANR_PARAM_MODE_HDR              = 2,                   /**< instance is created, but not initialized */
    ANR_PARAM_MODE_GRAY            = 3,                   /**< instance is confiured (ready to start) or stopped */
    ANR_PARAM_MODE_MAX                                      /**< max */
} ANRParamMode_t;

typedef struct RKAnr_Mfnr_Dynamic_s {
	int enable;
	float lowth_iso;
	float lowth_time;
	float highth_iso;
	float highth_time;
	int mfnr_enable_state;
}RKAnr_Mfnr_Dynamic_t;

typedef struct ANR_Manual_Attr_s
{
    int bayernrEn;
    RKAnr_Bayernr_Params_Select_t stBayernrParamSelect;

    int mfnrEn;
    RKAnr_Mfnr_Params_Select_t stMfnrParamSelect;

    int ynrEn;
    RKAnr_Ynr_Params_Select_t stYnrParamSelect;

    int uvnrEn;
    RKAnr_Uvnr_Params_Select_t stUvnrParamSelect;


} ANR_Manual_Attr_t;

typedef struct ANR_Auto_Attr_s
{
    //all ISO params and select param
    //bayernr
    int bayernrEn;
    RKAnr_Bayernr_Params_t stBayernrParams;
    RKAnr_Bayernr_Params_Select_t stBayernrParamSelect;

    //mfnr
    int mfnrEn;
    RKAnr_Mfnr_Params_t stMfnrParams;
    RKAnr_Mfnr_Params_Select_t stMfnrParamSelect;

    //ynr
    int ynrEn;
    RKAnr_Ynr_Params_t stYnrParams;
    RKAnr_Ynr_Params_Select_t stYnrParamSelect;

    //uvnr
    int uvnrEn;
    RKAnr_Uvnr_Params_t stUvnrParams;
    RKAnr_Uvnr_Params_Select_t stUvnrParamSelect;

    RKAnr_Mfnr_Dynamic_t stMfnr_dynamic;

} ANR_Auto_Attr_t;

typedef struct ANRProcResult_s {
    int bayernrEn;
    int mfnrEn;
    int ynrEN;
    int uvnrEn;

    //for sw simultaion
    RKAnr_Bayernr_Params_Select_t stBayernrParamSelect;
    RKAnr_Mfnr_Params_Select_t stMfnrParamSelect;
    RKAnr_Ynr_Params_Select_t stYnrParamSelect;
    RKAnr_Uvnr_Params_Select_t stUvnrParamSelect;

    //for fpga
    RKAnr_Bayernr_Fix_t stBayernrFix;
    RKAnr_Mfnr_Fix_t stMfnrFix;
    RKAnr_Uvnr_Fix_t stUvnrFix;
    RKAnr_Ynr_Fix_t stYnrFix;
    RKAnr_Gain_Fix_t stGainFix;
    //for motion detection
    CalibDb_MFNR_Motion_t  stMotion;
} ANRProcResult_t;


typedef struct ANRConfig_s {

    ANRState_t eState;
    ANROPMode_t eMode;

    int refYuvBit;

    //manual mode get nr & sharp levle from config to anrContext
    //unsigned int u32NrLevel;
    //unsigned int u32SharpLevel;

} ANRConfig_t;


typedef struct rk_aiq_nr_attrib_s {
    ANROPMode_t eMode;
    ANR_Auto_Attr_t stAuto;
    ANR_Manual_Attr_t stManual;
} rk_aiq_nr_attrib_t;


typedef struct ANRExpInfo_s {
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
} ANRExpInfo_t;


typedef enum rk_aiq_nr_module_e{
    ANR_MODULE_BAYERNR      = 0,                   /**< instance is created, but not initialized */
    ANR_MODULE_MFNR         = 1,                   /**< instance is confiured (ready to start) or stopped */
    ANR_MODULE_UVNR         = 2,                   /**< instance is running (processes frames) */
    ANR_MODULE_YNR          = 3,                   /**< instance is locked (for taking snapshots) */               /**< max */
} rk_aiq_nr_module_t;


typedef struct rk_aiq_nr_IQPara_s {
    int module_bits;//judge by bits
	
	CalibDb_BayerNr_t stBayernrPara;
	CalibDb_MFNR_t stMfnrPara;
	CalibDb_UVNR_t stUvnrPara;
	CalibDb_YNR_t stYnrPara;
    
} rk_aiq_nr_IQPara_t;


typedef struct rk_aiq_nr_JsonPara_s {
    int module_bits;//judge by bits
	
	CalibDbV2_BayerNrV1_t bayernr_v1;
	CalibDbV2_MFNR_t mfnr_v1;
	CalibDbV2_UVNR_t uvnr_v1;
	CalibDbV2_YnrV1_t ynr_v1;
    
} rk_aiq_nr_JsonPara_t;



RKAIQ_END_DECLARE

#endif

