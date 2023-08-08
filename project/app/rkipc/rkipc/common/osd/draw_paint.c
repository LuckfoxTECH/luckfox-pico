// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "draw_paint.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "draw_paint.cpp"

int interval_offset_ = 0;

void draw_solid_border(uint32_t *buffer, BorderInfo info) {
	int offset;
	int rect_w = info.rect.w;
	int rect_h = info.rect.h;
	int thick = info.thick;
	uint32_t color = info.color;
	uint32_t dotted_line[info.rect.w];

	for (int i = 0; i < info.rect.w; i++) {
		dotted_line[i] = color;
	}

	for (int j = 0; j < thick; j++) {
		offset = j * rect_w;
		memcpy(buffer + offset, dotted_line, rect_w * 4);
		offset = (rect_h - j - 1) * rect_w;
		memcpy(buffer + offset, dotted_line, rect_w * 4);
	}

	for (int j = 0; j < rect_h; j++) {
		for (int k = 0; k < thick; k++) {
			buffer[j * rect_w + k] = color;
			buffer[(j + 1) * rect_w - thick + k] = color;
		}
	}
}

void draw_dotted_border(uint32_t *buffer, BorderInfo info) {
	int offset;
	int rect_w = info.rect.w;
	int rect_h = info.rect.h;
	int thick = info.thick;
	uint32_t color = info.color;
	uint32_t color_key = info.color_key;
	int interval = info.interval;
	uint32_t dotted_line[info.rect.w];

	for (int k = 0; k < rect_w; k++) {
		if (((k + interval_offset_) / interval) % 2)
			dotted_line[k] = color;
		else
			dotted_line[k] = color_key;
	}

	//按线宽，画上下两条
	for (int j = 0; j < thick; j++) {
		for (int k = 0; k < rect_w; k++) {
			offset = j * rect_w;
			memcpy(buffer + offset, dotted_line, rect_w * 4);
			offset = (rect_h - j - 1) * rect_w;
			memcpy(buffer + offset, dotted_line, rect_w * 4);
		}
	}
	//按线宽，画左右两条
	for (int j = 0; j < rect_h; j++) {
		for (int k = 0; k < thick; k++) {
			if (((j + interval_offset_) / interval) % 2) {
				buffer[j * rect_w + k] = color;
				buffer[(j + 1) * rect_w - thick + k] = color;
			}
		}
	}
}

void draw_border(uint32_t *buffer, BorderInfo info) {
	if (info.display_style == BORDER_DOTTED) {
		interval_offset_ = 0;
		draw_dotted_border(buffer, info);
	} else if (info.display_style == BORDER_LINE) {
		draw_solid_border(buffer, info);
	} else if (info.display_style == BORDER_WATERFULL_LIGHT) {
		interval_offset_ += 40;
		draw_dotted_border(buffer, info);
	}
}
