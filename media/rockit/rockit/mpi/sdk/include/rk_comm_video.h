/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_RK_COMMON_VIDEO_H_
#define INCLUDE_RT_MPI_RK_COMMON_VIDEO_H_

#include "rk_comm_mb.h"
#include "rk_common.h"
#include "rk_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define SRC_LENS_COEF_SEG 2
#define DST_LENS_COEF_SEG 3
#define SRC_LENS_COEF_NUM 4
#define DST_LENS_COEF_NUM 4
#define DST_LENS_COEF_SEG_POINT (DST_LENS_COEF_SEG - 1)

#define RK_COLOR_YUV_Y_PLANE 0
#define RK_COLOR_YUV_UV_PLANE 1
#define RK_COLOR_RGB_PLANE 0
#define RK_MAX_COLOR_COMPONENT 2

#define ISP_BAYER_CHN (4)

typedef enum rkOPERATION_MODE_E {
  OPERATION_MODE_AUTO = 0,
  OPERATION_MODE_MANUAL = 1,
  OPERATION_MODE_BUTT
} OPERATION_MODE_E;

/*Angle of rotation*/
typedef enum rkROTATION_E {
  ROTATION_0 = 0,
  ROTATION_90 = 1,
  ROTATION_180 = 2,
  ROTATION_270 = 3,
  ROTATION_BUTT
} ROTATION_E;

typedef enum rkMIRROR_E {
  MIRROR_NONE = 0,
  MIRROR_HORIZONTAL = 1, /* RW; MIRROR */
  MIRROR_VERTICAL = 2,   /* RW; FLIP */
  MIRROR_BOTH = 3,       /* RW; MIRROR and FLIP */
  MIRROR_BUTT
} MIRROR_E;

typedef enum rkMB_SOURCE_E {
  MB_SOURCE_COMMON = 0,
  MB_SOURCE_MODULE = 1,
  MB_SOURCE_PRIVATE = 2,
  MB_SOURCE_USER = 3,
  MB_SOURCE_BUTT
} MB_SOURCE_E;

typedef enum rkDATA_RATE_E {
  DATA_RATE_X1 = 0, /* RW; output 1 pixel per clock */
  DATA_RATE_X2 = 1, /* RW; output 2 pixel per clock */

  DATA_RATE_BUTT
} DATA_RATE_E;

typedef struct rkBORDER_S {
  RK_U32 u32TopWidth;
  RK_U32 u32BottomWidth;
  RK_U32 u32LeftWidth;
  RK_U32 u32RightWidth;
  RK_U32 u32Color;
} BORDER_S;

typedef struct rkPOINT_S {
  RK_S32 s32X;
  RK_S32 s32Y;
} POINT_S;

typedef struct rkSIZE_S {
  RK_U32 u32Width;
  RK_U32 u32Height;
} SIZE_S;

typedef struct rkRECT_S {
  RK_S32 s32X;
  RK_S32 s32Y;
  RK_U32 u32Width;
  RK_U32 u32Height;
} RECT_S;

typedef struct rkVIDEO_REGION_INFO_S {
  RK_U32 u32RegionNum;         /* W; count of the region */
  RECT_S ATTRIBUTE *pstRegion; /* W; region attribute */
} VIDEO_REGION_INFO_S;

typedef struct rkCROP_INFO_S {
  RK_BOOL bEnable;
  RECT_S stRect;
} CROP_INFO_S;

typedef struct rkFRAME_RATE_CTRL_S {
  RK_S32 s32SrcFrameRate; /* RW; source frame rate */
  RK_S32 s32DstFrameRate; /* RW; dest frame rate */
} FRAME_RATE_CTRL_S;

typedef enum rkASPECT_RATIO_E {
  ASPECT_RATIO_NONE = 0,   /* full screen */
  ASPECT_RATIO_AUTO = 1,   /* ratio no change, 1:1*/
  ASPECT_RATIO_MANUAL = 2, /* ratio manual set */
  ASPECT_RATIO_BUTT
} ASPECT_RATIO_E;

typedef struct rkASPECT_RATIO_S {
  ASPECT_RATIO_E enMode; /* aspect ratio mode: none/auto/manual */
  RK_U32 u32BgColor;     /* background color, RGB 888 */
  RECT_S stVideoRect;    /* valid in ASPECT_RATIO_MANUAL mode */
} ASPECT_RATIO_S;

#define RK_VIDEO_FMT_MASK 0x000f0000
#define RK_VIDEO_FMT_YUV 0x00000000
#define RK_VIDEO_FMT_RGB 0x00010000
#define RK_VIDEO_FMT_BAYER 0X00020000

typedef enum rkPIXEL_FORMAT_E {
  RK_FMT_YUV420SP = RK_VIDEO_FMT_YUV, /* YYYY... UV...            */
  RK_FMT_YUV420SP_10BIT,
  RK_FMT_YUV422SP,       /* YYYY... UVUV...          */
  RK_FMT_YUV422SP_10BIT, ///< Not part of ABI
  RK_FMT_YUV420P,        /* YYYY... UUUU... VVVV     */
  RK_FMT_YUV420P_VU,     /* YYYY... VVVV... UUUU     */
  RK_FMT_YUV420SP_VU,    /* YYYY... VUVUVU...        */
  RK_FMT_YUV422P,        /* YYYY... UUUU... VVVV     */
  RK_FMT_YUV422SP_VU,    /* YYYY... VUVUVU...        */
  RK_FMT_YUV422_YUYV,    /* YUYVYUYV...              */
  RK_FMT_YUV422_UYVY,    /* UYVYUYVY...              */
  RK_FMT_YUV400SP,       /* YYYY...                  */
  RK_FMT_YUV440SP,       /* YYYY... UVUV...          */
  RK_FMT_YUV411SP,       /* YYYY... UV...            */
  RK_FMT_YUV444,         /* YUVYUVYUV...             */
  RK_FMT_YUV444SP,       /* YYYY... UVUVUVUV...      */
  RK_FMT_YUV444P,        /* YYYY... UUUU... VVVV     */
  RK_FMT_YUV422_YVYU,    /* YVYUYVYU...              */
  RK_FMT_YUV422_VYUY,    /* VYUYVYUY...              */
  RK_FMT_YUV_BUTT,

  RK_FMT_RGB565 = RK_VIDEO_FMT_RGB, /* 16-bit RGB               */
  RK_FMT_BGR565,                    /* 16-bit RGB               */
  RK_FMT_RGB555,                    /* 15-bit RGB               */
  RK_FMT_BGR555,                    /* 15-bit RGB               */
  RK_FMT_RGB444,                    /* 12-bit RGB               */
  RK_FMT_BGR444,                    /* 12-bit RGB               */
  RK_FMT_RGB888,                    /* 24-bit RGB               */
  RK_FMT_BGR888,                    /* 24-bit RGB               */
  RK_FMT_RGB101010,                 /* 30-bit RGB               */
  RK_FMT_BGR101010,                 /* 30-bit RGB               */
  RK_FMT_ARGB1555,                  /* 16-bit RGB               */
  RK_FMT_ABGR1555,                  /* 16-bit RGB               */
  RK_FMT_ARGB4444,                  /* 16-bit RGB               */
  RK_FMT_ABGR4444,                  /* 16-bit RGB               */
  RK_FMT_ARGB8565,                  /* 24-bit RGB               */
  RK_FMT_ABGR8565,                  /* 24-bit RGB               */
  RK_FMT_ARGB8888,                  /* 32-bit RGB               */
  RK_FMT_ABGR8888,                  /* 32-bit RGB               */
  RK_FMT_BGRA8888,                  /* 32-bit RGB               */
  RK_FMT_RGBA8888,                  /* 32-bit RGB               */
  RK_FMT_RGBA5551,                  /* 16-bit RGB               */
  RK_FMT_BGRA5551,                  /* 16-bit RGB               */
  RK_FMT_BGRA4444,                  /* 16-bit RGB               */
  RK_FMT_RGBA4444,                  /* 16-bit RGB               */
  RK_FMT_XBGR8888,                  /* 32-bit RGB               */
  RK_FMT_RGB_BUTT,

  RK_FMT_2BPP,

  RK_FMT_RGB_BAYER_SBGGR_8BPP = RK_VIDEO_FMT_BAYER, /* 8-bit raw */
  RK_FMT_RGB_BAYER_SGBRG_8BPP,  /* 8-bit raw                */
  RK_FMT_RGB_BAYER_SGRBG_8BPP,  /* 8-bit raw                */
  RK_FMT_RGB_BAYER_SRGGB_8BPP,  /* 8-bit raw                */
  RK_FMT_RGB_BAYER_SBGGR_10BPP, /* 10-bit raw               */
  RK_FMT_RGB_BAYER_SGBRG_10BPP, /* 10-bit raw               */
  RK_FMT_RGB_BAYER_SGRBG_10BPP, /* 10-bit raw               */
  RK_FMT_RGB_BAYER_SRGGB_10BPP, /* 10-bit raw               */
  RK_FMT_RGB_BAYER_SBGGR_12BPP, /* 12-bit raw               */
  RK_FMT_RGB_BAYER_SGBRG_12BPP, /* 12-bit raw               */
  RK_FMT_RGB_BAYER_SGRBG_12BPP, /* 12-bit raw               */
  RK_FMT_RGB_BAYER_SRGGB_12BPP, /* 12-bit raw               */
  RK_FMT_RGB_BAYER_14BPP,       /* 14-bit raw               */
  RK_FMT_RGB_BAYER_SBGGR_16BPP, /* 16-bit raw               */
  RK_FMT_RGB_BAYER_BUTT,
  RK_FMT_BUTT = RK_FMT_RGB_BAYER_BUTT,
} PIXEL_FORMAT_E;

typedef enum rkVIDEO_PROC_DEV_TYPE_E {
  VIDEO_PROC_DEV_GPU = 0x0, /* GPU device */
  VIDEO_PROC_DEV_RGA = 0x1, /* RGA device */

  VIDEO_PROC_DEV_BUTT
} VIDEO_PROC_DEV_TYPE_E;

typedef enum rkVIDEO_FIELD_E {
  VIDEO_FIELD_TOP = 0x1,        /* even field */
  VIDEO_FIELD_BOTTOM = 0x2,     /* odd field */
  VIDEO_FIELD_INTERLACED = 0x3, /* two interlaced fields */
  VIDEO_FIELD_FRAME = 0x4,      /* frame */

  VIDEO_FIELD_BUTT
} VIDEO_FIELD_E;

typedef enum rkVIDEO_FORMAT_E {
  VIDEO_FORMAT_LINEAR = 0,      /* nature video line */
  VIDEO_FORMAT_TILE_64x16,      /* tile cell: 64pixel x 16line */
  VIDEO_FORMAT_TILE_16x8,       /* tile cell: 16pixel x 8line */
  VIDEO_FORMAT_LINEAR_DISCRETE, /* The data bits are aligned in bytes */
  VIDEO_FORMAT_BUTT
} VIDEO_FORMAT_E;

typedef enum rkCOMPRESS_MODE_E {
  COMPRESS_MODE_NONE = 0, /* no compress */
  COMPRESS_AFBC_16x16,

  COMPRESS_MODE_BUTT
} COMPRESS_MODE_E;

typedef enum rkVIDEO_DISPLAY_MODE_E {
  VIDEO_DISPLAY_MODE_PREVIEW = 0x0,
  VIDEO_DISPLAY_MODE_PLAYBACK = 0x1,

  VIDEO_DISPLAY_MODE_BUTT
} VIDEO_DISPLAY_MODE_E;

typedef struct rkLUMA_INFO_S {
  RK_U64 u64LumaPixSum;     /* Luma sum of current frame */
  RK_U32 u32LumaPixAverage; /* Luma average of current frame */
  RK_U64 u64PTS;            /* PTS of current frame  */
} LUMA_INFO_S;

#define LDCI_STAT_WND_X 24
#define LDCI_STAT_WND_Y 16

#define DCF_DRSCRIPTION_LENGTH 32
#define DCF_CAPTURE_TIME_LENGTH 20

typedef struct rkISP_DCF_CONST_INFO_S {
  /* Describes image*/
  RK_U8 au8ImageDescription[DCF_DRSCRIPTION_LENGTH];
  /* Shows manufacturer of digital cameras*/
  RK_U8 au8Make[DCF_DRSCRIPTION_LENGTH];
  /* Shows model number of digital cameras*/
  RK_U8 au8Model[DCF_DRSCRIPTION_LENGTH];
  /* Shows firmware (internal software of digital cameras) version number*/
  RK_U8 au8Software[DCF_DRSCRIPTION_LENGTH];
  /*
   * Light source, actually this means white balance setting.
   * '0' means unknown, '1' daylight, '2' fluorescent, '3' tungsten,
   * '10' flash, '17' standard light A, '18' standard light B,
   * '19' standard light C, '20' D55, '21' D65, '22' D75, '255' other
   */
  RK_U8 u8LightSource;
  /* Focal length of lens used to take image. Unit is millimeter*/
  RK_U32 u32FocalLength;
  /* Indicates the type of scene. Value '0x01' means that the image was directly
   * photographed.*/
  RK_U8 u8SceneType;
  /*
   * Indicates the use of special processing on image data,
   * such as rendering geared to output.0 = Normal process
   * 1 = Custom process
   */
  RK_U8 u8CustomRendered;
  /* Indicates the equivalent focal length assuming a 35mm film camera, in mm*/
  RK_U8 u8FocalLengthIn35mmFilm;
  /*
   * Indicates the type of scene that was shot. 0 = Standard,
   * 1 = Landscape,2 = Portrait,3 = Night scene.
   */
  RK_U8 u8SceneCaptureType;
  /*
   * Indicates the degree of overall image gain adjustment.
   * 0 = None,1 = Low gain up,2 = High gain up,
   * 3 = Low gain down,4 = High gain down.
   */
  RK_U8 u8GainControl;
  /*
   * Indicates the direction of contrast processing applied
   * by the camera when the image was shot.0 = Normal,1 = Soft,2 = Hard
   */
  RK_U8 u8Contrast;
  /*
   * Indicates the direction of saturation processing applied
   * by the camera when the image was shot.
   * 0 = Normal,1 = Low saturation,2 = High saturation
   */
  RK_U8 u8Saturation;
  /*
   * Indicates the direction of sharpness processing applied by
   * the camera when the image was shot.0 = Normal,1 = Soft,2 = Hard
   */
  RK_U8 u8Sharpness;
  /*
   * Exposure metering method. '0' means unknown, '1' average,
   * '2' center weighted average, '3'spot, '4' multi-spot,
   * '5' multi-segment, '6' partial, '255' other
   */
  RK_U8 u8MeteringMode;
} ISP_DCF_CONST_INFO_S;

typedef struct rkISP_DCF_UPDATE_INFO_S {
  /* CCD sensitivity equivalent to Ag-Hr film speedrate*/
  RK_U32 u32ISOSpeedRatings;
  /* Exposure time (reciprocal of shutter speed).*/
  RK_U32 u32ExposureTime;
  /* Exposure bias (compensation) value of taking picture*/
  RK_U32 u32ExposureBiasValue;
  /*
   * Exposure program that the camera used when image was taken.
   * '1' means manual control, '2' program normal, '3' aperture priority,
   * '4' shutter priority, '5' program creative (slow program),
   * '6' program action(high-speed program), '7' portrait mode,
   * '8' landscape mode
   */
  RK_U8 u8ExposureProgram;
  /* The actual F-number (F-stop) of lens when the image was taken*/
  RK_U32 u32FNumber;
  /* Maximum aperture value of lens.*/
  RK_U32 u32MaxApertureValue;
  /*
   * Indicates the exposure mode set when the image was shot.
   * 0 = Auto exposure,1 = Manual exposure, 2 = Auto bracket
   */
  RK_U8 u8ExposureMode;
  /*
   * Indicates the white balance mode set when the image was shot.
   * 0 = Auto white balance ,1 = Manual white balance
   */
  RK_U8 u8WhiteBalance;
} ISP_DCF_UPDATE_INFO_S;

typedef struct rkISP_DCF_INFO_S {
  ISP_DCF_CONST_INFO_S stIspDCFConstInfo;
  ISP_DCF_UPDATE_INFO_S stIspDCFUpdateInfo;
} ISP_DCF_INFO_S;

typedef struct rkJPEG_DCF_S {
  /* The date and time when the picture data was generated*/
  RK_U8 au8CaptureTime[DCF_CAPTURE_TIME_LENGTH];
  /* whether the picture is captured when a flash lamp is on*/
  RK_BOOL bFlash;
  /*
   * Indicates the digital zoom ratio when the image was shot.
   * If the numerator of the recorded value is 0, this indicates
   * that digital zoom was not used.
   */
  RK_U32 u32DigitalZoomRatio;
  ISP_DCF_INFO_S stIspDCFInfo;
} JPEG_DCF_S;

typedef struct rkISP_FRAME_INFO_S {
  RK_U32 u32ISO;          /* ISP internal ISO : Again*Dgain*ISPgain */
  RK_U32 u32ExposureTime; /* Exposure time (reciprocal of shutter speed),unit is
                             us */
  RK_U32 u32IspDgain;
  RK_U32 u32Again;
  RK_U32 u32Dgain;
  RK_U32 au32Ratio[3];
  RK_U32 u32IspNrStrength;
  RK_U32 u32FNumber;  /* The actual F-number (F-stop) of lens when the image was
                         taken */
  RK_U32 u32SensorID; /* which sensor is used */
  RK_U32 u32SensorMode;
  RK_U32 u32HmaxTimes; /* Sensor HmaxTimes,unit is ns */
  RK_U32 u32Vmax;      /* Sensor Vmax,unit is line */
  RK_U32 u32VcNum;     /* when dump wdr frame, which is long or short  exposure
                          frame. */
} ISP_FRAME_INFO_S;

#define CFACOLORPLANE (3)
#define DNG_NP_SIZE (6)

typedef struct rkDNG_RATIONAL_S {
  RK_U32 u32Numerator;   /*represents the numerator of a fraction,*/
  RK_U32 u32Denominator; /* the denominator. */
} DNG_RATIONAL_S;

/*
Defines the structure of DNG image dynamic infomation
*/
typedef struct rkDNG_IMAGE_DYNAMIC_INFO_S {
  /* RO;Range: [0x0, 0xFFFF]; black level*/
  RK_U32 au32BlackLevel[ISP_BAYER_CHN];
  /*
   * specifies the selected white balance at time of capture,
   * encoded as the coordinates of a perfectly neutral
   * color in linear reference space values.
   */
  DNG_RATIONAL_S astAsShotNeutral[CFACOLORPLANE];
  /* RO;describes the amount of noise in a raw image */
  RK_DOUBLE adNoiseProfile[DNG_NP_SIZE];
} DNG_IMAGE_DYNAMIC_INFO_S;

typedef struct rkISP_HDR_INFO_S {
  RK_U32 u32ColorTemp;
  RK_U16 au16CCM[9];
  RK_U8 u8Saturation;
} ISP_HDR_INFO_S;

typedef struct rkISP_ATTACH_INFO_S {
  ISP_HDR_INFO_S stIspHdr;
  RK_U32 u32ISO;
  RK_U8 u8SnsWDRMode;
} ISP_ATTACH_INFO_S;

typedef enum rkFRAME_FLAG_E {
  FRAME_FLAG_SNAP_FLASH = 0x1 << 0,
  FRAME_FLAG_SNAP_CUR = 0x1 << 1,
  FRAME_FLAG_SNAP_REF = 0x1 << 2,
  FRAME_FLAG_SNAP_END = 0x1 << 31,
  FRAME_FLAG_BUTT
} FRAME_FLAG_E;

/* RGGB=4 */
#define ISP_WB_GAIN_NUM 4
/* 3*3=9 matrix */
#define ISP_CAP_CCM_NUM 9

typedef struct rkISP_CONFIG_INFO_S {
  RK_U32 u32ISO;
  RK_U32 u32IspDgain;
  RK_U32 u32ExposureTime;
  RK_U32 au32WhiteBalanceGain[ISP_WB_GAIN_NUM];
  RK_U32 u32ColorTemperature;
  RK_U16 au16CapCCM[ISP_CAP_CCM_NUM];
} ISP_CONFIG_INFO_S;

typedef struct rkVIDEO_SUPPLEMENT_S {
  RK_U64 u64JpegDCFPhyAddr;
  RK_U64 u64IspInfoPhyAddr;
  RK_U64 u64LowDelayPhyAddr;
  RK_U64 u64MotionDataPhyAddr;
  RK_U64 u64FrameDNGPhyAddr;

  RK_VOID *ATTRIBUTE pJpegDCFVirAddr;    /* JPEG_DCF_S, used in JPEG DCF */
  RK_VOID *ATTRIBUTE pIspInfoVirAddr;    /* ISP_FRAME_INFO_S, used in ISP debug,
                                            when get raw and send raw */
  RK_VOID *ATTRIBUTE pLowDelayVirAddr;   /* used in low delay */
  RK_VOID *ATTRIBUTE pMotionDataVirAddr; /* vpss 3dnr use: gme motion data,
                                            Filter motion data, gyro data.*/
  RK_VOID *ATTRIBUTE pFrameDNGVirAddr;
} VIDEO_SUPPLEMENT_S;

typedef enum rkCOLOR_GAMUT_E {
  COLOR_GAMUT_BT601 = 0,
  COLOR_GAMUT_BT709,
  COLOR_GAMUT_BT2020,
  COLOR_GAMUT_USER,
  COLOR_GAMUT_BUTT
} COLOR_GAMUT_E;

typedef struct rkISP_COLORGAMMUT_INFO_S {
  COLOR_GAMUT_E enColorGamut;
} ISP_COLORGAMMUT_INFO_S;

typedef enum rkDYNAMIC_RANGE_E {
  DYNAMIC_RANGE_SDR8 = 0,
  DYNAMIC_RANGE_SDR10,
  DYNAMIC_RANGE_HDR10,
  DYNAMIC_RANGE_HLG,
  DYNAMIC_RANGE_SLF,
  DYNAMIC_RANGE_XDR,
  DYNAMIC_RANGE_BUTT
} DYNAMIC_RANGE_E;

typedef enum rkDATA_BITWIDTH_E {
  DATA_BITWIDTH_8 = 0,
  DATA_BITWIDTH_10,
  DATA_BITWIDTH_12,
  DATA_BITWIDTH_14,
  DATA_BITWIDTH_16,
  DATA_BITWIDTH_BUTT
} DATA_BITWIDTH_E;

typedef struct rkVIDEO_FRAME_S {
  MB_BLK pMbBlk;
  RK_U32 u32Width;
  RK_U32 u32Height;
  RK_U32 u32VirWidth;
  RK_U32 u32VirHeight;
  VIDEO_FIELD_E enField;
  PIXEL_FORMAT_E enPixelFormat;
  VIDEO_FORMAT_E enVideoFormat;
  COMPRESS_MODE_E enCompressMode;
  DYNAMIC_RANGE_E enDynamicRange;
  COLOR_GAMUT_E enColorGamut;

  RK_VOID *pVirAddr[RK_MAX_COLOR_COMPONENT];

  RK_U32 u32TimeRef;
  RK_U64 u64PTS;

  RK_U64 u64PrivateData;
  RK_U32 u32FrameFlag; /* FRAME_FLAG_E, can be OR operation. */
} VIDEO_FRAME_S;

typedef struct rkVIDEO_FRAME_INFO_S {
  VIDEO_FRAME_S stVFrame;
} VIDEO_FRAME_INFO_S;

typedef struct rkMB_PIC_CAL_S {
  RK_U32 u32MBSize;
  RK_U32 u32VirWidth;
  RK_U32 u32VirHeight;
} MB_PIC_CAL_S;

typedef struct rkPIC_BUF_ATTR_S {
  RK_U32 u32Width;
  RK_U32 u32Height;
  PIXEL_FORMAT_E enPixelFormat;
  COMPRESS_MODE_E enCompMode;
} PIC_BUF_ATTR_S;

typedef struct rkVDEC_PIC_BUF_ATTR_S {
  RK_CODEC_ID_E enCodecType;
  PIC_BUF_ATTR_S stPicBufAttr;
} VDEC_PIC_BUF_ATTR_S;

typedef struct rkBITMAP_S {
  PIXEL_FORMAT_E enPixelFormat; /* Bitmap's pixel format */
  RK_U32 u32Width;              /* Bitmap's width */
  RK_U32 u32Height;             /* Bitmap's height */
  RK_VOID *ATTRIBUTE pData;     /* Address of Bitmap's data */
} BITMAP_S;

typedef struct rkLDC_ATTR_S {
  /* RW;Range: [0, 1];Whether aspect ration  is keep */
  RK_BOOL bAspect;
  /* RW; Range: [0, 100]; field angle ration of  horizontal,valid when
   * bAspect=0.*/
  RK_S32 s32XRatio;
  /* RW; Range: [0, 100]; field angle ration of  vertical,valid when
   * bAspect=0.*/
  RK_S32 s32YRatio;
  /* RW; Range: [0, 100]; field angle ration of  all,valid when bAspect=1.*/
  RK_S32 s32XYRatio;
  /* RW; Range: [-511, 511]; horizontal offset of the image distortion center
   * relative to image center.*/
  RK_S32 s32CenterXOffset;
  /* RW; Range: [-511, 511]; vertical offset of the image distortion center
   * relative to image center.*/
  RK_S32 s32CenterYOffset;
  /* RW; Range: [-300, 500]; LDC Distortion ratio.When spread
   * on,s32DistortionRatio range should be [0, 500]*/
  RK_S32 s32DistortionRatio;
} LDC_ATTR_S;

typedef struct rkLDCV2_ATTR_S {
  /* RW; focal length in horizontal direction, with 2 decimal numbers */
  RK_S32 s32FocalLenX;
  /* RW; focal length in vertical direction, with 2 decimal numbers */
  RK_S32 s32FocalLenY;
  /* RW; coordinate of image center, with 2 decimal numbers */
  RK_S32 s32CoorShiftX;
  /* RW; Y coordinate of image center, with 2 decimal numbers */
  RK_S32 s32CoorShiftY;
  /* RW; lens distortion coefficients of the source image, with 5 decimal
   * numbers */
  RK_S32 as32SrcCaliRatio[SRC_LENS_COEF_SEG][SRC_LENS_COEF_NUM];
  /* RW; Junction Point of the two segments */
  RK_S32 s32SrcJunPt;
  /* RW; lens distortion coefficients, with 5 decimal numbers */
  RK_S32 as32DstCaliRatio[DST_LENS_COEF_SEG][DST_LENS_COEF_NUM];
  /* RW; Junction Point of the three segments */
  RK_S32 as32DstJunPt[DST_LENS_COEF_SEG_POINT];
  /* RW; max undistorted distance before 3rd polynomial drop, with 16bits
   * decimal */
  RK_S32 s32MaxDu;
} LDCV2_ATTR_S;

typedef enum rkLDC_VIEW_TYPE_E {
  /*
   * View scale all but x and y independtly, this will keep
   * both x and y axis ,but corner maybe lost
   */
  LDC_VIEW_TYPE_ALL = 0,
  /* Not use view scale, this will lost some side and corner */
  LDC_VIEW_TYPE_CROP = 1,

  LDC_VIEW_TYPE_BUTT,
} LDC_VIEW_TYPE_E;

typedef struct rkLDCV3_ATTR_S {
  /* RW; Range: [0, 1], 0: all mode, 1: crop mode.*/
  LDC_VIEW_TYPE_E enViewType;
  /*
   * RW; Range: ABS(s32CenterXOffset)->[0, min(128, width*0.08)],
   * horizontal offset of the image distortion center relative to image center.
   */
  RK_S32 s32CenterXOffset;
  /*
   * RW; Range: ABS(s32CenterXOffset)->[0, min(128, height*0.08)],
   * vertical offset of the image distortion center relative to image center.
   */
  RK_S32 s32CenterYOffset;
  /* RW; Range: [-300, 300], LDC Distortion ratio.*/
  RK_S32 s32DistortionRatio;
  /*
   * RW; Range: [-300, 300], to remove the black region around when
   * performing pinchusion distortion correction with crop mode.
   */
  RK_S32 s32MinRatio;
} LDCV3_ATTR_S;

typedef enum rkROTATION_VIEW_TYPE_E {
  ROTATION_VIEW_TYPE_ALL = 0, /* View all source Image,no lose*/
  ROTATION_VIEW_TYPE_TYPICAL =
      1, /* View from rotation Image with source size,same lose*/
  ROTATION_VIEW_TYPE_INSIDE =
      2, /* View with no black section,all  in dest Image*/
  ROTATION_VIEW_TYPE_BUTT,
} ROTATION_VIEW_TYPE_E;

typedef struct rkROTATION_EX_S {
  /* RW;Range: [0, 2];Rotation mode*/
  ROTATION_VIEW_TYPE_E enViewType;
  /* RW;Range: [0,360];Rotation Angle:[0,360]*/
  RK_U32 u32Angle;
  /*
   * RW;Range: [-511, 511];Horizontal offset of the image
   * distortion center relative to image center.
   */
  RK_S32 s32CenterXOffset;
  /*
   * RW;Range: [-511, 511];Vertical offset of the image
   * distortion center relative to image center.
   */
  RK_S32 s32CenterYOffset;
  /* RW;Dest size of any angle rotation*/
  SIZE_S stDestSize;
} ROTATION_EX_S;

typedef enum rkWDR_MODE_E {
  WDR_MODE_NONE = 0,
  WDR_MODE_BUILT_IN,
  WDR_MODE_QUDRA,

  WDR_MODE_2To1_LINE,
  WDR_MODE_2To1_FRAME,
  WDR_MODE_2To1_FRAME_FULL_RATE,

  WDR_MODE_3To1_LINE,
  WDR_MODE_3To1_FRAME,
  WDR_MODE_3To1_FRAME_FULL_RATE,

  WDR_MODE_4To1_LINE,
  WDR_MODE_4To1_FRAME,
  WDR_MODE_4To1_FRAME_FULL_RATE,

  WDR_MODE_BUTT,
} WDR_MODE_E;

typedef enum rkFRAME_INTERRUPT_TYPE_E {
  FRAME_INTERRUPT_START,
  FRAME_INTERRUPT_EARLY,
  FRAME_INTERRUPT_EARLY_END,
  FRAME_INTERRUPT_BUTT,
} FRAME_INTERRUPT_TYPE_E;

typedef struct rkFRAME_INTERRUPT_ATTR_S {
  FRAME_INTERRUPT_TYPE_E enIntType;
  RK_U32 u32EarlyLine;
} FRAME_INTERRUPT_ATTR_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* INCLUDE_RT_MPI_RK_COMMON_VIDEO_H_ */
