/*
 * Copyright 2002-2005, Instant802 Networks, Inc.
 * Copyright 2005-2006, Devicescape Software, Inc.
 * Copyright 2006-2007	Jiri Benc <jbenc@suse.cz>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <net/atbm_mac80211.h>
#include <linux/nl80211.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/skbuff.h>
#include <linux/etherdevice.h>
#include <linux/if_arp.h>
#include <linux/rtnetlink.h>
#include <linux/bitmap.h>
#ifdef CONFIG_ATBM_PM_QOS
#include <linux/pm_qos.h>
#endif
#include <linux/inetdevice.h>
#include <net/net_namespace.h>
#include <net/cfg80211.h>
#ifdef CONFIG_WIRELESS_EXT
#include <net/iw_handler.h>
#endif
#ifdef IPV6_FILTERING
#include <net/if_inet6.h>
#include <net/addrconf.h>
#endif /*IPV6_FILTERING*/

#include "ieee80211_i.h"
#include "driver-ops.h"
#include "rate.h"
#include "mesh.h"
#include "wep.h"
#include "led.h"
#include "cfg.h"
#include "debugfs.h"
#include "wext_cfg.h"
#include "../apollo.h"

//extern int g_connetting;
#ifdef CONFIG_IF1NAME
#define WIFI_IF1NAME CONFIG_IF1NAME
#else
#define WIFI_IF1NAME "wlan%d"
#endif

#ifdef CONFIG_IF2NAME
#define WIFI_IF2NAME CONFIG_IF2NAME
#else
#define WIFI_IF2NAME "p2p%d"
#endif


#pragma message(WIFI_IF1NAME)
#pragma message(WIFI_IF2NAME)
static struct lock_class_key ieee80211_rx_skb_queue_class;
void ieee80211_configure_filter(struct ieee80211_sub_if_data *sdata)
{
	u64 mc;
	unsigned int changed_flags;
	unsigned int new_flags = 0;
	struct ieee80211_local *local = sdata->local;

#if 0
	if (!(SDATA_STATE_RUNNING & sdata->state))
		return;
#endif

	if (sdata->flags & IEEE80211_SDATA_ALLMULTI)
		new_flags |= FIF_ALLMULTI;

	if (sdata->flags & IEEE80211_SDATA_PROMISC)
		new_flags |= FIF_PROMISC_IN_BSS;

	if (sdata->vif.type == NL80211_IFTYPE_MONITOR ||
			local->scan_sdata == sdata)
		new_flags |= FIF_BCN_PRBRESP_PROMISC;

	if (sdata->vif.type == NL80211_IFTYPE_AP 
#ifdef CONFIG_ATBM_SUPPORT_IBSS
		|| sdata->vif.type == NL80211_IFTYPE_ADHOC
#endif
		)
		new_flags |= FIF_PROBE_REQ;

	if (sdata->vif.type == NL80211_IFTYPE_AP)
		new_flags |= FIF_PSPOLL;

	new_flags |= sdata->req_filt_flags;

	spin_lock_bh(&local->filter_lock);
	changed_flags = sdata->filter_flags ^ new_flags;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,35))
	mc = drv_prepare_multicast(local, sdata, &sdata->mc_list);
#else
	mc = drv_prepare_multicast(local, sdata, sdata->mc_count,
				   sdata->mc_list);
#endif
	spin_unlock_bh(&local->filter_lock);

	/* be a bit nasty */
	new_flags |= (1<<31);

	drv_configure_filter(local, sdata, changed_flags, &new_flags, mc);

	WARN_ON(new_flags & (1<<31));

	sdata->filter_flags = new_flags & ~(1<<31);
}
#ifdef CONFIG_ATBM_MAC80211_NO_USE
void ieee80211_notify_channel_change(struct ieee80211_local *local,
			struct ieee80211_sub_if_data *sdata)
{
	if (local->hw.flags & IEEE80211_HW_SUPPORTS_MULTI_CHANNEL) {
		BUG_ON(!sdata);
		ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_CHANNEL);
	} else {
		ieee80211_hw_config(local, IEEE80211_CONF_CHANGE_CHANNEL);
	}
}
#endif
static struct ieee80211_channel *ieee80211_recalc_channel(
			struct ieee80211_local *local,
			struct ieee80211_sub_if_data *sdata,
			u32 *changed)
{
	struct ieee80211_channel_state *chan_state;
	struct ieee80211_channel *chan, *scan_chan;
	enum nl80211_channel_type channel_type;
	u32 offchannel_flag;
	bool multi_channel;

	BUG_ON(local && sdata);
	BUG_ON(!local && !sdata);

	multi_channel = sdata ? true : false;
	local = local ? local : sdata->local;
	scan_chan = local->scan_channel;

	if (multi_channel) {
		if (local->scan_sdata != sdata)
			scan_chan = NULL;
		chan_state = &sdata->chan_state;
	} else if (scan_chan) {
		BUG_ON(!local->scan_sdata);
		chan_state = &local->scan_sdata->chan_state;
	} else {
		chan_state = &local->chan_state;
	}

	offchannel_flag = chan_state->conf.offchannel;
	if (scan_chan) {
		chan = scan_chan;
		/* If scanning on oper channel, use whatever channel-type
		 * is currently in use.
		 */
		if (chan == chan_state->oper_channel)
			channel_type = chan_state->_oper_channel_type;
		else
			channel_type = NL80211_CHAN_NO_HT;
		chan_state->conf.offchannel = true;
	} else if (chan_state->tmp_channel &&
		   chan_state->oper_channel != chan_state->tmp_channel) {
		chan = scan_chan = chan_state->tmp_channel;
		channel_type = chan_state->tmp_channel_type;
		chan_state->conf.offchannel = true;
	} else {
		chan = chan_state->oper_channel;
		channel_type = chan_state->_oper_channel_type;
		chan_state->conf.offchannel = false;
	}

	offchannel_flag ^= chan_state->conf.offchannel;

	if (offchannel_flag || chan != chan_state->conf.channel ||
	    channel_type != chan_state->conf.channel_type) {
		chan_state->conf.channel = chan;
		chan_state->conf.channel_type = channel_type;

		if (multi_channel) {
			*changed |= BSS_CHANGED_CHANNEL;
		} else {
			*changed |= IEEE80211_CONF_CHANGE_CHANNEL;
		}
	}

	return chan;
}

int ieee80211_hw_config(struct ieee80211_local *local, u32 changed)
{
	struct ieee80211_channel *chan = NULL; /* Only used when multi channel is off */
#ifdef CONFIG_ATBM_SUPPORT_MULTI_CHANNEL
	struct ieee80211_sub_if_data *sdata;
#endif
	int ret = 0;
	int power;
	bool is_ht;

	might_sleep();
#ifdef CONFIG_ATBM_SUPPORT_MULTI_CHANNEL
	if (local->hw.flags & IEEE80211_HW_SUPPORTS_MULTI_CHANNEL) {
		// XXX: broken code ahead
		//      we can't call bss_info_changed while in rcu section!
		rcu_read_lock();
		list_for_each_entry_rcu(sdata, &local->interfaces, list)
			ieee80211_recalc_channel(NULL, sdata, &changed);
		rcu_read_unlock();
		// XXX: broken code end
	} else 
#endif
	{
		chan = ieee80211_recalc_channel(local, NULL, &changed);
	}
#ifdef CONFIG_ATBM_SUPPORT_MULTI_CHANNEL
	if (local->hw.flags & IEEE80211_HW_SUPPORTS_MULTI_CHANNEL) {
		is_ht = true;
		rcu_read_lock();
		list_for_each_entry_rcu(sdata, &local->interfaces, list) {
			if (!conf_is_ht(&sdata->chan_state.conf)) {
				is_ht = false;
				break;
			}
		}
		rcu_read_unlock();
	} else 
#endif
	{
		is_ht = conf_is_ht(local->hw.conf.chan_conf);
	}

	if (!is_ht) {
		/*
		 * atbm_mac80211.h documents that this is only valid
		 * when the channel is set to an HT type, and
		 * that otherwise STATIC is used.
		 */
		local->hw.conf.smps_mode = IEEE80211_SMPS_STATIC;
	} else if (local->hw.conf.smps_mode != local->smps_mode) {
		local->hw.conf.smps_mode = local->smps_mode;
		changed |= IEEE80211_CONF_CHANGE_SMPS;
	}
#ifdef CONFIG_ATBM_SUPPORT_MULTI_CHANNEL
	if (local->hw.flags & IEEE80211_HW_SUPPORTS_MULTI_CHANNEL) {
		power = 0;
		rcu_read_lock();
		list_for_each_entry_rcu(sdata, &local->interfaces, list)
			power = max(power, sdata->chan_state.conf.channel->max_power);
		list_for_each_entry_rcu(sdata, &local->interfaces, list)
			power = min(power, (sdata->chan_state.conf.channel->max_power -
						local->power_constr_level));
		rcu_read_unlock();
	} else 
#endif
	{
		if ((local->scanning & SCAN_SW_SCANNING) ||
		    (local->scanning & SCAN_HW_SCANNING)) {
			power = chan->max_power;
		} else {
			power = local->power_constr_level ?
				(chan->max_power - local->power_constr_level) :
				chan->max_power;
		}
	}

	if (local->user_power_level >= 0)
		power = min(power, local->user_power_level);

	if (local->hw.conf.power_level != power) {
		changed |= IEEE80211_CONF_CHANGE_POWER;
		local->hw.conf.power_level = power;
	}

	if (changed && local->open_count) {
		ret = drv_config(local, changed);
		/*
		 * Goal:
		 * HW reconfiguration should never fail, the driver has told
		 * us what it can support so it should live up to that promise.
		 *
		 * Current status:
		 * rfkill is not integrated with mac80211 and a
		 * configuration command can thus fail if hardware rfkill
		 * is enabled
		 *
		 * FIXME: integrate rfkill with mac80211 and then add this
		 * WARN_ON() back
		 *
		 */
		/* WARN_ON(ret); */
	}

	return ret;
}

void ieee80211_bss_info_change_notify(struct ieee80211_sub_if_data *sdata,
				      u32 changed)
{
	struct ieee80211_local *local = sdata->local;
	static const u8 zero[ETH_ALEN] = { 0 };

	if (!changed)
		return;
#ifdef CONFIG_ATBM_SUPPORT_MULTI_CHANNEL
	if (local->hw.flags & IEEE80211_HW_SUPPORTS_MULTI_CHANNEL)
		ieee80211_recalc_channel(NULL, sdata, &changed);
#endif
	if (sdata->vif.type == NL80211_IFTYPE_STATION) {
		/*
		 * While not associated, claim a BSSID of all-zeroes
		 * so that drivers don't do any weird things with the
		 * BSSID at that time.
		 */
		if (sdata->vif.bss_conf.assoc)
			sdata->vif.bss_conf.bssid = sdata->u.mgd.bssid;
		else
			sdata->vif.bss_conf.bssid = zero;
	} 
#ifdef CONFIG_ATBM_SUPPORT_IBSS
	else if (sdata->vif.type == NL80211_IFTYPE_ADHOC)
		sdata->vif.bss_conf.bssid = sdata->u.ibss.bssid;
#endif
	else if (sdata->vif.type == NL80211_IFTYPE_AP)
		sdata->vif.bss_conf.bssid = sdata->vif.addr;
#ifdef CONFIG_ATBM_SUPPORT_WDS
	else if (sdata->vif.type == NL80211_IFTYPE_WDS)
		sdata->vif.bss_conf.bssid = NULL;
#endif
#ifdef CONFIG_MAC80211_ATBM_MESH
	else if (ieee80211_vif_is_mesh(&sdata->vif)) {
		sdata->vif.bss_conf.bssid = zero;
	}
#endif
	else if (sdata->vif.type == NL80211_IFTYPE_MONITOR){
		sdata->vif.bss_conf.bssid = zero;
	} else {
		WARN_ON(1);
		return;
	}

	switch (sdata->vif.type) {
	case NL80211_IFTYPE_AP:
#ifdef CONFIG_ATBM_SUPPORT_IBSS
	case NL80211_IFTYPE_ADHOC:
#endif
	case NL80211_IFTYPE_WDS:
#ifdef CONFIG_MAC80211_ATBM_MESH
	case NL80211_IFTYPE_MESH_POINT:
#endif
		break;
	default:
		/* do not warn to simplify caller in scan.c */
		changed &= ~BSS_CHANGED_BEACON_ENABLED;
		if (WARN_ON(changed & BSS_CHANGED_BEACON))
			return;
		break;
	}

	if (changed & BSS_CHANGED_BEACON_ENABLED) {
		if (local->quiescing || !ieee80211_sdata_running(sdata) ||
		    test_bit(SCAN_SW_SCANNING, &local->scanning)) {
			sdata->vif.bss_conf.enable_beacon = false;
		} else {
			/*
			 * Beacon should be enabled, but AP mode must
			 * check whether there is a beacon configured.
			 */
			switch (sdata->vif.type) {
			case NL80211_IFTYPE_AP:
				sdata->vif.bss_conf.enable_beacon =
					!!sdata->u.ap.beacon;
				break;
#ifdef CONFIG_ATBM_SUPPORT_IBSS
			case NL80211_IFTYPE_ADHOC:
				sdata->vif.bss_conf.enable_beacon =
					!!sdata->u.ibss.presp;
				break;
#endif
#ifdef CONFIG_MAC80211_ATBM_MESH
			case NL80211_IFTYPE_MESH_POINT:
				sdata->vif.bss_conf.enable_beacon =
					!!sdata->u.mesh.mesh_id_len;
				break;
#endif
			default:
				/* not reached */
				WARN_ON(1);
				break;
			}
		}
	}

	drv_bss_info_changed(local, sdata, &sdata->vif.bss_conf, changed);
}

u32 ieee80211_reset_erp_info(struct ieee80211_sub_if_data *sdata)
{
	sdata->vif.bss_conf.use_cts_prot = false;
	sdata->vif.bss_conf.use_short_preamble = false;
	sdata->vif.bss_conf.use_short_slot = false;
	return BSS_CHANGED_ERP_CTS_PROT |
	       BSS_CHANGED_ERP_PREAMBLE |
	       BSS_CHANGED_ERP_SLOT;
}

static void ieee80211_tasklet_handler(unsigned long data)
{
	struct ieee80211_local *local = (struct ieee80211_local *) data;
	struct sta_info *sta, *tmp;
	struct skb_eosp_msg_data *eosp_data;
	struct sk_buff *skb;
	struct sk_buff_head local_list;
	unsigned long flags;
	__u32  queue_len = 0;
	
	__atbm_skb_queue_head_init(&local_list);

	spin_lock_irqsave(&local->skb_queue.lock,flags);
	local->tasklet_running = true;
restart:
	queue_len += atbm_skb_queue_len(&local->skb_queue);
	atbm_skb_queue_splice_tail_init(&local->skb_queue, &local_list);
	spin_unlock_irqrestore(&local->skb_queue.lock,flags);

	while ((skb = __atbm_skb_dequeue(&local_list)) != NULL){
		switch (skb->pkt_type) {
		case IEEE80211_RX_MSG:
			/* Clear skb->pkt_type in order to not confuse kernel
			 * netstack. */
			skb->pkt_type = 0;
			ieee80211_rx(local_to_hw(local), skb);
			break;
		case IEEE80211_TX_STATUS_MSG:
			skb->pkt_type = 0;
			ieee80211_tx_status(local_to_hw(local), skb);
			break;
		case IEEE80211_EOSP_MSG:
			eosp_data = (void *)skb->cb;
			for_each_sta_info(local, eosp_data->sta, sta, tmp) {
				/* skip wrong virtual interface */
				if (memcmp(eosp_data->iface,
					   sta->sdata->vif.addr, ETH_ALEN))
					continue;
				clear_sta_flag(sta, WLAN_STA_SP);
				break;
			}
			atbm_dev_kfree_skb(skb);
			break;
		default:
			WARN(1, "unknown type %d\n",skb->pkt_type);
			atbm_dev_kfree_skb(skb);
			break;
		}
	}

	spin_lock_irqsave(&local->skb_queue.lock,flags);
	if(!atbm_skb_queue_empty(&local->skb_queue))
		goto restart;
	local->tasklet_running = false;
	spin_unlock_irqrestore(&local->skb_queue.lock,flags);

	if(queue_len >= 1000){
		atbm_printk_err("process %d skbs\n",queue_len);
	}
#if 0
	while ((skb = atbm_skb_dequeue(&local->skb_queue)) ||
	       (skb = atbm_skb_dequeue(&local->skb_queue_unreliable))) {
		switch (skb->pkt_type) {
		case IEEE80211_RX_MSG:
			/* Clear skb->pkt_type in order to not confuse kernel
			 * netstack. */
			skb->pkt_type = 0;
			ieee80211_rx(local_to_hw(local), skb);
			break;
		case IEEE80211_TX_STATUS_MSG:
			skb->pkt_type = 0;
			ieee80211_tx_status(local_to_hw(local), skb);
			break;
		case IEEE80211_EOSP_MSG:
			eosp_data = (void *)skb->cb;
			for_each_sta_info(local, eosp_data->sta, sta, tmp) {
				/* skip wrong virtual interface */
				if (memcmp(eosp_data->iface,
					   sta->sdata->vif.addr, ETH_ALEN))
					continue;
				clear_sta_flag(sta, WLAN_STA_SP);
				break;
			}
			atbm_dev_kfree_skb(skb);
			break;
		default:
			WARN(1, "mac80211: Packet is of unknown type %d\n",
			     skb->pkt_type);
			atbm_dev_kfree_skb(skb);
			break;
		}
	}
#endif
}
#ifdef CONFIG_ATBM_MAC80211_NO_USE
static void ieee80211_restart_work(struct atbm_work_struct *work)
{
	struct ieee80211_local *local =
		container_of(work, struct ieee80211_local, restart_work);

	/* wait for scan work complete */
	atbm_flush_workqueue(local->workqueue);

	mutex_lock(&local->mtx);
	WARN(test_bit(SCAN_HW_SCANNING, &local->scanning) 
#ifdef CONFIG_ATBM_SUPPORT_SCHED_SCAN
		|| local->sched_scanning
#endif
		,"called with hardware scan in progress\n");
	mutex_unlock(&local->mtx);

	rtnl_lock();
	ieee80211_scan_cancel(local);
	ieee80211_reconfig(local);
	rtnl_unlock();
}

void ieee80211_restart_hw(struct ieee80211_hw *hw)
{
	struct ieee80211_local *local = hw_to_local(hw);

	trace_api_restart_hw(local);

	wiphy_info(hw->wiphy,
		   "Hardware restart was requested\n");

	/* use this reason, ieee80211_reconfig will unblock it */
	ieee80211_stop_queues_by_reason(hw,
		IEEE80211_QUEUE_STOP_REASON_SUSPEND);

	atbm_schedule_work(&local->restart_work);
}
//EXPORT_SYMBOL(ieee80211_restart_hw);
#endif
int ieee80211_pre_restart_hw_sync(struct ieee80211_hw *hw)
{
	struct ieee80211_local *local = hw_to_local(hw);
	struct ieee80211_sub_if_data *sdata;

	if (!local->open_count)
		goto prepare_done;

	ieee80211_scan_cancel(local);	
	list_for_each_entry(sdata, &local->interfaces, list) {
		
		atbm_cancel_work_sync(&sdata->work);
	
		if (!ieee80211_sdata_running(sdata))
			continue;

		if(local->pending_scan_sdata&&(local->pending_scan_sdata == sdata))
		{
			atbm_printk_init("cancle pendding scan request\n");
			local->scan_sdata = local->pending_scan_sdata;
			local->scan_req = local->pending_scan_req;
			local->pending_scan_sdata = NULL;
			local->pending_scan_req = NULL;
			ieee80211_scan_cancel(local);
		}
		
		ieee80211_work_purge(sdata);
#ifdef CONFIG_ATBM_SUPPORT_P2P
		ieee80211_roc_purge(sdata);
#endif
	}
prepare_done:
	ieee80211_stop_queues_by_reason(hw,
			IEEE80211_QUEUE_STOP_REASON_SUSPEND);
	/* flush out all packets */
	synchronize_net();
	
	local->quiescing = true;
	/* make quiescing visible to timers everywhere */
	mb();
	
	atbm_flush_workqueue(local->workqueue);

	list_for_each_entry(sdata, &local->interfaces, list) {
		atbm_cancel_work_sync(&sdata->work);

		if (!ieee80211_sdata_running(sdata))
			continue;
		if (sdata->vif.type == NL80211_IFTYPE_MONITOR)
			continue;
		drv_remove_interface(local, &sdata->vif);
	}
	/* stop hardware - this must stop RX */
	if (local->open_count)
		ieee80211_stop_device(local);
	local->suspended = true;
	/* need suspended to be visible before quiescing is false */
	barrier();
	local->quiescing = false;

	return 0;
}
int  ieee80211_restart_hw_sync(struct ieee80211_hw *hw)
{
	
	struct ieee80211_local *local = hw_to_local(hw);
	struct ieee80211_sub_if_data *sdata;
	struct ieee80211_sub_if_data *sdata_ap = NULL;
	struct sta_info *sta;
	int res = 0;
	u8 i = 0;
	
	local->suspended = false;
	mb();
	/* setup fragmentation threshold */
	drv_set_frag_threshold(local, hw->wiphy->frag_threshold);

	/* reset coverage class */
	drv_set_coverage_class(local, hw->wiphy->coverage_class);
	
	/* everything else happens only if HW was up & running */
	if (!local->open_count)
		goto restart_end;
	
	res = drv_start(local);
	if (res) {
		goto restart_end;
	}

	/* add interfaces */
	list_for_each_entry(sdata, &local->interfaces, list) {
		if (sdata->vif.type != NL80211_IFTYPE_AP_VLAN &&
		    sdata->vif.type != NL80211_IFTYPE_MONITOR &&
		    ieee80211_sdata_running(sdata))
			res = drv_add_interface(local, &sdata->vif);
	}

	
	/* reconfigure tx conf */
	if (hw->queues >= IEEE80211_NUM_ACS) {
		list_for_each_entry(sdata, &local->interfaces, list) {
			if (sdata->vif.type == NL80211_IFTYPE_AP_VLAN ||
				sdata->vif.type == NL80211_IFTYPE_MONITOR ||
				!ieee80211_sdata_running(sdata))
				continue;

			for (i = 0; i < IEEE80211_NUM_ACS; i++)
				drv_conf_tx(local, sdata, i,
						&sdata->tx_conf[i]);
		}
	}

	/* reconfigure hardware */
	ieee80211_hw_config(local, ~0);

	list_for_each_entry(sdata, &local->interfaces, list)
		ieee80211_configure_filter(sdata);

	
	/* Finally also reconfigure all the BSS information */
	list_for_each_entry(sdata, &local->interfaces, list) {
		u32 changed;

		if (!ieee80211_sdata_running(sdata))
			continue;

		/* common change flags for all interface types */
		changed = BSS_CHANGED_ERP_CTS_PROT |
			  BSS_CHANGED_ERP_PREAMBLE |
			  BSS_CHANGED_ERP_SLOT |
			  BSS_CHANGED_HT |
			  BSS_CHANGED_BASIC_RATES |
			  BSS_CHANGED_BEACON_INT |
			  BSS_CHANGED_BSSID |
			  BSS_CHANGED_CQM |
			  BSS_CHANGED_QOS |
			  BSS_CHANGED_PS;

		switch (sdata->vif.type) {
		case NL80211_IFTYPE_STATION:
			mutex_lock(&sdata->u.mgd.mtx);
			if(sdata->vif.bss_conf.assoc){
				changed |= (BSS_CHANGED_STA_RESTART|BSS_CHANGED_ASSOC);
				if (sdata->vif.bss_conf.arp_filter_enabled) {
					changed |= BSS_CHANGED_ARP_FILTER;
				}
			}
			ieee80211_bss_info_change_notify(sdata, changed);
			mutex_unlock(&sdata->u.mgd.mtx);
			ieee80211_connection_loss(&sdata->vif);
			atbm_flush_workqueue(local->workqueue);
			break;
#ifdef CONFIG_ATBM_SUPPORT_IBSS
		case NL80211_IFTYPE_ADHOC:
			changed |= BSS_CHANGED_IBSS;
#endif
			/* fall through */
		case NL80211_IFTYPE_AP:
			changed |= BSS_CHANGED_SSID;
			sdata_ap = sdata;
			/* fall through */
#ifdef CONFIG_MAC80211_ATBM_MESH
		case NL80211_IFTYPE_MESH_POINT:
#endif
			changed |= BSS_CHANGED_BEACON |
				   BSS_CHANGED_BEACON_ENABLED;
			ieee80211_bss_info_change_notify(sdata, changed);
			break;
		case NL80211_IFTYPE_WDS:
			break;
		case NL80211_IFTYPE_AP_VLAN:
		case NL80211_IFTYPE_MONITOR:
			/* ignore virtual */
			break;
		case NL80211_IFTYPE_UNSPECIFIED:
		case NUM_NL80211_IFTYPES:
#ifdef CONFIG_ATBM_SUPPORT_P2P
		case NL80211_IFTYPE_P2P_CLIENT:
		case NL80211_IFTYPE_P2P_GO:
#endif
			WARN_ON(1);
			break;
		default:
			break;
		}
	}
	/* add STAs back */
	mutex_lock(&local->sta_mtx);
	list_for_each_entry(sta, &local->sta_list, list) {
		if (sta->uploaded) {
			sdata = sta->sdata;
			if (sdata->vif.type == NL80211_IFTYPE_AP_VLAN)
				sdata = container_of(sdata->bss,
					     struct ieee80211_sub_if_data,
					     u.ap);
			set_sta_flag(sta,WLAN_STA_RESTART);
			WARN_ON(drv_sta_add(local, sdata, &sta->sta));
			if(sdata->vif.type == NL80211_IFTYPE_AP){
				WARN_ON(ieee80211_rx_sta_cook_deauthen(sta)==false);
				WARN_ON(ieee80211_tx_sta_deauthen(sta)==false);
			}			
			clear_sta_flag(sta,WLAN_STA_RESTART);
		}
	}
	mutex_unlock(&local->sta_mtx);
	if(sdata_ap){
		WARN_ON(ieee80211_tx_multicast_deauthen(sdata_ap)==false);
	}
	/* add back keys */
	list_for_each_entry(sdata, &local->interfaces, list)
		if (ieee80211_sdata_running(sdata))
			ieee80211_enable_keys(sdata);

	/* setup RTS threshold */
	list_for_each_entry(sdata, &local->interfaces, list)
	    drv_set_rts_threshold(local, sdata, sdata->vif.bss_conf.rts_threshold);
restart_end:
	atbm_mod_timer(&local->sta_cleanup, jiffies + 1);
	ieee80211_wake_queues_by_reason(hw,
			IEEE80211_QUEUE_STOP_REASON_SUSPEND);
	return res;

}
#ifdef CONFIG_ATBM_SMPS
static void ieee80211_recalc_smps_work(struct atbm_work_struct *work)
{
	struct ieee80211_local *local =
		container_of(work, struct ieee80211_local, recalc_smps);

	mutex_lock(&local->iflist_mtx);
	ieee80211_recalc_smps(local);
	mutex_unlock(&local->iflist_mtx);
}
#endif
#ifdef CONFIG_INET
static int ieee80211_ifa_changed(struct notifier_block *nb,
				 unsigned long data, void *arg)
{
	struct in_ifaddr *ifa = arg;
	struct ieee80211_local *local =
		container_of(nb, struct ieee80211_local,
			     ifa_notifier);
	struct net_device *ndev = ifa->ifa_dev->dev;
	struct wireless_dev *wdev = ndev->ieee80211_ptr;
	struct in_device *idev;
	struct ieee80211_sub_if_data *sdata;
	struct ieee80211_bss_conf *bss_conf;
	struct ieee80211_if_managed *ifmgd=NULL;
	u8 bssid[ETH_ALEN];
	u8 sta_associated = 0;
	int c = 0;

	/* Make sure it's our interface that got changed */
	if (!wdev)
		return NOTIFY_DONE;

	if (wdev->wiphy != local->hw.wiphy)
		return NOTIFY_DONE;

	sdata = IEEE80211_DEV_TO_SUB_IF(ndev);
	bss_conf = &sdata->vif.bss_conf;

	if (!ieee80211_sdata_running(sdata))
		return NOTIFY_DONE;

	/* ARP filtering is only supported in managed mode */
	switch (sdata->vif.type) {
	case NL80211_IFTYPE_STATION:
	case NL80211_IFTYPE_AP:
	case NL80211_IFTYPE_P2P_GO:
		break;
	default:
		return NOTIFY_DONE;
	}

	idev = __in_dev_get_rtnl(sdata->dev);
	if (!idev)
		return NOTIFY_DONE;

	if (sdata->vif.type == NL80211_IFTYPE_STATION) {
		ifmgd = &sdata->u.mgd;
		mutex_lock(&ifmgd->mtx);
	}

	/* Copy the addresses to the bss_conf list */
	ifa = idev->ifa_list;
	while (c < IEEE80211_BSS_ARP_ADDR_LIST_LEN && ifa) {
		bss_conf->arp_addr_list[c] = ifa->ifa_address;
		ifa = ifa->ifa_next;
		c++;
	}

	/* If not all addresses fit the list, disable filtering */
	if (ifa) {
		sdata->arp_filter_state = false;
		c = 0;
	} else {
		sdata->arp_filter_state = true;
	}
	bss_conf->arp_addr_cnt = c;

	if (sdata->vif.type == NL80211_IFTYPE_STATION) {
		/* Configure driver only if associated */
		if (ifmgd->associated) {
			sta_associated = 1;
			memcpy(bssid,ifmgd->associated->bssid,ETH_ALEN);
			bss_conf->arp_filter_enabled = sdata->arp_filter_state;
			ieee80211_bss_info_change_notify(sdata,
				BSS_CHANGED_ARP_FILTER);
		}
		mutex_unlock(&ifmgd->mtx);
	} else {
		bss_conf->arp_filter_enabled = sdata->arp_filter_state;
		ieee80211_bss_info_change_notify(sdata,
						 BSS_CHANGED_ARP_FILTER);
	}

	if(sdata->vif.type == NL80211_IFTYPE_STATION){
		if(sta_associated&&(sdata->vif.bss_conf.arp_filter_enabled == true)
		&&(sdata->vif.bss_conf.arp_addr_cnt>0)){
			atbm_printk_debug("(%s):IPv4 enable,end_time(%d)\n",sdata->name,jiffies_to_msecs(jiffies));
			mutex_lock(&local->mtx);
			ieee80211_cancle_connecting_work(sdata,bssid,false);
			mutex_unlock(&local->mtx);
		}
	}
	return NOTIFY_DONE;
}

#ifdef IPV6_FILTERING
static void ieee80211_ifa6_changed_work(struct atbm_work_struct *work)
{
        struct ieee80211_local *local =
                container_of(work, struct ieee80211_local, ifa6_changed_work);
        struct ieee80211_sub_if_data *sdata = local->ifa6_sdata;
        struct ieee80211_bss_conf *bss_conf = &sdata->vif.bss_conf;
        struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;

        mutex_lock(&ifmgd->mtx);
        if (ifmgd->associated) {
                bss_conf->ndp_filter_enabled = sdata->ndp_filter_state;
                ieee80211_bss_info_change_notify(sdata,
                                         BSS_CHANGED_NDP_FILTER);
        }
        mutex_unlock(&ifmgd->mtx);

		if(sdata->vif.type == NL80211_IFTYPE_STATION){
			if(ifmgd->associated&&(sdata->vif.bss_conf.ndp_filter_enabled == true)
			&&(sdata->vif.bss_conf.ndp_addr_cnt>0)){
				mutex_lock(&local->mtx);
				ieee80211_cancle_connecting_work(sdata,ifmgd->associated->bssid,false);
				mutex_unlock(&local->mtx);
			}
		}
}

static int ieee80211_ifa6_changed(struct notifier_block *nb,
                                 unsigned long data, void *arg)
{
        struct inet6_ifaddr *ifa = (struct inet6_ifaddr *)arg;
        struct ieee80211_local *local =
                container_of(nb, struct ieee80211_local,
                             ifa6_notifier);
        struct net_device *ndev = (struct net_device *)ifa->idev->dev;
        struct wireless_dev *wdev = ndev->ieee80211_ptr;
        struct inet6_dev *idev;
        struct ieee80211_sub_if_data *sdata;
        struct ieee80211_bss_conf *bss_conf;
        //struct ieee80211_if_managed *ifmgd;
        int c = 0;

        /* Make sure it's our interface that got changed */
        if (!wdev)
                return NOTIFY_DONE;

        if (wdev->wiphy != local->hw.wiphy)
                return NOTIFY_DONE;

        sdata = IEEE80211_DEV_TO_SUB_IF(ndev);
        bss_conf = &sdata->vif.bss_conf;

        if (!ieee80211_sdata_running(sdata))
                return NOTIFY_DONE;

        /* NDP filtering is only supported in managed mode */
        switch (sdata->vif.type) {
        case NL80211_IFTYPE_STATION:
        case NL80211_IFTYPE_AP:
        case NL80211_IFTYPE_P2P_GO:
                break;
        default:
                return NOTIFY_DONE;
        }

        idev = __in6_dev_get(sdata->dev);
        if (!idev)
                return NOTIFY_DONE;

#if 0
        if (sdata->vif.type == NL80211_IFTYPE_STATION) {
                ifmgd = &sdata->u.mgd;
                mutex_lock(&ifmgd->mtx);
        }
#endif

        list_for_each_entry(ifa, &idev->addr_list, if_list) {
                c++;
                if (c > IEEE80211_BSS_NDP_ADDR_LIST_LEN)
                        break;
                memcpy(&bss_conf->ndp_addr_list[c-1], &ifa->addr, sizeof(struct in6_addr));
        }

        if (c > IEEE80211_BSS_NDP_ADDR_LIST_LEN) {
                sdata->ndp_filter_state = false;
                c = 0;
        } else {
                sdata->ndp_filter_state = true;
        }
        bss_conf->ndp_addr_cnt = c;

        if (sdata->vif.type == NL80211_IFTYPE_STATION) {
                local->ifa6_sdata = sdata;
                atbm_schedule_work(&local->ifa6_changed_work);
#if 0
                /* Configure driver only if associated */
                if (ifmgd->associated) {
                        bss_conf->ndp_filter_enabled = sdata->ndp_filter_state;
                        ieee80211_bss_info_change_notify(sdata,
                                                 BSS_CHANGED_NDP_FILTER);
                }
                mutex_unlock(&ifmgd->mtx);
#endif
        } else {
                bss_conf->ndp_filter_enabled = sdata->ndp_filter_state;
        }

        return NOTIFY_DONE;
}
#endif /*IPV6_FILTERING*/
#endif /*CONFIG_INET*/
#ifdef IEEE80211_SUPPORT_NAPI
static int ieee80211_napi_poll(struct napi_struct *napi, int budget)
{
	struct ieee80211_local *local =
		container_of(napi, struct ieee80211_local, napi);

	return local->ops->napi_poll(&local->hw, budget);
}

void ieee80211_napi_schedule(struct ieee80211_hw *hw)
{
	struct ieee80211_local *local = hw_to_local(hw);

	napi_schedule(&local->napi);
}
//EXPORT_SYMBOL(ieee80211_napi_schedule);

void ieee80211_napi_complete(struct ieee80211_hw *hw)
{
	struct ieee80211_local *local = hw_to_local(hw);

	napi_complete(&local->napi);
}
//EXPORT_SYMBOL(ieee80211_napi_complete);
#endif
/* There isn't a lot of sense in it, but you can transmit anything you like */
static const struct ieee80211_txrx_stypes
ieee80211_default_mgmt_stypes[NUM_NL80211_IFTYPES] = {
#ifdef CONFIG_ATBM_SUPPORT_IBSS
	[NL80211_IFTYPE_ADHOC] = {
		.tx = 0xffff,
		.rx = BIT(IEEE80211_STYPE_ACTION >> 4),
	},
#endif
	[NL80211_IFTYPE_STATION] = {
		.tx = 0xffff,
		.rx = BIT(IEEE80211_STYPE_ACTION >> 4) |
			BIT(IEEE80211_STYPE_PROBE_REQ >> 4),
	},
	[NL80211_IFTYPE_AP] = {
		.tx = 0xffff,
		.rx = BIT(IEEE80211_STYPE_ACTION >> 4)|
			#ifndef ATBM_AP_SME
			BIT(IEEE80211_STYPE_ASSOC_REQ >> 4)|
			BIT(IEEE80211_STYPE_REASSOC_REQ >> 4) |
			BIT(IEEE80211_STYPE_DISASSOC >> 4) |
			BIT(IEEE80211_STYPE_AUTH >> 4) |
			BIT(IEEE80211_STYPE_DEAUTH >> 4) |
			#endif
			BIT(IEEE80211_STYPE_PROBE_REQ >> 4),
	},
	[NL80211_IFTYPE_AP_VLAN] = {
		/* copy AP */
		.tx = 0xffff,
		.rx = BIT(IEEE80211_STYPE_ASSOC_REQ >> 4) |
			BIT(IEEE80211_STYPE_REASSOC_REQ >> 4) |
			BIT(IEEE80211_STYPE_PROBE_REQ >> 4) |
			BIT(IEEE80211_STYPE_DISASSOC >> 4) |
			BIT(IEEE80211_STYPE_AUTH >> 4) |
			BIT(IEEE80211_STYPE_DEAUTH >> 4) |
			BIT(IEEE80211_STYPE_ACTION >> 4),
	},
#ifdef CONFIG_ATBM_SUPPORT_P2P
	[NL80211_IFTYPE_P2P_CLIENT] = {
		.tx = 0xffff,
		.rx = BIT(IEEE80211_STYPE_ACTION >> 4) |
			BIT(IEEE80211_STYPE_PROBE_REQ >> 4),
	},
	[NL80211_IFTYPE_P2P_GO] = {
		.tx = 0xffff,
		.rx = BIT(IEEE80211_STYPE_PROBE_REQ >> 4) |
		#ifndef ATBM_AP_SME
			BIT(IEEE80211_STYPE_REASSOC_REQ >> 4) |
			BIT(IEEE80211_STYPE_ASSOC_REQ >> 4) |
			BIT(IEEE80211_STYPE_DISASSOC >> 4) |
			BIT(IEEE80211_STYPE_AUTH >> 4) |
			BIT(IEEE80211_STYPE_DEAUTH >> 4) |
		#endif
			BIT(IEEE80211_STYPE_ACTION >> 4),
	},
#endif
#ifdef CONFIG_MAC80211_ATBM_MESH
	[NL80211_IFTYPE_MESH_POINT] = {
		.tx = 0xffff,
		.rx = BIT(IEEE80211_STYPE_ACTION >> 4) |
			BIT(IEEE80211_STYPE_AUTH >> 4) |
			BIT(IEEE80211_STYPE_DEAUTH >> 4),
	},
#endif
};
void ieee80211_resume_timer(unsigned long data)
{
	struct ieee80211_local *local = (struct ieee80211_local *)data;
	atomic_set(&local->resume_timer_start,0);
	atbm_printk_pm("end resume\n");
}

bool ieee80211_clear_extra_ie(struct ieee80211_sub_if_data *sdata,enum ieee80211_special_work_type type)
{
	union iee80211_extra_ie *new_extra;
	union iee80211_extra_ie *old_extra;

	ASSERT_RTNL();
	
	switch(type){
	case IEEE80211_SPECIAL_AP_SPECIAL_BEACON:
	case IEEE80211_SPECIAL_AP_SPECIAL_PROBRSP:
		
		if(sdata->vif.type != NL80211_IFTYPE_AP){
			return false;
		}
		if(rtnl_dereference(sdata->u.ap.beacon) == NULL){
			return false;
		}

		if(type == IEEE80211_SPECIAL_AP_SPECIAL_BEACON){
			old_extra = (union iee80211_extra_ie *)rtnl_dereference(sdata->u.ap.beacon_extra);
			
		}else{
			old_extra = (union iee80211_extra_ie *)rtnl_dereference(sdata->u.ap.probe_response_extra);
			
		}
		
		break;
		
	case IEEE80211_SPECIAL_STA_SPECIAL_PROBR:
		if(sdata->vif.type != NL80211_IFTYPE_STATION){
			return false;
		}

		old_extra = (union iee80211_extra_ie *)rtnl_dereference(sdata->u.mgd.probe_request_extra);

		
		break;
	default:
		return false;
	}

	new_extra = atbm_kzalloc(sizeof(union iee80211_extra_ie),GFP_KERNEL);
	
	if(new_extra == NULL){
		return false;
	}
	
	new_extra->extra.extra = NULL;
	new_extra->extra.extra_len = 0;
	
	
	switch(type){
	case IEEE80211_SPECIAL_AP_SPECIAL_BEACON:
	case IEEE80211_SPECIAL_AP_SPECIAL_PROBRSP:
		
		if(type == IEEE80211_SPECIAL_AP_SPECIAL_BEACON)
			rcu_assign_pointer(sdata->u.ap.beacon_extra, &new_extra->beacon);
		else
			rcu_assign_pointer(sdata->u.ap.probe_response_extra, &new_extra->proberesponse);

		synchronize_rcu();
		
		ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_BEACON_ENABLED |
				BSS_CHANGED_BEACON |
				BSS_CHANGED_SSID);	
		
		break;
	case IEEE80211_SPECIAL_STA_SPECIAL_PROBR:
		rcu_assign_pointer(sdata->u.mgd.probe_request_extra,&new_extra->proberequest);
		synchronize_rcu();
		break;
	default:
		break;
	}

	if(old_extra)
		atbm_kfree(old_extra);
	return true;
}

bool ieee80211_updata_extra_ie(struct ieee80211_sub_if_data *sdata,enum ieee80211_special_work_type type,
										  union iee80211_extra_ie *extra)
{
	union iee80211_extra_ie *new_extra;
	union iee80211_extra_ie *old_extra;

	ASSERT_RTNL();
	
	switch(type){
	case IEEE80211_SPECIAL_AP_SPECIAL_BEACON:
	case IEEE80211_SPECIAL_AP_SPECIAL_PROBRSP:
		
		if(sdata->vif.type != NL80211_IFTYPE_AP){
			return false;
		}
		if(rtnl_dereference(sdata->u.ap.beacon) == NULL){
			return false;
		}

		if(type == IEEE80211_SPECIAL_AP_SPECIAL_BEACON)
			old_extra = (union iee80211_extra_ie *)rtnl_dereference(sdata->u.ap.beacon_extra);
		else
			old_extra = (union iee80211_extra_ie *)rtnl_dereference(sdata->u.ap.probe_response_extra);
		break;
		
	case IEEE80211_SPECIAL_STA_SPECIAL_PROBR:
		if(sdata->vif.type != NL80211_IFTYPE_STATION){
			return false;
		}

		old_extra = (union iee80211_extra_ie *)rtnl_dereference(sdata->u.mgd.probe_request_extra);
		break;
	default:
		return false;
	}

	new_extra = atbm_kzalloc(sizeof(union iee80211_extra_ie)+extra->extra.extra_len,GFP_KERNEL);

	if(new_extra == NULL){
		return false;
	}

	new_extra->extra.extra 	   = ((u8*)new_extra) + (sizeof(union iee80211_extra_ie));
	new_extra->extra.extra_len =  extra->extra.extra_len;

	memcpy(new_extra->extra.extra,extra->extra.extra,extra->extra.extra_len);

	
	switch(type){
	case IEEE80211_SPECIAL_AP_SPECIAL_BEACON:
	case IEEE80211_SPECIAL_AP_SPECIAL_PROBRSP:
		
		if(type == IEEE80211_SPECIAL_AP_SPECIAL_BEACON)
			rcu_assign_pointer(sdata->u.ap.beacon_extra, &new_extra->beacon);
		else
			rcu_assign_pointer(sdata->u.ap.probe_response_extra, &new_extra->proberesponse);

		synchronize_rcu();
		
		ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_BEACON_ENABLED |
				BSS_CHANGED_BEACON |
				BSS_CHANGED_SSID);	
		break;
	case IEEE80211_SPECIAL_STA_SPECIAL_PROBR:
		rcu_assign_pointer(sdata->u.mgd.probe_request_extra,&new_extra->proberequest);
		synchronize_rcu();
		break;
	default:
		break;
	}

	if(old_extra)
		atbm_kfree(old_extra);
	return true;
}
#ifdef CONFIG_ATBM_SUPPORT_AP_CONFIG
bool ieee80211_update_ap_config(struct ieee80211_sub_if_data *sdata,struct ieee80211_internal_ap_conf *conf,bool clear)
{
	struct ieee80211_internal_ap_conf *new_conf;
	struct ieee80211_internal_ap_conf *old_conf;
	
	old_conf = rtnl_dereference(sdata->internal_ap_conf);

	if(clear == false){
		new_conf = atbm_kzalloc(sizeof(struct ieee80211_internal_ap_conf),GFP_KERNEL);

		if(new_conf == NULL){
			return false;
		}

		memcpy(new_conf,conf,sizeof(struct ieee80211_internal_ap_conf));
		atbm_printk_debug("%s:ssid[%s],bssid[%pM],channel[%d]\n",__func__,conf->ssid,conf->bssid,conf->channel);
	}else {
		new_conf = NULL;
	}
	rcu_assign_pointer(sdata->internal_ap_conf,new_conf);

	synchronize_rcu();

	if(old_conf)
		atbm_kfree(old_conf);

	return true;
}
#endif
static bool ieee80211_special_work_scan_cb(struct ieee80211_sub_if_data *sdata,void *scan_store,
		   struct ieee80211_internal_scan_result *result,bool finish){

	struct ieee80211_rx_status *rx_status;

	if(finish == true){
		return false;
	}
	if (!ieee80211_sdata_running(sdata)){
		return false;
	}

	if(sdata->vif.type != NL80211_IFTYPE_STATION){
		return false;
	}
	
	if(result->sta.skb == NULL)
		return false;

	atbm_printk_debug("%s:receive\n",__func__);
	rx_status = (struct ieee80211_rx_status *) result->sta.skb->cb;
	rx_status->flag |= RX_FLAG_STA_LISTEN;
	atbm_skb_queue_tail(&sdata->skb_queue, result->sta.skb);
	ieee80211_queue_work(&sdata->local->hw, &sdata->work);

	return true;
}
bool atbm_internal_cmd_scan_triger(struct ieee80211_sub_if_data *sdata,struct ieee80211_internal_scan_request *req);
static void ieee80211_special_work(struct atbm_work_struct *work)
{
	struct ieee80211_local *local =
		container_of(work, struct ieee80211_local, special_work);
	struct sk_buff *skb = NULL;
	struct ieee80211_sub_if_data *sdata = NULL;
	struct ieee80211_sub_if_data *sdata_tmp = NULL;
	struct ieee80211_special_work_common *work_common;
	
	/*
	*rtnl lock here is need,make sure other cmd from application can not process at 
	*the same time
	*/
	rtnl_lock();
	
	while ((skb = atbm_skb_dequeue(&local->special_req_list))){
		
		work_common = (struct ieee80211_special_work_common *)skb->cb;
		
		list_for_each_entry(sdata_tmp, &local->interfaces, list) {
			/*
			*it is important to see if the sdata is running
			*/
			if (!ieee80211_sdata_running(sdata_tmp))
				continue;

			if(sdata_tmp != work_common->req_sdata)
				continue;

			sdata = sdata_tmp;
			break;
		}
		/*
		*maybe sdata has been removed or down
		*/
		if(sdata == NULL){
			atbm_kfree_skb(skb);
			continue;
		}
		switch(skb->pkt_type){
			case IEEE80211_SPECIAL_AP_SPECIAL_BEACON:
			case IEEE80211_SPECIAL_AP_SPECIAL_PROBRSP:
			case IEEE80211_SPECIAL_STA_SPECIAL_PROBR:
			{
				struct ieee80211_update_special *special_update;
				union iee80211_extra_ie extra;

				special_update = (struct ieee80211_update_special*)skb->cb;
				/*
				*special ie err
				*/
				if((special_update->special_len == 0)||(special_update->special_ie == NULL)){				
					ieee80211_clear_extra_ie(sdata,(enum ieee80211_special_work_type)skb->pkt_type);
					break;
				}
				extra.extra.extra     = special_update->special_ie;
				extra.extra.extra_len = special_update->special_len;
				
				ieee80211_updata_extra_ie(sdata,(enum ieee80211_special_work_type)skb->pkt_type,&extra);
				break;
			}
			case IEEE80211_SPECIAL_STA_PASSICE_SCAN:
			case IEEE80211_SPECIAL_STA_POSITIVE_SCAN:
			{
				struct ieee80211_special_work_scan *special_scan;
				struct ieee80211_internal_scan_request req;
				
				special_scan = (struct ieee80211_special_work_scan *)skb->cb;
				memset(&req,0,sizeof(struct ieee80211_internal_scan_request));
				
				req.channels   = special_scan->channels;
				req.n_channels = special_scan->n_channels;
				req.ies        = special_scan->ie;
				req.ie_len     = special_scan->ie_len;
				req.ssids      = special_scan->ssid;
				if(req.ssids)
					req.n_ssids = 1;
				if(special_scan->bssid){
					req.req_flags |= IEEE80211_INTERNAL_SCAN_FLAGS__NEED_BSSID;
					memcpy(req.bssid,special_scan->bssid,6);
					atbm_printk_err("[Scan Bssid][%pM]\n",req.bssid);
				}
				if(skb->pkt_type == IEEE80211_SPECIAL_STA_PASSICE_SCAN)
					req.req_flags = IEEE80211_INTERNAL_SCAN_FLAGS__PASSAVI_SCAN;

				req.req_flags |= IEEE80211_INTERNAL_SCAN_FLAGS__NEED_SKB;

				rcu_assign_pointer(req.result_handle,ieee80211_special_work_scan_cb);
				rcu_assign_pointer(req.priv,NULL);

				atbm_internal_cmd_scan_triger(sdata,&req);
				break;
			}
			case IEEE80211_SPECIAL_CHANGE_CHANNEL_FREQ:
				break;
			case IEEE80211_SPECIAL_STA_SET_AP_CONFIG:
				break;
			default:
				WARN_ON(1);
				break;
		}
		
		atbm_kfree_skb(skb);
	}
	
	rtnl_unlock();	
}
bool ieee80211_special_freq_update(struct ieee80211_local *local,struct ieee80211_special_freq *special)
{
	int hash_index = 0;
	struct hlist_head *hlist;
	struct hlist_node *node;
	struct ieee80211_special_freq *freq_node;
	struct ieee80211_special_freq *freq_node_target = NULL;

	ASSERT_RTNL();
	
	hash_index = atbm_hash_index((u8*)(&special->channel),sizeof(void*),ATBM_COMMON_HASHBITS);

	hlist = &local->special_freq_list[hash_index];

	hlist_for_each(node,hlist){
		freq_node = hlist_entry(node,struct ieee80211_special_freq,hnode);
		if (special->channel == freq_node->channel){
			freq_node_target = freq_node;
			break;
		}
	}

	
	if(freq_node_target == NULL){
		
		freq_node_target = atbm_kzalloc(sizeof(struct ieee80211_special_freq),GFP_ATOMIC);

		if(freq_node_target == NULL){
			return false;
		}
		hlist_add_head(&freq_node_target->hnode,hlist);
		atbm_printk_debug("%s:channel[%d] freq[%d]->freq[%d]\n",__func__,
			              channel_hw_value(special->channel),channel_center_freq(special->channel),
			              special->freq);
	}

	
	BUG_ON(freq_node_target == NULL);
	freq_node_target->channel = special->channel;
	freq_node_target->freq    = special->freq;
	
	channel_mask_special(freq_node_target->channel);

	return true;
}
void ieee80211_special_freq_clear(struct ieee80211_local *local,struct ieee80211_special_freq *special)
{
	int hash_index = 0;
	struct hlist_head *hlist;
	struct hlist_node *node;
	struct hlist_node *node_temp;
	struct ieee80211_special_freq *freq_node;
	
	ASSERT_RTNL();
	
	channel_clear_special(special->channel);
	hash_index = atbm_hash_index((u8*)(&special->channel),sizeof(void*),ATBM_COMMON_HASHBITS);

	hlist = &local->special_freq_list[hash_index];

	hlist_for_each_safe(node,node_temp,hlist){
		
		freq_node = hlist_entry(node,struct ieee80211_special_freq,hnode);
		
		if(special->channel != freq_node->channel){
			continue;
		}
		atbm_printk_debug("%s:channel(%d),freq(%d)\n",__func__,channel_hw_value(freq_node->channel),freq_node->freq);
		hlist_del(&freq_node->hnode);
		atbm_kfree(freq_node);
		return;
	}
	WARN_ON(1);
}
void ieee80211_special_freq_free(struct ieee80211_local *local)
{
	struct hlist_head *hlist;
	struct hlist_node *node;
	struct hlist_node *node_temp;
	struct ieee80211_special_freq *freq_node;
	int hash_index = 0;
	
	ASSERT_RTNL();
	
	for(hash_index = 0;hash_index<ATBM_COMMON_HASHENTRIES;hash_index++){
		hlist = &local->special_freq_list[hash_index];
		hlist_for_each_safe(node,node_temp,hlist){
			freq_node = hlist_entry(node,struct ieee80211_special_freq,hnode);
			hlist_del(&freq_node->hnode);
			atbm_kfree(freq_node);
		}
	}
}
static void ieee80211_name_init(struct ieee80211_local *local)
{
	local->ieee80211_name_base = NULL;
	local->ieee80211_name_len = 0;
	spin_lock_init(&local->ieee80211_name_lock);
	INIT_LIST_HEAD(&local->ieee80211_name_list);
}
static void ieee80211_name_free(struct ieee80211_local *local)
{
	struct ieee80211_name_def *def = NULL;
	spin_lock_bh(&local->ieee80211_name_lock);

	while(!list_empty(&local->ieee80211_name_list)){
		def = list_first_entry(&local->ieee80211_name_list, struct ieee80211_name_def,list);
		list_del(&def->list);
		atbm_kfree(def);
	}
	spin_unlock_bh(&local->ieee80211_name_lock);
}
struct ieee80211_hw *ieee80211_alloc_hw(size_t priv_data_len,
					const struct ieee80211_ops *ops)
{
	struct ieee80211_local *local;
	int priv_size, i;
	struct wiphy *wiphy;

	/* Ensure 32-byte alignment of our private data and hw private data.
	 * We use the wiphy priv data for both our ieee80211_local and for
	 * the driver's private data
	 *
	 * In memory it'll be like this:
	 *
	 * +-------------------------+
	 * | struct wiphy	    |
	 * +-------------------------+
	 * | struct ieee80211_local  |
	 * +-------------------------+
	 * | driver's private data   |
	 * +-------------------------+
	 *
	 */
	priv_size = ALIGN(sizeof(*local), NETDEV_ALIGN) + priv_data_len;

	wiphy = wiphy_new(&mac80211_config_ops, priv_size);

	if (!wiphy)
		return NULL;

	wiphy->mgmt_stypes = ieee80211_default_mgmt_stypes;
	wiphy->privid = mac80211_wiphy_privid;

	wiphy->flags |= WIPHY_FLAG_NETNS_OK
#ifdef CONFIG_ATBM_4ADDR
			| WIPHY_FLAG_4ADDR_AP
			| WIPHY_FLAG_4ADDR_STATION
#endif
#if (LINUX_VERSION_CODE > KERNEL_VERSION(3, 2, 0))
			| WIPHY_FLAG_REPORTS_OBSS
#endif
			;
#ifdef CONFIG_ATBM_SUPPORT_SAE
//#if (LINUX_VERSION_CODE > KERNEL_VERSION(4, 0, 0))
	wiphy->features |= NL80211_FEATURE_SAE;
//#endif
#endif
	if (!ops->set_key)
		wiphy->flags |= WIPHY_FLAG_IBSS_RSN;

	wiphy->bss_priv_size = sizeof(struct ieee80211_bss);

	local = wiphy_priv(wiphy);

	local->hw.wiphy = wiphy;

	local->hw.priv = (char *)local + ALIGN(sizeof(*local), NETDEV_ALIGN);

	BUG_ON(!ops->tx);
	BUG_ON(!ops->start);
	BUG_ON(!ops->stop);
	BUG_ON(!ops->config);
	BUG_ON(!ops->add_interface);
	BUG_ON(!ops->remove_interface);
	BUG_ON(!ops->configure_filter);
	local->ops = ops;

	local->hw.conf.chan_conf = &local->chan_state.conf;
	
	ieee80211_name_init(local);
	/* set up some defaults */
	local->hw.queues = 1;
	local->hw.max_rates = 1;
	local->hw.max_report_rates = 0;
	local->hw.max_rx_aggregation_subframes = 64;//IEEE80211_MAX_AMPDU_BUF;
	local->hw.max_tx_aggregation_subframes = 64;//IEEE80211_MAX_AMPDU_BUF;
	local->hw.offchannel_tx_hw_queue = IEEE80211_INVAL_HW_QUEUE;
	local->user_power_level = -1;
	local->uapsd_queues = IEEE80211_DEFAULT_UAPSD_QUEUES;
	local->uapsd_max_sp_len = IEEE80211_DEFAULT_MAX_SP_LEN;
	local->tasklet_running = false;
	local->adaptive_started = false;
	
	INIT_LIST_HEAD(&local->interfaces);
	mutex_init(&local->iflist_mtx);
	mutex_init(&local->mtx);

	mutex_init(&local->key_mtx);
	spin_lock_init(&local->filter_lock);
	spin_lock_init(&local->queue_stop_reason_lock);
	LOCAL_CONNECT_INIT(local);
	/*
	 * The rx_skb_queue is only accessed from tasklets,
	 * but other SKB queues are used from within IRQ
	 * context. Therefore, this one needs a different
	 * locking class so our direct, non-irq-safe use of
	 * the queue's lock doesn't throw lockdep warnings.
	 */
	skb_queue_head_init_class(&local->rx_skb_queue,
				  &ieee80211_rx_skb_queue_class);
	spin_lock_init(&local->rx_path_lock);
	ATBM_INIT_DELAYED_WORK(&local->scan_work, ieee80211_scan_work);

	ieee80211_work_init(local);
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	ATBM_INIT_WORK(&local->restart_work, ieee80211_restart_work);
#endif
	ATBM_INIT_WORK(&local->special_work,ieee80211_special_work);
	atbm_skb_queue_head_init(&local->special_req_list);
	atbm_common_hash_list_init(local->special_freq_list,ATBM_COMMON_HASHENTRIES);
	
#ifdef CONFIG_INET
#ifdef IPV6_FILTERING
    ATBM_INIT_WORK(&local->ifa6_changed_work, ieee80211_ifa6_changed_work);
#endif /*IPV6_FILTERING*/
#endif
#ifdef CONFIG_ATBM_SMPS
	ATBM_INIT_WORK(&local->recalc_smps, ieee80211_recalc_smps_work);
#endif
	local->smps_mode = IEEE80211_SMPS_OFF;
#ifdef CONFIG_ATBM_SUPPORT_SCHED_SCAN
	ATBM_INIT_WORK(&local->sched_scan_stopped_work,
		  ieee80211_sched_scan_stopped_work);
#endif

	sta_info_init(local);

	for (i = 0; i < IEEE80211_MAX_QUEUES; i++) {
		atbm_skb_queue_head_init(&local->pending[i]);
		atomic_set(&local->agg_queue_stop[i], 0);
	}
	tasklet_init(&local->tx_pending_tasklet, ieee80211_tx_pending,
		     (unsigned long)local);

	tasklet_init(&local->tasklet,
		     ieee80211_tasklet_handler,
		     (unsigned long) local);

	atbm_skb_queue_head_init(&local->skb_queue);
	atbm_skb_queue_head_init(&local->skb_queue_unreliable);
#ifdef IEEE80211_SUPPORT_NAPI
	/* init dummy netdev for use w/ NAPI */
	init_dummy_netdev(&local->napi_dev);
#endif
	ieee80211_led_names(local);
#ifdef CONFIG_ATBM_SUPPORT_P2P
	ieee80211_hw_roc_setup(local);
#endif
	atomic_set(&local->resume_timer_start,0);
	atbm_setup_timer(&local->resume_timer, ieee80211_resume_timer,
		    (unsigned long)local);
	ieee80211_scan_internal_int(local);
	
	return local_to_hw(local);
}
//EXPORT_SYMBOL(ieee80211_alloc_hw);
#ifdef CONFIG_ATBM_APOLLO_DEBUG
extern int atbm_debug_init_common(void * priv);
#endif
static char *init_dev_name = "ALTOBEAM";
int ieee80211_register_hw(struct ieee80211_hw *hw)
{
	struct ieee80211_local *local = hw_to_local(hw);
	struct ieee80211_channel_state *chan_state = &local->chan_state;
	int result, i;
	enum ieee80211_band band;
	int channels, max_bitrates;
	bool supp_ht;
	static const u32 cipher_suites[] = {
		/* keep WEP first, it may be removed below */
		WLAN_CIPHER_SUITE_WEP40,
		WLAN_CIPHER_SUITE_WEP104,
		WLAN_CIPHER_SUITE_TKIP,
		WLAN_CIPHER_SUITE_CCMP,
#ifdef CONFIG_WAPI_SUPPORT
		WLAN_CIPHER_SUITE_SMS4,
#endif
		/* keep last -- depends on hw flags! */
		WLAN_CIPHER_SUITE_AES_CMAC
	};

	if (hw->flags & IEEE80211_HW_QUEUE_CONTROL &&
	    (local->hw.offchannel_tx_hw_queue == IEEE80211_INVAL_HW_QUEUE ||
	     local->hw.offchannel_tx_hw_queue >= local->hw.queues))
		return -EINVAL;

	if (hw->max_report_rates == 0)
		hw->max_report_rates = hw->max_rates;
	
	BUG_ON(strlen(WIFI_IF1NAME)>IFNAMSIZ-1);
	BUG_ON(strlen(WIFI_IF2NAME)>IFNAMSIZ-1);

	/*
	 * generic code guarantees at least one band,
	 * set this very early because much code assumes
	 * that chan_state->conf.channel is assigned
	 */
	LOCAL_SET_CONNECT_STOP(local);
	channels = 0;
	max_bitrates = 0;
	supp_ht = false;
	for (band = 0; band < IEEE80211_NUM_BANDS; band++) {
		struct ieee80211_supported_band *sband;

		sband = local->hw.wiphy->bands[band];
		if (!sband)
			continue;
		if (!chan_state->oper_channel) {
			/* init channel we're on */
			/* soumik: Set default channel to a non-social channel */
			chan_state->conf.channel =
			/* chan_state->oper_channel = &sband->channels[0]; */
			chan_state->oper_channel = &sband->channels[2];
			chan_state->conf.channel_type = NL80211_CHAN_NO_HT;
		}
		channels += sband->n_channels;

		if (max_bitrates < sband->n_bitrates)
			max_bitrates = sband->n_bitrates;
		supp_ht = supp_ht || sband->ht_cap.ht_supported;
	}

	local->int_scan_req = atbm_kzalloc(sizeof(*local->int_scan_req) +
				      sizeof(void *) * channels, GFP_KERNEL);
	if (!local->int_scan_req)
		return -ENOMEM;

	for (band = 0; band < IEEE80211_NUM_BANDS; band++) {
		if (!local->hw.wiphy->bands[band])
			continue;
		#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 1, 0))
		local->int_scan_req->rates[band] = (u32) -1;
		#endif
	}

	/* if low-level driver supports AP, we also support VLAN */
	if (local->hw.wiphy->interface_modes & BIT(NL80211_IFTYPE_AP)) {
		hw->wiphy->interface_modes |= BIT(NL80211_IFTYPE_AP_VLAN);
		hw->wiphy->software_iftypes |= BIT(NL80211_IFTYPE_AP_VLAN);
	}

	/* mac80211 always supports monitor */
	hw->wiphy->interface_modes |= BIT(NL80211_IFTYPE_MONITOR);
	hw->wiphy->software_iftypes |= BIT(NL80211_IFTYPE_MONITOR);

	/*
	 * mac80211 doesn't support more than 1 channel, and also not more
	 * than one IBSS interface
	 */
	for (i = 0; i < hw->wiphy->n_iface_combinations; i++) {
		const struct ieee80211_iface_combination *c;
#ifdef CONFIG_ATBM_SUPPORT_IBSS
		int j;
#endif
		c = &hw->wiphy->iface_combinations[i];

		if (c->num_different_channels > 1)
			return -EINVAL;
#ifdef CONFIG_ATBM_SUPPORT_IBSS
		for (j = 0; j < c->n_limits; j++)
			if ((c->limits[j].types & BIT(NL80211_IFTYPE_ADHOC)) &&
			    c->limits[j].max > 1)
				return -EINVAL;
#endif
	}

#ifndef CONFIG_MAC80211_ATBM_MESH
	/* mesh depends on Kconfig, but drivers should set it if they want */
	local->hw.wiphy->interface_modes &= ~BIT(NL80211_IFTYPE_MESH_POINT);
#else

	/* if the underlying driver supports mesh, mac80211 will (at least)
	 * provide routing of mesh authentication frames to userspace */
	if (local->hw.wiphy->interface_modes & BIT(NL80211_IFTYPE_MESH_POINT))
		local->hw.wiphy->flags |= WIPHY_FLAG_MESH_AUTH;
#endif	
	/* mac80211 supports control port protocol changing */
	local->hw.wiphy->flags |= WIPHY_FLAG_CONTROL_PORT_PROTOCOL;

	if (local->hw.flags & IEEE80211_HW_SIGNAL_DBM)
		local->hw.wiphy->signal_type = CFG80211_SIGNAL_TYPE_MBM;
	else if (local->hw.flags & IEEE80211_HW_SIGNAL_UNSPEC)
		local->hw.wiphy->signal_type = CFG80211_SIGNAL_TYPE_UNSPEC;

	/*
	 * Calculate scan IE length -- we need this to alloc
	 * memory and to subtract from the driver limit. It
	 * includes the DS Params, (extended) supported rates, and HT
	 * information -- SSID is the driver's responsibility.
	 */
	local->scan_ies_len = 4 + max_bitrates /* (ext) supp rates */ +
		3 /* DS Params */;
	if (supp_ht)
		local->scan_ies_len += 2 + sizeof(struct ieee80211_ht_cap);

	if (!local->ops->hw_scan) {
		/* For hw_scan, driver needs to set these up. */
		local->hw.wiphy->max_scan_ssids = 4;
		local->hw.wiphy->max_scan_ie_len = IEEE80211_MAX_DATA_LEN;
	}

#ifdef SIGMSTAR_SCAN_FEATURE
{
	unsigned int randNum;
	for(i=0;i<CHANNEL_NUM;i++){
			 get_random_bytes(&randNum,sizeof(unsigned int));
		 	local->noise_floor[i] = -85 - ((u32)randNum%10);
    }
}
#endif  //SIGMSTAR_SCAN_FEATURE

	/*
	 * If the driver supports any scan IEs, then assume the
	 * limit includes the IEs mac80211 will add, otherwise
	 * leave it at zero and let the driver sort it out; we
	 * still pass our IEs to the driver but userspace will
	 * not be allowed to in that case.
	 */
	if (local->hw.wiphy->max_scan_ie_len)
		local->hw.wiphy->max_scan_ie_len -= local->scan_ies_len;

	/* Set up cipher suites unless driver already did */
	if (!local->hw.wiphy->cipher_suites) {
		local->hw.wiphy->cipher_suites = cipher_suites;
		local->hw.wiphy->n_cipher_suites = ARRAY_SIZE(cipher_suites);
		if (!(local->hw.flags & IEEE80211_HW_MFP_CAPABLE))
			local->hw.wiphy->n_cipher_suites--;
	}
#ifdef CONFIG_ATBM_USE_SW_ENC
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(5, 4, 0))
	if (IS_ERR(local->wep_tx_tfm) || IS_ERR(local->wep_rx_tfm)) {
#else
	//need find the condition later
	if (0) {
#endif
		if (local->hw.wiphy->cipher_suites == cipher_suites) {
			local->hw.wiphy->cipher_suites += 2;
			local->hw.wiphy->n_cipher_suites -= 2;
		} else {
			u32 *suites;
			int r, w = 0;

			/* Filter out WEP */

			suites = kmemdup(
				local->hw.wiphy->cipher_suites,
				sizeof(u32) * local->hw.wiphy->n_cipher_suites,
				GFP_KERNEL);
			if (!suites)
				return -ENOMEM;
			for (r = 0; r < local->hw.wiphy->n_cipher_suites; r++) {
				u32 suite = local->hw.wiphy->cipher_suites[r];
				if (suite == WLAN_CIPHER_SUITE_WEP40 ||
				    suite == WLAN_CIPHER_SUITE_WEP104)
					continue;
				suites[w++] = suite;
			}
			local->hw.wiphy->cipher_suites = suites;
			local->hw.wiphy->n_cipher_suites = w;
			local->wiphy_ciphers_allocated = true;
		}
	}
#endif
	if (!local->ops->remain_on_channel)
		local->hw.wiphy->max_remain_on_channel_duration = 5000;
#ifdef CONFIG_ATBM_SUPPORT_SCHED_SCAN
	if (local->ops->sched_scan_start)
		local->hw.wiphy->flags |= WIPHY_FLAG_SUPPORTS_SCHED_SCAN;
#endif
#ifdef ATBM_SURPORT_TDLS
	/* mac80211 based drivers don't support internal TDLS setup */
#if (LINUX_VERSION_CODE > KERNEL_VERSION(3, 0, 8))
	if (local->hw.wiphy->flags & WIPHY_FLAG_SUPPORTS_TDLS)
		local->hw.wiphy->flags |= WIPHY_FLAG_TDLS_EXTERNAL_SETUP;
#endif
#endif
	
	result = wiphy_register(local->hw.wiphy);
	if (result < 0)
		goto fail_wiphy_register;
	
	local->hw.wiphy->dev.init_name = init_dev_name;
	/*
	 * We use the number of queues for feature tests (QoS, HT) internally
	 * so restrict them appropriately.
	 */
	if (hw->queues > IEEE80211_MAX_QUEUES)
		hw->queues = IEEE80211_MAX_QUEUES;

	local->workqueue =
		atbm_alloc_ordered_workqueue(wiphy_name(local->hw.wiphy), 0);
	if (!local->workqueue) {
		result = -ENOMEM;
		goto fail_workqueue;
	}

	/*
	 * The hardware needs headroom for sending the frame,
	 * and we need some headroom for passing the frame to monitor
	 * interfaces, but never both at the same time.
	 */
	local->tx_headroom = max_t(unsigned int , local->hw.extra_tx_headroom,
				   IEEE80211_TX_STATUS_HEADROOM);

	debugfs_hw_add(local);
#ifdef CONFIG_ATBM_APOLLO_DEBUG
	atbm_debug_init_common(hw->priv);
#endif

	/*
	 * if the driver doesn't specify a max listen interval we
	 * use 5 which should be a safe default
	 */
	if (local->hw.max_listen_interval == 0)
		local->hw.max_listen_interval = 5;
#ifdef CONFIG_ATBM_USE_SW_ENC
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(5, 4, 0))
	result = ieee80211_wep_init(local);
#else
	ieee80211_wep_init(local);
	result = 0;
#endif
	if (result < 0)
		atbm_printk_err("Failed to initialize wep: %d\n",
			    result);
#endif
	rtnl_lock();
#ifndef CONFIG_RATE_HW_CONTROL
	result = atbm_ieee80211_init_rate_ctrl_alg(local,
					      hw->rate_control_algorithm);
	if (result < 0) {
		wiphy_debug(local->hw.wiphy,
			    "Failed to initialize rate control algorithm\n");
		goto fail_rate;
	}
#endif
#ifdef CONFIG_WIRELESS_EXT
	register_wext_common(local);
#endif
	/* add one default STA interface if supported */
	if (local->hw.wiphy->interface_modes & BIT(NL80211_IFTYPE_STATION)) {
		result = ieee80211_if_add(local, WIFI_IF1NAME, NULL,
					  NL80211_IFTYPE_STATION, NULL);
		if (result)
			atbm_printk_warn("Failed to add default virtual iface\n");
#if (ATBM_WIFI_PLATFORM != 12) && (NEED_P2P0_INTERFACE == 1)//CDLINUX no need p2p0
		result = ieee80211_if_add(local, WIFI_IF2NAME, NULL,
					  NL80211_IFTYPE_STATION, NULL);

		if (result)
			atbm_printk_warn("Failed to add default virtual p2p iface\n");
#endif  //#if (ATBM_WIFI_PLATFORM == 11)
	}
	rtnl_unlock();

	ieee80211_led_init(local);
#ifdef CONFIG_ATBM_PM_QOS
	local->network_latency_notifier.notifier_call =
		ieee80211_max_network_latency;
	result = pm_qos_add_notifier(PM_QOS_NETWORK_LATENCY,
				     &local->network_latency_notifier);
	if (result) {
		rtnl_lock();
		goto fail_pm_qos;
	}
#endif

#ifdef CONFIG_INET
	local->ifa_notifier.notifier_call = ieee80211_ifa_changed;
	result = register_inetaddr_notifier(&local->ifa_notifier);
	if (result)
		goto fail_ifa;

#ifdef IPV6_FILTERING
	local->ifa6_notifier.notifier_call = ieee80211_ifa6_changed;
	result = register_inet6addr_notifier(&local->ifa6_notifier);
	if (result)
		goto fail_ifa;
#endif /*IPV6_FILTERING*/
#endif
#ifdef IEEE80211_SUPPORT_NAPI
	netif_napi_add(&local->napi_dev, &local->napi, ieee80211_napi_poll,
			local->hw.napi_weight);
#endif
	return 0;

#ifdef CONFIG_INET
 fail_ifa:
#ifdef CONFIG_ATBM_PM_QOS
	pm_qos_remove_notifier(PM_QOS_NETWORK_LATENCY,
			       &local->network_latency_notifier);
#endif
	rtnl_lock();
#endif
#ifdef CONFIG_ATBM_PM_QOS
 fail_pm_qos:
	ieee80211_led_exit(local);
	ieee80211_remove_interfaces(local);
#endif
#ifndef CONFIG_RATE_HW_CONTROL
 fail_rate:
#endif
	rtnl_unlock();
#ifdef CONFIG_ATBM_USE_SW_ENC
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(5, 4, 0))
	ieee80211_wep_free(local);
#endif
#endif
	sta_info_stop(local);
	atbm_destroy_workqueue(local->workqueue);
 fail_workqueue:
	wiphy_unregister(local->hw.wiphy);
 fail_wiphy_register:
	if (local->wiphy_ciphers_allocated)
		atbm_kfree((void*)local->hw.wiphy->cipher_suites);
	atbm_kfree(local->int_scan_req);
	return result;
}
//EXPORT_SYMBOL(ieee80211_register_hw);

void ieee80211_unregister_hw(struct ieee80211_hw *hw)
{
	struct ieee80211_local *local = hw_to_local(hw);

	tasklet_kill(&local->tx_pending_tasklet);
	tasklet_kill(&local->tasklet);
#ifdef CONFIG_ATBM_PM_QOS
	pm_qos_remove_notifier(PM_QOS_NETWORK_LATENCY,
			       &local->network_latency_notifier);
#endif
#ifdef CONFIG_INET
	unregister_inetaddr_notifier(&local->ifa_notifier);
#ifdef IPV6_FILTERING
	unregister_inet6addr_notifier(&local->ifa6_notifier);
#endif /*IPV6_FILTERING*/
#endif
	rtnl_lock();

	/*
	 * At this point, interface list manipulations are fine
	 * because the driver cannot be handing us frames any
	 * more and the tasklet is killed.
	 */
	ieee80211_remove_interfaces(local);
	ieee80211_special_freq_free(local);
	rtnl_unlock();

	
	atomic_set(&local->resume_timer_start,0);
	atbm_del_timer_sync(&local->resume_timer);
	/*
	 * Now all work items will be gone, but the
	 * timer might still be armed, so delete it
	 */
	atbm_del_timer_sync(&local->work_timer);
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	atbm_cancel_work_sync(&local->restart_work);
#endif
	atbm_cancel_work_sync(&local->special_work);
#ifdef IPV6_FILTERING
#ifdef CONFIG_INET
	atbm_cancel_work_sync(&local->ifa6_changed_work);
#endif
#endif /*IPV6_FILTERING*/

	ieee80211_clear_tx_pending(local);
#ifndef CONFIG_RATE_HW_CONTROL
	rate_control_deinitialize(local);
#endif

	if (atbm_skb_queue_len(&local->skb_queue) ||
	    atbm_skb_queue_len(&local->skb_queue_unreliable))
		atbm_printk_warn("skb_queue not empty\n");
	atbm_skb_queue_purge(&local->skb_queue);
	atbm_skb_queue_purge(&local->skb_queue_unreliable);
	atbm_skb_queue_purge(&local->rx_skb_queue);
	atbm_skb_queue_purge(&local->special_req_list);

#ifdef CONFIG_MAC80211_ATBM_ROAMING_CHANGES
	/*
	 * Work can also be sheduled in call_rcu callback.
	 * Wait for all rcu callbacks to finish.
	 */
	rcu_barrier();
#endif
	atbm_flush_workqueue(local->workqueue);
	atbm_destroy_workqueue(local->workqueue);
	wiphy_unregister(local->hw.wiphy);
	sta_info_stop(local);
#ifdef CONFIG_ATBM_USE_SW_ENC
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(5, 4, 0))
	ieee80211_wep_free(local);
#endif
#endif
	ieee80211_led_exit(local);
	ieee80211_scan_internal_deinit(local);
	atbm_kfree(local->int_scan_req);
}
//EXPORT_SYMBOL(ieee80211_unregister_hw);

void ieee80211_free_hw(struct ieee80211_hw *hw)
{
	struct ieee80211_local *local = hw_to_local(hw);

	mutex_destroy(&local->iflist_mtx);
	mutex_destroy(&local->mtx);

	if (local->wiphy_ciphers_allocated)
		atbm_kfree((void*)(local->hw.wiphy->cipher_suites));
	ieee80211_name_free(local);
	wiphy_free(local->hw.wiphy);
}
//EXPORT_SYMBOL(ieee80211_free_hw);
int atbm_ieee80211_init(void)
//static int __init atbm_ieee80211_init(void)
{
	struct sk_buff *skb;
	int ret;

	BUILD_BUG_ON(sizeof(struct ieee80211_tx_info) > sizeof(skb->cb));
	BUILD_BUG_ON(offsetof(struct ieee80211_tx_info, driver_data) +
		     IEEE80211_TX_INFO_DRIVER_DATA_SIZE > sizeof(skb->cb));
	ETF_Test_is_Start();
#ifndef CONFIG_RATE_HW_CONTROL
	ret = rc80211_minstrel_init();
	if (ret)
		return ret;

	ret = rc80211_minstrel_ht_init();
	if (ret)
		goto err_minstrel;

	ret = rc80211_pid_init();
	if (ret)
		goto err_pid;
#endif
	ret = ieee80211_iface_init();
	if (ret)
		goto err_netdev;

	return 0;
 err_netdev:
 #ifndef CONFIG_RATE_HW_CONTROL
	atbm_rc80211_pid_exit();
 err_pid:
	atbm_rc80211_minstrel_ht_exit();
 err_minstrel:
	rc80211_minstrel_exit();
#endif
	return ret;
}

void atbm_ieee80211_exit(void)
//static void __exit ieee80211_exit(void)
{
#ifndef CONFIG_RATE_HW_CONTROL
	atbm_printk_exit("ieee80211_exit\n");
	atbm_rc80211_pid_exit();
	atbm_printk_exit("rc80211_pid_exit\n");
	atbm_rc80211_minstrel_ht_exit();
	atbm_printk_exit("atbm_rc80211_minstrel_ht_exit\n");
	rc80211_minstrel_exit();
	atbm_printk_exit("rc80211_minstrel_exit\n");
#endif
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,37))
	flush_scheduled_work();
#endif

	if (mesh_allocated)
		ieee80211s_stop();

	ieee80211_iface_exit();
	atbm_printk_exit("ieee80211_iface_exit\n");
	rcu_barrier();
}



//subsys_initcall(atbm_ieee80211_init);
//module_exit(ieee80211_exit);

//MODULE_DESCRIPTION("IEEE 802.11 subsystem");
//MODULE_LICENSE("GPL");
