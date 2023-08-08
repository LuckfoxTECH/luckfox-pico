// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include <fcntl.h>
#include <getopt.h>
#include <linux/serial.h>
#include <sys/ioctl.h>
#include <termios.h>

#include "audio.h"
#include "common.h"
#include "isp.h"
#include "log.h"
#include "param.h"
#include "rockiva.h"
#include "tuya_ipc.h"
#include "video.h"

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

static void uart_to_mcu_poweroff() {
	int fd;
	int baud = B115200;
	struct termios newtio;
	struct serial_rs485 rs485;

	fd = open("/dev/ttyS5", O_RDWR | O_NONBLOCK);
	if (fd < 0) {
		LOG_ERROR("Error opening serial port\n");
		return;
	}

	bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */

	/* man termios get more info on below settings */
	newtio.c_cflag = baud | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = 0;
	newtio.c_oflag = 0;
	newtio.c_lflag = 0;
	// block for up till 128 characters
	newtio.c_cc[VMIN] = 128;
	// 0.5 seconds read timeout
	newtio.c_cc[VTIME] = 5;
	/* now clean the modem line and activate the settings for the port */
	tcflush(fd, TCIOFLUSH);
	tcsetattr(fd, TCSANOW, &newtio);
	if (ioctl(fd, TIOCGRS485, &rs485) >= 0) {
		/* disable RS485 */
		rs485.flags &= ~(SER_RS485_ENABLED | SER_RS485_RTS_ON_SEND | SER_RS485_RTS_AFTER_SEND);
		rs485.delay_rts_after_send = 0;
		rs485.delay_rts_before_send = 0;
		if (ioctl(fd, TIOCSRS485, &rs485) < 0) {
			perror("Error setting RS-232 mode");
		}
	}

	/*
	 * The flag ASYNC_SPD_CUST might have already been set, so
	 * clear it to avoid confusing the kernel uart dirver.
	 */
	struct serial_struct ss;
	if (ioctl(fd, TIOCGSERIAL, &ss) < 0) {
		// return silently as some devices do not support TIOCGSERIAL
		LOG_INFO("return silently as some devices do not support TIOCGSERIAL\n");
		return;
	}
	// if ((ss.flags & ASYNC_SPD_MASK) != ASYNC_SPD_CUST)
	// 	return;
	ss.flags &= ~ASYNC_SPD_MASK;
	if (ioctl(fd, TIOCSSERIAL, &ss) < 0) {
		LOG_ERROR("TIOCSSERIAL failed");
		return;
	}

	// write
	int written = write(fd, "\x11\x26", 4);
	if (written < 0)
		LOG_ERROR("write()");
	else
		LOG_INFO("written is %d\n", written);
}

static void sigterm_handler(int signo) {
	LOG_INFO("received signo %d \n", signo);

	rk_tuya_low_power_enable();

	// system("poweroff");
	LOG_WARN("after poweroff\n");
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

extern void ioctl_msg_func(int sig_num);

int main(int argc, char **argv) {
	LOG_INFO("%s: main begin\n", get_time_string());
	signal(SIGINT, sigterm_handler);
	signal(SIGQUIT, sigterm_handler);
	signal(SIGINT, sigterm_handler);
	signal(SIGTERM, sigterm_handler);
	signal(SIGXCPU, sigterm_handler);

	rkipc_get_opt(argc, argv);
	LOG_INFO("rkipc_ini_path_ is %s, rkipc_iq_file_path_ is %s, rkipc_log_level "
	         "is %d\n",
	         rkipc_ini_path_, rkipc_iq_file_path_, rkipc_log_level);
	if (rkipc_ini_path_ == NULL)
		rkipc_ini_path_ = "/usr/share/rkipc.ini";

	int ssFd = -1, viDev = -1;
	const char *ssName = "/dev/video8";
	ssFd = open(ssName, O_RDWR | O_CLOEXEC);
	if (ssFd < 0) {
		LOG_ERROR("failed to open camera:%s\n", ssName);
		return -1;
	}
	viDev = open("/dev/mpi/vvi", O_RDONLY);
	if (viDev < 0) {
		LOG_ERROR("failed to open /dev/mpi/vvi\n");
		return -1;
	}

	// init
	rk_param_init(rkipc_ini_path_);
	LOG_INFO("%s: rk_param_init over\n", get_time_string());
	RK_MPI_SYS_Init();
	LOG_INFO("%s: RK_MPI_SYS_Init over\n", get_time_string());
	// venc
	rkipc_pipe_0_venc_init();
	LOG_INFO("%s: rkipc_pipe_0_venc_init over\n", get_time_string());
	// aiq
	rk_isp_init(0, rkipc_iq_file_path_);
	LOG_INFO("%s: rk_isp_init over\n", get_time_string());
	rk_isp_set_frame_rate_without_ini(0, rk_param_get_int("isp.0.adjustment:fps", 15));
	LOG_INFO("%s: rk_isp_set_frame_rate_without_ini over\n", get_time_string());
	// vi
	rkipc_vi_dev_init();
	LOG_INFO("%s: rkipc_vi_dev_init over\n", get_time_string());
	rkipc_pipe_0_vi_init();
	LOG_INFO("%s: rkipc_pipe_0_vi_init over\n", get_time_string());
	// bind
	rkipc_pipe_0_bind();
	LOG_INFO("%s: rkipc_pipe_0_bind over\n", get_time_string());

	atbm_init();
	LOG_INFO("%s: atbm_init over\n", get_time_string());
	get_status();
	LOG_INFO("%s: get_status over\n", get_time_string());

	if (rk_param_get_int("tuya:enable", 0))
		rk_tuya_init();
	LOG_INFO("%s: rk_tuya_init over\n", get_time_string());

	sleep(2); // wait 2s , avoid preempting CPU to affect outgoing speed
	// second stream and rtsp
	rk_video_init();
	LOG_INFO("%s: rk_video_init over\n", get_time_string());
	// if (rk_param_get_int("audio.0:enable", 0))
	// 	rkipc_audio_init();
	// LOG_INFO("%s: rkipc_audio_init over\n", get_time_string());

	if (rk_param_get_int("video.1:enable_npu", 0))
		rkipc_rockiva_init();
	LOG_INFO("%s: rkipc_rockiva_init over\n", get_time_string());

	while (g_main_run_) {
		usleep(1000 * 1000);
	}

	// deinit
	if (rk_param_get_int("tuya:enable", 0))
		rk_tuya_deinit();
	rk_video_deinit();
	rk_isp_deinit(0);
	if (rk_param_get_int("audio.0:enable", 0))
		rkipc_audio_deinit();
	RK_MPI_SYS_Exit();
	if (rk_param_get_int("video.1:enable_npu", 0))
		rkipc_rockiva_deinit();
	rk_param_deinit();

	return 0;
}
