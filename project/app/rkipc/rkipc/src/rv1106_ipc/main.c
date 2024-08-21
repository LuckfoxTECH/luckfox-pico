// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include <getopt.h>

#include "audio.h"
#include "common.h"
#include "isp.h"
#include "log.h"
#include "network.h"
#include "param.h"
#include "rockiva.h"
#include "server.h"
#include "storage.h"
#include "system.h"
#include "video.h"
#include <linux/input.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "rkipc.c"

enum { LOG_ERROR, LOG_WARN, LOG_INFO, LOG_DEBUG };

int enable_minilog = 0;
int rkipc_log_level = LOG_INFO;

static int g_main_run_ = 1;
char *rkipc_ini_path_ = NULL;
char *rkipc_iq_file_path_ = NULL;

static void sig_proc(int signo) {
	LOG_INFO("received signo %d \n", signo);
	g_main_run_ = 0;
}

static const char short_options[] = "c:a:l:";
static const struct option long_options[] = {{"config", required_argument, NULL, 'c'},
                                             {"aiq_file", no_argument, NULL, 'a'},
                                             {"log_level", no_argument, NULL, 'l'},
                                             {"help", no_argument, NULL, 'h'},
                                             {0, 0, 0, 0}};

static void usage_tip(FILE *fp, int argc, char **argv) {
	fprintf(fp,
	        "Usage: %s [options]\n"
	        "Version %s\n"
	        "Options:\n"
	        "-c | --config      rkipc ini file, default is "
	        "/userdata/rkipc.ini, need to be writable\n"
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
		case 'c':
			rkipc_ini_path_ = optarg;
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

#define AO_FREAD_SIZE 1024 * 4
static void *wait_key_event(void *arg) {
	int key_fd;
	key_fd = open("/dev/input/event0", O_RDONLY);
	if (key_fd < 0) {
		LOG_ERROR("can't open /dev/input/event0\n");
		return NULL;
	}
	fd_set rfds;
	int nfds = key_fd + 1;
	struct timeval timeout;
	struct input_event key_event;

	while (g_main_run_) {
		// The rfds collection must be emptied every time,
		// otherwise the descriptor changes cannot be detected
		timeout.tv_sec = 1;
		FD_ZERO(&rfds);
		FD_SET(key_fd, &rfds);
		select(nfds, &rfds, NULL, NULL, &timeout);
		// wait for the key event to occur
		if (FD_ISSET(key_fd, &rfds)) {
			read(key_fd, &key_event, sizeof(key_event));
			LOG_INFO("[timeval:sec:%d,usec:%d,type:%d,code:%d,value:%d]\n", key_event.time.tv_sec,
			         key_event.time.tv_usec, key_event.type, key_event.code, key_event.value);
			if ((key_event.code == KEY_VOLUMEDOWN) && key_event.value) {
				LOG_INFO("get KEY_VOLUMEDOWN\n");
				rkipc_ao_init();
				FILE *fp = fopen("/oem/usr/share/speaker_test.wav", "rb");
				int size = AO_FREAD_SIZE;
				char *tmp_data;
				tmp_data = malloc(AO_FREAD_SIZE);
				while (size > 0) {
					memset((void *)tmp_data, 0, AO_FREAD_SIZE);
					size = fread(tmp_data, 1, AO_FREAD_SIZE, fp);
					rkipc_ao_write(tmp_data, AO_FREAD_SIZE);
				}
				rkipc_ao_write(tmp_data, 0);
				free(tmp_data);
				fclose(fp);
				rkipc_ao_deinit();
			}

			if ((key_event.code == KEY_VOLUMEUP) && key_event.value) {
				LOG_INFO("get KEY_VOLUMEUP\n");
			}
		}
	}

	if (key_fd) {
		close(key_fd);
		key_fd = 0;
	}
	LOG_DEBUG("wait key event out\n");
	return NULL;
}

int main(int argc, char **argv) {
	pthread_t key_chk;
	LOG_DEBUG("main begin\n");
	rkipc_version_dump();
	signal(SIGINT, sig_proc);
	signal(SIGTERM, sig_proc);

	rkipc_get_opt(argc, argv);
	LOG_INFO("rkipc_ini_path_ is %s, rkipc_iq_file_path_ is %s, rkipc_log_level "
	         "is %d\n",
	         rkipc_ini_path_, rkipc_iq_file_path_, rkipc_log_level);

	// init
	rk_param_init(rkipc_ini_path_);
	rk_network_init(NULL);
	rk_system_init();
	if (rk_param_get_int("video.source:enable_npu", 0))
		rkipc_rockiva_init();
	if (rk_param_get_int("video.source:enable_aiq", 1)) {
		rk_isp_init(0, rkipc_iq_file_path_);
		if (rk_param_get_int("isp:init_form_ini", 1))
			rk_isp_set_from_ini(0);
	}
	RK_MPI_SYS_Init();
	rk_video_init();
	if (rk_param_get_int("audio.0:enable", 0))
		rkipc_audio_init();
	rkipc_server_init();
	rk_storage_init();
	pthread_create(&key_chk, NULL, wait_key_event, NULL);

	while (g_main_run_) {
		usleep(1000 * 1000);
	}

	// deinit
	pthread_join(key_chk, NULL);
	rk_storage_deinit();
	rkipc_server_deinit();
	rk_system_deinit();
	rk_video_deinit();
	if (rk_param_get_int("video.source:enable_aiq", 1))
		rk_isp_deinit(0);
	if (rk_param_get_int("audio.0:enable", 0))
		rkipc_audio_deinit();
	RK_MPI_SYS_Exit();
	if (rk_param_get_int("video.source:enable_npu", 0))
		rkipc_rockiva_deinit();
	rk_network_deinit();
	rk_param_deinit();

	return 0;
}
