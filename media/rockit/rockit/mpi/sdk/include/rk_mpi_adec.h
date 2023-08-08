/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_MPI_ADEC_H_
#define INCLUDE_RT_MPI_MPI_ADEC_H_

#include "rk_comm_adec.h"
#include "rk_comm_aio.h"
#include "rk_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

RK_S32 RK_MPI_ADEC_CreateChn(ADEC_CHN AdChn, const ADEC_CHN_ATTR_S *pstAttr);
RK_S32 RK_MPI_ADEC_DestroyChn(ADEC_CHN AdChn);
RK_S32 RK_MPI_ADEC_SendStream(ADEC_CHN AdChn, const AUDIO_STREAM_S *pstStream,
                              RK_BOOL bBlock);
RK_S32 RK_MPI_ADEC_ClearChnBuf(ADEC_CHN AdChn);
RK_S32 RK_MPI_ADEC_GetFrame(ADEC_CHN AdChn, AUDIO_FRAME_INFO_S *pstFrmInfo,
                            RK_BOOL bBlock);
RK_S32 RK_MPI_ADEC_ReleaseFrame(ADEC_CHN AdChn, AUDIO_FRAME_INFO_S *pstFrmInfo);
RK_S32 RK_MPI_ADEC_SendEndOfStream(ADEC_CHN AdChn, RK_BOOL bInstant);
RK_S32 RK_MPI_ADEC_QueryChnStat(ADEC_CHN AdChn,
                                ADEC_CHN_STATE_S *pstBufferStatus);

RK_S32 RK_MPI_ADEC_RegisterDecoder(RK_S32 *ps32Handle,
                                   const ADEC_DECODER_S *pstDecoder);
RK_S32 RK_MPI_ADEC_UnRegisterDecoder(RK_S32 s32Handle);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of INCLUDE_RT_MPI_MPI_ADEC_H_ */
