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

#include "osd.h"
#include "osd_rockchip_logo.h"
#include "uvc_log.h"
#include "uvc_mpi_config.h"
#include "uvc_video.h"

static RK_S32 osd_create(RGN_HANDLE handle, MPP_CHN_S *mppChn, RK_U32 layer,
                         RECT_S *rect) {
  RK_S32 s32Ret = RK_SUCCESS;
  RGN_ATTR_S stRgnAttr;
  RGN_CHN_ATTR_S stRgnChnAttr;
  if (mppChn->enModId == RK_ID_VPSS)
    stRgnAttr.enType = OVERLAY_EX_RGN;
  else
    stRgnAttr.enType = OVERLAY_RGN;
  stRgnAttr.unAttr.stOverlay.enPixelFmt = RK_FMT_RGBA8888;
  stRgnAttr.unAttr.stOverlay.stSize.u32Width = rect->u32Width;
  stRgnAttr.unAttr.stOverlay.stSize.u32Height = rect->u32Height;
  stRgnAttr.unAttr.stOverlay.u32ClutNum = 0;

  s32Ret = RK_MPI_RGN_Create(handle, &stRgnAttr);
  if (RK_SUCCESS != s32Ret) {
    LOG_ERROR("RK_MPI_RGN_Create (%d) failed with %#x!\n", handle, s32Ret);
    RK_MPI_RGN_Destroy(handle);
    return s32Ret;
  }
  memset(&stRgnChnAttr, 0, sizeof(stRgnChnAttr));
  stRgnChnAttr.bShow = RK_TRUE;
  stRgnChnAttr.enType = stRgnAttr.enType;
  stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = rect->s32X;
  stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = rect->s32Y;
  stRgnChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 0;
  stRgnChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 255;
  stRgnChnAttr.unChnAttr.stOverlayChn.u32Layer = layer;
  stRgnChnAttr.unChnAttr.stOverlayChn.stQpInfo.bEnable = RK_FALSE;

  s32Ret = RK_MPI_RGN_AttachToChn(handle, mppChn, &stRgnChnAttr);
  if (RK_SUCCESS != s32Ret) {
    LOG_ERROR("RK_MPI_RGN_AttachToChn (%d) failed with %#x!\n", handle, s32Ret);
  }

  return s32Ret;
}

static RK_S32 osd_destroy(RGN_HANDLE handle, MPP_CHN_S *mppChn) {
  RK_S32 s32Ret = RK_SUCCESS;

  s32Ret = RK_MPI_RGN_DetachFromChn(handle, mppChn);
  if (RK_SUCCESS != s32Ret) {
    LOG_ERROR("RK_MPI_RGN_DetachFrmChn (%d) failed with %#x!\n", handle,
              s32Ret);
  }
  s32Ret = RK_MPI_RGN_Destroy(handle);
  if (RK_SUCCESS != s32Ret) {
    LOG_ERROR("RK_MPI_RGN_Destroy [%d] failed with %#x\n", handle, s32Ret);
  }

  return s32Ret;
}

static RK_S32 osd_show_or_hide(RGN_HANDLE handle, const MPP_CHN_S *mppChn,
                               RK_BOOL bShow) {
  RGN_CHN_ATTR_S stChnAttr;
  RK_S32 s32Ret = RK_SUCCESS;

  if (RK_NULL == mppChn) {
    LOG_ERROR("input parameter is null. it is invaild!\n");
    return RK_FAILURE;
  }

  s32Ret = RK_MPI_RGN_GetDisplayAttr(handle, mppChn, &stChnAttr);
  if (RK_SUCCESS != s32Ret) {
    LOG_ERROR("RK_MPI_RGN_GetDisplayAttr (%d)) failed with %#x!\n", handle,
              s32Ret);
    return RK_FAILURE;
  }

  stChnAttr.bShow = bShow;

  s32Ret = RK_MPI_RGN_SetDisplayAttr(handle, mppChn, &stChnAttr);
  if (RK_SUCCESS != s32Ret) {
    LOG_ERROR("RK_MPI_RGN_SetDisplayAttr (%d)) failed with %#x!\n", handle,
              s32Ret);
    return RK_FAILURE;
  }

  return RK_SUCCESS;
}

static RK_S32 osd_test_show_rockchip_logo(RGN_HANDLE handle) {
  RK_S32 s32Ret = RK_SUCCESS;
  RGN_CANVAS_INFO_S stCanvasInfo;
  RK_U32 *data;
  FILE *p;

  LOG_ERROR("test show rockchip logo\n");
  memset(&stCanvasInfo, 0, sizeof(RGN_CANVAS_INFO_S));

  s32Ret = RK_MPI_RGN_GetCanvasInfo(handle, &stCanvasInfo);
  if (s32Ret != RK_SUCCESS) {
    LOG_ERROR("RK_MPI_RGN_GetCanvasInfo failed with %#x!\n", s32Ret);
  }

  data = (RK_U32 *)stCanvasInfo.u64VirAddr;
  p = fopen("/data/rockchip-logo-320-96.rgba", "r");
  if (p) {
    fread(data, 4, stCanvasInfo.u32VirWidth * stCanvasInfo.u32VirHeight, p);
    fclose(p);
  } else {
// no osd file, copy array to rgn
#if 1 // test rgba data to set 0
    memcpy(data, u32BGRA8888RKLOGO,
           stCanvasInfo.u32VirWidth * stCanvasInfo.u32VirHeight * 4);
#else // test rgba data
    for (RK_U32 i = 0; i < stCanvasInfo.u32VirWidth * stCanvasInfo.u32VirHeight;
         i++) {
      // RK_FMT_RGBA8888
      if (i < stCanvasInfo.u32VirWidth * stCanvasInfo.u32VirHeight / 4)
        data[i] = 0xff000000; // black
      else if (i < stCanvasInfo.u32VirWidth * stCanvasInfo.u32VirHeight / 2)
        data[i] = 0xffff0000; // blue
      else if (i < stCanvasInfo.u32VirWidth * stCanvasInfo.u32VirHeight / 4 * 3)
        data[i] = 0xff00ff00; // green
      else
        data[i] = 0xff0000ff; // red
    }
#endif
  }

  s32Ret = RK_MPI_RGN_UpdateCanvas(handle);
  if (s32Ret != RK_SUCCESS) {
    LOG_ERROR("RK_MPI_RGN_UpdateCanvas failed with %#x!\n", s32Ret);
  }

  return s32Ret;
}

static RK_S32 osd_test_init(UVC_MPI_CFG *uvcCfg) {
  RGN_HANDLE handle = 0;
  MPP_CHN_S mppChn;
  RK_U32 layer = 0;
  RECT_S rect;
  RK_S32 s32Ret;

  if (uvcCfg->uvc_cfg.fcc == V4L2_PIX_FMT_YUYV ||
      uvcCfg->osd_cfg.force_use_vpss) {
    mppChn.enModId = RK_ID_VPSS;
    mppChn.s32DevId = uvcCfg->vpss_cfg[MPI_VPSS_CHANNEL_TYPE_UVC].group_id;
    mppChn.s32ChnId = uvcCfg->vpss_cfg[MPI_VPSS_CHANNEL_TYPE_UVC].channel_id;
  } else {
    mppChn.enModId = RK_ID_VENC;
    mppChn.s32DevId = 0;
    mppChn.s32ChnId = uvcCfg->venc_cfg.common_cfg.channel_id;
  }
  rect.s32X = 100;
  rect.s32Y = 200;
  rect.u32Width = 320; // need align 16
  rect.u32Height = 96; // need align 16

  s32Ret = osd_create(handle, &mppChn, layer, &rect);
  s32Ret = osd_test_show_rockchip_logo(handle);
  // defalut is show, if want hide this osd, refer to below setting
  // osd_show_or_hide(handle, &mppChn, RK_FALSE);

  return s32Ret;
}

static RK_S32 osd_test_deinit(UVC_MPI_CFG *uvcCfg) {
  RK_S32 s32Ret;
  MPP_CHN_S mppChn;
  RGN_HANDLE handle = 0;

  if (uvcCfg->uvc_cfg.fcc == V4L2_PIX_FMT_YUYV ||
      uvcCfg->osd_cfg.force_use_vpss) {
    mppChn.enModId = RK_ID_VPSS;
    mppChn.s32DevId = uvcCfg->vpss_cfg[MPI_VPSS_CHANNEL_TYPE_UVC].group_id;
    mppChn.s32ChnId = uvcCfg->vpss_cfg[MPI_VPSS_CHANNEL_TYPE_UVC].channel_id;
  } else {
    mppChn.enModId = RK_ID_VENC;
    mppChn.s32DevId = 0;
    mppChn.s32ChnId = uvcCfg->venc_cfg.common_cfg.channel_id;
  }

  return osd_destroy(handle, &mppChn);
}

RK_S32 osd_start(UVC_MPI_CFG *uvcCfg) {
  RK_S32 s32Ret;

  // now just for test run
  s32Ret = osd_test_init(uvcCfg);

  // TODO(user) you can comment the code above, and implement it yourself
  return s32Ret;
}

RK_S32 osd_stop(UVC_MPI_CFG *uvcCfg) {
  RK_S32 s32Ret;

  // now just for test run
  s32Ret = osd_test_deinit(uvcCfg);

  // TODO(user) you can comment the code above, and implement it yourself
  return s32Ret;
}
