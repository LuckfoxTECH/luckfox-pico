/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_RK_COMM_VGS_H_
#define INCLUDE_RT_MPI_RK_COMM_VGS_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#include "rk_comm_video.h"
#include "rk_common.h"
#include "rk_errno.h"
#include "rk_type.h"

/* failure caused by malloc buffer */
#define RK_VGS_SUCCESS RK_SUCCESS
#define RK_ERR_VGS_NOBUF RK_DEF_ERR(RK_ID_VGS, RK_ERR_LEVEL_ERROR, RK_ERR_NOBUF)
#define RK_ERR_VGS_BUF_EMPTY                                                   \
  RK_DEF_ERR(RK_ID_VGS, RK_ERR_LEVEL_ERROR, RK_ERR_BUF_EMPTY)
#define RK_ERR_VGS_NULL_PTR                                                    \
  RK_DEF_ERR(RK_ID_VGS, RK_ERR_LEVEL_ERROR, RK_ERR_NULL_PTR)
#define RK_ERR_VGS_ILLEGAL_PARAM                                               \
  RK_DEF_ERR(RK_ID_VGS, RK_ERR_LEVEL_ERROR, RK_ERR_ILLEGAL_PARAM)
#define RK_ERR_VGS_BUF_FULL                                                    \
  RK_DEF_ERR(RK_ID_VGS, RK_ERR_LEVEL_ERROR, RK_ERR_BUF_FULL)
#define RK_ERR_VGS_SYS_NOTREADY                                                \
  RK_DEF_ERR(RK_ID_VGS, RK_ERR_LEVEL_ERROR, RK_ERR_NOTREADY)
#define RK_ERR_VGS_NOT_SUPPORT                                                 \
  RK_DEF_ERR(RK_ID_VGS, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_SUPPORT)
#define RK_ERR_VGS_NOT_PERMITTED                                               \
  RK_DEF_ERR(RK_ID_VGS, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_PERM)

#define VGS_MAX_ARRAY_SIZE 100

typedef RK_S32 VGS_HANDLE;

typedef enum rkVGS_COLOR_REVERT_MODE_E {
  VGS_COLOR_REVERT_NONE = 0, /* Not revert */
  VGS_COLOR_REVERT_RGB,      /* Revert RGB */
  VGS_COLOR_REVERT_ALPHA,    /* Revert alpha */
  VGS_COLOR_REVERT_BOTH,     /* Revert RGB and alpha*/
  VGS_COLOR_REVERT_BUTT
} VGS_COLOR_REVERT_MODE_E;

typedef struct rkVGS_OSD_REVERT_S {
  RECT_S stSrcRect;                          /* OSD color revert area */
  VGS_COLOR_REVERT_MODE_E enColorRevertMode; /* OSD color revert mode */
} VGS_OSD_REVERT_S;

typedef struct rkVGS_TASK_ATTR_S {
  VIDEO_FRAME_INFO_S stImgIn;  /* Input picture */
  VIDEO_FRAME_INFO_S stImgOut; /* Output picture */
  RK_U64 au64PrivateData[4];   /* RW; Private data of task */
  RK_U32 reserved; /* RW; Debug information,state of current picture */
} VGS_TASK_ATTR_S;

typedef struct rkVGS_DRAW_LINE_S {
  POINT_S stStartPoint; /* Line start point */
  POINT_S stEndPoint;   /* Line end point */
  RK_U32 u32Thick;      /* RW; Width of line */
  RK_U32 u32Color;      /* RW; Range: [0,0xFFFFFF]; Color of line */
} VGS_DRAW_LINE_S;

typedef enum rkVGS_COVER_TYPE_E {
  COVER_RECT = 0,    /* Retangle cover */
  COVER_QUAD_RANGLE, /* Quadrangle cover */
  COVER_BUTT
} VGS_COVER_TYPE_E;

typedef struct rkVGS_QUADRANGLE_COVER_S {
  RK_BOOL bSolid;     /* Solid or hollow cover */
  RK_U32 u32Thick;    /* RW; Range: [2,8]; Thick of the hollow quadrangle */
  POINT_S stPoint[4]; /* Four points of the quadrangle */
} VGS_QUADRANGLE_COVER_S;

typedef struct rkVGS_ADD_COVER_S {
  VGS_COVER_TYPE_E enCoverType; /* Cover type */
  union {
    RECT_S stDstRect;                    /* The rectangle area */
    VGS_QUADRANGLE_COVER_S stQuadRangle; /* The quadrangle area */
  };

  RK_U32 u32Color; /* RW; Range: [0,0xFFFFFF]; Color of cover */
} VGS_ADD_COVER_S;

typedef struct rkVGS_ADD_OSD_S {
  MB_BLK pMbBlk;
  RECT_S stRect;             /* Osd area */
  PIXEL_FORMAT_E enPixelFmt; /* Pixel format of osd */
  RK_U32 u32FgAlpha;         /* RW; Range: [0,255]; Foreground alpha of osd */
  RK_U32 u32BgAlpha;         /* RW; Range: [0,255]; Background alpha of osd */
} VGS_ADD_OSD_S;

typedef enum rkVGS_SCLCOEF_MODE_E {
  VGS_SCLCOEF_NORMAL = 0, /* normal scale coefficient*/
  VGS_SCLCOEF_TAP2 = 1,   /* scale coefficient of 2 tap */
  VGS_SCLCOEF_TAP4 = 2,   /* scale coefficient of 4 tap */
  VGS_SCLCOEF_TAP6 = 3,   /* scale coefficient of 6 tap */
  VGS_SCLCOEF_TAP8 = 4,   /* scale coefficient of 8 tap */
  VGS_SCLCOEF_BUTT
} VGS_SCLCOEF_MODE_E;

typedef enum rkVGS_MOSAIC_BLK_SIZE_E {
  RK_MOSAIC_BLK_SIZE_8 = 8,   /* block size 8*8 of MOSAIC */
  RK_MOSAIC_BLK_SIZE_16 = 16, /* block size 16*16 of MOSAIC */
  RK_MOSAIC_BLK_SIZE_32 = 32, /* block size 32*32 of MOSAIC */
  RK_MOSAIC_BLK_SIZE_64 = 64, /* block size 64*64 of MOSAIC */
  RK_MOSAIC_BLK_SIZE_BUT
} VGS_MOSAIC_BLK_SIZE_E;

typedef struct rkVGS_MOSAIC_S {
  VGS_MOSAIC_BLK_SIZE_E enBlkSize;
  RECT_S stDstRect; /* RW; The rectangle area. */
} VGS_MOSAIC_S;

typedef enum rkVGS_CROP_COORDINATE_E {
  VGS_CROP_RATIO_COOR = 0, /* Ratio coordinate. */
  VGS_CROP_ABS_COOR        /* Absolute coordinate. */
} VGS_CROP_COORDINATE_E;

typedef struct rkVGS_CROP_INFO_S {
  VGS_CROP_COORDINATE_E enCropCoordinate; /* RW; Range: [0, 1]; Coordinate mode
                                             of the crop start point. */
  RECT_S stCropRect;                      /* CROP rectangular. */
} VGS_CROP_INFO_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __RK_COMM_VGS_H__ */
