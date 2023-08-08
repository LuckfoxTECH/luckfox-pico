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
#include <unistd.h>

#include "test_comm_vpss.h"
#include "test_comm_imgproc.h"

#include "rk_mpi_vpss.h"
#include "rk_mpi_cal.h"
#include "rk_mpi_mb.h"
#include "rk_mpi_mmz.h"
#include "rk_mpi_sys.h"
#include "rk_comm_vpss.h"
#include "rk_common.h"
#include "rk_debug.h"

typedef struct test_vpss_proc_s {
     RK_BOOL bThreadStart;
     FILE *srcFp;
     FILE *dstSaveFp;
     VPSS_GRP VpssGrp;
     RK_U32 u32VpssChnNum;
     RK_U32 u32SendFrameRate;
     pthread_t VpssProcPid;
     TEST_VPSS_PROC_CTX_S stProcCtx;
} TEST_VPSS_PROC_S;

static TEST_VPSS_PROC_S gProcThread[VPSS_MAX_GRP_NUM];

RK_S32 TEST_VPSS_Start(
        VPSS_GRP VpssGrp,
        RK_U32 u32ChnNum,
        VPSS_GRP_ATTR_S *pstVpssGrpAttr,
        VPSS_CHN_ATTR_S *pstVpssChnAttr) {
    VPSS_CHN VpssChn = 0;
    RK_S32 s32Ret = RK_SUCCESS;

    s32Ret = RK_MPI_VPSS_CreateGrp(VpssGrp, pstVpssGrpAttr);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VPSS_CreateGrp(grp:%d) failed with %#x!", VpssGrp, s32Ret);
        return s32Ret;
    }

    for (VpssChn = 0; VpssChn < u32ChnNum; VpssChn++) {
        s32Ret = RK_MPI_VPSS_SetChnAttr(VpssGrp, VpssChn, &pstVpssChnAttr[VpssChn]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_VPSS_SetChnAttr failed with %#x", s32Ret);
            return s32Ret;
        }

        s32Ret = RK_MPI_VPSS_EnableChn(VpssGrp, VpssChn);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_VPSS_EnableChn failed with %#x", s32Ret);
            return s32Ret;
        }
    }

    s32Ret = RK_MPI_VPSS_StartGrp(VpssGrp);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VPSS_StartGrp failed with %#x", s32Ret);
        return s32Ret;
    }
    s32Ret = RK_MPI_VPSS_ResetGrp(VpssGrp);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VPSS_ResetGrp failed with %#x", s32Ret);
        return s32Ret;
    }

    return s32Ret;
}

RK_S32 TEST_VPSS_Stop(VPSS_GRP VpssGrp, RK_U32 u32ChnNum) {
    RK_S32 s32Ret = RK_SUCCESS;
    VPSS_CHN VpssChn = 0;

    for (VpssChn = 0; VpssChn < u32ChnNum; VpssChn++) {
        s32Ret = RK_MPI_VPSS_DisableChn(VpssGrp, VpssChn);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_VPSS_DisableChn failed with %#x!", s32Ret);
            return s32Ret;
        }
    }

    s32Ret = RK_MPI_VPSS_StopGrp(VpssGrp);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VPSS_StopGrp failed with %#x!", s32Ret);
        return s32Ret;
    }

    s32Ret = RK_MPI_VPSS_DestroyGrp(VpssGrp);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VPSS_DestroyGrp failed with %#x!", s32Ret);
        return s32Ret;
    }

    return RK_SUCCESS;
}

RK_S32 TEST_VPSS_GrpSetZoom(VPSS_GRP VpssGrp, RK_U32 u32Zoom, RK_BOOL bEnable) {
    RK_S32 s32Ret = RK_SUCCESS;
    VPSS_CROP_INFO_S stCropInfo;

    s32Ret = RK_MPI_VPSS_GetGrpCrop(VpssGrp, &stCropInfo);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VPSS_GetGrpCrop failed with %#x!", s32Ret);
        return s32Ret;
    }

    stCropInfo.bEnable = bEnable;
    stCropInfo.enCropCoordinate = VPSS_CROP_RATIO_COOR;
    stCropInfo.stCropRect.s32X = 500 - u32Zoom / 2;
    stCropInfo.stCropRect.s32Y = 500 - u32Zoom / 2;
    stCropInfo.stCropRect.u32Width = u32Zoom;
    stCropInfo.stCropRect.u32Height = u32Zoom;
    s32Ret = RK_MPI_VPSS_SetGrpCrop(VpssGrp, &stCropInfo);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VPSS_SetGrpCrop failed with %#x!", s32Ret);
        return s32Ret;
    }

    return s32Ret;
}

RK_S32 TEST_VPSS_ChnSetZoom(VPSS_GRP VpssGrp, VPSS_CHN VpssChn, RK_U32 u32Zoom, RK_BOOL bEnable) {
    RK_S32 s32Ret = RK_SUCCESS;
    VPSS_CROP_INFO_S stCropInfo;

    s32Ret = RK_MPI_VPSS_GetChnCrop(VpssGrp, VpssChn, &stCropInfo);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VPSS_GetChnCrop failed with %#x!", s32Ret);
        return s32Ret;
    }

    stCropInfo.bEnable = bEnable;
    stCropInfo.enCropCoordinate = VPSS_CROP_RATIO_COOR;
    stCropInfo.stCropRect.s32X = 500 - u32Zoom / 2;
    stCropInfo.stCropRect.s32Y = 500 - u32Zoom / 2;
    stCropInfo.stCropRect.u32Width = u32Zoom;
    stCropInfo.stCropRect.u32Height = u32Zoom;
    s32Ret = RK_MPI_VPSS_SetChnCrop(VpssGrp, VpssChn, &stCropInfo);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VPSS_SetChnCrop failed with %#x!", s32Ret);
        return s32Ret;
    }

    return s32Ret;
}

RK_S32 TEST_VPSS_SetChnRotation(VPSS_GRP VpssGrp, VPSS_CHN VpssChn, ROTATION_E enRotation) {
    RK_S32 s32Ret = RK_SUCCESS;
    ROTATION_E rotation = ROTATION_0;

    s32Ret = RK_MPI_VPSS_GetChnRotation(VpssGrp, VpssChn, &rotation);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VPSS_GetChnRotation failed with %#x!", s32Ret);
        return s32Ret;
    }
    s32Ret = RK_MPI_VPSS_SetChnRotation(VpssGrp, VpssChn, enRotation);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VPSS_SetChnRotation failed with %#x!", s32Ret);
        return s32Ret;
    }

    return s32Ret;
}

RK_S32 TEST_VPSS_SetChnRotationEx(VPSS_GRP VpssGrp, VPSS_CHN VpssChn, RK_U32 u32Angle) {
    RK_S32 s32Ret = RK_SUCCESS;
    VPSS_ROTATION_EX_ATTR_S stRotationEx;

    if (u32Angle == 0) {
        return s32Ret;
    }

    stRotationEx.bEnable = RK_TRUE;
    stRotationEx.stRotationEx.u32Angle = u32Angle;
    s32Ret = RK_MPI_VPSS_SetChnRotationEx(VpssGrp, VpssChn, &stRotationEx);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    s32Ret = RK_MPI_VPSS_GetChnRotationEx(VpssGrp, VpssChn, &stRotationEx);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    if (stRotationEx.bEnable != RK_TRUE
            || stRotationEx.stRotationEx.u32Angle != u32Angle) {
        s32Ret = RK_FAILURE;
        return s32Ret;
    }

    return s32Ret;
}


void* TEST_VPSS_Proc(void *pArgs) {
    TEST_VPSS_PROC_S *pstCtx = reinterpret_cast<TEST_VPSS_PROC_S *>(pArgs);
    RK_S32           s32Ret = RK_SUCCESS;
    MB_BLK           srcBlk = MB_INVALID_HANDLE;
    RK_S32           s32ReadLen = 0;
    RK_S32           s32FrameCount = 0;
    VIDEO_FRAME_INFO_S frameIn = {0};
    VIDEO_FRAME_INFO_S frameOut = {0};
    PIC_BUF_ATTR_S  stPicBufAttr;
    MB_PIC_CAL_S    stMbPicCalResult;

    stPicBufAttr.u32Width = pstCtx->stProcCtx.u32RawWidth;
    stPicBufAttr.u32Height = pstCtx->stProcCtx.u32RawHeight;
    stPicBufAttr.enCompMode = COMPRESS_MODE_NONE;
    stPicBufAttr.enPixelFormat = (PIXEL_FORMAT_E)pstCtx->stProcCtx.u32RawPixelFmt;
    s32Ret = RK_MPI_CAL_VGS_GetPicBufferSize(&stPicBufAttr, &stMbPicCalResult);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("get picture buffer size failed. err 0x%x", s32Ret);
        return RK_NULL;
    }

    RK_MPI_MMZ_Alloc(&srcBlk, stMbPicCalResult.u32MBSize, RK_MMZ_ALLOC_CACHEABLE);
    while (pstCtx->bThreadStart == RK_TRUE) {
        if (pstCtx->srcFp != RK_NULL) {
            s32Ret = fseek(pstCtx->srcFp, 0, SEEK_SET);
            if (s32Ret != 0) {
                break;
            }
            s32ReadLen = fread(RK_MPI_MB_Handle2VirAddr(srcBlk), 1, stMbPicCalResult.u32MBSize, pstCtx->srcFp);
            if (s32ReadLen <= 0) {
                RK_LOGE("read size is not enough. read %d, request %d", s32ReadLen, stMbPicCalResult.u32MBSize);
                break;
            }
        } else {
            s32Ret = TEST_COMM_FillImage((RK_U8 *)RK_MPI_MB_Handle2VirAddr(srcBlk),
                            pstCtx->stProcCtx.u32RawWidth,
                            pstCtx->stProcCtx.u32RawHeight,
                            RK_MPI_CAL_COMM_GetHorStride(pstCtx->stProcCtx.u32RawWidth,
                                    (PIXEL_FORMAT_E)pstCtx->stProcCtx.u32RawPixelFmt),
                            stMbPicCalResult.u32VirHeight,
                            (PIXEL_FORMAT_E)pstCtx->stProcCtx.u32RawPixelFmt,
                            s32FrameCount);
            if (s32Ret != RK_SUCCESS) {
                RK_MPI_MB_ReleaseMB(srcBlk);
                break;
            }
        }
        RK_MPI_SYS_MmzFlushCache(srcBlk, RK_FALSE);
        frameIn.stVFrame.pMbBlk = srcBlk;
        frameIn.stVFrame.u32Width = pstCtx->stProcCtx.u32RawWidth;
        frameIn.stVFrame.u32Height = pstCtx->stProcCtx.u32RawHeight;
        frameIn.stVFrame.u32VirWidth = stMbPicCalResult.u32VirWidth;
        frameIn.stVFrame.u32VirHeight = stMbPicCalResult.u32VirHeight;
        frameIn.stVFrame.enPixelFormat = (PIXEL_FORMAT_E)pstCtx->stProcCtx.u32RawPixelFmt;
        frameIn.stVFrame.enCompressMode = COMPRESS_MODE_NONE;
        s32Ret = RK_MPI_VPSS_SendFrame(pstCtx->VpssGrp, 0, &frameIn, -1);
        if (s32Ret != RK_SUCCESS) {
            RK_MPI_MB_ReleaseMB(srcBlk);
            break;
        }
        s32FrameCount++;

        for (RK_S32 i = 0; i < pstCtx->u32VpssChnNum; i++) {
            s32Ret = RK_MPI_VPSS_GetChnFrame(pstCtx->VpssGrp, i, &frameOut, -1);
            if (s32Ret != RK_SUCCESS) {
                continue;
            }
            RK_LOGI("get chn[%d] frame %p length %d", i, frameOut.stVFrame.pMbBlk, stMbPicCalResult.u32MBSize);
            if (pstCtx->dstSaveFp != RK_NULL) {
                RK_MPI_SYS_MmzFlushCache(frameOut.stVFrame.pMbBlk, RK_TRUE);
                fwrite(RK_MPI_MB_Handle2VirAddr(frameOut.stVFrame.pMbBlk),
                        1, stMbPicCalResult.u32MBSize, pstCtx->dstSaveFp);
                fflush(pstCtx->dstSaveFp);
            }
            RK_MPI_VPSS_ReleaseChnFrame(pstCtx->VpssGrp, i, &frameOut);
            usleep(1000000 / pstCtx->u32SendFrameRate);
        }
    }

    return RK_NULL;
}

RK_S32 TEST_VPSS_StartProc(
        VPSS_GRP VpssGrp, RK_U32 u32ChnNum,
        const TEST_VPSS_PROC_CTX_S *pstProcCtx) {
    RK_S32 s32Ret = 0;

    memcpy(&(gProcThread[VpssGrp].stProcCtx), pstProcCtx, sizeof(TEST_VPSS_PROC_CTX_S));

    if (gProcThread[VpssGrp].stProcCtx.srcFileName != RK_NULL) {
        gProcThread[VpssGrp].srcFp = fopen(gProcThread[VpssGrp].stProcCtx.srcFileName, "r");
        if (gProcThread[VpssGrp].srcFp == RK_NULL) {
            RK_LOGE("can't open file %s!", gProcThread[VpssGrp].stProcCtx.srcFileName);
            return RK_FAILURE;
        }
    }
    if (gProcThread[VpssGrp].stProcCtx.dstSaveFileName != RK_NULL) {
        gProcThread[VpssGrp].dstSaveFp = fopen(gProcThread[VpssGrp].stProcCtx.dstSaveFileName, "wb");
        if (gProcThread[VpssGrp].dstSaveFp == RK_NULL) {
            RK_LOGE("can't open file %s!", gProcThread[VpssGrp].stProcCtx.dstSaveFileName);
            return RK_FAILURE;
        }
    }

    gProcThread[VpssGrp].bThreadStart = RK_TRUE;
    gProcThread[VpssGrp].VpssGrp = VpssGrp;
    gProcThread[VpssGrp].u32VpssChnNum = u32ChnNum;
    gProcThread[VpssGrp].u32SendFrameRate = pstProcCtx->u32SendFrameRate;

    s32Ret = pthread_create(&(gProcThread[VpssGrp].VpssProcPid), 0,
                            TEST_VPSS_Proc, (RK_VOID *)&(gProcThread[VpssGrp]));
    if (s32Ret != 0) {
        return s32Ret;
    }

    return s32Ret;
}

RK_S32 TEST_VPSS_StopProc(VPSS_GRP VpssGrp) {
    if (RK_TRUE == gProcThread[VpssGrp].bThreadStart) {
        gProcThread[VpssGrp].bThreadStart = RK_FALSE;
        pthread_join(gProcThread[VpssGrp].VpssProcPid, 0);
        if (gProcThread[VpssGrp].srcFp != RK_NULL) {
            fclose(gProcThread[VpssGrp].srcFp);
            gProcThread[VpssGrp].srcFp = RK_NULL;
        }
        if (gProcThread[VpssGrp].dstSaveFp != RK_NULL) {
            fclose(gProcThread[VpssGrp].dstSaveFp);
            gProcThread[VpssGrp].dstSaveFp = RK_NULL;
        }
    }

    return RK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
