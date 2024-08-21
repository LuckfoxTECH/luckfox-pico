/**
  * Copyright (c) 2020 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#ifndef __LIB_IMAGEPROCESS_H_
#define __LIB_IMAGEPROCESS_H_

#include <rtthread.h>
#include <stdio.h>
#include <string.h>

#define MAX_DSTIMG_WIDTH        512
#define TYPE_RGB565             0
#define TYPE_RGB888             1
#define TYPE_ARGB8888           2
#define TYPE_RGB565_565         3
#define TYPE_RGB888_565         4
#define TYPE_ARGB8888_565       5

#define RK_ROTATE_USING_DSP     1
#define RK_SCALE_USING_DSP      1
#define RK_LZW_USING_DSP        1

#define IMG_PROCESS_ID                         0x50000002

#define DSP_ALGO_ROTATE_16BIT                  0x40000009
#define DSP_ALGO_ROTATE_16BIT_FAST             0x4000000A
#define DSP_ALGO_ROTATE_24BIT                  0x4000000B
#define DSP_ALGO_SCALE_24BIT                   0x4000000C
#define DSP_ALGO_DSP_ALGO_LZW_DECOMPRESS       0x4000000D

#undef  MIN
#define MIN(X, Y)           ((X)<(Y)?(X):(Y))
#undef  MAX
#define MAX(X, Y)           ((X)>(Y)?(X):(Y))
#undef  ABS
#define ABS(X)              (((X) < 0) ? (-(X)) : (X))

typedef struct image_st
{
    short  width;
    short  height;
    short  stride;
    unsigned char  *pdata;
    uint8_t format;
} image_st;

typedef struct rgb565pixel
{
    unsigned short  r: 5;
    unsigned short  g: 6;
    unsigned short  b: 5;
} rgb565pixel;

typedef struct rgb888pixel
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} rgb888pixel;

typedef struct rotateimage_st
{
    short width;
    short height;
    short stride;
    float cx;
    float cy;
    unsigned char *pdata;
} rotateimage_st;

struct rk_rotate_cfg
{
    struct rotateimage_st ps;
    struct rotateimage_st pd;
    int32_t angle;
    struct rotateimage_st pbg;
    short *preinfo;
};

struct rk_scale_cfg
{
    struct image_st ps;
    struct image_st pd;
    int32_t src_type;
};

struct rk_lzw_cfg
{
    uint8_t *src;
    uint32_t src_len;
    uint8_t *dst;
    uint32_t dst_len;
};

#ifdef __cplusplus
extern "C" {
#endif
void imgScale_ARGB2RGB(struct image_st *src, struct image_st *dst);
void imgScale_RGB2RGB(struct image_st *src, struct image_st *dst);
void imgScale_565RGB2RGB(struct image_st *src, struct image_st *dst);
void imgScale_ARGB2RGB565(image_st *src, image_st *dst);
void imgScale_RGB2RGB565(struct image_st *src, struct image_st *dst);
void imgScale_565RGB2RGB565(struct image_st *src, struct image_st *dst);
void imagerotate_ARGB2RGB(struct rotateimage_st *psrc, struct rotateimage_st *pdst, float angle);
void imagerotate_ARGB2RGB565(struct rotateimage_st *psrc, struct rotateimage_st *pdst, float angle);
void imagerotate_ARGB2RGB565Fast(struct rotateimage_st *psrc, struct rotateimage_st *psrcBG, struct rotateimage_st *pdst, short *preIndex, float angle);

void SaveBmp(const char *filename, unsigned char *rgbbuf, int width, int height, int bytePerDot, int stride);

void *rk_imagelib_dsp_dev(void);
void rk_imagelib_dsp_lock(void);
void rk_imagelib_dsp_unlock(void);
int rk_imagelib_init(void);
void rk_rotate_process(struct rotateimage_st *ps, struct rotateimage_st *pd, int32_t angle);
void rk_rotate_process_16bit(struct rotateimage_st *ps, struct rotateimage_st *pd, int32_t angle);
void rk_rotate_process_16bitfast(struct rotateimage_st *ps, struct rotateimage_st *pbg, struct rotateimage_st *pd, short *preIndex, int32_t angle);
void rk_scale_process(struct image_st *ps, struct image_st *pd, int32_t type);
void rk_lzw_decompress_dsp(uint8_t *src, uint32_t src_len, uint8_t *dst, uint32_t dst_len);
void rk_image_reset(struct image_st *ps, uint8_t dotbyte);
void rk_image_copy(struct image_st *ps, struct image_st *pd, uint8_t dotbyte);
void rk_image_copy_to565(struct image_st *ps, struct image_st *pd, uint8_t dotbyte);
void rk_image_copy_to888(struct image_st *ps, struct image_st *pd, uint8_t dotbyte);
void rk_imagelib_deinit(void);

void drawLine(uint8_t *fb, float x0, float y0, float x1, float y1,
              void (*plot)(uint8_t *fb, int x, int y, float c),
              void (*plot_shadow)(uint8_t* fb, int x, int y, float c));

#ifdef __cplusplus
}
#endif

#endif
