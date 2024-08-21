/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-02-26     tyx          first implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/select.h>
#include "tftp_xfer.h"
#include "tftp.h"

struct tftp_xfer_private
{
    struct sockaddr_in server;
    struct sockaddr_in sender;
    char *ip_addr;
    uint16_t port;
    uint16_t block;
};

static int tftp_recv_raw_data(struct tftp_xfer *xfer, void *buff, int len)
{
    struct tftp_xfer_private *_private;
    int sender_len = sizeof(struct sockaddr_in);
    int r_size = 0;

    /* udp receive data */
    _private = xfer->_private;
    r_size = recvfrom(xfer->sock, buff, len, 0, (struct sockaddr *)&_private->sender, (socklen_t *)&sender_len);
    if (r_size < 0)
    {
        return -TFTP_EXFER;
    }
    return r_size;
}

void tftp_transfer_err(struct tftp_xfer *xfer, uint16_t err_no, const char *err_msg)
{
    uint16_t *snd_packet;
    struct tftp_xfer_private *_private;
    int str_len;

    /* Calculate error message length */
    str_len = strlen(err_msg);
    if (str_len > 512)
    {
        str_len = 512;
    }
    /* maloc mem */
    snd_packet = malloc(str_len + 4);
    if (snd_packet == NULL)
    {
        return;
    }
    _private = xfer->_private;
    // Send err msg.
    snd_packet[0] = htons(TFTP_CMD_ERROR);
    snd_packet[1] = htons(err_no);
    strncpy((char *)&snd_packet[2], err_msg, str_len);
    sendto(xfer->sock, snd_packet, str_len + 4, 0, (struct sockaddr *)&_private->sender, sizeof(struct sockaddr_in));
    free(snd_packet);
}

int tftp_resp_ack(struct tftp_xfer *xfer)
{
    uint16_t snd_packet[2];
    struct tftp_xfer_private *_private;
    int size;

    _private = xfer->_private;
    // Send ACK.
    snd_packet[0] = htons(TFTP_CMD_ACK);
    snd_packet[1] = htons(_private->block);
    size = sendto(xfer->sock, snd_packet, sizeof(snd_packet), 0, (struct sockaddr *)&_private->sender, sizeof(struct sockaddr_in));
    if (size != sizeof(snd_packet))
    {
        return -TFTP_EXFER;
    }
    return TFTP_OK;
}

int tftp_wait_ack(struct tftp_xfer *xfer)
{
    int r_size;
    struct tftp_xfer_private *_private;
    uint16_t recv_buff[4];

    _private = xfer->_private;
    /* Receiving raw data */
    r_size = tftp_recv_raw_data(xfer, recv_buff, sizeof(recv_buff));
    /* check is ack */
    if (r_size >= 4 && ntohs(recv_buff[0]) == TFTP_CMD_ACK && ntohs(recv_buff[1]) == _private->block)
    {
        _private->block++;
        return TFTP_OK;
    }
    if (r_size < 0)
    {
        return r_size;
    }
    else if (r_size < 4)
    {
        return -TFTP_EDATA;
    }
    else if (ntohs(recv_buff[0]) != TFTP_CMD_ACK)
    {
        return -TFTP_EACK;
    }
    else if (ntohs(recv_buff[1]) != _private->block)
    {
        return -TFTP_EBLK;
    }
    return -TFTP_EOTHER;
}

int tftp_read_data(struct tftp_xfer *xfer, struct tftp_packet *pack, int len)
{
    int r_size;
    struct tftp_xfer_private *_private;

    _private = xfer->_private;
    /* Receiving raw data */
    r_size = tftp_recv_raw_data(xfer, pack, len);
    /* Check that the data is correct  */
    if (r_size >= 4 && ntohs(pack->cmd) == TFTP_CMD_DATA && ntohs(pack->info.block) == (uint16_t)(_private->block + 1))
    {
        _private->block = ntohs(pack->info.block);
        /* Return data length */
        return r_size - 4;
    }
    if (r_size < 0)
    {
        return r_size;
    }
    else if (r_size < 4)
    {
        tftp_printf("Bad packet: r_size=%d\n", r_size);
        return -TFTP_EDATA;
    }
    else if (ntohs(pack->cmd) == TFTP_CMD_ERROR)
    {
        tftp_printf("err[%d] msg:%s code:%d\n", ntohs(pack->info.code), pack->data, ntohs(pack->info.code));
        return -TFTP_ECMD;
    }
    else if ((_private->block + 1) != pack->info.block)
    {
        tftp_printf("Bad block recv:%d != check:%d\n", _private->block + 1, pack->info.block);
        return -TFTP_EBLK;
    }

    return -TFTP_EOTHER;
}

int tftp_write_data(struct tftp_xfer *xfer, struct tftp_packet *pack, int len)
{
    struct tftp_xfer_private *_private;
    int size;

    _private = xfer->_private;
    /* Packing header */
    pack->cmd = htons(TFTP_CMD_DATA);
    pack->info.block = htons(_private->block);
    /* Send data */
    size = sendto(xfer->sock, pack, len, 0, (struct sockaddr *)&_private->sender, sizeof(struct sockaddr_in));
    if (size != len)
    {
        return -TFTP_EXFER;
    }
    return size;
}

int tftp_send_request(struct tftp_xfer *xfer, uint16_t cmd, const char *remote_file)
{
    struct tftp_packet *send_packet;
    struct tftp_xfer_private *_private;
    int size, r_size;
    int res;

    _private = xfer->_private;
    /* Check connection type */
    if (xfer->type != TFTP_XFER_TYPE_CLIENT)
    {
        res = tftp_xfer_type_set(xfer, TFTP_XFER_TYPE_CLIENT);
        if (res != TFTP_OK)
        {
            return res;
        }
    }
    /* malloc mem */
    send_packet = malloc(sizeof(struct tftp_packet));
    if (send_packet == NULL)
    {
        return -TFTP_EMEM;
    }
    /* Packing request packet header */
    send_packet->cmd = htons(cmd);
    size = sprintf(send_packet->info.filename, "%s%c%s%c%d%c", remote_file, 0, xfer->mode, 0, xfer->blksize, 0) + 3;
    /* send data */
    r_size = sendto(xfer->sock, send_packet, size, 0, (struct sockaddr *)&_private->server, sizeof(struct sockaddr_in));
    free(send_packet);
    if (size != r_size)
    {
        return -TFTP_EXFER;
    }
    return TFTP_OK;
}

struct tftp_xfer *tftp_recv_request(struct tftp_xfer *xfer, struct tftp_packet *packet)
{
    struct tftp_xfer_private *_private;
    int size, mem_size;
    struct tftp_xfer *client_xfer = NULL;

    _private = xfer->_private;
    /* Check connection type */
    if (xfer->type != TFTP_XFER_TYPE_SERVER)
    {
        if (tftp_xfer_type_set(xfer, TFTP_XFER_TYPE_SERVER) != true)
        {
            return NULL;
        }
    }

    /* get packet size */
    mem_size = sizeof(struct tftp_packet);
    memset(packet, 0, mem_size);
    /* Receiving raw data */
    size = tftp_recv_raw_data(xfer, packet, mem_size);
    if (size > 0)
    {
        /* Determine the type of request */
        if (ntohs(packet->cmd) == TFTP_CMD_RRQ || ntohs(packet->cmd) == TFTP_CMD_WRQ)
        {
            /* Create connection */
            client_xfer = tftp_xfer_create(inet_ntoa(_private->sender.sin_addr),
                                           ntohs(_private->sender.sin_port));
            /* Copy connection information */
            if (client_xfer != NULL)
            {
                struct tftp_xfer_private *_client_private = client_xfer->_private;
                memcpy(&_client_private->sender, &_private->sender, sizeof(struct sockaddr_in));
            }
            if (ntohs(packet->cmd) == TFTP_CMD_RRQ)
            {
                ((struct tftp_xfer_private *)client_xfer->_private)->block = 1;
            }
        }
    }

    return client_xfer;
}

int tftp_xfer_type_set(struct tftp_xfer *xfer, int type)
{
    struct tftp_xfer_private *_private;

    _private = xfer->_private;
    /* Setting connection type for client */
    if (type == TFTP_XFER_TYPE_CLIENT)
    {
        /* Check whether the type is set */
        if (xfer->type != TFTP_XFER_TYPE_CLIENT)
        {
            /* Server type has been set. return */
            if (xfer->type == TFTP_XFER_TYPE_SERVER)
            {
                return -TFTP_EINVAL;
            }
            /* Initialize client connection */
            _private->server.sin_family = PF_INET;
            _private->server.sin_port = htons(_private->port);
            _private->server.sin_addr.s_addr = inet_addr(_private->ip_addr);
            xfer->type = TFTP_XFER_TYPE_CLIENT;
            return TFTP_OK;
        }
    }
    else if (type == TFTP_XFER_TYPE_SERVER)
    {
        if (xfer->type != TFTP_XFER_TYPE_SERVER)
        {
            if (xfer->type == TFTP_XFER_TYPE_CLIENT)
            {
                return -TFTP_EINVAL;
            }
            _private->server.sin_family = AF_INET;
            _private->server.sin_addr.s_addr = INADDR_ANY;
            _private->server.sin_port = htons(_private->port);
            /* Binding port */
            if (bind(xfer->sock, (struct sockaddr *)&_private->server, sizeof(struct sockaddr_in)) < 0)
            {
                tftp_printf("tftp server bind failed!! exit\n");
                return -TFTP_ESYS;
            }
            xfer->type = TFTP_XFER_TYPE_SERVER;
            return TFTP_OK;
        }
    }
    return -TFTP_EINVAL;
}

void tftp_xfer_mode_set(struct tftp_xfer *xfer, const char *mode)
{
    if (xfer->mode)
    {
        free(xfer->mode);
    }
    xfer->mode = strdup(mode);
}

int tftp_xfer_blksize_set(struct tftp_xfer *xfer, int blksize)
{
    if ((blksize < 8) || (blksize > XFER_DATA_SIZE_MAX))
    {
        return -TFTP_EINVAL;
    }
    xfer->blksize = blksize;

    return TFTP_OK;
}

struct tftp_xfer *tftp_xfer_create(const char *ip_addr, int port)
{
    int sock;
    struct tftp_xfer *xfer;
    struct tftp_xfer_private *_private;
    int mem_len;

    /* malloc connect object */
    mem_len = sizeof(struct tftp_xfer) + sizeof(struct tftp_xfer_private);
    xfer = malloc(mem_len);
    if (xfer == NULL)
    {
        tftp_printf("can't create tftp transfer!! exit\n");
        return NULL;
    }
    memset(xfer, 0, mem_len);
    _private = (struct tftp_xfer_private *)&xfer[1];

    /* create socket */
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        tftp_printf("can't create socket!! exit\n");
        free(xfer);
        return NULL;
    }

    /* Initialize private data */
    _private->ip_addr = strdup(ip_addr);
    _private->port = port;
    _private->block = 0;
    xfer->sock = sock;
    xfer->mode = strdup(TFTP_XFER_OCTET);
    xfer->blksize = XFER_DATA_SIZE_MAX;
    xfer->_private = _private;
    return xfer;
}

void tftp_xfer_destroy(struct tftp_xfer *xfer)
{
    struct tftp_xfer_private *_private;

    /* free all mem */
    _private = xfer->_private;
    closesocket(xfer->sock);
    free(_private->ip_addr);
    free(xfer->mode);
    free(xfer);
}
