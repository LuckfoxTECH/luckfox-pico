/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_MPI_AO_H_
#define INCLUDE_RT_MPI_MPI_AO_H_

#include "rk_comm_aio.h"
#include "rk_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

RK_S32 RK_MPI_AO_SetPubAttr(AUDIO_DEV AoDevId, const AIO_ATTR_S *pstAttr);
RK_S32 RK_MPI_AO_GetPubAttr(AUDIO_DEV AoDevId, AIO_ATTR_S *pstAttr);
RK_S32 RK_MPI_AO_Enable(AUDIO_DEV AoDevId);
RK_S32 RK_MPI_AO_Disable(AUDIO_DEV AoDevId);
RK_S32 RK_MPI_AO_EnableChn(AUDIO_DEV AoDevId, AO_CHN AoChn);
RK_S32 RK_MPI_AO_DisableChn(AUDIO_DEV AoDevId, AO_CHN AoChn);
RK_S32 RK_MPI_AO_SendFrame(AUDIO_DEV AoDevId, AO_CHN AoChn,
                           const AUDIO_FRAME_S *pstData, RK_S32 s32MilliSec);
RK_S32 RK_MPI_AO_EnableReSmp(AUDIO_DEV AoDevId, AO_CHN AoChn,
                             AUDIO_SAMPLE_RATE_E enInSampleRate);
RK_S32 RK_MPI_AO_DisableReSmp(AUDIO_DEV AoDevId, AO_CHN AoChn);
RK_S32 RK_MPI_AO_PauseChn(AUDIO_DEV AoDevId, AO_CHN AoChn);
RK_S32 RK_MPI_AO_ResumeChn(AUDIO_DEV AoDevId, AO_CHN AoChn);
RK_S32 RK_MPI_AO_ClearChnBuf(AUDIO_DEV AoDevId, AO_CHN AoChn);
RK_S32 RK_MPI_AO_QueryChnStat(AUDIO_DEV AoDevId, AO_CHN AoChn,
                              AO_CHN_STATE_S *pstStatus);
RK_S32 RK_MPI_AO_SetTrackMode(AUDIO_DEV AoDevId,
                              AUDIO_TRACK_MODE_E enTrackMode);
RK_S32 RK_MPI_AO_GetTrackMode(AUDIO_DEV AoDevId,
                              AUDIO_TRACK_MODE_E *penTrackMode);
RK_S32 RK_MPI_AO_SetVolumeCurve(AUDIO_DEV AoDevId,
                                AUDIO_VOLUME_CURVE_S *pvolumeCurve);
RK_S32 RK_MPI_AO_SetVolume(AUDIO_DEV AoDevId, RK_S32 s32VolumeDb);
RK_S32 RK_MPI_AO_GetVolume(AUDIO_DEV AoDevId, RK_S32 *ps32VolumeDb);
RK_S32 RK_MPI_AO_SetMute(AUDIO_DEV AoDevId, RK_BOOL bEnable,
                         const AUDIO_FADE_S *pstFade);
RK_S32 RK_MPI_AO_GetMute(AUDIO_DEV AoDevId, RK_BOOL *pbEnable,
                         AUDIO_FADE_S *pstFade);
RK_S32 RK_MPI_AO_SetChnAttr(AUDIO_DEV AoDevId, AO_CHN AoChn,
                            const AO_CHN_ATTR_S *pstAttrs);
RK_S32 RK_MPI_AO_GetChnAttr(AUDIO_DEV AoDevId, AO_CHN AoChn,
                            AO_CHN_ATTR_S *pstAttrs);
RK_S32 RK_MPI_AO_SetChnParams(AUDIO_DEV AoDevId, AO_CHN AoChn,
                              const AO_CHN_PARAM_S *pstParams);
RK_S32 RK_MPI_AO_GetChnParams(AUDIO_DEV AoDevId, AO_CHN AoChn,
                              AO_CHN_PARAM_S *pstParams);
// RK_S32 RK_MPI_AO_GetFd(AUDIO_DEV AoDevId, AO_CHN AoChn);
RK_S32 RK_MPI_AO_SaveFile(AUDIO_DEV AoDevId, AO_CHN AoChn,
                          AUDIO_SAVE_FILE_INFO_S *pstSaveFileInfo);
RK_S32 RK_MPI_AO_QueryFileStatus(AUDIO_DEV AoDevId, AO_CHN AoChn,
                                 AUDIO_FILE_STATUS_S *pstFileStatus);
RK_S32 RK_MPI_AO_ClrPubAttr(AUDIO_DEV AoDevId);
RK_S32 RK_MPI_AO_EnableVqe(AUDIO_DEV AoDevId, AO_CHN AoChn);
RK_S32 RK_MPI_AO_DisableVqe(AUDIO_DEV AoDevId, AO_CHN AoChn);
RK_S32 RK_MPI_AO_SetVqeAttr(AUDIO_DEV AoDevId, AO_CHN AoChn,
                            AO_VQE_CONFIG_S *pstVqeConfig);
RK_S32 RK_MPI_AO_GetVqeAttr(AUDIO_DEV AoDevId, AO_CHN AoChn,
                            AO_VQE_CONFIG_S *pstVqeConfig);
RK_S32 RK_MPI_AO_WaitEos(AUDIO_DEV AoDevId, AO_CHN AoChn, RK_S32 s32MilliSec);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of INCLUDE_RT_MPI_MPI_AO_H_ */
