/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_RK_MPI_AVS_H_
#define INCLUDE_RT_MPI_RK_MPI_AVS_H_

#include "rk_comm_avs.h"
#include "rk_comm_mb.h"
#include "rk_comm_video.h"
#include "rk_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* Group Settings */
RK_S32 RK_MPI_AVS_CreateGrp(AVS_GRP AVSGrp, const AVS_GRP_ATTR_S *pstGrpAttr);
RK_S32 RK_MPI_AVS_DestroyGrp(AVS_GRP AVSGrp);
RK_S32 RK_MPI_AVS_StartGrp(AVS_GRP AVSGrp);
RK_S32 RK_MPI_AVS_StopGrp(AVS_GRP AVSGrp);
RK_S32 RK_MPI_AVS_ResetGrp(AVS_GRP AVSGrp);
RK_S32 RK_MPI_AVS_GetGrpAttr(AVS_GRP AVSGrp, AVS_GRP_ATTR_S *pstGrpAttr);
RK_S32 RK_MPI_AVS_SetGrpAttr(AVS_GRP AVSGrp, const AVS_GRP_ATTR_S *pstGrpAttr);

/* Pipe Settings */
RK_S32 RK_MPI_AVS_SendPipeFrame(AVS_GRP AVSGrp, AVS_PIPE AVSPipe,
                                const VIDEO_FRAME_INFO_S *pstVideoFrame,
                                RK_S32 s32MilliSec);
RK_S32 RK_MPI_AVS_GetPipeFrame(AVS_GRP AVSGrp, AVS_PIPE AVSPipe,
                               VIDEO_FRAME_INFO_S *pstVideoFrame);
RK_S32 RK_MPI_AVS_ReleasePipeFrame(AVS_GRP AVSGrp, AVS_PIPE AVSPipe,
                                   const VIDEO_FRAME_INFO_S *pstVideoFrame);

/* Channel Settings */
RK_S32 RK_MPI_AVS_SetChnAttr(AVS_GRP AVSGrp, AVS_CHN AVSChn,
                             const AVS_CHN_ATTR_S *pstChnAttr);
RK_S32 RK_MPI_AVS_GetChnAttr(AVS_GRP AVSGrp, AVS_CHN AVSChn,
                             AVS_CHN_ATTR_S *pstChnAttr);
RK_S32 RK_MPI_AVS_EnableChn(AVS_GRP AVSGrp, AVS_CHN AVSChn);
RK_S32 RK_MPI_AVS_DisableChn(AVS_GRP AVSGrp, AVS_CHN AVSChn);
RK_S32 RK_MPI_AVS_GetChnFrame(AVS_GRP AVSGrp, AVS_CHN AVSChn,
                              VIDEO_FRAME_INFO_S *pstVideoFrame,
                              RK_S32 s32MilliSec);
RK_S32 RK_MPI_AVS_ReleaseChnFrame(AVS_GRP AVSGrp, AVS_CHN AVSChn,
                                  const VIDEO_FRAME_INFO_S *pstVideoFrame);
RK_S32 RK_MPI_AVS_SetModParam(const AVS_MOD_PARAM_S *pstModParam);
RK_S32 RK_MPI_AVS_GetModParam(AVS_MOD_PARAM_S *pstModParam);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* INCLUDE_RT_MPI_RK_MPI_AVS_H_ */
