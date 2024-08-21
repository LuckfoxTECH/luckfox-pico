/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-02-26     tyx          first implementation
 */

#include <rtthread.h>
#include <dfs_posix.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tftp.h"

RT_WEAK void *tftp_file_open(const char *fname, const char *mode, int is_write)
{
    int fd = 0;

    if (!rt_strcmp(mode, "octet"))
    {
        if (is_write)
        {
            fd = open(fname, O_WRONLY | O_CREAT, 0);
        }
        else
        {
            fd = open(fname, O_RDONLY, 0);
        }
    }
    else
    {
        rt_kprintf("tftp: No support this mode(%s).", mode);
    }

    return (void *)fd;
}

RT_WEAK int tftp_file_write(void *handle, int pos, void *buff, int len)
{
    int fd = (int)handle;

    return write(fd, buff, len);
}

RT_WEAK int tftp_file_read(void *handle, int pos, void *buff, int len)
{
    int fd = (int)handle;

    return read(fd, buff, len);
}

RT_WEAK void tftp_file_close(void *handle)
{
    close((int)handle);
}

static struct tftp_server *server;

static void tftp_server_thread(void *param)
{
    tftp_server_run((struct tftp_server *)param);
    server = RT_NULL;
}

#define _S_MODE_CMD        (100)
#define _CW_MODE_CMD       (101)
#define _CR_MODE_CMD       (102)
#define _IP_MODE_CMD       (103)
#define _P_MODE_CMD        (104)
#define _STOP_MODE_CMD     (107)
#define _UNKNOWN_MODE_CMD  (0)

struct _tftp_cmd
{
    const char *cmd_str;
    const char *help_info;
    int cmd;
};

static const struct _tftp_cmd _cmd_tab[] = 
{
    {"-s", "begin tftp server", _S_MODE_CMD},
    {"-w", "client write file to server", _CW_MODE_CMD},
    {"-r", "client read file from server", _CR_MODE_CMD},
    {"-p", "server port to listen on/connect to", _P_MODE_CMD},
    {"--stop", "stop tftp server", _STOP_MODE_CMD},
};

static void _tftp_help(void)
{
    int i;
    printf("Usage: tftp [-s|-w|-r host] [path]\n");
    printf("       tftp [-h|--stop]\n\n");
    for (i = 0; i < sizeof(_cmd_tab) / sizeof(_cmd_tab[0]); i++)
    {
        printf("       %-6.6s:  %s\n", _cmd_tab[i].cmd_str, _cmd_tab[i].help_info);
    }
    printf("\neg: \n");
    printf("    open server: tftp -s /\n");
    printf("    read  file : tftp -r 192.168.1.1 test.data\n");
    printf("    wriet file : tftp -w 192.168.1.1 test.data\n");
}

static int _tftp_msh(int argc, char *argv[])
{
    int i, j, cmd;
    int tftp_mode = 0;
    char *ip = RT_NULL;
    char *path[2] = {0};
    int port = 0, stop = 0;

    if (argc == 1)
    {
        goto _help;
    }
    for (i = 1; i < argc; i++)
    {
        cmd = _UNKNOWN_MODE_CMD;
        for (j = 0; j < sizeof(_cmd_tab) / sizeof(_cmd_tab[0]); j++)
        {
            if (strcmp(_cmd_tab[j].cmd_str, argv[i]) == 0)
            {
                cmd = _cmd_tab[j].cmd;
                break;
            }
        }
        switch (cmd)
        {
        case _S_MODE_CMD:
            if (tftp_mode != 0)
            {
                goto _help;
            }
            tftp_mode = _S_MODE_CMD;
            break;
        case _CW_MODE_CMD:
        case _CR_MODE_CMD:
            if (tftp_mode != 0)
            {
                goto _help;
            }
            tftp_mode = cmd;
            if ((i + 1) < argc)
            {
                ip = argv[i + 1];
                i ++;
            }
            break;
        case _P_MODE_CMD:
            if (port != 0)
            {
                goto _help;
            }
            if ((i + 1) < argc)
            {
                port = atoi(argv[i + 1]);
                i ++;
            }
            break;
        case _STOP_MODE_CMD:
            if (argc != 2)
            {
                goto _help;
            }
            stop = 1;
            break;
        default:
        {
            if (path[0] == RT_NULL)
            {
                path[0] = argv[i];
            }
            else if (path[1] == RT_NULL)
            {
                path[1] = argv[i];
            }
            else
            {
                goto _help;
            }
        }
        }
    }

    if (stop && server)
    {
        tftp_server_destroy(server);
        return 0;
    }

    if (port == 0)
    {
        port = 69;
    }
    if (tftp_mode == _S_MODE_CMD)
    {
        rt_thread_t tid;
        if (server != RT_NULL)
        {
            printf("tftp server is run");
            return 0;
        }
        if (path[0] == RT_NULL)
        {
            path[0] = "/";
        }
        server = tftp_server_create(path[0], port);
        tftp_server_write_set(server, 1);
        tid = rt_thread_create("tftps", tftp_server_thread, server, 2048, 18, 20);
        if (tid == NULL)
        {
            return -1;
        }
        rt_thread_startup(tid);
        return 0;
    }
    else if ((tftp_mode == _CW_MODE_CMD) || (tftp_mode == _CR_MODE_CMD))
    {
        struct tftp_client *client;

        if (ip == RT_NULL || path[0] == RT_NULL)
        {
            goto _help;
        }
        if (path[1] == RT_NULL)
        {
            path[1] = path[0];
        }
        client = tftp_client_create(ip, port);
        if (tftp_mode == _CR_MODE_CMD)
        {
            printf("file size:%d\n", tftp_client_pull(client, path[0], path[1]));
        }
        else
        {
            printf("file size:%d\n", tftp_client_push(client, path[0], path[1]));
        }
        tftp_client_destroy(client);
        return 0;
    }
    else
    {
        goto _help;
    }

_help:
    _tftp_help();
    return -1;
}
FINSH_FUNCTION_EXPORT_ALIAS(_tftp_msh, __cmd_tftp, tftp.);
