/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: sample cli file.
 * Author: Hisilicon
 * Create: 2020-09-09
 */

/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "hi_types.h"
#include "securec.h"
#include "hi_base.h"

#include "cJSON.h"

/*****************************************************************************
  4 函数实现
*****************************************************************************/
hi_void client_wait_event(hi_u8 cmdval)
{
    ssize_t recvbytes;
    char buf[SOCK_BUF_MAX];
    struct sockaddr_in clientaddr;
    socklen_t addrlen = sizeof(clientaddr);
    struct timeval tv;

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        sample_log_print("socket error:%s\n", strerror(errno));
        return -1;
    }

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(SOCK_EVENT_PORT);

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        sample_log_print("bind error:%s", strerror(errno));
        goto error_bind;
    }

    tv.tv_sec = 10;
    tv.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv)) != 0) {
        sample_log_print("setsockopt error:%s", strerror(errno));
        goto error_setsockopt;
    }

    memset(buf, 0, sizeof(buf));
    recvbytes = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&clientaddr, &addrlen);
    if (recvbytes < 0) {
        sample_log_print("recvfrom error:%s\n", strerror(errno));
        goto error_recvfrom;
    }

    sample_log_print("SAMPLE_CLIENT:--[%s]-[%d]--\n", buf, strlen(buf));
    cJSON* root = cJSON_Parse(buf);
    if (NULL == root) {
        sample_log_print("SAMPLE_CLIENT:parseJson---Parse fail\n");
        goto error_recvfrom;
    }

    cJSON* item = cJSON_GetObjectItem(root, "cmd");
    if (NULL != item) {
        if (cmdval == atoi(item->valuestring)) {
        }
    }

    cJSON_Delete(root);

error_bind:
error_setsockopt:
error_recvfrom:
    close(sockfd);
}

hi_void client_send_cmd(hi_s32 sockfd, const hi_char *cmd, hi_u32 cmd_len)
{
    struct sockaddr_in servaddr;
    ssize_t recvbytes;
    hi_char buf[SOCK_BUF_MAX];
    struct sockaddr_in clientaddr;
    socklen_t addrlen = sizeof(clientaddr);
    struct timeval tv;
    int retry = 3;

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(SOCK_PORT);

    if (cmd_len > SOCK_BUF_MAX) {
        sample_log_print("cmd length is overflow\n");
        return;
    }

    if (sendto(sockfd, cmd, strlen(cmd), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        sample_log_print("SAMPLE_CLIENT: sendto error:%s\n", strerror(errno));
        return;
    }

    tv.tv_sec = 10;
    tv.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv)) != 0) {
        sample_log_print("setsockopt error:%s", strerror(errno));
        return;
    }

    memset(buf, 0, sizeof(buf));
    recvbytes = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&clientaddr, &addrlen);
    if (recvbytes < 0) {
        sample_log_print("recvfrom error:%s\n", strerror(errno));
        return;
    }

    if (strcmp("OK", buf) != 0) {
        sample_log_print("SAMPLE_CLIENT: sendto cmd error!\n");
        return;
    }

    if (strcmp("getmac", cmd) == 0) {
        client_wait_event(2);
    } else if (strcmp("getip", cmd) == 0) {
        client_wait_event(3);
    } else if (strcmp("wakecode", cmd) == 0) {
        client_wait_event(15);
    } else if (strcmp("getall", cmd) == 0) {
        client_wait_event(20);
    }

    sample_log_print("SAMPLE_CLIENT: send cmd ok!\n");
}

hi_s32 main(hi_s32 argc, const hi_char **argv)
{
    (void)argc;
    hi_char cmd[SOCK_BUF_MAX] = {0};
    if (argc < 2 || argv == NULL) { /* 2:参数个数 */
        sample_log_print("usage sample_cli <cmd>\n");
        return -1;
    }
    if (strcmp("pirset", argv[1]) == 0) {
        if (sprintf_s(cmd, sizeof(cmd), "%s %s", argv[1], argv[2]) < 0) {
            sample_log_print("sprintf_s is error\n");
        }
    } else {
        if (strcpy_s(cmd, sizeof(cmd), argv[1]) < 0) {
            sample_log_print("strcpt is error\n");
        }
        cmd[strlen(argv[1])] = '\0';
    }

    hi_s32 sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        sample_log_print("socket is error:%s\n", strerror(errno));
        return -1;
    }

    client_send_cmd(sockfd, cmd, strlen(cmd));
    close(sockfd);
    return 0;
}

