// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <inttypes.h> // PRId64
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "iniparser.h"
#include "log.h"
#include "param.h"

#ifdef __GNUC__
#define RKIPC_MAYBE_UNUSED __attribute__((unused))
#else
#define RKIPC_MAYBE_UNUSED
#endif

#define UPALIGNTO(value, align) ((value + align - 1) & (~(align - 1)))
#define UPALIGNTO2(value) UPALIGNTO(value, 2)
#define UPALIGNTO4(value) UPALIGNTO(value, 4)
#define UPALIGNTO16(value) UPALIGNTO(value, 16)
#define DOWNALIGNTO16(value) (UPALIGNTO(value, 16) - 16)
#define MULTI_UPALIGNTO16(grad, value) UPALIGNTO16((int)(grad * value))

#ifdef RKIPC_SWAP
#undef RKIPC_SWAP
#endif
#define RKIPC_SWAP(x, y)                                                                           \
	{                                                                                              \
		(void)(&x == &y);                                                                          \
		typeof(x) __tmp = (x);                                                                     \
		(x) = (y);                                                                                 \
		(y) = __tmp;                                                                               \
	}

#ifdef RKIPC_ASSERT
#undef RKIPC_ASSERT
#endif
#define RKIPC_ASSERT(cond, err_msg, ...)                                                           \
	{                                                                                              \
		if (!(cond))                                                                               \
			LOG_ERROR("ASSERT FAILED %s\n", err_msg, ##__VA_ARGS__);                               \
		assert(cond);                                                                              \
	}

void *rk_signal_create(int defval, int maxval);
void rk_signal_destroy(void *sem);
int rk_signal_wait(void *sem, int timeout);
void rk_signal_give(void *sem);
void rk_signal_reset(void *sem);

long long rkipc_get_curren_time_ms();
char *get_time_string();
int read_cmdline_to_buf(void *buf, int len);
long get_cmd_val(const char *string, int len);
void rkipc_version_dump();
