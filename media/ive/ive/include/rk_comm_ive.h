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
#ifndef _RK_COMM_IVE_H_
#define _RK_COMM_IVE_H_

#include "rk_errno.h"
#include "rk_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

//typedef RK_S32 IVE_HANDLE;

typedef unsigned char IVE_U0Q8;
typedef unsigned short IVE_U0Q11;
typedef unsigned short IVE_U8Q2;
typedef unsigned short IVE_U9Q7;
typedef unsigned short IVE_U10Q0;
typedef short IVE_S9Q7;
typedef int IVE_S25Q7;
typedef IVE_U0Q8 ive_u0q8;
typedef IVE_U0Q11 ive_u0q11;
typedef IVE_U8Q2 ive_u8q2;
typedef IVE_U9Q7 ive_u9q7;
typedef IVE_U10Q0 ive_u10q0;
typedef IVE_S9Q7 ive_s9q7;
typedef IVE_S25Q7 ive_s25q7;

/* Type of the IVE_IMAGE_S data.*/
typedef enum rkIVE_IMAGE_TYPE_E {
    IVE_IMAGE_TYPE_U8C1 = 0x0,
    IVE_IMAGE_TYPE_S8C1 = 0x1,

    IVE_IMAGE_TYPE_YUV420SP = 0x2, /* YUV420 SemiPlanar */
    IVE_IMAGE_TYPE_YUV422SP = 0x3, /* YUV422 SemiPlanar */
    IVE_IMAGE_TYPE_YUV420P = 0x4,  /* YUV420 Planar */
    IVE_IMAGE_TYPE_YUV422P = 0x5,  /* YUV422 planar */

    IVE_IMAGE_TYPE_S8C2_PACKAGE = 0x6,
    IVE_IMAGE_TYPE_S8C2_PLANAR = 0x7,

    IVE_IMAGE_TYPE_S16C1 = 0x8,
    IVE_IMAGE_TYPE_U16C1 = 0x9,

    IVE_IMAGE_TYPE_U8C3_PACKAGE = 0xa,
    IVE_IMAGE_TYPE_U8C3_PLANAR = 0xb,

    IVE_IMAGE_TYPE_S32C1 = 0xc,
    IVE_IMAGE_TYPE_U32C1 = 0xd,

    IVE_IMAGE_TYPE_S64C1 = 0xe,
    IVE_IMAGE_TYPE_U64C1 = 0xf,

    IVE_IMAGE_TYPE_BUTT

} IVE_IMAGE_TYPE_E;

/* Definition of the IVE_IMAGE_S.*/
typedef struct rkIVE_IMAGE_S {
    RK_U64 au64PhyAddr[3];   /* RW;The physical address of the image */
    RK_U64 au64VirAddr[3];   /* RW;The virtual address of the image */
    RK_U32 au32Stride[3];    /* RW;The stride of the image */
    RK_U32 u32Width;         /* RW;The width of the image */
    RK_U32 u32Height;        /* RW;The height of the image */
    IVE_IMAGE_TYPE_E enType; /* RW;The type of the image */
    RK_U32 u32Reserved;
} IVE_IMAGE_S;

typedef IVE_IMAGE_S IVE_SRC_IMAGE_S;
typedef IVE_IMAGE_S IVE_DST_IMAGE_S;

/*
* Definition of the IVE_MEM_INFO_S.This struct special purpose for input or ouput, such as Hist, CCL, ShiTomasi.
*/
typedef struct rkIVE_MEM_INFO_S {
    RK_U64 u64PhyAddr; /* RW;The physical address of the memory */
    RK_U64 u64VirAddr; /* RW;The virtual address of the memory */
    RK_U32 u32Size;    /* RW;The size of memory */
    RK_U32 u32Reserved;
} IVE_MEM_INFO_S;
typedef IVE_MEM_INFO_S IVE_SRC_MEM_INFO_S;
typedef IVE_MEM_INFO_S IVE_DST_MEM_INFO_S;

/* Data struct*/
typedef struct rkIVE_DATA_S {
    RK_U64 u64PhyAddr; /* RW;The physical address of the data */
    RK_U64 u64VirAddr; /* RW;The virtaul address of the data */

    RK_U32 u32Stride; /* RW;The stride of 2D data by byte */
    RK_U32 u32Width;  /* RW;The width of 2D data by byte */
    RK_U32 u32Height; /* RW;The height of 2D data by byte */

    RK_U32 u32Reserved;
} IVE_DATA_S;
typedef IVE_DATA_S IVE_SRC_DATA_S;
typedef IVE_DATA_S IVE_DST_DATA_S;

/* Definition of RK_U16 point */
typedef struct rkIVE_POINT_U16_S {
    RK_U16 u16X; /* RW;The X coordinate of the point */
    RK_U16 u16Y; /* RW;The Y coordinate of the point */
} IVE_POINT_U16_S;

/*
* Definition of RK_S16 point
*/
typedef struct rkIVE_POINT_S16_S {
    RK_S16 s16X; /* RW;The X coordinate of the point */
    RK_S16 s16Y; /* RW;The Y coordinate of the point */
} IVE_POINT_S16_S;

typedef struct rkIVE_POINT_U32_S {
    RK_U32 u32X; /* RW;The X coordinate of the point */
    RK_U32 u32Y; /* RW;The Y coordinate of the point */
} IVE_POINT_U32_S;

typedef struct rkIVE_POINT_S32_S {
    RK_S32 s32X; /* RW;The X coordinate of the point */
    RK_S32 s32Y; /* RW;The Y coordinate of the point */
} IVE_POINT_S32_S;

typedef struct rkIVE_POINT_S25Q7_S {
    IVE_S25Q7 s25q7X; /* RW;The X coordinate of the point */
    IVE_S25Q7 s25q7Y; /* RW;The Y coordinate of the point */
} IVE_POINT_S25Q7_S;

typedef struct rkIVE_POINT_FLOAT {
    float fpX; /*X coordinate*/
    float fpY; /*Y coordinate*/
} IVE_POINT_FLOAT;

typedef struct rkIVE_MV_FLOAT {
    RK_S32 s32Statys; /*Result of tracking: 0-success; -1-failure*/
    RK_S16 fpX;       /*X coordinate*/
    RK_S16 fpY;       /*Y coordinate*/
} IVE_MV_FLOAT;

typedef struct rkIVE_MV_S16_S {
    RK_S32 s32Statys; /*Result of tracking: 0-success; -1-failure*/
    RK_S16 s16X;      /*X coordinate*/
    RK_S16 s16Y;      /*Y coordinate*/
} IVE_MV_S16_S;

typedef struct rkIVE_MV_S9Q7_S {
    RK_S32 s32Statys; /*Result of tracking: 0-success; -1-failure*/
    IVE_S9Q7 s9q7X;   /*X coordinate*/
    IVE_S9Q7 s9q7Y;   /*Y coordinate*/
} IVE_MV_S9Q7_S;

/* Definition of rectangle */
typedef struct rkIVE_RECT_U16_S {
    RK_U16 u16X;      /* RW;The location of X axis of the rectangle */
    RK_U16 u16Y;      /* RW;The location of Y axis of the rectangle */
    RK_U16 u16Width;  /* RW;The width of the rectangle */
    RK_U16 u16Height; /* RW;The height of the rectangle */
} IVE_RECT_U16_S;

typedef struct rkIVE_RECT_U32_S {
    RK_U32 u32X;      /* RW;The location of X axis of the rectangle */
    RK_U32 u32Y;      /* RW;The location of Y axis of the rectangle */
    RK_U32 u32Width;  /* RW;The width of the rectangle */
    RK_U32 u32Height; /* RW;The height of the rectangle */
} IVE_RECT_U32_S;

typedef struct rkIVE_RECT_S32_S {
    RK_S32 s32X;
    RK_S32 s32Y;
    RK_U32 u32Width;
    RK_U32 u32Height;
} IVE_RECT_S24Q8_S;

typedef struct rkIVE_LOOK_UP_TABLE_S {
    IVE_MEM_INFO_S stTable;
    RK_U16 u16ElemNum; /* RW;LUT's elements number */

    RK_U8 u8TabInPreci;
    RK_U8 u8TabOutNorm;

    RK_S32 s32TabInLower; /* RW;LUT's original input lower limit */
    RK_S32 s32TabInUpper; /* RW;LUT's original input upper limit */
} IVE_LOOK_UP_TABLE_S;

typedef enum rkEN_RK_ERR_CODE_E {
    ERR_IVE_SYS_TIMEOUT = 0x40,   /* IVE process timeout */
    ERR_IVE_QUERY_TIMEOUT = 0x41, /* IVE query timeout */
    ERR_IVE_OPEN_FILE = 0x42,     /* IVE open file error */
    ERR_IVE_READ_FILE = 0x43,     /* IVE read file error */
    ERR_IVE_WRITE_FILE = 0x44,    /* IVE write file error */
    ERR_IVE_BUS_ERR = 0x45,
    ERR_IVE_ILLEGAL_PARAMS = 0x46,
    ERR_IVE_DEVICE_ERROR = 0x47,
    ERR_IVE_NOT_SUPPORT = 0x48,
    ERR_IVE_ILLEGAL_STMEM = 0x49,

    ERR_IVE_BUTT
} EN_RK_ERR_CODE_E;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* __RVE_COMM_RVE_H__ */

