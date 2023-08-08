/*
 * Copyright 2020 Rockchip Electronics Co. LTD
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

#include <stdio.h>
#include <errno.h>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/poll.h>
#include "rk_defines.h"
#include "rk_debug.h"

#include "rk_mpi_ai.h"
#include "rk_mpi_aenc.h"
#include "rk_mpi_sys.h"
#include "rk_mpi_mb.h"

#include "test_comm_argparse.h"

static RK_BOOL gAiExit = RK_FALSE;
#define TEST_AI_WITH_FD 0
#define AI_ALGO_FRAMES 256  // baed on 16kHz, it's  128 during 8kHz

#ifdef DBG_MOD_ID
#undef DBG_MOD_ID
#endif
#define DBG_MOD_ID DBG_MOD_COMB1(RK_ID_AI)


typedef enum rkTestAIMODE_E {
    TEST_AI_MODE_AI_ONLY = 0,
    TEST_AI_MODE_BIND_AENC = 1,
} TEST_AI_MODE_E;

typedef struct _rkTEST_AENC_CFG {
    AENC_CHN_ATTR_S pstChnAttr;
    FILE *fp;
} TEST_AENC_CFG;


typedef struct _rkMpiAICtx {
    const char *srcFilePath;
    const char *dstFilePath;
    RK_S32      s32LoopCount;
    RK_S32      s32ChnNum;
    RK_S32      s32DeviceSampleRate;
    RK_S32      s32SampleRate;
    RK_S32      s32DeviceChannel;
    RK_S32      s32Channel;
    RK_S32      s32BitWidth;
    RK_S32      s32DevId;
    RK_S32      s32FrameNumber;
    RK_S32      s32FrameLength;
    char       *chCardName;
    RK_S32      s32ChnIndex;
    RK_S32      s32SetVolume;
    RK_S32      s32SetMute;
    RK_S32      s32SetFadeRate;
    RK_S32      s32SetTrackMode;
    RK_S32      s32GetVolume;
    RK_S32      s32GetMute;
    RK_S32      s32GetTrackMode;
    RK_S32      s32LoopbackMode;
    RK_S32      s32DevFd;
    RK_S32      s32AedEnable;
    RK_S32      s32BcdEnable;
    RK_S32      s32BuzEnable;
    RK_S32      s32VqeGapMs;
    RK_S32      s32VqeEnable;
    RK_S32      s32DumpAlgo;
    const char *pVqeCfgPath;
    TEST_AI_MODE_E enMode;
    // for aenc
    const char *aencFilePath;
    TEST_AENC_CFG stAencCfg;
    char       *chCodecId;
} TEST_AI_CTX_S;

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

RK_S32 test_ai_poll_event(RK_S32 timeoutMsec, RK_S32 fd) {
    RK_S32 num_fds = 1;
    struct pollfd pollFds[num_fds];
    RK_S32 ret = 0;

    RK_ASSERT(fd > 0);
    memset(pollFds, 0, sizeof(pollFds));
    pollFds[0].fd = fd;
    pollFds[0].events = (POLLPRI | POLLIN | POLLERR | POLLNVAL | POLLHUP);

    ret = poll(pollFds, num_fds, timeoutMsec);

    if (ret > 0 && (pollFds[0].revents & (POLLERR | POLLNVAL | POLLHUP))) {
        RK_LOGE("fd:%d polled error", fd);
        return -1;
    }

    return ret;
}

RK_S32 test_open_device_ai(TEST_AI_CTX_S *ctx) {
    AUDIO_DEV aiDevId = ctx->s32DevId;
    AUDIO_SOUND_MODE_E soundMode;

    AIO_ATTR_S aiAttr;
    RK_S32 result;
    memset(&aiAttr, 0, sizeof(AIO_ATTR_S));

    if (ctx->chCardName) {
        snprintf(reinterpret_cast<char *>(aiAttr.u8CardName),
                 sizeof(aiAttr.u8CardName), "%s", ctx->chCardName);
    }

    aiAttr.soundCard.channels = ctx->s32DeviceChannel;
    aiAttr.soundCard.sampleRate = ctx->s32DeviceSampleRate;
    aiAttr.soundCard.bitWidth = AUDIO_BIT_WIDTH_16;

    AUDIO_BIT_WIDTH_E bitWidth = ai_find_bit_width(ctx->s32BitWidth);
    if (bitWidth == AUDIO_BIT_WIDTH_BUTT) {
        goto __FAILED;
    }
    aiAttr.enBitwidth = bitWidth;
    aiAttr.enSamplerate = (AUDIO_SAMPLE_RATE_E)ctx->s32SampleRate;
    soundMode = ai_find_sound_mode(ctx->s32Channel);
    if (soundMode == AUDIO_SOUND_MODE_BUTT) {
        goto __FAILED;
    }
    aiAttr.enSoundmode = soundMode;
    aiAttr.u32FrmNum = ctx->s32FrameNumber;
    aiAttr.u32PtNumPerFrm = ctx->s32FrameLength;

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

    return RK_SUCCESS;
__FAILED:
    return RK_FAILURE;
}

RK_S32 test_init_ai_aed(TEST_AI_CTX_S *params) {
    RK_S32 result;

    if (params->s32AedEnable) {
        AI_AED_CONFIG_S stAiAedConfig, stAiAedConfig2;

        stAiAedConfig.fSnrDB = 10.0f;
        stAiAedConfig.fLsdDB = -25.0f;
        stAiAedConfig.s32Policy = 1;

        result = RK_MPI_AI_SetAedAttr(params->s32DevId, params->s32ChnIndex, &stAiAedConfig);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: SetAedAttr(%d,%d) failed with %#x",
                __FUNCTION__, params->s32DevId, params->s32ChnIndex, result);
            return result;
        }

        result = RK_MPI_AI_GetAedAttr(params->s32DevId, params->s32ChnIndex, &stAiAedConfig2);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: SetAedAttr(%d,%d) failed with %#x",
                __FUNCTION__, params->s32DevId, params->s32ChnIndex, result);
            return result;
        }

        result = memcmp(&stAiAedConfig, &stAiAedConfig2, sizeof(AI_AED_CONFIG_S));
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: set/get aed config is different: %d", __FUNCTION__, result);
            return result;
        }

        result = RK_MPI_AI_EnableAed(params->s32DevId, params->s32ChnIndex);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: EnableAed(%d,%d) failed with %#x",
                __FUNCTION__, params->s32DevId, params->s32ChnIndex, result);
            return result;
        }
    }

    return RK_SUCCESS;
}

static RK_S32 test_init_ai_aed2(TEST_AI_CTX_S *params) {
    RK_S32 result;

    if (params->s32AedEnable) {
        AI_AED_CONFIG_S stAiAedConfig, stAiAedConfig2;

        stAiAedConfig.fSnrDB = 10.0f;
        stAiAedConfig.fLsdDB = -25.0f;
        stAiAedConfig.s32Policy = 2;

        result = RK_MPI_AI_SetAedAttr(params->s32DevId, params->s32ChnIndex, &stAiAedConfig);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: SetAedAttr(%d,%d) failed with %#x",
                __FUNCTION__, params->s32DevId, params->s32ChnIndex, result);
            return result;
        }

        result = RK_MPI_AI_GetAedAttr(params->s32DevId, params->s32ChnIndex, &stAiAedConfig2);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: SetAedAttr(%d,%d) failed with %#x",
                __FUNCTION__, params->s32DevId, params->s32ChnIndex, result);
            return result;
        }

        result = memcmp(&stAiAedConfig, &stAiAedConfig2, sizeof(AI_AED_CONFIG_S));
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: set/get aed config is different: %d", __FUNCTION__, result);
            return result;
        }

        result = RK_MPI_AI_EnableAed(params->s32DevId, params->s32ChnIndex);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: EnableAed(%d,%d) failed with %#x",
                __FUNCTION__, params->s32DevId, params->s32ChnIndex, result);
            return result;
        }
    }

    return RK_SUCCESS;
}

static RK_S32 test_init_ai_bcd(TEST_AI_CTX_S *params) {
    RK_S32 result;

    if (params->s32BcdEnable) {
        AI_BCD_CONFIG_S stAiBcdConfig, stAiBcdConfig2;

        stAiBcdConfig.mFrameLen = 120;
        stAiBcdConfig.mBlankFrameMax = 50;
        stAiBcdConfig.mCryEnergy = -1.25f;
        stAiBcdConfig.mJudgeEnergy = -0.75f;
        stAiBcdConfig.mCryThres1 = 0.70f;
        stAiBcdConfig.mCryThres2 = 0.55f;

        result = RK_MPI_AI_SetBcdAttr(params->s32DevId, params->s32ChnIndex, &stAiBcdConfig);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: SetBcdAttr(%d,%d) failed with %#x",
                __FUNCTION__, params->s32DevId, params->s32ChnIndex, result);
            return result;
        }

        result = RK_MPI_AI_GetBcdAttr(params->s32DevId, params->s32ChnIndex, &stAiBcdConfig2);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: SetBcdAttr(%d,%d) failed with %#x",
                __FUNCTION__, params->s32DevId, params->s32ChnIndex, result);
            return result;
        }

        result = memcmp(&stAiBcdConfig, &stAiBcdConfig2, sizeof(AI_BCD_CONFIG_S));
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: set/get aed config is different: %d", __FUNCTION__, result);
            return result;
        }

        result = RK_MPI_AI_EnableBcd(params->s32DevId, params->s32ChnIndex);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: EnableBcd(%d,%d) failed with %#x",
                __FUNCTION__, params->s32DevId, params->s32ChnIndex, result);
            return result;
        }
    }

    return RK_SUCCESS;
}

static RK_S32 test_init_ai_bcd2(TEST_AI_CTX_S *params) {
    RK_S32 result;

    if (params->s32BcdEnable) {
        AI_BCD_CONFIG_S stAiBcdConfig, stAiBcdConfig2;

        stAiBcdConfig.mFrameLen = 122;
        stAiBcdConfig.mBlankFrameMax = 222;
        stAiBcdConfig.mCryEnergy = -1.25f;
        stAiBcdConfig.mJudgeEnergy = -0.75f;
        stAiBcdConfig.mCryThres1 = 0.70f;
        stAiBcdConfig.mCryThres2 = 0.55f;

        result = RK_MPI_AI_SetBcdAttr(params->s32DevId, params->s32ChnIndex, &stAiBcdConfig);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: SetBcdAttr(%d,%d) failed with %#x",
                __FUNCTION__, params->s32DevId, params->s32ChnIndex, result);
            return result;
        }

        result = RK_MPI_AI_GetBcdAttr(params->s32DevId, params->s32ChnIndex, &stAiBcdConfig2);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: SetBcdAttr(%d,%d) failed with %#x",
                __FUNCTION__, params->s32DevId, params->s32ChnIndex, result);
            return result;
        }

        result = memcmp(&stAiBcdConfig, &stAiBcdConfig2, sizeof(AI_BCD_CONFIG_S));
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: set/get aed config is different: %d", __FUNCTION__, result);
            return result;
        }

        result = RK_MPI_AI_EnableBcd(params->s32DevId, params->s32ChnIndex);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: EnableBcd(%d,%d) failed with %#x",
                __FUNCTION__, params->s32DevId, params->s32ChnIndex, result);
            return result;
        }
    }

    return RK_SUCCESS;
}

static RK_S32 test_init_ai_buz(TEST_AI_CTX_S *params) {
    RK_S32 result;

    if (params->s32BuzEnable) {
        AI_BUZ_CONFIG_S stAiBuzConfig, stAiBuzConfig2;

        stAiBuzConfig.mFrameLen = 120;
        stAiBuzConfig.mBlankFrameMax = 120;
        stAiBuzConfig.mEnergyMean = -0.90f;
        stAiBuzConfig.mEnergyMax = -1.5f;
        stAiBuzConfig.mBuzThres1 = 0.75f;
        stAiBuzConfig.mBuzThres2 = 0.60f;

        result = RK_MPI_AI_SetBuzAttr(params->s32DevId, params->s32ChnIndex, &stAiBuzConfig);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: SetBuzAttr(%d,%d) failed with %#x",
                __FUNCTION__, params->s32DevId, params->s32ChnIndex, result);
            return result;
        }

        result = RK_MPI_AI_GetBuzAttr(params->s32DevId, params->s32ChnIndex, &stAiBuzConfig2);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: SetBuzAttr(%d,%d) failed with %#x",
                __FUNCTION__, params->s32DevId, params->s32ChnIndex, result);
            return result;
        }

        result = memcmp(&stAiBuzConfig, &stAiBuzConfig2, sizeof(AI_BUZ_CONFIG_S));
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: set/get aed config is different: %d", __FUNCTION__, result);
            return result;
        }

        result = RK_MPI_AI_EnableBuz(params->s32DevId, params->s32ChnIndex);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: EnableBuz(%d,%d) failed with %#x",
                __FUNCTION__, params->s32DevId, params->s32ChnIndex, result);
            return result;
        }
    }

    return RK_SUCCESS;
}

static RK_S32 test_init_ai_buz2(TEST_AI_CTX_S *params) {
    RK_S32 result;

    if (params->s32BuzEnable) {
        AI_BUZ_CONFIG_S stAiBuzConfig, stAiBuzConfig2;

        stAiBuzConfig.mFrameLen = 120;
        stAiBuzConfig.mBlankFrameMax = 120;
        stAiBuzConfig.mEnergyMean = -0.90f;
        stAiBuzConfig.mEnergyMax = -1.5f;
        stAiBuzConfig.mBuzThres1 = 0.75f;
        stAiBuzConfig.mBuzThres2 = 0.60f;

        result = RK_MPI_AI_SetBuzAttr(params->s32DevId, params->s32ChnIndex, &stAiBuzConfig);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: SetBuzAttr(%d,%d) failed with %#x",
                __FUNCTION__, params->s32DevId, params->s32ChnIndex, result);
            return result;
        }

        result = RK_MPI_AI_GetBuzAttr(params->s32DevId, params->s32ChnIndex, &stAiBuzConfig2);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: SetBuzAttr(%d,%d) failed with %#x",
                __FUNCTION__, params->s32DevId, params->s32ChnIndex, result);
            return result;
        }

        result = memcmp(&stAiBuzConfig, &stAiBuzConfig2, sizeof(AI_BUZ_CONFIG_S));
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: set/get aed config is different: %d", __FUNCTION__, result);
            return result;
        }

        result = RK_MPI_AI_EnableBuz(params->s32DevId, params->s32ChnIndex);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: EnableBuz(%d,%d) failed with %#x",
                __FUNCTION__, params->s32DevId, params->s32ChnIndex, result);
            return result;
        }
    }

    return RK_SUCCESS;
}

RK_S32 test_init_ai_vqe(TEST_AI_CTX_S *params) {
    AI_VQE_CONFIG_S stAiVqeConfig, stAiVqeConfig2;
    RK_S32 result;

    if (params->s32VqeEnable == 0)
        return RK_SUCCESS;

    // Need to config enCfgMode to VQE attr even the VQE is not enabled
    memset(&stAiVqeConfig, 0, sizeof(AI_VQE_CONFIG_S));
    if (params->pVqeCfgPath != RK_NULL) {
        stAiVqeConfig.enCfgMode = AIO_VQE_CONFIG_LOAD_FILE;
        memcpy(stAiVqeConfig.aCfgFile, params->pVqeCfgPath, strlen(params->pVqeCfgPath));
    }

    if (params->s32VqeGapMs != 16 && params->s32VqeGapMs != 10) {
        RK_LOGE("Invalid gap: %d, just supports 16ms or 10ms for AI VQE", params->s32VqeGapMs);
        return RK_FAILURE;
    }

    stAiVqeConfig.s32WorkSampleRate = params->s32SampleRate;
    stAiVqeConfig.s32FrameSample = params->s32SampleRate * params->s32VqeGapMs / 1000;
    result = RK_MPI_AI_SetVqeAttr(params->s32DevId, params->s32ChnIndex, 0, 0, &stAiVqeConfig);
    if (result != RK_SUCCESS) {
        RK_LOGE("%s: SetVqeAttr(%d,%d) failed with %#x",
            __FUNCTION__, params->s32DevId, params->s32ChnIndex, result);
        return result;
    }

    result = RK_MPI_AI_GetVqeAttr(params->s32DevId, params->s32ChnIndex, &stAiVqeConfig2);
    if (result != RK_SUCCESS) {
        RK_LOGE("%s: SetVqeAttr(%d,%d) failed with %#x",
            __FUNCTION__, params->s32DevId, params->s32ChnIndex, result);
        return result;
    }

    result = memcmp(&stAiVqeConfig, &stAiVqeConfig2, sizeof(AI_VQE_CONFIG_S));
    if (result != RK_SUCCESS) {
        RK_LOGE("%s: set/get vqe config is different: %d", __FUNCTION__, result);
        return result;
    }

    result = RK_MPI_AI_EnableVqe(params->s32DevId, params->s32ChnIndex);
    if (result != RK_SUCCESS) {
        RK_LOGE("%s: EnableVqe(%d,%d) failed with %#x",
            __FUNCTION__, params->s32DevId, params->s32ChnIndex, result);
        return result;
    }

    return RK_SUCCESS;
}

RK_S32 test_init_mpi_ai(TEST_AI_CTX_S *params) {
    RK_S32 result;

    result = test_init_ai_aed(params);
    if (result != 0) {
        RK_LOGE("ai aed init fail, reason = %x, aiChn = %d", result, params->s32ChnIndex);
        return RK_FAILURE;
    }

    result = test_init_ai_bcd(params);
    if (result != 0) {
        RK_LOGE("ai bcd init fail, reason = %x, aiChn = %d", result, params->s32ChnIndex);
        return RK_FAILURE;
    }

    result = test_init_ai_buz(params);
    if (result != 0) {
        RK_LOGE("ai buz init fail, reason = %x, aiChn = %d", result, params->s32ChnIndex);
        return RK_FAILURE;
    }

    result = test_init_ai_vqe(params);
    if (result != 0) {
        RK_LOGE("ai vqe init fail, reason = %x, aiChn = %d", result, params->s32ChnIndex);
        return RK_FAILURE;
    }

    result =  RK_MPI_AI_EnableChn(params->s32DevId, params->s32ChnIndex);
    if (result != 0) {
        RK_LOGE("ai enable channel fail, aiChn = %d, reason = %x", params->s32ChnIndex, result);
        return RK_FAILURE;
    }

#if TEST_AI_WITH_FD
    // open fd immediate after enable chn will be better.
    params->s32DevFd = RK_MPI_AI_GetFd(params->s32DevId, params->s32ChnIndex);
    RK_LOGI("ai (devId: %d, chnId: %d), selectFd:%d", params->s32DevId, params->s32ChnIndex, params->s32DevFd);
#endif

    RK_BOOL needResample = (params->s32DeviceSampleRate != params->s32SampleRate) ? RK_TRUE : RK_FALSE;

    if (needResample == RK_TRUE) {
        RK_LOGI("need to resample %d -> %d", params->s32DeviceSampleRate, params->s32SampleRate);
        result = RK_MPI_AI_EnableReSmp(params->s32DevId, params->s32ChnIndex,
                                      (AUDIO_SAMPLE_RATE_E)params->s32SampleRate);
        if (result != 0) {
            RK_LOGE("ai enable channel fail, reason = %x, aiChn = %d", result, params->s32ChnIndex);
            return RK_FAILURE;
        }
    }

    return RK_SUCCESS;
}

RK_S32 test_deinit_mpi_ai(TEST_AI_CTX_S *params) {
    RK_S32 result;

    RK_MPI_AI_DisableReSmp(params->s32DevId, params->s32ChnIndex);
    if (params->s32BuzEnable) {
        result = RK_MPI_AI_DisableBuz(params->s32DevId, params->s32ChnIndex);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: RK_MPI_AI_DisableBuz(%d,%d) failed with %#x",
                __FUNCTION__, params->s32DevId, params->s32ChnIndex, result);
            return result;
        }
        params->s32BuzEnable = 0;
    }

    if (params->s32BcdEnable) {
        result = RK_MPI_AI_DisableBcd(params->s32DevId, params->s32ChnIndex);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: RK_MPI_AI_DisableBcd(%d,%d) failed with %#x",
                __FUNCTION__, params->s32DevId, params->s32ChnIndex, result);
            return result;
        }
        params->s32BcdEnable = 0;
    }

    if (params->s32AedEnable) {
        result = RK_MPI_AI_DisableAed(params->s32DevId, params->s32ChnIndex);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: RK_MPI_AI_DisableAed(%d,%d) failed with %#x",
                __FUNCTION__, params->s32DevId, params->s32ChnIndex, result);
            return result;
        }
        params->s32AedEnable = 0;
    }

    if (params->s32VqeEnable) {
        result = RK_MPI_AI_DisableVqe(params->s32DevId, params->s32ChnIndex);
        if (result != RK_SUCCESS) {
            RK_LOGE("%s: RK_MPI_AI_DisableVqe(%d,%d) failed with %#x",
                __FUNCTION__, params->s32DevId, params->s32ChnIndex, result);
            return result;
        }
        params->s32VqeEnable = 0;
    }

    result = RK_MPI_AI_DisableChn(params->s32DevId, params->s32ChnIndex);
    if (result != 0) {
        RK_LOGE("ai disable channel fail, reason = %d", result);
        return RK_FAILURE;
    }

    result =  RK_MPI_AI_Disable(params->s32DevId);
    if (result != 0) {
        RK_LOGE("ai disable fail, reason = %d", result);
        return RK_FAILURE;
    }

    return RK_SUCCESS;
}

void* sendDataThread(void * ptr) {
    TEST_AI_CTX_S *params = reinterpret_cast<TEST_AI_CTX_S *>(ptr);

    RK_S32 result = 0;
    RK_S32 s32MilliSec = -1;
    AUDIO_FRAME_S frame;
    FILE *fp_aed, *fp_bcd, *fp_buz;
    RK_S16 *buf_aed, *buf_bcd, *buf_buz;
    RK_S32 s32AiAlgoFrames = 0;
    RK_U32 aed_count = 0, aed_flag = 0;
    RK_U32 bcd_count = 0, bcd_flag = 0;
    RK_U32 buz_count = 0, buz_flag = 0;

    if (params->dstFilePath) {
        AUDIO_SAVE_FILE_INFO_S save;
        save.bCfg = RK_TRUE;
        save.u32FileSize = 1024;
        snprintf(save.aFilePath, sizeof(save.aFilePath), "%s", params->dstFilePath);
        snprintf(save.aFileName, sizeof(save.aFileName), "%s", "cap_out.pcm");
        RK_MPI_AI_SaveFile(params->s32DevId, params->s32ChnIndex, &save);
    }

    if (params->s32SampleRate == 16000)
        s32AiAlgoFrames = AI_ALGO_FRAMES;
    else if (params->s32SampleRate == 8000)
        s32AiAlgoFrames = (AI_ALGO_FRAMES >> 1);

    /* Do not dump if s32AiAlgoFrames is invalid */
    if (s32AiAlgoFrames == 0)
        params->s32DumpAlgo = 0;

    if (params->s32DumpAlgo) {
        buf_aed = (RK_S16 *)calloc(s32AiAlgoFrames * 2 * sizeof(RK_S16), 1);
        RK_ASSERT(buf_aed != RK_NULL);
        fp_aed = fopen("/tmp/cap_aed_2ch.pcm", "wb");
        RK_ASSERT(fp_aed != RK_NULL);

        buf_bcd = (RK_S16 *)calloc(s32AiAlgoFrames * 1 * sizeof(RK_S16), 1);
        RK_ASSERT(buf_bcd != RK_NULL);
        fp_bcd = fopen("/tmp/cap_bcd_1ch.pcm", "wb");
        RK_ASSERT(fp_bcd != RK_NULL);

        buf_buz = (RK_S16 *)calloc(s32AiAlgoFrames * 1 * sizeof(RK_S16), 1);
        RK_ASSERT(buf_aed != RK_NULL);
        fp_buz = fopen("/tmp/cap_buz_1ch.pcm", "wb");
        RK_ASSERT(fp_buz != RK_NULL);
    }

    while (!gAiExit) {
#if TEST_AI_WITH_FD
        test_ai_poll_event(-1, params->s32DevFd);
#endif
        if (params->s32AedEnable == 2) {
            if ((aed_count + 1) % 50 == 0) {
                result = RK_MPI_AI_DisableAed(params->s32DevId, params->s32ChnIndex);
                if (result != RK_SUCCESS) {
                    RK_LOGE("%s: RK_MPI_AI_DisableAed(%d,%d) failed with %#x",
                        __FUNCTION__, params->s32DevId, params->s32ChnIndex, result);
                    return RK_NULL;
                }
                if (aed_flag) {
                    RK_LOGI("%s - aed_count=%ld test_init_ai_aed\n", __func__, aed_count);
                    test_init_ai_aed(params);
                    aed_flag = 0;
                } else {
                    RK_LOGI("%s - aed_count=%ld test_init_ai_aed2\n", __func__, aed_count);
                    test_init_ai_aed2(params);
                    aed_flag = 1;
                }
            }
            aed_count++;
        }

        if (params->s32BcdEnable == 2) {
            if ((bcd_count + 1) % 50 == 0) {
                result = RK_MPI_AI_DisableBcd(params->s32DevId, params->s32ChnIndex);
                if (result != RK_SUCCESS) {
                    RK_LOGE("%s: RK_MPI_AI_DisableBcd(%d,%d) failed with %#x",
                        __FUNCTION__, params->s32DevId, params->s32ChnIndex, result);
                    return RK_NULL;
                }
                if (bcd_flag) {
                    RK_LOGI("%s - bcd_count=%ld test_init_ai_bcd\n", __func__, bcd_count);
                    test_init_ai_bcd(params);
                    bcd_flag = 0;
                } else {
                    RK_LOGI("%s - bcd_count=%ld test_init_ai_bcd2\n", __func__, bcd_count);
                    test_init_ai_bcd2(params);
                    bcd_flag = 1;
                }
            }
            bcd_count++;
        }

        if (params->s32BuzEnable == 2) {
            if ((buz_count + 1) % 50 == 0) {
                result = RK_MPI_AI_DisableBuz(params->s32DevId, params->s32ChnIndex);
                if (result != RK_SUCCESS) {
                    RK_LOGE("%s: RK_MPI_AI_DisableBuz(%d,%d) failed with %#x",
                        __FUNCTION__, params->s32DevId, params->s32ChnIndex, result);
                    return RK_NULL;
                }
                if (buz_flag) {
                    RK_LOGI("%s - buz_count=%ld test_init_ai_buz\n", __func__, buz_count);
                    test_init_ai_buz(params);
                    buz_flag = 0;
                } else {
                    RK_LOGI("%s - buz_count=%ld test_init_ai_buz2\n", __func__, buz_count);
                    test_init_ai_buz2(params);
                    buz_flag = 1;
                }
            }
            buz_count++;
        }

        result = RK_MPI_AI_GetFrame(params->s32DevId, params->s32ChnIndex, &frame, RK_NULL, s32MilliSec);
        if (result == 0) {
            void* data = RK_MPI_MB_Handle2VirAddr(frame.pMbBlk);
            RK_U32 len = RK_MPI_MB_GetSize(frame.pMbBlk);
            RK_LOGV("data = %p, len = %d", data, len);
            RK_MPI_AI_ReleaseFrame(params->s32DevId, params->s32ChnIndex, &frame, RK_NULL);
        }

        // dump results of SED(AED/BCD) modules
        if (params->s32AedEnable) {
            AI_AED_RESULT_S aed_result;

            memset(&aed_result, 0, sizeof(aed_result));
            result = RK_MPI_AI_GetAedResult(params->s32DevId, params->s32ChnIndex, &aed_result);
            if (result == 0) {
                RK_LOGI("AED Result: AcousticEvent:%d, LoudSound:%d",
                        aed_result.bAcousticEventDetected,
                        aed_result.bLoudSoundDetected);
            }
            if (params->s32DumpAlgo) {
                for (RK_S32 i = 0; i < s32AiAlgoFrames; i++) {
                    *(buf_aed + 2 * i + 0) = 10000 * aed_result.bAcousticEventDetected;
                    *(buf_aed + 2 * i + 1) = 10000 * aed_result.bLoudSoundDetected;
                }
                fwrite(buf_aed, s32AiAlgoFrames * 2 * sizeof(RK_S16), 1, fp_aed);
            }
        }

        if (params->s32BcdEnable) {
            AI_BCD_RESULT_S bcd_result;

            memset(&bcd_result, 0, sizeof(bcd_result));
            result = RK_MPI_AI_GetBcdResult(params->s32DevId, params->s32ChnIndex, &bcd_result);
            if (result == 0) {
                RK_LOGI("BCD Result: BabyCry:%d", bcd_result.bBabyCry);
            }
            if (params->s32DumpAlgo) {
                for (RK_S32 i = 0; i < s32AiAlgoFrames; i++) {
                    *(buf_bcd + 1 * i) = 10000 * bcd_result.bBabyCry;
                }
                fwrite(buf_bcd, s32AiAlgoFrames * 1 * sizeof(RK_S16), 1, fp_bcd);
            }
        }

        if (params->s32BuzEnable) {
            AI_BUZ_RESULT_S buz_result;

            memset(&buz_result, 0, sizeof(buz_result));
            result = RK_MPI_AI_GetBuzResult(params->s32DevId, params->s32ChnIndex, &buz_result);
            if (result == 0) {
                RK_LOGI("BUZ Result: Buzz:%d", buz_result.bBuzz);
            }
            if (params->s32DumpAlgo) {
                for (RK_S32 i = 0; i < s32AiAlgoFrames; i++) {
                    *(buf_buz + 1 * i) = 10000 * buz_result.bBuzz;
                }
                fwrite(buf_buz, s32AiAlgoFrames * 1 * sizeof(RK_S16), 1, fp_buz);
            }
        }
    }

    if (params->s32DumpAlgo) {
        if (buf_aed)
            free(buf_aed);
        if (buf_bcd)
            free(buf_bcd);
        if (buf_buz)
            free(buf_buz);

        if (fp_aed)
            fclose(fp_aed);
        if (fp_bcd)
            fclose(fp_bcd);
        if (fp_buz)
            fclose(fp_buz);
    }

    pthread_exit(NULL);
    return RK_NULL;
}

void* commandThread(void * ptr) {
    TEST_AI_CTX_S *params = reinterpret_cast<TEST_AI_CTX_S *>(ptr);

    AUDIO_FADE_S aFade;
    aFade.bFade = RK_FALSE;
    aFade.enFadeOutRate = (AUDIO_FADE_RATE_E)params->s32SetFadeRate;
    aFade.enFadeInRate = (AUDIO_FADE_RATE_E)params->s32SetFadeRate;
    RK_BOOL mute = (params->s32SetMute == 0) ? RK_FALSE : RK_TRUE;
    RK_LOGI("test info : mute = %d, volume = %d", mute, params->s32SetVolume);
    RK_MPI_AI_SetMute(params->s32DevId, mute, &aFade);
    RK_MPI_AI_SetVolume(params->s32DevId, params->s32SetVolume);

    if (params->s32SetTrackMode) {
        RK_LOGI("test info : set track mode = %d", params->s32SetTrackMode);
        RK_MPI_AI_SetTrackMode(params->s32DevId, (AUDIO_TRACK_MODE_E)params->s32SetTrackMode);
        params->s32SetTrackMode = 0;
    }

    if (params->s32GetVolume) {
        RK_S32 volume = 0;
        RK_MPI_AI_GetVolume(params->s32DevId, &volume);
        RK_LOGI("test info : get volume = %d", volume);
        params->s32GetVolume = 0;
    }

    if (params->s32GetMute) {
        RK_BOOL mute = RK_FALSE;
        AUDIO_FADE_S fade;
        RK_MPI_AI_GetMute(params->s32DevId, &mute, &fade);
        RK_LOGI("test info : is mute = %d", mute);
        params->s32GetMute = 0;
    }

    if (params->s32GetTrackMode) {
        AUDIO_TRACK_MODE_E trackMode;
        RK_MPI_AI_GetTrackMode(params->s32DevId, &trackMode);
        RK_LOGI("test info : get track mode = %d", trackMode);
        params->s32GetTrackMode = 0;
    }

    pthread_exit(NULL);
    return RK_NULL;
}

static RK_S32 test_set_channel_params_ai(TEST_AI_CTX_S *params) {
    AUDIO_DEV aiDevId = params->s32DevId;
    AI_CHN aiChn = params->s32ChnIndex;
    RK_S32 result = 0;
    AI_CHN_PARAM_S pstParams;

    memset(&pstParams, 0, sizeof(AI_CHN_PARAM_S));
    pstParams.enLoopbackMode = (AUDIO_LOOPBACK_MODE_E)params->s32LoopbackMode;
    pstParams.s32UsrFrmDepth = -1;
    result = RK_MPI_AI_SetChnParam(aiDevId, aiChn, &pstParams);
    if (result != RK_SUCCESS) {
        RK_LOGE("ai set channel params, aiChn = %d", aiChn);
        return RK_FAILURE;
    }

    return RK_SUCCESS;
}

RK_S32 unit_test_mpi_ai(TEST_AI_CTX_S *ctx) {
    RK_S32 i = 0;
    TEST_AI_CTX_S params[AI_MAX_CHN_NUM];
    pthread_t tidSend[AI_MAX_CHN_NUM];
    pthread_t tidComand[AI_MAX_CHN_NUM];
    RK_S32 result = RK_SUCCESS;

    if (test_open_device_ai(ctx) != RK_SUCCESS) {
        goto __FAILED;
    }

    for (i = 0; i < ctx->s32ChnNum; i++) {
        memcpy(&(params[i]), ctx, sizeof(TEST_AI_CTX_S));
        params[i].s32ChnIndex = i;
        params[i].s32DevFd = -1;
        result = test_set_channel_params_ai(&params[i]);
        if (result != RK_SUCCESS)
            goto __FAILED;
        result = test_init_mpi_ai(&params[i]);
        if (result != RK_SUCCESS)
            goto __FAILED;

        pthread_create(&tidSend[i], RK_NULL, sendDataThread, reinterpret_cast<void *>(&params[i]));
        pthread_create(&tidComand[i], RK_NULL, commandThread, reinterpret_cast<void *>(&params[i]));
    }

    for (i = 0; i < ctx->s32ChnNum; i++) {
        pthread_join(tidComand[i], RK_NULL);
        pthread_join(tidSend[i], RK_NULL);
        result = test_deinit_mpi_ai(&params[i]);
        if (result != RK_SUCCESS)
            goto __FAILED;
    }

    return RK_SUCCESS;
__FAILED:

    return RK_FAILURE;
}
static RK_U32 test_find_audio_enc_codec_id(TEST_AI_CTX_S *params) {
    if (strstr(params->chCodecId, "g726")) {
        return RK_AUDIO_ID_ADPCM_G726;
    } else if (strstr(params->chCodecId, "g711a")) {
        return RK_AUDIO_ID_PCM_ALAW;
    } else if (strstr(params->chCodecId, "g711u")) {
        return RK_AUDIO_ID_PCM_MULAW;
    }
}

static RK_U32 test_find_audio_enc_format(TEST_AI_CTX_S *params) {
    if (params->s32BitWidth == 8) {
        return AUDIO_BIT_WIDTH_8;
    } else if (params->s32BitWidth == 16) {
        return AUDIO_BIT_WIDTH_16;
    } else if (params->s32BitWidth == 24) {
        return AUDIO_BIT_WIDTH_24;
    }

    RK_LOGE("test not find format : %d", params->s32BitWidth);
    return -1;
}

void init_aenc_cfg(TEST_AI_CTX_S *ctx, RK_CODEC_ID_E  enCodecId, AUDIO_BIT_WIDTH_E enBitwidth) {
    RK_S32 s32ret = 0;

    ctx->stAencCfg.pstChnAttr.stCodecAttr.enType		= enCodecId;
    ctx->stAencCfg.pstChnAttr.stCodecAttr.u32Channels	= ctx->s32Channel;
    ctx->stAencCfg.pstChnAttr.stCodecAttr.u32SampleRate	= ctx->s32SampleRate;
    ctx->stAencCfg.pstChnAttr.stCodecAttr.enBitwidth	= enBitwidth;
    ctx->stAencCfg.pstChnAttr.stCodecAttr.pstResv		= RK_NULL;

    ctx->stAencCfg.pstChnAttr.enType		= enCodecId;
    ctx->stAencCfg.pstChnAttr.u32BufCount	= 4;

    s32ret = RK_MPI_AENC_CreateChn(0, &ctx->stAencCfg.pstChnAttr);
    if (s32ret) {
        RK_LOGE("create aenc chn %d err:0x%x\n", enCodecId, s32ret);
    }

}

RK_S32 test_init_mpi_aenc(TEST_AI_CTX_S *params)
{
    RK_S32 s32ret = 0;

    RK_U32 codecId = test_find_audio_enc_codec_id(params);
    if (codecId == -1) {
        return RK_FAILURE;
    }
    printf("codecId=%d\n",codecId);
    RK_U32 format = test_find_audio_enc_format(params);
    if (codecId == -1) {
        return RK_FAILURE;
    }
    printf("format=%d\n",format);

    init_aenc_cfg(params, (RK_CODEC_ID_E)codecId, (AUDIO_BIT_WIDTH_E)format);

    return RK_SUCCESS;
}



static void *GetMediaBuffer(void *arg) {
    RK_S32 s32MilliSec = -1;
    AUDIO_FRAME_S frame;
    RK_S32 s32DevId = 0;
    RK_S32 s32ChnIndex = 0;
    RK_S32 result = 0;
    FILE *file;

    file = fopen("/tmp/cap_out.pcm","wb");
    while (!gAiExit) {
        result = RK_MPI_AI_GetFrame(s32DevId, s32ChnIndex, &frame, RK_NULL, s32MilliSec);
        if (result == 0) {
            void* data = RK_MPI_MB_Handle2VirAddr(frame.pMbBlk);
            RK_U32 len = RK_MPI_MB_GetSize(frame.pMbBlk);
            RK_LOGI("data = %p, len = %d", data, len);
            if (file) {
                fwrite(data, len, 1, file);
                fflush(file);
            }
            RK_MPI_AI_ReleaseFrame(s32DevId, s32ChnIndex, &frame, RK_NULL);
        }
    }

    fclose(file);

}

static RK_S32 test_ai_bind_aenc_loop(TEST_AI_CTX_S *ctx) {

    printf("===test_ai_bind_aenc_loop===\n");
    RK_S32 s32ret = 0;
    AUDIO_STREAM_S pstStream;
    AENC_CHN AdChn = 0;
    RK_S32 eos = 0;
    RK_S32 count = 0;


    if (test_open_device_ai(ctx) != RK_SUCCESS) {
          goto __FAILED;
    }

    ctx->s32ChnIndex = 0;
    ctx->s32DevFd = -1;
    s32ret = test_set_channel_params_ai(ctx);
    if (s32ret != RK_SUCCESS)
        goto __FAILED;
    s32ret = test_init_mpi_ai(ctx);
    if (s32ret != RK_SUCCESS)
            goto __FAILED;

    ctx->stAencCfg.fp = fopen(ctx->aencFilePath, "wb");
    if (ctx->stAencCfg.fp == RK_NULL) {
        RK_LOGE("chn 0 can't open file %s in get picture thread!\n",ctx->aencFilePath);
        return RK_FAILURE;
    }

    //init_aenc_cfg(ctx, 0, (RK_CODEC_ID_E)ctx->u32DstCodec);


    if (test_init_mpi_aenc(ctx) == RK_FAILURE) {
            goto __FAILED;
    }

    //ai bind aenc
    MPP_CHN_S stSrcChn, stDestChn;
    stSrcChn.enModId    = RK_ID_AI;
    stSrcChn.s32DevId   = ctx->s32DevId;
    stSrcChn.s32ChnId   = ctx->s32ChnIndex;

    stDestChn.enModId   = RK_ID_AENC;
    stDestChn.s32DevId  = 0;
    stDestChn.s32ChnId  = 0;

    s32ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (s32ret != RK_SUCCESS) {
        RK_LOGE("===bind ai to aenc failed===\n");
        return RK_FAILURE;
    }

    //Save aenc data while saving ai data
    //default don't save
    pthread_t read_thread;
    pthread_create(&read_thread, NULL, GetMediaBuffer, NULL);


    while(!gAiExit){
        s32ret = RK_MPI_AENC_GetStream(AdChn, &pstStream, -1);
        if (s32ret == RK_SUCCESS) {
            MB_BLK bBlk = pstStream.pMbBlk;
            RK_VOID *pstFrame = RK_MPI_MB_Handle2VirAddr(bBlk);
            RK_S32 frameSize = pstStream.u32Len;
            RK_S32 timeStamp = pstStream.u64TimeStamp;
            eos = (frameSize <= 0) ? 1 : 0;
            if (pstFrame) {
                RK_LOGI("get frame data = %p, size = %d, timeStamp = %lld",
                                                  pstFrame, frameSize, timeStamp);
                if (ctx->stAencCfg.fp) {
                    fwrite(pstFrame, frameSize, 1, ctx->stAencCfg.fp);
                    fflush(ctx->stAencCfg.fp);
                }
                RK_MPI_AENC_ReleaseStream(AdChn, &pstStream);
                count++;
            }
        } else {
            RK_LOGE("fail to get aenc frame.");
        }

        if (eos) {
            RK_LOGI("get eos stream.");
            break;
        }

    }

__FAILED:
    if (ctx->stAencCfg.fp) {
        fclose(ctx->stAencCfg.fp);
        ctx->stAencCfg.fp = RK_NULL;
    }

    s32ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (s32ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_SYS_UnBind fail %x", s32ret);
    }

    s32ret = test_deinit_mpi_ai(ctx);
    RK_LOGE("test_deinit_mpi_ai %x", s32ret);

    s32ret = RK_MPI_AENC_DestroyChn(0);
    if (s32ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VDEC_DestroyChn fail %x", s32ret);
    }

    return RK_NULL;

}

static void mpi_ai_test_show_options(const TEST_AI_CTX_S *ctx) {
    RK_PRINT("cmd parse result:\n");
    RK_PRINT("input  file name      : %s\n", ctx->srcFilePath);
    RK_PRINT("output file name      : %s\n", ctx->dstFilePath);
    RK_PRINT("loop count            : %d\n", ctx->s32LoopCount);
    RK_PRINT("channel number        : %d\n", ctx->s32ChnNum);
    RK_PRINT("open sound rate       : %d\n", ctx->s32DeviceSampleRate);
    RK_PRINT("record data rate      : %d\n", ctx->s32SampleRate);
    RK_PRINT("sound card channel    : %d\n", ctx->s32DeviceChannel);
    RK_PRINT("output channel        : %d\n", ctx->s32Channel);
    RK_PRINT("bit_width             : %d\n", ctx->s32BitWidth);
    RK_PRINT("frame_number          : %d\n", ctx->s32FrameNumber);
    RK_PRINT("frame_length           : %d\n", ctx->s32FrameLength);
    RK_PRINT("sound card name       : %s\n", ctx->chCardName);
    RK_PRINT("device id             : %d\n", ctx->s32DevId);
    RK_PRINT("set volume            : %d\n", ctx->s32SetVolume);
    RK_PRINT("set mute              : %d\n", ctx->s32SetMute);
    RK_PRINT("set track_mode        : %d\n", ctx->s32SetTrackMode);
    RK_PRINT("get volume            : %d\n", ctx->s32GetVolume);
    RK_PRINT("get mute              : %d\n", ctx->s32GetMute);
    RK_PRINT("get track_mode        : %d\n", ctx->s32GetTrackMode);
    RK_PRINT("aed enable            : %d\n", ctx->s32AedEnable);
    RK_PRINT("bcd enable            : %d\n", ctx->s32BcdEnable);
    RK_PRINT("buz enable            : %d\n", ctx->s32BuzEnable);
    RK_PRINT("vqe gap duration (ms) : %d\n", ctx->s32VqeGapMs);
    RK_PRINT("vqe enable            : %d\n", ctx->s32VqeEnable);
    RK_PRINT("vqe config file       : %s\n", ctx->pVqeCfgPath);
    RK_PRINT("dump algo pcm data    : %d\n", ctx->s32DumpAlgo);
    RK_PRINT("test mode             : %d\n", ctx->enMode);
    RK_PRINT("aenc  file name       : %s\n", ctx->aencFilePath);
    RK_PRINT("input codec name      : %s\n", ctx->chCodecId);
}

static const char *const usages[] = {
    "./rk_mpi_ai_test [--device_rate rate] [--device_ch ch] [--out_rate rate] [--out_ch ch] "
                     "[--aed_enable] [--bcd_enable] [--buz_enable] [--vqe_enable] [--vqe_cfg]...",
    NULL,
};

static void sigterm_handler(int sig) {
    RK_PRINT("Catched SIGINT %d\n", sig);
    gAiExit = RK_TRUE;
}

int main(int argc, const char **argv) {
    RK_S32          i;
    RK_S32          s32Ret;
    TEST_AI_CTX_S  *ctx;

    ctx = reinterpret_cast<TEST_AI_CTX_S *>(malloc(sizeof(TEST_AI_CTX_S)));
    memset(ctx, 0, sizeof(TEST_AI_CTX_S));

    ctx->srcFilePath        = RK_NULL;
    ctx->dstFilePath        = RK_NULL;
    ctx->s32LoopCount       = 1;
    ctx->s32ChnNum          = 1;
    ctx->s32BitWidth        = 16;
    ctx->s32FrameNumber     = 4;
    ctx->s32FrameLength      = 1024;
    ctx->chCardName         = RK_NULL;
    ctx->s32DevId           = 0;
    ctx->s32SetVolume       = 100;
    ctx->s32SetMute         = 0;
    ctx->s32SetTrackMode    = 0;
    ctx->s32SetFadeRate     = 0;
    ctx->s32GetVolume       = 0;
    ctx->s32GetMute         = 0;
    ctx->s32GetTrackMode    = 0;
    ctx->s32AedEnable       = 0;
    ctx->s32BcdEnable       = 0;
    ctx->s32BuzEnable       = 0;
    ctx->s32VqeGapMs        = 16;
    ctx->s32VqeEnable       = 0;
    ctx->pVqeCfgPath        = RK_NULL;
    ctx->s32LoopbackMode    = AUDIO_LOOPBACK_NONE;
    ctx->s32DumpAlgo        = 0;

    ctx->enMode             = TEST_AI_MODE_AI_ONLY;
    ctx->aencFilePath       = RK_NULL;

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("basic options:"),

        OPT_INTEGER('\0', "device_rate", &(ctx->s32DeviceSampleRate),
                    "the sample rate of open sound card.  <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "device_ch", &(ctx->s32DeviceChannel),
                    "the number of sound card channels. <required>.", NULL, 0, 0),
        OPT_INTEGER('\0', "out_ch", &(ctx->s32Channel),
                    "the channels of out data. <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "out_rate", &(ctx->s32SampleRate),
                    "the sample rate of out data. <required>", NULL, 0, 0),
        OPT_STRING('o', "output", &(ctx->dstFilePath),
                    "output file name, e.g.(./ai). default(NULL).", NULL, 0, 0),
        OPT_INTEGER('n', "loop_count", &(ctx->s32LoopCount),
                    "loop running count. can be any count. default(1)", NULL, 0, 0),
        OPT_INTEGER('c', "channel_count", &(ctx->s32ChnNum),
                    "the count of adec channel. default(1).", NULL, 0, 0),
        OPT_INTEGER('\0', "bit", &(ctx->s32BitWidth),
                    "the bit width of open sound card, range(8, 16, 24), default(16)", NULL, 0, 0),
        OPT_INTEGER('\0', "frame_length", &(ctx->s32FrameLength),
                    "the frame length for open sound card, default(1024)", NULL, 0, 0),
        OPT_INTEGER('\0', "frame_number", &(ctx->s32FrameNumber),
                    "the frame number for open sound card, default(4)", NULL, 0, 0),
        OPT_STRING('\0', "sound_card_name", &(ctx->chCardName),
                    "the sound name for open sound card, default(NULL)", NULL, 0, 0),
        OPT_INTEGER('\0', "set_volume", &(ctx->s32SetVolume),
                    "set volume test, range(0, 100), default(100)", NULL, 0, 0),
        OPT_INTEGER('\0', "set_mute", &(ctx->s32SetMute),
                    "set mute test, range(0, 1), default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "set_fade", &(ctx->s32SetFadeRate),
                    "set fade rate, range(0, 7), default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "set_track_mode", &(ctx->s32SetTrackMode),
                    "set track mode test, range(0:normal, 1:both_left, 2:both_right, 3:exchange, 4:mix,"
                    "5:left_mute, 6:right_mute, 7:both_mute), default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "get_volume", &(ctx->s32GetVolume),
                    "get volume test, range(0, 1), default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "get_mute", &(ctx->s32GetMute),
                    "get mute test, range(0, 1), default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "get_track_mode", &(ctx->s32GetTrackMode),
                    "get track mode test, range(0, 1), default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "aed_enable", &(ctx->s32AedEnable),
                    "the aed enable, 0:disable 1:enable 2:reload test. default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "bcd_enable", &(ctx->s32BcdEnable),
                    "the bcd enable, 0:disable 1:enable. default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "buz_enable", &(ctx->s32BuzEnable),
                    "the buz enable, 0:disable 1:enable. default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "vqe_gap", &(ctx->s32VqeGapMs),
                    "the vqe gap duration in milliseconds. only supports 16ms or 10ms. default(16).", NULL, 0, 0),
        OPT_INTEGER('\0', "vqe_enable", &(ctx->s32VqeEnable),
                    "the vqe enable, 0:disable 1:enable. default(0).", NULL, 0, 0),
        OPT_STRING('\0', "vqe_cfg", &(ctx->pVqeCfgPath),
                    "the vqe config file, default(NULL)", NULL, 0, 0),
        OPT_INTEGER('\0', "loopback_mode", &(ctx->s32LoopbackMode),
                    "configure the loopback mode during ai runtime", NULL, 0, 0),
        OPT_INTEGER('\0', "dump_algo", &(ctx->s32DumpAlgo),
                    "dump algorithm pcm data during ai runtime", NULL, 0, 0),
        OPT_INTEGER('m', "mode", &(ctx->enMode),
                    "test mode(default 0; \n\t"
                    "0:ai get&release frame \n\t"
                    "1:ai bind one aenc(g711a) \n\t", NULL, 0, 0),
        OPT_STRING('\0', "aenc_file", &(ctx->aencFilePath),
                    "aenc file name, e.g.(./userdata/aenc/aenc.g711a). default(NULL).", NULL, 0, 0),
        OPT_STRING('C', "codec", &(ctx->chCodecId),
                    "codec, e.g.(g711a/g711u/g726). ", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nselect a test case to run.",
                                 "\nuse --help for details.");

    argc = argparse_parse(&argparse, argc, argv);
    mpi_ai_test_show_options(ctx);

    if (ctx->s32Channel <= 0
        || ctx->s32SampleRate <= 0
        || ctx->s32DeviceSampleRate <= 0
        || ctx->s32DeviceChannel <= 0) {
        argparse_usage(&argparse);
        goto __FAILED;
    }

    // setup the SIGINT to ctrl+c and handing de-init process
    signal(SIGINT, sigterm_handler);

    RK_MPI_SYS_Init();

    switch (ctx->enMode) {
    case TEST_AI_MODE_AI_ONLY:
        for (i = 0; i < ctx->s32LoopCount; i++) {
            RK_LOGI("start running loop count  = %d", i);
            s32Ret = unit_test_mpi_ai(ctx);
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("unit_test_mpi_ai failed: %d", s32Ret);
                goto __FAILED;
            }
            RK_LOGI("end running loop count  = %d", i);
        }
    break;
    case TEST_AI_MODE_BIND_AENC:
        /* command:rk_mpi_ai_test --device_rate 16000 --device_ch 2 --out_rate 16000
        --out_ch 2 --sound_card_name hw:0,0 -m 1 --aenc_file aenc.g711a -C g711a */
        s32Ret = test_ai_bind_aenc_loop(ctx);
    break;
    default:
        RK_LOGE("no support such test mode:%d", ctx->enMode);
    break;
    }

__FAILED:
    if (ctx) {
        free(ctx);
        ctx = RK_NULL;
    }

    RK_MPI_SYS_Exit();
    return 0;
}
