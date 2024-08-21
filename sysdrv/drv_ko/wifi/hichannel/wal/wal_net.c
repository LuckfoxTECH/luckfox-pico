/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: HMAC module HCC layer adaptation.
 * Author: kongcheng
 * Create: 2020-08-04
 */

/* 头文件包含 */
#include <linux/nl80211.h>
#include "hcc_comm.h"
#include "wal_net.h"
#include "securec.h"
#include "hcc_adapt.h"
#include "oal_netbuf.h"
#include <linux/etherdevice.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* static 函数声明 */
static oal_net_device_stats_stru* wal_netdev_get_stats(oal_net_device_stru *netdev);
static hi_s32 wal_netdev_open(oal_net_device_stru *netdev);
static hi_s32 wal_netdev_stop(oal_net_device_stru *netdev);
static hi_s32 wal_net_start_xmit(oal_netbuf_stru *netbuf, oal_net_device_stru *netdev);
static hi_s32 wal_net_device_ioctl(oal_net_device_stru *netdev, oal_ifreq_stru *ifr, hi_s32 cmd);
static hi_s32  wal_netdev_set_mac_addr(oal_net_device_stru *netdev, void *addr);
static hi_u32  wal_ioctl_get_param(oal_net_device_stru *netdev, oal_iw_request_info_stru *info, hi_void *iw,
    hi_s8 *pc_extra);
static hi_u32  wal_ioctl_set_param(oal_net_device_stru *netdev, oal_iw_request_info_stru *info, hi_void *iw,
    hi_s8 *pc_extra);

#define WAL_IOCTL_PRIV_SETPARAM             (OAL_SIOCIWFIRSTPRIV + 0)
#define WAL_IOCTL_PRIV_GETPARAM             (OAL_SIOCIWFIRSTPRIV + 1)
#define WAL_IOCTL_CMD_LEN                   512

/* 全局变量定义 */
static const oal_iw_priv_args_stru g_ast_iw_priv_args[] = {
    {WAL_IOCTL_PRIV_SETPARAM,    OAL_IW_PRIV_TYPE_CHAR | WAL_IOCTL_CMD_LEN, 0, "SET"}, /* 字符个数为512 */
    {WAL_IOCTL_PRIV_GETPARAM,    0, OAL_IW_PRIV_TYPE_CHAR | WAL_IOCTL_CMD_LEN, "GET"}, /* 字符个数为512 */
};

static const oal_iw_handler g_ast_iw_priv_handlers[] = {
    (oal_iw_handler)wal_ioctl_set_param,           /* SIOCWFIRSTPRIV+0 */  /* sub-ioctl set 入口 */
    (oal_iw_handler)wal_ioctl_get_param,           /* SIOCWFIRSTPRIV+1 */  /* sub-ioctl get 入口 */
};

oal_iw_handler_def_stru g_iw_handler_def = {
#ifdef CONFIG_WEXT_PRIV
    .private                = g_ast_iw_priv_handlers,
    .num_private            = hi_array_size(g_ast_iw_priv_handlers),
    .private_args           = g_ast_iw_priv_args,
    .num_private_args       = hi_array_size(g_ast_iw_priv_args),
#endif
    .get_wireless_stats     = HI_NULL
};

oal_net_device_ops_stru g_wal_net_dev_ops = {
    .ndo_get_stats          = wal_netdev_get_stats,
    .ndo_open               = wal_netdev_open,
    .ndo_stop               = wal_netdev_stop,
    .ndo_start_xmit         = wal_net_start_xmit,
    .ndo_do_ioctl           = wal_net_device_ioctl,
    .ndo_set_mac_address    = wal_netdev_set_mac_addr,
};

/* 函数定义 */
/*****************************************************************************
 函 数 名  : wal_netdev_get_stats
 功能描述  : 获取统计信息
*****************************************************************************/
static hi_u32  wal_ioctl_get_param(oal_net_device_stru *netdev, oal_iw_request_info_stru *info, hi_void *iw,
    hi_s8 *pc_extra)
{
    return HI_SUCCESS;
}

static hi_u32  wal_ioctl_set_param(oal_net_device_stru *netdev, oal_iw_request_info_stru *info, hi_void *iw,
    hi_s8 *pc_extra)
{
    hi_u32 ret;
    hi_char *payload = HI_NULL;
    oal_netbuf_stru *netbuf = HI_NULL;
    oal_iw_point_stru *iw_point = (oal_iw_point_stru *)iw;
    if (iw_point->length > WAL_IOCTL_CMD_LEN) {
        return HI_FAILURE;
    }

    netbuf = (oal_netbuf_stru *)oal_netbuf_alloc(iw_point->length + HCC_HDR_TOTAL_LEN, 0, 4); /* 4: 4字节对齐 */
    if (netbuf == HI_NULL) {
        oam_error_log0("oal_netbuf_alloc is error\n");
        return HI_FAILURE;
    }

    oal_netbuf_put(netbuf, iw_point->length);
    payload = oal_netbuf_data(netbuf);
    (hi_void)memcpy_s(payload, iw_point->length, pc_extra, iw_point->length);
    oal_netbuf_next(netbuf) = HI_NULL;
    oal_netbuf_prev(netbuf) = HI_NULL;

    ret = hcc_tx_data_adapt(netbuf, HCC_TYPE_MSG, HCC_SUB_TYPE_USER_MSG);
    if (ret != HI_SUCCESS) {
        oam_error_log0("hcc_tx_data_adapt is error\n");
        oal_netbuf_free(netbuf);
    }

    return HI_SUCCESS;
}

static oal_net_device_stats_stru* wal_netdev_get_stats(oal_net_device_stru *netdev)
{
    oal_net_device_stats_stru  *stats = HI_NULL;

    if (netdev == HI_NULL) {
        return HI_NULL;
    }

    stats = (oal_net_device_stats_stru *)&(netdev->stats);
    stats->rx_packets = 10; /* rx_packets 10 */
    stats->rx_bytes   = 10; /* rx_bytes 10 */
    stats->tx_packets = 10; /* tx_packets 10 */
    stats->tx_bytes   = 10; /* tx_bytes 10 */

    return stats;
}

static hi_s32 wal_netdev_open(oal_net_device_stru *netdev)
{
    if ((oal_netdevice_flags(netdev) & OAL_IFF_RUNNING) == 0) {
        oal_netdevice_flags(netdev) |= OAL_IFF_RUNNING;
    }
    return HI_SUCCESS;
}

static hi_s32  wal_netdev_stop(oal_net_device_stru *netdev)
{
    if ((oal_netdevice_flags(netdev) & OAL_IFF_RUNNING) != 0) {
        oal_netdevice_flags(netdev) &= (~OAL_IFF_RUNNING);
    }
    return HI_SUCCESS;
}

/* 注册给TCP/IP 协议栈的XMIT回调 */
static oal_net_dev_tx_enum wal_net_start_xmit(oal_netbuf_stru *netbuf, oal_net_device_stru *netdev)
{
    hi_u32 ret;

    if ((netbuf == HI_NULL) || (netdev == HI_NULL)) {
        oam_error_log2("wal_bridge_vap_xmit:: invalid params[%p][%p]", netbuf, netdev);
        return OAL_NETDEV_TX_OK;
    }

    if ((oal_netdevice_flags(netdev) & OAL_IFF_RUNNING) == 0) {
        oam_error_log0("wal_net_start_xmit:: netdev is not RUNNING");
        oal_netbuf_free(netbuf);
        return OAL_NETDEV_TX_OK;
    }

    oal_netbuf_stru *skb_buff = oal_netbuf_alloc(oal_netbuf_len(netbuf), 0, 0);
    if (skb_buff == HI_NULL) {
        oam_error_log0("***wal_net_start_xmit:: alloc_sbk failed");
        oal_netbuf_free(netbuf);
        return OAL_NETDEV_TX_OK;
    }

    memcpy_s(oal_netbuf_data(skb_buff), oal_netbuf_len(netbuf), oal_netbuf_data(netbuf), oal_netbuf_len(netbuf));
    oal_netbuf_len(skb_buff) = oal_netbuf_len(netbuf);

    oal_netbuf_free(netbuf);

    oal_netbuf_next(skb_buff) = HI_NULL;
    oal_netbuf_prev(skb_buff) = HI_NULL;

    ret = hcc_tx_data_adapt(skb_buff, HCC_TYPE_DATA, HCC_SUB_TYPE_IP_DATA);
    if (ret != HI_SUCCESS) {
        oal_netbuf_free(skb_buff);
    }

    return OAL_NETDEV_TX_OK;
}

static hi_s32 wal_net_device_ioctl(oal_net_device_stru *netdev, oal_ifreq_stru *ifr, hi_s32 cmd)
{
    hi_u32 ret = HI_SUCCESS;

    if ((netdev == HI_NULL) || (ifr == HI_NULL) || (ifr->ifr_data == HI_NULL)) {
        oam_error_log2("{wal_net_device_ioctl::pst_dev %p, pst_ifr %p!}",
                       (uintptr_t)netdev, (uintptr_t)ifr);
        return HI_FAILURE;
    }
    return ret;
}

/*****************************************************************************
 函 数 名  : wal_netdev_set_mac_addr
 功能描述  : 设置mac地址
 输入参数  : netdev: 网络设备
             addr : 地址
*****************************************************************************/
static hi_s32  wal_netdev_set_mac_addr(oal_net_device_stru *netdev, void *addr)
{
    oal_sockaddr_stru *mac_addr = HI_NULL;

    if ((netdev == HI_NULL) || (addr == HI_NULL)) {
        oam_error_log2("{wal_netdev_set_mac_addr::pst_net_dev or p_addr null ptr error %p, %p!}",
                       (uintptr_t)netdev, (uintptr_t)addr);

        return HI_FAILURE;
    }

    if (oal_netif_running(netdev)) {
        oam_warning_log0("{wal_netdev_set_mac_addr::cannot set address; device running!}");
        return HI_FAILURE;
    }

    mac_addr = (oal_sockaddr_stru *)addr;

    if (ether_is_multicast(mac_addr->sa_data)) {
        oam_warning_log0("{wal_netdev_set_mac_addr::can not set group/broadcast addr!}");
        return HI_FAILURE;
    }

    if (memcpy_s((netdev->dev_addr), WLAN_MAC_ADDR_LEN, (mac_addr->sa_data), WLAN_MAC_ADDR_LEN) != EOK) {
        oam_error_log0("{wal_netdev_set_mac_addr::mem safe function err!}");
        return HI_FAILURE;
    }

    /* 1131如果return则无法通过命令配置mac地址到寄存器 */
    /* when sta and ap are coexist,close the line "return HI_SUCCESS" DTS2016050607426 */
    /* set mac address,need open the following line,DTS2016102403276 */
    return HI_SUCCESS;
}

oal_net_device_ops_stru* wal_get_net_dev_ops(hi_void)
{
    return &g_wal_net_dev_ops;
}

oal_iw_handler_def_stru* wal_get_g_iw_handler_def(hi_void)
{
    return &g_iw_handler_def;
}

hi_u32 wal_net_register_netdev(oal_net_device_stru* netdev)
{
    if (netdev == HI_NULL) {
        return HI_FAILURE;
    }

    return register_netdev(netdev);
}

hi_s32 wal_rx_data_proc(oal_netbuf_stru *netbuf)
{
    oal_net_device_stru *netdev = oal_get_netdev_by_name("wlan0");
    if (netdev == HI_NULL) {
        oam_info_log0("wal_rx_data_proc:: netdev is NULL");
        oal_netbuf_free(netbuf);
        return HI_FAIL;
    }

    oal_dev_put(netdev);

    /* 对protocol模式赋值 */
    oal_netbuf_protocol(netbuf) = eth_type_trans(netbuf, netdev);

    return netif_rx_ni(netbuf);
}

hi_s32 wal_init_netdev(hi_u8 type, oal_net_device_stru *netdev)
{
#ifdef CONFIG_WEXT_PRIV
    netdev->wireless_handlers = wal_get_g_iw_handler_def();
#endif
    netdev->netdev_ops        = wal_get_net_dev_ops();

    oal_netdevice_destructor(netdev)      = wal_net_free_netdev;
    oal_netdevice_ifalias(netdev)         = HI_NULL;
    oal_netdevice_watchdog_timeo(netdev)  = 5; /* 固定设置为 5 */
    oal_netdevice_qdisc(netdev, HI_NULL);

    oal_netdevice_flags(netdev) &= ~OAL_IFF_RUNNING;   /* 将net device的flag设为down */

    return HI_SUCCESS;
}

hi_s32 netdev_register(hi_void)
{
    oal_net_device_stru *netdev = HI_NULL;
    hi_char dev_name[OAL_IF_NAME_SIZE] = "wlan0";

    netdev = wal_net_alloc_netdev(0, dev_name, oal_ether_setup);
    if (netdev == HI_NULL) {
        oam_error_log0("regiser_netdev:: netdev is NULL");
        return HI_FAILURE;
    }

    hi_s32 ret = wal_init_netdev(NL80211_IFTYPE_STATION, netdev);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    ret = wal_net_register_netdev(netdev);
    if (ret != HI_SUCCESS) {
        oam_warning_log1("{regiser_netdev:: wal_net_register_netdev failed[%d]}", ret);
        wal_net_free_netdev(netdev);
        return ret;
    }

    oam_warning_log0("netdev_register SUCCESSFULLY");
    return HI_SUCCESS;
}

hi_void netdev_unregister(hi_void)
{
    oal_net_device_stru *netdev = oal_get_netdev_by_name("wlan0");
    if (netdev == HI_NULL) {
        oam_error_log0("netdev_unregister:: netdev is NULL");
        return;
    }

    oal_dev_put(netdev);
    oal_netdevice_flags(netdev) &= ~OAL_IFF_RUNNING;
    oal_net_unregister_netdev(netdev);
}

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif
