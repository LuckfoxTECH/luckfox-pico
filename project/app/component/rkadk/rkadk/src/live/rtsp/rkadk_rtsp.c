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

#include "rkadk_rtsp.h"
#include "rkadk_log.h"
#include "rkadk_media_comm.h"
#include "rkadk_audio_encoder.h"
#include "rkadk_param.h"
#include "rtsp_demo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  bool start;
  bool bRequestIDR;
  bool bWaitIDR;
  bool bVencChnMux;
  bool bFirstKeyFrame;
  RKADK_U32 u32CamId;
  rtsp_demo_handle stRtspHandle;
  rtsp_session_handle stRtspSession;
} RKADK_RTSP_HANDLE_S;

static void RKADK_RTSP_SetVideoChn(RKADK_PARAM_STREAM_CFG_S *pstLiveCfg, RKADK_U32 u32CamId,
                                   MPP_CHN_S *pstViChn, MPP_CHN_S *pstVencChn,
                                   MPP_CHN_S *pstSrcVpssChn, MPP_CHN_S *pstDstVpssChn) {
  pstViChn->enModId = RK_ID_VI;
  pstViChn->s32DevId = u32CamId;
  pstViChn->s32ChnId = pstLiveCfg->vi_attr.u32ViChn;

  pstSrcVpssChn->enModId = RK_ID_VPSS;
  pstSrcVpssChn->s32DevId = pstLiveCfg->attribute.vpss_grp;
  pstSrcVpssChn->s32ChnId = pstLiveCfg->attribute.vpss_chn;

  pstDstVpssChn->enModId = RK_ID_VPSS;
  pstDstVpssChn->s32DevId = pstLiveCfg->attribute.vpss_grp;
  pstDstVpssChn->s32ChnId = 0; //When vpss is dst, chn is equal to 0

  pstVencChn->enModId = RK_ID_VENC;
  pstVencChn->s32DevId = 0;
  pstVencChn->s32ChnId = pstLiveCfg->attribute.venc_chn;
}

static void RKADK_RTSP_AudioSetChn(MPP_CHN_S *pstAiChn, MPP_CHN_S *pstAencChn) {
  pstAiChn->enModId = RK_ID_AI;
  pstAiChn->s32DevId = 0;
  pstAiChn->s32ChnId = LIVE_AI_CHN;

  pstAencChn->enModId = RK_ID_AENC;
  pstAencChn->s32DevId = 0;
  pstAencChn->s32ChnId = LIVE_AENC_CHN + 1;
}

static int RKADK_RTSP_SetVencAttr(RKADK_U32 u32CamId,
                                  RKADK_PARAM_STREAM_CFG_S *pstLiveCfg,
                                  VENC_CHN_ATTR_S *pstVencAttr) {
  int ret;
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg = NULL;
  RKADK_U32 u32DstFrameRateNum;
  RKADK_PARAM_FPS_S stFps;

  RKADK_CHECK_POINTER(pstVencAttr, RKADK_FAILURE);
  memset(pstVencAttr, 0, sizeof(VENC_CHN_ATTR_S));

  pstSensorCfg = RKADK_PARAM_GetSensorCfg(u32CamId);
  if (!pstSensorCfg) {
    RKADK_LOGE("RKADK_PARAM_GetSensorCfg failed");
    return -1;
  }

  pstVencAttr->stRcAttr.enRcMode = RKADK_PARAM_GetRcMode(
      pstLiveCfg->attribute.rc_mode, pstLiveCfg->attribute.codec_type);

  u32DstFrameRateNum = pstLiveCfg->attribute.framerate;
  if (u32DstFrameRateNum > pstSensorCfg->framerate) {
    RKADK_LOGW("CamId[%d] rtsp fps[%d] > sensor fps[%d], use sensor fps",
                u32CamId, u32DstFrameRateNum, pstSensorCfg->framerate);
    u32DstFrameRateNum = pstSensorCfg->framerate;

    stFps.u32Framerate = pstSensorCfg->framerate;
    stFps.enStreamType = RKADK_STREAM_TYPE_LIVE;
    RKADK_PARAM_SetCamParam(u32CamId, RKADK_PARAM_TYPE_FPS, &stFps);
  }

  ret = RKADK_MEDIA_SetRcAttr(&pstVencAttr->stRcAttr, pstLiveCfg->attribute.gop,
                              pstLiveCfg->attribute.bitrate,
                              pstSensorCfg->framerate, u32DstFrameRateNum);
  if (ret) {
    RKADK_LOGE("RKADK_MEDIA_SetRcAttr failed");
    return -1;
  }

  pstVencAttr->stVencAttr.enType =
      RKADK_MEDIA_GetRkCodecType(pstLiveCfg->attribute.codec_type);
  pstVencAttr->stVencAttr.enPixelFormat = pstLiveCfg->vi_attr.stChnAttr.enPixelFormat;
  pstVencAttr->stVencAttr.u32MaxPicWidth = pstLiveCfg->attribute.width;
  pstVencAttr->stVencAttr.u32MaxPicHeight = pstLiveCfg->attribute.height;
  pstVencAttr->stVencAttr.u32PicWidth = pstLiveCfg->attribute.width;
  pstVencAttr->stVencAttr.u32PicHeight = pstLiveCfg->attribute.height;
  pstVencAttr->stVencAttr.u32VirWidth = pstLiveCfg->attribute.width;
  pstVencAttr->stVencAttr.u32VirHeight = pstLiveCfg->attribute.height;
  pstVencAttr->stVencAttr.u32Profile = pstLiveCfg->attribute.profile;
  pstVencAttr->stVencAttr.u32StreamBufCnt = RKADK_PARAM_GetStreamBufCnt(u32CamId, false);
  pstVencAttr->stVencAttr.u32BufSize = pstLiveCfg->attribute.bufsize;

  return 0;
}

static RKADK_S32 RKADK_RTSP_InitService(RKADK_CODEC_TYPE_E enCodecType,
                                        RKADK_U32 port, const char *path,
                                        RKADK_RTSP_HANDLE_S *pHandle) {
  int ret = 0;
  RKADK_PARAM_AUDIO_CFG_S *pstAudioCfg = RKADK_PARAM_GetAudioCfg();

  if (!pstAudioCfg) {
    RKADK_LOGD("RKADK_PARAM_GetAudioCfg failed");
    return -1;
  }

  pHandle->stRtspHandle = create_rtsp_demo(port);
  pHandle->stRtspSession = rtsp_new_session(pHandle->stRtspHandle, path);
  if (enCodecType == RKADK_CODEC_TYPE_H264) {
    ret = rtsp_set_video(pHandle->stRtspSession, RTSP_CODEC_ID_VIDEO_H264, NULL, 0);
    if (ret) {
      RKADK_LOGE("rtsp_set_video failed(%d)", ret);
      return -1;
    }
  } else if (enCodecType == RKADK_CODEC_TYPE_H265) {
    ret = rtsp_set_video(pHandle->stRtspSession, RTSP_CODEC_ID_VIDEO_H265, NULL, 0);
    if (ret) {
      RKADK_LOGE("rtsp_set_video failed(%d)", ret);
      return -1;
    }
  } else {
    RKADK_LOGE("Unsupport enCodecType: %d", enCodecType);
    return -1;
  }

  ret = rtsp_sync_video_ts(pHandle->stRtspSession, rtsp_get_reltime(),
                            rtsp_get_ntptime());
  if (ret) {
    RKADK_LOGE("rtsp_sync_video_ts failed(%d)", ret);
    return -1;
  }

  ret = rtsp_set_audio(pHandle->stRtspSession, RTSP_CODEC_ID_AUDIO_G711A, NULL, 0);
  if (ret) {
    RKADK_LOGE("rtsp_set_audio failed(%d)", ret);
    return -1;
  }

  ret = rtsp_sync_audio_ts(pHandle->stRtspSession, rtsp_get_reltime(),
                            rtsp_get_ntptime());
  if (ret) {
    RKADK_LOGE("rtsp_sync_audio_ts failed(%d)", ret);
    return -1;
  }

  ret = rtsp_set_audio_sample_rate(pHandle->stRtspSession, pstAudioCfg->samplerate);
  if (ret) {
    RKADK_LOGE("rtsp_set_audio_sample_rate failed(%d)", ret);
    return -1;
  }

  ret = rtsp_set_audio_channels(pHandle->stRtspSession, pstAudioCfg->channels);
  if (ret) {
    RKADK_LOGE("rtsp_set_audio_channels failed(%d)", ret);
    return -1;
  }

  return 0;
}

static void RKADK_RTSP_DeInitService(RKADK_RTSP_HANDLE_S *pHandle) {
  if (pHandle->stRtspSession)
    rtsp_del_session(pHandle->stRtspSession);

  if (pHandle->stRtspHandle)
    rtsp_del_demo(pHandle->stRtspHandle);
}

static RKADK_S32 RKADK_RTSP_RequestIDR(RKADK_U32 u32CamId, RKADK_U32 u32ChnId) {
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

static void RKADK_RTSP_VencOutCb(RKADK_MEDIA_VENC_DATA_S mb, RKADK_VOID *handle) {
  RKADK_MEDIA_VENC_DATA_S stData = mb;
  RKADK_PARAM_STREAM_CFG_S *pstLiveCfg;
  RKADK_RTSP_HANDLE_S *pHandle = (RKADK_RTSP_HANDLE_S *)handle;
  RKADK_VOID *data;

  if (!pHandle) {
    RKADK_LOGE("Can't find rtsp handle");
    RK_MPI_VENC_ReleaseStream(stData.u32ChnId, &stData.stFrame);
    return;
  }

  pstLiveCfg = RKADK_PARAM_GetStreamCfg(pHandle->u32CamId, RKADK_STREAM_TYPE_LIVE);
  if (!pstLiveCfg) {
    RKADK_LOGE("Live RKADK_PARAM_GetStreamCfg Live failed");
    return;
  }

  if (!pHandle->start)
    return;

  if (!pHandle->bWaitIDR) {
    if (!RKADK_MEDIA_CheckIdrFrame(pstLiveCfg->attribute.codec_type,
                                   stData.stFrame.pstPack->DataType)) {
      if (!pHandle->bRequestIDR) {
        RKADK_LOGD("requst idr frame");
        RKADK_RTSP_RequestIDR(pHandle->u32CamId, pstLiveCfg->attribute.venc_chn);
        pHandle->bRequestIDR = true;
      } else {
        RKADK_LOGD("wait first idr frame");
      }

      return;
    }

    pHandle->bWaitIDR = true;
    if (pHandle->bFirstKeyFrame) {
      RKADK_KLOG("Rtsp first key frame pts: %lld", stData.stFrame.pstPack->u64PTS);
      pHandle->bFirstKeyFrame = false;
    }
  }

  data = RK_MPI_MB_Handle2VirAddr(stData.stFrame.pstPack->pMbBlk);
  if (pHandle->stRtspHandle && pHandle->stRtspSession) {
    rtsp_tx_video(pHandle->stRtspSession, (uint8_t *)data,
                  stData.stFrame.pstPack->u32Len, stData.stFrame.pstPack->u64PTS);
    rtsp_do_event(pHandle->stRtspHandle);
  }
}

static void RKADK_RTSP_AencOutCb(AUDIO_STREAM_S stFrame,
                                 RKADK_VOID *pHandle) {
  RKADK_CHECK_POINTER_N(pHandle);
  RKADK_RTSP_HANDLE_S *pstHandle = (RKADK_RTSP_HANDLE_S *)pHandle;
  RKADK_U8 *data;
  if (!pstHandle) {
    RKADK_LOGE("Can't find rtsp handle");
    return;
  }

  if (!pstHandle->start)
    return;

  data = (RKADK_U8 *)RK_MPI_MB_Handle2VirAddr(stFrame.pMbBlk);
  if (pstHandle->stRtspHandle && pstHandle->stRtspSession) {
    rtsp_tx_audio(pstHandle->stRtspSession, (uint8_t *)data,
                  stFrame.u32Len, stFrame.u64TimeStamp);
    rtsp_do_event(pstHandle->stRtspHandle);
  }
}

static RKADK_S32 RKADK_RTSP_VencGetData(RKADK_U32 u32CamId,
                                        MPP_CHN_S *pstVencChn,
                                        RKADK_RTSP_HANDLE_S *pHandle) {
  int ret = 0;

  VENC_RECV_PIC_PARAM_S stRecvParam;
  stRecvParam.s32RecvPicNum = -1;
  ret = RK_MPI_VENC_StartRecvFrame(pstVencChn->s32ChnId, &stRecvParam);
  if (ret) {
    RKADK_LOGE("RK_MPI_VENC_StartRecvFrame failed = %d", ret);
    return ret;
  }

  ret = RKADK_MEDIA_GetVencBuffer(pstVencChn, RKADK_RTSP_VencOutCb,
                                (RKADK_VOID *)pHandle);
  if (ret) {
    RKADK_LOGE("RKADK_MEDIA_GetVencBuffer failed = %d", ret);
    return ret;
  }


  return ret;
}

static int RKADK_RTSP_AencGetData(RKADK_U32 u32CamId,
                                  MPP_CHN_S *pstAencChn,
                                  RKADK_RTSP_HANDLE_S *pHandle) {
  int ret;

  ret = RKADK_MEDIA_GetAencBuffer(pstAencChn, RKADK_RTSP_AencOutCb,
                                  (RKADK_VOID *)pHandle);
  if (ret) {
    RKADK_LOGE("RKADK_MEDIA_GetVencBuffer failed = %d", ret);
    return ret;
  }

  return 0;
}

static bool RKADK_RTSP_IsUseVpss(RKADK_U32 u32CamId, RKADK_PARAM_STREAM_CFG_S *pstLiveCfg) {
  bool bUseVpss = false;
  RKADK_U32 u32SrcWidth, u32SrcHeight;
  RKADK_U32 u32DstWidth, u32DstHeight;
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg;

  pstSensorCfg = RKADK_PARAM_GetSensorCfg(u32CamId);
  if (!pstSensorCfg) {
    RKADK_LOGE("RKADK_PARAM_GetSensorCfg failed");
    return false;
  }

  u32SrcWidth = pstLiveCfg->vi_attr.stChnAttr.stSize.u32Width;
  u32SrcHeight = pstLiveCfg->vi_attr.stChnAttr.stSize.u32Height;
  u32DstWidth = pstLiveCfg->attribute.width;
  u32DstHeight = pstLiveCfg->attribute.height;

  if (u32DstWidth != u32SrcWidth || u32DstHeight != u32SrcHeight) {
    RKADK_LOGD("In[%d, %d], Out[%d, %d]", u32SrcWidth, u32SrcHeight,
               u32DstWidth, u32DstHeight);
    bUseVpss = true;
  }

  if (pstLiveCfg->attribute.post_aiisp)
    bUseVpss = true;

  return bUseVpss;
}

static RKADK_S32 RKADK_RTSP_SetAiAttr(AIO_ATTR_S *pstAiAttr,
                                      RKADK_PARAM_AUDIO_CFG_S *pstAudioParam) {
  int bytes = 2;
  AUDIO_SOUND_MODE_E soundMode;
  RKADK_CHECK_POINTER(pstAiAttr, RKADK_FAILURE);

  memset(pstAiAttr, 0, sizeof(AIO_ATTR_S));
  memcpy(pstAiAttr->u8CardName, pstAudioParam->ai_audio_node,
         strlen(pstAudioParam->ai_audio_node));
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
  return 0;
}

static RKADK_S32 RKADK_RTSP_SetAencAttr(RKADK_U32 u32CamId,
                                        RKADK_PARAM_AUDIO_CFG_S *pstAudioParam,
                                        AENC_CHN_ATTR_S *pstAencAttr) {

  RKADK_CHECK_POINTER(pstAudioParam, RKADK_FAILURE);
  RKADK_CHECK_POINTER(pstAencAttr, RKADK_FAILURE);

  memset(pstAencAttr, 0, sizeof(AENC_CHN_ATTR_S));
  pstAencAttr->enType = RK_AUDIO_ID_PCM_ALAW;
  pstAencAttr->u32BufCount = RKADK_PARAM_GetStreamBufCnt(u32CamId, true);
  pstAencAttr->stCodecAttr.enType = pstAencAttr->enType;
  pstAencAttr->stCodecAttr.u32Channels = pstAudioParam->channels;
  pstAencAttr->stCodecAttr.u32SampleRate = pstAudioParam->samplerate;
  pstAencAttr->stCodecAttr.enBitwidth = pstAudioParam->bit_width;
  pstAencAttr->stCodecAttr.pstResv = RK_NULL;

  return 0;
}

static RKADK_S32 RKADK_RTSP_EnableAudio(RKADK_U32 u32CamId, MPP_CHN_S stAiChn,
                                        MPP_CHN_S stAencChn, RKADK_PARAM_AUDIO_CFG_S *pstAudioParam) {
  int ret;
  AIO_ATTR_S stAiAttr;
  AENC_CHN_ATTR_S stAencAttr;

  // Create AI and AENC
  ret = RKADK_RTSP_SetAiAttr(&stAiAttr, pstAudioParam);
  if (ret) {
    RKADK_LOGE("RKADK_RTMP_SetAiAttr failed");
    return -1;
  }

  ret = RKADK_MPI_AI_Init(0, stAiChn.s32ChnId, &stAiAttr, pstAudioParam->vqe_mode,
                          pstAudioParam->vqe_config_path, pstAudioParam->mic_type);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_AI_Init faile(%d)", ret);
    return -1;
  }

  if (RKADK_RTSP_SetAencAttr(u32CamId, pstAudioParam, &stAencAttr)) {
    RKADK_LOGE("RKADK_RTMP_SetAencAttr error");
    goto failed;
  }

  ret = RKADK_MPI_AENC_Init(stAencChn.s32ChnId, &stAencAttr);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_AENC_Init error %d", ret);
    goto failed;
  }

  return 0;
failed:
  RKADK_MPI_AI_DeInit(stAiChn.s32DevId, stAiChn.s32ChnId, pstAudioParam->vqe_mode);

  return ret;
}

static RKADK_S32 RKADK_RTSP_DisableAudio(MPP_CHN_S stAiChn, MPP_CHN_S stAencChn,
                                         RKADK_PARAM_AUDIO_CFG_S *pstAudioParam) {
  int ret;

  // Destroy AENC before AI
  ret = RKADK_MPI_AENC_DeInit(stAencChn.s32ChnId);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_AENC_DeInit failed(%d)", ret);
    return ret;
  }

  // Destroy AI
  ret = RKADK_MPI_AI_DeInit(stAiChn.s32DevId, stAiChn.s32ChnId, pstAudioParam->vqe_mode);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_AI_DeInit failed %d", ret);
    return ret;
  }

  return 0;
}

RKADK_S32 RKADK_RTSP_Init(RKADK_U32 u32CamId, RKADK_U32 port, const char *path,
                          RKADK_MW_PTR *ppHandle) {
  int ret = 0;
  bool bSysInit = false;
  bool bUseVpss = false;
  MPP_CHN_S stViChn, stVencChn, stSrcVpssChn, stDstVpssChn;
  MPP_CHN_S stAiChn, stAencChn;
  RKADK_STREAM_TYPE_E enType;
  VPSS_GRP_ATTR_S stGrpAttr;
  VPSS_CHN_ATTR_S stChnAttr;
  RKADK_RTSP_HANDLE_S *pHandle;

  RKADK_CHECK_CAMERAID(u32CamId, RKADK_FAILURE);
  RKADK_CHECK_POINTER(path, RKADK_FAILURE);

  RKADK_LOGI("Rtsp[%d, %d, %s] Init...", u32CamId, port, path);
  RKADK_BUFINFO("enter rtsp[%d]", u32CamId);

  if (*ppHandle) {
    RKADK_LOGE("rtsp handle has been created");
    return -1;
  }

  bSysInit = RKADK_MPI_SYS_CHECK();
  if (!bSysInit) {
    RKADK_LOGE("System is not initialized");
    return -1;
  }

  RKADK_PARAM_STREAM_CFG_S *pstLiveCfg =
      RKADK_PARAM_GetStreamCfg(u32CamId, RKADK_STREAM_TYPE_LIVE);
  if (!pstLiveCfg) {
    RKADK_LOGE("Live RKADK_PARAM_GetStreamCfg Live failed");
    return -1;
  }

  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg = RKADK_PARAM_GetSensorCfg(u32CamId);
  if (!pstSensorCfg) {
    RKADK_LOGE("RKADK_PARAM_GetSensorCfg failed");
    return -1;
  }

  RKADK_PARAM_AUDIO_CFG_S *pstAudioCfg = RKADK_PARAM_GetAudioCfg();
  if (!pstAudioCfg) {
    RKADK_LOGE("RKADK_PARAM_GetAudioCfg failed");
    return -1;
  }

  pHandle = (RKADK_RTSP_HANDLE_S *)malloc(sizeof(RKADK_RTSP_HANDLE_S));
  if (!pHandle) {
    RKADK_LOGE("malloc pHandle failed");
    return -1;
  }
  memset(pHandle, 0, sizeof(RKADK_RTSP_HANDLE_S));
  pHandle->u32CamId = u32CamId;
  pHandle->bFirstKeyFrame = true;

  ret = RKADK_RTSP_InitService(pstLiveCfg->attribute.codec_type, port, path,
                               pHandle);
  if (ret) {
    RKADK_LOGE("RKADK_RTSP_InitService failed");
    free(pHandle);
    return -1;
  }

  RKADK_RTSP_AudioSetChn(&stAiChn, &stAencChn);
  ret = RKADK_RTSP_EnableAudio(u32CamId, stAiChn, stAencChn, pstAudioCfg);
  if (ret) {
    RKADK_LOGE("RKADK_RTSP_EnableAudio failed[%d]", ret);
    free(pHandle);
    return ret;
  }

  ret = RKADK_RTSP_AencGetData(u32CamId, &stAencChn, pHandle);
  if (ret) {
    RKADK_LOGE("RKADK_RTSP_AencGetData failed(%d)", ret);
    goto failed;
  }

  RKADK_RTSP_SetVideoChn(pstLiveCfg, u32CamId, &stViChn, &stVencChn, &stSrcVpssChn, &stDstVpssChn);
  enType = RKADK_PARAM_VencChnMux(u32CamId, stVencChn.s32ChnId);
  if (enType != RKADK_STREAM_TYPE_BUTT && enType != RKADK_STREAM_TYPE_LIVE) {
    switch (enType) {
    case RKADK_STREAM_TYPE_VIDEO_MAIN:
      RKADK_LOGI("Live and Record main venc[%d] mux", stVencChn.s32ChnId);
      break;
    case RKADK_STREAM_TYPE_VIDEO_SUB:
      RKADK_LOGI("Live and Record sub venc[%d] mux", stVencChn.s32ChnId);
      break;
    case RKADK_STREAM_TYPE_PREVIEW:
      RKADK_LOGI("Live and Preview venc[%d] mux", stVencChn.s32ChnId);
      break;
    default:
      RKADK_LOGW("Invaild venc[%d] mux, enType[%d]", stVencChn.s32ChnId,
                 enType);
      break;
    }
    pHandle->bVencChnMux = true;
  }

  // Create VI
  ret = RKADK_MPI_VI_Init(u32CamId, stViChn.s32ChnId,
                          &(pstLiveCfg->vi_attr.stChnAttr));
  if (ret) {
    RKADK_LOGE("RKADK_MPI_VI_Init faled %d", ret);
    goto failed;
  }
  RKADK_BUFINFO("create vi[%d]", stViChn.s32ChnId);

  bUseVpss = RKADK_RTSP_IsUseVpss(u32CamId, pstLiveCfg);
  if (bUseVpss) {
    memset(&stGrpAttr, 0, sizeof(VPSS_GRP_ATTR_S));
    memset(&stChnAttr, 0, sizeof(VPSS_CHN_ATTR_S));

    stGrpAttr.u32MaxW = pstSensorCfg->max_width;
    stGrpAttr.u32MaxH = pstSensorCfg->max_height;
    stGrpAttr.enPixelFormat = pstLiveCfg->vi_attr.stChnAttr.enPixelFormat;
    stGrpAttr.enCompressMode = COMPRESS_MODE_NONE;
    stGrpAttr.stFrameRate.s32SrcFrameRate = -1;
    stGrpAttr.stFrameRate.s32DstFrameRate = -1;
    stChnAttr.enChnMode = VPSS_CHN_MODE_USER;
    stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
    stChnAttr.enDynamicRange = DYNAMIC_RANGE_SDR8;
    stChnAttr.enPixelFormat = pstLiveCfg->vi_attr.stChnAttr.enPixelFormat;
    stChnAttr.stFrameRate.s32SrcFrameRate = -1;
    stChnAttr.stFrameRate.s32DstFrameRate = -1;
    stChnAttr.u32Width = pstLiveCfg->attribute.width;
    stChnAttr.u32Height = pstLiveCfg->attribute.height;
    stChnAttr.u32Depth = 0;
      stChnAttr.u32FrameBufCnt = pstLiveCfg->vi_attr.stChnAttr.stIspOpt.u32BufCount + 2;

    ret = RKADK_MPI_VPSS_Init(stSrcVpssChn.s32DevId, stSrcVpssChn.s32ChnId,
                              &stGrpAttr, &stChnAttr);
    if (ret) {
      RKADK_LOGE("RKADK_MPI_VPSS_Init falied[%d]",ret);
      goto failed;
    }
    RKADK_BUFINFO("create vpss[%d, %d]", stSrcVpssChn.s32DevId, stSrcVpssChn.s32ChnId);
  }

  // Create VENC
  VENC_CHN_ATTR_S stVencChnAttr;
  ret = RKADK_RTSP_SetVencAttr(u32CamId, pstLiveCfg, &stVencChnAttr);
  if (ret) {
    RKADK_LOGE("RKADK_RTSP_SetVencAttr failed");
    goto failed;
  }

  ret = RKADK_MPI_VENC_Init(u32CamId, stVencChn.s32ChnId, &stVencChnAttr);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_VENC_Init failed(%d)", ret);
    goto failed;
  }
  RKADK_BUFINFO("create venc[%d]", stVencChn.s32ChnId);

  RK_MPI_VENC_SetSceneMode(stVencChn.s32ChnId, RKADK_ENCODE_SENSE_CVR);
  RKADK_PARAM_SetVAdvancedParam(pstLiveCfg->attribute);

  //if use isp, set mirror/flip using aiq
  if (!pstSensorCfg->used_isp) {
    RKADK_STREAM_TYPE_E enStrmType = RKADK_STREAM_TYPE_LIVE;
    if (pstSensorCfg->mirror)
      RKADK_MEDIA_ToggleVencMirror(u32CamId, enStrmType, pstSensorCfg->mirror);
    if (pstSensorCfg->flip)
      RKADK_MEDIA_ToggleVencFlip(u32CamId, enStrmType, pstSensorCfg->flip);
  }

  ret = RKADK_RTSP_VencGetData(u32CamId, &stVencChn, pHandle);
  if (ret) {
    RKADK_LOGE("RKADK_RTSP_VencGetData failed(%d)", ret);
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
    // VI Bind VENC
    ret = RKADK_MPI_SYS_Bind(&stViChn, &stVencChn);
    if (ret) {
      RKADK_LOGE("Bind VI[%d] to VENC[%d] failed[%x]", stViChn.s32ChnId,
                  stVencChn.s32ChnId, ret);
      goto failed;
    }
  }
  RKADK_BUFINFO("rtsp bind[%d, %d, %d]", stViChn.s32ChnId, stSrcVpssChn.s32ChnId, stVencChn.s32ChnId);

  // Bind AI to AENC
  ret = RKADK_MPI_SYS_Bind(&stAiChn, &stAencChn);
  if (ret) {
    RKADK_LOGE("Bind AI[%d] and AENC[%d] failed[%d]", stAiChn.s32ChnId,
               stAencChn.s32ChnId, ret);
    goto unbind;
  }

  *ppHandle = (RKADK_MW_PTR)pHandle;
  RKADK_LOGI("Rtsp[%d, %d, %s] Init End...", u32CamId, port, path);
  return 0;

unbind:
  if (bUseVpss) {
    RKADK_MPI_SYS_UnBind(&stSrcVpssChn, &stVencChn);
    RKADK_MPI_SYS_UnBind(&stViChn, &stSrcVpssChn);
  } else {
    RKADK_MPI_SYS_UnBind(&stViChn, &stVencChn);
  }

failed:
  RKADK_LOGE("failed");
  RKADK_MPI_VENC_DeInit(stVencChn.s32ChnId);

  RKADK_MPI_VPSS_DeInit(stSrcVpssChn.s32DevId, stSrcVpssChn.s32ChnId);

  RKADK_MPI_VI_DeInit(u32CamId, stViChn.s32ChnId);

  RKADK_RTSP_DisableAudio(stAiChn, stAencChn, pstAudioCfg);

  if (pHandle) {
    RKADK_RTSP_DeInitService((RKADK_RTSP_HANDLE_S *)pHandle);
    free(pHandle);
  }

  return ret;
}

RKADK_S32 RKADK_RTSP_DeInit(RKADK_MW_PTR pHandle) {
  int ret = 0;
  bool bUseVpss = false;
  MPP_CHN_S stViChn, stVencChn, stSrcVpssChn, stDstVpssChn;
  MPP_CHN_S stAiChn, stAencChn;

  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);
  RKADK_RTSP_HANDLE_S *pstHandle = (RKADK_RTSP_HANDLE_S *)pHandle;

  RKADK_LOGI("Rtsp[%d] DeInit...", pstHandle->u32CamId);

  RKADK_PARAM_STREAM_CFG_S *pstLiveCfg =
      RKADK_PARAM_GetStreamCfg(pstHandle->u32CamId, RKADK_STREAM_TYPE_LIVE);
  if (!pstLiveCfg) {
    RKADK_LOGE("RKADK_PARAM_GetStreamCfg failed");
    return -1;
  }

  RKADK_PARAM_AUDIO_CFG_S *pstAudioCfg = RKADK_PARAM_GetAudioCfg();
  if (!pstAudioCfg) {
    RKADK_LOGE("RKADK_PARAM_GetAudioCfg failed");
    return -1;
  }

  RKADK_RTSP_SetVideoChn(pstLiveCfg, pstHandle->u32CamId, &stViChn, &stVencChn,
                         &stSrcVpssChn, &stDstVpssChn);

  // exit get media buffer
  if (pstHandle->bVencChnMux)
    RKADK_MEDIA_StopGetVencBuffer(&stVencChn, RKADK_RTSP_VencOutCb, pstHandle);

   // Stop get aenc data
  RKADK_RTSP_AudioSetChn(&stAiChn, &stAencChn);
  RKADK_MEDIA_StopGetAencBuffer(&stAencChn, RKADK_RTSP_AencOutCb, pstHandle);

  bUseVpss = RKADK_RTSP_IsUseVpss(pstHandle->u32CamId, pstLiveCfg);
  if (bUseVpss){
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
      RKADK_LOGE("UnBind VI[%d] to VENC[%d] failed[%x]", stViChn.s32ChnId,
                stVencChn.s32ChnId, ret);
      return ret;
    }
  }

  // destroy venc before vi
  ret = RKADK_MPI_VENC_DeInit(stVencChn.s32ChnId);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_VENC_DeInit failed(%d)", ret);
    return ret;
  }

  // Destory VPSS
  if (bUseVpss) {
    ret = RKADK_MPI_VPSS_DeInit(stSrcVpssChn.s32DevId, stSrcVpssChn.s32ChnId);
    if (ret) {
      RKADK_LOGE("DeInit VPSS[%d] failed[%x]", stSrcVpssChn.s32ChnId, ret);
      return ret;
    }
  }

  // destroy vi
  ret = RKADK_MPI_VI_DeInit(pstHandle->u32CamId, stViChn.s32ChnId);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_VI_DeInit failed %d", ret);
    return ret;
  }

  ret = RKADK_MPI_SYS_UnBind(&stAiChn, &stAencChn);
  if (ret) {
    RKADK_LOGE("UnBind AI[%d] and AENC[%d] failed[%d]", stAiChn.s32ChnId,
               stAencChn.s32ChnId, ret);
    return ret;
  }

  ret = RKADK_RTSP_DisableAudio(stAiChn, stAencChn, pstAudioCfg);
  if (ret) {
    RKADK_LOGE("RKADK_RTSP_DisableAudio failed(%d)", ret);
    return ret;
  }

  RKADK_RTSP_DeInitService((RKADK_RTSP_HANDLE_S *)pHandle);

  RKADK_LOGI("Rtsp[%d] DeInit End...", pstHandle->u32CamId);
  free(pHandle);
  return 0;
}

RKADK_S32 RKADK_RTSP_Start(RKADK_MW_PTR pHandle) {
  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);

  RKADK_RTSP_HANDLE_S *pstHandle = (RKADK_RTSP_HANDLE_S *)pHandle;
  if (pstHandle->start)
    return 0;

  RKADK_PARAM_STREAM_CFG_S *pstLiveCfg =
      RKADK_PARAM_GetStreamCfg(pstHandle->u32CamId, RKADK_STREAM_TYPE_LIVE);
  if (!pstLiveCfg) {
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
  stRecvParam.s32RecvPicNum = -1;
  return RK_MPI_VENC_StartRecvFrame(pstLiveCfg->attribute.venc_chn,
                                    &stRecvParam);
}

RKADK_S32 RKADK_RTSP_Stop(RKADK_MW_PTR pHandle) {
  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);

  RKADK_RTSP_HANDLE_S *pstHandle = (RKADK_RTSP_HANDLE_S *)pHandle;
  if (!pstHandle->start)
    return 0;

  RKADK_PARAM_STREAM_CFG_S *pstLiveCfg =
      RKADK_PARAM_GetStreamCfg(pstHandle->u32CamId, RKADK_STREAM_TYPE_LIVE);
  if (!pstLiveCfg) {
    RKADK_LOGE("RKADK_PARAM_GetStreamCfg failed");
    return -1;
  }

  pstHandle->start = false;

  // multiplex venc chn, thread get mediabuffer
  if (pstHandle->bVencChnMux)
    return 0;

  VENC_RECV_PIC_PARAM_S stRecvParam;
  stRecvParam.s32RecvPicNum = 0;
  return RK_MPI_VENC_StartRecvFrame(pstLiveCfg->attribute.venc_chn,
                                    &stRecvParam);
}
