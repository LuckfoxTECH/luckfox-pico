/*
 * Copyright 2020 Rockchip Electronics Co. LTD
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
#define DBG_MOD_ID DBG_MOD_COMB1(RK_ID_VGS)

#include <cstdio>
#include <cerrno>
#include <cstring>
#include <pthread.h>
#include "rk_type.h"
#include "rk_debug.h"
#include "rk_mpi_vgs.h"
#include "rk_mpi_mb.h"
#include "rk_mpi_sys.h"
#include "rk_mpi_cal.h"

#include "test_comm_argparse.h"

typedef enum rkVGS_TASK_TYPE_E {
    VGS_TASK_TYPE_NONE = 0,
    VGS_TASK_TYPE_SCALE,
    VGS_TASK_TYPE_ROTATE,
    VGS_TASK_TYPE_DRAW_LINE,
    VGS_TASK_TYPE_COVER,
    VGS_TASK_TYPE_OSD,
    VGS_TASK_TYPE_MOSAIC,
    VGS_TASK_TYPE_CROP,
    VGS_TASK_TYPE_BUTT
} VGS_TASK_TYPE_E;

typedef enum rkVGS_TASK_MODE_E {
    VGS_TASK_MODE_NONE = 0,
    VGS_TASK_MODE_ONE,
    VGS_TASK_MODE_ALL,
    VGS_TASK_MODE_BUTT
} VGS_TASK_MODE_E;

typedef struct _rkMpiVgsCtx {
    const char *srcFilePath;
    const char *dstFilePath;
    const char *osdFilePath;
    RK_S32  s32LoopCount;
    RK_S32  s32JobNum;
    RK_S32  s32TaskNum;
    RK_S32  s32TaskType;
    RK_S32  s32TaskMode;
    RK_U32  u32TaskArraySize;
    RK_S32  s32Angle;
    RK_S32  s32SrcWidth;
    RK_S32  s32SrcHeight;
    RK_S32  s32SrcVirWidth;
    RK_S32  s32SrcVirHeight;
    RK_S32  s32SrcCompressMode;
    RK_S32  s32SrcPixFormat;
    RK_S32  s32DstWidth;
    RK_S32  s32DstHeight;
    RK_S32  s32DstCompressMode;
    RK_S32  s32DstPixFormat;
    RK_S32  s32OsdWidth;
    RK_S32  s32OsdHeight;
    RK_S32  s32OsdCompressMode;
    RK_S32  s32OsdPixFormat;
    RECT_S  stCropRect;
    RK_S32  s32JobIdx;
    RK_U32  u32SrcSize;
} TEST_VGS_CTX_S;

typedef struct _rkMpiTestTaskCtx {
    VGS_ADD_COVER_S enCoverInfo[VGS_MAX_TASK_NUM];
    VGS_ADD_OSD_S   enOsdInfo[VGS_MAX_TASK_NUM * 8];
    VGS_MOSAIC_S    enMosaicInfo[VGS_MAX_TASK_NUM];
    VGS_DRAW_LINE_S enDrawLine[VGS_MAX_TASK_NUM];
    VGS_CROP_INFO_S stCropInfo;
} TEST_TASK_CTX_S;

RK_S32 test_open_and_read_file(const char *path, void *pu8SrcData, RK_U32 u32SrcSize) {
    FILE  *pFile        = NULL;
    RK_U32 u32ReadSize  = 0;
    pFile = fopen(path, "rb+");
    if (pFile == RK_NULL) {
        RK_LOGE("open path %s failed because %s.", path, strerror(errno));
        return RK_ERR_VGS_NULL_PTR;
    }
    if (pFile) {
         u32ReadSize = fread(pu8SrcData, 1, u32SrcSize, pFile);
         fflush(pFile);
         fclose(pFile);
    }
    RK_LOGD("unit_test_open_source u32ReadSize:%d", u32ReadSize);
    return RK_SUCCESS;
}

RK_S32 unit_test_vgs_generate_source(const char *srcFilePath, MB_BLK *pstSrcBlk, RK_U32 u32SrcSize) {
    RK_S32 s32Ret            = RK_SUCCESS;
    void  *pSrcData          = RK_NULL;
    s32Ret = RK_MPI_SYS_MmzAlloc(pstSrcBlk, RK_NULL, RK_NULL, u32SrcSize);
    if (s32Ret == RK_SUCCESS) {
       pSrcData = RK_MPI_MB_Handle2VirAddr(*pstSrcBlk);
       s32Ret = test_open_and_read_file(srcFilePath, pSrcData, u32SrcSize);
       RK_MPI_SYS_MmzFlushCache(*pstSrcBlk, RK_FALSE);
    }
    return s32Ret;
}

RK_S32 unit_test_vgs_generate_scale_task(
        const VGS_TASK_ATTR_S *pstTask, const TEST_VGS_CTX_S *ctx, MB_BLK srcBlk) {
    RK_S32           s32Ret         = RK_SUCCESS;
    VGS_TASK_ATTR_S *pTaskAttr      = const_cast<VGS_TASK_ATTR_S *>(pstTask);
    VIDEO_FRAME_S    videoFrameIn   = {0};
    VIDEO_FRAME_S    videoFrameOut  = {0};
    RK_U32           u32DstSize     = 0;
    MB_BLK           dstBlk         = RK_NULL;

    videoFrameIn.u32Width       = ctx->s32SrcWidth;
    videoFrameIn.u32Height      = ctx->s32SrcHeight;
    videoFrameIn.u32VirWidth    = ctx->s32SrcVirWidth;
    videoFrameIn.u32VirHeight   = ctx->s32SrcVirHeight;
    videoFrameIn.enPixelFormat  = (PIXEL_FORMAT_E)ctx->s32SrcPixFormat;
    videoFrameIn.u32TimeRef     = 0;
    videoFrameIn.u64PTS         = 0;
    videoFrameIn.pMbBlk         = srcBlk;

    PIC_BUF_ATTR_S stDstPicBufAttr;
    MB_PIC_CAL_S stDstMbPicCalResult;
    stDstPicBufAttr.u32Width = ctx->s32DstWidth;
    stDstPicBufAttr.u32Height = ctx->s32DstHeight;
    stDstPicBufAttr.enPixelFormat = (PIXEL_FORMAT_E)ctx->s32DstPixFormat;
    stDstPicBufAttr.enCompMode = (COMPRESS_MODE_E)ctx->s32DstCompressMode;
    s32Ret = RK_MPI_CAL_VGS_GetPicBufferSize(&stDstPicBufAttr, &stDstMbPicCalResult);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("get picture buffer size failed. err 0x%x", s32Ret);
        return s32Ret;
    }
    u32DstSize = stDstMbPicCalResult.u32MBSize;
    s32Ret = RK_MPI_SYS_MmzAlloc(&dstBlk, RK_NULL, RK_NULL, u32DstSize);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    videoFrameOut.u32Width      = ctx->s32DstWidth;
    videoFrameOut.u32Height     = ctx->s32DstHeight;
    videoFrameOut.u32VirWidth   = stDstMbPicCalResult.u32VirWidth;
    videoFrameOut.u32VirHeight  = stDstMbPicCalResult.u32VirHeight;
    videoFrameOut.enPixelFormat = (PIXEL_FORMAT_E)ctx->s32DstPixFormat;
    videoFrameOut.u32TimeRef    = 0;
    videoFrameOut.u64PTS        = 0;
    videoFrameOut.pMbBlk        = dstBlk;

    pTaskAttr->stImgIn.stVFrame  = videoFrameIn;
    pTaskAttr->stImgOut.stVFrame = videoFrameOut;
    RK_LOGD("src w(%d) h(%d) dst w(%d) h(%d)",
                pTaskAttr->stImgIn.stVFrame.u32Width,
                pTaskAttr->stImgIn.stVFrame.u32Height,
                pTaskAttr->stImgOut.stVFrame.u32Width,
                pTaskAttr->stImgOut.stVFrame.u32Height);

    return s32Ret;
}

RK_S32 unit_test_vgs_generate_rotation_task(
        const VGS_TASK_ATTR_S *pstTask, const TEST_VGS_CTX_S *ctx, MB_BLK srcBlk) {
    RK_S32           s32Ret            = RK_SUCCESS;
    VIDEO_FRAME_S    videoFrameIn      = {0};
    VIDEO_FRAME_S    videoFrameOut     = {0};
    VGS_TASK_ATTR_S *pTaskAttr         = const_cast<VGS_TASK_ATTR_S *>(pstTask);
    RK_U32           u32DstSize        = 0;
    MB_BLK           dstBlk            = RK_NULL;
    RK_S32          s32SrcWidth        = ctx->s32SrcWidth;
    RK_S32          s32SrcHeight       = ctx->s32SrcHeight;

    videoFrameIn.u32Width       = s32SrcWidth;
    videoFrameIn.u32Height      = s32SrcHeight;
    videoFrameIn.u32VirWidth    = ctx->s32SrcVirWidth;
    videoFrameIn.u32VirHeight   = ctx->s32SrcVirHeight;
    videoFrameIn.enPixelFormat  = (PIXEL_FORMAT_E)ctx->s32SrcPixFormat;
    videoFrameIn.u32TimeRef     = 0;
    videoFrameIn.u64PTS         = 0;
    videoFrameIn.pMbBlk         = srcBlk;

    PIC_BUF_ATTR_S stDstPicBufAttr;
    MB_PIC_CAL_S stDstMbPicCalResult;

    videoFrameOut.u32Width      = s32SrcWidth;
    videoFrameOut.u32Height     = s32SrcHeight;
    if ((ctx->s32Angle == ROTATION_90) || (ctx->s32Angle == ROTATION_270)) {
        videoFrameOut.u32Width  = s32SrcHeight;
        videoFrameOut.u32Height = s32SrcWidth;
    }
    stDstPicBufAttr.u32Width = videoFrameOut.u32Width;
    stDstPicBufAttr.u32Height = videoFrameOut.u32Height;
    stDstPicBufAttr.enPixelFormat = (PIXEL_FORMAT_E)ctx->s32DstPixFormat;
    stDstPicBufAttr.enCompMode = (COMPRESS_MODE_E)ctx->s32DstCompressMode;
    s32Ret = RK_MPI_CAL_VGS_GetPicBufferSize(&stDstPicBufAttr, &stDstMbPicCalResult);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("get picture buffer size failed. err 0x%x", s32Ret);
        return s32Ret;
    }
    u32DstSize = stDstMbPicCalResult.u32MBSize;
    s32Ret = RK_MPI_SYS_MmzAlloc(&dstBlk, RK_NULL, RK_NULL, u32DstSize);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    videoFrameOut.enPixelFormat = (PIXEL_FORMAT_E)ctx->s32DstPixFormat;
    videoFrameOut.u32VirWidth   = stDstMbPicCalResult.u32VirWidth;
    videoFrameOut.u32VirHeight  = stDstMbPicCalResult.u32VirHeight;
    videoFrameOut.u32TimeRef    = 0;
    videoFrameOut.u64PTS        = 0;
    videoFrameOut.pMbBlk        = dstBlk;

    pTaskAttr->stImgIn.stVFrame = videoFrameIn;
    pTaskAttr->stImgOut.stVFrame = videoFrameOut;
    RK_LOGD("src w(%d) h(%d) dst w(%d) h(%d)",
                pTaskAttr->stImgIn.stVFrame.u32Width,
                pTaskAttr->stImgIn.stVFrame.u32Height,
                pTaskAttr->stImgOut.stVFrame.u32Width,
                pTaskAttr->stImgOut.stVFrame.u32Height);

    return s32Ret;
}

RK_S32 unit_test_vgs_generate_crop_task(
        const VGS_TASK_ATTR_S *pstTask, VGS_CROP_INFO_S *pstCropInfo, const TEST_VGS_CTX_S *ctx, MB_BLK srcBlk) {
    RK_S32           s32Ret         = RK_SUCCESS;
    VGS_TASK_ATTR_S *pTaskAttr      = const_cast<VGS_TASK_ATTR_S *>(pstTask);
    VIDEO_FRAME_S    videoFrameIn   = {0};
    VIDEO_FRAME_S    videoFrameOut  = {0};
    RK_U32           u32DstSize     = 0;
    MB_BLK           dstBlk         = RK_NULL;

    videoFrameIn.u32Width       = ctx->s32SrcWidth;
    videoFrameIn.u32Height      = ctx->s32SrcHeight;
    videoFrameIn.u32VirWidth    = ctx->s32SrcVirWidth;
    videoFrameIn.u32VirHeight   = ctx->s32SrcVirHeight;
    videoFrameIn.enPixelFormat  = (PIXEL_FORMAT_E)ctx->s32SrcPixFormat;
    videoFrameIn.u32TimeRef     = 0;
    videoFrameIn.u64PTS         = 0;
    videoFrameIn.pMbBlk         = srcBlk;

    PIC_BUF_ATTR_S stDstPicBufAttr;
    MB_PIC_CAL_S stDstMbPicCalResult;

    stDstPicBufAttr.u32Width = ctx->s32DstWidth;
    stDstPicBufAttr.u32Height = ctx->s32DstHeight;
    stDstPicBufAttr.enPixelFormat = (PIXEL_FORMAT_E)ctx->s32DstPixFormat;
    stDstPicBufAttr.enCompMode = (COMPRESS_MODE_E)ctx->s32DstCompressMode;
    s32Ret = RK_MPI_CAL_VGS_GetPicBufferSize(&stDstPicBufAttr, &stDstMbPicCalResult);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("get picture buffer size failed. err 0x%x", s32Ret);
        return s32Ret;
    }
    u32DstSize = stDstMbPicCalResult.u32MBSize;
    s32Ret = RK_MPI_SYS_MmzAlloc(&dstBlk, RK_NULL, RK_NULL, u32DstSize);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    videoFrameOut.u32Width      = ctx->s32DstWidth;
    videoFrameOut.u32Height     = ctx->s32DstHeight;
    videoFrameOut.enPixelFormat = (PIXEL_FORMAT_E)ctx->s32DstPixFormat;
    videoFrameOut.u32VirWidth   = stDstMbPicCalResult.u32VirWidth;
    videoFrameOut.u32VirHeight  = stDstMbPicCalResult.u32VirHeight;
    videoFrameOut.u32TimeRef    = 0;
    videoFrameOut.u64PTS        = 0;
    videoFrameOut.pMbBlk        = dstBlk;

    pTaskAttr->stImgIn.stVFrame  = videoFrameIn;
    pTaskAttr->stImgOut.stVFrame = videoFrameOut;
    pstCropInfo->enCropCoordinate = VGS_CROP_RATIO_COOR;
    pstCropInfo->stCropRect.s32X = ctx->stCropRect.s32X;
    pstCropInfo->stCropRect.s32Y = ctx->stCropRect.s32Y;
    pstCropInfo->stCropRect.u32Width = ctx->stCropRect.u32Width;
    pstCropInfo->stCropRect.u32Height = ctx->stCropRect.u32Height;
    RK_LOGD("src w(%d) h(%d) dst w(%d) h(%d)",
                pTaskAttr->stImgIn.stVFrame.u32Width,
                pTaskAttr->stImgIn.stVFrame.u32Height,
                pTaskAttr->stImgOut.stVFrame.u32Width,
                pTaskAttr->stImgOut.stVFrame.u32Height);

    return s32Ret;
}

RK_VOID unit_test_set_vgs_cover_array_info(
        VGS_ADD_COVER_S *pstVgsAddCover, const TEST_VGS_CTX_S *ctx, RK_U32 u32ArraySize) {
    RK_U32   u32Color[VGS_MAX_TASK_NUM] = {0xff00ff00, 0x000000ff, 0x00ff0000,  0x00ff00ff,  0xff00ffff};
    RK_U32   u32Ratio[VGS_MAX_TASK_NUM] = {2, 4,  6,  8,  10,  2,  4,  6};
    RK_S32   s32SrcWidth     = ctx->s32SrcWidth;
    RK_S32   s32SrcHeight    = ctx->s32SrcHeight;
    for (RK_S32 i = 0; i < u32ArraySize; i++) {
        if (u32Ratio[i]) {
            pstVgsAddCover[i].enCoverType         = COVER_RECT;
            pstVgsAddCover[i].stDstRect.s32X      = s32SrcWidth / u32Ratio[i];
            pstVgsAddCover[i].stDstRect.s32Y      = s32SrcHeight / u32Ratio[i];
            pstVgsAddCover[i].stDstRect.u32Width  = s32SrcWidth / u32Ratio[i];
            pstVgsAddCover[i].stDstRect.u32Height = s32SrcHeight / u32Ratio[i];
            pstVgsAddCover[i].u32Color            = u32Color[i];  // green
        }
    }
}

RK_S32 unit_test_vgs_generate_cover_task_array(
        const VGS_TASK_ATTR_S *pstTask, VGS_ADD_COVER_S *pstVgsAddCover,
        const TEST_VGS_CTX_S *ctx, RK_U32 u32ArraySize, MB_BLK srcBlk) {
    RK_S32           s32Ret         = RK_SUCCESS;
    VIDEO_FRAME_S    videoFrameOut  = {0};
    VGS_TASK_ATTR_S *pTaskAttr      = const_cast<VGS_TASK_ATTR_S *>(pstTask);
    MB_BLK           dstBlk         = RK_NULL;

    s32Ret = RK_MPI_SYS_MmzAlloc(&dstBlk, RK_NULL, RK_NULL, ctx->u32SrcSize);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    memcpy(RK_MPI_MB_Handle2VirAddr(dstBlk),
           RK_MPI_MB_Handle2VirAddr(srcBlk), ctx->u32SrcSize);

    videoFrameOut.u32Width      = ctx->s32SrcWidth;
    videoFrameOut.u32Height     = ctx->s32SrcHeight;
    videoFrameOut.u32VirWidth   = ctx->s32SrcVirWidth;
    videoFrameOut.u32VirHeight  = ctx->s32SrcVirHeight;
    videoFrameOut.enPixelFormat = (PIXEL_FORMAT_E)ctx->s32SrcPixFormat;
    videoFrameOut.u32TimeRef    = 0;
    videoFrameOut.u64PTS        = 0;
    videoFrameOut.pMbBlk        = dstBlk;

    pTaskAttr->stImgIn.stVFrame = videoFrameOut;
    pTaskAttr->stImgOut.stVFrame = videoFrameOut;
    unit_test_set_vgs_cover_array_info(pstVgsAddCover, ctx, u32ArraySize);
    return s32Ret;
}

RK_S32 unit_test_set_vgs_osd_array_info(
        VGS_ADD_OSD_S *pstOsd, const TEST_VGS_CTX_S *ctx, RK_U32 u32ArraySize) {
    RK_U32          u32Ratio[VGS_MAX_TASK_NUM] = {2, 4,  6,  8,  10,  2,  4,  6};
    RK_U32          u32OsdWidth                = ctx->s32OsdWidth;
    RK_U32          u32OsdHeight               = ctx->s32OsdHeight;
    RK_U32          u32OsdPixFormat            = RK_FMT_YUV420SP;
    RK_U32          u32OsdCompressMode         = COMPRESS_MODE_NONE;
    RK_S32          s32Ret                     = RK_SUCCESS;
    MB_BLK          osdBlk                     = RK_NULL;
    RK_S32          s32SrcWidth                = ctx->s32SrcWidth;
    RK_S32          s32SrcHeight               = ctx->s32SrcHeight;
    const char      *osdFilePath               = ctx->osdFilePath;
    RK_U32          u32OsdSize                 = 0;
    PIC_BUF_ATTR_S stPicBufAttr;
    MB_PIC_CAL_S stMbPicCalResult;

    stPicBufAttr.u32Width = u32OsdWidth;
    stPicBufAttr.u32Height = u32OsdHeight;
    stPicBufAttr.enPixelFormat = (PIXEL_FORMAT_E)u32OsdPixFormat;
    stPicBufAttr.enCompMode = (COMPRESS_MODE_E)u32OsdCompressMode;
    s32Ret = RK_MPI_CAL_VGS_GetPicBufferSize(&stPicBufAttr, &stMbPicCalResult);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("get picture buffer size failed. err 0x%x", s32Ret);
        return s32Ret;
    }
    u32OsdSize = stMbPicCalResult.u32MBSize;
    for (RK_S32 i = 0; i < u32ArraySize; i++) {
        s32Ret = unit_test_vgs_generate_source(osdFilePath, &osdBlk, u32OsdSize);

        if (s32Ret != RK_SUCCESS) {
            return s32Ret;
        }
        if (u32Ratio[i]) {
            pstOsd[i].stRect.s32X      = s32SrcWidth / u32Ratio[i];
            pstOsd[i].stRect.s32Y      = s32SrcHeight /  u32Ratio[i];
            pstOsd[i].stRect.u32Width  = u32OsdWidth;
            pstOsd[i].stRect.u32Height = u32OsdHeight;
            pstOsd[i].enPixelFmt       = RK_FMT_YUV420SP;
            pstOsd[i].u32FgAlpha       = 0xff;  // no alpha
            pstOsd[i].pMbBlk           = osdBlk;
        }
    }
    return s32Ret;
}

RK_S32 unit_test_vgs_generate_osd_task_array(
        const VGS_TASK_ATTR_S *pstTask, VGS_ADD_OSD_S *pstOsd,
        const TEST_VGS_CTX_S *ctx, RK_U32 u32ArraySize, MB_BLK srcBlk) {
    RK_S32           s32Ret            = RK_SUCCESS;
    VGS_TASK_ATTR_S *pTaskAttr         = const_cast<VGS_TASK_ATTR_S *>(pstTask);
    VIDEO_FRAME_S    videoFrameOut     = {0};
    MB_BLK           dstBlk            = RK_NULL;

    s32Ret = RK_MPI_SYS_MmzAlloc(&dstBlk, RK_NULL, RK_NULL, ctx->u32SrcSize);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    memcpy(RK_MPI_MB_Handle2VirAddr(dstBlk),
           RK_MPI_MB_Handle2VirAddr(srcBlk), ctx->u32SrcSize);

    videoFrameOut.u32Width       = ctx->s32SrcWidth;
    videoFrameOut.u32Height      = ctx->s32SrcHeight;
    videoFrameOut.u32VirWidth    = ctx->s32SrcVirWidth;
    videoFrameOut.u32VirHeight   = ctx->s32SrcVirHeight;
    videoFrameOut.enPixelFormat  = (PIXEL_FORMAT_E)ctx->s32SrcPixFormat;
    videoFrameOut.u32TimeRef     = 0;
    videoFrameOut.u64PTS         = 0;
    videoFrameOut.pMbBlk        = dstBlk;

    pTaskAttr->stImgOut.stVFrame = videoFrameOut;
    pTaskAttr->stImgIn.stVFrame = videoFrameOut;
    unit_test_set_vgs_osd_array_info(pstOsd, ctx, u32ArraySize);
    return s32Ret;
}

RK_VOID unit_test_set_vgs_drawline_array_info(
        VGS_DRAW_LINE_S *pstDrawLine, const TEST_VGS_CTX_S *ctx, RK_U32 u32ArraySize) {
    RK_U32   u32Color[VGS_MAX_TASK_NUM] = {0xff00ff00, 0x0000ff00, 0xff000000,  0x00ff00ff,  0xff00ff00};
    RK_U32   u32Ratio[VGS_MAX_TASK_NUM] = {2, 4,  6,  8,  10,  2,  4,  6};
    RK_U32   u32Thick[VGS_MAX_TASK_NUM] = {10, 5,  20,  15,  10,  2,  4,  6};
    RK_S32   s32SrcWidth                = ctx->s32SrcWidth;
    RK_S32   s32SrcHeight               = ctx->s32SrcHeight;
    for (RK_S32 i = 0; i < u32ArraySize; i++) {
        if (u32Ratio[i]) {
            pstDrawLine[i].stStartPoint.s32X      = s32SrcWidth / u32Ratio[i];
            pstDrawLine[i].stStartPoint.s32Y      = s32SrcHeight /  u32Ratio[i];
            pstDrawLine[i].stEndPoint.s32X        = s32SrcWidth / u32Ratio[i] * 2;
            pstDrawLine[i].stEndPoint.s32Y        = s32SrcHeight / u32Ratio[i] * 2;
            pstDrawLine[i].u32Color               = u32Color[i];  // green
            pstDrawLine[i].u32Thick               = u32Thick[i];
        }
    }
}

RK_S32 unit_test_vgs_generate_drawline_task_array(
        const VGS_TASK_ATTR_S *pstTask, VGS_DRAW_LINE_S *pstDrawLine,
        const TEST_VGS_CTX_S *ctx, RK_U32 u32ArraySize, MB_BLK srcBlk) {
    RK_S32           s32Ret             = RK_SUCCESS;
    VGS_TASK_ATTR_S *pTaskAttr          = const_cast<VGS_TASK_ATTR_S *>(pstTask);
    VIDEO_FRAME_S    videoFrameOut      = {0};
    MB_BLK           dstBlk             = RK_NULL;

    s32Ret = RK_MPI_SYS_MmzAlloc(&dstBlk, RK_NULL, RK_NULL, ctx->u32SrcSize);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    memcpy(RK_MPI_MB_Handle2VirAddr(dstBlk),
           RK_MPI_MB_Handle2VirAddr(srcBlk), ctx->u32SrcSize);

    videoFrameOut.u32Width      = ctx->s32SrcWidth;
    videoFrameOut.u32Height     = ctx->s32SrcHeight;
    videoFrameOut.u32VirWidth   = ctx->s32SrcVirWidth;
    videoFrameOut.u32VirHeight  = ctx->s32SrcVirHeight;
    videoFrameOut.enPixelFormat = (PIXEL_FORMAT_E)ctx->s32SrcPixFormat;
    videoFrameOut.u32TimeRef    = 0;
    videoFrameOut.u64PTS        = 0;
    videoFrameOut.pMbBlk        = dstBlk;

    pTaskAttr->stImgOut.stVFrame = videoFrameOut;
    pTaskAttr->stImgIn.stVFrame = videoFrameOut;
    unit_test_set_vgs_drawline_array_info(pstDrawLine, ctx, u32ArraySize);
    return s32Ret;
}

RK_VOID unit_test_set_vgs_mosaic_array_info(
        VGS_MOSAIC_S *pstVgsMosaic, const TEST_VGS_CTX_S *ctx, RK_U32 u32ArraySize) {
    VGS_MOSAIC_BLK_SIZE_E u32BlkSize[VGS_MAX_TASK_NUM]  = {RK_MOSAIC_BLK_SIZE_8, RK_MOSAIC_BLK_SIZE_16,
        RK_MOSAIC_BLK_SIZE_32,  RK_MOSAIC_BLK_SIZE_64};
    RK_U32               u32Ratio[VGS_MAX_TASK_NUM]     = {4, 2, 6, 4, 10, 2, 4, 6};
    RK_S32               s32SrcWidth                    = ctx->s32SrcWidth;
    RK_S32               s32SrcHeight                   = ctx->s32SrcHeight;
    for (RK_S32 i = 0; i < u32ArraySize; i++) {
        if (u32Ratio[i]) {
            pstVgsMosaic[i].enBlkSize = u32BlkSize[i];
            pstVgsMosaic[i].stDstRect.s32X      = s32SrcWidth / u32Ratio[i];
            pstVgsMosaic[i].stDstRect.s32Y      = s32SrcHeight /  u32Ratio[i];
            pstVgsMosaic[i].stDstRect.u32Width  = s32SrcWidth /  u32Ratio[i];
            pstVgsMosaic[i].stDstRect.u32Height = s32SrcHeight /  u32Ratio[i];
        }
    }
}

RK_S32 unit_test_vgs_generate_mosaic_task_array(
        const VGS_TASK_ATTR_S *pstTask, VGS_MOSAIC_S *astVgsMosaic,
        const TEST_VGS_CTX_S *ctx, RK_U32 u32ArraySize, MB_BLK srcBlk) {
    RK_S32           s32Ret           = RK_SUCCESS;
    VIDEO_FRAME_S    videoFrameOut    = {0};
    VGS_TASK_ATTR_S *pTaskAttr        = const_cast<VGS_TASK_ATTR_S *>(pstTask);
    MB_BLK           dstBlk           = RK_NULL;

    s32Ret = RK_MPI_SYS_MmzAlloc(&dstBlk, RK_NULL, RK_NULL, ctx->u32SrcSize);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    memcpy(RK_MPI_MB_Handle2VirAddr(dstBlk),
           RK_MPI_MB_Handle2VirAddr(srcBlk), ctx->u32SrcSize);

    videoFrameOut.u32Width      = ctx->s32SrcWidth;
    videoFrameOut.u32Height     = ctx->s32SrcHeight;
    videoFrameOut.u32VirWidth   = ctx->s32SrcVirWidth;
    videoFrameOut.u32VirHeight  = ctx->s32SrcVirHeight;
    videoFrameOut.enPixelFormat = (PIXEL_FORMAT_E)ctx->s32SrcPixFormat;
    videoFrameOut.u32TimeRef    = 0;
    videoFrameOut.u64PTS        = 0;
    videoFrameOut.pMbBlk        = dstBlk;

    pTaskAttr->stImgIn.stVFrame = videoFrameOut;
    pTaskAttr->stImgOut.stVFrame = videoFrameOut;
    unit_test_set_vgs_mosaic_array_info(astVgsMosaic, ctx, u32ArraySize);
    return s32Ret;
}

RK_S32 unit_test_vgs_output_one_task(
        const VGS_TASK_ATTR_S *pstTask, const TEST_VGS_CTX_S *ctx, RK_U32 jobId, RK_U32 taskId) {
    RK_S32           s32Ret             = RK_SUCCESS;
    char             yuv_out_path[1024] = {0};
    VGS_TASK_ATTR_S *pTaskAttr          = const_cast<VGS_TASK_ATTR_S *>(pstTask);
    VIDEO_FRAME_S    videoFrameOut      = pTaskAttr->stImgOut.stVFrame;
    MB_BLK           bBlk               = videoFrameOut.pMbBlk;
    RK_VOID         *pstFrame           = RK_MPI_MB_Handle2VirAddr(bBlk);
    const char      *dstFilePath        = ctx->dstFilePath;
    PIC_BUF_ATTR_S stPicBufAttr;
    MB_PIC_CAL_S stMbPicCalResult;

    stPicBufAttr.u32Width = videoFrameOut.u32Width;
    stPicBufAttr.u32Height = videoFrameOut.u32Height;
    stPicBufAttr.enPixelFormat = videoFrameOut.enPixelFormat;
    stPicBufAttr.enCompMode = videoFrameOut.enCompressMode;
    s32Ret = RK_MPI_CAL_VGS_GetPicBufferSize(&stPicBufAttr, &stMbPicCalResult);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("get picture buffer size failed. err 0x%x", s32Ret);
        return s32Ret;
    }
    RK_U32 u32OutputSize = stMbPicCalResult.u32MBSize;

    snprintf(yuv_out_path,
            sizeof(yuv_out_path),
            "%svgs_out_%dx%d_j_%d_t_%d.yuv",
            dstFilePath,
            videoFrameOut.u32VirWidth,
            videoFrameOut.u32VirHeight, jobId, taskId);

    FILE *file = fopen(yuv_out_path, "wb+");
    if (file == RK_NULL) {
        RK_LOGE("open path %s failed because %s.", yuv_out_path, strerror(errno));
        return RK_ERR_SYS_NULL_PTR;
    }
    if (file) {
        if (pstFrame) {
            RK_LOGD("get frame data = %p, size = %d, bBlk:%p ", pstFrame, u32OutputSize, bBlk);
            RK_MPI_SYS_MmzFlushCache(bBlk, RK_TRUE);
            fwrite(pstFrame, 1, u32OutputSize, file);
            fflush(file);
        }
        fclose(file);
        file = NULL;
    }
    RK_MPI_MB_ReleaseMB(bBlk);
    return RK_SUCCESS;
}

RK_S32 unit_test_vgs_out_all_task(
        VGS_HANDLE hHandle, VGS_TASK_ATTR_S *pstTask,
        const TEST_VGS_CTX_S *ctx, RK_S32 jobId, RK_U32 u32TestTime) {
    RK_S32 s32Ret = RK_SUCCESS;
    for (RK_S32 i =0; i < u32TestTime; i++) {
        unit_test_vgs_output_one_task(&pstTask[i], ctx, jobId, i);
    }
    return RK_SUCCESS;
}

RK_S32 unit_test_release_task_ctx(TEST_TASK_CTX_S *taskCtx) {
    RK_S32   s32Ret               = RK_SUCCESS;
    VGS_ADD_OSD_S   *enOsdInfo    = taskCtx->enOsdInfo;
    for (RK_S32 i = 0; i < VGS_MAX_TASK_NUM; i++) {
        if ((enOsdInfo[i]).pMbBlk) {
            s32Ret = RK_MPI_MB_ReleaseMB((enOsdInfo[i]).pMbBlk);
        }
    }
    return s32Ret;
}

RK_S32 unit_test_vgs_add_one_task(
        VGS_HANDLE hHandle, VGS_TASK_ATTR_S *pstTask, const TEST_VGS_CTX_S *ctx,
        TEST_TASK_CTX_S *taskCtx, MB_BLK srcBlk, RK_S32 taskId) {
    RK_S32          s32Ret                          = RK_SUCCESS;
    VGS_ADD_COVER_S *enCoverInfo                    = taskCtx->enCoverInfo;
    VGS_ADD_OSD_S   *enOsdInfo                      = taskCtx->enOsdInfo;
    VGS_MOSAIC_S    *enMosaicInfo                   = taskCtx->enMosaicInfo;
    VGS_DRAW_LINE_S *enDrawLine                     = taskCtx->enDrawLine;
    VGS_CROP_INFO_S *pstCropInfo                    = &(taskCtx->stCropInfo);
    RK_S32          testTaskType                    = ctx->s32TaskType;
    RK_S32          s32TaskNum                      = ctx->s32TaskNum;
    RK_U32          u32ArraySize                    = ctx->u32TaskArraySize;
    RK_S32          s32Angle                        = ctx->s32Angle;
    switch (testTaskType) {
        case VGS_TASK_TYPE_SCALE:
            s32Ret = unit_test_vgs_generate_scale_task(pstTask, ctx, srcBlk);
            if (s32Ret != RK_SUCCESS) {
                return s32Ret;
            }
            s32Ret = RK_MPI_VGS_AddScaleTask(hHandle, pstTask, VGS_SCLCOEF_NORMAL);
            break;
        case VGS_TASK_TYPE_ROTATE:
            s32Ret = unit_test_vgs_generate_rotation_task(pstTask, ctx, srcBlk);
            if (s32Ret != RK_SUCCESS) {
                return s32Ret;
            }
            s32Ret = RK_MPI_VGS_AddRotationTask(hHandle, pstTask, ROTATION_E(s32Angle));
            break;
        case VGS_TASK_TYPE_CROP:
            s32Ret = unit_test_vgs_generate_crop_task(pstTask, pstCropInfo, ctx, srcBlk);
            if (s32Ret != RK_SUCCESS) {
                return s32Ret;
            }
            s32Ret = RK_MPI_VGS_AddCropTask(hHandle, pstTask, pstCropInfo);
            break;
        case VGS_TASK_TYPE_DRAW_LINE:
            s32Ret = unit_test_vgs_generate_drawline_task_array(pstTask, enDrawLine, ctx, u32ArraySize, srcBlk);
            if (s32Ret != RK_SUCCESS) {
                return s32Ret;
            }
            if (u32ArraySize == 1) {
                s32Ret = RK_MPI_VGS_AddDrawLineTask(hHandle, pstTask, enDrawLine);
            } else {
                s32Ret = RK_MPI_VGS_AddDrawLineTaskArray(hHandle, pstTask, enDrawLine, u32ArraySize);
            }
            break;
        case VGS_TASK_TYPE_COVER:
            s32Ret = unit_test_vgs_generate_cover_task_array(pstTask, enCoverInfo, ctx, u32ArraySize, srcBlk);
            if (s32Ret != RK_SUCCESS) {
                return s32Ret;
            }
            if (u32ArraySize == 1) {
                s32Ret = RK_MPI_VGS_AddCoverTask(hHandle, pstTask, enCoverInfo);
            } else {
                s32Ret = RK_MPI_VGS_AddCoverTaskArray(hHandle, pstTask, enCoverInfo, u32ArraySize);
            }
            break;
        case VGS_TASK_TYPE_OSD:
            s32Ret = unit_test_vgs_generate_osd_task_array(
                pstTask, &enOsdInfo[taskId*u32ArraySize], ctx, u32ArraySize, srcBlk);
            if (s32Ret != RK_SUCCESS) {
                return s32Ret;
            }
            if (u32ArraySize == 1) {
                s32Ret = RK_MPI_VGS_AddOsdTask(hHandle, pstTask, enOsdInfo);
            } else {
                s32Ret = RK_MPI_VGS_AddOsdTaskArray(hHandle, pstTask, &enOsdInfo[taskId*u32ArraySize], u32ArraySize);
            }
            break;
        case VGS_TASK_TYPE_MOSAIC:
            s32Ret = unit_test_vgs_generate_mosaic_task_array(pstTask, enMosaicInfo, ctx, u32ArraySize, srcBlk);
            if (s32Ret != RK_SUCCESS) {
                return s32Ret;
            }
            if (u32ArraySize == 1) {
                s32Ret = RK_MPI_VGS_AddMosaicTask(hHandle, pstTask, enMosaicInfo);
            } else {
                s32Ret = RK_MPI_VGS_AddMosaicTaskArray(hHandle, pstTask, enMosaicInfo, u32ArraySize);
            }
            break;
        default:
            break;
    }
    if (s32Ret != RK_SUCCESS) {
        RK_MPI_VGS_CancelJob(hHandle);
        return s32Ret;
    }
    if (taskId == (s32TaskNum -1)) {
        s32Ret = RK_MPI_VGS_EndJob(hHandle);
    }
    if (s32Ret != RK_SUCCESS) {
        RK_MPI_VGS_CancelJob(hHandle);
    }
    return s32Ret;
}

RK_S32 unit_test_vgs_add_all_task(
       VGS_HANDLE hHandle, VGS_TASK_ATTR_S *pstTask, const TEST_VGS_CTX_S *ctx, TEST_TASK_CTX_S *taskCtx,
       RK_U32 u32TestTime, MB_BLK srcBlk, RK_S32 *u32AllTestTime) {
    RK_S32          s32Ret                          = RK_SUCCESS;
    VGS_ADD_COVER_S *enCoverInfo                    = taskCtx->enCoverInfo;
    VGS_ADD_OSD_S   *enOsdInfo                      = taskCtx->enOsdInfo;
    VGS_MOSAIC_S    *enMosaicInfo                   = taskCtx->enMosaicInfo;
    VGS_DRAW_LINE_S *enDrawLine                     = taskCtx->enDrawLine;
    RK_U32          u32TaskIndex                    = 0;
    RK_U32          u32ArraySize                    = 4;
    RK_U32          u32ScaleWidth                   = 0;
    RK_U32          u32ScaleHeight                  = 0;
    RK_U32          u32Angle                        = ctx->s32Angle;
    for (RK_S32 i = 0; i < u32TestTime; i++) {
        u32ScaleWidth = ctx->s32DstWidth;
        u32ScaleHeight = ctx->s32DstHeight;
        s32Ret = unit_test_vgs_generate_scale_task(&pstTask[u32TaskIndex], ctx, srcBlk);
        if (s32Ret != RK_SUCCESS) {
            return s32Ret;
        }
        s32Ret = RK_MPI_VGS_AddScaleTask(hHandle, &pstTask[u32TaskIndex], VGS_SCLCOEF_NORMAL);
        if (s32Ret != RK_SUCCESS) {
            RK_MPI_VGS_CancelJob(hHandle);
            return s32Ret;
        }
        u32TaskIndex++;
    }
    for (RK_S32 i = 0; i < u32TestTime; i++) {
        s32Ret = unit_test_vgs_generate_rotation_task(&pstTask[u32TaskIndex], ctx, srcBlk);
        if (s32Ret != RK_SUCCESS) {
            return s32Ret;
        }
        s32Ret = RK_MPI_VGS_AddRotationTask(hHandle, &pstTask[u32TaskIndex], (ROTATION_E)u32Angle);
        if (s32Ret != RK_SUCCESS) {
            RK_MPI_VGS_CancelJob(hHandle);
            return s32Ret;
        }
        u32TaskIndex++;
    }
    for (RK_S32 i = 0; i < u32TestTime; i++) {
        s32Ret = unit_test_vgs_generate_cover_task_array(
            &pstTask[u32TaskIndex], enCoverInfo, ctx, u32ArraySize, srcBlk);
        if (s32Ret != RK_SUCCESS) {
            return s32Ret;
        }
        s32Ret = RK_MPI_VGS_AddCoverTaskArray(hHandle, &pstTask[u32TaskIndex], enCoverInfo, u32ArraySize);
        if (s32Ret != RK_SUCCESS) {
            RK_MPI_VGS_CancelJob(hHandle);
            return s32Ret;
        }
        u32TaskIndex++;
    }
    for (RK_S32 i = 0; i < u32TestTime; i++) {
        s32Ret = unit_test_vgs_generate_osd_task_array(
            &pstTask[u32TaskIndex],  &enOsdInfo[i*u32ArraySize], ctx, u32ArraySize, srcBlk);
        if (s32Ret != RK_SUCCESS) {
            return s32Ret;
        }
        RK_MPI_VGS_AddOsdTaskArray(hHandle, &pstTask[u32TaskIndex], &enOsdInfo[i*u32ArraySize], u32ArraySize);
        if (s32Ret != RK_SUCCESS) {
            RK_MPI_VGS_CancelJob(hHandle);
            return s32Ret;
        }
        u32TaskIndex++;
    }
    for (RK_S32 i = 0; i < u32TestTime; i++) {
        s32Ret = unit_test_vgs_generate_drawline_task_array(
            &pstTask[u32TaskIndex], enDrawLine, ctx, u32ArraySize, srcBlk);
        if (s32Ret != RK_SUCCESS) {
            return s32Ret;
        }
        s32Ret = RK_MPI_VGS_AddDrawLineTaskArray(hHandle, &pstTask[u32TaskIndex], enDrawLine, u32ArraySize);
        if (s32Ret != RK_SUCCESS) {
            RK_MPI_VGS_CancelJob(hHandle);
            return s32Ret;
        }
        u32TaskIndex++;
    }

    for (RK_S32 i = 0; i < u32TestTime; i++) {
        s32Ret = unit_test_vgs_generate_mosaic_task_array(
            &pstTask[u32TaskIndex], enMosaicInfo, ctx, u32ArraySize, srcBlk);
        if (s32Ret != RK_SUCCESS) {
            return s32Ret;
        }
        s32Ret = RK_MPI_VGS_AddMosaicTaskArray(hHandle, &pstTask[u32TaskIndex], enMosaicInfo, u32ArraySize);
        if (s32Ret != RK_SUCCESS) {
            RK_MPI_VGS_CancelJob(hHandle);
            return s32Ret;
        }
        u32TaskIndex++;
    }

    s32Ret = RK_MPI_VGS_EndJob(hHandle);
    if (s32Ret != RK_SUCCESS) {
        RK_MPI_VGS_CancelJob(hHandle);
        return s32Ret;
    }

    *u32AllTestTime = u32TaskIndex;
    return s32Ret;
}

RK_U32 unit_test_vgs_get_size(TEST_VGS_CTX_S *ctx) {
    RK_S32 s32Ret = RK_SUCCESS;
    PIC_BUF_ATTR_S stPicBufAttr;
    MB_PIC_CAL_S   stMbPicCalResult;

    if (ctx->s32SrcVirWidth == 0) {
        ctx->s32SrcVirWidth = ctx->s32SrcWidth;
    }
    if (ctx->s32SrcVirHeight == 0) {
        ctx->s32SrcVirHeight = ctx->s32SrcHeight;
    }

    stPicBufAttr.u32Width = ctx->s32SrcVirWidth;
    stPicBufAttr.u32Height = ctx->s32SrcVirHeight;
    stPicBufAttr.enPixelFormat = (PIXEL_FORMAT_E)ctx->s32SrcPixFormat;
    stPicBufAttr.enCompMode = (COMPRESS_MODE_E)ctx->s32SrcCompressMode;
    s32Ret = RK_MPI_CAL_COMM_GetPicBufferSize(&stPicBufAttr, &stMbPicCalResult);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("get picture buffer size failed. err 0x%x", s32Ret);
        return s32Ret;
    }

    return stMbPicCalResult.u32MBSize;
}

RK_S32 unit_test_vgs_one_job(TEST_VGS_CTX_S *ctx) {
    RK_S32 s32Ret                               = RK_SUCCESS;
    MB_BLK srcBlk                               = NULL;
    RK_S32  s32TaskNum                          = ctx->s32TaskNum;
    RK_S32  s32JobId                            = ctx->s32JobIdx;
    RK_S32  s32TaskMode                         = ctx->s32TaskMode;
    const char *srcFilePath                     = ctx->srcFilePath;
    RK_S32  s32SrcPixFormat                     = ctx->s32SrcPixFormat;
    RK_S32  s32SrcCompressMode                  = ctx->s32SrcCompressMode;
    RK_S32 u32OneTaskTestTime                   = s32TaskNum;
    RK_S32 u32AllTaskTestTime                   = 0;
    VGS_HANDLE hHandleAll[VGS_MAX_JOB_NUM]      = {0};
    VGS_TASK_ATTR_S stTask[VGS_MAX_TASK_NUM]    = {0};
    VGS_HANDLE hHandle                          = hHandleAll[s32JobId];
    RK_U32 s32SrcWidth                          = ctx->s32SrcWidth;
    RK_U32 s32SrcHeight                         = ctx->s32SrcHeight;
    TEST_TASK_CTX_S taskCtx;

    memset(&taskCtx, 0, sizeof(TEST_TASK_CTX_S));
    ctx->u32SrcSize = unit_test_vgs_get_size(ctx);
    s32Ret = unit_test_vgs_generate_source(srcFilePath, &srcBlk, ctx->u32SrcSize);
    if (s32Ret != RK_SUCCESS) {
        goto __FAILED;
    }
    s32Ret = RK_MPI_VGS_BeginJob(&hHandle);
    if (s32Ret != RK_SUCCESS) {
        goto __FAILED;
    }
    if (s32TaskMode == VGS_TASK_MODE_ONE) {
        for (RK_S32 taskId = 0; taskId < s32TaskNum; taskId++) {
            s32Ret = unit_test_vgs_add_one_task(hHandle, &stTask[taskId], ctx, &taskCtx, srcBlk, taskId);
            if (s32Ret != RK_SUCCESS) {
                goto __FAILED;
            }
        }
        for (RK_S32 taskId = 0; taskId < s32TaskNum; taskId++) {
            s32Ret = unit_test_vgs_output_one_task(&stTask[taskId], ctx, s32JobId, taskId);
            if (s32Ret != RK_SUCCESS) {
                goto __FAILED;
            }
        }
    } else if (s32TaskMode == VGS_TASK_MODE_ALL) {
        s32Ret = unit_test_vgs_add_all_task(
            hHandle, stTask, ctx, &taskCtx, u32OneTaskTestTime, srcBlk, &u32AllTaskTestTime);
        if (s32Ret != RK_SUCCESS) {
            goto __FAILED;
        }
        s32Ret = unit_test_vgs_out_all_task(hHandle, stTask, ctx, s32JobId, u32AllTaskTestTime);
        if (s32Ret != RK_SUCCESS) {
            goto __FAILED;
        }
    }
    unit_test_release_task_ctx(&taskCtx);
    RK_MPI_SYS_Free(srcBlk);
    RK_LOGD("single vgs test jobid %d running success.", s32JobId);
    return s32Ret;
__FAILED:
    unit_test_release_task_ctx(&taskCtx);
    RK_MPI_SYS_Free(srcBlk);
    RK_LOGE("single vgs test jobid %d running failed.", s32JobId);
    return s32Ret;
}

static void *unit_test_vgs_counts(void *arg) {
    TEST_VGS_CTX_S *ctx               = reinterpret_cast<TEST_VGS_CTX_S *>(arg);
    RK_S32  s32LoopCout               = ctx->s32LoopCount;
    RK_S32  s32LoopNum                = 0;
    RK_S32 s32Ret                     = RK_SUCCESS;
    for (RK_S32 i = 0; i < s32LoopCout; i++) {
        s32Ret = unit_test_vgs_one_job(ctx);
        if (s32Ret != RK_SUCCESS) {
           s32LoopNum = i;
           goto __FAILED;
        }
    }
     return RK_NULL;
__FAILED:
    RK_LOGE("single vgs test %d running failed.", s32LoopNum);
    return RK_NULL;
}

RK_S32 unit_test_vgs_all_job(TEST_VGS_CTX_S *ctx) {
    RK_S32 s32Ret                  = RK_SUCCESS;
    RK_S32  s32JobId               = ctx->s32JobIdx;
    RK_S32 u32JobTestTime          = ctx->s32JobNum;
    const char *srcFilePath        = ctx->srcFilePath;
    RK_S32 u32OneTaskTestTime      = 1;
    RK_S32 u32AllTaskTestTime      = 0;
    void  *pSrcData                = RK_NULL;
    MB_BLK srcBlk                  = RK_NULL;
    VGS_HANDLE hHandleAll[VGS_MAX_JOB_NUM];
    VGS_TASK_ATTR_S stTask[VGS_MAX_TASK_NUM];
    TEST_TASK_CTX_S taskCtx;

    memset(&taskCtx, 0, sizeof(TEST_TASK_CTX_S));
    ctx->u32SrcSize = unit_test_vgs_get_size(ctx);
    s32Ret = unit_test_vgs_generate_source(srcFilePath, &srcBlk, ctx->u32SrcSize);
    if (s32Ret == RK_SUCCESS) {
        for (RK_S32 i = 0; i < u32JobTestTime; i++) {
            VGS_HANDLE hHandle = hHandleAll[i];
            s32Ret = RK_MPI_VGS_BeginJob(&hHandle);
            if (s32Ret == RK_SUCCESS) {
                s32Ret = unit_test_vgs_add_all_task(
                    hHandle, stTask, ctx, &taskCtx, u32OneTaskTestTime, srcBlk, &u32AllTaskTestTime);
                if (s32Ret == RK_SUCCESS) {
                    s32Ret = unit_test_vgs_out_all_task(hHandle, stTask, ctx, s32JobId, u32AllTaskTestTime);
                }
            }
        }
    }
    unit_test_release_task_ctx(&taskCtx);
    RK_MPI_SYS_Free(srcBlk);
    return s32Ret;
}

RK_S32 unit_test_mpi_vgs(TEST_VGS_CTX_S *ctx) {
    RK_S32 s32Ret = RK_SUCCESS;
    pthread_t tids[VGS_MAX_JOB_NUM];
    TEST_VGS_CTX_S tmpCtx[VGS_MAX_JOB_NUM];

    for (RK_S32 jobIndex = 0; jobIndex < ctx->s32JobNum; jobIndex++) {
        memcpy(&(tmpCtx[jobIndex]), ctx, sizeof(TEST_VGS_CTX_S));
        pthread_create(&tids[jobIndex], 0, unit_test_vgs_counts, reinterpret_cast<void *>(&tmpCtx[jobIndex]));
        ctx->s32JobIdx++;
    }

    for (RK_S32 jobIndex = 0; jobIndex < ctx->s32JobNum; jobIndex++) {
        pthread_join(tids[jobIndex], RK_NULL);
    }

    return s32Ret;
}

static const char *const usages[] = {
    "./rk_mpi_vgs_test [-i SRC_PATH] [-o DST_PATH] [--src_width SRC_WIDTH]"
    "[--src_height SRC_HEIGHT] [--dst_width DST_WIDTH] [--dst_height DST_HEIGHT]",
    NULL,
};

static void mpi_vgs_test_show_options(const TEST_VGS_CTX_S *ctx) {
    RK_PRINT("cmd parse result:\n");
    RK_PRINT("input  file name       : %s\n", ctx->srcFilePath);
    RK_PRINT("output file name       : %s\n", ctx->dstFilePath);
    RK_PRINT("osd file name          : %s\n", ctx->osdFilePath);
    RK_PRINT("loop count             : %d\n", ctx->s32LoopCount);
    RK_PRINT("job number             : %d\n", ctx->s32JobNum);
    RK_PRINT("task number            : %d\n", ctx->s32TaskNum);
    RK_PRINT("task type              : %d\n", ctx->s32TaskType);
    RK_PRINT("task mode              : %d\n", ctx->s32TaskMode);
    RK_PRINT("task array size        : %d\n", ctx->u32TaskArraySize);
    RK_PRINT("rotate task angle      : %d\n", ctx->s32Angle);
    RK_PRINT("input width            : %d\n", ctx->s32SrcWidth);
    RK_PRINT("input height           : %d\n", ctx->s32SrcHeight);
    RK_PRINT("input vir width        : %d\n", ctx->s32SrcVirWidth);
    RK_PRINT("input vir height       : %d\n", ctx->s32SrcVirHeight);
    RK_PRINT("input compress mode    : %d\n", ctx->s32SrcCompressMode);
    RK_PRINT("input pixel format     : %d\n", ctx->s32SrcPixFormat);
    RK_PRINT("output width           : %d\n", ctx->s32DstWidth);
    RK_PRINT("output height          : %d\n", ctx->s32DstHeight);
    RK_PRINT("output compress mode   : %d\n", ctx->s32DstCompressMode);
    RK_PRINT("output pixel format    : %d\n", ctx->s32DstPixFormat);
    RK_PRINT("osd width              : %d\n", ctx->s32OsdWidth);
    RK_PRINT("osd height             : %d\n", ctx->s32OsdHeight);
    RK_PRINT("osd compress mode      : %d\n", ctx->s32OsdCompressMode);
    RK_PRINT("osd pixel format       : %d\n", ctx->s32OsdPixFormat);
    RK_PRINT("crop x                 : %d\n", ctx->stCropRect.s32X);
    RK_PRINT("crop y                 : %d\n", ctx->stCropRect.s32Y);
    RK_PRINT("crop w                 : %d\n", ctx->stCropRect.u32Width);
    RK_PRINT("crop h                 : %d\n", ctx->stCropRect.u32Height);
}
int main(int argc, const char **argv) {
    TEST_VGS_CTX_S ctx;
    memset(&ctx, 0, sizeof(TEST_VGS_CTX_S));
    RK_S32 s32Ret = RK_SUCCESS;
    //  set default params.
    ctx.dstFilePath     = RK_NULL;
    ctx.s32LoopCount    = 1;
    ctx.s32JobNum       = 1;
    ctx.s32TaskNum      = 1;
    ctx.s32TaskType      = VGS_TASK_TYPE_COVER;
    ctx.u32TaskArraySize  = 1;
    ctx.s32SrcCompressMode  = 0;
    ctx.s32SrcPixFormat     = RK_FMT_YUV420SP;
    ctx.s32DstCompressMode  = 0;
    ctx.s32DstPixFormat     = RK_FMT_YUV420SP;

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("basic options:"),
        OPT_STRING('i', "input",  &(ctx.srcFilePath),
                   "input file name. e.g.(/userdata/1080p.nv12). <required>", NULL, 0, 0),
        OPT_STRING('o', "output", &(ctx.dstFilePath),
                    "output file path. e.g.(/userdata/vgs/). default(NULL).", NULL, 0, 0),
        OPT_INTEGER('n', "loop_count", &(ctx.s32LoopCount),
                    "loop running count. default(1)", NULL, 0, 0),
        OPT_INTEGER('j', "job_number", &(ctx.s32JobNum),
                    "the job number of vgs. default(1).", NULL, 0, 0),
        OPT_INTEGER('t', "task_number", &(ctx.s32TaskNum),
                    "the task number of one job. default(1).", NULL, 0, 0),
        OPT_STRING('\0', "osd_file", &(ctx.osdFilePath),
                    "osd file path. e.g.(/userdata/vgs/). default(NULL).", NULL, 0, 0),
        OPT_INTEGER('\0', "task_type", &(ctx.s32TaskType),
                    "vgs task type. range(1,6). 1.scale. 2.rotate. 3.draw_line. 4.cover. 5.osd. 6.mosaic.", NULL, 0, 0),
        OPT_INTEGER('\0', "task_mode", &(ctx.s32TaskMode),
                    "task mode. 1: only one task type in one job. 2: all task type in one job. default(1)", NULL, 0, 0),
        OPT_INTEGER('\0', "task_array_size", &(ctx.u32TaskArraySize),
                    "array task size range(1,100). default(1)", NULL, 0, 0),
        OPT_INTEGER('\0', "angle", &(ctx.s32Angle),
                    "rotate task angle. 0:0. 1:90. 2:180. 3:270. default(1)", NULL, 0, 0),
        OPT_INTEGER('\0', "src_width", &(ctx.s32SrcWidth),
                    "src width. e.g.(1920). <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "src_height", &(ctx.s32SrcHeight),
                    "src height. e.g.(1080). <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "src_vir_width", &(ctx.s32SrcVirWidth),
                    "src vir width. e.g.(1920).", NULL, 0, 0),
        OPT_INTEGER('\0', "src_vir_height", &(ctx.s32SrcVirHeight),
                    "src vir height. e.g.(1080).", NULL, 0, 0),
        OPT_INTEGER('\0', "src_compress", &(ctx.s32SrcCompressMode),
                    "src compress mode. default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "src_format", &(ctx.s32SrcPixFormat),
                    "src pixel format. default(0. 0 is NV12).", NULL, 0, 0),
        OPT_INTEGER('\0', "dst_width", &(ctx.s32DstWidth),
                    "dst width. e.g.(1920). <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "dst_height", &(ctx.s32DstHeight),
                    "dst height. e.g.(1080). <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "dst_compress", &(ctx.s32DstCompressMode),
                    "dst compress mode. default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "dst_format", &(ctx.s32DstPixFormat),
                    "dst pixel format. default(0. 0 is NV12).", NULL, 0, 0),
        OPT_INTEGER('\0', "osd_width", &(ctx.s32OsdWidth),
                    "osd width. e.g.(1920). <required on OSD>", NULL, 0, 0),
        OPT_INTEGER('\0', "osd_height", &(ctx.s32OsdHeight),
                    "osd height. e.g.(1080). <required on OSD>", NULL, 0, 0),
        OPT_INTEGER('\0', "osd_compress", &(ctx.s32OsdCompressMode),
                    "osd compress mode. default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "osd_format", &(ctx.s32OsdPixFormat),
                    "osd pixel format. default(0. 0 is NV12).", NULL, 0, 0),
        OPT_INTEGER('\0', "crop_x", &(ctx.stCropRect.s32X),
                    "crop ratio rect x. default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "crop_y", &(ctx.stCropRect.s32Y),
                    "crop ratio rect y. default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "crop_w", &(ctx.stCropRect.u32Width),
                    "crop ratio rect width. default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "crop_h", &(ctx.stCropRect.u32Height),
                    "crop ratio rect height. default(0).", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nselect a test case to run.",
                                 "\nuse --help for details.");

    argc = argparse_parse(&argparse, argc, argv);
    mpi_vgs_test_show_options(&ctx);

    if (ctx.srcFilePath == RK_NULL
        || ctx.s32SrcWidth <= 0
        || ctx.s32SrcHeight <= 0
        || ctx.s32DstWidth <= 0
        || ctx.s32DstHeight <= 0) {
        argparse_usage(&argparse);
        goto __FAILED;
    }
    s32Ret = RK_MPI_SYS_Init();
    if (s32Ret != RK_SUCCESS) {
        goto __FAILED;
    }
    unit_test_mpi_vgs(&ctx);
    s32Ret = RK_MPI_SYS_Exit();
    if (s32Ret != RK_SUCCESS) {
        goto __FAILED;
    }
    RK_LOGD("test done !\n");
__FAILED:
    return 0;
}
