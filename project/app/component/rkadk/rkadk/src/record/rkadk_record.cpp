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

#include "rkadk_record.h"
#include "rkadk_log.h"
#include "rkadk_thumb_comm.h"
#include "rkadk_media_comm.h"
#include "rkadk_param.h"
#include "rkadk_audio_encoder.h"
#include <deque>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static RKADK_REC_REQUEST_FILE_NAMES_FN
    g_pfnRequestFileNames[RKADK_MAX_SENSOR_CNT] = {NULL};
static std::deque<char *>
    g_fileNameDeque[RKADK_MUXER_STREAM_MAX_CNT * RKADK_MAX_SENSOR_CNT];
static pthread_mutex_t g_fileNameMutexLock = PTHREAD_MUTEX_INITIALIZER;

static int GetRecordFileName(RKADK_VOID *pHandle, RKADK_CHAR *pcFileName,
                             RKADK_U32 muxerId) {
  int index, ret;
  RKADK_MUXER_HANDLE_S *pstRecorder;

  RKADK_MUTEX_LOCK(g_fileNameMutexLock);

  if (muxerId >= RKADK_MUXER_STREAM_MAX_CNT * RKADK_MAX_SENSOR_CNT) {
    RKADK_LOGE("Incorrect file index: %d", muxerId);
    RKADK_MUTEX_UNLOCK(g_fileNameMutexLock);
    return -1;
  }

  pstRecorder = (RKADK_MUXER_HANDLE_S *)pHandle;
  if (!pstRecorder) {
    RKADK_LOGE("pstRecorder is null");
    RKADK_MUTEX_UNLOCK(g_fileNameMutexLock);
    return -1;
  }

  if (!g_pfnRequestFileNames[pstRecorder->u32CamId]) {
    RKADK_LOGE("Not Registered request name callback");
    RKADK_MUTEX_UNLOCK(g_fileNameMutexLock);
    return -1;
  }

  if (g_fileNameDeque[muxerId].empty()) {
    ARRAY_FILE_NAME fileName;
    fileName = (ARRAY_FILE_NAME)malloc(pstRecorder->u32StreamCnt *
                                       RKADK_MAX_FILE_PATH_LEN);
    ret = g_pfnRequestFileNames[pstRecorder->u32CamId](
        pHandle, pstRecorder->u32StreamCnt, fileName);
    if (ret) {
      RKADK_LOGE("get file name failed(%d)", ret);
      free(fileName);
      RKADK_MUTEX_UNLOCK(g_fileNameMutexLock);
      return -1;
    }

    char *newName[pstRecorder->u32StreamCnt];
    for (int i = 0; i < (int)pstRecorder->u32StreamCnt; i++) {
      newName[i] = strdup(fileName[i]);
      index = i + (RKADK_MUXER_STREAM_MAX_CNT * pstRecorder->u32CamId);
      g_fileNameDeque[index].push_back(newName[i]);
    }
    free(fileName);
  }

  auto front = g_fileNameDeque[muxerId].front();
  strncpy(pcFileName, front, RKADK_MAX_FILE_PATH_LEN);
  g_fileNameDeque[muxerId].pop_front();
  free(front);

  RKADK_MUTEX_UNLOCK(g_fileNameMutexLock);
  return 0;
}

static RKADK_U32 GetPreRecordCacheTime(RKADK_PARAM_REC_CFG_S *pstRecCfg,
                                        RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg) {
  RKADK_U32 u32Integer = 0, u32Remainder = 0;
  RKADK_U32 u32PreRecCacheTime = 0;

  if(pstRecCfg->pre_record_time == 0)
    return 0;

  u32Integer = pstRecCfg->attribute[0].gop / pstSensorCfg->framerate;
  u32Remainder = pstRecCfg->attribute[0].gop % pstSensorCfg->framerate;
  u32PreRecCacheTime = pstRecCfg->pre_record_time + u32Integer;
  if (u32Remainder)
    u32PreRecCacheTime += 1;

  return u32PreRecCacheTime;
}

static void RKADK_RECORD_SetVideoChn(int index, RKADK_U32 u32CamId,
                                     RKADK_PARAM_REC_CFG_S *pstRecCfg,
                                     MPP_CHN_S *pstViChn, MPP_CHN_S *pstVencChn,
                                     MPP_CHN_S *pstSrcVpssChn, MPP_CHN_S *pstDstVpssChn) {
  pstViChn->enModId = RK_ID_VI;
  pstViChn->s32DevId = u32CamId;
  pstViChn->s32ChnId = pstRecCfg->vi_attr[index].u32ViChn;

  pstSrcVpssChn->enModId = RK_ID_VPSS;
  pstSrcVpssChn->s32DevId = pstRecCfg->attribute[index].vpss_grp;
  pstSrcVpssChn->s32ChnId = pstRecCfg->attribute[index].vpss_chn;

  pstDstVpssChn->enModId = RK_ID_VPSS;
  pstDstVpssChn->s32DevId = pstRecCfg->attribute[index].vpss_grp;
  pstDstVpssChn->s32ChnId = 0; //When vpss is dst, chn is equal to 0

  pstVencChn->enModId = RK_ID_VENC;
  pstVencChn->s32DevId = 0;
  pstVencChn->s32ChnId = pstRecCfg->attribute[index].venc_chn;
}

static void RKADK_RECORD_SetAudioChn(MPP_CHN_S *pstAiChn,
                                     MPP_CHN_S *pstAencChn) {
  pstAiChn->enModId = RK_ID_AI;
  pstAiChn->s32DevId = 0;
  pstAiChn->s32ChnId = RECORD_AI_CHN;

  pstAencChn->enModId = RK_ID_AENC;
  pstAencChn->s32DevId = 0;
  pstAencChn->s32ChnId = RECORD_AENC_CHN;
}

static int RKADK_RECORD_SetVideoAttr(int index, RKADK_U32 u32CamId,
                                     RKADK_PARAM_REC_CFG_S *pstRecCfg,
                                     VENC_CHN_ATTR_S *pstVencAttr) {
  int ret;
  RKADK_U32 u32Gop;
  RKADK_U32 u32DstFrameRateNum = 0;
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg = NULL;
  RKADK_U32 bitrate;

  RKADK_CHECK_POINTER(pstVencAttr, RKADK_FAILURE);

  pstSensorCfg = RKADK_PARAM_GetSensorCfg(u32CamId);
  if (!pstSensorCfg) {
    RKADK_LOGE("RKADK_PARAM_GetSensorCfg failed");
    return -1;
  }

  memset(pstVencAttr, 0, sizeof(VENC_CHN_ATTR_S));

  if(pstRecCfg->record_type == RKADK_REC_TYPE_LAPSE) {
    bitrate = pstRecCfg->attribute[index].bitrate / pstRecCfg->lapse_multiple;
    u32DstFrameRateNum = pstSensorCfg->framerate / pstRecCfg->lapse_multiple;
    if (u32DstFrameRateNum < 1)
      u32DstFrameRateNum = 1;
    else if (u32DstFrameRateNum > pstSensorCfg->framerate)
      u32DstFrameRateNum = pstSensorCfg->framerate;
  } else {
    bitrate = pstRecCfg->attribute[index].bitrate;
    u32DstFrameRateNum = pstSensorCfg->framerate;
  }
  u32Gop = pstRecCfg->attribute[index].gop;

  pstVencAttr->stRcAttr.enRcMode =
      RKADK_PARAM_GetRcMode(pstRecCfg->attribute[index].rc_mode,
                            pstRecCfg->attribute[index].codec_type);

  ret = RKADK_MEDIA_SetRcAttr(&pstVencAttr->stRcAttr, u32Gop, bitrate,
                              pstSensorCfg->framerate, u32DstFrameRateNum);
  if (ret) {
    RKADK_LOGE("RKADK_MEDIA_SetRcAttr failed");
    return -1;
  }

  pstVencAttr->stVencAttr.enType =
      RKADK_MEDIA_GetRkCodecType(pstRecCfg->attribute[index].codec_type);
  pstVencAttr->stVencAttr.enPixelFormat =
      pstRecCfg->vi_attr[index].stChnAttr.enPixelFormat;
  if (index == 0) {
    //main stream
    pstVencAttr->stVencAttr.u32MaxPicWidth = pstSensorCfg->max_width;
    pstVencAttr->stVencAttr.u32MaxPicHeight = pstSensorCfg->max_height;
  } else {
    //sub stream
    pstVencAttr->stVencAttr.u32MaxPicWidth = pstRecCfg->attribute[index].width;
    pstVencAttr->stVencAttr.u32MaxPicHeight = pstRecCfg->attribute[index].height;
  }
  pstVencAttr->stVencAttr.u32BufSize = pstRecCfg->attribute[index].bufsize;
  pstVencAttr->stVencAttr.u32PicWidth = pstRecCfg->attribute[index].width;
  pstVencAttr->stVencAttr.u32PicHeight = pstRecCfg->attribute[index].height;
  pstVencAttr->stVencAttr.u32VirWidth = pstRecCfg->attribute[index].width;
  pstVencAttr->stVencAttr.u32VirHeight = pstRecCfg->attribute[index].height;
  pstVencAttr->stVencAttr.u32Profile = pstRecCfg->attribute[index].profile;
  pstVencAttr->stVencAttr.u32StreamBufCnt = RKADK_PARAM_GetStreamBufCnt(u32CamId, false);

  return 0;
}

static bool RKADK_RECORD_IsUseVpss(int index, RKADK_PARAM_REC_CFG_S *pstRecCfg) {
  RKADK_U32 u32SrcWidth, u32SrcHeight;
  RKADK_U32 u32DstWidth, u32DstHeight;

  u32SrcWidth= pstRecCfg->vi_attr[index].stChnAttr.stSize.u32Width;
  u32SrcHeight = pstRecCfg->vi_attr[index].stChnAttr.stSize.u32Height;
  u32DstWidth = pstRecCfg->attribute[index].width;
  u32DstHeight = pstRecCfg->attribute[index].height;

  if (u32DstWidth == u32SrcWidth && u32DstHeight == u32SrcHeight) {
    return false;
  } else {
    RKADK_LOGD("In[%d, %d], Out[%d, %d]", u32SrcWidth, u32SrcHeight,
               u32DstWidth, u32DstHeight);
    return true;
  }
}

static int RKADK_RECORD_CreateVideoChn(RKADK_U32 u32CamId) {
  int ret;
  VENC_CHN_ATTR_S stVencChnAttr;
  RKADK_PARAM_REC_CFG_S *pstRecCfg = NULL;
  RKADK_PARAM_THUMB_CFG_S *ptsThumbCfg = NULL;
  RKADK_PARAM_COMM_CFG_S *pstCommCfg = NULL;
  bool bUseVpss = false;
  VPSS_GRP_ATTR_S stGrpAttr;
  VPSS_CHN_ATTR_S stChnAttr;
  RKADK_U32 u32ThumbVencChn;
  RKADK_THUMB_MODULE_E enThumbModule = RKADK_THUMB_MODULE_BUTT;

  pstRecCfg = RKADK_PARAM_GetRecCfg(u32CamId);
  if (!pstRecCfg) {
    RKADK_LOGE("RKADK_PARAM_GetRecCfg failed");
    return -1;
  }

  ptsThumbCfg = RKADK_PARAM_GetThumbCfg(u32CamId);
  if (!ptsThumbCfg) {
    RKADK_LOGE("RKADK_PARAM_GetThumbCfg failed");
    return -1;
  }

  pstCommCfg = RKADK_PARAM_GetCommCfg();
  if (!pstCommCfg) {
    RKADK_LOGE("RKADK_PARAM_GetCommCfg failed");
    return -1;
  }

  for (int i = 0; i < (int)pstRecCfg->file_num; i++) {
    ret = RKADK_RECORD_SetVideoAttr(i, u32CamId, pstRecCfg, &stVencChnAttr);
    if (ret) {
      RKADK_LOGE("RKADK_RECORD_SetVideoAttr(%d) failed", i);
      return ret;
    }

    // Create VI
    ret = RKADK_MPI_VI_Init(u32CamId, pstRecCfg->vi_attr[i].u32ViChn,
                            &(pstRecCfg->vi_attr[i].stChnAttr));
    if (ret) {
      RKADK_LOGE("RKADK_MPI_VI_Init faile, ret = %d", ret);
      return ret;
    }

    // Create VPSS
    bUseVpss = RKADK_RECORD_IsUseVpss(i, pstRecCfg);
    if (bUseVpss) {
      memset(&stGrpAttr, 0, sizeof(VPSS_GRP_ATTR_S));
      memset(&stChnAttr, 0, sizeof(VPSS_CHN_ATTR_S));

      stGrpAttr.u32MaxW = 4096;
      stGrpAttr.u32MaxH = 4096;
      stGrpAttr.enPixelFormat = pstRecCfg->vi_attr[i].stChnAttr.enPixelFormat;
      stGrpAttr.enCompressMode = COMPRESS_MODE_NONE;
      stGrpAttr.stFrameRate.s32SrcFrameRate = -1;
      stGrpAttr.stFrameRate.s32DstFrameRate = -1;
      stChnAttr.enChnMode = VPSS_CHN_MODE_USER;
      stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
      stChnAttr.enDynamicRange = DYNAMIC_RANGE_SDR8;
      stChnAttr.enPixelFormat = pstRecCfg->vi_attr[i].stChnAttr.enPixelFormat;
      stChnAttr.stFrameRate.s32SrcFrameRate = -1;
      stChnAttr.stFrameRate.s32DstFrameRate = -1;
      stChnAttr.u32Width = pstRecCfg->attribute[i].width;
      stChnAttr.u32Height = pstRecCfg->attribute[i].height;
      stChnAttr.u32Depth = 0;

      ret = RKADK_MPI_VPSS_Init(pstRecCfg->attribute[i].vpss_grp, pstRecCfg->attribute[i].vpss_chn,
                                &stGrpAttr, &stChnAttr);
      if (ret) {
        RKADK_LOGE("RKADK_MPI_VPSS_Init vpssfalied[%d]",ret);
        RKADK_MPI_VI_DeInit(u32CamId, pstRecCfg->vi_attr[i].u32ViChn);
        RKADK_MPI_VPSS_DeInit(pstRecCfg->attribute[i].vpss_grp, pstRecCfg->attribute[i].vpss_chn);
        return ret;
      }
    }

    // Create VENC
    ret = RKADK_MPI_VENC_Init(u32CamId, pstRecCfg->attribute[i].venc_chn, &stVencChnAttr);
    if (ret) {
      RKADK_LOGE("RKADK_MPI_VENC_Init failed(%d)", ret);

      if (bUseVpss)
        RKADK_MPI_VPSS_DeInit(pstRecCfg->attribute[i].vpss_grp, pstRecCfg->attribute[i].vpss_chn);

      RKADK_MPI_VI_DeInit(u32CamId, pstRecCfg->vi_attr[i].u32ViChn);
      return ret;
    }

    RK_MPI_VENC_SetSceneMode(pstRecCfg->attribute[i].venc_chn, RKADK_ENCODE_SENSE_CVR);
    RKADK_PARAM_SetVAdvancedParam(pstRecCfg->attribute[i]);

    if (i == 0) {
      enThumbModule = RKADK_THUMB_MODULE_MAIN_RECORD;
      u32ThumbVencChn = ptsThumbCfg->record_main_venc_chn;
    }
    else {
      enThumbModule = RKADK_THUMB_MODULE_SUB_RECORD;
      u32ThumbVencChn = ptsThumbCfg->record_sub_venc_chn;
    }

    ThumbnailInit(u32CamId, enThumbModule, ptsThumbCfg);
#ifndef THUMB_NORMAL
    ThumbnailChnBind(pstRecCfg->attribute[i].venc_chn, u32ThumbVencChn);
#endif
  }

  return 0;
}

static int RKADK_RECORD_DestoryVideoChn(RKADK_U32 u32CamId) {
  int ret;
  bool bUseVpss = false;
  RKADK_PARAM_REC_CFG_S *pstRecCfg = NULL;
  RKADK_PARAM_THUMB_CFG_S *ptsThumbCfg = NULL;
  RKADK_PARAM_COMM_CFG_S *pstCommCfg = NULL;
  RKADK_THUMB_MODULE_E enThumbModule = RKADK_THUMB_MODULE_BUTT;

  pstRecCfg = RKADK_PARAM_GetRecCfg(u32CamId);
  if (!pstRecCfg) {
    RKADK_LOGE("RKADK_PARAM_GetRecCfg failed");
    return -1;
  }

  ptsThumbCfg = RKADK_PARAM_GetThumbCfg(u32CamId);
  if (!ptsThumbCfg) {
    RKADK_LOGE("RKADK_PARAM_GetThumbCfg failed");
    return -1;
  }

  pstCommCfg = RKADK_PARAM_GetCommCfg();
  if (!pstCommCfg) {
    RKADK_LOGE("RKADK_PARAM_GetCommCfg failed");
    return -1;
  }

  for (int i = 0; i < (int)pstRecCfg->file_num; i++) {
    // Destroy VENC
    ret = RKADK_MPI_VENC_DeInit(pstRecCfg->attribute[i].venc_chn);
    if (ret) {
      RKADK_LOGE("RKADK_MPI_VENC_DeInit failed[%x]", ret);
      return ret;
    }

    bUseVpss = RKADK_RECORD_IsUseVpss(i, pstRecCfg);
    if (bUseVpss) {
      ret = RKADK_MPI_VPSS_DeInit(pstRecCfg->attribute[i].vpss_grp, pstRecCfg->attribute[i].vpss_chn);
      if (ret) {
        RKADK_LOGE("RKADK_MPI_VPSS_DeInit failed[%x]", ret);
        return ret;
      }
    }

    //Destroy thumb vi venc
    if (i == 0)
      enThumbModule = RKADK_THUMB_MODULE_MAIN_RECORD;
    else
      enThumbModule = RKADK_THUMB_MODULE_SUB_RECORD;

    ThumbnailDeInit(u32CamId, enThumbModule, ptsThumbCfg);

    // Destroy VI
    ret = RKADK_MPI_VI_DeInit(u32CamId, pstRecCfg->vi_attr[i].u32ViChn);
    if (ret) {
      RKADK_LOGE("RKADK_MPI_VI_DeInit failed[%x]", ret);
      return ret;
    }
  }

  return 0;
}

static int RKADK_RECORD_CreateAudioChn(RKADK_U32 u32CamId) {
  int ret;
  AUDIO_SOUND_MODE_E soundMode;
  AIO_ATTR_S stAiAttr;
  AENC_CHN_ATTR_S stAencAttr;
  RKADK_PARAM_COMM_CFG_S *pstCommCfg = NULL;
  RKADK_PARAM_AUDIO_CFG_S *pstAudioParam = NULL;
  int bytes = 2; // if the requirement is 16bit

  pstCommCfg = RKADK_PARAM_GetCommCfg();
  if (!pstCommCfg) {
    RKADK_LOGE("RKADK_PARAM_GetCommCfg failed");
    return -1;
  }

  pstAudioParam = RKADK_PARAM_GetAudioCfg();
  if (!pstAudioParam) {
    RKADK_LOGE("RKADK_PARAM_GetAudioCfg failed");
    return -1;
  }

  if (RKADK_MEDIA_EnableAencRegister(pstAudioParam->codec_type)) {
    ret = RKADK_AUDIO_ENCODER_Register(pstAudioParam->codec_type);
    if (ret) {
      RKADK_LOGE("RKADK_AUDIO_ENCODER_Register failed(%d)", ret);
      return ret;
    }
  }

  // Create AI
  memset(&stAiAttr, 0, sizeof(AIO_ATTR_S));
  memcpy(stAiAttr.u8CardName, pstAudioParam->audio_node,
         strlen(pstAudioParam->audio_node));
  stAiAttr.soundCard.channels = AUDIO_DEVICE_CHANNEL;
  stAiAttr.soundCard.sampleRate = pstAudioParam->samplerate;
  bytes = RKADK_MEDIA_GetAudioBitWidth(pstAudioParam->bit_width) / 8;
  stAiAttr.soundCard.bitWidth = pstAudioParam->bit_width;

  stAiAttr.enBitwidth = pstAudioParam->bit_width;
  stAiAttr.enSamplerate = (AUDIO_SAMPLE_RATE_E)pstAudioParam->samplerate;
  soundMode = RKADK_AI_GetSoundMode(pstAudioParam->channels);
  if (soundMode == AUDIO_SOUND_MODE_BUTT)
    return -1;

  stAiAttr.enSoundmode = soundMode;
  stAiAttr.u32FrmNum = 2;
  stAiAttr.u32PtNumPerFrm = bytes * pstAudioParam->samples_per_frame;
  stAiAttr.u32EXFlag = 0;
  stAiAttr.u32ChnCnt = pstAudioParam->channels;
  ret = RKADK_MPI_AI_Init(0, RECORD_AI_CHN, &stAiAttr, pstAudioParam->vqe_mode,
                          pstAudioParam->mic_type);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_AI_Init failed(%d)", ret);
    return ret;
  }

  // Create AENC
  memset(&stAencAttr, 0, sizeof(AENC_CHN_ATTR_S));
  stAencAttr.enType = RKADK_MEDIA_GetRkCodecType(pstAudioParam->codec_type);
  stAencAttr.u32BufCount = RKADK_PARAM_GetStreamBufCnt(u32CamId, true);
  stAencAttr.stCodecAttr.enType = stAencAttr.enType;
  stAencAttr.stCodecAttr.u32Channels = pstAudioParam->channels;
  stAencAttr.stCodecAttr.u32SampleRate = pstAudioParam->samplerate;
  stAencAttr.stCodecAttr.enBitwidth = pstAudioParam->bit_width;
  stAencAttr.stCodecAttr.pstResv = RK_NULL;

  if (pstAudioParam->codec_type == RKADK_CODEC_TYPE_MP3){
    stAencAttr.stCodecAttr.u32Resv[0] = pstAudioParam->samples_per_frame;
    stAencAttr.stCodecAttr.u32Resv[1] = pstAudioParam->bitrate;
  } else if (pstAudioParam->codec_type == RKADK_CODEC_TYPE_ACC) {
    stAencAttr.stCodecAttr.u32Resv[0] = 2;
    stAencAttr.stCodecAttr.u32Resv[1] = pstAudioParam->bitrate;
  }

  stAencAttr.stCodecAttr.u32Resv[3] = pstCommCfg->rec_mute;

  ret = RKADK_MPI_AENC_Init(RECORD_AENC_CHN, &stAencAttr);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_AENC_Init failed(%d)", ret);
    RKADK_MPI_AI_DeInit(0, RECORD_AI_CHN, pstAudioParam->vqe_mode);
    return -1;
  }

  return 0;
}

static int RKADK_RECORD_DestoryAudioChn() {
  int ret;
  RKADK_PARAM_AUDIO_CFG_S *pstAudioParam = NULL;

  pstAudioParam = RKADK_PARAM_GetAudioCfg();
  if (!pstAudioParam) {
    RKADK_LOGE("RKADK_PARAM_GetAudioCfg failed");
    return -1;
  }

  ret = RKADK_MPI_AENC_DeInit(RECORD_AENC_CHN);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_AENC_DeInit failed(%d)", ret);
    return ret;
  }

  ret = RKADK_MPI_AI_DeInit(0, RECORD_AI_CHN, pstAudioParam->vqe_mode);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_AI_DeInit failed(%d)", ret);
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

static void RKADK_RECORD_AencOutCb(AUDIO_STREAM_S stFrame,
                                   RKADK_VOID *pHandle) {
  // current rockit audio timestamp inaccurate, use fake time
  int duration = 0;
  RKADK_PARAM_AUDIO_CFG_S *pstAudioCfg = NULL;
  RKADK_MUXER_HANDLE_S *pstMuxer = NULL;

  RKADK_CHECK_POINTER_N(pHandle);

  pstMuxer = (RKADK_MUXER_HANDLE_S *)pHandle;
  pstAudioCfg = RKADK_PARAM_GetAudioCfg();
  if (!pstAudioCfg) {
    RKADK_LOGE("RKADK_PARAM_GetAudioCfg failed");
    return;
  }

  duration = 1000000 / (double)pstAudioCfg->samplerate * pstAudioCfg->samples_per_frame / pstAudioCfg->channels;

  if (!pstMuxer->u64AudioPts)
    pstMuxer->u64AudioPts = stFrame.u64TimeStamp;

  RKADK_MUXER_WriteAudioFrame(stFrame.pMbBlk, stFrame.u32Len, pstMuxer->u64AudioPts, pHandle);
  pstMuxer->u64AudioPts += duration;
}

static void RKADK_RECORD_VencOutCb(RKADK_MEDIA_VENC_DATA_S stData,
                                   RKADK_VOID *pHandle) {
  RKADK_CHECK_POINTER_N(pHandle);
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg = NULL;
  RKADK_U64 u64Pts = 0;

  RKADK_MUXER_HANDLE_S *pstMuxer = (RKADK_MUXER_HANDLE_S *)pHandle;

  pstSensorCfg = RKADK_PARAM_GetSensorCfg(pstMuxer->u32CamId);
  if (!pstSensorCfg) {
    RKADK_LOGE("RKADK_PARAM_GetSensorCfg failed");
    return;
  }

  if(pstMuxer->bLapseRecord)
    u64Pts = stData.stFrame.pstPack->u64PTS / pstSensorCfg->framerate;
  else
    u64Pts = stData.stFrame.pstPack->u64PTS;

  RKADK_MUXER_WriteVideoFrame(stData, u64Pts, pHandle);
}

static RKADK_S32 RKADK_RECORD_VencGetData(RKADK_U32 u32CamId,
                                          MPP_CHN_S *pstVencChn,
                                          RKADK_MW_PTR pRecorder) {
  int ret = 0;

  VENC_RECV_PIC_PARAM_S stRecvParam;
  stRecvParam.s32RecvPicNum = -1;
  ret = RK_MPI_VENC_StartRecvFrame(pstVencChn->s32ChnId, &stRecvParam);
  if (ret) {
    RKADK_LOGE("RK_MPI_VENC_StartRecvFrame failed[%x]", ret);
    return ret;
  }

  ret =
      RKADK_MEDIA_GetVencBuffer(pstVencChn, RKADK_RECORD_VencOutCb, pRecorder);
  if (ret) {
    RKADK_LOGE("RKADK_MEDIA_GetVencBuffer failed[%x]", ret);
    return ret;
  }

  return ret;
}

static int RKADK_RECORD_BindChn(RKADK_U32 u32CamId, RKADK_MW_PTR pRecorder) {
  int ret;
  bool bUseVpss;
  MPP_CHN_S stSrcChn, stDestChn, stSrcVpssChn, stDstVpssChn;
  RKADK_PARAM_REC_CFG_S *pstRecCfg = NULL;
  RKADK_PARAM_THUMB_CFG_S *ptsThumbCfg = NULL;
  RKADK_PARAM_COMM_CFG_S *pstCommCfg = NULL;

  pstRecCfg = RKADK_PARAM_GetRecCfg(u32CamId);
  if (!pstRecCfg) {
    RKADK_LOGE("RKADK_PARAM_GetRecCfg failed");
    return -1;
  }

  ptsThumbCfg = RKADK_PARAM_GetThumbCfg(u32CamId);
  if (!ptsThumbCfg) {
    RKADK_LOGE("RKADK_PARAM_GetThumbCfg failed");
    return -1;
  }

  pstCommCfg = RKADK_PARAM_GetCommCfg();
  if (!pstCommCfg) {
    RKADK_LOGE("RKADK_PARAM_GetCommCfg failed");
    return -1;
  }

  if (pstRecCfg->record_type != RKADK_REC_TYPE_LAPSE &&
      RKADK_MUXER_EnableAudio(u32CamId)) {
    RKADK_RECORD_SetAudioChn(&stSrcChn, &stDestChn);

    // Get aenc data
    ret = RKADK_MEDIA_GetAencBuffer(&stDestChn, RKADK_RECORD_AencOutCb,
                                    pRecorder);
    if (ret) {
      RKADK_LOGE("RKADK_MEDIA_GetAencBuffer failed[%x]", ret);
      return ret;
    }

    // Bind AI to AENC
    ret = RKADK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (ret) {
      RKADK_LOGE("RKADK_MPI_SYS_Bind failed(%d)", ret);
      return ret;
    }
  }

  for (int i = 0; i < (int)pstRecCfg->file_num; i++) {
    RKADK_RECORD_SetVideoChn(i, u32CamId, pstRecCfg, &stSrcChn, &stDestChn,
                             &stSrcVpssChn, &stDstVpssChn);

    // Get venc data
    if (RKADK_RECORD_VencGetData(u32CamId, &stDestChn, pRecorder))
      return -1;

    bUseVpss = RKADK_RECORD_IsUseVpss(i, pstRecCfg);
    if (bUseVpss) {
      // VPSS Bind VENC
      ret = RKADK_MPI_SYS_Bind(&stSrcVpssChn, &stDestChn);
      if (ret) {
        RKADK_LOGE("Bind VPSS[%d] to VENC[%d] failed[%d]", stSrcVpssChn.s32ChnId,
                   stDestChn.s32ChnId, ret);
        return ret;
      }

      // VI Bind VPSS
      ret = RKADK_MPI_SYS_Bind(&stSrcChn, &stDstVpssChn);
      if (ret) {
        RKADK_LOGE("Bind VI[%d] to VPSS[%d] failed[%d]", stSrcChn.s32ChnId,
                   stDstVpssChn.s32DevId, ret);
        return ret;
      }
    } else {
      // Bind VI to VENC
      ret = RKADK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
      if (ret) {
        RKADK_LOGE("RKADK_MPI_SYS_Bind failed(%d)", ret);
        return ret;
      }
    }
  }

  return 0;
}

static int RKADK_RECORD_UnBindChn(RKADK_U32 u32CamId) {
  int ret;
  MPP_CHN_S stSrcChn, stDestChn, stSrcVpssChn, pstDstVpssChn;
  RKADK_PARAM_REC_CFG_S *pstRecCfg = NULL;
  bool bUseVpss = false;

  pstRecCfg = RKADK_PARAM_GetRecCfg(u32CamId);
  if (!pstRecCfg) {
    RKADK_LOGE("RKADK_PARAM_GetRecCfg failed");
    return -1;
  }

  if (pstRecCfg->record_type != RKADK_REC_TYPE_LAPSE &&
      RKADK_MUXER_EnableAudio(u32CamId)) {
    RKADK_RECORD_SetAudioChn(&stSrcChn, &stDestChn);

    // Stop get aenc data
    RKADK_MEDIA_StopGetAencBuffer(&stDestChn, RKADK_RECORD_AencOutCb);

    // UnBind AI to AENC
    ret = RKADK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (ret) {
      RKADK_LOGE("RKADK_MPI_SYS_UnBind failed(%d)", ret);
      return ret;
    }
  }

  for (int i = 0; i < (int)pstRecCfg->file_num; i++) {
    RKADK_RECORD_SetVideoChn(i, u32CamId, pstRecCfg, &stSrcChn, &stDestChn,
                             &stSrcVpssChn, &pstDstVpssChn);

    // Stop get venc data
    RKADK_MEDIA_StopGetVencBuffer(&stDestChn, RKADK_RECORD_VencOutCb);

    bUseVpss = RKADK_RECORD_IsUseVpss(i, pstRecCfg);
    if (bUseVpss) {
      // VPSS UnBind VENC
      ret = RKADK_MPI_SYS_UnBind(&stSrcVpssChn, &stDestChn);
      if (ret) {
        RKADK_LOGE("UnBind VPSS[%d] to VENC[%d] failed[%d]", stSrcVpssChn.s32ChnId,
                   stDestChn.s32ChnId, ret);
        return ret;
      }

      // VI UnBind VPSS
      ret = RKADK_MPI_SYS_UnBind(&stSrcChn, &pstDstVpssChn);
      if (ret) {
        RKADK_LOGE("UnBind VI[%d] to VPSS[%d] failed[%d]", stSrcChn.s32ChnId,
                   pstDstVpssChn.s32DevId, ret);
        return ret;
      }
    } else {
      // UnBind VI to VENC
      ret = RKADK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
      if (ret) {
        RKADK_LOGE("RKADK_MPI_SYS_UnBind failed(%d)", ret);
        return ret;
      }
    }
  }

  return 0;
}

static RKADK_S32 RKADK_RECORD_SetMuxerAttr(RKADK_U32 u32CamId,
                                           RKADK_MUXER_ATTR_S *pstMuxerAttr) {
  bool bEnableAudio = false;
  RKADK_U32 bitrate;
  RKADK_PARAM_AUDIO_CFG_S *pstAudioParam = NULL;
  RKADK_PARAM_REC_CFG_S *pstRecCfg = NULL;
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg = NULL;

  RKADK_CHECK_CAMERAID(u32CamId, RKADK_FAILURE);

  pstAudioParam = RKADK_PARAM_GetAudioCfg();
  if (!pstAudioParam) {
    RKADK_LOGE("RKADK_PARAM_GetAudioCfg failed");
    return -1;
  }

  pstRecCfg = RKADK_PARAM_GetRecCfg(u32CamId);
  if (!pstRecCfg) {
    RKADK_LOGE("RKADK_PARAM_GetRecCfg failed");
    return -1;
  }

  bEnableAudio = RKADK_MUXER_EnableAudio(u32CamId);

  pstSensorCfg = RKADK_PARAM_GetSensorCfg(u32CamId);
  if (!pstSensorCfg) {
    RKADK_LOGE("RKADK_PARAM_GetSensorCfg failed");
    return -1;
  }

  memset(pstMuxerAttr, 0, sizeof(RKADK_MUXER_ATTR_S));

  if (pstRecCfg->record_type == RKADK_REC_TYPE_LAPSE)
    pstMuxerAttr->bLapseRecord = true;
  else
    pstMuxerAttr->bLapseRecord = false;

  pstMuxerAttr->u32CamId = u32CamId;
  pstMuxerAttr->u32StreamCnt = pstRecCfg->file_num;
  pstMuxerAttr->stPreRecordAttr.u32PreRecTimeSec = pstRecCfg->pre_record_time;
  pstMuxerAttr->stPreRecordAttr.enPreRecordMode = pstRecCfg->pre_record_mode;
  pstMuxerAttr->stPreRecordAttr.u32PreRecCacheTime = GetPreRecordCacheTime(pstRecCfg, pstSensorCfg);
  pstMuxerAttr->pcbRequestFileNames = GetRecordFileName;

  for (int i = 0; i < (int)pstMuxerAttr->u32StreamCnt; i++) {
    pstMuxerAttr->astStreamAttr[i].enType = pstRecCfg->file_type;
    if (pstRecCfg->record_type == RKADK_REC_TYPE_LAPSE) {
      bitrate = pstRecCfg->attribute[i].bitrate / pstRecCfg->lapse_multiple;
      pstMuxerAttr->astStreamAttr[i].u32TimeLenSec =
          pstRecCfg->record_time_cfg[i].lapse_interval;
      pstMuxerAttr->astStreamAttr[i].u32TrackCnt = 1; // only video track
    } else {
      bitrate = pstRecCfg->attribute[i].bitrate;
      pstMuxerAttr->astStreamAttr[i].u32TimeLenSec =
          pstRecCfg->record_time_cfg[i].record_time;
      pstMuxerAttr->astStreamAttr[i].u32TrackCnt = RKADK_MUXER_TRACK_MAX_CNT;
    }

    if (pstMuxerAttr->astStreamAttr[i].u32TrackCnt == RKADK_MUXER_TRACK_MAX_CNT)
      if (!bEnableAudio)
        pstMuxerAttr->astStreamAttr[i].u32TrackCnt = 1;

    // video track
    RKADK_MUXER_TRACK_SOURCE_S *aHTrackSrcHandle =
        &(pstMuxerAttr->astStreamAttr[i].aHTrackSrcHandle[0]);
    aHTrackSrcHandle->enTrackType = RKADK_TRACK_SOURCE_TYPE_VIDEO;
    aHTrackSrcHandle->u32ChnId = pstRecCfg->attribute[i].venc_chn;
    aHTrackSrcHandle->unTrackSourceAttr.stVideoInfo.enCodecType =
        pstRecCfg->attribute[i].codec_type;
    if (RKADK_MEDIA_GetPixelFormat(
            pstRecCfg->vi_attr[i].stChnAttr.enPixelFormat,
            aHTrackSrcHandle->unTrackSourceAttr.stVideoInfo.cPixFmt))
      return -1;
    aHTrackSrcHandle->unTrackSourceAttr.stVideoInfo.u32FrameRate =
        pstSensorCfg->framerate;
    aHTrackSrcHandle->unTrackSourceAttr.stVideoInfo.u16Level = 41;
    aHTrackSrcHandle->unTrackSourceAttr.stVideoInfo.u16Profile =
        pstRecCfg->attribute[i].profile;
    aHTrackSrcHandle->unTrackSourceAttr.stVideoInfo.u32BitRate = bitrate;
    aHTrackSrcHandle->unTrackSourceAttr.stVideoInfo.u32Width =
        pstRecCfg->attribute[i].width;
    aHTrackSrcHandle->unTrackSourceAttr.stVideoInfo.u32Height =
        pstRecCfg->attribute[i].height;
    aHTrackSrcHandle->unTrackSourceAttr.stVideoInfo.u32Gop =
        pstRecCfg->attribute[i].gop;

    if (pstRecCfg->record_type == RKADK_REC_TYPE_LAPSE || !bEnableAudio)
      continue;

    // audio track
    aHTrackSrcHandle = &(pstMuxerAttr->astStreamAttr[i].aHTrackSrcHandle[1]);
    aHTrackSrcHandle->enTrackType = RKADK_TRACK_SOURCE_TYPE_AUDIO;
    aHTrackSrcHandle->u32ChnId = RECORD_AENC_CHN;
    aHTrackSrcHandle->unTrackSourceAttr.stAudioInfo.enCodecType =
        pstAudioParam->codec_type;
    aHTrackSrcHandle->unTrackSourceAttr.stAudioInfo.u32BitWidth =
        RKADK_MEDIA_GetAudioBitWidth(pstAudioParam->bit_width);
    aHTrackSrcHandle->unTrackSourceAttr.stAudioInfo.u32ChnCnt =
        pstAudioParam->channels;
    aHTrackSrcHandle->unTrackSourceAttr.stAudioInfo.u32SamplesPerFrame =
        pstAudioParam->samples_per_frame;
    aHTrackSrcHandle->unTrackSourceAttr.stAudioInfo.u32SampleRate =
        pstAudioParam->samplerate;
    aHTrackSrcHandle->unTrackSourceAttr.stAudioInfo.u32Bitrate =
        pstAudioParam->bitrate;
  }

  return 0;
}

static void RKADK_RECORD_ResetVideoChn(RKADK_U32 index, RKADK_U32 u32CamId,
                                       RKADK_PARAM_REC_CFG_S *pstRecCfg,
                                       MPP_CHN_S *pstSrcChn, MPP_CHN_S *pstRecVenChn,
                                       bool bUseVpss) {
  if (!bUseVpss) {
    pstSrcChn->enModId = RK_ID_VI;
    pstSrcChn->s32DevId = u32CamId;
    pstSrcChn->s32ChnId = pstRecCfg->vi_attr[index].u32ViChn;
  } else {
    pstSrcChn->enModId = RK_ID_VPSS;
    pstSrcChn->s32DevId = u32CamId;
    pstSrcChn->s32ChnId = pstRecCfg->attribute[index].vpss_chn;
  }

  pstRecVenChn->enModId = RK_ID_VENC;
  pstRecVenChn->s32DevId = 0;
  pstRecVenChn->s32ChnId = pstRecCfg->attribute[index].venc_chn;
}

static RKADK_S32 RKADK_RECORD_ResetVideoAttr(RKADK_U32 u32CamId, RKADK_U32 index,
                                        RKADK_PARAM_REC_CFG_S *pstRecCfg,
                                        RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg,
                                        VENC_CHN_ATTR_S *pstVencAttr,
                                        VI_CHN_ATTR_S *pstViAttr) {
  bool bReset = false;
  RKADK_U32 u32DstFrameRateNum;
  RKADK_U32 bitrate;
  RKADK_U32 u32Gop;
  RKADK_U32 u32Width;
  RKADK_U32 u32Height;
  RK_CODEC_ID_E enType;

  bitrate = pstRecCfg->attribute[index].bitrate;
  u32Gop = pstRecCfg->attribute[index].gop;
  u32DstFrameRateNum = pstSensorCfg->framerate;
  u32Width = pstRecCfg->attribute[index].width;
  u32Height = pstRecCfg->attribute[index].height;

  if (pstRecCfg->record_type == RKADK_REC_TYPE_LAPSE) {
    bitrate = pstRecCfg->attribute[index].bitrate / pstRecCfg->lapse_multiple;
    u32DstFrameRateNum = pstSensorCfg->framerate / pstRecCfg->lapse_multiple;
    if (u32DstFrameRateNum < 1)
      u32DstFrameRateNum = 1;
    else if (u32DstFrameRateNum > pstSensorCfg->framerate)
      u32DstFrameRateNum = pstSensorCfg->framerate;
  }

  enType = RKADK_MEDIA_GetRkCodecType(pstRecCfg->attribute[index].codec_type);

  bReset = RKADK_MEDIA_CompareResolution(pstVencAttr, u32Width, u32Height);

  bReset |= RKADK_MEDIA_VencAttrCmp(pstVencAttr, enType, u32DstFrameRateNum, bitrate);

  if (!bReset)
    return -1;

  if (index == 0) {
    pstVencAttr->stVencAttr.u32MaxPicWidth = pstSensorCfg->max_width;
    pstVencAttr->stVencAttr.u32MaxPicHeight = pstSensorCfg->max_height;
    pstViAttr->stIspOpt.stMaxSize.u32Width = pstSensorCfg->max_width;
    pstViAttr->stIspOpt.stMaxSize.u32Height = pstSensorCfg->max_height;
  } else {
    pstVencAttr->stVencAttr.u32MaxPicWidth = u32Width;
    pstVencAttr->stVencAttr.u32MaxPicHeight = u32Height;
    pstViAttr->stIspOpt.stMaxSize.u32Width = u32Width;
    pstViAttr->stIspOpt.stMaxSize.u32Height = u32Height;
  }
  pstVencAttr->stVencAttr.u32PicWidth = u32Width;
  pstVencAttr->stVencAttr.u32PicHeight = u32Height;
  pstVencAttr->stVencAttr.u32VirWidth = u32Width;
  pstVencAttr->stVencAttr.u32VirHeight = u32Height;
  pstVencAttr->stVencAttr.enType = enType;
  pstVencAttr->stVencAttr.u32StreamBufCnt = RKADK_PARAM_GetStreamBufCnt(u32CamId, false);
  pstVencAttr->stRcAttr.enRcMode =
        RKADK_PARAM_GetRcMode(pstRecCfg->attribute[index].rc_mode,
                              pstRecCfg->attribute[index].codec_type);
  RKADK_MEDIA_SetRcAttr(&pstVencAttr->stRcAttr, u32Gop, bitrate,
                          pstSensorCfg->framerate, u32DstFrameRateNum);

  pstViAttr->stSize.u32Width = pstRecCfg->attribute[index].width;
  pstViAttr->stSize.u32Height = pstRecCfg->attribute[index].height;

  return 0;
}

static RKADK_U32 RKADK_RECORD_ResetThumb(RKADK_U32 u32CamId, RKADK_U32 index,
                                        RKADK_PARAM_REC_CFG_S *pstRecCfg) {
  int ret;
  RKADK_U32 u32ThumbChn;
  VENC_PACK_S stPack;
  VENC_STREAM_S stFrame;

  RKADK_PARAM_THUMB_CFG_S *ptsThumbCfg = RKADK_PARAM_GetThumbCfg(u32CamId);
  if (!ptsThumbCfg) {
    RKADK_LOGE("RKADK_PARAM_GetThumbCfg failed");
    return -1;
  }

  if (index == 0)
    u32ThumbChn = ptsThumbCfg->record_main_venc_chn;
  else
    u32ThumbChn = ptsThumbCfg->record_sub_venc_chn;

  //clean thumbnail
  stFrame.pstPack = &stPack;
  do {
    ret = RK_MPI_VENC_GetStream(u32ThumbChn, &stFrame, 1);
    if (ret == RK_SUCCESS) {
      ret = RK_MPI_VENC_ReleaseStream(u32ThumbChn, &stFrame);
      if (ret != RK_SUCCESS)
        RKADK_LOGE("RK_MPI_VENC_ReleaseStream fail %x", ret);
    } else {
      break;
    }
  } while(1);

  //make sure thumbnail
  RKADK_LOGI("Record stream [%d] request thumbnail", pstRecCfg->attribute[index].venc_chn);
#ifndef THUMB_NORMAL
  RK_MPI_VENC_ThumbnailRequest(pstRecCfg->attribute[index].venc_chn);
#else
  ThumbnailRequest(u32ThumbChn);
#endif

  return 0;
}

static RKADK_S32 RKADK_RECORD_ResetVideo(RKADK_U32 u32CamId,
                                         RKADK_PARAM_REC_CFG_S *pstRecCfg,
                                         RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg,
                                         RKADK_MW_PTR pRecorder) {
  int ret;
  bool bChangeResolution;
  bool bUseVpss = false;
  MPP_CHN_S stSrcChn, stRecVenChn;
  VENC_CHN_ATTR_S stVencAttr;
  VI_CHN_ATTR_S stViAttr;
  RKADK_MUXER_ATTR_S stMuxerAttr;

  ret = RKADK_RECORD_SetMuxerAttr(u32CamId, &stMuxerAttr);
  if (ret) {
    RKADK_LOGE("RKADK_RECORD_SetMuxerAttr failed");
    return -1;
  }

  for (RKADK_U32 index = 0; index < pstRecCfg->file_num; index++) {
    ret = RKADK_MUXER_Reset(pRecorder,
                            pstRecCfg->attribute[index].venc_chn);
    if (ret) {
      RKADK_LOGE("RKADK_MUXER_Reset [%d] failed[%d]",
                  pstRecCfg->attribute[index].venc_chn, ret);
      return -1;
    }

    ret = RKADK_RECORD_ResetThumb(u32CamId, index, pstRecCfg);
    if (ret)
      RKADK_LOGE("RKADK_RECORD_ResetThumb failed");

    memset(&stSrcChn, 0, sizeof(MPP_CHN_S));
    memset(&stRecVenChn, 0, sizeof(MPP_CHN_S));

    bUseVpss = RKADK_RECORD_IsUseVpss(index, pstRecCfg);
    RKADK_RECORD_ResetVideoChn(index, u32CamId, pstRecCfg,
                               &stSrcChn, &stRecVenChn, bUseVpss);

    memset(&stVencAttr, 0, sizeof(VENC_CHN_ATTR_S));
    memset(&stViAttr, 0, sizeof(VI_CHN_ATTR_S));

    ret = RK_MPI_VI_GetChnAttr(u32CamId, stSrcChn.s32ChnId, &stViAttr);
    if (ret != RK_SUCCESS) {
      RKADK_LOGE("RK_MPI_VI_GetChnAttr(%d) failed %x", stSrcChn.s32ChnId, ret);
      return -1;
    }

    ret = RK_MPI_VENC_GetChnAttr(stRecVenChn.s32ChnId, &stVencAttr);
    if (ret != RK_SUCCESS) {
      RKADK_LOGE("RK_MPI_VENC_GetChnAttr(%d) failed %x",
                  stRecVenChn.s32ChnId, ret);
      return -1;
    }

    bChangeResolution = RKADK_MEDIA_CompareResolution(&stVencAttr,
                                  pstRecCfg->attribute[index].width,
                                  pstRecCfg->attribute[index].height);

    ret = RKADK_RECORD_ResetVideoAttr(u32CamId, index, pstRecCfg, pstSensorCfg,
                                 &stVencAttr, &stViAttr);
    if (ret) {
      RKADK_LOGI("Record [%d] stream venc attr is not changed",
                  index);
      continue;
    }

    ret = RK_MPI_SYS_UnBind(&stSrcChn, &stRecVenChn);
    if (ret != RK_SUCCESS) {
      RKADK_LOGE("Record index[%d] VI UnBind VENC [%d %d]fail %x",
                  index, stSrcChn.s32ChnId,stRecVenChn.s32ChnId, ret);
      return -1;
    }

    ret = RK_MPI_VENC_SetChnAttr(stRecVenChn.s32ChnId, &stVencAttr);
    if (ret != RK_SUCCESS) {
      RKADK_LOGE("Record index[%d] set venc[%d] attr failed %x",
                  index, stRecVenChn.s32ChnId, ret);
      return -1;
    }

    if (bChangeResolution) {
      ret = RK_MPI_VI_SetChnAttr(u32CamId, stSrcChn.s32ChnId,
                            &stViAttr);
      if (ret != RK_SUCCESS) {
        RKADK_LOGE("RK_MPI_VI_SetChnAttr(%d) failed %x",
                    stSrcChn.s32ChnId, ret);
        return -1;
      }
    }

    ret = RKADK_MUXER_ResetParam(stRecVenChn.s32ChnId, pRecorder, &stMuxerAttr, index);
    if (ret) {
      RKADK_LOGE("RKADK_MUXER_Change(%d) failed", stRecVenChn.s32ChnId);
      return -1;
    }

    ret = RK_MPI_SYS_Bind(&stSrcChn, &stRecVenChn);
    if(ret != RK_SUCCESS) {
      RKADK_LOGE("Record index[%d] VI Bind VENC [%d %d] fail %x",
                  index, stSrcChn.s32ChnId,stRecVenChn.s32ChnId, ret);
      return -1;
    }
  }

  return 0;
}

static RKADK_S32 RKADK_RECORD_ResetAudio(RKADK_PARAM_REC_CFG_S *pstRecCfg,
                                         RKADK_MUXER_HANDLE_S *pstRecorder) {
  int ret;
  MPP_CHN_S stSrcChn, stDestChn;
  RKADK_REC_TYPE_E enRecType = RKADK_REC_TYPE_NORMAL;

  pstRecorder->u64AudioPts = 0;
  if (pstRecorder->bLapseRecord)
    enRecType = RKADK_REC_TYPE_LAPSE;

  if (pstRecCfg->record_type == enRecType) {
    RKADK_LOGI("Record type has not changed, no need to reset the audio");
    return 0;
  }

  RKADK_LOGI("Record reset audio state [%d %d] start...",
              pstRecorder->u32CamId, pstRecCfg->record_type);

  if (pstRecCfg->record_type == RKADK_REC_TYPE_LAPSE &&
      RKADK_MUXER_EnableAudio(pstRecorder->u32CamId)) {
    RKADK_RECORD_SetAudioChn(&stSrcChn, &stDestChn);

    // Stop get aenc data
    RKADK_MEDIA_StopGetAencBuffer(&stDestChn, RKADK_RECORD_AencOutCb);

    // UnBind AI to AENC
    ret = RKADK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (ret) {
      RKADK_LOGE("RKADK_MPI_SYS_UnBind failed(%d)", ret);
      RKADK_MEDIA_GetAencBuffer(&stDestChn, RKADK_RECORD_AencOutCb,
                                pstRecorder);
      return ret;
    }

    RKADK_RECORD_DestoryAudioChn();
    pstRecorder->bLapseRecord = true;
  } else if (pstRecCfg->record_type == RKADK_REC_TYPE_NORMAL &&
             RKADK_MUXER_EnableAudio(pstRecorder->u32CamId)){
    if (RKADK_RECORD_CreateAudioChn(pstRecorder->u32CamId)) {
      RKADK_LOGE("RKADK_RECORD_CreateAudioChn failed");
      return -1;
    }

    RKADK_RECORD_SetAudioChn(&stSrcChn, &stDestChn);

    // Get aenc data
    RKADK_MEDIA_GetAencBuffer(&stDestChn, RKADK_RECORD_AencOutCb,
                                    pstRecorder);

    // Bind AI to AENC
    ret = RKADK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (ret) {
      RKADK_LOGE("RKADK_MPI_SYS_Bind failed(%d)", ret);
      RKADK_MEDIA_StopGetAencBuffer(&stDestChn, RKADK_RECORD_AencOutCb);
      RKADK_RECORD_DestoryAudioChn();
      return ret;
    }

    pstRecorder->bLapseRecord = false;
  }

  RKADK_LOGI("Record reset audio state [%d %d] end...",
              pstRecorder->u32CamId, pstRecCfg->record_type);
  return 0;
}

RKADK_S32 RKADK_RECORD_Create(RKADK_RECORD_ATTR_S *pstRecAttr,
                              RKADK_MW_PTR *ppRecorder) {
  int ret;
  bool bSysInit = false;
  bool bEnableAudio = false;
  RKADK_PARAM_REC_CFG_S *pstRecCfg = NULL;

  RKADK_CHECK_POINTER(pstRecAttr, RKADK_FAILURE);
  RKADK_CHECK_CAMERAID(pstRecAttr->s32CamID, RKADK_FAILURE);

  bSysInit = RKADK_MPI_SYS_CHECK();
  if (!bSysInit) {
    RKADK_LOGE("System is not initialized");
    return -1;
  }
  RKADK_PARAM_Init(NULL, NULL);

  pstRecCfg = RKADK_PARAM_GetRecCfg(pstRecAttr->s32CamID);
  if (!pstRecCfg) {
    RKADK_LOGE("RKADK_PARAM_GetRecCfg failed");
    return -1;
  }

  RKADK_LOGI("Create Record[%d, %d] Start...", pstRecAttr->s32CamID,
             pstRecCfg->record_type);

  if (RKADK_RECORD_CreateVideoChn(pstRecAttr->s32CamID))
    return -1;

  bEnableAudio = RKADK_MUXER_EnableAudio(pstRecAttr->s32CamID);
  if (pstRecCfg->record_type != RKADK_REC_TYPE_LAPSE && bEnableAudio) {
    if (RKADK_RECORD_CreateAudioChn(pstRecAttr->s32CamID)) {
      RKADK_RECORD_DestoryVideoChn(pstRecAttr->s32CamID);
      return -1;
    }
  }

  g_pfnRequestFileNames[pstRecAttr->s32CamID] = pstRecAttr->pfnRequestFileNames;

  RKADK_MUXER_ATTR_S stMuxerAttr;
  ret = RKADK_RECORD_SetMuxerAttr(pstRecAttr->s32CamID, &stMuxerAttr);
  if (ret) {
    RKADK_LOGE("RKADK_RECORD_SetMuxerAttr failed");
    goto failed;
  }

  stMuxerAttr.pfnEventCallback = pstRecAttr->pfnEventCallback;
  if (RKADK_MUXER_Create(&stMuxerAttr, ppRecorder))
    goto failed;

  if (RKADK_RECORD_BindChn(pstRecAttr->s32CamID, *ppRecorder))
    goto failed;

  RKADK_LOGI("Create Record[%d, %d] End...", pstRecAttr->s32CamID,
             pstRecCfg->record_type);
  return 0;

failed:
  RKADK_LOGE("Create Record[%d, %d] failed", pstRecAttr->s32CamID,
             pstRecCfg->record_type);
  RKADK_RECORD_DestoryVideoChn(pstRecAttr->s32CamID);

  if (pstRecCfg->record_type != RKADK_REC_TYPE_LAPSE && bEnableAudio)
    RKADK_RECORD_DestoryAudioChn();

  return -1;
}

RKADK_S32 RKADK_RECORD_Destroy(RKADK_MW_PTR pRecorder) {
  RKADK_S32 ret, index;
  RKADK_U32 u32CamId;
  RKADK_MUXER_HANDLE_S *stRecorder = NULL;
  RKADK_PARAM_REC_CFG_S *pstRecCfg = NULL;

  RKADK_CHECK_POINTER(pRecorder, RKADK_FAILURE);
  stRecorder = (RKADK_MUXER_HANDLE_S *)pRecorder;
  if (!stRecorder) {
    RKADK_LOGE("stRecorder is null");
    return -1;
  }

  u32CamId = stRecorder->u32CamId;
  RKADK_CHECK_CAMERAID(u32CamId, RKADK_FAILURE);

  pstRecCfg = RKADK_PARAM_GetRecCfg(u32CamId);
  if (!pstRecCfg) {
    RKADK_LOGE("RKADK_PARAM_GetRecCfg failed");
    return -1;
  }

  RKADK_LOGI("Destory Record[%d, %d] Start...", stRecorder->u32CamId,
             pstRecCfg->record_type);

  for (int i = 0; i < (int)stRecorder->u32StreamCnt; i++) {
    index = i + (RKADK_MUXER_STREAM_MAX_CNT * u32CamId);
    while (!g_fileNameDeque[index].empty()) {
      RKADK_LOGI("clear file name deque[%d]", index);
      auto front = g_fileNameDeque[index].front();
      g_fileNameDeque[index].pop_front();
      free(front);
    }
    g_fileNameDeque[index].clear();
  }

  ret = RKADK_RECORD_UnBindChn(u32CamId);
  if (ret) {
    RKADK_LOGE("RKADK_RECORD_UnBindChn failed, ret = %d", ret);
    return ret;
  }

  ret = RKADK_RECORD_DestoryVideoChn(u32CamId);
  if (ret) {
    RKADK_LOGE("RKADK_RECORD_DestoryVideoChn failed[%x]", ret);
    return ret;
  }

  if (pstRecCfg->record_type != RKADK_REC_TYPE_LAPSE &&
      RKADK_MUXER_EnableAudio(u32CamId)) {
    ret = RKADK_RECORD_DestoryAudioChn();
    if (ret) {
      RKADK_LOGE("RKADK_RECORD_DestoryAudioChn failed, ret = %d", ret);
      return ret;
    }
  }

  ret = RKADK_MUXER_Destroy(pRecorder);
  if (ret) {
    RKADK_LOGE("RK_REC_Destroy failed, ret = %d", ret);
    return ret;
  }

  g_pfnRequestFileNames[u32CamId] = NULL;
  RKADK_LOGI("Destory Record[%d, %d] End...", u32CamId, pstRecCfg->record_type);
  return 0;
}

RKADK_S32 RKADK_RECORD_Start(RKADK_MW_PTR pRecorder) {
  return RKADK_MUXER_Start(pRecorder);
}

RKADK_S32 RKADK_RECORD_Stop(RKADK_MW_PTR pRecorder) {
  return RKADK_MUXER_Stop(pRecorder);
}

RKADK_S32 RKADK_RECORD_Reset(RKADK_MW_PTR *pRecorder) {
  int ret;
  RKADK_U32 u32CamId;
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg = NULL;
  RKADK_PARAM_REC_CFG_S *pstRecCfg = NULL;
  RKADK_MUXER_HANDLE_S *pstRecorder = NULL;

  RKADK_CHECK_POINTER(*pRecorder, RKADK_FAILURE);
  pstRecorder = (RKADK_MUXER_HANDLE_S *)*pRecorder;
  if (!pstRecorder) {
    RKADK_LOGE("pstRecorder is null");
    return -1;
  }

  RKADK_LOGI("Change [%d] start...", pstRecorder->u32CamId);

  u32CamId = pstRecorder->u32CamId;
  RKADK_CHECK_CAMERAID(u32CamId, RKADK_FAILURE);

  pstRecCfg = RKADK_PARAM_GetRecCfg(u32CamId);
  if (!pstRecCfg) {
    RKADK_LOGE("RKADK_PARAM_GetRecCfg failed");
    return -1;
  }

  pstSensorCfg = RKADK_PARAM_GetSensorCfg(u32CamId);
  if (!pstSensorCfg) {
    RKADK_LOGE("RKADK_PARAM_GetSensorCfg failed");
    return -1;
  }

  ret = RKADK_MUXER_Stop(*pRecorder);
  if (ret) {
    RKADK_LOGE("RKADK_MUXER_Stop failed[%d]", ret);
    return -1;
  }

  RKADK_MUXER_SetResetState(*pRecorder, true);
  ret = RKADK_RECORD_ResetVideo(u32CamId, pstRecCfg, pstSensorCfg, *pRecorder);
  if (ret) {
    RKADK_LOGE("RKADK_RECORD_ResetVideo failed");
    goto failed;
  }

  ret = RKADK_RECORD_ResetAudio(pstRecCfg, pstRecorder);
  if (ret) {
    RKADK_LOGE("RKADK_RECORD_ResetAudio failed");
    goto failed;
  }

  RKADK_MUXER_SetResetState(*pRecorder, false);
  RKADK_MUXER_Start(*pRecorder);

  RKADK_LOGI("Change [%d] end...", u32CamId);
  return 0;

failed:
  RKADK_LOGI("Change [%d] failed...", u32CamId);
  RKADK_MUXER_Start(*pRecorder);

  return -1;
}

RKADK_S32 RKADK_RECORD_SetFrameRate(RKADK_MW_PTR pRecorder,
                                    RKADK_RECORD_FPS_ATTR_S stFpsAttr) {
  RKADK_LOGE("Settings are not supported");
  return -1;
}

RKADK_S32
RKADK_RECORD_ManualSplit(RKADK_MW_PTR pRecorder,
                         RKADK_REC_MANUAL_SPLIT_ATTR_S *pstSplitAttr) {
  return RKADK_MUXER_ManualSplit(pRecorder, pstSplitAttr);
}

RKADK_S32 RKADK_RECORD_GetAencChn() { return RECORD_AENC_CHN; }

RKADK_S32 RKADK_RECORD_RegisterEventCallback(
    RKADK_MW_PTR pRecorder, RKADK_REC_EVENT_CALLBACK_FN pfnEventCallback) {
  RKADK_LOGE("unsupport toogle mirror");
  return -1;

}

RKADK_S32 RKADK_RECORD_ToogleMirror(RKADK_MW_PTR pRecorder,
                                    RKADK_STREAM_TYPE_E enStrmType,
                                    int mirror) {
  RKADK_LOGE("unsupport toogle mirror");
  return -1;
}

RKADK_S32 RKADK_RECORD_ToogleFlip(RKADK_MW_PTR pRecorder,
                                  RKADK_STREAM_TYPE_E enStrmType,
                                  int flip) {
  RKADK_LOGE("unsupport toogle flip");
  return -1;
}
