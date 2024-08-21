/**
  * Copyright (c) 2023 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#ifndef __PARSE_H__
#define __PARSE_H__

#include "aupipe.h"

#ifdef RT_USING_AUPIPE_ECNR

typedef enum _SkvAECEnable
{
    AEC_EN_DELAY   = 1 << 0,
    AEC_EN_CHN_MAP = 1 << 1,
    AEC_EN_HOWLING = 1 << 2,
} SkvAecEnable;

typedef enum _SkvBFEnable
{
    BF_EN_FAST_AEC   = 1 << 0,
    BF_EN_WAKEUP     = 1 << 1,
    BF_EN_DEREV      = 1 << 2,
    BF_EN_NLP        = 1 << 3,
    BF_EN_AES        = 1 << 4,
    BF_EN_AGC        = 1 << 5,
    BF_EN_ANR        = 1 << 6,
    BF_EN_GSC        = 1 << 7,
    BF_GSC_METHOD    = 1 << 8,
    BF_EN_FIX        = 1 << 9,
    BF_EN_STDT       = 1 << 10,
    BF_EN_CNG        = 1 << 11,
    BF_EN_EQ         = 1 << 12,
    BF_EN_CHN_SELECT = 1 << 13,
} SkvBFEnable;

typedef enum _SkvRxEnable
{
    RX_EN_ANR = 1 << 0,
} SkvRxEnable;

typedef struct _SkvAecParam
{
    int pos;
    int drop_ref_channel;
    int aec_mode_en;
    int delay_len;
    int look_ahead;
    short *mic_chns_map;
} SkvAecParam;

typedef struct _SkvAnrParam
{
    float noiseFactor;
    int swU;
    float psiMin;
    float psiMax;
    float fGmin;
} SkvAnrParam;

typedef struct _SkvAgcParam
{
    float attack_time;
    float release_time;
    float max_gain;
    float max_peak;
    float fRth0;
    float fRk0;
    float fRth1;

    int fs;
    int frmlen;
    float attenuate_time;
    float fRth2;
    float fRk1;
    float fRk2;
    float fLineGainDb;
    int swSmL0;
    int swSmL1;
    int swSmL2;
} SkvAgcParam;

typedef struct _SkvDereverbParam
{
    int rlsLg;
    int curveLg;
    int delay;
    float forgetting;
    float t60;
    float coCoeff;
} SkvDereverbParam;

typedef struct _SkvNlpParam
{
    short nlp16k[8][2];
} SkvNlpParam;

typedef struct _SkvCngParam
{
    float fGain;
    float fMpy;
    float fSmoothAlpha;
    float fSpeechGain;
} SkvCngParam;

typedef struct _SkvDtdParam
{
    float ksiThd_high;
    float ksiThd_low;
} SkvDtdParam;

typedef struct _SkvAesParam
{
    float beta_up;
    float beta_down;
} SkvAesParam;

typedef struct _SkvEqParam
{
    int shwParaLen;
    short pfCoeff[5][13];
} SkvEqParam;

typedef struct _SkvBeamFormParam
{
    int model_en;
    int ref_pos;
    int targ;
    int num_ref_channel;
    int drop_ref_channel;

    SkvDereverbParam *dereverb;
    SkvAesParam *aes;
    SkvNlpParam *nlp;
    SkvAnrParam *anr;
    SkvAgcParam *agc;
    SkvCngParam *cng;
    SkvDtdParam *dtd;
    SkvEqParam *eq;
} SkvBeamFormParam;

typedef struct _SkvRxParam
{
    /* Parameters of agc */
    int model_en;
    SkvAnrParam *anr;
} SkvRxParam;

/* additions */
typedef enum _RTSKVMODE
{
    RT_SKV_AEC = 1 << 0,
    RT_SKV_BF  = 1 << 1,
    RT_SKV_RX  = 1 << 2,
    RT_SKV_CMD = 1 << 3,
} RTSKVMODE;

typedef struct _RTSkvParam
{
    int model;
    SkvAecParam *aec;
    SkvBeamFormParam *bf;
    SkvRxParam *rx;
} RTSkvParam;

typedef struct _SkvFrameParam
{
    int bits;
    int rate;

    int reserve;
    int period_size;
    int in_channels;
    int out_channels;
    int src_channels;
    int ref_channels;
} SkvFrameParam;

RTSkvParam *parse_encr_configs(char *file_path);

#endif

#endif
