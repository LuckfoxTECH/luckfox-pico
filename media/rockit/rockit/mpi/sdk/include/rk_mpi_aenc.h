/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_MPI_AENC_H_
#define INCLUDE_RT_MPI_MPI_AENC_H_

#include "rk_comm_aenc.h"
#include "rk_comm_aio.h"
#include "rk_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

RK_S32 RK_MPI_AENC_CreateChn(AENC_CHN AeChn, const AENC_CHN_ATTR_S *pstAttr);
RK_S32 RK_MPI_AENC_DestroyChn(AENC_CHN AeChn);
RK_S32 RK_MPI_AENC_SendFrame(AENC_CHN AeChn, const AUDIO_FRAME_S *pstFrm,
                             const AEC_FRAME_S *pstAecFrm, RK_S32 s32MilliSec);
RK_S32 RK_MPI_AENC_GetStream(AENC_CHN AeChn, AUDIO_STREAM_S *pstStream,
                             RK_S32 s32MilliSec);
RK_S32 RK_MPI_AENC_ReleaseStream(AENC_CHN AeChn,
                                 const AUDIO_STREAM_S *pstStream);
RK_S32 RK_MPI_AENC_SetMute(AENC_CHN AeChn, RK_BOOL pstMuteFlag);
RK_S32 RK_MPI_AENC_SaveFile(AENC_CHN AeChn,
                            const AUDIO_SAVE_FILE_INFO_S *pstSaveFileInfo);
RK_S32 RK_MPI_AENC_QueryFileStatus(AENC_CHN AeChn,
                                   AUDIO_FILE_STATUS_S *pstFileStatus);
RK_S32 RK_MPI_AENC_RegisterEncoder(RK_S32 *ps32Handle,
                                   const AENC_ENCODER_S *pstEncoder);
RK_S32 RK_MPI_AENC_UnRegisterEncoder(RK_S32 s32Handle);
RK_S32 RK_MPI_AENC_GetFd(AENC_CHN AeChn);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of INCLUDE_RT_MPI_MPI_AENC_H_ */
