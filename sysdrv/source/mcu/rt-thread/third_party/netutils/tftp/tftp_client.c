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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/select.h>
#include "tftp_xfer.h"
#include "tftp.h"

struct tftp_client_private
{
    struct tftp_xfer *xfer;
    fd_set fdr;
    struct timeval timeout;
};

extern void *tftp_file_open(const char *fname, const char *mode, int is_write);
extern int tftp_file_write(void *handle, int pos, void *buff, int len);
extern int tftp_file_read(void *handle, int pos, void *buff, int len);
extern void tftp_file_close(void *handle);
extern int tftp_thread_create(void **task, void (*entry)(void *param), void *param);

static int tftp_client_select(struct tftp_client_private *_private)
{
    int ret;
    FD_ZERO(&_private->fdr);
    FD_SET(_private->xfer->sock, &_private->fdr);
    _private->timeout.tv_sec = 5;
    _private->timeout.tv_usec = 0;
    ret = select(_private->xfer->sock + 1, &_private->fdr, NULL, NULL, (void *)&_private->timeout);
    if (ret == 0)
    {
        return -TFTP_ETIMEOUT;
    }
    else if (ret < 0)
    {
        return -TFTP_ESYS;
    }
    return ret;
}

struct tftp_client *tftp_client_create(const char *ip_addr, int port)
{
    struct tftp_client_private *_private;
    struct tftp_client *client;

    /* malloc client mem */
    client = malloc(sizeof(struct tftp_client) + sizeof(struct tftp_client_private));
    if (client == NULL)
    {
        tftp_printf("create client failed!! exit \n");
        return NULL;
    }
    /* Creating Private Data */
    _private = (struct tftp_client_private *)&client[1];
    /* Create a client connection */
    _private->xfer = tftp_xfer_create(ip_addr, port);
    if (_private->xfer == NULL)
    {
        tftp_printf("tftp xfer create failed!! exit\n");
        free(client);
        return NULL;
    }
    /* Number of Initial Retries */
    client->max_retry = TFTP_MAX_RETRY;
    /* Binding Private Data */
    client->_private = _private;
    return client;
}

void tftp_client_destroy(struct tftp_client *client)
{
    struct tftp_client_private *_private;

    _private = client->_private;
    /* Release connection objects */
    tftp_xfer_destroy(_private->xfer);
    /* Free memory */
    free(client);
}

int tftp_client_push(struct tftp_client *client, const char *local_name, const char *remote_name)
{
    struct tftp_client_private *_private;
    void *fp;
    struct tftp_packet *pack;
    int send_size, r_size;
    int file_size = 0;
    int res;
    int max_retry;

    _private = client->_private;
    max_retry = client->max_retry;
    while (max_retry)
    {
        /* Send Write Request */
        res = tftp_send_request(_private->xfer, TFTP_CMD_WRQ, remote_name);
        if (res != TFTP_OK)
        {
            tftp_printf("tftp send request failed !! retry:%d. exit\n", client->max_retry - max_retry);
            max_retry = 0;
            break;
        }
        /* Waiting for server response */
        res = tftp_client_select(_private);
        if (res > 0 && FD_ISSET(_private->xfer->sock, &_private->fdr))
        {
            /* Receive the server response */
            break;
        }
        else if (res == -TFTP_ETIMEOUT)
        {
            tftp_printf("tftp selct timeout. retry\n");
            max_retry --;
            continue;
        }
        else
        {
            /* Waiting for Response Error */
            tftp_printf("tftp selct err:%d. exit\n", res);
            max_retry = 0;
            break;
        }
    }

    if (max_retry == 0)
    {
        return res;
    }
    /* Receiving ACK */
    res = tftp_wait_ack(_private->xfer);
    if (res != TFTP_OK)
    {
        tftp_printf("wait ack failed!! exit\n");
        return res;
    }
    /* Open file */
    fp = tftp_file_open(local_name, _private->xfer->mode, 1);
    if (fp == NULL)
    {
        tftp_printf("open file \"%s\" error.\n", local_name);
        return -TFTP_EFILE;
    }
    pack = malloc(sizeof(struct tftp_packet));
    if (pack == NULL)
    {
        tftp_transfer_err(_private->xfer, 0, "malloc pack failed!");
        tftp_file_close(fp);
        return -TFTP_EMEM;
    }
    while (1)
    {
        /* read file */
        r_size = tftp_file_read(fp, file_size, &pack->data, _private->xfer->blksize);
        if (r_size < 0)
        {
            max_retry = 0;
            break;
        }
        max_retry = client->max_retry;
        while (max_retry)
        {
            /* Send data to server */
            send_size = tftp_write_data(_private->xfer, pack, r_size + 4);
            if (send_size != (r_size + 4))
            {
                tftp_transfer_err(_private->xfer, 0, "send file err!");
                max_retry = 0;
                break;
            }
            /* Wait server ACK */
            res = tftp_client_select(_private);
            if (res > 0 && FD_ISSET(_private->xfer->sock, &_private->fdr))
            {
                /* Receive a server ACK */
                break;
            }
            else if (res == -TFTP_ETIMEOUT)
            {
                tftp_printf("tftp selct timeout. retry\n");
                max_retry --;
                continue;
            }
            else
            {
                tftp_printf("tftp selct err:%d. exit\n", res);
                max_retry = 0;
                break;
            }
        }

        if (max_retry == 0)
        {
            break;
        }
        /* Receiving ACK */
        if (tftp_wait_ack(_private->xfer) != TFTP_OK)
        {
            tftp_printf("wait ack failed!! exit\n");
            break;
        }
        file_size += r_size;
        if (r_size < _private->xfer->blksize)
        {
            break;
        }
    }
    /* close file */
    tftp_file_close(fp);
    free(pack);
    return file_size;
}

int tftp_client_pull(struct tftp_client *client, const char *remote_name, const char *local_name)
{
    struct tftp_client_private *_private;
    void *fp;
    struct tftp_packet *pack;
    int recv_size, w_size;
    int file_size = 0;
    int res;
    int max_retry;

    _private = client->_private;
    max_retry = client->max_retry;
    while (max_retry)
    {
        /* Send Read File Request */
        res = tftp_send_request(_private->xfer, TFTP_CMD_RRQ, remote_name);
        if (res != TFTP_OK)
        {
            tftp_printf("tftp send request failed !! retry:%d. exit\n", max_retry);
            max_retry = 0;
            break;
        }
        /* Waiting for the server to respond to the request */
        res = tftp_client_select(_private);
        if (res > 0 && FD_ISSET(_private->xfer->sock, &_private->fdr))
        {
            /* Receive the server response */
            break;
        }
        else if (res == -TFTP_ETIMEOUT)
        {
            tftp_printf("tftp selct timeout. retry\n");
            max_retry --;
            continue;
        }
        else
        {
            tftp_printf("tftp selct err:%d. exit\n", res);
            max_retry = 0;
            break;
        }        
    }

    /* More than the maximum number of retries. exit */
    if (max_retry == 0)
    {
        return res;
    }

    /* Request successful. open file */
    fp = tftp_file_open(local_name, _private->xfer->mode, 1);
    if (fp == NULL)
    {
        tftp_printf("open file \"%s\" error.\n", local_name);
        return -TFTP_EFILE;
    }
    pack = malloc(sizeof(struct tftp_packet));
    if (pack == NULL)
    {
        /* malloc failed. send err msg and exit */
        tftp_transfer_err(_private->xfer, 0, "malloc pack failed!");
        tftp_file_close(fp);
        return -TFTP_EMEM;
    }
    while (1)
    {
        /* Receiving data from server */
        recv_size = tftp_read_data(_private->xfer, pack, \
            (int)((uint8_t *)&pack->data - (uint8_t *)pack) + _private->xfer->blksize);
        if (recv_size < 0)
        {
            tftp_printf("read data err[%d]! exit\n", recv_size);
            break;
        }
        /* Write data to file */
        w_size = tftp_file_write(fp, file_size, &pack->data, recv_size);
        if (w_size != recv_size)
        {
            tftp_printf("write file err! exit\n");
            tftp_transfer_err(_private->xfer, 0, "write file err!");
            break;
        }
        file_size += recv_size;
        /* Data less than one package. Completion of reception */
        if (recv_size < _private->xfer->blksize)
        {
            tftp_resp_ack(_private->xfer);
            break;
        }
        max_retry = client->max_retry;
        while (max_retry)
        {
            /* Send a response signal */
            tftp_resp_ack(_private->xfer);
            /* Waiting for the server to send data */
            res = tftp_client_select(_private);
            if (res > 0 && FD_ISSET(_private->xfer->sock, &_private->fdr))
            {
                break;
            }
            else if (res == -TFTP_ETIMEOUT)
            {
                tftp_printf("tftp selct timeout. retry\n");
                max_retry --;
            }
            else
            {
                tftp_printf("tftp selct err:%d. exit\n", res);
                max_retry = 0;
                break;
            }
        }
        if (max_retry == 0)
        {
            break;
        }
    }
    /* close file */
    tftp_file_close(fp);
    free(pack);
    return file_size;
}
