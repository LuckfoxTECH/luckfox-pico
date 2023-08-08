/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef SRC_RT_MPI_INCLUDE_RK_MPI_TDE_H_
#define SRC_RT_MPI_INCLUDE_RK_MPI_TDE_H_

#include "rk_comm_tde.h"
#include "rk_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

RK_S32 RK_TDE_Open(RK_VOID);

RK_VOID RK_TDE_Close(RK_VOID);

TDE_HANDLE RK_TDE_BeginJob(RK_VOID);

RK_S32 RK_TDE_EndJob(TDE_HANDLE s32Handle, RK_BOOL bSync, RK_BOOL bBlock,
                     RK_U32 u32TimeOut);

RK_S32 RK_TDE_CancelJob(TDE_HANDLE s32Handle);

RK_S32 RK_TDE_WaitForDone(TDE_HANDLE s32Handle);

RK_S32 RK_TDE_WaitAllDone(RK_VOID);

RK_S32 RK_TDE_QuickCopy(TDE_HANDLE s32Handle, const TDE_SURFACE_S *pstSrc,
                        const TDE_RECT_S *pstSrcRect,
                        const TDE_SURFACE_S *pstDst,
                        const TDE_RECT_S *pstDstRect);

RK_S32 RK_TDE_QuickResize(TDE_HANDLE s32Handle, const TDE_SURFACE_S *pstSrc,
                          const TDE_RECT_S *pstSrcRect,
                          const TDE_SURFACE_S *pstDst,
                          const TDE_RECT_S *pstDstRect);

RK_S32 RK_TDE_Bitblit(TDE_HANDLE s32Handle, const TDE_SURFACE_S *pstBackGround,
                      const TDE_RECT_S *pstBackGroundRect,
                      const TDE_SURFACE_S *pstForeGround,
                      const TDE_RECT_S *pstForeGroundRect,
                      const TDE_SURFACE_S *pstDst, const TDE_RECT_S *pstDstRect,
                      const TDE_OPT_S *pstOpt);

RK_S32 RK_TDE_QuickFill(TDE_HANDLE s32Handle, TDE_SURFACE_S *pstDst,
                        TDE_RECT_S *pstDstRect, RK_U32 u32FillData);

RK_S32 RK_TDE_Rotate(TDE_HANDLE s32Handle, TDE_SURFACE_S *pstSrc,
                     TDE_RECT_S *pstSrcRect, TDE_SURFACE_S *pstDst,
                     TDE_RECT_S *pstDstRect, ROTATION_E enRotateAngle);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRC_RT_MPI_INCLUDE_RK_MPI_TDE_H_ */
