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

#include "rkadk_demuxer.h"
#include "linux_list.h"
#include "rkadk_log.h"
#include "rkadk_param.h"
#include "rkadk_signal.h"
#include "rkadk_thread.h"
#include "rkdemuxer.h"

RKADK_S32 RKADK_DEMUXER_Create(RKADK_MW_PTR *demuxerCfg, RKADK_DEMUXER_INPUT_S *demuxerInput) {
  DemuxerInput tempDemuxerInput;

  tempDemuxerInput.ptr = demuxerInput->ptr;
  tempDemuxerInput.s8ReadModeFlag = 0;
  tempDemuxerInput.s8VideoEnableFlag = demuxerInput->videoEnableFlag;
  tempDemuxerInput.s8AudioEnableFlag = demuxerInput->audioEnableFlag;

  RKADK_S32 ret = rkdemuxer_init(demuxerCfg, &tempDemuxerInput);
  if (ret != 0) {
    RKADK_LOGE("RKADK_DEMUXER_Create failed");
    return RKADK_FAILURE;
  }

  return RKADK_SUCCESS;
}

RKADK_VOID RKADK_DEMUXER_Destroy(RKADK_MW_PTR *demuxerCfg) {
  rkdemuxer_deinit(demuxerCfg);
}

RKADK_S32 RKADK_DEMUXER_GetParam(RKADK_MW_PTR demuxerCfg, const RKADK_CHAR *inputName, RKADK_DEMUXER_PARAM_S *demuxerParam) {
  DemuxerParam tempDemuxerParam;

  tempDemuxerParam.pstReadPacketCallback.read_video_packet = demuxerParam->pstReadPacketCallback.pfnReadVideoPacketCallback;
  tempDemuxerParam.pstReadPacketCallback.read_audio_packet = demuxerParam->pstReadPacketCallback.pfnReadAudioPacketCallback;
  RKADK_S32 ret = rkdemuxer_get_param(demuxerCfg, inputName, &tempDemuxerParam);
  if (ret != 0) {
    RKADK_LOGE("RKADK_DEMUXER_GetParam failed");
    return RKADK_FAILURE;
  }

  demuxerParam->totalTime = tempDemuxerParam.s32TotalTime;
  demuxerParam->pVideoCodec = (char *)tempDemuxerParam.pVideoCodec;
  demuxerParam->videoWidth = tempDemuxerParam.s32VideoWidth;
  demuxerParam->videoHeigh = tempDemuxerParam.s32VideoHeigh;
  demuxerParam->VideoFormat = tempDemuxerParam.s8VideoFormat;
  demuxerParam->videoTimeBaseNum = tempDemuxerParam.s32VideoTimeBaseNum;
  demuxerParam->videoTimeBaseDen = tempDemuxerParam.s32VideoTimeBaseDen;
  demuxerParam->videoFirstPTS = tempDemuxerParam.s64VideoFirstPTS;
  demuxerParam->pAudioCodec = (char *)tempDemuxerParam.pAudioCodec;
  demuxerParam->audioChannels = tempDemuxerParam.s32AudioChannels;
  demuxerParam->audioSampleRate = tempDemuxerParam.s32AudioSampleRate;
  demuxerParam->audioFormat = tempDemuxerParam.s8AudioFormat;
  demuxerParam->audioFirstPTS = tempDemuxerParam.s64AudioFirstPTS;
  demuxerParam->audioTimeBaseNum = tempDemuxerParam.s32AudioTimeBaseNum;
  demuxerParam->audioTimeBaseDen = tempDemuxerParam.s32AudioTimeBaseDen;

  return RKADK_SUCCESS;
}

RKADK_S32 RKADK_DEMUXER_ReadPacketStart(RKADK_MW_PTR demuxerCfg) {
  RKADK_S32 ret = rkdemuxer_read_packet_start(demuxerCfg);
  if (ret != 0) {
    RKADK_LOGE("RKADK_DEMUXER_ReadPacketStart failed");
    return RKADK_FAILURE;
  }

  return RKADK_SUCCESS;
}

RKADK_S32 RKADK_DEMUXER_ReadPacketStop(RKADK_MW_PTR demuxerCfg) {
  RKADK_S32 ret = rkdemuxer_read_packet_stop(demuxerCfg);
  if (ret != 0) {
    RKADK_LOGE("RKADK_DEMUXER_ReadPacketStop failed");
    return RKADK_FAILURE;
  }

  return RKADK_SUCCESS;
}
