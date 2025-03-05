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
#include "uvc_mpi_venc.h"
#include "rk_mpi_sys.h"
#include "uvc_log.h"
#include "uvc_video.h"
#include <cstring>

// config
static RK_S32 venc_set_gopsize(VENC_RC_ATTR_S *pRcAttr, RK_U32 u32GopSize);
static RK_S32 venc_set_bitrate(VENC_RC_ATTR_S *pRcAttr, RK_U32 u32BitRate);
static RK_S32 venc_set_framerate(VENC_RC_ATTR_S *pRcAttr, RK_U32 u32FpsIn,
                                 RK_U32 u32FpsOut);
static RK_S32 venc_set_fixqp(VENC_RC_ATTR_S *pRcAttr, RK_U32 u32FixIQp,
                             RK_U32 u32FixPQp, RK_U32 u32FixBQp);
static RK_S32 venc_set_qp(VENC_RC_ATTR_S *pRcAttr, VENC_RC_PARAM_S qp);
static RK_S32 config_venc_chn_attr(UVC_MPI_CFG uvcCfg);
static RK_S32 config_venc_rc_param(UVC_MPI_CFG uvcCfg);

UVC_VENC_CHN_CTX_S uvc_get_venc_chn_ctx(UVC_MPI_CFG uvcCfg) {
  UVC_VENC_CHN_CTX_S vencChnCtx;
  memset(&vencChnCtx, 0, sizeof(UVC_VENC_CHN_CTX_S));

  vencChnCtx.devId = 0;
  vencChnCtx.channelId = uvcCfg.venc_cfg.common_cfg.channel_id;

  return vencChnCtx;
}

static void set_venc_mjpeg_attr(VENC_CHN_ATTR_S *vencAttr, UVC_MPI_CFG uvcCfg) {
  vencAttr->stVencAttr.enType = RK_VIDEO_ID_MJPEG;
  vencAttr->stVencAttr.u32StreamBufCnt = uvcCfg.venc_cfg.mjpeg_cfg.buf_cnt;
  vencAttr->stRcAttr.enRcMode = uvcCfg.venc_cfg.mjpeg_cfg.rc_mode;

  venc_set_bitrate(&vencAttr->stRcAttr, uvcCfg.venc_cfg.mjpeg_cfg.bps);
  venc_set_framerate(&vencAttr->stRcAttr, uvcCfg.venc_cfg.mjpeg_cfg.fps_in,
                     uvcCfg.venc_cfg.mjpeg_cfg.fps_out);
  venc_set_qp(&vencAttr->stRcAttr, uvcCfg.venc_cfg.mjpeg_cfg.qp);
}

static void set_venc_h264_attr(VENC_CHN_ATTR_S *vencAttr, UVC_MPI_CFG uvcCfg) {
  vencAttr->stVencAttr.enType = RK_VIDEO_ID_AVC;
  vencAttr->stVencAttr.u32Profile = uvcCfg.venc_cfg.h264_cfg.profile;
  vencAttr->stVencAttr.u32StreamBufCnt = uvcCfg.venc_cfg.h264_cfg.buf_cnt;
  vencAttr->stRcAttr.enRcMode = uvcCfg.venc_cfg.h264_cfg.rc_mode;
  vencAttr->stGopAttr = uvcCfg.venc_cfg.h264_cfg.gop_mode;

  venc_set_gopsize(&vencAttr->stRcAttr, uvcCfg.venc_cfg.h264_cfg.gop);
  venc_set_bitrate(&vencAttr->stRcAttr, uvcCfg.venc_cfg.h264_cfg.bps);
  venc_set_framerate(&vencAttr->stRcAttr, uvcCfg.venc_cfg.h264_cfg.fps_in,
                     uvcCfg.venc_cfg.h264_cfg.fps_out);
  venc_set_qp(&vencAttr->stRcAttr, uvcCfg.venc_cfg.h264_cfg.qp);

  LOG_INFO("venc gop:%d bps:%d\n", uvcCfg.venc_cfg.h264_cfg.gop,
           uvcCfg.venc_cfg.h264_cfg.bps);
}

static void set_venc_h265_attr(VENC_CHN_ATTR_S *vencAttr, UVC_MPI_CFG uvcCfg) {
  vencAttr->stVencAttr.enType = RK_VIDEO_ID_HEVC;
  vencAttr->stVencAttr.u32Profile = uvcCfg.venc_cfg.h265_cfg.profile;
  vencAttr->stVencAttr.u32StreamBufCnt = uvcCfg.venc_cfg.h265_cfg.buf_cnt;
  vencAttr->stRcAttr.enRcMode = uvcCfg.venc_cfg.h265_cfg.rc_mode;
  vencAttr->stGopAttr = uvcCfg.venc_cfg.h265_cfg.gop_mode;

  venc_set_gopsize(&vencAttr->stRcAttr, uvcCfg.venc_cfg.h265_cfg.gop);
  venc_set_bitrate(&vencAttr->stRcAttr, uvcCfg.venc_cfg.h265_cfg.bps);
  venc_set_framerate(&vencAttr->stRcAttr, uvcCfg.venc_cfg.h265_cfg.fps_in,
                     uvcCfg.venc_cfg.h265_cfg.fps_out);
  venc_set_qp(&vencAttr->stRcAttr, uvcCfg.venc_cfg.h265_cfg.qp);
}

static VENC_CHN_ATTR_S get_venc_chn_attr(UVC_MPI_CFG uvcCfg) {
  VENC_CHN_ATTR_S vencAttr;
  memset(&vencAttr, 0, sizeof(VENC_CHN_ATTR_S));

  switch (uvcCfg.uvc_cfg.fcc) {
  case V4L2_PIX_FMT_MJPEG:
    set_venc_mjpeg_attr(&vencAttr, uvcCfg);
    break;
  case V4L2_PIX_FMT_H264:
    set_venc_h264_attr(&vencAttr, uvcCfg);
    break;
  case V4L2_PIX_FMT_H265:
    set_venc_h265_attr(&vencAttr, uvcCfg);
    break;
  default:
    LOG_ERROR("venc not support this fcc 0x%x\n", uvcCfg.uvc_cfg.fcc);
    break;
  }

  vencAttr.stVencAttr.enPixelFormat = RK_FMT_YUV420SP;
  vencAttr.stVencAttr.enMirror = uvcCfg.venc_cfg.common_cfg.mirror;
  vencAttr.stVencAttr.u32PicWidth = uvcCfg.uvc_cfg.width;
  vencAttr.stVencAttr.u32PicHeight = uvcCfg.uvc_cfg.height;
  vencAttr.stVencAttr.u32VirWidth = uvcCfg.uvc_cfg.width;
  vencAttr.stVencAttr.u32VirHeight = uvcCfg.uvc_cfg.height;
  vencAttr.stVencAttr.u32BufSize =
      uvcCfg.uvc_cfg.width * uvcCfg.uvc_cfg.height * 2;

  return vencAttr;
}

static VENC_RC_PARAM_S get_venc_rc_param(UVC_MPI_CFG uvcCfg) {
  VENC_RC_PARAM_S vencRcParam;
  memset(&vencRcParam, 0, sizeof(VENC_RC_PARAM_S));

  switch (uvcCfg.uvc_cfg.fcc) {
  case V4L2_PIX_FMT_MJPEG:
    vencRcParam = uvcCfg.venc_cfg.mjpeg_cfg.qp;
    break;
  case V4L2_PIX_FMT_H264:
    vencRcParam = uvcCfg.venc_cfg.h264_cfg.qp;
    break;
  case V4L2_PIX_FMT_H265:
    vencRcParam = uvcCfg.venc_cfg.h265_cfg.qp;
    break;
  default:
    LOG_ERROR("venc not support this fcc 0x%x\n", uvcCfg.uvc_cfg.fcc);
    break;
  }

  return vencRcParam;
}

static RK_S32 config_venc_chn_attr(UVC_MPI_CFG uvcCfg) {
  RK_S32 s32Ret = RK_FAILURE;
  UVC_VENC_CHN_CTX_S vencChnCtx = uvc_get_venc_chn_ctx(uvcCfg);
  VENC_CHN_ATTR_S vencAttr = get_venc_chn_attr(uvcCfg);

  s32Ret = RK_MPI_VENC_SetChnAttr(vencChnCtx.channelId, &vencAttr);

  return s32Ret;
}

static RK_S32 config_venc_rc_param(UVC_MPI_CFG uvcCfg) {
  RK_S32 s32Ret = RK_FAILURE;
  UVC_VENC_CHN_CTX_S vencChnCtx = uvc_get_venc_chn_ctx(uvcCfg);
  VENC_RC_PARAM_S vencRcParam = get_venc_rc_param(uvcCfg);

  s32Ret = RK_MPI_VENC_SetRcParam(vencChnCtx.channelId, &vencRcParam);

  return s32Ret;
}

static RK_S32 venc_set_bitrate(VENC_RC_ATTR_S *pRcAttr, RK_U32 u32BitRate) {
  switch (pRcAttr->enRcMode) {
  case VENC_RC_MODE_MJPEGCBR:
    pRcAttr->stMjpegCbr.u32BitRate = u32BitRate;
    break;
  case VENC_RC_MODE_H264CBR:
    pRcAttr->stH264Cbr.u32BitRate = u32BitRate;
    break;
  case VENC_RC_MODE_H265CBR:
    pRcAttr->stH265Cbr.u32BitRate = u32BitRate;
    break;
  case VENC_RC_MODE_MJPEGVBR:
    pRcAttr->stMjpegVbr.u32BitRate = u32BitRate;
    break;
  case VENC_RC_MODE_H264VBR:
    pRcAttr->stH264Vbr.u32BitRate = u32BitRate;
    break;
  case VENC_RC_MODE_H265VBR:
    pRcAttr->stH265Vbr.u32BitRate = u32BitRate;
    break;
  case VENC_RC_MODE_H264AVBR:
    pRcAttr->stH264Avbr.u32BitRate = u32BitRate;
    break;
  case VENC_RC_MODE_H265AVBR:
    pRcAttr->stH265Avbr.u32BitRate = u32BitRate;
    break;
  default:
    return RK_ERR_VENC_NOT_SUPPORT;
  }

  return RK_SUCCESS;
}

static RK_S32 venc_set_gopsize(VENC_RC_ATTR_S *pRcAttr, RK_U32 u32GopSize) {
  switch (pRcAttr->enRcMode) {
  case VENC_RC_MODE_H264CBR:
    pRcAttr->stH264Cbr.u32Gop = u32GopSize;
    break;
  case VENC_RC_MODE_H265CBR:
    pRcAttr->stH265Cbr.u32Gop = u32GopSize;
    break;
  case VENC_RC_MODE_H264VBR:
    pRcAttr->stH264Vbr.u32Gop = u32GopSize;
    break;
  case VENC_RC_MODE_H265VBR:
    pRcAttr->stH265Vbr.u32Gop = u32GopSize;
    break;
  case VENC_RC_MODE_H264AVBR:
    pRcAttr->stH264Avbr.u32Gop = u32GopSize;
    break;
  case VENC_RC_MODE_H265AVBR:
    pRcAttr->stH265Avbr.u32Gop = u32GopSize;
    break;
  case VENC_RC_MODE_H264FIXQP:
    pRcAttr->stH264FixQp.u32Gop = u32GopSize;
    break;
  case VENC_RC_MODE_H265FIXQP:
    pRcAttr->stH265FixQp.u32Gop = u32GopSize;
    break;
  default:
    return RK_ERR_VENC_NOT_SUPPORT;
  }

  return RK_SUCCESS;
}

static RK_S32 venc_set_framerate(VENC_RC_ATTR_S *pRcAttr, RK_U32 u32FpsIn,
                                 RK_U32 u32FpsOut) {
  switch (pRcAttr->enRcMode) {
  case VENC_RC_MODE_MJPEGCBR:
    pRcAttr->stMjpegCbr.u32SrcFrameRateNum = u32FpsIn;
    pRcAttr->stMjpegCbr.u32SrcFrameRateDen = 1;
    pRcAttr->stMjpegCbr.fr32DstFrameRateNum = u32FpsOut;
    pRcAttr->stMjpegCbr.fr32DstFrameRateDen = 1;
    break;
  case VENC_RC_MODE_H264CBR:
    pRcAttr->stH264Cbr.u32SrcFrameRateNum = u32FpsIn;
    pRcAttr->stH264Cbr.u32SrcFrameRateDen = 1;
    pRcAttr->stH264Cbr.fr32DstFrameRateNum = u32FpsOut;
    pRcAttr->stH264Cbr.fr32DstFrameRateDen = 1;
    break;
  case VENC_RC_MODE_H265CBR:
    pRcAttr->stH265Cbr.u32SrcFrameRateNum = u32FpsIn;
    pRcAttr->stH265Cbr.u32SrcFrameRateDen = 1;
    pRcAttr->stH265Cbr.fr32DstFrameRateNum = u32FpsOut;
    pRcAttr->stH265Cbr.fr32DstFrameRateDen = 1;
    break;
  case VENC_RC_MODE_MJPEGVBR:
    pRcAttr->stMjpegVbr.u32SrcFrameRateNum = u32FpsIn;
    pRcAttr->stMjpegVbr.u32SrcFrameRateDen = 1;
    pRcAttr->stMjpegVbr.fr32DstFrameRateNum = u32FpsOut;
    pRcAttr->stMjpegVbr.fr32DstFrameRateDen = 1;
    break;
  case VENC_RC_MODE_H264VBR:
    pRcAttr->stH264Vbr.u32SrcFrameRateNum = u32FpsIn;
    pRcAttr->stH264Vbr.u32SrcFrameRateDen = 1;
    pRcAttr->stH264Vbr.fr32DstFrameRateNum = u32FpsOut;
    pRcAttr->stH264Vbr.fr32DstFrameRateDen = 1;
    break;
  case VENC_RC_MODE_H265VBR:
    pRcAttr->stH265Vbr.u32SrcFrameRateNum = u32FpsIn;
    pRcAttr->stH265Vbr.u32SrcFrameRateDen = 1;
    pRcAttr->stH265Vbr.fr32DstFrameRateNum = u32FpsOut;
    pRcAttr->stH265Vbr.fr32DstFrameRateDen = 1;
    break;
  case VENC_RC_MODE_H264AVBR:
    pRcAttr->stH264Avbr.u32SrcFrameRateNum = u32FpsIn;
    pRcAttr->stH264Avbr.u32SrcFrameRateDen = 1;
    pRcAttr->stH264Avbr.fr32DstFrameRateNum = u32FpsOut;
    pRcAttr->stH264Avbr.fr32DstFrameRateDen = 1;
    break;
  case VENC_RC_MODE_H265AVBR:
    pRcAttr->stH265Avbr.u32SrcFrameRateNum = u32FpsIn;
    pRcAttr->stH265Avbr.u32SrcFrameRateDen = 1;
    pRcAttr->stH265Avbr.fr32DstFrameRateNum = u32FpsOut;
    pRcAttr->stH265Avbr.fr32DstFrameRateDen = 1;
    break;
  case VENC_RC_MODE_MJPEGFIXQP:
    pRcAttr->stMjpegFixQp.u32SrcFrameRateNum = u32FpsIn;
    pRcAttr->stMjpegFixQp.u32SrcFrameRateDen = 1;
    pRcAttr->stMjpegFixQp.fr32DstFrameRateNum = u32FpsOut;
    pRcAttr->stMjpegFixQp.fr32DstFrameRateDen = 1;
    break;
  case VENC_RC_MODE_H264FIXQP:
    pRcAttr->stH264FixQp.u32SrcFrameRateNum = u32FpsIn;
    pRcAttr->stH264FixQp.u32SrcFrameRateDen = 1;
    pRcAttr->stH264FixQp.fr32DstFrameRateNum = u32FpsOut;
    pRcAttr->stH264FixQp.fr32DstFrameRateDen = 1;
    break;
  case VENC_RC_MODE_H265FIXQP:
    pRcAttr->stH265FixQp.u32SrcFrameRateNum = u32FpsIn;
    pRcAttr->stH265FixQp.u32SrcFrameRateDen = 1;
    pRcAttr->stH265FixQp.fr32DstFrameRateNum = u32FpsOut;
    pRcAttr->stH265FixQp.fr32DstFrameRateDen = 1;
    break;

  default:
    return RK_ERR_VENC_NOT_SUPPORT;
  }

  return RK_SUCCESS;
}

static RK_S32 venc_set_fixqp(VENC_RC_ATTR_S *pRcAttr, RK_U32 u32FixIQp,
                             RK_U32 u32FixPQp, RK_U32 u32FixBQp) {
  switch (pRcAttr->enRcMode) {
  case VENC_RC_MODE_MJPEGFIXQP:
    pRcAttr->stMjpegFixQp.u32Qfactor = u32FixIQp;
    break;
  case VENC_RC_MODE_H264FIXQP:
    pRcAttr->stH264FixQp.u32IQp = u32FixIQp;
    pRcAttr->stH264FixQp.u32PQp = u32FixPQp;
    pRcAttr->stH264FixQp.u32BQp = u32FixBQp;
    break;
  case VENC_RC_MODE_H265FIXQP:
    pRcAttr->stH265FixQp.u32IQp = u32FixIQp;
    pRcAttr->stH265FixQp.u32PQp = u32FixPQp;
    pRcAttr->stH265FixQp.u32BQp = u32FixBQp;
    break;
  default:
    return RK_ERR_VENC_NOT_SUPPORT;
  }

  return RK_SUCCESS;
}

static RK_S32 venc_set_qp(VENC_RC_ATTR_S *pRcAttr, VENC_RC_PARAM_S qp) {
  switch (pRcAttr->enRcMode) {
  case VENC_RC_MODE_MJPEGFIXQP:
    venc_set_fixqp(pRcAttr, qp.stParamMjpeg.u32Qfactor,
                   qp.stParamMjpeg.u32Qfactor, qp.stParamMjpeg.u32Qfactor);
    break;
  case VENC_RC_MODE_H264FIXQP:
    venc_set_fixqp(pRcAttr, qp.stParamH264.u32MinIQp, qp.stParamH264.u32MinQp,
                   qp.stParamH264.u32MinQp);
    break;
  case VENC_RC_MODE_H265FIXQP:
    venc_set_fixqp(pRcAttr, qp.stParamH265.u32MinIQp, qp.stParamH265.u32MinQp,
                   qp.stParamH265.u32MinQp);
    break;
  case VENC_RC_MODE_MJPEGCBR:
  case VENC_RC_MODE_MJPEGVBR:
  case VENC_RC_MODE_H264CBR:
  case VENC_RC_MODE_H264VBR:
  case VENC_RC_MODE_H264AVBR:
  case VENC_RC_MODE_H265CBR:
  case VENC_RC_MODE_H265VBR:
  case VENC_RC_MODE_H265AVBR:
    // just call RK_MPI_VENC_SetRcParam
    break;
  default:
    return RK_ERR_VENC_NOT_SUPPORT;
  }

  return RK_SUCCESS;
}

static RK_S32 config_venc_common(UVC_MPI_CFG *uvcCfg) {
  RK_S32 s32Ret = RK_FAILURE;
  UVC_VENC_CHN_CTX_S vencChnCtx = uvc_get_venc_chn_ctx(*uvcCfg);

  if (uvcCfg->venc_cfg.common_cfg.change & MPI_VENC_COMMON_CFG_CHANGE_CREATE) {
    VENC_CHN_ATTR_S vencAttr = get_venc_chn_attr(*uvcCfg);
    s32Ret = RK_MPI_VENC_CreateChn(vencChnCtx.channelId, &vencAttr);
    if (s32Ret != RK_SUCCESS) {
      LOG_ERROR("RK_MPI_VENC_CreateChn fail 0x%x\n", s32Ret);
      goto __FAILED;
    } else {
      LOG_INFO("RK_MPI_VENC_CreateChn %d\n", vencChnCtx.channelId);
    }
    uvcCfg->venc_cfg.common_cfg.change &= (~MPI_VENC_COMMON_CFG_CHANGE_CREATE);
    uvcCfg->venc_cfg.common_cfg.change &=
        (~MPI_VENC_COMMON_CFG_CHANGE_CHN_ATTR);
  } else {
    if (uvcCfg->venc_cfg.common_cfg.change &
        MPI_VENC_COMMON_CFG_CHANGE_CHN_ATTR) {
      s32Ret = config_venc_chn_attr(*uvcCfg);
      if (s32Ret != RK_SUCCESS) {
        LOG_ERROR("config_venc_chn_attr fail 0x%x\n", s32Ret);
        goto __FAILED;
      } else {
        LOG_INFO("config_venc_chn_attr %d\n", vencChnCtx.channelId);
      }
      uvcCfg->venc_cfg.common_cfg.change &=
          (~MPI_VENC_COMMON_CFG_CHANGE_CHN_ATTR);
    }
  }
  if (uvcCfg->venc_cfg.common_cfg.change &
      MPI_VENC_COMMON_CFG_CHANGE_ROTATION) {
    s32Ret = RK_MPI_VENC_SetChnRotation(vencChnCtx.channelId,
                                        uvcCfg->venc_cfg.common_cfg.rotation);
    if (s32Ret != RK_SUCCESS) {
      LOG_ERROR("RK_MPI_VENC_CreateChn fail 0x%x\n", s32Ret);
      goto __FAILED;
    }
    uvcCfg->venc_cfg.common_cfg.change &=
        (~MPI_VENC_COMMON_CFG_CHANGE_ROTATION);
  }

__FAILED:
  return s32Ret;
}

static RK_S32 config_venc_mjpeg(UVC_MPI_CFG *uvcCfg) {
  RK_S32 s32Ret = RK_FAILURE;

  if (uvcCfg->venc_cfg.mjpeg_cfg.change & MPI_VENC_MJPEG_CFG_CHANGE_QFACTOR) {
    s32Ret = config_venc_rc_param(*uvcCfg);
    if (s32Ret != RK_SUCCESS) {
      LOG_ERROR("RK_MPI_VENC_CreateChn fail 0x%x\n", s32Ret);
      goto __FAILED;
    }
    uvcCfg->venc_cfg.mjpeg_cfg.change &= (~MPI_VENC_MJPEG_CFG_CHANGE_QFACTOR);
  }

__FAILED:
  return s32Ret;
}

static RK_S32 config_venc_h264(UVC_MPI_CFG *uvcCfg) {
  RK_S32 s32Ret = RK_FAILURE;

  if (uvcCfg->venc_cfg.h264_cfg.change & MPI_VENC_H264_CFG_CHANGE_QP) {
    s32Ret = config_venc_rc_param(*uvcCfg);
    if (s32Ret != RK_SUCCESS) {
      LOG_ERROR("RK_MPI_VENC_CreateChn fail 0x%x\n", s32Ret);
      goto __FAILED;
    }
    uvcCfg->venc_cfg.h264_cfg.change &= (~MPI_VENC_H264_CFG_CHANGE_QP);
  }

__FAILED:
  return s32Ret;
}

static RK_S32 config_venc_h265(UVC_MPI_CFG *uvcCfg) {
  RK_S32 s32Ret = RK_FAILURE;

  if (uvcCfg->venc_cfg.h265_cfg.change & MPI_VENC_H265_CFG_CHANGE_QP) {
    s32Ret = config_venc_rc_param(*uvcCfg);
    if (s32Ret != RK_SUCCESS) {
      LOG_ERROR("RK_MPI_VENC_CreateChn fail 0x%x\n", s32Ret);
      goto __FAILED;
    }
    uvcCfg->venc_cfg.h265_cfg.change &= (~MPI_VENC_H265_CFG_CHANGE_QP);
  }

__FAILED:
  return s32Ret;
}

RK_S32 uvc_venc_config(UVC_MPI_CFG *uvcCfg) {
  RK_S32 s32Ret = RK_FAILURE;

  config_venc_common(uvcCfg);

  switch (uvcCfg->uvc_cfg.fcc) {
  case V4L2_PIX_FMT_MJPEG:
    config_venc_mjpeg(uvcCfg);
    break;
  case V4L2_PIX_FMT_H264:
    config_venc_h264(uvcCfg);
    break;
  case V4L2_PIX_FMT_H265:
    config_venc_h265(uvcCfg);
    break;
  default:
    LOG_ERROR("venc not support this fcc 0x%x\n", uvcCfg->uvc_cfg.fcc);
    break;
  }

__FAILED:
  return s32Ret;
}

RK_S32 uvc_venc_start(UVC_MPI_CFG uvcCfg) {
  RK_S32 s32Ret = RK_FAILURE;
  VENC_RECV_PIC_PARAM_S stRecvParam;
  UVC_VENC_CHN_CTX_S vencChnCtx = uvc_get_venc_chn_ctx(uvcCfg);

  stRecvParam.s32RecvPicNum = -1;
  LOG_INFO("RK_MPI_VENC_StartRecvFrame %d\n", vencChnCtx.channelId);
  s32Ret = RK_MPI_VENC_StartRecvFrame(vencChnCtx.channelId, &stRecvParam);
  if (s32Ret != RK_SUCCESS) {
    LOG_ERROR("RK_MPI_VENC_StartRecvFrame fail 0x%x\n", s32Ret);
    goto __FAILED;
  }

__FAILED:
  return s32Ret;
}

RK_S32 uvc_venc_stop(UVC_MPI_CFG uvcCfg) {
  RK_S32 s32Ret = RK_FAILURE;
  UVC_VENC_CHN_CTX_S vencChnCtx = uvc_get_venc_chn_ctx(uvcCfg);

  s32Ret = RK_MPI_VENC_StopRecvFrame(vencChnCtx.channelId);
  if (s32Ret != RK_SUCCESS) {
    return s32Ret;
  }
  LOG_INFO("reset enc chn:%d\n", vencChnCtx.channelId);
  s32Ret = RK_MPI_VENC_ResetChn(vencChnCtx.channelId);
  if (s32Ret != RK_SUCCESS) {
    return s32Ret;
  }
  LOG_INFO("destroy enc chn:%d\n", vencChnCtx.channelId);
  s32Ret = RK_MPI_VENC_DestroyChn(vencChnCtx.channelId);
  if (s32Ret != RK_SUCCESS) {
    LOG_ERROR("RK_MPI_VDEC_DestroyChn :%d fail %x", vencChnCtx.channelId,
              s32Ret);
  }

__FAILED:
  return s32Ret;
}
