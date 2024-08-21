// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "smart_door.h"

static AUDIO_SOUND_MODE_E find_sound_mode(RK_S32 ch) {
    AUDIO_SOUND_MODE_E channel = AUDIO_SOUND_MODE_BUTT;
    switch (ch) {
      case 1:
        channel = AUDIO_SOUND_MODE_MONO;
        break;
      case 2:
        channel = AUDIO_SOUND_MODE_STEREO;
        break;
      default:
        RK_LOGE("channel = %d not support", ch);
        return AUDIO_SOUND_MODE_BUTT;
    }

    return channel;
}

static AUDIO_BIT_WIDTH_E find_bit_width(RK_S32 bit) {
    AUDIO_BIT_WIDTH_E bitWidth = AUDIO_BIT_WIDTH_BUTT;
    switch (bit) {
      case 8:
        bitWidth = AUDIO_BIT_WIDTH_8;
        break;
      case 16:
        bitWidth = AUDIO_BIT_WIDTH_16;
        break;
      case 24:
        bitWidth = AUDIO_BIT_WIDTH_24;
        break;
      default:
        RK_LOGE("bitwidth(%d) not support", bit);
        return AUDIO_BIT_WIDTH_BUTT;
    }

    return bitWidth;
}

RK_S32 smart_door_init_ao(SMART_DOOR_CTX_S *ctx) {
    RK_S32 s32Ret = RK_SUCCESS;

    ctx->stAoCfg.s32DevId           = 0;
    ctx->stAoCfg.s32ChnIndex        = 0;
    ctx->stAoCfg.srcFilePath        = RK_NULL;
    ctx->stAoCfg.dstFilePath        = RK_NULL;
    ctx->stAoCfg.chCardName         = "hw:0,0";
    ctx->stAoCfg.s32Channel         = 2;
    ctx->stAoCfg.s32ReSmpSampleRate = 44100;
    ctx->stAoCfg.s32DeviceChannel   = 2;
    ctx->stAoCfg.s32SampleRate      = 44100;

    ctx->stAoCfg.s32LoopCount       = 1;
    ctx->stAoCfg.s32ChnNum          = 1;
    ctx->stAoCfg.s32BitWidth        = 16;
    ctx->stAoCfg.s32PeriodCount     = 4;
    ctx->stAoCfg.s32PeriodSize      = 1024;
    ctx->stAoCfg.s32SetVolume       = 100;
    ctx->stAoCfg.s32SetMute         = 0;
    ctx->stAoCfg.s32SetTrackMode    = 0;
    ctx->stAoCfg.s32SetFadeRate     = 0;
    ctx->stAoCfg.s32GetVolume       = 0;
    ctx->stAoCfg.s32GetMute         = 0;
    ctx->stAoCfg.s32GetTrackMode    = 0;
    ctx->stAoCfg.s32QueryChnStat    = 0;
    ctx->stAoCfg.s32PauseResumeChn  = 0;
    ctx->stAoCfg.s32SaveFile        = 0;
    ctx->stAoCfg.s32QueryFileStat   = 0;
    ctx->stAoCfg.s32ClrChnBuf       = 0;
    ctx->stAoCfg.s32ClrPubAttr      = 0;
    ctx->stAoCfg.s32GetPubAttr      = 0;
    ctx->stAoCfg.s32LoopbackMode    = AUDIO_LOOPBACK_NONE;

    return s32Ret;
}

RK_S32 smart_door_create_ao(SMART_DOOR_AO_CFG stAoCfg) {
    RK_S32 result = 0;
    AUDIO_DEV aoDevId = stAoCfg.s32DevId;
    AO_CHN aoChn = stAoCfg.s32ChnIndex;
    AUDIO_SOUND_MODE_E soundMode;

    AIO_ATTR_S aoAttr;
    memset(&aoAttr, 0, sizeof(AIO_ATTR_S));

    if (stAoCfg.chCardName) {
        snprintf(aoAttr.u8CardName, sizeof(aoAttr.u8CardName), "%s", stAoCfg.chCardName);
    }

    aoAttr.soundCard.channels = stAoCfg.s32DeviceChannel;
    aoAttr.soundCard.sampleRate = stAoCfg.s32SampleRate;
    aoAttr.soundCard.bitWidth = AUDIO_BIT_WIDTH_16;

    AUDIO_BIT_WIDTH_E bitWidth = find_bit_width(stAoCfg.s32BitWidth);
    if (bitWidth == AUDIO_BIT_WIDTH_BUTT) {
        goto __FAILED;
    }
    aoAttr.enBitwidth = bitWidth;
    aoAttr.enSamplerate = (AUDIO_SAMPLE_RATE_E)stAoCfg.s32ReSmpSampleRate;
    soundMode = find_sound_mode(stAoCfg.s32Channel);
    if (soundMode == AUDIO_SOUND_MODE_BUTT) {
        goto __FAILED;
    }
    aoAttr.enSoundmode = soundMode;
    aoAttr.u32FrmNum = stAoCfg.s32PeriodCount;
    aoAttr.u32PtNumPerFrm = stAoCfg.s32PeriodSize;

    aoAttr.u32EXFlag = 0;
    aoAttr.u32ChnCnt = 2;

    RK_MPI_AO_SetPubAttr(aoDevId, &aoAttr);

    RK_MPI_AO_Enable(aoDevId);

    AO_CHN_PARAM_S pstParams;
    memset(&pstParams, 0, sizeof(AO_CHN_PARAM_S));

    pstParams.enLoopbackMode = (AUDIO_LOOPBACK_MODE_E)stAoCfg.s32LoopbackMode;
    result = RK_MPI_AO_SetChnParams(aoDevId, aoChn, &pstParams);
    if (result != RK_SUCCESS) {
        RK_LOGE("ao set channel params, aoChn = %d", aoChn);
        goto __FAILED;
    }

    result = RK_MPI_AO_EnableChn(aoDevId, aoChn);
    if (result != RK_SUCCESS) {
        RK_LOGE("ao enable channel fail, aoChn = %d, reason = %x", aoChn, result);
        goto __FAILED;
    }

    result = RK_MPI_AO_EnableReSmp(aoDevId, aoChn, (AUDIO_SAMPLE_RATE_E)stAoCfg.s32ReSmpSampleRate);
    if (result != RK_SUCCESS) {
        RK_LOGE("ao enable channel fail, reason = %x, aoChn = %d", result, aoChn);
        goto __FAILED;
    }

    RK_MPI_AO_SetTrackMode(aoDevId, AUDIO_TRACK_OUT_STEREO);

    return RK_SUCCESS;
__FAILED:
    return RK_FAILURE;
}

RK_S32 smart_door_destroy_ao(SMART_DOOR_AO_CFG stAoCfg) {

    RK_S32 result = RK_MPI_AO_DisableReSmp(stAoCfg.s32DevId, stAoCfg.s32ChnIndex);
    if (result) {
        RK_LOGE("ao disable resample fail, reason = %d", result);
        return RK_FAILURE;
    }

    result = RK_MPI_AO_DisableChn(stAoCfg.s32DevId, stAoCfg.s32ChnIndex);
    if (result) {
        RK_LOGE("ao disable channel fail, reason = %d", result);
        return RK_FAILURE;
    }

    return RK_SUCCESS;
}