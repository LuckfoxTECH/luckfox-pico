/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_MPI_VDEC_H_
#define INCLUDE_RT_MPI_MPI_VDEC_H_

#include "rk_comm_vdec.h"
#include "rk_comm_video.h"
#include "rk_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

RK_S32 RK_MPI_VDEC_CreateChn(VDEC_CHN VdChn, const VDEC_CHN_ATTR_S *pstAttr);
RK_S32 RK_MPI_VDEC_DestroyChn(VDEC_CHN VdChn);

RK_S32 RK_MPI_VDEC_GetChnAttr(VDEC_CHN VdChn, VDEC_CHN_ATTR_S *pstAttr);
RK_S32 RK_MPI_VDEC_SetChnAttr(VDEC_CHN VdChn, const VDEC_CHN_ATTR_S *pstAttr);

RK_S32 RK_MPI_VDEC_StartRecvStream(VDEC_CHN VdChn);
RK_S32 RK_MPI_VDEC_StopRecvStream(VDEC_CHN VdChn);

RK_S32 RK_MPI_VDEC_ResetChn(VDEC_CHN VdChn);

/* s32MilliSec: -1 is block,0 is no block,other positive number is timeout */
RK_S32 RK_MPI_VDEC_SendStream(VDEC_CHN VdChn, const VDEC_STREAM_S *pstStream,
                              RK_S32 s32MilliSec);
RK_S32 RK_MPI_VDEC_GetFrame(VDEC_CHN VdChn, VIDEO_FRAME_INFO_S *pstFrameInfo,
                            RK_S32 s32MilliSec);
RK_S32 RK_MPI_VDEC_ReleaseFrame(VDEC_CHN VdChn,
                                const VIDEO_FRAME_INFO_S *pstFrameInfo);

RK_S32 RK_MPI_VDEC_SetRotation(VDEC_CHN VdChn, ROTATION_E enRotation);
RK_S32 RK_MPI_VDEC_GetRotation(VDEC_CHN VdChn, ROTATION_E *penRotation);

RK_S32 RK_MPI_VDEC_GetModParam(VDEC_MOD_PARAM_S *pstModParam);
RK_S32 RK_MPI_VDEC_SetModParam(const VDEC_MOD_PARAM_S *pstModParam);
RK_S32 RK_MPI_VDEC_SetDisplayMode(VDEC_CHN VdChn,
                                  VIDEO_DISPLAY_MODE_E enDisplayMode);
RK_S32 RK_MPI_VDEC_GetDisplayMode(VDEC_CHN VdChn,
                                  VIDEO_DISPLAY_MODE_E *penDisplayMode);

RK_S32 RK_MPI_VDEC_QueryStatus(VDEC_CHN VdChn, VDEC_CHN_STATUS_S *pstStatus);

RK_S32 RK_MPI_VDEC_SetChnParam(VDEC_CHN VdChn,
                               const VDEC_CHN_PARAM_S *pstParam);
RK_S32 RK_MPI_VDEC_GetChnParam(VDEC_CHN VdChn, VDEC_CHN_PARAM_S *pstParam);

RK_S32 RK_MPI_VDEC_GetFd(VDEC_CHN VdChn);
RK_S32 RK_MPI_VDEC_CloseFd(VDEC_CHN VdChn);
RK_S32 RK_MPI_VDEC_AttachMbPool(VDEC_CHN VdChn, MB_POOL hMbPool);
RK_S32 RK_MPI_VDEC_DetachMbPool(VDEC_CHN VdChn);

RK_S32 RK_MPI_VDEC_SetUserPic(VDEC_CHN VdChn,
                              const VIDEO_FRAME_INFO_S *pstUsrPic);
RK_S32 RK_MPI_VDEC_EnableUserPic(VDEC_CHN VdChn, RK_BOOL bInstant);
RK_S32 RK_MPI_VDEC_DisableUserPic(VDEC_CHN VdChn);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif // INCLUDE_RT_MPI_MPI_VDEC_H_
