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

#include "test_mod_vpss.h"
#include "test_comm_imgproc.h"
#include "test_comm_sys.h"
#include "test_comm_utils.h"
#include "test_comm_vpss.h"

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#include "rk_debug.h"
#include "rk_mpi_vpss.h"
#include "rk_mpi_mb.h"
#include "rk_mpi_sys.h"
#include "rk_mpi_cal.h"

#define VPSS_GET_CHN_FRAME_TIMEOUT_MS      200

static void TEST_VPSS_InitAttr(
        TEST_VPSS_CTX_S *pstCtx, VPSS_GRP_ATTR_S *pstVpssGrpAttr, VPSS_CHN_ATTR_S *pstVpssChnAttrs) {
    memset(pstVpssGrpAttr, 0, sizeof(VPSS_GRP_ATTR_S));
    memset(pstVpssChnAttrs, 0, sizeof(VPSS_CHN_ATTR_S) * VPSS_MAX_CHN_NUM);

    pstVpssGrpAttr->u32MaxW = 4096;
    pstVpssGrpAttr->u32MaxH = 4096;
    pstVpssGrpAttr->enPixelFormat = (PIXEL_FORMAT_E)pstCtx->s32SrcPixFormat;
    pstVpssGrpAttr->enCompressMode = (COMPRESS_MODE_E)pstCtx->s32SrcCompressMode;
    pstVpssGrpAttr->stFrameRate.s32SrcFrameRate = pstCtx->s32SrcGrpRate;
    pstVpssGrpAttr->stFrameRate.s32DstFrameRate = pstCtx->s32DstGrpRate;

    for (RK_S32 i = 0; i < VPSS_MAX_CHN_NUM; i++) {
        pstVpssChnAttrs[i].enChnMode = (VPSS_CHN_MODE_E)pstCtx->s32ChnMode;
        pstVpssChnAttrs[i].enCompressMode = (COMPRESS_MODE_E)pstCtx->s32DstCompressMode;
        pstVpssChnAttrs[i].enDynamicRange = DYNAMIC_RANGE_SDR8;
        pstVpssChnAttrs[i].enPixelFormat = (PIXEL_FORMAT_E)pstCtx->s32DstPixFormat;
        pstVpssChnAttrs[i].stFrameRate.s32SrcFrameRate = pstCtx->s32SrcChnRate;
        pstVpssChnAttrs[i].stFrameRate.s32DstFrameRate = pstCtx->s32DstChnRate;
        pstVpssChnAttrs[i].u32Width = pstCtx->s32DstWidth;
        pstVpssChnAttrs[i].u32Height = pstCtx->s32DstHeight;
        pstVpssChnAttrs[i].u32Depth = pstCtx->u32ChnDepth;
        pstVpssChnAttrs[i].bFlip = (RK_BOOL)pstCtx->s32Flip;
        pstVpssChnAttrs[i].bMirror = (RK_BOOL)pstCtx->s32Mirror;
        pstVpssChnAttrs[i].u32FrameBufCnt = 1;
    }
}

static MB_POOL TEST_VPSS_CreatePool(TEST_VPSS_CTX_S *pstCtx) {
    MB_POOL_CONFIG_S stMbPoolCfg;
    PIC_BUF_ATTR_S stPicBufAttr;
    MB_PIC_CAL_S stMbPicCalResult;
    RK_S32 s32Ret = RK_SUCCESS;

    stPicBufAttr.u32Width = pstCtx->s32SrcWidth;
    stPicBufAttr.u32Height = pstCtx->s32SrcHeight;
    stPicBufAttr.enCompMode = (COMPRESS_MODE_E)pstCtx->s32SrcCompressMode;
    stPicBufAttr.enPixelFormat = (PIXEL_FORMAT_E)pstCtx->s32SrcPixFormat;
    s32Ret = RK_MPI_CAL_VGS_GetPicBufferSize(&stPicBufAttr, &stMbPicCalResult);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("get picture buffer size failed. err 0x%x", s32Ret);
        return MB_INVALID_POOLID;
    }

    memset(&stMbPoolCfg, 0, sizeof(MB_POOL_CONFIG_S));
    stMbPoolCfg.u64MBSize = stMbPicCalResult.u32MBSize;
    stMbPoolCfg.u32MBCnt  = 10;
    stMbPoolCfg.enRemapMode = MB_REMAP_MODE_CACHED;
    stMbPoolCfg.bPreAlloc = RK_TRUE;

    return RK_MPI_MB_CreatePool(&stMbPoolCfg);
}

static void *TEST_VPSS_ModSingleTest(void *arg) {
    RK_S32           s32Ret = RK_SUCCESS;
    TEST_VPSS_CTX_S *pstCtx = reinterpret_cast<TEST_VPSS_CTX_S *>(arg);
    void *retArg = RK_NULL;
    VIDEO_FRAME_INFO_S stChnFrameInfos[VPSS_MAX_CHN_NUM];
    VIDEO_FRAME_INFO_S stGrpFrameInfo;
    char cWritePath[128] = {0};

    memset(stChnFrameInfos, 0, sizeof(VIDEO_FRAME_INFO_S) * VPSS_MAX_CHN_NUM);
    memset(&stGrpFrameInfo, 0, sizeof(VIDEO_FRAME_INFO_S));

    s32Ret = TEST_VPSS_ModInit(pstCtx);
    if (s32Ret != RK_SUCCESS) {
        goto __FAILED;
    }

    for (RK_S32 loopCount = 0; loopCount < pstCtx->s32LoopCount; loopCount++) {
        s32Ret = TEST_VPSS_ModSendFrame(pstCtx);
        if (s32Ret != RK_SUCCESS) {
            goto __FAILED;
        }
        s32Ret = TEST_VPSS_ModGetChnFrame(pstCtx, stChnFrameInfos);
        if (s32Ret != RK_SUCCESS) {
            goto __FAILED;
        }
        for (RK_S32 i = 0; i < pstCtx->s32ChnNum; i++) {
            if (pstCtx->dstFilePath) {
                snprintf(cWritePath, sizeof(cWritePath), "%schn_out_%dx%d_%d_%d.bin",
                         pstCtx->dstFilePath, stChnFrameInfos[i].stVFrame.u32VirWidth,
                         stChnFrameInfos[i].stVFrame.u32VirHeight, pstCtx->s32GrpIndex, i);
                s32Ret = TEST_COMM_FileWriteOneFrame(cWritePath, &(stChnFrameInfos[i]));
                if (s32Ret != RK_SUCCESS) {
                    goto __FAILED;
                }
            }
            s32Ret = RK_MPI_VPSS_ReleaseChnFrame(pstCtx->s32GrpIndex, i, &(stChnFrameInfos[i]));
            if (s32Ret != RK_SUCCESS) {
                goto __FAILED;
            }
        }
    }

    retArg = arg;
__FAILED:
    TEST_VPSS_ModDeInit(pstCtx);
    if (retArg == RK_NULL) {
        RK_LOGE("single vpss test %d running failed.", pstCtx->s32GrpIndex);
    }
    return retArg;
}

RK_S32 TEST_VPSS_ModTest(TEST_VPSS_CTX_S *ctx) {
    RK_S32 s32Ret = RK_SUCCESS;
    pthread_t tids[VPSS_MAX_GRP_NUM];
    TEST_VPSS_CTX_S tmpCtx[VPSS_MAX_GRP_NUM];

    for (RK_S32 grpIndex = 0; grpIndex < ctx->s32GrpNum; grpIndex++) {
        memcpy(&(tmpCtx[grpIndex]), ctx, sizeof(TEST_VPSS_CTX_S));
        pthread_create(&tids[grpIndex], 0, TEST_VPSS_ModSingleTest, reinterpret_cast<void *>(&tmpCtx[grpIndex]));
        ctx->s32GrpIndex++;
    }

    for (RK_S32 grpIndex = 0; grpIndex < ctx->s32GrpNum; grpIndex++) {
        void *retval = RK_NULL;
        pthread_join(tids[grpIndex], &retval);
        if (retval == RK_NULL) {
            RK_LOGE("Has error test.");
            s32Ret = RK_FAILURE;
        }
    }

    return s32Ret;
}

RK_S32 TEST_VPSS_UnitTest(TEST_VPSS_CTX_S *pstCtx, VIDEO_FRAME_INFO_S *pstChnFrames) {
    RK_S32           s32Ret = RK_SUCCESS;

    s32Ret = TEST_VPSS_ModInit(pstCtx);
    if (s32Ret != RK_SUCCESS) {
        goto __FAILED;
    }
    s32Ret = TEST_VPSS_ModSendFrame(pstCtx);
    if (s32Ret != RK_SUCCESS) {
        goto __FAILED;
    }
    s32Ret = TEST_VPSS_ModGetChnFrame(pstCtx, pstChnFrames);
    if (s32Ret != RK_SUCCESS) {
        goto __FAILED;
    }

__FAILED:
    TEST_VPSS_ModDeInit(pstCtx);

    return s32Ret;
}

RK_S32 TEST_VPSS_ModInit(TEST_VPSS_CTX_S *pstCtx) {
    RK_S32           s32Ret = RK_SUCCESS;
    MB_BLK           srcBlk = MB_INVALID_HANDLE;
    VPSS_GRP_ATTR_S stVpssGrpAttr;
    VPSS_CHN_ATTR_S stVpssChnAttr[VPSS_MAX_CHN_NUM];

    TEST_VPSS_InitAttr(pstCtx, &stVpssGrpAttr, stVpssChnAttr);

    s32Ret = TEST_VPSS_Start(pstCtx->s32GrpIndex, pstCtx->s32ChnNum,
                             &stVpssGrpAttr, stVpssChnAttr);
    if (s32Ret != RK_SUCCESS) {
        goto __FAILED;
    }

    s32Ret = RK_MPI_VPSS_SetVProcDev(pstCtx->s32GrpIndex, (VIDEO_PROC_DEV_TYPE_E)pstCtx->s32VProcDevType);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VPSS_SetVProcDev(grp:%d) failed with %#x!", pstCtx->s32GrpIndex, s32Ret);
        return s32Ret;
    }

    s32Ret = TEST_VPSS_GrpSetZoom(pstCtx->s32GrpIndex, pstCtx->s32GrpCropRatio, pstCtx->bGrpCropEn);
    if (s32Ret != RK_SUCCESS) {
        goto __FAILED;
    }
    for (RK_S32 chnIndex = 0; chnIndex < pstCtx->s32ChnNum; chnIndex++) {
        s32Ret = TEST_VPSS_ChnSetZoom(pstCtx->s32GrpIndex, chnIndex, pstCtx->s32ChnCropRatio, pstCtx->bChnCropEn);
        if (s32Ret != RK_SUCCESS) {
            goto __FAILED;
        }
        s32Ret = TEST_VPSS_SetChnRotation(pstCtx->s32GrpIndex, chnIndex, (ROTATION_E)pstCtx->s32Rotation);
        if (s32Ret != RK_SUCCESS) {
            goto __FAILED;
        }
        s32Ret = TEST_VPSS_SetChnRotationEx(pstCtx->s32GrpIndex, chnIndex, pstCtx->s32RotationEx);
        if (s32Ret != RK_SUCCESS) {
            goto __FAILED;
        }
        if (pstCtx->bAttachPool) {
            pstCtx->attachPool = TEST_VPSS_CreatePool(pstCtx);
            if (pstCtx->attachPool == MB_INVALID_POOLID) {
                s32Ret = RK_FAILURE;
                goto __FAILED;
            }
            s32Ret = RK_MPI_VPSS_AttachMbPool(pstCtx->s32GrpIndex, chnIndex, pstCtx->attachPool);
            if (s32Ret != RK_SUCCESS) {
                goto __FAILED;
            }
        }
    }

    return RK_SUCCESS;
__FAILED:
    for (RK_S32 i = 0; i < pstCtx->s32ChnNum; i++) {
        if (pstCtx->bAttachPool) {
            s32Ret = RK_MPI_VPSS_DetachMbPool(pstCtx->s32GrpIndex, i);
            if (s32Ret != RK_SUCCESS) {
                continue;
            }
        }
    }
    if (pstCtx->bAttachPool) {
        RK_MPI_MB_DestroyPool(pstCtx->attachPool);
    }
    return s32Ret;
}

RK_S32 TEST_VPSS_ModDeInit(TEST_VPSS_CTX_S *pstCtx) {
    RK_S32 s32Ret = RK_SUCCESS;

    for (RK_S32 i = 0; i < pstCtx->s32ChnNum; i++) {
        if (pstCtx->bAttachPool) {
            s32Ret = RK_MPI_VPSS_DetachMbPool(pstCtx->s32GrpIndex, i);
            if (s32Ret != RK_SUCCESS) {
                continue;
            }
        }
    }
    TEST_VPSS_Stop(pstCtx->s32GrpIndex, pstCtx->s32ChnNum);
    if (pstCtx->bAttachPool) {
        RK_MPI_MB_DestroyPool(pstCtx->attachPool);
    }

    return s32Ret;
}


RK_S32 TEST_VPSS_ModSendFrame(TEST_VPSS_CTX_S *pstCtx) {
    RK_S32 s32Ret = RK_SUCCESS;
    PIC_BUF_ATTR_S stPicBufAttr;
    VIDEO_FRAME_INFO_S stVideoFrame;

    memset(&stVideoFrame, 0x0, sizeof(VIDEO_FRAME_INFO_S));

    stPicBufAttr.u32Width = pstCtx->s32SrcWidth;
    stPicBufAttr.u32Height = pstCtx->s32SrcHeight;
    stPicBufAttr.enPixelFormat = (PIXEL_FORMAT_E)pstCtx->s32SrcPixFormat;
    stPicBufAttr.enCompMode = (COMPRESS_MODE_E)pstCtx->s32SrcCompressMode;
    s32Ret = TEST_SYS_CreateVideoFrame(&stPicBufAttr, &stVideoFrame);
    if (s32Ret != RK_SUCCESS) {
        goto __FAILED;
    }
    if (pstCtx->srcFileName != RK_NULL) {
        s32Ret = TEST_COMM_FileReadOneFrame(pstCtx->srcFileName, &stVideoFrame);
        if (s32Ret != RK_SUCCESS) {
            goto __FAILED;
        }
    } else {
        s32Ret = TEST_COMM_FillImage(
                            (RK_U8 *)RK_MPI_MB_Handle2VirAddr(stVideoFrame.stVFrame.pMbBlk),
                            pstCtx->s32SrcWidth, pstCtx->s32SrcHeight,
                            RK_MPI_CAL_COMM_GetHorStride(pstCtx->s32SrcVirWidth,
                                            (PIXEL_FORMAT_E)pstCtx->s32SrcPixFormat),
                            pstCtx->s32SrcVirHeight,
                            (PIXEL_FORMAT_E)pstCtx->s32SrcPixFormat, 1);
        if (s32Ret != RK_SUCCESS) {
            goto __FAILED;
        }
    }
    RK_MPI_SYS_MmzFlushCache(stVideoFrame.stVFrame.pMbBlk, RK_FALSE);

    s32Ret = RK_MPI_VPSS_SendFrame(pstCtx->s32GrpIndex, 0, &stVideoFrame, -1);
    if (s32Ret != RK_SUCCESS) {
        goto __FAILED;
    }

__FAILED:
    RK_MPI_MB_ReleaseMB(stVideoFrame.stVFrame.pMbBlk);

    return s32Ret;
}

RK_S32 TEST_VPSS_ModGetGrpFrame(TEST_VPSS_CTX_S *pstCtx, VIDEO_FRAME_INFO_S *pstVideoFrame) {
    RK_S32 s32Ret = RK_SUCCESS;

    s32Ret = RK_MPI_VPSS_GetGrpFrame(pstCtx->s32GrpIndex, 0, pstVideoFrame);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    return s32Ret;
}

RK_S32 TEST_VPSS_ModGetChnFrame(TEST_VPSS_CTX_S *pstCtx, VIDEO_FRAME_INFO_S *pstVideoFrame) {
    RK_S32 s32Ret = RK_SUCCESS;

    for (RK_S32 i = 0; i < pstCtx->s32ChnNum; i++) {
        s32Ret = RK_MPI_VPSS_GetChnFrame(pstCtx->s32GrpIndex, i, &(pstVideoFrame[i]), VPSS_GET_CHN_FRAME_TIMEOUT_MS);
        if (s32Ret != RK_SUCCESS) {
            return RK_SUCCESS;
        }
    }

    return s32Ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

