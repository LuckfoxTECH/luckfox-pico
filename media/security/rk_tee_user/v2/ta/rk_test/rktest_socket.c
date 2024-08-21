// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2021 Rockchip Electronics Co. Ltd.
 */
#include <stdlib.h>
#include <string.h>
#include <tee_isocket.h>
#include <tee_tcpsocket.h>
#include <tee_udpsocket.h>
#include <__tee_tcpsocket_defines_extensions.h>
#include <trace.h>
#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include <tee_api_defines.h>
#include "rktest_handle.h"

TEE_Result handle_socket(void)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	TEE_iSocketHandle ctx;
	TEE_tcpSocket_Setup setup = { };
	uint32_t protocolError;

	setup.ipVersion = TEE_IP_VERSION_DC;
	setup.server_port = 2345;
	setup.server_addr = (char *)"127.0.0.1";

	res = TEE_tcpSocket->open(&ctx, &setup, &protocolError);
	if (res != TEE_SUCCESS) {
		EMSG("socket open fail!");
		return res;
	}

	char recv_buf[64];
	uint32_t recv_len = sizeof(recv_buf);
	res = TEE_tcpSocket->recv(ctx, recv_buf, &recv_len, 0xFFFFFFFF);
	if (res != TEE_SUCCESS) {
		EMSG("socket recv fail!");
		return res;
	}
	IMSG("TA get %d bytes data from server: %s", recv_len, recv_buf);

	char send_buf[] = "I am data from TA!";
	uint32_t send_len = sizeof(send_buf);
	res = TEE_tcpSocket->send(ctx, send_buf, &send_len, 0xFFFFFFFF);
	if (res != TEE_SUCCESS) {
		EMSG("socket send fail!");
		return res;
	}

	res = TEE_tcpSocket->error(ctx);
	if (res != TEE_SUCCESS) {
		EMSG("socket protocol error!");
		return res;
	}

	res = TEE_tcpSocket->close(ctx);
	if (res != TEE_SUCCESS) {
		EMSG("socket close fail!");
		return res;
	}
	return TEE_SUCCESS;
}
