/*
 * Copyright (c) 2022 Rockchip, Inc. All Rights Reserved.
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

#include "rkadk_osd.h"
#include "rkadk_log.h"
#include "rkadk_media_comm.h"
#include "rkadk_param.h"
#include <string.h>

RKADK_S32 RKADK_OSD_Init(RKADK_U32 u32OsdId, RKADK_OSD_ATTR_S *pstOsdAttr) {
  int ret;
  RGN_ATTR_S stRgnAttr;
  RGN_HANDLE RgnHandle = 0;

  RKADK_CHECK_POINTER(pstOsdAttr, RKADK_FAILURE);

  memset(&stRgnAttr, 0, sizeof(RGN_ATTR_S));

  switch (pstOsdAttr->enOsdType) {
    case RKADK_OSD_TYPE_NORMAL:
      stRgnAttr.enType = OVERLAY_RGN;
      break;
    case RKADK_OSD_TYPE_EXTRA:
      stRgnAttr.enType = OVERLAY_EX_RGN;
#ifndef RV1106_1103
      stRgnAttr.unAttr.stOverlay.enVProcDev = VIDEO_PROC_DEV_RGA;
#endif
      break;
    default:
      RKADK_LOGE("Unsupport other type: %d", pstOsdAttr->enOsdType);
      return -1;
  }

  stRgnAttr.unAttr.stOverlay.enPixelFmt = RKADK_MEDIA_GetRkPixelFormat(pstOsdAttr->Format);
  stRgnAttr.unAttr.stOverlay.stSize.u32Width  = UPALIGNTO(pstOsdAttr->Width, 16);
  stRgnAttr.unAttr.stOverlay.stSize.u32Height = UPALIGNTO(pstOsdAttr->Height, 16);
  RgnHandle = u32OsdId;

  ret = RK_MPI_RGN_Create(RgnHandle, &stRgnAttr);
  if (RK_SUCCESS != ret) {
    RKADK_LOGE("OSD [%d] create failed with %#x!", RgnHandle, ret);
    RK_MPI_RGN_Destroy(RgnHandle);
    return ret;
  }

  return 0;
}

RKADK_S32 RKADK_OSD_Deinit(RKADK_U32 u32OsdId) {
  int ret;
  RGN_HANDLE RgnHandle = 0;
  RgnHandle = u32OsdId;

  ret = RK_MPI_RGN_Destroy(RgnHandle);
  if (RK_SUCCESS != ret) {
    RK_LOGE("OSD [%d] destroy failed with %#x", RgnHandle, ret);
    return ret;
  }

  return 0;
}

RKADK_S32 RKADK_OSD_AttachToStream(RKADK_U32 u32OsdId, RKADK_U32 u32CamId,
      RKADK_STREAM_TYPE_E enStrmType, RKADK_OSD_STREAM_ATTR_S *pstOsdStreamAttr) {
  int ret;
  RGN_HANDLE RgnHandle = 0;
  RGN_CHN_ATTR_S stRgnChnAttr;
  MPP_CHN_S stMppChn;
  RgnHandle = u32OsdId;

  RKADK_CHECK_POINTER(pstOsdStreamAttr, RKADK_FAILURE);

  RKADK_PARAM_REC_CFG_S *pstRecCfg =
      RKADK_PARAM_GetRecCfg(u32CamId);
  if (!pstRecCfg) {
    RKADK_LOGE("RKADK_PARAM_GetRecCfg failed");
    return -1;
  }

  RKADK_PARAM_PHOTO_CFG_S *pstPhotoCfg =
      RKADK_PARAM_GetPhotoCfg(u32CamId);
  if (!pstPhotoCfg) {
    RKADK_LOGE("RKADK_PARAM_GetPhotoCfg failed");
    return -1;
  }

  RKADK_PARAM_STREAM_CFG_S *pstLiveCfg =
      RKADK_PARAM_GetStreamCfg(u32CamId, RKADK_STREAM_TYPE_LIVE);
  if (!pstLiveCfg) {
    RKADK_LOGE("Live RKADK_PARAM_GetStreamCfg Live failed");
    return -1;
  }

  RKADK_PARAM_STREAM_CFG_S *pstStreamCfg = RKADK_PARAM_GetStreamCfg(
      u32CamId, RKADK_STREAM_TYPE_PREVIEW);
  if (!pstStreamCfg) {
    RKADK_LOGE("RKADK_PARAM_GetStreamCfg failed");
    return -1;
  }

  memset(&stMppChn, 0, sizeof(MPP_CHN_S));
  stMppChn.enModId = RK_ID_VENC;
  stMppChn.s32DevId = 0;

  switch (enStrmType) {
    case RKADK_STREAM_TYPE_VIDEO_MAIN:
      stMppChn.s32ChnId = pstRecCfg->attribute[0].venc_chn;
      break;
    case RKADK_STREAM_TYPE_VIDEO_SUB:
      stMppChn.s32ChnId = pstRecCfg->attribute[1].venc_chn;
      break;
    case RKADK_STREAM_TYPE_SNAP:
      stMppChn.s32ChnId = pstPhotoCfg->venc_chn;
      break;
    case RKADK_STREAM_TYPE_PREVIEW:
      stMppChn.s32ChnId = pstStreamCfg->attribute.venc_chn;
      break;
    case RKADK_STREAM_TYPE_LIVE:
      stMppChn.s32ChnId = pstLiveCfg->attribute.venc_chn;
      break;
    default:
      RKADK_LOGE("Unsuport other stream type");
      break;
  }

  memset(&stRgnChnAttr, 0, sizeof(RGN_CHN_ATTR_S));
  if (pstOsdStreamAttr->bEnableShow)
    stRgnChnAttr.bShow = RK_TRUE;
  else
    stRgnChnAttr.bShow = RK_FALSE;

  switch (pstOsdStreamAttr->enOsdType) {
    case RKADK_OSD_TYPE_NORMAL:
      stRgnChnAttr.enType = OVERLAY_RGN;
      break;
    case RKADK_OSD_TYPE_EXTRA:
      stRgnChnAttr.enType = OVERLAY_EX_RGN;
      break;
    default:
      RKADK_LOGE("Unsupport other type: %d", pstOsdStreamAttr->enOsdType);
      return -1;
  }

  stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = pstOsdStreamAttr->Origin_X;
  stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = pstOsdStreamAttr->Origin_Y;

  ret = RK_MPI_RGN_AttachToChn(RgnHandle, &stMppChn, &stRgnChnAttr);
  if (RK_SUCCESS != ret) {
    RKADK_LOGE("OSD [%d] attach to stream [%d] failed with %#x!",
                RgnHandle, stMppChn.s32ChnId, ret);
    return ret;
  }

  return 0;
}

RKADK_S32 RKADK_OSD_DettachFromStream(RKADK_U32 u32OsdId, RKADK_U32 u32CamId,
      RKADK_STREAM_TYPE_E enStrmType) {
  int ret;
  RGN_HANDLE RgnHandle = 0;
  MPP_CHN_S stMppChn;
  RgnHandle = u32OsdId;

  RKADK_PARAM_REC_CFG_S *pstRecCfg =
      RKADK_PARAM_GetRecCfg(u32CamId);
  if (!pstRecCfg) {
    RKADK_LOGE("RKADK_PARAM_GetRecCfg failed");
    return -1;
  }

  RKADK_PARAM_PHOTO_CFG_S *pstPhotoCfg =
      RKADK_PARAM_GetPhotoCfg(u32CamId);
  if (!pstPhotoCfg) {
    RKADK_LOGE("RKADK_PARAM_GetPhotoCfg failed");
    return -1;
  }

  RKADK_PARAM_STREAM_CFG_S *pstLiveCfg =
      RKADK_PARAM_GetStreamCfg(u32CamId, RKADK_STREAM_TYPE_LIVE);
  if (!pstLiveCfg) {
    RKADK_LOGE("Live RKADK_PARAM_GetStreamCfg Live failed");
    return -1;
  }

  RKADK_PARAM_STREAM_CFG_S *pstStreamCfg = RKADK_PARAM_GetStreamCfg(
      u32CamId, RKADK_STREAM_TYPE_PREVIEW);
  if (!pstStreamCfg) {
    RKADK_LOGE("RKADK_PARAM_GetStreamCfg failed");
    return -1;
  }

  memset(&stMppChn, 0, sizeof(MPP_CHN_S));
  stMppChn.enModId = RK_ID_VENC;
  stMppChn.s32DevId = 0;

  switch (enStrmType) {
    case RKADK_STREAM_TYPE_VIDEO_MAIN:
      stMppChn.s32ChnId = pstRecCfg->attribute[0].venc_chn;
      break;
    case RKADK_STREAM_TYPE_VIDEO_SUB:
      stMppChn.s32ChnId = pstRecCfg->attribute[1].venc_chn;
      break;
    case RKADK_STREAM_TYPE_SNAP:
      stMppChn.s32ChnId = pstPhotoCfg->venc_chn;
      break;
    case RKADK_STREAM_TYPE_PREVIEW:
      stMppChn.s32ChnId = pstStreamCfg->attribute.venc_chn;
      break;
    case RKADK_STREAM_TYPE_LIVE:
      stMppChn.s32ChnId = pstLiveCfg->attribute.venc_chn;
      break;
    default:
      RKADK_LOGE("Unsuport other stream type");
      break;
  }

  ret = RK_MPI_RGN_DetachFromChn(RgnHandle, &stMppChn);
  if (RK_SUCCESS != ret) {
    RKADK_LOGE("OSD [%d] detach from stream [%d] failed with %#x!",
                RgnHandle, stMppChn.s32ChnId, ret);
    return ret;
  }

  return 0;
}

RKADK_S32 RKADK_OSD_UpdateBitMap(RKADK_U32 u32OsdId, RKADK_OSD_ATTR_S *pstOsdAttr) {
  int ret;
  RGN_HANDLE RgnHandle = 0;
  RgnHandle = u32OsdId;
  BITMAP_S stBitmap;

  RKADK_CHECK_POINTER(pstOsdAttr, RKADK_FAILURE);

  memset(&stBitmap, 0, sizeof(BITMAP_S));
  stBitmap.enPixelFormat = RKADK_MEDIA_GetRkPixelFormat(pstOsdAttr->Format);
  stBitmap.u32Width = pstOsdAttr->Width;
  stBitmap.u32Height = pstOsdAttr->Height;
  stBitmap.pData = pstOsdAttr->pData;

  ret = RK_MPI_RGN_SetBitMap(RgnHandle, &stBitmap);
  if (RK_SUCCESS != ret) {
    RK_LOGE("OSD [%d] update failed with %#x!", RgnHandle, ret);
    return ret;
  }

  return 0;
}

RKADK_S32 RKADK_OSD_UpdateDisplayAttr(RKADK_U32 u32OsdId, RKADK_U32 u32CamId,
      RKADK_STREAM_TYPE_E enStrmType, RKADK_OSD_STREAM_ATTR_S *pstOsdStreamAttr) {
  int ret;
  RGN_HANDLE RgnHandle = 0;
  RGN_CHN_ATTR_S stRgnChnAttr;
  MPP_CHN_S stMppChn;
  RgnHandle = u32OsdId;

  RKADK_CHECK_POINTER(pstOsdStreamAttr, RKADK_FAILURE);

  RKADK_PARAM_REC_CFG_S *pstRecCfg =
      RKADK_PARAM_GetRecCfg(u32CamId);
  if (!pstRecCfg) {
    RKADK_LOGE("RKADK_PARAM_GetRecCfg failed");
    return -1;
  }

  RKADK_PARAM_PHOTO_CFG_S *pstPhotoCfg =
      RKADK_PARAM_GetPhotoCfg(u32CamId);
  if (!pstPhotoCfg) {
    RKADK_LOGE("RKADK_PARAM_GetPhotoCfg failed");
    return -1;
  }

  RKADK_PARAM_STREAM_CFG_S *pstLiveCfg =
      RKADK_PARAM_GetStreamCfg(u32CamId, RKADK_STREAM_TYPE_LIVE);
  if (!pstLiveCfg) {
    RKADK_LOGE("Live RKADK_PARAM_GetStreamCfg Live failed");
    return -1;
  }

  RKADK_PARAM_STREAM_CFG_S *pstStreamCfg = RKADK_PARAM_GetStreamCfg(
      u32CamId, RKADK_STREAM_TYPE_PREVIEW);
  if (!pstStreamCfg) {
    RKADK_LOGE("RKADK_PARAM_GetStreamCfg failed");
    return -1;
  }

  memset(&stMppChn, 0, sizeof(MPP_CHN_S));
  stMppChn.enModId = RK_ID_VENC;
  stMppChn.s32DevId = 0;

  switch (enStrmType) {
    case RKADK_STREAM_TYPE_VIDEO_MAIN:
      stMppChn.s32ChnId = pstRecCfg->attribute[0].venc_chn;
      break;
    case RKADK_STREAM_TYPE_VIDEO_SUB:
      stMppChn.s32ChnId = pstRecCfg->attribute[1].venc_chn;
      break;
    case RKADK_STREAM_TYPE_SNAP:
      stMppChn.s32ChnId = pstPhotoCfg->venc_chn;
      break;
    case RKADK_STREAM_TYPE_PREVIEW:
      stMppChn.s32ChnId = pstStreamCfg->attribute.venc_chn;
      break;
    case RKADK_STREAM_TYPE_LIVE:
      stMppChn.s32ChnId = pstLiveCfg->attribute.venc_chn;
      break;
    default:
      RKADK_LOGE("Unsuport other stream type");
      break;
  }

  ret = RK_MPI_RGN_GetDisplayAttr(RgnHandle, &stMppChn, &stRgnChnAttr);
  if (RK_SUCCESS != ret) {
    RKADK_LOGE("OSD [%d] get display attr from stream [%d] failed with %#x!",
                RgnHandle, stMppChn.s32ChnId, ret);
    return ret;
  }

  if (pstOsdStreamAttr->bEnableShow)
    stRgnChnAttr.bShow = RK_TRUE;
  else
    stRgnChnAttr.bShow = RK_FALSE;

  stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = pstOsdStreamAttr->Origin_X;
  stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = pstOsdStreamAttr->Origin_Y;

  ret = RK_MPI_RGN_SetDisplayAttr(RgnHandle, &stMppChn, &stRgnChnAttr);
  if (RK_SUCCESS != ret) {
    RKADK_LOGE("OSD [%d] set display attr to stream [%d] failed with %#x!",
                RgnHandle, stMppChn.s32ChnId, ret);
    return ret;
  }

  return 0;
}

RKADK_S32 RKADK_OSD_UpdateOsdSize(RKADK_U32 u32OsdId, RKADK_OSD_ATTR_S *pstOsdAttr) {
  int ret;
  RGN_ATTR_S stRgnAttr;
  RGN_HANDLE RgnHandle = 0;
  RgnHandle = u32OsdId;

  RKADK_CHECK_POINTER(pstOsdAttr, RKADK_FAILURE);

  memset(&stRgnAttr, 0, sizeof(stRgnAttr));

  ret = RK_MPI_RGN_GetAttr(RgnHandle, &stRgnAttr);
  if (RK_SUCCESS != ret) {
    RK_LOGE("OSD [%d] get attr failed with %#x!", RgnHandle, ret);
    return ret;
  }

  if (stRgnAttr.unAttr.stOverlay.stSize.u32Width  == pstOsdAttr->Width &&
      stRgnAttr.unAttr.stOverlay.stSize.u32Height == pstOsdAttr->Height) {
    RKADK_LOGI("OSD size has not change old [%d %d] new [%d, %d]!",
    stRgnAttr.unAttr.stOverlay.stSize.u32Width,
    stRgnAttr.unAttr.stOverlay.stSize.u32Height,
    pstOsdAttr->Width, pstOsdAttr->Height);
    return 0;
  }

  stRgnAttr.unAttr.stOverlay.stSize.u32Width  = pstOsdAttr->Width;
  stRgnAttr.unAttr.stOverlay.stSize.u32Height = pstOsdAttr->Height;
  ret = RK_MPI_RGN_SetAttr(RgnHandle, &stRgnAttr);
  if (RK_SUCCESS != ret) {
    RK_LOGE("OSD [%d] set attr failed with %#x!", RgnHandle, ret);
    return ret;
  }

  return 0;
}
