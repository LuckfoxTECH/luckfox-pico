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

#include "rkadk_rtmp.h"
#include "rkmuxer.h"
#include "rkadk_log.h"
#include "rkadk_media_comm.h"
#include "rkadk_param.h"
#include "rkadk_audio_encoder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  bool bVencChnMux;
  RKADK_U32 u32CamId;
  RKADK_U32 u32MuxerId;
  VideoParam stVideo;
  AudioParam stAudio;
} RKADK_RTMP_HANDLE_S;

static void RKADK_RTMP_AudioSetChn(MPP_CHN_S *pstAiChn, MPP_CHN_S *pstAencChn) {
  pstAiChn->enModId = RK_ID_AI;
  pstAiChn->s32DevId = 0;
  pstAiChn->s32ChnId = LIVE_AI_CHN;

  pstAencChn->enModId = RK_ID_AENC;
  pstAencChn->s32DevId = 0;
  pstAencChn->s32ChnId = LIVE_AENC_CHN;
}

static RKADK_S32 RKADK_RTMP_SetAiAttr(AIO_ATTR_S *pstAiAttr,
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

static RKADK_S32 RKADK_RTMP_SetAencAttr(RKADK_U32 u32CamId,
                                        RKADK_PARAM_AUDIO_CFG_S *pstAudioParam,
                                        AENC_CHN_ATTR_S *pstAencAttr) {

  RKADK_CHECK_POINTER(pstAudioParam, RKADK_FAILURE);
  RKADK_CHECK_POINTER(pstAencAttr, RKADK_FAILURE);

  memset(pstAencAttr, 0, sizeof(AENC_CHN_ATTR_S));
  pstAencAttr->enType = RKADK_MEDIA_GetRkCodecType(pstAudioParam->codec_type);
  pstAencAttr->u32BufCount = RKADK_PARAM_GetStreamBufCnt(u32CamId, true);
  pstAencAttr->stCodecAttr.enType = pstAencAttr->enType;
  pstAencAttr->stCodecAttr.u32Channels = pstAudioParam->channels;
  pstAencAttr->stCodecAttr.u32SampleRate = pstAudioParam->samplerate;
  pstAencAttr->stCodecAttr.enBitwidth = pstAudioParam->bit_width;
  pstAencAttr->stCodecAttr.pstResv = RK_NULL;

  if (pstAudioParam->codec_type == RKADK_CODEC_TYPE_ACC){
    pstAencAttr->stCodecAttr.u32Resv[0] = 2;
    pstAencAttr->stCodecAttr.u32Resv[1] = pstAudioParam->bitrate;
  }
  return 0;
}

static void RKADK_RTMP_SetVideoChn(RKADK_PARAM_STREAM_CFG_S *pstLiveCfg, RKADK_U32 u32CamId,
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

static int RKADK_RTMP_SetVencAttr(RKADK_U32 u32CamId,
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
    RKADK_LOGW("CamId[%d] rtmp fps[%d] > sensor fps[%d], use sensor fps",
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

static int RKADK_RTMP_SetMuxerAttr(RKADK_U32 u32CamId,
                                   RKADK_PARAM_STREAM_CFG_S *pstLiveCfg,
                                   RKADK_PARAM_AUDIO_CFG_S *pstAudioParam,
                                   RKADK_RTMP_HANDLE_S *pHandle) {
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg = NULL;
  bool bEnableAudio = pstAudioParam->codec_type == RKADK_CODEC_TYPE_ACC ? true : false;

  pstSensorCfg = RKADK_PARAM_GetSensorCfg(u32CamId);
  if (!pstSensorCfg) {
    RKADK_LOGE("RKADK_PARAM_GetSensorCfg failed");
    return -1;
  }

  pHandle->u32MuxerId =
      RECORD_FILE_NUM_MAX * RKADK_MAX_SENSOR_CNT + u32CamId;
  pHandle->stVideo.width = pstLiveCfg->attribute.width;
  pHandle->stVideo.height = pstLiveCfg->attribute.height;
  pHandle->stVideo.bit_rate = pstLiveCfg->attribute.bitrate;
  pHandle->stVideo.frame_rate_den = 1;
  pHandle->stVideo.frame_rate_num = pstLiveCfg->attribute.framerate;
  pHandle->stVideo.profile = pstLiveCfg->attribute.profile;
  pHandle->stVideo.level = 41;

  switch (pstLiveCfg->attribute.codec_type) {
  case RKADK_CODEC_TYPE_H264:
    memcpy(pHandle->stVideo.codec, "H.264", strlen("H.264"));
    break;
  case RKADK_CODEC_TYPE_H265:
    memcpy(pHandle->stVideo.codec, "H.265", strlen("H.265"));
    break;
  default:
    RKADK_LOGE("not support enCodecType: %d", pstLiveCfg->attribute.codec_type);
    return -1;
  }

  if (!bEnableAudio)
    return 0;

  pHandle->stAudio.channels = pstAudioParam->channels;
  pHandle->stAudio.frame_size = pstAudioParam->samples_per_frame;
  pHandle->stAudio.sample_rate = pstAudioParam->samplerate;

  switch (RKADK_MEDIA_GetAudioBitWidth(pstAudioParam->bit_width)) {
  case 16:
    memcpy(pHandle->stAudio.format, "S16", strlen("S16"));
    break;
  case 32:
    memcpy(pHandle->stAudio.format, "S32", strlen("S32"));
    break;
  default:
    RKADK_LOGE("not support u32BitWidth: %d", pstAudioParam->bit_width);
    return -1;
  }

  switch (pstAudioParam->codec_type) {
  case RKADK_CODEC_TYPE_MP3:
    memcpy(pHandle->stAudio.codec, "MP3", strlen("MP3"));
    break;
  case RKADK_CODEC_TYPE_ACC:
    memcpy(pHandle->stAudio.codec, "ACC", strlen("ACC"));
    break;
  case RKADK_CODEC_TYPE_MP2:
    memcpy(pHandle->stAudio.codec, "MP2", strlen("MP2"));
    break;
  default:
    RKADK_LOGE("not support enCodecType: %d", pstAudioParam->codec_type);
    return -1;
  }
  return 0;
}

static bool RKADK_RTMP_IsUseVpss(RKADK_U32 u32CamId, RKADK_PARAM_STREAM_CFG_S *pstLiveCfg) {
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

static RKADK_S32 RKADK_RTMP_EnableVideo(RKADK_U32 u32CamId, MPP_CHN_S stViChn,
                                        MPP_CHN_S stVencChn, MPP_CHN_S stSrcVpssChn,
                                        RKADK_PARAM_STREAM_CFG_S *pstLiveCfg,
                                        bool bUseVpss) {
  int ret = 0;
  VENC_CHN_ATTR_S stVencChnAttr;
  VPSS_GRP_ATTR_S stGrpAttr;
  VPSS_CHN_ATTR_S stChnAttr;

  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg = RKADK_PARAM_GetSensorCfg(u32CamId);
  if (!pstSensorCfg) {
    RKADK_LOGE("RKADK_PARAM_GetSensorCfg failed");
    return -1;
  }

  // Create VI
  ret = RKADK_MPI_VI_Init(u32CamId, stViChn.s32ChnId,
                          &(pstLiveCfg->vi_attr.stChnAttr));
  if (ret) {
    RKADK_LOGE("RKADK_MPI_VI_Init faled %d", ret);
    return ret;
  }
  RKADK_BUFINFO("create vi[%d]", stViChn.s32ChnId);

  // Create VPSS
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

    ret = RKADK_MPI_VPSS_Init(pstLiveCfg->attribute.vpss_grp, pstLiveCfg->attribute.vpss_chn,
                              &stGrpAttr, &stChnAttr);
    if (ret) {
      RKADK_LOGE("RKADK_MPI_VPSS_Init vpssfalied[%d]",ret);
      RKADK_MPI_VI_DeInit(u32CamId, pstLiveCfg->vi_attr.u32ViChn);
      RKADK_MPI_VPSS_DeInit(pstLiveCfg->attribute.vpss_grp, pstLiveCfg->attribute.vpss_chn);
      return ret;
    }
    RKADK_BUFINFO("create vpss[%d, %d]", pstLiveCfg->attribute.vpss_grp, pstLiveCfg->attribute.vpss_chn);
  }

  // Create VENC
  ret = RKADK_RTMP_SetVencAttr(u32CamId, pstLiveCfg, &stVencChnAttr);
  if (ret) {
    RKADK_LOGE("RKADK_RTMP_SetVencAttr failed");
    goto failed;
  }

  ret = RKADK_MPI_VENC_Init(u32CamId, stVencChn.s32ChnId, &stVencChnAttr);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_VENC_Init failed(%d)", ret);
    goto failed;
  }
  RKADK_BUFINFO("create venc[%d]", stVencChn.s32ChnId);

  RKADK_PARAM_SetVAdvancedParam(pstLiveCfg->attribute);

  //if use isp, set mirror/flip using aiq
  if (!pstSensorCfg->used_isp) {
    RKADK_STREAM_TYPE_E enStrmType = RKADK_STREAM_TYPE_LIVE;
    if (pstSensorCfg->mirror)
      RKADK_MEDIA_ToggleVencMirror(u32CamId, enStrmType, pstSensorCfg->mirror);
    if (pstSensorCfg->flip)
      RKADK_MEDIA_ToggleVencFlip(u32CamId, enStrmType, pstSensorCfg->flip);
  }

  return 0;

failed:
  if (bUseVpss)
    RKADK_MPI_VPSS_DeInit(pstLiveCfg->attribute.vpss_grp, pstLiveCfg->attribute.vpss_chn);

  RKADK_MPI_VI_DeInit(u32CamId, stViChn.s32ChnId);
  return ret;
}

static RKADK_S32 RKADK_RTMP_DisableVideo(RKADK_U32 u32CamId, MPP_CHN_S stViChn,
                                         MPP_CHN_S stVencChn, MPP_CHN_S stSrcVpssChn,
                                         RKADK_PARAM_STREAM_CFG_S *pstLiveCfg, bool bUseVpss) {
  int ret;

  // Destroy VENC before VI
  ret = RKADK_MPI_VENC_DeInit(stVencChn.s32ChnId);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_VENC_DeInit failed(%d)", ret);
    return ret;
  }

  // Destory VPSS
  if (bUseVpss) {
    ret = RKADK_MPI_VPSS_DeInit(pstLiveCfg->attribute.vpss_grp, stSrcVpssChn.s32ChnId);
    if (ret) {
      RKADK_LOGE("DeInit VPSS[%d] failed[%d]", stSrcVpssChn.s32ChnId, ret);
      return ret;
    }
  }

  // Destroy VI
  ret = RKADK_MPI_VI_DeInit(u32CamId, stViChn.s32ChnId);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_VI_DeInit failed %d", ret);
    return ret;
  }

  return 0;
}

static RKADK_S32 RKADK_RTMP_EnableAudio(RKADK_U32 u32CamId, MPP_CHN_S stAiChn,
                                        MPP_CHN_S stAencChn, RKADK_PARAM_AUDIO_CFG_S *pstAudioParam) {
  int ret;
  AIO_ATTR_S stAiAttr;
  AENC_CHN_ATTR_S stAencAttr;

  if (RKADK_MEDIA_EnableAencRegister(pstAudioParam->codec_type)) {
    ret = RKADK_AUDIO_ENCODER_Register(pstAudioParam->codec_type);
    if (ret) {
      RKADK_LOGE("RKADK_AUDIO_ENCODER_Register failed(%d)", ret);
      return ret;
    }
  }

  // Create AI and AENC
  ret = RKADK_RTMP_SetAiAttr(&stAiAttr, pstAudioParam);
  if (ret) {
    RKADK_LOGE("RKADK_RTMP_SetAiAttr failed");
    goto unregist;
  }

  ret = RKADK_MPI_AI_Init(0, stAiChn.s32ChnId, &stAiAttr, pstAudioParam->vqe_mode,
                          pstAudioParam->vqe_config_path, pstAudioParam->mic_type);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_AI_Init faile(%d)", ret);
    goto unregist;
  }

  if (RKADK_RTMP_SetAencAttr(u32CamId, pstAudioParam, &stAencAttr)) {
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

unregist:
    if (RKADK_MEDIA_EnableAencRegister(pstAudioParam->codec_type))
      RKADK_AUDIO_ENCODER_UnRegister(pstAudioParam->codec_type);

  return ret;
}

static RKADK_S32 RKADK_RTMP_DisableAudio(MPP_CHN_S stAiChn, MPP_CHN_S stAencChn,
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

  if (RKADK_MEDIA_EnableAencRegister(pstAudioParam->codec_type)) {
    ret = RKADK_AUDIO_ENCODER_UnRegister(pstAudioParam->codec_type);
    if (ret) {
      RKADK_LOGE("RKADK_AUDIO_ENCODER_UnRegister failed(%d)", ret);
      return ret;
    }
  }

  return 0;
}

static RKADK_S32 RKADK_RTMP_InitMuxer(RKADK_U32 u32CamId, char *path,
                                      RKADK_PARAM_STREAM_CFG_S *pstLiveCfg,
                                      RKADK_PARAM_AUDIO_CFG_S *pstAudioParam,
                                      RKADK_RTMP_HANDLE_S *pHandle) {
  int ret = 0;

  ret = RKADK_RTMP_SetMuxerAttr(u32CamId, pstLiveCfg, pstAudioParam, pHandle);
  if (ret) {
    RKADK_LOGE("RKADK_RTMP_SetMuxerAttr failed");
    return ret;
  }

  ret = rkmuxer_init(pHandle->u32MuxerId, (char *)"flv", path,
                     &pHandle->stVideo, &pHandle->stAudio);
  if (ret) {
    RKADK_LOGE("rkmuxer_init failed[%d]", ret);
    return -1;
  }

  return 0;
}

static void RKADK_RTMP_AencOutCb(AUDIO_STREAM_S stFrame,
                                 RKADK_VOID *pHandle) {
  int headerSize = 7;
  RKADK_CHECK_POINTER_N(pHandle);
  RKADK_RTMP_HANDLE_S *pstHandle = (RKADK_RTMP_HANDLE_S *)pHandle;
  RKADK_U8 *data;
  if (!pstHandle) {
    RKADK_LOGE("Can't find rtmp handle");
    return;
  }

  data = (RKADK_U8 *)RK_MPI_MB_Handle2VirAddr(stFrame.pMbBlk);
  rkmuxer_write_audio_frame(pstHandle->u32MuxerId, data + headerSize,
                            stFrame.u32Len - headerSize, stFrame.u64TimeStamp);
}

static int RKADK_RTMP_AencGetData(RKADK_U32 u32CamId,
                                  MPP_CHN_S *pstAencChn,
                                  RKADK_RTMP_HANDLE_S *pHandle) {
  int ret;

  ret = RKADK_MEDIA_GetAencBuffer(pstAencChn, RKADK_RTMP_AencOutCb,
                                  (RKADK_VOID *)pHandle);
  if (ret) {
    RKADK_LOGE("RKADK_MEDIA_GetVencBuffer failed = %d", ret);
    return ret;
  }

  return 0;
}

static void RKADK_RTMP_VencOutCb(RKADK_MEDIA_VENC_DATA_S stData,
                                 RKADK_VOID *pHandle) {
  RKADK_CHECK_POINTER_N(pHandle);
  RKADK_RTMP_HANDLE_S *pstHandle = (RKADK_RTMP_HANDLE_S *)pHandle;
  RKADK_U8 *data;
  if (!pstHandle) {
    RKADK_LOGE("Can't find rtmp handle");
    return;
  }

  data = (RKADK_U8 *)RK_MPI_MB_Handle2VirAddr(stData.stFrame.pstPack->pMbBlk);
  if ((stData.stFrame.pstPack->DataType.enH264EType == H264E_NALU_ISLICE ||
    stData.stFrame.pstPack->DataType.enH264EType == H264E_NALU_IDRSLICE) ||
    (stData.stFrame.pstPack->DataType.enH265EType == H265E_NALU_ISLICE ||
    stData.stFrame.pstPack->DataType.enH265EType == H265E_NALU_IDRSLICE)) {
    rkmuxer_write_video_frame(pstHandle->u32MuxerId, data,
                              stData.stFrame.pstPack->u32Len,
                              stData.stFrame.pstPack->u64PTS, 1);
  } else {
    rkmuxer_write_video_frame(pstHandle->u32MuxerId, data,
                              stData.stFrame.pstPack->u32Len,
                              stData.stFrame.pstPack->u64PTS, 0);
  }
}

static int RKADK_RTMP_VencGetData(RKADK_U32 u32CamId,
                                  MPP_CHN_S *pstVencChn,
                                  RKADK_RTMP_HANDLE_S *pHandle) {
  int ret = 0;

  VENC_RECV_PIC_PARAM_S stRecvParam;
  stRecvParam.s32RecvPicNum = -1;
  ret = RK_MPI_VENC_StartRecvFrame(pstVencChn->s32ChnId, &stRecvParam);
  if (ret) {
    RKADK_LOGE("RK_MPI_VENC_StartRecvFrame failed = %d", ret);
    return ret;
  }

  ret = RKADK_MEDIA_GetVencBuffer(pstVencChn, RKADK_RTMP_VencOutCb,
                                  (RKADK_VOID *)pHandle);
  if (ret) {
    RKADK_LOGE("RKADK_MEDIA_GetVencBuffer failed = %d", ret);
    return ret;
  }

  return 0;
}

RKADK_S32 RKADK_RTMP_Init(RKADK_U32 u32CamId, const char *path,
                          RKADK_MW_PTR *ppHandle) {
  int ret = 0;
  bool bSysInit = false;
  bool bEnableAudio, bUseVpss = false;
  MPP_CHN_S stAiChn, stAencChn;
  MPP_CHN_S stViChn, stVencChn, stSrcVpssChn, stDstVpssChn;
  RKADK_RTMP_HANDLE_S *pHandle;
  RKADK_STREAM_TYPE_E enType;

  RKADK_CHECK_CAMERAID(u32CamId, RKADK_FAILURE);
  RKADK_CHECK_POINTER(path, RKADK_FAILURE);

  RKADK_LOGI("Rtmp[%d, %s] Init...", u32CamId, path);
  RKADK_BUFINFO("enter rtmp[%d]", u32CamId);

  if (*ppHandle) {
    RKADK_LOGE("rtmp handle has been created");
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

  bUseVpss = RKADK_RTMP_IsUseVpss(u32CamId, pstLiveCfg);

  RKADK_PARAM_AUDIO_CFG_S *pstAudioParam = RKADK_PARAM_GetAudioCfg();
  if (!pstAudioParam) {
    RKADK_LOGE("RKADK_PARAM_GetAudioCfg failed");
    return -1;
  }

  /* mp3 not compatible with flv */
  bEnableAudio = pstAudioParam->codec_type == RKADK_CODEC_TYPE_ACC ? true : false;

  pHandle = (RKADK_RTMP_HANDLE_S *)malloc(sizeof(RKADK_RTMP_HANDLE_S));
  if (!pHandle) {
    RKADK_LOGE("malloc pHandle failed");
    return -1;
  }
  memset(pHandle, 0, sizeof(RKADK_RTMP_HANDLE_S));
  pHandle->u32CamId = u32CamId;

  // Create Muxer
  ret = RKADK_RTMP_InitMuxer(u32CamId, (char *)path, pstLiveCfg, pstAudioParam, pHandle);
  if (ret) {
    RKADK_LOGE("RKADK_RTMP_InitMuxer failed");
    free(pHandle);
    return ret;
  }

  RKADK_RTMP_SetVideoChn(pstLiveCfg, u32CamId, &stViChn, &stVencChn, &stSrcVpssChn, &stDstVpssChn);
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
  ret = RKADK_RTMP_EnableVideo(u32CamId, stViChn, stVencChn, stSrcVpssChn,
                               pstLiveCfg, bUseVpss);
  if (ret) {
    RKADK_LOGE("RKADK_RTMP_EnableVideo failed[%d]", ret);
    free(pHandle);
    return ret;
  }

  if (bEnableAudio) {
    RKADK_RTMP_AudioSetChn(&stAiChn, &stAencChn);
    ret = RKADK_RTMP_EnableAudio(u32CamId, stAiChn, stAencChn, pstAudioParam);
    if (ret) {
      RKADK_LOGE("RKADK_RTMP_EnableAudio failed[%d]", ret);
      RKADK_RTMP_DisableVideo(u32CamId, stViChn, stVencChn, stSrcVpssChn, pstLiveCfg, bUseVpss);
      free(pHandle);
      return ret;
    }

    ret = RKADK_RTMP_AencGetData(u32CamId, &stAencChn, pHandle);
    if (ret) {
      RKADK_LOGE("RKADK_RTMP_AencGetData failed(%d)", ret);
      goto failed;
    }
  }

  ret = RKADK_RTMP_VencGetData(u32CamId, &stVencChn, pHandle);
  if (ret) {
    RKADK_LOGE("RKADK_RTMP_VencGetData failed(%d)", ret);
    goto failed;
  }

  if (bUseVpss) {
    // VPSS Bind VENC
    ret = RKADK_MPI_SYS_Bind(&stSrcVpssChn, &stVencChn);
    if (ret) {
      RKADK_LOGE("Bind VPSS[%d] to VENC[%d] failed[%d]", stSrcVpssChn.s32ChnId,
                 stVencChn.s32ChnId, ret);
      goto failed;
    }

    // VI Bind VPSS
    ret = RKADK_MPI_SYS_Bind(&stViChn, &stDstVpssChn);
    if (ret) {
      RKADK_LOGE("Bind VI[%d] to VPSS[%d] failed[%d]", stViChn.s32ChnId,
                 stDstVpssChn.s32DevId, ret);
      RKADK_MPI_SYS_UnBind(&stSrcVpssChn, &stVencChn);
      goto failed;
    }
  } else {
    // Bind VI to VENC
    ret = RKADK_MPI_SYS_Bind(&stViChn, &stVencChn);
    if (ret) {
      RKADK_LOGE("Bind VI[%d] and VENC[%d] failed[%d]", stViChn.s32ChnId,
                 stVencChn.s32ChnId, ret);
      goto failed;
    }
  }
  RKADK_BUFINFO("rtmp bind[%d, %d, %d]", stViChn.s32ChnId, stSrcVpssChn.s32ChnId, stVencChn.s32ChnId);

  // Bind AI to AENC
  if (bEnableAudio) {
    ret = RKADK_MPI_SYS_Bind(&stAiChn, &stAencChn);
    if (ret) {
      RKADK_LOGE("Bind AI[%d] and AENC[%d] failed[%d]", stAiChn.s32ChnId,
                 stAencChn.s32ChnId, ret);
      goto unbind;
    }
  }

  *ppHandle = (RKADK_MW_PTR)pHandle;
  RKADK_LOGI("Rtmp[%d, %s] Init End...", u32CamId, path);
  return 0;

unbind:
  if (bUseVpss) {
    RKADK_MPI_SYS_UnBind(&stSrcVpssChn, &stVencChn);
    RKADK_MPI_SYS_UnBind(&stViChn, &stSrcVpssChn);
  } else {
    RKADK_MPI_SYS_UnBind(&stViChn, &stVencChn);
  }

failed:
  RKADK_RTMP_DisableVideo(u32CamId, stViChn, stVencChn, stSrcVpssChn, pstLiveCfg, bUseVpss);

  if (bEnableAudio)
    RKADK_RTMP_DisableAudio(stAiChn, stAencChn, pstAudioParam);

  if (pHandle)
    free(pHandle);

  RKADK_LOGE("failed");
  return ret;
}

RKADK_S32 RKADK_RTMP_DeInit(RKADK_MW_PTR pHandle) {
  int ret = 0;
  bool bDisableAudio, bUseVpss = false;
  MPP_CHN_S stAiChn, stAencChn;
  MPP_CHN_S stViChn, stVencChn, stSrcVpssChn, stDstVpssChn;

  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);
  RKADK_RTMP_HANDLE_S *pstHandle = (RKADK_RTMP_HANDLE_S *)pHandle;

  RKADK_LOGI("Rtmp[%d] DeInit...", pstHandle->u32CamId);

  RKADK_PARAM_STREAM_CFG_S *pstLiveCfg =
      RKADK_PARAM_GetStreamCfg(pstHandle->u32CamId, RKADK_STREAM_TYPE_LIVE);
  if (!pstLiveCfg) {
    RKADK_LOGE("RKADK_PARAM_GetStreamCfg failed");
    return -1;
  }

  RKADK_PARAM_AUDIO_CFG_S *pstAudioParam = RKADK_PARAM_GetAudioCfg();
  if (!pstAudioParam) {
    RKADK_LOGE("RKADK_PARAM_GetAudioCfg failed");
    return ret;
  }

  bDisableAudio =
      pstAudioParam->codec_type == RKADK_CODEC_TYPE_ACC ? true : false;

  RKADK_RTMP_SetVideoChn(pstLiveCfg, pstHandle->u32CamId, &stViChn, &stVencChn,
                         &stSrcVpssChn, &stDstVpssChn);

  // Stop get venc data
  RKADK_MEDIA_StopGetVencBuffer(&stVencChn, RKADK_RTMP_VencOutCb, pstHandle);

   // Stop get aenc data
  if (bDisableAudio) {
    RKADK_RTMP_AudioSetChn(&stAiChn, &stAencChn);
    RKADK_MEDIA_StopGetAencBuffer(&stAencChn, RKADK_RTMP_AencOutCb, pstHandle);
  }

  bUseVpss = RKADK_RTMP_IsUseVpss(pstHandle->u32CamId, pstLiveCfg);
  if (bUseVpss) {
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
                 stDstVpssChn.s32DevId, ret);
      return ret;
    }
  } else {
    // UnBind VI and VENC
    ret = RKADK_MPI_SYS_UnBind(&stViChn, &stVencChn);
    if (ret) {
      RKADK_LOGE("UnBind VI[%d] and VENC[%d] failed[%d]", stViChn.s32ChnId,
                 stVencChn.s32ChnId, ret);
      return ret;
    }
  }

  // UnBind AI and AENC
  if (bDisableAudio) {
    ret = RKADK_MPI_SYS_UnBind(&stAiChn, &stAencChn);
    if (ret) {
      RKADK_LOGE("UnBind AI[%d] and AENC[%d] failed[%d]", stAiChn.s32ChnId,
                 stAencChn.s32ChnId, ret);
      return ret;
    }
  }

  // Destroy MUXER
  rkmuxer_deinit(pstHandle->u32MuxerId);

  // Disable Video
  ret = RKADK_RTMP_DisableVideo(pstHandle->u32CamId, stViChn, stVencChn,
                                stSrcVpssChn, pstLiveCfg, bUseVpss);
  if (ret) {
    RKADK_LOGE("RKADK_RTMP_DisableVideo failed(%d)", ret);
    return ret;
  }

  // Disable Audio
  if (bDisableAudio) {
    RKADK_RTMP_AudioSetChn(&stAiChn, &stAencChn);
    ret = RKADK_RTMP_DisableAudio(stAiChn, stAencChn, pstAudioParam);
    if (ret) {
      RKADK_LOGE("RKADK_RTMP_DisableAudio failed(%d)", ret);
      return ret;
    }
  }

  RKADK_LOGI("Rtmp[%d] DeInit End...", pstHandle->u32CamId);
  free(pHandle);
  return 0;
}
