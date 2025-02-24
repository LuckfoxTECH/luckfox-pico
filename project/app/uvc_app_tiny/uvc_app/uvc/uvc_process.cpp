/*
 * Copyright (C) 2019 Rockchip Electronics Co., Ltd.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL), available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "uvc_process.h"
#include "osd.h"
#include "uvc_mpi_config.h"
#include "uvc_mpi_venc.h"
#include "uvc_mpi_vi.h"
#include "uvc_mpi_vpss.h"
#include "uvc_video.h"

#include "uvc_log.h"
#include <cstring>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "rk_common.h"
#include "rk_mpi_mb.h"
#include "rk_mpi_rgn.h"
#include "rk_mpi_sys.h"
#include <sys/prctl.h>
#include <thread>

#define UVC_DYNAMIC_DEBUG_OUT_CHECK "/tmp/uvc_out"
#define UVC_DYNAMIC_DEBUG_FPS "/tmp/uvc_fps"

static pthread_mutex_t gUvcProcessLock = PTHREAD_MUTEX_INITIALIZER;
static UVCProcess *gUvcProcess[CAM_MAX_NUM];

typedef enum _UVC_FMT_TYPE {
  UVC_FMT_TYPE_DEBUG_YUYV_UVC = 0, // for debug
  UVC_FMT_TYPE_VI_UVC,             // now only run VI_VPSS_UVC
  UVC_FMT_TYPE_VI_VENC_UVC,        // now only run VI_VPSS_VENC_UVC
  UVC_FMT_TYPE_VI_VPSS_UVC,
  UVC_FMT_TYPE_VI_VPSS_VENC_UVC,
  UVC_FMT_TYPE_MAX,
} UVCFmtType;

typedef struct _UVC_PROCESS_BUFF {
  int fd;
  void *frame;
} UVCProcessBuff;

typedef struct _UVC_VENC_TEMP_INFO {
  VENC_STREAM_S stVencFrame;
  VENC_STREAM_S stReleaseVencFrame;
  int recvCnt;
  int force_idr_cnt;
} UVCVencTempInfo;

typedef struct _UVC_DEBUG_FILE_INFO {
  int debugCnt;
  FILE *pDebugFile;
} UVCDegbuFileInfo;

typedef struct _UVC_PROCESS_CTX {
  UVC_CTRL_INFO mUvcCtx;
  UVC_MPI_CFG mUvcCfg;
  UVC_VI_CHN_CTX_S viChnCtx;
  UVC_VENC_CHN_CTX_S vencChnCtx;
  UVC_VPSS_CHN_CTX_S vpssChnCtx;
  UVCVencTempInfo vencTempInfo;
  UVCProcessBuff *mBuff;
  UVCFmtType mFmtType;
  UVCDegbuFileInfo mUvcDebug;
  int mUvcBuffCount;
  bool mStart;
  std::thread *mThread;
  pthread_cond_t mCond;
  pthread_mutex_t mCondMutex;
  struct uvc_buffer *mUvcBuf;
  // debug
  MB_POOL debugPool;
  int getBufCnt;
  int releaseBufCnt;
  // fps debug
  struct timeval enterTime;
  int sendCount;
} UVCProcessCtx;

typedef void (*uvc_release_callback)();

static UVC_RET_TYPE do_process(UVCProcessCtx *ctx);

// need to actually test whether need this part for prepare.
extern "C" int uvc_process_config(UVC_CTRL_INFO uvcCtrlInfo) {
  RK_S32 s32Ret = RK_FAILURE;

  LOG_INFO(
      "index:%d width = %d, height = %d, fcc = %d, fps = %d, auto_eptz = %d\n",
      uvcCtrlInfo.index, uvcCtrlInfo.width, uvcCtrlInfo.height, uvcCtrlInfo.fcc,
      uvcCtrlInfo.fps, uvcCtrlInfo.eptz[UVC_EPTZ_AUTO]);

  if ((uvcCtrlInfo.fcc == V4L2_PIX_FMT_H264) &&
      !access("/tmp/use_encodec_h265", 0)) {
    LOG_ERROR("force use h265 encodec\n");
    uvcCtrlInfo.fcc = V4L2_PIX_FMT_H265;
  }

  if (uvcCtrlInfo.index >= CAM_MAX_NUM) {
    LOG_ERROR("index:%d exceed max:%d\n", uvcCtrlInfo.index, CAM_MAX_NUM);
    return -1;
  }
  if (gUvcProcess[0] == NULL) {
    // s32Ret = RK_MPI_SYS_Init();
    // if (s32Ret != RK_SUCCESS) {
    //  LOG_ERROR("RK_MPI_SYS_Init %x\n", s32Ret);
    //  return -1;
    //}
  }
  if (gUvcProcess[uvcCtrlInfo.index] == NULL) {
    LOG_INFO("new uvc process index %d\n", uvcCtrlInfo.index);
    gUvcProcess[uvcCtrlInfo.index] = new UVCProcess();
    // configProcess here just do onece.
    // gUvcProcess[uvcCtrlInfo.index]->configProcess(uvcCtrlInfo);
  } else {
    LOG_INFO("old uvc process index %d\n", uvcCtrlInfo.index);
  }
  return gUvcProcess[uvcCtrlInfo.index]->configProcess(uvcCtrlInfo);
}

extern "C" int uvc_process_start(UVC_CTRL_INFO uvcCtrlInfo) {
  RK_S32 s32Ret = RK_FAILURE;

  LOG_INFO("index:%d width = %d, height = %d, fcc = %d, fps = %d, eptz = %d \n",
           uvcCtrlInfo.index, uvcCtrlInfo.width, uvcCtrlInfo.height,
           uvcCtrlInfo.fcc, uvcCtrlInfo.fps, uvcCtrlInfo.eptz[UVC_EPTZ_AUTO]);

  if (uvcCtrlInfo.index >= CAM_MAX_NUM) {
    LOG_ERROR("index:%d exceed max:%d\n", uvcCtrlInfo.index, CAM_MAX_NUM);
    return -1;
  }
  if (gUvcProcess[uvcCtrlInfo.index]) {
    gUvcProcess[uvcCtrlInfo.index]->startProcess();
  }

  return 0;
}

extern "C" void uvc_process_stop(UVC_CTRL_INFO uvcCtrlInfo) {
  LOG_INFO("stop uvc process index %d\n", uvcCtrlInfo.index);

  if (gUvcProcess[uvcCtrlInfo.index]) {
    gUvcProcess[uvcCtrlInfo.index]->stopProcess();
    // delete remove to process_delete.
  }
}

extern "C" void uvc_process_delete(UVC_CTRL_INFO uvcCtrlInfo) {
  LOG_INFO("delete uvc process index %d %p\n", uvcCtrlInfo.index,
           gUvcProcess[uvcCtrlInfo.index]);

  if (gUvcProcess[uvcCtrlInfo.index]) {
    delete gUvcProcess[uvcCtrlInfo.index];
    gUvcProcess[uvcCtrlInfo.index] = nullptr;
  }
}

extern "C" int uvc_process_release_frame(UVC_CTRL_INFO uvcCtrlInfo,
                                         void *frame) {
  if (gUvcProcess[uvcCtrlInfo.index]) {
    gUvcProcess[uvcCtrlInfo.index]->releaseFrame(frame);
  }

  return 0;
}

void _uvc_process_release_frame(UVCProcessCtx *ctx, void *frame) {
  MB_BLK blk = (MB_BLK)frame;
  if (!blk) {
    LOG_ERROR("blk is null\n");
    return;
  }
  ctx->releaseBufCnt++;
  LOG_DEBUG("releaseFrame:%p, cnt:%d\n", blk, ctx->releaseBufCnt);

  switch (ctx->mFmtType) {
  case UVC_FMT_TYPE_DEBUG_YUYV_UVC:
    RK_MPI_MB_ReleaseMB(blk);
    break;
  case UVC_FMT_TYPE_VI_UVC:
    VIDEO_FRAME_INFO_S stViFrameInfo;
    stViFrameInfo.stVFrame.pMbBlk = blk;
    RK_MPI_VI_ReleaseChnFrame(ctx->viChnCtx.pipeId, ctx->viChnCtx.channelId,
                              &stViFrameInfo);
    break;
  case UVC_FMT_TYPE_VI_VPSS_UVC:
    VIDEO_FRAME_INFO_S stVpssFrameInfo;
    stVpssFrameInfo.stVFrame.pMbBlk = blk;
    RK_MPI_VPSS_ReleaseChnFrame(ctx->vpssChnCtx.group,
                                ctx->vpssChnCtx.channelId, &stVpssFrameInfo);
    break;
  case UVC_FMT_TYPE_VI_VENC_UVC:
  case UVC_FMT_TYPE_VI_VPSS_VENC_UVC:
    ctx->vencTempInfo.stReleaseVencFrame.pstPack->pMbBlk = blk;
    RK_MPI_VENC_ReleaseStream(ctx->vencChnCtx.channelId,
                              &ctx->vencTempInfo.stReleaseVencFrame);
    break;
  default:
    LOG_ERROR("not support this type:%d\n", ctx->mFmtType);
    break;
  }
}

UVCProcessCtx *getUVCProcessCtx(void *ctx) {
  return reinterpret_cast<UVCProcessCtx *>(ctx);
}

int UVCProcess::initialize() {
  UVCProcessCtx *ctx = getUVCProcessCtx(mCtx);
  uvc_mpi_cfg_set_default(&ctx->mUvcCfg);

  return 0;
}

int UVCProcess::deinitialize() {
  UVCProcessCtx *ctx = getUVCProcessCtx(mCtx);

  if (ctx)
    free(ctx);

  return 0;
}

UVCProcess::UVCProcess() : mCtx(NULL) {
  UVCProcessCtx *ctx = (UVCProcessCtx *)calloc(sizeof(UVCProcessCtx), 1);
  mCtx = reinterpret_cast<void *>(ctx);

  initialize();
}

UVCProcess::~UVCProcess() { deinitialize(); }

int UVCProcess::configVi() {
  UVCProcessCtx *ctx = getUVCProcessCtx(mCtx);
#ifndef RK_ENABLE_FASTBOOT
  uvc_vi_config(&ctx->mUvcCfg, MPI_VI_CHANNEL_TYPE_UVC);
#endif
  ctx->viChnCtx = uvc_get_vi_chn_ctx(ctx->mUvcCfg, MPI_VI_CHANNEL_TYPE_UVC);

  return 0;
}

int UVCProcess::configVenc() {
  UVCProcessCtx *ctx = getUVCProcessCtx(mCtx);
#ifndef RK_ENABLE_FASTBOOT
  uvc_venc_config(&ctx->mUvcCfg);
#endif
  ctx->vencChnCtx = uvc_get_venc_chn_ctx(ctx->mUvcCfg);

  return 0;
}

int UVCProcess::configVpss() {
  UVCProcessCtx *ctx = getUVCProcessCtx(mCtx);
  uvc_vpss_config(&ctx->mUvcCfg, MPI_VPSS_CHANNEL_TYPE_UVC);
  ctx->vpssChnCtx =
      uvc_get_vpss_chn_ctx(ctx->mUvcCfg, MPI_VPSS_CHANNEL_TYPE_UVC);

  return 0;
}

int UVCProcess::startVi() {
  UVCProcessCtx *ctx = getUVCProcessCtx(mCtx);
#ifndef RK_ENABLE_FASTBOOT
  uvc_vi_start(ctx->mUvcCfg, MPI_VI_CHANNEL_TYPE_UVC);
#endif
  return 0;
}

int UVCProcess::stopVi() {
  UVCProcessCtx *ctx = getUVCProcessCtx(mCtx);
#ifndef RK_ENABLE_FASTBOOT
  uvc_vi_stop(ctx->mUvcCfg, MPI_VI_CHANNEL_TYPE_UVC);
#endif
  return 0;
}

int UVCProcess::startVpss() {
  UVCProcessCtx *ctx = getUVCProcessCtx(mCtx);
  uvc_vpss_start(ctx->mUvcCfg, MPI_VPSS_CHANNEL_TYPE_UVC);

  return 0;
}

int UVCProcess::stopVpss() {
  UVCProcessCtx *ctx = getUVCProcessCtx(mCtx);
  uvc_vpss_stop(ctx->mUvcCfg, MPI_VPSS_CHANNEL_TYPE_UVC);

  return 0;
}

int UVCProcess::startVenc() {
  UVCProcessCtx *ctx = getUVCProcessCtx(mCtx);
#ifndef RK_ENABLE_FASTBOOT
  uvc_venc_start(ctx->mUvcCfg);
#endif
  ctx->vencTempInfo.stVencFrame.pstPack =
      reinterpret_cast<VENC_PACK_S *>(malloc(sizeof(VENC_PACK_S)));
  ctx->vencTempInfo.stReleaseVencFrame.pstPack =
      reinterpret_cast<VENC_PACK_S *>(malloc(sizeof(VENC_PACK_S)));

  return 0;
}

int UVCProcess::stopVenc() {
  UVCProcessCtx *ctx = getUVCProcessCtx(mCtx);
  uvc_venc_stop(ctx->mUvcCfg);
  if (ctx->vencTempInfo.stVencFrame.pstPack)
    free(ctx->vencTempInfo.stVencFrame.pstPack);
  if (ctx->vencTempInfo.stReleaseVencFrame.pstPack)
    free(ctx->vencTempInfo.stReleaseVencFrame.pstPack);

  return 0;
}

int UVCProcess::bindViVpss() {
  UVCProcessCtx *ctx = getUVCProcessCtx(mCtx);
  int ret;
  MPP_CHN_S mSrcChn;
  MPP_CHN_S mDestChn;

  mSrcChn.enModId = RK_ID_VI;
  mSrcChn.s32DevId = ctx->viChnCtx.devId;
  mSrcChn.s32ChnId = ctx->viChnCtx.channelId;
  mDestChn.enModId = RK_ID_VPSS;
  mDestChn.s32DevId = ctx->vpssChnCtx.group;
  mDestChn.s32ChnId = ctx->vpssChnCtx.channelId;
  ret = RK_MPI_SYS_Bind(&mSrcChn, &mDestChn);

  return ret;
}

int UVCProcess::unBindViVpss() {
  UVCProcessCtx *ctx = getUVCProcessCtx(mCtx);
  int ret;
  MPP_CHN_S mSrcChn;
  MPP_CHN_S mDestChn;

  mSrcChn.enModId = RK_ID_VI;
  mSrcChn.s32DevId = ctx->viChnCtx.devId;
  mSrcChn.s32ChnId = ctx->viChnCtx.channelId;
  mDestChn.enModId = RK_ID_VPSS;
  mDestChn.s32DevId = ctx->vpssChnCtx.group;
  mDestChn.s32ChnId = ctx->vpssChnCtx.channelId;
  ret = RK_MPI_SYS_UnBind(&mSrcChn, &mDestChn);

  return ret;
}

int UVCProcess::bindViVenc() {
  UVCProcessCtx *ctx = getUVCProcessCtx(mCtx);
  int ret;
  MPP_CHN_S mSrcChn;
  MPP_CHN_S mDestChn;

  mSrcChn.enModId = RK_ID_VI;
  mSrcChn.s32DevId = ctx->viChnCtx.devId;
  mSrcChn.s32ChnId = ctx->viChnCtx.channelId;
  mDestChn.enModId = RK_ID_VENC;
  mDestChn.s32DevId = ctx->vencChnCtx.devId;
  mDestChn.s32ChnId = ctx->vencChnCtx.channelId;
  ret = RK_MPI_SYS_Bind(&mSrcChn, &mDestChn);

  return ret;
}

int UVCProcess::unBindViVenc() {
  UVCProcessCtx *ctx = getUVCProcessCtx(mCtx);
  int ret;
  MPP_CHN_S mSrcChn;
  MPP_CHN_S mDestChn;

  mSrcChn.enModId = RK_ID_VI;
  mSrcChn.s32DevId = ctx->viChnCtx.devId;
  mSrcChn.s32ChnId = ctx->viChnCtx.channelId;
  mDestChn.enModId = RK_ID_VENC;
  mDestChn.s32DevId = ctx->vencChnCtx.devId;
  mDestChn.s32ChnId = ctx->vencChnCtx.channelId;
  ret = RK_MPI_SYS_UnBind(&mSrcChn, &mDestChn);

  return ret;
}

int UVCProcess::bindVpssVenc() {
  UVCProcessCtx *ctx = getUVCProcessCtx(mCtx);
  int ret;
  MPP_CHN_S mSrcChn;
  MPP_CHN_S mDestChn;

  mSrcChn.enModId = RK_ID_VPSS;
  mSrcChn.s32DevId = ctx->vpssChnCtx.group;
  mSrcChn.s32ChnId = ctx->vpssChnCtx.channelId;
  mDestChn.enModId = RK_ID_VENC;
  mDestChn.s32DevId = ctx->vencChnCtx.devId;
  mDestChn.s32ChnId = ctx->vencChnCtx.channelId;
  ret = RK_MPI_SYS_Bind(&mSrcChn, &mDestChn);

  return ret;
}

int UVCProcess::unBindVpssVenc() {
  UVCProcessCtx *ctx = getUVCProcessCtx(mCtx);
  int ret;
  MPP_CHN_S mSrcChn;
  MPP_CHN_S mDestChn;

  mSrcChn.enModId = RK_ID_VPSS;
  mSrcChn.s32DevId = ctx->vpssChnCtx.group;
  mSrcChn.s32ChnId = ctx->vpssChnCtx.channelId;
  mDestChn.enModId = RK_ID_VENC;
  mDestChn.s32DevId = ctx->vencChnCtx.devId;
  mDestChn.s32ChnId = ctx->vencChnCtx.channelId;
  ret = RK_MPI_SYS_UnBind(&mSrcChn, &mDestChn);

  return ret;
}

int UVCProcess::configProcess(UVC_CTRL_INFO uvcCtrlInfo) {
  UVCProcessCtx *ctx = getUVCProcessCtx(mCtx);
  int32_t use_time_us, now_time_us, last_time_us;
  struct timespec now_tm = {0, 0};
  int check_ret;

  if (memcmp(&ctx->mUvcCtx, &uvcCtrlInfo, sizeof(UVC_CTRL_INFO))) {
    LOG_INFO("index:%d config process paramter change.\n", uvcCtrlInfo.index);
    memcpy(&ctx->mUvcCtx, &uvcCtrlInfo, sizeof(UVC_CTRL_INFO));
    ctx->mUvcCfg.index = uvcCtrlInfo.index;
    ctx->mUvcCfg.uvc_cfg.width = uvcCtrlInfo.width;
    ctx->mUvcCfg.uvc_cfg.height = uvcCtrlInfo.height;
    ctx->mUvcCfg.uvc_cfg.fps = uvcCtrlInfo.fps;
    ctx->mUvcCfg.uvc_cfg.fcc = uvcCtrlInfo.fcc;

    clock_gettime(CLOCK_MONOTONIC, &now_tm);
    last_time_us = now_tm.tv_sec * 1000000LL + now_tm.tv_nsec / 1000; // us
    check_ret = check_uvc_mpi_cfg_file_init(&ctx->mUvcCfg);
    if (check_ret) {
      // when not config need set this change here
      ctx->mUvcCfg.venc_cfg.common_cfg.change |=
          MPI_VENC_COMMON_CFG_CHANGE_CREATE;
    }
    clock_gettime(CLOCK_MONOTONIC, &now_tm);
    now_time_us = now_tm.tv_sec * 1000000LL + now_tm.tv_nsec / 1000; // us
    use_time_us = now_time_us - last_time_us;
    LOG_INFO("check_uvc_mpi_cfg_file_init time:%d us\n", use_time_us);

    // choice the vi uvc fmt
    switch (ctx->mUvcCtx.fcc) {
    case V4L2_PIX_FMT_YUYV:
      if (ctx->mUvcCfg.common_cfg.uvc_enable_vpss)
        ctx->mFmtType = UVC_FMT_TYPE_VI_VPSS_UVC;
      else
        ctx->mFmtType = UVC_FMT_TYPE_VI_UVC;
      ctx->mUvcBuffCount =
          ctx->mUvcCfg.vi_cfg[MPI_VI_CHANNEL_TYPE_UVC].buf_cnt +
          ctx->mUvcCfg.vpss_cfg[MPI_VPSS_CHANNEL_TYPE_UVC].buf_cnt;
      if (ctx->mUvcCfg.common_cfg.yuyv_debug)
        ctx->mFmtType = UVC_FMT_TYPE_DEBUG_YUYV_UVC;
      // tmp for workaround vpss stuck
      ctx->mUvcBuffCount = 1;

      break;
    case V4L2_PIX_FMT_NV12: // reserver
      if (ctx->mUvcCfg.common_cfg.uvc_enable_vpss)
        ctx->mFmtType = UVC_FMT_TYPE_VI_VPSS_UVC;
      else
        ctx->mFmtType = UVC_FMT_TYPE_VI_UVC;
      ctx->mUvcBuffCount =
          ctx->mUvcCfg.vi_cfg[MPI_VI_CHANNEL_TYPE_UVC].buf_cnt +
          ctx->mUvcCfg.vpss_cfg[MPI_VPSS_CHANNEL_TYPE_UVC].buf_cnt;
      break;
    case V4L2_PIX_FMT_MJPEG:
      if (ctx->mUvcCfg.common_cfg.uvc_enable_vpss) {
          ctx->mFmtType = UVC_FMT_TYPE_VI_VPSS_VENC_UVC;
          ctx->mUvcBuffCount =
              ctx->mUvcCfg.venc_cfg.mjpeg_cfg.buf_cnt +
              ctx->mUvcCfg.vpss_cfg[MPI_VPSS_CHANNEL_TYPE_UVC].buf_cnt;
      } else {
          ctx->mFmtType = UVC_FMT_TYPE_VI_VENC_UVC;
          ctx->mUvcBuffCount = ctx->mUvcCfg.venc_cfg.mjpeg_cfg.buf_cnt;
      }
      ctx->vencTempInfo.force_idr_cnt = 0;
      break;
    case V4L2_PIX_FMT_H264:
      if (ctx->mUvcCfg.common_cfg.uvc_enable_vpss) {
        ctx->mFmtType = UVC_FMT_TYPE_VI_VPSS_VENC_UVC;
        ctx->mUvcBuffCount =
            ctx->mUvcCfg.venc_cfg.h264_cfg.buf_cnt +
            ctx->mUvcCfg.vpss_cfg[MPI_VPSS_CHANNEL_TYPE_UVC].buf_cnt;
      } else {
        ctx->mFmtType = UVC_FMT_TYPE_VI_VENC_UVC;
        ctx->mUvcBuffCount = ctx->mUvcCfg.venc_cfg.h264_cfg.buf_cnt;
      }

      ctx->vencTempInfo.force_idr_cnt =
          ctx->mUvcCfg.venc_cfg.common_cfg.idr_gop *
          ctx->mUvcCfg.venc_cfg.common_cfg.idr_cnt;

      break;
    case V4L2_PIX_FMT_H265:
      if (ctx->mUvcCfg.common_cfg.uvc_enable_vpss) {
        ctx->mFmtType = UVC_FMT_TYPE_VI_VPSS_VENC_UVC;
        ctx->mUvcBuffCount =
            ctx->mUvcCfg.venc_cfg.h265_cfg.buf_cnt +
            ctx->mUvcCfg.vpss_cfg[MPI_VPSS_CHANNEL_TYPE_UVC].buf_cnt;
      } else {
        ctx->mFmtType = UVC_FMT_TYPE_VI_VENC_UVC;
        ctx->mUvcBuffCount = ctx->mUvcCfg.venc_cfg.h265_cfg.buf_cnt;
      }

      ctx->vencTempInfo.force_idr_cnt =
          ctx->mUvcCfg.venc_cfg.common_cfg.idr_gop *
          ctx->mUvcCfg.venc_cfg.common_cfg.idr_cnt;
      break;
    default:
      LOG_ERROR("process not support this fcc 0x%x\n", ctx->mUvcCtx.fcc);
      break;
    }

    switch (ctx->mFmtType) {
    case UVC_FMT_TYPE_DEBUG_YUYV_UVC:
      LOG_INFO("uvc debug\n");
      ctx->mUvcBuffCount = 3;
      break;
    case UVC_FMT_TYPE_VI_UVC:
      configVi();
      break;
    case UVC_FMT_TYPE_VI_VPSS_UVC:
      configVi();
      configVpss();
      break;
    case UVC_FMT_TYPE_VI_VENC_UVC:
      configVi();
      configVenc();
      break;
    case UVC_FMT_TYPE_VI_VPSS_VENC_UVC:
      configVi();
      configVpss();
      configVenc();
      break;
    default:
      LOG_ERROR("not support this type:%d\n", ctx->mFmtType);
      break;
    }
  }

  return ctx->mUvcBuffCount;
}

int UVCProcess::configCropScale(RECT_S src, RECT_S dst) {
  UVCProcessCtx *ctx = getUVCProcessCtx(mCtx);
  VPSS_CROP_INFO_S stCropInfo;
  memset(&stCropInfo, 0, sizeof(VPSS_CROP_INFO_S));

  if (memcmp(&src, &dst, sizeof(RECT_S))) {
    stCropInfo.bEnable = RK_TRUE;
    stCropInfo.enCropCoordinate = VPSS_CROP_ABS_COOR;
    stCropInfo.stCropRect = dst;
  } else { // workaround
    // stCropInfo.bEnable = RK_FALSE;
    stCropInfo.bEnable = RK_TRUE;
    stCropInfo.enCropCoordinate = VPSS_CROP_ABS_COOR;
    stCropInfo.stCropRect = dst;
  }
  uvc_vpss_config_crop(ctx->mUvcCfg, stCropInfo, MPI_VPSS_CHANNEL_TYPE_UVC);

  return 0;
}

int UVCProcess::resetCtx() {
  RK_S32 s32Ret;
  UVCProcessCtx *ctx = getUVCProcessCtx(mCtx);

  if (ctx->mBuff) {
    free(ctx->mBuff);
    ctx->mBuff = nullptr;
  }
  if (ctx->mUvcBuf)
    ctx->mUvcBuf = nullptr;
  if (ctx->mUvcDebug.pDebugFile)
    fclose(ctx->mUvcDebug.pDebugFile);
  memset(&ctx->mUvcDebug, 0, sizeof(UVCDegbuFileInfo));
  memset(&ctx->mUvcCtx, 0, sizeof(UVC_CTRL_INFO));
  ctx->vencTempInfo.recvCnt = 0;
  ctx->getBufCnt = 0;
  ctx->releaseBufCnt = 0;

  return 0;
}

int UVCProcess::startProcess() {
  RK_S32 s32Ret;
  UVCProcessCtx *ctx = getUVCProcessCtx(mCtx);
  ctx->mBuff =
      (UVCProcessBuff *)calloc(sizeof(UVCProcessBuff), ctx->mUvcBuffCount);

  switch (ctx->mFmtType) {
  case UVC_FMT_TYPE_DEBUG_YUYV_UVC:
    MB_POOL_CONFIG_S stMbPoolCfg;
    memset(&stMbPoolCfg, 0, sizeof(MB_POOL_CONFIG_S));
    stMbPoolCfg.u64MBSize =
        ctx->mUvcCfg.uvc_cfg.width * ctx->mUvcCfg.uvc_cfg.height * 2;
    stMbPoolCfg.u32MBCnt = 3;
    stMbPoolCfg.enAllocType = MB_ALLOC_TYPE_DMA;
    stMbPoolCfg.bPreAlloc = RK_TRUE;
    ctx->debugPool = RK_MPI_MB_CreatePool(&stMbPoolCfg);
    break;
  case UVC_FMT_TYPE_VI_UVC:
    startVi();
    break;
  case UVC_FMT_TYPE_VI_VPSS_UVC:
    startVi();
    startVpss();
    bindViVpss();
    break;
  case UVC_FMT_TYPE_VI_VENC_UVC:
    startVi();
    startVenc();
#ifndef RK_ENABLE_FASTBOOT
    bindViVenc();
#endif
    break;
  case UVC_FMT_TYPE_VI_VPSS_VENC_UVC:
    startVi();
    startVpss();
    startVenc();
    bindViVpss();
    bindVpssVenc();
    break;
  default:
    LOG_ERROR("not support this type:%d\n", ctx->mFmtType);
    break;
  }

  ctx->mStart = true;
  // if use one thread do uvc. do not call this thread
  ctx->mThread = new std::thread(threadLoop, reinterpret_cast<void *>(this));

  if (ctx->mUvcCfg.osd_cfg.enable)
    osd_start(&ctx->mUvcCfg);

  return 0;
}

int UVCProcess::stopProcess() {
  UVCProcessCtx *ctx = getUVCProcessCtx(mCtx);

  ctx->mStart = false;
  if (ctx->mThread) {
    ctx->mThread->join();
    delete ctx->mThread;
    ctx->mThread = nullptr;
  }

  if (ctx->mUvcCfg.osd_cfg.enable)
    osd_stop(&ctx->mUvcCfg);

  uvc_buffer_deinit(ctx->mUvcCtx.video_id);
  switch (ctx->mFmtType) {
  case UVC_FMT_TYPE_DEBUG_YUYV_UVC:
    RK_MPI_MB_DestroyPool(ctx->debugPool);
    break;
  case UVC_FMT_TYPE_VI_UVC:
    stopVi();
    break;
  case UVC_FMT_TYPE_VI_VENC_UVC:
#ifndef RK_ENABLE_FASTBOOT
    unBindViVenc();
    stopVenc();
    stopVi();
#endif
    break;
  case UVC_FMT_TYPE_VI_VPSS_UVC:
    unBindViVpss();
    stopVpss();
    stopVi();
    break;
  case UVC_FMT_TYPE_VI_VPSS_VENC_UVC:
    unBindViVpss();
    unBindVpssVenc();
    stopVenc();
    stopVpss();
    stopVi();
    break;
  default:
    LOG_ERROR("not support this type:%d\n", ctx->mFmtType);
    break;
  }
  resetCtx();

  return 0;
}

int UVCProcess::releaseFrame(void *frame) {
  UVCProcessCtx *ctx = getUVCProcessCtx(mCtx);
  _uvc_process_release_frame(ctx, frame);

  return 0;
}

// reserver for one thread
int UVCProcess::doProcess() {
  RK_S32 s32Ret;
  UVCProcessCtx *ctx = getUVCProcessCtx(mCtx);
  int ret;

  ret = do_process(ctx);

  return ret;
}

UVC_RET_TYPE queue_uvc_buf(UVCProcessCtx *ctx) {
  if (!ctx->mUvcBuf) {
    ctx->mUvcBuf = uvc_buffer_write_get(ctx->mUvcCtx.video_id);
    LOG_DEBUG("uvc_buffer_write_get (buf: %p)\n", ctx->mUvcBuf);
  }

  return UVC_RET_OK;
}

UVC_RET_TYPE dequeue_uvc_buf(UVCProcessCtx *ctx, void *frame) {
  VIDEO_FRAME_INFO_S *videoFrame;
  VENC_STREAM_S *vencFrame;
  int idx = 0;

  if (ctx->mUvcBuf) {
    struct uvc_buffer uvc_buf;
    memcpy(&uvc_buf, ctx->mUvcBuf, sizeof(struct uvc_buffer));

    switch (ctx->mFmtType) {
    case UVC_FMT_TYPE_DEBUG_YUYV_UVC:
    case UVC_FMT_TYPE_VI_UVC:
    case UVC_FMT_TYPE_VI_VPSS_UVC:
      videoFrame = reinterpret_cast<VIDEO_FRAME_INFO_S *>(frame);
      uvc_buf.frame = reinterpret_cast<void *>(videoFrame->stVFrame.pMbBlk);
      uvc_buf.buffer = RK_MPI_MB_Handle2VirAddr(
          videoFrame->stVFrame.pMbBlk); // for debug file
      uvc_buf.fd = RK_MPI_MB_Handle2Fd(videoFrame->stVFrame.pMbBlk);
      uvc_buf.seq = videoFrame->stVFrame.u32TimeRef;
      uvc_buf.pts = videoFrame->stVFrame.u64PTS;
      uvc_buf.size = ctx->mUvcBuf->total_size; // tmp
      uvc_buf.offset = 0;
      // uvc_buf->size = RK_MPI_MB_GetLength(videoFrame->stVFrame.pMbBlk);  //
      // need update rockit
      RK_MPI_SYS_MmzFlushCache(videoFrame->stVFrame.pMbBlk, RK_TRUE);
      // LOG_INFO("uvc_buf->size:%d\n", uvc_buf->size);
      break;
    case UVC_FMT_TYPE_VI_VENC_UVC:
    case UVC_FMT_TYPE_VI_VPSS_VENC_UVC:
      vencFrame = reinterpret_cast<VENC_STREAM_S *>(frame);
      uvc_buf.frame = reinterpret_cast<void *>(vencFrame->pstPack->pMbBlk);
      uvc_buf.buffer = RK_MPI_MB_Handle2VirAddr(
          vencFrame->pstPack->pMbBlk); // for debug file
      uvc_buf.fd = RK_MPI_MB_Handle2Fd(vencFrame->pstPack->pMbBlk);
      uvc_buf.seq = vencFrame->u32Seq;
      uvc_buf.pts = vencFrame->pstPack->u64PTS;
      uvc_buf.size = vencFrame->pstPack->u32Len;
      uvc_buf.offset = vencFrame->pstPack->u32Offset;
      if (vencFrame->pstPack->u32Offset) {
        LOG_DEBUG("u32Offset:%d\n", vencFrame->pstPack->u32Offset);
      }
      break;
    default:
      LOG_ERROR("not support this fmt:%d\n", ctx->mFmtType);
      break;
    }

    // for debug file
    if (ctx->mUvcCfg.common_cfg.uvc_debug) {
      if (ctx->mUvcDebug.pDebugFile && ctx->mUvcCfg.common_cfg.uvc_debug_cnt) {
        if (ctx->mFmtType != UVC_FMT_TYPE_VI_VENC_UVC &&
            ctx->mFmtType !=
                UVC_FMT_TYPE_VI_VPSS_VENC_UVC) // venc will flush cache
          RK_MPI_SYS_MmzFlushCache((MB_BLK)uvc_buf.frame, RK_TRUE);
        fwrite(uvc_buf.buffer, 1, uvc_buf.size, ctx->mUvcDebug.pDebugFile);
        ctx->mUvcDebug.debugCnt++;
        if (ctx->mUvcDebug.debugCnt >= ctx->mUvcCfg.common_cfg.uvc_debug_cnt) {
          LOG_INFO("debugCnt:%d, close debug file\n", ctx->mUvcDebug.debugCnt);
          fflush(ctx->mUvcDebug.pDebugFile);
          fclose(ctx->mUvcDebug.pDebugFile);
          ctx->mUvcDebug.pDebugFile = NULL;
          ctx->mUvcDebug.debugCnt = -1;
        }
      } else if (!access(UVC_DYNAMIC_DEBUG_OUT_CHECK, 0) ||
                 (ctx->mUvcDebug.debugCnt == 0 &&
                  ctx->mUvcCfg.common_cfg.uvc_debug_cnt)) {
        ctx->mUvcDebug.pDebugFile =
            fopen(ctx->mUvcCfg.common_cfg.uvc_debug_file, "w+b");
        if (ctx->mUvcDebug.pDebugFile) {
          LOG_INFO("debug out file open\n");
          if (ctx->mFmtType != UVC_FMT_TYPE_VI_VENC_UVC &&
              ctx->mFmtType !=
                  UVC_FMT_TYPE_VI_VPSS_VENC_UVC) // venc will flush cache
            RK_MPI_SYS_MmzFlushCache((MB_BLK)uvc_buf.frame, RK_TRUE);
          fwrite(uvc_buf.buffer, 1, uvc_buf.size, ctx->mUvcDebug.pDebugFile);
          ctx->mUvcDebug.debugCnt = 1;
          if (ctx->mUvcDebug.debugCnt >=
              ctx->mUvcCfg.common_cfg.uvc_debug_cnt) {
            LOG_INFO("debugCnt:%d, close debug file\n",
                     ctx->mUvcDebug.debugCnt);
            fflush(ctx->mUvcDebug.pDebugFile);
            fclose(ctx->mUvcDebug.pDebugFile);
            ctx->mUvcDebug.pDebugFile = NULL;
            ctx->mUvcDebug.debugCnt = -1;
          }
        }
        if (!access(UVC_DYNAMIC_DEBUG_OUT_CHECK, 0)) {
          char cmd[30];
          sprintf(cmd, "rm %s", UVC_DYNAMIC_DEBUG_OUT_CHECK);
          system(cmd);
        }
      }
    }

    for (int i = 0; i < ctx->mUvcBuffCount; i++) {
      if (uvc_buf.fd == ctx->mBuff[i].fd &&
          uvc_buf.frame == ctx->mBuff[i].frame) {
        idx = i;
        break;
      } else if (ctx->mBuff[i].fd) {
        idx++;
      }
    }
    if (idx == ctx->mUvcBuffCount) {
      LOG_DEBUG("exceed max uvc buf:%d, abdon mb:%p\n", ctx->mUvcBuffCount,
                uvc_buf.frame);
      _uvc_process_release_frame(ctx, uvc_buf.frame);
    } else {
      uvc_buf.index = idx;
      ctx->mBuff[idx].fd = uvc_buf.fd;
      ctx->mBuff[idx].frame = uvc_buf.frame;
      memcpy(ctx->mUvcBuf, &uvc_buf, sizeof(struct uvc_buffer));
      uvc_buffer_read_set(ctx->mUvcCtx.video_id, ctx->mUvcBuf);
      LOG_DEBUG("debug info!idx:%d frame mb:%p uvc_buf->fd:%d size=%d "
                "uvc_buf->seq:%d \n",
                idx, uvc_buf.frame, uvc_buf.fd, uvc_buf.size, uvc_buf.seq);
      ctx->mUvcBuf = NULL;
      if (ctx->mUvcCfg.common_cfg.uvc_debug &&
          !access(UVC_DYNAMIC_DEBUG_FPS, 0)) {
        ctx->sendCount++;
        if (ctx->sendCount % 100 == 0) {
          struct timeval now_time;
          gettimeofday(&now_time, NULL);
          int64_t use_timems =
              (now_time.tv_sec * 1000 + now_time.tv_usec / 1000) -
              (ctx->enterTime.tv_sec * 1000 + ctx->enterTime.tv_usec / 1000);
          ctx->enterTime.tv_sec = now_time.tv_sec;
          ctx->enterTime.tv_usec = now_time.tv_usec;
          LOG_INFO("uvc process fps:%0.1f\n", (1000.0 * 100) / use_timems);
        }
      }
    }
    ctx->getBufCnt++;
    LOG_DEBUG("get buff cnt:%d\n", ctx->getBufCnt);
  }

  return UVC_RET_OK;
}

static RK_S32 uvc_out_yuyv_debug(UVCProcessCtx *ctx,
                                 VIDEO_FRAME_INFO_S *videoFrame) {
  static int debugCnt;
  MB_BLK blk;
  RK_U8 *p;
  int width, height, x, y;
  width = ctx->mUvcCfg.uvc_cfg.width;
  height = ctx->mUvcCfg.uvc_cfg.height;
  blk = RK_MPI_MB_GetMB(ctx->debugPool, width * height * 2, RK_FALSE);
  if (!blk) {
    usleep(10 * 1000);
    return RK_FAILURE;
  }
  p = (RK_U8 *)RK_MPI_MB_Handle2VirAddr(blk);

  debugCnt++;
  for (y = 0; y < height; y++, p += width * 2) {
    for (x = 0; x < width / 2; x++) {
      p[x * 4 + 0] = x * 2 + 0 + y + debugCnt * 3;
      p[x * 4 + 2] = x * 2 + 1 + y + debugCnt * 3;
      p[x * 4 + 1] = 128 + y / 2 + debugCnt * 2;
      p[x * 4 + 3] = 64 + x + debugCnt * 5;
    }
  }

  RK_MPI_SYS_MmzFlushCache(blk, RK_TRUE);
  videoFrame->stVFrame.pMbBlk = blk;

  return RK_SUCCESS;
}

static UVC_RET_TYPE uvc_out_yuv_process(UVCProcessCtx *ctx) {
  VIDEO_FRAME_INFO_S videoFrame;
  RK_S32 s32Ret;
  UVC_RET_TYPE ret = UVC_RET_WAT;

  queue_uvc_buf(ctx);
  if (!ctx->mUvcBuf) {
    LOG_DEBUG("uvc_buffer_write_get null buff\n");
    usleep(2 * 1000);
  } else {
    if (ctx->mFmtType == UVC_FMT_TYPE_VI_UVC) 
      s32Ret = RK_MPI_VI_GetChnFrame(ctx->viChnCtx.pipeId,
                                     ctx->viChnCtx.channelId, &videoFrame, 33);
    else if (ctx->mFmtType == UVC_FMT_TYPE_VI_VPSS_UVC)
      s32Ret = RK_MPI_VPSS_GetChnFrame(
          ctx->vpssChnCtx.group, ctx->vpssChnCtx.channelId, &videoFrame, 33);
    else if (ctx->mFmtType == UVC_FMT_TYPE_DEBUG_YUYV_UVC)
      s32Ret = uvc_out_yuyv_debug(ctx, &videoFrame);
    else
      ret = UVC_RET_ERR;

    if (s32Ret == RK_SUCCESS) {
      ret = dequeue_uvc_buf(ctx, (void *)&videoFrame);
    } else {
      LOG_DEBUG("get yuv timeout:0x%x\n", s32Ret);
      // usleep(5*1000);   // wait
    }
  }

  return ret;
}

static UVC_RET_TYPE uvc_out_venc_process(UVCProcessCtx *ctx) {
  RK_S32 s32Ret;
  UVC_RET_TYPE ret = UVC_RET_WAT;

  queue_uvc_buf(ctx);
  // LOG_INFO("test lqh\n");
  if (!ctx->mUvcBuf) {
    LOG_DEBUG("uvc_buffer_write_get null buff\n");
    usleep(2 * 1000);
  } else {
    s32Ret = RK_MPI_VENC_GetStream(ctx->vencChnCtx.channelId,
                                   &ctx->vencTempInfo.stVencFrame, 33);
    if (s32Ret == RK_SUCCESS) {
      ret = dequeue_uvc_buf(ctx, (void *)&ctx->vencTempInfo.stVencFrame);
      ctx->vencTempInfo.recvCnt++;
    } else if (s32Ret == RK_ERR_VENC_NOT_CONFIG) {
      LOG_ERROR("venc chn:%d not config\n", ctx->vencChnCtx.channelId);
      ret = UVC_RET_ERR;
    } else {
      LOG_DEBUG("get venc timeout:0x%x\n", s32Ret);
      // usleep(5*1000);   // wait
    }
  }

  return ret;
}

// uvc_video can call this func direct to reduce thread use.
static UVC_RET_TYPE do_process(UVCProcessCtx *ctx) {
  UVC_RET_TYPE ret = UVC_RET_OK;

  switch (ctx->mFmtType) {
  case UVC_FMT_TYPE_DEBUG_YUYV_UVC:
  case UVC_FMT_TYPE_VI_UVC:
  case UVC_FMT_TYPE_VI_VPSS_UVC:
    ret = uvc_out_yuv_process(ctx);
    break;
  case UVC_FMT_TYPE_VI_VENC_UVC:
  case UVC_FMT_TYPE_VI_VPSS_VENC_UVC:
    ret = uvc_out_venc_process(ctx);
    break;
  default:
    LOG_ERROR("not support this type:%d\n", ctx->mFmtType);
    ret = UVC_RET_ERR;
    break;
  }
#if 0 // reduce thread do this
  if (ctx->mUvcCfg.common_cfg.nn_enable) {
    int32_t use_time_us, now_time_us, last_time_us;
    struct timespec now_tm = {0, 0};
    clock_gettime(CLOCK_MONOTONIC, &now_tm);
    last_time_us = now_tm.tv_sec * 1000000LL + now_tm.tv_nsec / 1000; // us
    ctx->nnCtx->doProcess();
    clock_gettime(CLOCK_MONOTONIC, &now_tm);
    now_time_us = now_tm.tv_sec * 1000000LL + now_tm.tv_nsec / 1000; // us
    use_time_us = now_time_us - last_time_us;
    LOG_INFO("nn process use time:%d us\n", use_time_us);
  }
#endif

  return ret;
}

void *UVCProcess::threadLoop(void *arg) {
  prctl(PR_SET_NAME, "uvcProcessThread");
  UVCProcess *uvcProcess = reinterpret_cast<UVCProcess *>(arg);
  UVCProcessCtx *ctx = getUVCProcessCtx(uvcProcess->getCtx());
  UVC_RET_TYPE ret = UVC_RET_OK;

  // wait uvc streamon
  while (ctx->mStart && (!uvc_get_user_run_state(ctx->mUvcCtx.video_id) ||
                         !uvc_buffer_write_enable(ctx->mUvcCtx.video_id))) {
    usleep(10 * 1000);
  }

  // when streamon start
  while (ctx->mStart) {
    ret = do_process(ctx);
    if (ret == UVC_RET_ERR)
      break;
  }
  LOG_INFO("thread type:%d exit ret:%d\n", ctx->mFmtType, ret);

  return NULL;
}
