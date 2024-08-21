/*
 * Copyright (c) 2021 Rockchip, Inc. All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef __RKADK_MEDIA_COMM_H__
#define __RKADK_MEDIA_COMM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "rk_comm_video.h"
#include "rk_comm_aiisp.h"
#include "rk_mpi_aenc.h"
#include "rk_mpi_adec.h"
#include "rk_mpi_ai.h"
#include "rk_mpi_mb.h"
#include "rk_mpi_sys.h"
#include "rk_mpi_venc.h"
#include "rk_mpi_vi.h"
#include "rk_mpi_vpss.h"
#include "rk_mpi_rgn.h"
#include "rk_mpi_cal.h"
#include "rk_mpi_mmz.h"
#include "rk_mpi_vo.h"
#include "rk_mpi_vdec.h"

#ifdef RV1106_1103
#include "rk_mpi_amix.h"
#endif

#include "rkadk_common.h"
#include <stdbool.h>

#define RKADK_ENCODE_SENSE_IPC SCENE_0
#define RKADK_ENCODE_SENSE_CVR SCENE_2

/* audio capture maximum count */
#define RKADK_MEDIA_AI_MAX_CNT (1)

/* audio encoder maximum count */
#define RKADK_MEDIA_AENC_MAX_CNT (3 * RKADK_MAX_SENSOR_CNT)

/* video capture maximum count */
#define RKADK_MEDIA_VI_MAX_CNT (4 * RKADK_MAX_SENSOR_CNT)

/* video encoder maximum count */
#define RKADK_MEDIA_VENC_MAX_CNT (7 * RKADK_MAX_SENSOR_CNT)

/* vpss maximum count */
#define RKADK_MEDIA_VPSS_MAX_CNT (6 * RKADK_MAX_SENSOR_CNT)

/* vo maximum count */
#define RKADK_MEDIA_VO_MAX_CNT (6 * RKADK_MAX_SENSOR_CNT)

/* ai aenc maximum bind count */
#define RKADK_AI_AENC_MAX_BIND_CNT RKADK_MEDIA_AENC_MAX_CNT

/* vi venc maximum bind count */
#define RKADK_VI_VENC_MAX_BIND_CNT RKADK_MEDIA_VENC_MAX_CNT

/* vi vpss maximum bind count */
#define RKADK_VI_VPSS_MAX_BIND_CNT RKADK_MEDIA_VPSS_MAX_CNT

/* vpss venc maximum bind count */
#define RKADK_VPSS_VENC_MAX_BIND_CNT RKADK_MEDIA_VPSS_MAX_CNT

typedef struct {
  RKADK_U32 u32ChnId;
  VENC_STREAM_S stFrame;
} RKADK_MEDIA_VENC_DATA_S;

typedef void (*RKADK_MEDIA_VENC_DATA_PROC_FUNC)(RKADK_MEDIA_VENC_DATA_S stData,
                                                RKADK_VOID *pHandle);
typedef void (*RKADK_MEDIA_AENC_DATA_PROC_FUNC)(AUDIO_STREAM_S stFrame,
                                                RKADK_VOID *pHandle);

typedef struct {
  AIISP_CALLBACK_FUNC_S stAiIspCallback;      /* post isp callback function */
  const RK_CHAR        *pModelFilePath;       /* post isp model file path   */
  RK_U32                u32FrameBufCnt;       /* RW; frame buffer cnt    */
} RKADK_POST_ISP_ATTR_S;

#define RKADK_BUFINFO(fmt, ...)  RKADK_MEDIA_DumpBufinfo(fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

RKADK_S32 RKADK_MPI_SYS_Init();
RKADK_S32 RKADK_MPI_SYS_Exit();
bool RKADK_MPI_SYS_CHECK();

AUDIO_SOUND_MODE_E RKADK_AI_GetSoundMode(RKADK_U32 ch);
RKADK_S32 RKADK_MPI_AI_Init(AUDIO_DEV aiDevId, RKADK_S32 s32AiChnId,
                            AIO_ATTR_S *pstAiAttr, RKADK_VQE_MODE_E enMode,
                            const char *pVqeCfgPath, RKADK_U32 micType);
RKADK_S32 RKADK_MPI_AI_DeInit(AUDIO_DEV aiDevId, RKADK_S32 s32AiChnId,
                              RKADK_VQE_MODE_E enMode);

RKADK_S32 RKADK_MPI_AENC_Init(RKADK_S32 s32AencChnId,
                              AENC_CHN_ATTR_S *pstAencChnAttr);
RKADK_S32 RKADK_MPI_AENC_DeInit(RKADK_S32 s32AencChnId);

RKADK_S32 RKADK_MPI_VI_Init(RKADK_U32 u32CamId, RKADK_S32 s32ChnId,
                            VI_CHN_ATTR_S *pstViChnAttr);
RKADK_S32 RKADK_MPI_VI_DeInit(RKADK_U32 u32CamId, RKADK_S32 s32ChnId);

RKADK_S32 RKADK_MPI_VI_QueryCameraStatus(RKADK_U32 u32CamId);

RKADK_S32 RKADK_MPI_VENC_Init(RKADK_U32 u32CamId, RKADK_S32 s32ChnId,
                              VENC_CHN_ATTR_S *pstVencChnAttr);
RKADK_S32 RKADK_MPI_VENC_DeInit(RKADK_S32 s32ChnId);

RKADK_S32 RKADK_MPI_VPSS_Init(RKADK_S32 s32VpssGrp, RKADK_S32 s32VpssChn,
                              VPSS_GRP_ATTR_S *pstVpssGrpAttr, VPSS_CHN_ATTR_S *pstVpssChnAttr);

RKADK_S32 RKADK_MPI_VPSS_DeInit(RKADK_S32 s32VpssGrp, RKADK_S32 s32VpssChn);

RKADK_S32 RKADK_MPI_VO_Init(RKADK_S32 s32VoLay, RKADK_S32 s32VoDev, RKADK_S32 s32VoChn,
                        VO_PUB_ATTR_S *pstVoPubAttr, VO_VIDEO_LAYER_ATTR_S *pstLayerAttr,
                        VO_CHN_ATTR_S *pstChnAttr, RKADK_VO_SPLICE_MODE_E enSpliceMode);

RKADK_S32 RKADK_MPI_VO_DeInit(RKADK_S32 s32VoLay, RKADK_S32 s32VoDev, RKADK_S32 s32VoChn);

RKADK_S32 RKADK_MPI_SYS_Bind(const MPP_CHN_S *pstSrcChn,
                             const MPP_CHN_S *pstDestChn);
RKADK_S32 RKADK_MPI_SYS_UnBind(const MPP_CHN_S *pstSrcChn,
                               const MPP_CHN_S *pstDestChn);

PIXEL_FORMAT_E RKADK_MEDIA_GetRkPixelFormat(RKADK_FORMAT_E Format);

VO_INTF_TYPE_E RKADK_MEDIA_GetRkVoIntfTpye(RKADK_VO_INTF_TYPE_E enIntfType);

RKADK_CODEC_TYPE_E RKADK_MEDIA_GetCodecType(RK_CODEC_ID_E enType);

RK_CODEC_ID_E RKADK_MEDIA_GetRkCodecType(RKADK_CODEC_TYPE_E enType);

RKADK_S32 RKADK_MEDIA_SetRcAttr(VENC_RC_ATTR_S *pstRcAttr, RKADK_U32 u32Gop,
                                RKADK_U32 u32BitRate, RKADK_U32 u32SrcFrameRate,
                                RKADK_U32 u32DstFrameRate);

bool RKADK_MEDIA_CompareResolution(VENC_CHN_ATTR_S *pstRecAttr,
                                RKADK_U32 u32Width, RKADK_U32 u32Height);

bool RKADK_MEDIA_VencAttrCmp(VENC_CHN_ATTR_S *pstRecAttr,
                                  RKADK_U32 u32Gop, RKADK_U32 u32DstFrameRate,
                                  RKADK_U32 u32Bitrate);

RKADK_S32 RKADK_MEDIA_GetAencBuffer(MPP_CHN_S *pstChn,
                                    RKADK_MEDIA_AENC_DATA_PROC_FUNC pfnDataCB,
                                    RKADK_VOID *pHandle);

RKADK_S32
RKADK_MEDIA_StopGetAencBuffer(MPP_CHN_S *pstChn,
                              RKADK_MEDIA_AENC_DATA_PROC_FUNC pfnDataCB,
                              RKADK_VOID *pHandle);

RKADK_S32 RKADK_MEDIA_GetVencBuffer(MPP_CHN_S *pstChn,
                                    RKADK_MEDIA_VENC_DATA_PROC_FUNC pfnDataCB,
                                    RKADK_VOID *pHandle);

RKADK_S32
RKADK_MEDIA_StopGetVencBuffer(MPP_CHN_S *pstChn,
                              RKADK_MEDIA_VENC_DATA_PROC_FUNC pfnDataCB,
                              RKADK_VOID *pHandle);

RKADK_S32 RKADK_MEDIA_FrameBufMalloc(RKADK_FRAME_ATTR_S *pstFrameAttr);

RKADK_S32 RKADK_MEDIA_FrameFree(RKADK_FRAME_ATTR_S *pstFrameAttr);

bool RKADK_MEDIA_CheckFrameAttr(RKADK_FRAME_ATTR_S *pstFrameAttr);

bool RKADK_MEDIA_CheckIdrFrame(RKADK_CODEC_TYPE_E enCodecType,
                               VENC_DATA_TYPE_U uDataType);

RKADK_U16 RKADK_MEDIA_GetAudioBitWidth(AUDIO_BIT_WIDTH_E enBitWidth);

int RKADK_MEDIA_GetPixelFormat(PIXEL_FORMAT_E enPixelFormat, RKADK_CHAR *cPixFmt);

bool RKADK_MEDIA_EnableAencRegister(RKADK_CODEC_TYPE_E eCodecType);

RKADK_S32 RKADK_MEDIA_SetVencRotation(RKADK_U32 u32CamId,
                              ROTATION_E enRotation, RKADK_STREAM_TYPE_E enStreamType);

RKADK_S32 RKADK_MEDIA_QueryVencStatus(RKADK_U32 u32CamId, RKADK_STREAM_TYPE_E enStreamType);

RKADK_S32 RKADK_MEDIA_ToggleVencMirror(RKADK_U32 u32CamId,
                                    RKADK_STREAM_TYPE_E enStrmType,
                                    bool mirror);

RKADK_S32 RKADK_MEDIA_ToggleVencFlip(RKADK_U32 u32CamId,
                                  RKADK_STREAM_TYPE_E enStrmType,
                                  bool flip);

void RKADK_MEDIA_DumpBufinfo(const char *fmt, const char *fname, const int row, ...);

RKADK_S32 RKADK_MEDIA_EnablePostIsp(RKADK_U32 u32CamId, RKADK_STREAM_TYPE_E enStrmType,
                                  RKADK_POST_ISP_ATTR_S *pstPostIspAttr);

RKADK_S32 RKADK_MEDIA_SetPostIspAttr(RKADK_U32 u32CamId,
                                  RKADK_STREAM_TYPE_E enStrmType, bool bEnable,
                                  RKADK_POST_ISP_ATTR_S *pstPostIspAttr);

RKADK_S32 RKADK_MEDIA_SetVencState(RKADK_U32 u32CamId, RKADK_S32 s32ChnId, bool state);

#ifdef __cplusplus
}
#endif
#endif
