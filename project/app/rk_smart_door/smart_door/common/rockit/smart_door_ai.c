// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "smart_door.h"

static AUDIO_SOUND_MODE_E ai_find_sound_mode(RK_S32 ch) {
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

static AUDIO_BIT_WIDTH_E ai_find_bit_width(RK_S32 bit) {
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

static RK_S32 init_ai_aed(SMART_DOOR_AI_CFG stAiCfg) {
    RK_S32 result;

    if (stAiCfg.s32AedEnable) {
        AI_AED_CONFIG_S stAiAedConfig, stAiAedConfig2;

        stAiAedConfig.fSnrDB = 10.0f;
        stAiAedConfig.fLsdDB = -25.0f;
        stAiAedConfig.s32Policy = 1;

        result = RK_MPI_AI_SetAedAttr(stAiCfg.s32DevId, stAiCfg.s32ChnIndex, &stAiAedConfig);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: SetAedAttr(%d,%d) failed with %#x",
                __FUNCTION__, stAiCfg.s32DevId, stAiCfg.s32ChnIndex, result);
            return result;
        }

        result = RK_MPI_AI_GetAedAttr(stAiCfg.s32DevId, stAiCfg.s32ChnIndex, &stAiAedConfig2);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: SetAedAttr(%d,%d) failed with %#x",
                __FUNCTION__, stAiCfg.s32DevId, stAiCfg.s32ChnIndex, result);
            return result;
        }

        result = memcmp(&stAiAedConfig, &stAiAedConfig2, sizeof(AI_AED_CONFIG_S));
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: set/get aed config is different: %d", __FUNCTION__, result);
            return result;
        }

        result = RK_MPI_AI_EnableAed(stAiCfg.s32DevId, stAiCfg.s32ChnIndex);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: EnableAed(%d,%d) failed with %#x",
                __FUNCTION__, stAiCfg.s32DevId, stAiCfg.s32ChnIndex, result);
            return result;
        }
    }

    return RK_SUCCESS;
}

static RK_VOID bcd_callback() {
    RK_LOGI("baby cry callback successed\n");
}

static RK_S32 init_ai_bcd(SMART_DOOR_AI_CFG stAiCfg) {
    RK_S32 result;

    if (stAiCfg.s32BcdEnable) {
        AI_BCD_CONFIG_S stAiBcdConfig, stAiBcdConfig2;

        stAiBcdConfig.mFrameLen = 122;
        stAiBcdConfig.mBlankFrameMax = 222;
        stAiBcdConfig.mCryEnergy = -1.25f;
        stAiBcdConfig.mJudgeEnergy = -0.75f;
        stAiBcdConfig.mCryThres1 = 0.70f;
        stAiBcdConfig.mCryThres2 = 0.55f;

        result = RK_MPI_AI_SetBcdAttr(stAiCfg.s32DevId, stAiCfg.s32ChnIndex, &stAiBcdConfig);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: SetBcdAttr(%d,%d) failed with %#x",
                __FUNCTION__, stAiCfg.s32DevId, stAiCfg.s32ChnIndex, result);
            return result;
        }

        result = RK_MPI_AI_GetBcdAttr(stAiCfg.s32DevId, stAiCfg.s32ChnIndex, &stAiBcdConfig2);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: SetBcdAttr(%d,%d) failed with %#x",
                __FUNCTION__, stAiCfg.s32DevId, stAiCfg.s32ChnIndex, result);
            return result;
        }

        result = memcmp(&stAiBcdConfig, &stAiBcdConfig2, sizeof(AI_BCD_CONFIG_S));
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: set/get aed config is different: %d", __FUNCTION__, result);
            return result;
        }

        result = RK_MPI_AI_EnableBcd(stAiCfg.s32DevId, stAiCfg.s32ChnIndex);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: EnableBcd(%d,%d) failed with %#x",
                __FUNCTION__, stAiCfg.s32DevId, stAiCfg.s32ChnIndex, result);
            return result;
        }
    }

    return RK_SUCCESS;
}

static RK_S32 init_ai_vqe(SMART_DOOR_AI_CFG stAiCfg) {
    AI_VQE_CONFIG_S stAiVqeConfig, stAiVqeConfig2;
    RK_S32 result;

    if (stAiCfg.s32VqeEnable == 0)
        return RK_SUCCESS;

    // Need to config enCfgMode to VQE attr even the VQE is not enabled
    memset(&stAiVqeConfig, 0, sizeof(AI_VQE_CONFIG_S));
    if (stAiCfg.pVqeCfgPath != RK_NULL) {
        stAiVqeConfig.enCfgMode = AIO_VQE_CONFIG_LOAD_FILE;
        memcpy(stAiVqeConfig.aCfgFile, stAiCfg.pVqeCfgPath, strlen(stAiCfg.pVqeCfgPath));
    }

    if (stAiCfg.s32VqeGapMs != 16 && stAiCfg.s32VqeGapMs != 10) {
        RK_LOGE("Invalid gap: %d, just supports 16ms or 10ms for AI VQE", stAiCfg.s32VqeGapMs);
        return RK_FAILURE;
    }

    stAiVqeConfig.s32WorkSampleRate = stAiCfg.s32SampleRate;
    stAiVqeConfig.s32FrameSample = stAiCfg.s32SampleRate * stAiCfg.s32VqeGapMs / 1000;
    result = RK_MPI_AI_SetVqeAttr(stAiCfg.s32DevId, stAiCfg.s32ChnIndex, 0, 0, &stAiVqeConfig);
    if (result != RK_SUCCESS) {
        RK_LOGE("%s: SetVqeAttr(%d,%d) failed with %#x",
            __FUNCTION__, stAiCfg.s32DevId, stAiCfg.s32ChnIndex, result);
        return result;
    }

    result = RK_MPI_AI_GetVqeAttr(stAiCfg.s32DevId, stAiCfg.s32ChnIndex, &stAiVqeConfig2);
    if (result != RK_SUCCESS) {
        RK_LOGE("%s: SetVqeAttr(%d,%d) failed with %#x",
            __FUNCTION__, stAiCfg.s32DevId, stAiCfg.s32ChnIndex, result);
        return result;
    }

    result = memcmp(&stAiVqeConfig, &stAiVqeConfig2, sizeof(AI_VQE_CONFIG_S));
    if (result != RK_SUCCESS) {
        RK_LOGE("%s: set/get vqe config is different: %d", __FUNCTION__, result);
        return result;
    }

    result = RK_MPI_AI_EnableVqe(stAiCfg.s32DevId, stAiCfg.s32ChnIndex);
    if (result != RK_SUCCESS) {
        RK_LOGE("%s: EnableVqe(%d,%d) failed with %#x",
            __FUNCTION__, stAiCfg.s32DevId, stAiCfg.s32ChnIndex, result);
        return result;
    }

    return RK_SUCCESS;
}

RK_S32 smart_door_init_ai(SMART_DOOR_CTX_S *ctx) {
    RK_S32 s32Ret = RK_SUCCESS;

    ctx->stAiCfg.s32DevId           = 0;
    ctx->stAiCfg.s32ChnIndex        = 0;
    ctx->stAiCfg.s32DeviceSampleRate = 44100;
    ctx->stAiCfg.s32DeviceChannel   = 2;
    ctx->stAiCfg.s32SampleRate      = 44100;
    ctx->stAiCfg.s32Channel         = 2;
    ctx->stAiCfg.chCardName         = "hw:0,0";
    ctx->stAiCfg.srcFilePath        = RK_NULL;
    ctx->stAiCfg.dstFilePath        = RK_NULL;
    ctx->stAiCfg.s32LoopCount       = 1;
    ctx->stAiCfg.s32ChnNum          = 1;
    ctx->stAiCfg.s32BitWidth        = 16;
    ctx->stAiCfg.s32PeriodCount     = 4;
    ctx->stAiCfg.s32PeriodSize      = 1024;
    ctx->stAiCfg.s32DevFd           = -1;
    ctx->stAiCfg.s32SetVolume       = 100;
    ctx->stAiCfg.s32SetMute         = 0;
    ctx->stAiCfg.s32SetTrackMode    = 0;
    ctx->stAiCfg.s32SetFadeRate     = 0;
    ctx->stAiCfg.s32GetVolume       = 0;
    ctx->stAiCfg.s32GetMute         = 0;
    ctx->stAiCfg.s32GetTrackMode    = 0;
    ctx->stAiCfg.s32AedEnable       = 0;
    ctx->stAiCfg.s32BcdEnable       = 0;
    ctx->stAiCfg.s32VqeGapMs        = 16;
    ctx->stAiCfg.s32VqeEnable       = 0;
    ctx->stAiCfg.pVqeCfgPath        = RK_NULL;
    ctx->stAiCfg.s32LoopbackMode    = AUDIO_LOOPBACK_NONE;
    ctx->stAiCfg.s32DumpAlgo        = 0;

    return s32Ret;
}

RK_S32 smart_door_create_ai(SMART_DOOR_AI_CFG stAiCfg) {
    AUDIO_DEV aiDevId = stAiCfg.s32DevId;
    AUDIO_SOUND_MODE_E soundMode;

    AIO_ATTR_S aiAttr;
    RK_S32 result;
    memset(&aiAttr, 0, sizeof(AIO_ATTR_S));

    if (stAiCfg.chCardName) {
        snprintf(aiAttr.u8CardName, sizeof(aiAttr.u8CardName), "%s", stAiCfg.chCardName);
    }

    aiAttr.soundCard.channels = stAiCfg.s32DeviceChannel;
    aiAttr.soundCard.sampleRate = stAiCfg.s32DeviceSampleRate;
    AUDIO_BIT_WIDTH_E bitWidth = ai_find_bit_width(stAiCfg.s32BitWidth);
    if (bitWidth == AUDIO_BIT_WIDTH_BUTT) {
        goto __FAILED;
    }

    aiAttr.soundCard.bitWidth = bitWidth;
    aiAttr.enBitwidth = bitWidth;
    aiAttr.enSamplerate = (AUDIO_SAMPLE_RATE_E)stAiCfg.s32SampleRate;
    soundMode = ai_find_sound_mode(stAiCfg.s32Channel);
    if (soundMode == AUDIO_SOUND_MODE_BUTT) {
        goto __FAILED;
    }

    aiAttr.enSoundmode = soundMode;
    aiAttr.u32FrmNum = stAiCfg.s32PeriodCount;
    aiAttr.u32PtNumPerFrm = stAiCfg.s32PeriodSize;

    aiAttr.u32EXFlag = 0;
    aiAttr.u32ChnCnt = 2;

    result = RK_MPI_AI_SetPubAttr(aiDevId, &aiAttr);
    if (result != 0) {
        RK_LOGE("ai set attr fail, reason = %d", result);
        goto __FAILED;
    }

    result = RK_MPI_AI_Enable(aiDevId);
    if (result != 0) {
        RK_LOGE("ai enable fail, reason = %d", result);
        goto __FAILED;
    }

    AI_CHN aiChn = stAiCfg.s32ChnIndex;
    AI_CHN_PARAM_S pstParams;

    memset(&pstParams, 0, sizeof(AI_CHN_PARAM_S));
    pstParams.enLoopbackMode = (AUDIO_LOOPBACK_MODE_E)stAiCfg.s32LoopbackMode;
    pstParams.s32UsrFrmDepth = 1;
    result = RK_MPI_AI_SetChnParam(aiDevId, aiChn, &pstParams);
    if (result != RK_SUCCESS) {
        RK_LOGE("ai set channel params, aiChn = %d", aiChn);
        goto __FAILED;
    }

    result = init_ai_aed(stAiCfg);
    if (result != 0) {
        RK_LOGE("ai aed init fail, reason = %x, aiChn = %d", result, stAiCfg.s32ChnIndex);
        goto __FAILED;
    }

    result = init_ai_bcd(stAiCfg);
    if (result != 0) {
        RK_LOGE("ai bcd init fail, reason = %x, aiChn = %d", result, stAiCfg.s32ChnIndex);
        goto __FAILED;
    }

    result = init_ai_vqe(stAiCfg);
    if (result != 0) {
        RK_LOGE("ai vqe init fail, reason = %x, aiChn = %d", result, stAiCfg.s32ChnIndex);
        goto __FAILED;
    }

    result = RK_MPI_AI_EnableChn(stAiCfg.s32DevId, stAiCfg.s32ChnIndex);
    if (result != 0) {
        RK_LOGE("ai enable channel fail, aiChn = %d, reason = %x", stAiCfg.s32ChnIndex, result);
        goto __FAILED;
    }

    RK_BOOL needResample = (stAiCfg.s32DeviceSampleRate != stAiCfg.s32SampleRate) ? RK_TRUE : RK_FALSE;

    if (needResample == RK_TRUE) {
        RK_LOGI("need to resample %d -> %d", stAiCfg.s32DeviceSampleRate, stAiCfg.s32SampleRate);
        result = RK_MPI_AI_EnableReSmp(stAiCfg.s32DevId, stAiCfg.s32ChnIndex,
                                      (AUDIO_SAMPLE_RATE_E)stAiCfg.s32SampleRate);
        if (result != 0) {
            RK_LOGE("ai enable channel fail, reason = %x, aiChn = %d", result, stAiCfg.s32ChnIndex);
            goto __FAILED;
        }
    }

    return RK_SUCCESS;

__FAILED:
    return RK_FAILURE;
}

RK_S32 smart_door_destroy_ai(SMART_DOOR_AI_CFG stAiCfg) {
    RK_S32 result;

    RK_MPI_AI_DisableReSmp(stAiCfg.s32DevId, stAiCfg.s32ChnIndex);
    if (stAiCfg.s32BcdEnable) {
        result = RK_MPI_AI_DisableBcd(stAiCfg.s32DevId, stAiCfg.s32ChnIndex);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: RK_MPI_AI_DisableBcd(%d,%d) failed with %#x",
                __FUNCTION__, stAiCfg.s32DevId, stAiCfg.s32ChnIndex, result);
            return result;
        }
        stAiCfg.s32BcdEnable = 0;
    }

    if (stAiCfg.s32AedEnable) {
        result = RK_MPI_AI_DisableAed(stAiCfg.s32DevId, stAiCfg.s32ChnIndex);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: RK_MPI_AI_DisableAed(%d,%d) failed with %#x",
                __FUNCTION__, stAiCfg.s32DevId, stAiCfg.s32ChnIndex, result);
            return result;
        }
        stAiCfg.s32AedEnable = 0;
    }

    if (stAiCfg.s32VqeEnable) {
        result = RK_MPI_AI_DisableVqe(stAiCfg.s32DevId, stAiCfg.s32ChnIndex);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: RK_MPI_AI_DisableVqe(%d,%d) failed with %#x",
                __FUNCTION__, stAiCfg.s32DevId, stAiCfg.s32ChnIndex, result);
            return result;
        }
        stAiCfg.s32VqeEnable = 0;
    }

    result = RK_MPI_AI_DisableChn(stAiCfg.s32DevId, stAiCfg.s32ChnIndex);
    if (result != 0) {
        RK_LOGE("ai disable channel fail, reason = %d", result);
        return RK_FAILURE;
    }

    result =  RK_MPI_AI_Disable(stAiCfg.s32DevId);
    if (result != 0) {
        RK_LOGE("ai disable fail, reason = %d", result);
        return RK_FAILURE;
    }

    return RK_SUCCESS;
}
