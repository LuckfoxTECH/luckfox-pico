/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_RK_MPI_IVS_H_
#define INCLUDE_RT_MPI_RK_MPI_IVS_H_

#include "rk_comm_ivs.h"
#include "rk_comm_video.h"
#include "rk_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

RK_S32 RK_MPI_IVS_Init(RK_VOID);

RK_S32 RK_MPI_IVS_Exit(RK_VOID);

RK_S32 RK_MPI_IVS_CreateChn(IVS_CHN IvsChn, IVS_CHN_ATTR_S *pstAttr);

RK_S32 RK_MPI_IVS_DestroyChn(IVS_CHN IvsChn);

RK_S32 RK_MPI_IVS_SetChnAttr(IVS_CHN IvsChn, IVS_CHN_ATTR_S *pstAttr);

RK_S32 RK_MPI_IVS_GetChnAttr(IVS_CHN IvsChn, IVS_CHN_ATTR_S *pstAttr);

RK_S32 RK_MPI_IVS_SetMdAttr(IVS_CHN IvsChn, IVS_MD_ATTR_S *pstMdAttr);

RK_S32 RK_MPI_IVS_GetMdAttr(IVS_CHN IvsChn, IVS_MD_ATTR_S *pstMdAttr);

RK_S32 RK_MPI_IVS_SendFrame(IVS_CHN VdChn, const VIDEO_FRAME_INFO_S *pstFrame,
                            RK_S32 s32MilliSec);

RK_S32 RK_MPI_IVS_GetResults(IVS_CHN VdChn, IVS_RESULT_INFO_S *pstResults,
                             RK_S32 s32MilliSec);
RK_S32 RK_MPI_IVS_ReleaseResults(IVS_CHN IvsChn, IVS_RESULT_INFO_S *pstResults);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif // INCLUDE_RT_MPI_RK_MPI_IVS_H_
