// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef __RKSOCKET__
#define __RKSOCKET__

#define SOCKERR_IO -1
#define SOCKERR_CLOSED -2
#define SOCKERR_INVARG -3
#define SOCKERR_TIMEOUT -4
#define SOCKERR_OK 0

#define CS_PATH "/var/tmp/rkipc"

int serv_listen(const char *name);
int serv_accept(int fd);
int sock_write(int fd, const void *buff, int count);
int sock_read(int fd, void *buff, int count);

#endif