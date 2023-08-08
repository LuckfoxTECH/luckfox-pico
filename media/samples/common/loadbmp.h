/*
 * Copyright 2021 Rockchip Electronics Co. LTD
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
#ifndef __LOAD_BMP_H__
#define __LOAD_BMP_H__

#include "rk_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* the color format OSD supported */
typedef enum rkOSD_COLOR_FMT_E {
	OSD_COLOR_FMT_RGB444 = 0,
	OSD_COLOR_FMT_RGB4444 = 1,
	OSD_COLOR_FMT_RGB555 = 2,
	OSD_COLOR_FMT_RGB565 = 3,
	OSD_COLOR_FMT_ARGB1555 = 4,
	OSD_COLOR_FMT_BGRA5551 = 5,
	OSD_COLOR_FMT_RGB888 = 6,
	OSD_COLOR_FMT_BGR888 = 7,
	OSD_COLOR_FMT_ARGB8888 = 8,
	OSD_COLOR_FMT_BGRA8888 = 9,
	OSD_COLOR_FMT_BUTT
} OSD_COLOR_FMT_E;

typedef struct rkOSD_RGB_S {
	RK_U8 u8B;
	RK_U8 u8G;
	RK_U8 u8R;
	RK_U8 u8Reserved;
} OSD_RGB_S;

typedef struct rkOSD_SURFACE_S {
	OSD_COLOR_FMT_E enColorFmt; /* color format */
	RK_U8 *pu8PhyAddr;          /* physical address */
	RK_U16 u16Height;           /* operation height */
	RK_U16 u16Width;            /* operation width */
	RK_U16 u16Stride;           /* surface stride */
	RK_U16 u16Reserved;
} OSD_SURFACE_S;

typedef struct tag_OSD_Logo {
	RK_U32 width;      /* out */
	RK_U32 height;     /* out */
	RK_U32 stride;     /* in */
	RK_U8 *pRGBBuffer; /* in/out */
} OSD_LOGO_T;

typedef struct tag_OSD_BITMAPINFOHEADER {
	RK_U16 biSize;
	RK_U32 biWidth;
	RK_S32 biHeight;
	RK_U16 biPlanes;
	RK_U16 biBitCount;
	RK_U32 biCompression;
	RK_U32 biSizeImage;
	RK_U32 biXPelsPerMeter;
	RK_U32 biYPelsPerMeter;
	RK_U32 biClrUsed;
	RK_U32 biClrImportant;
} OSD_BITMAPINFOHEADER;

typedef struct tag_OSD_BITMAPFILEHEADER {
	RK_U32 bfSize;
	RK_U16 bfReserved1;
	RK_U16 bfReserved2;
	RK_U32 bfOffBits;
} OSD_BITMAPFILEHEADER;

typedef struct tag_OSD_RGBQUAD {
	RK_U8 rgbBlue;
	RK_U8 rgbGreen;
	RK_U8 rgbRed;
	RK_U8 rgbReserved;
} OSD_RGBQUAD;

typedef struct tag_OSD_BITMAPINFO {
	OSD_BITMAPINFOHEADER bmiHeader;
	OSD_RGBQUAD bmiColors[1];
} OSD_BITMAPINFO;

typedef struct rkOSD_COMPONENT_INFO_S {
	RK_S32 alen;
	RK_S32 rlen;
	RK_S32 glen;
	RK_S32 blen;
} OSD_COMP_INFO;

RK_S32 load_image(const char *filename, OSD_LOGO_T *pVideoLogo);
RK_S32 load_bitmap_2_surface(const char *pstFileName, const OSD_SURFACE_S *pstSurface,
                             RK_U8 *pu8Virt);
RK_S32 create_surface_by_bitmap(const char *pstFileName, OSD_SURFACE_S *pstSurface,
                                RK_U8 *pu8Virt);
RK_S32 get_bmp_info(const char *filename, OSD_BITMAPFILEHEADER *pBmpFileHeader,
                    OSD_BITMAPINFO *pBmpInfo);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef __LOAD_BMP_H__*/
