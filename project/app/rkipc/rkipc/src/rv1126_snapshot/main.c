// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include <getopt.h>

#include "common.h"
#include "isp.h"
#include "log.h"
#include "video.h"

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

static void sig_proc(int signo) {
	LOG_INFO("received signo %d \n", signo);
	g_main_run_ = 0;
}

static const char short_options[] = "s:d:n::a:l:";
static const struct option long_options[] = {{"subdev", required_argument, NULL, 's'},
                                             {"device", required_argument, NULL, 'd'},
                                             {"num", required_argument, NULL, 'n'},
                                             {"aiq_file", no_argument, NULL, 'a'},
                                             {"log_level", no_argument, NULL, 'l'},
                                             {"help", no_argument, NULL, 'h'},
                                             {0, 0, 0, 0}};

static void usage_tip(FILE *fp, int argc, char **argv) {
	fprintf(fp,
	        "Usage: %s [options]\n"
	        "Version %s\n"
	        "Options:\n"
	        "-s | --subdev		real camera isp sub devices\n"
	        "-d | --device		fake camera video devices\n"
	        "-n | --num			dump num\n"
	        "-a | --aiq_file    aiq file dir path, default is /etc/iqfiles\n"
	        "-l | --log_level   log_level [0/1/2/3], default is 2\n"
	        "-h | --help        for help \n\n"
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
		case 's':
			rkipc_subdev_ = optarg;
			break;
		case 'd':
			rkipc_device_ = optarg;
			break;
		case 'n':
			rkipc_dump_num = atoi(optarg);
			break;
		case 'a':
			rkipc_iq_file_path_ = optarg;
			break;
		case 'l':
			rkipc_log_level = atoi(optarg);
			break;
		case 'h':
			usage_tip(stdout, argc, argv);
			exit(EXIT_SUCCESS);
		default:
			usage_tip(stderr, argc, argv);
			exit(EXIT_FAILURE);
		}
	}
}

int main(int argc, char **argv) {
	LOG_INFO("main begin\n");
	rkipc_version_dump();
	signal(SIGINT, sig_proc);

	rkipc_get_opt(argc, argv);
	LOG_INFO("rkipc_iq_file_path_ is %s, rkipc_log_level is %d\n", rkipc_iq_file_path_,
	         rkipc_log_level);

	// init
	rk_video_init(); // do isp_init in video_init for fast

	while (g_main_run_) {
		usleep(1000 * 1000);
	}

	// deinit
	rk_video_deinit();

	return 0;
}
