// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>

#include "socket.h"

union {
	struct cmsghdr cm;
	char control[CMSG_SPACE(sizeof(int))];
} control_un;

int serv_listen(const char *name) {
	int fd, len;
	struct sockaddr_un unix_addr;

	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
		return (-1);

	fcntl(fd, F_SETFD, FD_CLOEXEC);

	unlink(name);
	memset(&unix_addr, 0, sizeof(unix_addr));
	unix_addr.sun_family = AF_UNIX;
	strcpy(unix_addr.sun_path, name);
	len = sizeof(unix_addr.sun_family) + strlen(unix_addr.sun_path);

	if (bind(fd, (struct sockaddr *)&unix_addr, len) < 0)
		goto error;
	if (chmod(name, 0666) < 0)
		goto error;

	if (listen(fd, 5) < 0)
		goto error;

	return (fd);

error:
	close(fd);
	return (-1);
}

int serv_accept(int fd) {
	int ret;
	socklen_t len;
	struct sockaddr_un unix_addr;

	len = sizeof(unix_addr);

	if ((ret = accept(fd, (struct sockaddr *)&unix_addr, &len)) < 0)
		return (-1);

	return ret;
}

int sock_write(int fd, const void *buff, int count) {
	char *pts = (char *)buff;
	int status = 0, n;

	if (count < 0)
		return SOCKERR_OK;

	while (status != count) {
		n = write(fd, (void *)&pts[status], count - status);

		if (n < 0) {
			if (errno == EPIPE)
				return SOCKERR_CLOSED;
			else if (errno == EINTR)
				continue;
			else
				return SOCKERR_IO;
		}
		status += n;
	}

	return status;
}

int sock_read(int fd, void *buff, int count) {
	char *pts = (char *)buff;
	int status = 0, n;

	if (count <= 0)
		return SOCKERR_OK;
	while (status != count) {
		n = read(fd, (void *)&pts[status], count - status);

		if (n < 0) {
			if (errno == EINTR)
				continue;
			else
				return SOCKERR_IO;
		}

		if (n == 0)
			return SOCKERR_CLOSED;

		status += n;
	}

	return status;
}
