// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// #include <locale.h>

#include "osd.h"
#include "common.h"
#include "font_factory.h"
#include "osd_common.h"
#include <iconv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "osd.c"

#define FONT_LIBRARY_PATH "/usr/share/simsun_en.ttf"

#define MAX_OSD_NUM 8

static int g_osd_server_run_ = 1;
static int g_osd_font_already_set = 0;
static double g_x_rate = 1.0;
static double g_y_rate = 1.0;
static pthread_t osd_time_thread_id_;
static pthread_mutex_t g_osd_mutex = PTHREAD_MUTEX_INITIALIZER;
static void *g_osd_signal;

rk_osd_cover_create_callback rk_osd_cover_create_ = NULL;
rk_osd_cover_destroy_callback rk_osd_cover_destroy_ = NULL;
rk_osd_mosaic_create_callback rk_osd_mosaic_create_ = NULL;
rk_osd_mosaic_destroy_callback rk_osd_mosaic_destroy_ = NULL;
rk_osd_bmp_create_callback rk_osd_bmp_create_ = NULL;
rk_osd_bmp_destroy_callback rk_osd_bmp_destroy_ = NULL;
rk_osd_bmp_change_callback rk_osd_bmp_change_ = NULL;

void rk_osd_cover_create_callback_register(rk_osd_cover_create_callback callback_ptr) {
	rk_osd_cover_create_ = callback_ptr;
}

void rk_osd_cover_destroy_callback_register(rk_osd_cover_destroy_callback callback_ptr) {
	rk_osd_cover_destroy_ = callback_ptr;
}

void rk_osd_mosaic_create_callback_register(rk_osd_mosaic_create_callback callback_ptr) {
	rk_osd_mosaic_create_ = callback_ptr;
}

void rk_osd_mosaic_destroy_callback_register(rk_osd_mosaic_destroy_callback callback_ptr) {
	rk_osd_mosaic_destroy_ = callback_ptr;
}

void rk_osd_bmp_create_callback_register(rk_osd_bmp_create_callback callback_ptr) {
	rk_osd_bmp_create_ = callback_ptr;
}

void rk_osd_bmp_destroy_callback_register(rk_osd_bmp_destroy_callback callback_ptr) {
	rk_osd_bmp_destroy_ = callback_ptr;
}

void rk_osd_bmp_change_callback_register(rk_osd_bmp_change_callback callback_ptr) {
	rk_osd_bmp_change_ = callback_ptr;
}

int fill_image(osd_data_s *data) { return load_bmp_form_file(data); }

int fill_text(osd_data_s *data) {
	// LOG_INFO("fill_text\n");
	if (data->text.font_path == NULL) {
		LOG_ERROR("font_path is NULL\n");
		return -1;
	}

	// int ret = create_font(data->text.font_path, data->text.font_size);
	// if (ret)
	// 	return -1;
	set_font_color(data->text.font_color);
	draw_argb8888_text(data->buffer, data->width, data->height, data->text.wch);
	// destroy_font();
	return 0;
}

int generate_date_time(const char *fmt, wchar_t *result, int r_size) {
	char year[8] = {0}, month[4] = {0}, day[4] = {0};
	char week[16] = {0}, hms[12] = {0};
	wchar_t w_ymd[16] = {0};
	wchar_t w_week[16] = {0};
	int wid = -1;
	int wchar_cnt = 0;

	time_t curtime;
	curtime = time(0);
	strftime(year, sizeof(year), "%Y", localtime(&curtime));
	strftime(month, sizeof(month), "%m", localtime(&curtime));
	strftime(day, sizeof(day), "%d", localtime(&curtime));

	if (strstr(fmt, OSD_FMT_TIME0)) {
		strftime(hms, sizeof(hms), "%H:%M:%S", localtime(&curtime));
	} else if (strstr(fmt, OSD_FMT_TIME1)) {
		strftime(hms, sizeof(hms), "%I:%M:%S %p", localtime(&curtime));
	}

	wchar_cnt = sizeof(w_week) / sizeof(wchar_t);
	if (strstr(fmt, OSD_FMT_WEEK0)) {
		strftime(week, sizeof(week), "%u", localtime(&curtime));
		wid = week[0] - '0';
		switch (wid) {
		case 1:
			swprintf(w_week, wchar_cnt, L" 星期一");
			break;
		case 2:
			swprintf(w_week, wchar_cnt, L" 星期二");
			break;
		case 3:
			swprintf(w_week, wchar_cnt, L" 星期三");
			break;
		case 4:
			swprintf(w_week, wchar_cnt, L" 星期四");
			break;
		case 5:
			swprintf(w_week, wchar_cnt, L" 星期五");
			break;
		case 6:
			swprintf(w_week, wchar_cnt, L" 星期六");
			break;
		case 7:
			swprintf(w_week, wchar_cnt, L" 星期日");
			break;
		default:
			LOG_ERROR("osd strftime week error\n");
			swprintf(w_week, wchar_cnt, L" 星期*");
			break;
		}
	} else if (strstr(fmt, OSD_FMT_WEEK1)) {
		strftime(week, sizeof(week), "%A", localtime(&curtime));
		swprintf(w_week, wchar_cnt, L" %s", week);
	}

	wchar_cnt = sizeof(w_ymd) / sizeof(wchar_t);
	if (strstr(fmt, OSD_FMT_CHR)) {
		if (strstr(fmt, OSD_FMT_YMD0))
			swprintf(w_ymd, wchar_cnt, L"%s-%s-%s", year, month, day);
		else if (strstr(fmt, OSD_FMT_YMD1))
			swprintf(w_ymd, wchar_cnt, L"%s-%s-%s", month, day, year);
		else if (strstr(fmt, OSD_FMT_YMD2))
			swprintf(w_ymd, wchar_cnt, L"%s-%s-%s", day, month, year);
		else if (strstr(fmt, OSD_FMT_YMD3))
			swprintf(w_ymd, wchar_cnt, L"%s/%s/%s", year, month, day);
		else if (strstr(fmt, OSD_FMT_YMD4))
			swprintf(w_ymd, wchar_cnt, L"%s/%s/%s", month, day, year);
		else if (strstr(fmt, OSD_FMT_YMD5))
			swprintf(w_ymd, wchar_cnt, L"%s/%s/%s", day, month, year);
	} else {
		if (strstr(fmt, OSD_FMT_YMD0))
			swprintf(w_ymd, wchar_cnt, L"%s年%s月%s日", year, month, day);
		else if (strstr(fmt, OSD_FMT_YMD1))
			swprintf(w_ymd, wchar_cnt, L"%s月%s日%s年", month, day, year);
		else if (strstr(fmt, OSD_FMT_YMD2))
			swprintf(w_ymd, wchar_cnt, L"%s日%s月%s年", day, month, year);
	}

	swprintf(result, r_size, L"%ls%ls %s", w_ymd, w_week, hms);
	return wchar_cnt;
}

static void *osd_time_server(void *arg) {
	printf("#Start %s thread, arg:%p\n", __func__, arg);
	prctl(PR_SET_NAME, "osd_time_server", 0, 0, 0);
	int osd_time_id = 0;
	int last_time_sec, wchar_cnt;
	const char *osd_type;
	const char *date_style;
	const char *time_style;
	char entry[128] = {'\0'};
	osd_data_s osd_data;
	time_t rawtime;
	struct tm *cur_time_info;

	memset(&osd_data, 0, sizeof(osd_data));
	for (int i = 0; i < MAX_OSD_NUM; i++) {
		snprintf(entry, 127, "osd.%d:type", i);
		osd_type = rk_param_get_string(entry, NULL);
		if (osd_type == NULL)
			continue;
		if (!strcmp(osd_type, "dateTime"))
			osd_time_id = i;
	}
	LOG_INFO("osd_time_id is %d\n", osd_time_id);

	// init
	snprintf(entry, 127, "osd.%d:enabled", osd_time_id);
	osd_data.enable = rk_param_get_int(entry, 0);
	snprintf(entry, 127, "osd.%d:position_x", osd_time_id);
	osd_data.origin_x = UPALIGNTO16((int)(rk_param_get_int(entry, -1) * g_x_rate));
	snprintf(entry, 127, "osd.%d:position_y", osd_time_id);
	osd_data.origin_y = UPALIGNTO16((int)(rk_param_get_int(entry, -1) * g_y_rate));
	osd_data.text.font_size = rk_param_get_int("osd.common:font_size", -1);
	sscanf(rk_param_get_string("osd.common:font_color", NULL), "%x", &osd_data.text.font_color);
	LOG_INFO("osd_data.text.font_color is %x\n", osd_data.text.font_color);
	osd_data.text.color_inverse = 1;
	osd_data.text.font_path = rk_param_get_string("osd.common:font_path", NULL);
	// osd_data.text.format = rk_param_get_int("osd.common:???", -1);

	// get time
	memset(osd_data.text.format, 0, 128);
	snprintf(entry, 127, "osd.%d:date_style", osd_time_id);
	date_style = rk_param_get_string(entry, NULL);
	snprintf(entry, 127, "osd.%d:time_style", osd_time_id);
	time_style = rk_param_get_string(entry, NULL);
	snprintf(entry, 127, "osd.%d:display_week_enabled", osd_time_id);
	int display_week_enabled = rk_param_get_int(entry, 0);
	if (time_style) {
		if (!strcmp(time_style, "12hour")) {
			strcat(osd_data.text.format, OSD_FMT_TIME1);
			strcat(osd_data.text.format, OSD_FMT_SPACE);
		} else {
			strcat(osd_data.text.format, OSD_FMT_TIME0);
			strcat(osd_data.text.format, OSD_FMT_SPACE);
		}
	}

	if (display_week_enabled) {
		strcat(osd_data.text.format, OSD_FMT_WEEK0);
		strcat(osd_data.text.format, OSD_FMT_SPACE);
	}

	if (date_style) {
		strcat(osd_data.text.format, date_style);
	}
	LOG_INFO("osd_data.text.format is %s\n", osd_data.text.format);

	wchar_cnt = generate_date_time(osd_data.text.format, osd_data.text.wch, 128);
	osd_data.width = UPALIGNTO16(wchar_cnt * osd_data.text.font_size);
	osd_data.height = UPALIGNTO16(osd_data.text.font_size);
	osd_data.size = osd_data.width * osd_data.height * 4; // BGRA8888 4byte
	osd_data.buffer = malloc(osd_data.size);
	memset(osd_data.buffer, 0, osd_data.size);
	fill_text(&osd_data);
	// rk_osd_bmp_destroy_(osd_time_id);
	rk_osd_bmp_create_(osd_time_id, &osd_data);
	free(osd_data.buffer);

	time(&rawtime);
	cur_time_info = localtime(&rawtime);
	last_time_sec = cur_time_info->tm_sec;
	while (g_osd_server_run_) {
		// only update time bmp
		rk_signal_wait(g_osd_signal, 100); // TODO: in xx s 00 ms update
		time(&rawtime);
		cur_time_info = localtime(&rawtime);
		if (cur_time_info->tm_sec == last_time_sec)
			continue;
		else
			last_time_sec = cur_time_info->tm_sec;
		wchar_cnt = generate_date_time(osd_data.text.format, osd_data.text.wch, 128);
		osd_data.width = UPALIGNTO16(wchar_cnt * osd_data.text.font_size);
		osd_data.height = UPALIGNTO16(osd_data.text.font_size);
		osd_data.size = osd_data.width * osd_data.height * 4; // BGRA8888 4byte
		osd_data.buffer = malloc(osd_data.size);
		memset(osd_data.buffer, 0, osd_data.size);
		fill_text(&osd_data);
		rk_osd_bmp_change_(osd_time_id, &osd_data);
		free(osd_data.buffer);
	}
	LOG_INFO("exit\n");

	return 0;
}

int rk_osd_init() {
	LOG_DEBUG("%s\n", __func__);
	pthread_mutex_lock(&g_osd_mutex);
	if (g_osd_signal)
		rk_signal_destroy(g_osd_signal);
	g_osd_signal = rk_signal_create(0, 1);
	if (!g_osd_signal) {
		LOG_ERROR("create signal fail\n");
		return -1;
	}
	const char *osd_type;
	char entry[128] = {'\0'};
	osd_data_s osd_data;
	int video_width = rk_param_get_int("video.0:width", -1);
	int video_height = rk_param_get_int("video.0:height", -1);
	int normalized_screen_width = rk_param_get_int("osd.common:normalized_screen_width", -1);
	int normalized_screen_height = rk_param_get_int("osd.common:normalized_screen_height", -1);
	g_x_rate = (double)video_width / (double)normalized_screen_width;
	g_y_rate = (double)video_height / (double)normalized_screen_height;
	LOG_DEBUG("g_x_rate is %lf, g_y_rate is %lf\n", g_x_rate, g_y_rate);

	for (int i = 0; i < MAX_OSD_NUM; i++) {
		snprintf(entry, 127, "osd.%d:type", i);
		osd_type = rk_param_get_string(entry, NULL);
		if (osd_type == NULL)
			continue;
		LOG_DEBUG("i is %d, osd_type is %s\n", i, osd_type);

		snprintf(entry, 127, "osd.%d:enabled", i);
		osd_data.enable = rk_param_get_int(entry, 0);
		if (osd_data.enable == 0)
			continue;
		snprintf(entry, 127, "osd.%d:position_x", i);
		osd_data.origin_x = UPALIGNTO16((int)(rk_param_get_int(entry, -1) * g_x_rate));
		snprintf(entry, 127, "osd.%d:position_y", i);
		osd_data.origin_y = UPALIGNTO16((int)(rk_param_get_int(entry, -1) * g_y_rate));

		if (!strcmp(osd_type, "image")) {
			snprintf(entry, 127, "osd.%d:image_path", i);
			osd_data.image = rk_param_get_string(entry, NULL);
			// load bmp
			fill_image(&osd_data);
			rk_osd_bmp_create_(i, &osd_data);
			if (osd_data.buffer)
				free(osd_data.buffer);
		} else if (!strcmp(osd_type, "privacyMask")) {
			snprintf(entry, 127, "osd.%d:width", i);
			osd_data.width = UPALIGNTO16((int)(rk_param_get_int(entry, -1) * g_x_rate));
			snprintf(entry, 127, "osd.%d:height", i);
			osd_data.height = UPALIGNTO16((int)(rk_param_get_int(entry, -1) * g_y_rate));
			while (osd_data.origin_x + osd_data.width > video_width) {
				osd_data.width -= 16;
			}
			while (osd_data.origin_y + osd_data.height > video_height) {
				osd_data.height -= 16;
			}
			if (osd_data.origin_x < 0 || osd_data.origin_y < 0 || osd_data.width < 0 ||
			    osd_data.height < 0) {
				continue;
			}

			snprintf(entry, 127, "osd.%d:style", i);
			const char *style = rk_param_get_string(entry, "cover");
			if (!strcmp(style, "cover") && rk_osd_cover_create_)
				rk_osd_cover_create_(i, &osd_data);
			else if (!strcmp(style, "mosaic") && rk_osd_mosaic_create_)
				rk_osd_mosaic_create_(i, &osd_data);
		} else {
			osd_data.text.font_size = rk_param_get_int("osd.common:font_size", -1);
			// osd_data.text.font_color = 0xfff799;
			sscanf(rk_param_get_string("osd.common:font_color", NULL), "%x",
			       &osd_data.text.font_color);
			LOG_DEBUG("osd_data.text.font_color is %x\n", osd_data.text.font_color);
			osd_data.text.color_inverse = 1;
			osd_data.text.font_path = rk_param_get_string("osd.common:font_path", NULL);
			if (!g_osd_font_already_set) {
				create_font(osd_data.text.font_path, osd_data.text.font_size);
				set_font_color(osd_data.text.font_color);
				g_osd_font_already_set = 1;
			}
			if (!strcmp(osd_type, "channelName") || !strcmp(osd_type, "character")) {
				snprintf(entry, 127, "osd.%d:display_text", i);
				const char *display_text = rk_param_get_string(entry, NULL);
				LOG_DEBUG("display_text is %s\n", display_text);
				LOG_DEBUG("strlen(display_text) is %ld\n", strlen(display_text));
#if 0
				int ret = mbstowcs(osd_data.text.wch, display_text, strlen(display_text));
				LOG_DEBUG("mbstowcs ret is %d\n", ret);
				if (ret == -1)
					continue;
				osd_data.text.wch[ret] = '\0';
#else
				int ret;
				int src_len = strlen(display_text);
				int out_len = 0;
				// To have this temporary variable,
				// otherwise iconv will directly overwrite the original pointer
				char *tmp_out_buffer = (char *)osd_data.text.wch;
				iconv_t cd = iconv_open("WCHAR_T", "UTF-8");
				if (cd == (iconv_t)-1) {
					perror("iconv_open error");
					continue;
				}
				memcpy(entry, display_text, src_len); // iconv maybe change the char *
				memset(entry + src_len, 0, 1);
				char *tmp_in_buffer = (char *)entry;
				ret = iconv(cd, &tmp_in_buffer, (size_t *)&src_len, &tmp_out_buffer,
				            (size_t *)&out_len);
				if (ret == -1)
					perror("iconv error");
				iconv_close(cd);
				osd_data.text.wch[abs(out_len) / 4] = '\0';
				LOG_DEBUG("out_len is %d\n", out_len);
				LOG_DEBUG("wcslen(osd_data.text.wch) is %ld\n", wcslen(osd_data.text.wch));
				// for(int i=0;i< strlen(display_text); i++) {
				// 	LOG_INFO("111 display_text [%02x]\n",display_text[i]);
				// }
				// for (int i = 0; i < wcslen(osd_data.text.wch); i++) {
				// 	LOG_INFO("222 osd_data.text.wch [%02x]\n", osd_data.text.wch[i]);
				// }
#endif
				osd_data.width = UPALIGNTO16(wcslen(osd_data.text.wch) * osd_data.text.font_size);
				osd_data.height = UPALIGNTO16(osd_data.text.font_size);
				osd_data.size = osd_data.width * osd_data.height * 4; // BGRA8888 4byte
				osd_data.buffer = malloc(osd_data.size);
				memset(osd_data.buffer, 0, osd_data.size);
				fill_text(&osd_data);
				rk_osd_bmp_create_(i, &osd_data);
				free(osd_data.buffer);
			} else if (!strcmp(osd_type, "dateTime")) {
				g_osd_server_run_ = 1;
				pthread_create(&osd_time_thread_id_, NULL, osd_time_server, NULL);
			}
		}
	}
	pthread_mutex_unlock(&g_osd_mutex);

	return 0;
}

int rk_osd_deinit() {
	LOG_DEBUG("begin\n");
	pthread_mutex_lock(&g_osd_mutex);
	const char *osd_type;
	char entry[128] = {'\0'};
	g_osd_server_run_ = 0;
	if (g_osd_signal) {
		rk_signal_give(g_osd_signal);
		pthread_join(osd_time_thread_id_, NULL);
		rk_signal_destroy(g_osd_signal);
		g_osd_signal = NULL;
	}
	for (int i = 0; i < MAX_OSD_NUM; i++) {
		snprintf(entry, 127, "osd.%d:type", i);
		osd_type = rk_param_get_string(entry, NULL);
		if (osd_type == NULL)
			continue;
		// Because enable has been set to 0, after skipping here, the destroy will also be skipped
		// snprintf(entry, 127, "osd.%d:enabled", i);
		// if (rk_param_get_int(entry, 0) == 0)
		// 	continue;
		LOG_DEBUG("i is %d, osd_type is %s\n", i, osd_type);

		if (!strcmp(osd_type, "channelName")) {
			rk_osd_bmp_destroy_(i);
		} else if (!strcmp(osd_type, "dateTime")) {
			rk_osd_bmp_destroy_(i);
		} else if (!strcmp(osd_type, "character")) {
			rk_osd_bmp_destroy_(i);
		} else if (!strcmp(osd_type, "privacyMask")) {
			snprintf(entry, 127, "osd.%d:style", i);
			const char *style = rk_param_get_string(entry, "cover");
			if (!strcmp(style, "cover") && rk_osd_cover_destroy_)
				rk_osd_cover_destroy_(i);
			else if (!strcmp(style, "mosaic") && rk_osd_mosaic_destroy_)
				rk_osd_mosaic_destroy_(i);
		} else if (!strcmp(osd_type, "image")) {
			rk_osd_bmp_destroy_(i);
		}
	}
	destroy_font();
	g_osd_font_already_set = 0;
	LOG_DEBUG("over\n");
	pthread_mutex_unlock(&g_osd_mutex);

	return 0;
}

int rk_osd_restart() {
	rk_osd_deinit();
	rk_osd_init();

	return 0;
}

int rk_osd_privacy_mask_restart() {
	pthread_mutex_lock(&g_osd_mutex);
	const char *osd_type;
	char entry[128] = {'\0'};
	// destroy
	for (int i = 0; i < MAX_OSD_NUM; i++) {
		snprintf(entry, 127, "osd.%d:type", i);
		osd_type = rk_param_get_string(entry, NULL);
		if (osd_type == NULL)
			continue;
		// Because enable has been set to 0, after skipping here, the destroy will also be skipped
		// snprintf(entry, 127, "osd.%d:enabled", i);
		// if (rk_param_get_int(entry, 0) == 0)
		// 	continue;
		LOG_DEBUG("i is %d, osd_type is %s\n", i, osd_type);
		if (strcmp(osd_type, "privacyMask"))
			continue;
		snprintf(entry, 127, "osd.%d:style", i);
		const char *style = rk_param_get_string(entry, "cover");
		if (!strcmp(style, "cover") && rk_osd_cover_destroy_)
			rk_osd_cover_destroy_(i);
		else if (!strcmp(style, "mosaic") && rk_osd_mosaic_destroy_)
			rk_osd_mosaic_destroy_(i);
	}
	// create
	osd_data_s osd_data;
	int video_width = rk_param_get_int("video.0:width", -1);
	int video_height = rk_param_get_int("video.0:height", -1);
	int normalized_screen_width = rk_param_get_int("osd.common:normalized_screen_width", -1);
	int normalized_screen_height = rk_param_get_int("osd.common:normalized_screen_height", -1);
	g_x_rate = (double)video_width / (double)normalized_screen_width;
	g_y_rate = (double)video_height / (double)normalized_screen_height;
	LOG_DEBUG("g_x_rate is %lf, g_y_rate is %lf\n", g_x_rate, g_y_rate);

	for (int i = 0; i < MAX_OSD_NUM; i++) {
		snprintf(entry, 127, "osd.%d:type", i);
		osd_type = rk_param_get_string(entry, NULL);
		if (osd_type == NULL)
			continue;
		LOG_DEBUG("i is %d, osd_type is %s\n", i, osd_type);

		snprintf(entry, 127, "osd.%d:enabled", i);
		osd_data.enable = rk_param_get_int(entry, 0);
		if (osd_data.enable == 0)
			continue;
		snprintf(entry, 127, "osd.%d:position_x", i);
		osd_data.origin_x = UPALIGNTO16((int)(rk_param_get_int(entry, -1) * g_x_rate));
		snprintf(entry, 127, "osd.%d:position_y", i);
		osd_data.origin_y = UPALIGNTO16((int)(rk_param_get_int(entry, -1) * g_y_rate));

		if (strcmp(osd_type, "privacyMask"))
			continue;

		snprintf(entry, 127, "osd.%d:width", i);
		osd_data.width = UPALIGNTO16((int)(rk_param_get_int(entry, -1) * g_x_rate));
		snprintf(entry, 127, "osd.%d:height", i);
		osd_data.height = UPALIGNTO16((int)(rk_param_get_int(entry, -1) * g_y_rate));

		while (osd_data.origin_x + osd_data.width > video_width) {
			osd_data.width -= 16;
		}
		while (osd_data.origin_y + osd_data.height > video_height) {
			osd_data.height -= 16;
		}
		if (osd_data.origin_x < 0 || osd_data.origin_y < 0 || osd_data.width < 0 ||
		    osd_data.height < 0) {
			continue;
		}
		LOG_DEBUG("xywh is %d,%d,%d,%d\n", osd_data.origin_x, osd_data.origin_y, osd_data.width,
		          osd_data.height);

		snprintf(entry, 127, "osd.%d:style", i);
		const char *style = rk_param_get_string(entry, "cover");
		if (!strcmp(style, "cover") && rk_osd_cover_create_)
			rk_osd_cover_create_(i, &osd_data);
		else if (!strcmp(style, "mosaic") && rk_osd_mosaic_create_)
			rk_osd_mosaic_create_(i, &osd_data);
	}
	LOG_INFO("over\n");
	pthread_mutex_unlock(&g_osd_mutex);

	return 0;
}

// for socket server
// osd.common

int rk_osd_get_is_presistent_text(int *value) {
	*value = rk_param_get_int("osd.common:is_presistent_text", -1);

	return 0;
}

int rk_osd_set_is_presistent_text(int value) {
	rk_param_set_int("osd.common:is_presistent_text", value);

	return 0;
}

int rk_osd_get_font_size(int *value) {
	*value = rk_param_get_int("osd.common:font_size", -1);

	return 0;
}

int rk_osd_set_font_size(int value) {
	rk_param_set_int("osd.common:font_size", value);

	return 0;
}

int rk_osd_get_boundary(int *value) {
	*value = rk_param_get_int("osd.common:boundary", -1);

	return 0;
}

int rk_osd_set_boundary(int value) {
	rk_param_set_int("osd.common:boundary", value);

	return 0;
}

int rk_osd_get_normalized_screen_width(int *value) {
	*value = rk_param_get_int("osd.common:normalized_screen_width", -1);

	return 0;
}

int rk_osd_get_normalized_screen_height(int *value) {
	*value = rk_param_get_int("osd.common:normalized_screen_height", -1);

	return 0;
}

int rk_osd_get_attribute(const char **value) {
	*value = rk_param_get_string("osd.common:attribute", NULL);

	return 0;
}

int rk_osd_set_attribute(const char *value) {
	rk_param_set_string("osd.common:attribute", value);

	return 0;
}

int rk_osd_get_font_color_mode(const char **value) {
	*value = rk_param_get_string("osd.common:font_color_mode", NULL);

	return 0;
}

int rk_osd_set_font_color_mode(const char *value) {
	rk_param_set_string("osd.common:font_color_mode", value);

	return 0;
}

int rk_osd_get_font_color(const char **value) {
	*value = rk_param_get_string("osd.common:font_color", NULL);

	return 0;
}

int rk_osd_set_font_color(const char *value) {
	rk_param_set_string("osd.common:font_color", value);

	return 0;
}

int rk_osd_get_alignment(const char **value) {
	*value = rk_param_get_string("osd.common:alignment", NULL);

	return 0;
}

int rk_osd_set_alignment(const char *value) {
	rk_param_set_string("osd.common:alignment", value);

	return 0;
}

int rk_osd_get_font_path(const char **value) {
	*value = rk_param_get_string("osd.common:font_path", NULL);

	return 0;
}

int rk_osd_set_font_path(const char *value) {
	rk_param_set_string("osd.common:font_path", value);

	return 0;
}

// osd.x

int rk_osd_get_enabled(int id, int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "osd.%d:enabled", id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_osd_set_enabled(int id, int value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "osd.%d:enabled", id);
	rk_param_set_int(entry, value);

	return 0;
}

int rk_osd_get_position_x(int id, int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "osd.%d:position_x", id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_osd_set_position_x(int id, int value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "osd.%d:position_x", id);
	rk_param_set_int(entry, value);

	return 0;
}

int rk_osd_get_position_y(int id, int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "osd.%d:position_y", id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_osd_set_position_y(int id, int value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "osd.%d:position_y", id);
	rk_param_set_int(entry, value);

	return 0;
}

int rk_osd_get_height(int id, int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "osd.%d:height", id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_osd_set_height(int id, int value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "osd.%d:height", id);
	rk_param_set_int(entry, value);

	return 0;
}

int rk_osd_get_width(int id, int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "osd.%d:width", id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_osd_set_width(int id, int value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "osd.%d:width", id);
	rk_param_set_int(entry, value);

	return 0;
}

int rk_osd_get_display_week_enabled(int id, int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "osd.%d:display_week_enabled", id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_osd_set_display_week_enabled(int id, int value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "osd.%d:display_week_enabled", id);
	rk_param_set_int(entry, value);

	return 0;
}

int rk_osd_get_date_style(int id, const char **value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "osd.%d:date_style", id);
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_osd_set_date_style(int id, const char *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "osd.%d:date_style", id);
	rk_param_set_string(entry, value);

	return 0;
}

int rk_osd_get_time_style(int id, const char **value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "osd.%d:time_style", id);
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_osd_set_time_style(int id, const char *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "osd.%d:time_style", id);
	rk_param_set_string(entry, value);

	return 0;
}

int rk_osd_get_type(int id, const char **value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "osd.%d:type", id);
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_osd_set_type(int id, const char *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "osd.%d:type", id);
	rk_param_set_string(entry, value);

	return 0;
}

int rk_osd_get_display_text(int id, const char **value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "osd.%d:display_text", id);
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_osd_set_display_text(int id, const char *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "osd.%d:display_text", id);
	rk_param_set_string(entry, value);

	return 0;
}

int rk_osd_get_image_path(int id, const char **value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "osd.%d:image_path", id);
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_osd_set_image_path(int id, const char *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "osd.%d:image_path", id);
	rk_param_set_string(entry, value);

	return 0;
}

int rk_osd_get_style(int id, const char **value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "osd.%d:style", id);
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_osd_set_style(int id, const char *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "osd.%d:style", id);
	rk_param_set_string(entry, value);

	return 0;
}
