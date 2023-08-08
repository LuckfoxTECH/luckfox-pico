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

#include "loadbmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

OSD_COMP_INFO s_OSDCompInfo[OSD_COLOR_FMT_BUTT] = {
    {0, 4, 4, 4}, /*RGB444*/
    {4, 4, 4, 4}, /*ARGB4444*/
    {0, 5, 5, 5}, /*RGB555*/
    {0, 5, 6, 5}, /*RGB565*/
    {1, 5, 5, 5}, /*ARGB1555*/
    {1, 5, 5, 5}, /*BGRA5551*/
    {0, 8, 8, 8}, /*RGB888*/
    {0, 8, 8, 8}, /*BGR888*/
    {8, 8, 8, 8}, /*ARGB8888*/
    {8, 8, 8, 8}  /*BGRA8888*/
};

RK_U32 OSD_MAKECOLOR_ARGB(RK_U8 r, RK_U8 g, RK_U8 b, OSD_COMP_INFO compinfo) {
	RK_U8 r1, g1, b1;
	RK_U32 pixel = 0;
	RK_U32 tmp = compinfo.alen - 1;
	RK_U8 alen = compinfo.alen;

	r1 = g1 = b1 = 0;
	r1 = r >> (8 - compinfo.rlen);
	g1 = g >> (8 - compinfo.glen);
	b1 = b >> (8 - compinfo.blen);
	while (alen) {
		pixel |= (1 << tmp);
		tmp--;
		alen--;
	}

	pixel |= (r1 << compinfo.alen | (g1 << (compinfo.rlen + compinfo.alen)) |
	          (b1 << (compinfo.rlen + compinfo.glen + compinfo.alen)));
	return pixel;
}

RK_U32 OSD_MAKECOLOR_BGRA(RK_U8 r, RK_U8 g, RK_U8 b, OSD_COMP_INFO compinfo) {
	RK_U8 r1, g1, b1;
	RK_U32 pixel = 0;
	RK_U32 tmp = compinfo.alen + compinfo.rlen + compinfo.glen + compinfo.blen - 1;
	RK_U8 alen = compinfo.alen;

	r1 = g1 = b1 = 0;
	r1 = r >> (8 - compinfo.rlen);
	g1 = g >> (8 - compinfo.glen);
	b1 = b >> (8 - compinfo.blen);
	while (alen) {
		pixel |= (1 << tmp);
		tmp--;
		alen--;
	}

	pixel |= ((r1 << (compinfo.blen + compinfo.glen)) | (g1 << compinfo.blen) | b1);
	return pixel;
}

RK_S32 get_bmp_info(const char *filename, OSD_BITMAPFILEHEADER *pBmpFileHeader,
                    OSD_BITMAPINFO *pBmpInfo) {
	FILE *pFile;

	RK_U16 bfType;

	if (NULL == filename) {
		printf("OSD_LoadBMP: filename=NULL\n");
		return -1;
	}

	if ((pFile = fopen(filename, "rb")) == NULL) {
		printf("Open file faild:%s!\n", filename);
		return -1;
	}

	(void)fread(&bfType, 1, sizeof(bfType), pFile);
	if (bfType != 0x4d42) {
		printf("not bitmap file\n");
		close(pFile);
		return -1;
	}

	(void)fread(pBmpFileHeader, 1, sizeof(OSD_BITMAPFILEHEADER), pFile);
	(void)fread(pBmpInfo, 1, sizeof(OSD_BITMAPINFO), pFile);
	fclose(pFile);

	return 0;
}

RK_S32 load_bmp_ex(const char *filename, OSD_LOGO_T *pVideoLogo, OSD_COLOR_FMT_E enFmt) {
	FILE *pFile;
	RK_U16 i, j;

	RK_U32 w, h;
	RK_U16 Bpp;

	OSD_BITMAPFILEHEADER bmpFileHeader;
	OSD_BITMAPINFO bmpInfo;

	RK_U8 *pOrigBMPBuf;
	RK_U8 *pRGBBuf;
	RK_U32 stride;
	RK_U8 r, g, b;
	RK_U8 *pStart;
	RK_U16 *pDst;
	RK_U32 *pu32Dst;

	if (NULL == filename) {
		printf("load_bmp_ex: filename=NULL\n");
		return -1;
	}

	if (get_bmp_info(filename, &bmpFileHeader, &bmpInfo) < 0) {
		return -1;
	}

	Bpp = bmpInfo.bmiHeader.biBitCount / 8;
	if (Bpp < 2) {
		/* only support 1555.8888  888 bitmap */
		printf("bitmap format not supported!\n");
		return -1;
	}

	if (bmpInfo.bmiHeader.biCompression != 0) {
		printf("not support compressed bitmap file!\n");
		return -1;
	}

	if (bmpInfo.bmiHeader.biHeight < 0) {
		printf("bmpInfo.bmiHeader.biHeight < 0\n");
		return -1;
	}

	if ((pFile = fopen(filename, "rb")) == NULL) {
		printf("Open file faild:%s!\n", filename);
		return -1;
	}

	pVideoLogo->width = (RK_U16)bmpInfo.bmiHeader.biWidth;
	pVideoLogo->height =
	    (RK_U16)((bmpInfo.bmiHeader.biHeight > 0) ? bmpInfo.bmiHeader.biHeight
	                                              : (-bmpInfo.bmiHeader.biHeight));
	w = pVideoLogo->width;
	h = pVideoLogo->height;

	stride = w * Bpp;

	if (stride % 4) {
		stride = (stride & 0xfffc) + 4;
	}

	/* RGB8888 or RGB1555 */
	pOrigBMPBuf = (RK_U8 *)(malloc(h * stride));
	if (NULL == pOrigBMPBuf) {
		printf("not enough memory to malloc!\n");
		fclose(pFile);
		return -1;
	}

	pRGBBuf = pVideoLogo->pRGBBuffer;

	fseek(pFile, bmpFileHeader.bfOffBits, 0);
	if (fread(pOrigBMPBuf, 1, h * stride, pFile) != (h * stride)) {
		printf("fread (%d*%d)error!line:%d\n", h, stride, __LINE__);
		return -1;
	}

	if (enFmt >= OSD_COLOR_FMT_RGB888) {
		pVideoLogo->stride = pVideoLogo->width * 4;
	} else {
		pVideoLogo->stride = pVideoLogo->width * 2;
	}

	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			if (Bpp == 3) {
				switch (enFmt) {
				case OSD_COLOR_FMT_RGB444:
				case OSD_COLOR_FMT_RGB555:
				case OSD_COLOR_FMT_RGB565:
				case OSD_COLOR_FMT_ARGB1555:
					/* start color convert */
					pStart = pOrigBMPBuf + ((h - 1) - i) * stride + j * Bpp;
					pDst = (RK_U16 *)(pRGBBuf + i * pVideoLogo->stride + j * 2);
					b = *(pStart);
					g = *(pStart + 1);
					r = *(pStart + 2);
					*pDst = (RK_U16)OSD_MAKECOLOR_ARGB(r, g, b, s_OSDCompInfo[enFmt]);
					break;
				case OSD_COLOR_FMT_BGRA5551:
					/* start color convert */
					pStart = pOrigBMPBuf + ((h - 1) - i) * stride + j * Bpp;
					pDst = (RK_U16 *)(pRGBBuf + i * pVideoLogo->stride + j * 2);
					b = *(pStart);
					g = *(pStart + 1);
					r = *(pStart + 2);
					*pDst = (RK_U16)OSD_MAKECOLOR_BGRA(r, g, b, s_OSDCompInfo[enFmt]);
					break;
				case OSD_COLOR_FMT_BGR888:
				case OSD_COLOR_FMT_BGRA8888:
					memcpy(pRGBBuf + i * pVideoLogo->stride + j * 4,
					       pOrigBMPBuf + ((h - 1) - i) * stride + j * Bpp, Bpp);
					*(pRGBBuf + i * pVideoLogo->stride + j * 4 + 3) = 0xff; /*alpha*/
					break;
				case OSD_COLOR_FMT_RGB888:
				case OSD_COLOR_FMT_ARGB8888:
					/* start color convert */
					pStart = pOrigBMPBuf + ((h - 1) - i) * stride + j * Bpp;
					pu32Dst = (RK_U32 *)(pRGBBuf + i * pVideoLogo->stride + j * 4);
					b = *(pStart);
					g = *(pStart + 1);
					r = *(pStart + 2);
					*pu32Dst = OSD_MAKECOLOR_ARGB(r, g, b, s_OSDCompInfo[enFmt]);
					break;
				default:
					printf("file(%s), line(%d), no such format!\n", __FILE__, __LINE__);
					break;
				}
			} else if ((Bpp == 2) || (Bpp == 4)) {
				memcpy(pRGBBuf + i * pVideoLogo->stride + j * Bpp,
				       pOrigBMPBuf + ((h - 1) - i) * stride + j * Bpp, Bpp);
			}
		}
	}

	free(pOrigBMPBuf);
	pOrigBMPBuf = NULL;

	fclose(pFile);
	return 0;
}

char *get_ext_name(char *filename) {
	char *pret = NULL;
	RK_U32 fnLen;

	if (NULL == filename) {
		printf("filename can't be null!");
		return NULL;
	}

	fnLen = strlen(filename);
	while (fnLen) {
		pret = filename + fnLen;
		if (*pret == '.')
			return (pret + 1);

		fnLen--;
	}

	return pret;
}

RK_S32 load_image_ex(const char *filename, OSD_LOGO_T *pVideoLogo,
                     OSD_COLOR_FMT_E enFmt) {
	char *ext = get_ext_name((char *)(filename));

	if (strcmp(ext, "bmp") == 0) {
		if (0 != load_bmp_ex(filename, pVideoLogo, enFmt)) {
			printf("OSD_LoadBMP error!\n");
			return -1;
		}
	} else {
		printf("not supported image file!\n");
		return -1;
	}

	return 0;
}

RK_S32 create_surface_by_bitmap(const char *pstFileName, OSD_SURFACE_S *pstSurface,
                                RK_U8 *pu8Virt) {
	OSD_LOGO_T stLogo;
	stLogo.pRGBBuffer = pu8Virt;
	if (load_image_ex(pstFileName, &stLogo, pstSurface->enColorFmt) < 0) {
		printf("load bmp error!\n");
		return -1;
	}

	pstSurface->u16Height = stLogo.height;
	pstSurface->u16Width = stLogo.width;
	pstSurface->u16Stride = stLogo.stride;

	return 0;
}
