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

#include "rkadk_disp.h"
#include "rkadk_log.h"
#include "rkadk_param.h"
#include "rkadk_media_comm.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct {
  bool bInit;
  RKADK_U32 u32CamId;
  bool bSendBuffer;
  pthread_t tid; //send frame to vo thread
} RKADK_DISP_HANDLE_S;

static RKADK_DISP_HANDLE_S stDispHandle = {
    .bInit = false, .u32CamId = 0, .bSendBuffer = false, .tid= 0};

static int RKADK_DISP_CreateVo(RKADK_U32 VoLayer, RKADK_U32 VoDev,
                               RKADK_PARAM_DISP_CFG_S *pstDispCfg) {
  int ret = RK_SUCCESS;
  RKADK_VO_SPLICE_MODE_E enVoSpliceMode;
  VO_PUB_ATTR_S            stVoPubAttr;
  VO_VIDEO_LAYER_ATTR_S    stLayerAttr;
  VO_CHN_ATTR_S            stChnAttr;
  COMPRESS_MODE_E enCompressMode = COMPRESS_MODE_NONE;

  memset(&stVoPubAttr, 0, sizeof(VO_PUB_ATTR_S));
  memset(&stLayerAttr, 0, sizeof(VO_VIDEO_LAYER_ATTR_S));
  memset(&stChnAttr, 0, sizeof(VO_CHN_ATTR_S));

  stVoPubAttr.enIntfType = RKADK_MEDIA_GetRkVoIntfTpye(RKADK_PARAM_GetIntfType(pstDispCfg->intf_type));
  stVoPubAttr.enIntfSync = VO_OUTPUT_DEFAULT;

  /* Enable Layer */
  enVoSpliceMode = RKADK_PARAM_GetSpliceMode(pstDispCfg->splice_mode);
  stLayerAttr.enPixFormat      = RKADK_PARAM_GetPixFmt(pstDispCfg->img_type, &enCompressMode);
  stLayerAttr.enCompressMode   = enCompressMode;
  stLayerAttr.u32DispFrmRt     = pstDispCfg->frame_rate;
  if (enVoSpliceMode == SPLICE_MODE_BYPASS) {
    stLayerAttr.stDispRect.s32X = pstDispCfg->x;
    stLayerAttr.stDispRect.s32Y = pstDispCfg->y;
    stLayerAttr.stDispRect.u32Width = pstDispCfg->width;
    stLayerAttr.stDispRect.u32Height = pstDispCfg->height;
  }

  stChnAttr.stRect.s32X = pstDispCfg->x;
  stChnAttr.stRect.s32Y = pstDispCfg->y;
  stChnAttr.stRect.u32Width = pstDispCfg->width;
  stChnAttr.stRect.u32Height = pstDispCfg->height;
  stChnAttr.u32FgAlpha = 255;
  stChnAttr.u32BgAlpha = 0;
  stChnAttr.enMirror = MIRROR_NONE;
  stChnAttr.enRotation = (ROTATION_E)pstDispCfg->rotation;
  stChnAttr.u32Priority = 1;

  ret = RKADK_MPI_VO_Init(VoLayer, VoDev, pstDispCfg->vo_chn,
                          &stVoPubAttr, &stLayerAttr, &stChnAttr, enVoSpliceMode);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_Vo_Init failed, ret = %x", ret);
    return ret;
  }

  RKADK_LOGI("Create vo [dev: %d, layer: %d, chn: %d] success!",
              VoDev, VoLayer, pstDispCfg->vo_chn);
  return ret;
}

static int RKADK_DISP_DestroyVo(RKADK_PARAM_DISP_CFG_S *pstDispCfg) {
  int ret = 0;

  ret = RKADK_MPI_VO_DeInit(pstDispCfg->vo_layer, pstDispCfg->vo_device, pstDispCfg->vo_chn);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_Vo_DeInit failed, ret = %x", ret);
    return ret;
  }

  RKADK_LOGI("Destroy vo [dev: %d, layer: %d, chn: %d] success!",
            pstDispCfg->vo_device, pstDispCfg->vo_layer, pstDispCfg->vo_chn);
  return ret;
}

static void RKADK_DISP_SetChn(RKADK_U32 u32CamId,
                              RKADK_PARAM_DISP_CFG_S *pstDispCfg,
                              MPP_CHN_S *pstViChn, MPP_CHN_S *pstVoChn,
                              MPP_CHN_S *pstSrcVpssChn, MPP_CHN_S *pstDstVpssChn) {
  pstViChn->enModId = RK_ID_VI;
  pstViChn->s32DevId = u32CamId;
  pstViChn->s32ChnId = pstDispCfg->vi_attr.u32ViChn;

  pstSrcVpssChn->enModId = RK_ID_VPSS;
  pstSrcVpssChn->s32DevId = pstDispCfg->vpss_grp;
  pstSrcVpssChn->s32ChnId = pstDispCfg->vpss_chn;

  pstDstVpssChn->enModId = RK_ID_VPSS;
  pstDstVpssChn->s32DevId = pstDispCfg->vpss_grp;
  pstDstVpssChn->s32ChnId = 0; //When vpss is dst, chn is equal to 0

  pstVoChn->enModId = RK_ID_VO;
  pstVoChn->s32DevId = pstDispCfg->vo_device;
  pstVoChn->s32ChnId = pstDispCfg->vo_chn;
}

static RKADK_S32 RKADK_DISP_Enable(RKADK_U32 u32CamId, RKADK_PARAM_DISP_CFG_S *pstDispCfg,
                                   RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg) {
  int ret = 0;
  VPSS_GRP_ATTR_S          stGrpAttr;
  VPSS_CHN_ATTR_S          stChnAttr;

  // Create VI
  ret = RKADK_MPI_VI_Init(u32CamId, pstDispCfg->vi_attr.u32ViChn,
                          &(pstDispCfg->vi_attr.stChnAttr));
  if (ret) {
    RKADK_LOGE("RKADK_MPI_VI_Init faled %d", ret);
    return ret;
  }
  RKADK_BUFINFO("create vi[%d] end", pstDispCfg->vi_attr.u32ViChn);

  // Create VPSS
  memset(&stGrpAttr, 0, sizeof(VPSS_GRP_ATTR_S));
  memset(&stChnAttr, 0, sizeof(VPSS_CHN_ATTR_S));

  stGrpAttr.u32MaxW = pstSensorCfg->max_width;
  stGrpAttr.u32MaxH = pstSensorCfg->max_height;
  stGrpAttr.enPixelFormat = pstDispCfg->vi_attr.stChnAttr.enPixelFormat;
  stGrpAttr.enCompressMode = COMPRESS_MODE_NONE;
  stGrpAttr.stFrameRate.s32SrcFrameRate = -1;
  stGrpAttr.stFrameRate.s32DstFrameRate = -1;
  stChnAttr.enChnMode = VPSS_CHN_MODE_USER;
  stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
  stChnAttr.enDynamicRange = DYNAMIC_RANGE_SDR8;
  stChnAttr.enPixelFormat = pstDispCfg->vi_attr.stChnAttr.enPixelFormat;
  stChnAttr.stFrameRate.s32SrcFrameRate = -1;
  stChnAttr.stFrameRate.s32DstFrameRate = -1;
  stChnAttr.u32Width = pstDispCfg->vi_attr.stChnAttr.stSize.u32Width;
  stChnAttr.u32Height = pstDispCfg->vi_attr.stChnAttr.stSize.u32Height;
  stChnAttr.u32FrameBufCnt = pstDispCfg->vi_attr.stChnAttr.stIspOpt.u32BufCount + 2;
  if (!pstSensorCfg->used_isp) {
    stChnAttr.bMirror = (RK_BOOL)pstSensorCfg->mirror;
    stChnAttr.bFlip = (RK_BOOL)pstSensorCfg->flip;
  }

#ifdef RV1106_1103
  //unbind mode
  stChnAttr.u32Depth = 3;
#else
  stChnAttr.u32Depth = 0;
#endif

  ret = RKADK_MPI_VPSS_Init(pstDispCfg->vpss_grp, pstDispCfg->vpss_chn,
                            &stGrpAttr, &stChnAttr);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_VPSS_Init vpss falied[%d]",ret);
    return ret;
  }

  RKADK_BUFINFO("create vpss[%d, %d] end", pstDispCfg->vpss_grp, pstDispCfg->vpss_chn);

  // Create VO
  ret = RKADK_DISP_CreateVo(pstDispCfg->vo_layer, pstDispCfg->vo_device, pstDispCfg);
  if (ret) {
    RKADK_LOGE("Create vo [dev: %d, layer: %d, chn: %d] failed, ret = %x",
                pstDispCfg->vo_device, pstDispCfg->vo_layer, pstDispCfg->vo_chn, ret);
    return ret;
  }
  RKADK_BUFINFO("create vo[%d] end", pstDispCfg->vo_chn);

  return 0;
}

static bool RKADK_DISP_CheckParam(RKADK_PARAM_DISP_CFG_S *pstDispCfg) {
  if (pstDispCfg->x < 0 || pstDispCfg->y < 0 || pstDispCfg->width <= 0 ||
      pstDispCfg->height <= 0) {
    RKADK_LOGE("Display rect erro [x: %d, y: %d, width: %d, height: %d]",
              pstDispCfg->x, pstDispCfg->y, pstDispCfg->width, pstDispCfg->height);
    return false;
  }

  return true;
}

static bool RKADK_DISP_CheckRect(RKADK_RECT_S stRect,
                                 RKADK_PARAM_DISP_CFG_S *pstDispCfg, bool bVpss) {
  if (stRect.u32X < 0 || stRect.u32Y < 0 || stRect.u32Width <= 0 ||
      stRect.u32Height <= 0) {
    RKADK_LOGE("Rect erro [x: %d, y: %d, width: %d, height: %d]",
          stRect.u32X, stRect.u32Y, stRect.u32Width, stRect.u32Height);
    return false;
  }

  if (bVpss) {
    if (stRect.u32X + stRect.u32Width > pstDispCfg->vi_attr.stChnAttr.stSize.u32Width) {
      RKADK_LOGE("Vpss crop rect x: %d + width: %d > input width: %d",
                stRect.u32X, stRect.u32Width,
                pstDispCfg->vi_attr.stChnAttr.stSize.u32Width);
      return false;
    }

    if (stRect.u32Y + stRect.u32Y > pstDispCfg->vi_attr.stChnAttr.stSize.u32Height) {
      RKADK_LOGE("Vpss crop rect y: %d + height: %d > input height: %d",
                stRect.u32Y , stRect.u32Y,
                pstDispCfg->vi_attr.stChnAttr.stSize.u32Height);
      return false;
    }
  }

  return true;
}

static void *RKADK_DISP_GetVpssMb(void *arg) {
  int s32Ret;
  VIDEO_FRAME_INFO_S stVideoFrame;

  RKADK_DISP_HANDLE_S *pstDispHandle = (RKADK_DISP_HANDLE_S *)arg;
  if (!pstDispHandle) {
    RKADK_LOGE("Get VPSS MB thread invalid param");
    return NULL;
  }

  RKADK_PARAM_DISP_CFG_S *pstDispCfg = RKADK_PARAM_GetDispCfg(pstDispHandle->u32CamId);
  if (!pstDispCfg) {
    RKADK_LOGE("RKADK_PARAM_GetDispCfg[%d] failed", pstDispHandle->u32CamId);
    return NULL;
  }

  while (pstDispHandle->bSendBuffer) {
    s32Ret = RK_MPI_VPSS_GetChnFrame(pstDispCfg->vpss_grp, pstDispCfg->vpss_chn,
                                      &stVideoFrame, 1000);
    if (s32Ret == RK_SUCCESS) {
      s32Ret = RK_MPI_VO_SendFrame(pstDispCfg->vo_layer, pstDispCfg->vo_chn, &stVideoFrame, -1);
      if (s32Ret != RK_SUCCESS)
        RKADK_LOGE("RK_MPI_VO_SendFrame failed[%x]", s32Ret);

      s32Ret = RK_MPI_VPSS_ReleaseChnFrame(pstDispCfg->vpss_grp, pstDispCfg->vpss_chn, &stVideoFrame);
      if (s32Ret != RK_SUCCESS)
        RKADK_LOGE("RK_MPI_VPSS_ReleaseChnFrame failed[%x]", s32Ret);
    } else {
      RKADK_LOGE("RK_MPI_VPSS_GetChnFrame[%d, %d] timeout[%x]",
                pstDispCfg->vpss_grp, pstDispCfg->vpss_chn, s32Ret);
    }
  }

  RKADK_LOGD("Exit!");
  return NULL;
}

RKADK_S32 RKADK_DISP_Init(RKADK_U32 u32CamId) {
  int ret = 0;
  bool bSysInit = false;
  MPP_CHN_S stViChn, stVoChn, stSrcVpssChn, stDstVpssChn;

  RKADK_CHECK_CAMERAID(u32CamId, RKADK_FAILURE);

  if (stDispHandle.bInit) {
    RKADK_LOGI("Disp u32CamId[%d] has been initialized", u32CamId);
    return 0;
  }

  RKADK_LOGI("Disp u32CamId[%d] Init Start...", u32CamId);

  bSysInit = RKADK_MPI_SYS_CHECK();
  if (!bSysInit) {
    RKADK_LOGE("System is not initialized");
    return -1;
  }

  RKADK_PARAM_DISP_CFG_S *pstDispCfg = RKADK_PARAM_GetDispCfg(u32CamId);
  if (!pstDispCfg) {
    RKADK_LOGE("RKADK_PARAM_GetDispCfg[%d] failed", u32CamId);
    return -1;
  }

  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg = RKADK_PARAM_GetSensorCfg(u32CamId);
  if (!pstSensorCfg) {
    RKADK_LOGE("RKADK_PARAM_GetSensorCfg failed");
    return -1;
  }

  if (!RKADK_DISP_CheckParam(pstDispCfg))
    return -1;

  RKADK_DISP_SetChn(u32CamId, pstDispCfg, &stViChn, &stVoChn,
                    &stSrcVpssChn, &stDstVpssChn);
  ret = RKADK_DISP_Enable(u32CamId, pstDispCfg, pstSensorCfg);
  if (ret) {
    RKADK_LOGE("RKADK_DISP_Enable failed(%d)", ret);
    goto failed;
  }

  stDispHandle.u32CamId = u32CamId;
#ifdef RV1106_1103
  char name[RKADK_THREAD_NAME_LEN];
  stDispHandle.bSendBuffer = true;
  ret = pthread_create(&stDispHandle.tid, NULL,
                       RKADK_DISP_GetVpssMb, &stDispHandle);
  if (ret) {
    RKADK_LOGE("Create get vpss mb(%d) thread failed %d", stSrcVpssChn.s32ChnId, ret);
    goto failed;
  }
  snprintf(name, sizeof(name), "GetVpssMb_%d", stSrcVpssChn.s32ChnId);
  pthread_setname_np(stDispHandle.tid, name);
#else
  // Bind VPSS to VO
  ret = RKADK_MPI_SYS_Bind(&stSrcVpssChn, &stVoChn);
  if (ret) {
    RKADK_LOGE("Bind VPSS[%d] to VENC[%d] failed[%x]", stSrcVpssChn.s32ChnId,
                stVoChn.s32ChnId, ret);
    goto failed;
  }
#endif

  // VI Bind VPSS
  ret = RKADK_MPI_SYS_Bind(&stViChn, &stDstVpssChn);
  if (ret) {
    RKADK_LOGE("Bind VI[%d] to VPSS[%d] failed[%x]", stViChn.s32ChnId,
                stDstVpssChn.s32DevId, ret);
    goto failed;
  }

  stDispHandle.bInit = true;
  RKADK_LOGI("Disp u32CamId[%d] Init End...", u32CamId);
  return 0;

failed:
  RKADK_LOGI("Disp u32CamId[%d] Init failed...", u32CamId);
  RKADK_MPI_SYS_UnBind(&stSrcVpssChn, &stVoChn);

  RKADK_MPI_SYS_UnBind(&stViChn, &stDstVpssChn);

  RKADK_DISP_DestroyVo(pstDispCfg);

  RKADK_MPI_VPSS_DeInit(pstDispCfg->vpss_grp, pstDispCfg->vpss_chn);

  RKADK_MPI_VI_DeInit(u32CamId, stViChn.s32ChnId);
  return ret;
}

RKADK_S32 RKADK_DISP_DeInit(RKADK_U32 u32CamId) {
  int ret = 0;
  MPP_CHN_S stViChn, stVoChn, stSrcVpssChn, stDstVpssChn;

  RKADK_CHECK_CAMERAID(u32CamId, RKADK_FAILURE);

  if (!stDispHandle.bInit || stDispHandle.u32CamId != u32CamId) {
    RKADK_LOGI("Disp u32CamId[%d] not initialized", u32CamId);
    return -1;
  }

  RKADK_LOGI("Disp u32CamId[%d] DeInit Start...", u32CamId);

  RKADK_PARAM_DISP_CFG_S *pstDispCfg = RKADK_PARAM_GetDispCfg(u32CamId);
  if (!pstDispCfg) {
    RKADK_LOGE("Live RKADK_PARAM_GetDispCfg failed");
    return -1;
  }

  RKADK_DISP_SetChn(u32CamId, pstDispCfg, &stViChn,
                    &stVoChn, &stSrcVpssChn, &stDstVpssChn);

#ifdef RV1106_1103
  stDispHandle.bSendBuffer = false;
  if (stDispHandle.tid) {
    RKADK_LOGD("Request to cancel get mb thread...");
    ret = pthread_join(stDispHandle.tid, NULL);
    if (ret)
      RKADK_LOGE("Exit get vpss[%d] mb thread failed!", stSrcVpssChn.s32ChnId);
    else
      RKADK_LOGI("Exit get vpss[%d] mb thread ok", stSrcVpssChn.s32ChnId);
    stDispHandle.tid = 0;
  }
#else
  // VPSS UnBind VO
  ret = RKADK_MPI_SYS_UnBind(&stSrcVpssChn, &stVoChn);
  if (ret) {
      RKADK_LOGE("UnBind VPSS[%d] to VO[%d] failed[%x]", stSrcVpssChn.s32ChnId,
                 stVoChn.s32ChnId, ret);
    return ret;
  }
#endif

  // VI UnBind VPSS
  ret = RKADK_MPI_SYS_UnBind(&stViChn, &stDstVpssChn);
  if (ret) {
    RKADK_LOGE("UnBind VI[%d] to VPSS[%d] failed[%x]", stViChn.s32ChnId,
                stDstVpssChn.s32DevId, ret);
    return ret;
  }

  ret = RKADK_DISP_DestroyVo(pstDispCfg);
  if (ret) {
    RKADK_LOGE("Destory VO[%d] failed(%d)", stVoChn.s32ChnId, ret);
    return ret;
  }

  ret = RKADK_MPI_VPSS_DeInit(pstDispCfg->vpss_grp, pstDispCfg->vpss_chn);
  if (ret) {
    RKADK_LOGE("DeInit VPSS[%d] failed[%x]", pstDispCfg->vpss_chn, ret);
    return ret;
  }

  ret = RKADK_MPI_VI_DeInit(u32CamId, stViChn.s32ChnId);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_VI_DeInit failed(%d)", ret);
    return ret;
  }

  stDispHandle.bInit = false;
  RKADK_LOGI("Disp u32CamId[%d] DeInit End...", u32CamId);
  return 0;
}

RKADK_S32 RKADK_DISP_SetAttr(RKADK_U32 u32CamId, RKADK_DISP_ATTR_S *pstAttr) {
  int ret;
  VO_CHN_ATTR_S stVoChnAttr;
  VPSS_CHN_ATTR_S stVpssChnAttr;
  VPSS_CROP_INFO_S stVpssCropInfo;

  RKADK_CHECK_CAMERAID(u32CamId, RKADK_FAILURE);
  RKADK_CHECK_POINTER(pstAttr, RKADK_FAILURE);

  RKADK_PARAM_DISP_CFG_S *pstDispCfg = RKADK_PARAM_GetDispCfg(u32CamId);
  if (!pstDispCfg) {
    RKADK_LOGE("RKADK_PARAM_GetDispCfg[%d] failed", u32CamId);
    return -1;
  }

  if ((!RKADK_DISP_CheckRect(pstAttr->stVpssCropRect, pstDispCfg, true) ||
      !RKADK_DISP_CheckRect(pstAttr->stVoRect, pstDispCfg, false)))
    return -1;

  ret = RK_MPI_VO_PauseChn(pstDispCfg->vo_layer, pstDispCfg->vo_chn);
  if (ret) {
    RKADK_LOGE("Pause vo [layer: %d, chn: %d] failed!, ret: %x",
                pstDispCfg->vo_layer, pstDispCfg->vo_chn, ret);
    return ret;
  }

  ret = RK_MPI_VPSS_GetChnCrop(pstDispCfg->vpss_grp, pstDispCfg->vpss_chn, &stVpssCropInfo);
  if (ret != RK_SUCCESS) {
      RKADK_LOGE("Get Vpss[%d, %d] Crop attr failed with %#x!",
              pstDispCfg->vpss_grp, pstDispCfg->vpss_chn, ret);
      return ret;
  }

  stVpssCropInfo.bEnable = RK_TRUE;
  stVpssCropInfo.enCropCoordinate = VPSS_CROP_ABS_COOR;
  stVpssCropInfo.stCropRect.s32X = pstAttr->stVpssCropRect.u32X;
  stVpssCropInfo.stCropRect.s32Y = pstAttr->stVpssCropRect.u32Y;
  stVpssCropInfo.stCropRect.u32Width = pstAttr->stVpssCropRect.u32Width;
  stVpssCropInfo.stCropRect.u32Height = pstAttr->stVpssCropRect.u32Height;
  ret = RK_MPI_VPSS_SetChnCrop(pstDispCfg->vpss_grp, pstDispCfg->vpss_chn, &stVpssCropInfo);
  if (ret != RK_SUCCESS) {
    RKADK_LOGE("Set Vpss[%d, %d] Crop attr failed with %#x!",
            pstDispCfg->vpss_grp, pstDispCfg->vpss_chn, ret);
    return ret;
  }

  ret = RK_MPI_VPSS_GetChnAttr(pstDispCfg->vpss_grp, pstDispCfg->vpss_chn, &stVpssChnAttr);
  if (ret != RK_SUCCESS) {
      RKADK_LOGE("Get Vpss[%d, %d] attr attr failed with %#x!",
              pstDispCfg->vpss_grp, pstDispCfg->vpss_chn, ret);
      return ret;
  }

  stVpssChnAttr.u32Width = pstAttr->stVpssCropRect.u32Width;
  stVpssChnAttr.u32Height = pstAttr->stVpssCropRect.u32Height;

  ret = RK_MPI_VPSS_SetChnAttr(pstDispCfg->vpss_grp, pstDispCfg->vpss_chn, &stVpssChnAttr);
  if (ret != RK_SUCCESS) {
    RKADK_LOGE("Set Vpss[%d, %d] attr attr failed with %#x!",
            pstDispCfg->vpss_grp, pstDispCfg->vpss_chn, ret);
    return ret;
  }

  ret = RK_MPI_VO_RefreshChn(pstDispCfg->vo_layer, pstDispCfg->vo_chn);
  if (ret) {
    RKADK_LOGE("Refresh vo [layer: %d, chn: %d] failed!, ret: %x",
                pstDispCfg->vo_layer, pstDispCfg->vo_chn, ret);
    return ret;
  }

  ret = RK_MPI_VO_GetChnAttr(pstDispCfg->vo_layer, pstDispCfg->vo_chn, &stVoChnAttr);
  if (ret) {
    RKADK_LOGE("Display get vo chn: %d attr failed!, ret: %d",
              pstDispCfg->vo_chn, ret);
    return -1;
  }

  stVoChnAttr.stRect.s32X = pstAttr->stVoRect.u32X;
  stVoChnAttr.stRect.s32Y = pstAttr->stVoRect.u32Y;
  stVoChnAttr.stRect.u32Width = pstAttr->stVoRect.u32Width;
  stVoChnAttr.stRect.u32Height = pstAttr->stVoRect.u32Height;

  ret = RK_MPI_VO_SetChnAttr(pstDispCfg->vo_layer, pstDispCfg->vo_chn, &stVoChnAttr);
  if (ret) {
    RKADK_LOGE("Display set vo chn: %d attr failed!, ret: %d",
              pstDispCfg->vo_chn, ret);
    return -1;
  }

  ret = RK_MPI_VO_ResumeChn(pstDispCfg->vo_layer, pstDispCfg->vo_chn);
  if (ret) {
    RKADK_LOGE("Resume vo [layer: %d, chn: %d] failed!, ret: %x",
                pstDispCfg->vo_layer, pstDispCfg->vo_chn, ret);
    return ret;
  }

  RKADK_LOGI("Set attr vpss rect: [%d, %d, %d, %d], vo rect: [%d, %d, %d, %d]",
              pstAttr->stVpssCropRect.u32X, pstAttr->stVpssCropRect.u32Y,
              pstAttr->stVpssCropRect.u32Width, pstAttr->stVpssCropRect.u32Height,
              pstAttr->stVpssCropRect.u32X, pstAttr->stVpssCropRect.u32Y,
              pstAttr->stVpssCropRect.u32Width, pstAttr->stVpssCropRect.u32Height);

  return 0;
}
