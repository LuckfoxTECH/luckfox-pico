/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-02-26     tyx          first implementation
 */

#include <stdint.h>
#include <stdbool.h>

#ifndef __TFTP_XFER_H__
#define __TFTP_XFER_H__

#define TFTP_CMD_RRQ        (1) /*Read request (RRQ)*/
#define TFTP_CMD_WRQ        (2) /*Write request (WRQ) */
#define TFTP_CMD_DATA       (3) /*Data (DATA)*/
#define TFTP_CMD_ACK        (4) /*Acknowledgment (ACK)*/
#define TFTP_CMD_ERROR      (5) /*Error (ERROR)*/

#define TFTP_XFER_OCTET ("octet")
#define TFTP_XFER_ASCII ("ascii")

#define TFTP_XFER_TYPE_CLIENT (0x01)
#define TFTP_XFER_TYPE_SERVER (0x02)

#define XFER_DATA_SIZE_MAX (512)

union file_info 
{
    uint16_t code;
    uint16_t block;
    char filename[2];
};

struct tftp_packet
{
    uint16_t cmd;
    union file_info info;
    char data[XFER_DATA_SIZE_MAX];
};

struct tftp_xfer
{
    int sock;
    int type;
    int blksize;
    char *mode;
    void *_private;
};

struct tftp_xfer *tftp_xfer_create(const char *ip_addr, int port);
void tftp_xfer_destroy(struct tftp_xfer *xfer);
int tftp_send_request(struct tftp_xfer *xfer, uint16_t cmd, const char *remote_file);
struct tftp_xfer *tftp_recv_request(struct tftp_xfer *xfer, struct tftp_packet *packet);
void tftp_xfer_mode_set(struct tftp_xfer *xfer, const char *mode);
int tftp_xfer_blksize_set(struct tftp_xfer *xfer, int blksize);
int tftp_xfer_type_set(struct tftp_xfer *xfer, int type);
int tftp_read_data(struct tftp_xfer *xfer, struct tftp_packet *pack, int size);
void tftp_transfer_err(struct tftp_xfer *xfer, uint16_t err_no, const char *err_msg);
int tftp_wait_ack(struct tftp_xfer *xfer);
int tftp_write_data(struct tftp_xfer *xfer, struct tftp_packet *pack, int len);
int tftp_resp_ack(struct tftp_xfer *xfer);

#endif
