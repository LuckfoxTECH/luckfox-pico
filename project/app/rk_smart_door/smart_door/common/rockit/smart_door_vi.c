// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "smart_door.h"
#include "isp.h"

RK_S32 smart_door_init_vi(SMART_DOOR_VI_CFG *stViCfg) {
    RK_S32 s32Ret = RK_SUCCESS;

    stViCfg->stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
    stViCfg->stChnAttr.stIspOpt.u32BufCount = 2;
    stViCfg->stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
    stViCfg->stChnAttr.u32Depth = 1;
    stViCfg->stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
    stViCfg->stChnAttr.stFrameRate.s32SrcFrameRate = -1;
    stViCfg->stChnAttr.stFrameRate.s32DstFrameRate = -1;

    return s32Ret;
}

RK_S32 smart_door_create_vi(SMART_DOOR_VI_CFG stViCfg) {
    RK_S32 s32Ret = RK_FAILURE;

#if VIDEO_CHN0_WORKING
    if (stViCfg.channelId == 0) {
        RK_LOGI("%s skip for channel %d", __func__, stViCfg.channelId);
        return RK_SUCCESS;
    }
#endif

    s32Ret = RK_MPI_VI_GetDevAttr(stViCfg.devId, &stViCfg.stDevAttr);
    if (s32Ret == RK_ERR_VI_NOT_CONFIG) {
        s32Ret = RK_MPI_VI_SetDevAttr(stViCfg.devId, &stViCfg.stDevAttr);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_VI_SetDevAttr %x", s32Ret);
            goto __FAILED;
        }
    } else {
        RK_LOGE("RK_MPI_VI_SetDevAttr already");
    }

    s32Ret = RK_MPI_VI_GetDevIsEnable(stViCfg.devId);
    if (s32Ret != RK_SUCCESS) {
        s32Ret = RK_MPI_VI_EnableDev(stViCfg.devId);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_VI_EnableDev %x", s32Ret);
            goto __FAILED;
        }
        stViCfg.stBindPipe.u32Num = 1;
        stViCfg.stBindPipe.PipeId[0] = stViCfg.pipeId;
        if (stViCfg.bIfIspGroupInit)
            stViCfg.stBindPipe.bUserStartPipe[0] = RK_TRUE;
        s32Ret = RK_MPI_VI_SetDevBindPipe(stViCfg.devId, &stViCfg.stBindPipe);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_VI_SetDevBindPipe %x", s32Ret);
            goto __FAILED;
        }
    } else {
        RK_LOGE("RK_MPI_VI_EnableDev already");
    }
    s32Ret = RK_MPI_VI_SetChnAttr(stViCfg.pipeId, stViCfg.channelId, &stViCfg.stChnAttr);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VI_SetChnAttr %x", s32Ret);
        goto __FAILED;
    }
    RK_LOGD("RK_MPI_VI_EnableChn %x %d %d", stViCfg.devId, stViCfg.pipeId, stViCfg.channelId);
    if (stViCfg.bIfIspGroupInit) {
        s32Ret = RK_MPI_VI_EnableChnExt(stViCfg.pipeId, stViCfg.channelId);
    } else {
        s32Ret = RK_MPI_VI_EnableChn(stViCfg.pipeId, stViCfg.channelId);
    }
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VI_EnableChn %x", s32Ret);
        goto __FAILED;
    }
    if (stViCfg.stDebugFile.bCfg) {
        s32Ret = RK_MPI_VI_ChnSaveFile(stViCfg.pipeId, stViCfg.channelId, &stViCfg.stDebugFile);
        RK_LOGD("RK_MPI_VI_ChnSaveFile %x", s32Ret);
    }

__FAILED:
    return s32Ret;
}

static RK_U64 TEST_COMM_GetNowUs() {
    struct timespec time = {0, 0};
    clock_gettime(CLOCK_MONOTONIC, &time);
    return (RK_U64)time.tv_sec * 1000000 + (RK_U64)time.tv_nsec / 1000; /* microseconds */
}

RK_S32 __attribute__((unused)) smart_door_start_vi(SMART_DOOR_VI_CFG stViCfg) {
    RK_S32 s32Ret;
    RK_S32 loopCount = 0;
    RK_S32 waitTime = 33;

    RK_MPI_VI_SetChnFreeze(stViCfg.pipeId, stViCfg.channelId, stViCfg.bFreeze);
    while (loopCount < stViCfg.loopCountSet) {
        s32Ret = RK_MPI_VI_GetChnFrame(stViCfg.pipeId, stViCfg.channelId, &stViCfg.stViFrame, waitTime);
        if (s32Ret == RK_SUCCESS) {
            RK_U64 nowUs = TEST_COMM_GetNowUs();
            void *data = RK_MPI_MB_Handle2VirAddr(stViCfg.stViFrame.stVFrame.pMbBlk);
            RK_LOGE("RK_MPI_VI_GetChnFrame ok:data %p loop:%d seq:%d pts:%lld ms len=%d", data, loopCount,
                     stViCfg.stViFrame.stVFrame.u32TimeRef, stViCfg.stViFrame.stVFrame.u64PTS / 1000,
                     stViCfg.stViFrame.stVFrame.u64PrivateData);
            s32Ret = RK_MPI_VI_QueryChnStatus(stViCfg.pipeId, stViCfg.channelId, &stViCfg.stChnStatus);
            RK_LOGE("RK_MPI_VI_QueryChnStatus ret %x, w:%d,h:%d,enable:%d," \
                    "current frame id:%d,input lost:%d,output lost:%d," \
                    "framerate:%d,vbfail:%d delay=%lldus",
                     s32Ret,
                     stViCfg.stChnStatus.stSize.u32Width,
                     stViCfg.stChnStatus.stSize.u32Height,
                     stViCfg.stChnStatus.bEnable,
                     stViCfg.stChnStatus.u32CurFrameID,
                     stViCfg.stChnStatus.u32InputLostFrame,
                     stViCfg.stChnStatus.u32OutputLostFrame,
                     stViCfg.stChnStatus.u32FrameRate,
                     stViCfg.stChnStatus.u32VbFail,
                     nowUs - stViCfg.stViFrame.stVFrame.u64PTS);
            s32Ret = RK_MPI_VI_ReleaseChnFrame(stViCfg.pipeId, stViCfg.channelId, &stViCfg.stViFrame);
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("RK_MPI_VI_ReleaseChnFrame fail %x", s32Ret);
            }
            loopCount++;
        } else {
            RK_LOGE("RK_MPI_VI_GetChnFrame timeout %x", s32Ret);
        }
        usleep(10*1000);
    }
    return 0;
}

RK_S32 smart_door_vi_get_frame(SMART_DOOR_VI_CFG *vi_cfg, void **data) {
    RK_S32 s32Ret = RK_FAILURE;
    RK_S32 waitTime = 100;

    if (!vi_cfg)
        return s32Ret;

    s32Ret = RK_MPI_VI_GetChnFrame(vi_cfg->pipeId, vi_cfg->channelId, &vi_cfg->stViFrame, waitTime);
        if (s32Ret == RK_SUCCESS) {
            RK_U64 nowUs = TEST_COMM_GetNowUs();
            *data = RK_MPI_MB_Handle2VirAddr(vi_cfg->stViFrame.stVFrame.pMbBlk);
            RK_LOGI("RK_MPI_VI_GetChnFrame ok: channel:%d pipe:%d data %p seq:%d pts:%lld ms len=%d",
                     vi_cfg->channelId, vi_cfg->pipeId, data,
                     vi_cfg->stViFrame.stVFrame.u32TimeRef, vi_cfg->stViFrame.stVFrame.u64PTS / 1000,
                     vi_cfg->stViFrame.stVFrame.u64PrivateData);
            s32Ret = RK_MPI_VI_QueryChnStatus(vi_cfg->pipeId, vi_cfg->channelId, &vi_cfg->stChnStatus);
            RK_LOGD("RK_MPI_VI_QueryChnStatus ret %x, w:%d,h:%d,enable:%d," \
                    "current frame id:%d,input lost:%d,output lost:%d," \
                    "framerate:%d,vbfail:%d delay=%lldus",
                     s32Ret,
                     vi_cfg->stChnStatus.stSize.u32Width,
                     vi_cfg->stChnStatus.stSize.u32Height,
                     vi_cfg->stChnStatus.bEnable,
                     vi_cfg->stChnStatus.u32CurFrameID,
                     vi_cfg->stChnStatus.u32InputLostFrame,
                     vi_cfg->stChnStatus.u32OutputLostFrame,
                     vi_cfg->stChnStatus.u32FrameRate,
                     vi_cfg->stChnStatus.u32VbFail,
                     nowUs - vi_cfg->stViFrame.stVFrame.u64PTS);
        } else {
            RK_LOGE("RK_MPI_VI_GetChnFrame timeout %x", s32Ret);
        }

    return s32Ret;
}

RK_S32 smart_door_vi_release_frame(SMART_DOOR_VI_CFG *vi_cfg) {
    RK_S32 s32Ret = RK_FAILURE;

    s32Ret = RK_MPI_VI_ReleaseChnFrame(vi_cfg->pipeId, vi_cfg->channelId, &vi_cfg->stViFrame);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VI_ReleaseChnFrame fail %x", s32Ret);
    }

    return s32Ret;
}

RK_S32 smart_door_destroy_vi(SMART_DOOR_VI_CFG stViCfg) {
    RK_S32 s32Ret = RK_FAILURE;

#if VIDEO_CHN0_WORKING
    if (stViCfg.channelId == 0) {
        RK_LOGI("%s skip for channel %d", __func__, stViCfg.channelId);
        return RK_SUCCESS;
    }
#endif

    if (stViCfg.bIfIspGroupInit) {
        s32Ret = RK_MPI_VI_DisableChnExt(stViCfg.pipeId, stViCfg.channelId);
    } else {
        s32Ret = RK_MPI_VI_DisableChn(stViCfg.pipeId, stViCfg.channelId);
    }
    RK_LOGD("RK_MPI_VI_DisableChn pipe=%d ret:%x", stViCfg.pipeId, s32Ret);

    //other pipe may be running, do not need disable device.
    if (stViCfg.bIfIspGroupInit) {
        s32Ret = RK_MPI_VI_DisableDev(stViCfg.devId);
        RK_LOGD("RK_MPI_VI_DisableDev pipe=%d ret:%x", stViCfg.pipeId, s32Ret);
    }

    return s32Ret;
}