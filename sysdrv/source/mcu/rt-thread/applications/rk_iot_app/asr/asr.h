/*
 *  Copyright (C) 2019, Fuzhou Rockchip Electronics Co., Ltd.
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
#ifndef __ASR_H__
#define __ASR_H__

#include <rtthread.h>
#include <stdio.h>

#ifdef PRJCONF_ASR_USE_VAD
#define APP_USING_VAD
#define AUDIO_VAD_BUFFER_SIZE   (64 * 1024) /* 128 KB */
#else
#undef APP_USING_VAD
#endif

#ifndef PRJCONF_ASR_SOUND_CARD
#define PRJCONF_ASR_SOUND_CARD "adcc"
#endif

#if defined(PRJCONF_AI_RK) || defined(PRJCONF_AI_SPEECH)
#define ASR_RECORD_SAMPLERATE 16000
#define ASR_RECORD_BUFFER_SIZE 4096
#define ASR_RECORD_CHANNELS 2
#define ASR_RECORD_BITS 16
#elif defined(PRJCONF_AI_CUSTOM) || defined(PRJCONF_AI_LWAKEUP)
#define ASR_RECORD_SAMPLERATE 48000
#define ASR_RECORD_BUFFER_SIZE 11520
#define ASR_RECORD_CHANNELS 4
#define ASR_RECORD_BITS 16
#endif

#define ASR_QUEUE_CMD_LOAD           0x00000001
#define ASR_QUEUE_CMD_STOP           0x00000002

#define WAKEUP_CONFIG                0x40000001
#define WAKEUP_START                 0x40000002
#define ASR_WAKEUP_CONFIG            0x40000003
#define ASR_WAKEUP_START             0x40000004
#define WAKEUP_ID                    0x50000001
#define ASR_WAKE_ID                  0x50000002
#define WAKEUP_WORD_LEN              32
#if defined(PRJCONF_AI_RK) || defined(PRJCONF_AI_SPEECH)
#define ASR_BUF_LEN                  320
#elif defined(PRJCONF_AI_CUSTOM)
#define ASR_BUF_LEN                  1024
#elif defined(PRJCONF_AI_LWAKEUP)
#define ASR_BUF_LEN                  2048
#endif

#define RKDSP_CTL_QUEUE_WORK         0x11
#define RKDSP_CTL_DEQUEUE_WORK       0x12

#ifndef APP_USING_VAD
#if defined(PRJCONF_AI_RK) || defined(PRJCONF_AI_SPEECH)
struct wake_work_param
{
    uint32_t buf;
    uint32_t buf_size;
    uint32_t result;
    uint32_t reserved;
};
#elif defined(PRJCONF_AI_CUSTOM)
struct wake_work_param
{
    uint32_t in_buf;
    uint32_t buf_size;
    uint32_t out_buf;
    uint32_t result;
    uint32_t aec_buf;
};
#elif defined(PRJCONF_AI_LWAKEUP)
struct wake_work_param
{
    uint32_t buf;
    uint32_t buf_size;
    uint32_t output;
    uint32_t result;
};
#endif
#endif

struct wake_cfg_param
{
    uint32_t advan_size;
    uint32_t period_size;
    uint32_t ringbuf_size;
    uint32_t ringbuf_addr;
    uint32_t mcu_hw_ptr;
    uint32_t dsp_hw_ptr;
    uint32_t result;
    uint32_t reserve[0];
};

enum asr_state
{
    ASR_IDLE = 0,
    ASR_RUNNING,
    ASR_STOP,
    ASR_ERROR
};

void asr_start_app(void);
int asr_load(void);
int asr_stop(void);

#endif
