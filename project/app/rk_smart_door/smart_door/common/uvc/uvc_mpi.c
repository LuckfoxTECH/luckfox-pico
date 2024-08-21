// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "smart_door.h"
#include "uvc_mpi_config.h"
#include <linux/videodev2.h>
#include "uvc_mpi_vi.h"
#include "uvc_mpi_vpss.h"
#include "uvc_configfs.h"
#include "uvc_mpi_venc.h"

UVC_VI_CHN_CTX_S uvc_get_vi_chn_ctx(UVC_MPI_CFG uvcCfg,
                                    MpiViChannelType chnType) {
  UVC_VI_CHN_CTX_S viChnCtx;
  memset(&viChnCtx, 0, sizeof(UVC_VI_CHN_CTX_S));

  viChnCtx.devId = uvcCfg.vi_cfg[chnType].dev_id;
  viChnCtx.pipeId = viChnCtx.devId;
  viChnCtx.channelId = uvcCfg.vi_cfg[chnType].channel_id;

  return viChnCtx;
}

RK_S32 uvc_vi_config(UVC_MPI_CFG *uvcCfg, MpiViChannelType chnType) {
    RK_S32 s32Ret = RK_SUCCESS;

    if(!ctx) {
        ctx = (SMART_DOOR_CTX_S *)(malloc(sizeof(SMART_DOOR_CTX_S)));
        if (!ctx) {
            RK_LOGE("malloc SMART_DOOR_CTX_S memory failed");
            return -1;
        }
        memset(ctx, 0, sizeof(SMART_DOOR_CTX_S));
    } else {
        memset(&ctx->stViCfg[2], 0, sizeof(ctx->stViCfg[2]));
        memset(&ctx->stVencCfg[2], 0, sizeof(ctx->stVencCfg[2]));
    }

    ctx->stViCfg[2].devId = uvcCfg->vi_cfg[chnType].dev_id;
    ctx->stViCfg[2].pipeId = ctx->stViCfg[2].devId;
    ctx->stViCfg[2].channelId = uvcCfg->vi_cfg[chnType].channel_id;
    ctx->stViCfg[2].stChnAttr.stSize.u32Width = uvcCfg->uvc_cfg.width;
    ctx->stViCfg[2].stChnAttr.stSize.u32Height = uvcCfg->uvc_cfg.height;

    return s32Ret;
}

RK_S32 uvc_vi_start(UVC_MPI_CFG uvcCfg, MpiViChannelType chnType) {
    RK_S32 s32Ret = RK_SUCCESS;

    s32Ret = smart_door_init_vi(&ctx->stViCfg[2]);
    if (s32Ret) {
        RK_LOGE("init vi failed: %d", s32Ret);
        return s32Ret;
    }
    smart_door_create_vi(ctx->stViCfg[2]);

    return s32Ret;
}

RK_S32 uvc_vi_stop(UVC_MPI_CFG uvcCfg, MpiViChannelType chnType) {
    RK_S32 s32Ret = RK_SUCCESS;

    s32Ret = smart_door_destroy_vi(ctx->stViCfg[2]);
    if (s32Ret) {
        RK_LOGE("destroy vi failed: %d", s32Ret);
        return s32Ret;
    }
    return s32Ret;
}

UVC_VPSS_CHN_CTX_S uvc_get_vpss_chn_ctx(UVC_MPI_CFG uvcCfg,
                                        MpiVpssChannelType chnType) {
  UVC_VPSS_CHN_CTX_S vpssChnCtx;
  memset(&vpssChnCtx, 0, sizeof(UVC_VPSS_CHN_CTX_S));

  vpssChnCtx.group = uvcCfg.vpss_cfg[chnType].group_id;
  vpssChnCtx.channelId = uvcCfg.vpss_cfg[chnType].channel_id;

  return vpssChnCtx;
}

RK_S32 uvc_vpss_config(UVC_MPI_CFG *uvcCfg, MpiVpssChannelType chnType) {
    PIXEL_FORMAT_E outFormat;
    RK_U32  u32Depth;

    switch (uvcCfg->uvc_cfg.fcc) {
    case V4L2_PIX_FMT_YUYV:
      outFormat = RK_FMT_YUV422_YUYV;
      u32Depth = uvcCfg->vpss_cfg[chnType].buf_cnt;
      break;
    case V4L2_PIX_FMT_NV12:
      outFormat = RK_FMT_YUV420SP;
      u32Depth = uvcCfg->vpss_cfg[chnType].buf_cnt;
      break;
    case V4L2_PIX_FMT_MJPEG:
    case V4L2_PIX_FMT_H264:
    case V4L2_PIX_FMT_H265:
      outFormat = RK_FMT_YUV420SP;
      u32Depth = 0;
      break;
    default:
      RK_LOGE("vpss not support this fcc 0x%x\n", uvcCfg->uvc_cfg.fcc);
      break;
    }

    ctx->stVpssCfg.u32GrpId = 0;
    ctx->stVpssCfg.u32VpssChnCnt = 1;
    ctx->stVpssCfg.u32ChnId[0] = VPSS_CHN0;
    ctx->stVpssCfg.stVpssChnAttr[0].enChnMode = VPSS_CHN_MODE_USER;
    ctx->stVpssCfg.stVpssChnAttr[0].u32Width = uvcCfg->uvc_cfg.width;
    ctx->stVpssCfg.stVpssChnAttr[0].u32Height = uvcCfg->uvc_cfg.height;
    ctx->stVpssCfg.stVpssChnAttr[0].enPixelFormat = outFormat;
    ctx->stVpssCfg.stVpssChnAttr[0].u32Depth = u32Depth;
    ctx->stVpssCfg.bMirror = RK_FALSE;
    ctx->stVpssCfg.bFlip = RK_FALSE;
}

RK_S32 uvc_vpss_start(UVC_MPI_CFG uvcCfg, MpiVpssChannelType chnType) {
    RK_S32 s32Ret = RK_SUCCESS;

    s32Ret = smart_door_init_vpss(ctx);
    if (s32Ret) {
        RK_LOGE("init vpss failed: %d", s32Ret);
        return s32Ret;
    }

    s32Ret = smart_door_create_vpss(ctx->stVpssCfg);
    if (s32Ret) {
        RK_LOGE("create vpss failed: %d", s32Ret);
        return s32Ret;
    }
    return s32Ret;
}

RK_S32 uvc_vpss_stop(UVC_MPI_CFG uvcCfg, MpiVpssChannelType chnType) {
    RK_S32 s32Ret = RK_SUCCESS;

    s32Ret = smart_door_destroy_vpss(ctx->stVpssCfg);
    if (s32Ret) {
        RK_LOGE("destroy vpss failed: %d", s32Ret);
        return s32Ret;
    }
    return s32Ret;
}

UVC_VENC_CHN_CTX_S uvc_get_venc_chn_ctx(UVC_MPI_CFG uvcCfg) {
  UVC_VENC_CHN_CTX_S vencChnCtx;
  memset(&vencChnCtx, 0, sizeof(UVC_VENC_CHN_CTX_S));

  vencChnCtx.devId = 0;
  vencChnCtx.channelId = uvcCfg.venc_cfg.common_cfg.channel_id;

  return vencChnCtx;
}

RK_S32 uvc_venc_config(UVC_MPI_CFG *uvcCfg) {

    switch (uvcCfg->uvc_cfg.fcc) {
    case V4L2_PIX_FMT_MJPEG:
        ctx->stVencCfg[2].u32DstCodec = RK_VIDEO_ID_MJPEG;
        ctx->stVencCfg[2].enRcMode = VENC_RC_MODE_MJPEGCBR;
        break;
    case V4L2_PIX_FMT_H264:
        ctx->stVencCfg[2].u32DstCodec = RK_VIDEO_ID_AVC;
        ctx->stVencCfg[2].enRcMode = VENC_RC_MODE_H264CBR;
        break;
    case V4L2_PIX_FMT_H265:
        ctx->stVencCfg[2].u32DstCodec = RK_VIDEO_ID_HEVC;
        ctx->stVencCfg[2].enRcMode = VENC_RC_MODE_H265CBR;
        break;
    default:
        RK_LOGE("venc not support this fcc 0x%x\n", uvcCfg->uvc_cfg.fcc);
        break;
    }

    ctx->stVencCfg[2].u32ChnId = uvcCfg->venc_cfg.common_cfg.channel_id;
    ctx->stVencCfg[2].u32SrcWidth = uvcCfg->uvc_cfg.width;
    ctx->stVencCfg[2].u32SrcHeight = uvcCfg->uvc_cfg.height;
    ctx->stVencCfg[2].u32srcVirWidth = uvcCfg->uvc_cfg.width;
    ctx->stVencCfg[2].u32srcVirHeight = uvcCfg->uvc_cfg.height;
    ctx->stVencCfg[2].u32BufferSize = uvcCfg->uvc_cfg.width * uvcCfg->uvc_cfg.height * 2;
    ctx->stVencCfg[2].u32BitRateKb = 2 * 1024;
    ctx->stVencCfg[2].u32FixQp = 0;// valid when rc_mode is fixqp
    ctx->stVencCfg[2].bSuperFrm = RK_FALSE;
    ctx->stVencCfg[2].enGopMode = VENC_GOPMODE_NORMALP;
    ctx->stVencCfg[2].u32GopSize = 30;

    uvcCfg->venc_cfg.common_cfg.change &= (~MPI_VENC_COMMON_CFG_CHANGE_CREATE);
    uvcCfg->venc_cfg.common_cfg.change &= (~MPI_VENC_COMMON_CFG_CHANGE_CHN_ATTR);
    uvcCfg->venc_cfg.common_cfg.change &= (~MPI_VENC_COMMON_CFG_CHANGE_ROTATION);
    uvcCfg->venc_cfg.mjpeg_cfg.change &= (~MPI_VENC_MJPEG_CFG_CHANGE_QFACTOR);

    return RK_SUCCESS;
}

RK_S32 uvc_venc_start(UVC_MPI_CFG uvcCfg) {
    RK_S32 s32Ret = RK_SUCCESS;

    s32Ret = smart_door_init_venc(&ctx->stVencCfg[2]);
    if (s32Ret) {
        RK_LOGE("init venc failed: %d", s32Ret);
        return s32Ret;
    }
    s32Ret = smart_door_create_venc(ctx->stVencCfg[2]);
    if (s32Ret) {
        RK_LOGE("create venc failed: %d", s32Ret);
        return s32Ret;
    }
    return s32Ret;
}

RK_S32 uvc_venc_stop(UVC_MPI_CFG uvcCfg) {
    RK_S32 s32Ret = RK_SUCCESS;

    s32Ret = smart_door_destroy_venc(ctx->stVencCfg[2]);
    if (s32Ret) {
        RK_LOGE("create venc failed: %d", s32Ret);
        return s32Ret;
    }
    return s32Ret;
}