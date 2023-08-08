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

#ifndef SRC_TESTS_RT_MPI_COMMON_TEST_COMM_SYS_H_
#define SRC_TESTS_RT_MPI_COMMON_TEST_COMM_SYS_H_

#include "rk_comm_video.h"
#include "rk_comm_vpss.h"
#include "rk_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

RK_S32 TEST_SYS_CreateVideoFrame(const PIC_BUF_ATTR_S *pstBufAttr,
                                 VIDEO_FRAME_INFO_S *pstVideoFrame);
RK_S32 TEST_SYS_FuzzyCompareFrameByFile(const char *pFileName,
                                        VIDEO_FRAME_INFO_S *pstVideoFrame,
                                        RK_DOUBLE dThreshold);
RK_S32 TEST_SYS_FuzzyCompareFrame(VIDEO_FRAME_INFO_S *pstVideoFrame,
                                  RK_DOUBLE dThreshold);

RK_S32 TEST_SYS_VpssBindVo(VPSS_GRP VpssGrp, VPSS_CHN VpssChn, VO_DEV VoDev,
                           VO_CHN VoChn);
RK_S32 TEST_SYS_VpssUnbindVo(VPSS_GRP VpssGrp, VPSS_CHN VpssChn, VO_DEV VoDev,
                             VO_CHN VoChn);

RK_S32 TEST_SYS_VdecBindVpss(VDEC_CHN VdecChn, VPSS_GRP VpssGrp,
                             VPSS_CHN VpssChn);
RK_S32 TEST_SYS_VdecUnbindVpss(VDEC_CHN VdecChn, VPSS_GRP VpssGrp,
                               VPSS_CHN VpssChn);

RK_S32 TEST_SYS_VpssBindVenc(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
                             VENC_CHN VencChn);
RK_S32 TEST_SYS_VpssUnbindVenc(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
                               VENC_CHN VencChn);

RK_S32 TEST_SYS_VencBindVdec(VENC_CHN VencChn, VDEC_CHN VdecChn);
RK_S32 TEST_SYS_VencUnbindVdec(VENC_CHN VencChn, VDEC_CHN VdecChn);

RK_S32 TEST_SYS_VdecBindVo(VDEC_CHN VdecChn, VO_DEV VoDev, VO_CHN VoChn);
RK_S32 TEST_SYS_VdecUnbindVo(VDEC_CHN VdecChn, VO_DEV VoDev, VO_CHN VoChn);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif // SRC_TESTS_RT_MPI_COMMON_TEST_COMM_SYS_H_
