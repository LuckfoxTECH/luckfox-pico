/*
 * Copyright (c) 2022 Rockchip, Inc. All Rights Reserved.
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

#include "rkadk_audio_encoder_mp3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "mp3_enc_types.h"
#include "mp3_enc_table1.h"
#include "mp3_enc_table2.h"
#include "aenc_mp3_register.h"
#ifdef __cplusplus
}
#endif

static RKADK_S32 extCodecHandle = -1;
static RKADK_U32 mp3InitCnt = 0;

typedef struct _RKADK_AENC_MP3_CTX_S {
  mp3_enc *pMp3Enc;
  RKADK_S32 frameLength;
} RKADK_AENC_MP3_CTX_S;

RKADK_S32 RKAduioMp3EncoderOpen(RK_VOID *pEncoderAttr, RK_VOID **ppEncoder) {
  int bitrate = 0;
  if (pEncoderAttr == NULL) {
    RKADK_LOGE("pEncoderAttr is NULL");
    return RKADK_FAILURE;
  }

  AENC_ATTR_CODEC_S *attr = (AENC_ATTR_CODEC_S *)pEncoderAttr;
  if(attr->enType != RK_AUDIO_ID_MP3) {
    RKADK_LOGE("Invalid enType[%d]", attr->enType);
    return RKADK_FAILURE;
  }

  RKADK_AENC_MP3_CTX_S *ctx = (RKADK_AENC_MP3_CTX_S *)malloc(sizeof(RKADK_AENC_MP3_CTX_S));
  if (!ctx) {
    RKADK_LOGE("malloc aenc mp3 ctx failed");
    return RKADK_FAILURE;
  }

  memset(ctx, 0, sizeof(RKADK_AENC_MP3_CTX_S));
  if (attr->u32Resv[0] <= 0) {
    RKADK_LOGE("Invalid frame length[%d]", attr->u32Resv[0]);
    goto __FAILED;
  }

  ctx->frameLength = attr->u32Resv[0];
  bitrate = attr->u32Resv[1] / 1000;
  RKADK_LOGD("MP3Encode: sample_rate = %d, channel = %d, bitrate = %d.", attr->u32SampleRate, attr->u32Channels, bitrate);
  ctx->pMp3Enc = Mp3EncodeVariableInit(attr->u32SampleRate, attr->u32Channels, bitrate);
  if (ctx->pMp3Enc->frame_size <= 0){
    RKADK_LOGE("MP3Encode init failed! r:%d c:%d\n", attr->u32SampleRate, attr->u32Channels);
    goto __FAILED;
  }

  RKADK_LOGD("MP3Encode FrameSize = %d", ctx->pMp3Enc->frame_size);
  *ppEncoder = (RK_VOID *)ctx;

  return RKADK_SUCCESS;

__FAILED:
  RKAduioMp3EncoderClose((RK_VOID *)ctx);
  *ppEncoder = RK_NULL;
  return RKADK_FAILURE;
}

RKADK_S32 RKAduioMp3EncoderClose(RK_VOID *pEncoder) {
  RKADK_AENC_MP3_CTX_S *ctx = (RKADK_AENC_MP3_CTX_S *)pEncoder;

  if (ctx == NULL)
    return RKADK_SUCCESS;

  Mp3EncodeDeinit(ctx->pMp3Enc);
  free(ctx);
  ctx = NULL;
  return RKADK_SUCCESS;
}

RKADK_S32 RKAduioMp3EncoderEncode(RK_VOID *pEncoder, RK_VOID *pEncParam) {
  RKADK_AENC_MP3_CTX_S *ctx = (RKADK_AENC_MP3_CTX_S *)pEncoder;
  AUDIO_ADENC_PARAM_S *pParam = (AUDIO_ADENC_PARAM_S *)pEncParam;

  if(ctx == NULL || pParam == NULL) {
    RKADK_LOGE("Invalid ctx or pParam");
    return AENC_ENCODER_ERROR;
  }

  RKADK_U32 u32EncSize = 0;
  RKADK_U8 *inData = pParam->pu8InBuf;
  RKADK_U64 inPts = pParam->u64InTimeStamp;
  RKADK_U32 inbufSize = 0;
  RKADK_U32 copySize = 0;

  // if input buffer is NULL, this means eos(end of stream)
  if (inData == NULL) {
    pParam->u64OutTimeStamp = inPts;
  }

  inbufSize = 2 * ctx->pMp3Enc->frame_size;
  copySize = (pParam->u32InLen > inbufSize) ? inbufSize : pParam->u32InLen;
  memcpy(ctx->pMp3Enc->config.in_buf, inData, copySize);
  pParam->u32InLen = pParam->u32InLen - copySize;
  u32EncSize = L3_compress(ctx->pMp3Enc, 0, (unsigned char **)(&ctx->pMp3Enc->config.out_buf));

  u32EncSize = (u32EncSize > pParam->u32OutLen) ? pParam->u32OutLen : u32EncSize;
  memcpy(pParam->pu8OutBuf, ctx->pMp3Enc->config.out_buf, u32EncSize);
  pParam->u64OutTimeStamp = inPts;
  pParam->u32OutLen = u32EncSize;

  return AENC_ENCODER_OK;
}

RKADK_S32 RegisterAencMp3(void) {
  if (!mp3InitCnt) {
    RKADK_S32 ret;
    AENC_ENCODER_S aencCtx;
    memset(&aencCtx, 0, sizeof(AENC_ENCODER_S));

    extCodecHandle = -1;
    aencCtx.enType = RK_AUDIO_ID_MP3;
    snprintf((RK_CHAR*)(aencCtx.aszName),
             sizeof(aencCtx.aszName), "rkaudio");
    aencCtx.u32MaxFrmLen = 2048;
    aencCtx.pfnOpenEncoder = RKAduioMp3EncoderOpen;
    aencCtx.pfnEncodeFrm = RKAduioMp3EncoderEncode;
    aencCtx.pfnCloseEncoder = RKAduioMp3EncoderClose;

    RKADK_LOGD("register external aenc(%s)", aencCtx.aszName);
    ret = RK_MPI_AENC_RegisterEncoder(&extCodecHandle, &aencCtx);
    if (ret != RKADK_SUCCESS) {
      RKADK_LOGE("aenc %s register decoder fail %x", aencCtx.aszName, ret);
      return RKADK_FAILURE;
    }
  }

  mp3InitCnt++;
  return RKADK_SUCCESS;
}

RKADK_S32 UnRegisterAencMp3(void) {
  if (extCodecHandle == -1) {
    return RKADK_SUCCESS;
  }

  if (0 == mp3InitCnt) {
    return 0;
  } else if (1 == mp3InitCnt) {
    RKADK_LOGD("unregister external aenc");
    RKADK_S32 ret = RK_MPI_AENC_UnRegisterEncoder(extCodecHandle);
    if (ret != RKADK_SUCCESS) {
      RKADK_LOGE("aenc unregister decoder fail %x", ret);
      return RKADK_FAILURE;
    }

    extCodecHandle = -1;
  }
  mp3InitCnt--;
  return RKADK_SUCCESS;
}
