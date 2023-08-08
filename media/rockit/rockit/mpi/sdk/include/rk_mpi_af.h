/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_MPI_AF_H_
#define INCLUDE_RT_MPI_MPI_AF_H_

#include <string>
#include <vector>

#include "rk_comm_af.h"
#include "rk_comm_aio.h"
#include "rk_common.h"
#include "rk_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

RK_S32 RK_MPI_AF_Create(AF_CHN AfChn, const AF_ATTR_S *pstAttr);
RK_S32 RK_MPI_AF_Destroy(AF_CHN AfChn);
RK_S32 RK_MPI_AF_SendFrame(AF_CHN AfChn, AUDIO_FRAME_INFO_S *frame,
                           RK_S32 s32MilliSec);
RK_S32 RK_MPI_AF_GetFrame(AF_CHN AfChn, AUDIO_FRAME_INFO_S *frame,
                          RK_S32 s32MilliSec);
RK_S32 RK_MPI_AF_ReleaseFrame(AF_CHN AfChn, AUDIO_FRAME_INFO_S *frame);
RK_S32 RK_MPI_AF_SetChnAttr(AF_CHN AfChn, const AF_ATTR_S *pstAttr);
RK_S32 RK_MPI_AF_GetChnAttr(AF_CHN AfChn, AF_ATTR_S *pstAttr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif // INCLUDE_RT_MPI_MPI_AF_H_