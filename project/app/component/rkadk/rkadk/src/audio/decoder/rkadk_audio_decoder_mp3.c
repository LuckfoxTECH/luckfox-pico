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

#include "rkadk_audio_decoder_mp3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "mp3dec.h"
#ifdef __cplusplus
}
#endif

#define MP3MAXFRAMESIZE 4608
static RKADK_S32 extCodecHandle = -1;
static RKADK_U32 mp3DecInitCnt = 0;

typedef struct _MP3DecInfo
{
  /* pointers to platform-specific data structures */
  void *FrameHeaderPS;
  void *SideInfoPS;
  void *ScaleFactorInfoPS;
  void *HuffmanInfoPS;
  void *DequantInfoPS;
  void *IMDCTInfoPS;
  void *SubbandInfoPS;

  /* buffer which must be large enough to hold largest possible main_data section */
  unsigned char mainBuf[MAINBUF_SIZE];

  /* special info for "free" bitrate files */
  int freeBitrateFlag;
  int freeBitrateSlots;

  /* user-accessible info */
  int bitrate;
  int nChans;
  int samprate;
  int nGrans;                /* granules per frame */
  int nGranSamps;            /* samples per granule */
  int nSlots;
  int layer;
  int vbr;                    /* is vbr flag */
  uint32_t fSize;             /* file length, no tags */
  uint32_t fCount;            /* frame count */
  char TOC[100];              /* TOC */

  MPEGVersion version;

  int mainDataBegin;
  int mainDataBytes;

  int part23Length[MAX_NGRAN][MAX_NCHAN];

} MP3DecInfo;

typedef struct {
  HMP3Decoder pMp3Dec;
  MP3FrameInfo frameInfo;
  RK_U8 decMp3buf[MP3MAXFRAMESIZE];
  short int decPCMbuf[MP3MAXFRAMESIZE];
  RK_U64 audioPts;
} RKADK_ADEC_MP3_CTX_S;

static RKADK_S32 RKAduioMp3DecoderOpen(RK_VOID *pDecoderAttr, RK_VOID **ppDecoder) {
  if (pDecoderAttr == NULL) {
    RKADK_LOGE("pDecoderAttr is NULL");
    return RKADK_FAILURE;
  }

  RKADK_ADEC_MP3_CTX_S *ctx = (RKADK_ADEC_MP3_CTX_S *)malloc(sizeof(RKADK_ADEC_MP3_CTX_S));
  if (!ctx) {
    RKADK_LOGE("malloc adec mp3 ctx failed");
    return RKADK_FAILURE;
  }

  memset(ctx, 0, sizeof(RKADK_ADEC_MP3_CTX_S));
  ctx->pMp3Dec = MP3InitDecoder();
  if (!ctx->pMp3Dec) {
    RKADK_LOGE("malloc adec pMp3Dec failed");
    free(ctx);
    ctx = NULL;
    return RKADK_FAILURE;
  }

  memset(ctx->decMp3buf, 0, sizeof(ctx->decMp3buf));
  memset(ctx->decPCMbuf, 0, sizeof(ctx->decPCMbuf));

  *ppDecoder = (RK_VOID *)ctx;
  return RKADK_SUCCESS;
}

static RKADK_S32 RKAduioMp3DecoderClose(RK_VOID *pDecoder) {
  RKADK_ADEC_MP3_CTX_S *ctx = (RKADK_ADEC_MP3_CTX_S *)pDecoder;

  if (ctx == NULL)
      return RKADK_FAILURE;

  if (ctx->pMp3Dec) {
    MP3FreeDecoder(ctx->pMp3Dec);
    ctx->pMp3Dec = NULL;
  }

  if (ctx)
    free(ctx);

  return RKADK_SUCCESS;
}

static RKADK_S32 RKAduioMp3DecoderDecode(RK_VOID *pDecoder, RK_VOID *pDecParam) {
  RKADK_S32 ret = 0;
  RKADK_ADEC_MP3_CTX_S *ctx = (RKADK_ADEC_MP3_CTX_S *)pDecoder;
  RKADK_S32 skip = 0;

  if (ctx == NULL || ctx->pMp3Dec == NULL || pDecParam == NULL)
    return ADEC_DECODER_ERROR;

  AUDIO_ADENC_PARAM_S *pParam = (AUDIO_ADENC_PARAM_S *)pDecParam;
  RK_BOOL eos = RK_FALSE;
  RKADK_U8 *pInput = NULL;
  RKADK_S32 inLength = pParam->u32InLen;
  RKADK_S32 copySize = 0;
  RKADK_U64 calcPts = 0;

  if ((pParam->pu8InBuf == NULL) || (inLength == 0))
    eos = RK_TRUE;

  copySize =  (inLength <= MP3MAXFRAMESIZE) ? inLength : MP3MAXFRAMESIZE;
  inLength -= copySize;
  memcpy(ctx->decMp3buf, pParam->pu8InBuf, copySize);
  pInput = (RKADK_U8 *)ctx->decMp3buf;

  if (copySize && (skip = MP3FindSyncWord(pInput, copySize)) < 0) {
    printf("mp3 decode don't find sync word\n");
    pParam->u32InLen = 0;
    return ADEC_DECODER_ERROR;
  }

  copySize -= skip;
  pInput += skip;

  ret = MP3Decode(ctx->pMp3Dec, &pInput, &copySize, ctx->decPCMbuf, 0);
  pParam->u32InLen = inLength + copySize;

  if (ret) {
    if(eos)
      return ADEC_DECODER_EOS;

    pParam->u32InLen = inLength;
    if (ret == ERR_MP3_INDATA_UNDERFLOW) {
      RKADK_LOGE("mp3 decode input data underflow");
      return ADEC_DECODER_ERROR;
    }

    if(ret == -2) {
      RKADK_LOGW("mp3 encoded data does not start from the first frame");
    } else {
      RKADK_LOGE("mp3 decode error, ret = %d", ret);
      return ADEC_DECODER_ERROR;
    }
  }

  MP3GetLastFrameInfo(ctx->pMp3Dec, &ctx->frameInfo);

  MP3DecInfo *mp3DecInfo = (MP3DecInfo *)ctx->pMp3Dec;
  pParam->u32OutLen = mp3DecInfo->nGrans * mp3DecInfo->nGranSamps * ctx->frameInfo.nChans * 2;
  memcpy(pParam->pu8OutBuf, (RKADK_U8 *)ctx->decPCMbuf, pParam->u32OutLen);

  calcPts = (((RK_U64)pParam->u32OutLen / 2) * 1000000) / ctx->frameInfo.samprate;
  if (pParam->u64InTimeStamp)
    pParam->u64OutTimeStamp = pParam->u64InTimeStamp;
  else if (ctx->audioPts)
    pParam->u64OutTimeStamp = ctx->audioPts + calcPts;

  if (pParam->u64OutTimeStamp)
    ctx->audioPts = pParam->u64OutTimeStamp;
  else {
    ctx->audioPts += calcPts;
  }

  return ADEC_DECODER_OK;
}

static RKADK_S32 RKAduioMp3DecoderGetFrameInfo(RK_VOID *pDecoder, RK_VOID *pInfo) {
  ADEC_FRAME_INFO_S stFrameInfo;
  RKADK_ADEC_MP3_CTX_S *ctx = (RKADK_ADEC_MP3_CTX_S *)pDecoder;

  if (ctx == NULL || pInfo == NULL)
    return RKADK_FAILURE;

  MP3GetLastFrameInfo(ctx->pMp3Dec, &ctx->frameInfo);

  if (!ctx->frameInfo.bitsPerSample) {
    RKADK_LOGE("mp3 decode get info failed");
    return RKADK_FAILURE;
  }

  memset(&stFrameInfo, 0, sizeof(ADEC_FRAME_INFO_S));
  stFrameInfo.u32Channels = ctx->frameInfo.nChans;
  stFrameInfo.u32SampleRate = ctx->frameInfo.samprate;
  stFrameInfo.enBitWidth = AUDIO_BIT_WIDTH_16;
  memcpy(pInfo, &stFrameInfo, sizeof(ADEC_FRAME_INFO_S));
  return RKADK_SUCCESS;
}

RKADK_S32 RegisterAdecMp3() {
  if (!mp3DecInitCnt) {
    ADEC_DECODER_S adecCtx;
    memset(&adecCtx, 0, sizeof(ADEC_DECODER_S));

    extCodecHandle = -1;
    adecCtx.enType = RK_AUDIO_ID_MP3;
    snprintf((RK_CHAR*)(adecCtx.aszName), sizeof(adecCtx.aszName), "rkaudio1");
    adecCtx.pfnOpenDecoder  = RKAduioMp3DecoderOpen;
    adecCtx.pfnDecodeFrm    = RKAduioMp3DecoderDecode;
    adecCtx.pfnGetFrmInfo   = RKAduioMp3DecoderGetFrameInfo;
    adecCtx.pfnCloseDecoder = RKAduioMp3DecoderClose;
    adecCtx.pfnResetDecoder = RK_NULL;//RKMpiFdkAac_Decoder_reset;

    RKADK_LOGD("register ext audio decoder");
    RKADK_S32 ret = RK_MPI_ADEC_RegisterDecoder(&extCodecHandle, &adecCtx);
    if (ret != RKADK_SUCCESS) {
      RKADK_LOGE("adec register decoder fail, ret = 0x%x", ret);
      return RKADK_FAILURE;
    }
  }

  mp3DecInitCnt++;
  return RKADK_SUCCESS;
}

RKADK_S32 UnRegisterAdecMp3() {
  if (extCodecHandle == -1)
    return RKADK_SUCCESS;

  if (0 == mp3DecInitCnt) {
    return 0;
  } else if (1 == mp3DecInitCnt) {
    RKADK_LOGD("unregister ext audio decoder");
    RKADK_S32 ret = RK_MPI_ADEC_UnRegisterDecoder(extCodecHandle);
    if (ret != RKADK_SUCCESS) {
      RKADK_LOGE("adec unregister decoder fail, ret = 0x%x", ret);
      return RKADK_FAILURE;
    }

    extCodecHandle = -1;
  }

  mp3DecInitCnt--;
  return RKADK_SUCCESS;
}