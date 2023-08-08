/*
 * Copyright 2020 Rockchip Electronics Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#ifndef _RK_IVE_H_
#define _RK_IVE_H_

#if __cplusplus
extern "C" {
#endif

#include "rk_comm_ive.h"
#include "rk_common.h"

#define IVE_ALIGN(size, align) ((size + align - 1) & (~(align - 1)))
#define IVE_ALIGN_DOWN(size, align) (size / align * align)

#define MAX_IMAGE_WIDTH 4096
#define IVE_RKST_NUM 256
#define IVE_HIST_NUM 256
#define IVE_MAP_NUM 256
#define IVE_MAX_REGION_NUM 254
#define IVE_ST_MAX_CORNER_NUM 500
#define LK_OPTICAL_FLOW_MAX_POINT_NUM 500
#define IVE_PYRAMID_MAX_LEVEL 8

/*
 * Definition of the union of IVE_8BIT_U.
 */
typedef struct rkIVE_8BIT_U {
    char s8Val;
    RK_U8 u8Val;
} IVE_8BIT_U;

/*
 *Region struct
 */
typedef struct rkIVE_REGION_S {
    RK_U32 u32Area;   /*Represented by the pixel number*/
    RK_U16 u16Left;   /*Circumscribed rectangle left border*/
    RK_U16 u16Right;  /*Circumscribed rectangle right border*/
    RK_U16 u16Top;    /*Circumscribed rectangle top border*/
    RK_U16 u16Bottom; /*Circumscribed rectangle bottom border*/
} IVE_REGION_S;

/*
 * LKOpticalFlowPyr output mode
 */
typedef enum rkIVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_E {
    IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_NONE = 0,   /*Output none*/
    IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_STATUS = 1, /*Output status*/
    IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_BOTH = 2,   /*Output status and err*/
    IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_BUTT
} IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_E;
/*
 * LKOpticalFlowPyr control parameters
 */
typedef struct rkIVE_LK_OPTICAL_FLOW_PYR_CTRL_S {
    IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_E enOutMode;
    RK_BOOL bUseInitFlow;   /*where to use initial flow*/
    RK_U16 u16PtsNum;          /*Number of the feature points,<=500*/
    RK_U8 u8MaxLevel;          /*0<=u8MaxLevel<=3*/
    IVE_U0Q8 u0q8MinEigThr; /*Minimum eigenvalue threshold*/
    RK_U8 u8IterCnt;           /*Maximum iteration times, <=20*/
    IVE_U0Q11 u0q11Eps;     /*Used for exit criteria: dx^2 + dy^2 < u0q11Eps */
} IVE_LK_OPTICAL_FLOW_PYR_CTRL_S;

/*
 * LKOpticalFlow control parameters
 */
typedef struct rkIVE_LK_OPTICAL_FLOW_CTRL_S {
    RK_U16 u16PtsNum; /*Number of the feature points,<=500*/
    IVE_U0Q8 u0q8MinEigThr; /*Minimum eigenvalue threshold*/
    RK_U8 u8IterCnt;           /*Maximum iteration times, <=20*/
    IVE_U0Q11 u0q11Eps;     /*Used for exit criteria: dx^2 + dy^2 < u0q11Eps */
} IVE_LK_OPTICAL_FLOW_CTRL_S;

/*
 *DMA mode
 */
typedef enum rkIVE_DMA_MODE_E {
    IVE_DMA_MODE_DIRECT_COPY = 0x0,
    IVE_DMA_MODE_INTERVAL_COPY = 0x1,
    IVE_DMA_MODE_SET_3BYTE = 0x2,
    IVE_DMA_MODE_SET_8BYTE = 0x3,
    IVE_DMA_MODE_BUTT
} IVE_DMA_MODE_E;

/*
 *DMA control parameter
 */
typedef struct rkIVE_DMA_CTRL_S {
    IVE_DMA_MODE_E enMode;
    RK_U64 u64Val;      /*Used in memset mode*/
    RK_U8 u8HorSegSize; /*Used in interval-copy mode, every row was segmented by
                        u8HorSegSize bytes, restricted in values of 2,3,4,8,16*/
    RK_U8 u8ElemSize; /*Used in interval-copy mode, the valid bytes copied in front
                      of every segment in a valid row, which
                      0<u8ElemSize<u8HorSegSize*/
    RK_U8 u8VerSegRows; /*Used in interval-copy mode, copy one row in every
                        u8VerSegRows*/
} IVE_DMA_CTRL_S;

/*
 * SOBEL in/out image format.
 */
typedef enum rkIVE_SOBEL_IMAGE_TYPE_E {
    IVE_SOBEL_IMAGE_TYPE_U8C1 = 0x0,
    IVE_SOBEL_IMAGE_TYPE_S8C1 = 0x1,
    IVE_SOBEL_IMAGE_TYPE_U16C1 = 0x2,
    IVE_SOBEL_IMAGE_TYPE_S16C1 = 0x3,

    IVE_SOBEL_IMAGE_TYPE_BUTT

} IVE_SOBEL_IMAGE_TYPE_E;

/*
 *SOBEL output ctrl
 */
typedef enum rkIVE_SOBEL_OUT_CTRL_E {
    IVE_SOBEL_OUT_CTRL_BOTH = 0x0, /*Output horizontal and vertical*/
    IVE_SOBEL_OUT_CTRL_HOR = 0x1,  /*Output horizontal*/
    IVE_SOBEL_OUT_CTRL_VER = 0x2,  /*Output vertical*/
    IVE_SOBEL_OUT_CTRL_BUTT
} IVE_SOBEL_OUT_CTRL_E;

/*
 * FILTER in/out image format.
 */
typedef enum rkIVE_FILTER_IMAGE_TYPE_E {
    IVE_FILTER_IMAGE_TYPE_U8C1 = 0x0,
    IVE_FILTER_IMAGE_TYPE_S8C1 = 0x1,
    IVE_FILTER_IMAGE_TYPE_U16C1 = 0x2,
    IVE_FILTER_IMAGE_TYPE_S16C1 = 0x3,

    IVE_FILTER_IMAGE_TYPE_BUTT

} IVE_FILTER_IMAGE_TYPE_E;
/*
 *Filter control parameters
 *You need to set these parameters when using the filter operator.
 *Modify template filter from 3x3 to 5x5
 */
typedef struct rkIVE_FILTER_CTRL_S {
    RK_U8 u8Norm;      /*Normalization parameter, by right shift*/
    RK_U8 u8OutMode;   /* 0:RK_U8   1:RK_S8;  2:RK_U16;  3:RK_S16;*/
    RK_S8 as8Mask[25]; /*Template parameter filter coefficient*/
} IVE_FILTER_CTRL_S;

/*
 *Sobel control parameters
 *You need to set these parameters when using the sobel operator.
 *Modify template filter from 3x3 to 5x5
 */
typedef struct rkIVE_SOBEL_CTRL_S {
    RK_U8 u8OutCtrl;   /* 0:hv 1:h 2:v*/
    RK_U8 u8Norm;      /*Normalization parameter, by right shift*/
    RK_U8 u8OutMode;   /* 0:RK_U8   1:RK_S8;  2:RK_U16;  3:RK_S16;*/
    RK_S8 as8Mask[25]; /*Template parameter filter coefficient*/
} IVE_SOBEL_CTRL_S;

/*
 * CSC in/out image format.
 */
typedef enum rkIVE_CSC_IMAGE_TYPE_E {
    IVE_CSC_IMAGE_TYPE_YUV420SP = 0x0, /*YUV420 SemiPlanar*/
    IVE_CSC_IMAGE_TYPE_YUV422SP = 0x1, /*YUV422 SemiPlanar*/

    IVE_CSC_IMAGE_TYPE_U8C3_PACKAGE = 0x2,
    IVE_CSC_IMAGE_TYPE_U8C3_PLANAR = 0x3,

    IVE_CSC_IMAGE_TYPE_BUTT

} IVE_CSC_IMAGE_TYPE_E;

/*
 * CSC working mode.
 */
typedef enum rkIVE_CSC_MODE_E {
    IVE_CSC_MODE_LIMIT_BT601_YUV2RGB =
        0x00, /*CSC: YUV2RGB, video transfer mode, RGB value range [16, 235]*/
    IVE_CSC_MODE_LIMIT_BT709_YUV2RGB =
        0x01, /*CSC: YUV2RGB, video transfer mode, RGB value range [16, 235]*/
    IVE_CSC_MODE_FULL_BT601_YUV2RGB =
        0x02, /*CSC: YUV2RGB, picture transfer mode, RGB value range [0, 255]*/
    IVE_CSC_MODE_FULL_BT709_YUV2RGB =
        0x03, /*CSC: YUV2RGB, picture transfer mode, RGB value range [0, 255]*/

    IVE_CSC_MODE_LIMIT_BT601_YUV2HSV =
        0x10, /*CSC: YUV2HSV, picture transfer mode, HSV value range [16, 240]*/
    IVE_CSC_MODE_LIMIT_BT709_YUV2HSV =
        0x11, /*CSC: YUV2HSV, picture transfer mode, HSV value range [16, 240]*/
    IVE_CSC_MODE_FULL_BT601_YUV2HSV =
        0x12, /*CSC: YUV2HSV, picture transfer mode, HSV value range [0, 255]*/
    IVE_CSC_MODE_FULL_BT709_YUV2HSV =
        0x13, /*CSC: YUV2HSV, picture transfer mode, HSV value range [0, 255]*/

    IVE_CSC_MODE_LIMIT_BT601_RGB2YUV =
        0x20, /*CSC: RGB2YUV, picture transfer mode, Y:[16, 235],U\V:[16, 240]*/
    IVE_CSC_MODE_LIMIT_BT709_RGB2YUV =
        0x21, /*CSC: RGB2YUV, picture transfer mode, Y:[16, 235],U\V:[16, 240]*/
    IVE_CSC_MODE_FULL_BT601_RGB2YUV =
        0x22, /*CSC: RGB2YUV, video transfer mode, YUV value range [0, 255]*/
    IVE_CSC_MODE_FULL_BT709_RGB2YUV =
        0x23, /*CSC: RGB2YUV, video transfer mode, YUV value range [0, 255]*/

    IVE_CSC_MODE_LIMIT_BT601_RGB2HSV =
        0x30, /*CSC: YUV2HSV, picture transfer mode, HSV value range [16, 240]*/
    IVE_CSC_MODE_LIMIT_BT709_RGB2HSV =
        0x31, /*CSC: YUV2HSV, picture transfer mode, HSV value range [16, 240]*/
    IVE_CSC_MODE_FULL_BT601_RGB2HSV =
        0x32, /*CSC: YUV2HSV, picture transfer mode, HSV value range [0, 255]*/
    IVE_CSC_MODE_FULL_BT709_RGB2HSV =
        0x33, /*CSC: YUV2HSV, picture transfer mode, HSV value range [0, 255]*/

    IVE_CSC_MODE_BUTT

} IVE_CSC_MODE_E;

/*
 *CSC control parameters
 *You need to set these parameters when using the CSC operator.
 *Removed the CSC output format, replaced by setting the output image type.
 */
typedef struct rkIVE_CSC_CTRL_S {
    IVE_CSC_MODE_E enMode; /*Working mode*/
    RK_U8 u8InDataFmt;
    RK_U8 u8OutDataFmt;
    RK_U8 u8YUV2RGBRange;
    RK_U8 u8RGB2YUVRange;
} IVE_CSC_CTRL_S;

/*
 *Magnitude and angle control parameter
 */
typedef struct rkIVE_MAG_AND_ANG_CTRL_S {
    RK_S32 reserve;
    RK_U16 u16Thr;
    RK_S8 as8Mask[25];
    IVE_MEM_INFO_S stMem;
} IVE_MAG_AND_ANG_CTRL_S;

/*
 *Dilate control parameters
 */
typedef struct rkIVE_DILATE_CTRL_S {
    RK_U8 au8Mask[25]; /*The template parameter value must be 0 or 255.*/
} IVE_DILATE_CTRL_S;

/*
 *Erode control parameter
 */
typedef struct rkIVE_ERODE_CTRL_S {
    RK_U8 au8Mask[25]; /*The template parameter value must be 0 or 255.*/
} IVE_ERODE_CTRL_S;

/*
 *Morphy control parameter
 */
typedef struct rkIVE_MORPH_CTRL_S {
    RK_U8 enMode;      // 0: dilate 1: erode
    RK_U8 au8Mask[25]; /*The template parameter value must be 0 or 255.*/
} IVE_MORPH_CTRL_S;

/*
 * CAST in/out image format.
 */
typedef enum rkIVE_CAST_IMAGE_TYPE_E {
    IVE_CAST_IMAGE_TYPE_U8C1 = 0x0,
    IVE_CAST_IMAGE_TYPE_U16C1 = 0x1,

    IVE_CAST_IMAGE_TYPE_S8C1 = 0x2,
    IVE_CAST_IMAGE_TYPE_S16C1 = 0x3,

    IVE_CAST_IMAGE_TYPE_BUTT

} IVE_CAST_IMAGE_TYPE_E;

/*
 * THRESH in/out image format.
 */
typedef enum rkIVE_THRESH_IMAGE_TYPE_E {
    IVE_THRESH_IMAGE_TYPE_U8C1 = 0x0,
    IVE_THRESH_IMAGE_TYPE_U16C1 = 0x1,

    IVE_THRESH_IMAGE_TYPE_S8C1 = 0x2,
    IVE_THRESH_IMAGE_TYPE_S16C1 = 0x3,

    IVE_THRESH_IMAGE_TYPE_BUTT

} IVE_THRESH_IMAGE_TYPE_E;

/*
 * Thresh control parameters.
 */
typedef struct rkIVE_THRESH_CTRL_S {
    RK_U8 u8I0Mode; /* 0: src value 1: config value*/
    RK_U8 u8I1Mode; /* 0: src value 1: config value*/
    RK_U8 u8I2Mode; /* 0: src value 1: config value*/
    RK_U8 u8InDataType;
    RK_U8 u8OutDataType;
    RK_S32 s32LowThr;  /*user-defined threshold,  0<=u8LowThr<=255 */
    RK_S32 s32HighThr; /*user-defined threshold, if
                       enMode<IVE_THRESH_MODE_MIN_MID_MAX, u8HighThr is not
                       used, else 0<=u8LowThr<=u8HighThr<=255;*/
    RK_S32 s32I0Val;   /*Minimum value when tri-level thresholding*/
    RK_S32 s32I1Val;   /*Middle value when tri-level thresholding, if enMode<2,
                       u32MidVal is not used; */
    RK_S32 s32I2Val;   /*Maxmum value when tri-level thresholding*/
} IVE_THRESH_CTRL_S;

/*
 * Type of the Thresh mode.
 */
typedef enum rkIVE_THRESH_MODE_E {
    IVE_THRESH_MODE_BINARY = 0x0, /*srcVal <= lowThr, dstVal = minVal; srcVal >
                                     lowThr, dstVal = maxVal.*/
    IVE_THRESH_MODE_TRUNC = 0x1,  /*srcVal <= lowThr, dstVal = srcVal; srcVal >
                                     lowThr, dstVal = maxVal.*/
    IVE_THRESH_MODE_TO_MINVAL = 0x2, /*srcVal <= lowThr, dstVal = minVal; srcVal
                                        > lowThr, dstVal = srcVal.*/

    IVE_THRESH_MODE_MIN_MID_MAX =
        0x3, /*srcVal <= lowThr, dstVal = minVal;  lowThr < srcVal <= highThr,
                dstVal = midVal; srcVal > highThr, dstVal = maxVal.*/
    IVE_THRESH_MODE_ORI_MID_MAX =
        0x4, /*srcVal <= lowThr, dstVal = srcVal;  lowThr < srcVal <= highThr,
                dstVal = midVal; srcVal > highThr, dstVal = maxVal.*/
    IVE_THRESH_MODE_MIN_MID_ORI =
        0x5, /*srcVal <= lowThr, dstVal = minVal;  lowThr < srcVal <= highThr,
                dstVal = midVal; srcVal > highThr, dstVal = srcVal.*/
    IVE_THRESH_MODE_MIN_ORI_MAX =
        0x6, /*srcVal <= lowThr, dstVal = minVal;  lowThr < srcVal <= highThr,
                dstVal = srcVal; srcVal > highThr, dstVal = maxVal.*/
    IVE_THRESH_MODE_ORI_MID_ORI =
        0x7, /*srcVal <= lowThr, dstVal = srcVal;  lowThr < srcVal <= highThr,
                dstVal = midVal; srcVal > highThr, dstVal = srcVal.*/

    IVE_THRESH_MODE_BUTT
} IVE_THRESH_MODE_E;

/*
 * Thresh control parameters.
 */
typedef struct rkIVE_THRESH_U8_CTRL_S {
    IVE_THRESH_MODE_E enMode;
    RK_U8 u8LowThr;  /* user-defined threshold,  0<=u8LowThr<=255 */
    RK_U8 u8HighThr; /* user-defined threshold, if
                      enMode<IVE_THRESH_MODE_MIN_MID_MAX, u8HighThr is not used,
                      else 0<=u8LowThr<=u8HighThr<=255; */
    RK_U8 u8MinVal;  /* Minimum value when tri-level thresholding */
    RK_U8 u8MidVal;  /* Middle value when tri-level thresholding, if enMode<2,
                        u32MidVal is not used; */
    RK_U8 u8MaxVal;  /* Maxmum value when tri-level thresholding */
} IVE_THRESH_U8_CTRL_S;

/*
* Type of the Thresh_S16 mode
*/
typedef enum rkIVE_THRESH_S16_MODE_E {
    IVE_THRESH_S16_MODE_S16_TO_S8_MIN_MID_MAX = 0x0,
    IVE_THRESH_S16_MODE_S16_TO_S8_MIN_ORI_MAX = 0x1,
    IVE_THRESH_S16_MODE_S16_TO_U8_MIN_MID_MAX = 0x2,
    IVE_THRESH_S16_MODE_S16_TO_U8_MIN_ORI_MAX = 0x3,

    IVE_THRESH_S16_MODE_BUTT
} IVE_THRESH_S16_MODE_E;

/*
* Thresh_S16 control parameters
*/
typedef struct rkIVE_THRESH_S16_CTRL_S {
    IVE_THRESH_S16_MODE_E enMode;
    RK_S16 s16LowThr;     /* User-defined threshold */
    RK_S16 s16HighThr;    /* User-defined threshold */
    IVE_8BIT_U un8MinVal; /* Minimum value when tri-level thresholding */
    IVE_8BIT_U un8MidVal; /* Middle value when tri-level thresholding */
    IVE_8BIT_U un8MaxVal; /* Maxmum value when tri-level thresholding */
} IVE_THRESH_S16_CTRL_S;

/*
* Type of the Thresh_U16 mode
*/
typedef enum rkIVE_THRESH_U16_MODE_E {
    IVE_THRESH_U16_MODE_U16_TO_U8_MIN_MID_MAX = 0x0,
    IVE_THRESH_U16_MODE_U16_TO_U8_MIN_ORI_MAX = 0x1,

    IVE_THRESH_U16_MODE_BUTT
} IVE_THRESH_U16_MODE_E;

/*
* Thresh_U16 control parameters
*/
typedef struct rkIVE_THRESH_U16_CTRL_S {
    IVE_THRESH_U16_MODE_E enMode;
    RK_U16 u16LowThr;
    RK_U16 u16HighThr;
    RK_U8 u8MinVal;
    RK_U8 u8MidVal;
    RK_U8 u8MaxVal;
} IVE_THRESH_U16_CTRL_S;

/*
 *Type of the Sub output results
 */
typedef enum rkIVE_SUB_MODE_E {
    IVE_SUB_MODE_ABS = 0x0, /*Absolute value of the difference*/
    IVE_SUB_MODE_SHIFT =
        0x1, /*The output result is obtained by shifting the result one digit
                right to reserve the signed bit.*/
    IVE_SUB_MODE_BUTT
} IVE_SUB_MODE_E;

/*
 *Sub control parameters
 */
typedef struct rkIVE_SUB_CTRL_S {
    IVE_SUB_MODE_E enMode;
} IVE_SUB_CTRL_S;

/*
 *Type of the Integ output results
 * Spec: add IVE_INTEG_OUT_CTRL_SQSUM output format
 */
typedef enum rkIVE_INTEG_OUT_CTRL_E {
    IVE_INTEG_OUT_CTRL_SUM = 0x0,
    IVE_INTEG_OUT_CTRL_SQSUM = 0x1,
    IVE_INTEG_OUT_CTRL_COMBINE = 0x2,

    IVE_INTEG_OUT_CTRL_BUTT
} IVE_INTEG_OUT_CTRL_E;

/*
 * Integ control parameters
 */
typedef struct rkIVE_INTEG_CTRL_S {
    IVE_INTEG_OUT_CTRL_E enOutCtrl;
    IVE_MEM_INFO_S stMem;
} IVE_INTEG_CTRL_S;

/*
 *Mode of 8BitTo8Bit
 */
typedef enum rkIVE_8BIT_TO_8BIT_MODE_E {
    IVE_8BIT_TO_8BIT_MODE_S8_TO_S8 = 0x0,
    IVE_8BIT_TO_8BIT_MODE_S8_TO_U8_ABS = 0x1,
    IVE_8BIT_TO_8BIT_MODE_S8_TO_U8_BIAS = 0x2,
    IVE_8BIT_TO_8BIT_MODE_U8_TO_U8 = 0x3,

    IVE_8BIT_TO_8BIT_MODE_BUTT
} IVE_8BIT_TO_8BIT_MODE_E;

/*
 *8BitTo8Bit control parameters
 */
typedef struct rkIVE_8BIT_TO_8BIT_CTRL_S {
    IVE_8BIT_TO_8BIT_MODE_E enMode;
    RK_U8 u8Denominator;
    RK_U8 u8Numerator;
    RK_S8 s8Bias;
} IVE_8BIT_TO_8BIT_CTRL_S;

/*
 *Mode of 16BitTo8Bit
 */
typedef enum rkIVE_16BIT_TO_8BIT_MODE_E {
    IVE_16BIT_TO_8BIT_MODE_S16_TO_S8 = 0x0,
    IVE_16BIT_TO_8BIT_MODE_S16_TO_U8_ABS = 0x1,
    IVE_16BIT_TO_8BIT_MODE_S16_TO_U8_BIAS = 0x2,
    IVE_16BIT_TO_8BIT_MODE_U16_TO_U8 = 0x3,

    IVE_16BIT_TO_8BIT_MODE_BUTT
} IVE_16BIT_TO_8BIT_MODE_E;

/*
 *16BitTo8Bit control parameters
 */
typedef struct rkIVE_16BIT_TO_8BIT_CTRL_S {
    IVE_16BIT_TO_8BIT_MODE_E enMode;
    RK_U16 u16Denominator;
    RK_U8 u8Numerator;
    RK_S8 s8Bias;
} IVE_16BIT_TO_8BIT_CTRL_S;

/*
 *Type of the OrdStaFilter
 */
typedef enum rkIVE_ORD_STAT_FILTER_MODE_E {
    IVE_ORD_STAT_FILTER_MODE_MEDIAN = 0x0,
    IVE_ORD_STAT_FILTER_MODE_MAX = 0x1,
    IVE_ORD_STAT_FILTER_MODE_MIN = 0x2,

    IVE_ORD_STAT_FILTER_MODE_BUTT
} IVE_ORD_STAT_FILTER_MODE_E;

/*
 *OrdStaFilter control parameters
 */
typedef struct rkIVE_ORD_STAT_FILTER_CTRL_S {
    IVE_ORD_STAT_FILTER_MODE_E enMode;

} IVE_ORD_STAT_FILTER_CTRL_S;

/*
 *Type of the Map
 */
typedef enum rkIVE_MAP_MODE_E {
    IVE_MAP_MODE_U8 = 0x0,
    IVE_MAP_MODE_S16 = 0x1,
    IVE_MAP_MODE_U16 = 0x2,

    IVE_MAP_MODE_BUTT
} IVE_MAP_MODE_E;

/*
 * Map control struct
 */
typedef struct rkIVE_MAP_CTRL_S {
    IVE_MAP_MODE_E enMode;
} IVE_MAP_CTRL_S;

/*
 * Map unsigned 8 bit LUT memory struct
 */
typedef struct rkIVE_MAP_U8BIT_LUT_MEM_S {
    RK_U8 au8Map[IVE_MAP_NUM];
} IVE_MAP_U8BIT_LUT_MEM_S;

/*
 * Map unsigned 16 bit LUT memory struct
 */
typedef struct rkIVE_MAP_U16BIT_LUT_MEM_S {
    RK_U16 au16Map[IVE_MAP_NUM];
} IVE_MAP_U16BIT_LUT_MEM_S;

/*
 * Map signed 16 bit LUT memory struct
 */
typedef struct rkIVE_MAP_S16BIT_LUT_MEM_S {
    RK_S16 as16Map[IVE_MAP_NUM];
} IVE_MAP_S16BIT_LUT_MEM_S;

/*
 * Equalizehist control member struct
 */
typedef struct rkIVE_EQUALIZE_RKST_CTRL_MEM_S {
    RK_U32 au32Hist[IVE_RKST_NUM];
    RK_U8 au8Map[IVE_MAP_NUM];
} IVE_EQUALIZE_RKST_CTRL_MEM_S;

/*
 *Type of the Map
 */
typedef enum rkIVE_EQUALIZE_MODE_E {
    IVE_EQUALIZE_MODE_HIST = 0x0,
    IVE_EQUALIZE_MODE_EQHIST_WITH_EXT_HIST = 0x1,
    IVE_EQUALIZE_MODE_EQHIST = 0x2,

    IVE_EQUALIZE_MODE_BUTT
} IVE_EQUALIZE_MODE_E;

/*
 *Equalizehist control parameters
 */
typedef struct rkIVE_EQUALIZE_RKST_CTRL_S {
    IVE_EQUALIZE_MODE_E enMode;
    RK_U32 u32HistArray[256];
    IVE_MEM_INFO_S u32HistMem;
} IVE_EQHIST_CTRL_S;

/*
 *Add control parameters
 */
typedef struct rkIVE_ADD_CTRL_S {
    RK_U16 u0q16X; /*x of "xA+yB"*/
    RK_U16 u0q16Y; /*y of "xA+yB"*/
} IVE_ADD_CTRL_S;

/*
 *NCC dst memory struct
 */
typedef struct rkIVE_NCC_DST_MEM_S {
    RK_U64 u64Numerator;
    RK_U64 u64QuadSum1;
    RK_U64 u64QuadSum2;
    RK_U8 u8Reserved[8];
} IVE_NCC_DST_MEM_S;

/*
 *CCBLOB struct
 */
typedef struct rkIVE_CCBLOB_S {
    RK_U32 u32CurAreaThr; /*Threshold of the result regions' area*/
    RK_S8 s8LabelStatus;  /*-1: Labeled failed ; 0: Labeled successfully*/
    RK_U8 u8RegionNum;    /*Number of valid region, non-continuous stored*/
    RK_U8 reserved[6];
    IVE_REGION_S astRegion[IVE_MAX_REGION_NUM]; /*Valid regions with 'u32Area>0'
                                                   and 'label = ArrayIndex+1'*/
} IVE_CCBLOB_S;

/*
 *Type of the CCL
 */
typedef enum rkIVE_CCL_MODE_E {
    IVE_CCL_MODE_4C = 0x0, /*4-connected*/
    IVE_CCL_MODE_8C = 0x1, /*8-connected*/

    IVE_CCL_MODE_BUTT
} IVE_CCL_MODE_E;

/*
 *CCL control struct
 */
typedef struct rkIVE_CCL_CTRL_S {
    IVE_CCL_MODE_E enMode; /*Mode*/
    RK_U16 u16InitAreaThr;    /*Init threshold of region area*/
    RK_U16 u16Step;           /*Increase area step for once*/
    IVE_MEM_INFO_S stMem;
} IVE_CCL_CTRL_S;

/*
 *GMM control struct
 */
typedef struct rkIVE_GMM_CTRL_S {
    RK_U8 u8PicFormat;
    RK_U8 u8FirstFrameFlag;
    RK_U8 u8EnBgOut;
    RK_U8 u8MaxModelNum;
    RK_U8 u8UpdateVar;

    ive_u8q2 u8q2WeightInitVal;   /*Initial weight*/
    ive_u8q2 u8q2WeightAddFactor; /*Weight adding factor*/
    ive_u8q2 u8q2WeightReduFactor; /*Weight reduction factor*/
    ive_u8q2 u8q2WeightThr;   /*Weight threshold*/

    RK_U8 u8VarThreshGen;    /*Global sensitivity factor*/
    ive_u8q2 u8q2BgRatio; /*Background ratio*/

    ive_u10q0 u10q0InitVar; /*Initial noise Variance*/
    ive_u10q0 u10q0MinVar;  /*Min variation*/
    ive_u10q0 u10q0MaxVar;  /*Max variation*/
    RK_U8 u8VarThr;            /*Variance Threshold*/
} IVE_GMM_CTRL_S;

/*
 *GMM control struct
 */
typedef struct rkIVE_GMM2_CTRL_S {
    RK_U8 u8PicFormat;
    RK_U8 u8FirstFrameFlag;
    RK_U8 u8EnBgOut;
    RK_U8 u8MaxModelNum;
    RK_U8 u8UseVarFactor;
    RK_U8 u8GlobalLearningRateMode; /*Learning rate*/
    RK_U8 u8UpdateVar;

    ive_u8q2 u8q2WeightInitVal;   /*Initial weight*/
    ive_u8q2 u8q2WeightAddFactor; /*Weight adding factor*/
    ive_u8q2 u8q2WeightReduFactor; /*Weight reduction factor*/
    ive_u8q2 u8q2WeightThr;   /*Weight threshold*/

    RK_U8 u8VarThreshGen; /*Global sensitivity factor*/
    ive_u8q2 u8q2BgRatio; /*Background ratio*/

    ive_u10q0 u10q0InitVar; /*Initial noise Variance*/
    ive_u10q0 u10q0MinVar;  /*Min variation*/
    ive_u10q0 u10q0MaxVar;  /*Max variation*/
    RK_U8 u8VarThr;         /*Variance Threshold*/
} IVE_GMM2_CTRL_S;

typedef struct rkIVE_GMM_NUM_INFO_S {
    RK_U32 GMMModelNum;   /*Stack size for output*/
    RK_U8 u8Reserved[12]; /*For 16 byte align*/
} IVE_GMM_NUM_INFO_S;

/*
 *CannyHysEdge control struct
 */
typedef struct rkIVE_CANNY_HYS_EDGE_CTRL_S {
    IVE_MEM_INFO_S stMem;
    RK_U16 u16LowThr;
    RK_U16 u16HighThr;
    RK_S8 as8Mask[25];
} IVE_CANNY_EDGE_CTRL_S;

/*
 *Canny stack size struct
 */
typedef struct rkIVE_CANNY_STACK_SIZE_S {
    RK_U32 u32StackSize;  /*Stack size for output*/
    RK_U8 u8Reserved[12]; /*For 16 byte align*/
} IVE_CANNY_STACK_SIZE_S;

/*
 *LBP compare mode
 */
typedef enum rkIVE_LBP_CMP_MODE_E {
    IVE_LBP_CMP_MODE_NORMAL = 0x0, /* P(x)-P(center)>= un8BitThr.s8Val, s(x)=1; else s(x)=0; */
    IVE_LBP_CMP_MODE_ABS = 0x1, /* Abs(P(x)-P(center))>=un8BitThr.u8Val, s(x)=1; else s(x)=0; */

    IVE_LBP_CMP_MODE_BUTT
} IVE_LBP_CMP_MODE_E;

/*
 *LBP control struct
 */
typedef struct rkIVE_LBP_CTRL_S {
    IVE_LBP_CMP_MODE_E enMode;
    IVE_8BIT_U un8BitThr;
} IVE_LBP_CTRL_S;

/*
 *Type of the GradientFilter output format
 */
typedef enum rkIVE_NORM_GRAD_OUT_CTRL_E {
    IVE_NORM_GRAD_OUT_CTRL_HOR_AND_VER = 0x0,
    IVE_NORM_GRAD_OUT_CTRL_HOR = 0x1,
    IVE_NORM_GRAD_OUT_CTRL_VER = 0x2,
    IVE_NORM_GRAD_OUT_CTRL_COMBINE = 0x3,

    IVE_NORM_GRAD_OUT_CTRL_BUTT
} IVE_NORM_GRAD_OUT_CTRL_E;

/*
 *GradientFilter control parameters
 */
typedef struct rkIVE_NORM_GRAD_CTRL_S {
    IVE_NORM_GRAD_OUT_CTRL_E enOutCtrl;
    RK_S8 as8Mask[25];
    RK_U8 u8Norm;
} IVE_NORM_GRAD_CTRL_S;

/*
 * Shi-Tomasi maximum eigenvalue
 */
typedef struct rkIVE_ST_CORNER_MEM_S {
    RK_U32 stmem[800];
} IVE_ST_CORNER_MEM_S;

/*
 *Shi-Tomasi control parameters
 */
typedef struct rkIVE_ST_CANDI_CORNER_CTRL_S {
    RK_U8 u0q8QualityLevel;
} IVE_ST_CANDI_CORNER_CTRL_S;

/*
 *Shi-Tomasi stack size struct
 */
typedef struct rkIVE_ST_CANDI_STACK_SIZE_S {
    RK_U32 u32StackSize;  /*Stack size for output*/
    RK_U8 u8Reserved[12]; /*For 16 byte align*/
} IVE_ST_CANDI_STACK_SIZE_S;

/*
 *Shi-Tomasi corner info struct
 */
typedef struct rkIVE_ST_CANDI_CORNER_INFO_S {
    RK_U32 : 2;
    RK_U32 u8Lambda : 8;
    RK_U32 u11Y : 11;
    RK_U32 u11X : 11;
} IVE_ST_CANDI_CORNER_INFO_S;

/*
 *Shi-Tomasi corner info struct
 */
typedef struct rkIVE_ST_CORNER_INFO_S {
    RK_U16 u16CornerNum;
    IVE_POINT_U16_S astCorner[IVE_ST_MAX_CORNER_NUM];
    RK_U8 u8Lambda[IVE_ST_MAX_CORNER_NUM];
} IVE_ST_CORNER_INFO_S;

/*
 *Shi-Tomasi control parameters
 */
typedef struct rkIVE_ST_CORNER_CTRL_S {
    IVE_MEM_INFO_S stMem;
    RK_U16 u16MaxCornerNum;
    RK_U16 u16MinDist;
} IVE_ST_CORNER_CTRL_S;

/*
 *Shi-Tomasi control parameters
 */
typedef struct rkIVE_STCORNER_STEPS_CTRL_S {
    RK_U8 u8StepSel;                // step selection
    RK_U8 u8LookUpTblClose;         // lookup table for performance 1: close
    RK_U8 u0q8QualityLevel;         // step1 theshhold value of lambda
    RK_U16 u16Step2MaxCornerNum;    // step2 max corner num limit
    RK_U16 u8Step2MinDist;          // step2 min distance limit
    RK_U32 u32Step1OutCornerNum;    // step1 output num of candidate corner
    RK_U32 u32Step2CandiCornerNum;  // step2 input num of candidate corner
    RK_U16 u16Step2OutCornerNum;    // step2 output num of filtered coerner
} IVE_STCORNER_CTRL_S;

/*
 * GradFg mode enum
 */
typedef enum rkIVE_GRAD_FG_MODE_E {
    IVE_GRAD_FG_MODE_USE_CUR_GRAD = 0x0,
    IVE_GRAD_FG_MODE_FIND_MIN_GRAD = 0x1,

    IVE_GRAD_FG_MODE_BUTT
} IVE_GRAD_FG_MODE_E;

/*
 * GradFg ctrl struct
 */
typedef struct rkIVE_GRAD_FG_CTRL_S {
    IVE_GRAD_FG_MODE_E enMode; /*Calculation mode*/
    RK_U16 u16EdwFactor; /*Edge width adjustment factor (range: 500 to 2000; default: 1000)*/
    RK_U8 u8CrlCoefThr;  /*Gradient vector correlation coefficient threshold (ranges: 50 to 100; default: 80)*/
    RK_U8 u8MagCrlThr;   /*Gradient amplitude threshold (range: 0 to 20; default: 4)*/
    RK_U8 u8MinMagDiff;  /*Gradient magnitude difference threshold (range: 2 to 8; default: 2)*/
    RK_U8 u8NoiseVal; /*Gradient amplitude noise threshold (range: 1 to 8; default: 1)*/
    RK_U8 u8EdwDark;  /*Black pixels enable flag (range: 0 (no), 1 (yes); default: 1)*/
} IVE_GRAD_FG_CTRL_S;

/*
 *Backgroud model pixel info
 */
typedef struct rkIVE_BG_MODEL_PIX_S {
    RK_U8 bg_model[14];
} IVE_BG_MODEL_PIX_S;

/*
 *MatchBgModel control parameters
 */
typedef struct rkIVE_MATCH_BG_MODEL_CTRL_S {
    RK_U8 u8CodeWordNum;   /* The cwnum of current training codebook.Value range is 0~2 */
    RK_U32 u32CurFrmNum;     /* Current frame timestamp, in frame units */
    RK_U8 u8TrainTimeThr; /* Potential background replacement time threshold (range: 10 to 100 frames; default: 20) */
    RK_U8 u8DiffMaxThr; /* Maximum of background differential threshold (range: 5 to 15; default: 10) */
    RK_U8 u8DiffMinThr; /* Minimum of background differential threshold (range: 5 to 15; default: 10) */
} IVE_MATCH_BG_MODEL_CTRL_S;

/*
 *MatchBgModel control parameters
 */
typedef struct rkIVE_UPDATE_BG_MODEL_CTRL_S {
    RK_U8 u8CodeWordNum;   /* The cwnum of current training codebook.Value range is 0~2 */
    RK_U32 u32CurFrmNum; /* Current frame timestamp, in frame units */
    RK_U8 u8TimeThr; /* Potential background replacement time threshold (range: 10 to 100 frames; default: 20) */
    RK_U8 u8DiffMaxThr; /* Maximum of background differential threshold (range: 5 to 15; default: 10) */
    RK_U8 u8DiffMinThr; /* Minimum of background differential threshold (range: 5 to 15; default: 10) */
    RK_U8 u8FastLearnRate; /* Quick background learning rate (range: 1 to 10; default: 4) */
    RK_U8 u8Alpha;
} IVE_UPDATE_BG_MODEL_CTRL_S;

/*
 * Sad mode
 */
typedef enum rkIVE_SAD_MODE_E {
    IVE_SAD_MODE_MB_4X4 = 0x0,   /*4x4*/
    IVE_SAD_MODE_MB_8X8 = 0x1,   /*8x8*/
    IVE_SAD_MODE_MB_16X16 = 0x2, /*16x16*/

    IVE_SAD_MODE_BUTT
} IVE_SAD_MODE_E;
/*
 *Sad output ctrl
 */
typedef enum rkIVE_SAD_OUT_CTRL_E {
    IVE_SAD_OUT_CTRL_16BIT_BOTH = 0x0, /*Output 16 bit sad and thresh*/
    IVE_SAD_OUT_CTRL_8BIT_BOTH = 0x1,  /*Output 8 bit sad and thresh*/
    IVE_SAD_OUT_CTRL_16BIT_SAD = 0x2,  /*Output 16 bit sad*/
    IVE_SAD_OUT_CTRL_8BIT_SAD = 0x3,   /*Output 8 bit sad*/
    IVE_SAD_OUT_CTRL_THRESH = 0x4,     /*Output thresh,16 bits sad */

    IVE_SAD_OUT_CTRL_BUTT
} IVE_SAD_OUT_CTRL_E;

/*
 *Sad output mode
 */
typedef enum rkIVE_SAD_OUT_MODE_E {
    IVE_SAD_OUT_MODE_SAD = 0x0,
    IVE_SAD_OUT_MODE_BOTH = 0x1,
    IVE_SAD_OUT_MODE_THRESH = 0x2,

    IVE_SAD_OUT_MODE_BUTT
} IVE_SAD_OUT_MODE_E;

/*
 *Sad output bits
 */
typedef enum rkIVE_SAD_OUT_BITS_E {
    IVE_SAD_OUT_8_BITS = 0x0,
    IVE_SAD_OUT_16_BITS = 0x1,

    IVE_SAD_OUT_BITS_BUTT
} IVE_SAD_OUT_BITS_E;

/*
 * Sad ctrl param
 */
typedef struct rkIVE_SAD_CTRL_S {
    IVE_SAD_MODE_E enMode;
    IVE_SAD_OUT_MODE_E enOutMode;
    IVE_SAD_OUT_BITS_E enOutBits;
    RK_U16 u16Thr;  /*srcVal <= u16Thr, dstVal = minVal; srcVal > u16Thr, dstVal = maxVal.*/
    RK_U8 u8MinVal; /*Min value*/
    RK_U8 u8MaxVal; /*Max value*/
} IVE_SAD_CTRL_S;

/*
 *Type of the Sub output results
 */
typedef enum rkIVE_LOGICOP_MODE_E {
    IVE_LOGICOP_MODE_AND = 0x0,
    IVE_LOGICOP_MODE_OR = 0x1,
    IVE_LOGICOP_MODE_XOR = 0x2,
    IVE_LOGICOP_MODE_SUB_ABS = 0x3,
    IVE_LOGICOP_MODE_SUB_SHIFT = 0x4,
    IVE_LOGICOP_MODE_ADD = 0x5,
} IVE_LOGICOP_MODE_E;

/*
 *logicOp control parameters
 */
typedef struct rkIVE_LOGICOP_CTRL_S {
    IVE_LOGICOP_MODE_E enMode;
    RK_U16 u0q16X; /*x of "xA+yB"*/
    RK_U16 u0q16Y; /*y of "xA+yB"*/
} IVE_LOGICOP_CTRL_S;

/*
 *Type of the denoise output results
 */
typedef enum rkIVE_DENOISE_MODE_E {
    IVE_DENOISE_MODE_MIN = 0x0,
    IVE_DENOISE_MODE_MEDIAN = 0x1,
    IVE_DENOISE_MODE_MAX = 0x2,
} IVE_DENOISE_MODE_E;

/*
 *denoise control parameters
 */
typedef struct rkIVE_DENOISE_CTRL_S {
    IVE_DENOISE_MODE_E enMode;
} IVE_DENOISE_CTRL_S;

/*
 * Resize zoom mode
 */
typedef enum rkIVE_RESIZE_MODE_E {
    IVE_RESIZE_MODE_LINEAR = 0x0, /*Bilinear interpolation*/
    IVE_RESIZE_MODE_AREA = 0x1,   /*Area-based (or super) interpolation*/

    IVE_RESIZE_MODE_BUTT
} IVE_RESIZE_MODE_E;

/*
 * Resize ctrl param
 */
typedef struct rkIVE_RESIZE_CTRL_S {
    IVE_RESIZE_MODE_E enMode;
    IVE_MEM_INFO_S stMem;
    unsigned short u16Num;
} IVE_RESIZE_CTRL_S;

typedef struct rkIVE_WARP_AFFINE_CTRL_S {
    IVE_MEM_INFO_S stMem;
    RK_FLOAT stAffineMat[6];
} IVE_WARP_AFFINE_CTRL_S;

typedef struct rkIVE_PYRAMID_CTRL_S {
    IVE_MEM_INFO_S stPyramidMem;
    RK_U8 level;
} IVE_PYRAMID_CTRL_S;

#if __cplusplus
}
#endif
#endif /*_RK_IVE_H_*/