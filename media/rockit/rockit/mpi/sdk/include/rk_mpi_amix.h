/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2022 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_MPI_AMIX_H_
#define INCLUDE_RT_MPI_MPI_AMIX_H_

#include "rk_comm_aio.h"
#include "rk_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

RK_S32 RK_MPI_AMIX_ListContents(AUDIO_DEV AmixDevId);
RK_S32 RK_MPI_AMIX_ListControls(AUDIO_DEV AmixDevId);
RK_S32 RK_MPI_AMIX_SetControl(AUDIO_DEV AmixDevId, const char *control,
                              char *values);
RK_S32 RK_MPI_AMIX_GetControl(AUDIO_DEV AmixDevId, const char *control,
                              char *values);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of INCLUDE_RT_MPI_MPI_AMIX_H_ */
