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

#include "rkadk_param.h"
#include "rkadk_param_map.h"
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define RKISPP_SCALE0_NV12_WIDTH_MAX 2080
#define RKISPP_SCALE1_WIDTH_MAX 1280

/** parameter context */
static RKADK_PARAM_CONTEXT_S g_stPARAMCtx = {
    .bInit = false, .mutexLock = PTHREAD_MUTEX_INITIALIZER, .stCfg = {0}};

static RKADK_S32 RKADK_PARAM_SaveViCfg(char *path, RKADK_U32 viIndex,
                                       RKADK_U32 u32CamId) {
  int ret = 0;
  RKADK_PARAM_MEDIA_CFG_S *pstMediaCfg = NULL;
  RKADK_MAP_TABLE_CFG_S *pstMapTableCfg = NULL;

  RKADK_CHECK_CAMERAID(u32CamId, RKADK_FAILURE);
  pstMediaCfg = &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId];

  switch (viIndex) {
  case 0:
    pstMapTableCfg = RKADK_PARAM_GetMapTable(u32CamId, RKADK_PARAM_VI0_MAP);
    break;

  case 1:
    pstMapTableCfg = RKADK_PARAM_GetMapTable(u32CamId, RKADK_PARAM_VI1_MAP);
    break;

  case 2:
    pstMapTableCfg = RKADK_PARAM_GetMapTable(u32CamId, RKADK_PARAM_VI2_MAP);
    break;

  case 3:
    pstMapTableCfg = RKADK_PARAM_GetMapTable(u32CamId, RKADK_PARAM_VI3_MAP);
    break;

  default:
    RKADK_LOGE("Invaild vi index = %d", viIndex);
    return -1;
  }
  RKADK_CHECK_POINTER(pstMapTableCfg, RKADK_FAILURE);

  ret = RKADK_Struct2Ini(path, &pstMediaCfg->stViCfg[viIndex],
                         pstMapTableCfg->pstMapTable,
                         pstMapTableCfg->u32TableLen);
  if (ret)
    RKADK_LOGE("save u32CamId[%d] VI[%d] param failed", u32CamId, viIndex);

  return ret;
}

static RKADK_S32 RKADK_PARAM_SaveVersion(char *path) {
  int ret = 0;
  RKADK_PARAM_VERSION_S *pstVersion = &g_stPARAMCtx.stCfg.stVersion;

  ret = RKADK_Struct2Ini(path, pstVersion, g_stVersionMapTable,
                         sizeof(g_stVersionMapTable) /
                             sizeof(RKADK_SI_CONFIG_MAP_S));
  if (ret)
    RKADK_LOGE("save version failed");

  return ret;
}

static RKADK_S32 RKADK_PARAM_SaveCommCfg(char *path) {
  int ret = 0;
  RKADK_PARAM_COMM_CFG_S *pstCommCfg = &g_stPARAMCtx.stCfg.stCommCfg;

  ret = RKADK_Struct2Ini(path, pstCommCfg, g_stCommCfgMapTable,
                         sizeof(g_stCommCfgMapTable) /
                             sizeof(RKADK_SI_CONFIG_MAP_S));
  if (ret)
    RKADK_LOGE("save common param failed");

  return ret;
}

static RKADK_S32 RKADK_PARAM_SaveAudioCfg(char *path) {
  int ret = 0;
  RKADK_PARAM_AUDIO_CFG_S *pstAudioCfg = &g_stPARAMCtx.stCfg.stAudioCfg;

  ret = RKADK_Struct2Ini(path, pstAudioCfg, g_stAudioCfgMapTable,
                         sizeof(g_stAudioCfgMapTable) /
                             sizeof(RKADK_SI_CONFIG_MAP_S));
  if (ret)
    RKADK_LOGE("save audio param failed");

  return ret;
}

static RKADK_S32 RKADK_PARAM_SaveThumbCfg(char *path, RKADK_U32 u32CamId) {
  int ret = 0;
  RKADK_PARAM_THUMB_CFG_S *pstThumbCfg;
  RKADK_MAP_TABLE_CFG_S *pstMapTableCfg = NULL;

  RKADK_CHECK_CAMERAID(u32CamId, RKADK_FAILURE);

  pstThumbCfg = &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stThumbCfg;

  pstMapTableCfg = RKADK_PARAM_GetMapTable(u32CamId, RKADK_PARAM_THUMB_MAP);
  RKADK_CHECK_POINTER(pstMapTableCfg, RKADK_FAILURE);

  ret = RKADK_Struct2Ini(path, pstThumbCfg, pstMapTableCfg->pstMapTable,
                         pstMapTableCfg->u32TableLen);
  if (ret)
    RKADK_LOGE("save thumb param failed");

  return ret;
}

static RKADK_S32 RKADK_PARAM_SaveSensorCfg(char *path, RKADK_U32 u32CamId) {
  int ret;
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg;
  RKADK_MAP_TABLE_CFG_S *pstMapTableCfg = NULL;

  RKADK_CHECK_CAMERAID(u32CamId, RKADK_FAILURE);

  pstSensorCfg = &g_stPARAMCtx.stCfg.stSensorCfg[u32CamId];

  pstMapTableCfg = RKADK_PARAM_GetMapTable(u32CamId, RKADK_PARAM_SENSOR_MAP);
  RKADK_CHECK_POINTER(pstMapTableCfg, RKADK_FAILURE);

  ret = RKADK_Struct2Ini(path, pstSensorCfg, pstMapTableCfg->pstMapTable,
                         pstMapTableCfg->u32TableLen);
  if (ret)
    RKADK_LOGE("save sensor[%d] param failed", u32CamId);

  return ret;
}

static RKADK_S32 RKADK_PARAM_SavePhotoCfg(char *path, RKADK_U32 u32CamId) {
  int ret;
  RKADK_PARAM_PHOTO_CFG_S *pstPhotoCfg;
  RKADK_MAP_TABLE_CFG_S *pstMapTableCfg = NULL;

  RKADK_CHECK_CAMERAID(u32CamId, RKADK_FAILURE);

  pstPhotoCfg = &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stPhotoCfg;

  pstMapTableCfg = RKADK_PARAM_GetMapTable(u32CamId, RKADK_PARAM_PHOTO_MAP);
  RKADK_CHECK_POINTER(pstMapTableCfg, RKADK_FAILURE);

  ret = RKADK_Struct2Ini(path, pstPhotoCfg, pstMapTableCfg->pstMapTable,
                         pstMapTableCfg->u32TableLen);
  if (ret)
    RKADK_LOGE("save sensor[%d] photo param failed", u32CamId);

  return ret;
}

static RKADK_S32 RKADK_PARAM_SaveStreamCfg(char *path, RKADK_U32 u32CamId,
                                           RKADK_STREAM_TYPE_E enStrmType) {
  int ret = 0;
  RKADK_PARAM_STREAM_CFG_S *pstStreamCfg;
  RKADK_PARAM_MAP_TYPE_E enMapType;
  RKADK_MAP_TABLE_CFG_S *pstMapTableCfg = NULL;

  RKADK_CHECK_CAMERAID(u32CamId, RKADK_FAILURE);

  switch (enStrmType) {
  case RKADK_STREAM_TYPE_LIVE:
    pstStreamCfg = &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stLiveCfg;
    enMapType = RKADK_PARAM_LIVE_MAP;
    break;
  case RKADK_STREAM_TYPE_PREVIEW:
    pstStreamCfg = &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stStreamCfg;
    enMapType = RKADK_PARAM_PREVIEW_MAP;
    break;
  default:
    RKADK_LOGW("unsupport enStrmType = %d", enStrmType);
    return -1;
  }

  pstMapTableCfg = RKADK_PARAM_GetMapTable(u32CamId, enMapType);
  RKADK_CHECK_POINTER(pstMapTableCfg, RKADK_FAILURE);

  ret = RKADK_Struct2Ini(path, &pstStreamCfg->attribute,
                         pstMapTableCfg->pstMapTable,
                         pstMapTableCfg->u32TableLen);
  if (ret)
    RKADK_LOGE("save sensor[%d] stream(enStrmType = %d) param failed",
               enStrmType, u32CamId);

  return ret;
}

static RKADK_S32 RKADK_PARAM_SaveRecCfg(char *path, RKADK_U32 u32CamId) {
  int ret = 0;
  RKADK_PARAM_REC_CFG_S *pstRecCfg;
  RKADK_MAP_TABLE_CFG_S *pstMapTableCfg = NULL;

  RKADK_CHECK_CAMERAID(u32CamId, RKADK_FAILURE);

  pstRecCfg = &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stRecCfg;

  pstMapTableCfg = RKADK_PARAM_GetMapTable(u32CamId, RKADK_PARAM_REC_MAP);
  RKADK_CHECK_POINTER(pstMapTableCfg, RKADK_FAILURE);

  ret = RKADK_Struct2Ini(path, pstRecCfg, pstMapTableCfg->pstMapTable,
                         pstMapTableCfg->u32TableLen);
  if (ret) {
    RKADK_LOGE("save sensor[%d] record param failed", u32CamId);
    return ret;
  }

  return 0;
}

static RKADK_S32 RKADK_PARAM_SaveRecTime(char *path, RKADK_U32 u32CamId,
                                         RKADK_STREAM_TYPE_E enStrmType) {
  int ret;
  RKADK_U32 index;
  RKADK_PARAM_REC_CFG_S *pstRecCfg;
  RKADK_MAP_TABLE_CFG_S *pstMapTableCfg = NULL;

  RKADK_CHECK_CAMERAID(u32CamId, RKADK_FAILURE);

  pstRecCfg = &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stRecCfg;
  switch (enStrmType) {
  case RKADK_STREAM_TYPE_VIDEO_MAIN:
    index = 0;
    pstMapTableCfg =
        RKADK_PARAM_GetMapTable(u32CamId, RKADK_PARAM_REC_MAIN_TIME_MAP);
    break;
  case RKADK_STREAM_TYPE_VIDEO_SUB:
    index = 1;
    pstMapTableCfg =
        RKADK_PARAM_GetMapTable(u32CamId, RKADK_PARAM_REC_SUB_TIME_MAP);
    break;
  default:
    return -1;
  }

  if (!pstMapTableCfg)
    return -1;

  ret = RKADK_Struct2Ini(path, &pstRecCfg->record_time_cfg[index],
                         pstMapTableCfg->pstMapTable,
                         pstMapTableCfg->u32TableLen);
  if (ret) {
    RKADK_LOGE("save sensor[%d] record time[%d] param failed", u32CamId, index);
    return ret;
  }

  return 0;
}

static RKADK_S32 RKADK_PARAM_SaveRecAttr(char *path, RKADK_U32 u32CamId,
                                         RKADK_STREAM_TYPE_E enStrmType) {
  int ret;
  RKADK_U32 index;
  RKADK_PARAM_REC_CFG_S *pstRecCfg;
  RKADK_MAP_TABLE_CFG_S *pstMapTableCfg = NULL;

  RKADK_CHECK_CAMERAID(u32CamId, RKADK_FAILURE);

  pstRecCfg = &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stRecCfg;
  switch (enStrmType) {
  case RKADK_STREAM_TYPE_VIDEO_MAIN:
    index = 0;
    pstMapTableCfg =
        RKADK_PARAM_GetMapTable(u32CamId, RKADK_PARAM_REC_MAIN_MAP);
    break;
  case RKADK_STREAM_TYPE_VIDEO_SUB:
    index = 1;
    pstMapTableCfg = RKADK_PARAM_GetMapTable(u32CamId, RKADK_PARAM_REC_SUB_MAP);
    break;
  default:
    return -1;
  }

  if (!pstMapTableCfg)
    return -1;

  ret = RKADK_Struct2Ini(path, &pstRecCfg->attribute[index],
                         pstMapTableCfg->pstMapTable,
                         pstMapTableCfg->u32TableLen);
  if (ret) {
    RKADK_LOGE("save sensor[%d] record attribute[%d] param failed", u32CamId,
               index);
    return ret;
  }

  return 0;
}

static RKADK_S32 RKADK_PARAM_SaveDispCfg(char *path, RKADK_U32 u32CamId) {
  int ret;
  RKADK_PARAM_DISP_CFG_S *pstDispCfg;
  RKADK_MAP_TABLE_CFG_S *pstMapTableCfg = NULL;

  RKADK_CHECK_CAMERAID(u32CamId, RKADK_FAILURE);

  pstDispCfg = &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stDispCfg;
  pstMapTableCfg = RKADK_PARAM_GetMapTable(u32CamId, RKADK_PARAM_DISP_MAP);
  RKADK_CHECK_POINTER(pstMapTableCfg, RKADK_FAILURE);

  ret = RKADK_Struct2Ini(path, pstDispCfg, pstMapTableCfg->pstMapTable,
                         pstMapTableCfg->u32TableLen);
  if (ret)
    RKADK_LOGE("save sensor[%d] display param failed", u32CamId);

  return ret;
}

static bool RKADK_PARAM_CheckCfg(RKADK_U32 *u32Value, RKADK_U32 u32DefValue,
                                 const char *tag) {
  if (*u32Value == 0) {
    RKADK_LOGW("%s: invalid value(%d), use default(%d)", tag, *u32Value,
               u32DefValue);
    *u32Value = u32DefValue;
    return true;
  } else {
    return false;
  }
}

static bool RKADK_PARAM_CheckCfgU32(RKADK_U32 *u32Value, RKADK_U32 u32Min,
                                    RKADK_U32 u32Max, RKADK_U32 u32DefValue,
                                    const char *tag) {
  if ((*u32Value > u32Max) || (*u32Value < u32Min)) {
    RKADK_LOGW("%s: invalid value(%d), use default(%d)", tag, *u32Value,
               u32DefValue);
    *u32Value = u32DefValue;
    return true;
  } else {
    return false;
  }
}

static bool RKADK_PARAM_CheckCfgStr(char *value, const char *defValue,
                                    RKADK_U32 S32ValueLen, const char *tag) {
  RKADK_U32 len;

  len = S32ValueLen > strlen(defValue) ? strlen(defValue) : S32ValueLen;
  if (!strlen(value)) {
    RKADK_LOGW("%s: invalid value(%s), use default(%s)", tag, value, defValue);
    strncpy(value, defValue, len);
    return true;
  } else {
    return false;
  }
}

static void RKADK_PARAM_CheckCommCfg(char *path) {
  bool change = false;
  RKADK_PARAM_COMM_CFG_S *pstCommCfg = &g_stPARAMCtx.stCfg.stCommCfg;

  change = RKADK_PARAM_CheckCfgU32(&pstCommCfg->sensor_count, 1,
                                   RKADK_MAX_SENSOR_CNT, 1, "sensor_count");
  change |= RKADK_PARAM_CheckCfgU32(&pstCommCfg->speaker_volume, 0, 100, 70,
                                    "speaker_volume");
  change |= RKADK_PARAM_CheckCfgU32(&pstCommCfg->mic_volume, 0, 100, 70,
                                    "mic_volume");

  if (change)
    RKADK_PARAM_SaveCommCfg(path);
}

static void RKADK_PARAM_CheckAudioCfg(char *path) {
  bool change = false;
  RKADK_PARAM_AUDIO_CFG_S *pstAudioCfg = &g_stPARAMCtx.stCfg.stAudioCfg;

  change = RKADK_PARAM_CheckCfgStr(pstAudioCfg->audio_node, AI_DEVICE_NAME,
                                   RKADK_BUFFER_LEN, "audio_node");
  change |= RKADK_PARAM_CheckCfgU32((RKADK_U32 *)&pstAudioCfg->bit_width,
                                    AUDIO_BIT_WIDTH_8, AUDIO_BIT_WIDTH_32,
                                    AUDIO_BIT_WIDTH, "bit_width");
  change |=
      RKADK_PARAM_CheckCfg(&pstAudioCfg->channels, AUDIO_CHANNEL, "channels");
  change |= RKADK_PARAM_CheckCfgU32((RKADK_U32 *)&pstAudioCfg->mic_type,
                                    RKADK_MIC_TYPE_LEFT, RKADK_MIC_TYPE_BOTH,
                                    RKADK_MIC_TYPE_LEFT, "mic_type");
  change |= RKADK_PARAM_CheckCfg(&pstAudioCfg->samplerate, AUDIO_SAMPLE_RATE,
                                 "samplerate");
  change |= RKADK_PARAM_CheckCfg(&pstAudioCfg->samples_per_frame,
                                 AUDIO_FRAME_COUNT, "samples_per_frame");
  change |=
      RKADK_PARAM_CheckCfg(&pstAudioCfg->bitrate, AUDIO_BIT_REAT, "bitrate");
  change |= RKADK_PARAM_CheckCfgU32((RKADK_U32 *)&pstAudioCfg->vqe_mode,
                                    RKADK_VQE_MODE_AI_TALK, RKADK_VQE_MODE_BUTT,
                                    RKADK_VQE_MODE_AI_RECORD, "vqe_mode");
  change |= RKADK_PARAM_CheckCfgU32((RKADK_U32 *)&pstAudioCfg->codec_type,
                                    RKADK_CODEC_TYPE_G711A, RKADK_CODEC_TYPE_PCM,
                                    RKADK_CODEC_TYPE_MP3, "codec_type");

  if (change)
    RKADK_PARAM_SaveAudioCfg(path);
}

static void RKADK_PARAM_CheckThumbCfg(char *path, RKADK_U32 u32CamId) {
  bool change = false;
  RKADK_PARAM_THUMB_CFG_S *pstThumbCfg =
    &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stThumbCfg;

  change = RKADK_PARAM_CheckCfgU32(&pstThumbCfg->thumb_width, 240, 1280,
                                   THUMB_WIDTH, "thumb_width");
  change |= RKADK_PARAM_CheckCfgU32(&pstThumbCfg->thumb_height, 180, 720,
                                    THUMB_HEIGHT, "thumb_height");
  change |= RKADK_PARAM_CheckCfgU32(&pstThumbCfg->photo_venc_chn, 0, VENC_MAX_CHN_NUM,
                                    THUMB_PHOTO_VENC_CHN, "thumb photo_venc_chn");
  change |= RKADK_PARAM_CheckCfgU32(&pstThumbCfg->record_main_venc_chn, 0, VENC_MAX_CHN_NUM,
                                    THUMB_RECORD_MAIN_VENC_CHN, "thumb record_main_venc_chn");
  change |= RKADK_PARAM_CheckCfgU32(&pstThumbCfg->record_sub_venc_chn, 0, VENC_MAX_CHN_NUM,
                                    THUMB_RECORD_SUB_VENC_CHN, "thumb record_sub_venc_chn");

  if (change)
    RKADK_PARAM_SaveThumbCfg(path, u32CamId);
}

static void RKADK_PARAM_CheckSensorCfg(char *path, RKADK_U32 u32CamId) {
  bool change = false;
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg =
      &g_stPARAMCtx.stCfg.stSensorCfg[u32CamId];

  change = RKADK_PARAM_CheckCfg(&pstSensorCfg->framerate, VIDEO_FRAME_RATE,
                                "framerate");

  if (change)
    RKADK_PARAM_SaveSensorCfg(path, u32CamId);
}

static void RKADK_PARAM_CheckStreamCfg(char *path, RKADK_U32 u32CamId,
                                       RKADK_STREAM_TYPE_E enStrmType) {
  bool change = false;
  RKADK_PARAM_VENC_ATTR_S *pstAttribute = NULL;

  switch (enStrmType) {
  case RKADK_STREAM_TYPE_LIVE:
    pstAttribute = &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stLiveCfg.attribute;
    break;
  case RKADK_STREAM_TYPE_PREVIEW:
    pstAttribute =
        &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stStreamCfg.attribute;
    break;
  default:
    RKADK_LOGW("unsupport enStrmType = %d", enStrmType);
    return;
  }

  if (strcmp(pstAttribute->rc_mode, "CBR") &&
      strcmp(pstAttribute->rc_mode, "VBR") &&
      strcmp(pstAttribute->rc_mode, "AVBR")) {
    memcpy(pstAttribute->rc_mode, "CBR", strlen("CBR"));
    change = true;
  }

  change |= RKADK_PARAM_CheckCfg(&pstAttribute->width, STREAM_VIDEO_WIDTH,
                                 "stream width");
  change |= RKADK_PARAM_CheckCfg(&pstAttribute->height, STREAM_VIDEO_HEIGHT,
                                 "stream height");
  change |= RKADK_PARAM_CheckCfg(&pstAttribute->bufsize,
                                 STREAM_VIDEO_WIDTH * STREAM_VIDEO_HEIGHT * 3 / 2,
                                 "stream bufsize");
  change |= RKADK_PARAM_CheckCfgU32(&pstAttribute->venc_chn, 0,
                                    VENC_MAX_CHN_NUM, 1, "stream venc_chn");
  change |= RKADK_PARAM_CheckCfgU32(&pstAttribute->vpss_chn, 0, VPSS_MAX_CHN_NUM,
                                    1, "stream vpss_chn");
  change |= RKADK_PARAM_CheckCfg(&pstAttribute->bitrate, 4 * 1024 * 1024,
                                 "stream bitrate");
  change |= RKADK_PARAM_CheckCfg(&pstAttribute->gop, VIDEO_GOP, "stream gop");
  change |= RKADK_PARAM_CheckCfg(&pstAttribute->profile, VIDEO_PROFILE,
                                 "stream profile");
  change |= RKADK_PARAM_CheckCfgU32(
      (RKADK_U32 *)&pstAttribute->codec_type, RKADK_CODEC_TYPE_H264,
      RKADK_CODEC_TYPE_MJPEG, RKADK_CODEC_TYPE_H264, "stream codec_type");

  if (change)
    RKADK_PARAM_SaveStreamCfg(path, u32CamId, enStrmType);
}

static void RKADK_PARAM_CheckPhotoCfg(char *path, RKADK_U32 u32CamId) {
  bool change = false;
  RKADK_PARAM_PHOTO_CFG_S *pstPhotoCfg =
      &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stPhotoCfg;

  change = RKADK_PARAM_CheckCfg(&pstPhotoCfg->image_width, PHOTO_VIDEO_WIDTH,
                                "photo width");
  change |= RKADK_PARAM_CheckCfg(&pstPhotoCfg->image_height, PHOTO_VIDEO_HEIGHT,
                                 "photo height");
  change |= RKADK_PARAM_CheckCfg(&pstPhotoCfg->snap_num, 1, "photo snap_num");
  change |= RKADK_PARAM_CheckCfgU32(&pstPhotoCfg->venc_chn, 0, VENC_MAX_CHN_NUM,
                                    2, "photo venc_chn");
  change |= RKADK_PARAM_CheckCfgU32(&pstPhotoCfg->vpss_chn, 0, VPSS_MAX_CHN_NUM,
                                    2, "photo vpss_chn");

  if (change)
    RKADK_PARAM_SavePhotoCfg(path, u32CamId);
}

static void RKADK_PARAM_CheckRecCfg(char *path, RKADK_U32 u32CamId) {
  bool change = false;
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg =
      &g_stPARAMCtx.stCfg.stSensorCfg[u32CamId];
  RKADK_PARAM_REC_CFG_S *pstRecCfg =
      &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stRecCfg;

  change = RKADK_PARAM_CheckCfgU32((RKADK_U32 *)&pstRecCfg->record_type,
                                   RKADK_REC_TYPE_NORMAL, RKADK_REC_TYPE_LAPSE,
                                   RKADK_REC_TYPE_NORMAL, "record_type");

  change |= RKADK_PARAM_CheckCfgU32((RKADK_U32 *)&pstRecCfg->file_type,
                                    RKADK_MUXER_TYPE_MP4, RKADK_MUXER_TYPE_FLV,
                                    RKADK_MUXER_TYPE_MP4, "file_type");

  change |= RKADK_PARAM_CheckCfgU32(
      (RKADK_U32 *)&pstRecCfg->pre_record_mode, RKADK_MUXER_PRE_RECORD_NONE,
      RKADK_MUXER_PRE_RECORD_SINGLE, RKADK_MUXER_PRE_RECORD_NONE,
      "pre_record_mode");
  change |= RKADK_PARAM_CheckCfgU32(&pstRecCfg->lapse_multiple, 1,
                                    pstSensorCfg->framerate,
                                    pstSensorCfg->framerate, "lapse_multiple");
  change |=
      RKADK_PARAM_CheckCfgU32(&pstRecCfg->file_num, 1, RECORD_FILE_NUM_MAX,
                              RECORD_FILE_NUM_MAX, "file_num");

  if (change)
    RKADK_PARAM_SaveRecCfg(path, u32CamId);

  for (int i = 0; i < (int)pstRecCfg->file_num; i++) {
    RKADK_STREAM_TYPE_E enStrmType;
    RKADK_PARAM_VENC_ATTR_S *pstAttribute = &pstRecCfg->attribute[i];
    RKADK_PARAM_REC_TIME_CFG_S *pstRecTimeCfg = &pstRecCfg->record_time_cfg[i];

    // check record time
    change =
        RKADK_PARAM_CheckCfg(&pstRecTimeCfg->record_time, 60, "record_time");
    change |=
        RKADK_PARAM_CheckCfg(&pstRecTimeCfg->splite_time, 60, "splite_time");
    change |= RKADK_PARAM_CheckCfg(&pstRecTimeCfg->lapse_interval, 60,
                                   "lapse_interval");

    if (change)
      RKADK_PARAM_SaveRecTime(path, u32CamId, enStrmType);

    // check venc attribute
    change = false;
    if (strcmp(pstAttribute->rc_mode, "CBR") &&
        strcmp(pstAttribute->rc_mode, "VBR") &&
        strcmp(pstAttribute->rc_mode, "AVBR")) {
      memcpy(pstAttribute->rc_mode, "CBR", strlen("CBR"));
      change = true;
    }

    RKADK_U32 u32DefWidth, u32DefHeight, u32DefChn, u32DefBitrate;
    if (i == 0) {
      enStrmType = RKADK_STREAM_TYPE_VIDEO_MAIN;
      u32DefWidth = RECORD_VIDEO_WIDTH;
      u32DefHeight = RECORD_VIDEO_HEIGHT;
      u32DefChn = 0;
      u32DefBitrate = 30 * 1024 * 1024;
    } else {
      enStrmType = RKADK_STREAM_TYPE_VIDEO_SUB;
      u32DefWidth = RECORD_VIDEO_WIDTH_S;
      u32DefHeight = RECORD_VIDEO_HEIGHT_S;
      u32DefChn = 1;
      u32DefBitrate = 4 * 1024 * 1024;
    }

    change |=
        RKADK_PARAM_CheckCfg(&pstAttribute->width, u32DefWidth, "rec width");
    change |=
        RKADK_PARAM_CheckCfg(&pstAttribute->height, u32DefHeight, "rec height");
    change |=
        RKADK_PARAM_CheckCfg(&pstAttribute->bufsize, u32DefWidth * u32DefHeight * 3 / 2, "rec bufsize");
    change |=
        RKADK_PARAM_CheckCfgU32(&pstAttribute->venc_chn, 0, VENC_MAX_CHN_NUM,
                                u32DefChn, "rec venc_chn");
    change |= RKADK_PARAM_CheckCfgU32(
        &pstAttribute->vpss_chn, 0, VPSS_MAX_CHN_NUM, u32DefChn, "rec vpss_chn");
    change |= RKADK_PARAM_CheckCfg(&pstAttribute->bitrate, u32DefBitrate,
                                   "rec bitrate");
    change |= RKADK_PARAM_CheckCfg(&pstAttribute->gop, VIDEO_GOP, "rec gop");
    change |= RKADK_PARAM_CheckCfg(&pstAttribute->profile, VIDEO_PROFILE,
                                   "rec profile");
    change |= RKADK_PARAM_CheckCfgU32(
        (RKADK_U32 *)&pstAttribute->codec_type, RKADK_CODEC_TYPE_H264,
        RKADK_CODEC_TYPE_MJPEG, RKADK_CODEC_TYPE_H264, "rec codec_type");

    if (change)
      RKADK_PARAM_SaveRecAttr(path, u32CamId, enStrmType);
  }
}

static void RKADK_PARAM_CheckViCfg(char *path, RKADK_U32 u32CamId,
                                   RKADK_U32 index) {
  bool change = false;
  RKADK_U32 u32ChnId;
  const char *pixFmt = "NV12";
  const char *deviceName;
  const char *module;
  RKADK_U32 u32DefWidth, u32DefHeight;
  RKADK_PARAM_VI_CFG_S *pstViCfg =
      &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stViCfg[index];

  u32ChnId = index + (u32CamId * RKADK_ISPP_VI_NODE_CNT);
  if (u32ChnId >= VI_MAX_CHN_NUM)
    u32ChnId = VI_MAX_CHN_NUM - 1;

  switch (index) {
  case 0:
    deviceName = DEF_DEVICE_NEME_VI_0;
    module = DEF_MODULE_VI_0;
    u32DefWidth = RECORD_VIDEO_WIDTH;
    u32DefHeight = RECORD_VIDEO_HEIGHT;
    break;
  case 1:
    deviceName = DEF_DEVICE_NEME_VI_1;
    module = DEF_MODULE_VI_1;
    u32DefWidth = DEF_WIDTH_VI_1;
    u32DefHeight = DEF_HEIGHT_VI_1;
    break;
  case 2:
    deviceName = DEF_DEVICE_NEME_VI_2;
    module = DEF_MODULE_VI_2;
    u32DefWidth = DEF_WIDTH_VI_2;
    u32DefHeight = DEF_HEIGHT_VI_2;
    break;
  case 3:
    deviceName = DEF_DEVICE_NEME_VI_3;
    module = DEF_MODULE_VI_3;
    u32DefWidth = STREAM_VIDEO_WIDTH;
    u32DefHeight = STREAM_VIDEO_HEIGHT;
    break;
  default:
    break;
  }

  change = RKADK_PARAM_CheckCfg(&pstViCfg->width, u32DefWidth, "vi width");
  change |= RKADK_PARAM_CheckCfg(&pstViCfg->height, u32DefHeight, "vi height");
  change |= RKADK_PARAM_CheckCfgU32(&pstViCfg->chn_id, 0, VI_MAX_CHN_NUM,
                                    u32ChnId, "vi chn_id");
  change |= RKADK_PARAM_CheckCfgStr(pstViCfg->device_name, deviceName,
                                    MAX_VI_ENTITY_NAME_LEN, "vi device_name");
  change |= RKADK_PARAM_CheckCfgStr(pstViCfg->module, module, RKADK_BUFFER_LEN,
                                    "vi module");
  change |= RKADK_PARAM_CheckCfg(&pstViCfg->buf_cnt, VIDEO_BUFFER_COUNT, "vi buf_cnt");
  change |= RKADK_PARAM_CheckCfgStr(pstViCfg->pix_fmt, pixFmt,
                                    RKADK_PIX_FMT_LEN, "vi pix_fmt");

  if (change)
    RKADK_PARAM_SaveViCfg(path, index, u32CamId);
}

static void RKADK_PARAM_CheckDispCfg(char *path, RKADK_U32 u32CamId) {
  bool change = false;
  RKADK_PARAM_DISP_CFG_S *pstDispCfg =
      &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stDispCfg;

  change =
      RKADK_PARAM_CheckCfg(&pstDispCfg->width, DISP_WIDTH, "display width");
  change |=
      RKADK_PARAM_CheckCfg(&pstDispCfg->height, DISP_HEIGHT, "display height");

  if (pstDispCfg->rotaion != 0 && pstDispCfg->rotaion != 90 &&
      pstDispCfg->rotaion != 180 && pstDispCfg->rotaion != 270) {
    RKADK_LOGW("invalid vpss rotaion(%d), use default(0)", pstDispCfg->rotaion);
    pstDispCfg->rotaion = 0;
    change = true;
  }

  if (pstDispCfg->enable_buf_pool)
    change |= RKADK_PARAM_CheckCfg(&pstDispCfg->buf_pool_cnt, 3,
                                   "display buf_pool_cnt");
  change |= RKADK_PARAM_CheckCfgU32(&pstDispCfg->vpss_chn, 0, VPSS_MAX_CHN_NUM, 3,
                                    "display vpss_chn");
  change |= RKADK_PARAM_CheckCfgStr(pstDispCfg->device_node, "/dev/dri/card0",
                                    RKADK_BUFFER_LEN, "display vo device_node");
  change |= RKADK_PARAM_CheckCfgStr(pstDispCfg->img_type, "RGB888",
                                    RKADK_PIX_FMT_LEN, "display pix_fmt");

#ifdef RKADK_ENABLE_DISP
  change |= RKADK_PARAM_CheckCfgU32((RKADK_U32 *)&pstDispCfg->plane_type,
                                    VO_PLANE_PRIMARY, VO_PLANE_CURSOR,
                                    VO_PLANE_PRIMARY, "display plane_type");
#endif
  change |= RKADK_PARAM_CheckCfgU32(&pstDispCfg->vo_chn, 0, VO_MAX_CHN_NUM, 0,
                                    "display vo_chn");

  if (change)
    RKADK_PARAM_SavePhotoCfg(path, u32CamId);
}

static bool RKADK_PARAM_CheckVersion(char *path) {
  int ret;
  char version[RKADK_BUFFER_LEN];
  RKADK_PARAM_CFG_S *pstCfg = &g_stPARAMCtx.stCfg;

  memset(version, 0, RKADK_BUFFER_LEN);
  memset(&pstCfg->stVersion, 0, sizeof(RKADK_PARAM_VERSION_S));

  ret = RKADK_Ini2Struct(
      (char *)RKADK_DEFPARAM_PATH, &pstCfg->stVersion, g_stVersionMapTable,
      sizeof(g_stVersionMapTable) / sizeof(RKADK_SI_CONFIG_MAP_S));
  if (ret) {
    RKADK_LOGE("load default ini version failed");
    return false;
  }
  strncpy(version, pstCfg->stVersion.version,
          strlen(pstCfg->stVersion.version));

  memset(&pstCfg->stVersion, 0, sizeof(RKADK_PARAM_VERSION_S));
  ret = RKADK_Ini2Struct(path, &pstCfg->stVersion, g_stVersionMapTable,
                         sizeof(g_stVersionMapTable) /
                             sizeof(RKADK_SI_CONFIG_MAP_S));
  if (ret) {
    RKADK_LOGE("load setting ini version failed");
    return false;
  }

  if (strcmp(version, pstCfg->stVersion.version)) {
    RKADK_LOGW("default ini version[%s] != setting ini version[%s]", version,
               pstCfg->stVersion.version);
    return false;
  }

  return true;
}

static void RKADK_PARAM_DefCommCfg(char *path) {
  RKADK_PARAM_COMM_CFG_S *pstCommCfg = &g_stPARAMCtx.stCfg.stCommCfg;

  memset(pstCommCfg, 0, sizeof(RKADK_PARAM_COMM_CFG_S));
  pstCommCfg->sensor_count = 1;
  pstCommCfg->rec_mute = false;
  pstCommCfg->speaker_volume = 70;
  pstCommCfg->mic_volume = 70;
  pstCommCfg->vpss_devcie = 1;
  RKADK_PARAM_SaveCommCfg(path);
}

static void RKADK_PARAM_DefAudioCfg(char *path) {
  RKADK_PARAM_AUDIO_CFG_S *pstAudioCfg = &g_stPARAMCtx.stCfg.stAudioCfg;

  memset(pstAudioCfg, 0, sizeof(RKADK_PARAM_AUDIO_CFG_S));
  memcpy(pstAudioCfg->audio_node, AI_DEVICE_NAME, strlen(AI_DEVICE_NAME));
  pstAudioCfg->bit_width = AUDIO_BIT_WIDTH;
  pstAudioCfg->channels = AUDIO_CHANNEL;
  pstAudioCfg->mic_type = AUDIO_MIC_TYPE;
  pstAudioCfg->samplerate = AUDIO_SAMPLE_RATE;
  pstAudioCfg->samples_per_frame = AUDIO_FRAME_COUNT;
  pstAudioCfg->bitrate = AUDIO_BIT_REAT;
  pstAudioCfg->vqe_mode = RKADK_VQE_MODE_AI_RECORD;
  pstAudioCfg->codec_type = RKADK_CODEC_TYPE_MP3;
  RKADK_PARAM_SaveAudioCfg(path);
}

static void RKADK_PARAM_DefSensorCfg(RKADK_U32 u32CamId, char *path) {
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg = &g_stPARAMCtx.stCfg.stSensorCfg[0];

  memset(pstSensorCfg, 0, sizeof(RKADK_PARAM_SENSOR_CFG_S));
  if (u32CamId == 0) {
    pstSensorCfg->max_width = SENSOR_MAX_WIDTH;
    pstSensorCfg->max_height = SENSOR_MAX_HEIGHT;
  }

  pstSensorCfg->used_isp = true;
  pstSensorCfg->framerate = VIDEO_FRAME_RATE;
  pstSensorCfg->flip = false;
  pstSensorCfg->mirror = false;
  pstSensorCfg->enable_wrap = false;
  pstSensorCfg->wrap_buf_line = SENSOR_MAX_HEIGHT / 4;
  RKADK_PARAM_SaveSensorCfg(path, u32CamId);
}

static void RKADK_PARAM_DefViCfg(RKADK_U32 u32CamId, RKADK_U32 u32ViIndex,
                                 char *path) {
  RKADK_PARAM_VI_CFG_S *pstViCfg =
      &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stViCfg[u32ViIndex];

  memset(pstViCfg, 0, sizeof(RKADK_PARAM_VI_CFG_S));
  pstViCfg->chn_id = u32ViIndex + (u32CamId * RKADK_ISPP_VI_NODE_CNT);
  if (pstViCfg->chn_id >= VI_MAX_CHN_NUM)
    pstViCfg->chn_id = VI_MAX_CHN_NUM - 1;

  pstViCfg->buf_cnt = VIDEO_BUFFER_COUNT;
  memcpy(pstViCfg->pix_fmt, "NV12", strlen("NV12"));

  switch (u32ViIndex) {
  case 0:
    memcpy(pstViCfg->device_name, DEF_DEVICE_NEME_VI_0, strlen(DEF_DEVICE_NEME_VI_0));
    pstViCfg->width = RECORD_VIDEO_WIDTH;
    pstViCfg->height = RECORD_VIDEO_HEIGHT;
    memcpy(pstViCfg->module, DEF_MODULE_VI_0, strlen(DEF_MODULE_VI_0));
    break;
  case 1:
    memcpy(pstViCfg->device_name, DEF_DEVICE_NEME_VI_1, strlen(DEF_DEVICE_NEME_VI_1));
    pstViCfg->width = DEF_WIDTH_VI_1;
    pstViCfg->height = DEF_HEIGHT_VI_1;
    memcpy(pstViCfg->module, DEF_MODULE_VI_1, strlen(DEF_MODULE_VI_1));
    break;
  case 2:
    memcpy(pstViCfg->device_name, DEF_DEVICE_NEME_VI_2, strlen(DEF_DEVICE_NEME_VI_2));
    pstViCfg->width = DEF_WIDTH_VI_2;
    pstViCfg->height = DEF_HEIGHT_VI_2;
    memcpy(pstViCfg->module, DEF_MODULE_VI_2, strlen(DEF_MODULE_VI_2));
    break;
  case 3:
    memcpy(pstViCfg->device_name, DEF_DEVICE_NEME_VI_3, strlen(DEF_DEVICE_NEME_VI_3));
    pstViCfg->width = STREAM_VIDEO_WIDTH;
    pstViCfg->height = STREAM_VIDEO_HEIGHT;
    memcpy(pstViCfg->module, DEF_MODULE_VI_3, strlen(DEF_MODULE_VI_3));
    break;
  default:
    RKADK_LOGE("Nonsupport vi index: %d", u32ViIndex);
    return;
  }

  RKADK_PARAM_SaveViCfg(path, u32ViIndex, u32CamId);
}

static void RKADK_PARAM_DefRecTime(RKADK_U32 u32CamId,
                                   RKADK_STREAM_TYPE_E enStrmType, char *path) {
  RKADK_PARAM_REC_TIME_CFG_S *pstTimeCfg = NULL;

  if (u32CamId >= RKADK_MAX_SENSOR_CNT) {
    RKADK_LOGE("invalid camera id: %d", u32CamId);
    return;
  }

  if (enStrmType == RKADK_STREAM_TYPE_VIDEO_MAIN)
    pstTimeCfg =
        &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stRecCfg.record_time_cfg[0];
  else
    pstTimeCfg =
        &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stRecCfg.record_time_cfg[1];

  memset(pstTimeCfg, 0, sizeof(RKADK_PARAM_REC_TIME_CFG_S));
  pstTimeCfg->record_time = 60;
  pstTimeCfg->splite_time = 60;
  pstTimeCfg->lapse_interval = 60;

  RKADK_PARAM_SaveRecTime(path, u32CamId, enStrmType);
}

static void RKADK_PARAM_DefRecAttr(RKADK_U32 u32CamId,
                                   RKADK_STREAM_TYPE_E enStrmType, char *path) {
  RKADK_U32 u32Width, u32Height;
  RKADK_U32 u32Chn, u32VpssGrp, u32VpssChn;
  RKADK_U32 u32Bitrate;
  RKADK_PARAM_VENC_ATTR_S *pstAttr = NULL;
  RKADK_PARAM_REC_CFG_S *pstRecCfg;

  if (u32CamId >= RKADK_MAX_SENSOR_CNT) {
    RKADK_LOGE("invalid camera id: %d", u32CamId);
    return;
  }

  pstRecCfg = &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stRecCfg;
  switch (enStrmType) {
  case RKADK_STREAM_TYPE_VIDEO_MAIN:
    pstAttr = &pstRecCfg->attribute[0];
    u32Width = RECORD_VIDEO_WIDTH;
    u32Height = RECORD_VIDEO_HEIGHT;
    u32Chn = 0;
    u32VpssGrp = 0;
    u32VpssChn = 0;
    u32Bitrate = 30 * 1024 * 1024;
    break;
  case RKADK_STREAM_TYPE_VIDEO_SUB:
    pstAttr = &pstRecCfg->attribute[1];
    u32Width = RECORD_VIDEO_WIDTH_S;
    u32Height = RECORD_VIDEO_HEIGHT_S;
    u32Chn = 1;
    u32VpssGrp = 2;
    u32VpssChn = 0;
    u32Bitrate = 4 * 1024 * 1024;
    break;
  default:
    return;
  }

  memset(pstAttr, 0, sizeof(RKADK_PARAM_VENC_ATTR_S));
  pstAttr->width = u32Width;
  pstAttr->height = u32Height;
  pstAttr->bufsize = u32Width * u32Height * 3 / 2;
  pstAttr->venc_chn = u32Chn;
  pstAttr->vpss_grp = u32VpssGrp;
  pstAttr->vpss_chn = u32VpssChn;
  pstAttr->bitrate = u32Bitrate;
  pstAttr->gop = VIDEO_GOP;
  pstAttr->profile = VIDEO_PROFILE;
  pstAttr->codec_type = RKADK_CODEC_TYPE_H264;
  memcpy(pstAttr->rc_mode, "CBR", strlen("CBR"));

  pstAttr->venc_param.max_qp = -1;
  pstAttr->venc_param.min_qp = -1;

  RKADK_PARAM_SaveRecAttr(path, u32CamId, enStrmType);
}

static void RKADK_PARAM_DefRecCfg(RKADK_U32 u32CamId, char *path) {
  RKADK_PARAM_REC_CFG_S *pstRecCfg =
      &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stRecCfg;

  memset(pstRecCfg, 0, sizeof(RKADK_PARAM_REC_CFG_S));
  pstRecCfg->record_type = RKADK_REC_TYPE_NORMAL;
  pstRecCfg->file_type = RKADK_MUXER_TYPE_MP4;
  pstRecCfg->pre_record_time = 0;
  pstRecCfg->pre_record_mode = RKADK_MUXER_PRE_RECORD_NONE;
  pstRecCfg->lapse_multiple = VIDEO_FRAME_RATE;
  pstRecCfg->file_num = 1;
  RKADK_PARAM_SaveRecCfg(path, u32CamId);
}

static void RKADK_PARAM_DefPhotoCfg(RKADK_U32 u32CamId, char *path) {
  RKADK_PARAM_PHOTO_CFG_S *pstPhotoCfg =
      &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stPhotoCfg;

  memset(pstPhotoCfg, 0, sizeof(RKADK_PARAM_PHOTO_CFG_S));
  pstPhotoCfg->snap_num = 1;
  pstPhotoCfg->image_width = PHOTO_VIDEO_WIDTH;
  pstPhotoCfg->image_height = PHOTO_VIDEO_HEIGHT;
  pstPhotoCfg->venc_chn = 2;
  pstPhotoCfg->vpss_grp = 0;
  pstPhotoCfg->vpss_chn = 0;
  pstPhotoCfg->enable_combo = false;
  pstPhotoCfg->combo_venc_chn = 0;
  pstPhotoCfg->qfactor = 70;

  RKADK_PARAM_SavePhotoCfg(path, u32CamId);
}

static void RKADK_PARAM_DefStreamCfg(RKADK_U32 u32CamId, char *path,
                                     RKADK_STREAM_TYPE_E enStrmType) {
  RKADK_PARAM_STREAM_CFG_S *pstStreamCfg = NULL;

  switch (enStrmType) {
  case RKADK_STREAM_TYPE_LIVE:
    pstStreamCfg = &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stLiveCfg;
    break;
  case RKADK_STREAM_TYPE_PREVIEW:
    pstStreamCfg = &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stStreamCfg;
    break;
  default:
    RKADK_LOGW("unsupport enStrmType = %d", enStrmType);
    return;
  }

  memset(pstStreamCfg, 0, sizeof(RKADK_PARAM_STREAM_CFG_S));
  pstStreamCfg->attribute.width = STREAM_VIDEO_WIDTH;
  pstStreamCfg->attribute.height = STREAM_VIDEO_HEIGHT;
  pstStreamCfg->attribute.bufsize = STREAM_VIDEO_WIDTH * STREAM_VIDEO_HEIGHT * 3 / 2;
  pstStreamCfg->attribute.venc_chn = 1;
  pstStreamCfg->attribute.vpss_grp = 2;
  pstStreamCfg->attribute.vpss_chn = 0;
  pstStreamCfg->attribute.gop = VIDEO_GOP;
  pstStreamCfg->attribute.bitrate = 10 * 1024;
  pstStreamCfg->attribute.profile = VIDEO_PROFILE;
  pstStreamCfg->attribute.codec_type = RKADK_CODEC_TYPE_H264;
  memcpy(pstStreamCfg->attribute.rc_mode, "VBR", strlen("VBR"));

  pstStreamCfg->attribute.venc_param.max_qp = -1;
  pstStreamCfg->attribute.venc_param.min_qp = -1;

  RKADK_PARAM_SaveStreamCfg(path, u32CamId, enStrmType);
}

static void RKADK_PARAM_DefDispCfg(RKADK_U32 u32CamId, char *path) {
  RKADK_PARAM_DISP_CFG_S *pstDispCfg =
      &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stDispCfg;

  memset(pstDispCfg, 0, sizeof(RKADK_PARAM_DISP_CFG_S));
  if (u32CamId == 0) {
    pstDispCfg->width = DISP_WIDTH;
    pstDispCfg->height = DISP_HEIGHT;
    // vpss
    pstDispCfg->enable_buf_pool = true;
    pstDispCfg->buf_pool_cnt = 3;
    pstDispCfg->rotaion = 90;
    pstDispCfg->vpss_grp = 2;
    pstDispCfg->vpss_chn = 0;
    // vo
    memcpy(pstDispCfg->device_node, "/dev/dri/card0", strlen("/dev/dri/card0"));
    memcpy(pstDispCfg->img_type, "RGB888", strlen("RGB888"));
#ifdef RKADK_ENABLE_DISP
    pstDispCfg->plane_type = VO_PLANE_PRIMARY;
#endif
    pstDispCfg->z_pos = 0;
    pstDispCfg->vo_chn = 0;
  }

  RKADK_PARAM_SaveDispCfg(path, u32CamId);
}

static void RKADK_PARAM_DefThumbCfg(RKADK_U32 u32CamId, char *path) {
  RKADK_PARAM_THUMB_CFG_S *pstThumbCfg =
    &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stThumbCfg;

  memset(pstThumbCfg, 0, sizeof(RKADK_PARAM_THUMB_CFG_S));
  pstThumbCfg->thumb_width = THUMB_WIDTH;
  pstThumbCfg->thumb_height = THUMB_HEIGHT;
  pstThumbCfg->photo_venc_chn = THUMB_PHOTO_VENC_CHN;
  pstThumbCfg->record_main_venc_chn = THUMB_RECORD_MAIN_VENC_CHN;
  pstThumbCfg->record_sub_venc_chn = THUMB_RECORD_SUB_VENC_CHN;
  pstThumbCfg->vpss_grp = THUMB_VPSS_GRP;
  pstThumbCfg->vpss_chn = THUMB_VPSS_CHN;
  RKADK_PARAM_SaveThumbCfg(path, u32CamId);
}

static void RKADK_PARAM_Dump() {
  int i, j;
  RKADK_PARAM_CFG_S *pstCfg = &g_stPARAMCtx.stCfg;

  printf("Version: %s\n", pstCfg->stVersion.version);

  printf("Common Config\n");
  printf("\tsensor_count: %d\n", pstCfg->stCommCfg.sensor_count);
  printf("\trec_mute: %d\n", pstCfg->stCommCfg.rec_mute);
  printf("\tspeaker_volume: %d\n", pstCfg->stCommCfg.speaker_volume);
  printf("\tmic_volume: %d\n", pstCfg->stCommCfg.mic_volume);

  printf("Audio Config\n");
  printf("\taudio_node: %s\n", pstCfg->stAudioCfg.audio_node);
  printf("\tbit_width: %d\n", pstCfg->stAudioCfg.bit_width);
  printf("\tchannels: %d\n", pstCfg->stAudioCfg.channels);
  printf("\tmic_type: %d\n", pstCfg->stAudioCfg.mic_type);
  printf("\tsamplerate: %d\n", pstCfg->stAudioCfg.samplerate);
  printf("\tsamples_per_frame: %d\n", pstCfg->stAudioCfg.samples_per_frame);
  printf("\tbitrate: %d\n", pstCfg->stAudioCfg.bitrate);
  printf("\tvqe_mode: %d\n", pstCfg->stAudioCfg.vqe_mode);
  printf("\tcodec_type: %d\n", pstCfg->stAudioCfg.codec_type);

  for (i = 0; i < (int)pstCfg->stCommCfg.sensor_count; i++) {
    printf("Sensor[%d] Config\n", i);
    printf("\tsensor[%d] used_isp: %d\n", i, pstCfg->stSensorCfg[i].used_isp);
    printf("\tsensor[%d] max_width: %d\n", i, pstCfg->stSensorCfg[i].max_width);
    printf("\tsensor[%d] max_height: %d\n", i,
           pstCfg->stSensorCfg[i].max_height);
    printf("\tsensor[%d] framerate: %d\n", i, pstCfg->stSensorCfg[i].framerate);
    printf("\tsensor[%d] flip: %d\n", i, pstCfg->stSensorCfg[i].flip);
    printf("\tsensor[%d] mirror: %d\n", i, pstCfg->stSensorCfg[i].mirror);
    printf("\tsensor[%d] enable_wrap: %d\n", i, pstCfg->stSensorCfg[i].enable_wrap);
    printf("\tsensor[%d] wrap_buf_line: %d\n", i, pstCfg->stSensorCfg[i].wrap_buf_line);

    printf("\tVI Config\n");
    for (j = 0; j < RKADK_ISPP_VI_NODE_CNT; j++) {
      printf("\t\tsensor[%d] VI[%d] device_name: %s\n", i, j,
             pstCfg->stMediaCfg[i].stViCfg[j].device_name);
      printf("\t\tsensor[%d] VI[%d] module: %s\n", i, j,
             pstCfg->stMediaCfg[i].stViCfg[j].module);
      printf("\t\tsensor[%d] VI[%d] chn_id: %d\n", i, j,
             pstCfg->stMediaCfg[i].stViCfg[j].chn_id);
      printf("\t\tsensor[%d] VI[%d] buf_cnt: %d\n", i, j,
             pstCfg->stMediaCfg[i].stViCfg[j].buf_cnt);
      printf("\t\tsensor[%d] VI[%d] pix_fmt: %s\n", i, j,
             pstCfg->stMediaCfg[i].stViCfg[j].pix_fmt);
      printf("\t\tsensor[%d] VI[%d] width: %d\n", i, j,
             pstCfg->stMediaCfg[i].stViCfg[j].width);
      printf("\t\tsensor[%d] VI[%d] height: %d\n", i, j,
             pstCfg->stMediaCfg[i].stViCfg[j].height);
    }

    printf("\tRecord Config\n");
    printf("\t\tsensor[%d] stRecCfg record_type: %d\n", i,
           pstCfg->stMediaCfg[i].stRecCfg.record_type);
    printf("\t\tsensor[%d] stRecCfg file_type: %d\n", i,
           pstCfg->stMediaCfg[i].stRecCfg.file_type);
    printf("\t\tsensor[%d] stRecCfg pre_record_time: %d\n", i,
           pstCfg->stMediaCfg[i].stRecCfg.pre_record_time);
    printf("\t\tsensor[%d] stRecCfg pre_record_mode: %d\n", i,
           pstCfg->stMediaCfg[i].stRecCfg.pre_record_mode);
    printf("\t\tsensor[%d] stRecCfg lapse_multiple: %d\n", i,
           pstCfg->stMediaCfg[i].stRecCfg.lapse_multiple);
    printf("\t\tsensor[%d] stRecCfg file_num: %d\n", i,
           pstCfg->stMediaCfg[i].stRecCfg.file_num);

    for (j = 0; j < (int)pstCfg->stMediaCfg[i].stRecCfg.file_num; j++) {
      printf("\t\tsensor[%d] stRecCfg record_time: %d\n", i,
             pstCfg->stMediaCfg[i].stRecCfg.record_time_cfg[j].record_time);
      printf("\t\tsensor[%d] stRecCfg splite_time: %d\n", i,
             pstCfg->stMediaCfg[i].stRecCfg.record_time_cfg[j].splite_time);
      printf("\t\tsensor[%d] stRecCfg lapse_interval: %d\n", i,
             pstCfg->stMediaCfg[i].stRecCfg.record_time_cfg[j].lapse_interval);
      printf("\t\tsensor[%d] stRecCfg[%d] width: %d\n", i, j,
             pstCfg->stMediaCfg[i].stRecCfg.attribute[j].width);
      printf("\t\tsensor[%d] stRecCfg[%d] height: %d\n", i, j,
             pstCfg->stMediaCfg[i].stRecCfg.attribute[j].height);
      printf("\t\tsensor[%d] stRecCfg[%d] bufsize: %d\n", i, j,
             pstCfg->stMediaCfg[i].stRecCfg.attribute[j].bufsize);
      printf("\t\tsensor[%d] stRecCfg[%d] bitrate: %d\n", i, j,
             pstCfg->stMediaCfg[i].stRecCfg.attribute[j].bitrate);
      printf("\t\tsensor[%d] stRecCfg[%d] gop: %d\n", i, j,
             pstCfg->stMediaCfg[i].stRecCfg.attribute[j].gop);
      printf("\t\tsensor[%d] stRecCfg[%d] profile: %d\n", i, j,
             pstCfg->stMediaCfg[i].stRecCfg.attribute[j].profile);
      printf("\t\tsensor[%d] stRecCfg[%d] venc_chn: %d\n", i, j,
             pstCfg->stMediaCfg[i].stRecCfg.attribute[j].venc_chn);
      printf("\t\tsensor[%d] stRecCfg[%d] vpss_grp: %d\n", i, j,
             pstCfg->stMediaCfg[i].stRecCfg.attribute[j].vpss_grp);
      printf("\t\tsensor[%d] stRecCfg[%d] vpss_chn: %d\n", i, j,
             pstCfg->stMediaCfg[i].stRecCfg.attribute[j].vpss_chn);
      printf("\t\tsensor[%d] stRecCfg[%d] codec_type: %d\n", i, j,
             pstCfg->stMediaCfg[i].stRecCfg.attribute[j].codec_type);
      printf("\t\tsensor[%d] stRecCfg[%d] rc_mode: %s\n", i, j,
             pstCfg->stMediaCfg[i].stRecCfg.attribute[j].rc_mode);
      printf("\t\tsensor[%d] stRecCfg[%d] first_frame_qp: %d\n", i, j,
             pstCfg->stMediaCfg[i].stRecCfg.attribute[j].venc_param.first_frame_qp);
      printf("\t\tsensor[%d] stRecCfg[%d] qp_step: %d\n", i, j,
             pstCfg->stMediaCfg[i].stRecCfg.attribute[j].venc_param.qp_step);
      printf("\t\tsensor[%d] stRecCfg[%d] max_qp: %d\n", i, j,
             pstCfg->stMediaCfg[i].stRecCfg.attribute[j].venc_param.max_qp);
      printf("\t\tsensor[%d] stRecCfg[%d] min_qp: %d\n", i, j,
             pstCfg->stMediaCfg[i].stRecCfg.attribute[j].venc_param.min_qp);
    }

    printf("\tPhoto Config\n");
    printf("\t\tsensor[%d] stPhotoCfg image_width: %d\n", i,
           pstCfg->stMediaCfg[i].stPhotoCfg.image_width);
    printf("\t\tsensor[%d] stPhotoCfg image_height: %d\n", i,
           pstCfg->stMediaCfg[i].stPhotoCfg.image_height);
    printf("\t\tsensor[%d] stPhotoCfg snap_num: %d\n", i,
           pstCfg->stMediaCfg[i].stPhotoCfg.snap_num);
    printf("\t\tsensor[%d] stPhotoCfg venc_chn: %d\n", i,
           pstCfg->stMediaCfg[i].stPhotoCfg.venc_chn);
    printf("\t\tsensor[%d] stPhotoCfg vpss_grp: %d\n", i,
           pstCfg->stMediaCfg[i].stPhotoCfg.vpss_grp);
    printf("\t\tsensor[%d] stPhotoCfg vpss_chn: %d\n", i,
           pstCfg->stMediaCfg[i].stPhotoCfg.vpss_chn);
    printf("\t\tsensor[%d] stPhotoCfg enable_combo: %d\n", i,
           pstCfg->stMediaCfg[i].stPhotoCfg.enable_combo);
    printf("\t\tsensor[%d] stPhotoCfg combo_venc_chn: %d\n", i,
           pstCfg->stMediaCfg[i].stPhotoCfg.combo_venc_chn);
    printf("\t\tsensor[%d] stPhotoCfg qfactor: %d\n", i,
           pstCfg->stMediaCfg[i].stPhotoCfg.qfactor);

    printf("\tPreview Config\n");
    printf("\t\tsensor[%d] stStreamCfg width: %d\n", i,
           pstCfg->stMediaCfg[i].stStreamCfg.attribute.width);
    printf("\t\tsensor[%d] stStreamCfg height: %d\n", i,
           pstCfg->stMediaCfg[i].stStreamCfg.attribute.height);
    printf("\t\tsensor[%d] stStreamCfg bufsize: %d\n", i,
           pstCfg->stMediaCfg[i].stStreamCfg.attribute.bufsize);
    printf("\t\tsensor[%d] stStreamCfg bitrate: %d\n", i,
           pstCfg->stMediaCfg[i].stStreamCfg.attribute.bitrate);
    printf("\t\tsensor[%d] stStreamCfg gop: %d\n", i,
           pstCfg->stMediaCfg[i].stStreamCfg.attribute.gop);
    printf("\t\tsensor[%d] stStreamCfg profile: %d\n", i,
           pstCfg->stMediaCfg[i].stStreamCfg.attribute.profile);
    printf("\t\tsensor[%d] stStreamCfg venc_chn: %d\n", i,
           pstCfg->stMediaCfg[i].stStreamCfg.attribute.venc_chn);
    printf("\t\tsensor[%d] stStreamCfg vpss_grp: %d\n", i,
           pstCfg->stMediaCfg[i].stStreamCfg.attribute.vpss_grp);
    printf("\t\tsensor[%d] stStreamCfg vpss_chn: %d\n", i,
           pstCfg->stMediaCfg[i].stStreamCfg.attribute.vpss_chn);
    printf("\t\tsensor[%d] stStreamCfg codec_type: %d\n", i,
           pstCfg->stMediaCfg[i].stStreamCfg.attribute.codec_type);
    printf("\t\tsensor[%d] stStreamCfg rc_mode: %s\n", i,
           pstCfg->stMediaCfg[i].stStreamCfg.attribute.rc_mode);
    printf(
        "\t\tsensor[%d] stStreamCfg first_frame_qp: %d\n", i,
        pstCfg->stMediaCfg[i].stStreamCfg.attribute.venc_param.first_frame_qp);
    printf("\t\tsensor[%d] stStreamCfg qp_step: %d\n", i,
           pstCfg->stMediaCfg[i].stStreamCfg.attribute.venc_param.qp_step);
    printf("\t\tsensor[%d] stStreamCfg max_qp: %d\n", i,
           pstCfg->stMediaCfg[i].stStreamCfg.attribute.venc_param.max_qp);
    printf("\t\tsensor[%d] stStreamCfg min_qp: %d\n", i,
           pstCfg->stMediaCfg[i].stStreamCfg.attribute.venc_param.min_qp);

    printf("\tLive Config\n");
    printf("\t\tsensor[%d] stLiveCfg width: %d\n", i,
           pstCfg->stMediaCfg[i].stLiveCfg.attribute.width);
    printf("\t\tsensor[%d] stLiveCfg height: %d\n", i,
           pstCfg->stMediaCfg[i].stLiveCfg.attribute.height);
    printf("\t\tsensor[%d] stLiveCfg bufsize: %d\n", i,
           pstCfg->stMediaCfg[i].stLiveCfg.attribute.bufsize);
    printf("\t\tsensor[%d] stLiveCfg bitrate: %d\n", i,
           pstCfg->stMediaCfg[i].stLiveCfg.attribute.bitrate);
    printf("\t\tsensor[%d] stLiveCfg gop: %d\n", i,
           pstCfg->stMediaCfg[i].stLiveCfg.attribute.gop);
    printf("\t\tsensor[%d] stLiveCfg profile: %d\n", i,
           pstCfg->stMediaCfg[i].stLiveCfg.attribute.profile);
    printf("\t\tsensor[%d] stLiveCfg venc_chn: %d\n", i,
           pstCfg->stMediaCfg[i].stLiveCfg.attribute.venc_chn);
    printf("\t\tsensor[%d] stLiveCfg vpss_grp: %d\n", i,
           pstCfg->stMediaCfg[i].stLiveCfg.attribute.vpss_grp);
    printf("\t\tsensor[%d] stLiveCfg vpss_chn: %d\n", i,
           pstCfg->stMediaCfg[i].stLiveCfg.attribute.vpss_chn);
    printf("\t\tsensor[%d] stLiveCfg codec_type: %d\n", i,
           pstCfg->stMediaCfg[i].stLiveCfg.attribute.codec_type);
    printf("\t\tsensor[%d] stLiveCfg rc_mode: %s\n", i,
           pstCfg->stMediaCfg[i].stLiveCfg.attribute.rc_mode);
    printf("\t\tsensor[%d] stLiveCfg first_frame_qp: %d\n", i,
           pstCfg->stMediaCfg[i].stLiveCfg.attribute.venc_param.first_frame_qp);
    printf("\t\tsensor[%d] stLiveCfg qp_step: %d\n", i,
           pstCfg->stMediaCfg[i].stLiveCfg.attribute.venc_param.qp_step);
    printf("\t\tsensor[%d] stLiveCfg max_qp: %d\n", i,
           pstCfg->stMediaCfg[i].stLiveCfg.attribute.venc_param.max_qp);
    printf("\t\tsensor[%d] stLiveCfg min_qp: %d\n", i,
           pstCfg->stMediaCfg[i].stLiveCfg.attribute.venc_param.min_qp);

    printf("\tDisplay Config\n");
    printf("\t\tsensor[%d] stDispCfg width: %d\n", i,
           pstCfg->stMediaCfg[i].stDispCfg.width);
    printf("\t\tsensor[%d] stDispCfg height: %d\n", i,
           pstCfg->stMediaCfg[i].stDispCfg.height);
    printf("\t\tsensor[%d] stDispCfg enable_buf_pool: %d\n", i,
           pstCfg->stMediaCfg[i].stDispCfg.enable_buf_pool);
    printf("\t\tsensor[%d] stDispCfg buf_pool_cnt: %d\n", i,
           pstCfg->stMediaCfg[i].stDispCfg.buf_pool_cnt);
    printf("\t\tsensor[%d] stDispCfg rotaion: %d\n", i,
           pstCfg->stMediaCfg[i].stDispCfg.rotaion);
    printf("\t\tsensor[%d] stDispCfg vpss_grp: %d\n", i,
           pstCfg->stMediaCfg[i].stDispCfg.vpss_grp);
    printf("\t\tsensor[%d] stDispCfg vpss_chn: %d\n", i,
           pstCfg->stMediaCfg[i].stDispCfg.vpss_chn);
    printf("\t\tsensor[%d] stDispCfg device_node: %s\n", i,
           pstCfg->stMediaCfg[i].stDispCfg.device_node);
    printf("\t\tsensor[%d] stDispCfg img_type: %s\n", i,
           pstCfg->stMediaCfg[i].stDispCfg.img_type);
#ifdef RKADK_ENABLE_DISP
    printf("\t\tsensor[%d] stDispCfg plane_type: %d\n", i,
           pstCfg->stMediaCfg[i].stDispCfg.plane_type);
#endif
    printf("\t\tsensor[%d] stDispCfg z_pos: %d\n", i,
           pstCfg->stMediaCfg[i].stDispCfg.z_pos);
    printf("\t\tsensor[%d] stDispCfg vo_chn: %d\n", i,
           pstCfg->stMediaCfg[i].stDispCfg.vo_chn);

    printf("\tThumb Config\n");
    printf("\t\tsensor[%d] stThumbCfg thumb_width: %d\n", i,
           pstCfg->stMediaCfg[i].stThumbCfg.thumb_width);
    printf("\t\tsensor[%d] stThumbCfg thumb_height: %d\n", i,
           pstCfg->stMediaCfg[i].stThumbCfg.thumb_height);
    printf("\t\tsensor[%d] stThumbCfg photo_venc_chn: %d\n", i,
           pstCfg->stMediaCfg[i].stThumbCfg.photo_venc_chn);
    printf("\t\tsensor[%d] stThumbCfg record_main_venc_chn: %d\n", i,
           pstCfg->stMediaCfg[i].stThumbCfg.record_main_venc_chn);
    printf("\t\tsensor[%d] stThumbCfg record_sub_venc_chn: %d\n", i,
           pstCfg->stMediaCfg[i].stThumbCfg.record_sub_venc_chn);
    printf("\t\tsensor[%d] stThumbCfg vpss_grp: %d\n", i,
           pstCfg->stMediaCfg[i].stThumbCfg.vpss_grp);
    printf("\t\tsensor[%d] stThumbCfg vpss_chn: %d\n", i,
           pstCfg->stMediaCfg[i].stThumbCfg.vpss_chn);
  }
}

static void RKADK_PARAM_DumpViAttr() {
  int i, j;
  RKADK_PARAM_CFG_S *pstCfg = &g_stPARAMCtx.stCfg;

  for (i = 0; i < (int)pstCfg->stCommCfg.sensor_count; i++) {
    for (j = 0; j < (int)pstCfg->stMediaCfg[i].stRecCfg.file_num; j++) {
      printf("\tRec VI Attribute\n");
      printf("\t\tsensor[%d] stRecCfg[%d] stIspOpt.aEntityName: %s\n", i, j,
             pstCfg->stMediaCfg[i]
                 .stRecCfg.vi_attr[j]
                 .stChnAttr.stIspOpt.aEntityName);
      printf("\t\tsensor[%d] stRecCfg[%d] u32ViChn: %d\n", i, j,
             pstCfg->stMediaCfg[i].stRecCfg.vi_attr[j].u32ViChn);
      printf(
          "\t\tsensor[%d] stRecCfg[%d] u32Width: %d\n", i, j,
          pstCfg->stMediaCfg[i].stRecCfg.vi_attr[j].stChnAttr.stSize.u32Width);
      printf(
          "\t\tsensor[%d] stRecCfg[%d] u32Height: %d\n", i, j,
          pstCfg->stMediaCfg[i].stRecCfg.vi_attr[j].stChnAttr.stSize.u32Height);
      printf("\t\tsensor[%d] stRecCfg[%d] stIspOpt.u32BufCount: %d\n", i, j,
             pstCfg->stMediaCfg[i]
                 .stRecCfg.vi_attr[j]
                 .stChnAttr.stIspOpt.u32BufCount);
      printf("\t\tsensor[%d] stRecCfg[%d] enPixelFormat: %d\n", i, j,
             pstCfg->stMediaCfg[i].stRecCfg.vi_attr[j].stChnAttr.enPixelFormat);
      printf("\t\tsensor[%d] stRecCfg[%d] stIspOpt.enMemoryType: %d\n", i, j,
             pstCfg->stMediaCfg[i]
                 .stRecCfg.vi_attr[j]
                 .stChnAttr.stIspOpt.enMemoryType);
    }

    printf("\tPhoto VI Attribute\n");
    printf("\t\tsensor[%d] stPhotoCfg stIspOpt.aEntityName: %s\n", i,
           pstCfg->stMediaCfg[i]
               .stPhotoCfg.vi_attr.stChnAttr.stIspOpt.aEntityName);
    printf("\t\tsensor[%d] stPhotoCfg u32ViChn: %d\n", i,
           pstCfg->stMediaCfg[i].stPhotoCfg.vi_attr.u32ViChn);
    printf("\t\tsensor[%d] stPhotoCfg u32Width: %d\n", i,
           pstCfg->stMediaCfg[i].stPhotoCfg.vi_attr.stChnAttr.stSize.u32Width);
    printf("\t\tsensor[%d] stPhotoCfg u32Height: %d\n", i,
           pstCfg->stMediaCfg[i].stPhotoCfg.vi_attr.stChnAttr.stSize.u32Height);
    printf("\t\tsensor[%d] stPhotoCfg stIspOpt.u32BufCount: %d\n", i,
           pstCfg->stMediaCfg[i]
               .stPhotoCfg.vi_attr.stChnAttr.stIspOpt.u32BufCount);
    printf("\t\tsensor[%d] stPhotoCfg enPixelFormat: %d\n", i,
           pstCfg->stMediaCfg[i].stPhotoCfg.vi_attr.stChnAttr.enPixelFormat);
    printf("\t\tsensor[%d] stPhotoCfg stIspOpt.enMemoryType: %d\n", i,
           pstCfg->stMediaCfg[i]
               .stPhotoCfg.vi_attr.stChnAttr.stIspOpt.enMemoryType);

    printf("\tStream VI Attribute\n");
    printf("\t\tsensor[%d] stStreamCfg stIspOpt.aEntityName: %s\n", i,
           pstCfg->stMediaCfg[i]
               .stStreamCfg.vi_attr.stChnAttr.stIspOpt.aEntityName);
    printf("\t\tsensor[%d] stStreamCfg u32ViChn: %d\n", i,
           pstCfg->stMediaCfg[i].stStreamCfg.vi_attr.u32ViChn);
    printf("\t\tsensor[%d] stStreamCfg u32Width: %d\n", i,
           pstCfg->stMediaCfg[i].stStreamCfg.vi_attr.stChnAttr.stSize.u32Width);
    printf(
        "\t\tsensor[%d] stStreamCfg u32Height: %d\n", i,
        pstCfg->stMediaCfg[i].stStreamCfg.vi_attr.stChnAttr.stSize.u32Height);
    printf("\t\tsensor[%d] stStreamCfg stIspOpt.u32BufCount: %d\n", i,
           pstCfg->stMediaCfg[i]
               .stStreamCfg.vi_attr.stChnAttr.stIspOpt.u32BufCount);
    printf("\t\tsensor[%d] stStreamCfg enPixelFormat: %d\n", i,
           pstCfg->stMediaCfg[i].stStreamCfg.vi_attr.stChnAttr.enPixelFormat);
    printf("\t\tsensor[%d] stStreamCfg stIspOpt.enMemoryType: %d\n", i,
           pstCfg->stMediaCfg[i]
               .stStreamCfg.vi_attr.stChnAttr.stIspOpt.enMemoryType);

    printf("\tLive VI Attribute\n");
    printf(
        "\t\tsensor[%d] stLiveCfg stIspOpt.aEntityName: %s\n", i,
        pstCfg->stMediaCfg[i].stLiveCfg.vi_attr.stChnAttr.stIspOpt.aEntityName);
    printf("\t\tsensor[%d] stLiveCfg u32ViChn: %d\n", i,
           pstCfg->stMediaCfg[i].stLiveCfg.vi_attr.u32ViChn);
    printf("\t\tsensor[%d] stLiveCfg u32Width: %d\n", i,
           pstCfg->stMediaCfg[i].stLiveCfg.vi_attr.stChnAttr.stSize.u32Width);
    printf("\t\tsensor[%d] stLiveCfg u32Height: %d\n", i,
           pstCfg->stMediaCfg[i].stLiveCfg.vi_attr.stChnAttr.stSize.u32Height);
    printf(
        "\t\tsensor[%d] stLiveCfg stIspOpt.u32BufCount: %d\n", i,
        pstCfg->stMediaCfg[i].stLiveCfg.vi_attr.stChnAttr.stIspOpt.u32BufCount);
    printf("\t\tsensor[%d] stLiveCfg enPixelFormat: %d\n", i,
           pstCfg->stMediaCfg[i].stLiveCfg.vi_attr.stChnAttr.enPixelFormat);
    printf("\t\tsensor[%d] stLiveCfg stIspOpt.enMemoryType: %d\n", i,
           pstCfg->stMediaCfg[i]
               .stLiveCfg.vi_attr.stChnAttr.stIspOpt.enMemoryType);

    printf("\tDisplay VI Attribute\n");
    printf(
        "\t\tsensor[%d] stDispCfg stIspOpt.aEntityName: %s\n", i,
        pstCfg->stMediaCfg[i].stDispCfg.vi_attr.stChnAttr.stIspOpt.aEntityName);
    printf("\t\tsensor[%d] stDispCfg u32ViChn: %d\n", i,
           pstCfg->stMediaCfg[i].stDispCfg.vi_attr.u32ViChn);
    printf("\t\tsensor[%d] stDispCfg u32Width: %d\n", i,
           pstCfg->stMediaCfg[i].stDispCfg.vi_attr.stChnAttr.stSize.u32Width);
    printf("\t\tsensor[%d] stDispCfg u32Height: %d\n", i,
           pstCfg->stMediaCfg[i].stDispCfg.vi_attr.stChnAttr.stSize.u32Height);
    printf(
        "\t\tsensor[%d] stDispCfg stIspOpt.u32BufCount: %d\n", i,
        pstCfg->stMediaCfg[i].stDispCfg.vi_attr.stChnAttr.stIspOpt.u32BufCount);
    printf("\t\tsensor[%d] stDispCfg enPixelFormat: %d\n", i,
           pstCfg->stMediaCfg[i].stDispCfg.vi_attr.stChnAttr.enPixelFormat);
    printf("\t\tsensor[%d] stDispCfg stIspOpt.enMemoryType: %d\n", i,
           pstCfg->stMediaCfg[i]
               .stDispCfg.vi_attr.stChnAttr.stIspOpt.enMemoryType);

    printf("\tThumb VI Attribute\n");
    printf("\t\tsensor[%d] stThumbCfg stIspOpt.aEntityName: %s\n", i,
           pstCfg->stMediaCfg[i]
               .stThumbCfg.vi_attr.stChnAttr.stIspOpt.aEntityName);
    printf("\t\tsensor[%d] stThumbCfg u32ViChn: %d\n", i,
           pstCfg->stMediaCfg[i].stThumbCfg.vi_attr.u32ViChn);
    printf("\t\tsensor[%d] stThumbCfg u32Width: %d\n", i,
           pstCfg->stMediaCfg[i].stThumbCfg.vi_attr.stChnAttr.stSize.u32Width);
    printf("\t\tsensor[%d] stThumbCfg u32Height: %d\n", i,
           pstCfg->stMediaCfg[i].stThumbCfg.vi_attr.stChnAttr.stSize.u32Height);
    printf("\t\tsensor[%d] stThumbCfg stIspOpt.u32BufCount: %d\n", i,
           pstCfg->stMediaCfg[i]
               .stThumbCfg.vi_attr.stChnAttr.stIspOpt.u32BufCount);
    printf("\t\tsensor[%d] stThumbCfg enPixelFormat: %d\n", i,
           pstCfg->stMediaCfg[i].stThumbCfg.vi_attr.stChnAttr.enPixelFormat);
    printf("\t\tsensor[%d] stThumbCfg stIspOpt.enMemoryType: %d\n", i,
           pstCfg->stMediaCfg[i]
               .stThumbCfg.vi_attr.stChnAttr.stIspOpt.enMemoryType);
  }
}

static RKADK_S32 RKADK_PARAM_LoadParam(char *path,
                                       char (*sensorPath)[RKADK_PATH_LEN]) {
  int i, j, ret = 0;
  RKADK_MAP_TABLE_CFG_S *pstMapTableCfg = NULL;
  RKADK_PARAM_MAP_TYPE_E enMapType = RKADK_PARAM_MAP_BUTT;
  RKADK_PARAM_CFG_S *pstCfg = &g_stPARAMCtx.stCfg;

  if (!RKADK_PARAM_CheckVersion(path))
    return -1;

  // load common config
  memset(&pstCfg->stCommCfg, 0, sizeof(RKADK_PARAM_COMM_CFG_S));
  ret = RKADK_Ini2Struct(path, &pstCfg->stCommCfg, g_stCommCfgMapTable,
                         sizeof(g_stCommCfgMapTable) /
                             sizeof(RKADK_SI_CONFIG_MAP_S));
  if (ret) {
    RKADK_LOGW("common config param not exist, use default");
    return ret;
  }
  RKADK_PARAM_CheckCommCfg(path);

  // load audio config
  memset(&pstCfg->stAudioCfg, 0, sizeof(RKADK_PARAM_AUDIO_CFG_S));
  ret = RKADK_Ini2Struct(path, &pstCfg->stAudioCfg, g_stAudioCfgMapTable,
                         sizeof(g_stAudioCfgMapTable) /
                             sizeof(RKADK_SI_CONFIG_MAP_S));
  if (ret) {
    RKADK_LOGW("audio config param not exist, use default");
    return ret;
  }
  RKADK_PARAM_CheckAudioCfg(path);

  // load sensor config
  for (i = 0; i < (int)pstCfg->stCommCfg.sensor_count; i++) {
    pstMapTableCfg = RKADK_PARAM_GetMapTable(i, RKADK_PARAM_SENSOR_MAP);
    RKADK_CHECK_POINTER(pstMapTableCfg, RKADK_FAILURE);

    memset(&pstCfg->stSensorCfg[i], 0, sizeof(RKADK_PARAM_SENSOR_CFG_S));
    ret = RKADK_Ini2Struct(sensorPath[i], &pstCfg->stSensorCfg[i],
                           pstMapTableCfg->pstMapTable,
                           pstMapTableCfg->u32TableLen);
    if (ret) {
      RKADK_LOGW("sensor[%d] config param not exist, use default", i);
      return ret;
    }
    RKADK_PARAM_CheckSensorCfg(sensorPath[i], i);

    // load vi config
    for (j = 0; j < RKADK_ISPP_VI_NODE_CNT; j++) {
      if (j == 0) {
        enMapType = RKADK_PARAM_VI0_MAP;
      } else if (j == 1) {
        enMapType = RKADK_PARAM_VI1_MAP;
      } else if (j == 2) {
        enMapType = RKADK_PARAM_VI2_MAP;
      } else {
        enMapType = RKADK_PARAM_VI3_MAP;
      }

      pstMapTableCfg = RKADK_PARAM_GetMapTable(i, enMapType);
      RKADK_CHECK_POINTER(pstMapTableCfg, RKADK_FAILURE);

      memset(&pstCfg->stMediaCfg[i].stViCfg[j], 0,
             sizeof(RKADK_PARAM_VI_CFG_S));
      ret = RKADK_Ini2Struct(sensorPath[i], &pstCfg->stMediaCfg[i].stViCfg[j],
                             pstMapTableCfg->pstMapTable,
                             pstMapTableCfg->u32TableLen);

      if (ret) {
        RKADK_LOGW("sensor[%d] vi[%d] config param not exist, use default", i,
                   j);
        return ret;
      }
      RKADK_PARAM_CheckViCfg(sensorPath[i], i, j);
    }

    // load record config
    memset(&pstCfg->stMediaCfg[i].stRecCfg, 0, sizeof(RKADK_PARAM_REC_CFG_S));
    pstMapTableCfg = RKADK_PARAM_GetMapTable(i, RKADK_PARAM_REC_MAP);
    RKADK_CHECK_POINTER(pstMapTableCfg, RKADK_FAILURE);

    ret = RKADK_Ini2Struct(sensorPath[i], &pstCfg->stMediaCfg[i].stRecCfg,
                           pstMapTableCfg->pstMapTable,
                           pstMapTableCfg->u32TableLen);
    if (ret) {
      RKADK_LOGW("sensor[%d] record param not exist, use default", i);
      return ret;
    }

    for (j = 0; j < (int)pstCfg->stMediaCfg[i].stRecCfg.file_num; j++) {
      RKADK_MAP_TABLE_CFG_S *pstTimeMapTable = NULL;
      RKADK_MAP_TABLE_CFG_S *pstParamMapTable = NULL;

      memset(&pstCfg->stMediaCfg[i].stRecCfg.attribute[j], 0,
             sizeof(RKADK_PARAM_VENC_ATTR_S));
      if (j == 0) {
        pstTimeMapTable =
            RKADK_PARAM_GetMapTable(i, RKADK_PARAM_REC_MAIN_TIME_MAP);
        pstMapTableCfg = RKADK_PARAM_GetMapTable(i, RKADK_PARAM_REC_MAIN_MAP);
        pstParamMapTable =
            RKADK_PARAM_GetMapTable(i, RKADK_PARAM_REC_MAIN_PARAM_MAP);
      } else {
        pstTimeMapTable =
            RKADK_PARAM_GetMapTable(i, RKADK_PARAM_REC_SUB_TIME_MAP);
        pstMapTableCfg = RKADK_PARAM_GetMapTable(i, RKADK_PARAM_REC_SUB_MAP);
        pstParamMapTable =
            RKADK_PARAM_GetMapTable(i, RKADK_PARAM_REC_SUB_PARAM_MAP);
      }

      if (!pstMapTableCfg || !pstParamMapTable || !pstTimeMapTable)
        return -1;

      ret = RKADK_Ini2Struct(
          sensorPath[i], &pstCfg->stMediaCfg[i].stRecCfg.record_time_cfg[j],
          pstTimeMapTable->pstMapTable, pstTimeMapTable->u32TableLen);
      if (ret) {
        RKADK_LOGW("sensor[%d] rec time[%d] not exist, use default", i, j);
        return ret;
      }

      ret = RKADK_Ini2Struct(
          sensorPath[i], &pstCfg->stMediaCfg[i].stRecCfg.attribute[j],
          pstMapTableCfg->pstMapTable, pstMapTableCfg->u32TableLen);
      if (ret) {
        RKADK_LOGW("sensor[%d] rec attribute[%d] param not exist, use default",
                   i, j);
        return ret;
      }

      ret = RKADK_Ini2Struct(
          sensorPath[i],
          &pstCfg->stMediaCfg[i].stRecCfg.attribute[j].venc_param,
          pstParamMapTable->pstMapTable, pstParamMapTable->u32TableLen);
      if (ret)
        RKADK_LOGW(
            "sensor[%d] rec attribute[%d] venc param not exist, use default", i, j);
    }
    RKADK_PARAM_CheckRecCfg(sensorPath[i], i);

    // load preview config
    memset(&pstCfg->stMediaCfg[i].stStreamCfg.attribute, 0,
           sizeof(RKADK_PARAM_VENC_ATTR_S));
    pstMapTableCfg = RKADK_PARAM_GetMapTable(i, RKADK_PARAM_PREVIEW_MAP);
    RKADK_CHECK_POINTER(pstMapTableCfg, RKADK_FAILURE);

    ret = RKADK_Ini2Struct(
        sensorPath[i], &pstCfg->stMediaCfg[i].stStreamCfg.attribute,
        pstMapTableCfg->pstMapTable, pstMapTableCfg->u32TableLen);
    if (ret) {
      RKADK_LOGW("sensor[%d] stream config param not exist, use default", i);
      return ret;
    }

    // load preview venc param
    pstMapTableCfg = RKADK_PARAM_GetMapTable(i, RKADK_PARAM_PREVIEW_PARAM_MAP);
    RKADK_CHECK_POINTER(pstMapTableCfg, RKADK_FAILURE);

    ret = RKADK_Ini2Struct(
        sensorPath[i], &pstCfg->stMediaCfg[i].stStreamCfg.attribute.venc_param,
        pstMapTableCfg->pstMapTable, pstMapTableCfg->u32TableLen);
    if (ret)
      RKADK_LOGW("sensor[%d] stream venc param not exist, use default", i);

    RKADK_PARAM_CheckStreamCfg(sensorPath[i], i, RKADK_STREAM_TYPE_PREVIEW);

    // load live config
    memset(&pstCfg->stMediaCfg[i].stLiveCfg.attribute, 0,
           sizeof(RKADK_PARAM_VENC_ATTR_S));
    pstMapTableCfg = RKADK_PARAM_GetMapTable(i, RKADK_PARAM_LIVE_MAP);
    RKADK_CHECK_POINTER(pstMapTableCfg, RKADK_FAILURE);

    ret = RKADK_Ini2Struct(
        sensorPath[i], &pstCfg->stMediaCfg[i].stLiveCfg.attribute,
        pstMapTableCfg->pstMapTable, pstMapTableCfg->u32TableLen);
    if (ret) {
      RKADK_LOGW("sensor[%d] live config param not exist, use default", i);
      return ret;
    }

    // load live venc param
    pstMapTableCfg = RKADK_PARAM_GetMapTable(i, RKADK_PARAM_LIVE_PARAM_MAP);
    RKADK_CHECK_POINTER(pstMapTableCfg, RKADK_FAILURE);

    ret = RKADK_Ini2Struct(
        sensorPath[i], &pstCfg->stMediaCfg[i].stLiveCfg.attribute.venc_param,
        pstMapTableCfg->pstMapTable, pstMapTableCfg->u32TableLen);
    if (ret)
      RKADK_LOGW("sensor[%d] live venc param not exist, use default", i);

    RKADK_PARAM_CheckStreamCfg(sensorPath[i], i, RKADK_STREAM_TYPE_LIVE);

    // load photo config
    pstMapTableCfg = RKADK_PARAM_GetMapTable(i, RKADK_PARAM_PHOTO_MAP);
    RKADK_CHECK_POINTER(pstMapTableCfg, RKADK_FAILURE);

    memset(&pstCfg->stMediaCfg[i].stPhotoCfg, 0,
           sizeof(RKADK_PARAM_PHOTO_CFG_S));
    ret = RKADK_Ini2Struct(sensorPath[i], &pstCfg->stMediaCfg[i].stPhotoCfg,
                           pstMapTableCfg->pstMapTable,
                           pstMapTableCfg->u32TableLen);
    if (ret) {
      RKADK_LOGW("sensor[%d] photo config param not exist, use default", i);
      return ret;
    }
    RKADK_PARAM_CheckPhotoCfg(sensorPath[i], i);

    // load display config
    pstMapTableCfg = RKADK_PARAM_GetMapTable(i, RKADK_PARAM_DISP_MAP);
    RKADK_CHECK_POINTER(pstMapTableCfg, RKADK_FAILURE);

    memset(&pstCfg->stMediaCfg[i].stDispCfg, 0, sizeof(RKADK_PARAM_DISP_CFG_S));
    ret = RKADK_Ini2Struct(sensorPath[i], &pstCfg->stMediaCfg[i].stDispCfg,
                           pstMapTableCfg->pstMapTable,
                           pstMapTableCfg->u32TableLen);
    if (ret) {
      RKADK_LOGW("sensor[%d] display config param not exist, use default", i);
      return ret;
    }
    RKADK_PARAM_CheckDispCfg(sensorPath[i], i);

    // load thumb config
    pstMapTableCfg = RKADK_PARAM_GetMapTable(i, RKADK_PARAM_THUMB_MAP);
    RKADK_CHECK_POINTER(pstMapTableCfg, RKADK_FAILURE);

    memset(&pstCfg->stMediaCfg[i].stThumbCfg, 0, sizeof(RKADK_PARAM_THUMB_CFG_S));
    ret = RKADK_Ini2Struct(sensorPath[i], &pstCfg->stMediaCfg[i].stThumbCfg,
                           pstMapTableCfg->pstMapTable,
                           pstMapTableCfg->u32TableLen);
    if (ret) {
      RKADK_LOGW("thumb config param not exist, use default");
      return ret;
    }
    RKADK_PARAM_CheckThumbCfg(sensorPath[i], i);
  }

  return 0;
}

static void RKADK_PARAM_UseDefault() {
  int i;
  RKADK_PARAM_REC_CFG_S *pstRecCfg;
  RKADK_STREAM_TYPE_E enStrmType = RKADK_STREAM_TYPE_BUTT;

  // default version
  RKADK_PARAM_VERSION_S *pstVersion = &g_stPARAMCtx.stCfg.stVersion;
  memcpy(pstVersion->version, RKADK_PARAM_VERSION, strlen(RKADK_PARAM_VERSION));
  RKADK_PARAM_SaveVersion(g_stPARAMCtx.path);

  // default common config
  RKADK_PARAM_DefCommCfg(g_stPARAMCtx.path);

  // default audio config
  RKADK_PARAM_DefAudioCfg(g_stPARAMCtx.path);

  // default sensor.0 config
  RKADK_PARAM_DefSensorCfg(0, g_stPARAMCtx.sensorPath[0]);

  // default vi config
  for (i = 0; i < RKADK_ISPP_VI_NODE_CNT; i++)
    RKADK_PARAM_DefViCfg(0, i, g_stPARAMCtx.sensorPath[0]);

  // default sensor.0.rec config
  RKADK_PARAM_DefRecCfg(0, g_stPARAMCtx.sensorPath[0]);

  pstRecCfg = &g_stPARAMCtx.stCfg.stMediaCfg[0].stRecCfg;
  for (i = 0; i < (int)pstRecCfg->file_num; i++) {
    if (i == 0)
      enStrmType = RKADK_STREAM_TYPE_VIDEO_MAIN;
    else
      enStrmType = RKADK_STREAM_TYPE_VIDEO_SUB;

    RKADK_PARAM_DefRecAttr(0, enStrmType, g_stPARAMCtx.sensorPath[0]);
    RKADK_PARAM_DefRecTime(0, enStrmType, g_stPARAMCtx.sensorPath[0]);
  }

  // default sensor.0.photo config
  RKADK_PARAM_DefPhotoCfg(0, g_stPARAMCtx.sensorPath[0]);

  // default sensor.0.stream config
  RKADK_PARAM_DefStreamCfg(0, g_stPARAMCtx.sensorPath[0],
                           RKADK_STREAM_TYPE_PREVIEW);

  // default sensor.0.live config
  RKADK_PARAM_DefStreamCfg(0, g_stPARAMCtx.sensorPath[0],
                           RKADK_STREAM_TYPE_LIVE);

  // default sensor.0.disp config
  RKADK_PARAM_DefDispCfg(0, g_stPARAMCtx.sensorPath[0]);

  // default sensor.0.thumb config
  RKADK_PARAM_DefThumbCfg(0, g_stPARAMCtx.sensorPath[0]);
}

static void RKADK_PARAM_Mkdirs(char *dir) {
  char *p = NULL;

  RKADK_CHECK_POINTER_N(dir);
  if (strlen(dir) == 0) {
    RKADK_LOGE("strlen(dir) is 0");
    return;
  }

  if (dir[0] == '/') {
    p = strchr(dir + 1, '/');
  } else {
    p = strchr(dir, '/');
  }

  if (p) {
    *p = '\0';

    if (access(dir, F_OK)) {
      if (mkdir(dir, 0777)) {
        RKADK_LOGE("mkdir %s failed", dir);
        return;
      }
    }

    if (chdir(dir)) {
      RKADK_LOGE("chdir %s failed", dir);
      return;
    }
  } else {
    if (access(dir, F_OK))
      if (mkdir(dir, 0777))
        RKADK_LOGE("mkdir %s failed", dir);

    return;
  }

  RKADK_PARAM_Mkdirs(p + 1);
}

static RKADK_S32 RKADK_PARAM_LoadDefault() {
  int ret;
  int bufLen = RKADK_PATH_LEN * 2 + 4;
  char buffer[bufLen];
  char defSensorPath[RKADK_MAX_SENSOR_CNT][RKADK_PATH_LEN];
  RKADK_PARAM_COMM_CFG_S *pstCommCfg = &g_stPARAMCtx.stCfg.stCommCfg;

  for (int i = 0; i < RKADK_MAX_SENSOR_CNT; i++) {
    memset(defSensorPath[i], 0, RKADK_PATH_LEN);
    sprintf(defSensorPath[i], "%s_%d.ini", RKADK_DEFPARAM_PATH_SENSOR_PREFIX,
            i);
    RKADK_LOGD("defSensorPath[%d]: %s", i, defSensorPath[i]);
  }

  ret = RKADK_PARAM_LoadParam((char *)RKADK_DEFPARAM_PATH, defSensorPath);
  if (ret) {
    RKADK_LOGE("load default ini failed");
    return ret;
  }

  char *ptr = strrchr(g_stPARAMCtx.path, '/');
  if (ptr) {
    memset(buffer, 0, bufLen);
    memcpy(buffer, g_stPARAMCtx.path, strlen(g_stPARAMCtx.path) - strlen(ptr));
    if (access(buffer, F_OK))
      RKADK_PARAM_Mkdirs(buffer);
  }

  memset(buffer, 0, bufLen);
  sprintf(buffer, "cp %s %s", RKADK_DEFPARAM_PATH, g_stPARAMCtx.path);
  RKADK_LOGD("%s", buffer);
  system(buffer);

  for (int i = 0; i < (int)pstCommCfg->sensor_count; i++) {
    memset(buffer, 0, bufLen);
    sprintf(buffer, "cp %s %s", defSensorPath[i], g_stPARAMCtx.sensorPath[i]);
    RKADK_LOGD("%s", buffer);
    system(buffer);
  }

  return 0;
}

static RKADK_S32 RKADK_PARAM_MatchViIndex(RKADK_STREAM_TYPE_E enStrmType,
                                          RKADK_S32 s32CamId, RKADK_U32 width,
                                          RKADK_U32 height) {
  int index;
  int matchViCnt = 0;
  int matchViIndex[RKADK_ISPP_VI_NODE_CNT] = {0};
  char module[RKADK_BUFFER_LEN];
  bool bSaveViCfg = false;
  RKADK_PARAM_VI_CFG_S *pstViCfg = NULL;
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg =
      &g_stPARAMCtx.stCfg.stSensorCfg[s32CamId];

  RKADK_CHECK_CAMERAID(s32CamId, RKADK_FAILURE);
  memset(module, 0, RKADK_BUFFER_LEN);

  switch (enStrmType) {
  case RKADK_STREAM_TYPE_VIDEO_MAIN:
    memcpy(module, "RECORD_MAIN", strlen("RECORD_MAIN"));
    bSaveViCfg = true;
    break;

  case RKADK_STREAM_TYPE_VIDEO_SUB:
    memcpy(module, "RECORD_SUB", strlen("RECORD_SUB"));
    break;

  case RKADK_STREAM_TYPE_SNAP:
    memcpy(module, "PHOTO", strlen("PHOTO"));
    bSaveViCfg = true;
    break;

  case RKADK_STREAM_TYPE_PREVIEW:
    memcpy(module, "PREVIEW", strlen("PREVIEW"));
    break;

  case RKADK_STREAM_TYPE_LIVE:
    memcpy(module, "LIVE", strlen("LIVE"));
    break;

  case RKADK_STREAM_TYPE_DISP:
    memcpy(module, "DISP", strlen("DISP"));
    break;

  case RKADK_STREAM_TYPE_THUMB:
    memcpy(module, "THUMB", strlen("THUMB"));
    break;

  default:
    RKADK_LOGE("Invaild mode = %d", enStrmType);
    return -1;
  }

  for (index = 0; index < RKADK_ISPP_VI_NODE_CNT; index++) {
    pstViCfg = &g_stPARAMCtx.stCfg.stMediaCfg[s32CamId].stViCfg[index];
    if (strstr(pstViCfg->module, module)) {
      if (!pstSensorCfg->used_isp) {
        RKADK_LOGD("Sensor[%d] %s[%d x %d] match VI[%d][%d x %d]", s32CamId,
                   module, width, height, index, pstViCfg->width,
                   pstViCfg->height);
        return index;
      } else {
        if(pstViCfg->width == width && pstViCfg->height == height) {
          RKADK_LOGD("Sensor[%d] %s[%d x %d] match VI[%d][%d x %d]", s32CamId,
                     module, width, height, index, pstViCfg->width,
                     pstViCfg->height);
          return index;
        }
        matchViIndex[matchViCnt] = index;
        matchViCnt++;
      }
    }
  }

  if (!matchViCnt) {
    RKADK_LOGE("Sensor[%d] %s[%d x %d] not find matched VI",
               s32CamId, module, width, height);
    return -1;
  } else if (matchViCnt == 1) {
    index = matchViIndex[0];
  } else {
    if (width == pstSensorCfg->max_width && height == pstSensorCfg->max_height) {
      index = matchViIndex[0];
    } else {
      index = matchViIndex[1];
    }
  }

  if (enStrmType == RKADK_STREAM_TYPE_VIDEO_MAIN) {
    pstSensorCfg->wrap_buf_line = height;
    RKADK_PARAM_SaveSensorCfg(g_stPARAMCtx.sensorPath[s32CamId], s32CamId);
  }

  pstViCfg = &g_stPARAMCtx.stCfg.stMediaCfg[s32CamId].stViCfg[index];
  RKADK_LOGD("Sensor[%d] %s[%d x %d] match vi.%d[%d x %d] bSaveViCfg[%d]\n", s32CamId,
             module, width, height, index, pstViCfg->width, pstViCfg->height, bSaveViCfg);

  if (bSaveViCfg) {
    pstViCfg->width = width;
    pstViCfg->height = height;
    RKADK_PARAM_SaveViCfg(g_stPARAMCtx.sensorPath[s32CamId], index, s32CamId);
  }
  return index;
}

PIXEL_FORMAT_E RKADK_PARAM_GetPixFmt(char *pixFmt,
                                     COMPRESS_MODE_E *enCompressMode) {
  PIXEL_FORMAT_E enPixelFormat = RK_FMT_YUV420SP;

  RKADK_CHECK_POINTER(pixFmt, RK_FMT_BUTT);
  RKADK_CHECK_POINTER(enCompressMode, RK_FMT_BUTT);

  *enCompressMode = COMPRESS_MODE_NONE;

  if (!strcmp(pixFmt, "FBC0") || !strcmp(pixFmt, "FBC2")) {
    enPixelFormat = RK_FMT_YUV420SP;
    *enCompressMode = COMPRESS_AFBC_16x16;
  } else if (!strcmp(pixFmt, "NV12"))
    enPixelFormat = RK_FMT_YUV420SP;
  else if (!strcmp(pixFmt, "YUV420SP_10BIT"))
    enPixelFormat = RK_FMT_YUV420SP_10BIT;
  else if (!strcmp(pixFmt, "NV16"))
    enPixelFormat = RK_FMT_YUV422SP;
  else if (!strcmp(pixFmt, "YUV422SP_10BIT"))
    enPixelFormat = RK_FMT_YUV422SP_10BIT;
  else if (!strcmp(pixFmt, "YUV420P"))
    enPixelFormat = RK_FMT_YUV420P;
  else if (!strcmp(pixFmt, "YUV420P_VU"))
    enPixelFormat = RK_FMT_YUV420P_VU;
  else if (!strcmp(pixFmt, "YUV420SP_VU"))
    enPixelFormat = RK_FMT_YUV420SP_VU;
  else if (!strcmp(pixFmt, "YUV422P"))
    enPixelFormat = RK_FMT_YUV422P;
  else if (!strcmp(pixFmt, "YUV422SP_VU"))
    enPixelFormat = RK_FMT_YUV422SP_VU;
  else if (!strcmp(pixFmt, "YUV422_YUYV"))
    enPixelFormat = RK_FMT_YUV422_YUYV;
  else if (!strcmp(pixFmt, "YUV422_UYVY"))
    enPixelFormat = RK_FMT_YUV422_UYVY;
  else if (!strcmp(pixFmt, "YUV400SP"))
    enPixelFormat = RK_FMT_YUV400SP;
  else if (!strcmp(pixFmt, "YUV440SP"))
    enPixelFormat = RK_FMT_YUV440SP;
  else if (!strcmp(pixFmt, "YUV411SP"))
    enPixelFormat = RK_FMT_YUV411SP;
  else if (!strcmp(pixFmt, "YUV444"))
    enPixelFormat = RK_FMT_YUV444;
  else if (!strcmp(pixFmt, "YUV444SP"))
    enPixelFormat = RK_FMT_YUV444SP;
  else if (!strcmp(pixFmt, "YUV422_YVYU"))
    enPixelFormat = RK_FMT_YUV422_YVYU;
  else if (!strcmp(pixFmt, "YUV422_VYUY"))
    enPixelFormat = RK_FMT_YUV422_VYUY;
  else if (!strcmp(pixFmt, "RGB565"))
    enPixelFormat = RK_FMT_RGB565;
  else if (!strcmp(pixFmt, "BGR565"))
    enPixelFormat = RK_FMT_BGR565;
  else if (!strcmp(pixFmt, "RGB555"))
    enPixelFormat = RK_FMT_RGB555;
  else if (!strcmp(pixFmt, "BGR555"))
    enPixelFormat = RK_FMT_BGR555;
  else if (!strcmp(pixFmt, "RGB444"))
    enPixelFormat = RK_FMT_RGB444;
  else if (!strcmp(pixFmt, "BGR444"))
    enPixelFormat = RK_FMT_BGR444;
  else if (!strcmp(pixFmt, "RGB888"))
    enPixelFormat = RK_FMT_RGB888;
  else if (!strcmp(pixFmt, "BGR888"))
    enPixelFormat = RK_FMT_BGR888;
  else if (!strcmp(pixFmt, "RGB101010"))
    enPixelFormat = RK_FMT_RGB101010;
  else if (!strcmp(pixFmt, "BGR101010"))
    enPixelFormat = RK_FMT_BGR101010;
  else if (!strcmp(pixFmt, "ARGB1555"))
    enPixelFormat = RK_FMT_ARGB1555;
  else if (!strcmp(pixFmt, "ABGR1555"))
    enPixelFormat = RK_FMT_ABGR1555;
  else if (!strcmp(pixFmt, "ARGB4444"))
    enPixelFormat = RK_FMT_ARGB4444;
  else if (!strcmp(pixFmt, "ABGR4444"))
    enPixelFormat = RK_FMT_ABGR4444;
  else if (!strcmp(pixFmt, "ARGB8565"))
    enPixelFormat = RK_FMT_ARGB8565;
  else if (!strcmp(pixFmt, "ABGR8565"))
    enPixelFormat = RK_FMT_ABGR8565;
  else if (!strcmp(pixFmt, "ARGB8888"))
    enPixelFormat = RK_FMT_ARGB8888;
  else if (!strcmp(pixFmt, "ABGR8888"))
    enPixelFormat = RK_FMT_ABGR8888;
  else if (!strcmp(pixFmt, "BGRA8888"))
    enPixelFormat = RK_FMT_BGRA8888;
  else if (!strcmp(pixFmt, "RGBA8888"))
    enPixelFormat = RK_FMT_RGBA8888;
  else if (!strcmp(pixFmt, "RGBA5551"))
    enPixelFormat = RK_FMT_RGBA5551;
  else if (!strcmp(pixFmt, "BGRA5551"))
    enPixelFormat = RK_FMT_BGRA5551;
  else if (!strcmp(pixFmt, "BGRA4444"))
    enPixelFormat = RK_FMT_BGRA4444;
  else if (!strcmp(pixFmt, "RGB_BAYER_SBGGR_8BPP"))
    enPixelFormat = RK_FMT_RGB_BAYER_SBGGR_8BPP;
  else if (!strcmp(pixFmt, "RGB_BAYER_SGBRG_8BPP"))
    enPixelFormat = RK_FMT_RGB_BAYER_SGBRG_8BPP;
  else if (!strcmp(pixFmt, "RGB_BAYER_SGRBG_8BPP"))
    enPixelFormat = RK_FMT_RGB_BAYER_SGRBG_8BPP;
  else if (!strcmp(pixFmt, "RGB_BAYER_SRGGB_8BPP"))
    enPixelFormat = RK_FMT_RGB_BAYER_SRGGB_8BPP;
  else if (!strcmp(pixFmt, "RGB_BAYER_SBGGR_10BPP"))
    enPixelFormat = RK_FMT_RGB_BAYER_SBGGR_10BPP;
  else if (!strcmp(pixFmt, "RGB_BAYER_SGBRG_10BPP"))
    enPixelFormat = RK_FMT_RGB_BAYER_SGBRG_10BPP;
  else if (!strcmp(pixFmt, "RGB_BAYER_SGRBG_10BPP"))
    enPixelFormat = RK_FMT_RGB_BAYER_SGRBG_10BPP;
  else if (!strcmp(pixFmt, "RGB_BAYER_SRGGB_10BPP"))
    enPixelFormat = RK_FMT_RGB_BAYER_SRGGB_10BPP;
  else if (!strcmp(pixFmt, "RGB_BAYER_SBGGR_12BPP"))
    enPixelFormat = RK_FMT_RGB_BAYER_SBGGR_12BPP;
  else if (!strcmp(pixFmt, "RGB_BAYER_SGBRG_12BPP"))
    enPixelFormat = RK_FMT_RGB_BAYER_SGBRG_12BPP;
  else if (!strcmp(pixFmt, "RGB_BAYER_SGRBG_12BPP"))
    enPixelFormat = RK_FMT_RGB_BAYER_SGRBG_12BPP;
  else if (!strcmp(pixFmt, "RGB_BAYER_SRGGB_12BPP"))
    enPixelFormat = RK_FMT_RGB_BAYER_SRGGB_12BPP;
  else if (!strcmp(pixFmt, "RGB_BAYER_14BPP"))
    enPixelFormat = RK_FMT_RGB_BAYER_14BPP;
  else if (!strcmp(pixFmt, "RGB_BAYER_SBGGR_16BPP"))
    enPixelFormat = RK_FMT_RGB_BAYER_SBGGR_16BPP;
  else
    RKADK_LOGE("Invalid image type: %s", pixFmt);

  return enPixelFormat;
}

static RKADK_S32 RKADK_PARAM_SetStreamViAttr(RKADK_S32 s32CamId,
                                             RKADK_STREAM_TYPE_E enStrmType) {
  int index;
  RKADK_PARAM_STREAM_CFG_S *pstStreamCfg;
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg =
      &g_stPARAMCtx.stCfg.stSensorCfg[s32CamId];
  RKADK_PARAM_VI_CFG_S *pstViCfg = NULL;

  RKADK_CHECK_CAMERAID(s32CamId, RKADK_FAILURE);

  if (enStrmType == RKADK_STREAM_TYPE_PREVIEW)
    pstStreamCfg = &g_stPARAMCtx.stCfg.stMediaCfg[s32CamId].stStreamCfg;
  else
    pstStreamCfg = &g_stPARAMCtx.stCfg.stMediaCfg[s32CamId].stLiveCfg;

  index = RKADK_PARAM_MatchViIndex(enStrmType, s32CamId,
                                   pstStreamCfg->attribute.width,
                                   pstStreamCfg->attribute.height);
  if (index < 0 || index >= RKADK_ISPP_VI_NODE_CNT) {
    RKADK_LOGE("not find match vi index");
    return -1;
  }

  pstViCfg = &g_stPARAMCtx.stCfg.stMediaCfg[s32CamId].stViCfg[index];
  pstStreamCfg->vi_attr.u32ViChn = pstViCfg->chn_id;
  memcpy(pstStreamCfg->vi_attr.stChnAttr.stIspOpt.aEntityName,
         pstViCfg->device_name, sizeof(pstViCfg->device_name));
  pstStreamCfg->vi_attr.stChnAttr.stIspOpt.u32BufCount = pstViCfg->buf_cnt;
  pstStreamCfg->vi_attr.stChnAttr.stIspOpt.enMemoryType =
      VI_V4L2_MEMORY_TYPE_DMABUF;
  pstStreamCfg->vi_attr.stChnAttr.stIspOpt.stMaxSize.u32Width = pstViCfg->width;
  pstStreamCfg->vi_attr.stChnAttr.stIspOpt.stMaxSize.u32Height = pstViCfg->height;
  pstStreamCfg->vi_attr.stChnAttr.stSize.u32Width = pstViCfg->width;
  pstStreamCfg->vi_attr.stChnAttr.stSize.u32Height = pstViCfg->height;
  pstStreamCfg->vi_attr.stChnAttr.enPixelFormat = RKADK_PARAM_GetPixFmt(
      pstViCfg->pix_fmt, &(pstStreamCfg->vi_attr.stChnAttr.enCompressMode));
  pstStreamCfg->vi_attr.stChnAttr.u32Depth = 0;
  pstStreamCfg->vi_attr.stChnAttr.stFrameRate.s32SrcFrameRate = -1;
  pstStreamCfg->vi_attr.stChnAttr.stFrameRate.s32DstFrameRate = -1;
  if (!pstSensorCfg->used_isp && pstSensorCfg->mirror)
    pstStreamCfg->vi_attr.stChnAttr.bMirror = RK_TRUE;
  if (!pstSensorCfg->used_isp && pstSensorCfg->flip)
    pstStreamCfg->vi_attr.stChnAttr.bFlip = RK_TRUE;

  return 0;
}

static RKADK_S32 RKADK_PARAM_SetPhotoViAttr(RKADK_S32 s32CamId) {
  int index;
  RKADK_PARAM_PHOTO_CFG_S *pstPhotoCfg =
      &g_stPARAMCtx.stCfg.stMediaCfg[s32CamId].stPhotoCfg;
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg =
      &g_stPARAMCtx.stCfg.stSensorCfg[s32CamId];
  RKADK_PARAM_VI_CFG_S *pstViCfg = NULL;

  RKADK_CHECK_CAMERAID(s32CamId, RKADK_FAILURE);

  index = RKADK_PARAM_MatchViIndex(RKADK_STREAM_TYPE_SNAP, s32CamId,
                                   pstPhotoCfg->image_width,
                                   pstPhotoCfg->image_height);
  if (index < 0 || index >= RKADK_ISPP_VI_NODE_CNT) {
    RKADK_LOGE("not find match vi index");
    return -1;
  }

  pstViCfg = &g_stPARAMCtx.stCfg.stMediaCfg[s32CamId].stViCfg[index];
  pstPhotoCfg->vi_attr.u32ViChn = pstViCfg->chn_id;
  memcpy(pstPhotoCfg->vi_attr.stChnAttr.stIspOpt.aEntityName,
         pstViCfg->device_name, sizeof(pstViCfg->device_name));
  pstPhotoCfg->vi_attr.stChnAttr.stIspOpt.u32BufCount = pstViCfg->buf_cnt;
  pstPhotoCfg->vi_attr.stChnAttr.stIspOpt.enMemoryType =
      VI_V4L2_MEMORY_TYPE_DMABUF;
  pstPhotoCfg->vi_attr.stChnAttr.stIspOpt.stMaxSize.u32Width = pstSensorCfg->max_width;
  pstPhotoCfg->vi_attr.stChnAttr.stIspOpt.stMaxSize.u32Height = pstSensorCfg->max_height;
  pstPhotoCfg->vi_attr.stChnAttr.stSize.u32Width = pstViCfg->width;
  pstPhotoCfg->vi_attr.stChnAttr.stSize.u32Height = pstViCfg->height;
  pstPhotoCfg->vi_attr.stChnAttr.enPixelFormat = RKADK_PARAM_GetPixFmt(
      pstViCfg->pix_fmt, &(pstPhotoCfg->vi_attr.stChnAttr.enCompressMode));
  pstPhotoCfg->vi_attr.stChnAttr.u32Depth = 0;
  pstPhotoCfg->vi_attr.stChnAttr.stFrameRate.s32SrcFrameRate = -1;
  pstPhotoCfg->vi_attr.stChnAttr.stFrameRate.s32DstFrameRate = -1;
  if (!pstSensorCfg->used_isp && pstSensorCfg->mirror)
    pstPhotoCfg->vi_attr.stChnAttr.bMirror = RK_TRUE;
  if (!pstSensorCfg->used_isp && pstSensorCfg->flip)
    pstPhotoCfg->vi_attr.stChnAttr.bFlip = RK_TRUE;

  return 0;
}

static RKADK_S32 RKADK_PARAM_SetRecViAttr(RKADK_S32 s32CamId) {
  int i, index;
  RKADK_STREAM_TYPE_E enStrmType = RKADK_STREAM_TYPE_BUTT;
  RKADK_PARAM_REC_CFG_S *pstRecCfg =
      &g_stPARAMCtx.stCfg.stMediaCfg[s32CamId].stRecCfg;
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg =
      &g_stPARAMCtx.stCfg.stSensorCfg[s32CamId];
  RKADK_PARAM_VI_CFG_S *pstViCfg = NULL;

  RKADK_CHECK_CAMERAID(s32CamId, RKADK_FAILURE);

  for (i = 0; i < (int)pstRecCfg->file_num; i++) {
    if (i == 0)
      enStrmType = RKADK_STREAM_TYPE_VIDEO_MAIN;
    else
      enStrmType = RKADK_STREAM_TYPE_VIDEO_SUB;

    index = RKADK_PARAM_MatchViIndex(enStrmType, s32CamId,
                                     pstRecCfg->attribute[i].width,
                                     pstRecCfg->attribute[i].height);
    if (index < 0 || index >= RKADK_ISPP_VI_NODE_CNT) {
      RKADK_LOGE("not find match vi index");
      return -1;
    }

    pstViCfg = &g_stPARAMCtx.stCfg.stMediaCfg[s32CamId].stViCfg[index];
    pstRecCfg->vi_attr[i].u32ViChn = pstViCfg->chn_id;
    memcpy(pstRecCfg->vi_attr[i].stChnAttr.stIspOpt.aEntityName,
           pstViCfg->device_name, sizeof(pstViCfg->device_name));
    pstRecCfg->vi_attr[i].stChnAttr.stIspOpt.u32BufCount = pstViCfg->buf_cnt;
    pstRecCfg->vi_attr[i].stChnAttr.stIspOpt.enMemoryType =
        VI_V4L2_MEMORY_TYPE_DMABUF;
    if (i == 0) {
      //main stream;
      pstRecCfg->vi_attr[i].stChnAttr.stIspOpt.stMaxSize.u32Width = pstSensorCfg->max_width;
      pstRecCfg->vi_attr[i].stChnAttr.stIspOpt.stMaxSize.u32Height = pstSensorCfg->max_height;
    } else {
      //sub stream;
      pstRecCfg->vi_attr[i].stChnAttr.stIspOpt.stMaxSize.u32Width = pstViCfg->width;
      pstRecCfg->vi_attr[i].stChnAttr.stIspOpt.stMaxSize.u32Height = pstViCfg->height;
    }
    pstRecCfg->vi_attr[i].stChnAttr.stSize.u32Width = pstViCfg->width;
    pstRecCfg->vi_attr[i].stChnAttr.stSize.u32Height = pstViCfg->height;
    pstRecCfg->vi_attr[i].stChnAttr.enPixelFormat = RKADK_PARAM_GetPixFmt(
        pstViCfg->pix_fmt, &(pstRecCfg->vi_attr[i].stChnAttr.enCompressMode));
    pstRecCfg->vi_attr[i].stChnAttr.u32Depth = 0;
    pstRecCfg->vi_attr[i].stChnAttr.stFrameRate.s32SrcFrameRate = -1;
    pstRecCfg->vi_attr[i].stChnAttr.stFrameRate.s32DstFrameRate = -1;
    if (!pstSensorCfg->used_isp && pstSensorCfg->mirror)
      pstRecCfg->vi_attr[i].stChnAttr.bMirror = RK_TRUE;
    if (!pstSensorCfg->used_isp && pstSensorCfg->flip)
      pstRecCfg->vi_attr[i].stChnAttr.bFlip = RK_TRUE;
  }

  return 0;
}

static RKADK_S32 RKADK_PARAM_SetDispViAttr(RKADK_S32 s32CamId) {
  int index;
  bool bSensorHorizontal = false, bDispHorizontal = false;
  RKADK_U32 width, height;
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg =
      &g_stPARAMCtx.stCfg.stSensorCfg[s32CamId];
  RKADK_PARAM_DISP_CFG_S *pstDispCfg =
      &g_stPARAMCtx.stCfg.stMediaCfg[s32CamId].stDispCfg;
  RKADK_PARAM_VI_CFG_S *pstViCfg = NULL;

  RKADK_CHECK_CAMERAID(s32CamId, RKADK_FAILURE);

  if (pstSensorCfg->max_width > pstSensorCfg->max_height)
    bSensorHorizontal = true;

  if (pstDispCfg->width > pstDispCfg->height)
    bDispHorizontal = true;

  if ((bSensorHorizontal && bDispHorizontal) ||
      (!bSensorHorizontal && !bDispHorizontal)) {
    width = pstDispCfg->width;
    height = pstDispCfg->height;
  } else {
    width = pstDispCfg->height;
    height = pstDispCfg->width;
  }

  index = RKADK_PARAM_MatchViIndex(RKADK_STREAM_TYPE_DISP, s32CamId, width, height);
  if (index < 0 || index >= RKADK_ISPP_VI_NODE_CNT) {
    RKADK_LOGE("not find match vi index");
    return -1;
  }

  pstViCfg = &g_stPARAMCtx.stCfg.stMediaCfg[s32CamId].stViCfg[index];
  pstDispCfg->vi_attr.u32ViChn = pstViCfg->chn_id;
  memcpy(pstDispCfg->vi_attr.stChnAttr.stIspOpt.aEntityName,
         pstViCfg->device_name, sizeof(pstViCfg->device_name));
  pstDispCfg->vi_attr.stChnAttr.stIspOpt.u32BufCount = pstViCfg->buf_cnt;
  pstDispCfg->vi_attr.stChnAttr.stIspOpt.enMemoryType =
      VI_V4L2_MEMORY_TYPE_DMABUF;
  pstDispCfg->vi_attr.stChnAttr.stIspOpt.stMaxSize.u32Width = pstViCfg->width;
  pstDispCfg->vi_attr.stChnAttr.stIspOpt.stMaxSize.u32Height = pstViCfg->height;
  pstDispCfg->vi_attr.stChnAttr.stSize.u32Width = pstViCfg->width;
  pstDispCfg->vi_attr.stChnAttr.stSize.u32Height = pstViCfg->height;
  pstDispCfg->vi_attr.stChnAttr.enPixelFormat = RKADK_PARAM_GetPixFmt(
      pstViCfg->pix_fmt, &(pstDispCfg->vi_attr.stChnAttr.enCompressMode));
  pstDispCfg->vi_attr.stChnAttr.u32Depth = 0;
  pstDispCfg->vi_attr.stChnAttr.stFrameRate.s32SrcFrameRate = -1;
  pstDispCfg->vi_attr.stChnAttr.stFrameRate.s32DstFrameRate = -1;
  if (!pstSensorCfg->used_isp && pstSensorCfg->mirror)
    pstDispCfg->vi_attr.stChnAttr.bMirror = RK_TRUE;
  if (!pstSensorCfg->used_isp && pstSensorCfg->flip)
    pstDispCfg->vi_attr.stChnAttr.bFlip = RK_TRUE;

  return 0;
}

static RKADK_S32 RKADK_PARAM_SetThumbViAttr(RKADK_S32 s32CamId) {
  int index;
  RKADK_PARAM_THUMB_CFG_S *pstThumbCfg =
      &g_stPARAMCtx.stCfg.stMediaCfg[s32CamId].stThumbCfg;
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg =
      &g_stPARAMCtx.stCfg.stSensorCfg[s32CamId];
  RKADK_PARAM_VI_CFG_S *pstViCfg = NULL;

  RKADK_CHECK_CAMERAID(s32CamId, RKADK_FAILURE);

  index = RKADK_PARAM_MatchViIndex(RKADK_STREAM_TYPE_THUMB, s32CamId,
                                   pstThumbCfg->thumb_width,
                                   pstThumbCfg->thumb_height);
  if (index < 0 || index >= RKADK_ISPP_VI_NODE_CNT) {
    RKADK_LOGE("not find match vi index");
    return -1;
  }

  pstViCfg = &g_stPARAMCtx.stCfg.stMediaCfg[s32CamId].stViCfg[index];
  pstThumbCfg->vi_attr.u32ViChn = pstViCfg->chn_id;
  memcpy(pstThumbCfg->vi_attr.stChnAttr.stIspOpt.aEntityName,
         pstViCfg->device_name, sizeof(pstViCfg->device_name));
  pstThumbCfg->vi_attr.stChnAttr.stIspOpt.u32BufCount = pstViCfg->buf_cnt;
  pstThumbCfg->vi_attr.stChnAttr.stIspOpt.enMemoryType =
      VI_V4L2_MEMORY_TYPE_DMABUF;
  pstThumbCfg->vi_attr.stChnAttr.stIspOpt.stMaxSize.u32Width = pstViCfg->width;
  pstThumbCfg->vi_attr.stChnAttr.stIspOpt.stMaxSize.u32Height = pstViCfg->height;
  pstThumbCfg->vi_attr.stChnAttr.stSize.u32Width = pstViCfg->width;
  pstThumbCfg->vi_attr.stChnAttr.stSize.u32Height = pstViCfg->height;
  pstThumbCfg->vi_attr.stChnAttr.enPixelFormat = RKADK_PARAM_GetPixFmt(
      pstViCfg->pix_fmt, &(pstThumbCfg->vi_attr.stChnAttr.enCompressMode));
  pstThumbCfg->vi_attr.stChnAttr.u32Depth = 0;
  pstThumbCfg->vi_attr.stChnAttr.stFrameRate.s32SrcFrameRate = -1;
  pstThumbCfg->vi_attr.stChnAttr.stFrameRate.s32DstFrameRate = -1;
  if (!pstSensorCfg->used_isp && pstSensorCfg->mirror)
    pstThumbCfg->vi_attr.stChnAttr.bMirror = RK_TRUE;
  if (!pstSensorCfg->used_isp && pstSensorCfg->flip)
    pstThumbCfg->vi_attr.stChnAttr.bFlip = RK_TRUE;

  return 0;
}

static RKADK_S32 RKADK_PARAM_SetMediaViAttr() {
  int i, ret = 0;

  for (i = 0; i < (int)g_stPARAMCtx.stCfg.stCommCfg.sensor_count; i++) {
    // Must be called before setRecattr
    ret = RKADK_PARAM_SetStreamViAttr(i, RKADK_STREAM_TYPE_PREVIEW);
    if (ret)
      break;

    ret = RKADK_PARAM_SetStreamViAttr(i, RKADK_STREAM_TYPE_LIVE);
    if (ret)
      break;

    // Must be called before SetPhotoAttr
    ret = RKADK_PARAM_SetRecViAttr(i);
    if (ret)
      break;

    ret = RKADK_PARAM_SetPhotoViAttr(i);
    if (ret)
      break;

    ret = RKADK_PARAM_SetDispViAttr(i);
    if (ret)
      break;

    ret = RKADK_PARAM_SetThumbViAttr(i);
    if (ret)
      break;
  }

#ifdef RKADK_DUMP_CONFIG
  RKADK_PARAM_DumpViAttr();
#endif

  return ret;
}

static void RKADK_PARAM_SetMicVolume(RKADK_U32 volume) {
  char buffer[RKADK_VOLUME_LEN];
  if (volume < 0 || volume > 100) {
    RKADK_LOGE("Set mic input volume failed. Mic input volume range is [0,100]");
    return;
  }

  RKADK_LOGI("volume = %d", volume);
  volume = (int)(volume * 2.55 + 0.5);
  memset(buffer, 0, RKADK_VOLUME_LEN);
  sprintf(buffer, "%d", volume);
  RK_MPI_AMIX_SetControl(0, "ADC Digital Left Volume", buffer);
  RK_MPI_AMIX_SetControl(0, "ADC Digital Right Volume", buffer);
}

static void RKADK_PARAM_SetSpeakerVolume(RKADK_U32 volume) {
  char buffer[RKADK_VOLUME_LEN];
  if (volume < 0 || volume > 100) {
    RKADK_LOGE("Set speaker volume failed. Speaker volume range is[0,100]");
    return;
  }

  RKADK_LOGI("volume = %d", volume);
  volume = (int)(volume * 0.3 + 0.5);
  memset(buffer, 0, RKADK_VOLUME_LEN);
  sprintf(buffer, "%d", volume);
  RK_MPI_AMIX_SetControl(0, "DAC LINEOUT Volume", buffer);
}

static void RKADK_PARAM_RecMute(bool mute, RKADK_U32 volume) {
  char buffer[RKADK_VOLUME_LEN];
  volume = (int)(volume * 2.55 + 0.5);
  memset(buffer, 0, RKADK_VOLUME_LEN);
  sprintf(buffer, "%d", volume);

  RKADK_LOGI("RKADK_PARAM_RecMute mute = %d", mute);

  if (mute) {
    RK_MPI_AENC_SetMute(RECORD_AENC_CHN, (RK_BOOL)mute);
  } else {
    RK_MPI_AMIX_SetControl(0, "ADC Digital Left Volume", buffer);
    RK_MPI_AMIX_SetControl(0, "ADC Digital Right Volume", buffer);
    RK_MPI_AENC_SetMute(RECORD_AENC_CHN, (RK_BOOL)mute);
  }
}

static void RKADK_PARAM_SetVolume() {
  RKADK_PARAM_COMM_CFG_S *pstCommCfg = &g_stPARAMCtx.stCfg.stCommCfg;

  RKADK_PARAM_SetMicVolume(pstCommCfg->mic_volume);
  RKADK_PARAM_SetSpeakerVolume(pstCommCfg->speaker_volume);
}

VENC_RC_MODE_E RKADK_PARAM_GetRcMode(char *rcMode,
                                     RKADK_CODEC_TYPE_E enCodecType) {
  VENC_RC_MODE_E enRcMode = VENC_RC_MODE_BUTT;

  RKADK_CHECK_POINTER(rcMode, VENC_RC_MODE_BUTT);

  switch (enCodecType) {
  case RKADK_CODEC_TYPE_H264:
    if (!strcmp(rcMode, "CBR"))
      enRcMode = VENC_RC_MODE_H264CBR;
    else if (!strcmp(rcMode, "VBR"))
      enRcMode = VENC_RC_MODE_H264VBR;
    else if (!strcmp(rcMode, "AVBR"))
      enRcMode = VENC_RC_MODE_H264AVBR;
    break;
  case RKADK_CODEC_TYPE_H265:
    if (!strcmp(rcMode, "CBR"))
      enRcMode = VENC_RC_MODE_H265CBR;
    else if (!strcmp(rcMode, "VBR"))
      enRcMode = VENC_RC_MODE_H265VBR;
    else if (!strcmp(rcMode, "AVBR"))
      enRcMode = VENC_RC_MODE_H265AVBR;
    break;
  case RKADK_CODEC_TYPE_MJPEG:
    if (!strcmp(rcMode, "CBR"))
      enRcMode = VENC_RC_MODE_MJPEGCBR;
    else if (!strcmp(rcMode, "VBR"))
      enRcMode = VENC_RC_MODE_MJPEGVBR;
    break;
  default:
    RKADK_LOGE("Nonsupport codec type: %d", enCodecType);
    break;
  }

  return enRcMode;
}

static RKADK_S32 RKADK_PARAM_SetRcParam(RKADK_PARAM_VENC_ATTR_S stVencAttr) {
  bool bSetRcParam = false;
  int ret = 0;
  RKADK_S32 u32StepQp, u32MinQp, u32MaxQp;
  VENC_RC_PARAM_S stRcParam;

  ret = RK_MPI_VENC_GetRcParam(stVencAttr.venc_chn, &stRcParam);
  if (ret) {
    RKADK_LOGE("venc_chn[%d] GetRcParam failed[%d]", stVencAttr.venc_chn, ret);
    return -1;
  }

  if (stVencAttr.codec_type == RKADK_CODEC_TYPE_H264) {
    u32StepQp = stRcParam.stParamH264.u32StepQp;
    u32MaxQp = stRcParam.stParamH264.u32MaxQp;
    u32MinQp= stRcParam.stParamH264.u32MinQp;
  } else if (stVencAttr.codec_type == RKADK_CODEC_TYPE_H265) {
    u32StepQp = stRcParam.stParamH265.u32StepQp;
    u32MaxQp = stRcParam.stParamH265.u32MaxQp;
    u32MinQp= stRcParam.stParamH265.u32MinQp;
  } else {
    return -1;
  }

  if (stVencAttr.venc_param.qp_step > 0) {
    u32StepQp = stVencAttr.venc_param.qp_step;
    bSetRcParam = true;
  }

  if (stVencAttr.venc_param.max_qp >= 1 && stVencAttr.venc_param.max_qp <= 51) {
    u32MaxQp = stVencAttr.venc_param.max_qp;
    bSetRcParam = true;
  }

  if (stVencAttr.venc_param.min_qp >= 1
    && stVencAttr.venc_param.min_qp <= stVencAttr.venc_param.max_qp) {
    u32MinQp = stVencAttr.venc_param.min_qp;
    bSetRcParam = true;
  }

  if (stVencAttr.venc_param.first_frame_qp >= stVencAttr.venc_param.min_qp
    && stVencAttr.venc_param.first_frame_qp <= stVencAttr.venc_param.max_qp) {
    stRcParam.s32FirstFrameStartQp = stVencAttr.venc_param.first_frame_qp;
    bSetRcParam = true;
  }

  if (!bSetRcParam)
    return 0;

  switch (stVencAttr.codec_type) {
  case RKADK_CODEC_TYPE_H264:
    stRcParam.stParamH264.u32StepQp = u32StepQp;
    stRcParam.stParamH264.u32MaxQp = u32MaxQp;
    stRcParam.stParamH264.u32MinQp = u32MinQp;
    break;
  case RKADK_CODEC_TYPE_H265:
    stRcParam.stParamH265.u32StepQp = u32StepQp;
    stRcParam.stParamH265.u32MaxQp = u32MaxQp;
    stRcParam.stParamH265.u32MinQp = u32MinQp;
    break;
  default:
    RKADK_LOGE("Nonsupport codec type: %d", stVencAttr.codec_type);
    return -1;
  }

  ret = RK_MPI_VENC_SetRcParam(stVencAttr.venc_chn, &stRcParam);
  if (ret) {
    RKADK_LOGE("venc_chn[%d] SetRcParam failed[%d]", stVencAttr.venc_chn, ret);
    return -1;
  }

  return 0;
}

static RKADK_S32 RKADK_PARAM_SetVencTrans(RKADK_PARAM_VENC_ATTR_S stVencAttr) {
  int ret = 0;
  RKADK_U32 scalingList;
  VENC_H264_TRANS_S stH264Trans;
  VENC_H265_TRANS_S stH265Trans;

  scalingList = stVencAttr.venc_param.scaling_list ? 1 : 0;

  switch (stVencAttr.codec_type) {
  case RKADK_CODEC_TYPE_H264:
    ret = RK_MPI_VENC_GetH264Trans(stVencAttr.venc_chn, &stH264Trans);
    if (ret) {
      RKADK_LOGE("venc_chn[%d] GetH264Trans failed[%d]", stVencAttr.venc_chn, ret);
      return -1;
    }

    if (stH264Trans.bScalingListValid == scalingList)
      return 0;

    stH264Trans.bScalingListValid = scalingList;
    ret = RK_MPI_VENC_SetH264Trans(stVencAttr.venc_chn, &stH264Trans);
    if (ret) {
      RKADK_LOGE("venc_chn[%d] SetH264Trans failed[%d]", stVencAttr.venc_chn, ret);
      return -1;
    }

    break;
  case RKADK_CODEC_TYPE_H265:
    ret = RK_MPI_VENC_GetH265Trans(stVencAttr.venc_chn, &stH265Trans);
    if (ret) {
      RKADK_LOGE("venc_chn[%d] GetH265Trans failed[%d]", stVencAttr.venc_chn, ret);
      return -1;
    }

    if (stH265Trans.bScalingListEnabled == scalingList)
      return 0;

    stH265Trans.bScalingListEnabled = scalingList;
    ret = RK_MPI_VENC_SetH265Trans(stVencAttr.venc_chn, &stH265Trans);
    if (ret) {
      RKADK_LOGE("venc_chn[%d] SetH265Trans failed[%d]", stVencAttr.venc_chn, ret);
      return -1;
    }

    break;
  default:
    RKADK_LOGE("Nonsupport codec type: %d", stVencAttr.codec_type);
    return -1;
  }

  return 0;
}

static RKADK_S32 RKADK_PARAM_SetVencVui(RKADK_PARAM_VENC_ATTR_S stVencAttr) {
  int ret = 0, fullRange;
  VENC_H264_VUI_S stH264Vui;
  VENC_H265_VUI_S stH265Vui;

  fullRange = stVencAttr.venc_param.full_range ? 1 : 0;

  switch (stVencAttr.codec_type) {
  case RKADK_CODEC_TYPE_H264:
    ret = RK_MPI_VENC_GetH264Vui(stVencAttr.venc_chn, &stH264Vui);
    if (ret) {
      RKADK_LOGE("venc_chn[%d] GetH264Vui failed[%d]", stVencAttr.venc_chn, ret);
      return -1;
    }

    if (stH264Vui.stVuiVideoSignal.video_full_range_flag == fullRange)
      return 0;

    stH264Vui.stVuiVideoSignal.video_full_range_flag = fullRange;
    ret = RK_MPI_VENC_SetH264Vui(stVencAttr.venc_chn, &stH264Vui);
    if(ret) {
      RKADK_LOGE("venc_chn[%d] SetH264Vui failed[%d]", stVencAttr.venc_chn, ret);
      return -1;
    }

    break;
  case RKADK_CODEC_TYPE_H265:
    ret = RK_MPI_VENC_GetH265Vui(stVencAttr.venc_chn, &stH265Vui);
    if (ret) {
      RKADK_LOGE("venc_chn[%d] GetH265Vui failed[%d]", stVencAttr.venc_chn, ret);
      return -1;
    }

    if (stH265Vui.stVuiVideoSignal.video_full_range_flag == fullRange)
      return 0;

    stH265Vui.stVuiVideoSignal.video_full_range_flag = fullRange;
    ret = RK_MPI_VENC_SetH265Vui(stVencAttr.venc_chn, &stH265Vui);
    if(ret) {
      RKADK_LOGE("venc_chn[%d] SetH265Vui failed[%d]", stVencAttr.venc_chn, ret);
      return -1;
    }

    break;
  default:
    RKADK_LOGE("Nonsupport codec type: %d", stVencAttr.codec_type);
    return -1;
  }

  return 0;
}

static void RKADK_PARAM_Strtok(char *input, RKADK_S32 *s32Output,
                               RKADK_S32 u32OutputLen, const char *delim) {
  char *p;

  for (int i = 0; i < u32OutputLen; i++) {
    if (!i)
      p = strtok(input, delim);
    else
      p = strtok(NULL, delim);

    if (!p)
      break;

    s32Output[i] = atoi(p);
  }
}

static RKADK_S32 RKADK_PARAM_SetVencHierarchicalQp(RKADK_PARAM_VENC_ATTR_S stVencAttr) {
  int ret = 0;
  VENC_HIERARCHICAL_QP_S stHierarchicalQp;

  //default false
  if (!stVencAttr.venc_param.hier_qp_en)
    return 0;

  ret = RK_MPI_VENC_GetHierarchicalQp(stVencAttr.venc_chn, &stHierarchicalQp);
  if (ret) {
    RKADK_LOGE("venc_chn[%d] GetHierarchicalQp failed[%d]", stVencAttr.venc_chn, ret);
    return -1;
  }

  stHierarchicalQp.bHierarchicalQpEn = RK_TRUE;
  if (!strcmp(stVencAttr.venc_param.hier_qp_delta, "NONE"))
    RKADK_PARAM_Strtok(stVencAttr.venc_param.hier_qp_delta,
                       stHierarchicalQp.s32HierarchicalQpDelta, 4, ",");

  if (!strcmp(stVencAttr.venc_param.hier_frame_num, "NONE"))
    RKADK_PARAM_Strtok(stVencAttr.venc_param.hier_frame_num,
                       stHierarchicalQp.s32HierarchicalFrameNum, 4, ",");

  ret = RK_MPI_VENC_SetHierarchicalQp(stVencAttr.venc_chn, &stHierarchicalQp);
  if (ret) {
    RKADK_LOGE("venc_chn[%d] SetHierarchicalQp failed[%d]", stVencAttr.venc_chn, ret);
    return -1;
  }

  return 0;
}

RKADK_S32 RKADK_PARAM_SetVAdvancedParam(RKADK_PARAM_VENC_ATTR_S stVencAttr) {
  int ret;

  ret = RKADK_PARAM_SetRcParam(stVencAttr);
  ret |= RKADK_PARAM_SetVencTrans(stVencAttr);
  ret |= RKADK_PARAM_SetVencVui(stVencAttr);
  ret |= RKADK_PARAM_SetVencHierarchicalQp(stVencAttr);

  return ret;
}

RKADK_PARAM_CONTEXT_S *RKADK_PARAM_GetCtx() {
  RKADK_CHECK_INIT(g_stPARAMCtx.bInit, NULL);
  return &g_stPARAMCtx;
}

RKADK_PARAM_COMM_CFG_S *RKADK_PARAM_GetCommCfg() {
  RKADK_CHECK_INIT(g_stPARAMCtx.bInit, NULL);
  return &g_stPARAMCtx.stCfg.stCommCfg;
}

RKADK_PARAM_REC_CFG_S *RKADK_PARAM_GetRecCfg(RKADK_U32 u32CamId) {
  RKADK_CHECK_INIT(g_stPARAMCtx.bInit, NULL);
  RKADK_CHECK_CAMERAID(u32CamId, NULL);
  return &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stRecCfg;
}

RKADK_PARAM_SENSOR_CFG_S *RKADK_PARAM_GetSensorCfg(RKADK_U32 u32CamId) {
  RKADK_CHECK_INIT(g_stPARAMCtx.bInit, NULL);
  RKADK_CHECK_CAMERAID(u32CamId, NULL);
  return &g_stPARAMCtx.stCfg.stSensorCfg[u32CamId];
}

RKADK_PARAM_STREAM_CFG_S *
RKADK_PARAM_GetStreamCfg(RKADK_U32 u32CamId, RKADK_STREAM_TYPE_E enStrmType) {
  RKADK_CHECK_INIT(g_stPARAMCtx.bInit, NULL);
  RKADK_CHECK_CAMERAID(u32CamId, NULL);

  if (enStrmType == RKADK_STREAM_TYPE_PREVIEW)
    return &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stStreamCfg;
  else
    return &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stLiveCfg;
}

RKADK_PARAM_PHOTO_CFG_S *RKADK_PARAM_GetPhotoCfg(RKADK_U32 u32CamId) {
  RKADK_CHECK_INIT(g_stPARAMCtx.bInit, NULL);
  RKADK_CHECK_CAMERAID(u32CamId, NULL);
  return &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stPhotoCfg;
}

RKADK_PARAM_DISP_CFG_S *RKADK_PARAM_GetDispCfg(RKADK_U32 u32CamId) {
  RKADK_CHECK_INIT(g_stPARAMCtx.bInit, NULL);
  RKADK_CHECK_CAMERAID(u32CamId, NULL);
  return &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stDispCfg;
}

RKADK_PARAM_THUMB_CFG_S *RKADK_PARAM_GetThumbCfg(RKADK_U32 u32CamId) {
  RKADK_CHECK_INIT(g_stPARAMCtx.bInit, NULL);
  RKADK_CHECK_CAMERAID(u32CamId, NULL);
  return &g_stPARAMCtx.stCfg.stMediaCfg[u32CamId].stThumbCfg;
}

RKADK_PARAM_AUDIO_CFG_S *RKADK_PARAM_GetAudioCfg() {
  RKADK_CHECK_INIT(g_stPARAMCtx.bInit, NULL);
  return &g_stPARAMCtx.stCfg.stAudioCfg;
}

RKADK_S32 RKADK_PARAM_GetVencChnId(RKADK_U32 u32CamId,
                                   RKADK_STREAM_TYPE_E enStrmType) {
  RKADK_S32 s32VencChnId = -1;

  RKADK_CHECK_CAMERAID(u32CamId, RKADK_FAILURE);

  switch (enStrmType) {
  case RKADK_STREAM_TYPE_VIDEO_MAIN: {
    RKADK_PARAM_REC_CFG_S *pstRecCfg = RKADK_PARAM_GetRecCfg(u32CamId);
    if (!pstRecCfg)
      RKADK_LOGE("RKADK_PARAM_GetRecCfg failed");
    else
      s32VencChnId = pstRecCfg->attribute[0].venc_chn;
    break;
  }
  case RKADK_STREAM_TYPE_VIDEO_SUB: {
    RKADK_PARAM_REC_CFG_S *pstRecCfg = RKADK_PARAM_GetRecCfg(u32CamId);
    if (!pstRecCfg)
      RKADK_LOGE("RKADK_PARAM_GetRecCfg failed");
    else
      s32VencChnId = pstRecCfg->attribute[1].venc_chn;
    break;
  }
  case RKADK_STREAM_TYPE_SNAP: {
    RKADK_PARAM_PHOTO_CFG_S *pstPhotoCfg = RKADK_PARAM_GetPhotoCfg(u32CamId);
    if (!pstPhotoCfg)
      RKADK_LOGE("RKADK_PARAM_GetPhotoCfg failed");
    else
      s32VencChnId = pstPhotoCfg->venc_chn;
    break;
  }
  case RKADK_STREAM_TYPE_PREVIEW:
  case RKADK_STREAM_TYPE_LIVE: {
    RKADK_PARAM_STREAM_CFG_S *pstStreamCfg =
        RKADK_PARAM_GetStreamCfg(u32CamId, enStrmType);
    if (!pstStreamCfg)
      RKADK_LOGE("RKADK_PARAM_GetStreamCfg failed");
    else
      s32VencChnId = pstStreamCfg->attribute.venc_chn;
    break;
  }
  default:
    RKADK_LOGE("Unsupport stream type: %d", enStrmType);
    break;
  }

  return s32VencChnId;
}

RKADK_PARAM_RES_E RKADK_PARAM_GetResType(RKADK_U32 width, RKADK_U32 height) {
  RKADK_PARAM_RES_E type = RKADK_RES_BUTT;

  if (width == RKADK_WIDTH_720P && height == RKADK_HEIGHT_720P)
    type = RKADK_RES_720P;
  else if (width == RKADK_WIDTH_1080P && height == RKADK_HEIGHT_1080P)
    type = RKADK_RES_1080P;
  else if (width == RKADK_WIDTH_1296P && height == RKADK_HEIGHT_1296P)
    type = RKADK_RES_1296P;
  else if (width == RKADK_WIDTH_1440P && height == RKADK_HEIGHT_1440P)
    type = RKADK_RES_1440P;
  else if (width == RKADK_WIDTH_1520P && height == RKADK_HEIGHT_1520P)
    type = RKADK_RES_1520P;
  else if (width == RKADK_WIDTH_1600P && height == RKADK_HEIGHT_1600P)
    type = RKADK_RES_1600P;
  else if (width == RKADK_WIDTH_1620P && height == RKADK_HEIGHT_1620P)
    type = RKADK_RES_1620P;
  else if (width == RKADK_WIDTH_1944P && height == RKADK_HEIGHT_1944P)
    type = RKADK_RES_1944P;
  else if (width == RKADK_WIDTH_3840P && height == RKADK_HEIGHT_2160P)
    type = RKADK_RES_2160P;
  else
    RKADK_LOGE("Unsupport resolution(%d*%d)", width, height);

  return type;
}

RKADK_S32 RKADK_PARAM_GetResolution(RKADK_PARAM_RES_E type, RKADK_U32 *width,
                                    RKADK_U32 *height) {
  switch (type) {
  case RKADK_RES_720P:
    *width = RKADK_WIDTH_720P;
    *height = RKADK_HEIGHT_720P;
    break;
  case RKADK_RES_1080P:
    *width = RKADK_WIDTH_1080P;
    *height = RKADK_HEIGHT_1080P;
    break;
  case RKADK_RES_1296P:
    *width = RKADK_WIDTH_1296P;
    *height = RKADK_HEIGHT_1296P;
    break;
  case RKADK_RES_1440P:
    *width = RKADK_WIDTH_1440P;
    *height = RKADK_HEIGHT_1440P;
    break;
  case RKADK_RES_1520P:
    *width = RKADK_WIDTH_1520P;
    *height = RKADK_HEIGHT_1520P;
    break;
  case RKADK_RES_1600P:
    *width = RKADK_WIDTH_1600P;
    *height = RKADK_HEIGHT_1600P;
    break;
  case RKADK_RES_1620P:
    *width = RKADK_WIDTH_1620P;
    *height = RKADK_HEIGHT_1620P;
    break;
  case RKADK_RES_1944P:
    *width = RKADK_WIDTH_1944P;
    *height = RKADK_HEIGHT_1944P;
    break;
  case RKADK_RES_2160P:
    *width = RKADK_WIDTH_3840P;
    *height = RKADK_HEIGHT_2160P;
    break;
  default:
    RKADK_LOGE("Unsupport resolution type: %d, set to 1080P", type);
    *width = RKADK_WIDTH_1080P;
    *height = RKADK_HEIGHT_1080P;
    break;
  }

  return 0;
}

static RKADK_CODEC_TYPE_E
RKADK_PARAM_GetCodecType(RKADK_S32 s32CamId, RKADK_STREAM_TYPE_E enStreamType) {
  RKADK_CODEC_TYPE_E enCodecType = RKADK_CODEC_TYPE_BUTT;
  RKADK_PARAM_REC_CFG_S *pstRecCfg;
  RKADK_PARAM_STREAM_CFG_S *pstStreamCfg;

  switch (enStreamType) {
  case RKADK_STREAM_TYPE_VIDEO_MAIN:
    pstRecCfg = RKADK_PARAM_GetRecCfg(s32CamId);
    enCodecType = pstRecCfg->attribute[0].codec_type;
    break;

  case RKADK_STREAM_TYPE_VIDEO_SUB:
    pstRecCfg = RKADK_PARAM_GetRecCfg(s32CamId);
    enCodecType = pstRecCfg->attribute[1].codec_type;
    break;

  case RKADK_STREAM_TYPE_PREVIEW:
  case RKADK_STREAM_TYPE_LIVE:
    pstStreamCfg = RKADK_PARAM_GetStreamCfg(s32CamId, enStreamType);
    enCodecType = pstStreamCfg->attribute.codec_type;
    break;
  default:
    RKADK_LOGE("Unsupport enStreamType: %d", enStreamType);
    break;
  }

  return enCodecType;
}

static RKADK_S32
RKADK_PARAM_SetCodecType(RKADK_S32 s32CamId,
                         RKADK_PARAM_CODEC_CFG_S *pstCodecCfg) {
  RKADK_S32 ret;
  RKADK_PARAM_REC_CFG_S *pstRecCfg;
  RKADK_PARAM_STREAM_CFG_S *pstStreamCfg;

  switch (pstCodecCfg->enStreamType) {
  case RKADK_STREAM_TYPE_VIDEO_MAIN:
    pstRecCfg = RKADK_PARAM_GetRecCfg(s32CamId);
    if (pstRecCfg->attribute[0].codec_type == pstCodecCfg->enCodecType)
      return 0;

    pstRecCfg->attribute[0].codec_type = pstCodecCfg->enCodecType;
    ret = RKADK_PARAM_SaveRecAttr(g_stPARAMCtx.sensorPath[s32CamId], s32CamId,
                                  RKADK_STREAM_TYPE_VIDEO_MAIN);
    break;

  case RKADK_STREAM_TYPE_VIDEO_SUB:
    pstRecCfg = RKADK_PARAM_GetRecCfg(s32CamId);
    if (pstRecCfg->attribute[1].codec_type == pstCodecCfg->enCodecType)
      return 0;

    pstRecCfg->attribute[1].codec_type = pstCodecCfg->enCodecType;
    ret = RKADK_PARAM_SaveRecAttr(g_stPARAMCtx.sensorPath[s32CamId], s32CamId,
                                  RKADK_STREAM_TYPE_VIDEO_SUB);
    break;

  case RKADK_STREAM_TYPE_PREVIEW:
  case RKADK_STREAM_TYPE_LIVE:
    pstStreamCfg =
        RKADK_PARAM_GetStreamCfg(s32CamId, pstCodecCfg->enStreamType);
    if (pstStreamCfg->attribute.codec_type == pstCodecCfg->enCodecType)
      return 0;

    pstStreamCfg->attribute.codec_type = pstCodecCfg->enCodecType;
    ret = RKADK_PARAM_SaveStreamCfg(g_stPARAMCtx.sensorPath[s32CamId], s32CamId,
                                    pstCodecCfg->enStreamType);
    break;

  default:
    RKADK_LOGE("Unsupport enStreamType: %d", pstCodecCfg->enStreamType);
    break;
  }

  return ret;
}

static RKADK_U32 RKADK_PARAM_GetBitrate(RKADK_S32 s32CamId,
                                        RKADK_STREAM_TYPE_E enStreamType) {
  RKADK_U32 bitrate = 0;
  RKADK_PARAM_REC_CFG_S *pstRecCfg;
  RKADK_PARAM_STREAM_CFG_S *pstStreamCfg;

  switch (enStreamType) {
  case RKADK_STREAM_TYPE_VIDEO_MAIN:
    pstRecCfg = RKADK_PARAM_GetRecCfg(s32CamId);
    bitrate = pstRecCfg->attribute[0].bitrate;
    break;

  case RKADK_STREAM_TYPE_VIDEO_SUB:
    pstRecCfg = RKADK_PARAM_GetRecCfg(s32CamId);
    bitrate = pstRecCfg->attribute[1].bitrate;
    break;

  case RKADK_STREAM_TYPE_PREVIEW:
  case RKADK_STREAM_TYPE_LIVE:
    pstStreamCfg = RKADK_PARAM_GetStreamCfg(s32CamId, enStreamType);
    bitrate = pstStreamCfg->attribute.bitrate;
    break;
  default:
    RKADK_LOGE("Unsupport enStreamType: %d", enStreamType);
    break;
  }

  return bitrate;
}

static RKADK_S32 RKADK_PARAM_SetBitrate(RKADK_S32 s32CamId,
                                        RKADK_PARAM_BITRATE_S *pstBitrate) {
  RKADK_S32 ret = -1;
  RKADK_PARAM_REC_CFG_S *pstRecCfg;
  RKADK_PARAM_STREAM_CFG_S *pstStreamCfg;

  switch (pstBitrate->enStreamType) {
  case RKADK_STREAM_TYPE_VIDEO_MAIN:
    pstRecCfg = RKADK_PARAM_GetRecCfg(s32CamId);
    if (pstRecCfg->attribute[0].bitrate == pstBitrate->u32Bitrate)
      return 0;

    pstRecCfg->attribute[0].bitrate = pstBitrate->u32Bitrate;
    ret = RKADK_PARAM_SaveRecAttr(g_stPARAMCtx.sensorPath[s32CamId], s32CamId,
                                  RKADK_STREAM_TYPE_VIDEO_MAIN);
    break;

  case RKADK_STREAM_TYPE_VIDEO_SUB:
    pstRecCfg = RKADK_PARAM_GetRecCfg(s32CamId);
    if (pstRecCfg->attribute[1].bitrate == pstBitrate->u32Bitrate)
      return 0;

    pstRecCfg->attribute[1].bitrate = pstBitrate->u32Bitrate;
    ret = RKADK_PARAM_SaveRecAttr(g_stPARAMCtx.sensorPath[s32CamId], s32CamId,
                                  RKADK_STREAM_TYPE_VIDEO_SUB);
    break;

  case RKADK_STREAM_TYPE_PREVIEW:
  case RKADK_STREAM_TYPE_LIVE:
    pstStreamCfg = RKADK_PARAM_GetStreamCfg(s32CamId, pstBitrate->enStreamType);
    if (pstStreamCfg->attribute.bitrate == pstBitrate->u32Bitrate)
      return 0;

    pstStreamCfg->attribute.bitrate = pstBitrate->u32Bitrate;
    ret = RKADK_PARAM_SaveStreamCfg(g_stPARAMCtx.sensorPath[s32CamId], s32CamId,
                                    pstBitrate->enStreamType);
    break;

  default:
    RKADK_LOGE("Unsupport enStreamType: %d", pstBitrate->enStreamType);
    break;
  }

  return ret;
}

static RKADK_U32 RKADK_PARAM_GetGop(RKADK_S32 s32CamId,
                                    RKADK_STREAM_TYPE_E enStreamType) {
  RKADK_U32 u32Gop = 0;
  RKADK_PARAM_REC_CFG_S *pstRecCfg;
  RKADK_PARAM_STREAM_CFG_S *pstStreamCfg;

  switch (enStreamType) {
  case RKADK_STREAM_TYPE_VIDEO_MAIN:
    pstRecCfg = RKADK_PARAM_GetRecCfg(s32CamId);
    u32Gop = pstRecCfg->attribute[0].gop;
    break;

  case RKADK_STREAM_TYPE_VIDEO_SUB:
    pstRecCfg = RKADK_PARAM_GetRecCfg(s32CamId);
    u32Gop = pstRecCfg->attribute[1].gop;
    break;

  case RKADK_STREAM_TYPE_PREVIEW:
  case RKADK_STREAM_TYPE_LIVE:
    pstStreamCfg = RKADK_PARAM_GetStreamCfg(s32CamId, enStreamType);
    u32Gop = pstStreamCfg->attribute.gop;
    break;
  default:
    RKADK_LOGE("Unsupport enStreamType: %d", enStreamType);
    break;
  }

  return u32Gop;
}

static RKADK_S32 RKADK_PARAM_SetGop(RKADK_S32 s32CamId,
                                    RKADK_PARAM_GOP_S *pstGop) {
  RKADK_S32 ret;
  RKADK_PARAM_REC_CFG_S *pstRecCfg;
  RKADK_PARAM_STREAM_CFG_S *pstStreamCfg;

  switch (pstGop->enStreamType) {
  case RKADK_STREAM_TYPE_VIDEO_MAIN:
    pstRecCfg = RKADK_PARAM_GetRecCfg(s32CamId);
    if (pstRecCfg->attribute[0].gop == pstGop->u32Gop)
      return 0;

    pstRecCfg->attribute[0].gop = pstGop->u32Gop;
    ret = RKADK_PARAM_SaveRecAttr(g_stPARAMCtx.sensorPath[s32CamId], s32CamId,
                                  RKADK_STREAM_TYPE_VIDEO_MAIN);
    break;

  case RKADK_STREAM_TYPE_VIDEO_SUB:
    pstRecCfg = RKADK_PARAM_GetRecCfg(s32CamId);
    if (pstRecCfg->attribute[1].gop == pstGop->u32Gop)
      return 0;

    pstRecCfg->attribute[1].gop = pstGop->u32Gop;
    ret = RKADK_PARAM_SaveRecAttr(g_stPARAMCtx.sensorPath[s32CamId], s32CamId,
                                  RKADK_STREAM_TYPE_VIDEO_SUB);
    break;

  case RKADK_STREAM_TYPE_PREVIEW:
  case RKADK_STREAM_TYPE_LIVE:
    pstStreamCfg = RKADK_PARAM_GetStreamCfg(s32CamId, pstGop->enStreamType);
    if (pstStreamCfg->attribute.gop == pstGop->u32Gop)
      return 0;

    pstStreamCfg->attribute.gop = pstGop->u32Gop;
    ret = RKADK_PARAM_SaveStreamCfg(g_stPARAMCtx.sensorPath[s32CamId], s32CamId,
                                    pstGop->enStreamType);
    break;

  default:
    RKADK_LOGE("Unsupport enStreamType: %d", pstGop->enStreamType);
    break;
  }

  return ret;
}

static RKADK_U32 RKADK_PARAM_GetRecTime(RKADK_S32 s32CamId,
                                        RKADK_STREAM_TYPE_E enStreamType,
                                        RKADK_PARAM_TYPE_E enParamType) {
  RKADK_U32 time = -1;
  RKADK_PARAM_REC_TIME_CFG_S *pstRecTimeCfg;

  if (enStreamType == RKADK_STREAM_TYPE_VIDEO_MAIN)
    pstRecTimeCfg =
        &g_stPARAMCtx.stCfg.stMediaCfg[s32CamId].stRecCfg.record_time_cfg[0];
  else
    pstRecTimeCfg =
        &g_stPARAMCtx.stCfg.stMediaCfg[s32CamId].stRecCfg.record_time_cfg[1];

  switch (enParamType) {
  case RKADK_PARAM_TYPE_RECORD_TIME:
    time = pstRecTimeCfg->record_time;
    break;
  case RKADK_PARAM_TYPE_SPLITTIME:
    time = pstRecTimeCfg->splite_time;
    break;
  case RKADK_PARAM_TYPE_LAPSE_INTERVAL:
    time = pstRecTimeCfg->lapse_interval;
    break;
  default:
    RKADK_LOGE("Invalid enParamType: %d", enParamType);
    break;
  }

  return time;
}

static RKADK_S32 RKADK_PARAM_SetRecTime(RKADK_S32 s32CamId,
                                        RKADK_PARAM_REC_TIME_S *pstRecTime,
                                        RKADK_PARAM_TYPE_E enParamType) {
  RKADK_S32 ret;
  RKADK_PARAM_REC_TIME_CFG_S *pstRecTimeCfg;

  if (pstRecTime->enStreamType == RKADK_STREAM_TYPE_VIDEO_MAIN)
    pstRecTimeCfg =
        &g_stPARAMCtx.stCfg.stMediaCfg[s32CamId].stRecCfg.record_time_cfg[0];
  else
    pstRecTimeCfg =
        &g_stPARAMCtx.stCfg.stMediaCfg[s32CamId].stRecCfg.record_time_cfg[1];

  switch (enParamType) {
  case RKADK_PARAM_TYPE_RECORD_TIME:
    if (pstRecTimeCfg->record_time == pstRecTime->time)
      return 0;

    pstRecTimeCfg->record_time = pstRecTime->time;
    break;
  case RKADK_PARAM_TYPE_SPLITTIME:
    if (pstRecTimeCfg->splite_time == pstRecTime->time)
      return 0;

    pstRecTimeCfg->splite_time = pstRecTime->time;
    break;
  case RKADK_PARAM_TYPE_LAPSE_INTERVAL:
    if (pstRecTimeCfg->lapse_interval == pstRecTime->time)
      return 0;

    pstRecTimeCfg->lapse_interval = pstRecTime->time;
    break;
  default:
    RKADK_LOGE("Invalid enParamType: %d", enParamType);
    return -1;
  }

  ret = RKADK_PARAM_SaveRecTime(g_stPARAMCtx.sensorPath[s32CamId], s32CamId,
                                pstRecTime->enStreamType);
  return ret;
}

RKADK_S32 RKADK_PARAM_GetCamParam(RKADK_S32 s32CamId,
                                  RKADK_PARAM_TYPE_E enParamType,
                                  RKADK_VOID *pvParam) {
  RKADK_CHECK_CAMERAID(s32CamId, RKADK_FAILURE);
  RKADK_CHECK_INIT(g_stPARAMCtx.bInit, RKADK_FAILURE);
  RKADK_CHECK_POINTER(pvParam, RKADK_FAILURE);

  // RKADK_LOGD("s32CamId: %d, enParamType: %d, u32_pvParam: %d, b_pvParam: %d",
  // s32CamId, enParamType, *(RKADK_U32 *)pvParam, *(bool *)pvParam);

  RKADK_MUTEX_LOCK(g_stPARAMCtx.mutexLock);

  RKADK_PARAM_REC_CFG_S *pstRecCfg =
      &g_stPARAMCtx.stCfg.stMediaCfg[s32CamId].stRecCfg;
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg =
      &g_stPARAMCtx.stCfg.stSensorCfg[s32CamId];
  RKADK_PARAM_PHOTO_CFG_S *pstPhotoCfg =
      &g_stPARAMCtx.stCfg.stMediaCfg[s32CamId].stPhotoCfg;

  switch (enParamType) {
  case RKADK_PARAM_TYPE_FPS:
    *(RKADK_U32 *)pvParam = pstSensorCfg->framerate;
    break;
  case RKADK_PARAM_TYPE_FLIP:
    *(bool *)pvParam = pstSensorCfg->flip;
    break;
  case RKADK_PARAM_TYPE_MIRROR:
    *(bool *)pvParam = pstSensorCfg->mirror;
    break;
  case RKADK_PARAM_TYPE_LDC:
    *(RKADK_U32 *)pvParam = pstSensorCfg->ldc;
    break;
  case RKADK_PARAM_TYPE_ANTIFOG:
    *(RKADK_U32 *)pvParam = pstSensorCfg->antifog;
    break;
  case RKADK_PARAM_TYPE_WDR:
    *(RKADK_U32 *)pvParam = pstSensorCfg->wdr;
    break;
  case RKADK_PARAM_TYPE_HDR:
    *(RKADK_U32 *)pvParam = pstSensorCfg->hdr;
    break;
  case RKADK_PARAM_TYPE_RES:
    *(RKADK_PARAM_RES_E *)pvParam = RKADK_PARAM_GetResType(
        pstRecCfg->attribute[0].width, pstRecCfg->attribute[0].height);
    break;
  case RKADK_PARAM_TYPE_CODEC_TYPE: {
    RKADK_PARAM_CODEC_CFG_S *pstCodecCfg;

    pstCodecCfg = (RKADK_PARAM_CODEC_CFG_S *)pvParam;
    pstCodecCfg->enCodecType =
        RKADK_PARAM_GetCodecType(s32CamId, pstCodecCfg->enStreamType);
    break;
  }
  case RKADK_PARAM_TYPE_BITRATE: {
    RKADK_PARAM_BITRATE_S *pstBitrate;

    pstBitrate = (RKADK_PARAM_BITRATE_S *)pvParam;
    pstBitrate->u32Bitrate =
        RKADK_PARAM_GetBitrate(s32CamId, pstBitrate->enStreamType);
    break;
  }
  case RKADK_PARAM_TYPE_GOP: {
    RKADK_PARAM_GOP_S *pstGop;

    pstGop = (RKADK_PARAM_GOP_S *)pvParam;
    pstGop->u32Gop = RKADK_PARAM_GetGop(s32CamId, pstGop->enStreamType);
    break;
  }
  case RKADK_PARAM_TYPE_RECORD_TIME:
  case RKADK_PARAM_TYPE_SPLITTIME:
  case RKADK_PARAM_TYPE_LAPSE_INTERVAL: {
    RKADK_PARAM_REC_TIME_S *pstRecTime;

    pstRecTime = (RKADK_PARAM_REC_TIME_S *)pvParam;
    pstRecTime->time =
        RKADK_PARAM_GetRecTime(s32CamId, pstRecTime->enStreamType, enParamType);
    break;
  }
  case RKADK_PARAM_TYPE_RECORD_TYPE:
    *(RKADK_REC_TYPE_E *)pvParam = pstRecCfg->record_type;
    break;
  case RKADK_PARAM_TYPE_FILE_CNT:
    *(RKADK_U32 *)pvParam = pstRecCfg->file_num;
    break;
  case RKADK_PARAM_TYPE_LAPSE_MULTIPLE:
    *(RKADK_U32 *)pvParam = pstRecCfg->lapse_multiple;
    break;
  case RKADK_PARAM_TYPE_PRE_RECORD_TIME:
    *(RKADK_U32 *)pvParam = pstRecCfg->pre_record_time;
    break;
  case RKADK_PARAM_TYPE_PRE_RECORD_MODE:
    *(RKADK_MUXER_PRE_RECORD_MODE_E *)pvParam = pstRecCfg->pre_record_mode;
    break;
  case RKADK_PARAM_TYPE_PHOTO_RES:
    *(RKADK_PARAM_RES_E *)pvParam = RKADK_PARAM_GetResType(
        pstPhotoCfg->image_width, pstPhotoCfg->image_height);
    break;
  case RKADK_PARAM_TYPE_SNAP_NUM:
    *(RKADK_U32 *)pvParam = pstPhotoCfg->snap_num;
    break;
  default:
    RKADK_LOGE("Unsupport enParamType(%d)", enParamType);
    RKADK_MUTEX_UNLOCK(g_stPARAMCtx.mutexLock);
    return -1;
  }

  RKADK_MUTEX_UNLOCK(g_stPARAMCtx.mutexLock);
  return 0;
}

RKADK_S32 RKADK_PARAM_SetCamParam(RKADK_S32 s32CamId,
                                  RKADK_PARAM_TYPE_E enParamType,
                                  const RKADK_VOID *pvParam) {
  RKADK_S32 ret;
  bool bSaveRecCfg = false;
  bool bSaveRecAttr = false;
  bool bSavePhotoCfg = false;
  bool bSaveSensorCfg = false;
  RKADK_STREAM_TYPE_E enStrmType = RKADK_STREAM_TYPE_BUTT;
  RKADK_PARAM_RES_E type = RKADK_RES_BUTT;

  // RKADK_LOGD("s32CamId: %d, enParamType: %d, u32_pvParam: %d, b_pvParam: %d",
  // s32CamId, enParamType, *(RKADK_U32 *)pvParam, *(bool *)pvParam);

  RKADK_CHECK_CAMERAID(s32CamId, RKADK_FAILURE);
  RKADK_CHECK_INIT(g_stPARAMCtx.bInit, RKADK_FAILURE);
  RKADK_CHECK_POINTER(pvParam, RKADK_FAILURE);

  RKADK_MUTEX_LOCK(g_stPARAMCtx.mutexLock);

  RKADK_PARAM_REC_CFG_S *pstRecCfg =
      &g_stPARAMCtx.stCfg.stMediaCfg[s32CamId].stRecCfg;
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg =
      &g_stPARAMCtx.stCfg.stSensorCfg[s32CamId];
  RKADK_PARAM_PHOTO_CFG_S *pstPhotoCfg =
      &g_stPARAMCtx.stCfg.stMediaCfg[s32CamId].stPhotoCfg;

  switch (enParamType) {
  case RKADK_PARAM_TYPE_FPS:
    RKADK_CHECK_EQUAL(pstSensorCfg->framerate, *(RKADK_U32 *)pvParam,
                      g_stPARAMCtx.mutexLock, RKADK_SUCCESS);
    pstSensorCfg->framerate = *(RKADK_U32 *)pvParam;
    bSaveSensorCfg = true;
    break;
  case RKADK_PARAM_TYPE_FLIP:
    RKADK_CHECK_EQUAL(pstSensorCfg->flip, *(bool *)pvParam,
                      g_stPARAMCtx.mutexLock, RKADK_SUCCESS);
    pstSensorCfg->flip = *(bool *)pvParam;
    bSaveSensorCfg = true;
    break;
  case RKADK_PARAM_TYPE_MIRROR:
    RKADK_CHECK_EQUAL(pstSensorCfg->mirror, *(bool *)pvParam,
                      g_stPARAMCtx.mutexLock, RKADK_SUCCESS);
    pstSensorCfg->mirror = *(bool *)pvParam;
    bSaveSensorCfg = true;
    break;
  case RKADK_PARAM_TYPE_LDC:
    RKADK_CHECK_EQUAL(pstSensorCfg->ldc, *(RKADK_U32 *)pvParam,
                      g_stPARAMCtx.mutexLock, RKADK_SUCCESS);
    pstSensorCfg->ldc = *(RKADK_U32 *)pvParam;
    bSaveSensorCfg = true;
    break;
  case RKADK_PARAM_TYPE_ANTIFOG:
    RKADK_CHECK_EQUAL(pstSensorCfg->antifog, *(RKADK_U32 *)pvParam,
                      g_stPARAMCtx.mutexLock, RKADK_SUCCESS);
    pstSensorCfg->antifog = *(RKADK_U32 *)pvParam;
    bSaveSensorCfg = true;
    break;
  case RKADK_PARAM_TYPE_WDR:
    RKADK_CHECK_EQUAL(pstSensorCfg->wdr, *(RKADK_U32 *)pvParam,
                      g_stPARAMCtx.mutexLock, RKADK_SUCCESS);
    pstSensorCfg->wdr = *(RKADK_U32 *)pvParam;
    bSaveSensorCfg = true;
    break;
  case RKADK_PARAM_TYPE_HDR:
    RKADK_CHECK_EQUAL(pstSensorCfg->hdr, *(RKADK_U32 *)pvParam,
                      g_stPARAMCtx.mutexLock, RKADK_SUCCESS);
    pstSensorCfg->hdr = *(RKADK_U32 *)pvParam;
    bSaveSensorCfg = true;
    break;
  case RKADK_PARAM_TYPE_RES:
    type = *(RKADK_PARAM_RES_E *)pvParam;
    RKADK_PARAM_GetResolution(type, &(pstRecCfg->attribute[0].width),
                              &(pstRecCfg->attribute[0].height));
    enStrmType = RKADK_STREAM_TYPE_VIDEO_MAIN;
    bSaveRecAttr = true;
    break;
  case RKADK_PARAM_TYPE_CODEC_TYPE:
    ret =
        RKADK_PARAM_SetCodecType(s32CamId, (RKADK_PARAM_CODEC_CFG_S *)pvParam);
    RKADK_MUTEX_UNLOCK(g_stPARAMCtx.mutexLock);
    return ret;
  case RKADK_PARAM_TYPE_BITRATE:
    ret = RKADK_PARAM_SetBitrate(s32CamId, (RKADK_PARAM_BITRATE_S *)pvParam);
    RKADK_MUTEX_UNLOCK(g_stPARAMCtx.mutexLock);
    return ret;
  case RKADK_PARAM_TYPE_GOP:
    ret = RKADK_PARAM_SetGop(s32CamId, (RKADK_PARAM_GOP_S *)pvParam);
    RKADK_MUTEX_UNLOCK(g_stPARAMCtx.mutexLock);
    return ret;
  case RKADK_PARAM_TYPE_RECORD_TIME:
  case RKADK_PARAM_TYPE_SPLITTIME:
  case RKADK_PARAM_TYPE_LAPSE_INTERVAL:
    ret = RKADK_PARAM_SetRecTime(s32CamId, (RKADK_PARAM_REC_TIME_S *)pvParam,
                                 enParamType);
    RKADK_MUTEX_UNLOCK(g_stPARAMCtx.mutexLock);
    return ret;
  case RKADK_PARAM_TYPE_RECORD_TYPE:
    RKADK_CHECK_EQUAL(pstRecCfg->record_type, *(RKADK_REC_TYPE_E *)pvParam,
                      g_stPARAMCtx.mutexLock, RKADK_SUCCESS);
    pstRecCfg->record_type = *(RKADK_REC_TYPE_E *)pvParam;
    bSaveRecCfg = true;
    break;
  case RKADK_PARAM_TYPE_FILE_CNT:
    RKADK_CHECK_EQUAL(pstRecCfg->file_num, *(RKADK_U32 *)pvParam,
                      g_stPARAMCtx.mutexLock, RKADK_SUCCESS);
    pstRecCfg->file_num = *(RKADK_U32 *)pvParam;
    bSaveRecCfg = true;
    break;
  case RKADK_PARAM_TYPE_LAPSE_MULTIPLE:
    RKADK_CHECK_EQUAL(pstRecCfg->lapse_multiple, *(RKADK_U32 *)pvParam,
                      g_stPARAMCtx.mutexLock, RKADK_SUCCESS);
    pstRecCfg->lapse_multiple = *(RKADK_U32 *)pvParam;
    bSaveRecCfg = true;
    break;
  case RKADK_PARAM_TYPE_PRE_RECORD_TIME:
    RKADK_CHECK_EQUAL(pstRecCfg->pre_record_time, *(RKADK_U32 *)pvParam,
                      g_stPARAMCtx.mutexLock, RKADK_SUCCESS);
    pstRecCfg->pre_record_time = *(RKADK_U32 *)pvParam;
    bSaveRecCfg = true;
    break;
  case RKADK_PARAM_TYPE_PRE_RECORD_MODE:
    RKADK_CHECK_EQUAL(pstRecCfg->pre_record_mode,
                      *(RKADK_MUXER_PRE_RECORD_MODE_E *)pvParam,
                      g_stPARAMCtx.mutexLock, RKADK_SUCCESS);
    pstRecCfg->pre_record_mode = *(RKADK_MUXER_PRE_RECORD_MODE_E *)pvParam;
    bSaveRecCfg = true;
    break;
  case RKADK_PARAM_TYPE_PHOTO_RES:
    type = *(RKADK_PARAM_RES_E *)pvParam;
    RKADK_PARAM_GetResolution(type, &(pstPhotoCfg->image_width),
                              &(pstPhotoCfg->image_height));
    bSavePhotoCfg = true;
    break;
  case RKADK_PARAM_TYPE_SNAP_NUM:
    RKADK_CHECK_EQUAL(pstPhotoCfg->snap_num, *(RKADK_U32 *)pvParam,
                      g_stPARAMCtx.mutexLock, RKADK_SUCCESS);
    pstPhotoCfg->snap_num = *(RKADK_U32 *)pvParam;
    bSavePhotoCfg = true;
    break;
  default:
    RKADK_LOGE("Unsupport enParamType(%d)", enParamType);
    RKADK_MUTEX_UNLOCK(g_stPARAMCtx.mutexLock);
    return -1;
  }

  if (bSaveSensorCfg)
    RKADK_PARAM_SaveSensorCfg(g_stPARAMCtx.sensorPath[s32CamId], s32CamId);

  if (bSaveRecCfg)
    RKADK_PARAM_SaveRecCfg(g_stPARAMCtx.sensorPath[s32CamId], s32CamId);

  if (bSaveRecAttr) {
    RKADK_PARAM_SaveRecAttr(g_stPARAMCtx.sensorPath[s32CamId], s32CamId,
                            enStrmType);
    if (pstSensorCfg->used_isp)
      RKADK_PARAM_SetRecViAttr(s32CamId);
  }

  if (bSavePhotoCfg) {
    RKADK_PARAM_SavePhotoCfg(g_stPARAMCtx.sensorPath[s32CamId], s32CamId);
    if (pstSensorCfg->used_isp)
      RKADK_PARAM_SetPhotoViAttr(s32CamId);
  }

  RKADK_MUTEX_UNLOCK(g_stPARAMCtx.mutexLock);
  return 0;
}

RKADK_S32 RKADK_PARAM_SetCommParam(RKADK_PARAM_TYPE_E enParamType,
                                   const RKADK_VOID *pvParam) {
  RKADK_CHECK_POINTER(pvParam, RKADK_FAILURE);
  RKADK_CHECK_INIT(g_stPARAMCtx.bInit, RKADK_FAILURE);

  // RKADK_LOGD("enParamType: %d, u32_pvParam: %d, b_pvParam: %d", enParamType,
  // *(RKADK_U32 *)pvParam, *(bool *)pvParam);

  RKADK_MUTEX_LOCK(g_stPARAMCtx.mutexLock);
  RKADK_PARAM_COMM_CFG_S *pstCommCfg = &g_stPARAMCtx.stCfg.stCommCfg;
  RKADK_LOGI("RKADK_PARAM_SetCommParam enParamType = %d", enParamType);
  switch (enParamType) {
  case RKADK_PARAM_TYPE_REC_MUTE:
    RKADK_CHECK_EQUAL(pstCommCfg->rec_mute, *(bool *)pvParam,
                      g_stPARAMCtx.mutexLock, RKADK_SUCCESS);

    pstCommCfg->rec_mute = *(bool *)pvParam;
    RKADK_PARAM_RecMute(pstCommCfg->rec_mute, pstCommCfg->mic_volume);
    break;
  case RKADK_PARAM_TYPE_VOLUME:
    RKADK_CHECK_EQUAL(pstCommCfg->speaker_volume, *(bool *)pvParam,
                      g_stPARAMCtx.mutexLock, RKADK_SUCCESS);

    pstCommCfg->speaker_volume = *(RKADK_U32 *)pvParam;
    RKADK_PARAM_SetSpeakerVolume(pstCommCfg->speaker_volume);
    break;
  case RKADK_PARAM_TYPE_MIC_VOLUME:
    RKADK_CHECK_EQUAL(pstCommCfg->mic_volume, *(bool *)pvParam,
                      g_stPARAMCtx.mutexLock, RKADK_SUCCESS);

    pstCommCfg->mic_volume = *(RKADK_U32 *)pvParam;
    RKADK_PARAM_SetMicVolume(pstCommCfg->mic_volume);
    break;
  default:
    RKADK_LOGE("Unsupport enParamType(%d)", enParamType);
    RKADK_MUTEX_UNLOCK(g_stPARAMCtx.mutexLock);
    return -1;
  }

  RKADK_PARAM_SaveCommCfg(g_stPARAMCtx.path);
  RKADK_MUTEX_UNLOCK(g_stPARAMCtx.mutexLock);
  return 0;
}

RKADK_S32 RKADK_PARAM_GetCommParam(RKADK_PARAM_TYPE_E enParamType,
                                   RKADK_VOID *pvParam) {
  RKADK_CHECK_POINTER(pvParam, RKADK_FAILURE);
  RKADK_CHECK_INIT(g_stPARAMCtx.bInit, RKADK_FAILURE);

  // RKADK_LOGD("enParamType: %d, u32_pvParam: %d, b_pvParam: %d", enParamType,
  // *(RKADK_U32 *)pvParam, *(bool *)pvParam);

  RKADK_MUTEX_LOCK(g_stPARAMCtx.mutexLock);
  RKADK_PARAM_COMM_CFG_S *pstCommCfg = &g_stPARAMCtx.stCfg.stCommCfg;
  switch (enParamType) {
  case RKADK_PARAM_TYPE_REC_MUTE:
    *(bool *)pvParam = pstCommCfg->rec_mute;
    break;
  case RKADK_PARAM_TYPE_VOLUME:
    *(RKADK_U32 *)pvParam = pstCommCfg->speaker_volume;
    break;
  case RKADK_PARAM_TYPE_MIC_VOLUME:
    *(RKADK_U32 *)pvParam = pstCommCfg->mic_volume;
    break;
  default:
    RKADK_LOGE("Unsupport enParamType(%d)", enParamType);
    RKADK_MUTEX_UNLOCK(g_stPARAMCtx.mutexLock);
    return -1;
  }

  RKADK_MUTEX_UNLOCK(g_stPARAMCtx.mutexLock);
  return 0;
}

RKADK_S32 RKADK_PARAM_SetDefault() {
  RKADK_S32 ret = RKADK_SUCCESS;

  RKADK_MUTEX_LOCK(g_stPARAMCtx.mutexLock);

  memset(&g_stPARAMCtx.stCfg, 0, sizeof(RKADK_PARAM_CFG_S));
  ret = RKADK_PARAM_LoadDefault();
  if (ret) {
    RKADK_LOGE("load default ini failed");
  } else {
    ret = RKADK_PARAM_SetMediaViAttr();
    if (ret)
      RKADK_LOGE("set media vi attribute failed");

    RKADK_PARAM_SetVolume();

#ifdef RKADK_DUMP_CONFIG
    RKADK_PARAM_Dump();
#endif
  }

  RKADK_MUTEX_UNLOCK(g_stPARAMCtx.mutexLock);
  return ret;
}

RKADK_VOID RKADK_PARAM_SetPath(char *path, char **sensorPath) {
  RKADK_U32 u32PathLen;
  RKADK_U32 sPathCount = 0;
  char *sPath = NULL;

  memset(g_stPARAMCtx.path, 0, RKADK_PATH_LEN);
  if (path) {
    u32PathLen = strlen(path);
    if (u32PathLen > (RKADK_PATH_LEN - 1)) {
      RKADK_LOGW("path[%s] len[%d] > %d", path, u32PathLen, RKADK_PATH_LEN - 1);
      memcpy(g_stPARAMCtx.path, RKADK_PARAM_PATH, strlen(RKADK_PARAM_PATH));
    } else {
      memcpy(g_stPARAMCtx.path, path, u32PathLen);
    }
  } else {
    memcpy(g_stPARAMCtx.path, RKADK_PARAM_PATH, strlen(RKADK_PARAM_PATH));
  }
  RKADK_LOGI("path: %s", g_stPARAMCtx.path);

  if (sensorPath) {
    while ((sPath = *sensorPath++) != NULL) {
      if (sPathCount == RKADK_MAX_SENSOR_CNT) {
        RKADK_LOGW("input sensor path count > RKADK_MAX_SENSOR_CNT[%d]",
                   RKADK_MAX_SENSOR_CNT);
        break;
      }

      u32PathLen = strlen(sPath) > (RKADK_PATH_LEN - 1) ? (RKADK_PATH_LEN - 1)
                                                        : strlen(sPath);

      memset(g_stPARAMCtx.sensorPath[sPathCount], 0, RKADK_PATH_LEN);
      memcpy(g_stPARAMCtx.sensorPath[sPathCount], sPath, u32PathLen);
      RKADK_LOGI("sensorPath[%d]: %s[%d]", sPathCount,
                 g_stPARAMCtx.sensorPath[sPathCount], u32PathLen);

      sPathCount++;
    }
  } else {
    for (int i = 0; i < RKADK_MAX_SENSOR_CNT; i++) {
      memset(g_stPARAMCtx.sensorPath[i], 0, RKADK_PATH_LEN);
      sprintf(g_stPARAMCtx.sensorPath[i], "%s_%d.ini",
              RKADK_PARAM_PATH_SENSOR_PREFIX, i);

      RKADK_LOGI("sensorPath[%d]: %s", i, g_stPARAMCtx.sensorPath[i]);
    }
  }
}

RKADK_S32 RKADK_PARAM_Init(char *globalSetting, char **sesnorSettingArrary) {
  RKADK_S32 ret = RKADK_SUCCESS;

  /* Check Module Init Status */
  if (g_stPARAMCtx.bInit)
    return RKADK_SUCCESS;

  RKADK_MUTEX_LOCK(g_stPARAMCtx.mutexLock);

  RKADK_PARAM_SetPath(globalSetting, sesnorSettingArrary);

  memset(&g_stPARAMCtx.stCfg, 0, sizeof(RKADK_PARAM_CFG_S));

  ret = RKADK_PARAM_LoadParam(g_stPARAMCtx.path, g_stPARAMCtx.sensorPath);
  if (ret) {
    RKADK_LOGE("load setting ini failed, load default ini");
    ret = RKADK_PARAM_LoadDefault();
    if (ret) {
      RKADK_LOGE("load default ini failed, use default param");
      RKADK_PARAM_UseDefault();
    }
  }

  ret = RKADK_PARAM_SetMediaViAttr();
  if (ret) {
    RKADK_LOGE("set media vi attribute failed");
    goto end;
  }

  RKADK_PARAM_SetVolume();

#ifdef RKADK_DUMP_CONFIG
  RKADK_PARAM_Dump();
#endif

  g_stPARAMCtx.bInit = true;

end:
  RKADK_MUTEX_UNLOCK(g_stPARAMCtx.mutexLock);
  return ret;
}

RKADK_S32 RKADK_PARAM_Deinit() {
  // reserved
  return 0;
}

RKADK_STREAM_TYPE_E RKADK_PARAM_VencChnMux(RKADK_U32 u32CamId,
                                           RKADK_U32 u32ChnId) {
  int i;

  RKADK_CHECK_CAMERAID(u32CamId, RKADK_STREAM_TYPE_BUTT);

  RKADK_PARAM_REC_CFG_S *pstRecCfg = RKADK_PARAM_GetRecCfg(u32CamId);
  if (pstRecCfg) {
    for (i = 0; i < (int)pstRecCfg->file_num; i++) {
      if (u32ChnId == pstRecCfg->attribute[i].venc_chn) {
        if (i == 0)
          return RKADK_STREAM_TYPE_VIDEO_MAIN;
        else
          return RKADK_STREAM_TYPE_VIDEO_SUB;
      }
    }
  }

  RKADK_PARAM_STREAM_CFG_S *pstStreamCfg =
      RKADK_PARAM_GetStreamCfg(u32CamId, RKADK_STREAM_TYPE_PREVIEW);
  if (pstStreamCfg && (u32ChnId == pstStreamCfg->attribute.venc_chn))
    return RKADK_STREAM_TYPE_PREVIEW;

  RKADK_PARAM_STREAM_CFG_S *pstLiveCfg =
      RKADK_PARAM_GetStreamCfg(u32CamId, RKADK_STREAM_TYPE_LIVE);
  if (pstLiveCfg && (u32ChnId == pstLiveCfg->attribute.venc_chn))
    return RKADK_STREAM_TYPE_LIVE;

  return RKADK_STREAM_TYPE_BUTT;
}

RKADK_U32 RKADK_PARAM_GetStreamBufCnt(RKADK_U32 u32CamId, bool bIsAudio) {
  RKADK_U32 u32Integer = 0, u32Remainder = 0;
  RKADK_U32 u32PreRecCacheTime = 0;
  RKADK_U32 u32BufCount = RKADK_MUXER_CELL_MAX_CNT;
  RKADK_PARAM_AUDIO_CFG_S *pstAudioParam = NULL;
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg = NULL;
  RKADK_PARAM_REC_CFG_S *pstRecCfg = NULL;

  pstAudioParam = RKADK_PARAM_GetAudioCfg();
  if (!pstAudioParam) {
    RKADK_LOGE("RKADK_PARAM_GetAudioCfg failed");
    return u32BufCount;
  }

  pstRecCfg = RKADK_PARAM_GetRecCfg(u32CamId);
  if (!pstRecCfg) {
    RKADK_LOGE("RKADK_PARAM_GetRecCfg failed");
    return u32BufCount;
  }

  pstSensorCfg = RKADK_PARAM_GetSensorCfg(u32CamId);
  if (!pstSensorCfg) {
    RKADK_LOGE("RKADK_PARAM_GetSensorCfg failed");
    return u32BufCount;
  }

  if(pstRecCfg->pre_record_time == 0)
    return u32BufCount;

  u32Integer = pstRecCfg->attribute[0].gop / pstSensorCfg->framerate;
  u32Remainder = pstRecCfg->attribute[0].gop % pstSensorCfg->framerate;
  u32PreRecCacheTime = pstRecCfg->pre_record_time + u32Integer;
  if (u32Remainder)
    u32PreRecCacheTime += 1;

  if (bIsAudio)
    u32BufCount = pstAudioParam->channels * (pstAudioParam->samplerate
                  / pstAudioParam->samples_per_frame) * (u32PreRecCacheTime + 2);
  else
    u32BufCount = (u32PreRecCacheTime + 2) * pstSensorCfg->framerate;

  return u32BufCount;
}
