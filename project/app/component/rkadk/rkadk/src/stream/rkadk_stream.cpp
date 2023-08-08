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

#include "rkadk_stream.h"
#include "rkadk_log.h"
#include "rkadk_media_comm.h"
#include "rkadk_param.h"
#include "rkadk_audio_encoder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  bool start;
  bool bVencChnMux;
  bool bRequestIDR;
  bool bWaitIDR;
  bool bFirstKeyFrame;
  RKADK_U32 u32CamId;
  RKADK_U32 videoSeq;
  RKADK_CODEC_TYPE_E enCodecType;
  RKADK_VENC_DATA_PROC_FUNC pfnVencDataCB;
} STREAM_VIDEO_HANDLE_S;

typedef struct {
  bool start;
  bool bGetBuffer;
  pthread_t pcmTid;
  RKADK_U32 audioSeq;
  RKADK_U32 pcmSeq;
  MPP_CHN_S stAiChn;
  MPP_CHN_S stAencChn;
  RKADK_CODEC_TYPE_E enCodecType;
  RKADK_AUDIO_DATA_PROC_FUNC pfnPcmDataCB;
  RKADK_AUDIO_DATA_PROC_FUNC pfnAencDataCB;
} STREAM_AUDIO_HANDLE_S;

static int RKADK_STREAM_CheckCodecType(RKADK_CODEC_TYPE_E enCodecType) {
  if (enCodecType < RKADK_CODEC_TYPE_G711A) {
    RKADK_LOGE("invalid audio enCodecType = %d", enCodecType);
    return -1;
  }

  return 0;
}

/**************************************************/
/*                     Video API                  */
/**************************************************/
static RKADK_S32 RKADK_STREAM_RequestIDR(RKADK_U32 u32CamId,
                                         RKADK_U32 u32ChnId) {
  int ret = 0;
  RKADK_STREAM_TYPE_E enType = RKADK_PARAM_VencChnMux(u32CamId, u32ChnId);

  if (enType == RKADK_STREAM_TYPE_VIDEO_MAIN ||
      enType == RKADK_STREAM_TYPE_VIDEO_SUB) {
    int i;
    RKADK_PARAM_REC_CFG_S *pstRecCfg = RKADK_PARAM_GetRecCfg(u32CamId);
    if (!pstRecCfg) {
      RKADK_LOGE("RKADK_PARAM_GetRecCfg failed");
      return false;
    }

    for (i = 0; i < (int)pstRecCfg->file_num; i++) {
      RKADK_LOGI("chn mux, venc_chn[%d] request IDR",
                 pstRecCfg->attribute[i].venc_chn);
      ret |= RK_MPI_VENC_RequestIDR(pstRecCfg->attribute[i].venc_chn, RK_FALSE);
    }
  } else {
    ret = RK_MPI_VENC_RequestIDR(u32ChnId, RK_FALSE);
  }

  return ret;
}

static void RKADK_STREAM_VencOutCb(RKADK_MEDIA_VENC_DATA_S stData,
                                   RKADK_VOID *pHandle) {
  RKADK_VIDEO_STREAM_S vStreamData;

  STREAM_VIDEO_HANDLE_S *pstHandle = (STREAM_VIDEO_HANDLE_S *)pHandle;
  if (!pstHandle) {
    RKADK_LOGE("Get venc data thread invalid param");
    return;
  }

  RKADK_PARAM_STREAM_CFG_S *pstStreamCfg =
      RKADK_PARAM_GetStreamCfg(pstHandle->u32CamId, RKADK_STREAM_TYPE_PREVIEW);
  if (!pstStreamCfg) {
    RKADK_LOGE("RKADK_PARAM_GetStreamCfg failed");
    return;
  }

  if (!pstHandle->bWaitIDR) {
    if (!RKADK_MEDIA_CheckIdrFrame(pstStreamCfg->attribute.codec_type,
                                   stData.stFrame.pstPack->DataType)) {
      if (!pstHandle->bRequestIDR) {
        RKADK_LOGD("requst idr frame");
        RKADK_STREAM_RequestIDR(pstHandle->u32CamId,
                                pstStreamCfg->attribute.venc_chn);
        pstHandle->bRequestIDR = true;
      } else {
        RKADK_LOGD("wait first idr frame");
      }

      return;
    }

    pstHandle->bWaitIDR = true;
    if (pstHandle->bFirstKeyFrame) {
      RKADK_KLOG("Stream first key frame pts: %lld", stData.stFrame.pstPack->u64PTS);
      pstHandle->bFirstKeyFrame = false;
    }
  }

  memset(&vStreamData, 0, sizeof(RKADK_VIDEO_STREAM_S));
  vStreamData.bEndOfStream = (RKADK_BOOL)stData.stFrame.pstPack->bStreamEnd;
  vStreamData.u32CamId = pstHandle->u32CamId;
  vStreamData.u32Seq = pstHandle->videoSeq;
  vStreamData.astPack.apu8Addr =
      (RKADK_U8 *)RK_MPI_MB_Handle2VirAddr(stData.stFrame.pstPack->pMbBlk);
  vStreamData.astPack.au32Len = stData.stFrame.pstPack->u32Len;
  vStreamData.astPack.u64PTS = stData.stFrame.pstPack->u64PTS;
  vStreamData.astPack.stDataType.enPayloadType =
      pstStreamCfg->attribute.codec_type;

  if (pstStreamCfg->attribute.codec_type == RKADK_CODEC_TYPE_H264)
    vStreamData.astPack.stDataType.enH264EType =
        stData.stFrame.pstPack->DataType.enH264EType;
  else if (pstStreamCfg->attribute.codec_type == RKADK_CODEC_TYPE_H265)
    vStreamData.astPack.stDataType.enH265EType =
        stData.stFrame.pstPack->DataType.enH265EType;
  else
    RKADK_LOGE("Unsupported codec type: %d",
               pstStreamCfg->attribute.codec_type);

  if (pstHandle->pfnVencDataCB && pstHandle->start)
    pstHandle->pfnVencDataCB(&vStreamData);

  pstHandle->videoSeq++;
}

static void RKADK_STREAM_SetVideoChn(RKADK_PARAM_STREAM_CFG_S *pstStreamCfg,
                                     RKADK_U32 u32CamId, MPP_CHN_S *pstViChn,
                                     MPP_CHN_S *pstVencChn,
                                     MPP_CHN_S *pstSrcVpssChn, MPP_CHN_S *pstDstVpssChn) {
  pstViChn->enModId = RK_ID_VI;
  pstViChn->s32DevId = u32CamId;
  pstViChn->s32ChnId = pstStreamCfg->vi_attr.u32ViChn;

  pstSrcVpssChn->enModId = RK_ID_VPSS;
  pstSrcVpssChn->s32DevId = pstStreamCfg->attribute.vpss_grp;
  pstSrcVpssChn->s32ChnId = pstStreamCfg->attribute.vpss_chn;

  pstDstVpssChn->enModId = RK_ID_VPSS;
  pstDstVpssChn->s32DevId = pstStreamCfg->attribute.vpss_grp;
  pstDstVpssChn->s32ChnId = 0; //When vpss is dst, chn is equal to 0

  pstVencChn->enModId = RK_ID_VENC;
  pstVencChn->s32DevId = 0;
  pstVencChn->s32ChnId = pstStreamCfg->attribute.venc_chn;
}

static bool RKADK_STREAM_IsUseVpss(RKADK_PARAM_STREAM_CFG_S *pstStreamCfg) {
  RKADK_U32 u32SrcWidth = pstStreamCfg->vi_attr.stChnAttr.stSize.u32Width;
  RKADK_U32 u32SrcHeight = pstStreamCfg->vi_attr.stChnAttr.stSize.u32Height;
  RKADK_U32 u32DstWidth = pstStreamCfg->attribute.width;
  RKADK_U32 u32DstHeight = pstStreamCfg->attribute.height;

  if (u32DstWidth == u32SrcWidth && u32DstHeight == u32SrcHeight) {
    return false;
  } else {
    RKADK_LOGD("In[%d, %d], Out[%d, %d]", u32SrcWidth, u32SrcHeight,
               u32DstWidth, u32DstHeight);
    return true;
  }
}

static int RKADK_STREAM_SetVencAttr(RKADK_U32 u32CamId,
                                    RKADK_PARAM_STREAM_CFG_S *pstStreamCfg,
                                    VENC_CHN_ATTR_S *pstVencAttr) {
  int ret;
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg = NULL;

  RKADK_CHECK_POINTER(pstVencAttr, RKADK_FAILURE);
  memset(pstVencAttr, 0, sizeof(VENC_CHN_ATTR_S));

  pstSensorCfg = RKADK_PARAM_GetSensorCfg(u32CamId);
  if (!pstSensorCfg) {
    RKADK_LOGE("RKADK_PARAM_GetSensorCfg failed");
    return -1;
  }

  pstVencAttr->stRcAttr.enRcMode = RKADK_PARAM_GetRcMode(
      pstStreamCfg->attribute.rc_mode, pstStreamCfg->attribute.codec_type);
  ret =
      RKADK_MEDIA_SetRcAttr(&pstVencAttr->stRcAttr, pstStreamCfg->attribute.gop,
                            pstStreamCfg->attribute.bitrate,
                            pstSensorCfg->framerate, pstSensorCfg->framerate);
  if (ret) {
    RKADK_LOGE("RKADK_MEDIA_SetRcAttr failed");
    return -1;
  }

  pstVencAttr->stVencAttr.enType =
      RKADK_MEDIA_GetRkCodecType(pstStreamCfg->attribute.codec_type);
  pstVencAttr->stVencAttr.enPixelFormat =
      pstStreamCfg->vi_attr.stChnAttr.enPixelFormat;
  pstVencAttr->stVencAttr.u32MaxPicWidth = pstStreamCfg->attribute.width;
  pstVencAttr->stVencAttr.u32MaxPicHeight = pstStreamCfg->attribute.height;
  pstVencAttr->stVencAttr.u32PicWidth = pstStreamCfg->attribute.width;
  pstVencAttr->stVencAttr.u32PicHeight = pstStreamCfg->attribute.height;
  pstVencAttr->stVencAttr.u32VirWidth = pstStreamCfg->attribute.width;
  pstVencAttr->stVencAttr.u32VirHeight = pstStreamCfg->attribute.height;
  pstVencAttr->stVencAttr.u32Profile = pstStreamCfg->attribute.profile;
  pstVencAttr->stVencAttr.u32StreamBufCnt = RKADK_PARAM_GetStreamBufCnt(u32CamId, false);
  pstVencAttr->stVencAttr.u32BufSize = pstStreamCfg->attribute.bufsize;

  return 0;
}

static RKADK_S32 RKADK_STREAM_VencGetData(RKADK_U32 u32CamId,
                                          MPP_CHN_S *pstVencChn,
                                          STREAM_VIDEO_HANDLE_S *pHandle) {
  int ret = 0;

  VENC_RECV_PIC_PARAM_S stRecvParam;
  stRecvParam.s32RecvPicNum = -1;
  ret = RK_MPI_VENC_StartRecvFrame(pstVencChn->s32ChnId, &stRecvParam);
  if (ret) {
    RKADK_LOGE("RK_MPI_VENC_StartRecvFrame failed[%x]", ret);
    return ret;
  }

  ret = RKADK_MEDIA_GetVencBuffer(pstVencChn, RKADK_STREAM_VencOutCb, pHandle);
  if (ret) {
    RKADK_LOGE("RKADK_MEDIA_GetVencBuffer failed[%x]", ret);
    return ret;
  }

  return ret;
}

RKADK_S32 RKADK_STREAM_VideoInit(RKADK_STREAM_VIDEO_ATTR_S *pstVideoAttr,
                                 RKADK_MW_PTR *ppHandle) {
  int ret = 0;
  MPP_CHN_S stViChn, stVencChn, stSrcVpssChn, stDstVpssChn;
  RKADK_STREAM_TYPE_E enType;
  STREAM_VIDEO_HANDLE_S *pVideoHandle = NULL;
  VENC_CHN_ATTR_S stVencChnAttr;
  bool bUseVpss;
  VPSS_GRP_ATTR_S stGrpAttr;
  VPSS_CHN_ATTR_S stChnAttr;

  RKADK_CHECK_POINTER(pstVideoAttr, RKADK_FAILURE);
  RKADK_CHECK_CAMERAID(pstVideoAttr->u32CamId, RKADK_FAILURE);

  if (*ppHandle) {
    RKADK_LOGE("Video handle has been created");
    return -1;
  }

  RKADK_LOGI("Preview[%d] Video Init...", pstVideoAttr->u32CamId);

  if (!RKADK_MPI_SYS_CHECK()) {
    RKADK_LOGE("System is not initialized");
    return -1;
  }
  RKADK_PARAM_Init(NULL, NULL);

  RKADK_PARAM_STREAM_CFG_S *pstStreamCfg = RKADK_PARAM_GetStreamCfg(
      pstVideoAttr->u32CamId, RKADK_STREAM_TYPE_PREVIEW);
  if (!pstStreamCfg) {
    RKADK_LOGE("RKADK_PARAM_GetStreamCfg failed");
    return -1;
  }

  pVideoHandle = (STREAM_VIDEO_HANDLE_S *)malloc(sizeof(STREAM_VIDEO_HANDLE_S));
  if (!pVideoHandle) {
    RKADK_LOGE("malloc video handle failed");
    return -1;
  }
  memset(pVideoHandle, 0, sizeof(STREAM_VIDEO_HANDLE_S));

  pVideoHandle->bFirstKeyFrame = true;
  pVideoHandle->enCodecType = pstStreamCfg->attribute.codec_type;
  pVideoHandle->u32CamId = pstVideoAttr->u32CamId;
  pVideoHandle->pfnVencDataCB = pstVideoAttr->pfnDataCB;

  RKADK_STREAM_SetVideoChn(pstStreamCfg, pstVideoAttr->u32CamId, &stViChn,
                           &stVencChn, &stSrcVpssChn,&stDstVpssChn);

  enType = RKADK_PARAM_VencChnMux(pVideoHandle->u32CamId, stVencChn.s32ChnId);
  if (enType != RKADK_STREAM_TYPE_BUTT && enType != RKADK_STREAM_TYPE_PREVIEW) {
    switch (enType) {
    case RKADK_STREAM_TYPE_VIDEO_MAIN:
      RKADK_LOGI("Preview and Record main venc[%d] mux", stVencChn.s32ChnId);
      break;
    case RKADK_STREAM_TYPE_VIDEO_SUB:
      RKADK_LOGI("Preview and Record sub venc[%d] mux", stVencChn.s32ChnId);
      break;
    case RKADK_STREAM_TYPE_LIVE:
      RKADK_LOGI("Preview and Live venc[%d] mux", stVencChn.s32ChnId);
      break;
    default:
      RKADK_LOGW("Invaild venc[%d] mux, enType[%d]", stVencChn.s32ChnId,
                 enType);
      break;
    }
    pVideoHandle->bVencChnMux = true;
  }

  ret = RKADK_STREAM_SetVencAttr(pVideoHandle->u32CamId, pstStreamCfg,
                                 &stVencChnAttr);
  if (ret) {
    RKADK_LOGE("RKADK_STREAM_SetVencAttr failed");
    goto failed;
  }

  // Create VI
  ret = RKADK_MPI_VI_Init(pVideoHandle->u32CamId, stViChn.s32ChnId,
                          &(pstStreamCfg->vi_attr.stChnAttr));
  if (ret) {
    RKADK_LOGE("RKADK_MPI_VI_Init faleded[%x]", ret);
    return ret;
  }

  bUseVpss = RKADK_STREAM_IsUseVpss(pstStreamCfg);
  // Cteate VPSS
  if (bUseVpss) {
    memset(&stGrpAttr, 0, sizeof(VPSS_GRP_ATTR_S));
    memset(&stChnAttr, 0, sizeof(VPSS_CHN_ATTR_S));

    stGrpAttr.u32MaxW = 4096;
    stGrpAttr.u32MaxH = 4096;
    stGrpAttr.enPixelFormat = pstStreamCfg->vi_attr.stChnAttr.enPixelFormat;
    stGrpAttr.enCompressMode = COMPRESS_MODE_NONE;
    stGrpAttr.stFrameRate.s32SrcFrameRate = -1;
    stGrpAttr.stFrameRate.s32DstFrameRate = -1;
    stChnAttr.enChnMode = VPSS_CHN_MODE_USER;
    stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
    stChnAttr.enDynamicRange = DYNAMIC_RANGE_SDR8;
    stChnAttr.enPixelFormat = pstStreamCfg->vi_attr.stChnAttr.enPixelFormat;
    stChnAttr.stFrameRate.s32SrcFrameRate = -1;
    stChnAttr.stFrameRate.s32DstFrameRate = -1;
    stChnAttr.u32Width = pstStreamCfg->attribute.width;
    stChnAttr.u32Height = pstStreamCfg->attribute.height;
    stChnAttr.u32Depth = 0;

    ret = RKADK_MPI_VPSS_Init(pstStreamCfg->attribute.vpss_grp, pstStreamCfg->attribute.vpss_chn,
                              &stGrpAttr, &stChnAttr);
    if (ret) {
      RKADK_LOGE("RKADK_MPI_VPSS_Init vpssfalied[%d]",ret);
      RKADK_MPI_VI_DeInit(pVideoHandle->u32CamId, pstStreamCfg->vi_attr.u32ViChn);
      RKADK_MPI_VPSS_DeInit(pstStreamCfg->attribute.vpss_grp, pstStreamCfg->attribute.vpss_chn);
      return ret;
    }
  }

  // Create VENC
  ret = RKADK_MPI_VENC_Init(pVideoHandle->u32CamId, stVencChn.s32ChnId, &stVencChnAttr);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_VENC_Init failed[%x]", ret);
    goto failed;
  }

  RK_MPI_VENC_SetSceneMode(stVencChn.s32ChnId, RKADK_ENCODE_SENSE_CVR);
  RKADK_PARAM_SetVAdvancedParam(pstStreamCfg->attribute);

  ret = RKADK_STREAM_VencGetData(pVideoHandle->u32CamId, &stVencChn, pVideoHandle);
  if (ret) {
    RKADK_LOGE("RKADK_STREAM_VencGetData failed[%x]", ret);
    goto failed;
  }

  if (bUseVpss) {
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
    ret = RKADK_MPI_SYS_Bind(&stViChn, &stVencChn);
    if (ret) {
      RKADK_LOGE("RKADK_MPI_SYS_Bind failed[%x]", ret);
      goto failed;
    }
  }

  *ppHandle = (RKADK_MW_PTR)pVideoHandle;
  RKADK_LOGI("Preview[%d] Video Init End...", pVideoHandle->u32CamId);
  return 0;

failed:
  RKADK_LOGE("failed");
  RKADK_MPI_VENC_DeInit(stVencChn.s32ChnId);

  if (bUseVpss)
    RKADK_MPI_VPSS_DeInit(pstStreamCfg->attribute.vpss_grp, pstStreamCfg->attribute.vpss_chn);

  RKADK_MPI_VI_DeInit(pstVideoAttr->u32CamId, stViChn.s32ChnId);

  if (pVideoHandle)
    free(pVideoHandle);

  return ret;
}

RKADK_S32 RKADK_STREAM_VideoDeInit(RKADK_MW_PTR pHandle) {
  int ret = 0;
  MPP_CHN_S stViChn, stVencChn, stSrcVpssChn, stDstVpssChn;
  STREAM_VIDEO_HANDLE_S *pstHandle;
  bool bUseVpss;

  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);
  pstHandle = (STREAM_VIDEO_HANDLE_S *)pHandle;
  RKADK_CHECK_CAMERAID(pstHandle->u32CamId, RKADK_FAILURE);

  RKADK_LOGI("Preview[%d] Video DeInit...", pstHandle->u32CamId);

  RKADK_PARAM_STREAM_CFG_S *pstStreamCfg =
      RKADK_PARAM_GetStreamCfg(pstHandle->u32CamId, RKADK_STREAM_TYPE_PREVIEW);
  if (!pstStreamCfg) {
    RKADK_LOGE("RKADK_PARAM_GetStreamCfg failed");
    return -1;
  }

  RKADK_STREAM_SetVideoChn(pstStreamCfg, pstHandle->u32CamId, &stViChn,
                           &stVencChn, &stSrcVpssChn, &stDstVpssChn);
  RKADK_MEDIA_StopGetVencBuffer(&stVencChn, RKADK_STREAM_VencOutCb);

  bUseVpss = RKADK_STREAM_IsUseVpss(pstStreamCfg);
  if (bUseVpss) {
    // VPSS UnBind VENC
    ret = RKADK_MPI_SYS_UnBind(&stSrcVpssChn, &stVencChn);
    if (ret) {
      RKADK_LOGE("UnBind VPSS[%d] to VENC[%d] failed[%x]", stSrcVpssChn.s32ChnId,
                 stVencChn.s32ChnId, ret);
      return ret;
    }

    // VI UnBind VPSS
    ret = RKADK_MPI_SYS_UnBind(&stViChn, &stDstVpssChn);
    if (ret) {
      RKADK_LOGE("UnBind VI[%d] to VPSS[%d] failed[%x]", stViChn.s32ChnId,
                 stDstVpssChn.s32DevId, ret);
      return ret;
    }
  } else {
    // VI UnBind VENC
    ret = RKADK_MPI_SYS_UnBind(&stViChn, &stVencChn);
    if (ret) {
      RKADK_LOGE("RKADK_MPI_SYS_UnBind failed[%x]", ret);
      return ret;
    }
  }

  // destroy venc before vi
  ret = RKADK_MPI_VENC_DeInit(stVencChn.s32ChnId);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_VENC_DeInit failed[%x]", ret);
    return ret;
  }

  // destroy VPSS
  if (bUseVpss) {
    ret = RKADK_MPI_VPSS_DeInit(pstStreamCfg->attribute.vpss_grp, pstStreamCfg->attribute.vpss_chn);
    if (ret) {
      RKADK_LOGE("DeInit VPSS[%d] failed[%x]", stSrcVpssChn.s32ChnId, ret);
      return ret;
    }
  }

  // destroy vi
  ret = RKADK_MPI_VI_DeInit(pstHandle->u32CamId, stViChn.s32ChnId);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_VI_DeInit failed[%x]", ret);
    return ret;
  }

  RKADK_LOGI("Preview[%d] Video DeInit End...", pstHandle->u32CamId);

  if (pHandle) {
    free(pHandle);
    pHandle = NULL;
  }


  return 0;
}

RKADK_S32 RKADK_STREAM_VencStart(RKADK_MW_PTR pHandle, RKADK_S32 s32FrameCnt) {
  RKADK_PARAM_STREAM_CFG_S *pstStreamCfg = NULL;
  STREAM_VIDEO_HANDLE_S *pstHandle;

  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);
  pstHandle = (STREAM_VIDEO_HANDLE_S *)pHandle;
  RKADK_CHECK_CAMERAID(pstHandle->u32CamId, RKADK_FAILURE);

  if (pstHandle->start)
    return 0;

  pstStreamCfg =
      RKADK_PARAM_GetStreamCfg(pstHandle->u32CamId, RKADK_STREAM_TYPE_PREVIEW);
  if (!pstStreamCfg) {
    RKADK_LOGE("RKADK_PARAM_GetStreamCfg failed");
    return -1;
  }

  pstHandle->start = true;
  pstHandle->bRequestIDR = false;
  pstHandle->bWaitIDR = false;

  // multiplex venc chn, thread get mediabuffer
  if (pstHandle->bVencChnMux)
    return 0;

  VENC_RECV_PIC_PARAM_S stRecvParam;
  stRecvParam.s32RecvPicNum = s32FrameCnt;
  return RK_MPI_VENC_StartRecvFrame(pstStreamCfg->attribute.venc_chn,
                                    &stRecvParam);
}

RKADK_S32 RKADK_STREAM_VencStop(RKADK_MW_PTR pHandle) {
  RKADK_PARAM_STREAM_CFG_S *pstStreamCfg = NULL;
  STREAM_VIDEO_HANDLE_S *pstHandle;

  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);
  pstHandle = (STREAM_VIDEO_HANDLE_S *)pHandle;
  RKADK_CHECK_CAMERAID(pstHandle->u32CamId, RKADK_FAILURE);

  if (!pstHandle->start)
    return 0;

  pstStreamCfg =
      RKADK_PARAM_GetStreamCfg(pstHandle->u32CamId, RKADK_STREAM_TYPE_PREVIEW);
  if (!pstStreamCfg) {
    RKADK_LOGE("RKADK_PARAM_GetStreamCfg failed");
    return -1;
  }

  pstHandle->start = false;

  // multiplex venc chn, thread get mediabuffer
  if (pstHandle->bVencChnMux)
    return 0;

  VENC_RECV_PIC_PARAM_S stRecvParam;
  stRecvParam.s32RecvPicNum = 0;
  return RK_MPI_VENC_StartRecvFrame(pstStreamCfg->attribute.venc_chn,
                                    &stRecvParam);
}

RKADK_S32 RKADK_STREAM_GetVideoInfo(RKADK_U32 u32CamId,
                                    RKADK_VIDEO_INFO_S *pstVideoInfo) {
  RKADK_PARAM_STREAM_CFG_S *pstStreamCfg = NULL;
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg = NULL;

  RKADK_CHECK_POINTER(pstVideoInfo, RKADK_FAILURE);
  RKADK_CHECK_CAMERAID(u32CamId, RKADK_FAILURE);

  RKADK_PARAM_Init(NULL, NULL);

  pstSensorCfg = RKADK_PARAM_GetSensorCfg(u32CamId);
  if (!pstSensorCfg) {
    RKADK_LOGE("RKADK_PARAM_GetSensorCfg failed");
    return -1;
  }

  pstStreamCfg =
      RKADK_PARAM_GetStreamCfg(u32CamId, RKADK_STREAM_TYPE_PREVIEW);
  if (!pstStreamCfg) {
    RKADK_LOGE("RKADK_PARAM_GetStreamCfg failed");
    return -1;
  }

  memset(pstVideoInfo, 0, sizeof(RKADK_VIDEO_INFO_S));
  pstVideoInfo->enCodecType = pstStreamCfg->attribute.codec_type;
  pstVideoInfo->u32Width = pstStreamCfg->attribute.width;
  pstVideoInfo->u32Height = pstStreamCfg->attribute.height;
  pstVideoInfo->u32BitRate = pstStreamCfg->attribute.bitrate;
  pstVideoInfo->u32FrameRate = pstSensorCfg->framerate;
  pstVideoInfo->u32Gop = pstStreamCfg->attribute.gop;
  return 0;
}

/**************************************************/
/*                     Audio API                  */
/**************************************************/
static void RKADK_STREAM_AencOutCb(AUDIO_STREAM_S stFrame,
                                   RKADK_VOID *pHandle) {
  RKADK_AUDIO_STREAM_S stStreamData;

  STREAM_AUDIO_HANDLE_S *pstHandle = (STREAM_AUDIO_HANDLE_S *)pHandle;
  if (!pstHandle) {
    RKADK_LOGE("Get aenc thread invalid param");
    return;
  }

  memset(&stStreamData, 0, sizeof(RKADK_AUDIO_STREAM_S));
  stStreamData.pStream = (RKADK_U8 *)RK_MPI_MB_Handle2VirAddr(stFrame.pMbBlk);
  stStreamData.u32Len = stFrame.u32Len;
  stStreamData.u32Seq = pstHandle->audioSeq;
  stStreamData.u64TimeStamp = stFrame.u64TimeStamp;
  stStreamData.enType = pstHandle->enCodecType;

  if (pstHandle->pfnAencDataCB && pstHandle->start)
    pstHandle->pfnAencDataCB(&stStreamData);

  pstHandle->audioSeq++;
}

static void *RKADK_STREAM_GetPcmMB(void *params) {
  int ret;
  RKADK_AUDIO_STREAM_S stStreamData;
  AUDIO_FRAME_S frame;

  STREAM_AUDIO_HANDLE_S *pHandle = (STREAM_AUDIO_HANDLE_S *)params;
  if (!pHandle) {
    RKADK_LOGE("Get pcm thread invalid param");
    return NULL;
  }

  while (pHandle->bGetBuffer) {
    ret = RK_MPI_AI_GetFrame(pHandle->stAiChn.s32DevId,
                             pHandle->stAiChn.s32ChnId, &frame, RK_NULL, -1);
    if (ret) {
      RKADK_LOGD("RK_MPI_AI_GetFrame failed[%x]", ret);
      break;
    }

    memset(&stStreamData, 0, sizeof(RKADK_AUDIO_STREAM_S));
    stStreamData.pStream = (RKADK_U8 *)RK_MPI_MB_Handle2VirAddr(frame.pMbBlk);
    stStreamData.u32Len = frame.u32Len;
    stStreamData.u32Seq = pHandle->pcmSeq;
    stStreamData.u64TimeStamp = frame.u64TimeStamp;
    stStreamData.enType = RKADK_CODEC_TYPE_PCM;

    if (pHandle->pfnPcmDataCB && pHandle->start)
      pHandle->pfnPcmDataCB(&stStreamData);

    pHandle->pcmSeq++;
    RK_MPI_AI_ReleaseFrame(pHandle->stAiChn.s32DevId, pHandle->stAiChn.s32ChnId,
                           &frame, RK_NULL);
  }

  RKADK_LOGD("Exit pcm read thread");
  return NULL;
}

static int
RKADK_STREAM_CreateDataThread(STREAM_AUDIO_HANDLE_S *pHandle) {
  int ret = 0;
  char name[256];

  pHandle->bGetBuffer = true;
  if (pHandle->enCodecType == RKADK_CODEC_TYPE_PCM) {
    ret =
        pthread_create(&pHandle->pcmTid, NULL, RKADK_STREAM_GetPcmMB, pHandle);
  } else {
    ret = RKADK_MEDIA_GetAencBuffer(&pHandle->stAencChn, RKADK_STREAM_AencOutCb,
                                    pHandle);

    if (pHandle->pfnPcmDataCB)
      ret |= pthread_create(&pHandle->pcmTid, NULL, RKADK_STREAM_GetPcmMB,
                            pHandle);
  }

  if (ret) {
    RKADK_LOGE("Create read audio thread for failed %d", ret);
    return ret;
  }
  if (pHandle->pcmTid) {
    snprintf(name, sizeof(name), "AudioStream_%d", pHandle->enCodecType);
    pthread_setname_np(pHandle->pcmTid, name);
  }

  return 0;
}

static int
RKADK_STREAM_DestoryDataThread(STREAM_AUDIO_HANDLE_S *pHandle) {
  int ret = 0;
  if (pHandle->enCodecType != RKADK_CODEC_TYPE_PCM) {
    ret = RKADK_MEDIA_StopGetAencBuffer(&pHandle->stAencChn,
                                        RKADK_STREAM_AencOutCb);
  }

  if (ret)
    RKADK_LOGE("RKADK_MEDIA_StopGetAencBuffer failed");

  pHandle->bGetBuffer = false;
  if (pHandle->pcmTid) {
    ret = pthread_join(pHandle->pcmTid, NULL);
    if (ret)
      RKADK_LOGE("read pcm thread exit failed!");
    else
      RKADK_LOGD("read pcm thread exit ok");
    pHandle->pcmTid = 0;
  }

  return ret;
}

static RKADK_S32
RKADK_STREAM_SetAiConfig(MPP_CHN_S *pstAiChn, AIO_ATTR_S *pstAiAttr,
                         RKADK_PARAM_AUDIO_CFG_S *pstAudioParam) {
  AUDIO_SOUND_MODE_E soundMode;
  int bytes = 2; // if the requirement is 16bit
  RKADK_CHECK_POINTER(pstAiChn, RKADK_FAILURE);
  RKADK_CHECK_POINTER(pstAiAttr, RKADK_FAILURE);

  memset(pstAiAttr, 0, sizeof(AIO_ATTR_S));
  memcpy(pstAiAttr->u8CardName, pstAudioParam->audio_node,
         strlen(pstAudioParam->audio_node));
  pstAiAttr->soundCard.channels = AUDIO_DEVICE_CHANNEL;
  pstAiAttr->soundCard.sampleRate = pstAudioParam->samplerate;
  bytes = RKADK_MEDIA_GetAudioBitWidth(pstAudioParam->bit_width) / 8;
  pstAiAttr->soundCard.bitWidth = pstAudioParam->bit_width;

  pstAiAttr->enBitwidth = pstAudioParam->bit_width;
  pstAiAttr->enSamplerate = (AUDIO_SAMPLE_RATE_E)pstAudioParam->samplerate;
  soundMode = RKADK_AI_GetSoundMode(pstAudioParam->channels);
  if (soundMode == AUDIO_SOUND_MODE_BUTT)
    return -1;

  pstAiAttr->enSoundmode = soundMode;
  pstAiAttr->u32FrmNum = 2;
  pstAiAttr->u32PtNumPerFrm = bytes * pstAudioParam->samples_per_frame;
  pstAiAttr->u32EXFlag = 0;
  pstAiAttr->u32ChnCnt = pstAudioParam->channels;

  pstAiChn->enModId = RK_ID_AI;
  pstAiChn->s32DevId = 0;
  pstAiChn->s32ChnId = PREVIEW_AI_CHN;
  return 0;
}

static RKADK_S32 RKADK_STREAM_SetAencConfig(RKADK_U32 u32CamId,
                                            STREAM_AUDIO_HANDLE_S *pstHandle,
                                            AENC_CHN_ATTR_S *pstAencAttr) {
  RKADK_PARAM_AUDIO_CFG_S *pstAudioParam = NULL;

  RKADK_CHECK_POINTER(pstHandle, RKADK_FAILURE);
  RKADK_CHECK_POINTER(pstAencAttr, RKADK_FAILURE);

  pstAudioParam = RKADK_PARAM_GetAudioCfg();
  if (!pstAudioParam) {
    RKADK_LOGE("RKADK_PARAM_GetAudioCfg failed");
    return -1;
  }

  memset(pstAencAttr, 0, sizeof(AENC_CHN_ATTR_S));
  pstAencAttr->enType = RKADK_MEDIA_GetRkCodecType(pstHandle->enCodecType);
  pstAencAttr->u32BufCount = RKADK_PARAM_GetStreamBufCnt(u32CamId, true);
  pstAencAttr->stCodecAttr.enType = pstAencAttr->enType;
  pstAencAttr->stCodecAttr.u32Channels = pstAudioParam->channels;
  pstAencAttr->stCodecAttr.u32SampleRate = pstAudioParam->samplerate;
  pstAencAttr->stCodecAttr.enBitwidth = pstAudioParam->bit_width;
  pstAencAttr->stCodecAttr.pstResv = RK_NULL;

  if (pstHandle->enCodecType == RKADK_CODEC_TYPE_MP3){
    pstAencAttr->stCodecAttr.u32Resv[0] = pstAudioParam->samples_per_frame;
    pstAencAttr->stCodecAttr.u32Resv[1] = pstAudioParam->bitrate;
  } else if (pstHandle->enCodecType == RKADK_CODEC_TYPE_ACC){
    pstAencAttr->stCodecAttr.u32Resv[0] = 2;
    pstAencAttr->stCodecAttr.u32Resv[1] = pstAudioParam->bitrate;
  }

  pstHandle->stAencChn.enModId = RK_ID_AENC;
  pstHandle->stAencChn.s32DevId = 0;
  pstHandle->stAencChn.s32ChnId = PREVIEW_AENC_CHN;
  return 0;
}

RKADK_S32 RKADK_STREAM_AudioInit(RKADK_STREAM_AUDIO_ATTR_S *pstAudioAttr,
                                 RKADK_MW_PTR *ppHandle) {
  int ret;
  RKADK_PARAM_AUDIO_CFG_S *pstAudioParam = NULL;
  STREAM_AUDIO_HANDLE_S *pAudioHandle = NULL;

  RKADK_CHECK_POINTER(pstAudioAttr, RKADK_FAILURE);
  RKADK_CHECK_CAMERAID(pstAudioAttr->u32CamId, RKADK_FAILURE);

  if (*ppHandle) {
    RKADK_LOGE("Audio handle has been created");
    return -1;
  }

  RKADK_LOGI("Preview Audio Init...");

  if (RKADK_STREAM_CheckCodecType(pstAudioAttr->enCodecType))
    return -1;

  if (!RKADK_MPI_SYS_CHECK()) {
    RKADK_LOGE("System is not initialized");
    return -1;
  }
  RKADK_PARAM_Init(NULL, NULL);

  pAudioHandle = (STREAM_AUDIO_HANDLE_S *)malloc(sizeof(STREAM_AUDIO_HANDLE_S));
  if (!pAudioHandle) {
    RKADK_LOGE("malloc audio handle failed");
    return -1;
  }
  memset(pAudioHandle, 0, sizeof(STREAM_AUDIO_HANDLE_S));

  pAudioHandle->pfnPcmDataCB = pstAudioAttr->pfnPcmDataCB;
  pAudioHandle->pfnAencDataCB = pstAudioAttr->pfnAencDataCB;

  pstAudioParam = RKADK_PARAM_GetAudioCfg();
  if (!pstAudioParam) {
    RKADK_LOGE("RKADK_PARAM_GetAudioCfg failed");
    return -1;
  }

  if (pstAudioAttr->enCodecType != pstAudioParam->codec_type) {
    RKADK_LOGW("enCodecType[%d] != codec_type[%d]", pstAudioAttr->enCodecType,
               pstAudioParam->codec_type);

    if (pstAudioAttr->enCodecType == RKADK_CODEC_TYPE_BUTT)
      pstAudioAttr->enCodecType = pstAudioParam->codec_type;
  }

  pAudioHandle->enCodecType = pstAudioAttr->enCodecType;
  if (RKADK_MEDIA_EnableAencRegister(pAudioHandle->enCodecType)) {
    ret = RKADK_AUDIO_ENCODER_Register(pAudioHandle->enCodecType);
    if (ret) {
      RKADK_LOGE("RKADK_AUDIO_ENCODER_Register failed(%d)", ret);
      return ret;
    }
  }

  // Create AI
  AIO_ATTR_S aiAttr;
  ret = RKADK_STREAM_SetAiConfig(&pAudioHandle->stAiChn, &aiAttr, pstAudioParam);
  if (ret) {
    RKADK_LOGE("RKADK_STREAM_SetAiConfig failed");
    return ret;
  }

  ret = RKADK_MPI_AI_Init(pAudioHandle->stAiChn.s32DevId, pAudioHandle->stAiChn.s32ChnId,
                          &aiAttr, pstAudioParam->vqe_mode, pstAudioParam->mic_type);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_AI_Init faile[%x]", ret);
    return ret;
  }

  if (pAudioHandle->enCodecType != RKADK_CODEC_TYPE_PCM) {
    AENC_CHN_ATTR_S aencAttr;
    if (RKADK_STREAM_SetAencConfig(pstAudioAttr->u32CamId, pAudioHandle, &aencAttr)) {
      RKADK_LOGE("StreamSetAencChnAttr failed");
      goto pcm_mode;
    }

    ret = RKADK_MPI_AENC_Init(pAudioHandle->stAencChn.s32ChnId, &aencAttr);
    if (ret) {
      RKADK_LOGE("RKADK_MPI_AENC_Init failed[%x]", ret);
      goto failed;
    }

    ret = RKADK_STREAM_CreateDataThread(pAudioHandle);
    if (ret)
      goto failed;

    ret = RKADK_MPI_SYS_Bind(&pAudioHandle->stAiChn, &pAudioHandle->stAencChn);
    if (ret) {
      RKADK_LOGE("RKADK_MPI_SYS_Bind failed[%x]", ret);
      goto failed;
    }

    *ppHandle = (RKADK_MW_PTR)pAudioHandle;
    RKADK_LOGI("Preview[%d] Audio Init End...", pAudioHandle->enCodecType);
    return 0;
  }

pcm_mode:
  RKADK_LOGI("PCM Mode");
  ret = RKADK_STREAM_CreateDataThread(pAudioHandle);
  if (ret)
    goto failed;

  *ppHandle = (RKADK_MW_PTR)pAudioHandle;
  RKADK_LOGI("Preview[%d] Audio Init End...", pAudioHandle->enCodecType);
  return 0;

failed:
  RKADK_MPI_AENC_DeInit(pAudioHandle->stAencChn.s32ChnId);
  RKADK_MPI_AI_DeInit(pAudioHandle->stAiChn.s32DevId, pAudioHandle->stAiChn.s32ChnId,
                      pstAudioParam->vqe_mode);

  if (pAudioHandle)
    free(pAudioHandle);

  return ret;
}

RKADK_S32 RKADK_STREAM_AudioDeInit(RKADK_MW_PTR pHandle) {
  int ret = 0;
  RKADK_PARAM_AUDIO_CFG_S *pstAudioParam = NULL;
  STREAM_AUDIO_HANDLE_S *pstHandle;

  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);
  pstHandle = (STREAM_AUDIO_HANDLE_S *)pHandle;

  RKADK_LOGI("Preview Audio DeInit...");

  pstAudioParam = RKADK_PARAM_GetAudioCfg();
  if (!pstAudioParam) {
    RKADK_LOGE("RKADK_PARAM_GetAudioCfg failed");
    return -1;
  }

  RKADK_STREAM_DestoryDataThread(pstHandle);

  if (pstHandle->enCodecType != RKADK_CODEC_TYPE_PCM) {
    ret = RKADK_MPI_SYS_UnBind(&pstHandle->stAiChn, &pstHandle->stAencChn);
    if (ret) {
      RKADK_LOGE("RKADK_MPI_SYS_UnBind failed[%x]", ret);
      return ret;
    }

    ret = RKADK_MPI_AENC_DeInit(pstHandle->stAencChn.s32ChnId);
    if (ret) {
      RKADK_LOGE("Destroy AENC[%d] failed[%x]", pstHandle->stAencChn.s32ChnId,
                 ret);
      return ret;
    }
  }

  ret =
      RKADK_MPI_AI_DeInit(pstHandle->stAiChn.s32DevId,
                          pstHandle->stAiChn.s32ChnId, pstAudioParam->vqe_mode);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_AI_DeInit failed[%x]", ret);
    return ret;
  }

  if (RKADK_MEDIA_EnableAencRegister(pstHandle->enCodecType)) {
    ret = RKADK_AUDIO_ENCODER_UnRegister(pstHandle->enCodecType);
    if (ret) {
      RKADK_LOGE("RKADK_AUDIO_ENCODER_UnRegister failed[%x]", ret);
      return ret;
    }
  }

  pstHandle->start = false;
  if (pHandle) {
    free(pHandle);
    pHandle = NULL;
  }

  RKADK_LOGI("Preview Audio DeInit End...");
  return 0;
}

RKADK_S32 RKADK_STREAM_AencStart(RKADK_MW_PTR pHandle) {
  STREAM_AUDIO_HANDLE_S *pstHandle;

  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);
  pstHandle = (STREAM_AUDIO_HANDLE_S *)pHandle;

  pstHandle->start = true;
  return 0;
}

RKADK_S32 RKADK_STREAM_AencStop(RKADK_MW_PTR pHandle) {
  STREAM_AUDIO_HANDLE_S *pstHandle;

  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);
  pstHandle = (STREAM_AUDIO_HANDLE_S *)pHandle;

  pstHandle->start = false;
  return 0;
}

RKADK_S32 RKADK_STREAM_GetAudioInfo(RKADK_MW_PTR pHandle,
                                    RKADK_AUDIO_INFO_S *pstAudioInfo) {
  RKADK_PARAM_AUDIO_CFG_S *pstAudioParam = NULL;

  RKADK_CHECK_POINTER(pstAudioInfo, RKADK_FAILURE);
  memset(pstAudioInfo, 0, sizeof(RKADK_AUDIO_INFO_S));

  RKADK_PARAM_Init(NULL, NULL);

  pstAudioParam = RKADK_PARAM_GetAudioCfg();
  if (!pstAudioParam) {
    RKADK_LOGE("RKADK_PARAM_GetAudioCfg failed");
    return -1;
  }

  if (pHandle) {
    STREAM_AUDIO_HANDLE_S *pstHandle = (STREAM_AUDIO_HANDLE_S *)pHandle;
    pstAudioInfo->enCodecType = pstHandle->enCodecType;
  } else {
    pstAudioInfo->enCodecType = RKADK_CODEC_TYPE_PCM;
  }

  pstAudioInfo->u16SampleBitWidth =
      RKADK_MEDIA_GetAudioBitWidth(pstAudioParam->bit_width);
  pstAudioInfo->u32ChnCnt = pstAudioParam->channels;
  pstAudioInfo->u32SampleRate = pstAudioParam->samplerate;
  pstAudioInfo->u32AvgBytesPerSec = pstAudioParam->bitrate / 8;
  pstAudioInfo->u32SamplesPerFrame = pstAudioParam->samples_per_frame;

  return 0;
}
