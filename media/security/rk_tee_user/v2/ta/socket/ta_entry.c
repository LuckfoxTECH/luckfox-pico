// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2016, Linaro Limited
 * All rights reserved.
 */

#include <stdlib.h>
#include <string.h>
#include <ta_socket.h>
#include <tee_internal_api.h>
#include <tee_isocket.h>
#include <tee_tcpsocket.h>
#include <tee_udpsocket.h>
#include <trace.h>

TEE_Result TA_CreateEntryPoint(void)
{
	return TEE_SUCCESS;
}

void TA_DestroyEntryPoint(void)
{
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types,
				    TEE_Param params[4],
				    void **session_ctx)
{
	(void)param_types;
	(void)params;
	(void)session_ctx;
	return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void *session_ctx)
{
	(void)session_ctx;
}

struct sock_handle {
	TEE_iSocketHandle ctx;
	TEE_iSocket *socket;
};

static TEE_Result ta_entry_tcp_open(uint32_t param_types, TEE_Param params[4])
{
	TEE_Result res = TEE_ERROR_GENERIC;
	struct sock_handle h = { };
	TEE_tcpSocket_Setup setup = { };
	uint32_t req_param_types =
		TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
				TEE_PARAM_TYPE_MEMREF_INPUT,
				TEE_PARAM_TYPE_MEMREF_OUTPUT,
				TEE_PARAM_TYPE_VALUE_OUTPUT);

	if (param_types != req_param_types) {
		EMSG("got param_types 0x%x, expected 0x%x",
			param_types, req_param_types);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	if (params[2].memref.size < sizeof(struct sock_handle)) {
		params[2].memref.size = sizeof(struct sock_handle);
		return TEE_ERROR_SHORT_BUFFER;
	}

	setup.ipVersion = params[0].value.a;
	setup.server_port = params[0].value.b;
	setup.server_addr = strndup(params[1].memref.buffer,
				    params[1].memref.size);
	if (!setup.server_addr)
		return TEE_ERROR_OUT_OF_MEMORY;

	h.socket = TEE_tcpSocket;
	res = h.socket->open(&h.ctx, &setup, &params[3].value.a);
	free(setup.server_addr);
	if (res == TEE_SUCCESS) {
		memcpy(params[2].memref.buffer, &h, sizeof(h));
		params[2].memref.size = sizeof(h);
	}
	return res;
}

static TEE_Result ta_entry_udp_open(uint32_t param_types, TEE_Param params[4])
{
	TEE_Result res = TEE_ERROR_GENERIC;
	struct sock_handle h = { };
	TEE_udpSocket_Setup setup = { };
	uint32_t req_param_types =
		TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
				TEE_PARAM_TYPE_MEMREF_INPUT,
				TEE_PARAM_TYPE_MEMREF_OUTPUT,
				TEE_PARAM_TYPE_VALUE_OUTPUT);

	if (param_types != req_param_types) {
		EMSG("got param_types 0x%x, expected 0x%x",
			param_types, req_param_types);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	if (params[2].memref.size < sizeof(struct sock_handle)) {
		params[2].memref.size = sizeof(struct sock_handle);
		return TEE_ERROR_SHORT_BUFFER;
	}

	setup.ipVersion = params[0].value.a;
	setup.server_port = params[0].value.b;
	setup.server_addr = strndup(params[1].memref.buffer,
				    params[1].memref.size);
	if (!setup.server_addr)
		return TEE_ERROR_OUT_OF_MEMORY;

	h.socket = TEE_udpSocket;
	res = h.socket->open(&h.ctx, &setup, &params[3].value.a);
	free(setup.server_addr);
	if (res == TEE_SUCCESS) {
		memcpy(params[2].memref.buffer, &h, sizeof(h));
		params[2].memref.size = sizeof(h);
	}
	return res;
}

static TEE_Result ta_entry_close(uint32_t param_types, TEE_Param params[4])
{
	struct sock_handle *h = NULL;
	uint32_t req_param_types =
		TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
				TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE,
				TEE_PARAM_TYPE_NONE);

	if (param_types != req_param_types) {
		EMSG("got param_types 0x%x, expected 0x%x",
			param_types, req_param_types);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	if (params[0].memref.size != sizeof(struct sock_handle))
		return TEE_ERROR_BAD_PARAMETERS;

	h = params[0].memref.buffer;
	return h->socket->close(h->ctx);
}

static TEE_Result ta_entry_send(uint32_t param_types, TEE_Param params[4])
{
	struct sock_handle *h = NULL;
	uint32_t req_param_types =
		TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
				TEE_PARAM_TYPE_MEMREF_INPUT,
				TEE_PARAM_TYPE_VALUE_INOUT,
				TEE_PARAM_TYPE_NONE);

	if (param_types != req_param_types) {
		EMSG("got param_types 0x%x, expected 0x%x",
			param_types, req_param_types);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	if (params[0].memref.size != sizeof(*h))
		return TEE_ERROR_BAD_PARAMETERS;

	h = params[0].memref.buffer;
	params[2].value.b = params[1].memref.size;
	return h->socket->send(h->ctx, params[1].memref.buffer,
			       &params[2].value.b, params[2].value.a);
}

static TEE_Result ta_entry_recv(uint32_t param_types, TEE_Param params[4])
{
	struct sock_handle *h = NULL;
	uint32_t req_param_types =
		TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
				TEE_PARAM_TYPE_MEMREF_OUTPUT,
				TEE_PARAM_TYPE_VALUE_INPUT,
				TEE_PARAM_TYPE_NONE);

	if (param_types != req_param_types) {
		EMSG("got param_types 0x%x, expected 0x%x",
			param_types, req_param_types);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	if (params[0].memref.size != sizeof(struct sock_handle))
		return TEE_ERROR_BAD_PARAMETERS;

	h = params[0].memref.buffer;
	return h->socket->recv(h->ctx, params[1].memref.buffer,
			       &params[1].memref.size, params[2].value.a);
}

static TEE_Result ta_entry_error(uint32_t param_types, TEE_Param params[4])
{
	struct sock_handle *h = NULL;
	uint32_t req_param_types =
		TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
				TEE_PARAM_TYPE_VALUE_OUTPUT,
				TEE_PARAM_TYPE_NONE,
				TEE_PARAM_TYPE_NONE);

	if (param_types != req_param_types) {
		EMSG("got param_types 0x%x, expected 0x%x",
			param_types, req_param_types);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	if (params[0].memref.size != sizeof(struct sock_handle))
		return TEE_ERROR_BAD_PARAMETERS;

	h = params[0].memref.buffer;
	params[1].value.a = h->socket->error(h->ctx);
	return TEE_SUCCESS;
}

static TEE_Result ta_entry_ioctl(uint32_t param_types, TEE_Param params[4])
{
	struct sock_handle *h = NULL;
	uint32_t req_param_types =
		TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
				TEE_PARAM_TYPE_MEMREF_INOUT,
				TEE_PARAM_TYPE_VALUE_INPUT,
				TEE_PARAM_TYPE_NONE);

	if (param_types != req_param_types) {
		EMSG("got param_types 0x%x, expected 0x%x",
			param_types, req_param_types);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	if (params[0].memref.size != sizeof(struct sock_handle))
		return TEE_ERROR_BAD_PARAMETERS;

	h = params[0].memref.buffer;
	return h->socket->ioctl(h->ctx, params[2].value.a,
				params[1].memref.buffer,
				&params[1].memref.size);
}



TEE_Result TA_InvokeCommandEntryPoint(void *session_ctx,
				      uint32_t cmd_id, uint32_t param_types,
				      TEE_Param params[4])
{
	(void)session_ctx;

	switch (cmd_id) {
	case TA_SOCKET_CMD_TCP_OPEN:
		return ta_entry_tcp_open(param_types, params);
	case TA_SOCKET_CMD_UDP_OPEN:
		return ta_entry_udp_open(param_types, params);
	case TA_SOCKET_CMD_CLOSE:
		return ta_entry_close(param_types, params);
	case TA_SOCKET_CMD_SEND:
		return ta_entry_send(param_types, params);
	case TA_SOCKET_CMD_RECV:
		return ta_entry_recv(param_types, params);
	case TA_SOCKET_CMD_ERROR:
		return ta_entry_error(param_types, params);
	case TA_SOCKET_CMD_IOCTL:
		return ta_entry_ioctl(param_types, params);
	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}
}
