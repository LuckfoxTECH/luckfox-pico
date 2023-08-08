/******************************************************************************
 *
 * Copyright 2019, Fuzhou Rockchip Electronics Co.Ltd . All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Fuzhou Rockchip Electronics Co.Ltd .
 *
 *
 *****************************************************************************/
#ifndef __RK_AIQ_TYPES_AHDR_ALGO_INT_H__
#define __RK_AIQ_TYPES_AHDR_ALGO_INT_H__

#include "RkAiqCalibDbTypes.h"

enum {
    AHDR_NORMAL = 0,
    AHDR_HDR = 1,
    AHDR_NIGHT = 2
};

typedef struct globalLuma_s
{
    float globalLumaMode;
    float EnvLv[13];
    float ISO[13];
    float Tolerance;
    float GlobeLuma[13];

} globalLuma_t ;

typedef struct detailsHighLight_s
{
    float DetailsHighLightMode;
    float OEPdf[13];
    float EnvLv[13];
    float Tolerance;
    float DetailsHighLight[13];

} detailsHighLight_t ;

typedef struct detailsLowLight_s
{
    float DetailsLowLightMode;
    float FocusLuma[13];
    float DarkPdf[13];
    float ISO[13];//use long frame
    float Tolerance;
    float DetailsLowLight[13];

} detailsLowLight_t ;

typedef struct localtmo_s
{
    float localtmoMode;
    float DynamicRange[13];
    float EnvLv[13];
    float Tolerance;
    float LocalTmoStrength[13];

} localtmo_t ;

typedef struct globaltmo_s
{
    bool isHdrGlobalTmo;
    float mode;
    float iir;
    float DynamicRange[13];
    float EnvLv[13];
    float Tolerance;
    float GlobalTmoStrength[13];
} globaltmo_t;

typedef struct tmo_config_s
{
    bool bTmoEn;
    bool isLinearTmo;
    globalLuma_t Luma;
    detailsHighLight_t DtsHiLit;
    detailsLowLight_t DtsLoLit;
    globaltmo_t global;
    localtmo_t local;
    float damp;
    float AeConvergedPdf;
} tmo_config_t ;

typedef struct merge_config_s {
    int MergeMode;
    float EnvLv[13]; //0: dark 1:bright
    float MoveCoef[13];
    float OECurve_smooth[13];  //current over exposure curve slope
    float OECurve_offset[13];  //current over exposure curve offset
    float MDCurveLM_smooth[13];  //Move Detect curve slope betwwen long frame and middle frame
    float MDCurveLM_offset[13];  //Move Detect curve offset betwwen long frame and middle frame
    float MDCurveMS_smooth[13];  //Move Detect curve slope betwwen middle frame and short frame
    float MDCurveMS_offset[13];  //Move Detect curve slope betwwen middle frame and short frame
    float OECurve_damp;
    float MDCurveLM_damp;
    float MDCurveMS_damp ;
} merge_config_t;

typedef struct AhdrConfig_s {
    merge_config_t merge_para;
    tmo_config_t   tmo_para;
} AhdrConfig_t;

typedef struct tmoCtrlData_s
{
    float stCoef;
    float stCoefMax;
    float stCoefMin;
    int   stMax;
    int   stMin;
} tmoCtrlData_t;

typedef struct AGlobalTmoData_s
{
    bool en;
    float stCoef;
    float stCoefMax;
    float stCoefMin;
    int   stMax;
    int   stMin;
} AGlobalTmoData_t;

typedef struct mgeCtrlData_S
{
    float stCoef;
    float stCoefMax;
    float stCoefMin;
    int   stSmthMax;
    int   stSmthMin;
    int   stOfstMax;
    int   stOfstMin;
} mgeCtrlData_t;

typedef struct amgeAttr_s
{
    mgeCtrlData_t stMDCurveLM;
    mgeCtrlData_t stMDCurveMS;
    mgeCtrlData_t stOECurve;
} amgeAttr_t;

typedef struct atmoAttr_S
{
    tmoCtrlData_t stGlobeLuma;
    tmoCtrlData_t stDtlsLL;
    tmoCtrlData_t stDtlsHL;
    tmoCtrlData_t stLocalTMO;
    AGlobalTmoData_t stGlobalTMO;
} atmoAttr_t;

typedef struct mmgeAttr_S
{
    float OECurve_smooth;
    float OECurve_offset;
    float MDCurveLM_smooth;
    float MDCurveLM_offset;
    float MDCurveMS_smooth;
    float MDCurveMS_offset;
    float dampOE;
    float dampMDLM;
    float dampMDMS;
} mmgeAttr_t;

typedef struct mtmoAttr_S
{
    bool Enable;
    float stGlobeLuma;
    float stDtlsHL;
    float stDtlsLL;
    float stLocalTMOStrength;
    float stGlobalTMOStrength;
    float damp;
} mtmoAttr_t;

typedef struct ahdrAttr_S
{
    bool      bUpdateTmo;
    bool      bUpdateMge;
    amgeAttr_t stMgeAuto;
    atmoAttr_t stTmoAuto;
} ahdrAttr_t;

typedef struct mhdrAttr_s
{
    bool      bUpdateTmo;
    bool      bUpdateMge;
    mmgeAttr_t stMgeManual;
    mtmoAttr_t stTmoManual;
} mhdrAttr_t;

typedef enum hdr_OpMode_s {
    HDR_OpMode_Api_OFF = 0, // run IQ ahdr
    HDR_OpMode_Auto = 1, //run api auto ahdr
    HDR_OpMode_MANU = 2, //run api manual ahdr
    HDR_OpMode_SET_LEVEL = 3, // its prevously fast mode, run api set level
    HDR_OpMode_DarkArea = 4, // for dark area luma inprove, no matter the scene is night, normal, or hdr
    HDR_OpMode_Tool = 5, // for dark area luma inprove, no matter the scene is night, normal, or hdr
} hdr_OpMode_t;

typedef struct CurrCtlData_s
{
    int SceneMode;
    float GlobalLumaMode;
    float DetailsHighLightMode;
    float DetailsLowLightMode;
    float GlobalTmoMode;
    float LocalTMOMode;
    float Envlv;
    float MoveCoef;
    float ISO;
    float OEPdf;
    float FocusLuma;
    float DarkPdf;
    float DynamicRange;
} CurrCtlData_t;

typedef struct CurrRegData_s
{
    float OECurve_smooth;
    float OECurve_offset;
    float MDCurveLM_smooth;
    float MDCurveLM_offset;
    float MDCurveMS_smooth;
    float MDCurveMS_offset;

    float GlobalLuma;
    float DetailsLowlight;
    float DetailsHighlight;
    float LocalTmoStrength;
    float GlobaltmoStrength;
} CurrRegData_t;

typedef struct DarkArea_s
{
    int level;
} DarkArea_t;

typedef struct FastMode_s
{
    int level;
} FastMode_t;

typedef struct hdrAttr_s
{
    hdr_OpMode_t    opMode;
    ahdrAttr_t    stAuto;
    mhdrAttr_t stManual;
    FastMode_t stSetLevel;
    DarkArea_t stDarkArea;
    CurrCtlData_t CtlInfo;
    CurrRegData_t RegInfo;
    CalibDb_Ahdr_Para_t stTool;
} hdrAttr_t;

typedef struct MgeProcRes_s
{
    unsigned char  sw_hdrmge_mode;
    unsigned short sw_hdrmge_gain0_inv;
    unsigned short sw_hdrmge_gain0;
    unsigned short sw_hdrmge_gain1_inv;
    unsigned short sw_hdrmge_gain1;
    unsigned char  sw_hdrmge_gain2;
    unsigned char  sw_hdrmge_ms_dif_0p8;
    unsigned char  sw_hdrmge_lm_dif_0p9;
    unsigned char  sw_hdrmge_ms_dif_0p15;
    unsigned char  sw_hdrmge_lm_dif_0p15;
    unsigned short sw_hdrmge_l0_y[17];
    unsigned short sw_hdrmge_l1_y[17];
    unsigned short sw_hdrmge_e_y[17];
} MgeProcRes_t;

typedef struct TmoProcRes_s
{
    unsigned short sw_hdrtmo_cnt_vsize;
    unsigned char  sw_hdrtmo_gain_ld_off2;
    unsigned char  sw_hdrtmo_gain_ld_off1;
    unsigned char  sw_hdrtmo_big_en;
    unsigned char  sw_hdrtmo_nobig_en;
    unsigned char  sw_hdrtmo_newhist_en;
    unsigned char  sw_hdrtmo_cnt_mode;
    unsigned short sw_hdrtmo_expl_lgratio;
    unsigned char  sw_hdrtmo_lgscl_ratio;
    unsigned char  sw_hdrtmo_cfg_alpha;
    unsigned short sw_hdrtmo_set_gainoff;
    unsigned short sw_hdrtmo_set_palpha;
    unsigned short sw_hdrtmo_set_lgmax;
    unsigned short sw_hdrtmo_set_lgmin;
    unsigned char  sw_hdrtmo_set_weightkey;
    unsigned short sw_hdrtmo_set_lgmean;
    unsigned short sw_hdrtmo_set_lgrange1;
    unsigned short sw_hdrtmo_set_lgrange0;
    unsigned short sw_hdrtmo_set_lgavgmax;
    unsigned char  sw_hdrtmo_clipgap1;
    unsigned char  sw_hdrtmo_clipgap0;
    unsigned char  sw_hdrtmo_clipratio1;
    unsigned char  sw_hdrtmo_clipratio0;
    unsigned char  sw_hdrtmo_ratiol;
    unsigned short sw_hdrtmo_lgscl_inv;
    unsigned short sw_hdrtmo_lgscl;
    unsigned short sw_hdrtmo_lgmax;
    unsigned short sw_hdrtmo_hist_low;
    unsigned short sw_hdrtmo_hist_min;
    unsigned char  sw_hdrtmo_hist_shift;
    unsigned short sw_hdrtmo_hist_0p3;
    unsigned short sw_hdrtmo_hist_high;
    unsigned short sw_hdrtmo_palpha_lwscl;
    unsigned short sw_hdrtmo_palpha_lw0p5;
    unsigned short sw_hdrtmo_palpha_0p18;
    unsigned short sw_hdrtmo_maxgain;
    unsigned short sw_hdrtmo_maxpalpha;
} TmoProcRes_t;

typedef struct PredictPara_s
{
    bool Scenestable;
    int K_Rolgmean;
    int iir;
    int iir_max;
    int global_tmo_strength;
    int cnt_mode;
    int cnt_vsize;
} PredictPara_t;

typedef struct PredictKPara_s
{
    float correction_factor;
    float correction_offset;
    float Hdr3xLongPercent;
    float UseLongUpTh;
    float UseLongLowTh;

} PredictKPara_t;

typedef struct TmoFlickerPara_s
{
    int cnt_mode;
    int cnt_vsize;
    bool GlobalTmoStrengthDown;
    float GlobalTmoStrength;
    float LumaDeviation[3];
    float AeConvergedPdf;
    bool IsAeConverged;
    unsigned short ro_hdrtmo_lgmean;
    int iir;
    int iirmax;
    int height;
    int width;
    PredictKPara_t PredictK;
    float damp;
} TmoFlickerPara_t;

typedef struct RkAiqAhdrProcResult_s
{
    TmoProcRes_t TmoProcRes;
    MgeProcRes_t MgeProcRes;
    hdrAttr_t hdrAttr;
    bool LongFrameMode;
    bool isHdrGlobalTmo;
    bool bTmoEn;
    bool isLinearTmo;
    TmoFlickerPara_t TmoFlicker;
    PredictPara_t Predict;
} RkAiqAhdrProcResult_t;


#endif
