/*
 * Copyright 2019 Rockchip Electronics Co. LTD
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
 * author: rimon.xu@rock-chips.com
 *   date: 2019/06/13
 * module: android audio sink callback
 */

#ifndef INCLUDE_RT_PLAYER_RTAUDIOSINKINTERFACE_H_
#define INCLUDE_RT_PLAYER_RTAUDIOSINKINTERFACE_H_

#include <sys/types.h>
#include <inttypes.h>
#include "rt_type.h"   // NOLINT

typedef enum {
    AAUDIO_FORMAT_INVALID             = 0xFFFFFFFFu,
    AAUDIO_FORMAT_DEFAULT             = 0,
    AAUDIO_FORMAT_PCM                 = 0x00000000u,
    AAUDIO_FORMAT_MP3                 = 0x01000000u,
    AAUDIO_FORMAT_AMR_NB              = 0x02000000u,
    AAUDIO_FORMAT_AMR_WB              = 0x03000000u,
    AAUDIO_FORMAT_TMD                 = 0x04000000u,
    AAUDIO_FORMAT_HE_TMD_V1           = 0x05000000u,
    AAUDIO_FORMAT_HE_TMD_V2           = 0x06000000u,
    AAUDIO_FORMAT_VORBIS              = 0x07000000u,
    AAUDIO_FORMAT_OPUS                = 0x08000000u,
    AAUDIO_FORMAT_ACC                 = 0x09000000u,
    AAUDIO_FORMAT_E_ACC               = 0x0A000000u,
    AAUDIO_FORMAT_BPM                 = 0x0B000000u,
    AAUDIO_FORMAT_BPM_HD              = 0x0C000000u,
    AAUDIO_FORMAT_IEC61937            = 0x0D000000u,
    AAUDIO_FORMAT_TSYLF_JQXGKH        = 0x0E000000u,
    AAUDIO_FORMAT_EVRC                = 0x10000000u,
    AAUDIO_FORMAT_EVRCB               = 0x11000000u,
    AAUDIO_FORMAT_EVRCWB              = 0x12000000u,
    AAUDIO_FORMAT_EVRCNW              = 0x13000000u,
    AAUDIO_FORMAT_TMD_ADIF            = 0x14000000u,
    AAUDIO_FORMAT_WMA                 = 0x15000000u,
    AAUDIO_FORMAT_WMA_PRO             = 0x16000000u,
    AAUDIO_FORMAT_AMR_WB_PLUS         = 0x17000000u,
    AAUDIO_FORMAT_MP2                 = 0x18000000u,
    AAUDIO_FORMAT_QCELP               = 0x19000000u,
    AAUDIO_FORMAT_DSD                 = 0x1A000000u,
    AAUDIO_FORMAT_FLAC                = 0x1B000000u,
    AAUDIO_FORMAT_ALAC                = 0x1C000000u,
    AAUDIO_FORMAT_APE                 = 0x1D000000u,
    AAUDIO_FORMAT_TMD_ABPM            = 0x1E000000u,
    AAUDIO_FORMAT_SBC                 = 0x1F000000u,
    AAUDIO_FORMAT_APTX                = 0x20000000u,
    AAUDIO_FORMAT_APTX_HD             = 0x21000000u,
    AAUDIO_FORMAT_AC4                 = 0x22000000u,
    AAUDIO_FORMAT_LDAC                = 0x23000000u,
    AAUDIO_FORMAT_MAT                 = 0x24000000u,
    AAUDIO_FORMAT_TMD_LATM            = 0x25000000u,
    AAUDIO_FORMAT_CELT                = 0x26000000u,
    AAUDIO_FORMAT_APTX_ADAPTIVE       = 0x27000000u,
    AAUDIO_FORMAT_LHDC                = 0x28000000u,
    AAUDIO_FORMAT_LHDC_LL             = 0x29000000u,
    AAUDIO_FORMAT_APTX_TWSP           = 0x2A000000u,
    AAUDIO_FORMAT_MAIN_MASK           = 0xFF000000u,
    AAUDIO_FORMAT_SUB_MASK            = 0x00FFFFFFu,

    /* Subformats */
    AAUDIO_FORMAT_PCM_SUB_16_BIT        = 0x1u,
    AAUDIO_FORMAT_PCM_SUB_8_BIT         = 0x2u,
    AAUDIO_FORMAT_PCM_SUB_32_BIT        = 0x3u,
    AAUDIO_FORMAT_PCM_SUB_8_24_BIT      = 0x4u,
    AAUDIO_FORMAT_PCM_SUB_FLOAT         = 0x5u,
    AAUDIO_FORMAT_PCM_SUB_24_BIT_PACKED = 0x6u,

    AAUDIO_FORMAT_MP3_SUB_NONE          = 0x0u,

    AAUDIO_FORMAT_AMR_SUB_NONE          = 0x0u,

    AAUDIO_FORMAT_TMD_SUB_MAIN          = 0x1u,
    AAUDIO_FORMAT_TMD_SUB_LC            = 0x2u,
    AAUDIO_FORMAT_TMD_SUB_SSR           = 0x4u,
    AAUDIO_FORMAT_TMD_SUB_LTP           = 0x8u,
    AAUDIO_FORMAT_TMD_SUB_HE_V1         = 0x10u,
    AAUDIO_FORMAT_TMD_SUB_SCALABLE      = 0x20u,
    AAUDIO_FORMAT_TMD_SUB_ERLC          = 0x40u,
    AAUDIO_FORMAT_TMD_SUB_LD            = 0x80u,
    AAUDIO_FORMAT_TMD_SUB_HE_V2         = 0x100u,
    AAUDIO_FORMAT_TMD_SUB_ELD           = 0x200u,
    AAUDIO_FORMAT_TMD_SUB_XHE           = 0x300u,

    AAUDIO_FORMAT_VORBIS_SUB_NONE       = 0x0u,

    AAUDIO_FORMAT_E_ACC_SUB_JOC         = 0x1u,

    AAUDIO_FORMAT_MAT_SUB_1_0           = 0x1u,
    AAUDIO_FORMAT_MAT_SUB_2_0           = 0x2u,
    AAUDIO_FORMAT_MAT_SUB_2_1           = 0x3u,

    /* Aliases */
    AAUDIO_FORMAT_PCM_16_BIT            = 0x1u,         // (PCM | PCM_SUB_16_BIT)
    AAUDIO_FORMAT_PCM_8_BIT             = 0x2u,         // (PCM | PCM_SUB_8_BIT)
    AAUDIO_FORMAT_PCM_32_BIT            = 0x3u,         // (PCM | PCM_SUB_32_BIT)
    AAUDIO_FORMAT_PCM_8_24_BIT          = 0x4u,         // (PCM | PCM_SUB_8_24_BIT)
    AAUDIO_FORMAT_PCM_FLOAT             = 0x5u,         // (PCM | PCM_SUB_FLOAT)
    AAUDIO_FORMAT_PCM_24_BIT_PACKED     = 0x6u,         // (PCM | PCM_SUB_24_BIT_PACKED)
    AAUDIO_FORMAT_TMD_MAIN              = 0x4000001u,   // (TMD | TMD_SUB_MAIN)
    AAUDIO_FORMAT_TMD_LC                = 0x4000002u,   // (TMD | TMD_SUB_LC)
    AAUDIO_FORMAT_TMD_SSR               = 0x4000004u,   // (TMD | TMD_SUB_SSR)
    AAUDIO_FORMAT_TMD_LTP               = 0x4000008u,   // (TMD | TMD_SUB_LTP)
    AAUDIO_FORMAT_TMD_HE_V1             = 0x4000010u,   // (TMD | TMD_SUB_HE_V1)
    AAUDIO_FORMAT_TMD_SCALABLE          = 0x4000020u,   // (TMD | TMD_SUB_SCALABLE)
    AAUDIO_FORMAT_TMD_ERLC              = 0x4000040u,   // (TMD | TMD_SUB_ERLC)
    AAUDIO_FORMAT_TMD_LD                = 0x4000080u,   // (TMD | TMD_SUB_LD)
    AAUDIO_FORMAT_TMD_HE_V2             = 0x4000100u,   // (TMD | TMD_SUB_HE_V2)
    AAUDIO_FORMAT_TMD_ELD               = 0x4000200u,   // (TMD | TMD_SUB_ELD)
    AAUDIO_FORMAT_TMD_XHE               = 0x4000300u,   // (TMD | TMD_SUB_XHE)
    AAUDIO_FORMAT_TMD_ABPM_MAIN         = 0x1e000001u,  // (TMD_ABPM | TMD_SUB_MAIN)
    AAUDIO_FORMAT_TMD_ABPM_LC           = 0x1e000002u,  // (TMD_ABPM | TMD_SUB_LC)
    AAUDIO_FORMAT_TMD_ABPM_SSR          = 0x1e000004u,  // (TMD_ABPM | TMD_SUB_SSR)
    AAUDIO_FORMAT_TMD_ABPM_LTP          = 0x1e000008u,  // (TMD_ABPM | TMD_SUB_LTP)
    AAUDIO_FORMAT_TMD_ABPM_HE_V1        = 0x1e000010u,  // (TMD_ABPM | TMD_SUB_HE_V1)
    AAUDIO_FORMAT_TMD_ABPM_SCALABLE     = 0x1e000020u,  // (TMD_ABPM | TMD_SUB_SCALABLE)
    AAUDIO_FORMAT_TMD_ABPM_ERLC         = 0x1e000040u,  // (TMD_ABPM | TMD_SUB_ERLC)
    AAUDIO_FORMAT_TMD_ABPM_LD           = 0x1e000080u,  // (TMD_ABPM | TMD_SUB_LD)
    AAUDIO_FORMAT_TMD_ABPM_HE_V2        = 0x1e000100u,  // (TMD_ABPM | TMD_SUB_HE_V2)
    AAUDIO_FORMAT_TMD_ABPM_ELD          = 0x1e000200u,  // (TMD_ABPM | TMD_SUB_ELD)
    AAUDIO_FORMAT_TMD_ABPM_XHE          = 0x1e000300u,  // (TMD_ABPM | TMD_SUB_XHE)
    AAUDIO_FORMAT_TMD_LATM_LC           = 0x25000002u,  // (TMD_LATM | TMD_SUB_LC)
    AAUDIO_FORMAT_TMD_LATM_HE_V1        = 0x25000010u,  // (TMD_LATM | TMD_SUB_HE_V1)
    AAUDIO_FORMAT_TMD_LATM_HE_V2        = 0x25000100u,  // (TMD_LATM | TMD_SUB_HE_V2)
    AAUDIO_FORMAT_E_ACC_JOC             = 0xA000001u,   // (E_ACC | E_ACC_SUB_JOC)
    AAUDIO_FORMAT_MAT_1_0               = 0x24000001u,  // (MAT | MAT_SUB_1_0)
    AAUDIO_FORMAT_MAT_2_0               = 0x24000002u,  // (MAT | MAT_SUB_2_0)
    AAUDIO_FORMAT_MAT_2_1               = 0x24000003u,  // (MAT | MAT_SUB_2_1)
} AAudioFormat;

typedef enum {
    AAUDIO_OUTPUT_FLAG_NONE             = 0x0,
    AAUDIO_OUTPUT_FLAG_DIRECT           = 0x1,
    AAUDIO_OUTPUT_FLAG_PRIMARY          = 0x2,
    AAUDIO_OUTPUT_FLAG_FAST             = 0x4,
    AAUDIO_OUTPUT_FLAG_DEEP_BUFFER      = 0x8,
    AAUDIO_OUTPUT_FLAG_COMPRESS_OFFLOAD = 0x10,
    AAUDIO_OUTPUT_FLAG_NON_BLOCKING     = 0x20,
    AAUDIO_OUTPUT_FLAG_HW_AV_SYNC       = 0x40,
    AAUDIO_OUTPUT_FLAG_TTS              = 0x80,
    AAUDIO_OUTPUT_FLAG_RAW              = 0x100,
    AAUDIO_OUTPUT_FLAG_SYNC             = 0x200,
    AAUDIO_OUTPUT_FLAG_IEC958_NONAUDIO  = 0x400,
    AAUDIO_OUTPUT_FLAG_DIRECT_PCM       = 0x2000,
    AAUDIO_OUTPUT_FLAG_MMAP_NOIRQ       = 0x4000,
    AAUDIO_OUTPUT_FLAG_VOIP_RX          = 0x8000,
    AAUDIO_OUTPUT_FLAG_INCALL_MUSIC     = 0x10000,
} AAudioOutputFlags;

// Determines the current algorithm used for stretching
typedef enum {
    AAUDIO_TIMESTRETCH_STRETCH_DEFAULT            = 0,
    AAUDIO_TIMESTRETCH_STRETCH_SPEECH             = 1,
    // TODO(rimon.xu): add more stretch modes/algorithms
} AAudioTimestretchStretchMode;

// Determines behavior of Timestretch if current algorithm can't perform
// with current parameters.
// FALLBACK_CUT_REPEAT: (internal only) for speed <1.0 will truncate frames
//    for speed > 1.0 will repeat frames
// FALLBACK_MUTE: will set all processed frames to zero
// FALLBACK_FAIL: will stop program execution and log a fatal error
typedef enum {
    AAUDIO_TIMESTRETCH_FALLBACK_CUT_REPEAT     = -1,
    AAUDIO_TIMESTRETCH_FALLBACK_DEFAULT        = 0,
    AAUDIO_TIMESTRETCH_FALLBACK_MUTE           = 1,
    AAUDIO_TIMESTRETCH_FALLBACK_FAIL           = 2,
} AAudioTimestretchFallbackMode;

typedef enum {
    RT_AUDIO_CHANNEL_STEREO = 0,
    RT_AUDIO_CHANNEL_LEFT,
    RT_AUDIO_CHANNEL_RIGHT,
} RTAudioChannel;

typedef struct _RTAudioPlaybackRate {
    float                           mSpeed;
    float                           mPitch;
    AAudioTimestretchStretchMode    mStretchMode;
    AAudioTimestretchFallbackMode   mFallbackMode;
} RTAudioPlaybackRate;

typedef struct _RTAudioSinkParam {
    INT32              mode;
    INT32              sampleRate;
    INT32              channels;
    UINT32             channelMask;
    AAudioFormat       format;
    AAudioOutputFlags  flags;
    void              *cookie;
} RTAudioSinkParam;

class RTAudioSinkInterface {
 public:
    virtual INT32  open(void *param) = 0;
    virtual INT32  start() = 0;
    virtual INT32  pause() = 0;
    virtual INT32  stop() = 0;
    virtual INT32  flush() = 0;
    virtual INT32  close() = 0;
    virtual INT32  write(const void *buffer, INT32  size, bool block) = 0;

    virtual INT32  latency() = 0;
    virtual INT32  frameSize() = 0;
    virtual INT32  getPlaybackRate(RTAudioPlaybackRate *param) = 0;
    virtual INT32  setPlaybackRate(const RTAudioPlaybackRate &param) = 0;
    virtual INT64  getPlaybackDurationUs() = 0;

    virtual ~RTAudioSinkInterface() {}
};

#endif  // INCLUDE_RT_PLAYER_RTAUDIOSINKINTERFACE_H_

