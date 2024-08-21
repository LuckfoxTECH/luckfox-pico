// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2016, Linaro Limited
 */

#include <assert.h>
#include <err.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ta_socket.h>
#include <tee_isocket.h>
#include <tee_tcpsocket.h>
#include <__tee_tcpsocket_defines_extensions.h>
#include <tee_udpsocket.h>
#include <unistd.h>

#include "xtest_test.h"
#include "xtest_helpers.h"
#include "sock_server.h"
#include "rand_stream.h"

struct socket_handle {
	uint64_t buf[2];
	size_t blen;
};

static TEE_Result socket_tcp_open(TEEC_Session *session, uint32_t ip_vers,
				  const char *addr, uint16_t port,
				  struct socket_handle *handle,
				  uint32_t *error, uint32_t *ret_orig)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;

	memset(handle, 0, sizeof(*handle));

	op.params[0].value.a = ip_vers;
	op.params[0].value.b = port;
	op.params[1].tmpref.buffer = (void *)addr;
	op.params[1].tmpref.size = strlen(addr) + 1;
	op.params[2].tmpref.buffer = handle->buf;
	op.params[2].tmpref.size = sizeof(handle->buf);

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_TEMP_INPUT,
					 TEEC_MEMREF_TEMP_OUTPUT,
					 TEEC_VALUE_OUTPUT);

	res = TEEC_InvokeCommand(session, TA_SOCKET_CMD_TCP_OPEN,
				 &op, ret_orig);

	handle->blen = op.params[2].tmpref.size;
	*error = op.params[3].value.a;
	return res;
}

static TEE_Result socket_udp_open(TEEC_Session *session, uint32_t ip_vers,
				  const char *addr, uint16_t port,
				  struct socket_handle *handle,
				  uint32_t *error, uint32_t *ret_orig)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;

	memset(handle, 0, sizeof(*handle));

	op.params[0].value.a = ip_vers;
	op.params[0].value.b = port;
	op.params[1].tmpref.buffer = (void *)addr;
	op.params[1].tmpref.size = strlen(addr) + 1;
	op.params[2].tmpref.buffer = handle->buf;
	op.params[2].tmpref.size = sizeof(handle->buf);

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_TEMP_INPUT,
					 TEEC_MEMREF_TEMP_OUTPUT,
					 TEEC_VALUE_OUTPUT);

	res = TEEC_InvokeCommand(session, TA_SOCKET_CMD_UDP_OPEN,
				 &op, ret_orig);

	handle->blen = op.params[2].tmpref.size;
	*error = op.params[3].value.a;
	return res;
}

static TEE_Result socket_send(TEEC_Session *session,
			      struct socket_handle *handle,
			      const void *data, size_t *dlen,
			      uint32_t timeout, uint32_t *ret_orig)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;

	op.params[0].tmpref.buffer = handle->buf;
	op.params[0].tmpref.size = handle->blen;
	op.params[1].tmpref.buffer = (void *)data;
	op.params[1].tmpref.size = *dlen;
	op.params[2].value.a = timeout;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
					 TEEC_MEMREF_TEMP_INPUT,
					 TEEC_VALUE_INOUT, TEEC_NONE);

	res = TEEC_InvokeCommand(session, TA_SOCKET_CMD_SEND, &op, ret_orig);

	*dlen = op.params[2].value.b;
	return res;
}

static TEE_Result socket_recv(TEEC_Session *session,
			      struct socket_handle *handle,
			      void *data, size_t *dlen,
			      uint32_t timeout, uint32_t *ret_orig)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;

	op.params[0].tmpref.buffer = handle->buf;
	op.params[0].tmpref.size = handle->blen;
	op.params[1].tmpref.buffer = (void *)data;
	op.params[1].tmpref.size = *dlen;
	op.params[2].value.a = timeout;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
					 TEEC_MEMREF_TEMP_OUTPUT,
					 TEEC_VALUE_INPUT, TEEC_NONE);

	res = TEEC_InvokeCommand(session, TA_SOCKET_CMD_RECV, &op, ret_orig);

	*dlen = op.params[1].tmpref.size;
	return res;
}

static TEE_Result socket_get_error(TEEC_Session *session,
			      struct socket_handle *handle,
			      uint32_t *proto_error, uint32_t *ret_orig)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;

	op.params[0].tmpref.buffer = handle->buf;
	op.params[0].tmpref.size = handle->blen;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
					 TEEC_VALUE_OUTPUT,
					 TEEC_NONE, TEEC_NONE);

	res = TEEC_InvokeCommand(session, TA_SOCKET_CMD_ERROR, &op, ret_orig);

	*proto_error = op.params[1].value.a;
	return res;
}

static TEE_Result socket_close(TEEC_Session *session,
			      struct socket_handle *handle, uint32_t *ret_orig)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;

	op.params[0].tmpref.buffer = handle->buf;
	op.params[0].tmpref.size = handle->blen;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
					 TEEC_NONE, TEEC_NONE, TEEC_NONE);

	return TEEC_InvokeCommand(session, TA_SOCKET_CMD_CLOSE, &op, ret_orig);
}

static TEE_Result socket_ioctl(TEEC_Session *session,
			      struct socket_handle *handle, uint32_t ioctl_cmd,
			      void *data, size_t *dlen, uint32_t *ret_orig)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;

	op.params[0].tmpref.buffer = handle->buf;
	op.params[0].tmpref.size = handle->blen;
	op.params[1].tmpref.buffer = data;
	op.params[1].tmpref.size = *dlen;
	op.params[2].value.a = ioctl_cmd;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
					 TEEC_MEMREF_TEMP_INOUT,
					 TEEC_VALUE_INPUT, TEEC_NONE);

	res = TEEC_InvokeCommand(session, TA_SOCKET_CMD_IOCTL, &op, ret_orig);

	*dlen = op.params[1].tmpref.size;
	return res;
}



struct test_200x_io_state {
	struct rand_stream *read_rs;
	struct rand_stream *write_rs;
	bool rfail;
};

static void test_200x_init_io_state(struct test_200x_io_state *s,
				    int read_seed, int write_seed)
{
	memset(s, 0, sizeof(*s));
	s->read_rs = rand_stream_alloc(read_seed, 100);
	s->write_rs = rand_stream_alloc(write_seed, 100);
	assert(s->read_rs && s->write_rs);
}

static bool test_200x_tcp_accept_cb(void *ptr, int fd, short *events)
{
	(void)ptr;
	(void)fd;
	(void)events;
	return true;
}

static bool test_200x_tcp_read_cb(void *ptr, int fd, short *events)
{
	struct test_200x_io_state *iostate = ptr;
	ssize_t r = 0;
	uint8_t buf[100] = { };
	uint8_t buf2[100] = { };

	(void)events;
	r = read(fd, buf, sizeof(buf));
	if (r <= 0)
		return false;

	rand_stream_read(iostate->read_rs, buf2, r);
	if (memcmp(buf, buf2, r)) {
		iostate->rfail = true;
		return false;
	}

	return true;
}

static bool test_200x_tcp_write_cb(void *ptr, int fd, short *events)
{
	struct test_200x_io_state *iostate = ptr;
	size_t num_bytes = 100;
	const void *bytes = NULL;
	ssize_t r = 0;

	(void)events;

	bytes = rand_stream_peek(iostate->write_rs, &num_bytes);
	r = write(fd, bytes, num_bytes);
	if (r < 0)
		return false;

	rand_stream_advance(iostate->write_rs, num_bytes);
	return true;
}

static void xtest_tee_test_2001(ADBG_Case_t *c)
{
	struct sock_server ts = { };
	TEEC_Session session = { };
	uint32_t ret_orig = 0;
	uint32_t proto_error = 9;
	struct socket_handle sh = { };
	uint8_t buf[64] = { };
	uint8_t buf2[64] = { };
	size_t blen = 0;
	struct test_200x_io_state server_iostate = { };
	struct test_200x_io_state local_iostate = { };
	struct sock_io_cb cb = {
		.accept = test_200x_tcp_accept_cb,
		.read = test_200x_tcp_read_cb,
		.write = test_200x_tcp_write_cb,
		.ptr = &server_iostate,
	};

	test_200x_init_io_state(&server_iostate, 1, 2);
	test_200x_init_io_state(&local_iostate, 2, 1);

	Do_ADBG_BeginSubCase(c, "Start server");
	if (!ADBG_EXPECT_TRUE(c, sock_server_init_tcp(&ts, &cb)))
		return;
	Do_ADBG_EndSubCase(c, "Start server");

	Do_ADBG_BeginSubCase(c, "TCP Socket open");
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, xtest_teec_open_session(
			&session, &socket_ta_uuid, NULL, &ret_orig)))
		goto out;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_tcp_open(&session,
			TEE_IP_VERSION_DC, ts.bind->host, ts.bind->port,
			&sh, &proto_error, &ret_orig)))
		goto out_close_session;

	Do_ADBG_EndSubCase(c, "TCP Socket open");

	Do_ADBG_BeginSubCase(c, "TCP Socket send");
	blen = sizeof(buf);
	rand_stream_read(local_iostate.write_rs, buf, blen);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_send(&session, &sh,
			buf, &blen, TEE_TIMEOUT_INFINITE, &ret_orig)))
		goto out_close_session;
	ADBG_EXPECT_COMPARE_UNSIGNED(c, blen, ==, sizeof(buf));
	Do_ADBG_EndSubCase(c, "TCP Socket send");

	Do_ADBG_BeginSubCase(c, "TCP Socket recv");
	blen = sizeof(buf);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_recv(&session, &sh,
			buf, &blen, TEE_TIMEOUT_INFINITE, &ret_orig)))
		goto out_close_session;
	ADBG_EXPECT_COMPARE_UNSIGNED(c, blen, ==, sizeof(buf));
	rand_stream_read(local_iostate.read_rs, buf2, blen);
	ADBG_EXPECT_BUFFER(c, buf2, blen, buf, blen);
	Do_ADBG_EndSubCase(c, "TCP Socket recv");

	/*
	 * All written bytes above (with the TA) is quite likely to have
	 * hit the tcp server by now.
	 */
	ADBG_EXPECT_TRUE(c, !server_iostate.rfail);

	Do_ADBG_BeginSubCase(c, "TCP Socket get error");
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_get_error(&session, &sh,
			&proto_error, &ret_orig)))
		goto out_close_session;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, proto_error))
		goto out_close_session;
	Do_ADBG_EndSubCase(c, "TCP Socket get error");

	Do_ADBG_BeginSubCase(c, "TCP Socket close");
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_close(&session, &sh,
			&ret_orig)))
		goto out_close_session;
	Do_ADBG_EndSubCase(c, "TCP Socket close");

	Do_ADBG_BeginSubCase(c, "TCP Socket ioctl");


	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_tcp_open(&session,
			TEE_IP_VERSION_DC, ts.bind->host, ts.bind->port,
			&sh, &proto_error, &ret_orig)))
		goto out_close_session;

	blen = sizeof(buf);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_ioctl(&session, &sh, 0x00F00000,
			buf, &blen, &ret_orig)))
		goto out_close_session;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_close(&session, &sh,
			&ret_orig)))
		goto out_close_session;


	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_tcp_open(&session,
			TEE_IP_VERSION_DC, ts.bind->host, ts.bind->port,
			&sh, &proto_error, &ret_orig)))
		goto out_close_session;

	blen = sizeof(buf);
	ADBG_EXPECT_TEEC_RESULT(c, TEEC_ERROR_TARGET_DEAD,
				socket_ioctl(&session, &sh,
					TEE_ISOCKET_PROTOCOLID_TCP << 24,
					buf, &blen, &ret_orig));
	TEEC_CloseSession(&session);

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, xtest_teec_open_session(
			&session, &socket_ta_uuid, NULL, &ret_orig)))
		goto out;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_tcp_open(&session,
			TEE_IP_VERSION_DC, ts.bind->host, ts.bind->port,
			&sh, &proto_error, &ret_orig)))
		goto out_close_session;

	blen = sizeof(buf);
	ADBG_EXPECT_TEEC_RESULT(c, TEEC_ERROR_TARGET_DEAD,
				socket_ioctl(&session, &sh, 0x32 << 24,
				buf, &blen, &ret_orig));

	Do_ADBG_EndSubCase(c, "TCP Socket ioctl");

out_close_session:
	TEEC_CloseSession(&session);
out:
	sock_server_uninit(&ts);
}
ADBG_CASE_DEFINE(regression, 2001, xtest_tee_test_2001,
		"Trivial TCP iSocket API tests");

struct test_2002_barrier {
	pthread_mutex_t mu;
	pthread_barrier_t bar;
};

struct test_2002_arg {
	bool success;
	size_t tnum;
	struct test_2002_barrier *bar;
};

static void  xtest_2002_wait_barrier(struct test_2002_barrier *bar)
{
	/*
	 * Once the mutex is taken the barrier is initialized so the mutex
	 * can be released immediately.
	 */
	xtest_mutex_lock(&bar->mu);
	xtest_mutex_unlock(&bar->mu);
	xtest_barrier_wait(&bar->bar);
}

static void *xtest_tee_test_2002_thread(void *arg)
{
	struct test_2002_arg *a = arg;
	TEE_Result res = TEE_ERROR_GENERIC;
	struct sock_server ts = { };
	TEEC_Session session = { };
	uint32_t ret_orig = 0;
	uint32_t proto_error = 0;
	struct socket_handle sh = { };
	struct test_200x_io_state server_iostate = { };
	struct test_200x_io_state local_iostate = { };
	struct sock_io_cb cb = {
		.accept = test_200x_tcp_accept_cb,
		.read = test_200x_tcp_read_cb,
		.write = test_200x_tcp_write_cb,
		.ptr = &server_iostate,
	};
	int seed[2] = { 1 + a->tnum * 2, 2 + a->tnum * 2 };
	size_t send_limit = 10000;
	size_t recv_limit = 10000;
	size_t sent_bytes = 0;
	size_t recvd_bytes = 0;

	test_200x_init_io_state(&server_iostate, seed[0], seed[1]);
	test_200x_init_io_state(&local_iostate, seed[1], seed[0]);

	if (!sock_server_init_tcp(&ts, &cb)) {
		xtest_2002_wait_barrier(a->bar);
		return NULL;
	}

	res = xtest_teec_open_session(&session, &socket_ta_uuid, NULL,
				      &ret_orig);

	xtest_2002_wait_barrier(a->bar);
	if (res != TEE_SUCCESS)
		goto out;

	res = socket_tcp_open(&session, TEE_IP_VERSION_DC, ts.bind->host,
			      ts.bind->port, &sh, &proto_error, &ret_orig);
	if (res != TEE_SUCCESS)
		goto out_close_session;

	while (sent_bytes < send_limit && recvd_bytes < recv_limit) {
		const void *peek = NULL;
		uint8_t buf[64] = { };
		uint8_t buf2[64] = { };
		size_t blen = 0;

		blen = sizeof(buf);
		peek = rand_stream_peek(local_iostate.write_rs, &blen);
		res = socket_send(&session, &sh, peek, &blen,
				  TEE_TIMEOUT_INFINITE, &ret_orig);
		if (res != TEE_SUCCESS)
			goto out_close_session;
		rand_stream_advance(local_iostate.write_rs, blen);
		sent_bytes += blen;

		blen = sizeof(buf);
		res = socket_recv(&session, &sh, buf, &blen,
				  TEE_TIMEOUT_INFINITE, &ret_orig);
		if (res != TEE_SUCCESS)
			goto out_close_session;
		rand_stream_read(local_iostate.read_rs, buf2, blen);
		if (memcmp(buf2, buf, blen))
			goto out_close_session;
		recvd_bytes += blen;
	}


	res = socket_close(&session, &sh, &ret_orig);
	if (res != TEE_SUCCESS)
		goto out_close_session;

	/*
	 * All written bytes above (with the TA) is quite likely to have
	 * hit the tcp server by now.
	 */
	a->success = !server_iostate.rfail;

out_close_session:
	TEEC_CloseSession(&session);
out:
	sock_server_uninit(&ts);
	return NULL;
}

#define NUM_THREADS	3

static void xtest_tee_test_2002(ADBG_Case_t *c)
{
	struct test_2002_barrier bar = { .mu = PTHREAD_MUTEX_INITIALIZER };
	struct test_2002_arg arg[NUM_THREADS] = { };
	size_t n = 0;
	size_t nt = 0;
	pthread_t thr[NUM_THREADS] = { };

	Do_ADBG_BeginSubCase(c, "Stressing with %d threads", NUM_THREADS);

	xtest_mutex_lock(&bar.mu);

	nt = NUM_THREADS;
	for (n = 0; n < nt; n++) {
		arg[n].success = false;
		arg[n].tnum = n;
		arg[n].bar = &bar;
		if (!ADBG_EXPECT(c, 0, pthread_create(thr + n, NULL,
					xtest_tee_test_2002_thread, arg + n)))
			nt = n; /* break loop and start cleanup */
	}

	xtest_barrier_init(&bar.bar, nt + 1);
	xtest_mutex_unlock(&bar.mu);
	xtest_barrier_wait(&bar.bar);

	for (n = 0; n < nt; n++) {
		ADBG_EXPECT(c, 0, pthread_join(thr[n], NULL));
		ADBG_EXPECT_TRUE(c, arg[n].success);
	}

	xtest_mutex_destroy(&bar.mu);
	xtest_barrier_destroy(&bar.bar);

	Do_ADBG_EndSubCase(c, "Stressing with %d threads", NUM_THREADS);
}
ADBG_CASE_DEFINE(regression, 2002, xtest_tee_test_2002,
		"Concurrent stressing TCP iSocket API tests");

static bool test_2003_accept_cb(void *ptr, int fd, short *events)
{
	int val = 0;

	(void)ptr;
	(void)events;

	val = 4 * 1024;
	if (setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &val, sizeof(val)))
		warn("test_2003_accept_cb: setsockopt");
	return true;
}

static void xtest_tee_test_2003(ADBG_Case_t *c)
{
	struct sock_server ts = { };
	TEEC_Session session = { };
	uint32_t ret_orig = 0;
	uint32_t proto_error = 0;
	struct socket_handle sh = { };
	void *buf = NULL;
	const size_t blen0 = 16 * 1024;
	size_t blen = 0;
	uint32_t val = 0;
	struct sock_io_cb cb = { .accept = test_2003_accept_cb };

	Do_ADBG_BeginSubCase(c, "Start server");
	if (!ADBG_EXPECT_TRUE(c, sock_server_init_tcp(&ts, &cb)))
		return;
	buf = calloc(1, blen0);
	if (!ADBG_EXPECT_NOT_NULL(c, buf))
		goto out;
	Do_ADBG_EndSubCase(c, "Start server");

	Do_ADBG_BeginSubCase(c, "TCP Socket open");
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, xtest_teec_open_session(
			&session, &socket_ta_uuid, NULL, &ret_orig)))
		goto out;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_tcp_open(&session,
			TEE_IP_VERSION_DC, ts.bind->host, ts.bind->port,
			&sh, &proto_error, &ret_orig)))
		goto out_close_session;

	blen = sizeof(val);
	val = 4 * 1024;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_ioctl(&session, &sh,
			TEE_TCP_SET_RECVBUF, &val, &blen, &ret_orig)))
		goto out_close_session;

	blen = sizeof(val);
	val = 4 * 1024;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_ioctl(&session, &sh,
			TEE_TCP_SET_SENDBUF, &val, &blen, &ret_orig)))
		goto out_close_session;

	Do_ADBG_EndSubCase(c, "TCP Socket open");

	Do_ADBG_BeginSubCase(c, "TCP Socket send (10 ms timeout)");
	while (true) {
		TEE_Result res = TEE_ERROR_GENERIC;

		blen = blen0;
		memset(buf, 0, blen0);
		res = socket_send(&session, &sh, buf, &blen, 10, &ret_orig);
		if (res == TEE_ISOCKET_ERROR_TIMEOUT)
			break;
		if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
			goto out_close_session;
		ADBG_EXPECT_COMPARE_UNSIGNED(c, blen, ==, blen0);
	}
	Do_ADBG_EndSubCase(c, "TCP Socket send (10 ms timeout)");

	Do_ADBG_BeginSubCase(c, "TCP Socket recv (10 ms timeout)");
	blen = blen0;
	ADBG_EXPECT_TEEC_RESULT(c, TEE_ISOCKET_ERROR_TIMEOUT,
				socket_recv(&session, &sh, buf, &blen,
					    10, &ret_orig));
	ADBG_EXPECT_COMPARE_UNSIGNED(c, blen, ==, blen0);
	Do_ADBG_EndSubCase(c, "TCP Socket recv (10 ms timeout)");

	Do_ADBG_BeginSubCase(c, "TCP Socket get error");
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_get_error(&session, &sh,
			&proto_error, &ret_orig)))
		goto out_close_session;
	ADBG_EXPECT_TEEC_RESULT(c, TEE_ISOCKET_ERROR_TIMEOUT, proto_error);
	Do_ADBG_EndSubCase(c, "TCP Socket get error");

out_close_session:
	TEEC_CloseSession(&session);
out:
	free(buf);
	sock_server_uninit(&ts);
}
ADBG_CASE_DEFINE(regression, 2003, xtest_tee_test_2003,
		"Timeout TCP iSocket API tests");

static bool test_200x_udp_accept_cb(void *ptr, int fd, short *events)
{
	struct test_200x_io_state *iostate = ptr;
	struct sockaddr_storage sass = { };
	struct sockaddr *sa = (struct sockaddr *)&sass;
	socklen_t slen = sizeof(sass);
	uint8_t buf[100] = { };
	uint8_t buf2[100] = { };
	ssize_t r = 0;
	size_t l = 0;

	(void)events;

	r = recvfrom(fd, buf, sizeof(buf), 0, sa, &slen);
	if (r == -1)
		return false;

	l = r;
	rand_stream_read(iostate->read_rs, buf2, l);
	if (memcmp(buf, buf2, l))
		iostate->rfail = true;

	rand_stream_read(iostate->write_rs, buf, l);
	return sendto(fd, buf, l, 0, sa, slen) != -1;
}

static void xtest_tee_test_2004(ADBG_Case_t *c)
{
	struct sock_server ts = { };
	struct sock_server ts2 = { };
	struct sock_server ts3 = { };
	bool ts_inited = false;
	bool ts2_inited = false;
	bool ts3_inited = false;
	TEEC_Session session = { };
	uint32_t ret_orig = 0;
	uint32_t proto_error = 0;
	struct socket_handle sh = { };
	uint8_t buf[64] = { };
	uint8_t buf2[64] = { };
	size_t blen = 0;
	uint16_t port = 0;
	struct test_200x_io_state server_iostate = { };
	struct test_200x_io_state local_iostate = { };
	struct sock_io_cb cb = {
		.accept = test_200x_udp_accept_cb,
		.ptr = &server_iostate,
	};

	test_200x_init_io_state(&server_iostate, 1, 2);
	test_200x_init_io_state(&local_iostate, 2, 1);

	Do_ADBG_BeginSubCase(c, "Start server");
	if (!ADBG_EXPECT_TRUE(c, sock_server_init_udp(&ts, &cb)))
		return;
	ts_inited = true;
	if (!ADBG_EXPECT_TRUE(c, sock_server_init_udp(&ts2, &cb)))
		goto out;
	ts2_inited = true;
	if (!ADBG_EXPECT_TRUE(c, sock_server_init_udp(&ts3, &cb)))
		goto out;
	ts3_inited = true;
	Do_ADBG_EndSubCase(c, "Start server");

	Do_ADBG_BeginSubCase(c, "UDP Socket open");
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, xtest_teec_open_session(
			&session, &socket_ta_uuid, NULL, &ret_orig)))
		goto out;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_udp_open(&session,
			TEE_IP_VERSION_DC, ts.bind->host, ts.bind->port,
			&sh, &proto_error, &ret_orig)))
		goto out_close_session;

	Do_ADBG_EndSubCase(c, "UDP Socket open");

	Do_ADBG_BeginSubCase(c, "UDP Socket send");
	blen = sizeof(buf);
	rand_stream_read(local_iostate.write_rs, buf, blen);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_send(&session, &sh,
			buf, &blen, TEE_TIMEOUT_INFINITE, &ret_orig)))
		goto out_close_session;
	ADBG_EXPECT_COMPARE_UNSIGNED(c, blen, ==, sizeof(buf));
	Do_ADBG_EndSubCase(c, "UDP Socket send");

	Do_ADBG_BeginSubCase(c, "UDP Socket recv");
	blen = sizeof(buf);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_recv(&session, &sh,
			buf, &blen, TEE_TIMEOUT_INFINITE, &ret_orig)))
		goto out_close_session;
	ADBG_EXPECT_COMPARE_UNSIGNED(c, blen, ==, sizeof(buf));
	rand_stream_read(local_iostate.read_rs, buf2, blen);
	ADBG_EXPECT_BUFFER(c, buf2, blen, buf, blen);
	ADBG_EXPECT_TRUE(c, !server_iostate.rfail);
	Do_ADBG_EndSubCase(c, "UDP Socket recv");

	Do_ADBG_BeginSubCase(c, "UDP Socket get error");
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_get_error(&session, &sh,
			&proto_error, &ret_orig)))
		goto out_close_session;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, proto_error))
		goto out_close_session;
	Do_ADBG_EndSubCase(c, "UDP Socket get error");

	Do_ADBG_BeginSubCase(c, "UDP Socket close");
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_close(&session, &sh,
			&ret_orig)))
		goto out_close_session;
	Do_ADBG_EndSubCase(c, "UDP Socket close");

	Do_ADBG_BeginSubCase(c, "UDP Socket ioctl");

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_udp_open(&session,
			TEE_IP_VERSION_DC, ts.bind->host, ts.bind->port,
			&sh, &proto_error, &ret_orig)))
		goto out_close_session;

	blen = sizeof(buf);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_ioctl(&session, &sh, 0x00F00000,
			buf, &blen, &ret_orig)))
		goto out_close_session;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_close(&session, &sh,
			&ret_orig)))
		goto out_close_session;


	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_udp_open(&session,
			TEE_IP_VERSION_DC, ts.bind->host, ts.bind->port,
			&sh, &proto_error, &ret_orig)))
		goto out_close_session;

	blen = sizeof(buf);
	ADBG_EXPECT_TEEC_RESULT(c, TEEC_ERROR_TARGET_DEAD,
				socket_ioctl(&session, &sh,
					TEE_ISOCKET_PROTOCOLID_UDP << 24,
					buf, &blen, &ret_orig));
	TEEC_CloseSession(&session);

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, xtest_teec_open_session(
			&session, &socket_ta_uuid, NULL, &ret_orig)))
		goto out;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_udp_open(&session,
			TEE_IP_VERSION_DC, ts.bind->host, ts.bind->port,
			&sh, &proto_error, &ret_orig)))
		goto out_close_session;

	blen = sizeof(buf);
	ADBG_EXPECT_TEEC_RESULT(c, TEEC_ERROR_TARGET_DEAD,
				socket_ioctl(&session, &sh, 0x32 << 24,
				buf, &blen, &ret_orig));

	Do_ADBG_EndSubCase(c, "UDP Socket ioctl");

	Do_ADBG_BeginSubCase(c, "UDP Socket change port");

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, xtest_teec_open_session(
			&session, &socket_ta_uuid, NULL, &ret_orig)))
		goto out;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_udp_open(&session,
			TEE_IP_VERSION_DC, ts.bind->host, ts.bind->port,
			&sh, &proto_error, &ret_orig)))
		goto out_close_session;
	sock_server_uninit(&ts);
	ts_inited = false;

	port = ts2.bind->port;
	blen = sizeof(port);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_ioctl(&session, &sh,
			TEE_UDP_CHANGEPORT, &port, &blen, &ret_orig)))
		goto out_close_session;

	Do_ADBG_BeginSubCase(c, "UDP Socket send");
	blen = sizeof(buf);
	rand_stream_read(local_iostate.write_rs, buf, blen);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_send(&session, &sh,
			buf, &blen, TEE_TIMEOUT_INFINITE, &ret_orig)))
		goto out_close_session;
	ADBG_EXPECT_COMPARE_UNSIGNED(c, blen, ==, sizeof(buf));
	Do_ADBG_EndSubCase(c, "UDP Socket send");

	Do_ADBG_BeginSubCase(c, "UDP Socket recv");
	blen = sizeof(buf);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_recv(&session, &sh,
			buf, &blen, TEE_TIMEOUT_INFINITE, &ret_orig)))
		goto out_close_session;
	ADBG_EXPECT_COMPARE_UNSIGNED(c, blen, ==, sizeof(buf));
	rand_stream_read(local_iostate.read_rs, buf2, blen);
	ADBG_EXPECT_BUFFER(c, buf2, blen, buf, blen);
	ADBG_EXPECT_TRUE(c, !server_iostate.rfail);
	Do_ADBG_EndSubCase(c, "UDP Socket recv");

	Do_ADBG_EndSubCase(c, "UDP Socket change port");

	Do_ADBG_BeginSubCase(c, "UDP Socket change addr");

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, xtest_teec_open_session(
			&session, &socket_ta_uuid, NULL, &ret_orig)))
		goto out;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_udp_open(&session,
			TEE_IP_VERSION_DC, ts2.bind->host, ts2.bind->port,
			&sh, &proto_error, &ret_orig)))
		goto out_close_session;
	sock_server_uninit(&ts2);
	ts2_inited = false;

	port = ts3.bind->port;
	blen = sizeof(port);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_ioctl(&session, &sh,
			TEE_UDP_CHANGEPORT, &port, &blen, &ret_orig)))
		goto out_close_session;

	blen = strlen(ts3.bind->host) + 1;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_ioctl(&session, &sh,
			TEE_UDP_CHANGEADDR, ts3.bind->host, &blen, &ret_orig)))
		goto out_close_session;

	Do_ADBG_BeginSubCase(c, "UDP Socket send");
	blen = sizeof(buf);
	rand_stream_read(local_iostate.write_rs, buf, blen);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_send(&session, &sh,
			buf, &blen, TEE_TIMEOUT_INFINITE, &ret_orig)))
		goto out_close_session;
	ADBG_EXPECT_COMPARE_UNSIGNED(c, blen, ==, sizeof(buf));
	Do_ADBG_EndSubCase(c, "UDP Socket send");

	Do_ADBG_BeginSubCase(c, "UDP Socket recv");
	blen = sizeof(buf);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, socket_recv(&session, &sh,
			buf, &blen, TEE_TIMEOUT_INFINITE, &ret_orig)))
		goto out_close_session;
	ADBG_EXPECT_COMPARE_UNSIGNED(c, blen, ==, sizeof(buf));
	rand_stream_read(local_iostate.read_rs, buf2, blen);
	ADBG_EXPECT_BUFFER(c, buf2, blen, buf, blen);
	ADBG_EXPECT_TRUE(c, !server_iostate.rfail);
	Do_ADBG_EndSubCase(c, "UDP Socket recv");

	Do_ADBG_EndSubCase(c, "UDP Socket change addr");


out_close_session:
	TEEC_CloseSession(&session);
out:
	if (ts_inited)
		sock_server_uninit(&ts);
	if (ts2_inited)
		sock_server_uninit(&ts2);
	if (ts3_inited)
		sock_server_uninit(&ts3);
}
ADBG_CASE_DEFINE(regression, 2004, xtest_tee_test_2004,
		"UDP iSocket API tests");
