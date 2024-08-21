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
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <unistd.h>

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
  RKADK_S64 s64RecentPts;
  RKADK_U64 u64TimeoutCnt; //Continuous timeout count
} RKADK_GET_VENC_MB_ATTR_S;

typedef struct {
  bool bUsed;
  bool bReset;
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
  pthread_mutex_t voMutex;
  pthread_mutex_t bindMutex;
  RKADK_MEDIA_INFO_S stAiInfo[RKADK_MEDIA_AI_MAX_CNT];
  RKADK_MEDIA_INFO_S stAencInfo[RKADK_MEDIA_AENC_MAX_CNT];
  RKADK_MEDIA_INFO_S stViInfo[RKADK_MEDIA_VI_MAX_CNT];
  RKADK_MEDIA_INFO_S stVencInfo[RKADK_MEDIA_VENC_MAX_CNT];
  RKADK_MEDIA_INFO_S stVpssInfo[RKADK_MEDIA_VPSS_MAX_CNT];
  RKADK_MEDIA_INFO_S stVoInfo[RKADK_MEDIA_VO_MAX_CNT];
  RKADK_BIND_INFO_S stAiAencInfo[RKADK_AI_AENC_MAX_BIND_CNT];
  RKADK_BIND_INFO_S stViVencInfo[RKADK_VI_VENC_MAX_BIND_CNT];
  RKADK_BIND_INFO_S stViVpssInfo[RKADK_VI_VPSS_MAX_BIND_CNT];
  RKADK_BIND_INFO_S stVpssVencInfo[RKADK_VPSS_VENC_MAX_BIND_CNT];
} RKADK_MEDIA_CONTEXT_S;

struct RKADK_FORMAT_MAP {
  RKADK_FORMAT_E Format;
  PIXEL_FORMAT_E enPixelFormat;
};

static const struct RKADK_FORMAT_MAP fromat[] = {
    {RKADK_FMT_ARGB1555, RK_FMT_ARGB1555},
    {RKADK_FMT_ABGR1555, RK_FMT_ABGR1555},
    {RKADK_FMT_RGBA5551, RK_FMT_RGBA5551},
    {RKADK_FMT_BGRA5551, RK_FMT_BGRA5551},
    {RKADK_FMT_ARGB4444, RK_FMT_ARGB4444},
    {RKADK_FMT_ABGR4444, RK_FMT_ABGR4444},
    {RKADK_FMT_RGBA4444, RK_FMT_RGBA4444},
    {RKADK_FMT_BGRA4444, RK_FMT_BGRA4444},
    {RKADK_FMT_ARGB8888, RK_FMT_ARGB8888},
    {RKADK_FMT_ABGR8888, RK_FMT_ABGR8888},
    {RKADK_FMT_RGBA8888, RK_FMT_RGBA8888},
    {RKADK_FMT_BGRA8888, RK_FMT_BGRA8888},
    {RKADK_FMT_YUV420SP, RK_FMT_YUV420SP},
    {RKADK_FMT_YUV420SP_10BIT, RK_FMT_YUV420SP_10BIT},
    {RKADK_FMT_YUV422SP, RK_FMT_YUV422SP},
    {RKADK_FMT_YUV422_UYVY, RK_FMT_YUV422_UYVY},
    {RKADK_FMT_2BPP, RK_FMT_2BPP}
};

static int g_dumpBufinfo = 0;
static bool g_bSysInit = false;
static RKADK_MEDIA_CONTEXT_S g_stMediaCtx;
static int g_bVpssGrpInitCnt[VPSS_MAX_GRP_NUM] = {0};
static int g_bVoLayerDevInitCnt[VO_MAX_LAYER_NUM][VO_MAX_DEV_NUM] = {0};

void RKADK_MEDIA_DumpBufinfo(const char *fmt, const char *fname, const int row, ...) {
  char line[256];

  if (!g_dumpBufinfo)
    return;

  va_list args;
  va_start(args, row);
  snprintf(line, sizeof(line), "[RKADK_BUFINFO] %s[%d]: %s\n", fname, row, fmt);
  vfprintf(stdout, line, args);
  va_end(args);

#ifdef RV1106_1103
  system("cat /proc/rk_dma_heap/dma_heap_info");
#else
  system("cat /sys/kernel/debug/dma_buf/bufinfo");
#endif
}

static int RKADK_MEDIA_CtxInit() {
  int ret;

  g_dumpBufinfo = getenv("rkadk_dump_bufinfo") && atoi(getenv("rkadk_dump_bufinfo"));

  memset((void *)&g_stMediaCtx, 0, sizeof(RKADK_MEDIA_CONTEXT_S));
  memset(g_bVpssGrpInitCnt, 0, VPSS_MAX_GRP_NUM * sizeof(int));
  memset(g_bVoLayerDevInitCnt, 0, VO_MAX_LAYER_NUM * VO_MAX_DEV_NUM * sizeof(int));

  ret = pthread_mutex_init(&g_stMediaCtx.aiMutex, NULL);
  ret |= pthread_mutex_init(&g_stMediaCtx.aencMutex, NULL);
  ret |= pthread_mutex_init(&g_stMediaCtx.viMutex, NULL);
  ret |= pthread_mutex_init(&g_stMediaCtx.vencMutex, NULL);
  ret |= pthread_mutex_init(&g_stMediaCtx.vpssMutex, NULL);
  ret |= pthread_mutex_init(&g_stMediaCtx.voMutex, NULL);
  ret |= pthread_mutex_init(&g_stMediaCtx.bindMutex, NULL);

  if (ret) {
    RKADK_LOGE("pthread_mutex_init failed[%d]", ret);
    return -1;
  }

  return 0;
}

static int RKADK_MEDIA_CtxDeInit() {
  int ret;

  ret = pthread_mutex_destroy(&g_stMediaCtx.aiMutex);
  ret |= pthread_mutex_destroy(&g_stMediaCtx.aencMutex);
  ret |= pthread_mutex_destroy(&g_stMediaCtx.viMutex);
  ret |= pthread_mutex_destroy(&g_stMediaCtx.vencMutex);
  ret |= pthread_mutex_destroy(&g_stMediaCtx.vpssMutex);
  ret |= pthread_mutex_destroy(&g_stMediaCtx.voMutex);
  ret |= pthread_mutex_destroy(&g_stMediaCtx.bindMutex);

  if (ret) {
    RKADK_LOGE("pthread_mutex_destroy failed[%d]", ret);
    return -1;
  }

  return 0;
}

static void RKADK_MEDIA_CheckVoLayerParam(VO_VIDEO_LAYER_ATTR_S *pstLayerAttr, VO_SYNC_INFO_S stSyncInfo) {
  RECT_S *pstLayerRect = &(pstLayerAttr->stDispRect);

  if (pstLayerRect->u32Width <= 0 || pstLayerRect->u32Width > stSyncInfo.u16Hact) {
    RKADK_LOGW("Invalid vo layer w[%d], use pub w[%d]", pstLayerRect->u32Width, stSyncInfo.u16Hact);
    pstLayerRect->u32Width = stSyncInfo.u16Hact;
  }

  if (pstLayerRect->u32Height <= 0 || pstLayerRect->u32Height > stSyncInfo.u16Vact) {
    RKADK_LOGW("Invalid vo layer h[%d], use pub h[%d]", pstLayerRect->u32Height, stSyncInfo.u16Vact);
    pstLayerRect->u32Height = stSyncInfo.u16Vact;
  }

  if (pstLayerRect->s32X < 0 || pstLayerRect->s32X >= pstLayerRect->u32Width) {
    RKADK_LOGW("Invalid vo layer x[%d], layer w[%d]", pstLayerRect->s32X, pstLayerRect->u32Width);
    pstLayerRect->s32X = 0;
  }

  if (pstLayerRect->s32Y < 0 || pstLayerRect->s32Y >= pstLayerRect->u32Height) {
    RKADK_LOGW("Invalid vo layer y[%d], layer h[%d]", pstLayerRect->s32Y, pstLayerRect->u32Height);
    pstLayerRect->s32Y = 0;
  }
}

static RKADK_U32 RKADK_MPI_VO_CreateLayDev(RKADK_S32 s32VoLay, RKADK_S32 s32VoDev,
                                          VO_PUB_ATTR_S *pstVoPubAttr, VO_VIDEO_LAYER_ATTR_S *pstLayerAttr,
                                          RKADK_VO_SPLICE_MODE_E enVoSpliceMode) {
  int ret = 0;
  VO_SPLICE_MODE_E enSpliceMode = VO_SPLICE_MODE_RGA;

  if (!g_bVoLayerDevInitCnt[s32VoLay][s32VoDev]) {
    ret = RK_MPI_VO_BindLayer(s32VoLay, s32VoDev, VO_LAYER_MODE_GRAPHIC);
    if (ret) {
      RKADK_LOGE("RK_MPI_VO_BindLayer [lay: %d, dev: %d] failed, ret = %x", s32VoLay, s32VoDev, ret);
      return ret;
    }

    ret = RK_MPI_VO_SetPubAttr(s32VoDev, pstVoPubAttr);
    if (ret) {
      RKADK_LOGE("RK_MPI_VO_SetPubAttr failed, ret = %x", ret);
      return ret;
    }

    ret = RK_MPI_VO_Enable(s32VoDev);
    if (ret) {
      RKADK_LOGE("RK_MPI_VO_Enable failed, ret = %x", ret);
      return ret;
    }

    ret = RK_MPI_VO_GetPubAttr(s32VoDev, pstVoPubAttr);
    if (ret) {
      RKADK_LOGE("RK_MPI_VO_GetPubAttr failed, ret = %x", ret);
      return ret;
    }

    if (enVoSpliceMode == SPLICE_MODE_GPU)
      enSpliceMode = VO_SPLICE_MODE_GPU;
    else if (enVoSpliceMode == SPLICE_MODE_RGA)
      enSpliceMode = VO_SPLICE_MODE_RGA;
#ifndef RV1106_1103
    else if (enVoSpliceMode == SPLICE_MODE_BYPASS)
      pstLayerAttr->bBypassFrame = RK_TRUE;
#endif

    if (enVoSpliceMode != SPLICE_MODE_BYPASS) {
      pstLayerAttr->stDispRect.s32X = 0;
      pstLayerAttr->stDispRect.s32Y = 0;
      pstLayerAttr->stDispRect.u32Width = pstVoPubAttr->stSyncInfo.u16Hact;
      pstLayerAttr->stDispRect.u32Height = pstVoPubAttr->stSyncInfo.u16Vact;
    }
    pstLayerAttr->stImageSize.u32Width = pstVoPubAttr->stSyncInfo.u16Hact;
    pstLayerAttr->stImageSize.u32Height = pstVoPubAttr->stSyncInfo.u16Vact;

    RKADK_MEDIA_CheckVoLayerParam(pstLayerAttr, pstVoPubAttr->stSyncInfo);

    ret = RK_MPI_VO_SetLayerAttr(s32VoLay, pstLayerAttr);
    if (ret) {
      RKADK_LOGE("RK_MPI_VO_SetLayerAttr failed, ret = %x", ret);
      return ret;
    }

    if (enVoSpliceMode != SPLICE_MODE_BYPASS)
      RK_MPI_VO_SetLayerSpliceMode(s32VoLay, enSpliceMode);

    RK_MPI_VO_SetLayerDispBufLen(s32VoLay, 3);

    ret = RK_MPI_VO_EnableLayer(s32VoLay);
    if (ret) {
      RKADK_LOGE("RK_MPI_VO_EnableLayer failed, ret = %x", ret);
      return ret;
    }
  } else {
    ret = RK_MPI_VO_GetPubAttr(s32VoDev, pstVoPubAttr);
    if (ret) {
      RKADK_LOGE("RK_MPI_VO_GetPubAttr failed, ret = %x", ret);
    } else {
      if (enVoSpliceMode != SPLICE_MODE_BYPASS) {
        pstLayerAttr->stDispRect.s32X  = 0;
        pstLayerAttr->stDispRect.s32Y  = 0;
        pstLayerAttr->stDispRect.u32Width = pstVoPubAttr->stSyncInfo.u16Hact;
        pstLayerAttr->stDispRect.u32Height = pstVoPubAttr->stSyncInfo.u16Vact;
      }
      pstLayerAttr->stImageSize.u32Width = pstVoPubAttr->stSyncInfo.u16Hact;
      pstLayerAttr->stImageSize.u32Height = pstVoPubAttr->stSyncInfo.u16Vact;

      RKADK_MEDIA_CheckVoLayerParam(pstLayerAttr, pstVoPubAttr->stSyncInfo);
    }
  }

  g_bVoLayerDevInitCnt[s32VoLay][s32VoDev]++;
  RKADK_LOGD("Vo Layer[%d] Devices[%d] InitCnt[%d]", s32VoLay, s32VoDev, g_bVoLayerDevInitCnt[s32VoLay][s32VoDev]);
  return 0;
}

static RKADK_U32 RKADK_MPI_VO_DestroyLayDev(RKADK_S32 s32VoLay, RKADK_S32 s32VoDev) {
  int ret;

  if (g_bVoLayerDevInitCnt[s32VoLay][s32VoDev] == 1) {
    ret = RK_MPI_VO_DisableLayer(s32VoLay);
    if (ret) {
      RKADK_LOGE("RK_MPI_VO_DisableLayer failed, ret = %x", ret);
      return ret;
    }

    ret = RK_MPI_VO_Disable(s32VoDev);
    if (ret) {
      RKADK_LOGE("RK_MPI_VO_Disable failed, ret = %x", ret);
      return ret;
    }

    ret = RK_MPI_VO_UnBindLayer(s32VoLay, s32VoDev);
    if (ret) {
      RKADK_LOGE("RK_MPI_VO_UnBindLayer failed, ret = %x", ret);
      return ret;
    }
  }

  g_bVoLayerDevInitCnt[s32VoLay][s32VoDev]--;
  RKADK_LOGD("Vo Layer[%d] Devices[%d] DeInitCnt[%d]", s32VoLay, s32VoDev, g_bVoLayerDevInitCnt[s32VoLay][s32VoDev]);
  return 0;
}

static RKADK_U32 RKADK_MPI_VPSS_CreateGrp(RKADK_S32 s32VpssGrp, VPSS_GRP_ATTR_S *pstVpssGrpAttr) {
  int ret;
  RKADK_PARAM_COMM_CFG_S *pstCommCfg = NULL;

  if (!g_bVpssGrpInitCnt[s32VpssGrp]) {
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
  }

  g_bVpssGrpInitCnt[s32VpssGrp]++;
  RKADK_LOGD("VpssGrp[%d] InitCnt[%d]", s32VpssGrp, g_bVpssGrpInitCnt[s32VpssGrp]);
  return 0;
}

static RKADK_U32 RKADK_MPI_VPSS_DestroyGrp(RKADK_S32 s32VpssGrp) {
  int ret;

  if (g_bVpssGrpInitCnt[s32VpssGrp] == 1) {
    ret = RK_MPI_VPSS_StopGrp(s32VpssGrp);
    if (ret) {
      RKADK_LOGE("RK_MPI_VPSS_StopGrp[%d] failed[%x]", s32VpssGrp, ret);
    }

    ret = RK_MPI_VPSS_DestroyGrp(s32VpssGrp);
    if (ret) {
      RKADK_LOGE("RK_MPI_VPSS_DestroyGrp[%d] failed[%x]", s32VpssGrp, ret);
      return ret;
    }
  }

  g_bVpssGrpInitCnt[s32VpssGrp]--;
  RKADK_LOGD("VpssGrp[%d] InitCnt[%d]", s32VpssGrp, g_bVpssGrpInitCnt[s32VpssGrp]);
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

    bMatch = ((pstInfo[i].s32ChnId == s32ChnId) && (pstInfo[i].s32DevId == s32DevId));
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
    ret = RKADK_MEDIA_CtxInit();
    if (ret) {
      RKADK_LOGE("RKADK_MEDIA_CtxInit failed[%d]", ret);
      return ret;
    }

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
    RKADK_MEDIA_CtxDeInit();
    RK_MPI_VO_CloseFd();

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

static RKADK_S32 RKADK_MPI_AI_EnableVqe(AUDIO_DEV s32DevId, RKADK_S32 s32AiChnId,
                                        RK_U32 u32SampleRate, RKADK_VQE_MODE_E enMode,
                                        const char *pVqeCfgPath) {
  int ret;
  RK_S32 s32VqeGapMs = 16; //just supports 16ms or 10ms for AI VQE
  AI_VQE_CONFIG_S stAiVqeConfig;

#ifdef RV1106_1103
  ret = RK_MPI_AMIX_SetControl(s32DevId, "I2STDM Digital Loopback Mode", (char *)"Mode2");
  if (ret) {
    RKADK_LOGE("AI[%d,%d] set I2STDM Digital Loopback Mode failed: %x", s32DevId, s32AiChnId, ret);
    return -1;
  }
#endif

  memset(&stAiVqeConfig, 0, sizeof(AI_VQE_CONFIG_S));
  stAiVqeConfig.enCfgMode = AIO_VQE_CONFIG_LOAD_FILE;
  memcpy(stAiVqeConfig.aCfgFile, pVqeCfgPath, strlen(pVqeCfgPath));
  stAiVqeConfig.s32WorkSampleRate = u32SampleRate;
  stAiVqeConfig.s32FrameSample = u32SampleRate * s32VqeGapMs / 1000;

  ret = RK_MPI_AI_SetVqeAttr(s32DevId, s32AiChnId, 0, 0, &stAiVqeConfig);
  if (ret) {
    RKADK_LOGE("Ai[%d,%d] SetVqeAttr failed: %#x", s32DevId, s32AiChnId, ret);
    return -1;
  }

  ret = RK_MPI_AI_EnableVqe(s32DevId, s32AiChnId);
  if (ret) {
    RKADK_LOGE("AI[%d,%d] EnableVqe failed: %#x", s32DevId, s32AiChnId, ret);
    return -1;
  }

  return 0;
}

RKADK_S32  RKADK_MPI_AI_Init(AUDIO_DEV aiDevId, RKADK_S32 s32AiChnId,
                            AIO_ATTR_S *pstAiAttr, RKADK_VQE_MODE_E enMode,
                            const char *pVqeCfgPath, RKADK_U32 micType) {
  int ret = -1;
  RKADK_S32 i;
  RKADK_U32 s32SetTrackMode = 0;
  AI_CHN_PARAM_S pstParams;
  RKADK_PARAM_AUDIO_CFG_S *pstAudioParam;
  RKADK_PARAM_COMM_CFG_S *pstCommCfg = NULL;

  RKADK_CHECK_POINTER(pstAiAttr, RKADK_FAILURE);

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

  memset(&pstParams, 0, sizeof(AI_CHN_PARAM_S));
  pstParams.enLoopbackMode = AUDIO_LOOPBACK_NONE;
  pstParams.s32UsrFrmDepth = pstAudioParam->ai_depth;

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

    if (enMode != RKADK_VQE_MODE_BUTT)
      RKADK_MPI_AI_EnableVqe(aiDevId, s32AiChnId, pstAiAttr->enSamplerate, enMode, pVqeCfgPath);

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

    ret = RK_MPI_AI_SetVolume(aiDevId, pstCommCfg->mic_volume);
    if (ret) {
      RKADK_LOGE("AI[%d, %d] set volume[%d] failed[%x]", aiDevId, s32AiChnId, pstCommCfg->mic_volume, ret);
    }

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
    if (enMode != RKADK_VQE_MODE_BUTT) {
#ifdef RV1106_1103
      ret = RK_MPI_AMIX_SetControl(aiDevId, "I2STDM Digital Loopback Mode", (char *)"Disabled");
      if (ret)
        RKADK_LOGE("AI[%d, %d] set I2STDM Digital Loopback Mode failed: %x", aiDevId, s32AiChnId, ret);
#endif
      ret = RK_MPI_AI_DisableVqe(aiDevId, s32AiChnId);
      if (ret)
        RKADK_LOGE("AI[%d, %d] DisableVqe failed: %d", aiDevId, s32AiChnId, ret);
    }

    ret = RK_MPI_AI_DisableChn(aiDevId, s32AiChnId);
    if (ret) {
      RKADK_LOGE("AI[%d, %d] disable chn failed[%x]", aiDevId, s32AiChnId, ret);
      goto exit;
    }

    ret = RK_MPI_AI_Disable(aiDevId);
    if (ret != 0) {
      RKADK_LOGE("Ai[%d] disable failed[%x]", aiDevId, ret);
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
    g_stMediaCtx.stAencInfo[i].s32DevId = 0;
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
    g_stMediaCtx.stAencInfo[i].s32DevId = -1;
    g_stMediaCtx.stAencInfo[i].s32ChnId = -1;
    g_stMediaCtx.stAencInfo[i].enCodecType = RK_VIDEO_ID_Unused;
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
      if (ret) {
        RKADK_LOGE("RK_MPI_VI_SetDevAttr failed[%x]", ret);
        goto exit;
      }
    } else {
      RKADK_LOGI("RK_MPI_VI_SetDevAttr already");
    }

    // 1.get dev enable status
    ret = RK_MPI_VI_GetDevIsEnable(u32CamId);
    if (ret) {
      // 1-2.enable dev
      ret = RK_MPI_VI_EnableDev(u32CamId);
      if (ret) {
        RKADK_LOGE("RK_MPI_VI_EnableDev failed[%x]", ret);
        goto exit;
      }

      // 1-3.bind dev/pipe
      stBindPipe.u32Num = 1;
      stBindPipe.PipeId[0] = u32CamId;
      ret = RK_MPI_VI_SetDevBindPipe(u32CamId, &stBindPipe);
      if (ret) {
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
    g_stMediaCtx.stVencInfo[i].bReset = false;
    g_stMediaCtx.stVencInfo[i].s32DevId = 0;
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
    g_stMediaCtx.stVencInfo[i].bReset = false;
    g_stMediaCtx.stVencInfo[i].s32DevId = -1;
    g_stMediaCtx.stVencInfo[i].s32ChnId = -1;
    g_stMediaCtx.stVencInfo[i].enCodecType = RK_VIDEO_ID_Unused;
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
  RKADK_S32 i;
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

    ret = RKADK_MPI_VPSS_DestroyGrp(s32VpssGrp);
    if (ret) {
      RKADK_LOGE("RKADK_MPI_VPSS_DestroyGrp[%d] failed[%x]", s32VpssGrp, ret);
      goto exit;
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

static void RKADK_MEDIA_CheckVoParam(VO_VIDEO_LAYER_ATTR_S *pstLayerAttr, VO_CHN_ATTR_S *pstChnAttr) {
  RKADK_U32 sum = 0;
  RECT_S *pstChnRect = &(pstChnAttr->stRect);
  RECT_S *pstLayerRect = &(pstLayerAttr->stDispRect);

  if (pstChnRect->s32X < pstLayerRect->s32X || pstChnRect->s32X > pstLayerRect->u32Width) {
    RKADK_LOGW("Invalid vo chn x[%d], use layer x[%d]", pstChnRect->s32X, pstLayerRect->s32X);
    pstChnRect->s32X = pstLayerRect->s32X;
  }

  if (pstChnRect->s32Y < pstLayerRect->s32Y || pstChnRect->s32Y > pstLayerRect->u32Height) {
    RKADK_LOGW("Invalid vo chn y[%d], use layer y[%d]", pstChnRect->s32Y, pstLayerRect->s32Y);
    pstChnRect->s32Y = pstLayerRect->s32Y;
  }

  if (pstChnRect->u32Width <= 0 || (pstChnRect->u32Width > pstLayerRect->u32Width)) {
    RKADK_LOGW("Invalid vo chn w[%d], use layer w[%d]", pstChnRect->u32Width, pstLayerRect->u32Width);
    pstChnRect->u32Width = pstLayerRect->u32Width;
  }

  if (pstChnRect->u32Height <= 0 || pstChnRect->u32Height > pstLayerRect->u32Height) {
    RKADK_LOGW("Invalid vo chn h[%d], use layer h[%d]", pstChnRect->u32Height, pstLayerRect->u32Height);
    pstChnRect->u32Height = pstLayerRect->u32Height;
  }

  sum = pstChnRect->s32X + pstChnRect->u32Width;
  if (sum > pstLayerRect->u32Width) {
    RKADK_LOGW("The vo chn x-coordinate[%d][%d, %d] over layer w[%d]", sum, pstChnRect->s32X,
                pstChnRect->u32Width, pstLayerRect->u32Width);

    pstChnRect->u32Width = pstLayerRect->u32Width - pstChnRect->s32X;
  }

  sum = pstChnRect->s32Y + pstChnRect->u32Height;
  if (sum > pstLayerRect->u32Height) {
    RKADK_LOGW("The vo chn y-coordinate[%d][%d, %d] over layer h[%d]", sum, pstChnRect->s32Y,
                pstChnRect->u32Height, pstLayerRect->u32Height);

    pstChnRect->u32Height = pstLayerRect->u32Height - pstChnRect->s32Y;
  }
}

RKADK_S32 RKADK_MPI_VO_Init(RKADK_S32 s32VoLay, RKADK_S32 s32VoDev, RKADK_S32 s32VoChn,
                        VO_PUB_ATTR_S *pstVoPubAttr, VO_VIDEO_LAYER_ATTR_S *pstLayerAttr,
                        VO_CHN_ATTR_S *pstChnAttr, RKADK_VO_SPLICE_MODE_E enVoSpliceMode) {
  int ret = -1;
  RKADK_S32 i;

  RKADK_CHECK_POINTER(pstVoPubAttr, RKADK_FAILURE);
  RKADK_CHECK_POINTER(pstLayerAttr, RKADK_FAILURE);
  RKADK_CHECK_POINTER(pstChnAttr, RKADK_FAILURE);

  RKADK_MUTEX_LOCK(g_stMediaCtx.voMutex);

  i = RKADK_MEDIA_GetIdx(g_stMediaCtx.stVoInfo, RKADK_MEDIA_VO_MAX_CNT,
                         s32VoDev, s32VoChn, "VO_INIT");
  if (i < 0) {
    i = RKADK_MEDIA_FindUsableIdx(g_stMediaCtx.stVoInfo, RKADK_MEDIA_VO_MAX_CNT,
                                  "VO_INIT");
    if (i < 0) {
      RKADK_LOGE("not find usable index");
      goto exit;
    }
  }

  if (0 == g_stMediaCtx.stVoInfo[i].s32InitCnt) {
    ret = RKADK_MPI_VO_CreateLayDev(s32VoLay, s32VoDev, pstVoPubAttr,
                                      pstLayerAttr, enVoSpliceMode);
    if (ret) {
      RKADK_LOGE("RKADK_MPI_Vo_CreateLayDev failed [%d]", ret);
      goto exit;
    }

    RKADK_MEDIA_CheckVoParam(pstLayerAttr, pstChnAttr);

    ret = RK_MPI_VO_SetChnAttr(s32VoLay, s32VoChn, pstChnAttr);
    if (ret) {
      RKADK_LOGE("RK_MPI_VO_SetChnAttr failed, ret = %x", ret);
      goto exit;
    }

    ret = RK_MPI_VO_EnableChn(s32VoLay, s32VoChn);
    if (ret) {
      RKADK_LOGE("RK_MPI_VO_EnableChn failed, ret = %x", ret);
      goto exit;
    }

    g_stMediaCtx.stVoInfo[i].bUsed = true;
    g_stMediaCtx.stVoInfo[i].s32DevId = s32VoDev;
    g_stMediaCtx.stVoInfo[i].s32ChnId = s32VoChn;
  }

  g_stMediaCtx.stVoInfo[i].s32InitCnt++;
  RKADK_LOGD("Vo Layer[%d] Devices[%d] Chn[%d] InitCnt[%d]", s32VoLay, s32VoDev, s32VoChn,
             g_stMediaCtx.stVoInfo[i].s32InitCnt);
  ret = 0;

exit:
  RKADK_MUTEX_UNLOCK(g_stMediaCtx.voMutex);
  return 0;
}

RKADK_S32 RKADK_MPI_VO_DeInit(RKADK_S32 s32VoLay, RKADK_S32 s32VoDev, RKADK_S32 s32VoChn) {
  int ret = -1;
  RKADK_S32 i;
  RKADK_S32 s32InitCnt;

  RKADK_MUTEX_LOCK(g_stMediaCtx.voMutex);

  i = RKADK_MEDIA_GetIdx(g_stMediaCtx.stVoInfo, RKADK_MEDIA_VO_MAX_CNT,
                         s32VoDev, s32VoChn, "VO_DEINIT");
  if (i < 0) {
    RKADK_LOGE("not find matched index[%d] s32ChnId[%d]", i, s32VoChn);
    goto exit;
  }

  s32InitCnt = g_stMediaCtx.stVoInfo[i].s32InitCnt;
  if (0 == s32InitCnt) {
    RKADK_LOGD("Vo Layer[%d] Devices[%d] Chn[%d] has already deinit",
                s32VoLay, s32VoDev, s32VoChn);
    RKADK_MUTEX_UNLOCK(g_stMediaCtx.voMutex);
    return 0;
  } else if (1 == s32InitCnt) {
    ret = RK_MPI_VO_DisableChn(s32VoLay, s32VoChn);
    if (ret) {
      RKADK_LOGE("Destory Vo Chn[%d] failed[%x]", s32VoChn, ret);
      goto exit;
    }

    ret = RKADK_MPI_VO_DestroyLayDev(s32VoLay, s32VoDev);
    if (ret) {
      RKADK_LOGE("RKADK_MPI_Vo_DestroyLayDev[%d, %d] failed[%x]", s32VoLay, s32VoDev, ret);
      goto exit;
    }

    g_stMediaCtx.stVoInfo[i].bUsed = false;
    g_stMediaCtx.stVoInfo[i].s32ChnId = -1;
    g_stMediaCtx.stVoInfo[i].s32DevId = -1;
  }

  g_stMediaCtx.stVoInfo[i].s32InitCnt--;
  RKADK_LOGD("Vo Layer[%d] Devices[%d] Chn[%d], DeInitCnt[%d]", s32VoLay, s32VoDev, s32VoChn,
             g_stMediaCtx.stVoInfo[i].s32InitCnt);
  ret = 0;

exit:
  RKADK_MUTEX_UNLOCK(g_stMediaCtx.voMutex);
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
      if (ret)
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
  char name[RKADK_THREAD_NAME_LEN];
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
    RKADK_LOGE("Get aencChnId[%d] MB thread has been created, s32GetCnt[%d]",
               pstChn->s32ChnId, pstMediaInfo->stGetAencMBAttr.s32GetCnt);
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
                              RKADK_MEDIA_AENC_DATA_PROC_FUNC pfnDataCB,
                              RKADK_VOID *pHandle) {
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

  for (i = 0; i < RKADK_MEDIA_AENC_MAX_CNT; i++) {
    if (pstMediaInfo->stGetAencMBAttr.cbList[i] == pfnDataCB
        && pstMediaInfo->stGetAencMBAttr.pHandle[i] == pHandle) {
      RKADK_LOGD("remove i[%d] cbList s32GetCnt[%d]", i, pstMediaInfo->stGetAencMBAttr.s32GetCnt);
      pstMediaInfo->stGetAencMBAttr.cbList[i] = NULL;
      pstMediaInfo->stGetAencMBAttr.pHandle[i] = NULL;
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
    ret = RK_MPI_VENC_GetStream(pstMediaInfo->s32ChnId, &stData.stFrame, 2000);

    if (ret == RK_SUCCESS) {
      for (int i = 0; i < (int)pstMediaInfo->stGetVencMBAttr.s32GetCnt; i++)
        if (pstMediaInfo->stGetVencMBAttr.cbList[i])
          pstMediaInfo->stGetVencMBAttr.cbList[i](
              stData, pstMediaInfo->stGetVencMBAttr.pHandle[i]);

      pstMediaInfo->stGetVencMBAttr.s64RecentPts = stData.stFrame.pstPack->u64PTS;
      pstMediaInfo->stGetVencMBAttr.u64TimeoutCnt = 0;
      ret = RK_MPI_VENC_ReleaseStream(pstMediaInfo->s32ChnId, &stData.stFrame);
      if (ret)
        RKADK_LOGE("RK_MPI_VENC_ReleaseStream failed[%x]", ret);
    } else {
      if (!pstMediaInfo->bReset) {
        RKADK_LOGE("RK_MPI_VENC_GetStream chn[%d] timeout[%x]", pstMediaInfo->s32ChnId, ret);
        pstMediaInfo->stGetVencMBAttr.u64TimeoutCnt++;

        //dump video info
#ifdef RV1106_1103
        system("cat proc/rkisp-vir0 | grep frame");
        system("cat /dev/mpi/vsys");

#ifndef ENABLE_AOV
        system("cat /proc/vcodec/enc/venc_info");
#endif
#else
        system("dumpsys sys");
        system("dumpsys vi");
        system("dumpsys vpss");
        system("dumpsys venc");
#endif
      }
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
  char name[RKADK_THREAD_NAME_LEN];
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
    RKADK_LOGE("Get vencChnId[%d] MB thread has been created, s32GetCnt[%d]",
               pstChn->s32ChnId, pstMediaInfo->stGetVencMBAttr.s32GetCnt);
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
                              RKADK_MEDIA_VENC_DATA_PROC_FUNC pfnDataCB,
                              RKADK_VOID *pHandle) {
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

  for (i = 0; i < RKADK_MEDIA_VENC_MAX_CNT; i++) {
    if (pstMediaInfo->stGetVencMBAttr.cbList[i] == pfnDataCB
        && pstMediaInfo->stGetVencMBAttr.pHandle[i] == pHandle) {
      RKADK_LOGD("remove i[%d] cbList s32GetCnt[%d]", i, pstMediaInfo->stGetVencMBAttr.s32GetCnt);
      pstMediaInfo->stGetVencMBAttr.cbList[i] = NULL;
      pstMediaInfo->stGetVencMBAttr.pHandle[i] = NULL;
      pstMediaInfo->stGetVencMBAttr.s32GetCnt--;
      break;
    }
  }

  if (!pstMediaInfo->stGetVencMBAttr.s32GetCnt) {
    pstMediaInfo->stGetVencMBAttr.bGetBuffer = false;
    if (pstMediaInfo->stGetVencMBAttr.tid) {
      RKADK_LOGD("Request to cancel venc mb thread...");
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
  } else if (pstSrcChn->enModId == RK_ID_VI &&
             pstDestChn->enModId == RK_ID_VPSS) {
    s32BindCount = RKADK_VI_VPSS_MAX_BIND_CNT;
    *pstInfo = g_stMediaCtx.stViVpssInfo;
  } else if (pstSrcChn->enModId == RK_ID_VPSS &&
             pstDestChn->enModId == RK_ID_VENC) {
    s32BindCount = RKADK_VPSS_VENC_MAX_BIND_CNT;
    *pstInfo = g_stMediaCtx.stVpssVencInfo;
  } else if (pstSrcChn->enModId == RK_ID_VPSS &&
             pstDestChn->enModId == RK_ID_VO) {
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

PIXEL_FORMAT_E RKADK_MEDIA_GetRkPixelFormat(RKADK_FORMAT_E Format) {
  RKADK_U32 i;

  for (i = 0; i < sizeof(fromat) / sizeof(fromat[0]); i++) {
    if (fromat[i].Format == Format)
    return fromat[i].enPixelFormat;
  }

  return RK_FMT_BUTT;
}

VO_INTF_TYPE_E RKADK_MEDIA_GetRkVoIntfTpye(RKADK_VO_INTF_TYPE_E enIntfType) {
  VO_INTF_TYPE_E enVoIntfType;

  switch (enIntfType) {
  case DISPLAY_TYPE_VGA:
    enVoIntfType = VO_INTF_VGA;
    break;

  case DISPLAY_TYPE_HDMI:
    enVoIntfType = VO_INTF_HDMI;
    break;

  case DISPLAY_TYPE_EDP:
    enVoIntfType = VO_INTF_EDP;
    break;

  case DISPLAY_TYPE_DP:
    enVoIntfType = VO_INTF_DP;
    break;

  case DISPLAY_TYPE_HDMI_EDP:
    enVoIntfType = VO_INTF_HDMI | VO_INTF_EDP;
    break;

  case DISPLAY_TYPE_MIPI:
    enVoIntfType = VO_INTF_MIPI;
    break;

  case DISPLAY_TYPE_LCD:
    enVoIntfType = VO_INTF_LCD;
    break;

  case DISPLAY_TYPE_DEFAULT:
    enVoIntfType = VO_INTF_DEFAULT;
    break;

  default:
    enVoIntfType = VO_INTF_DEFAULT;
    RKADK_LOGW("IntfType not set, use VO_INTF_DEFAULT");
  }

  return enVoIntfType;
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

bool RKADK_MEDIA_CompareResolution(VENC_CHN_ATTR_S *pstVencChnAttr,
                                RKADK_U32 u32Width, RKADK_U32 u32Height) {
  RKADK_LOGD("Old width height[%d %d], new width height[%d %d]",
            pstVencChnAttr->stVencAttr.u32PicWidth, pstVencChnAttr->stVencAttr.u32PicHeight,
            u32Width, u32Height);
  if (pstVencChnAttr->stVencAttr.u32PicWidth == u32Width &&
      pstVencChnAttr->stVencAttr.u32PicHeight == u32Height)
    return false;

  return true;
}

bool RKADK_MEDIA_VencAttrCmp(VENC_CHN_ATTR_S *pstVencChnAttr,
                                  RKADK_U32 u32Gop, RKADK_U32 u32DstFrameRate,
                                  RKADK_U32 u32Bitrate) {
  bool reset = false;
  RKADK_U32 u32OldFrameRate = 0, u32OldBitrate = 0, u32OldGop = 0;
  VENC_RC_ATTR_S *pstRcAttr = &pstVencChnAttr->stRcAttr;

  u32Bitrate = u32Bitrate / 1000;
  switch (pstRcAttr->enRcMode) {
  case VENC_RC_MODE_H265CBR:
    u32OldFrameRate = pstRcAttr->stH265Cbr.fr32DstFrameRateNum;
    u32OldBitrate = pstRcAttr->stH265Cbr.u32BitRate;
    u32OldGop = pstRcAttr->stH265Cbr.u32Gop;
    break;
  case VENC_RC_MODE_H265VBR:
    u32OldFrameRate = pstRcAttr->stH265Vbr.fr32DstFrameRateNum;
    u32OldBitrate = pstRcAttr->stH265Vbr.u32BitRate;
    u32OldGop = pstRcAttr->stH265Vbr.u32Gop;
    break;
  case VENC_RC_MODE_H265AVBR:
    u32OldFrameRate = pstRcAttr->stH265Avbr.fr32DstFrameRateNum;
    u32OldBitrate = pstRcAttr->stH265Avbr.u32BitRate;
    u32OldGop = pstRcAttr->stH265Avbr.u32Gop;
    break;
  case VENC_RC_MODE_MJPEGCBR:
    u32OldFrameRate = pstRcAttr->stMjpegCbr.fr32DstFrameRateNum;
    u32OldBitrate = pstRcAttr->stMjpegCbr.u32BitRate;
    break;
  case VENC_RC_MODE_MJPEGVBR:
    u32OldFrameRate = pstRcAttr->stMjpegVbr.fr32DstFrameRateNum;
    u32OldBitrate = pstRcAttr->stMjpegVbr.u32BitRate;
    break;
  case VENC_RC_MODE_H264CBR:
    u32OldFrameRate = pstRcAttr->stH264Cbr.fr32DstFrameRateNum;
    u32OldBitrate = pstRcAttr->stH264Cbr.u32BitRate;
    u32OldGop = pstRcAttr->stH264Cbr.u32Gop;
    break;
  case VENC_RC_MODE_H264VBR:
    u32OldFrameRate = pstRcAttr->stH264Vbr.fr32DstFrameRateNum;
    u32OldBitrate = pstRcAttr->stH264Vbr.u32BitRate;
    u32OldGop = pstRcAttr->stH264Vbr.u32Gop;
    break;
  case VENC_RC_MODE_H264AVBR:
    u32OldFrameRate = pstRcAttr->stH264Avbr.fr32DstFrameRateNum;
    u32OldBitrate = pstRcAttr->stH264Avbr.u32BitRate;
    u32OldGop = pstRcAttr->stH264Avbr.u32Gop;
    break;
  default:
    RKADK_LOGE("Invalid rc mode: %d", pstRcAttr->enRcMode);
    return false;
  }

  if (u32OldFrameRate != u32DstFrameRate)
    reset = true;

  if (u32OldBitrate != u32Bitrate)
    reset |= true;

  if ((u32OldGop != 0) && (u32OldGop != u32Gop))
    reset |= true;

  RKADK_LOGD("Old fps [%d], new fps [%d]", u32OldFrameRate, u32DstFrameRate);
  RKADK_LOGD("Old bitrate [%d], new bitrate [%d]", u32OldBitrate, u32Bitrate);
  RKADK_LOGD("Old gop [%d], new gop [%d]", u32OldGop, u32Gop);

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
  case RKADK_THUMB_TYPE_RGBA8888:
  case RKADK_THUMB_TYPE_BGRA8888:
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
  case RK_FMT_YUV422_UYVY:
    memcpy(cPixFmt, "UYVY", strlen("UYVY"));
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


RKADK_S32 RKADK_MEDIA_SetVencRotation(RKADK_U32 u32CamId,
                              ROTATION_E enRotation, RKADK_STREAM_TYPE_E enStreamType) {
  int ret = 0, ret1 = 0;
  RKADK_S32 s32VencChnId, s32ThumbChId;
  ROTATION_E rotation;

  s32VencChnId = RKADK_PARAM_GetVencChnId(u32CamId, enStreamType);
  if (s32VencChnId < 0) {
    RKADK_LOGE("Stream[%d] get venc chn id failed", enStreamType);
    return -1;
  }

  ret = RK_MPI_VENC_GetChnRotation(s32VencChnId, &rotation);
  if (ret != RK_SUCCESS) {
    RKADK_LOGE("Get venc[%d] rotation failed[%x]", s32VencChnId, ret);
    return -1;
  }

  if (rotation == enRotation)
    return 0;

  ret = RK_MPI_VENC_SetChnRotation(s32VencChnId, enRotation);
  if (ret != RK_SUCCESS) {
    RKADK_LOGE("Set venc[%d] rotation failed[%x]", s32VencChnId, ret);

    if (enStreamType == RKADK_STREAM_TYPE_SNAP && (enRotation == ROTATION_90 || enRotation == ROTATION_270))
      RKADK_LOGW("jpeg input resolution must be 16 aligned, please check");
  } else {
    s32ThumbChId = RKADK_PARAM_GetThumbChnId(u32CamId, enStreamType);
    if (s32ThumbChId != -1) {
      //set thumb rotation
      ret1 = RK_MPI_VENC_SetChnRotation(s32ThumbChId, enRotation);
      if (ret1) {
        RKADK_LOGE("Set thumb[%d] rotation failed[%x]", s32ThumbChId, ret1);

        if (enRotation == ROTATION_90 || enRotation == ROTATION_270)
          RKADK_LOGW("jpeg input resolution must be 16 aligned, please check");
      }
    }
  }

  return ret;
}

RKADK_S32 RKADK_MEDIA_QueryVencStatus(RKADK_U32 u32CamId, RKADK_STREAM_TYPE_E enStreamType) {
  RKADK_S32 s32ChnId;
  int i, ret = 0;
  struct sysinfo info;
  RKADK_U64 u64TimeoutCnt = 0;
  RKADK_S64 s64currentTime = 0, s64RecentPts = 0;

  s32ChnId = RKADK_PARAM_GetVencChnId(u32CamId, enStreamType);
  if (s32ChnId < 0) {
    RKADK_LOGE("Stream[%d] get venc chn id failed", enStreamType);
    return 0;
  }

  RKADK_MUTEX_LOCK(g_stMediaCtx.vencMutex);

  for (i = 0; i < RKADK_MEDIA_VENC_MAX_CNT; i++) {
    if (!g_stMediaCtx.stVencInfo[i].bUsed)
      continue;

    if (g_stMediaCtx.stVencInfo[i].s32ChnId == s32ChnId)
      break;
  }

  if (i == RKADK_MEDIA_VENC_MAX_CNT) {
    RKADK_LOGE("not find matched u32CamId[%d] s32ChnId[%d]", u32CamId, s32ChnId);
    ret = 0;
    goto exit;
  }

  sysinfo(&info);
  s64currentTime = info.uptime * 1000000LL;
  s64RecentPts = g_stMediaCtx.stVencInfo[i].stGetVencMBAttr.s64RecentPts;
  assert(s64currentTime - s64RecentPts > 0);

  if (s64RecentPts != 0 && s64currentTime - s64RecentPts > 3000000) {
    RKADK_LOGW("CamId[%d] VENC[%d] timeout more than 3 seconds!", u32CamId, s32ChnId);
    ret = -1;
    goto exit;
  }

  u64TimeoutCnt = g_stMediaCtx.stVencInfo[i].stGetVencMBAttr.u64TimeoutCnt;
  if (u64TimeoutCnt >= 3) {
    RKADK_LOGW("CamId[%d] VENC[%d] timeout more than %lld times!", u32CamId, s32ChnId, u64TimeoutCnt);
    ret = -1;
    goto exit;
  }

exit:
  RKADK_MUTEX_UNLOCK(g_stMediaCtx.vencMutex);
  return ret;
}

//flip: mirror + 180, both: 180
static MIRROR_E RKADK_MEDIA_GetVencMirror(RKADK_S32 s32VencChn, VENC_CHN_ATTR_S *pstVencChnAttr) {
  int ret;
  MIRROR_E enMirror = MIRROR_NONE;
  ROTATION_E rotation;

  ret = RK_MPI_VENC_GetChnRotation(s32VencChn, &rotation);
  if (ret != RK_SUCCESS) {
    RKADK_LOGE("Get venc[%d] rotation failed[%x]", s32VencChn, ret);
    return MIRROR_NONE;
  }

  enMirror = pstVencChnAttr->stVencAttr.enMirror;
  if (rotation == ROTATION_180) {
    if (pstVencChnAttr->stVencAttr.enMirror == MIRROR_NONE)
      enMirror = MIRROR_BOTH;
    else if (pstVencChnAttr->stVencAttr.enMirror == MIRROR_HORIZONTAL)
      enMirror = MIRROR_VERTICAL;
  }

  return enMirror;
}

static RKADK_S32 RKADK_MEDIA_SetVencMirror(RKADK_U32 u32CamId,
                                    RKADK_STREAM_TYPE_E enStrmType,
                                    MIRROR_E enMirror, int s32VencChn,
                                    VENC_CHN_ATTR_S *pstVencChnAttr) {
  int ret, ret1;
  int s32ThumbChId;
  MIRROR_E mirror;

  if (enMirror == MIRROR_BOTH)
    mirror = MIRROR_NONE;
  else
    mirror = enMirror;

  if (mirror != pstVencChnAttr->stVencAttr.enMirror) {
    pstVencChnAttr->stVencAttr.enMirror = mirror;
    ret = RK_MPI_VENC_SetChnAttr(s32VencChn, pstVencChnAttr);
    if (ret != RK_SUCCESS) {
      RKADK_LOGE("Set venc[%d] attr failed[%x]", s32VencChn, ret);
      return -1;
    } else {
      //set thumb flip/mirror
      s32ThumbChId = RKADK_PARAM_GetThumbChnId(u32CamId, enStrmType);
      if (s32ThumbChId != -1) {
        memset(pstVencChnAttr, 0, sizeof(VENC_CHN_ATTR_S));
        ret1 = RK_MPI_VENC_GetChnAttr(s32ThumbChId, pstVencChnAttr);
        if (ret1 != RK_SUCCESS) {
          RKADK_LOGE("Get thumb[%d] venc attr failed [%x]", s32ThumbChId, ret1);
        } else {
          pstVencChnAttr->stVencAttr.enMirror = mirror;
          ret1 = RK_MPI_VENC_SetChnAttr(s32ThumbChId, pstVencChnAttr);
          if (ret1 != RK_SUCCESS)
            RKADK_LOGE("Set thumb[%d] venc attr failed[%x]", s32ThumbChId, ret1);
        }
      }
    }
  }

  if (enMirror == MIRROR_VERTICAL || enMirror == MIRROR_BOTH)
    ret = RKADK_MEDIA_SetVencRotation(u32CamId, ROTATION_180, enStrmType);
  else
    ret = RKADK_MEDIA_SetVencRotation(u32CamId, ROTATION_0, enStrmType);

  return ret;
}

RKADK_S32 RKADK_MEDIA_ToggleVencMirror(RKADK_U32 u32CamId,
                                    RKADK_STREAM_TYPE_E enStrmType,
                                    bool mirror) {
  int ret = 0;
  int s32VencChn;
  VENC_CHN_ATTR_S stVencChnAttr;
  MIRROR_E enMirror = MIRROR_NONE;

  RKADK_CHECK_CAMERAID(u32CamId, RKADK_FAILURE);

  s32VencChn = RKADK_PARAM_GetVencChnId(u32CamId, enStrmType);
  if (s32VencChn == -1)
    return -1;

  memset(&stVencChnAttr, 0, sizeof(VENC_CHN_ATTR_S));
  ret = RK_MPI_VENC_GetChnAttr(s32VencChn, &stVencChnAttr);
  if (ret != RK_SUCCESS) {
    RKADK_LOGE("RK_MPI_VENC_GetChnAttr[%d] failed [%x]", s32VencChn, ret);
    return -1;
  }

  enMirror = RKADK_MEDIA_GetVencMirror(s32VencChn, &stVencChnAttr);
  if (mirror)
    enMirror = (MIRROR_E)(enMirror | MIRROR_HORIZONTAL);
  else
    enMirror = (MIRROR_E)(enMirror & (~MIRROR_HORIZONTAL));

  return RKADK_MEDIA_SetVencMirror(u32CamId, enStrmType, enMirror, s32VencChn, &stVencChnAttr);
}

RKADK_S32 RKADK_MEDIA_ToggleVencFlip(RKADK_U32 u32CamId,
                                  RKADK_STREAM_TYPE_E enStrmType,
                                  bool flip) {
  int ret = 0;
  int s32VencChn;
  VENC_CHN_ATTR_S stVencChnAttr;
  MIRROR_E enMirror = MIRROR_NONE;

  RKADK_CHECK_CAMERAID(u32CamId, RKADK_FAILURE);

  s32VencChn = RKADK_PARAM_GetVencChnId(u32CamId, enStrmType);
  if (s32VencChn == -1)
    return -1;

  memset(&stVencChnAttr, 0, sizeof(VENC_CHN_ATTR_S));
  ret = RK_MPI_VENC_GetChnAttr(s32VencChn, &stVencChnAttr);
  if (ret != RK_SUCCESS) {
    RKADK_LOGE("RK_MPI_VENC_GetChnAttr[%d] failed [%x]", s32VencChn, ret);
    return -1;
  }

  enMirror = RKADK_MEDIA_GetVencMirror(s32VencChn, &stVencChnAttr);
  if (flip)
    enMirror = (MIRROR_E)(enMirror | MIRROR_VERTICAL);
  else
    enMirror = (MIRROR_E)(enMirror & (~MIRROR_VERTICAL));

  return RKADK_MEDIA_SetVencMirror(u32CamId, enStrmType, enMirror, s32VencChn, &stVencChnAttr);
}

RKADK_S32 RKADK_MEDIA_EnablePostIsp(RKADK_U32 u32CamId, RKADK_STREAM_TYPE_E enStrmType,
                                  RKADK_POST_ISP_ATTR_S *pstPostIspAttr) {
  int ret;
  RKADK_S32 s32VpssGrp = -1, s32VpssChn = -1;
  AIISP_ATTR_S stPostIspAttr;

  RKADK_CHECK_CAMERAID(u32CamId, RKADK_FAILURE);
  RKADK_CHECK_POINTER(pstPostIspAttr, RKADK_FAILURE);

  RKADK_PARAM_GetVpssId(u32CamId, enStrmType, &s32VpssGrp, &s32VpssChn);
  if (s32VpssGrp == -1 || s32VpssChn == -1) {
    RKADK_LOGE("RKADK_PARAM_GetVpssId[%d, %d] failed", s32VpssGrp, s32VpssChn);
    return -1;
  }

  memset(&stPostIspAttr, 0, sizeof(AIISP_ATTR_S));
  stPostIspAttr.bEnable = RK_TRUE;
  stPostIspAttr.stAiIspCallback.pfUpdateCallback = pstPostIspAttr->stAiIspCallback.pfUpdateCallback;
  stPostIspAttr.stAiIspCallback.pPrivateData = pstPostIspAttr->stAiIspCallback.pPrivateData;
  stPostIspAttr.pModelFilePath = pstPostIspAttr->pModelFilePath;
  stPostIspAttr.u32FrameBufCnt = pstPostIspAttr->u32FrameBufCnt;
  ret = RK_MPI_VPSS_SetGrpAIISPAttr(s32VpssGrp, &stPostIspAttr);
  if (RK_SUCCESS != ret)
    RK_LOGE("RK_MPI_VPSS_SetGrpAIISPAttr[%d] failed[%x]", s32VpssGrp, ret);

  return ret;
}

RKADK_S32 RKADK_MEDIA_SetPostIspAttr(RKADK_U32 u32CamId,
                                  RKADK_STREAM_TYPE_E enStrmType, bool bEnable,
                                  RKADK_POST_ISP_ATTR_S *pstPostIspAttr) {
  int ret;
  RKADK_S32 s32VpssGrp = -1, s32VpssChn = -1;
  AIISP_ATTR_S stPostIspAttr;

  RKADK_CHECK_CAMERAID(u32CamId, RKADK_FAILURE);

  RKADK_PARAM_GetVpssId(u32CamId, enStrmType, &s32VpssGrp, &s32VpssChn);
  if (s32VpssGrp == -1 || s32VpssChn == -1) {
    RKADK_LOGE("RKADK_PARAM_GetVpssId[%d, %d] failed", s32VpssGrp, s32VpssChn);
    return -1;
  }

  memset(&stPostIspAttr, 0, sizeof(AIISP_ATTR_S));
  ret = RK_MPI_VPSS_GetGrpAIISPAttr(s32VpssGrp, &stPostIspAttr);
  if (ret != RK_SUCCESS) {
    RK_LOGE("RK_MPI_VPSS_GetGrpAIISPAttr[%d] failed[%x]", s32VpssGrp, ret);
    return ret;
  }

  if (stPostIspAttr.bEnable == (RK_BOOL)bEnable)
    return 0;

  stPostIspAttr.bEnable = (RK_BOOL)bEnable;
  if (bEnable) {
    RKADK_CHECK_POINTER(pstPostIspAttr, RKADK_FAILURE);
    stPostIspAttr.stAiIspCallback.pfUpdateCallback = pstPostIspAttr->stAiIspCallback.pfUpdateCallback;
    stPostIspAttr.stAiIspCallback.pPrivateData = pstPostIspAttr->stAiIspCallback.pPrivateData;
    stPostIspAttr.pModelFilePath = pstPostIspAttr->pModelFilePath;
    stPostIspAttr.u32FrameBufCnt = pstPostIspAttr->u32FrameBufCnt;
  }

  ret = RK_MPI_VPSS_SetGrpAIISPAttr(s32VpssGrp, &stPostIspAttr);
  if (RK_SUCCESS != ret)
    RK_LOGE("RK_MPI_VPSS_SetGrpAIISPAttr[%d] failed[%x]", s32VpssGrp, ret);

  return ret;
}

RKADK_S32 RKADK_MEDIA_SetVencState(RKADK_U32 u32CamId, RKADK_S32 s32ChnId, bool state) {
  RKADK_S32 i;

  RKADK_MUTEX_LOCK(g_stMediaCtx.vencMutex);

  i = RKADK_MEDIA_GetIdx(g_stMediaCtx.stVencInfo, RKADK_MEDIA_VENC_MAX_CNT, 0,
                         s32ChnId, "VENC_STATE");
  if (i < 0) {
    RKADK_LOGE("not find matched index[%d] s32ChnId[%d]", i, s32ChnId);
    RKADK_MUTEX_UNLOCK(g_stMediaCtx.vencMutex);
    return -1;
  }

  if (g_stMediaCtx.stVencInfo[i].s32InitCnt > 0)
    g_stMediaCtx.stVencInfo[i].bReset = state;

  RKADK_MUTEX_UNLOCK(g_stMediaCtx.vencMutex);
  return 0;
}
