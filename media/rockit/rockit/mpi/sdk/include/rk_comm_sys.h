/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_RK_COMMON_SYS_H_
#define INCLUDE_RT_MPI_RK_COMMON_SYS_H_

#include "rk_errno.h"
#include "rk_type.h"
// #include "rk_debug.h"
#include "rk_comm_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define BIND_DEST_MAXNUM 64

#define RK_TRACE_SYS(level, fmt, ...)                                          \
  do {                                                                         \
    RK_TRACE(level, RK_ID_SYS, "[Func]:%s [Line]:%d [Info]:" fmt,              \
             __FUNCTION__, __LINE__, ##__VA_ARGS__);                           \
  } while (0)

typedef struct rkVPSS_VENC_WRAP_PARAM_S {
  RK_BOOL bAllOnline;
  /*
   * Input frame rate of VIPROC(linear/Frame mode WDR is the
   * sensor frame rate, and the line mode WDR is the frame
   * rate after synthesis)
   */
  RK_U32 u32FrameRate;
  RK_U32 u32FullLinesStd; /* Total height of sensor, include of VBlank */

  SIZE_S stLargeStreamSize; /* VENC Large Stream Size */
  SIZE_S stSmallStreamSize; /* VENC Small Stream Size */
} VPSS_VENC_WRAP_PARAM_S;

typedef struct rkMPP_SYS_CONFIG_S {
  RK_U32 u32Align;
} MPP_SYS_CONFIG_S;

typedef struct rkSYS_VIRMEM_INFO_S {
  RK_U64 u64PhyAddr;
  RK_BOOL bCached;
} SYS_VIRMEM_INFO_S;

typedef struct rkMPP_BIND_DEST_S {
  RK_U32 u32Num;
  MPP_CHN_S astMppChn[BIND_DEST_MAXNUM];
} MPP_BIND_DEST_S;

typedef enum rkSCALE_RANGE_E {
  SCALE_RANGE_0 = 0, /* scale range <   8/64 */
  SCALE_RANGE_1,     /* scale range >=  8/64 */
  SCALE_RANGE_2,     /* scale range >= 10/64 */
  SCALE_RANGE_3,     /* scale range >= 15/64 */
  SCALE_RANGE_4,     /* scale range >= 19/64 */
  SCALE_RANGE_5,     /* scale range >= 24/64 */
  SCALE_RANGE_6,     /* scale range >= 29/64 */
  SCALE_RANGE_7,     /* scale range >= 33/64 */
  SCALE_RANGE_8,     /* scale range >= 35/64 */
  SCALE_RANGE_9,     /* scale range >= 38/64 */
  SCALE_RANGE_10,    /* scale range >= 42/64 */
  SCALE_RANGE_11,    /* scale range >= 45/64 */
  SCALE_RANGE_12,    /* scale range >= 48/64 */
  SCALE_RANGE_13,    /* scale range >= 51/64 */
  SCALE_RANGE_14,    /* scale range >= 53/64 */
  SCALE_RANGE_15,    /* scale range >= 55/64 */
  SCALE_RANGE_16,    /* scale range >= 57/64 */
  SCALE_RANGE_17,    /* scale range >= 60/64 */
  SCALE_RANGE_18,    /* scale range >  1     */
  SCALE_RANGE_BUTT,
} SCALE_RANGE_E;

typedef enum rkCOEFF_LEVEL_E {
  COEFF_LEVEL_0 = 0, /* coefficient level 0 */
  COEFF_LEVEL_1,     /* coefficient level 1 */
  COEFF_LEVEL_2,     /* coefficient level 2 */
  COEFF_LEVEL_3,     /* coefficient level 3 */
  COEFF_LEVEL_4,     /* coefficient level 4 */
  COEFF_LEVEL_5,     /* coefficient level 5 */
  COEFF_LEVEL_6,     /* coefficient level 6 */
  COEFF_LEVEL_7,     /* coefficient level 7 */
  COEFF_LEVEL_8,     /* coefficient level 8 */
  COEFF_LEVEL_9,     /* coefficient level 9 */
  COEFF_LEVEL_10,    /* coefficient level 10 */
  COEFF_LEVEL_11,    /* coefficient level 11 */
  COEFF_LEVEL_12,    /* coefficient level 12 */
  COEFF_LEVEL_13,    /* coefficient level 13 */
  COEFF_LEVEL_14,    /* coefficient level 14 */
  COEFF_LEVEL_15,    /* coefficient level 15 */
  COEFF_LEVEL_16,    /* coefficient level 16 */
  COEFF_LEVEL_17,    /* coefficient level 17 */
  COEFF_LEVEL_18,    /* coefficient level 18 */
  COEFF_LEVEL_BUTT,
} COEFF_LEVEL_E;

typedef struct rkSCALE_COEFF_LEVEL_S {
  COEFF_LEVEL_E enHorLum; /* horizontal luminance   coefficient level */
  COEFF_LEVEL_E enHorChr; /* horizontal chrominance coefficient level */
  COEFF_LEVEL_E enVerLum; /* vertical   luminance   coefficient level */
  COEFF_LEVEL_E enVerChr; /* vertical   chrominance coefficient level */
} SCALE_COEFF_LEVEL_S;

typedef struct rkSCALE_RANGE_S {
  SCALE_RANGE_E enHorizontal;
  SCALE_RANGE_E enVertical;
} SCALE_RANGE_S;

typedef struct rkSCALE_COEFF_INFO_S {
  SCALE_RANGE_S stScaleRange;
  SCALE_COEFF_LEVEL_S stScaleCoeffLevel;
} SCALE_COEFF_INFO_S;

typedef struct rkGPS_INFO_S {
  /*
   * GPS LatitudeRef Indicates whether the latitude
   * is north or south latitude,'N'/'S', default 'N'
   */
  RK_CHAR chGPSLatitudeRef;
  /*
   * GPS Latitude is expressed as degrees, minutes and
   * seconds,a typical format like "dd/1,mm/1,ss/1",
   * default 0/0, 0/0, 0/0
   */
  RK_U32 au32GPSLatitude[3][2];
  /*
   * GPS LongitudeRef Indicates whether the longitude
   * is east or west longitude, 'E'/'W', default 'E'
   */
  RK_CHAR chGPSLongitudeRef;
  /*
   * GPS Longitude is expressed as degrees, minutes
   * and seconds,a typical format like "dd/1,mm/1,ss/1",
   * default 0/0, 0/0, 0/0
   */
  RK_U32 au32GPSLongitude[3][2];
  /*
   * GPS AltitudeRef Indicates the reference altitude
   * used, 0 - above sea level, 1 - below sea level
   * default 0
   */
  RK_U8 u8GPSAltitudeRef;
  /*
   * GPS AltitudeRef Indicates the altitude based on
   * the reference u8GPSAltitudeRef, the reference unit
   * is meters, default 0/0
   */
  RK_U32 au32GPSAltitude[2];
} GPS_INFO_S;

typedef enum rkCHN_INPUT_MODE_E {
  CHN_INPUT_MODE_NORMAL,        /* CHN receive all packet */
  CHN_INPUT_MODE_REMAIN_NEWEST, /* CHN remain newest packet */
  CHN_INPUT_MODE_DROP_ALWAYS,   /* CHN drop all packet */
  CHN_INPUT_MODE_BUTT
} CHN_INPUT_MODE_E;

typedef struct rkRAW_FRAME_COMPRESS_PARAM_S {
  RK_U32 u32CompRatio_10Bit; /* compression ratio of 10 bit */
  RK_U32 u32CompRatio_12Bit; /* compression ratio of 12 bit */
  RK_U32 u32CompRatio_14Bit; /* compression ratio of 14 bit */
} RAW_FRAME_COMPRESS_PARAM_S;

#define RK_ERR_SYS_NULL_PTR                                                    \
  RK_DEF_ERR(RK_ID_SYS, RK_ERR_LEVEL_ERROR, RK_ERR_NULL_PTR)
#define RK_ERR_SYS_NOTREADY                                                    \
  RK_DEF_ERR(RK_ID_SYS, RK_ERR_LEVEL_ERROR, RK_ERR_NOTREADY)
#define RK_ERR_SYS_NOT_PERM                                                    \
  RK_DEF_ERR(RK_ID_SYS, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_PERM)
#define RK_ERR_SYS_NOMEM RK_DEF_ERR(RK_ID_SYS, RK_ERR_LEVEL_ERROR, RK_ERR_NOMEM)
#define RK_ERR_SYS_ILLEGAL_PARAM                                               \
  RK_DEF_ERR(RK_ID_SYS, RK_ERR_LEVEL_ERROR, RK_ERR_ILLEGAL_PARAM)
#define RK_ERR_SYS_BUSY RK_DEF_ERR(RK_ID_SYS, RK_ERR_LEVEL_ERROR, RK_ERR_BUSY)
#define RK_ERR_SYS_NOT_SUPPORT                                                 \
  RK_DEF_ERR(RK_ID_SYS, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_SUPPORT)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* INCLUDE_RT_MPI_RK_COMMON_SYS_H_ */
