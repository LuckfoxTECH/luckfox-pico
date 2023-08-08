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

#ifndef __RKADK_STREAM_H__
#define __RKADK_STREAM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "rk_mpi_venc.h"
#include "rkadk_common.h"

/* the data type of video encode */
typedef struct {
  RKADK_CODEC_TYPE_E enPayloadType; /* <H.264/H.265/JPEG/MJPEG */
  union {
    H264E_NALU_TYPE_E enH264EType; /* <H264E NALU types */
    H265E_NALU_TYPE_E enH265EType; /* <H265E NALU types */
    JPEGE_PACK_TYPE_E enJPEGEType; /* <TODO: JPEGE PACK types*/
  };
} RKADK_VENC_DATA_TYPE_S;

/* the data pack of video encode */
typedef struct {
  RKADK_U8 *apu8Addr;                /* <the virtual address of stream */
  RKADK_U32 au32Len;                 /* <the length of stream */
  RKADK_U64 u64PTS;                  /* <time stamp */
  RKADK_VENC_DATA_TYPE_S stDataType; /* <the type of stream */
} RKADK_VENC_DATA_PACK_S;

/* the data of video encode */
typedef struct {
  RKADK_VENC_DATA_PACK_S astPack; /* <stream pack attribute */
  RKADK_U32 u32Seq;               /* <the list number of stream */
  RKADK_BOOL bEndOfStream;        /* <frame end flag */
  RKADK_U32 u32CamId;
} RKADK_VIDEO_STREAM_S;

typedef RKADK_S32 (*RKADK_VENC_DATA_PROC_FUNC)(
    RKADK_VIDEO_STREAM_S *pVStreamData);

typedef struct {
  RKADK_U8 *pStream;      /* the virtual address of stream */
  RKADK_U32 u32Len;       /* stream lenth, by bytes */
  RKADK_U64 u64TimeStamp; /* frame time stamp */
  RKADK_U32 u32Seq; /* frame seq, if stream is not a valid frame,u32Seq is 0 */
  RKADK_CODEC_TYPE_E enType;
} RKADK_AUDIO_STREAM_S;

/** data proc callback function pointer */
typedef RKADK_S32 (*RKADK_AUDIO_DATA_PROC_FUNC)(
    RKADK_AUDIO_STREAM_S *pAStreamData);

/** Video Information */
typedef struct {
  RKADK_CODEC_TYPE_E enCodecType;
  RKADK_U32 u32Width;
  RKADK_U32 u32Height;
  RKADK_U32 u32BitRate;
  RKADK_U32 u32FrameRate;
  RKADK_U32 u32Gop;
} RKADK_VIDEO_INFO_S;

/** audio Information */
typedef struct {
  RKADK_CODEC_TYPE_E enCodecType;
  RKADK_U32 u32ChnCnt;
  RKADK_U32 u32SampleRate;
  RKADK_U32 u32AvgBytesPerSec;
  RKADK_U32 u32SamplesPerFrame;
  RKADK_U16 u16SampleBitWidth;
} RKADK_AUDIO_INFO_S;

typedef struct {
  RKADK_U32 u32CamId;
  RKADK_CODEC_TYPE_E enCodecType;
  RKADK_AUDIO_DATA_PROC_FUNC pfnPcmDataCB;
  RKADK_AUDIO_DATA_PROC_FUNC pfnAencDataCB;
} RKADK_STREAM_AUDIO_ATTR_S;

typedef struct {
  RKADK_U32 u32CamId;
  RKADK_VENC_DATA_PROC_FUNC pfnDataCB;
} RKADK_STREAM_VIDEO_ATTR_S;

RKADK_S32 RKADK_STREAM_VideoInit(RKADK_STREAM_VIDEO_ATTR_S *pstVideoAttr,
                                 RKADK_MW_PTR *ppHandle);

RKADK_S32 RKADK_STREAM_VideoDeInit(RKADK_MW_PTR pHandle);

RKADK_S32 RKADK_STREAM_VencStart(RKADK_MW_PTR pHandle, RKADK_S32 s32FrameCnt);

RKADK_S32 RKADK_STREAM_VencStop(RKADK_MW_PTR pHandle);

RKADK_S32 RKADK_STREAM_GetVideoInfo(RKADK_U32 u32CamId,
                                    RKADK_VIDEO_INFO_S *pstVideoInfo);

RKADK_S32 RKADK_STREAM_AudioInit(RKADK_STREAM_AUDIO_ATTR_S *pstAudioAttr,
                                 RKADK_MW_PTR *ppHandle);

RKADK_S32 RKADK_STREAM_AudioDeInit(RKADK_MW_PTR pHandle);

RKADK_S32 RKADK_STREAM_AencStart(RKADK_MW_PTR pHandle);

RKADK_S32 RKADK_STREAM_AencStop(RKADK_MW_PTR pHandle);

RKADK_S32 RKADK_STREAM_GetAudioInfo(RKADK_MW_PTR pHandle,
                                    RKADK_AUDIO_INFO_S *pstAudioInfo);

#ifdef __cplusplus
}
#endif
#endif
