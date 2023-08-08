// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _RK_OSD_DRAW_PAINT_H_
#define _RK_OSD_DRAW_PAINT_H_

#include "common.h"
#include "osd_common.h"

enum BorderEffect {
	BORDER_LINE = 0,
	BORDER_DOTTED,
	BORDER_WATERFULL_LIGHT,
};

typedef struct DrawRect {
	int x;
	int y;
	int w;
	int h;
} DrawRect;

typedef struct BorderInfo {
	DrawRect rect;
	uint32_t color;
	uint32_t color_key;
	int thick;
	int display_style;
	int dotted_offset;
	int interval;
} BorderInfo;

void draw_solid_border(uint32_t *buffer, BorderInfo info);
void draw_dotted_border(uint32_t *buffer, BorderInfo info);
void draw_border(uint32_t *buffer, BorderInfo info);

#endif // _RK_OSD_DRAW_PAINT_H_
