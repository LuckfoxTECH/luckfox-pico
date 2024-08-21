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

#ifndef __RKADK_PARAM_H__
#define __RKADK_PARAM_H__

#include "rkadk_common.h"
#include "rkadk_media_comm.h"
#include "rkadk_record.h"
#include "rkadk_platform_param.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* version */
#define RKADK_PARAM_VERSION "1.4"

#define RKADK_VOLUME_LEN 3
#define RKADK_RC_MODE_LEN 5

/* audio default parameters */
/* g711u must be 16K, g711a can be either 8K or 16K */
#define AUDIO_SAMPLE_RATE 16000
#define AUDIO_DEVICE_CHANNEL 2
#define AUDIO_CHANNEL 2
#define AUDIO_MIC_TYPE 0
#define AUDIO_BIT_REAT 160000
#define AUDIO_FRAME_COUNT 1152
#define AUDIO_BIT_WIDTH AUDIO_BIT_WIDTH_16
#define AI_DEPTH 1
#ifdef RV1106_1103
#define AUDIO_DEVICE_NAME "hw:0,0"
#elif RK3506
#define AUDIO_DEVICE_NAME "hw:0,0"
#else
#define AUDIO_DEVICE_NAME "default"
#endif
#define AI_VQE_CONFIG_PATH "/oem/usr/share/vqefiles/config_aivqe.json"

/* video default parameters */
#define VIDEO_GOP 25
#define VIDEO_FRAME_RATE 25
#define VIDEO_PROFILE 100
#define VIDEO_BUFFER_COUNT 2
#define STREAM_VIDEO_WIDTH 1280
#define STREAM_VIDEO_HEIGHT 720
#define RECORD_VIDEO_WIDTH SENSOR_MAX_WIDTH
#define RECORD_VIDEO_HEIGHT SENSOR_MAX_HEIGHT
#define RECORD_VIDEO_WIDTH_S STREAM_VIDEO_WIDTH
#define RECORD_VIDEO_HEIGHT_S STREAM_VIDEO_HEIGHT
#define PHOTO_VIDEO_WIDTH SENSOR_MAX_WIDTH
#define PHOTO_VIDEO_HEIGHT SENSOR_MAX_HEIGHT
#define DISP_WIDTH 720
#define DISP_HEIGHT 1280

/* thumb default parameters */
#define THUMB_WIDTH 320
#define THUMB_HEIGHT 180
#define THUMB_PHOTO_VENC_CHN 5
#define THUMB_RECORD_MAIN_VENC_CHN 6
#define THUMB_RECORD_SUB_VENC_CHN 7
#define THUMB_VPSS_GRP 1
#define THUMB_VPSS_CHN 0

#define RECORD_AI_CHN 0
#define RECORD_AENC_CHN 0

#define PREVIEW_AI_CHN RECORD_AI_CHN
#define PREVIEW_AENC_CHN RECORD_AENC_CHN

#define LIVE_AI_CHN RECORD_AI_CHN
#define LIVE_AENC_CHN RECORD_AENC_CHN

/* vo default parameters */
#define VO_DEVICE 0
#define VOP_LAYER 0

/* setting file path */
#define RKADK_DEFPARAM_PATH "/oem/usr/etc/rkadk_defsetting.ini"
#define RKADK_DEFPARAM_PATH_SENSOR_PREFIX "/oem/usr/etc/rkadk_defsetting_sensor"
#define RKADK_PARAM_PATH "/data/rkadk/rkadk_setting.ini"
#define RKADK_PARAM_PATH_SENSOR_PREFIX "/data/rkadk/rkadk_setting_sensor"

/* Resolution */
#define RKADK_WIDTH_480P 848
#define RKADK_HEIGHT_480P 480

#define RKADK_WIDTH_720P 1280
#define RKADK_HEIGHT_720P 720

#define RKADK_WIDTH_1080P 1920
#define RKADK_HEIGHT_1080P 1080

#define RKADK_WIDTH_1296P 2304
#define RKADK_HEIGHT_1296P 1296

#define RKADK_WIDTH_1440P 2560
#define RKADK_HEIGHT_1440P 1440

#define RKADK_WIDTH_1520P 2688
#define RKADK_HEIGHT_1520P 1520

#define RKADK_WIDTH_1600P 2560
#define RKADK_HEIGHT_1600P 1600

#define RKADK_WIDTH_1620P 2880
#define RKADK_HEIGHT_1620P 1616

#define RKADK_WIDTH_1944P 2592
#define RKADK_HEIGHT_1944P 1944

#define RKADK_WIDTH_2160P 3840
#define RKADK_HEIGHT_2160P 2160

#define RKADK_WIDTH_3456P 4608
#define RKADK_HEIGHT_3456P 3456

#define RKADK_WIDTH_4000P 6000
#define RKADK_HEIGHT_4000P 4000

#define RKADK_WIDTH_4480P 6720
#define RKADK_HEIGHT_4480P 4480

#define RKADK_WIDTH_4275P 7600
#define RKADK_HEIGHT_4275P 4275

#define RKADK_WIDTH_4320P 7680
#define RKADK_HEIGHT_4320P 4320

#define RKADK_WIDTH_3672P 6528
#define RKADK_HEIGHT_3672P 3672

/* Resolution type */
typedef enum {
  RKADK_RES_720P = 0, /* 1280*720 */
  RKADK_RES_1080P,    /* 1920*1080 */
  RKADK_RES_1296P,    /* 2304*1296 */
  RKADK_RES_1440P,    /* 2560*1440 */
  RKADK_RES_1520P,    /* 2688*1520 */
  RKADK_RES_1600P,    /* 2560*1600 */
  RKADK_RES_1620P,    /* 2880*1616, height 8 alignment */
  RKADK_RES_1944P,    /* 2592*1944 */
  RKADK_RES_2160P,    /* 3840*2160 */
  RKADK_RES_3456P,    /* 4608*3456 */
  RKADK_RES_4000P,    /* 6000*4000 */
  RKADK_RES_4480P,    /* 6720*4480 */
  RKADK_RES_4275P,    /* 7600*4275 */
  RKADK_RES_4320P,    /* 7680*4320 */
  RKADK_RES_3672P,    /* 6528*3672 */
  RKADK_RES_BUTT,
} RKADK_PARAM_RES_E;

/* config param */
typedef enum {
  // CAM Dependent Param
  RKADK_PARAM_TYPE_FPS,             /* framerate */
  RKADK_PARAM_TYPE_GOP,             /* gop */
  RKADK_PARAM_TYPE_RES,             /* specify RKADK_PARAM_RES_E(record) */
  RKADK_PARAM_TYPE_PHOTO_RES,       /* specify RKADK_PARAM_RES_E(photo) */
  RKADK_PARAM_TYPE_CODEC_TYPE,      /* specify RKADK_PARAM_CODEC_CFG_S */
  RKADK_PARAM_TYPE_BITRATE,         /* specify RKADK_PARAM_BITRATE_S */
  RKADK_PARAM_TYPE_FLIP,            /* bool */
  RKADK_PARAM_TYPE_MIRROR,          /* bool */
  RKADK_PARAM_TYPE_LDC,             /* ldc level [0,255] */
  RKADK_PARAM_TYPE_ANTIFOG,         /* antifog value, [0,10] */
  RKADK_PARAM_TYPE_WDR,             /* wdr level, [0,10] */
  RKADK_PARAM_TYPE_HDR,             /* 0: normal, 1: HDR2, 2: HDR3, [0,2] */
  RKADK_PARAM_TYPE_RECORD_TYPE,     /* specify RKADK_REC_TYPE_E */
  RKADK_PARAM_TYPE_RECORD_TIME,     /* specify RKADK_PARAM_REC_TIME_S, record time(s) */
  RKADK_PARAM_TYPE_PRE_RECORD_TIME, /* pre record time, unit in second(s) */
  RKADK_PARAM_TYPE_PRE_RECORD_MODE, /* pre record mode, specify MUXER_PRE_RECORD_MODE_E */
  RKADK_PARAM_TYPE_SPLITTIME, /* specify RKADK_PARAM_REC_TIME_S, manual splite time(s) */
  RKADK_PARAM_TYPE_FILE_CNT,  /* record file count, maximum RECORD_FILE_NUM_MAX */
  RKADK_PARAM_TYPE_LAPSE_INTERVAL, /* specify RKADK_PARAM_REC_TIME_S, lapse interval(s) */
  RKADK_PARAM_TYPE_LAPSE_MULTIPLE, /* lapse multiple */
  RKADK_PARAM_TYPE_JPEG_SLICE,     /* enable/disable jpeg slice */
  RKADK_PARAM_TYPE_SLICE_HEIGHT,     /* set jpeg slice height */

  // COMM Dependent Param
  RKADK_PARAM_TYPE_REC_MUTE,      /* record audio mute, bool */
  RKADK_PARAM_TYPE_VOLUME,          /* speaker volume, [0,100] */
  RKADK_PARAM_TYPE_MIC_VOLUME,      /* mic volume, [0,100] */
  RKADK_PARAM_TYPE_BUTT
} RKADK_PARAM_TYPE_E;

typedef struct {
  RKADK_STREAM_TYPE_E enStreamType;
  RKADK_CODEC_TYPE_E enCodecType;
} RKADK_PARAM_CODEC_CFG_S;

typedef struct {
  RKADK_STREAM_TYPE_E enStreamType;
  RKADK_U32 u32Bitrate;
} RKADK_PARAM_BITRATE_S;

typedef struct {
  RKADK_STREAM_TYPE_E enStreamType;
  RKADK_U32 time;
} RKADK_PARAM_REC_TIME_S;

typedef struct {
  RKADK_STREAM_TYPE_E enStreamType;
  RKADK_U32 u32Framerate;
} RKADK_PARAM_FPS_S;

typedef struct {
  RKADK_STREAM_TYPE_E enStreamType;
  RKADK_U32 u32Gop;
} RKADK_PARAM_GOP_S;

typedef struct tagRKADK_PARAM_VERSION_S {
  char version[RKADK_BUFFER_LEN];
} RKADK_PARAM_VERSION_S;

typedef struct tagRKADK_PARAM_VI_CFG_S {
  RKADK_U32 chn_id;
  char device_name[MAX_VI_ENTITY_NAME_LEN];
  RKADK_U32 buf_cnt;
  RKADK_U32 width;
  RKADK_U32 height;
  RKADK_U32 depth;
  char pix_fmt[RKADK_PIX_FMT_LEN]; /* options: NV12/NV16/YUYV/FBC0/FBC2 */

  /* options: NONE/RECORD_MAIN/RECORD_SUB/PREVIEW/PHOTO/LIVE/DISP */
  char module[RKADK_BUFFER_LEN];
} RKADK_PARAM_VI_CFG_S;

typedef struct tagRKADK_PARAM_COMM_CFG_S {
  RKADK_U32 sensor_count;
  bool rec_mute;          /* false:disable record audio, true:enable */
  RKADK_U32 speaker_volume; /* speaker volume, [0,100] */
  RKADK_U32 mic_volume;     /* mic input volume, [0,100] */
  RKADK_U32 vpss_devcie; /* 0: GPU device, 1: RGA device */
} RKADK_PARAM_COMM_CFG_S;

typedef struct tagRKADK_PARAM_SENSOR_CFG_S {
  bool used_isp;
  RKADK_U32 max_width;
  RKADK_U32 max_height;
  RKADK_U32 framerate;
  bool flip;          /* FLIP */
  bool mirror;        /* MIRROR */
  bool enable_wrap;
  RKADK_U32 wrap_buf_line;
  RKADK_U32 ldc;      /* LDC level, [0,255]*/
  RKADK_U32 wdr;      /* WDR level, [0,10] */
  RKADK_U32 hdr;      /* hdr, [0: normal, 1: HDR2, 2: HDR3] */
  RKADK_U32 antifog;  /* antifog value, [0,10] */
} RKADK_PARAM_SENSOR_CFG_S;

typedef struct tagRKADK_PARAM_AUDIO_CFG_S {
  char ai_audio_node[RKADK_BUFFER_LEN];
  RKADK_U32 ai_depth;
  AUDIO_BIT_WIDTH_E bit_width;
  RKADK_U32 channels;
  RKADK_U32 mic_type;
  RKADK_U32 samplerate;
  RKADK_U32 samples_per_frame;
  RKADK_U32 bitrate;
  RKADK_VQE_MODE_E vqe_mode;
  char vqe_config_path[RKADK_PATH_LEN];
  RKADK_CODEC_TYPE_E codec_type;
} RKADK_PARAM_AUDIO_CFG_S;

typedef struct tagRKADK_PARAM_VENC_PARAM_S {
  /* rc param */
  RKADK_S32 first_frame_qp; /* start QP value of the first frame */
  RKADK_S32 qp_step;
  RKADK_S32 max_qp; /* max QP: [8, 51] */
  RKADK_S32 min_qp; /* min QP: [0, 48], can't be larger than max_qp */
  RKADK_S32 frame_min_qp; /* range:[0, 51]; the frame min QP value, recommended larger than min_qp */
  RKADK_S32 i_min_qp; /* min qp for i frame */
  RKADK_S32 i_frame_min_qp; /* range:[0, 51]; the I frame min QP value, recommended larger than i_min_qp */

  bool hier_qp_en;
  char hier_qp_delta[RKADK_BUFFER_LEN];
  char hier_frame_num[RKADK_BUFFER_LEN];

  bool full_range;
  bool scaling_list;
} RKADK_PARAM_VENC_PARAM_S;

typedef struct tagRKADK_PARAM_VENC_ATTR_S {
  RKADK_U32 width;
  RKADK_U32 height;
  RKADK_U32 bufsize;
  RKADK_U32 bitrate;
  RKADK_U32 framerate;
  RKADK_U32 gop;
  RKADK_U32 profile;
  RKADK_CODEC_TYPE_E codec_type;
  RKADK_U32 venc_chn;
  RKADK_U32 vpss_grp;
  RKADK_U32 vpss_chn;
  bool post_aiisp;
  char rc_mode[RKADK_RC_MODE_LEN]; /* options: CBR/VBR/AVBR */
  RKADK_PARAM_VENC_PARAM_S venc_param;
} RKADK_PARAM_VENC_ATTR_S;

typedef struct {
  RKADK_U32 u32ViChn;
  VI_CHN_ATTR_S stChnAttr;
} RKADK_PRAAM_VI_ATTR_S;

typedef struct tagRKADK_PARAM_REC_TIME_CFG_S {
  RKADK_U32 record_time;
  RKADK_U32 splite_time;
  RKADK_U32 lapse_interval;
} RKADK_PARAM_REC_TIME_CFG_S;

typedef struct tagRKADK_PARAM_REC_CFG_S {
  bool switch_res;
  bool enable_audio;
  RKADK_REC_TYPE_E record_type;
  RKADK_MUXER_FILE_TYPE_E file_type;
  RKADK_U32 pre_record_time;
  RKADK_MUXER_PRE_RECORD_MODE_E pre_record_mode;
  RKADK_U32 lapse_multiple;
  RKADK_U32 file_num;
  RKADK_PARAM_REC_TIME_CFG_S record_time_cfg[RECORD_FILE_NUM_MAX];
  RKADK_PARAM_VENC_ATTR_S attribute[RECORD_FILE_NUM_MAX];
  RKADK_PRAAM_VI_ATTR_S vi_attr[RECORD_FILE_NUM_MAX];
} RKADK_PARAM_REC_CFG_S;

typedef struct tagRKADK_PARAM_STREAM_CFG_S {
  RKADK_PARAM_VENC_ATTR_S attribute;
  RKADK_PRAAM_VI_ATTR_S vi_attr;
} RKADK_PARAM_STREAM_CFG_S;

typedef struct tagRKADK_PARAM_PHOTO_CFG_S {
  RKADK_U32 image_width;
  RKADK_U32 image_height;
  RKADK_U32 venc_chn;
  RKADK_U32 vpss_grp;
  RKADK_U32 vpss_chn;
  bool post_aiisp;
  bool enable_combo;
  RKADK_U32 combo_venc_chn;
  RKADK_U32 qfactor;
  bool switch_res;
  bool jpeg_slice;
  RKADK_U32 slice_height;
  RKADK_U32 max_slice_width;
  RKADK_U32 max_slice_height;
  RKADK_PRAAM_VI_ATTR_S vi_attr;
} RKADK_PARAM_PHOTO_CFG_S;

typedef struct tagRKADK_PARAM_DISP_CFG_S {
  RKADK_U32 x;
  RKADK_U32 y;
  RKADK_U32 width;
  RKADK_U32 height;
  RKADK_U32 frame_rate;
  RKADK_U32 rotation;
  RKADK_U32 vpss_grp;
  RKADK_U32 vpss_chn;
  char img_type[RKADK_PIX_FMT_LEN]; /* specify PIXEL_FORMAT_E: NV12/RGB888... */
  char intf_type[RKADK_INTF_FMT_LEN]; /* specify RKADK_VO_INTF_TYPE_E */
  char splice_mode[RKADK_SPLICE_MODE_LEN]; /* vo layer splice mode: RGA/GPU/BYPASS */
  RKADK_U32 vo_device;
  RKADK_U32 vo_layer;
  RKADK_U32 vo_chn;
  RKADK_PRAAM_VI_ATTR_S vi_attr;
} RKADK_PARAM_DISP_CFG_S;

typedef struct tagRKADK_PARAM_THUMB_CFG_S {
  // 4 alignment
  RKADK_U32 thumb_width;
  // 2 alignment
  RKADK_U32 thumb_height;
  // photo
  RKADK_U32 photo_venc_chn;
  // record
  RKADK_U32 record_main_venc_chn;
  RKADK_U32 record_sub_venc_chn;
  RKADK_U32 vpss_grp;
  RKADK_U32 vpss_chn;
  RKADK_U32 qfactor;
  RKADK_PRAAM_VI_ATTR_S vi_attr;
} RKADK_PARAM_THUMB_CFG_S;

typedef struct tagRKADK_PARAM_MEDIA_CFG_S {
  RKADK_PARAM_VI_CFG_S stViCfg[RKADK_ISPP_VI_NODE_CNT];
  RKADK_PARAM_REC_CFG_S stRecCfg;
  RKADK_PARAM_STREAM_CFG_S stStreamCfg;
  RKADK_PARAM_STREAM_CFG_S stLiveCfg;
  RKADK_PARAM_PHOTO_CFG_S stPhotoCfg;
  RKADK_PARAM_DISP_CFG_S stDispCfg;
  RKADK_PARAM_THUMB_CFG_S stThumbCfg;
} RKADK_PARAM_MEDIA_CFG_S;

typedef struct tagPARAM_CFG_S {
  RKADK_PARAM_VERSION_S stVersion;
  RKADK_PARAM_COMM_CFG_S stCommCfg;
  RKADK_PARAM_AUDIO_CFG_S stAudioCfg;
  RKADK_PARAM_SENSOR_CFG_S stSensorCfg[RKADK_MAX_SENSOR_CNT];
  RKADK_PARAM_MEDIA_CFG_S stMediaCfg[RKADK_MAX_SENSOR_CNT];
} RKADK_PARAM_CFG_S;

/* Param Context */
typedef struct {
  bool bInit;                /* module init status */
  pthread_mutex_t mutexLock; /* param lock, protect pstCfg */
  RKADK_PARAM_CFG_S stCfg;   /* param config */
  char defPath[RKADK_PATH_LEN];
  char defSensorPath[RKADK_MAX_SENSOR_CNT][RKADK_PATH_LEN];
  char path[RKADK_PATH_LEN];
  char sensorPath[RKADK_MAX_SENSOR_CNT][RKADK_PATH_LEN];
} RKADK_PARAM_CONTEXT_S;

/**
 * @brief     Parameter Module Init
 * @param[in] globalSetting: global setting ini file path
 * @param[in] sesnorSettingArrary: sensor setting ini file path arrary
 * @return    0 success,non-zero error code.
 */
RKADK_S32 RKADK_PARAM_Init(char *globalSetting, char **sesnorSettingArrary);

/**
 * @brief     Parameter Module Deinit
 * @return    0 success,non-zero error code.
 */
RKADK_S32 RKADK_PARAM_Deinit(RKADK_VOID);

/**
 * @brief         get cam related Item Values.
 * @param[in]     s32CamID: the specify cam id,valid value range:
 * [0,RKADK_PDT_MEDIA_VCAP_DEV_MAX_CNT]
 * @param[in]     enType: param type
 * @param[in]     pvParam: param value
 * @return 0      success,non-zero error code.
 */
RKADK_S32 RKADK_PARAM_GetCamParam(RKADK_S32 s32CamID,
                                  RKADK_PARAM_TYPE_E enParamType,
                                  RKADK_VOID *pvParam);

/**
 * @brief         set cam related Item Values.
 * @param[in]     enWorkMode: workmode
 * @param[in]     s32CamID: the specify cam id,valid value range:
 * [0,RKADK_PDT_MEDIA_VCAP_DEV_MAX_CNT]
 * @param[in]     enType: param type
 * @param[in]     pvParam: param value
 * @return        0 success,non-zero error code.
 */
RKADK_S32 RKADK_PARAM_SetCamParam(RKADK_S32 s32CamID,
                                  RKADK_PARAM_TYPE_E enParamType,
                                  const RKADK_VOID *pvParam);

/**
 * @brief        get common parameter configure
 * @param[in]    enParamType : param type
 * @param[out]   pvParam : param value
 * @return       0 success,non-zero error code.
 */
RKADK_S32 RKADK_PARAM_GetCommParam(RKADK_PARAM_TYPE_E enParamType,
                                   RKADK_VOID *pvParam);

/**
 * @brief        set common parameter configure
 * @param[in]    enParamType : param type
 * @param[out]   pvParam : param value
 * @return       0 success,non-zero error code.
 */
RKADK_S32 RKADK_PARAM_SetCommParam(RKADK_PARAM_TYPE_E enParamType,
                                   const RKADK_VOID *pvParam);

/**
 * @brief        defualt all parameters configure
 * @return       0 success,non-zero error code.
 */
RKADK_S32 RKADK_PARAM_SetDefault(RKADK_VOID);

/**
 * @brief        RKADK_PARAM_RES_E to width and height
 * @return       0 success,non-zero error code.
 */
RKADK_S32 RKADK_PARAM_GetResolution(RKADK_PARAM_RES_E type, RKADK_U32 *width,
                                    RKADK_U32 *height);

/**
 * @brief        width and height to RKADK_PARAM_RES_E
 * @return       0 success,non-zero error code.
 */
RKADK_PARAM_RES_E RKADK_PARAM_GetResType(RKADK_U32 width, RKADK_U32 height);

/**
 * @brief        get venc chn id
 * @return       venc chn id success,-1 error code.
 */
RKADK_S32 RKADK_PARAM_GetVencChnId(RKADK_U32 u32CamId,
                                   RKADK_STREAM_TYPE_E enStrmType);

RKADK_S32 RKADK_PARAM_GetThumbChnId(RKADK_U32 u32CamId,
                                   RKADK_STREAM_TYPE_E enStrmType);

void RKADK_PARAM_GetVpssId(RKADK_U32 u32CamId, RKADK_STREAM_TYPE_E enStrmType,
                                   RKADK_S32 *s32VpssGrp, RKADK_S32 *s32VpssChn);


RKADK_PARAM_CONTEXT_S *RKADK_PARAM_GetCtx(RKADK_VOID);

RKADK_PARAM_COMM_CFG_S *RKADK_PARAM_GetCommCfg();

RKADK_PARAM_REC_CFG_S *RKADK_PARAM_GetRecCfg(RKADK_U32 u32CamId);

RKADK_PARAM_STREAM_CFG_S *
RKADK_PARAM_GetStreamCfg(RKADK_U32 u32CamId, RKADK_STREAM_TYPE_E enStrmType);

RKADK_PARAM_PHOTO_CFG_S *RKADK_PARAM_GetPhotoCfg(RKADK_U32 u32CamId);

RKADK_PARAM_SENSOR_CFG_S *RKADK_PARAM_GetSensorCfg(RKADK_U32 u32CamId);

RKADK_PARAM_DISP_CFG_S *RKADK_PARAM_GetDispCfg(RKADK_U32 u32CamId);

RKADK_PARAM_THUMB_CFG_S *RKADK_PARAM_GetThumbCfg(RKADK_U32 u32CamId);

RKADK_PARAM_AUDIO_CFG_S *RKADK_PARAM_GetAudioCfg(RKADK_VOID);

VENC_RC_MODE_E RKADK_PARAM_GetRcMode(char *rcMode,
                                     RKADK_CODEC_TYPE_E enCodecType);

RKADK_S32 RKADK_PARAM_SetVAdvancedParam(RKADK_PARAM_VENC_ATTR_S stVencAttr);

RKADK_STREAM_TYPE_E RKADK_PARAM_VencChnMux(RKADK_U32 u32CamId,
                                           RKADK_U32 u32ChnId);

PIXEL_FORMAT_E RKADK_PARAM_GetPixFmt(char *pixFmt, COMPRESS_MODE_E *enCompressMode);

RKADK_VO_INTF_TYPE_E RKADK_PARAM_GetIntfType(char *intfType);

RKADK_U32 RKADK_PARAM_GetStreamBufCnt(RKADK_U32 u32CamId, bool bIsAudio);

RKADK_VO_INTF_TYPE_E RKADK_PARAM_GetSpliceMode(char *spliceMode);

#ifdef __cplusplus
}
#endif
#endif
