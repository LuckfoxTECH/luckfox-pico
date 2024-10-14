// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "smart_door.h"

static void* smart_door_aenc_get_stream(void *pArgs) {
    SMART_DOOR_CTX_S *pstCtx     = (SMART_DOOR_CTX_S *)(pArgs);
    RK_S32 s32ret = 0;
    AUDIO_STREAM_S pstStream;
    RK_S32 eos = 0;
    RK_S32 count = 0;

    while(!pstCtx->aencthreadExit) {
        s32ret = RK_MPI_AENC_GetStream(pstCtx->stAencCfg.u32ChnId, &pstStream, -1);
        if (s32ret == RK_SUCCESS) {
            MB_BLK bBlk = pstStream.pMbBlk;
            RK_VOID *pstFrame = RK_MPI_MB_Handle2VirAddr(bBlk);
            RK_S32 frameSize = pstStream.u32Len;
            RK_S32 timeStamp = pstStream.u64TimeStamp;
            eos = (frameSize <= 0) ? 1 : 0;
            if (pstFrame) {
                RK_LOGI("get frame data = %p, size = %d, timeStamp = %lld",
                                                  pstFrame, frameSize, timeStamp);
                if (pstCtx->stAencCfg.fp) {
                    fwrite(pstFrame, frameSize, 1, pstCtx->stAencCfg.fp);
                    fflush(pstCtx->stAencCfg.fp);
                }
                RK_MPI_AENC_ReleaseStream(pstCtx->stAencCfg.u32ChnId, &pstStream);
                count++;
            }
            if (pstCtx->stAencCfg.s32MaxFrame > 0 && count == pstCtx->stAencCfg.s32MaxFrame) {
                RK_LOGI("aenc get s32MaxFrame=%d stream!", pstCtx->stAencCfg.s32MaxFrame);
                break;
            }
        } else {
            RK_LOGE("fail to get aenc frame.");
        }

        if (eos) {
            RK_LOGI("get eos stream.");
            break;
        }

    }

    pstCtx->aencTaskEnd = RK_TRUE;
    RK_LOGI("%s out", __FUNCTION__);
    return RK_NULL;
}

RK_S32 smart_door_init_aenc(SMART_DOOR_CTX_S *ctx) {
    RK_S32 s32Ret = RK_SUCCESS;

    ctx->stAencCfg.s32DevId = 0;
    ctx->stAencCfg.u32ChnId = 0;
    ctx->stAencCfg.s32Channel = 2;
    ctx->stAencCfg.s32SampleRate = 44100;
    ctx->stAencCfg.enBitwidth = AUDIO_BIT_WIDTH_16;
    ctx->stAencCfg.enType = RK_AUDIO_ID_PCM_ALAW;
    ctx->stAencCfg.s32MaxFrame = 500;
    ctx->stAencCfg.aencFilePath = "/tmp/aenc.g711a";

    ctx->stAencCfg.fp = fopen(ctx->stAencCfg.aencFilePath, "wb");
    if (ctx->stAencCfg.fp == RK_NULL) {
        RK_LOGE("chn 0 can't open file %s in get picture thread!\n", ctx->stAencCfg.aencFilePath);
        s32Ret = RK_FAILURE;
    }

    return s32Ret;
}

RK_S32 smart_door_create_aenc(SMART_DOOR_AENC_CFG stAencCfg) {
    RK_S32 s32Ret = RK_SUCCESS;

    stAencCfg.pstChnAttr.stCodecAttr.enType		= stAencCfg.enType;
    stAencCfg.pstChnAttr.stCodecAttr.u32Channels	= stAencCfg.s32Channel;
    stAencCfg.pstChnAttr.stCodecAttr.u32SampleRate	= stAencCfg.s32SampleRate;
    stAencCfg.pstChnAttr.stCodecAttr.enBitwidth	= stAencCfg.enBitwidth;
    stAencCfg.pstChnAttr.stCodecAttr.pstResv		= RK_NULL;
    stAencCfg.pstChnAttr.enType		= stAencCfg.enType;
    stAencCfg.pstChnAttr.u32BufCount	= 4;

    s32Ret = RK_MPI_AENC_CreateChn(stAencCfg.u32ChnId, &stAencCfg.pstChnAttr);
    if (s32Ret) {
        RK_LOGE("create aenc chn %d err:0x%x\n", stAencCfg.enType, s32Ret);
    }

    return s32Ret;
}

RK_S32 smart_door_start_aenc(SMART_DOOR_CTX_S *ctx) {
    pthread_create(&ctx->pGetAencStreamThread, 0, smart_door_aenc_get_stream, (void *)(ctx));
    return RK_SUCCESS;
}

RK_S32 smart_door_destroy_aenc(SMART_DOOR_CTX_S *ctx) {
    RK_S32 s32Ret = RK_SUCCESS;

    pthread_join(ctx->pGetAencStreamThread, RK_NULL);

    if (ctx->stAencCfg.fp) {
        fclose(ctx->stAencCfg.fp);
        ctx->stAencCfg.fp = RK_NULL;
    }

    s32Ret = RK_MPI_AENC_DestroyChn(ctx->stAencCfg.u32ChnId);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VDEC_DestroyChn fail %x", s32Ret);
    }

    return s32Ret;
}