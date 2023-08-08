// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "bmp_reader.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "bmp_reader.c"

#define WIDTHBYTES(bits) ((((bits) + 31) >> 5) * 4)

void bmp24_to_argb8888(BITMAPINFOHEADER bitInfoHead, RGBQUAD *argb_8888_data, uint8_t *bmp_data) {
	LOG_INFO("bmp24_to_argb8888\n");
	int k;
	int index = 0;
	int width = bitInfoHead.biWidth;
	int height = bitInfoHead.biHeight;
	int pitch = WIDTHBYTES(width * bitInfoHead.biBitCount);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			k = (height - i - 1) * pitch + j * 3;
			argb_8888_data[index].rgbBlue = bmp_data[k];
			argb_8888_data[index].rgbGreen = bmp_data[k + 1];
			argb_8888_data[index].rgbRed = bmp_data[k + 2];
			if ((argb_8888_data[index].rgbBlue == 0x08) &&
			    (argb_8888_data[index].rgbGreen == 0x00) && (argb_8888_data[index].rgbRed == 0x00))
				argb_8888_data[index].rgbReserved = 0X00;
			else
				argb_8888_data[index].rgbReserved = 0Xff;
			index++;
		}
	}
}

void bmp32_to_argb8888(BITMAPINFOHEADER bitInfoHead, RGBQUAD *argb_8888_data, uint8_t *bmp_data) {
	LOG_INFO("bmp32_to_argb8888\n");
	int k;
	int index = 0;
	int width = bitInfoHead.biWidth;
	int height = bitInfoHead.biHeight;
	int pitch = WIDTHBYTES(width * bitInfoHead.biBitCount);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			k = (height - i - 1) * pitch + j * 4;
			argb_8888_data[index].rgbBlue = bmp_data[k];
			argb_8888_data[index].rgbGreen = bmp_data[k + 1];
			argb_8888_data[index].rgbRed = bmp_data[k + 2];
			argb_8888_data[index].rgbReserved = bmp_data[k + 3];
			index++;
		}
	}
}

int bmp_check(FILE *pfile, BITMAPFILEHEADER *bitHead, BITMAPINFOHEADER *bitInfoHead) {
	LOG_INFO("bmp_check\n");
	uint16_t fileType;
	fread(&fileType, 1, sizeof(uint16_t), pfile);
	if (fileType != 0x4d42) {
		LOG_ERROR("file is not .bmp file!");
		return -1;
	}

	fread(bitHead, 1, sizeof(BITMAPFILEHEADER), pfile);
	fread(bitInfoHead, 1, sizeof(BITMAPINFOHEADER), pfile);

	if (bitInfoHead->biBitCount < 24) {
		LOG_ERROR("bmp must be 24/32 depth!");
		return -1;
	}
	LOG_INFO("bitInfoHead->biBitCount is %d\n", bitInfoHead->biBitCount);

	return 0;
}

int load_bmp_form_file(osd_data_s *data) {
	BITMAPFILEHEADER bitHead;
	BITMAPINFOHEADER bitInfoHead;
	uint8_t *bmp_data;
	LOG_INFO("load_bmp_form_file\n");
	FILE *pfile = fopen(data->image, "rb");
	if (pfile == NULL) {
		printf("Bmp file open fail!\n");
		return -1;
	}
	// read head
	if (bmp_check(pfile, &bitHead, &bitInfoHead))
		return -1;
	// read data
	LOG_INFO("ReadBmpData\n");
	int width = bitInfoHead.biWidth;
	int height = bitInfoHead.biHeight;
	int pitch = WIDTHBYTES(width * bitInfoHead.biBitCount);
	long dataSize = pitch * height;
	bmp_data = (uint8_t *)malloc(dataSize);
	if (bmp_data == NULL) {
		printf("bmp_data malloc fail!\n");
		return -1;
	}
	if (fread(bmp_data, 1, dataSize, pfile) <= 0) {
		printf("bmp_data fread fail!\n");
		return -1;
	}

	data->buffer = malloc(bitInfoHead.biWidth * bitInfoHead.biHeight * sizeof(RGBQUAD));
	memset(data->buffer, 0, bitInfoHead.biWidth * bitInfoHead.biHeight * sizeof(RGBQUAD));
	if (data->buffer == NULL) {
		printf("data->buffer malloc fail!\n");
		return -1;
	}

	if (bitInfoHead.biBitCount == 24) {
		bmp24_to_argb8888(bitInfoHead, (RGBQUAD *)data->buffer, bmp_data);
	} else {
		bmp32_to_argb8888(bitInfoHead, (RGBQUAD *)data->buffer, bmp_data);
	}

	data->width = bitInfoHead.biWidth;
	data->height = bitInfoHead.biHeight;
	data->size = data->width * data->height * sizeof(RGBQUAD);

	LOG_INFO("FreeBmpData\n");
	fclose(pfile);
	if (bmp_data) {
		free(bmp_data);
		bmp_data = NULL;
	}

	return 0;
}

void save_argb8888_to_bmp(unsigned char *buffer, int buf_w, int buf_h) {

	int filesize = 54 + 4 * buf_w * buf_h;
	LOG_DEBUG("buf_w is %d, buf_h is %d, filesize is %d\n", buf_w, buf_h, filesize);
	FILE *f;
	unsigned char bmpfileheader[14] = {'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
	unsigned char bmpinfoheader[40] = {40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 32, 0};

	bmpfileheader[2] = (unsigned char)(filesize);
	bmpfileheader[3] = (unsigned char)(filesize >> 8);
	bmpfileheader[4] = (unsigned char)(filesize >> 16);
	bmpfileheader[5] = (unsigned char)(filesize >> 24);

	bmpinfoheader[4] = (unsigned char)(buf_w);
	bmpinfoheader[5] = (unsigned char)(buf_w >> 8);
	bmpinfoheader[6] = (unsigned char)(buf_w >> 16);
	bmpinfoheader[7] = (unsigned char)(buf_w >> 24);
	bmpinfoheader[8] = (unsigned char)(buf_h);
	bmpinfoheader[9] = (unsigned char)(buf_h >> 8);
	bmpinfoheader[10] = (unsigned char)(buf_h >> 16);
	bmpinfoheader[11] = (unsigned char)(buf_h >> 24);

	f = fopen("/tmp/tmp.bmp", "wb");
	fwrite(bmpfileheader, 1, 14, f);
	fwrite(bmpinfoheader, 1, 40, f);
	for (int k = 0; k < buf_h; k++) {
		fwrite(buffer + (buf_w * (buf_h - k - 1) * 4), 4, buf_w, f);
	}
	fclose(f);
}