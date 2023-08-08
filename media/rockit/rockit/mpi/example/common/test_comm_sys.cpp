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

#include <string.h>

#include "test_comm_sys.h"
#include "test_comm_utils.h"
#include "test_comm_imgproc.h"

#include "rk_common.h"
#include "rk_mpi_sys.h"
#include "rk_mpi_cal.h"
#include "rk_mpi_mb.h"
#include "rk_mpi_mmz.h"

RK_S32 TEST_SYS_CreateVideoFrame(const PIC_BUF_ATTR_S *pstBufAttr, VIDEO_FRAME_INFO_S *pstVideoFrame) {
    MB_PIC_CAL_S stCalResult;

    if (pstBufAttr == RK_NULL || pstVideoFrame == RK_NULL) {
        return RK_FAILURE;
    }

    CHECK_RET(RK_MPI_CAL_VGS_GetPicBufferSize(pstBufAttr, &stCalResult),
             "VGS get pic size");

    CHECK_RET(RK_MPI_SYS_MmzAlloc_Cached(&(pstVideoFrame->stVFrame.pMbBlk),
                                        RK_NULL, RK_NULL, stCalResult.u32MBSize),
             "RK_MPI_SYS_MmzAlloc_Cached");

    pstVideoFrame->stVFrame.u32Width = pstBufAttr->u32Width;
    pstVideoFrame->stVFrame.u32Height = pstBufAttr->u32Height;
    pstVideoFrame->stVFrame.u32VirWidth = stCalResult.u32VirWidth;
    pstVideoFrame->stVFrame.u32VirHeight = stCalResult.u32VirHeight;
    pstVideoFrame->stVFrame.enPixelFormat = pstBufAttr->enPixelFormat;
    pstVideoFrame->stVFrame.enCompressMode = pstBufAttr->enCompMode;

    return RK_SUCCESS;
}

RK_S32 TEST_SYS_VpssBindVo(VPSS_GRP VpssGrp, VPSS_CHN VpssChn, VO_DEV VoDev, VO_CHN VoChn) {
    RK_S32 s32Ret = RK_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = RK_ID_VPSS;
    stSrcChn.s32DevId = VpssGrp;
    stSrcChn.s32ChnId = VpssChn;

    stDestChn.enModId = RK_ID_VO;
    stDestChn.s32DevId = VoDev;
    stDestChn.s32ChnId = VoChn;

    s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("failed with %#x!", s32Ret);
        return RK_FAILURE;
    }

    return s32Ret;
}

RK_S32 TEST_SYS_VpssUnbindVo(VPSS_GRP VpssGrp, VPSS_CHN VpssChn, VO_DEV VoDev, VO_CHN VoChn) {
    RK_S32 s32Ret = RK_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = RK_ID_VPSS;
    stSrcChn.s32DevId = VpssGrp;
    stSrcChn.s32ChnId = VpssChn;

    stDestChn.enModId = RK_ID_VO;
    stDestChn.s32DevId = VoDev;
    stDestChn.s32ChnId = VoChn;

    s32Ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("failed with %#x!", s32Ret);
        return RK_FAILURE;
    }

    return s32Ret;
}

RK_S32 TEST_SYS_VdecBindVpss(VDEC_CHN VdecChn, VPSS_GRP VpssGrp, VPSS_CHN VpssChn) {
    RK_S32 s32Ret = RK_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = RK_ID_VDEC;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = VdecChn;

    stDestChn.enModId = RK_ID_VPSS;
    stDestChn.s32DevId = VpssGrp;
    stDestChn.s32ChnId = VpssChn;

    s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("failed with %#x!", s32Ret);
        return RK_FAILURE;
    }

    return s32Ret;
}

RK_S32 TEST_SYS_VdecUnbindVpss(VDEC_CHN VdecChn, VPSS_GRP VpssGrp, VPSS_CHN VpssChn) {
    RK_S32 s32Ret = RK_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = RK_ID_VDEC;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = VdecChn;

    stDestChn.enModId = RK_ID_VPSS;
    stDestChn.s32DevId = VpssGrp;
    stDestChn.s32ChnId = VpssChn;

    s32Ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("failed with %#x!", s32Ret);
        return RK_FAILURE;
    }

    return s32Ret;
}

RK_S32 TEST_SYS_VpssBindVenc(VPSS_GRP VpssGrp, VPSS_CHN VpssChn, VENC_CHN VencChn) {
    RK_S32 s32Ret = RK_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = RK_ID_VPSS;
    stSrcChn.s32DevId = VpssGrp;
    stSrcChn.s32ChnId = VpssChn;

    stDestChn.enModId = RK_ID_VENC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = VencChn;

    s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("failed with %#x!", s32Ret);
        return RK_FAILURE;
    }

    return s32Ret;
}

RK_S32 TEST_SYS_VpssUnbindVenc(VPSS_GRP VpssGrp, VPSS_CHN VpssChn, VENC_CHN VencChn) {
    RK_S32 s32Ret = RK_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = RK_ID_VPSS;
    stSrcChn.s32DevId = VpssGrp;
    stSrcChn.s32ChnId = VpssChn;

    stDestChn.enModId = RK_ID_VENC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = VencChn;

    s32Ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("failed with %#x!", s32Ret);
        return RK_FAILURE;
    }

    return s32Ret;
}

RK_S32 TEST_SYS_VencBindVdec(VENC_CHN VencChn, VDEC_CHN VdecChn) {
    RK_S32 s32Ret = RK_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = RK_ID_VENC;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = VencChn;

    stDestChn.enModId = RK_ID_VDEC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = VdecChn;

    s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("failed with %#x!", s32Ret);
        return RK_FAILURE;
    }

    return s32Ret;
}

RK_S32 TEST_SYS_VencUnbindVdec(VENC_CHN VencChn, VDEC_CHN VdecChn) {
    RK_S32 s32Ret = RK_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = RK_ID_VENC;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = VencChn;

    stDestChn.enModId = RK_ID_VDEC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = VdecChn;

    s32Ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("failed with %#x!", s32Ret);
        return RK_FAILURE;
    }

    return s32Ret;
}

RK_S32 TEST_SYS_VdecBindVo(VDEC_CHN VdecChn, VO_DEV VoDev, VO_CHN VoChn) {
    RK_S32 s32Ret = RK_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = RK_ID_VDEC;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = VdecChn;

    stDestChn.enModId = RK_ID_VO;
    stDestChn.s32DevId = VoDev;
    stDestChn.s32ChnId = VoChn;

    s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("failed with %#x!", s32Ret);
        return RK_FAILURE;
    }

    return s32Ret;
}

RK_S32 TEST_SYS_VdecUnbindVo(VDEC_CHN VdecChn, VO_DEV VoDev, VO_CHN VoChn) {
    RK_S32 s32Ret = RK_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = RK_ID_VDEC;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = VdecChn;

    stDestChn.enModId = RK_ID_VO;
    stDestChn.s32DevId = VoDev;
    stDestChn.s32ChnId = VoChn;

    s32Ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("failed with %#x!", s32Ret);
        return RK_FAILURE;
    }

    return s32Ret;
}

RK_S32 TEST_SYS_FuzzyCompareFrameByFile(
        const char *pFileName, VIDEO_FRAME_INFO_S *pstVideoFrame, RK_DOUBLE dThreshold) {
    RK_S32 s32Ret = RK_SUCCESS;
    RK_BOOL bDiff = RK_TRUE;
    VIDEO_FRAME_INFO_S stCompareFrame;
    PIC_BUF_ATTR_S stBufAttr;

    memset(&stCompareFrame, 0, sizeof(VIDEO_FRAME_INFO_S));
    memset(&stBufAttr, 0, sizeof(PIC_BUF_ATTR_S));

    stBufAttr.u32Width = pstVideoFrame->stVFrame.u32VirWidth;
    stBufAttr.u32Height = pstVideoFrame->stVFrame.u32VirHeight;
    stBufAttr.enPixelFormat = pstVideoFrame->stVFrame.enPixelFormat;
    stBufAttr.enCompMode = pstVideoFrame->stVFrame.enCompressMode;
    s32Ret = TEST_SYS_CreateVideoFrame(&stBufAttr, &stCompareFrame);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    s32Ret = TEST_COMM_FileReadOneFrame(pFileName, &stCompareFrame);
    if (s32Ret != RK_SUCCESS) {
        goto __FAILED;
    }
    RK_MPI_SYS_MmzFlushCache(stCompareFrame.stVFrame.pMbBlk, RK_FALSE);
    RK_MPI_SYS_MmzFlushCache(pstVideoFrame->stVFrame.pMbBlk, RK_FALSE);
    bDiff = TEST_COMM_CompareImageFuzzy(
                (RK_U8 *)RK_MPI_MB_Handle2VirAddr(pstVideoFrame->stVFrame.pMbBlk),
                (RK_U8 *)RK_MPI_MB_Handle2VirAddr(stCompareFrame.stVFrame.pMbBlk),
                RK_MPI_CAL_COMM_GetHorStride(pstVideoFrame->stVFrame.u32VirWidth,
                pstVideoFrame->stVFrame.enPixelFormat),
                pstVideoFrame->stVFrame.u32Width,
                pstVideoFrame->stVFrame.u32Height,
                dThreshold);
    if (bDiff) {
        RK_LOGE("image compare has too large different.");
        s32Ret = RK_FAILURE;
        goto __FAILED;
    }

__FAILED:
    RK_MPI_MB_ReleaseMB(stCompareFrame.stVFrame.pMbBlk);
    return s32Ret;
}

RK_S32 TEST_SYS_FuzzyCompareFrame(VIDEO_FRAME_INFO_S *pstVideoFrame, RK_DOUBLE dThreshold) {
    RK_S32 s32Ret = RK_SUCCESS;
    RK_BOOL bDiff = RK_TRUE;
    VIDEO_FRAME_INFO_S stCompareFrame;
    PIC_BUF_ATTR_S stBufAttr;

    memset(&stCompareFrame, 0, sizeof(VIDEO_FRAME_INFO_S));
    memset(&stBufAttr, 0, sizeof(PIC_BUF_ATTR_S));

    stBufAttr.u32Width = pstVideoFrame->stVFrame.u32VirWidth;
    stBufAttr.u32Height = pstVideoFrame->stVFrame.u32VirHeight;
    stBufAttr.enPixelFormat = pstVideoFrame->stVFrame.enPixelFormat;
    stBufAttr.enCompMode = pstVideoFrame->stVFrame.enCompressMode;
    s32Ret = TEST_SYS_CreateVideoFrame(&stBufAttr, &stCompareFrame);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    s32Ret = TEST_COMM_FillImage((RK_U8 *)RK_MPI_MB_Handle2VirAddr(stCompareFrame.stVFrame.pMbBlk),
                        pstVideoFrame->stVFrame.u32Width, pstVideoFrame->stVFrame.u32Height,
                        RK_MPI_CAL_COMM_GetHorStride(pstVideoFrame->stVFrame.u32VirWidth,
                                        pstVideoFrame->stVFrame.enPixelFormat),
                        pstVideoFrame->stVFrame.u32VirHeight,
                        pstVideoFrame->stVFrame.enPixelFormat, 1);
    if (s32Ret != RK_SUCCESS) {
        goto __FAILED;
    }

    RK_MPI_SYS_MmzFlushCache(stCompareFrame.stVFrame.pMbBlk, RK_FALSE);
    RK_MPI_SYS_MmzFlushCache(pstVideoFrame->stVFrame.pMbBlk, RK_FALSE);
    bDiff = TEST_COMM_CompareImageFuzzy(
                (RK_U8 *)RK_MPI_MB_Handle2VirAddr(pstVideoFrame->stVFrame.pMbBlk),
                (RK_U8 *)RK_MPI_MB_Handle2VirAddr(stCompareFrame.stVFrame.pMbBlk),
                RK_MPI_CAL_COMM_GetHorStride(pstVideoFrame->stVFrame.u32VirWidth,
                pstVideoFrame->stVFrame.enPixelFormat),
                pstVideoFrame->stVFrame.u32Width,
                pstVideoFrame->stVFrame.u32Height,
                dThreshold);
    if (bDiff) {
        RK_LOGE("image compare has too large different.");
        s32Ret = RK_FAILURE;
        goto __FAILED;
    }

__FAILED:
    RK_MPI_MB_ReleaseMB(stCompareFrame.stVFrame.pMbBlk);
    return s32Ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
