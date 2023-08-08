/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_MPI_VO_H_
#define INCLUDE_RT_MPI_MPI_VO_H_

#include "rk_comm_video.h"
#include "rk_comm_vo.h"
#include "rk_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* Device Settings */
RK_S32 RK_MPI_VO_SetPubAttr(VO_DEV VoDev, const VO_PUB_ATTR_S *pstPubAttr);
RK_S32 RK_MPI_VO_GetPubAttr(VO_DEV VoDev, VO_PUB_ATTR_S *pstPubAttr);

RK_S32 RK_MPI_VO_GetPostProcessParam(VO_DEV VoDev, VO_CSC_S *pstParam);
RK_S32 RK_MPI_VO_SetPostProcessParam(VO_DEV VoDev, VO_CSC_S *pstParam);

RK_S32 RK_MPI_VO_Enable(VO_DEV VoDev);
RK_S32 RK_MPI_VO_Disable(VO_DEV VoDev);

/* Interface Setting */
RK_S32 RK_MPI_VO_GetHdmiParam(RK_U32 enIntfType, RK_U32 u32Id,
                              VO_HDMI_PARAM_S *pstHDMIParam);
RK_S32 RK_MPI_VO_SetHdmiParam(RK_U32 enIntfType, RK_U32 u32Id,
                              const VO_HDMI_PARAM_S *pstHDMIParam);

RK_S32 RK_MPI_VO_Get_Edid(RK_U32 enIntfType, RK_U32 u32Id,
                          VO_EDID_S *pstEdidData);
RK_S32 RK_MPI_VO_GetSinkCapability(RK_U32 enIntfType, RK_U32 u32Id,
                                   VO_SINK_CAPABILITY_S *pstSinkCap);

RK_S32 RK_MPI_VO_RegCallbackFunc(RK_U32 enIntfType, RK_U32 u32Id,
                                 RK_VO_CALLBACK_FUNC_S *pstCallbackFunc);
RK_S32 RK_MPI_VO_UnRegCallbackFunc(RK_U32 enIntfType, RK_U32 u32Id,
                                   RK_VO_CALLBACK_FUNC_S *pstCallbackFunc);

RK_S32 RK_MPI_VO_CloseFd(RK_VOID);

/* Framebuffer Settings */
RK_S32 RK_MPI_VO_CreateGraphicsFrameBuffer(RK_U32 Width, RK_U32 Height,
                                           RK_U32 Format, RK_VOID **fd);
RK_S32 RK_MPI_VO_DestroyGraphicsFrameBuffer(RK_VOID *fd);
RK_VOID *RK_MPI_VO_GetGraphicsFrameBuffer(RK_VOID *fd);
RK_S32 RK_MPI_VO_GetFrameInfo(RK_VOID *pMblk, VO_FRAME_INFO_S *pstFrame);

/* Pre-created framebuffer mode*/
RK_S32 RK_MPI_VO_SetGFxMode(VO_GFX_MODE_E u32Mode);
RK_S32 RK_MPI_VO_GetGfxFrameBuffer(VO_LAYER VoLayer, VO_CHN VoChn,
                                   VO_FRAME_INFO_S *pstFrame);
RK_S32 RK_MPI_VO_UpdateGfxFrameBuffer(VO_LAYER VoLayer, VO_CHN VoChn,
                                      VO_FRAME_INFO_S *pstFrame);
RK_S32 RK_MPI_VO_ReleaseGfxFrameBuffer(VO_LAYER VoLayer, VO_CHN VoChn);

RK_S32 RK_MPI_VO_ClearLayersBinding(RK_VOID);

/* Layers Settings */
RK_S32 RK_MPI_VO_BindLayer(VO_LAYER VoLayer, VO_DEV VoDev,
                           VO_LAYER_MODE_E Mode);
RK_S32 RK_MPI_VO_UnBindLayer(VO_LAYER VoLayer, VO_DEV VoDev);

RK_S32 RK_MPI_VO_SetLayerAttr(VO_LAYER VoLayer,
                              const VO_VIDEO_LAYER_ATTR_S *pstLayerVideoAttr);
RK_S32 RK_MPI_VO_GetLayerAttr(VO_LAYER VoLayer,
                              VO_VIDEO_LAYER_ATTR_S *pstLayerAttr);

RK_S32 RK_MPI_VO_EnableLayer(VO_LAYER VoLayer);
RK_S32 RK_MPI_VO_DisableLayer(VO_LAYER VoLayer);

RK_S32 RK_MPI_VO_SetLayerPriority(VO_LAYER VoLayer, RK_U32 u32Priority);
RK_S32 RK_MPI_VO_GetLayerPriority(VO_LAYER VoLayer, RK_U32 *pu32Priority);

RK_S32 RK_MPI_VO_SetLayerCSC(VO_LAYER VoLayer, const VO_CSC_S *pstVideoCSC);
RK_S32 RK_MPI_VO_GetLayerCSC(VO_LAYER VoLayer, VO_CSC_S *pstVideoCSC);

RK_S32 RK_MPI_VO_SetLayerDispBufLen(VO_LAYER VoLayer, RK_U32 u32BufLen);
RK_S32 RK_MPI_VO_GetLayerDispBufLen(VO_LAYER VoLayer, RK_U32 *pu32BufLen);

RK_S32 RK_MPI_VO_GetLayerFrame(VO_LAYER VoLayer, VIDEO_FRAME_INFO_S *pstVFrame,
                               RK_S32 s32MilliSec);
RK_S32 RK_MPI_VO_ReleaseLayerFrame(VO_LAYER VoLayer,
                                   VIDEO_FRAME_INFO_S *pstVFrame);

RK_S32 RK_MPI_VO_SendLayerFrame(VO_LAYER VoLayer,
                                VIDEO_FRAME_INFO_S *pstVFrame);
RK_S32 RK_MPI_VO_SetCursorPostion(VO_LAYER VoLayer, const RK_U32 x,
                                  const RK_U32 y);
RK_S32 RK_MPI_VO_SetCursorLogicalRect(VO_LAYER VoLayer, const RK_U32 maxWidth,
                                      const RK_U32 maxHeight);

/* General Operation of Channel */
RK_S32 RK_MPI_VO_EnableChn(VO_LAYER VoLayer, VO_CHN VoChn);
RK_S32 RK_MPI_VO_DisableChn(VO_LAYER VoLayer, VO_CHN VoChn);

RK_S32 RK_MPI_VO_SetChnAttr(VO_LAYER VoLayer, VO_CHN VoChn,
                            const VO_CHN_ATTR_S *pstChnAttr);
RK_S32 RK_MPI_VO_GetChnAttr(VO_LAYER VoLayer, VO_CHN VoChn,
                            VO_CHN_ATTR_S *pstChnAttr);

RK_S32 RK_MPI_VO_SetChnParam(VO_LAYER VoLayer, VO_CHN VoChn,
                             const VO_CHN_PARAM_S *pstChnParam);
RK_S32 RK_MPI_VO_GetChnParam(VO_LAYER VoLayer, VO_CHN VoChn,
                             VO_CHN_PARAM_S *pstChnParam);

RK_S32 RK_MPI_VO_SetChnDispPos(VO_LAYER VoLayer, VO_CHN VoChn,
                               const POINT_S *pstDispPos);
RK_S32 RK_MPI_VO_GetChnDispPos(VO_LAYER VoLayer, VO_CHN VoChn,
                               POINT_S *pstDispPos);

RK_S32 RK_MPI_VO_GetChnFrame(VO_LAYER VoLayer, VO_CHN VoChn,
                             VIDEO_FRAME_INFO_S *pstFrame, RK_S32 s32MilliSec);
RK_S32 RK_MPI_VO_ReleaseChnFrame(VO_LAYER VoLayer, VO_CHN VoChn,
                                 const VIDEO_FRAME_INFO_S *pstFrame);

RK_S32 RK_MPI_VO_SetChnFrameRate(VO_LAYER VoLayer, VO_CHN VoChn,
                                 RK_S32 s32ChnFrmRate);
RK_S32 RK_MPI_VO_GetChnFrameRate(VO_LAYER VoLayer, VO_CHN VoChn,
                                 RK_S32 *ps32ChnFrmRate);

RK_S32 RK_MPI_VO_PauseChn(VO_LAYER VoLayer, VO_CHN VoChn);
RK_S32 RK_MPI_VO_ResumeChn(VO_LAYER VoLayer, VO_CHN VoChn);
RK_S32 RK_MPI_VO_StepChn(VO_LAYER VoLayer, VO_CHN VoChn);
RK_S32 RK_MPI_VO_RefreshChn(VO_LAYER VoLayer, VO_CHN VoChn);

RK_S32 RK_MPI_VO_ShowChn(VO_LAYER VoLayer, VO_CHN VoChn);
RK_S32 RK_MPI_VO_HideChn(VO_LAYER VoLayer, VO_CHN VoChn);

RK_S32 RK_MPI_VO_SendFrame(VO_LAYER VoLayer, VO_CHN VoChn,
                           VIDEO_FRAME_INFO_S *pstVFrame, RK_S32 s32MilliSec);

RK_S32 RK_MPI_VO_ClearChnBuffer(VO_LAYER VoLayer, VO_CHN VoChn,
                                RK_BOOL bClrAll);

RK_S32 RK_MPI_VO_SetChnBorder(VO_LAYER VoLayer, VO_CHN VoChn,
                              const VO_BORDER_S *pstBorder);
RK_S32 RK_MPI_VO_GetChnBorder(VO_LAYER VoLayer, VO_CHN VoChn,
                              VO_BORDER_S *pstBorder);

RK_S32 RK_MPI_VO_GetChnPts(VO_LAYER VoLayer, VO_CHN VoChn, RK_U64 *pu64ChnPts);
RK_S32 RK_MPI_VO_QueryChnStat(VO_LAYER VoLayer, VO_CHN VoChn,
                              VO_QUERY_STATUS_S *pstStatus);

/* WBC setting */
RK_S32 RK_MPI_VO_SetWbcSource(VO_WBC VoWbc,
                              const VO_WBC_SOURCE_S *pstWbcSource);
RK_S32 RK_MPI_VO_GetWbcSource(VO_WBC VoWbc, VO_WBC_SOURCE_S *pstWbcSources);

RK_S32 RK_MPI_VO_EnableWbc(VO_WBC VoWbc);
RK_S32 RK_MPI_VO_DisableWbc(VO_WBC VoWbc);

RK_S32 RK_MPI_VO_SetWbcAttr(VO_WBC VoWbc, const VO_WBC_ATTR_S *pstWbcAttr);
RK_S32 RK_MPI_VO_GetWbcAttr(VO_WBC VoWbc, VO_WBC_ATTR_S *pstWbcAttr);

RK_S32 RK_MPI_VO_SetWbcMode(VO_WBC VoWbc, VO_WBC_MODE_E enWbcMode);
RK_S32 RK_MPI_VO_GetWbcMode(VO_WBC VoWbc, VO_WBC_MODE_E *penWbcMode);

RK_S32 RK_MPI_VO_SetWbcDepth(VO_WBC VoWbc, RK_S32 u32Depth);
RK_S32 RK_MPI_VO_GetWbcDepth(VO_WBC VoWbc, RK_S32 *pu32Depth);

RK_S32 RK_MPI_VO_GetWbcFrame(VO_WBC VoWbc, VIDEO_FRAME_INFO_S *pstVFrame,
                             RK_S32 s32MilliSec);
RK_S32 RK_MPI_VO_ReleaseWbcFrame(VO_WBC VoWbc, VIDEO_FRAME_INFO_S *pstVFrame);

/* splice type setting */
RK_S32 RK_MPI_VO_SetLayerSpliceMode(VO_LAYER VoLayer,
                                    VO_SPLICE_MODE_E enSpliceMode);
RK_S32 RK_MPI_VO_GetLayerSpliceMode(VO_LAYER VoLayer,
                                    VO_SPLICE_MODE_E *enSpliceMode);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*INCLUDE_RT_MPI_MPI_VO_H_ */
