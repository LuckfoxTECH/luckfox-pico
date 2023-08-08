// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _RK_BMP_READER_H_
#define _RK_BMP_READER_H_

#include <stdint.h>

#include "common.h"
#include "osd_common.h"

typedef struct tagBITMAPFILEHEADER {
	uint32_t bfSize;
	uint16_t bfReserved1;
	uint16_t bfReserved2;
	uint32_t bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
	uint32_t biSize;
	int32_t biWidth;
	int32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	int32_t biXPelsPerMeter;
	int32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagRGBQUAD {
	uint8_t rgbReserved;
	uint8_t rgbRed;
	uint8_t rgbGreen;
	uint8_t rgbBlue;
} RGBQUAD;

int load_bmp_form_file(osd_data_s *data);
void save_argb8888_to_bmp(unsigned char *buffer, int buf_w, int buf_h);

#endif