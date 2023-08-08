/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_RK_MPI_BUFFER_H_
#define INCLUDE_RT_MPI_RK_MPI_BUFFER_H_

#include "rk_comm_video.h"
#include "rk_common.h"
#include "rk_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

RK_S32 RK_MPI_CAL_COMM_GetPicBufferSize(const PIC_BUF_ATTR_S *pstBufAttr,
                                        MB_PIC_CAL_S *pstPicCal);
RK_S32 RK_MPI_CAL_TDE_GetPicBufferSize(const PIC_BUF_ATTR_S *pstBufAttr,
                                       MB_PIC_CAL_S *pstPicCal);
RK_S32 RK_MPI_CAL_VGS_GetPicBufferSize(const PIC_BUF_ATTR_S *pstBufAttr,
                                       MB_PIC_CAL_S *pstPicCal);
RK_S32 RK_MPI_CAL_VDEC_GetPicBufferSize(const VDEC_PIC_BUF_ATTR_S *pstBufAttr,
                                        MB_PIC_CAL_S *pstPicCal);

RK_U32 RK_MPI_CAL_VGS_GetPicVirWidth(RK_U32 u32Width,
                                     PIXEL_FORMAT_E enPixelFormat);
RK_U32 RK_MPI_CAL_VGS_GetPicVirHeight(RK_U32 u32Height,
                                      PIXEL_FORMAT_E enPixelFormat);

RK_U32 RK_MPI_CAL_VDEC_GetVirWidth(const VDEC_PIC_BUF_ATTR_S *pstBufAttr);
RK_U32 RK_MPI_CAL_VDEC_GetVirHeight(const VDEC_PIC_BUF_ATTR_S *pstBufAttr);

RK_U32 RK_MPI_CAL_COMM_GetHorStride(RK_U32 u32VirWidth,
                                    PIXEL_FORMAT_E enPixelFormat);
RK_U32 RK_MPI_CAL_COMM_GetVirWidth(RK_U32 u32HorStride,
                                   PIXEL_FORMAT_E enPixelFormat);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif // INCLUDE_RT_MPI_RK_MPI_BUFFER_H_
