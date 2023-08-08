// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include <getopt.h>

#include "common.h"
#include "isp.h"
#include "log.h"
#include "rkdrm_display.h"
#include <fcntl.h>
#include <linux/videodev2.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "rkipc.c"

enum { LOG_ERROR, LOG_WARN, LOG_INFO, LOG_DEBUG };

static int g_main_run_ = 1;
int enable_minilog = 0;
int rkipc_log_level = LOG_DEBUG;
int rkipc_dump_num = 5;
char *rkipc_iq_file_path_ = NULL;
char *rkipc_subdev_ = NULL;
char *rkipc_device_ = NULL;

struct buffer {
	void *start;
	size_t length;
	int export_fd;
	int sequence;
};

int input_width = 1280;
int input_height = 720;
int output_width = 1280;
int output_height = 720;
int x_offset = 0;
int y_offset = 0;

int isp_fd;
struct display g_disp;
struct buffer *tmp_buffers = NULL;
struct v4l2_buffer buf;

static void sig_proc(int signo) {
	LOG_INFO("received signo %d \n", signo);
	g_main_run_ = 0;
}

static const char short_options[] = "w:h:a:l:";
static const struct option long_options[] = {{"input_width", required_argument, NULL, 'w'},
                                             {"input_height", required_argument, NULL, 'h'},
                                             {"aiq_file", no_argument, NULL, 'a'},
                                             {"log_level", no_argument, NULL, 'l'},
                                             {0, 0, 0, 0}};

static void usage_tip(FILE *fp, int argc, char **argv) {
	fprintf(fp,
	        "Usage: %s [options]\n"
	        "Version %s\n"
	        "Options:\n"
	        "-w | --input_width	 input width\n"
	        "-h | --input_height input height\n"
	        "-a | --aiq_file    aiq file dir path, default is /etc/iqfiles\n"
	        "-l | --log_level   log_level [0/1/2/3], default is 2\n"
	        "\n",
	        argv[0], "V1.0");
}

void rkipc_get_opt(int argc, char *argv[]) {
	for (;;) {
		int idx;
		int c;
		c = getopt_long(argc, argv, short_options, long_options, &idx);
		if (-1 == c)
			break;
		switch (c) {
		case 0: /* getopt_long() flag */
			break;
		case 'w':
			input_width = atoi(optarg);
			break;
		case 'h':
			input_height = atoi(optarg);
			break;
		case 'a':
			rkipc_iq_file_path_ = optarg;
			break;
		case 'l':
			rkipc_log_level = atoi(optarg);
			break;
		default:
			usage_tip(stderr, argc, argv);
			exit(EXIT_FAILURE);
		}
	}
}

static long getCurrentTimeMsec() {
	long msec = 0;
	char str[20] = {0};
	struct timeval stuCurrentTime;

	gettimeofday(&stuCurrentTime, NULL);
	sprintf(str, "%ld%03ld", stuCurrentTime.tv_sec, (stuCurrentTime.tv_usec) / 1000);
	for (size_t i = 0; i < strlen(str); i++) {
		msec = msec * 10 + (str[i] - '0');
	}

	return msec;
}

static void *get_vi_send_vo(void *arg) {
	printf("#Start %s thread, arg:%p\n", __func__, arg);
	prctl(PR_SET_NAME, "get_vi_send_vo", 0, 0, 0);
	int v4l2_index = 0;
	int ret = 0;
	struct v4l2_crop crop;
	ioctl(isp_fd, VIDIOC_G_CROP, &crop);
	long now_time;

	while (g_main_run_) {
		// crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
		// crop.c.left = x_offset;
		// crop.c.top = y_offset;
		// crop.c.width = 1280;
		// crop.c.height = 720;
		// ret = ioctl(isp_fd, VIDIOC_S_CROP, &crop);
		// printf("crop left top is %d,%d\n", crop.c.left, crop.c.top);

		ret = ioctl(isp_fd, VIDIOC_DQBUF, &buf);
		if (ret == -1) {
			printf("VIDIOC_DQBUF fail\n");
			return NULL;
		}
		// now_time = getCurrentTimeMsec();
#if 1
		for (int i = 0; i < output_height; i++) {
			memcpy(g_disp.buf[v4l2_index].map + i * output_width * 4,
			       tmp_buffers[v4l2_index].start + (i + y_offset) * input_width * 4 + x_offset * 4,
			       output_width * 4);
		}
#else
		memcpy(g_disp.buf[v4l2_index].map, tmp_buffers[v4l2_index].start,
		       output_width * output_height * 4);
#endif
		// printf("memcpy use time:%ld\n", getCurrentTimeMsec() - now_time);
		drmCommit(&g_disp.buf[v4l2_index], output_width, output_height, 0, 0, &g_disp.dev,
		          g_disp.plane_type);

		ret = ioctl(isp_fd, VIDIOC_QBUF, &buf);
		if (ret == -1) {
			printf("VIDIOC_QBUF fail\n");
		}

		v4l2_index++;
		if (v4l2_index == BUF_COUNT)
			v4l2_index = 0;
	}

	return 0;
}

#define FMT_NUM_PLANES 1
int main(int argc, char **argv) {
	LOG_INFO("main begin\n");
	signal(SIGINT, sig_proc);
	int ret;

	rkipc_get_opt(argc, argv);
	LOG_INFO("rkipc_iq_file_path_ is %s, rkipc_log_level is %d\n", rkipc_iq_file_path_,
	         rkipc_log_level);
	LOG_INFO("input_width is %d, input_height is %d\n", input_width, input_height);

	memset(&g_disp, 0, sizeof(g_disp));
	g_disp.fmt = DRM_FORMAT_XRGB8888;
	g_disp.width = output_width;
	g_disp.height = output_height;
	g_disp.plane_type = DRM_PLANE_TYPE_PRIMARY;
	g_disp.buf_cnt = BUF_COUNT;
	drm_display_init(&g_disp);

	printf("g_disp.buf[0].map is %p\n", g_disp.buf[0].map);

	// // read file to buffer
	// FILE *pfile = fopen("/userdata/read.yuv", "rb");
	// if (pfile == NULL) {
	// 	printf("yuv file open fail!\n");
	// 	return -1;
	// }
	// if (fread(g_disp.buf[0].map, 1, input_width * input_height * 4, pfile) <= 0) {
	// 	printf("bmp_data fread fail!\n");
	// 	return -1;
	// }

	// drmCommit(&g_disp.buf[0], input_width, input_height, 0, 0, &g_disp.dev, g_disp.plane_type);
	// while (g_main_run_) {
	// 	sleep(1);
	// }
	// return 0;

	rk_isp_init(0, rkipc_iq_file_path_);

	enum v4l2_buf_type buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	struct v4l2_plane planes[FMT_NUM_PLANES];

	isp_fd = open("/dev/video9", O_RDWR /* required */ /*| O_NONBLOCK*/, 0);
	if (-1 == isp_fd) {
		printf("Cannot open /dev/video9: %d, %s\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}
	printf("isp_fd is %d\n", isp_fd);

	// 设置视频捕获格式
	struct v4l2_format fmt;
	memset(&fmt, 0, sizeof(fmt));
	fmt.type = buf_type;
	fmt.fmt.pix.width = input_width;
	fmt.fmt.pix.height = input_height;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_XBGR32;
	fmt.fmt.pix.field = V4L2_FIELD_NONE;
	if (ioctl(isp_fd, VIDIOC_S_FMT, &fmt) == -1) {
		printf("VIDIOC_S_FMT fail\n");
		return -1;
	}
	printf("VIDIOC_S_FMT success\n");

	// 分配视频捕获内存
	struct v4l2_requestbuffers req;
	memset(&req, 0, sizeof(req));
	req.count = BUF_COUNT;
	req.type = buf_type;
	req.memory = V4L2_MEMORY_MMAP;
	if (ioctl(isp_fd, VIDIOC_REQBUFS, &req) == -1) {
		printf("VIDIOC_REQBUFS fail\n");
		return -1;
	}
	printf("VIDIOC_REQBUFS success\n");

	// 获取并记录缓存的物理空间
	tmp_buffers = (struct buffer *)calloc(req.count, sizeof(struct buffer));
	for (int numBufs = 0; numBufs < req.count; numBufs++) {
		memset(&buf, 0, sizeof(buf));
		buf.type = buf_type;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = numBufs;
		if (V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE == buf_type) {
			buf.m.planes = planes;
			buf.length = FMT_NUM_PLANES;
		}
		// 读取缓存
		if (ioctl(isp_fd, VIDIOC_QUERYBUF, &buf) == -1) {
			printf("VIDIOC_QUERYBUF fail\n");
			return -1;
		}

		if (buf_type == V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE) {
			tmp_buffers[numBufs].length = buf.m.planes[0].length;
			tmp_buffers[numBufs].start =
			    mmap(NULL /* start anywhere */, buf.m.planes[0].length,
			         PROT_READ | PROT_WRITE /* required */, MAP_SHARED /* recommended */, isp_fd,
			         buf.m.planes[0].m.mem_offset);
		} else {
			tmp_buffers[numBufs].length = buf.length;
			tmp_buffers[numBufs].start =
			    mmap(NULL /* start anywhere */, buf.length, PROT_READ | PROT_WRITE /* required */,
			         MAP_SHARED /* recommended */, isp_fd, buf.m.offset);
		}

		if (tmp_buffers[numBufs].start == MAP_FAILED) {
			printf("start == MAP_FAILED\n");
			return -1;
		}

		// export buf dma fd
		struct v4l2_exportbuffer expbuf;
		memset(&expbuf, 0, sizeof(expbuf));
		expbuf.type = buf_type;
		expbuf.index = numBufs;
		expbuf.flags = O_CLOEXEC;
		if (ioctl(isp_fd, VIDIOC_EXPBUF, &expbuf) < 0) {
			printf("get dma buf failed\n");
			return -1;
		} else {
			printf(" get dma buf(%d)-fd: %d\n", numBufs, expbuf.fd);
		}
		tmp_buffers[numBufs].export_fd = expbuf.fd;

		// 放入缓存队列
		if (ioctl(isp_fd, VIDIOC_QBUF, &buf) == -1) {
			printf("VIDIOC_QBUF fail\n");
			return -1;
		}
	}

	ret = ioctl(isp_fd, VIDIOC_STREAMON, &buf_type);
	if (ret == -1) {
		printf("VIDIOC_STREAMON fail\n");
		return -1;
	} else {
		printf("VIDIOC_STREAMON success\n");
	}

	// 读取缓存
	memset(&buf, 0, sizeof(buf));
	buf.type = buf_type;
	buf.memory = V4L2_MEMORY_MMAP;
	buf.index = 0;
	memset(planes, 0, sizeof(struct v4l2_plane) * FMT_NUM_PLANES);
	if (V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE == buf_type) {
		buf.m.planes = planes;
		buf.length = FMT_NUM_PLANES;
	}

	pthread_t thread_id;
	pthread_create(&thread_id, NULL, get_vi_send_vo, NULL);

	char c;
	struct termios tTTYState;

	// get the terminal state
	tcgetattr(STDIN_FILENO, &tTTYState);
	// turn off canonical mode
	tTTYState.c_lflag &= ~ICANON;
	// minimum of number input read.
	tTTYState.c_cc[VMIN] = 1; /* 有一个数据时就立刻返回 */
	// set the terminal attributes.
	tcsetattr(STDIN_FILENO, TCSANOW, &tTTYState);
	while (g_main_run_) {
		c = fgetc(stdin); /* 会休眠直到有输入 */
		printf("get char : %c\n", c);
		switch (c) {
		case 'A':
			x_offset -= 100;
			if (x_offset < 0)
				x_offset = 0;
			break;
		case 'D':
			x_offset += 100;
			if (x_offset > output_width)
				x_offset = output_width;
			break;
		case 'W':
			y_offset -= 100;
			if (y_offset < 0)
				y_offset = 0;
			break;
		case 'S':
			y_offset += 100;
			if (y_offset > output_height)
				y_offset = output_height;
			break;
		case 'Q':
			g_main_run_ = 0;
			break;
		default:
			break;
		}
		printf("x,y is %d,%d\n", x_offset, y_offset);
	}
	close(isp_fd);
	rk_isp_deinit(0);
	free(tmp_buffers);
	//	fclose(pfile);

	return 0;
}
