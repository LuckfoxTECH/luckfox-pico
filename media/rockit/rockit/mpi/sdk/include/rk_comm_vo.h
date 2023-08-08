/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_RK_COMMON_VO_H_
#define INCLUDE_RT_MPI_RK_COMMON_VO_H_

#include "rk_comm_video.h"
#include "rk_common.h"
#include "rk_type.h"

#define VO_DEF_WBC_DEPTH_LEN 8

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef enum rkEN_VOU_ERR_CODE_E {
  EN_ERR_VO_DEV_NOT_CONFIG = 0x40,
  EN_ERR_VO_DEV_NOT_ENABLE = 0x41,
  EN_ERR_VO_DEV_HAS_ENABLED = 0x42,
  EN_ERR_VO_DEV_HAS_BINDED = 0x43,
  EN_ERR_VO_DEV_NOT_BINDED = 0x44,

  ERR_VO_NOT_ENABLE = 0x45,
  ERR_VO_NOT_DISABLE = 0x46,
  ERR_VO_NOT_CONFIG = 0x47,

  ERR_VO_CHN_NOT_DISABLE = 0x48,
  ERR_VO_CHN_NOT_ENABLE = 0x49,
  ERR_VO_CHN_NOT_CONFIG = 0x4a,
  ERR_VO_CHN_NOT_ALLOC = 0x4b,

  ERR_VO_CCD_INVALID_PAT = 0x4c,
  ERR_VO_CCD_INVALID_POS = 0x4d,

  ERR_VO_WAIT_TIMEOUT = 0x4e,
  ERR_VO_INVALID_VFRAME = 0x4f,
  ERR_VO_INVALID_RECT_PARA = 0x50,
  ERR_VO_SETBEGIN_ALREADY = 0x51,
  ERR_VO_SETBEGIN_NOTYET = 0x52,
  ERR_VO_SETEND_ALREADY = 0x53,
  ERR_VO_SETEND_NOTYET = 0x54,

  ERR_VO_WBC_NOT_DISABLE = 0x55,
  ERR_VO_WBC_NOT_CONFIG = 0x56,

  ERR_VO_CHN_AREA_OVERLAP = 0x57,

  EN_ERR_INVALID_WBCID = 0x58,
  EN_ERR_INVALID_LAYERID = 0x59,
  EN_ERR_VO_LAYER_HAS_BINDED = 0x5a,
  EN_ERR_VO_LAYER_NOT_BINDED = 0x5b,
  ERR_VO_WBC_HAS_BIND = 0x5c,
  ERR_VO_WBC_HAS_CONFIG = 0x5d,
  ERR_VO_WBC_NOT_BIND = 0x5e,

  /* new added */
  ERR_VO_BUTT
} EN_VOU_ERR_CODE_E;

/* System define error code */
#define RK_ERR_VO_BUSY RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, RK_ERR_BUSY)
#define RK_ERR_VO_NO_MEM RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, RK_ERR_NOMEM)
#define RK_ERR_VO_NULL_PTR                                                     \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, RK_ERR_NULL_PTR)
#define RK_ERR_VO_SYS_NOTREADY                                                 \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, RK_ERR_NOTREADY)
#define RK_ERR_VO_INVALID_DEVID                                                \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, RK_ERR_INVALID_DEVID)
#define RK_ERR_VO_INVALID_CHNID                                                \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, RK_ERR_INVALID_CHNID)
#define RK_ERR_VO_ILLEGAL_PARAM                                                \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, RK_ERR_ILLEGAL_PARAM)
#define RK_ERR_VO_NOT_SUPPORT                                                  \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_SUPPORT)
#define RK_ERR_VO_NOT_PERMIT                                                   \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_PERM)
#define RK_ERR_VO_INVALID_WBCID                                                \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, EN_ERR_INVALID_WBCID)
#define RK_ERR_VO_INVALID_LAYERID                                              \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, EN_ERR_INVALID_LAYERID)

/* Device relative error code */
#define RK_ERR_VO_DEV_NOT_CONFIG                                               \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, EN_ERR_VO_DEV_NOT_CONFIG)
#define RK_ERR_VO_DEV_NOT_ENABLE                                               \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, EN_ERR_VO_DEV_NOT_ENABLE)
#define RK_ERR_VO_DEV_HAS_ENABLED                                              \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, EN_ERR_VO_DEV_HAS_ENABLED)
#define RK_ERR_VO_DEV_HAS_BINDED                                               \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, EN_ERR_VO_DEV_HAS_BINDED)
#define RK_ERR_VO_DEV_NOT_BINDED                                               \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, EN_ERR_VO_DEV_NOT_BINDED)

/* layer relative error code */
#define RK_ERR_VO_LAYER_NOT_ENABLE                                             \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, ERR_VO_NOT_ENABLE)
#define RK_ERR_VO_LAYER_NOT_DISABLE                                            \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, ERR_VO_NOT_DISABLE)
#define RK_ERR_VO_LAYER_NOT_CONFIG                                             \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, ERR_VO_NOT_CONFIG)
#define RK_ERR_VO_LAYER_HAS_BINDED                                             \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, EN_ERR_VO_LAYER_HAS_BINDED)
#define RK_ERR_VO_LAYER_NOT_BINDED                                             \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, EN_ERR_VO_LAYER_NOT_BINDED)

/* WBC Relative error code */

#define RK_ERR_VO_WBC_NOT_DISABLE                                              \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, ERR_VO_WBC_NOT_DISABLE)
#define RK_ERR_VO_WBC_NOT_CONFIG                                               \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, ERR_VO_WBC_NOT_CONFIG)
#define RK_ERR_VO_WBC_HAS_CONFIG                                               \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, ERR_VO_WBC_HAS_CONFIG)
#define RK_ERR_VO_WBC_NOT_BIND                                                 \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, ERR_VO_WBC_NOT_BIND)
#define RK_ERR_VO_WBC_HAS_BIND                                                 \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, ERR_VO_WBC_HAS_BIND)

/* Channel Relative error code */
#define RK_ERR_VO_CHN_NOT_DISABLE                                              \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, ERR_VO_CHN_NOT_DISABLE)
#define RK_ERR_VO_CHN_NOT_ENABLE                                               \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, ERR_VO_CHN_NOT_ENABLE)
#define RK_ERR_VO_CHN_NOT_CONFIG                                               \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, ERR_VO_CHN_NOT_CONFIG)
#define RK_ERR_VO_CHN_NOT_ALLOC                                                \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, ERR_VO_CHN_NOT_ALLOC)
#define RK_ERR_VO_CHN_AREA_OVERLAP                                             \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, ERR_VO_CHN_AREA_OVERLAP)

/* Cascade Relatvie error code */
#define RK_ERR_VO_INVALID_PATTERN                                              \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, ERR_VO_CCD_INVALID_PAT)
#define RK_ERR_VO_INVALID_POSITION                                             \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, ERR_VO_CCD_INVALID_POS)

/* MISCellaneous error code */
#define RK_ERR_VO_WAIT_TIMEOUT                                                 \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, ERR_VO_WAIT_TIMEOUT)
#define RK_ERR_VO_INVALID_VFRAME                                               \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, ERR_VO_INVALID_VFRAME)
#define RK_ERR_VO_INVALID_RECT_PARA                                            \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, ERR_VO_INVALID_RECT_PARA)
#define RK_ERR_VO_SETBEGIN_ALREADY                                             \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, ERR_VO_SETBEGIN_ALREADY)
#define RK_ERR_VO_SETBEGIN_NOTYET                                              \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, ERR_VO_SETBEGIN_NOTYET)
#define RK_ERR_VO_SETEND_ALREADY                                               \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, ERR_VO_SETEND_ALREADY)
#define RK_ERR_VO_SETEND_NOTYET                                                \
  RK_DEF_ERR(RK_ID_VO, RK_ERR_LEVEL_ERROR, ERR_VO_SETEND_NOTYET)

/* VO video output interface type */
#define VO_INTF_CVBS (0x01L << 0)
#define VO_INTF_YPBPR (0x01L << 1)
#define VO_INTF_VGA (0x01L << 2)
#define VO_INTF_BT656 (0x01L << 3)
#define VO_INTF_BT1120 (0x01L << 4)
#define VO_INTF_LCD (0x01L << 6)
#define VO_INTF_LVDS (0x01L << 7)
#define VO_INTF_MIPI (0x01L << 9)
#define VO_INTF_MIPI1 (0x01L << 10)
#define VO_INTF_EDP (0x01L << 11)
#define VO_INTF_EDP1 (0x01L << 12)
#define VO_INTF_HDMI (0x01L << 13)
#define VO_INTF_HDMI1 (0x01L << 14)
#define VO_INTF_DP (0x01L << 15)
#define VO_INTF_DP1 (0x01L << 16)
#define VO_INTF_DEFAULT (0x01L << 17)

#define VO_INTF_NUM 18

typedef RK_U32 VO_INTF_TYPE_E;

typedef enum rkVO_INTF_SYNC_E {
  VO_OUTPUT_PAL = 0, /* PAL standard */
  VO_OUTPUT_NTSC,    /* NTSC standard */

  VO_OUTPUT_1080P24, /* 1920 x 1080 at 24 Hz. */
  VO_OUTPUT_1080P25, /* 1920 x 1080 at 25 Hz. */
  VO_OUTPUT_1080P30, /* 1920 x 1080 at 30 Hz. */

  VO_OUTPUT_720P50, /* 1280 x  720 at 50 Hz. */
  VO_OUTPUT_720P60, /* 1280 x  720 at 60 Hz. */

  VO_OUTPUT_1080I50, /* 1920 x 1080 at 50 Hz, interlace. */
  VO_OUTPUT_1080I60, /* 1920 x 1080 at 60 Hz, interlace. */
  VO_OUTPUT_1080P50, /* 1920 x 1080 at 50 Hz. */
  VO_OUTPUT_1080P60, /* 1920 x 1080 at 60 Hz. */

  VO_OUTPUT_576P50, /* 720  x  576 at 50 Hz. */
  VO_OUTPUT_480P60, /* 720  x  480 at 60 Hz. */

  VO_OUTPUT_800x600_60,   /* VESA 800 x 600 at 60 Hz (non-interlaced) */
  VO_OUTPUT_1024x768_60,  /* VESA 1024 x 768 at 60 Hz (non-interlaced) */
  VO_OUTPUT_1280x1024_60, /* VESA 1280 x 1024 at 60 Hz (non-interlaced) */
  VO_OUTPUT_1366x768_60,  /* VESA 1366 x 768 at 60 Hz (non-interlaced) */
  VO_OUTPUT_1440x900_60,  /* VESA 1440 x 900 at 60 Hz (non-interlaced) CVT
                             Compliant */
  VO_OUTPUT_1280x800_60,  /* 1280*800@60Hz VGA@60Hz */
  VO_OUTPUT_1600x1200_60, /* VESA 1600 x 1200 at 60 Hz (non-interlaced) */
  VO_OUTPUT_1680x1050_60, /* VESA 1680 x 1050 at 60 Hz (non-interlaced) */
  VO_OUTPUT_1920x1200_60, /* VESA 1920 x 1600 at 60 Hz (non-interlaced) CVT
                             (Reduced Blanking) */
  VO_OUTPUT_640x480_60,   /* VESA 640 x 480 at 60 Hz (non-interlaced) CVT */
  VO_OUTPUT_960H_PAL,     /* ITU-R BT.1302 960 x 576 at 50 Hz (interlaced) */
  VO_OUTPUT_960H_NTSC,    /* ITU-R BT.1302 960 x 480 at 60 Hz (interlaced) */
  VO_OUTPUT_1920x2160_30, /* 1920x2160_30 */
  VO_OUTPUT_2560x1440_30, /* 2560x1440_30 */
  VO_OUTPUT_2560x1440_60, /* 2560x1440_60 */
  VO_OUTPUT_2560x1600_60, /* 2560x1600_60 */
  VO_OUTPUT_3840x2160_24, /* 3840x2160_24 */
  VO_OUTPUT_3840x2160_25, /* 3840x2160_25 */
  VO_OUTPUT_3840x2160_30, /* 3840x2160_30 */
  VO_OUTPUT_3840x2160_50, /* 3840x2160_50 */
  VO_OUTPUT_3840x2160_60, /* 3840x2160_60 */
  VO_OUTPUT_4096x2160_24, /* 4096x2160_24 */
  VO_OUTPUT_4096x2160_25, /* 4096x2160_25 */
  VO_OUTPUT_4096x2160_30, /* 4096x2160_30 */
  VO_OUTPUT_4096x2160_50, /* 4096x2160_50 */
  VO_OUTPUT_4096x2160_60, /* 4096x2160_60 */
  /* For HDMI2.1  */
  VO_OUTPUT_7680x4320_24, /* 7680x4320_24 */
  VO_OUTPUT_7680x4320_25, /* 7680x4320_25 */
  VO_OUTPUT_7680x4320_30, /* 7680x4320_30 */
  VO_OUTPUT_7680x4320_50, /* 7680x4320_50 */
  VO_OUTPUT_7680x4320_60, /* 7680x4320_60 */

  VO_OUTPUT_3840x1080_60, /* For split mode */

  VO_OUTPUT_USER, /* User timing. */
  VO_OUTPUT_DEFAULT,

  VO_OUTPUT_BUTT
} VO_INTF_SYNC_E;

typedef enum rkVO_ZOOM_IN_E {
  VO_ZOOM_IN_RECT = 0, /* Zoom in by rect */
  VO_ZOOM_IN_RATIO,    /* Zoom in by ratio */
  VO_ZOOM_IN_BUTT
} VO_ZOOM_IN_E;

typedef enum rkVO_CSC_MATRIX_E {
  VO_CSC_MATRIX_IDENTITY = 0, /* Identity CSC matrix. */

  VO_CSC_MATRIX_BT601_TO_BT709, /* BT601 to BT709 */
  VO_CSC_MATRIX_BT709_TO_BT601, /* BT709 to BT601 */

  VO_CSC_MATRIX_BT601_TO_RGB_PC, /* BT601 to RGB */
  VO_CSC_MATRIX_BT709_TO_RGB_PC, /* BT709 to RGB */

  VO_CSC_MATRIX_RGB_TO_BT601_PC, /* RGB to BT601 FULL */
  VO_CSC_MATRIX_RGB_TO_BT709_PC, /* RGB to BT709 FULL */

  VO_CSC_MATRIX_RGB_TO_BT2020_PC, /* RGB to BT.2020 */
  VO_CSC_MATRIX_BT2020_TO_RGB_PC, /* BT.2020 to RGB */

  VO_CSC_MATRIX_RGB_TO_BT601_TV, /* RGB to BT601 LIMIT */
  VO_CSC_MATRIX_RGB_TO_BT709_TV, /* RGB to BT709 LIMIT */

  VO_CSC_MATRIX_BUTT
} VO_CSC_MATRIX_E;

typedef struct rkVO_CHN_ATTR_S {
  RK_U32 u32Priority;    /* Video out overlay pri sd */
  RECT_S stRect;         /* Rectangle of video output channel */
  RK_BOOL bDeflicker;    /* Deflicker or not sd */
  RK_U32 u32FgAlpha;     /* Alpha of A = 1 in pixel format BGRA5551/RGBA5551 */
  RK_U32 u32BgAlpha;     /* Alpha of A = 0 in pixel format BGRA5551/RGBA5551 */
  RK_BOOL bEnKeyColor;   /* Enable key color or not when pixel format
                            BGRA5551/RGBA5551 */
  RK_U32 u32KeyColor;    /* Key color value of pixel format BGRA5551/RGBA5551,
                            B[0:4] G[5:9] R[10:14] */
  MIRROR_E enMirror;     /* RW, Mirror */
  ROTATION_E enRotation; /* RW, rotation. */
} VO_CHN_ATTR_S;

typedef struct rkVO_CHN_PARAM_S {
  ASPECT_RATIO_S stAspectRatio; /* RW; aspect ratio */
} VO_CHN_PARAM_S;

typedef struct rkVO_BORDER_S {
  RK_BOOL bBorderEn; /* RW; Do frame or not */
  BORDER_S stBorder; /* RW; frame's top, bottom, left, right width and color */
} VO_BORDER_S;

typedef struct rkVO_QUERY_STATUS_S {
  RK_U32 u32ChnBufUsed; /* Channel buffer that been occupied */
} VO_QUERY_STATUS_S;

typedef struct rkVO_SYNC_INFO_S {
  RK_BOOL bSynm; /* RW; sync mode(0:timing,as BT.656; 1:signal,as LCD) */
  RK_BOOL bIop;  /* RW; interlaced or progressive display(0:i; 1:p) */

  RK_U16 u16Vact; /* RW; vertical active area */
  RK_U16 u16Vbb;  /* RW; vertical back blank porch */
  RK_U16 u16Vfb;  /* RW; vertical front blank porch */

  RK_U16 u16Hact; /* RW; horizontal active area */
  RK_U16 u16Hbb;  /* RW; horizontal back blank porch */
  RK_U16 u16Hfb;  /* RW; horizontal front blank porch */
  RK_U16 u16Hmid; /* RW; bottom horizontal active area */

  RK_U16 u16Bvact; /* RW; bottom vertical active area */
  RK_U16 u16Bvbb;  /* RW; bottom vertical back blank porch */
  RK_U16 u16Bvfb;  /* RW; bottom vertical front blank porch */

  RK_U16 u16Hpw; /* RW; horizontal pulse width */
  RK_U16 u16Vpw; /* RW; vertical pulse width */

  RK_BOOL bIdv; /* RW; inverse data valid of output */
  RK_BOOL bIhs; /* RW; polarity of horizontal synch signal, 0: negative, 1:
                   positive */
  RK_BOOL bIvs; /* RW; polarity of vertical synch signal, 0: negative, 1:
                   positive */

  RK_U16 u16FrameRate; /* RW; frame rate of output */
  RK_U16 u16PixClock;  /* RW; pixel clock, the unit is KHZ */
} VO_SYNC_INFO_S;

typedef struct rkVO_PUB_ATTR_S {
  RK_U32 u32BgColor; /* RW; Background color of a device, in RGB format. */
  VO_INTF_TYPE_E enIntfType; /* RW; Type of a VO interface */
  VO_INTF_SYNC_E enIntfSync; /* RW; Type of a VO interface timing */
  VO_SYNC_INFO_S stSyncInfo; /* RW; Information about VO interface timings */
} VO_PUB_ATTR_S;

typedef struct rkVO_WBC_ATTR_S {
  SIZE_S stTargetSize;            /* RW; WBC Zoom target size */
  PIXEL_FORMAT_E enPixelFormat;   /* RW; the pixel format of WBC output */
  RK_U32 u32FrameRate;            /* RW; frame rate control */
  DYNAMIC_RANGE_E enDynamicRange; /* RW; Write back dynamic range type */
  COMPRESS_MODE_E enCompressMode; /* RW; Write back Compressing mode   */
} VO_WBC_ATTR_S;

typedef enum rkVO_WBC_MODE_E {
  /*
   * In this mode, wbc will capture frames according
   * to dev frame rate and wbc frame rate
   */
  VO_WBC_MODE_NORMAL = 0,
  /*
   * In this mode, wbc will drop dev repeat frame,
   * and capture the real frame according to video layer's
   * display rate and wbc frame rate
   */
  VO_WBC_MODE_DROP_REPEAT,
  /*
   * In this mode, wbc will drop dev repeat frame
   * which repeats more than 3 times, and change
   * two progressive frames to one interlace frame
   */
  VO_WBC_MODE_PROG_TO_INTL,
  VO_WBC_MODE_BUTT,
} VO_WBC_MODE_E;

typedef enum rkVO_WBC_SOURCE_TYPE_E {
  VO_WBC_SOURCE_DEV = 0x0,   /* WBC source is device */
  VO_WBC_SOURCE_VIDEO = 0x1, /* WBC source is video layer */
  VO_WBC_SOURCE_GRAPHIC =
      0x2, /* WBC source is graphic layer. Warning: not supported */
  VO_WBC_SOURCE_BUTT
} VO_WBC_SOURCE_TYPE_E;

typedef struct rkVO_WBC_SOURCE_S {
  VO_WBC_SOURCE_TYPE_E enSourceType; /* RW; WBC source's type */
  RK_U32 u32SourceId;                /* RW; WBC source's ID */
} VO_WBC_SOURCE_S;

typedef enum rkVO_PART_MODE_E {
  VO_PART_MODE_SINGLE = 0, /* single partition, which use software to make
                              multi-picture in one hardware cell */
  VO_PART_MODE_MULTI =
      1, /* muliti partition, each partition is a hardware cell */
  VO_PART_MODE_BUTT
} VO_PART_MODE_E;

typedef enum rkVO_SPLICE_MODE_E {
  VO_SPLICE_MODE_GPU = 0,
  VO_SPLICE_MODE_RGA
} VO_SPLICE_MODE_E;

typedef enum rkVO_LAYER_MODE_E {
  VO_LAYER_MODE_CURSOR = 0,
  VO_LAYER_MODE_GRAPHIC = 1,
  VO_LAYER_MODE_VIDEO = 2,
  VO_LAYER_MODE_BUTT
} VO_LAYER_MODE_E;

typedef struct rkVO_VIDEO_LAYER_ATTR_S {
  RECT_S stDispRect;              /* RW; Display resolution */
  SIZE_S stImageSize;             /* RW; Canvas size of the video layer */
  RK_U32 u32DispFrmRt;            /* RW; Display frame rate */
  PIXEL_FORMAT_E enPixFormat;     /* RW; Pixel format of the video layer */
  RK_BOOL bDoubleFrame;           /* RW; Whether to double frames */
  COMPRESS_MODE_E enCompressMode; /* RW; Video Layer output compress mode */
  DYNAMIC_RANGE_E
  enDstDynamicRange; /* RW; Video Layer output dynamic range type */
} VO_VIDEO_LAYER_ATTR_S;

typedef struct rkVO_LAYER_PARAM_S {
  ASPECT_RATIO_S stAspectRatio; /* RW; aspect ratio */
} VO_LAYER_PARAM_S;

typedef struct rkVO_ZOOM_RATIO_S {
  /*
   * RW; Range: [0, 1000]; XRatio = x * 1000 / W,
   * x means the start point to be zoomed,
   * W means displaying channel's width.
   */
  RK_U32 u32XRatio;
  /*
   * RW; Range: [0, 1000]; YRatio = y * 1000 / H,
   * y means the start point to be zoomed,
   * H means displaying channel's height.
   */
  RK_U32 u32YRatio;
  /*
   * RW; Range: [0, 1000]; WRatio = w * 1000 / W,
   * w means the width to be zoomed,
   * W means displaying channel's width.
   */
  RK_U32 u32WRatio;
  /*
   * RW; Range: [0, 1000]; HRatio = h * 1000 / H,
   * h means the height to be zoomed,
   * H means displaying channel's height.
   */
  RK_U32 u32HRatio;
} VO_ZOOM_RATIO_S;

typedef struct rkVO_ZOOM_ATTR_S {
  VO_ZOOM_IN_E enZoomType; /* choose the type of zoom in */
  union {
    RECT_S stZoomRect;           /* zoom in by rect */
    VO_ZOOM_RATIO_S stZoomRatio; /* zoom in by ratio */
  };
} VO_ZOOM_ATTR_S;

typedef struct rkVO_CSC_S {
  VO_CSC_MATRIX_E enCscMatrix; /* CSC matrix */
  RK_U32 u32Luma;      /* RW; Range:    [0, 100]; luminance, default: 50 */
  RK_U32 u32Contrast;  /* RW; Range:    [0, 100]; contrast, default: 50 */
  RK_U32 u32Hue;       /* RW; Range:    [0, 100]; hue, default: 50 */
  RK_U32 u32Satuature; /* RW; Range:    [0, 100]; satuature, default: 50 */
} VO_CSC_S;

typedef struct rkVO_REGION_INFO_S {
  RK_U32 u32RegionNum;         /* count of the region */
  RECT_S *ATTRIBUTE pstRegion; /* region attribute */
} VO_REGION_INFO_S;

typedef struct rkVO_LAYER_BOUNDARY_S {
  RK_U32 u32Width;
  RK_U32 u32Color[2];
} VO_LAYER_BOUNDARY_S;

typedef struct rkVO_CHN_BOUNDARY_S {
  RK_BOOL bBoundaryEn;  /* do Frame or not */
  RK_U32 u32ColorIndex; /* the index of Frame color,[0,1] */
} VO_CHN_BOUNDARY_S;

typedef struct rkVO_MOD_PARAM_S {
  /*
   * RW, Range: [0, 1];  YC(Luminance and Chrominance)
   * changes or not when passing through VO
   */
  RK_BOOL bTransparentTransmit;
  RK_BOOL bExitDev;
  RK_BOOL bWbcBgBlackEn;
  RK_BOOL bDevClkExtEn;
  RK_BOOL bSaveBufMode[VO_MAX_PHY_DEV_NUM]; /* save buff mode */
} VO_MOD_PARAM_S;

typedef enum rkVO_CLK_SOURCE_E {
  VO_CLK_SOURCE_PLL,
  VO_CLK_SOURCE_LCDMCLK,

  VO_CLK_SOURCE_BUTT
} VO_CLK_SOURCE_E;

typedef struct rkVO_USER_INTFSYNC_PLL_S {
  RK_U32 u32Fbdiv;
  RK_U32 u32Frac;
  RK_U32 u32Refdiv;
  RK_U32 u32Postdiv1;
  RK_U32 u32Postdiv2;
} VO_USER_INTFSYNC_PLL_S;

typedef struct rkVO_USER_INTFSYNC_ATTR_S {
  VO_CLK_SOURCE_E enClkSource;

  union {
    VO_USER_INTFSYNC_PLL_S stUserSyncPll;
    RK_U32 u32LcdMClkDiv;
  };
} VO_USER_INTFSYNC_ATTR_S;

typedef struct rkVO_USER_INTFSYNC_INFO_S {
  VO_USER_INTFSYNC_ATTR_S stUserIntfSyncAttr;
  RK_U32 u32PreDiv;
  RK_U32 u32DevDiv;
  RK_BOOL bClkReverse;
} VO_USER_INTFSYNC_INFO_S;

typedef struct rkVO_GFX_FRAME_INFO_S {
  RK_U32 u32Width;
  RK_U32 u32Height;
  RK_U32 u32VirWidth;
  RK_U32 u32VirHeight;
  PIXEL_FORMAT_E enPixelFormat;
  RK_U32 u32FgAlpha;
  RK_U32 u32BgAlpha;
  RK_VOID *pData;
  RK_U32 u32Size;
} VO_FRAME_INFO_S;

typedef struct rk_VO_EDID_S {
  RK_BOOL bEdidValid;
  RK_U32 u32Edidlength;
  RK_U8 u8Edid[512];
} VO_EDID_S;

typedef struct rk_VO_SINK_CAPABILITY_S {
  RK_BOOL bConnected;
  RK_BOOL bSupportYCbCr;
  RK_BOOL bSupportHDMI;
} VO_SINK_CAPABILITY_S;

/** hpd event handling callback function */
typedef void (*RK_VO_CallBack)(RK_VOID *pPrivateData);

typedef struct rk_VO_CALLBACK_FUNC_S {
  RK_VO_CallBack pfnEventCallback;
  RK_VOID *pPrivateData;
} RK_VO_CALLBACK_FUNC_S;

typedef enum rkVO_GFX_MODE_E {
  VO_MODE_NORMAL,
  VO_MODE_GFX_PRE_CREATED
} VO_GFX_MODE_E;

typedef enum rkVO_HDMI_MODE_E {
  VO_HDMI_MODE_AUTO = 0, /* According EDID */
  VO_HDMI_MODE_HDMI,
  VO_HDMI_MODE_DVI
} VO_HDMI_MODE_E;

typedef enum rkVO_HDMI_COLOR_FMT_E {
  VO_HDMI_COLOR_FORMAT_RGB = 0,
  VO_HDMI_COLOR_FORMAT_YCBCR444,
  VO_HDMI_COLOR_FORMAT_YCBCR422,
  VO_HDMI_COLOR_FORMAT_YCBCR420,
  VO_HDMI_COLOR_FORMT_AUTO,
  VO_HDMI_COLOR_FORMAT_BUTT
} VO_HDMI_COLOR_FMT_E;

typedef enum rkVO_HDMI_QUANT_RANGE_E {
  VO_HDMI_QUANT_RANGE_AUTO = 0, /* Limited: CEA Mode; Full: Non-CEA Mode */
  VO_HDMI_QUANT_RANGE_LIMITED,
  VO_HDMI_QUANT_RANGE_FULL,
  VO_HDMI_QUANT_RANGE_BUTT
} VO_HDMI_QUANT_RANGE_E;

typedef struct rk_VO_HDMI_PARAM_S {
  VO_HDMI_MODE_E enHdmiMode;
  VO_HDMI_COLOR_FMT_E enColorFmt;
  VO_HDMI_QUANT_RANGE_E enQuantRange; /* Effective in enColorFmt == RGB mode */
} VO_HDMI_PARAM_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef INCLUDE_RT_MPI_RK_COMMON_VO_H_ */
