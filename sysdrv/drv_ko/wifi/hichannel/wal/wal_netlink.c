/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: ioctl implementatioin.
 * Author: Hisilicon
 * Create: 2020-09-28
 */
/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include <linux/netlink.h>
#include <net/sock.h>
#include "wal_netlink.h"
#include "securec.h"
#include "oal_net.h"
#include "oal_netbuf.h"
#include "hcc_host.h"
#include "hcc_comm.h"
#include "hcc_adapt.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
/*****************************************************************************
  2 枚举、结构体定义
*****************************************************************************/
typedef struct {
    struct sock *netlink_sk;
    hi_u32 user_pid;
}netlink_user_s;

static hi_void oal_recieve_user_msg(struct sk_buff *skb);
/*****************************************************************************
  3 宏定义、全局变量
*****************************************************************************/
#define NETLINK_CHANNEL_MODEID          28
#undef NLMSG_ALIGNTO
#define NLMSG_ALIGNTO                   1

static struct netlink_kernel_cfg cfg = {
    .input  = oal_recieve_user_msg,
};

static netlink_user_s g_netlink_user;
/*****************************************************************************
  4 函数实现
*****************************************************************************/
hi_s32 oal_send_user_msg(hi_u8 *pbuf, hi_u32 len)
{
    struct sk_buff *nl_skb = HI_NULL;
    struct nlmsghdr *nlh = HI_NULL;

    if (g_netlink_user.netlink_sk == HI_NULL) {
        return HI_FAILURE;
    }

    if ((pbuf == HI_NULL) || (len == 0) || (len > MAX_USER_LONG_DATA_LEN)) {
        oam_error_log1("send_usrmsg is fail:len:%d\n", len);
        return HI_FAILURE;
    }

    /* 创建sk_buff 空间 */
    nl_skb = nlmsg_new(len, GFP_ATOMIC);
    if (nl_skb == HI_NULL) {
        oam_error_log0("nlmsg_new is fail.\n");
        return HI_FAILURE;
    }

    /* 设置netlink消息头部 */
    nlh = nlmsg_put(nl_skb, 0, 0, NETLINK_CHANNEL_MODEID, len, 0);
    if (nlh == HI_NULL) {
        oam_error_log0("nlmsg_put is fail.\n");
        nlmsg_free(nl_skb);
        return HI_FAILURE;
    }

    /* 拷贝数据发送 */
    (hi_void)memcpy_s(nlmsg_data(nlh), nlmsg_len(nlh), pbuf, len);
    hi_s32 ret = netlink_unicast(g_netlink_user.netlink_sk, nl_skb, g_netlink_user.user_pid, MSG_DONTWAIT);
    if (ret == -1) {
        oam_error_log0("netlink_unicast is fail.\n");
        nlmsg_free(nl_skb);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static hi_void oal_recieve_user_msg(struct sk_buff *skb)
{
    hi_u32 payload_len;
    hi_char *umsg = HI_NULL;
    struct nlmsghdr *nlh = HI_NULL;
    oal_netbuf_stru *netbuf = HI_NULL;

    if (skb == HI_NULL) {
        return;
    }

    if (skb->len <= nlmsg_total_size(0)) {
        oam_warning_log2("oal_netlink_deinit is success\n", skb->len, nlmsg_total_size(0));
        return;
    }

    nlh = nlmsg_hdr(skb);
    umsg = NLMSG_DATA(nlh);
    payload_len = nlh->nlmsg_len - NLMSG_HDRLEN; /* header len is nlmsg_total_size */
    g_netlink_user.user_pid = nlh->nlmsg_pid;

    if (payload_len > MAX_USER_LONG_DATA_LEN) {
        oam_error_log1("payload len[%d] is fail\n", payload_len);
        return;
    }

    hcc_type_enum type = (payload_len > MAX_USER_DATA_LEN) ? HCC_TYPE_DATA : HCC_TYPE_MSG;
    netbuf = (oal_netbuf_stru *)oal_netbuf_alloc(payload_len, 0, 0);
    if (netbuf == HI_NULL) {
        oam_error_log0("oal_netbuf_alloc is fail\n");
        return;
    }

    (hi_void)memcpy_s(oal_netbuf_data(netbuf), payload_len, umsg, payload_len);
    oal_netbuf_len(netbuf) = payload_len;
    oal_netbuf_next(netbuf) = HI_NULL;
    oal_netbuf_prev(netbuf) = HI_NULL;

    if (hcc_tx_data_adapt(netbuf, type, HCC_SUB_TYPE_USER_MSG) != HI_SUCCESS) {
        oam_error_log0("hcc_tx_data_adapt is fail\n");
        oal_netbuf_free(netbuf);
    }
}

hi_s32 oal_netlink_init(hi_void)
{
    if (g_netlink_user.netlink_sk != HI_NULL) {
        oam_error_log0("oal_netlink_init is fail\n");
        return HI_FAILURE;
    }

    /* create netlink socket */
    g_netlink_user.netlink_sk = (struct sock *)netlink_kernel_create(&init_net, NETLINK_CHANNEL_MODEID, &cfg);
    if (g_netlink_user.netlink_sk == HI_NULL) {
        oam_error_log0("netlink_kernel_create is fail\n");
        return HI_FAILURE;
    }

    oam_warning_log0("oal_netlink_init is success\n");
    return HI_SUCCESS;
}

hi_s32 oal_netlink_deinit(hi_void)
{
    if (g_netlink_user.netlink_sk == HI_NULL) {
        printk("oal_netlink_deinit is fail\n");
        return HI_FAILURE;
    }

    netlink_kernel_release(g_netlink_user.netlink_sk); /* release. */
    g_netlink_user.netlink_sk = HI_NULL;
    g_netlink_user.user_pid = 0;
    oam_warning_log0("oal_netlink_deinit is success\n");
    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

