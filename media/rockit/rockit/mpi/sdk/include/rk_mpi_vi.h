/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_MPI_VI_H_
#define INCLUDE_RT_MPI_MPI_VI_H_

#include "rk_comm_vi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* 1 for vi device */
RK_S32 RK_MPI_VI_SetDevAttr(VI_DEV ViDev, const VI_DEV_ATTR_S *pstDevAttr);
RK_S32 RK_MPI_VI_GetDevAttr(VI_DEV ViDev, VI_DEV_ATTR_S *pstDevAttr);
RK_S32 RK_MPI_VI_EnableDev(VI_DEV ViDev);
RK_S32 RK_MPI_VI_DisableDev(VI_DEV ViDev);
RK_S32 RK_MPI_VI_SetDevBindPipe(VI_DEV ViDev,
                                const VI_DEV_BIND_PIPE_S *pstDevBindPipe);
RK_S32 RK_MPI_VI_GetDevBindPipe(VI_DEV ViDev,
                                VI_DEV_BIND_PIPE_S *pstDevBindPipe);
RK_S32 RK_MPI_VI_GetDevIsEnable(VI_DEV ViDev);
RK_S32 RK_MPI_VI_QueryDevStatus(VI_DEV Videv, VI_DEV_STATUS_S *pstDevStatus);

RK_S32 RK_MPI_VI_StartPipe(VI_PIPE ViPipe);
RK_S32 RK_MPI_VI_StopPipe(VI_PIPE ViPipe);
/* 2 for vi chn */
RK_S32 RK_MPI_VI_SetChnAttr(VI_PIPE ViPipe, VI_CHN ViChn,
                            const VI_CHN_ATTR_S *pstChnAttr);
RK_S32 RK_MPI_VI_GetChnAttr(VI_PIPE ViPipe, VI_CHN ViChn,
                            VI_CHN_ATTR_S *pstChnAttr);

RK_S32 RK_MPI_VI_EnableChn(VI_PIPE ViPipe, VI_CHN ViChn);
RK_S32 RK_MPI_VI_EnableChnExt(VI_PIPE ViPipe, VI_CHN ViChn);
RK_S32 RK_MPI_VI_DisableChn(VI_PIPE ViPipe, VI_CHN ViChn);
RK_S32 RK_MPI_VI_DisableChnExt(VI_PIPE ViPipe, VI_CHN ViChn);
RK_S32 RK_MPI_VI_PauseChn(VI_PIPE ViPipe, VI_CHN ViChn);
RK_S32 RK_MPI_VI_ResumeChn(VI_PIPE ViPipe, VI_CHN ViChn);
RK_S32 RK_MPI_VI_GetChnFrame(VI_PIPE ViPipe, VI_CHN ViChn,
                             VIDEO_FRAME_INFO_S *pstFrameInfo,
                             RK_S32 s32MilliSec);
RK_S32 RK_MPI_VI_ReleaseChnFrame(VI_PIPE ViPipe, VI_CHN ViChn,
                                 const VIDEO_FRAME_INFO_S *pstFrameInfo);
RK_S32 RK_MPI_VI_ChnSaveFile(VI_PIPE ViPipe, VI_CHN ViChn,
                             VI_SAVE_FILE_INFO_S *pstSaveFileInfo);
RK_S32 RK_MPI_VI_QueryChnStatus(VI_PIPE ViPipe, VI_CHN ViChn,
                                VI_CHN_STATUS_S *pstChnStatus);
RK_S32 RK_MPI_VI_GetChnWrapBufAttr(VI_PIPE ViPipe, VI_CHN ViChn,
                                   VI_CHN_BUF_WRAP_S *pstViWrap);
RK_S32 RK_MPI_VI_SetChnWrapBufAttr(VI_PIPE ViPipe, VI_CHN ViChn,
                                   const VI_CHN_BUF_WRAP_S *pstViWrap);
RK_S32 RK_MPI_VI_GetChnFd(VI_PIPE ViPipe, VI_CHN ViChn);
RK_S32 RK_MPI_VI_CloseChnFd(VI_PIPE ViPipe, VI_CHN ViChn);

RK_S32 RK_MPI_VI_SetChnFreeze(VI_PIPE ViPipe, VI_CHN ViChn, RK_BOOL bFreeze);
RK_S32 RK_MPI_VI_GetChnFreeze(VI_PIPE ViPipe, VI_CHN ViChn, RK_BOOL *pbFreeze);

RK_S32 RK_MPI_VI_AttachMbPool(VI_PIPE ViPipe, VI_CHN ViChn, MB_POOL hMbPool);
RK_S32 RK_MPI_VI_DetachMbPool(VI_PIPE ViPipe, VI_CHN ViChn);

RK_S32 RK_MPI_VI_GetChnMirrorFlip(VI_PIPE ViPipe, VI_CHN ViChn,
                                  VI_ISP_MIRROR_FLIP_S *pstMirrFlip);
RK_S32 RK_MPI_VI_SetChnMirrorFlip(VI_PIPE ViPipe, VI_CHN ViChn,
                                  VI_ISP_MIRROR_FLIP_S stMirrFlip);

RK_S32 RK_MPI_VI_GetEptz(VI_PIPE ViPipe, VI_CHN ViChn,
                         VI_CROP_INFO_S *stCropInfo);
RK_S32 RK_MPI_VI_SetEptz(VI_PIPE ViPipe, VI_CHN ViChn,
                         VI_CROP_INFO_S stCropInfo);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* INCLUDE_RT_MPI_MPI_VI_H_ */
