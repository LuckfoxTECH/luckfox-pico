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
#include "uvc_mpi_vpss.h"
#include "rk_mpi_sys.h"
#include "rk_mpi_vpss.h"
#include "uvc_log.h"
#include "uvc_video.h"
#include <cstring>

typedef struct _rkVpssCfg {
  RK_U32 u32VpssChnCnt;
  VPSS_GRP_ATTR_S stGrpVpssAttr;
  VPSS_CHN_ATTR_S stVpssChnAttr[VPSS_MAX_CHN_NUM];
} VPSS_CFG_S;

typedef struct _rkMpiVpssCtx {
  UVC_VPSS_CHN_CTX_S stChnCtx;
  VPSS_CFG_S stVpssCfg;
} MPI_VPSS_CTX_S;

UVC_VPSS_CHN_CTX_S uvc_get_vpss_chn_ctx(UVC_MPI_CFG uvcCfg,
                                        MpiVpssChannelType chnType) {
  UVC_VPSS_CHN_CTX_S vpssChnCtx;
  memset(&vpssChnCtx, 0, sizeof(UVC_VPSS_CHN_CTX_S));

  vpssChnCtx.group = uvcCfg.vpss_cfg[chnType].group_id;
  vpssChnCtx.channelId = uvcCfg.vpss_cfg[chnType].channel_id;

  return vpssChnCtx;
}

// todo: read cfg
static MPI_VPSS_CTX_S uvc_get_vpss_ctx(UVC_MPI_CFG uvcCfg,
                                       MpiVpssChannelType chnType) {
  MPI_VPSS_CTX_S vpssCtx;
  PIXEL_FORMAT_E outFormat = RK_FMT_YUV420SP;
  int chn = uvcCfg.vpss_cfg[chnType].channel_id;

  memset(&vpssCtx, 0, sizeof(MPI_VPSS_CTX_S));
  switch (chnType) {
  case MPI_VPSS_CHANNEL_TYPE_UVC:
    switch (uvcCfg.uvc_cfg.fcc) {
    case V4L2_PIX_FMT_YUYV:
      outFormat = RK_FMT_YUV422_YUYV;
      vpssCtx.stVpssCfg.stVpssChnAttr[chn].u32Depth =
          uvcCfg.vpss_cfg[chnType].buf_cnt;
      break;
    case V4L2_PIX_FMT_NV12:
      outFormat = RK_FMT_YUV420SP;
      vpssCtx.stVpssCfg.stVpssChnAttr[chn].u32Depth =
          uvcCfg.vpss_cfg[chnType].buf_cnt;
      break;
    case V4L2_PIX_FMT_MJPEG:
    case V4L2_PIX_FMT_H264:
    case V4L2_PIX_FMT_H265:
      outFormat = RK_FMT_YUV420SP;
      vpssCtx.stVpssCfg.stVpssChnAttr[chn].u32Depth = 0; // bind venc.
      break;
    default:
      LOG_ERROR("vpss not support this fcc 0x%x\n", uvcCfg.uvc_cfg.fcc);
      break;
    }
    vpssCtx.stVpssCfg.stVpssChnAttr[chn].u32Width = uvcCfg.uvc_cfg.width;
    vpssCtx.stVpssCfg.stVpssChnAttr[chn].u32Height = uvcCfg.uvc_cfg.height;
    break;
  case MPI_VPSS_CHANNEL_TYPE_NN:
    outFormat = uvcCfg.vpss_cfg[chnType].format;
    vpssCtx.stVpssCfg.stVpssChnAttr[chn].u32Width =
        uvcCfg.vpss_cfg[chnType].assign_width;
    vpssCtx.stVpssCfg.stVpssChnAttr[chn].u32Height =
        uvcCfg.vpss_cfg[chnType].assign_height;
    vpssCtx.stVpssCfg.stVpssChnAttr[chn].u32Depth =
        uvcCfg.vpss_cfg[chnType].buf_cnt;
    break;
  default:
    LOG_ERROR("vpss not support this type 0x%x\n", chnType);
    break;
  }

  vpssCtx.stChnCtx = uvc_get_vpss_chn_ctx(uvcCfg, chnType);
  vpssCtx.stVpssCfg.stGrpVpssAttr.u32MaxW = 4096;
  vpssCtx.stVpssCfg.stGrpVpssAttr.u32MaxH = 4096;
  vpssCtx.stVpssCfg.stGrpVpssAttr.enPixelFormat = outFormat;
  vpssCtx.stVpssCfg.stGrpVpssAttr.stFrameRate.s32SrcFrameRate =
      uvcCfg.vpss_cfg[chnType].fps_in;
  vpssCtx.stVpssCfg.stGrpVpssAttr.stFrameRate.s32DstFrameRate =
      uvcCfg.vpss_cfg[chnType].fps_out;
  vpssCtx.stVpssCfg.stGrpVpssAttr.enCompressMode = uvcCfg.vpss_cfg[chnType].fbc;
  { // chn
    vpssCtx.stVpssCfg.stVpssChnAttr[chn].enChnMode = VPSS_CHN_MODE_USER;
    vpssCtx.stVpssCfg.stVpssChnAttr[chn].enDynamicRange = DYNAMIC_RANGE_SDR8;
    vpssCtx.stVpssCfg.stVpssChnAttr[chn].enPixelFormat = outFormat;
    vpssCtx.stVpssCfg.stVpssChnAttr[chn].stFrameRate.s32SrcFrameRate = -1;
    vpssCtx.stVpssCfg.stVpssChnAttr[chn].stFrameRate.s32DstFrameRate = -1;
    vpssCtx.stVpssCfg.stVpssChnAttr[chn].enCompressMode =
        uvcCfg.vpss_cfg[chnType].fbc;
    vpssCtx.stVpssCfg.stVpssChnAttr[chn].u32FrameBufCnt =
        uvcCfg.vpss_cfg[chnType].buf_cnt;
  }

  return vpssCtx;
}

RK_S32 uvc_vpss_config(UVC_MPI_CFG *uvcCfg, MpiVpssChannelType chnType) {
  RK_S32 s32Ret = RK_FAILURE;
  MPI_VPSS_CTX_S ctx;
  ctx = uvc_get_vpss_ctx(*uvcCfg, chnType);

  s32Ret =
      RK_MPI_VPSS_CreateGrp(ctx.stChnCtx.group, &ctx.stVpssCfg.stGrpVpssAttr);
  if (s32Ret != RK_SUCCESS) {
    LOG_ERROR("RK_MPI_VPSS_CreateGrp fail 0x%x\n", s32Ret);
    return s32Ret;
  }

  s32Ret = RK_MPI_VPSS_SetChnAttr(
      ctx.stChnCtx.group, ctx.stChnCtx.channelId,
      &ctx.stVpssCfg.stVpssChnAttr[ctx.stChnCtx.channelId]);
  if (s32Ret != RK_SUCCESS) {
    LOG_ERROR("RK_MPI_VPSS_SetChnAttr fail 0x%x\n", s32Ret);
    return s32Ret;
  }

  return RK_SUCCESS;
}

// from the data.
RK_S32 uvc_vpss_config_crop(UVC_MPI_CFG uvcCfg, VPSS_CROP_INFO_S stCropInfo,
                            MpiVpssChannelType chnType) {
  RK_S32 s32Ret = RK_FAILURE;
  UVC_VPSS_CHN_CTX_S ctx;
  ctx = uvc_get_vpss_chn_ctx(uvcCfg, chnType);

  s32Ret = RK_MPI_VPSS_SetChnCrop(ctx.group, ctx.channelId, &stCropInfo);
  if (s32Ret != RK_SUCCESS) {
    LOG_ERROR("RK_MPI_VPSS_SetChnCrop fail 0x%x\n", s32Ret);
    return s32Ret;
  }

  return RK_SUCCESS;
}

RK_S32 uvc_vpss_start(UVC_MPI_CFG uvcCfg, MpiVpssChannelType chnType) {
  RK_S32 s32Ret = RK_FAILURE;
  UVC_VPSS_CHN_CTX_S ctx;
  ctx = uvc_get_vpss_chn_ctx(uvcCfg, chnType);

  s32Ret = RK_MPI_VPSS_EnableChn(ctx.group, ctx.channelId);
  if (s32Ret != RK_SUCCESS) {
    LOG_ERROR("RK_MPI_VPSS_EnableChn fail 0x%x\n", s32Ret);
    return s32Ret;
  }
  s32Ret = RK_MPI_VPSS_StartGrp(ctx.group);
  if (s32Ret != RK_SUCCESS) {
    LOG_ERROR("RK_MPI_VPSS_StartGrp fail 0x%x\n", s32Ret);
    return s32Ret;
  }

  return RK_SUCCESS;
}

RK_S32 uvc_vpss_stop(UVC_MPI_CFG uvcCfg, MpiVpssChannelType chnType) {
  RK_S32 s32Ret = RK_FAILURE;
  UVC_VPSS_CHN_CTX_S ctx;
  ctx = uvc_get_vpss_chn_ctx(uvcCfg, chnType);

  s32Ret = RK_MPI_VPSS_StopGrp(ctx.group);
  if (s32Ret != RK_SUCCESS) {
    LOG_ERROR("RK_MPI_VPSS_StopGrp fail 0x%x\n", s32Ret);
    return s32Ret;
  }

  s32Ret = RK_MPI_VPSS_DisableChn(ctx.group, ctx.channelId);
  if (s32Ret != RK_SUCCESS) {
    LOG_ERROR("RK_MPI_VPSS_DisableChn fail 0x%x\n", s32Ret);
    return s32Ret;
  }

  s32Ret = RK_MPI_VPSS_DestroyGrp(ctx.group);
  if (s32Ret != RK_SUCCESS) {
    LOG_ERROR("RK_MPI_VPSS_DestroyGrp fail 0x%x\n", s32Ret);
    return s32Ret;
  }

  return RK_SUCCESS;
}
