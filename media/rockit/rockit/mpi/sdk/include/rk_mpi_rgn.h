/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_RK_MPI_RGN_H__
#define INCLUDE_RT_MPI_RK_MPI_RGN_H__

#include "rk_comm_rgn.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

RK_S32 RK_MPI_RGN_Create(RGN_HANDLE Handle, const RGN_ATTR_S *pstRegion);
RK_S32 RK_MPI_RGN_Destroy(RGN_HANDLE Handle);

RK_S32 RK_MPI_RGN_GetAttr(RGN_HANDLE Handle, RGN_ATTR_S *pstRegion);
RK_S32 RK_MPI_RGN_SetAttr(RGN_HANDLE Handle, const RGN_ATTR_S *pstRegion);

RK_S32 RK_MPI_RGN_SetBitMap(RGN_HANDLE Handle, const BITMAP_S *pstBitmap);

RK_S32 RK_MPI_RGN_AttachToChn(RGN_HANDLE Handle, const MPP_CHN_S *pstChn,
                              const RGN_CHN_ATTR_S *pstChnAttr);
RK_S32 RK_MPI_RGN_DetachFromChn(RGN_HANDLE Handle, const MPP_CHN_S *pstChn);

RK_S32 RK_MPI_RGN_SetDisplayAttr(RGN_HANDLE Handle, const MPP_CHN_S *pstChn,
                                 const RGN_CHN_ATTR_S *pstChnAttr);
RK_S32 RK_MPI_RGN_GetDisplayAttr(RGN_HANDLE Handle, const MPP_CHN_S *pstChn,
                                 RGN_CHN_ATTR_S *pstChnAttr);

RK_S32 RK_MPI_RGN_GetCanvasInfo(RGN_HANDLE Handle,
                                RGN_CANVAS_INFO_S *pstCanvasInfo);
RK_S32 RK_MPI_RGN_UpdateCanvas(RGN_HANDLE Handle);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* INCLUDE_RT_MPI_RK_MPI_RGN_H__ */
