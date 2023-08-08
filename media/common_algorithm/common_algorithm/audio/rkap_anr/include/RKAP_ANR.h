/*
 *  Copyright (C) 2020, Fuzhou Rockchip Electronics Co., Ltd.
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
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __RKAP_ANR_H__
#define __RKAP_ANR_H__

#include "RKAP_Common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RKAP_ANR_State_S
{
    /* Basic info */
    int swSampleRate;        /* 8k~48k */
    int swFrameLen;          /* frame time only 10ms|16ms|20ms */
    int enHpfSwitch;         /* HighPass Filter Enable */
    int enLpfSwitch;         /* LowPass Filter Enable */

    float fPostAddGain;     /* post-gain */
    float fGmin;            /* spectral gain floor,unit:(dB),default:-30dB */
    float fNoiseFactor;     /* noise suppression factor,default:0.98 */
    float fHpfFc;           /* High Pass Filter cut-off frequency */
    float fLpfFc;           /* Low Pass Filter cut-off frequency */

} RKAP_ANR_State;

extern RKAP_Handle RKAP_ANR_Init(RKAP_ANR_State *st);
extern void RKAP_ANR_Destroy(RKAP_Handle handle);
extern int RKAP_ANR_Process(RKAP_Handle handle, short *pfSigIn, short *pfSigOut);
extern int RKAP_ANR_Control(RKAP_Handle handle, int request, void *arg);
extern void RKAP_ANR_DumpVersion(void);

#ifdef __cplusplus
}
#endif

#endif /* __RKAP_ANR_H__ */
