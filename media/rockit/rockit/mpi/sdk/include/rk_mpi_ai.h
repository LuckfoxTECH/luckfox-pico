/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_MPI_AI_H_
#define INCLUDE_RT_MPI_MPI_AI_H_

#include "rk_comm_aio.h"
#include "rk_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

RK_S32 RK_MPI_AI_SetPubAttr(AUDIO_DEV AiDevId, const AIO_ATTR_S *pstAttr);
RK_S32 RK_MPI_AI_GetPubAttr(AUDIO_DEV AiDevId, AIO_ATTR_S *pstAttr);
RK_S32 RK_MPI_AI_Enable(AUDIO_DEV AiDevId);
RK_S32 RK_MPI_AI_Disable(AUDIO_DEV AiDevId);
RK_S32 RK_MPI_AI_EnableChn(AUDIO_DEV AiDevId, AI_CHN AiChn);
RK_S32 RK_MPI_AI_DisableChn(AUDIO_DEV AiDevId, AI_CHN AiChn);
RK_S32 RK_MPI_AI_GetFrame(AUDIO_DEV AiDevId, AI_CHN AiChn,
                          AUDIO_FRAME_S *pstFrm, AEC_FRAME_S *pstAecFrm,
                          RK_S32 s32MilliSec);
RK_S32 RK_MPI_AI_ReleaseFrame(AUDIO_DEV AiDevId, AI_CHN AiChn,
                              const AUDIO_FRAME_S *pstFrm,
                              const AEC_FRAME_S *pstAecFrm);
RK_S32 RK_MPI_AI_SetChnParam(AUDIO_DEV AiDevId, AI_CHN AiChn,
                             const AI_CHN_PARAM_S *pstChnParam);
RK_S32 RK_MPI_AI_GetChnParam(AUDIO_DEV AiDevId, AI_CHN AiChn,
                             AI_CHN_PARAM_S *pstChnParam);
RK_S32 RK_MPI_AI_EnableReSmp(AUDIO_DEV AiDevId, AI_CHN AiChn,
                             AUDIO_SAMPLE_RATE_E enOutSampleRate);
RK_S32 RK_MPI_AI_DisableReSmp(AUDIO_DEV AiDevId, AI_CHN AiChn);
RK_S32 RK_MPI_AI_SetTrackMode(AUDIO_DEV AiDevId,
                              AUDIO_TRACK_MODE_E enTrackMode);
RK_S32 RK_MPI_AI_GetTrackMode(AUDIO_DEV AiDevId,
                              AUDIO_TRACK_MODE_E *penTrackMode);
RK_S32 RK_MPI_AI_SetVolumeCurve(AUDIO_DEV AiDevId,
                                AUDIO_VOLUME_CURVE_S *pvolumeCurve);
RK_S32 RK_MPI_AI_SetVolume(AUDIO_DEV AiDevId, RK_S32 s32VolumeDb);
RK_S32 RK_MPI_AI_GetVolume(AUDIO_DEV AiDevId, RK_S32 *ps32VolumeDb);
RK_S32 RK_MPI_AI_SetMute(AUDIO_DEV AiDevId, RK_BOOL bEnable,
                         const AUDIO_FADE_S *pstFade);
RK_S32 RK_MPI_AI_GetMute(AUDIO_DEV AiDevId, RK_BOOL *pbEnable,
                         AUDIO_FADE_S *pstFade);
RK_S32 RK_MPI_AI_SetChnAttr(AUDIO_DEV AiDevId, AI_CHN AiChn,
                            const AI_CHN_ATTR_S *pstAttrs);
RK_S32 RK_MPI_AI_GetChnAttr(AUDIO_DEV AiDevId, AI_CHN AiChn,
                            AI_CHN_ATTR_S *pstAttrs);
RK_S32 RK_MPI_AI_ClrPubAttr(AUDIO_DEV AiDevId);
RK_S32 RK_MPI_AI_SaveFile(AUDIO_DEV AiDevId, AI_CHN AiChn,
                          const AUDIO_SAVE_FILE_INFO_S *pstSaveFileInfo);
RK_S32 RK_MPI_AI_QueryFileStatus(AUDIO_DEV AiDevId, AI_CHN AiChn,
                                 AUDIO_FILE_STATUS_S *pstFileStatus);
RK_S32 RK_MPI_AI_GetFd(AUDIO_DEV AiDevId, AI_CHN AiChn);
RK_S32 RK_MPI_AI_SetVqeAttr(AUDIO_DEV AiDevId, AI_CHN AiChn, AUDIO_DEV AoDevId,
                            AO_CHN AoChn, const AI_VQE_CONFIG_S *pstVqeConfig);
RK_S32 RK_MPI_AI_GetVqeAttr(AUDIO_DEV AiDevId, AI_CHN AiChn,
                            AI_VQE_CONFIG_S *pstVqeConfig);
RK_S32 RK_MPI_AI_EnableVqe(AUDIO_DEV AiDevId, AI_CHN AiChn);
RK_S32 RK_MPI_AI_DisableVqe(AUDIO_DEV AiDevId, AI_CHN AiChn);

RK_S32 RK_MPI_AI_SetAedAttr(AUDIO_DEV AiDevId, AI_CHN AiChn,
                            const AI_AED_CONFIG_S *pstAedConfig);
RK_S32 RK_MPI_AI_GetAedAttr(AUDIO_DEV AiDevId, AI_CHN AiChn,
                            AI_AED_CONFIG_S *pstAedConfig);
RK_S32 RK_MPI_AI_EnableAed(AUDIO_DEV AiDevId, AI_CHN AiChn);
RK_S32 RK_MPI_AI_DisableAed(AUDIO_DEV AiDevId, AI_CHN AiChn);
RK_S32 RK_MPI_AI_GetAedResult(AUDIO_DEV AiDevId, AI_CHN AiChn,
                              AI_AED_RESULT_S *pstAedResult);

RK_S32 RK_MPI_AI_SetBcdAttr(AUDIO_DEV AiDevId, AI_CHN AiChn,
                            const AI_BCD_CONFIG_S *pstBcdConfig);
RK_S32 RK_MPI_AI_GetBcdAttr(AUDIO_DEV AiDevId, AI_CHN AiChn,
                            AI_BCD_CONFIG_S *pstBcdConfig);
RK_S32 RK_MPI_AI_EnableBcd(AUDIO_DEV AiDevId, AI_CHN AiChn);
RK_S32 RK_MPI_AI_DisableBcd(AUDIO_DEV AiDevId, AI_CHN AiChn);
RK_S32 RK_MPI_AI_GetBcdResult(AUDIO_DEV AiDevId, AI_CHN AiChn,
                              AI_BCD_RESULT_S *pstBcdResult);

RK_S32 RK_MPI_AI_SetBuzAttr(AUDIO_DEV AiDevId, AI_CHN AiChn,
                            const AI_BUZ_CONFIG_S *pstBuzConfig);
RK_S32 RK_MPI_AI_GetBuzAttr(AUDIO_DEV AiDevId, AI_CHN AiChn,
                            AI_BUZ_CONFIG_S *pstBuzConfig);
RK_S32 RK_MPI_AI_EnableBuz(AUDIO_DEV AiDevId, AI_CHN AiChn);
RK_S32 RK_MPI_AI_DisableBuz(AUDIO_DEV AiDevId, AI_CHN AiChn);
RK_S32 RK_MPI_AI_GetBuzResult(AUDIO_DEV AiDevId, AI_CHN AiChn,
                              AI_BUZ_RESULT_S *pstBuzResult);

RK_S32 RK_MPI_AI_SetGbsAttr(AUDIO_DEV AiDevId, AI_CHN AiChn,
                            const AI_GBS_CONFIG_S *pstGbsConfig);
RK_S32 RK_MPI_AI_GetGbsAttr(AUDIO_DEV AiDevId, AI_CHN AiChn,
                            AI_GBS_CONFIG_S *pstGbsConfig);
RK_S32 RK_MPI_AI_EnableGbs(AUDIO_DEV AiDevId, AI_CHN AiChn);
RK_S32 RK_MPI_AI_DisableGbs(AUDIO_DEV AiDevId, AI_CHN AiChn);
RK_S32 RK_MPI_AI_GetGbsResult(AUDIO_DEV AiDevId, AI_CHN AiChn,
                              AI_GBS_RESULT_S *pstGbsResult);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of INCLUDE_RT_MPI_MPI_AI_H_ */
