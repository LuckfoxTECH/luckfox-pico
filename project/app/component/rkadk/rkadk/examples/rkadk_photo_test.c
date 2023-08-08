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
#include "rkadk_photo.h"
#include <getopt.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern int optind;
extern char *optarg;

static bool is_quit = false;
static RKADK_CHAR optstr[] = "a:I:p:t:h";

#define IQ_FILE_PATH "/etc/iqfiles"

static RKADK_THUMB_TYPE_E enDataType = RKADK_THUMB_TYPE_NV12;
static void print_usage(const RKADK_CHAR *name) {
  printf("usage example:\n");
  printf("\t%s [-a /etc/iqfiles] [-I 0] [-t NV12]\n", name);
  printf("\t-a: enable aiq with dirpath provided, eg:-a "
         "/oem/etc/iqfiles/, Default /oem/etc/iqfiles,"
         "without this option aiq should run in other application\n");
  printf("\t-I: camera id, Default 0\n");
  printf("\t-p: param ini directory path, Default:/data/rkadk\n");
  printf("\t-t: data type, default NV12, options: NV12, RGB565, "
         "RBG888, RGBA8888\n");
}

static void sigterm_handler(int sig) {
  fprintf(stderr, "signal %d\n", sig);
  is_quit = true;
}

static void PhotoDataRecv(RKADK_PHOTO_RECV_DATA_S *pstData) {
  static RKADK_U32 photoId = 0;
  char jpegPath[128];
  FILE *file = NULL;

  if (!pstData || !pstData->pu8DataBuf) {
    RKADK_LOGE("Invalid photo data");
    return;
  }

  memset(jpegPath, 0, 128);
  sprintf(jpegPath, "/tmp/PhotoTest_%d.jpeg", photoId);
  file = fopen(jpegPath, "w");
  if (!file) {
    RKADK_LOGE("Create jpeg file(%s) failed", jpegPath);
    return;
  }

  RKADK_LOGD("save u32CamId[%d] jpeg to %s", pstData->u32CamId, jpegPath);

  fwrite(pstData->pu8DataBuf, 1, pstData->u32DataLen, file);
  fclose(file);

  photoId++;
  if (photoId > 10)
    photoId = 0;
}

int main(int argc, char *argv[]) {
  int c, ret, fps;
  RKADK_U32 u32CamId = 0;
  RKADK_CHAR *pIqfilesPath = IQ_FILE_PATH;
  RKADK_PHOTO_ATTR_S stPhotoAttr;
  RKADK_TAKE_PHOTO_ATTR_S stTakePhotoAttr;
  const char *iniPath = NULL;
  char path[RKADK_PATH_LEN];
  char sensorPath[RKADK_MAX_SENSOR_CNT][RKADK_PATH_LEN];
  RKADK_MW_PTR pHandle = NULL;

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
      u32CamId = atoi(optarg);
      break;
    case 'p':
      iniPath = optarg;
      RKADK_LOGD("iniPath: %s", iniPath);
      break;
    case 't':
      if (strstr(optarg, "RGB565"))
        enDataType = RKADK_THUMB_TYPE_RGB565;
      else if (strstr(optarg, "RGB888"))
        enDataType = RKADK_THUMB_TYPE_RGB888;
      else if (strstr(optarg, "RGBA8888"))
        enDataType = RKADK_THUMB_TYPE_RGBA8888;
      break;
    case 'h':
    default:
      print_usage(argv[0]);
      optind = 0;
      return 0;
    }
  }
  optind = 0;

  RKADK_LOGD("#camera id: %d", u32CamId);

  RKADK_MPI_SYS_Init();

#ifdef RKAIQ
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

  ret = RKADK_PARAM_GetCamParam(u32CamId, RKADK_PARAM_TYPE_FPS, &fps);
  if (ret) {
    RKADK_LOGE("RKADK_PARAM_GetCamParam fps failed");
    return -1;
  }

  rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
  RKADK_BOOL fec_enable = RKADK_FALSE;
  SAMPLE_ISP_Start(u32CamId, hdr_mode, fec_enable, pIqfilesPath, fps);
#endif

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

  ret = RKADK_PHOTO_Init(&stPhotoAttr, &pHandle);
  if (ret) {
    RKADK_LOGE("RKADK_PHOTO_Init failed(%d)", ret);
#ifdef RKAIQ
    SAMPLE_ISP_Stop(u32CamId);
#endif
    return -1;
  }

  signal(SIGINT, sigterm_handler);

  char cmd[64];
  printf("\n#Usage: input 'quit' to exit programe!\n"
         "peress any other key to capture one picture to file\n");

  RKADK_PARAM_RES_E type;
  while (!is_quit) {
    fgets(cmd, sizeof(cmd), stdin);
    if (strstr(cmd, "quit") || is_quit) {
      RKADK_LOGD("#Get 'quit' cmd!");
      break;
    } else if (strstr(cmd, "1080")) {
      type = RKADK_RES_1080P;
      RKADK_PARAM_SetCamParam(0, RKADK_PARAM_TYPE_PHOTO_RES, &type);
      RKADK_PHOTO_Reset(&pHandle);
    } else if (strstr(cmd, "720")) {
      type = RKADK_RES_720P;
      RKADK_PARAM_SetCamParam(0, RKADK_PARAM_TYPE_PHOTO_RES, &type);
      RKADK_PHOTO_Reset(&pHandle);
    } else if (strstr(cmd, "1620")) {
      type = RKADK_RES_1620P;
      RKADK_PARAM_SetCamParam(0, RKADK_PARAM_TYPE_PHOTO_RES, &type);
      RKADK_PHOTO_Reset(&pHandle);
    }

    if (RKADK_PHOTO_TakePhoto(pHandle, &stTakePhotoAttr)) {
      RKADK_LOGE("RKADK_PHOTO_TakePhoto failed");
      break;
    }

    usleep(500000);
  }

  RKADK_PHOTO_DeInit(pHandle);

#ifdef RKAIQ
  SAMPLE_ISP_Stop(u32CamId);
#endif
  RKADK_MPI_SYS_Exit();
  return 0;
}
