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

#ifndef SRC_TESTS_RT_MPI_COMMON_TEST_COMM_TDE_H_
#define SRC_TESTS_RT_MPI_COMMON_TEST_COMM_TDE_H_

#include "rk_comm_tde.h"
#include "rk_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef enum TEST_TDE_OP_TYPE_E {
  TDE_OP_QUICK_COPY = 0,
  TDE_OP_QUICK_RESIZE,
  TDE_OP_QUICK_FILL,
  TDE_OP_ROTATION,
  TDE_OP_MIRROR,
  TDE_OP_COLOR_KEY
} TEST_TDE_OP_TYPE_E;

typedef struct _TEST_TDE_PROC_CTX {
  const char *srcFileName;
  const char *dstFileName;
  TDE_SURFACE_S pstSrc;
  TDE_RECT_S pstSrcRect;
  TDE_SURFACE_S pstDst;
  TDE_RECT_S pstDstRect;
  TDE_OPT_S stOpt;
  RK_S32 fillData;
  ROTATION_E rotateAngle;
  RK_S32 opType;
  RK_S32 s32JobNum;
  RK_S32 s32TaskNum;
  RK_S32 s32ProcessTimes;
} TEST_TDE_PROC_CTX_S;

RK_S32 TEST_TDE_TransSurfaceToVideoFrame(TEST_TDE_PROC_CTX_S *pstCtx,
                                         VIDEO_FRAME_INFO_S *pstFrames);

TDE_HANDLE TEST_TDE_BeginJob();
RK_S32 TEST_TDE_AddTask(TEST_TDE_PROC_CTX_S *pstCtx, TDE_HANDLE jobHandle);
RK_S32 TEST_TDE_EndJob(TDE_HANDLE jobHandle);

RK_S32 TEST_TDE_LoadSrcFrame(TEST_TDE_PROC_CTX_S *pstCtx);
RK_S32 TEST_TDE_CreateDstFrame(TEST_TDE_PROC_CTX_S *pstCtx);
RK_S32 TEST_TDE_CreateDstCMAFrame(TEST_TDE_PROC_CTX_S *pstCtx);
RK_S32 TEST_TDE_ProcessJob(TEST_TDE_PROC_CTX_S *pstCtx,
                           VIDEO_FRAME_INFO_S *pstFrames);
RK_S32 TEST_TDE_MultiTest(TEST_TDE_PROC_CTX_S *ctx);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif // SRC_TESTS_RT_MPI_COMMON_TEST_COMM_TDE_H_
