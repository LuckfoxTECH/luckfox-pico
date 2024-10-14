/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-02-26     tyx          first implementation
 */

#ifndef __TFTP_H__
#define __TFTP_H__

#define TFTP_OK         (0)
#define TFTP_ETIMEOUT   (2)
#define TFTP_EMEM       (3)
#define TFTP_ESYS       (4)
#define TFTP_EACK       (5)
#define TFTP_EBLK       (6)
#define TFTP_EDATA      (7)
#define TFTP_EFILE      (8)
#define TFTP_ECMD       (9)
#define TFTP_EINVAL     (10)
#define TFTP_EXFER      (11)
#define TFTP_EOTHER     (10000)

#define TFTP_MAX_RETRY (3)
#define TFTP_SERVER_CONNECT_MAX (5)

#define tftp_printf printf

struct tftp_client
{
    int max_retry;
    void *_private;
};

struct tftp_server
{
    int is_stop;
    int is_write;
    char *root_name;
    void *_private;
};

struct tftp_client *tftp_client_create(const char *ip_addr, int port);
void tftp_client_destroy(struct tftp_client *client);
int tftp_client_push(struct tftp_client *client, const char *local_name, const char *remote_name);
int tftp_client_pull(struct tftp_client *client, const char *remote_name, const char *local_name);
struct tftp_server *tftp_server_create(const char *root_name, int port);
void tftp_server_run(struct tftp_server *server);
void tftp_server_destroy(struct tftp_server *server);
void tftp_server_write_set(struct tftp_server *server, int is_write);
#endif
