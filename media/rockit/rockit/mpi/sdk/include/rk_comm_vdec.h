/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_RK_COMM_VDEC_H_

#define INCLUDE_RT_MPI_RK_COMM_VDEC_H_
#include "rk_comm_mb.h"
#include "rk_comm_video.h"
#include "rk_common.h"
#include "rk_errno.h"
#include "rk_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define RK_IO_BLOCK RK_TRUE
#define RK_IO_NOBLOCK RK_FALSE

typedef enum rkVIDEO_DEC_MODE_E {
  VIDEO_DEC_MODE_IPB = 0,
  VIDEO_DEC_MODE_IP,
  VIDEO_DEC_MODE_I,
  VIDEO_DEC_MODE_BUTT
} VIDEO_DEC_MODE_E;

typedef enum rkVIDEO_OUTPUT_ORDER_E {
  VIDEO_OUTPUT_ORDER_DISP = 0,
  VIDEO_OUTPUT_ORDER_DEC,
  VIDEO_OUTPUT_ORDER_BUTT
} VIDEO_OUTPUT_ORDER_E;

typedef enum rkVIDEO_MODE_E {
  VIDEO_MODE_STREAM = 0, /* send by stream */
  VIDEO_MODE_FRAME,      /* send by frame  */
  VIDEO_MODE_COMPAT,     /* One frame supports multiple packets sending. */
  /* The current frame is considered to end when bEndOfFrame is equal to RK_TRUE
   */
  VIDEO_MODE_BUTT
} VIDEO_MODE_E;

typedef struct rkVDEC_PARAM_VIDEO_S {
  RK_BOOL bDeiEn;         /* RW, deinterlace enable */
  RK_S32 s32ErrThreshold; /* RW, Range: [0, 100]; */
  /* threshold for stream error process, 0: discard with any error, 100 : keep
   * data with any error */
  VIDEO_DEC_MODE_E enDecMode; /* RW; */
  /* decode mode , 0: deocde IPB frames, 1: only decode I frame & P frame , 2:
   * only decode I frame */
  VIDEO_OUTPUT_ORDER_E enOutputOrder; /* RW; */
  /* frames output order ,0: the same with display order , 1: the same width
   * decoder order */
  COMPRESS_MODE_E enCompressMode; /* RW; compress mode */
  VIDEO_FORMAT_E enVideoFormat;   /* RW; video format */
} VDEC_PARAM_VIDEO_S;

typedef struct rkVDEC_PARAM_PICTURE_S {
  PIXEL_FORMAT_E enPixelFormat; /* RW; out put pixel format */
  RK_U32 u32Alpha; /* RW, Range: [0, 255]; value 0 is transparent. */
  /* [0 ,127]   is deemed to transparent when enPixelFormat is ARGB1555 or
   * ABGR1555 [128 ,256] is deemed to non-transparent when enPixelFormat is
   * ARGB1555 or ABGR1555
   */
} VDEC_PARAM_PICTURE_S;

typedef struct rkVDEC_CHN_PARAM_S {
  RK_CODEC_ID_E enType;      /* RW; video type to be decoded   */
  RK_U32 u32DisplayFrameNum; /* RW, Range: [0, 16]; display frame num */
  union {
    VDEC_PARAM_VIDEO_S
    stVdecVideoParam; /* structure with video ( h265/h264/mpeg2/mpeg4) */
    VDEC_PARAM_PICTURE_S
    stVdecPictureParam; /* structure with picture (jpeg/mjpeg ) */
  };
} VDEC_CHN_PARAM_S;

typedef struct rkVDEC_ATTR_VIDEO_S {
  RK_U32 u32RefFrameNum;      /* RW, Range: [0, 16]; reference frame num. */
  RK_BOOL bTemporalMvpEnable; /* RW; */
  /* specifies whether temporal motion vector predictors can be used for inter
   * prediction */
  RK_U32 u32TmvBufSize; /* RW; tmv buffer size(Byte) */
} VDEC_ATTR_VIDEO_S;

typedef struct rkVDEC_CHN_ATTR_S {
  VIDEO_MODE_E enMode;
  RK_CODEC_ID_E enType;    /* RW; video type to be decoded   */
  RK_U32 u32PicWidth;      /* RW; pic width */
  RK_U32 u32PicHeight;     /* RW; pic height */
  RK_U32 u32PicVirWidth;   /* RW; pic virtual width */
  RK_U32 u32PicVirHeight;  /* RW; pic  virtual height */
  RK_U32 u32StreamBufSize; /* RW; stream buffer size(Byte) */
  RK_U32 u32FrameBufSize;  /* RW; frame buffer size(Byte) */
  RK_U32 u32FrameBufCnt;   /* RW; frame buffer cnt */
  RK_U32 u32StreamBufCnt;  /* RW; stream buffer cnt */
  union {
    VDEC_ATTR_VIDEO_S stVdecVideoAttr; /* structure with video ( h264/h265) */
  };
} VDEC_CHN_ATTR_S;

typedef struct rkVDEC_STREAM_S {
  MB_BLK pMbBlk;
  RK_U32 u32Len;        /* W; stream len */
  RK_U64 u64PTS;        /* W; time stamp */
  RK_BOOL bEndOfStream; /* W; is the end of all stream */
  RK_BOOL bEndOfFrame;  /* W; is the end of frame */
  RK_BOOL bBypassMbBlk; /* FALSE: copy, TRUE: MbBlock owned by internal */
} VDEC_STREAM_S;

typedef struct rkVDEC_DECODE_ERROR_S {
  RK_S32 s32FormatErr;      /* R; format error. eg: do not support filed */
  RK_S32 s32PicSizeErrSet;  /* R; picture width or height is larger than chnnel
                               width or height */
  RK_S32 s32StreamUnsprt;   /* R; unsupport the stream specification */
  RK_S32 s32PackErr;        /* R; stream package error */
  RK_S32 s32PrtclNumErrSet; /* R; protocol num is not enough. eg: slice, pps,
                               sps */
  RK_S32 s32RefErrSet;      /* R; refrence num is not enough */
  RK_S32 s32PicBufSizeErrSet; /* R; the buffer size of picture is not enough */
  RK_S32 s32StreamSizeOver;   /* R; the stream size is too big and and force
                                 discard stream */
  RK_S32 s32VdecStreamNotRelease; /* R; the stream not released for too long
                                     time */
} VDEC_DECODE_ERROR_S;

typedef struct rkVDEC_CHN_STATUS_S {
  RK_CODEC_ID_E enType;       /* R; video type to be decoded */
  RK_U32 u32LeftStreamBytes;  /* R; left stream bytes waiting for decode */
  RK_U32 u32LeftStreamFrames; /* R; left frames waiting for decode,only valid
                                 for VIDEO_MODE_FRAME */
  RK_U32 u32LeftPics;         /* R; pics waiting for output */
  RK_BOOL bStartRecvStream;   /* R; had started recv stream? */
  RK_U32 u32RecvStreamFrames; /* R; how many frames of stream has been received.
                                 valid when send by frame. */
  RK_U32 u32DecodeStreamFrames;     /* R; how many frames of stream has been
                                       decoded. valid when send by frame. */
  VDEC_DECODE_ERROR_S stVdecDecErr; /* R; information about decode error */
  RK_U32 u32Width;
  RK_U32 u32Height;
} VDEC_CHN_STATUS_S;

typedef enum rkVDEC_EVNT_E {
  VDEC_EVNT_STREAM_ERR = 1,
  VDEC_EVNT_UNSUPPORT,
  VDEC_EVNT_OVER_REFTHR,
  VDEC_EVNT_REF_NUM_OVER,
  VDEC_EVNT_SLICE_NUM_OVER,
  VDEC_EVNT_SPS_NUM_OVER,
  VDEC_EVNT_PPS_NUM_OVER,
  VDEC_EVNT_PICBUF_SIZE_ERR,
  VDEC_EVNT_SIZE_OVER,
  VDEC_EVNT_IMG_SIZE_CHANGE,
  VDEC_EVNT_VPS_NUM_OVER,
  VDEC_EVNT_BUTT
} VDEC_EVNT_E;

typedef struct rkVDEC_PICTURE_MOD_PARAM_S {
  RK_U32 u32MaxPicWidth;
  RK_U32 u32MaxPicHeight;
  RK_BOOL bSupportProgressive;
  RK_BOOL bDynamicAllocate;
} VDEC_PICTURE_MOD_PARAM_S;

typedef struct rkVDEC_VIDEO_MOD_PARAM_S {
  RK_U32 u32MaxPicWidth;
  RK_U32 u32MaxPicHeight;
  RK_U32 u32MaxSliceNum;
  RK_U32 u32VdhMsgNum;
  RK_U32 u32VdhBinSize;
  RK_U32 u32VdhExtMemLevel;
} VDEC_VIDEO_MOD_PARAM_S;

typedef struct rkVDEC_MOD_PARAM_S {
  MB_SOURCE_E enVdecMBSource; /* RW, Range: [1, 3];  frame buffer mode  */
  RK_U32 u32MiniBufMode;      /* RW, Range: [0, 1];  stream buffer mode */
  RK_U32 u32ParallelMode;     /* RW, Range: [0, 1];  VDH working mode   */
  VDEC_VIDEO_MOD_PARAM_S stVideoModParam;
  VDEC_PICTURE_MOD_PARAM_S stPictureModParam;
} VDEC_MOD_PARAM_S;

/*********************************************************************************************/
/* invlalid channel ID */
#define RK_ERR_VDEC_INVALID_CHNID                                              \
  RK_DEF_ERR(RK_ID_VDEC, RK_ERR_LEVEL_ERROR, RK_ERR_INVALID_CHNID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define RK_ERR_VDEC_ILLEGAL_PARAM                                              \
  RK_DEF_ERR(RK_ID_VDEC, RK_ERR_LEVEL_ERROR, RK_ERR_ILLEGAL_PARAM)
/* channel exists */
#define RK_ERR_VDEC_EXIST                                                      \
  RK_DEF_ERR(RK_ID_VDEC, RK_ERR_LEVEL_ERROR, RK_ERR_EXIST)
/* using a NULL point */
#define RK_ERR_VDEC_NULL_PTR                                                   \
  RK_DEF_ERR(RK_ID_VDEC, RK_ERR_LEVEL_ERROR, RK_ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing
 * attribute */
#define RK_ERR_VDEC_NOT_CONFIG                                                 \
  RK_DEF_ERR(RK_ID_VDEC, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define RK_ERR_VDEC_NOT_SUPPORT                                                \
  RK_DEF_ERR(RK_ID_VDEC, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change stati attribute */
#define RK_ERR_VDEC_NOT_PERM                                                   \
  RK_DEF_ERR(RK_ID_VDEC, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_PERM)
/* the channle is not existed  */
#define RK_ERR_VDEC_UNEXIST                                                    \
  RK_DEF_ERR(RK_ID_VDEC, RK_ERR_LEVEL_ERROR, RK_ERR_UNEXIST)
/* failure caused by malloc memory */
#define RK_ERR_VDEC_NOMEM                                                      \
  RK_DEF_ERR(RK_ID_VDEC, RK_ERR_LEVEL_ERROR, RK_ERR_NOMEM)
/* failure caused by malloc buffer */
#define RK_ERR_VDEC_NOBUF                                                      \
  RK_DEF_ERR(RK_ID_VDEC, RK_ERR_LEVEL_ERROR, RK_ERR_NOBUF)
/* no data in buffer */
#define RK_ERR_VDEC_BUF_EMPTY                                                  \
  RK_DEF_ERR(RK_ID_VDEC, RK_ERR_LEVEL_ERROR, RK_ERR_BUF_EMPTY)
/* no buffer for new data */
#define RK_ERR_VDEC_BUF_FULL                                                   \
  RK_DEF_ERR(RK_ID_VDEC, RK_ERR_LEVEL_ERROR, RK_ERR_BUF_FULL)
/* system is not ready,had not initialed or loaded */
#define RK_ERR_VDEC_SYS_NOTREADY                                               \
  RK_DEF_ERR(RK_ID_VDEC, RK_ERR_LEVEL_ERROR, RK_ERR_NOTREADY)
/* system busy */
#define RK_ERR_VDEC_BUSY RK_DEF_ERR(RK_ID_VDEC, RK_ERR_LEVEL_ERROR, RK_ERR_BUSY)
/* bad address,  eg. used for copy_from_user & copy_to_user   */
#define RK_ERR_VDEC_BADADDR                                                    \
  RK_DEF_ERR(RK_ID_VDEC, RK_ERR_LEVEL_ERROR, RK_ERR_BADADDR)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef  INCLUDE_RT_MPI_RK_COMM_VDEC_H_ */
