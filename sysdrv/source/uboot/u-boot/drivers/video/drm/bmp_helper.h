/*
 * (C) Copyright 2008-2017 Fuzhou Rockchip Electronics Co., Ltd
 * Author: Mark Yao <mark.yao@rock-chips.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _BMP_HELPER_H_
#define _BMP_HELPER_H_

#define BMP_RLE8_ESCAPE		0
#define BMP_RLE8_EOL		0
#define BMP_RLE8_EOBMP		1
#define BMP_RLE8_DELTA		2

#define range(x, min, max) ((x) < (min)) ? (min) : (((x) > (max)) ? (max) : (x))

int bmpdecoder(void *bmp_addr, void *dst, int dst_bpp);
#endif /* _BMP_HELPER_H_ */
