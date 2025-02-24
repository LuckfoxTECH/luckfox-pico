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
#include "uvc_mpi_vi.h"
#include "rk_mpi_sys.h"
#include "rk_mpi_vi.h"
#include "uvc_log.h"
#include "uvc_video.h"
#include <cstring>

typedef struct _rkMpiVICtx {
  UVC_VI_CHN_CTX_S stChnCtx;
  COMPRESS_MODE_E enCompressMode;
  VI_DEV_ATTR_S stDevAttr;
  VI_DEV_BIND_PIPE_S stBindPipe;
  VI_CHN_ATTR_S stChnAttr;
  VI_CHN_STATUS_S stChnStatus;
} MPI_VI_CTX_S;

UVC_VI_CHN_CTX_S uvc_get_vi_chn_ctx(UVC_MPI_CFG uvcCfg,
                                    MpiViChannelType chnType) {
  UVC_VI_CHN_CTX_S viChnCtx;
  memset(&viChnCtx, 0, sizeof(UVC_VI_CHN_CTX_S));

  viChnCtx.devId = uvcCfg.vi_cfg[chnType].dev_id;
  viChnCtx.pipeId = viChnCtx.devId;
  viChnCtx.channelId = uvcCfg.vi_cfg[chnType].channel_id;

  return viChnCtx;
}

static MPI_VI_CTX_S uvc_get_vi_ctx(UVC_MPI_CFG *uvcCfg,
                                   MpiViChannelType chnType) {
  MPI_VI_CTX_S viCtx;
  memset(&viCtx, 0, sizeof(MPI_VI_CTX_S));

  LOG_INFO("chnType:%d uvc out:%dx%d, vi assign_resolution:%dx%d vi_name:%s\n",
           chnType, uvcCfg->uvc_cfg.width, uvcCfg->uvc_cfg.height,
           uvcCfg->vi_cfg[chnType].assign_width,
           uvcCfg->vi_cfg[chnType].assign_height,
           uvcCfg->vi_cfg[chnType].dev_name);

  //  this is for uvc vi/nn vi different cfg.
  switch (chnType) {
  case MPI_VI_CHANNEL_TYPE_UVC:
    if (uvcCfg->vi_cfg[chnType].assign_width <= 0 ||
        uvcCfg->vi_cfg[chnType].assign_height <=
            0) { // not assign the vi uvc output
      if (uvcCfg->uvc_cfg.width < uvcCfg->vi_cfg[chnType].min_width ||
          uvcCfg->uvc_cfg.height <
              uvcCfg->vi_cfg[chnType]
                  .min_height) { // not support this resolution(too small)
        uvcCfg->vi_cfg[chnType].assign_width =
            uvcCfg->vi_cfg[chnType].min_width;
        uvcCfg->vi_cfg[chnType].assign_height =
            uvcCfg->vi_cfg[chnType].min_height;
        LOG_INFO("uvc out:%dx%d, the min resolution:%dx%d\n",
                 uvcCfg->uvc_cfg.width, uvcCfg->uvc_cfg.height,
                 uvcCfg->vi_cfg[chnType].min_width,
                 uvcCfg->vi_cfg[chnType].min_height);
      } else if (uvcCfg->uvc_cfg.width > uvcCfg->vi_cfg[chnType].max_width ||
                 uvcCfg->uvc_cfg.height >
                     uvcCfg->vi_cfg[chnType].max_height) { // not support this
                                                           // resolution(too
                                                           // large)
        uvcCfg->vi_cfg[chnType].assign_width =
            uvcCfg->vi_cfg[chnType].max_width;
        uvcCfg->vi_cfg[chnType].assign_height =
            uvcCfg->vi_cfg[chnType].max_height;
        LOG_INFO("uvc out:%dx%d, the max resolution:%dx%d\n",
                 uvcCfg->uvc_cfg.width, uvcCfg->uvc_cfg.height,
                 uvcCfg->vi_cfg[chnType].max_width,
                 uvcCfg->vi_cfg[chnType].max_height);
      } else {
        uvcCfg->vi_cfg[chnType].assign_width = uvcCfg->uvc_cfg.width;
        uvcCfg->vi_cfg[chnType].assign_height = uvcCfg->uvc_cfg.height;
      }
    }

    switch (uvcCfg->uvc_cfg.fcc) {
    case V4L2_PIX_FMT_YUYV:
    case V4L2_PIX_FMT_NV12: // reserver
#if 0
      if (uvcCfg->uvc_cfg.width == 1280) {
        LOG_ERROR("vi workaround and here alloc 1 buf count!!!\n");
        uvcCfg->vi_cfg[chnType].buf_cnt =
            1; // workaround for 1280*720 vpss stuck
      }
#endif
      viCtx.stChnAttr.enPixelFormat =
          uvcCfg->vi_cfg[chnType].format; // RK_FMT_YUV422_YUYV;
      viCtx.stChnAttr.u32Depth = 0; // viCtx.stChnAttr.stIspOpt.u32BufCount;  //
                                    // bind 0; not bind u32BufCount;
      break;
    case V4L2_PIX_FMT_MJPEG:
    case V4L2_PIX_FMT_H264:
    case V4L2_PIX_FMT_H265:
      viCtx.stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
      viCtx.stChnAttr.u32Depth = 0; // bind 0; not bind u32BufCount;
      break;
    default:
      LOG_ERROR("vi not support this fcc 0x%x\n", uvcCfg->uvc_cfg.fcc);
      break;
    }
    break;
  case MPI_VI_CHANNEL_TYPE_NN:
    viCtx.stChnAttr.enPixelFormat = uvcCfg->vi_cfg[chnType].format;
    viCtx.stChnAttr.u32Depth = 0;
    break;
  default:
    LOG_ERROR("vi not support this chnType 0x%x\n", chnType);
    break;
  }

  // common cfg
  viCtx.stChnCtx = uvc_get_vi_chn_ctx(*uvcCfg, chnType);
  viCtx.stChnAttr.stIspOpt.bNoUseLibV4L2 = RK_FALSE;
  viCtx.stChnAttr.stIspOpt.u32BufCount = uvcCfg->vi_cfg[chnType].buf_cnt;
  viCtx.stChnAttr.stIspOpt.u32BufSize =
      0; // uvcCfg.vi_cfg[chnType].width * uvcCfg.vi_cfg[chnType].height * 2;
         // uvcCfg.uvc_cfg.width * uvcCfg.uvc_cfg.height;
  viCtx.stChnAttr.stIspOpt.enMemoryType = uvcCfg->vi_cfg[chnType].memory_type;
  if (strncmp(uvcCfg->vi_cfg[chnType].dev_name, "null", strlen("null"))) {
    memcpy(viCtx.stChnAttr.stIspOpt.aEntityName,
           uvcCfg->vi_cfg[chnType].dev_name,
           strlen(uvcCfg->vi_cfg[chnType].dev_name));
  }
  viCtx.stChnAttr.stFrameRate.s32SrcFrameRate =
      uvcCfg->vi_cfg[chnType].fps_in > 0 ? uvcCfg->vi_cfg[chnType].fps_in : -1;
  viCtx.stChnAttr.stFrameRate.s32DstFrameRate =
      uvcCfg->vi_cfg[chnType].fps_out > 0 ? uvcCfg->vi_cfg[chnType].fps_out
                                          : -1;
  viCtx.stChnAttr.stSize.u32Width = uvcCfg->vi_cfg[chnType].assign_width;
  viCtx.stChnAttr.stSize.u32Height = uvcCfg->vi_cfg[chnType].assign_height;
  LOG_INFO("chnType:%d uvc out:%dx%d, vi resolution:%dx%d name:%s\n", chnType,
           uvcCfg->uvc_cfg.width, uvcCfg->uvc_cfg.height,
           viCtx.stChnAttr.stSize.u32Width, viCtx.stChnAttr.stSize.u32Height,
           viCtx.stChnAttr.stIspOpt.aEntityName);

  return viCtx;
}

// todo: multi vi from cfg.
RK_S32 uvc_vi_config(UVC_MPI_CFG *uvcCfg, MpiViChannelType chnType) {
  RK_S32 s32Ret = RK_FAILURE;
  MPI_VI_CTX_S ctx;
  ctx = uvc_get_vi_ctx(uvcCfg, chnType);

  // 0. get dev config status
  s32Ret = RK_MPI_VI_GetDevAttr(ctx.stChnCtx.devId, &ctx.stDevAttr);
  if (s32Ret == RK_ERR_VI_NOT_CONFIG) {
    // 0-1.config dev
    s32Ret = RK_MPI_VI_SetDevAttr(ctx.stChnCtx.devId, &ctx.stDevAttr);
    if (s32Ret != RK_SUCCESS) {
      LOG_ERROR("RK_MPI_VI_SetDevAttr %x\n", s32Ret);
      goto __FAILED;
    }
  }
  // 1.get  dev enable status
  s32Ret = RK_MPI_VI_GetDevIsEnable(ctx.stChnCtx.devId);
  if (s32Ret != RK_SUCCESS) {
    // 1-2.enable dev
    s32Ret = RK_MPI_VI_EnableDev(ctx.stChnCtx.devId);
    if (s32Ret != RK_SUCCESS) {
      LOG_ERROR("RK_MPI_VI_EnableDev %x\n", s32Ret);
      goto __FAILED;
    }
    // 1-3.bind dev/pipe
    ctx.stBindPipe.u32Num = ctx.stChnCtx.pipeId;
    ctx.stBindPipe.PipeId[0] = ctx.stChnCtx.pipeId;
    s32Ret = RK_MPI_VI_SetDevBindPipe(ctx.stChnCtx.devId, &ctx.stBindPipe);
    if (s32Ret != RK_SUCCESS) {
      LOG_ERROR("RK_MPI_VI_SetDevBindPipe %x\n", s32Ret);
      goto __FAILED;
    }
  }
  // 2.config channel
  s32Ret = RK_MPI_VI_SetChnAttr(ctx.stChnCtx.pipeId, ctx.stChnCtx.channelId,
                                &ctx.stChnAttr);
  if (s32Ret != RK_SUCCESS) {
    LOG_ERROR("RK_MPI_VI_SetChnAttr %x\n", s32Ret);
    goto __FAILED;
  }

__FAILED:
  return s32Ret;
}

RK_S32 uvc_vi_start(UVC_MPI_CFG uvcCfg, MpiViChannelType chnType) {
  RK_S32 s32Ret = RK_FAILURE;
  UVC_VI_CHN_CTX_S ctx;
  ctx = uvc_get_vi_chn_ctx(uvcCfg, chnType);

  // isp&aiq init here

  // 3.enable channel
  LOG_INFO("RK_MPI_VI_EnableChn %x %d %d\n", ctx.devId, ctx.pipeId,
           ctx.channelId);
  s32Ret = RK_MPI_VI_EnableChn(ctx.pipeId, ctx.channelId);
  if (s32Ret != RK_SUCCESS) {
    LOG_ERROR("RK_MPI_VI_EnableChn %x\n", s32Ret);
    goto __FAILED;
  }

__FAILED:
  return s32Ret;
}

RK_S32 uvc_vi_stop(UVC_MPI_CFG uvcCfg, MpiViChannelType chnType) {
  RK_S32 s32Ret = RK_FAILURE;
  UVC_VI_CHN_CTX_S ctx;
  ctx = uvc_get_vi_chn_ctx(uvcCfg, chnType);

  // 5. disable one chn
  s32Ret = RK_MPI_VI_DisableChn(ctx.pipeId, ctx.channelId);
  if (s32Ret != RK_SUCCESS) {
    LOG_ERROR("RK_MPI_VI_DisableChn %x\n", s32Ret);
    goto __FAILED;
  }
  // 6.disable dev(will diabled all chn)
  s32Ret = RK_MPI_VI_DisableDev(ctx.devId);
  if (s32Ret != RK_SUCCESS) {
    LOG_ERROR("RK_MPI_VI_DisableDev %x\n", s32Ret);
    goto __FAILED;
  }

// isp&aiq deinit here

__FAILED:
  return s32Ret;
}
