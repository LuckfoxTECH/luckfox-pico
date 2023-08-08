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

#include "rkadk_muxer.h"
#include "rkadk_thumb_comm.h"
#include "rkadk_param.h"
#include "linux_list.h"
#include "rkadk_log.h"
#include "rkadk_signal.h"
#include "rkadk_thread.h"
#include "rkmuxer.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(_a) (sizeof(_a) / sizeof((_a)[0]))
#endif // ARRAY_SIZE

/** Stream Count Check */
#define RKADK_CHECK_STREAM_CNT(cnt)                                            \
  do {                                                                         \
    if ((cnt > RKADK_MUXER_STREAM_MAX_CNT) || (cnt == 0)) {                    \
      RKADK_LOGE("stream count[%d] > RKADK_MUXER_STREAM_MAX_CNT or == 0",      \
                 cnt);                                                         \
      return -1;                                                               \
    }                                                                          \
  } while (0)

typedef struct {
  struct list_head mark;
  MB_BLK pMbBlk;
  RKADK_MUXER_CELL_CALLBACK pfnCellReleaseBuf;
  unsigned char *buf;
  RKADK_U32 size;
  int isKeyFrame;
  int64_t pts;
  bool bIsPool;
  struct list_head *pool;
} MUXER_BUF_CELL_S;

typedef struct {
  bool bEnableSplit;
  bool bSplitRecord;
  RKADK_U32 u32SplitDurationSec;
} MANUAL_SPLIT_ATTR;

typedef struct {
  bool bGetThumb;
  bool bRequestThumb;
} MANUAL_THUMB_PARAM;

typedef struct {
  struct list_head stVList;
  struct list_head stAList;
  RKADK_MUXER_PRE_RECORD_ATTR_S stAttr;
  pthread_mutex_t mutex;
} MANUAL_PRE_RECORD_PARAM;

typedef struct {
  RKADK_MW_PTR ptr;
  RKADK_U32 u32CamId;
  RKADK_U32 u32VencChn; // venc channel id
  RKADK_U32 u32ThumbVencChn; // thumb venc channel id
  int muxerId;
  char cFileName[RKADK_MAX_FILE_PATH_LEN];
  const char *cOutputFmt;
  VideoParam stVideo;
  AudioParam stAudio;
  int32_t gop;
  int32_t duration;     // s
  int32_t realDuration; // ms
  int64_t startTime;    // us
  RKADK_U32 frameCnt;
  bool bEnableStream;
  bool bReseting;
  bool bMuxering;
  bool bFirstFile;
  bool bLapseRecord;
  bool bFirstKeyFrame;
  bool bWriteFirstFrame;
  RKADK_MUXER_REQUEST_FILE_NAME_CB pcbRequestFileNames;
  RKADK_MUXER_EVENT_CALLBACK_FN pfnEventCallback;

  void *pThread;
  pthread_mutex_t mutex;
  void *pSignal;

  // list param
  MUXER_BUF_CELL_S stVCell[RKADK_MUXER_CELL_MAX_CNT]; // video list cache size
  MUXER_BUF_CELL_S stACell[RKADK_MUXER_CELL_MAX_CNT]; // audio list cache size
  struct list_head stVFree;     // video list remain size
  struct list_head stAFree;     // audio list remain size
  struct list_head stProcList;  // process list

  MANUAL_THUMB_PARAM stThumbParam;
  MANUAL_SPLIT_ATTR stManualSplit;
  MANUAL_PRE_RECORD_PARAM stPreRecParam;
} MUXER_HANDLE_S;

static void RKADK_MUXER_ListInit(MUXER_HANDLE_S *pstMuxerHandle) {
  INIT_LIST_HEAD(&pstMuxerHandle->stVFree);
  INIT_LIST_HEAD(&pstMuxerHandle->stAFree);
  INIT_LIST_HEAD(&pstMuxerHandle->stProcList);
  INIT_LIST_HEAD(&pstMuxerHandle->stPreRecParam.stAList);
  INIT_LIST_HEAD(&pstMuxerHandle->stPreRecParam.stVList);

  for (unsigned int i = 0; i < ARRAY_SIZE(pstMuxerHandle->stVCell); i++) {
    INIT_LIST_HEAD(&pstMuxerHandle->stVCell[i].mark);
    pstMuxerHandle->stVCell[i].pool = &pstMuxerHandle->stVFree;
    list_add_tail(&pstMuxerHandle->stVCell[i].mark, &pstMuxerHandle->stVFree);
  }

  for (unsigned int i = 0; i < ARRAY_SIZE(pstMuxerHandle->stACell); i++) {
    INIT_LIST_HEAD(&pstMuxerHandle->stACell[i].mark);
    pstMuxerHandle->stACell[i].pool = &pstMuxerHandle->stAFree;
    list_add_tail(&pstMuxerHandle->stACell[i].mark, &pstMuxerHandle->stAFree);
  }
}

static void RKADK_MUXER_CellFree(MUXER_HANDLE_S *pstMuxerHandle,
                                 MUXER_BUF_CELL_S *cell) {
  bool bIsPool = cell->bIsPool;
  struct list_head *pool = cell->pool;

  list_del_init(&cell->mark);
  if (cell->buf)
    cell->buf = NULL;

  if (cell->pfnCellReleaseBuf)
    cell->pfnCellReleaseBuf(cell->pMbBlk);

  if (bIsPool) {
    memset(cell, 0, sizeof(MUXER_BUF_CELL_S));
    INIT_LIST_HEAD(&cell->mark);
    cell->pool = pool;
    RKADK_MUTEX_LOCK(pstMuxerHandle->mutex);
    list_add_tail(&cell->mark, cell->pool);
    RKADK_MUTEX_UNLOCK(pstMuxerHandle->mutex);
  } else {
    free(cell);
    cell = NULL;
  }
}

static int RKADK_MUXER_CellReleaseBuf(MB_BLK pMbBlk) {
  int ret;

  if (pMbBlk == NULL)
    return -1;

  ret = RK_MPI_MB_ReleaseMB(pMbBlk);
  if (ret != RK_SUCCESS) {
    RKADK_LOGE("RK_MPI_MB_ReleaseMB failed[%x]", ret);
    return -1;
  }

  return 0;
}

static void RKADK_MUXER_ListRelease(MUXER_HANDLE_S *pstMuxerHandle,
                                    struct list_head *head) {
  MUXER_BUF_CELL_S *cell = NULL;
  MUXER_BUF_CELL_S *cell_n = NULL;

  list_for_each_entry_safe(cell, cell_n, head, mark) {
    RKADK_MUXER_CellFree(pstMuxerHandle, cell);
  }
}

static MUXER_BUF_CELL_S *RKADK_MUXER_CellGet(MUXER_HANDLE_S *pstMuxerHandle,
                                             struct list_head *head) {
  MUXER_BUF_CELL_S *cell = NULL;
  MUXER_BUF_CELL_S *cell_n = NULL;

  RKADK_MUTEX_LOCK(pstMuxerHandle->mutex);
  list_for_each_entry_safe(cell, cell_n, head, mark) {
    list_del_init(&cell->mark);
    RKADK_MUTEX_UNLOCK(pstMuxerHandle->mutex);
    return cell;
  }
  RKADK_MUTEX_UNLOCK(pstMuxerHandle->mutex);
  return NULL;
}

static int RKADK_MUXER_GetListSize(MUXER_HANDLE_S *pstMuxerHandle,
                                             struct list_head *head) {
  int size = 0;
  MUXER_BUF_CELL_S *cell = NULL;

  list_for_each_entry(cell, head, mark) {
    size++;
  }

  return size;
}

static void RKADK_MUXER_CellPush(MUXER_HANDLE_S *pstMuxerHandle,
                                 struct list_head *head,
                                 MUXER_BUF_CELL_S *one) {
  int ret = -1;
  MUXER_BUF_CELL_S *cell = NULL;
  MUXER_BUF_CELL_S *cell_n = NULL;

  RKADK_MUTEX_LOCK(pstMuxerHandle->mutex);
  do {
    list_for_each_entry_safe(cell, cell_n, head, mark) {
      if ((cell->pts > one->pts) && (cell->pool == one->pool)) {
        list_add_tail(&one->mark, &cell->mark);
        ret = 0;
        break;
      }
    }
    if (ret) {
      list_add_tail(&one->mark, head);
    }
  } while (0);
  RKADK_MUTEX_UNLOCK(pstMuxerHandle->mutex);
}

static MUXER_BUF_CELL_S *RKADK_MUXER_CellPop(MUXER_HANDLE_S *pstMuxerHandle,
                                             struct list_head *head) {
  MUXER_BUF_CELL_S *rst = NULL;
  MUXER_BUF_CELL_S *cell = NULL;
  MUXER_BUF_CELL_S *cell_n = NULL;

  RKADK_MUTEX_LOCK(pstMuxerHandle->mutex);
  do {
    list_for_each_entry_safe(cell, cell_n, head, mark) {
      list_del_init(&cell->mark);
      rst = cell;
      break;
    }
  } while (0);
  RKADK_MUTEX_UNLOCK(pstMuxerHandle->mutex);
  return rst;
}

void RKADK_MUXER_ListDropPFrame(MUXER_HANDLE_S *pstMuxerHandle) {
  MUXER_BUF_CELL_S *rst = NULL;
  MUXER_BUF_CELL_S *cell = NULL;
  MUXER_BUF_CELL_S *cell_n = NULL;

  RKADK_MUTEX_LOCK(pstMuxerHandle->mutex);
  list_for_each_entry_safe(cell, cell_n, &pstMuxerHandle->stProcList, mark) {
    if (cell->isKeyFrame == 0 && cell->pool == &pstMuxerHandle->stVFree) {
      rst = cell;
    }
  }
  if (rst) {
    list_del_init(&rst->mark);
  }
  RKADK_MUTEX_UNLOCK(pstMuxerHandle->mutex);

  if (rst)
    RKADK_MUXER_CellFree(pstMuxerHandle, rst);
  else
    RKADK_LOGE("drop pframe fail");
}

static void RKADK_MUXER_PreRecPush(MUXER_HANDLE_S *pstMuxerHandle,
                                struct list_head *pstList, MUXER_BUF_CELL_S *one) {
  int ret = -1;
  int64_t cacheDuration = 0;
  MUXER_BUF_CELL_S *cell = NULL, *cell_n = NULL;
  MUXER_BUF_CELL_S *pstPreRecCell = NULL;
  MANUAL_PRE_RECORD_PARAM *pstPreRecParam;

  if (pstMuxerHandle->bLapseRecord) {
    if (RKADK_MUXER_GetListSize(pstMuxerHandle, pstList) > 0)
      RKADK_MUXER_ListRelease(pstMuxerHandle, pstList);

    return;
  }

  pstPreRecParam = &pstMuxerHandle->stPreRecParam;
  if (pstPreRecParam->stAttr.u32PreRecTimeSec <= 0)
      return;

  pstPreRecCell = (MUXER_BUF_CELL_S *)malloc(sizeof(MUXER_BUF_CELL_S));
  if (NULL == pstPreRecCell) {
    RKADK_LOGE("Stream[%d]: malloc pre_record cell failed", pstMuxerHandle->u32VencChn);
    return;
  }

  INIT_LIST_HEAD(&pstPreRecCell->mark);
  pstPreRecCell->buf = one->buf;
  pstPreRecCell->isKeyFrame = one->isKeyFrame;
  pstPreRecCell->pts = one->pts;
  pstPreRecCell->size = one->size;
  pstPreRecCell->bIsPool = false;
  pstPreRecCell->pool = one->pool;
  pstPreRecCell->pMbBlk = one->pMbBlk;
  pstPreRecCell->pfnCellReleaseBuf = RKADK_MUXER_CellReleaseBuf;
  RK_MPI_MB_AddUserCnt(one->pMbBlk);

  RKADK_MUTEX_LOCK(pstMuxerHandle->stPreRecParam.mutex);
  do {
    list_for_each_entry_safe(cell, cell_n, pstList, mark) {
      if (cell->pts > pstPreRecCell->pts) {
        list_add_tail(&pstPreRecCell->mark, &cell->mark);
        ret = 0;
        break;
      }
    }

    if (ret) {
      list_add_tail(&pstPreRecCell->mark, pstList);
    }
  } while (0);

  if (!list_empty(pstList)) {
    cell = list_first_entry(pstList, MUXER_BUF_CELL_S, mark);
    if (cell) {
      cell_n = list_entry(pstList->prev, MUXER_BUF_CELL_S, mark);
      if (cell_n)
        cacheDuration = cell_n->pts - cell->pts;

      if (cacheDuration >= pstPreRecParam->stAttr.u32PreRecCacheTime * 1000000)
        RKADK_MUXER_CellFree(pstMuxerHandle, cell);
    }
  }

  RKADK_MUTEX_UNLOCK(pstMuxerHandle->stPreRecParam.mutex);
}

static MUXER_HANDLE_S *RKADK_MUXER_FindHandle(RKADK_MUXER_HANDLE_S *pstMuxer,
                                              RKADK_U32 chnId) {
  MUXER_HANDLE_S *pstMuxerHandle = NULL;

  for (int i = 0; i < (int)pstMuxer->u32StreamCnt; i++) {
    pstMuxerHandle = (MUXER_HANDLE_S *)pstMuxer->pMuxerHandle[i];

    if (pstMuxerHandle && pstMuxerHandle->u32VencChn == chnId)
      break;
  }

  return pstMuxerHandle;
}

int RKADK_MUXER_WriteVideoFrame(RKADK_MEDIA_VENC_DATA_S stData,
                                int64_t pts, void *handle) {
  int cnt = 0, isKeyFrame = 0;
  MUXER_BUF_CELL_S cell;
  MUXER_BUF_CELL_S *pstCell;

  RKADK_CHECK_POINTER(handle, RKADK_FAILURE);

  RKADK_MUXER_HANDLE_S *pstMuxer = (RKADK_MUXER_HANDLE_S *)handle;
  MUXER_HANDLE_S *pstMuxerHandle = RKADK_MUXER_FindHandle(pstMuxer, stData.u32ChnId);
  if (!pstMuxerHandle)
    return -1;

  if (pstMuxerHandle->bReseting)
    return 0;

  if ((stData.stFrame.pstPack->DataType.enH264EType == H264E_NALU_ISLICE ||
      stData.stFrame.pstPack->DataType.enH264EType == H264E_NALU_IDRSLICE) ||
      (stData.stFrame.pstPack->DataType.enH265EType == H265E_NALU_ISLICE ||
      stData.stFrame.pstPack->DataType.enH265EType == H265E_NALU_IDRSLICE))
      isKeyFrame = 1;

  if (pstMuxerHandle->bFirstKeyFrame && isKeyFrame) {
    RKADK_KLOG("Muxer[%d] Stream[%d] first key frame pts: %lld",
               pstMuxerHandle->muxerId, stData.u32ChnId, pts);
    pstMuxerHandle->bFirstKeyFrame = false;
  }

  cell.buf = (unsigned char *)RK_MPI_MB_Handle2VirAddr(stData.stFrame.pstPack->pMbBlk);
  if (NULL == cell.buf) {
    RKADK_LOGE("Stream[%d]: RK_MPI_MB_Handle2VirAddr failed", pstMuxerHandle->u32VencChn);
    return -1;
  }

  cell.isKeyFrame = isKeyFrame;
  cell.pts = pts;
  cell.size = stData.stFrame.pstPack->u32Len;
  cell.bIsPool = true;
  cell.pool = &pstMuxerHandle->stVFree;
  cell.pMbBlk = stData.stFrame.pstPack->pMbBlk;
  cell.pfnCellReleaseBuf = RKADK_MUXER_CellReleaseBuf;
  RKADK_MUXER_PreRecPush(pstMuxerHandle, &pstMuxerHandle->stPreRecParam.stVList, &cell);

  if (!pstMuxerHandle->bEnableStream)
    return 0;

  while ((pstCell = RKADK_MUXER_CellGet(pstMuxerHandle, &pstMuxerHandle->stVFree)) == NULL) {
      if (cnt % 100 == 0)
        RKADK_LOGI("Stream[%d] get video cell fail, retry, cnt = %d", stData.u32ChnId, cnt);
      cnt++;
      usleep(10000);
  }

  pstCell->buf = cell.buf;
  pstCell->isKeyFrame = isKeyFrame;
  pstCell->pts = pts;
  pstCell->size = cell.size;
  pstCell->bIsPool = true;
  pstCell->pMbBlk = cell.pMbBlk;
  pstCell->pfnCellReleaseBuf = RKADK_MUXER_CellReleaseBuf;
  RK_MPI_MB_AddUserCnt(stData.stFrame.pstPack->pMbBlk);
  RKADK_MUXER_CellPush(pstMuxerHandle, &pstMuxerHandle->stProcList, pstCell);
  RKADK_SIGNAL_Give(pstMuxerHandle->pSignal);
  return 0;
}

int RKADK_MUXER_WriteAudioFrame(void *pMbBlk, RKADK_U32 size, int64_t pts,
                                void *handle) {
  int cnt = 0;
  MUXER_HANDLE_S *pstMuxerHandle = NULL;
  RKADK_MUXER_HANDLE_S *pstMuxer = NULL;
  int headerSize = 0; // aenc header size
  MUXER_BUF_CELL_S cell;
  MUXER_BUF_CELL_S *pstCell;

  RKADK_CHECK_POINTER(handle, RKADK_FAILURE);

  pstMuxer = (RKADK_MUXER_HANDLE_S *)handle;
  for (int i = 0; i < (int)pstMuxer->u32StreamCnt; i++) {
    pstMuxerHandle = (MUXER_HANDLE_S *)pstMuxer->pMuxerHandle[i];
    if (!pstMuxerHandle)
      continue;

    if (pstMuxerHandle->bReseting)
      continue;

    if (!strcmp(pstMuxerHandle->stAudio.codec, "ACC"))
      headerSize = 7; //ACC header size

    cell.buf = (unsigned char *)RK_MPI_MB_Handle2VirAddr(pMbBlk) + headerSize;
    if (NULL == cell.buf) {
      RKADK_LOGE("Stream[%d]: RK_MPI_MB_Handle2VirAddr failed", pstMuxerHandle->u32VencChn);
      return -1;
    }

    cell.size = size - headerSize;
    cell.isKeyFrame = 0;
    cell.pts = pts;
    cell.bIsPool = true;
    cell.pool = &pstMuxerHandle->stAFree;
    cell.pMbBlk = pMbBlk;
    cell.pfnCellReleaseBuf = RKADK_MUXER_CellReleaseBuf;
    RKADK_MUXER_PreRecPush(pstMuxerHandle, &pstMuxerHandle->stPreRecParam.stAList, &cell);

    if (!pstMuxerHandle->bEnableStream)
      continue;

    while ((pstCell = RKADK_MUXER_CellGet(pstMuxerHandle, &pstMuxerHandle->stAFree)) == NULL) {
      if (cnt % 100 == 0)
        RKADK_LOGI("Stream[%d] get audio cell fail, retry, cnt = %d",pstMuxerHandle->u32VencChn, cnt);
      cnt++;
      usleep(10000);
    }

    pstCell->buf = cell.buf;
    pstCell->size = cell.size;
    pstCell->isKeyFrame = 0;
    pstCell->pts = pts;
    pstCell->bIsPool = true;
    pstCell->pMbBlk = pMbBlk;
    pstCell->pfnCellReleaseBuf = RKADK_MUXER_CellReleaseBuf;
    RK_MPI_MB_AddUserCnt(pMbBlk);
    RKADK_MUXER_CellPush(pstMuxerHandle, &pstMuxerHandle->stProcList, pstCell);
    RKADK_SIGNAL_Give(pstMuxerHandle->pSignal);
  }

  return 0;
}

void RKADK_MUXER_ProcessEvent(MUXER_HANDLE_S *pstMuxerHandle,
                              RKADK_MUXER_EVENT_E enEventType, int64_t value) {
  RKADK_MUXER_EVENT_INFO_S stEventInfo;
  memset(&stEventInfo, 0, sizeof(RKADK_MUXER_EVENT_INFO_S));

  if (!pstMuxerHandle->pfnEventCallback) {
    RKADK_LOGD("Unregistered event callback");
    return;
  }

  stEventInfo.enEvent = enEventType;
  stEventInfo.unEventInfo.stFileInfo.u32Duration = value;
  memcpy(stEventInfo.unEventInfo.stFileInfo.asFileName,
         pstMuxerHandle->cFileName, strlen(pstMuxerHandle->cFileName));
  pstMuxerHandle->pfnEventCallback(pstMuxerHandle, &stEventInfo);
}

static void RKADK_MUXER_Close(MUXER_HANDLE_S *pstMuxerHandle) {
  if (!pstMuxerHandle->bMuxering)
    return;

  // Stop muxer
  rkmuxer_deinit(pstMuxerHandle->muxerId);

  if (pstMuxerHandle->realDuration <= 0) {
    pstMuxerHandle->realDuration =
      pstMuxerHandle->frameCnt * (1000 / pstMuxerHandle->stVideo.frame_rate_num);
    RKADK_LOGI("The revised Duration = %d, frameCnt = %d",
      pstMuxerHandle->realDuration, pstMuxerHandle->frameCnt);
  }

  if (pstMuxerHandle->stManualSplit.bSplitRecord)
    RKADK_MUXER_ProcessEvent(pstMuxerHandle, RKADK_MUXER_EVENT_MANUAL_SPLIT_END,
                             pstMuxerHandle->realDuration);
  else
    RKADK_MUXER_ProcessEvent(pstMuxerHandle, RKADK_MUXER_EVENT_FILE_END,
                             pstMuxerHandle->realDuration);

  // Reset muxer
  pstMuxerHandle->realDuration = 0;
  pstMuxerHandle->startTime = 0;
  pstMuxerHandle->frameCnt = 0;
  pstMuxerHandle->bMuxering = false;
  pstMuxerHandle->bFirstFile = false;
  pstMuxerHandle->stThumbParam.bGetThumb = false;
  pstMuxerHandle->stThumbParam.bRequestThumb = false;
  pstMuxerHandle->stManualSplit.bEnableSplit = false;
  pstMuxerHandle->stManualSplit.bSplitRecord = false;
}

static bool RKADK_MUXER_GetThumb(MUXER_HANDLE_S *pstMuxerHandle) {
  int ret;
  bool bBuildInOk = false;
  int position = 0;
  RKADK_CHAR *pData = NULL;
  VENC_PACK_S stPack;
  VENC_STREAM_S stFrame;
  FILE *fp = NULL;

  stFrame.pstPack = &stPack;

  position = rkmuxer_get_thumb_pos(pstMuxerHandle->muxerId);
  if (position > 0) {
    ret = RK_MPI_VENC_GetStream(pstMuxerHandle->u32ThumbVencChn, &stFrame, 1);
    if (ret == RK_SUCCESS) {
      pData = (RKADK_CHAR *)RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
      fp = fopen(pstMuxerHandle->cFileName, "r+");
      if (!fp) {
        RKADK_LOGE("Open %s file failed, errno = %d", pstMuxerHandle->cFileName, errno);
        RK_MPI_VENC_ReleaseStream(pstMuxerHandle->u32ThumbVencChn, &stFrame);
        return false;
      }

      if (!fseek(fp, position, SEEK_SET)) {
        fwrite(pData, 1, stFrame.pstPack->u32Len, fp);
        RKADK_LOGI("Stream [%d] thumbnail [seq: %d, len: %d] build in %s file position = %d done!",
                    pstMuxerHandle->u32VencChn, stFrame.u32Seq, stFrame.pstPack->u32Len,
                    pstMuxerHandle->cFileName, position);
        bBuildInOk = false;
      } else {
        RKADK_LOGE("Seek failed");
        bBuildInOk = true;
      }

      fclose(fp);
      ret = RK_MPI_VENC_ReleaseStream(pstMuxerHandle->u32ThumbVencChn, &stFrame);
      if (ret != RK_SUCCESS) {
        RKADK_LOGE("RK_MPI_VENC_ReleaseStream fail %x", ret);
      }
      return bBuildInOk;
    } else {
      return true;
    }
  } else {
    RKADK_LOGI("Stream [%d] position[%d] invalid value, retry!",pstMuxerHandle->u32VencChn, position);
    return true;
  }
}

static bool RKADK_MUXER_CheckEnd(MUXER_HANDLE_S *pstMuxerHandle, MUXER_BUF_CELL_S *cell) {
  bool bFileSwitch;
  int position = 0;
  RKADK_U32 u32Duration;

  if (!cell->isKeyFrame || pstMuxerHandle->duration <= 0)
    return false;

  if (pstMuxerHandle->stManualSplit.bSplitRecord)
    u32Duration = pstMuxerHandle->stManualSplit.u32SplitDurationSec;
  else
    u32Duration = pstMuxerHandle->duration;

  if (pstMuxerHandle->stManualSplit.bEnableSplit) {
    position = rkmuxer_get_thumb_pos(pstMuxerHandle->muxerId);
    if(position <= 0)
      return false;

    RKADK_LOGI("File switch: manual_split[%d], duration: %d",
        pstMuxerHandle->u32VencChn, pstMuxerHandle->realDuration);
    pstMuxerHandle->stManualSplit.bEnableSplit = false;
    RKADK_MUXER_Close(pstMuxerHandle);
    pstMuxerHandle->stManualSplit.bSplitRecord = true;
    return true;
  }

  bFileSwitch = cell->pts - pstMuxerHandle->startTime >=
                (u32Duration * 1000000 - 1000000 / pstMuxerHandle->stVideo.frame_rate_num);
  if (bFileSwitch) {
    RKADK_LOGI("File switch: chn = %d, duration: %d, frameCnt = %d",
        pstMuxerHandle->u32VencChn, pstMuxerHandle->realDuration, pstMuxerHandle->frameCnt + 1);
    RKADK_MUXER_Close(pstMuxerHandle);
    return true;
  }

  return false;
}

static void RKADK_MUXER_RequestThumb(MUXER_HANDLE_S *pstMuxerHandle, MUXER_BUF_CELL_S *cell) {
  RKADK_U32 u32Duration;
  bool bThumbPts, bThumbFrame;
  bool bRequestThumb;
  int ret;

  if (pstMuxerHandle->duration <= 0)
    return;

  if (pstMuxerHandle->stManualSplit.bSplitRecord)
    u32Duration = pstMuxerHandle->stManualSplit.u32SplitDurationSec;
  else
    u32Duration = pstMuxerHandle->duration;

#ifndef THUMB_NORMAL
  bRequestThumb = cell->isKeyFrame && pstMuxerHandle->stThumbParam.bRequestThumb;

  bThumbFrame = pstMuxerHandle->frameCnt > ((u32Duration - pstMuxerHandle->gop /
                pstMuxerHandle->stVideo.frame_rate_num) * pstMuxerHandle->stVideo.frame_rate_num)
                ? true : false;

  bThumbPts = cell->pts - pstMuxerHandle->startTime >=
              (u32Duration - pstMuxerHandle->gop / pstMuxerHandle->stVideo.frame_rate_num) * 1000000
              ? true : false;

  if (bRequestThumb && (bThumbFrame || bThumbPts)) {
    RKADK_LOGI("Stream [%d] request thumbnail frameCnt = %d, realDuration = %d",
                pstMuxerHandle->u32VencChn, pstMuxerHandle->frameCnt, pstMuxerHandle->realDuration);
    ret = RK_MPI_VENC_ThumbnailRequest(pstMuxerHandle->u32VencChn);
    if (ret)
      RKADK_LOGE("RK_MPI_VENC_ThumbnailRequest fail %x", ret);
    pstMuxerHandle->stThumbParam.bRequestThumb = false;
  }
#else
  bRequestThumb = pstMuxerHandle->stThumbParam.bRequestThumb;

  bThumbFrame = pstMuxerHandle->frameCnt > (u32Duration * pstMuxerHandle->stVideo.frame_rate_num - 1)
                ? true : false;

  bThumbPts = cell->pts - pstMuxerHandle->startTime >=
              (u32Duration * 1000000 - 1000000 / pstMuxerHandle->stVideo.frame_rate_num * 2)
              ? true : false;

  if (bRequestThumb && (bThumbFrame || bThumbPts) ) {
    RKADK_LOGI("Request thumbnail frameCnt = %d, realDuration = %d", pstMuxerHandle->frameCnt, pstMuxerHandle->realDuration);
    ThumbnailRequest(pstMuxerHandle->u32ThumbVencChn);
    pstMuxerHandle->stThumbParam.bRequestThumb = false;
  }
#endif
}

static void RKADK_MUXER_ForceRequestThumb(MUXER_HANDLE_S *pstMuxerHandle) {

#ifndef THUMB_NORMAL
  RK_MPI_VENC_ThumbnailRequest(pstMuxerHandle->u32VencChn);
#else
  ThumbnailRequest(pstMuxerHandle->u32ThumbVencChn);
#endif

  pstMuxerHandle->stThumbParam.bRequestThumb = false;
}

static int RKADK_MUXER_PreRecProc(MUXER_HANDLE_S *pstMuxerHandle) {
  int size;
  int64_t s64FirstTime = 0, s64SeekTime = 0;
  bool bPreRecord = false, bFindKeyFrame = false;
  MUXER_BUF_CELL_S *cell = NULL;
  RKADK_MUXER_PRE_RECORD_ATTR_S *pstAttr;

  if (pstMuxerHandle->bLapseRecord)
    return 0;

  size = RKADK_MUXER_GetListSize(pstMuxerHandle, &pstMuxerHandle->stPreRecParam.stVList);
  if (size < pstMuxerHandle->stVideo.frame_rate_num)
    return 0;

  pstAttr = &pstMuxerHandle->stPreRecParam.stAttr;
  switch (pstAttr->enPreRecordMode) {
  case RKADK_MUXER_PRE_RECORD_MANUAL_SPLIT:
    bPreRecord = pstMuxerHandle->stManualSplit.bSplitRecord && (pstAttr->u32PreRecTimeSec > 0);
    break;
  case RKADK_MUXER_PRE_RECORD_SINGLE:
    bPreRecord = pstMuxerHandle->bFirstFile && (pstAttr->u32PreRecTimeSec > 0);
    break;
  default:
    return 0;
  }

  if (!bPreRecord)
    return 0;

  RKADK_MUTEX_LOCK(pstMuxerHandle->stPreRecParam.mutex);
  RKADK_MUXER_ListRelease(pstMuxerHandle, &pstMuxerHandle->stProcList);
  while (!list_empty(&pstMuxerHandle->stPreRecParam.stVList)) {
    cell = list_first_entry(&pstMuxerHandle->stPreRecParam.stVList, MUXER_BUF_CELL_S, mark);
    if (!s64FirstTime)
      s64FirstTime = cell->pts;

    if (!bFindKeyFrame) {
      if (cell->isKeyFrame) {
        bFindKeyFrame = true;
        s64SeekTime = cell->pts - s64FirstTime;
      } else {
        RKADK_MUXER_CellFree(pstMuxerHandle, cell);
        continue;
      }
    }

    list_del_init(&cell->mark);
    list_add_tail(&cell->mark, &pstMuxerHandle->stProcList);
  }

  if (!bFindKeyFrame) {
    RKADK_LOGD("don't find KeyFrame, pre_record fialed");
    RKADK_MUTEX_UNLOCK(pstMuxerHandle->stPreRecParam.mutex);
    return 0;
  }

  s64FirstTime = 0;
  while (!list_empty(&pstMuxerHandle->stPreRecParam.stAList)) {
    cell = list_first_entry(&pstMuxerHandle->stPreRecParam.stAList, MUXER_BUF_CELL_S, mark);
    if (!s64FirstTime)
      s64FirstTime = cell->pts;

    if ((cell->pts - s64FirstTime) < s64SeekTime) {
      RKADK_MUXER_CellFree(pstMuxerHandle, cell);
      continue;
    }

    list_del_init(&cell->mark);
    list_add_tail(&cell->mark, &pstMuxerHandle->stProcList);
  }

  RKADK_MUTEX_UNLOCK(pstMuxerHandle->stPreRecParam.mutex);
  return 1;
}

static bool RKADK_MUXER_Proc(void *params) {
  int ret = 0;
  RKADK_U32 u32Duration;
  MUXER_BUF_CELL_S *cell = NULL;

  if (!params) {
    RKADK_LOGE("Invalid param");
    return false;
  }

  MUXER_HANDLE_S *pstMuxerHandle = (MUXER_HANDLE_S *)params;
  RKADK_SIGNAL_Wait(pstMuxerHandle->pSignal, pstMuxerHandle->duration * 1000);

  cell = RKADK_MUXER_CellPop(pstMuxerHandle, &pstMuxerHandle->stProcList);
  while (cell) {
    // Create muxer
    if (pstMuxerHandle->bEnableStream) {
      if (pstMuxerHandle->stManualSplit.bSplitRecord)
        u32Duration = pstMuxerHandle->stManualSplit.u32SplitDurationSec;
      else
        u32Duration = pstMuxerHandle->duration;

      if (!pstMuxerHandle->bMuxering && cell->isKeyFrame) {
        ret = pstMuxerHandle->pcbRequestFileNames(pstMuxerHandle->ptr,
                                                  pstMuxerHandle->cFileName,
                                                  pstMuxerHandle->muxerId);
        if (ret) {
          RKADK_LOGE("request file name failed");
        } else {
          RKADK_LOGI("Ready to recod new video file path:[%s]",
                    pstMuxerHandle->cFileName);
          RKADK_MUXER_ProcessEvent(pstMuxerHandle, RKADK_MUXER_EVENT_FILE_BEGIN, u32Duration);
          ret = rkmuxer_init(pstMuxerHandle->muxerId,
                            (char *)pstMuxerHandle->cOutputFmt,
                            pstMuxerHandle->cFileName, &pstMuxerHandle->stVideo,
                            &pstMuxerHandle->stAudio);
          if (ret) {
            RKADK_LOGE("rkmuxer_init[%d] failed[%d]", pstMuxerHandle->muxerId, ret);
          } else {
            if (RKADK_MUXER_PreRecProc(pstMuxerHandle)) {
              MUXER_BUF_CELL_S *firstCell = RKADK_MUXER_CellPop(pstMuxerHandle, &pstMuxerHandle->stProcList);
              if (firstCell) {
                RKADK_MUXER_CellFree(pstMuxerHandle, cell);
                cell = firstCell;
              } else {
                RKADK_LOGE("pre_record proc ok, but cell pop fialed");
                RKADK_MUXER_ListRelease(pstMuxerHandle, &pstMuxerHandle->stProcList);
              }
            }

            pstMuxerHandle->bMuxering = true;
            pstMuxerHandle->startTime = cell->pts;
            pstMuxerHandle->stThumbParam.bGetThumb = true;
            pstMuxerHandle->stThumbParam.bRequestThumb = true;
          }
        }
      } else if (!pstMuxerHandle->bMuxering) {
        if(cell->pool == &pstMuxerHandle->stVFree) {
          RKADK_LOGI("Stream [%d] request idr!", pstMuxerHandle->u32VencChn);
          RK_MPI_VENC_RequestIDR(pstMuxerHandle->u32VencChn, RK_FALSE);
        }
      }

      // Process
      if (pstMuxerHandle->bMuxering) {
        if (RKADK_MUXER_CheckEnd(pstMuxerHandle, cell))
          continue;

        // Write
        if (cell->pool == &pstMuxerHandle->stVFree) {
          rkmuxer_write_video_frame(pstMuxerHandle->muxerId, cell->buf,
                                    cell->size, cell->pts, cell->isKeyFrame);

          if (pstMuxerHandle->bWriteFirstFrame) {
            RKADK_KLOG("Muxer[%d] Stream[%d] write first frame pts: %lld",
                       pstMuxerHandle->muxerId, pstMuxerHandle->u32VencChn, cell->pts);
            pstMuxerHandle->bWriteFirstFrame = false;
          }

          if (cell->pts < pstMuxerHandle->startTime)
            RKADK_LOGE("Stream [%d] muxer pts err pts = %lld, startTime = %lld",
                        pstMuxerHandle->u32VencChn, cell->pts, pstMuxerHandle->startTime);
          pstMuxerHandle->realDuration = (cell->pts - pstMuxerHandle->startTime) / 1000;
          pstMuxerHandle->frameCnt++;
          RKADK_MUXER_RequestThumb(pstMuxerHandle, cell);
        } else if (cell->pool == &pstMuxerHandle->stAFree) {
          rkmuxer_write_audio_frame(pstMuxerHandle->muxerId, cell->buf,
                                    cell->size, cell->pts);
        } else {
          RKADK_LOGE("unknow pool");
        }
      }

      if (pstMuxerHandle->stThumbParam.bGetThumb)
        pstMuxerHandle->stThumbParam.bGetThumb = RKADK_MUXER_GetThumb(pstMuxerHandle);
    }

    // free and next
    RKADK_MUXER_CellFree(pstMuxerHandle, cell);
    cell = RKADK_MUXER_CellPop(pstMuxerHandle, &pstMuxerHandle->stProcList);
  }

  // Check exit
  if (!pstMuxerHandle->bEnableStream)
    RKADK_MUXER_Close(pstMuxerHandle);

  return pstMuxerHandle->bEnableStream;
}

static RKADK_S32 RKADK_MUXER_SetAVParam(MUXER_HANDLE_S *pMuxerHandle,
                                    RKADK_MUXER_STREAM_ATTR_S *pstSrcStreamAttr) {
  int i;
  RKADK_PARAM_THUMB_CFG_S *ptsThumbCfg = NULL;
  RKADK_MUXER_TRACK_SOURCE_S *pstTrackSource = NULL;

  ptsThumbCfg = RKADK_PARAM_GetThumbCfg(pMuxerHandle->u32CamId);
  if (!ptsThumbCfg) {
    RKADK_LOGE("RKADK_PARAM_GetThumbCfg failed");
    return -1;
  }

  for (i = 0; i < (int)pstSrcStreamAttr->u32TrackCnt; i++) {
    pstTrackSource = &(pstSrcStreamAttr->aHTrackSrcHandle[i]);

    if (pstTrackSource->enTrackType == RKADK_TRACK_SOURCE_TYPE_VIDEO) {
      RKADK_TRACK_VIDEO_SOURCE_INFO_S *pstVideoInfo =
          &(pstTrackSource->unTrackSourceAttr.stVideoInfo);

      pMuxerHandle->gop = pstVideoInfo->u32Gop;
      pMuxerHandle->u32VencChn = pstTrackSource->u32ChnId;
      pMuxerHandle->stVideo.width = pstVideoInfo->u32Width;
      pMuxerHandle->stVideo.height = pstVideoInfo->u32Height;
      pMuxerHandle->stVideo.bit_rate = pstVideoInfo->u32BitRate;
      pMuxerHandle->stVideo.frame_rate_den = 1;
      pMuxerHandle->stVideo.frame_rate_num = pstVideoInfo->u32FrameRate;
      pMuxerHandle->stVideo.profile = pstVideoInfo->u16Profile;
      pMuxerHandle->stVideo.level = pstVideoInfo->u16Level;

      switch (pstVideoInfo->enCodecType) {
      case RKADK_CODEC_TYPE_H264:
        memcpy(pMuxerHandle->stVideo.codec, "H.264", strlen("H.264"));
        break;
      case RKADK_CODEC_TYPE_H265:
        memcpy(pMuxerHandle->stVideo.codec, "H.265", strlen("H.265"));
        break;
      default:
        RKADK_LOGE("not support enCodecType: %d", pstVideoInfo->enCodecType);
        return -1;
      }

      //thumbnail infomation
      if (!pMuxerHandle->stVideo.thumb.data) {
        pMuxerHandle->stVideo.thumb.width = ptsThumbCfg->thumb_width;
        pMuxerHandle->stVideo.thumb.height = ptsThumbCfg->thumb_height;
        pMuxerHandle->stVideo.thumb.vir_width = ptsThumbCfg->thumb_width;
        pMuxerHandle->stVideo.thumb.vir_height = ptsThumbCfg->thumb_height;
        pMuxerHandle->stVideo.thumb.data_size = ptsThumbCfg->thumb_width *
                                                ptsThumbCfg->thumb_height * 3 / 2;
        pMuxerHandle->stVideo.thumb.data = (unsigned char *)malloc(pMuxerHandle->stVideo.thumb.data_size);
        if (pMuxerHandle->stVideo.thumb.data)
          memset(pMuxerHandle->stVideo.thumb.data, 0, pMuxerHandle->stVideo.thumb.data_size);
      }
    } else if (pstTrackSource->enTrackType == RKADK_TRACK_SOURCE_TYPE_AUDIO) {
      RKADK_TRACK_AUDIO_SOURCE_INFO_S *pstAudioInfo =
          &(pstTrackSource->unTrackSourceAttr.stAudioInfo);

      pMuxerHandle->stAudio.channels = pstAudioInfo->u32ChnCnt;
      pMuxerHandle->stAudio.frame_size = pstAudioInfo->u32SamplesPerFrame;
      pMuxerHandle->stAudio.sample_rate = pstAudioInfo->u32SampleRate;

      switch (pstAudioInfo->u32BitWidth) {
      case 16:
        memcpy(pMuxerHandle->stAudio.format, "S16", strlen("S16"));
        break;
      case 32:
        memcpy(pMuxerHandle->stAudio.format, "S32", strlen("S32"));
        break;
      default:
        RKADK_LOGE("not support u32BitWidth: %d", pstAudioInfo->u32BitWidth);
        return -1;
      }

      switch (pstAudioInfo->enCodecType) {
      case RKADK_CODEC_TYPE_MP3:
        memcpy(pMuxerHandle->stAudio.codec, "MP2", strlen("MP2"));
        break;
      case RKADK_CODEC_TYPE_MP2:
        memcpy(pMuxerHandle->stAudio.codec, "MP2", strlen("MP2"));
        break;
      default:
        RKADK_LOGE("not support enCodecType: %d", pstAudioInfo->enCodecType);
        return -1;
      }
    }
  }

  return 0;
}

static RKADK_S32 RKADK_MUXER_Enable(RKADK_MUXER_ATTR_S *pstMuxerAttr,
                                    RKADK_MUXER_HANDLE_S *pstMuxer) {
  int i;
  char name[256];
  MUXER_HANDLE_S *pMuxerHandle = NULL;
  RKADK_MUXER_STREAM_ATTR_S *pstSrcStreamAttr = NULL;
  RKADK_PARAM_THUMB_CFG_S *ptsThumbCfg = NULL;

  ptsThumbCfg = RKADK_PARAM_GetThumbCfg(pstMuxer->u32CamId);
  if (!ptsThumbCfg) {
    RKADK_LOGE("RKADK_PARAM_GetThumbCfg failed");
    return -1;
  }

  for (i = 0; i < (int)pstMuxerAttr->u32StreamCnt; i++) {
    pMuxerHandle = (MUXER_HANDLE_S *)malloc(sizeof(MUXER_HANDLE_S));
    if (!pMuxerHandle) {
      RKADK_LOGE("malloc muxer handle failed");
      return -1;
    }
    memset(pMuxerHandle, 0, sizeof(MUXER_HANDLE_S));

    pMuxerHandle->u32CamId = pstMuxerAttr->u32CamId;
    pMuxerHandle->muxerId =
        i + (pstMuxerAttr->u32CamId * RKADK_MUXER_STREAM_MAX_CNT);
    pMuxerHandle->pcbRequestFileNames = pstMuxerAttr->pcbRequestFileNames;
    pMuxerHandle->pfnEventCallback = pstMuxerAttr->pfnEventCallback;
    pMuxerHandle->bLapseRecord = pstMuxer->bLapseRecord;
    pMuxerHandle->bReseting = false;

    pstSrcStreamAttr = &(pstMuxerAttr->astStreamAttr[i]);
    pMuxerHandle->duration = pstSrcStreamAttr->u32TimeLenSec;
    pMuxerHandle->bFirstFile = true;
    pMuxerHandle->bFirstKeyFrame = true;
    pMuxerHandle->bWriteFirstFrame = true;

    memcpy(&pMuxerHandle->stPreRecParam.stAttr, &pstMuxerAttr->stPreRecordAttr,
            sizeof(RKADK_MUXER_PRE_RECORD_ATTR_S));
    pMuxerHandle->stPreRecParam.mutex = PTHREAD_MUTEX_INITIALIZER;

    if (i == 0)
      pMuxerHandle->u32ThumbVencChn = ptsThumbCfg->record_main_venc_chn;
    else
      pMuxerHandle->u32ThumbVencChn = ptsThumbCfg->record_sub_venc_chn;

    switch (pstSrcStreamAttr->enType) {
    case RKADK_MUXER_TYPE_MP4:
      pMuxerHandle->cOutputFmt = "mp4";
      break;
    case RKADK_MUXER_TYPE_MPEGTS:
      pMuxerHandle->cOutputFmt = "mpegts";
      break;
    case RKADK_MUXER_TYPE_FLV:
      pMuxerHandle->cOutputFmt = "flv";
      break;
    default:
      RKADK_LOGE("not support type: %d", pstSrcStreamAttr->enType);
      return -1;
    }

    if (RKADK_MUXER_SetAVParam(pMuxerHandle, pstSrcStreamAttr)) {
      RKADK_LOGE("RKADK_MUXER_SetAVParam failed");
      return -1;
    }

    // Init List
    RKADK_MUXER_ListInit(pMuxerHandle);

    // Create signal
    pMuxerHandle->pSignal = RKADK_SIGNAL_Create(0, 1);
    if (!pMuxerHandle->pSignal) {
      RKADK_LOGE("RKADK_SIGNAL_Create failed");
      return -1;
    }
    snprintf(name, sizeof(name), "Muxer_%d", pMuxerHandle->u32VencChn);
    pMuxerHandle->ptr = (RKADK_MW_PTR)pstMuxer;
    pMuxerHandle->mutex = PTHREAD_MUTEX_INITIALIZER;
    pMuxerHandle->pThread = RKADK_THREAD_Create(RKADK_MUXER_Proc, pMuxerHandle, name);
    if (!pMuxerHandle->pThread) {
      RKADK_LOGE("RKADK_THREAD_Create failed");
      return -1;
    }
    pstMuxer->pMuxerHandle[i] = (RKADK_MW_PTR)pMuxerHandle;
  }

  return 0;
}

RKADK_S32 RKADK_MUXER_Create(RKADK_MUXER_ATTR_S *pstMuxerAttr,
                             RKADK_MW_PTR *ppHandle) {
  int i, ret = 0;
  RKADK_MUXER_HANDLE_S *pstMuxer = NULL;

  RKADK_LOGI("Create Record[%d] Start...", pstMuxerAttr->u32CamId);

  RKADK_CHECK_POINTER(pstMuxerAttr, RKADK_FAILURE);
  RKADK_CHECK_CAMERAID(pstMuxerAttr->u32CamId, RKADK_FAILURE);

  if (*ppHandle) {
    RKADK_LOGE("Muxer Handle has been created");
    return -1;
  }

  pstMuxer = (RKADK_MUXER_HANDLE_S *)malloc(sizeof(RKADK_MUXER_HANDLE_S));
  if (!pstMuxer) {
    RKADK_LOGE("malloc pstMuxer failed");
    return -1;
  }
  memset(pstMuxer, 0, sizeof(RKADK_MUXER_HANDLE_S));

  pstMuxer->u32CamId = pstMuxerAttr->u32CamId;
  pstMuxer->u32StreamCnt = pstMuxerAttr->u32StreamCnt;
  pstMuxer->bLapseRecord = pstMuxerAttr->bLapseRecord;

  ret = RKADK_MUXER_Enable(pstMuxerAttr, pstMuxer);
  if (ret) {
    RKADK_LOGE("RKADK_MUXER_Enable failed");
    goto failed;
  }

  *ppHandle = (RKADK_MW_PTR)pstMuxer;
  RKADK_LOGI("Create Record[%d] Stop...", pstMuxerAttr->u32CamId);
  return 0;

failed:
  for (i = 0; i < (int)pstMuxerAttr->u32StreamCnt; i++) {
    if (pstMuxer->pMuxerHandle[i]) {
      free(pstMuxer->pMuxerHandle[i]);
      pstMuxer->pMuxerHandle[i] = NULL;
    }
  }

  if (pstMuxer) {
    free(pstMuxer);
    pstMuxer = NULL;
  }

  return ret;
}

RKADK_S32 RKADK_MUXER_Destroy(RKADK_MW_PTR pHandle) {
  MUXER_HANDLE_S *pstMuxerHandle = NULL;
  RKADK_MUXER_HANDLE_S *pstMuxer = NULL;

  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);

  pstMuxer = (RKADK_MUXER_HANDLE_S *)pHandle;
  RKADK_CHECK_STREAM_CNT(pstMuxer->u32StreamCnt);

  RKADK_LOGI("Destory Muxer[%d] Start...", pstMuxer->u32CamId);

  for (int i = 0; i < (int)pstMuxer->u32StreamCnt; i++) {
    pstMuxerHandle = (MUXER_HANDLE_S *)pstMuxer->pMuxerHandle[i];

    if (!pstMuxerHandle) {
      RKADK_LOGD("Muxer Handle[%d] is NULL", i);
      continue;
    }

    // Set flag off
    pstMuxerHandle->bEnableStream = false;

    // exit thread
    RKADK_THREAD_SetExit(pstMuxerHandle->pThread);

    RKADK_SIGNAL_Give(pstMuxerHandle->pSignal);

    // Destroy thread
    RKADK_THREAD_Destory(pstMuxerHandle->pThread);
    pstMuxerHandle->pThread = NULL;

    // Destroy signal
    RKADK_SIGNAL_Destroy(pstMuxerHandle->pSignal);

    // Release list
    RKADK_MUXER_ListRelease(pstMuxerHandle, &pstMuxerHandle->stProcList);
    RKADK_MUXER_ListRelease(pstMuxerHandle, &pstMuxerHandle->stPreRecParam.stAList);
    RKADK_MUXER_ListRelease(pstMuxerHandle, &pstMuxerHandle->stPreRecParam.stVList);

    if (pstMuxerHandle->stVideo.thumb.data) {
      free(pstMuxerHandle->stVideo.thumb.data);
      pstMuxerHandle->stVideo.thumb.data = NULL;
    }

    free(pstMuxer->pMuxerHandle[i]);
    pstMuxer->pMuxerHandle[i] = NULL;
  }

  RKADK_LOGI("Destory Muxer[%d] Stop...", pstMuxer->u32CamId);

  if (pstMuxer) {
    free(pstMuxer);
    pstMuxer = NULL;
  }

  return 0;
}

RKADK_S32 RKADK_MUXER_Start(RKADK_MW_PTR pHandle) {
  MUXER_HANDLE_S *pstMuxerHandle = NULL;
  RKADK_MUXER_HANDLE_S *pstMuxer = NULL;

  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);

  pstMuxer = (RKADK_MUXER_HANDLE_S *)pHandle;
  RKADK_CHECK_STREAM_CNT(pstMuxer->u32StreamCnt);

  for (int i = 0; i < (int)pstMuxer->u32StreamCnt; i++) {
    pstMuxerHandle = (MUXER_HANDLE_S *)pstMuxer->pMuxerHandle[i];
    if (!pstMuxerHandle) {
      RKADK_LOGD("Muxer Handle[%d] is NULL", i);
      continue;
    }

    RK_MPI_VENC_RequestIDR(pstMuxerHandle->u32VencChn, RK_FALSE);
    RKADK_MUXER_ForceRequestThumb(pstMuxerHandle);
    pstMuxerHandle->bEnableStream = true;
    pstMuxerHandle->bFirstFile = true;
    RKADK_MUXER_ProcessEvent(pstMuxerHandle, RKADK_MUXER_EVENT_STREAM_START, 0);
  }

  return 0;
}

RKADK_S32 RKADK_MUXER_Stop(RKADK_MW_PTR pHandle) {
  MUXER_HANDLE_S *pstMuxerHandle = NULL;
  RKADK_MUXER_HANDLE_S *pstMuxer = NULL;

  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);

  pstMuxer = (RKADK_MUXER_HANDLE_S *)pHandle;
  RKADK_CHECK_STREAM_CNT(pstMuxer->u32StreamCnt);

  for (int i = 0; i < (int)pstMuxer->u32StreamCnt; i++) {
    pstMuxerHandle = (MUXER_HANDLE_S *)pstMuxer->pMuxerHandle[i];
    if (!pstMuxerHandle) {
      RKADK_LOGD("Muxer Handle[%d] is NULL", i);
      continue;
    }

    pstMuxerHandle->bEnableStream = false;
    pstMuxerHandle->stManualSplit.bEnableSplit = false;
    pstMuxerHandle->stManualSplit.bSplitRecord = false;
    RKADK_MUXER_ProcessEvent(pstMuxerHandle, RKADK_MUXER_EVENT_STREAM_STOP, 0);
    RKADK_SIGNAL_Give(pstMuxerHandle->pSignal);
  }

  return 0;
}

RKADK_S32 RKADK_MUXER_SetFrameRate(RKADK_MW_PTR pHandle,
                                   RKADK_MUXER_FPS_ATTR_S stFpsAttr) {
  return 0;
}

RKADK_S32
RKADK_MUXER_ManualSplit(RKADK_MW_PTR pHandle,
                        RKADK_MUXER_MANUAL_SPLIT_ATTR_S *pstSplitAttr) {
  MUXER_HANDLE_S *pstMuxerHandle = NULL;
  RKADK_MUXER_HANDLE_S *pstMuxer = NULL;

  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);
  RKADK_CHECK_POINTER(pstSplitAttr, RKADK_FAILURE);

  pstMuxer = (RKADK_MUXER_HANDLE_S *)pHandle;
  RKADK_CHECK_STREAM_CNT(pstMuxer->u32StreamCnt);

  if (!pstSplitAttr->u32DurationSec) {
    RKADK_LOGE("invalid manual split duration[%d]", pstSplitAttr->u32DurationSec);
    return -1;
  }

  for (int i = 0; i < (int)pstMuxer->u32StreamCnt; i++) {
    pstMuxerHandle = (MUXER_HANDLE_S *)pstMuxer->pMuxerHandle[i];

    if (!pstMuxerHandle) {
      RKADK_LOGD("Muxer Handle[%d] is NULL", i);
      continue;
    }

    if (!pstMuxerHandle->bEnableStream)
      return -1;

    if (pstSplitAttr->enManualType == MUXER_PRE_MANUAL_SPLIT)
      pstMuxerHandle->stPreRecParam.stAttr.enPreRecordMode = RKADK_MUXER_PRE_RECORD_MANUAL_SPLIT;

    pstMuxerHandle->stManualSplit.bEnableSplit = true;
    RK_MPI_VENC_RequestIDR(pstMuxerHandle->u32VencChn, RK_FALSE);
    RKADK_MUXER_ForceRequestThumb(pstMuxerHandle);
    pstMuxerHandle->stManualSplit.u32SplitDurationSec = pstSplitAttr->u32DurationSec;
  }

  return 0;
}

bool RKADK_MUXER_EnableAudio(RKADK_S32 s32CamId) {
  bool bEnable = false;
  RKADK_PARAM_REC_CFG_S *pstRecCfg = NULL;
  RKADK_PARAM_AUDIO_CFG_S *pstAudioCfg = NULL;

  pstRecCfg = RKADK_PARAM_GetRecCfg(s32CamId);
  if (!pstRecCfg) {
    RKADK_LOGE("RKADK_PARAM_GetRecCfg failed");
    return false;
  }

  pstAudioCfg = RKADK_PARAM_GetAudioCfg();
  if (!pstAudioCfg) {
    RKADK_LOGE("RKADK_PARAM_GetAudioCfg failed");
    return false;
  }

  switch (pstAudioCfg->codec_type) {
  case RKADK_CODEC_TYPE_MP3:
    bEnable = true;
    break;

  case RKADK_CODEC_TYPE_MP2:
    if (pstRecCfg->file_type == RKADK_MUXER_TYPE_FLV)
      bEnable = false;
    else
      bEnable = true;
    break;

  default:
    bEnable = false;
    break;
  }

  return bEnable;
}

RKADK_S32 RKADK_MUXER_ResetParam(RKADK_U32 chnId, RKADK_MW_PTR pHandle,
                             RKADK_MUXER_ATTR_S *pstMuxerAttr, int index) {
  int ret;
  MUXER_HANDLE_S *pstMuxerHandle = NULL;
  RKADK_MUXER_HANDLE_S *pstMuxer = NULL;

  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);
  RKADK_CHECK_POINTER(pstMuxerAttr, RKADK_FAILURE);

  pstMuxer = (RKADK_MUXER_HANDLE_S *)pHandle;
  RKADK_CHECK_STREAM_CNT(pstMuxer->u32StreamCnt);

  pstMuxerHandle = RKADK_MUXER_FindHandle(pstMuxer, chnId);
  if (!pstMuxerHandle) {
    RKADK_LOGE("don't find muxer handle");
    return -1;
  }

  pstMuxerHandle->bLapseRecord = pstMuxerAttr->bLapseRecord;
  pstMuxerHandle->duration = pstMuxerAttr->astStreamAttr[index].u32TimeLenSec;
  if (pstMuxerHandle->bLapseRecord)
    memset(&pstMuxerHandle->stAudio, 0, sizeof(AudioParam));

  ret = RKADK_MUXER_SetAVParam(pstMuxerHandle, &pstMuxerAttr->astStreamAttr[index]);
  if (ret) {
    RKADK_LOGE("RKADK_MUXER_SetAVParam failed");
    return -1;
  }

  return 0;
}

RKADK_S32 RKADK_MUXER_Reset(RKADK_MW_PTR pHandle, RKADK_U32 chnId) {
  MUXER_HANDLE_S *pstMuxerHandle = NULL;
  RKADK_MUXER_HANDLE_S *pstMuxer = NULL;
  char name[256];

  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);

  pstMuxer = (RKADK_MUXER_HANDLE_S *)pHandle;
  RKADK_CHECK_STREAM_CNT(pstMuxer->u32StreamCnt);

  RKADK_LOGI("Reset Muxer[%d] Start...", chnId);

  pstMuxerHandle = RKADK_MUXER_FindHandle(pstMuxer, chnId);

  if (!pstMuxerHandle) {
    RKADK_LOGD("Muxer Handle is NULL");
    return -1;
  }

  RKADK_THREAD_SetExit(pstMuxerHandle->pThread);
  RKADK_SIGNAL_Give(pstMuxerHandle->pSignal);

  // Destroy thread
  RKADK_THREAD_Destory(pstMuxerHandle->pThread);
  pstMuxerHandle->pThread = NULL;

  snprintf(name, sizeof(name), "Muxer_%d", pstMuxerHandle->u32VencChn);
  pstMuxerHandle->pThread = RKADK_THREAD_Create(RKADK_MUXER_Proc, pstMuxerHandle, name);
  if (!pstMuxerHandle->pThread) {
    RKADK_LOGE("RKADK_THREAD_Create failed");
    return -1;
  }

  // Release list
  RKADK_MUXER_ListRelease(pstMuxerHandle, &pstMuxerHandle->stProcList);
  RKADK_MUXER_ListRelease(pstMuxerHandle, &pstMuxerHandle->stPreRecParam.stAList);
  RKADK_MUXER_ListRelease(pstMuxerHandle, &pstMuxerHandle->stPreRecParam.stVList);

  RKADK_LOGI("Reset Muxer[%d] End...", chnId);
  return 0;
}

void RKADK_MUXER_SetResetState(RKADK_MW_PTR pHandle, bool state) {
  MUXER_HANDLE_S *pstMuxerHandle = NULL;
  RKADK_MUXER_HANDLE_S *pstMuxer = NULL;

  RKADK_CHECK_POINTER_N(pHandle);
  pstMuxer = (RKADK_MUXER_HANDLE_S *)pHandle;
  if (pstMuxer->u32StreamCnt > RKADK_MUXER_STREAM_MAX_CNT || !pstMuxer->u32StreamCnt)
    return;

  for (int i = 0; i < (int)pstMuxer->u32StreamCnt; i++) {
    pstMuxerHandle = (MUXER_HANDLE_S *)pstMuxer->pMuxerHandle[i];
    if (!pstMuxerHandle) {
      RKADK_LOGD("Muxer Handle[%d] is NULL", i);
      continue;
    }

    pstMuxerHandle->bReseting = state;
  }
}