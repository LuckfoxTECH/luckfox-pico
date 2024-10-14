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

#ifndef SRC_TESTS_RT_MPI_COMMON_TEST_COMM_VPSS_H_
#define SRC_TESTS_RT_MPI_COMMON_TEST_COMM_VPSS_H_

#include "rk_common.h"
#include "rk_comm_vpss.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct _TEST_VPSS_PROC_CTX {
    const char *srcFileName;
    const char *dstSaveFileName;
    RK_U32 u32RawWidth;
    RK_U32 u32RawHeight;
    RK_U32 u32RawPixelFmt;
    RK_U32 u32SendFrameRate;
} TEST_VPSS_PROC_CTX_S;

RK_S32 TEST_VPSS_Start(VPSS_GRP VpssGrp, RK_U32 u32ChnNum,
                        VPSS_GRP_ATTR_S *pstVpssGrpAttr,
                        VPSS_CHN_ATTR_S *pstVpssChnAttr);
RK_S32 TEST_VPSS_Stop(VPSS_GRP VpssGrp, RK_U32 u32ChnNum);

RK_S32 TEST_VPSS_GrpSetZoom(VPSS_GRP VpssGrp, RK_U32 u32Zoom, RK_BOOL bEnable);
RK_S32 TEST_VPSS_ChnSetZoom(VPSS_GRP VpssGrp, VPSS_CHN VpssChn, RK_U32 u32Zoom, RK_BOOL bEnable);

RK_S32 TEST_VPSS_SetChnRotation(VPSS_GRP VpssGrp, VPSS_CHN VpssChn, ROTATION_E enRotation);
RK_S32 TEST_VPSS_SetChnRotationEx(VPSS_GRP VpssGrp, VPSS_CHN VpssChn, RK_U32 u32Angle);

RK_S32 TEST_VPSS_StartProc(
        VPSS_GRP VpssGrp, RK_U32 u32ChnNum,
        const TEST_VPSS_PROC_CTX_S *pstProcCtx);
RK_S32 TEST_VPSS_StopProc(VPSS_GRP VpssGrp);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif  // SRC_TESTS_RT_MPI_COMMON_TEST_COMM_VPSS_H_
