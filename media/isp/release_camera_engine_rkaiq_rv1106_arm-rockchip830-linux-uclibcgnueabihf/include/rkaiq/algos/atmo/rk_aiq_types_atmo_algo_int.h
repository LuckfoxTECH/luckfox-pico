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
#ifndef __RK_AIQ_TYPES_ATMO_ALGO_INT_H__
#define __RK_AIQ_TYPES_ATMO_ALGO_INT_H__

#include "RkAiqCalibDbTypes.h"
#include "atmo_head.h"

typedef struct globalLuma_s
{
    GlobalLumaMode_t globalLumaMode;
    float* EnvLv;
    int len;
    float* ISO;
    float Tolerance;
    float* GlobeLuma;
} globalLuma_t ;

typedef struct detailsHighLight_s
{
    DetailsHighLightMode_t DetailsHighLightMode;
    float* OEPdf;
    int len;
    float* EnvLv;
    float Tolerance;
    float* DetailsHighLight;

} detailsHighLight_t ;

typedef struct detailsLowLight_s
{
    DetailsLowLightMode_t DetailsLowLightMode;
    float* FocusLuma;
    int len;
    float* DarkPdf;
    float* ISO;//use long frame
    float Tolerance;
    float* DetailsLowLight;

} detailsLowLight_t ;

typedef struct localtmo_s
{
    TmoTypeMode_t localtmoMode;
    float* DynamicRange;
    int len;
    float* EnvLv;
    float Tolerance;
    float* LocalTmoStrength;
} localtmo_t ;

typedef struct globaltmo_s
{
    bool isHdrGlobalTmo;
    TmoTypeMode_t mode;
    float iir;
    float* DynamicRange;
    int len;
    float* EnvLv;
    float Tolerance;
    float* GlobalTmoStrength;
} globaltmo_t;

typedef struct AtmoConfig_s {
    bool bTmoEn;
    bool isLinearTmo;
    globalLuma_t Luma;
    detailsHighLight_t DtsHiLit;
    detailsLowLight_t DtsLoLit;
    globaltmo_t global;
    localtmo_t local;
    float damp;
} AtmoConfig_t;

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

typedef struct atmoAttr_s
{
    tmoCtrlData_t stGlobeLuma;
    tmoCtrlData_t stDtlsLL;
    tmoCtrlData_t stDtlsHL;
    tmoCtrlData_t stLocalTMO;
    AGlobalTmoData_t stGlobalTMO;
} atmoAttr_t;

typedef struct mtmoAttr_s
{
    bool Enable;
    float stGlobeLuma;
    float stDtlsHL;
    float stDtlsLL;
    float stLocalTMOStrength;
    float stGlobalTMOStrength;
    float damp;
} mtmoAttr_t;

typedef struct atmoAttr_V2_s
{
    bool      bUpdateTmo;
    atmoAttr_t stTmoAuto;
} atmoAttr_V2_t;

typedef struct mtmoAttr_V2_s
{
    bool      bUpdateTmo;
    mtmoAttr_t stTmoManual;
} mtmoAttr_V2_t;

typedef enum tmo_OpMode_s {
    TMO_OPMODE_API_OFF = 0, // run IQ ahdr
    TMO_OPMODE_AUTO = 1, //run api auto ahdr
    TMO_OPMODE_MANU = 2, //run api manual ahdr
    TMO_OPMODE_SET_LEVEL = 3, // its prevously fast mode, run api set level
    TMO_OPMODE_DARKAREA = 4, // for dark area luma inprove, no matter the scene is night, normal, or hdr
    TMO_OPMODE_TOOL = 5, // for dark area luma inprove, no matter the scene is night, normal, or hdr
} tmo_OpMode_t;

typedef struct TmoCurrCtlData_s
{
    GlobalLumaMode_t GlobalLumaMode;
    DetailsHighLightMode_t DetailsHighLightMode;
    DetailsLowLightMode_t DetailsLowLightMode;
    TmoTypeMode_t GlobalTmoMode;
    TmoTypeMode_t LocalTMOMode;
    float Envlv;
    float ISO;
    float OEPdf;
    float FocusLuma;
    float DarkPdf;
    float DynamicRange;
} TmoCurrCtlData_t;

typedef struct TmoCurrRegData_s
{
    float GlobalLuma;
    float DetailsLowlight;
    float DetailsHighlight;
    float LocalTmoStrength;
    float GlobaltmoStrength;
} TmoCurrRegData_t;

typedef struct DarkArea_s
{
    int level;
} DarkArea_t;

typedef struct FastMode_s
{
    int level;
} FastMode_t;

typedef struct tmoAttr_s
{
    tmo_OpMode_t    opMode;
    atmoAttr_V2_t    stAuto;
    mtmoAttr_V2_t    stManual;
    FastMode_t    stSetLevel;
    DarkArea_t    stDarkArea;
    TmoCurrCtlData_t CtlInfo;
    TmoCurrRegData_t RegInfo;
    CalibDbV2_tmo_t stTool;
} tmoAttr_t;

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
    float StableThr;
    int iir;
    int iirmax;
    int height;
    int width;
    PredictKPara_t PredictK;
} TmoFlickerPara_t;

typedef struct RkAiqAtmoProcResult_s
{
    TmoProcRes_t Res;
    tmoAttr_t hdrAttr;
    bool LongFrameMode;
    bool isHdrGlobalTmo;
    bool bTmoEn;
    bool isLinearTmo;
    TmoFlickerPara_t TmoFlicker;
    PredictPara_t Predict;
} RkAiqAtmoProcResult_t;


#endif
