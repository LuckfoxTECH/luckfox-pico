/*
 * Copyright 2021 Rockchip Electronics Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SRC_TESTS_RT_MPI_COMMON_TEST_MOD_VPSS_H_
#define SRC_TESTS_RT_MPI_COMMON_TEST_MOD_VPSS_H_

#include "rk_comm_vpss.h"
#include "rk_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct _rkMpiVPSSCtx {
  const char *srcFileName;
  const char *dstFilePath;
  RK_S32 s32LoopCount;
  RK_S32 s32VProcDevType;
  RK_S32 s32GrpNum;
  RK_S32 s32ChnNum;
  RK_BOOL bGrpCropEn;
  RK_BOOL bChnCropEn;
  RK_S32 s32GrpCropRatio;
  RK_S32 s32ChnCropRatio;

  RK_S32 s32SrcWidth;
  RK_S32 s32SrcHeight;
  RK_S32 s32SrcVirWidth;
  RK_S32 s32SrcVirHeight;
  RK_S32 s32SrcCompressMode;
  RK_S32 s32SrcPixFormat;

  RK_S32 s32DstWidth;
  RK_S32 s32DstHeight;
  RK_S32 s32DstCompressMode;
  RK_S32 s32DstPixFormat;

  RK_S32 s32Rotation;
  RK_S32 s32RotationEx;
  RK_S32 s32Mirror;
  RK_S32 s32Flip;

  RK_BOOL bAttachPool;
  RK_S32 s32ChnMode;
  RK_U32 u32ChnDepth;

  RK_S32 s32SrcChnRate;
  RK_S32 s32DstChnRate;
  RK_S32 s32SrcGrpRate;
  RK_S32 s32DstGrpRate;

  RK_S32 s32GrpIndex;
  MB_POOL attachPool;
} TEST_VPSS_CTX_S;

RK_S32 TEST_VPSS_ModInit(TEST_VPSS_CTX_S *pstCtx);
RK_S32 TEST_VPSS_ModDeInit(TEST_VPSS_CTX_S *pstCtx);
RK_S32 TEST_VPSS_ModSendFrame(TEST_VPSS_CTX_S *pstCtx);
RK_S32 TEST_VPSS_ModGetGrpFrame(TEST_VPSS_CTX_S *pstCtx,
                                VIDEO_FRAME_INFO_S *pstVideoFrame);
RK_S32 TEST_VPSS_ModGetChnFrame(TEST_VPSS_CTX_S *pstCtx,
                                VIDEO_FRAME_INFO_S *pstVideoFrame);

RK_S32 TEST_VPSS_ModTest(TEST_VPSS_CTX_S *pstCtx);
RK_S32 TEST_VPSS_UnitTest(TEST_VPSS_CTX_S *pstCtx,
                          VIDEO_FRAME_INFO_S *pstChnFrames);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif // SRC_TESTS_RT_MPI_COMMON_TEST_MOD_VPSS_H_
