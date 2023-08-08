/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_RK_COMM_AENC_H_
#define INCLUDE_RT_MPI_RK_COMM_AENC_H_

#include "rk_comm_aio.h"
#include "rk_common.h"
#include "rk_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct rkAENC_ATTR_CODEC_S {
  RK_CODEC_ID_E enType;
  AUDIO_BIT_WIDTH_E enBitwidth;
  RK_U32 u32Channels;
  RK_U32 u32SampleRate;
  RK_U32 u32Resv[4]; /* resv for user */
  RK_VOID *pstResv;  /* resv for user */
} AENC_ATTR_CODEC_S;

typedef struct rkAENC_CHN_ATTR_S {
  RK_CODEC_ID_E enType; /* audio codec id */
  RK_U32 u32BufCount;   /* encode buffer count */

  AENC_ATTR_CODEC_S stCodecAttr;
} AENC_CHN_ATTR_S;

typedef struct rkAENC_ENCODER_S {
  RK_CODEC_ID_E enType;
  RK_U32 u32MaxFrmLen;
  RK_CHAR aszName[17];
  RK_S32 (*pfnOpenEncoder)(RK_VOID *pEncoderAttr, RK_VOID **ppEncoder);
  RK_S32 (*pfnEncodeFrm)(RK_VOID *pEncoder, RK_VOID *pParam);
  RK_S32 (*pfnCloseEncoder)(RK_VOID *pEncoder);
} AENC_ENCODER_S;

typedef enum rkEN_AENC_ERR_CODE_E {
  AENC_ERR_ENCODER_ERR = 64,
  AENC_ERR_VQE_ERR = 65,
  AENC_ERR_REGISTER_ERR = 66
} EN_AENC_ERR_CODE_E;

/* result of register AENC */
typedef enum rkENC_ENCODER_RESULT {
  AENC_ENCODER_OK = RK_SUCCESS,
  AENC_ENCODER_TRY_AGAIN,
  AENC_ENCODER_ERROR,
  AENC_ENCODER_EOS,
} AENC_ENCODER_RESULT;

/* invlalid device ID */
#define RK_ERR_AENC_INVALID_DEVID                                              \
  RK_DEF_ERR(RK_ID_AENC, RK_ERR_LEVEL_ERROR, RK_ERR_INVALID_DEVID)
/* invlalid channel ID */
#define RK_ERR_AENC_INVALID_CHNID                                              \
  RK_DEF_ERR(RK_ID_AENC, RK_ERR_LEVEL_ERROR, RK_ERR_INVALID_CHNID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define RK_ERR_AENC_ILLEGAL_PARAM                                              \
  RK_DEF_ERR(RK_ID_AENC, RK_ERR_LEVEL_ERROR, RK_ERR_ILLEGAL_PARAM)
/* channel exists */
#define RK_ERR_AENC_EXIST                                                      \
  RK_DEF_ERR(RK_ID_AENC, RK_ERR_LEVEL_ERROR, RK_ERR_EXIST)
/* channel unexists */
#define RK_ERR_AENC_UNEXIST                                                    \
  RK_DEF_ERR(RK_ID_AENC, RK_ERR_LEVEL_ERROR, RK_ERR_UNEXIST)
/* using a NULL point */
#define RK_ERR_AENC_NULL_PTR                                                   \
  RK_DEF_ERR(RK_ID_AENC, RK_ERR_LEVEL_ERROR, RK_ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing
 * attribute */
#define RK_ERR_AENC_NOT_CONFIG                                                 \
  RK_DEF_ERR(RK_ID_AENC, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define RK_ERR_AENC_NOT_SUPPORT                                                \
  RK_DEF_ERR(RK_ID_AENC, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change static attribute */
#define RK_ERR_AENC_NOT_PERM                                                   \
  RK_DEF_ERR(RK_ID_AENC, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_PERM)
/* failure caused by malloc memory */
#define RK_ERR_AENC_NOMEM                                                      \
  RK_DEF_ERR(RK_ID_AENC, RK_ERR_LEVEL_ERROR, RK_ERR_NOMEM)
/* failure caused by malloc buffer */
#define RK_ERR_AENC_NOBUF                                                      \
  RK_DEF_ERR(RK_ID_AENC, RK_ERR_LEVEL_ERROR, RK_ERR_NOBUF)
/* no data in buffer */
#define RK_ERR_AENC_BUF_EMPTY                                                  \
  RK_DEF_ERR(RK_ID_AENC, RK_ERR_LEVEL_ERROR, RK_ERR_BUF_EMPTY)
/* no buffer for new data */
#define RK_ERR_AENC_BUF_FULL                                                   \
  RK_DEF_ERR(RK_ID_AENC, RK_ERR_LEVEL_ERROR, RK_ERR_BUF_FULL)
/* system is not ready,had not initialed or loaded*/
#define RK_ERR_AENC_SYS_NOTREADY                                               \
  RK_DEF_ERR(RK_ID_AENC, RK_ERR_LEVEL_ERROR, RK_ERR_NOTREADY)
/* encoder internal err */
#define RK_ERR_AENC_ENCODER_ERR                                                \
  RK_DEF_ERR(RK_ID_AENC, RK_ERR_LEVEL_ERROR, AENC_ERR_ENCODER_ERR)
/* vqe internal err */
#define RK_ERR_AENC_VQE_ERR                                                    \
  RK_DEF_ERR(RK_ID_AENC, RK_ERR_LEVEL_ERROR, AENC_ERR_VQE_ERR)
/* register Aenc fail */
#define RK_ERR_AENC_REGISTER_ERR                                               \
  RK_DEF_ERR(RK_ID_AENC, RK_ERR_LEVEL_ERROR, AENC_ERR_REGISTER_ERR)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef INCLUDE_RT_MPI_RK_COMM_AENC_H_*/
