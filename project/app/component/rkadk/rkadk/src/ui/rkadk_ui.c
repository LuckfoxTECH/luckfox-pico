/*
 * Copyright (c) 2023 Rockchip, Inc. All Rights Reserved.
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

#include "rkadk_ui.h"
#include "rkadk_media_comm.h"
#include "rkadk_log.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
  RKADK_U32 u32VoLay;
  RKADK_U32 u32VoDev;
  RKADK_U32 u32VoChn;
} RKADK_UI_HANDLE_S;

RKADK_S32 RKADK_UI_Create(RKADK_UI_ATTR_S *pstUiAttr, RKADK_MW_PTR *ppUi) {
  int ret = 0;
  RKADK_UI_HANDLE_S *pstHandle = NULL;
  VO_PUB_ATTR_S stVoPubAttr;
  VO_VIDEO_LAYER_ATTR_S stLayerAttr;
  VO_CHN_ATTR_S stChnAttr;

  RKADK_CHECK_POINTER(pstUiAttr, RKADK_FAILURE);

  if (*ppUi) {
    RKADK_LOGE("Ui has been initialized!");
    return -1;
  }

  pstHandle = (RKADK_UI_HANDLE_S *)malloc(sizeof(RKADK_UI_HANDLE_S));
  if (!pstHandle) {
    RKADK_LOGE("malloc ui handle failed!");
    return -1;
  }

  memset(pstHandle, 0, sizeof(RKADK_UI_HANDLE_S));
  memset(&stVoPubAttr, 0, sizeof(VO_PUB_ATTR_S));
  memset(&stLayerAttr, 0, sizeof(VO_VIDEO_LAYER_ATTR_S));
  memset(&stChnAttr, 0, sizeof(VO_CHN_ATTR_S));

  stVoPubAttr.enIntfType = RKADK_MEDIA_GetRkVoIntfTpye(pstUiAttr->enUiVoIntfTye);
  stVoPubAttr.enIntfSync = VO_OUTPUT_DEFAULT;

  /* Enable Layer */
  switch (pstUiAttr->enUiVoFormat) {
    case VO_FORMAT_RGB888:
      stLayerAttr.enPixFormat = RK_FMT_RGB888;
      break;
    case VO_FORMAT_NV12:
      stLayerAttr.enPixFormat = RK_FMT_YUV420SP;
      break;
    case VO_FORMAT_RGB565:
      stLayerAttr.enPixFormat = RK_FMT_RGB565;
      break;
    case VO_FORMAT_BGR565:
      stLayerAttr.enPixFormat = RK_FMT_BGR565;
      break;
    case VO_FORMAT_RGB444:
      stLayerAttr.enPixFormat = RK_FMT_RGB444;
      break;
    default:
      RKADK_LOGW("unsupport pix format: %d, use default[%d]", stLayerAttr.enPixFormat , RK_FMT_RGB888);
      stLayerAttr.enPixFormat = RK_FMT_RGB888;
  }
  stLayerAttr.enCompressMode = COMPRESS_MODE_NONE;
  stLayerAttr.u32DispFrmRt = pstUiAttr->u32DispFrmRt;
  if (pstUiAttr->enVoSpliceMode == SPLICE_MODE_BYPASS) {
    stLayerAttr.stDispRect.s32X = pstUiAttr->u32DispX;
    stLayerAttr.stDispRect.s32Y = pstUiAttr->u32DispY;
    stLayerAttr.stDispRect.u32Width = pstUiAttr->u32DispWidth;
    stLayerAttr.stDispRect.u32Height = pstUiAttr->u32DispHeight;
  }

  stChnAttr.stRect.s32X = pstUiAttr->u32DispX;
  stChnAttr.stRect.s32Y = pstUiAttr->u32DispY;
  stChnAttr.stRect.u32Width = pstUiAttr->u32DispWidth;
  stChnAttr.stRect.u32Height = pstUiAttr->u32DispHeight;
  stChnAttr.u32FgAlpha = 255;
  stChnAttr.u32BgAlpha = 0;
  stChnAttr.enRotation = (ROTATION_E)pstUiAttr->u32Rotation;
  stChnAttr.u32Priority = 1;

  if (!pstUiAttr->bMirror && !pstUiAttr->bFlip)
      stChnAttr.enMirror = MIRROR_NONE;
  else if (pstUiAttr->bMirror && !pstUiAttr->bFlip)
      stChnAttr.enMirror = MIRROR_HORIZONTAL;
  else if (pstUiAttr->bFlip && !pstUiAttr->bMirror)
      stChnAttr.enMirror = MIRROR_VERTICAL;
  else
      stChnAttr.enMirror = MIRROR_BOTH;

  pstHandle->u32VoLay = pstUiAttr->u32VoLay;
  pstHandle->u32VoDev = pstUiAttr->u32VoDev;
  pstHandle->u32VoChn = pstUiAttr->u32VoChn;

  ret = RKADK_MPI_VO_Init(pstUiAttr->u32VoLay, pstUiAttr->u32VoDev, pstUiAttr->u32VoChn,
                          &stVoPubAttr, &stLayerAttr, &stChnAttr, pstUiAttr->enVoSpliceMode);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_Vo_Init failed, ret[%x]", ret);
    goto failed;
  }

  *ppUi = pstHandle;
  RKADK_LOGI("UI Create [layer: %d, dev: %d, chn: %d] success!",
              pstUiAttr->u32VoLay, pstUiAttr->u32VoDev, pstUiAttr->u32VoChn);
  return 0;

failed:
  RKADK_MPI_VO_DeInit(pstUiAttr->u32VoLay, pstUiAttr->u32VoDev, pstUiAttr->u32VoChn);

  if (pstHandle)
    free(pstHandle);

  RKADK_LOGE("UI Create [layer: %d, dev: %d, chn: %d] failed!",
              pstUiAttr->u32VoLay, pstUiAttr->u32VoDev, pstUiAttr->u32VoChn);
  return ret;
}

RKADK_S32 RKADK_UI_Destroy(RKADK_MW_PTR pUi) {
  int ret;
  RKADK_UI_HANDLE_S *pstHandle = NULL;

  RKADK_CHECK_POINTER(pUi, RKADK_FAILURE);
  pstHandle = (RKADK_UI_HANDLE_S *)pUi;

  ret = RKADK_MPI_VO_DeInit(pstHandle->u32VoLay, pstHandle->u32VoDev, pstHandle->u32VoChn);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_Vo_DeInit failed, ret[%x]", ret);
    goto failed;
  }

  RKADK_LOGI("UI Destory [layer: %d, dev: %d, chn: %d] success!",
              pstHandle->u32VoLay, pstHandle->u32VoDev, pstHandle->u32VoChn);
  free(pstHandle);
  pstHandle = NULL;
  return 0;

failed:
  RKADK_LOGE("UI Destory [layer: %d, dev: %d, chn: %d] failed!",
              pstHandle->u32VoLay, pstHandle->u32VoDev, pstHandle->u32VoChn);
  free(pstHandle);
  pstHandle = NULL;
  return ret;
}

RKADK_S32 RKADK_UI_Update(RKADK_MW_PTR pUi, RKADK_UI_FRAME_INFO *pstUiFrameInfo) {
  int ret;
  RKADK_UI_HANDLE_S *pstHandle = NULL;
  VIDEO_FRAME_INFO_S stVoVFrame;

  RKADK_CHECK_POINTER(pUi, RKADK_FAILURE);
  RKADK_CHECK_POINTER(pstUiFrameInfo, RKADK_FAILURE);

  pstHandle = (RKADK_UI_HANDLE_S *)pUi;
  memset(&stVoVFrame, 0, sizeof(VIDEO_FRAME_INFO_S));

  stVoVFrame.stVFrame.u32Width = pstUiFrameInfo->u32Width;
  stVoVFrame.stVFrame.u32Height = pstUiFrameInfo->u32Height;
  stVoVFrame.stVFrame.u32VirWidth = pstUiFrameInfo->u32Width;
  stVoVFrame.stVFrame.u32VirHeight = pstUiFrameInfo->u32Height;
  stVoVFrame.stVFrame.enPixelFormat = RKADK_MEDIA_GetRkPixelFormat(pstUiFrameInfo->Format);
  stVoVFrame.stVFrame.pMbBlk = pstUiFrameInfo->pMblk;

  ret = RK_MPI_VO_SendFrame(pstHandle->u32VoLay, pstHandle->u32VoChn, &stVoVFrame, 1000);
  if(ret) {
    RKADK_LOGE("RK_MPI_VO_SendFrame timeout: ret[%x]", ret);
    return ret;
  }

  return 0;
}