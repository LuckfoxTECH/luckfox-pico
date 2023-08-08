// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include <rga/im2d.h>
#include <rga/rga.h>
#include <rkmedia_api.h>
#include <rkmedia_venc.h>

#include <assert.h>
#include <fcntl.h>
#include <getopt.h>
#include <linux/serial.h>
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/random.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

int rk_video_init();
int rk_video_deinit();
