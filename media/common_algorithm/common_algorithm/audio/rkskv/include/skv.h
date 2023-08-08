/*
 *  Copyright (C) 2021, Fuzhou Rockchip Electronics Co., Ltd.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of Fuzhou Rockchip Electronics Co., Ltd. nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _SKV_LIB_DEFINE_
#define _SKV_LIB_DEFINE_

#define INFO_RIGHT 1                // Succeed initialize SKV preprocessor
#define INFO_PARAM_ERROR 2          // Cann't support the parameter of audio that you input
#define INFO_EXCEEDDATE_ERROR 3     // Exceed the limited date
#define INFO_UNKNOW_ERROR 4         // Unknown error

#define FRAME_SIZE 256
#define FRAME_SIZE_PBNLMS 256
#define NUM_PBNLMS_FILTER 4

typedef enum _SkvAECEnable{
    AEC_EN_DELAY   = 1 << 0,
    AEC_EN_CHN_MAP = 1 << 1,
} SkvAecEnable;

typedef enum _SkvBFEnable {
    BF_EN_FAST_AEC = 1 << 0,
    BF_EN_WAKEUP   = 1 << 1,
    BF_EN_DEREV    = 1 << 2,
    BF_EN_NLP      = 1 << 3,
    BF_EN_AES      = 1 << 4,
    BF_EN_AGC      = 1 << 5,
    BF_EN_ANR      = 1 << 6,
    BF_EN_GSC      = 1 << 7,
    BF_GSC_METHOD  = 1 << 8,
    BF_EN_FIX      = 1 << 9,
    BF_EN_STDT     = 1 << 10,
    BF_EN_CNG      = 1 << 11,
    BF_EN_EQ      = 1 << 12,
} SkvBFEnable;

typedef enum _SkvRxEnable{
    RX_EN_ANR = 1 << 0,
} SkvRxEnable;

typedef struct _SkvAecParam {
    int      pos;
    int      drop_ref_channel;
    int      aec_mode;
    int      delay_len;
    int      look_ahead;
    short   *mic_chns_map;
} SkvAecParam;

typedef struct _SkvAnrParam {
    float    noiseFactor;
    int      swU;
    float    psiMin;
    float    psiMax;
    float    fGmin;
} SkvAnrParam;

typedef struct _SkvAgcParam {
    float    attack_time;
    float    release_time;
    float    max_gain;
    float    max_peak;
    float    fRth0;
    float    fRk0;
    float    fRth1;

    int      fs;
    int      frmlen;
    float    attenuate_time;
    float    fRth2;
    float    fRk1;
    float    fRk2;
    float    fLineGainDb;
    int      swSmL0;
    int      swSmL1;
    int      swSmL2;
} SkvAgcParam;

typedef struct _SkvDereverbParam {
    int      rlsLg;
    int      curveLg;
    int      delay;
    float    forgetting;
    float    t60;
    float    coCoeff;
} SkvDereverbParam;

typedef struct _SkvNlpParam {
    short    nlp16k[8][2];
} SkvNlpParam;

typedef struct _SkvCngParam{
    float    fGain;
    float    fMpy;
    float    fSmoothAlpha;
    float    fSpeechGain;
} SkvCngParam;

typedef struct _SkvDtdParam {
    float    ksiThd_high;
    float    ksiThd_low;
} SkvDtdParam;

typedef struct _SkvAesParam {
    float beta_up;
    float beta_down;
} SkvAesParam;

typedef struct _SkvEqParam {
    int shwParaLen;
    short pfCoeff[5][13];
} SkvEqParam;

typedef struct _SkvBeamFormParam {
    int      model_en;
    int      ref_pos;
    int      targ;
    int      num_ref_channel;
    int      drop_ref_channel;

    SkvDereverbParam* dereverb;
    SkvAesParam* aes;
    SkvNlpParam* nlp;
    SkvAnrParam* anr;
    SkvAgcParam* agc;
    SkvCngParam* cng;
    SkvDtdParam* dtd;
    SkvEqParam* eq;
} SkvBeamFormParam;

typedef struct _SkvRxParam {
    /* Parameters of agc */
    int model_en;
    SkvAnrParam* anr;
} SkvRxParam;

extern void* rkaudio_preprocess_init(int sampling_rate, int bits_persample,
    int num_src_channel, int num_ref_channel, void *param);
extern void rkaudio_preprocess_destory(void *handle);
extern int rkaudio_preprocess_short(void *handle, short *in, short *out,
    int in_size, int *is_wakeup);
extern void rkaudio_param_printf(int mic, int ref, void *param);

#endif  // _SKV_LIB_DEFINE_
