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
 */

#include "sample_comm.h"
#include <stdlib.h>
//#include "test_comm_imgproc.h"
//#include "test_comm_utils.h"

#define RK_CLIP3(l, h, a) ((a) < (l) ? (l) : ((a) > (h) ? (h) : (a)))

static void get_rgb_color(RK_U32 *R, RK_U32 *G, RK_U32 *B, RK_S32 x, RK_S32 y,
                          RK_S32 frm_cnt) {
	// moving color bar
	RK_U8 Y = (0 + x + y + frm_cnt * 3);
	RK_U8 U = (128 + (y / 2) + frm_cnt * 2);
	RK_U8 V = (64 + (x / 2) + frm_cnt * 5);

	RK_S32 _R = Y + ((360 * (V - 128)) >> 8);
	RK_S32 _G = Y - (((88 * (U - 128) + 184 * (V - 128))) >> 8);
	RK_S32 _B = Y + ((455 * (U - 128)) >> 8);

	R[0] = RK_CLIP3(0, 255, _R);
	G[0] = RK_CLIP3(0, 255, _G);
	B[0] = RK_CLIP3(0, 255, _B);
}

static void fill_MPP_FMT_RGB565(RK_U8 *p, RK_U32 R, RK_U32 G, RK_U32 B, RK_U32 be) {
	// MPP_FMT_RGB565 = ffmpeg: rgb565be
	// 16 bit pixel     MSB  -------->  LSB
	//                 (rrrr,rggg,gggb,bbbb)
	// big    endian   |  byte 0 |  byte 1 |
	// little endian   |  byte 1 |  byte 0 |
	RK_U16 val =
	    (((R >> 3) & 0x1f) << 11) | (((G >> 2) & 0x3f) << 5) | (((B >> 3) & 0x1f) << 0);
	if (be) {
		p[0] = (val >> 8) & 0xff;
		p[1] = (val >> 0) & 0xff;
	} else {
		p[0] = (val >> 0) & 0xff;
		p[1] = (val >> 8) & 0xff;
	}
}

static void fill_MPP_FMT_BGR565(RK_U8 *p, RK_U32 R, RK_U32 G, RK_U32 B, RK_U32 be) {
	// MPP_FMT_BGR565 = ffmpeg: bgr565be
	// 16 bit pixel     MSB  -------->  LSB
	//                 (bbbb,bggg,gggr,rrrr)
	// big    endian   |  byte 0 |  byte 1 |
	// little endian   |  byte 1 |  byte 0 |
	RK_U16 val =
	    (((R >> 3) & 0x1f) << 0) | (((G >> 2) & 0x3f) << 5) | (((B >> 3) & 0x1f) << 11);
	if (be) {
		p[0] = (val >> 8) & 0xff;
		p[1] = (val >> 0) & 0xff;
	} else {
		p[0] = (val >> 0) & 0xff;
		p[1] = (val >> 8) & 0xff;
	}
}

static void fill_MPP_FMT_RGB555(RK_U8 *p, RK_U32 R, RK_U32 G, RK_U32 B, RK_U32 be) {
	// MPP_FMT_RGB555 = ffmpeg: rgb555be
	// 16 bit pixel     MSB  -------->  LSB
	//                 (0rrr,rrgg,gggb,bbbb)
	// big    endian   |  byte 0 |  byte 1 |
	// little endian   |  byte 1 |  byte 0 |
	RK_U16 val =
	    (((R >> 3) & 0x1f) << 10) | (((G >> 3) & 0x1f) << 5) | (((B >> 3) & 0x1f) << 0);
	if (be) {
		p[0] = (val >> 8) & 0xff;
		p[1] = (val >> 0) & 0xff;
	} else {
		p[0] = (val >> 0) & 0xff;
		p[1] = (val >> 8) & 0xff;
	}
}

static void fill_MPP_FMT_BGR555(RK_U8 *p, RK_U32 R, RK_U32 G, RK_U32 B, RK_U32 be) {
	// MPP_FMT_BGR555 = ffmpeg: bgr555be
	// 16 bit pixel     MSB  -------->  LSB
	//                 (0bbb,bbgg,gggr,rrrr)
	// big    endian   |  byte 0 |  byte 1 |
	// little endian   |  byte 1 |  byte 0 |
	RK_U16 val =
	    (((R >> 3) & 0x1f) << 0) | (((G >> 3) & 0x1f) << 5) | (((B >> 3) & 0x1f) << 10);
	if (be) {
		p[0] = (val >> 8) & 0xff;
		p[1] = (val >> 0) & 0xff;
	} else {
		p[0] = (val >> 0) & 0xff;
		p[1] = (val >> 8) & 0xff;
	}
}

static void fill_MPP_FMT_RGB444(RK_U8 *p, RK_U32 R, RK_U32 G, RK_U32 B, RK_U32 be) {
	// MPP_FMT_RGB444 = ffmpeg: rgb444be
	// 16 bit pixel     MSB  -------->  LSB
	//                 (0000,rrrr,gggg,bbbb)
	// big    endian   |  byte 0 |  byte 1 |
	// little endian   |  byte 1 |  byte 0 |
	RK_U16 val =
	    (((R >> 4) & 0xf) << 8) | (((G >> 4) & 0xf) << 4) | (((B >> 4) & 0xf) << 0);
	if (be) {
		p[0] = (val >> 8) & 0xff;
		p[1] = (val >> 0) & 0xff;
	} else {
		p[0] = (val >> 0) & 0xff;
		p[1] = (val >> 8) & 0xff;
	}
}

static void fill_MPP_FMT_BGR444(RK_U8 *p, RK_U32 R, RK_U32 G, RK_U32 B, RK_U32 be) {
	// MPP_FMT_BGR444 = ffmpeg: bgr444be
	// 16 bit pixel     MSB  -------->  LSB
	//                 (0000,bbbb,gggg,rrrr)
	// big    endian   |  byte 0 |  byte 1 |
	// little endian   |  byte 1 |  byte 0 |
	RK_U16 val =
	    (((R >> 4) & 0xf) << 0) | (((G >> 4) & 0xf) << 4) | (((B >> 4) & 0xf) << 8);
	if (be) {
		p[0] = (val >> 8) & 0xff;
		p[1] = (val >> 0) & 0xff;
	} else {
		p[0] = (val >> 0) & 0xff;
		p[1] = (val >> 8) & 0xff;
	}
}

static void fill_MPP_FMT_RGB888(RK_U8 *p, RK_U32 R, RK_U32 G, RK_U32 B, RK_U32 be) {
	// MPP_FMT_RGB888
	// 24 bit pixel     MSB  -------->  LSB
	//                 (rrrr,rrrr,gggg,gggg,bbbb,bbbb)
	// big    endian   |  byte 0 |  byte 1 |  byte 2 |
	// little endian   |  byte 2 |  byte 1 |  byte 0 |
	if (be) {
		p[0] = R;
		p[1] = G;
		p[2] = B;
	} else {
		p[0] = B;
		p[1] = G;
		p[2] = R;
	}
}

static void fill_MPP_FMT_BGR888(RK_U8 *p, RK_U32 R, RK_U32 G, RK_U32 B, RK_U32 be) {
	// MPP_FMT_BGR888
	// 24 bit pixel     MSB  -------->  LSB
	//                 (bbbb,bbbb,gggg,gggg,rrrr,rrrr)
	// big    endian   |  byte 0 |  byte 1 |  byte 2 |
	// little endian   |  byte 2 |  byte 1 |  byte 0 |
	if (be) {
		p[0] = B;
		p[1] = G;
		p[2] = R;
	} else {
		p[0] = R;
		p[1] = G;
		p[2] = B;
	}
}

static void fill_MPP_FMT_RGB101010(RK_U8 *p, RK_U32 R, RK_U32 G, RK_U32 B, RK_U32 be) {
	// MPP_FMT_RGB101010
	// 32 bit pixel     MSB  -------->  LSB
	//                 (00rr,rrrr,rrrr,gggg,gggg,ggbb,bbbb,bbbb)
	// big    endian   |  byte 0 |  byte 1 |  byte 2 |  byte 3 |
	// little endian   |  byte 3 |  byte 2 |  byte 1 |  byte 0 |
	RK_U32 val =
	    (((R * 4) & 0x3ff) << 20) | (((G * 4) & 0x3ff) << 10) | (((B * 4) & 0x3ff) << 0);
	if (be) {
		p[0] = (val >> 24) & 0xff;
		p[1] = (val >> 16) & 0xff;
		p[2] = (val >> 8) & 0xff;
		p[3] = (val >> 0) & 0xff;
	} else {
		p[0] = (val >> 0) & 0xff;
		p[1] = (val >> 8) & 0xff;
		p[2] = (val >> 16) & 0xff;
		p[3] = (val >> 24) & 0xff;
	}
}

static void fill_MPP_FMT_BGR101010(RK_U8 *p, RK_U32 R, RK_U32 G, RK_U32 B, RK_U32 be) {
	// MPP_FMT_BGR101010
	// 32 bit pixel     MSB  -------->  LSB
	//                 (00bb,bbbb,bbbb,gggg,gggg,ggrr,rrrr,rrrr)
	// big    endian   |  byte 0 |  byte 1 |  byte 2 |  byte 3 |
	// little endian   |  byte 3 |  byte 2 |  byte 1 |  byte 0 |
	RK_U32 val =
	    (((R * 4) & 0x3ff) << 0) | (((G * 4) & 0x3ff) << 10) | (((B * 4) & 0x3ff) << 20);
	if (be) {
		p[0] = (val >> 24) & 0xff;
		p[1] = (val >> 16) & 0xff;
		p[2] = (val >> 8) & 0xff;
		p[3] = (val >> 0) & 0xff;
	} else {
		p[0] = (val >> 0) & 0xff;
		p[1] = (val >> 8) & 0xff;
		p[2] = (val >> 16) & 0xff;
		p[3] = (val >> 24) & 0xff;
	}
}

static void fill_MPP_FMT_ARGB8888(RK_U8 *p, RK_U32 R, RK_U32 G, RK_U32 B, RK_U32 be) {
	// MPP_FMT_ARGB8888
	// 32 bit pixel     MSB  -------->  LSB
	//                 (XXXX,XXXX,rrrr,rrrr,gggg,gggg,bbbb,bbbb)
	// big    endian   |  byte 0 |  byte 1 |  byte 2 |  byte 3 |
	// little endian   |  byte 3 |  byte 2 |  byte 1 |  byte 0 |
	if (be) {
		p[0] = 0xff;
		p[1] = R;
		p[2] = G;
		p[3] = B;
	} else {
		p[0] = B;
		p[1] = G;
		p[2] = R;
		p[3] = 0xff;
	}
}

static void fill_MPP_FMT_ABGR8888(RK_U8 *p, RK_U32 R, RK_U32 G, RK_U32 B, RK_U32 be) {
	// MPP_FMT_ABGR8888
	// 32 bit pixel     MSB  -------->  LSB
	//                 (XXXX,XXXX,bbbb,bbbb,gggg,gggg,rrrr,rrrr)
	// big    endian   |  byte 0 |  byte 1 |  byte 2 |  byte 3 |
	// little endian   |  byte 3 |  byte 2 |  byte 1 |  byte 0 |
	if (be) {
		p[0] = 0xff;
		p[1] = B;
		p[2] = G;
		p[3] = R;
	} else {
		p[0] = R;
		p[1] = G;
		p[2] = B;
		p[3] = 0xff;
	}
}

static void fill_MPP_FMT_BGRA8888(RK_U8 *p, RK_U32 R, RK_U32 G, RK_U32 B, RK_U32 be) {
	// MPP_FMT_BGRA8888
	// 32 bit pixel     MSB  -------->  LSB
	//                 (bbbb,bbbb,gggg,gggg,rrrr,rrrr,XXXX,XXXX)
	// big    endian   |  byte 0 |  byte 1 |  byte 2 |  byte 3 |
	// little endian   |  byte 3 |  byte 2 |  byte 1 |  byte 0 |
	if (be) {
		p[0] = B;
		p[1] = G;
		p[2] = R;
		p[3] = 0xff;
	} else {
		p[0] = 0xff;
		p[1] = R;
		p[2] = G;
		p[3] = B;
	}
}

static void fill_MPP_FMT_RGBA8888(RK_U8 *p, RK_U32 R, RK_U32 G, RK_U32 B, RK_U32 be) {
	// MPP_FMT_RGBA8888
	// 32 bit pixel     MSB  -------->  LSB
	//                 (rrrr,rrrr,gggg,gggg,bbbb,bbbb,XXXX,XXXX)
	// big    endian   |  byte 0 |  byte 1 |  byte 2 |  byte 3 |
	// little endian   |  byte 3 |  byte 2 |  byte 1 |  byte 0 |
	if (be) {
		p[0] = R;
		p[1] = G;
		p[2] = B;
		p[3] = 0xff;
	} else {
		p[0] = 0xff;
		p[1] = B;
		p[2] = G;
		p[3] = R;
	}
}

static void fill_MPP_FMT_ARGB1555(RK_U8 *p, RK_U32 R, RK_U32 G, RK_U32 B, RK_U32 be) {
	RK_U8 r1, g1, b1, a1;
	// RK_U16 *u16p = reinterpret_cast<RK_U16 *>(p);
	RK_U16 *u16p = p;

	a1 = 1;
	r1 = g1 = b1 = 0;
	r1 = R >> 3;
	g1 = G >> 3;
	b1 = B >> 3;

	if (be) {
		*u16p = a1 | (r1 << 1) | (g1 << 6) | (b1 << 11);
	} else {
		*u16p = (a1 << 15) + (r1 << 10) | (g1 << 5) | b1;
	}
}

static void fill_MPP_FMT_ABGR1555(RK_U8 *p, RK_U32 R, RK_U32 G, RK_U32 B, RK_U32 be) {
	RK_U8 r1, g1, b1, a1;
	// RK_U16 *u16p = reinterpret_cast<RK_U16 *>(p);
	RK_U16 *u16p = p;

	a1 = 1;
	r1 = g1 = b1 = 0;
	r1 = R >> 3;
	g1 = G >> 3;
	b1 = B >> 3;

	if (be) {
		*u16p = a1 | (b1 << 1) | (g1 << 6) | (r1 << 11);
	} else {
		*u16p = (a1 << 15) + (b1 << 10) | (g1 << 5) | r1;
	}
}

static void fill_MPP_FMT_ARGB4444(RK_U8 *p, RK_U32 R, RK_U32 G, RK_U32 B, RK_U32 be) {
	RK_U16 val = (0xf << 12) | (((R >> 4) & 0xf) << 8) | (((G >> 4) & 0xf) << 4) |
	             (((B >> 4) & 0xf) << 0);
	if (be) {
		p[0] = (val >> 8) & 0xff;
		p[1] = (val >> 0) & 0xff;
	} else {
		p[0] = (val >> 0) & 0xff;
		p[1] = (val >> 8) & 0xff;
	}
}

static void fill_MPP_FMT_ABGR4444(RK_U8 *p, RK_U32 R, RK_U32 G, RK_U32 B, RK_U32 be) {
	RK_U16 val = (((R >> 4) & 0xf) << 0) | (((G >> 4) & 0xf) << 4) |
	             (((B >> 4) & 0xf) << 8) | (0xf << 12);
	if (be) {
		p[0] = (val >> 8) & 0xff;
		p[1] = (val >> 0) & 0xff;
	} else {
		p[0] = (val >> 0) & 0xff;
		p[1] = (val >> 8) & 0xff;
	}
}

static void fill_MPP_FMT_BGRA4444(RK_U8 *p, RK_U32 R, RK_U32 G, RK_U32 B, RK_U32 be) {
	RK_U8 r1, g1, b1, a1;
	// RK_U16 *u16p = reinterpret_cast<RK_U16 *>(p);
	RK_U16 *u16p = p;

	a1 = 0xf;
	r1 = g1 = b1 = 0;
	r1 = R >> 4;
	g1 = G >> 4;
	b1 = B >> 4;

	if (be) {
		*u16p = b1 | (g1 << 4) | (r1 << 8) | (a1 << 12);
	} else {
		*u16p = (b1 << 12) + (g1 << 8) | (r1 << 4) | a1;
	}
}

static void fill_MPP_FMT_RGBA5551(RK_U8 *p, RK_U32 R, RK_U32 G, RK_U32 B, RK_U32 be) {
	RK_U8 r1, g1, b1, a1;
	// RK_U16 *u16p = reinterpret_cast<RK_U16 *>(p);
	RK_U16 *u16p = p;

	a1 = 1;
	r1 = g1 = b1 = 0;
	r1 = R >> 3;
	g1 = G >> 3;
	b1 = B >> 3;

	if (be) {
		*u16p = r1 | (g1 << 5) | (b1 << 10) | (a1 << 15);
	} else {
		*u16p = (r1 << 11) | (g1 << 6) | (b1 << 1) | a1;
	}
}

static void fill_MPP_FMT_BGRA5551(RK_U8 *p, RK_U32 R, RK_U32 G, RK_U32 B, RK_U32 be) {
	RK_U8 r1, g1, b1, a1;
	// RK_U16 *u16p = reinterpret_cast<RK_U16 *>(p);
	RK_U16 *u16p = p;

	a1 = 1;
	r1 = g1 = b1 = 0;
	r1 = R >> 3;
	g1 = G >> 3;
	b1 = B >> 3;

	if (be) {
		*u16p = b1 | (g1 << 5) | (r1 << 10) | (a1 << 15);
	} else {
		*u16p = (b1 << 11) + (g1 << 6) | (r1 << 1) | a1;
	}
}

typedef void (*FillRgbFunc)(RK_U8 *p, RK_U32 R, RK_U32 G, RK_U32 B, RK_U32 be);

FillRgbFunc fill_rgb_funcs[] = {fill_MPP_FMT_RGB565,
                                fill_MPP_FMT_BGR565,
                                fill_MPP_FMT_RGB555,
                                fill_MPP_FMT_BGR555,
                                fill_MPP_FMT_RGB444,
                                fill_MPP_FMT_BGR444,
                                fill_MPP_FMT_RGB888,
                                fill_MPP_FMT_BGR888,
                                fill_MPP_FMT_RGB101010,
                                fill_MPP_FMT_BGR101010,
                                fill_MPP_FMT_ARGB1555,
                                fill_MPP_FMT_ABGR1555,
                                fill_MPP_FMT_ARGB4444,
                                fill_MPP_FMT_ABGR4444,
                                RK_NULL,
                                RK_NULL,
                                fill_MPP_FMT_ARGB8888,
                                fill_MPP_FMT_ABGR8888,
                                fill_MPP_FMT_BGRA8888,
                                fill_MPP_FMT_RGBA8888,
                                fill_MPP_FMT_RGBA5551,
                                fill_MPP_FMT_BGRA5551,
                                fill_MPP_FMT_BGRA4444};

static RK_S32 util_check_stride_by_pixel(RK_S32 workaround, RK_S32 width,
                                         RK_S32 hor_stride, RK_S32 pixel_size) {
	if (!workaround && hor_stride < width * pixel_size) {
		RK_LOGW("warning: stride by bytes %d is smarller than width %d mutiple by pixel "
		        "size %d",
		        hor_stride, width, pixel_size);
		RK_LOGW("multiple stride %d by pixel size %d and set new byte stride to %d",
		        hor_stride, pixel_size, hor_stride * pixel_size);
		workaround = 1;
	}

	return workaround;
}

static RK_S32 util_check_8_pixel_aligned(RK_S32 workaround, RK_S32 hor_stride,
                                         RK_S32 pixel_aign, RK_S32 pixel_size,
                                         const char *fmt_name) {
	if (!workaround && hor_stride != RK_ALIGN(hor_stride, pixel_aign * pixel_size)) {
		RK_LOGW("warning: vepu only support 8 aligned horizontal stride in pixel for %s "
		        "with pixel size %d",
		        fmt_name, pixel_size);
		RK_LOGW("set byte stride to %d to match the requirement",
		        RK_ALIGN(hor_stride, pixel_aign * pixel_size));
		workaround = 1;
	}

	return workaround;
}

RK_S32 SAMPLE_COMM_FillImage(RK_U8 *buf, RK_U32 width, RK_U32 height, RK_U32 hor_stride,
                             RK_U32 ver_stride, PIXEL_FORMAT_E fmt, RK_U32 frame_count) {
	RK_S32 ret = RK_SUCCESS;
	RK_U8 *buf_y = buf;
	RK_U8 *buf_c = buf + hor_stride * ver_stride;
	RK_U32 x, y, i;
	static RK_S32 is_pixel_stride = 0;
	static RK_S32 not_8_pixel = 0;

	switch (fmt) {
	case RK_FMT_YUV420SP: {
		RK_U8 *p = buf_y;

		for (y = 0; y < height; y++, p += hor_stride) {
			for (x = 0; x < width; x++) {
				p[x] = x + y + frame_count * 3;
			}
		}

		p = buf + hor_stride * ver_stride;
		for (y = 0; y < height / 2; y++, p += hor_stride) {
			for (x = 0; x < width / 2; x++) {
				p[x * 2 + 0] = 128 + y + frame_count * 2;
				p[x * 2 + 1] = 64 + x + frame_count * 5;
			}
		}
	} break;
	case RK_FMT_YUV422SP: {
		RK_U8 *p = buf_y;

		for (y = 0; y < height; y++, p += hor_stride) {
			for (x = 0; x < width; x++) {
				p[x] = x + y + frame_count * 3;
			}
		}

		p = buf + hor_stride * ver_stride;
		for (y = 0; y < height; y++, p += hor_stride) {
			for (x = 0; x < width / 2; x++) {
				p[x * 2 + 0] = 128 + y / 2 + frame_count * 2;
				p[x * 2 + 1] = 64 + x + frame_count * 5;
			}
		}
	} break;
	case RK_FMT_YUV420P: {
		RK_U8 *p = buf_y;

		for (y = 0; y < height; y++, p += hor_stride) {
			for (x = 0; x < width; x++) {
				p[x] = x + y + frame_count * 3;
			}
		}

		p = buf_c;
		for (y = 0; y < height / 2; y++, p += hor_stride / 2) {
			for (x = 0; x < width / 2; x++) {
				p[x] = 128 + y + frame_count * 2;
			}
		}

		p = buf_c + hor_stride * ver_stride / 4;
		for (y = 0; y < height / 2; y++, p += hor_stride / 2) {
			for (x = 0; x < width / 2; x++) {
				p[x] = 64 + x + frame_count * 5;
			}
		}
	} break;
	case RK_FMT_YUV420SP_VU: {
		RK_U8 *p = buf_y;

		for (y = 0; y < height; y++, p += hor_stride) {
			for (x = 0; x < width; x++) {
				p[x] = x + y + frame_count * 3;
			}
		}

		p = buf_c;
		for (y = 0; y < height / 2; y++, p += hor_stride) {
			for (x = 0; x < width / 2; x++) {
				p[x * 2 + 1] = 128 + y + frame_count * 2;
				p[x * 2 + 0] = 64 + x + frame_count * 5;
			}
		}
	} break;
	case RK_FMT_YUV422P: {
		RK_U8 *p = buf_y;

		for (y = 0; y < height; y++, p += hor_stride) {
			for (x = 0; x < width; x++) {
				p[x] = x + y + frame_count * 3;
			}
		}

		p = buf_c;
		for (y = 0; y < height; y++, p += hor_stride / 2) {
			for (x = 0; x < width / 2; x++) {
				p[x] = 128 + y / 2 + frame_count * 2;
			}
		}

		p = buf_c + hor_stride * ver_stride / 2;
		for (y = 0; y < height; y++, p += hor_stride / 2) {
			for (x = 0; x < width / 2; x++) {
				p[x] = 64 + x + frame_count * 5;
			}
		}
	} break;
	case RK_FMT_YUV422SP_VU: {
		RK_U8 *p = buf_y;

		for (y = 0; y < height; y++, p += hor_stride) {
			for (x = 0; x < width; x++) {
				p[x] = x + y + frame_count * 3;
			}
		}

		p = buf_c;
		for (y = 0; y < height; y++, p += hor_stride) {
			for (x = 0; x < width / 2; x++) {
				p[x * 2 + 1] = 128 + y / 2 + frame_count * 2;
				p[x * 2 + 0] = 64 + x + frame_count * 5;
			}
		}
	} break;
	case RK_FMT_YUV422_YUYV: {
		RK_U8 *p = buf_y;

		for (y = 0; y < height; y++, p += hor_stride * 2) {
			for (x = 0; x < width / 2; x++) {
				p[x * 4 + 0] = x * 2 + 0 + y + frame_count * 3;
				p[x * 4 + 2] = x * 2 + 1 + y + frame_count * 3;
				p[x * 4 + 1] = 128 + y / 2 + frame_count * 2;
				p[x * 4 + 3] = 64 + x + frame_count * 5;
			}
		}
	} break;
	case RK_FMT_YUV422_YVYU: {
		RK_U8 *p = buf_y;

		for (y = 0; y < height; y++, p += hor_stride * 2) {
			for (x = 0; x < width / 2; x++) {
				p[x * 4 + 0] = x * 2 + 0 + y + frame_count * 3;
				p[x * 4 + 2] = x * 2 + 1 + y + frame_count * 3;
				p[x * 4 + 3] = 128 + y / 2 + frame_count * 2;
				p[x * 4 + 1] = 64 + x + frame_count * 5;
			}
		}
	} break;
	case RK_FMT_YUV422_UYVY: {
		RK_U8 *p = buf_y;

		for (y = 0; y < height; y++, p += hor_stride * 2) {
			for (x = 0; x < width / 2; x++) {
				p[x * 4 + 1] = x * 2 + 0 + y + frame_count * 3;
				p[x * 4 + 3] = x * 2 + 1 + y + frame_count * 3;
				p[x * 4 + 0] = 128 + y / 2 + frame_count * 2;
				p[x * 4 + 2] = 64 + x + frame_count * 5;
			}
		}
	} break;
	case RK_FMT_YUV422_VYUY: {
		RK_U8 *p = buf_y;

		for (y = 0; y < height; y++, p += hor_stride * 2) {
			for (x = 0; x < width / 2; x++) {
				p[x * 4 + 1] = x * 2 + 0 + y + frame_count * 3;
				p[x * 4 + 3] = x * 2 + 1 + y + frame_count * 3;
				p[x * 4 + 2] = 128 + y / 2 + frame_count * 2;
				p[x * 4 + 0] = 64 + x + frame_count * 5;
			}
		}
	} break;
	case RK_FMT_YUV400SP: {
		RK_U8 *p = buf_y;

		for (y = 0; y < height; y++, p += hor_stride) {
			for (x = 0; x < width; x++) {
				p[x] = x + y + frame_count * 3;
			}
		}
	} break;
	case RK_FMT_RGB565:
	case RK_FMT_BGR565:
	case RK_FMT_RGB555:
	case RK_FMT_BGR555:
	case RK_FMT_RGB444:
	case RK_FMT_BGR444:
	case RK_FMT_ARGB1555:
	case RK_FMT_ABGR1555:
	case RK_FMT_RGBA5551:
	case RK_FMT_BGRA5551:
	case RK_FMT_ARGB4444:
	case RK_FMT_ABGR4444:
	case RK_FMT_BGRA4444: {
		RK_U8 *p = buf_y;
		RK_U32 pix_w = 2;
		FillRgbFunc fill = fill_rgb_funcs[fmt - RK_VIDEO_FMT_RGB];

		if (util_check_stride_by_pixel(is_pixel_stride, width, hor_stride, pix_w)) {
			hor_stride *= pix_w;
			is_pixel_stride = 1;
		}

		if (util_check_8_pixel_aligned(not_8_pixel, hor_stride, 8, pix_w, "16bit RGB")) {
			hor_stride = RK_ALIGN(hor_stride, 16);
			not_8_pixel = 1;
		}

		for (y = 0; y < height; y++, p += hor_stride) {
			for (x = 0, i = 0; x < width; x++, i += pix_w) {
				RK_U32 R, G, B;

				get_rgb_color(&R, &G, &B, x, y, frame_count);
				fill(p + i, R, G, B, 1);
			}
		}
	} break;
	case RK_FMT_RGB101010:
	case RK_FMT_BGR101010:
	case RK_FMT_ARGB8888:
	case RK_FMT_ABGR8888:
	case RK_FMT_BGRA8888:
	case RK_FMT_RGBA8888: {
		RK_U8 *p = buf_y;
		RK_U32 pix_w = 4;
		FillRgbFunc fill = fill_rgb_funcs[fmt - RK_VIDEO_FMT_RGB];

		if (util_check_stride_by_pixel(is_pixel_stride, width, hor_stride, pix_w)) {
			hor_stride *= pix_w;
			is_pixel_stride = 1;
		}

		if (util_check_8_pixel_aligned(not_8_pixel, hor_stride, 8, pix_w, "32bit RGB")) {
			hor_stride = RK_ALIGN(hor_stride, 32);
			not_8_pixel = 1;
		}

		for (y = 0; y < height; y++, p += hor_stride) {
			for (x = 0, i = 0; x < width; x++, i += pix_w) {
				RK_U32 R, G, B;

				get_rgb_color(&R, &G, &B, x, y, frame_count);
				fill(p + i, R, G, B, 1);
			}
		}
	} break;
	case RK_FMT_BGR888:
	case RK_FMT_RGB888: {
		RK_U8 *p = buf_y;
		RK_U32 pix_w = 3;
		FillRgbFunc fill = fill_rgb_funcs[fmt - RK_VIDEO_FMT_RGB];

		if (util_check_stride_by_pixel(is_pixel_stride, width, hor_stride, pix_w)) {
			hor_stride *= pix_w;
			is_pixel_stride = 1;
		}

		if (util_check_8_pixel_aligned(not_8_pixel, hor_stride, 8, pix_w, "24bit RGB")) {
			hor_stride = RK_ALIGN(hor_stride, 24);
			not_8_pixel = 1;
		}

		for (y = 0; y < height; y++, p += hor_stride) {
			for (x = 0, i = 0; x < width; x++, i += pix_w) {
				RK_U32 R, G, B;

				get_rgb_color(&R, &G, &B, x, y, frame_count);
				fill(p + i, R, G, B, 1);
			}
		}
	} break;
	default: {
		RK_LOGE("filling function do not support type %d\n", fmt);
		ret = -1;
	} break;
	}
	return ret;
}
