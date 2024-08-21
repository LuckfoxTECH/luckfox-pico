// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "common.h"

#ifndef _RK_COLOR_TABLES_H_
#define _RK_COLOR_TABLES_H_

#define PALETTE_TABLE_LEN 256

extern const uint32_t rgb888_palette_table[PALETTE_TABLE_LEN];
extern const uint32_t yuv444_palette_table[PALETTE_TABLE_LEN];

unsigned char find_color(const uint32_t *pal, unsigned int len, unsigned char r, unsigned char g,
                         unsigned char b);

#endif // _RK_OSD_MIDDLEWARE_H_
