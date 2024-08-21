/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2016, Linaro Limited
 */

#ifndef XTEST_SOCK_SERVER_H
#define XTEST_SOCK_SERVER_H

#include <pthread.h>
#include <stdbool.h>
#include <sys/types.h>

struct sock_server_bind {
	int fd;
	char host[255];
	int port;
};

struct sock_server {
	struct sock_server_bind *bind;
	size_t num_binds;
	int quit_fd;
	int stop_fd;
	pthread_t thr;
	pthread_mutex_t mu;
	bool error;
	struct sock_io_cb *cb;
};

struct sock_io_cb {
	bool (*accept)(void *ptr, int fd, short *events);
	bool (*read)(void *ptr, int fd, short *events);
	bool (*write)(void *ptr, int fd, short *events);
	void *ptr;
};

bool sock_server_init_tcp(struct sock_server *sock_serv, struct sock_io_cb *cb);
bool sock_server_init_udp(struct sock_server *sock_serv, struct sock_io_cb *cb);
void sock_server_uninit(struct sock_server *sock_serv);
void sock_server_lock(struct sock_server *sock_serv);
void sock_server_unlock(struct sock_server *sock_serv);

#endif /*XTEST_SOCK_SERVER_H*/
