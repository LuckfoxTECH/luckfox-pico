/*
 * Copyright 2021 Rockchip Electronics Co. LTD
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
 */
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#include <pthread.h>
#include <string.h>
#include <cerrno>
#include "test_comm_tde.h"
#include "test_comm_imgproc.h"
#include "test_comm_sys.h"
#include "test_comm_utils.h"
#include "rk_mpi_tde.h"
#include "rk_mpi_cal.h"
#include "rk_mpi_mb.h"
#include "rk_mpi_mmz.h"
#include "rk_mpi_sys.h"
#include "rk_common.h"
#include "rk_debug.h"

RK_S32 TEST_TDE_Save_DstFrame(TEST_TDE_PROC_CTX_S *pstCtx) {
    RK_S32 s32Ret = RK_SUCCESS;
    MB_PIC_CAL_S stCalResult;
    PIC_BUF_ATTR_S stPicBufAttr;
    RK_U32 u32DstWidth = pstCtx->pstDst.u32Width;
    RK_U32 u32DstHeight = pstCtx->pstDst.u32Height;
    RK_U32 u32DstPixelFormat = pstCtx->pstDst.enColorFmt;
    RK_S32 s32DstCompressMode = pstCtx->pstDst.enComprocessMode;;
    stPicBufAttr.u32Width = u32DstWidth;
    stPicBufAttr.u32Height = u32DstHeight;
    stPicBufAttr.enPixelFormat = (PIXEL_FORMAT_E)u32DstPixelFormat;
    stPicBufAttr.enCompMode = (COMPRESS_MODE_E)s32DstCompressMode;

    s32Ret = RK_MPI_CAL_VGS_GetPicBufferSize(&stPicBufAttr, &stCalResult);
    if (pstCtx->dstFileName != RK_NULL) {
        FILE *fp = fopen(pstCtx->dstFileName, "wb+");
        if (fp == RK_NULL) {
            RK_LOGE("fopen %s failed, error: %s", pstCtx->dstFileName, strerror(errno));
            return RK_FAILURE;
        }
        fwrite(RK_MPI_MB_Handle2VirAddr(pstCtx->pstDst.pMbBlk),
                1, stCalResult.u32MBSize, fp);
        fflush(fp);
        fclose(fp);
    }
    return s32Ret;
}

RK_S32 TEST_TDE_TransSurfaceToVideoFrame(TEST_TDE_PROC_CTX_S *pstCtx, VIDEO_FRAME_INFO_S *pstFrames) {
    TDE_SURFACE_S *surface = &pstCtx->pstDst;
    pstFrames->stVFrame.enCompressMode = surface->enComprocessMode;
    pstFrames->stVFrame.pMbBlk = surface->pMbBlk;
    pstFrames->stVFrame.u32Width = surface->u32Width;
    pstFrames->stVFrame.u32Height = surface->u32Height;
    pstFrames->stVFrame.u32VirWidth = surface->u32Width;
    pstFrames->stVFrame.u32VirHeight = surface->u32Height;
    pstFrames->stVFrame.enPixelFormat = surface->enColorFmt;
    return RK_SUCCESS;
}

RK_S32 TEST_TDE_AddTask(TEST_TDE_PROC_CTX_S *pstCtx, TDE_HANDLE jobHandle) {
    RK_S32 s32Ret = RK_SUCCESS;

    switch (pstCtx->opType) {
        case TDE_OP_QUICK_COPY: {
          s32Ret = RK_TDE_QuickCopy(jobHandle,
                &pstCtx->pstSrc, &pstCtx->pstSrcRect,
                &pstCtx->pstDst, &pstCtx->pstDstRect);
        } break;
        case TDE_OP_QUICK_RESIZE: {
          s32Ret = RK_TDE_QuickResize(jobHandle,
                &pstCtx->pstSrc, &pstCtx->pstSrcRect,
                &pstCtx->pstDst, &pstCtx->pstDstRect);
        } break;
        case TDE_OP_QUICK_FILL: {
          s32Ret = RK_TDE_QuickFill(jobHandle,
                &pstCtx->pstDst, &pstCtx->pstDstRect,
                pstCtx->fillData);
        } break;
        case TDE_OP_ROTATION: {
          s32Ret = RK_TDE_Rotate(jobHandle,
                &pstCtx->pstSrc, &pstCtx->pstSrcRect,
                &pstCtx->pstDst, &pstCtx->pstDstRect,
                (ROTATION_E)pstCtx->rotateAngle);
        } break;
        case TDE_OP_COLOR_KEY:
        case TDE_OP_MIRROR: {
          s32Ret = RK_TDE_Bitblit(jobHandle,
                &pstCtx->pstDst, &pstCtx->pstDstRect,
                &pstCtx->pstSrc, &pstCtx->pstSrcRect,
                &pstCtx->pstDst, &pstCtx->pstDstRect,
                &pstCtx->stOpt);
        } break;
        default: {
          RK_LOGE("unknown operation type %d", pstCtx->opType);
        break;
        }
    }
    return s32Ret;
}

TDE_HANDLE TEST_TDE_BeginJob() {
    TDE_HANDLE jobHandle = -1;
    jobHandle = RK_TDE_BeginJob();
    return jobHandle;
}

RK_S32 TEST_TDE_EndJob(TDE_HANDLE jobHandle) {
    RK_S32 s32Ret = RK_SUCCESS;
    s32Ret = RK_TDE_EndJob(jobHandle, RK_FALSE, RK_TRUE, 10);
    if (s32Ret != RK_SUCCESS) {
        RK_TDE_CancelJob(jobHandle);
        return RK_FAILURE;
    }
    RK_TDE_WaitForDone(jobHandle);
    return s32Ret;
}

RK_S32 TEST_TDE_LoadSrcFrame(TEST_TDE_PROC_CTX_S *pstCtx) {
    RK_S32 s32Ret = RK_SUCCESS;
    PIC_BUF_ATTR_S stPicBufAttr;
    VIDEO_FRAME_INFO_S stVideoFrame;
    RK_U8 *pstVideoFrame;
    RK_U32 u32VirSrcWidth = 0;
    RK_U32 u32VirSrcHeight = 0;
    RK_U32 u32SrcWidth = pstCtx->pstSrc.u32Width;
    RK_U32 u32SrcHeight = pstCtx->pstSrc.u32Height;
    PIXEL_FORMAT_E u32SrcPixelFormat = pstCtx->pstSrc.enColorFmt;
    COMPRESS_MODE_E u32SrcCompressMode = pstCtx->pstSrc.enComprocessMode;

    memset(&stVideoFrame, 0x0, sizeof(VIDEO_FRAME_INFO_S));

    stPicBufAttr.u32Width = u32SrcWidth;
    stPicBufAttr.u32Height = u32SrcHeight;
    stPicBufAttr.enPixelFormat = u32SrcPixelFormat;
    stPicBufAttr.enCompMode = u32SrcCompressMode;
    s32Ret = TEST_SYS_CreateVideoFrame(&stPicBufAttr, &stVideoFrame);
    u32VirSrcWidth = stVideoFrame.stVFrame.u32VirWidth;
    u32VirSrcHeight = stVideoFrame.stVFrame.u32VirHeight;

    if (s32Ret != RK_SUCCESS) {
        goto __FAILED;
    }

    pstVideoFrame = (RK_U8 *)RK_MPI_MB_Handle2VirAddr(stVideoFrame.stVFrame.pMbBlk);
    if (pstCtx->srcFileName != RK_NULL) {
        s32Ret = TEST_COMM_FileReadOneFrame(pstCtx->srcFileName, &stVideoFrame);
        if (s32Ret != RK_SUCCESS) {
            goto __FAILED;
        }
    } else {
        s32Ret = TEST_COMM_FillImage(
                    pstVideoFrame,
                    u32SrcWidth, u32SrcHeight,
                    RK_MPI_CAL_COMM_GetHorStride(u32VirSrcWidth, u32SrcPixelFormat),
                    u32VirSrcHeight,
                    u32SrcPixelFormat, 1);
        if (s32Ret != RK_SUCCESS) {
            goto __FAILED;
        }
    }
    RK_MPI_SYS_MmzFlushCache(stVideoFrame.stVFrame.pMbBlk, RK_FALSE);
    pstCtx->pstSrc.pMbBlk = stVideoFrame.stVFrame.pMbBlk;
__FAILED:
    if (s32Ret != RK_SUCCESS) {
        RK_MPI_MB_ReleaseMB(stVideoFrame.stVFrame.pMbBlk);
    }
    return s32Ret;
}

RK_S32 TEST_TDE_CreateDstFrame(TEST_TDE_PROC_CTX_S *pstCtx) {
    RK_S32 s32Ret = RK_SUCCESS;
    PIC_BUF_ATTR_S stPicBufAttr;
    VIDEO_FRAME_INFO_S stVideoFrame;

    RK_U32 u32DstWidth = pstCtx->pstDst.u32Width;
    RK_U32 u32DstHeight = pstCtx->pstDst.u32Height;
    RK_U32 u32DstPixelFormat = pstCtx->pstDst.enColorFmt;
    RK_S32 s32DstCompressMode = pstCtx->pstDst.enComprocessMode;;

    stPicBufAttr.u32Width = u32DstWidth;
    stPicBufAttr.u32Height = u32DstHeight;
    stPicBufAttr.enPixelFormat = (PIXEL_FORMAT_E)u32DstPixelFormat;
    stPicBufAttr.enCompMode = (COMPRESS_MODE_E)s32DstCompressMode;
    s32Ret = TEST_SYS_CreateVideoFrame(&stPicBufAttr, &stVideoFrame);
    if (s32Ret != RK_SUCCESS) {
        goto __FAILED;
    }

    pstCtx->pstDst.pMbBlk = stVideoFrame.stVFrame.pMbBlk;

__FAILED:
    if (s32Ret != RK_SUCCESS) {
        RK_MPI_MB_ReleaseMB(stVideoFrame.stVFrame.pMbBlk);
    }
    return s32Ret;
}

RK_S32 TEST_TDE_ProcessJob(TEST_TDE_PROC_CTX_S *pstCtx, VIDEO_FRAME_INFO_S *pstFrames) {
    RK_S32 s32Ret = RK_SUCCESS;

    TEST_TDE_LoadSrcFrame(pstCtx);
    TEST_TDE_CreateDstFrame(pstCtx);

    TDE_HANDLE jobHandle = TEST_TDE_BeginJob();
    RK_S32 s32TaskNum = (pstCtx->s32TaskNum == 0) ? 1 : pstCtx->s32TaskNum;

    for (RK_S32 taskIdx = 0; taskIdx < s32TaskNum; taskIdx++) {
        s32Ret = TEST_TDE_AddTask(pstCtx, jobHandle);
        if (s32Ret != RK_SUCCESS) {
            goto __FAILED;
        }
    }

    s32Ret = TEST_TDE_EndJob(jobHandle);
    if (s32Ret != RK_SUCCESS) {
        goto __FAILED;
    }
    if (pstFrames) {
        TEST_TDE_TransSurfaceToVideoFrame(pstCtx, pstFrames);
    }


__FAILED:
    if (s32Ret != RK_SUCCESS) {
        RK_MPI_MB_ReleaseMB(pstCtx->pstSrc.pMbBlk);
        RK_MPI_MB_ReleaseMB(pstCtx->pstDst.pMbBlk);
    }
    return s32Ret;
}

void* TEST_TDE_SingleProc(void *pArgs) {
    RK_S32 s32Ret = RK_SUCCESS;
    TEST_TDE_PROC_CTX_S *pstCtx = reinterpret_cast<TEST_TDE_PROC_CTX_S *>(pArgs);
    VIDEO_FRAME_INFO_S pstFrames;
    s32Ret = TEST_TDE_ProcessJob(pstCtx, &pstFrames);
    if (s32Ret != RK_SUCCESS) {
        goto __FAILED;
    }
    TEST_TDE_Save_DstFrame(pstCtx);

__FAILED:
    RK_MPI_MB_ReleaseMB(pstCtx->pstSrc.pMbBlk);
    RK_MPI_MB_ReleaseMB(pstCtx->pstDst.pMbBlk);
    return RK_NULL;
}

RK_S32 TEST_TDE_MultiTest(TEST_TDE_PROC_CTX_S *ctx) {
    RK_S32 s32Ret = RK_SUCCESS;
    pthread_t tids[TDE_MAX_JOB_NUM];
    TEST_TDE_PROC_CTX_S tmpCtx[TDE_MAX_JOB_NUM];

    for (RK_S32 jobIndex = 0; jobIndex < ctx->s32JobNum; jobIndex++) {
        memcpy(&(tmpCtx[jobIndex]), ctx, sizeof(TEST_TDE_PROC_CTX_S));
        pthread_create(&tids[jobIndex], 0, TEST_TDE_SingleProc, reinterpret_cast<void *>(&tmpCtx[jobIndex]));
    }

    for (RK_S32 jobIndex = 0; jobIndex < ctx->s32JobNum; jobIndex++) {
        void *retval;
        pthread_join(tids[jobIndex], &retval);
        if (retval == RK_NULL) {
            RK_LOGE("tde multi test error test id:%d", tids[jobIndex]);
            s32Ret = RK_FAILURE;
        }
    }

    return s32Ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
