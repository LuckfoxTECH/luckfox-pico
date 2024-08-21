/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    jpeg_hdr.h
  * @version V0.0.1
  * @brief   jpeg enc device abstract for swallow
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-09-12     jpeg enc Team   first implementation
  *
  ******************************************************************************
  */

#ifndef __JPEGE_HDR_H__
#define __JPEGE_HDR_H__

#include <rtdevice.h>
#include <rtthread.h>
#include <rthw.h>
#include "hal_base.h"
#include "jpeg_enc.h"

typedef void *JpegeBits;

void jpege_bits_init(JpegeBits *ctx);
void jpege_bits_deinit(JpegeBits ctx);

void jpege_bits_setup(JpegeBits ctx, uint8_t *buf, int32_t size);
void jpege_bits_put(JpegeBits ctx, uint32_t val, int32_t len);
void jpege_bits_align_byte(JpegeBits ctx);
void jpege_seek_bits(JpegeBits ctx, int32_t len);
uint8_t *jpege_bits_get_buf(JpegeBits ctx);
int32_t jpege_bits_get_bitpos(JpegeBits ctx);
int32_t jpege_bits_get_bytepos(JpegeBits ctx);

rt_err_t jpeg_write_header(void *ctx);
rt_err_t jpeg_write_header_time_stamp(void *ctx, void *args);

#endif /*__HAL_JPEGE_HDR_H__*/

