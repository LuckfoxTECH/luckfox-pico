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

#include "isp/sample_isp.h"
#include "rkadk_common.h"
#include "rkadk_log.h"
#include "rkadk_media_comm.h"
#include "rkadk_param.h"
#include "rkadk_record.h"
#include <getopt.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern int optind;
extern char *optarg;

static RKADK_CHAR optstr[] = "a:I:p:h";

static bool is_quit = false;
#define IQ_FILE_PATH "/etc/iqfiles"

static void print_usage(const RKADK_CHAR *name) {
  printf("usage example:\n");
  printf("\t%s [-a /etc/iqfiles] [-I 0] [-t 0]\n", name);
  printf("\t-a: enable aiq with dirpath provided, eg:-a "
         "/oem/etc/iqfiles/, Default /oem/etc/iqfiles,"
         "without this option aiq should run in other application\n");
  printf("\t-I: Camera id, Default:0\n");
  printf("\t-p: param ini directory path, Default:/data/rkadk\n");
}

static RKADK_S32
GetRecordFileName(RKADK_MW_PTR pRecorder, RKADK_U32 u32FileCnt,
                  RKADK_CHAR (*paszFilename)[RKADK_MAX_FILE_PATH_LEN]) {
  static RKADK_U32 u32FileIdx = 0;

  RKADK_LOGD("u32FileCnt:%d, pRecorder:%p", u32FileCnt, pRecorder);

  if (u32FileIdx >= 10)
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

static void sigterm_handler(int sig) {
  fprintf(stderr, "signal %d\n", sig);
  is_quit = true;
}

int main(int argc, char *argv[]) {
  int c, ret, fps;
  RKADK_RECORD_ATTR_S stRecAttr;
  RKADK_CHAR *pIqfilesPath = IQ_FILE_PATH;
  RKADK_MW_PTR pRecorder = NULL;
  RK_BOOL fec_enable = RK_FALSE;
  RKADK_REC_MANUAL_SPLIT_ATTR_S stSplitAttr;
  const char *iniPath = NULL;
  RKADK_PARAM_RES_E type;
  RKADK_PARAM_CODEC_CFG_S stCodecType;
  char path[RKADK_PATH_LEN];
  char sensorPath[RKADK_MAX_SENSOR_CNT][RKADK_PATH_LEN];
  rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;

  // set default value
  stRecAttr.s32CamID = 0;
  stRecAttr.pfnRequestFileNames = GetRecordFileName;
  stRecAttr.pfnEventCallback = RecordEventCallback;

  while ((c = getopt(argc, argv, optstr)) != -1) {
    const char *tmp_optarg = optarg;
    switch (c) {
    case 'a':
      if (!optarg && NULL != argv[optind] && '-' != argv[optind][0]) {
        tmp_optarg = argv[optind++];
      }

      if (tmp_optarg)
        pIqfilesPath = (char *)tmp_optarg;
      break;
    case 'I':
      stRecAttr.s32CamID = atoi(optarg);
      break;
    case 'p':
      iniPath = optarg;
      RKADK_LOGD("iniPath: %s", iniPath);
      break;
    case 'h':
    default:
      print_usage(argv[0]);
      optind = 0;
      return -1;
    }
  }
  optind = 0;

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
  ret = RKADK_PARAM_GetCamParam(stRecAttr.s32CamID, RKADK_PARAM_TYPE_FPS, &fps);
  if (ret) {
    RKADK_LOGE("RKADK_PARAM_GetCamParam fps failed");
    return -1;
  }

  SAMPLE_ISP_Start(stRecAttr.s32CamID, hdr_mode, fec_enable, pIqfilesPath, fps);

  IspProcess(stRecAttr.s32CamID);
#endif

  if (RKADK_RECORD_Create(&stRecAttr, &pRecorder)) {
    RKADK_LOGE("Create recorder failed");
#ifdef RKAIQ
    SAMPLE_ISP_Stop(stRecAttr.s32CamID);
#endif
    return -1;
  }

  RKADK_LOGD("initial finish\n");

  RKADK_RECORD_Start(pRecorder);

  signal(SIGINT, sigterm_handler);
  char cmd[64];
  printf("\n#Usage: input 'quit' to exit programe!\n"
         "peress any other key to quit\n");

  while (!is_quit) {
    fgets(cmd, sizeof(cmd), stdin);
    if (strstr(cmd, "quit") || is_quit) {
      RKADK_LOGD("#Get 'quit' cmd!");
      break;
    } else if (strstr(cmd, "MS")) { // Manual Split
      RKADK_PARAM_REC_TIME_S stRecTime;
      stRecTime.enStreamType = RKADK_STREAM_TYPE_VIDEO_MAIN;
      stRecTime.time = 20; // default time

      RKADK_PARAM_GetCamParam(0, RKADK_PARAM_TYPE_SPLITTIME, &stRecTime);
      stSplitAttr.enManualType = MUXER_PRE_MANUAL_SPLIT;
      stSplitAttr.u32DurationSec = stRecTime.time;
      RKADK_RECORD_ManualSplit(pRecorder, &stSplitAttr);
    } else if (strstr(cmd, "LR")) { // Lapse Record
      type = RKADK_REC_TYPE_LAPSE;
      RKADK_PARAM_SetCamParam(0, RKADK_PARAM_TYPE_RECORD_TYPE, &type);
      RKADK_RECORD_Reset(&pRecorder);
    } else if (strstr(cmd, "NR")) { // Normal Record
      type = RKADK_REC_TYPE_NORMAL;
      RKADK_PARAM_SetCamParam(0, RKADK_PARAM_TYPE_RECORD_TYPE, &type);
      RKADK_RECORD_Reset(&pRecorder);
    } else if (strstr(cmd, "1620")) {
      type = RKADK_RES_1620P;
      RKADK_PARAM_SetCamParam(0, RKADK_PARAM_TYPE_RES, &type);
      RKADK_RECORD_Reset(&pRecorder);
    } else if (strstr(cmd, "1600")) {
      type = RKADK_RES_1600P;
      RKADK_PARAM_SetCamParam(0, RKADK_PARAM_TYPE_RES, &type);
      RKADK_RECORD_Reset(&pRecorder);
    } else if (strstr(cmd, "1440")) {
      type = RKADK_RES_1440P;
      RKADK_PARAM_SetCamParam(0, RKADK_PARAM_TYPE_RES, &type);
      RKADK_RECORD_Reset(&pRecorder);
    } else if (strstr(cmd, "264")) {
      stCodecType.enCodecType = RKADK_CODEC_TYPE_H264;
      stCodecType.enStreamType = RKADK_STREAM_TYPE_VIDEO_MAIN;
      RKADK_PARAM_SetCamParam(0, RKADK_PARAM_TYPE_CODEC_TYPE, &stCodecType);
      stCodecType.enStreamType = RKADK_STREAM_TYPE_VIDEO_SUB;
      RKADK_PARAM_SetCamParam(0, RKADK_PARAM_TYPE_CODEC_TYPE, &stCodecType);
      RKADK_RECORD_Reset(&pRecorder);
    } else if (strstr(cmd, "265")) {
      stCodecType.enCodecType = RKADK_CODEC_TYPE_H265;
      stCodecType.enStreamType = RKADK_STREAM_TYPE_VIDEO_MAIN;
      RKADK_PARAM_SetCamParam(0, RKADK_PARAM_TYPE_CODEC_TYPE, &stCodecType);
      stCodecType.enStreamType = RKADK_STREAM_TYPE_VIDEO_SUB;
      RKADK_PARAM_SetCamParam(0, RKADK_PARAM_TYPE_CODEC_TYPE, &stCodecType);
      RKADK_RECORD_Reset(&pRecorder);
    } else if (strstr(cmd, "start")) {
      RKADK_RECORD_Start(pRecorder);
    } else if (strstr(cmd, "stop")) {
      RKADK_RECORD_Stop(pRecorder);
    }

    usleep(500000);
  }

  RKADK_RECORD_Stop(pRecorder);
  RKADK_RECORD_Destroy(pRecorder);

#ifdef RKAIQ
  SAMPLE_ISP_Stop(stRecAttr.s32CamID);
#endif
  RKADK_MPI_SYS_Exit();
  RKADK_LOGD("exit!");
  return 0;
}
