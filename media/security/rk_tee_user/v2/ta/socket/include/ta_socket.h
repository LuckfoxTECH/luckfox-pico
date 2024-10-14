/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2016, Linaro Limited
 * All rights reserved.
 */

#ifndef __TA_SOCKET_H
#define __TA_SOCKET_H

#define TA_SOCKET_UUID { 0x873bcd08, 0xc2c3, 0x11e6, { \
			 0xa9, 0x37, 0xd0, 0xbf, 0x9c, 0x45, 0xc6, 0x1c } }

/*
 * Opens a TCP socket and returns an opaque handle
 *
 * [in]     params[0].value.a	ipVersion
 * [in]     params[0].value.b	server port
 * [in]     params[1].memref	server address
 * [out]    params[2].memref	handle
 * [out]    params[3].value.a	protocol error
 */
#define TA_SOCKET_CMD_TCP_OPEN	0

/*
 * Opens a UDP socket and returns an opaque handle
 *
 * [in]     params[0].value.a	ipVersion
 * [in]     params[0].value.b	server port
 * [in]     params[1].memref	server address
 * [out]    params[2].memref	handle
 * [out]    params[3].value.a	protocol error
 */
#define TA_SOCKET_CMD_UDP_OPEN	1

/*
 * Closes a socket
 *
 * [in]     params[0].memref	handle
 */
#define TA_SOCKET_CMD_CLOSE	2

/*
 * Send data on socket
 *
 * [in]     params[0].memref	handle
 * [in]     params[1].memref	data
 * [in]     params[2].value.a	timeout
 * [out]    params[2].value.b	sent bytes
 */
#define TA_SOCKET_CMD_SEND	3

/*
 * Receive data on socket
 *
 * [in]     params[0].memref	handle
 * [out]    params[1].memref	data
 * [in]     params[2].value.a	timeout
 */
#define TA_SOCKET_CMD_RECV	4

/*
 * Retrieve protocol error from socket
 *
 * [in]     params[0].memref	handle
 * [out]    params[1].value.a	error code
 */
#define TA_SOCKET_CMD_ERROR	5

/*
 * Ioctl on socket
 *
 * [in]     params[0].memref	handle
 * [in/out] params[1].memref	data
 * [in]     params[2].value.a	command code
 */
#define TA_SOCKET_CMD_IOCTL	6

#endif /*__TA_SOCKET_H*/
