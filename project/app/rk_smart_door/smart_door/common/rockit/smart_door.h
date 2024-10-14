// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <fcntl.h>
#include <termios.h>

#include "rk_defines.h"
#include "rk_debug.h"
#include "rk_mpi_cal.h"
#include "rk_mpi_mb.h"
#include "rk_mpi_sys.h"
#include "rk_mpi_vi.h"
#include "rk_mpi_venc.h"
#include "rk_mpi_vdec.h"
#include "rk_mpi_vpss.h"
#include "rk_mpi_ai.h"
#include "rk_mpi_aenc.h"
#include "rk_mpi_ao.h"
#include "rk_mpi_adec.h"
#include "rk_comm_video.h"
#include "rk_comm_aenc.h"

#define IQFILE_PATH	"/etc/iqfiles/"

#define RK_ARRAY_ELEMS(a)      (sizeof(a) / sizeof((a)[0]))
#define RK_ALIGN(x, a)         (((x) + (a) - 1) & ~((a) - 1))
#define RK_ALIGN_16(x)         RK_ALIGN(x, 16)
#define RK_ALIGN_64(x)         RK_ALIGN(x, 64)
#define RK_ALIGN_256(x)        RK_ALIGN(x, 256)
#define RK_ALIGN_256_ODD(x)    (RK_ALIGN(x, 256) | 256)

typedef struct rk_smart_door_vi_cfg {
    RK_S32 devId;
    RK_S32 pipeId;
    RK_S32 channelId;
    RK_S32 loopCountSet;
    RK_S32 selectFd;
    RK_BOOL bFreeze;
    RK_BOOL bIfIspGroupInit;
    COMPRESS_MODE_E enCompressMode;
    VI_DEV_ATTR_S stDevAttr;
    VI_DEV_BIND_PIPE_S stBindPipe;
    VI_CHN_ATTR_S stChnAttr;
    VI_SAVE_FILE_INFO_S stDebugFile;
    VIDEO_FRAME_INFO_S stViFrame;
    VI_CHN_STATUS_S stChnStatus;
} SMART_DOOR_VI_CFG;

typedef struct rk_smart_door_venc_cfg {
    RK_U32 u32ChnId;
    RK_U32 u32SrcWidth;
    RK_U32 u32SrcHeight;
    RK_U32 u32srcVirWidth;
    RK_U32 u32srcVirHeight;
    RK_U32 u32ChNum;
    PIXEL_FORMAT_E enSrcPixFormat;
    RK_U32 u32DstCodec;
    RK_U32 u32BufferSize;
    RK_U32 u32StreamBufCnt;
    RK_U32 u32BitRateKb;
    RK_U32 u32GopSize;
    RK_U32 u32RoiTestCount;
    RK_U32 u32FixQp;
    RK_S32 s32SnapPicCount;
    MB_POOL vencPool;
    RK_BOOL bFrameRate;
    RK_BOOL bInsertUserData;
    COMPRESS_MODE_E enCompressMode;
    VENC_RC_MODE_E enRcMode;
    VENC_GOP_MODE_E enGopMode;
    VENC_CROP_TYPE_E enCropType;
    ROTATION_E enRotation;
    MIRROR_E enMirror;
    RK_BOOL bSuperFrm;
    RK_BOOL bFrmLost;
    RK_BOOL bIntraRefresh;
    RK_BOOL bHierarchicalQp;
    RK_BOOL bMjpegParam;
    RK_BOOL bForceIdr;
    RK_BOOL bFullRange;
    pthread_t pGetStreamThread;
    RK_BOOL threadExit;
    RK_BOOL vencTaskEnd;
    void *(*venc_func)(void *);
    const char *dstFilePath;
    RK_BOOL sendRtsp;
    RK_S32 loopCount;
    RK_BOOL bChangeResolution;
    RK_U32 u32ResWidth;
    RK_U32 u32ResHeight;
    RK_S32(*change_res_func)(int, int);
} SMART_DOOR_VENC_CFG;

typedef struct rk_smart_door_vpss_cfg {
    RK_U32 u32GrpId;
    RK_U32 u32ChnId[VPSS_MAX_CHN_NUM];
    RK_U32 u32VpssChnCnt;
    RK_BOOL bMirror;
    RK_BOOL bFlip;
    ROTATION_E rotation;
    VPSS_GRP_ATTR_S stGrpVpssAttr;
    VPSS_CHN_ATTR_S stVpssChnAttr[VPSS_MAX_CHN_NUM];
} SMART_DOOR_VPSS_CFG;

typedef struct rk_smart_door_ai_cfg {
    const char *srcFilePath;
    const char *dstFilePath;
    RK_S32      s32LoopCount;
    RK_S32      s32ChnNum;
    RK_S32      s32DeviceSampleRate;
    RK_S32      s32SampleRate;
    RK_S32      s32DeviceChannel;
    RK_S32      s32Channel;
    RK_S32      s32BitWidth;
    RK_S32      s32DevId;
    RK_S32      s32PeriodCount;
    RK_S32      s32PeriodSize;
    char       *chCardName;
    RK_S32      s32ChnIndex;
    RK_S32      s32SetVolume;
    RK_S32      s32SetMute;
    RK_S32      s32SetFadeRate;
    RK_S32      s32SetTrackMode;
    RK_S32      s32GetVolume;
    RK_S32      s32GetMute;
    RK_S32      s32GetTrackMode;
    RK_S32      s32LoopbackMode;
    RK_S32      s32DevFd;
    RK_S32      s32AedEnable;
    RK_S32      s32BcdEnable;
    RK_S32      s32VqeGapMs;
    RK_S32      s32VqeEnable;
    RK_S32      s32DumpAlgo;
    const char *pVqeCfgPath;
} SMART_DOOR_AI_CFG;

typedef struct rk_smart_door_aenc_cfg {
    RK_S32      s32DevId;
    RK_S32      u32ChnId;
    RK_S32      s32Channel;
    RK_S32      s32SampleRate;
    RK_S32      s32MaxFrame;
    AUDIO_BIT_WIDTH_E   enBitwidth;
    RK_CODEC_ID_E     enType;
    const char *aencFilePath;
    AENC_CHN_ATTR_S pstChnAttr;
    FILE *fp;
} SMART_DOOR_AENC_CFG;

typedef struct rk_smart_door_ao_cfg {
    const char *srcFilePath;
    const char *dstFilePath;
    RK_S32      s32LoopCount;
    RK_S32      s32ChnNum;
    RK_S32      s32SampleRate;
    RK_S32      s32ReSmpSampleRate;
    RK_S32      s32Channel;
    RK_S32      s32DeviceChannel;
    RK_S32      s32BitWidth;
    RK_S32      s32DevId;
    RK_S32      s32PeriodCount;
    RK_S32      s32PeriodSize;
    char       *chCardName;
    RK_S32      s32ChnIndex;
    RK_S32      s32SetVolume;
    RK_S32      s32SetMute;
    RK_S32      s32SetFadeRate;
    RK_S32      s32SetTrackMode;
    RK_S32      s32GetVolume;
    RK_S32      s32GetMute;
    RK_S32      s32GetTrackMode;
    RK_S32      s32QueryChnStat;
    RK_S32      s32PauseResumeChn;
    RK_S32      s32SaveFile;
    RK_S32      s32QueryFileStat;
    RK_S32      s32ClrChnBuf;
    RK_S32      s32ClrPubAttr;
    RK_S32      s32GetPubAttr;
    RK_S32      s32LoopbackMode;
} SMART_DOOR_AO_CFG;

typedef struct rk_smart_door_adec_cfg {
    RK_S32      s32DevId;
    RK_S32      u32ChnId;
    RK_S32      s32Channel;
    RK_S32      s32ReSmpSampleRate;
    RK_CODEC_ID_E     enType;
    const char *adecFilePath;
} SMART_DOOR_ADEC_CFG;

typedef struct rk_smart_door_ctx {
    pthread_t pGetAencStreamThread;
    RK_BOOL aencthreadExit;
    RK_BOOL aencTaskEnd;
    pthread_t pGetAdecStreamThread;
    RK_BOOL adecthreadExit;
    RK_BOOL adecTaskEnd;
    SMART_DOOR_VI_CFG stViCfg[3];
    SMART_DOOR_VPSS_CFG stVpssCfg;
    SMART_DOOR_VENC_CFG stVencCfg[3];
    SMART_DOOR_AI_CFG   stAiCfg;
    SMART_DOOR_AENC_CFG stAencCfg;
    SMART_DOOR_ADEC_CFG stAdecCfg;
    SMART_DOOR_AO_CFG   stAoCfg;
} SMART_DOOR_CTX_S;

SMART_DOOR_CTX_S *ctx;

RK_S32 smart_door_init_vi(SMART_DOOR_VI_CFG *stViCfg);
RK_S32 smart_door_create_vi(SMART_DOOR_VI_CFG stViCfg);
RK_S32 smart_door_vi_get_frame(SMART_DOOR_VI_CFG *vi_cfg, void **data);
RK_S32 smart_door_vi_release_frame(SMART_DOOR_VI_CFG *vi_cfg);
RK_S32 smart_door_destroy_vi(SMART_DOOR_VI_CFG stViCfg);

RK_S32 smart_door_init_vpss(SMART_DOOR_CTX_S *ctx);
RK_S32 smart_door_create_vpss(SMART_DOOR_VPSS_CFG stVpssCfg);
RK_S32 smart_door_destroy_vpss(SMART_DOOR_VPSS_CFG stVpssCfg);

RK_S32 smart_door_init_venc(SMART_DOOR_VENC_CFG *stVencCfg);
RK_S32 smart_door_create_venc(SMART_DOOR_VENC_CFG stVencCfg);
RK_S32 smart_door_start_venc(SMART_DOOR_VENC_CFG *stVencCfg);
RK_S32 smart_door_destroy_venc(SMART_DOOR_VENC_CFG stVencCfg);
void* smart_door_venc_get_stream(void *pArgs);

RK_S32 smart_door_init_ai(SMART_DOOR_CTX_S *ctx);
RK_S32 smart_door_create_ai(SMART_DOOR_AI_CFG stAiCfg);
RK_S32 smart_door_destroy_ai(SMART_DOOR_AI_CFG stAiCfg);

RK_S32 smart_door_init_aenc(SMART_DOOR_CTX_S *ctx);
RK_S32 smart_door_create_aenc(SMART_DOOR_AENC_CFG stAencCfg);
RK_S32 smart_door_start_aenc(SMART_DOOR_CTX_S *ctx);
RK_S32 smart_door_destroy_aenc(SMART_DOOR_CTX_S *ctx);

RK_S32 smart_door_init_adec(SMART_DOOR_CTX_S *ctx);
RK_S32 smart_door_create_adec(SMART_DOOR_ADEC_CFG stAdecCfg);
RK_S32 smart_door_start_adec(SMART_DOOR_CTX_S *ctx);
RK_S32 smart_door_destroy_adec(SMART_DOOR_CTX_S *ctx);

RK_S32 smart_door_init_ao(SMART_DOOR_CTX_S *ctx);
RK_S32 smart_door_create_ao(SMART_DOOR_AO_CFG stAoCfg);
RK_S32 smart_door_destroy_ao(SMART_DOOR_AO_CFG stAoCfg);

int smart_door_pipe_rtsp_start(void);
void smart_door_pipe_rtsp_stop(void);
void smart_door_pipe_rtsp_set_resolution(int width, int height);

int smart_door_pipe_ai_aenc(void);
int smart_door_pipe_adec_ao(void);
int smart_door_uart(void) ;
int uvc_start();
int uvc_stop(void);

void smart_door_rtsp_init();
void smart_door_rtsp_deinit();
void smart_door_rtsp_send_live0(void *data, RK_U32 u32Len, RK_U64 u64PTS);
