// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "common.h"
#include "draw_paint.h"
#include "font_factory.h"
#include "log.h"
#include "param.h"

#include <rga/im2d.h>
#include <rga/rga.h>

#include "ivm_dependent.h"
#include <fcntl.h>
#include <libdrm/drm_fourcc.h>
#include <sys/mman.h>
#include <unistd.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "event.c"

// static int board_x, board_y, board_w, board_h;
// static int font_size = 32;
// static int font_num = 32;
// static int text_w, text_h;
// static int g_enabled;
// static int g_time_threshold = 1;
// static int event_init_over_flag = 0;
// static time_t alarm_time;
// static time_t current_time;
// static IVM_HANDLE ivm = NULL;
// static int border_buffer_fd;
// static int text_buffer_fd;
// static uint8_t *border_buffer;
// static uint8_t *text_buffer;
// static double g_x_rate = 1.0;
// static double g_y_rate = 1.0;

// void *alloc_drm_buf(int *fd, int in_w, int in_h, int in_bpp) {
// 	static const char *card = "/dev/dri/card0";
// 	int drm_fd = -1;
// 	int flag = O_RDWR;
// 	int ret;
// 	void *map = NULL;

// 	void *vir_addr = NULL;
// 	struct drm_prime_handle fd_args;
// 	struct drm_mode_map_dumb mmap_arg;
// 	struct drm_mode_destroy_dumb destory_arg;

// 	struct drm_mode_create_dumb alloc_arg;

// 	drm_fd = open(card, flag);
// 	if (drm_fd < 0) {
// 		printf("failed to open %s\n", card);
// 		return NULL;
// 	}

// 	memset(&alloc_arg, 0, sizeof(alloc_arg));
// 	alloc_arg.bpp = in_bpp;
// 	alloc_arg.width = in_w;
// 	alloc_arg.height = in_h;

// 	ret = drmIoctl(drm_fd, DRM_IOCTL_MODE_CREATE_DUMB, &alloc_arg);
// 	if (ret) {
// 		printf("failed to create dumb buffer: %s\n", strerror(errno));
// 		return NULL;
// 	}

// 	memset(&fd_args, 0, sizeof(fd_args));
// 	fd_args.fd = -1;
// 	fd_args.handle = alloc_arg.handle;
// 	;
// 	fd_args.flags = 0;
// 	ret = drmIoctl(drm_fd, DRM_IOCTL_PRIME_HANDLE_TO_FD, &fd_args);
// 	if (ret) {
// 		printf("rk-debug handle_to_fd failed ret=%d,err=%s, handle=%x \n", ret, strerror(errno),
// 		       fd_args.handle);
// 		return NULL;
// 	}
// 	printf("Dump fd = %d \n", fd_args.fd);
// 	*fd = fd_args.fd;

// 	// handle to Virtual address
// 	memset(&mmap_arg, 0, sizeof(mmap_arg));
// 	mmap_arg.handle = alloc_arg.handle;

// 	ret = drmIoctl(drm_fd, DRM_IOCTL_MODE_MAP_DUMB, &mmap_arg);
// 	if (ret) {
// 		printf("failed to create map dumb: %s\n", strerror(errno));
// 		vir_addr = NULL;
// 		goto destory_dumb;
// 	}
// 	vir_addr = map =
// 	    mmap64(0, alloc_arg.size, PROT_READ | PROT_WRITE, MAP_SHARED, drm_fd, mmap_arg.offset);
// 	if (map == MAP_FAILED) {
// 		printf("failed to mmap buffer: %s\n", strerror(errno));
// 		vir_addr = NULL;
// 		goto destory_dumb;
// 	}
// 	printf("alloc map=%x \n", map);
// 	close(drm_fd);
// 	return vir_addr;
// destory_dumb:
// 	memset(&destory_arg, 0, sizeof(destory_arg));
// 	destory_arg.handle = alloc_arg.handle;
// 	int fdd = *fd;
// 	ret = drmIoctl(fdd, DRM_IOCTL_MODE_DESTROY_DUMB, &destory_arg);
// 	if (ret)
// 		printf("failed to destory dumb %d\n", ret);
// 	return vir_addr;
// }

// static int ivm_event_cb(ivm_type_t type, void *event_result, void *userdata) {
// 	ivm_ai_object_array_t *detect = NULL;
// 	switch (type) {
// 	case IVM_DETECT:
// 		// detect = (ivm_ai_object_array_t *)event_result;
// 		// LOG_INFO("type: %d, detect count: %d, first rect: ltrb[%d, %d, %d, %d]\n", type,
// 		//        detect->count, detect->object[0].box.left, detect->object[0].box.top,
// 		//        detect->object[0].box.right, detect->object[0].box.bottom);
// 		break;
// 	case IVM_BORDER_ALARM:
// 		// todo:用rga画框并更新，闪烁的效果

// 		// LOG_INFO("%s: type: %d, char: %s\n", __func__, type, (char *)event_result);
// 		time(&alarm_time); // update alarm time
// 		                   // if (rk_param_get_int("osd.3:enabled", -1) == 0) {
// 		                   //     rk_osd_set_enabled(3, 1);
// 		                   //     //rk_osd_restart();
// 		                   // }
// 		break;
// 	case IVM_DRAW:
// 	case IVM_INVALID:
// 	case IVM_END:
// 	default:
// 		LOG_INFO("%s: unexcept type: %d, userdata: %p\n", __func__, type, userdata);
// 	}
// 	return 0;
// }

// int rk_event_ri_set_regions() {
// 	//
// 注意：ivm_set_regions要千分制坐标，而实际像素坐标又需要和web的normalized_screen做归一化，两个不一样。
// 	// update board into
// 	int position_x = rk_param_get_int("event.regional_invasion:position_x", 0);
// 	int position_y = rk_param_get_int("event.regional_invasion:position_y", 0);
// 	int width = rk_param_get_int("event.regional_invasion:width", 0);
// 	int height = rk_param_get_int("event.regional_invasion:height", 0);
// 	LOG_INFO("origin x,y,w,h is %d,%d,%d,%d\n", position_x, position_y, width, height);

// 	board_x = (double)position_x * g_x_rate;
// 	board_y = (double)position_y * g_y_rate;
// 	board_w = (double)width * g_x_rate;
// 	board_h = (double)height * g_y_rate;
// 	// UPALIGNTO4
// 	LOG_INFO("normalized board x,y,w,h is %d,%d,%d,%d\n", board_x, board_y, board_w, board_h);
// 	board_x = UPALIGNTO4(board_x);
// 	board_y = UPALIGNTO4(board_y);
// 	board_w = UPALIGNTO4(board_w);
// 	board_h = UPALIGNTO4(board_h);
// 	LOG_INFO("normalized and UPALIGNTO4 board x,y,w,h is %d,%d,%d,%d\n", board_x, board_y, board_w,
// 	         board_h);
// 	border_buffer = alloc_drm_buf(&border_buffer_fd, board_w, board_h, 32);
// 	text_w = font_size * font_num;
// 	text_h = font_size;
// 	text_buffer = alloc_drm_buf(&text_buffer_fd, text_w, text_h, 32);
// 	memset(text_buffer, 0x00, text_w * text_h * 4);
// 	set_font_color(0x0000ffff);
// 	draw_argb8888_text(text_buffer, text_w, text_h, L"检测到人员入侵！"); // ARGB

// 	// set regions, need origin / normalized * 1000
// 	int normalized_screen_width = rk_param_get_int("osd.common:normalized_screen_width", -1);
// 	int normalized_screen_height = rk_param_get_int("osd.common:normalized_screen_height", -1);
// 	g_enabled = rk_param_get_int("event.regional_invasion:enabled", 0);
// 	LOG_INFO("g_enabled is %d\n", g_enabled);
// 	if (!g_enabled)
// 		return 1;
// 	ivm_regions_t regions;
// 	memset(&regions, 0, sizeof(regions));
// 	int x = (double)position_x / (double)normalized_screen_width * 1000.0;
// 	int y = (double)position_y / (double)normalized_screen_height * 1000.0;
// 	int w = (double)width / (double)normalized_screen_width * 1000.0;
// 	int h = (double)height / (double)normalized_screen_height * 1000.0;
// 	LOG_INFO("normalized x,y,w,h is %d,%d,%d,%d\n", x, y, w, h);
// 	regions.regions_num = 1;
// 	regions.regions->points_num = 4;
// 	regions.regions->points[0].x = x;
// 	regions.regions->points[0].y = y;
// 	regions.regions->points[1].x = x + w;
// 	regions.regions->points[1].y = y;
// 	regions.regions->points[2].x = x;
// 	regions.regions->points[2].y = y + h;
// 	regions.regions->points[3].x = x + w;
// 	regions.regions->points[3].y = y + h;
// 	ivm_set_regions(ivm, IVM_BORDER_ALARM, &regions);
// 	int sensitivity_level = rk_param_get_int("event.regional_invasion:sensitivity_level", 3);
// 	ivm_set_sensitivity(ivm, IVM_BORDER_ALARM, sensitivity_level);

// 	return 0;
// }

// int rk_event_init() {
// 	LOG_INFO("%s\n", __func__);
// 	// get g_x_rate and g_y_rate
// 	int video_width = rk_param_get_int("video.0:width", -1);
// 	int video_height = rk_param_get_int("video.0:height", -1);
// 	int normalized_screen_width = rk_param_get_int("osd.common:normalized_screen_width", -1);
// 	int normalized_screen_height = rk_param_get_int("osd.common:normalized_screen_height", -1);
// 	g_x_rate = (double)video_width / (double)normalized_screen_width;
// 	g_y_rate = (double)video_height / (double)normalized_screen_height;
// 	LOG_INFO("g_x_rate is %lf, g_y_rate is %lf\n", g_x_rate, g_y_rate);

// 	// ivm init
// 	ivm_dpd_attr_t g_attr = {0};
// 	g_attr.attr.detect_width = rk_param_get_int("video.0:width", -1);
// 	g_attr.attr.detect_height = rk_param_get_int("video.0:height", -1);
// 	LOG_INFO("w is %d, h is %d\n", g_attr.attr.detect_width, g_attr.attr.detect_height);
// 	sprintf(g_attr.attr.runtime_so_path, "/usr/lib/librknn_runtime.so");
// 	sprintf(g_attr.attr.data_path, "/usr/lib/object_detection_ipc.data");

// 	sprintf(g_attr.attr.lic_path, "/userdata/key.lic");             // do nothing
// 	sprintf(g_attr.attr.face_db_path, "/userdata/face.db");         // do nothing
// 	sprintf(g_attr.attr.face_db_bak_path, "/userdata/face_bak.db"); // do nothing
// 	ivm = ivm_dpd_init(&g_attr);
// 	LOG_INFO("ivm is %p\n", ivm);
// 	ivm_open(ivm, IVM_DETECT, NULL, ivm_event_cb);
// 	ivm_open(ivm, IVM_BORDER_ALARM, NULL, ivm_event_cb);
// 	rk_event_ri_set_regions();
// 	time(&alarm_time);
// 	time(&current_time);

// 	event_init_over_flag = 1;

// 	return 0;
// }

// int rk_event_deinit() {
// 	LOG_INFO("%s\n", __func__);
// 	if (!event_init_over_flag)
// 		return 0;
// 	event_init_over_flag = 0;
// 	ivm_close(ivm, IVM_BORDER_ALARM);
// 	ivm_deinit(ivm);
// 	close(border_buffer_fd);
// 	close(text_buffer_fd);

// 	return 0;
// }

// int event_process(int fd, void *addr, int width, int height, size_t size,
//                   unsigned long long timestamp) {
// 	int ret;
// 	rga_buffer_t pat;
// 	rga_buffer_t src;

// 	// LOG_INFO("event_init_over_flag is %d, g_enabled is %d\n",event_init_over_flag,g_enabled);
// 	if (!event_init_over_flag || !g_enabled)
// 		return 0;
// 	ret = ivm_dpd_detect_send(ivm, fd, addr, size, timestamp);
// 	// LOG_DEBUG("w is %d, h is %d, pts is %lld\n", width, height, timestamp);
// 	ret |= ivm_dpd_draw(ivm, fd, timestamp, width, height, -1, IVM_DRAW_PEOPLE | IVM_DRAW_FACE);
// 	// if current_time - alarm_time > 2, and already_alarm_flag is 1, disable osd;
// 	time(&current_time);

// 	memset(border_buffer, 0x00, board_w * board_h * 4);
// 	BorderInfo border_info;
// 	border_info.rect.w = board_w;
// 	border_info.rect.h = board_h;
// 	border_info.thick = 10;
// 	if ((difftime(current_time, alarm_time) < g_time_threshold)) {
// 		border_info.display_style = BORDER_WATERFULL_LIGHT;
// 		// text overlay
// 		pat = wrapbuffer_fd(text_buffer_fd, text_w, text_h, RK_FORMAT_BGRA_8888);
// 		src = wrapbuffer_fd(fd, width, height, RK_FORMAT_YCbCr_420_SP);
// 		im_rect src_text_rect = {board_x, board_y, text_w, text_h};
// 		im_rect pat_text_rect = {0, 0, text_w, text_h};
// 		src.color_space_mode = IM_YUV_TO_RGB_BT601_FULL | IM_RGB_TO_YUV_BT601_FULL;
// 		ret = improcess(src, src, pat, src_text_rect, src_text_rect, pat_text_rect,
// 		                IM_ALPHA_BLEND_DST_OVER | IM_ALPHA_BLEND_PRE_MUL);
// 		if (ret != IM_STATUS_SUCCESS) {
// 			LOG_ERROR("text improcess ret is %d, reason: %s\n", ret, imStrError(ret));
// 		}
// 	} else {
// 		border_info.display_style = BORDER_DOTTED;
// 	}

// 	border_info.interval = (board_w + board_h) / 40;
// 	border_info.color = 0xff0000ff;
// 	border_info.color_key = 0x00000000;
// 	// 下面这三个参数无效
// 	border_info.rect.x = 0;
// 	border_info.rect.y = 0;
// 	border_info.dotted_offset = 0;
// 	draw_border((uint32_t *)border_buffer, border_info);

// 	// save_argb8888_to_bmp(border_buffer, board_w, board_h);
// 	// LOG_INFO("rgba size is %d, nv12 size is %d\n", board_w*board_h*4, size);
// 	pat = wrapbuffer_fd(border_buffer_fd, board_w, board_h, RK_FORMAT_BGRA_8888);
// 	src = wrapbuffer_fd(fd, width, height, RK_FORMAT_YCbCr_420_SP);
// 	im_rect src_rect = {board_x, board_y, board_w, board_h};
// 	im_rect pat_rect = {0, 0, board_w, board_h};
// 	src.color_space_mode = IM_YUV_TO_RGB_BT601_FULL | IM_RGB_TO_YUV_BT601_FULL;
// 	ret = improcess(src, src, pat, src_rect, src_rect, pat_rect,
// 	                IM_ALPHA_BLEND_DST_OVER | IM_ALPHA_BLEND_PRE_MUL);
// 	if (ret != IM_STATUS_SUCCESS) {
// 		LOG_ERROR("board improcess ret is %d, reason: %s\n", ret, imStrError(ret));
// 	}

// 	return ret;
// }

// regional invasion
int rk_event_ri_get_enabled(int *value) {
	*value = rk_param_get_int("event.regional_invasion:enabled", -1);

	return 0;
}

int rk_event_ri_set_enabled(int value) {
	rk_param_set_int("event.regional_invasion:enabled", value);
	// rk_event_ri_set_regions();
	return 0;
}

int rk_event_ri_get_position_x(int *value) {
	*value = rk_param_get_int("event.regional_invasion:position_x", -1);

	return 0;
}

int rk_event_ri_set_position_x(int value) {
	rk_param_set_int("event.regional_invasion:position_x", value);
	// rk_event_ri_set_regions();
	return 0;
}

int rk_event_ri_get_position_y(int *value) {
	*value = rk_param_get_int("event.regional_invasion:position_y", -1);

	return 0;
}

int rk_event_ri_set_position_y(int value) {
	rk_param_set_int("event.regional_invasion:position_y", value);
	// rk_event_ri_set_regions();
	return 0;
}

int rk_event_ri_get_width(int *value) {
	*value = rk_param_get_int("event.regional_invasion:width", -1);

	return 0;
}

int rk_event_ri_set_width(int value) {
	rk_param_set_int("event.regional_invasion:width", value);
	// rk_event_ri_set_regions();
	return 0;
}

int rk_event_ri_get_height(int *value) {
	*value = rk_param_get_int("event.regional_invasion:height", -1);

	return 0;
}

int rk_event_ri_set_height(int value) {
	rk_param_set_int("event.regional_invasion:height", value);
	// rk_event_ri_set_regions();
	return 0;
}

int rk_event_ri_get_proportion(int *value) {
	*value = rk_param_get_int("event.regional_invasion:proportion", -1);

	return 0;
}

int rk_event_ri_set_proportion(int value) {
	rk_param_set_int("event.regional_invasion:proportion", value);
	// rk_event_ri_set_regions();
	return 0;
}

int rk_event_ri_get_sensitivity_level(int *value) {
	*value = rk_param_get_int("event.regional_invasion:sensitivity_level", -1);

	return 0;
}

int rk_event_ri_set_sensitivity_level(int value) {
	rk_param_set_int("event.regional_invasion:sensitivity_level", value);
	// // [0-100] → [0-5]
	// ivm_set_sensitivity(ivm, IVM_BORDER_ALARM, (int)(value / 20));
	// // rk_event_ri_set_regions();
	return 0;
}

int rk_event_ri_get_time_threshold(int *value) {
	*value = rk_param_get_int("event.regional_invasion:time_threshold", 3);

	return 0;
}

int rk_event_ri_set_time_threshold(int value) {
	rk_param_set_int("event.regional_invasion:time_threshold", value);
	// g_time_threshold = value;
	// // rk_event_ri_set_regions();
	return 0;
}
