/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_MPI_VENC_H_
#define INCLUDE_RT_MPI_MPI_VENC_H_

#include "rk_comm_venc.h"
#include "rk_comm_video.h"
#include "rk_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

RK_S32 RK_MPI_VENC_CreateChn(VENC_CHN VeChn, const VENC_CHN_ATTR_S *pstAttr);
RK_S32 RK_MPI_VENC_DestroyChn(VENC_CHN VeChn);
RK_S32 RK_MPI_VENC_ResetChn(VENC_CHN VeChn);
RK_S32 RK_MPI_VENC_SetSceneMode(VENC_CHN VeChn,
                                const VENC_SCENE_MODE_E enSceneMode);
RK_S32 RK_MPI_VENC_GetSceneMode(VENC_CHN VeChn,
                                VENC_SCENE_MODE_E *penSceneMode);
RK_S32 RK_MPI_VENC_StartRecvFrame(VENC_CHN VeChn,
                                  const VENC_RECV_PIC_PARAM_S *pstRecvParam);
RK_S32 RK_MPI_VENC_StopRecvFrame(VENC_CHN VeChn);
RK_S32 RK_MPI_VENC_QueryStatus(VENC_CHN VeChn, VENC_CHN_STATUS_S *pstStatus);
RK_S32 RK_MPI_VENC_EnableSvc(VENC_CHN VeChn, RK_BOOL bEnable);
RK_S32 RK_MPI_VENC_EnableMotionDeblur(VENC_CHN VeChn, RK_BOOL bEnable);
RK_S32 RK_MPI_VENC_EnableMotionStaticSwitch(VENC_CHN VeChn, RK_BOOL bEnable);
RK_S32 RK_MPI_VENC_SetChnAttr(VENC_CHN VeChn,
                              const VENC_CHN_ATTR_S *pstChnAttr);
RK_S32 RK_MPI_VENC_GetChnAttr(VENC_CHN VeChn, VENC_CHN_ATTR_S *pstChnAttr);
RK_S32
RK_MPI_VENC_SetChnBufWrapAttr(VENC_CHN VeChn,
                              const VENC_CHN_BUF_WRAP_S *pstVencChnBufWrap);
RK_S32 RK_MPI_VENC_GetChnBufWrapAttr(VENC_CHN VeChn,
                                     VENC_CHN_BUF_WRAP_S *pstVencChnBufWrap);
RK_S32 RK_MPI_VENC_SetChnRefBufShareAttr(
    VENC_CHN VeChn, const VENC_CHN_REF_BUF_SHARE_S *pstVencChnRefBufShare);
RK_S32 RK_MPI_VENC_GetChnRefBufShareAttr(
    VENC_CHN VeChn, VENC_CHN_REF_BUF_SHARE_S *pstVencChnRefBufShare);
RK_S32 RK_MPI_VENC_SetComboAttr(VENC_CHN VeChn,
                                VENC_COMBO_ATTR_S *pstComboAttr);
RK_S32 RK_MPI_VENC_GetComboAttr(VENC_CHN VeChn,
                                VENC_COMBO_ATTR_S *pstComboAttr);
RK_S32 RK_MPI_VENC_EnableThumbnail(VENC_CHN VeChn);
RK_S32 RK_MPI_VENC_ThumbnailBind(VENC_CHN VeChn, VENC_CHN VeChnTb);
RK_S32 RK_MPI_VENC_ThumbnailRequest(VENC_CHN VeChn);
RK_S32 RK_MPI_VENC_SetChnParam(VENC_CHN VeChn,
                               const VENC_CHN_PARAM_S *pstChnParam);
RK_S32 RK_MPI_VENC_GetChnParam(VENC_CHN VeChn, VENC_CHN_PARAM_S *pstChnParam);
RK_S32 RK_MPI_VENC_SendFrame(VENC_CHN VeChn, const VIDEO_FRAME_INFO_S *pstFrame,
                             RK_S32 s32MilliSec);
RK_S32 RK_MPI_VENC_SendFrameEx(VENC_CHN VeChn,
                               const USER_FRAME_INFO_S *pstFrame,
                               RK_S32 s32MilliSec);
RK_S32 RK_MPI_VENC_GetStream(VENC_CHN VeChn, VENC_STREAM_S *pstStream,
                             RK_S32 s32MilliSec);
RK_S32 RK_MPI_VENC_ReleaseStream(VENC_CHN VeChn, VENC_STREAM_S *pstStream);
RK_S32 RK_MPI_VENC_RequestIDR(VENC_CHN VeChn, RK_BOOL bInstant);
RK_S32 RK_MPI_VENC_SetRoiAttr(VENC_CHN VeChn,
                              const VENC_ROI_ATTR_S *pstRoiAttr);
RK_S32 RK_MPI_VENC_GetRoiAttr(VENC_CHN VeChn, RK_U32 u32Index,
                              VENC_ROI_ATTR_S *pstRoiAttr);
RK_S32 RK_MPI_VENC_SetRcParam(VENC_CHN VeChn,
                              const VENC_RC_PARAM_S *pstRcParam);
RK_S32 RK_MPI_VENC_GetRcParam(VENC_CHN VeChn, VENC_RC_PARAM_S *pstRcParam);
RK_S32 RK_MPI_VENC_SetRcAdvParam(VENC_CHN VeChn,
                                 const VENC_RC_ADVPARAM_S *pstRcAdvParam);
RK_S32 RK_MPI_VENC_GetRcAdvParam(VENC_CHN VeChn,
                                 VENC_RC_ADVPARAM_S *pstRcAdvParam);
RK_S32
RK_MPI_VENC_SetFrameLostStrategy(VENC_CHN VeChn,
                                 const VENC_FRAMELOST_S *pstFrmLostParam);
RK_S32 RK_MPI_VENC_GetFrameLostStrategy(VENC_CHN VeChn,
                                        VENC_FRAMELOST_S *pstFrmLostParam);
RK_S32 RK_MPI_VENC_SetSuperFrameStrategy(
    VENC_CHN VeChn, const VENC_SUPERFRAME_CFG_S *pstSuperFrmParam);
RK_S32
RK_MPI_VENC_GetSuperFrameStrategy(VENC_CHN VeChn,
                                  VENC_SUPERFRAME_CFG_S *pstSuperFrmParam);
RK_S32 RK_MPI_VENC_SetIntraRefresh(VENC_CHN VeChn,
                                   const VENC_INTRA_REFRESH_S *pstIntraRefresh);
RK_S32 RK_MPI_VENC_GetIntraRefresh(VENC_CHN VeChn,
                                   VENC_INTRA_REFRESH_S *pstIntraRefresh);
RK_S32
RK_MPI_VENC_SetHierarchicalQp(VENC_CHN VeChn,
                              const VENC_HIERARCHICAL_QP_S *pstHierarchicalQp);
RK_S32 RK_MPI_VENC_GetHierarchicalQp(VENC_CHN VeChn,
                                     VENC_HIERARCHICAL_QP_S *pstHierarchicalQp);
RK_S32
RK_MPI_VENC_SetDeBreathEffect(VENC_CHN VeChn,
                              const VENC_DEBREATHEFFECT_S *pstDeBreathEffect);
RK_S32 RK_MPI_VENC_GetDeBreathEffect(VENC_CHN VeChn,
                                     VENC_DEBREATHEFFECT_S *pstDeBreathEffect);
RK_S32 RK_MPI_VENC_SetJpegParam(VENC_CHN VeChn,
                                const VENC_JPEG_PARAM_S *pstJpegParam);
RK_S32 RK_MPI_VENC_GetFd(VENC_CHN VeChn);
RK_S32 RK_MPI_VENC_CloseFd(VENC_CHN VeChn);
RK_S32 RK_MPI_VENC_InsertUserData(VENC_CHN VeChn, RK_U8 *pu8Data,
                                  RK_U32 u32Len);
RK_S32 RK_MPI_VENC_SetChnRotation(VENC_CHN VeChn, ROTATION_E enRotation);
RK_S32 RK_MPI_VENC_GetChnRotation(VENC_CHN VeChn, ROTATION_E *enRotation);
RK_S32 RK_MPI_VENC_SetQpmap(VENC_CHN VeChn, const MB_BLK blk);
RK_S32 RK_MPI_VENC_GetQpmap(VENC_CHN VeChn, MB_BLK *pBlk);
RK_S32 RK_MPI_VENC_AttachMbPool(VENC_CHN VeChn, MB_POOL hMbPool);
RK_S32 RK_MPI_VENC_DetachMbPool(VENC_CHN VeChn);

// H264
RK_S32
RK_MPI_VENC_SetH264IntraPred(VENC_CHN VeChn,
                             const VENC_H264_INTRA_PRED_S *pstH264IntraPred);
RK_S32 RK_MPI_VENC_GetH264IntraPred(VENC_CHN VeChn,
                                    VENC_H264_INTRA_PRED_S *pstH264IntraPred);
RK_S32 RK_MPI_VENC_SetH264Trans(VENC_CHN VeChn,
                                const VENC_H264_TRANS_S *pstH264Trans);
RK_S32 RK_MPI_VENC_GetH264Trans(VENC_CHN VeChn,
                                VENC_H264_TRANS_S *pstH264Trans);
RK_S32 RK_MPI_VENC_SetH264Entropy(VENC_CHN VeChn,
                                  const VENC_H264_ENTROPY_S *pstH264EntropyEnc);
RK_S32 RK_MPI_VENC_GetH264Entropy(VENC_CHN VeChn,
                                  VENC_H264_ENTROPY_S *pstH264EntropyEnc);
RK_S32 RK_MPI_VENC_SetH264Dblk(VENC_CHN VeChn,
                               const VENC_H264_DBLK_S *pstH264Dblk);
RK_S32 RK_MPI_VENC_GetH264Dblk(VENC_CHN VeChn, VENC_H264_DBLK_S *pstH264Dblk);
RK_S32 RK_MPI_VENC_SetH264Vui(VENC_CHN VeChn,
                              const VENC_H264_VUI_S *pstH264Vui);
RK_S32 RK_MPI_VENC_GetH264Vui(VENC_CHN VeChn, VENC_H264_VUI_S *pstH264Vui);

// H265
RK_S32 RK_MPI_VENC_SetH265Trans(VENC_CHN VeChn,
                                const VENC_H265_TRANS_S *pstH265Trans);
RK_S32 RK_MPI_VENC_GetH265Trans(VENC_CHN VeChn,
                                VENC_H265_TRANS_S *pstH265Trans);
RK_S32 RK_MPI_VENC_SetH265Entropy(VENC_CHN VeChn,
                                  const VENC_H265_ENTROPY_S *pstH265EntropyEnc);
RK_S32 RK_MPI_VENC_GetH265Entropy(VENC_CHN VeChn,
                                  VENC_H265_ENTROPY_S *pstH265EntropyEnc);
RK_S32 RK_MPI_VENC_SetH265Dblk(VENC_CHN VeChn,
                               const VENC_H265_DBLK_S *pstH265Dblk);
RK_S32 RK_MPI_VENC_GetH265Dblk(VENC_CHN VeChn, VENC_H265_DBLK_S *pstH265Dblk);
RK_S32 RK_MPI_VENC_SetH265Sao(VENC_CHN VeChn,
                              const VENC_H265_SAO_S *pstH265Sao);
RK_S32 RK_MPI_VENC_GetH265Sao(VENC_CHN VeChn, VENC_H265_SAO_S *pstH265Sao);
RK_S32 RK_MPI_VENC_SetH265PredUnit(VENC_CHN VeChn,
                                   const VENC_H265_PU_S *pstPredUnit);
RK_S32 RK_MPI_VENC_GetH265PredUnit(VENC_CHN VeChn, VENC_H265_PU_S *pstPredUnit);
RK_S32 RK_MPI_VENC_SetH265Vui(VENC_CHN VeChn,
                              const VENC_H265_VUI_S *pstH265Vui);
RK_S32 RK_MPI_VENC_GetH265Vui(VENC_CHN VeChn, VENC_H265_VUI_S *pstH265Vui);

// MJPEG
RK_S32 RK_MPI_VENC_SetMjpegParam(VENC_CHN VeChn,
                                 const VENC_MJPEG_PARAM_S *pstMjpegParam);
RK_S32 RK_MPI_VENC_GetMjpegParam(VENC_CHN VeChn,
                                 VENC_MJPEG_PARAM_S *pstMjpegParam);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif // INCLUDE_RT_MPI_MPI_VENC_H_
