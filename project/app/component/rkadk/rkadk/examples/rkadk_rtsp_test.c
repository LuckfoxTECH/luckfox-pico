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
#include "rkadk_osd.h"
#include "rkadk_param.h"
#include "rkadk_rtsp.h"
#include <getopt.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern int optind;
extern char *optarg;

#define IQ_FILE_PATH "/etc/iqfiles"

static bool is_quit = false;
static RKADK_CHAR optstr[] = "a:I:p:o";

static void print_usage(const RKADK_CHAR *name) {
  printf("usage example:\n");
  printf("\t%s [-a /etc/iqfiles] [-I 0]\n", name);
  printf("\t-a: enable aiq with dirpath provided, eg:-a "
         "/oem/etc/iqfiles/, Default /oem/etc/iqfiles,"
         "without this option aiq should run in other application\n");
  printf("\t-I: Camera id, Default:0\n");
  printf("\t-p: param ini directory path, Default:/data/rkadk\n");
  printf("\t-p: osd file , Default:/userdata/rkadk_ARGB8888\n");
}

static void sigterm_handler(int sig) {
  fprintf(stderr, "signal %d\n", sig);
  is_quit = true;
}

int main(int argc, char *argv[]) {
  int c, ret, fps;
  RKADK_OSD_ATTR_S OsdAttr;
  RKADK_OSD_STREAM_ATTR_S OsdStreamAttr;
  RKADK_U32 u32CamId = 0;
  RKADK_U32 u32OsdId = 0;
  RKADK_CHAR *pIqfilesPath = IQ_FILE_PATH;
  RKADK_MW_PTR pHandle = NULL;
  const char *iniPath = NULL;
  char path[RKADK_PATH_LEN];
  char sensorPath[RKADK_MAX_SENSOR_CNT][RKADK_PATH_LEN];
  char *osdfile = "/userdata/rkadk_ARGB8888";

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
    case 'o':
      osdfile = optarg;
      RKADK_LOGD("osdfile: %s", osdfile);
      break;
    default:
      print_usage(argv[0]);
      optind = 0;
      return 0;
    }
  }
  optind = 0;

  RKADK_LOGD("#camera id: %d", u32CamId);

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
  ret = RKADK_PARAM_GetCamParam(u32CamId, RKADK_PARAM_TYPE_FPS, &fps);
  if (ret) {
    RKADK_LOGE("RKADK_PARAM_GetCamParam fps failed");
    return -1;
  }

  rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
  RKADK_BOOL fec_enable = RKADK_FALSE;
  SAMPLE_ISP_Start(u32CamId, hdr_mode, fec_enable, pIqfilesPath, fps);
#endif

  ret = RKADK_RTSP_Init(u32CamId, 554, "/live/main_stream", &pHandle);
  if (ret) {
    RKADK_LOGE("RKADK_RTSP_Init failed(%d)", ret);
#ifdef RKAIQ
    SAMPLE_ISP_Stop(u32CamId);
#endif
    return -1;
  }

  RKADK_RTSP_Start(pHandle);

  memset(&OsdAttr, 0, sizeof(RKADK_OSD_ATTR_S));
  memset(&OsdStreamAttr, 0, sizeof(RKADK_OSD_STREAM_ATTR_S));
  OsdAttr.Format = RKADK_FMT_ARGB8888;
  OsdAttr.Width = 320;
  OsdAttr.Height = 128;
  OsdAttr.pData = malloc(OsdAttr.Height * OsdAttr.Width * 4);

  OsdStreamAttr.Origin_X = 0;
  OsdStreamAttr.Origin_Y = 0;

  RKADK_OSD_Init(u32OsdId, &OsdAttr);
  RKADK_OSD_AttachToStream(u32OsdId, u32CamId, RKADK_STREAM_TYPE_LIVE,
                           &OsdStreamAttr);

  FILE *fp = fopen(osdfile, "rw");
  if (!fp) {
    RKADK_LOGD("open osd file fail");
  } else {
    RKADK_LOGD("open osd file success");
    fread((RKADK_U8 *)OsdAttr.pData, OsdAttr.Width * OsdAttr.Height * 4, 1, fp);
    fclose(fp);
    RKADK_OSD_UpdateBitMap(u32OsdId, &OsdAttr);
  }

  signal(SIGINT, sigterm_handler);

  char cmd[64];
  printf("\n#Usage: input 'quit' to exit programe!\n"
         "peress any other key to capture one picture to file\n");
  while (!is_quit) {
    fgets(cmd, sizeof(cmd), stdin);
    if (strstr(cmd, "quit") || is_quit) {
      RKADK_LOGD("#Get 'quit' cmd!");
      break;
    } else if (strstr(cmd, "start")) {
      RKADK_RTSP_Start(pHandle);
    } else if (strstr(cmd, "stop")) {
      RKADK_RTSP_Stop(pHandle);
    }

    usleep(500000);
  }

  RKADK_OSD_DettachFromStream(u32OsdId, u32CamId, RKADK_STREAM_TYPE_LIVE);
  RKADK_OSD_Deinit(u32OsdId);
  free(OsdAttr.pData);
  RKADK_RTSP_Stop(pHandle);
  RKADK_RTSP_DeInit(pHandle);
  pHandle = NULL;

#ifdef RKAIQ
  SAMPLE_ISP_Stop(u32CamId);
#endif
  RKADK_MPI_SYS_Exit();
  return 0;
}
