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

#include <stdlib.h>
#include <string.h>

#include "test_comm_rgn.h"
#include "test_comm_bmp.h"
#include "test_comm_imgproc.h"

#include "rk_mpi_rgn.h"
#include "rk_mpi_cal.h"
#include "rk_comm_rgn.h"
#include "rk_common.h"
#include "rk_debug.h"

RK_S32 TEST_RGN_CreateOverlayAndAttach(
        RGN_HANDLE RgnHandle, RK_U32 u32Width, RK_U32 u32Height,
        PIXEL_FORMAT_E enFmt, const MPP_CHN_S *pstChn, RK_U32 u32ChnNum) {
    RGN_ATTR_S stRgnAttr;
    RGN_CHN_ATTR_S stRgnChnAttr;
    RK_S32 s32Ret = RK_SUCCESS;

    memset(&stRgnAttr, 0, sizeof(RGN_ATTR_S));
    memset(&stRgnChnAttr, 0, sizeof(RGN_CHN_ATTR_S));

    stRgnAttr.enType = OVERLAY_RGN;
    stRgnAttr.unAttr.stOverlay.enPixelFmt = enFmt;
    stRgnAttr.unAttr.stOverlay.stSize.u32Width  = u32Width;
    stRgnAttr.unAttr.stOverlay.stSize.u32Height = u32Height;

    s32Ret = RK_MPI_RGN_Create(RgnHandle, &stRgnAttr);
    if (RK_SUCCESS != s32Ret) {
        RK_LOGE("RK_MPI_RGN_Create (%d) failed with %#x!", RgnHandle, s32Ret);
        RK_MPI_RGN_Destroy(RgnHandle);
        return RK_FAILURE;
    }

    for (RK_S32 i = 0; i < u32ChnNum; i++) {
        stRgnChnAttr.bShow = RK_TRUE;
        stRgnChnAttr.enType = OVERLAY_RGN;
        stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 0;
        stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 0;
        stRgnChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 128;
        stRgnChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 128;
        stRgnChnAttr.unChnAttr.stOverlayChn.u32Layer = i;
        s32Ret = RK_MPI_RGN_AttachToChn(RgnHandle, &(pstChn[i]), &stRgnChnAttr);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("RK_MPI_RGN_AttachToChn (%d) failed with %#x!", RgnHandle, s32Ret);
            return RK_FAILURE;
        }
    }

    return s32Ret;
}

RK_S32 TEST_RGN_CreateCoverAndAttach(
        RGN_HANDLE RgnHandle, const COVER_CHN_ATTR_S *pstCoverAttr,
        const MPP_CHN_S *pstChn, RK_U32 u32ChnNum) {
    RGN_ATTR_S stRgnAttr;
    RGN_CHN_ATTR_S stRgnChnAttr;
    RK_S32 s32Ret = RK_SUCCESS;

    memset(&stRgnAttr, 0, sizeof(RGN_ATTR_S));
    memset(&stRgnChnAttr, 0, sizeof(RGN_CHN_ATTR_S));

    stRgnAttr.enType = COVER_RGN;

    s32Ret = RK_MPI_RGN_Create(RgnHandle, &stRgnAttr);
    if (RK_SUCCESS != s32Ret) {
        RK_LOGE("RK_MPI_RGN_Create (%d) failed with %#x!", RgnHandle, s32Ret);
        RK_MPI_RGN_Destroy(RgnHandle);
        return RK_FAILURE;
    }

    for (RK_S32 i = 0; i < u32ChnNum; i++) {
        stRgnChnAttr.bShow = RK_TRUE;
        stRgnChnAttr.enType = COVER_RGN;
        memcpy(&(stRgnChnAttr.unChnAttr.stCoverChn), pstCoverAttr, sizeof(COVER_CHN_ATTR_S));
        s32Ret = RK_MPI_RGN_AttachToChn(RgnHandle, &(pstChn[i]), &stRgnChnAttr);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("RK_MPI_RGN_AttachToChn (%d) failed with %#x!", RgnHandle, s32Ret);
            return RK_FAILURE;
        }
    }

    return s32Ret;
}

RK_S32 TEST_RGN_DestroyAndDetach(
        RGN_HANDLE RgnHandle, const MPP_CHN_S *pstChn, RK_U32 u32ChnNum) {
    RK_S32 s32Ret = RK_SUCCESS;

    for (RK_S32 i = 0; i < u32ChnNum; i++) {
        s32Ret = RK_MPI_RGN_DetachFromChn(RgnHandle, &(pstChn[i]));
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("RK_MPI_RGN_DetachFrmChn (%d) failed with %#x!", RgnHandle, s32Ret);
            return RK_FAILURE;
        }
    }

    s32Ret = RK_MPI_RGN_Destroy(RgnHandle);
    if (RK_SUCCESS != s32Ret) {
        RK_LOGE("RK_MPI_RGN_Destroy [%d] failed with %#x", RgnHandle, s32Ret);
    }

    return s32Ret;
}

RK_S32 TEST_RGN_ChangePosition(
        RGN_HANDLE RgnHandle, const MPP_CHN_S *pstChn, RK_S32 s32X, RK_S32 s32Y) {
    RGN_CHN_ATTR_S stChnAttr;
    RK_S32 s32Ret = RK_SUCCESS;

    if (RK_NULL == pstChn) {
        RK_LOGE("input parameter is null. it is invaild!");
        return RK_FAILURE;
    }

    s32Ret = RK_MPI_RGN_GetDisplayAttr(RgnHandle, pstChn, &stChnAttr);
    if (RK_SUCCESS != s32Ret) {
        RK_LOGE("RK_MPI_RGN_GetDisplayAttr (%d)) failed with %#x!", RgnHandle, s32Ret);
        return RK_FAILURE;
    }

    stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = s32X;
    stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = s32Y;
    s32Ret = RK_MPI_RGN_SetDisplayAttr(RgnHandle, pstChn, &stChnAttr);
    if (RK_SUCCESS != s32Ret) {
        RK_LOGE("RK_MPI_RGN_SetDisplayAttr (%d)) failed with %#x!", RgnHandle, s32Ret);
        return RK_FAILURE;
    }

    return s32Ret;
}

/******************************************************************************
 * funciton : osd region show or hide
 ******************************************************************************/
RK_S32 TEST_RGN_ShowOrHide(RGN_HANDLE RgnHandle, const MPP_CHN_S *pstChn, RK_BOOL bShow) {
    RGN_CHN_ATTR_S stChnAttr;
    RK_S32 s32Ret = RK_SUCCESS;

    if (RK_NULL == pstChn) {
        RK_LOGE("input parameter is null. it is invaild!");
        return RK_FAILURE;
    }

    s32Ret = RK_MPI_RGN_GetDisplayAttr(RgnHandle, pstChn, &stChnAttr);
    if (RK_SUCCESS != s32Ret) {
        RK_LOGE("RK_MPI_RGN_GetDisplayAttr (%d)) failed with %#x!", RgnHandle, s32Ret);
        return RK_FAILURE;
    }

    stChnAttr.bShow = bShow;

    s32Ret = RK_MPI_RGN_SetDisplayAttr(RgnHandle, pstChn, &stChnAttr);
    if (RK_SUCCESS != s32Ret) {
        RK_LOGE("RK_MPI_RGN_SetDisplayAttr (%d)) failed with %#x!", RgnHandle, s32Ret);
        return RK_FAILURE;
    }

    return RK_SUCCESS;
}

RK_S32 TEST_RGN_LoadBmp(const RK_CHAR *filename, BITMAP_S *pstBitmap, PIXEL_FORMAT_E enBmpFmt) {
    OSD_SURFACE_S Surface;
    OSD_BITMAPFILEHEADER bmpFileHeader;
    OSD_BITMAPINFO bmpInfo;

    if (TEST_COMM_GetBmpInfo(filename, &bmpFileHeader, &bmpInfo) < 0) {
        RK_LOGE("GetBmpInfo err!\n");
        return RK_FAILURE;
    }

    switch (enBmpFmt) {
      case RK_FMT_ARGB8888:
        Surface.enColorFmt = OSD_COLOR_FMT_ARGB8888;
      break;
      case RK_FMT_BGRA8888:
        Surface.enColorFmt = OSD_COLOR_FMT_BGRA8888;
      break;
      case RK_FMT_ARGB1555:
        Surface.enColorFmt = OSD_COLOR_FMT_ARGB1555;
      break;
      case RK_FMT_BGRA5551:
        Surface.enColorFmt = OSD_COLOR_FMT_BGRA5551;
      break;
      case RK_FMT_ARGB4444:
        Surface.enColorFmt = OSD_COLOR_FMT_ARGB4444;
      break;
      case RK_FMT_BGRA4444:
        Surface.enColorFmt = OSD_COLOR_FMT_BGRA4444;
      break;
      default:
        return RK_FAILURE;
    }

    pstBitmap->pData = malloc(4 * (bmpInfo.bmiHeader.biWidth) * (bmpInfo.bmiHeader.biHeight));

    if (RK_NULL == pstBitmap->pData) {
        RK_LOGE("malloc osd memroy err!");
        return RK_FAILURE;
    }

    TEST_COMM_CreateSurfaceByBitmap(filename, &Surface, reinterpret_cast<RK_U8 *>(pstBitmap->pData));

    pstBitmap->u32Width = Surface.u16Width;
    pstBitmap->u32Height = Surface.u16Height;
    pstBitmap->enPixelFormat = (PIXEL_FORMAT_E)enBmpFmt;

    return RK_SUCCESS;
}

RK_S32 TEST_RGN_CreateBmp(RK_U32 u32Width, RK_U32 u32Height, PIXEL_FORMAT_E enBmpFmt, BITMAP_S *pstBitmap) {
    RK_S32 s32Ret = RK_SUCCESS;
    PIC_BUF_ATTR_S stBuffAttr;
    MB_PIC_CAL_S stPicCal;
    RK_U8 *pu8Data = RK_NULL;

    stBuffAttr.u32Width = u32Width;
    stBuffAttr.u32Height = u32Height;
    stBuffAttr.enPixelFormat = enBmpFmt;
    stBuffAttr.enCompMode = COMPRESS_MODE_NONE;
    s32Ret = RK_MPI_CAL_COMM_GetPicBufferSize(&stBuffAttr, &stPicCal);
    if (RK_SUCCESS != s32Ret) {
        RK_LOGE("RK_MPI_CAL_COMM_GetPicBufferSize failed with %#x!", s32Ret);
        return s32Ret;
    }

    pu8Data = (RK_U8 *)malloc(stPicCal.u32MBSize);

    // in order to diff raw map data, set 64 frame cnt
    s32Ret = TEST_COMM_FillImage(pu8Data, u32Width, u32Height,
                   u32Width, u32Height, enBmpFmt, 64);
    if (RK_SUCCESS != s32Ret) {
        RK_LOGE("TEST_COMM_FillImage failed with %#x!", s32Ret);
        return s32Ret;
    }

    pstBitmap->u32Width = u32Width;
    pstBitmap->u32Height = u32Height;
    pstBitmap->enPixelFormat = enBmpFmt;
    pstBitmap->pData = (RK_VOID *)pu8Data;

    return s32Ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
