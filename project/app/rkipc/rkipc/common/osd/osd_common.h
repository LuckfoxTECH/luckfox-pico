// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef _RKIPC_OSD_H_
#define _RKIPC_OSD_H_

#include "font_factory.h"
#include <wchar.h>

#define OSD_FMT_SPACE " "
#define OSD_FMT_TIME0 "24hour"
#define OSD_FMT_TIME1 "12hour"
#define OSD_FMT_WEEK0 "WEEKCN"
#define OSD_FMT_WEEK1 "WEEK"
#define OSD_FMT_CHR "CHR"
#define OSD_FMT_YMD0 "YYYY-MM-DD"
#define OSD_FMT_YMD1 "MM-DD-YYYY"
#define OSD_FMT_YMD2 "DD-MM-YYYY"
#define OSD_FMT_YMD3 "YYYY/MM/DD"
#define OSD_FMT_YMD4 "MM/DD/YYYY"
#define OSD_FMT_YMD5 "DD/MM/YYYY"

#define WEB_VIEW_RECT_W 704
#define WEB_VIEW_RECT_H 480
#define MAX_WCH_BYTE 128

enum {
	OSD_TYPE_DATE = 0,
	OSD_TYPE_IMAGE = 1,
	OSD_TYPE_TEXT = 2,
	OSD_TYPE_BORDER = 3,
};

typedef struct text_data {
	wchar_t wch[MAX_WCH_BYTE];
	unsigned int font_size;
	unsigned int font_color;
	unsigned int color_inverse;
	const char *font_path;
	char format[128];
} text_data_s;

typedef struct border_data {
	int color_index;
	int color_key;
	int thick;
	int display_style;
} border_data_s;

typedef struct osd_data {
	int type;
	union {
		const char *image;
		text_data_s text;
		border_data_s border;
	};
	int width;
	int height;
	unsigned char *buffer;
	unsigned int size;

	int origin_x;
	int origin_y;
	int enable;
} osd_data_s;

typedef struct Region_Rect {
	int left;
	int right;
	int top;
	int bottom;
} region_rect_s;

typedef struct Region_Invade {
	int enable;
	int width;
	int height;
	float position_x;
	float position_y;
	int proportion;
	int sensitivity_level;
	int time_threshold;
	int blink_cnt_;
	int blink_interval_ms_;
	int blink_wait_cnt_;
	int blink_status_;
} region_invade_s;

#endif