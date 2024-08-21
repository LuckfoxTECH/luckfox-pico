/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    image_info.h
  * @version V0.1
  * @brief   image info
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-07-18     Tony.zheng      first implementation
  *
  ******************************************************************************
  */

#ifndef __IMAGE_INFO_H__
#define __IMAGE_INFO_H__

/* Macro define */
#undef  MIN
#define MIN(X, Y)           ((X)<(Y)?(X):(Y))
#undef  MAX
#define MAX(X, Y)           ((X)>(Y)?(X):(Y))
#undef  ABS
#define ABS(X)              (((X) < 0) ? (-(X)) : (X))

/**
 * image_info_t "type" define
 */
#define IMG_TYPE_RAW        0
#define IMG_TYPE_COMPRESS   1
#define COLOR_KEY_EN        (0x01UL << 24)

/**
 * Image info for display
 */
typedef struct
{
    uint8_t  type;
    uint8_t  pixel;

    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
    uint32_t size;
    uint8_t  *data;
    uint32_t colorkey;
} image_info_t;

/**
 * display rotate.
 */
void rt_display_rotate_4bit(float angle, int w, int h, unsigned char *src, unsigned char *dst, int dst_str, int xcen, int ycen);
void rt_display_rotate_8bit(float angle, int w, int h, unsigned char *src, unsigned char *dst, int dst_str, int xcen, int ycen);
void rt_display_rotate_16bit(float angle, int w, int h, unsigned short *src, unsigned short *dst, int dst_str, int xcen, int ycen);
void rt_display_rotate_24bit(float angle, int w, int h, unsigned char *src, unsigned char *dst, int dst_str, int xcen, int ycen);
void rt_display_rotate_32bit(float angle, int w, int h, unsigned int *src, unsigned int *dst, int dst_str, int xcen, int ycen);

/**
 * fill image data to fb buffer
 */
void rt_display_img_fill(image_info_t *img_info, rt_uint8_t *fb,
                         rt_int32_t xVir, rt_int32_t xoffset, rt_int32_t yoffset);

#endif
