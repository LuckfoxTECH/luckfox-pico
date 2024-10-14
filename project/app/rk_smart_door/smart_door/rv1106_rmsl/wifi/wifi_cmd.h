// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef __WIFI_CMD_H__
#define __WIFI_CMD_H__

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <malloc.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define CMD_MAX_LEN         1500
#define SOCK_BUF_MAX        1500
#define SOCK_PORT           8822
#define SOCK_EVENT_PORT     8823

#define WAKE_CODE_FIRST     ((0x0F << 8) + 0)
#define WAKE_CODE_NET       ((0x0F << 8) + 1)
#define WAKE_CODE_PIR       ((0x0F << 8) + 2)
#define WAKE_CODE_KEY       ((0x0F << 8) + 3)

#define EVENT_ID_KEY        ((0x16 << 8) + 0)
#define EVENT_ID_PIR        ((0x16 << 8) + 3)

void smart_door_wifi_cmd(void);

#endif
