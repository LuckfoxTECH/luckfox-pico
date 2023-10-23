/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: sample cli file.
 * Author: Hisilicon
 * Create: 2020-09-09
 */
/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/netlink.h>
#include <linux/sockios.h>
#include <linux/wireless.h>

#include "securec.h"
#include "hi_base.h"
#include "hichannel_host.h"

/*****************************************************************************
  2 宏定义、全局变量
*****************************************************************************/
#define NETLINK_SOCKET_PORT_ID                  1100
#define NETLINK_CHANNEL_MODEID                  28
#undef NLMSG_ALIGNTO
#define NLMSG_ALIGNTO                           1
#define USLEEP_TIMES                             10

typedef struct {
    hi_s32 skfd;
    pthread_t channel_thread;
    hi_channel_rx_func rx_func;
} netlink_monitor_s;
/*****************************************************************************
  3 枚举、结构体定义
*****************************************************************************/
static hi_bool            g_channel_terminate = HI_FALSE;
static netlink_monitor_s *g_channel_monitor = HI_NULL;
/*****************************************************************************
  4 函数实现
*****************************************************************************/
static hi_void *hi_channel_host_thread(hi_void *args)
{
    hi_s32 rev_len;
    hi_s32 payload_len;
    hi_char msg[SYSTEM_CMD_SIZE];
    struct nlmsghdr *nlh = HI_NULL;
    struct sockaddr_nl daddr;
    socklen_t len  = sizeof(struct sockaddr_nl);
    sample_unused(args);

    while (!g_channel_terminate) {
        (hi_void)memset_s(&msg[0], sizeof(msg), 0, sizeof(msg));
        rev_len = recvfrom(g_channel_monitor->skfd, &msg[0], sizeof(msg),
            MSG_WAITALL, (struct sockaddr *)&daddr, &len);
        if (rev_len == -1) {
            if (errno == EINTR) {
                usleep(USLEEP_TIMES);
                continue;
            } else {
                sample_log_print("recvfrom error! fd:%d\n", g_channel_monitor->skfd);
                return HI_NULL;
            }
        }

        if (rev_len <= NLMSG_HDRLEN) {
            usleep(USLEEP_TIMES);
            continue;
        }

        nlh = (struct nlmsghdr *)msg;
        payload_len = rev_len - NLMSG_HDRLEN;
        sample_log_print("hi_channel_host_thread:%x,%d,%d,%d\n", nlh->nlmsg_type, payload_len, rev_len, NLMSG_HDRLEN);
        if (g_channel_monitor->rx_func != HI_NULL) {
            g_channel_monitor->rx_func((hi_u8 *)NLMSG_DATA(nlh), payload_len);
        }
    }

    return HI_NULL;
}

hi_s32 hi_channel_register_rx_cb(hi_channel_rx_func rx_func)
{
    if ((g_channel_monitor == HI_NULL) || (rx_func == HI_NULL)) {
        sample_log_print("hi_channel_register_rx_cb is fail\n");
        return HI_FAILURE;
    }

    g_channel_monitor->rx_func = rx_func;
    return HI_SUCCESS;
}

hi_s32 hi_channel_send_to_dev(unsigned char *buf, int len)
{
    int ret;
    struct nlmsghdr *nlh = HI_NULL;
    struct sockaddr_nl daddr;

    if ((buf == HI_NULL) || (len <= 0) || (len > MAX_SEND_DATA_SIZE)) {
        sample_log_print("sendto sata len:%d\n", len);
        return HI_FAILURE;
    }

    (hi_void)memset_s(&daddr, sizeof(daddr), 0, sizeof(daddr));
    daddr.nl_family = AF_NETLINK; /* netlink id */
    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(len));
    if (nlh == HI_NULL) {
        sample_log_print("malloc mem is fail\n");
        return HI_FAILURE;
    }

    (hi_void)memset_s(nlh, sizeof(nlh), 0x00, sizeof(nlh));
    nlh->nlmsg_len = NLMSG_SPACE(len);
    nlh->nlmsg_pid = NETLINK_SOCKET_PORT_ID;
    (hi_void)memcpy_s(NLMSG_DATA(nlh), NLMSG_SPACE(len), buf, len);
    ret = sendto(g_channel_monitor->skfd, nlh, nlh->nlmsg_len, 0,
        (struct sockaddr *)&daddr, sizeof(struct sockaddr_nl));
    if (ret == -1) {
        sample_log_print("sendto error:%s\n", strerror(errno));
        free(nlh);
        return HI_FAILURE;
    }

    free(nlh);
    return 0;
}

hi_s32 hi_channel_init(hi_void)
{
    hi_s32 ret;
    struct sockaddr_nl saddr = {0};

    if (g_channel_monitor != HI_NULL) {
        sample_log_print("hi_channel_init is fail\n");
        return HI_FAILURE;
    }

    g_channel_monitor = (netlink_monitor_s *)malloc(sizeof(netlink_monitor_s));
    if (g_channel_monitor == HI_NULL) {
        return HI_FAILURE;
    }

    g_channel_terminate = HI_FALSE;
    (hi_void)memset_s(g_channel_monitor, sizeof(netlink_monitor_s), 0, sizeof(netlink_monitor_s));
    g_channel_monitor->skfd = -1;
    g_channel_monitor->skfd = socket(AF_NETLINK, SOCK_RAW, NETLINK_CHANNEL_MODEID);
    if (g_channel_monitor->skfd == -1) {
        sample_log_print("create is fail:%s\n", strerror(errno));
        goto deinit;
    }

    (hi_void)memset_s(&saddr, sizeof(saddr), 0x00, sizeof(saddr));
    saddr.nl_family = AF_NETLINK;          /* netlink id */
    saddr.nl_pid = NETLINK_SOCKET_PORT_ID; /* self pid */

    ret = bind(g_channel_monitor->skfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (ret != 0) {
        goto deinit;
    }

    ret = pthread_create(&g_channel_monitor->channel_thread, HI_NULL, hi_channel_host_thread, HI_NULL);
    if (ret != HI_SUCCESS) {
        goto deinit;
    }

    return HI_SUCCESS;
deinit:
    if (g_channel_monitor->skfd != -1) {
        close(g_channel_monitor->skfd);
        g_channel_monitor->skfd = -1;
    }

    if (g_channel_monitor != HI_NULL) {
        free(g_channel_monitor);
        g_channel_monitor = HI_NULL;
    }
    return HI_FAILURE;
}

hi_s32 hi_channel_deinit(hi_void)
{
    if (g_channel_monitor == HI_NULL) {
        sample_log_print("hi_channel_deinit is fail\n");
        return HI_FAILURE;
    }

    g_channel_terminate = HI_TRUE;

    if (g_channel_monitor->channel_thread) {
        pthread_cancel(g_channel_monitor->channel_thread);
        pthread_join(g_channel_monitor->channel_thread, HI_NULL);
    }

    if (g_channel_monitor->skfd != -1) {
        close(g_channel_monitor->skfd);
        g_channel_monitor->skfd = -1;
    }

    if (g_channel_monitor != HI_NULL) {
        free(g_channel_monitor);
        g_channel_monitor = HI_NULL;
    }

    return HI_SUCCESS;
}

