// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "smart_door.h"
#include "wifi_cmd.h"

static int send_cmd_to_wifi(const char *cmd, unsigned int cmd_len)
{
    struct sockaddr_in servaddr;
    ssize_t recvbytes;
    char buf[SOCK_BUF_MAX];
    struct sockaddr_in clientaddr;
    socklen_t addrlen = sizeof(clientaddr);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        RK_LOGE("socket is error:%s\n", strerror(errno));
        return -1;
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(SOCK_PORT);

    if (cmd_len > SOCK_BUF_MAX) {
        RK_LOGE("length is overflow\n");
        return -1;
    }

    if (sendto(sockfd, cmd, strlen(cmd), MSG_DONTWAIT, (const struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        RK_LOGE("sendto error:%s\n", strerror(errno));
        return -1;
    }

    memset(buf, 0, sizeof(buf));
    recvbytes = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&clientaddr, &addrlen);
    if (recvbytes < 0) {
        RK_LOGE("recvfrom error: %s\n", strerror(errno));
        return -1;
    }

    if (strcmp("OK", buf) != 0) {
        RK_LOGE("strcmp error!\n");
        return -1;
    }

    RK_LOGI("send cmd [%s] ok!\n", cmd);
    close(sockfd);
    return 0;
}

static int get_wakecode_or_event(char *data, unsigned int data_len)
{
    ssize_t recvbytes;
    char buf[SOCK_BUF_MAX];
    struct sockaddr_in clientaddr;
    socklen_t addrlen = sizeof(clientaddr);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        RK_LOGE("socket is error:%s\n", strerror(errno));
        return -1;
    }

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(SOCK_EVENT_PORT);

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        RK_LOGE("bind error:%s", strerror(errno));
        return -1;
    }

    if (data_len > SOCK_BUF_MAX) {
        RK_LOGE("length is overflow\n");
        return -1;
    }

    memset(buf, 0, sizeof(buf));
    recvbytes = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&clientaddr, &addrlen);
    if (recvbytes < 0) {
        RK_LOGE("recvfrom error: %s\n", strerror(errno));
        return -1;
    }
    memcpy(data, buf, recvbytes);
    close(sockfd);
    return recvbytes;
}

static int format_to_num(char *data)
{
    int cmd, num;
    int ret;

    ret = sscanf(data, "%*8s%d%*3s%*[a-z]%*2s%d", &cmd, &num);
    if (ret != 2) {
        RK_LOGE("format_to_num error!");
        return -1;
    }
    return (cmd << 8) + num;
}

static void do_work_by_wakecode_or_event(void)
{
    char wakecode_or_event[64];
    int data_len;

    memset(wakecode_or_event, 0, sizeof(wakecode_or_event));
    data_len = get_wakecode_or_event(wakecode_or_event, sizeof(wakecode_or_event));
    if (data_len > 0) {
        switch(format_to_num(wakecode_or_event)) {
            case WAKE_CODE_FIRST:
                RK_LOGI("first time power on\n");
                break;
            case WAKE_CODE_NET:
                RK_LOGI("power on by network\n");
                break;
            case WAKE_CODE_PIR:
                RK_LOGI("power on by PIR\n");
                break;
            case WAKE_CODE_KEY:
                RK_LOGI("power on by key\n");
                break;
            case EVENT_ID_KEY:
                RK_LOGI("key event\n");
                break;
            case EVENT_ID_PIR:
                RK_LOGI("PIR event\n");
                break;
        }
    } else {
        RK_LOGE("error, get wakecode or event fail!\n");
    }
}

void smart_door_wifi_cmd(void)
{
    char *cmd;

    cmd = "getip";
    if (send_cmd_to_wifi(cmd, strlen(cmd)))
        return;

    cmd = "getmac";
    if (send_cmd_to_wifi(cmd, strlen(cmd)))
        return;

    cmd = "getall";
    if (send_cmd_to_wifi(cmd, strlen(cmd)))
        return;

    cmd = "wakecode";
    if (send_cmd_to_wifi(cmd, strlen(cmd)))
        return;

    do_work_by_wakecode_or_event();

    cmd = "deepsleep";
    if (send_cmd_to_wifi(cmd, strlen(cmd)))
        return;
}
