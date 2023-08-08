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

#include "rkadk_disp.h"
#include "rkadk_log.h"
#include "rkadk_param.h"
#include "rkadk_media_comm.h"
#include "rkmedia_api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void RKADK_DISP_SetChn(RKADK_PARAM_DISP_CFG_S *pstDispCfg,
                              MPP_CHN_S *pstViChn, MPP_CHN_S *pstVoChn,
                              MPP_CHN_S *pstRgaChn) {
  pstViChn->enModId = RK_ID_VI;
  pstViChn->s32DevId = 0;
  pstViChn->s32ChnId = pstDispCfg->vi_attr.u32ViChn;

  pstRgaChn->enModId = RK_ID_RGA;
  pstRgaChn->s32DevId = 0;
  pstRgaChn->s32ChnId = pstDispCfg->rga_chn;

  pstVoChn->enModId = RK_ID_VO;
  pstVoChn->s32DevId = 0;
  pstVoChn->s32ChnId = pstDispCfg->vo_chn;
}

static bool RKADK_DISP_IsUseRga(RKADK_PARAM_DISP_CFG_S *pstDispCfg) {
  RKADK_U32 u32InWidth, u32InHeight;
  RKADK_U32 u32OutWidth, u32OutHeight;
  IMAGE_TYPE_E enInImgType, enOutImgType;

  enInImgType = pstDispCfg->vi_attr.stChnAttr.enPixFmt;
  u32InWidth = pstDispCfg->vi_attr.stChnAttr.u32Width;
  u32InHeight = pstDispCfg->vi_attr.stChnAttr.u32Height;
  u32OutWidth = pstDispCfg->width;
  u32OutHeight = pstDispCfg->height;
  enOutImgType = RKADK_PARAM_GetPixFmt(pstDispCfg->img_type);

  if (pstDispCfg->rotaion != 0 || enOutImgType != enInImgType ||
      (u32InWidth != u32OutWidth || u32InHeight != u32OutHeight)) {
    RKADK_LOGD("rotaion: %d", pstDispCfg->rotaion);
    RKADK_LOGD("enInImgType: %d, enOutImgType: %d", enInImgType, enOutImgType);
    RKADK_LOGD("In[%d, %d], Out[%d, %d]", u32InWidth, u32InHeight, u32OutWidth,
               u32OutHeight);
    return true;
  }

  return false;
}

static RKADK_S32 RKADK_DISP_Enable(RKADK_U32 u32CamId,
                                   RKADK_PARAM_DISP_CFG_S *pstDispCfg,
                                   bool bUseRga) {
  int ret = 0;
  RGA_ATTR_S stRgaAttr;
  VO_CHN_ATTR_S stVoAttr;

  // Create VI
  ret = RKADK_MPI_VI_Init(u32CamId, pstDispCfg->vi_attr.u32ViChn,
                          &(pstDispCfg->vi_attr.stChnAttr));
  if (ret) {
    RKADK_LOGE("RKADK_MPI_VI_Init faled %d", ret);
    return ret;
  }

  // Create RGA
  if (bUseRga) {
    memset(&stRgaAttr, 0, sizeof(stRgaAttr));
    stRgaAttr.bEnBufPool = (RK_BOOL)pstDispCfg->enable_buf_pool;
    stRgaAttr.u16BufPoolCnt = pstDispCfg->buf_pool_cnt;
    stRgaAttr.u16Rotaion = pstDispCfg->rotaion;
    stRgaAttr.stImgIn.u32X = 0;
    stRgaAttr.stImgIn.u32Y = 0;
    stRgaAttr.stImgIn.imgType = pstDispCfg->vi_attr.stChnAttr.enPixFmt;
    stRgaAttr.stImgIn.u32Width = pstDispCfg->vi_attr.stChnAttr.u32Width;
    stRgaAttr.stImgIn.u32Height = pstDispCfg->vi_attr.stChnAttr.u32Height;
    stRgaAttr.stImgIn.u32HorStride = pstDispCfg->vi_attr.stChnAttr.u32Width;
    stRgaAttr.stImgIn.u32VirStride = pstDispCfg->vi_attr.stChnAttr.u32Height;
    stRgaAttr.stImgOut.u32X = 0;
    stRgaAttr.stImgOut.u32Y = 0;
    stRgaAttr.stImgOut.imgType = RKADK_PARAM_GetPixFmt(pstDispCfg->img_type);
    stRgaAttr.stImgOut.u32Width = pstDispCfg->width;
    stRgaAttr.stImgOut.u32Height = pstDispCfg->height;
    stRgaAttr.stImgOut.u32HorStride = pstDispCfg->width;
    stRgaAttr.stImgOut.u32VirStride = pstDispCfg->height;
    ret = RKADK_MPI_RGA_Init(pstDispCfg->rga_chn, &stRgaAttr);
    if (ret) {
      RKADK_LOGE("Init rga[%d] falied[%d]", pstDispCfg->rga_chn, ret);
      RKADK_MPI_VI_DeInit(u32CamId, pstDispCfg->vi_attr.u32ViChn);
      return ret;
    }
  }

  // Create VO
  memset(&stVoAttr, 0, sizeof(VO_CHN_ATTR_S));
  stVoAttr.pcDevNode = pstDispCfg->device_node;
  stVoAttr.emPlaneType = pstDispCfg->plane_type;
  stVoAttr.enImgType = RKADK_PARAM_GetPixFmt(pstDispCfg->img_type);
  stVoAttr.u16Zpos = pstDispCfg->z_pos;
  stVoAttr.stImgRect.s32X = 0;
  stVoAttr.stImgRect.s32Y = 0;
  stVoAttr.stImgRect.u32Width = pstDispCfg->width;
  stVoAttr.stImgRect.u32Height = pstDispCfg->height;
  stVoAttr.stDispRect.s32X = 0;
  stVoAttr.stDispRect.s32Y = 0;
  stVoAttr.stDispRect.u32Width = pstDispCfg->width;
  stVoAttr.stDispRect.u32Height = pstDispCfg->height;
  ret = RK_MPI_VO_CreateChn(pstDispCfg->vo_chn, &stVoAttr);
  if (ret) {
    RKADK_LOGE("Create VO[%d] failed(%d)", pstDispCfg->vo_chn, ret);
    RKADK_MPI_RGA_DeInit(pstDispCfg->rga_chn);
    RKADK_MPI_VI_DeInit(u32CamId, pstDispCfg->vi_attr.u32ViChn);
    return ret;
  }

  return 0;
}

RKADK_S32 RKADK_DISP_Init(RKADK_U32 u32CamId) {
  int ret = 0;
  bool bSysInit = false;
  bool bUseRga = false;
  MPP_CHN_S stViChn, stVoChn, stRgaChn;

  RKADK_CHECK_CAMERAID(u32CamId, RKADK_FAILURE);
  RKADK_LOGI("Disp u32CamId[%d] Init Start...", u32CamId);

  bSysInit = RKADK_MPI_SYS_CHECK();
  if (!bSysInit) {
    RKADK_LOGE("System is not initialized");
    return -1;
  }
  RKADK_PARAM_Init(NULL, NULL);

  RKADK_PARAM_DISP_CFG_S *pstDispCfg = RKADK_PARAM_GetDispCfg(u32CamId);
  if (!pstDispCfg) {
    RKADK_LOGE("RKADK_PARAM_GetDispCfg[%d] failed", u32CamId);
    return -1;
  }

  bUseRga = RKADK_DISP_IsUseRga(pstDispCfg);

  RKADK_DISP_SetChn(pstDispCfg, &stViChn, &stVoChn, &stRgaChn);
  ret = RKADK_DISP_Enable(u32CamId, pstDispCfg, bUseRga);
  if (ret) {
    RKADK_LOGE("RKADK_DISP_Enable failed(%d)", ret);
    return ret;
  }

  if (bUseRga) {
    // Bind RGA to VO
    ret = RK_MPI_SYS_Bind(&stRgaChn, &stVoChn);
    if (ret) {
      RKADK_LOGE("Bind RGA[%d] to VO[%d] failed(%d)", stRgaChn.s32ChnId,
                 stVoChn.s32ChnId, ret);
      goto failed;
    }

    // Bind VI to RGA
    ret = RKADK_MPI_SYS_Bind(&stViChn, &stRgaChn);
    if (ret) {
      RKADK_LOGE("Bind VI[%d] to RGA[%d] failed(%d)", stViChn.s32ChnId,
                 stRgaChn.s32ChnId, ret);
      RK_MPI_SYS_UnBind(&stRgaChn, &stVoChn);
      goto failed;
    }
  } else {
    // Bind VI to VO
    ret = RK_MPI_SYS_Bind(&stViChn, &stVoChn);
    if (ret) {
      RKADK_LOGE("Bind VI[%d] to VO[%d] failed(%d)", stViChn.s32ChnId,
                 stVoChn.s32ChnId, ret);
      goto failed;
    }
  }

  RKADK_LOGI("Disp u32CamId[%d] Init End...", u32CamId);
  return 0;

failed:
  RKADK_LOGI("Disp u32CamId[%d] Init failed...", u32CamId);
  RK_MPI_VO_DestroyChn(stVoChn.s32ChnId);

  if (bUseRga)
    RKADK_MPI_RGA_DeInit(stRgaChn.s32ChnId);

  RKADK_MPI_VI_DeInit(u32CamId, stViChn.s32ChnId);
  return ret;
}

RKADK_S32 RKADK_DISP_DeInit(RKADK_U32 u32CamId) {
  int ret = 0;
  bool bUseRga = false;
  MPP_CHN_S stViChn, stVoChn, stRgaChn;

  RKADK_CHECK_CAMERAID(u32CamId, RKADK_FAILURE);
  RKADK_LOGI("Disp u32CamId[%d] DeInit Start...", u32CamId);

  RKADK_PARAM_DISP_CFG_S *pstDispCfg = RKADK_PARAM_GetDispCfg(u32CamId);
  if (!pstDispCfg) {
    RKADK_LOGE("Live RKADK_PARAM_GetDispCfg failed");
    return -1;
  }

  RKADK_DISP_SetChn(pstDispCfg, &stViChn, &stVoChn, &stRgaChn);

  bUseRga = RKADK_DISP_IsUseRga(pstDispCfg);
  if (bUseRga) {
    ret = RK_MPI_SYS_UnBind(&stRgaChn, &stVoChn);
    if (ret) {
      RKADK_LOGE("UnBind RGA[%d] to VO[%d] failed(%d)", stRgaChn.s32ChnId,
                 stVoChn.s32ChnId, ret);
      return ret;
    }

    ret = RKADK_MPI_SYS_UnBind(&stViChn, &stRgaChn);
    if (ret) {
      RKADK_LOGE("UnBind VI[%d] to RGA[%d] failed(%d)", stViChn.s32ChnId,
                 stRgaChn.s32ChnId, ret);
      return ret;
    }
  } else {
    ret = RK_MPI_SYS_UnBind(&stViChn, &stVoChn);
    if (ret) {
      RKADK_LOGE("UnBind VI[%d] to VO[%d] failed(%d)", stViChn.s32ChnId,
                 stVoChn.s32ChnId, ret);
      return ret;
    }
  }

  ret = RK_MPI_VO_DestroyChn(stVoChn.s32ChnId);
  if (ret) {
    RKADK_LOGE("Destory VO[%d] failed(%d)", stVoChn.s32ChnId, ret);
    return ret;
  }

  if (bUseRga) {
    ret = RKADK_MPI_RGA_DeInit(stRgaChn.s32ChnId);
    if (ret) {
      RKADK_LOGE("DeInit RGA[%d] failed(%d)", stRgaChn.s32ChnId, ret);
      return ret;
    }
  }

  ret = RKADK_MPI_VI_DeInit(u32CamId, stViChn.s32ChnId);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_VI_DeInit failed(%d)", ret);
    return ret;
  }

  RKADK_LOGI("Disp u32CamId[%d] DeInit End...", u32CamId);
  return 0;
}

bool RKADK_DISP_CheckRect(RKADK_RECT_S stRect) {
  if (stRect.u32X < 0 || stRect.u32Y < 0 || stRect.u32Width <= 0 ||
      stRect.u32Height <= 0)
    return false;

  return true;
}

RKADK_S32 RKADK_DISP_SetAttr(RKADK_U32 u32CamId,
                                RKADK_DISP_ATTR_S *pstAttr) {
  int ret;
  bool bSetRgaAttr = false, bSetVoattr = false;
  RGA_ATTR_S stRgaAttr;
  VO_CHN_ATTR_S stVoAttr;

  RKADK_CHECK_POINTER(pstAttr, RKADK_FAILURE);

  RKADK_PARAM_DISP_CFG_S *pstDispCfg = RKADK_PARAM_GetDispCfg(u32CamId);
  if (!pstDispCfg) {
    RKADK_LOGE("RKADK_PARAM_GetDispCfg[%d] failed", u32CamId);
    return -1;
  }

  /**************************************************
   * RGA only supports dynamic modification for
   * the attributes listed below:
   *   stRgaAttr.stImgIn.u32X
   *   stRgaAttr.stImgIn.u32Y
   *   stRgaAttr.stImgIn.u32Width
   *   stRgaAttr.stImgIn.u32Height
   *   stRgaAttr.stImgOut.u32X
   *   stRgaAttr.stImgOut.u32Y
   *   stRgaAttr.stImgOut.u32Width
   *   stRgaAttr.stImgOut.u32Height
   * ************************************************/
  memset(&stRgaAttr, 0, sizeof(RGA_ATTR_S));
  stRgaAttr.u16Rotaion = pstDispCfg->rotaion;
  if (RKADK_DISP_CheckRect(pstAttr->stRgaRect.stInRect)) {
    bSetRgaAttr = true;
    stRgaAttr.stImgIn.u32X = pstAttr->stRgaRect.stInRect.u32X;
    stRgaAttr.stImgIn.u32Y = pstAttr->stRgaRect.stInRect.u32Y;
    stRgaAttr.stImgIn.u32Width = pstAttr->stRgaRect.stInRect.u32Width;
    stRgaAttr.stImgIn.u32Height = pstAttr->stRgaRect.stInRect.u32Height;
  }

  if (RKADK_DISP_CheckRect(pstAttr->stRgaRect.stOutRect)) {
    bSetRgaAttr = true;
    stRgaAttr.stImgOut.u32X = pstAttr->stRgaRect.stOutRect.u32X;
    stRgaAttr.stImgOut.u32Y = pstAttr->stRgaRect.stOutRect.u32Y;
    stRgaAttr.stImgOut.u32Width = pstAttr->stRgaRect.stOutRect.u32Width;
    stRgaAttr.stImgOut.u32Height = pstAttr->stRgaRect.stOutRect.u32Height;
  }

  if (bSetRgaAttr) {
    if (!stRgaAttr.stImgIn.u32Width || !stRgaAttr.stImgIn.u32Height) {
      stRgaAttr.stImgIn.u32Width = pstDispCfg->vi_attr.stChnAttr.u32Width;
      stRgaAttr.stImgIn.u32Height = pstDispCfg->vi_attr.stChnAttr.u32Height;
      RKADK_LOGD("RGA used default stImgIn[0, 0, %d, %d]",
                 stRgaAttr.stImgIn.u32Width, stRgaAttr.stImgIn.u32Height);
    }

    if (!stRgaAttr.stImgOut.u32Width || !stRgaAttr.stImgOut.u32Height) {
      stRgaAttr.stImgOut.u32Width = pstDispCfg->width;
      stRgaAttr.stImgOut.u32Height = pstDispCfg->height;
      RKADK_LOGD("RGA used default stImgOut[0, 0, %d, %d]",
                 stRgaAttr.stImgOut.u32Width, stRgaAttr.stImgOut.u32Height);
    }

    ret = RK_MPI_RGA_SetChnAttr(pstDispCfg->rga_chn, &stRgaAttr);
    if (ret) {
      RKADK_LOGE("RGA Set Attr: ImgIn[%u,%u,%u,%u] "
                 "ImgOut[%u,%u,%u,%u], rotation=%u failed[%d]",
                 stRgaAttr.stImgIn.u32X, stRgaAttr.stImgIn.u32Y,
                 stRgaAttr.stImgIn.u32Width, stRgaAttr.stImgIn.u32Height,
                 stRgaAttr.stImgOut.u32X, stRgaAttr.stImgOut.u32Y,
                 stRgaAttr.stImgOut.u32Width, stRgaAttr.stImgOut.u32Height,
                 stRgaAttr.u16Rotaion, ret);
      return ret;
    }
  }

  /**************************************************
   * Vo only supports dynamic modification for
   * the attributes listed below:
   *   stVoRect.stImgRect.s32X
   *   stVoRect.stImgRect.S32Y
   *   stVoRect.stImgRect.u32Width
   *   stVoRect.stImgRect.u32Height
   *   stVoRect.stDispRect.s32X
   *   stVoRect.stDispRect.s32Y
   *   stVoRect.stDispRect.u32Width
   *   stVoRect.stDispRect.u32Height
   * ************************************************/
  memset(&stVoAttr, 0, sizeof(VO_CHN_ATTR_S));
  if (RKADK_DISP_CheckRect(pstAttr->stVoRect.stInRect)) {
    bSetVoattr = true;
    stVoAttr.stImgRect.s32X = pstAttr->stVoRect.stInRect.u32X;
    stVoAttr.stImgRect.s32Y = pstAttr->stVoRect.stInRect.u32Y;
    stVoAttr.stImgRect.u32Width = pstAttr->stVoRect.stInRect.u32Width;
    stVoAttr.stImgRect.u32Height = pstAttr->stVoRect.stInRect.u32Height;
  }

  if (RKADK_DISP_CheckRect(pstAttr->stVoRect.stOutRect)) {
    bSetVoattr = true;
    stVoAttr.stDispRect.s32X = pstAttr->stVoRect.stOutRect.u32X;
    stVoAttr.stDispRect.s32Y = pstAttr->stVoRect.stOutRect.u32Y;
    stVoAttr.stDispRect.u32Width = pstAttr->stVoRect.stOutRect.u32Width;
    stVoAttr.stDispRect.u32Height = pstAttr->stVoRect.stOutRect.u32Height;
  }

  if (bSetVoattr) {
    if (!stVoAttr.stImgRect.u32Width || !stVoAttr.stImgRect.u32Height) {
      RKADK_LOGD("VO used default stImgRect[0, 0, %d, %d]", pstDispCfg->width,
                 pstDispCfg->height);
      stVoAttr.stImgRect.u32Width = pstDispCfg->width;
      stVoAttr.stImgRect.u32Height = pstDispCfg->height;
    }

    if (!stVoAttr.stDispRect.u32Width || !stVoAttr.stDispRect.u32Height) {
      RKADK_LOGD("VO used default stDispRect[0, 0, %d, %d]", pstDispCfg->width,
                 pstDispCfg->height);
      stVoAttr.stDispRect.u32Width = pstDispCfg->width;
      stVoAttr.stDispRect.u32Height = pstDispCfg->height;
    }

    ret = RK_MPI_VO_SetChnAttr(pstDispCfg->vo_chn, &stVoAttr);
    if (ret) {
      RKADK_LOGE("VO Set Attr: ImgIn[%u,%u,%u,%u] "
                 "ImgOut[%u,%u,%u,%u] failed[%d]",
                 stVoAttr.stImgRect.s32X, stVoAttr.stImgRect.s32Y,
                 stVoAttr.stImgRect.u32Width, stVoAttr.stImgRect.u32Height,
                 stVoAttr.stDispRect.s32X, stVoAttr.stDispRect.s32Y,
                 stVoAttr.stDispRect.u32Width, stVoAttr.stDispRect.u32Height,
                 ret);
      return ret;
    }
  }

  return 0;
}
