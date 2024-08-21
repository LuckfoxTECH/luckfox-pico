/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2023 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_RK_COMMON_AVS_H_
#define INCLUDE_RT_MPI_RK_COMMON_AVS_H_

#include "rk_common.h"
#include "rk_errno.h"
#include "rk_comm_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define AVS_SPLIT_NUM            2
#define AVS_SPLIT_PIPE_NUM       6
#define AVS_CUBE_MAP_SURFACE_NUM 6

typedef enum rkAVS_LUT_ACCURACY_E {
    AVS_LUT_ACCURACY_HIGH    = 0,    /* LUT high accuracy. */
    AVS_LUT_ACCURACY_LOW     = 1,    /* LUT low accuracy. */

    AVS_LUT_ACCURACY_BUTT
} AVS_LUT_ACCURACY_E;

typedef enum rkAVS_LUT_STEP_E {
    AVS_LUT_STEP_HIGH    = 0,    /* LUT step size 16  pixel */
    AVS_LUT_STEP_MEDIUM  = 1,    /* LUT step size 32 pixel */
    AVS_LUT_STEP_LOW     = 2,    /* LUT step size 64 pixel */

    AVS_LUT_STEP_BUTT
} AVS_LUT_STEP_E;

typedef enum rkAVS_FUSE_WIDTH_E {
    AVS_FUSE_WIDTH_HIGH      = 0,    /* Fusion zone size 128 pixel */
    AVS_FUSE_WIDTH_MEDIUM    = 1,    /* Fusion zone size 256 pixel */
    AVS_FUSE_WIDTH_LOW       = 2,    /* Fusion zone size 512 pixel */

    AVS_FUSE_WIDTH_BUTT
} AVS_FUSE_WIDTH_E;

typedef enum rkAVS_PROJECTION_MODE_E {
    AVS_PROJECTION_EQUIRECTANGULAR       = 0,  /* Equirectangular mode. */
    AVS_PROJECTION_RECTILINEAR           = 1,  /* Rectilinear mode. */
    AVS_PROJECTION_CYLINDRICAL           = 2,  /* Cylindrical mode. */
    AVS_PROJECTION_CUBE_MAP              = 3,  /* Cube map mode. */
    AVS_PROJECTION_EQUIRECTANGULAR_TRANS = 4,  /* Transvers Equirectangular  mode. */

    AVS_PROJECTION_BUTT
} AVS_PROJECTION_MODE_E;

typedef enum rkAVS_GAIN_MODE_E {
    AVS_GAIN_MODE_MANUAL = 0,
    AVS_GAIN_MODE_AUTO   = 1,

    AVS_GAIN_MODE_BUTT
} AVS_GAIN_MODE_E;

typedef enum rkAVS_MODE_E {
    AVS_MODE_BLEND        = 0, /* according to LUT stitching, blend at the splicing point */
    AVS_MODE_NOBLEND_VER  = 1, /* place input images vertically together, no blend at the stitching point. */
    AVS_MODE_NOBLEND_HOR  = 2, /* place input images horizontally together, no blend at the stitching point */

    /* Only 4 image stitching is supported,
    *   two rows are placed, two rows are placed together,
    *   no blend at the stitching point.
    *  The input image arrangement position of each pipe
    *   is as follows.
    * +-----------+-----------+
    * |   pipe0   |   pipe1   |
    * +-----------+-----------+
    * |   pipe2   |   pipe3   |
    * +-----------+-----------+
    * */
    AVS_MODE_NOBLEND_QR   = 3,
    AVS_MODE_NOBLEND_OVL  = 4,

    AVS_MODE_BUTT
} AVS_MODE_E;

typedef enum rkAVS_PARAM_SOURCE_E {
    AVS_PARAM_SOURCE_LUT   = 0,     /* Look up table*/
    AVS_PARAM_SOURCE_CALIB = 1,     /* Calibration file */

    AVS_PARAM_SOURCE_MODE_BUT
} AVS_PARAM_SOURCE_E;

typedef struct rkAVS_GAIN_ATTR_S {
    AVS_GAIN_MODE_E enMode;
    RK_S32          s32Coef[AVS_PIPE_NUM];
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

typedef struct rkAVS_LUT_STEP_S {
    AVS_LUT_STEP_E    enStepX;
    AVS_LUT_STEP_E    enStepY;
} AVS_STEP_ATTR_S;

typedef struct rkAVS_SPLIT_ATTR_S {
    RK_U32   u32PipeNum;
    AVS_PIPE AVSPipe[AVS_SPLIT_PIPE_NUM];
} AVS_SPLIT_ATTR_S;

typedef struct rkAVS_CUBE_MAP_ATTR_S {
    RK_BOOL   bBgColor;
    RK_U32    u32BgColor;
    RK_U32    u32SurfaceLength;
    POINT_S   stStartPoint[AVS_CUBE_MAP_SURFACE_NUM];  /* RW; Start point of each surface. */
} AVS_CUBE_MAP_ATTR_S;

typedef struct rkAVS_LUT_S {
    AVS_LUT_ACCURACY_E enAccuracy;
    AVS_FUSE_WIDTH_E   enFuseWidth;
    AVS_STEP_ATTR_S    stLutStep;
    RK_VOID           *pVirAddr[AVS_PIPE_NUM];
} AVS_LUT_S;

typedef struct rkAVS_CALIB_S {
    const RK_CHAR *pCalibFilePath;
    const RK_CHAR *pMeshAlphaPath;
} AVS_CALIB_S;

typedef struct rkAVS_FINAL_LUT_S {
    MB_BLK pMeshBlk[AVS_PIPE_NUM];
    MB_BLK pAlphaBlk[AVS_PIPE_NUM];
    MB_BLK pLdchBlk[AVS_PIPE_NUM];
    MB_BLK pParamBlk[AVS_PIPE_NUM];
} AVS_FINAL_LUT_S;

typedef struct rkAVS_INPUT_ATTR_S {
    AVS_PARAM_SOURCE_E  enParamSource;         /* RW; Input param source. */
    union {
        AVS_LUT_S       stLUT;                 /* Look up table. */
        AVS_CALIB_S     stCalib;               /* Calibration file. */
    };
    SIZE_S              stSize;                /* Source resolution */
} AVS_INPUT_ATTR_S;

typedef struct rkAVS_OVERLAY_ATTR_S {
    RK_BOOL   bBgColor;
    RK_U32    u32BgColor;
} AVS_OVERLAY_ATTR_S;

typedef struct rkAVS_OUTPUT_ATTR_S {
    AVS_PROJECTION_MODE_E    enPrjMode;                  /* RW; Projection mode. */
    POINT_S                  stCenter;                   /* Center point. */
    AVS_FOV_S                stFOV;                      /* Output FOV. */
    AVS_ROTATION_S           stORIRotation;              /* Output original rotation. */
    AVS_ROTATION_S           stRotation;                 /* Output rotation. */
    AVS_SPLIT_ATTR_S         stSplitAttr[AVS_SPLIT_NUM]; /* Split attribute for 7 or 8 inputs stitching. */
    AVS_CUBE_MAP_ATTR_S      stCubeMapAttr;              /* Cube map attribute. */
    SIZE_S                   stSize;                     /* Mesh resolution in AVS_MODE_BLEND mode;
                                                          * Canvas resolution in AVS_MODE_NOBLEND_OVERLAY mode. */
    RK_FLOAT                 fDistance;                  /* Optimum stitch distance. */
    AVS_OVERLAY_ATTR_S       stOverlayAttr;              /* Overlay attribute. */
} AVS_OUTPUT_ATTR_S;

typedef struct rkAVS_GRP_ATTR_S {
    AVS_MODE_E           enMode;                        /* Group work mode */
    RK_U32               u32PipeNum;                    /* RW; Pipe number. */
    RK_BOOL              bSyncPipe;                     /* RW; Whether sync pipe image. */
    AVS_INPUT_ATTR_S     stInAttr;                      /* Input attribute */
    AVS_GAIN_ATTR_S      stGainAttr;                    /* Gain attribute. */
    RK_U64               u64BBoxPhyAddr[AVS_PIPE_NUM];  /* Physical address of bounding box data. */
    AVS_OUTPUT_ATTR_S    stOutAttr;                     /* Output attribute. */
    FRAME_RATE_CTRL_S    stFrameRate;                   /* Frame rate control info. */
} AVS_GRP_ATTR_S;

typedef struct rkAVS_CHN_ATTR_S {
    RK_U32              u32Width;           /* RW; Width of target image. */
    RK_U32              u32Height;          /* RW; Height of target image. */
    COMPRESS_MODE_E     enCompressMode;     /* RW; Compression mode of the output. */
    DYNAMIC_RANGE_E     enDynamicRange;     /* RW; Dynamic range. */
    RK_U32              u32Depth;           /* RW; Chn user list depth. */
    FRAME_RATE_CTRL_S   stFrameRate;        /* Frame rate control info. */
    RK_U32              u32FrameBufCnt;     /* RW; frame buffer cnt only used by MB_SOURCE_PRIVATE */
} AVS_CHN_ATTR_S;

typedef struct rkAVS_PIPE_ATTR_S {
    RECT_S      stSrcRect;
    RECT_S      stDstRect;
    RK_U32      u32Priority;        /* RW; Overlay priority*/
} AVS_PIPE_ATTR_S;

typedef struct rkAVS_MOD_PARAM_S {
    RK_U32 u32WorkingSetSize;               /* RW; AVS work */
    MB_SOURCE_E enMBSource;                 /* RW; AVS MB pool source type */
} AVS_MOD_PARAM_S;

#define RK_AVS_OK                   RK_SUCCESS
#define RK_ERR_AVS_NULL_PTR         RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_NULL_PTR)
#define RK_ERR_AVS_NOTREADY         RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_NOTREADY)
#define RK_ERR_AVS_INVALID_DEVID    RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_INVALID_DEVID)
#define RK_ERR_AVS_INVALID_CHNID    RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_INVALID_CHNID)
#define RK_ERR_AVS_INVALID_PIPEID   RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_INVALID_PIPEID)
#define RK_ERR_AVS_EXIST            RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_EXIST)
#define RK_ERR_AVS_UNEXIST          RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_UNEXIST)
#define RK_ERR_AVS_NOT_SUPPORT      RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_SUPPORT)
#define RK_ERR_AVS_NOT_PERM         RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_PERM)
#define RK_ERR_AVS_NOMEM            RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_NOMEM)
#define RK_ERR_AVS_NOBUF            RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_NOBUF)
#define RK_ERR_AVS_ILLEGAL_PARAM    RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_ILLEGAL_PARAM)
#define RK_ERR_AVS_BUF_EMPTY        RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_BUF_EMPTY)
#define RK_ERR_AVS_BUF_FULL         RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_BUF_FULL)
#define RK_ERR_AVS_BUSY             RK_DEF_ERR(RK_ID_AVS, RK_ERR_LEVEL_ERROR, RK_ERR_BUSY)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* INCLUDE_RT_MPI_RK_COMMON_AVS_H_ */
