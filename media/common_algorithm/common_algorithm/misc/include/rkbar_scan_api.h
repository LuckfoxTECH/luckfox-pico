// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef _RKBAR_SCAN_API_H_
#define _RKBAR_SCAN_API_H_
#include <stdint.h>
#include <stdio.h>
#include <string.h>
typedef struct image_s {
	unsigned width, height;  /* image size */
	void *data;              /* image sample data */
	unsigned long datalen;   /* allocated/mapped size of data */
	unsigned crop_x, crop_y; /* crop rectangle */
	unsigned crop_w, crop_h;
	void *userdata; /* user specified data associated w/image */
	uint8_t *bin;
	uint8_t *tmp;
} image_t;
#ifdef __cplusplus
extern "C" {
#endif
int rkbar_init(void **handle);
int rkbar_scan(void *handle, image_t *src);
const char *rkbar_getresult(void *handle);
void rkbar_deinit(void *handle);
#ifdef __cplusplus
}
#endif

#endif
