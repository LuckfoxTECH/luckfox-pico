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
#include "rk_defines.h"
#include "rk_debug.h"
#include "rk_mpi_adec.h"
#include "rk_mpi_ao.h"
#include "rk_mpi_mb.h"
#include "rk_mpi_sys.h"

#include "test_comm_argparse.h"

#define USE_AO_MIXER 0

typedef enum rkTestAOMODE_E {
    TEST_AO_MODE_AO_ONLY = 0,
    TEST_AO_MODE_BIND_ADEC = 1,
} TEST_AO_MODE_E;

typedef struct _rkMpiAOCtx {
    const char *srcFilePath;
    const char *dstFilePath;
    RK_S32      s32LoopCount;
    RK_S32      s32ChnNum;
    RK_S32      s32SampleRate;
    RK_S32      s32ReSmpSampleRate;
    RK_S32      s32Channel;
    RK_S32      s32DeviceChannel;
    RK_S32      s32BitWidth;
    RK_S32      s32DevId;
    RK_S32      s32PeriodCount;
    RK_S32      s32PeriodSize;
    char       *chCardName;
    RK_S32      s32ChnIndex;
    RK_S32      s32SetVolume;
    RK_S32      s32SetMute;
    RK_S32      s32SetFadeRate;
    RK_S32      s32SetTrackMode;
    RK_S32      s32GetVolume;
    RK_S32      s32GetMute;
    RK_S32      s32GetTrackMode;
    RK_S32      s32QueryChnStat;
    RK_S32      s32PauseResumeChn;
    RK_S32      s32SaveFile;
    RK_S32      s32QueryFileStat;
    RK_S32      s32ClrChnBuf;
    RK_S32      s32ClrPubAttr;
    RK_S32      s32GetPubAttr;
    RK_S32      s32LoopbackMode;
    TEST_AO_MODE_E enMode;
    const char *adecFilePath;
    ADEC_CHN_ATTR_S pstChnAttr;
} TEST_AO_CTX_S;

void query_ao_flow_graph_stat(AUDIO_DEV aoDevId, AO_CHN aoChn) {
    RK_S32 ret = 0;
    AO_CHN_STATE_S pstStat;
    memset(&pstStat, 0, sizeof(AO_CHN_STATE_S));
    ret = RK_MPI_AO_QueryChnStat(aoDevId, aoChn, &pstStat);
    if (ret == RK_SUCCESS) {
        RK_LOGI("query ao flow status:");
        RK_LOGI("total number of channel buffer : %d", pstStat.u32ChnTotalNum);
        RK_LOGI("free number of channel buffer : %d", pstStat.u32ChnFreeNum);
        RK_LOGI("busy number of channel buffer : %d", pstStat.u32ChnBusyNum);
    }
}

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

RK_S32 test_open_device_ao(TEST_AO_CTX_S *ctx) {
    AUDIO_DEV aoDevId = ctx->s32DevId;
    AUDIO_SOUND_MODE_E soundMode;

    AIO_ATTR_S aoAttr;
    memset(&aoAttr, 0, sizeof(AIO_ATTR_S));

    if (ctx->chCardName) {
        snprintf(reinterpret_cast<char *>(aoAttr.u8CardName),
                 sizeof(aoAttr.u8CardName), "%s", ctx->chCardName);
    }

    aoAttr.soundCard.channels = ctx->s32DeviceChannel;
    aoAttr.soundCard.sampleRate = ctx->s32SampleRate;
    aoAttr.soundCard.bitWidth = AUDIO_BIT_WIDTH_16;

    AUDIO_BIT_WIDTH_E bitWidth = find_bit_width(ctx->s32BitWidth);
    if (bitWidth == AUDIO_BIT_WIDTH_BUTT) {
        goto __FAILED;
    }
    aoAttr.enBitwidth = bitWidth;
    aoAttr.enSamplerate = (AUDIO_SAMPLE_RATE_E)ctx->s32ReSmpSampleRate;
    soundMode = find_sound_mode(ctx->s32Channel);
    if (soundMode == AUDIO_SOUND_MODE_BUTT) {
        goto __FAILED;
    }
    aoAttr.enSoundmode = soundMode;
    aoAttr.u32FrmNum = ctx->s32PeriodCount;
    aoAttr.u32PtNumPerFrm = ctx->s32PeriodSize;

    aoAttr.u32EXFlag = 0;
    aoAttr.u32ChnCnt = 2;

    RK_MPI_AO_SetPubAttr(aoDevId, &aoAttr);

    RK_MPI_AO_Enable(aoDevId);

    return RK_SUCCESS;
__FAILED:
    return RK_FAILURE;
}

RK_S32 test_init_mpi_ao(TEST_AO_CTX_S *params) {
    RK_S32 result;

    result =  RK_MPI_AO_EnableChn(params->s32DevId, params->s32ChnIndex);
    if (result != 0) {
        RK_LOGE("ao enable channel fail, aoChn = %d, reason = %x", params->s32ChnIndex, result);
        return RK_FAILURE;
    }

    // set sample rate of input data
    result = RK_MPI_AO_EnableReSmp(params->s32DevId, params->s32ChnIndex,
                                  (AUDIO_SAMPLE_RATE_E)params->s32ReSmpSampleRate);
    if (result != 0) {
        RK_LOGE("ao enable channel fail, reason = %x, aoChn = %d", result, params->s32ChnIndex);
        return RK_FAILURE;
    }

    return RK_SUCCESS;
}

RK_S32 deinit_mpi_ao(AUDIO_DEV aoDevId, AO_CHN aoChn) {
    RK_S32 result = RK_MPI_AO_DisableReSmp(aoDevId, aoChn);
    if (result != 0) {
        RK_LOGE("ao disable resample fail, reason = %d", result);
        return RK_FAILURE;
    }

    result = RK_MPI_AO_DisableChn(aoDevId, aoChn);
    if (result != 0) {
        RK_LOGE("ao disable channel fail, reason = %d", result);
        return RK_FAILURE;
    }

    return RK_SUCCESS;
}

RK_S32 test_close_device_ao(TEST_AO_CTX_S *ctx) {
    AUDIO_DEV aoDevId = ctx->s32DevId;
    RK_S32 result =  RK_MPI_AO_Disable(aoDevId);
    if (result != 0) {
        RK_LOGE("ao disable fail, reason = %d", result);
        return RK_FAILURE;
    }
    return RK_SUCCESS;
}

static RK_S32 test_set_channel_params_ao(TEST_AO_CTX_S *params) {
    AUDIO_DEV aoDevId = params->s32DevId;
    AO_CHN aoChn = params->s32ChnIndex;
    RK_S32 result = 0;
    AO_CHN_PARAM_S pstParams;
    memset(&pstParams, 0, sizeof(AO_CHN_PARAM_S));
#if USE_AO_MIXER
    // for test : aoChn0 output left channel,  aoChn1 output right channel,
    if (aoChn == 0) {
        pstParams.enMode = AUDIO_CHN_MODE_LEFT;
    } else if (aoChn == 1) {
        pstParams.enMode = AUDIO_CHN_MODE_RIGHT;
    }
#endif
    pstParams.enLoopbackMode = (AUDIO_LOOPBACK_MODE_E)params->s32LoopbackMode;
    result = RK_MPI_AO_SetChnParams(aoDevId, aoChn, &pstParams);
    if (result != RK_SUCCESS) {
        RK_LOGE("ao set channel params, aoChn = %d", aoChn);
        return RK_FAILURE;
    }

    return RK_SUCCESS;
}

void* sendDataThread(void * ptr) {
    TEST_AO_CTX_S *params = reinterpret_cast<TEST_AO_CTX_S *>(ptr);
    MB_POOL_CONFIG_S pool_config;
    // set default value for struct
    RK_U8 *srcData = RK_NULL;
    AUDIO_FRAME_S frame;
    RK_U64 timeStamp = 0;
    RK_S32 s32MilliSec = -1;
    RK_S32 size = 0;
    RK_S32 result = 0;
    FILE *file = RK_NULL;
    RK_LOGI("params->s32ChnIndex : %d", params->s32ChnIndex);
    if (USE_AO_MIXER) {
        if (params->s32ChnIndex == 0) {
            file = fopen("8000_1_ao0.pcm", "rb");
        } else if (params->s32ChnIndex == 1) {
            file = fopen("8000_1_ao1.pcm", "rb");
        }
    } else {
        file = fopen(params->srcFilePath, "rb");
    }

    if (file == RK_NULL) {
        RK_LOGE("open save file %s failed because %s.", params->srcFilePath, strerror(errno));
        goto __EXIT;
    }

    srcData = reinterpret_cast<RK_U8 *>(calloc(1024, sizeof(RK_U8)));
    memset(srcData, 0, 1024);
    while (1) {
        size = fread(srcData, 1, 1024, file);

        frame.u32Len = size;
        frame.u64TimeStamp = timeStamp++;
        frame.enBitWidth = find_bit_width(params->s32BitWidth);
        frame.enSoundMode = find_sound_mode(params->s32Channel);
        frame.bBypassMbBlk = RK_FALSE;

        MB_EXT_CONFIG_S extConfig;
        memset(&extConfig, 0, sizeof(extConfig));
        extConfig.pOpaque = srcData;
        extConfig.pu8VirAddr = srcData;
        extConfig.u64Size = size;
        RK_MPI_SYS_CreateMB(&(frame.pMbBlk), &extConfig);
__RETRY:
        result = RK_MPI_AO_SendFrame(params->s32DevId, params->s32ChnIndex, &frame, s32MilliSec);
        if (result < 0) {
            RK_LOGE("send frame fail, result = %d, TimeStamp = %lld, s32MilliSec = %d",
                result, frame.u64TimeStamp, s32MilliSec);
            goto __RETRY;
        }
        RK_MPI_MB_ReleaseMB(frame.pMbBlk);

        if (size <= 0) {
            RK_LOGI("eof");
            break;
        }
    }

__EXIT:
    RK_MPI_AO_WaitEos(params->s32DevId, params->s32ChnIndex, s32MilliSec);
    if (file) {
        fclose(file);
        file = RK_NULL;
    }
    free(srcData);
    return RK_NULL;
}

void* commandThread(void * ptr) {
    TEST_AO_CTX_S *params = reinterpret_cast<TEST_AO_CTX_S *>(ptr);

    {
        AUDIO_FADE_S aFade;
        aFade.bFade = RK_FALSE;
        aFade.enFadeOutRate = (AUDIO_FADE_RATE_E)params->s32SetFadeRate;
        aFade.enFadeInRate = (AUDIO_FADE_RATE_E)params->s32SetFadeRate;
        RK_BOOL mute = (params->s32SetMute == 0) ? RK_FALSE : RK_TRUE;
        RK_LOGI("test info : mute = %d, volume = %d", mute, params->s32SetVolume);
        RK_MPI_AO_SetMute(params->s32DevId, mute, &aFade);
        RK_MPI_AO_SetVolume(params->s32DevId, params->s32SetVolume);
    }

    if (params->s32SetTrackMode) {
        RK_LOGI("test info : set track mode = %d", params->s32SetTrackMode);
        RK_MPI_AO_SetTrackMode(params->s32DevId, (AUDIO_TRACK_MODE_E)params->s32SetTrackMode);
        params->s32SetTrackMode = 0;
    }

    if (params->s32GetVolume) {
        RK_S32 volume = 0;
        RK_MPI_AO_GetVolume(params->s32DevId, &volume);
        RK_LOGI("test info : get volume = %d", volume);
        params->s32GetVolume = 0;
    }

    if (params->s32GetMute) {
        RK_BOOL mute = RK_FALSE;
        AUDIO_FADE_S fade;
        RK_MPI_AO_GetMute(params->s32DevId, &mute, &fade);
        RK_LOGI("test info : is mute = %d", mute);
        params->s32GetMute = 0;
    }

    if (params->s32GetTrackMode) {
        AUDIO_TRACK_MODE_E trackMode;
        RK_MPI_AO_GetTrackMode(params->s32DevId, &trackMode);
        RK_LOGI("test info : get track mode = %d", trackMode);
        params->s32GetTrackMode = 0;
    }

    if (params->s32QueryChnStat) {
        query_ao_flow_graph_stat(params->s32DevId, params->s32ChnIndex);
        params->s32QueryChnStat = 0;
    }

    if (params->s32SaveFile) {
        AUDIO_SAVE_FILE_INFO_S saveFile;
        memset(&saveFile, 0, sizeof(AUDIO_SAVE_FILE_INFO_S));
        if (params->dstFilePath) {
            saveFile.bCfg = RK_TRUE;
            saveFile.u32FileSize = 1024 * 1024;
            snprintf(saveFile.aFileName, sizeof(saveFile.aFileName), "%s", "ao_save_file.bin");
            snprintf(saveFile.aFilePath, sizeof(saveFile.aFilePath), "%s", params->dstFilePath);
        }
        RK_MPI_AO_SaveFile(params->s32DevId, params->s32ChnIndex, &saveFile);
        params->s32SaveFile = 0;
    }

    if (params->s32QueryFileStat) {
        AUDIO_FILE_STATUS_S fileStat;
        RK_MPI_AO_QueryFileStatus(params->s32DevId, params->s32ChnIndex, &fileStat);
        RK_LOGI("test info : query save file status = %d", fileStat.bSaving);
        params->s32QueryFileStat = 0;
    }

    if (params->s32PauseResumeChn) {
        usleep(500 * 1000);
        RK_MPI_AO_PauseChn(params->s32DevId, params->s32ChnIndex);
        RK_LOGI("pause test");
        usleep(1000 * 1000);
        RK_MPI_AO_ResumeChn(params->s32DevId, params->s32ChnIndex);
        RK_LOGI("resume test");
        params->s32PauseResumeChn = 0;
    }

    if (params->s32ClrChnBuf) {
        RK_MPI_AO_ClearChnBuf(params->s32DevId, params->s32ChnIndex);
        params->s32ClrChnBuf = 0;
    }

    if (params->s32ClrPubAttr) {
        RK_MPI_AO_ClrPubAttr(params->s32DevId);
        params->s32ClrPubAttr = 0;
    }

    if (params->s32GetPubAttr) {
        AIO_ATTR_S pstAttr;
        RK_MPI_AO_GetPubAttr(params->s32DevId, &pstAttr);
        RK_LOGI("input stream rate = %d", pstAttr.enSamplerate);
        RK_LOGI("input stream sound mode = %d", pstAttr.enSoundmode);
        RK_LOGI("open sound card rate = %d", pstAttr.soundCard.sampleRate);
        RK_LOGI("open sound card channel = %d", pstAttr.soundCard.channels);
        params->s32GetPubAttr = 0;
    }

    return RK_NULL;
}

RK_S32 unit_test_mpi_ao(TEST_AO_CTX_S *ctx) {
    RK_S32 i = 0;
    TEST_AO_CTX_S params[AO_MAX_CHN_NUM];
    pthread_t tidSend[AO_MAX_CHN_NUM];
    pthread_t tidReceive[AO_MAX_CHN_NUM];

    if (ctx->s32ChnNum > AO_MAX_CHN_NUM) {
        RK_LOGE("ao chn(%d) > max_chn(%d)", ctx->s32ChnNum, AO_MAX_CHN_NUM);
        goto __FAILED;
    }

    if (test_open_device_ao(ctx) != RK_SUCCESS) {
        goto __FAILED;
    }

    for (i = 0; i < ctx->s32ChnNum; i++) {
        memcpy(&(params[i]), ctx, sizeof(TEST_AO_CTX_S));
        params[i].s32ChnIndex = i;

        test_set_channel_params_ao(&params[i]);
        test_init_mpi_ao(&params[i]);
        pthread_create(&tidSend[i], RK_NULL, sendDataThread, reinterpret_cast<void *>(&params[i]));
        pthread_create(&tidReceive[i], RK_NULL, commandThread, reinterpret_cast<void *>(&params[i]));
    }

    for (i = 0; i < ctx->s32ChnNum; i++) {
        pthread_join(tidSend[i], RK_NULL);
        pthread_join(tidReceive[i], RK_NULL);
        deinit_mpi_ao(params[i].s32DevId, params[i].s32ChnIndex);
    }

    test_close_device_ao(ctx);

    return RK_SUCCESS;
__FAILED:

    return RK_FAILURE;
}


static RK_S32 adec_data_free(void *opaque) {
    if (opaque) {
        free(opaque);
        opaque = RK_NULL;
    }
    return 0;
}

static void *send_stream_thread(void *arg) {
    RK_S32 s32ret = 0;
    TEST_AO_CTX_S *params = reinterpret_cast<TEST_AO_CTX_S *>(arg);
    RK_U8 *srcData = RK_NULL;
    RK_S32 srcSize = 0;
    FILE  *file = RK_NULL;
    RK_S32 err = 0;
    RK_S32 pktEos = 0;
    AUDIO_STREAM_S stAudioStream;
    ADEC_CHN AdChn = 0;
    RK_U64 timeStamp = 0;
    RK_S32 count = 0;
    RK_S64 dataOffset = 0;

    file = fopen(params->adecFilePath, "rb");
    if (file == RK_NULL) {
        RK_LOGE("failed to open input file(%s), error: %s", params->adecFilePath, strerror(errno));
        goto __FAILED;
    }

    while (1) {
        srcData = reinterpret_cast<RK_U8 *>(calloc(1024, sizeof(RK_U8)));
        memset(srcData, 0, 1024);

        srcSize = fread(srcData, 1, 1024, file);

        if (srcSize == 0 || srcData == RK_NULL) {
            RK_LOGI("read eos packet, now send eos packet!");
            pktEos = 1;
        }

        if (pktEos) {
            RK_MPI_ADEC_SendEndOfStream(AdChn, RK_FALSE);
            // free srcData here, otherwise it will be leaked
            free(srcData);
            break;
        } else {
            stAudioStream.u32Len = srcSize;
            stAudioStream.u64TimeStamp = timeStamp;
            stAudioStream.u32Seq = ++count;
            stAudioStream.bBypassMbBlk = RK_TRUE;
            MB_EXT_CONFIG_S extConfig = {0};
            extConfig.pFreeCB = adec_data_free;
            extConfig.pOpaque = srcData;
            extConfig.pu8VirAddr = srcData;
            extConfig.u64Size    = srcSize;
            RK_MPI_SYS_CreateMB(&(stAudioStream.pMbBlk), &extConfig);
__RETRY:
            s32ret = RK_MPI_ADEC_SendStream(AdChn, &stAudioStream, RK_TRUE);
            if (s32ret != RK_SUCCESS) {
                RK_LOGE("fail to send adec stream.");
                goto __RETRY;
            }
            RK_MPI_MB_ReleaseMB(stAudioStream.pMbBlk);
        }
        timeStamp++;
    }

__FAILED:
    if (file) {
        fclose(file);
        file = RK_NULL;
    }

    return RK_NULL;
}

static RK_S32 test_adec_bind_ao_loop(TEST_AO_CTX_S *ctx) {
    RK_S32 s32ret = 0;
    TEST_AO_CTX_S params;
    ADEC_CHN_ATTR_S stChnAttr;

    if (test_open_device_ao(ctx) != RK_SUCCESS) {
        goto __FAILED;
    }

    memcpy(&params, ctx, sizeof(TEST_AO_CTX_S));
    params.s32ChnIndex = 0;
    test_set_channel_params_ao(&params);
    test_init_mpi_ao(&params);
    RK_MPI_AO_SetTrackMode(ctx->s32DevId, AUDIO_TRACK_OUT_STEREO);

    memset(&stChnAttr, 0, sizeof(ADEC_CHN_ATTR_S));
    stChnAttr.stCodecAttr.enType  = RK_AUDIO_ID_ADPCM_G726;
    stChnAttr.stCodecAttr.u32Channels = params.s32Channel;
    stChnAttr.stCodecAttr.u32SampleRate = params.s32ReSmpSampleRate;
    stChnAttr.stCodecAttr.u32BitPerCodedSample = 4;
    stChnAttr.enType      = RK_AUDIO_ID_ADPCM_G726;
    stChnAttr.enMode      = ADEC_MODE_STREAM;
    stChnAttr.u32BufCount = 4;
    stChnAttr.u32BufSize  = 50*1024;  // 50K
    s32ret = RK_MPI_ADEC_CreateChn(0, &stChnAttr);
    if (s32ret) {
        RK_LOGE("create adec chn %d err:0x%x\n", 0, s32ret);
        s32ret = RK_FAILURE;
    } else {
        s32ret = RK_SUCCESS;
    }

    //ai bind aenc
    MPP_CHN_S stSrcChn, stDestChn;
    stSrcChn.enModId    = RK_ID_ADEC;
    stSrcChn.s32DevId   = 0;
    stSrcChn.s32ChnId   = 0;

    stDestChn.enModId   = RK_ID_AO;
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
    pthread_create(&read_thread, RK_NULL, send_stream_thread, reinterpret_cast<void *>(ctx));
    pthread_join(read_thread, RK_NULL);

__FAILED:
    s32ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (s32ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_SYS_UnBind fail %x", s32ret);
    }
    deinit_mpi_ao(0, 0);
    s32ret = RK_MPI_ADEC_DestroyChn(0);
    if (s32ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_ADEC_DestroyChn fail %x", s32ret);
    }
    return RK_NULL;
}

static const char *const usages[] = {
    "./rk_mpi_ao_test [-i src_path] [--device_rate rate] [--device_ch ch] [--input_rate rate] [--input_ch ch]...",
    NULL,
};

static void mpi_ao_test_show_options(const TEST_AO_CTX_S *ctx) {
    RK_PRINT("cmd parse result:\n");
    RK_PRINT("input  file name      : %s\n", ctx->srcFilePath);
    RK_PRINT("output file name      : %s\n", ctx->dstFilePath);
    RK_PRINT("loop count            : %d\n", ctx->s32LoopCount);
    RK_PRINT("channel number        : %d\n", ctx->s32ChnNum);
    RK_PRINT("open sound rate       : %d\n", ctx->s32SampleRate);
    RK_PRINT("open sound channel    : %d\n", ctx->s32DeviceChannel);
    RK_PRINT("input stream rate     : %d\n", ctx->s32ReSmpSampleRate);
    RK_PRINT("input channel         : %d\n", ctx->s32Channel);
    RK_PRINT("bit_width             : %d\n", ctx->s32BitWidth);
    RK_PRINT("period_count          : %d\n", ctx->s32PeriodCount);
    RK_PRINT("period_size           : %d\n", ctx->s32PeriodSize);
    RK_PRINT("sound card name       : %s\n", ctx->chCardName);
    RK_PRINT("device id             : %d\n", ctx->s32DevId);
    RK_PRINT("set volume            : %d\n", ctx->s32SetVolume);
    RK_PRINT("set mute              : %d\n", ctx->s32SetMute);
    RK_PRINT("set track_mode        : %d\n", ctx->s32SetTrackMode);
    RK_PRINT("get volume            : %d\n", ctx->s32GetVolume);
    RK_PRINT("get mute              : %d\n", ctx->s32GetMute);
    RK_PRINT("get track_mode        : %d\n", ctx->s32GetTrackMode);
    RK_PRINT("query stat            : %d\n", ctx->s32QueryChnStat);
    RK_PRINT("pause and resume chn  : %d\n", ctx->s32PauseResumeChn);
    RK_PRINT("save file             : %d\n", ctx->s32SaveFile);
    RK_PRINT("query save file stat  : %d\n", ctx->s32QueryFileStat);
    RK_PRINT("clear buf             : %d\n", ctx->s32ClrChnBuf);
    RK_PRINT("get attribute         : %d\n", ctx->s32GetPubAttr);
    RK_PRINT("clear attribute       : %d\n", ctx->s32ClrPubAttr);
    RK_PRINT("set loopback mode     : %d\n", ctx->s32LoopbackMode);
    RK_PRINT("adec input file name  : %s\n", ctx->adecFilePath);
}

int main(int argc, const char **argv) {
    RK_S32          i;
    RK_S32          s32Ret;
    TEST_AO_CTX_S  *ctx;

    ctx = reinterpret_cast<TEST_AO_CTX_S *>(malloc(sizeof(TEST_AO_CTX_S)));
    memset(ctx, 0, sizeof(TEST_AO_CTX_S));

    ctx->srcFilePath        = RK_NULL;
    ctx->dstFilePath        = RK_NULL;
    ctx->s32LoopCount       = 1;
    ctx->s32ChnNum          = 1;
    ctx->s32SampleRate      = 48000;
    ctx->s32ReSmpSampleRate = 0;
    ctx->s32DeviceChannel   = 2;
    ctx->s32Channel         = 0;
    ctx->s32BitWidth        = 16;
    ctx->s32PeriodCount     = 4;
    ctx->s32PeriodSize      = 1024;
    ctx->chCardName         = RK_NULL;
    ctx->s32DevId           = 0;
    ctx->s32SetVolume       = 100;
    ctx->s32SetMute         = 0;
    ctx->s32SetTrackMode    = 0;
    ctx->s32SetFadeRate     = 0;
    ctx->s32GetVolume       = 0;
    ctx->s32GetMute         = 0;
    ctx->s32GetTrackMode    = 0;
    ctx->s32QueryChnStat    = 0;
    ctx->s32PauseResumeChn  = 0;
    ctx->s32SaveFile        = 0;
    ctx->s32QueryFileStat   = 0;
    ctx->s32ClrChnBuf       = 0;
    ctx->s32ClrPubAttr      = 0;
    ctx->s32GetPubAttr      = 0;
    ctx->s32LoopbackMode    = AUDIO_LOOPBACK_NONE;
    ctx->enMode = TEST_AO_MODE_AO_ONLY;

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("basic options:"),
        OPT_STRING('i', "input",  &(ctx->srcFilePath),
                   "input file name , e.g.(./*.pcm). <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "input_ch", &(ctx->s32Channel),
                    "the sample rate of input data. <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "input_rate", &(ctx->s32ReSmpSampleRate),
                    "the sample rate of input data. <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "device_ch", &(ctx->s32DeviceChannel),
                    "the number of sound card channels. default(2).", NULL, 0, 0),
        OPT_INTEGER('\0', "device_rate", &(ctx->s32SampleRate),
                    "the sample rate of open sound card.  default(48000).", NULL, 0, 0),
        OPT_STRING('o', "output", &(ctx->dstFilePath),
                    "output file name, e.g.(./ao). default(NULL).", NULL, 0, 0),
        OPT_INTEGER('n', "loop_count", &(ctx->s32LoopCount),
                    "loop running count. can be any count. default(1)", NULL, 0, 0),
        OPT_INTEGER('c', "channel_count", &(ctx->s32ChnNum),
                    "the count of ao channel. default(1).", NULL, 0, 0),
        OPT_INTEGER('\0', "bit", &(ctx->s32BitWidth),
                    "the bit width of open sound card, range(8, 16, 24), default(16)", NULL, 0, 0),
        OPT_INTEGER('\0', "period_size", &(ctx->s32PeriodSize),
                    "the period size for open sound card, default(1024)", NULL, 0, 0),
        OPT_INTEGER('\0', "period_count", &(ctx->s32PeriodCount),
                    "the period count for open sound card, default(4)", NULL, 0, 0),
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
        OPT_INTEGER('\0', "query_stat", &(ctx->s32QueryChnStat),
                    "query ao statistics info, range(0, 1), default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "pause_resume", &(ctx->s32PauseResumeChn),
                    "test ao chn pause and resume function, range(0, 1), default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "save_file", &(ctx->s32SaveFile),
                    "test ao save file, if enabled, must set output file, range(0, 1), default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "query_file_stat", &(ctx->s32QueryFileStat),
                    "query file status, range(0, 1), default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "clr_buf", &(ctx->s32ClrChnBuf),
                    "clear buffer of channel, range(0, 1), default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "clr_attr", &(ctx->s32ClrPubAttr),
                    "clear attribute of channel, range(0, 1), default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "get_attr", &(ctx->s32GetPubAttr),
                    "get attribute of device, range(0, 1), default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "loopback_mode", &(ctx->s32LoopbackMode),
                    "configure the loopback mode during ao runtime", NULL, 0, 0),
        OPT_INTEGER('m', "mode", &(ctx->enMode),
                    "test mode(default 0; \n\t"
                    "0:ao send frame \n\t"
                    "1:adec(g726) bind one ao \n\t", NULL, 0, 0),
        OPT_STRING('\0', "enc_path",  &(ctx->adecFilePath),
                   "adec input file name , e.g.(./*.g726). <required>", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nselect a test case to run.",
                                 "\nuse --help for details.");

    argc = argparse_parse(&argparse, argc, argv);
    mpi_ao_test_show_options(ctx);

    if (ctx->srcFilePath == RK_NULL
        || ctx->s32Channel <= 0
        || ctx->s32ReSmpSampleRate <= 0) {
        argparse_usage(&argparse);
        goto __FAILED;
    }

    RK_MPI_SYS_Init();

    switch (ctx->enMode) {
    case TEST_AO_MODE_AO_ONLY:
        for (i = 0; i < ctx->s32LoopCount; i++) {
            RK_LOGI("start running loop count  = %d", i);
            s32Ret = unit_test_mpi_ao(ctx);
            if (s32Ret != RK_SUCCESS) {
                goto __FAILED;
            }
            RK_LOGI("end running loop count  = %d", i);
        }
    break;
    case TEST_AO_MODE_BIND_ADEC:
        /* command:rk_mpi_ai_test --device_rate 16000 --device_ch 2 --out_rate 16000
        --out_ch 2 --sound_card_name hw:0,0 -m 1 --aenc_file aenc.g711a -C g711a */
        s32Ret = test_adec_bind_ao_loop(ctx);
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
