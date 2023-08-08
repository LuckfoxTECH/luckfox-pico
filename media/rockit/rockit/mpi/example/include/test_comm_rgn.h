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

#ifndef SRC_TESTS_RT_MPI_COMMON_TEST_RGN_COMM_H_
#define SRC_TESTS_RT_MPI_COMMON_TEST_RGN_COMM_H_

#include "rk_comm_rgn.h"
#include "rk_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

RK_S32 TEST_RGN_CreateOverlayAndAttach(RGN_HANDLE RgnHandle, RK_U32 u32Width,
                                       RK_U32 u32Height, PIXEL_FORMAT_E enFmt,
                                       const MPP_CHN_S *pstChn,
                                       RK_U32 u32ChnNum);
RK_S32 TEST_RGN_CreateCoverAndAttach(RGN_HANDLE RgnHandle,
                                     const COVER_CHN_ATTR_S *pstCoverAttr,
                                     const MPP_CHN_S *pstChn, RK_U32 u32ChnNum);
RK_S32 TEST_RGN_DestroyAndDetach(RGN_HANDLE RgnHandle, const MPP_CHN_S *pstChn,
                                 RK_U32 u32ChnNum);

RK_S32 TEST_RGN_ChangePosition(RGN_HANDLE RgnHandle, const MPP_CHN_S *pstChn,
                               RK_S32 s32X, RK_S32 s32Y);

RK_S32 TEST_RGN_ShowOrHide(RGN_HANDLE RgnHandle, const MPP_CHN_S *pstChn,
                           RK_BOOL bShow);

RK_S32 TEST_RGN_LoadBmp(const RK_CHAR *filename, BITMAP_S *pstBitmap,
                        PIXEL_FORMAT_E enBmpFmt);
RK_S32 TEST_RGN_CreateBmp(RK_U32 u32Width, RK_U32 u32Height,
                          PIXEL_FORMAT_E enBmpFmt, BITMAP_S *pstBitmap);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif // SRC_TESTS_RT_MPI_COMMON_TEST_RGN_COMM_H_
