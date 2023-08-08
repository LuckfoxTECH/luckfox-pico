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
 *
 */
#ifdef DBG_MOD_ID
#undef DBG_MOD_ID
#endif
#define DBG_MOD_ID DBG_MOD_COMB1(RK_ID_RGN)

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "rk_debug.h"
#include "rk_type.h"
#include "rk_mpi_rgn.h"
#include "rk_mpi_sys.h"
#include "rk_mpi_cal.h"
#include "rk_comm_vpss.h"
#include "rk_mpi_vpss.h"
#include "rk_mpi_mmz.h"

#include "test_comm_bmp.h"
#include "test_comm_argparse.h"
#include "test_comm_utils.h"
#include "test_comm_rgn.h"
#include "test_comm_venc.h"
#include "test_comm_vpss.h"

/* PixFormat: big-edian:BGRA8888 => A:bit31~bit24 R:bit23~bit16 G:bit15~bit8 B:bit7~bit0 */
const RK_U32 u32BGRA8888ColorTblUser[RGN_CLUT_NUM] = {
    // transparency/gray/red/earthy yellow /blue
    0x00ffffff, 0xff5e6060, 0xffe9491e, 0xfff4bc1f, 0xff1ca2dd, 0xff87bd43,
    0xffff1f1f, 0xff0000af, 0xff0000d7, 0xff0000ff, 0xff005f00, 0xff005f5f,
    0xff005f87, 0xff005faf, 0xff005fd7, 0xff005fff, 0xff008000, 0xff008080,
    0xff008700, 0xff00875f, 0xff008787, 0xff0087af, 0xff0087d7, 0xff0087ff,
    0xff00af00, 0xff00af5f, 0xff00af87, 0xff00afaf, 0xff00afd7, 0xff00afff,
    0xff00d700, 0xff00d75f, 0xff00d787, 0xff00d7af, 0xff00d7d7, 0xff00d7ff,
    0xff00ff00, 0xff00ff28, 0xff00ff5f, 0xff00ff87, 0xff00ffaf, 0xff00ffd7,
    0xff00ffff, 0xff00ffff, 0xff080808, 0xff121212, 0xff1c1c1c, 0xff262626,
    0xff303030, 0xff3a3a3a, 0xff444444, 0xff4e4e4e, 0xff585858, 0xff5f0000,
    0xff5f005f, 0xff5f0087, 0xff5f00af, 0xff5f00d7, 0xff5f00ff, 0xff5f5f00,
    0xff5f5f5f, 0xff5f5f87, 0xff5f5faf, 0xff5f5fd7, 0xff5f5fff, 0xff5f8700,
    0xff5f875f, 0xff5f8787, 0xff5f87af, 0xff5f87d7, 0xff5f87ff, 0xff5faf00,
    0xff5faf5f, 0xff5faf87, 0xff5fafaf, 0xff5fafd7, 0xff5fafff, 0xff5fd700,
    0xff5fd75f, 0xff5fd787, 0xff5fd7af, 0xff5fd7d7, 0xff5fd7ff, 0xff5fff00,
    0xff5fff5f, 0xff5fff87, 0xff5fffaf, 0xff5fffd7, 0xff5fffff, 0xff626262,
    0xff6c6c6c, 0xff767676, 0xff800000, 0xff800080, 0xff808000, 0xff808080,
    0xff808080, 0xff870000, 0xff87005f, 0xff870087, 0xff8700af, 0xff8700d7,
    0xff8700ff, 0xff875f00, 0xff875f5f, 0xff875f87, 0xff875faf, 0xff875fd7,
    0xff875fff, 0xff878700, 0xff87875f, 0xff878787, 0xff8787af, 0xff8787d7,
    0xff8787ff, 0xff87af00, 0xff87af5f, 0xff87af87, 0xff87afaf, 0xff87afd7,
    0xff87afff, 0xff87d700, 0xff87d75f, 0xff87d787, 0xff87d7af, 0xff87d7d7,
    0xff87d7ff, 0xff87ff00, 0xff87ff5f, 0xff87ff87, 0xff87ffaf, 0xff87ffd7,
    0xff87ffff, 0xff8a8a8a, 0xff949494, 0xff9e9e9e, 0xffa8a8a8, 0xffaf0000,
    0xffaf005f, 0xffaf0087, 0xffaf00af, 0xffaf00d7, 0xffaf00ff, 0xffaf5f00,
    0xffaf5f5f, 0xffaf5f87, 0xffaf5faf, 0xffaf5fd7, 0xffaf5fff, 0xffaf8700,
    0xffaf875f, 0xffaf8787, 0xffaf87af, 0xffaf87d7, 0xffaf87ff, 0xffafaf00,
    0xffafaf5f, 0xffafaf87, 0xffafafaf, 0xffafafd7, 0xffafafff, 0xffafd700,
    0xffafd75f, 0xffafd787, 0xffafd7af, 0xffafd7d7, 0xffafd7ff, 0xffafff00,
    0xffafff5f, 0xffafff87, 0xffafffaf, 0xffafffd7, 0xffafffff, 0xffb2b2b2,
    0xffbcbcbc, 0xffc0c0c0, 0xffc6c6c6, 0xffd0d0d0, 0xffd70000, 0xffd7005f,
    0xffd70087, 0xffd700af, 0xffd700d7, 0xffd700ff, 0xffd75f00, 0xffd75f5f,
    0xffd75f87, 0xffd75faf, 0xffd75fd7, 0xffd75fff, 0xffd78700, 0xffd7875f,
    0xffd78787, 0xffd787af, 0xffd787d7, 0xffd787ff, 0xffd7af00, 0xffd7af5f,
    0xffd7af87, 0xffd7afaf, 0xffd7afd7, 0xffd7afff, 0xffd7d700, 0xffd7d75f,
    0xffd7d787, 0xffd7d7af, 0xffd7d7d7, 0xffd7d7ff, 0xffd7ff00, 0xffd7ff5f,
    0xffd7ff87, 0xffd7ffaf, 0xffd7ffd7, 0xffd7ffff, 0xffdadada, 0xffe4e4e4,
    0xffeeeeee, 0xffff0000, 0xffff0028, 0xffff005f, 0xffff0087, 0xffff00af,
    0xffff00d7, 0xffff00ff, 0xffff00ff, 0xffff5f00, 0xffff5f5f, 0xffff5f87,
    0xffff5faf, 0xffff5fd7, 0xffff5fff, 0xffff8700, 0xffff875f, 0xffff8787,
    0xffff87af, 0xffff87d7, 0xffff87ff, 0xffffaf00, 0xffffaf5f, 0xffffaf87,
    0xffffafaf, 0xffffafd7, 0xffffafff, 0xffffd700, 0xffffd75f, 0xffffd787,
    0xffffd7af, 0xffffd7d7, 0xffffd7ff, 0xffffff00, 0xffffff28, 0xffffff5f,
    0xffffff87, 0xffffffaf, 0xffffffd7, 0xffffffff,
};

typedef struct _rkTestRGNCtx {
    const char *srcFileRawName;
    const char *srcFileBmpName;
    const char *dstSaveFileName;
    RK_S32      s32Operation;
    RK_S32      s32LoopCount;
    RK_S32      s32RgnCount;
    RK_S32      s32BitmapCount;
    RK_S32      s32CanvasCount;
    RECT_S      stRegion;
    RK_U32      u32RawWidth;
    RK_U32      u32RawHeight;
    RK_U32      u32RawFormat;
    RK_U32      u32BmpFormat;
    RK_BOOL     bRgnQp;
    RK_BOOL     bAbsQp;
    RK_S32      s32Qp;
    RK_U32      u32ClutNum;
    pthread_t   vencSendFrameTid;
    pthread_t   vencGetStreamTid;
    RK_U32      u32AttachMod;
    RK_U32      u32Codec;
    RK_U32      u32BgAlpha;
    RK_U32      u32FgAlpha;
    RK_U32      u32ColorLUT0;
    RK_U32      u32ColorLUT1;
    RK_BOOL     bRgnInv;
    RK_U32      u32InvWidth;
    RK_U32      u32InvHeight;
    RK_U32      u32LumThresh;
    RK_U32      u32Rgntime;
} TEST_RGN_CTX_S;

RK_S32 load_file_osdmem (const RK_CHAR *filename, RK_U8 *pu8Virt, RK_U32 u32Width, RK_U32 u32Height, RK_U32  pixel_size, RK_U32 shift_value) {
    RK_U32 mem_len = u32Width;
    RK_U32 read_len = mem_len * pixel_size >> shift_value;
    RK_U32 read_height;
    FILE *file = NULL;

    file = fopen(filename, "rb");
    if (file == NULL) {
        RK_LOGE("open filename: %s file failed!", filename);
        return RK_FAILURE;
    }
    for (read_height = 0; read_height < u32Height; read_height++) {
        fread((pu8Virt + (u32Width * read_height * pixel_size >> shift_value)), 1, read_len, file);
    }
    fclose(file);
    return RK_SUCCESS;
}

RK_S32 TEST_RGN_LOAD_MEM(const RK_CHAR *filename, BITMAP_S *pstBitmap, TEST_RGN_CTX_S *pstRgnCtx) {
    RK_S32  s32Ret = RK_SUCCESS;
    RK_U32  pixel_size, shift_value;

    switch (pstRgnCtx->u32BmpFormat) {
      case RK_FMT_ARGB8888:
      case RK_FMT_ABGR8888:
      case RK_FMT_BGRA8888:
      case RK_FMT_RGBA8888:
      {
        pixel_size = 4;
		shift_value = 0;
      }
      break;
      case RK_FMT_ARGB1555:
      case RK_FMT_BGRA5551:
      case RK_FMT_RGBA5551:
      case RK_FMT_ABGR1555:
      case RK_FMT_ARGB4444:
      case RK_FMT_BGRA4444:
      case RK_FMT_RGBA4444:
      case RK_FMT_ABGR4444:
      {
        pixel_size = 2;
		shift_value = 0;
      }
      break;
      case RK_FMT_2BPP:
      {
        pixel_size = 1;
        shift_value = 2;
      }
      break;
      default:
      RK_LOGE("no support style");
        return RK_FAILURE;
    }

    pstBitmap->pData = malloc(pstRgnCtx->stRegion.u32Width * pstRgnCtx->stRegion.u32Height * pixel_size >> shift_value);

    if (filename) {
        s32Ret = load_file_osdmem(filename, reinterpret_cast<RK_U8 *>(pstBitmap->pData), pstRgnCtx->stRegion.u32Width,
                        pstRgnCtx->stRegion.u32Height, pixel_size, shift_value);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("Load file osd memory failed!");
            return RK_FAILURE;
        }
    } else {
        memset(pstBitmap->pData, 0xff, pstRgnCtx->stRegion.u32Width * pstRgnCtx->stRegion.u32Height * pixel_size >> shift_value);
    }
    pstBitmap->u32Width = pstRgnCtx->stRegion.u32Width;
    pstBitmap->u32Height = pstRgnCtx->stRegion.u32Height;
    pstBitmap->enPixelFormat = (PIXEL_FORMAT_E)pstRgnCtx->u32BmpFormat;

    return RK_SUCCESS;
}

RK_S32 test_rgn_overlay_process(TEST_RGN_CTX_S *pstRgnCtx, MPP_CHN_S *pstMppChn) {
    RK_S32          s32Ret       = RK_SUCCESS;
    RGN_HANDLE      RgnHandle    = 0;
    VENC_CHN        VencChn      = 0;
    POINT_S         stPoint      = {0};
    BITMAP_S        stBitmap;
    RGN_ATTR_S      stRgnAttr;
    RGN_CHN_ATTR_S  stRgnChnAttr;
    VENC_ATTR_S     stVencAttr;
    RK_BOOL         bShow;

    /****************************************
    step 1: create overlay regions
    ****************************************/
    for (RK_S32 i = 0; i < pstRgnCtx->s32RgnCount; i++) {
        stRgnAttr.enType = OVERLAY_RGN;
        stRgnAttr.unAttr.stOverlay.enPixelFmt = (PIXEL_FORMAT_E)pstRgnCtx->u32BmpFormat;
        stRgnAttr.unAttr.stOverlay.stSize.u32Width  = pstRgnCtx->stRegion.u32Width;
        stRgnAttr.unAttr.stOverlay.stSize.u32Height = pstRgnCtx->stRegion.u32Height;
        stRgnAttr.unAttr.stOverlay.u32ClutNum = pstRgnCtx->u32ClutNum;
        if (pstRgnCtx->u32ClutNum)
            memcpy(stRgnAttr.unAttr.stOverlay.u32Clut, u32BGRA8888ColorTblUser, sizeof(u32BGRA8888ColorTblUser));

        RgnHandle = i;

        s32Ret = RK_MPI_RGN_Create(RgnHandle, &stRgnAttr);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("RK_MPI_RGN_Create (%d) failed with %#x!", RgnHandle, s32Ret);
            RK_MPI_RGN_Destroy(RgnHandle);
            return RK_FAILURE;
        }
        RK_LOGI("The handle: %d, create success!", RgnHandle);
    }

    /*********************************************
    step 2: display overlay regions to groups
    *********************************************/
    for (RK_S32 i = 0; i < pstRgnCtx->s32RgnCount; i++) {
        RgnHandle = i;

        memset(&stRgnChnAttr, 0, sizeof(stRgnChnAttr));
        stRgnChnAttr.bShow = RK_TRUE;
        stRgnChnAttr.enType = OVERLAY_RGN;
        stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = pstRgnCtx->stRegion.s32X + 150 * (i % 3);
        stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = pstRgnCtx->stRegion.s32Y + 150 * (i / 3);
        stRgnChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = pstRgnCtx->u32FgAlpha;
        stRgnChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = pstRgnCtx->u32FgAlpha;
        stRgnChnAttr.unChnAttr.stOverlayChn.u32Layer = i;
        stRgnChnAttr.unChnAttr.stOverlayChn.stQpInfo.bEnable = pstRgnCtx->bRgnQp;
        stRgnChnAttr.unChnAttr.stOverlayChn.stQpInfo.bForceIntra = RK_TRUE;
        stRgnChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = pstRgnCtx->bAbsQp;
        stRgnChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp = pstRgnCtx->s32Qp;
        stRgnChnAttr.unChnAttr.stOverlayChn.u32ColorLUT[0] = pstRgnCtx->u32ColorLUT0;
        stRgnChnAttr.unChnAttr.stOverlayChn.u32ColorLUT[1] = pstRgnCtx->u32ColorLUT1;
        stRgnChnAttr.unChnAttr.stOverlayChn.stInvertColor.bInvColEn = pstRgnCtx->bRgnInv;
        stRgnChnAttr.unChnAttr.stOverlayChn.stInvertColor.stInvColArea.u32Width = pstRgnCtx->u32InvWidth;
        stRgnChnAttr.unChnAttr.stOverlayChn.stInvertColor.stInvColArea.u32Height = pstRgnCtx->u32InvHeight;
        stRgnChnAttr.unChnAttr.stOverlayChn.stInvertColor.enChgMod = LESSTHAN_LUM_THRESH;
        stRgnChnAttr.unChnAttr.stOverlayChn.stInvertColor.u32LumThresh = pstRgnCtx->u32LumThresh;
        s32Ret = RK_MPI_RGN_AttachToChn(RgnHandle, pstMppChn, &stRgnChnAttr);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("RK_MPI_RGN_AttachToChn (%d) failed with %#x!", RgnHandle, s32Ret);
            return RK_FAILURE;
        }
    }

    RK_LOGI("Display region to chn success!");
    usleep(5000);

    /*********************************************
    step 3: show bitmap
    *********************************************/
    RK_S64 s64ShowBmpStart = TEST_COMM_GetNowUs();
    s32Ret = TEST_RGN_LOAD_MEM(pstRgnCtx->srcFileBmpName, &stBitmap, pstRgnCtx);
    if (RK_SUCCESS != s32Ret) {
        RK_LOGE("Load osd file failed with %#x!", s32Ret);
        return RK_FAILURE;
    }

    for (RK_S32 k = 0; k < pstRgnCtx->s32BitmapCount; k++) {
        for (RK_S32 i = 0; i < pstRgnCtx->s32RgnCount; i++) {
            RgnHandle = i;
            s32Ret = RK_MPI_RGN_SetBitMap(RgnHandle, &stBitmap);
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("RK_MPI_RGN_SetBitMap failed with %#x!", s32Ret);
                return RK_FAILURE;
            }
        }

        if (k == pstRgnCtx->s32BitmapCount -1) {
            if (RK_NULL != stBitmap.pData) {
                free(stBitmap.pData);
                stBitmap.pData = NULL;
            }
        }
        RK_LOGE("RK_MPI_RGN_SetBitMap success count: %d!", k + 1);
        usleep(1000000);
    }
    RK_S64 s64ShowBmpEnd = TEST_COMM_GetNowUs();
    RK_LOGI("Handle:%d, space time %lld us, load bmp success!", RgnHandle, s64ShowBmpEnd - s64ShowBmpStart);

    /*********************************************
    step 4: use update canvas interface
    *********************************************/
    s64ShowBmpStart = TEST_COMM_GetNowUs();
    for (RK_S32 k = 0; k < pstRgnCtx->s32CanvasCount; k++) {
        for (RK_S32 i = 0; i < pstRgnCtx->s32RgnCount; i++) {
            RGN_CANVAS_INFO_S stCanvasInfo;
            RgnHandle = i;
            memset(&stCanvasInfo, 0, sizeof(RGN_CANVAS_INFO_S));

            s32Ret = RK_MPI_RGN_GetCanvasInfo(RgnHandle, &stCanvasInfo);
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("RK_MPI_RGN_GetCanvasInfo failed with %#x!", s32Ret);
                return RK_FAILURE;
            }
            memset(reinterpret_cast<void *>(stCanvasInfo.u64VirAddr), 0xff,
                    stCanvasInfo.u32VirWidth * stCanvasInfo.u32VirHeight >> 2);
            s32Ret = RK_MPI_RGN_UpdateCanvas(RgnHandle);
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("RK_MPI_RGN_UpdateCanvas failed with %#x!", s32Ret);
                return RK_FAILURE;
            }
        }
        RK_LOGE("RK_MPI_RGN_UpdateCanvas success count: %d!", k + 1);
        usleep(1000000);
    }
    s64ShowBmpEnd = TEST_COMM_GetNowUs();
    RK_LOGI("Handle:%d, space time %lld us, update canvas success!", RgnHandle, s64ShowBmpEnd - s64ShowBmpStart);


    /*********************************************
    step 5: change overlay regions' position
    *********************************************/
    RK_S64 s64ChgPosStart = TEST_COMM_GetNowUs();
    RgnHandle = 0;

    stPoint.s32X = 128;
    stPoint.s32Y = 128;
    s32Ret = TEST_RGN_ChangePosition(RgnHandle, pstMppChn, stPoint.s32X, stPoint.s32Y);
    if (RK_SUCCESS != s32Ret) {
        RK_LOGE("Change region(%d) position failed with %#x!", RgnHandle, s32Ret);
        return RK_FAILURE;
    }

    RK_S64 s64ChgPosEnd = TEST_COMM_GetNowUs();

    RK_LOGI("Handle:%d, space time %lld us, change point success,new point(x:%d,y:%d)!",
        RgnHandle, s64ChgPosEnd - s64ChgPosStart, stPoint.s32X, stPoint.s32Y);
    usleep(50000);

    /*********************************************
    step 6: show or hide overlay regions
    *********************************************/
    RgnHandle = 0;
    bShow = RK_FALSE;

    for (RK_S32 i = 0; i < 2; i++) {
        s32Ret = TEST_RGN_ShowOrHide(RgnHandle, pstMppChn, bShow);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("Region(%d) show failed with %#x!", RgnHandle, s32Ret);
            return RK_FAILURE;
        }
        RK_LOGI("Handle:%d, %s osd success", RgnHandle, bShow ? "show" : "hide");

        bShow = bShow ? RK_FALSE : RK_TRUE;

        usleep(300000);
    }

    /*********************************************
    step 7: Detach osd from chn
    *********************************************/
    for (RK_S32 i = 0; i < pstRgnCtx->s32RgnCount; i++) {
        RgnHandle = i;
        s32Ret = RK_MPI_RGN_DetachFromChn(RgnHandle, pstMppChn);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("RK_MPI_RGN_DetachFrmChn (%d) failed with %#x!", RgnHandle, s32Ret);
            return RK_FAILURE;
        }

        RK_LOGI("Detach handle:%d from chn success", RgnHandle);
        usleep(5000);
    }

    /*********************************************
    step 8: destory region
    *********************************************/
    for (RK_S32 i = 0; i < pstRgnCtx->s32RgnCount; i++) {
        RgnHandle = i;
        s32Ret = RK_MPI_RGN_Destroy(RgnHandle);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("RK_MPI_RGN_Destroy [%d] failed with %#x", RgnHandle, s32Ret);
        }
        RK_LOGI("Destory handle:%d success", RgnHandle);
    }

    return s32Ret;
}

RK_S32 test_rgn_cover_process(TEST_RGN_CTX_S *pstRgnCtx, MPP_CHN_S *pstMppChn) {
    RK_S32 s32Ret = RK_SUCCESS;
    RGN_HANDLE coverHandle;
    RGN_ATTR_S stCoverAttr;
    RGN_CHN_ATTR_S stCoverChnAttr;

    memset(&stCoverAttr, 0, sizeof(stCoverAttr));
    memset(&stCoverChnAttr, 0, sizeof(RGN_CHN_ATTR_S));

    for (RK_S32 i = 0; i < pstRgnCtx->s32RgnCount; i++) {
        coverHandle = i;
        stCoverAttr.enType = COVER_RGN;
        s32Ret = RK_MPI_RGN_Create(coverHandle, &stCoverAttr);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("failed with %#x!", s32Ret);
            return RK_FAILURE;
        }

        stCoverChnAttr.bShow = RK_TRUE;
        stCoverChnAttr.enType = COVER_RGN;
        stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32X = pstRgnCtx->stRegion.s32X;
        stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32Y = pstRgnCtx->stRegion.s32Y;
        stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Width = pstRgnCtx->stRegion.u32Width;
        stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Height = pstRgnCtx->stRegion.u32Height;
        stCoverChnAttr.unChnAttr.stCoverChn.u32Color = 0x00f800;
        stCoverChnAttr.unChnAttr.stCoverChn.u32Layer = i;
        s32Ret = RK_MPI_RGN_AttachToChn(coverHandle, pstMppChn, &stCoverChnAttr);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("failed with %#x!", s32Ret);
            goto AttachCover_failed;
        }
    }

    RK_LOGI("create an cover region and attach it to chn0");
    usleep(1000000);

    for (RK_S32 i = 0; i < pstRgnCtx->s32RgnCount; i++) {
        coverHandle = i;
        s32Ret = RK_MPI_RGN_GetDisplayAttr(coverHandle, pstMppChn, &stCoverChnAttr);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("failed with %#x!", s32Ret);
            goto exit;
        }
        stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32X = 64 * i;
        stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32Y = 64 * i;
        stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Width = 256;
        stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Height = 256;
        stCoverChnAttr.unChnAttr.stCoverChn.u32Color = 0x0000f8;
        stCoverChnAttr.unChnAttr.stCoverChn.u32Layer = 1;
        s32Ret = RK_MPI_RGN_SetDisplayAttr(coverHandle, pstMppChn, &stCoverChnAttr);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("failed with %#x!", s32Ret);
            goto exit;
        }

        RK_LOGI("resize the cover region %d to <%d, %d, %d, %d>, color<0x%x>",
            i,
            stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32X,
            stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32Y,
            stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Width,
            stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Height,
            stCoverChnAttr.unChnAttr.stCoverChn.u32Color);
    }
    usleep(500000);

    for (RK_S32 i = 0; i < pstRgnCtx->s32RgnCount; i++) {
        coverHandle = i;
        s32Ret = TEST_RGN_ShowOrHide(coverHandle, pstMppChn, RK_FALSE);
    }
    RK_LOGI("hide the cover and the overlay success");
    usleep(500000);

    for (RK_S32 i = 0; i < pstRgnCtx->s32RgnCount; i++) {
        coverHandle = i;
        s32Ret = TEST_RGN_ShowOrHide(coverHandle, pstMppChn, RK_TRUE);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("failed with %#x!", s32Ret);
            goto exit;
        }
    }

    RK_LOGI("show the cover and the overlay success");
    usleep(500000);

exit:
    for (RK_S32 i = 0; i < pstRgnCtx->s32RgnCount; i++) {
        coverHandle = i;
        RK_MPI_RGN_DetachFromChn(coverHandle, pstMppChn);
    }

AttachCover_failed:
    for (RK_S32 i = 0; i < pstRgnCtx->s32RgnCount; i++) {
        coverHandle = i;
        RK_MPI_RGN_Destroy(coverHandle);
    }
    return s32Ret;
}

RK_S32 test_rgn_mosaic_process(TEST_RGN_CTX_S *pstRgnCtx, MPP_CHN_S *pstMppChn) {
    RK_S32 s32Ret = RK_SUCCESS;
    RGN_HANDLE Handle;
    RGN_ATTR_S stAttr;
    RGN_CHN_ATTR_S stChnAttr;

    memset(&stAttr, 0, sizeof(RGN_ATTR_S));
    memset(&stChnAttr, 0, sizeof(RGN_CHN_ATTR_S));

    for (RK_S32 i = 0; i < pstRgnCtx->s32RgnCount; i++) {
        Handle = i;
        stAttr.enType = MOSAIC_RGN;
        s32Ret = RK_MPI_RGN_Create(Handle, &stAttr);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("failed with %#x!", s32Ret);
            return RK_FAILURE;
        }

        stChnAttr.bShow = RK_TRUE;
        stChnAttr.enType = MOSAIC_RGN;
        stChnAttr.unChnAttr.stMosaicChn.stRect.s32X = pstRgnCtx->stRegion.s32X;
        stChnAttr.unChnAttr.stMosaicChn.stRect.s32Y = pstRgnCtx->stRegion.s32Y;
        stChnAttr.unChnAttr.stMosaicChn.stRect.u32Width = pstRgnCtx->stRegion.u32Width;
        stChnAttr.unChnAttr.stMosaicChn.stRect.u32Height = pstRgnCtx->stRegion.u32Height;
        stChnAttr.unChnAttr.stMosaicChn.enBlkSize = MOSAIC_BLK_SIZE_8;
        stChnAttr.unChnAttr.stMosaicChn.u32Layer = i;
        s32Ret = RK_MPI_RGN_AttachToChn(Handle, pstMppChn, &stChnAttr);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("failed with %#x!", s32Ret);
            goto AttachCover_failed;
        }
    }

    RK_LOGI("create an cover region and attach it to chn0");
    usleep(1000000);

    for (RK_S32 i = 0; i < pstRgnCtx->s32RgnCount; i++) {
        Handle = i;
        RK_S32 s32Ret = TEST_RGN_ChangePosition(Handle, pstMppChn, 64 * i, 64 * i);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("failed with %#x!", s32Ret);
            goto exit;
        }

        RK_LOGI("move the cover region %d to <%d, %d, %d, %d>, blk size<0x%x>",
            i,
            stChnAttr.unChnAttr.stCoverChn.stRect.s32X,
            stChnAttr.unChnAttr.stCoverChn.stRect.s32Y,
            stChnAttr.unChnAttr.stCoverChn.stRect.u32Width,
            stChnAttr.unChnAttr.stCoverChn.stRect.u32Height,
            stChnAttr.unChnAttr.stMosaicChn.enBlkSize);
    }
    usleep(500000);

    for (RK_S32 i = 0; i < pstRgnCtx->s32RgnCount; i++) {
        Handle = i;
        s32Ret = TEST_RGN_ShowOrHide(Handle, pstMppChn, RK_FALSE);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("failed with %#x!", s32Ret);
            goto exit;
        }
    }
    RK_LOGI("hide the cover and the overlay success");
    usleep(500000);

    for (RK_S32 i = 0; i < pstRgnCtx->s32RgnCount; i++) {
        Handle = i;
        s32Ret = TEST_RGN_ShowOrHide(Handle, pstMppChn, RK_TRUE);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("failed with %#x!", s32Ret);
            goto exit;
        }
    }

    RK_LOGI("show the cover and the overlay success");
    usleep(500000);

exit:
    for (RK_S32 i = 0; i < pstRgnCtx->s32RgnCount; i++) {
        Handle = i;
        RK_MPI_RGN_DetachFromChn(Handle, pstMppChn);
    }
AttachCover_failed:
    for (RK_S32 i = 0; i < pstRgnCtx->s32RgnCount; i++) {
        Handle = i;
        RK_MPI_RGN_Destroy(Handle);
    }
    return s32Ret;
}

RK_S32 unit_test_mpi_rgn_venc(TEST_RGN_CTX_S *pstRgnCtx) {
    RK_S32 s32Ret = RK_SUCCESS;
    VENC_CHN VencChn = 0;
    MPP_CHN_S stMppChn;
    COMMON_TEST_VENC_CTX_S stVencCtx;

    stMppChn.enModId = RK_ID_VENC;
    stMppChn.s32DevId = 0;
    stMppChn.s32ChnId = VencChn;

    memset(&stVencCtx, 0, sizeof(COMMON_TEST_VENC_CTX_S));

    stVencCtx.u32Width = pstRgnCtx->u32RawWidth;
    stVencCtx.u32Height = pstRgnCtx->u32RawHeight;
    stVencCtx.enPixFmt = RK_FMT_YUV420SP;
    stVencCtx.enType = (RK_CODEC_ID_E)pstRgnCtx->u32Codec;
    stVencCtx.VencChn = VencChn;
    stVencCtx.u32StreamBufCnt = 4;
    stVencCtx.pSrcFramePath = pstRgnCtx->srcFileRawName;
    stVencCtx.pSaveStreamPath = pstRgnCtx->dstSaveFileName;

    s32Ret = TEST_VENC_Start(&stVencCtx);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    for (RK_U32 i = 0; i < pstRgnCtx->u32Rgntime; i++) {
        pstRgnCtx->stRegion.u32Width += 32 * i;
        pstRgnCtx->stRegion.u32Height += 32 * i;
        switch (pstRgnCtx->s32Operation) {
        case OVERLAY_RGN: {
            s32Ret = test_rgn_overlay_process(pstRgnCtx, &stMppChn);
        } break;
        case COVER_RGN: {
            s32Ret = test_rgn_cover_process(pstRgnCtx, &stMppChn);
        } break;
        default:
            RK_LOGE("unsupport operation %d.", pstRgnCtx->s32Operation);
            s32Ret = RK_FAILURE;
        }
        if (s32Ret != RK_SUCCESS) {
            return s32Ret;
        }
        RK_LOGI("Rgn run number of time: %d\n", i + 1);
    }
    s32Ret = TEST_VENC_Stop(VencChn);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    return s32Ret;
}

RK_S32 unit_test_mpi_rgn_vpss(TEST_RGN_CTX_S *pstRgnCtx) {
    RK_S32 s32Ret = RK_SUCCESS;
    VPSS_GRP VpssGrp = 0;
    MPP_CHN_S stMppChn;
    VPSS_GRP_ATTR_S stGrpAttr;
    VPSS_CHN_ATTR_S stChnAttr;
    TEST_VPSS_PROC_CTX_S stProcCtx;

    stMppChn.enModId = RK_ID_VPSS;
    stMppChn.s32DevId = VpssGrp;
    stMppChn.s32ChnId = 0;

    memset(&stGrpAttr, 0, sizeof(VPSS_GRP_ATTR_S));
    memset(&stChnAttr, 0, sizeof(VPSS_CHN_ATTR_S));
    memset(&stProcCtx, 0, sizeof(TEST_VPSS_PROC_CTX_S));

    stGrpAttr.u32MaxW = 4096;
    stGrpAttr.u32MaxH = 4096;
    stGrpAttr.enPixelFormat = (PIXEL_FORMAT_E)pstRgnCtx->u32RawFormat;
    stGrpAttr.enCompressMode = COMPRESS_MODE_NONE;
    stGrpAttr.stFrameRate.s32SrcFrameRate = -1;
    stGrpAttr.stFrameRate.s32DstFrameRate = -1;
    stChnAttr.enChnMode = VPSS_CHN_MODE_USER;
    stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
    stChnAttr.enDynamicRange = DYNAMIC_RANGE_SDR8;
    stChnAttr.enPixelFormat = (PIXEL_FORMAT_E)pstRgnCtx->u32RawFormat;
    stChnAttr.stFrameRate.s32SrcFrameRate = -1;
    stChnAttr.stFrameRate.s32DstFrameRate = -1;
    stChnAttr.u32Width = pstRgnCtx->u32RawWidth;
    stChnAttr.u32Height = pstRgnCtx->u32RawHeight;
    stChnAttr.u32Depth = 8;
    s32Ret = TEST_VPSS_Start(VpssGrp, 1, &stGrpAttr, &stChnAttr);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    stProcCtx.srcFileName = pstRgnCtx->srcFileRawName;
    stProcCtx.dstSaveFileName = pstRgnCtx->dstSaveFileName;
    stProcCtx.u32RawWidth = pstRgnCtx->u32RawWidth;
    stProcCtx.u32RawHeight = pstRgnCtx->u32RawHeight;
    stProcCtx.u32RawPixelFmt = pstRgnCtx->u32RawFormat;
    stProcCtx.u32SendFrameRate = 30;
    s32Ret = TEST_VPSS_StartProc(VpssGrp, 1, &stProcCtx);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    for (RK_U32 i = 0; i < pstRgnCtx->u32Rgntime; i++) {
        pstRgnCtx->stRegion.u32Width += 32 * i;
        pstRgnCtx->stRegion.u32Height += 32 * i;
        switch (pstRgnCtx->s32Operation) {
        case OVERLAY_RGN: {
            s32Ret = test_rgn_overlay_process(pstRgnCtx, &stMppChn);
        } break;
        case COVER_RGN: {
            s32Ret = test_rgn_cover_process(pstRgnCtx, &stMppChn);
        } break;
        default:
            RK_LOGE("unsupport operation %d.", pstRgnCtx->s32Operation);
            s32Ret = RK_FAILURE;
        }
        if (s32Ret != RK_SUCCESS) {
            return s32Ret;
        }
        RK_LOGI("Rgn run number of time: %d\n", i + 1);
    }

    s32Ret = TEST_VPSS_StopProc(VpssGrp);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    s32Ret = TEST_VPSS_Stop(VpssGrp, 1);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    return s32Ret;
}

static void mpi_rgn_test_show_options(const TEST_RGN_CTX_S *ctx) {
    RK_PRINT("cmd parse result:\n");
    RK_PRINT("rgn input raw file name   : %s\n", ctx->srcFileRawName);
    RK_PRINT("rgn input bmp file name   : %s\n", ctx->srcFileBmpName);
    RK_PRINT("rgn output file name      : %s\n", ctx->dstSaveFileName);
    RK_PRINT("rgn count                 : %d\n", ctx->s32RgnCount);
    RK_PRINT("rgn operation             : %d\n", ctx->s32Operation);
    RK_PRINT("rgn region pos x          : %d\n", ctx->stRegion.s32X);
    RK_PRINT("rgn region pos y          : %d\n", ctx->stRegion.s32Y);
    RK_PRINT("rgn canvas width          : %d\n", ctx->stRegion.u32Width);
    RK_PRINT("rgn canvas height         : %d\n", ctx->stRegion.u32Height);
    RK_PRINT("rgn raw width             : %d\n", ctx->u32RawWidth);
    RK_PRINT("rgn raw height            : %d\n", ctx->u32RawHeight);
    RK_PRINT("clut num                  : %d\n", ctx->u32ClutNum);
    RK_PRINT("encode codec type         : %d\n", ctx->u32Codec);
}

static const char *const usages[] = {
    "./rk_mpi_rgn_test [-w RECT_W] [-h RECT_H]...",
    NULL,
};

int main(int argc, const char **argv) {
    RK_S32 s32Ret = RK_SUCCESS;
    TEST_RGN_CTX_S stRgnCtx;

    memset(&stRgnCtx, 0, sizeof(TEST_RGN_CTX_S));
    stRgnCtx.s32Operation   = OVERLAY_RGN;
    stRgnCtx.s32LoopCount   = 1;
    stRgnCtx.s32RgnCount    = 1;
    stRgnCtx.u32BmpFormat   = RK_FMT_ARGB8888;
    stRgnCtx.bRgnQp         = RK_FALSE;
    stRgnCtx.u32RawFormat   = RK_FMT_YUV420SP;
    stRgnCtx.u32AttachMod   = RK_ID_VENC;
    stRgnCtx.u32Codec       = RK_VIDEO_ID_AVC;
    stRgnCtx.bRgnInv        = RK_FALSE;
    stRgnCtx.u32ColorLUT0   = 0;
    stRgnCtx.u32ColorLUT1   = 0xffffff;
    stRgnCtx.u32LumThresh   = 100;
    stRgnCtx.u32InvWidth    = 16;
    stRgnCtx.u32InvHeight   = 16;
    stRgnCtx.s32BitmapCount = 1;
    stRgnCtx.s32CanvasCount = 1;
    stRgnCtx.u32Rgntime     = 1;

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("basic options:"),
        OPT_STRING('i', "input_raw_name", &(stRgnCtx.srcFileRawName),
                    "input raw data file name. default(RK_NULL)", NULL, 0, 0),
        OPT_STRING('\0', "input_bmp_name", &(stRgnCtx.srcFileBmpName),
                    "input bmp data file name. e.g.(/userdata/mm.bmp). default(NULL)", NULL, 0, 0),
        OPT_STRING('o', "output_name", &(stRgnCtx.dstSaveFileName),
                    "output stream file name. default(RK_NULL).", NULL, 0, 0),
        OPT_INTEGER('r', "rgn_count", &(stRgnCtx.s32RgnCount),
                    "the number of rgn handle. default(1).", NULL, 0, 0),
        OPT_INTEGER('p', "operation", &(stRgnCtx.s32Operation),
                    "RGN operation. default(0). 0: overlay. 1: cover. 2: mosaic.", NULL, 0, 0),
        OPT_INTEGER('x', "rect_x", &(stRgnCtx.stRegion.s32X),
                    "RGN region pos x. default(0).", NULL, 0, 0),
        OPT_INTEGER('y', "rect_y", &(stRgnCtx.stRegion.s32Y),
                    "RGN region pos x. default(0).", NULL, 0, 0),
        OPT_INTEGER('w', "bmp_w", &(stRgnCtx.stRegion.u32Width),
                    "bmp width. default(0). <required>", NULL, 0, 0),
        OPT_INTEGER('h', "bmp_h", &(stRgnCtx.stRegion.u32Height),
                    "bmp height. default(0). <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "bmp_c", &(stRgnCtx.s32BitmapCount),
                    "set bitmap count. default(1). <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "cav_c", &(stRgnCtx.s32CanvasCount),
                    "update canvas count. default(1). <required>", NULL, 0, 0),
        OPT_INTEGER('q', "rgn_qp", &(stRgnCtx.bRgnQp),
                    "rgn roi qp. default(0). ", NULL, 0, 0),
        OPT_INTEGER('\0', "bAbsQp", &(stRgnCtx.bAbsQp),
                    "rgn Abs qp. default(0). ", NULL, 0, 0),
        OPT_INTEGER('\0', "s32Qp", &(stRgnCtx.s32Qp),
                    "rgn Qp number. default(0). ", NULL, 0, 0),
        OPT_INTEGER('W', "raw_w", &(stRgnCtx.u32RawWidth),
                    "raw width. default(0). <required>", NULL, 0, 0),
        OPT_INTEGER('H', "raw_h", &(stRgnCtx.u32RawHeight),
                    "raw height. default(0). <required>", NULL, 0, 0),
        OPT_INTEGER('F', "raw_fmt", &(stRgnCtx.u32RawFormat),
                    "raw pixel format. default(0). 0: NV12", NULL, 0, 0),
        OPT_INTEGER('f', "format", &(stRgnCtx.u32BmpFormat),
                    "bmp pixel format. default(65552).65552: ARGB8888, 65553: ABGR8888,65554: BGRA8888,65555: RGBA8888" , NULL, 0, 0),
        OPT_INTEGER('\0', "\0", &(stRgnCtx.u32BmpFormat),
                    "65548: ARGB4444, 65549: ABGR4444,65558: BGRA4444,65559: RGBA4444", NULL, 0, 0),
        OPT_INTEGER('\0', "\0", &(stRgnCtx.u32BmpFormat),
                    "65546: ARGB1555, 65547: ABGR1555,65556: RGBA5551, 65557: BGRA5551, 65561: 2BPP", NULL, 0, 0),
        OPT_INTEGER('\0', "clut_num", &(stRgnCtx.u32ClutNum),
                    "set color loop up table num. default(0). range[0, 255].", NULL, 0, 0),
        OPT_INTEGER('\0', "mod", &(stRgnCtx.u32AttachMod),
                    "attach module. default(4), 4: VENC, 6: VPSS"),
        OPT_INTEGER('C', "codec", &(stRgnCtx.u32Codec),
                    "venc encode codec(8:h264, 9:mjpeg, 12:h265, 15:jpeg, ...). default(8)", NULL, 0, 0),
        OPT_INTEGER('\0', "BgAlpha", &(stRgnCtx.u32BgAlpha),
                    "BgAlpha", NULL, 0, 0),
        OPT_INTEGER('\0', "FgAlpha", &(stRgnCtx.u32FgAlpha),
                    "FgAlpha", NULL, 0, 0),
        OPT_INTEGER('\0', "LUT0", &(stRgnCtx.u32ColorLUT0),
                    "set 2bpp 0 color. default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "LUT1", &(stRgnCtx.u32ColorLUT1),
                    "set 2bpp 1 color default(0xffffff)", NULL, 0, 0),
        OPT_INTEGER('\0', "rgn_inv", &(stRgnCtx.bRgnInv),
                    "rgn invert color enable default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "inv_num", &(stRgnCtx.u32LumThresh),
                    "rgn invert color thresh default(100)", NULL, 0, 0),
        OPT_INTEGER('\0', "inv_width", &(stRgnCtx.u32InvWidth),
                    "rgn width of cell inverse area. default(16), 16, 32, 64", NULL, 0, 0),
        OPT_INTEGER('\0', "inv_height", &(stRgnCtx.u32InvHeight),
                    "rgn height of cell inverse area. default(16), 16, 32, 64", NULL, 0, 0),
        OPT_INTEGER('t', "rgn_time", &(stRgnCtx.u32Rgntime),
                    "test rgn number of times. default(1)", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nselect a test case to run.",
                                "\nuse --help for details.");

    argc = argparse_parse(&argparse, argc, argv);
    mpi_rgn_test_show_options(&stRgnCtx);

    if (stRgnCtx.stRegion.u32Width <= 0 ||
            stRgnCtx.stRegion.u32Height <= 0 ||
            stRgnCtx.u32RawWidth <= 0 ||
            stRgnCtx.u32RawHeight <= 0) {
        argparse_usage(&argparse);
        return RK_FAILURE;
    }

    s32Ret = RK_MPI_SYS_Init();
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    switch (stRgnCtx.u32AttachMod) {
      case RK_ID_VENC: {
        s32Ret = unit_test_mpi_rgn_venc(&stRgnCtx);
      } break;
      case RK_ID_VPSS: {
        s32Ret = unit_test_mpi_rgn_vpss(&stRgnCtx);
      } break;
      default:
      break;
    }
    if (s32Ret != RK_SUCCESS) {
        goto __FAILED;
    }

    s32Ret = RK_MPI_SYS_Exit();
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    RK_LOGI("test running ok.");
    return RK_SUCCESS;

__FAILED:
    RK_MPI_SYS_Exit();
    RK_LOGE("test running failed!");
    return s32Ret;
}

