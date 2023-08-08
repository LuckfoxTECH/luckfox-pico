/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_MPI_COMM_AIO_H_
#define INCLUDE_RT_MPI_MPI_COMM_AIO_H_

#include "rk_comm_mb.h"
#include "rk_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define MAX_AUDIO_FILE_PATH_LEN 256
#define MAX_AUDIO_FILE_NAME_LEN 256

typedef enum rkAUDIO_SAMPLE_RATE_E {
  AUDIO_SAMPLE_RATE_DISABLE = 0,
  AUDIO_SAMPLE_RATE_8000 = 8000,   /* 8K samplerate*/
  AUDIO_SAMPLE_RATE_12000 = 12000, /* 12K samplerate*/
  AUDIO_SAMPLE_RATE_11025 = 11025, /* 11.025K samplerate*/
  AUDIO_SAMPLE_RATE_16000 = 16000, /* 16K samplerate*/
  AUDIO_SAMPLE_RATE_22050 = 22050, /* 22.050K samplerate*/
  AUDIO_SAMPLE_RATE_24000 = 24000, /* 24K samplerate*/
  AUDIO_SAMPLE_RATE_32000 = 32000, /* 32K samplerate*/
  AUDIO_SAMPLE_RATE_44100 = 44100, /* 44.1K samplerate*/
  AUDIO_SAMPLE_RATE_48000 = 48000, /* 48K samplerate*/
  AUDIO_SAMPLE_RATE_64000 = 64000, /* 64K samplerate*/
  AUDIO_SAMPLE_RATE_96000 = 96000, /* 96K samplerate*/
  AUDIO_SAMPLE_RATE_BUTT,
} AUDIO_SAMPLE_RATE_E;

typedef struct rkAUDIO_STREAM_S {
  MB_BLK pMbBlk;
  RK_U32 u32Len;       /* stream lenth, by bytes */
  RK_U64 u64TimeStamp; /* frame time stamp*/
  RK_U32 u32Seq; /* frame seq,if stream is not a valid frame, u32Seq is 0*/
  RK_BOOL bBypassMbBlk; /* FALSE: copy, TRUE: MbBlock owned by internal */
} AUDIO_STREAM_S;

typedef enum rkAUDIO_BIT_WIDTH_E {
  AUDIO_BIT_WIDTH_8 = 0,   /* 8bit width */
  AUDIO_BIT_WIDTH_16 = 1,  /* 16bit width */
  AUDIO_BIT_WIDTH_24 = 2,  /* 24bit width */
  AUDIO_BIT_WIDTH_32 = 3,  /* 32bit width */
  AUDIO_BIT_WIDTH_FLT = 4, /* float, 32bit width */
  AUDIO_BIT_WIDTH_BUTT,
} AUDIO_BIT_WIDTH_E;

typedef enum rkAIO_SOUND_MODE_E {
  AUDIO_SOUND_MODE_MONO = 0,   /*mono*/
  AUDIO_SOUND_MODE_STEREO = 1, /*stereo*/
  AUDIO_SOUND_MODE_4_CHN = 4,
  AUDIO_SOUND_MODE_6_CHN = 6,
  AUDIO_SOUND_MODE_8_CHN = 8,
  AUDIO_SOUND_MODE_BUTT
} AUDIO_SOUND_MODE_E;

typedef enum rkAUDIO_CHN_MODE_E {
  AUDIO_CHN_MODE_LEFT = 10,
  AUDIO_CHN_MODE_RIGHT = 11,
  AUDIO_CHN_MODE_BUTT
} AUDIO_CHN_MODE_E;

typedef enum rkAUDIO_LOOPBACK_MODE_E {
  AUDIO_LOOPBACK_NONE = 0,     /* audio no use loopback */
  AUDIO_LOOPBACK_SW_MICL_REFR, /* audio use software loopback (Mic Left + Ref
                                  Right) */
  AUDIO_LOOPBACK_SW_REFL_MICR, /* audio use software loopback (Ref Left + Mic
                                  Right) */
  AUDIO_LOOPBACK_BUTT
} AUDIO_LOOPBACK_MODE_E;

typedef enum rkAUDIO_CHN_ATTR_MODE_E {
  AUDIO_CHN_ATTR_PPM = 1,
  AUDIO_CHN_ATTR_RATE = 2,
  AUDIO_CHN_ATTR_BUTT
} AUDIO_CHN_ATTR_MODE_E;

typedef struct rkAUDIO_FRAME_S {
  MB_BLK pMbBlk;
  AUDIO_BIT_WIDTH_E enBitWidth;   /*audio frame bitwidth*/
  AUDIO_SOUND_MODE_E enSoundMode; /*audio frame momo or stereo mode*/
  RK_U64 u64TimeStamp;            /*audio frame timestamp*/
  RK_U32 u32Seq;                  /*audio frame seq*/
  RK_U32 u32Len; /*data lenth per channel in frame, u32Len <= 0 mean eos*/
  RK_BOOL bBypassMbBlk; /* FALSE: copy, TRUE: MbBlock owned by internal */
} AUDIO_FRAME_S;

typedef struct rkAEC_FRAME_S {
  AUDIO_FRAME_S stRefFrame; /* AEC reference audio frame */
  RK_BOOL bValid;           /* whether frame is valid */
  RK_BOOL bSysBind;         /* whether is sysbind */
} AEC_FRAME_S;

typedef struct rkAUDIO_FRAME_INFO_S {
  AUDIO_FRAME_S *pstFrame; /*frame ptr*/
  RK_U32 u32Id;            /*frame id*/
} AUDIO_FRAME_INFO_S;

typedef struct rkAUDIO_ADENC_PARAM_S {
  RK_U8 *pu8InBuf;
  RK_U32 u32InLen;
  RK_U64 u64InTimeStamp;

  RK_U8 *pu8OutBuf;
  RK_U32 u32OutLen;
  RK_U64 u64OutTimeStamp;
} AUDIO_ADENC_PARAM_S;

typedef enum rkAIO_MODE_E {
  AIO_MODE_I2S_MASTER = 0,  /* AIO I2S master mode */
  AIO_MODE_I2S_SLAVE,       /* AIO I2S slave mode */
  AIO_MODE_PCM_SLAVE_STD,   /* AIO PCM slave standard mode */
  AIO_MODE_PCM_SLAVE_NSTD,  /* AIO PCM slave non-standard mode */
  AIO_MODE_PCM_MASTER_STD,  /* AIO PCM master standard mode */
  AIO_MODE_PCM_MASTER_NSTD, /* AIO PCM master non-standard mode */
  AIO_MODE_BUTT
} AIO_MODE_E;

typedef struct rkAIO_SOUND_CARD {
  RK_U32 channels;
  RK_U32 sampleRate;
  AUDIO_BIT_WIDTH_E bitWidth;
} AIO_SOUND_CARD;

typedef struct rkAIO_ATTR_S {
  // params of sound card
  AIO_SOUND_CARD soundCard;
  // input data sample rate
  AUDIO_SAMPLE_RATE_E enSamplerate;
  // bitwidth
  AUDIO_BIT_WIDTH_E enBitwidth;
  // momo or steror
  AUDIO_SOUND_MODE_E enSoundmode;
  /* expand 8bit to 16bit,use AI_EXPAND(only valid for AI 8bit),
   * use AI_CUT(only valid for extern Codec for 24bit)
   */
  RK_U32 u32EXFlag;
  /* frame num in buf[2,MAX_AUDIO_FRAME_NUM] */
  RK_U32 u32FrmNum;
  /*
   * point num per frame (80/160/240/320/480/1024/2048)
   * (ADPCM IMA should add 1 point, AMR only support 160)
   */
  RK_U32 u32PtNumPerFrm;
  RK_U32 u32ChnCnt; /* channle number on FS, valid value:1/2/4/8 */
  /*
   * name of sound card, if it is setted, we will
   * using it to open sound card, otherwise, use
   * the index of device to open sound card
   */
  RK_U8 u8CardName[64];
} AIO_ATTR_S;

typedef struct rkAI_CHN_PARAM_S {
  RK_S32 s32UsrFrmDepth;
  AUDIO_LOOPBACK_MODE_E enLoopbackMode;
} AI_CHN_PARAM_S;

typedef struct rkAO_CHN_PARAM_S {
  AUDIO_CHN_MODE_E enMode;
  AUDIO_LOOPBACK_MODE_E enLoopbackMode;
} AO_CHN_PARAM_S;

typedef struct rkAI_CHN_ATTR_S {
  AUDIO_CHN_ATTR_MODE_E enChnAttr;
  union {
    RK_S32 s32Ppm;
    RK_U32 u32SampleRate;
  };
} AI_CHN_ATTR_S;

typedef struct rkAO_CHN_ATTR_S {
  AUDIO_CHN_ATTR_MODE_E enChnAttr;
  union {
    RK_S32 s32Ppm;
    RK_U32 u32SampleRate;
  };
} AO_CHN_ATTR_S;

typedef struct rkAO_CHN_STATE_S {
  RK_U32 u32ChnTotalNum; /* total number of channel buffer */
  RK_U32 u32ChnFreeNum;  /* free number of channel buffer */
  RK_U32 u32ChnBusyNum;  /* busy number of channel buffer */
} AO_CHN_STATE_S;

typedef enum rkAUDIO_TRACK_MODE_E {
  AUDIO_TRACK_NORMAL = 0,
  AUDIO_TRACK_BOTH_LEFT = 1,
  AUDIO_TRACK_BOTH_RIGHT = 2,
  AUDIO_TRACK_EXCHANGE = 3,
  AUDIO_TRACK_MIX = 4,
  AUDIO_TRACK_LEFT_MUTE = 5,
  AUDIO_TRACK_RIGHT_MUTE = 6,
  AUDIO_TRACK_BOTH_MUTE = 7,
  AUDIO_TRACK_FRONT_LEFT = 8,
  AUDIO_TRACK_FRONT_RIGHT = 9,
  AUDIO_TRACK_OUT_STEREO = 10,

  AUDIO_TRACK_BUTT
} AUDIO_TRACK_MODE_E;

typedef enum rkAUDIO_FADE_RATE_E {
  AUDIO_FADE_RATE_1 = 0,
  AUDIO_FADE_RATE_2 = 1,
  AUDIO_FADE_RATE_4 = 2,
  AUDIO_FADE_RATE_8 = 3,
  AUDIO_FADE_RATE_16 = 4,
  AUDIO_FADE_RATE_32 = 5,
  AUDIO_FADE_RATE_64 = 6,
  AUDIO_FADE_RATE_128 = 7,

  AUDIO_FADE_RATE_BUTT
} AUDIO_FADE_RATE_E;

typedef struct rkAUDIO_FADE_S {
  RK_BOOL bFade;
  AUDIO_FADE_RATE_E enFadeInRate;
  AUDIO_FADE_RATE_E enFadeOutRate;
} AUDIO_FADE_S;

typedef enum rkAUDIO_VOLUME_CURVE_E {
  AUDIO_CURVE_UNSET = 0,
  AUDIO_CURVE_LINEAR = 1,
  AUDIO_CURVE_LOGARITHM = 2,
  AUDIO_CURVE_CUSTOMIZE = 3,

  AUDIO_CURVE_BUTT
} AUDIO_VOLUME_CURVE_E;

typedef struct rkAUDIO_VOLUME_CURVE_S {
  AUDIO_VOLUME_CURVE_E enCurveType;
  RK_S32 s32Resolution;
  RK_FLOAT fMinDB;
  RK_FLOAT fMaxDB;
  RK_U32 *pCurveTable;
} AUDIO_VOLUME_CURVE_S;

/*Defines the configure parameters of AI saving file.*/
typedef struct rkAUDIO_SAVE_FILE_INFO_S {
  RK_BOOL bCfg;
  RK_CHAR aFilePath[MAX_AUDIO_FILE_PATH_LEN];
  RK_CHAR aFileName[MAX_AUDIO_FILE_NAME_LEN];
  RK_U32 u32FileSize; /*in KB*/
} AUDIO_SAVE_FILE_INFO_S;

/*Defines whether the file is saving or not .*/
typedef struct rkAUDIO_FILE_STATUS_S {
  RK_BOOL bSaving;
} AUDIO_FILE_STATUS_S;

typedef enum rkAIO_VQE_CONFIG_METHOD {
  AIO_VQE_CONFIG_NONE,
  AIO_VQE_CONFIG_USER,      // user set all configs by parameters
  AIO_VQE_CONFIG_LOAD_FILE, // read and parse config file
} AIO_VQE_CONFIG_METHOD;

typedef enum rkAI_VQE_TYPE_S {
  AI_VQE_NONE = 0,
  AI_VQE_RECORD,
  AI_VQE_TALK,
} AI_VQE_TYPE_S;

typedef struct rkAI_VQE_CONFIG_S {
  AIO_VQE_CONFIG_METHOD enCfgMode; /* see AIO_VQE_CONFIG_METHOD */
  RK_S32 s32WorkSampleRate; /* Sample Rate: 8KHz/16KHz/48KHz. default: 16KHz*/
  RK_S32 s32FrameSample; /* VQE frame length, default: 256 frames(16ms,16KHz) */
  union {
    /* config file if enCfgMode = AIO_VQE_CONFIG_LOAD_FILE */
    RK_CHAR aCfgFile[MAX_AUDIO_FILE_PATH_LEN];
  };
} AI_VQE_CONFIG_S;

typedef struct rkAI_AED_CONFIG_S {
  RK_FLOAT fSnrDB;
  RK_FLOAT fLsdDB;
  RK_S32 s32Policy;
} AI_AED_CONFIG_S;

typedef struct rkAI_AED_RESULT_S {
  RK_BOOL bAcousticEventDetected;
  RK_BOOL bLoudSoundDetected;
} AI_AED_RESULT_S;

typedef struct rkAI_BCD_CONFIG_S {
  RK_S32 mFrameLen; // Statistics frame length, the longer it is, the harder it
                    // is to wake up
  RK_S32 mBlankFrameMax; // Reset the frame length, and re-statistics when the
                         // frame length exceeds it
  RK_FLOAT mCryEnergy;
  RK_FLOAT mJudgeEnergy;
  RK_FLOAT mCryThres1;
  RK_FLOAT mCryThres2;
} AI_BCD_CONFIG_S;

typedef struct rkAI_BCD_RESULT_S {
  RK_BOOL bBabyCry;
} AI_BCD_RESULT_S;

typedef struct rkAI_BUZ_CONFIG_S {
  RK_S32 mFrameLen; // Statistics frame length, the longer it is, the harder it
                    // is to wake up
  RK_S32 mBlankFrameMax; // Reset the frame length, and re-statistics when the
                         // frame length exceeds it
  RK_FLOAT mEnergyMean;
  RK_FLOAT mEnergyMax;
  RK_FLOAT mBuzThres1;
  RK_FLOAT mBuzThres2;
} AI_BUZ_CONFIG_S;

typedef struct rkAI_BUZ_RESULT_S {
  RK_BOOL bBuzz;
} AI_BUZ_RESULT_S;

typedef struct rkAI_GBS_CONFIG_S {
  RK_S32 mFrameLen; // Statistics frame length, the longer it is, the harder it
                    // is to wake up
} AI_GBS_CONFIG_S;

typedef struct rkAI_GBS_RESULT_S {
  RK_BOOL bGbs;
} AI_GBS_RESULT_S;

/**Defines the configure parameters of ANR.*/
typedef struct rkAUDIO_ANR_CONFIG_S {
  RK_FLOAT fNoiseFactor;
  RK_S32 s32SwU;
  RK_FLOAT fPsiMin;
  RK_FLOAT fPsiMax;
  RK_FLOAT fGmin;
} AUDIO_ANR_CONFIG_S;

typedef struct rkAUDIO_AGC_CONFIG_S {
  RK_FLOAT fAttackTime;
  RK_FLOAT fReleaseTime;
  RK_FLOAT fAttenuateTime;
  RK_FLOAT fMaxGain;
  RK_FLOAT fMaxPeak;
  RK_FLOAT fRth0;
  RK_FLOAT fRth1;
  RK_FLOAT fRth2;
  RK_FLOAT fRk0;
  RK_FLOAT fRk1;
  RK_FLOAT fRk2;
  RK_FLOAT fLineGainDb;
  RK_S32 s32SwSmL0;
  RK_S32 s32SwSmL1;
  RK_S32 s32SwSmL2;
} AUDIO_AGC_CONFIG_S;

typedef enum rkAO_VQE_MASK {
  AO_VQE_MASK_NONE = 1 << 0,
  AO_VQE_MASK_3A = 1 << 1,
  AO_VQE_MASK_AGC = 1 << 2,
  AO_VQE_MASK_ANR = 1 << 3,
} AO_VQE_MASK;

typedef struct rkAO_VQE_USER_CONFIG_S {
  RK_U32 u32OpenMask; /* see AO_VQE_MASK */

  AUDIO_AGC_CONFIG_S stAgcCfg;
  AUDIO_ANR_CONFIG_S stAnrCfg;
} AO_VQE_USER_CONFIG_S;

typedef struct rkAO_VQE_CONFIG_S {
  AIO_VQE_CONFIG_METHOD enCfgMode; /* see AIO_VQE_CONFIG_METHOD */
  RK_S32 s32WorkSampleRate; /* Sample Rate: 8KHz/16KHz/48KHz. default: 16KHz*/
  RK_S32 s32FrameSample; /* VQE frame length, default: 256 frames(16ms,16KHz) */
  union {
    /* config file if enCfgMode = AIO_VQE_CONFIG_LOAD_FILE */
    RK_CHAR aCfgFile[MAX_AUDIO_FILE_PATH_LEN];
    /* set user' parameters if enCfgMode = AIO_VQE_CONFIG_USER */
    AO_VQE_USER_CONFIG_S stUsrCfg;
  };
} AO_VQE_CONFIG_S;

typedef enum rkEN_AIO_ERR_CODE_E {
  AIO_ERR_VQE_ERR = 65, /*vqe error*/
} RK_AIO_ERR_CODE_E;

/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define RK_ERR_AIO_ILLEGAL_PARAM                                               \
  RK_DEF_ERR(RK_ID_AIO, RK_ERR_LEVEL_ERROR, RK_ERR_ILLEGAL_PARAM)
/* using a NULL point */
#define RK_ERR_AIO_NULL_PTR                                                    \
  RK_DEF_ERR(RK_ID_AIO, RK_ERR_LEVEL_ERROR, RK_ERR_NULL_PTR)
/* operation is not supported by NOW */
#define RK_ERR_AIO_NOT_PERM                                                    \
  RK_DEF_ERR(RK_ID_AIO, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_PERM)
/* vqe  err */
#define RK_ERR_AIO_REGISTER_ERR                                                \
  RK_DEF_ERR(RK_ID_AIO, RK_ERR_LEVEL_ERROR, AIO_ERR_VQE_ERR)

/* invlalid device ID */
#define RK_ERR_AI_INVALID_DEVID                                                \
  RK_DEF_ERR(RK_ID_AI, RK_ERR_LEVEL_ERROR, RK_ERR_INVALID_DEVID)
/* invlalid channel ID */
#define RK_ERR_AI_INVALID_CHNID                                                \
  RK_DEF_ERR(RK_ID_AI, RK_ERR_LEVEL_ERROR, RK_ERR_INVALID_CHNID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define RK_ERR_AI_ILLEGAL_PARAM                                                \
  RK_DEF_ERR(RK_ID_AI, RK_ERR_LEVEL_ERROR, RK_ERR_ILLEGAL_PARAM)
/* using a NULL point */
#define RK_ERR_AI_NULL_PTR                                                     \
  RK_DEF_ERR(RK_ID_AI, RK_ERR_LEVEL_ERROR, RK_ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing
 * attribute */
#define RK_ERR_AI_NOT_CONFIG                                                   \
  RK_DEF_ERR(RK_ID_AI, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define RK_ERR_AI_NOT_SUPPORT                                                  \
  RK_DEF_ERR(RK_ID_AI, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change stati attribute */
#define RK_ERR_AI_NOT_PERM                                                     \
  RK_DEF_ERR(RK_ID_AI, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_PERM)
/* the devide is not enabled  */
#define RK_ERR_AI_NOT_ENABLED                                                  \
  RK_DEF_ERR(RK_ID_AI, RK_ERR_LEVEL_ERROR, RK_ERR_UNEXIST)
/* failure caused by malloc memory */
#define RK_ERR_AI_NOMEM RK_DEF_ERR(RK_ID_AI, RK_ERR_LEVEL_ERROR, RK_ERR_NOMEM)
/* failure caused by malloc buffer */
#define RK_ERR_AI_NOBUF RK_DEF_ERR(RK_ID_AI, RK_ERR_LEVEL_ERROR, RK_ERR_NOBUF)
/* no data in buffer */
#define RK_ERR_AI_BUF_EMPTY                                                    \
  RK_DEF_ERR(RK_ID_AI, RK_ERR_LEVEL_ERROR, RK_ERR_BUF_EMPTY)
/* no buffer for new data */
#define RK_ERR_AI_BUF_FULL                                                     \
  RK_DEF_ERR(RK_ID_AI, RK_ERR_LEVEL_ERROR, RK_ERR_BUF_FULL)
/* system is not ready,had not initialed or loaded*/
#define RK_ERR_AI_SYS_NOTREADY                                                 \
  RK_DEF_ERR(RK_ID_AI, RK_ERR_LEVEL_ERROR, RK_ERR_NOTREADY)

#define RK_ERR_AI_BUSY RK_DEF_ERR(RK_ID_AI, RK_ERR_LEVEL_ERROR, RK_ERR_BUSY)
/* vqe  err */
#define RK_ERR_AI_VQE_ERR                                                      \
  RK_DEF_ERR(RK_ID_AI, RK_ERR_LEVEL_ERROR, AIO_ERR_VQE_ERR)

/* invlalid device ID */
#define RK_ERR_AO_INVALID_DEVID                                                \
  RK_DEF_ERR(RK_ID_AO, RK_ERR_LEVEL_ERROR, RK_ERR_INVALID_DEVID)
/* invlalid channel ID */
#define RK_ERR_AO_INVALID_CHNID                                                \
  RK_DEF_ERR(RK_ID_AO, RK_ERR_LEVEL_ERROR, RK_ERR_INVALID_CHNID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define RK_ERR_AO_ILLEGAL_PARAM                                                \
  RK_DEF_ERR(RK_ID_AO, RK_ERR_LEVEL_ERROR, RK_ERR_ILLEGAL_PARAM)
/* using a NULL point */
#define RK_ERR_AO_NULL_PTR                                                     \
  RK_DEF_ERR(RK_ID_AO, RK_ERR_LEVEL_ERROR, RK_ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing
 * attribute */
#define RK_ERR_AO_NOT_CONFIG                                                   \
  RK_DEF_ERR(RK_ID_AO, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define RK_ERR_AO_NOT_SUPPORT                                                  \
  RK_DEF_ERR(RK_ID_AO, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change stati attribute */
#define RK_ERR_AO_NOT_PERM                                                     \
  RK_DEF_ERR(RK_ID_AO, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_PERM)
/* the devide is not enabled  */
#define RK_ERR_AO_NOT_ENABLED                                                  \
  RK_DEF_ERR(RK_ID_AO, RK_ERR_LEVEL_ERROR, RK_ERR_UNEXIST)
/* failure caused by malloc memory */
#define RK_ERR_AO_NOMEM RK_DEF_ERR(RK_ID_AO, RK_ERR_LEVEL_ERROR, RK_ERR_NOMEM)
/* failure caused by malloc buffer */
#define RK_ERR_AO_NOBUF RK_DEF_ERR(RK_ID_AO, RK_ERR_LEVEL_ERROR, RK_ERR_NOBUF)
/* no data in buffer */
#define RK_ERR_AO_BUF_EMPTY                                                    \
  RK_DEF_ERR(RK_ID_AO, RK_ERR_LEVEL_ERROR, RK_ERR_BUF_EMPTY)
/* no buffer for new data */
#define RK_ERR_AO_BUF_FULL                                                     \
  RK_DEF_ERR(RK_ID_AO, RK_ERR_LEVEL_ERROR, RK_ERR_BUF_FULL)
/* system is not ready,had not initialed or loaded*/
#define RK_ERR_AO_SYS_NOTREADY                                                 \
  RK_DEF_ERR(RK_ID_AO, RK_ERR_LEVEL_ERROR, RK_ERR_NOTREADY)

#define RK_ERR_AO_BUSY RK_DEF_ERR(RK_ID_AO, RK_ERR_LEVEL_ERROR, RK_ERR_BUSY)
/* vqe  err */
#define RK_ERR_AO_VQE_ERR                                                      \
  RK_DEF_ERR(RK_ID_AO, RK_ERR_LEVEL_ERROR, AIO_ERR_VQE_ERR)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif // INCLUDE_RT_MPI_MPI_COMM_AIO_H_
