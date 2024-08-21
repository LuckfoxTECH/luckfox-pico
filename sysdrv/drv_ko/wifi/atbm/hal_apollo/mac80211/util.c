/*
 * Copyright 2002-2005, Instant802 Networks, Inc.
 * Copyright 2005-2006, Devicescape Software, Inc.
 * Copyright 2006-2007	Jiri Benc <jbenc@suse.cz>
 * Copyright 2007	Johannes Berg <johannes@sipsolutions.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * utilities for mac80211
 */

#include <net/atbm_mac80211.h>
#include <linux/netdevice.h>
#include <linux/export.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/skbuff.h>
#include <linux/etherdevice.h>
#include <linux/if_arp.h>
#include <linux/bitmap.h>
#include <linux/crc32.h>
#include <net/net_namespace.h>
#include <net/cfg80211.h>
#include <net/rtnetlink.h>

#include "ieee80211_i.h"
#include "driver-ops.h"
#include "rate.h"
#include "mesh.h"
#include "wme.h"
#include "led.h"
#include "wep.h"
#include "atbm_common.h"
#include "../apollo.h"
//#include <net/atbm_mac80211.h>

/* privid for wiphys to determine whether they belong to us or not */
void *mac80211_wiphy_privid = &mac80211_wiphy_privid;
#ifdef CONFIG_ATBM_MAC80211_NO_USE
struct ieee80211_hw *wiphy_to_ieee80211_hw(struct wiphy *wiphy)
{
	struct ieee80211_local *local;
	//BUG_ON(!wiphy);
	
	if(!wiphy){
		atbm_printk_err("%s %d ,ERROR !!! wiphy is NULL\n",__func__,__LINE__);
		return NULL;
	}
	local = wiphy_priv(wiphy);
	return &local->hw;
}
//EXPORT_SYMBOL(wiphy_to_ieee80211_hw);
#endif
u8 *ieee80211_get_bssid(struct ieee80211_hdr *hdr, size_t len,
			enum nl80211_iftype type)
{
	__le16 fc = hdr->frame_control;

	 /* drop ACK/CTS frames and incorrect hdr len (ctrl) */
	if (len < 16)
		return NULL;

	if (ieee80211_is_data(fc)) {
		if (len < 24) /* drop incorrect hdr len (data) */
			return NULL;

		if (ieee80211_has_a4(fc))
			return NULL;
		if (ieee80211_has_tods(fc))
			return hdr->addr1;
		if (ieee80211_has_fromds(fc))
			return hdr->addr2;

		return hdr->addr3;
	}

	if (ieee80211_is_mgmt(fc)) {
		if (len < 24) /* drop incorrect hdr len (mgmt) */
			return NULL;
		return hdr->addr3;
	}

	if (ieee80211_is_ctl(fc)) {
		if(ieee80211_is_pspoll(fc))
			return hdr->addr1;

		if (ieee80211_is_back_req(fc)) {
			switch (type) {
			case NL80211_IFTYPE_STATION:
				return hdr->addr2;
			case NL80211_IFTYPE_AP:
			case NL80211_IFTYPE_AP_VLAN:
				return hdr->addr1;
			default:
				break; /* fall through to the return */
			}
		}
	}

	return NULL;
}

void ieee80211_tx_set_protected(struct ieee80211_tx_data *tx)
{
	struct sk_buff *skb = tx->skb;
	struct ieee80211_hdr *hdr;

	do {
		hdr = (struct ieee80211_hdr *) skb->data;
		hdr->frame_control |= cpu_to_le16(IEEE80211_FCTL_PROTECTED);
	} while ((skb = skb->next));
}
#ifndef CONFIG_RATE_HW_CONTROL
int ieee80211_frame_duration(enum ieee80211_band band, size_t len,
			     int rate, int erp, int short_preamble)
{
	int dur;

	/* calculate duration (in microseconds, rounded up to next higher
	 * integer if it includes a fractional microsecond) to send frame of
	 * len bytes (does not include FCS) at the given rate. Duration will
	 * also include SIFS.
	 *
	 * rate is in 100 kbps, so divident is multiplied by 10 in the
	 * DIV_ROUND_UP() operations.
	 */

	if (band == IEEE80211_BAND_5GHZ || erp) {
		/*
		 * OFDM:
		 *
		 * N_DBPS = DATARATE x 4
		 * N_SYM = Ceiling((16+8xLENGTH+6) / N_DBPS)
		 *	(16 = SIGNAL time, 6 = tail bits)
		 * TXTIME = T_PREAMBLE + T_SIGNAL + T_SYM x N_SYM + Signal Ext
		 *
		 * T_SYM = 4 usec
		 * 802.11a - 17.5.2: aSIFSTime = 16 usec
		 * 802.11g - 19.8.4: aSIFSTime = 10 usec +
		 *	signal ext = 6 usec
		 */
		dur = 16; /* SIFS + signal ext */
		dur += 16; /* 17.3.2.3: T_PREAMBLE = 16 usec */
		dur += 4; /* 17.3.2.3: T_SIGNAL = 4 usec */
		dur += 4 * DIV_ROUND_UP((16 + 8 * (len + 4) + 6) * 10,
					4 * rate); /* T_SYM x N_SYM */
	} else {
		/*
		 * 802.11b or 802.11g with 802.11b compatibility:
		 * 18.3.4: TXTIME = PreambleLength + PLCPHeaderTime +
		 * Ceiling(((LENGTH+PBCC)x8)/DATARATE). PBCC=0.
		 *
		 * 802.11 (DS): 15.3.3, 802.11b: 18.3.4
		 * aSIFSTime = 10 usec
		 * aPreambleLength = 144 usec or 72 usec with short preamble
		 * aPLCPHeaderLength = 48 usec or 24 usec with short preamble
		 */
		dur = 10; /* aSIFSTime = 10 usec */
		dur += short_preamble ? (72 + 24) : (144 + 48);

		dur += DIV_ROUND_UP(8 * (len + 4) * 10, rate);
	}

	return dur;
}
#endif
#ifdef CONFIG_ATBM_MAC80211_NO_USE
/* Exported duration function for driver use */
__le16 ieee80211_generic_frame_duration(struct ieee80211_hw *hw,
					struct ieee80211_vif *vif,
					enum ieee80211_band band,
					size_t frame_len,
					struct ieee80211_rate *rate)
{
	struct ieee80211_sub_if_data *sdata;
	u16 dur;
	int erp;
	bool short_preamble = false;

	erp = 0;
	if (vif) {
		sdata = vif_to_sdata(vif);
		short_preamble = sdata->vif.bss_conf.use_short_preamble;
		if (sdata->flags & IEEE80211_SDATA_OPERATING_GMODE)
			erp = rate->flags & IEEE80211_RATE_ERP_G;
	}

	dur = ieee80211_frame_duration(band, frame_len, rate->bitrate, erp,
				       short_preamble);

	return cpu_to_le16(dur);
}
//EXPORT_SYMBOL(ieee80211_generic_frame_duration);

__le16 ieee80211_rts_duration(struct ieee80211_hw *hw,
			      struct ieee80211_vif *vif, size_t frame_len,
			      const struct ieee80211_tx_info *frame_txctl)
{
	struct ieee80211_local *local = hw_to_local(hw);
	struct ieee80211_rate *rate;
	struct ieee80211_sub_if_data *sdata;
	bool short_preamble;
	int erp;
	u16 dur;
	struct ieee80211_supported_band *sband;

	sband = local->hw.wiphy->bands[frame_txctl->band];

	short_preamble = false;

	rate = &sband->bitrates[frame_txctl->control.rts_cts_rate_idx];

	erp = 0;
	if (vif) {
		sdata = vif_to_sdata(vif);
		short_preamble = sdata->vif.bss_conf.use_short_preamble;
		if (sdata->flags & IEEE80211_SDATA_OPERATING_GMODE)
			erp = rate->flags & IEEE80211_RATE_ERP_G;
	}

	/* CTS duration */
	dur = ieee80211_frame_duration(sband->band, 10, rate->bitrate,
				       erp, short_preamble);
	/* Data frame duration */
	dur += ieee80211_frame_duration(sband->band, frame_len, rate->bitrate,
					erp, short_preamble);
	/* ACK duration */
	dur += ieee80211_frame_duration(sband->band, 10, rate->bitrate,
					erp, short_preamble);

	return cpu_to_le16(dur);
}
//EXPORT_SYMBOL(ieee80211_rts_duration);

__le16 ieee80211_ctstoself_duration(struct ieee80211_hw *hw,
				    struct ieee80211_vif *vif,
				    size_t frame_len,
				    const struct ieee80211_tx_info *frame_txctl)
{
	struct ieee80211_local *local = hw_to_local(hw);
	struct ieee80211_rate *rate;
	struct ieee80211_sub_if_data *sdata;
	bool short_preamble;
	int erp;
	u16 dur;
	struct ieee80211_supported_band *sband;

	sband = local->hw.wiphy->bands[frame_txctl->band];

	short_preamble = false;

	rate = &sband->bitrates[frame_txctl->control.rts_cts_rate_idx];
	erp = 0;
	if (vif) {
		sdata = vif_to_sdata(vif);
		short_preamble = sdata->vif.bss_conf.use_short_preamble;
		if (sdata->flags & IEEE80211_SDATA_OPERATING_GMODE)
			erp = rate->flags & IEEE80211_RATE_ERP_G;
	}

	/* Data frame duration */
	dur = ieee80211_frame_duration(sband->band, frame_len, rate->bitrate,
				       erp, short_preamble);
	if (!(frame_txctl->flags & IEEE80211_TX_CTL_NO_ACK)) {
		/* ACK duration */
		dur += ieee80211_frame_duration(sband->band, 10, rate->bitrate,
						erp, short_preamble);
	}

	return cpu_to_le16(dur);
}
//EXPORT_SYMBOL(ieee80211_ctstoself_duration);
#endif
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23))
static bool ieee80211_all_queues_started(struct ieee80211_hw *hw)
{
	unsigned int queue;

	for (queue = 0; queue < hw->queues; queue++)
		if (ieee80211_queue_stopped(hw, queue))
			return false;
	return true;
}
#endif

void ieee80211_propagate_queue_wake(struct ieee80211_local *local, int queue)
{
	struct ieee80211_sub_if_data *sdata;

	list_for_each_entry_rcu(sdata, &local->interfaces, list) {
		int ac;

#ifdef CONFIG_MAC80211_ATBM_ROAMING_CHANGES
		if (test_bit(SDATA_STATE_OFFCHANNEL, &sdata->state)
			    || sdata->queues_locked)
#else
		if (test_bit(SDATA_STATE_OFFCHANNEL, &sdata->state))
#endif
			continue;

		if (sdata->vif.cab_queue != IEEE80211_INVAL_HW_QUEUE &&
		    local->queue_stop_reasons[sdata->vif.cab_queue] != 0)
			continue;

		for (ac = 0; ac < IEEE80211_NUM_ACS; ac++) {
			int ac_queue = sdata->vif.hw_queue[ac];

			if (ac_queue == queue ||
			    (sdata->vif.cab_queue == queue &&
			     local->queue_stop_reasons[ac_queue] == 0 &&
			     atbm_skb_queue_empty(&local->pending[ac_queue]))){
				netif_wake_subqueue(sdata->dev, ac);
			}
		}
	}
}

static void __ieee80211_wake_queue(struct ieee80211_hw *hw, int queue,
				   enum queue_stop_reason reason)
{
	struct ieee80211_local *local = hw_to_local(hw);

	trace_wake_queue(local, queue, reason);

	if (WARN_ON(queue >= hw->queues))
		return;

	if (!test_bit(reason, &local->queue_stop_reasons[queue]))
		return;

	__clear_bit(reason, &local->queue_stop_reasons[queue]);

	if (local->queue_stop_reasons[queue] != 0)
		/* someone still has this queue stopped */
		return;

	if (atbm_skb_queue_empty(&local->pending[queue])) {
		rcu_read_lock();
		ieee80211_propagate_queue_wake(local, queue);
		rcu_read_unlock();
	} else
		tasklet_schedule(&local->tx_pending_tasklet);
}

void ieee80211_wake_queue_by_reason(struct ieee80211_hw *hw, int queue,
				    enum queue_stop_reason reason)
{
	struct ieee80211_local *local = hw_to_local(hw);
	unsigned long flags;

	spin_lock_irqsave(&local->queue_stop_reason_lock, flags);
	__ieee80211_wake_queue(hw, queue, reason);
	spin_unlock_irqrestore(&local->queue_stop_reason_lock, flags);
}

void ieee80211_wake_queue(struct ieee80211_hw *hw, int queue)
{
	ieee80211_wake_queue_by_reason(hw, queue,
				       IEEE80211_QUEUE_STOP_REASON_DRIVER);
}
//EXPORT_SYMBOL(ieee80211_wake_queue);

static void __ieee80211_stop_queue(struct ieee80211_hw *hw, int queue,
				   enum queue_stop_reason reason)
{
	struct ieee80211_local *local = hw_to_local(hw);
	struct ieee80211_sub_if_data *sdata;

	trace_stop_queue(local, queue, reason);

	if (WARN_ON(queue >= hw->queues))
		return;

	if (test_bit(reason, &local->queue_stop_reasons[queue]))
		return;

	__set_bit(reason, &local->queue_stop_reasons[queue]);

	rcu_read_lock();
	list_for_each_entry_rcu(sdata, &local->interfaces, list) {
		int ac;

		for (ac = 0; ac < IEEE80211_NUM_ACS; ac++) {
			if (sdata->vif.hw_queue[ac] == queue ||
			    sdata->vif.cab_queue == queue)
				netif_stop_subqueue(sdata->dev, ac);
		}
	}
	rcu_read_unlock();
}

void ieee80211_stop_queue_by_reason(struct ieee80211_hw *hw, int queue,
				    enum queue_stop_reason reason)
{
	struct ieee80211_local *local = hw_to_local(hw);
	unsigned long flags;

	spin_lock_irqsave(&local->queue_stop_reason_lock, flags);
	__ieee80211_stop_queue(hw, queue, reason);
	spin_unlock_irqrestore(&local->queue_stop_reason_lock, flags);
}

void ieee80211_stop_queue(struct ieee80211_hw *hw, int queue)
{
	ieee80211_stop_queue_by_reason(hw, queue,
				       IEEE80211_QUEUE_STOP_REASON_DRIVER);
}
//EXPORT_SYMBOL(ieee80211_stop_queue);

void ieee80211_add_pending_skb(struct ieee80211_local *local,
			       struct sk_buff *skb)
{
	struct ieee80211_hw *hw = &local->hw;
	unsigned long flags;
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	int queue = info->hw_queue;

	if (WARN_ON(!info->control.vif)) {
		atbm_kfree_skb(skb);
		return;
	}

	spin_lock_irqsave(&local->queue_stop_reason_lock, flags);
	__ieee80211_stop_queue(hw, queue, IEEE80211_QUEUE_STOP_REASON_SKB_ADD);
	__atbm_skb_queue_tail(&local->pending[queue], skb);
	__ieee80211_wake_queue(hw, queue, IEEE80211_QUEUE_STOP_REASON_SKB_ADD);
	spin_unlock_irqrestore(&local->queue_stop_reason_lock, flags);
}

void ieee80211_add_pending_skbs_fn(struct ieee80211_local *local,
				   struct sk_buff_head *skbs,
				   void (*fn)(void *data), void *data)
{
	struct ieee80211_hw *hw = &local->hw;
	struct sk_buff *skb;
	unsigned long flags;
	int queue, i;

	spin_lock_irqsave(&local->queue_stop_reason_lock, flags);
	while ((skb = atbm_skb_dequeue(skbs))) {
		struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);

		if (WARN_ON(!info->control.vif)) {
			atbm_kfree_skb(skb);
			continue;
		}

		queue = info->hw_queue;

		__ieee80211_stop_queue(hw, queue,
				IEEE80211_QUEUE_STOP_REASON_SKB_ADD);

		__atbm_skb_queue_tail(&local->pending[queue], skb);
	}

	if (fn)
		fn(data);

	for (i = 0; i < hw->queues; i++)
		__ieee80211_wake_queue(hw, i,
			IEEE80211_QUEUE_STOP_REASON_SKB_ADD);
	spin_unlock_irqrestore(&local->queue_stop_reason_lock, flags);
}

void ieee80211_add_pending_skbs(struct ieee80211_local *local,
				struct sk_buff_head *skbs)
{
	ieee80211_add_pending_skbs_fn(local, skbs, NULL, NULL);
}

void ieee80211_stop_queues_by_reason(struct ieee80211_hw *hw,
				    enum queue_stop_reason reason)
{
	struct ieee80211_local *local = hw_to_local(hw);
	unsigned long flags;
	int i;

	spin_lock_irqsave(&local->queue_stop_reason_lock, flags);

	for (i = 0; i < hw->queues; i++)
		__ieee80211_stop_queue(hw, i, reason);

	spin_unlock_irqrestore(&local->queue_stop_reason_lock, flags);
}

void ieee80211_stop_queues(struct ieee80211_hw *hw)
{
	ieee80211_stop_queues_by_reason(hw,
					IEEE80211_QUEUE_STOP_REASON_DRIVER);
}
//EXPORT_SYMBOL(ieee80211_stop_queues);

int ieee80211_queue_stopped(struct ieee80211_hw *hw, int queue)
{
	struct ieee80211_local *local = hw_to_local(hw);
	unsigned long flags;
	int ret;

	if (WARN_ON(queue >= hw->queues))
		return true;

	spin_lock_irqsave(&local->queue_stop_reason_lock, flags);
	ret = !!local->queue_stop_reasons[queue];
	spin_unlock_irqrestore(&local->queue_stop_reason_lock, flags);
	return ret;
}
//EXPORT_SYMBOL(ieee80211_queue_stopped);

void ieee80211_wake_queues_by_reason(struct ieee80211_hw *hw,
				     enum queue_stop_reason reason)
{
	struct ieee80211_local *local = hw_to_local(hw);
	unsigned long flags;
	int i;

	spin_lock_irqsave(&local->queue_stop_reason_lock, flags);

	for (i = 0; i < hw->queues; i++)
		__ieee80211_wake_queue(hw, i, reason);

	spin_unlock_irqrestore(&local->queue_stop_reason_lock, flags);
}

void ieee80211_wake_queues(struct ieee80211_hw *hw)
{
	ieee80211_wake_queues_by_reason(hw, IEEE80211_QUEUE_STOP_REASON_DRIVER);
}
//EXPORT_SYMBOL(ieee80211_wake_queues);
#ifdef CONFIG_ATBM_MAC80211_NO_USE
void ieee80211_iterate_active_interfaces(
	struct ieee80211_hw *hw,
	void (*iterator)(void *data, u8 *mac,
			 struct ieee80211_vif *vif),
	void *data)
{
	struct ieee80211_local *local = hw_to_local(hw);
	struct ieee80211_sub_if_data *sdata;

	mutex_lock(&local->iflist_mtx);

	list_for_each_entry(sdata, &local->interfaces, list) {
		switch (sdata->vif.type) {
		case NL80211_IFTYPE_MONITOR:
		case NL80211_IFTYPE_AP_VLAN:
			continue;
		default:
			break;
		}
		if (ieee80211_sdata_running(sdata))
			iterator(data, sdata->vif.addr,
				 &sdata->vif);
	}

	mutex_unlock(&local->iflist_mtx);
}
//EXPORT_SYMBOL_GPL(ieee80211_iterate_active_interfaces);
#endif

void ieee80211_iterate_active_interfaces_atomic(
	struct ieee80211_hw *hw,
	void (*iterator)(void *data, u8 *mac,
			 struct ieee80211_vif *vif),
	void *data)
{
	struct ieee80211_local *local = hw_to_local(hw);
	struct ieee80211_sub_if_data *sdata;

	if((local->quiescing == true) || (local->suspended == true))
		return;
	rcu_read_lock();

	list_for_each_entry_rcu(sdata, &local->interfaces, list) {
		switch (sdata->vif.type) {
		case NL80211_IFTYPE_MONITOR:
		case NL80211_IFTYPE_AP_VLAN:
			continue;
		default:
			break;
		}
		if (ieee80211_sdata_running(sdata))
			iterator(data, sdata->vif.addr,
				 &sdata->vif);
	}

	rcu_read_unlock();
}
//EXPORT_SYMBOL_GPL(ieee80211_iterate_active_interfaces_atomic);

/*
 * Nothing should have been stuffed into the workqueue during
 * the suspend->resume cycle. If this WARN is seen then there
 * is a bug with either the driver suspend or something in
 * mac80211 stuffing into the workqueue which we haven't yet
 * cleared during mac80211's suspend cycle.
 */
static bool ieee80211_can_queue_work(struct ieee80211_local *local)
{
	if ((local->quiescing == true)||(local->suspended && !local->resuming)){
		pr_warn("queueing ieee80211 work while going to suspend\n");
		return false;
	}
	return true;
}

void ieee80211_queue_work(struct ieee80211_hw *hw, struct atbm_work_struct *work)
{
	struct ieee80211_local *local = hw_to_local(hw);

	if (!ieee80211_can_queue_work(local))
		return;

	atbm_queue_work(local->workqueue, work);
}
//EXPORT_SYMBOL(ieee80211_queue_work);

void ieee80211_queue_delayed_work(struct ieee80211_hw *hw,
				  struct atbm_delayed_work *dwork,
				  unsigned long delay)
{
	struct ieee80211_local *local = hw_to_local(hw);

	if (!ieee80211_can_queue_work(local))
		return;

	atbm_queue_delayed_work(local->workqueue, dwork, delay);
}
//EXPORT_SYMBOL(ieee80211_queue_delayed_work);

static int ieee802_11_parse_vendor_specific(u8 *pos, size_t elen,
					    struct ieee802_atbm_11_elems *elems,
					    int show_errors)
{
	unsigned int oui;
	//unsigned char i = 0,offsert = 4;
	//int encry_type_count = 0;

	/* first 3 bytes in vendor specific information element are the IEEE
	 * OUI of the vendor. The following byte is used a vendor specific
	 * sub-type. */
	if (elen < 4) {
		if (show_errors) {
			atbm_printk_err("short vendor specific "
				   "information element ignored (len=%lu)",
				   (unsigned long) elen);
		}
		return -1;
	}

	oui = ATBM_WPA_GET_BE24(pos);
	switch (oui) {
	case ATBM_OUI_MICROSOFT:
		/* Microsoft/Wi-Fi information elements are further typed and
		 * subtyped */
		switch (pos[3]) {
		case 1:
			/* Microsoft OUI (00:50:F2) with OUI Type 1:
			 * real WPA information element */
			elems->wpa = pos;
			elems->wpa_len = elen;
			break;
		case ATBM_WMM_OUI_TYPE:
			/* WMM information element */
			if (elen < 5) {
				atbm_printk_debug( "short WMM "
					   "information element ignored "
					   "(len=%lu)",
					   (unsigned long) elen);
				return -1;
			}
			switch (pos[4]) {
			case ATBM_WMM_OUI_SUBTYPE_INFORMATION_ELEMENT:
				elems->wmm_info = pos;
				elems->wmm_info_len = elen;
				break;
			case ATBM_WMM_OUI_SUBTYPE_PARAMETER_ELEMENT:
				elems->wmm_param = pos;
				elems->wmm_param_len = elen;
				break;
			default:
				atbm_printk_debug( "unknown WMM "
					   "information element ignored "
					   "(subtype=%d len=%lu)",
					   pos[4], (unsigned long) elen);
				return -1;
			}
			break;
		case 4:
			/* Wi-Fi Protected Setup (WPS) IE */
			elems->wps_ie = pos;
			elems->wps_ie_len = elen;
			break;
		default:
			atbm_printk_debug( "Unknown Microsoft "
				   "information element ignored "
				   "(type=%d len=%lu)",
				   pos[3], (unsigned long) elen);
			return -1;
		}
		break;

	case ATBM_OUI_WFA:
		switch (pos[3]) {
		case ATBM_P2P_OUI_TYPE:
			/* Wi-Fi Alliance - P2P IE */
			elems->p2p_ie = pos;
			elems->p2p_ie_len = elen;
			break;
		case ATBM_WFD_OUI_TYPE:
			/* Wi-Fi Alliance - WFD IE */
			elems->wfd = pos;
			elems->wfd_len = elen;
			break;
		default:
			atbm_printk_debug("Unknown WFA "
				   "information element ignored "
				   "(type=%d len=%lu)\n",
				   pos[3], (unsigned long) elen);
			return -1;
		}
		break;

	default:
		/*
		printk( "unknown vendor specific "
			   "information element ignored (vendor OUI "
			   "%02x:%02x:%02x len=%lu)",
			   pos[0], pos[1], pos[2], (unsigned long) elen);*/
		return -1;
	}

	return 0;
}

u32 atbm_ieee802_11_parse_elems_crc(u8 *start, size_t len,
			       struct ieee802_atbm_11_elems *elems,
			       u64 filter, u32 crc)
{
	size_t left = len;
	u8 *pos = start;
	bool calc_crc = filter != 0;
	//unsigned char i = 0,offsert = 4;
	//int encry_type_count = 0;
	memset(elems, 0, sizeof(*elems));
	elems->ie_start = start;
	elems->total_len = len;
	elems->encry_info = 0;
	while (left >= 2) {
		u8 id, elen;

		id = *pos++;
		elen = *pos++;
		left -= 2;

		if (elen > left)
			break;

		if (calc_crc && id < 64 && (filter & (1ULL << id)))
			crc = crc32_be(crc, pos - 2, elen + 2);

		switch (id) {
		case ATBM_WLAN_EID_SSID:
			elems->ssid = pos;
			elems->ssid_len = elen;
			break;
		case ATBM_WLAN_EID_SUPP_RATES:
			elems->supp_rates = pos;
			elems->supp_rates_len = elen;
			break;
		case ATBM_WLAN_EID_FH_PARAMS:
			elems->fh_params = pos;
			elems->fh_params_len = elen;
			break;
		case ATBM_WLAN_EID_DS_PARAMS:
			elems->ds_params = pos;
			elems->ds_params_len = elen;
			break;
		case ATBM_WLAN_EID_CF_PARAMS:
			elems->cf_params = pos;
			elems->cf_params_len = elen;
			break;
		case ATBM_WLAN_EID_TIM:
			if (elen >= sizeof(struct ieee80211_tim_ie)) {
				elems->tim = (void *)pos;
				elems->tim_len = elen;
			}
			break;
		case ATBM_WLAN_EID_IBSS_PARAMS:
			elems->ibss_params = pos;
			elems->ibss_params_len = elen;
			break;
		case ATBM_WLAN_EID_CHALLENGE:
			elems->challenge = pos;
			elems->challenge_len = elen;
			break;
		case ATBM_WLAN_EID_VENDOR_SPECIFIC:
			#if 0
			if (elen >= 4 && pos[0] == 0x00 && pos[1] == 0x50 &&
			    pos[2] == 0xf2) {
				/* Microsoft OUI (00:50:F2) */

				if (calc_crc)
					crc = crc32_be(crc, pos - 2, elen + 2);

				if (pos[3] == 1) {
					/* OUI Type 1 - WPA IE */
					elems->wpa = pos;
					elems->wpa_len = elen;
				} else if (elen >= 5 && pos[3] == 2) {
					/* OUI Type 2 - WMM IE */
					if (pos[4] == 0) {
						elems->wmm_info = pos;
						elems->wmm_info_len = elen;
					} else if (pos[4] == 1) {
						elems->wmm_param = pos;
						elems->wmm_param_len = elen;
					}
				}
			}
			/*
			*process p2p ie
			*p2p outui {0x50,0x6F,0x9A,0x09};
			*/
			if(elen >= 4 && pos[0] == 0x50 && pos[1] == 0x6F &&
			    pos[2] == 0x9A && pos[3]== 0x09)
			{
				if (calc_crc)
					crc = crc32_be(crc, pos - 2, elen + 2);
				elems->p2p_ie = pos;
				elems->p2p_ie_len = elen;
			}
			#endif
			ieee802_11_parse_vendor_specific(pos,elen,elems,1);
			break;
		case ATBM_WLAN_EID_RSN:
			elems->rsn = pos;
			elems->rsn_len = elen;

			break;
		case ATBM_WLAN_EID_ERP_INFO:
			elems->erp_info = pos;
			elems->erp_info_len = elen;
			break;
		case ATBM_WLAN_EID_EXT_SUPP_RATES:
			elems->ext_supp_rates = pos;
			elems->ext_supp_rates_len = elen;
			break;
		case ATBM_WLAN_EID_HT_CAPABILITY:
			if (elen >= sizeof(struct ieee80211_ht_cap))
				elems->ht_cap_elem = (void *)pos;
			break;
		case ATBM_WLAN_EID_HT_INFORMATION:
			if (elen >= sizeof(struct ieee80211_ht_info))
				elems->ht_info_elem = (void *)pos;
			break;
		case ATBM_WLAN_EID_MESH_ID:
			elems->mesh_id = pos;
			elems->mesh_id_len = elen;
			break;
		case ATBM_WLAN_EID_MESH_CONFIG:
			if (elen >= sizeof(struct ieee80211_meshconf_ie))
				elems->mesh_config = (void *)pos;
			break;
		case ATBM_WLAN_EID_PEER_MGMT:
			elems->peering = pos;
			elems->peering_len = elen;
			break;
		case ATBM_WLAN_EID_PREQ:
			elems->preq = pos;
			elems->preq_len = elen;
			break;
		case ATBM_WLAN_EID_PREP:
			elems->prep = pos;
			elems->prep_len = elen;
			break;
		case ATBM_WLAN_EID_PERR:
			elems->perr = pos;
			elems->perr_len = elen;
			break;
		case ATBM_WLAN_EID_RANN:
			if (elen >= sizeof(struct ieee80211_rann_ie))
				elems->rann = (void *)pos;
			break;
		case ATBM_WLAN_EID_CHANNEL_SWITCH:
			elems->ch_switch_elem = pos;
			elems->ch_switch_elem_len = elen;
			break;
		case ATBM_WLAN_EID_QUIET:
			if (!elems->quiet_elem) {
				elems->quiet_elem = pos;
				elems->quiet_elem_len = elen;
			}
			elems->num_of_quiet_elem++;
			break;
		case ATBM_WLAN_EID_COUNTRY:
			elems->country_elem = pos;
			elems->country_elem_len = elen;
			break;
		case ATBM_WLAN_EID_PWR_CONSTRAINT:
			elems->pwr_constr_elem = pos;
			elems->pwr_constr_elem_len = elen;
			break;
		case ATBM_WLAN_EID_TIMEOUT_INTERVAL:
			elems->timeout_int = pos;
			elems->timeout_int_len = elen;
			break;
		case ATBM_WLAN_EID_EXT_CHANSWITCH_ANN:
			elems->extended_ch_switch_elem = pos;
			elems->extended_ch_switch_elem_len = elen;
			break;
		case ATBM_WLAN_EID_SECONDARY_CH_OFFSET:
			elems->secondary_ch_elem=pos;
			elems->secondary_ch_elem_len=elen;

			break;
		case ATBM_WLAN_EID_PRIVATE:
			elems->atbm_special = pos;
			elems->atbm_special_len = elen;
			break;
		default:
			break;
		}

		left -= elen;
		pos += elen;
	}

	return crc;
}

void ieee802_11_parse_elems(u8 *start, size_t len,
			    struct ieee802_atbm_11_elems *elems)
{
	atbm_ieee802_11_parse_elems_crc(start, len, elems, 0, 0);
}

void ieee80211_set_wmm_default(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(local, sdata);
	struct ieee80211_tx_queue_params qparam;
	int ac;
	bool use_11b;
	int aCWmin, aCWmax;

	if (!local->ops->conf_tx)
		return;

	if (local->hw.queues < IEEE80211_NUM_ACS)
		return;

	memset(&qparam, 0, sizeof(qparam));

	use_11b = (chan_state->conf.channel->band == IEEE80211_BAND_2GHZ) &&
		 !(sdata->flags & IEEE80211_SDATA_OPERATING_GMODE);

	for (ac = 0; ac < IEEE80211_NUM_ACS; ac++) {
		/* Set defaults according to 802.11-2007 Table 7-37 */
		aCWmax = 1023;
		if (use_11b)
			aCWmin = 31;
		else
			aCWmin = 15;

		switch (ac) {
		case 3: /* AC_BK */
			qparam.cw_max = aCWmax;
			qparam.cw_min = aCWmin;
			qparam.txop = 0;
			qparam.aifs = 7;
			break;
		default: /* never happens but let's not leave undefined */
		case 2: /* AC_BE */
			qparam.cw_max = aCWmax;
			qparam.cw_min = aCWmin;
			qparam.txop = 0;
			qparam.aifs = 3;
			break;
		case 1: /* AC_VI */
			qparam.cw_max = aCWmin;
			qparam.cw_min = (aCWmin + 1) / 2 - 1;
			if (use_11b)
				qparam.txop = 6016/32;
			else
				qparam.txop = 3008/32;
			qparam.aifs = 2;
			break;
		case 0: /* AC_VO */
			qparam.cw_max = (aCWmin + 1) / 2 - 1;
			qparam.cw_min = (aCWmin + 1) / 4 - 1;
			if (use_11b)
				qparam.txop = 3264/32;
			else
				qparam.txop = 1504/32;
			qparam.aifs = 2;
			break;
		}

		qparam.uapsd = false;

		sdata->tx_conf[ac] = qparam;
		drv_conf_tx(local, sdata, ac, &qparam);
	}

	/* after reinitialize QoS TX queues setting to default,
	 * disable QoS at all */

	if (sdata->vif.type != NL80211_IFTYPE_MONITOR) {
		sdata->vif.bss_conf.qos =
			sdata->vif.type != NL80211_IFTYPE_STATION;
		ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_QOS);
	}
}

void ieee80211_sta_def_wmm_params(struct ieee80211_sub_if_data *sdata,
				  const size_t supp_rates_len,
				  const u8 *supp_rates)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(local, sdata);
	int i, have_higher_than_11mbit = 0;

	/* cf. IEEE 802.11 9.2.12 */
	for (i = 0; i < supp_rates_len; i++)
		if ((supp_rates[i] & 0x7f) * 5 > 110)
			have_higher_than_11mbit = 1;

	if (chan_state->conf.channel->band == IEEE80211_BAND_2GHZ &&
	    have_higher_than_11mbit)
		sdata->flags |= IEEE80211_SDATA_OPERATING_GMODE;
	else
		sdata->flags &= ~IEEE80211_SDATA_OPERATING_GMODE;

	ieee80211_set_wmm_default(sdata);
}

u32 ieee80211_atbm_mandatory_rates(struct ieee80211_local *local,
			      enum ieee80211_band band)
{
	struct ieee80211_supported_band *sband;
	struct ieee80211_rate *bitrates;
	u32 mandatory_rates;
	enum ieee80211_rate_flags mandatory_flag;
	int i;

	sband = local->hw.wiphy->bands[band];
	if (WARN_ON(!sband))
		return 1;

	if (band == IEEE80211_BAND_2GHZ)
		mandatory_flag = IEEE80211_RATE_MANDATORY_B;
	else
		mandatory_flag = IEEE80211_RATE_MANDATORY_A;

	bitrates = sband->bitrates;
	mandatory_rates = 0;
	for (i = 0; i < sband->n_bitrates; i++)
		if (bitrates[i].flags & mandatory_flag)
			mandatory_rates |= BIT(i);
	return mandatory_rates;
}

void ieee80211_send_auth(struct ieee80211_sub_if_data *sdata,
			 u16 transaction, u16 auth_alg, u16 status,
			 u8 *extra, size_t extra_len, const u8 *bssid,
			 const u8 *key, u8 key_len, u8 key_idx)
{
	struct ieee80211_local *local = sdata->local;
	struct sk_buff *skb;
	struct atbm_ieee80211_mgmt *mgmt;
#ifdef CONFIG_ATBM_USE_SW_ENC
	int err;
#endif

	skb = atbm_dev_alloc_skb(local->hw.extra_tx_headroom +
			    sizeof(*mgmt) + 6 + extra_len);
	if (!skb)
		return;

	atbm_skb_reserve(skb, local->hw.extra_tx_headroom);

	mgmt = (struct atbm_ieee80211_mgmt *) atbm_skb_put(skb, 24 + 6);
	memset(mgmt, 0, 24 + 6);
	mgmt->frame_control = cpu_to_le16(IEEE80211_FTYPE_MGMT |
					  IEEE80211_STYPE_AUTH);
	memcpy(mgmt->da, bssid, ETH_ALEN);
	memcpy(mgmt->sa, sdata->vif.addr, ETH_ALEN);
	memcpy(mgmt->bssid, bssid, ETH_ALEN);
	mgmt->u.auth.auth_alg = cpu_to_le16(auth_alg);
	mgmt->u.auth.auth_transaction = cpu_to_le16(transaction);
	mgmt->u.auth.status_code = cpu_to_le16(status);
	if (extra)
		memcpy(atbm_skb_put(skb, extra_len), extra, extra_len);
	if (auth_alg == WLAN_AUTH_SHARED_KEY && transaction == 3) {
#ifdef CONFIG_ATBM_USE_SW_ENC
		mgmt->frame_control |= cpu_to_le16(IEEE80211_FCTL_PROTECTED);
		err = ieee80211_wep_encrypt(local, skb, key, key_len, key_idx);
		WARN_ON(err);
#else
		mgmt->u.auth.status_code = cpu_to_le16(1);
 		atbm_printk_always("%s:not support shared key\n",__func__);
#endif
	}
 	atbm_printk_mgmt( "%s %d transaction =%d,key_idx %d,key <%s> \n",__func__,__LINE__,transaction,key_idx,key);
	IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT;
	ieee80211_tx_skb(sdata, skb);
}

int ieee80211_build_preq_ies(struct ieee80211_local *local, u8 *buffer,
			     const u8 *ie, size_t ie_len,
			     enum ieee80211_band band, u32 rate_mask,
			     u8 channel)
{
	struct ieee80211_supported_band *sband;
	u8 *pos;
	size_t offset = 0, noffset;
	int supp_rates_len, i;
	u8 rates[32];
	int num_rates;
	int ext_rates_len;

	sband = local->hw.wiphy->bands[band];

	pos = buffer;
#ifdef CONFIG_ATBM_SUPPORT_SCHED_SCAN
#ifdef ROAM_OFFLOAD
	if (!sband)
		goto out;
#endif /*ROAM_OFFLOAD*/
#endif
	num_rates = 0;
	for (i = 0; i < sband->n_bitrates; i++) {
		if ((BIT(i) & rate_mask) == 0)
			continue; /* skip rate */
		if(num_rates<4){
			rates[num_rates++] = (u8) ((sband->bitrates[i].bitrate / 5)|0x80);
		}else{
			rates[num_rates++] = (u8) (sband->bitrates[i].bitrate / 5);

		}
	}

	supp_rates_len = min_t(int, num_rates, 8);

	*pos++ = ATBM_WLAN_EID_SUPP_RATES;
	*pos++ = supp_rates_len;
	memcpy(pos, rates, supp_rates_len);
	pos += supp_rates_len;

	/* insert "request information" if in custom IEs */
	if (ie && ie_len) {
		static const u8 before_extrates[] = {
			ATBM_WLAN_EID_SSID,
			ATBM_WLAN_EID_SUPP_RATES,
			ATBM_WLAN_EID_REQUEST,
		};
		noffset = atbm_ieee80211_ie_split(ie, ie_len,
					     before_extrates,
					     ARRAY_SIZE(before_extrates),
					     offset);
		memcpy(pos, ie + offset, noffset - offset);
		pos += noffset - offset;
		offset = noffset;
	}

	ext_rates_len = num_rates - supp_rates_len;
	if (ext_rates_len > 0) {
		*pos++ = ATBM_WLAN_EID_EXT_SUPP_RATES;
		*pos++ = ext_rates_len;
		memcpy(pos, rates + supp_rates_len, ext_rates_len);
		pos += ext_rates_len;
	}

	if (channel && sband->band == IEEE80211_BAND_2GHZ) {
		*pos++ = ATBM_WLAN_EID_DS_PARAMS;
		*pos++ = 1;
		*pos++ = channel;
	}

	/* insert custom IEs that go before HT */
	if (ie && ie_len) {
		static const u8 before_ht[] = {
			ATBM_WLAN_EID_SSID,
			ATBM_WLAN_EID_SUPP_RATES,
			ATBM_WLAN_EID_REQUEST,
			ATBM_WLAN_EID_EXT_SUPP_RATES,
			ATBM_WLAN_EID_DS_PARAMS,
			ATBM_WLAN_EID_SUPPORTED_REGULATORY_CLASSES,
		};
		noffset = atbm_ieee80211_ie_split(ie, ie_len,
					     before_ht, ARRAY_SIZE(before_ht),
					     offset);
		memcpy(pos, ie + offset, noffset - offset);
		pos += noffset - offset;
		offset = noffset;
	}

	if (sband->ht_cap.ht_supported) {
		u16 cap = sband->ht_cap.cap;
		__le16 tmp;

		*pos++ = ATBM_WLAN_EID_HT_CAPABILITY;
		*pos++ = sizeof(struct ieee80211_ht_cap);
		memset(pos, 0, sizeof(struct ieee80211_ht_cap));
		tmp = cpu_to_le16(cap);
		memcpy(pos, &tmp, sizeof(u16));
		pos += sizeof(u16);
		*pos++ = sband->ht_cap.ampdu_factor |
			 (sband->ht_cap.ampdu_density <<
				IEEE80211_HT_AMPDU_PARM_DENSITY_SHIFT);
		memcpy(pos, &sband->ht_cap.mcs, sizeof(sband->ht_cap.mcs));
		pos += sizeof(sband->ht_cap.mcs);
		pos += 2 + 4 + 1; /* ext info, BF cap, antsel */
	}

	/*
	 * If adding more here, adjust code in main.c
	 * that calculates local->scan_ies_len.
	 */

	/* add any remaining custom IEs */
	if (ie && ie_len) {
		noffset = ie_len;
		memcpy(pos, ie + offset, noffset - offset);
		pos += noffset - offset;
	}
#ifdef CONFIG_ATBM_SUPPORT_SCHED_SCAN
#ifdef ROAM_OFFLOAD
out:
#endif /*ROAM_OFFLOAD*/
#endif
	return pos - buffer;
}

struct sk_buff *ieee80211_build_probe_req(struct ieee80211_sub_if_data *sdata,
					  u8 *dst, u32 ratemask,
					  const u8 *ssid, size_t ssid_len,
					  const u8 *ie, size_t ie_len,
					  bool directed)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(local, sdata);
	struct sk_buff *skb;
	struct atbm_ieee80211_mgmt *mgmt;
	size_t buf_len;
	u8 *buf;
	u8 chan;

	/* FIXME: come up with a proper value */
	buf = atbm_kmalloc(200 + ie_len, GFP_KERNEL);
	if (!buf)
		return NULL;

	/*
	 * Do not send DS Channel parameter for directed probe requests
	 * in order to maximize the chance that we get a response.  Some
	 * badly-behaved APs don't respond when this parameter is included.
	 */
	if (directed)
		chan = 0;
	else{
#ifdef AP_MODE_SEND_PROBE_REQ	

		if(sdata->vif.type == NL80211_IFTYPE_AP){
			chan = channel_hw_value(chan_state->oper_channel);
			atbm_printk_err("ieee80211_build_probe_req : ap work channel[%d] \n",chan);
		}else
#endif
		{
			chan = ieee80211_frequency_to_channel(
				channel_center_freq(chan_state->conf.channel));
		}
	}
	buf_len = ieee80211_build_preq_ies(local, buf, ie, ie_len,
					   chan_state->conf.channel->band,
					   ratemask, chan);

	skb = ieee80211_probereq_get(&local->hw, &sdata->vif,
				     ssid, ssid_len,
				     buf, buf_len,NULL);
	if (!skb)
		goto out;

	if (dst) {
		mgmt = (struct atbm_ieee80211_mgmt *) skb->data;
		memcpy(mgmt->da, dst, ETH_ALEN);
		memcpy(mgmt->bssid, dst, ETH_ALEN);
	}

	IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT;

 out:
	atbm_kfree(buf);

	return skb;
}

void ieee80211_send_probe_req(struct ieee80211_sub_if_data *sdata, u8 *dst,
			      const u8 *ssid, size_t ssid_len,
			      const u8 *ie, size_t ie_len,
			      u32 ratemask, bool directed, bool no_cck)
{
	struct sk_buff *skb;

	skb = ieee80211_build_probe_req(sdata, dst, ratemask, ssid, ssid_len,
					ie, ie_len, directed);
	if (skb) {
		if (no_cck){
			IEEE80211_SKB_CB(skb)->flags |=
				IEEE80211_TX_CTL_NO_CCK_RATE;
		//	IEEE80211_SKB_CB(skb)->control.rates[0].flags |= IEEE80211_TX_RC_MCS; 
		}
		ieee80211_tx_skb(sdata, skb);
	}
}
bool ieee80211_send_special_probe_req(struct ieee80211_sub_if_data *sdata, u8 *dst,
			      const u8 *ssid, size_t ssid_len,
			      const u8 *special_ie, size_t special_ie_len)
{
	u8 *special = NULL;
#ifndef AP_MODE_SEND_PROBE_REQ	
	if((sdata->vif.type != NL80211_IFTYPE_STATION)&&(sdata->vif.type != NL80211_IFTYPE_MONITOR)){
		return false;
	}
#endif
	if(special_ie_len > 255){
		return false;
	}

	if(special_ie&&special_ie_len){
		special = atbm_kmalloc(special_ie_len+2,GFP_ATOMIC);

		if(special == NULL){
			return false;
		}

		special[0] = ATBM_WLAN_EID_PRIVATE;
		special[1] = special_ie_len;

		memcpy(&special[2],special_ie,special_ie_len);
	}

	ieee80211_send_probe_req(sdata,dst,ssid,ssid_len,special,special_ie_len?special_ie_len+2:0,-1,true,false);

	if(special)
		atbm_kfree(special);
	return true;
}

bool ieee80211_send_special_probe_response(struct ieee80211_sub_if_data *sdata, u8 *dst,
			      const u8 *special_ie, size_t special_ie_len)
{
	struct sk_buff *skb = NULL;
	struct atbm_ieee80211_mgmt *mgmt = NULL;
	
	if(sdata->vif.type != NL80211_IFTYPE_AP){
		return false;
	}

	if(special_ie_len > 255){
		return false;
	}

#ifdef ATBM_PROBE_RESP_EXTRA_IE
	skb = ieee80211_proberesp_get(&sdata->local->hw,&sdata->vif);
#endif
	if(skb == NULL){
		return false;
	}

	if(special_ie && special_ie_len){
		/*
		*add special ie
		*/
		u8 *special = NULL;
		
		if(atbm_pskb_expand_head(skb,0,special_ie_len+2,GFP_ATOMIC)){
			return false;
		}

		
		special = skb->data + skb->len;

		*special++ = ATBM_WLAN_EID_PRIVATE;
		*special++ = special_ie_len;
		memcpy(special,special_ie,special_ie_len);

		atbm_skb_put(skb,special_ie_len+2);
	}

	mgmt = (struct atbm_ieee80211_mgmt *)skb->data;
	memcpy(mgmt->da,dst,6);
	
	ieee80211_tx_skb(sdata, skb);
	
	return true;
}
void ieee80211_ap_rx_queued_mgmt_special(struct ieee80211_sub_if_data *sdata,
				  struct sk_buff *skb)
{
#if 0
	  /*
	  *the follow code is a demo , add other by yourself
	  */
	  struct ieee80211_rx_status *rx_status;
	  struct atbm_ieee80211_mgmt *mgmt;
	  struct ieee802_atbm_11_elems elems;
	  int baselen;
	  rx_status = (struct ieee80211_rx_status *) skb->cb;
	  mgmt = (struct atbm_ieee80211_mgmt *)skb->data;

	  if (skb->len < 24)
		  return;
	  if(ieee80211_is_probe_req(mgmt->frame_control)){
	      u8 *elements = NULL;
		  const u8 *atbm_ie = NULL;
		  const u8 *special = NULL;
		  int special_len = 0;
		  int freq;
		  baselen = offsetof(struct atbm_ieee80211_mgmt, u.probe_req.variable);
		  if (baselen > skb->len){
			  atbm_printk_err("[probe req] error ! \n");
			  return;
		  }
		  elements = mgmt->u.probe_req.variable;
		  ieee802_11_parse_elems(elements, skb->len - baselen, &elems);
		  if (elems.ds_params && elems.ds_params_len == 1)
			  freq = ieee80211_channel_to_frequency(elems.ds_params[0],
								rx_status->band);
		  else
			  freq = rx_status->freq;
	  
		  //elements = mgmt->u.probe_req.variable;
		  atbm_ie = atbm_ieee80211_find_ie(ATBM_WLAN_EID_PRIVATE,elements,
					(int)(skb->len-offsetof(struct atbm_ieee80211_mgmt, u.probe_req.variable)));
  
		  if(atbm_ie){
			  char special_data[255] = {0};
			  special_len = atbm_ie[1];
			  special = &atbm_ie[2];
			  memcpy(special_data,special,special_len);
			  /*
			  *send probe response
			  */
			  atbm_printk_cfg("[probe req] from [%pM] channel[%d] special ie[%d][%d][%s]\n",mgmt->sa,freq,atbm_ie[0],atbm_ie[1],special_data);
			  memcpy(special_data,"RECV_PROBE_REQ",14);
			  special_data[14] = 0;
			  ieee80211_send_special_probe_response(sdata,mgmt->sa,special_data,14);
			  
		  }else{
			  atbm_printk_cfg("[probe req] from [%pM] channel[%d] \n",mgmt->sa,freq);
		  }
	  }else {
		  /*
		  *other frame
		  */	  
	  }
#endif

}
static bool ieee80211_update_special_ie(struct ieee80211_sub_if_data *sdata,enum ieee80211_special_work_type type,
												enum atbm_ieee80211_eid eid, const u8 *special_ie, size_t special_ie_len)
{
	bool res = true;
	struct sk_buff *skb;
	u8 *special = NULL;
	struct ieee80211_update_special *special_update;

	if((type < IEEE80211_SPECIAL_NONE_TYPE) ||(type>IEEE80211_SPECIAL_STA_SPECIAL_PROBR)){
		atbm_printk_err("%s _ %d \n",__func__,__LINE__);
		res = false;
		goto exit;
	}
	if((special_ie) && (special_ie_len > 0)){
		if((!!special_ie) ^ (!!special_ie_len)){
			atbm_printk_err("%s _ %d \n",__func__,__LINE__);
			res = false;
			goto exit;
		}

		skb = atbm_dev_alloc_skb(special_ie_len+2);

		if(skb == NULL){
			atbm_printk_err("%s _ %d \n",__func__,__LINE__);
			res = false;
			goto exit;
		}
		special = skb->data;
		special[0] = eid;
		special[1] = special_ie_len;

		memcpy(&special[2],special_ie,special_ie_len);
		atbm_skb_put(skb,special_ie_len+2);
		skb->pkt_type = type;
		special_update = (struct ieee80211_update_special*)skb->cb;

		special_update->req_sdata = sdata;
		special_update->special_ie = special;
		special_update->special_len = special_ie_len+2;
	}else{
		skb = atbm_dev_alloc_skb(special_ie_len+2);

		if(skb == NULL){
			atbm_printk_err("%s _ %d \n",__func__,__LINE__);
			res = false;
			goto exit;
		}
		special = skb->data;
		special[0] = eid;
		special[1] = 0;

	
		skb->pkt_type = type;
		special_update = (struct ieee80211_update_special*)skb->cb;

		special_update->req_sdata = sdata;
		special_update->special_ie = NULL;
		special_update->special_len = 0;
	}
	
	atbm_skb_queue_tail(&sdata->local->special_req_list, skb);
	atbm_schedule_work(&sdata->local->special_work);
	res = true;
	
exit:
	return res;
}
bool ieee80211_ap_update_special_beacon(struct ieee80211_sub_if_data *sdata,
		const u8 *special_ie, size_t special_ie_len)
{
	bool res = true;
	/*
	*only ap mode can update beacon
	*/
	if(sdata->vif.type != NL80211_IFTYPE_AP){
		res = false;
		goto exit;
	}
	/*
	*make sure that ,ap mode is running now
	*/
	
	if (!ieee80211_sdata_running(sdata)){
		res = false;
		goto exit;
	}

	res = ieee80211_update_special_ie(sdata,IEEE80211_SPECIAL_AP_SPECIAL_BEACON,ATBM_WLAN_EID_PRIVATE,special_ie,special_ie_len);
exit:
	return res;
}

bool ieee80211_ap_update_special_probe_response(struct ieee80211_sub_if_data *sdata,
		const u8 *special_ie, size_t special_ie_len)
{
	bool res = true;
	/*
	*only ap mode can update beacon
	*/
	if(sdata->vif.type != NL80211_IFTYPE_AP){
		res = false;
		goto exit;
	}
	/*
	*make sure that ,ap mode is running now
	*/
	
	if (!ieee80211_sdata_running(sdata)){
		res = false;
		goto exit;
	}

	res = ieee80211_update_special_ie(sdata,IEEE80211_SPECIAL_AP_SPECIAL_PROBRSP,ATBM_WLAN_EID_PRIVATE,special_ie,special_ie_len);
exit:
	return res;
}

bool ieee80211_ap_update_special_probe_request(struct ieee80211_sub_if_data *sdata,
		const u8 *special_ie, size_t special_ie_len)
{
	bool res = true;
	/*
	*only sta mode can update beacon
	*/
	if(sdata->vif.type != NL80211_IFTYPE_STATION){
		res = false;
		goto exit;
	}
	/*
	*make sure that ,sta mode is running now
	*/
	
	if (!ieee80211_sdata_running(sdata)){
		res = false;
		goto exit;
	}

	res = ieee80211_update_special_ie(sdata,IEEE80211_SPECIAL_STA_SPECIAL_PROBR,ATBM_WLAN_EID_PRIVATE,special_ie,special_ie_len);
exit:
	return res;
}

bool ieee80211_ap_update_vendor_probe_request(struct ieee80211_sub_if_data *sdata,
		const u8 *special_ie, size_t special_ie_len)
{
	bool res = true;
	/*
	*only sta mode can update beacon
	*/
	if(sdata->vif.type != NL80211_IFTYPE_STATION){
		res = false;
		goto exit;
	}
	/*
	*make sure that ,sta mode is running now
	*/
	
	if (!ieee80211_sdata_running(sdata)){
		res = false;
		goto exit;
	}

	res = ieee80211_update_special_ie(sdata,IEEE80211_SPECIAL_STA_SPECIAL_PROBR,ATBM_WLAN_EID_VENDOR_SPECIFIC,special_ie,special_ie_len);
exit:
	return res;
}


/*
*ieee80211_sta_triger_passive_scan - triger sta into passive scan mode
*
*@sdata       interface of the sta will be in sta mode
*@channels    channel list. if null ,will scan all of 2.4G channel
*@n_channels  number of channel in the channel list
*/
bool ieee80211_sta_triger_passive_scan(struct ieee80211_sub_if_data *sdata,
													u8 *channels,size_t n_channels)
{
	bool res = true;
	struct sk_buff *skb;
	struct ieee80211_special_work_scan *scan_req;
	size_t i;
	/*
	*make sure that ,sta mode is running now
	*/
	
	if (!ieee80211_sdata_running(sdata)){
		res = false;
		goto exit;
	}

	/*
	*only station mode can triger scan
	*/
	if(sdata->vif.type != NL80211_IFTYPE_STATION){
		res = false;
		goto exit;
	}

	if((!!channels) ^ (!!n_channels)){
		res = false;
		goto exit;
	}
	
	if(n_channels >= IEEE80211_ATBM_MAX_SCAN_CHANNEL_INDEX)
	{
		res = false;
		goto exit;
	}
	
	for(i = 0;i<n_channels;i++){
		if(ieee8011_channel_valid(&sdata->local->hw,channels[i]) == false){
			res = false;
			goto exit;
		}
	}
	
	skb = atbm_dev_alloc_skb(n_channels);

	if(skb == NULL){
		res = false;
		goto exit;
	}

	skb->pkt_type = IEEE80211_SPECIAL_STA_PASSICE_SCAN;
	scan_req = (struct ieee80211_special_work_scan*)skb->cb;
	memset(scan_req,0,sizeof(struct ieee80211_special_work_scan));
	scan_req->scan_sdata = sdata;
	if(n_channels&&channels){
		scan_req->channels = skb->data;
		memcpy(scan_req->channels,channels,n_channels);
		scan_req->n_channels = n_channels;
	}

	atbm_skb_queue_tail(&sdata->local->special_req_list, skb);
	atbm_schedule_work(&sdata->local->special_work);
	res = true;
exit:
	return res;
}
/*
*ieee80211_sta_triger_positive_scan - triger sta into positive scan mode
*
*@sdata       interface of the sta will be in scan mode
*@channels    channel list .if null ,will scan all of 2.4G channel
*@n_channels  number of channel in the channel list
*@ssid        ssid will be scanning
*@ssid_len    length of the ssid
*@ie          special ie will be inclued in probe request
*@ie_len      length of the ie
*/
bool ieee80211_sta_triger_positive_scan(struct ieee80211_sub_if_data *sdata,
													  u8 *channels,size_t n_channels,
													  u8 *ssid,size_t ssid_len,
													  u8 *ie,u16 ie_len,
													  u8 *bssid)
{
	bool res = true;
	struct sk_buff *skb;
	struct ieee80211_special_work_scan *scan_req;
	size_t len = 0;
	size_t i = 0;
	void *pos;
	void *pos_end;
	/*
	*make sure that ,sta mode is running now
	*/
	
	if (!ieee80211_sdata_running(sdata)){
		res = false;
		goto exit;
	}

	/*
	*only station mode can triger scan
	*/
	if(sdata->vif.type != NL80211_IFTYPE_STATION){
		res = false;
		goto exit;
	}

	if((!!channels) ^ (!!n_channels)){
		res = false;
		goto exit;
	}
	
	if(n_channels >= IEEE80211_ATBM_MAX_SCAN_CHANNEL_INDEX)
	{
		res = false;
		goto exit;
	}
	
	for(i = 0;i<n_channels;i++){
		if(ieee8011_channel_valid(&sdata->local->hw,channels[i]) == false){
			res = false;
			goto exit;
		}
	}
	
	if((!!ssid) ^ (!!ssid_len)){
		res = false;
		goto exit;
	}
	
	if(ssid_len > IEEE80211_MAX_SSID_LEN){
		res = false;
		goto exit;
	}
	
	if((!!ie) ^ (!!ie_len)){
		res = false;
		goto exit;
	}
	
	if(ie_len>257){
		res = false;
		goto exit;
	}
	
	len = n_channels+ie_len;

	if(bssid)
		len += 6;
	
	if(ssid && ssid_len)
		len += sizeof(struct cfg80211_ssid);
	
	skb = atbm_dev_alloc_skb(len);

	if(skb == NULL){
		res = false;
		goto exit;
	}

	skb->pkt_type = IEEE80211_SPECIAL_STA_POSITIVE_SCAN;
	scan_req = (struct ieee80211_special_work_scan*)skb->cb;
	memset(scan_req,0,sizeof(struct ieee80211_special_work_scan));
	scan_req->scan_sdata = sdata;

	pos = (void*)skb->data;
	pos_end = (void*)(skb->data+len);
	/*
	*ssid
	*/
	if(ssid){
		scan_req->ssid = pos;
		
	//	BUG_ON(scan_req->ssid == NULL);
		
		if(scan_req->ssid == NULL){
			atbm_printk_err("%s %d ,ERROR !!! scan_req->ssid is NULL\n",__func__,__LINE__);
			
		}else{
			scan_req->ssid->ssid_len = ssid_len;
			memcpy(scan_req->ssid->ssid,ssid,ssid_len);
			
			pos = (void*)(scan_req->ssid + 1);
		}
	}
	/*
	*channel
	*/
	if(channels){
		
		scan_req->channels = pos;
		scan_req->n_channels = n_channels;
		memcpy(scan_req->channels,channels,n_channels);

		pos = (void*)(scan_req->channels + n_channels);
	}
	/*
	*ie
	*/
	if(ie){
		scan_req->ie = pos;
		scan_req->ie_len = ie_len;
		memcpy(scan_req->ie,ie,ie_len);
		pos = (void*)(scan_req->ie+ie_len);
	}

	if(bssid){
		scan_req->bssid = pos;
		memcpy(scan_req->bssid,bssid,6);
	}
	
	WARN_ON(pos != pos_end);
	atbm_skb_queue_tail(&sdata->local->special_req_list, skb);
	atbm_schedule_work(&sdata->local->special_work);
	res = true;
exit:	
	return res;
}


extern int atbm_internal_recv_6441_vendor_ie(struct atbm_vendor_cfg_ie *recv_ie);
#define ETH_P_CUSTOM 0x88cc
#define ETH_P_EAPOL 0x888E

int ieee80211_send_L2_2_hight_layer(struct ieee80211_sub_if_data *sdata,
												struct sk_buff *skb,struct net_device *dev);
int ieee80211_send_mgmt_to_wpa_supplicant(struct ieee80211_sub_if_data *sdata,
												struct sk_buff *skb);

int ieee80211_add_8023_header(struct sk_buff *skb, const char *addr,
			     enum nl80211_iftype iftype)
{
	struct ieee80211_hdr *hdr = (struct ieee80211_hdr *)skb->data;
	unsigned short hdrlen;
	struct ethhdr *ehdr;
	unsigned short len;
	unsigned char dst[6];
	unsigned char src[6];
	char *da,*sa,*bssid;

	
	hdrlen = ieee80211_hdrlen(hdr->frame_control);
	
	if(iftype == NL80211_IFTYPE_AP || iftype == NL80211_IFTYPE_P2P_GO){
		bssid = hdr->addr1;
		sa = hdr->addr2;
		da = hdr->addr3;
	}else{
		da = hdr->addr1;
		bssid = hdr->addr2;
		sa = hdr->addr3;
		
	}
	memcpy(dst, da, 6);
	memcpy(src, sa, 6);
	
	//atbm_skb_pull(skb, hdrlen);
	//len = htons(skb->len);
	len = sizeof(struct ethhdr);
	ehdr = (struct ethhdr *) atbm_skb_push(skb,len);
	memcpy(ehdr->h_dest, addr, 6);
	memcpy(ehdr->h_source, src, 6);
	ehdr->h_proto = htons(ETH_P_CUSTOM);
	return 0;
}

int ieee80211_add_simple_ratp_header(struct ieee80211_sub_if_data *sdata,struct sk_buff *skb)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_rate *rate = NULL;
	struct ieee80211_supported_band *sband;
	struct ieee80211_rx_status *status = IEEE80211_SKB_RXCB(skb);
//	struct ieee80211_hw *hw = &local->hw;

	sband = local->hw.wiphy->bands[status->band];
	if (WARN_ON(!sband))
		goto drop;

	rate = &sband->bitrates[status->rate_idx];
	status->rx_flags = 0;


	
	
drop:
	return -1;
}

int ieee80211_send_mgmt_to_wpa_supplicant(struct ieee80211_sub_if_data *sdata,
												struct sk_buff *skb)
{
	struct ieee80211_rx_status *status = IEEE80211_SKB_RXCB(skb);

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,4,0))  && !defined(COMPAT_KERNEL_RELEASE)
		if (cfg80211_rx_mgmt(sdata->dev, status->freq,
					 skb->data, skb->len,
					 GFP_ATOMIC) == 0) 
#elif (LINUX_VERSION_CODE < KERNEL_VERSION(3,6,0))
		if (cfg80211_rx_mgmt(sdata->dev, status->freq,
					 -90,skb->data, skb->len,
					 GFP_ATOMIC) == 0) 
#elif  (LINUX_VERSION_CODE < KERNEL_VERSION(3,11,0))
		if (cfg80211_rx_mgmt(&sdata->wdev, status->freq,
					 -90,skb->data, skb->len,
					 GFP_ATOMIC) == 0)
	//#elif  (LINUX_VERSION_CODE < KERNEL_VERSION(3,18,25))
#elif  (LINUX_VERSION_CODE < KERNEL_VERSION(3,18,00))
		if (cfg80211_rx_mgmt(&sdata->wdev, status->freq,
					 -90,skb->data, skb->len,0,
					 GFP_ATOMIC) == 0)
#else
		if (cfg80211_rx_mgmt(&sdata->wdev, status->freq,
					 -90,skb->data, skb->len,0) == 0)
#endif
		{
			return 0;
		}
	
	return -1;
}

int ieee80211_send_L2_2_hight_layer(struct ieee80211_sub_if_data *sdata,
												struct sk_buff *skb,struct net_device *dev)
{
	int res = -1;
	struct sk_buff *xmit_skb = NULL;
	xmit_skb = atbm_skb_clone(skb, GFP_ATOMIC);
	if (!xmit_skb && net_ratelimit())
		atbm_printk_err( "ieee80211_send_L2_2_hight_layer: failed to clone "
		       "multicast frame\n");
	res = ieee80211_add_8023_header(xmit_skb, sdata->vif.addr, sdata->vif.type);
	if (res < 0){
		atbm_printk_err("ieee80211_data_to_8023 faile!! \n");
		atbm_dev_kfree_skb(xmit_skb);
		return res;
	}	   
	if (xmit_skb && dev) {		
		xmit_skb->dev = dev;	
		eth_type_trans(xmit_skb, dev);
	//	atbm_printk_err("[atbm_netif_receive_skb] protocol ==> %x \n ",eth_type_trans(xmit_skb, dev));
		xmit_skb->ip_summed = CHECKSUM_UNNECESSARY;
		xmit_skb->protocol = htons(ETH_P_CUSTOM);
		//xmit_skb->pkt_type = PACKET_OTHERHOST;
		memset(xmit_skb->cb, 0, sizeof(xmit_skb->cb));
		if(xmit_skb->len > 0){
			res = atbm_netif_receive_skb(xmit_skb);
			if(res < 0){
				atbm_printk_err("[error] ieee80211_send_L2_2_hight_layer err! \n ");
				atbm_dev_kfree_skb(xmit_skb);
			}
		}else{
			atbm_printk_err("skb->len = 0 \n");
		}
	}else{
		atbm_printk_err("skb=%p dev=%p \n",xmit_skb,dev);
	}
	return res;
}


#ifdef CONFIG_ATBM_STA_LISTEN

static  const u8 *atbm_ieee80211_find_vendor_cfg_ie(u8 eid, const u8 *ies, int len)
{
	if(len < 2){
		return NULL;
	}
	while (1) {
		len -= ies[1] + 2;
		ies += ies[1] + 2;
		if(len < 2){
			break;
		}
		if(ies[0] == 221 && ies[1] > 3 && ies[2] == 0x41 && ies[3] == 0x54 && ies[4] == 0x42){
			break;
		}
	}
	
	if (len < 2)
		return NULL;
	if (len < 2 + ies[1])
		return NULL;
	return ies;
}


void ieee80211_sta_rx_queued_mgmt_special(struct ieee80211_sub_if_data *sdata,
				  struct sk_buff *skb)
{
	#if 1
	struct atbm_ieee80211_mgmt *mgmt = (struct atbm_ieee80211_mgmt *)skb->data;
	u8 *elements;
	int baselen;
	struct ieee802_atbm_11_elems elems;
	struct ieee80211_rx_status *rx_status = IEEE80211_SKB_RXCB(skb);
	int freq;
	char ssid[32]={0};
	u8 *ie = NULL;
	struct atbm_vendor_cfg_ie *private_ie;
	u8 OUI[4];
	/*
	*the follow code is a demo , add other by yourself
	*/
	
	if (skb->len < 24){
		atbm_printk_err("ieee80211_sta_rx_queued_mgmt_special:skb->len < 24 \n");
		return;
	}

	if (ieee80211_is_probe_resp(mgmt->frame_control)) {

		//atbm_printk_err("recv probe resp! \n");

		private_ie = (struct atbm_vendor_cfg_ie *)atbm_ieee80211_find_vendor_cfg_ie(221,mgmt->u.probe_resp.variable,
				                   skb->len-offsetof(struct atbm_ieee80211_mgmt, u.probe_resp.variable));
		
		if(private_ie){
			
		//	atbm_printk_err("[%s] recv from ssid[%s],psk[%s] sa[%pM],da[%pM],priv_bssid[%pM] \n",
		//			sdata->name,private_ie->ssid,private_ie->password,mgmt->sa,mgmt->da,sdata->vif.addr);
			
			if(memcmp(mgmt->da,sdata->vif.addr,6) != 0){
				memcpy(mgmt->da,sdata->vif.addr,6);
			}
			atbm_internal_recv_6441_vendor_ie(private_ie);
			/* send data to up layer*/
			ieee80211_send_L2_2_hight_layer(sdata,skb,sdata->dev);
			
			
		}


	} else if(ieee80211_is_beacon(mgmt->frame_control)){
#if 1
			baselen = offsetof(struct atbm_ieee80211_mgmt, u.beacon.variable);
		if (baselen > skb->len){
			atbm_printk_cfg("[beacon] error ! \n");
			return;
		}
		elements = mgmt->u.beacon.variable;
		ieee802_11_parse_elems(elements, skb->len - baselen, &elems);
		if (elems.ds_params && elems.ds_params_len == 1)
			freq = ieee80211_channel_to_frequency(elems.ds_params[0],
						      rx_status->band);
		else
			freq = rx_status->freq;

		freq = (freq - 2412)/5 + 1;
		memcpy(ssid,elems.ssid,elems.ssid_len);
		ie = (u8 *)atbm_ieee80211_find_ie(ATBM_WLAN_EID_PRIVATE,mgmt->u.beacon.variable,
				                   skb->len-offsetof(struct atbm_ieee80211_mgmt, u.beacon.variable));			
		if(ie){
			u8 special_data[255]={0};
			ie[2+ie[1]] = 0;
			atbm_printk_cfg("==========>>>[beacon] from [%pM] channel[%d] ssid[%s] ie[%d][%d][%s]\n",mgmt->bssid,freq,ssid,ie[0],ie[1],ie+2);
			
			memcpy(special_data,"RECV_BEACON",11);
			//ieee80211_send_special_probe_req(sdata, NULL, NULL,0, special_data, 11);
			
			//atbm_printk_err("recv_beacon ####################### \n");
			/*
			if(!sned_one){
			special_data[0] = ATBM_WLAN_EID_PRIVATE;
			special_data[1] = 11;
			memcpy(special_data + 2,"RECV_BEACON",11);
			ieee80211_sta_triger_positive_scan(sdata,&freq,1,NULL,0,&special_data[0],special_data[1] + 2);
				sned_one=true;
			}
			*/
			
		}
		else{
			atbm_printk_cfg("[beacon] from [%pM] channel[%d] ssid[%s] \n",mgmt->bssid,freq,ssid);
			return ;
		}
#endif
	}else if(ieee80211_is_action(mgmt->frame_control)) {
		//atbm_printk_err("[action] from [%pM]  \n",mgmt->bssid);
	}else if(ieee80211_is_probe_req(mgmt->frame_control)){
		//atbm_printk_err("[probe req] from [%pM]  \n",mgmt->bssid);
	//	struct atbm_vendor_cfg_ie *private_ie;

	
		
		
		private_ie = (struct atbm_vendor_cfg_ie *)atbm_ieee80211_find_ie(221,mgmt->u.probe_req.variable,
				                   skb->len-offsetof(struct atbm_ieee80211_mgmt, u.probe_req.variable));
		
		if(private_ie){
			OUI[0] = (ATBM_6441_PRIVATE_OUI >> 24) & 0xFF;
			OUI[1] = (ATBM_6441_PRIVATE_OUI >> 16) & 0xFF;
			OUI[2] = (ATBM_6441_PRIVATE_OUI >> 8) & 0xFF;
			OUI[3] = ATBM_6441_PRIVATE_OUI & 0xFF;
			if(memcmp(private_ie->OUI,OUI,4) == 0){
				atbm_printk_err("[%s] recv from ssid[%s],psk[%s] sa[%pM],da[%pM],priv_bssid[%pM] \n",
						sdata->name,private_ie->ssid,private_ie->password,mgmt->sa,mgmt->da,sdata->vif.addr);
				
				if(memcmp(mgmt->da,sdata->vif.addr,6) != 0){
					memcpy(mgmt->da,sdata->vif.addr,6);
				}
				atbm_internal_recv_6441_vendor_ie(private_ie);
				/* send data to up layer*/
				ieee80211_send_L2_2_hight_layer(sdata,skb,sdata->dev);
				
			}
		}
	}else{
		//atbm_printk_err("frame_control[0x%x] from [%pM]  \n",mgmt->frame_control,mgmt->bssid);
	}

#endif

}
#endif
u32 ieee80211_sta_get_rates(struct ieee80211_local *local,
			    struct ieee802_atbm_11_elems *elems,
			    enum ieee80211_band band)
{
	struct ieee80211_supported_band *sband;
	struct ieee80211_rate *bitrates;
	size_t num_rates;
	u32 supp_rates;
	int i, j;
	sband = local->hw.wiphy->bands[band];

	if (WARN_ON(!sband))
		return 1;

	bitrates = sband->bitrates;
	num_rates = sband->n_bitrates;
	supp_rates = 0;
	for (i = 0; i < elems->supp_rates_len +
		     elems->ext_supp_rates_len; i++) {
		u8 rate = 0;
		int own_rate;
		if (i < elems->supp_rates_len)
			rate = elems->supp_rates[i];
		else if (elems->ext_supp_rates)
			rate = elems->ext_supp_rates
				[i - elems->supp_rates_len];
		own_rate = 5 * (rate & 0x7f);
		for (j = 0; j < num_rates; j++)
			if (bitrates[j].bitrate == own_rate)
				supp_rates |= BIT(j);
	}
	return supp_rates;
}

void ieee80211_stop_device(struct ieee80211_local *local)
{
	ieee80211_led_radio(local, false);
	ieee80211_mod_tpt_led_trig(local, 0, IEEE80211_TPT_LEDTRIG_FL_RADIO);
	atbm_flush_workqueue(local->workqueue);
	drv_stop(local);
}
#if defined (CONFIG_PM)
int ieee80211_reconfig(struct ieee80211_local *local)
{
	struct ieee80211_hw *hw = &local->hw;
#if defined (ATBM_SUSPEND_REMOVE_INTERFACE) || defined (ATBM_SUPPORT_WOW)
	struct sta_info *sta;
	int res;
#endif
#if defined (CONFIG_PM) ||defined (ATBM_SUSPEND_REMOVE_INTERFACE) || defined (ATBM_SUPPORT_WOW)
	struct ieee80211_sub_if_data *sdata;
#endif
#ifdef ATBM_SUSPEND_REMOVE_INTERFACE
	int i;
#endif

#ifdef CONFIG_PM
	if (local->suspended)
		local->resuming = true;
	atbm_printk_pm("%s:wowlan(%d)\n",__func__,local->wowlan);
#ifdef ATBM_SUPPORT_WOW
	if (local->wowlan) {
		bool suspended = local->suspended;
		local->wowlan = false;
		/*
		* When the driver is resumed, first data is dropped
		* by MAC layer. To eliminate it, the local->suspended
		* should be clear.
		*/
		local->suspended = false;
		res = drv_resume(local);
		if (res < 0) {
			local->suspended = suspended;
			local->resuming = false;
			return res;
		}
		if (res == 0)
			goto wake_up;
		WARN_ON(res > 1);
		/*
		 * res is 1, which means the driver requested
		 * to go through a regular reset on wakeup.
		 */
	}
#endif
#endif
#ifdef ATBM_SUSPEND_REMOVE_INTERFACE
	/* setup fragmentation threshold */
	drv_set_frag_threshold(local, hw->wiphy->frag_threshold);

	/* reset coverage class */
	drv_set_coverage_class(local, hw->wiphy->coverage_class);

	/* everything else happens only if HW was up & running */
	if (!local->open_count)
		goto wake_up;

	/*
	 * Upon resume hardware can sometimes be goofy due to
	 * various platform / driver / bus issues, so restarting
	 * the device may at times not work immediately. Propagate
	 * the error.
	 */
	res = drv_start(local);
	if (res) {
		WARN(local->suspended, "Hardware became unavailable "
		     "upon resume. This could be a software issue "
		     "prior to suspend or a hardware issue.\n");
		return res;
	}

	ieee80211_led_radio(local, true);
	ieee80211_mod_tpt_led_trig(local,
				   IEEE80211_TPT_LEDTRIG_FL_RADIO, 0);

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
			changed |= BSS_CHANGED_ASSOC;
			mutex_lock(&sdata->u.mgd.mtx);
			if(sdata->vif.bss_conf.assoc){
				changed |= (BSS_CHANGED_STA_RESTART|BSS_CHANGED_ASSOC);
				if (sdata->vif.bss_conf.arp_filter_enabled) {
					changed |= BSS_CHANGED_ARP_FILTER;
				}
			}
			ieee80211_bss_info_change_notify(sdata, changed);
			mutex_unlock(&sdata->u.mgd.mtx);
			break;
#ifdef CONFIG_ATBM_SUPPORT_IBSS
		case NL80211_IFTYPE_ADHOC:
			changed |= BSS_CHANGED_IBSS;
#endif
			/* fall through */
		case NL80211_IFTYPE_AP:
			changed |= BSS_CHANGED_SSID;
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

			WARN_ON(drv_sta_add(local, sdata, &sta->sta));
		}
	}
	mutex_unlock(&local->sta_mtx);
	/*
	 * Clear the WLAN_STA_BLOCK_BA flag so new aggregation
	 * sessions can be established after a resume.
	 *
	 * Also tear down aggregation sessions since reconfiguring
	 * them in a hardware restart scenario is not easily done
	 * right now, and the hardware will have lost information
	 * about the sessions, but we and the AP still think they
	 * are active. This is really a workaround though.
	 */
	if (hw->flags & IEEE80211_HW_AMPDU_AGGREGATION) {
		mutex_lock(&local->sta_mtx);

		list_for_each_entry(sta, &local->sta_list, list) {
			ieee80211_sta_tear_down_BA_sessions(sta, true);
			clear_sta_flag(sta, WLAN_STA_BLOCK_BA);
		}

		mutex_unlock(&local->sta_mtx);
	}

	/* add back keys */
	list_for_each_entry(sdata, &local->interfaces, list)
		if (ieee80211_sdata_running(sdata))
			ieee80211_enable_keys(sdata);


	/* setup RTS threshold */
	list_for_each_entry(sdata, &local->interfaces, list)
	    drv_set_rts_threshold(local, sdata, sdata->vif.bss_conf.rts_threshold);
#endif
#if defined (ATBM_SUSPEND_REMOVE_INTERFACE) || defined (ATBM_SUPPORT_WOW)
 wake_up:
#endif
	ieee80211_wake_queues_by_reason(hw,
			IEEE80211_QUEUE_STOP_REASON_SUSPEND);

	/*
	 * If this is for hw restart things are still running.
	 * We may want to change that later, however.
	 */
	if (!local->suspended)
		return 0;

#ifdef CONFIG_PM
	/* first set suspended false, then resuming */	
	atbm_printk_pm("%s:resume_timer_start\n",__func__);
	atomic_set(&local->resume_timer_start,1);
	atbm_mod_timer(&local->resume_timer, round_jiffies(jiffies + 2*HZ));
	local->suspended = false;
	mb();
	local->resuming = false;

	list_for_each_entry(sdata, &local->interfaces, list) {
		switch(sdata->vif.type) {
		case NL80211_IFTYPE_STATION:
			ieee80211_sta_restart(sdata);
			break;
#ifdef CONFIG_ATBM_SUPPORT_IBSS
		case NL80211_IFTYPE_ADHOC:
			ieee80211_ibss_restart(sdata);
			break;
#endif
#ifdef CONFIG_MAC80211_ATBM_MESH
		case NL80211_IFTYPE_MESH_POINT:
			ieee80211_mesh_restart(sdata);
#endif
			break;
		default:
			break;
		}
	}

	atbm_mod_timer(&local->sta_cleanup, jiffies + 1);
#ifdef CONFIG_MAC80211_ATBM_MESH
	mutex_lock(&local->sta_mtx);
	list_for_each_entry(sta, &local->sta_list, list)
		mesh_plink_restart(sta);
	mutex_unlock(&local->sta_mtx);
#endif
#else
	WARN_ON(1);
#endif
	return 0;
}
#endif
#ifdef CONFIG_ATBM_MAC80211_NO_USE
void ieee80211_resume_disconnect(struct ieee80211_vif *vif)
{
	struct ieee80211_sub_if_data *sdata;
	struct ieee80211_local *local;
	struct ieee80211_key *key;

	if (WARN_ON(!vif))
		return;

	sdata = vif_to_sdata(vif);
	local = sdata->local;

	if (WARN_ON(!local->resuming))
		return;

	if (WARN_ON(vif->type != NL80211_IFTYPE_STATION))
		return;

	sdata->flags |= IEEE80211_SDATA_DISCONNECT_RESUME;

	mutex_lock(&local->key_mtx);
	list_for_each_entry(key, &sdata->key_list, list)
		key->flags |= KEY_FLAG_TAINTED;
	mutex_unlock(&local->key_mtx);
}
#endif
//EXPORT_SYMBOL_GPL(ieee80211_resume_disconnect);
#ifdef CONFIG_ATBM_SMPS
static int check_mgd_smps(struct ieee80211_if_managed *ifmgd,
			  enum ieee80211_smps_mode *smps_mode)
{
	if (ifmgd->associated) {
		*smps_mode = ifmgd->ap_smps;

		if (*smps_mode == IEEE80211_SMPS_AUTOMATIC) {
			if (ifmgd->powersave)
				*smps_mode = IEEE80211_SMPS_DYNAMIC;
			else
				*smps_mode = IEEE80211_SMPS_OFF;
		}

		return 1;
	}

	return 0;
}

/* must hold iflist_mtx */
void ieee80211_recalc_smps(struct ieee80211_local *local)
{
	struct ieee80211_sub_if_data *sdata;
	enum ieee80211_smps_mode smps_mode = IEEE80211_SMPS_OFF;
	int count = 0;

	lockdep_assert_held(&local->iflist_mtx);

	/*
	 * This function could be improved to handle multiple
	 * interfaces better, but right now it makes any
	 * non-station interfaces force SM PS to be turned
	 * off. If there are multiple station interfaces it
	 * could also use the best possible mode, e.g. if
	 * one is in static and the other in dynamic then
	 * dynamic is ok.
	 */

	list_for_each_entry(sdata, &local->interfaces, list) {
		if (!ieee80211_sdata_running(sdata))
			continue;
		if (sdata->vif.type != NL80211_IFTYPE_STATION)
			goto set;

		count += check_mgd_smps(&sdata->u.mgd, &smps_mode);

		if (count > 1) {
			smps_mode = IEEE80211_SMPS_OFF;
			break;
		}
	}

	if (smps_mode == local->smps_mode)
		return;

 set:
	local->smps_mode = smps_mode;
	/* changed flag is auto-detected for this */
	ieee80211_hw_config(local, 0);
}
#endif
static bool atbm_ieee80211_id_in_list(const u8 *ids, int n_ids, u8 id)
{
	int i;

	for (i = 0; i < n_ids; i++)
		if (ids[i] == id)
			return true;
	return false;
}

/**
 * atbm_ieee80211_ie_split - split an IE buffer according to ordering
 *
 * @ies: the IE buffer
 * @ielen: the length of the IE buffer
 * @ids: an array with element IDs that are allowed before
 *	the split
 * @n_ids: the size of the element ID array
 * @offset: offset where to start splitting in the buffer
 *
 * This function splits an IE buffer by updating the @offset
 * variable to point to the location where the buffer should be
 * split.
 *
 * It assumes that the given IE buffer is well-formed, this
 * has to be guaranteed by the caller!
 *
 * It also assumes that the IEs in the buffer are ordered
 * correctly, if not the result of using this function will not
 * be ordered correctly either, i.e. it does no reordering.
 *
 * The function returns the offset where the next part of the
 * buffer starts, which may be @ielen if the entire (remainder)
 * of the buffer should be used.
 */
size_t atbm_ieee80211_ie_split(const u8 *ies, size_t ielen,
			  const u8 *ids, int n_ids, size_t offset)
{
	size_t pos = offset;

	while (pos < ielen && atbm_ieee80211_id_in_list(ids, n_ids, ies[pos]))
		pos += 2 + ies[pos + 1];

	return pos;
}

size_t ieee80211_ie_split_vendor(const u8 *ies, size_t ielen, size_t offset)
{
	size_t pos = offset;

	while (pos < ielen && ies[pos] != ATBM_WLAN_EID_VENDOR_SPECIFIC)
		pos += 2 + ies[pos + 1];

	return pos;
}
#ifdef CONFIG_ATBM_MAC80211_NO_USE
static void _ieee80211_enable_rssi_reports(struct ieee80211_sub_if_data *sdata,
					    int rssi_min_thold,
					    int rssi_max_thold)
{
	trace_api_enable_rssi_reports(sdata, rssi_min_thold, rssi_max_thold);

	if (WARN_ON(sdata->vif.type != NL80211_IFTYPE_STATION))
		return;

	/*
	 * Scale up threshold values before storing it, as the RSSI averaging
	 * algorithm uses a scaled up value as well. Change this scaling
	 * factor if the RSSI averaging algorithm changes.
	 */
	sdata->u.mgd.rssi_min_thold = rssi_min_thold*16;
	sdata->u.mgd.rssi_max_thold = rssi_max_thold*16;
}

void ieee80211_enable_rssi_reports(struct ieee80211_vif *vif,
				    int rssi_min_thold,
				    int rssi_max_thold)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);

	WARN_ON(rssi_min_thold == rssi_max_thold ||
		rssi_min_thold > rssi_max_thold);

	_ieee80211_enable_rssi_reports(sdata, rssi_min_thold,
				       rssi_max_thold);
}
//EXPORT_SYMBOL(ieee80211_enable_rssi_reports);

void ieee80211_disable_rssi_reports(struct ieee80211_vif *vif)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);

	_ieee80211_enable_rssi_reports(sdata, 0, 0);
}
//EXPORT_SYMBOL(ieee80211_disable_rssi_reports);
#endif
#if defined(CONFIG_MAC80211_ATBM_MESH) || defined(ATBM_SURPORT_TDLS)

int ieee80211_add_srates_ie(struct ieee80211_vif *vif, struct sk_buff *skb)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(local, sdata);
	struct ieee80211_supported_band *sband;
	int rate;
	u8 i, rates, *pos;

	sband = local->hw.wiphy->bands[chan_state->conf.channel->band];
	rates = sband->n_bitrates;
	if (rates > 8)
		rates = 8;

	if (atbm_skb_tailroom(skb) < rates + 2)
		return -ENOMEM;

	pos = atbm_skb_put(skb, rates + 2);
	*pos++ = ATBM_WLAN_EID_SUPP_RATES;
	*pos++ = rates;
	for (i = 0; i < rates; i++) {
		rate = sband->bitrates[i].bitrate;
		*pos++ = (u8) (rate / 5);
	}

	return 0;
}

int ieee80211_add_ext_srates_ie(struct ieee80211_vif *vif, struct sk_buff *skb)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(local, sdata);
	struct ieee80211_supported_band *sband;
	int rate;
	u8 i, exrates, *pos;

	sband = local->hw.wiphy->bands[chan_state->conf.channel->band];
	exrates = sband->n_bitrates;
	if (exrates > 8)
		exrates -= 8;
	else
		exrates = 0;

	if (atbm_skb_tailroom(skb) < exrates + 2)
		return -ENOMEM;

	if (exrates) {
		pos = atbm_skb_put(skb, exrates + 2);
		*pos++ = ATBM_WLAN_EID_EXT_SUPP_RATES;
		*pos++ = exrates;
		for (i = 8; i < sband->n_bitrates; i++) {
			rate = sband->bitrates[i].bitrate;
			*pos++ = (u8) (rate / 5);
		}
	}
	return 0;
}
#endif
struct cfg80211_bss *ieee80211_atbm_get_bss(struct wiphy *wiphy,
				      struct ieee80211_channel *channel,
				      const u8 *bssid,
				      const u8 *ssid, size_t ssid_len,
				      u16 capa_mask, u16 capa_val)
{
	#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 4, 0))
	return  cfg80211_get_bss(wiphy, channel,
			bssid, ssid, ssid_len, capa_mask, capa_val);
	#else
	/*
	enum ieee80211_bss_type {
	IEEE80211_BSS_TYPE_ESS,
	IEEE80211_BSS_TYPE_PBSS,
	IEEE80211_BSS_TYPE_IBSS,
	IEEE80211_BSS_TYPE_MBSS,
	IEEE80211_BSS_TYPE_ANY
	};
	enum ieee80211_privacy {
	IEEE80211_PRIVACY_ON,
	IEEE80211_PRIVACY_OFF,
	IEEE80211_PRIVACY_ANY
	};
	*/
	enum ieee80211_bss_type bss_type = IEEE80211_BSS_TYPE_ANY;
	enum ieee80211_privacy  privacy = IEEE80211_PRIVACY_ANY;

	if(capa_mask&WLAN_CAPABILITY_ESS)
		bss_type = IEEE80211_BSS_TYPE_ESS;
	else if(capa_mask&WLAN_CAPABILITY_IBSS)
		bss_type = IEEE80211_BSS_TYPE_IBSS;
	else
		bss_type = IEEE80211_BSS_TYPE_ANY;

	if(capa_mask&WLAN_CAPABILITY_PRIVACY)
		privacy = IEEE80211_PRIVACY_ON;
	else
		privacy = IEEE80211_PRIVACY_ANY;

	return cfg80211_get_bss(wiphy,channel,bssid,ssid,ssid_len,bss_type,privacy);
	#endif
}

void ieee80211_atbm_put_bss(struct wiphy *wiphy, struct cfg80211_bss *pub)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)
	cfg80211_put_bss(wiphy, pub);
#else
//	BUG_ON(wiphy == NULL);
	if(wiphy == NULL){
		atbm_printk_err("%s %d ,ERROR !!! wiphy is NULL\n",__func__,__LINE__);
		return;
	}
	cfg80211_put_bss(pub);
#endif
}
int ieee80211_atbm_ref_bss(struct wiphy *wiphy, struct cfg80211_bss *pub)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)
	cfg80211_ref_bss(wiphy,pub);
	return 0;
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
	cfg80211_ref_bss(pub);
	return 0;
#else
	/*
	*get bss again for ourself.
	*/
	const u8 *ssid = NULL;
	const u8 *bss_ssid = NULL;
	size_t ssid_len = 0;
	struct cfg80211_bss *bss;

	ssid = ieee80211_bss_get_ie(pub, ATBM_WLAN_EID_SSID);

	if(ssid){
		ssid_len = ssid[1];
		bss_ssid = ssid+2;
	}

	bss = ieee80211_atbm_get_bss(wiphy,pub->channel,pub->bssid,
								bss_ssid,ssid_len,pub->capability,
								pub->capability);

	return 	bss != pub;
#endif
}
int ieee80211_atbm_handle_bss(struct wiphy *wiphy, struct cfg80211_bss *pub)
{
	return ieee80211_atbm_ref_bss(wiphy,pub);
}

void ieee80211_atbm_release_bss(struct wiphy *wiphy, struct cfg80211_bss *pub)
{
	ieee80211_atbm_put_bss(wiphy,pub);
}

struct cfg80211_bss *__ieee80211_atbm_get_authen_bss(struct ieee80211_vif *vif,
					  struct ieee80211_channel *channel,
				      const u8 *bssid,
				      const u8 *ssid, size_t ssid_len)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct cfg80211_bss *bss = NULL;
	struct cfg80211_bss *authening_bss = NULL;
	
	rcu_read_lock();
	authening_bss = rcu_dereference(ifmgd->authen_bss);
	while(authening_bss != NULL){
		
		if(channel != authening_bss->channel){
			break;
		}
		
		if(bssid&&(atbm_compare_ether_addr(bssid, authening_bss->bssid)!=0)){
			break;
		}
		
		if(ssid&&ssid_len){
			const u8* bss_ssid;
			
			bss_ssid = ieee80211_bss_get_ie(authening_bss, ATBM_WLAN_EID_SSID);

			if((bss_ssid == NULL) || (bss_ssid[1] != ssid_len)){
				break;
			}

			if(memcmp(ssid,bss_ssid+2,ssid_len) != 0){
				break;
			}
		}
		if(ieee80211_atbm_handle_bss(local->hw.wiphy,authening_bss) == 0){
			bss = authening_bss;
		}else {			
			atbm_printk_err("%s:bss get err\n",__func__);
		}
		break;
	}
	rcu_read_unlock();

	return bss;
}

void __ieee80211_atbm_put_authen_bss(struct ieee80211_vif *vif,struct cfg80211_bss *bss)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);
	struct ieee80211_local *local = sdata->local;
	/*
	*ifmgd->authen_bss may have be released
	*/
	ieee80211_atbm_release_bss(local->hw.wiphy,bss);
}
struct cfg80211_bss *ieee80211_atbm_get_authen_bss(struct ieee80211_vif *vif,
					  struct ieee80211_channel *channel,
				      const u8 *bssid,
				      const u8 *ssid, size_t ssid_len){
	struct cfg80211_bss *bss = NULL;
	
	bss = __ieee80211_atbm_get_authen_bss(vif,channel,bssid,ssid,ssid_len);

	return bss;
}
void ieee80211_atbm_put_authen_bss(struct ieee80211_vif *vif,struct cfg80211_bss *bss)
{
	__ieee80211_atbm_put_authen_bss(vif,bss);
}
static char* ieee8211_find_name(struct ieee80211_hw *hw,const char *name)
{
	/*
	*phy0-name;
	*/
	struct ieee80211_local *local = hw_to_local(hw);
	int phy_namelen = strlen(wiphy_name(local->hw.wiphy))+1;
	int new_namelen = phy_namelen+strlen(name)+1;
	struct ieee80211_name_def *def = NULL;
	struct ieee80211_name_def *def_next = NULL;
	struct ieee80211_name_def *new_def = NULL;
	char *find_name = NULL;
	spin_lock_bh(&local->ieee80211_name_lock);
	
	
	if(new_namelen > 128){
		goto exit;
	}

	list_for_each_entry_safe(def,def_next,&local->ieee80211_name_list,list){

		if(def->s_name != name)
			continue;

		list_del(&def->list);

		new_def = atbm_krealloc(def,new_namelen+sizeof(struct ieee80211_name_def),GFP_ATOMIC);

		if(new_def == NULL){
			goto exit;
		}
		find_name = new_def->mem;
		new_def->d_name = find_name;
		new_def->s_name = name;
		new_def->name_size = new_namelen;

		memcpy(find_name,wiphy_name(local->hw.wiphy),strlen(wiphy_name(local->hw.wiphy)));
		find_name[strlen(wiphy_name(local->hw.wiphy))] = '-';
		memcpy(find_name+strlen(wiphy_name(local->hw.wiphy))+1,name,strlen(name)+1);
		break;
	}
	if(find_name){
		atbm_printk_debug("%s:find_name[%s]\n",__func__,find_name);
		list_add_tail(&new_def->list, &local->ieee80211_name_list);
	}
exit:
	spin_unlock_bh(&local->ieee80211_name_lock);
	return find_name;
}
char *ieee80211_alloc_name(struct ieee80211_hw *hw,const char *name)
{
	struct ieee80211_local *local = hw_to_local(hw);
	const char *phy_name = wiphy_name(local->hw.wiphy);
	int alloc_name_len = strlen(name)+strlen(phy_name)+2/*'-' + 0*/;
	char *alloc_name = NULL;
	struct ieee80211_name_def *def;

	alloc_name = ieee8211_find_name(hw,name);

	if(alloc_name)
		return alloc_name;
	
	spin_lock_bh(&local->ieee80211_name_lock);
	/*
	*local->ieee80211_name_len + alloc_name_len may be is too long
	*/
	if(alloc_name_len < 0)
		goto exit;
	
	if(alloc_name_len >= 128){
		atbm_printk_err("%s:name is too long\n",__func__);
		goto exit;
	}
	
	def = atbm_kzalloc(alloc_name_len + sizeof(struct ieee80211_name_def),GFP_ATOMIC);

	if(def == NULL){
		atbm_printk_err("%s def is null\n",__func__);
		goto exit;
	}
	
	alloc_name = def->mem;
	def->s_name = name;
	def->d_name = alloc_name;
	def->name_size = alloc_name_len;
	/*
	*phy_name-name
	*/
	memcpy(alloc_name,phy_name,strlen(phy_name));
	alloc_name[strlen(phy_name)] = '-';
	memcpy(alloc_name+strlen(phy_name)+1,name,strlen(name)+1);
	atbm_printk_debug("[%s],alloc_name_len[%d]\n",alloc_name,alloc_name_len);
	list_add_tail(&def->list, &local->ieee80211_name_list);
exit:
	spin_unlock_bh(&local->ieee80211_name_lock);
	return alloc_name;
}
/**
 * _ieee80211_is_robust_mgmt_frame - check if frame is a robust management frame
 * @hdr: the frame (buffer must include at least the first octet of payload)
 */
static inline bool _atbm_ieee80211_is_robust_mgmt_frame(struct ieee80211_hdr *hdr)
{
	if (ieee80211_is_disassoc(hdr->frame_control) ||
	    ieee80211_is_deauth(hdr->frame_control))
		return true;

	if (ieee80211_is_action(hdr->frame_control)) {
		u8 category;

		/*
		 * Action frames, excluding Public Action frames, are Robust
		 * Management Frames. However, if we are looking at a Protected
		 * frame, skip the check since the data may be encrypted and
		 * the frame has already been found to be a Robust Management
		 * Frame (by the other end).
		 */
		if (ieee80211_has_protected(hdr->frame_control))
			return true;
		category = *(((u8 *) hdr) + 24) & 0x7f;

		return category != ATBM_WLAN_CATEGORY_PUBLIC &&
			category != ATBM_WLAN_CATEGORY_HT &&
			category != ATBM_WLAN_CATEGORY_WNM_UNPROTECTED &&
			category != ATBM_WLAN_CATEGORY_SELF_PROTECTED &&
			category != ATBM_WLAN_CATEGORY_UNPROT_DMG &&
			category != ATBM_WLAN_CATEGORY_VHT &&
			category != ATBM_WLAN_CATEGORY_VENDOR_SPECIFIC;
	}

	return false;
}

/**
 * ieee80211_is_robust_mgmt_frame - check if skb contains a robust mgmt frame
 * @skb: the skb containing the frame, length will be checked
 */
bool atbm_ieee80211_is_robust_mgmt_frame(struct sk_buff *skb)
{
	if (skb->len < IEEE80211_MIN_ACTION_SIZE)
		return false;
	return _atbm_ieee80211_is_robust_mgmt_frame((void *)skb->data);
}
#ifndef CONFIG_IEEE80211_SPECIAL_FILTER
struct sk_buff *ieee80211_special_queue_package(struct ieee80211_vif *vif,struct sk_buff *skb)
{
	return skb;
}
#endif
