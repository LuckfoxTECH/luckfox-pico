/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_RK_COMMON_AVS_H_
#define INCLUDE_RT_MPI_RK_COMMON_AVS_H_

#include "rk_comm_video.h"
#include "rk_common.h"
#include "rk_errno.h"
#include "rk_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define AVS_MAX_GRP_NUM 32
#define AVS_PIPE_NUM 6
#define AVS_MAX_CHN_NUM 2
#define AVS_SPLIT_NUM 2
#define AVS_SPLIT_PIPE_NUM 6
#define AVS_CUBE_MAP_SURFACE_NUM 6

#define AVS_MAX_IN_WIDTH 8192
#define AVS_MAX_IN_HEIGHT 8192
#define AVS_MIN_IN_WIDTH 1280
#define AVS_MIN_IN_HEIGHT 720

#define AVS_MAX_OUT_WIDTH 10000
#define AVS_MAX_OUT_HEIGHT 10000
#define AVS_MIN_OUT_WIDTH 256
#define AVS_MIN_OUT_HEIGHT 256
#define MAX_AVS_FILE_PATH_LEN 256

typedef enum rkAVS_LUT_ACCURAY_E {
  AVS_LUT_ACCURACY_HIGH = 0, /* LUT high accuracy. */
  AVS_LUT_ACCURACY_LOW = 1,  /* LUT low accuracy. */

  AVS_LUT_ACCURACY_BUTT
} AVS_LUT_ACCURAY_E;

typedef struct rkAVS_LUT_S {
  AVS_LUT_ACCURAY_E enAccuracy;
  RK_CHAR aFilePath[MAX_AVS_FILE_PATH_LEN];
} AVS_LUT_S;

typedef enum rkAVS_PROJECTION_MODE_E {
  AVS_PROJECTION_EQUIRECTANGULAR = 0, /* Equirectangular mode. */
  AVS_PROJECTION_RECTILINEAR = 1,     /* Rectilinear mode. */
  AVS_PROJECTION_CYLINDRICAL = 2,     /* Cylindrical mode. */
  AVS_PROJECTION_CUBE_MAP = 3,        /* Cube map mode. */

  AVS_PROJECTION_BUTT
} AVS_PROJECTION_MODE_E;

typedef enum rkAVS_GAIN_MODE_E {
  AVS_GAIN_MODE_MANUAL = 0,
  AVS_GAIN_MODE_AUTO = 1,

  AVS_GAIN_MODE_BUTT
} AVS_GAIN_MODE_E;

typedef enum rkAVS_MODE_E {
  AVS_MODE_BLEND =
      0, /* according to LUT stitching, blend at the splicing point */
  AVS_MODE_NOBLEND_VER = 1, /* place input images vertically together, no blend
                               at the stitching point. */
  AVS_MODE_NOBLEND_HOR = 2, /* place input images horizontally together, no
                               blend at the stitching point */

  /* Only 4 image stitching is supported,
   * two rows are placed, two rows are placed together,
   * no blend at the stitching point. */
  AVS_MODE_NOBLEND_QR = 3,

  AVS_MODE_BUTT
} AVS_MODE_E;

typedef struct rkAVS_GAIN_ATTR_S {
  AVS_GAIN_MODE_E enMode;
  RK_S32 s32Coef[AVS_PIPE_NUM];
} AVS_GAIN_ATTR_S;

typedef struct rkAVS_ROTATION_S {
  RK_S32 s32Yaw;
  RK_S32 s32Pitch;
  RK_S32 s32Roll;
} AVS_ROTATION_S;

typedef struct rkAVS_FOV_S {
  RK_U32 u32FOVX;
  RK_U32 u32FOVY;
} AVS_FOV_S;

typedef struct rkAVS_SPLIT_ATTR_S {
  RK_U32 u32PipeNum;
  AVS_PIPE AVSPipe[AVS_SPLIT_PIPE_NUM];
} AVS_SPLIT_ATTR_S;

typedef struct rkAVS_CUBE_MAP_ATTR_S {
  RK_BOOL bBgColor;
  RK_U32 u32BgColor;
  RK_U32 u32SurfaceLength;
  POINT_S stStartPoint[AVS_CUBE_MAP_SURFACE_NUM]; /* RW; Start point of each
                                                     surface. */
} AVS_CUBE_MAP_ATTR_S;

typedef struct rkAVS_OUTPUT_ATTR_S {
  AVS_PROJECTION_MODE_E enPrjMode;             /* RW; Projection mode. */
  POINT_S stCenter;                            /* Center point. */
  AVS_FOV_S stFOV;                             /* Output FOV. */
  AVS_ROTATION_S stORIRotation;                /* Output original rotation. */
  AVS_ROTATION_S stRotation;                   /* Output rotation. */
  AVS_SPLIT_ATTR_S stSplitAttr[AVS_SPLIT_NUM]; /* Split attribute for 7 or 8
                                                  inputs stitching. */
  AVS_CUBE_MAP_ATTR_S stCubeMapAttr;           /* Cube map attribute. */
} AVS_OUTPUT_ATTR_S;

typedef struct rkAVS_GRP_ATTR_S {
  AVS_MODE_E enMode;          /* Group work mode */
  RK_U32 u32PipeNum;          /* RW; Pipe number. */
  RK_BOOL bSyncPipe;          /* RW; Whether sync pipe image. */
  AVS_LUT_S stLUT;            /* Look up table. */
  AVS_GAIN_ATTR_S stGainAttr; /* Gain attribute. */
  RK_U64
  u64BBoxPhyAddr[AVS_PIPE_NUM];  /* Physical address of bounding box data. */
  AVS_OUTPUT_ATTR_S stOutAttr;   /* Output attribute. */
  FRAME_RATE_CTRL_S stFrameRate; /* Frame rate control info. */
} AVS_GRP_ATTR_S;

typedef struct rkAVS_CHN_ATTR_S {
  RK_U32 u32Width;                /* RW; Width of target image. */
  RK_U32 u32Height;               /* RW; Height of target image. */
  COMPRESS_MODE_E enCompressMode; /* RW; Compression mode of the output. */
  DYNAMIC_RANGE_E enDynamicRange; /* RW; Dynamic range. */
  RK_U32 u32Depth;                /* RW; Chn user list depth. */
  FRAME_RATE_CTRL_S stFrameRate;  /* Frame rate control info. */
} AVS_CHN_ATTR_S;

typedef struct rkAVS_MOD_PARAM_S {
  RK_U32 u32WorkingSetSize; /* RW; AVS work */
} AVS_MOD_PARAM_S;

#define RK_AVS_OK RK_SUCCESS
#define RK_ERR_AVS_NULL_PTR                                                    \
  RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_NULL_PTR)
#define RK_ERR_AVS_NOTREADY                                                    \
  RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_NOTREADY)
#define RK_ERR_AVS_INVALID_DEVID                                               \
  RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_INVALID_DEVID)
#define RK_ERR_AVS_INVALID_CHNID                                               \
  RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_INVALID_CHNID)
#define RK_ERR_AVS_INVALID_PIPEID                                              \
  RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_INVALID_PIPEID)
#define RK_ERR_AVS_EXIST RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_EXIST)
#define RK_ERR_AVS_UNEXIST                                                     \
  RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_UNEXIST)
#define RK_ERR_AVS_NOT_SUPPORT                                                 \
  RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_SUPPORT)
#define RK_ERR_AVS_NOT_PERM                                                    \
  RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_PERM)
#define RK_ERR_AVS_NOMEM RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_NOMEM)
#define RK_ERR_AVS_NOBUF RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_NOBUF)
#define RK_ERR_AVS_ILLEGAL_PARAM                                               \
  RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_ILLEGAL_PARAM)
#define RK_ERR_AVS_BUF_EMPTY                                                   \
  RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_BUF_EMPTY)
#define RK_ERR_AVS_BUF_FULL                                                    \
  RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_BUF_FULL)
#define RK_ERR_AVS_BUSY RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_BUSY)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* INCLUDE_RT_MPI_RK_COMMON_AVS_H_ */
