/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_RK_COMM_VI_H_
#define INCLUDE_RT_MPI_RK_COMM_VI_H_

#include "rk_comm_video.h"
#include "rk_common.h"
#include "rk_errno.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define VI_CHN0 0
#define VI_CHN1 1
#define VI_CHN2 2
#define VI_CHN3 3
#define VI_CHN4 4
#define VI_CHN5 5

#define MAX_VI_FILE_PATH_LEN 256
#define MAX_VI_FILE_NAME_LEN 256
#define MAX_VI_ENTITY_NAME_LEN 32
#define MAX_VI_BIND_PIPE_NUM 16

/* interface mode of video input */
typedef enum rkVI_INTF_MODE_E {
  VI_MODE_BT656 = 0,          /* ITU-R BT.656 YUV4:2:2 */
  VI_MODE_BT601,              /* ITU-R BT.601 YUV4:2:2 */
  VI_MODE_DIGITAL_CAMERA,     /* digatal camera mode */
  VI_MODE_BT1120_STANDARD,    /* BT.1120 progressive mode */
  VI_MODE_BT1120_INTERLEAVED, /* BT.1120 interstage mode */
  VI_MODE_MIPI,               /* MIPI RAW mode */
  VI_MODE_MIPI_YUV420_NORMAL, /* MIPI YUV420 normal mode */
  VI_MODE_MIPI_YUV420_LEGACY, /* MIPI YUV420 legacy mode */
  VI_MODE_MIPI_YUV422,        /* MIPI YUV422 mode */
  VI_MODE_LVDS,               /* LVDS mode */
  VI_MODE_SPI,                /* SPi mode */
  VI_MODE_SLVS,               /* SLVS mode */

  VI_MODE_BUTT
} VI_INTF_MODE_E;

/* Input mode */
typedef enum rkVI_INPUT_MODE_E {
  VI_INPUT_MODE_BT656 = 0,      /* ITU-R BT.656 YUV4:2:2 */
  VI_INPUT_MODE_BT601,          /* ITU-R BT.601 YUV4:2:2 */
  VI_INPUT_MODE_DIGITAL_CAMERA, /* digatal camera mode */
  VI_INPUT_MODE_INTERLEAVED,    /* interstage mode */
  VI_INPUT_MODE_MIPI,           /* MIPI mode */
  VI_INPUT_MODE_LVDS,           /* LVDS mode */
  VI_INPUT_MODE_SPI,            /* SPi mode */
  VI_INPUT_MODE_SLVS,           /* SLVS mode */

  VI_INPUT_MODE_BUTT
} VI_INPUT_MODE_E;

/* Work mode */
typedef enum rkVI_WORK_MODE_E {
  VI_WORK_MODE_1Multiplex = 0, /* 1 Multiplex mode */
  VI_WORK_MODE_2Multiplex,     /* 2 Multiplex mode */
  VI_WORK_MODE_3Multiplex,     /* 3 Multiplex mode */
  VI_WORK_MODE_4Multiplex,     /* 4 Multiplex mode */

  VI_WORK_MODE_BUTT
} VI_WORK_MODE_E;

/* Sequence of YUV data */
typedef enum rkVI_YUV_DATA_SEQ_E {
  /*
   * The input sequence of the second component
   * (only contains u and v) in BT.1120 mode is VUVU
   */
  VI_DATA_SEQ_VUVU = 0,
  /*
   * The input sequence of the second component
   * (only contains u and v) in BT.1120 mode is UVUV
   */
  VI_DATA_SEQ_UVUV,

  VI_DATA_SEQ_UYVY, /* The input sequence of YUV is UYVY */
  VI_DATA_SEQ_VYUY, /* The input sequence of YUV is VYUY */
  VI_DATA_SEQ_YUYV, /* The input sequence of YUV is YUYV */
  VI_DATA_SEQ_YVYU, /* The input sequence of YUV is YVYU */

  VI_DATA_SEQ_BUTT
} VI_YUV_DATA_SEQ_E;

/* Input data type */
typedef enum rkVI_DATA_TYPE_E {
  VI_DATA_TYPE_YUV = 0,
  VI_DATA_TYPE_RGB,

  VI_DATA_TYPE_BUTT
} VI_DATA_TYPE_E;

/* The attributes of a VI device */
typedef struct rkVI_DEV_ATTR_S {
  /* RW;Interface mode */
  VI_INTF_MODE_E enIntfMode;
  /* RW;Work mode */
  VI_WORK_MODE_E enWorkMode;
  /* The below members must be configured in BT.601 mode or DC mode and are
   * invalid in other modes */
  /* RW;Input data sequence (only the YUV format is supported) */
  VI_YUV_DATA_SEQ_E enDataSeq;
  /* RW;RGB: CSC-709 or CSC-601, PT YUV444 disable; YUV: default yuv CSC coef PT
   * YUV444 enable. */
  VI_DATA_TYPE_E enInputDataType;
  /* RW;Input max size */
  SIZE_S stMaxSize;
  /* RW;Data rate of Device */
  DATA_RATE_E enDataRate;
} VI_DEV_ATTR_S;

/* The status of chn */
typedef struct rkVI_DEV_STATUS_S {
  SIZE_S stSize;    /* RO;chn output size */
  RK_BOOL bProbeOk; /* RO;whether sensor is probed success */
} VI_DEV_STATUS_S;

/* Information of pipe binded to device */
typedef struct rkVI_DEV_BIND_PIPE_S {
  RK_U32 u32Num;                        /* RW;Range [1,VI_MAX_PHY_PIPE_NUM] */
  VI_PIPE PipeId[MAX_VI_BIND_PIPE_NUM]; /* RW;Array of pipe ID */
} VI_DEV_BIND_PIPE_S;

/* The attributes of pipe */
typedef struct rkVI_PIPE_ATTR_S {
  /* RW;Range:[0,1];ISP bypass enable */
  RK_BOOL bIspBypass;
  /* RW;Range:[0,1];Range[VI_PIPE_MIN_WIDTH,VI_PIPE_MAX_WIDTH];Maximum width */
  RK_U32 u32MaxW;
  /* RW;Range[VI_PIPE_MIN_HEIGHT,VI_PIPE_MAX_HEIGHT];Maximum height */
  RK_U32 u32MaxH;
  /* RW;Pixel format */
  PIXEL_FORMAT_E enPixFmt;
  /* RW;Range:[0,4];Compress mode.*/
  COMPRESS_MODE_E enCompressMode;
  /* RW;Range:[0,4];Bit width*/
  DATA_BITWIDTH_E enBitWidth;
  /* RW;Frame rate */
  FRAME_RATE_CTRL_S stFrameRate;
} VI_PIPE_ATTR_S;

typedef enum rkVI_ALLOC_BUF_TYPE_E {
  VI_ALLOC_BUF_TYPE_INTERNAL,
  VI_ALLOC_BUF_TYPE_EXTERNAL
} VI_ALLOC_BUF_TYPE_E;

typedef enum rkVI_V4L2_CAPTURE_TYPE {
  VI_V4L2_CAPTURE_TYPE_VIDEO_CAPTURE = 1,
  VI_V4L2_CAPTURE_TYPE_VBI_CAPTURE = 4,
  VI_V4L2_CAPTURE_TYPE_SLICED_VBI_CAPTURE = 6,
  VI_V4L2_CAPTURE_TYPE_VIDEO_CAPTURE_MPLANE = 9,
  VI_V4L2_CAPTURE_TYPE_SDR_CAPTURE = 11,
  VI_V4L2_CAPTURE_TYPE_META_CAPTURE = 13,
  /* Deprecated, do not use */
  VI_V4L2_CAPTURE_TYPE_PRIVATE = 0x80,
} VI_V4L2_CAPTURE_TYPE;

typedef enum rkVI_V4L2_MEMORY_TYPE {
  VI_V4L2_MEMORY_TYPE_MMAP = 1,
  VI_V4L2_MEMORY_TYPE_USERPTR = 2,
  VI_V4L2_MEMORY_TYPE_OVERLAY = 3,
  VI_V4L2_MEMORY_TYPE_DMABUF = 4,
} VI_V4L2_MEMORY_TYPE;

/* The attributes of channel for isp opt */
typedef struct rkVI_ISP_OPT_S {
  RK_U32 u32BufCount;                          /* RW;isp buf count */
  RK_U32 u32BufSize;                           /* R;isp buf size */
  VI_V4L2_CAPTURE_TYPE enCaptureType;          /* RW;isp capture type */
  VI_V4L2_MEMORY_TYPE enMemoryType;            /* RW;isp buf memory type */
  RK_CHAR aEntityName[MAX_VI_ENTITY_NAME_LEN]; /* RW;isp capture entity name*/
  RK_BOOL bNoUseLibV4L2;                       /* RW;is no use libv4l2 */
  SIZE_S stMaxSize; /* RW;isp output max resolution */
} VI_ISP_OPT_S;

/* The attributes of channel */
typedef struct rkVI_CHN_ATTR_S {
  SIZE_S stSize;                  /* RW;Channel out put size */
  PIXEL_FORMAT_E enPixelFormat;   /* RW;Pixel format */
  DYNAMIC_RANGE_E enDynamicRange; /* RW;Dynamic Range */
  VIDEO_FORMAT_E enVideoFormat;   /* RW;Video format */
  COMPRESS_MODE_E enCompressMode; /* RW;256B Segment compress or no compress. */
  RK_BOOL bMirror;                /* RW;Mirror enable */
  RK_BOOL bFlip;                  /* RW;Flip enable */
  RK_U32 u32Depth;                /* RW; Range: [0, 8]; User get list depth.*/
  FRAME_RATE_CTRL_S stFrameRate;  /* RW;Frame rate */
  VI_ALLOC_BUF_TYPE_E enAllocBufType; /* RW;channel alloc buf opt */
  VI_ISP_OPT_S stIspOpt;              /* RW;isp opt */
} VI_CHN_ATTR_S;

/* The status of chn */
typedef struct rkVI_CHN_STATUS_S {
  RK_BOOL bEnable;           /* RO;Whether this channel is enabled */
  RK_U32 u32FrameRate;       /* RO;current frame rate */
  RK_U32 u32CurFrameID;      /* RO;current frame id */
  RK_U32 u32InputLostFrame;  /* RO;input lost frame count */
  RK_U32 u32OutputLostFrame; /* RO;output lost frame count */
  RK_U32 u32VbFail;          /* RO;Video buffer malloc failure */
  SIZE_S stSize;             /* RO;chn output size */
} VI_CHN_STATUS_S;

/* Defines the configure parameters of VI saving file. */
typedef struct rkVI_SAVE_FILE_INFO_S {
  RK_BOOL bCfg;
  RK_CHAR aFilePath[MAX_VI_FILE_PATH_LEN];
  RK_CHAR aFileName[MAX_VI_FILE_NAME_LEN];
  RK_U32 u32FileSize; /*in KB*/
} VI_SAVE_FILE_INFO_S;

typedef struct rkVI_CHN_BUF_WRAP_S {
  RK_BOOL bEnable;
  RK_U32 u32BufLine; /* RW; Range: [128, H]; Chn buffer allocated by line. */
  RK_U32 u32WrapBufferSize; /* RW; Whether to allocate buffer according to
                               compression. */
} VI_CHN_BUF_WRAP_S;

/* struct rkisp_mirror_flip
 * mirror: global for all output stream
 * flip: independent for all output stream
 */
typedef struct rkISP_MIRROR_FLIP_S {
  RK_U8 mirror;
  RK_U8 flip;
} __attribute__((packed)) VI_ISP_MIRROR_FLIP_S;

typedef enum rkVI_CROP_COORDINATE_E {
  VI_CROP_RATIO_COOR = 0,
  VI_CROP_ABS_COOR,
  VI_CROP_BUTT
} VI_CROP_COORDINATE_E;

typedef struct rkVI_CROP_INFO_S {
  RK_BOOL bEnable;
  VI_CROP_COORDINATE_E enCropCoordinate;
  RECT_S stCropRect;
} VI_CROP_INFO_S;

#define RK_ERR_VI_INVALID_PARA                                                 \
  RK_DEF_ERR(RK_ID_VI, RK_ERR_LEVEL_ERROR, RK_ERR_ILLEGAL_PARAM)
#define RK_ERR_VI_INVALID_DEVID                                                \
  RK_DEF_ERR(RK_ID_VI, RK_ERR_LEVEL_ERROR, RK_ERR_INVALID_DEVID)
#define RK_ERR_VI_INVALID_PIPEID                                               \
  RK_DEF_ERR(RK_ID_VI, RK_ERR_LEVEL_ERROR, RK_ERR_INVALID_PIPEID)
#define RK_ERR_VI_INVALID_CHNID                                                \
  RK_DEF_ERR(RK_ID_VI, RK_ERR_LEVEL_ERROR, RK_ERR_INVALID_CHNID)
#define RK_ERR_VI_INVALID_NULL_PTR                                             \
  RK_DEF_ERR(RK_ID_VI, RK_ERR_LEVEL_ERROR, RK_ERR_NULL_PTR)
#define RK_ERR_VI_FAILED_NOTCONFIG                                             \
  RK_DEF_ERR(RK_ID_VI, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_CONFIG)
#define RK_ERR_VI_SYS_NOTREADY                                                 \
  RK_DEF_ERR(RK_ID_VI, RK_ERR_LEVEL_ERROR, RK_ERR_NOTREADY)
#define RK_ERR_VI_BUF_EMPTY                                                    \
  RK_DEF_ERR(RK_ID_VI, RK_ERR_LEVEL_ERROR, RK_ERR_BUF_EMPTY)
#define RK_ERR_VI_BUF_FULL                                                     \
  RK_DEF_ERR(RK_ID_VI, RK_ERR_LEVEL_ERROR, RK_ERR_BUF_FULL)
#define RK_ERR_VI_NOMEM RK_DEF_ERR(RK_ID_VI, RK_ERR_LEVEL_ERROR, RK_ERR_NOMEM)
#define RK_ERR_VI_NOT_SUPPORT                                                  \
  RK_DEF_ERR(RK_ID_VI, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_SUPPORT)
#define RK_ERR_VI_BUSY RK_DEF_ERR(RK_ID_VI, RK_ERR_LEVEL_ERROR, RK_ERR_BUSY)
#define RK_ERR_VI_NOT_PERM                                                     \
  RK_DEF_ERR(RK_ID_VI, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_PERM)
/* try to enable or initialize system,device or pipe or channel, before
 * configing attribute */
#define RK_ERR_VI_NOT_CONFIG                                                   \
  RK_DEF_ERR(RK_ID_VI, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_CONFIG)
/* channel exists */
#define RK_ERR_VI_EXIST RK_DEF_ERR(RK_ID_VI, RK_ERR_LEVEL_ERROR, RK_ERR_EXIST)
/* the channel is not existed  */
#define RK_ERR_VI_UNEXIST                                                      \
  RK_DEF_ERR(RK_ID_VI, RK_ERR_LEVEL_ERROR, RK_ERR_UNEXIST)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* End of #ifndef INCLUDE_RT_MPI_RK_COMM_VI_H_ */
