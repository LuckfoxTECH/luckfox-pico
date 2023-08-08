/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_RK_COMM_ADEC_H_
#define INCLUDE_RT_MPI_RK_COMM_ADEC_H_

#include "rk_comm_aio.h"
#include "rk_comm_mb.h"
#include "rk_common.h"
#include "rk_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef enum rkADEC_MODE_E {
  /*
   * require input is valid dec pack(a complete frame encode result),
   * e.g.the stream get from AENC is a valid dec pack, the stream know
   * actually pack len from file is also a dec pack.
   * this mode is high-performative*/
  ADEC_MODE_PACK = 0,

  /*
   * input is stream,low-performative, if you couldn't find out whether
   * a stream is vaild dec pack,you could use this mode
   */
  ADEC_MODE_STREAM,

  ADEC_MODE_BUTT
} ADEC_MODE_E;

typedef struct rkADEC_ATTR_CODEC_S {
  RK_CODEC_ID_E enType;
  RK_U32 u32Channels;
  RK_U32 u32SampleRate;
  RK_U32 u32BitPerCodedSample; /* codewords */

  RK_VOID *pExtraData;
  RK_U32 u32ExtraDataSize;

  RK_U32 u32Resv[4]; /* resv for user */
  RK_VOID *pstResv;  /* resv for user */
} ADEC_ATTR_CODEC_S;

typedef struct rkADEC_FRAME_INFO_S {
  RK_U32 u32SampleRate;
  RK_U32 u32Channels;
  RK_U64 u64ChnLayout;
  AUDIO_BIT_WIDTH_E enBitWidth;
  RK_U32 resv[2];
} ADEC_FRAME_INFO_S;

typedef struct rkADEC_CH_ATTR_S {
  RK_CODEC_ID_E enType; /* audio codec id */
  ADEC_MODE_E enMode;   /* decode mode */
  RK_U32 u32BufCount;   /* decode buffer count */
  RK_U32 u32BufSize;    /* decode buffer size(buffer size to store pcm data) */

  ADEC_ATTR_CODEC_S stCodecAttr;
} ADEC_CHN_ATTR_S;

typedef struct rkADEC_CH_STATE_S {
  RK_BOOL bEndOfStream;    /* EOS flag */
  RK_U32 u32BufferFrmNum;  /* total number of channel buffer */
  RK_U32 u32BufferFreeNum; /* free number of channel buffer */
  RK_U32 u32BufferBusyNum; /* busy number of channel buffer */
} ADEC_CHN_STATE_S;

typedef enum rkEN_ADEC_ERR_CODE_E {
  ADEC_ERR_DECODER_ERR = 64,
  ADEC_ERR_BUF_LACK,
  ADEC_ERR_REGISTER_ERR,
} RK_ADEC_ERR_CODE_E;

typedef struct rkADEC_DECODER_S {
  RK_CODEC_ID_E enType;
  RK_UCHAR aszName[17];
  // open decoder
  RK_S32 (*pfnOpenDecoder)(RK_VOID *pDecoderAttr, RK_VOID **ppDecoder);
  RK_S32 (*pfnDecodeFrm)(RK_VOID *pDecoder, RK_VOID *pParam);
  // get audio frames infor
  RK_S32 (*pfnGetFrmInfo)(RK_VOID *pDecoder, RK_VOID *pInfo);
  // close audio decoder
  RK_S32 (*pfnCloseDecoder)(RK_VOID *pDecoder);
  // reset audio decoder
  RK_S32 (*pfnResetDecoder)(RK_VOID *pDecoder);
} ADEC_DECODER_S;

/* result of register ADEC */
typedef enum rkADEC_DECODER_RESULT {
  ADEC_DECODER_OK = RK_SUCCESS,
  ADEC_DECODER_TRY_AGAIN,
  ADEC_DECODER_ERROR,
  ADEC_DECODER_EOS,
} ADEC_DECODER_RESULT;

/* invlalid device ID */
#define RK_ERR_ADEC_INVALID_DEVID                                              \
  RK_DEF_ERR(RK_ID_ADEC, RK_ERR_LEVEL_ERROR, RK_ERR_INVALID_DEVID)
/* invlalid channel ID */
#define RK_ERR_ADEC_INVALID_CHNID                                              \
  RK_DEF_ERR(RK_ID_ADEC, RK_ERR_LEVEL_ERROR, RK_ERR_INVALID_CHNID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define RK_ERR_ADEC_ILLEGAL_PARAM                                              \
  RK_DEF_ERR(RK_ID_ADEC, RK_ERR_LEVEL_ERROR, RK_ERR_ILLEGAL_PARAM)
/* channel exists */
#define RK_ERR_ADEC_EXIST                                                      \
  RK_DEF_ERR(RK_ID_ADEC, RK_ERR_LEVEL_ERROR, RK_ERR_EXIST)
/* channel unexists */
#define RK_ERR_ADEC_UNEXIST                                                    \
  RK_DEF_ERR(RK_ID_ADEC, RK_ERR_LEVEL_ERROR, RK_ERR_UNEXIST)
/* using a NULL point */
#define RK_ERR_ADEC_NULL_PTR                                                   \
  RK_DEF_ERR(RK_ID_ADEC, RK_ERR_LEVEL_ERROR, RK_ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing
 * attribute */
#define RK_ERR_ADEC_NOT_CONFIG                                                 \
  RK_DEF_ERR(RK_ID_ADEC, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define RK_ERR_ADEC_NOT_SUPPORT                                                \
  RK_DEF_ERR(RK_ID_ADEC, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change stati attribute */
#define RK_ERR_ADEC_NOT_PERM                                                   \
  RK_DEF_ERR(RK_ID_ADEC, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_PERM)
/* failure caused by malloc memory */
#define RK_ERR_ADEC_NOMEM                                                      \
  RK_DEF_ERR(RK_ID_ADEC, RK_ERR_LEVEL_ERROR, RK_ERR_NOMEM)
/* failure caused by malloc buffer */
#define RK_ERR_ADEC_NOBUF                                                      \
  RK_DEF_ERR(RK_ID_ADEC, RK_ERR_LEVEL_ERROR, RK_ERR_NOBUF)
/* no data in buffer */
#define RK_ERR_ADEC_BUF_EMPTY                                                  \
  RK_DEF_ERR(RK_ID_ADEC, RK_ERR_LEVEL_ERROR, RK_ERR_BUF_EMPTY)
/* no buffer for new data */
#define RK_ERR_ADEC_BUF_FULL                                                   \
  RK_DEF_ERR(RK_ID_ADEC, RK_ERR_LEVEL_ERROR, RK_ERR_BUF_FULL)
/* system is not ready,had not initialed or loaded*/
#define RK_ERR_ADEC_SYS_NOTREADY                                               \
  RK_DEF_ERR(RK_ID_ADEC, RK_ERR_LEVEL_ERROR, RK_ERR_NOTREADY)
/* decoder internal err */
#define RK_ERR_ADEC_DECODER_ERR                                                \
  RK_DEF_ERR(RK_ID_ADEC, RK_ERR_LEVEL_ERROR, ADEC_ERR_DECODER_ERR)
/* input buffer not enough to decode one frame */
#define RK_ERR_ADEC_BUF_LACK                                                   \
  RK_DEF_ERR(RK_ID_ADEC, RK_ERR_LEVEL_ERROR, ADEC_ERR_BUF_LACK)
/* register ADec fail */
#define RK_ERR_ADEC_REGISTER_ERR                                               \
  RK_DEF_ERR(RK_ID_ADEC, RK_ERR_LEVEL_ERROR, ADEC_ERR_REGISTER_ERR)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef  INCLUDE_RT_MPI_RK_COMM_ADEC_H_ */
