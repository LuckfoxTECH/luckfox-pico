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

#include "rkadk_param.h"
#include "rkadk_param_map.h"
#include "rkadk_struct2ini.h"
#include <signal.h>
#include <stdbool.h>

static bool is_quit = false;

static void sigterm_handler(int sig) {
  fprintf(stderr, "signal %d\n", sig);
  is_quit = true;
}

void dump_map(RKADK_SI_CONFIG_MAP_S *pmap, int cnt) {
  for (int i = 0; i < cnt; i++) {
    printf("\n the map id = %d", i);
    printf("\n struct_name = %s", pmap->structName);
    printf("\n struct_member = %s", pmap->structMember);
    printf("\n key_vlaue_type = %d", pmap->keyVlaueType);
    printf("\n string_length = %d", pmap->stringLength);
    printf("\n offset = %d", pmap->offset);
    pmap++;
  }
}

static void SetVersion() {
  RKADK_PARAM_VERSION_S stParamVersion;

  dump_map(g_stVersionMapTable,
           sizeof(g_stVersionMapTable) / sizeof(RKADK_SI_CONFIG_MAP_S));

  memset(&stParamVersion, 0, sizeof(RKADK_PARAM_VERSION_S));

  strcpy(stParamVersion.version, RKADK_PARAM_VERSION);

  RKADK_Struct2Ini(RKADK_PARAM_PATH, &stParamVersion, g_stVersionMapTable,
                   sizeof(g_stVersionMapTable) / sizeof(RKADK_SI_CONFIG_MAP_S));
}

static void SetCommCfg() {
  RKADK_PARAM_COMM_CFG_S stParamCommCfg;

  memset(&stParamCommCfg, 0, sizeof(RKADK_PARAM_COMM_CFG_S));
  stParamCommCfg.sensor_count = 1;
  stParamCommCfg.rec_mute = false;
  stParamCommCfg.speaker_volume = 100;
  stParamCommCfg.mic_volume = 100;
  stParamCommCfg.vpss_devcie = 1;

  RKADK_Struct2Ini(RKADK_PARAM_PATH, &stParamCommCfg, g_stCommCfgMapTable,
                   sizeof(g_stCommCfgMapTable) / sizeof(RKADK_SI_CONFIG_MAP_S));
}

static void SetSensorCfg() {
  RKADK_PARAM_SENSOR_CFG_S stSensorCfg;
  char sensorPath[RKADK_PATH_LEN];

  memset(&stSensorCfg, 0, sizeof(RKADK_PARAM_SENSOR_CFG_S));
  stSensorCfg.max_width = SENSOR_MAX_WIDTH;
  stSensorCfg.max_height = SENSOR_MAX_HEIGHT;
  stSensorCfg.used_isp = true;
  stSensorCfg.framerate = VIDEO_FRAME_RATE;
  stSensorCfg.flip = false;
  stSensorCfg.mirror = false;
  stSensorCfg.enable_wrap = false;
  stSensorCfg.wrap_buf_line = SENSOR_MAX_HEIGHT / 4;

  memset(sensorPath, 0, RKADK_PATH_LEN);
  sprintf(sensorPath, "%s_%d.ini", RKADK_PARAM_PATH_SENSOR_PREFIX, 0);
  RKADK_Struct2Ini(sensorPath, &stSensorCfg, g_stSensorCfgMapTable,
                   sizeof(g_stSensorCfgMapTable) /
                       sizeof(RKADK_SI_CONFIG_MAP_S));
}

static void SetAudioCfg() {
  RKADK_PARAM_AUDIO_CFG_S stAudioCfg;

  memset(&stAudioCfg, 0, sizeof(RKADK_PARAM_AUDIO_CFG_S));
  strcpy(stAudioCfg.ai_audio_node, AUDIO_DEVICE_NAME);
  stAudioCfg.ai_depth = AI_DEPTH;
  stAudioCfg.bit_width = AUDIO_BIT_WIDTH;
  stAudioCfg.channels = AUDIO_CHANNEL;
  stAudioCfg.mic_type = AUDIO_MIC_TYPE;
  stAudioCfg.samplerate = AUDIO_SAMPLE_RATE;
  stAudioCfg.samples_per_frame = AUDIO_FRAME_COUNT;
  stAudioCfg.bitrate = AUDIO_BIT_REAT;
  stAudioCfg.vqe_mode = RKADK_VQE_MODE_AI_RECORD;
  stAudioCfg.codec_type = RKADK_CODEC_TYPE_MP3;

  RKADK_Struct2Ini(RKADK_PARAM_PATH, &stAudioCfg, g_stAudioCfgMapTable,
                   sizeof(g_stAudioCfgMapTable) /
                       sizeof(RKADK_SI_CONFIG_MAP_S));
}

static void SetRecCfg() {
  char sensorPath[RKADK_PATH_LEN];
  RKADK_PARAM_REC_CFG_S stParamRecCfg;

  memset(&stParamRecCfg, 0, sizeof(RKADK_PARAM_REC_CFG_S));
  stParamRecCfg.record_type = RKADK_REC_TYPE_NORMAL;
  stParamRecCfg.file_type = RKADK_MUXER_TYPE_MP4;
  stParamRecCfg.pre_record_time = 0;
  stParamRecCfg.pre_record_mode = RKADK_MUXER_PRE_RECORD_NONE;
  stParamRecCfg.lapse_multiple = 30;
  stParamRecCfg.file_num = 2;
  stParamRecCfg.enable_audio = true;

  stParamRecCfg.record_time_cfg[0].record_time = 60;
  stParamRecCfg.record_time_cfg[0].splite_time = 60;
  stParamRecCfg.record_time_cfg[0].lapse_interval = 60;
  stParamRecCfg.attribute[0].width = RECORD_VIDEO_WIDTH;
  stParamRecCfg.attribute[0].height = RECORD_VIDEO_HEIGHT;
  stParamRecCfg.attribute[0].bitrate = 30 * 1024 * 1024;
  stParamRecCfg.attribute[0].framerate = VIDEO_FRAME_RATE;
  stParamRecCfg.attribute[0].gop = VIDEO_GOP;
  stParamRecCfg.attribute[0].profile = VIDEO_PROFILE;
  stParamRecCfg.attribute[0].codec_type = RKADK_CODEC_TYPE_H264;
  stParamRecCfg.attribute[0].venc_chn = 0;
  stParamRecCfg.attribute[0].vpss_grp = 0;
  stParamRecCfg.attribute[0].vpss_chn = 0;
  strcpy(stParamRecCfg.attribute[0].rc_mode, "CBR");
  stParamRecCfg.attribute[0].venc_param.max_qp = 48;
  stParamRecCfg.attribute[0].venc_param.min_qp = 8;
  stParamRecCfg.attribute[0].venc_param.full_range = true;
  stParamRecCfg.attribute[0].venc_param.scaling_list = true;
  stParamRecCfg.attribute[0].venc_param.hier_qp_en = true;
  strcpy(stParamRecCfg.attribute[0].venc_param.hier_qp_delta, "-3,0,0,0");
  strcpy(stParamRecCfg.attribute[0].venc_param.hier_frame_num, "3,0,0,0");

  stParamRecCfg.record_time_cfg[1].record_time = 60;
  stParamRecCfg.record_time_cfg[1].splite_time = 60;
  stParamRecCfg.record_time_cfg[1].lapse_interval = 60;
  stParamRecCfg.attribute[1].width = RECORD_VIDEO_WIDTH_S;
  stParamRecCfg.attribute[1].height = RECORD_VIDEO_HEIGHT_S;
  stParamRecCfg.attribute[1].bufsize = RECORD_VIDEO_WIDTH_S * RECORD_VIDEO_HEIGHT_S * 3 / 2;
  stParamRecCfg.attribute[1].bitrate = 4 * 1024 * 1024;
  stParamRecCfg.attribute[1].framerate = VIDEO_FRAME_RATE;
  stParamRecCfg.attribute[1].gop = VIDEO_GOP;
  stParamRecCfg.attribute[1].profile = VIDEO_PROFILE;
  stParamRecCfg.attribute[1].codec_type = RKADK_CODEC_TYPE_H264;
  stParamRecCfg.attribute[1].venc_chn = 1;
  stParamRecCfg.attribute[1].vpss_grp = 2;
  stParamRecCfg.attribute[1].vpss_chn = 0;
  strcpy(stParamRecCfg.attribute[1].rc_mode, "VBR");
  stParamRecCfg.attribute[1].venc_param.max_qp = 48;
  stParamRecCfg.attribute[1].venc_param.min_qp = 8;
  stParamRecCfg.attribute[1].venc_param.full_range = true;
  stParamRecCfg.attribute[1].venc_param.scaling_list = true;
  stParamRecCfg.attribute[1].venc_param.hier_qp_en = true;
  strcpy(stParamRecCfg.attribute[1].venc_param.hier_qp_delta, "-3,0,0,0");
  strcpy(stParamRecCfg.attribute[1].venc_param.hier_frame_num, "3,0,0,0");

  memset(sensorPath, 0, RKADK_PATH_LEN);
  sprintf(sensorPath, "%s_%d.ini", RKADK_PARAM_PATH_SENSOR_PREFIX, 0);

  RKADK_Struct2Ini(sensorPath, &stParamRecCfg, g_stRecCfgMapTable,
                   sizeof(g_stRecCfgMapTable) / sizeof(RKADK_SI_CONFIG_MAP_S));
  RKADK_Struct2Ini(
      sensorPath, &stParamRecCfg.attribute[0], g_stRecCfgMapTable_0,
      sizeof(g_stRecCfgMapTable_0) / sizeof(RKADK_SI_CONFIG_MAP_S));
  RKADK_Struct2Ini(
      sensorPath, &stParamRecCfg.attribute[1], g_stRecCfgMapTable_1,
      sizeof(g_stRecCfgMapTable_1) / sizeof(RKADK_SI_CONFIG_MAP_S));
  RKADK_Struct2Ini(
      sensorPath, &stParamRecCfg.record_time_cfg[0], g_stRecTimeCfgMapTable_0,
      sizeof(g_stRecTimeCfgMapTable_0) / sizeof(RKADK_SI_CONFIG_MAP_S));
  RKADK_Struct2Ini(
      sensorPath, &stParamRecCfg.record_time_cfg[1], g_stRecTimeCfgMapTable_1,
      sizeof(g_stRecTimeCfgMapTable_1) / sizeof(RKADK_SI_CONFIG_MAP_S));
  RKADK_Struct2Ini(sensorPath, &stParamRecCfg.attribute[0].venc_param,
                   g_stRecParamMapTable_0, sizeof(g_stRecParamMapTable_0) /
                                               sizeof(RKADK_SI_CONFIG_MAP_S));
  RKADK_Struct2Ini(sensorPath, &stParamRecCfg.attribute[1].venc_param,
                   g_stRecParamMapTable_1, sizeof(g_stRecParamMapTable_1) /
                                               sizeof(RKADK_SI_CONFIG_MAP_S));
}

static void SetStreamCfg() {
  char sensorPath[RKADK_PATH_LEN];
  RKADK_PARAM_STREAM_CFG_S stStreamCfg;

  memset(&stStreamCfg, 0, sizeof(RKADK_PARAM_STREAM_CFG_S));
  stStreamCfg.attribute.width = STREAM_VIDEO_WIDTH;
  stStreamCfg.attribute.height = STREAM_VIDEO_HEIGHT;
  stStreamCfg.attribute.bufsize = STREAM_VIDEO_WIDTH * STREAM_VIDEO_HEIGHT * 3 / 2;
  stStreamCfg.attribute.bitrate = 10 * 1024;
  stStreamCfg.attribute.framerate = VIDEO_FRAME_RATE;
  stStreamCfg.attribute.gop = VIDEO_GOP;
  stStreamCfg.attribute.profile = VIDEO_PROFILE;
  stStreamCfg.attribute.codec_type = RKADK_CODEC_TYPE_H264;
  stStreamCfg.attribute.venc_chn = 1;
  stStreamCfg.attribute.vpss_grp = 2;
  stStreamCfg.attribute.vpss_chn = 0;
  strcpy(stStreamCfg.attribute.rc_mode, "VBR");
  stStreamCfg.attribute.venc_param.max_qp = 48;
  stStreamCfg.attribute.venc_param.min_qp = 8;
  stStreamCfg.attribute.venc_param.full_range = true;
  stStreamCfg.attribute.venc_param.scaling_list = true;
  stStreamCfg.attribute.venc_param.hier_qp_en = true;
  strcpy(stStreamCfg.attribute.venc_param.hier_qp_delta, "-3,0,0,0");
  strcpy(stStreamCfg.attribute.venc_param.hier_frame_num, "3,0,0,0");

  memset(sensorPath, 0, RKADK_PATH_LEN);
  sprintf(sensorPath, "%s_%d.ini", RKADK_PARAM_PATH_SENSOR_PREFIX, 0);

  RKADK_Struct2Ini(sensorPath, &stStreamCfg, g_stPreviewCfgMapTable,
                   sizeof(g_stPreviewCfgMapTable) /
                      sizeof(RKADK_SI_CONFIG_MAP_S));

  RKADK_Struct2Ini(sensorPath, &stStreamCfg.attribute.venc_param,
                   g_stPreviewParamMapTable, sizeof(g_stPreviewParamMapTable) /
                                                 sizeof(RKADK_SI_CONFIG_MAP_S));
}

static void SetLiveCfg() {
  char sensorPath[RKADK_PATH_LEN];
  RKADK_PARAM_STREAM_CFG_S stLiveCfg;

  memset(&stLiveCfg, 0, sizeof(RKADK_PARAM_STREAM_CFG_S));
  stLiveCfg.attribute.width = STREAM_VIDEO_WIDTH;
  stLiveCfg.attribute.height = STREAM_VIDEO_HEIGHT;
  stLiveCfg.attribute.bitrate = 4 * 1024 * 1024;
  stLiveCfg.attribute.framerate = VIDEO_FRAME_RATE;
  stLiveCfg.attribute.gop = VIDEO_GOP;
  stLiveCfg.attribute.profile = VIDEO_PROFILE;
  stLiveCfg.attribute.codec_type = RKADK_CODEC_TYPE_H264;
  stLiveCfg.attribute.venc_chn = 1;
  stLiveCfg.attribute.vpss_grp = 2;
  stLiveCfg.attribute.vpss_chn = 0;
  strcpy(stLiveCfg.attribute.rc_mode, "VBR");
  stLiveCfg.attribute.venc_param.max_qp = 48;
  stLiveCfg.attribute.venc_param.min_qp = 8;
  stLiveCfg.attribute.venc_param.full_range = true;
  stLiveCfg.attribute.venc_param.scaling_list = true;
  stLiveCfg.attribute.venc_param.hier_qp_en = true;
  strcpy(stLiveCfg.attribute.venc_param.hier_qp_delta, "-3,0,0,0");
  strcpy(stLiveCfg.attribute.venc_param.hier_frame_num, "3,0,0,0");

  memset(sensorPath, 0, RKADK_PATH_LEN);
  sprintf(sensorPath, "%s_%d.ini", RKADK_PARAM_PATH_SENSOR_PREFIX, 0);

  RKADK_Struct2Ini(sensorPath, &stLiveCfg, g_stLiveCfgMapTable,
                   sizeof(g_stLiveCfgMapTable) / sizeof(RKADK_SI_CONFIG_MAP_S));

  RKADK_Struct2Ini(sensorPath, &stLiveCfg.attribute.venc_param,
                   g_stLiveParamMapTable, sizeof(g_stLiveParamMapTable) /
                                              sizeof(RKADK_SI_CONFIG_MAP_S));
}

static void SetPhotoCfg() {
  char sensorPath[RKADK_PATH_LEN];
  RKADK_PARAM_PHOTO_CFG_S stPhotoCfg;

  memset(&stPhotoCfg, 0, sizeof(RKADK_PARAM_PHOTO_CFG_S));
  stPhotoCfg.image_width = PHOTO_VIDEO_WIDTH;
  stPhotoCfg.image_height = PHOTO_VIDEO_HEIGHT;
  stPhotoCfg.venc_chn = 2;
  stPhotoCfg.vpss_grp = 0;
  stPhotoCfg.vpss_chn = 0;
  stPhotoCfg.enable_combo = false;
  stPhotoCfg.combo_venc_chn = 0;
  stPhotoCfg.qfactor = 70;

  memset(sensorPath, 0, RKADK_PATH_LEN);
  sprintf(sensorPath, "%s_%d.ini", RKADK_PARAM_PATH_SENSOR_PREFIX, 0);

  RKADK_Struct2Ini(sensorPath, &stPhotoCfg, g_stPhotoCfgMapTable,
                   sizeof(g_stPhotoCfgMapTable) /
                       sizeof(RKADK_SI_CONFIG_MAP_S));
}

static void SetViCfg() {
  char sensorPath[RKADK_PATH_LEN];
  RKADK_PARAM_VI_CFG_S stViCfg;

  memset(sensorPath, 0, RKADK_PATH_LEN);
  sprintf(sensorPath, "%s_%d.ini", RKADK_PARAM_PATH_SENSOR_PREFIX, 0);

  memset(&stViCfg, 0, sizeof(RKADK_PARAM_VI_CFG_S));
  stViCfg.chn_id = 0;
  strcpy(stViCfg.device_name, DEF_DEVICE_NEME_VI_0);
  stViCfg.width = RECORD_VIDEO_WIDTH;
  stViCfg.height = RECORD_VIDEO_HEIGHT;
  stViCfg.buf_cnt = VIDEO_BUFFER_COUNT;
  strcpy(stViCfg.pix_fmt, "NV12");
  strcpy(stViCfg.module, "RECORD_MAIN|PHOTO");
  RKADK_Struct2Ini(sensorPath, &stViCfg, g_stViCfgMapTable_0,
                   sizeof(g_stViCfgMapTable_0) / sizeof(RKADK_SI_CONFIG_MAP_S));

  memset(&stViCfg, 0, sizeof(RKADK_PARAM_VI_CFG_S));
  stViCfg.chn_id = 1;
  strcpy(stViCfg.device_name, DEF_DEVICE_NEME_VI_1);
  stViCfg.width = THUMB_WIDTH;
  stViCfg.height = THUMB_HEIGHT;
  stViCfg.buf_cnt = VIDEO_BUFFER_COUNT;
  strcpy(stViCfg.pix_fmt, "NV12");
  strcpy(stViCfg.module, "THUMB");
  RKADK_Struct2Ini(sensorPath, &stViCfg, g_stViCfgMapTable_1,
                   sizeof(g_stViCfgMapTable_1) / sizeof(RKADK_SI_CONFIG_MAP_S));

  memset(&stViCfg, 0, sizeof(RKADK_PARAM_VI_CFG_S));
  stViCfg.chn_id = 2;
  strcpy(stViCfg.device_name, DEF_DEVICE_NEME_VI_2);
  stViCfg.buf_cnt = VIDEO_BUFFER_COUNT;
  stViCfg.width = STREAM_VIDEO_WIDTH;
  stViCfg.height = STREAM_VIDEO_HEIGHT;
  strcpy(stViCfg.pix_fmt, "NV12");
  strcpy(stViCfg.module, "RECORD_SUB|PREVIEW|LIVE|DISP");
  RKADK_Struct2Ini(sensorPath, &stViCfg, g_stViCfgMapTable_2,
                   sizeof(g_stViCfgMapTable_2) / sizeof(RKADK_SI_CONFIG_MAP_S));

  memset(&stViCfg, 0, sizeof(RKADK_PARAM_VI_CFG_S));
  stViCfg.chn_id = 3;
  strcpy(stViCfg.device_name, DEF_DEVICE_NEME_VI_3);
  stViCfg.width = STREAM_VIDEO_WIDTH;
  stViCfg.height = STREAM_VIDEO_HEIGHT;
  stViCfg.buf_cnt = VIDEO_BUFFER_COUNT;
  strcpy(stViCfg.pix_fmt, "NV12");
  strcpy(stViCfg.module, "NONE");
  RKADK_Struct2Ini(sensorPath, &stViCfg, g_stViCfgMapTable_3,
                   sizeof(g_stViCfgMapTable_3) / sizeof(RKADK_SI_CONFIG_MAP_S));
}

static void SetDispCfg() {
  char sensorPath[RKADK_PATH_LEN];
  RKADK_PARAM_DISP_CFG_S stDispCfg;

  memset(sensorPath, 0, RKADK_PATH_LEN);
  sprintf(sensorPath, "%s_%d.ini", RKADK_PARAM_PATH_SENSOR_PREFIX, 0);

  memset(&stDispCfg, 0, sizeof(RKADK_PARAM_DISP_CFG_S));
  stDispCfg.width = DISP_WIDTH;
  stDispCfg.height = DISP_HEIGHT;
  stDispCfg.rotation = 1;
  stDispCfg.vpss_grp = 3;
  stDispCfg.vpss_chn = 0;
  strcpy(stDispCfg.img_type, "RGB888");
  strcpy(stDispCfg.splice_mode, "RGA");
#ifdef RV1106_1103
  strcpy(stDispCfg.intf_type, "default");
#else
  strcpy(stDispCfg.intf_type, "MIPI");
#endif
  stDispCfg.vo_device = 0;
  stDispCfg.vo_layer = 0;
  stDispCfg.vo_chn = 0;
  RKADK_Struct2Ini(sensorPath, &stDispCfg, g_stDispCfgMapTable,
                   sizeof(g_stDispCfgMapTable) / sizeof(RKADK_SI_CONFIG_MAP_S));
}

static void SetThumbCfg() {
  char sensorPath[RKADK_PATH_LEN];
  RKADK_PARAM_THUMB_CFG_S stThumbCfg;

  memset(sensorPath, 0, RKADK_PATH_LEN);
  sprintf(sensorPath, "%s_%d.ini", RKADK_PARAM_PATH_SENSOR_PREFIX, 0);

  memset(&stThumbCfg, 0, sizeof(RKADK_PARAM_THUMB_CFG_S));
  stThumbCfg.thumb_width = THUMB_WIDTH;
  stThumbCfg.thumb_height = THUMB_HEIGHT;
  stThumbCfg.photo_venc_chn = THUMB_PHOTO_VENC_CHN;
  stThumbCfg.record_main_venc_chn = THUMB_RECORD_MAIN_VENC_CHN;
  stThumbCfg.record_sub_venc_chn = THUMB_RECORD_SUB_VENC_CHN;
  stThumbCfg.vpss_grp = THUMB_VPSS_GRP;
  stThumbCfg.vpss_chn = THUMB_VPSS_CHN;
  stThumbCfg.qfactor = 50;

  RKADK_Struct2Ini(sensorPath, &stThumbCfg, g_stThumbCfgMapTable,
                   sizeof(g_stThumbCfgMapTable) / sizeof(RKADK_SI_CONFIG_MAP_S));
}

int main(int argc, char *argv[]) {

  RKADK_LOGD("this is demo for struct2ini");

  signal(SIGINT, sigterm_handler);

  char cmd[64];
  printf("\n#Usage: input 'quit' to exit programe!\n"
         "input 'set' key to write config param to ini file\n"
         "input 'get' key to read config param from ini file\n");
  while (!is_quit) {
    fgets(cmd, sizeof(cmd), stdin);
    if (strstr(cmd, "quit") || is_quit) {
      RKADK_LOGD("#Get 'quit' cmd!");
      break;
    }

    if (strstr(cmd, "set")) {
      RKADK_LOGD("#Set version");
      SetVersion();
      RKADK_LOGD("SetVersion done");

      RKADK_LOGD("#Set config param");
      SetCommCfg();
      RKADK_LOGD("SetCommCfg done");

      SetAudioCfg();
      RKADK_LOGD("SetAudioCfg done");

      SetSensorCfg();
      RKADK_LOGD("SetSensorCfg done");

      SetViCfg();
      RKADK_LOGD("SetViCfg done");

      SetRecCfg();
      RKADK_LOGD("SetRecCfg done");

      SetPhotoCfg();
      RKADK_LOGD("SetPhotoCfg done");

      SetStreamCfg();
      RKADK_LOGD("SetStreamCfg done");

      SetLiveCfg();
      RKADK_LOGD("SetLiveCfg done");

      SetDispCfg();
      RKADK_LOGD("SetDispCfg done");

      SetThumbCfg();
      RKADK_LOGD("SetThumbCfg done");
    }

    if (strstr(cmd, "get")) {
      RKADK_LOGD("#Get config param");
      RKADK_PARAM_Init(NULL, NULL);
    }

    usleep(500000);
  }

  return 0;
}
