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
#include "rkadk_stream.h"
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

static FILE *g_output_file = NULL;
static FILE *g_pcm_file = NULL;
static bool is_quit = false;
static RKADK_CHAR *g_output_path = "/data/ai.pcm";
static RKADK_CHAR optstr[] = "a:I:m:e:o:p:h";

// default use ini audio codec type
static RKADK_CODEC_TYPE_E g_enCodecType = RKADK_CODEC_TYPE_BUTT;

static void print_usage(const RKADK_CHAR *name) {
  printf("usage example:\n");
  printf("\t%s [-a /etc/iqfiles] [-I 0] [-m audio] [-e g711a] [-o "
         "/tmp/aenc.g711a]\n",
         name);
  printf("\t-a: enable aiq with dirpath provided, eg:-a "
         "/oem/etc/iqfiles/, Default /oem/etc/iqfiles,"
         "without this option aiq should run in other application\n");
  printf("\t-I: Camera id, Default:0\n");
  printf("\t-m: Test mode, Value: audio, video, Default:\"audio\"\n");
  printf("\t-e: Encode type, Value:pcm, g711a, g711u, mp2, mp3, "
         "Default:ini audio codec type\n");
  printf("\t-o: Output path, Default:\"/data/ai.pcm\"\n");
  printf("\t-p: param ini directory path, Default:/data/rkadk\n");
  ;
}

static void sigterm_handler(int sig) {
  fprintf(stderr, "signal %d\n", sig);
  is_quit = true;
}

static RKADK_S32 VencDataCb(RKADK_VIDEO_STREAM_S *pVStreamData) {
  if (g_output_file) {
    fwrite(pVStreamData->astPack.apu8Addr, 1, pVStreamData->astPack.au32Len,
           g_output_file);
    RKADK_LOGD("#Write seq: %d, pts: %lld, size: %zu", pVStreamData->u32Seq,
               pVStreamData->astPack.u64PTS, pVStreamData->astPack.au32Len);
  }

  return 0;
}

static int VideoTest(RKADK_U32 u32CamId, RKADK_CHAR *pIqfilesPath) {
  RKADK_S32 ret, fps;
  RKADK_MW_PTR pHandle = NULL;
  RKADK_STREAM_VIDEO_ATTR_S stVideoAttr;
  RKADK_VIDEO_INFO_S stVideoInfo;

  g_output_file = fopen(g_output_path, "w");
  if (!g_output_file) {
    RKADK_LOGE("open %s file failed, exit", g_output_path);
    return -1;
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

  memset(&stVideoAttr, 0, sizeof(RKADK_STREAM_VIDEO_ATTR_S));
  stVideoAttr.pfnDataCB = VencDataCb;
  stVideoAttr.u32CamId = u32CamId;

  RKADK_STREAM_GetVideoInfo(u32CamId, &stVideoInfo);
  RKADK_LOGD("stVideoInfo.enCodecType: %d", stVideoInfo.enCodecType);
  RKADK_LOGD("stVideoInfo.u32BitRate: %d", stVideoInfo.u32BitRate);
  RKADK_LOGD("stVideoInfo.u32FrameRate: %d", stVideoInfo.u32FrameRate);
  RKADK_LOGD("stVideoInfo.u32Gop: %d", stVideoInfo.u32Gop);
  RKADK_LOGD("stVideoInfo.u32Height: %d", stVideoInfo.u32Height);
  RKADK_LOGD("stVideoInfo.u32Width: %d", stVideoInfo.u32Width);

  ret = RKADK_STREAM_VideoInit(&stVideoAttr, &pHandle);
  if (ret) {
    RKADK_LOGE("RKADK_STREAM_VideoInit failed = %d", ret);
#ifdef RKAIQ
    SAMPLE_ISP_Stop(u32CamId);
#endif
    return -1;
  }

  ret = RKADK_STREAM_VencStart(pHandle, -1);
  if (ret) {
    RKADK_LOGE("RKADK_STREAM_VencStart failed");
#ifdef RKAIQ
    SAMPLE_ISP_Stop(u32CamId);
#endif
    return -1;
  }

  RKADK_LOGD("initial finish\n");
  signal(SIGINT, sigterm_handler);

  char cmd[64];
  printf("\n#Usage: input 'quit' to exit programe!\n"
         "peress any other key to manual split file\n");

  while (!is_quit) {
    fgets(cmd, sizeof(cmd), stdin);
    if (strstr(cmd, "quit") || is_quit) {
      RKADK_LOGD("#Get 'quit' cmd!");
      break;
    }

#if 0
    if (strstr(cmd, "fps")) {
      int fpsTest = 20;
      RKADK_PARAM_GOP_S stGopCfg;
      RKADK_S32 s32VencChn;

      // set ini fps
      RKADK_PARAM_SetCamParam(u32CamId, RKADK_PARAM_TYPE_FPS, &fpsTest);

      // set ini gop
      stGopCfg.enStreamType = RKADK_STREAM_TYPE_PREVIEW;
      stGopCfg.u32Gop = fpsTest;
      RKADK_PARAM_SetCamParam(u32CamId, RKADK_PARAM_TYPE_GOP, &stGopCfg);

      // set aiq fps
      SAMPLE_ISP_SET_FrameRate(u32CamId, fpsTest);

      // set mpp fps and gop
      s32VencChn =
          RKADK_PARAM_GetVencChnId(u32CamId, RKADK_STREAM_TYPE_PREVIEW);
      RK_MPI_VENC_SetGop(s32VencChn, stGopCfg.u32Gop);
      RK_MPI_VENC_SetFps(s32VencChn, fpsTest, 1, fpsTest, 1);

      // get ini fps and gop
      RKADK_PARAM_GetCamParam(u32CamId, RKADK_PARAM_TYPE_FPS, &fpsTest);
      RKADK_LOGD("fps: %d", fpsTest);
      RKADK_PARAM_SetCamParam(u32CamId, RKADK_PARAM_TYPE_GOP, &stGopCfg);
      RKADK_LOGD("gop: %d", stGopCfg.u32Gop);
    }
#endif
    usleep(500000);
  }

  RKADK_LOGD("exit!");

  ret = RKADK_STREAM_VencStop(pHandle);
  if (ret)
    RKADK_LOGE("RKADK_STREAM_VencStop failed");

  ret = RKADK_STREAM_VideoDeInit(pHandle);
  if (ret)
    RKADK_LOGE("RKADK_STREAM_VideoDeInit failed = %d", ret);

#ifdef RKAIQ
  SAMPLE_ISP_Stop(u32CamId);
#endif

  if (g_output_file) {
    fclose(g_output_file);
    g_output_file = NULL;
  }

  return 0;
}

static RKADK_S32 AencDataCb(RKADK_AUDIO_STREAM_S *pAStreamData) {
  if (!g_output_file) {
    g_output_file = fopen(g_output_path, "w");
    if (!g_output_file) {
      RKADK_LOGE("open %s file failed, exit", g_output_path);
      return -1;
    }
  }

  fwrite(pAStreamData->pStream, 1, pAStreamData->u32Len, g_output_file);
  RKADK_LOGD("#Write Aenc seq: %d, pts: %lld, size: %zu", pAStreamData->u32Seq,
             pAStreamData->u64TimeStamp, pAStreamData->u32Len);

  return 0;
}

static RKADK_S32 PcmDataCb(RKADK_AUDIO_STREAM_S *pAStreamData) {

  if (!g_pcm_file) {
    g_pcm_file = fopen("/data/ai.pcm", "w");
    if (!g_pcm_file) {
      RKADK_LOGE("open /data/ai.pcm file failed, exit");
      return -1;
    }
  }

  fwrite(pAStreamData->pStream, 1, pAStreamData->u32Len, g_pcm_file);
  RKADK_LOGD("#Write pcm seq: %d, pts: %lld, size: %zu", pAStreamData->u32Seq,
             pAStreamData->u64TimeStamp, pAStreamData->u32Len);

  return 0;
}

static int AudioTest(RKADK_U32 u32CamId) {
  RKADK_S32 ret;
  RKADK_MW_PTR pHandle = NULL;
  RKADK_STREAM_AUDIO_ATTR_S stAudioAttr;
  RKADK_AUDIO_INFO_S stAudioInfo;

  memset(&stAudioAttr, 0, sizeof(RKADK_STREAM_AUDIO_ATTR_S));
  stAudioAttr.u32CamId = u32CamId;
  stAudioAttr.enCodecType = g_enCodecType;
  stAudioAttr.pfnPcmDataCB = PcmDataCb;
  stAudioAttr.pfnAencDataCB = AencDataCb;
  ret = RKADK_STREAM_AudioInit(&stAudioAttr, &pHandle);
  if (ret) {
    RKADK_LOGE("RKADK_STREAM_AudioInit failed = %d", ret);
    return ret;
  }

  RKADK_STREAM_GetAudioInfo(pHandle, &stAudioInfo);
  RKADK_LOGD("stAudioInfo.enCodecType: %d", stAudioInfo.enCodecType);
  RKADK_LOGD("stAudioInfo.u16SampleBitWidth: %d",
             stAudioInfo.u16SampleBitWidth);
  RKADK_LOGD("stAudioInfo.u32AvgBytesPerSec: %d",
             stAudioInfo.u32AvgBytesPerSec);
  RKADK_LOGD("stAudioInfo.u32ChnCnt: %d", stAudioInfo.u32ChnCnt);
  RKADK_LOGD("stAudioInfo.u32SampleRate: %d", stAudioInfo.u32SampleRate);
  RKADK_LOGD("stAudioInfo.u32SamplesPerFrame: %d",
             stAudioInfo.u32SamplesPerFrame);

  ret = RKADK_STREAM_AencStart(pHandle);
  if (ret) {
    RKADK_LOGE("RKADK_STREAM_AencStart failed");
    return -1;
  }

  RKADK_LOGD("initial finish");
  signal(SIGINT, sigterm_handler);

  char cmd[64];
  while (!is_quit) {
    fgets(cmd, sizeof(cmd), stdin);
    if (strstr(cmd, "quit")) {
      RKADK_LOGD("#Get 'quit' cmd!");
      break;
    } else if (strstr(cmd, "start")) {
      RKADK_STREAM_AencStart(pHandle);
    } else if (strstr(cmd, "stop")) {
      RKADK_STREAM_AencStop(pHandle);
    }

    usleep(500000);
  }

  ret = RKADK_STREAM_AencStop(pHandle);
  if (ret) {
    RKADK_LOGE("RKADK_STREAM_AencStop failed");
  }

  ret = RKADK_STREAM_AudioDeInit(pHandle);
  if (ret)
    RKADK_LOGE("RKADK_STREAM_AudioDeInit failed = %d", ret);

  if (g_pcm_file) {
    fclose(g_pcm_file);
    g_pcm_file = NULL;
  }

  if (g_output_file) {
    fclose(g_output_file);
    g_output_file = NULL;
  }

  return 0;
}

int main(int argc, char *argv[]) {
  RKADK_U32 u32CamId = 0;
  RKADK_CHAR *pMode = "audio";
  RKADK_CHAR *pIqfilesPath = IQ_FILE_PATH;
  int c;
  const char *iniPath = NULL;
  char path[RKADK_PATH_LEN];
  char sensorPath[RKADK_MAX_SENSOR_CNT][RKADK_PATH_LEN];

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
    case 'm':
      pMode = optarg;
      break;
    case 'e':
      if (!strcmp(optarg, "pcm")) {
        g_enCodecType = RKADK_CODEC_TYPE_PCM;
        g_output_path = "/data/ai.pcm";
      } else if (!strcmp(optarg, "g711a")) {
        g_enCodecType = RKADK_CODEC_TYPE_G711A;
        g_output_path = "/data/aenc.g711a";
      } else if (!strcmp(optarg, "g711u")) {
        g_enCodecType = RKADK_CODEC_TYPE_G711U;
        g_output_path = "/data/aenc.g711u";
      } else if (!strcmp(optarg, "mp3")) {
        g_enCodecType = RKADK_CODEC_TYPE_MP3;
        g_output_path = "/data/aenc.mp3";
      } else {
        RKADK_LOGE("unknow encode type: %s", optarg);
        return 0;
      }
      RKADK_LOGD("Encode type: %d", g_enCodecType);
      break;
    case 'o':
      g_output_path = optarg;
      RKADK_LOGD("g_output_path = %s", g_output_path);
      break;
    case 'p':
      iniPath = optarg;
      RKADK_LOGD("iniPath: %s", iniPath);
      break;
    case 'h':
    default:
      print_usage(argv[0]);
      optind = 0;
      return 0;
    }
  }
  optind = 0;

  RKADK_LOGD("#Test mode: %s", pMode);
  RKADK_LOGD("#Out path: %s", g_output_path);

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

  if (!strcmp(pMode, "audio"))
    AudioTest(u32CamId);
  else if (!strcmp(pMode, "video"))
    VideoTest(u32CamId, pIqfilesPath);
  else {
    RKADK_LOGE("Invalid test mode: %s", pMode);
    return -1;
  }
  RKADK_MPI_SYS_Exit();
  return 0;
}
