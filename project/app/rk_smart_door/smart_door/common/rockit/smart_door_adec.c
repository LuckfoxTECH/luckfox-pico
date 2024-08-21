// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "smart_door.h"

static RK_S32 adec_data_free(void *opaque) {
    if (opaque) {
        free(opaque);
        opaque = RK_NULL;
    }
    return 0;
}

static void *smart_door_adec_put_stream(void *arg) {
    SMART_DOOR_CTX_S *pstCtx     = (SMART_DOOR_CTX_S *)(arg);
    RK_S32 s32ret = 0;
    RK_U8 *srcData = RK_NULL;
    RK_S32 srcSize = 0;
    FILE  *file = RK_NULL;
    RK_S32 err = 0;
    RK_S32 pktEos = 0;
    AUDIO_STREAM_S stAudioStream;
    ADEC_CHN AdChn =  pstCtx->stAdecCfg.u32ChnId;
    RK_U64 timeStamp = 0;
    RK_S32 count = 0;
    RK_S64 dataOffset = 0;

    file = fopen(pstCtx->stAdecCfg.adecFilePath, "rb");
    if (file == RK_NULL) {
        RK_LOGE("failed to open input file(%s), error: %s", pstCtx->stAdecCfg.adecFilePath, strerror(errno));
        goto __FAILED;
    }

    while (!pstCtx->adecthreadExit) {
        srcData = calloc(1024, sizeof(RK_U8));
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
    pstCtx->adecTaskEnd = RK_TRUE;
    return RK_NULL;
}

RK_S32 smart_door_init_adec(SMART_DOOR_CTX_S *ctx) {
    RK_S32 s32ret = RK_SUCCESS;

    ctx->stAdecCfg.s32DevId = 0;
    ctx->stAdecCfg.u32ChnId = 0;
    ctx->stAdecCfg.s32Channel = 2;
    ctx->stAdecCfg.s32ReSmpSampleRate = 44100;
    ctx->stAdecCfg.enType = RK_AUDIO_ID_PCM_ALAW;
    ctx->stAdecCfg.adecFilePath = "/tmp/aenc.g711a";

    return s32ret;
}

RK_S32 smart_door_create_adec(SMART_DOOR_ADEC_CFG stAdecCfg) {
    RK_S32 s32ret = RK_SUCCESS;
    ADEC_CHN_ATTR_S stChnAttr;

    memset(&stChnAttr, 0, sizeof(ADEC_CHN_ATTR_S));
    stChnAttr.stCodecAttr.enType  = stAdecCfg.enType;
    stChnAttr.stCodecAttr.u32Channels = stAdecCfg.s32Channel;
    stChnAttr.stCodecAttr.u32SampleRate = stAdecCfg.s32ReSmpSampleRate;
    stChnAttr.stCodecAttr.u32BitPerCodedSample = 4;
    stChnAttr.enType      = stAdecCfg.enType;
    stChnAttr.enMode      = ADEC_MODE_STREAM;
    stChnAttr.u32BufCount = 4;
    stChnAttr.u32BufSize  = 50*1024;  // 50K

    s32ret = RK_MPI_ADEC_CreateChn(stAdecCfg.u32ChnId, &stChnAttr);
    if (s32ret) {
        RK_LOGE("create adec chn %d err:0x%x\n", stAdecCfg.u32ChnId, s32ret);
    }

    return s32ret;
}

RK_S32 smart_door_start_adec(SMART_DOOR_CTX_S *ctx) {
    pthread_create(&ctx->pGetAdecStreamThread, 0, smart_door_adec_put_stream, (void *)(ctx));
    return RK_SUCCESS;
}

RK_S32 smart_door_destroy_adec(SMART_DOOR_CTX_S *ctx) {
    RK_S32 s32ret = RK_SUCCESS;

    s32ret = RK_MPI_ADEC_DestroyChn(ctx->stAdecCfg.u32ChnId);
    if (s32ret) {
        RK_LOGE("RK_MPI_ADEC_DestroyChn fail %x", s32ret);
    }

    return s32ret;
}