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
#include "rkadk_log.h"
#include "rkadk_media_comm.h"
#include "rkadk_param.h"
#include "rkadk_player.h"

#include <errno.h>
#include <getopt.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern int optind;
extern char *optarg;
static bool is_quit = false;
static RKADK_CHAR optstr[] = "i:x:y:W:H:r:mfvh";

static void print_usage(const RKADK_CHAR *name) {
  printf("usage example:\n");
  printf("\t%s [-i xxx.mp4] [-x 180] [-y 320] [-W 360] [-H 640] [-r 90] "
         "[-m] [-f] [-v]\n",
         name);
  printf("\t-i: input url, Default: /etc/bsa_file/8k8bpsMono.wav\n");
  printf("\t-x: display x coordinate, Default: 0\n");
  printf("\t-y: display y coordinate, Default: 0\n");
  printf("\t-W: display width, Default: Physical screen width\n");
  printf("\t-H: display height, Default: Physical screen height\n");
  printf("\t-r: rotation, option: 0, 90, 180, 270, Default: 0\n");
  printf("\t-m: mirror enable, Default: disable\n");
  printf("\t-f: flip enable, Default: disable\n");
  printf("\t-v: video enable, Default: disable\n");
  printf("\t-h: help\n");
}

static void sigterm_handler(int sig) {
  fprintf(stderr, "signal %d\n", sig);
  is_quit = true;
}

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
  case RKADK_PLAYER_EVENT_STARTED:
    printf("+++++ RKADK_PLAYER_EVENT_STARTED +++++\n");
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

void param_init(RKADK_PLAYER_FRAMEINFO_S *pstFrmInfo) {
  RKADK_CHECK_POINTER_N(pstFrmInfo);

  memset(pstFrmInfo, 0, sizeof(RKADK_PLAYER_FRAMEINFO_S));

  pstFrmInfo->u32DispWidth = MAX_VO_DISPLAY_WIDTH;
  pstFrmInfo->u32DispHeight = MAX_VO_DISPLAY_HEIGTHT;
  pstFrmInfo->u32ImgWidth = pstFrmInfo->u32DispWidth;
  pstFrmInfo->u32ImgHeight = pstFrmInfo->u32DispHeight;
  pstFrmInfo->u32VoLayerMode = 2;
  pstFrmInfo->u32VoFormat = VO_FORMAT_RGB888;
  pstFrmInfo->u32VoDev = VO_DEV_HD0;
  pstFrmInfo->u32EnIntfType = DISPLAY_TYPE_DEFAULT; // DISPLAY_TYPE_MIPI;
  pstFrmInfo->u32DispFrmRt = 29;
  pstFrmInfo->enIntfSync = RKADK_VO_OUTPUT_DEFAULT;
  pstFrmInfo->u32EnMode = CHNN_ASPECT_RATIO_AUTO;
  pstFrmInfo->u32BorderColor = 0x0000FA;
  pstFrmInfo->u32ChnnNum = 1;
  pstFrmInfo->stSyncInfo.bIdv = RKADK_TRUE;
  pstFrmInfo->stSyncInfo.bIhs = RKADK_TRUE;
  pstFrmInfo->stSyncInfo.bIvs = RKADK_TRUE;
  pstFrmInfo->stSyncInfo.bSynm = RKADK_TRUE;
  pstFrmInfo->stSyncInfo.bIop = RKADK_TRUE;
  pstFrmInfo->stSyncInfo.u16FrameRate = 60;
  pstFrmInfo->stSyncInfo.u16PixClock = 65000;
  pstFrmInfo->stSyncInfo.u16Hact = 1200;
  pstFrmInfo->stSyncInfo.u16Hbb = 24;
  pstFrmInfo->stSyncInfo.u16Hfb = 240;
  pstFrmInfo->stSyncInfo.u16Hpw = 136;
  pstFrmInfo->stSyncInfo.u16Hmid = 0;
  pstFrmInfo->stSyncInfo.u16Vact = 1200;
  pstFrmInfo->stSyncInfo.u16Vbb = 200;
  pstFrmInfo->stSyncInfo.u16Vfb = 194;
  pstFrmInfo->stSyncInfo.u16Vpw = 6;

  return;
}

int main(int argc, char *argv[]) {
  RKADK_PLAYER_FRAMEINFO_S stFrmInfo;
  int c, ret;
  char *file = "/userdata/16000_2.mp3";
  RKADK_BOOL bVideoEnable = true;
  RKADK_MW_PTR pPlayer = NULL;
  int retplayer = 0;

  param_init(&stFrmInfo);

  while ((c = getopt(argc, argv, optstr)) != -1) {
    switch (c) {
    case 'i':
      file = optarg;
      break;
    case 'x':
      stFrmInfo.stVoAttr.stChnRect.u32X = atoi(optarg);
      break;
    case 'y':
      stFrmInfo.stVoAttr.stChnRect.u32Y = atoi(optarg);
      break;
    case 'W':
      stFrmInfo.stVoAttr.stChnRect.u32Width = atoi(optarg);
      break;
    case 'H':
      stFrmInfo.stVoAttr.stChnRect.u32Height = atoi(optarg);
      break;
    case 'r':
      stFrmInfo.stVoAttr.u32Rotation = atoi(optarg);
      break;
    case 'm':
      stFrmInfo.stVoAttr.bMirror = true;
      break;
    case 'f':
      stFrmInfo.stVoAttr.bFlip = true;
      break;
    case 'v':
      bVideoEnable = true;
      break;
    case 'h':
    default:
      print_usage(argv[0]);
      optind = 0;
      return 0;
    }
  }
  optind = 0;

  RKADK_LOGD("#play file: %s", file);
  RKADK_LOGD(
      "#video display rect[%d, %d, %d, %d]", stFrmInfo.stVoAttr.stChnRect.u32X,
      stFrmInfo.stVoAttr.stChnRect.u32Y, stFrmInfo.stVoAttr.stChnRect.u32Width,
      stFrmInfo.stVoAttr.stChnRect.u32Height);
  RKADK_LOGD("#Rotation: %d", stFrmInfo.stVoAttr.u32Rotation);
  RKADK_LOGD("#mirror: %d", stFrmInfo.stVoAttr.bMirror);
  RKADK_LOGD("#flip: %d", stFrmInfo.stVoAttr.bFlip);

  signal(SIGINT, sigterm_handler);

  RKADK_MPI_SYS_Init();

  RKADK_PLAYER_CFG_S stPlayCfg;
  memset(&stPlayCfg, 0, sizeof(RKADK_PLAYER_CFG_S));
  stPlayCfg.bEnableAudio = true;
  if (bVideoEnable)
    stPlayCfg.bEnableVideo = true;

  stPlayCfg.pfnPlayerCallback = PlayerEventFnTest;
  if (RKADK_PLAYER_Create(&pPlayer, &stPlayCfg)) {
    RKADK_LOGE("RKADK_PLAYER_Create failed");
    return -1;
  }

  ret = RKADK_PLAYER_SetDataSource(pPlayer, file);
  if (ret) {
    RKADK_LOGE("SetDataSource failed, ret = %d", ret);
    return -1;
  }

  if (bVideoEnable)
    RKADK_PLAYER_SetVideoSink(pPlayer, &stFrmInfo);

  ret = RKADK_PLAYER_Prepare(pPlayer);
  if (ret) {
    RKADK_LOGE("Prepare failed, ret = %d", ret);
    return -1;
  }

  retplayer = RKADK_PLAYER_Play(pPlayer);
  if (retplayer) {
    RKADK_LOGE("Play failed, ret = %d", retplayer);
    return -1;
  }
  // RKADK_PLAYER_Seek(pPlayer, 1000); //seek 1s

  char cmd[64];
  printf("\n#Usage: input 'quit' to exit programe!\n"
         "peress any other key to capture one picture to file\n");
  while (!is_quit) {
    fgets(cmd, sizeof(cmd), stdin);
    RKADK_LOGD("#Input cmd: %s", cmd);
    if (strstr(cmd, "quit") || is_quit) {
      RKADK_LOGD("#Get 'quit' cmd!");
      if (retplayer) {
        goto __FAILED;
      }
      break;
    }

    if (retplayer) {
      goto __FAILED;
    }

    RKADK_PLAYER_Stop(pPlayer);

    ret = RKADK_PLAYER_SetDataSource(pPlayer, file);
    if (ret) {
      RKADK_LOGE("SetDataSource failed, ret = %d", ret);
      break;
    }
    ret = RKADK_PLAYER_Prepare(pPlayer);
    if (ret) {
      RKADK_LOGE("Prepare failed, ret = %d", ret);
      break;
    }

    retplayer = RKADK_PLAYER_Play(pPlayer);
    if (retplayer) {
      RKADK_LOGE("Play failed, ret = %d", retplayer);
      break;
    }

    usleep(500 * 1000);
  }

__FAILED:
  RKADK_PLAYER_Destroy(pPlayer);
  pPlayer = NULL;
  RKADK_MPI_SYS_Exit();
  return 0;
}
