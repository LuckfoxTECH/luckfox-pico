// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "common.h"
#include "version.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "common.c"

typedef struct rk_signal_t {
	sem_t sem;
	int max_val;
} rk_signal_t;

/**
 * @brief 创建信号量
 *
 * @param defval 取值为0
 * @param maxval 取值为1
 *
 * @return 成功,返回信号量句柄; 否则,返回空值
 */
void *rk_signal_create(int defval, int maxval) {
	rk_signal_t *h = (rk_signal_t *)malloc(sizeof(rk_signal_t));

	if (h == NULL) {
		return NULL;
	}
	if (sem_init(&(h->sem), 0, defval) == -1) { /* 初始化信号量失败,失败原因见error */
		perror("sem_init: ");
		free(h);
		h = NULL;
	} else
		h->max_val = maxval;
	return h;
}

/**
 * @brief 销毁信号量
 *
 * @param signal 信号量句柄
 */
void rk_signal_destroy(void *sem) {
	if (sem == NULL) {
		return;
	}

	sem_destroy((sem_t *)sem);
	free(sem);
}

/**
 * @brief 等待信号量
 *
 * @param signal 信号量句柄
 * @param timeout -1表示无限等待;其他值表示等待的时间(ms)
 *
 * @return 成功,返回0; 否则,返回-1
 */
int rk_signal_wait(void *sem, int timeout) {
	struct timespec tv;

	if (sem == NULL) {
		return 0;
	}

	if (timeout < 0) { /* 需要判断返回值,因为如果信号量被destroy了也会返回的 */
		return sem_wait((sem_t *)sem) == 0 ? 0 : -1;
	} else {
		clock_gettime(CLOCK_REALTIME, &tv);

		tv.tv_nsec += (timeout % 1000) * 1000000;
		if (tv.tv_nsec >= 1000000000) {
			tv.tv_sec += 1;
			tv.tv_nsec -= 1000000000;
		}
		tv.tv_sec += timeout / 1000;
		if (sem_timedwait((sem_t *)sem, (const struct timespec *)&tv)) {
			return -1;
		}
		return 0;
	}
}

/**
 * @brief 释放信号量
 *
 * @param signal 信号量句柄
 */
void rk_signal_give(void *sem) {
	int val;

	if (sem == NULL) {
		return;
	}

	sem_getvalue((sem_t *)sem, &val);

	if (val < ((rk_signal_t *)sem)->max_val) {
		sem_post((sem_t *)sem);
	}
}

/**
 * @brief 重置信号量
 *
 * @param signal 信号量句柄
 */
void rk_signal_reset(void *sem) { rk_signal_give(sem); }

long long rkipc_get_curren_time_ms() {
	long long msec = 0;
	char str[20] = {0};
	struct timespec current_time = {0, 0};

	clock_gettime(CLOCK_MONOTONIC, &current_time);
	sprintf(str, "%ld%03ld", current_time.tv_sec, (current_time.tv_nsec) / 1000000);
	for (size_t i = 0; i < strlen(str); i++) {
		msec = msec * 10 + (str[i] - '0');
	}

	return msec;
}

char *get_time_string() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	struct tm *timeinfo = localtime(&tv.tv_sec);
	static char time_string[64];
	sprintf(time_string, "%.2d:%.2d:%.2d.%.6ld", timeinfo->tm_hour, timeinfo->tm_min,
	        timeinfo->tm_sec, tv.tv_usec);
	return time_string;
}

int read_cmdline_to_buf(void *buf, int len) {
	int fd;
	int ret;
	if (buf == NULL || len < 0) {
		printf("%s: illegal para\n", __func__);
		return -1;
	}
	memset(buf, 0, len);
	fd = open("/proc/cmdline", O_RDONLY);
	if (fd < 0) {
		perror("open:");
		return -1;
	}
	ret = read(fd, buf, len);
	close(fd);
	return ret;
}

long get_cmd_val(const char *string, int len) {
	char *addr;
	long value = 0;
	char key_equal[16];
	static char cmdline[1024];
	static char cmd_init = 0;

	if (cmd_init == 0) {
		cmd_init = 1;
		memset(cmdline, 0, sizeof(cmdline));
		read_cmdline_to_buf(cmdline, sizeof(cmdline));
	}

	snprintf(key_equal, sizeof(key_equal), "%s=", string);
	addr = strstr(cmdline, string);
	if (addr) {
		value = strtol(addr + strlen(string) + 1, NULL, len);
		printf("get %s value: 0x%0lx\n", string, value);
	}
	return value;
}

void rkipc_version_dump() {
	LOG_INFO("rkipc version: %s\n", RKIPC_VERSION_INFO);
	LOG_INFO("rkipc info: %s\n", RKIPC_BUILD_INFO);
	LOG_INFO("rkipc type: %s\n", RKIPC_TYPE);
}