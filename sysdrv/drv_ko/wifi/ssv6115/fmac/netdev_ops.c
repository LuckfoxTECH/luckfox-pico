/*
 * Copyright (c) 2021 iComm-semi Ltd.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/**
 * @file netdev_ops.c
 * @brief Net device ops functions.
 */


/*******************************************************************************
 *         Include Files
 ******************************************************************************/
#include <linux/version.h>
#include <linux/if_arp.h>
#include <linux/inetdevice.h>
#include <linux/etherdevice.h>

#include "fmac.h"
#include "hci/drv_hci_ops.h"
#include "lmac_msg.h"
#include "netdev_ops.h"
#include "fmac_tx.h"
#include "fmac_defs.h"
#include "fmac_msg_tx.h"
#include "ssvdevice/rftool/ssv_phy_rf.h"
#include "ssv_cfg.h"
#include "ssv_debug.h"

#ifdef FMAC_BRIDGE
#include "fmac_bridge.h"
#endif

/*******************************************************************************
 *         Local Defines
 ******************************************************************************/
static int ssv_open(struct net_device *dev);
static int ssv_close(struct net_device *dev);
static struct net_device_stats *ssv_get_stats(struct net_device *dev);
#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 11, 0))
static struct rtnl_link_stats64 *ssv_get_stats64(struct net_device *dev, struct rtnl_link_stats64 *netstats);
#else
static void ssv_get_stats64(struct net_device *dev, struct rtnl_link_stats64 *netstats);
#endif
static int ssv_set_mac_address(struct net_device *dev, void *addr);
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 6, 0))
static int ssv_netdev_notifier_call(struct notifier_block *nb, unsigned long state, void *ndev);
#endif


/*******************************************************************************
 *         Local Enumerations
 ******************************************************************************/


/*******************************************************************************
 *         Local Structures
 ******************************************************************************/


/*******************************************************************************
 *         Global Variables
 ******************************************************************************/
extern struct ssv6xxx_cfg ssv_cfg;

/*******************************************************************************
 *         Local Variables
 ******************************************************************************/
static const struct net_device_ops ssv_netdev_ops = {
    .ndo_open               = ssv_open,
    .ndo_stop               = ssv_close,
    .ndo_start_xmit         = ssv_start_xmit,
    .ndo_get_stats          = ssv_get_stats,
    .ndo_get_stats64        = ssv_get_stats64,
    .ndo_set_mac_address    = ssv_set_mac_address
};

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 6, 0))
static struct notifier_block netdev_notifier = {
    .notifier_call          = ssv_netdev_notifier_call,
};
#endif

/*******************************************************************************
 *         Local Functions
 ******************************************************************************/
/*******************************************************************************
 *         netdev callbacks
 ******************************************************************************/
/**
 * int (*ndo_open)(struct net_device *dev);
 *     This function is called when network device transistions to the up
 *     state.
 *
 * - Start FW if this is the first interface opened
 * - Add interface at fw level
 */
static int ssv_open(struct net_device *dev)
{
    struct ssv_vif *ssv_vif = netdev_priv(dev);
    struct ssv_softc *sc = ssv_vif->sc;
    struct mm_add_if_cfm add_if_cfm;
    int error = 0;
    static bool bInit = false;

    // SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);

    // Check if it is the first opened VIF
    if ((sc->vif_started == 0) && (false == bInit))
    {     
        sc->recovery_flag = true;
        // Start the FW
        if ((error = ssv_send_start(sc)))
        {
            return error;
        }
        sc->recovery_flag = false;
        /* Device is now started */
        if (SSV_DEV_STARTED < (sizeof(sc->drv_flags) * 8)) {
            set_bit(SSV_DEV_STARTED, &sc->drv_flags);
        } else {
            BUG_ON(1);
        }
        bInit = true;
	ssv_vif->tx_total_byte = 0;
	ssv_vif->tx_total_cnt = 0;
	ssv_vif->rx_total_byte = 0;
	ssv_vif->rx_total_cnt = 0;
    }

 REOPEN_VIF:   
    if (SSV_VIF_TYPE(ssv_vif) == NL80211_IFTYPE_AP_VLAN) {
        /* For AP_vlan use same fw and drv indexes. We ensure that this index
           will not be used by fw for another vif by taking index >= NX_VIRT_DEV_MAX */
        add_if_cfm.inst_nbr = ssv_vif->drv_vif_index;
        netif_tx_stop_all_queues(dev);
    } else {
        /* Forward the information to the LMAC,
         *     p2p value not used in FMAC configuration, iftype is sufficient */
        
        sc->recovery_flag = true;
        if (false == ssv_vif->use_monitor) 
        {
            if ((error = ssv_send_add_if(sc, dev->dev_addr,
                                      SSV_VIF_TYPE(ssv_vif), false, &add_if_cfm)))
            {
                return error;
            }

            if (add_if_cfm.status != 0){
                SSV_LOG_DBG("add if CFM error\n");
                return -EIO;
            }
        }
    }

    //When recive vif_idx(firmware return) is not match the driver vif index, reopen interface again. 
     if (false == ssv_vif->use_monitor)
     {
        if(ssv_vif->drv_vif_index != add_if_cfm.inst_nbr)
        {
            SSV_LOG_DBG("drv and fw vif idx not match (drv %d:fw %d), reopen\n", ssv_vif->drv_vif_index, add_if_cfm.inst_nbr);
            if((error = ssv_send_remove_if(sc, add_if_cfm.inst_nbr)))
            {
                SSV_LOG_DBG("remove interface failed\n");
                return error;
            }
            // SSV_LOG_DBG("goto Reopen VIF\n");
            goto REOPEN_VIF;
        }
     }

    sc->recovery_flag = false;

#ifdef FMAC_BRIDGE
    br0_netdev_open(dev);
#endif

    mutex_lock(&sc->cb_lock);
    /* Save the index retrieved from LMAC */
    //ssv_vif->drv_vif_index = add_if_cfm.inst_nbr;    
    ssv_vif->up = true;
    sc->vif_started++;
    sc->vif_table[add_if_cfm.inst_nbr] = ssv_vif;

    if ((!dev->dev_addr) ||(!memcmp(&sc->maddr[0], dev->dev_addr, ETH_ALEN)) ) {
        sc->if_mode[0][0] = SSV_VIF_TYPE(ssv_vif); //2
        sc->if_mode[0][1] = ssv_vif->drv_vif_index;
        SSV_LOG_DBG("if mode 0 add,dev->dev_addr[%pM]\n",dev->dev_addr); //14:b2:e5:ff:a9:fc
    } else if ((!dev->dev_addr) ||(!memcmp(&sc->maddr[1], dev->dev_addr, ETH_ALEN)) ) {
        sc->if_mode[1][0] = SSV_VIF_TYPE(ssv_vif); //3
        sc->if_mode[1][1] = ssv_vif->drv_vif_index; 
        SSV_LOG_DBG("if mode 1 add,dev->dev_addr[%pM]\n",dev->dev_addr); //14:b2:e5:ff:a9:fd
    } else if ((!dev->dev_addr) ||(ssv_vif->drv_vif_index == 1)) {
        memcpy(&sc->maddr[1], dev->dev_addr, ETH_ALEN);
        sc->if_mode[1][0] = SSV_VIF_TYPE(ssv_vif);
        sc->if_mode[1][1] = ssv_vif->drv_vif_index;
        SSV_LOG_DBG("if mode 2 add, dev->dev_addr[%pM]\n",dev->dev_addr);
    } else {
        SSV_LOG_DBG("===== mac address need check =====\n");
    }
    mutex_unlock(&sc->cb_lock);

    SSV_LOG_DBG("add_if_cfm: drv_vif_index %d\n", ssv_vif->drv_vif_index);
    // SSV_LOG_DBG("add_if_cfm: drv_vif_index %d vif_idx = %d\n", ssv_vif->drv_vif_index, ssv_vif->drv_vif_index);

    ssv_drv_hci_start(sc->hci_priv, sc->hci_ops); //start all hci wifi sw txq
    if (ssv_cfg.cca) 
        ssv_drv_hci_write_word(sc->hci_priv,sc->hci_ops,0x0810b3ac,0x1b5d33d6);
    
    netif_tx_stop_all_queues(dev);
    netif_carrier_off(dev);

    return error;
}

/**
 * int (*ndo_stop)(struct net_device *dev);
 *     This function is called when network device transistions to the down
 *     state.
 *
 * - Remove interface at fw level
 * - Reset FW if this is the last interface opened
 */
static int ssv_close(struct net_device *dev)
{
    struct ssv_vif *ssv_vif = netdev_priv(dev);
    struct ssv_softc *sc = ssv_vif->sc;
    u8 ch_num=0;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 8, 0))
	struct cfg80211_scan_info info = {
    	.aborted = true,
	};
#endif

    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);

    while((sc->scan_request) && 
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0))
          (sc->scan_request->wdev == &ssv_vif->wdev)
#else
          (sc->scan_request->dev == ssv_vif->wdev.netdev)
#endif
         ) {
        //home channel + off channel + 50ms
        msleep(ssv_cfg.scan_period*2+50);
        ch_num++;
        if(ch_num==14) break;
    }

    /* Abort scan request on the vif */
    if (sc->scan_request &&
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0))
        sc->scan_request->wdev == &ssv_vif->wdev
#else
        sc->scan_request->dev == ssv_vif->wdev.netdev
#endif
       ) {

#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 8, 0))
        cfg80211_scan_done(sc->scan_request, true);
#else
        cfg80211_scan_done(sc->scan_request, &info);
#endif
        sc->scan_request = NULL;
    }

    if (false == ssv_vif->use_monitor) 
        ssv_send_remove_if(sc, ssv_vif->drv_vif_index);

    if (sc->roc_elem && (sc->roc_elem->wdev == &ssv_vif->wdev)) {
        kfree(sc->roc_elem);
        sc->roc_elem = NULL;
    }

#ifdef FMAC_BRIDGE
        ssv_bridge_flush(ssv_vif);
#endif

    /* Ensure that we won't process disconnect ind */
    mutex_lock(&sc->cb_lock);

    ssv_vif->up = false;
    if (netif_carrier_ok(dev)) {
        if (SSV_VIF_TYPE(ssv_vif) == NL80211_IFTYPE_STATION ||
            SSV_VIF_TYPE(ssv_vif) == NL80211_IFTYPE_P2P_CLIENT) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 2, 0)
            cfg80211_disconnected(dev, WLAN_REASON_DEAUTH_LEAVING,
                                  NULL, 0, true, GFP_ATOMIC);
#else
            cfg80211_disconnected(dev, WLAN_REASON_DEAUTH_LEAVING,
                                  NULL, 0, GFP_ATOMIC);
#endif
            
            netif_tx_stop_all_queues(dev);
            netif_carrier_off(dev);
        } else if (SSV_VIF_TYPE(ssv_vif) == NL80211_IFTYPE_AP_VLAN) {
            netif_carrier_off(dev);
        } else {
            netdev_warn(dev, "AP not stopped when disabling interface");
        }
    }

    mutex_unlock(&sc->cb_lock);

    /* For station, cfg80211_disconnect will execute ssv_send_disconnect_req()
     * Host driver receive "ssv_rx_sm_disconnect_ind" message to pause hci txq.
     *
     * It only take care that it should pause hci txq when softap shutdown.
     */
    if ((SSV_VIF_TYPE(ssv_vif) == NL80211_IFTYPE_AP) || (SSV_VIF_TYPE(ssv_vif) == NL80211_IFTYPE_AP_VLAN))
    {
        struct ssv_sta *cur, *tmp;

        list_for_each_entry_safe(cur, tmp, &ssv_vif->ap.sta_list, list) {
            if (true == cur->valid) {
                ssv_drv_hci_tx_pause_by_sta(sc->hci_priv, sc->hci_ops, cur->sta_idx);
            }
        }
    }
    sc->vif_table[ssv_vif->drv_vif_index] = NULL;
    sc->vif_started--;

    if (SSV_VIF_TYPE(ssv_vif) == NL80211_IFTYPE_MONITOR) {
        ssv_chanctx_unlink(ssv_vif);
    }

#if 0
    if (sc->vif_started == 0) {
        ssv_send_reset(sc);

        // Set parameters to firmware
        ssv_send_me_config_req(sc);

        // Set channel parameters to firmware
        ssv_send_me_chan_config_req(sc);

        if (SSV_DEV_STARTED < (sizeof(sc->drv_flags) * 8)) {
            clear_bit(SSV_DEV_STARTED, &sc->drv_flags);
        } else {
            BUG_ON(1);
        }
    }
#endif

    return 0;
}

/**
 * struct net_device_stats* (*ndo_get_stats)(struct net_device *dev);
 *	Called when a user wants to get the network device usage
 *	statistics. Drivers must do one of the following:
 *	1. Define @ndo_get_stats64 to fill in a zero-initialised
 *	   rtnl_link_stats64 structure passed by the caller.
 *	2. Define @ndo_get_stats to update a net_device_stats structure
 *	   (which should normally be dev->stats) and return a pointer to
 *	   it. The structure may be changed asynchronously only if each
 *	   field is written atomically.
 *	3. Update dev->stats asynchronously and atomically, and define
 *	   neither operation.
 */

static struct net_device_stats *ssv_get_stats(struct net_device *dev)
{
    struct ssv_vif *vif = netdev_priv(dev);

    return &vif->net_stats;
}
#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 11, 0))
static struct rtnl_link_stats64 *ssv_get_stats64(struct net_device *dev, struct rtnl_link_stats64 *netstats)
#else
static void ssv_get_stats64(struct net_device *dev, struct rtnl_link_stats64 *netstats)
#endif
{
	struct ssv_vif *vif = netdev_priv(dev);
	netstats->rx_packets = (u64)vif->rx_total_cnt;
	netstats->rx_bytes = (u64)vif->rx_total_byte;
	netstats->tx_packets = (u64)vif->tx_total_cnt;
	netstats->tx_bytes = (u64)vif->tx_total_byte;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 11, 0))
	return netstats;
#endif
}

/**
 * int (*ndo_set_mac_address)(struct net_device *dev, void *addr);
 *	This function  is called when the Media Access Control address
 *	needs to be changed. If this interface is not defined, the
 *	mac address can not be changed.
 */
static int ssv_set_mac_address(struct net_device *dev, void *addr)
{
    struct sockaddr *sa = addr;
    int ret;

    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);

    ret = eth_mac_addr(dev, sa);

    return ret;
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 6, 0))
static inline void wdev_lock(struct wireless_dev *wdev)
    __acquires(wdev)
{
    mutex_lock(&wdev->mtx);
    __acquire(wdev->mtx);
}
 
static inline void wdev_unlock(struct wireless_dev *wdev)
    __releases(wdev)
{
    __release(wdev->mtx);
    mutex_unlock(&wdev->mtx);
}

extern int ssv_stop_ap(struct wiphy *wiphy, struct net_device *dev);
static int ssv_netdev_notifier_call(struct notifier_block * nb, unsigned long state, void *ndev)
{
    struct net_device *dev = ndev;
    struct wireless_dev *wdev = NULL;
    struct wiphy *wiphy = NULL;
    int err;

    if (!dev || !dev->ieee80211_ptr)
        return NOTIFY_DONE;

    wdev = dev->ieee80211_ptr;
    wiphy = wdev->wiphy;
    if (!wiphy || !wiphy_priv(wiphy))
        return NOTIFY_DONE;

    /* We point wiphy->privid to sc in function ssv6xxx_fmac_init(), 
     * so we use it to check whether we own this wiphy or not here. */
    if (wiphy_priv(wiphy) == wiphy->privid) {
        if ((state == NETDEV_GOING_DOWN) && (wdev->iftype == NL80211_IFTYPE_AP)) {
            SSV_LOG_DBG("netdev is going down, stop ap!!\n");
            err = ssv_stop_ap(wiphy, dev);
            if (!err) {
                wdev_lock(wdev);
                wdev->beacon_interval = 0;
                wdev->channel = NULL;
                wdev_unlock(wdev);
            }
        }
    }

    return NOTIFY_DONE;
}
#endif


/*******************************************************************************
 *         Global Functions
 ******************************************************************************/
struct wireless_dev *ssv_interface_add(struct ssv_softc *sc,
                                               const char *name,
                                               enum nl80211_iftype type,
                                               struct vif_params *params)
{
    struct net_device *ndev;
    struct ssv_vif *vif;
    int min_idx, max_idx;
    int vif_idx = -1;
    int i;

	SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);
	
    // Look for an available VIF
    if (type == NL80211_IFTYPE_AP_VLAN) {
        min_idx = NX_VIRT_DEV_MAX;
        max_idx = NX_VIRT_DEV_MAX + NX_REMOTE_STA_MAX;
    } else {
        min_idx = 0;
        max_idx = NX_VIRT_DEV_MAX;
    }

    for (i = min_idx; i < max_idx; i++) {
        if ((sc->avail_idx_map) & BIT(i)) {
            vif_idx = i;
            break;
        }
    }

    if (vif_idx < 0)
        return NULL;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 17, 0))
    ndev = alloc_netdev(sizeof(*vif), name, NET_NAME_UNKNOWN, ssv_netdev_setup);
#else
    ndev = alloc_netdev(sizeof(*vif), name, ssv_netdev_setup);
#endif
    if (!ndev)
        return NULL;

    vif = netdev_priv(ndev);
    ndev->ieee80211_ptr = &vif->wdev;
    vif->wdev.wiphy = sc->wiphy;
    vif->sc = sc;
    vif->ndev = ndev;
    vif->drv_vif_index = vif_idx;
    //vif->drv_vif_index = vif_idx;
    SET_NETDEV_DEV(ndev, wiphy_dev(vif->wdev.wiphy));
    vif->wdev.netdev = ndev;
    vif->wdev.iftype = type;
    vif->up = false;
    vif->ch_index = SSV_CH_NOT_SET;
    memset(&vif->net_stats, 0, sizeof(vif->net_stats));

    ndev->type = ARPHRD_ETHER;
    switch (type) {
    case NL80211_IFTYPE_STATION:
    case NL80211_IFTYPE_P2P_CLIENT:
        vif->sta.ap = NULL;
        vif->sta.tdls_sta = NULL;
        break;
    case NL80211_IFTYPE_MESH_POINT:
        INIT_LIST_HEAD(&vif->ap.mpath_list);
        INIT_LIST_HEAD(&vif->ap.proxy_list);
        vif->ap.create_path = false;
        vif->ap.generation = 0;
        INIT_LIST_HEAD(&vif->ap.sta_list);
        memset(&vif->ap.bcn, 0, sizeof(vif->ap.bcn));        
	break;
    case NL80211_IFTYPE_AP:
    case NL80211_IFTYPE_P2P_GO:
        INIT_LIST_HEAD(&vif->ap.sta_list);
        memset(&vif->ap.bcn, 0, sizeof(vif->ap.bcn));
        break;
    case NL80211_IFTYPE_AP_VLAN:
    {
        struct ssv_vif *master_vif;
        bool found = false;
        list_for_each_entry(master_vif, &sc->vifs, list) {
            if ((SSV_VIF_TYPE(master_vif) == NL80211_IFTYPE_AP)
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0))
                && !(!memcmp(master_vif->ndev->dev_addr, params->macaddr,
                           ETH_ALEN))
#endif
               ) {
                 found=true;
                 break;
            }
        }

        if (!found)
            goto err;

         vif->ap_vlan.master = master_vif;
         vif->ap_vlan.sta_4a = NULL;
         break;
    }
    case NL80211_IFTYPE_MONITOR:
    {
        ndev->type = ARPHRD_IEEE80211_RADIOTAP;
        // ndev->type = ARPHRD_IEEE80211;
        break;
    }
    default:
        break;
    }

    // SSV_LOG_DBG("[%s][%d] ndev->type = %u\n", __FUNCTION__, __LINE__, ndev->type);

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0))
    if (type == NL80211_IFTYPE_AP_VLAN)
        memcpy(ndev->dev_addr, params->macaddr, ETH_ALEN);
    else {
#endif
        // sc->wphy->perm_addr == sc->macaddr[0][0]
        if (0 == vif->drv_vif_index)
            memcpy(ndev->dev_addr, sc->wiphy->perm_addr, ETH_ALEN);
        else
            memcpy(ndev->dev_addr, (const void *)&sc->maddr[1][0], ETH_ALEN);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0))
    }
#endif

    if (params) {
        vif->use_4addr = params->use_4addr;
        ndev->ieee80211_ptr->use_4addr = params->use_4addr;
    } else
        vif->use_4addr = false;

    if (register_netdevice(ndev))
        goto err;

#ifdef FMAC_BRIDGE
    br0_attach(vif);
#endif

    mutex_lock(&sc->cb_lock);
    list_add_tail(&vif->list, &sc->vifs);
    mutex_unlock(&sc->cb_lock);
    sc->avail_idx_map &= ~BIT(vif_idx);

    return &vif->wdev;

err:
    free_netdev(ndev);
    return NULL;
}

void ssv_netdev_setup(struct net_device *dev)
{
    ether_setup(dev);
    dev->priv_flags &= ~IFF_TX_SKB_SHARING;
    dev->netdev_ops = &ssv_netdev_ops;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
    dev->needs_free_netdev = true;
#else
    dev->destructor = free_netdev;
#endif
    dev->watchdog_timeo = SSV_TX_LIFETIME_MS;

    dev->needed_headroom = SSV_TX_HDR_SIZE;

    SSV_LOG_DBG("headroom = %d\n", dev->needed_headroom);
    dev->hw_features = 0;
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 6, 0))
int ssv_netdev_notifier_register(void)
{
    int err;

    err = register_netdevice_notifier(&netdev_notifier);

    return err;
}

void ssv_netdev_notifier_unregister(void)
{
    unregister_netdevice_notifier(&netdev_notifier);
}
#endif

