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

#ifndef SRC_TESTS_RT_MPI_COMMON_TEST_COMM_VENC_H_
#define SRC_TESTS_RT_MPI_COMMON_TEST_COMM_VENC_H_

#include "rk_common.h"
#include "rk_comm_venc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct _COMMON_TEST_VENC_CTX_S {
    VENC_CHN VencChn;
    RK_U32 u32Width;
    RK_U32 u32Height;
    RK_U32 u32StreamBufCnt;
    RK_S32 s32RecvPicNum;
    RK_CODEC_ID_E enType;
    RK_U32 u32ReadPicNum;  // for performance test, read one picture to test.
    PIXEL_FORMAT_E enPixFmt;
    const char *pSrcFramePath;
    const char *pSaveStreamPath;
} COMMON_TEST_VENC_CTX_S;

RK_S32 TEST_VENC_Create(COMMON_TEST_VENC_CTX_S *vencCtx);

RK_S32 TEST_VENC_Start(COMMON_TEST_VENC_CTX_S *vencCtx);
RK_S32 TEST_VENC_Stop(VENC_CHN VencChn);

RK_S32 TEST_VENC_SnapStart(COMMON_TEST_VENC_CTX_S *vencCtx);
RK_S32 TEST_VENC_SnapProcess(COMMON_TEST_VENC_CTX_S *vencCtx);
RK_S32 TEST_VENC_SnapStop(VENC_CHN VencChn);

RK_S32 TEST_VENC_SET_BitRate(VENC_RC_ATTR_S *pRcAttr, RK_U32 u32BitRate, RK_U32 u32BitRateMax, RK_U32 u32BitRateMin);
RK_S32 TEST_VENC_SET_GopSize(VENC_RC_ATTR_S *pRcAttr, RK_U32 u32GopSize);
RK_S32 TEST_VENC_SET_FrameRate(VENC_RC_ATTR_S *pRcAttr, RK_U32 u32Fps);
RK_S32 TEST_VENC_SET_FixQp(VENC_RC_ATTR_S *pRcAttr, RK_U32 u32FixIQp,
                           RK_U32 u32FixPQp, RK_U32 u32FixBQp);
RK_S32 TEST_VENC_SET_StatTime(VENC_RC_ATTR_S *pRcAttr, RK_U32 u32StatTime);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif  // SRC_TESTS_RT_MPI_COMMON_TEST_COMM_VENC_H_
