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

#include "rkadk_common.h"
#include "rkadk_media_comm.h"
#include "rkadk_log.h"
#include "rkadk_param.h"
#include "rkadk_record.h"
#include "rkadk_photo.h"
#include "rkadk_player.h"
#include "rkadk_stream.h"
#include "rkadk_rtsp.h"
#include "isp/sample_isp.h"
#include <getopt.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//#define ENABLE_PALYER
//#define ENABLE_RTSP
//#define ENABLE_STREAM_AUDIO
//#define ENABLE_STREAM_VIDEO
#define ENABLE_PHOTO
//#define ENABLE_RESET

extern int optind;
extern char *optarg;

static RKADK_CHAR optstr[] = "a:I:p:P:m:";

#ifdef ENABLE_STREAM_AUDIO
static FILE *g_aenc_file = NULL;
static FILE *g_pcm_file = NULL;
static char *g_aenc_path = "/mnt/sdcard/aenc.bin";
static char *g_pcm_path = "/mnt/sdcard/ai.pcm";
#endif

#ifdef ENABLE_STREAM_VIDEO
static FILE *g_output_file = NULL;
static char *g_venc_path = "/mnt/sdcard/venc.bin";
#endif

static bool is_quit = false;
#define IQ_FILE_PATH "/etc/iqfiles"

static void print_usage(const RKADK_CHAR *name) {
  printf("usage example:\n");
  printf("\t%s [-a /etc/iqfiles] [-I 0] [-p 0] [-P /data/photo.wav]\n", name);
  printf("\t-a: enable aiq with dirpath provided, eg:-a "
         "/oem/etc/iqfiles/, Default /oem/etc/iqfiles,"
         "without this option aiq should run in other application\n");
  printf("\t-I: Camera id, Default:0\n");
  printf("\t-p: param ini directory path, Default:/data/rkadk\n");
  printf("\t-P: player audio path, Default:/mnt/sdcard/photo.wav\n");
  printf("\t-m: record/photo multiple sensors, Default:0, options: 1(all isp sensors), 2(isp+ahd sensors)\n");
}

static RKADK_S32
GetRecordFileName(RKADK_MW_PTR pRecorder, RKADK_U32 u32FileCnt,
                  RKADK_CHAR (*paszFilename)[RKADK_MAX_FILE_PATH_LEN]) {
  static RKADK_U32 u32FileIdx = 0;

  RKADK_LOGD("u32FileCnt:%d, pRecorder:%p", u32FileCnt, pRecorder);

  if (u32FileIdx >= 100)
    u32FileIdx = 0;

  for (RKADK_U32 i = 0; i < u32FileCnt; i++) {
    sprintf(paszFilename[i], "/mnt/sdcard/RecordTest_%u.mp4", u32FileIdx);
    u32FileIdx++;
  }

  return 0;
}

static RKADK_VOID
RecordEventCallback(RKADK_MW_PTR pRecorder,
                    const RKADK_MUXER_EVENT_INFO_S *pstEventInfo) {
  switch (pstEventInfo->enEvent) {
  case RKADK_MUXER_EVENT_STREAM_START:
    printf("+++++ RKADK_MUXER_EVENT_STREAM_START +++++\n");
    break;
  case RKADK_MUXER_EVENT_STREAM_STOP:
    printf("+++++ RKADK_MUXER_EVENT_STREAM_STOP +++++\n");
    break;
  case RKADK_MUXER_EVENT_FILE_BEGIN:
    printf("+++++ RKADK_MUXER_EVENT_FILE_BEGIN +++++\n");
    printf("\tstFileInfo: %s\n",
           pstEventInfo->unEventInfo.stFileInfo.asFileName);
    printf("\tu32Duration: %d\n",
           pstEventInfo->unEventInfo.stFileInfo.u32Duration);
    break;
  case RKADK_MUXER_EVENT_FILE_END:
    printf("+++++ RKADK_MUXER_EVENT_FILE_END +++++\n");
    printf("\tstFileInfo: %s\n",
           pstEventInfo->unEventInfo.stFileInfo.asFileName);
    printf("\tu32Duration: %d\n",
           pstEventInfo->unEventInfo.stFileInfo.u32Duration);
    break;
  case RKADK_MUXER_EVENT_MANUAL_SPLIT_END:
    printf("+++++ RKADK_MUXER_EVENT_MANUAL_SPLIT_END +++++\n");
    printf("\tstFileInfo: %s\n",
           pstEventInfo->unEventInfo.stFileInfo.asFileName);
    printf("\tu32Duration: %d\n",
           pstEventInfo->unEventInfo.stFileInfo.u32Duration);
    break;
  case RKADK_MUXER_EVENT_ERR_CREATE_FILE_FAIL:
    printf("+++++ RKADK_MUXER_EVENT_ERR_CREATE_FILE_FAIL +++++\n");
    break;
  case RKADK_MUXER_EVENT_ERR_WRITE_FILE_FAIL:
    printf("+++++ RKADK_MUXER_EVENT_ERR_WRITE_FILE_FAIL +++++\n");
    break;
  default:
    printf("+++++ Unknown event(%d) +++++\n", pstEventInfo->enEvent);
    break;
  }
}

#if ENABLE_PALYER
static RKADK_VOID PlayerEventFnTest(RKADK_MW_PTR pPlayer,
                                    RKADK_PLAYER_EVENT_E enEvent,
                                    RKADK_VOID *pData) {
  int position = 0;

  switch (enEvent) {
  case RKADK_PLAYER_EVENT_STATE_CHANGED:
    printf("+++++ RKADK_PLAYER_EVENT_STATE_CHANGED +++++\n");
    break;
  case RKADK_PLAYER_EVENT_EOF:
    printf("+++++ RKADK_PLAYER_EVENT_EOF +++++\n");
    break;
  case RKADK_PLAYER_EVENT_SOF:
    printf("+++++ RKADK_PLAYER_EVENT_SOF +++++\n");
    break;
  case RKADK_PLAYER_EVENT_PROGRESS:
    if (pData)
      position = *((int *)pData);
    printf("+++++ RKADK_PLAYER_EVENT_PROGRESS(%d ms) +++++\n", position);
    break;
  case RKADK_PLAYER_EVENT_SEEK_END:
    printf("+++++ RKADK_PLAYER_EVENT_SEEK_END +++++\n");
    break;
  case RKADK_PLAYER_EVENT_ERROR:
    printf("+++++ RKADK_PLAYER_EVENT_ERROR +++++\n");
    break;
  case RKADK_PLAYER_EVENT_PREPARED:
    printf("+++++ RKADK_PLAYER_EVENT_PREPARED +++++\n");
    break;
  case RKADK_PLAYER_EVENT_PLAY:
    printf("+++++ RKADK_PLAYER_EVENT_PLAY +++++\n");
    break;
  case RKADK_PLAYER_EVENT_PAUSED:
    printf("+++++ RKADK_PLAYER_EVENT_PAUSED +++++\n");
    break;
  case RKADK_PLAYER_EVENT_STOPPED:
    printf("+++++ RKADK_PLAYER_EVENT_STOPPED +++++\n");
    break;
  default:
    printf("+++++ Unknown event(%d) +++++\n", enEvent);
    break;
  }
}
#endif

#ifdef RKAIQ
static int IspProcess(RKADK_S32 u32CamId) {
  int ret;
  bool mirror = false, flip = false;

  // set mirror flip
  ret = RKADK_PARAM_GetCamParam(u32CamId, RKADK_PARAM_TYPE_MIRROR, &mirror);
  if (ret)
    RKADK_LOGE("RKADK_PARAM_GetCamParam mirror failed");

  ret = RKADK_PARAM_GetCamParam(u32CamId, RKADK_PARAM_TYPE_FLIP, &flip);
  if (ret)
    RKADK_LOGE("RKADK_PARAM_GetCamParam flip failed");

  if (mirror || flip) {
    ret = SAMPLE_ISP_SET_MirrorFlip(u32CamId, mirror, flip);
    if (ret)
      RKADK_LOGE("SAMPLE_ISP_SET_MirrorFlip failed");
  }

#ifdef RKADK_DUMP_ISP_RESULT
  // mirror flip
  ret = SAMPLE_ISP_GET_MirrorFlip(u32CamId, &mirror, &flip);
  if (ret)
    RKADK_LOGE("SAMPLE_ISP_GET_MirrorFlip failed");
  else
    RKADK_LOGD("GET mirror = %d, flip = %d", mirror, flip);
#endif

  return 0;
}
#endif

#ifdef ENABLE_STREAM_AUDIO
static RKADK_S32 AencDataCb(RKADK_AUDIO_STREAM_S *pAStreamData) {
  if (!g_aenc_file) {
    g_aenc_file = fopen(g_aenc_path, "w");
    if (!g_aenc_file) {
      RKADK_LOGE("open %s file failed, exit", g_aenc_path);
      return -1;
    }
  }

  fwrite(pAStreamData->pStream, 1, pAStreamData->u32Len, g_aenc_file);

  return 0;
}

static RKADK_S32 PcmDataCb(RKADK_AUDIO_STREAM_S *pAStreamData) {

  if (!g_pcm_file) {
    g_pcm_file = fopen(g_pcm_path, "w");
    if (!g_pcm_file) {
      RKADK_LOGE("open %s file failed, exit", g_pcm_path);
      return -1;
    }
  }

  fwrite(pAStreamData->pStream, 1, pAStreamData->u32Len, g_pcm_file);

  return 0;
}
#endif

#ifdef ENABLE_STREAM_VIDEO
static RKADK_S32 VencDataCb(RKADK_VIDEO_STREAM_S *pVStreamData) {
  if (!g_output_file) {
    g_output_file = fopen(g_venc_path, "w");
    if (!g_output_file) {
      RKADK_LOGE("open %s file failed, exit", g_venc_path);
      return -1;
    }
  }

  fwrite(pVStreamData->astPack.apu8Addr, 1, pVStreamData->astPack.au32Len, g_output_file);
  return 0;
}
#endif

#ifdef ENABLE_PHOTO
static void PhotoDataRecv(RKADK_PHOTO_RECV_DATA_S *pstData) {
  static RKADK_U32 photoId = 0;
  char jpegPath[128];
  FILE *file = NULL;

  if (!pstData || !pstData->pu8DataBuf) {
    RKADK_LOGE("Invalid photo data");
    return;
  }

  memset(jpegPath, 0, 128);
  sprintf(jpegPath, "/mnt/sdcard/PhotoTest_%d.jpeg", photoId);
  file = fopen(jpegPath, "w");
  if (!file) {
    RKADK_LOGE("Create jpeg file(%s) failed", jpegPath);
    return;
  }

  RKADK_LOGD("save u32CamId[%d] jpeg to %s", pstData->u32CamId, jpegPath);

  fwrite(pstData->pu8DataBuf, 1, pstData->u32DataLen, file);
  fclose(file);

  photoId++;
  if (photoId > 100)
    photoId = 0;
}
#endif

static void sigterm_handler(int sig) {
  fprintf(stderr, "signal %d\n", sig);
  is_quit = true;
}

int main(int argc, char *argv[]) {
  int c, ret, inCmd = 0;
  RKADK_BOOL bMultiCam = RKADK_FALSE;
  RKADK_BOOL bMultiSensor = RK_FALSE;
  const char *iniPath = NULL;
  char *file = "/mnt/sdcard/photo.wav";
  char path[RKADK_PATH_LEN];
  char sensorPath[RKADK_MAX_SENSOR_CNT][RKADK_PATH_LEN];
  RKADK_U32 u32CamId = 0;
  //record
  RKADK_RECORD_ATTR_S stRecAttr;
  RKADK_MW_PTR pRecordHandle = NULL, pRecordHandle1 = NULL;

#ifdef RKAIQ
  RKADK_PARAM_FPS_S stFps;
  const char *tmp_optarg = optarg;
  SAMPLE_ISP_PARAM stIspParam;

  memset(&stIspParam, 0, sizeof(SAMPLE_ISP_PARAM));
  stIspParam.iqFileDir = IQ_FILE_PATH;
#endif

#ifdef ENABLE_RESET
  int change;
  RKADK_PARAM_RES_E stResType;
  RKADK_PARAM_CODEC_CFG_S stCodecType;
  RKADK_PARAM_REC_TIME_S stRecTime;
  RKADK_REC_TYPE_E stRecType;
  RKADK_REC_MANUAL_SPLIT_ATTR_S stSplitAttr;
#endif

#ifdef ENABLE_PHOTO
  //photo
  RKADK_PHOTO_ATTR_S stPhotoAttr;
  RKADK_TAKE_PHOTO_ATTR_S stTakePhotoAttr;
  RKADK_MW_PTR pPhotoHandle = NULL, pPhotoHandle1 = NULL;
#endif

#ifdef ENABLE_STREAM_AUDIO
  //stream
  RKADK_STREAM_AUDIO_ATTR_S stAudioAttr;
  RKADK_MW_PTR pAudioHandle = NULL;
#endif

#ifdef ENABLE_STREAM_VIDEO
RKADK_MW_PTR pVideoHandle = NULL;
RKADK_STREAM_VIDEO_ATTR_S stVideoAttr;
#endif

#ifdef ENABLE_RTSP
  //rtsp
  RKADK_MW_PTR pRtsHandle = NULL;
#endif

#ifdef ENABLE_PLAYER
  //player
  RKADK_PLAYER_CFG_S stPlayCfg;
  RKADK_MW_PTR pPlayerHandle = NULL;
#endif

  while ((c = getopt(argc, argv, optstr)) != -1) {
    switch (c) {
#ifdef RKAIQ
    case 'a':
      if (!optarg && NULL != argv[optind] && '-' != argv[optind][0]) {
        tmp_optarg = argv[optind++];
      }

      if (tmp_optarg)
        stIspParam.iqFileDir = (char *)tmp_optarg;
      break;
#endif
    case 'I':
      u32CamId = atoi(optarg);
      break;
    case 'p':
      iniPath = optarg;
      RKADK_LOGD("iniPath: %s", iniPath);
    case 'P':
      file = optarg;
      RKADK_LOGD("player audio file: %s", file);
      break;
    case 'm':
      inCmd = atoi(optarg);
      if (inCmd == 1) {
        bMultiCam = RKADK_TRUE;
        bMultiSensor = RKADK_TRUE;
      } else if (inCmd == 2)
        bMultiSensor = RKADK_TRUE;
      RKADK_LOGD("enable bMultiCam: %d, bMultiSensor: %d", bMultiCam, bMultiSensor);
      break;
    default:
      print_usage(argv[0]);
      optind = 0;
      return -1;
    }
  }
  optind = 0;

  if (bMultiSensor)
    u32CamId = 0;

  stRecAttr.s32CamID = u32CamId;
  stRecAttr.pfnRequestFileNames = GetRecordFileName;
  stRecAttr.pfnEventCallback = RecordEventCallback;

#ifdef ENABLE_PHOTO
  memset(&stTakePhotoAttr, 0, sizeof(RKADK_TAKE_PHOTO_ATTR_S));
  stTakePhotoAttr.enPhotoType = RKADK_PHOTO_TYPE_SINGLE;

  memset(&stPhotoAttr, 0, sizeof(RKADK_PHOTO_ATTR_S));
  stPhotoAttr.u32CamId = u32CamId;
  stPhotoAttr.pfnPhotoDataProc = PhotoDataRecv;
  stPhotoAttr.stThumbAttr.bSupportDCF = RKADK_FALSE;
  stPhotoAttr.stThumbAttr.stMPFAttr.eMode = RKADK_PHOTO_MPF_SINGLE;
  stPhotoAttr.stThumbAttr.stMPFAttr.sCfg.u8LargeThumbNum = 1;
  stPhotoAttr.stThumbAttr.stMPFAttr.sCfg.astLargeThumbSize[0].u32Width = 320;
  stPhotoAttr.stThumbAttr.stMPFAttr.sCfg.astLargeThumbSize[0].u32Height = 180;
#endif

#ifdef ENABLE_STREAM_AUDIO
  memset(&stAudioAttr, 0, sizeof(RKADK_STREAM_AUDIO_ATTR_S));
  stAudioAttr.u32CamId = u32CamId;
  stAudioAttr.enCodecType = RKADK_CODEC_TYPE_PCM;
  stAudioAttr.pfnPcmDataCB = PcmDataCb;
  stAudioAttr.pfnAencDataCB = AencDataCb;
#endif

#ifdef ENABLE_STREAM_VIDEO
  memset(&stVideoAttr, 0, sizeof(RKADK_STREAM_VIDEO_ATTR_S));
  stVideoAttr.pfnDataCB = VencDataCb;
  stVideoAttr.u32CamId = u32CamId;
#endif

#if ENABLE_PALYER
  memset(&stPlayCfg, 0, sizeof(RKADK_PLAYER_CFG_S));
  stPlayCfg.bEnableAudio = true;
  stPlayCfg.pfnPlayerCallback = PlayerEventFnTest;
#endif

  RKADK_MPI_SYS_Init();

  if (iniPath) {
    memset(path, 0, RKADK_PATH_LEN);
    memset(sensorPath, 0, RKADK_MAX_SENSOR_CNT * RKADK_PATH_LEN);
    sprintf(path, "%s/rkadk_setting.ini", iniPath);
    for (int i = 0; i < RKADK_MAX_SENSOR_CNT; i++)
      sprintf(sensorPath[i], "%s/rkadk_setting_sensor_%d.ini", iniPath, i);

    /*
    lg:
      char *sPath[] = {"/data/rkadk/rkadk_setting_sensor_0.ini",
      "/data/rkadk/rkadk_setting_sensor_1.ini", NULL};
    */
    char *sPath[] = {sensorPath[0], sensorPath[1], NULL};

    RKADK_PARAM_Init(path, sPath);
  } else {
    RKADK_PARAM_Init(NULL, NULL);
  }

#ifdef RKAIQ
  stFps.enStreamType = RKADK_STREAM_TYPE_SENSOR;
  ret = RKADK_PARAM_GetCamParam(u32CamId, RKADK_PARAM_TYPE_FPS, &stFps);
  if (ret) {
    RKADK_LOGE("RKADK_PARAM_GetCamParam u32CamId[%d] fps failed", u32CamId);
    return -1;
  }

  stIspParam.WDRMode = RK_AIQ_WORKING_MODE_NORMAL;
  stIspParam.bMultiCam = bMultiCam;
  stIspParam.fps = stFps.u32Framerate;
  SAMPLE_ISP_Start(u32CamId, stIspParam);
  //IspProcess(u32CamId);

  if (bMultiCam) {
    ret = RKADK_PARAM_GetCamParam(1, RKADK_PARAM_TYPE_FPS, &stFps);
    if (ret) {
      RKADK_LOGE("RKADK_PARAM_GetCamParam u32CamId[1] fps failed");
      SAMPLE_ISP_Stop(u32CamId);
      return -1;
    }

    SAMPLE_ISP_Start(1, stIspParam);
  }
#endif

  ret = RKADK_RECORD_Create(&stRecAttr, &pRecordHandle);
  if (ret) {
    RKADK_LOGE("Create recorder u32CamId[%d] failed", u32CamId);
    goto _FAILURE;
  }
  RKADK_RECORD_Start(pRecordHandle);

  if (bMultiSensor) {
    stRecAttr.s32CamID = 1;
    if (RKADK_RECORD_Create(&stRecAttr, &pRecordHandle1)) {
      RKADK_LOGE("Create recorder u32CamId[1] failed");
      goto _FAILURE;
    }
    RKADK_RECORD_Start(pRecordHandle1);
  }

#ifdef ENABLE_PHOTO
  ret = RKADK_PHOTO_Init(&stPhotoAttr, &pPhotoHandle);
  if (ret) {
    RKADK_LOGE("Create u32CamId[%d] photo failed", u32CamId);
    goto _FAILURE;
  }

  if (bMultiSensor) {
    stPhotoAttr.u32CamId = 1;
    ret = RKADK_PHOTO_Init(&stPhotoAttr, &pPhotoHandle1);
    if (ret) {
      RKADK_LOGE("Create u32CamId[1] photo failed");
      goto _FAILURE;
    }
  }
#endif

#ifdef ENABLE_STREAM_AUDIO
  ret = RKADK_STREAM_AudioInit(&stAudioAttr, &pAudioHandle);
  if (ret) {
    RKADK_LOGE("RKADK_STREAM_AudioInit failed = %d", ret);
    goto _FAILURE;
  }

  ret = RKADK_STREAM_AencStart(pAudioHandle);
  if (ret) {
    RKADK_LOGE("RKADK_STREAM_AencStart failed");
    goto _FAILURE;
  }
#endif

#ifdef ENABLE_STREAM_VIDEO
  ret = RKADK_STREAM_VideoInit(&stVideoAttr, &pVideoHandle);
  if (ret) {
    RKADK_LOGE("RKADK_STREAM_VideoInit failed = %d", ret);
    goto _FAILURE;
  }

  ret = RKADK_STREAM_VencStart(pVideoHandle, -1);
  if (ret) {
    RKADK_LOGE("RKADK_STREAM_VencStart failed");
    goto _FAILURE;
  }
#endif

#ifdef ENABLE_RTSP
  ret = RKADK_RTSP_Init(u32CamId, 554, "/live/main_stream", &pRtsHandle);
  if (ret) {
    RKADK_LOGE("RKADK_RTSP_Init failed");
    goto _FAILURE;
  }

  RKADK_RTSP_Start(pRtsHandle);
#endif

#if ENABLE_PALYER
  if (RKADK_PLAYER_Create(&pPlayerHandle, &stPlayCfg)) {
    RKADK_LOGE("RKADK_PLAYER_Create failed");
    goto _FAILURE;
  }
#endif

  signal(SIGINT, sigterm_handler);
  while (!is_quit) {
#ifdef ENABLE_RESET
    change = rand() % 4;
    if (change == 0) {
      RKADK_LOGI("===================Rand type [%d] switch resolution enter", change);
      stResType++;
      if (stResType >= RKADK_RES_1944P)
        stResType = RKADK_RES_1080P;

      RKADK_PARAM_SetCamParam(u32CamId, RKADK_PARAM_TYPE_RES, &stResType);
      RKADK_PARAM_SetCamParam(u32CamId, RKADK_PARAM_TYPE_PHOTO_RES, &stResType);
      RKADK_RECORD_Reset(&pRecordHandle);

#ifdef ENABLE_PHOTO
      RKADK_PHOTO_Reset(&pPhotoHandle);
      RKADK_PHOTO_TakePhoto(pPhotoHandle, &stTakePhotoAttr);
#endif

#if ENABLE_PALYER
      RKADK_PLAYER_SetDataSource(pPlayerHandle, file);
      RKADK_PLAYER_Prepare(pPlayerHandle);
      RKADK_PLAYER_Play(pPlayerHandle);
      usleep(50000);
      RKADK_PLAYER_Stop(pPlayerHandle);
#endif
      RKADK_LOGI("===================Reset resolution = %d", stResType);
    } else if (change == 1) {
      RKADK_LOGI("===================Rand type [%d] switch encode types enter", change);
      stCodecType.enCodecType++;
      if (stCodecType.enCodecType >= RKADK_CODEC_TYPE_MJPEG)
        stCodecType.enCodecType = RKADK_CODEC_TYPE_H264;

      stCodecType.enStreamType = RKADK_STREAM_TYPE_VIDEO_MAIN;
      RKADK_PARAM_SetCamParam(u32CamId, RKADK_PARAM_TYPE_CODEC_TYPE, &stCodecType);
      RKADK_RECORD_Reset(&pRecordHandle);

#ifdef ENABLE_PHOTO
      RKADK_PHOTO_TakePhoto(pPhotoHandle, &stTakePhotoAttr);
#endif

#if ENABLE_PALYER
      RKADK_PLAYER_SetDataSource(pPlayerHandle, file);
      RKADK_PLAYER_Prepare(pPlayerHandle);
      RKADK_PLAYER_Play(pPlayerHandle);
      usleep(50000);
      RKADK_PLAYER_Stop(pPlayerHandle);
#endif
      RKADK_LOGI("===================Reset encode type = %d", stCodecType.enCodecType);
    } else if (change == 2) {
      RKADK_LOGI("Rand type [%d] switch recrd types enter", change);
      stRecType++;
      if (stRecType >= RKADK_REC_TYPE_BUTT)
        stRecType = RKADK_REC_TYPE_NORMAL;
      RKADK_PARAM_SetCamParam(u32CamId, RKADK_PARAM_TYPE_RECORD_TYPE, &stRecType);
      RKADK_RECORD_Reset(&pRecordHandle);

#ifdef ENABLE_PHOTO
      RKADK_PHOTO_TakePhoto(pPhotoHandle, &stTakePhotoAttr);
#endif

#if ENABLE_PALYER
      RKADK_PLAYER_SetDataSource(pPlayerHandle, file);
      RKADK_PLAYER_Prepare(pPlayerHandle);
      RKADK_PLAYER_Play(pPlayerHandle);
      usleep(50000);
      RKADK_PLAYER_Stop(pPlayerHandle);
#endif
      RKADK_LOGI("===================Reset record type = %d", stRecType);
    } else {
      RKADK_LOGI("===================Rand type [%d] switch manual types enter", change);
      stRecTime.enStreamType++;
      if (stRecTime.enStreamType >= RKADK_STREAM_TYPE_SNAP)
        stRecTime.enStreamType = RKADK_STREAM_TYPE_VIDEO_MAIN;
      stRecTime.time = 20; //default time

      RKADK_PARAM_GetCamParam(u32CamId, RKADK_PARAM_TYPE_SPLITTIME, &stRecTime);
      stSplitAttr.enManualType = MUXER_PRE_MANUAL_SPLIT;
      stSplitAttr.u32DurationSec = stRecTime.time;
      RKADK_RECORD_ManualSplit(pRecordHandle, &stSplitAttr);

#ifdef ENABLE_PHOTO
      RKADK_PHOTO_TakePhoto(pPhotoHandle, &stTakePhotoAttr);
#endif

#if ENABLE_PALYER
      RKADK_PLAYER_SetDataSource(pPlayerHandle, file);
      RKADK_PLAYER_Prepare(pPlayerHandle);
      RKADK_PLAYER_Play(pPlayerHandle);
      usleep(50000);
      RKADK_PLAYER_Stop(pPlayerHandle);
#endif
      RKADK_LOGI("===================Manual record stream = %d", stRecTime.enStreamType);
    }
#else
    RKADK_PHOTO_TakePhoto(pPhotoHandle, &stTakePhotoAttr);
    if (bMultiSensor) {
      if (RKADK_PHOTO_TakePhoto(pPhotoHandle1, &stTakePhotoAttr))
        RKADK_LOGE("RKADK_PHOTO_TakePhoto u32CamId[1] failed");
    }
#endif
    usleep(5000000);
  }

_FAILURE:
  RKADK_LOGD("exit!");

#ifdef ENABLE_RTSP
  RKADK_RTSP_Stop(pRtsHandle);
  RKADK_RTSP_DeInit(pRtsHandle);
#endif

  RKADK_RECORD_Stop(pRecordHandle);
  RKADK_RECORD_Destroy(pRecordHandle);

  if (bMultiSensor) {
    RKADK_RECORD_Stop(pRecordHandle1);
    RKADK_RECORD_Destroy(pRecordHandle1);
  }

#ifdef ENABLE_PHOTO
  RKADK_PHOTO_DeInit(pPhotoHandle);
  if (bMultiSensor)
    RKADK_PHOTO_DeInit(pPhotoHandle1);
#endif

#ifdef ENABLE_STREAM_AUDIO
  RKADK_STREAM_AencStop(pAudioHandle);
  RKADK_STREAM_AudioDeInit(pAudioHandle);
  if (g_aenc_file)
    fclose(g_aenc_file);

  if (g_pcm_file)
    fclose(g_pcm_file);
#endif

#ifdef ENABLE_STREAM_VIDEO
  RKADK_STREAM_VencStop(pVideoHandle);
  RKADK_STREAM_VideoDeInit(pVideoHandle);
  if (g_output_file) {
    fclose(g_output_file);
    g_output_file = NULL;
  }
#endif

#if ENABLE_PALYER
  RKADK_PLAYER_Stop(pPlayerHandle);
  RKADK_PLAYER_Destroy(pPlayerHandle);
#endif

#ifdef RKAIQ
  SAMPLE_ISP_Stop(stRecAttr.s32CamID);

  if (bMultiCam)
    SAMPLE_ISP_Stop(1);
#endif
  RKADK_MPI_SYS_Exit();
  return 0;
}
