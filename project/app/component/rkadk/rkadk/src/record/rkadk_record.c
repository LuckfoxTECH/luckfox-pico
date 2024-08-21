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
#include "linux_list.h"
#include "file_cache.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  struct list_head mark;
  char filename[RKADK_MAX_FILE_PATH_LEN];
} FILE_NAME_CELL_S;

static RKADK_REC_REQUEST_FILE_NAMES_FN
    g_pfnRequestFileNames[RKADK_MAX_SENSOR_CNT] = {NULL};
static struct list_head
    g_fileNameList[RKADK_MUXER_STREAM_MAX_CNT * RKADK_MAX_SENSOR_CNT];
static pthread_mutex_t g_fileNameMutexLock = PTHREAD_MUTEX_INITIALIZER;

static int GetFileNameListSize(struct list_head *head) {
  int size = 0;
  FILE_NAME_CELL_S *cell = NULL;

  list_for_each_entry(cell, head, mark) {
    size++;
  }

  return size;
}

static void FileNameListInit(RKADK_MUXER_HANDLE_S *stRecorder) {
  int index;

  for (int i = 0; i < (int)stRecorder->u32StreamCnt; i++) {
    index = i + (RKADK_MUXER_STREAM_MAX_CNT * stRecorder->u32CamId);
    INIT_LIST_HEAD(&g_fileNameList[index]);
  }
}

static FILE_NAME_CELL_S *FileNameCellPop(struct list_head *head) {
  FILE_NAME_CELL_S *rst = NULL;
  FILE_NAME_CELL_S *cell = NULL;
  FILE_NAME_CELL_S *cell_n = NULL;

  do {
    list_for_each_entry_safe(cell, cell_n, head, mark) {
      list_del_init(&cell->mark);
      rst = cell;
      break;
    }
  } while (0);
  return rst;
}

static void FileNameListRelease(RKADK_MUXER_HANDLE_S *stRecorder) {
  int index;
  FILE_NAME_CELL_S *cell = NULL;
  FILE_NAME_CELL_S *cell_n = NULL;

  for (int i = 0; i < (int)stRecorder->u32StreamCnt; i++) {
    index = i + (RKADK_MUXER_STREAM_MAX_CNT * stRecorder->u32CamId);

    list_for_each_entry_safe(cell, cell_n, &g_fileNameList[index], mark) {
      list_del_init(&cell->mark);
      free(cell);
    }
  }
}

static int GetRecordFileName(RKADK_VOID *pHandle, RKADK_CHAR *pcFileName,
                             RKADK_U32 muxerId) {
  int index, ret, size = 0;
  int len = 0;
  ARRAY_FILE_NAME fileName = NULL;
  FILE_NAME_CELL_S *cell = NULL;
  FILE_NAME_CELL_S *rst = NULL;
  RKADK_MUXER_HANDLE_S *pstRecorder;

  RKADK_MUTEX_LOCK(g_fileNameMutexLock);

  if (muxerId >= RKADK_MUXER_STREAM_MAX_CNT * RKADK_MAX_SENSOR_CNT) {
    RKADK_LOGE("Incorrect file index: %d", muxerId);
    goto failed;
  }

  pstRecorder = (RKADK_MUXER_HANDLE_S *)pHandle;
  if (!pstRecorder) {
    RKADK_LOGE("pstRecorder is null");
    goto failed;
  }

  if (!g_pfnRequestFileNames[pstRecorder->u32CamId]) {
    RKADK_LOGE("Not Registered request name callback");
    goto failed;
  }

  size = GetFileNameListSize(&g_fileNameList[muxerId]);
  if (!size) {
    fileName = (ARRAY_FILE_NAME)malloc(pstRecorder->u32StreamCnt *
                                       RKADK_MAX_FILE_PATH_LEN);
    if (!fileName) {
      RKADK_LOGE("Stream[%d]: malloc fileName failed", muxerId);
      goto failed;
    }

    ret = g_pfnRequestFileNames[pstRecorder->u32CamId](
        pHandle, pstRecorder->u32StreamCnt, fileName);
    if (ret) {
      RKADK_LOGE("Stream[%d]: get file name failed(%d)", muxerId, ret);
      goto failed;
    }

    for (int i = 0; i < (int)pstRecorder->u32StreamCnt; i++) {
      cell = (FILE_NAME_CELL_S *)malloc(sizeof(FILE_NAME_CELL_S));
      if (NULL == cell) {
        RKADK_LOGE("Stream[%d]: malloc file name cell failed", muxerId);
        goto failed;
      }
      memset(cell->filename, 0, RKADK_MAX_FILE_PATH_LEN);
      INIT_LIST_HEAD(&cell->mark);

      len = strlen(fileName[i]) > RKADK_MAX_FILE_PATH_LEN ? RKADK_MAX_FILE_PATH_LEN : strlen(fileName[i]);
      memcpy(cell->filename, fileName[i], len);

      index = i + (RKADK_MUXER_STREAM_MAX_CNT * pstRecorder->u32CamId);
      list_add_tail(&cell->mark, &g_fileNameList[index]);
    }
    free(fileName);
  }

  rst = FileNameCellPop(&g_fileNameList[muxerId]);
  if (!rst) {
    RKADK_LOGE("Stream[%d]: cell pop failed", muxerId);
    goto failed;
  }
  strncpy(pcFileName, rst->filename, RKADK_MAX_FILE_PATH_LEN);
  free(rst);

  RKADK_MUTEX_UNLOCK(g_fileNameMutexLock);
  return 0;

failed:
  if (fileName)
    free(fileName);

  RKADK_MUTEX_UNLOCK(g_fileNameMutexLock);
  return -1;
}

static RKADK_U32 GetPreRecordCacheTime(RKADK_PARAM_REC_CFG_S *pstRecCfg,
                                        RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg) {
  RKADK_U32 u32Integer = 0, u32Remainder = 0;
  RKADK_U32 u32PreRecCacheTime = 0;

  if(pstRecCfg->pre_record_time == 0)
    return 0;

  u32Integer = pstRecCfg->attribute[0].gop / pstRecCfg->attribute[0].framerate;
  u32Remainder = pstRecCfg->attribute[0].gop % pstRecCfg->attribute[0].framerate;
  u32PreRecCacheTime = pstRecCfg->pre_record_time + u32Integer;
  if (u32Remainder)
    u32PreRecCacheTime += 1;

  return u32PreRecCacheTime;
}

static void RKADK_RECORD_SetVideoChn(int index, RKADK_U32 u32CamId,
                                     RKADK_PARAM_REC_CFG_S *pstRecCfg,
                                     MPP_CHN_S *pstViChn, MPP_CHN_S *pstVencChn,
                                     MPP_CHN_S *pstSrcVpssChn, MPP_CHN_S *pstDstVpssChn,
                                     RKADK_MW_PTR pRecorder) {

  pstVencChn->enModId = RK_ID_VENC;
  pstVencChn->s32DevId = 0;
  pstVencChn->s32ChnId = pstRecCfg->attribute[index].venc_chn;

  pstViChn->enModId = RK_ID_VI;
  pstViChn->s32DevId = u32CamId;
  if (pRecorder)
    pstViChn->s32ChnId = RKADK_MUXER_GetViChn(pRecorder, pstVencChn->s32ChnId);
  if (pstViChn->s32ChnId == -1)
    pstViChn->s32ChnId = pstRecCfg->vi_attr[index].u32ViChn;

  pstSrcVpssChn->enModId = RK_ID_VPSS;
  pstSrcVpssChn->s32DevId = pstRecCfg->attribute[index].vpss_grp;
  pstSrcVpssChn->s32ChnId = pstRecCfg->attribute[index].vpss_chn;

  pstDstVpssChn->enModId = RK_ID_VPSS;
  pstDstVpssChn->s32DevId = pstRecCfg->attribute[index].vpss_grp;
  pstDstVpssChn->s32ChnId = 0; //When vpss is dst, chn is equal to 0
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
  RKADK_PARAM_FPS_S stFps;

  RKADK_CHECK_POINTER(pstVencAttr, RKADK_FAILURE);

  pstSensorCfg = RKADK_PARAM_GetSensorCfg(u32CamId);
  if (!pstSensorCfg) {
    RKADK_LOGE("RKADK_PARAM_GetSensorCfg failed");
    return -1;
  }

  memset(pstVencAttr, 0, sizeof(VENC_CHN_ATTR_S));

  if(pstRecCfg->record_type == RKADK_REC_TYPE_LAPSE) {
    u32DstFrameRateNum = pstRecCfg->attribute[index].framerate / pstRecCfg->lapse_multiple;
    if (u32DstFrameRateNum < 1)
      u32DstFrameRateNum = 1;
    else if (u32DstFrameRateNum > pstRecCfg->attribute[index].framerate)
      u32DstFrameRateNum = pstRecCfg->attribute[index].framerate;
  } else {
    u32DstFrameRateNum = pstRecCfg->attribute[index].framerate;
  }
  bitrate = pstRecCfg->attribute[index].bitrate;
  u32Gop = pstRecCfg->attribute[index].gop;

  if (u32DstFrameRateNum > pstSensorCfg->framerate) {
    RKADK_LOGW("CamId[%d] record[%d] fps[%d] > sensor fps[%d], use sensor fps",
                u32CamId, index, u32DstFrameRateNum, pstSensorCfg->framerate);
    u32DstFrameRateNum = pstSensorCfg->framerate;

    if (index == 0)
      stFps.enStreamType = RKADK_STREAM_TYPE_VIDEO_MAIN;
    else
      stFps.enStreamType = RKADK_STREAM_TYPE_VIDEO_SUB;
    stFps.u32Framerate = pstSensorCfg->framerate;
    RKADK_PARAM_SetCamParam(u32CamId, RKADK_PARAM_TYPE_FPS, &stFps);
  }

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

static bool RKADK_RECORD_IsUseVpss(RKADK_U32 u32CamId, int index,
                                RKADK_PARAM_REC_CFG_S *pstRecCfg) {
  RKADK_U32 u32SrcWidth, u32SrcHeight;
  RKADK_U32 u32DstWidth, u32DstHeight;
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg;

  pstSensorCfg = RKADK_PARAM_GetSensorCfg(u32CamId);
  if (!pstSensorCfg) {
    RKADK_LOGE("RKADK_PARAM_GetSensorCfg failed");
    return false;
  }

  u32SrcWidth= pstRecCfg->vi_attr[index].stChnAttr.stSize.u32Width;
  u32SrcHeight = pstRecCfg->vi_attr[index].stChnAttr.stSize.u32Height;
  u32DstWidth = pstRecCfg->attribute[index].width;
  u32DstHeight = pstRecCfg->attribute[index].height;
  if (u32DstWidth != u32SrcWidth || u32DstHeight != u32SrcHeight) {
    RKADK_LOGD("In[%d, %d], Out[%d, %d]", u32SrcWidth, u32SrcHeight,
               u32DstWidth, u32DstHeight);
    return true;
  }

#ifdef RV1106_1103
  //main record use vpss switch resolution
  if (!pstSensorCfg->used_isp) {
    if (pstRecCfg->switch_res && index == 0)
      return true;
  }
#endif

  if (pstRecCfg->attribute[index].post_aiisp)
    return true;

  return false;
}

static int RKADK_RECORD_CreateVideoChn(RKADK_RECORD_ATTR_S *pstRecAttr) {
  int ret;
  RKADK_U32 u32VpssGrp, u32VpssChn;
  VENC_CHN_ATTR_S stVencChnAttr;
  RKADK_PARAM_REC_CFG_S *pstRecCfg = NULL;
  RKADK_PARAM_THUMB_CFG_S *ptsThumbCfg = NULL;
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg = NULL;
  RKADK_PARAM_COMM_CFG_S *pstCommCfg = NULL;
  bool bUseVpss = false;
  VPSS_GRP_ATTR_S stGrpAttr;
  VPSS_CHN_ATTR_S stChnAttr;
  RKADK_U32 u32ThumbVencChn;
  RKADK_THUMB_MODULE_E enThumbModule = RKADK_THUMB_MODULE_BUTT;
  RKADK_STREAM_TYPE_E enStrmType;

  pstRecCfg = RKADK_PARAM_GetRecCfg(pstRecAttr->s32CamID);
  if (!pstRecCfg) {
    RKADK_LOGE("RKADK_PARAM_GetRecCfg failed");
    return -1;
  }

  ptsThumbCfg = RKADK_PARAM_GetThumbCfg(pstRecAttr->s32CamID);
  if (!ptsThumbCfg) {
    RKADK_LOGE("RKADK_PARAM_GetThumbCfg failed");
    return -1;
  }

  pstCommCfg = RKADK_PARAM_GetCommCfg();
  if (!pstCommCfg) {
    RKADK_LOGE("RKADK_PARAM_GetCommCfg failed");
    return -1;
  }

  pstSensorCfg = RKADK_PARAM_GetSensorCfg(pstRecAttr->s32CamID);
  if (!pstSensorCfg) {
    RKADK_LOGE("RKADK_PARAM_GetSensorCfg failed");
    return -1;
  }

  for (int i = 0; i < (int)pstRecCfg->file_num; i++) {
    u32VpssGrp = pstRecCfg->attribute[i].vpss_grp;
    u32VpssChn = pstRecCfg->attribute[i].vpss_chn;

    ret = RKADK_RECORD_SetVideoAttr(i, pstRecAttr->s32CamID, pstRecCfg, &stVencChnAttr);
    if (ret) {
      RKADK_LOGE("RKADK_RECORD_SetVideoAttr(%d) failed", i);
      return ret;
    }

    // Create VI
    ret = RKADK_MPI_VI_Init(pstRecAttr->s32CamID, pstRecCfg->vi_attr[i].u32ViChn,
                            &(pstRecCfg->vi_attr[i].stChnAttr));
    if (ret) {
      RKADK_LOGE("RKADK_MPI_VI_Init faile, ret = %d", ret);
      return ret;
    }
    RKADK_BUFINFO("create vi[%d]", pstRecCfg->vi_attr[i].u32ViChn);

    // Create VPSS
    bUseVpss = RKADK_RECORD_IsUseVpss(pstRecAttr->s32CamID, i, pstRecCfg);
    if (bUseVpss) {
      memset(&stGrpAttr, 0, sizeof(VPSS_GRP_ATTR_S));
      memset(&stChnAttr, 0, sizeof(VPSS_CHN_ATTR_S));

      stGrpAttr.u32MaxW = pstSensorCfg->max_width;
      stGrpAttr.u32MaxH = pstSensorCfg->max_height;
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
      stChnAttr.u32Depth = 0;
      stChnAttr.u32FrameBufCnt = pstRecCfg->vi_attr[i].stChnAttr.stIspOpt.u32BufCount + 2;

      if (i == 0) {
        stChnAttr.u32Width = pstSensorCfg->max_width;
        stChnAttr.u32Height = pstSensorCfg->max_height;
      } else {
        stChnAttr.u32Width = pstRecCfg->attribute[i].width;
        stChnAttr.u32Height = pstRecCfg->attribute[i].height;
      }

      ret = RKADK_MPI_VPSS_Init(u32VpssGrp, u32VpssChn, &stGrpAttr, &stChnAttr);
      if (ret) {
        RKADK_LOGE("RKADK_MPI_VPSS_Init vpss falied[%d]",ret);
        RKADK_MPI_VI_DeInit(pstRecAttr->s32CamID, pstRecCfg->vi_attr[i].u32ViChn);
        RKADK_MPI_VPSS_DeInit(u32VpssGrp, u32VpssChn);
        return ret;
      }
      RKADK_BUFINFO("create vpss[%d, %d]", u32VpssGrp, u32VpssChn);

      if (i == 0) {
        ret = RK_MPI_VPSS_GetChnAttr(u32VpssGrp, u32VpssChn, &stChnAttr);
        if (ret) {
          RKADK_LOGE("RK_MPI_VPSS_GetChnAttr vpss_grp[%d] vpss_chn[%d] falied[%x]",
                      u32VpssGrp, u32VpssChn, ret);
          RKADK_MPI_VI_DeInit(pstRecAttr->s32CamID, pstRecCfg->vi_attr[i].u32ViChn);
          RKADK_MPI_VPSS_DeInit(u32VpssGrp, u32VpssChn);
          return ret;
        }

        stChnAttr.u32Width = pstRecCfg->attribute[i].width;
        stChnAttr.u32Height = pstRecCfg->attribute[i].height;
        ret = RK_MPI_VPSS_SetChnAttr(u32VpssGrp, u32VpssChn, &stChnAttr);
        if (ret) {
          RKADK_LOGE("RK_MPI_VPSS_SetChnAttr vpss_grp[%d] vpss_chn[%d] falied[%x]",
                      u32VpssGrp, u32VpssChn, ret);
          RKADK_MPI_VI_DeInit(pstRecAttr->s32CamID, pstRecCfg->vi_attr[i].u32ViChn);
          RKADK_MPI_VPSS_DeInit(u32VpssGrp, u32VpssChn);
          return ret;
        }
      }

      if (pstRecCfg->attribute[i].post_aiisp) {
        if (i == 0)
          enStrmType = RKADK_STREAM_TYPE_VIDEO_MAIN;
        else
          enStrmType = RKADK_STREAM_TYPE_VIDEO_SUB;
        RKADK_MEDIA_EnablePostIsp(pstRecAttr->s32CamID, enStrmType, pstRecAttr->pstPostIspAttr);
      }
    }

    // Create VENC
    ret = RKADK_MPI_VENC_Init(pstRecAttr->s32CamID, pstRecCfg->attribute[i].venc_chn, &stVencChnAttr);
    if (ret) {
      RKADK_LOGE("RKADK_MPI_VENC_Init failed(%d)", ret);

      if (bUseVpss)
        RKADK_MPI_VPSS_DeInit(u32VpssGrp, u32VpssChn);

      RKADK_MPI_VI_DeInit(pstRecAttr->s32CamID, pstRecCfg->vi_attr[i].u32ViChn);
      return ret;
    }
    RKADK_BUFINFO("create venc[%d]", pstRecCfg->attribute[i].venc_chn);

    RKADK_PARAM_SetVAdvancedParam(pstRecCfg->attribute[i]);
    if (pstRecCfg->record_type != RKADK_REC_TYPE_AOV_LAPSE)
      RK_MPI_VENC_SetSceneMode(pstRecCfg->attribute[i].venc_chn, RKADK_ENCODE_SENSE_CVR);

    if (i == 0) {
      enThumbModule = RKADK_THUMB_MODULE_MAIN_RECORD;
      u32ThumbVencChn = ptsThumbCfg->record_main_venc_chn;
    }
    else {
      enThumbModule = RKADK_THUMB_MODULE_SUB_RECORD;
      u32ThumbVencChn = ptsThumbCfg->record_sub_venc_chn;
    }

    ThumbnailInit(pstRecAttr->s32CamID, enThumbModule, ptsThumbCfg);
#ifndef THUMB_NORMAL
    ThumbnailChnBind(pstRecCfg->attribute[i].venc_chn, u32ThumbVencChn);
#endif

    //if use isp, set mirror/flip using aiq
    if (!pstSensorCfg->used_isp) {
      enStrmType = RKADK_STREAM_TYPE_VIDEO_MAIN;
      if (i != 0)
        enStrmType = RKADK_STREAM_TYPE_VIDEO_SUB;

      if (pstSensorCfg->mirror)
        RKADK_MEDIA_ToggleVencMirror(pstRecAttr->s32CamID, enStrmType, pstSensorCfg->mirror);
      if (pstSensorCfg->flip)
        RKADK_MEDIA_ToggleVencFlip(pstRecAttr->s32CamID, enStrmType, pstSensorCfg->flip);
    }
  }

  return 0;
}

static int RKADK_RECORD_DestoryVideoChn(RKADK_U32 u32CamId, RKADK_MW_PTR pRecorder) {
  int ret;
  bool bUseVpss = false;
  RKADK_PARAM_REC_CFG_S *pstRecCfg = NULL;
  RKADK_PARAM_THUMB_CFG_S *ptsThumbCfg = NULL;
  RKADK_PARAM_COMM_CFG_S *pstCommCfg = NULL;
  RKADK_THUMB_MODULE_E enThumbModule = RKADK_THUMB_MODULE_BUTT;
  int viChn = -1;

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

    if (pRecorder)
      bUseVpss = RKADK_MUXER_IsUseVpss(pRecorder, pstRecCfg->attribute[i].venc_chn);
    else
      bUseVpss = RKADK_RECORD_IsUseVpss(u32CamId, i, pstRecCfg);
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

    // Destory VI
    if (pRecorder)
      viChn = RKADK_MUXER_GetViChn(pRecorder, pstRecCfg->attribute[i].venc_chn);
    if(viChn == -1)
      viChn = pstRecCfg->vi_attr[i].u32ViChn;
    ret = RKADK_MPI_VI_DeInit(u32CamId, viChn);
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
  memcpy(stAiAttr.u8CardName, pstAudioParam->ai_audio_node,
         strlen(pstAudioParam->ai_audio_node));
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
                          pstAudioParam->vqe_config_path, pstAudioParam->mic_type);
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

  RKADK_MUXER_WriteAudioFrame(stFrame.pMbBlk, stFrame.u32Len, stFrame.u64TimeStamp, pHandle);
  pstMuxer->u64AudioPts += duration;
}

static void RKADK_RECORD_VencOutCb(RKADK_MEDIA_VENC_DATA_S stData,
                                   RKADK_VOID *pHandle) {
  RKADK_CHECK_POINTER_N(pHandle);
  RKADK_MUXER_WriteVideoFrame(stData, pHandle);
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

  if (pstRecCfg->record_type == RKADK_REC_TYPE_NORMAL &&
      RKADK_MUXER_EnableAudio(u32CamId)) {
    RKADK_RECORD_SetAudioChn(&stSrcChn, &stDestChn);

    // Get aenc data
    ret = RKADK_MEDIA_GetAencBuffer(&stDestChn, RKADK_RECORD_AencOutCb, pRecorder);
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
                             &stSrcVpssChn, &stDstVpssChn, pRecorder);

    // Get venc data
    if (RKADK_RECORD_VencGetData(u32CamId, &stDestChn, pRecorder))
      return -1;

    bUseVpss = RKADK_RECORD_IsUseVpss(u32CamId, i, pstRecCfg);
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
    RKADK_BUFINFO("record bind[%d, %d, %d]", stSrcChn.s32ChnId, stSrcVpssChn.s32ChnId, stDestChn.s32ChnId);
  }

  return 0;
}

static int RKADK_RECORD_UnBindChn(RKADK_U32 u32CamId,
                                  RKADK_MUXER_HANDLE_S *pstRecorder) {
  int ret;
  MPP_CHN_S stSrcChn, stDestChn, stSrcVpssChn, pstDstVpssChn;
  RKADK_PARAM_REC_CFG_S *pstRecCfg = NULL;
  bool bUseVpss = false;

  pstRecCfg = RKADK_PARAM_GetRecCfg(u32CamId);
  if (!pstRecCfg) {
    RKADK_LOGE("RKADK_PARAM_GetRecCfg failed");
    return -1;
  }

  if (pstRecorder->enRecType == RKADK_REC_TYPE_NORMAL &&
      RKADK_MUXER_EnableAudio(u32CamId)) {
    RKADK_RECORD_SetAudioChn(&stSrcChn, &stDestChn);

    // Stop get aenc data
    RKADK_MEDIA_StopGetAencBuffer(&stDestChn, RKADK_RECORD_AencOutCb, pstRecorder);

    // UnBind AI to AENC
    ret = RKADK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (ret) {
      RKADK_LOGE("RKADK_MPI_SYS_UnBind failed(%d)", ret);
      return ret;
    }
  }

  for (int i = 0; i < (int)pstRecCfg->file_num; i++) {
    RKADK_RECORD_SetVideoChn(i, u32CamId, pstRecCfg, &stSrcChn, &stDestChn,
                             &stSrcVpssChn, &pstDstVpssChn, (RKADK_MW_PTR)pstRecorder);

    // Stop get venc data
    RKADK_MEDIA_StopGetVencBuffer(&stDestChn, RKADK_RECORD_VencOutCb, pstRecorder);

    bUseVpss = RKADK_MUXER_IsUseVpss((RKADK_MW_PTR)pstRecorder, stDestChn.s32ChnId);
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
  int ret;
  bool bEnableAudio = false;
  ROTATION_E enRotation = ROTATION_0;
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

  pstMuxerAttr->enRecType = pstRecCfg->record_type;
  pstMuxerAttr->u32CamId = u32CamId;
  pstMuxerAttr->u32StreamCnt = pstRecCfg->file_num;
  pstMuxerAttr->stPreRecordAttr.u32PreRecTimeSec = pstRecCfg->pre_record_time;
  pstMuxerAttr->stPreRecordAttr.enPreRecordMode = pstRecCfg->pre_record_mode;
  pstMuxerAttr->stPreRecordAttr.u32PreRecCacheTime = GetPreRecordCacheTime(pstRecCfg, pstSensorCfg);
  pstMuxerAttr->pcbRequestFileNames = GetRecordFileName;

  for (int i = 0; i < (int)pstMuxerAttr->u32StreamCnt; i++) {
    ret = RK_MPI_VENC_GetChnRotation(pstRecCfg->attribute[i].venc_chn, &enRotation);
    if (ret != RK_SUCCESS)
      RKADK_LOGE("Get venc[%d] rotation failed[%x]", pstRecCfg->attribute[i].venc_chn, ret);

    pstMuxerAttr->astStreamAttr[i].u32ViChn = pstRecCfg->vi_attr[i].u32ViChn;
    pstMuxerAttr->astStreamAttr[i].u32VencChn = pstRecCfg->attribute[i].venc_chn;
    pstMuxerAttr->astStreamAttr[i].bUseVpss = RKADK_RECORD_IsUseVpss(u32CamId, i, pstRecCfg);
    pstMuxerAttr->astStreamAttr[i].enType = pstRecCfg->file_type;
    if (pstRecCfg->record_type != RKADK_REC_TYPE_NORMAL) {
      pstMuxerAttr->astStreamAttr[i].u32TimeLenSec = pstRecCfg->record_time_cfg[i].lapse_interval;
      pstMuxerAttr->astStreamAttr[i].u32TrackCnt = 1; // only video track
    } else {
      pstMuxerAttr->astStreamAttr[i].u32TimeLenSec = pstRecCfg->record_time_cfg[i].record_time;
      pstMuxerAttr->astStreamAttr[i].u32TrackCnt = RKADK_MUXER_TRACK_MAX_CNT;
    }

    if (pstMuxerAttr->astStreamAttr[i].u32TrackCnt == RKADK_MUXER_TRACK_MAX_CNT)
      if (!bEnableAudio)
        pstMuxerAttr->astStreamAttr[i].u32TrackCnt = 1;

    // video track
    RKADK_MUXER_TRACK_SOURCE_S *aHTrackSrcHandle =
        &(pstMuxerAttr->astStreamAttr[i].aHTrackSrcHandle[0]);
    aHTrackSrcHandle->enTrackType = RKADK_TRACK_SOURCE_TYPE_VIDEO;
    aHTrackSrcHandle->unTrackSourceAttr.stVideoInfo.enCodecType =
        pstRecCfg->attribute[i].codec_type;
    if (RKADK_MEDIA_GetPixelFormat(
            pstRecCfg->vi_attr[i].stChnAttr.enPixelFormat,
            aHTrackSrcHandle->unTrackSourceAttr.stVideoInfo.cPixFmt))
      return -1;
    aHTrackSrcHandle->unTrackSourceAttr.stVideoInfo.u32FrameRate =
        pstRecCfg->attribute[i].framerate;
    aHTrackSrcHandle->unTrackSourceAttr.stVideoInfo.u16Level = 41;
    aHTrackSrcHandle->unTrackSourceAttr.stVideoInfo.u16Profile =
        pstRecCfg->attribute[i].profile;
    aHTrackSrcHandle->unTrackSourceAttr.stVideoInfo.u32BitRate =
        pstRecCfg->attribute[i].bitrate;
    aHTrackSrcHandle->unTrackSourceAttr.stVideoInfo.u32Gop =
        pstRecCfg->attribute[i].gop;

    if (enRotation == ROTATION_90 || enRotation == ROTATION_270) {
      aHTrackSrcHandle->unTrackSourceAttr.stVideoInfo.u32Width =
          pstRecCfg->attribute[i].height;
      aHTrackSrcHandle->unTrackSourceAttr.stVideoInfo.u32Height =
          pstRecCfg->attribute[i].width;
    } else {
      aHTrackSrcHandle->unTrackSourceAttr.stVideoInfo.u32Width =
          pstRecCfg->attribute[i].width;
      aHTrackSrcHandle->unTrackSourceAttr.stVideoInfo.u32Height =
          pstRecCfg->attribute[i].height;
    }

    if (pstRecCfg->record_type != RKADK_REC_TYPE_NORMAL || !bEnableAudio)
      continue;

    // audio track
    aHTrackSrcHandle = &(pstMuxerAttr->astStreamAttr[i].aHTrackSrcHandle[1]);
    aHTrackSrcHandle->enTrackType = RKADK_TRACK_SOURCE_TYPE_AUDIO;
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
                                       bool bUseVpss, RKADK_MW_PTR pRecorder) {
  pstRecVenChn->enModId = RK_ID_VENC;
  pstRecVenChn->s32DevId = 0;
  pstRecVenChn->s32ChnId = pstRecCfg->attribute[index].venc_chn;

  if (!bUseVpss) {
    pstSrcChn->enModId = RK_ID_VI;
    pstSrcChn->s32DevId = u32CamId;

    if (pRecorder)
      pstSrcChn->s32ChnId = RKADK_MUXER_GetViChn(pRecorder, pstRecVenChn->s32ChnId);

    if (pstSrcChn->s32ChnId == -1)
      pstSrcChn->s32ChnId = pstRecCfg->vi_attr[index].u32ViChn;
  } else {
    pstSrcChn->enModId = RK_ID_VPSS;
    pstSrcChn->s32DevId = pstRecCfg->attribute[index].vpss_grp;
    pstSrcChn->s32ChnId = pstRecCfg->attribute[index].vpss_chn;
  }
}

static bool RKADK_RECORD_ResetVideoAttr(RKADK_U32 u32CamId, RKADK_U32 index,
                                        RKADK_PARAM_REC_CFG_S *pstRecCfg,
                                        RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg,
                                        VENC_CHN_ATTR_S *pstVencAttr,
                                        VI_CHN_ATTR_S *pstViAttr,
                                        VPSS_CHN_ATTR_S *pstVpssAttr) {
  bool bReset = false;
  RKADK_U32 u32DstFrameRateNum;
  RKADK_U32 bitrate;
  RKADK_U32 u32Gop;
  RKADK_U32 u32Width;
  RKADK_U32 u32Height;
  RK_CODEC_ID_E enType;
  RKADK_PARAM_FPS_S stFps;

  bitrate = pstRecCfg->attribute[index].bitrate;
  u32Gop = pstRecCfg->attribute[index].gop;
  u32DstFrameRateNum = pstRecCfg->attribute[index].framerate;
  u32Width = pstRecCfg->attribute[index].width;
  u32Height = pstRecCfg->attribute[index].height;

  if (pstRecCfg->record_type == RKADK_REC_TYPE_LAPSE) {
    u32DstFrameRateNum = pstRecCfg->attribute[index].framerate / pstRecCfg->lapse_multiple;
    if (u32DstFrameRateNum < 1)
      u32DstFrameRateNum = 1;
    else if (u32DstFrameRateNum > pstRecCfg->attribute[index].framerate)
      u32DstFrameRateNum = pstRecCfg->attribute[index].framerate;
  }

  if (u32DstFrameRateNum > pstSensorCfg->framerate) {
    RKADK_LOGW("CamId[%d] record[%d] fps[%d] > sensor fps[%d], use sensor fps",
                u32CamId, index, u32DstFrameRateNum, pstSensorCfg->framerate);
    u32DstFrameRateNum = pstSensorCfg->framerate;

    if (index == 0)
      stFps.enStreamType = RKADK_STREAM_TYPE_VIDEO_MAIN;
    else
      stFps.enStreamType = RKADK_STREAM_TYPE_VIDEO_SUB;
    stFps.u32Framerate = pstSensorCfg->framerate;
    RKADK_PARAM_SetCamParam(u32CamId, RKADK_PARAM_TYPE_FPS, &stFps);
  }

  enType = RKADK_MEDIA_GetRkCodecType(pstRecCfg->attribute[index].codec_type);
  if (pstVencAttr->stVencAttr.enType != enType) {
    RKADK_LOGD("Old codec type [%d], new codec type [%d]",
              pstVencAttr->stVencAttr.enType, enType);
    bReset = true;
  }

  bReset |= RKADK_MEDIA_CompareResolution(pstVencAttr, u32Width, u32Height);
  bReset |= RKADK_MEDIA_VencAttrCmp(pstVencAttr, u32Gop, u32DstFrameRateNum, bitrate);
  if (!bReset)
    return false;

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
  pstVpssAttr->u32Width = pstRecCfg->attribute[index].width;
  pstVpssAttr->u32Height = pstRecCfg->attribute[index].height;

  return true;
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
  RK_MPI_VENC_ResetChn(u32ThumbChn);

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
  bool bUseVpss = false, bReset = false;
  MPP_CHN_S stSrcChn, stRecVenChn;
  VENC_CHN_ATTR_S stVencAttr;
  VI_CHN_ATTR_S stViAttr;
  VPSS_CHN_ATTR_S stVpssAttr;
  RKADK_MUXER_ATTR_S stMuxerAttr;
  RKADK_U32 u32VpssGrp, u32VpssChn;

  ret = RKADK_RECORD_SetMuxerAttr(u32CamId, &stMuxerAttr);
  if (ret) {
    RKADK_LOGE("RKADK_RECORD_SetMuxerAttr failed");
    return -1;
  }

  ret = RKADK_MUXER_Reset(pRecorder);
  if (ret) {
    RKADK_LOGE("RKADK_MUXER_Reset[%d]", u32CamId);
    return -1;
  }

  for (RKADK_U32 index = 0; index < pstRecCfg->file_num; index++) {
    ret = RKADK_RECORD_ResetThumb(u32CamId, index, pstRecCfg);
    if (ret)
      RKADK_LOGE("RKADK_RECORD_ResetThumb failed");

    memset(&stSrcChn, 0, sizeof(MPP_CHN_S));
    memset(&stRecVenChn, 0, sizeof(MPP_CHN_S));

    bUseVpss = RKADK_MUXER_IsUseVpss(pRecorder, pstRecCfg->attribute[index].venc_chn);
    RKADK_RECORD_ResetVideoChn(index, u32CamId, pstRecCfg,
                               &stSrcChn, &stRecVenChn, bUseVpss, pRecorder);

    memset(&stVencAttr, 0, sizeof(VENC_CHN_ATTR_S));
    memset(&stViAttr, 0, sizeof(VI_CHN_ATTR_S));
    memset(&stVpssAttr, 0, sizeof(VPSS_CHN_ATTR_S));

    if (pstRecCfg->record_type == RKADK_REC_TYPE_AOV_LAPSE)
      RK_MPI_VENC_SetSceneMode(pstRecCfg->attribute[index].venc_chn, RKADK_ENCODE_SENSE_IPC);
    else
      RK_MPI_VENC_SetSceneMode(pstRecCfg->attribute[index].venc_chn, RKADK_ENCODE_SENSE_CVR);

    u32VpssGrp = pstRecCfg->attribute[index].vpss_grp;
    u32VpssChn = pstRecCfg->attribute[index].vpss_chn;
    if (bUseVpss) {
      ret = RK_MPI_VPSS_GetChnAttr(u32VpssGrp, u32VpssChn, &stVpssAttr);
      if (ret) {
        RKADK_LOGE("Record[%d] get vpss grp[%d] chn[%d] attr failed[%x]",
                    index, u32VpssGrp, u32VpssChn, ret);
        return -1;
      }
    } else {
      ret = RK_MPI_VI_GetChnAttr(u32CamId, stSrcChn.s32ChnId, &stViAttr);
      if (ret != RK_SUCCESS) {
        RKADK_LOGE("RK_MPI_VI_GetChnAttr[%d] failed [%x]", stSrcChn.s32ChnId, ret);
        return -1;
      }
    }

    ret = RK_MPI_VENC_GetChnAttr(pstRecCfg->attribute[index].venc_chn, &stVencAttr);
    if (ret != RK_SUCCESS) {
      RKADK_LOGE("RK_MPI_VENC_GetChnAttr[%d] failed [%x]", pstRecCfg->attribute[index].venc_chn, ret);
      return -1;
    }

    bChangeResolution = RKADK_MEDIA_CompareResolution(&stVencAttr,
                                  pstRecCfg->attribute[index].width,
                                  pstRecCfg->attribute[index].height);

    bReset = RKADK_RECORD_ResetVideoAttr(u32CamId, index, pstRecCfg, pstSensorCfg,
                                 &stVencAttr, &stViAttr, &stVpssAttr);
    if (!bReset) {
      RKADK_LOGI("Record [%d] stream venc attr is not changed", index);
    } else {
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
        if (bUseVpss) {
          ret = RK_MPI_VPSS_SetChnAttr(u32VpssGrp, u32VpssChn, &stVpssAttr);
          if (ret != RK_SUCCESS) {
            RKADK_LOGE("Record[%d] set vpss grp[%d] chn[%d] attr falied[%x]",
                        index, u32VpssGrp, u32VpssChn, ret);
            return -1;
          }
        } else {
          ret = RK_MPI_VI_SetChnAttr(u32CamId, stSrcChn.s32ChnId, &stViAttr);
          if (ret != RK_SUCCESS) {
            RKADK_LOGE("RK_MPI_VI_SetChnAttr(%d) failed %x",
                        stSrcChn.s32ChnId, ret);
            return -1;
          }
        }
      }
    }

    ret = RKADK_MUXER_ResetParam(stRecVenChn.s32ChnId, pRecorder, &stMuxerAttr, index);
    if (ret) {
      RKADK_LOGE("RKADK_MUXER_Change(%d) failed", stRecVenChn.s32ChnId);
      return -1;
    }

    if (bReset) {
      ret = RK_MPI_SYS_Bind(&stSrcChn, &stRecVenChn);
      if(ret != RK_SUCCESS) {
        RKADK_LOGE("Record index[%d] VI Bind VENC [%d, %d] failed[%x]",
                    index, stSrcChn.s32ChnId, stRecVenChn.s32ChnId, ret);
        return -1;
      }
    }
  }

  return 0;
}

static RKADK_S32 RKADK_RECORD_ResetAudio(RKADK_PARAM_REC_CFG_S *pstRecCfg,
                                         RKADK_MUXER_HANDLE_S *pstRecorder) {
  int ret;
  MPP_CHN_S stSrcChn, stDestChn;

  pstRecorder->u64AudioPts = 0;
  if (pstRecCfg->record_type == pstRecorder->enRecType) {
    RKADK_LOGI("Record type has not changed, no need to reset the audio");
    return 0;
  }

  if (pstRecCfg->record_type != RKADK_REC_TYPE_NORMAL && pstRecorder->enRecType != RKADK_REC_TYPE_NORMAL) {
    RKADK_LOGI("Record type[%d] changed type[%d], no need to reset the audio",
                pstRecorder->enRecType, pstRecCfg->record_type);
    return 0;
  }

  RKADK_LOGI("Record reset audio state [%d %d] start...",
              pstRecorder->u32CamId, pstRecCfg->record_type);

  if (pstRecCfg->record_type != RKADK_REC_TYPE_NORMAL &&
      RKADK_MUXER_EnableAudio(pstRecorder->u32CamId)) {
    RKADK_RECORD_SetAudioChn(&stSrcChn, &stDestChn);

    // Stop get aenc data
    RKADK_MEDIA_StopGetAencBuffer(&stDestChn, RKADK_RECORD_AencOutCb, pstRecorder);

    // UnBind AI to AENC
    ret = RKADK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (ret) {
      RKADK_LOGE("RKADK_MPI_SYS_UnBind failed(%d)", ret);
      RKADK_MEDIA_GetAencBuffer(&stDestChn, RKADK_RECORD_AencOutCb,
                                pstRecorder);
      return ret;
    }

    RKADK_RECORD_DestoryAudioChn();
  } else if (pstRecCfg->record_type == RKADK_REC_TYPE_NORMAL &&
             RKADK_MUXER_EnableAudio(pstRecorder->u32CamId)){
    if (RKADK_RECORD_CreateAudioChn(pstRecorder->u32CamId)) {
      RKADK_LOGE("RKADK_RECORD_CreateAudioChn failed");
      return -1;
    }

    RKADK_RECORD_SetAudioChn(&stSrcChn, &stDestChn);

    // Get aenc data
    RKADK_MEDIA_GetAencBuffer(&stDestChn, RKADK_RECORD_AencOutCb, pstRecorder);

    // Bind AI to AENC
    ret = RKADK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (ret) {
      RKADK_LOGE("RKADK_MPI_SYS_Bind failed(%d)", ret);
      RKADK_MEDIA_StopGetAencBuffer(&stDestChn, RKADK_RECORD_AencOutCb, pstRecorder);
      RKADK_RECORD_DestoryAudioChn();
      return ret;
    }
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

  pstRecCfg = RKADK_PARAM_GetRecCfg(pstRecAttr->s32CamID);
  if (!pstRecCfg) {
    RKADK_LOGE("RKADK_PARAM_GetRecCfg failed");
    return -1;
  }

  RKADK_LOGI("Create Record[%d, %d] Start...", pstRecAttr->s32CamID, pstRecCfg->record_type);
  RKADK_BUFINFO("enter record[%d]", pstRecAttr->s32CamID);

  if (RKADK_RECORD_CreateVideoChn(pstRecAttr))
    return -1;

  bEnableAudio = RKADK_MUXER_EnableAudio(pstRecAttr->s32CamID);
  if (pstRecCfg->record_type == RKADK_REC_TYPE_NORMAL && bEnableAudio) {
    if (RKADK_RECORD_CreateAudioChn(pstRecAttr->s32CamID)) {
      RKADK_RECORD_DestoryVideoChn(pstRecAttr->s32CamID, NULL);
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
  stMuxerAttr.u32FragKeyFrame = pstRecAttr->u32FragKeyFrame;
  memcpy(&stMuxerAttr.stAovAttr, &pstRecAttr->stAovAttr, sizeof(RKADK_AOV_ATTR_S));

  if (RKADK_MUXER_Create(&stMuxerAttr, ppRecorder))
    goto failed;

  if (RKADK_MUXER_Enable(&stMuxerAttr, *ppRecorder)) {
    RKADK_LOGE("RKADK_MUXER_Enable failed");
    RKADK_MUXER_Destroy(*ppRecorder);
    goto failed;
  }

  FileNameListInit(*ppRecorder);

  if (RKADK_RECORD_BindChn(pstRecAttr->s32CamID, *ppRecorder)) {
    RKADK_MUXER_Disable(*ppRecorder);
    RKADK_MUXER_Destroy(*ppRecorder);
    goto failed;
  }

  RKADK_LOGI("Create Record[%d, %d] End...", pstRecAttr->s32CamID,
             pstRecCfg->record_type);
  return 0;

failed:
  RKADK_LOGE("Create Record[%d, %d] failed", pstRecAttr->s32CamID, pstRecCfg->record_type);
  RKADK_RECORD_DestoryVideoChn(pstRecAttr->s32CamID, NULL);

  if (pstRecCfg->record_type == RKADK_REC_TYPE_NORMAL && bEnableAudio)
    RKADK_RECORD_DestoryAudioChn();

  return -1;
}

RKADK_S32 RKADK_RECORD_Destroy(RKADK_MW_PTR pRecorder) {
  RKADK_S32 ret;
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

  RKADK_LOGI("Destory Record[%d, %d] Start...", stRecorder->u32CamId, stRecorder->enRecType);

  FileNameListRelease(stRecorder);

  ret = RKADK_RECORD_UnBindChn(u32CamId, stRecorder);
  if (ret) {
    RKADK_LOGE("RKADK_RECORD_UnBindChn failed, ret = %d", ret);
    return ret;
  }

  RKADK_MUXER_Disable(pRecorder);

  ret = RKADK_RECORD_DestoryVideoChn(u32CamId, pRecorder);
  if (ret) {
    RKADK_LOGE("RKADK_RECORD_DestoryVideoChn failed[%x]", ret);
    return ret;
  }

  if (stRecorder->enRecType == RKADK_REC_TYPE_NORMAL &&
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
  RKADK_LOGI("Destory Record[%d, %d] End...", u32CamId, stRecorder->enRecType);
  return 0;
}

RKADK_S32 RKADK_RECORD_Start(RKADK_MW_PTR pRecorder) {
  return RKADK_MUXER_Start(pRecorder);
}

RKADK_S32 RKADK_RECORD_Stop(RKADK_MW_PTR pRecorder) {
  return RKADK_MUXER_Stop(pRecorder);
}

RKADK_S32 RKADK_RECORD_Single_Start(RKADK_MW_PTR pRecorder, RKADK_STREAM_TYPE_E enStrmType) {
  if (enStrmType != RKADK_STREAM_TYPE_VIDEO_MAIN && enStrmType != RKADK_STREAM_TYPE_VIDEO_SUB) {
    RKADK_LOGE("Invalid stream type[%d]", enStrmType);
    return -1;
  }

  return RKADK_MUXER_Single_Start(pRecorder, enStrmType);
}

RKADK_S32 RKADK_RECORD_Single_Stop(RKADK_MW_PTR pRecorder, RKADK_STREAM_TYPE_E enStrmType) {
  if (enStrmType != RKADK_STREAM_TYPE_VIDEO_MAIN && enStrmType != RKADK_STREAM_TYPE_VIDEO_SUB) {
    RKADK_LOGE("Invalid stream type[%d]", enStrmType);
    return -1;
  }

  return RKADK_MUXER_Single_Stop(pRecorder, enStrmType);
}

int RKADK_RECORD_ResetCheck(RKADK_U32 u32CamId,
                                         RKADK_PARAM_REC_CFG_S *pstRecCfg,
                                         RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg,
                                         RKADK_MUXER_HANDLE_S *pstRecorder) {
  int ret;
  RKADK_U32 u32DstFrameRateNum;
  RKADK_U32 bitrate;
  RKADK_U32 u32Gop;
  RKADK_U32 u32Width;
  RKADK_U32 u32Height;
  RK_CODEC_ID_E enType;
  bool bReset = false;
  VENC_CHN_ATTR_S stVencAttr;

  for (RKADK_U32 index = 0; index < pstRecCfg->file_num; index++) {
    bitrate = pstRecCfg->attribute[index].bitrate;
    u32Gop = pstRecCfg->attribute[index].gop;
    u32DstFrameRateNum = pstRecCfg->attribute[index].framerate;
    u32Width = pstRecCfg->attribute[index].width;
    u32Height = pstRecCfg->attribute[index].height;

    memset(&stVencAttr, 0, sizeof(VENC_CHN_ATTR_S));
    ret = RK_MPI_VENC_GetChnAttr(pstRecCfg->attribute[index].venc_chn, &stVencAttr);
    if (ret != RK_SUCCESS) {
      RKADK_LOGE("RK_MPI_VENC_GetChnAttr[%d] failed [%x]", pstRecCfg->attribute[index].venc_chn, ret);
      return -1;
    }

    if (pstRecCfg->record_type == RKADK_REC_TYPE_LAPSE) {
      u32DstFrameRateNum = pstRecCfg->attribute[index].framerate / pstRecCfg->lapse_multiple;
      if (u32DstFrameRateNum < 1)
        u32DstFrameRateNum = 1;
      else if (u32DstFrameRateNum > pstRecCfg->attribute[index].framerate)
        u32DstFrameRateNum = pstRecCfg->attribute[index].framerate;
    }

    if (u32DstFrameRateNum > pstSensorCfg->framerate)
      u32DstFrameRateNum = pstSensorCfg->framerate;

    bReset = RKADK_MEDIA_CompareResolution(&stVencAttr, u32Width, u32Height);
    if (bReset) {
#ifndef RV1106_1103
      RKADK_LOGD("rv1126/1109 nonsupport dynamic setting resolution");
      return -1;
#endif
    }

    enType = RKADK_MEDIA_GetRkCodecType(pstRecCfg->attribute[index].codec_type);
    if (stVencAttr.stVencAttr.enType != enType) {
#ifndef RV1106_1103
      RKADK_LOGD("rv1126/1109 nonsupport dynamic setting code type, Old type [%d], new type [%d]",
                stVencAttr.stVencAttr.enType, enType);
      return -1;
#else
      RKADK_LOGD("Old codec type [%d], new codec type [%d]",
                stVencAttr.stVencAttr.enType, enType);
      bReset = true;
#endif
    }

    if (pstRecCfg->record_type != pstRecorder->enRecType) {
      RKADK_LOGD("Old record type[%d], new record type[%d]", pstRecorder->enRecType, pstRecCfg->record_type);
      bReset = true;
    }

    bReset |= RKADK_MEDIA_VencAttrCmp(&stVencAttr, u32Gop, u32DstFrameRateNum, bitrate);
    if (bReset)
      return 1;
  }

  return 0;
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

  RKADK_LOGI("Record[%d] reset start...", pstRecorder->u32CamId);

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

  ret = RKADK_RECORD_ResetCheck(u32CamId, pstRecCfg, pstSensorCfg, pstRecorder);
  if (ret == 0) {
    RKADK_LOGI("Record param is not changed");
    return 0;
  } else if (ret < 0) {
    return -1;
  }

  ret = RKADK_MUXER_Stop(*pRecorder);
  if (ret) {
    RKADK_LOGE("RKADK_MUXER_Stop failed[%d]", ret);
    return -1;
  }

  RKADK_MUXER_SetResetState(*pRecorder, true);

  ret = RKADK_RECORD_ResetAudio(pstRecCfg, pstRecorder);
  if (ret) {
    RKADK_LOGE("RKADK_RECORD_ResetAudio failed");
    goto failed;
  }

  ret = RKADK_RECORD_ResetVideo(u32CamId, pstRecCfg, pstSensorCfg, *pRecorder);
  if (ret) {
    RKADK_LOGE("RKADK_RECORD_ResetVideo failed");
    goto failed;
  }

  RKADK_MUXER_SetResetState(*pRecorder, false);
  RKADK_LOGI("Record[%d] reset end...", u32CamId);
  return 0;

failed:
  RKADK_LOGI("Record[%d] reset failed...", u32CamId);
  return -1;
}

RKADK_S32
RKADK_RECORD_ManualSplit(RKADK_MW_PTR pRecorder,
                         RKADK_REC_MANUAL_SPLIT_ATTR_S *pstSplitAttr) {
  return RKADK_MUXER_ManualSplit(pRecorder, pstSplitAttr);
}

RKADK_S32 RKADK_RECORD_GetAencChn() { return RECORD_AENC_CHN; }

RKADK_S32 RKADK_RECORD_ToggleMirror(RKADK_MW_PTR pRecorder,
                                    RKADK_STREAM_TYPE_E enStrmType,
                                    int mirror) {
  RKADK_MUXER_HANDLE_S *pstRecorder = NULL;

  RKADK_CHECK_POINTER(pRecorder, RKADK_FAILURE);
  pstRecorder = (RKADK_MUXER_HANDLE_S *)pRecorder;
  if (!pstRecorder) {
    RKADK_LOGE("pstRecorder is null");
    return -1;
  }

  return RKADK_MEDIA_ToggleVencMirror(pstRecorder->u32CamId, enStrmType, (bool)mirror);
}

RKADK_S32 RKADK_RECORD_ToggleFlip(RKADK_MW_PTR pRecorder,
                                  RKADK_STREAM_TYPE_E enStrmType,
                                  int flip) {
  RKADK_MUXER_HANDLE_S *pstRecorder = NULL;

  RKADK_CHECK_POINTER(pRecorder, RKADK_FAILURE);
  pstRecorder = (RKADK_MUXER_HANDLE_S *)pRecorder;
  if (!pstRecorder) {
    RKADK_LOGE("pstRecorder is null");
    return -1;
  }

  return RKADK_MEDIA_ToggleVencFlip(pstRecorder->u32CamId, enStrmType, (bool)flip);
}

RKADK_S32 RKADK_RECORD_SetRotation(RKADK_MW_PTR pRecorder,
                                  ROTATION_E enRotation,
                                  RKADK_STREAM_TYPE_E enStreamType) {
  int ret;
  RKADK_U32 u32Width, u32Height;
  RKADK_S32 s32VencChnId;
  VENC_CHN_ATTR_S stVencChnAttr;
  RKADK_MUXER_HANDLE_S *pstRecorder = NULL;

  RKADK_CHECK_POINTER(pRecorder, RKADK_FAILURE);
  pstRecorder = (RKADK_MUXER_HANDLE_S *)pRecorder;
  if (!pstRecorder) {
    RKADK_LOGE("pstRecorder is null");
    return -1;
  }

  ret = RKADK_MEDIA_SetVencRotation(pstRecorder->u32CamId, enRotation, enStreamType);
  if (ret == 0) {
    s32VencChnId = RKADK_PARAM_GetVencChnId(pstRecorder->u32CamId, enStreamType);
    if (s32VencChnId < 0) {
      RKADK_LOGE("Stream[%d] get venc chn id failed", enStreamType);
    } else {
      memset(&stVencChnAttr, 0, sizeof(VENC_CHN_ATTR_S));
      ret = RK_MPI_VENC_GetChnAttr(s32VencChnId, &stVencChnAttr);
      if (ret != RK_SUCCESS) {
        RKADK_LOGE("Stream[%d] get venc attr failed [%x]", s32VencChnId, ret);
      } else {
        if (enRotation == ROTATION_90 || enRotation == ROTATION_270) {
          u32Width = stVencChnAttr.stVencAttr.u32PicHeight;
          u32Height = stVencChnAttr.stVencAttr.u32PicWidth;
        } else {
          u32Width = stVencChnAttr.stVencAttr.u32PicWidth;
          u32Height = stVencChnAttr.stVencAttr.u32PicHeight;
        }

        RKADK_MUXER_UpdateRes(pRecorder, s32VencChnId, u32Width, u32Height);
      }
    }

    return 0;
  }

  return -1;
}

RKADK_S32 RKADK_RECORD_FileCacheInit(FILE_CACHE_ARG *pstFileCacheAttr) {
#ifdef FILE_CACHE
  int ret = 0;

  RKADK_CHECK_POINTER(pstFileCacheAttr, RKADK_FAILURE);
  ret = file_cache_init(pstFileCacheAttr);
  RKADK_MUXER_FsCacheNotify();
  return ret;
#else
  RKADK_LOGE("Not define FILE_CACHE");
  return -1;
#endif
}

RKADK_S32 RKADK_RECORD_FileCacheDeInit() {
#ifdef FILE_CACHE
  return file_cache_deinit();
#else
  RKADK_LOGE("Not define FILE_CACHE");
  return -1;
#endif
}

void RKADK_RECORD_FileCacheSetMode(RKADK_REC_TYPE_E enRecType) {
#ifdef FILE_CACHE
  FILE_WRITE_MODE enWriteMode;

  if (enRecType == RKADK_REC_TYPE_AOV_LAPSE)
    enWriteMode = AOV_MODE;
  else
    enWriteMode = NORMAL_MODE;
  file_cache_set_mode(enWriteMode);
#else
  RKADK_LOGE("Not define FILE_CACHE");
#endif
}