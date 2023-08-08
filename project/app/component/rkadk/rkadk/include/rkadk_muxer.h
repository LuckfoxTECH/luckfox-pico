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

#ifndef __RKADK_MUXER_H__
#define __RKADK_MUXER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "rkadk_common.h"
#include "rkadk_media_comm.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RKADK_MUXER_FILE_NAME_LEN 256
#define RKADK_MUXER_STREAM_MAX_CNT RECORD_FILE_NUM_MAX
#define RKADK_MUXER_TRACK_MAX_CNT 2 /* a video track and a audio track */
#define RKADK_MUXER_CELL_MAX_CNT 40

typedef enum {
  RKADK_MUXER_EVENT_STREAM_START = 0,
  RKADK_MUXER_EVENT_STREAM_STOP,
  RKADK_MUXER_EVENT_FILE_BEGIN,
  RKADK_MUXER_EVENT_FILE_END,
  RKADK_MUXER_EVENT_MANUAL_SPLIT_END,
  RKADK_MUXER_EVENT_ERR_CREATE_FILE_FAIL,
  RKADK_MUXER_EVENT_ERR_WRITE_FILE_FAIL,
  RKADK_MUXER_EVENT_BUTT
} RKADK_MUXER_EVENT_E;

typedef struct {
  RKADK_CHAR asFileName[RKADK_MUXER_FILE_NAME_LEN];
  RKADK_U32 u32Duration; // ms
} RKADK_MUXER_FILE_EVENT_INFO_S;

typedef struct {
  RKADK_CHAR asFileName[RKADK_MUXER_FILE_NAME_LEN];
  RKADK_S32 s32ErrorCode;
} RKADK_MUXER_ERROR_EVENT_INFO_S;

typedef struct {
  RKADK_MUXER_EVENT_E enEvent;
  union {
    RKADK_MUXER_FILE_EVENT_INFO_S stFileInfo;
    RKADK_MUXER_ERROR_EVENT_INFO_S stErrorInfo;
  } unEventInfo;
} RKADK_MUXER_EVENT_INFO_S;

/* muxer event callback function */
typedef RKADK_VOID (*RKADK_MUXER_EVENT_CALLBACK_FN)(
    RKADK_MW_PTR pHandle, const RKADK_MUXER_EVENT_INFO_S *pstEventInfo);

/* Muxer request file name callback function */
typedef int (*RKADK_MUXER_REQUEST_FILE_NAME_CB)(RKADK_VOID *pHandle,
                                                RKADK_CHAR *pcFileName,
                                                RKADK_U32 u32MuxerId);

/* cell release buf callback */
typedef int (*RKADK_MUXER_CELL_CALLBACK)(void *pMbBlk);

/* muxer manual split type enum */
typedef enum {
  MUXER_PRE_MANUAL_SPLIT,    /* pre manual split type */
  MUXER_NORMAL_MANUAL_SPLIT, /* normal manual split type */
} RKADK_MUXER_MANUAL_SPLIT_TYPE_E;

/* pre manual split attribute */
typedef struct {
  RKADK_MUXER_MANUAL_SPLIT_TYPE_E enManualType; /* maual split type */
  RKADK_U32 u32DurationSec; /* file duration of manual split file */
} RKADK_MUXER_MANUAL_SPLIT_ATTR_S;

typedef enum {
  RKADK_MUXER_TYPE_MP4 = 0,
  RKADK_MUXER_TYPE_MPEGTS,
  RKADK_MUXER_TYPE_FLV,
  RKADK_MUXER_TYPE_BUTT
} RKADK_MUXER_FILE_TYPE_E;

typedef enum {
  RKADK_TRACK_SOURCE_TYPE_VIDEO = 0,
  RKADK_TRACK_SOURCE_TYPE_AUDIO,
  RKADK_TRACK_SOURCE_TYPE_BUTT
} RKADK_TRACK_SOURCE_TYPE_E;

typedef struct {
  RKADK_CODEC_TYPE_E enCodecType;
  RKADK_CHAR cPixFmt[RKADK_PIX_FMT_LEN];
  RKADK_U32 u32Width;
  RKADK_U32 u32Height;
  RKADK_U32 u32BitRate;
  RKADK_U32 u32FrameRate;
  RKADK_U32 u32Gop;
  RKADK_U16 u16Profile;
  RKADK_U16 u16Level;
} RKADK_TRACK_VIDEO_SOURCE_INFO_S;

typedef struct {
  RKADK_CODEC_TYPE_E enCodecType;
  RKADK_U16 u32BitWidth;
  RKADK_U32 u32ChnCnt;
  RKADK_U32 u32SampleRate;
  RKADK_U32 u32SamplesPerFrame;
  RKADK_U32 u32Bitrate;
} RKADK_TRACK_AUDIO_SOURCE_INFO_S;

typedef struct {
  RKADK_U32 u32ChnId;
  RKADK_TRACK_SOURCE_TYPE_E enTrackType;
  union {
    RKADK_TRACK_VIDEO_SOURCE_INFO_S stVideoInfo; /* <video track info */
    RKADK_TRACK_AUDIO_SOURCE_INFO_S stAudioInfo; /* <audio track info */
  } unTrackSourceAttr;
} RKADK_MUXER_TRACK_SOURCE_S;

/* muxer stream attribute */
typedef struct {
  RKADK_U32 u32TimeLenSec; /* record time */
  RKADK_U32 u32TrackCnt;   /* track cnt*/
  RKADK_MUXER_TRACK_SOURCE_S
  aHTrackSrcHandle[RKADK_MUXER_TRACK_MAX_CNT]; /* array of track source cnt */
  RKADK_MUXER_FILE_TYPE_E enType;
} RKADK_MUXER_STREAM_ATTR_S;

typedef enum {
  RKADK_MUXER_PRE_RECORD_NONE = 0,
  RKADK_MUXER_PRE_RECORD_MANUAL_SPLIT, /* manual split file prerecord */
  RKADK_MUXER_PRE_RECORD_SINGLE        /* first file prerecord */
} RKADK_MUXER_PRE_RECORD_MODE_E;

typedef struct {
  RKADK_U32 u32PreRecTimeSec; /* pre record time, unit in second(s)*/
  RKADK_U32 u32PreRecCacheTime;
  RKADK_MUXER_PRE_RECORD_MODE_E enPreRecordMode;
} RKADK_MUXER_PRE_RECORD_ATTR_S;

/** muxer fps attribute */
typedef struct {
  RKADK_U32 u32Fps;      /* framerate */
  RKADK_BOOL bSplitFile; /* stop current file, record the next file immediately
                            at the new framerate */
  RKADK_STREAM_TYPE_E enStreamType; /* stream type */
} RKADK_MUXER_FPS_ATTR_S;

/* muxer attribute param */
typedef struct {
  RKADK_U32 u32CamId;
  bool bLapseRecord;
  RKADK_U32 u32StreamCnt; /* stream cnt */
  RKADK_MUXER_STREAM_ATTR_S
  astStreamAttr[RKADK_MUXER_STREAM_MAX_CNT]; /* array of stream attr */
  RKADK_MUXER_PRE_RECORD_ATTR_S stPreRecordAttr;
  RKADK_MUXER_REQUEST_FILE_NAME_CB pcbRequestFileNames;
  RKADK_MUXER_EVENT_CALLBACK_FN pfnEventCallback;
} RKADK_MUXER_ATTR_S;

typedef struct {
  RKADK_U32 u32CamId;
  bool bLapseRecord;
  RKADK_U32 u32StreamCnt;
  RKADK_MW_PTR pMuxerHandle[RKADK_MUXER_STREAM_MAX_CNT];
  RKADK_U64 u64AudioPts;
} RKADK_MUXER_HANDLE_S;

/**
 * @brief create a new muxer
 * @param[in]pstRecAttr : the attribute of muxer
 * @param[out]ppRecorder : pointer of muxer
 * @return 0 success
 * @return others failure
 */
RKADK_S32 RKADK_MUXER_Create(RKADK_MUXER_ATTR_S *pstMuxerAttr,
                             RKADK_MW_PTR *ppHandle);

/**
 * @brief destory a muxer.
 * @param[in]pRecorder : pointer of muxer
 * @return 0 success
 * @return others failure
 */
RKADK_S32 RKADK_MUXER_Destroy(RKADK_MW_PTR pHandle);

/**
 * @brief start muxer
 * @param[in]pRecorder, pointer of muxer
 * @return 0 success
 * @return -1 failure
 */
RKADK_S32 RKADK_MUXER_Start(RKADK_MW_PTR pHandle);

/**
 * @brief stop muxer
 * @param[in]pRecorder : pointer of muxer
 * @param[in]bQuickMode : quick stop mode flag.
 * @return 0 success
 * @return others failure
 */
RKADK_S32 RKADK_MUXER_Stop(RKADK_MW_PTR pHandle);

/**
 * @brief set muxer framerate
 * @param[in]pRecorder : pointer of muxer
 * @param[in]stFpsAttr : fps attribute
 * the new framerate
 * @return 0 success
 * @return others failure
 */
RKADK_S32 RKADK_MUXER_SetFrameRate(RKADK_MW_PTR pHandle,
                                   RKADK_MUXER_FPS_ATTR_S stFpsAttr);

/**
 * @brief manual splite file.
 * @param[in]pRecorder : pointer of muxer
 * @param[in]pstSplitAttr : manual split attr.
 * @return 0 success
 * @return others failure
 */
RKADK_S32
RKADK_MUXER_ManualSplit(RKADK_MW_PTR pHandle,
                        RKADK_MUXER_MANUAL_SPLIT_ATTR_S *pstSplitAttr);

/**
 * @brief whether to enable audio
 */
bool RKADK_MUXER_EnableAudio(RKADK_S32 s32CamId);

/**
 * @brief write video frame
 */
int RKADK_MUXER_WriteVideoFrame(RKADK_MEDIA_VENC_DATA_S stData, int64_t pts,
                                void *handle);

/**
 * @brief write audio frame
 */
int RKADK_MUXER_WriteAudioFrame(void *pMbBlk, RKADK_U32 size, int64_t pts,
                                void *handle);

RKADK_S32 RKADK_MUXER_ResetParam(RKADK_U32 chnId, RKADK_MW_PTR pHandle,
                                 RKADK_MUXER_ATTR_S *pstMuxerAttr, int index);

RKADK_S32 RKADK_MUXER_Reset(RKADK_MW_PTR pHandle, RKADK_U32 chnId);

void RKADK_MUXER_SetResetState(RKADK_MW_PTR pHandle, bool state);

#ifdef __cplusplus
}
#endif
#endif
