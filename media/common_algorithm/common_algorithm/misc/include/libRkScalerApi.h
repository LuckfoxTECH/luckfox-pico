//
/////////////////////////////////////////////////////////////////////////
// Copyright(c) Rock-chip, All right reserved.
//
// This file is Rock-chip's property. It contains Rock-chip's trade secret,
// proprietary and confidential information.
// The information and code contained in this file is only for authorized
// Rock-chip employees to design, create, modify, or review.
// DO NOT DISTRIBUTE, DO NOT DUPLICATE OR TRANSMIT IN ANY FORM WITHOUT
// PROPER AUTHORIZATION.
// If you are not an intended recipient of this file, you must not copy,
// distribute, modify, or take any action in reliance on it.
// If you have received this file in error, please immediately notify
// Rock-chip and permanently delete the original and any copy of any file
// and any printout thereof.
//
//////////////////////////////////////////////////////////////////////////
//
// Last update 20230608


#ifndef __LIB_RK_SCALER_API_H_
#define __LIB_RK_SCALER_API_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define RKSCALER_MAX_YUV_PLANE_NUM  3   // max plane number is 3 for YUV Semi-Plane
#define RKSCALER_MAX_CORE_NUM       16  // max core number for multi-thread

//////////////////////////////////////////////////////////////////////////
//---- DataType
typedef     signed char             RK_S8;          // s8bit
typedef     unsigned char           RK_U8;          // u8bit
typedef     short                   RK_S16;         // s16bit
typedef     unsigned short          RK_U16;         // u16bit
typedef     int                     RK_S32;         // s32bit
typedef     unsigned int            RK_U32;         // u32bit
typedef     unsigned long long      RK_U64;         // u64bit
typedef     long long               RK_S64;         // s64bit
typedef     float                   RK_F32;         // f32bit
typedef     double                  RK_D64;         // d64bit

//////////////////////////////////////////////////////////////////////////
//---- ScalerFormat_e
enum ScalerFormat_e
{
    SCALER_FMT_YUV420P  = 0, // YUV420P
    SCALER_FMT_YUV422P  = 1, // YUV422P
    SCALER_FMT_YUV440P  = 2, // YUV440P
    SCALER_FMT_YUV444P  = 3, // YUV444P
    SCALER_FMT_YUV420SP = 4, // YUV420SP
    SCALER_FMT_YUV422SP = 5, // YUV422SP
    SCALER_FMT_YUV440SP = 6, // YUV440SP
    SCALER_FMT_YUV444SP = 7, // YUV444SP
    SCALER_FMT_MAX
};

//---- interpolation method
enum ScalerMethod_e
{
    SCALER_METHOD_NEAREST  = 0, // nearest interpolation
    SCALER_METHOD_BILINEAR = 1, // bilinear interpolation
    SCALER_METHOD_MAX
};

//////////////////////////////////////////////////////////////////////////
//---- RkScalerParams
typedef struct _RkScalerParams {
    RK_U32  nCallCnt;                                 // [i] process call counter, reserved
    RK_U32  nMethodLuma;                              // [i] interpolation method for luma (Y), see ScalerMethod_e
    RK_U32  nMethodChrm;                              // [i] interpolation method for chroma (UV), see ScalerMethod_e
    RK_U32  nCores;                                   // [i] multi-thread cores to use, set equal to the number of cpu cores

    // Src
    RK_U32  nSrcFmt;                                  // [i] src format, see ScalerFormat_e
    RK_U32  nSrcWid;                                  // [i] src width  [pixel]
    RK_U32  nSrcHgt;                                  // [i] src height [pixel]
    RK_U32  nSrcWStrides[RKSCALER_MAX_YUV_PLANE_NUM]; // [i] src width  strides [byte]
    RK_U32  nSrcHStrides[RKSCALER_MAX_YUV_PLANE_NUM]; // [i] src height strides [pixel]
    RK_U8  *pSrcBufs[RKSCALER_MAX_YUV_PLANE_NUM];     // [i] src buffer pointers of each plane

    // Dst
    RK_U32  nDstFmt;                                  // [i] dst format, see ScalerFormat_e
    RK_U32  nDstWid;                                  // [i] dst width  [pixel]
    RK_U32  nDstHgt;                                  // [i] dst height [pixel]
    RK_U32  nDstWStrides[RKSCALER_MAX_YUV_PLANE_NUM]; // [i] dst width  strides [byte]
    RK_U32  nDstHStrides[RKSCALER_MAX_YUV_PLANE_NUM]; // [i] dst height strides [pixel]
    RK_U8  *pDstBufs[RKSCALER_MAX_YUV_PLANE_NUM];     // [i] dst buffer pointers of each plane
} RkScalerParams;

//////////////////////////////////////////////////////////////////////////

typedef void* RkScalerContext;

/**
 * @brief  make multi-thread resource initialized
 * @param  [int] cores - number of threads to use when processing
 * @return [int] error code, 0 indicates everything is ok
 */
int RkScalerInit(RkScalerContext *pCtx, int cores);

/**
 * @brief  run the scaling prcess
 * @param  [RkScalerParams*] pScalerParams - the processing parameters
 * @return [int] error code, 0 indicates everything is ok
 */
int RkScalerProcessor(RkScalerContext ctx, RkScalerParams *pScalerParams);

/**
 * @brief  release the multi-thread resource
 * @return [int] error code, 0 indicates everything is ok
 */
int RkScalerDeinit(RkScalerContext ctx);

/**
 * @brief  set the log level (TODO)
 * @param  [RkScalerContext] ctx
 * @param  [int] logLevel
 * @return [int] error code, 0 indicates everything is ok
 */
int RkScalerSetLoglevel(RkScalerContext ctx, int logLevel);

#ifdef __cplusplus
}
#endif

#endif // __LIB_RK_SCALER_API_H_
