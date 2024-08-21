/*
 * Mac80211 STA API for altobeam APOLLO drivers
 *
 * Copyright (c) 2016, altobeam
 *
 * Based on:
 * Copyright (c) 2010, stericsson
 * Author: Dmitry Tarnyagin <dmitry.tarnyagin@stericsson.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/sched.h>
#include <linux/firmware.h>
#include <linux/if_arp.h>
#include <linux/ipv6.h>
#include <linux/icmpv6.h>
#include <net/ndisc.h>

#include "apollo.h"
#include "sta.h"
#include "ap.h"
#include "fwio.h"
#include "bh.h"
#include "debug.h"
#include "wsm.h"
#include "hwio.h"
#ifdef CONFIG_ATBM_SUPPORT_SCHED_SCAN
#ifdef ROAM_OFFLOAD
#include <net/netlink.h>
#endif /*ROAM_OFFLOAD*/
#endif
//#ifdef CONFIG_ATBM_APOLLO_TESTMODE
#include "atbm_testmode.h"
#include <net/netlink.h>
//#endif /* CONFIG_ATBM_APOLLO_TESTMODE */

#include "net/atbm_mac80211.h"
#include "dbg_event.h"
#include "smartconfig.h"
#if defined(CONFIG_ATBM_APOLLO_STA_DEBUG)
#define sta_printk(...) atbm_printk_always(__VA_ARGS__)
#else
#define sta_printk(...)
#endif
extern int start_choff;
#include "mac80211/ieee80211_i.h"
#define WEP_ENCRYPT_HDR_SIZE    4
#define WEP_ENCRYPT_TAIL_SIZE   4
#define WPA_ENCRYPT_HDR_SIZE    8
#define WPA_ENCRYPT_TAIL_SIZE   12
#define WPA2_ENCRYPT_HDR_SIZE   8
#define WPA2_ENCRYPT_TAIL_SIZE  8
#define WAPI_ENCRYPT_HDR_SIZE   18
#define WAPI_ENCRYPT_TAIL_SIZE  16
#define MAX_ARP_REPLY_TEMPLATE_SIZE     120
/**************start_tx start_rx globla variable*******************/
#if defined(CONFIG_NL80211_TESTMODE) && defined(CONFIG_ATBM_TEST_TOOL)
static u8 ETF_bStart_Tx = 0;
static u8 ETF_bStart_Rx = 0;
static char ch_and_type[20] = {0};
#endif
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
const int atbm_1d_to_ac[8] = {
	IEEE80211_AC_BE,
	IEEE80211_AC_BK,
	IEEE80211_AC_BK,
	IEEE80211_AC_BE,
	IEEE80211_AC_VI,
	IEEE80211_AC_VI,
	IEEE80211_AC_VO,
	IEEE80211_AC_VO
};

/**
 * enum atbm_ac_numbers - AC numbers as used in apollo
 * @ATBM_APOLLO_AC_VO: voice
 * @ATBM_APOLLO_AC_VI: video
 * @ATBM_APOLLO_AC_BE: best effort
 * @ATBM_APOLLO_AC_BK: background
 */
enum atbm_ac_numbers {
	ATBM_APOLLO_AC_VO	= 0,
	ATBM_APOLLO_AC_VI	= 1,
	ATBM_APOLLO_AC_BE	= 2,
	ATBM_APOLLO_AC_BK	= 3,
};
#endif /*CONFIG_ATBM_APOLLO_TESTMODE*/

#ifdef IPV6_FILTERING
#define MAX_NEIGHBOR_ADVERTISEMENT_TEMPLATE_SIZE 144
#endif /*IPV6_FILTERING*/

static inline void __atbm_free_event_queue(struct list_head *list)
{
	while (!list_empty(list)) {
		struct atbm_wsm_event *event =
			list_first_entry(list, struct atbm_wsm_event,
			link);
		list_del(&event->link);
		atbm_kfree(event);
	}
}

#ifdef CONFIG_ATBM_APOLLO_TESTMODE
/* User priority to WSM queue mapping */
const int atbm_priority_to_queueId[8] = {
	WSM_QUEUE_BEST_EFFORT,
	WSM_QUEUE_BACKGROUND,
	WSM_QUEUE_BACKGROUND,
	WSM_QUEUE_BEST_EFFORT,
	WSM_QUEUE_VIDEO,
	WSM_QUEUE_VIDEO,
	WSM_QUEUE_VOICE,
	WSM_QUEUE_VOICE
};
#endif /*CONFIG_ATBM_APOLLO_TESTMODE*/
static inline void __atbm_bf_configure(struct atbm_vif *priv)
{
	priv->bf_table.numOfIEs = __cpu_to_le32(3);
	priv->bf_table.entry[0].ieId = ATBM_WLAN_EID_VENDOR_SPECIFIC;
	priv->bf_table.entry[0].actionFlags = WSM_BEACON_FILTER_IE_HAS_CHANGED |
					WSM_BEACON_FILTER_IE_NO_LONGER_PRESENT |
					WSM_BEACON_FILTER_IE_HAS_APPEARED;
	priv->bf_table.entry[0].oui[0] = 0x50;
	priv->bf_table.entry[0].oui[1] = 0x6F;
	priv->bf_table.entry[0].oui[2] = 0x9A;

	priv->bf_table.entry[1].ieId = ATBM_WLAN_EID_ERP_INFO;
	priv->bf_table.entry[1].actionFlags = WSM_BEACON_FILTER_IE_HAS_CHANGED |
					WSM_BEACON_FILTER_IE_NO_LONGER_PRESENT |
					WSM_BEACON_FILTER_IE_HAS_APPEARED;

	priv->bf_table.entry[2].ieId = ATBM_WLAN_EID_HT_INFORMATION;
	priv->bf_table.entry[2].actionFlags = WSM_BEACON_FILTER_IE_HAS_CHANGED |
					WSM_BEACON_FILTER_IE_NO_LONGER_PRESENT |
					WSM_BEACON_FILTER_IE_HAS_APPEARED;

	priv->bf_control.enabled = WSM_BEACON_FILTER_ENABLE;
}

/* ******************************************************************** */
/* STA API								*/

int atbm_start(struct ieee80211_hw *dev)
{
	struct atbm_common *hw_priv = dev->priv;
	int ret = 0;
	if(atbm_bh_is_term(hw_priv)){
		return ret;
	}
	/* Assign Max SSIDs supported based on the firmware revision*/
	mutex_lock(&hw_priv->conf_mutex);

#ifdef CONFIG_ATBM_APOLLO_TESTMODE
	spin_lock_bh(&hw_priv->tsm_lock);
	memset(&hw_priv->tsm_stats, 0, sizeof(struct atbm_tsm_stats));
	memset(hw_priv->atbm_tsm_stats, 0, 4 * sizeof(struct atbm_tsm_stats));
	memset(&hw_priv->tsm_info, 0, sizeof(struct atbm_tsm_info));
	spin_unlock_bh(&hw_priv->tsm_lock);
#endif /*CONFIG_ATBM_APOLLO_TESTMODE*/
	memcpy(hw_priv->mac_addr, dev->wiphy->perm_addr, ETH_ALEN);
	hw_priv->softled_state = 0;

	ret = atbm_setup_mac(hw_priv);
	if (WARN_ON(ret))
		goto out;

out:
	mutex_unlock(&hw_priv->conf_mutex);
	return ret;
}

void atbm_stop(struct ieee80211_hw *dev)
{
	struct atbm_common *hw_priv = dev->priv;
	struct atbm_vif *priv = NULL;
	LIST_HEAD(list);
	int i;
	if(atbm_bh_is_term(hw_priv)){
		return;
	}
	wsm_lock_tx(hw_priv);

	while (down_trylock(&hw_priv->scan.lock)) {
		/* Scan is in progress. Force it to stop. */
		hw_priv->scan.req = NULL;
		schedule();
	}
	up(&hw_priv->scan.lock);

	atbm_hw_cancel_delayed_work(&hw_priv->scan.probe_work,true);
	atbm_hw_cancel_delayed_work(&hw_priv->scan.timeout,true);
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
	atbm_hw_cancel_delayed_work(&hw_priv->advance_scan_timeout,true);
#endif
	atbm_flush_workqueue(hw_priv->workqueue);
#ifdef CONFIG_ATBM_BA_STATUS
	atbm_del_timer_sync(&hw_priv->ba_timer);
#endif

	mutex_lock(&hw_priv->conf_mutex);

	hw_priv->softled_state = 0;
	/* atbm_set_leds(hw_priv); */

	spin_lock_bh(&hw_priv->event_queue_lock);
	list_splice_init(&hw_priv->event_queue, &list);
	spin_unlock_bh(&hw_priv->event_queue_lock);
	__atbm_free_event_queue(&list);

	for (i = 0; i < 4; i++)
		atbm_queue_clear(&hw_priv->tx_queue[i], ATBM_WIFI_ALL_IFS);
#if 0
	/* HACK! */
	if (atomic_xchg(&hw_priv->tx_lock, 1) != 1)
		sta_printk( "[STA] TX is force-unlocked "
			"due to stop request.\n");
#endif

	atbm_for_each_vif(hw_priv, priv, i) {
		if (!priv)
			continue;
		priv->mode = NL80211_IFTYPE_UNSPECIFIED;
		priv->listening = false;
#ifndef CONFIG_TX_NO_CONFIRM
		priv->delayed_link_loss = 0;
#endif
		priv->join_status = ATBM_APOLLO_JOIN_STATUS_PASSIVE;
		atbm_hw_cancel_delayed_work(&priv->join_timeout,false);
#ifndef CONFIG_TX_NO_CONFIRM
		atbm_hw_cancel_delayed_work(&priv->bss_loss_work,false);
		atbm_hw_cancel_delayed_work(&priv->connection_loss_work,false);
#endif
		atbm_hw_cancel_delayed_work(&priv->link_id_gc_work,false);
		atbm_hw_cancel_delayed_work(&priv->dhcp_retry_work,false);
		atbm_del_timer_sync(&priv->mcast_timeout);
	}

	wsm_unlock_tx(hw_priv);

	mutex_unlock(&hw_priv->conf_mutex);
}

int atbm_add_interface(struct ieee80211_hw *dev,
			 struct ieee80211_vif *vif)
{
	int ret;
	struct atbm_common *hw_priv = dev->priv;
	struct atbm_vif *priv;
	struct atbm_vif **drv_priv = (void *)vif->drv_priv;
#ifndef P2P_MULTIVIF
	int i;

	if (atomic_read(&hw_priv->num_vifs) >= ATBM_WIFI_MAX_VIFS)
		return -EOPNOTSUPP;
#endif
	if(atbm_bh_is_term(hw_priv)){
		return -EOPNOTSUPP;
	}
	atbm_printk_sta("%s: vif->type %d, vif->p2p %d, addr %pM\n",
			__func__, vif->type, vif->p2p, vif->addr);

	priv = ABwifi_get_vif_from_ieee80211(vif);
	atomic_set(&priv->enabled, 0);

	*drv_priv = priv;
	/* __le32 auto_calibration_mode = __cpu_to_le32(1); */

	mutex_lock(&hw_priv->conf_mutex);

	priv->mode = vif->type;

	atbm_hw_vif_write_lock(&hw_priv->vif_list_lock);
	if (atomic_read(&hw_priv->num_vifs) < ATBM_WIFI_MAX_VIFS) {
#ifdef P2P_MULTIVIF
		if (!memcmp(vif->addr, hw_priv->addresses[0].addr, ETH_ALEN)) {
			priv->if_id = 0;
		} else if (!memcmp(vif->addr, hw_priv->addresses[1].addr,
			ETH_ALEN)) {
			priv->if_id = 2;
		} else if (!memcmp(vif->addr, hw_priv->addresses[2].addr,
			ETH_ALEN)) {
			priv->if_id = 1;
		}
		sta_printk( "%s: if_id %d mac %pM\n",
				__func__, priv->if_id, vif->addr);
#else
		for (i = 0; i < ATBM_WIFI_MAX_VIFS; i++)
			if (!memcmp(vif->addr, hw_priv->addresses[i].addr,
						ETH_ALEN))
				break;
		if (i == ATBM_WIFI_MAX_VIFS) {
			atbm_hw_vif_write_unlock(&hw_priv->vif_list_lock);
			mutex_unlock(&hw_priv->conf_mutex);
			return -EINVAL;
		}
		priv->if_id = i;
#endif
		hw_priv->if_id_slot |= BIT(priv->if_id);
		priv->hw_priv = hw_priv;
		priv->hw = dev;
		priv->vif = vif;
		atomic_inc(&hw_priv->num_vifs);
#ifdef	ATBM_WIFI_QUEUE_LOCK_BUG
		{
			/*
			*at mac80211,there is 16 queues(local->hw_queue) supported to use by user.
			*so if_id = 0 use the first four queues,if_id =1 use second four queues.so 
			*when add interface ,initing the queue as below.
			*/
			u8 index = 0;
			priv->queue_cap = ATBM_QUEUE_DEFAULT_CAP;
			for(index=0;index<IEEE80211_NUM_ACS;index++){
				vif->hw_queue[index] = 4*priv->if_id + index;
				atbm_printk_sta("%s[%d],hw_queue[%d]=[%d]\n",__func__,priv->if_id,index,vif->hw_queue[index]);
			} 
		}
#endif
#ifdef CONFIG_ATBM_SUPPORT_P2P
#ifdef ATBM_P2P_CHANGE
		if(priv->if_id == 0){
			atomic_set(&hw_priv->combination,0);
		}
#endif
#endif
		/*
		*BUG!!!!!,we shoud not get priv from vif->drv_priv, because of that at 
		* some time ,the vif_lock is not inited before called add_interface function,
		*but vif->drv_priv has been malloc by mac80211 without inited,so when we get
		*priv with spin_lock_bh(&priv->vif_lock),can make the program die.
		*To modify the bug,we shoud do as follow:
		*1. to make sure priv->vif_lock has been inited,so we judge that
		*   the i_priv->enabled has been inited as 1;
		*/
#ifdef ATBM_PKG_REORDER
		atbm_reorder_func_init(priv);
#endif
		ret = atbm_debug_init_priv(hw_priv, priv);
		WARN_ON(ret);
		atbm_vif_setup_params(priv);
		WARN_ON(ATBM_HW_VIF_GET(hw_priv->vif_list[priv->if_id]) != NULL);
		ATBM_HW_VIF_SET(hw_priv->vif_list[priv->if_id],vif);
		
	} else {
		atbm_hw_vif_write_unlock(&hw_priv->vif_list_lock);
		mutex_unlock(&hw_priv->conf_mutex);
		return -EOPNOTSUPP;
	}
	atbm_hw_vif_write_unlock(&hw_priv->vif_list_lock);
	/* TODO:COMBO :Check if MAC address matches the one expected by FW */
	memcpy(hw_priv->mac_addr, vif->addr, ETH_ALEN);
	
#ifdef  ATBM_VIF_LIST_USE_RCU_LOCK
	synchronize_rcu();
#endif

	/* Enable auto-calibration */
	/* Exception in subsequent channel switch; disabled.
	WARN_ON(wsm_write_mib(hw_priv, WSM_MIB_ID_SET_AUTO_CALIBRATION_MODE,
		&auto_calibration_mode, sizeof(auto_calibration_mode)));
	*/
	sta_printk( "[STA] Interface ID:%d of type:%d added\n",
				priv->if_id, priv->mode);

	if(vif->type == NL80211_IFTYPE_MONITOR){
			priv->join_status = ATBM_APOLLO_JOIN_STATUS_SIMPLE_MONITOR;
			atbm_printk_sta("%s:start monitor\n",__func__);
	}
	mutex_unlock(&hw_priv->conf_mutex);

	atbm_vif_setup(priv);
	ret = WARN_ON(atbm_setup_mac_pvif(priv));

	return ret;
}

void atbm_remove_interface(struct ieee80211_hw *dev,
			     struct ieee80211_vif *vif)
{
	struct atbm_common *hw_priv = dev->priv;
	struct atbm_vif *priv = ABwifi_get_vif_from_ieee80211(vif);
	struct wsm_reset reset = {
		.reset_statistics = true,
	};
	int i;
	bool is_htcapie = false;
	struct atbm_vif *tmp_priv;
	struct wsm_operational_mode mode = {
		.power_mode = wsm_power_mode_quiescent,
		.disableMoreFlagUsage = true,
	};

	atbm_printk_sta(" !!! %s: enter, priv=%p type %d p2p %d addr %pM\n",
			__func__, priv, vif->type, vif->p2p, vif->addr);

	if((atomic_read(&priv->enabled)==0)||(priv->hw_priv != hw_priv)){
		atbm_printk_err("[%s] has been removed,dont remove again\n",vif_to_sdata(vif)->name);
		return;
	}
	atomic_set(&priv->enabled, 0);
	down(&hw_priv->scan.lock);
	mutex_lock(&hw_priv->conf_mutex);
	atbm_tx_queues_lock(hw_priv);
	/*
	*must make sure that there are no pkgs in the lmc.
	*/
	wsm_lock_tx_async(hw_priv);
	if(atbm_bh_is_term(hw_priv)){
		goto reset_priv;
	}
	wsm_flush_tx(hw_priv);
	switch (priv->join_status) {
	case ATBM_APOLLO_JOIN_STATUS_IBSS:
		wsm_reset(hw_priv, &reset, priv->if_id);
#if (LINUX_VERSION_CODE > KERNEL_VERSION(5, 10, 60))
	
		fallthrough;
#endif
	case ATBM_APOLLO_JOIN_STATUS_STA:
		wsm_lock_tx(hw_priv);
		//if (atbm_hw_priv_queue_work(hw_priv, &priv->unjoin_work) <= 0)
		//	wsm_unlock_tx(hw_priv);
		mutex_unlock(&hw_priv->conf_mutex);
		atbm_unjoin_work(&priv->unjoin_work);
		mutex_lock(&hw_priv->conf_mutex);
		break;
	case ATBM_APOLLO_JOIN_STATUS_AP:
		for (i = 0; priv->link_id_map; ++i) {
			if (priv->link_id_map & BIT(i)) {
				ABwifi_unmap_link(priv, i);
				priv->link_id_map &= ~BIT(i);
			}
		}
		memset(priv->link_id_db, 0,
				sizeof(priv->link_id_db));
		priv->sta_asleep_mask = 0;
		priv->enable_beacon = false;
		priv->tx_multicast = false;
		priv->aid0_bit_set = false;
		priv->buffered_multicasts = false;
		priv->pspoll_mask = 0;
		reset.link_id = 0;
		wsm_reset(hw_priv, &reset, priv->if_id);
		WARN_ON(wsm_set_operational_mode(hw_priv, &mode, priv->if_id));
		atbm_for_each_vif(hw_priv, tmp_priv, i) {
#ifdef P2P_MULTIVIF
			if ((i == (ATBM_WIFI_MAX_VIFS - 1)) || !tmp_priv)
#else
			if (!tmp_priv)
#endif
				continue;
			if ((tmp_priv->join_status == ATBM_APOLLO_JOIN_STATUS_STA) && tmp_priv->htcap)
				is_htcapie = true;
		}

		if (is_htcapie) {
			hw_priv->vif0_throttle = ATBM_WIFI_HOST_VIF0_11N_THROTTLE;
			hw_priv->vif1_throttle = ATBM_WIFI_HOST_VIF1_11N_THROTTLE;
			sta_printk(KERN_ERR "AP REMOVE HTCAP 11N %d\n",hw_priv->vif0_throttle);
		} else {
			hw_priv->vif0_throttle = ATBM_WIFI_HOST_VIF0_11BG_THROTTLE;
			hw_priv->vif1_throttle = ATBM_WIFI_HOST_VIF1_11BG_THROTTLE;
			sta_printk(KERN_ERR "AP REMOVE 11BG %d\n",hw_priv->vif0_throttle);
		}
		if(priv->if_id != 0)
		{
			struct atbm_vif *priv_wlan;

			if((priv_wlan=__ABwifi_hwpriv_to_vifpriv(hw_priv,0))!= NULL)
			{
				if(hw_priv->roc_wlan_pm.pmMode != WSM_PSM_ACTIVE)
					memcpy(&priv_wlan->powersave_mode,&hw_priv->roc_wlan_pm,sizeof(struct wsm_set_pm));
				atbm_set_pm(priv_wlan,&priv_wlan->powersave_mode);
			}
		}
		break;
	case ATBM_APOLLO_JOIN_STATUS_MONITOR:
#if defined(CONFIG_ATBM_STA_LISTEN) || defined(CONFIG_ATBM_SUPPORT_P2P)
		atbm_disable_listening(priv);
#endif
		break;
	case ATBM_APOLLO_JOIN_STATUS_SIMPLE_MONITOR:
		atbm_printk_sta("%s:ATBM_APOLLO_JOIN_STATUS_SIMPLE_MONITOR\n",__func__);
		atbm_stop_monitor_mode(priv);
		break;
	
#ifdef CONFIG_ATBM_STA_LISTEN
	case ATBM_APOLLO_JOIN_STATUS_STA_LISTEN:
		WARN_ON(1);
		break;
#endif
	default:
		break;
	}
reset_priv:
#ifdef ATBM_PKG_REORDER
	atbm_reorder_func_reset(priv,0xff);
#endif
	/* TODO:COMBO: Change Queue Module */
	sta_printk("%s:priv->if_id(%d)\n",__func__,priv->if_id);
	if (!__atbm_flush(hw_priv, true, priv->if_id))
		wsm_unlock_tx(hw_priv);
	atbm_hw_cancel_delayed_work(&priv->dhcp_retry_work,true);
	
	spin_lock_bh(&priv->dhcp_retry_spinlock);
	if(priv->dhcp_retry_skb){		
		atbm_dev_kfree_skb(priv->dhcp_retry_skb);
		priv->dhcp_retry_skb = NULL;
	}
	spin_unlock_bh(&priv->dhcp_retry_spinlock);
#ifndef CONFIG_TX_NO_CONFIRM	
	atbm_hw_cancel_delayed_work(&priv->bss_loss_work,false);
	atbm_hw_cancel_delayed_work(&priv->connection_loss_work,false);
#endif
	atbm_hw_cancel_delayed_work(&priv->link_id_gc_work,false);
	atbm_hw_cancel_delayed_work(&priv->join_timeout,false);
#if 0
	atbm_hw_cancel_delayed_work(&priv->set_cts_work,false);
#endif
#ifdef CONFIG_ATBM_SUPPORT_P2P
	atbm_hw_cancel_delayed_work(&priv->pending_offchanneltx_work,false);
#endif
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	atbm_hw_cancel_queue_work(&priv->update_filtering_work,true);
#endif
	atbm_hw_cancel_queue_work(&priv->set_beacon_wakeup_period_work,true);
	atbm_del_timer_sync(&priv->mcast_timeout);
	/* TODO:COMBO: May be reset of these variables "delayed_link_loss and
	 * join_status to default can be removed as dev_priv will be freed by
	 * mac80211 */
	spin_lock_bh(&priv->ps_state_lock);
	if(priv->join_status == ATBM_APOLLO_JOIN_STATUS_AP){
		int link_id = 0;
		for (link_id = 0; link_id < ATBMWIFI_MAX_STA_IN_AP_MODE; ++link_id) {
			if(priv->link_id_db[link_id].status != ATBM_APOLLO_LINK_OFF){
				atbm_skb_queue_purge(&priv->link_id_db[link_id].rx_queue);
			}
		}
	}
	spin_unlock_bh(&priv->ps_state_lock);
#ifndef CONFIG_TX_NO_CONFIRM	
	priv->delayed_link_loss = 0;
#endif
	priv->join_status = ATBM_APOLLO_JOIN_STATUS_PASSIVE;
	wsm_unlock_tx(hw_priv);

	if ((priv->if_id ==1) && (priv->mode == NL80211_IFTYPE_AP
		|| priv->mode == NL80211_IFTYPE_P2P_GO)) {
		hw_priv->is_go_thru_go_neg = false;
	}
#ifdef CONFIG_ATBM_SUPPORT_P2P
#ifdef ATBM_P2P_CHANGE
	if((priv->if_id == 1)&&(vif->p2p)){
		atbm_printk_sta("%s:reset p2p go save\n",__func__);
		atomic_set(&hw_priv->go_bssid_set,0);
		atomic_set(&hw_priv->receive_go_resp,0);
		atomic_set(&hw_priv->p2p_oper_channel,0);
	}
	else if(priv->if_id == 0){
		atbm_printk_sta("%s:disable combination mode\n",__func__);
		atomic_set(&hw_priv->combination,0);		
	}
#endif
#endif
#ifdef	ATBM_WIFI_QUEUE_LOCK_BUG
		atbm_clear_priv_queue_cap(priv);	
#endif

	atbm_hw_vif_write_lock(&hw_priv->vif_list_lock);
	atbm_priv_vif_list_write_lock(&priv->vif_lock);
	ATBM_HW_VIF_SET(hw_priv->vif_list[priv->if_id],NULL);
	hw_priv->if_id_slot &= (~BIT(priv->if_id));
	atomic_dec(&hw_priv->num_vifs);
	if (atomic_read(&hw_priv->num_vifs) == 0) {
		atbm_free_keys(hw_priv);
		memset(hw_priv->mac_addr, 0, ETH_ALEN);
	}
	atbm_priv_vif_list_write_unlock(&priv->vif_lock);
	atbm_hw_vif_write_unlock(&hw_priv->vif_list_lock);
	priv->listening = false;

	atbm_tx_queues_unlock(hw_priv);
	
#ifdef  ATBM_VIF_LIST_USE_RCU_LOCK
	synchronize_rcu();
#endif
	
	atbm_debug_release_priv(priv);
	mutex_unlock(&hw_priv->conf_mutex);
	up(&hw_priv->scan.lock);
	/*
	*flush all work
	*/
	atbm_flush_workqueue(hw_priv->workqueue);
	memset(priv, 0, sizeof(struct atbm_vif));
}

int atbm_change_interface(struct ieee80211_hw *dev,
				struct ieee80211_vif *vif,
				enum nl80211_iftype new_type,
				bool p2p)
{
	int ret = 0;
	atbm_printk_sta("atbm_change_interface: type %d (%d), p2p %d (%d)\n",
			new_type, vif->type, p2p, vif->p2p);
	if (new_type != vif->type || vif->p2p != p2p) {
		atbm_remove_interface(dev, vif);
		vif->type = new_type;
		vif->p2p = p2p;
		ret = atbm_add_interface(dev, vif);
	}

	return ret;
}

int atbm_config(struct ieee80211_hw *dev, u32 changed)
{
	int ret = 0;
	struct atbm_common *hw_priv = dev->priv;
	struct ieee80211_conf *conf = &dev->conf;
	bool queue_change_chwork = false;
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
	int max_power_level = 0;
	int min_power_level = 0;
#endif

	u32 support_changed = IEEE80211_CONF_CHANGE_MONITOR|
						IEEE80211_CONF_CHANGE_IDLE|
						IEEE80211_CONF_CHANGE_POWER|
						IEEE80211_CONF_CHANGE_CHANNEL;

	/* TODO:COMBO: adjust to multi vif interface
	 * IEEE80211_CONF_CHANGE_IDLE is still handled per atbm_vif*/
	int if_id = 0;
	struct atbm_vif *priv;
	if(atbm_bh_is_term(hw_priv)){
		return 0;
	}

	if((!!(support_changed & changed)) == 0)
	{
		atbm_printk_debug( "%s:changed(%x)\n",__func__,changed);
		return ret;
	}
	while (changed &
		(IEEE80211_CONF_CHANGE_MONITOR|IEEE80211_CONF_CHANGE_IDLE)) {
		struct ieee80211_local *local = hw_to_local(dev);
		u8 i = 0;
		priv = NULL;
		/* TBD: It looks like it's transparent
		 * there's a monitor interface present -- use this
		 * to determine for example whether to calculate
		 * timestamps for packets or not, do not use instead
		 * of filter flags! */
		if(!(changed&IEEE80211_CONF_CHANGE_MONITOR)){
			break;
		}
		if(!(changed&IEEE80211_CONF_CHANGE_CHANNEL)){
			break;
		}
		if(local->monitors == 0){
			break;
		}

		atbm_for_each_vif(hw_priv, priv, i) {
			if (priv == NULL)
				continue;
			if (priv->join_status != ATBM_APOLLO_JOIN_STATUS_SIMPLE_MONITOR)
				continue;
			break;
		}
		if(i == ATBM_WIFI_MAX_VIFS){
			atbm_printk_debug("%s: no priv is moniter\n",__func__);
			break;
		}
		down(&hw_priv->scan.lock);
		mutex_lock(&hw_priv->conf_mutex);
		hw_priv->channel = conf->chan_conf->channel;
		hw_priv->channel_type = conf->chan_conf->channel_type;
		if(hw_priv->monitor_if_id != -1)
			atbm_stop_monitor_mode(priv);
		if(hw_priv->monitor_if_id == -1){
			atbm_start_monitor_mode(priv,hw_priv->channel);
			mutex_unlock(&hw_priv->conf_mutex);
			up(&hw_priv->scan.lock);
			return ret;
		}
		mutex_unlock(&hw_priv->conf_mutex);
		up(&hw_priv->scan.lock);
		sta_printk(
				"ignore IEEE80211_CONF_CHANGE_MONITOR (%d)"
				"IEEE80211_CONF_CHANGE_IDLE (%d)\n",
			(changed & IEEE80211_CONF_CHANGE_MONITOR) ? 1 : 0,
			(changed & IEEE80211_CONF_CHANGE_IDLE) ? 1 : 0);
		break;
	}
	down(&hw_priv->scan.lock);
	mutex_lock(&hw_priv->conf_mutex);
	priv = __ABwifi_hwpriv_to_vifpriv(hw_priv, hw_priv->scan.if_id);
	/* TODO: IEEE80211_CONF_CHANGE_QOS */
	/* TODO:COMBO:Change when support is available mac80211*/
	if (changed & IEEE80211_CONF_CHANGE_POWER) {
		/*hw_priv->output_power = conf->power_level;*/
		sta_printk("Output power ++%d\n",conf->power_level);
		hw_priv->output_power = 20;
		sta_printk("Output power --%d\n",hw_priv->output_power);
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
		/* Testing if Power Level to set is out of device power range */
		if (conf->chan_conf->channel->band ==
			IEEE80211_BAND_2GHZ) {
			max_power_level = hw_priv->txPowerRange[0].max_power_level;
			min_power_level = hw_priv->txPowerRange[0].min_power_level;
		} else {
			max_power_level = hw_priv->txPowerRange[1].max_power_level;
			min_power_level = hw_priv->txPowerRange[1].min_power_level;
		}
		if (hw_priv->output_power > max_power_level)
			hw_priv->output_power = max_power_level;
		else if (hw_priv->output_power < min_power_level)
			hw_priv->output_power = min_power_level;
#endif /* CONFIG_ATBM_APOLLO_TESTMODE */
		sta_printk( "[STA] TX power: %d\n",
				hw_priv->output_power);
		WARN_ON(wsm_set_output_power(hw_priv,
					     hw_priv->output_power * 10,
					     if_id));
	}
#ifndef ATBM_SUPPORT_WIDTH_40M
	if ((changed & IEEE80211_CONF_CHANGE_CHANNEL) &&
			(hw_priv->channel != conf->chan_conf->channel)) {
		/* Switch Channel commented for CC Mode */
		struct ieee80211_channel *ch = conf->chan_conf->channel;
		sta_printk( "[STA] Freq %d (wsm ch: %d).\n",
			channel_center_freq(ch), channel_hw_value(ch));
		/* Earlier there was a call to __atbm_flush().
		   Removed as deemed unnecessary */

			hw_priv->channel = ch;
	}
#else
	// add change for 40M
	/*
	*chan_state is used for wlan0 and p2p0 ,because we only surport one channel
	*chan_state->tmp_channel and chan_state->tmp_channel_type are usd by sta 
	*mode when sta is trying to asscoiating with ap and at that time chan_state->conf.offchannel
	*will be true.
	*chan_state->oper_channel and chan_state->_oper_channel_type is the current channel which 
	*used by wlan0 or p2p0.
	*/
	while(changed & IEEE80211_CONF_CHANGE_CHANNEL){
		#define CHTYPE_IS_20M(chtype) 	((chtype == NL80211_CHAN_HT20)||(chtype == NL80211_CHAN_NO_HT))
		u8 new_type;
		enum nl80211_channel_type pre_hw_channel_type = hw_priv->channel_type;
		struct ieee80211_channel  * pre_hw_channel = hw_priv->channel;
		struct ieee80211_sub_if_data *sdata;
		struct ieee80211_local *local = hw_to_local(dev);
		struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(local, NULL);

		/*
		*if tmp_channel is used,the channel will be set in the join work.
		*/
		if((chan_state->tmp_channel == conf->chan_conf->channel)&&
		   (chan_state->tmp_channel_type == conf->chan_conf->channel_type)&&
		   (chan_state->conf.offchannel == true)){
			atbm_printk_sta("%s:tmp_channel action\n",__func__);
			hw_priv->channel = conf->chan_conf->channel;
			if(chan_state->_oper_channel_type == conf->chan_conf->channel_type)
				hw_priv->channel_type = conf->chan_conf->channel_type;
			if(pre_hw_channel == hw_priv->channel)
				break;
		}
		/*
		*if oper_channel is used,change both hw_priv->channel and hw_priv->channel_type
		*/
		if((chan_state->oper_channel == conf->chan_conf->channel)&&
		   (chan_state->_oper_channel_type == conf->chan_conf->channel_type)&&
		   (chan_state->conf.offchannel == false)){
		  atbm_printk_sta("%s:oper_channel action\n",__func__);
		   hw_priv->channel = conf->chan_conf->channel;
		   hw_priv->channel_type = conf->chan_conf->channel_type;
		}
		/*
		*here,if hw_priv->channel != conf->chan_conf->channel,must be somethieng
		*wrong,only update channel ,do not triger channel switch work
		*/
		if(hw_priv->channel != conf->chan_conf->channel){
			atbm_printk_sta("%s:channel maybe err,must updata\n",__func__);
			hw_priv->channel = conf->chan_conf->channel;
		   	hw_priv->channel_type = conf->chan_conf->channel_type;
			break;
		}

		if((pre_hw_channel == hw_priv->channel)&&(pre_hw_channel_type == hw_priv->channel_type)){
			atbm_printk_sta("%s:channel not chage(%p)(%d)\n",__func__,hw_priv->channel,hw_priv->channel_type);
			break;
		}
		/*
		*search the action sdata to triger the channel workqueue.
		*here is safe to search the local->interfaces list
		*/
		list_for_each_entry(sdata, &local->interfaces, list){

			if((sdata->vif.type == NL80211_IFTYPE_MONITOR)||
				(sdata->vif.type == NL80211_IFTYPE_AP_VLAN)){
				continue;
			}
			if (!ieee80211_sdata_running(sdata)){
				continue;
			}

			/*
			*search the active priv to triger channel switch work
			*priv->join_status must has been set and higher than 
			*ATBM_APOLLO_JOIN_STATUS_MONITOR
			*prev channel type is not equal with current channel typer
			*and the current channel type must be 40M
			*/
			if(vif_chw(&sdata->vif) !=  conf->chan_conf->channel_type){
				atbm_printk_sta("[%s]:channel_type(%d),(%d)\n",sdata->name,vif_chw(&sdata->vif),conf->chan_conf->channel_type);
				continue;
			}

			priv = ABwifi_get_vif_from_ieee80211(&sdata->vif);

			if(priv == NULL){
				atbm_printk_sta("%s:priv == NULL\n",__func__);
				continue;
			}
			if(atomic_read(&priv->enabled)==0){
				atbm_printk_sta("%s:priv->enabled = 0\n",__func__);
				continue;
			}

			if(priv->join_status <= ATBM_APOLLO_JOIN_STATUS_MONITOR){
				atbm_printk_sta( "%s:not join or not ap mode\n",sdata->name);
				continue;
			}


			hw_priv->channel_type = conf->chan_conf->channel_type;
			new_type = CHTYPE_IS_20M(conf->chan_conf->channel_type);

			if(pre_hw_channel_type != conf->chan_conf->channel_type){
				if(new_type == 0)
					queue_change_chwork = true;
				else
					queue_change_chwork = false;		
			}
			
			if(sdata->vif.p2p&&(queue_change_chwork == true)){
				WARN_ON(ieee80211_chw_is_ht40(vif_chw(&sdata->vif)));
				atbm_printk_sta("%s:p2p not suport 40M\n",__func__);
				queue_change_chwork = false;
			}
			
			if(pre_hw_channel != hw_priv->channel){
				atbm_printk_sta("[%s]:%d mode channel change\n",sdata->name,priv->join_status);
				queue_change_chwork = true;
			}

			if(queue_change_chwork == true){
				struct wsm_set_chantype set_channtype;
				
				wsm_lock_tx_async(hw_priv);
				wsm_flush_tx(hw_priv);

				set_channtype.band = (hw_priv->channel->band == IEEE80211_BAND_5GHZ) ?
									 WSM_PHY_BAND_5G : WSM_PHY_BAND_2_4G;
				set_channtype.flag = 0;
				set_channtype.channelNumber = channel_hw_value(hw_priv->channel);
				set_channtype.channelType = (u32)hw_priv->channel_type;
				atbm_printk_sta("%s:chatype(%d),channelNumber(%d)\n",__func__,
					set_channtype.channelType,set_channtype.channelNumber);
				if(wsm_set_chantype_func(hw_priv,&set_channtype,priv->if_id))
				{
					/*
					*if we set channel type err to lmc,send 20 M is safe
					*/
					atomic_set(&hw_priv->phy_chantype,0);
				}else if(ieee80211_chw_is_ht40(hw_priv->channel_type)){
					atomic_set(&hw_priv->phy_chantype,1);
					atomic_set(&hw_priv->tx_20M_lock,0);
				}
				else{
					atomic_set(&hw_priv->phy_chantype,0);
					atomic_set(&hw_priv->tx_20M_lock,1);
				}
				wsm_unlock_tx(hw_priv);
				break;
			}
		}
		break;
	}
	mutex_unlock(&hw_priv->conf_mutex);
	up(&hw_priv->scan.lock);
#endif
	return ret;
}

void atbm_update_filtering(struct atbm_vif *priv)
{
	int ret;
	bool bssid_filtering = !priv->rx_filter.bssid;
	struct atbm_common *hw_priv = ABwifi_vifpriv_to_hwpriv(priv);
	
	if (priv->join_status == ATBM_APOLLO_JOIN_STATUS_PASSIVE)
		return;
	else if (priv->join_status == ATBM_APOLLO_JOIN_STATUS_MONITOR)
		bssid_filtering = false;
	/*
	* When acting as p2p client being connected to p2p GO, in order to
	* receive frames from a different p2p device, turn off bssid filter.
	*
	* WARNING: FW dependency!
	* This can only be used with FW WSM371 and its successors.
	* In that FW version even with bssid filter turned off,
	* device will block most of the unwanted frames.
	*/
	if (priv->vif && priv->vif->p2p)
		bssid_filtering = false;

	ret = wsm_set_rx_filter(hw_priv, &priv->rx_filter, priv->if_id);
	if (!ret)
		ret = wsm_set_bssid_filtering(hw_priv, bssid_filtering,
					priv->if_id);
#if 0
	if (!ret) {
		ret = wsm_set_multicast_filter(hw_priv, &priv->multicast_filter,
						priv->if_id);
	}
#endif
	if (ret)
		atbm_printk_warn("%s: Update filtering failed: %d.,if(%d)\n",
				__func__, ret,priv->if_id);
	return;
}
#ifdef CONFIG_ATBM_MAC80211_NO_USE
void atbm_update_filtering_work(struct atbm_work_struct *work)
{
	struct atbm_vif *priv =
		container_of(work, struct atbm_vif,
		update_filtering_work);
	if(atbm_bh_is_term(priv->hw_priv)){
		return;
	}
	//atbm_update_filtering(priv);
}
#endif
void atbm_set_beacon_wakeup_period_work(struct atbm_work_struct *work)
{
	struct atbm_vif *priv =
		container_of(work, struct atbm_vif,
		set_beacon_wakeup_period_work);
	if(atbm_bh_is_term(priv->hw_priv)){
//		wsm_unlock_tx(priv->hw_priv);
		return;
	}
	WARN_ON(wsm_set_beacon_wakeup_period(priv->hw_priv,
		priv->beacon_int * priv->join_dtim_period >
		MAX_BEACON_SKIP_TIME_MS ? 1 :
		priv->join_dtim_period, 0, priv->if_id));
}
#ifdef ATBM_SUPPORT_WOW
u64 atbm_prepare_multicast(struct ieee80211_hw *hw,
			     struct ieee80211_vif *vif,
			     struct netdev_hw_addr_list *mc_list)
{
	static u8 broadcast_ipv6[ETH_ALEN] = {
		0x33, 0x33, 0x00, 0x00, 0x00, 0x01
	};
	static u8 broadcast_ipv4[ETH_ALEN] = {
		0x01, 0x00, 0x5e, 0x00, 0x00, 0x01
	};
	struct netdev_hw_addr *ha;
	int count = 0;
	struct atbm_vif *priv = ABwifi_get_vif_from_ieee80211(vif);

	if (WARN_ON(!priv))
		return netdev_hw_addr_list_count(mc_list);

#ifdef P2P_MULTIVIF
	if (priv->if_id == ATBM_WIFI_GENERIC_IF_ID)
		return 0;
#endif

	/* Disable multicast filtering */
	priv->has_multicast_subscription = false;
	memset(&priv->multicast_filter, 0x00, sizeof(priv->multicast_filter));

	if (netdev_hw_addr_list_count(mc_list) > WSM_MAX_GRP_ADDRTABLE_ENTRIES)
		return 0;

	/* Enable if requested */
	netdev_hw_addr_list_for_each(ha, mc_list) {
		sta_printk( "[STA] multicast: %pM\n", ha->addr);
		memcpy(&priv->multicast_filter.macAddress[count],
		       ha->addr, ETH_ALEN);
		if (memcmp(ha->addr, broadcast_ipv4, ETH_ALEN) &&
				memcmp(ha->addr, broadcast_ipv6, ETH_ALEN))
			priv->has_multicast_subscription = true;
		count++;
	}

	if (count) {

		priv->multicast_filter.enable = __cpu_to_le32(1);
		priv->multicast_filter.numOfAddresses = __cpu_to_le32(count);
	}

	return netdev_hw_addr_list_count(mc_list);
}
#endif
void atbm_configure_filter(struct ieee80211_hw *hw,
			     struct ieee80211_vif *vif,
			     unsigned int changed_flags,
			     unsigned int *total_flags,
			     u64 multicast)
{
	struct atbm_common *hw_priv = hw->priv;
	struct atbm_vif *priv = ABwifi_get_vif_from_ieee80211(vif);
	
	if(atbm_bh_is_term(hw_priv)){
		*total_flags &= ~(1<<31);
		return;
	}
#ifdef P2P_MULTIVIF
	if (priv->if_id == ATBM_WIFI_GENERIC_IF_ID) {
		*total_flags &= ~(1<<31);
		return;
	}
#endif

	*total_flags &= FIF_PROMISC_IN_BSS |
			FIF_OTHER_BSS |
			FIF_FCSFAIL |
			FIF_BCN_PRBRESP_PROMISC |
			FIF_PROBE_REQ;

	down(&hw_priv->scan.lock);
	mutex_lock(&hw_priv->conf_mutex);

	priv->rx_filter.promiscuous = (*total_flags & FIF_PROMISC_IN_BSS)
			? 1 : 0;
	priv->rx_filter.bssid = (*total_flags & (FIF_OTHER_BSS |
			FIF_PROBE_REQ)) ? 1 : 0;
	priv->rx_filter.fcs = (*total_flags & FIF_FCSFAIL) ? 1 : 0;
	priv->bf_control.bcn_count = (*total_flags &
			(FIF_BCN_PRBRESP_PROMISC |
			 FIF_PROMISC_IN_BSS |
			 FIF_PROBE_REQ)) ? 1 : 0;
#if 0
	if (priv->listening ^ listening) {
		priv->listening = listening;
		wsm_lock_tx(hw_priv);
		atbm_update_listening(priv, listening);
		wsm_unlock_tx(hw_priv);
	}
#endif
	atbm_update_filtering(priv);
	mutex_unlock(&hw_priv->conf_mutex);
	up(&hw_priv->scan.lock);
}

int atbm_conf_tx(struct ieee80211_hw *dev, struct ieee80211_vif *vif,
		u16 queue, const struct ieee80211_tx_queue_params *params)
{
	struct atbm_common *hw_priv = dev->priv;
	struct atbm_vif *priv = ABwifi_get_vif_from_ieee80211(vif);
	int ret = 0;
	/* To prevent re-applying PM request OID again and again*/
	bool old_uapsdFlags;
	if(atbm_bh_is_term(hw_priv)){
		return 0;
	}
	if (WARN_ON(!priv))
		return -EOPNOTSUPP;

#ifdef P2P_MULTIVIF
	if (priv->if_id == ATBM_WIFI_GENERIC_IF_ID)
	{
		sta_printk("%s:priv->if_id == ATBM_WIFI_GENERIC_IF_ID\n",__func__);
		return 0;
	}
#endif

	mutex_lock(&hw_priv->conf_mutex);

	if (queue < dev->queues) {
		old_uapsdFlags = priv->uapsd_info.uapsdFlags;

		WSM_TX_QUEUE_SET(&priv->tx_queue_params, queue, 0, 0, 0);
		ret = wsm_set_tx_queue_params(hw_priv,
				&priv->tx_queue_params.params[queue],
				queue, priv->if_id);
		if (ret) {
			sta_printk("%s:wsm_set_tx_queue_params\n",__func__);
			ret = -EINVAL;
			goto out;
		}

                WSM_EDCA_SET(&priv->edca, queue, params->aifs,
                                params->cw_min, params->cw_max, params->txop, 0xc8,
                                params->uapsd);
		ret = wsm_set_edca_params(hw_priv, &priv->edca, priv->if_id);
		if (ret) {
			sta_printk("%s:wsm_set_edca_params\n",__func__);
			ret = -EINVAL;
			goto out;
		}

		if (priv->mode == NL80211_IFTYPE_STATION) {
			ret = atbm_set_uapsd_param(priv, &priv->edca);
			if (!ret && priv->setbssparams_done &&
				(priv->join_status == ATBM_APOLLO_JOIN_STATUS_STA) &&
				(old_uapsdFlags != priv->uapsd_info.uapsdFlags))
				atbm_set_pm(priv, &priv->powersave_mode);
		}
	} else
	{
		sta_printk("%s:queue(%d),dev->queues(%d)\n",__func__,queue,dev->queues);
		ret = -EINVAL;
	}

out:
	mutex_unlock(&hw_priv->conf_mutex);
	return ret;
}

int atbm_get_stats(struct ieee80211_hw *dev,
		     struct ieee80211_low_level_stats *stats)
{
	struct atbm_common *hw_priv = dev->priv;

	memcpy(stats, &hw_priv->stats, sizeof(*stats));
	return 0;
}

/*
int atbm_get_tx_stats(struct ieee80211_hw *dev,
			struct ieee80211_tx_queue_stats *stats)
{
	int i;
	struct atbm_common *priv = dev->priv;

	for (i = 0; i < dev->queues; ++i)
		atbm_queue_get_stats(&priv->tx_queue[i], &stats[i]);

	return 0;
}
*/
int atbm_set_pm(struct atbm_vif *priv, const struct wsm_set_pm *arg)
{
	struct wsm_set_pm pm = *arg;
	struct atbm_vif *other_priv=NULL;

	
	if(priv->if_id>=2){
		atbm_printk_err("%s %d ,ERROR !!! priv->if_id = %d >=2\n",__func__,__LINE__,priv->if_id);
		return -1;
	}
	other_priv = __ABwifi_hwpriv_to_vifpriv(priv->hw_priv,1-priv->if_id);
	/*
	*there is only one interface ,so we send ps mode to firmware directly.
	*/
	if(other_priv == NULL){
		sta_printk( "%s,if(%d),there is only one interface\n",__func__,priv->if_id);
		goto set_self_ps;
	}

	/*
	*there is  no station interface ,so we send ps mode to firmware directly.
	*/
	if(other_priv->join_status != ATBM_APOLLO_JOIN_STATUS_STA)
	{
		sta_printk( "atbm_set_pm(1) set_self_ps(%d)\n",priv->if_id);
		if(other_priv->join_status == ATBM_APOLLO_JOIN_STATUS_AP)
		{
			if(pm.pmMode == WSM_PSM_ACTIVE)
			{
				goto set_self_ps;
			}

			memcpy(&priv->powersave_mode,&pm,sizeof(struct wsm_set_pm));

			return 0;
		}
		goto set_self_ps;
	}

	/*
	*if there is a station disassociated with ap , we only can send active mode
	*to firmware.
	*/
	if(!(other_priv->join_status == ATBM_APOLLO_JOIN_STATUS_STA &&
				other_priv->bss_params.aid &&
				other_priv->setbssparams_done))
	{
		sta_printk( "atbm_set_pm(2) set_self_ps(%d)\n",priv->if_id);
		
		if(pm.pmMode == WSM_PSM_ACTIVE)
		{
			goto set_self_ps;
		}

		memcpy(&priv->powersave_mode,&pm,sizeof(struct wsm_set_pm));

		return 0;
	}

	do
	{
		u8 self_ps_mod = !!pm.pmMode;
		u8 other_ps_mod = !!other_priv->powersave_mode.pmMode;

		#define CHANGE_FW_PS_MODE(change_priv,ps) \
			do										\
			{										\
				if(change_priv->firmware_ps_mode.pmMode != ps) \
				{												\
					change_priv->firmware_ps_mode.pmMode = ps;	\
					wsm_set_pm(change_priv->hw_priv,&change_priv->firmware_ps_mode,	\
					change_priv->if_id);	\
					sta_printk( "change if_id(%d) to ps mode(%x)\n",change_priv->if_id,ps); \
				}							\
			}while(0)
			
		if(priv->join_status == ATBM_APOLLO_JOIN_STATUS_STA)
			memcpy(&priv->powersave_mode,&pm,sizeof(struct wsm_set_pm));
		
		sta_printk( "%s:self_%d(%d),other_%d(%d)\n",__func__,priv->if_id,self_ps_mod,
			other_priv->if_id,other_ps_mod);
		
		if(self_ps_mod^other_ps_mod)
		{
			if(!self_ps_mod)
			{
				/*
				*keep other if alive
				*/
				CHANGE_FW_PS_MODE(other_priv,WSM_PSM_ACTIVE);
			}
			else
			{
				/*
				*keep self alive
				*/
				CHANGE_FW_PS_MODE(priv,WSM_PSM_ACTIVE);	
				sta_printk( "%s:other_if_%d is in ps ,so self_if_%d change into ps\n",
					__func__,other_priv->if_id,priv->if_id);
				return 0;
			}
		}
		else
		{
			if(!self_ps_mod)
			{
				CHANGE_FW_PS_MODE(other_priv,WSM_PSM_ACTIVE);
			}
			else
			{
				if(((!(priv->join_status == ATBM_APOLLO_JOIN_STATUS_STA&&
					priv->bss_params.aid&&priv->setbssparams_done))||(priv->filter4.enable))
					&&other_priv->filter4.enable)
					CHANGE_FW_PS_MODE(other_priv,other_priv->powersave_mode.pmMode);
				else{
					sta_printk( "%s:all interface ip is not enable,so all interface keep alive\n",
					__func__);
					CHANGE_FW_PS_MODE(other_priv,WSM_PSM_ACTIVE);
					CHANGE_FW_PS_MODE(priv,WSM_PSM_ACTIVE);

					return 0;
				}
			}
		}
	}while(0);
	
set_self_ps:
	if((priv->join_status == ATBM_APOLLO_JOIN_STATUS_STA &&
				priv->bss_params.aid &&
				priv->setbssparams_done)){
		sta_printk("%s:set_self_ps,if(%d),pmmode(%x)\n",__func__,
			priv->if_id,pm.pmMode);
		if (priv->uapsd_info.uapsdFlags != 0)
			pm.pmMode &= ~WSM_PSM_FAST_PS_FLAG;
		if (memcmp(&pm, &priv->firmware_ps_mode,
				sizeof(struct wsm_set_pm))) {
			priv->firmware_ps_mode = pm;
			return wsm_set_pm(priv->hw_priv, &pm,
					priv->if_id);
		} else {
			return 0;
		}
	}
	else
	{
		sta_printk( "%s:if_id(%d) is not assco\n",__func__,priv->if_id);
	}

	return 0;
}
u8 aes_mac_key_index = 0;
int atbm_set_key(struct ieee80211_hw *dev, enum set_key_cmd cmd,
		   struct ieee80211_vif *vif, struct ieee80211_sta *sta,
		   struct ieee80211_key_conf *key)
{
	int ret = -EOPNOTSUPP;
	struct atbm_common *hw_priv = dev->priv;
	struct atbm_vif *priv = ABwifi_get_vif_from_ieee80211(vif);

	if(atbm_bh_is_term(hw_priv)){
		return 0;
	}
#ifdef P2P_MULTIVIF
	WARN_ON(priv->if_id == ATBM_WIFI_GENERIC_IF_ID);
#endif
	mutex_lock(&hw_priv->conf_mutex);


	//sta_printk( "%s cmd<%d> cipher<%x> key<%s> keyidx %d pairwise %d \n",__func__,cmd,key->cipher,&key->key[0],key->keyidx,(key->flags & IEEE80211_KEY_FLAG_PAIRWISE));
	{
		int idx = (int)(~0);
		struct wsm_add_key *wsm_key = NULL;
		if(cmd == SET_KEY)
		{
			idx = atbm_alloc_key(hw_priv);
			priv->cipherType = key->cipher;
		}
		else if(cmd == DISABLE_KEY)
		{
			idx = key->hw_key_idx;
		}

		if (idx < 0) {
			ret = -EINVAL;
			sta_printk("cmd(%d),index(%x),err\n",cmd,idx);
			goto finally;
		}
		wsm_key = &hw_priv->keys[idx];
		if(cmd == SET_KEY)
		{
			u8 *peer_addr = NULL;
			int pairwise = (key->flags & IEEE80211_KEY_FLAG_PAIRWISE) ?
			1 : 0;
			key->flags |= IEEE80211_KEY_FLAG_ALLOC_IV;
		//	BUG_ON(pairwise && !sta);
			if(pairwise && !sta){
				atbm_printk_err("%s %d ,ERROR !!! pairwise ,sta is NULL\n",__func__,__LINE__);
				ret = -EINVAL;
				goto finally;
			}
			if (sta)
			peer_addr = sta->addr;
			sta_printk(KERN_ERR "SET_KEY:idx(%d),if_id(%d)\n",idx,priv->if_id);
			switch (key->cipher) {
			case WLAN_CIPHER_SUITE_WEP40:
			case WLAN_CIPHER_SUITE_WEP104:
				if (key->keylen > 16) {
					atbm_free_key(hw_priv, idx);
					ret = -EINVAL;
					goto finally;
				}
				//sta_printk("%s WEP key<%s> keyidx %d,pairwise %d\n",__func__,&key->key[0],key->keyidx,pairwise);
				if (pairwise) {
					wsm_key->type = WSM_KEY_TYPE_WEP_PAIRWISE;
					memcpy(wsm_key->wepPairwiseKey.peerAddress,
						 peer_addr, ETH_ALEN);
					memcpy(wsm_key->wepPairwiseKey.keyData,
						&key->key[0], key->keylen);
					wsm_key->wepPairwiseKey.keyLength = key->keylen;
				} else {
					wsm_key->type = WSM_KEY_TYPE_WEP_DEFAULT;
					memcpy(wsm_key->wepGroupKey.keyData,
						&key->key[0], key->keylen);
					wsm_key->wepGroupKey.keyLength = key->keylen;
					wsm_key->wepGroupKey.keyId = key->keyidx;
				}
				break;
			case WLAN_CIPHER_SUITE_TKIP:
				if (pairwise) {
					wsm_key->type = WSM_KEY_TYPE_TKIP_PAIRWISE;
					memcpy(wsm_key->tkipPairwiseKey.peerAddress,
						peer_addr, ETH_ALEN);
					memcpy(wsm_key->tkipPairwiseKey.tkipKeyData,
						&key->key[0],  16);
					memcpy(wsm_key->tkipPairwiseKey.txMicKey,
						&key->key[16],  8);
					memcpy(wsm_key->tkipPairwiseKey.rxMicKey,
						&key->key[24],  8);
				} else {
					size_t mic_offset =
						(priv->mode == NL80211_IFTYPE_AP) ?
						16 : 24;
					wsm_key->type = WSM_KEY_TYPE_TKIP_GROUP;
					memcpy(wsm_key->tkipGroupKey.tkipKeyData,
						&key->key[0],  16);
					memcpy(wsm_key->tkipGroupKey.rxMicKey,
						&key->key[mic_offset],  8);

					/* TODO: Where can I find TKIP SEQ? */
					memset(wsm_key->tkipGroupKey.rxSeqCounter,
						0,		8);
					wsm_key->tkipGroupKey.keyId = key->keyidx;

				}
				break;
			case WLAN_CIPHER_SUITE_CCMP:
				if (pairwise) {
					wsm_key->type = WSM_KEY_TYPE_AES_PAIRWISE;
					memcpy(wsm_key->aesPairwiseKey.peerAddress,
						peer_addr, ETH_ALEN);
					memcpy(wsm_key->aesPairwiseKey.aesKeyData,
						&key->key[0],  16);
				} else {
					wsm_key->type = WSM_KEY_TYPE_AES_GROUP;
					memcpy(wsm_key->aesGroupKey.aesKeyData,
						&key->key[0],  16);
					/* TODO: Where can I find AES SEQ? */
					memset(wsm_key->aesGroupKey.rxSeqCounter,
						0,              8);
					wsm_key->aesGroupKey.keyId = key->keyidx;
				}
				break;
			case WLAN_CIPHER_SUITE_AES_CMAC: /*add 11W support*/
				{
					struct wsm_protected_mgmt_policy mgmt_policy;
					mgmt_policy.protectedMgmtEnable = 1;
					mgmt_policy.unprotectedMgmtFramesAllowed = 1;
					mgmt_policy.encryptionForAuthFrame = 1;
					wsm_set_protected_mgmt_policy(hw_priv, &mgmt_policy,
						      priv->if_id);
					sta_printk("WLAN_CIPHER_SUITE_AES_CMAC,index(%d)\n",key->keyidx);
					wsm_key->type = WSM_KEY_TYPE_IGTK_GROUP;
					memcpy(wsm_key->igtkGroupKey.igtKeyData, &key->key[0], 16);
					memset(wsm_key->igtkGroupKey.ipn, 0, 8);
					wsm_key->igtkGroupKey.keyId = key->keyidx;
					aes_mac_key_index = key->keyidx;
				}
				break;
#ifdef CONFIG_ATBM_APOLLO_WAPI_SUPPORT
			case WLAN_CIPHER_SUITE_SMS4:
				if (pairwise) {
					wsm_key->type = WSM_KEY_TYPE_WAPI_PAIRWISE;
					memcpy(wsm_key->wapiPairwiseKey.peerAddress,
						peer_addr, ETH_ALEN);
					memcpy(wsm_key->wapiPairwiseKey.wapiKeyData,
						&key->key[0],  16);
					memcpy(wsm_key->wapiPairwiseKey.micKeyData,
						&key->key[16], 16);
					wsm_key->wapiPairwiseKey.keyId = key->keyidx;
				} else {
					wsm_key->type = WSM_KEY_TYPE_WAPI_GROUP;
					memcpy(wsm_key->wapiGroupKey.wapiKeyData,
						&key->key[0],  16);
					memcpy(wsm_key->wapiGroupKey.micKeyData,
						&key->key[16], 16);
					wsm_key->wapiGroupKey.keyId = key->keyidx;
				}
				break;
#endif /* CONFIG_ATBM_APOLLO_WAPI_SUPPORT */
			default:
				WARN_ON(1);
				atbm_free_key(hw_priv, idx);
				ret = -EOPNOTSUPP;
				goto finally;
			}
			
			ret = WARN_ON(wsm_add_key(hw_priv, wsm_key, priv->if_id));
			if (!ret)
				key->hw_key_idx = idx;
			else
				atbm_free_key(hw_priv, idx);
#ifdef CONFIG_ATBM_LMAC_FILTER_IP_FRAME
			if (!ret && (pairwise
				|| wsm_key->type == WSM_KEY_TYPE_WEP_DEFAULT)
				&& (priv->filter4.enable & 0x2))
					atbm_set_arpreply(dev, vif);
#ifdef IPV6_FILTERING
	                if (!ret && (pairwise
				|| wsm_key->type == WSM_KEY_TYPE_WEP_DEFAULT)
				&& (priv->filter6.enable & 0x2))
					atbm_set_na(dev, vif);
#endif /*IPV6_FILTERING*/
#endif
		}
		else if(cmd == DISABLE_KEY)
		{
			if (idx > WSM_KEY_MAX_IDX) {
				ret = -EINVAL;
				goto finally;
			}
			sta_printk(KERN_ERR "DISABLE_KEY:idx(%d),key_type(%d),if_id(%d)\n",idx,wsm_key->type,priv->if_id);
			ret = wsm_remove_key(hw_priv, wsm_key, priv->if_id);
			atbm_free_key(hw_priv,idx);
		}else {
			//BUG_ON("Unsupported command");
			atbm_printk_err("%s %d ,ERROR !!! Unsupported command(%x)\n",__func__,__LINE__,cmd);
		}
	}
finally:
	mutex_unlock(&hw_priv->conf_mutex);
	return ret;
}

void atbm_wep_key_work(struct atbm_work_struct *work)
{
	struct atbm_vif *priv =
		container_of(work, struct atbm_vif , wep_key_work);
	struct atbm_common *hw_priv = ABwifi_vifpriv_to_hwpriv(priv);
	u8 queueId = atbm_queue_get_queue_id(hw_priv->pending_frame_id);
	struct atbm_queue *queue = &hw_priv->tx_queue[queueId];
	__le32 wep_default_key_id = __cpu_to_le32(
		priv->wep_default_key_id);
	
	if(atbm_bh_is_term(hw_priv)){
		#ifdef CONFIG_ATBM_APOLLO_TESTMODE
		
		if(atbm_queue_remove(hw_priv, queue,hw_priv->pending_frame_id)){
			atbm_printk_err("%s %d ,ERROR !!! atbm_queue_remove error\n",__func__,__LINE__);
			//return;
		}
		#else

		if(atbm_queue_remove(queue, hw_priv->pending_frame_id)){
			atbm_printk_err("%s %d ,ERROR !!! atbm_queue_remove error\n",__func__,__LINE__);
			//return;
		}

		#endif
		wsm_unlock_tx(hw_priv);
		return;
	}

	if(queueId >= 4){
		atbm_printk_err("%s %d ,ERROR !!! queueId >= 4\n",__func__,__LINE__);
		wsm_unlock_tx(hw_priv);
		return;
	}	
	sta_printk("[STA] Setting default WEP key: %d\n",
		priv->wep_default_key_id);
	wsm_flush_tx(hw_priv);
	WARN_ON(wsm_write_mib(hw_priv, WSM_MIB_ID_DOT11_WEP_DEFAULT_KEY_ID,
		&wep_default_key_id, sizeof(wep_default_key_id), priv->if_id));
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
	atbm_queue_requeue(hw_priv, queue,
		hw_priv->pending_frame_id, true);
#else
	atbm_queue_requeue(queue, hw_priv->pending_frame_id, true);
#endif
	wsm_unlock_tx(hw_priv);
}
int atbm_tool_rts_threshold = 0;
int atbm_set_rts_threshold(struct ieee80211_hw *hw,
		struct ieee80211_vif *vif, u32 value)
{
	struct atbm_common *hw_priv = hw->priv;
	int ret;
	__le32 val32;
	struct atbm_vif *priv = ABwifi_get_vif_from_ieee80211(vif);
	int if_id = priv->if_id;

	if(atbm_bh_is_term(hw_priv)){
		return 0;
	}
#ifdef P2P_MULTIVIF
	WARN_ON(priv->if_id == ATBM_WIFI_GENERIC_IF_ID);
#endif

	if (value != (u32) -1)
		val32 = __cpu_to_le32(value);
	else
		val32 = 0; /* disabled */
	/* mutex_lock(&priv->conf_mutex); */
	ret = WARN_ON(wsm_write_mib(hw_priv, WSM_MIB_ID_DOT11_RTS_THRESHOLD,
		&val32, sizeof(val32), if_id));
	/* mutex_unlock(&priv->conf_mutex); */
	atbm_tool_rts_threshold = val32;
	return ret;
}
#if 0
int atbm_set_tx_rate_retry_cnt(struct ieee80211_hw *hw,
		struct ieee80211_vif *vif, u8 value1,u8 value2,u8 value3,u8 value4)
{
	struct atbm_common *hw_priv = hw->priv;
	int ret;
	__le32 val32;
	struct atbm_vif *priv = ABwifi_get_vif_from_ieee80211(vif);
	int if_id = priv->if_id;

#ifdef P2P_MULTIVIF
	WARN_ON(priv->if_id == ATBM_WIFI_GENERIC_IF_ID);
#endif

	WARN_ON((value1 >10)||(value2 >10)||(value3 >10)||(value4 >10));

	val32 = value1|
			((value1+value2)<<8)|
			((value1+value2+value3)<<16)|
			((value1+value2+value3+value4)<<24); /* disabled */

	/* mutex_lock(&priv->conf_mutex); */
	ret = WARN_ON(wsm_write_mib(hw_priv, WSM_MIB_ID_DOT11_TRANSMIT_RETRY_CNT,
		&val32, sizeof(val32), if_id));
	/* mutex_unlock(&priv->conf_mutex); */
	return ret;
}
#endif
#ifdef ATBM_SDIO_PATCH
int atbm_remove_seqlist(struct atbm_common *hw_priv)
{
        struct atbm_seq_bit_map *bitmap=NULL,*tmp=NULL;
        spin_lock_bh(&hw_priv->SeqBitMapLock);
        list_for_each_entry_safe(bitmap,tmp,&hw_priv->SeqBitMapList,link){
	        list_del(&bitmap->link);
	        atbm_kfree(bitmap);
        }
        spin_unlock_bh(&hw_priv->SeqBitMapLock);
	return 0;
}
#endif

/* TODO: COMBO: Flush only a particular interface specific parts */
int __atbm_flush(struct atbm_common *hw_priv, bool drop, int if_id)
{
	int i, ret;
	struct atbm_vif *priv =
		__ABwifi_hwpriv_to_vifpriv(hw_priv, if_id);

	if(atbm_bh_is_term(hw_priv)){
		sta_printk("<wifi>atbm_bh_is_term just drop txpacket\n");
		drop = true;
	}

	if(priv == NULL){
		atbm_printk_err("%s:if_id[%d] is already reset\n",__func__,if_id);
		return -1;
	}
	
	for (;;) {
		/* TODO: correct flush handling is required when dev_stop.
		 * Temporary workaround: 2s
		 */
		if (drop) {
			for (i = 0; i < 4; ++i)
				atbm_queue_clear(&hw_priv->tx_queue[i],
					if_id);
		#ifdef ATBM_SDIO_PATCH
			atbm_remove_seqlist(hw_priv);
		#endif		
		} else {
			ret = atbm_wait_event_timeout_stay_awake(hw_priv,
				hw_priv->tx_queue_stats.wait_link_id_empty,
				atbm_queue_stats_is_empty(
					&hw_priv->tx_queue_stats, -1, if_id),
				7* HZ,true);
		}

		if (!drop && unlikely(ret <= 0)) {
			wsm_vif_lock_tx(priv);
			if(hw_priv->hw_bufs_used == 0){
				for (i = 0; i < 4; ++i)
					atbm_queue_clear(&hw_priv->tx_queue[i],if_id);
			}else {
				u8 queueid = 0;
				atbm_printk_err( "__atbm_flush: ETIMEDOUT.....hw_priv->hw_bufs_used(%d),tx_lock(%d)\n",hw_priv->hw_bufs_used,
					atomic_read(&hw_priv->tx_lock));
				for (queueid = 0;queueid<4;queueid++)
					atbm_printk_err("%s:queue[%d]-->pending(%d),vif%d_pending(%d),stats->vif_pend(%d)\n",__func__,queueid,
					hw_priv->tx_queue[queueid].num_pending,if_id,hw_priv->tx_queue[queueid].num_pending_vif[if_id],
					hw_priv->tx_queue_stats.num_queued[if_id]);
				mdelay(1000);
				ret = -ETIMEDOUT;
				wsm_unlock_tx(hw_priv);
				break;
			}
			wsm_unlock_tx(hw_priv);
			ret  = 0;
		} else {
			ret = 0;
		}

		wsm_vif_lock_tx(priv);
		if (unlikely(!atbm_queue_stats_is_empty(
				&hw_priv->tx_queue_stats, -1, if_id))) {
			/* Highly unlekely: WSM requeued frames. */
			wsm_unlock_tx(hw_priv);
			continue;
		}
		break;
	}
	return ret;
}

void atbm_flush(struct ieee80211_hw *hw,
		  struct ieee80211_vif *vif,
		  bool drop)
{
	struct atbm_common *hw_priv = hw->priv;
	struct atbm_vif *priv = ABwifi_get_vif_from_ieee80211(vif);

	/*TODO:COMBO: reenable this part of code when flush callback
	 * is implemented per vif */
	/*switch (hw_priv->mode) {
	case NL80211_IFTYPE_MONITOR:
		drop = true;
		break;
	case NL80211_IFTYPE_AP:
		if (!hw_priv->enable_beacon)
			drop = true;
		break;
	}*/
	if (!(hw_priv->if_id_slot & BIT(priv->if_id)))
		return;

	if (!WARN_ON(__atbm_flush(hw_priv, drop, priv->if_id)))
		wsm_unlock_tx(hw_priv);

	return;
}
#ifdef CONFIG_ATBM_SUPPORT_P2P
int atbm_remain_on_channel(struct ieee80211_hw *hw,
				struct ieee80211_vif *vif,
			     struct ieee80211_channel *chan,
			     enum nl80211_channel_type channel_type,
			     int duration, u64 cookie)
{
	int ret;
	struct atbm_common *hw_priv = hw->priv;
	struct atbm_vif *priv = ABwifi_get_vif_from_ieee80211(vif);
	int if_id = priv->if_id;	
	#ifdef P2P_MULTIVIF
	struct atbm_vif *p2p_priv = __ABwifi_hwpriv_to_vifpriv(hw_priv,1);
	#endif
	if(atbm_bh_is_term(hw_priv)){
		return -1;
	}
	down(&hw_priv->scan.lock);
	mutex_lock(&hw_priv->conf_mutex);

	hw_priv->roc_if_id = priv->if_id;
	hw_priv->roc_start_time = jiffies;
	hw_priv->roc_duration = duration+100;
	smp_mb();
	#ifdef P2P_MULTIVIF
	atbm_printk_mgmt("%s:p2p_priv(%x)\n",__func__,(unsigned int)p2p_priv);
	if(p2p_priv)
		atbm_printk_mgmt( "%s:p2p_priv->join_status(%d)\n",__func__,p2p_priv->join_status);
	if((p2p_priv == NULL) || (p2p_priv->join_status <= ATBM_APOLLO_JOIN_STATUS_IBSS))	
	#else
	if((priv->join_status == ATBM_APOLLO_JOIN_STATUS_PASSIVE))
	#endif
	{
		ret = WARN_ON(__atbm_flush(hw_priv, false, if_id));
		if(!ret){
			wsm_unlock_tx(hw_priv);
			atbm_enable_listening(priv, chan);
			atbm_printk_mgmt( "%s:remain channel ready,duration(%d),(%llx)\n",__func__,duration,cookie);
		}
		//hw_priv->roc_not_send = 0;
	}
	else
	{
		atbm_printk_err("%s:roc_not_send\n",__func__);
		ret = -1;
		//ret = 0;
		//hw_priv->roc_not_send = 1;
	}

	if (!ret) {
		
		if(duration>hw->wiphy->max_remain_on_channel_duration){
			atbm_printk_err("%s %d ,ERROR !!! duration=%d > max_remain_on_channel_duration = %d\n",
				__func__,__LINE__,duration,hw->wiphy->max_remain_on_channel_duration);
			
			mutex_unlock(&hw_priv->conf_mutex);
			up(&hw_priv->scan.lock);
			return -1;
		}
		
		atomic_set(&hw_priv->remain_on_channel, 1);
		atbm_hw_priv_queue_delayed_work(hw_priv,
				   &hw_priv->rem_chan_timeout,
				   (duration+100) * HZ / 1000);
		#ifdef P2P_MULTIVIF
		priv->join_status = ATBM_APOLLO_JOIN_STATUS_MONITOR;
		#endif
		ieee80211_ready_on_channel(hw,100);
	} else {
		hw_priv->roc_if_id = -1;
		atomic_set(&hw_priv->remain_on_channel, 0);
		atbm_printk_err("atbm_remain_on_channel,err if_id(%d)\n", priv->if_id);
		atbm_scan_listenning_restart_delayed(priv);
	}

	/* set the channel to supplied ieee80211_channel pointer, if it
        is not set. This is to remove the crash while sending a probe res
        in listen state. Later channel will updated on
        IEEE80211_CONF_CHANGE_CHANNEL event*/
	if(!hw_priv->channel) {
		hw_priv->channel = chan;
	}
	hw_priv->roc_cookie = cookie;
	mutex_unlock(&hw_priv->conf_mutex);
	if(ret)
		up(&hw_priv->scan.lock);
	return ret;
}

int atbm_cancel_remain_on_channel(struct ieee80211_hw *hw)
{
	struct atbm_common *hw_priv = hw->priv;

	sta_printk( "[STA] Cancel remain on channel\n");

	if (atomic_read(&hw_priv->remain_on_channel))
		atbm_hw_cancel_delayed_work(&hw_priv->rem_chan_timeout,true);

	if (atomic_read(&hw_priv->remain_on_channel))
		atbm_rem_chan_timeout(&hw_priv->rem_chan_timeout.work);

	return 0;
}
#endif
/* ******************************************************************** */
/* WSM callbacks							*/
/*
void atbm_channel_switch_cb(struct atbm_common *hw_priv)
{
	wsm_unlock_tx(hw_priv);
}
*/
void atbm_free_event_queue(struct atbm_common *hw_priv)
{
	LIST_HEAD(list);

	spin_lock_bh(&hw_priv->event_queue_lock);
	list_splice_init(&hw_priv->event_queue, &list);
	spin_unlock_bh(&hw_priv->event_queue_lock);

	__atbm_free_event_queue(&list);
}

void atbm_event_handler(struct atbm_work_struct *work)
{
	struct atbm_common *hw_priv =
		container_of(work, struct atbm_common, event_handler);
	struct atbm_vif *priv;
	struct atbm_wsm_event *event;
	LIST_HEAD(list);

	spin_lock_bh(&hw_priv->event_queue_lock);
	list_splice_init(&hw_priv->event_queue, &list);
	spin_unlock_bh(&hw_priv->event_queue_lock);
	if(atbm_bh_is_term(hw_priv))
	{
		goto event_handler_out;
	}
	mutex_lock(&hw_priv->conf_mutex);
	list_for_each_entry(event, &list, link) {
		priv = __ABwifi_hwpriv_to_vifpriv(hw_priv, event->if_id);
		if (!priv) {
			sta_printk( "[CQM] Event for non existing "
				   "interface, ignoring.\n");
			continue;
		}
		switch (event->evt.eventId) {
			case WSM_EVENT_ERROR:
				/* I even don't know what is it about.. */
				//STUB();
				break;
			case WSM_EVENT_BSS_LOST:
			{
#ifndef CONFIG_TX_NO_CONFIRM
				spin_lock_bh(&priv->bss_loss_lock);
				if (priv->bss_loss_status > ATBM_APOLLO_BSS_LOSS_NONE) {
					spin_unlock_bh(&priv->bss_loss_lock);
					break;
				}
				priv->bss_loss_status = ATBM_APOLLO_BSS_LOSS_CHECKING;
				spin_unlock_bh(&priv->bss_loss_lock);
				sta_printk("[CQM] BSS lost.");
				sta_printk( "[CQM] BSS lost.\n");
				atbm_hw_cancel_delayed_work(&priv->bss_loss_work,true);
				atbm_hw_cancel_delayed_work(&priv->connection_loss_work,true);
				atbm_hw_cancel_delayed_work(&priv->dhcp_retry_work,true);
				if (!down_trylock(&hw_priv->scan.lock)) {
					up(&hw_priv->scan.lock);
					priv->delayed_link_loss = 0;
					atbm_hw_priv_queue_delayed_work(hw_priv,
							&priv->bss_loss_work, 0);
				} else {
					/* Scan is in progress. Delay reporting. */
					/* Scan complete will trigger bss_loss_work */
					priv->delayed_link_loss = 1;
					/* Also we're starting watchdog. */
					atbm_hw_priv_queue_delayed_work(hw_priv,
							&priv->bss_loss_work, 10 * HZ);
				}
#else
				ieee80211_connection_loss(priv->vif);				
#endif
				break;
			}
#ifndef CONFIG_TX_NO_CONFIRM
			case WSM_EVENT_BSS_REGAINED:
			{
				sta_printk( "[CQM] BSS regained.\n");
				priv->delayed_link_loss = 0;
				spin_lock_bh(&priv->bss_loss_lock);
				priv->bss_loss_status = ATBM_APOLLO_BSS_LOSS_NONE;
				spin_unlock_bh(&priv->bss_loss_lock);
				atbm_hw_cancel_delayed_work(&priv->bss_loss_work,true);
				atbm_hw_cancel_delayed_work(&priv->connection_loss_work,true);
				atbm_hw_cancel_delayed_work(&priv->dhcp_retry_work,true);
				break;
			}
#endif
			case WSM_EVENT_RADAR_DETECTED:
				//STUB();
				break;
			case WSM_EVENT_RCPI_RSSI:
			{
#ifdef CONFIG_ATBM_SUPPORT_RCPI_RSSI
				/* RSSI: signed Q8.0, RCPI: unsigned Q7.1
				 * RSSI = RCPI / 2 - 110 */
				int rcpiRssi = (int)(event->evt.eventData & 0xFF);
				int cqm_evt;
#if (PROJ_TYPE>=ARES_A)
				if(rcpiRssi > 128)
					rcpiRssi = rcpiRssi -256;
				else
					rcpiRssi = (s8)rcpiRssi;
#else //ATHEBNAB
					
				if (priv->cqm_use_rssi)
					rcpiRssi = (s8)rcpiRssi;
				else
					rcpiRssi =  rcpiRssi / 2 - 110;
#endif //(PROJ_TYPE>=ARES_A)

				cqm_evt = (rcpiRssi <= priv->cqm_rssi_thold) ?
					NL80211_CQM_RSSI_THRESHOLD_EVENT_LOW :
					NL80211_CQM_RSSI_THRESHOLD_EVENT_HIGH;
				sta_printk( "[CQM] RSSI event: %d", rcpiRssi);
				ieee80211_cqm_rssi_notify(priv->vif, cqm_evt,
									GFP_KERNEL);
#endif
				break;
			}
			case WSM_EVENT_BT_INACTIVE:
				//STUB();
				break;
			case WSM_EVENT_BT_ACTIVE:
				//STUB();
				break;
			case WSM_EVENT_INACTIVITY:
			{
				//int link_id = ffs((u32)(event->evt.eventData)) - 1;
				int link_id = 0;
				int i = 0;
				struct sk_buff *skb;
		        struct atbm_ieee80211_mgmt *deauth;
		        struct atbm_link_entry *entry = NULL;
				for(i = 0;i<ATBMWIFI_MAX_STA_IN_AP_MODE + 1;i++){

					if((BIT(i)&event->evt.eventData) == 0){
						continue;
					}
					
					if(i == 0){
						WARN_ON(1);
						continue;
					}
					
					link_id = i;
					atbm_printk_always("Inactivity entry[%d],[%pM]\n", link_id,priv->link_id_db[link_id-1].mac);
					ABwifi_unmap_link(priv, link_id);

					skb = atbm_dev_alloc_skb(sizeof(struct atbm_ieee80211_mgmt) + 64);
					if(skb == NULL){
						atbm_printk_err("Inactivity Event skb NULL\n");
						continue;
					}
					
					atbm_skb_reserve(skb, 64);
					deauth = (struct atbm_ieee80211_mgmt *)atbm_skb_put(skb, sizeof(struct atbm_ieee80211_mgmt));
                    WARN_ON(!deauth);
                    entry = &priv->link_id_db[link_id - 1];
                    deauth->duration = 0;
                    memcpy(deauth->da, priv->vif->addr, ETH_ALEN);
                    memcpy(deauth->sa, entry->mac/*priv->link_id_db[i].mac*/, ETH_ALEN);
                    memcpy(deauth->bssid, priv->vif->addr, ETH_ALEN);
					deauth->frame_control = cpu_to_le16(IEEE80211_FTYPE_MGMT |
		                                                                    IEEE80211_STYPE_DEAUTH |
		                                                                    IEEE80211_FCTL_TODS);
	                deauth->u.deauth.reason_code = WLAN_REASON_DEAUTH_LEAVING;
	                deauth->seq_ctrl = 0;
		            atbm_ieee80211_rx(hw_priv->hw, skb);
					sta_printk(" Inactivity Deauth Frame sent for MAC SA %pM \t and DA %pM\n", deauth->sa, deauth->da);
					atbm_hw_priv_queue_work(priv->hw_priv, &priv->set_tim_work);
				}
				break;
			}
		case WSM_EVENT_PS_MODE_ERROR:
			{
				sta_printk("[apollo]:PS_MODE_ERROR");
				if (!priv->uapsd_info.uapsdFlags &&
					(priv->user_pm_mode != WSM_PSM_PS))
				{
					struct wsm_set_pm pm = priv->powersave_mode;
					int ret = 0;

					priv->powersave_mode.pmMode = WSM_PSM_ACTIVE;
					ret = atbm_set_pm (priv, &priv->powersave_mode);
					if(ret)
						priv->powersave_mode = pm;
					
#ifdef CONFIG_ATBM_STA_DYNAMIC_PS
					priv->vif->bss_conf.ps_enabled = false;
#endif
				}
                break;
			}
		}
	}
	mutex_unlock(&hw_priv->conf_mutex);
event_handler_out:
	__atbm_free_event_queue(&list);
}
#ifndef CONFIG_TX_NO_CONFIRM
void atbm_bss_loss_work(struct atbm_work_struct *work)
{
	struct atbm_vif *priv =
		container_of(work, struct atbm_vif, bss_loss_work.work);
	struct atbm_common *hw_priv = ABwifi_vifpriv_to_hwpriv(priv);
	int timeout; /* in beacons */

	if(atbm_bh_is_term(hw_priv)){
		return;
	}
	timeout = priv->cqm_link_loss_count -
		priv->cqm_beacon_loss_count;

	/* Skip the confimration procedure in P2P case */
	if (priv->vif->p2p)
		goto report;

	spin_lock_bh(&priv->bss_loss_lock);
	if (priv->bss_loss_status == ATBM_APOLLO_BSS_LOSS_CHECKING) {
//		spin_unlock(&priv->bss_loss_lock);
		priv->bss_loss_status = ATBM_APOLLO_BSS_LOSS_CONFIRMED;
	} else if (priv->bss_loss_status == ATBM_APOLLO_BSS_LOSS_CONFIRMING) {
		priv->bss_loss_status = ATBM_APOLLO_BSS_LOSS_NONE;
		spin_unlock_bh(&priv->bss_loss_lock);
		return;
	}
	spin_unlock_bh(&priv->bss_loss_lock);

report:
	if (priv->cqm_beacon_loss_count) {
		sta_printk( "[CQM] Beacon loss.\n");
		if (timeout <= 0)
			timeout = 0;
//		ieee80211_cqm_beacon_miss_notify(priv->vif, GFP_KERNEL);
	} else {
		timeout = 0;
	}

	atbm_hw_cancel_delayed_work(&priv->connection_loss_work,true);
	atbm_hw_priv_queue_delayed_work(hw_priv,
		&priv->connection_loss_work,
		timeout * HZ / 10);

	spin_lock_bh(&priv->bss_loss_lock);
	priv->bss_loss_status = ATBM_APOLLO_BSS_LOSS_NONE;
	spin_unlock_bh(&priv->bss_loss_lock);
}

void atbm_connection_loss_work(struct atbm_work_struct *work)
{
	struct atbm_vif *priv =
		container_of(work, struct atbm_vif,
				connection_loss_work.work);
	sta_printk( "[CQM] Reporting connection loss.\n");
	if(atbm_bh_is_term(priv->hw_priv)){
		return;
	}
	ieee80211_connection_loss(priv->vif);
}

void atbm_tx_failure_work(struct atbm_work_struct *work)
{
	struct atbm_vif *priv =
		container_of(work, struct atbm_vif, tx_failure_work);
	sta_printk( "[CQM] Reporting TX failure.\n");
	if(atbm_bh_is_term(priv->hw_priv)){
		return;
	}
	ieee80211_connection_loss(priv->vif);
//	ieee80211_cqm_tx_fail_notify(priv->vif, GFP_KERNEL);

}
#endif
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
/**
 * atbm_device_power_calc- Device power calculation
 * from values fetch from SDD File.
 *
 * @priv: the private structure
 * @Max_output_power: Power fetch from SDD
 * @fe_cor: front-end loss correction
 * @band: Either 2GHz or 5GHz
 *
 */
void atbm_device_power_calc(struct atbm_common *hw_priv,
		s16 max_output_power, s16 fe_cor, u32 band)
{
	s16 power_calc;

	power_calc = max_output_power - fe_cor;
	if ((power_calc % 16) != 0)
		power_calc += 16;

	hw_priv->txPowerRange[band].max_power_level = power_calc/16;
	/*
	 * 12dBm is control range supported by firmware.
	 * This means absolute min power is
	 * max_power_level - 12.
	 */
	hw_priv->txPowerRange[band].min_power_level =
		hw_priv->txPowerRange[band].max_power_level - 12;
	hw_priv->txPowerRange[band].stepping = 1;

}
#endif
/* ******************************************************************** */
/* Internal API								*/

#ifdef CONFIG_SUPPORT_SDD

/*
* This function is called to Parse the SDD file
 *to extract listen_interval and PTA related information
*/
static int atbm_parse_SDD_file(struct atbm_common *hw_priv)
{
	u8 *sdd_data = (u8 *)hw_priv->sdd->data;
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
	s16 max_output_power_2G;
	s16 max_output_power_5G;
	s16 fe_cor_2G;
	s16 fe_cor_5G;
	int i;
#endif
	struct atbm_sdd {
		u8 id ;
		u8 length ;
		u8 data[] ;
	} *pElement;
	int parsedLength = 0;
	#define SDD_PTA_CFG_ELT_ID 0xEB
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
	#define SDD_MAX_OUTPUT_POWER_2G4_ELT_ID 0xE3
	#define SDD_MAX_OUTPUT_POWER_5G_ELT_ID  0xE4
	#define SDD_FE_COR_2G4_ELT_ID   0x30
	#define SDD_FE_COR_5G_ELT_ID    0x31
	#define MIN(x, y, z) (x < y ? (x < z ? x : z) : (y < z ? y : z))
#endif
	#define FIELD_OFFSET(type, field) ((u8 *)&((type *)0)->field - (u8 *)0)

	hw_priv->is_BT_Present = false;

	pElement = (struct atbm_sdd *)sdd_data;

	pElement = (struct atbm_sdd *)((u8 *)pElement +
		FIELD_OFFSET(struct atbm_sdd, data) + pElement->length);

	parsedLength += (FIELD_OFFSET(struct atbm_sdd, data) +
			pElement->length);

	while (parsedLength <= hw_priv->sdd->size) {
		switch (pElement->id) {
		case SDD_PTA_CFG_ELT_ID:
		{
			hw_priv->conf_listen_interval =
				(*((u16 *)pElement->data+1) >> 7) & 0x1F;
			hw_priv->is_BT_Present = true;
			sta_printk( "PTA element found.\n");
			sta_printk( "Listen Interval %d\n",
						hw_priv->conf_listen_interval);
		}
		break;
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
		case SDD_MAX_OUTPUT_POWER_2G4_ELT_ID:
		{
			max_output_power_2G =
				*((s16 *)pElement->data);
		}
		break;
		case SDD_FE_COR_2G4_ELT_ID:
		{
			fe_cor_2G =
				*((s16 *)pElement->data);
		}
		break;
		case SDD_MAX_OUTPUT_POWER_5G_ELT_ID:
		{
			max_output_power_5G =
				*((s16 *)(pElement->data + 4));
		}
		break;
		case SDD_FE_COR_5G_ELT_ID:
		{
			fe_cor_5G = MIN(
				*((s16 *)pElement->data),
				*((s16 *)(pElement->data + 2)),
				*((s16 *)(pElement->data + 4)));

			fe_cor_5G = MIN(
				fe_cor_5G,
				*((s16 *)(pElement->data + 6)),
				*((s16 *)(pElement->data + 8)));
		}
		break;
#endif

		default:
		break;
		}

		pElement = (struct atbm_sdd *)
			((u8 *)pElement + FIELD_OFFSET(struct atbm_sdd, data)
					+ pElement->length);
		parsedLength +=
		(FIELD_OFFSET(struct atbm_sdd, data) + pElement->length);
	}

	if (hw_priv->is_BT_Present == false) {
		sta_printk( "PTA element NOT found.\n");
		hw_priv->conf_listen_interval = 0;
	}
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
	/* Max/Min Power Calculation for 2.4G */
	atbm_device_power_calc(hw_priv, max_output_power_2G,
		fe_cor_2G, IEEE80211_BAND_2GHZ);
	/* Max/Min Power Calculation for 5G */
	atbm_device_power_calc(hw_priv, max_output_power_5G,
		fe_cor_5G, IEEE80211_BAND_5GHZ);

	for (i = 0; i < 2; ++i) {
		sta_printk( "[STA] Power Values Read from SDD %s:"
			"min_power_level[%d]: %d max_power_level[%d]:"
			"%d stepping[%d]: %d\n", __func__, i,
			hw_priv->txPowerRange[i].min_power_level, i,
			hw_priv->txPowerRange[i].max_power_level, i,
			hw_priv->txPowerRange[i].stepping);
	}


	sta_printk( "%s output power before %d\n",__func__,hw_priv->output_power);
        /*BUG:TX output power is not set untill config_apollo is called*/
        /*This would lead to 0 power set in fw and would effect scan & p2p-find*/
        /*Setting to default value here from sdd which would be overwritten when*/
        /*we make connection to AP.This value is used only during scan & p2p-ops*/
        /*untill AP connection is made*/
        if (!hw_priv->output_power)
                hw_priv->output_power=hw_priv->txPowerRange[IEEE80211_BAND_2GHZ].max_power_level;

        sta_printk( "%s output power after %d\n",__func__,hw_priv->output_power);
#else
        sta_printk( "%s output power before %d\n",__func__,hw_priv->output_power);
        /*BUG:TX output power: Hardcoding to 20dbm if CCX is not enabled*/
        /*TODO: This might change*/
        if (!hw_priv->output_power)
                hw_priv->output_power=20;
        sta_printk( "%s output power after %d\n",__func__,hw_priv->output_power);
#endif
	return 0;

	#undef SDD_PTA_CFG_ELT_ID
	#undef FIELD_OFFSET
}


static char *sdd = SDD_FILE_DEFAULT_PATH;
module_param(sdd, charp, 0644);
MODULE_PARM_DESC(sdd, "Override sdd file");
#endif

int atbm_setup_mac(struct atbm_common *hw_priv)
{
	int ret = 0;
#ifdef CONFIG_SUPPORT_SDD
	if (0) {
		const char *sdd_path = sdd;
		struct wsm_configuration cfg = {
			.dot11StationId = &hw_priv->mac_addr[0],
		};
		sta_printk( "setup mac addr start :[ %02x,%02x,%02x,%02x,%02x,%02x ] \n",hw_priv->mac_addr[0],
			hw_priv->mac_addr[1],
			hw_priv->mac_addr[2],
			hw_priv->mac_addr[3],
			hw_priv->mac_addr[4],
			hw_priv->mac_addr[5]);

		ret = request_firmware(&hw_priv->sdd,
			sdd_path, hw_priv->pdev);

		if (unlikely(ret)) {
			atbm_dbg(ATBM_APOLLO_DBG_ERROR,
				"%s: can't load sdd file %s.\n",
				__func__, sdd_path);
			return ret;
		}
//#define CONFIG_SUPPORT_SDD
#ifdef CONFIG_SUPPORT_SDD
		cfg.dpdData = (u8*)hw_priv->sdd->data;
		cfg.dpdData_size = (u32)hw_priv->sdd->size;
#else
		cfg.dpdData = NULL;
		cfg.dpdData_size = 0;
#endif
		sta_printk( "setup SDD file len %ld \n",cfg.dpdData_size);

		/* Set low-power mode. */
		ret |= WARN_ON(wsm_configuration(hw_priv, &cfg,0));

		sta_printk( "setup mac addr end :[ %02x,%02x,%02x,%02x,%02x,%02x ] \n",hw_priv->mac_addr[0],
			hw_priv->mac_addr[1],
			hw_priv->mac_addr[2],
			hw_priv->mac_addr[3],
			hw_priv->mac_addr[4],
			hw_priv->mac_addr[5]);
		/* Parse SDD file for PTA element */
		atbm_parse_SDD_file(hw_priv);
	}
	else
#endif
	{
		//const char *sdd_path = sdd;
		struct wsm_configuration cfg = {
			.dot11StationId = &hw_priv->mac_addr[0],
			.dot11RtsThreshold = 1000,
		};
//#define CONFIG_SUPPORT_SDD
#ifdef CONFIG_SUPPORT_SDD
		cfg.dpdData = (u8*)hw_priv->sdd->data;
		cfg.dpdData_size = (u32)hw_priv->sdd->size;
#else
		cfg.dpdData = NULL;
		cfg.dpdData_size = 0;
#endif
		sta_printk("atbm_setup_mac:addr(%pm)\n",&hw_priv->mac_addr[0]);
		ret |= WARN_ON(wsm_configuration(hw_priv, &cfg,0));
	}

	if (ret)
		return ret;

	return 0;
}
#ifdef CONFIG_ATBM_SUPPORT_P2P
void atbm_pending_offchanneltx_work(struct atbm_work_struct *work)
{
	struct atbm_vif *priv =
	container_of(work, struct atbm_vif, pending_offchanneltx_work.work);
	struct atbm_common *hw_priv = ABwifi_vifpriv_to_hwpriv(priv);

	if(atbm_bh_is_term(hw_priv)){
		return;
	}
	mutex_lock(&hw_priv->conf_mutex);
	atbm_disable_listening(priv);
	hw_priv->roc_if_id = -1;
	up(&hw_priv->scan.lock);
	mutex_unlock(&hw_priv->conf_mutex);
}

void atbm_offchannel_work(struct atbm_work_struct *work)
{
	struct atbm_vif *priv =
		container_of(work, struct atbm_vif, offchannel_work);
	struct atbm_common *hw_priv = ABwifi_vifpriv_to_hwpriv(priv);
	u8 queueId = atbm_queue_get_queue_id(hw_priv->pending_frame_id);
	struct atbm_queue *queue = &hw_priv->tx_queue[queueId];
	int listening_on = -1;
	
	if(atbm_bh_is_term(hw_priv)){
		wsm_unlock_tx(hw_priv);
		return;
	}

	if((queueId >= 4) || (!hw_priv->channel)){
		atbm_printk_err("%s %d ,ERROR !!! %s , %s\n",__func__,__LINE__,
				queueId >= 4?"queueId >= 4":" ",hw_priv->channel?"hw_priv->channel is NULL":" ");
		wsm_unlock_tx(hw_priv);
		return;
	}

	
	if (unlikely(down_trylock(&hw_priv->scan.lock))) {
		int ret;
		sta_printk(KERN_ERR "atbm_offchannel_work***** drop frame\n");
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
		ret = atbm_queue_remove(hw_priv, queue,
				hw_priv->pending_frame_id);
#else
		ret = atbm_queue_remove(queue, hw_priv->pending_frame_id);
#endif
		if (ret)
			sta_printk(KERN_ERR "atbm_offchannel_work: "
				       "queue_remove failed %d\n", ret);
		wsm_unlock_tx(hw_priv);
		return;
	}
	mutex_lock(&hw_priv->conf_mutex);
	hw_priv->roc_if_id = priv->if_id;
	if (likely(!priv->join_status)) {
		wsm_vif_flush_tx(priv);
		listening_on = atbm_enable_listening(priv, hw_priv->channel);
		/* atbm_update_filtering(priv); */
	}
	if (unlikely(!priv->join_status)&&(listening_on != 0))
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
		atbm_queue_remove(hw_priv, queue,
				hw_priv->pending_frame_id);
#else
		atbm_queue_remove(queue, hw_priv->pending_frame_id);
#endif /*CONFIG_ATBM_APOLLO_TESTMODE*/
	else
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
		atbm_queue_requeue(hw_priv, queue,
			hw_priv->pending_frame_id, false);
#else
		atbm_queue_requeue(queue, hw_priv->pending_frame_id, false);
#endif
	if(listening_on == 0){
		atbm_hw_priv_queue_delayed_work(hw_priv,
				&priv->pending_offchanneltx_work,msecs_to_jiffies(5));
	}
	else{
		sta_printk(KERN_ERR "atbm_offchannel_work if_id(%d),cannot in listen state\n",
			priv->if_id);		
	}
	mutex_unlock(&hw_priv->conf_mutex);
	if(listening_on){
		hw_priv->roc_if_id = -1;
		up(&hw_priv->scan.lock);
	}
	wsm_unlock_tx(hw_priv);
}
#endif
void atbm_restart_join_bss(struct atbm_vif *priv,struct cfg80211_bss *bss)
{
	const u8 *ssidie;
	const u8 *dtimie;
	const struct ieee80211_tim_ie *tim = NULL;
	struct atbm_common *hw_priv = ABwifi_vifpriv_to_hwpriv(priv);
	struct wsm_operational_mode mode = {
		.power_mode = wsm_power_mode_quiescent,
		.disableMoreFlagUsage = true,
	};
	struct wsm_template_frame frame = {
		.frame_type = WSM_FRAME_TYPE_PROBE_REQUEST,
	};
	int ret = 0;
	struct wsm_protected_mgmt_policy mgmt_policy;
	frame.skb = ieee80211_probereq_get(hw_priv->hw, priv->vif, NULL, 0,
		vif_to_sdata(priv->vif)->last_scan_ie, vif_to_sdata(priv->vif)->last_scan_ie_len,NULL);
	if (!frame.skb)
		return;
	
#ifdef P2P_MULTIVIF
	ret = wsm_set_template_frame(hw_priv, &frame,
		priv->if_id ? 1 : 0);
#else
	ret = wsm_set_template_frame(hw_priv, &frame,
			priv->if_id);
#endif
	if(frame.skb){
		atbm_dev_kfree_skb(frame.skb);
	}
	if(ret)
		return;
	ssidie = ieee80211_bss_get_ie(bss,ATBM_WLAN_EID_SSID);
	dtimie = ieee80211_bss_get_ie(bss,ATBM_WLAN_EID_TIM);
	if (dtimie)
		tim = (struct ieee80211_tim_ie *)&dtimie[2];

	if(atomic_read(&priv->enabled) == 1){
		struct wsm_join join = {
			.mode = (bss->capability & WLAN_CAPABILITY_IBSS) ?
				WSM_JOIN_MODE_IBSS : WSM_JOIN_MODE_BSS,
			.preambleType = WSM_JOIN_PREAMBLE_SHORT,
			.probeForJoin = 1,
			/* dtimPeriod will be updated after association */
			.dtimPeriod = 1,
			.beaconInterval = bss->beacon_interval,
		};

		if (priv->if_id)
			join.flags |= WSM_FLAG_MAC_INSTANCE_1;
		else
			join.flags &= ~WSM_FLAG_MAC_INSTANCE_1;
#ifdef CONFIG_ATBM_BT_COMB
		/* BT Coex related changes */
		if (hw_priv->is_BT_Present) {
			if (((hw_priv->conf_listen_interval * 100) %
					bss->beacon_interval) == 0)
				priv->listen_interval =
					((hw_priv->conf_listen_interval * 100) /
					bss->beacon_interval);
			else
				priv->listen_interval =
					((hw_priv->conf_listen_interval * 100) /
					bss->beacon_interval + 1);
		}
#endif

		if (tim && tim->dtim_period > 1) {
			join.dtimPeriod = tim->dtim_period;
			priv->join_dtim_period = tim->dtim_period;
		}
		priv->beacon_int = bss->beacon_interval;
		sta_printk( "[STA] Join DTIM: %d, interval: %d\n",
				join.dtimPeriod, priv->beacon_int);
		hw_priv->is_go_thru_go_neg = false;
		join.channelNumber = channel_hw_value(hw_priv->channel);
		/* basicRateSet will be updated after association.
		Currently these values are hardcoded */
		if (hw_priv->channel->band == IEEE80211_BAND_5GHZ) {
			join.band = WSM_PHY_BAND_5G;
			join.basicRateSet = 64; /*6 mbps*/
		}else{
			join.band = WSM_PHY_BAND_2_4G;
			join.basicRateSet = 7; /*1, 2, 5.5 mbps*/
		}
		memcpy(&join.bssid[0], bss->bssid, sizeof(join.bssid));
		memcpy(&priv->join_bssid[0], bss->bssid, sizeof(priv->join_bssid));

		if (ssidie) {
			join.ssidLength = ssidie[1];
			if (WARN_ON(join.ssidLength > sizeof(join.ssid)))
				join.ssidLength = sizeof(join.ssid);
			memcpy(&join.ssid[0], &ssidie[2], join.ssidLength);
			if(strstr(&join.ssid[0],"5.1.4"))
				msleep(200);
//#ifdef ROAM_OFFLOAD
			if((priv->vif->type == NL80211_IFTYPE_STATION)) {
				priv->ssid_length = join.ssidLength;
				memcpy(priv->ssid, &join.ssid[0], priv->ssid_length);
			}
//#endif /*ROAM_OFFLOAD*/
		}

		if (priv->vif->p2p) {
			join.flags |= WSM_JOIN_FLAGS_P2P_GO;
#ifdef P2P_MULTIVIF
			join.flags |= (1 << 6);
#endif
			join.basicRateSet =
				atbm_rate_mask_to_wsm(hw_priv, 0xFF0);
		}
		
#ifdef ATBM_SUPPORT_WIDTH_40M		
		if(priv->if_id&&(priv->vif->p2p==true))
		{
			join.channel_type = (u32)(hw_priv->channel_type>NL80211_CHAN_NO_HT ? 
									  NL80211_CHAN_HT20 : NL80211_CHAN_NO_HT);
		}
		else
		{
			spin_lock_bh(&hw_priv->spinlock_channel_type);
			join.channel_type = (u32)hw_priv->channel_type;
			spin_unlock_bh(&hw_priv->spinlock_channel_type);
			if(start_choff<=NL80211_CHAN_HT40PLUS)
				join.channel_type = start_choff;
			sta_printk("%s:join.channel_type(%d)\n",__func__,join.channel_type);
		}
#endif
		memset(&priv->powersave_mode,0,sizeof(struct wsm_set_pm));
		atbm_set_pm(priv,&priv->powersave_mode);
		//WARN_ON(wsm_reset(hw_priv, &reset, priv->if_id));
		WARN_ON(wsm_set_operational_mode(hw_priv, &mode, priv->if_id));
		WARN_ON(wsm_set_block_ack_policy(hw_priv,
			/*hw_priv->ba_tid_mask*/0, hw_priv->ba_tid_rx_mask, priv->if_id));
#ifdef CONFIG_ATBM_BA_STATUS
		spin_lock_bh(&hw_priv->ba_lock);
		hw_priv->ba_ena = false;
		hw_priv->ba_cnt = 0;
		hw_priv->ba_acc = 0;
		hw_priv->ba_hist = 0;
		hw_priv->ba_cnt_rx = 0;
		hw_priv->ba_acc_rx = 0;
		spin_unlock_bh(&hw_priv->ba_lock);
#endif
		mgmt_policy.protectedMgmtEnable = 0;
		mgmt_policy.unprotectedMgmtFramesAllowed = 1;
		mgmt_policy.encryptionForAuthFrame = 1;
		wsm_set_protected_mgmt_policy(hw_priv, &mgmt_policy,
					      priv->if_id);
		atbm_printk_sta("%s:ch(%d),chtype(%d),if_id(%d)\n",__func__,
			join.channelNumber,join.channel_type,priv->if_id);
		wsm_join(hw_priv, &join, priv->if_id);

		priv->join_status = ATBM_APOLLO_JOIN_STATUS_STA;

		/* Due to beacon filtering it is possible that the
		 * AP's beacon is not known for the mac80211 stack.
		 * Disable filtering temporary to make sure the stack
		 * receives at least one */
		priv->disable_beacon_filter = true;
#ifdef CONFIG_ATBM_SUPPORT_P2P
#ifdef ATBM_P2P_CHANGE
		if(priv->if_id == 0){
			atbm_printk_sta("%s:enable combination mode\n",__func__);
			atomic_set(&hw_priv->combination,1);
		}
#endif
#endif
#ifdef	ATBM_WIFI_QUEUE_LOCK_BUG
		atbm_set_priv_queue_cap(priv);
#endif

		atbm_update_filtering(priv);
#ifdef ATBM_P2P_CHANGE
		if(priv->if_id == 1){
			atbm_printk_sta("%s:reset p2p go save\n",__func__);
			atomic_set(&hw_priv->go_bssid_set,0);
			atomic_set(&hw_priv->receive_go_resp,0);
		}
#endif
	}
}
void atbm_join_work(struct atbm_work_struct *work)
{
	struct atbm_vif *priv =
		container_of(work, struct atbm_vif, join_work);
	struct atbm_common *hw_priv = ABwifi_vifpriv_to_hwpriv(priv);
	u8 queueId = atbm_queue_get_queue_id(hw_priv->pending_frame_id);
	struct atbm_queue *queue = &hw_priv->tx_queue[queueId];
	const struct atbm_txpriv *txpriv = NULL;
	struct sk_buff *skb = NULL;
	const struct wsm_tx *wsm;
	const struct ieee80211_hdr *frame;
	const u8 *bssid;
	struct cfg80211_bss *bss;
	const u8 *ssidie;
	const u8 *dtimie;
	const struct ieee80211_tim_ie *tim = NULL;
	struct wsm_protected_mgmt_policy mgmt_policy;
	//struct wsm_reset reset = {
	//	.reset_statistics = true,
	//};
	struct wsm_operational_mode mode = {
		.power_mode = wsm_power_mode_quiescent,
		.disableMoreFlagUsage = true,
	};

	if(queueId >= 4){
		atbm_printk_err("%s %d ,ERROR !!! queueId=%d >= 4\n",__func__,__LINE__,queueId);
		wsm_unlock_tx(hw_priv);
		return;
	}
	if (atbm_queue_get_skb(queue,	hw_priv->pending_frame_id,
			&skb, &txpriv)) {
		wsm_unlock_tx(hw_priv);
		return;
	}

	if(atbm_bh_is_term(hw_priv)){
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
		atbm_queue_remove(hw_priv, queue, hw_priv->pending_frame_id);
#else
		atbm_queue_remove(queue, hw_priv->pending_frame_id);
#endif /*CONFIG_ATBM_APOLLO_TESTMODE*/
		wsm_unlock_tx(hw_priv);
		return;
	}
	wsm = (struct wsm_tx *)&skb->data[0];
	frame = (struct ieee80211_hdr *)&skb->data[txpriv->offset];
	bssid = &frame->addr1[0]; /* AP SSID in a 802.11 frame */

	if((!wsm) || (!hw_priv->channel)){
		atbm_printk_err("%s %d ,ERROR !!! %s , %s\n",__func__,__LINE__,
			wsm?" ":"wsm is NULL",hw_priv->channel?" ":"hw_priv->channel is NULL");
		wsm_unlock_tx(hw_priv);
		return;
	}	

	if (unlikely(priv->join_status)) {
		wsm_lock_tx(hw_priv);
		sta_printk("[%s] line:%d\n", __func__, __LINE__);
		atbm_unjoin_work(&priv->unjoin_work);
	}

	atbm_hw_cancel_delayed_work(&priv->join_timeout,true);

	bss = ieee80211_atbm_get_authen_bss(priv->vif, hw_priv->channel,
			bssid, NULL, 0);
	if (!bss) {
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
		atbm_queue_remove(hw_priv, queue, hw_priv->pending_frame_id);
#else
		atbm_queue_remove(queue, hw_priv->pending_frame_id);
#endif /*CONFIG_ATBM_APOLLO_TESTMODE*/
		wsm_unlock_tx(hw_priv);
		return;
	}
	ssidie = ieee80211_bss_get_ie(bss,ATBM_WLAN_EID_SSID);
	dtimie = ieee80211_bss_get_ie(bss,ATBM_WLAN_EID_TIM);
	if (dtimie)
		tim = (struct ieee80211_tim_ie *)&dtimie[2];

	mutex_lock(&hw_priv->conf_mutex);
	if(atomic_read(&priv->enabled) == 1)
	{
		struct wsm_join join = {
			.mode = (bss->capability & WLAN_CAPABILITY_IBSS) ?
				WSM_JOIN_MODE_IBSS : WSM_JOIN_MODE_BSS,
			.preambleType = WSM_JOIN_PREAMBLE_SHORT,
			.probeForJoin = 1,
			/* dtimPeriod will be updated after association */
			.dtimPeriod = 1,
			.beaconInterval = bss->beacon_interval,
		};

		if (priv->if_id)
			join.flags |= WSM_FLAG_MAC_INSTANCE_1;
		else
			join.flags &= ~WSM_FLAG_MAC_INSTANCE_1;
#ifdef CONFIG_ATBM_BT_COMB
		/* BT Coex related changes */
		if (hw_priv->is_BT_Present) {
			if (((hw_priv->conf_listen_interval * 100) %
					bss->beacon_interval) == 0)
				priv->listen_interval =
					((hw_priv->conf_listen_interval * 100) /
					bss->beacon_interval);
			else
				priv->listen_interval =
					((hw_priv->conf_listen_interval * 100) /
					bss->beacon_interval + 1);
		}
#endif
		if (tim && tim->dtim_period > 1) {
			join.dtimPeriod = tim->dtim_period;
			priv->join_dtim_period = tim->dtim_period;
		}
		priv->beacon_int = bss->beacon_interval;
		sta_printk( "[STA] Join DTIM: %d, interval: %d\n",
				join.dtimPeriod, priv->beacon_int);
		hw_priv->is_go_thru_go_neg = false;
		join.channelNumber = channel_hw_value(hw_priv->channel);
		/* basicRateSet will be updated after association.
		Currently these values are hardcoded */
		if (hw_priv->channel->band == IEEE80211_BAND_5GHZ) {
			join.band = WSM_PHY_BAND_5G;
			join.basicRateSet = 64; /*6 mbps*/
		}else{
			join.band = WSM_PHY_BAND_2_4G;
			join.basicRateSet = 7; /*1, 2, 5.5 mbps*/
		}
		memcpy(&join.bssid[0], bssid, sizeof(join.bssid));
		memcpy(&priv->join_bssid[0], bssid, sizeof(priv->join_bssid));

		if (ssidie) {
			join.ssidLength = ssidie[1];
			if (WARN_ON(join.ssidLength > sizeof(join.ssid)))
				join.ssidLength = sizeof(join.ssid);
			memcpy(&join.ssid[0], &ssidie[2], join.ssidLength);
			if(strstr(&join.ssid[0],"5.1.4"))
				msleep(200);
//#ifdef ROAM_OFFLOAD
			if((priv->vif->type == NL80211_IFTYPE_STATION)) {
				priv->ssid_length = join.ssidLength;
				memcpy(priv->ssid, &join.ssid[0], priv->ssid_length);
			}
//#endif /*ROAM_OFFLOAD*/
		}

		if (priv->vif->p2p) {
			join.flags |= WSM_JOIN_FLAGS_P2P_GO;
#ifdef P2P_MULTIVIF
			join.flags |= (1 << 6);
#endif
			join.basicRateSet =
				atbm_rate_mask_to_wsm(hw_priv, 0xFF0);
		}
		
#ifdef ATBM_SUPPORT_WIDTH_40M		
		if(priv->if_id&&(priv->vif->p2p==true))
		{
			join.channel_type = (u32)(hw_priv->channel_type>NL80211_CHAN_NO_HT ? 
									  NL80211_CHAN_HT20 : NL80211_CHAN_NO_HT);
		}
		else
		{
			spin_lock_bh(&hw_priv->spinlock_channel_type);
			join.channel_type = (u32)hw_priv->channel_type;
			spin_unlock_bh(&hw_priv->spinlock_channel_type);
			if(start_choff<=NL80211_CHAN_HT40PLUS)
				join.channel_type = start_choff;
			sta_printk("%s:join.channel_type(%d)\n",__func__,join.channel_type);
		}
#endif
		wsm_flush_tx(hw_priv);
		/* Queue unjoin if not associated in 3 sec. */
		atbm_hw_priv_queue_delayed_work(hw_priv,
			&priv->join_timeout, 3 * HZ);
		#ifdef CONFIG_PM
		/*Stay Awake for Join Timeout*/
		atbm_pm_stay_awake(&hw_priv->pm_state, 3 * HZ);
		#endif
#if defined(CONFIG_ATBM_STA_LISTEN) || defined(CONFIG_ATBM_SUPPORT_P2P)
		atbm_disable_listening(priv);
#endif
		/*
		*only try to awake othe interface
		*/
		memset(&priv->powersave_mode,0,sizeof(struct wsm_set_pm));
		atbm_set_pm(priv,&priv->powersave_mode);
		//WARN_ON(wsm_reset(hw_priv, &reset, priv->if_id));
		WARN_ON(wsm_set_operational_mode(hw_priv, &mode, priv->if_id));
		WARN_ON(wsm_set_block_ack_policy(hw_priv,
			/*hw_priv->ba_tid_mask*/0, hw_priv->ba_tid_rx_mask, priv->if_id));
#ifdef CONFIG_ATBM_BA_STATUS
		spin_lock_bh(&hw_priv->ba_lock);
		hw_priv->ba_ena = false;
		hw_priv->ba_cnt = 0;
		hw_priv->ba_acc = 0;
		hw_priv->ba_hist = 0;
		hw_priv->ba_cnt_rx = 0;
		hw_priv->ba_acc_rx = 0;
		spin_unlock_bh(&hw_priv->ba_lock);
#endif
		mgmt_policy.protectedMgmtEnable = 0;
		mgmt_policy.unprotectedMgmtFramesAllowed = 1;
		mgmt_policy.encryptionForAuthFrame = 1;
		wsm_set_protected_mgmt_policy(hw_priv, &mgmt_policy,
					      priv->if_id);
		atbm_printk_sta( "%s:ch(%d),chtype(%d),if_id(%d)\n",__func__,
			join.channelNumber,join.channel_type,priv->if_id);
		if (wsm_join(hw_priv, &join, priv->if_id)) {
			memset(&priv->join_bssid[0],
				0, sizeof(priv->join_bssid));
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
			atbm_queue_remove(hw_priv, queue,
						hw_priv->pending_frame_id);
#else
			atbm_queue_remove(queue, hw_priv->pending_frame_id);
#endif /*CONFIG_ATBM_APOLLO_TESTMODE*/
			atbm_hw_cancel_delayed_work(&priv->join_timeout,true);
#ifdef CONFIG_ATBM_SUPPORT_P2P
#ifdef ATBM_P2P_CHANGE
			if(priv->if_id == 0){
				atbm_printk_sta( "%s:disable combination mode\n",__func__);
				atomic_set(&hw_priv->combination,0);				
			}
#endif
#endif
		} else {
			/* Upload keys */
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
			atbm_queue_requeue(hw_priv, queue,
				hw_priv->pending_frame_id, true);
#else
			atbm_queue_requeue(queue, hw_priv->pending_frame_id,
						true);
#endif
			priv->join_status = ATBM_APOLLO_JOIN_STATUS_STA;

			/* Due to beacon filtering it is possible that the
			 * AP's beacon is not known for the mac80211 stack.
			 * Disable filtering temporary to make sure the stack
			 * receives at least one */
			priv->disable_beacon_filter = true;
#ifdef CONFIG_ATBM_SUPPORT_P2P
#ifdef ATBM_P2P_CHANGE
			if(priv->if_id == 0){
				atbm_printk_sta("%s:enable combination mode\n",__func__);
				atomic_set(&hw_priv->combination,1);
			}
#endif
#endif

			#ifdef	ATBM_WIFI_QUEUE_LOCK_BUG
			atbm_set_priv_queue_cap(priv);
			#endif
		}
		atbm_printk_sta("%s:end\n",__func__);
		atbm_update_filtering(priv);
#ifdef CONFIG_ATBM_SUPPORT_P2P
#ifdef ATBM_P2P_CHANGE
		if(priv->if_id == 1){
			atbm_printk_sta( "%s:reset p2p go save\n",__func__);
			atomic_set(&hw_priv->go_bssid_set,0);
			atomic_set(&hw_priv->receive_go_resp,0);
		}
#endif
#endif
	}else {
		atbm_printk_sta("%s:priv is not enable\n",__func__);
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
		atbm_queue_remove(hw_priv, queue, hw_priv->pending_frame_id);
#else
		atbm_queue_remove(queue, hw_priv->pending_frame_id);
#endif /*CONFIG_ATBM_APOLLO_TESTMODE*/
	}
	mutex_unlock(&hw_priv->conf_mutex);
	ieee80211_atbm_put_authen_bss(priv->vif,bss);
	wsm_unlock_tx(hw_priv);
}

void atbm_join_timeout(struct atbm_work_struct *work)
{
	struct atbm_vif *priv =
		container_of(work, struct atbm_vif, join_timeout.work);
	if(atbm_bh_is_term(priv->hw_priv)){
		return;
	}
	sta_printk( "[WSM] Issue unjoin command (TMO).\n");
	wsm_lock_tx(priv->hw_priv);
	sta_printk("[%s] line:%d\n", __func__, __LINE__);
	atbm_unjoin_work(&priv->unjoin_work);
}

void atbm_unjoin_work(struct atbm_work_struct *work)
{
	struct atbm_vif *priv =
		container_of(work, struct atbm_vif, unjoin_work);
	struct atbm_common *hw_priv = ABwifi_vifpriv_to_hwpriv(priv);

	struct wsm_reset reset = {
		.reset_statistics = true,
	};
	bool is_htcapie = false;
	int i;
	struct atbm_vif *tmp_priv;
	struct wsm_operational_mode mode = {
		.power_mode = wsm_power_mode_quiescent,
		.disableMoreFlagUsage = true,
	};
	
	if(atbm_bh_is_term(hw_priv)){
		wsm_unlock_tx(hw_priv);
		return;
	}
#ifdef CONFIG_ATBM_BA_STATUS
	atbm_del_timer_sync(&hw_priv->ba_timer);
#endif
	mutex_lock(&hw_priv->conf_mutex);
	if (unlikely(atomic_read(&hw_priv->scan.in_progress)
#ifdef CONFIG_ATBM_SUPPORT_P2P
		||atomic_read(&hw_priv->remain_on_channel)
#endif
		)) {
		#if 0
		if (priv->delayed_unjoin) {
			wiphy_dbg(priv->hw->wiphy,
				"%s: Delayed unjoin "
				"is already scheduled.\n",
				__func__);
			wsm_unlock_tx(hw_priv);
			sta_printk("%s:priv->delayed_unjoin = true\n",__func__);
		} else {
			priv->delayed_unjoin = true;
			sta_printk("%s:set priv->delayed_unjoin = true\n",__func__);
		}
		#else
		priv->delayed_unjoin = true;
		wsm_unlock_tx(hw_priv);
		#endif
		mutex_unlock(&hw_priv->conf_mutex);
		return;
	}

	if (priv->join_status &&
			priv->join_status > ATBM_APOLLO_JOIN_STATUS_STA) {
		atbm_printk_err("Unexpected: join status: %d\n",priv->join_status);

	//	BUG_ON(1);
		mutex_unlock(&hw_priv->conf_mutex);
		wsm_unlock_tx(hw_priv);
		return;
	}
	if (priv->join_status) {
		atbm_printk_sta("%s:join_status(%d),if_id(%d)\n",__func__,
			priv->join_status,priv->if_id);
#ifdef CONFIG_ATBM_MAC80211_NO_USE
		atbm_hw_cancel_queue_work(&priv->update_filtering_work,true);
#endif
		atbm_hw_cancel_queue_work(&priv->set_beacon_wakeup_period_work,true);
		memset(&priv->join_bssid[0], 0, sizeof(priv->join_bssid));
		priv->join_status = ATBM_APOLLO_JOIN_STATUS_PASSIVE;

#ifdef 	ATBM_PKG_REORDER
		atbm_reorder_func_reset(priv,ATBM_APOLLO_LINK_ID_UNMAPPED-1);
#endif
		/* Unjoin is a reset. */
		wsm_flush_tx(hw_priv);
		
#ifdef ATBM_SUPPORT_WIDTH_40M				
		if(priv->vif->p2p == false)
		{
			sta_printk(KERN_ERR "atbm_unjoin_work:chantype_change_work+++++\n");
#ifdef CONFIG_ATBM_40M_AUTO_CCA
			mutex_unlock(&hw_priv->conf_mutex);
			if(atbm_hw_cancel_delayed_work(&priv->chantype_change_work,false))
					atbm_channel_type_change_work(&priv->chantype_change_work.work);
			if(atbm_work_pending(&hw_priv->get_cca_work))
			{
				atbm_hw_cancel_queue_work(&hw_priv->get_cca_work,true);
				atbm_get_cca_work(&hw_priv->get_cca_work);
			}
			mutex_lock(&hw_priv->conf_mutex);
#endif
			atomic_set(&hw_priv->cca_detect_running,0);
			atomic_set(&hw_priv->tx_20M_lock,0);
			atomic_set(&hw_priv->phy_chantype,0);			
		}
#endif
		WARN_ON(wsm_keep_alive_period(hw_priv, 0, priv->if_id));
		WARN_ON(wsm_reset(hw_priv, &reset, priv->if_id));
		WARN_ON(wsm_set_operational_mode(hw_priv, &mode, priv->if_id));
		WARN_ON(wsm_set_output_power(hw_priv,
			hw_priv->output_power * 10, priv->if_id));
		priv->tmpframe_probereq_set = false;
		priv->join_dtim_period = 0;
		priv->cipherType = 0;
		WARN_ON(atbm_setup_mac_pvif(priv));
		atbm_free_event_queue(hw_priv);
		atbm_hw_cancel_queue_work(&hw_priv->event_handler,true);
#ifndef CONFIG_TX_NO_CONFIRM
		atbm_hw_cancel_delayed_work(&priv->connection_loss_work,true);
#endif
		WARN_ON(wsm_set_block_ack_policy(hw_priv,
			0, hw_priv->ba_tid_rx_mask, priv->if_id));
		priv->disable_beacon_filter = false;
		atbm_update_filtering(priv);
		priv->setbssparams_done = false;
		memset(&priv->association_mode, 0,
			sizeof(priv->association_mode));
		memset(&priv->bss_params, 0, sizeof(priv->bss_params));
		memset(&priv->firmware_ps_mode, 0,
			sizeof(priv->firmware_ps_mode));
		priv->htcap = false;
		do
		{
			/*
			*if there is other interface ,try to change the other interface
			*to ps mode.
			*/
			struct wsm_set_pm pm;
			pm.pmMode = WSM_PSM_PS;
			atbm_set_pm(priv,&pm);
			memset(&priv->powersave_mode, 0,
			sizeof(priv->powersave_mode));
		}while(0);
		atbm_for_each_vif(hw_priv, tmp_priv, i) {
#ifdef P2P_MULTIVIF
			if ((i == (ATBM_WIFI_MAX_VIFS - 1)) || !tmp_priv)
#else
			if (!tmp_priv)
#endif
				continue;
			if ((tmp_priv->join_status == ATBM_APOLLO_JOIN_STATUS_STA) && tmp_priv->htcap)
				is_htcapie = true;
		}

		if (is_htcapie) {
			hw_priv->vif0_throttle = ATBM_WIFI_HOST_VIF0_11N_THROTTLE;
			hw_priv->vif1_throttle = ATBM_WIFI_HOST_VIF1_11N_THROTTLE;
			sta_printk(KERN_ERR "UNJOIN HTCAP 11N %d\n",hw_priv->vif0_throttle);
		} else {
			hw_priv->vif0_throttle = ATBM_WIFI_HOST_VIF0_11BG_THROTTLE;
			hw_priv->vif1_throttle = ATBM_WIFI_HOST_VIF1_11BG_THROTTLE;
			sta_printk(KERN_ERR "UNJOIN 11BG %d\n",hw_priv->vif0_throttle);
		}
		sta_printk( "[STA] Unjoin.\n");
	}
#ifdef CONFIG_ATBM_SUPPORT_P2P
#ifdef ATBM_P2P_CHANGE
	if(priv->if_id == 1){
		atbm_printk_sta("%s:reset p2p go save\n",__func__);
		atomic_set(&hw_priv->go_bssid_set,0);
		atomic_set(&hw_priv->receive_go_resp,0);
		atomic_set(&hw_priv->p2p_oper_channel,0);
	}
	else if(priv->if_id == 0){
		atbm_printk_sta("%s:disable combination mode\n",__func__);
		atomic_set(&hw_priv->combination,0);
	}
#endif
#endif

#ifdef	ATBM_WIFI_QUEUE_LOCK_BUG
	atbm_clear_priv_queue_cap(priv);	
#endif

	mutex_unlock(&hw_priv->conf_mutex);
	wsm_unlock_tx(hw_priv);
}
#if defined(CONFIG_ATBM_STA_LISTEN) || defined(CONFIG_ATBM_SUPPORT_P2P)

int atbm_enable_listening(struct atbm_vif *priv,
				struct ieee80211_channel *chan)
{
	/* TODO:COMBO: Channel is common to HW currently in mac80211.
	Change the code below once channel is made per VIF */
	struct atbm_common *hw_priv = ABwifi_vifpriv_to_hwpriv(priv);
	struct wsm_start start = {
#ifdef P2P_MULTIVIF
		.mode = WSM_START_MODE_P2P_DEV | (priv->if_id ? (1 << 4) : 0),
#else
		.mode = WSM_START_MODE_P2P_DEV | (priv->if_id << 4),
#endif
		.band = (chan->band == IEEE80211_BAND_5GHZ) ?
				WSM_PHY_BAND_5G : WSM_PHY_BAND_2_4G,
		.channelNumber = channel_hw_value(chan),
		.beaconInterval = 100,
		.DTIMPeriod = 1,
		.probeDelay = 0,
		.basicRateSet = 0x0F,
	};
	if((priv->vif->type == NL80211_IFTYPE_P2P_CLIENT) || (priv->vif->type == NL80211_IFTYPE_P2P_GO)){
		start.mode |= BIT(6);
		atbm_printk_err("[P2P MODE] SET BIT(6)");
	}
#ifdef P2P_MULTIVIF
	if(priv->if_id != 2) {
		WARN_ON(priv->join_status > ATBM_APOLLO_JOIN_STATUS_MONITOR);
		sta_printk(KERN_ERR "%s:if_id(%d),priv->join_status(%d)\n",__func__,priv->if_id,
			priv->join_status);
		return -1;
	}
	if (priv->join_status >= ATBM_APOLLO_JOIN_STATUS_MONITOR)
		return 0;
	if (priv->join_status == ATBM_APOLLO_JOIN_STATUS_PASSIVE)
		priv->join_status = ATBM_APOLLO_JOIN_STATUS_MONITOR;

	WARN_ON(priv->join_status > ATBM_APOLLO_JOIN_STATUS_MONITOR);
#endif //change by wp

#ifdef ATBM_SUPPORT_WIDTH_40M
	if(priv->if_id == 0)
		start.channel_type = (u32)(hw_priv->channel_type);
	else
		start.channel_type = NL80211_CHAN_HT20;
#endif
	atbm_printk_sta("if(%d):atbm_enable_listening:channelNumber(%d),channel_type(%d)\n",priv->if_id,start.channelNumber,
	start.channel_type);
//	#ifdef P2P_MULTIVIF
	return wsm_start(hw_priv, &start, ATBM_WIFI_GENERIC_IF_ID);
//	#else
//	return wsm_start(hw_priv, &start, priv->if_id);
//	#endif
}
int atbm_disable_listening(struct atbm_vif *priv)
{
	int ret;
	struct wsm_reset reset = {
		.reset_statistics = true,
	};
#ifdef P2P_MULTIVIF
	if(priv->if_id != 2) {
		WARN_ON(priv->join_status > ATBM_APOLLO_JOIN_STATUS_MONITOR);
        return 0;
	}
#endif //change by wp
	priv->join_status = ATBM_APOLLO_JOIN_STATUS_PASSIVE;

	WARN_ON(priv->join_status > ATBM_APOLLO_JOIN_STATUS_MONITOR);

	if (1
#ifdef CONFIG_ATBM_SUPPORT_P2P
		&&(priv->hw_priv->roc_if_id == -1)
#endif
#ifdef CONFIG_ATBM_STA_LISTEN
		&&(priv->hw_priv->sta_listen_if == -1)
#endif
	   )
		return 0;
	sta_printk( "atbm_disable_listening++++++++\n");
	
//#ifdef P2P_MULTIVIF
	ret = wsm_reset(priv->hw_priv, &reset, ATBM_WIFI_GENERIC_IF_ID);
	atbm_printk_sta("%s:atbm_disable_listening(%d)\n",__func__,priv->if_id);
//#else
//	ret = wsm_reset(priv->hw_priv, &reset, priv->if_id);
//#endif
	return ret;
}
#endif
#ifdef CONFIG_ATBM_STA_LISTEN
static int atbm_sta_enable_listen(struct atbm_vif *priv)
{
	struct atbm_common *hw_priv = ABwifi_vifpriv_to_hwpriv(priv);
	int ret = 0;
	struct ieee80211_channel *chan = NULL;

	if((priv->join_status != ATBM_APOLLO_JOIN_STATUS_PASSIVE)&&
	   (priv->join_status != ATBM_APOLLO_JOIN_STATUS_STA_LISTEN)){
	   if(hw_priv->sta_listen_if_save == -1)
			 hw_priv->sta_listen_if_save = priv->if_id;
	   atbm_printk_err("%s:join status err(%d)\n",__func__,priv->join_status);
	   return -1;
	}
	
	rcu_read_lock();
	chan = rcu_dereference(hw_priv->sta_listen_channel);
	rcu_read_unlock();

	if(chan == NULL){
		hw_priv->sta_listen_if_save = -1;
		hw_priv->sta_listen_if = -1;		
		priv->join_status = ATBM_APOLLO_JOIN_STATUS_PASSIVE;
	   	atbm_printk_err("%s:chan is NULL\n",__func__);
	   	return -1;
	}
	
	ret = WARN_ON(__atbm_flush(hw_priv, false, priv->if_id));
	if(!ret){
		wsm_unlock_tx(hw_priv);
		atbm_enable_listening(priv, chan);
	}
	
	if(ret == 0){
		priv->join_status = ATBM_APOLLO_JOIN_STATUS_STA_LISTEN;
		hw_priv->sta_listen_if = priv->if_id;
		hw_priv->sta_listen_if_save = priv->if_id;
		if(hw_priv->channel == NULL)
			hw_priv->channel = chan;
		atbm_printk_err("%s:vif[%d] in listen\n",__func__,priv->if_id);
	}

	return ret;
}

static int atbm_sta_disable_listen(struct atbm_vif *priv)
{
	struct atbm_common *hw_priv = priv->hw_priv;
	int ret = 0;
	
	atbm_printk_err("%s:(%d)(%d)\n",__func__,priv->if_id,priv->join_status);
	WARN_ON(priv->join_status != ATBM_APOLLO_JOIN_STATUS_STA_LISTEN);
	WARN_ON(priv->if_id != hw_priv->sta_listen_if);
	
	ret = WARN_ON(__atbm_flush(hw_priv, false, priv->if_id));
	if(!ret){
		wsm_unlock_tx(hw_priv);
	}

	atbm_disable_listening(priv);
	smp_mb();
	priv->join_status = ATBM_APOLLO_JOIN_STATUS_PASSIVE;
	atbm_printk_err("%s:vif[%d] exit listen\n",__func__,hw_priv->sta_listen_if);
	hw_priv->sta_listen_if = -1;
	/*make sure all listen pkg processed*/
	synchronize_rcu();
	return ret;
}
int atbm_sta_triger_listen(struct ieee80211_hw *hw,struct ieee80211_vif *vif,struct ieee80211_channel *chan)
{
	struct atbm_common *hw_priv = hw->priv;
	struct atbm_vif *priv = ABwifi_get_vif_from_ieee80211(vif);
	int ret;
	if(atbm_bh_is_term(hw_priv)){
		return -EOPNOTSUPP;
	}

	if(atomic_read(&priv->enabled) == 0){
		atbm_printk_err("%s:priv is not enable\n",__func__);
		return -EOPNOTSUPP;
	}
	atbm_flush_workqueue(hw_priv->workqueue);
	mutex_lock(&hw_priv->conf_mutex);
	rcu_assign_pointer(hw_priv->sta_listen_channel,chan);
	synchronize_rcu();
	if(hw_priv->sta_listen_if != -1){
		struct atbm_vif *listen_priv = __ABwifi_hwpriv_to_vifpriv(hw_priv,hw_priv->sta_listen_if);
		if(listen_priv)
			atbm_sta_disable_listen(listen_priv);
	}
	hw_priv->sta_listen_if_save = -1;
	hw_priv->sta_listen_if = -1;
	smp_mb();
	ret = atbm_sta_enable_listen(priv);
	mutex_unlock(&hw_priv->conf_mutex);

	return ret;
}
int atbm_sta_stop_listen(struct ieee80211_hw *hw,struct ieee80211_vif *vif)
{
	struct atbm_common *hw_priv = hw->priv;
	struct atbm_vif *priv = ABwifi_get_vif_from_ieee80211(vif);

	if(atbm_bh_is_term(hw_priv)){		
		atbm_printk_err("%s:bh is disable\n",__func__);
		return -EOPNOTSUPP;
	}

	if(atomic_read(&priv->enabled) == 0){
		atbm_printk_err("%s:priv is not enable\n",__func__);
		return -EOPNOTSUPP;
	}
	
	atbm_flush_workqueue(hw_priv->workqueue);

	mutex_lock(&hw_priv->conf_mutex);
	atbm_sta_disable_listen(priv);
	mutex_unlock(&hw_priv->conf_mutex);
	return 0;
}
void atbm_sta_listen_int(struct atbm_common *hw_priv)
{
	hw_priv->sta_listen_channel = NULL;
	hw_priv->sta_listen_if = -1;
	hw_priv->sta_listen_if_save = -1;
}
#endif
/* TODO:COMBO:UAPSD will be supported only on one interface */
int atbm_set_uapsd_param(struct atbm_vif *priv,
				const struct wsm_edca_params *arg)
{
	struct atbm_common *hw_priv = ABwifi_vifpriv_to_hwpriv(priv);
	int ret;
	u16 uapsdFlags = 0;

	/* Here's the mapping AC [queue, bit]
	VO [0,3], VI [1, 2], BE [2, 1], BK [3, 0]*/

	if (arg->params[0].uapsdEnable)
		uapsdFlags |= 1 << 3;

	if (arg->params[1].uapsdEnable)
		uapsdFlags |= 1 << 2;

	if (arg->params[2].uapsdEnable)
		uapsdFlags |= 1 << 1;

	if (arg->params[3].uapsdEnable)
		uapsdFlags |= 1;

	/* Currently pseudo U-APSD operation is not supported, so setting
	* MinAutoTriggerInterval, MaxAutoTriggerInterval and
	* AutoTriggerStep to 0 */

	priv->uapsd_info.uapsdFlags = cpu_to_le16(uapsdFlags);
	priv->uapsd_info.minAutoTriggerInterval = 0;
	priv->uapsd_info.maxAutoTriggerInterval = 0;
	priv->uapsd_info.autoTriggerStep = 0;
	sta_printk("%s:uapsdFlags(%x)\n",__func__,uapsdFlags);
	ret = wsm_set_uapsd_info(hw_priv, &priv->uapsd_info,
				 priv->if_id);
	return ret;
}
#ifdef CONFIG_ATBM_BA_STATUS
void atbm_ba_work(struct atbm_work_struct *work)
{
	struct atbm_common *hw_priv =
		container_of(work, struct atbm_common, ba_work);
	//u8 tx_ba_tid_mask;

	/* TODO:COMBO: reenable this part of code */
/*	if (priv->join_status != ATBM_APOLLO_JOIN_STATUS_STA)
		return;
	if (!priv->setbssparams_done)
		return;*/

	sta_printk("BA work****\n");
	spin_lock_bh(&hw_priv->ba_lock);
//	tx_ba_tid_mask = hw_priv->ba_ena ? hw_priv->ba_tid_mask : 0;
	//tx_ba_tid_mask = hw_priv->ba_tid_mask;
	spin_unlock_bh(&hw_priv->ba_lock);

	wsm_lock_tx(hw_priv);

	WARN_ON(wsm_set_block_ack_policy(hw_priv,
		hw_priv->ba_tid_tx_mask, hw_priv->ba_tid_rx_mask, -1)); /*TODO:COMBO*/

	wsm_unlock_tx(hw_priv);
}

void atbm_ba_timer(unsigned long arg)
{
	bool ba_ena;
	struct atbm_common *hw_priv =
		(struct atbm_common *)arg;

	spin_lock_bh(&hw_priv->ba_lock);
	atbm_debug_ba(hw_priv, hw_priv->ba_cnt, hw_priv->ba_acc,
			hw_priv->ba_cnt_rx, hw_priv->ba_acc_rx);

	if (atomic_read(&hw_priv->scan.in_progress)) {
		hw_priv->ba_cnt = 0;
		hw_priv->ba_acc = 0;
		hw_priv->ba_cnt_rx = 0;
		hw_priv->ba_acc_rx = 0;
		goto skip_statistic_update;
	}

	if (hw_priv->ba_cnt >= ATBM_APOLLO_BLOCK_ACK_CNT &&
		(hw_priv->ba_acc / hw_priv->ba_cnt >= ATBM_APOLLO_BLOCK_ACK_THLD ||
		(hw_priv->ba_cnt_rx >= ATBM_APOLLO_BLOCK_ACK_CNT &&
		hw_priv->ba_acc_rx / hw_priv->ba_cnt_rx >=
			ATBM_APOLLO_BLOCK_ACK_THLD)))
		ba_ena = true;
	else
		ba_ena = false;

	hw_priv->ba_cnt = 0;
	hw_priv->ba_acc = 0;
	hw_priv->ba_cnt_rx = 0;
	hw_priv->ba_acc_rx = 0;

	if (ba_ena != hw_priv->ba_ena) {
		if (ba_ena || ++hw_priv->ba_hist >= ATBM_APOLLO_BLOCK_ACK_HIST) {
			hw_priv->ba_ena = ba_ena;
			hw_priv->ba_hist = 0;
#if 0
			sta_printk( "[STA] %s block ACK:\n",
				ba_ena ? "enable" : "disable");
			atbm_queue_work(hw_priv->workqueue, &hw_priv->ba_work);
#endif
		}
	} else if (hw_priv->ba_hist)
		--hw_priv->ba_hist;

skip_statistic_update:
	spin_unlock_bh(&hw_priv->ba_lock);
}
#endif
void atbm_vif_setup_params(struct atbm_vif *priv)
{
	/* Setup per vif workitems and locks */
	spin_lock_init(&priv->vif_lock);
	ATBM_INIT_WORK(&priv->join_work, atbm_join_work);
	ATBM_INIT_DELAYED_WORK(&priv->join_timeout, atbm_join_timeout);
	ATBM_INIT_WORK(&priv->unjoin_work, atbm_unjoin_work);
	ATBM_INIT_WORK(&priv->wep_key_work, atbm_wep_key_work);
#ifdef CONFIG_ATBM_SUPPORT_P2P
	ATBM_INIT_WORK(&priv->offchannel_work, atbm_offchannel_work);
	ATBM_INIT_DELAYED_WORK(&priv->pending_offchanneltx_work,
			atbm_pending_offchanneltx_work);
#endif
#ifndef CONFIG_TX_NO_CONFIRM
	ATBM_INIT_DELAYED_WORK(&priv->bss_loss_work, atbm_bss_loss_work);
	ATBM_INIT_DELAYED_WORK(&priv->connection_loss_work,
			  atbm_connection_loss_work);
	spin_lock_init(&priv->bss_loss_lock);
	ATBM_INIT_WORK(&priv->tx_failure_work, atbm_tx_failure_work);
#endif
	spin_lock_init(&priv->ps_state_lock);
//	ATBM_INIT_DELAYED_WORK(&priv->set_cts_work, atbm_set_cts_work);
	ATBM_INIT_WORK(&priv->set_tim_work, atbm_set_tim_work);
	ATBM_INIT_WORK(&priv->multicast_start_work, atbm_multicast_start_work);
	ATBM_INIT_WORK(&priv->multicast_stop_work, atbm_multicast_stop_work);
	ATBM_INIT_WORK(&priv->link_id_work, atbm_link_id_work);
	ATBM_INIT_DELAYED_WORK(&priv->link_id_gc_work, atbm_link_id_gc_work);
#if 0
	ATBM_INIT_WORK(&priv->linkid_reset_work, atbm_link_id_reset);
#endif
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	ATBM_INIT_WORK(&priv->update_filtering_work, atbm_update_filtering_work);
#endif
	ATBM_INIT_WORK(&priv->set_beacon_wakeup_period_work,
		atbm_set_beacon_wakeup_period_work);
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,3,0))
    ATBM_INIT_WORK(&priv->ht_info_update_work, atbm_ht_info_update_work);
#endif
#ifdef ATBM_SUPPORT_WIDTH_40M	
#ifdef CONFIG_ATBM_40M_AUTO_CCA
	ATBM_INIT_DELAYED_WORK(&priv->chantype_change_work, atbm_channel_type_change_work);
#endif
#endif
	atbm_init_timer(&priv->mcast_timeout);
	//DHCP
	spin_lock_init(&priv->dhcp_retry_spinlock);
	ATBM_INIT_DELAYED_WORK(&priv->dhcp_retry_work, atbm_dhcp_retry_work);
	spin_lock_bh(&priv->dhcp_retry_spinlock);
	priv->dhcp_retry_skb=NULL;
	spin_unlock_bh(&priv->dhcp_retry_spinlock);
	priv->mcast_timeout.data = (unsigned long)priv;
	priv->mcast_timeout.function = atbm_mcast_timeout;
	priv->setbssparams_done = false;
	priv->power_set_true = 0;
	priv->user_power_set_true = 0;
	priv->user_pm_mode = 0;


	/* Initialising the broadcast filter */
	memset(priv->broadcast_filter.MacAddr, 0xFF, ETH_ALEN);
	priv->broadcast_filter.nummacaddr = 1;
	priv->broadcast_filter.address_mode = 1;
	priv->broadcast_filter.filter_mode = 1;
	priv->htcap = false;
#ifdef ATBM_SUPPORT_WIDTH_40M
	priv->join_status = ATBM_APOLLO_JOIN_STATUS_PASSIVE;
#endif
	sta_printk(KERN_ERR " !!! %s: enabling priv\n", __func__);
	atomic_set(&priv->enabled, 1);
}
int atbm_vif_setup(struct atbm_vif *priv)
{
	struct atbm_common *hw_priv = priv->hw_priv;
	int ret = 0;

#ifdef P2P_MULTIVIF
	if (priv->if_id < 2) {
#endif
		/* default EDCA */
		WSM_EDCA_SET(&priv->edca, 0, 0x0002, 0x0003, 0x0007,
				47, 0xc8, false);
		WSM_EDCA_SET(&priv->edca, 1, 0x0002, 0x0007, 0x000f,
				94, 0xc8, false);
			WSM_EDCA_SET(&priv->edca, 2, 0x0003, 0x000f, 0x03ff,
				0, 0xc8, false);
		WSM_EDCA_SET(&priv->edca, 3, 0x0007, 0x000f, 0x03ff,
				0, 0xc8, false);
		ret = wsm_set_edca_params(hw_priv, &priv->edca, priv->if_id);
		if (WARN_ON(ret))
			goto out;

		ret = atbm_set_uapsd_param(priv, &priv->edca);
		if (WARN_ON(ret))
			goto out;

		memset(priv->bssid, ~0, ETH_ALEN);
		priv->wep_default_key_id = -1;
		priv->cipherType = 0;
		priv->cqm_link_loss_count = 40;
		priv->cqm_beacon_loss_count = 20;

		/* Temporary configuration - beacon filter table */
		__atbm_bf_configure(priv);
#ifdef P2P_MULTIVIF
	}
#endif
out:
	return ret;
}

int atbm_setup_mac_pvif(struct atbm_vif *priv)
{
	int ret = 0;
	/* NOTE: There is a bug in FW: it reports signal
	* as RSSI if RSSI subscription is enabled.
	* It's not enough to set WSM_RCPI_RSSI_USE_RSSI. */
	/* NOTE2: RSSI based reports have been switched to RCPI, since
	* FW has a bug and RSSI reported values are not stable,
	* what can leads to signal level oscilations in user-end applications */
	struct wsm_rcpi_rssi_threshold threshold = {
		.rssiRcpiMode = WSM_RCPI_RSSI_THRESHOLD_ENABLE |
		WSM_RCPI_RSSI_DONT_USE_UPPER |
		WSM_RCPI_RSSI_DONT_USE_LOWER,
		.rollingAverageCount = 16,
	};

	/* Remember the decission here to make sure, we will handle
	 * the RCPI/RSSI value correctly on WSM_EVENT_RCPI_RSS */
	if (threshold.rssiRcpiMode & WSM_RCPI_RSSI_USE_RSSI)
		priv->cqm_use_rssi = true;


	/* Configure RSSI/SCPI reporting as RSSI. */
#ifdef P2P_MULTIVIF
	ret = wsm_set_rcpi_rssi_threshold(priv->hw_priv, &threshold,
					priv->if_id ? 1 : 0);
#else
	ret = wsm_set_rcpi_rssi_threshold(priv->hw_priv, &threshold,
					priv->if_id);
#endif
#ifdef IPC_AP_USED_11G_NO_RTS
	if (priv->mode != NL80211_IFTYPE_STATION)
	{
		wsm_set_rts_threshold(priv->hw_priv,priv->if_id);	
	}
#endif
	return ret;
}
#ifdef CONFIG_ATBM_SUPPORT_P2P
void atbm_rem_chan_timeout(struct atbm_work_struct *work)
{
	struct atbm_common *hw_priv =
		container_of(work, struct atbm_common, rem_chan_timeout.work);
	int ret, if_id;
	struct atbm_vif *priv;
	if (atomic_read(&hw_priv->remain_on_channel) == 0) {
		return;
	}
	
	atbm_printk_mgmt("%s:roc_cookie(%llx)\n",__func__,hw_priv->roc_cookie);
	ieee80211_remain_on_channel_expired(hw_priv->hw, hw_priv->roc_cookie);
	mutex_lock(&hw_priv->conf_mutex);
	if_id = hw_priv->roc_if_id;
	priv = __ABwifi_hwpriv_to_vifpriv(hw_priv, if_id);
	if(atbm_bh_is_term(hw_priv))
	{
		goto chan_timeout_exit;
	}
//	if(hw_priv->roc_not_send == 0)
	{
		ret = WARN_ON(__atbm_flush(hw_priv, false, if_id));

		if (!ret) {
			wsm_unlock_tx(hw_priv);
			atbm_disable_listening(priv);
		}
	}
chan_timeout_exit:
	atomic_set(&hw_priv->remain_on_channel, 0);
	hw_priv->roc_if_id = -1;

	atbm_scan_listenning_restart_delayed(priv);
	mutex_unlock(&hw_priv->conf_mutex);
	up(&hw_priv->scan.lock);
}
#endif
const u8 *atbm_get_ie(u8 *start, size_t len, u8 ie)
{
	u8 *end, *pos;

	pos = start;
	if (pos == NULL)
		return NULL;
	end = pos + len;

	while (pos + 1 < end) {
		if (pos + 2 + pos[1] > end)
			break;
		if (pos[0] == ie)
			return pos;
		pos += 2 + pos[1];
	}

	return NULL;
}
#ifdef ATBM_SUPPORT_WOW
/**
 * atbm_set_macaddrfilter -called when tesmode command
 * is for setting mac address filter
 *
 * @hw: the hardware
 * @data: incoming data
 *
 * Returns: 0 on success or non zero value on failure
 */
int atbm_set_macaddrfilter(struct atbm_common *hw_priv, struct atbm_vif *priv, u8 *data)
{
	struct wsm_mac_addr_filter *mac_addr_filter =  NULL;
	struct wsm_mac_addr_info *addr_info = NULL;
	u8 action_mode = 0, no_of_mac_addr = 0, i = 0;
	int ret = 0;
	u16 macaddrfiltersize = 0;

	/* Retrieving Action Mode */
	action_mode = data[0];
	/* Retrieving number of address entries */
	no_of_mac_addr = data[1];

	addr_info = (struct wsm_mac_addr_info *)&data[2];

	/* Computing sizeof Mac addr filter */
	macaddrfiltersize =  sizeof(*mac_addr_filter) + \
			(no_of_mac_addr * sizeof(struct wsm_mac_addr_info));

	mac_addr_filter = atbm_kzalloc(macaddrfiltersize, GFP_KERNEL);
	if (!mac_addr_filter) {
		ret = -ENOMEM;
		goto exit_p;
	}
	mac_addr_filter->action_mode = action_mode;
	mac_addr_filter->numfilter = no_of_mac_addr;

	for (i = 0; i < no_of_mac_addr; i++) {
		mac_addr_filter->macaddrfilter[i].address_mode = \
						addr_info[i].address_mode;
		memcpy(mac_addr_filter->macaddrfilter[i].MacAddr, \
				addr_info[i].MacAddr , ETH_ALEN);
		mac_addr_filter->macaddrfilter[i].filter_mode = \
						addr_info[i].filter_mode;
	}
	ret = WARN_ON(wsm_write_mib(hw_priv, WSM_MIB_ID_MAC_ADDR_FILTER, \
					 mac_addr_filter, macaddrfiltersize, priv->if_id));

	atbm_kfree(mac_addr_filter);
exit_p:
	return ret;
}
#endif
#if 0
/**
 * atbm_set_multicastaddrfilter -called when tesmode command
 * is for setting the ipv4 address filter
 *
 * @hw: the hardware
 * @data: incoming data
 *
 * Returns: 0 on success or non zero value on failure
 */
static int atbm_set_multicastfilter(struct atbm_common *hw_priv, struct atbm_vif *priv, u8 *data)
{
	u8 i = 0;
	int ret = 0;

	memset(&priv->multicast_filter, 0, sizeof(priv->multicast_filter));
	priv->multicast_filter.enable = (u32)data[0];
	priv->multicast_filter.numOfAddresses = (u32)data[1];

	for (i = 0; i < priv->multicast_filter.numOfAddresses; i++) {
		memcpy(&priv->multicast_filter.macAddress[i], \
			   &data[2+(i*ETH_ALEN)], ETH_ALEN);
	}
	/* Configure the multicast mib in case of drop all multicast */
	if (priv->multicast_filter.enable != 2)
		return ret;

	ret = wsm_write_mib(hw_priv, WSM_MIB_ID_DOT11_GROUP_ADDRESSES_TABLE, \
		&priv->multicast_filter, sizeof(priv->multicast_filter), priv->if_id);

	return ret;
}
#endif
#ifdef CONFIG_ATBM_LMAC_FILTER_IP_FRAME
#ifdef IPV6_FILTERING
/**
 * atbm_set_ipv6addrfilter -called when tesmode command
 * is for setting the ipv6 address filter
 *
 * @hw: the hardware
 * @data: incoming data
 * @if_id: interface id
 *
 * Returns: 0 on success or non zero value on failure
 */
static int atbm_set_ipv6addrfilter(struct ieee80211_hw *hw,
				     u8 *data, int if_id)
{
	struct atbm_common *hw_priv = (struct atbm_common *) hw->priv;
	struct wsm_ipv6_filter  *ipv6_filter =  NULL;
	struct ipv6_addr_info *ipv6_info = NULL;
	u8 action_mode = 0, no_of_ip_addr = 0, i = 0, ret = 0;
	u16 ipaddrfiltersize = 0;

	/* Retrieving Action Mode */
	action_mode = data[0];
	/* Retrieving number of ipv4 address entries */
	no_of_ip_addr = data[1];

	ipv6_info = (struct ipv6_addr_info *)&data[2];

	/* Computing sizeof Mac addr filter */
	ipaddrfiltersize =  sizeof(*ipv6_filter) + \
			(no_of_ip_addr * sizeof(struct wsm_ip6_addr_info));


	ipv6_filter = atbm_kzalloc(ipaddrfiltersize, GFP_KERNEL);
	if (!ipv6_filter) {
		ret = -ENOMEM;
		goto exit_p;
	}
	ipv6_filter->action_mode = action_mode;
	ipv6_filter->numfilter = no_of_ip_addr;

	for (i = 0; i < no_of_ip_addr; i++) {
		ipv6_filter->ipv6filter[i].address_mode = \
					ipv6_info[i].address_mode;
		ipv6_filter->ipv6filter[i].filter_mode = \
					ipv6_info[i].filter_mode;
		memcpy(ipv6_filter->ipv6filter[i].ipv6, \
					(u8 *)(ipv6_info[i].ipv6), 16);
	}

	ret = WARN_ON(wsm_write_mib(hw_priv, WSM_MIB_IP_IPV6_ADDR_FILTER, \
					 ipv6_filter, ipaddrfiltersize, \
					 if_id));

	atbm_kfree(ipv6_filter);
exit_p:
	return ret;
}
#endif /*IPV6_FILTERING*/

/**
 * atbm_set_data_filter -configure data filter in device
*
 * @hw: the hardware
 * @vif: vif
 * @data: incoming data
 * @len: incoming data length
 *
 */
void atbm_set_data_filter(struct ieee80211_hw *hw,
			   struct ieee80211_vif *vif,
			   void *data, int len)
{
	int ret = 0;
	int filter_id;

	if (!data) {
		ret = -EINVAL;
		goto exit_p;
	}
	filter_id=*((enum atbm_data_filterid*)data);

	switch (filter_id) {
#ifdef IPV6_FILTERING
	case IPV6ADDR_FILTER_ID:
		ret = atbm_set_ipv6addrfilter(hw, \
			&((u8 *)data)[4], priv->if_id);
		break;
#endif /*IPV6_FILTERING*/
	default:
		ret = -EINVAL;
		break;
	}
exit_p:
	ret = ret;
	 return ;
}
/**
 * atbm_set_arpreply -called for creating and
 * configuring arp response template frame
 *
 * @hw: the hardware
 *
 * Returns: 0 on success or non zero value on failure
 */
int atbm_set_arpreply(struct ieee80211_hw *hw, struct ieee80211_vif *vif)
{
	struct atbm_vif *priv = ABwifi_get_vif_from_ieee80211(vif);
	struct atbm_common *hw_priv = (struct atbm_common *)hw->priv;
        u32 framehdrlen, encrypthdr, encrypttailsize, framebdylen = 0;
        bool encrypt = false;
        int ret = 0;
        u8 *template_frame = NULL;
        struct ieee80211_hdr_3addr *dot11hdr = NULL;
        struct ieee80211_snap_hdr *snaphdr = NULL;
        struct arphdr *arp_hdr = NULL;

        template_frame = atbm_kzalloc(MAX_ARP_REPLY_TEMPLATE_SIZE, GFP_ATOMIC);
        if (!template_frame) {
                sta_printk( "[STA] Template frame memory failed\n");
                ret = -ENOMEM;
                goto exit_p;
        }
        dot11hdr = (struct ieee80211_hdr_3addr *)&template_frame[4];

        framehdrlen = sizeof(*dot11hdr);
        if ((priv->vif->type == NL80211_IFTYPE_AP) && priv->vif->p2p)
                priv->cipherType = WLAN_CIPHER_SUITE_CCMP;
        switch (priv->cipherType) {

        case WLAN_CIPHER_SUITE_WEP40:
        case WLAN_CIPHER_SUITE_WEP104:
                sta_printk( "[STA] WEP\n");
                encrypthdr = WEP_ENCRYPT_HDR_SIZE;
                encrypttailsize = WEP_ENCRYPT_TAIL_SIZE;
                encrypt = 1;
                break;


        case WLAN_CIPHER_SUITE_TKIP:
                sta_printk( "[STA] WPA\n");
                encrypthdr = WPA_ENCRYPT_HDR_SIZE;
                encrypttailsize = WPA_ENCRYPT_TAIL_SIZE;
                encrypt = 1;
                break;

        case WLAN_CIPHER_SUITE_CCMP:
                sta_printk( "[STA] WPA2\n");
                encrypthdr = WPA2_ENCRYPT_HDR_SIZE;
                encrypttailsize = WPA2_ENCRYPT_TAIL_SIZE;
                encrypt = 1;
                break;

		case WLAN_CIPHER_SUITE_AES_CMAC:
                sta_printk( "[STA] WPA2\n");
                encrypthdr = WPA2_ENCRYPT_HDR_SIZE;
                encrypttailsize = WPA2_ENCRYPT_TAIL_SIZE;
                encrypt = 1;
                break;
	#ifdef CONFIG_WAPI_SUPPORT
        case WLAN_CIPHER_SUITE_SMS4:
                sta_printk( "[STA] WAPI\n");
                encrypthdr = WAPI_ENCRYPT_HDR_SIZE;
                encrypttailsize = WAPI_ENCRYPT_TAIL_SIZE;
                encrypt = 1;
                break;
	#endif
        default:
                encrypthdr = 0;
                encrypttailsize = 0;
                encrypt = 0;
                break;
        }

        framehdrlen += encrypthdr;

        /* Filling the 802.11 Hdr */
        dot11hdr->frame_control = cpu_to_le16(IEEE80211_FTYPE_DATA);
        if (priv->vif->type == NL80211_IFTYPE_STATION)
                dot11hdr->frame_control |= cpu_to_le16(IEEE80211_FCTL_TODS);
        else
                dot11hdr->frame_control |= cpu_to_le16(IEEE80211_FCTL_FROMDS);

        if (encrypt)
                dot11hdr->frame_control |= cpu_to_le16(IEEE80211_FCTL_WEP);

        if (priv->vif->bss_conf.qos) {
                sta_printk( "[STA] QOS Enabled\n");
                dot11hdr->frame_control |= cpu_to_le16(IEEE80211_QOS_DATAGRP);
                 *(u16 *)(dot11hdr + 1) = 0x0;
                 framehdrlen += 2;
        } else {
                dot11hdr->frame_control |= cpu_to_le16(IEEE80211_STYPE_DATA);
        }

        memcpy(dot11hdr->addr1, priv->vif->bss_conf.bssid, ETH_ALEN);
        memcpy(dot11hdr->addr2, priv->vif->addr, ETH_ALEN);
        memcpy(dot11hdr->addr3, priv->vif->bss_conf.bssid, ETH_ALEN);

        /* Filling the LLC/SNAP Hdr */
        snaphdr = (struct ieee80211_snap_hdr *)((u8 *)dot11hdr + framehdrlen);
        memcpy(snaphdr, (struct ieee80211_snap_hdr *)rfc1042_header, \
                sizeof(*snaphdr));
        *(u16 *)(++snaphdr) = cpu_to_be16(ETH_P_ARP);
        /* Updating the framebdylen with snaphdr and LLC hdr size */
        framebdylen = sizeof(*snaphdr) + 2;

        /* Filling the ARP Reply Payload */
        arp_hdr = (struct arphdr *)((u8 *)dot11hdr + framehdrlen + framebdylen);
        arp_hdr->ar_hrd = cpu_to_be16(ARPHRD_ETHER);
        arp_hdr->ar_pro = cpu_to_be16(ETH_P_IP);
        arp_hdr->ar_hln = ETH_ALEN;
        arp_hdr->ar_pln = 4;
        arp_hdr->ar_op = cpu_to_be16(ARPOP_REPLY);

        /* Updating the frmbdylen with Arp Reply Hdr and Arp payload size(20) */
        framebdylen += sizeof(*arp_hdr) + 20;

        /* Updating the framebdylen with Encryption Tail Size */
        framebdylen += encrypttailsize;

        /* Filling the Template Frame Hdr */
        template_frame[0] = WSM_FRAME_TYPE_ARP_REPLY; /* Template frame type */
        template_frame[1] = 0xFF; /* Rate to be fixed */
        ((u16 *)&template_frame[2])[0] = framehdrlen + framebdylen;

        ret = WARN_ON(wsm_write_mib(hw_priv, WSM_MIB_ID_TEMPLATE_FRAME, \
                                template_frame, (framehdrlen+framebdylen+4), priv->if_id));
        atbm_kfree(template_frame);
exit_p:
        return ret;
}
#endif
#ifdef CONFIG_ATBM_SUPPORT_SCHED_SCAN
#ifdef ROAM_OFFLOAD
/**
 * atbm_testmode_event -send asynchronous event
 * to userspace
 *
 * @wiphy: the wiphy
 * @msg_id: STE msg ID
 * @data: data to be sent
 * @len: data length
 * @gfp: allocation flag
 *
 * Returns: 0 on success or non zero value on failure
 */
int atbm_testmode_event(struct wiphy *wiphy, const u32 msg_id,
			 const void *data, int len, gfp_t gfp)
{
	struct sk_buff *skb = cfg80211_testmode_alloc_event_skb(wiphy,
		nla_total_size(len+sizeof(msg_id)), gfp);

	if (!skb)
		return -ENOMEM;

	cfg80211_testmode_event(skb, gfp);
	return 0;
}
#endif /*ROAM_OFFLOAD*/
#endif
#ifdef CONFIG_ATBM_LMAC_FILTER_IP_FRAME
#ifdef IPV6_FILTERING
/**
 * atbm_set_na -called for creating and
 * configuring NDP Neighbor Advertisement (NA) template frame
 *
 * @hw: the hardware
 * @vif: vif
 *
 * Returns: 0 on success or non zero value on failure
 */
int atbm_set_na(struct ieee80211_hw *hw, struct ieee80211_vif *vif)
{
	struct atbm_vif *priv = ABwifi_get_vif_from_ieee80211(vif);
	struct atbm_common *hw_priv = (struct atbm_common *)hw->priv;
	u32 framehdrlen, encrypthdr, encrypttailsize, framebdylen = 0;
	bool encrypt = false;
	int ret = 0;
	u8 *template_frame = NULL;
	struct ieee80211_hdr_3addr *dot11hdr = NULL;
	struct ieee80211_snap_hdr *snaphdr = NULL;
	struct ipv6hdr *ipv6_hdr = NULL;
	struct icmp6hdr *icmp6_hdr = NULL;
	struct nd_msg *na = NULL;
	struct nd_opt_hdr *opt_hdr = NULL;

	template_frame = atbm_kzalloc(MAX_NEIGHBOR_ADVERTISEMENT_TEMPLATE_SIZE, GFP_ATOMIC);
	if (!template_frame) {
		sta_printk( "[STA] Template frame memory failed\n");
		ret = -ENOMEM;
		goto exit_p;
	}
	dot11hdr = (struct ieee80211_hdr_3addr *)&template_frame[4];

	framehdrlen = sizeof(*dot11hdr);
        if ((priv->vif->type == NL80211_IFTYPE_AP) && priv->vif->p2p)
		priv->cipherType = WLAN_CIPHER_SUITE_CCMP;
	switch (priv->cipherType) {

	case WLAN_CIPHER_SUITE_WEP40:
	case WLAN_CIPHER_SUITE_WEP104:
		sta_printk( "[STA] WEP\n");
		encrypthdr = WEP_ENCRYPT_HDR_SIZE;
		encrypttailsize = WEP_ENCRYPT_TAIL_SIZE;
		encrypt = 1;
		break;


	case WLAN_CIPHER_SUITE_TKIP:
		sta_printk( "[STA] WPA\n");
		encrypthdr = WPA_ENCRYPT_HDR_SIZE;
		encrypttailsize = WPA_ENCRYPT_TAIL_SIZE;
		encrypt = 1;
		break;

	case WLAN_CIPHER_SUITE_CCMP:
		sta_printk( "[STA] WPA2\n");
		encrypthdr = WPA2_ENCRYPT_HDR_SIZE;
		encrypttailsize = WPA2_ENCRYPT_TAIL_SIZE;
		encrypt = 1;
		break;
#ifdef CONFIG_WAPI_SUPPORT
	case WLAN_CIPHER_SUITE_SMS4:
		sta_printk( "[STA] WAPI\n");
		encrypthdr = WAPI_ENCRYPT_HDR_SIZE;
		encrypttailsize = WAPI_ENCRYPT_TAIL_SIZE;
		encrypt = 1;
		break;
#endif
	default:
		encrypthdr = 0;
		encrypttailsize = 0;
		encrypt = 0;
		break;
	}

	framehdrlen += encrypthdr;

	/* Filling the 802.11 Hdr */
	dot11hdr->frame_control = cpu_to_le16(IEEE80211_FTYPE_DATA);
	if (priv->vif->type == NL80211_IFTYPE_STATION)
		dot11hdr->frame_control |= cpu_to_le16(IEEE80211_FCTL_TODS);
	else
		dot11hdr->frame_control |= cpu_to_le16(IEEE80211_FCTL_FROMDS);

	if (encrypt)
		dot11hdr->frame_control |= cpu_to_le16(IEEE80211_FCTL_WEP);

	if (priv->vif->bss_conf.qos) {
		sta_printk( "[STA] QOS Enabled\n");
		dot11hdr->frame_control |= cpu_to_le16(IEEE80211_QOS_DATAGRP);
		/* Filling QOS Control Field */
		 *(u16 *)(dot11hdr + 1) = 0x0;
		 framehdrlen += 2;
	} else {
		dot11hdr->frame_control |= cpu_to_le16(IEEE80211_STYPE_DATA);
	}

	memcpy(dot11hdr->addr1, priv->vif->bss_conf.bssid, ETH_ALEN);
	memcpy(dot11hdr->addr2, priv->vif->addr, ETH_ALEN);
	memcpy(dot11hdr->addr3, priv->vif->bss_conf.bssid, ETH_ALEN);

	/* Filling the LLC/SNAP Hdr */
	snaphdr = (struct ieee80211_snap_hdr *)((u8 *)dot11hdr + framehdrlen);
	memcpy(snaphdr, (struct ieee80211_snap_hdr *)rfc1042_header, \
		sizeof(*snaphdr));
	*(u16 *)(++snaphdr) = cpu_to_be16(ETH_P_IPV6);
	/* Updating the framebdylen with snaphdr and LLC hdr size */
	framebdylen = sizeof(*snaphdr) + 2;

	/* Filling the ipv6 header */
	ipv6_hdr = (struct ipv6hdr *)((u8 *)dot11hdr + framehdrlen + framebdylen);
	ipv6_hdr->version = 6;
	ipv6_hdr->priority = 0;
	ipv6_hdr->payload_len = cpu_to_be16(32); /* ??? check the be or le ??? whether to use cpu_to_be16(32)*/
	ipv6_hdr->nexthdr = 58;
	ipv6_hdr->hop_limit = 255;

	/* Updating the framebdylen with ipv6 Hdr */
	framebdylen += sizeof(*ipv6_hdr);

	/* Filling the Neighbor Advertisement */
	na = (struct nd_msg *)((u8 *)dot11hdr + framehdrlen + framebdylen);
	icmp6_hdr = (struct icmp6hdr *)(&na->icmph);
	icmp6_hdr->icmp6_type = NDISC_NEIGHBOUR_ADVERTISEMENT;
	icmp6_hdr->icmp6_code = 0;
	/* checksum (2 bytes), RSO fields (4 bytes) and target IP address (16 bytes) shall be filled by firmware */

	/* Filling the target link layer address in the optional field */
	opt_hdr = (struct nd_opt_hdr *)(&na->opt[0]);
	opt_hdr->nd_opt_type = 2;
	opt_hdr->nd_opt_len = 1;
	/* optional target link layer address (6 bytes) shall be filled by firmware */

	/* Updating the framebdylen with the ipv6 payload length */
	framebdylen += 32;

	/* Updating the framebdylen with Encryption Tail Size */
	framebdylen += encrypttailsize;

	/* Filling the Template Frame Hdr */
	template_frame[0] = WSM_FRAME_TYPE_NA; /* Template frame type */
	template_frame[1] = 0xFF; /* Rate to be fixed */
	((u16 *)&template_frame[2])[0] = framehdrlen + framebdylen;

	ret = WARN_ON(wsm_write_mib(hw_priv, WSM_MIB_ID_TEMPLATE_FRAME, \
				template_frame, (framehdrlen+framebdylen+4), \
				priv->if_id));

	atbm_kfree(template_frame);

exit_p:
	return ret;
}
#endif /*IPV6_FILTERING*/
#endif
int atbm_tool_shortGi = 0;
int atbm_tool_use_short_slot = 0;
int atbm_tool_use_short_preamble = 0;
int atbm_tool_use_cts_prot = 0;

#if defined(CONFIG_NL80211_TESTMODE) && defined(CONFIG_ATBM_TEST_TOOL)


/**
 * atbm_tesmode_reply -called inside a testmode command
 * handler to send a response to user space
 *
 * @wiphy: the wiphy
 * @data: data to be send to user space
 * @len: data length
 *
 * Returns: 0 on success or non zero value on failure
 */
int atbm_tesmode_reply(struct wiphy *wiphy,
				const void *data, int len)
{	
	struct ieee80211_local *local = wiphy_priv(wiphy);
	int ret = 0;

	if(local->hw.vendcmd_nl80211 == 0)
	{ 
#if defined(CONFIG_NL80211_TESTMODE)		
		struct sk_buff *skb = cfg80211_testmode_alloc_reply_skb(wiphy,
			nla_total_size(len));

		if (!skb){
			return -ENOMEM;
		}
		ret = nla_put(skb, ATBM_TM_MSG_DATA, len, data);
		if (ret) {
			atbm_kfree_skb(skb);
			return ret;
		}

		return cfg80211_testmode_reply(skb);
#endif
	}
	else
	{
		if(len >0)
		{ 
			local->hw.vendreturn.len = len;
			memcpy((u8*)local->hw.vendreturn.respbuff,(u8*)data,len);
		}
		ret = len;
	}

	return ret;
}

/**
 * atbm_start_stop_tsm - starts/stops collecting TSM
 *
 * @hw: the hardware
 * @data: data frame
 *
 * Returns: 0 on success or non zero value on failure
 */
int atbm_start_stop_tsm(struct ieee80211_hw *hw, void *data)
{
#ifdef CONFIG_ATBM_APOLLO_TESTMODE

	struct atbm_msg_start_stop_tsm *start_stop_tsm =
		(struct atbm_msg_start_stop_tsm *) data;
	struct atbm_common *hw_priv = hw->priv;
	hw_priv->start_stop_tsm.start = start_stop_tsm->start;
	hw_priv->start_stop_tsm.up = start_stop_tsm->up;
	//hw_priv->start_stop_tsm.packetization_delay =
	//	start_stop_tsm->packetization_delay;
	sta_printk("[STA] %s: start : %u: up : %u",
		__func__, hw_priv->start_stop_tsm.start,
		hw_priv->start_stop_tsm.up);
	hw_priv->tsm_info.ac = atbm_1d_to_ac[start_stop_tsm->up];

	if (!hw_priv->start_stop_tsm.start) {
		spin_lock_bh(&hw_priv->tsm_lock);
		memset(&hw_priv->tsm_stats, 0, sizeof(hw_priv->tsm_stats));
		memset(&hw_priv->tsm_info, 0, sizeof(hw_priv->tsm_info));
		spin_unlock_bh(&hw_priv->tsm_lock);
	}
#endif
	return 0;
}

/**
 * atbm_get_tsm_params - Retrieves TSM parameters
 *
 * @hw: the hardware
 *
 * Returns: TSM parameters collected
 */
int atbm_get_tsm_params(struct ieee80211_hw *hw)
{
#ifdef CONFIG_ATBM_APOLLO_TESTMODE

	struct atbm_common *hw_priv = hw->priv;
	struct atbm_tsm_stats tsm_stats;
	u32 pkt_count;
	spin_lock_bh(&hw_priv->tsm_lock);
	pkt_count = hw_priv->tsm_stats.txed_msdu_count;
	sta_printk( "[STA] : pkt_count : %u discarded:%u",
				hw_priv->tsm_stats.txed_msdu_count ,
			 hw_priv->tsm_stats.msdu_discarded_count);
	if (pkt_count) {
		hw_priv->tsm_stats.avg_q_delay =
			hw_priv->tsm_info.sum_pkt_q_delay/pkt_count;
		hw_priv->tsm_stats.avg_transmit_delay =
			hw_priv->tsm_info.sum_media_delay/pkt_count;
	} else {
		hw_priv->tsm_stats.avg_q_delay = 0;
		hw_priv->tsm_stats.avg_transmit_delay = 0;
	}
	sta_printk( "[STA] : Txed MSDU count : %u\n",
		 hw_priv->tsm_stats.txed_msdu_count);
	sta_printk( "[STA] : Average queue delay : %u\n",
			 hw_priv->tsm_stats.avg_q_delay);
	sta_printk( "[STA] : Average transmit delay : %u\n",
			 hw_priv->tsm_stats.avg_transmit_delay);
	sta_printk( "\n [STA] : "
		"	Txed MSDU count 0~500us: %u,\n"
		"	Txed MSDU count 500~1ms: %u,\n"
		"	Txed MSDU count 1~2ms: %u,\n"
		"	Txed MSDU count 2~4ms: %u,\n"
		"	Txed MSDU count 4~6ms: %u,\n"
		"	Txed MSDU count 6~8ms: %u,\n"
		"	Txed MSDU count 8~10ms: %u,\n"
		"   Txed MSDU count 10~20ms: %u,\n, "
		"   Txed MSDU count 20~40ms: %u,\n, "
		"   Txed MSDU count 40~xxms: %u,\n,",
		hw_priv->tsm_stats.bin00,
		hw_priv->tsm_stats.bin01,
		hw_priv->tsm_stats.bin02,
		hw_priv->tsm_stats.bin03,
		hw_priv->tsm_stats.bin04,
		hw_priv->tsm_stats.bin05,
		hw_priv->tsm_stats.bin0,
		hw_priv->tsm_stats.bin1,
		hw_priv->tsm_stats.bin2,
		hw_priv->tsm_stats.bin3);
	memcpy(&tsm_stats, &hw_priv->tsm_stats, sizeof(hw_priv->tsm_stats));
	/* Reset the TSM statistics */
	memset(&hw_priv->tsm_stats, 0, sizeof(hw_priv->tsm_stats));
	hw_priv->tsm_info.sum_pkt_q_delay = 0;
	hw_priv->tsm_info.sum_media_delay = 0;
	spin_unlock_bh(&hw_priv->tsm_lock);
	return atbm_tesmode_reply(hw->wiphy, &tsm_stats,
				     sizeof(hw_priv->tsm_stats));
#else

	return -1;
#endif 
}
/**
 * atbm_get_roam_delay - Retrieves roam delay
 *
 * @hw: the hardware
 *
 * Returns: Returns the last measured roam delay
 */
int atbm_get_roam_delay(struct ieee80211_hw *hw)
{
#ifdef CONFIG_ATBM_APOLLO_TESTMODE

	struct atbm_common *hw_priv = hw->priv;
	u16 roam_delay = hw_priv->tsm_info.roam_delay / 1000;
	sta_printk( "[STA] %s: Roam delay : %u",
		__func__, roam_delay);
	spin_lock_bh(&hw_priv->tsm_lock);
	hw_priv->tsm_info.roam_delay = 0;
	hw_priv->tsm_info.use_rx_roaming = 0;
	spin_unlock_bh(&hw_priv->tsm_lock);
	return atbm_tesmode_reply(hw->wiphy, &roam_delay, sizeof(u16));
#else
	return -1;
#endif
}


static int atbm_set_tx_power_level(struct ieee80211_hw *hw, int power)
{
	struct atbm_common *hw_priv = hw->priv;
	struct atbm_vif * vif = NULL;
	int i = 0;

	hw_priv->output_power = power;
	atbm_for_each_vif(hw_priv,vif,i){
		if (vif != NULL)
		{
		wsm_set_output_power(hw_priv,
						     hw_priv->output_power * 10,
						     vif->if_id);
		}
	}
	sta_printk( "[STA] %s: Power set on Device : %d",
		__func__, power);
	return 0;
}

static int atbm_set_wme_uapsd(struct ieee80211_hw *hw, int apsd)
{
	int if_id = 0;
	struct atbm_common *hw_priv = (struct atbm_common *) hw->priv;
	struct atbm_vif *priv;
	int ret = 0, i;
	struct wsm_edca_params arg;

	priv = ABwifi_hwpriv_to_vifpriv(hw_priv, if_id);
	if (unlikely(!priv)) {
		sta_printk(KERN_ERR "[STA] %s: Warning Priv is Null\n",
			   __func__);
		return 0;
	}
	atbm_priv_vif_list_read_unlock(&priv->vif_lock);
	mutex_lock(&hw_priv->conf_mutex);

	for (i = 0; i < 4; i++)
	{
		arg.params[i].uapsdEnable = !!apsd;
	}
	ret = atbm_set_uapsd_param(priv, &arg);
	ret = ret;
	mutex_unlock(&hw_priv->conf_mutex);
	return 0;
}
static int atbm_set_power_save_mode(struct ieee80211_hw *hw,
				 u8 *data, int len)
{
	int ps_elems = 0;
	struct atbm_common *hw_priv = (struct atbm_common *) hw->priv;
	struct atbm_vif *priv;
	int if_id = 0;
	/* Interface ID is hard coded here, as interface is not
         * passed in testmode command.
         * Also it is assumed here that STA will be on interface
         * 0 always.
         */


	priv = ABwifi_hwpriv_to_vifpriv(hw_priv, if_id);

	if (unlikely(!priv)) {
		sta_printk(KERN_ERR "[STA] %s: Warning Priv is Null\n",
			   __func__);
		return 0;
	}

	atbm_priv_vif_list_read_unlock(&priv->vif_lock);
	mutex_lock(&hw_priv->conf_mutex);

	ps_elems = *(int*)data;

	if (ps_elems == 0)
		priv->user_pm_mode = WSM_PSM_ACTIVE;
	else if (ps_elems == 1)
		priv->user_pm_mode = WSM_PSM_PS;
	else if (ps_elems == 2)
		priv->user_pm_mode = WSM_PSM_FAST_PS;
	else
		return 0;

	sta_printk( "[STA] Aid: %d, Joined: %s, Powersave: %s\n",
		priv->bss_params.aid,
		priv->join_status == ATBM_APOLLO_JOIN_STATUS_STA ? "yes" : "no",
		priv->user_pm_mode == WSM_PSM_ACTIVE ? "WSM_PSM_ACTIVE" :
		priv->user_pm_mode == WSM_PSM_PS ? "WSM_PSM_PS" :
		priv->user_pm_mode == WSM_PSM_FAST_PS ? "WSM_PSM_FAST_PS" :
		"UNKNOWN");
	if (priv->join_status == ATBM_APOLLO_JOIN_STATUS_STA &&
			priv->bss_params.aid ) {
		priv->powersave_mode.pmMode = priv->user_pm_mode;
		sta_printk("atbm_set_pm %d\n",priv->powersave_mode.pmMode);
		atbm_set_pm(priv, &priv->powersave_mode);
	}
	else
		priv->user_power_set_true = ps_elems;
	mutex_unlock(&hw_priv->conf_mutex);
	return 0;
}





#if 0
extern int wsm_test_cmd(struct atbm_common *hw_priv,u8 *buffer,int size);
#endif
#ifdef USB_BUS
extern 	int atbm_device_txrx_test_init(struct atbm_common *hw_priv,int caseNum );
int atbm_device_wakeup_test(struct atbm_common *hw_priv, int enable)
{
	//atbm_device_txrx_test_init(hw_priv,enable);
	return 0;
}
#else
int atbm_device_wakeup_test(struct atbm_common *hw_priv, int enable)
{
	return 0;
}
#endif

enum altm_atbm_msg{
	ALTM_SET_BLOCK_ACK_RX = 7,
	ALTM_GET_BLOCK_ACK_RX = 8,
	ALTM_SET_BLOCK_ACK_TX = 9,
	ALTM_GET_BLOCK_ACK_TX = 10,
	ALTM_GET_TSM_STATS = 11,
	ALTM_CLEAR_TSM_STATS = 12,
	ALTM_GET_PREAMBLE = 17,
	ALTM_GET_CTSPROT = 18,

	ALTM_GET_STATUS_COMMON = 19,
	ALTM_GET_STATUS_COUNTERS = 20,
	ALTM_GET_STATUS_PRIV = 21,
	ALTM_GET_RSSI = 22,
	ALTM_SET_PS_MODE = 23,
	ALTM_SET_TX_POWER = 24,
	ALTM_SET_WME_APSD = 25,

	ALTM_GET_REVINFO = 28,
	ALTM_GET_SHMEM = 29,
	ALTM_SET_SHMEM = 30,
	ALTM_GET_PACKETCNT = 37,
	ALTM_SET_DBG_PRINT_TO_HOST = 38,
	ALTM_SET_ZERO_COUNTERS_TABLE = 39,
	ALTM_GET_DBG_PRINT_TO_HOST = 40,
	ALTM_SET_FWCMD = 42,
	ALTM_SET_WAKEUP = 43,

	ALTM_SET_SHORT_GI = 44,
	ALTM_SET_40M = 45,
	ALTM_GET_EDCA_PARAM = 46,
	ALTM_SET_TXQUEUE_PARAM = 47,

	ALTM_GET_RTSTHRESHOLD = 13,
	ALTM_SET_RTSTHRESHOLD = 14,
	ALTM_SET_CTSPROT = 15,
	ALTM_SET_PREAMBLE = 16,

    ALTM_GET_SHORTSLOT = 27,
	ALTM_SET_SHORTSLOT = 26,

	ALTM_SET_SRL = 48,
	ALTM_SET_LRL = 49,
	ALTM_SHOW_EVENT_CMD = 50,
	ALTM_CLEAR_EVENT_CMD = 51,
	ALTM_SET_CCA = 100,
	ATBM_MSG_GET_TSM_PARAMS = 102,
	ATBM_MSG_START_STOP_TSM = 103,
	ATBM_MSG_GET_ROAM_DELAY = 104,	
	ATBM_START_SMARTCONFIG = 107,
	ATBM_SET_START_TX = 108,
	ATBM_SET_STOP_TX = 109,
	ATBM_SET_START_RX = 110,
	ATBM_SET_STOP_RX = 111,
	ATBM_WRITE_REG = 112,
	ATBM_READ_REG = 113,
};

struct altbeam_tsm_stats {
	//u64 actual_msrmt_start_time;
	u16 msrmt_duration;
	//u8 peer_sta_addr[6];
	u8 tid;
	u8 reporting_reason;
	u32 txed_msdu_count;
	u32 msdu_discarded_count;
	u32 msdu_failed_count;
	u32 multi_retry_count;
	u32 qos_cfpolls_lost_count;
	u32 avg_q_delay;
	u32 avg_transmit_delay;
};
struct altm_msg_shmem{
	u8 type;
	u32 address;
	u32 buff;
	u32 size;
};

struct altbm_tx_queue_params {
	u16 txop;
	u16 cw_min;
	u16 cw_max;
	u8 aifs;
	u8 queue;
};

extern void atbm_set_shortGI(u32 shortgi);
#ifdef CONFIG_ATBM_40M_AUTO_CCA
extern void atbm_set_40M(struct atbm_common *hw_priv,u32 is40M);
extern void atbm_set_cca(struct atbm_common *hw_priv,u8 *buff);
#endif
#define BUFFER_SIZE 1024
static char buff[BUFFER_SIZE];

extern int wsm_start_tx(struct atbm_common *hw_priv, struct ieee80211_vif *vif);
int wsm_stop_tx(struct atbm_common *hw_priv);

int atbm_altmtest_cmd(struct ieee80211_hw *hw, void *data, int len)
{
	struct altm_msg{
		u8 type;
		u32 value;
	};
	struct altm_msg_txrx{
		u32 type;
		u32 value;
		u32 externData[20];
	};
	int ret = 0;
	struct altbeam_tsm_stats altbeam_tsm_stat[4];
	int i = 0;
	struct atbm_common *hw_priv = hw->priv;
	struct atbm_vif * vif = NULL;
	struct atbm_debug_param seq;
	u8 ucDbgPrintOpenFlag = 0;
	u8 ucZeroCounterTable = 0;
	char *pFwCmd = NULL;
	char *str = NULL;
	int queue;
	int length;
	//int registerValue;

	struct altm_msg *msg =0;
	struct altm_msg_txrx *msg_txrx =0;
	u32 value = 0;
	struct altm_msg_shmem *altm_shmem;
	char * const fw_types[] = {
				"ETF",
				"WFM",
				"WSM",
				"HI test",
				"Platform test"
			};
	if(hw->vendcmd_nl80211 == 0)
	{
		struct nlattr *data_p = nla_find(data, len, ATBM_TM_MSG_DATA);
		if (!data_p)
			return ret;
		msg = (struct altm_msg *)nla_data(data_p);
	}
	else
		msg = (struct altm_msg *)data;
	sta_printk(KERN_ERR "[STA] %s: type: %i\n", __func__, msg->type);


	switch (msg->type)
	{
#ifdef CONFIG_ATBM_40M_AUTO_CCA
		case ALTM_SET_CCA:
		{ 
			atbm_set_cca(hw_priv,(u8*)(&msg->value));
			break;
		}
#endif
#ifndef CONFIG_RATE_HW_CONTROL
		case ALTM_SET_SRL:
			sta_printk(KERN_ERR "Retry limits: %d (short).\n", msg->value);
			spin_lock_bh(&hw_priv->tx_policy_cache.lock);
			hw_priv->short_frame_max_tx_count =
				( msg->value < 0x0F) ?
				 msg->value : 0x0F;
			hw_priv->hw->max_rate_tries = hw_priv->short_frame_max_tx_count;
			spin_unlock_bh(&hw_priv->tx_policy_cache.lock);
			break;
		case ALTM_SET_LRL:
			sta_printk(KERN_ERR "Retry limits: %d (long).\n", msg->value);
			spin_lock_bh(&hw_priv->tx_policy_cache.lock);
			hw_priv->long_frame_max_tx_count =  msg->value;
			spin_unlock_bh(&hw_priv->tx_policy_cache.lock);
			break;
#endif
		case ALTM_GET_SHORTSLOT:
			sta_printk(KERN_ERR "use_short_slot is %d\n", atbm_tool_use_short_slot);
			break;
		case ALTM_SET_SHORTSLOT:
			atbm_for_each_vif(hw_priv,vif,i){
				if(vif != NULL)
				{
					__le32 slot_time = (!!msg->value) ?
							__cpu_to_le32(9) : __cpu_to_le32(20);
					//add by wp fix PHICOMM AP4  long slot  bug
					if(vif->htcap){
						 slot_time = 	__cpu_to_le32(9);
					}
					sta_printk(KERN_ERR "[STA] Slot time :%d us.\n",__le32_to_cpu(slot_time));
					WARN_ON(wsm_write_mib(hw_priv, WSM_MIB_ID_DOT11_SLOT_TIME,
						&slot_time, sizeof(slot_time), vif->if_id));
					atbm_tool_use_short_slot = (slot_time == __cpu_to_le32(9))? 1:0;
					break;
				}
			}
			break;

		case ALTM_GET_PREAMBLE:
			sta_printk(KERN_ERR "use_short_preamble is %d\n", atbm_tool_use_short_preamble);
			break;
		case ALTM_SET_PREAMBLE:
			sta_printk(KERN_ERR "set_preamble is %d\n", msg->value);
			atbm_for_each_vif(hw_priv,vif,i){
				if(vif != NULL)
				{
					vif->association_mode.preambleType = msg->value ? WSM_JOIN_PREAMBLE_SHORT :
															WSM_JOIN_PREAMBLE_LONG;
					wsm_set_association_mode(hw_priv,
						&vif->association_mode, vif->if_id);
					atbm_tool_use_short_preamble = vif->association_mode.preambleType;
					break;
				}
			}
			break;

		case ALTM_GET_CTSPROT:
			sta_printk(KERN_ERR "use_cts_prot is %d\n", atbm_tool_use_cts_prot);
			break;
		case ALTM_SET_CTSPROT:
			{
				__le32 use_cts_prot = 0;
				value = msg->value? 1:0;
				use_cts_prot = __cpu_to_le32(value);
				sta_printk(KERN_ERR "set_cts_prot is %d\n", use_cts_prot);
				atbm_for_each_vif(hw_priv,vif,i){
					if(vif != NULL)
					{
						wsm_write_mib(hw_priv, WSM_MIB_ID_NON_ERP_PROTECTION,
							&use_cts_prot, sizeof(use_cts_prot),vif->if_id);
						atbm_tool_use_cts_prot = value;
						break;
					}
				}
			}
			break;
		case ALTM_SET_RTSTHRESHOLD:
			sta_printk(KERN_ERR "set_rtsthr is %d\n", msg->value);
			atbm_for_each_vif(hw_priv,vif,i){
				if(vif != NULL)
				{
					__le32 val32;
					value = msg->value;
					if (value != (u32) -1)
						val32 = __cpu_to_le32(value);
					else
						val32 = 0; /* disabled */

					/* mutex_lock(&priv->conf_mutex); */
					ret = (wsm_write_mib(hw_priv, WSM_MIB_ID_DOT11_RTS_THRESHOLD,
						&val32, sizeof(val32), vif->if_id));
					atbm_tool_rts_threshold = val32;
					break;
				}
			}
			break;
		case ALTM_GET_RTSTHRESHOLD:
			sta_printk(KERN_ERR "get_rts_threshold = %d\n", atbm_tool_rts_threshold);
			break;
		case ALTM_SET_TXQUEUE_PARAM:
			{
				struct altbm_tx_queue_params *altbm_param;
				struct ieee80211_tx_queue_params params;
				altbm_param = (struct altbm_tx_queue_params *)&msg->value;
				params.aifs = altbm_param->aifs;
				params.txop = altbm_param->txop;
				params.cw_min  = altbm_param->cw_min;
				params.cw_max = altbm_param->cw_max;
				params.uapsd = false;
				sta_printk(KERN_ERR "set_tx_queue:aifs(%d),txop(%d),cw_min(%d),cw_max(%d)\n", 
					params.aifs,params.txop,params.cw_min,params.cw_max);
				atbm_for_each_vif(hw_priv,vif,i){
					if((vif != NULL) && (vif->vif != NULL)){
							atbm_conf_tx(hw, vif->vif, altbm_param->queue, &params);
							break;
					}
				}
			}
			break;
#ifdef CONFIG_ATBM_40M_AUTO_CCA
		case ALTM_SET_40M:
			atbm_set_40M(hw_priv,msg->value);
			break;
#endif
		case ALTM_GET_EDCA_PARAM:
			atbm_for_each_vif(hw_priv,vif,i){
				if((vif != NULL)){
					for (queue = 0;queue < 4; queue++)
						sta_printk(KERN_ERR "vif[%d],queue[%d]: txOpLimit=%d,aifns=%d,cwMin=%d,cwMax=%d;\n", i, queue,vif->edca.params[queue].txOpLimit, vif->edca.params[queue].aifns, vif->edca.params[queue].cwMin, vif->edca.params[queue].cwMax);
				}
			}
			break;
		case ALTM_SET_WAKEUP:
		/*
			if(	 msg->value == 1){		
				hw_priv->etf_channel = 1;
				hw_priv->etf_channel_type = NL80211_CHAN_HT20;
				hw_priv->etf_rate = WSM_TRANSMIT_RATE_6;
				hw_priv->etf_len = 200;	

				atbm_for_each_vif(hw_priv,vif,i) {
					if((vif != NULL)){
						
						wsm_start_tx(hw_priv, vif->vif);
						break;
					}
				}
		
			}
			else */if(	 msg->value == 1){		
				hw_priv->etf_channel = 6;
				hw_priv->etf_channel_type = NL80211_CHAN_HT40PLUS;
				hw_priv->etf_rate = WSM_TRANSMIT_RATE_HT_65;
				hw_priv->etf_len = 1000;	

				atbm_for_each_vif(hw_priv,vif,i) {
					if((vif != NULL)){
						
						wsm_start_tx(hw_priv, vif->vif);
						break;
					}
				}
		
			}
			else {
				wsm_stop_tx(hw_priv);
			}
			//ret = atbm_device_wakeup_test(hw_priv, msg->value);
			sta_printk(KERN_ERR "device wakeup set:%d, ret:%d\n", msg->value, ret);
			break;
		case ALTM_GET_REVINFO:
			ret = sprintf(buff,
			"   Input buffers: %d x %d bytes\n"
			"   Hardware: %d.%d\n"
			"   %s firmware, ver: %d, build: %d,"
			    " api: %d, cap: 0x%.4X\n",
			hw_priv->wsm_caps.numInpChBufs,
			hw_priv->wsm_caps.sizeInpChBuf,
			hw_priv->wsm_caps.hardwareId,
			hw_priv->wsm_caps.hardwareSubId,
			fw_types[hw_priv->wsm_caps.firmwareType],
			hw_priv->wsm_caps.firmwareVersion,
			hw_priv->wsm_caps.firmwareBuildNumber,
			hw_priv->wsm_caps.firmwareApiVer,
			hw_priv->wsm_caps.firmwareCap);
			if (ret > 0)
			{
				ret = atbm_tesmode_reply(hw->wiphy, (void *)buff, ret);
			}
			break;
		case ALTM_GET_SHMEM:
			altm_shmem = (struct altm_msg_shmem *)msg;
			if (altm_shmem->size <= 4)
			{
				sta_printk(KERN_ERR "size:0x%x, buff:%#x\n", altm_shmem->size, altm_shmem->buff);
				ret = wsm_read_shmem(hw_priv, altm_shmem->address, &altm_shmem->buff, altm_shmem->size);
				//atbm_direct_read_reg_32(hw_priv,altm_shmem->address,&altm_shmem->buff);
				if (ret == 0)
				{
					sta_printk(KERN_ERR "size:0x%x, buff:%x\n", altm_shmem->size, altm_shmem->buff);
					ret = atbm_tesmode_reply(hw->wiphy, &altm_shmem->buff, altm_shmem->size);
				}
			}else
			{
				sta_printk(KERN_ERR "proccess error, size range 1~4\n");
			}
			break;
		case ALTM_SET_SHMEM:
			altm_shmem = (struct altm_msg_shmem *)msg;
			if (altm_shmem->size <= 4)
			{
				sta_printk(KERN_ERR "size:0x%x, buff:%#x\n", altm_shmem->size, altm_shmem->buff);
				//atbm_direct_write_reg_32(hw_priv, altm_shmem->address,altm_shmem->buff);
				ret = wsm_write_shmem(hw_priv, altm_shmem->address, altm_shmem->size, &altm_shmem->buff);
			}else
			{
				sta_printk(KERN_ERR "proccess error, size range 1~4\n");
			}
			break;
		case ALTM_SET_WME_APSD:
			sta_printk(KERN_ERR "set wme_apsd(%d)\n",msg->value);
			ret = atbm_set_wme_uapsd(hw, msg->value);
			break;
		case ALTM_SET_BLOCK_ACK_RX:
			sta_printk(KERN_ERR "block_ack_rx(%x)\n",msg->value);
			hw_priv->ba_tid_rx_mask = (u8)msg->value;
			break;
		case ALTM_GET_BLOCK_ACK_RX:
			value = (u32)hw_priv->ba_tid_rx_mask;
			ret = atbm_tesmode_reply(hw->wiphy, &value, sizeof(value));
			break;
		case ALTM_SET_BLOCK_ACK_TX:
			sta_printk(KERN_ERR "block_ack_tx(%x)\n",msg->value);
			hw_priv->ba_tid_tx_mask = (u8)msg->value;
			break;
		case ALTM_GET_BLOCK_ACK_TX:
			value = (u32)hw_priv->ba_tid_tx_mask;
			ret = atbm_tesmode_reply(hw->wiphy, &value, sizeof(value));
			break;
		case ALTM_CLEAR_TSM_STATS:
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
			spin_lock_bh(&hw_priv->tsm_lock);
			memset(&hw_priv->tsm_stats, 0, sizeof(struct atbm_tsm_stats));
			memset(hw_priv->atbm_tsm_stats, 0, 4 * sizeof(struct atbm_tsm_stats));
			memset(&hw_priv->tsm_info, 0, sizeof(struct atbm_tsm_info));
			spin_unlock_bh(&hw_priv->tsm_lock);
#endif
			break;
		case ALTM_GET_TSM_STATS:
			memset(&altbeam_tsm_stat[0], 0, sizeof(altbeam_tsm_stat));
#ifdef CONFIG_ATBM_APOLLO_TESTMODE

			for (; i < 4; i++)
			{
				altbeam_tsm_stat[i].msrmt_duration = hw_priv->atbm_tsm_stats[i].msrmt_duration;
				altbeam_tsm_stat[i].tid = hw_priv->atbm_tsm_stats[i].tid;
				altbeam_tsm_stat[i].reporting_reason = hw_priv->atbm_tsm_stats[i].reporting_reason;
				altbeam_tsm_stat[i].txed_msdu_count = hw_priv->atbm_tsm_stats[i].txed_msdu_count;
				altbeam_tsm_stat[i].msdu_discarded_count = hw_priv->atbm_tsm_stats[i].msdu_discarded_count;
				altbeam_tsm_stat[i].msdu_failed_count = hw_priv->atbm_tsm_stats[i].msdu_failed_count;
				altbeam_tsm_stat[i].multi_retry_count = hw_priv->atbm_tsm_stats[i].multi_retry_count;
				altbeam_tsm_stat[i].qos_cfpolls_lost_count = hw_priv->atbm_tsm_stats[i].qos_cfpolls_lost_count;
				altbeam_tsm_stat[i].avg_q_delay = hw_priv->atbm_tsm_stats[i].avg_q_delay;
				altbeam_tsm_stat[i].avg_transmit_delay = hw_priv->atbm_tsm_stats[i].avg_transmit_delay;
			}
#endif
			ret = atbm_tesmode_reply(hw->wiphy, &altbeam_tsm_stat[0], sizeof(altbeam_tsm_stat));
			break;
		case ATBM_MSG_GET_TSM_PARAMS:			
			ret = atbm_get_tsm_params(hw);
			break;
		
		case ATBM_MSG_START_STOP_TSM:
			ret = atbm_start_stop_tsm(hw, (u8*)(&msg->value));
			break;
		case ATBM_MSG_GET_ROAM_DELAY:
			ret = atbm_get_roam_delay(hw);
			break;
		case ALTM_GET_STATUS_COMMON:
			seq.buff = buff;
			seq.private = (void *)hw_priv;
			seq.size = BUFFER_SIZE;
			seq.cout = 0;
			atbm_status_show_common((void *)&seq, NULL);
			ret = atbm_tesmode_reply(hw->wiphy, buff, seq.cout);
			break;
		case ALTM_GET_STATUS_COUNTERS:
			seq.buff = buff;
			seq.private = (void *)hw_priv;
			seq.size = BUFFER_SIZE;
			seq.cout = 0;
			atbm_counters_show((void *)&seq, NULL);
			ret = atbm_tesmode_reply(hw->wiphy, buff, seq.cout);
			break;
		case ALTM_GET_PACKETCNT:
			seq.buff = buff;
			seq.private = (void *)hw_priv;
			seq.size = BUFFER_SIZE;
			seq.cout = 0;
			ret = atbm_pkt_show((void *)&seq, NULL);
			ret |= atbm_tesmode_reply(hw->wiphy, buff, seq.cout);
			break;
		case ALTM_GET_STATUS_PRIV:
			seq.buff = buff;
			seq.private = (void *)hw_priv;
			seq.size = BUFFER_SIZE;
			seq.cout = 0;
			sta_printk(KERN_ERR "hw_bufs_used:     %d\n",hw_priv->hw_bufs_used);
			atbm_for_each_vif(hw_priv,vif,i){
				if(vif != NULL){
					seq.private = (void *)vif;
					seq.cout += snprintf(seq.buff + seq.cout, seq.size - seq.cout,
						"0===========atbm_status_show_priv====%s=====:\n",vif_to_sdata(vif->vif)->name);
					atbm_status_show_priv((void *)&seq, NULL);
				}
			}

			seq.cout += snprintf(seq.buff + seq.cout, seq.size - seq.cout,
				"=================End==================:\n");
			ret = atbm_tesmode_reply(hw->wiphy, buff, seq.cout);
			break;

		case ALTM_GET_RSSI:
			sta_printk(KERN_ERR "xxxxxxxxxxxxxxx\n");
			seq.buff = buff;
			seq.private = (void *)hw_priv;
			seq.size = BUFFER_SIZE;
			seq.cout = 0;
			ret = atbm_statistics_show((void *)&seq, NULL);
			ret |= atbm_tesmode_reply(hw->wiphy, buff, seq.cout);
			break;
		case ALTM_SET_PS_MODE:
			sta_printk(KERN_ERR "set ps mode(%d)\n",msg->value);
			ret = atbm_set_power_save_mode(hw, (u8 *)&msg->value, sizeof(msg->value));
			break;
		case ALTM_SET_TX_POWER:
			sta_printk(KERN_ERR "set_tx_power(%d)\n",msg->value);
			ret = atbm_set_tx_power_level(hw, msg->value);
			break;
		case ALTM_SET_DBG_PRINT_TO_HOST:
			ucDbgPrintOpenFlag = !!msg->value;

			sta_printk(KERN_ERR "%s dbgflag:%d\n", __func__, ucDbgPrintOpenFlag);

			atbm_for_each_vif(hw_priv,vif,i){
				if (vif != NULL)
				{
					WARN_ON(wsm_write_mib(hw_priv, WSM_MIB_ID_DBG_PRINT_TO_HOST,
						&ucDbgPrintOpenFlag, sizeof(ucDbgPrintOpenFlag), vif->if_id));
					break;
				}
			}
			break;
		case ALTM_SET_ZERO_COUNTERS_TABLE:
			sta_printk(KERN_ERR "set_zero_counters_table\n");
			atbm_for_each_vif(hw_priv,vif,i){
				if (vif != NULL)
				{
					WARN_ON(wsm_write_mib(hw_priv, WSM_MIB_ID_ZERO_COUNTERS_TABLE,
						&ucZeroCounterTable, sizeof(ucZeroCounterTable), vif->if_id));
					break;
				}
			}
			break;
		case ALTM_GET_DBG_PRINT_TO_HOST:
			sta_printk(KERN_ERR "ALTM_GET_DBG_PRINT_TO_HOST\n");
			WARN_ON(wsm_read_mib(hw_priv, WSM_MIB_ID_DBG_PRINT_TO_HOST,
			&ucDbgPrintOpenFlag, sizeof(ucDbgPrintOpenFlag),-1));
			ret = atbm_tesmode_reply(hw->wiphy, &ucDbgPrintOpenFlag, sizeof(ucDbgPrintOpenFlag));
			break;
		case ALTM_SET_FWCMD:
			pFwCmd = (char *)&msg[1];
			length = msg->value;
			atbm_printk_sta("enter atbm_set_fwcmd msg[1]:%s\n", (char *)&msg[1]);
			atbm_printk_sta("set fwcmd(%s)\n",pFwCmd);
			atbm_for_each_vif(hw_priv,vif,i){
				if (vif != NULL)
				{
					WARN_ON(wsm_write_mib(hw_priv, WSM_MIB_ID_FW_CMD,
						pFwCmd, length, vif->if_id));
					break;
				}
			}
			break;
		case ALTM_SET_SHORT_GI:
			sta_printk(KERN_ERR "set_short_gi(%d)\n",msg->value);
			atbm_set_shortGI(msg->value);
			atbm_tool_shortGi = msg->value;
			break;
		case ALTM_SHOW_EVENT_CMD:
			EELOG_Show();
			break;	
		case ALTM_CLEAR_EVENT_CMD:
			EELOG_Clear();
			break;
#ifdef ATBM_SUPPORT_SMARTCONFIG
		case ATBM_START_SMARTCONFIG:
			atbm_smartconfig_start(hw_priv,msg->value);
			break;
#endif
		case ATBM_SET_START_TX:
      			//mutex_lock(&hw_priv->conf_mutex);
                        //wsm_stop_tx(hw_priv);
                        //mutex_unlock(&hw_priv->conf_mutex);

			atbm_printk_sta("enter atbm_set_start_tx:value:%d\n", msg->value);
			if(ETF_bStart_Tx || ETF_bStart_Rx){
				atbm_printk_err("Error!:already start_tx,please stop_tx first!!\n");
				return 0;
			}
			//mutex_lock(&hw_priv->conf_mutex);
             //           wsm_stop_tx(hw_priv);
              //          mutex_unlock(&hw_priv->conf_mutex);

			if(hw->vendcmd_nl80211 == 0)
       			 {
               			 struct nlattr *data_p = nla_find(data, len, ATBM_TM_MSG_DATA);
               			 if (!data_p)
                        		return ret;
                		msg_txrx = (struct altm_msg_txrx *)nla_data(data_p);
     			   }
       			 else{
 				msg_txrx = (struct altm_msg_txrx *)data;
			}

			hw_priv->etf_channel = msg_txrx->externData[0];
			hw_priv->etf_rate = msg_txrx->externData[1];
			hw_priv->etf_len = msg_txrx->externData[2];
			hw_priv->etf_channel_type = msg_txrx->externData[3];
			hw_priv->etf_greedfiled = msg_txrx->externData[4];
			atbm_for_each_vif(hw_priv,vif,i){
				if(vif != NULL){
					atbm_printk_sta("*******\n");
					down(&hw_priv->scan.lock);
					mutex_lock(&hw_priv->conf_mutex);
					ETF_bStart_Tx = 1;
					wsm_start_tx(hw_priv, vif->vif);
					mutex_unlock(&hw_priv->conf_mutex);
					break;
				}
			}
			break;
		case ATBM_SET_STOP_TX:
			if(ETF_bStart_Tx == 0){
				atbm_printk_sta("please start_tx first, then stop_tx");
				return -EINVAL;
			}
			 mutex_lock(&hw_priv->conf_mutex);
			 wsm_stop_tx(hw_priv);
			 ETF_bStart_Tx = 0;
                         mutex_unlock(&hw_priv->conf_mutex);
			break;
		case ATBM_SET_START_RX:
			if(ETF_bStart_Rx || ETF_bStart_Tx){
				atbm_printk_sta("Error!already ETF_bStartTx/ETF_bStartRx,please stop_rx first!!!\n");
				return -EINVAL;
			}
			str = (char *)&msg[1];
			memset(str+msg->value, 0, strlen(str)-msg->value);
			memcpy(ch_and_type, str+10, msg->value-10);
			
			atbm_for_each_vif(hw_priv, vif, i){
				if(vif != NULL){
					ETF_bStart_Rx = 1;
					WARN_ON(wsm_write_mib(hw_priv, WSM_MIB_ID_FW_CMD,
						str, msg->value, vif->if_id));
				}
				break;
			}
			atbm_printk_sta("fwcmd:%s\n", str);
			break;
		case ATBM_SET_STOP_RX:
			if((0 == ETF_bStart_Rx) || (0 == ch_and_type[0])){
				atbm_printk_sta("please start_rx first,then stop_rx!!!\n");
				return -EINVAL;
			}
			str = (char *)&msg[1];
			memset(str+10, 0, 20);
			memcpy(str+10, ch_and_type, strlen(ch_and_type));

			atbm_for_each_vif(hw_priv, vif, i){
                                if(vif != NULL){
                                        ETF_bStart_Rx = 0;
                                        WARN_ON(wsm_write_mib(hw_priv, WSM_MIB_ID_FW_CMD,
                                                str, strlen(str), vif->if_id));
                                }
                                break;
                        }
		case ATBM_WRITE_REG:
			altm_shmem = (struct altm_msg_shmem *)msg;
			if (altm_shmem->size <= 4){
				atbm_printk_sta("Write Register %x=%x\n",altm_shmem->address,altm_shmem->buff);
				atbm_direct_write_reg_32(hw_priv,altm_shmem->address,altm_shmem->buff);
			}else{
				atbm_printk_sta("proccess error, size range 1~4\n");
			}
			break;
		case ATBM_READ_REG:
			altm_shmem = (struct altm_msg_shmem *)msg;
			if (altm_shmem->size <= 4){
				atbm_direct_read_reg_32(hw_priv,altm_shmem->address,&altm_shmem->buff);
				atbm_printk_sta("Read Register %x=%x\n",altm_shmem->address,altm_shmem->buff);
			}else{
				atbm_printk_sta("proccess error, size range 1~4\n");
			}
			break;
		default:
			break;
	}

	return ret;
}
#endif /* CONFIG_ATBM_APOLLO_TESTMODE */
