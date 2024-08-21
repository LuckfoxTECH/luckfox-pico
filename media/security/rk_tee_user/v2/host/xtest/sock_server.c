// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2016, Linaro Limited
 */

#include <sys/types.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <err.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

#include "sock_server.h"

struct server_state {
	struct sock_state *socks;
	struct pollfd *fds;
	nfds_t nfds;
	bool got_quit;
	struct sock_io_cb *cb;
};

#define SOCK_BUF_SIZE	512

struct sock_state {
	bool (*cb)(struct server_state *srvst, size_t idx);
	struct sock_server_bind *serv;
};

static bool server_io_cb(struct server_state *srvst, size_t idx)
{
	short revents = srvst->fds[idx].revents;
	short *events = &srvst->fds[idx].events;
	struct sock_io_cb *cb = srvst->cb;
	int fd = 0;

	fd = srvst->fds[idx].fd;
	if (revents & POLLIN) {
		if (!cb->read)
			*events &= ~POLLIN;
		else if (!cb->read(cb->ptr, fd, events))
			goto close;
	}

	if (revents & POLLOUT) {
		if (!cb->write)
			*events &= ~POLLOUT;
		else if (!cb->write(cb->ptr, fd, events))
			goto close;
	}

	if (!(revents & ~(POLLIN | POLLOUT)))
		return true;
close:
	if (close(fd)) {
		warn("server_io_cb: close(%d)", fd);
		return false;
	}
	srvst->fds[idx].fd = -1;
	return true;
}

static bool server_add_state(struct server_state *srvst,
			     bool (*cb)(struct server_state *srvst, size_t idx),
			     struct sock_server_bind *serv, int fd,
			     short poll_events)
{
	void *p = NULL;
	size_t n = 0;

	for (n = 0; n < srvst->nfds; n++) {
		if (srvst->fds[n].fd == -1) {
			srvst->socks[n].cb = cb;
			srvst->socks[n].serv = serv;
			srvst->fds[n].fd = fd;
			srvst->fds[n].events = poll_events;
			srvst->fds[n].revents = 0;
			return true;
		}
	}

	p = realloc(srvst->socks, sizeof(*srvst->socks) * (srvst->nfds + 1));
	if (!p)
		return false;
	srvst->socks = p;
	srvst->socks[srvst->nfds].cb = cb;
	srvst->socks[srvst->nfds].serv = serv;

	p = realloc(srvst->fds, sizeof(*srvst->fds) * (srvst->nfds + 1));
	if (!p)
		return false;
	srvst->fds = p;
	srvst->fds[srvst->nfds].fd = fd;
	srvst->fds[srvst->nfds].events = poll_events;
	srvst->fds[srvst->nfds].revents = 0;

	srvst->nfds++;
	return true;
}

static bool tcp_server_accept_cb(struct server_state *srvst, size_t idx)
{
	short revents = srvst->fds[idx].revents;
	struct sockaddr_storage sass = { };
	struct sockaddr *sa = (struct sockaddr *)&sass;
	socklen_t len = sizeof(sass);
	int fd = 0;
	short io_events = POLLIN | POLLOUT;

	if (!(revents & POLLIN))
		return false;

	fd = accept(srvst->fds[idx].fd, sa, &len);
	if (fd == -1) {
		if (errno == EAGAIN || errno == EWOULDBLOCK ||
		    errno == ECONNABORTED)
			return true;
		return false;
	}

	if (srvst->cb->accept &&
	    !srvst->cb->accept(srvst->cb->ptr, fd, &io_events)) {
		if (close(fd))
			warn("server_accept_cb: close(%d)", fd);
		return true;
	}

	return server_add_state(srvst, server_io_cb, srvst->socks[idx].serv,
				fd, io_events);
}

static bool udp_server_cb(struct server_state *srvst, size_t idx)
{
	short revents = srvst->fds[idx].revents;

	if (!(revents & POLLIN))
		return false;

	return srvst->cb->accept(srvst->cb->ptr, srvst->fds[idx].fd, NULL);
}

static bool server_quit_cb(struct server_state *srvst, size_t idx)
{
	(void)idx;
	srvst->got_quit = true;
	return true;
}

static void sock_server(struct sock_server *ts,
			bool (*cb)(struct server_state *srvst, size_t idx))
{
	struct server_state srvst = { .cb = ts->cb };
	int pres = 0;
	size_t n = 0;
	char b = 0;

	sock_server_lock(ts);

	for (n = 0; n < ts->num_binds; n++) {
		if (!server_add_state(&srvst, cb, ts->bind + n,
				      ts->bind[n].fd, POLLIN))
			goto bad;
	}

	if (!server_add_state(&srvst, server_quit_cb, NULL,
			      ts->quit_fd, POLLIN))
		goto bad;

	while (true) {
		sock_server_unlock(ts);
		/*
		 * First sleep 5 ms to make it easier to test send timeouts
		 * due to this rate limit.
		 */
		poll(NULL, 0, 5);
		pres = poll(srvst.fds, srvst.nfds, -1);
		sock_server_lock(ts);
		if (pres < 0)
			goto bad;

		for (n = 0; pres && n < srvst.nfds; n++) {
			if (srvst.fds[n].revents) {
				pres--;
				if (!srvst.socks[n].cb(&srvst, n))
					goto bad;
			}
		}

		if (srvst.got_quit)
			goto out;
	}

bad:
	ts->error = true;
out:
	for (n = 0; n < srvst.nfds; n++) {
		/* Don't close accept and quit fds */
		if (srvst.fds[n].fd != -1 && srvst.socks[n].serv &&
		    srvst.fds[n].fd != srvst.socks[n].serv->fd) {
			if (close(srvst.fds[n].fd))
				warn("sock_server: close(%d)", srvst.fds[n].fd);
		}
	}
	free(srvst.socks);
	free(srvst.fds);
	if (read(ts->quit_fd, &b, 1) != 1)
		ts->error = true;

	sock_server_unlock(ts);
}

static void *sock_server_stream(void *arg)
{
	sock_server(arg, tcp_server_accept_cb);
	return NULL;
}

static void *sock_server_dgram(void *arg)
{
	sock_server(arg, udp_server_cb);
	return NULL;
}

static void sock_server_add_fd(struct sock_server *ts, struct addrinfo *ai)
{
	struct sock_server_bind serv = { };
	struct sockaddr_storage sass = { };
	struct sockaddr *sa = (struct sockaddr *)&sass;
	struct sockaddr_in *sain = (struct sockaddr_in *)&sass;
	struct sockaddr_in6 *sain6 = (struct sockaddr_in6 *)&sass;
	void *src = NULL;
	socklen_t len = sizeof(sass);
	struct sock_server_bind *p = NULL;

	serv.fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
	if (serv.fd < 0)
		return;

	if (bind(serv.fd, ai->ai_addr, ai->ai_addrlen))
		goto bad;

	if (ai->ai_socktype == SOCK_STREAM && listen(serv.fd, 5))
		goto bad;

	if (getsockname(serv.fd, sa, &len))
		goto bad;

	switch (sa->sa_family) {
	case AF_INET:
		src = &sain->sin_addr;
		serv.port = ntohs(sain->sin_port);
		break;
	case AF_INET6:
		src = &sain6->sin6_addr;
		serv.port = ntohs(sain6->sin6_port);
	default:
		goto bad;
	}

	if (!inet_ntop(sa->sa_family, src, serv.host, sizeof(serv.host)))
		goto bad;

	p = realloc(ts->bind, sizeof(*p) * (ts->num_binds + 1));
	if (!p)
		goto bad;

	ts->bind = p;
	p[ts->num_binds] = serv;
	ts->num_binds++;
	return;
bad:
	if (close(serv.fd))
		warn("sock_server_add_fd: close(%d)", serv.fd);
}

void sock_server_uninit(struct sock_server *ts)
{
	size_t n = 0;
	int e = 0;

	if (ts->stop_fd != -1) {
		if (close(ts->stop_fd))
			warn("sock_server_uninit: close(%d)", ts->stop_fd);
		ts->stop_fd = -1;
		e = pthread_join(ts->thr, NULL);
		if (e)
			warnx("sock_server_uninit: pthread_join: %s",
			      strerror(e));
	}

	e = pthread_mutex_destroy(&ts->mu);
	if (e)
		warnx("sock_server_uninit: pthread_mutex_destroy: %s",
		      strerror(e));

	for (n = 0; n < ts->num_binds; n++)
		if (close(ts->bind[n].fd))
			warn("sock_server_uninit: close(%d)", ts->bind[n].fd);
	free(ts->bind);
	if (ts->quit_fd != -1 && close(ts->quit_fd))
		warn("sock_server_uninit: close(%d)", ts->quit_fd);
	memset(ts, 0, sizeof(*ts));
	ts->quit_fd = -1;
	ts->stop_fd = -1;
}

static bool sock_server_init(struct sock_server *ts, struct sock_io_cb *cb,
			     int socktype)
{
	struct addrinfo hints = { };
	struct addrinfo *ai = NULL;
	struct addrinfo *ai0 = NULL;
	int fd_pair[2] = { };
	int e = 0;

	memset(ts, 0, sizeof(*ts));
	ts->quit_fd = -1;
	ts->stop_fd = -1;
	ts->cb = cb;

	e = pthread_mutex_init(&ts->mu, NULL);
	if (e) {
		warnx("sock_server_init: pthread_mutex_init: %s", strerror(e));
		return false;
	}

	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = socktype;

	if (getaddrinfo(NULL, "0", &hints, &ai0))
		return false;

	for (ai = ai0; ai; ai = ai->ai_next)
		sock_server_add_fd(ts, ai);

	freeaddrinfo(ai0);

	if (!ts->num_binds)
		return false;

	if (pipe(fd_pair)) {
		sock_server_uninit(ts);
		return false;
	}

	ts->quit_fd = fd_pair[0];

	if (socktype == SOCK_STREAM)
		e = pthread_create(&ts->thr, NULL, sock_server_stream, ts);
	else
		e = pthread_create(&ts->thr, NULL, sock_server_dgram, ts);
	if (e) {
		warnx("sock_server_init: pthread_create: %s", strerror(e));
		if (close(fd_pair[1]))
			warn("sock_server_init: close(%d)", fd_pair[1]);
		sock_server_uninit(ts);
		return false;
	}

	ts->stop_fd = fd_pair[1];
	return true;
}

bool sock_server_init_tcp(struct sock_server *sock_serv, struct sock_io_cb *cb)
{
	return sock_server_init(sock_serv, cb, SOCK_STREAM);
}

bool sock_server_init_udp(struct sock_server *sock_serv, struct sock_io_cb *cb)
{
	return sock_server_init(sock_serv, cb, SOCK_DGRAM);
}

void sock_server_lock(struct sock_server *ts)
{
	int e = pthread_mutex_lock(&ts->mu);

	if (e)
		errx(1, "sock_server_lock: pthread_mutex_lock: %s", strerror(e));
}

void sock_server_unlock(struct sock_server *ts)
{
	int e = pthread_mutex_unlock(&ts->mu);

	if (e)
		errx(1, "sock_server_unlock: pthread_mutex_unlock: %s",
		     strerror(e));
}
