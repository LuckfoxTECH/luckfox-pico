/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_RK_COMM_VENC_H_

#define INCLUDE_RT_MPI_RK_COMM_VENC_H_
#include "rk_comm_rc.h"
#include "rk_comm_video.h"
#include "rk_common.h"
#include "rk_errno.h"
#include "rk_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* invlalid channel ID */
#define RK_ERR_VENC_INVALID_CHNID                                              \
  RK_DEF_ERR(RK_ID_VENC, RK_ERR_LEVEL_ERROR, RK_ERR_INVALID_CHNID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define RK_ERR_VENC_ILLEGAL_PARAM                                              \
  RK_DEF_ERR(RK_ID_VENC, RK_ERR_LEVEL_ERROR, RK_ERR_ILLEGAL_PARAM)
/* channel exists */
#define RK_ERR_VENC_EXIST                                                      \
  RK_DEF_ERR(RK_ID_VENC, RK_ERR_LEVEL_ERROR, RK_ERR_EXIST)
/* channel exists */
#define RK_ERR_VENC_UNEXIST                                                    \
  RK_DEF_ERR(RK_ID_VENC, RK_ERR_LEVEL_ERROR, RK_ERR_UNEXIST)
/* using a NULL point */
#define RK_ERR_VENC_NULL_PTR                                                   \
  RK_DEF_ERR(RK_ID_VENC, RK_ERR_LEVEL_ERROR, RK_ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing
 * attribute */
#define RK_ERR_VENC_NOT_CONFIG                                                 \
  RK_DEF_ERR(RK_ID_VENC, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define RK_ERR_VENC_NOT_SUPPORT                                                \
  RK_DEF_ERR(RK_ID_VENC, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change stati attribute */
#define RK_ERR_VENC_NOT_PERM                                                   \
  RK_DEF_ERR(RK_ID_VENC, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_PERM)
/* failure caused by malloc memory */
#define RK_ERR_VENC_NOMEM                                                      \
  RK_DEF_ERR(RK_ID_VENC, RK_ERR_LEVEL_ERROR, RK_ERR_NOMEM)
/* failure caused by malloc buffer */
#define RK_ERR_VENC_NOBUF                                                      \
  RK_DEF_ERR(RK_ID_VENC, RK_ERR_LEVEL_ERROR, RK_ERR_NOBUF)
/* no data in buffer */
#define RK_ERR_VENC_BUF_EMPTY                                                  \
  RK_DEF_ERR(RK_ID_VENC, RK_ERR_LEVEL_ERROR, RK_ERR_BUF_EMPTY)
/* no buffer for new data */
#define RK_ERR_VENC_BUF_FULL                                                   \
  RK_DEF_ERR(RK_ID_VENC, RK_ERR_LEVEL_ERROR, RK_ERR_BUF_FULL)
/* system is not ready,had not initialed or loaded */
#define RK_ERR_VENC_SYS_NOTREADY                                               \
  RK_DEF_ERR(RK_ID_VENC, RK_ERR_LEVEL_ERROR, RK_ERR_NOTREADY)
/* system is busy */
#define RK_ERR_VENC_BUSY RK_DEF_ERR(RK_ID_VENC, RK_ERR_LEVEL_ERROR, RK_ERR_BUSY)

#define VENC_QP_SGRM_NUM 52
#define MAX_TILE_NUM 1
#define RC_TEXTURE_THR_SIZE 16

/* the nalu type of H264E */
typedef enum rkH264E_NALU_TYPE_E {
  H264E_NALU_BSLICE = 0,   /*B SLICE types*/
  H264E_NALU_PSLICE = 1,   /*P SLICE types*/
  H264E_NALU_ISLICE = 2,   /*I SLICE types*/
  H264E_NALU_IDRSLICE = 5, /*IDR SLICE types*/
  H264E_NALU_SEI = 6,      /*SEI types*/
  H264E_NALU_SPS = 7,      /*SPS types*/
  H264E_NALU_PPS = 8,      /*PPS types*/
  H264E_NALU_BUTT
} H264E_NALU_TYPE_E;

/* the nalu type of H265E */
typedef enum rkH265E_NALU_TYPE_E {
  H265E_NALU_BSLICE = 0,    /*B SLICE types*/
  H265E_NALU_PSLICE = 1,    /*P SLICE types*/
  H265E_NALU_ISLICE = 2,    /*I SLICE types*/
  H265E_NALU_IDRSLICE = 19, /*IDR SLICE types*/
  H265E_NALU_VPS = 32,      /*VPS types*/
  H265E_NALU_SPS = 33,      /*SPS types*/
  H265E_NALU_PPS = 34,      /*PPS types*/
  H265E_NALU_SEI = 39,      /*SEI types*/

  H265E_NALU_BUTT
} H265E_NALU_TYPE_E;

/* the reference type of H264E slice */
typedef enum rkH264E_REFSLICE_TYPE_E {
  H264E_REFSLICE_FOR_1X = 1, /*Reference slice for H264E_REF_MODE_1X*/
  H264E_REFSLICE_FOR_2X = 2, /*Reference slice for H264E_REF_MODE_2X*/
  H264E_REFSLICE_FOR_4X = 5, /*Reference slice for H264E_REF_MODE_4X*/
  H264E_REFSLICE_FOR_BUTT    /*slice not for reference*/
} H264E_REFSLICE_TYPE_E;

/* the pack type of JPEGE */
typedef enum rkJPEGE_PACK_TYPE_E {
  JPEGE_PACK_ECS = 5,      /*ECS types*/
  JPEGE_PACK_APP = 6,      /*APP types*/
  JPEGE_PACK_VDO = 7,      /*VDO types*/
  JPEGE_PACK_PIC = 8,      /*PIC types*/
  JPEGE_PACK_DCF = 9,      /*DCF types*/
  JPEGE_PACK_DCF_PIC = 10, /*DCF PIC types*/
  JPEGE_PACK_BUTT
} JPEGE_PACK_TYPE_E;

typedef enum rkH264E_PROFILE_E {
  H264E_PROFILE_BASELINE = 66,
  H264E_PROFILE_MAIN = 77,
  H264E_PROFILE_HIGH = 100,
  H264E_PROFILE__BUTT
} H264E_PROFILE_E;

typedef enum rkH265E_PROFILE_E {
  H265E_PROFILE_MAIN = 0,
  H265E_PROFILE_MAIN10 = 1,
  H265E_PROFILE__BUTT
} H265E_PROFILE_E;

/* the pack type of PRORES */
typedef enum rkPRORES_PACK_TYPE_E {
  PRORES_PACK_PIC = 1, /*PIC types*/
  PRORES_PACK_BUTT
} PRORES_PACK_TYPE_E;

/* the data type of VENC */
typedef union rkVENC_DATA_TYPE_U {
  H264E_NALU_TYPE_E enH264EType; /* R; H264E NALU types*/
  JPEGE_PACK_TYPE_E enJPEGEType; /* R; JPEGE pack types*/
  H265E_NALU_TYPE_E enH265EType; /* R; H264E NALU types*/
  PRORES_PACK_TYPE_E enPRORESType;
} VENC_DATA_TYPE_U;

/* the pack info of VENC */
typedef struct rkVENC_PACK_INFO_S {
  VENC_DATA_TYPE_U u32PackType; /* R; the pack type*/
  RK_U32 u32PackOffset;
  RK_U32 u32PackLength;
} VENC_PACK_INFO_S;

/* Defines a stream packet */
typedef struct rkVENC_PACK_S {
  MB_BLK pMbBlk; /* R; the virtual address of stream */
  RK_U32 u32Len; /* R; the length of stream */

  RK_U64 u64PTS;      /* R; PTS */
  RK_BOOL bFrameEnd;  /* R; frame end */
  RK_BOOL bStreamEnd; /* R; stream end */

  VENC_DATA_TYPE_U DataType; /* R; the type of stream */
  RK_U32 u32Offset;  /* R; the offset between the Valid data and the start
                        address */
  RK_U32 u32DataNum; /* R; the  stream packets num */
  VENC_PACK_INFO_S stPackInfo[8]; /* R; the stream packet Information */
} VENC_PACK_S;

/* Defines the frame type and reference attributes of the H.264 frame skipping
 * reference streams */
typedef enum rkH264E_REF_TYPE_E {
  BASE_IDRSLICE = 0, /* the Idr frame at Base layer*/
  /* the P frame at Base layer, referenced by other frames at Base layer and
     reference to Idr frame*/
  BASE_PSLICE_REFTOIDR,
  /* the P frame at Base layer, referenced by other frames at Base layer*/
  BASE_PSLICE_REFBYBASE,
  /* the P frame at Base layer, referenced by other frames at Enhance layer*/
  BASE_PSLICE_REFBYENHANCE,
  /* the P frame at Enhance layer, referenced by other frames at Enhance layer*/
  ENHANCE_PSLICE_REFBYENHANCE,
  /* the P frame at Enhance layer ,not referenced*/
  ENHANCE_PSLICE_NOTFORREF,
  ENHANCE_PSLICE_BUTT
} H264E_REF_TYPE_E;

typedef enum rkH264E_REF_TYPE_E H265E_REF_TYPE_E;

/* Defines the features of an H.264 stream */
typedef struct rkVENC_STREAM_INFO_H264_S {
  RK_U32 u32PicBytesNum;     /* R; the coded picture stream byte number */
  RK_U32 u32Inter16x16MbNum; /* R; the inter16x16 macroblock num */
  RK_U32 u32Inter8x8MbNum;   /* R; the inter8x8 macroblock num */
  RK_U32 u32Intra16MbNum;    /* R; the intra16x16 macroblock num */
  RK_U32 u32Intra8MbNum;     /* R; the intra8x8 macroblock num */
  RK_U32 u32Intra4MbNum;     /* R; the inter4x4 macroblock num */
  /* R; Type of encoded frames in advanced frame skipping reference mode*/
  H264E_REF_TYPE_E enRefType;
  /* R; Number of times that channel attributes or parameters (including RC
   * parameters) are set*/
  RK_U32 u32UpdateAttrCnt;
  RK_U32 u32StartQp; /* R; the start Qp of encoded frames*/
  RK_U32 u32MeanQp;  /* R; the mean Qp of encoded frames*/
  RK_BOOL bPSkip;
} VENC_STREAM_INFO_H264_S;

/* Defines the features of an H.265 stream */
typedef struct rkVENC_STREAM_INFO_H265_S {
  RK_U32 u32PicBytesNum;     /* R; the coded picture stream byte number */
  RK_U32 u32Inter64x64CuNum; /* R; the inter64x64 cu num  */
  RK_U32 u32Inter32x32CuNum; /* R; the inter32x32 cu num  */
  RK_U32 u32Inter16x16CuNum; /* R; the inter16x16 cu num  */
  RK_U32 u32Inter8x8CuNum;   /* R; the inter8x8   cu num  */
  RK_U32 u32Intra32x32CuNum; /* R; the Intra32x32 cu num  */
  RK_U32 u32Intra16x16CuNum; /* R; the Intra16x16 cu num  */
  RK_U32 u32Intra8x8CuNum;   /* R; the Intra8x8   cu num  */
  RK_U32 u32Intra4x4CuNum;   /* R; the Intra4x4   cu num  */
  /* R; Type of encoded frames in advanced frame skipping reference mode*/
  H265E_REF_TYPE_E enRefType;
  /* R; Number of times that channel attributes or parameters (including RC
   * parameters) are set*/
  RK_U32 u32UpdateAttrCnt;
  RK_U32 u32StartQp; /* R; the start Qp of encoded frames*/
  RK_U32 u32MeanQp;  /* R; the mean Qp of encoded frames*/
  RK_BOOL bPSkip;
} VENC_STREAM_INFO_H265_S;

/* the sse info */
typedef struct rkVENC_SSE_INFO_S {
  RK_BOOL bSSEEn;   /* RW; Range:[0,1]; Region SSE enable */
  RK_U32 u32SSEVal; /* R; Region SSE value */
} VENC_SSE_INFO_S;

/* the advance information of the h264e */
typedef struct rkVENC_STREAM_ADVANCE_INFO_H264_S {
  RK_U32 u32ResidualBitNum;             /* R; the residual num */
  RK_U32 u32HeadBitNum;                 /* R; the head bit num */
  RK_U32 u32MadiVal;                    /* R; the madi value */
  RK_U32 u32MadpVal;                    /* R; the madp value */
  RK_DOUBLE dPSNRVal;                   /* R; the PSNR value */
  RK_U32 u32MseLcuCnt;                  /* R; the lcu cnt of the mse */
  RK_U32 u32MseSum;                     /* R; the sum of the mse */
  VENC_SSE_INFO_S stSSEInfo[8];         /* R; the information of the sse */
  RK_U32 u32QpHstgrm[VENC_QP_SGRM_NUM]; /* R; the Qp rkstogram value */
  RK_U32 u32MoveScene16x16Num; /* R; the 16x16 cu num of the move scene*/
  RK_U32 u32MoveSceneBits;     /* R; the stream bit num of the move scene */
} VENC_STREAM_ADVANCE_INFO_H264_S;

/* the advance information of the Jpege */
typedef struct rkVENC_STREAM_ADVANCE_INFO_JPEG_S {
  // RK_U32 u32Reserved;
} VENC_STREAM_ADVANCE_INFO_JPEG_S;

/* the advance information of the Prores */
typedef struct rkVENC_STREAM_ADVANCE_INFO_PRORES_S {
  // RK_U32 u32Reserved;
} VENC_STREAM_ADVANCE_INFO_PRORES_S;

/* the advance information of the h265e */
typedef struct rkVENC_STREAM_ADVANCE_INFO_H265_S {
  RK_U32 u32ResidualBitNum;             /* R; the residual num */
  RK_U32 u32HeadBitNum;                 /* R; the head bit num */
  RK_U32 u32MadiVal;                    /* R; the madi value */
  RK_U32 u32MadpVal;                    /* R; the madp value */
  RK_DOUBLE dPSNRVal;                   /* R; the PSNR value */
  RK_U32 u32MseLcuCnt;                  /* R; the lcu cnt of the mse */
  RK_U32 u32MseSum;                     /* R; the sum of the mse */
  VENC_SSE_INFO_S stSSEInfo[8];         /* R; the information of the sse */
  RK_U32 u32QpHstgrm[VENC_QP_SGRM_NUM]; /* R; the Qp histogram value */
  RK_U32 u32MoveScene32x32Num; /* R; the 32x32 cu num of the move scene*/
  RK_U32 u32MoveSceneBits;     /* R; the stream bit num of the move scene */
} VENC_STREAM_ADVANCE_INFO_H265_S;

/* Defines the features of an jpege stream */
typedef struct rkVENC_STREAM_INFO_PRORES_S {
  RK_U32 u32PicBytesNum;
  RK_U32 u32UpdateAttrCnt;
} VENC_STREAM_INFO_PRORES_S;

/* Defines the features of an jpege stream */
typedef struct rkVENC_STREAM_INFO_JPEG_S {
  RK_U32 u32PicBytesNum; /* R; the coded picture stream byte number */
  /* R; Number of times that channel attributes or parameters (including RC
   * parameters) are set*/
  RK_U32 u32UpdateAttrCnt;
  RK_U32 u32Qfactor; /* R; image quality */
} VENC_STREAM_INFO_JPEG_S;

/* Defines the features of an stream */
typedef struct rkVENC_STREAM_S {
  VENC_PACK_S ATTRIBUTE *pstPack; /* R; stream pack attribute*/
  RK_U32 ATTRIBUTE u32PackCount;  /* R; the pack number of one frame stream*/
  RK_U32 u32Seq;                  /* R; the list number of stream*/

  union {
    VENC_STREAM_INFO_H264_S stH264Info;     /* R; the stream info of h264*/
    VENC_STREAM_INFO_JPEG_S stJpegInfo;     /* R; the stream info of jpeg*/
    VENC_STREAM_INFO_H265_S stH265Info;     /* R; the stream info of h265*/
    VENC_STREAM_INFO_PRORES_S stProresInfo; /* R; the stream info of prores*/
  };

  union {
    VENC_STREAM_ADVANCE_INFO_H264_S
    stAdvanceH264Info; /* R; the stream info of h264*/
    VENC_STREAM_ADVANCE_INFO_JPEG_S
    stAdvanceJpegInfo; /* R; the stream info of jpeg*/
    VENC_STREAM_ADVANCE_INFO_H265_S
    stAdvanceH265Info; /* R; the stream info of h265*/
    VENC_STREAM_ADVANCE_INFO_PRORES_S
    stAdvanceProresInfo; /* R; the stream info of prores*/
  };
} VENC_STREAM_S;

typedef struct rkVENC_STREAM_INFO_S {
  H265E_REF_TYPE_E enRefType; /* R;Type of encoded frames in advanced frame
                                 skipping reference mode*/

  RK_U32 u32PicBytesNum; /* R;the coded picture stream byte number */

  /* R;When channel attributes 'bByFrame == 1', it means count of frames.
   * When channel attributes 'bByFrame == 0', it means count of packets.
   */
  RK_U32 u32PicCnt;
  RK_U32 u32StartQp; /* R;the start Qp of encoded frames*/
  RK_U32 u32MeanQp;  /* R;the mean Qp of encoded frames*/
  RK_BOOL bPSkip;

  RK_U32 u32ResidualBitNum; /* R;residual*/
  RK_U32 u32HeadBitNum;     /* R;head information*/
  RK_U32 u32MadiVal;        /* R;madi*/
  RK_U32 u32MadpVal;        /* R;madp*/
  RK_U32 u32MseSum;         /* R;Sum of MSE value */
  RK_U32 u32MseLcuCnt;      /* R;Sum of LCU number */
  RK_DOUBLE dPSNRVal;       /* R;PSNR*/
} VENC_STREAM_INFO_S;

/* the gop mode */
typedef enum rkVENC_GOP_MODE_E {
  VENC_GOPMODE_INIT = 0, /* RO;  init mode(not set) */
  VENC_GOPMODE_NORMALP,  /* RO;  normal p mode */
  VENC_GOPMODE_TSVC2,    /* RO;  tsvc: 2 layer */
  VENC_GOPMODE_TSVC3,    /* RO;  tsvc: 3 layer */
  VENC_GOPMODE_TSVC4,    /* RO;  tsvc: 4 layer */
  VENC_GOPMODE_SMARTP,   /* RO;  smart p mode */
  VENC_GOPMODE_BUTT
} VENC_GOP_MODE_E;

typedef enum rkVENC_ROTATION_E {
  VENC_ROTATION_0 = 0,
  VENC_ROTATION_90 = 90,
  VENC_ROTATION_180 = 180,
  VENC_ROTATION_270 = 270,
  VENC_ROTATION_BUTT
} VENC_ROTATION_E;

typedef struct rkVENC_MPF_CFG_S {
  RK_U8 u8LargeThumbNailNum;
  SIZE_S astLargeThumbNailSize[2];
} VENC_MPF_CFG_S;

typedef enum rkVENC_PIC_RECEIVE_MODE_E {
  VENC_PIC_RECEIVE_SINGLE = 0,
  VENC_PIC_RECEIVE_MULTI,
  VENC_PIC_RECEIVE_BUTT
} VENC_PIC_RECEIVE_MODE_E;

/*the attribute of jpege*/
typedef struct rkVENC_ATTR_JPEG_S {
  RK_BOOL bSupportDCF;     /*RW; Range:[0,1]; support dcf */
  VENC_MPF_CFG_S stMPFCfg; /*RW; Range:[0,1]; config of Mpf*/
  VENC_PIC_RECEIVE_MODE_E enReceiveMode;
} VENC_ATTR_JPEG_S;

/*the attribute of mjpege*/
typedef struct rkVENC_ATTR_MJPEG_S {
  // reserved
} VENC_ATTR_MJPEG_S;

/*the attribute of h264e*/
typedef struct rkVENC_ATTR_H264_S {
  RK_U32 u32Level;
  // reserved
} VENC_ATTR_H264_S;

/*the attribute of h265e*/
typedef struct rkVENC_ATTR_H265_S {
  // reserved
} VENC_ATTR_H265_S;

/*the attribute of the Venc*/
typedef struct rkVENC_ATTR_S {
  RK_CODEC_ID_E enType;
  RK_U32 u32MaxPicWidth;
  RK_U32 u32MaxPicHeight;
  PIXEL_FORMAT_E enPixelFormat;
  MIRROR_E enMirror; // mirror type
  RK_U32 u32BufSize;
  RK_U32 u32Profile;
  RK_BOOL bByFrame;
  RK_U32 u32PicWidth;
  RK_U32 u32PicHeight;
  RK_U32 u32VirWidth; // stride width, eg. u32VirWidth=(u32PicWidth+15)&(~15)
  RK_U32
  u32VirHeight; // stride height, eg. u32VirHeight=(u32PicHeight+15)&(~15)
  RK_U32 u32StreamBufCnt; // RW; cache nums of encoded output
  union {
    VENC_ATTR_H264_S stAttrH264e;   // attributes of H264e
    VENC_ATTR_H265_S stAttrH265e;   // attributes of H265e
    VENC_ATTR_MJPEG_S stAttrMjpege; // attributes of Mjpeg
    VENC_ATTR_JPEG_S stAttrJpege;   // attributes of jpeg
  };
} VENC_ATTR_S;

/*the attribute of the gop*/
typedef struct rkVENC_GOP_ATTR_S {
  VENC_GOP_MODE_E enGopMode; /* RW;  reference gop mode */
  RK_S32 s32VirIdrLen;       /* RW;  virtual IDR frame length for smartp mode*/
  RK_U32 u32MaxLtrCount; /* RW;  normalp and other mode switch dynamic, please
                            setting to 1; only need normalp, setting to 0 to
                            save buffer */
  RK_U32 u32TsvcPreload; /* RW;  smartp and tsvc mode switch dynamic, please
                            setting to 1; only need smartp, setting to 0 to save
                            buffer */
} VENC_GOP_ATTR_S;

/* the attribute of the venc chnl */
typedef struct rkVENC_CHN_ATTR_S {
  VENC_ATTR_S stVencAttr;    /*the attribute of video encoder*/
  VENC_RC_ATTR_S stRcAttr;   /*the attribute of rate ctrl*/
  VENC_GOP_ATTR_S stGopAttr; /*the attribute of gop*/
} VENC_CHN_ATTR_S;

/* the attribute of the buf wrap */
typedef struct rkVENC_CHN_BUF_WRAP_S {
  RK_BOOL bEnable;
  RK_U32 u32BufLine; /* RW; Range: [128, H]; Chn buffer allocated by line. */
  RK_U32 u32WrapBufferSize; /* RW; Whether to allocate buffer according to
                               compression. */
} VENC_CHN_BUF_WRAP_S;

/* the attribute of the buf share */
typedef struct rkVENC_CHN_REF_BUF_SHARE_S {
  RK_BOOL bEnable;
} VENC_CHN_REF_BUF_SHARE_S;

/* the attribute of the combo */
typedef struct rkVENC_COMBO_ATTR_S {
  RK_BOOL bEnable;
  RK_S32 s32ChnId; /* RW; Range: [0, VENC_MAX_CHN_NUM); The src combo channel */
} VENC_COMBO_ATTR_S;

/* the param of receive picture */
typedef struct rkVENC_RECV_PIC_PARAM_S {
  /* RW; Range:[-1, 2147483647]; Number of frames received and encoded by the
     encoding channel, 0 is not supported, -1 is not limit recv pic num*/
  RK_S32 s32RecvPicNum;
} VENC_RECV_PIC_PARAM_S;

/* the status of the venc chnl */
typedef struct rkVENC_CHN_STATUS_S {
  RK_U32 u32LeftPics;         /* R; left picture number */
  RK_U32 u32LeftStreamBytes;  /* R; left stream bytes*/
  RK_U32 u32LeftStreamFrames; /* R; left stream frames*/
  RK_U32 u32CurPacks;         /* R; pack number of current frame*/
  /* R; Number of frames to be received. This member is valid after
   * RK_MPI_VENC_StartRecvPicEx is called.*/
  RK_U32 u32LeftRecvPics;
  /* R; Number of frames to be encoded. This member is valid after
   * RK_MPI_VENC_StartRecvPicEx is called.*/
  RK_U32 u32LeftEncPics;
  RK_BOOL bJpegSnapEnd; /* R; the end of Snap.*/
  VENC_STREAM_INFO_S stVencStrmInfo;
} VENC_CHN_STATUS_S;

/* the param of the h264e slice split */
typedef struct rkVENC_H264_SLICE_SPLIT_S {
  /* RW; Range:[0,1]; slice split enable, RK_TRUE:enable, RK_FALSE:diable,
   * default value:RK_FALSE*/
  RK_BOOL bSplitEnable;
  /* RW; the max number is (Picture height + 15)/16; this value presents the mb
   * line number of one slice*/
  RK_U32 u32MbLineNum;
} VENC_H264_SLICE_SPLIT_S;

/* the param of the h264e intra pred */
typedef struct rkVENC_H264_INTRA_PRED_S {
  /* RW; Range:[0,1];default: RK_FALSE, see the H.264 protocol for the meaning*/
  RK_U32 constrained_intra_pred_flag;
} VENC_H264_INTRA_PRED_S;

/* the param of the h264e trans */
typedef struct rkVENC_H264_TRANS_S {
  /* RW; Range:[0,2]; Conversion mode for inter-prediction&intra-prediction,0:
   * trans4x4, trans8x8; 1: trans4x4*/
  RK_U32 u32TransMode;

  RK_U32
  bScalingListValid;             /* RW; Range:[0,1,2]; enable Scaling, default:
                                    0(disable), 1(scaling list 1), 2(scaling list 2) */
  RK_U8 InterScalingList8X8[64]; /* RW; Range:[1,255]; A quantization table for
                                    8x8 inter-prediction*/
  RK_U8 IntraScalingList8X8[64]; /* RW; Range:[1,255]; A quantization table for
                                    8x8 intra-prediction*/
  /* RW; Range:[-12,12];default value: -6, see the H.264 protocol for the
   * meaning*/
  RK_S32 chroma_qp_index_offset;
} VENC_H264_TRANS_S;

/* the param of the h264e entropy */
typedef struct rkVENC_H264_ENTROPY_S {
  /* RW; Range:[0,1];dafault 1.  Entropy encoding mode for the all frame,
   * 0:cavlc, 1:cabac */
  RK_U32 u32EntropyEncMode;
  /* RW; Range:[0,2];dafault 0. see the H.264 protocol for the meaning(it works
   * when u32EntropyEncMode equal to 1) */
  RK_U32 cabac_init_idc;
} VENC_H264_ENTROPY_S;

/* the config of the h264e poc */
typedef struct rkVENC_H264_POC_S {
  RK_U32 pic_order_cnt_type; /* RW; Range:[0,2]; see the H.264 protocol for the
                                meaning */
} VENC_H264_POC_S;

/* the param of the h264e deblocking */
typedef struct rkVENC_H264_DBLK_S {
  RK_U32 disable_deblocking_filter_idc; /*  RW; Range:[0,2]; see the H.264
                                           protocol for the meaning */
  RK_S32 slice_alpha_c0_offset_div2;    /*  RW; Range:[-6,+6]; see the H.264
                                           protocol for the meaning */
  RK_S32 slice_beta_offset_div2; /*  RW; Range:[-6,+6]; see the H.264 protocol
                                    for the meaning */
} VENC_H264_DBLK_S;

/* the param of the h264e vui timing info */
typedef struct rkVENC_H264_VUI_TIME_INFO_S {
  /* RW; Range:[0,1]; If 1, timing info belows will be encoded into vui.*/
  RK_U8 timing_info_present_flag;
  /* RW; Range:[0,1]; see the H.264 protocol for the meaning. */
  RK_U8 fixed_frame_rate_flag;
  /* RW; Range:(0,4294967295]; see the H.264 protocol for the meaning */
  RK_U32 num_units_in_tick;
  /* RW; Range:(0,4294967295]; see the H.264 protocol for the meaning */
  RK_U32 time_scale;
} VENC_VUI_H264_TIME_INFO_S;

/* the param of the vui aspct ratio */
typedef struct rkVENC_VUI_ASPECT_RATIO_S {
  /* RW; Range:[0,1]; If 1, aspectratio info belows will be encoded into vui */
  RK_U8 aspect_ratio_info_present_flag;
  /* RW; Range:[0,255]; 17~254 is reserved,see the protocol for the meaning.*/
  RK_U8 aspect_ratio_idc;
  /* RW; Range:[0,1]; If 1, oversacan info belows will be encoded into vui.*/
  RK_U8 overscan_info_present_flag;
  /* RW; Range:[0,1]; see the protocol for the meaning. */
  RK_U8 overscan_appropriate_flag;
  /* RW; Range:(0, 65535]; see the protocol for the meaning. */
  RK_U16 sar_width;
  /* RW; Range:(0, 65535]; see the protocol for the meaning. notes:
   * sar_width  and  sar_height  shall  be  relatively  prime.
   */
  RK_U16 sar_height;
} VENC_VUI_ASPECT_RATIO_S;

/* the param of the vui video signal */
typedef struct rkVENC_VUI_VIDEO_SIGNAL_S {
  /* RW; Range:[0,1]; If 1, video singnal info will be encoded into vui. */
  RK_U8 video_signal_type_present_flag;
  /* RW; H.264e Range:[0,7], H.265e Range:[0,5]; see the protocol for the
   * meaning. */
  RK_U8 video_format;
  RK_U8 video_full_range_flag; /* RW; Range: [0,1]; see the protocol for the
                                  meaning.*/
  RK_U8 colour_description_present_flag; /* RO; Range: [0,1]; see the protocol
                                            for the meaning.*/
  RK_U8 colour_primaries; /* RO; Range: [0,255]; see the protocol for the
                             meaning. */
  RK_U8 transfer_characteristics; /* RO; Range: [0,255]; see the protocol for
                                     the meaning. */
  RK_U8 matrix_coefficients;      /* RO; Range:[0,255]; see the protocol for the
                                     meaning. */
} VENC_VUI_VIDEO_SIGNAL_S;

/* the param of the vui video signal */
typedef struct rkVENC_VUI_BITSTREAM_RESTRIC_S {
  RK_U8 bitstream_restriction_flag; /* RW; Range: [0,1]; see the protocol for
                                       the meaning.*/
} VENC_VUI_BITSTREAM_RESTRIC_S;

/* the param of the h264e vui */
typedef struct rkVENC_H264_VUI_S {
  VENC_VUI_ASPECT_RATIO_S stVuiAspectRatio;
  VENC_VUI_H264_TIME_INFO_S stVuiTimeInfo;
  VENC_VUI_VIDEO_SIGNAL_S stVuiVideoSignal;
  VENC_VUI_BITSTREAM_RESTRIC_S stVuiBitstreamRestric;
} VENC_H264_VUI_S;

/* the param of the h265e vui timing info */
typedef struct rkVENC_VUI_H265_TIME_INFO_S {
  /* RW; Range:[0,1]; If 1, timing info belows will be encoded into vui.*/
  RK_U32 timing_info_present_flag;
  /* RW; Range:[0,4294967295]; see the H.265 protocol for the meaning. */
  RK_U32 num_units_in_tick;
  /* RW; Range:(0,4294967295]; see the H.265 protocol for the meaning */
  RK_U32 time_scale;
  /* RW; Range:(0,4294967294]; see the H.265 protocol for the meaning */
  RK_U32 num_ticks_poc_diff_one_minus1;
} VENC_VUI_H265_TIME_INFO_S;

/* the param of the h265e vui */
typedef struct rkVENC_H265_VUI_S {
  VENC_VUI_ASPECT_RATIO_S stVuiAspectRatio;
  VENC_VUI_H265_TIME_INFO_S stVuiTimeInfo;
  VENC_VUI_VIDEO_SIGNAL_S stVuiVideoSignal;
  VENC_VUI_BITSTREAM_RESTRIC_S stVuiBitstreamRestric;
} VENC_H265_VUI_S;

/* the param of the jpege */
typedef struct rkVENC_JPEG_PARAM_S {
  RK_U32 u32Qfactor; /* RW; Range:[1,99]; Qfactor value  */
  RK_U8 u8YQt[64];   /* RW; Range:[1, 255]; Y quantization table */
  RK_U8 u8CbQt[64];  /* RW; Range:[1, 255]; Cb quantization table */
  RK_U8 u8CrQt[64];  /* RW; Range:[1, 255]; Cr quantization table */
  /* RW; the max MCU number is
   * (picwidth + 15) >> 4 x (picheight + 15) >> 4 x 2]; MCU number of one ECS
   */
  RK_U32 u32MCUPerECS;
} VENC_JPEG_PARAM_S;

/* the param of the mjpege */
typedef struct rkVENC_MJPEG_PARAM_S {
  RK_U8 u8YQt[64];  /* RW; Range:[1, 255]; Y quantization table */
  RK_U8 u8CbQt[64]; /* RW; Range:[1, 255]; Cb quantization table */
  RK_U8 u8CrQt[64]; /* RW; Range:[1, 255]; Cr quantization table */
  /* RW; the max MCU number is
   * (picwidth + 15) >> 4 x (picheight + 15) >> 4 x 2]; MCU number of one ECS
   */
  RK_U32 u32MCUPerECS;
} VENC_MJPEG_PARAM_S;

/* the param of the ProRes */
typedef struct rkVENC_PRORES_PARAM_S {
  /* RW; Range:[1, 255]; Luma quantization table */
  RK_U8 u8LumaQt[64];
  /* RW; Range:[1, 255]; Chroma quantization table */
  RK_U8 u8ChromaQt[64];
  /* RW:  identifies the encoder vendor or product that generated the compressed
   * frame*/
  RK_CHAR encoder_identifier[4];
} VENC_PRORES_PARAM_S;

/* the attribute of the roi */
typedef struct rkVENC_ROI_ATTR_S {
  /* RW; Range:[0, 7]; Index of an ROI. The system supports indexes ranging from
   * 0 to 7 */
  RK_U32 u32Index;
  /* RW; Range:[0, 1]; Whether to enable this ROI */
  RK_BOOL bEnable;
  /* RW; Range:[0, 1]; QP mode of an ROI.RK_FALSE: relative QP.RK_TURE: absolute
   * QP.*/
  RK_BOOL bAbsQp;
  /* RW; Range:[-51, 51]; QP value,only relative mode can QP value less than 0.
   */
  RK_S32 s32Qp;
  RK_BOOL bIntra; /* flag of forced intra macroblock*/
  RECT_S stRect;  /* RW; Region of an ROI*/
} VENC_ROI_ATTR_S;

/* ROI struct */
typedef struct rkVENC_ROI_ATTR_EX_S {
  /* RW; Range:[0, 7]; Index of an ROI. The system supports indexes ranging from
   * 0 to 7 */
  RK_U32 u32Index;
  /* RW; Range:[0, 1]; Subscript of array   0: I Frame; 1: P/B Frame; 2: VI
   * Frame; other params are the same. */
  RK_BOOL bEnable[3];
  /* RW; Range:[0, 1]; QP mode of an ROI.RK_FALSE: relative QP.RK_TURE: absolute
   * QP.*/
  RK_BOOL bAbsQp[3];
  /* RW; Range:[-51, 51]; QP value,only relative mode can QP value less than 0.
   */
  RK_S32 s32Qp[3];
  RECT_S stRect[3]; /* RW;Region of an ROI*/
} VENC_ROI_ATTR_EX_S;

/* the param of the roibg frame rate */
typedef struct rkVENC_ROIBG_FRAME_RATE_S {
  /* RW; Range:[-1, 2147483647];Source frame rate of a non-ROI,can not be
   * configured 0*/
  RK_S32 s32SrcFrmRate;
  /* RW; Range:[-1, 2147483647];Target frame rate of a non-ROI,can not be larger
   * than s32SrcFrmRate*/
  RK_S32 s32DstFrmRate;
} VENC_ROIBG_FRAME_RATE_S;

/* the param of the roibg frame rate */
typedef struct rkVENC_REF_PARAM_S {
  RK_U32 u32Base;    /* RW; Range:[0,4294967295]; Base layer period*/
  RK_U32 u32Enhance; /* RW; Range:[0,255]; Enhance layer period*/
  /* RW; Range:[0, 1]; Whether some frames at the base layer are referenced by
   * other frames at the base layer. When bEnablePred is rk_FALSE, all frames at
   * the base layer reference IDR frames.
   */
  RK_BOOL bEnablePred;
} VENC_REF_PARAM_S;

/* Jpeg snap mode */
typedef enum rkVENC_JPEG_ENCODE_MODE_E {
  JPEG_ENCODE_ALL = 0, /* Jpeg channel snap all the pictures when started. */
  JPEG_ENCODE_SNAP =
      1, /* Jpeg channel snap the flashed pictures when started. */
  JPEG_ENCODE_BUTT,
} VENC_JPEG_ENCODE_MODE_E;

/* the information of the stream */
typedef struct rkVENC_STREAM_BUF_INFO_S {
  RK_U64 u64PhyAddr[MAX_TILE_NUM]; /* R; Start physical address for a stream
                                      buffer */
  RK_VOID ATTRIBUTE *pUserAddr[MAX_TILE_NUM]; /* R; Start virtual address for a
                                                 stream buffer */
  RK_U64 ATTRIBUTE u64BufSize[MAX_TILE_NUM];  /* R; Stream buffer size */
} VENC_STREAM_BUF_INFO_S;

/* the param of the h265e slice split */
typedef struct rkVENC_H265_SLICE_SPLIT_S {
  /* RW; Range:[0,1]; slice split enable, RK_TRUE:enable, RK_FALSE:diable,
   * default value:RK_FALSE */
  RK_BOOL bSplitEnable;
  /* RW; Range:(Picture height + lcu size minus one)/lcu size;this value
   * presents lcu line number */
  RK_U32 u32LcuLineNum;
} VENC_H265_SLICE_SPLIT_S;

/* the param of the h265e pu */
typedef struct rkVENC_H265_PU_S {
  RK_U32 constrained_intra_pred_flag; /* RW; Range:[0,1]; see the H.265 protocol
                                         for the meaning. */
  RK_U32 strong_intra_smoothing_enabled_flag; /* RW; Range:[0,1]; see the H.265
                                                 protocol for the meaning. */
} VENC_H265_PU_S;

/* the param of the h265e trans */
typedef struct rkVENC_H265_TRANS_S {
  RK_S32 cb_qp_offset; /* RW; Range:[-12,12]; see the H.265 protocol for the
                          meaning. */
  RK_S32 cr_qp_offset; /* RW; Range:[-12,12]; see the H.265 protocol for the
                          meaning. */

  RK_U32 bScalingListEnabled; /* RW; Range:[0,1,2]; enable Scaling, default:
                                 0(disable), 1(scaling list 1), 2(scaling list
                                 2) */

  RK_BOOL bScalingListTu4Valid; /* RW; Range:[0,1]; If 1, ScalingList4X4 belows
                                   will be encoded.*/
  RK_U8 InterScalingList4X4[2][16]; /* RW; Range:[1,255]; Scaling List for inter
                                       4X4 block.*/
  RK_U8 IntraScalingList4X4[2][16]; /* RW; Range:[1,255]; Scaling List for intra
                                       4X4 block.*/

  RK_BOOL bScalingListTu8Valid; /* RW; Range:[0,1]; If 1, ScalingList8X8 belows
                                   will be encoded.*/
  RK_U8 InterScalingList8X8[2][64]; /* RW; Range:[1,255]; Scaling List for inter
                                       8X8 block.*/
  RK_U8 IntraScalingList8X8[2][64]; /* RW; Range:[1,255]; Scaling List for intra
                                       8X8 block.*/

  RK_BOOL bScalingListTu16Valid;      /* RW; Range:[0,1]; If 1, ScalingList16X16
                                         belows will be encoded.*/
  RK_U8 InterScalingList16X16[2][64]; /* RW; Range:[1,255]; Scaling List for
                                         inter 16X16 block..*/
  RK_U8 IntraScalingList16X16[2][64]; /* RW; Range:[1,255]; Scaling List for
                                         inter 16X16 block.*/

  RK_BOOL bScalingListTu32Valid;   /* RW; Range:[0,1]; If 1, ScalingList32X32
                                      belows will be encoded.*/
  RK_U8 InterScalingList32X32[64]; /* RW; Range:[1,255]; Scaling List for inter
                                      32X32 block..*/
  RK_U8 IntraScalingList32X32[64]; /* RW; Range:[1,255]; Scaling List for inter
                                      32X32 block.*/
} VENC_H265_TRANS_S;

/* the param of the h265e entroy */
typedef struct rkVENC_H265_ENTROPY_S {
  RK_U32 cabac_init_flag; /* RW; Range:[0,1]; see the H.265 protocol for the
                             meaning. */
} VENC_H265_ENTROPY_S;

/* the param of the h265e deblocking */
typedef struct rkVENC_H265_DBLK_S {
  RK_U32
  slice_deblocking_filter_disabled_flag; /* RW; Range:[0,1]; see the H.265
                                            protocol for the meaning. */
  RK_S32 slice_beta_offset_div2; /* RW; Range:[-6,6]; see the H.265 protocol for
                                    the meaning. */
  RK_S32 slice_tc_offset_div2;   /* RW; Range:[-6,6]; see the H.265 protocol for
                                    the meaning. */
} VENC_H265_DBLK_S;

/* the param of the h265e sao */
typedef struct rkVENC_H265_SAO_S {
  /*RW; Range:[0,1]; Indicates whether SAO filtering is performed on the
   * luminance component of the current slice. */
  RK_U32 slice_sao_luma_flag;
  /*RW; Range:[0,1]; Indicates whether SAO filtering is performed on the
   * chrominance component of the current slice*/
  RK_U32 slice_sao_chroma_flag;
  /*RW; Range:[0,7]*/
  RK_U32 slice_sao_bit_ratio;
} VENC_H265_SAO_S;

/* venc mode type */
typedef enum rkVENC_INTRA_REFRESH_MODE_E {
  INTRA_REFRESH_ROW = 0, /* Line mode */
  INTRA_REFRESH_COLUMN,  /* Column mode */
  INTRA_REFRESH_BUTT
} VENC_INTRA_REFRESH_MODE_E;

/* the param of the intra refresh */
typedef struct rkVENC_INTRA_REFRESH_S {
  /*RW; Range:[0,1]; intra refresh enable, RK_TRUE:enable, RK_FALSE:diable,
   * default value:RK_FALSE*/
  RK_BOOL bRefreshEnable;
  /*RW; The mode of intra refresh*/
  VENC_INTRA_REFRESH_MODE_E enIntraRefreshMode;
  /*RW; Number of rows/column to be refreshed during each I macroblock refresh*/
  RK_U32 u32RefreshNum;
  /*RW; Range:[0,51]; QP value of the I frame*/
  RK_U32 u32ReqIQp;
} VENC_INTRA_REFRESH_S;

/* venc mode type */
typedef enum rkVENC_MODTYPE_E {
  MODTYPE_VENC = 1, /* VENC */
  MODTYPE_H264E,    /* H264e */
  MODTYPE_H265E,    /* H265e */
  MODTYPE_JPEGE,    /* Jpege */
  MODTYPE_RC,       /* Rc */
  MODTYPE_BUTT
} VENC_MODTYPE_E;

/* the param of the h264e mod */
typedef struct rkVENC_MOD_H264E_S {
  RK_U32 u32OneStreamBuffer;   /* RW; Range:[0,1]; one stream buffer*/
  RK_U32 u32H264eMiniBufMode;  /* RW; Range:[0,1]; H264e MiniBufMode*/
  RK_U32 u32H264ePowerSaveEn;  /* RW; Range:[0,1]; H264e PowerSaveEn*/
  MB_SOURCE_E enH264eMBSource; /* RW; H264e VBSource*/
  RK_BOOL bQpHstgrmEn;         /* RW; Range:[0,1]*/
} VENC_MOD_H264E_S;

/* the param of the h265e mod */
typedef struct rkVENC_MOD_H265E_S {
  RK_U32 u32OneStreamBuffer;   /* RW; Range:[0,1]; one stream buffer*/
  RK_U32 u32H265eMiniBufMode;  /* RW; Range:[0,1]; H265e MiniBufMode*/
  RK_U32 u32H265ePowerSaveEn;  /* RW; Range:[0,2]; H265e PowerSaveEn*/
  MB_SOURCE_E enH265eMBSource; /* RW; H265e VBSource*/
  RK_BOOL bQpHstgrmEn;         /* RW; Range:[0,1]*/
} VENC_MOD_H265E_S;

/* the param of the jpege mod */
typedef struct rkVENC_MOD_JPEGE_S {
  RK_U32 u32OneStreamBuffer;    /* RW; Range:[0,1]; one stream buffer*/
  RK_U32 u32JpegeMiniBufMode;   /* RW; Range:[0,1]; Jpege MiniBufMode*/
  RK_U32 u32JpegClearStreamBuf; /* RW; Range:[0,1]; JpegClearStreamBuf*/
} VENC_MOD_JPEGE_S;

typedef struct rkVENC_MOD_RC_S {
  RK_U32 u32ClrStatAfterSetBr;
} VENC_MOD_RC_S;
/* the param of the venc mod */
typedef struct rkVENC_MOD_VENC_S {
  RK_U32 u32VencBufferCache; /* RW; Range:[0,1]; VencBufferCache*/
  RK_U32 u32FrameBufRecycle; /* RW; Range:[0,1]; FrameBufRecycle*/
} VENC_MOD_VENC_S;

/* the param of the mod */
typedef struct rkVENC_MODPARAM_S {
  VENC_MODTYPE_E enVencModType; /* RW; VencModType*/
  union {
    VENC_MOD_VENC_S stVencModParam;
    VENC_MOD_H264E_S stH264eModParam;
    VENC_MOD_H265E_S stH265eModParam;
    VENC_MOD_JPEGE_S stJpegeModParam;
    VENC_MOD_RC_S stRcModParam;
  };
} VENC_PARAM_MOD_S;

typedef enum rkVENC_FRAME_TYPE_E {
  VENC_FRAME_TYPE_NONE = 1,
  VENC_FRAME_TYPE_IDR,
  VENC_FRAME_TYPE_BUTT
} VENC_FRAME_TYPE_E;

/* the information of the user rc*/
typedef struct rkUSER_RC_INFO_S {
  /* RW; Range:[0,1]; Indicates whether the QpMap mode is valid for the current
   * frame*/
  RK_BOOL bQpMapValid;
  /* RW; Range:[0,1]; Indicates whether the SkipWeight mode is valid for the
   * current frame*/
  RK_BOOL bSkipWeightValid;
  /* RW; Range:[0,51];QP value of the first 16 x 16 block in QpMap mode */
  RK_U32 u32BlkStartQp;
  MB_BLK pMbBlkQpMap;      /* RW; Blk of the QP table in QpMap mode*/
  MB_BLK pMbBlkSkipWeight; /* RW; Blk of the SkipWeight table in QpMap mode*/
  VENC_FRAME_TYPE_E
  enFrameType; /* RW; Encoding frame type of the current frame */
} USER_RC_INFO_S;

/* the information of the user frame*/
typedef struct rkUSER_FRAME_INFO_S {
  VIDEO_FRAME_INFO_S stUserFrame;
  USER_RC_INFO_S stUserRcInfo;
} USER_FRAME_INFO_S;

/* the config of the sse*/
typedef struct rkVENC_SSE_CFG_S {
  /* RW; Range:[0, 7]; Index of an SSE. The system supports indexes ranging from
   * 0 to 7 */
  RK_U32 u32Index;
  RK_BOOL bEnable; /* RW; Range:[0, 1]; Whether to enable SSE */
  RECT_S stRect;   /* RW; */
} VENC_SSE_CFG_S;

typedef enum rkVENC_CROP_TYPE_E {
  VENC_CROP_NONE = 0,
  VENC_CROP_ONLY,
  VENC_CROP_SCALE,
  VENC_CROP_BUTT
} VENC_CROP_TYPE_E;

typedef struct rkVENC_SCALE_RECT_S {
  RECT_S stSrc; /* RW; Scale src region, note: x/y/w/h must be multi of 2, scale
                   ratio not more than 16*/
  RECT_S stDst; /* RW; Scale dst region, note: x/y/w/h must be multi of 2, scale
                   ratio not more than 16*/
} VENC_SCALE_RECT_S;

/* the param of the crop/scale */
typedef struct rkVENC_CROP_INFO_S {
  VENC_CROP_TYPE_E enCropType; /* RW; Range:[0, 3); Crop/Scale region enable */
  RECT_S stCropRect; /* RW; Crop region, note: x/w/h must be multi of 2*/
  VENC_SCALE_RECT_S
  stScaleRect; /* RW; Scale region, note: x/y/w/h must be multi of 2 */
} VENC_CROP_INFO_S;

/* the param of the venc frame rate */
typedef struct rkVENC_FRAME_RATE_S {
  RK_BOOL bEnable;         /* RW; Range:[0, 1];  frame rate enable */
  RK_S32 s32SrcFrmRateNum; /* RW; Range:[0, 240]; Input frame rate of num for a
                              channel*/
  RK_S32 s32SrcFrmRateDen; /* RW; Range:[0, 240]; Input frame rate of den for a
                              channel*/
  RK_S32 s32DstFrmRateNum; /* RW; Range:[0, 240]; Output frame rate of num for a
                              channel*/
  RK_S32 s32DstFrmRateDen; /* RW; Range:[0, 240]; Output frame rate of den for a
                              channel*/
} VENC_FRAME_RATE_S;

/* the param of the venc encode chnl */
typedef struct rkVENC_CHN_PARAM_S {
  RK_BOOL bColor2Grey; /* RW; Range:[0, 1]; Whether to enable Color2Grey.*/
  RK_U32 u32Priority;  /* RW; Range:[0, 1]; The priority of the coding chnl.*/
  /* RW: Range:[0,4294967295]; Maximum number of frames in a stream buffer*/
  RK_U32 u32MaxStrmCnt;
  /* RW: Range:(0,4294967295]; the frame num needed to wake up  obtaining
   * streams */
  RK_U32 u32PollWakeUpFrmCnt;
  VENC_CROP_INFO_S stCropCfg;
  VENC_FRAME_RATE_S stFrameRate;
} VENC_CHN_PARAM_S;

/*the ground protect of FOREGROUND*/
typedef struct rkVENC_FOREGROUND_PROTECT_S {
  RK_BOOL bForegroundCuRcEn;
  /*RW; Range:[0, 16]; The direction for controlling the macroblock-level bit
   * rate*/
  RK_U32 u32ForegroundDirectionThresh;
  /*RW; Range:[0, 15]; The gain of the thresh*/
  RK_U32 u32ForegroundThreshGain;
  /*RW; Range:[0, 255]; The offset of the thresh*/
  RK_U32 u32ForegroundThreshOffset;
  /*RW; Range:[0, 255]; Mad threshold for controlling the foreground
   * macroblock-level bit rate of P frames */
  RK_U32 u32ForegroundThreshP[RC_TEXTURE_THR_SIZE];
  /*RW; Range:[0, 255]; Mad threshold for controlling the foreground
   * macroblock-level bit rate of B frames */
  RK_U32 u32ForegroundThreshB[RC_TEXTURE_THR_SIZE];
} VENC_FOREGROUND_PROTECT_S;

/* the scene mode of the venc encode chnl */
typedef enum rkVENC_SCENE_MODE_E {
  /* RW; A scene in which the camera does not move or periodically moves
     continuously*/
  SCENE_0 = 0,
  /* RW; Motion scene at high bit rate*/
  SCENE_1 = 1,
  /* RW; It has regular continuous motion at medium bit rate and the encoding
     pressure is relatively large*/
  SCENE_2 = 2,
  SCENE_BUTT
} VENC_SCENE_MODE_E;

typedef struct rkVENC_DEBREATHEFFECT_S {
  RK_BOOL bEnable;     /* RW; Range:[0,1];default: 0, DeBreathEffect enable */
  RK_S32 s32Strength0; /* RW; Range:[0,35];The Strength0 of DeBreathEffect.*/
  RK_S32 s32Strength1; /* RW; Range:[0,35];The Strength1 of DeBreathEffect.*/
} VENC_DEBREATHEFFECT_S;

typedef struct rkVENC_CU_PREDICTION_S {
  OPERATION_MODE_E enPredMode; /* RW; CU tendency configuration mode  */

  RK_U32
  u32Intra32Cost; /* RW; Range:[0,15]; Tendency adjustment in Intra32 mode*/
  RK_U32
  u32Intra16Cost; /* RW; Range:[0,15]; Tendency adjustment in Intra16 mode*/
  RK_U32
  u32Intra8Cost; /* RW; Range:[0,15]; Tendency adjustment in Intra8 mode*/
  RK_U32
  u32Intra4Cost; /* RW; Range:[0,15]; Tendency adjustment in Intra4 mode*/

  RK_U32
  u32Inter64Cost; /* RW; Range:[0,15]; Tendency adjustment in Intra64 mode*/
  RK_U32
  u32Inter32Cost; /* RW; Range:[0,15]; Tendency adjustment in Inter32 mode*/
  RK_U32
  u32Inter16Cost; /* RW; Range:[0,15]; Tendency adjustment in Inter16 mode*/
  RK_U32
  u32Inter8Cost; /* RW; Range:[0,15]; Tendency adjustment in Inter8 mode*/
} VENC_CU_PREDICTION_S;

typedef struct rkVENC_SKIP_BIAS_S {
  /* RW; Range:[0,1];   Flag indicating whether the skip tendency function is
   * enabled*/
  RK_BOOL bSkipBiasEn;
  /* RW; Range:[0,15];  used to calculate the SAD threshold for foreground
   * detection*/
  RK_U32 u32SkipThreshGain;
  /* RW; Range:[0,255]; used to calculate the SAD threshold for foreground
   * detection*/
  RK_U32 u32SkipThreshOffset;
  RK_U32 u32SkipBackgroundCost; /* RW; Range:[0,15];  Skip tendency adjustment
                                   in the background*/
  RK_U32 u32SkipForegroundCost; /* RW; Range:[0,15];  Skip tendency adjustment
                                   in the foreground*/
} VENC_SKIP_BIAS_S;

typedef struct rkVENC_HIERARCHICAL_QP_S {
  RK_BOOL bHierarchicalQpEn; /* RW; Range:[0,1];    Hierarchical QP enable*/
  /* RW; Range:[-10,10]; QP delta of the frames at each layer relative to the
   * P-frame at layer 0*/
  RK_S32 s32HierarchicalQpDelta[4];
  /* RW; Range:[0,5]; Number of frames at each layer*/
  RK_S32 s32HierarchicalFrameNum[4];
} VENC_HIERARCHICAL_QP_S;

typedef struct rkVENC_CHN_POOL_S {
  //  VB_POOL hPicVbPool;     /* RW;  vb pool id for pic buffer */
  //  VB_POOL hPicInfoVbPool; /* RW;  vb pool id for pic info buffer */
} VENC_CHN_POOL_S;

typedef struct rkVENC_RC_ADVPARAM_S {
  RK_U32 u32ClearStatAfterSetAttr; /* RW; Range:[0,1]; Clear Stat After SetAttr
                                      enable */
} VENC_RC_ADVPARAM_S;

typedef enum rkRC_SUPERFRM_MODE_E {
  SUPERFRM_NONE = 0,
  SUPERFRM_DISCARD,
  SUPERFRM_REENCODE,
  SUPERFRM_BUTT
} VENC_SUPERFRM_MODE_E;

typedef enum rkVENC_RC_PRIORITY_E {
  VENC_RC_PRIORITY_BITRATE_FIRST = 1,
  VENC_RC_PRIORITY_FRAMEBITS_FIRST,
  VENC_RC_PRIORITY_BUTT,
} VENC_RC_PRIORITY_E;

typedef struct rkVENC_SUPERFRAME_CFG_S {
  VENC_SUPERFRM_MODE_E enSuperFrmMode;
  RK_U32 u32SuperIFrmBitsThr;
  RK_U32 u32SuperPFrmBitsThr;
  RK_U32 u32SuperBFrmBitsThr;
  VENC_RC_PRIORITY_E enRcPriority;
} VENC_SUPERFRAME_CFG_S;

typedef enum rkVENC_FRAMELOST_MODE_E {
  FRMLOST_NORMAL = 0,
  FRMLOST_PSKIP,
  FRMLOST_BUTT,
} VENC_FRAMELOST_MODE_E;

typedef struct rkVENC_FRAMELOST_S {
  RK_BOOL bFrmLostOpen;
  RK_U32 u32FrmLostBpsThr;
  VENC_FRAMELOST_MODE_E enFrmLostMode;
  RK_U32 u32EncFrmGaps;
} VENC_FRAMELOST_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef  INCLUDE_RT_MPI_RK_COMM_VENC_H_ */
