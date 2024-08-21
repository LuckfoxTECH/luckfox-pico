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

#define TFTP_SERVER_EVENT_CONNECT   (0x1 << 0)
#define TFTP_SERVER_EVENT_DATA      (0x1 << 1)
#define TFTP_SERVER_EVENT_TIMEOUT   (0x1 << 2)

#define TFTP_SERVER_FILE_NAME_MAX   (512)

#define TFTP_SERVER_REQ_READ    (0x0)
#define TFTP_SERVER_REQ_WRITE   (0x1)

extern void *tftp_file_open(const char *fname, const char *mode, int is_write);
extern int tftp_file_write(void *handle, int pos, void *buff, int len);
extern int tftp_file_read(void *handle, int pos, void *buff, int len);
extern void tftp_file_close(void *handle);

struct tftp_client_xfer
{
    struct tftp_xfer *xfer;
    int16_t w_r;
    int16_t retry;
    int pos;
    int last_read;
    void *fd;
};

struct tftp_server_private
{
    struct tftp_xfer *server_xfer;
    struct tftp_client_xfer *client_table;
    int table_num;
    fd_set fdr;
    struct timeval timeout;
};

static int tftp_server_select(struct tftp_server *server)
{
    struct tftp_server_private *_private;
    int max_sock, i;
    int ret;

    _private = server->_private;
    FD_ZERO(&_private->fdr);
    /* Select server */
    FD_SET(_private->server_xfer->sock, &_private->fdr);
    max_sock = _private->server_xfer->sock;
    /* Select all client connections */
    for (i = 0; i < _private->table_num; i++)
    {
        if (_private->client_table[i].xfer != NULL)
        {
            FD_SET(_private->client_table[i].xfer->sock, &_private->fdr);
            if (max_sock < _private->client_table[i].xfer->sock)
            {
                max_sock = _private->client_table[i].xfer->sock;
            }
        }
    }
    /* Setting timeout time */
    _private->timeout.tv_sec = 5;
    _private->timeout.tv_usec = 0;
    ret = select(max_sock + 1, &_private->fdr, NULL, NULL, (void *)&_private->timeout);
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

static struct tftp_client_xfer *tftp_client_xfer_get(struct tftp_server *server, int index)
{
    struct tftp_server_private *_private;

    _private = server->_private;
    if (_private->table_num > index)
    {
        return &_private->client_table[index];
    }
    return NULL;
}

static struct tftp_client_xfer *tftp_client_xfer_add(struct tftp_server *server, struct tftp_xfer *xfer)
{
    struct tftp_server_private *_private;
    int i;

    _private = server->_private;
    /* View space and add */
    for (i = 0; i < _private->table_num; i++)
    {
        if (_private->client_table[i].xfer == NULL)
        {
            memset(&_private->client_table[i], 0, sizeof(struct tftp_client_xfer));
            _private->client_table[i].xfer = xfer;
            return &_private->client_table[i];
        }
    }
    return NULL;
}

static void tftp_client_xfer_delete(struct tftp_server *server, struct tftp_xfer *xfer)
{
    struct tftp_server_private *_private;
    int i;

    _private = server->_private;
    /* Find a clinet xfer and remove */
    for (i = 0; i < _private->table_num; i++)
    {
        if (_private->client_table[i].xfer == xfer)
        {
            _private->client_table[i].xfer = NULL;
            break;
        }
    }
}

static void tftp_client_xfer_destroy(struct tftp_server *server, struct tftp_client_xfer *client)
{
    /* Close client connection */
    tftp_xfer_destroy(client->xfer);
    /* close file */
    tftp_file_close(client->fd);
    /* Delete client */
    tftp_client_xfer_delete(server, client->xfer);
}

static void tftp_server_send_file(struct tftp_server *server, struct tftp_client_xfer *client, struct tftp_packet *packet, bool resend)
{
    int r_size, s_size;
    int retry = TFTP_MAX_RETRY;

    if (resend == false)
    {
        client->pos += client->last_read;
    }
    /* read file */
    r_size = tftp_file_read(client->fd, client->pos, &packet->data, client->xfer->blksize);
    if (r_size < 0)
    {
        r_size = 0;
    }
    while (1)
    {
        /* Send data to client */
        s_size = tftp_write_data(client->xfer, packet, r_size + 4);
        if (r_size == (s_size - 4))
        {
            break;
        }
        /* Failed to send data. retry */
        if (retry-- == 0)
        {
            break;
        }
    }
    /* Maximum number of retries */
    if (retry == 0)
    {
        /* Destroy client connection */
        tftp_client_xfer_destroy(server, client);
    }
    else
    {
        client->last_read = r_size;
    }
}

static void tftp_server_send_ack(struct tftp_server *server, struct tftp_client_xfer *client)
{
    int retry = TFTP_MAX_RETRY;
    bool res;

    while (1)
    {
        /* send ack */
        res = tftp_resp_ack(client->xfer);
        if (res == TFTP_OK)
        {
            break;
        }
        /* send failed. retry */
        if (retry-- == 0)
        {
            break;
        }
    }
    if (retry == 0)
    {
        /* Maximum number of retries */
        tftp_client_xfer_destroy(server, client);
    }
}

static void tftp_server_transf_handle(struct tftp_server *server, struct tftp_client_xfer *client, int event, struct tftp_packet *packet)
{
    switch (event)
    {
    case TFTP_SERVER_EVENT_CONNECT:
        if (client->w_r == TFTP_SERVER_REQ_READ)
        {
            /* Read the file request and return the file data */
            tftp_server_send_file(server, client, packet, false);
        }
        else
        {
            /* Write file request, return ACK */
            tftp_server_send_ack(server, client);
        }
        break;
    case TFTP_SERVER_EVENT_DATA:
        /* Receive data from client */
        if (client->w_r == TFTP_SERVER_REQ_READ)
        {
            /* If reques is read. Receive ACK */
            if (tftp_wait_ack(client->xfer) == TFTP_OK)
            {
                /* Receive ACK success. If it's the last package of data, close client */
                if (client->last_read < client->xfer->blksize)
                {
                    tftp_client_xfer_destroy(server, client);
                    break;
                }
                /* Receive ACK success. Continue sending data */
                tftp_server_send_file(server, client, packet, false);
                client->retry = TFTP_MAX_RETRY;
            }
            else
            {
                /* Receive ACK failed. close client */
                tftp_transfer_err(client->xfer, 0, "err ack!");
                tftp_client_xfer_destroy(server, client);
            }
        }
        else
        {
            /* Write File Request handle */
            int recv_size, w_size;
            /* Receiving File Data from Client */
            recv_size = tftp_read_data(client->xfer, packet,
                                       (int)((uint8_t *)&packet->data - (uint8_t *)packet) + client->xfer->blksize);
            if (recv_size < 0)
            {
                /* Receiving failed. */
                tftp_printf("server read data err! disconnect client\n");
                tftp_client_xfer_destroy(server, client);
            }
            else
            {
                /* write file */
                w_size = tftp_file_write(client->fd, client->pos, &packet->data, recv_size);
                if (w_size != recv_size)
                {
                    /* Write file error, close connection */
                    tftp_printf("server write file err! disconnect client\n");
                    tftp_transfer_err(client->xfer, 0, "write file err!");
                    tftp_client_xfer_destroy(server, client);
                    break;
                }
                /* Reply ack */
                tftp_server_send_ack(server, client);
                client->pos += recv_size;
                /* Receive the last packet of data. close client */
                if (recv_size < client->xfer->blksize)
                {
                    tftp_client_xfer_destroy(server, client);
                }
            }
        }
        break;
    case TFTP_SERVER_EVENT_TIMEOUT:
        /* Timeout handle */
        if (client->w_r == TFTP_SERVER_REQ_READ)
        {
            /* resend file */
            if (client->retry > 0)
            {
                tftp_server_send_file(server, client, packet, true);
                client->retry--;
            }
            else
            {
                /* Maximum number of retransmissions */
                tftp_client_xfer_destroy(server, client);
            }
        }
        else
        {
            if (client->retry > 0)
            {
                /* resend ack */
                tftp_server_send_ack(server, client);
                client->retry--;
            }
            else
            {
                tftp_client_xfer_destroy(server, client);
            }
        }
        break;
    default:
        tftp_printf("warr!! unknown event:%d\n", event);
        break;
    }
}

static struct tftp_client_xfer *tftp_server_request_handle(struct tftp_server *server, struct tftp_packet *packet)
{
    struct tftp_xfer *xfer;
    struct tftp_server_private *_private;
    char *path, *full_path;
    int name_len;
    struct tftp_client_xfer *client_xfer;
    void *fd = NULL;
    char *mode;
    char *blksize_str;
    int blocksize;

    _private = server->_private;
    /* Receiving client requests */
    memset(packet, 0, sizeof(struct tftp_packet));
    xfer = tftp_recv_request(_private->server_xfer, packet);
    if (xfer == NULL)
    {
        return NULL;
    }
    /* Can write ? */
    if (ntohs(packet->cmd) == TFTP_CMD_WRQ && (!server->is_write))
    {
        tftp_printf("server read only!\n");
        tftp_transfer_err(xfer, 0, "server read only!");
        tftp_xfer_destroy(xfer);
        return NULL;
    }

    /* Get file path */
    path = packet->info.filename;
    /* Get transfer mode */
    mode = path + strlen(path) + 1;
    tftp_xfer_mode_set(xfer, mode);
    /* Get block size */
    blksize_str = mode + strlen(mode) + 1;
    if (strcmp(blksize_str, "blksize") == 0)
    {
        blocksize = atoi(blksize_str + strlen(blksize_str) + 1);
        if (tftp_xfer_blksize_set(xfer, blocksize) != TFTP_OK)
        {
            tftp_printf("set block size err:%d\n", blocksize);
            tftp_transfer_err(xfer, 0, "block size err!");
            tftp_xfer_destroy(xfer);
            return NULL;
        }
    }
    /* Get full file path */
    name_len = strlen(path) + strlen(server->root_name);
    if (name_len >= TFTP_SERVER_FILE_NAME_MAX)
    {
        tftp_printf("file name is to long!!\n");
        tftp_transfer_err(xfer, 0, "file name to long!");
        tftp_xfer_destroy(xfer);
        return NULL;
    }
    full_path = malloc(name_len);
    if (full_path == NULL)
    {
        tftp_printf("mallo full path failed!\n");
        tftp_transfer_err(xfer, 0, "server err!");
        tftp_xfer_destroy(xfer);
        return NULL;
    }

    strcpy(full_path, server->root_name);
    if (path[0] != '/')
    {
        strcat(full_path, "/");
    }
    strcat(full_path, path);

    /* open file */
    if (ntohs(packet->cmd) == TFTP_CMD_RRQ)
    {
        fd = tftp_file_open(full_path, TFTP_XFER_OCTET, 0);
    }
    else if (ntohs(packet->cmd) == TFTP_CMD_WRQ)
    {
        fd = tftp_file_open(full_path, TFTP_XFER_OCTET, 1);
    }
    free(full_path);
    if (fd == NULL)
    {
        tftp_printf("open file failed!\n");
        tftp_transfer_err(xfer, 0, "file err");
        tftp_xfer_destroy(xfer);
        return NULL;
    }
    /* push client to queue */
    client_xfer = tftp_client_xfer_add(server, xfer);
    if (client_xfer == NULL)
    {
        tftp_printf("too many connections!!");
        tftp_transfer_err(xfer, 0, "too many connections!");
        tftp_xfer_destroy(xfer);
        tftp_file_close(fd);
    }
    else
    {
        client_xfer->w_r = ntohs(packet->cmd) == TFTP_CMD_RRQ ? \
            TFTP_SERVER_REQ_READ : TFTP_SERVER_REQ_WRITE;
        client_xfer->retry = TFTP_MAX_RETRY;
        client_xfer->fd = fd;
        client_xfer->pos = 0;
        client_xfer->last_read = 0;
    }
    return client_xfer;
}

void tftp_server_run(struct tftp_server *server)
{
    struct tftp_xfer *xfer;
    struct tftp_packet *packet;
    struct tftp_server_private *_private;
    int res, i;
    struct tftp_client_xfer *client_xfer;

    if (server == NULL)
    {
        return;
    }
    _private = server->_private;
    /* malloc transport packet */
    packet = malloc(sizeof(struct tftp_packet));
    if (packet == NULL)
    {
        return;
    }
    /* Create connect */
    xfer = tftp_xfer_create("0.0.0.0", 69);
    if (xfer == NULL)
    {
        free(packet);
        return;
    }
    /* Set connection type to server */
    if (tftp_xfer_type_set(xfer, TFTP_XFER_TYPE_SERVER) != TFTP_OK)
    {
        free(packet);
        tftp_xfer_destroy(xfer);
        return;
    }
    _private->server_xfer = xfer;
    tftp_printf("tftp server start!\n");
    /* run server */
    while (!server->is_stop)
    {
        /* Waiting client data */
        res = tftp_server_select(server);
        if (res == -TFTP_ETIMEOUT)
        {
            /* Waiting for data timeout */
            for (i = 0; i < _private->table_num; i++)
            {
                if (_private->client_table[i].xfer != NULL)
                {
                    client_xfer = tftp_client_xfer_get(server, i);
                    tftp_server_transf_handle(server, client_xfer, TFTP_SERVER_EVENT_TIMEOUT, packet);
                }
            }
            continue;
        }
        else if (res < 0)
        {
            break;
        }
        else
        {
            /* Connection request handle */
            if (FD_ISSET(_private->server_xfer->sock, &_private->fdr))
            {
                client_xfer = tftp_server_request_handle(server, packet);
                if (client_xfer != NULL)
                {
                    tftp_server_transf_handle(server, client_xfer, TFTP_SERVER_EVENT_CONNECT, packet);
                }
            }
            /* Client data handle */
            for (i = 0; i < _private->table_num; i++)
            {
                if (_private->client_table[i].xfer != NULL &&
                    FD_ISSET(_private->client_table[i].xfer->sock, &_private->fdr))
                {
                    client_xfer = tftp_client_xfer_get(server, i);
                    tftp_server_transf_handle(server, client_xfer, TFTP_SERVER_EVENT_DATA, packet);
                }
            }
        }
    }
    /* exit. destroy all client */
    for (i = 0; i < _private->table_num; i++)
    {
        if (_private->client_table[i].xfer != NULL)
        {
            client_xfer = tftp_client_xfer_get(server, i);
            tftp_client_xfer_destroy(server, client_xfer);
        }
    }
    /* free server */
    tftp_xfer_destroy(_private->server_xfer);
    free(_private->client_table);
    free(server->root_name);
    free(server);
    free(packet);
    tftp_printf("tftp server stop!\n");
}

struct tftp_server *tftp_server_create(const char *root_name, int port)
{
    struct tftp_server_private *_private;
    struct tftp_server *server;
    int mem_len;

    /* new server object */
    mem_len = sizeof(struct tftp_server_private) + sizeof(struct tftp_server);
    server = malloc(mem_len);
    if (server == NULL)
    {
        return NULL;
    }
    /* init server object */
    memset(server, 0, mem_len);
    server->root_name = strdup(root_name);
    if (server->root_name == NULL)
    {
        free(server);
        return NULL;
    }
    _private = (struct tftp_server_private *)&server[1];
    server->_private = _private;
    mem_len = sizeof(struct tftp_client_xfer) * TFTP_SERVER_CONNECT_MAX;
    /* malloc client queue */
    _private->client_table = malloc(mem_len);
    if (_private->client_table == NULL)
    {
        free(server);
        return NULL;
    }
    memset(_private->client_table, 0, mem_len);
    _private->table_num = TFTP_SERVER_CONNECT_MAX;
    return server;
}

void tftp_server_write_set(struct tftp_server *server, int is_write)
{
    if (server != NULL)
    {
        server->is_write = is_write;
    }
}

void tftp_server_destroy(struct tftp_server *server)
{
    if (server != NULL)
    {
        server->is_stop = 1;
    }
}
