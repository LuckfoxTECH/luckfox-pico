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

#ifndef __RKADK_DEMUXER_H__
#define __RKADK_DEMUXER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "rkadk_common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define DEMUXER_TYPE_ACTIVE RKADK_TRUE
#define DEMUXER_TYPE_PASSIVE RKADK_FALSE

typedef RKADK_VOID (*RKADK_DEMUXER_READ_PACKET_CALLBACK_FN)(RKADK_MW_PTR pHandle);

typedef struct {
  RKADK_DEMUXER_READ_PACKET_CALLBACK_FN pfnReadVideoPacketCallback;
  RKADK_DEMUXER_READ_PACKET_CALLBACK_FN pfnReadAudioPacketCallback;
} RKADK_DEMUXER_READ_PACKET_CALLBACK_S;

typedef struct {
  RKADK_VOID *ptr;
  RKADK_BOOL  readModeFlag;
  RKADK_BOOL  videoEnableFlag;
  RKADK_BOOL  audioEnableFlag;
  const char *transport;
  RKADK_U32 u32IoTimeout;
} RKADK_DEMUXER_INPUT_S;

typedef struct {
  RKADK_S32   totalTime;
  RKADK_CHAR *pVideoCodec;
  RKADK_S32   videoWidth;
  RKADK_S32   videoHeigh;
  RKADK_S8    VideoFormat;
  RKADK_S32   videoTimeBaseNum;
  RKADK_S32   videoTimeBaseDen;
  RKADK_S32   videoAvgFrameRate;
  RKADK_S64   videoFirstPTS;
  RKADK_CHAR *pAudioCodec;
  RKADK_S32   audioChannels;
  RKADK_S32   audioSampleRate;
  RKADK_S8    audioFormat;
  RKADK_S64   audioFirstPTS;
  RKADK_S32   audioTimeBaseNum;
  RKADK_S32   audioTimeBaseDen;
  RKADK_DEMUXER_READ_PACKET_CALLBACK_S pstReadPacketCallback;
} RKADK_DEMUXER_PARAM_S;

/**
 * @brief create a new demuxer
 * @param[in]demuxerCfg : pointer of demuxerCfg
 * @param[in]demuxerParam : pointer of demuxer cfg
 * @return 0 success
 * @return others failure
 */

RKADK_S32 RKADK_DEMUXER_Create(RKADK_MW_PTR *demuxerCfg, RKADK_DEMUXER_INPUT_S *demuxerParam);

/**
 * @brief destory a demuxer.
 * @param[in]demuxerCfg : pointer of demuxerCfg
 */
RKADK_VOID RKADK_DEMUXER_Destroy(RKADK_MW_PTR *demuxerCfg);

/**
 * @brief get demuxer param
 * @param[in]demuxerCfg : pointer of demuxerCfg
 * @param[in]inputName : path of input file
 * @param[in]demuxerParam : pointer of demuxer params
 * @return 0 success
 * @return others failure
 */

RKADK_S32 RKADK_DEMUXER_GetParam(RKADK_MW_PTR demuxerCfg, const RKADK_CHAR *inputName, RKADK_DEMUXER_PARAM_S *demuxerParam);

/**
 * @brief start demuxer
 * @param[in]demuxerCfg : pointer of demuxerCfg
 * @param[in]startPts : start pts of demuxer
 * @return 0 success
 * @return others failure
 */
RKADK_S32 RKADK_DEMUXER_ReadPacketStart(RKADK_MW_PTR demuxerCfg, RKADK_S64 startPts);

/**
 * @brief stop demuxer
 * @param[in]demuxerCfg : pointer of demuxerCfg
 * @return 0 success
 * @return others failure
 */
RKADK_S32 RKADK_DEMUXER_ReadPacketStop(RKADK_MW_PTR demuxerCfg);

/**
 * @brief actively read one video packet
 * @param[in]demuxerCfg : pointer of demuxerCfg
 * @param[in]outputPacket : pointer of output packet
 * @return 0 success
 * @return others failure
 */
RKADK_S32 RKADK_DEMUXER_ReadOneVideoPacket(RKADK_MW_PTR demuxerCfg, void *outputPacket);

/**
 * @brief actively read one audio packet
 * @param[in]demuxerCfg : pointer of demuxerCfg
 * @param[in]outputPacket : pointer of output packet
 * @return 0 success
 * @return others failure
 */
RKADK_S32 RKADK_DEMUXER_ReadOneAudioPacket(RKADK_MW_PTR demuxerCfg, void *outputPacket);

/**
 * @brief read video duration
 * @param[in]demuxerCfg : pointer of demuxerCfg
 * @param[in]duration : pointer of duration
 * @return 0 success
 * @return others failure
 */
RKADK_S32 RKADK_DEMUXER_ReadVideoDuration(RKADK_MW_PTR demuxerCfg, RKADK_S64 *duration);

/**
 * @brief read audio duration
 * @param[in]demuxerCfg : pointer of demuxerCfg
 * @param[in]duration : pointer of duration
 * @return 0 success
 * @return others failure
 */
RKADK_S32 RKADK_DEMUXER_ReadAudioDuration(RKADK_MW_PTR demuxerCfg, RKADK_S64 *duration);

#ifdef __cplusplus
}
#endif
#endif
