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

#include "rkadk_media_comm.h"
#include "rkadk_thumb_comm.h"
#include "rkadk_param.h"
#include "rkadk_log.h"
#include "rkadk_version.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RKADK_VQE_FRAME_SAMPLE 320; // 20ms;
#define RKADK_DEFAULT_VQE_FILE "/usr/share/rkap_3a/para/16k/RKAP_3A_Para.bin"

typedef struct {
  bool bUsed;
  RKADK_S32 s32BindCnt;
  MPP_CHN_S stSrcChn;
  MPP_CHN_S stDestChn;
} RKADK_BIND_INFO_S;

typedef struct {
  bool bGetBuffer;
  RKADK_S32 s32GetCnt;
  pthread_t tid;
  RKADK_VOID *pHandle[RKADK_MEDIA_AENC_MAX_CNT];
  RKADK_MEDIA_AENC_DATA_PROC_FUNC cbList[RKADK_MEDIA_AENC_MAX_CNT];
} RKADK_GET_AENC_MB_ATTR_S;

typedef struct {
  bool bGetBuffer;
  RKADK_S32 s32GetCnt;
  pthread_t tid;
  RKADK_VOID *pHandle[RKADK_MEDIA_VENC_MAX_CNT];
  RKADK_MEDIA_VENC_DATA_PROC_FUNC cbList[RKADK_MEDIA_VENC_MAX_CNT];
} RKADK_GET_VENC_MB_ATTR_S;

typedef struct {
  bool bUsed;
  RKADK_S32 s32InitCnt;
  RKADK_S32 s32DevId;
  RKADK_S32 s32ChnId;
  RK_CODEC_ID_E enCodecType;
  RKADK_GET_AENC_MB_ATTR_S stGetAencMBAttr;
  RKADK_GET_VENC_MB_ATTR_S stGetVencMBAttr;
} RKADK_MEDIA_INFO_S;

typedef struct {
  pthread_mutex_t aiMutex;
  pthread_mutex_t aencMutex;
  pthread_mutex_t viMutex;
  pthread_mutex_t vencMutex;
  pthread_mutex_t vpssMutex;
  pthread_mutex_t bindMutex;
  RKADK_MEDIA_INFO_S stAiInfo[RKADK_MEDIA_AI_MAX_CNT];
  RKADK_MEDIA_INFO_S stAencInfo[RKADK_MEDIA_AENC_MAX_CNT];
  RKADK_MEDIA_INFO_S stViInfo[RKADK_MEDIA_VI_MAX_CNT];
  RKADK_MEDIA_INFO_S stVencInfo[RKADK_MEDIA_VENC_MAX_CNT];
  RKADK_MEDIA_INFO_S stVpssInfo[RKADK_MEDIA_VPSS_MAX_CNT];
  RKADK_BIND_INFO_S stAiAencInfo[RKADK_AI_AENC_MAX_BIND_CNT];
  RKADK_BIND_INFO_S stViVencInfo[RKADK_VI_VENC_MAX_BIND_CNT];
  RKADK_BIND_INFO_S stViVpssInfo[RKADK_VI_VPSS_MAX_BIND_CNT];
  RKADK_BIND_INFO_S stVpssVencInfo[RKADK_VPSS_VENC_MAX_BIND_CNT];
} RKADK_MEDIA_CONTEXT_S;

static bool g_bSysInit = false;
static RKADK_MEDIA_CONTEXT_S g_stMediaCtx;
static bool g_bVpssGrpInit[VPSS_MAX_GRP_NUM] = {0};

static void RKADK_MEDIA_CtxInit() {
  memset((void *)&g_stMediaCtx, 0, sizeof(RKADK_MEDIA_CONTEXT_S));
  g_stMediaCtx.aiMutex = PTHREAD_MUTEX_INITIALIZER;
  g_stMediaCtx.aencMutex = PTHREAD_MUTEX_INITIALIZER;
  g_stMediaCtx.viMutex = PTHREAD_MUTEX_INITIALIZER;
  g_stMediaCtx.vencMutex = PTHREAD_MUTEX_INITIALIZER;
  g_stMediaCtx.vpssMutex = PTHREAD_MUTEX_INITIALIZER;
  g_stMediaCtx.bindMutex = PTHREAD_MUTEX_INITIALIZER;
}

static RKADK_U32 RKADK_MPI_VPSS_CreateGrp(RKADK_S32 s32VpssGrp, VPSS_GRP_ATTR_S *pstVpssGrpAttr) {
  int ret;
  RKADK_PARAM_COMM_CFG_S *pstCommCfg = NULL;
  if (g_bVpssGrpInit[s32VpssGrp])
    return 0;

  pstCommCfg = RKADK_PARAM_GetCommCfg();
  if (!pstCommCfg) {
    RKADK_LOGE("RKADK_PARAM_GetCommCfg failed");
    return -1;
  }

  ret = RK_MPI_VPSS_CreateGrp(s32VpssGrp, pstVpssGrpAttr);
  if (ret) {
    RK_MPI_VPSS_DestroyGrp(s32VpssGrp);
    RKADK_LOGE("RK_MPI_VPSS_CreateGrp(grp:%d) failed with %#x",s32VpssGrp, ret);
    return ret;
  }

  ret = RK_MPI_VPSS_StartGrp(s32VpssGrp);
  if (ret) {
    RK_MPI_VPSS_StopGrp(s32VpssGrp);
    RK_MPI_VPSS_DestroyGrp(s32VpssGrp);
    RKADK_LOGE("RK_MPI_VPSS_StartGrp failed with %#x", ret);
    return ret;
  }

  ret = RK_MPI_VPSS_ResetGrp(s32VpssGrp);
  if (ret) {
    RKADK_LOGE("RK_MPI_VPSS_ResetGrp failed with %#x", ret);
  }

  ret = RK_MPI_VPSS_SetVProcDev(s32VpssGrp, (VIDEO_PROC_DEV_TYPE_E)pstCommCfg->vpss_devcie);
  if (ret) {
    RKADK_LOGE("RK_MPI_VPSS_SetVProcDev(grp:%d) failed with %#x!", s32VpssGrp, ret);
    RK_MPI_VPSS_StopGrp(s32VpssGrp);
    RK_MPI_VPSS_DestroyGrp(s32VpssGrp);
    return ret;
  }

  g_bVpssGrpInit[s32VpssGrp] = true;
  return 0;
}

static RKADK_U32 RKADK_MPI_VPSS_DestroyGrp(RKADK_S32 s32VpssGrp) {
  int ret;
  if (!g_bVpssGrpInit[s32VpssGrp])
    return 0;

  ret = RK_MPI_VPSS_StopGrp(s32VpssGrp);
  if (ret) {
    RKADK_LOGE("RK_MPI_VPSS_StopGrp[%d] failed[%x]", s32VpssGrp, ret);
  }

  ret = RK_MPI_VPSS_DestroyGrp(s32VpssGrp);
  if (ret) {
    RKADK_LOGE("RK_MPI_VPSS_DestroyGrp[%d] failed[%x]", s32VpssGrp, ret);
    return ret;
  }

  g_bVpssGrpInit[s32VpssGrp] = false;
  return 0;
}

static RKADK_U32 RKADK_MEDIA_FindUsableIdx(RKADK_MEDIA_INFO_S *pstInfo,
                                           int count, const char *mode) {
  for (int i = 0; i < count; i++) {
    if (!pstInfo[i].bUsed) {
      RKADK_LOGD("%s: find usable index[%d]", mode, i);
      return i;
    }
  }

  return -1;
}

static RKADK_S32 RKADK_MEDIA_GetIdx(RKADK_MEDIA_INFO_S *pstInfo, int count,
                                    RKADK_S32 s32DevId, RKADK_S32 s32ChnId,
                                    const char *mode) {
  bool bMatch = false;

  for (int i = 0; i < count; i++) {
    if (!pstInfo[i].bUsed)
      continue;

    if (!strcmp(mode, "VI_INIT") || !strcmp(mode, "VI_DEINIT") ||
        !strcmp(mode, "AI_INIT") || !strcmp(mode, "AI_DEINIT") ||
        !strcmp(mode, "VPSS_INIT") || !strcmp(mode, "VPSS_DEINIT"))
      bMatch =
          pstInfo[i].s32ChnId == s32ChnId && pstInfo[i].s32DevId == s32DevId;
    else
      bMatch = pstInfo[i].s32ChnId == s32ChnId;

    if (bMatch) {
      RKADK_LOGD("%s: find matched index[%d] s32DevId[%d] ChnId[%d]", mode, i,
                 s32DevId, s32ChnId);
      return i;
    }
  }

  return -1;
}

RKADK_S32 RKADK_MPI_SYS_Init() {
  int ret = 0;

  if (!g_bSysInit) {
    RKADK_VERSION_Dump();
    RKADK_MEDIA_CtxInit();
    ret = RK_MPI_SYS_Init();
    if (ret) {
      RKADK_LOGE("RK_MPI_SYS_Init failed[%d]", ret);
      return ret;
    }
    g_bSysInit = true;
  } else {
    RKADK_LOGI("System has been initialized");
  }

  RKADK_LOGI("System initialization succeeded");
  return 0;
}

RKADK_S32 RKADK_MPI_SYS_Exit() {
  int ret;

  if (g_bSysInit) {
    ret = RK_MPI_SYS_Exit();
    if (ret) {
      RKADK_LOGE("RK_MPI_SYS_Exit failed[%d]", ret);
      return ret;
    }
    g_bSysInit = false;
  } else {
    RKADK_LOGI("System has been deinitialized");
  }

  RKADK_LOGI("System deinitialization succeeded");
  return 0;
}

bool RKADK_MPI_SYS_CHECK() {
  return g_bSysInit;
}

#if 0
static RKADK_S32 RKADK_MPI_AI_EnableVqe(RKADK_S32 s32AiChnId,
                                        RK_U32 u32SampleRate,
                                        RKADK_VQE_MODE_E enMode) {
  int ret;
  AI_TALKVQE_CONFIG_S stAiVqeTalkAttr;
  AI_RECORDVQE_CONFIG_S stAiVqeRecordAttr;

  switch (enMode) {
  case RKADK_VQE_MODE_AI_TALK:
    memset(&stAiVqeTalkAttr, 0, sizeof(AI_TALKVQE_CONFIG_S));
    stAiVqeTalkAttr.s32WorkSampleRate = u32SampleRate;
    stAiVqeTalkAttr.s32FrameSample = RKADK_VQE_FRAME_SAMPLE;
    strcpy(stAiVqeTalkAttr.aParamFilePath, RKADK_DEFAULT_VQE_FILE);
    stAiVqeTalkAttr.u32OpenMask =
        AI_TALKVQE_MASK_AEC | AI_TALKVQE_MASK_ANR | AI_TALKVQE_MASK_AGC;

    ret = RK_MPI_AI_SetTalkVqeAttr(s32AiChnId, &stAiVqeTalkAttr);
    if (ret) {
      RKADK_LOGE("AI[%d] SetTalkVqeAttr failed: %d", s32AiChnId, ret);
      return -1;
    }
    break;

  case RKADK_VQE_MODE_AI_RECORD:
    memset(&stAiVqeRecordAttr, 0, sizeof(AI_RECORDVQE_CONFIG_S));
    stAiVqeRecordAttr.s32WorkSampleRate = u32SampleRate;
    stAiVqeRecordAttr.s32FrameSample = RKADK_VQE_FRAME_SAMPLE;
    stAiVqeRecordAttr.stAnrConfig.fPostAddGain = 0;
    stAiVqeRecordAttr.stAnrConfig.fGmin = -20;
    stAiVqeRecordAttr.stAnrConfig.fNoiseFactor = 0.98;
    stAiVqeRecordAttr.stAnrConfig.enHpfSwitch = 0;
    stAiVqeRecordAttr.stAnrConfig.fHpfFc = 100.0f;
    stAiVqeRecordAttr.stAnrConfig.enLpfSwitch = 0;
    stAiVqeRecordAttr.stAnrConfig.fLpfFc = 10000.0f;
    stAiVqeRecordAttr.u32OpenMask = AI_RECORDVQE_MASK_ANR;

    ret = RK_MPI_AI_SetRecordVqeAttr(s32AiChnId, &stAiVqeRecordAttr);
    if (ret) {
      RKADK_LOGE("AI[%d] SetRecordVqeAttr failed: %d", s32AiChnId, ret);
      return -1;
    }
    break;

  default:
    RKADK_LOGW("NonSupport enMode: %d", enMode);
    return -1;
  }

  ret = RK_MPI_AI_EnableVqe(s32AiChnId);
  if (ret) {
    RKADK_LOGE("AI[%d] EnableVqe failed: %d", s32AiChnId, ret);
    return -1;
  }

  return 0;
}
#endif

RKADK_S32  RKADK_MPI_AI_Init(AUDIO_DEV aiDevId, RKADK_S32 s32AiChnId,
                            AIO_ATTR_S *pstAiAttr, RKADK_VQE_MODE_E enMode,
                            RKADK_U32 micType) {
  int ret = -1;
  RKADK_S32 i;
  RKADK_U32 s32SetTrackMode = 0;
  AI_CHN_PARAM_S pstParams;
  memset(&pstParams, 0, sizeof(AI_CHN_PARAM_S));
  pstParams.enLoopbackMode = AUDIO_LOOPBACK_NONE;
  pstParams.s32UsrFrmDepth = 1;

  RKADK_CHECK_POINTER(pstAiAttr, RKADK_FAILURE);

  RKADK_MUTEX_LOCK(g_stMediaCtx.aiMutex);

  i = RKADK_MEDIA_GetIdx(g_stMediaCtx.stAiInfo, RKADK_MEDIA_AI_MAX_CNT, aiDevId,
                         s32AiChnId, "AI_INIT");
  if (i < 0) {
    i = RKADK_MEDIA_FindUsableIdx(g_stMediaCtx.stAiInfo, RKADK_MEDIA_AI_MAX_CNT,
                                  "AI_INIT");
    if (i < 0) {
      RKADK_LOGE("not find usable index");
      goto exit;
    }
  }

  if (0 == g_stMediaCtx.stAiInfo[i].s32InitCnt) {
    ret = RK_MPI_AI_SetPubAttr(aiDevId, pstAiAttr);
    if (ret != 0) {
      RKADK_LOGE("AI[%d] set attr failed[%x]", aiDevId, ret);
      goto exit;
    }

    ret = RK_MPI_AI_Enable(aiDevId);
    if (ret != 0) {
      RKADK_LOGE("AI[%d] enable failed[%x]", aiDevId, ret);
      goto exit;
    }

    ret = RK_MPI_AI_SetChnParam(aiDevId, s32AiChnId, &pstParams);
    if (ret != 0) {
      RKADK_LOGE("AI[%d] enable chn param failed[%x]", aiDevId, ret);
      goto exit;
    }

    ret = RK_MPI_AI_EnableChn(aiDevId, s32AiChnId);
    if (ret) {
      RKADK_LOGE("AI[%d, %d] enable chn failed[%x]", aiDevId, s32AiChnId, ret);
      goto exit;
    }

    if (pstAiAttr->u32ChnCnt == 2) {
      if (micType == RKADK_MIC_TYPE_LEFT) {
        s32SetTrackMode = AUDIO_TRACK_BOTH_LEFT;
      } else if (micType == RKADK_MIC_TYPE_RIGHT) {
        s32SetTrackMode = AUDIO_TRACK_BOTH_RIGHT;
      } else if (micType == RKADK_MIC_TYPE_BOTH) {
        s32SetTrackMode = AUDIO_TRACK_NORMAL;
      } else {
        RKADK_LOGE("AI channel = %d, mic type = %d not support", pstAiAttr->u32ChnCnt, micType);
      }
    } else if (pstAiAttr->u32ChnCnt == 1) {
      if (micType == RKADK_MIC_TYPE_LEFT) {
        s32SetTrackMode = AUDIO_TRACK_FRONT_LEFT;
      } else if (micType == RKADK_MIC_TYPE_RIGHT) {
        s32SetTrackMode = AUDIO_TRACK_FRONT_RIGHT;
      } else {
        RKADK_LOGE("AI channel = %d, mic type = %d not support", pstAiAttr->u32ChnCnt, micType);
      }
    } else {
      RKADK_LOGE("AI channel = %d, mic type = %d not support", pstAiAttr->u32ChnCnt, micType);
    }

    ret = RK_MPI_AI_SetTrackMode(aiDevId, (AUDIO_TRACK_MODE_E)s32SetTrackMode);
    if (ret) {
      RKADK_LOGE("AI[%d, %d] mic type[%d] enable failed[%x]", aiDevId, s32AiChnId, micType, ret);
    }

#if 0
    if (enMode != RKADK_VQE_MODE_BUTT)
      RKADK_MPI_AI_EnableVqe(s32AiChnId, pstAiChnAttr->u32SampleRate, enMode);
#endif

    g_stMediaCtx.stAiInfo[i].bUsed = true;
    g_stMediaCtx.stAiInfo[i].s32ChnId = s32AiChnId;
    g_stMediaCtx.stAiInfo[i].s32DevId = aiDevId;
  }

  g_stMediaCtx.stAiInfo[i].s32InitCnt++;
  RKADK_LOGD("aiChnId[%d], InitCnt[%d]", s32AiChnId,
             g_stMediaCtx.stAiInfo[i].s32InitCnt);
  ret = 0;

exit:
  RKADK_MUTEX_UNLOCK(g_stMediaCtx.aiMutex);
  return ret;
}

AUDIO_SOUND_MODE_E RKADK_AI_GetSoundMode(RKADK_U32 ch) {
  AUDIO_SOUND_MODE_E channel = AUDIO_SOUND_MODE_BUTT;
  switch (ch) {
  case 1:
    channel = AUDIO_SOUND_MODE_MONO;
    break;
  case 2:
    channel = AUDIO_SOUND_MODE_STEREO;
    break;
  default:
    RKADK_LOGE("channel = %d not support", ch);
    return AUDIO_SOUND_MODE_BUTT;
  }

  return channel;
}

RKADK_S32 RKADK_MPI_AI_DeInit(AUDIO_DEV aiDevId, RKADK_S32 s32AiChnId,
                              RKADK_VQE_MODE_E enMode) {
  int ret = -1;
  RKADK_S32 i;
  RKADK_S32 s32InitCnt;

  RKADK_MUTEX_LOCK(g_stMediaCtx.aiMutex);

  i = RKADK_MEDIA_GetIdx(g_stMediaCtx.stAiInfo, RKADK_MEDIA_AI_MAX_CNT, aiDevId,
                         s32AiChnId, "AI_DEINIT");
  if (i < 0) {
    RKADK_LOGE("not find matched index[%d] s32AiChnId[%d]", i, s32AiChnId);
    goto exit;
  }

  s32InitCnt = g_stMediaCtx.stAiInfo[i].s32InitCnt;
  if (0 == s32InitCnt) {
    RKADK_LOGD("aiChnId[%d] has already deinit", s32AiChnId);
    RKADK_MUTEX_UNLOCK(g_stMediaCtx.aiMutex);
    return 0;
  } else if (1 == s32InitCnt) {
#if 0
    if (enMode != RKADK_VQE_MODE_BUTT) {
      ret = RK_MPI_AI_DisableVqe(s32AiChnId);
      if (ret)
        RKADK_LOGE("AI[%d] DisableVqe failed: %d", s32AiChnId, ret);
    }
#endif

    ret = RK_MPI_AI_DisableChn(aiDevId, s32AiChnId);
    if (ret) {
      RKADK_LOGE("AI[%d, %d] disable chn failed[%x]", aiDevId, s32AiChnId, ret);
      goto exit;
    }

    ret = RK_MPI_AI_Disable(aiDevId);
    if (ret != 0) {
      RK_LOGE("Ai[%d] disable failed[%x]", aiDevId, ret);
      return RK_FAILURE;
    }
    g_stMediaCtx.stAiInfo[i].bUsed = false;
    g_stMediaCtx.stAiInfo[i].s32ChnId = -1;
    g_stMediaCtx.stAiInfo[i].s32DevId = -1;
  }

  g_stMediaCtx.stAiInfo[i].s32InitCnt--;
  RKADK_LOGD("aiChnId[%d], InitCnt[%d]", s32AiChnId,
             g_stMediaCtx.stAiInfo[i].s32InitCnt);
  ret = 0;

exit:
  RKADK_MUTEX_UNLOCK(g_stMediaCtx.aiMutex);
  return ret;
}

RKADK_S32 RKADK_MPI_AENC_Init(RKADK_S32 s32AencChnId,
                              AENC_CHN_ATTR_S *pstAencChnAttr) {
  int ret = -1;
  RKADK_S32 i;

  RKADK_CHECK_POINTER(pstAencChnAttr, RKADK_FAILURE);

  RKADK_MUTEX_LOCK(g_stMediaCtx.aencMutex);

  i = RKADK_MEDIA_GetIdx(g_stMediaCtx.stAencInfo, RKADK_MEDIA_AENC_MAX_CNT, 0,
                         s32AencChnId, "AENC_INIT");
  if (i < 0) {
    i = RKADK_MEDIA_FindUsableIdx(g_stMediaCtx.stAencInfo,
                                  RKADK_MEDIA_AENC_MAX_CNT, "AENC_INIT");
    if (i < 0) {
      RKADK_LOGE("not find usable index");
      goto exit;
    }
  } else {
    if (g_stMediaCtx.stAencInfo[i].enCodecType != pstAencChnAttr->enType) {
      RKADK_LOGW("find matched index[%d], but CodecType inequality[%d, %d]", i,
                 g_stMediaCtx.stAencInfo[i].enCodecType,
                 pstAencChnAttr->enType);
    }
  }

  if (0 == g_stMediaCtx.stAencInfo[i].s32InitCnt) {
    ret = RK_MPI_AENC_CreateChn(s32AencChnId, pstAencChnAttr);
    if (ret) {
      RKADK_LOGE("Create AENC[%d] failed(%d)", s32AencChnId, ret);
      goto exit;
    }

    RK_MPI_AENC_SetMute(s32AencChnId, (RK_BOOL)pstAencChnAttr->stCodecAttr.u32Resv[3]);

    g_stMediaCtx.stAencInfo[i].bUsed = true;
    g_stMediaCtx.stAencInfo[i].s32ChnId = s32AencChnId;
    g_stMediaCtx.stAencInfo[i].enCodecType = pstAencChnAttr->enType;
  }

  g_stMediaCtx.stAencInfo[i].s32InitCnt++;
  RKADK_LOGD("aencChnId[%d], InitCnt[%d]", s32AencChnId,
             g_stMediaCtx.stAencInfo[i].s32InitCnt);
  ret = 0;

exit:
  RKADK_MUTEX_UNLOCK(g_stMediaCtx.aencMutex);
  return ret;
}

RKADK_S32 RKADK_MPI_AENC_DeInit(RKADK_S32 s32AencChnId) {
  int ret = -1;
  RKADK_S32 i;
  RKADK_S32 s32InitCnt;

  RKADK_MUTEX_LOCK(g_stMediaCtx.aencMutex);

  i = RKADK_MEDIA_GetIdx(g_stMediaCtx.stAencInfo, RKADK_MEDIA_AENC_MAX_CNT, 0,
                         s32AencChnId, "AENC_DEINIT");
  if (i < 0) {
    RKADK_LOGE("not find matched index[%d] s32AencChnId[%d]", i, s32AencChnId);
    goto exit;
  }

  s32InitCnt = g_stMediaCtx.stAencInfo[i].s32InitCnt;
  if (0 == s32InitCnt) {
    RKADK_LOGD("aencChnId[%d] has already deinit", s32AencChnId);
    RKADK_MUTEX_UNLOCK(g_stMediaCtx.aencMutex);
    return 0;
  } else if (1 == s32InitCnt) {
    ret = RK_MPI_AENC_DestroyChn(s32AencChnId);
    if (ret) {
      RKADK_LOGE("Destroy AENC[%d] error %d", s32AencChnId, ret);
      goto exit;
    }

    g_stMediaCtx.stAencInfo[i].bUsed = false;
    g_stMediaCtx.stAencInfo[i].s32ChnId = -1;
  }

  g_stMediaCtx.stAencInfo[i].s32InitCnt--;
  RKADK_LOGD("aencChnId[%d], InitCnt[%d]", s32AencChnId,
             g_stMediaCtx.stAencInfo[i].s32InitCnt);
  ret = 0;

exit:
  RKADK_MUTEX_UNLOCK(g_stMediaCtx.aencMutex);
  return ret;
}

RKADK_S32 RKADK_MPI_VI_Init(RKADK_U32 u32CamId, RKADK_S32 s32ViChnId,
                            VI_CHN_ATTR_S *pstViChnAttr) {
  int ret = -1;
  RKADK_S32 i;
  VI_DEV_ATTR_S stDevAttr;
  VI_DEV_BIND_PIPE_S stBindPipe;
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg;

  pstSensorCfg = RKADK_PARAM_GetSensorCfg(u32CamId);
  if (!pstSensorCfg) {
    RKADK_LOGE("RKADK_PARAM_GetSensorCfg failed");
    return -1;
  }

  memset(&stDevAttr, 0, sizeof(stDevAttr));
  memset(&stBindPipe, 0, sizeof(stBindPipe));

  RKADK_CHECK_POINTER(pstViChnAttr, RKADK_FAILURE);

  RKADK_MUTEX_LOCK(g_stMediaCtx.viMutex);

  i = RKADK_MEDIA_GetIdx(g_stMediaCtx.stViInfo, RKADK_MEDIA_VI_MAX_CNT,
                         u32CamId, s32ViChnId, "VI_INIT");
  if (i < 0) {
    i = RKADK_MEDIA_FindUsableIdx(g_stMediaCtx.stViInfo, RKADK_MEDIA_VI_MAX_CNT,
                                  "VI_INIT");
    if (i < 0) {
      RKADK_LOGE("not find usable index");
      goto exit;
    }
  }

  if (0 == g_stMediaCtx.stViInfo[i].s32InitCnt) {
    // 0. get dev config status
    ret = RK_MPI_VI_GetDevAttr(u32CamId, &stDevAttr);
    if (ret == RK_ERR_VI_NOT_CONFIG) {
      // 0-1.config dev
      ret = RK_MPI_VI_SetDevAttr(u32CamId, &stDevAttr);
      if (ret != RK_SUCCESS) {
        RKADK_LOGE("RK_MPI_VI_SetDevAttr failed[%x]", ret);
        goto exit;
      }
    } else {
      RKADK_LOGI("RK_MPI_VI_SetDevAttr already");
    }

    // 1.get dev enable status
    ret = RK_MPI_VI_GetDevIsEnable(u32CamId);
    if (ret != RK_SUCCESS) {
      // 1-2.enable dev
      ret = RK_MPI_VI_EnableDev(u32CamId);
      if (ret != RK_SUCCESS) {
        RKADK_LOGE("RK_MPI_VI_EnableDev failed[%x]", ret);
        goto exit;
      }

      // 1-3.bind dev/pipe
      stBindPipe.u32Num = 1;
      stBindPipe.PipeId[0] = u32CamId;
      ret = RK_MPI_VI_SetDevBindPipe(u32CamId, &stBindPipe);
      if (ret != RK_SUCCESS) {
        RKADK_LOGE("RK_MPI_VI_SetDevBindPipe failed[%x]", ret);
        RK_MPI_VI_DisableDev(u32CamId);
        goto exit;
      }
    } else {
      RKADK_LOGI("RK_MPI_VI_EnableDev already");
    }

    ret = RK_MPI_VI_SetChnAttr(u32CamId, s32ViChnId, pstViChnAttr);
    if (ret) {
      RKADK_LOGE("Set VI[%d] attribute failed[%x]", s32ViChnId, ret);
      RK_MPI_VI_DisableDev(u32CamId);
      goto exit;
    }

    // set wrap mode attr
    if (s32ViChnId == 0 && pstSensorCfg->enable_wrap) {
      VI_CHN_BUF_WRAP_S stViWrap;
      memset(&stViWrap, 0, sizeof(VI_CHN_BUF_WRAP_S));
      stViWrap.bEnable           = RK_TRUE;
      stViWrap.u32BufLine        = pstSensorCfg->wrap_buf_line;
      stViWrap.u32WrapBufferSize = pstViChnAttr->stSize.u32Height * pstViChnAttr->stSize.u32Width * 3 / 2;  // nv12 (w * wrapLine *3 / 2)
      RKADK_LOGE("set channel wrap line: %d, wrapBuffSize = %d", stViWrap.u32BufLine, stViWrap.u32WrapBufferSize);
      RK_MPI_VI_SetChnWrapBufAttr(u32CamId, s32ViChnId, &stViWrap);
    }

    ret = RK_MPI_VI_EnableChn(u32CamId, s32ViChnId);
    if (ret) {
      RKADK_LOGE("Create VI[%d] failed[%x]", s32ViChnId, ret);
      RK_MPI_VI_DisableDev(u32CamId);
      goto exit;
    }

    g_stMediaCtx.stViInfo[i].bUsed = true;
    g_stMediaCtx.stViInfo[i].s32ChnId = s32ViChnId;
    g_stMediaCtx.stViInfo[i].s32DevId = u32CamId;
  }

  g_stMediaCtx.stViInfo[i].s32InitCnt++;
  RKADK_LOGD("CamId[%d] viChnId[%d], InitCnt[%d]", u32CamId, s32ViChnId,
             g_stMediaCtx.stViInfo[i].s32InitCnt);
  ret = 0;

exit:
  RKADK_MUTEX_UNLOCK(g_stMediaCtx.viMutex);
  return ret;
}

RKADK_S32 RKADK_MPI_VI_DeInit(RKADK_U32 u32CamId, RKADK_S32 s32ViChnId) {
  int ret = -1;
  RKADK_S32 i, j;
  RKADK_S32 s32InitCnt;

  RKADK_MUTEX_LOCK(g_stMediaCtx.viMutex);

  i = RKADK_MEDIA_GetIdx(g_stMediaCtx.stViInfo, RKADK_MEDIA_VI_MAX_CNT,
                         u32CamId, s32ViChnId, "VI_DEINIT");
  if (i < 0) {
    RKADK_LOGE("not find matched index[%d] s32ChnId[%d]", i, s32ViChnId);
    goto exit;
  }

  s32InitCnt = g_stMediaCtx.stViInfo[i].s32InitCnt;
  if (0 == s32InitCnt) {
    RKADK_LOGD("viChnId[%d] has already deinit", s32ViChnId);
    RKADK_MUTEX_UNLOCK(g_stMediaCtx.viMutex);
    return 0;
  } else if (1 == s32InitCnt) {
    ret = RK_MPI_VI_DisableChn(u32CamId, s32ViChnId);
    if (ret) {
      RKADK_LOGE("Destory VI[%d] failed[%x]", s32ViChnId, ret);
      goto exit;
    }

    for (j = 0; j < RKADK_MEDIA_VI_MAX_CNT; j++) {
      if (g_stMediaCtx.stViInfo[j].s32DevId == (RKADK_S32)u32CamId) {
        if (j == i)
          continue;
        if (g_stMediaCtx.stViInfo[j].s32InitCnt > 0)
          break;
      }
    }

    if (j == RKADK_MEDIA_VI_MAX_CNT) {
      ret = RK_MPI_VI_DisableDev(u32CamId);
      if (ret) {
        RKADK_LOGE("RK_MPI_VI_DisableDev[%d] failed[%x]", u32CamId, ret);
        goto exit;
      }
    }

    g_stMediaCtx.stViInfo[i].bUsed = false;
    g_stMediaCtx.stViInfo[i].s32ChnId = -1;
    g_stMediaCtx.stViInfo[i].s32DevId = -1;
  }

  g_stMediaCtx.stViInfo[i].s32InitCnt--;
  RKADK_LOGD("CamId[%d] viChnId[%d], InitCnt[%d]", u32CamId, s32ViChnId,
             g_stMediaCtx.stViInfo[i].s32InitCnt);
  ret = 0;

exit:
  RKADK_MUTEX_UNLOCK(g_stMediaCtx.viMutex);
  return ret;
}

RKADK_S32 RKADK_MPI_VI_QueryCameraStatus(RKADK_U32 u32CamId) {
  int ret;
  VI_DEV_STATUS_S stDevStatus;

  if (!RKADK_MPI_SYS_CHECK()) {
    RKADK_LOGE("System is not initialized");
    return -1;
  }

  memset(&stDevStatus, 0, sizeof(VI_DEV_STATUS_S));

  ret = RK_MPI_VI_QueryDevStatus(u32CamId, &stDevStatus);
  if (ret == RK_SUCCESS) {
    if (!stDevStatus.bProbeOk) {
      RKADK_LOGE("Camera sensor probe fail");
      return -1;
    }
  }

  return ret;
}

RKADK_S32 RKADK_MPI_VENC_Init(RKADK_U32 u32CamId, RKADK_S32 s32ChnId,
                              VENC_CHN_ATTR_S *pstVencChnAttr) {
  int ret = -1;
  RKADK_S32 i;
  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg;

  pstSensorCfg = RKADK_PARAM_GetSensorCfg(u32CamId);
  if (!pstSensorCfg) {
    RKADK_LOGE("RKADK_PARAM_GetSensorCfg failed");
    return -1;
  }

  RKADK_CHECK_POINTER(pstVencChnAttr, RKADK_FAILURE);

  RKADK_MUTEX_LOCK(g_stMediaCtx.vencMutex);

  i = RKADK_MEDIA_GetIdx(g_stMediaCtx.stVencInfo, RKADK_MEDIA_VENC_MAX_CNT, 0,
                         s32ChnId, "VENC_INIT");
  if (i < 0) {
    i = RKADK_MEDIA_FindUsableIdx(g_stMediaCtx.stVencInfo,
                                  RKADK_MEDIA_VENC_MAX_CNT, "VENC_INIT");
    if (i < 0) {
      RKADK_LOGE("not find usable index");
      goto exit;
    }
  } else {
    if (g_stMediaCtx.stVencInfo[i].enCodecType !=
        pstVencChnAttr->stVencAttr.enType) {
      RKADK_LOGW("find matched index[%d], but CodecType inequality[%d, %d]", i,
                 g_stMediaCtx.stVencInfo[i].enCodecType,
                 pstVencChnAttr->stVencAttr.enType);
    }
  }

  if (0 == g_stMediaCtx.stVencInfo[i].s32InitCnt) {
    ret = RK_MPI_VENC_CreateChn(s32ChnId, pstVencChnAttr);
    if (ret) {
      RKADK_LOGE("Create VENC[%d] failed[%x]", s32ChnId, ret);
      goto exit;
    }

    if (s32ChnId == 0 && pstSensorCfg->enable_wrap) {
      VENC_CHN_BUF_WRAP_S stVencChnBufWrap;
      memset(&stVencChnBufWrap, 0, sizeof(VENC_CHN_BUF_WRAP_S));
      stVencChnBufWrap.bEnable = RK_TRUE;
      stVencChnBufWrap.u32BufLine = pstSensorCfg->wrap_buf_line;
      RKADK_LOGE("set venc channel wrap line: %d", stVencChnBufWrap.u32BufLine);
      RK_MPI_VENC_SetChnBufWrapAttr(s32ChnId, &stVencChnBufWrap);
    }

    VENC_CHN_REF_BUF_SHARE_S stVencChnRefBufShare;
    memset(&stVencChnRefBufShare, 0, sizeof(VENC_CHN_REF_BUF_SHARE_S));
    stVencChnRefBufShare.bEnable = RK_TRUE;
    RK_MPI_VENC_SetChnRefBufShareAttr(s32ChnId, &stVencChnRefBufShare);

    // VENC_RECV_PIC_PARAM_S stRecvParam;
    // memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
    // ret = RK_MPI_VENC_StartRecvFrame(s32ChnId, &stRecvParam);
    // if (ret) {
    //  RKADK_LOGE("RK_MPI_VENC_StartRecvFrame ChnId[%d] failed[%x]", s32ChnId,
    //  ret); goto exit;
    //}

    g_stMediaCtx.stVencInfo[i].bUsed = true;
    g_stMediaCtx.stVencInfo[i].s32ChnId = s32ChnId;
    g_stMediaCtx.stVencInfo[i].enCodecType = pstVencChnAttr->stVencAttr.enType;
  }

  g_stMediaCtx.stVencInfo[i].s32InitCnt++;
  RKADK_LOGD("vencChnId[%d], InitCnt[%d]", s32ChnId,
             g_stMediaCtx.stVencInfo[i].s32InitCnt);
  ret = 0;

exit:
  RKADK_MUTEX_UNLOCK(g_stMediaCtx.vencMutex);
  return ret;
}

RKADK_S32 RKADK_MPI_VENC_DeInit(RKADK_S32 s32ChnId) {
  int ret = -1;
  RKADK_S32 i;
  RKADK_S32 s32InitCnt;

  RKADK_MUTEX_LOCK(g_stMediaCtx.vencMutex);

  i = RKADK_MEDIA_GetIdx(g_stMediaCtx.stVencInfo, RKADK_MEDIA_VENC_MAX_CNT, 0,
                         s32ChnId, "VENC_DEINIT");
  if (i < 0) {
    RKADK_LOGE("not find matched index[%d] s32ChnId[%d]", i, s32ChnId);
    goto exit;
  }

  s32InitCnt = g_stMediaCtx.stVencInfo[i].s32InitCnt;
  if (0 == s32InitCnt) {
    RKADK_LOGD("vencChnId[%d] has already deinit", s32ChnId);
    RKADK_MUTEX_UNLOCK(g_stMediaCtx.vencMutex);
    return 0;
  } else if (1 == s32InitCnt) {
    // ret = RK_MPI_VENC_StopRecvFrame(s32ChnId);
    // if (ret) {
    //  RKADK_LOGE("RK_MPI_VENC_StopRecvFrame VENC[%d] failed[%x]", s32ChnId,
    //  ret); goto exit;
    //}

    ret = RK_MPI_VENC_DestroyChn(s32ChnId);
    if (ret) {
      RKADK_LOGE("Destroy VENC[%d] failed[%x]", s32ChnId, ret);
      goto exit;
    }

    g_stMediaCtx.stVencInfo[i].bUsed = false;
    g_stMediaCtx.stVencInfo[i].s32ChnId = -1;
  }

  g_stMediaCtx.stVencInfo[i].s32InitCnt--;
  RKADK_LOGD("vencChnId[%d], InitCnt[%d]", s32ChnId,
             g_stMediaCtx.stVencInfo[i].s32InitCnt);
  ret = 0;

exit:
  RKADK_MUTEX_UNLOCK(g_stMediaCtx.vencMutex);
  return ret;
}

RKADK_S32 RKADK_MPI_VPSS_Init(RKADK_S32 s32VpssGrp, RKADK_S32 s32VpssChn,
                              VPSS_GRP_ATTR_S *pstVpssGrpAttr,
                              VPSS_CHN_ATTR_S *pstVpssChnAttr) {
  int ret = -1;
  RKADK_S32 i;

  RKADK_CHECK_POINTER(pstVpssGrpAttr, RKADK_FAILURE);
  RKADK_CHECK_POINTER(pstVpssChnAttr, RKADK_FAILURE);

  RKADK_MUTEX_LOCK(g_stMediaCtx.vpssMutex);

  i = RKADK_MEDIA_GetIdx(g_stMediaCtx.stVpssInfo, RKADK_MEDIA_VPSS_MAX_CNT,
                         s32VpssGrp, s32VpssChn, "VPSS_INIT");
  if (i < 0) {
    i = RKADK_MEDIA_FindUsableIdx(g_stMediaCtx.stVpssInfo, RKADK_MEDIA_VPSS_MAX_CNT,
                                  "VPSS_INIT");
    if (i < 0) {
      RKADK_LOGE("not find usable index");
      goto exit;
    }
  }

  if (0 == g_stMediaCtx.stVpssInfo[i].s32InitCnt) {
    ret = RKADK_MPI_VPSS_CreateGrp(s32VpssGrp, pstVpssGrpAttr);
    if (ret) {
      RKADK_LOGE("RKADK_MPI_VPSS_CreateGrp failed [%d]", ret);
      goto exit;
    }

    ret = RK_MPI_VPSS_SetChnAttr(s32VpssGrp, s32VpssChn, pstVpssChnAttr);
    if (ret) {
      RKADK_LOGE("RK_MPI_VPSS_SetChnAttr failed with %#x", ret);
      goto exit;
    }

    ret = RK_MPI_VPSS_EnableChn(s32VpssGrp, s32VpssChn);
    if (ret) {
      RKADK_LOGE("RK_MPI_VPSS_EnableChn failed with %#x", ret);
      goto exit;
    }

    g_stMediaCtx.stVpssInfo[i].bUsed = true;
    g_stMediaCtx.stVpssInfo[i].s32DevId = s32VpssGrp;
    g_stMediaCtx.stVpssInfo[i].s32ChnId = s32VpssChn;
  }

  g_stMediaCtx.stVpssInfo[i].s32InitCnt++;
  RKADK_LOGD("VpssGrp[%d] VpssChn[%d], InitCnt[%d]", s32VpssGrp, s32VpssChn,
             g_stMediaCtx.stVpssInfo[i].s32InitCnt);
  ret = 0;

exit:
  RKADK_MUTEX_UNLOCK(g_stMediaCtx.vpssMutex);
  return ret;
}

RKADK_S32 RKADK_MPI_VPSS_DeInit(RKADK_S32 s32VpssGrp, RKADK_S32 s32VpssChn) {
  int ret = -1;
  RKADK_S32 i, j;
  RKADK_S32 s32InitCnt;

  RKADK_MUTEX_LOCK(g_stMediaCtx.vpssMutex);

  i = RKADK_MEDIA_GetIdx(g_stMediaCtx.stVpssInfo, RKADK_MEDIA_VPSS_MAX_CNT,
                         s32VpssGrp, s32VpssChn, "VPSS_DEINIT");
  if (i < 0) {
    RKADK_LOGE("not find matched index[%d] s32ChnId[%d]", i, s32VpssChn);
    goto exit;
  }

  s32InitCnt = g_stMediaCtx.stVpssInfo[i].s32InitCnt;
  if (0 == s32InitCnt) {
    RKADK_LOGD("VpssChn[%d] has already deinit", s32VpssChn);
    RKADK_MUTEX_UNLOCK(g_stMediaCtx.vpssMutex);
    return 0;
  } else if (1 == s32InitCnt) {
    ret = RK_MPI_VPSS_DisableChn(s32VpssGrp, s32VpssChn);
    if (ret) {
      RKADK_LOGE("Destory VPSS[%d] failed[%x]", s32VpssChn, ret);
      goto exit;
    }

    for (j = 0; j < RKADK_MEDIA_VPSS_MAX_CNT; j++) {
      if (g_stMediaCtx.stVpssInfo[j].s32DevId == s32VpssGrp) {
        if (j == i)
          continue;
        if (g_stMediaCtx.stVpssInfo[j].s32InitCnt > 0)
          break;
      }
    }

    if (j == RKADK_MEDIA_VPSS_MAX_CNT) {
      ret = RKADK_MPI_VPSS_DestroyGrp(s32VpssGrp);
      if (ret) {
        RKADK_LOGE("RKADK_MPI_VPSS_DestroyGrp[%d] failed[%x]", s32VpssGrp, ret);
        goto exit;
      }
    }

    g_stMediaCtx.stVpssInfo[i].bUsed = false;
    g_stMediaCtx.stVpssInfo[i].s32ChnId = -1;
    g_stMediaCtx.stVpssInfo[i].s32DevId = -1;
  }

  g_stMediaCtx.stVpssInfo[i].s32InitCnt--;
  RKADK_LOGD("VpssGrp[%d] VpssChn[%d], InitCnt[%d]", s32VpssGrp, s32VpssChn,
             g_stMediaCtx.stVpssInfo[i].s32InitCnt);
  ret = 0;

exit:
  RKADK_MUTEX_UNLOCK(g_stMediaCtx.vpssMutex);
  return ret;
}

static void *RKADK_MEDIA_GetAencMb(void *params) {
  int ret;
  AUDIO_STREAM_S stFrame;

  RKADK_MEDIA_INFO_S *pstMediaInfo = (RKADK_MEDIA_INFO_S *)params;
  if (!pstMediaInfo) {
    RKADK_LOGE("Get MB thread invalid param");
    return NULL;
  }

  while (pstMediaInfo->stGetAencMBAttr.bGetBuffer) {
    ret = RK_MPI_AENC_GetStream(pstMediaInfo->s32ChnId, &stFrame, 1200);
    if (ret == RK_SUCCESS) {
      for (int i = 0; i < (int)pstMediaInfo->stGetAencMBAttr.s32GetCnt; i++)
        if (pstMediaInfo->stGetAencMBAttr.cbList[i])
          pstMediaInfo->stGetAencMBAttr.cbList[i](
              stFrame, pstMediaInfo->stGetAencMBAttr.pHandle[i]);

      ret = RK_MPI_AENC_ReleaseStream(pstMediaInfo->s32ChnId, &stFrame);
      if (ret != RK_SUCCESS)
        RKADK_LOGE("RK_MPI_AENC_ReleaseStream failed[%x]", ret);
    }
  }

  RKADK_LOGI("Exit get aenc mb thread");
  return NULL;
}

RKADK_S32 RKADK_MEDIA_GetAencBuffer(MPP_CHN_S *pstChn,
                                    RKADK_MEDIA_AENC_DATA_PROC_FUNC pfnDataCB,
                                    RKADK_VOID *pHandle) {
  int ret = -1;
  char name[256];
  RKADK_S32 i;
  RKADK_MEDIA_INFO_S *pstMediaInfo;

  RKADK_MUTEX_LOCK(g_stMediaCtx.aencMutex);

  i = RKADK_MEDIA_GetIdx(g_stMediaCtx.stAencInfo, RKADK_MEDIA_AENC_MAX_CNT, 0,
                         pstChn->s32ChnId, "AENC_GET_MB");
  if (i < 0) {
    RKADK_LOGE("not find matched index[%d] s32ChnId[%d]", i, pstChn->s32ChnId);
    goto exit;
  }

  pstMediaInfo = &g_stMediaCtx.stAencInfo[i];
  if (pstMediaInfo->s32InitCnt == 0) {
    RKADK_LOGE("aencChnId[%d] don't init", pstChn->s32ChnId);
    goto exit;
  }

  pstMediaInfo->stGetAencMBAttr
      .cbList[pstMediaInfo->stGetAencMBAttr.s32GetCnt] = pfnDataCB;
  pstMediaInfo->stGetAencMBAttr
      .pHandle[pstMediaInfo->stGetAencMBAttr.s32GetCnt] = pHandle;
  pstMediaInfo->stGetAencMBAttr.s32GetCnt++;

  if (pstMediaInfo->stGetAencMBAttr.bGetBuffer) {
    RKADK_LOGE("Get aencChnId[%d] MB thread has been created",
               pstChn->s32ChnId);
    ret = 0;
    goto exit;
  }

  pstMediaInfo->stGetAencMBAttr.bGetBuffer = true;
  ret = pthread_create(&pstMediaInfo->stGetAencMBAttr.tid, NULL,
                       RKADK_MEDIA_GetAencMb, pstMediaInfo);
  if (ret) {
    RKADK_LOGE("Create get aenc mb(%d) thread failed %d", pstChn->s32ChnId,
               ret);
    goto exit;
  }
  snprintf(name, sizeof(name), "GetAencMb_%d", pstChn->s32ChnId);
  pthread_setname_np(pstMediaInfo->stGetAencMBAttr.tid, name);

exit:
  RKADK_MUTEX_UNLOCK(g_stMediaCtx.aencMutex);
  return ret;
}

RKADK_S32
RKADK_MEDIA_StopGetAencBuffer(MPP_CHN_S *pstChn,
                              RKADK_MEDIA_AENC_DATA_PROC_FUNC pfnDataCB) {
  int i, ret = 0;
  RKADK_MEDIA_INFO_S *pstMediaInfo;

  RKADK_MUTEX_LOCK(g_stMediaCtx.aencMutex);

  i = RKADK_MEDIA_GetIdx(g_stMediaCtx.stAencInfo, RKADK_MEDIA_AENC_MAX_CNT, 0,
                         pstChn->s32ChnId, "AENC_GET_MB");
  if (i < 0) {
    RKADK_LOGE("not find matched index[%d] s32ChnId[%d]", i, pstChn->s32ChnId);
    ret = -1;
    goto exit;
  }

  pstMediaInfo = &g_stMediaCtx.stAencInfo[i];
  if (pstMediaInfo->s32InitCnt == 0) {
    RKADK_LOGE("aencChnId[%d] don't init", pstChn->s32ChnId);
    ret = -1;
    goto exit;
  }

  for (i = 0; i < pstMediaInfo->stGetAencMBAttr.s32GetCnt; i++) {
    if (pstMediaInfo->stGetAencMBAttr.cbList[i] == pfnDataCB) {
      RKADK_LOGD("remove cbList, i = %d", i);
      pstMediaInfo->stGetAencMBAttr.cbList[i] = NULL;
      pstMediaInfo->stGetAencMBAttr.s32GetCnt--;
      break;
    }
  }

  if (!pstMediaInfo->stGetAencMBAttr.s32GetCnt) {
    pstMediaInfo->stGetAencMBAttr.bGetBuffer = false;
    if (pstMediaInfo->stGetAencMBAttr.tid) {
      RKADK_LOGD("Request to cancel aenc mb thread...");
      ret = pthread_join(pstMediaInfo->stGetAencMBAttr.tid, NULL);
      if (ret)
        RKADK_LOGE("Exit get aenc mb thread failed!");
      else
        RKADK_LOGI("Exit get aenc mb thread ok");
      pstMediaInfo->stGetAencMBAttr.tid = 0;
    }
  }

exit:
  RKADK_MUTEX_UNLOCK(g_stMediaCtx.aencMutex);
  return ret;
}

static void *RKADK_MEDIA_GetVencMb(void *params) {
  int ret;
  RKADK_MEDIA_VENC_DATA_S stData;
  VENC_PACK_S stPack;

  RKADK_MEDIA_INFO_S *pstMediaInfo = (RKADK_MEDIA_INFO_S *)params;
  if (!pstMediaInfo) {
    RKADK_LOGE("Get MB thread invalid param");
    return NULL;
  }

  memset(&stData, 0, sizeof(stData));
  memset(&stPack, 0, sizeof(stPack));
  stData.stFrame.pstPack = &stPack;
  stData.u32ChnId = pstMediaInfo->s32ChnId;
  while (pstMediaInfo->stGetVencMBAttr.bGetBuffer) {
    ret = RK_MPI_VENC_GetStream(pstMediaInfo->s32ChnId, &stData.stFrame, 1200);

    if (ret == RK_SUCCESS) {
      for (int i = 0; i < (int)pstMediaInfo->stGetVencMBAttr.s32GetCnt; i++)
        if (pstMediaInfo->stGetVencMBAttr.cbList[i])
          pstMediaInfo->stGetVencMBAttr.cbList[i](
              stData, pstMediaInfo->stGetVencMBAttr.pHandle[i]);

      ret = RK_MPI_VENC_ReleaseStream(pstMediaInfo->s32ChnId, &stData.stFrame);
      if (ret != RK_SUCCESS)
        RKADK_LOGE("RK_MPI_VENC_ReleaseStream failed[%x]", ret);
    } else {
      RKADK_LOGE("RK_MPI_VENC_GetStream chn[%d] timeout[%x]", pstMediaInfo->s32ChnId, ret);
    }
  }

  RKADK_LOGI("Exit get venc mb thread");
  return NULL;
}

RKADK_S32 RKADK_MEDIA_GetVencBuffer(MPP_CHN_S *pstChn,
                                    RKADK_MEDIA_VENC_DATA_PROC_FUNC pfnDataCB,
                                    RKADK_VOID *pHandle) {
  int ret = -1;
  RKADK_S32 i;
  char name[256];
  RKADK_MEDIA_INFO_S *pstMediaInfo;

  RKADK_MUTEX_LOCK(g_stMediaCtx.vencMutex);

  i = RKADK_MEDIA_GetIdx(g_stMediaCtx.stVencInfo, RKADK_MEDIA_VENC_MAX_CNT, 0,
                         pstChn->s32ChnId, "VENC_GET_MB");
  if (i < 0) {
    RKADK_LOGE("not find matched index[%d] s32ChnId[%d]", i, pstChn->s32ChnId);
    goto exit;
  }

  pstMediaInfo = &g_stMediaCtx.stVencInfo[i];
  if (pstMediaInfo->s32InitCnt == 0) {
    RKADK_LOGE("vencChnId[%d] don't init", pstChn->s32ChnId);
    goto exit;
  }

  pstMediaInfo->stGetVencMBAttr
      .cbList[pstMediaInfo->stGetVencMBAttr.s32GetCnt] = pfnDataCB;
  pstMediaInfo->stGetVencMBAttr
      .pHandle[pstMediaInfo->stGetVencMBAttr.s32GetCnt] = pHandle;
  pstMediaInfo->stGetVencMBAttr.s32GetCnt++;

  if (pstMediaInfo->stGetVencMBAttr.bGetBuffer) {
    RKADK_LOGE("Get vencChnId[%d] MB thread has been created",
               pstChn->s32ChnId);
    ret = 0;
    goto exit;
  }

  pstMediaInfo->stGetVencMBAttr.bGetBuffer = true;
  ret = pthread_create(&pstMediaInfo->stGetVencMBAttr.tid, NULL,
                       RKADK_MEDIA_GetVencMb, pstMediaInfo);
  if (ret) {
    RKADK_LOGE("Create get venc mb(%d) thread failed %d", pstChn->s32ChnId,
               ret);
    goto exit;
  }
  snprintf(name, sizeof(name), "GetVencMb_%d", pstChn->s32ChnId);
  pthread_setname_np(pstMediaInfo->stGetVencMBAttr.tid, name);

exit:
  RKADK_MUTEX_UNLOCK(g_stMediaCtx.vencMutex);
  return ret;
}

RKADK_S32
RKADK_MEDIA_StopGetVencBuffer(MPP_CHN_S *pstChn,
                              RKADK_MEDIA_VENC_DATA_PROC_FUNC pfnDataCB) {
  int i, ret = 0;
  RKADK_MEDIA_INFO_S *pstMediaInfo;

  RKADK_MUTEX_LOCK(g_stMediaCtx.vencMutex);

  i = RKADK_MEDIA_GetIdx(g_stMediaCtx.stVencInfo, RKADK_MEDIA_VENC_MAX_CNT, 0,
                         pstChn->s32ChnId, "VENC_GET_MB");
  if (i < 0) {
    RKADK_LOGE("not find matched index[%d] s32ChnId[%d]", i, pstChn->s32ChnId);
    ret = -1;
    goto exit;
  }

  pstMediaInfo = &g_stMediaCtx.stVencInfo[i];
  if (pstMediaInfo->s32InitCnt == 0) {
    RKADK_LOGE("vencChnId[%d] don't init", pstChn->s32ChnId);
    ret = -1;
    goto exit;
  }

  for (i = 0; i < pstMediaInfo->stGetVencMBAttr.s32GetCnt; i++) {
    if (pstMediaInfo->stGetVencMBAttr.cbList[i] == pfnDataCB) {
      RKADK_LOGD("remove cbList, i = %d", i);
      pstMediaInfo->stGetVencMBAttr.cbList[i] = NULL;
      pstMediaInfo->stGetVencMBAttr.s32GetCnt--;
      break;
    }
  }

  if (!pstMediaInfo->stGetVencMBAttr.s32GetCnt) {
    pstMediaInfo->stGetVencMBAttr.bGetBuffer = false;
    if (pstMediaInfo->stGetVencMBAttr.tid) {
      RKADK_LOGE("Request to cancel venc mb thread...");
      ret = pthread_join(pstMediaInfo->stGetVencMBAttr.tid, NULL);
      if (ret)
        RKADK_LOGE("Exit get venc mb thread failed!");
      else
        RKADK_LOGI("Exit get venc mb thread ok");
      pstMediaInfo->stGetVencMBAttr.tid = 0;
    }
  }

exit:
  RKADK_MUTEX_UNLOCK(g_stMediaCtx.vencMutex);
  return ret;
}

static RKADK_U32 RKADK_BIND_FindUsableIdx(RKADK_BIND_INFO_S *pstInfo,
                                          int count) {
  for (int i = 0; i < count; i++) {
    if (!pstInfo[i].bUsed) {
      RKADK_LOGD("find usable index[%d]", i);
      return i;
    }
  }

  return -1;
}

static RKADK_S32 RKADK_BIND_GetIdx(RKADK_BIND_INFO_S *pstInfo, int count,
                                   const MPP_CHN_S *pstSrcChn,
                                   const MPP_CHN_S *pstDestChn) {
  RKADK_CHECK_POINTER(pstSrcChn, RKADK_FAILURE);
  RKADK_CHECK_POINTER(pstDestChn, RKADK_FAILURE);

  for (int i = 0; i < count; i++) {
    if (!pstInfo[i].bUsed)
      continue;

    if (pstInfo[i].stSrcChn.s32ChnId == pstSrcChn->s32ChnId &&
        pstInfo[i].stSrcChn.enModId == pstSrcChn->enModId &&
        pstInfo[i].stSrcChn.s32DevId == pstSrcChn->s32DevId &&
        pstInfo[i].stDestChn.s32ChnId == pstDestChn->s32ChnId &&
        pstInfo[i].stDestChn.enModId == pstDestChn->enModId &&
        pstInfo[i].stDestChn.s32DevId == pstDestChn->s32DevId) {
      RKADK_LOGD("find matched index[%d]: src ChnId[%d] dest ChnId[%d]", i,
                 pstSrcChn->s32ChnId, pstDestChn->s32ChnId);
      return i;
    }
  }

  return -1;
}

static RKADK_S32 RKADK_MEDIA_GetBindInfo(const MPP_CHN_S *pstSrcChn,
                                         const MPP_CHN_S *pstDestChn,
                                         RKADK_BIND_INFO_S **pstInfo) {
  RKADK_S32 s32BindCount = -1;

  if (pstSrcChn->enModId == RK_ID_AI && pstDestChn->enModId == RK_ID_AENC) {
    s32BindCount = RKADK_AI_AENC_MAX_BIND_CNT;
    *pstInfo = g_stMediaCtx.stAiAencInfo;
  } else if (pstSrcChn->enModId == RK_ID_VI &&
             pstDestChn->enModId == RK_ID_VENC) {
    s32BindCount = RKADK_VI_VENC_MAX_BIND_CNT;
    *pstInfo = g_stMediaCtx.stViVencInfo;
  } else if (pstSrcChn->enModId == RK_ID_VI && pstDestChn->enModId == RK_ID_VPSS) {
    s32BindCount = RKADK_VI_VPSS_MAX_BIND_CNT;
    *pstInfo = g_stMediaCtx.stViVpssInfo;
  } else if (pstSrcChn->enModId == RK_ID_VPSS &&
             pstDestChn->enModId == RK_ID_VENC) {
    s32BindCount = RKADK_VPSS_VENC_MAX_BIND_CNT;
    *pstInfo = g_stMediaCtx.stVpssVencInfo;
  } else {
    RKADK_LOGE("Nonsupport: src enModId: %d, dest enModId: %d",
               pstSrcChn->enModId, pstDestChn->enModId);
  }

  return s32BindCount;
}

RKADK_S32 RKADK_MPI_SYS_Bind(const MPP_CHN_S *pstSrcChn,
                             const MPP_CHN_S *pstDestChn) {
  int ret = -1, count;
  RKADK_S32 i;
  RKADK_BIND_INFO_S *pstInfo = NULL;

  RKADK_CHECK_POINTER(pstSrcChn, RKADK_FAILURE);
  RKADK_CHECK_POINTER(pstDestChn, RKADK_FAILURE);

  count = RKADK_MEDIA_GetBindInfo(pstSrcChn, pstDestChn, &pstInfo);
  if (count < 0) {
    RKADK_LOGE("RKADK_MEDIA_GetBindInfo failed");
    return -1;
  }

  RKADK_MUTEX_LOCK(g_stMediaCtx.bindMutex);

  i = RKADK_BIND_GetIdx(pstInfo, count, pstSrcChn, pstDestChn);
  if (i < 0) {
    i = RKADK_BIND_FindUsableIdx(pstInfo, count);
    if (i < 0) {
      RKADK_LOGE("not find usable index, src chn[%d], dst chn[%d]",
                  pstSrcChn->s32ChnId, pstDestChn->s32ChnId);
      goto exit;
    }
  }

  if (0 == pstInfo[i].s32BindCnt) {
    ret = RK_MPI_SYS_Bind(pstSrcChn, pstDestChn);
    if (ret) {
      RKADK_LOGE("Bind src[%d, %d, %d] and dest[%d, %d, %d] failed[%x]",
                  pstSrcChn->s32DevId, pstSrcChn->enModId, pstSrcChn->s32ChnId,
                  pstDestChn->s32DevId, pstDestChn->enModId,
                  pstDestChn->s32ChnId, ret);
      goto exit;
    }

    pstInfo[i].bUsed = true;
    memcpy(&pstInfo[i].stSrcChn, pstSrcChn, sizeof(MPP_CHN_S));
    memcpy(&pstInfo[i].stDestChn, pstDestChn, sizeof(MPP_CHN_S));
  }

  pstInfo[i].s32BindCnt++;
  RKADK_LOGD("src[%d, %d, %d], dest[%d, %d, %d], BindCnt[%d]",
              pstSrcChn->s32DevId, pstSrcChn->enModId, pstSrcChn->s32ChnId,
              pstDestChn->s32DevId, pstDestChn->enModId, pstDestChn->s32ChnId,
              pstInfo[i].s32BindCnt);
  ret = 0;

exit:
  RKADK_MUTEX_UNLOCK(g_stMediaCtx.bindMutex);
  return ret;
}

RKADK_S32 RKADK_MPI_SYS_UnBind(const MPP_CHN_S *pstSrcChn,
                               const MPP_CHN_S *pstDestChn) {
  int ret = -1, count;
  RKADK_S32 i;
  RKADK_BIND_INFO_S *pstInfo = NULL;

  RKADK_CHECK_POINTER(pstSrcChn, RKADK_FAILURE);
  RKADK_CHECK_POINTER(pstDestChn, RKADK_FAILURE);

  count = RKADK_MEDIA_GetBindInfo(pstSrcChn, pstDestChn, &pstInfo);
  if (count < 0) {
    RKADK_LOGE("RKADK_MEDIA_GetBindInfo failed");
    return -1;
  }

  RKADK_MUTEX_LOCK(g_stMediaCtx.bindMutex);

  i = RKADK_BIND_GetIdx(pstInfo, count, pstSrcChn, pstDestChn);
  if (i < 0) {
    RKADK_MUTEX_UNLOCK(g_stMediaCtx.bindMutex);
    RKADK_LOGE("not find usable index");
    return -1;
  }

  if (0 == pstInfo[i].s32BindCnt) {
    RKADK_LOGD("src[%d, %d, %d], dest[%d, %d, %d] has already UnBind",
                pstSrcChn->s32DevId, pstSrcChn->enModId, pstSrcChn->s32ChnId,
                pstDestChn->s32DevId, pstDestChn->enModId, pstDestChn->s32ChnId);
    RKADK_MUTEX_UNLOCK(g_stMediaCtx.bindMutex);
    return 0;
  } else if (1 == pstInfo[i].s32BindCnt) {
    ret = RK_MPI_SYS_UnBind(pstSrcChn, pstDestChn);
    if (ret) {
      RKADK_LOGE("UnBind src[%d, %d, %d] and dest[%d, %d, %d] failed[%x]",
                  pstSrcChn->s32DevId, pstSrcChn->enModId, pstSrcChn->s32ChnId,
                  pstDestChn->s32DevId, pstDestChn->enModId,
                  pstDestChn->s32ChnId, ret);
      goto exit;
    }

    pstInfo[i].bUsed = false;
    memset(&pstInfo[i].stSrcChn, 0, sizeof(MPP_CHN_S));
    memset(&pstInfo[i].stDestChn, 0, sizeof(MPP_CHN_S));
  }

  pstInfo[i].s32BindCnt--;
  RKADK_LOGD("src[%d, %d, %d], dest[%d, %d, %d], BindCnt[%d]",
              pstSrcChn->s32DevId, pstSrcChn->enModId, pstSrcChn->s32ChnId,
              pstDestChn->s32DevId, pstDestChn->enModId, pstDestChn->s32ChnId,
              pstInfo[i].s32BindCnt);
  ret = 0;

exit:
  RKADK_MUTEX_UNLOCK(g_stMediaCtx.bindMutex);
  return ret;
}

RK_CODEC_ID_E RKADK_MEDIA_GetRkCodecType(RKADK_CODEC_TYPE_E enType) {
  RK_CODEC_ID_E enCodecType;

  switch (enType) {
  case RKADK_CODEC_TYPE_H264:
    enCodecType = RK_VIDEO_ID_AVC;
    break;

  case RKADK_CODEC_TYPE_H265:
    enCodecType = RK_VIDEO_ID_HEVC;
    break;

  case RKADK_CODEC_TYPE_MJPEG:
    enCodecType = RK_VIDEO_ID_MJPEG;
    break;

  case RKADK_CODEC_TYPE_JPEG:
    enCodecType = RK_VIDEO_ID_JPEG;
    break;

  case RKADK_CODEC_TYPE_G711A:
    enCodecType = RK_AUDIO_ID_PCM_ALAW;
    break;

  case RKADK_CODEC_TYPE_G711U:
    enCodecType = RK_AUDIO_ID_PCM_MULAW;
    break;

  case RKADK_CODEC_TYPE_G726:
    enCodecType = RK_AUDIO_ID_ADPCM_G726;
    break;

  case RKADK_CODEC_TYPE_MP2:
    enCodecType = RK_AUDIO_ID_MP2;
    break;

  case RKADK_CODEC_TYPE_MP3:
    enCodecType = RK_AUDIO_ID_MP3;
    break;
  case RKADK_CODEC_TYPE_ACC:
    enCodecType = RK_AUDIO_ID_ACC;
    break;

  default:
    enCodecType = RK_VIDEO_ID_Unused;
    break;
  }

  return enCodecType;
}

RKADK_CODEC_TYPE_E RKADK_MEDIA_GetCodecType(RK_CODEC_ID_E enType) {
  RKADK_CODEC_TYPE_E enCodecType;

  switch (enType) {
  case RK_VIDEO_ID_AVC:
    enCodecType = RKADK_CODEC_TYPE_H264;
    break;

  case RK_VIDEO_ID_HEVC:
    enCodecType = RKADK_CODEC_TYPE_H265;
    break;

  case RK_VIDEO_ID_MJPEG:
    enCodecType = RKADK_CODEC_TYPE_MJPEG;
    break;

  case RK_VIDEO_ID_JPEG:
    enCodecType = RKADK_CODEC_TYPE_JPEG;
    break;

  case RK_AUDIO_ID_PCM_ALAW:
    enCodecType = RKADK_CODEC_TYPE_G711A;
    break;

  case RK_AUDIO_ID_PCM_MULAW:
    enCodecType = RKADK_CODEC_TYPE_G711U;
    break;

  case RK_AUDIO_ID_ADPCM_G726:
    enCodecType = RKADK_CODEC_TYPE_G726;
    break;

  case RK_AUDIO_ID_MP2:
    enCodecType = RKADK_CODEC_TYPE_MP2;
    break;

  case RK_AUDIO_ID_MP3:
    enCodecType = RKADK_CODEC_TYPE_MP3;
    break;

  default:
    enCodecType = RKADK_CODEC_TYPE_BUTT;
    break;
  }

  return enCodecType;
}

RKADK_S32 RKADK_MEDIA_SetRcAttr(VENC_RC_ATTR_S *pstRcAttr, RKADK_U32 u32Gop,
                                RKADK_U32 u32BitRate, RKADK_U32 u32SrcFrameRate,
                                RKADK_U32 u32DstFrameRate) {
  switch (pstRcAttr->enRcMode) {
  case VENC_RC_MODE_H265CBR:
    pstRcAttr->stH265Cbr.u32Gop = u32Gop;
    pstRcAttr->stH265Cbr.u32BitRate = u32BitRate / 1000;
    pstRcAttr->stH265Cbr.fr32DstFrameRateDen = 1;
    pstRcAttr->stH265Cbr.fr32DstFrameRateNum = u32DstFrameRate;
    pstRcAttr->stH265Cbr.u32SrcFrameRateDen = 1;
    pstRcAttr->stH265Cbr.u32SrcFrameRateNum = u32SrcFrameRate;
    break;
  case VENC_RC_MODE_H265VBR:
    pstRcAttr->stH265Vbr.u32Gop = u32Gop;
    pstRcAttr->stH265Vbr.u32BitRate = u32BitRate / 1000;
    pstRcAttr->stH265Vbr.fr32DstFrameRateDen = 1;
    pstRcAttr->stH265Vbr.fr32DstFrameRateNum = u32DstFrameRate;
    pstRcAttr->stH265Vbr.u32SrcFrameRateDen = 1;
    pstRcAttr->stH265Vbr.u32SrcFrameRateNum = u32SrcFrameRate;
    break;
  case VENC_RC_MODE_H265AVBR:
    pstRcAttr->stH265Avbr.u32Gop = u32Gop;
    pstRcAttr->stH265Avbr.u32BitRate = u32BitRate / 1000;
    pstRcAttr->stH265Avbr.fr32DstFrameRateDen = 1;
    pstRcAttr->stH265Avbr.fr32DstFrameRateNum = u32DstFrameRate;
    pstRcAttr->stH265Avbr.u32SrcFrameRateDen = 1;
    pstRcAttr->stH265Avbr.u32SrcFrameRateNum = u32SrcFrameRate;
    break;
  case VENC_RC_MODE_MJPEGCBR:
    pstRcAttr->stMjpegCbr.fr32DstFrameRateDen = 1;
    pstRcAttr->stMjpegCbr.fr32DstFrameRateNum = u32DstFrameRate;
    pstRcAttr->stMjpegCbr.u32SrcFrameRateDen = 1;
    pstRcAttr->stMjpegCbr.u32SrcFrameRateNum = u32SrcFrameRate;
    pstRcAttr->stMjpegCbr.u32BitRate = u32BitRate / 1000;
    break;
  case VENC_RC_MODE_MJPEGVBR:
    pstRcAttr->stMjpegVbr.fr32DstFrameRateDen = 1;
    pstRcAttr->stMjpegVbr.fr32DstFrameRateNum = u32DstFrameRate;
    pstRcAttr->stMjpegVbr.u32SrcFrameRateDen = 1;
    pstRcAttr->stMjpegVbr.u32SrcFrameRateNum = u32SrcFrameRate;
    pstRcAttr->stMjpegVbr.u32BitRate = u32BitRate / 1000;
    break;
  case VENC_RC_MODE_H264CBR:
    pstRcAttr->stH264Cbr.u32Gop = u32Gop;
    pstRcAttr->stH264Cbr.u32BitRate = u32BitRate / 1000;
    pstRcAttr->stH264Cbr.fr32DstFrameRateDen = 1;
    pstRcAttr->stH264Cbr.fr32DstFrameRateNum = u32DstFrameRate;
    pstRcAttr->stH264Cbr.u32SrcFrameRateDen = 1;
    pstRcAttr->stH264Cbr.u32SrcFrameRateNum = u32SrcFrameRate;
    break;
  case VENC_RC_MODE_H264VBR:
    pstRcAttr->stH264Vbr.u32Gop = u32Gop;
    pstRcAttr->stH264Vbr.u32BitRate = u32BitRate / 1000;
    pstRcAttr->stH264Vbr.fr32DstFrameRateDen = 1;
    pstRcAttr->stH264Vbr.fr32DstFrameRateNum = u32DstFrameRate;
    pstRcAttr->stH264Vbr.u32SrcFrameRateDen = 1;
    pstRcAttr->stH264Vbr.u32SrcFrameRateNum = u32SrcFrameRate;
    break;
  case VENC_RC_MODE_H264AVBR:
    pstRcAttr->stH264Avbr.u32Gop = u32Gop;
    pstRcAttr->stH264Avbr.u32BitRate = u32BitRate / 1000;
    pstRcAttr->stH264Avbr.fr32DstFrameRateDen = 1;
    pstRcAttr->stH264Avbr.fr32DstFrameRateNum = u32DstFrameRate;
    pstRcAttr->stH264Avbr.u32SrcFrameRateDen = 1;
    pstRcAttr->stH264Avbr.u32SrcFrameRateNum = u32SrcFrameRate;
    break;
  default:
    RKADK_LOGE("Invalid rc mode: %d", pstRcAttr->enRcMode);
    return -1;
  }

  return 0;
}

bool RKADK_MEDIA_CompareResolution(VENC_CHN_ATTR_S *pstRecAttr,
                                RKADK_U32 u32Width, RKADK_U32 u32Height) {
  RKADK_LOGD("Old width height[%d %d], new width height[%d %d]",
            pstRecAttr->stVencAttr.u32PicWidth, pstRecAttr->stVencAttr.u32PicHeight,
            u32Width, u32Height);
  if (pstRecAttr->stVencAttr.u32PicWidth == u32Width &&
      pstRecAttr->stVencAttr.u32PicHeight == u32Height)
    return false;

  return true;
}

bool RKADK_MEDIA_VencAttrCmp(VENC_CHN_ATTR_S *pstRecAttr,
                                  RK_CODEC_ID_E enType, RKADK_U32 u32DstFrameRate,
                                  RKADK_U32 u32Bitrate) {
  bool reset = false;
  RKADK_U32 u32OldFrameRate = 0, u32OldBitrate = 0;
  VENC_RC_ATTR_S *pstRcAttr = &pstRecAttr->stRcAttr;

  if (pstRecAttr->stVencAttr.enType != enType) {
    RKADK_LOGD("Old codec type [%d], new codec type [%d]",
              pstRecAttr->stVencAttr.enType, enType);
    reset = true;
  }

  u32Bitrate = u32Bitrate / 1000;
  switch (pstRcAttr->enRcMode) {
  case VENC_RC_MODE_H265CBR:
    u32OldFrameRate = pstRcAttr->stH265Cbr.fr32DstFrameRateNum;
    u32OldBitrate = pstRcAttr->stH265Cbr.u32BitRate;

    if (pstRcAttr->stH265Cbr.fr32DstFrameRateNum != u32DstFrameRate)
      reset |= true;

    if (pstRcAttr->stH265Cbr.u32BitRate != u32Bitrate)
      reset |= true;
    break;
  case VENC_RC_MODE_H265VBR:
    u32OldFrameRate = pstRcAttr->stH265Vbr.fr32DstFrameRateNum;
    u32OldBitrate = pstRcAttr->stH265Vbr.u32BitRate;

    if (pstRcAttr->stH265Vbr.fr32DstFrameRateNum != u32DstFrameRate)
      reset |= true;

    if (pstRcAttr->stH265Vbr.u32BitRate != u32Bitrate)
      reset |= true;
    break;
  case VENC_RC_MODE_H265AVBR:
    u32OldFrameRate = pstRcAttr->stH265Avbr.fr32DstFrameRateNum;
    u32OldBitrate = pstRcAttr->stH265Avbr.u32BitRate;

    if (pstRcAttr->stH265Avbr.fr32DstFrameRateNum != u32DstFrameRate)
      reset |= true;

    if (pstRcAttr->stH265Avbr.u32BitRate != u32Bitrate)
      reset |= true;
    break;
  case VENC_RC_MODE_MJPEGCBR:
    u32OldFrameRate = pstRcAttr->stMjpegCbr.fr32DstFrameRateNum;
    u32OldBitrate = pstRcAttr->stMjpegCbr.u32BitRate;

    if (pstRcAttr->stMjpegCbr.fr32DstFrameRateNum != u32DstFrameRate)
      reset |= true;

    if (pstRcAttr->stMjpegCbr.u32BitRate != u32Bitrate)
      reset |= true;
    break;
  case VENC_RC_MODE_MJPEGVBR:
    u32OldFrameRate = pstRcAttr->stMjpegVbr.fr32DstFrameRateNum;
    u32OldBitrate = pstRcAttr->stMjpegVbr.u32BitRate;

    if (pstRcAttr->stMjpegVbr.fr32DstFrameRateNum != u32DstFrameRate)
      reset |= true;

    if (pstRcAttr->stMjpegVbr.u32BitRate != u32Bitrate)
      reset |= true;
    break;
  case VENC_RC_MODE_H264CBR:
    u32OldFrameRate = pstRcAttr->stH264Cbr.fr32DstFrameRateNum;
    u32OldBitrate = pstRcAttr->stH264Cbr.u32BitRate;

    if (pstRcAttr->stH264Cbr.fr32DstFrameRateNum != u32DstFrameRate)
      reset |= true;

    if (pstRcAttr->stH264Cbr.u32BitRate != u32Bitrate)
      reset |= true;
    break;
  case VENC_RC_MODE_H264VBR:
    u32OldFrameRate = pstRcAttr->stH264Vbr.fr32DstFrameRateNum;
    u32OldBitrate = pstRcAttr->stH264Vbr.u32BitRate;

    if (pstRcAttr->stH264Vbr.fr32DstFrameRateNum != u32DstFrameRate)
      reset |= true;

    if (pstRcAttr->stH264Vbr.u32BitRate != u32Bitrate)
      reset |= true;
    break;
  case VENC_RC_MODE_H264AVBR:
    u32OldFrameRate = pstRcAttr->stH264Avbr.fr32DstFrameRateNum;
    u32OldBitrate = pstRcAttr->stH264Avbr.u32BitRate;

    if (pstRcAttr->stH264Avbr.fr32DstFrameRateNum != u32DstFrameRate)
      reset |= true;

    if (pstRcAttr->stH264Avbr.u32BitRate != u32Bitrate)
      reset |= true;
    break;
  default:
    RKADK_LOGE("Invalid rc mode: %d", pstRcAttr->enRcMode);
    break;
  }

  RKADK_LOGD("Old fps [%d], new fps [%d]", u32OldFrameRate, u32DstFrameRate);

  RKADK_LOGD("Old bitrate [%d], new bitrate [%d]", u32OldBitrate, u32Bitrate);

  return reset;
}

RKADK_S32 RKADK_MEDIA_FrameBufMalloc(RKADK_FRAME_ATTR_S *pstFrameAttr) {
  if (pstFrameAttr->pu8Buf)
    return 0;

  switch (pstFrameAttr->enType) {
  case RKADK_THUMB_TYPE_NV12:
    pstFrameAttr->u32BufSize =
        pstFrameAttr->u32VirWidth * pstFrameAttr->u32VirHeight * 3 / 2;
    break;
  case RKADK_THUMB_TYPE_JPEG:
    pstFrameAttr->u32BufSize =
        pstFrameAttr->u32VirWidth * pstFrameAttr->u32VirHeight;
    break;
  case RKADK_THUMB_TYPE_RGB565:
    pstFrameAttr->u32BufSize =
        pstFrameAttr->u32VirWidth * pstFrameAttr->u32VirHeight * 2;
    break;
  case RKADK_THUMB_TYPE_RGB888:
    pstFrameAttr->u32BufSize =
        pstFrameAttr->u32VirWidth * pstFrameAttr->u32VirHeight * 3;
    break;
  case RKADK_THUMB_TYPE_RGBA8888:
    pstFrameAttr->u32BufSize =
        pstFrameAttr->u32VirWidth * pstFrameAttr->u32VirHeight * 4;
    break;

  default:
    RKADK_LOGE("Invalid enType[%d]", pstFrameAttr->enType);
    return -1;
  }

  pstFrameAttr->pu8Buf = (RKADK_U8 *)malloc(pstFrameAttr->u32BufSize);
  if (!pstFrameAttr->pu8Buf) {
    RKADK_LOGE("malloc pu8Buf failed, u32BufSize = %d",
               pstFrameAttr->u32BufSize);
    return -1;
  }

  RKADK_LOGD("malloc frame buffer[%p][%d, %d, %d, %d], u32BufSize[%d]",
             pstFrameAttr->pu8Buf, pstFrameAttr->u32Width,
             pstFrameAttr->u32Height, pstFrameAttr->u32VirWidth,
             pstFrameAttr->u32VirHeight, pstFrameAttr->u32BufSize);
  return 0;
}

RKADK_S32 RKADK_MEDIA_FrameFree(RKADK_FRAME_ATTR_S *pstFrameAttr) {
  RKADK_CHECK_POINTER(pstFrameAttr, RKADK_FAILURE);

  if (pstFrameAttr->pu8Buf) {
    RKADK_LOGD("free buffer[%p]", pstFrameAttr->pu8Buf);
    free(pstFrameAttr->pu8Buf);
    pstFrameAttr->pu8Buf = NULL;
  }

  return 0;
}

bool RKADK_MEDIA_CheckFrameAttr(RKADK_FRAME_ATTR_S *pstFrameAttr) {

  if (pstFrameAttr->enType < RKADK_THUMB_TYPE_NV12 ||
      pstFrameAttr->enType > RKADK_THUMB_TYPE_RGBA8888) {
    RKADK_LOGE("Invalid thumb type = %d", pstFrameAttr->enType);
    return false;
  }

  if (pstFrameAttr->u32Width != UPALIGNTO(pstFrameAttr->u32Width, 4)) {
    RKADK_LOGE("Width is not 4-aligned");
    return false;
  }

  if (pstFrameAttr->u32Height != UPALIGNTO(pstFrameAttr->u32Height, 2)) {
    RKADK_LOGE("Height is not 2-aligned");
    return false;
  }

  if (pstFrameAttr->u32VirWidth != UPALIGNTO(pstFrameAttr->u32VirWidth, 4)) {
    RKADK_LOGE("VirWidth is not 4-aligned");
    return false;
  }

  if (pstFrameAttr->u32VirHeight != UPALIGNTO(pstFrameAttr->u32VirHeight, 2)) {
    RKADK_LOGE("VirHeight is not 2-aligned");
    return false;
  }

  return true;
}

bool RKADK_MEDIA_CheckIdrFrame(RKADK_CODEC_TYPE_E enCodecType,
                               VENC_DATA_TYPE_U uDataType) {
  bool bIdr = false;

  switch (enCodecType) {
  case RKADK_CODEC_TYPE_H264:
    if (uDataType.enH264EType == H264E_NALU_IDRSLICE ||
        uDataType.enH264EType == H264E_NALU_ISLICE)
      bIdr = true;
    break;

  case RKADK_CODEC_TYPE_H265:
    if (uDataType.enH265EType == H265E_NALU_IDRSLICE ||
        uDataType.enH265EType == H265E_NALU_ISLICE)
      bIdr = true;
    break;

  default:
    break;
  }

  return bIdr;
}

RKADK_U16 RKADK_MEDIA_GetAudioBitWidth(AUDIO_BIT_WIDTH_E enBitWidth) {
  RKADK_U16 bitWidth = -1;

  switch (enBitWidth) {
  case AUDIO_BIT_WIDTH_8:
    bitWidth = 8;
    break;
  case AUDIO_BIT_WIDTH_16:
    bitWidth = 16;
    break;
  case AUDIO_BIT_WIDTH_24:
    bitWidth = 24;
    break;
  case AUDIO_BIT_WIDTH_32:
    bitWidth = 32;
    break;
  case AUDIO_BIT_WIDTH_FLT:
    bitWidth = sizeof(float);
    break;
  default:
    break;
  }

  return bitWidth;
}

int RKADK_MEDIA_GetPixelFormat(PIXEL_FORMAT_E enPixelFormat,
                               RKADK_CHAR *cPixFmt) {
  int ret = 0;

  switch (enPixelFormat) {
  case RK_FMT_YUV420SP:
    memcpy(cPixFmt, "NV12", strlen("NV12"));
    break;
  case RK_FMT_YUV422SP:
    memcpy(cPixFmt, "NV16", strlen("NV16"));
    break;
  default:
    RKADK_LOGE("Unsupport pixel format: %d", enPixelFormat);
    ret = -1;
    break;
  }

  return ret;
}

bool RKADK_MEDIA_EnableAencRegister(RKADK_CODEC_TYPE_E eCodecType) {
  if (eCodecType == RKADK_CODEC_TYPE_MP3 || eCodecType == RKADK_CODEC_TYPE_ACC)
    return true;

  return false;
}