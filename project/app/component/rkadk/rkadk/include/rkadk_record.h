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

#ifndef __RKADK_RECORD_H__
#define __RKADK_RECORD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "rkadk_common.h"
#include "rkadk_muxer.h"

typedef RKADK_MUXER_FPS_ATTR_S RKADK_RECORD_FPS_ATTR_S;
typedef RKADK_MUXER_MANUAL_SPLIT_ATTR_S RKADK_REC_MANUAL_SPLIT_ATTR_S;
typedef RKADK_MUXER_EVENT_CALLBACK_FN RKADK_REC_EVENT_CALLBACK_FN;

/* record create file function */
typedef RKADK_S32 (*RKADK_REC_REQUEST_FILE_NAMES_FN)(
    RKADK_MW_PTR pRecorder, RKADK_U32 u32FileCnt,
    RKADK_CHAR (*paszFilename)[RKADK_MAX_FILE_PATH_LEN]);

/* record type enum */
typedef enum {
  RKADK_REC_TYPE_NORMAL = 0, /* normal record */
  RKADK_REC_TYPE_LAPSE, /* time lapse record, record a frame by an fixed time
                           interval */
  RKADK_REC_TYPE_BUTT
} RKADK_REC_TYPE_E;

/** record task's attribute */
typedef struct {
  RKADK_S32 s32CamID;                                  /* camera id */
  RKADK_REC_REQUEST_FILE_NAMES_FN pfnRequestFileNames; /* rec callbak */
  RKADK_REC_EVENT_CALLBACK_FN pfnEventCallback;        /* event callbak */
} RKADK_RECORD_ATTR_S;

/****************************************************************************/
/*                            Interface Definition                          */
/****************************************************************************/
/**
 * @brief create a new recorder
 * @param[in]pstRecAttr : the attribute of recorder
 * @param[out]ppRecorder : pointer of recorder
 * @return 0 success
 * @return others failure
 */
RKADK_S32 RKADK_RECORD_Create(RKADK_RECORD_ATTR_S *pstRecAttr,
                              RKADK_MW_PTR *ppRecorder);

/**
 * @brief destory a recorder.
 * @param[in]pRecorder : pointer of recorder
 * @return 0 success
 * @return others failure
 */
RKADK_S32 RKADK_RECORD_Destroy(RKADK_MW_PTR pRecorder);

/**
 * @brief start recorder
 * @param[in]pRecorder, pointer of recorder
 * @return 0 success
 * @return -1 failure
 */
RKADK_S32 RKADK_RECORD_Start(RKADK_MW_PTR pRecorder);

/**
 * @brief stop recorder
 * @param[in]pRecorder : pointer of recorder
 * @return 0 success
 * @return others failure
 */
RKADK_S32 RKADK_RECORD_Stop(RKADK_MW_PTR pRecorder);

/**
 * @param[in]pRecorder : pointer of recorder
 * @return 0 success
 * @return others failure
 */
RKADK_S32 RKADK_RECORD_Reset(RKADK_MW_PTR *pRecorder);

/**
 * @brief set recorder framerate
 * @param[in]pRecorder : pointer of recorder
 * @param[in]stFpsAttr : fps attribute
 * @return 0 success
 * @return others failure
 */
RKADK_S32 RKADK_RECORD_SetFrameRate(RKADK_MW_PTR pRecorder,
                                    RKADK_RECORD_FPS_ATTR_S stFpsAttr);

/**
 * @brief manual splite file.
 * @param[in]pRecorder : pointer of recorder
 * @param[in]pstSplitAttr : manual split attr.
 * @return 0 success
 * @return others failure
 */
RKADK_S32 RKADK_RECORD_ManualSplit(RKADK_MW_PTR pRecorder,
                                   RKADK_REC_MANUAL_SPLIT_ATTR_S *pstSplitAttr);

/**
 * @brief register recorder envent callback
 * @param[in]pRecorder : pointer of recorder
 * @param[in]pfnEventCallback : callback function
 * @return 0 success
 * @return others failure
 */
RKADK_S32
RKADK_RECORD_RegisterEventCallback(
    RKADK_MW_PTR pRecorder, RKADK_REC_EVENT_CALLBACK_FN pfnEventCallback);

/**
 * @brief get aenc chn id.
 * @return aenc chn id success
 * @return -1 failure
 */
RKADK_S32 RKADK_RECORD_GetAencChn();

/**
 * @brief toggle mirror implemented by VPSS
 * @return 0 success
 * @return -1 failure
 */
RKADK_S32 RKADK_RECORD_ToogleMirror(RKADK_MW_PTR pRecorder,
                                    RKADK_STREAM_TYPE_E enStrmType, int mirror);

/**
 * @brief toggle flip implemented by VPSS
 * @return 0 success
 * @return -1 failure
 */
RKADK_S32 RKADK_RECORD_ToogleFlip(RKADK_MW_PTR pRecorder,
                                  RKADK_STREAM_TYPE_E enStrmType, int flip);

#if 0
/**
 * @brief get single frame at time.
 * @param[in]pszFileName : video file
 * @param[in]pstFrameAttr : frame attribute
 * @param[in]u32VeChn : venc channel
 * @param[in]u64TimeUs : frame time(us)
 * @return 0 success
 * @return -1 failure
 */
RKADK_S32 RKADK_GetSingleFrameAtTime(RKADK_CHAR *pszFileName,
                                     RKADK_FRAME_ATTR_S *pstFrameAttr,
                                     RKADK_U32 u32VeChn, RKADK_U64 u64TimeUs);
#endif

#ifdef __cplusplus
}
#endif
#endif
