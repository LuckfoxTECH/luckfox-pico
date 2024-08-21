/*
 * Interface handling (except master interface)
 *
 * Copyright 2002-2005, Instant802 Networks, Inc.
 * Copyright 2005-2006, Devicescape Software, Inc.
 * Copyright (c) 2006 Jiri Benc <jbenc@suse.cz>
 * Copyright 2008, Johannes Berg <johannes@sipsolutions.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/if_arp.h>
#include <linux/netdevice.h>
#include <linux/rtnetlink.h>
#include <net/atbm_mac80211.h>
#include <net/ieee80211_radiotap.h>
#ifdef CONFIG_WIRELESS_EXT 
#include <net/iw_handler.h>
#endif
#include "ieee80211_i.h"
#include "sta_info.h"
#include "debugfs_netdev.h"
#include "mesh.h"
#include "led.h"
#include "driver-ops.h"
#include "wme.h"
#include "rate.h"

#ifdef CHKSUM_HW_SUPPORT
#pragma message("TCP/IP Hw Check Sum Enbale")
#endif
#ifdef CONFIG_ATBM_STA_LISTEN
static void ieee80211_sdata_listen_recalc(struct ieee80211_local *local,bool start,bool flush);
#endif
/**
 * DOC: Interface list locking
 *
 * The interface list in each struct ieee80211_local is protected
 * three-fold:
 *
 * (1) modifications may only be done under the RTNL
 * (2) modifications and readers are protected against each other by
 *     the iflist_mtx.
 * (3) modifications are done in an RCU manner so atomic readers
 *     can traverse the list in RCU-safe blocks.
 *
 * As a consequence, reads (traversals) of the list can be protected
 * by either the RTNL, the iflist_mtx or RCU.
 */
static void ieee80211_medium_traffic_timer(unsigned long data)
{
	struct ieee80211_sub_if_data *sdata = (struct ieee80211_sub_if_data *)data;
	unsigned long current_tx_bytes;
	unsigned long current_rx_bytes;

	if(ieee80211_sdata_running(sdata) == 0){
		return;
	}

	current_tx_bytes = sdata->dev->stats.tx_bytes - sdata->traffic.last_tx_bytes;
	current_rx_bytes = sdata->dev->stats.rx_bytes - sdata->traffic.last_rx_bytes;
	sdata->traffic.last_tx_bytes = sdata->dev->stats.tx_bytes;
	sdata->traffic.last_rx_bytes = sdata->dev->stats.rx_bytes;
	sdata->traffic.current_tx_tp = current_tx_bytes*8;
	sdata->traffic.current_rx_tp = current_rx_bytes*8;
	atbm_printk_debug("%s:tx_tp(%ld),rx_tp(%ld)\n",sdata->name,sdata->traffic.current_tx_tp,sdata->traffic.current_rx_tp);
	atbm_mod_timer(&sdata->traffic.traffic_timer,jiffies + msecs_to_jiffies(IEEE80211_MEDIUM_TRAFFIC_PRIOD));
#ifdef CONFIG_ATBM_STA_DYNAMIC_PS
	rcu_read_lock();
	if( sdata->vif.type == NL80211_IFTYPE_STATION && sdata->u.mgd.associated){
		
		u32 need_ps    = !!(sdata->traffic.current_tx_tp + sdata->traffic.current_rx_tp < IEEE80211_KEEP_WAKEUP_TP_PER_SECOND);
		u32 current_ps = !!(sdata->ps_allowed == true && sdata->vif.bss_conf.ps_enabled == true);
		atbm_printk_debug("%s:need_ps(%d),current_ps(%d),ps_allowed(%d),ps_enabled(%d)\n",__func__,need_ps,current_ps,
			sdata->ps_allowed ,sdata->vif.bss_conf.ps_enabled);
		if(need_ps != current_ps){
			struct sk_buff *skb = atbm_dev_alloc_skb(0);
			
			if(skb){
				skb->pkt_type = IEEE80211_SDATA_IDLE_RECAL;
				atbm_skb_queue_tail(&sdata->skb_queue, skb);
				ieee80211_queue_work(&sdata->local->hw, &sdata->work);
			}
		}
	}
	rcu_read_unlock();
#endif
}
void ieee80211_medium_traffic_start(struct ieee80211_sub_if_data *sdata)
{
	sdata->traffic.current_rx_tp = 0;
	sdata->traffic.current_tx_tp = 0;
	sdata->traffic.last_tx_bytes = sdata->dev->stats.tx_bytes;
	sdata->traffic.last_rx_bytes = sdata->dev->stats.rx_bytes;
	atbm_mod_timer(&sdata->traffic.traffic_timer,jiffies + msecs_to_jiffies(IEEE80211_MEDIUM_TRAFFIC_PRIOD));
}

void ieee80211_medium_traffic_concle(struct ieee80211_sub_if_data *sdata)
{
	atbm_del_timer_sync(&sdata->traffic.traffic_timer);
	sdata->traffic.current_rx_tp = 0;
	sdata->traffic.current_tx_tp = 0;
	sdata->traffic.last_tx_bytes = sdata->dev->stats.tx_bytes;
	sdata->traffic.last_rx_bytes = sdata->dev->stats.rx_bytes;
}

void ieee80211_medium_traffic_init(struct ieee80211_sub_if_data *sdata)
{
	sdata->traffic.current_rx_tp = 0;
	sdata->traffic.current_tx_tp = 0;
	sdata->traffic.last_tx_bytes = sdata->dev->stats.tx_bytes;
	sdata->traffic.last_rx_bytes = sdata->dev->stats.rx_bytes;
	atbm_setup_timer(&sdata->traffic.traffic_timer,ieee80211_medium_traffic_timer,(unsigned long) sdata);
}

static int ieee80211_change_mtu(struct net_device *dev, int new_mtu)
{
	int meshhdrlen;
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	meshhdrlen = (sdata->vif.type == NL80211_IFTYPE_MESH_POINT) ? 5 : 0;

	/* FIX: what would be proper limits for MTU?
	 * This interface uses 802.3 frames. */
	if (new_mtu < 256 ||
	    new_mtu > IEEE80211_MAX_DATA_LEN - 24 - 6 - meshhdrlen) {
		return -EINVAL;
	}

#ifdef CONFIG_MAC80211_ATBM_VERBOSE_DEBUG
	atbm_printk_debug( "%s: setting MTU %d\n", dev->name, new_mtu);
#endif /* CONFIG_MAC80211_ATBM_VERBOSE_DEBUG */
	dev->mtu = new_mtu;
	return 0;
}
#ifdef CONFIG_ATBM_SUPPORT_REALTIME_CHANGE_MAC
static int ieee80211_change_mac(struct net_device *dev, void *addr)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct sockaddr *sa = addr;
	int ret;
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_hw *hw = NULL;
	u8 i = 0;
	int found = -1;
	struct ieee80211_sub_if_data *other_sdata = NULL;
	
	if(local->open_count){
		return -EBUSY;
	}
	
	if (ieee80211_sdata_running(sdata))
		return -EBUSY;
	
	hw = &local->hw;

	for(i = 0;i < hw->wiphy->n_addresses;i++){
		if(memcmp(sdata->vif.addr,hw->wiphy->addresses[i].addr,ETH_ALEN))
			continue;
		found = i;
		break;
	}

	if(found != 0){
		atbm_printk_err("[%s] do not support change mac\n",sdata->name);
		return -EBUSY;
	}
	list_for_each_entry(other_sdata, &local->interfaces, list) {
		if (memcmp(local->hw.wiphy->addresses[1].addr,
			   other_sdata->vif.addr, ETH_ALEN) == 0) {
			struct sockaddr other_sa;
			memcpy(&other_sa,sa,sizeof(struct sockaddr));
			memcpy(other_sa.sa_data,sa->sa_data,ETH_ALEN);
#ifdef ATBM_P2P_ADDR_USE_LOCAL_BIT
			other_sa.sa_data[0] ^= BIT(1);
#else
			other_sa.sa_data[5] += 1;
#endif		
			if(eth_mac_addr(other_sdata->dev,&other_sa) == 0){
				memcpy(other_sdata->vif.addr,other_sa.sa_data, ETH_ALEN);
				call_netdevice_notifiers(NETDEV_CHANGEADDR, other_sdata->dev);
			}
			atbm_printk_always("[%s] change mac[%pM]\n",other_sdata->name,other_sa.sa_data);
			break;
		}

	}
	/*
	list_for_each_entry(other_sdata, &local->interfaces, list) {
		if (memcmp(local->hw.wiphy->addresses[1].addr,
			   sdata->vif.addr, ETH_ALEN) == 0) {
			struct sockaddr other_sa;
			memcpy(&other_sa,sa,sizeof(struct sockaddr));
			memcpy(sa->sa_data,hw->wiphy->addresses[1].addr,ETH_ALEN);
			if(eth_mac_addr(other_sdata->dev,&other_sa) == 0){
				memcpy(other_sdata->vif.addr,hw->wiphy->addresses[1].addr, ETH_ALEN);
				call_netdevice_notifiers(NETDEV_CHANGEADDR, other_sdata->dev);
			}
		}
	}
	*/
	ret = eth_mac_addr(dev, sa);
	
	if (ret == 0){
		memcpy(sdata->vif.addr, sa->sa_data, ETH_ALEN);
		WARN_ON(hw->wiphy->n_addresses < 2);
		memcpy(hw->wiphy->addresses[0].addr,sdata->vif.addr,ETH_ALEN);
		memcpy(hw->wiphy->addresses[1].addr,sdata->vif.addr,ETH_ALEN);
#ifdef ATBM_P2P_ADDR_USE_LOCAL_BIT
		hw->wiphy->addresses[1].addr[0] ^= BIT(1);
#else
		hw->wiphy->addresses[1].addr[5] += 1;
#endif
		SET_IEEE80211_PERM_ADDR(hw, hw->wiphy->addresses[0].addr);
	}
	return ret;
}
#endif
static inline int identical_mac_addr_allowed(int type1, int type2)
{
	return type1 == NL80211_IFTYPE_MONITOR ||
		type2 == NL80211_IFTYPE_MONITOR ||
		(type1 == NL80211_IFTYPE_AP && type2 == NL80211_IFTYPE_WDS) ||
		(type1 == NL80211_IFTYPE_WDS &&
			(type2 == NL80211_IFTYPE_WDS ||
			 type2 == NL80211_IFTYPE_AP)) ||
		(type1 == NL80211_IFTYPE_AP && type2 == NL80211_IFTYPE_AP_VLAN) ||
		(type1 == NL80211_IFTYPE_AP_VLAN &&
			(type2 == NL80211_IFTYPE_AP ||
			 type2 == NL80211_IFTYPE_AP_VLAN));
}

static int ieee80211_check_concurrent_iface(struct ieee80211_sub_if_data *sdata,
					    enum nl80211_iftype iftype)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_sub_if_data *nsdata;
	struct net_device *dev = sdata->dev;

	ASSERT_RTNL();

	/* we hold the RTNL here so can safely walk the list */
	list_for_each_entry(nsdata, &local->interfaces, list) {
		struct net_device *ndev = nsdata->dev;

		if (ndev != dev && ieee80211_sdata_running(nsdata)) {

			atbm_printk_debug("%s:[%s] iftype(%d),[%s] iftype (%d)\n",__func__,sdata->name,iftype,nsdata->name,
				nsdata->vif.type);
			
#ifndef CONFIG_ATBM_SUPPORT_MULTIAP
			/*
			*only support one ap mode
			*/
			if(iftype == NL80211_IFTYPE_AP &&
				(nsdata->vif.type == NL80211_IFTYPE_AP && nsdata->u.ap.beacon)){
				atbm_printk_err("only support one ap mode[%s],[%s]\n",
				sdata->name,nsdata->name);
				return -EBUSY;
			}
#endif
			if((iftype == NL80211_IFTYPE_MONITOR)&&
			   (!(sdata->u.mntr_flags & MONITOR_FLAG_COOK_FRAMES))){
				/*
				*only support one monitor mode
				*/
				if(nsdata->vif.type == NL80211_IFTYPE_MONITOR){
					atbm_printk_err("only support one monitor mode[%s],[%s]\n",
					sdata->name,nsdata->name);
					return -EBUSY;
				}
				/*
				*when associated with ap or stated as ap mode, can not change to
				*monitor mode
				*/
				if (nsdata->vif.type == NL80211_IFTYPE_STATION && nsdata->u.mgd.associated){
					atbm_printk_err("[%s] is associated ,[%s] can not run into monior\n",
						nsdata->name,sdata->name);
					return -EBUSY;
				}
				if (nsdata->vif.type == NL80211_IFTYPE_AP && nsdata->u.ap.beacon){
					atbm_printk_err("[%s] is ap mode ,[%s] can not run into monior\n",
						nsdata->name,sdata->name);
					return -EBUSY;
				}
			}
			/*
			*when we act as monitor mode ,other mode shoud not be enabled
			*/
			if((nsdata->vif.type == NL80211_IFTYPE_MONITOR)&&
			   (!(nsdata->u.mntr_flags & MONITOR_FLAG_COOK_FRAMES))){
			   atbm_printk_err("[%s] in monitor mode,so [%s] can not up\n",
			   	nsdata->name,sdata->name);
			   return -EBUSY;
			}
#ifdef CONFIG_ATBM_SUPPORT_IBSS			  
			/*
			 * Allow only a single IBSS interface to be up at any
			 * time. This is restricted because beacon distribution
			 * cannot work properly if both are in the same IBSS.
			 *
			 * To remove this restriction we'd have to disallow them
			 * from setting the same SSID on different IBSS interfaces
			 * belonging to the same hardware. Then, however, we're
			 * faced with having to adopt two different TSF timers...
			 */
			if (iftype == NL80211_IFTYPE_ADHOC &&
			    nsdata->vif.type == NL80211_IFTYPE_ADHOC)
				return -EBUSY;
#endif
			/*
			 * The remaining checks are only performed for interfaces
			 * with the same MAC address.
			 */
			if (atbm_compare_ether_addr(dev->dev_addr, ndev->dev_addr))
				continue;

			/*
			 * check whether it may have the same address
			 */
			if (!identical_mac_addr_allowed(iftype,
							nsdata->vif.type))
				return -ENOTUNIQ;

			/*
			 * can only add VLANs to enabled APs
			 */
			if (iftype == NL80211_IFTYPE_AP_VLAN &&
			    nsdata->vif.type == NL80211_IFTYPE_AP)
				sdata->bss = &nsdata->u.ap;
		}
	}

	return 0;
}

static int ieee80211_check_queues(struct ieee80211_sub_if_data *sdata)
{
	int n_queues = sdata->local->hw.queues;
	int i;

	for (i = 0; i < IEEE80211_NUM_ACS; i++) {
		if (WARN_ON_ONCE(sdata->vif.hw_queue[i] ==
				 IEEE80211_INVAL_HW_QUEUE))
			return -EINVAL;
		//printk("(%p)sdata->vif.hw_queue[%d]=%d ",sdata,i,sdata->vif.hw_queue[i]);
		if (WARN_ON_ONCE(sdata->vif.hw_queue[i] >=
				 n_queues))
			return -EINVAL;
	}

	if ((sdata->vif.type != NL80211_IFTYPE_AP) ||
	    !(sdata->local->hw.flags & IEEE80211_HW_QUEUE_CONTROL)) {
		sdata->vif.cab_queue = IEEE80211_INVAL_HW_QUEUE;
		return 0;
	}

	if (WARN_ON_ONCE(sdata->vif.cab_queue == IEEE80211_INVAL_HW_QUEUE))
		return -EINVAL;

	if (WARN_ON_ONCE(sdata->vif.cab_queue >= n_queues))
		return -EINVAL;

	return 0;
}

void ieee80211_adjust_monitor_flags(struct ieee80211_sub_if_data *sdata,
				    const int offset)
{
	u32 flags = sdata->u.mntr_flags, req_flags = 0;

#define ADJUST(_f, _s) do {\
	if (flags & MONITOR_FLAG_##_f)\
		req_flags |= _s;\
	} while (0)

	ADJUST(PLCPFAIL, FIF_PLCPFAIL);
	ADJUST(CONTROL, FIF_PSPOLL);
	ADJUST(CONTROL, FIF_CONTROL);
	ADJUST(FCSFAIL, FIF_FCSFAIL);
	ADJUST(OTHER_BSS, FIF_OTHER_BSS);
	if (offset > 0)
		sdata->req_filt_flags |= req_flags;
	else
		sdata->req_filt_flags &= ~req_flags;

#undef ADJUST
}

static void ieee80211_set_default_queues(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_local *local = sdata->local;
	int i;

	for (i = 0; i < IEEE80211_NUM_ACS; i++) {
		if (local->hw.flags & IEEE80211_HW_QUEUE_CONTROL)
			sdata->vif.hw_queue[i] = IEEE80211_INVAL_HW_QUEUE;
		else
			sdata->vif.hw_queue[i] = i;
		//printk("%s (%p)sdata->vif.hw_queue[%d]=%d\n",__func__,sdata,i,sdata->vif.hw_queue[i]);
	}
	sdata->vif.cab_queue = IEEE80211_INVAL_HW_QUEUE;
	
}

/*
 * NOTE: Be very careful when changing this function, it must NOT return
 * an error on interface type changes that have been pre-checked, so most
 * checks should be in ieee80211_check_concurrent_iface.
 */
static int ieee80211_do_open(struct net_device *dev, bool coming_up)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
#ifdef CONFIG_ATBM_SUPPORT_WDS
	struct sta_info *sta;
#endif
	u32 changed = 0;
	int res;
	u32 hw_reconf_flags = 0;

	sdata->vif.bss_conf.chan_conf = &sdata->chan_state.conf;
	
	////printk("%s (%p)sdata->vif.hw_queue[0]=%d\n",__func__,sdata,sdata->vif.hw_queue[0]);

	switch (sdata->vif.type) {
#ifdef CONFIG_ATBM_SUPPORT_WDS
	case NL80211_IFTYPE_WDS:
		if (!is_valid_ether_addr(sdata->u.wds.remote_addr))
			return -ENOLINK;
		break;
#endif
	case NL80211_IFTYPE_AP_VLAN:
		if (!sdata->bss)
			return -ENOLINK;
		list_add(&sdata->u.vlan.list, &sdata->bss->vlans);
		break;
	case NL80211_IFTYPE_AP:
		sdata->bss = &sdata->u.ap;
		break;
#ifdef CONFIG_MAC80211_ATBM_MESH
	case NL80211_IFTYPE_MESH_POINT:
#endif
	case NL80211_IFTYPE_STATION:
	case NL80211_IFTYPE_MONITOR:
#ifdef CONFIG_ATBM_SUPPORT_IBSS
	case NL80211_IFTYPE_ADHOC:
#endif
		/* no special treatment */
		break;
	case NL80211_IFTYPE_UNSPECIFIED:
	case NUM_NL80211_IFTYPES:
#ifdef CONFIG_ATBM_SUPPORT_P2P
	case NL80211_IFTYPE_P2P_CLIENT:
	case NL80211_IFTYPE_P2P_GO:
#endif
		/* cannot happen */
		WARN_ON(1);
		break;
	default:
		WARN_ON(1);
		break;
	}

	if (local->open_count == 0) {
		res = drv_start(local);
		if (res)
			goto err_del_bss;
#ifdef IEEE80211_SUPPORT_NAPI
		if (local->ops->napi_poll)
			napi_enable(&local->napi);
#endif
		/* we're brought up, everything changes */
		hw_reconf_flags = ~0;
		ieee80211_led_radio(local, true);
		ieee80211_mod_tpt_led_trig(local,
					   IEEE80211_TPT_LEDTRIG_FL_RADIO, 0);
	}

	/*
	 * Copy the hopefully now-present MAC address to
	 * this interface, if it has the special null one.
	 */
	if (is_zero_ether_addr(dev->dev_addr)) {
		memcpy(dev->dev_addr,
		       local->hw.wiphy->perm_addr,
		       ETH_ALEN);
		memcpy(dev->perm_addr, dev->dev_addr, ETH_ALEN);

		if (!is_valid_ether_addr(dev->dev_addr)) {
			if (!local->open_count)
				drv_stop(local);
			return -EADDRNOTAVAIL;
		}
	}

	switch (sdata->vif.type) {
	case NL80211_IFTYPE_AP_VLAN:
		/* no need to tell driver */
		break;
	case NL80211_IFTYPE_MONITOR:
		if (sdata->u.mntr_flags & MONITOR_FLAG_COOK_FRAMES) {
			local->cooked_mntrs++;
			break;
		}

		/* must be before the call to ieee80211_configure_filter */
		local->monitors++;
		if (local->monitors == 1) {
			local->hw.conf.flags |= IEEE80211_CONF_MONITOR;
			hw_reconf_flags |= IEEE80211_CONF_CHANGE_MONITOR;
		}

		ieee80211_adjust_monitor_flags(sdata, 1);
		ieee80211_configure_filter(sdata);
		netif_carrier_on(dev);
		local->only_monitors++;
		local->monitor_sdata=sdata;
#if (LINUX_VERSION_CODE > KERNEL_VERSION(5, 10, 60))
		
			fallthrough;
#endif
	default:
	
		if (coming_up) {
			res = drv_add_interface(local, &sdata->vif);
			if (res)
				goto err_stop;
			//printk("%s <%d> ,(%p)sdata->vif.hw_queue[0]=%d\n",__func__,__LINE__,sdata,sdata->vif.hw_queue[0]);
			res = ieee80211_check_queues(sdata);
			if (res)
				goto err_del_interface;
		}
		//printk("%s <%d> ,(%p)sdata->vif.hw_queue[0]=%d\n",__func__,__LINE__,sdata,sdata->vif.hw_queue[0]);

		if (sdata->vif.type == NL80211_IFTYPE_AP) {
			ieee80211_configure_filter(sdata);
			ieee80211_medium_traffic_start(sdata);
		}

		changed |= ieee80211_reset_erp_info(sdata);
		ieee80211_bss_info_change_notify(sdata, changed);

		if (sdata->vif.type == NL80211_IFTYPE_STATION){
			netif_carrier_off(dev);
		}else {
			netif_carrier_on(dev);
		}

#ifdef 	CONFIG_MAC80211_BRIDGE
		//add by wp for ipc bridge
		dev->priv_flags &= ~IFF_DONT_BRIDGE;
#endif //	CONFIG_MAC80211_BRIDGE
	}

	set_bit(SDATA_STATE_RUNNING, &sdata->state);
#ifdef CONFIG_ATBM_SUPPORT_WDS
	if (sdata->vif.type == NL80211_IFTYPE_WDS) {
		/* Create STA entry for the WDS peer */
		sta = sta_info_alloc(sdata, sdata->u.wds.remote_addr,
				     GFP_KERNEL);
		if (!sta) {
			res = -ENOMEM;
			goto err_del_interface;
		}

		/* no atomic bitop required since STA is not live yet */
		set_sta_flag(sta, WLAN_STA_AUTHORIZED);

		res = sta_info_insert(sta);
		if (res) {
			/* STA has been freed */
			goto err_del_interface;
		}
#ifndef CONFIG_RATE_HW_CONTROL
		rate_control_rate_init(sta);
#endif
	}
#endif
	mutex_lock(&local->mtx);	
	hw_reconf_flags |= __ieee80211_recalc_idle(local);
	mutex_unlock(&local->mtx);

	if (coming_up)
		local->open_count++;
#ifdef CONFIG_MAC80211_BRIDGE
	br0_netdev_open(dev);
#endif	// CONFIG_MAC80211_BRIDGE

	if (hw_reconf_flags) {
		ieee80211_hw_config(local, hw_reconf_flags);
		/*
		 * set default queue parameters so drivers don't
		 * need to initialise the hardware if the hardware
		 * doesn't start up with sane defaults
		 */
		ieee80211_set_wmm_default(sdata);
	}

	ieee80211_recalc_ps(local, -1);

	netif_tx_start_all_queues(dev);

	return 0;
 err_del_interface:
	drv_remove_interface(local, &sdata->vif);
 err_stop:
	if (!local->open_count)
		drv_stop(local);
 err_del_bss:
	sdata->bss = NULL;
	if (sdata->vif.type == NL80211_IFTYPE_AP_VLAN)
		list_del(&sdata->u.vlan.list);
	clear_bit(SDATA_STATE_RUNNING, &sdata->state);
	return res;
}

static int ieee80211_open(struct net_device *dev)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	int err;

	/* fail early if user set an invalid address */
	if (!is_valid_ether_addr(dev->dev_addr))
		return -EADDRNOTAVAIL;
	//printk("%s (%p)sdata->vif.hw_queue[0]=%d\n",__func__,sdata,sdata->vif.hw_queue[0]);

	err = ieee80211_check_concurrent_iface(sdata, sdata->vif.type);
	if (err)
		return err;

	return ieee80211_do_open(dev, true);
}

static void ieee80211_do_stop(struct ieee80211_sub_if_data *sdata,
			      bool going_down)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(local, sdata);
	unsigned long flags;
	struct sk_buff *skb, *tmp;
	u32 hw_reconf_flags = 0;
	int i;
	enum nl80211_channel_type orig_ct;
	
#ifdef CONFIG_ATBM_STA_LISTEN
	mutex_lock(&local->mtx);
	if(local->listen_sdata == sdata)
		ieee80211_sdata_listen_recalc(local,false,false);
	mutex_unlock(&local->mtx);
#endif
	clear_bit(SDATA_STATE_RUNNING, &sdata->state);

	if (local->scan_sdata == sdata)
		ieee80211_scan_cancel(local);
	else if(local->pending_scan_sdata&&(local->pending_scan_sdata == sdata))
	{
		local->scan_sdata = local->pending_scan_sdata;
		local->scan_req = local->pending_scan_req;
		local->pending_scan_sdata = NULL;
		local->pending_scan_req = NULL;
		ieee80211_scan_cancel(local);
	}		
	/*
	 * Stop TX on this interface first.
	 */
	netif_tx_stop_all_queues(sdata->dev);
#ifdef CONFIG_ATBM_SUPPORT_P2P
	ieee80211_roc_purge(sdata);
#endif
	/*
	 * Purge work for this interface.
	 */
	ieee80211_work_purge(sdata);

#ifdef ATBM_AP_SME
	ieee80211_ap_sme_mlme_purge(sdata);
	ieee80211_ap_sme_event_purge(sdata);
#endif
	/*
	 * Remove all stations associated with this interface.
	 *
	 * This must be done before calling ops->remove_interface()
	 * because otherwise we can later invoke ops->sta_notify()
	 * whenever the STAs are removed, and that invalidates driver
	 * assumptions about always getting a vif pointer that is valid
	 * (because if we remove a STA after ops->remove_interface()
	 * the driver will have removed the vif info already!)
	 *
	 * This is relevant only in AP, WDS and mesh modes, since in
	 * all other modes we've already removed all stations when
	 * disconnecting etc.
	 */
	sta_info_flush(local, sdata);

	netif_addr_lock_bh(sdata->dev);
	spin_lock_bh(&local->filter_lock);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,35))
	__hw_addr_unsync(&sdata->mc_list, &sdata->dev->mc,
			 sdata->dev->addr_len);
#else
	__dev_addr_unsync(&sdata->mc_list, &sdata->mc_count,
			  &sdata->dev->mc_list, &sdata->dev->mc_count);
#endif
	spin_unlock_bh(&local->filter_lock);
	netif_addr_unlock_bh(sdata->dev);

	ieee80211_configure_filter(sdata);
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	atbm_del_timer_sync(&sdata->dynamic_ps_timer);
	atbm_cancel_work_sync(&sdata->dynamic_ps_enable_work);
#endif
#ifdef CONFIG_ATBM_RADAR_DETECT
#ifdef CONFIG_ATBM_5G_PRETEND_2G
	ieee80211_dfs_cac_abort(sdata);
#endif
#endif
	
	ieee80211_medium_traffic_concle(sdata);
	
	/* APs need special treatment */
	if (sdata->vif.type == NL80211_IFTYPE_AP) {
		struct ieee80211_sub_if_data *vlan, *tmpsdata;
#ifdef ATBM_PROBE_RESP_EXTRA_IE
		struct proberesp_data *old_proberesp;
#endif
		struct beacon_data *old_beacon =
			rtnl_dereference(sdata->u.ap.beacon);
		struct beacon_extra *old_extra = rtnl_dereference(sdata->u.ap.beacon_extra);
		struct probe_response_extra *old_proberesp_extra = rtnl_dereference(sdata->u.ap.probe_response_extra);
		
#ifdef ATBM_PROBE_RESP_EXTRA_IE
		old_proberesp = rtnl_dereference(sdata->u.ap.proberesp);		
		RCU_INIT_POINTER(sdata->u.ap.proberesp, NULL);
#endif
		
		/* sdata_running will return false, so this will disable */
		ieee80211_bss_info_change_notify(sdata,
						 BSS_CHANGED_BEACON_ENABLED);

		/* remove beacon */
		RCU_INIT_POINTER(sdata->u.ap.beacon, NULL);
		RCU_INIT_POINTER(sdata->u.ap.beacon_extra,NULL);
		RCU_INIT_POINTER(sdata->u.ap.probe_response_extra,NULL);
		synchronize_rcu();
		atbm_kfree(old_beacon);
		if(old_extra)
			atbm_kfree(old_extra);
#ifdef ATBM_PROBE_RESP_EXTRA_IE
		if(old_proberesp)
			atbm_kfree(old_proberesp);
#endif
		if(old_proberesp_extra)
			atbm_kfree(old_proberesp_extra);
		
		/* down all dependent devices, that is VLANs */
		list_for_each_entry_safe(vlan, tmpsdata, &sdata->u.ap.vlans,
					 u.vlan.list)
			dev_close(vlan->dev);
		WARN_ON(!list_empty(&sdata->u.ap.vlans));

		/* free all potentially still buffered bcast frames */
		local->total_ps_buffered -= atbm_skb_queue_len(&sdata->u.ap.ps_bc_buf);
		atbm_skb_queue_purge(&sdata->u.ap.ps_bc_buf);
	}if(sdata->vif.type == NL80211_IFTYPE_STATION){
		struct probe_request_extra *extra = rtnl_dereference(sdata->u.mgd.probe_request_extra);
		
		RCU_INIT_POINTER(sdata->u.mgd.probe_request_extra,NULL);
		synchronize_rcu();
		if(extra)
			atbm_kfree(extra);
		
		ieee80211_free_authen_bss(sdata);
	}
#ifdef CONFIG_ATBM_SUPPORT_AP_CONFIG
	ieee80211_update_ap_config(sdata,NULL,true);
#endif
	if (going_down)
		local->open_count--;

	switch (sdata->vif.type) {
	case NL80211_IFTYPE_AP_VLAN:
		list_del(&sdata->u.vlan.list);
		/* no need to tell driver */
		break;
	case NL80211_IFTYPE_MONITOR:
		if (sdata->u.mntr_flags & MONITOR_FLAG_COOK_FRAMES) {
			local->cooked_mntrs--;
			break;
		}

		local->monitors--;
		if (local->monitors == 0) {
			local->hw.conf.flags &= ~IEEE80211_CONF_MONITOR;
			hw_reconf_flags |= IEEE80211_CONF_CHANGE_MONITOR;
		}

		ieee80211_adjust_monitor_flags(sdata, -1);
		ieee80211_configure_filter(sdata);
		local->only_monitors--;
		local->monitor_sdata = NULL;
		rcu_assign_pointer(sdata->local->internal_monitor.req.monitor_rx,NULL);
		rcu_assign_pointer(sdata->local->internal_monitor.req.priv,NULL);
		synchronize_rcu();
#if (LINUX_VERSION_CODE > KERNEL_VERSION(5, 10, 60))
		
			fallthrough;
#endif
    // break;
	default:
		atbm_flush_work(&sdata->work);
		ieee80211_special_filter_exit(sdata);
		/*
		 * When we get here, the interface is marked down.
		 * Call synchronize_rcu() to wait for the RX path
		 * should it be using the interface and enqueuing
		 * frames at this very time on another CPU.
		 */
		synchronize_rcu();
		atbm_skb_queue_purge(&sdata->skb_queue);
		#ifdef ATBM_AP_SME
		ieee80211_ap_sme_mlme_purge(sdata);
		ieee80211_ap_sme_event_purge(sdata);
		#endif
#ifdef CONFIG_MAC80211_ATBM_MESH
		/*
		 * Disable beaconing here for mesh only, AP and IBSS
		 * are already taken care of.
		 */
		if (sdata->vif.type == NL80211_IFTYPE_MESH_POINT)
			ieee80211_bss_info_change_notify(sdata,
				BSS_CHANGED_BEACON_ENABLED);
#endif

		/*
		 * Free all remaining keys, there shouldn't be any,
		 * except maybe group keys in AP more or WDS?
		 */
		ieee80211_free_keys(sdata);
		mutex_lock(&local->mtx);
		if(sdata->last_scan_ie)
			atbm_kfree(sdata->last_scan_ie);
		sdata->last_scan_ie_len = 0;
		sdata->last_scan_ie = NULL;
		mutex_unlock(&local->mtx);
		if (going_down)
			drv_remove_interface(local, &sdata->vif);
	}

	sdata->bss = NULL;
#ifdef CONFIG_MAC80211_BRIDGE
	//void ieee80211_brigde_flush(_adapter *priv);
	ieee80211_brigde_flush(sdata);
#endif	// CONFIG_MAC80211_BRIDGE

	mutex_lock(&local->mtx);
	hw_reconf_flags |= __ieee80211_recalc_idle(local);
	mutex_unlock(&local->mtx);

	ieee80211_recalc_ps(local, -1);

	if (local->open_count == 0) {
		#ifdef IEEE80211_SUPPORT_NAPI
		if (local->ops->napi_poll)
			napi_disable(&local->napi);
		#endif
		ieee80211_clear_tx_pending(local);
		ieee80211_stop_device(local);

		/* no reconfiguring after stop! */
		hw_reconf_flags = 0;
	}

	/* Re-calculate channel-type, in case there are multiple vifs
	 * on different channel types.
	 */
	orig_ct = chan_state->_oper_channel_type;
#ifdef CONFIG_ATBM_SUPPORT_MULTI_CHANNEL
	if (local->hw.flags & IEEE80211_HW_SUPPORTS_MULTI_CHANNEL)
		ieee80211_set_channel_type(local, sdata, NL80211_CHAN_NO_HT);
	else
#endif
		ieee80211_set_channel_type(local, sdata, NL80211_CHAN_NO_HT);

	/* do after stop to avoid reconfiguring when we stop anyway */
	if (hw_reconf_flags || (orig_ct != chan_state->_oper_channel_type))
		ieee80211_hw_config(local, hw_reconf_flags);

	spin_lock_irqsave(&local->queue_stop_reason_lock, flags);
	for (i = 0; i < IEEE80211_MAX_QUEUES; i++) {
		atbm_skb_queue_walk_safe(&local->pending[i], skb, tmp) {
			struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
			if (info->control.vif == &sdata->vif) {
				__atbm_skb_unlink(skb, &local->pending[i]);
				atbm_dev_kfree_skb_irq(skb);
			}
		}
	}
	spin_unlock_irqrestore(&local->queue_stop_reason_lock, flags);

}

static int ieee80211_stop(struct net_device *dev)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	ieee80211_do_stop(sdata, true);

	return 0;
}

static void ieee80211_set_multicast_list(struct net_device *dev)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	int allmulti, promisc, sdata_allmulti, sdata_promisc;

#if 0
	if (!(SDATA_STATE_RUNNING & sdata->state))
		return;
#endif

	allmulti = !!(dev->flags & IFF_ALLMULTI);
	promisc = !!(dev->flags & IFF_PROMISC);
	sdata_allmulti = !!(sdata->flags & IEEE80211_SDATA_ALLMULTI);
	sdata_promisc = !!(sdata->flags & IEEE80211_SDATA_PROMISC);

	if (allmulti != sdata_allmulti)
		sdata->flags ^= IEEE80211_SDATA_ALLMULTI;

	if (promisc != sdata_promisc)
		sdata->flags ^= IEEE80211_SDATA_PROMISC;

	spin_lock_bh(&local->filter_lock);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,35))
	__hw_addr_sync(&sdata->mc_list, &dev->mc, dev->addr_len);
#else
	__dev_addr_sync(&sdata->mc_list, &sdata->mc_count,
			&dev->mc_list, &dev->mc_count);
#endif
	spin_unlock_bh(&local->filter_lock);
	ieee80211_queue_work(&local->hw, &sdata->reconfig_filter);
}

/*
 * Called when the netdev is removed or, by the code below, before
 * the interface type changes.
 */
static void ieee80211_teardown_sdata(struct net_device *dev)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	int flushed;
	int i;

	/* free extra data */
	ieee80211_free_keys(sdata);

	ieee80211_debugfs_remove_netdev(sdata);

	for (i = 0; i < IEEE80211_FRAGMENT_MAX; i++)
		__atbm_skb_queue_purge(&sdata->fragments[i].skb_list);
	sdata->fragment_next = 0;
#ifdef CONFIG_MAC80211_ATBM_MESH
	if (ieee80211_vif_is_mesh(&sdata->vif))
		mesh_rmc_free(sdata);
#endif
	flushed = sta_info_flush(local, sdata);
	WARN_ON(flushed);
}
static void ieee80211_sdata_uninit(struct net_device *dev)
{
#ifdef CONFIG_ATBM_STA_LISTEN
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	mutex_lock(&local->mtx);
	atbm_printk_err("%s:listen_sdata(%p)\n",__func__,local->listen_sdata);
	if(local->listen_sdata == sdata)
		ieee80211_sdata_listen_recalc(local,false,true);
	mutex_unlock(&local->mtx);
#endif
	ieee80211_teardown_sdata(dev);
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 4, 0))
static u16 ieee80211_netdev_select_queue(struct net_device *dev,
                                        struct sk_buff *skb,
										struct net_device *sb_dev)
										
#elif (LINUX_VERSION_CODE >=  KERNEL_VERSION(4,9,37))

static u16 ieee80211_netdev_select_queue(struct net_device *dev,
										struct sk_buff *skb,
                                        void *accel_priv,
                                        select_queue_fallback_t fallback)


#elif ( LINUX_VERSION_CODE >=  KERNEL_VERSION(4,4,0))

static u16 ieee80211_netdev_select_queue(struct net_device *dev,
                                        struct sk_buff *skb,
                                         struct net_device *sb_dev,
                                        select_queue_fallback_t fallback)

#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 11, 0))
static u16 ieee80211_netdev_select_queue(struct net_device *dev,
										struct sk_buff *skb,
                                        void *accel_priv,
                                        select_queue_fallback_t fallback)
#else
static u16 ieee80211_netdev_select_queue(struct net_device *dev,struct sk_buff *skb)
#endif
{
	return ieee80211_select_queue(IEEE80211_DEV_TO_SUB_IF(dev), skb);
}
/*
*CONFIG_ATBM_IOCTRL : use net ioctrl to process some cmd
*/
#if defined(CONFIG_ATBM_IOCTRL)

typedef struct android_wifi_priv_cmd_s {
	char *buf;
	int used_len;
	int total_len;
} android_wifi_priv_cmd;

struct altm_msg{
	unsigned int type;
	unsigned int value;
	unsigned int externData[30];
};
#define IEEE80211_CMD_GET(cmdname,index) (cmdname##_buff[index] == NULL ? \
	cmdname##_default:cmdname##_buff[index])

#ifdef CONFIG_ATBM_IOCTRL_VENDOR_CMD
enum ANDROID_WIFI_CMD {
	ANDROID_WIFI_CMD_START,				
	ANDROID_WIFI_CMD_STOP,			
	ANDROID_WIFI_CMD_SCAN_ACTIVE,
	ANDROID_WIFI_CMD_SCAN_PASSIVE,		
	ANDROID_WIFI_CMD_RSSI,	
	ANDROID_WIFI_CMD_LINKSPEED,
	ANDROID_WIFI_CMD_RXFILTER_START,
	ANDROID_WIFI_CMD_RXFILTER_STOP,	
	ANDROID_WIFI_CMD_RXFILTER_ADD,	
	ANDROID_WIFI_CMD_RXFILTER_REMOVE,
	ANDROID_WIFI_CMD_BTCOEXSCAN_START,
	ANDROID_WIFI_CMD_BTCOEXSCAN_STOP,
	ANDROID_WIFI_CMD_BTCOEXMODE,
	ANDROID_WIFI_CMD_SETSUSPENDOPT,
	ANDROID_WIFI_CMD_P2P_DEV_ADDR,	
	ANDROID_WIFI_CMD_SETFWPATH,		
	ANDROID_WIFI_CMD_SETBAND,		
	ANDROID_WIFI_CMD_GETBAND,			
	ANDROID_WIFI_CMD_COUNTRY,			
	ANDROID_WIFI_CMD_P2P_SET_NOA,
	ANDROID_WIFI_CMD_P2P_GET_NOA,	
	ANDROID_WIFI_CMD_P2P_SET_PS,	
	ANDROID_WIFI_CMD_SET_AP_WPS_P2P_IE,
#ifdef CONFIG_PNO_SUPPORT
	ANDROID_WIFI_CMD_PNOSSIDCLR_SET,
	ANDROID_WIFI_CMD_PNOSETUP_SET,
	ANDROID_WIFI_CMD_PNOENABLE_SET,
	ANDROID_WIFI_CMD_PNODEBUG_SET,
#endif

	ANDROID_WIFI_CMD_MACADDR,

	ANDROID_WIFI_CMD_BLOCK,

	ANDROID_WIFI_CMD_WFD_ENABLE,
	ANDROID_WIFI_CMD_WFD_DISABLE,
	
	ANDROID_WIFI_CMD_WFD_SET_TCPPORT,
	ANDROID_WIFI_CMD_WFD_SET_MAX_TPUT,
	ANDROID_WIFI_CMD_WFD_SET_DEVTYPE,
	ANDROID_WIFI_CMD_CHANGE_DTIM,
	ANDROID_WIFI_CMD_HOSTAPD_SET_MACADDR_ACL,
	ANDROID_WIFI_CMD_HOSTAPD_ACL_ADD_STA,
	ANDROID_WIFI_CMD_HOSTAPD_ACL_REMOVE_STA,
#ifdef CONFIG_GTK_OL
	ANDROID_WIFI_CMD_GTK_REKEY_OFFLOAD,
#endif //CONFIG_GTK_OL
	ANDROID_WIFI_CMD_P2P_DISABLE,
	ANDROID_WIFI_CMD_MAX
};

static const char *android_wifi_cmd_str[ANDROID_WIFI_CMD_MAX] = {
	"START",
	"STOP",
	"SCAN-ACTIVE",
	"SCAN-PASSIVE",
	"RSSI",
	"LINKSPEED",
	"RXFILTER-START",
	"RXFILTER-STOP",
	"RXFILTER-ADD",
	"RXFILTER-REMOVE",
	"BTCOEXSCAN-START",
	"BTCOEXSCAN-STOP",
	"BTCOEXMODE",
	"SETSUSPENDOPT",
	"P2P_DEV_ADDR",
	"SETFWPATH",
	"SETBAND",
	"GETBAND",
	"COUNTRY",
	"P2P_SET_NOA",
	"P2P_GET_NOA",
	"P2P_SET_PS",
	"SET_AP_WPS_P2P_IE",
#ifdef CONFIG_PNO_SUPPORT
	"PNOSSIDCLR",
	"PNOSETUP",
	"PNOFORCE",
	"PNODEBUG",
#endif

	"MACADDR",

	"BLOCK",
	"WFD-ENABLE",
	"WFD-DISABLE",
	"WFD-SET-TCPPORT",
	"WFD-SET-MAXTPUT",
	"WFD-SET-DEVTYPE",
	"SET_DTIM",
	"HOSTAPD_SET_MACADDR_ACL",
	"HOSTAPD_ACL_ADD_STA",
	"HOSTAPD_ACL_REMOVE_STA",
#ifdef CONFIG_GTK_OL
	"GTK_REKEY_OFFLOAD",
#endif //CONFIG_GTK_OL
/*	Private command for	P2P disable*/
	"P2P_DISABLE"
};
typedef int (*android_cmd_handle)(struct ieee80211_sub_if_data *sdata,char *cmd,u32 cmd_len,u8 cmd_index);

int ieee80211_android_cmd_handle_default(struct ieee80211_sub_if_data *sdata, 
	char *cmd,u32 cmd_len,u8 cmd_index)
{
	int ret = 0;
	atbm_printk_debug("%s:android cmd-->(%s) unhandle\n",__func__,cmd);

	return ret;
}

static const android_cmd_handle ieee80211_android_cmd_handle_buff[] = {
	/*ANDROID_WIFI_CMD_START*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_STOP*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_SCAN_ACTIVE*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_SCAN_PASSIVE*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_RSSI*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_LINKSPEED*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_RXFILTER_START*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_RXFILTER_STOP*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_RXFILTER_ADD*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_RXFILTER_REMOVE*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_BTCOEXSCAN_START*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_BTCOEXSCAN_STOP*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_BTCOEXMODE*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_SETSUSPENDOPT*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_P2P_DEV_ADDR*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_SETFWPATH*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_SETBAND*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_GETBAND*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_COUNTRY*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_P2P_SET_NOA*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_P2P_GET_NOA*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_P2P_SET_PS*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_SET_AP_WPS_P2P_IE*/
	ieee80211_android_cmd_handle_default,
#ifdef CONFIG_PNO_SUPPORT
	/*ANDROID_WIFI_CMD_PNOSSIDCLR_SET*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_PNOSETUP_SET*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_PNOENABLE_SET*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_PNODEBUG_SET*/
	ieee80211_android_cmd_handle_default,
#endif

	/*ANDROID_WIFI_CMD_MACADDR*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_BLOCK*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_WFD_ENABLE*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_WFD_DISABLE*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_WFD_SET_TCPPORT*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_WFD_SET_MAX_TPUT*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_WFD_SET_DEVTYPE*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_CHANGE_DTIM*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_HOSTAPD_SET_MACADDR_ACL*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_HOSTAPD_ACL_ADD_STA*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_HOSTAPD_ACL_REMOVE_STA*/
	ieee80211_android_cmd_handle_default,
#ifdef CONFIG_GTK_OL
	/*ANDROID_WIFI_CMD_GTK_REKEY_OFFLOAD*/
	ieee80211_android_cmd_handle_default,
#endif //CONFIG_GTK_OL
	/*ANDROID_WIFI_CMD_P2P_DISABLE*/
	ieee80211_android_cmd_handle_default,
	/*ANDROID_WIFI_CMD_MAX*/
	ieee80211_android_cmd_handle_default

 };
int ieee80211_android_cmdstr_to_num(char *cmdstr)
{
	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 0, 0))
	#define strnicmp	strncasecmp
	#endif /* Linux kernel >= 4.0.0 */
	
	int cmd_num;
	for(cmd_num=0 ; cmd_num<ANDROID_WIFI_CMD_MAX; cmd_num++)
		if(0 == strnicmp(cmdstr , android_wifi_cmd_str[cmd_num], 
			strlen(android_wifi_cmd_str[cmd_num])) )
			break;

	return cmd_num;
}
/*
*process android cmd,only retrun success
*/
int ieee80211_netdev_process_android_cmd(struct net_device *dev, struct ifreq *rq)
{	
#if 0
	char *android_cmd = NULL;
	int cmd_num = ANDROID_WIFI_CMD_MAX;
	android_cmd_handle call_fn = NULL;
	int ret=0;
	android_wifi_priv_cmd android_cmd_struct;
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	
	if (!rq->ifr_data) 
	{
		printk(KERN_ERR "ifr->ifr_data == NULL\n");
		ret = -EINVAL;
		goto exit;
	}

	memset(&android_cmd_struct,0,sizeof(android_wifi_priv_cmd));
	
	if (copy_from_user(&android_cmd_struct, rq->ifr_data, sizeof(android_wifi_priv_cmd))) 
	{
		printk(KERN_ERR "copy_from_user err\n");
		ret = -EFAULT;
		goto exit;
	}
	if(android_cmd_struct.total_len<=0)
	{
		printk(KERN_ERR "android_cmd_struct.total_len<=0\n");
		ret = -EINVAL;
		goto exit;
	}
	android_cmd = atbm_kmalloc(android_cmd_struct.total_len,GFP_KERNEL);

	if(android_cmd == NULL)
	{
		printk(KERN_ERR "atbm_kmalloc err\n");
		ret = -EFAULT;
		goto exit;
	}

	memset(android_cmd,0,android_cmd_struct.total_len);

	if (copy_from_user(android_cmd, (void *)android_cmd_struct.buf, 
		android_cmd_struct.total_len)) 
	{
		printk(KERN_ERR "copy_from_user android_cmd err\n");
		ret = -EFAULT;
		goto exit;
	}
	if(android_cmd == NULL)
	{
		printk(KERN_ERR "android_cmd == NULL\n");
		ret = -EFAULT;
		goto exit;
	}
	printk(KERN_ERR "%s\n",__func__);
	cmd_num = ieee80211_android_cmdstr_to_num(android_cmd);

	if(cmd_num >= ANDROID_WIFI_CMD_MAX)
	{
		printk(KERN_ERR "no cmd found\n");
		ret = -EFAULT;
		goto exit;
	}

	call_fn = IEEE80211_CMD_GET(ieee80211_android_cmd_handle,cmd_num);
	
	ret = call_fn(sdata,android_cmd,android_cmd_struct.total_len,cmd_num);
exit:
	if(android_cmd)
		atbm_kfree(android_cmd);
	return ret;
#else

	return 0;
#endif
}
#ifdef CONFIG_ATBM_DRIVER_RATE_CTRL
extern int rate_altm_control_test(struct wiphy *wiphy, void *data, int len);
#endif

/*
*vendor cmd ----> atbm_tool
*/
int ieee80211_netdev_process_vendor_cmd(struct net_device *dev, struct ifreq *rq)
{
	struct altm_msg vendor_msg;
	int ret=0;
	int i;
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	int len = 0;
	
	if (!rq->ifr_data) 
	{
		atbm_printk_err("%s:ifr->ifr_data == NULL\n",__func__);
		ret = -EINVAL;
		goto exit;
	}
	
	memset(&vendor_msg,0,sizeof(struct altm_msg));
	
	if (copy_from_user(&vendor_msg, rq->ifr_data, sizeof(struct altm_msg))) 
	{
		atbm_printk_err("copy_from_user err\n");
		ret = -EFAULT;
		goto exit;
	}
	len = sizeof(struct altm_msg);
	atbm_printk_debug("type = %d, value = %d\n", vendor_msg.type, vendor_msg.value);
	for(i=0;i<30;i++)
		atbm_printk_debug("%d ", vendor_msg.externData[i]);
	sdata->local->hw.vendcmd_nl80211 = 1;
#ifdef CONFIG_ATBM_DRIVER_RATE_CTRL
	ret = rate_altm_control_test(sdata->local->hw.wiphy,&vendor_msg,len);
#endif
#if defined(CONFIG_NL80211_TESTMODE) && defined(CONFIG_ATBM_TEST_TOOL)
	ret |= atbm_altmtest_cmd(&sdata->local->hw, &vendor_msg, len); 
#endif
	if((ret > 0)&&(ret<=len))
	{
		if (copy_to_user(rq->ifr_data,(u8*)(&sdata->local->hw.vendreturn),sizeof(struct response)))
		{
			atbm_printk_err("copy_to_user err\n");
			ret = -EFAULT;
		}
		else
			ret = 0;
		goto exit;
	}
exit:
	sdata->local->hw.vendcmd_nl80211 = 0;
	return ret;	
}
#else
int ieee80211_netdev_process_vendor_cmd(struct net_device *dev, struct ifreq *rq)
{
	return 0;
}
int ieee80211_netdev_process_android_cmd(struct net_device *dev, struct ifreq *rq)
{
	return 0;
}
#endif
#if 0

/* -------------------------- IOCTL LIST -------------------------- */

/* Basic operations */
#define SIOCSIWNAME	0x8B00		/* Unused */
#define SIOCGIWNAME	0x8B01		/* get name == wireless protocol */
#define SIOCSIWNWID	0x8B02		/* set network id (the cell) */
#define SIOCGIWNWID	0x8B03		/* get network id */
#define SIOCSIWFREQ	0x8B04		/* set channel/frequency (Hz) */
#define SIOCGIWFREQ	0x8B05		/* get channel/frequency (Hz) */
#define SIOCSIWMODE	0x8B06		/* set operation mode */
#define SIOCGIWMODE	0x8B07		/* get operation mode */
#define SIOCSIWSENS	0x8B08		/* set sensitivity (dBm) */
#define SIOCGIWSENS	0x8B09		/* get sensitivity (dBm) */

/* Informative stuff */
#define SIOCSIWRANGE	0x8B0A		/* Unused */
#define SIOCGIWRANGE	0x8B0B		/* Get range of parameters */
#define SIOCSIWPRIV	0x8B0C		/* Unused */
#define SIOCGIWPRIV	0x8B0D		/* get private ioctl interface info */

/* Mobile IP support */
#define SIOCSIWSPY	0x8B10		/* set spy addresses */
#define SIOCGIWSPY	0x8B11		/* get spy info (quality of link) */

/* Access Point manipulation */
#define SIOCSIWAP	0x8B14		/* set access point MAC addresses */
#define SIOCGIWAP	0x8B15		/* get access point MAC addresses */
#define SIOCGIWAPLIST	0x8B17		/* get list of access point in range */

/* 802.11 specific support */
#define SIOCSIWESSID	0x8B1A		/* set ESSID (network name) */
#define SIOCGIWESSID	0x8B1B		/* get ESSID */
#define SIOCSIWNICKN	0x8B1C		/* set node name/nickname */
#define SIOCGIWNICKN	0x8B1D		/* get node name/nickname */
/* As the ESSID and NICKN are strings up to 32 bytes long, it doesn't fit
 * within the 'iwreq' structure, so we need to use the 'data' member to
 * point to a string in user space, like it is done for RANGE...
 * The "flags" member indicate if the ESSID is active or not (promiscuous).
 */

/* Other parameters usefull in 802.11 and some other devices */
#define SIOCSIWRATE	0x8B20		/* set default bit rate (bps) */
#define SIOCGIWRATE	0x8B21		/* get default bit rate (bps) */
#define SIOCSIWRTS	0x8B22		/* set RTS/CTS threshold (bytes) */
#define SIOCGIWRTS	0x8B23		/* get RTS/CTS threshold (bytes) */
#define SIOCSIWFRAG	0x8B24		/* set fragmentation thr (bytes) */
#define SIOCGIWFRAG	0x8B25		/* get fragmentation thr (bytes) */
#define SIOCSIWTXPOW	0x8B26		/* set transmit power (dBm) */
#define SIOCGIWTXPOW	0x8B27		/* get transmit power (dBm) */

/* Encoding stuff (scrambling, hardware security, WEP...) */
#define SIOCSIWENCODE	0x8B2A		/* set encoding token & mode */
#define SIOCGIWENCODE	0x8B2B		/* get encoding token & mode */
/* Power saving stuff (power management, unicast and multicast) */
#define SIOCSIWPOWER	0x8B2C		/* set Power Management settings */
#define SIOCGIWPOWER	0x8B2D		/* get Power Management settings */
#endif
typedef struct ioctl_list{
	int cmd;
	char *cmd_str;
}ioctl_list_t;

ioctl_list_t list_data[]={
{0x8B00,"SIOCSIWNAME"},{0x8B01,"SIOCGIWNAME"},{0x8B02,"SIOCSIWNWID"},{0x8B03,"SIOCGIWNWID"},
{0x8B04,"SIOCSIWFREQ"},{0x8B05,"SIOCGIWFREQ"},{0x8B06,"SIOCSIWMODE"},{0x8B07,"SIOCGIWMODE"},
{0x8B08,"SIOCSIWSENS"},{0x8B09,"SIOCGIWSENS"},{0x8B0A,"SIOCSIWRANGE"},{0x8B0B,"SIOCGIWRANGE"},
{0x8B0C,"SIOCSIWPRIV"},{0x8B0D,"SIOCGIWPRIV"},{0x8B10,"SIOCSIWSPY"},{0x8B11,"SIOCGIWSPY"},
{0x8B14,"SIOCSIWAP"},{0x8B15,"SIOCGIWAP"},{0x8B17,"SIOCGIWAPLIST"},{0x8B1A,"SIOCSIWESSID"},
{0x8B1B,"SIOCGIWESSID"},{0x8B1C,"SIOCSIWNICKN"},{0x8B1D,"SIOCGIWNICKN"},{0x8B20,"SIOCSIWRATE"},
{0x8B21,"SIOCGIWRATE"},{0x8B22,"SIOCSIWRTS"},{0x8B23,"SIOCGIWRTS"},{0x8B24,"SIOCSIWFRAG"},
{0x8B25,"SIOCGIWFRAG"},{0x8B26,"SIOCSIWTXPOW"},{0x8B27,"SIOCGIWTXPOW"},{0x8B2A,"SIOCSIWENCODE"},
{0x8B2B,"SIOCGIWENCODE"},{0x8B2C,"SIOCSIWPOWER"},{0x8B2D,"SIOCGIWPOWER"},
{-1,NULL}
};


#define IEEE80211_NETDEV_BASE_CMD			SIOCDEVPRIVATE
#define IEEE80211_NETDEV_WEXT_CMD			SIOCDEVPRIVATE
#define IEEE80211_NETDEV_ANDROID_CMD		(SIOCDEVPRIVATE+1)
#define IEEE80211_NETDEV_VENDOR_CMD			(SIOCDEVPRIVATE+2)
#if defined(CONFIG_NL80211_TESTMODE) && defined(CONFIG_ATBM_TEST_TOOL)
extern int atbm_altmtest_cmd(struct ieee80211_hw *hw, void *data, int len);
#endif
#ifdef CONFIG_ATBM_DEV_IOCTL
extern int atbm_wext_cmd(struct net_device *dev, void *data, int len);
#endif
/*
* process wext cmd ,but now........
*/
#ifdef CONFIG_ATBM_DEV_IOCTL
int ieee80211_netdev_process_wext_cmd(struct net_device *dev, struct ifreq *rq)
{
	struct altm_wext_msg{
		int type;
		int value;
		char externData[256];
	};

	int ret=0;
	struct ieee80211_sub_if_data *sdata = NULL;
	struct ieee80211_local *local = NULL;
	struct ieee80211_hw *hw = NULL;
	struct atbm_vif *priv = NULL;
	struct altm_wext_msg vendor_msg;

	atbm_printk_debug("%s\n",__func__);

	if(dev == NULL){
		atbm_printk_err("atbm_wext_cmd() dev NULL\n");
		return -1;
	}

	sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	if(sdata == NULL){
		atbm_printk_err("error, netdev sdata NULL\n");
		return -1;
	}

	local = sdata->local;
	if(local == NULL){
		atbm_printk_err("error, netdev local NULL\n");
		return -1;
	}
	
	hw = &local->hw;
	if(hw == NULL){
		atbm_printk_err("error, netdev hw NULL\n");
		return -1;
	}
	
//	mutex_lock(&local->iflist_mtx);

	priv = (struct atbm_vif *)sdata->vif.drv_priv;
	if(priv == NULL){
		atbm_printk_err("error, netdev priv NULL\n");
		ret = -EINVAL;
		goto exit;
	}
	
	if (!rq->ifr_data) 
	{
		atbm_printk_err("error, netdev ifr->ifr_data == NULL\n");
		ret = -EINVAL;
		goto exit;
	}
	
	memset(&vendor_msg,0,sizeof(struct altm_wext_msg));
	
	if (copy_from_user(&vendor_msg, rq->ifr_data, sizeof(struct altm_msg))) 
	{
		atbm_printk_err("error, netdev copy_from_user\n");
		ret = -EFAULT;
		goto exit;
	}

	atbm_printk_debug("1,type = %d, value = %d\n", vendor_msg.type, vendor_msg.value);

	sdata->local->hw.vendcmd_nl80211 = 1;

	ret = atbm_wext_cmd(dev, &vendor_msg, sizeof(struct altm_wext_msg)); 
	atbm_printk_debug("2,type = %d, value = %d\n", vendor_msg.type, vendor_msg.value);
	if(ret != 0){
		atbm_printk_err("error, netdev atbm_wext_cmd\n");
		ret = -EFAULT;
		goto exit;
	}
	
	if (copy_to_user(rq->ifr_data,(u8*)(&vendor_msg),sizeof(vendor_msg)))
	{
		atbm_printk_err("error, netdev copy_to_user err\n");
		ret = -EFAULT;
		goto exit;
	}
	
	atbm_printk_debug("3,type = %d, value = %d\n", vendor_msg.type, vendor_msg.value);
	
exit:
	sdata->local->hw.vendcmd_nl80211 = 0;
//	mutex_unlock(&local->iflist_mtx);
	return ret; 

}
#endif
int ieee80211_netdev_cmd_handle_default(struct net_device *dev, 
	struct ifreq *rq)
{
	int ret = 0;
	atbm_printk_err( "%s\n",__func__);
	return ret;
}

typedef int (* ieee80211_netdev_cmd_handle)(struct net_device *dev, struct ifreq *rq);
ieee80211_netdev_cmd_handle ieee80211_netdev_cmd_handle_buff[]={
#ifdef CONFIG_ATBM_DEV_IOCTL
	ieee80211_netdev_process_wext_cmd,
#else
	ieee80211_netdev_cmd_handle_default,
#endif
#ifdef CONFIG_ATBM_IOCTRL_VENDOR_CMD
	ieee80211_netdev_process_android_cmd,
	ieee80211_netdev_process_vendor_cmd
#else
	ieee80211_netdev_cmd_handle_default,
	ieee80211_netdev_cmd_handle_default,
#endif
};

int ieee80211_netdev_ioctrl(struct net_device *dev, struct ifreq *rq, int cmd)
{	
	int ret = 0,cmd_value=0,i = 0;
	ieee80211_netdev_cmd_handle call_func = NULL;

	if((cmd > IEEE80211_NETDEV_VENDOR_CMD) 
		||
		(cmd < IEEE80211_NETDEV_BASE_CMD) )
	{
		cmd_value = list_data[0].cmd;
		do{
			if(cmd_value == cmd){
				atbm_printk_err( "ioctl:cmd[%s][0x%x] not support\n",list_data[i].cmd_str,cmd);
				break;
			}
			i++;
			cmd_value = list_data[i].cmd;
		}while(cmd_value != -1);
		
		ret = -EINVAL;

		goto exit;
	}
	call_func = IEEE80211_CMD_GET(ieee80211_netdev_cmd_handle,cmd - IEEE80211_NETDEV_BASE_CMD);
	ret = call_func(dev,rq);
	
exit:
	return ret;
}
#endif
#ifdef CONFIG_ATBM_SUPPORT_NET_TIMEOUT
static void ieee80211_tx_timeout(struct net_device *dev)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	unsigned long flags;
	u8 i;
	struct sk_buff *skb = atbm_dev_alloc_skb(0);
	
	spin_lock_irqsave(&local->queue_stop_reason_lock, flags);

	for (i = 0; i <local->hw.queues; i++)
		atbm_printk_debug("%s:queue[%d],reason[%ld]\n",__func__,i,local->queue_stop_reasons[i]);

	spin_unlock_irqrestore(&local->queue_stop_reason_lock, flags);
	
	#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 7, 0))
	dev->trans_start = jiffies;
	#endif

	if (unlikely(!skb))
		return;

	skb->pkt_type = IEEE80211_SDATA_QUEUE_FLUSH;
	atbm_skb_queue_tail(&sdata->skb_queue, skb);
	ieee80211_queue_work(&local->hw, &sdata->work);
}
#endif
static const struct net_device_ops ieee80211_dataif_ops = {
	.ndo_open		= ieee80211_open,
	.ndo_stop		= ieee80211_stop,
	.ndo_uninit		= ieee80211_sdata_uninit,
	.ndo_start_xmit		= ieee80211_subif_start_xmit,
	.ndo_set_rx_mode	= ieee80211_set_multicast_list,
	.ndo_change_mtu 	= ieee80211_change_mtu,
#ifdef CONFIG_ATBM_SUPPORT_REALTIME_CHANGE_MAC
	.ndo_set_mac_address 	= ieee80211_change_mac,
#endif
	.ndo_select_queue	= ieee80211_netdev_select_queue,
#if defined(CONFIG_ATBM_IOCTRL)
	.ndo_do_ioctl = ieee80211_netdev_ioctrl,
#endif
#ifdef CONFIG_ATBM_SUPPORT_NET_TIMEOUT
	.ndo_tx_timeout = ieee80211_tx_timeout,
#endif
};


#if (LINUX_VERSION_CODE > KERNEL_VERSION(5, 4, 0))
static u16 ieee80211_monitor_select_queue(struct net_device *dev,
										struct sk_buff *skb,	
										struct net_device *sb_dev)

#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 9, 37))
static u16 ieee80211_monitor_select_queue(struct net_device *dev,
										struct sk_buff *skb,
										void *accel_priv,
										select_queue_fallback_t fallback)


#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 4, 0))
static u16 ieee80211_monitor_select_queue(struct net_device *dev,
										struct sk_buff *skb,
										struct net_device *sb_dev,	
										select_queue_fallback_t fallback)
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 11, 0))
static u16 ieee80211_monitor_select_queue(struct net_device *dev,
										struct sk_buff *skb,
										void *accel_priv,
										select_queue_fallback_t fallback)
#else
static u16 ieee80211_monitor_select_queue(struct net_device *dev,struct sk_buff *skb)
#endif
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_hdr *hdr;
	struct ieee80211_radiotap_header *rtap = (void *)skb->data;
	u8 *p;

	if (local->hw.queues < IEEE80211_NUM_ACS)
		return 0;

	if (skb->len < 4 ||
	    skb->len < le16_to_cpu(rtap->it_len) + 2 /* frame control */)
		return 0; /* doesn't matter, frame will be dropped */

	hdr = (void *)((u8 *)skb->data + le16_to_cpu(rtap->it_len));

	if (!ieee80211_is_data(hdr->frame_control)) {
		skb->priority = 7;
		return ieee802_1d_to_ac[skb->priority];
	}
	if (!ieee80211_is_data_qos(hdr->frame_control)) {
		skb->priority = 0;
		return ieee802_1d_to_ac[skb->priority];
	}

	p = ieee80211_get_qos_ctl(hdr);
	skb->priority = *p & IEEE80211_QOS_CTL_TAG1D_MASK;

	return ieee80211_downgrade_queue(local, skb);
}

static const struct net_device_ops ieee80211_monitorif_ops = {
	.ndo_open		= ieee80211_open,
	.ndo_stop		= ieee80211_stop,
	.ndo_uninit		= ieee80211_teardown_sdata,
	.ndo_start_xmit		= ieee80211_monitor_start_xmit,
	.ndo_set_rx_mode	= ieee80211_set_multicast_list,
	.ndo_change_mtu 	= ieee80211_change_mtu,
	.ndo_set_mac_address 	= eth_mac_addr,
	.ndo_select_queue	= ieee80211_monitor_select_queue,
#if defined(CONFIG_ATBM_IOCTRL)
	.ndo_do_ioctl = ieee80211_netdev_ioctrl,
#endif
};
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0))
static void ieee80211_if_free(struct net_device *dev)
{
        free_percpu(dev->tstats);
}
#endif

static void ieee80211_if_setup(struct net_device *dev)
{
	ether_setup(dev);
	dev->priv_flags &= ~IFF_TX_SKB_SHARING;
	netdev_attach_ops(dev, &ieee80211_dataif_ops);
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,29))
	/* Do we need this ? */
	/* we will validate the address ourselves in ->open */
	dev->validate_addr = NULL;
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0))
	dev->needs_free_netdev = true;
    dev->priv_destructor = ieee80211_if_free;
#else
	dev->destructor = free_netdev;
#endif
}

static void ieee80211_iface_work(struct atbm_work_struct *work)
{
	struct ieee80211_sub_if_data *sdata =
		container_of(work, struct ieee80211_sub_if_data, work);
	struct ieee80211_local *local = sdata->local;
	struct sk_buff *skb;
	struct sta_info *sta;
#ifdef CONFIG_ATBM_SW_AGGTX
	struct ieee80211_ra_tid *ra_tid;
#endif

	if (!ieee80211_sdata_running(sdata))
		return;

	if (local->scanning)
		return;

	/*
	 * ieee80211_queue_work() should have picked up most cases,
	 * here we'll pick the rest.
	 */
	if (WARN(local->suspended,"going to suspend\n"))
		return;

	/* first process frames */
	while ((skb = atbm_skb_dequeue(&sdata->skb_queue))) {
		struct atbm_ieee80211_mgmt *mgmt = (void *)skb->data;
		if(0){}
#ifdef CONFIG_ATBM_STA_DYNAMIC_PS
		else if(skb->pkt_type == IEEE80211_SDATA_IDLE_RECAL){
			atbm_printk_debug("%s: idle recal\n",sdata->name);
			ieee80211_start_ps_recal_work(sdata);
		}
#endif
#ifdef CONFIG_ATBM_SW_AGGTX
		else if (skb->pkt_type == IEEE80211_SDATA_QUEUE_AGG_START) {
			ra_tid = (void *)&skb->cb;
			ieee80211_start_tx_ba_cb(&sdata->vif, ra_tid->ra,
						 ra_tid->tid);
		} else if (skb->pkt_type == IEEE80211_SDATA_QUEUE_AGG_STOP) {
			ra_tid = (void *)&skb->cb;
			ieee80211_stop_tx_ba_cb(&sdata->vif, ra_tid->ra,
						ra_tid->tid);
		}
#endif
#ifdef CONFIG_ATBM_SUPPORT_NET_TIMEOUT
		else if(skb->pkt_type == IEEE80211_SDATA_QUEUE_FLUSH){
			u8 i;
			unsigned long flags;
			atbm_printk_debug("%s:flush queue\n",__func__);
			drv_flush(local,sdata,true);
			spin_lock_irqsave(&local->queue_stop_reason_lock, flags);

			for (i = 0; i <local->hw.queues; i++)
				atbm_printk_debug("%s:queue[%d],reason[%ld]\n",__func__,i,local->queue_stop_reasons[i]);

			spin_unlock_irqrestore(&local->queue_stop_reason_lock, flags);
		}
#endif
		else if (ieee80211_is_action(mgmt->frame_control) &&
			   mgmt->u.action.category == ATBM_WLAN_CATEGORY_BACK) {
			int len = skb->len;

			mutex_lock(&local->sta_mtx);
			sta = sta_info_get_bss(sdata, mgmt->sa);
			if (sta) {
				switch (mgmt->u.action.u.addba_req.action_code) {
				case WLAN_ACTION_ADDBA_REQ:
					ieee80211_process_addba_request(
							local, sta, mgmt, len);
					break;
				case WLAN_ACTION_ADDBA_RESP:
#ifdef CONFIG_ATBM_SW_AGGTX
					ieee80211_process_addba_resp(local, sta,
								     mgmt, len);
#endif
					break;
				case WLAN_ACTION_DELBA:
					ieee80211_process_delba(sdata, sta,
								mgmt, len);
					break;
				default:
					atbm_printk_err("%s:%d\n",__func__,mgmt->u.action.u.addba_req.action_code);
					WARN_ON(1);
					break;
				}
			}
			mutex_unlock(&local->sta_mtx);
		} 
#if 0
		else if (ieee80211_is_data_qos(mgmt->frame_control)) {
			WARN_ON(1);
			/*
			*here can not stop rx agg,because lmac will handle the agg process
			*/
#if 0
			struct ieee80211_hdr *hdr = (void *)mgmt;
			/*
			 * So the frame isn't mgmt, but frame_control
			 * is at the right place anyway, of course, so
			 * the if statement is correct.
			 *
			 * Warn if we have other data frame types here,
			 * they must not get here.
			 */
			WARN_ON(hdr->frame_control &
					cpu_to_le16(IEEE80211_STYPE_NULLFUNC));
			WARN_ON(!(hdr->seq_ctrl &
					cpu_to_le16(IEEE80211_SCTL_FRAG)));
			/*
			 * This was a fragment of a frame, received while
			 * a block-ack session was active. That cannot be
			 * right, so terminate the session.
			 */
			mutex_lock(&local->sta_mtx);
			sta = sta_info_get_bss(sdata, mgmt->sa);
			if (sta) {
				u16 tid = *ieee80211_get_qos_ctl(hdr) &
						IEEE80211_QOS_CTL_TID_MASK;

				__ieee80211_stop_rx_ba_session(
					sta, tid, WLAN_BACK_RECIPIENT,
					WLAN_REASON_QSTA_REQUIRE_SETUP,
					true);
			}
			mutex_unlock(&local->sta_mtx);
#endif
		} 
#endif
	else {
			switch (sdata->vif.type) {
			case NL80211_IFTYPE_AP:
			case NL80211_IFTYPE_P2P_GO:
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,3,0))
				if (ieee80211_is_beacon(mgmt->frame_control)) {
					struct ieee80211_bss_conf *bss_conf =
						&sdata->vif.bss_conf;
					u32 bss_info_changed = 0, erp = 0;
					#if 0
					struct ieee80211_channel *channel =
						ieee80211_get_channel(local->hw.wiphy,
								IEEE80211_SKB_RXCB(skb)->freq);
					if(cfg80211_inform_bss_frame(local->hw.wiphy, channel,
							(struct ieee80211_mgmt *)mgmt, skb->len,
							IEEE80211_SKB_RXCB(skb)->signal, GFP_ATOMIC))
					
					erp = cfg80211_get_local_erp(local->hw.wiphy,
							IEEE80211_SKB_RXCB(skb)->freq);
					#else
					{
						const u8 *p;
						int tmp = 0;
						p = cfg80211_find_ie(ATBM_WLAN_EID_SUPP_RATES,
						mgmt->u.beacon.variable,skb->len-
						offsetof(struct atbm_ieee80211_mgmt,u.probe_resp.variable));
						if (p) {
							/* Check for pure 11b access poin */
							if (p[1] == 4)
								erp |= WLAN_ERP_USE_PROTECTION;
						}

						p = cfg80211_find_ie(ATBM_WLAN_EID_ERP_INFO,
								mgmt->u.beacon.variable, skb->len-
						offsetof(struct atbm_ieee80211_mgmt,u.probe_resp.variable));
						if (!p){
							atbm_kfree_skb(skb);
							continue;
						}
						tmp = p[ERP_INFO_BYTE_OFFSET];
						erp |= tmp;
					}

					#endif
					if (!!bss_conf->use_cts_prot !=
							!!(erp & WLAN_ERP_USE_PROTECTION)) {
						bss_conf->use_cts_prot =
							!!(erp & WLAN_ERP_USE_PROTECTION);
						bss_info_changed |= BSS_CHANGED_ERP_CTS_PROT;
					}
					ieee80211_bss_info_change_notify(sdata,
							bss_info_changed);
				}else
#endif
				ieee80211_ap_rx_queued_mgmt_special(sdata, skb);		
				break;
			case NL80211_IFTYPE_STATION:
				ieee80211_sta_rx_queued_mgmt(sdata, skb);
				break;
#ifdef CONFIG_ATBM_SUPPORT_IBSS
			case NL80211_IFTYPE_ADHOC:
				ieee80211_ibss_rx_queued_mgmt(sdata, skb);
				break;
#endif
#ifdef CONFIG_MAC80211_ATBM_MESH
			case NL80211_IFTYPE_MESH_POINT:
				if (!ieee80211_vif_is_mesh(&sdata->vif))
					break;
				ieee80211_mesh_rx_queued_mgmt(sdata, skb);
				break;
#endif
			default:
				WARN(1, "for unkown if");
				break;
			}
#ifdef CONFIG_IEEE80211_SPECIAL_FILTER
			ieee80211_special_check_package(local,skb); 	
#endif
		}
		atbm_kfree_skb(skb);
	}

	/* then other type-dependent work */
	switch (sdata->vif.type) {
	case NL80211_IFTYPE_STATION:
#ifdef CONFIG_ATBM_MAC80211_NO_USE
		ieee80211_sta_work(sdata);
#endif
		break;
#ifdef CONFIG_ATBM_SUPPORT_IBSS
	case NL80211_IFTYPE_ADHOC:
		ieee80211_ibss_work(sdata);
		break;
#endif
#ifdef CONFIG_MAC80211_ATBM_MESH
	case NL80211_IFTYPE_MESH_POINT:
		if (!ieee80211_vif_is_mesh(&sdata->vif))
			break;
		ieee80211_mesh_work(sdata);
		break;
#endif
	default:
		break;
	}
}

static void ieee80211_reconfig_filter(struct atbm_work_struct *work)
{
	struct ieee80211_sub_if_data *sdata =
		container_of(work, struct ieee80211_sub_if_data,
			     reconfig_filter);

	ieee80211_configure_filter(sdata);
}

/*
 * Helper function to initialise an interface to a specific type.
 */
static void ieee80211_setup_sdata(struct ieee80211_sub_if_data *sdata,
				  enum nl80211_iftype type)
{
	/* clear type-dependent union */
	memset(&sdata->u, 0, sizeof(sdata->u));
	atomic_set(&sdata->connectting,IEEE80211_ATBM_CONNECT_DONE);
#ifdef CONFIG_MAC80211_ATBM_ROAMING_CHANGES
	sdata->queues_locked = 0;
#endif
	/* and set some type-dependent values */
	sdata->vif.type = type;
	sdata->vif.p2p = false;
	netdev_attach_ops(sdata->dev, &ieee80211_dataif_ops);
	sdata->wdev.iftype = type;

	sdata->control_port_protocol = cpu_to_be16(ETH_P_PAE);
	sdata->control_port_no_encrypt = false;

	/* only monitor differs */
	sdata->dev->type = ARPHRD_ETHER;

	atbm_skb_queue_head_init(&sdata->skb_queue);
	ATBM_INIT_WORK(&sdata->work, ieee80211_iface_work);
	ATBM_INIT_WORK(&sdata->reconfig_filter, ieee80211_reconfig_filter);
#ifdef ATBM_AP_SME
	ieee80211_ap_sme_queue_mgmt_init(sdata);
	ieee80211_ap_sme_event_init(sdata);
#endif
	ieee80211_special_filter_init(sdata);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,35))

	__hw_addr_init(&sdata->mc_list);

#endif


	/*
	 * Initialize wiphy parameters to IEEE 802.11 MIB default values.
	 * RTS threshold is disabled by default with the special -1 value.
	 */
	sdata->vif.bss_conf.retry_short = sdata->wdev.wiphy->retry_short = 7;
	sdata->vif.bss_conf.retry_long =  sdata->wdev.wiphy->retry_long = 4;
	sdata->vif.bss_conf.rts_threshold =  sdata->wdev.wiphy->rts_threshold = (u32) -1;

	switch (type) {
#ifdef CONFIG_ATBM_SUPPORT_P2P
	case NL80211_IFTYPE_P2P_GO:
		type = NL80211_IFTYPE_AP;
		sdata->vif.type = type;
		sdata->vif.p2p = true;
#if (LINUX_VERSION_CODE > KERNEL_VERSION(5, 10, 60))
		
			fallthrough;
#endif
		/* fall through */
#endif
	case NL80211_IFTYPE_AP:
		atbm_skb_queue_head_init(&sdata->u.ap.ps_bc_buf);
		INIT_LIST_HEAD(&sdata->u.ap.vlans);
		sdata->local->uapsd_queues = IEEE80211_DEFAULT_UAPSD_QUEUES;
		break;
#ifdef CONFIG_ATBM_SUPPORT_P2P
	case NL80211_IFTYPE_P2P_CLIENT:
		type = NL80211_IFTYPE_STATION;
		sdata->vif.type = type;
		sdata->vif.p2p = true;
#if (LINUX_VERSION_CODE > KERNEL_VERSION(5, 10, 60))
		
			fallthrough;
#endif
		/* fall through */
#endif
	case NL80211_IFTYPE_STATION:
		ieee80211_sta_setup_sdata(sdata);
		break;
#ifdef CONFIG_ATBM_SUPPORT_IBSS
	case NL80211_IFTYPE_ADHOC:
		ieee80211_ibss_setup_sdata(sdata);
		break;
#endif
#ifdef CONFIG_MAC80211_ATBM_MESH
	case NL80211_IFTYPE_MESH_POINT:
		if (ieee80211_vif_is_mesh(&sdata->vif))
			ieee80211_mesh_init_sdata(sdata);
		break;
#endif
	case NL80211_IFTYPE_MONITOR:
		{
			#ifdef	ATBM_WIFI_QUEUE_LOCK_BUG
			struct ieee80211_sub_if_data *attach_sdata = NULL;
			
			list_for_each_entry(attach_sdata, &sdata->local->interfaces, list){
				if(attach_sdata == sdata)
					continue;
				if(strstr(sdata->name,attach_sdata->name) == NULL)
					continue;
				atbm_printk_debug("%s:add[%s],attach[%s]\n",__func__,sdata->name,attach_sdata->name);
				memcpy(sdata->vif.hw_queue,attach_sdata->vif.hw_queue,IEEE80211_NUM_ACS);
			}
			#endif
			/*
			*when hostapd setups ,it will add a new net devie called mon.wlan0 or mon.p2p0.
			*at this time we set sdata->raw_dev false ,and the new devie enter RADIOTAP mode
			*/
			/*
			*iwconifg can change wlan0 or p2p0 into monitor time,but some app needs prism header
			*,others need radiotap header.
			*
			*/
#ifdef CONFIG_ATBM_MONITOR_HDR_PRISM
			if(sdata->local->hw.flags & IEEE80211_HW_MONITOR_NEED_PRISM_HEADER){
				sdata->dev->type = ARPHRD_IEEE80211_PRISM;
			}else 
#endif
			{
				sdata->dev->type = ARPHRD_IEEE80211_RADIOTAP;
			}
			netdev_attach_ops(sdata->dev, &ieee80211_monitorif_ops);
			sdata->u.mntr_flags = MONITOR_FLAG_CONTROL |
					      MONITOR_FLAG_OTHER_BSS;
		}
		break;
	case NL80211_IFTYPE_WDS:
	case NL80211_IFTYPE_AP_VLAN:
		break;
	case NL80211_IFTYPE_UNSPECIFIED:
	case NUM_NL80211_IFTYPES:
		BUG();
		break;
	default:
		break;
	}

	ieee80211_debugfs_add_netdev(sdata);

	drv_bss_info_changed(sdata->local, sdata, &sdata->vif.bss_conf,
			     BSS_CHANGED_RETRY_LIMITS);
}

/*
 * Helper function to initialise an interface to a specific type.
 */
static void ieee80211_delete_sdata(struct ieee80211_sub_if_data *sdata)
{
	switch (sdata->vif.type) {
	case NL80211_IFTYPE_P2P_GO:
	case NL80211_IFTYPE_AP:
		break;
		
	case NL80211_IFTYPE_P2P_CLIENT:
	case NL80211_IFTYPE_STATION:
		{
#if defined (CONFIG_ATBM_SUPPORT_CHANSWITCH) || defined (CONFIG_ATBM_MAC80211_NO_USE)
		struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;	
#endif
#ifdef CONFIG_ATBM_MAC80211_NO_USE		
		atbm_del_timer_sync(&ifmgd->bcn_mon_timer);
		atbm_del_timer_sync(&ifmgd->conn_mon_timer);
		atbm_del_timer_sync(&ifmgd->timer);
#endif
#ifdef CONFIG_ATBM_SUPPORT_CHANSWITCH
		atbm_del_timer_sync(&ifmgd->chswitch_timer);
#endif
		}
		break;
	default:
		break;
	}
}

static int ieee80211_runtime_change_iftype(struct ieee80211_sub_if_data *sdata,
					   enum nl80211_iftype type)
{
	struct ieee80211_local *local = sdata->local;
	int ret, err;
	enum nl80211_iftype internal_type = type;
	bool p2p = false;

	ASSERT_RTNL();

	if (!local->ops->change_interface)
		return -EBUSY;

	switch (sdata->vif.type) {
	case NL80211_IFTYPE_AP:
	case NL80211_IFTYPE_STATION:
#ifdef CONFIG_ATBM_SUPPORT_IBSS
	case NL80211_IFTYPE_ADHOC:
#endif
		/*
		 * Could maybe also all others here?
		 * Just not sure how that interacts
		 * with the RX/config path e.g. for
		 * mesh.
		 */
		break;
	case NL80211_IFTYPE_MONITOR:
		if(local->only_monitors)
			break;
		else 
			return -EBUSY;
	default:
		return -EBUSY;
	}

	switch (type) {
	case NL80211_IFTYPE_AP:
	case NL80211_IFTYPE_STATION:
#ifdef CONFIG_ATBM_SUPPORT_IBSS
	case NL80211_IFTYPE_ADHOC:
#endif
		/*
		 * Could probably support everything
		 * but WDS here (WDS do_open can fail
		 * under memory pressure, which this
		 * code isn't prepared to handle).
		 */
		break;
	case NL80211_IFTYPE_MONITOR:
		if(local->only_monitors == 0)
			break;
		else 
			return -EBUSY;
#ifdef CONFIG_ATBM_SUPPORT_P2P
	case NL80211_IFTYPE_P2P_CLIENT:
		p2p = true;
		internal_type = NL80211_IFTYPE_STATION;
		break;
	case NL80211_IFTYPE_P2P_GO:
		p2p = true;
		internal_type = NL80211_IFTYPE_AP;
		break;
#endif
	default:
		return -EBUSY;
	}

	ret = ieee80211_check_concurrent_iface(sdata, internal_type);
	if (ret)
		return ret;

	ieee80211_do_stop(sdata, false);

	ieee80211_teardown_sdata(sdata->dev);

	ret = drv_change_interface(local, sdata, internal_type, p2p);
	if (ret)
		type = sdata->vif.type;

	/*
	 * Ignore return value here, there's not much we can do since
	 * the driver changed the interface type internally already.
	 * The warnings will hopefully make driver authors fix it :-)
	 */
	ieee80211_check_queues(sdata);

	ieee80211_setup_sdata(sdata, type);

	err = ieee80211_do_open(sdata->dev, false);
	WARN_ON(err);
	return ret;
}

int ieee80211_if_change_type(struct ieee80211_sub_if_data *sdata,
			     enum nl80211_iftype type)
{
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(sdata->local, sdata);
	int ret;

	ASSERT_RTNL();

	atbm_printk_err("[%s] change to [%d]\n",sdata->name,type);
	if (type == ieee80211_vif_type_p2p(&sdata->vif))
		return 0;

	/* Setting ad-hoc mode on non-IBSS channel is not supported. */
#ifdef CONFIG_ATBM_SUPPORT_IBSS
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,11,0))
	if (chan_state->oper_channel->flags & IEEE80211_CHAN_NO_IBSS &&
	    type == NL80211_IFTYPE_ADHOC)
		return -EOPNOTSUPP;
#endif
#endif
	if(sdata->local->only_monitors){
		if(sdata->local->monitor_sdata == sdata){
			atbm_printk_err( "[%s] in monitor mode,change to other mode\n",sdata->local->monitor_sdata->name);
		}else {
			atbm_printk_err( "[%s] in monitor mode,so [%s] can not change\n",
				sdata->local->monitor_sdata->name,sdata->name);
		}
	}
	else if(type == NL80211_IFTYPE_MONITOR){
		struct ieee80211_sub_if_data *other_sdata;
		list_for_each_entry(other_sdata, &sdata->local->interfaces, list){
			
			 if ((other_sdata->vif.type != NL80211_IFTYPE_MONITOR)&&
			 	ieee80211_sdata_running(other_sdata)&&(other_sdata != sdata)){
			 	atbm_printk_err("[%s] is running,so please close [%s]\n",other_sdata->name,other_sdata->name);
				atbm_printk_err("and try again later to set[%s] to monitor mode\n",sdata->name);
				return -EOPNOTSUPP;
			 }
		}
	}
	if (ieee80211_sdata_running(sdata)) {
		ret = ieee80211_runtime_change_iftype(sdata, type);
		if (ret)
			return ret;
	} else {
		/* Purge and reset type-dependent state. */
		ieee80211_teardown_sdata(sdata->dev);
		ieee80211_setup_sdata(sdata, type);
	}

	/* reset some values that shouldn't be kept across type changes */
	sdata->vif.bss_conf.basic_rates =
		ieee80211_atbm_mandatory_rates(sdata->local,
			chan_state->conf.channel->band);
	sdata->drop_unencrypted = 0;
	if (type == NL80211_IFTYPE_STATION)
		sdata->u.mgd.use_4addr = false;

	return 0;
}

static void ieee80211_assign_perm_addr(struct ieee80211_local *local,
				       struct net_device *dev,
				       enum nl80211_iftype type)
{
	struct ieee80211_sub_if_data *sdata;
	int i;

	/* default ... something at least */
	memcpy(dev->perm_addr, local->hw.wiphy->perm_addr, ETH_ALEN);

	if (is_zero_ether_addr(local->hw.wiphy->addr_mask) &&
	    local->hw.wiphy->n_addresses <= 1)
		return;


	mutex_lock(&local->iflist_mtx);

	switch (type) {
	case NL80211_IFTYPE_WDS:
	case NL80211_IFTYPE_AP_VLAN:
		/* match up with an AP interface */
		list_for_each_entry(sdata, &local->interfaces, list) {
			if (sdata->vif.type != NL80211_IFTYPE_AP)
				continue;
			memcpy(dev->perm_addr, sdata->vif.addr, ETH_ALEN);
			break;
		}
		/* keep default if no AP interface present */
		break;
	case NL80211_IFTYPE_MONITOR:
		/* doesn't matter */
		/*
		*some time monitor needs mac
		*/
	default:
		/* assign a new address if possible -- try n_addresses first */
		for (i = 0; i < local->hw.wiphy->n_addresses; i++) {
			bool used = false;

			list_for_each_entry(sdata, &local->interfaces, list) {
				if (memcmp(local->hw.wiphy->addresses[i].addr,
					   sdata->vif.addr, ETH_ALEN) == 0) {
					if((sdata->vif.type == NL80211_IFTYPE_MONITOR) && 
				   	   (sdata->u.mntr_flags & MONITOR_FLAG_COOK_FRAMES)){
					   atbm_printk_err("[%s] in cook monitor mode,we can use that addr\n",
					   	sdata->name);
					   continue;
					}
					used = true;
					break;
				}
			}

			if (!used) {
				memcpy(dev->perm_addr,
				       local->hw.wiphy->addresses[i].addr,
				       ETH_ALEN);
				break;
			}
		}

		
#if 1
		/* try mask if available */
		if (is_zero_ether_addr(local->hw.wiphy->addr_mask))
			break;
		WARN_ON(1);
#else

		m = local->hw.wiphy->addr_mask;
		mask =	((u64)m[0] << 5*8) | ((u64)m[1] << 4*8) |
			((u64)m[2] << 3*8) | ((u64)m[3] << 2*8) |
			((u64)m[4] << 1*8) | ((u64)m[5] << 0*8);

		if (__ffs64(mask) + hweight64(mask) != fls64(mask)) {
			/* not a contiguous mask ... not handled now! */
			atbm_printk_err("not contiguous\n");
			break;
		}

		m = local->hw.wiphy->perm_addr;
		start = ((u64)m[0] << 5*8) | ((u64)m[1] << 4*8) |
			((u64)m[2] << 3*8) | ((u64)m[3] << 2*8) |
			((u64)m[4] << 1*8) | ((u64)m[5] << 0*8);

		inc = 1ULL<<__ffs64(mask);
		val = (start & mask);
		addr = (start & ~mask) | (val & mask);
		do {
			bool used = false;

			tmp_addr[5] = addr >> 0*8;
			tmp_addr[4] = addr >> 1*8;
			tmp_addr[3] = addr >> 2*8;
			tmp_addr[2] = addr >> 3*8;
			tmp_addr[1] = addr >> 4*8;
			tmp_addr[0] = addr >> 5*8;

			val += inc;

			list_for_each_entry(sdata, &local->interfaces, list) {
				if (memcmp(tmp_addr, sdata->vif.addr,
							ETH_ALEN) == 0) {
					used = true;
					break;
				}
			}

			if (!used) {
				memcpy(dev->perm_addr, tmp_addr, ETH_ALEN);
				break;
			}
			addr = (start & ~mask) | (val & mask);
		} while (addr != start);

		break;
#endif
	}

	mutex_unlock(&local->iflist_mtx);
}
#ifdef CONFIG_WIRELESS_EXT
extern struct iw_handler_def atbm_handlers_def;
#endif
int ieee80211_if_add(struct ieee80211_local *local, const char *name,
		     struct net_device **new_dev, enum nl80211_iftype type,
		     struct vif_params *params)
{
	struct net_device *ndev;
	struct ieee80211_sub_if_data *sdata = NULL;
	int ret, i;
	int txqs = 1;

	ASSERT_RTNL();

	if (local->hw.queues >= IEEE80211_NUM_ACS)
		txqs = IEEE80211_NUM_ACS;
//#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,18,25))
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,18,00))
	ndev = alloc_netdev_mqs(sizeof(*sdata) + local->hw.vif_data_size,
				name,NET_NAME_UNKNOWN, ieee80211_if_setup, txqs, 1);
#else	
	ndev = alloc_netdev_mqs(sizeof(*sdata) + local->hw.vif_data_size,
				name, ieee80211_if_setup, txqs, 1);
#endif
	if (!ndev)
		return -ENOMEM;
	dev_net_set(ndev, wiphy_net(local->hw.wiphy));
#ifdef CHKSUM_HW_SUPPORT
	ndev->hw_features = (NETIF_F_RXCSUM|NETIF_F_IP_CSUM);
	ndev->features |= ndev->hw_features;
	atbm_printk_init("+++++++++++++++++++++++++++++++hw checksum open ++++++++++++++++++++\n");
#endif

#ifdef CONFIG_ATBM_SUPPORT_SG
	atbm_printk_init("enable sg\n");
	ndev->hw_features |= NETIF_F_SG;
	ndev->features    |= NETIF_F_SG;
#endif
/* This is an optimization, just ignore for older kernels */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,26))
	ndev->needed_headroom = local->tx_headroom +
				4*6 /* four MAC addresses */
				+ 2 + 2 + 2 + 2 /* ctl, dur, seq, qos */
				+ 6 /* mesh */
				+ 8 /* rfc1042/bridge tunnel */
				- ETH_HLEN /* ethernet hard_header_len */
				+ IEEE80211_ENCRYPT_HEADROOM;
	ndev->needed_tailroom = IEEE80211_ENCRYPT_TAILROOM;
#endif

	ret = dev_alloc_name(ndev, ndev->name);
	if (ret < 0)
		goto fail;

	ieee80211_assign_perm_addr(local, ndev, type);
	memcpy(ndev->dev_addr, ndev->perm_addr, ETH_ALEN);
	SET_NETDEV_DEV(ndev, wiphy_dev(local->hw.wiphy));

	/* don't use IEEE80211_DEV_TO_SUB_IF because it checks too much */
	sdata = netdev_priv(ndev);
	ndev->ieee80211_ptr = &sdata->wdev;
	memcpy(sdata->vif.addr, ndev->dev_addr, ETH_ALEN);
	memcpy(sdata->name, ndev->name, IFNAMSIZ);

	/* initialise type-independent data */
	sdata->wdev.wiphy = local->hw.wiphy;
	sdata->local = local;
	sdata->dev = ndev;
#ifdef CONFIG_INET
	sdata->arp_filter_state = true;
#ifdef IPV6_FILTERING
	sdata->ndp_filter_state = true;
#endif /*IPV6_FILTERING*/
#endif

	for (i = 0; i < IEEE80211_FRAGMENT_MAX; i++)
		atbm_skb_queue_head_init(&sdata->fragments[i].skb_list);

	INIT_LIST_HEAD(&sdata->key_list);

	for (i = 0; i < IEEE80211_NUM_BANDS; i++) {
		struct ieee80211_supported_band *sband;
		sband = local->hw.wiphy->bands[i];
		sdata->rc_rateidx_mask[i] =
			sband ? (1 << sband->n_bitrates) - 1 : 0;

		if (!sdata->chan_state.oper_channel) {
			/* init channel we're on */
			/* soumik: set default channel to non social channel */
			sdata->chan_state.conf.channel =
			/* sdata->chan_state.oper_channel = &sband->channels[0]; */
			sdata->chan_state.oper_channel = &sband->channels[2];
			sdata->chan_state.conf.channel_type = NL80211_CHAN_NO_HT;
		}
	}

	
#ifdef CONFIG_ATBM_AP_CHANNEL_CHANGE_EVENT
	ATBM_INIT_DELAYED_WORK(&sdata->ap_channel_event_work,ieee80211_ap_channel_event_work);
#endif

	sdata->dynamic_ps_forced_timeout = -1;
#ifdef	CONFIG_ATBM_RADAR_DETECT
#ifdef CONFIG_ATBM_5G_PRETEND_2G
	ATBM_INIT_DELAYED_WORK(&sdata->dfs_cac_timer_work,
			  ieee80211_dfs_cac_timer_work);
#endif
#endif
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	ATBM_INIT_WORK(&sdata->dynamic_ps_enable_work,
		  ieee80211_dynamic_ps_enable_work);
	ATBM_INIT_WORK(&sdata->dynamic_ps_disable_work,
		  ieee80211_dynamic_ps_disable_work);
	atbm_setup_timer(&sdata->dynamic_ps_timer,
		    ieee80211_dynamic_ps_timer, (unsigned long) sdata);
#endif
	/*
	*medium traffic init
	*/
	ieee80211_medium_traffic_init(sdata);
	
	sdata->vif.bss_conf.listen_interval = local->hw.max_listen_interval;

	ieee80211_set_default_queues(sdata);
	
	/* setup type-dependent data */
	ieee80211_setup_sdata(sdata, type);

	if (params) {
		ndev->ieee80211_ptr->use_4addr = params->use_4addr;
		if (type == NL80211_IFTYPE_STATION)
			sdata->u.mgd.use_4addr = params->use_4addr;
	}
#ifdef CONFIG_WIRELESS_EXT
    ndev->wireless_handlers = &atbm_handlers_def;
#endif
#ifdef CONFIG_ATBM_SUPPORT_NET_TIMEOUT
	ndev->watchdog_timeo = msecs_to_jiffies(100);
#endif
	ret = register_netdevice(ndev);
	if (ret)
		goto fail;

	mutex_lock(&local->iflist_mtx);
	list_add_tail_rcu(&sdata->list, &local->interfaces);
	mutex_unlock(&local->iflist_mtx);
#ifdef CONFIG_MAC80211_BRIDGE
	br0_attach(sdata);
#endif //CONFIG_MAC80211_BRIDGE
	if (new_dev)
		*new_dev = ndev;

	return 0;

 fail:
	free_netdev(ndev);
	return ret;
}

void ieee80211_if_remove(struct ieee80211_sub_if_data *sdata)
{
	ASSERT_RTNL();

	atbm_cancel_work_sync(&sdata->reconfig_filter);
	atbm_cancel_work_sync(&sdata->work);
	
#ifdef CONFIG_MAC80211_BRIDGE
	br0_detach(sdata);
#endif //CONFIG_MAC80211_BRIDGE

	mutex_lock(&sdata->local->iflist_mtx);
	list_del_rcu(&sdata->list);
	mutex_unlock(&sdata->local->iflist_mtx);
	ieee80211_delete_sdata(sdata);
#ifdef CONFIG_MAC80211_ATBM_MESH
	if (ieee80211_vif_is_mesh(&sdata->vif))
		mesh_path_flush_by_iface(sdata);
#endif
	synchronize_rcu();
	unregister_netdevice(sdata->dev);
}

/*
 * Remove all interfaces, may only be called at hardware unregistration
 * time because it doesn't do RCU-safe list removals.
 */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,65) || (ATBM_WIFI_PLATFORM == 10))
 void ieee80211_remove_interfaces(struct ieee80211_local *local)
 {
	 struct ieee80211_sub_if_data *sdata, *tmp;
	 LIST_HEAD(unreg_list);
	 LIST_HEAD(wdev_list);
 
	 ASSERT_RTNL();
 
	 /*
	  * Close all AP_VLAN interfaces first, as otherwise they
	  * might be closed while the AP interface they belong to
	  * is closed, causing unregister_netdevice_many() to crash.
	  */
	 list_for_each_entry(sdata, &local->interfaces, list)
		 if (sdata->vif.type == NL80211_IFTYPE_AP_VLAN)
			 dev_close(sdata->dev);
 
	 /*
	  * Close all AP_VLAN interfaces first, as otherwise they
	  * might be closed while the AP interface they belong to
	  * is closed, causing unregister_netdevice_many() to crash.
	  */
	 list_for_each_entry(sdata, &local->interfaces, list)
		 if (sdata->vif.type == NL80211_IFTYPE_AP_VLAN)
			 dev_close(sdata->dev);
	 mutex_lock(&local->iflist_mtx);
	 list_for_each_entry_safe(sdata, tmp, &local->interfaces, list) {
		 list_del(&sdata->list);
 
#ifdef CONFIG_MAC80211_BRIDGE
	    br0_detach(sdata);
#endif //CONFIG_MAC80211_BRIDGE
		 if (sdata->dev)
			 unregister_netdevice_queue(sdata->dev, &unreg_list);
		 else
			 list_add(&sdata->list, &wdev_list);
	 }
	 mutex_unlock(&local->iflist_mtx);
	 unregister_netdevice_many(&unreg_list);
	 list_for_each_entry_safe(sdata, tmp, &wdev_list, list) {
		 list_del(&sdata->list);
		 cfg80211_unregister_wdev(&sdata->wdev);
		 atbm_kfree(sdata);
	 }
 }

#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,33)&& LINUX_VERSION_CODE < KERNEL_VERSION(3,10,65))
void ieee80211_remove_interfaces(struct ieee80211_local *local)
{
	struct ieee80211_sub_if_data *sdata, *tmp;
	LIST_HEAD(unreg_list);

	ASSERT_RTNL();

	mutex_lock(&local->iflist_mtx);
	list_for_each_entry_safe(sdata, tmp, &local->interfaces, list) {
		list_del(&sdata->list);
#ifdef CONFIG_MAC80211_BRIDGE
		br0_detach(sdata);
#endif //CONFIG_MAC80211_BRIDGE
#ifdef CONFIG_MAC80211_ATBM_MESH
		if (ieee80211_vif_is_mesh(&sdata->vif))
			mesh_path_flush_by_iface(sdata);
#endif

		unregister_netdevice_queue(sdata->dev, &unreg_list);
	}
	mutex_unlock(&local->iflist_mtx);
	unregister_netdevice_many(&unreg_list);
	list_del(&unreg_list);
}
#else
void ieee80211_remove_interfaces(struct ieee80211_local *local)
{
	struct ieee80211_sub_if_data *sdata, *tmp;

	ASSERT_RTNL();
	

	list_for_each_entry_safe(sdata, tmp, &local->interfaces, list) {
		mutex_lock(&local->iflist_mtx);
		list_del(&sdata->list);
		mutex_unlock(&local->iflist_mtx);
#ifdef CONFIG_MAC80211_BRIDGE
		br0_detach(sdata);
#endif //CONFIG_MAC80211_BRIDGE

		unregister_netdevice(sdata->dev);
	}
}
#endif
#define IEEE80211_ATBM_IDLE_OFF_AUTHENING					BIT(0)
#define IEEE80211_ATBM_IDLE_OFF_REMAIN_ON_CHANNEL			BIT(1)
#define IEEE80211_ATBM_IDLE_OFF_WORKING						BIT(2)
#define IEEE80211_ATBM_IDLE_OFF_SCANNING					BIT(3)
#define IEEE80211_ATBM_IDLE_OFF_OTHERS						BIT(4)

static u32 ieee80211_idle_off(struct ieee80211_local *local,
			      int reason)
{
	if (!(local->hw.conf.flags & IEEE80211_CONF_IDLE))
		return 0;

#ifdef CONFIG_MAC80211_ATBM_VERBOSE_DEBUG
	wiphy_debug(local->hw.wiphy, "device no longer idle - %s\n", reason);
#endif

	local->hw.conf.flags &= ~IEEE80211_CONF_IDLE;
	return IEEE80211_CONF_CHANGE_IDLE;
}

static u32 ieee80211_idle_on(struct ieee80211_local *local)
{
	struct ieee80211_sub_if_data *sdata;
	if (local->hw.conf.flags & IEEE80211_CONF_IDLE)
		return 0;

#ifdef CONFIG_MAC80211_ATBM_VERBOSE_DEBUG
	wiphy_debug(local->hw.wiphy, "device now idle\n");
#endif
	list_for_each_entry(sdata, &local->interfaces, list)
		drv_flush(local, sdata, false);

	local->hw.conf.flags |= IEEE80211_CONF_IDLE;
	return IEEE80211_CONF_CHANGE_IDLE;
}
#ifdef CONFIG_ATBM_STA_LISTEN
static void ieee80211_sdata_listen_recalc(struct ieee80211_local *local,bool start,bool flush)
{
	struct ieee80211_sub_if_data *sdata;
	bool running = 0;
	lockdep_assert_held(&local->mtx);
	
	if(!local->listen_sdata || !local->listen_channel){
		atbm_printk_err("%s,not found listen channel or listen sdata\n",__func__);
		return;
	}
	
	running = ieee80211_sdata_running(local->listen_sdata);
	
	if(start == true){
		if((local->listen_started == false)&&running){
			list_for_each_entry(sdata, &local->interfaces, list)
				drv_flush(local, sdata, false);
			if(local->ops->sta_triger_listen)
					local->ops->sta_triger_listen(&local->hw,&local->listen_sdata->vif,local->listen_channel);
				local->listen_started = true;
		}
	}else {
		if((local->listen_started == true)&&running){
			list_for_each_entry(sdata, &local->interfaces, list)
				drv_flush(local, sdata, false);
			if(local->ops->sta_stop_listen)
				local->ops->sta_stop_listen(&local->hw,&local->listen_sdata->vif);
			local->listen_started = false;
		}

		if(flush == true){
			local->listen_sdata = NULL;
			local->listen_channel = NULL;
		}
	}
}

int ieee80211_set_sta_channel(struct ieee80211_sub_if_data *sdata,int channel)
{
	int freq;	
	struct ieee80211_channel *chan = NULL;
	struct ieee80211_local *local = sdata->local;

	if(sdata->vif.type != NL80211_IFTYPE_STATION){
		atbm_printk_err("%s,interface not station! \n",__func__);
		return -EINVAL;
	}
	if(channel != 0){
		if(channel<14){
			freq = 2412+(channel-1)*5;
		}else if(channel == 14){
			freq = 2484;
		}else{
			freq = 5000 + 5 * channel;
		}

	chan = ieee80211_get_channel(local->hw.wiphy,freq);

		if(chan == NULL){
			atbm_printk_err("%s,chan invaid! \n",__func__);
			return -EINVAL;
		}
	
		mutex_lock(&local->mtx);
		ieee80211_sdata_listen_recalc(local,false,true);
		local->listen_sdata = sdata;
		local->listen_channel = chan;
		local->listen_started = false;
		__ieee80211_recalc_idle(local);
		mutex_unlock(&local->mtx);
	}else{
		mutex_lock(&local->mtx);
		ieee80211_sdata_listen_recalc(local,false,true);
		mutex_unlock(&local->mtx);
	}
	return 0; 
}

#endif
u32 __ieee80211_recalc_idle(struct ieee80211_local *local)
{
	struct ieee80211_sub_if_data *sdata;
	int count = 0;
	int authening = 0;
	bool working = false, scanning = false, hw_roc = false;
	struct ieee80211_work *wk;
	unsigned int led_trig_start = 0, led_trig_stop = 0;
#ifdef CONFIG_ATBM_SUPPORT_P2P
	struct ieee80211_roc_work *roc;
#endif

#ifdef CONFIG_PROVE_LOCKING
	WARN_ON(debug_locks && !lockdep_rtnl_is_held() &&
		!lockdep_is_held(&local->iflist_mtx));
#endif
	lockdep_assert_held(&local->mtx);

	list_for_each_entry(sdata, &local->interfaces, list) {
		if (!ieee80211_sdata_running(sdata)) {
			sdata->vif.bss_conf.idle = true;
			continue;
		}

		sdata->old_idle = sdata->vif.bss_conf.idle;
		if(sdata->vif.type == NL80211_IFTYPE_STATION && 
			rcu_dereference_protected(sdata->u.mgd.authen_bss,lockdep_is_held(&local->mtx))){
			authening = 1;
			sdata->vif.bss_conf.idle = false;
			continue;
		}
		/* do not count disabled managed interfaces */
		if (sdata->vif.type == NL80211_IFTYPE_STATION &&
		    !sdata->u.mgd.associated) {
			sdata->vif.bss_conf.idle = true;
			continue;
		}
#ifdef CONFIG_ATBM_SUPPORT_IBSS
		/* do not count unused IBSS interfaces */
		if (sdata->vif.type == NL80211_IFTYPE_ADHOC &&
		    !sdata->u.ibss.ssid_len) {
			sdata->vif.bss_conf.idle = true;
			continue;
		}
#endif
		sdata->vif.bss_conf.idle = false;
		/* count everything else */
		count++;
	}
#ifdef CONFIG_ATBM_SUPPORT_P2P
	if (!local->ops->remain_on_channel) {
		list_for_each_entry(roc, &local->roc_list, list) {
			working = true;
			roc->sdata->vif.bss_conf.idle = false;
		}
	}
#endif

	list_for_each_entry(wk, &local->work_list, list) {
		working = true;
		wk->sdata->vif.bss_conf.idle = false;
	}

	if (local->scan_sdata) {
		scanning = true;
		local->scan_sdata->vif.bss_conf.idle = false;
	}
#ifdef CONFIG_ATBM_SUPPORT_P2P
	if (local->hw_roc_channel)
		hw_roc = true;
#endif

	list_for_each_entry(sdata, &local->interfaces, list) {
		if (sdata->old_idle == sdata->vif.bss_conf.idle)
			continue;
		if (!ieee80211_sdata_running(sdata))
			continue;
		ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_IDLE);
	}
#ifdef CONFIG_ATBM_STA_LISTEN
	if(local->listen_channel && local->listen_sdata && ieee80211_sdata_running(local->listen_sdata)){
		/*
		*idle ,start listen mode
		*/
		if((working==true) || (scanning==true) || (hw_roc == true) ||(authening == true) || 
		   (local->listen_sdata->vif.bss_conf.idle == false)){
			ieee80211_sdata_listen_recalc(local,false,false);
		}else {			
			ieee80211_sdata_listen_recalc(local,true,false);			
			count++;
		}
	}
#endif
	if (working || scanning || hw_roc)
		led_trig_start |= IEEE80211_TPT_LEDTRIG_FL_WORK;
	else
		led_trig_stop |= IEEE80211_TPT_LEDTRIG_FL_WORK;

	if (count)
		led_trig_start |= IEEE80211_TPT_LEDTRIG_FL_CONNECTED;
	else
		led_trig_stop |= IEEE80211_TPT_LEDTRIG_FL_CONNECTED;

	ieee80211_mod_tpt_led_trig(local, led_trig_start, led_trig_stop);

	if (authening){
		return ieee80211_idle_off(local, IEEE80211_ATBM_IDLE_OFF_AUTHENING);
	}
	if (hw_roc)
		return ieee80211_idle_off(local, IEEE80211_ATBM_IDLE_OFF_REMAIN_ON_CHANNEL);
	if (working)
		return ieee80211_idle_off(local, IEEE80211_ATBM_IDLE_OFF_WORKING);
	if (scanning)
		return ieee80211_idle_off(local, IEEE80211_ATBM_IDLE_OFF_SCANNING);
	if (!count)
		return ieee80211_idle_on(local);
	else
		return ieee80211_idle_off(local, IEEE80211_ATBM_IDLE_OFF_OTHERS);

	return 0;
}

void ieee80211_recalc_idle(struct ieee80211_local *local)
{
	u32 chg;

	mutex_lock(&local->iflist_mtx);
	chg = __ieee80211_recalc_idle(local);
	mutex_unlock(&local->iflist_mtx);
	if (chg)
		ieee80211_hw_config(local, chg);
}

static int netdev_notify(struct notifier_block *nb,
			 unsigned long state,
			 void *ndev)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 14, 0))
	struct net_device *dev = ndev;
#else
	struct net_device *dev = netdev_notifier_info_to_dev(ndev);
#endif
	struct ieee80211_sub_if_data *sdata;

	if (state != NETDEV_CHANGENAME)
		return 0;

	if (!dev->ieee80211_ptr || !dev->ieee80211_ptr->wiphy)
		return 0;

	if (dev->ieee80211_ptr->wiphy->privid != mac80211_wiphy_privid)
		return 0;

	sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	memcpy(sdata->name, dev->name, IFNAMSIZ);

	ieee80211_debugfs_rename_netdev(sdata);
	return 0;
}

static struct notifier_block mac80211_netdev_notifier = {
	.notifier_call = netdev_notify,
};

int ieee80211_iface_init(void)
{
	return register_netdevice_notifier(&mac80211_netdev_notifier);
}

void ieee80211_iface_exit(void)
{
	unregister_netdevice_notifier(&mac80211_netdev_notifier);
}
