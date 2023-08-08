/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Header file for oal_net.c.
 * Author: Hisilicon
 * Create: 2020-08-04
 */

#ifndef __OAL_NET_H__
#define __OAL_NET_H__

/* 头文件包含 */
#include <linux/netdevice.h>
#include <linux/kernel.h>
#include <net/iw_handler.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include "hi_types_base.h"
#include "oal_netbuf.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define OAL_SIOCIWFIRSTPRIV         0x8BE0
/* iw_priv参数类型OAL封装 */
#define OAL_IW_PRIV_TYPE_BYTE       0x1000       /* Char as number */
#define OAL_IW_PRIV_TYPE_CHAR       0x2000       /* Char as character */
#define OAL_IW_PRIV_TYPE_INT        0x4000       /* 32 bits integer */
#define OAL_IW_PRIV_TYPE_ADDR       0x6000       /* struct sockaddr */
#define OAL_IW_PRIV_SIZE_FIXED      0x0800       /* Variable or fixed number of args */
/* 宏定义 */
#define OAL_IF_NAME_SIZE            16
#define oal_netif_running(net_dev)  netif_running(net_dev)

//#define oal_netdevice_destructor(_pst_dev) ((_pst_dev)->destructor)
#define oal_netdevice_destructor(_pst_dev) ((_pst_dev)->priv_destructor)
#define oal_netdevice_qdisc(_pst_dev, pst_val) ((_pst_dev)->qdisc = (pst_val))
#define oal_netdevice_ifalias(_pst_dev) ((_pst_dev)->ifalias)
#define oal_netdevice_watchdog_timeo(_pst_dev) ((_pst_dev)->watchdog_timeo)
#define oal_netdevice_flags(_pst_dev) ((_pst_dev)->flags)
#define oal_netif_running(net_dev) netif_running(net_dev)
#define oal_dev_put(_pst_dev)  dev_put(_pst_dev)

#define OAL_IFF_RUNNING IFF_RUNNING

#define OAL_NETDEV_TX_OK     NETDEV_TX_OK
#define OAL_NETDEV_TX_BUSY   NETDEV_TX_BUSY
#define OAL_NETDEV_TX_LOCKED NETDEV_TX_LOCKED

#define OAL_INIT_NET                init_net
#define NETLINK_DEV_ERROR                 27

#ifndef GFP_ATOMIC
#define GFP_ATOMIC                          0
#endif

typedef struct net_device           oal_net_device_stru;
typedef struct net_device_ops       oal_net_device_ops_stru;
typedef struct net_device_stats     oal_net_device_stats_stru;
typedef struct ifreq                oal_ifreq_stru;
typedef struct iw_handler_def       oal_iw_handler_def_stru;
typedef struct iw_priv_args         oal_iw_priv_args_stru;
typedef iw_handler                  oal_iw_handler;
typedef netdev_tx_t                 oal_net_dev_tx_enum;
typedef struct sock              oal_sock_stru;

/* ENUM定义 */
typedef enum _wlan_net_queue_type_ {
    WLAN_HI_QUEUE = 0,
    WLAN_NORMAL_QUEUE,

    WLAN_TCP_DATA_QUEUE,
    WLAN_TCP_ACK_QUEUE,

    WLAN_UDP_BK_QUEUE,
    WLAN_UDP_BE_QUEUE,
    WLAN_UDP_VI_QUEUE,
    WLAN_UDP_VO_QUEUE,

    WLAN_NET_QUEUE_BUTT
} wlan_net_queue_type;
/* iw_handler_def结构体封装 */
typedef struct {
    hi_u16       cmd;        /* Wireless Extension command */
    hi_u16       flags;      /* More to come ;-) */
} oal_iw_request_info_stru;

typedef struct {
    hi_void*  pointer;        /* Pointer to the data  (in user space) */
    hi_u16    length;         /* number of fields or size in bytes */
    hi_u16    flags;          /* Optional params */
} oal_iw_point_stru;

/* 结构体定义 */
typedef struct {
    hi_u16       sa_family;      /* address family, AF_xxx   */
    hi_u8        sa_data[14];    /* 14 bytes of protocol address */
} oal_sockaddr_stru;

/* inline 函数定义 */
static inline oal_sock_stru* oal_netlink_kernel_create(hi_void (*input)(oal_netbuf_stru *pst_netbuf))
{
    struct netlink_kernel_cfg cfg;

    memset_s(&cfg, sizeof(cfg), 0, sizeof(cfg));
    cfg.groups = 0;
    cfg.input = input;
    cfg.cb_mutex = NULL;

    return netlink_kernel_create(&init_net, NETLINK_DEV_ERROR, &cfg);
}

static inline hi_void oal_ether_setup(oal_net_device_stru *net_device)
{
    if (net_device == HI_NULL) {
        return;
    }

    ether_setup(net_device);
    return;
}

static inline oal_net_device_stru* oal_get_netdev_by_name(const hi_char* pc_name)
{
    return dev_get_by_name(&init_net, pc_name);
}

static inline hi_void oal_net_unregister_netdev(oal_net_device_stru* netdev)
{
    if (netdev == HI_NULL) {
        return;
    }
    unregister_netdev(netdev);
}

static inline hi_void oal_netif_stop_queue(oal_net_device_stru* netdev)
{
    if (netdev == HI_NULL) {
        return;
    }
    netif_stop_queue(netdev);
}

static inline hi_void oal_netif_wake_queue(oal_net_device_stru* netdev)
{
    if (netdev == HI_NULL) {
        return;
    }
    netif_wake_queue(netdev);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of oal_net.h */

