/*
 * mac80211 work implementation
 *
 * Copyright 2003-2008, Jouni Malinen <j@w1.fi>
 * Copyright 2004, Instant802 Networks, Inc.
 * Copyright 2005, Devicescape Software, Inc.
 * Copyright 2006-2007	Jiri Benc <jbenc@suse.cz>
 * Copyright 2007, Michael Wu <flamingice@sourmilk.net>
 * Copyright 2009, Johannes Berg <johannes@sipsolutions.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/delay.h>
#include <linux/if_ether.h>
#include <linux/skbuff.h>
#include <linux/if_arp.h>
#include <linux/etherdevice.h>
#include <linux/crc32.h>
#include <linux/slab.h>
#include <net/atbm_mac80211.h>
#include <asm/unaligned.h>

#include "ieee80211_i.h"
#include "rate.h"
#include "driver-ops.h"
#include "cfg.h"

#define IEEE80211_AUTH_TIMEOUT (HZ / 2)
#define IEEE80211_AUTH_MAX_TRIES 3
#define IEEE80211_ASSOC_TIMEOUT (HZ / 2)
#define IEEE80211_ASSOC_MAX_TRIES 3
#define IEEE80211_CONNECT_TIMEOUT	(HZ/2)
#define WK_CONNECT_TRIES_MAX (5)
static void ieee80211_work_empty_start_pendding(struct ieee80211_local *local);


enum work_action {
	WORK_ACT_MISMATCH,
	WORK_ACT_NONE,
	WORK_ACT_TIMEOUT,
	WORK_ACT_DONE,
};


/* utils */
static inline void ASSERT_WORK_MTX(struct ieee80211_local *local)
{
	lockdep_assert_held(&local->mtx);
}

/*
 * We can have multiple work items (and connection probing)
 * scheduling this timer, but we need to take care to only
 * reschedule it when it should fire _earlier_ than it was
 * asked for before, or if it's not pending right now. This
 * function ensures that. Note that it then is required to
 * run this function for all timeouts after the first one
 * has happened -- the work that runs from this timer will
 * do that.
 */
static void run_again(struct ieee80211_local *local,
		      unsigned long timeout)
{
	ASSERT_WORK_MTX(local);

	if (!atbm_timer_pending(&local->work_timer) ||
	    time_before(timeout, local->work_timer.expires))
		atbm_mod_timer(&local->work_timer, timeout);
}
#if ((LINUX_VERSION_CODE < KERNEL_VERSION(2,6,40)) || (defined (ATBM_ALLOC_MEM_DEBUG)))
static void work_free_rcu(struct rcu_head *head)
{
	struct ieee80211_work *wk =
		container_of(head, struct ieee80211_work, rcu_head);

	atbm_kfree(wk);
}

void free_work(struct ieee80211_work *wk)
{
	call_rcu(&wk->rcu_head, work_free_rcu);
}
#else
void free_work(struct ieee80211_work *wk)
{
	kfree_rcu(wk, rcu_head);
}

#endif
static int ieee80211_compatible_rates(const u8 *supp_rates, int supp_rates_len,
				      struct ieee80211_supported_band *sband,
				      u32 *rates)
{
	int i, j, count;
	*rates = 0;
	count = 0;
	for (i = 0; i < supp_rates_len; i++) {
		int rate = (supp_rates[i] & 0x7F) * 5;

		for (j = 0; j < sband->n_bitrates; j++)
			if (sband->bitrates[j].bitrate == rate) {
				*rates |= BIT(j);
				count++;
				break;
			}
	}

	return count;
}

/* frame sending functions */

static void ieee80211_add_ht_ie(struct sk_buff *skb, const u8 *ht_info_ie,
				struct ieee80211_supported_band *sband,
				struct ieee80211_channel *channel,
				enum ieee80211_smps_mode smps)
{
	struct ieee80211_ht_info *ht_info;
	u8 *pos;
	u32 flags = channel->flags;
	u16 cap = sband->ht_cap.cap;
	__le16 tmp;

	if (!sband->ht_cap.ht_supported)
		return;

	if (!ht_info_ie)
		return;

	if (ht_info_ie[1] < sizeof(struct ieee80211_ht_info))
		return;

	ht_info = (struct ieee80211_ht_info *)(ht_info_ie + 2);

	/* determine capability flags */

	switch (ht_info->ht_param & IEEE80211_HT_PARAM_CHA_SEC_OFFSET) {
	case IEEE80211_HT_PARAM_CHA_SEC_ABOVE:
		if (flags & IEEE80211_CHAN_NO_HT40PLUS) {
			cap &= ~IEEE80211_HT_CAP_SUP_WIDTH_20_40;
			cap &= ~IEEE80211_HT_CAP_SGI_40;
		}
		break;
	case IEEE80211_HT_PARAM_CHA_SEC_BELOW:
		if (flags & IEEE80211_CHAN_NO_HT40MINUS) {
			cap &= ~IEEE80211_HT_CAP_SUP_WIDTH_20_40;
			cap &= ~IEEE80211_HT_CAP_SGI_40;
		}
		break;
	}

	/* set SM PS mode properly */
	cap &= ~IEEE80211_HT_CAP_SM_PS;
	switch (smps) {
	case IEEE80211_SMPS_AUTOMATIC:
	case IEEE80211_SMPS_NUM_MODES:
		WARN_ON(1);
#if (LINUX_VERSION_CODE > KERNEL_VERSION(5, 10, 60))
	
		fallthrough;
#endif
	case IEEE80211_SMPS_OFF:
		cap |= WLAN_HT_CAP_SM_PS_DISABLED <<
			IEEE80211_HT_CAP_SM_PS_SHIFT;
		break;
	case IEEE80211_SMPS_STATIC:
		cap |= WLAN_HT_CAP_SM_PS_STATIC <<
			IEEE80211_HT_CAP_SM_PS_SHIFT;
		break;
	case IEEE80211_SMPS_DYNAMIC:
		cap |= WLAN_HT_CAP_SM_PS_DYNAMIC <<
			IEEE80211_HT_CAP_SM_PS_SHIFT;
		break;
	}

	/* reserve and fill IE */

	pos = atbm_skb_put(skb, sizeof(struct ieee80211_ht_cap) + 2);
	*pos++ = ATBM_WLAN_EID_HT_CAPABILITY;
	*pos++ = sizeof(struct ieee80211_ht_cap);
	memset(pos, 0, sizeof(struct ieee80211_ht_cap));

	/* capability flags */
	tmp = cpu_to_le16(cap);
	memcpy(pos, &tmp, sizeof(u16));
	pos += sizeof(u16);

	/* AMPDU parameters */
	*pos++ = sband->ht_cap.ampdu_factor |
		 (sband->ht_cap.ampdu_density <<
			IEEE80211_HT_AMPDU_PARM_DENSITY_SHIFT);

	/* MCS set */
	memcpy(pos, &sband->ht_cap.mcs, sizeof(sband->ht_cap.mcs));
	pos += sizeof(sband->ht_cap.mcs);

	/* extended capabilities */
	pos += sizeof(__le16);

	/* BF capabilities */
	pos += sizeof(__le32);

	/* antenna selection */
	pos += sizeof(u8);
}

static void ieee80211_send_assoc(struct ieee80211_sub_if_data *sdata,
				 struct ieee80211_work *wk)
{
	struct ieee80211_local *local = sdata->local;
	struct sk_buff *skb;
	struct atbm_ieee80211_mgmt *mgmt;
	u8 *pos, qos_info;
	size_t offset = 0, noffset;
	int i, count, rates_len, supp_rates_len;
	u16 capab;
	struct ieee80211_supported_band *sband;
	u32 rates = 0;

	sband = local->hw.wiphy->bands[wk->chan->band];

	if (wk->assoc.supp_rates_len) {
		/*
		 * Get all rates supported by the device and the AP as
		 * some APs don't like getting a superset of their rates
		 * in the association request (e.g. D-Link DAP 1353 in
		 * b-only mode)...
		 */
		rates_len = ieee80211_compatible_rates(wk->assoc.supp_rates,
						       wk->assoc.supp_rates_len,
						       sband, &rates);
	} else {
		/*
		 * In case AP not provide any supported rates information
		 * before association, we send information element(s) with
		 * all rates that we support.
		 */
		rates = ~0;
		rates_len = sband->n_bitrates;
	}

	skb = atbm_alloc_skb(local->hw.extra_tx_headroom +
			sizeof(*mgmt) + /* bit too much but doesn't matter */
			2 + wk->assoc.ssid_len + /* SSID */
			4 + rates_len + /* (extended) rates */
			4 + /* power capability */
			2 + 2 * sband->n_channels + /* supported channels */
			2 + sizeof(struct ieee80211_ht_cap) + /* HT */
			wk->ie_len + /* extra IEs */
			9, /* WMM */
			GFP_KERNEL);
	if (!skb)
		return;

	atbm_skb_reserve(skb, local->hw.extra_tx_headroom);

	capab = WLAN_CAPABILITY_ESS;

	if (sband->band == IEEE80211_BAND_2GHZ) {
		if (!(local->hw.flags & IEEE80211_HW_2GHZ_SHORT_SLOT_INCAPABLE))
			capab |= WLAN_CAPABILITY_SHORT_SLOT_TIME;
		if (!(local->hw.flags & IEEE80211_HW_2GHZ_SHORT_PREAMBLE_INCAPABLE))
			capab |= WLAN_CAPABILITY_SHORT_PREAMBLE;
	}

	if (wk->assoc.capability & WLAN_CAPABILITY_PRIVACY)
		capab |= WLAN_CAPABILITY_PRIVACY;

	if ((wk->assoc.capability & WLAN_CAPABILITY_SPECTRUM_MGMT) &&
	    (local->hw.flags & IEEE80211_HW_SPECTRUM_MGMT))
		capab |= WLAN_CAPABILITY_SPECTRUM_MGMT;

	mgmt = (struct atbm_ieee80211_mgmt *) atbm_skb_put(skb, 24);
	memset(mgmt, 0, 24);
	memcpy(mgmt->da, wk->filter_ta, ETH_ALEN);
	memcpy(mgmt->sa, sdata->vif.addr, ETH_ALEN);
	memcpy(mgmt->bssid, wk->filter_ta, ETH_ALEN);

	if (!sdata->vif.p2p && !is_zero_ether_addr(wk->assoc.prev_bssid)) {
		atbm_skb_put(skb, 10);
		mgmt->frame_control = cpu_to_le16(IEEE80211_FTYPE_MGMT |
						  IEEE80211_STYPE_REASSOC_REQ);
		mgmt->u.reassoc_req.capab_info = cpu_to_le16(capab);
		mgmt->u.reassoc_req.listen_interval =
				cpu_to_le16(sdata->vif.bss_conf.listen_interval);
		memcpy(mgmt->u.reassoc_req.current_ap, wk->assoc.prev_bssid,
		       ETH_ALEN);
	} else {
		atbm_skb_put(skb, 4);
		mgmt->frame_control = cpu_to_le16(IEEE80211_FTYPE_MGMT |
						  IEEE80211_STYPE_ASSOC_REQ);
		mgmt->u.assoc_req.capab_info = cpu_to_le16(capab);
		mgmt->u.assoc_req.listen_interval =
				cpu_to_le16(sdata->vif.bss_conf.listen_interval);
	}

	/* SSID */
	pos = atbm_skb_put(skb, 2 + wk->assoc.ssid_len);
	*pos++ = ATBM_WLAN_EID_SSID;
	*pos++ = wk->assoc.ssid_len;
	memcpy(pos, wk->assoc.ssid, wk->assoc.ssid_len);

	/* add all rates which were marked to be used above */
	supp_rates_len = rates_len;
	if (supp_rates_len > 8)
		supp_rates_len = 8;

	pos = atbm_skb_put(skb, supp_rates_len + 2);
	*pos++ = ATBM_WLAN_EID_SUPP_RATES;
	*pos++ = supp_rates_len;

	count = 0;
	for (i = 0; i < sband->n_bitrates; i++) {
		if (BIT(i) & rates) {
			int rate = sband->bitrates[i].bitrate;
			if(i<4){
				*pos++ = (u8) ((rate / 5)|0x80);
			}else{
				*pos++ = (u8) (rate / 5);
			}
			if (++count == 8)
				break;
		}
	}

	if (rates_len > count) {
		pos = atbm_skb_put(skb, rates_len - count + 2);
		*pos++ = ATBM_WLAN_EID_EXT_SUPP_RATES;
		*pos++ = rates_len - count;

		for (i++; i < sband->n_bitrates; i++) {
			if (BIT(i) & rates) {
				int rate = sband->bitrates[i].bitrate;
				*pos++ = (u8)(rate / 5);
			}
		}
	}

	if (capab & WLAN_CAPABILITY_SPECTRUM_MGMT) {
		/* 1. power capabilities */
		pos = atbm_skb_put(skb, 4);
		*pos++ = ATBM_WLAN_EID_PWR_CAPABILITY;
		*pos++ = 2;
		*pos++ = 0; /* min tx power */
		*pos++ = wk->chan->max_power; /* max tx power */

		/* 2. supported channels */
		/* TODO: get this in reg domain format */
		pos = atbm_skb_put(skb, 2 * sband->n_channels + 2);
		*pos++ = ATBM_WLAN_EID_SUPPORTED_CHANNELS;
		*pos++ = 2 * sband->n_channels;
		for (i = 0; i < sband->n_channels; i++) {
			*pos++ = ieee80211_frequency_to_channel(
					channel_center_freq(&sband->channels[i]));
			*pos++ = 1; /* one channel in the subband*/
		}
	}

	/* if present, add any custom IEs that go before HT */
	if (wk->ie_len && wk->ie) {
		static const u8 before_ht[] = {
			ATBM_WLAN_EID_SSID,
			ATBM_WLAN_EID_SUPP_RATES,
			ATBM_WLAN_EID_EXT_SUPP_RATES,
			ATBM_WLAN_EID_PWR_CAPABILITY,
			ATBM_WLAN_EID_SUPPORTED_CHANNELS,
			ATBM_WLAN_EID_RSN,
			ATBM_WLAN_EID_QOS_CAPA,
			ATBM_WLAN_EID_RRM_ENABLED_CAPABILITIES,
			ATBM_WLAN_EID_MOBILITY_DOMAIN,
			ATBM_WLAN_EID_SUPPORTED_REGULATORY_CLASSES,
		};
		noffset = atbm_ieee80211_ie_split(wk->ie, wk->ie_len,
					     before_ht, ARRAY_SIZE(before_ht),
					     offset);
		pos = atbm_skb_put(skb, noffset - offset);
		memcpy(pos, wk->ie + offset, noffset - offset);
		offset = noffset;
	}

	if (wk->assoc.use_11n && wk->assoc.wmm_used &&
	    local->hw.queues >= 4)
		ieee80211_add_ht_ie(skb, wk->assoc.ht_information_ie,
				    sband, wk->chan, wk->assoc.smps);

	/* if present, add any custom non-vendor IEs that go after HT */
	if (wk->ie_len && wk->ie) {
		noffset = ieee80211_ie_split_vendor(wk->ie, wk->ie_len,
						    offset);
		pos = atbm_skb_put(skb, noffset - offset);
		memcpy(pos, wk->ie + offset, noffset - offset);
		offset = noffset;
	}

	if (wk->assoc.wmm_used && local->hw.queues >= 4) {
		if (wk->assoc.uapsd_used) {
			qos_info = local->uapsd_queues;
			qos_info |= (local->uapsd_max_sp_len <<
				     IEEE80211_WMM_IE_STA_QOSINFO_SP_SHIFT);
		} else {
			qos_info = 0;
		}

		pos = atbm_skb_put(skb, 9);
		*pos++ = ATBM_WLAN_EID_VENDOR_SPECIFIC;
		*pos++ = 7; /* len */
		*pos++ = 0x00; /* Microsoft OUI 00:50:F2 */
		*pos++ = 0x50;
		*pos++ = 0xf2;
		*pos++ = 2; /* WME */
		*pos++ = 0; /* WME info */
		*pos++ = 1; /* WME ver */
		*pos++ = qos_info;
	}

	/* add any remaining custom (i.e. vendor specific here) IEs */
	if (wk->ie_len && wk->ie) {
		noffset = wk->ie_len;
		pos = atbm_skb_put(skb, noffset - offset);
		memcpy(pos, wk->ie + offset, noffset - offset);
	}

	IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT;
	ieee80211_tx_skb(sdata, skb);
}

static void ieee80211_remove_auth_bss(struct ieee80211_local *local,
				      struct ieee80211_work *wk)
{
	struct cfg80211_bss *cbss;
	u16 capa_val = WLAN_CAPABILITY_ESS;

	if (wk->probe_auth.privacy)
		capa_val |= WLAN_CAPABILITY_PRIVACY;

	cbss = ieee80211_atbm_get_bss(local->hw.wiphy, wk->chan, wk->filter_ta,
				wk->probe_auth.ssid, wk->probe_auth.ssid_len,
				WLAN_CAPABILITY_ESS | WLAN_CAPABILITY_PRIVACY,
				capa_val);
	if (!cbss)
		return;

	cfg80211_unlink_bss(local->hw.wiphy, cbss);
	ieee80211_atbm_put_bss(local->hw.wiphy,cbss);
}

static enum work_action __must_check
ieee80211_direct_probe(struct ieee80211_work *wk)
{
	struct ieee80211_sub_if_data *sdata = wk->sdata;
	struct ieee80211_local *local = sdata->local;
	
	if (!wk->probe_auth.synced) {
		int ret = drv_tx_sync(local, sdata, wk->filter_ta,
				      IEEE80211_TX_SYNC_AUTH);
		if (ret)
			return WORK_ACT_TIMEOUT;
	}
	wk->probe_auth.synced = true;

	wk->probe_auth.tries++;
	if (wk->probe_auth.tries > IEEE80211_AUTH_MAX_TRIES) {
		atbm_printk_err("%s: direct probe to %pM timed out\n",
		       sdata->name, wk->filter_ta);

		ieee80211_free_authen_bss(sdata);
		/*
		 * Most likely AP is not in the range so remove the
		 * bss struct for that AP.
		 */
		ieee80211_remove_auth_bss(local, wk);

		return WORK_ACT_TIMEOUT;
	}

	atbm_printk_mgmt("%s: direct probe to %pM (try %d/%i)\n",
	       sdata->name, wk->filter_ta, wk->probe_auth.tries,
	       IEEE80211_AUTH_MAX_TRIES);

	/*
	 * Direct probe is sent to broadcast address as some APs
	 * will not answer to direct packet in unassociated state.
	 */
	ieee80211_send_probe_req(sdata, NULL, wk->probe_auth.ssid,
				 wk->probe_auth.ssid_len, NULL, 0,
				 (u32) -1, true, false);

	wk->timeout = jiffies + IEEE80211_AUTH_TIMEOUT;
	run_again(local, wk->timeout);

	return WORK_ACT_NONE;
}


static enum work_action __must_check
ieee80211_authenticate(struct ieee80211_work *wk)
{
	struct ieee80211_sub_if_data *sdata = wk->sdata;
	struct ieee80211_local *local = sdata->local;
	struct cfg80211_bss *bss;
	u16 trans = 1;
	u16 status = 0;
	
	if (!wk->probe_auth.synced) {
		int ret = drv_tx_sync(local, sdata, wk->filter_ta,
				      IEEE80211_TX_SYNC_AUTH);
		if (ret)
			return WORK_ACT_TIMEOUT;
	}
	wk->probe_auth.synced = true;

	/* HACK!!! apollo device requires SSID to be available at AUTH stage.
	 * cfg80211 beacon cache is designed to handle multi-SSID BSSes, so
	 * bss struct returned by ieee80211_atbm_get_bss() has random SSID if BSS
	 * just changed SSID before authentication (typical for p2p).
	 * This is a firmware design fault, however as a workaround cfg80211
	 * beacon cache is purged to make sure target BSS is searchable
	 * in rb-tree at the AUTH stage.
	 */
	#if 0
	while (true) {
		bss = ieee80211_atbm_get_bss(local->hw.wiphy,
				wk->probe_auth.bss->channel,
				wk->probe_auth.bss->bssid,
				NULL, 0, 0, 0);
		if (WARN_ON(!bss))
			break;
		if (bss == wk->probe_auth.bss) {
			ieee80211_atbm_put_bss(local->hw.wiphy,bss);
			break;
		}
		cfg80211_unlink_bss(local->hw.wiphy, bss);
	}
	#else

	bss = __ieee80211_atbm_get_authen_bss(&sdata->vif,wk->probe_auth.bss->channel,
				wk->probe_auth.bss->bssid,NULL,0);
	if(bss)
		__ieee80211_atbm_put_authen_bss(&sdata->vif,bss);
	else
		WARN_ON(1);
	#endif
	/* End of the hack */

#ifdef CONFIG_MAC80211_ATBM_ROAMING_CHANGES
	/*
	 * IEEE 802.11r - Fast BSS Transition is currently not supported by
	 * altobeam solution.
	 * TODO: When 11r will be supported check if we can lock queues
	 * when algorithm == WLAN_AUTH_FT.
	 */
	if (WLAN_AUTH_FT != wk->probe_auth.algorithm)
		sdata->queues_locked = 1;
#endif
	wk->probe_auth.tries++;
	if (wk->probe_auth.tries > IEEE80211_AUTH_MAX_TRIES) {
		atbm_printk_err("%s: authentication with %pM"
		       " timed out\n", sdata->name, wk->filter_ta);

		/*
		 * Most likely AP is not in the range so remove the
		 * bss struct for that AP.
		 */
		ieee80211_free_authen_bss(sdata);
		
		ieee80211_remove_auth_bss(local, wk);
#ifdef CONFIG_MAC80211_ATBM_ROAMING_CHANGES
		sdata->queues_locked = 0;
#endif

		return WORK_ACT_TIMEOUT;
	}

	atbm_printk_mgmt("%s: authenticate with %pM (try %d)\n",
	       sdata->name, wk->filter_ta, wk->probe_auth.tries);

	wk->probe_auth.transaction = 2;
#ifdef CONFIG_ATBM_SUPPORT_SAE
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0))

//#if (LINUX_VERSION_CODE > KERNEL_VERSION(4, 0, 0))
	if (wk->probe_auth.algorithm == WLAN_AUTH_SAE) {
		trans = wk->probe_auth.sae_trans;
		status = wk->probe_auth.sae_status;
		wk->probe_auth.transaction = trans;
	}
#endif
#endif
	ieee80211_send_auth(sdata, trans, wk->probe_auth.algorithm, status, wk->ie,
			    wk->ie_len, wk->filter_ta, NULL, 0, 0);
//	wk->probe_auth.transaction = 2;

	wk->timeout = jiffies + IEEE80211_AUTH_TIMEOUT;
	run_again(local, wk->timeout);

	return WORK_ACT_NONE;
}

static enum work_action __must_check
ieee80211_associate(struct ieee80211_work *wk)
{
	struct ieee80211_sub_if_data *sdata = wk->sdata;
	struct ieee80211_local *local = sdata->local;

	if (!wk->assoc.synced) {
		int ret = drv_tx_sync(local, sdata, wk->filter_ta,
				      IEEE80211_TX_SYNC_ASSOC);
		if (ret)
			return WORK_ACT_TIMEOUT;
	}
	wk->assoc.synced = true;

	wk->assoc.tries++;
	if (wk->assoc.tries > IEEE80211_ASSOC_MAX_TRIES) {
		atbm_printk_err("%s: association with %pM"
		       " timed out\n",
		       sdata->name, wk->filter_ta);

		/*
		 * Most likely AP is not in the range so remove the
		 * bss struct for that AP.
		 */
		if (wk->assoc.bss)
			cfg80211_unlink_bss(local->hw.wiphy, wk->assoc.bss);
		ieee80211_cancle_connecting_work(wk->sdata,wk->filter_ta,true);
		return WORK_ACT_TIMEOUT;
	}

	atbm_printk_mgmt( "%s: associate with %pM (try %d)\n",
	       sdata->name, wk->filter_ta, wk->assoc.tries);
	ieee80211_send_assoc(sdata, wk);

	wk->timeout = jiffies + IEEE80211_ASSOC_TIMEOUT;
	run_again(local, wk->timeout);

	return WORK_ACT_NONE;
}
#if 0

static enum work_action __must_check
ieee80211_remain_on_channel_timeout(struct ieee80211_work *wk)
{
	/*
	 * First time we run, do nothing -- the generic code will
	 * have switched to the right channel etc.
	 */
	if (!wk->started) {
		wk->timeout = jiffies + msecs_to_jiffies(wk->remain.duration);
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,6,0))
		cfg80211_ready_on_channel(wk->sdata->dev, (unsigned long) wk,
					  wk->chan, wk->chan_type,
					  wk->remain.duration, GFP_KERNEL);
#elif (LINUX_VERSION_CODE < KERNEL_VERSION(3,8,0))
		cfg80211_ready_on_channel(&wk->sdata->wdev, (unsigned long) wk,
					  wk->chan, wk->chan_type,
					  wk->remain.duration, GFP_KERNEL);
#else
		cfg80211_ready_on_channel(&wk->sdata->wdev, (unsigned long) wk,
					  wk->chan,
					  wk->remain.duration, GFP_KERNEL);
#endif

		return WORK_ACT_NONE;
	}

	return WORK_ACT_TIMEOUT;
}
static enum work_action __must_check
ieee80211_offchannel_tx(struct ieee80211_work *wk)
{
	if (!wk->started) {
		wk->timeout = jiffies + msecs_to_jiffies(wk->offchan_tx.wait);

		/*
		 * After this, offchan_tx.frame remains but now is no
		 * longer a valid pointer -- we still need it as the
		 * cookie for canceling this work/status matching.
		 */
		ieee80211_tx_skb(wk->sdata, wk->offchan_tx.frame);

		return WORK_ACT_NONE;
	}

	return WORK_ACT_TIMEOUT;
}
#endif
static enum work_action __must_check
ieee80211_assoc_beacon_wait(struct ieee80211_work *wk)
{
	if (wk->started)
		return WORK_ACT_TIMEOUT;

	/*
	 * Wait up to one beacon interval ...
	 * should this be more if we miss one?
	 */
	atbm_printk_mgmt("%s: waiting for beacon from %pM\n",
	       wk->sdata->name, wk->filter_ta);
	wk->timeout = TU_TO_EXP_TIME(wk->assoc.bss->beacon_interval);
	return WORK_ACT_NONE;
}

static enum work_action __must_check
ieee80211_wk_connecting(struct ieee80211_work *wk)
{
	struct ieee80211_local *local = wk->sdata->local;
	struct ieee80211_sub_if_data *sdata = wk->sdata;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	lockdep_assert_held(&local->mtx);
	wk->connecting.tries++;
	atbm_printk_mgmt("%s:connecting.tries(%d)\n",__func__,wk->connecting.tries);
	if(wk->connecting.tries>wk->connecting.retry_max){
		atbm_printk_err("%s: time out\n",__func__);
		return WORK_ACT_TIMEOUT;
	}

	if(ifmgd->associated&&(sdata->vif.bss_conf.arp_filter_enabled == true)
		&&(sdata->vif.bss_conf.arp_addr_cnt>0)){
		atbm_printk_mgmt("%s:arp_filter_state\n",__func__);
		return WORK_ACT_TIMEOUT;
	}
#ifdef IPV6_FILTERING
	if(ifmgd->associated&&(sdata->vif.bss_conf.ndp_filter_enabled==true)
	&&(sdata->vif.bss_conf.ndp_addr_cnt>0)){
		atbm_printk_mgmt("%s:arp_filter_state\n",__func__);
		return WORK_ACT_TIMEOUT;
	}
#endif /*IPV6_FILTERING*/
	wk->timeout = jiffies + IEEE80211_CONNECT_TIMEOUT;
//	wk->connecting.retry_max = WK_CONNECT_TRIES_MAX; 
	run_again(local, wk->timeout);
	return WORK_ACT_NONE;
}

#if 0
static enum work_action __must_check
ieee80211_wk_disconnecting(struct ieee80211_work *wk)
{
	struct ieee80211_local *local = wk->sdata->local;
	struct ieee80211_sub_if_data *sdata = wk->sdata;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	lockdep_assert_held(&local->mtx);
	wk->disconnecting.tries++;
	if(wk->disconnecting.tries>=wk->disconnecting.retry_max){
		return WORK_ACT_TIMEOUT;
	}
	wk->timeout = jiffies + (HZ/10);
//	wk->connecting.retry_max = WK_CONNECT_TRIES_MAX; 
	run_again(local, wk->timeout);
	return WORK_ACT_NONE;
}
#endif
static void ieee80211_auth_challenge(struct ieee80211_work *wk,
				     struct atbm_ieee80211_mgmt *mgmt,
				     size_t len)
{
	struct ieee80211_sub_if_data *sdata = wk->sdata;
	u8 *pos;
	struct ieee802_atbm_11_elems elems;

	pos = mgmt->u.auth.variable;
	ieee802_11_parse_elems(pos, len - (pos - (u8 *) mgmt), &elems);
	if (!elems.challenge)
		return;
	ieee80211_send_auth(sdata, 3, wk->probe_auth.algorithm, 0,
			    elems.challenge - 2, elems.challenge_len + 2,
			    wk->filter_ta, wk->probe_auth.key,
			    wk->probe_auth.key_len, wk->probe_auth.key_idx);
	wk->probe_auth.transaction = 4;
}

static enum work_action __must_check
ieee80211_rx_mgmt_auth(struct ieee80211_work *wk,
		       struct atbm_ieee80211_mgmt *mgmt, size_t len)
{
	u16 auth_alg, auth_transaction, status_code;
	struct ieee80211_sub_if_data *sdata = wk->sdata;
	
	if (wk->type != IEEE80211_WORK_AUTH)
		return WORK_ACT_MISMATCH;

	if (len < 24 + 6)
		return WORK_ACT_NONE;

	auth_alg = le16_to_cpu(mgmt->u.auth.auth_alg);
	auth_transaction = le16_to_cpu(mgmt->u.auth.auth_transaction);
	status_code = le16_to_cpu(mgmt->u.auth.status_code);

	if (auth_alg != wk->probe_auth.algorithm ||
	    auth_transaction != wk->probe_auth.transaction){
	   atbm_printk_mgmt("%s: %pM denied authentication (auth_alg[%d,%d],transaction[%d:%d])\n",
		       wk->sdata->name, mgmt->sa, auth_alg, wk->probe_auth.algorithm,auth_transaction,wk->probe_auth.transaction);
		ieee80211_free_authen_bss(sdata);
		return WORK_ACT_NONE;
	}
	if (status_code != WLAN_STATUS_SUCCESS) {
		atbm_printk_err("%s: %pM denied authentication (status %d)\n",
		       wk->sdata->name, mgmt->sa, status_code);
#ifdef CONFIG_MAC80211_ATBM_ROAMING_CHANGES
		wk->sdata->queues_locked = 0;
#endif
		if(status_code == WLAN_STATUS_NOT_SUPPORTED_AUTH_ALG){
			atbm_printk_err("%s:auhen log err(%d)(%d)\n",__func__,auth_alg,wk->probe_auth.algorithm);
			/*
			*triger wpa_supplicant to retransmit an authen frame with the rignt algorithm
			*/
			mgmt->u.auth.auth_alg = cpu_to_le16(wk->probe_auth.algorithm);
		}
		ieee80211_free_authen_bss(sdata);
		return WORK_ACT_DONE;
	}

	switch (wk->probe_auth.algorithm) {
	case WLAN_AUTH_OPEN:
	case WLAN_AUTH_LEAP:
	case WLAN_AUTH_FT:
#ifdef CONFIG_ATBM_SUPPORT_SAE
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0))

//#if (LINUX_VERSION_CODE > KERNEL_VERSION(4, 0, 0))
	case WLAN_AUTH_SAE:
#endif
#endif
		break;
	case WLAN_AUTH_SHARED_KEY:
		if (wk->probe_auth.transaction != 4) {
			ieee80211_auth_challenge(wk, mgmt, len);
			/* need another frame */
			return WORK_ACT_NONE;
		}
		break;
	default:
		WARN_ON(1);
		return WORK_ACT_NONE;
	}

#if 0
#if (LINUX_VERSION_CODE > KERNEL_VERSION(4, 0, 0))
	if (wk->probe_auth.algorithm == WLAN_AUTH_SAE &&
	    wk->probe_auth.transaction != 2) {
		/*
		 * Report auth frame to user space for processing since another
		 * round of Authentication frames is still needed.
		 */
		cfg80211_rx_mlme_mgmt(sdata->dev, (u8 *)mgmt, len);
		return WORK_ACT_NONE;
	}
#endif
#endif

	atbm_printk_always("%s: authenticated\n", wk->sdata->name);
	return WORK_ACT_DONE;
}

static enum work_action __must_check
ieee80211_rx_mgmt_assoc_resp(struct ieee80211_work *wk,
			     struct atbm_ieee80211_mgmt *mgmt, size_t len,
			     bool reassoc)
{
	struct ieee80211_sub_if_data *sdata = wk->sdata;
	struct ieee80211_local *local = sdata->local;
	u16 capab_info, status_code, aid;
	struct ieee802_atbm_11_elems elems;
	u8 *pos;

	if (wk->type != IEEE80211_WORK_ASSOC)
		return WORK_ACT_MISMATCH;

	/*
	 * AssocResp and ReassocResp have identical structure, so process both
	 * of them in this function.
	 */

	if (len < 24 + 6)
		return WORK_ACT_NONE;

	capab_info = le16_to_cpu(mgmt->u.assoc_resp.capab_info);
	status_code = le16_to_cpu(mgmt->u.assoc_resp.status_code);
	aid = le16_to_cpu(mgmt->u.assoc_resp.aid);

	atbm_printk_mgmt("%s: RX %sssocResp from %pM (capab=0x%x "
	       "status=%d aid=%d)\n",
	       sdata->name, reassoc ? "Rea" : "A", mgmt->sa,
	       capab_info, status_code, (u16)(aid & ~(BIT(15) | BIT(14))));

	pos = mgmt->u.assoc_resp.variable;
	ieee802_11_parse_elems(pos, len - (pos - (u8 *) mgmt), &elems);

	if (status_code == WLAN_STATUS_ASSOC_REJECTED_TEMPORARILY &&
	    elems.timeout_int && elems.timeout_int_len == 5 &&
	    elems.timeout_int[0] == WLAN_TIMEOUT_ASSOC_COMEBACK) {
		u32 tu, ms;
		tu = get_unaligned_le32(elems.timeout_int + 1);
		ms = tu * 1024 / 1000;
		atbm_printk_err( "%s: %pM rejected association temporarily; "
		       "comeback duration %u TU (%u ms)\n",
		       sdata->name, mgmt->sa, tu, ms);
		wk->timeout = jiffies + msecs_to_jiffies(ms);
		if (ms > IEEE80211_ASSOC_TIMEOUT)
			run_again(local, wk->timeout);
		return WORK_ACT_NONE;
	}

	if (status_code != WLAN_STATUS_SUCCESS)
		atbm_printk_err( "%s: %pM denied association (code=%d)\n",
		       sdata->name, mgmt->sa, status_code);
	else
		atbm_printk_always( "%s: associated\n", sdata->name);
#ifdef CONFIG_MAC80211_ATBM_ROAMING_CHANGES
	sdata->queues_locked = 0;
#endif

	return WORK_ACT_DONE;
}

static enum work_action __must_check
ieee80211_rx_mgmt_probe_resp(struct ieee80211_work *wk,
			     struct atbm_ieee80211_mgmt *mgmt, size_t len,
			     struct ieee80211_rx_status *rx_status)
{
	struct ieee80211_sub_if_data *sdata = wk->sdata;
	struct ieee80211_local *local = sdata->local;
	size_t baselen;

	ASSERT_WORK_MTX(local);
	if (wk->type != IEEE80211_WORK_DIRECT_PROBE)
		return WORK_ACT_MISMATCH;

	if (len < 24 + 12)
		return WORK_ACT_NONE;

	baselen = (u8 *) mgmt->u.probe_resp.variable - (u8 *) mgmt;
	if (baselen > len)
		return WORK_ACT_NONE;

	atbm_printk_mgmt("%s: direct probe responded\n", sdata->name);
	return WORK_ACT_DONE;
}

static enum work_action __must_check
ieee80211_rx_mgmt_beacon(struct ieee80211_work *wk,
			 struct atbm_ieee80211_mgmt *mgmt, size_t len)
{
	struct ieee80211_sub_if_data *sdata = wk->sdata;
	struct ieee80211_local *local = sdata->local;

	ASSERT_WORK_MTX(local);

	if (wk->type != IEEE80211_WORK_ASSOC_BEACON_WAIT)
		return WORK_ACT_MISMATCH;

	if (len < 24 + 12)
		return WORK_ACT_NONE;

	atbm_printk_mgmt("%s: beacon received\n", sdata->name);
	return WORK_ACT_DONE;
}

static void ieee80211_work_rx_queued_mgmt(struct ieee80211_local *local,
					  struct sk_buff *skb)
{
	struct ieee80211_rx_status *rx_status;
	struct atbm_ieee80211_mgmt *mgmt;
	struct ieee80211_work *wk;
	enum work_action rma = WORK_ACT_NONE;
	u16 fc;

	rx_status = (struct ieee80211_rx_status *) skb->cb;
	mgmt = (struct atbm_ieee80211_mgmt *) skb->data;
	fc = le16_to_cpu(mgmt->frame_control);

	mutex_lock(&local->mtx);

	list_for_each_entry(wk, &local->work_list, list) {
		const u8 *bssid = NULL;
		switch (wk->type) {
		case IEEE80211_WORK_DIRECT_PROBE:
		case IEEE80211_WORK_AUTH:
		case IEEE80211_WORK_ASSOC:
		case IEEE80211_WORK_ASSOC_BEACON_WAIT:
			bssid = wk->filter_ta;
			break;
		default:
			continue;
		}

		/*
		 * Before queuing, we already verified mgmt->sa,
		 * so this is needed just for matching.
		 */
		if (atbm_compare_ether_addr(bssid, mgmt->bssid))
			continue;

		switch (fc & IEEE80211_FCTL_STYPE) {
		case IEEE80211_STYPE_BEACON:
			rma = ieee80211_rx_mgmt_beacon(wk, mgmt, skb->len);
			break;
		case IEEE80211_STYPE_PROBE_RESP:
			rma = ieee80211_rx_mgmt_probe_resp(wk, mgmt, skb->len,
							   rx_status);
			break;
		case IEEE80211_STYPE_AUTH:
			rma = ieee80211_rx_mgmt_auth(wk, mgmt, skb->len);
			break;
		case IEEE80211_STYPE_ASSOC_RESP:
			rma = ieee80211_rx_mgmt_assoc_resp(wk, mgmt,
							   skb->len, false);
			break;
		case IEEE80211_STYPE_REASSOC_RESP:
			rma = ieee80211_rx_mgmt_assoc_resp(wk, mgmt,
							   skb->len, true);
			break;
		default:
			WARN_ON(1);
			rma = WORK_ACT_NONE;
		}

		/*
		 * We've either received an unexpected frame, or we have
		 * multiple work items and need to match the frame to the
		 * right one.
		 */
		if (rma == WORK_ACT_MISMATCH)
			continue;

		/*
		 * We've processed this frame for that work, so it can't
		 * belong to another work struct.
		 * NB: this is also required for correctness for 'rma'!
		 */
		break;
	}

	switch (rma) {
	case WORK_ACT_MISMATCH:
		/* ignore this unmatched frame */
		break;
	case WORK_ACT_NONE:
		break;
	case WORK_ACT_DONE:
		list_del_rcu(&wk->list);
		break;
	default:
		WARN(1, "unexpected: %d", rma);
	}
//	ieee80211_work_empty_start_pendding(local);
	mutex_unlock(&local->mtx);

	if (rma != WORK_ACT_DONE)
		goto out;

	switch (wk->done(wk, skb)) {
	case WORK_DONE_DESTROY:
		free_work(wk);
		break;
	case WORK_DONE_REQUEUE:
		synchronize_rcu();
		wk->started = false; /* restart */
		mutex_lock(&local->mtx);
		list_add_tail(&wk->list, &local->work_list);
		mutex_unlock(&local->mtx);
	}

 out:
	atbm_kfree_skb(skb);
}

static bool ieee80211_work_ct_coexists(enum nl80211_channel_type wk_ct,
				       enum nl80211_channel_type oper_ct)
{
	switch (wk_ct) {
	case NL80211_CHAN_NO_HT:
		return true;
	case NL80211_CHAN_HT20:
		if (oper_ct != NL80211_CHAN_NO_HT)
			return true;
		return false;
	case NL80211_CHAN_HT40MINUS:
	case NL80211_CHAN_HT40PLUS:
		return (wk_ct == oper_ct);
	}
	WARN_ON(1); /* shouldn't get here */
	return false;
}

static void ieee80211_work_timer(unsigned long data)
{
	struct ieee80211_local *local = (void *) data;

	if (local->quiescing)
		return;

	ieee80211_queue_work(&local->hw, &local->work_work);
}
static void ieee80211_work_empty_start_pendding(struct ieee80211_local *local)
{
	lockdep_assert_held(&local->mtx);
#ifdef CONFIG_ATBM_SUPPORT_P2P	
	if(local->roc_pendding&&local->roc_pendding_sdata){
		struct ieee80211_roc_work * roc = local->roc_pendding;
		int ret;
		u32 init_duration = 0;
		u32 roc_duration = 0;
		u32 roc_duration_left = 0;
		u8 roc_timeout = 0;
		ret = -1;
		init_duration = roc->duration;

		if(init_duration == 0)
			init_duration = 100;
		roc_duration_left = init_duration;
		if(init_duration>70)
			roc_duration_left = init_duration -70;
			
		atbm_printk_mgmt("%s:roc_pendding,roc_duration_left(%d),init_duration(%d)\n",__func__,
			roc_duration_left,init_duration);
		roc_timeout = !time_is_after_jiffies(roc->pending_start_time+(roc_duration_left*HZ)/1000);

		if(roc_timeout || list_empty(&local->work_list)){
			local->roc_pendding = NULL;
			local->roc_pendding_sdata = NULL;
			if(roc_timeout){
				ret = -1;
			}
			else if (list_empty(&local->work_list)){
				roc_duration = ((roc->pending_start_time+(init_duration*HZ)/1000 - jiffies)*1000)/HZ;

				if(roc_duration>70)
					ret = ieee80211_start_pending_roc_work(local,roc->hw_roc_dev,
					roc->sdata,roc->chan,roc->chan_type,roc_duration,&roc->cookie,roc->frame);
				else{
					ret = -1;
				}
			}else{
				//ret = -1;
				//BUG_ON(1);
				atbm_printk_err("%s %d ,ERROR !!! \n",__func__,__LINE__);
				return;
			}

			if(ret)
			{
			#if 0
					mutex_unlock(&local->mtx);
					printk(KERN_ERR "%s:pennding roc err \n",__func__);
					ieee80211_roc_notify_destroy(roc);
					mutex_lock(&local->mtx);
			#else
				atbm_printk_mgmt("%s:pennding roc err roc_timeout(%d),roc_duration(%d),cookie(%llx)\n",
					__func__,roc_timeout,roc_duration,roc->mgmt_tx_cookie ? roc->mgmt_tx_cookie: roc->cookie);
			//	BUG_ON(local->scanning);
				//BUG_ON(!list_empty(&local->roc_list));
				if((local->scanning) || (!list_empty(&local->roc_list))){
					atbm_printk_err("%s %d ,ERROR !!!local->scanning = %ld ,list_empty=%d \n",
						__func__,__LINE__,local->scanning,list_empty(&local->roc_list));
					return;
				}
				
				roc->started = true;
				list_add_tail(&roc->list, &local->roc_list);
				/*
				*first ,send remain_on_channel_ready event to wpa_supplicatn
				*second, send cancle_remain_on_channel event to wpa_supplicant
				*/
	//				ieee80211_ready_on_channel(&local->hw);
				ieee80211_remain_on_channel_expired(&local->hw,
						(roc->mgmt_tx_cookie ? roc->mgmt_tx_cookie: roc->cookie));
			#endif
			}
			else
			{
				atbm_kfree(roc);
			}
		}

	}
#endif
	if(local->scan_req &&!local->scanning){
		u8  scan_timeout;

		scan_timeout = !time_is_after_jiffies(local->pending_scan_start_time+(4*HZ));

		if(scan_timeout || list_empty(&local->work_list)){
			if(scan_timeout){
				if (local->scan_req != local->int_scan_req){
					atbm_printk_mgmt("%s(%s):cancle scan,scan_timeout(%d),work_list(%d)\n",__func__,local->scan_sdata->name,scan_timeout,list_empty(&local->work_list));
					atbm_notify_scan_done(local,local->scan_req, false);
					local->scanning = 0;
					local->scan_req = NULL;
					local->scan_sdata = NULL;
				}
			}
			else if(list_empty(&local->work_list)){
				atbm_printk_mgmt("%s(%s):start delayed scan opration\n",__func__,local->scan_sdata->name);
				ieee80211_queue_delayed_work(&local->hw,
					     &local->scan_work,
					     round_jiffies_relative(0));
			}
			else{
				atbm_printk_err("%s %d ,ERROR !!! \n",__func__,__LINE__);
				//WARN_ON(1);
				return;
			}
		}
	}
}
static bool ieee80211_work_need_sync_channel(struct ieee80211_work *wk)
{
	return wk->chan ? true:false;
}
#ifdef CONFIG_ATBM_STA_DYNAMIC_PS
static enum work_action __must_check
ieee80211_wk_ps_recal(struct ieee80211_work *wk)
{
	struct ieee80211_local *local = wk->sdata->local;

	ieee80211_recalc_ps_vif(local,-1);	
	return WORK_ACT_TIMEOUT;
}
#endif
static void ieee80211_work_work(struct atbm_work_struct *work)
{
	struct ieee80211_local *local =
		container_of(work, struct ieee80211_local, work_work);
	struct ieee80211_channel_state *chan_state = &local->chan_state;
	struct sk_buff *skb;
	struct ieee80211_work *wk, *tmp;
	LIST_HEAD(free_work);
	enum work_action rma;
	bool remain_off_channel = false;
	u8 in_listenning = 1;
	
	if (local->scanning)
		return;

	/*
	 * ieee80211_queue_work() should have picked up most cases,
	 * here we'll pick the rest.
	 */
	if (WARN(local->suspended, "work during suspend\n"))
		return;

	/* first process frames to avoid timing out while a frame is pending */
	while ((skb = atbm_skb_dequeue(&local->work_skb_queue)))
		ieee80211_work_rx_queued_mgmt(local, skb);

	mutex_lock(&local->mtx);
#ifdef CONFIG_ATBM_SUPPORT_P2P	
	in_listenning = !list_empty(&local->roc_list);
#else
	in_listenning = 0;
#endif
	ieee80211_recalc_idle(local);

	list_for_each_entry_safe(wk, tmp, &local->work_list, list) {
		bool started = wk->started;
		
		if(in_listenning){
			if(!started){
				atbm_printk_mgmt("%s:in_listenning delay work\n",__func__);
				continue;
			}
		}
		if(ieee80211_work_need_sync_channel(wk) == true){
			/* mark work as started if it's on the current off-channel */
			if (!started && chan_state->tmp_channel &&
			    wk->chan == chan_state->tmp_channel &&
			    wk->chan_type == chan_state->tmp_channel_type) {
			    if((wk->type != IEEE80211_WORK_CONNECTTING)&&(atomic_read(&local->connectting)==0)){
					started = true;
					wk->timeout = jiffies;
			    }
			}
			if (!started && !chan_state->tmp_channel) {
				/*
				 * TODO: could optimize this by leaving the
				 *	 station vifs in awake mode if they
				 *	 happen to be on the same channel as
				 *	 the requested channel
				 */
				atbm_printk_mgmt("%s:start work ch(%d)(%d)\n",__func__,channel_hw_value(wk->chan),wk->type);
				
				if(wk->type != IEEE80211_WORK_CONNECTTING){
	//				ieee80211_offchannel_stop_beaconing(local);
	//				ieee80211_offchannel_stop_station(local);
				}
				else {
					atomic_set(&wk->sdata->connectting,IEEE80211_ATBM_CONNECT_RUN);
					atomic_set(&local->connectting,1);
				}

				chan_state->tmp_channel = wk->chan;
				chan_state->tmp_channel_type = wk->chan_type;
				ieee80211_hw_config(local, 0);
				started = true;
				wk->timeout = jiffies;
			}
		}else {
			started = true;
			wk->timeout = jiffies;
		}
		/* don't try to work with items that aren't started */
		if (!started){			
			atbm_printk_mgmt("%s:not start work ch(%d)\n",__func__,wk->type);
			continue;
		}
		if (time_is_after_jiffies(wk->timeout)) {
			/*
			 * This work item isn't supposed to be worked on
			 * right now, but take care to adjust the timer
			 * properly.
			 */
			run_again(local, wk->timeout);
			continue;
		}

		switch (wk->type) {
		default:
			WARN_ON(1);
			/* nothing */
			rma = WORK_ACT_NONE;
			break;
		case IEEE80211_WORK_ABORT:
			rma = WORK_ACT_TIMEOUT;
			break;
		case IEEE80211_WORK_DIRECT_PROBE:
			rma = ieee80211_direct_probe(wk);
			break;
		case IEEE80211_WORK_AUTH:
			rma = ieee80211_authenticate(wk);
			break;
		case IEEE80211_WORK_ASSOC:
			rma = ieee80211_associate(wk);
			break;
		case IEEE80211_WORK_ASSOC_BEACON_WAIT:
			rma = ieee80211_assoc_beacon_wait(wk);
			break;
		case IEEE80211_WORK_CONNECTTING:
			rma = ieee80211_wk_connecting(wk);
			break;
#ifdef CONFIG_ATBM_STA_DYNAMIC_PS
		case IEEE80211_WORK_PS_RECAL:
			rma = ieee80211_wk_ps_recal(wk);
			break;
#endif
		}

		wk->started = started;

		switch (rma) {
		case WORK_ACT_NONE:
			/* might have changed the timeout */
			run_again(local, wk->timeout);
			break;
		case WORK_ACT_TIMEOUT:
			list_del_rcu(&wk->list);
			synchronize_rcu();
			list_add(&wk->list, &free_work);
			break;
		default:
			WARN(1, "unexpected: %d", rma);
		}		
	}

	list_for_each_entry(wk, &local->work_list, list) {
		if (!wk->started)
			continue;
		if (wk->chan != chan_state->tmp_channel)
			continue;
		if (!ieee80211_work_ct_coexists(wk->chan_type,
						chan_state->tmp_channel_type))
			continue;
		remain_off_channel = true;
	}

	if (!remain_off_channel && chan_state->tmp_channel) {
		chan_state->tmp_channel = NULL;
		/* If tmp_channel wasn't operating channel, then
		 * we need to go back on-channel.
		 * NOTE:  If we can ever be here while scannning,
		 * or if the hw_config() channel config logic changes,
		 * then we may need to do a more thorough check to see if
		 * we still need to do a hardware config.  Currently,
		 * we cannot be here while scanning, however.
		 */
		if(ieee80211_get_channel_mode(local,NULL) != CHAN_MODE_UNDEFINED){
			atbm_printk_mgmt("%s:reset work ch(%d)\n",__func__,channel_hw_value(chan_state->oper_channel));
			ieee80211_hw_config(local, 0);
		}

		/* At the least, we need to disable offchannel_ps,
		 * so just go ahead and run the entire offchannel
		 * return logic here.  We *could* skip enabling
		 * beaconing if we were already on-oper-channel
		 * as a future optimization.
		 */
//		ieee80211_offchannel_return(local, true);

		/* give connection some time to breathe */
		run_again(local, jiffies + HZ/10);
	}

	ieee80211_work_empty_start_pendding(local);

	ieee80211_recalc_idle(local);

	mutex_unlock(&local->mtx);

	list_for_each_entry_safe(wk, tmp, &free_work, list) {
		wk->done(wk, NULL);
		list_del(&wk->list);
		atbm_kfree(wk);
	}
}

void ieee80211_add_work(struct ieee80211_work *wk)
{
	struct ieee80211_local *local;

#ifndef CONFIG_ATBM_STA_DYNAMIC_PS
	if (WARN_ON(!wk->chan))
		return;
#endif

	if (WARN_ON(!wk->sdata))
		return;

	if (WARN_ON(!wk->done))
		return;

	if (WARN_ON(!ieee80211_sdata_running(wk->sdata)))
		return;

	wk->started = false;

	local = wk->sdata->local;
	mutex_lock(&local->mtx);
	list_add_tail(&wk->list, &local->work_list);
	mutex_unlock(&local->mtx);

	ieee80211_queue_work(&local->hw, &local->work_work);
}

void ieee80211_work_init(struct ieee80211_local *local)
{
	atomic_set(&local->connectting,0);
	INIT_LIST_HEAD(&local->work_list);
	atbm_setup_timer(&local->work_timer, ieee80211_work_timer,
		    (unsigned long)local);
	ATBM_INIT_WORK(&local->work_work, ieee80211_work_work);
	atbm_skb_queue_head_init(&local->work_skb_queue);
}

void ieee80211_work_purge(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_work *wk;
	bool cleanup = false;

	atbm_cancel_work_sync(&sdata->reconfig_filter);

	mutex_lock(&local->mtx);
	list_for_each_entry(wk, &local->work_list, list) {
		if (wk->sdata != sdata)
			continue;
		cleanup = true;
		wk->type = IEEE80211_WORK_ABORT;
		wk->started = true;
		wk->timeout = jiffies;
	}	
	ieee80211_free_authen_bss(sdata);
	mutex_unlock(&local->mtx);

	/* run cleanups etc. */
	if (cleanup)
		ieee80211_work_work(&local->work_work);

	mutex_lock(&local->mtx);
	list_for_each_entry(wk, &local->work_list, list) {
		if (wk->sdata != sdata)
			continue;
		WARN_ON(1);
		break;
	}
	mutex_unlock(&local->mtx);
}

ieee80211_rx_result ieee80211_work_rx_mgmt(struct ieee80211_sub_if_data *sdata,
					   struct sk_buff *skb)
{
	struct ieee80211_local *local = sdata->local;
	struct atbm_ieee80211_mgmt *mgmt;
	struct ieee80211_work *wk;
	u16 fc;

	if (skb->len < 24)
	{
		atbm_printk_mgmt("%s:skb->len < 24\n",__func__);
		return RX_DROP_MONITOR;
	}

	mgmt = (struct atbm_ieee80211_mgmt *) skb->data;
	fc = le16_to_cpu(mgmt->frame_control);
	list_for_each_entry_rcu(wk, &local->work_list, list) {
		if (sdata != wk->sdata)
			continue;
		if (atbm_compare_ether_addr(wk->filter_ta, mgmt->sa))
			continue;
		if (atbm_compare_ether_addr(wk->filter_ta, mgmt->bssid))
			continue;

		switch (fc & IEEE80211_FCTL_STYPE) {
		case IEEE80211_STYPE_AUTH:
		case IEEE80211_STYPE_PROBE_RESP:
		case IEEE80211_STYPE_ASSOC_RESP:
		case IEEE80211_STYPE_REASSOC_RESP:
		case IEEE80211_STYPE_BEACON:
			atbm_skb_queue_tail(&local->work_skb_queue, skb);
			ieee80211_queue_work(&local->hw, &local->work_work);
			return RX_QUEUED;
		}
	}
	return RX_CONTINUE;
}
static enum work_done_result ieee80211_connecting_work_done(struct ieee80211_work *wk,
						  struct sk_buff *skb)
{
	struct ieee80211_sub_if_data *sdata = wk->sdata;
	struct ieee80211_local *local = sdata->local;
	atbm_printk_mgmt("%s\n",__func__);
	if(atomic_read(&sdata->connectting) == IEEE80211_ATBM_CONNECT_RUN)
		atomic_set(&local->connectting,0);
	atomic_set(&sdata->connectting,IEEE80211_ATBM_CONNECT_DONE);
	return WORK_DONE_DESTROY;
}

void ieee80211_start_connecting_work(struct ieee80211_sub_if_data *sdata,struct ieee80211_channel *chan,
	u8* bssid)
{
	struct ieee80211_work *wk;
	wk = atbm_kzalloc(sizeof(*wk), GFP_KERNEL);
	if (WARN_ON(!wk))
		return ;

	wk->chan = chan;
	wk->chan_type = NL80211_CHAN_NO_HT;
	wk->sdata = sdata;
	wk->done  = ieee80211_connecting_work_done;
	wk->type = IEEE80211_WORK_CONNECTTING;
	wk->connecting.retry_max = WK_CONNECT_TRIES_MAX;
	wk->connecting.scan_reties = 0;
	memcpy(wk->filter_ta,bssid, ETH_ALEN);
	atbm_printk_debug("%s:bssid(%pM)\n",__func__,bssid);
	atomic_set(&sdata->connectting,IEEE80211_ATBM_CONNECT_SET);
	ieee80211_add_work(wk);
}
#ifdef CONFIG_ATBM_STA_DYNAMIC_PS
static enum work_done_result ieee80211_ps_recal_done(struct ieee80211_work *wk,
						  struct sk_buff *skb)
{
	atbm_printk_debug("%s\n",__func__);
	return WORK_DONE_DESTROY;
}

void ieee80211_start_ps_recal_work(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_work *wk;
	
	wk = atbm_kzalloc(sizeof(*wk), GFP_ATOMIC);
	if (WARN_ON(!wk))
		return ;
	
	wk->sdata = sdata;
	wk->done  = ieee80211_ps_recal_done;
	wk->type = IEEE80211_WORK_PS_RECAL;
	ieee80211_add_work(wk);
}
#endif
void ieee80211_cancle_connecting_work(struct ieee80211_sub_if_data *sdata,u8* bssid,bool delayed)
{
	struct ieee80211_work  *wk = NULL;
	struct ieee80211_local *local = sdata->local;
	bool cleanup = false;
	lockdep_assert_held(&local->mtx);
	list_for_each_entry(wk, &local->work_list, list) {
		if (wk->sdata != sdata)
			continue;

		if (wk->type != IEEE80211_WORK_CONNECTTING)
			continue;
		
		if (memcmp(bssid, wk->filter_ta, ETH_ALEN))
			continue;	
		cleanup = true;
		wk->type = IEEE80211_WORK_ABORT;
		wk->started = true;
		wk->timeout = jiffies;
	}
	if(delayed == false){
		mutex_unlock(&local->mtx);
		/* run cleanups etc. */
		if (cleanup == true){
			atbm_printk_mgmt("%s:cleanup\n",__func__);
			ieee80211_work_work(&local->work_work);
		}
		mutex_lock(&local->mtx);
	}else if (cleanup == true){
		ieee80211_queue_work(&local->hw, &local->work_work);
	}
}
void ieee80211_assign_authen_bss(struct ieee80211_sub_if_data *sdata,struct cfg80211_bss *pub)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;

	atbm_printk_mgmt("%s:assign authen bss[%p]\n",sdata->name,pub);
	lockdep_assert_held(&local->mtx);
	/*
	*hold our bss for later use
	*/
	if(ieee80211_atbm_handle_bss(local->hw.wiphy,pub) == 0){
		/*
		*save pub
		*/
	}else {
		atbm_printk_mgmt("%s:get bss err\n",sdata->name);
		pub = NULL;
	}
	rcu_assign_pointer(ifmgd->authen_bss,pub);
	synchronize_rcu();	
}

void ieee80211_free_authen_bss(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct cfg80211_bss *free_bss;
	lockdep_assert_held(&local->mtx);
	
	atbm_printk_always("%s:free authen bss ++\n",sdata->name);
	if(sdata->vif.type != NL80211_IFTYPE_STATION){
		atbm_printk_err("%s: is not sta mode\n",sdata->name);
		return;
	}
	free_bss = rcu_dereference_protected(ifmgd->authen_bss,lockdep_is_held(&local->mtx));
	if(free_bss == NULL){
		return;
	}
	rcu_assign_pointer(ifmgd->authen_bss, NULL);
	synchronize_rcu();	
	
	atbm_printk_always("%s:free authen bss --\n",sdata->name);
	atbm_printk_mgmt("%s:free authen bss[%p]\n",sdata->name,free_bss);
	ieee80211_atbm_release_bss(local->hw.wiphy,free_bss);
}
