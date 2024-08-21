/*
 * Copyright (c) 2021 Rockchip, Inc. All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "rkadk_photo.h"
#include "rkadk_log.h"
#include "rkadk_media_comm.h"
#include "rkadk_param.h"
#include "rkadk_thumb_comm.h"
#include "rkadk_signal.h"
#include <byteswap.h>
#include <assert.h>
#include <malloc.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <utime.h>

#ifdef JPEG_SLICE
#include "libRkScalerApi.h"
#endif

#define JPG_THM_FIND_NUM_MAX 50
#define JPG_EXIF_FLAG_LEN 6
#define JPG_DIRECTORY_ENTRY_LEN 12
#define JPG_DE_TYPE_COUNT 12
#define JPG_MP_FLAG_LEN 4
#define JPG_MP_ENTRY_LEN 16
#define JPG_THUMB_TAG_LEN 4

#define PHOTO_THM_VDEC_CHN (VDEC_MAX_CHN_NUM - 2)
#define PHOTO_THM_VPSS_GRP (VPSS_MAX_GRP_NUM - 2)
#define PHOTO_THM_VPSS_CHN 0

#define GET_DATA_VDEC_CHN (VDEC_MAX_CHN_NUM - 3)
#define GET_DATA_VPSS_GRP (VPSS_MAX_GRP_NUM - 3)
#define GET_DATA_VPSS_CHN 0

#define JPG_MMAP_FILE_PATH "/tmp/.mmap.jpeg"

typedef enum {
  RKADK_JPG_LITTLE_ENDIAN, // II
  RKADK_JPG_BIG_ENDIAN,    // MM
  RKADK_JPG_BYTE_ORDER_BUTT
} RKADK_JPG_BYTE_ORDER_E;

typedef struct {
  RKADK_U16 u16Type;
  RKADK_U16 u16TypeByte;
} RKADK_JPG_DE_TYPE_S;

typedef struct {
  RKADK_S32 s32Witdh;
  RKADK_S32 s32Height; //uneven slice
  RKADK_S32 s32EvenHeight; //even slice
  RKADK_S32 s32LastWidth;
  RKADK_S32 s32LastHeight;
} RKADK_JPG_SLICE_RES;

typedef struct {
  bool bJpegSlice;
  RKADK_U32 u32SliceCount;
  pthread_t sliceTid;
  RKADK_JPG_SLICE_RES stViSlice;
  RKADK_JPG_SLICE_RES stVencSlice;

  //mcu count contained in each slice, the unit of mcu is 16 x 16
  RK_U32 u32MCUPerECS;
} RKADK_JPG_SLICE_PARAM;

typedef struct {
  RKADK_U32 u32CamId;
  RKADK_U32 u32ViChn;
  bool bReseting;
  bool bUseVpss;
  RKADK_PHOTO_DATA_RECV_FN_PTR pDataRecvFn;
  pthread_t tid;
  bool bGetJpeg;
  RKADK_U32 u32PhotoCnt;
  RKADK_JPG_SLICE_PARAM stSliceParam;
  void *userdata;
} RKADK_PHOTO_HANDLE_S;

static RKADK_U8 *RKADK_PHOTO_Mmap(RKADK_CHAR *FileName, RKADK_U32 u32PhotoLen) {
  char data = 0xff;
  FILE *fd = NULL;
  RKADK_U8 *pu8Photo = NULL;

  fd = fopen(FileName, "w+");
  if (!fd) {
    RKADK_LOGE("open %s failed", FileName);
    return NULL;
  }

  if (fseek(fd, u32PhotoLen, SEEK_SET)) {
    fclose(fd);
    RKADK_LOGE("seek %s failed", FileName);
    return NULL;
  }
  fwrite(&data, 1, 1, fd);

  pu8Photo = (RKADK_U8 *)mmap(NULL, u32PhotoLen, PROT_READ | PROT_WRITE, MAP_SHARED, fileno(fd), 0);
  if (pu8Photo == MAP_FAILED) {
    fclose(fd);
    RKADK_LOGE("Mmap %s, errno: %d",FileName, errno);
    return NULL;
  }
  fclose(fd);

  return pu8Photo;
}

static int RKADK_PHOTO_SetViSliceParam(RKADK_PHOTO_HANDLE_S *pHandle,
                                RKADK_PARAM_PHOTO_CFG_S *pstPhotoCfg, RKADK_U32 u32Width,
                                RKADK_U32 u32Height) {
  int count;
  RKADK_U32 multiplier = 0;
  RKADK_S32 s32Height = 0, s32SaveHeight = 0;

  pHandle->stSliceParam.stViSlice.s32Witdh = u32Width;
  pHandle->stSliceParam.stViSlice.s32LastWidth = u32Width;

  multiplier = u32Height * pHandle->stSliceParam.stVencSlice.s32Height;
  s32Height = multiplier / pstPhotoCfg->image_height;
  if (multiplier % pstPhotoCfg->image_height) {
    s32SaveHeight = s32Height;
    s32Height += 1;
  }

RETRY:
  if (s32Height % 2) {
    pHandle->stSliceParam.stViSlice.s32Height = s32Height - 1;
    pHandle->stSliceParam.stViSlice.s32EvenHeight = s32Height + 1;
  } else {
    pHandle->stSliceParam.stViSlice.s32Height = s32Height;
    pHandle->stSliceParam.stViSlice.s32EvenHeight = s32Height;
  }

  count = (pHandle->stSliceParam.u32SliceCount - 1) / 2;
  pHandle->stSliceParam.stViSlice.s32LastHeight = u32Height - (pHandle->stSliceParam.stViSlice.s32Height
    + pHandle->stSliceParam.stViSlice.s32EvenHeight) * count;
  if ((pHandle->stSliceParam.u32SliceCount - 1) % 2)
    pHandle->stSliceParam.stViSlice.s32LastHeight -= pHandle->stSliceParam.stViSlice.s32Height;

  if (pHandle->stSliceParam.stViSlice.s32LastHeight <= 0) {
    RKADK_LOGD("Invalid s32LastHeight[%d], recalculation", pHandle->stSliceParam.stViSlice.s32LastHeight);
    s32Height = s32SaveHeight;
    goto RETRY;
  }

  RKADK_LOGD("Vi slice w*h[%d, %d, %d], last slice w*h[%d, %d]",
              pHandle->stSliceParam.stViSlice.s32Witdh,
              pHandle->stSliceParam.stViSlice.s32Height,
              pHandle->stSliceParam.stViSlice.s32EvenHeight,
              pHandle->stSliceParam.stViSlice.s32LastWidth,
              pHandle->stSliceParam.stViSlice.s32LastHeight);

  return 0;
}

static int RKADK_PHOTO_SetSliceParam(RKADK_PHOTO_HANDLE_S *pHandle,
                              RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg,
                              RKADK_PARAM_PHOTO_CFG_S *pstPhotoCfg) {
  if (pstPhotoCfg->slice_height <= 0 || pstPhotoCfg->slice_height > pstPhotoCfg->image_height) {
    RKADK_LOGE("Invalid slice_height[%d], image_height[%d]", pstPhotoCfg->slice_height, pstPhotoCfg->image_height);
    return -1;
  }

  if (pstPhotoCfg->slice_height % 16) {
    RKADK_LOGE("Invalid slice_height[%d], must be 16 aligned", pstPhotoCfg->slice_height);
    return -1;
  }

  if (pstPhotoCfg->vi_attr.stChnAttr.enPixelFormat != RK_FMT_YUV420SP
      && pstPhotoCfg->vi_attr.stChnAttr.enPixelFormat != RK_FMT_YUV422SP) {
    RKADK_LOGE("Jpeg slice unsupport format: %d", pstPhotoCfg->vi_attr.stChnAttr.enPixelFormat);
    return -1;
  }

  pHandle->stSliceParam.stVencSlice.s32Witdh = pstPhotoCfg->image_width;
  pHandle->stSliceParam.stVencSlice.s32Height = pstPhotoCfg->slice_height;

  pHandle->stSliceParam.u32SliceCount = pstPhotoCfg->image_height / pstPhotoCfg->slice_height;
  if (pstPhotoCfg->image_height % pstPhotoCfg->slice_height)
    pHandle->stSliceParam.u32SliceCount += 1;

  pHandle->stSliceParam.u32MCUPerECS = UPALIGNTO(pstPhotoCfg->image_width, 16) * pstPhotoCfg->slice_height / 256;

  pHandle->stSliceParam.stVencSlice.s32LastWidth = pHandle->stSliceParam.stVencSlice.s32Witdh;
  pHandle->stSliceParam.stVencSlice.s32LastHeight
    = pstPhotoCfg->image_height - pstPhotoCfg->slice_height * (pHandle->stSliceParam.u32SliceCount - 1);

  if (pHandle->stSliceParam.stVencSlice.s32Witdh > JPEG_SLICE_WIDTH_MAX) {
    RKADK_LOGE("Slice output width[%d] > %d", pHandle->stSliceParam.stVencSlice.s32Witdh, JPEG_SLICE_WIDTH_MAX);
    return -1;
  }

  if (pHandle->stSliceParam.stVencSlice.s32Height > JPEG_SLICE_HEIGHT_MAX) {
    RKADK_LOGE("Slice output height[%d] > %d", pHandle->stSliceParam.stVencSlice.s32Witdh, JPEG_SLICE_HEIGHT_MAX);
    return -1;
  }

  RKADK_LOGD("Jpeg slice w*h[%d, %d], last slice w*h[%d, %d], u32MCUPerECS: %d, slice count: %d",
              pHandle->stSliceParam.stVencSlice.s32Witdh,
              pHandle->stSliceParam.stVencSlice.s32Height,
              pHandle->stSliceParam.stVencSlice.s32LastWidth,
              pHandle->stSliceParam.stVencSlice.s32LastHeight,
              pHandle->stSliceParam.u32MCUPerECS,
              pHandle->stSliceParam.u32SliceCount);

  return 0;
}

//#define JPEG_SLICE_WRITE
//#define FULL_IMAGE_TEST
static void *RKADK_PHOTO_SliceProc(void *params) {
#ifdef JPEG_SLICE
  int ret, i = 0;
  int srcOffsetY = 0, srcOffsetUV = 0;
  void *pSrcData = NULL, *pDstBuf = NULL;
  RKADK_U32 u32DstBufSize;
  MB_POOL vencMbPool = MB_INVALID_POOLID;
  VIDEO_FRAME_INFO_S stViFrame, stFrame;
  RkScalerContext scalerContext;
  RkScalerParams scalerParam;
  enum ScalerFormat_e format;
  RKADK_PARAM_PHOTO_CFG_S *pstPhotoCfg = NULL;
  MB_BLK pMbBlk = NULL;
  MB_POOL_CONFIG_S stMbPoolCfg;

  VENC_STREAM_S stStream;
  VENC_PACK_S stPack;
  RKADK_PHOTO_RECV_DATA_S stData;
  RKADK_U8 *pu8JpgData;

  bool bGetThumb = false;
  VENC_STREAM_S stThumbFrame;
  VENC_PACK_S stThumbPack;
  RKADK_U32 u32PhotoLen;
  RKADK_U8 *pu8Photo = NULL;

#ifdef JPEG_SLICE_WRITE
  FILE *file = NULL;
  char slicePath[128];
  static int frameCnt = 0;
#endif

#ifdef FULL_IMAGE_TEST
  char *imageBuf = NULL;
  int imageSize;
  int imageOffsetY = 0, imageOffsetUV = 0;
#endif

  RKADK_PHOTO_HANDLE_S *pHandle = (RKADK_PHOTO_HANDLE_S *)params;
  if (!pHandle) {
    RKADK_LOGE("Get jpeg thread invalid param");
    return NULL;
  }

  if (!pHandle->pDataRecvFn) {
    RKADK_LOGE("u32CamId[%d] don't register callback", pHandle->u32CamId);
    return NULL;
  }

  pstPhotoCfg = RKADK_PARAM_GetPhotoCfg(pHandle->u32CamId);
  if (!pstPhotoCfg) {
    RKADK_LOGE("RKADK_PARAM_GetPhotoCfg failed");
    return NULL;
  }

  RKADK_PARAM_THUMB_CFG_S *ptsThumbCfg = RKADK_PARAM_GetThumbCfg(pHandle->u32CamId);
  if (!ptsThumbCfg) {
    RKADK_LOGE("RKADK_PARAM_GetThumbCfg failed");
    return NULL;
  }

  u32PhotoLen = pHandle->stSliceParam.stVencSlice.s32Witdh * pHandle->stSliceParam.stVencSlice.s32Height
                + ptsThumbCfg->thumb_width * ptsThumbCfg->thumb_height;

  pu8Photo = RKADK_PHOTO_Mmap((RKADK_CHAR *)JPG_MMAP_FILE_PATH, u32PhotoLen);
  if (!pu8Photo)
    return NULL;

  if (pstPhotoCfg->vi_attr.stChnAttr.enPixelFormat == RK_FMT_YUV420SP) {
    format = SCALER_FMT_YUV420SP;
    u32DstBufSize = pstPhotoCfg->max_slice_width * pstPhotoCfg->max_slice_height * 1.5;
  } else {
    format = SCALER_FMT_YUV422SP;
    u32DstBufSize = pstPhotoCfg->max_slice_width * pstPhotoCfg->max_slice_height * 2;
  }

#ifdef FULL_IMAGE_TEST
  imageSize = pstPhotoCfg->image_width * pstPhotoCfg->image_height * 1.5;
  imageBuf = (char *)malloc(imageSize);
  imageOffsetUV = pstPhotoCfg->image_width * pstPhotoCfg->image_height;
#endif

  memset(&stMbPoolCfg, 0, sizeof(MB_POOL_CONFIG_S));
  stMbPoolCfg.u64MBSize = u32DstBufSize;
  stMbPoolCfg.u32MBCnt  = 1;
  stMbPoolCfg.enAllocType = MB_ALLOC_TYPE_DMA;
  stMbPoolCfg.bPreAlloc = RK_TRUE;
  vencMbPool = RK_MPI_MB_CreatePool(&stMbPoolCfg);
  if (vencMbPool == MB_INVALID_POOLID) {
    RKADK_LOGE("Create venc MB pool failed!");
    return NULL;
  }

  ret = RkScalerInit(&scalerContext, 1);
  if (ret) {
    RKADK_LOGE("Init scaler context failed[%d]", ret);
    return NULL;
  }

  memset(&scalerParam, 0, sizeof(RkScalerParams));
  scalerParam.nMethodLuma = SCALER_METHOD_BILINEAR;
  scalerParam.nMethodChrm = SCALER_METHOD_NEAREST;
  scalerParam.nCores = 1;
  scalerParam.nSrcFmt = format;
  scalerParam.pSrcBufs[2] = NULL;
  scalerParam.nDstFmt = format;
  scalerParam.pDstBufs[2] = NULL;

  memset(&stViFrame, 0, sizeof(VIDEO_FRAME_INFO_S));
  memset(&stFrame, 0, sizeof(VIDEO_FRAME_INFO_S));
  memset(&stStream, 0, sizeof(VENC_STREAM_S));
  memset(&stPack, 0, sizeof(VENC_PACK_S));
  memset(&stThumbFrame, 0, sizeof(VENC_STREAM_S));
  memset(&stThumbPack, 0, sizeof(VENC_PACK_S));

  stStream.pstPack = &stPack;
  stThumbFrame.pstPack = &stThumbPack;

  // drop first frame
  ret = RK_MPI_VENC_GetStream(pstPhotoCfg->venc_chn, &stStream, 1000);
  if (ret == RK_SUCCESS) {
    RK_MPI_VENC_ReleaseStream(pstPhotoCfg->venc_chn, &stStream);
  } else {
    RKADK_LOGE("RK_MPI_VENC_GetStream[%d] timeout[%x]", pstPhotoCfg->venc_chn, ret);
    RK_MPI_VENC_StopRecvFrame(pstPhotoCfg->venc_chn);
  }
  RK_MPI_VENC_ResetChn(pstPhotoCfg->venc_chn);

  // drop first thumb frame
  ret = RK_MPI_VENC_GetStream(ptsThumbCfg->photo_venc_chn, &stThumbFrame, 1000);
  if (ret == RK_SUCCESS) {
    RK_MPI_VENC_ReleaseStream(ptsThumbCfg->photo_venc_chn, &stThumbFrame);
  } else {
    RKADK_LOGE("RK_MPI_VENC_GetStream[%d] timeout[%x]", ptsThumbCfg->photo_venc_chn, ret);
    RK_MPI_VENC_StopRecvFrame(ptsThumbCfg->photo_venc_chn);
  }
  RK_MPI_VENC_ResetChn(ptsThumbCfg->photo_venc_chn);

  while (pHandle->bGetJpeg) {
    ret = RK_MPI_VI_GetChnFrame(pHandle->u32CamId, pstPhotoCfg->vi_attr.u32ViChn, &stViFrame, 1000);
    if (ret == RK_SUCCESS) {
      if (pHandle->u32PhotoCnt > 0) {
        if (pHandle->stSliceParam.stViSlice.s32Witdh != stViFrame.stVFrame.u32VirWidth)
          RKADK_PHOTO_SetViSliceParam(pHandle, pstPhotoCfg, stViFrame.stVFrame.u32VirWidth,
                                      stViFrame.stVFrame.u32VirHeight);

        pSrcData = RK_MPI_MB_Handle2VirAddr(stViFrame.stVFrame.pMbBlk);
        srcOffsetY = 0;
        srcOffsetUV = stViFrame.stVFrame.u32VirWidth * stViFrame.stVFrame.u32VirHeight;

#ifdef JPEG_SLICE_WRITE
        if (!file) {
          memset(slicePath, 0, 128);
          sprintf(slicePath, "/data/slice_%d.yuv", frameCnt);
          file = fopen(slicePath, "w");
          if (!file)
            RKADK_LOGD("Craete %s failed", slicePath);
          else
            RKADK_LOGD("Create %s success", slicePath);
        }

#ifdef FULL_IMAGE_TEST
        imageOffsetY = 0;
        imageOffsetUV = pstPhotoCfg->image_width * pstPhotoCfg->image_height;
#endif
#endif

        for (i = 1; i <= pHandle->stSliceParam.u32SliceCount; i++) {
          scalerParam.nCallCnt = i;
          if (scalerParam.nCallCnt == pHandle->stSliceParam.u32SliceCount) {
            scalerParam.nSrcWid = pHandle->stSliceParam.stViSlice.s32LastWidth;
            scalerParam.nSrcHgt = pHandle->stSliceParam.stViSlice.s32LastHeight;
            scalerParam.nDstWid = pHandle->stSliceParam.stVencSlice.s32LastWidth;
            scalerParam.nDstHgt = pHandle->stSliceParam.stVencSlice.s32LastHeight;
          } else {
            scalerParam.nSrcWid = pHandle->stSliceParam.stViSlice.s32Witdh;

            if (i % 2)
              scalerParam.nSrcHgt = pHandle->stSliceParam.stViSlice.s32Height;
            else
              scalerParam.nSrcHgt = pHandle->stSliceParam.stViSlice.s32EvenHeight;

            scalerParam.nDstWid = pHandle->stSliceParam.stVencSlice.s32Witdh;
            scalerParam.nDstHgt = pHandle->stSliceParam.stVencSlice.s32Height;
          }
          //Y
          scalerParam.nSrcWStrides[0] = scalerParam.nSrcWid;
          scalerParam.nSrcHStrides[0] = scalerParam.nSrcHgt;
          scalerParam.nDstWStrides[0] = scalerParam.nDstWid;
          scalerParam.nDstHStrides[0] = scalerParam.nDstHgt;

          //UV
          scalerParam.nSrcWStrides[1] = scalerParam.nSrcWid;
          scalerParam.nDstWStrides[1] = scalerParam.nDstWid;
          if (format == SCALER_FMT_YUV420SP) {
            scalerParam.nSrcHStrides[1] = scalerParam.nSrcHgt / 2;
            scalerParam.nDstHStrides[1] = scalerParam.nDstHgt / 2;
          } else {
            scalerParam.nSrcHStrides[1] = scalerParam.nSrcHgt;
            scalerParam.nDstHStrides[1] = scalerParam.nDstHgt;
          }

          //Y+C mode
          scalerParam.pSrcBufs[0] = (RK_U8*)pSrcData + srcOffsetY;
          scalerParam.pSrcBufs[1] = (RK_U8*)pSrcData + srcOffsetUV;

          pMbBlk = RK_MPI_MB_GetMB(vencMbPool, u32DstBufSize, RK_TRUE);
          if (RK_NULL == pMbBlk) {
            RKADK_LOGE("RK_MPI_MB_GetMB failed");
            goto Exit;
          }

          pDstBuf = RK_MPI_MB_Handle2VirAddr(pMbBlk);
          scalerParam.pDstBufs[0] = (RK_U8*)pDstBuf;
          scalerParam.pDstBufs[1] = (RK_U8*)pDstBuf + scalerParam.nDstWStrides[0] * scalerParam.nDstHStrides[0];

          /* call scaler processor */
          ret = RkScalerProcessor(scalerContext, &scalerParam);
          if (ret != 0) {
            RKADK_LOGE("RkScalerProcessor failed[%d]", ret);
            RK_MPI_MB_ReleaseMB(pMbBlk);
            goto Exit;
          }

          srcOffsetY += scalerParam.nSrcWStrides[0] * scalerParam.nSrcHStrides[0];
          srcOffsetUV += scalerParam.nSrcWStrides[1] * scalerParam.nSrcHStrides[1];
          RK_MPI_SYS_MmzFlushCache(pMbBlk, RK_FALSE);

#ifdef JPEG_SLICE_WRITE
#ifndef FULL_IMAGE_TEST
          if (file) {
            fwrite(pDstBuf, 1, u32DstBufSize, file);
            fsync(file);
          }
#else
          memcpy(imageBuf + imageOffsetY, (RK_U8*)pDstBuf, scalerParam.nDstWStrides[0] * scalerParam.nDstHStrides[0]);
          memcpy(imageBuf + imageOffsetUV, (RK_U8*)pDstBuf + scalerParam.nDstWStrides[0] * scalerParam.nDstHStrides[0],
                  scalerParam.nDstWStrides[1] * scalerParam.nDstHStrides[1]);

          imageOffsetY += scalerParam.nDstWStrides[0] * scalerParam.nDstHStrides[0];
          imageOffsetUV += scalerParam.nDstWStrides[1] * scalerParam.nDstHStrides[1];
#endif
#endif

          //Send venc frame
          stFrame.stVFrame.pMbBlk = pMbBlk;
          stFrame.stVFrame.u32Width = scalerParam.nDstWid;
          stFrame.stVFrame.u32Height = scalerParam.nDstHgt;
          stFrame.stVFrame.u32VirWidth = scalerParam.nDstWid;
          stFrame.stVFrame.u32VirHeight = scalerParam.nDstHgt;
          stFrame.stVFrame.enPixelFormat = pstPhotoCfg->vi_attr.stChnAttr.enPixelFormat;
          stFrame.stVFrame.u64PTS = stViFrame.stVFrame.u64PTS + i - 1;
          if (scalerParam.nCallCnt == pHandle->stSliceParam.u32SliceCount)
            stFrame.stVFrame.u32FrameFlag = FRAME_FLAG_SNAP_END;
          else
            stFrame.stVFrame.u32FrameFlag = 0;

          ret = RK_MPI_VENC_SendFrame(pstPhotoCfg->venc_chn, &stFrame, -1);
          if (ret != RK_SUCCESS) {
            RKADK_LOGE("RK_MPI_VENC_SendFrame failed[%x]", ret);
            RK_MPI_MB_ReleaseMB(pMbBlk);
            goto Exit;
          }

          ret = RK_MPI_MB_ReleaseMB(pMbBlk);
          if (ret != RK_SUCCESS)
            RKADK_LOGE("RK_MPI_MB_ReleaseMB failed[%x]", ret);

          //Get jpeg
          ret = RK_MPI_VENC_GetStream(pstPhotoCfg->venc_chn, &stStream, -1);
          if (ret == RK_SUCCESS) {
            pu8JpgData = (RKADK_U8 *)RK_MPI_MB_Handle2VirAddr(stStream.pstPack->pMbBlk);
            memset(&stData, 0, sizeof(RKADK_PHOTO_RECV_DATA_S));
            stData.userdata = pHandle->userdata;

            if (!bGetThumb) {
              ret = RK_MPI_VENC_GetStream(ptsThumbCfg->photo_venc_chn, &stThumbFrame, 1000);
              if (ret == RK_SUCCESS) {
                stData.u32DataLen = ThumbnailPhotoData(pu8JpgData, stStream.pstPack->u32Len, stThumbFrame, pu8Photo);
                stData.pu8DataBuf = pu8Photo;
                stData.u32CamId = pHandle->u32CamId;
                stData.bStreamEnd = stStream.pstPack->bStreamEnd;
                pHandle->pDataRecvFn(&stData);

                ret = RK_MPI_VENC_ReleaseStream(ptsThumbCfg->photo_venc_chn, &stThumbFrame);
                if (ret != RK_SUCCESS)
                  RKADK_LOGE("RK_MPI_VENC_ReleaseStream failed[%x]", ret);

                RK_MPI_VENC_ResetChn(ptsThumbCfg->photo_venc_chn);
              } else {
                RKADK_LOGW("Get thumb venc frame failed[%x]", ret);
                RK_MPI_VENC_StopRecvFrame(ptsThumbCfg->photo_venc_chn);
                stData.pu8DataBuf = pu8JpgData;
                stData.u32DataLen = stStream.pstPack->u32Len;
                stData.u32CamId = pHandle->u32CamId;
                stData.bStreamEnd = stStream.pstPack->bStreamEnd;
                pHandle->pDataRecvFn(&stData);
              }
              bGetThumb = true;
            }else {
              stData.pu8DataBuf = pu8JpgData;
              stData.u32DataLen = stStream.pstPack->u32Len;
              stData.u32CamId = pHandle->u32CamId;
              stData.bStreamEnd = stStream.pstPack->bStreamEnd;
              pHandle->pDataRecvFn(&stData);
            }

            if (stData.bStreamEnd) {
              bGetThumb = false;
              RKADK_LOGD("Photo success, seq = %d, len = %d", stStream.u32Seq, stStream.pstPack->u32Len);
              RKADK_BUFINFO("take photo[%d]", pstPhotoCfg->venc_chn);
            }

            ret = RK_MPI_VENC_ReleaseStream(pstPhotoCfg->venc_chn, &stStream);
            if (ret != RK_SUCCESS)
              RKADK_LOGE("RK_MPI_VENC_ReleaseStream failed[%x]", ret);
          } else {
            RKADK_LOGE("RK_MPI_VENC_GetStream failed[%x]", ret);
            goto Exit;
          }
        }

#ifdef JPEG_SLICE_WRITE
        if (file) {
#ifdef FULL_IMAGE_TEST
          fwrite(imageBuf, 1, imageSize, file);
#endif
          fclose(file);
          file = NULL;
          RKADK_LOGD("Close slice path: %s", slicePath);

          frameCnt++;
        }
#endif

        pHandle->u32PhotoCnt -= 1;
      }

      ret = RK_MPI_VI_ReleaseChnFrame(pHandle->u32CamId, pstPhotoCfg->vi_attr.u32ViChn, &stViFrame);
      if (ret)
        RKADK_LOGE("RK_MPI_VI_ReleaseChnFrame failed[%x]", ret);
    } else {
      RKADK_LOGE("RK_MPI_VI_GetChnFrame failed[%x]", ret);
    }
  }

Exit:
  //release the context
  ret = RkScalerDeinit(scalerContext);
  if (ret)
    RKADK_LOGE("Deinit scaler context failed[%d]", ret);

  if (vencMbPool != MB_INVALID_POOLID)
    RK_MPI_MB_DestroyPool(vencMbPool);

#ifdef FULL_IMAGE_TEST
  if (imageBuf)
    free(imageBuf);
#endif

  if (pu8Photo)
    munmap(pu8Photo, u32PhotoLen);

  RKADK_LOGD("Exit jpeg slice thread");
  return NULL;
#else
  RKADK_LOGE("No define JPEG_SLICE macro");
  return NULL;
#endif
}

static void *RKADK_PHOTO_GetJpeg(void *params) {
  int ret;
  VENC_STREAM_S stFrame, stThumbFrame;
  VENC_PACK_S stPack, stThumbPack;
  RKADK_PHOTO_RECV_DATA_S stData;
  RKADK_U8 *pu8JpgData;
  RKADK_U32 u32PhotoLen;
  RKADK_U8 *pu8Photo = NULL;

  RKADK_PHOTO_HANDLE_S *pHandle = (RKADK_PHOTO_HANDLE_S *)params;
  if (!pHandle) {
    RKADK_LOGE("Get jpeg thread invalid param");
    return NULL;
  }

  if (!pHandle->pDataRecvFn) {
    RKADK_LOGE("u32CamId[%d] don't register callback", pHandle->u32CamId);
    return NULL;
  }

  RKADK_PARAM_THUMB_CFG_S *ptsThumbCfg = RKADK_PARAM_GetThumbCfg(pHandle->u32CamId);
  if (!ptsThumbCfg) {
    RKADK_LOGE("RKADK_PARAM_GetThumbCfg failed");
    return NULL;
  }

  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg = RKADK_PARAM_GetSensorCfg(pHandle->u32CamId);
  if (!pstSensorCfg) {
    RKADK_LOGE("RKADK_PARAM_GetSensorCfg failed");
    return NULL;
  }

  RKADK_PARAM_PHOTO_CFG_S *pstPhotoCfg = RKADK_PARAM_GetPhotoCfg(pHandle->u32CamId);
  if (!pstPhotoCfg) {
    RKADK_LOGE("RKADK_PARAM_GetPhotoCfg failed");
    return NULL;
  }

  stFrame.pstPack = &stPack;
  stThumbFrame.pstPack = &stThumbPack;
  u32PhotoLen = pstSensorCfg->max_width * pstSensorCfg->max_height
                + ptsThumbCfg->thumb_width * ptsThumbCfg->thumb_height;

  pu8Photo = RKADK_PHOTO_Mmap((RKADK_CHAR *)JPG_MMAP_FILE_PATH, u32PhotoLen);
  if (!pu8Photo)
    return NULL;

  // drop first frame
  ret = RK_MPI_VENC_GetStream(pstPhotoCfg->venc_chn, &stFrame, 1000);
  if (ret == RK_SUCCESS) {
    RK_MPI_VENC_ReleaseStream(pstPhotoCfg->venc_chn, &stFrame);
  } else {
    RKADK_LOGE("RK_MPI_VENC_GetStream[%d] timeout[%x]", pstPhotoCfg->venc_chn, ret);
    RK_MPI_VENC_StopRecvFrame(pstPhotoCfg->venc_chn);
  }

  // drop first thumb frame
  ret = RK_MPI_VENC_GetStream(ptsThumbCfg->photo_venc_chn, &stThumbFrame, 1000);
  if (ret == RK_SUCCESS) {
    RK_MPI_VENC_ReleaseStream(ptsThumbCfg->photo_venc_chn, &stThumbFrame);
  } else {
    RKADK_LOGE("RK_MPI_VENC_GetStream[%d] timeout[%x]", ptsThumbCfg->photo_venc_chn, ret);
    RK_MPI_VENC_StopRecvFrame(ptsThumbCfg->photo_venc_chn);
  }

  RK_MPI_VENC_ResetChn(pstPhotoCfg->venc_chn);
  RK_MPI_VENC_ResetChn(ptsThumbCfg->photo_venc_chn);

  while (pHandle->bGetJpeg) {
    ret = RK_MPI_VENC_GetStream(pstPhotoCfg->venc_chn, &stFrame, 1000);
    if (ret == RK_SUCCESS) {
      pu8JpgData = (RKADK_U8 *)RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
      memset(&stData, 0, sizeof(RKADK_PHOTO_RECV_DATA_S));
      stData.userdata = pHandle->userdata;

      ret = RK_MPI_VENC_GetStream(ptsThumbCfg->photo_venc_chn, &stThumbFrame, 1000);
      if (ret == RK_SUCCESS) {
        stData.u32DataLen = ThumbnailPhotoData(pu8JpgData, stFrame.pstPack->u32Len, stThumbFrame, pu8Photo);
        stData.pu8DataBuf = pu8Photo;
        stData.u32CamId = pHandle->u32CamId;
        stData.bStreamEnd = true;
        pHandle->pDataRecvFn(&stData);

        ret = RK_MPI_VENC_ReleaseStream(ptsThumbCfg->photo_venc_chn, &stThumbFrame);
        if (ret != RK_SUCCESS)
          RKADK_LOGE("RK_MPI_VENC_ReleaseStream failed[%x]", ret);

        RK_MPI_VENC_ResetChn(ptsThumbCfg->photo_venc_chn);
      } else {
        RKADK_LOGW("Get thumb frame failed[%x]", ret);
        system("cat /proc/vcodec/enc/venc_info");
        RK_MPI_VENC_StopRecvFrame(ptsThumbCfg->photo_venc_chn);
        stData.pu8DataBuf = pu8JpgData;
        stData.u32DataLen = stFrame.pstPack->u32Len;
        stData.u32CamId = pHandle->u32CamId;
        stData.bStreamEnd = true;
        pHandle->pDataRecvFn(&stData);
      }

      RKADK_LOGD("Photo success, seq = %d, len = %d", stFrame.u32Seq, stFrame.pstPack->u32Len);
      RKADK_BUFINFO("take photo[%d]", pstPhotoCfg->venc_chn);
      ret = RK_MPI_VENC_ReleaseStream(pstPhotoCfg->venc_chn, &stFrame);
      if (ret != RK_SUCCESS)
        RKADK_LOGE("RK_MPI_VENC_ReleaseStream failed[%x]", ret);

      RK_MPI_VENC_ResetChn(pstPhotoCfg->venc_chn);
      pHandle->u32PhotoCnt--;
    }
  }

  if (pu8Photo)
    munmap(pu8Photo, u32PhotoLen);

  RKADK_LOGD("Exit get jpeg thread");
  return NULL;
}

static int RKADK_PHOTO_SetVencAttr(RKADK_PHOTO_HANDLE_S *pHandle,
                                    RKADK_PHOTO_THUMB_ATTR_S stThumbAttr,
                                    VENC_CHN_ATTR_S *pstVencAttr) {
  int ret;
  RKADK_U32 u32MaxWidth = 0, u32MaxHeight = 0;
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg = NULL;
  RKADK_PARAM_PHOTO_CFG_S *pstPhotoCfg = NULL;
  VENC_ATTR_JPEG_S *pstAttrJpege = &(pstVencAttr->stVencAttr.stAttrJpege);

  pstPhotoCfg = RKADK_PARAM_GetPhotoCfg(pHandle->u32CamId);
  if (!pstPhotoCfg) {
    RKADK_LOGE("RKADK_PARAM_GetPhotoCfg failed");
    return -1;
  }

  pstSensorCfg = RKADK_PARAM_GetSensorCfg(pHandle->u32CamId);
  if (!pstSensorCfg) {
    RKADK_LOGE("RKADK_PARAM_GetSensorCfg failed");
    return -1;
  }

  if (pstPhotoCfg->jpeg_slice) {
    ret = RKADK_PHOTO_SetSliceParam(pHandle, pstSensorCfg, pstPhotoCfg);
    if (ret)
      return -1;

    RKADK_PHOTO_SetViSliceParam(pHandle, pstPhotoCfg, pstPhotoCfg->vi_attr.stChnAttr.stSize.u32Width,
                                pstPhotoCfg->vi_attr.stChnAttr.stSize.u32Height);

    u32MaxWidth = pstPhotoCfg->max_slice_width;
    u32MaxHeight = pstPhotoCfg->max_slice_height;
  } else {
    u32MaxWidth = pstSensorCfg->max_width;
    u32MaxHeight = pstSensorCfg->max_height;
  }

  memset(pstVencAttr, 0, sizeof(VENC_CHN_ATTR_S));
  pstVencAttr->stVencAttr.enType = RK_VIDEO_ID_JPEG;
  pstVencAttr->stVencAttr.enPixelFormat =
      pstPhotoCfg->vi_attr.stChnAttr.enPixelFormat;
  pstVencAttr->stVencAttr.u32MaxPicWidth = u32MaxWidth;
  pstVencAttr->stVencAttr.u32MaxPicHeight = u32MaxHeight;
  pstVencAttr->stVencAttr.u32PicWidth = pstPhotoCfg->image_width;
  pstVencAttr->stVencAttr.u32PicHeight = pstPhotoCfg->image_height;
  pstVencAttr->stVencAttr.u32VirWidth = pstPhotoCfg->image_width;
  pstVencAttr->stVencAttr.u32VirHeight = pstPhotoCfg->image_height;
  pstVencAttr->stVencAttr.u32StreamBufCnt = 1;
  pstVencAttr->stVencAttr.u32BufSize = u32MaxWidth * u32MaxHeight;

  pstAttrJpege->bSupportDCF = (RK_BOOL)stThumbAttr.bSupportDCF;
  pstAttrJpege->stMPFCfg.u8LargeThumbNailNum =
      stThumbAttr.stMPFAttr.sCfg.u8LargeThumbNum;
  if (pstAttrJpege->stMPFCfg.u8LargeThumbNailNum >
      RKADK_MPF_LARGE_THUMB_NUM_MAX)
    pstAttrJpege->stMPFCfg.u8LargeThumbNailNum = RKADK_MPF_LARGE_THUMB_NUM_MAX;

  switch (stThumbAttr.stMPFAttr.eMode) {
  case RKADK_PHOTO_MPF_SINGLE:
    pstAttrJpege->enReceiveMode = VENC_PIC_RECEIVE_SINGLE;
    pstAttrJpege->stMPFCfg.astLargeThumbNailSize[0].u32Width =
        UPALIGNTO(stThumbAttr.stMPFAttr.sCfg.astLargeThumbSize[0].u32Width, 4);
    pstAttrJpege->stMPFCfg.astLargeThumbNailSize[0].u32Height =
        UPALIGNTO(stThumbAttr.stMPFAttr.sCfg.astLargeThumbSize[0].u32Height, 2);
    break;
  case RKADK_PHOTO_MPF_MULTI:
    pstAttrJpege->enReceiveMode = VENC_PIC_RECEIVE_MULTI;
    for (int i = 0; i < pstAttrJpege->stMPFCfg.u8LargeThumbNailNum; i++) {
      pstAttrJpege->stMPFCfg.astLargeThumbNailSize[i].u32Width = UPALIGNTO(
          stThumbAttr.stMPFAttr.sCfg.astLargeThumbSize[i].u32Width, 4);
      pstAttrJpege->stMPFCfg.astLargeThumbNailSize[i].u32Height = UPALIGNTO(
          stThumbAttr.stMPFAttr.sCfg.astLargeThumbSize[i].u32Height, 2);
    }
    break;
  default:
    pstAttrJpege->enReceiveMode = VENC_PIC_RECEIVE_BUTT;
    break;
  }

  return 0;
}

static void RKADK_PHOTO_CreateVencCombo(RKADK_S32 s32ChnId,
                                        VENC_CHN_ATTR_S *pstVencChnAttr,
                                        RKADK_PARAM_PHOTO_CFG_S *pstPhotoCfg) {
  VENC_RECV_PIC_PARAM_S stRecvParam;
  VENC_CHN_BUF_WRAP_S stVencChnBufWrap;
  VENC_CHN_REF_BUF_SHARE_S stVencChnRefBufShare;
  VENC_COMBO_ATTR_S stComboAttr;
  VENC_JPEG_PARAM_S stJpegParam;
  memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
  memset(&stVencChnBufWrap, 0, sizeof(VENC_CHN_BUF_WRAP_S));
  memset(&stVencChnRefBufShare, 0, sizeof(VENC_CHN_REF_BUF_SHARE_S));
  memset(&stComboAttr, 0, sizeof(VENC_COMBO_ATTR_S));
  memset(&stJpegParam, 0, sizeof(stJpegParam));

  RK_MPI_VENC_CreateChn(s32ChnId, pstVencChnAttr);

  stVencChnBufWrap.bEnable = RK_TRUE;
  RK_MPI_VENC_SetChnBufWrapAttr(s32ChnId, &stVencChnBufWrap);

  stVencChnRefBufShare.bEnable = RK_TRUE;
  RK_MPI_VENC_SetChnRefBufShareAttr(s32ChnId, &stVencChnRefBufShare);

  stRecvParam.s32RecvPicNum = 1;
  RK_MPI_VENC_StartRecvFrame(s32ChnId, &stRecvParam);

  stComboAttr.bEnable = RK_TRUE;
  stComboAttr.s32ChnId = pstPhotoCfg->combo_venc_chn;
  RK_MPI_VENC_SetComboAttr(s32ChnId, &stComboAttr);

  stJpegParam.u32Qfactor = pstPhotoCfg->qfactor;
  RK_MPI_VENC_SetJpegParam(s32ChnId, &stJpegParam);
}

static void RKADK_PHOTO_SetVideoChn(RKADK_PARAM_PHOTO_CFG_S *pstPhotoCfg,
                                    RKADK_U32 u32CamId, MPP_CHN_S *pstViChn,
                                    MPP_CHN_S *pstVencChn, MPP_CHN_S *pstSrcVpssChn,
                                    MPP_CHN_S *pstDstVpssChn) {
  pstViChn->enModId = RK_ID_VI;
  pstViChn->s32DevId = u32CamId;
  pstViChn->s32ChnId = pstPhotoCfg->vi_attr.u32ViChn;

  pstSrcVpssChn->enModId = RK_ID_VPSS;
  pstSrcVpssChn->s32DevId = pstPhotoCfg->vpss_grp;
  pstSrcVpssChn->s32ChnId = pstPhotoCfg->vpss_chn;

  pstDstVpssChn->enModId = RK_ID_VPSS;
  pstDstVpssChn->s32DevId = pstPhotoCfg->vpss_grp;
  pstDstVpssChn->s32ChnId = 0; //When vpss is dst, chn is equal to 0

  pstVencChn->enModId = RK_ID_VENC;
  pstVencChn->s32DevId = 0;
  pstVencChn->s32ChnId = pstPhotoCfg->venc_chn;
}

static bool RKADK_PHOTO_EnableJpegSlice(RKADK_U32 u32CamId,
                                 RKADK_PARAM_PHOTO_CFG_S *pstPhotoCfg) {
  bool bJpegSlice = false;
  RKADK_U32 u32ViWidth = pstPhotoCfg->vi_attr.stChnAttr.stSize.u32Width;
  RKADK_U32 u32ViHeight = pstPhotoCfg->vi_attr.stChnAttr.stSize.u32Height;

  if (!pstPhotoCfg->jpeg_slice)
    return false;

#ifdef JPEG_SLICE
  if (pstPhotoCfg->image_width != u32ViWidth ||
      pstPhotoCfg->image_height != u32ViHeight) {
    RKADK_LOGD("In[%d, %d], Out[%d, %d]", u32ViWidth, u32ViHeight,
               pstPhotoCfg->image_width, pstPhotoCfg->image_height);
    bJpegSlice = true;
  }
#endif

  return bJpegSlice;
}

static bool RKADK_PHOTO_IsUseVpss(RKADK_U32 u32CamId,
                                 RKADK_PARAM_PHOTO_CFG_S *pstPhotoCfg) {
  RKADK_U32 u32ViWidth = pstPhotoCfg->vi_attr.stChnAttr.stSize.u32Width;
  RKADK_U32 u32ViHeight = pstPhotoCfg->vi_attr.stChnAttr.stSize.u32Height;

  if (pstPhotoCfg->jpeg_slice)
    return false;

  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg = RKADK_PARAM_GetSensorCfg(u32CamId);
  if (!pstSensorCfg) {
    RKADK_LOGE("RKADK_PARAM_GetSensorCfg failed");
    return false;
  }

  if (pstPhotoCfg->image_width != u32ViWidth ||
      pstPhotoCfg->image_height != u32ViHeight) {
    RKADK_LOGD("In[%d, %d], Out[%d, %d]", u32ViWidth, u32ViHeight,
               pstPhotoCfg->image_width, pstPhotoCfg->image_height);
    return true;
  }

#ifdef RV1106_1103
  //use vpss switch resolution
  if (!pstSensorCfg->used_isp) {
    if (pstPhotoCfg->switch_res)
      return true;
  }
#endif

#ifndef RV1106_1103
  if (pstPhotoCfg->vi_attr.stChnAttr.enPixelFormat == RK_FMT_YUV422SP)
    return true;
#endif

  if (pstPhotoCfg->post_aiisp)
    return true;

  return false;
}

static void RKADK_PHOTO_ResetAttr(RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg,
                                  RKADK_PARAM_PHOTO_CFG_S *pstPhotoCfg,
                                  VENC_CHN_ATTR_S *pstVencAttr,
                                  VI_CHN_ATTR_S *pstViAttr,
                                  VPSS_CHN_ATTR_S *pstVpssAttr) {
  pstVencAttr->stVencAttr.u32PicWidth = pstPhotoCfg->image_width;
  pstVencAttr->stVencAttr.u32PicHeight = pstPhotoCfg->image_height;
  pstVencAttr->stVencAttr.u32VirWidth = pstPhotoCfg->image_width;
  pstVencAttr->stVencAttr.u32VirHeight = pstPhotoCfg->image_height;

  pstViAttr->stIspOpt.stMaxSize.u32Width = pstSensorCfg->max_width;
  pstViAttr->stIspOpt.stMaxSize.u32Height = pstSensorCfg->max_height;
  pstViAttr->stSize.u32Width = pstPhotoCfg->image_width;
  pstViAttr->stSize.u32Height = pstPhotoCfg->image_height;

  pstVpssAttr->u32Width = pstPhotoCfg->image_width;
  pstVpssAttr->u32Height = pstPhotoCfg->image_height;
}

RKADK_S32 RKADK_PHOTO_Init(RKADK_PHOTO_ATTR_S *pstPhotoAttr, RKADK_MW_PTR *ppHandle) {
  int ret;
  bool bSysInit = false;
  char name[RKADK_THREAD_NAME_LEN];
  MPP_CHN_S stViChn, stVencChn, stSrcVpssChn, stDstVpssChn;
  VENC_CHN_ATTR_S stVencAttr;
  RKADK_PARAM_THUMB_CFG_S *ptsThumbCfg = NULL;
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg = NULL;
  RKADK_PARAM_PHOTO_CFG_S *pstPhotoCfg = NULL;
  VPSS_GRP_ATTR_S stGrpAttr;
  VPSS_CHN_ATTR_S stChnAttr;
  RKADK_THUMB_MODULE_E enThumbModule = RKADK_THUMB_MODULE_PHOTO;
  RKADK_PHOTO_HANDLE_S *pHandle = NULL;

  RKADK_CHECK_POINTER(pstPhotoAttr, RKADK_FAILURE);
  RKADK_CHECK_CAMERAID(pstPhotoAttr->u32CamId, RKADK_FAILURE);

  if (*ppHandle) {
    RKADK_LOGE("Photo[%d] has been initialized", pstPhotoAttr->u32CamId);
    return -1;
  }

  RKADK_LOGI("Photo[%d] Init...", pstPhotoAttr->u32CamId);
  RKADK_BUFINFO("enter photo[%d]", pstPhotoAttr->u32CamId);

  pHandle = (RKADK_PHOTO_HANDLE_S *)malloc(sizeof(RKADK_PHOTO_HANDLE_S));
  if (!pHandle) {
    RKADK_LOGE("malloc photo handle failed");
    return -1;
  }
  memset(pHandle, 0, sizeof(RKADK_PHOTO_HANDLE_S));

  pHandle->u32CamId = pstPhotoAttr->u32CamId;
  pHandle->pDataRecvFn = pstPhotoAttr->pfnPhotoDataProc;
  pHandle->userdata = pstPhotoAttr->userdata;

  pstPhotoCfg = RKADK_PARAM_GetPhotoCfg(pstPhotoAttr->u32CamId);
  if (!pstPhotoCfg) {
    RKADK_LOGE("RKADK_PARAM_GetPhotoCfg failed");
    return -1;
  }

  ptsThumbCfg = RKADK_PARAM_GetThumbCfg(pstPhotoAttr->u32CamId);
  if (!ptsThumbCfg) {
    RKADK_LOGE("RKADK_PARAM_GetThumbCfg failed");
    return -1;
  }

  pstSensorCfg = RKADK_PARAM_GetSensorCfg(pstPhotoAttr->u32CamId);
  if (!pstSensorCfg) {
    RKADK_LOGE("RKADK_PARAM_GetSensorCfg failed");
    return -1;
  }

  bSysInit = RKADK_MPI_SYS_CHECK();
  if (!bSysInit) {
    RKADK_LOGE("System is not initialized");
    return -1;
  }

  RKADK_PHOTO_SetVideoChn(pstPhotoCfg, pstPhotoAttr->u32CamId, &stViChn, &stVencChn,
                     &stSrcVpssChn, &stDstVpssChn);
  ret = RKADK_PHOTO_SetVencAttr(pHandle, pstPhotoAttr->stThumbAttr, &stVencAttr);
  if (ret)
    return -1;

  // Create VI
  pHandle->u32ViChn = pstPhotoCfg->vi_attr.u32ViChn;
  ret = RKADK_MPI_VI_Init(pstPhotoAttr->u32CamId, stViChn.s32ChnId,
                          &(pstPhotoCfg->vi_attr.stChnAttr));
  if (ret) {
    RKADK_LOGE("RKADK_MPI_VI_Init failed[%x]", ret);
    return ret;
  }
  RKADK_BUFINFO("create vi[%d]", pHandle->u32ViChn);

  pHandle->stSliceParam.bJpegSlice = RKADK_PHOTO_EnableJpegSlice(pstPhotoAttr->u32CamId, pstPhotoCfg);
  pHandle->bUseVpss = RKADK_PHOTO_IsUseVpss(pstPhotoAttr->u32CamId, pstPhotoCfg);
  // Create VPSS
  if (pHandle->bUseVpss) {
    memset(&stGrpAttr, 0, sizeof(VPSS_GRP_ATTR_S));
    memset(&stChnAttr, 0, sizeof(VPSS_CHN_ATTR_S));

    stGrpAttr.u32MaxW = pstSensorCfg->max_width;
    stGrpAttr.u32MaxH = pstSensorCfg->max_height;
    stGrpAttr.enPixelFormat = pstPhotoCfg->vi_attr.stChnAttr.enPixelFormat;
    stGrpAttr.enCompressMode = COMPRESS_MODE_NONE;
    stGrpAttr.stFrameRate.s32SrcFrameRate = -1;
    stGrpAttr.stFrameRate.s32DstFrameRate = -1;
    stChnAttr.enChnMode = VPSS_CHN_MODE_USER;
    stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
    stChnAttr.enDynamicRange = DYNAMIC_RANGE_SDR8;
    stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
    stChnAttr.stFrameRate.s32SrcFrameRate = -1;
    stChnAttr.stFrameRate.s32DstFrameRate = -1;
    stChnAttr.u32Width = pstSensorCfg->max_width;
    stChnAttr.u32Height = pstSensorCfg->max_height;
    stChnAttr.u32Depth = 0;
    stChnAttr.u32FrameBufCnt = 1;
    ret = RKADK_MPI_VPSS_Init(pstPhotoCfg->vpss_grp, pstPhotoCfg->vpss_chn,
                              &stGrpAttr, &stChnAttr);
    if (ret) {
      RKADK_LOGE("RKADK_MPI_VPSS_Init vpss_grp[%d] vpss_chn[%d] falied[%x]",
                  pstPhotoCfg->vpss_grp, pstPhotoCfg->vpss_chn, ret);
      RKADK_MPI_VI_DeInit(pstPhotoAttr->u32CamId, pstPhotoCfg->vi_attr.u32ViChn);
      RKADK_MPI_VPSS_DeInit(pstPhotoCfg->vpss_grp, pstPhotoCfg->vpss_chn);
      return ret;
    }
    RKADK_BUFINFO("create vpss[%d, %d] end", pstPhotoCfg->vpss_grp, pstPhotoCfg->vpss_chn);

    ret = RK_MPI_VPSS_GetChnAttr(pstPhotoCfg->vpss_grp, pstPhotoCfg->vpss_chn, &stChnAttr);
    if (ret) {
      RKADK_LOGE("RK_MPI_VPSS_GetChnAttr vpss_grp[%d] vpss_chn[%d] falied[%x]",
                  pstPhotoCfg->vpss_grp, pstPhotoCfg->vpss_chn, ret);
      RKADK_MPI_VI_DeInit(pstPhotoAttr->u32CamId, pstPhotoCfg->vi_attr.u32ViChn);
      RKADK_MPI_VPSS_DeInit(pstPhotoCfg->vpss_grp, pstPhotoCfg->vpss_chn);
      return ret;
    }

    stChnAttr.u32Width = pstPhotoCfg->image_width;
    stChnAttr.u32Height = pstPhotoCfg->image_height;
    ret = RK_MPI_VPSS_SetChnAttr(pstPhotoCfg->vpss_grp, pstPhotoCfg->vpss_chn, &stChnAttr);
    if (ret) {
      RKADK_LOGE("RK_MPI_VPSS_SetChnAttr vpss_grp[%d] vpss_chn[%d] falied[%x]",
                  pstPhotoCfg->vpss_grp, pstPhotoCfg->vpss_chn, ret);
      RKADK_MPI_VI_DeInit(pstPhotoAttr->u32CamId, pstPhotoCfg->vi_attr.u32ViChn);
      RKADK_MPI_VPSS_DeInit(pstPhotoCfg->vpss_grp, pstPhotoCfg->vpss_chn);
      return ret;
    }

    if (pstPhotoCfg->post_aiisp)
      RKADK_MEDIA_EnablePostIsp(pstPhotoAttr->u32CamId, RKADK_STREAM_TYPE_SNAP, pstPhotoAttr->pstPostIspAttr);
  }

  // Create VENC
  if (pstPhotoCfg->enable_combo) {
    RKADK_LOGD("Select combo mode");
    RKADK_PHOTO_CreateVencCombo(stVencChn.s32ChnId, &stVencAttr, pstPhotoCfg);
    RKADK_BUFINFO("create venc[%d] combo", stVencChn.s32ChnId);
  } else {
    ret = RK_MPI_VENC_CreateChn(stVencChn.s32ChnId, &stVencAttr);
    if (ret) {
      RKADK_LOGE("Create Venc failed[%x]", ret);
      goto failed;
    }
    RKADK_BUFINFO("create venc[%d]", stVencChn.s32ChnId);

    VENC_CHN_REF_BUF_SHARE_S stVencChnRefBufShare;
    memset(&stVencChnRefBufShare, 0, sizeof(VENC_CHN_REF_BUF_SHARE_S));
    stVencChnRefBufShare.bEnable = RK_TRUE;
    RK_MPI_VENC_SetChnRefBufShareAttr(stVencChn.s32ChnId, &stVencChnRefBufShare);

    VENC_JPEG_PARAM_S stJpegParam;
    memset(&stJpegParam, 0, sizeof(VENC_JPEG_PARAM_S));
    stJpegParam.u32Qfactor = pstPhotoCfg->qfactor;
    if (pHandle->stSliceParam.bJpegSlice)
      stJpegParam.u32MCUPerECS = pHandle->stSliceParam.u32MCUPerECS;
    RK_MPI_VENC_SetJpegParam(stVencChn.s32ChnId, &stJpegParam);

    // must, for no streams callback running failed
    VENC_RECV_PIC_PARAM_S stRecvParam;
    stRecvParam.s32RecvPicNum = 1;
    ret = RK_MPI_VENC_StartRecvFrame(stVencChn.s32ChnId, &stRecvParam);
    if (ret) {
      RKADK_LOGE("RK_MPI_VENC_StartRecvFrame failed[%x]", ret);
      goto failed;
    }
  }

  ret = ThumbnailInit(pstPhotoAttr->u32CamId, enThumbModule, ptsThumbCfg);
  if (ret) {
    RKADK_LOGI("Thumbnail venc [%d] Init fail [%d]",
                ptsThumbCfg->photo_venc_chn, ret);
    goto failed;
  }

#ifndef THUMB_NORMAL
  ThumbnailChnBind(stVencChn.s32ChnId, ptsThumbCfg->photo_venc_chn);
#endif

  //if use isp, set mirror/flip using aiq
  if (!pstSensorCfg->used_isp) {
    RKADK_STREAM_TYPE_E enStrmType = RKADK_STREAM_TYPE_SNAP;
    if (pstSensorCfg->mirror)
      RKADK_MEDIA_ToggleVencMirror(pstPhotoAttr->u32CamId, enStrmType, pstSensorCfg->mirror);
    if (pstSensorCfg->flip)
      RKADK_MEDIA_ToggleVencFlip(pstPhotoAttr->u32CamId, enStrmType, pstSensorCfg->flip);
  }

  if (pHandle->stSliceParam.bJpegSlice) {
    pHandle->bGetJpeg = true;
    ret = pthread_create(&pHandle->stSliceParam.sliceTid, NULL, RKADK_PHOTO_SliceProc, pHandle);
    if (ret) {
      RKADK_LOGE("Create slice(%d) thread failed [%d]", pstPhotoAttr->u32CamId, ret);
      goto failed;
    }
    snprintf(name, sizeof(name), "Slice_%d", stVencChn.s32ChnId);
    pthread_setname_np(pHandle->stSliceParam.sliceTid, name);
  } else if (pHandle->bUseVpss) {
    // VPSS Bind VENC
    ret = RKADK_MPI_SYS_Bind(&stSrcVpssChn, &stVencChn);
    if (ret) {
      RKADK_LOGE("Bind VPSS[%d] to VENC[%d] failed[%x]", stSrcVpssChn.s32ChnId,
                 stVencChn.s32ChnId, ret);
      goto failed;
    }

    // VI Bind VPSS
    ret = RKADK_MPI_SYS_Bind(&stViChn, &stDstVpssChn);
    if (ret) {
      RKADK_LOGE("Bind VI[%d] to VPSS[%d] failed[%x]", stViChn.s32ChnId,
                 stDstVpssChn.s32DevId, ret);
      RKADK_MPI_SYS_UnBind(&stSrcVpssChn, &stVencChn);
      goto failed;
    }
  } else {
    // VI Bind VENC
    if (!pstPhotoCfg->enable_combo) {
      ret = RKADK_MPI_SYS_Bind(&stViChn, &stVencChn);
      if (ret) {
        RKADK_LOGE("Bind VI[%d] to VENC[%d] failed[%x]", stViChn.s32ChnId,
                    stVencChn.s32ChnId, ret);
        goto failed;
      }
    }
  }
  RKADK_BUFINFO("photo bind[%d, %d, %d]", stViChn.s32ChnId, stSrcVpssChn.s32ChnId, stVencChn.s32ChnId);

  if (!pHandle->stSliceParam.bJpegSlice) {
    pHandle->bGetJpeg = true;
    ret = pthread_create(&pHandle->tid, NULL, RKADK_PHOTO_GetJpeg, pHandle);
    if (ret) {
      RKADK_LOGE("Create get jpg(%d) thread failed [%d]", pstPhotoAttr->u32CamId, ret);
      goto failed;
    }
    snprintf(name, sizeof(name), "PhotoGetJpeg_%d", stVencChn.s32ChnId);
    pthread_setname_np(pHandle->tid, name);
  }

  *ppHandle = (RKADK_MW_PTR)pHandle;
  RKADK_LOGI("Photo[%d] Init End...", pstPhotoAttr->u32CamId);
  return 0;

failed:
  RKADK_LOGE("failed");
  RK_MPI_VENC_DestroyChn(stVencChn.s32ChnId);

  pHandle->bGetJpeg = false;
  if (pHandle->stSliceParam.sliceTid) {
    ret = pthread_join(pHandle->stSliceParam.sliceTid, NULL);
    if (ret)
      RKADK_LOGE("Exit slice thread failed!");
    pHandle->stSliceParam.sliceTid = 0;
  }

  if (pHandle->tid) {
    ret = pthread_join(pHandle->tid, NULL);
    if (ret)
      RKADK_LOGE("Exit get jpeg thread failed!");
    pHandle->tid = 0;
  }

  if (pHandle->bUseVpss)
    RKADK_MPI_VPSS_DeInit(pstPhotoCfg->vpss_grp, pstPhotoCfg->vpss_chn);

  RKADK_MPI_VI_DeInit(pstPhotoAttr->u32CamId, stViChn.s32ChnId);

  if (pHandle)
    free(pHandle);

  return ret;
}

RKADK_S32 RKADK_PHOTO_DeInit(RKADK_MW_PTR pHandle) {
  int ret;
  MPP_CHN_S stViChn, stVencChn, stSrcVpssChn, stDstVpssChn;
  RKADK_PHOTO_HANDLE_S *pstHandle;

  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);
  pstHandle = (RKADK_PHOTO_HANDLE_S *)pHandle;
  RKADK_CHECK_CAMERAID(pstHandle->u32CamId, RKADK_FAILURE);

  RKADK_LOGI("Photo[%d] DeInit...", pstHandle->u32CamId);

  RKADK_PARAM_PHOTO_CFG_S *pstPhotoCfg =
      RKADK_PARAM_GetPhotoCfg(pstHandle->u32CamId);
  if (!pstPhotoCfg) {
    RKADK_LOGE("RKADK_PARAM_GetPhotoCfg failed");
    return -1;
  }

  RKADK_PARAM_THUMB_CFG_S *ptsThumbCfg =
      RKADK_PARAM_GetThumbCfg(pstHandle->u32CamId);
  if (!ptsThumbCfg) {
    RKADK_LOGE("RKADK_PARAM_GetThumbCfg failed");
    return -1;
  }

  RKADK_PHOTO_SetVideoChn(pstPhotoCfg, pstHandle->u32CamId, &stViChn, &stVencChn,
                     &stSrcVpssChn, &stDstVpssChn);
  stViChn.s32ChnId = pstHandle->u32ViChn;

  pstHandle->bGetJpeg = false;

#if 1
  // The current version cannot be forced to exit
  ret = RK_MPI_VENC_StopRecvFrame(stVencChn.s32ChnId);
  if (ret) {
    RKADK_LOGE("StopRecvFrame VENC[%d] failed[%d]", stVencChn.s32ChnId, ret);
    return ret;
  }
#else
  VENC_RECV_PIC_PARAM_S stRecvParam;
  stRecvParam.s32RecvPicNum = 1;
  ret = RK_MPI_VENC_StartRecvFrame(stVencChn.s32ChnId, &stRecvParam);
  if (ret) {
    RKADK_LOGE("RK_MPI_VENC_StartRecvFrame failed[%x]", ret);
    return ret;
  }
#endif

  if (pstHandle->tid) {
    ret = pthread_join(pstHandle->tid, NULL);
    if (ret)
      RKADK_LOGE("Exit get jpeg thread failed!");
    pstHandle->tid = 0;
  }

  if (pstHandle->stSliceParam.bJpegSlice) {
    if (pstHandle->stSliceParam.sliceTid) {
      ret = pthread_join(pstHandle->stSliceParam.sliceTid, NULL);
      if (ret)
        RKADK_LOGE("Exit slice thread failed!");
      pstHandle->stSliceParam.sliceTid = 0;
    }
  } else if (pstHandle->bUseVpss) {
    // VPSS UnBind VENC
    ret = RKADK_MPI_SYS_UnBind(&stSrcVpssChn, &stVencChn);
    if (ret) {
      RKADK_LOGE("UnBind VPSS[%d] to VENC[%d] failed[%d]", stSrcVpssChn.s32ChnId,
                 stVencChn.s32ChnId, ret);
      return ret;
    }

    // VI UnBind VPSS
    ret = RKADK_MPI_SYS_UnBind(&stViChn, &stDstVpssChn);
    if (ret) {
      RKADK_LOGE("UnBind VI[%d] to VPSS[%d] failed[%d]", stViChn.s32ChnId,
                 stSrcVpssChn.s32ChnId, ret);
      return ret;
    }
  } else {
    if (!pstPhotoCfg->enable_combo) {
      // VI UnBind VENC
      ret = RKADK_MPI_SYS_UnBind(&stViChn, &stVencChn);
      if (ret) {
        RKADK_LOGE("UnBind VI[%d] to VENC[%d] failed[%d]", stViChn.s32ChnId,
                  stVencChn.s32ChnId, ret);
        return ret;
      }
    }
  }

  // Destory VENC
  ret = RK_MPI_VENC_DestroyChn(stVencChn.s32ChnId);
  if (ret) {
    RKADK_LOGE("Destory VENC[%d] failed[%d]", stVencChn.s32ChnId, ret);
    return ret;
  }

  // Destory VPSS
  if (pstHandle->bUseVpss) {
    ret = RKADK_MPI_VPSS_DeInit(pstPhotoCfg->vpss_grp, pstPhotoCfg->vpss_chn);
    if (ret) {
      RKADK_LOGE("DeInit VPSS[%d] failed[%d]", pstPhotoCfg->vpss_chn, ret);
      return ret;
    }
  }

  // Destory VI
  ret = RKADK_MPI_VI_DeInit(pstHandle->u32CamId, stViChn.s32ChnId);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_VI_DeInit failed[%d]", ret);
    return ret;
  }

  ThumbnailDeInit(pstHandle->u32CamId, RKADK_THUMB_MODULE_PHOTO, ptsThumbCfg);

  pstHandle->pDataRecvFn = NULL;
  RKADK_LOGI("Photo[%d] DeInit End...", pstHandle->u32CamId);

  if (pHandle) {
    free(pHandle);
    pHandle = NULL;
  }

  return 0;
}

RKADK_S32 RKADK_PHOTO_TakePhoto(RKADK_MW_PTR pHandle, RKADK_TAKE_PHOTO_ATTR_S *pstAttr) {
  int ret = 0;
  VENC_RECV_PIC_PARAM_S stRecvParam;
  RKADK_PHOTO_HANDLE_S *pstHandle;

  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);
  pstHandle = (RKADK_PHOTO_HANDLE_S *)pHandle;
  RKADK_CHECK_CAMERAID(pstHandle->u32CamId, RKADK_FAILURE);

  if (pstHandle->u32PhotoCnt > 0) {
    RKADK_LOGI("The last photo shoot wasn't over, u32PhotoCnt: %d", pstHandle->u32PhotoCnt);
    return 0;
  }

  if (pstHandle->bReseting) {
    RKADK_LOGI("Now reset, no photos allowed");
    return 0;
  }

  RKADK_PARAM_PHOTO_CFG_S *pstPhotoCfg =
      RKADK_PARAM_GetPhotoCfg(pstHandle->u32CamId);
  if (!pstPhotoCfg) {
    RKADK_LOGE("RKADK_PARAM_GetPhotoCfg failed");
    return -1;
  }

  if (pstAttr->enPhotoType == RKADK_PHOTO_TYPE_LAPSE) {
    // TODO
    RKADK_LOGI("nonsupport photo type = %d", pstAttr->enPhotoType);
    return -1;
  }

  memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
  if (pstAttr->enPhotoType == RKADK_PHOTO_TYPE_SINGLE)
    stRecvParam.s32RecvPicNum = 1;
  else
    stRecvParam.s32RecvPicNum = pstAttr->unPhotoTypeAttr.stMultipleAttr.s32Count;

  pstHandle->u32PhotoCnt = stRecvParam.s32RecvPicNum;
  if (pstHandle->stSliceParam.bJpegSlice)
    stRecvParam.s32RecvPicNum *= pstHandle->stSliceParam.u32SliceCount;

  RKADK_LOGI("Photo[%d] Take photo number = %d, s32RecvPicNum: %d", pstHandle->u32CamId, pstHandle->u32PhotoCnt, stRecvParam.s32RecvPicNum);

#ifndef THUMB_NORMAL
  ret = RK_MPI_VENC_StartRecvFrame(pstPhotoCfg->venc_chn, &stRecvParam);
#else
  RKADK_PARAM_THUMB_CFG_S *ptsThumbCfg = RKADK_PARAM_GetThumbCfg(pstHandle->u32CamId);
  if (!ptsThumbCfg) {
    RKADK_LOGE("RKADK_PARAM_GetThumbCfg failed");
    return -1;
  }

  ret = RK_MPI_VENC_StartRecvFrame(pstPhotoCfg->venc_chn, &stRecvParam);
  if(ret) {
    RKADK_LOGE("Take photo failed[%x]", ret);
    return ret;
  }

  stRecvParam.s32RecvPicNum = pstHandle->u32PhotoCnt;
  ret = RK_MPI_VENC_StartRecvFrame(ptsThumbCfg->photo_venc_chn, &stRecvParam);
  if(ret) {
    RKADK_LOGE("Start recv thumb failed[%x]", ret);
    return ret;
  }
#endif

  return ret;
}

RKADK_S32 RKADK_PHOTO_Reset(RKADK_MW_PTR *pHandle) {
  int ret;
  bool bPhoto;
  RKADK_PARAM_PHOTO_CFG_S *pstPhotoCfg = NULL;
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg = NULL;
  MPP_CHN_S stViChn, stVencChn, stSrcVpssChn, stDstVpssChn;
  VENC_CHN_ATTR_S stVencAttr;
  VI_CHN_ATTR_S stViAttr;
  VPSS_CHN_ATTR_S stVpssAttr;
  RKADK_PHOTO_HANDLE_S *pstHandle;

  RKADK_CHECK_POINTER(*pHandle, RKADK_FAILURE);
  pstHandle = (RKADK_PHOTO_HANDLE_S *)*pHandle;
  RKADK_CHECK_CAMERAID(pstHandle->u32CamId, RKADK_FAILURE);

#ifndef RV1106_1103
  RKADK_LOGE("rv1126/1109 nonsupport dynamic setting resolution, please recreate!");
  return -1;
#endif

  if (pstHandle->u32PhotoCnt > 0) {
    RKADK_LOGE("The last photo shoot wasn't over, u32PhotoCnt: %d", pstHandle->u32PhotoCnt);
    return -1;
  }

  RKADK_LOGI("Photo[%d] Reset start...", pstHandle->u32CamId);

  pstPhotoCfg = RKADK_PARAM_GetPhotoCfg(pstHandle->u32CamId);
  if (!pstPhotoCfg) {
    RKADK_LOGE("RKADK_PARAM_GetPhotoCfg failed");
    return -1;
  }

  pstSensorCfg = RKADK_PARAM_GetSensorCfg(pstHandle->u32CamId);
  if (!pstSensorCfg) {
    RKADK_LOGE("RKADK_PARAM_GetSensorCfg failed");
    return -1;
  }

  if (pstPhotoCfg->enable_combo) {
    RKADK_LOGE("Photo combo venc [%d], not support reset",
                pstPhotoCfg->combo_venc_chn);
    return -1;
  }

  pstHandle->bReseting = true;

  RKADK_PHOTO_SetVideoChn(pstPhotoCfg, pstHandle->u32CamId, &stViChn, &stVencChn,
                     &stSrcVpssChn, &stDstVpssChn);
  stViChn.s32ChnId = pstHandle->u32ViChn;

  memset(&stVencAttr, 0, sizeof(VENC_CHN_ATTR_S));
  memset(&stViAttr, 0, sizeof(VI_CHN_ATTR_S));
  memset(&stVpssAttr, 0, sizeof(VPSS_CHN_ATTR_S));

  if (!pstHandle->stSliceParam.bJpegSlice) {
    ret = RK_MPI_VI_GetChnAttr(pstHandle->u32CamId, stViChn.s32ChnId, &stViAttr);
    if (ret != RK_SUCCESS) {
      RKADK_LOGE("Photo get vi[%d] attr failed[%x]", stViChn.s32ChnId, ret);
      goto EXIT;
    }
  }

  ret = RK_MPI_VENC_GetChnAttr(stVencChn.s32ChnId, &stVencAttr);
  if (ret != RK_SUCCESS) {
    RKADK_LOGE("Photo get venc[%d] attr failed[%x]", stVencChn.s32ChnId, ret);
    goto EXIT;
  }

  if (pstHandle->bUseVpss) {
    ret = RK_MPI_VPSS_GetChnAttr(pstPhotoCfg->vpss_grp, pstPhotoCfg->vpss_chn, &stVpssAttr);
    if (ret) {
      RKADK_LOGE("Photo get vpss grp[%d] chn[%d] attr failed[%x]",
                  pstPhotoCfg->vpss_grp, pstPhotoCfg->vpss_chn, ret);
      goto EXIT;
    }
  }

  bPhoto = RKADK_MEDIA_CompareResolution(&stVencAttr,
                                  pstPhotoCfg->image_width,
                                  pstPhotoCfg->image_height);
  if (!bPhoto) {
    RKADK_LOGE("Photo dose not reset venc attr");
    goto EXIT;
  }

  RKADK_PHOTO_ResetAttr(pstSensorCfg, pstPhotoCfg,
                        &stVencAttr, &stViAttr, &stVpssAttr);

  if (pstHandle->stSliceParam.bJpegSlice) {
    ret = RKADK_PHOTO_SetSliceParam(pstHandle, pstSensorCfg, pstPhotoCfg);
    if (ret)
      goto EXIT;

    RKADK_PHOTO_SetViSliceParam(pstHandle, pstPhotoCfg, pstPhotoCfg->vi_attr.stChnAttr.stSize.u32Width,
                                pstPhotoCfg->vi_attr.stChnAttr.stSize.u32Height);
  } else if (pstHandle->bUseVpss) {
    // VPSS UnBind VENC
    ret = RKADK_MPI_SYS_UnBind(&stSrcVpssChn, &stVencChn);
    if (ret) {
      RKADK_LOGE("UnBind VPSS[%d] to VENC[%d] failed[%x]", stSrcVpssChn.s32ChnId,
                 stVencChn.s32ChnId, ret);
      goto EXIT;
    }
  } else {
    ret = RKADK_MPI_SYS_UnBind(&stViChn, &stVencChn);
    if (ret != RK_SUCCESS) {
      RKADK_LOGE("Photo VI UnBind VENC [%d %d]fail %x", stViChn.s32ChnId,
                  stVencChn.s32ChnId, ret);
      goto EXIT;
    }
  }

  ret = RK_MPI_VENC_SetChnAttr(stVencChn.s32ChnId, &stVencAttr);
  if (ret != RK_SUCCESS) {
    RKADK_LOGE("Photo set venc[%d] attr failed %x",
                stVencChn.s32ChnId, ret);
    goto EXIT;
  }

  if (pstHandle->stSliceParam.bJpegSlice) {
    VENC_JPEG_PARAM_S stJpegParam;
    memset(&stJpegParam, 0, sizeof(VENC_JPEG_PARAM_S));

#if 0
    //RK_MPI_VENC_GetJpegParam not yet released
    ret = RK_MPI_VENC_GetJpegParam(stVencChn.s32ChnId, &stJpegParam);
    if (ret) {
      RKADK_LOGE("GetJpegParam failed[%x]", ret);
      goto EXIT;
    }
#else
    stJpegParam.u32Qfactor = pstPhotoCfg->qfactor;
#endif

    stJpegParam.u32MCUPerECS = pstHandle->stSliceParam.u32MCUPerECS;
    ret = RK_MPI_VENC_SetJpegParam(stVencChn.s32ChnId, &stJpegParam);
    if (ret) {
      RKADK_LOGE("SetJpegParam failed[%x]", ret);
      goto EXIT;
    }
  } else if (pstHandle->bUseVpss) {
    ret = RK_MPI_VPSS_SetChnAttr(pstPhotoCfg->vpss_grp, pstPhotoCfg->vpss_chn, &stVpssAttr);
    if (ret) {
      RKADK_LOGE("Photo set vpss grp[%d] chn[%d] attr falied[%x]",
                  pstPhotoCfg->vpss_grp, pstPhotoCfg->vpss_chn, ret);
      goto EXIT;
    }

    ret = RKADK_MPI_SYS_Bind(&stSrcVpssChn, &stVencChn);
    if (ret) {
      RKADK_LOGE("Photo VPSS[%d] Bind VENC[%d] failed[%x]", stSrcVpssChn.s32ChnId,
                 stVencChn.s32ChnId, ret);
      goto EXIT;
    }
  } else {
    ret = RK_MPI_VI_SetChnAttr(pstHandle->u32CamId, stViChn.s32ChnId,
                              &stViAttr);
    if (ret != RK_SUCCESS) {
      RKADK_LOGE("Photo set VI[%d] attr failed %x", stViChn.s32ChnId, ret);
      goto EXIT;
    }

    ret = RKADK_MPI_SYS_Bind(&stViChn, &stVencChn);
    if (ret != RK_SUCCESS) {
      RKADK_LOGE("Photo VI Bind VENC [%d %d] fail %x",stViChn.s32ChnId,
                  stVencChn.s32ChnId, ret);
      goto EXIT;
    }
  }

  pstHandle->bReseting = false;
  RKADK_LOGI("Photo[%d] Reset end...", pstHandle->u32CamId);
  return 0;

EXIT:
  pstHandle->bReseting = false;
  return -1;
}

static RKADK_S32 RKADK_PHOTO_GetJpgResolution(RKADK_CHAR *pcFileName,
                                        RKADK_PHOTO_DATA_ATTR_S *pstDataAttr) {
  RKADK_U32 cur = 0;

  if (pstDataAttr->pu8Buf[0] != 0xFF || pstDataAttr->pu8Buf[1] != 0xD8) {
    RKADK_LOGD("Invalid jpeg data");
    return -1;
  }

  cur += 2;
  while(cur + 4 + 4 < pstDataAttr->u32BufSize) {
    if (pstDataAttr->pu8Buf[cur] != 0xFF) {
      RKADK_LOGE("Bad Jpg file, 0xFF expected at offset 0x%x", cur);
      break;
    }

    if (pstDataAttr->pu8Buf[cur + 1] == 0xC0 || pstDataAttr->pu8Buf[cur + 1] == 0xC2) {
      cur += 5;
      pstDataAttr->u32Height = bswap_16(*(RKADK_U16 *) (pstDataAttr->pu8Buf + cur));
      cur += 2;
      pstDataAttr->u32Width = bswap_16(*(RKADK_U16 *) (pstDataAttr->pu8Buf + cur));
      RKADK_LOGD("%s w*h[%d, %d]", pcFileName, pstDataAttr->u32Width, pstDataAttr->u32Height);
      return 0;
    }

    cur += 2;
    cur += bswap_16(*(RKADK_U16 *) (pstDataAttr->pu8Buf + cur));
  }

  return -1;
}

static RKADK_S32 RKADK_PHOTO_BuildInThm(FILE *fd,
                                        RKADK_THUMB_ATTR_S *pstThumbAttr) {

  char tag[JPG_THUMB_TAG_LEN];
  RKADK_U32 u32Size, u32totalSize;

  // offset: 4btye tag
  if (fseek(fd, -JPG_THUMB_TAG_LEN, SEEK_END)) {
    RKADK_LOGE("seek file failed");
    return -1;
  }

  // read thm tag
  if (fread(tag, JPG_THUMB_TAG_LEN, 1, fd) != 1) {
    RKADK_LOGE("read jpg thumb tag failed");
    return -1;
  }

  if (tag[0] == 't' || tag[1] == 'h' || tag[2] == 'm') {
    RKADK_LOGD("already exist thm[%d] tag, cover", tag[3]);

    // offset: 4btye tag + 4btye size
    if (fseek(fd, -(JPG_THUMB_TAG_LEN + 4), SEEK_CUR)) {
      RKADK_LOGE("seek file failed");
      return -1;
    }

    // read thm size
    if (fread(&u32Size, 4, 1, fd) != 1) {
      RKADK_LOGE("read jpg thumb tag failed");
      return -1;
    }

    u32totalSize = pstThumbAttr->u32BufSize + 16;
    if (u32totalSize < u32Size)
      u32Size = u32totalSize;

    // offset: thm data size + 4byte thm size
    if (fseek(fd, -(u32Size + 4), SEEK_CUR)) {
      RKADK_LOGE("seek file failed");
      return -1;
    }
  }

  if (fwrite(pstThumbAttr->pu8Buf, pstThumbAttr->u32BufSize, 1, fd) != 1) {
    RKADK_LOGE("write thm data failed");
    return -1;
  }

  if (fwrite((char *)&pstThumbAttr->u32Width, 4, 1, fd) != 1) {
    RKADK_LOGE("write thm width failed");
    return -1;
  }

  if (fwrite((char *)&pstThumbAttr->u32Height, 4, 1, fd) != 1) {
    RKADK_LOGE("write thm height failed");
    return -1;
  }

  if (fwrite((char *)&pstThumbAttr->u32VirWidth, 4, 1, fd) != 1) {
    RKADK_LOGE("write thm virtual width failed");
    return -1;
  }

  if (fwrite((char *)&pstThumbAttr->u32VirHeight, 4, 1, fd) != 1) {
    RKADK_LOGE("write thm virtual height failed");
    return -1;
  }

  // 16: 4bytes width + 4bytes height + 4bytes VirWidth + 4bytes VirHeight
  u32Size = pstThumbAttr->u32BufSize + 16;
  if (fwrite((char *)&u32Size, sizeof(RKADK_U32), 1, fd) != 1) {
    RKADK_LOGE("write thm len failed");
    return -1;
  }

  tag[0] = 't';
  tag[1] = 'h';
  tag[2] = 'm';
  tag[3] = pstThumbAttr->enType;

  if (fwrite(tag, JPG_THUMB_TAG_LEN, 1, fd) != 1) {
    RKADK_LOGE("write thm tag failed");
    return -1;
  }

  RKADK_LOGD("done");
  return 0;
}

static RKADK_S32 RKADK_PHOTO_GetThmInFile(FILE *fd,
                                          RKADK_THUMB_ATTR_S *pstThumbAttr) {
  int ret = -1;
  bool bMallocBuf = false;
  char tag[JPG_THUMB_TAG_LEN];
  RKADK_U32 u32Size = 0;

  if (pstThumbAttr->enType == RKADK_THUMB_TYPE_JPEG)
    return -1;

  // seek to file end
  if (fseek(fd, 0, SEEK_END)) {
    RKADK_LOGE("seek file end failed");
    return -1;
  }

  if (!pstThumbAttr->pu8Buf)
    bMallocBuf = true;

  while (1) {
    // offset: 4btye tag
    if (fseek(fd, -JPG_THUMB_TAG_LEN, SEEK_CUR)) {
      RKADK_LOGE("seek file failed");
      break;
    }

    // read thm tag
    if (fread(tag, JPG_THUMB_TAG_LEN, 1, fd) != 1) {
      RKADK_LOGE("read jpg thumb tag failed");
      break;
    }

    if (tag[2] == 0xFF && tag[3] == 0xD9) {
      RKADK_LOGD("read jpg EOF tag(0xFFD9)");
      break;
    }

    if (tag[0] != 't' || tag[1] != 'h' || tag[2] != 'm') {
      RKADK_LOGD("can't read thm[%d] tag", pstThumbAttr->enType);
      break;
    }

    RKADK_LOGD("tag[0] = %d, %c", tag[0], tag[0]);
    RKADK_LOGD("tag[1] = %d, %c", tag[1], tag[1]);
    RKADK_LOGD("tag[2] = %d, %c", tag[2], tag[2]);
    RKADK_LOGD("tag[3] = %d", tag[3]);
    RKADK_LOGD("pstThumbAttr->enType = %d", pstThumbAttr->enType);

    // offset: 4btye tag + 4btye size
    if (fseek(fd, -(JPG_THUMB_TAG_LEN + 4), SEEK_CUR)) {
      RKADK_LOGE("seek file failed");
      break;
    }

    // read thm size
    if (fread(&u32Size, 4, 1, fd) != 1) {
      RKADK_LOGE("read jpg thumb tag failed");
      break;
    }
    RKADK_LOGD("u32Size = %d", u32Size);

    if (tag[3] == pstThumbAttr->enType) {
      // 16: 4bytes width + 4bytes height + 4bytes VirWidth + 4bytes VirHeight
      RKADK_U32 u32DataLen = u32Size - 16;

      // offset: thm data size + 4byte thm size
      if (fseek(fd, -(u32Size + 4), SEEK_CUR)) {
        RKADK_LOGE("seek file failed");
        break;
      }

      if (bMallocBuf) {
        pstThumbAttr->pu8Buf = (RKADK_U8 *)malloc(u32DataLen);
        if (!pstThumbAttr->pu8Buf) {
          RKADK_LOGE("malloc thumbnail buffer[%d] failed", u32DataLen);
          break;
        }
        RKADK_LOGD("malloc thumbnail buffer[%p], u32DataLen[%d]",
                   pstThumbAttr->pu8Buf, u32DataLen);

        pstThumbAttr->u32BufSize = u32DataLen;
      } else {
        if (u32DataLen > pstThumbAttr->u32BufSize)
          RKADK_LOGW("buffer size[%d] < thumbnail data len[%d]",
                     pstThumbAttr->u32BufSize, u32DataLen);
        else
          pstThumbAttr->u32BufSize = u32DataLen;
      }

      // read thm data
      if (fread(pstThumbAttr->pu8Buf, pstThumbAttr->u32BufSize, 1, fd) != 1) {
        RKADK_LOGE("read jpg thumb data failed");
        break;
      }

      // seek the remain data
      if (u32DataLen > pstThumbAttr->u32BufSize) {
        if (fseek(fd, u32DataLen - pstThumbAttr->u32BufSize, SEEK_CUR)) {
          RKADK_LOGE("seek remain data failed");
          break;
        }
      }

      if (fread(&(pstThumbAttr->u32Width), 4, 1, fd) != 1) {
        RKADK_LOGE("read jpg thumb width failed");
        break;
      }

      if (fread(&(pstThumbAttr->u32Height), 4, 1, fd) != 1) {
        RKADK_LOGE("read jpg thumb height failed");
        break;
      }

      if (fread(&(pstThumbAttr->u32VirWidth), 4, 1, fd) != 1) {
        RKADK_LOGE("read jpg thumb virtual width failed");
        break;
      }

      if (fread(&(pstThumbAttr->u32VirHeight), 4, 1, fd) != 1) {
        RKADK_LOGE("read jpg thumb virtual height failed");
        break;
      }

      ret = 0;
      RKADK_LOGD("[%d, %d, %d, %d]", pstThumbAttr->u32Width,
                 pstThumbAttr->u32Height, pstThumbAttr->u32VirWidth,
                 pstThumbAttr->u32VirHeight);
      RKADK_LOGD("done");
      break;
    } else {
      if (fseek(fd, -(u32Size + 4), SEEK_CUR)) {
        RKADK_LOGE("seek failed");
        break;
      }
    }
  }

  if (ret) {
    if (fseek(fd, 0, SEEK_SET)) {
      RKADK_LOGE("seek jpg file header failed");
      ret = 0;
    }

    if (bMallocBuf)
      RKADK_PHOTO_ThumbBufFree(pstThumbAttr);
  }

  return ret;
}

static RKADK_S32 RKADK_PHOTO_GetJpgThm(FILE *fd, RKADK_CHAR *pszFileName,
                                   RKADK_THUMB_ATTR_S *pstThumbAttr) {
  int ret = -1, i;
  RKADK_U16 u16ExifLen;
  RKADK_U8 *pFile;
  RKADK_S64 len = 0, cur = 0, exif_end = 0;

  if (fseek(fd, 0, SEEK_END) || (len = ftell(fd)) == -1 ||
      fseek(fd, 0, SEEK_SET)) {
    RKADK_LOGE("seek %s failed", pszFileName);
    return -1;
  }

  pFile = (RKADK_U8 *)mmap(NULL, len, PROT_READ, MAP_SHARED, fileno(fd), 0);
  if (pFile == MAP_FAILED) {
    RKADK_LOGE("mmap %s failed, errno: %d", pszFileName, errno);
    return -1;
  }

  while (cur + 4 + 4 < len) {
    if (pFile[cur] != 0xFF) {
      RKADK_LOGE("Bad Jpg file, 0xFF expected at offset 0x%llx", cur);
      goto exit;
    }

    if (pFile[cur + 1] == 0xD8 || pFile[cur + 1] == 0xD9) {
      cur += 2;
    } else if (pFile[cur + 1] == 0xE1) {
      int _tmp;
      _tmp = cur + 1 + 2 + 1;
      if (pFile[_tmp + 0] == 'E' && pFile[_tmp + 1] == 'x' &&
          pFile[_tmp + 2] == 'i' && pFile[_tmp + 3] == 'f') {
        /* Found the 0xFFE1 (EXIF) */
        cur += 2;
        break;
      }
    }

    cur += 2;
    cur += bswap_16(*(RKADK_U16 *) (pFile + cur));
  }

  /* Instead of parsing Exif, searching 0xFFD8 */
  u16ExifLen = bswap_16(*(RKADK_U16 *) (pFile + cur));

  if (u16ExifLen + cur >= len) {
    RKADK_LOGE("Bad Jpg file, Exif len exceed at offset 0x%llx", cur);
    goto exit;
  }

  exif_end = cur + u16ExifLen;
  for (i = 2; i < u16ExifLen; i++) {
    if (pFile[cur + i] != 0xFF || pFile[cur + i + 1] != 0xD8)
      continue;

    cur += i;
    /* Found the thumbnail */
    break;
  }

  /* pFile[cur, exif_end) is the thumbnail */
  if (!pstThumbAttr->pu8Buf) {
    pstThumbAttr->pu8Buf = (RKADK_U8 *)malloc(exif_end - cur);
    if (!pstThumbAttr->pu8Buf) {
      RKADK_LOGE("malloc jpg thumb buffer failed, len = %lld", exif_end - cur);
      goto exit;
    }

    pstThumbAttr->u32BufSize = exif_end - cur;
    RKADK_LOGD("malloc jpg thumb buffer[%p, %d]", pstThumbAttr->pu8Buf, pstThumbAttr->u32BufSize);
  } else {
    if ((int)pstThumbAttr->u32BufSize < (exif_end - cur))
        RKADK_LOGW("buffer size[%d] < thm data size[%lld]",
                   pstThumbAttr->u32BufSize, exif_end - cur);
    else
      pstThumbAttr->u32BufSize = exif_end - cur;
  }

  memcpy(pstThumbAttr->pu8Buf, pFile + cur, pstThumbAttr->u32BufSize);
  ret = RKADK_SUCCESS;

exit:
  munmap(pFile, len);
  return ret;
}

static RKADK_S32 RKADK_PHOTO_GetThumb(RKADK_U32 u32CamId,
                                      RKADK_CHAR *pszFileName,
                                      RKADK_JPG_THUMB_TYPE_E eThmType,
                                      RKADK_THUMB_ATTR_S *pstThumbAttr) {
  FILE *fd = NULL;
  RKADK_S32 ret = -1, result;
  RKADK_THUMB_ATTR_S stTmpThmAttr;
  RKADK_THUMB_ATTR_S *pstThmAttr = NULL;
  struct stat stStatBuf;
  struct utimbuf stTimebuf;
  bool bFree = false;

  RKADK_PARAM_THUMB_CFG_S *ptsThumbCfg = RKADK_PARAM_GetThumbCfg(u32CamId);
  if (!ptsThumbCfg) {
    RKADK_LOGE("RKADK_PARAM_GetThumbCfg failed");
    return -1;
  }

  if (!pstThumbAttr->u32Width || !pstThumbAttr->u32Height) {
    pstThumbAttr->u32Width = UPALIGNTO(ptsThumbCfg->thumb_width, 4);
    pstThumbAttr->u32Height = UPALIGNTO(ptsThumbCfg->thumb_height, 2);
  }

  if (!pstThumbAttr->u32VirWidth || !pstThumbAttr->u32VirHeight) {
    pstThumbAttr->u32VirWidth = pstThumbAttr->u32Width;
    pstThumbAttr->u32VirHeight = pstThumbAttr->u32Height;
  }

  fd = fopen(pszFileName, "r+");
  if (!fd) {
    RKADK_LOGE("open %s failed", pszFileName);
    return -1;
  }

  memset(&stTimebuf, 0, sizeof(struct utimbuf));
  result = stat(pszFileName, &stStatBuf);
  if (result) {
    RKADK_LOGW("stat[%s] failed[%d]", pszFileName, result);
  } else {
    stTimebuf.actime = stStatBuf.st_atime;
    stTimebuf.modtime = stStatBuf.st_mtime;
  }

  ret = RKADK_PHOTO_GetThmInFile(fd, pstThumbAttr);
  if (!ret)
    goto exit;

  memset(&stTmpThmAttr, 0, sizeof(RKADK_THUMB_ATTR_S));
  if (pstThumbAttr->enType == RKADK_THUMB_TYPE_JPEG)
    pstThmAttr = pstThumbAttr;
  else
    pstThmAttr = &stTmpThmAttr;

  ret = RKADK_PHOTO_GetJpgThm(fd, pszFileName, pstThmAttr);
  if (ret) {
    RKADK_LOGE("Get Jpg thumbnail failed");
    goto exit;
  }

  if (pstThumbAttr->enType == RKADK_THUMB_TYPE_JPEG)
    goto exit;

  if (RKADK_PHOTO_GetJpgResolution(pszFileName, &stTmpThmAttr)) {
    RKADK_LOGE("get %s jpg thumb resolution failed, use default", pszFileName);
    stTmpThmAttr.u32Width = ptsThumbCfg->thumb_width;
    stTmpThmAttr.u32VirWidth = stTmpThmAttr.u32Width;
    stTmpThmAttr.u32Height = ptsThumbCfg->thumb_height;
    stTmpThmAttr.u32VirHeight = stTmpThmAttr.u32Height;
  }

  ret = ThumbnailJpgDecode(&stTmpThmAttr, pstThumbAttr, &bFree);
  if (!ret) {
    if (RKADK_PHOTO_BuildInThm(fd, pstThumbAttr))
      RKADK_LOGE("RKADK_PHOTO_BuildInThm failed");
  }

exit:
  if (fd)
    fclose(fd);

  if (ret)
    RKADK_PHOTO_ThumbBufFree(pstThumbAttr);

  if (bFree)
    RKADK_PHOTO_ThumbBufFree(&stTmpThmAttr);

  if (stTimebuf.actime != 0 && stTimebuf.modtime != 0) {
    result = utime(pszFileName, &stTimebuf);
    if (result)
      RKADK_LOGW("utime[%s] failed[%d]", pszFileName, result);
  }

  return ret;
}

RKADK_S32 RKADK_PHOTO_GetThmInJpg(RKADK_U32 u32CamId,
                                  RKADK_CHAR *pszFileName,
                                  RKADK_JPG_THUMB_TYPE_E eThmType,
                                  RKADK_U8 *pu8Buf, RKADK_U32 *pu32Size) {
  int ret;
  RKADK_THUMB_ATTR_S stThumbAttr;

  RKADK_CHECK_POINTER(pszFileName, RKADK_FAILURE);
  RKADK_CHECK_POINTER(pu8Buf, RKADK_FAILURE);

  memset(&stThumbAttr, 0, sizeof(RKADK_THUMB_ATTR_S));

  stThumbAttr.u32Width = 0;
  stThumbAttr.u32Height = 0;
  stThumbAttr.u32VirWidth = 0;
  stThumbAttr.u32VirHeight = 0;
  stThumbAttr.enType = RKADK_THUMB_TYPE_JPEG;
  stThumbAttr.pu8Buf = pu8Buf;
  stThumbAttr.u32BufSize = *pu32Size;
  stThumbAttr.s32VdecChn = PHOTO_THM_VDEC_CHN;
  stThumbAttr.s32VpssGrp = PHOTO_THM_VPSS_GRP;
  stThumbAttr.s32VpssChn = PHOTO_THM_VPSS_CHN;

  ret = RKADK_PHOTO_GetThumb(u32CamId, pszFileName, eThmType, &stThumbAttr);
  *pu32Size = stThumbAttr.u32BufSize;

  return ret;
}

RKADK_S32 RKADK_PHOTO_ThumbBufFree(RKADK_THUMB_ATTR_S *pstThumbAttr) {
  return RKADK_MEDIA_FrameFree((RKADK_FRAME_ATTR_S *)pstThumbAttr);
}

RKADK_S32 RKADK_PHOTO_GetThmInJpgEx(RKADK_U32 u32CamId, RKADK_CHAR *pszFileName,
                                    RKADK_JPG_THUMB_TYPE_E eThmType,
                                    RKADK_THUMB_ATTR_S *pstThumbAttr) {
  int ret;

  RKADK_CHECK_POINTER(pszFileName, RKADK_FAILURE);
  RKADK_CHECK_POINTER(pstThumbAttr, RKADK_FAILURE);

  if (pstThumbAttr->s32VdecChn < 0)
    pstThumbAttr->s32VdecChn = PHOTO_THM_VDEC_CHN;

  if (pstThumbAttr->s32VpssGrp < 0)
    pstThumbAttr->s32VpssGrp = PHOTO_THM_VPSS_GRP;

  if (pstThumbAttr->s32VpssChn < 0)
    pstThumbAttr->s32VpssChn = PHOTO_THM_VPSS_CHN;

  ret = RKADK_PHOTO_GetThumb(u32CamId, pszFileName, eThmType, pstThumbAttr);
  if (ret)
    RKADK_PHOTO_ThumbBufFree(pstThumbAttr);

  return ret;
}

RKADK_S32 RKADK_PHOTO_GetData(RKADK_CHAR *pcFileName,
                              RKADK_PHOTO_DATA_ATTR_S *pstDataAttr) {
  int ret = -1;
  RKADK_S32 s32ReadSize = 0, s32DataSize = 0;
  RKADK_THUMB_ATTR_S stSrcDateAttr;
  bool bFree = false;

  RKADK_CHECK_POINTER(pcFileName, RKADK_FAILURE);
  RKADK_CHECK_POINTER(pstDataAttr, RKADK_FAILURE);

  if (pstDataAttr->s32VdecChn < 0)
    pstDataAttr->s32VdecChn = GET_DATA_VDEC_CHN;

  if (pstDataAttr->s32VpssGrp < 0)
    pstDataAttr->s32VpssGrp = GET_DATA_VPSS_GRP;

  if (pstDataAttr->s32VpssChn < 0)
    pstDataAttr->s32VpssChn = GET_DATA_VPSS_CHN;

  if (pstDataAttr->enType == RKADK_THUMB_TYPE_JPEG) {
    RKADK_LOGE("Invalid type = %d", pstDataAttr->enType);
    return -1;
  }

  FILE *fd = fopen(pcFileName, "rb");
  if (!fd) {
    RKADK_LOGE("Could not open %s", pcFileName);
    return -1;
  }

  if (fseek(fd, 0, SEEK_END) || (s32DataSize = ftell(fd)) == -1 ||
      fseek(fd, 0, SEEK_SET)) {
    RKADK_LOGE("get %s size failed", pcFileName);
    goto exit;
  }

  memset(&stSrcDateAttr, 0, sizeof(RKADK_THUMB_ATTR_S));
  stSrcDateAttr.enType = RKADK_THUMB_TYPE_JPEG;
  stSrcDateAttr.pu8Buf = (RKADK_U8 *)malloc(s32DataSize);
  if (!stSrcDateAttr.pu8Buf) {
    RKADK_LOGE("malloc failed, len: %d", s32DataSize);
    goto exit;
  }
  RKADK_LOGD("malloc jpg buffer[%p, %d]", stSrcDateAttr.pu8Buf, s32DataSize);

  memset(stSrcDateAttr.pu8Buf, 0, s32DataSize);
  s32ReadSize = fread(stSrcDateAttr.pu8Buf, 1, s32DataSize, fd);
  if (s32ReadSize != s32DataSize)
    RKADK_LOGW("u32ReadSize[%d] != u32DataSize[%d]", s32ReadSize, s32DataSize);

  stSrcDateAttr.u32BufSize = s32ReadSize;
  if (RKADK_PHOTO_GetJpgResolution(pcFileName, &stSrcDateAttr)) {
    RKADK_LOGE("get %s jpg thumb resolution failed", pcFileName);
    goto exit;
  }

  if (!pstDataAttr->u32Width || !pstDataAttr->u32Height) {
    RKADK_LOGD("use default resolution[%d, %d]", stSrcDateAttr.u32Width, stSrcDateAttr.u32Height);
    pstDataAttr->u32Width = stSrcDateAttr.u32Width;
    pstDataAttr->u32VirWidth = pstDataAttr->u32Width;
    pstDataAttr->u32Height = stSrcDateAttr.u32Height;
    pstDataAttr->u32VirHeight = pstDataAttr->u32Height;
  }

  ret = ThumbnailJpgDecode(&stSrcDateAttr, (RKADK_THUMB_ATTR_S *)pstDataAttr, &bFree);

exit:
  if (fd)
    fclose(fd);

  if (bFree)
    RKADK_PHOTO_FreeData(&stSrcDateAttr);

  if (ret)
    RKADK_PHOTO_FreeData(pstDataAttr);

  return ret;
}

RKADK_S32 RKADK_PHOTO_FreeData(RKADK_PHOTO_DATA_ATTR_S *pstDataAttr) {
  return RKADK_MEDIA_FrameFree((RKADK_FRAME_ATTR_S *)pstDataAttr);
}
