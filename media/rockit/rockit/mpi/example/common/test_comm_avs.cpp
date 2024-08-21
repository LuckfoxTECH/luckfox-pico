/*
 * Copyright 2021 Rockchip Electronics Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance: the License.
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

#include <random>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#include <string.h>
#include <stdlib.h>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "rk_debug.h"

#include "test_common.h"
#include "test_comm_avs.h"
#include "test_comm_sys.h"
#include "test_comm_imgproc.h"
#include "test_comm_utils.h"

#include "rk_mpi_avs.h"
#include "rk_mpi_cal.h"
#include "rk_mpi_mb.h"
#include "rk_mpi_mmz.h"
#include "rk_mpi_sys.h"

RK_S32 TEST_AVS_Start(
        AVS_GRP AvsGrp,
        RK_U32 s32ChnNum,
        AVS_GRP_ATTR_S *pstAvsGrpAttr,
        AVS_CHN_ATTR_S *pstAvsChnAttr) {
    AVS_CHN AvsChn = 0;
    RK_S32 s32Ret = RK_SUCCESS;

    s32Ret = RK_MPI_AVS_CreateGrp(AvsGrp, pstAvsGrpAttr);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("avs [%d, %d] RK_MPI_AVS_CreateGrp failed: %#x!",
                AvsGrp, 0, s32Ret);
        return s32Ret;
    }
    RK_LOGV("avs [%d, %d] RK_MPI_AVS_CreateGrp already.", AvsGrp, 0);

    for (AvsChn = 0; AvsChn < s32ChnNum; AvsChn++) {
        s32Ret = RK_MPI_AVS_SetChnAttr(AvsGrp, AvsChn, &pstAvsChnAttr[AvsChn]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("avs [%d, %d]RK_MPI_AVS_SetChnAttr failed: %#x!",
                    AvsGrp, AvsChn, s32Ret);
            return s32Ret;
        }
        RK_LOGV("avs [%d, %d] RK_MPI_AVS_SetChnAttr already.", AvsGrp, AvsChn);

        s32Ret = RK_MPI_AVS_EnableChn(AvsGrp, AvsChn);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_AVS_EnableChn failed: %#x!", AvsChn, s32Ret);
            return s32Ret;
        }
        RK_LOGV("avs [%d, %d] RK_MPI_AVS_EnableChn already.", AvsGrp, AvsChn);
    }

    s32Ret = RK_MPI_AVS_StartGrp(AvsGrp);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_AVS_StartGrp failed: %#x!", AvsGrp, s32Ret);
        return s32Ret;
    }
    RK_LOGV("avs [%d, %d] RK_MPI_AVS_StartGrp already.", AvsGrp, 0);


    s32Ret = RK_MPI_AVS_ResetGrp(AvsGrp);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_AVS_ResetGrp failed: %#x!", AvsGrp, s32Ret);
        return s32Ret;
    }
    RK_LOGV("avs [%d, %d] RK_MPI_AVS_ResetGrp already.", AvsGrp, 0);

    return s32Ret;
}

RK_S32 TEST_AVS_Stop(AVS_GRP AvsGrp, RK_U32 s32ChnNum) {
    RK_S32 s32Ret = RK_SUCCESS;
    AVS_CHN AvsChn = 0;
    AVS_GRP_ATTR_S tmpGrpAttr;
    MB_BLK tmpLutBlk;

    for (AvsChn = 0; AvsChn < s32ChnNum; AvsChn++) {
        s32Ret = RK_MPI_AVS_DisableChn(AvsGrp, AvsChn);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("avs [%d, %d] RK_MPI_AVS_DisableChn failed: %#x!!",
                AvsGrp, AvsChn, s32Ret);

            goto __FAILED;
        }
        RK_LOGV("avs [%d, %d] RK_MPI_AVS_DisableChn already.", AvsGrp, AvsChn);
    }

    s32Ret = RK_MPI_AVS_GetGrpAttr(AvsGrp, &tmpGrpAttr);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("avs [%d, %d] RK_MPI_AVS_GetGrpAttr failed: %#x!!",
                AvsGrp, 0, s32Ret);
        goto __FAILED;
    }
    RK_LOGV("avs [%d, %d] RK_MPI_AVS_GetGrpAttr already.", AvsGrp, 0);

    for (RK_S32 AvsPipe = 0; AvsPipe < tmpGrpAttr.u32PipeNum; AvsPipe++) {
        if (AVS_MODE_BLEND == tmpGrpAttr.enMode &&
            AVS_PARAM_SOURCE_LUT == tmpGrpAttr.stInAttr.enParamSource &&
            tmpGrpAttr.stInAttr.stLUT.pVirAddr[AvsPipe]) {
            tmpLutBlk = RK_MPI_MB_VirAddr2Handle(tmpGrpAttr.stInAttr.stLUT.pVirAddr[AvsPipe]);
            if (tmpLutBlk) {
                s32Ret = RK_MPI_MB_ReleaseMB(tmpLutBlk);
                if (s32Ret != RK_SUCCESS) {
                    RK_LOGE("avs [%d, %d] release Lut Blk %p virAddr %p failed: %#x!!",
                        AvsGrp, AvsPipe, tmpLutBlk,
                        tmpGrpAttr.stInAttr.stLUT.pVirAddr[AvsPipe], s32Ret);
                }
                RK_LOGV("avs [%d, %d] release Lut Blk %p virAddr %p already.",
                        AvsGrp, AvsPipe, tmpLutBlk,
                        tmpGrpAttr.stInAttr.stLUT.pVirAddr[AvsPipe]);
                tmpGrpAttr.stInAttr.stLUT.pVirAddr[AvsPipe] = RK_NULL;
                tmpLutBlk = RK_NULL;
            }
        }
    }

    s32Ret = RK_MPI_AVS_StopGrp(AvsGrp);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("avs [%d, %d] RK_MPI_AVS_StopGrp failed: %#x!!",
                AvsGrp, 0, s32Ret);
        goto __FAILED;
    }
    RK_LOGV("avs [%d, %d] RK_MPI_AVS_StopGrp already.", AvsGrp, 0);

    s32Ret = RK_MPI_AVS_DestroyGrp(AvsGrp);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("avs [%d, %d] RK_MPI_AVS_DestroyGrp failed: %#x!!",
                AvsGrp, 0, s32Ret);
        goto __FAILED;
    }
    RK_LOGV("avs [%d, %d] RK_MPI_AVS_DestroyGrp already.", AvsGrp, 0);

__FAILED:

    return s32Ret;
}

RK_S32 TEST_AVS_ModCreateFramePool(TEST_AVS_CTX_S *pstCtx) {
    RK_S32   s32Ret  = RK_SUCCESS;
    MB_POOL_CONFIG_S     stMbPoolCfg;
    PIC_BUF_ATTR_S       stPicBufAttr;
    MB_PIC_CAL_S         stMbPicCalResult;

    stPicBufAttr.u32Width      = pstCtx->u32SrcWidth;
    stPicBufAttr.u32Height     = pstCtx->u32SrcHeight;
    stPicBufAttr.enCompMode    = pstCtx->enSrcCompressMode;
    stPicBufAttr.enPixelFormat = pstCtx->enSrcPixFormat;

    s32Ret = RK_MPI_CAL_COMM_GetPicBufferSize(&stPicBufAttr, &stMbPicCalResult);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("get picture buffer size failed: %#x", s32Ret);
        return s32Ret;
    }

    for (RK_S32 AvsPipe = 0; AvsPipe < AVS_PIPE_NUM; AvsPipe++) {
        memset(&stMbPoolCfg, 0, sizeof(MB_POOL_CONFIG_S));
        stMbPoolCfg.u64MBSize = stMbPicCalResult.u32MBSize;
        stMbPoolCfg.u32MBCnt  = 2;
        stMbPoolCfg.enAllocType = MB_ALLOC_TYPE_DMA;
        stMbPoolCfg.bPreAlloc = RK_TRUE;
        pstCtx->srcVFramePools[AvsPipe] = RK_MPI_MB_CreatePool(&stMbPoolCfg);
    }

    return s32Ret;
}

RK_S32 TEST_AVS_ModDestroyFramePool(TEST_AVS_CTX_S *pstCtx) {
    RK_S32   s32Ret  = RK_SUCCESS;
    for (RK_S32 AvsPipe = 0; AvsPipe < AVS_PIPE_NUM; AvsPipe++) {
        if (MB_INVALID_POOLID != pstCtx->srcVFramePools[AvsPipe]) {
            RK_MPI_MB_DestroyPool(pstCtx->srcVFramePools[AvsPipe]);
        }
    }

    return s32Ret;
}

RK_S32 TEST_AVS_ModGetFrameFromPool(TEST_AVS_CTX_S *pstCtx,
                                VIDEO_FRAME_INFO_S **pstVideoFrames) {
    RK_S32   s32Ret  = RK_SUCCESS;
    MB_BLK   blk     = RK_NULL;
    RK_U32   u32BufferSize   = 0;
    static RK_S32 s32LoopCnt = 0;
    RK_CHAR  cWritePath[MAX_FILE_PATH_LEN] = {0};

    PIC_BUF_ATTR_S pstBufAttr;
    MB_PIC_CAL_S   stMbPicCalResult;

    pstBufAttr.u32Width      = pstCtx->u32SrcWidth;
    pstBufAttr.u32Height     = pstCtx->u32SrcHeight;
    pstBufAttr.enCompMode    = pstCtx->enSrcCompressMode;
    pstBufAttr.enPixelFormat = pstCtx->enSrcPixFormat;

    s32Ret = RK_MPI_CAL_COMM_GetPicBufferSize(&pstBufAttr, &stMbPicCalResult);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("get picture buffer size failed: %#x", s32Ret);
        return s32Ret;
    }

    u32BufferSize = stMbPicCalResult.u32MBSize;

    for (RK_S32 AvsPipe = 0; AvsPipe < pstCtx->s32PipeNum; AvsPipe++) {
        if (MB_INVALID_POOLID == pstCtx->srcVFramePools[AvsPipe]) {
            RK_LOGE("avs [%d, %d] srcVFramePool unexist %d!",
                    pstCtx->s32GrpIndex, AvsPipe, pstCtx->srcVFramePools[AvsPipe]);
            continue;
        }
        blk = RK_MPI_MB_GetMB(pstCtx->srcVFramePools[AvsPipe], u32BufferSize, RK_TRUE);
        if (!blk) {
            continue;
        }

        pstVideoFrames[AvsPipe]->stVFrame.pMbBlk = blk;
        pstVideoFrames[AvsPipe]->stVFrame.u32Width = pstBufAttr.u32Width;
        pstVideoFrames[AvsPipe]->stVFrame.u32Height = pstBufAttr.u32Height;
        pstVideoFrames[AvsPipe]->stVFrame.u32VirWidth = stMbPicCalResult.u32VirWidth;
        pstVideoFrames[AvsPipe]->stVFrame.u32VirHeight = stMbPicCalResult.u32VirHeight;
        pstVideoFrames[AvsPipe]->stVFrame.enPixelFormat = pstBufAttr.enPixelFormat;
        pstVideoFrames[AvsPipe]->stVFrame.enCompressMode = pstBufAttr.enCompMode;

        if (RK_NULL != pstCtx->srcFilePath) {
            if (COMPRESS_MODE_NONE == pstBufAttr.enCompMode) {
                snprintf(cWritePath, sizeof(cWritePath),
                        "%simage_data/camera%d_%dx%d_nv12.yuv",
                        pstCtx->srcFilePath, AvsPipe,
                        pstBufAttr.u32Width, pstBufAttr.u32Height);
            } else if (COMPRESS_AFBC_16x16 == pstBufAttr.enCompMode) {
                snprintf(cWritePath, sizeof(cWritePath),
                        "%safbc_image_data/camera%d_%dx%d_nv12_afbc.yuv",
                        pstCtx->srcFilePath, AvsPipe,
                        pstBufAttr.u32Width, pstBufAttr.u32Height);
            }

            s32Ret = TEST_COMM_FileReadOneFrame(cWritePath, pstVideoFrames[AvsPipe]);
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("avs [%d, %d] pipe frame %p fread file: %s failed: %#x!",
                        pstCtx->s32GrpIndex, AvsPipe,
                        pstVideoFrames[AvsPipe], cWritePath, s32Ret);
            }
            RK_LOGV("avs [%d, %d] pipe frame %p fread file: %s already.",
                    pstCtx->s32GrpIndex, AvsPipe, pstVideoFrames[AvsPipe], cWritePath);
        } else {
            if (COMPRESS_MODE_NONE == pstBufAttr.enCompMode) {
                s32Ret = TEST_COMM_FillImage(
                            (RK_U8 *)RK_MPI_MB_Handle2VirAddr(pstVideoFrames[AvsPipe]->stVFrame.pMbBlk),
                            pstBufAttr.u32Width,
                            pstBufAttr.u32Height,
                            RK_MPI_CAL_COMM_GetHorStride(pstVideoFrames[AvsPipe]->stVFrame.u32Width,
                                    pstVideoFrames[AvsPipe]->stVFrame.enPixelFormat),
                            pstVideoFrames[AvsPipe]->stVFrame.u32VirHeight,
                            pstVideoFrames[AvsPipe]->stVFrame.enPixelFormat,
                            s32LoopCnt + AvsPipe);
                if (s32Ret != RK_SUCCESS) {
                    RK_LOGE("fill COMPRESS_MODE_NONE data into pipe %d buf %p failed: %#x!",
                            AvsPipe, pstVideoFrames[AvsPipe], s32Ret);
                }
                s32LoopCnt++;
            } else if (COMPRESS_AFBC_16x16 == pstBufAttr.enCompMode) {
                RK_LOGE("unsupport fill COMPRESS_AFBC_16x16 data into buf generated by CPU!");
            }
        }
        RK_MPI_SYS_MmzFlushCache(pstVideoFrames[AvsPipe]->stVFrame.pMbBlk, RK_FALSE);
    }

    return s32Ret;
}

RK_S32 TEST_AVS_ModReleaseFrameToPool(TEST_AVS_CTX_S *pstCtx,
                                VIDEO_FRAME_INFO_S **pstVideoFrames) {
    RK_S32   s32Ret  = RK_SUCCESS;
    for (RK_S32 AvsPipe = 0; AvsPipe < AVS_PIPE_NUM; AvsPipe++) {
        if (pstVideoFrames[AvsPipe] && pstVideoFrames[AvsPipe]->stVFrame.pMbBlk) {
            s32Ret = RK_MPI_MB_ReleaseMB(pstVideoFrames[AvsPipe]->stVFrame.pMbBlk);
            pstVideoFrames[AvsPipe]->stVFrame.pMbBlk = RK_NULL;
        }
    }

    return s32Ret;
}


RK_S32 TEST_AVS_ModCreateFrame(TEST_AVS_CTX_S *pstCtx,
                               VIDEO_FRAME_INFO_S **pstVideoFrames) {
    RK_S32   s32Ret  = RK_SUCCESS;
    AVS_PIPE AvsPipe = 0;
    static RK_S32 s32LoopCnt = 0;
    RK_CHAR  cWritePath[MAX_FILE_PATH_LEN] = {0};
    PIC_BUF_ATTR_S pstBufAttr;

    pstBufAttr.u32Width      = pstCtx->u32SrcWidth;
    pstBufAttr.u32Height     = pstCtx->u32SrcHeight;
    pstBufAttr.enCompMode    = pstCtx->enSrcCompressMode;
    pstBufAttr.enPixelFormat = pstCtx->enSrcPixFormat;

    for (; AvsPipe < pstCtx->s32PipeNum; AvsPipe++) {
        s32Ret = TEST_SYS_CreateVideoFrame(&pstBufAttr, pstVideoFrames[AvsPipe]);
        if (s32Ret != RK_SUCCESS) {
            goto __FAILED;
        }
        RK_LOGV("avs [%d, %d] create pipe frame %p already.",
                pstCtx->s32GrpIndex, AvsPipe, pstVideoFrames[AvsPipe]);

        if (RK_NULL != pstCtx->srcFilePath) {
            if (COMPRESS_MODE_NONE == pstBufAttr.enCompMode) {
                snprintf(cWritePath, sizeof(cWritePath),
                        "%simage_data/camera%d_%dx%d_nv12.yuv",
                        pstCtx->srcFilePath, AvsPipe,
                        pstBufAttr.u32Width, pstBufAttr.u32Height);
            } else if (COMPRESS_AFBC_16x16 == pstBufAttr.enCompMode) {
                snprintf(cWritePath, sizeof(cWritePath),
                        "%safbc_image_data/camera%d_%dx%d_nv12_afbc.yuv",
                        pstCtx->srcFilePath, AvsPipe,
                        pstBufAttr.u32Width, pstBufAttr.u32Height);
            }
            RK_LOGV("avs [%d, %d] pipe frame %p fread file: %s.",
                    pstCtx->s32GrpIndex, AvsPipe, pstVideoFrames[AvsPipe], cWritePath);

            s32Ret = TEST_COMM_FileReadOneFrame(cWritePath, pstVideoFrames[AvsPipe]);
            if (s32Ret != RK_SUCCESS) {
                goto __FAILED;
            }
        } else {
            if (COMPRESS_MODE_NONE == pstBufAttr.enCompMode) {
                s32Ret = TEST_COMM_FillImage(
                            (RK_U8 *)RK_MPI_MB_Handle2VirAddr(pstVideoFrames[AvsPipe]->stVFrame.pMbBlk),
                            pstBufAttr.u32Width,
                            pstBufAttr.u32Height,
                            RK_MPI_CAL_COMM_GetHorStride(pstVideoFrames[AvsPipe]->stVFrame.u32Width,
                                    pstVideoFrames[AvsPipe]->stVFrame.enPixelFormat),
                            pstVideoFrames[AvsPipe]->stVFrame.u32VirHeight,
                            pstVideoFrames[AvsPipe]->stVFrame.enPixelFormat,
                            s32LoopCnt + AvsPipe);
                if (s32Ret != RK_SUCCESS) {
                    RK_LOGE("fill COMPRESS_MODE_NONE data into pipe %d buf %p failed: %#x!",
                            AvsPipe, pstVideoFrames[AvsPipe], s32Ret);

                    goto __FAILED;
                }
                s32LoopCnt++;
            } else if (COMPRESS_AFBC_16x16 == pstBufAttr.enCompMode) {
                RK_LOGE("unsupport fill COMPRESS_AFBC_16x16 data into buf generated by CPU!");
            }
        }
        RK_MPI_SYS_MmzFlushCache(pstVideoFrames[AvsPipe]->stVFrame.pMbBlk, RK_FALSE);
    }

    return s32Ret;

__FAILED:
    for (; AvsPipe >= 0; AvsPipe--) {
        RK_MPI_MB_ReleaseMB(pstVideoFrames[AvsPipe]->stVFrame.pMbBlk);
    }

    return s32Ret;
}

RK_S32 TEST_AVS_ModSendFrame(AVS_GRP AvsGrp,
                             RK_U32 s32PipeNum,
                             VIDEO_FRAME_INFO_S **pstVideoFrames) {
    RK_S32 s32Ret    = RK_SUCCESS;
    AVS_PIPE AvsPipe = 0;

    for (; AvsPipe < s32PipeNum; AvsPipe++) {
        s32Ret = RK_MPI_AVS_SendPipeFrame(AvsGrp, AvsPipe, pstVideoFrames[AvsPipe], -1);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("avs [%d, %d] RK_MPI_AVS_SendPipeFrame %p failed: %#x!",
                    AvsGrp, AvsPipe, pstVideoFrames[AvsPipe]->stVFrame.pMbBlk, s32Ret);
        }
    }

    return s32Ret;
}

RK_S32 TEST_AVS_ModGetChnFrame(AVS_GRP AvsGrp,
                               RK_U32 s32ChnNum,
                               VIDEO_FRAME_INFO_S **pstVideoFrames) {
    RK_S32 s32Ret  = RK_SUCCESS;
    AVS_CHN AvsChn = 0;

    for (; AvsChn < s32ChnNum; AvsChn++) {
        s32Ret = RK_MPI_AVS_GetChnFrame(AvsGrp, AvsChn,
                                        pstVideoFrames[AvsChn], 40);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("avs [%d, %d] RK_MPI_AVS_GetChnFrame failed: %#x!",
                    AvsGrp, AvsChn, s32Ret);
        }
    }

    return s32Ret;
}

RK_S32 TEST_AVS_ModSaveChnFrame(AVS_GRP AvsGrp,
                                RK_U32 s32ChnNum,
                                const RK_CHAR *dstFilePath,
                                VIDEO_FRAME_INFO_S **pstVideoFrames) {
    RK_S32 s32Ret  = RK_SUCCESS;
    AVS_CHN AvsChn = 0;

    for (; AvsChn < s32ChnNum; AvsChn++) {
        if (!pstVideoFrames[AvsChn]) {
            continue;
        }

        RK_CHAR pWritePath[MAX_FILE_PATH_LEN]       = {RK_NULL};
        snprintf(pWritePath, sizeof(pWritePath), "%schn_out_%dx%d_%d_%d_%s.bin",
                    dstFilePath, pstVideoFrames[AvsChn]->stVFrame.u32Width,
                    pstVideoFrames[AvsChn]->stVFrame.u32Height, AvsGrp, AvsChn,
                    pstVideoFrames[AvsChn]->stVFrame.enCompressMode ? "afbc": "none");

        s32Ret = TEST_COMM_FileWriteOneFrame(pWritePath, pstVideoFrames[AvsChn]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("avs [%d, %d] save chn frame %p to file %s failed: %#x!",
                    AvsGrp, AvsChn, pstVideoFrames[AvsChn],
                    pWritePath, s32Ret);
        }
    }

    return s32Ret;
}

RK_S32 TEST_AVS_ModReleaseChnFrame(AVS_GRP AvsGrp,
                                   RK_U32 s32ChnNum,
                                   VIDEO_FRAME_INFO_S **pstVideoFrames) {
    RK_S32 s32Ret  = RK_SUCCESS;
    AVS_CHN AvsChn = 0;

    for (; AvsChn < s32ChnNum; AvsChn++) {
        s32Ret = RK_MPI_AVS_ReleaseChnFrame(AvsGrp, AvsChn,
                                        pstVideoFrames[AvsChn]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("avs [%d, %d] RK_MPI_AVS_ReleaseChnFrame failed: %#x!",
                    AvsGrp, AvsChn, s32Ret);
        }
    }

    return s32Ret;
}

#include <unistd.h>

RK_S32 TEST_AVS_UnitTest(TEST_AVS_CTX_S *pstCtx) {
    RK_S32      s32Ret = RK_SUCCESS;

    VIDEO_FRAME_INFO_S **pstPipeFrames;
    VIDEO_FRAME_INFO_S **pstChnFrames;

    pstPipeFrames = reinterpret_cast<VIDEO_FRAME_INFO_S **>(
                        malloc(sizeof(VIDEO_FRAME_INFO_S *) * AVS_PIPE_NUM));
    pstChnFrames = reinterpret_cast<VIDEO_FRAME_INFO_S **>(
                        malloc(sizeof(VIDEO_FRAME_INFO_S *) * AVS_MAX_CHN_NUM));
    for (RK_S32 i = 0; i < AVS_PIPE_NUM; i++) {
        pstPipeFrames[i] = reinterpret_cast<VIDEO_FRAME_INFO_S *>(malloc(sizeof(VIDEO_FRAME_INFO_S)));
        memset(pstPipeFrames[i], 0, sizeof(VIDEO_FRAME_INFO_S));
    }
    for (RK_S32 i = 0; i < AVS_MAX_CHN_NUM; i++) {
        pstChnFrames[i] = reinterpret_cast<VIDEO_FRAME_INFO_S *>(malloc(sizeof(VIDEO_FRAME_INFO_S)));
        memset(pstChnFrames[i], 0, sizeof(VIDEO_FRAME_INFO_S));
    }

    s32Ret = TEST_AVS_ModCreateFrame(pstCtx, pstPipeFrames);
    if (s32Ret != RK_SUCCESS) {
        goto __FREE_PIPE_FRAME;
    }

    s32Ret = TEST_AVS_ModInit(pstCtx);
    if (s32Ret != RK_SUCCESS) {
        goto __FREE_PIPE_FRAME;
    }

    for (RK_S32 i = 0; i < pstCtx->s32LoopCount; i++) {
        s32Ret = TEST_AVS_ModSendFrame(pstCtx->s32GrpIndex, pstCtx->s32PipeNum, pstPipeFrames);
        if (s32Ret != RK_SUCCESS) {
            continue;
        }

        s32Ret = TEST_AVS_ModGetChnFrame(pstCtx->s32GrpIndex, pstCtx->s32ChnNum, pstChnFrames);
        if (s32Ret != RK_SUCCESS) {
            continue;
        }

        if (pstCtx->dstFilePath != RK_NULL) {
            s32Ret = TEST_AVS_ModSaveChnFrame(pstCtx->s32GrpIndex, pstCtx->s32ChnNum,
                                              pstCtx->dstFilePath, pstChnFrames);
            if (s32Ret != RK_SUCCESS) {
                goto __FAILED;
            }
        }

        s32Ret = TEST_AVS_ModReleaseChnFrame(pstCtx->s32GrpIndex, pstCtx->s32ChnNum, pstChnFrames);
        if (s32Ret != RK_SUCCESS) {
            goto __FAILED;
        }
        usleep(40 * 1000);
    }

__FAILED:
    s32Ret = TEST_AVS_ModDeInit(pstCtx);

__FREE_PIPE_FRAME:
    for (RK_S32 i = 0; i < AVS_PIPE_NUM; i++) {
        if (i < pstCtx->s32PipeNum) {
            if (pstPipeFrames[i] &&
                pstPipeFrames[i]->stVFrame.pMbBlk) {
                RK_MPI_MB_ReleaseMB(pstPipeFrames[i]->stVFrame.pMbBlk);
            }
        }
        RK_SAFE_FREE(pstPipeFrames[i]);
    }

    for (RK_S32 i = 0; i < AVS_MAX_CHN_NUM; i++)
        RK_SAFE_FREE(pstChnFrames[i]);

    RK_SAFE_FREE(pstPipeFrames);
    RK_SAFE_FREE(pstChnFrames);

    return s32Ret;
}

RK_S32 TEST_AVS_InitAttr(TEST_AVS_CTX_S *pstCtx,
                       AVS_GRP_ATTR_S *pstAvsGrpAttr,
                       AVS_CHN_ATTR_S *pstAvsChnAttrs) {
    RK_S32 s32Ret = RK_SUCCESS;

    if (AVS_MODE_BLEND == pstCtx->enAvsWorkMode) {
        if (AVS_PARAM_SOURCE_LUT == pstCtx->enParamSource) {
            s32Ret = TEST_AVS_InitAttr_Blend_Lut(pstCtx, pstAvsGrpAttr, pstAvsChnAttrs);
        } else if (AVS_PARAM_SOURCE_CALIB == pstCtx->enParamSource) {
            s32Ret = TEST_AVS_InitAttr_Blend_Calib(pstCtx, pstAvsGrpAttr, pstAvsChnAttrs);
        }
    } else if (pstCtx->enAvsWorkMode > AVS_MODE_BLEND && pstCtx->enAvsWorkMode < AVS_MODE_BUTT) {
        s32Ret = TEST_AVS_InitAttr_NoBlend(pstCtx, pstAvsGrpAttr, pstAvsChnAttrs);
    }

    if (RK_TRUE == pstCtx->bGrpExSize) {
        pstAvsGrpAttr->stOutAttr.stSize.u32Width  = pstCtx->u32GrpWidth;
        pstAvsGrpAttr->stOutAttr.stSize.u32Height = pstCtx->u32GrpHeight;
    }

    pstAvsGrpAttr->stInAttr.stSize.u32Width  = pstCtx->u32SrcWidth;
    pstAvsGrpAttr->stInAttr.stSize.u32Height = pstCtx->u32SrcHeight;

    return s32Ret;
}

RK_S32 TEST_AVS_InitAttr_Blend_Lut(TEST_AVS_CTX_S *pstCtx,
                       AVS_GRP_ATTR_S *pstAvsGrpAttr,
                       AVS_CHN_ATTR_S *pstAvsChnAttrs) {
    RK_S32 s32Ret = RK_SUCCESS;
    RK_VOID   **pLUTVirAddr = RK_NULL;

    if (!pstCtx->lutFilePath) {
        return RK_ERR_NULL_PTR;
    }

    memset(pstAvsGrpAttr, 0, sizeof(AVS_GRP_ATTR_S));
    memset(pstAvsChnAttrs, 0, sizeof(AVS_CHN_ATTR_S) * AVS_MAX_CHN_NUM);

    s32Ret = TEST_AVS_MallocLutBuf(pstCtx, &pLUTVirAddr);
    if (RK_SUCCESS != s32Ret) {
        return s32Ret;
    }

    pstAvsGrpAttr->enMode                           = pstCtx->enAvsWorkMode;
    pstAvsGrpAttr->u32PipeNum                       = pstCtx->s32PipeNum;
    pstAvsGrpAttr->bSyncPipe                        = pstCtx->bFrameSync;

    pstAvsGrpAttr->stInAttr.enParamSource           = pstCtx->enParamSource;
    pstAvsGrpAttr->stInAttr.stLUT.enAccuracy        = pstCtx->enLutAccuracy;
    pstAvsGrpAttr->stInAttr.stLUT.enFuseWidth       = pstCtx->enLutFuseWidth;
    pstAvsGrpAttr->stInAttr.stLUT.stLutStep.enStepX = pstCtx->stLutStep.enStepX;
    pstAvsGrpAttr->stInAttr.stLUT.stLutStep.enStepY = pstCtx->stLutStep.enStepY;
    for (RK_S32 i = 0; i < pstCtx->s32PipeNum; i++) {
        pstAvsGrpAttr->stInAttr.stLUT.pVirAddr[i] = pLUTVirAddr[i];
    }

    pstAvsGrpAttr->stOutAttr.enPrjMode              = pstCtx->enAvsProjMode;
    pstAvsGrpAttr->stOutAttr.stCenter.s32X          = pstCtx->stCenter.s32X;
    pstAvsGrpAttr->stOutAttr.stCenter.s32Y          = pstCtx->stCenter.s32Y;
    pstAvsGrpAttr->stOutAttr.stFOV.u32FOVX          = pstCtx->stFov.u32FOVX;
    pstAvsGrpAttr->stOutAttr.stFOV.u32FOVY          = pstCtx->stFov.u32FOVY;
    pstAvsGrpAttr->stOutAttr.stORIRotation.s32Roll  = pstCtx->stOriRotation.s32Roll;
    pstAvsGrpAttr->stOutAttr.stORIRotation.s32Pitch = pstCtx->stOriRotation.s32Pitch;
    pstAvsGrpAttr->stOutAttr.stORIRotation.s32Yaw   = pstCtx->stOriRotation.s32Yaw;
    pstAvsGrpAttr->stOutAttr.stRotation.s32Roll     = pstCtx->stRotation.s32Roll;
    pstAvsGrpAttr->stOutAttr.stRotation.s32Pitch    = pstCtx->stRotation.s32Pitch;
    pstAvsGrpAttr->stOutAttr.stRotation.s32Yaw      = pstCtx->stRotation.s32Yaw;
    pstAvsGrpAttr->stFrameRate.s32SrcFrameRate      = pstCtx->s32SrcGrpRate;
    pstAvsGrpAttr->stFrameRate.s32DstFrameRate      = pstCtx->s32DstGrpRate;

    for (RK_S32 i = 0; i < AVS_MAX_CHN_NUM; i++) {
        if (0 == i) {
            pstAvsChnAttrs[i].u32Width                = pstCtx->u32DstWidth;
            pstAvsChnAttrs[i].u32Height               = pstCtx->u32DstHeight;
            pstAvsChnAttrs[i].enCompressMode          = pstCtx->enDstCompressMode;
        } else {
            pstAvsChnAttrs[i].u32Width                = RK_ALIGN(pstCtx->u32DstWidth / 2, 16);
            pstAvsChnAttrs[i].u32Height               = RK_ALIGN(pstCtx->u32DstHeight / 2, 2);
            pstAvsChnAttrs[i].enCompressMode          = COMPRESS_MODE_NONE;
        }
        pstAvsChnAttrs[i].enDynamicRange              = DYNAMIC_RANGE_SDR8;
        pstAvsChnAttrs[i].u32Depth                    = pstCtx->u32ChnDepth;
        pstAvsChnAttrs[i].stFrameRate.s32SrcFrameRate = pstCtx->s32SrcChnRate;
        pstAvsChnAttrs[i].stFrameRate.s32DstFrameRate = pstCtx->s32DstChnRate;
        pstAvsChnAttrs[i].u32FrameBufCnt              = pstCtx->u32FrameBufCnt;
    }

    return s32Ret;
}

RK_S32 TEST_AVS_InitAttr_Blend_Calib(TEST_AVS_CTX_S *pstCtx,
                       AVS_GRP_ATTR_S *pstAvsGrpAttr,
                       AVS_CHN_ATTR_S *pstAvsChnAttrs) {
    RK_S32 s32Ret = RK_SUCCESS;

    memset(pstAvsGrpAttr, 0, sizeof(AVS_GRP_ATTR_S));
    memset(pstAvsChnAttrs, 0, sizeof(AVS_CHN_ATTR_S) * AVS_MAX_CHN_NUM);

    pstAvsGrpAttr->enMode                           = pstCtx->enAvsWorkMode;
    pstAvsGrpAttr->u32PipeNum                       = pstCtx->s32PipeNum;
    pstAvsGrpAttr->bSyncPipe                        = pstCtx->bFrameSync;

    pstAvsGrpAttr->stInAttr.enParamSource           = pstCtx->enParamSource;
    if (pstCtx->calibFileName) {
        pstAvsGrpAttr->stInAttr.stCalib.pCalibFilePath = pstCtx->calibFileName;
    }
    if (pstCtx->meshFilePath) {
        pstAvsGrpAttr->stInAttr.stCalib.pMeshAlphaPath = pstCtx->meshFilePath;
    }

    pstAvsGrpAttr->stOutAttr.enPrjMode              = pstCtx->enAvsProjMode;
    pstAvsGrpAttr->stOutAttr.stCenter.s32X          = pstCtx->stCenter.s32X;
    pstAvsGrpAttr->stOutAttr.stCenter.s32Y          = pstCtx->stCenter.s32Y;
    pstAvsGrpAttr->stOutAttr.stFOV.u32FOVX          = pstCtx->stFov.u32FOVX;
    pstAvsGrpAttr->stOutAttr.stFOV.u32FOVY          = pstCtx->stFov.u32FOVY;
    pstAvsGrpAttr->stOutAttr.stORIRotation.s32Roll  = pstCtx->stOriRotation.s32Roll;
    pstAvsGrpAttr->stOutAttr.stORIRotation.s32Pitch = pstCtx->stOriRotation.s32Pitch;
    pstAvsGrpAttr->stOutAttr.stORIRotation.s32Yaw   = pstCtx->stOriRotation.s32Yaw;
    pstAvsGrpAttr->stOutAttr.stRotation.s32Roll     = pstCtx->stRotation.s32Roll;
    pstAvsGrpAttr->stOutAttr.stRotation.s32Pitch    = pstCtx->stRotation.s32Pitch;
    pstAvsGrpAttr->stOutAttr.stRotation.s32Yaw      = pstCtx->stRotation.s32Yaw;
    pstAvsGrpAttr->stOutAttr.fDistance              = pstCtx->fDistance;
    pstAvsGrpAttr->stFrameRate.s32SrcFrameRate      = pstCtx->s32SrcGrpRate;
    pstAvsGrpAttr->stFrameRate.s32DstFrameRate      = pstCtx->s32DstGrpRate;

    for (RK_S32 i = 0; i < pstCtx->s32ChnNum; i++) {
        if (0 == i) {
            pstAvsChnAttrs[i].u32Width                = pstCtx->u32DstWidth;
            pstAvsChnAttrs[i].u32Height               = pstCtx->u32DstHeight;
            pstAvsChnAttrs[i].enCompressMode          = pstCtx->enDstCompressMode;
        } else {
            pstAvsChnAttrs[i].u32Width                = RK_ALIGN(pstCtx->u32DstWidth / 2, 16);
            pstAvsChnAttrs[i].u32Height               = RK_ALIGN(pstCtx->u32DstHeight / 2, 2);
            pstAvsChnAttrs[i].enCompressMode          = COMPRESS_MODE_NONE;
        }
        pstAvsChnAttrs[i].enCompressMode              = pstCtx->enDstCompressMode;
        pstAvsChnAttrs[i].enDynamicRange              = DYNAMIC_RANGE_SDR8;
        pstAvsChnAttrs[i].u32Depth                    = pstCtx->u32ChnDepth;
        pstAvsChnAttrs[i].stFrameRate.s32SrcFrameRate = pstCtx->s32SrcChnRate;
        pstAvsChnAttrs[i].stFrameRate.s32DstFrameRate = pstCtx->s32DstChnRate;
        pstAvsChnAttrs[i].u32FrameBufCnt              = pstCtx->u32FrameBufCnt;
    }

    return s32Ret;
}

RK_S32 TEST_AVS_InitAttr_NoBlend(TEST_AVS_CTX_S *pstCtx,
                       AVS_GRP_ATTR_S *pstAvsGrpAttr,
                       AVS_CHN_ATTR_S *pstAvsChnAttrs) {
    RK_S32 s32Ret = RK_SUCCESS;

    memset(pstAvsGrpAttr, 0, sizeof(AVS_GRP_ATTR_S));
    memset(pstAvsChnAttrs, 0, sizeof(AVS_CHN_ATTR_S) * AVS_MAX_CHN_NUM);

    pstAvsGrpAttr->enMode                           = pstCtx->enAvsWorkMode;
    pstAvsGrpAttr->u32PipeNum                       = pstCtx->s32PipeNum;
    pstAvsGrpAttr->bSyncPipe                        = pstCtx->bFrameSync;

    pstAvsGrpAttr->stFrameRate.s32SrcFrameRate      = pstCtx->s32SrcGrpRate;
    pstAvsGrpAttr->stFrameRate.s32DstFrameRate      = pstCtx->s32DstGrpRate;

    for (RK_S32 i = 0; i < pstCtx->s32ChnNum; i++) {
        if (0 == i) {
            pstAvsChnAttrs[i].u32Width                = pstCtx->u32DstWidth;
            pstAvsChnAttrs[i].u32Height               = pstCtx->u32DstHeight;
            pstAvsChnAttrs[i].enCompressMode          = pstCtx->enDstCompressMode;
        } else {
            pstAvsChnAttrs[i].u32Width                = RK_ALIGN(pstCtx->u32DstWidth / 2, 16);
            pstAvsChnAttrs[i].u32Height               = RK_ALIGN(pstCtx->u32DstHeight / 2, 2);
            pstAvsChnAttrs[i].enCompressMode          = COMPRESS_MODE_NONE;
        }
        pstAvsChnAttrs[i].enDynamicRange              = DYNAMIC_RANGE_SDR8;
        pstAvsChnAttrs[i].u32Depth                    = pstCtx->u32ChnDepth;
        pstAvsChnAttrs[i].stFrameRate.s32SrcFrameRate = pstCtx->s32SrcChnRate;
        pstAvsChnAttrs[i].stFrameRate.s32DstFrameRate = pstCtx->s32DstChnRate;
        pstAvsChnAttrs[i].u32FrameBufCnt              = pstCtx->u32FrameBufCnt;
    }

    return s32Ret;
}

RK_S32 TEST_AVS_ModInit(TEST_AVS_CTX_S *pstCtx) {
    RK_S32           s32Ret = RK_SUCCESS;
    MB_BLK           srcBlk = MB_INVALID_HANDLE;
    AVS_GRP_ATTR_S stAvsGrpAttr;
    AVS_CHN_ATTR_S stAvsChnAttr[AVS_MAX_CHN_NUM];

    s32Ret = TEST_AVS_InitAttr(pstCtx, &stAvsGrpAttr, stAvsChnAttr);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("avs [%d, %d] TEST_AVS_InitAttr failed: %#x!",
                pstCtx->s32GrpIndex, 0, s32Ret);
        return s32Ret;
    }

    if (pstCtx->bGetFinalLut) {
        s32Ret = TEST_AVS_GetFinalLut(pstCtx->s32GrpIndex, &stAvsGrpAttr);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("avs [%d, %d] TEST_AVS_GetFinalLut failed: %#x!",
                    pstCtx->s32GrpIndex, 0, s32Ret);
            goto __FAILED;
        }
    }

    s32Ret = TEST_AVS_Start(pstCtx->s32GrpIndex, pstCtx->s32ChnNum,
                             &stAvsGrpAttr, stAvsChnAttr);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("avs [%d, %d] TEST_AVS_Start failed: %#x!",
                pstCtx->s32GrpIndex, 0, s32Ret);
        goto __FAILED;
    }

    return RK_SUCCESS;

__FAILED:
    return s32Ret;
}

RK_S32 TEST_AVS_ModDeInit(TEST_AVS_CTX_S *pstCtx) {
    RK_S32 s32Ret = RK_SUCCESS;

    s32Ret = TEST_AVS_Stop(pstCtx->s32GrpIndex, pstCtx->s32ChnNum);

    return s32Ret;
}

RK_U32 TEST_AVS_GetFileLen(const RK_CHAR* File) {
    FILE* FILE;
    RK_U32 u32FileLen;
    RK_S32 s32Ret;

    FILE = fopen(File, "rb");
    if (NULL != FILE) {
        s32Ret = fseek(FILE, 0L, SEEK_END);
        if (0 != s32Ret) {
            RK_LOGE("fseek err!");
            fclose(FILE);
            return 0;
        }

        u32FileLen = ftell(FILE);

        s32Ret = fseek(FILE, 0L, SEEK_SET);
        if (0 != s32Ret) {
            RK_LOGE("fseek err!");
            fclose(FILE);
            return 0;
        }

        fclose(FILE);
    } else {
        RK_LOGE("open file %s fail!", File);
        u32FileLen = 0;
    }

    return u32FileLen;
}

RK_S32 TEST_AVS_LoadFile(const RK_CHAR* File, RK_VOID* Addr, RK_U32 u32Size) {
    FILE* FILE;
    RK_U32 u32ReadBytes;

    FILE = fopen(File, "rb");
    if (FILE != NULL) {
        u32ReadBytes = fread(Addr, u32Size, 1, FILE);
        fclose(FILE);
    } else {
        RK_LOGE("open file of %s failed!", File);
        return RK_FAILURE;
    }

    return RK_SUCCESS;
}

RK_S32 TEST_AVS_MallocLutBuf(TEST_AVS_CTX_S *pstCtx, RK_VOID ***lutBufsVirAddr) {
    RK_S32     s32Ret = RK_SUCCESS;
    RK_U32     u32LUTSize = 0;
    RK_CHAR    pLUTPath[AVS_PIPE_NUM][MAX_FILE_PATH_LEN] = {RK_NULL};
    MB_BLK     pLUTBlk[AVS_PIPE_NUM]            = {RK_NULL};
    RK_VOID  **pLUTVirAddr = reinterpret_cast<RK_VOID **>(
                        malloc(sizeof(RK_VOID*) * AVS_PIPE_NUM));

    for (RK_S32 i = 0; i < pstCtx->s32PipeNum; i++) {
        snprintf(pLUTPath[i], sizeof(pLUTPath[i]), "%s%s%d%s",
                    pstCtx->lutFilePath, "rk_ps_lut_", i, ".bin");

        u32LUTSize = TEST_AVS_GetFileLen(pLUTPath[i]);
        if (0 == u32LUTSize) {
            RK_LOGE("avs [%d, %d] open lut file %s failed!",
                    pstCtx->s32GrpIndex, i, pLUTPath[i]);
            s32Ret = RK_FAILURE;

            goto __FREE_LUT;
        }

        u32LUTSize = RK_ALIGN_256(u32LUTSize);

        s32Ret = RK_MPI_SYS_MmzAllocEx(&pLUTBlk[i], NULL, NULL,
                                    u32LUTSize,
                                    MB_REMAP_MODE_CACHED);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("avs [%d, %d] alloc LUT buf failed with %#x!",
                    pstCtx->s32GrpIndex, i, s32Ret);
            goto __FREE_LUT;
        }
        pLUTVirAddr[i] = RK_MPI_MMZ_Handle2VirAddr(pLUTBlk[i]);

        RK_LOGV("avs [%d, %d] pipe lut buf size %d viraddr %p fread lut file: %s.",
                    pstCtx->s32GrpIndex, i, u32LUTSize, pLUTVirAddr[i], pLUTPath[i]);
        s32Ret = TEST_AVS_LoadFile(pLUTPath[i], pLUTVirAddr[i], u32LUTSize);
        if (RK_SUCCESS != s32Ret) {
            goto __FREE_LUT;
        }
    }

    *lutBufsVirAddr = pLUTVirAddr;

    return s32Ret;

__FREE_LUT:

    for (RK_S32 i = 0; i < AVS_PIPE_NUM; i++) {
        RK_SAFE_FREE(pLUTVirAddr[i]);
    }
    RK_SAFE_FREE(pLUTVirAddr);

    return s32Ret;
}

RK_S32 TEST_AVS_FreeLutBuf(AVS_GRP_ATTR_S *pstAvsGrpAttr) {
    RK_S32 s32Ret = RK_SUCCESS;
    MB_BLK pLUTBlk = RK_NULL;

    for (RK_S32 i = 0; i < AVS_PIPE_NUM; i++) {
        pLUTBlk = RK_MPI_MMZ_VirAddr2Handle(pstAvsGrpAttr->stInAttr.stLUT.pVirAddr[i]);
        if (RK_NULL != pLUTBlk) {
            s32Ret = RK_MPI_SYS_MmzFree(pLUTBlk);
            if (RK_SUCCESS != s32Ret) {
                RK_LOGE("pipe %d Lut buf RK_MPI_SYS_MmzFree failed: %#x!", i, s32Ret);
            }
            pLUTBlk = RK_NULL;
        }
    }

    return s32Ret;
}

RK_S32 TEST_AVS_ComChnSetScale(TEST_AVS_CTX_S *pstCtx) {
    RK_S32 s32Ret = RK_SUCCESS;
    AVS_CHN_ATTR_S tmpChnAttr;

    for (RK_S32 AvsChn = 0; AvsChn < pstCtx->s32ChnNum; AvsChn++) {
        s32Ret = RK_MPI_AVS_GetChnAttr(pstCtx->s32GrpIndex, AvsChn, &tmpChnAttr);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("avs [%d, %d] RK_MPI_AVS_GetChnAttr failed: %#x!",
                    pstCtx->s32GrpIndex, AvsChn, s32Ret);
            return s32Ret;
        }

        tmpChnAttr.u32Width = pstCtx->u32DstWidth;
        tmpChnAttr.u32Height = pstCtx->u32DstHeight;
        RK_LOGV("set new chn attr u32Width: %d, u32Height: %d.",
                tmpChnAttr.u32Width, tmpChnAttr.u32Height);

        s32Ret = RK_MPI_AVS_SetChnAttr(pstCtx->s32GrpIndex, AvsChn, &tmpChnAttr);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("avs [%d, %d] RK_MPI_AVS_SetChnAttr failed: %#x!",
                    pstCtx->s32GrpIndex, AvsChn, s32Ret);
            return s32Ret;
        }
    }

    return s32Ret;
}

RK_S32 TEST_AVS_ComGrpChnSetScale(TEST_AVS_CTX_S *pstCtx) {
    RK_S32 s32Ret = RK_SUCCESS;
    AVS_GRP_ATTR_S tmpGrpAttr;
    AVS_CHN_ATTR_S tmpChnAttr;

    s32Ret = RK_MPI_AVS_GetGrpAttr(pstCtx->s32GrpIndex, &tmpGrpAttr);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("avs [%d] RK_MPI_AVS_GetGrpAttr failed: %#x!",
                    pstCtx->s32GrpIndex, s32Ret);
        return s32Ret;
    }

    tmpGrpAttr.stOutAttr.stCenter.s32X = pstCtx->stCenter.s32X;
    tmpGrpAttr.stOutAttr.stCenter.s32Y = pstCtx->stCenter.s32Y;
    RK_LOGV("set new grp attr centerX: %d, centerY: %d.",
            tmpGrpAttr.stOutAttr.stCenter.s32X, tmpGrpAttr.stOutAttr.stCenter.s32Y);

    s32Ret = RK_MPI_AVS_SetGrpAttr(pstCtx->s32GrpIndex, &tmpGrpAttr);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("avs [%d] RK_MPI_AVS_SetGrpAttr failed: %#x!",
                    pstCtx->s32GrpIndex, s32Ret);
        return s32Ret;
    }

    for (RK_S32 AvsChn = 0; AvsChn < pstCtx->s32ChnNum; AvsChn++) {
        s32Ret = RK_MPI_AVS_GetChnAttr(pstCtx->s32GrpIndex, AvsChn, &tmpChnAttr);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("avs [%d, %d] RK_MPI_AVS_GetChnAttr failed: %#x!",
                    pstCtx->s32GrpIndex, AvsChn, s32Ret);
            return s32Ret;
        }

        tmpChnAttr.u32Width = pstCtx->u32DstWidth;
        tmpChnAttr.u32Height = pstCtx->u32DstHeight;
        RK_LOGV("set new chn attr u32Width: %d, u32Height: %d.",
                tmpChnAttr.u32Width, tmpChnAttr.u32Height);

        s32Ret = RK_MPI_AVS_SetChnAttr(pstCtx->s32GrpIndex, AvsChn, &tmpChnAttr);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("avs [%d, %d] RK_MPI_AVS_SetChnAttr failed: %#x!",
                    pstCtx->s32GrpIndex, AvsChn, s32Ret);
            return s32Ret;
        }
    }

    return s32Ret;
}

RK_S32 TEST_AVS_GetFinalLut(AVS_GRP AvsGrp, AVS_GRP_ATTR_S *pstAvsGrpAttr) {
    RK_S32 s32Ret = RK_SUCCESS;

    s32Ret = RK_MPI_AVS_CreateGrp(AvsGrp, pstAvsGrpAttr);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("avs [%d, %d] RK_MPI_AVS_CreateGrp failed: %#x!",
                AvsGrp, 0, s32Ret);
        return s32Ret;
    }
    RK_LOGV("avs [%d, %d] RK_MPI_AVS_CreateGrp already.", AvsGrp, 0);

    RK_U16 *u16LdchData[AVS_PIPE_NUM] = {RK_NULL};
    AVS_FINAL_LUT_S pstFinalLut;

    MB_EXT_CONFIG_S stMbExtConfig;
    for (RK_S32 i = 0; i < pstAvsGrpAttr->u32PipeNum; i++) {
        u16LdchData[i] = (RK_U16 *)(malloc(33196));

        memset(&stMbExtConfig, 0, sizeof(MB_EXT_CONFIG_S));
        stMbExtConfig.pOpaque = u16LdchData[i];
        stMbExtConfig.pu8VirAddr = (RK_U8 *)u16LdchData[i];
        stMbExtConfig.u64Size = 33196;

        s32Ret = RK_MPI_SYS_CreateMB(&(pstFinalLut.pLdchBlk[i]), &stMbExtConfig);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("avs [%d, %d] create ldch blk failed with %#x!",
                    AvsGrp, i, s32Ret);
        }
    }

    s32Ret = RK_MPI_AVS_GetFinalLut(AvsGrp, &pstFinalLut);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("avs [%d, %d] RK_MPI_AVS_GetFinalLut failed: %#x!",
                AvsGrp, 0, s32Ret);
        goto __FAILED;
    }
    RK_LOGV("avs [%d, %d] RK_MPI_AVS_GetFinalLut already.", AvsGrp, 0);

__FAILED:
    for (RK_S32 i = 0; i < pstAvsGrpAttr->u32PipeNum; i++) {
        RK_MPI_SYS_Free(pstFinalLut.pLdchBlk[i]);
        if (u16LdchData[i]) {
            free(u16LdchData[i]);
        }
    }

    s32Ret = RK_MPI_AVS_DestroyGrp(AvsGrp);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("avs [%d, %d] RK_MPI_AVS_DestroyGrp failed: %#x!!",
                AvsGrp, 0, s32Ret);
    }
    RK_LOGV("avs [%d, %d] RK_MPI_AVS_DestroyGrp already.", AvsGrp, 0);

    return s32Ret;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

