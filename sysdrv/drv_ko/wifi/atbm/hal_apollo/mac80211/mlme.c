/*
 * BSS client mode implementation
 * Copyright 2003-2008, Jouni Malinen <j@w1.fi>
 * Copyright 2004, Instant802 Networks, Inc.
 * Copyright 2005, Devicescape Software, Inc.
 * Copyright 2006-2007	Jiri Benc <jbenc@suse.cz>
 * Copyright 2007, Michael Wu <flamingice@sourmilk.net>
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
#include <linux/moduleparam.h>
#include <linux/rtnetlink.h>
#ifdef CONFIG_ATBM_PM_QOS
#include <linux/pm_qos.h>
#endif
#include <linux/crc32.h>
#include <linux/slab.h>
#include <linux/export.h>
#include <net/atbm_mac80211.h>
#include <asm/unaligned.h>

#include "ieee80211_i.h"
#include "driver-ops.h"
#include "rate.h"
#include "led.h"


static int wifi_status = 0;

unsigned int atbm_wifi_status_get(void)
{
	return wifi_status;
}
static void atbm_wifi_status_set(unsigned int state)
{
	wifi_status = state;
	return;
}
//extern int g_connetting;
#if 0
static int max_nullfunc_tries = 2;
module_param(max_nullfunc_tries, int, 0644);
MODULE_PARM_DESC(max_nullfunc_tries,
		 "Maximum nullfunc tx tries before disconnecting (reason 4).");
static int max_probe_tries = 5;
module_param(max_probe_tries, int, 0644);
MODULE_PARM_DESC(max_probe_tries,
		 "Maximum probe tries before disconnecting (reason 4).");
#endif
#if defined (CONFIG_ATBM_MAC80211_NO_USE) || defined(CONFIG_PM)
static void ieee80211_sta_connection_lost(struct ieee80211_sub_if_data *sdata,
					  u8 *bssid, u8 reason);
#endif
/*
 * Beacon loss timeout is calculated as N frames times the
 * advertised beacon interval.  This may need to be somewhat
 * higher than what hardware might detect to account for
 * delays in the host processing frames. But since we also
 * probe on beacon miss before declaring the connection lost
 * default to what we want.
 */
#define IEEE80211_BEACON_LOSS_COUNT	7

/*
 * Time the connection can be idle before we probe
 * it to see if we can still talk to the AP.
 */
#define IEEE80211_CONNECTION_IDLE_TIME	(30 * HZ)
/*
 * Time we wait for a probe response after sending
 * a probe request because of beacon loss or for
 * checking the connection still works.
 */
#if 0
static int probe_wait_ms = 500;
module_param(probe_wait_ms, int, 0644);
MODULE_PARM_DESC(probe_wait_ms,"Maximum time";
#endif
/*
 * Weight given to the latest Beacon frame when calculating average signal
 * strength for Beacon frames received in the current BSS. This must be
 * between 1 and 15.
 */
#define IEEE80211_SIGNAL_AVE_WEIGHT	3

/*
 * How many Beacon frames need to have been used in average signal strength
 * before starting to indicate signal change events.
 */
#define IEEE80211_SIGNAL_AVE_MIN_COUNT	4

#define TMR_RUNNING_TIMER	0
#define TMR_RUNNING_CHANSW	1

/*
 * All cfg80211 functions have to be called outside a locked
 * section so that they can acquire a lock themselves... This
 * is much simpler than queuing up things in cfg80211, but we
 * do need some indirection for that here.
 */
enum rx_mgmt_action {
	/* no action required */
	RX_MGMT_NONE,

	/* caller must call cfg80211_send_deauth() */
	RX_MGMT_CFG80211_DEAUTH,

	/* caller must call cfg80211_send_disassoc() */
	RX_MGMT_CFG80211_DISASSOC,
};

/* utils */
static inline void ASSERT_MGD_MTX(struct ieee80211_if_managed *ifmgd)
{
	lockdep_assert_held(&ifmgd->mtx);
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
#ifdef CONFIG_ATBM_MAC80211_NO_USE
static void run_again(struct ieee80211_if_managed *ifmgd,
			     unsigned long timeout)
{
	ASSERT_MGD_MTX(ifmgd);

	if (!atbm_timer_pending(&ifmgd->timer) ||
	    time_before(timeout, ifmgd->timer.expires))
		atbm_mod_timer(&ifmgd->timer, timeout);
}

void ieee80211_sta_reset_beacon_monitor(struct ieee80211_sub_if_data *sdata)
{
	if (sdata->local->hw.flags & IEEE80211_HW_BEACON_FILTER)
		return;

	atbm_mod_timer(&sdata->u.mgd.bcn_mon_timer,
		  round_jiffies_up(jiffies + sdata->u.mgd.beacon_timeout));
}

void ieee80211_sta_reset_conn_monitor(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;

	if (unlikely(!sdata->u.mgd.associated))
		return;

	if (sdata->local->hw.flags & IEEE80211_HW_CONNECTION_MONITOR)
		return;

	atbm_mod_timer(&sdata->u.mgd.conn_mon_timer,
		  round_jiffies_up(jiffies + IEEE80211_CONNECTION_IDLE_TIME));

	ifmgd->probe_send_count = 0;
}

void ieee80211_send_nullfunc(struct ieee80211_local *local,
			     struct ieee80211_sub_if_data *sdata,
			     int powersave)
{
	struct sk_buff *skb;
	struct ieee80211_hdr_3addr *nullfunc;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;

	skb = ieee80211_nullfunc_get(&local->hw, &sdata->vif);
	if (!skb)
		return;

	nullfunc = (struct ieee80211_hdr_3addr *) skb->data;
	if (powersave)
		nullfunc->frame_control |= cpu_to_le16(IEEE80211_FCTL_PM);

	IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT;
	if (ifmgd->flags & (IEEE80211_STA_BEACON_POLL |
			    IEEE80211_STA_CONNECTION_POLL))
		IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_CTL_USE_MINRATE;

	ieee80211_tx_skb(sdata, skb);
}

void ieee80211_sta_rx_notify(struct ieee80211_sub_if_data *sdata,
			     struct ieee80211_hdr *hdr)
{
	/*
	 * We can postpone the mgd.timer whenever receiving unicast frames
	 * from AP because we know that the connection is working both ways
	 * at that time. But multicast frames (and hence also beacons) must
	 * be ignored here, because we need to trigger the timer during
	 * data idle periods for sending the periodic probe request to the
	 * AP we're connected to.
	 */
	if (is_multicast_ether_addr(hdr->addr1))
		return;

	ieee80211_sta_reset_conn_monitor(sdata);
}

static void ieee80211_reset_ap_probe(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;

	if (!(ifmgd->flags & (IEEE80211_STA_BEACON_POLL |
			      IEEE80211_STA_CONNECTION_POLL)))
	    return;

	ifmgd->flags &= ~(IEEE80211_STA_CONNECTION_POLL |
			  IEEE80211_STA_BEACON_POLL);
	mutex_lock(&sdata->local->iflist_mtx);
	ieee80211_recalc_ps(sdata->local, -1);
	mutex_unlock(&sdata->local->iflist_mtx);

	if (sdata->local->hw.flags & IEEE80211_HW_CONNECTION_MONITOR)
		return;

	/*
	 * We've received a probe response, but are not sure whether
	 * we have or will be receiving any beacons or data, so let's
	 * schedule the timers again, just in case.
	 */
	ieee80211_sta_reset_beacon_monitor(sdata);

	atbm_mod_timer(&ifmgd->conn_mon_timer,
		  round_jiffies_up(jiffies +
				   IEEE80211_CONNECTION_IDLE_TIME));
}

void ieee80211_sta_tx_notify(struct ieee80211_sub_if_data *sdata,
			     struct ieee80211_hdr *hdr, bool ack)
{
	if (!ieee80211_is_data(hdr->frame_control))
	    return;

	if (ack)
		ieee80211_sta_reset_conn_monitor(sdata);

	if (ieee80211_is_nullfunc(hdr->frame_control) &&
	    sdata->u.mgd.probe_send_count > 0) {
		if (ack)
			sdata->u.mgd.probe_send_count = 0;
		else
			sdata->u.mgd.nullfunc_failed = true;
		ieee80211_queue_work(&sdata->local->hw, &sdata->work);
	}
}

static void ieee80211_mgd_probe_ap_send(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	const u8 *ssid;
	u8 *dst = ifmgd->associated->bssid;
	u8 unicast_limit = max(1, max_probe_tries - 3);

	/*
	 * Try sending broadcast probe requests for the last three
	 * probe requests after the first ones failed since some
	 * buggy APs only support broadcast probe requests.
	 */
	if (ifmgd->probe_send_count >= unicast_limit)
		dst = NULL;

	/*
	 * When the hardware reports an accurate Tx ACK status, it's
	 * better to send a nullfunc frame instead of a probe request,
	 * as it will kick us off the AP quickly if we aren't associated
	 * anymore. The timeout will be reset if the frame is ACKed by
	 * the AP.
	 */
	ifmgd->probe_send_count++;
	if (sdata->local->hw.flags & IEEE80211_HW_REPORTS_TX_ACK_STATUS) {
		ifmgd->nullfunc_failed = false;
		ieee80211_send_nullfunc(sdata->local, sdata, 0);
	} else {
		ssid = ieee80211_bss_get_ie(ifmgd->associated, ATBM_WLAN_EID_SSID);
		ieee80211_send_probe_req(sdata, dst, ssid + 2, ssid[1], NULL, 0,
					 (u32) -1, true, false);
	}

	ifmgd->probe_timeout = jiffies + msecs_to_jiffies(probe_wait_ms);
	run_again(ifmgd, ifmgd->probe_timeout);
}

static void ieee80211_mgd_probe_ap(struct ieee80211_sub_if_data *sdata,
				   bool beacon)
{
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(sdata->local, sdata);
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	bool already = false;

	if (!ieee80211_sdata_running(sdata))
		return;

	if (sdata->local->scanning)
		return;

	if (chan_state->tmp_channel)
		return;

	mutex_lock(&ifmgd->mtx);

	if (!ifmgd->associated)
		goto out;

#ifdef CONFIG_MAC80211_ATBM_VERBOSE_DEBUG
	if (beacon && net_ratelimit())
		atbm_printk_debug("%s: detected beacon loss from AP "
		       "- sending probe request\n", sdata->name);
#endif

	/*
	 * The driver/our work has already reported this event or the
	 * connection monitoring has kicked in and we have already sent
	 * a probe request. Or maybe the AP died and the driver keeps
	 * reporting until we disassociate...
	 *
	 * In either case we have to ignore the current call to this
	 * function (except for setting the correct probe reason bit)
	 * because otherwise we would reset the timer every time and
	 * never check whether we received a probe response!
	 */
	if (ifmgd->flags & (IEEE80211_STA_BEACON_POLL |
			    IEEE80211_STA_CONNECTION_POLL))
		already = true;

	if (beacon)
		ifmgd->flags |= IEEE80211_STA_BEACON_POLL;
	else
		ifmgd->flags |= IEEE80211_STA_CONNECTION_POLL;

	if (already)
		goto out;

	mutex_lock(&sdata->local->iflist_mtx);
	ieee80211_recalc_ps(sdata->local, -1);
	mutex_unlock(&sdata->local->iflist_mtx);

	ifmgd->probe_send_count = 0;
	ieee80211_mgd_probe_ap_send(sdata);
 out:
	mutex_unlock(&ifmgd->mtx);
}

static void ieee80211_sta_timer(unsigned long data)
{
	struct ieee80211_sub_if_data *sdata =
		(struct ieee80211_sub_if_data *) data;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_local *local = sdata->local;

	if (local->quiescing) {
		set_bit(TMR_RUNNING_TIMER, &ifmgd->timers_running);
		return;
	}

	ieee80211_queue_work(&local->hw, &sdata->work);
}

void ieee80211_sta_work(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_vif *vif = &sdata->vif;
	struct ieee80211_bss_conf *bss_conf = &vif->bss_conf;

	/* then process the rest of the work */
	mutex_lock(&ifmgd->mtx);

	if (ifmgd->flags & (IEEE80211_STA_BEACON_POLL |
			    IEEE80211_STA_CONNECTION_POLL) &&
	    ifmgd->associated) {
		u8 bssid[ETH_ALEN];
		int max_tries;

		memcpy(bssid, ifmgd->associated->bssid, ETH_ALEN);

		if (local->hw.flags & IEEE80211_HW_REPORTS_TX_ACK_STATUS)
			max_tries = max_nullfunc_tries;
		else
			max_tries = max_probe_tries;

		/* ACK received for nullfunc probing frame */
		if (!ifmgd->probe_send_count)
			ieee80211_reset_ap_probe(sdata);
		else if (ifmgd->nullfunc_failed) {
			if (ifmgd->probe_send_count < max_tries) {
#ifdef CONFIG_MAC80211_ATBM_VERBOSE_DEBUG
				wiphy_debug(local->hw.wiphy,
					    "%s: No ack for nullfunc frame to"
					    " AP %pM, try %d/%i\n",
					    sdata->name, bssid,
					    ifmgd->probe_send_count, max_tries);
#endif
				ieee80211_mgd_probe_ap_send(sdata);
			} else {
#ifdef CONFIG_MAC80211_ATBM_VERBOSE_DEBUG
				wiphy_debug(local->hw.wiphy,
					    "%s: No ack for nullfunc frame to"
					    " AP %pM, disconnecting.\n",
					    sdata->name, bssid);
#endif
				ieee80211_sta_connection_lost(sdata, bssid,
					WLAN_REASON_DISASSOC_DUE_TO_INACTIVITY);
			}
		} else if (time_is_after_jiffies(ifmgd->probe_timeout))
			run_again(ifmgd, ifmgd->probe_timeout);
		else if (local->hw.flags & IEEE80211_HW_REPORTS_TX_ACK_STATUS) {
#ifdef CONFIG_MAC80211_ATBM_VERBOSE_DEBUG
			wiphy_debug(local->hw.wiphy,
				    "%s: Failed to send nullfunc to AP %pM"
				    " after %dms, disconnecting.\n",
				    sdata->name,
				    bssid, probe_wait_ms);
#endif
			ieee80211_sta_connection_lost(sdata, bssid,
				WLAN_REASON_DISASSOC_DUE_TO_INACTIVITY);
		} else if (ifmgd->probe_send_count < max_tries) {
#ifdef CONFIG_MAC80211_ATBM_VERBOSE_DEBUG
			wiphy_debug(local->hw.wiphy,
				    "%s: No probe response from AP %pM"
				    " after %dms, try %d/%i\n",
				    sdata->name,
				    bssid, probe_wait_ms,
				    ifmgd->probe_send_count, max_tries);
#endif
			ieee80211_mgd_probe_ap_send(sdata);
		/* STE: Changed to follow STE beacon_miss/beacon_loss design */
		} else if (!(bss_conf->cqm_beacon_miss_thold) &&
			   !(bss_conf->cqm_tx_fail_thold)) {
			/*
			 * We actually lost the connection ... or did we?
			 * Let's make sure!
			 */
			wiphy_debug(local->hw.wiphy,
				    "%s: No probe response from AP %pM"
				    " after %dms, disconnecting.\n",
				    sdata->name,
				    bssid, probe_wait_ms);

			ieee80211_sta_connection_lost(sdata, bssid,
				WLAN_REASON_DISASSOC_DUE_TO_INACTIVITY);
		}
	}

	mutex_unlock(&ifmgd->mtx);
}

static void ieee80211_sta_bcn_mon_timer(unsigned long data)
{
	struct ieee80211_sub_if_data *sdata =
		(struct ieee80211_sub_if_data *) data;
	struct ieee80211_local *local = sdata->local;

	if (local->quiescing)
		return;

	ieee80211_queue_work(&sdata->local->hw,
			     &sdata->u.mgd.beacon_connection_loss_work);
}

static void ieee80211_sta_conn_mon_timer(unsigned long data)
{
	struct ieee80211_sub_if_data *sdata =
		(struct ieee80211_sub_if_data *) data;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_local *local = sdata->local;

	if (local->quiescing)
		return;

	ieee80211_queue_work(&local->hw, &ifmgd->monitor_work);
}

static void ieee80211_sta_monitor_work(struct atbm_work_struct *work)
{
	struct ieee80211_sub_if_data *sdata =
		container_of(work, struct ieee80211_sub_if_data,
			     u.mgd.monitor_work);

	ieee80211_mgd_probe_ap(sdata, false);
}

static void ieee80211_restart_sta_timer(struct ieee80211_sub_if_data *sdata, bool monitor)
{
	if (sdata->vif.type == NL80211_IFTYPE_STATION) {
		if (monitor) {
			sdata->u.mgd.flags &= ~(IEEE80211_STA_BEACON_POLL |
					IEEE80211_STA_CONNECTION_POLL);

			/* let's probe the connection once */
			ieee80211_queue_work(&sdata->local->hw,
				   &sdata->u.mgd.monitor_work);
		}
		/* and do all the other regular work too */
		ieee80211_queue_work(&sdata->local->hw, &sdata->work);
	}
}

#endif
static int ecw2cw(int ecw)
{
	return (1 << ecw) - 1;
}

/*
 * ieee80211_enable_ht should be called only after the operating band
 * has been determined as ht configuration depends on the hw's
 * HT abilities for a specific band.
 */
static u32 ieee80211_enable_ht(struct ieee80211_sub_if_data *sdata,
			       struct ieee80211_ht_info *hti,
			       const u8 *bssid, u16 ap_ht_cap_flags,
			       bool beacon_htcap_ie)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(local, sdata);
	struct ieee80211_supported_band *sband;
	struct sta_info *sta;
	u32 changed = 0;
	int hti_cfreq;
	u16 ht_opmode;
	bool enable_ht = true;
	enum nl80211_channel_type prev_chantype;
	enum nl80211_channel_type channel_type = NL80211_CHAN_NO_HT;

	sband = local->hw.wiphy->bands[chan_state->conf.channel->band];

	prev_chantype = sdata->vif.bss_conf.channel_type;

	/* HT is not supported */
	if (!sband->ht_cap.ht_supported)
		enable_ht = false;

	if (enable_ht) {
		hti_cfreq = ieee80211_channel_to_frequency(hti->control_chan,
							   sband->band);
		/* check that channel matches the right operating channel */
		if (channel_center_freq(chan_state->conf.channel) != hti_cfreq) {
			/* Some APs mess this up, evidently.
			 * Netgear WNDR3700 sometimes reports 4 higher than
			 * the actual channel, for instance.
			 */
			atbm_printk_err(
			       "%s: Wrong control channel in association"
			       " response: configured center-freq: %d"
			       " hti-cfreq: %d  hti->control_chan: %d"
			       " band: %d.  Disabling HT.\n",
			       sdata->name,
			       channel_center_freq(chan_state->conf.channel),
			       hti_cfreq, hti->control_chan,
			       sband->band);
			enable_ht = false;
		}
	}

	if (enable_ht) {
		channel_type = NL80211_CHAN_HT20;

		if (!(ap_ht_cap_flags & IEEE80211_HT_CAP_40MHZ_INTOLERANT) &&
		    (sband->ht_cap.cap & IEEE80211_HT_CAP_SUP_WIDTH_20_40) &&
		    (hti->ht_param & IEEE80211_HT_PARAM_CHAN_WIDTH_ANY)) {
			switch(hti->ht_param & IEEE80211_HT_PARAM_CHA_SEC_OFFSET) {
			case IEEE80211_HT_PARAM_CHA_SEC_ABOVE:
				if (!(chan_state->conf.channel->flags &
				    IEEE80211_CHAN_NO_HT40PLUS))
					channel_type = NL80211_CHAN_HT40PLUS;
				break;
			case IEEE80211_HT_PARAM_CHA_SEC_BELOW:
				if (!(chan_state->conf.channel->flags &
				    IEEE80211_CHAN_NO_HT40MINUS))
					channel_type = NL80211_CHAN_HT40MINUS;
				break;
			}
		}
	}

	if (chan_state->tmp_channel)
		chan_state->tmp_channel_type = channel_type;

	if (!ieee80211_set_channel_type(local, sdata, channel_type)) {
		/* can only fail due to HT40+/- mismatch */
		channel_type = NL80211_CHAN_HT20;
		WARN_ON(!ieee80211_set_channel_type(local, sdata, channel_type));
	}

	if (beacon_htcap_ie && (prev_chantype != channel_type)) {
		/*
		 * Whenever the AP announces the HT mode change that can be
		 * 40MHz intolerant or etc., it would be safer to stop tx
		 * queues before doing hw config to avoid buffer overflow.
		 */
		ieee80211_stop_queues_by_reason(&sdata->local->hw,
				IEEE80211_QUEUE_STOP_REASON_CHTYPE_CHANGE);

		/* flush out all packets */
		synchronize_net();

		drv_flush(local, sdata, false);
	}

	/* channel_type change automatically detected */
	ieee80211_hw_config(local, 0);

	if (prev_chantype != channel_type) {
		rcu_read_lock();
		sta = sta_info_get(sdata, bssid);
		if (sta){
#ifndef CONFIG_RATE_HW_CONTROL
			rate_control_rate_update(local, sband, sta,
						 IEEE80211_RC_HT_CHANGED,
						 channel_type);
#endif
			ieee80211_ht_cap_to_sta_channel_type(sta);
		}
		rcu_read_unlock();

		if (beacon_htcap_ie)
			ieee80211_wake_queues_by_reason(&sdata->local->hw,
				IEEE80211_QUEUE_STOP_REASON_CHTYPE_CHANGE);
	}

	ht_opmode = le16_to_cpu(hti->operation_mode);

	/* if bss configuration changed store the new one */
	if (sdata->ht_opmode_valid != enable_ht ||
	    sdata->vif.bss_conf.ht_operation_mode != ht_opmode ||
	    prev_chantype != channel_type) {
		changed |= BSS_CHANGED_HT;
		sdata->vif.bss_conf.ht_operation_mode = ht_opmode;
		sdata->ht_opmode_valid = enable_ht;
	}

	return changed;
}

/* frame sending functions */

static void ieee80211_send_deauth_disassoc(struct ieee80211_sub_if_data *sdata,
					   const u8 *bssid, u16 stype, u16 reason,
					   void *cookie, bool send_frame)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct sk_buff *skb;
	struct atbm_ieee80211_mgmt *mgmt;

	skb = atbm_dev_alloc_skb(local->hw.extra_tx_headroom + sizeof(*mgmt));
	if (!skb)
		return;

	atbm_skb_reserve(skb, local->hw.extra_tx_headroom);

	mgmt = (struct atbm_ieee80211_mgmt *) atbm_skb_put(skb, 24);
	memset(mgmt, 0, 24);
	memcpy(mgmt->da, bssid, ETH_ALEN);
	memcpy(mgmt->sa, sdata->vif.addr, ETH_ALEN);
	memcpy(mgmt->bssid, bssid, ETH_ALEN);
	mgmt->frame_control = cpu_to_le16(IEEE80211_FTYPE_MGMT | stype);
	atbm_skb_put(skb, 2);
	/* u.deauth.reason_code == u.disassoc.reason_code */
	mgmt->u.deauth.reason_code = cpu_to_le16(reason);
	#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,11,0))	
	if (stype == IEEE80211_STYPE_DEAUTH)
		if (cookie)
			__cfg80211_send_deauth(sdata->dev, (u8 *)mgmt, skb->len);
		else
			cfg80211_send_deauth(sdata->dev, (u8 *)mgmt, skb->len);
	else
		if (cookie)
			__cfg80211_send_disassoc(sdata->dev, (u8 *)mgmt, skb->len);
		else
			cfg80211_send_disassoc(sdata->dev, (u8 *)mgmt, skb->len);
	#else
	/*
	cfg80211_tx_mlme_mgmt(sdata->dev, frame_buf,
				      IEEE80211_DEAUTH_FRAME_LEN);
	*/
	if (cookie)
		cfg80211_tx_mlme_mgmt(sdata->dev, (u8 *)mgmt, skb->len);
	else
	{
		struct wireless_dev *wdev = sdata->dev->ieee80211_ptr;
		
		atbm_wdev_lock(wdev);
		cfg80211_tx_mlme_mgmt(sdata->dev, (u8 *)mgmt, skb->len);
		atbm_wdev_unlock(wdev);
	}
	#endif
	if (!(ifmgd->flags & IEEE80211_STA_MFP_ENABLED))
		IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT;

	if (send_frame)
		ieee80211_tx_skb(sdata, skb);
	else
		atbm_kfree_skb(skb);
}
#ifdef CONFIG_ATBM_MAC80211_NO_USE
void ieee80211_send_pspoll(struct ieee80211_local *local,
			   struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_pspoll *pspoll;
	struct sk_buff *skb;

	skb = ieee80211_pspoll_get(&local->hw, &sdata->vif);
	if (!skb)
		return;

	pspoll = (struct ieee80211_pspoll *) skb->data;
	pspoll->frame_control |= cpu_to_le16(IEEE80211_FCTL_PM);

	IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT;
	ieee80211_tx_skb(sdata, skb);
}
#endif


#ifdef CONFIG_ATBM_4ADDR
static void ieee80211_send_4addr_nullfunc(struct ieee80211_local *local,
					  struct ieee80211_sub_if_data *sdata)
{
	struct sk_buff *skb;
	struct ieee80211_hdr *nullfunc;
	__le16 fc;

	if (WARN_ON(sdata->vif.type != NL80211_IFTYPE_STATION))
		return;

	skb = atbm_dev_alloc_skb(local->hw.extra_tx_headroom + 30);
	if (!skb)
		return;

	atbm_skb_reserve(skb, local->hw.extra_tx_headroom);

	nullfunc = (struct ieee80211_hdr *) atbm_skb_put(skb, 30);
	memset(nullfunc, 0, 30);
	fc = cpu_to_le16(IEEE80211_FTYPE_DATA | IEEE80211_STYPE_NULLFUNC |
			 IEEE80211_FCTL_FROMDS | IEEE80211_FCTL_TODS);
	nullfunc->frame_control = fc;
	memcpy(nullfunc->addr1, sdata->u.mgd.bssid, ETH_ALEN);
	memcpy(nullfunc->addr2, sdata->vif.addr, ETH_ALEN);
	memcpy(nullfunc->addr3, sdata->u.mgd.bssid, ETH_ALEN);
	memcpy(nullfunc->addr4, sdata->vif.addr, ETH_ALEN);

	IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT;
	ieee80211_tx_skb(sdata, skb);
}
#endif
/* spectrum management related things */
#ifdef CONFIG_ATBM_SUPPORT_CHANSWITCH
static void ieee80211_chswitch_work(struct atbm_work_struct *work)
{
	struct ieee80211_sub_if_data *sdata =
		container_of(work, struct ieee80211_sub_if_data, u.mgd.chswitch_work);
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(sdata->local, sdata);
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
#ifdef ATBM_SUPPORT_WIDTH_40M
	enum nl80211_channel_type prev_channel_type;
#endif
	if (!ieee80211_sdata_running(sdata))
		return;

	mutex_lock(&ifmgd->mtx);
	if (!ifmgd->associated)
		goto out;

	chan_state->oper_channel = chan_state->csa_channel;
	if (!sdata->local->ops->channel_switch) {
#ifdef ATBM_SUPPORT_WIDTH_40M
		prev_channel_type = sdata->local->chan_state.conf.channel_type;
#endif
		/* call "hw_config" only if doing sw channel switch */
		ieee80211_hw_config(sdata->local,
			IEEE80211_CONF_CHANGE_CHANNEL);
#ifdef ATBM_SUPPORT_WIDTH_40M
		/*
		*if channel type has been changed,update the rate ctrl
		*/
		if(prev_channel_type != sdata->local->chan_state.conf.channel_type)
		{
			struct sta_info *sta;
			struct ieee80211_supported_band *sband;
			sband = sdata->local->hw.wiphy->bands[chan_state->conf.channel->band];

			rcu_read_lock();
			sta = sta_info_get(sdata, ifmgd->associated->bssid);
#ifndef CONFIG_RATE_HW_CONTROL
			if (sta)
				rate_control_rate_update(sdata->local, sband, sta,
							 IEEE80211_RC_HT_CHANGED,
							 sdata->local->chan_state.conf.channel_type);
#endif
			rcu_read_unlock();
		}
#endif
	} else {
		/* update the device channel directly */
		chan_state->conf.channel = chan_state->oper_channel;
	}

	/* XXX: shouldn't really modify cfg80211-owned data! */
	ifmgd->associated->channel = chan_state->oper_channel;

	ieee80211_wake_queues_by_reason(&sdata->local->hw,
					IEEE80211_QUEUE_STOP_REASON_CSA);
 out:
	ifmgd->flags &= ~IEEE80211_STA_CSA_RECEIVED;
	mutex_unlock(&ifmgd->mtx);
}
#ifdef CONFIG_ATBM_MAC80211_NO_USE
void ieee80211_chswitch_done(struct ieee80211_vif *vif, bool success)
{
	struct ieee80211_channel_state *chan_state;
	struct ieee80211_sub_if_data *sdata;
	struct ieee80211_if_managed *ifmgd;

	sdata = vif_to_sdata(vif);
	chan_state = ieee80211_get_channel_state(sdata->local, sdata);
	ifmgd = &sdata->u.mgd;

	trace_api_chswitch_done(sdata, success);
	if (!success) {
		/*
		 * If the channel switch was not successful, stay
		 * around on the old channel. We currently lack
		 * good handling of this situation, possibly we
		 * should just drop the association.
		 */
		chan_state->csa_channel = chan_state->oper_channel;
	}

	ieee80211_queue_work(&sdata->local->hw, &ifmgd->chswitch_work);
}
//EXPORT_SYMBOL(ieee80211_chswitch_done);
#endif
#ifdef CONFIG_ATBM_SUPPORT_CHANSWITCH
static void ieee80211_chswitch_timer(unsigned long data)
{
	struct ieee80211_sub_if_data *sdata =
		(struct ieee80211_sub_if_data *) data;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;

	if (sdata->local->quiescing) {
		set_bit(TMR_RUNNING_CHANSW, &ifmgd->timers_running);
		return;
	}

	ieee80211_queue_work(&sdata->local->hw, &ifmgd->chswitch_work);
}
#endif
/*
static void  ieee80211_scan_delay_timer(unsigned long data)
{
	printk("Start BG Scan\n");
//	g_connetting = 0;
}
*/
#ifdef ATBM_SUPPORT_WIDTH_40M

void ieee80211_sta_process_chanswitch(struct ieee80211_sub_if_data *sdata,
					  struct atbm_ieee80211_channel_sw_packed_ie *sw_packed_ie,
					  struct ieee80211_bss *bss,
					  u64 timestamp)
{
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(sdata->local, sdata);
	struct cfg80211_bss *cbss =
		container_of((void *)bss, struct cfg80211_bss, priv);
	struct ieee80211_channel *new_ch;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_channel_sw_ie *sw_elem = sw_packed_ie->chan_sw_ie;
	struct atbm_ieee80211_ext_chansw_ie *ex_sw_elem = sw_packed_ie->ex_chan_sw_ie;
	struct atbm_ieee80211_sec_chan_offs_ie *sec_chan_ie = sw_packed_ie->sec_chan_offs_ie;
	enum ieee80211_band new_band = IEEE80211_NUM_BANDS;
	#define CHANGE_SW_CHANNEL_BIT		(1<<0)
	#define CHANGE_CHANNEL_TYPE_BIT		(1<<1)
	u8 change = 0;
	u8 new_ch_num = 0;
	int new_freq;
	u8 mode;
	u8 count;	
	ASSERT_MGD_MTX(ifmgd);

	if (!ifmgd->associated)
		return;

	if (sdata->local->scanning)
		return;

	/* Disregard subsequent beacons if we are already running a timer
	   processing a CSA */

	if (ifmgd->flags & IEEE80211_STA_CSA_RECEIVED)
		return;
	
	if(ex_sw_elem)
	{
		if(
			(ex_sw_elem->new_operaring_class == 112)
			||
			(
			  (ex_sw_elem->new_operaring_class>=115)
			  &&
			  (ex_sw_elem->new_operaring_class<=130)
			)
		  )
		{
			new_band = IEEE80211_BAND_5GHZ;
		}
		else if
			( 
			  (ex_sw_elem->new_operaring_class>=81)
			  &&
			  (ex_sw_elem->new_operaring_class<=84)
			)
		{
			new_band = IEEE80211_BAND_2GHZ;
		}

		new_ch_num = ex_sw_elem->new_ch_num;
		mode = ex_sw_elem->mode;
		count = ex_sw_elem->count;
		change |=  CHANGE_SW_CHANNEL_BIT;
	}
	else if(sw_elem)
	{
		new_band =  cbss->channel->band;
		new_ch_num = sw_elem->new_ch_num;
		mode = sw_elem->mode;
		count = sw_elem->count;
		change |=  CHANGE_SW_CHANNEL_BIT;
	}
	else
	{
		/*
		*without ex_sw_elem,sw_elem. do not change the channel
		*/
		new_band =  cbss->channel->band;
		new_ch_num = channel_hw_value(chan_state->conf.channel);
		mode = 0;
		count = 0;
		change &=  ~CHANGE_SW_CHANNEL_BIT;
	}
		

	new_freq = ieee80211_channel_to_frequency(new_ch_num,new_band);

	new_ch = ieee80211_get_channel(sdata->local->hw.wiphy, new_freq);
	if (!new_ch || new_ch->flags & IEEE80211_CHAN_DISABLED)
		return;

	chan_state->csa_channel = new_ch;

	if(sec_chan_ie)
	{
		enum nl80211_channel_type prev_chantype = chan_state->conf.channel_type;
		enum nl80211_channel_type new_chantype = prev_chantype;
		switch(sec_chan_ie->sec_chan_offs)
		{
			case IEEE80211_HT_PARAM_CHA_SEC_NONE:
				new_chantype = NL80211_CHAN_NO_HT;
				break;
			case IEEE80211_HT_PARAM_CHA_SEC_ABOVE:
				new_chantype = NL80211_CHAN_HT40PLUS;
				break;
			case IEEE80211_HT_PARAM_CHA_SEC_BELOW:
				new_chantype = NL80211_CHAN_HT40MINUS;
				break;
			default:
				break;
		}
		
		if(prev_chantype == new_chantype)
			return;
		change |=  CHANGE_CHANNEL_TYPE_BIT;
		ieee80211_set_channel_type(sdata->local,sdata,new_chantype);
		return;
	}
	
	if(!change)
	{
		return;
	}
#if 0	
	if (sdata->local->ops->channel_switch) {
		/* use driver's channel switch callback */
		struct ieee80211_channel_switch ch_switch;
		memset(&ch_switch, 0, sizeof(ch_switch));
		ch_switch.timestamp = timestamp;
		if (mode) {
			ch_switch.block_tx = true;
			ieee80211_stop_queues_by_reason(&sdata->local->hw,
					IEEE80211_QUEUE_STOP_REASON_CSA);
		}
		ch_switch.channel = new_ch;
		ch_switch.count =count;
		ifmgd->flags |= IEEE80211_STA_CSA_RECEIVED;
		drv_channel_switch(sdata->local, &ch_switch);
		return;
	}
#endif	
	/* channel switch handled in software */
	if (count <= 1) {
		ieee80211_queue_work(&sdata->local->hw, &ifmgd->chswitch_work);
	} else {
		if (mode)
			ieee80211_stop_queues_by_reason(&sdata->local->hw,
					IEEE80211_QUEUE_STOP_REASON_CSA);
		ifmgd->flags |= IEEE80211_STA_CSA_RECEIVED;
		atbm_mod_timer(&ifmgd->chswitch_timer,
			  jiffies +
			  msecs_to_jiffies(mode *
					   cbss->beacon_interval));
	}
}
#else
void ieee80211_sta_process_chanswitch(struct ieee80211_sub_if_data *sdata,
				      struct ieee80211_channel_sw_ie *sw_elem,
				      struct ieee80211_bss *bss,
				      u64 timestamp)
{
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(sdata->local, sdata);
	struct cfg80211_bss *cbss =
		container_of((void *)bss, struct cfg80211_bss, priv);
	struct ieee80211_channel *new_ch;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	int new_freq = ieee80211_channel_to_frequency(sw_elem->new_ch_num,
						      cbss->channel->band);

	ASSERT_MGD_MTX(ifmgd);

	if (!ifmgd->associated)
		return;

	if (sdata->local->scanning)
		return;

	/* Disregard subsequent beacons if we are already running a timer
	   processing a CSA */

	if (ifmgd->flags & IEEE80211_STA_CSA_RECEIVED)
		return;

	new_ch = ieee80211_get_channel(sdata->local->hw.wiphy, new_freq);
	if (!new_ch || new_ch->flags & IEEE80211_CHAN_DISABLED)
		return;

	chan_state->csa_channel = new_ch;

	if (sdata->local->ops->channel_switch) {
		/* use driver's channel switch callback */
		struct ieee80211_channel_switch ch_switch;
		memset(&ch_switch, 0, sizeof(ch_switch));
		ch_switch.timestamp = timestamp;
		if (sw_elem->mode) {
			ch_switch.block_tx = true;
			ieee80211_stop_queues_by_reason(&sdata->local->hw,
					IEEE80211_QUEUE_STOP_REASON_CSA);
		}
		ch_switch.channel = new_ch;
		ch_switch.count = sw_elem->count;
		ifmgd->flags |= IEEE80211_STA_CSA_RECEIVED;
		drv_channel_switch(sdata->local, &ch_switch);
		return;
	}

	/* channel switch handled in software */
	if (sw_elem->count <= 1) {
		ieee80211_queue_work(&sdata->local->hw, &ifmgd->chswitch_work);
	} else {
		if (sw_elem->mode)
			ieee80211_stop_queues_by_reason(&sdata->local->hw,
					IEEE80211_QUEUE_STOP_REASON_CSA);
		ifmgd->flags |= IEEE80211_STA_CSA_RECEIVED;
		atbm_mod_timer(&ifmgd->chswitch_timer,
			  jiffies +
			  msecs_to_jiffies(sw_elem->count *
					   cbss->beacon_interval));
	}
}
#endif
#endif
static void ieee80211_handle_pwr_constr(struct ieee80211_sub_if_data *sdata,
					u16 capab_info, u8 *pwr_constr_elem,
					u8 pwr_constr_elem_len)
{
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(sdata->local, sdata);

	if (!(capab_info & WLAN_CAPABILITY_SPECTRUM_MGMT))
		return;

	/* Power constraint IE length should be 1 octet */
	if (pwr_constr_elem_len != 1)
		return;

	if ((*pwr_constr_elem <= chan_state->conf.channel->max_power) &&
	    (*pwr_constr_elem != sdata->local->power_constr_level)) {
		sdata->local->power_constr_level = *pwr_constr_elem;
		ieee80211_hw_config(sdata->local, 0);
	}
}
#ifdef CONFIG_ATBM_MAC80211_NO_USE
void ieee80211_enable_dyn_ps(struct ieee80211_vif *vif)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_bss_conf *conf = &sdata->vif.bss_conf;

	WARN_ON(sdata->vif.type != NL80211_IFTYPE_STATION ||
		!(local->hw.flags & IEEE80211_HW_SUPPORTS_PS) ||
		(local->hw.flags & IEEE80211_HW_SUPPORTS_DYNAMIC_PS));

	sdata->disable_dynamic_ps = false;
	conf->dynamic_ps_timeout = sdata->dynamic_ps_user_timeout;
}
//EXPORT_SYMBOL(ieee80211_enable_dyn_ps);

void ieee80211_disable_dyn_ps(struct ieee80211_vif *vif)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_bss_conf *conf = &sdata->vif.bss_conf;

	WARN_ON(sdata->vif.type != NL80211_IFTYPE_STATION ||
		!(local->hw.flags & IEEE80211_HW_SUPPORTS_PS) ||
		(local->hw.flags & IEEE80211_HW_SUPPORTS_DYNAMIC_PS));

	sdata->disable_dynamic_ps = true;
	conf->dynamic_ps_timeout = 0;
	atbm_del_timer_sync(&sdata->dynamic_ps_timer);
	ieee80211_queue_work(&local->hw,
			     &sdata->dynamic_ps_enable_work);
}
//EXPORT_SYMBOL(ieee80211_disable_dyn_ps);
#endif
/* powersave */
static void ieee80211_enable_ps(struct ieee80211_local *local,
				struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_bss_conf *conf = &sdata->vif.bss_conf;

	/*
	 * If we are scanning right now then the parameters will
	 * take effect when scan finishes.
	 */
	if (local->scanning) /* XXX: investigate this codepath */
		return;
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	if (conf->dynamic_ps_timeout > 0 &&
	    !(local->hw.flags & IEEE80211_HW_SUPPORTS_DYNAMIC_PS)) {
		atbm_mod_timer(&sdata->dynamic_ps_timer, jiffies +
			  msecs_to_jiffies(conf->dynamic_ps_timeout));
	} else {
		if (local->hw.flags & IEEE80211_HW_PS_NULLFUNC_STACK)
			ieee80211_send_nullfunc(local, sdata, 1);

		if ((local->hw.flags & IEEE80211_HW_PS_NULLFUNC_STACK) &&
		    (local->hw.flags & IEEE80211_HW_REPORTS_TX_ACK_STATUS))
			return;

		conf->ps_enabled = true;
		ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_PS);
	}
#else
	conf->ps_enabled = true;
	ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_PS);
#endif
}

static void ieee80211_change_ps(struct ieee80211_local *local)
{
	struct ieee80211_sub_if_data *sdata;

	/* XXX: This needs to be verified */
	list_for_each_entry(sdata, &local->interfaces, list) {
		atbm_printk_debug("%s:ps_allowed(%d)(%d)\n",sdata->name,sdata->ps_allowed,sdata->vif.bss_conf.ps_enabled);
		if (sdata->ps_allowed) {
			atbm_printk_debug("%s:ps enable\n",sdata->name);
			ieee80211_enable_ps(local, sdata);
		} else if (sdata->vif.bss_conf.ps_enabled) {
			sdata->vif.bss_conf.ps_enabled = false;
			atbm_printk_debug("%s:ps disable\n",sdata->name);
			ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_PS);
#ifdef CONFIG_ATBM_MAC80211_NO_USE
			atbm_del_timer_sync(&sdata->dynamic_ps_timer);
			atbm_cancel_work_sync(&sdata->dynamic_ps_enable_work);
#endif
		}
	}
}

static bool ieee80211_powersave_allowed(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_if_managed *mgd = &sdata->u.mgd;
	struct sta_info *sta = NULL;
	bool authorized = false;
	
#ifdef CONFIG_ATBM_SMPS
	if (!mgd->powersave)
		return false;
#endif
	if (mgd->broken_ap)
		return false;

	if (!mgd->associated)
		return false;

	if (!mgd->associated->beacon_ies)
		return false;

	if (mgd->flags & (IEEE80211_STA_BEACON_POLL |
			  IEEE80211_STA_CONNECTION_POLL))
		return false;
	
#ifdef CONFIG_ATBM_STA_DYNAMIC_PS
	if(sdata->traffic.current_tx_tp + sdata->traffic.current_rx_tp >= IEEE80211_KEEP_WAKEUP_TP_PER_SECOND){
		atbm_printk_debug("%s:keep wakeup\n",__func__);
		return false;
	}
#endif

	rcu_read_lock();
	sta = sta_info_get(sdata, mgd->bssid);
	if (sta)
		authorized = test_sta_flag(sta, WLAN_STA_AUTHORIZED);
	rcu_read_unlock();

	return authorized;
}

/* need to hold RTNL or interface lock */
void ieee80211_recalc_ps_vif(struct ieee80211_local *local, s32 latency)
{
	struct ieee80211_sub_if_data *sdata;
	struct ieee80211_bss_conf *conf;
	int timeout;
	s32 beaconint_us;
	
	list_for_each_entry(sdata, &local->interfaces, list) {
		if (!ieee80211_sdata_running(sdata))
			continue;
		if (sdata->vif.type == NL80211_IFTYPE_AP) {
			/* If an AP vif is found, then disable PS
			 * by setting the count to zero thereby setting
			 * ps_sdata to NULL.
			 */
			//count = 0;
			//break;
			/* XXX: I don't think I understand the above. Any ideas? */
		}
		if (sdata->vif.type != NL80211_IFTYPE_STATION)
			continue;
		if (!ieee80211_powersave_allowed(sdata)) {
			sdata->ps_allowed = false;
			continue;
		}

		conf = &sdata->vif.bss_conf;
		beaconint_us = ieee80211_tu_to_usec(
					sdata->vif.bss_conf.beacon_int);
#ifdef CONFIG_ATBM_MAC80211_NO_USE
#ifdef CONFIG_ATBM_PM_QOS
		if (latency < 0)
			latency = pm_qos_request(PM_QOS_NETWORK_LATENCY);
#else
		latency = beaconint_us*2;		
#endif
		timeout = sdata->dynamic_ps_forced_timeout;
		if (timeout < 0) {
			/*
			 * Go to full PSM if the user configures a very low
			 * latency requirement.
			 * The 2000 second value is there for compatibility
			 * until the PM_QOS_NETWORK_LATENCY is configured
			 * with real values.
			 */
			if (latency > (1900 * USEC_PER_MSEC) &&
				latency != (2000 * USEC_PER_SEC))
				timeout = 0;
			else
				timeout = 100;
		}
		sdata->dynamic_ps_user_timeout = timeout;
		if (!sdata->disable_dynamic_ps)
			conf->dynamic_ps_timeout =
				sdata->dynamic_ps_user_timeout;

		if (beaconint_us > latency) {
			sdata->ps_allowed = false;
		} else {
			struct ieee80211_bss *bss;
			int maxslp = 1;
			u8 dtimper;

			bss = (void *)sdata->u.mgd.associated->priv;
			dtimper = bss->dtim_period;

			/* If the TIM IE is invalid, pretend the value is 1 */
			if (!dtimper)
				dtimper = 1;
			else if (dtimper > 1)
				maxslp = min_t(int, dtimper,
							latency / beaconint_us);

			sdata->vif.bss_conf.max_sleep_period = maxslp;
			sdata->vif.bss_conf.ps_dtim_period = dtimper;
			sdata->ps_allowed = true;
		}
#else
		timeout = (latency > 0 && latency <beaconint_us) ? latency:beaconint_us;
		
		sdata->dynamic_ps_user_timeout = timeout;
		conf->dynamic_ps_timeout	   = timeout;
		
		atbm_printk_debug("%s:beaconint_us(%x)\n",__func__,beaconint_us);
		{
			struct ieee80211_bss *bss;
			int maxslp = 1;
			u8 dtimper;

			bss = (void *)sdata->u.mgd.associated->priv;
			dtimper = bss->dtim_period;

			/* If the TIM IE is invalid, pretend the value is 1 */
			if (!dtimper)
				dtimper = 1;
			else if (dtimper > 1)
				maxslp = min_t(int, dtimper,
							latency / beaconint_us);

			sdata->vif.bss_conf.max_sleep_period = maxslp;
			sdata->vif.bss_conf.ps_dtim_period = dtimper;
			sdata->ps_allowed = true;
		}
#endif
	}
	ieee80211_change_ps(local);
}
void ieee80211_recalc_ps(struct ieee80211_local *local, s32 latency)
{
	struct ieee80211_sub_if_data *sdata;
	struct ieee80211_work *wk;

	if (!(local->hw.flags & IEEE80211_HW_SUPPORTS_PS)) {
		list_for_each_entry(sdata, &local->interfaces, list)
			sdata->ps_allowed = false;
		return;
	}

	if (!list_empty(&local->work_list)) {
		int busy_work = 0;
		list_for_each_entry(wk, &local->work_list, list){
#ifdef CONFIG_ATBM_STA_DYNAMIC_PS
			if(wk->type != IEEE80211_WORK_PS_RECAL){
				wk->sdata->ps_allowed = false;
				busy_work++;
			}
#else
			busy_work++;
#endif
		}
		if(busy_work){
			atbm_printk_err("%s:work busy\n",__func__);
			goto change;
		}
	}
	
	ieee80211_recalc_ps_vif(local,latency);
	return;
 change:
	ieee80211_change_ps(local);
}
#ifdef CONFIG_ATBM_MAC80211_NO_USE

void ieee80211_dynamic_ps_disable_work(struct atbm_work_struct *work)
{
	struct ieee80211_sub_if_data *sdata =
		container_of(work, struct ieee80211_sub_if_data,
			     dynamic_ps_disable_work);

	if (sdata->vif.bss_conf.ps_enabled) {
		sdata->vif.bss_conf.ps_enabled = false;
		ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_PS);
	}

	// XXX: with queue patches this should be updated:
	ieee80211_wake_queues_by_reason(&sdata->local->hw,
					IEEE80211_QUEUE_STOP_REASON_PS);
}

void ieee80211_dynamic_ps_enable_work(struct atbm_work_struct *work)
{
	struct ieee80211_sub_if_data *sdata =
		container_of(work, struct ieee80211_sub_if_data,
			     dynamic_ps_enable_work);
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_if_managed *ifmgd;
	unsigned long flags;
	int q;

	/* can only happen when PS was just disabled anyway */
	if (!sdata->ps_allowed)
		return;

	ifmgd = &sdata->u.mgd;

	if (sdata->vif.bss_conf.ps_enabled)
		return;

	if (!sdata->disable_dynamic_ps &&
	    sdata->vif.bss_conf.dynamic_ps_timeout > 0) {
		/* don't enter PS if TX frames are pending */
		if (drv_tx_frames_pending(local)) {
			atbm_mod_timer(&sdata->dynamic_ps_timer, jiffies +
				  msecs_to_jiffies(
				  sdata->vif.bss_conf.dynamic_ps_timeout));
			return;
		}

		/*
		 * transmission can be stopped by others which leads to
		 * dynamic_ps_timer expiry. Postpone the ps timer if it
		 * is not the actual idle state.
		 */
		spin_lock_irqsave(&local->queue_stop_reason_lock, flags);
		for (q = 0; q < local->hw.queues; q++) {
			if (local->queue_stop_reasons[q]) {
				spin_unlock_irqrestore(&local->queue_stop_reason_lock,
						       flags);
				atbm_mod_timer(&sdata->dynamic_ps_timer, jiffies +
					  msecs_to_jiffies(
					  sdata->vif.bss_conf.dynamic_ps_timeout));
				return;
			}
		}
		spin_unlock_irqrestore(&local->queue_stop_reason_lock, flags);
	}

	if ((local->hw.flags & IEEE80211_HW_PS_NULLFUNC_STACK) &&
	    (!(ifmgd->flags & IEEE80211_STA_NULLFUNC_ACKED))) {
		netif_tx_stop_all_queues(sdata->dev);

		if (drv_tx_frames_pending(local))
			atbm_mod_timer(&sdata->dynamic_ps_timer, jiffies +
				  msecs_to_jiffies(
				  sdata->vif.bss_conf.dynamic_ps_timeout));
		else {
			ieee80211_send_nullfunc(local, sdata, 1);
			/* Flush to get the tx status of nullfunc frame */
			drv_flush(local, sdata, false);
		}
	}

	if (!((local->hw.flags & IEEE80211_HW_REPORTS_TX_ACK_STATUS) &&
	      (local->hw.flags & IEEE80211_HW_PS_NULLFUNC_STACK)) ||
	    (ifmgd->flags & IEEE80211_STA_NULLFUNC_ACKED)) {
		ifmgd->flags &= ~IEEE80211_STA_NULLFUNC_ACKED;
		sdata->vif.bss_conf.ps_enabled = true;
		ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_PS);
	}

	if (local->hw.flags & IEEE80211_HW_PS_NULLFUNC_STACK)
		netif_tx_wake_all_queues(sdata->dev);
}

void ieee80211_dynamic_ps_timer(unsigned long data)
{
	struct ieee80211_sub_if_data *sdata = (void *) data;
	struct ieee80211_local *local = sdata->local;

	if (local->quiescing || local->suspended)
		return;

	ieee80211_queue_work(&local->hw, &sdata->dynamic_ps_enable_work);
}
#endif
#if 0
void ieee80211_scan_delay(unsigned long data)
{
}
#endif
#ifdef CONFIG_ATBM_SUPPORT_P2P
#define MAX_P2P_NOA_DESC 4
/* TODO: check if not defined already */
struct noa_desc {
	u8	count;
	__le32	duration;
	__le32	interval;
	__le32	start;
}__packed;

struct noa_attr {
	u8	index;
	u8	oppPsCTWindow;
	struct noa_desc dsc[MAX_P2P_NOA_DESC];
}__packed;

struct p2p_attr {
	u8	type;
	__le16	len;
	u8	data[0];
}__packed;

static void ieee80211_sta_p2p_noa_check(struct ieee80211_local *local,
					struct ieee80211_sub_if_data *sdata,
					u8 *p2p_ie, size_t p2p_ie_len)
{
	struct ieee80211_bss_conf *conf = &sdata->vif.bss_conf;
	struct cfg80211_p2p_ps p2p_ps = {0};
	struct p2p_attr *attr;
	struct noa_attr *noa_attr = NULL;
	u8 *ptr;
	u16 len = 0, noa_len = 0;
	int i;
	size_t left;
	size_t elen;

	if (!p2p_ie)
		goto out;
	ptr = p2p_ie;
	left = p2p_ie_len;

	while (left >= 2) {

		elen = *++ptr;
		ptr++;
		left -= 2;
		if (elen > left)
			break;
		ptr += 4;

		/* Find Noa attr */
		for (i = 0; i < elen-4;) {
			attr = (struct p2p_attr *) &ptr[i];
			len = __le32_to_cpu(attr->len);

			switch (attr->type) {
			case 0x0C:
				noa_attr = (struct noa_attr *) &attr->data[0];
				noa_len = len;
				break;
			default:
				break;
			}
			if (noa_attr)
				break;
			i = i + len + 3;
		}

		left -= elen;
		ptr += elen - 4;

		if (!noa_attr)
			/* parse next P2P IE if any left */
			continue;

		/* Get NOA settings */
		p2p_ps.opp_ps = !!(noa_attr->oppPsCTWindow & BIT(7));
		p2p_ps.ctwindow = (noa_attr->oppPsCTWindow & (~BIT(7)));

		if (noa_len >= (sizeof(struct noa_desc) + 2)) {
			/* currently FW API supports only one descriptor */
			p2p_ps.count = noa_attr->dsc[0].count;
			p2p_ps.start =
				__le32_to_cpu(noa_attr->dsc[0].start);
			p2p_ps.duration =
				__le32_to_cpu(noa_attr->dsc[0].duration);
			p2p_ps.interval =
				__le32_to_cpu(noa_attr->dsc[0].interval);
		}
		/* do not continue if one descriptor found */
		break;
	}

out:
	/* Notify driver if change is required */
	if (memcmp(&conf->p2p_ps, &p2p_ps, sizeof(p2p_ps))) {
		/* do not change legacy ps settings */
		p2p_ps.legacy_ps = conf->p2p_ps.legacy_ps;
		conf->p2p_ps = p2p_ps;
		atbm_printk_debug("ieee80211_sta_p2p_noa_check\n");
		if (local->hw.flags & IEEE80211_HW_SUPPORTS_P2P_PS)
			ieee80211_bss_info_change_notify(sdata,
							 BSS_CHANGED_P2P_PS);
	}
}


/* P2P */
static void ieee80211_sta_p2p_params(struct ieee80211_local *local,
				     struct ieee80211_sub_if_data *sdata,
				     u8 *p2p_ie, size_t p2p_ie_len)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;

	if (!p2p_ie) {
		if (ifmgd->p2p_last_ie_len) {
			memset(ifmgd->p2p_last_ie, 0x00,
			       sizeof(ifmgd->p2p_last_ie));
			ifmgd->p2p_last_ie_len = 0;
			ieee80211_sta_p2p_noa_check(local, sdata, p2p_ie,
						    p2p_ie_len);
			return;
		}
	}

	if (p2p_ie_len != ifmgd->p2p_last_ie_len ||
	    memcmp(p2p_ie, ifmgd->p2p_last_ie, p2p_ie_len)) {
		/* BSS_CHANGED_P2P_PS */
		ieee80211_sta_p2p_noa_check(local, sdata, p2p_ie,
					    p2p_ie_len);
	}

	memcpy(ifmgd->p2p_last_ie, p2p_ie, p2p_ie_len);
	ifmgd->p2p_last_ie_len = p2p_ie_len;
}
#endif
/* MLME */
static void ieee80211_sta_wmm_params(struct ieee80211_local *local,
				     struct ieee80211_sub_if_data *sdata,
				     u8 *wmm_param, size_t wmm_param_len)
{
	struct ieee80211_tx_queue_params params;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	size_t left;
	int count;
	u8 *pos, uapsd_queues = 0;

	if (!local->ops->conf_tx)
		return;

	if (local->hw.queues < IEEE80211_NUM_ACS)
		return;

	if (!wmm_param)
		return;

	if (wmm_param_len < 8 || wmm_param[5] /* version */ != 1)
		return;

	if (ifmgd->flags & IEEE80211_STA_UAPSD_ENABLED)
		uapsd_queues = local->uapsd_queues;

	count = wmm_param[6] & 0x0f;
	if (count == ifmgd->wmm_last_param_set)
		return;
	ifmgd->wmm_last_param_set = count;

	pos = wmm_param + 8;
	left = wmm_param_len - 8;

	memset(&params, 0, sizeof(params));

	local->wmm_acm = 0;
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
	local->wmm_admitted_ups = 0;
#endif /*CONFIG_ATBM_APOLLO_TESTMODE*/
	for (; left >= 4; left -= 4, pos += 4) {
		int aci = (pos[0] >> 5) & 0x03;
		int acm = (pos[0] >> 4) & 0x01;
		bool uapsd = false;
		int queue;

		switch (aci) {
		case 1: /* AC_BK */
			queue = 3;
			if (acm)
				local->wmm_acm |= BIT(1) | BIT(2); /* BK/- */
			if (uapsd_queues & IEEE80211_WMM_IE_STA_QOSINFO_AC_BK)
				uapsd = true;
			break;
		case 2: /* AC_VI */
			queue = 1;
			if (acm)
				local->wmm_acm |= BIT(4) | BIT(5); /* CL/VI */
			if (uapsd_queues & IEEE80211_WMM_IE_STA_QOSINFO_AC_VI)
				uapsd = true;
			break;
		case 3: /* AC_VO */
			queue = 0;
			if (acm)
				local->wmm_acm |= BIT(6) | BIT(7); /* VO/NC */
			if (uapsd_queues & IEEE80211_WMM_IE_STA_QOSINFO_AC_VO)
				uapsd = true;
			break;
		case 0: /* AC_BE */
		default:
			queue = 2;
			if (acm)
				local->wmm_acm |= BIT(0) | BIT(3); /* BE/EE */
			if (uapsd_queues & IEEE80211_WMM_IE_STA_QOSINFO_AC_BE)
				uapsd = true;
			break;
		}

		params.aifs = pos[0] & 0x0f;
		params.cw_max = ecw2cw((pos[1] & 0xf0) >> 4);
		params.cw_min = ecw2cw(pos[1] & 0x0f);
		params.txop = get_unaligned_le16(pos + 2);
		params.uapsd = uapsd;

#ifdef CONFIG_MAC80211_ATBM_VERBOSE_DEBUG
		wiphy_debug(local->hw.wiphy,
			    "WMM queue=%d aci=%d acm=%d aifs=%d "
			    "cWmin=%d cWmax=%d txop=%d uapsd=%d\n",
			    queue, aci, acm,
			    params.aifs, params.cw_min, params.cw_max,
			    params.txop, params.uapsd);
#endif
		sdata->tx_conf[queue] = params;
		if (drv_conf_tx(local, sdata, queue, &params))
			atbm_printk_debug("failed to set TX queue parameters for queue %d\n",
				    queue);
	}

	/* enable WMM or activate new settings */
	sdata->vif.bss_conf.qos = true;
	ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_QOS);
}

static u32 ieee80211_handle_bss_capability(struct ieee80211_sub_if_data *sdata,
					   u16 capab, bool erp_valid, u8 erp)
{
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(sdata->local, sdata);
	struct ieee80211_bss_conf *bss_conf = &sdata->vif.bss_conf;
	u32 changed = 0;
	bool use_protection;
	bool use_short_preamble;
	bool use_short_slot;

	if (erp_valid) {
		use_protection = (erp & WLAN_ERP_USE_PROTECTION) != 0;
		use_short_preamble = (erp & WLAN_ERP_BARKER_PREAMBLE) == 0;
	} else {
		use_protection = false;
		use_short_preamble = !!(capab & WLAN_CAPABILITY_SHORT_PREAMBLE);
	}

	use_short_slot = !!(capab & WLAN_CAPABILITY_SHORT_SLOT_TIME);
	if (chan_state->conf.channel->band == IEEE80211_BAND_5GHZ)
		use_short_slot = true;

	if (use_protection != bss_conf->use_cts_prot) {
		bss_conf->use_cts_prot = use_protection;
		changed |= BSS_CHANGED_ERP_CTS_PROT;
	}

	if (use_short_preamble != bss_conf->use_short_preamble) {
		bss_conf->use_short_preamble = use_short_preamble;
		changed |= BSS_CHANGED_ERP_PREAMBLE;
	}

	if (use_short_slot != bss_conf->use_short_slot) {
		bss_conf->use_short_slot = use_short_slot;
		changed |= BSS_CHANGED_ERP_SLOT;
	}

	return changed;
}

static void ieee80211_set_associated(struct ieee80211_sub_if_data *sdata,
				     struct cfg80211_bss *cbss,
				     u32 bss_info_changed)
{
	struct ieee80211_bss *bss = (void *)cbss->priv;
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_bss_conf *bss_conf = &sdata->vif.bss_conf;
	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 0))
	const struct cfg80211_bss_ies *ies;
	#endif
	bss_info_changed |= BSS_CHANGED_ASSOC;
	/* set timing information */
	bss_conf->beacon_int = cbss->beacon_interval;
	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 0))
	rcu_read_lock();
	ies = rcu_dereference(cbss->ies);
	bss_conf->timestamp = ies->tsf;
	rcu_read_unlock();
	#else
	bss_conf->timestamp = cbss->tsf;
	#endif
	
	atbm_wifi_status_set(1);

	bss_info_changed |= BSS_CHANGED_BEACON_INT;
	bss_info_changed |= ieee80211_handle_bss_capability(sdata,
		cbss->capability, bss->has_erp_value, bss->erp_value);

	sdata->u.mgd.beacon_timeout = usecs_to_jiffies(ieee80211_tu_to_usec(
		IEEE80211_BEACON_LOSS_COUNT * bss_conf->beacon_int));

	sdata->u.mgd.associated = cbss;
	memcpy(sdata->u.mgd.bssid, cbss->bssid, ETH_ALEN);

	sdata->u.mgd.flags |= IEEE80211_STA_RESET_SIGNAL_AVE;

	/* just to be sure */
	sdata->u.mgd.flags &= ~(IEEE80211_STA_CONNECTION_POLL |
				IEEE80211_STA_BEACON_POLL);

	ieee80211_led_assoc(local, 1);

	if (local->hw.flags & IEEE80211_HW_NEED_DTIM_PERIOD)
		bss_conf->dtim_period = bss->dtim_period;
	else
		bss_conf->dtim_period = 0;

	bss_conf->assoc = 1;
	/*
	 * For now just always ask the driver to update the basic rateset
	 * when we have associated, we aren't checking whether it actually
	 * changed or not.
	 */
	bss_info_changed |= BSS_CHANGED_BASIC_RATES;

	/* And the BSSID changed - we're associated now */
	bss_info_changed |= BSS_CHANGED_BSSID;

	/* Tell the driver to monitor connection quality (if supported) */
	if ((local->hw.flags & IEEE80211_HW_SUPPORTS_CQM_RSSI) &&
	    bss_conf->cqm_rssi_thold)
		bss_info_changed |= BSS_CHANGED_CQM;

	/* Enable ARP filtering */
	if (bss_conf->arp_filter_enabled != sdata->arp_filter_state) {
		bss_conf->arp_filter_enabled = sdata->arp_filter_state;
		bss_info_changed |= BSS_CHANGED_ARP_FILTER;
	}

#ifdef IPV6_FILTERING
	/* Enable NDP filtering */
	if (bss_conf->ndp_filter_enabled != sdata->ndp_filter_state) {
		bss_conf->ndp_filter_enabled = sdata->ndp_filter_state;
		bss_info_changed |= BSS_CHANGED_NDP_FILTER;
	}
#endif /*IPV6_FILTERING*/

	ieee80211_bss_info_change_notify(sdata, bss_info_changed);

	mutex_lock(&local->iflist_mtx);
	ieee80211_recalc_ps(local, -1);
#ifdef CONFIG_ATBM_SMPS
	ieee80211_recalc_smps(local);
#endif
	mutex_unlock(&local->iflist_mtx);

	netif_tx_start_all_queues(sdata->dev);
	netif_carrier_on(sdata->dev);
	ieee80211_medium_traffic_start(sdata);
}

static void ieee80211_set_disassoc(struct ieee80211_sub_if_data *sdata,
				   bool remove_sta, bool tx)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_local *local = sdata->local;
	struct sta_info *sta;
	u32 changed = 0, config_changed = 0;
	u8 bssid[ETH_ALEN];

	ASSERT_MGD_MTX(ifmgd);
	
	if (WARN_ON(!ifmgd->associated))
		return;

	atbm_wifi_status_set(0);
	
	memcpy(bssid, ifmgd->associated->bssid, ETH_ALEN);
	mutex_lock(&local->mtx);
	ieee80211_cancle_connecting_work(sdata,bssid,false);
	mutex_unlock(&local->mtx);
	ifmgd->associated = NULL;

	synchronize_rcu();

	memset(ifmgd->bssid, 0, ETH_ALEN);

	/*
	 * we need to commit the associated = NULL change because the
	 * scan code uses that to determine whether this iface should
	 * go to/wake up from powersave or not -- and could otherwise
	 * wake the queues erroneously.
	 */
	smp_mb();

	/*
	 * Thus, we can only afterwards stop the queues -- to account
	 * for the case where another CPU is finishing a scan at this
	 * time -- we don't want the scan code to enable queues.
	 */

	netif_tx_stop_all_queues(sdata->dev);
	netif_carrier_off(sdata->dev);
	ieee80211_medium_traffic_concle(sdata);
	mutex_lock(&local->sta_mtx);
	sta = sta_info_get(sdata, bssid);
	if (sta) {
		set_sta_flag(sta, WLAN_STA_BLOCK_BA);
		ieee80211_sta_tear_down_BA_sessions(sta, tx);
	}
	mutex_unlock(&local->sta_mtx);

	changed |= ieee80211_reset_erp_info(sdata);

	ieee80211_led_assoc(local, 0);
	changed |= BSS_CHANGED_ASSOC;
	sdata->vif.bss_conf.assoc = false;

	ieee80211_set_wmm_default(sdata);

	/* channel(_type) changes are handled by ieee80211_hw_config */
	WARN_ON(!ieee80211_set_channel_type(local, sdata, NL80211_CHAN_NO_HT));

	/* on the next assoc, re-program HT parameters */
	sdata->ht_opmode_valid = false;

	local->power_constr_level = 0;
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	atbm_del_timer_sync(&sdata->dynamic_ps_timer);
	atbm_cancel_work_sync(&sdata->dynamic_ps_enable_work);
#endif
	if (sdata->vif.bss_conf.ps_enabled) {
		sdata->vif.bss_conf.ps_enabled = false;
		ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_PS);
	}
	sdata->ps_allowed = false;

	ieee80211_hw_config(local, config_changed);
	/* Disable ARP filtering */
	if (sdata->vif.bss_conf.arp_filter_enabled) {
		sdata->vif.bss_conf.arp_filter_enabled = false;
		changed |= BSS_CHANGED_ARP_FILTER;
	}

#ifdef IPV6_FILTERING
	/* Disable NDP filtering */
	if (sdata->vif.bss_conf.ndp_filter_enabled) {
		sdata->vif.bss_conf.ndp_filter_enabled = false;
		changed |= BSS_CHANGED_NDP_FILTER;
	}
#endif /*IPV6_FILTERING*/

	/* The BSSID (not really interesting) and HT changed */
	changed |= BSS_CHANGED_BSSID | BSS_CHANGED_HT;
	ieee80211_bss_info_change_notify(sdata, changed);

	/* remove AP and TDLS peers */
	if (remove_sta)
		sta_info_flush(local, sdata);
#ifdef CONFIG_MAC80211_BRIDGE
	ieee80211_brigde_flush(sdata);
#endif //CONFIG_MAC80211_BRIDGE
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	atbm_del_timer_sync(&sdata->u.mgd.conn_mon_timer);
	atbm_del_timer_sync(&sdata->u.mgd.bcn_mon_timer);
	atbm_del_timer_sync(&sdata->u.mgd.timer);
#endif
#ifdef CONFIG_ATBM_SUPPORT_CHANSWITCH
	atbm_del_timer_sync(&sdata->u.mgd.chswitch_timer);
#endif
}



struct sk_buff *ieee80211_ap_probereq_get(struct ieee80211_hw *hw,
					  struct ieee80211_vif *vif)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct sk_buff *skb;
	const u8 *ssid;

	if (WARN_ON(sdata->vif.type != NL80211_IFTYPE_STATION))
		return NULL;

	ASSERT_MGD_MTX(ifmgd);

	if (!ifmgd->associated)
		return NULL;

	ssid = ieee80211_bss_get_ie(ifmgd->associated, ATBM_WLAN_EID_SSID);
	skb = ieee80211_build_probe_req(sdata, ifmgd->associated->bssid,
					(u32) -1, ssid + 2, ssid[1],
					NULL, 0, true);

	return skb;
}
//EXPORT_SYMBOL(ieee80211_ap_probereq_get);

static void __ieee80211_connection_loss(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_local *local = sdata->local;
	u8 bssid[ETH_ALEN];

	mutex_lock(&ifmgd->mtx);
	if (!ifmgd->associated) {
		mutex_unlock(&ifmgd->mtx);
		return;
	}

	memcpy(bssid, ifmgd->associated->bssid, ETH_ALEN);

	atbm_printk_always("<WARNING>%s:AP %pM lost.....\n",
	       sdata->name, bssid);

	ieee80211_set_disassoc(sdata, true, true);
	mutex_unlock(&ifmgd->mtx);
	/*
	 * must be outside lock due to cfg80211,
	 * but that's not a problem.
	 */
	ieee80211_send_deauth_disassoc(sdata, bssid,
				       IEEE80211_STYPE_DEAUTH,
				       WLAN_REASON_DISASSOC_DUE_TO_INACTIVITY,
				       NULL, true);

	mutex_lock(&local->mtx);	
	ieee80211_recalc_idle(local);
	mutex_unlock(&local->mtx);
}

void ieee80211_beacon_connection_loss_work(struct atbm_work_struct *work)
{
	struct ieee80211_sub_if_data *sdata =
		container_of(work, struct ieee80211_sub_if_data,
			     u.mgd.beacon_connection_loss_work);

	if(sdata->local->adaptive_started == true){
		atbm_printk_debug("adaptive_started! ieee80211_beacon_connection_loss_work stop");
		return;
	}

	if (sdata->local->hw.flags & IEEE80211_HW_CONNECTION_MONITOR)
	{
		__ieee80211_connection_loss(sdata);
	}
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	else
		ieee80211_mgd_probe_ap(sdata, true);
#endif
}
#ifdef CONFIG_ATBM_MAC80211_NO_USE
void ieee80211_beacon_loss(struct ieee80211_vif *vif)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);
	struct ieee80211_hw *hw = &sdata->local->hw;

	trace_api_beacon_loss(sdata);

	WARN_ON(hw->flags & IEEE80211_HW_CONNECTION_MONITOR);
	ieee80211_queue_work(hw, &sdata->u.mgd.beacon_connection_loss_work);
}
//EXPORT_SYMBOL(ieee80211_beacon_loss);
#endif
void ieee80211_connection_loss(struct ieee80211_vif *vif)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);
	struct ieee80211_hw *hw = &sdata->local->hw;

	trace_api_connection_loss(sdata);

	WARN_ON(!(hw->flags & IEEE80211_HW_CONNECTION_MONITOR));
	ieee80211_queue_work(hw, &sdata->u.mgd.beacon_connection_loss_work);
}
//EXPORT_SYMBOL(ieee80211_connection_loss);


static enum rx_mgmt_action __must_check
ieee80211_rx_mgmt_deauth(struct ieee80211_sub_if_data *sdata,
			 struct atbm_ieee80211_mgmt *mgmt, size_t len)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	const u8 *bssid = NULL;
	u16 reason_code;

	if (len < 24 + 2)
		return RX_MGMT_NONE;

	ASSERT_MGD_MTX(ifmgd);

	bssid = ifmgd->associated->bssid;

	reason_code = le16_to_cpu(mgmt->u.deauth.reason_code);
	atbm_printk_always("%s: deauthenticated from %pM (Reason: %u)\n",
			sdata->name, bssid, reason_code);

#ifdef CONFIG_MAC80211_ATBM_ROAMING_CHANGES
	sdata->queues_locked = 0;
#endif
	ieee80211_set_disassoc(sdata, true, false);
	mutex_lock(&sdata->local->mtx);
	ieee80211_free_authen_bss(sdata);
	ieee80211_recalc_idle(sdata->local);
	mutex_unlock(&sdata->local->mtx);

	return RX_MGMT_CFG80211_DEAUTH;
}


static enum rx_mgmt_action __must_check
ieee80211_rx_mgmt_disassoc(struct ieee80211_sub_if_data *sdata,
			   struct atbm_ieee80211_mgmt *mgmt, size_t len)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	u16 reason_code;

	if (len < 24 + 2)
		return RX_MGMT_NONE;

	ASSERT_MGD_MTX(ifmgd);

	if (WARN_ON(!ifmgd->associated))
		return RX_MGMT_NONE;

	if (WARN_ON(memcmp(ifmgd->associated->bssid, mgmt->sa, ETH_ALEN)))
		return RX_MGMT_NONE;

	reason_code = le16_to_cpu(mgmt->u.disassoc.reason_code);

	atbm_printk_always("%s: disassociated from %pM (Reason: %u)\n",
			sdata->name, mgmt->sa, reason_code);

	ieee80211_set_disassoc(sdata, true, false);
	mutex_lock(&sdata->local->mtx);
	ieee80211_free_authen_bss(sdata);
	ieee80211_recalc_idle(sdata->local);
	mutex_unlock(&sdata->local->mtx);
	return RX_MGMT_CFG80211_DISASSOC;
}

static bool ieee80211_assoc_success(struct ieee80211_work *wk,
				    struct atbm_ieee80211_mgmt *mgmt, size_t len)
{
	struct ieee80211_sub_if_data *sdata = wk->sdata;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(local, sdata);
	struct ieee80211_supported_band *sband;
	struct sta_info *sta;
	struct cfg80211_bss *cbss = wk->assoc.bss;
	u8 *pos;
	u32 rates, basic_rates;
	u16 capab_info, aid;
	struct ieee802_atbm_11_elems elems;
	struct ieee80211_bss_conf *bss_conf = &sdata->vif.bss_conf;
	u32 changed = 0;
	int i, j, err;
	bool have_higher_than_11mbit = false;
	u16 ap_ht_cap_flags;
	int min_rate = INT_MAX, min_rate_index = -1;

	/* AssocResp and ReassocResp have identical structure */

	aid = le16_to_cpu(mgmt->u.assoc_resp.aid);
	capab_info = le16_to_cpu(mgmt->u.assoc_resp.capab_info);

	if ((aid & (BIT(15) | BIT(14))) != (BIT(15) | BIT(14)))
		atbm_printk_mgmt(
		       "%s: invalid AID value 0x%x; bits 15:14 not set\n",
		       sdata->name, aid);
	aid &= ~(BIT(15) | BIT(14));

	ifmgd->broken_ap = false;

	if (aid == 0 || aid > IEEE80211_MAX_AID) {
		atbm_printk_err(
		       "%s: invalid AID value %d\n",
		       sdata->name, aid);
		aid = 0;
		ifmgd->broken_ap = true;
	}

	pos = mgmt->u.assoc_resp.variable;
	ieee802_11_parse_elems(pos, len - (pos - (u8 *) mgmt), &elems);

	if (!elems.supp_rates) {
		atbm_printk_err("%s: no SuppRates element in AssocResp\n",
		       sdata->name);
	//	return false;
	}

	ifmgd->aid = aid;

	mutex_lock(&sdata->local->sta_mtx);
	/*
	 * station info was already allocated and inserted before
	 * the association and should be available to us
	 */
	sta = sta_info_get_rx(sdata, cbss->bssid);
	if (WARN_ON(!sta)) {
		mutex_unlock(&sdata->local->sta_mtx);
		return false;
	}

	set_sta_flag(sta, WLAN_STA_AUTH);
	set_sta_flag(sta, WLAN_STA_ASSOC);
	set_sta_flag(sta, WLAN_STA_ASSOC_AP);
	if (!(ifmgd->flags & IEEE80211_STA_CONTROL_PORT))
		set_sta_flag(sta, WLAN_STA_AUTHORIZED);

	rates = 0;
	basic_rates = 0;
	sband = local->hw.wiphy->bands[wk->chan->band];

	for (i = 0; i < elems.supp_rates_len; i++) {
		int rate = (elems.supp_rates[i] & 0x7f) * 5;
		bool is_basic = !!(elems.supp_rates[i] & 0x80);

		if (rate > 110)
			have_higher_than_11mbit = true;

		for (j = 0; j < sband->n_bitrates; j++) {
			if (sband->bitrates[j].bitrate == rate) {
				rates |= BIT(j);
				if (is_basic)
					basic_rates |= BIT(j);
				if (rate < min_rate) {
					min_rate = rate;
					min_rate_index = j;
				}
				break;
			}
		}
	}

	for (i = 0; i < elems.ext_supp_rates_len; i++) {
		int rate = (elems.ext_supp_rates[i] & 0x7f) * 5;
		bool is_basic = !!(elems.ext_supp_rates[i] & 0x80);

		if (rate > 110)
			have_higher_than_11mbit = true;

		for (j = 0; j < sband->n_bitrates; j++) {
			if (sband->bitrates[j].bitrate == rate) {
				rates |= BIT(j);
				if (is_basic)
					basic_rates |= BIT(j);
				if (rate < min_rate) {
					min_rate = rate;
					min_rate_index = j;
				}
				break;
			}
		}
	}

	/*
	 * some buggy APs don't advertise basic_rates. use the lowest
	 * supported rate instead.
	 */
	if (unlikely(!basic_rates) && min_rate_index >= 0) {
		atbm_printk_err( "%s: No basic rates in AssocResp. "
		       "Using min supported rate instead.\n", sdata->name);
		basic_rates = BIT(min_rate_index);
	}
	sta->sta.supp_rates[wk->chan->band] = rates;
	sdata->vif.bss_conf.basic_rates = basic_rates;

	/* cf. IEEE 802.11 9.2.12 */
	if (wk->chan->band == IEEE80211_BAND_2GHZ &&
	    have_higher_than_11mbit)
		sdata->flags |= IEEE80211_SDATA_OPERATING_GMODE;
	else
		sdata->flags &= ~IEEE80211_SDATA_OPERATING_GMODE;
	//printk("%s:sssht_cap_elem(%p),ifmgd->flags(%x),time(%ld) ms\n",__func__,elems.ht_cap_elem,ifmgd->flags & IEEE80211_STA_DISABLE_11N,jiffies_to_msecs(EndTime-StartTime));
	if (elems.ht_cap_elem && !(ifmgd->flags & IEEE80211_STA_DISABLE_11N))
		ieee80211_ht_cap_ie_to_sta_ht_cap(sband,
				elems.ht_cap_elem, &sta->sta.ht_cap);

	ap_ht_cap_flags = sta->sta.ht_cap.cap;
#ifndef CONFIG_RATE_HW_CONTROL
	rate_control_rate_init(sta);
#endif
	if (ifmgd->flags & IEEE80211_STA_MFP_ENABLED)
		set_sta_flag(sta, WLAN_STA_MFP);

	if (elems.wmm_param)
		set_sta_flag(sta, WLAN_STA_WME);

	/* sta_info_reinsert will also unlock the mutex lock */
	err = sta_info_reinsert(sta);
	sta = NULL;
	if (err) {
		atbm_printk_err( "%s: failed to insert STA entry for"
		       " the AP (error %d)\n", sdata->name, err);
		return false;
	}

	/*
	 * Always handle WMM once after association regardless
	 * of the first value the AP uses. Setting -1 here has
	 * that effect because the AP values is an unsigned
	 * 4-bit value.
	 */
	ifmgd->wmm_last_param_set = -1;

	if (elems.wmm_param)
		ieee80211_sta_wmm_params(local, sdata, elems.wmm_param,
					 elems.wmm_param_len);
	else
		ieee80211_set_wmm_default(sdata);

	chan_state->oper_channel = wk->chan;

	if (elems.ht_info_elem && elems.wmm_param &&
	    (sdata->local->hw.queues >= 4) &&
	    !(ifmgd->flags & IEEE80211_STA_DISABLE_11N))
		changed |= ieee80211_enable_ht(sdata, elems.ht_info_elem,
					       cbss->bssid, ap_ht_cap_flags,
					       false);

	/* set AID and assoc capability,
	 * ieee80211_set_associated() will tell the driver */
	bss_conf->aid = aid;
	bss_conf->assoc_capability = capab_info;
	ieee80211_set_associated(sdata, cbss, changed);
#ifdef CONFIG_ATBM_SUPPORT_P2P
	/*
	*process p2p ie ,and set p2p power save params:CTWindows,and NoA;
	*/
	while(sdata->vif.p2p)
	{
		if(!(elems.p2p_ie&&elems.p2p_ie_len))
		{
			atbm_printk_err( "is p2p station,but p2p go do not send p2p ie\n");
			break;
		}
		ieee80211_sta_p2p_params(local, sdata, elems.p2p_ie,
					 elems.p2p_ie_len);

		break;
	}
#endif

#ifdef CONFIG_ATBM_4ADDR
	/*
	 * If we're using 4-addr mode, let the AP know that we're
	 * doing so, so that it can create the STA VLAN on its side
	 */
	if (ifmgd->use_4addr)
		ieee80211_send_4addr_nullfunc(local, sdata);
#endif
	/*
	 * Start timer to probe the connection to the AP now.
	 * Also start the timer that will detect beacon loss.
	 */
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	ieee80211_sta_rx_notify(sdata, (struct ieee80211_hdr *)mgmt);
	ieee80211_sta_reset_beacon_monitor(sdata);
#endif

	return true;
}


static void ieee80211_rx_bss_info(struct ieee80211_sub_if_data *sdata,
				  struct atbm_ieee80211_mgmt *mgmt,
				  size_t len,
				  struct ieee80211_rx_status *rx_status,
				  struct ieee802_atbm_11_elems *elems,
				  bool beacon)
{
	struct ieee80211_local *local = sdata->local;
	int freq;
	struct ieee80211_bss *bss;
	struct ieee80211_channel *channel;
	bool need_ps = false;
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(local, sdata); 

	if (sdata->u.mgd.associated) {
		bss = (void *)sdata->u.mgd.associated->priv;
		/* not previously set so we may need to recalc */
		need_ps = !bss->dtim_period;
	}

	if (elems->ds_params && elems->ds_params_len == 1)
		freq = ieee80211_channel_to_frequency(elems->ds_params[0],
						      rx_status->band);
	else
		freq = rx_status->freq;

	channel = ieee80211_get_channel(local->hw.wiphy, freq);

	if (!channel || channel->flags & IEEE80211_CHAN_DISABLED)
		return;

	bss = ieee80211_bss_info_update(local, rx_status, mgmt, len, elems,
					channel, beacon);
	if (bss)
		ieee80211_rx_bss_put(local, bss);

	if (!sdata->u.mgd.associated)
		return;

	if (need_ps) {
		mutex_lock(&local->iflist_mtx);
		ieee80211_recalc_ps(local, -1);
		mutex_unlock(&local->iflist_mtx);
	}
	//add by wp , AP channel change we need deauth
	if(/*(sdata->u.mgd.associated) && */
		(channel_hw_value(chan_state->conf.channel) != channel_hw_value(channel))){
		atbm_printk_err("rx_bss_info channel change %d,deauth it [%pM][%pM][%pM][%x] seq=%d\n",channel_hw_value(channel),
			mgmt->da,mgmt->sa,mgmt->bssid,mgmt->frame_control,mgmt->seq_ctrl);
		if(elems->ssid&&elems->ssid_len){
			u8 *ssid = atbm_kzalloc(elems->ssid_len+1, GFP_KERNEL);

			if(ssid){
				memcpy(ssid,elems->ssid,elems->ssid_len);
				ssid[elems->ssid_len] = 0;
				atbm_printk_err("%s:channel change ssid(%s)\n",__func__,ssid);
				atbm_kfree(ssid);
			}
			
		}
		ieee80211_connection_loss(&sdata->vif);
		return;
	}
#ifdef CONFIG_ATBM_SUPPORT_CHANSWITCH
	if (elems->ch_switch_elem && (elems->ch_switch_elem_len == 3) &&
	    (memcmp(mgmt->bssid, sdata->u.mgd.associated->bssid,
							ETH_ALEN) == 0)) {
#ifndef ATBM_SUPPORT_WIDTH_40M
		struct ieee80211_channel_sw_ie *sw_elem =
			(struct ieee80211_channel_sw_ie *)elems->ch_switch_elem;
		ieee80211_sta_process_chanswitch(sdata, sw_elem,
						 bss, rx_status->mactime);
#else
		struct atbm_ieee80211_channel_sw_packed_ie sw_packed_ie = {
				.chan_sw_ie = NULL,
				.ex_chan_sw_ie = NULL,
				.sec_chan_offs_ie = NULL,
				};

		sw_packed_ie.chan_sw_ie = (struct ieee80211_channel_sw_ie *)elems->ch_switch_elem;
		ieee80211_sta_process_chanswitch(sdata,
								&sw_packed_ie,
								bss,
								rx_status->mactime);
#endif
	}
#endif
}


static void ieee80211_rx_mgmt_probe_resp(struct ieee80211_sub_if_data *sdata,
					 struct sk_buff *skb)
{
	struct atbm_ieee80211_mgmt *mgmt = (void *)skb->data;
	struct ieee80211_if_managed *ifmgd;
	struct ieee80211_rx_status *rx_status = (void *) skb->cb;
	size_t baselen, len = skb->len;
	struct ieee802_atbm_11_elems elems;

	ifmgd = &sdata->u.mgd;

	ASSERT_MGD_MTX(ifmgd);

	if (memcmp(mgmt->da, sdata->vif.addr, ETH_ALEN))
		return; /* ignore ProbeResp to foreign address */

	baselen = (u8 *) mgmt->u.probe_resp.variable - (u8 *) mgmt;
	if (baselen > len)
		return;

	ieee802_11_parse_elems(mgmt->u.probe_resp.variable, len - baselen,
				&elems);

	ieee80211_rx_bss_info(sdata, mgmt, len, rx_status, &elems, false);
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	if (ifmgd->associated &&
	    memcmp(mgmt->bssid, ifmgd->associated->bssid, ETH_ALEN) == 0)
		ieee80211_reset_ap_probe(sdata);
#endif
}

/*
 * This is the canonical list of information elements we care about,
 * the filter code also gives us all changes to the Microsoft OUI
 * (00:50:F2) vendor IE which is used for WMM which we need to track.
 *
 * We implement beacon filtering in software since that means we can
 * avoid processing the frame here and in cfg80211, and userspace
 * will not be able to tell whether the hardware supports it or not.
 *
 * XXX: This list needs to be dynamic -- userspace needs to be able to
 *	add items it requires. It also needs to be able to tell us to
 *	look out for other vendor IEs.
 */
static const u64 care_about_ies =
	(1ULL << ATBM_WLAN_EID_COUNTRY) |
	(1ULL << ATBM_WLAN_EID_ERP_INFO) |
	(1ULL << ATBM_WLAN_EID_CHANNEL_SWITCH) |
	(1ULL << ATBM_WLAN_EID_PWR_CONSTRAINT) |
	(1ULL << ATBM_WLAN_EID_HT_CAPABILITY) |
	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 0))
	(1ULL <<WLAN_EID_HT_OPERATION);
	#else
	(1ULL << ATBM_WLAN_EID_HT_INFORMATION);
	#endif

static void ieee80211_rx_mgmt_beacon(struct ieee80211_sub_if_data *sdata,
				     struct atbm_ieee80211_mgmt *mgmt,
				     size_t len,
				     struct ieee80211_rx_status *rx_status)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	struct ieee80211_bss_conf *bss_conf = &sdata->vif.bss_conf;
#endif
	size_t baselen;
	struct ieee802_atbm_11_elems elems;
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(local, sdata); 
	u32 changed = 0;
	bool erp_valid ; 
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	bool directed_tim = false;
#endif
	u8 erp_value = 0;
	u32 ncrc;
	u8 *bssid;

	ASSERT_MGD_MTX(ifmgd);
	/* Process beacon from the current BSS */
	baselen = (u8 *) mgmt->u.beacon.variable - (u8 *) mgmt;
	if (baselen > len)
		return;

	if (rx_status->freq != channel_center_freq(chan_state->conf.channel))
		return;

	/*
	 * We might have received a number of frames, among them a
	 * disassoc frame and a beacon...
	 */
	if (!ifmgd->associated)
		return;

	bssid = ifmgd->associated->bssid;

	/*
	 * And in theory even frames from a different AP we were just
	 * associated to a split-second ago!
	 */
	if (memcmp(bssid, mgmt->bssid, ETH_ALEN) != 0)
		return;

	/* Track average RSSI from the Beacon frames of the current AP */
	ifmgd->last_beacon_signal = rx_status->signal;
	if (ifmgd->flags & IEEE80211_STA_RESET_SIGNAL_AVE) {
		ifmgd->flags &= ~IEEE80211_STA_RESET_SIGNAL_AVE;
		ifmgd->ave_beacon_signal = rx_status->signal * 16;
		ifmgd->last_cqm_event_signal = 0;
		ifmgd->count_beacon_signal = 1;
		ifmgd->last_ave_beacon_signal = 0;
	} else {
		ifmgd->ave_beacon_signal =
			(IEEE80211_SIGNAL_AVE_WEIGHT * rx_status->signal * 16 +
			 (16 - IEEE80211_SIGNAL_AVE_WEIGHT) *
			 ifmgd->ave_beacon_signal) / 16;
		ifmgd->count_beacon_signal++;
	}
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	if (ifmgd->rssi_min_thold != ifmgd->rssi_max_thold &&
	    ifmgd->count_beacon_signal >= IEEE80211_SIGNAL_AVE_MIN_COUNT) {
		int sig = ifmgd->ave_beacon_signal;
		int last_sig = ifmgd->last_ave_beacon_signal;

		/*
		 * if signal crosses either of the boundaries, invoke callback
		 * with appropriate parameters
		 */
		if (sig > ifmgd->rssi_max_thold &&
		    (last_sig <= ifmgd->rssi_min_thold || last_sig == 0)) {
			ifmgd->last_ave_beacon_signal = sig;
			drv_rssi_callback(local, RSSI_EVENT_HIGH);
		} else if (sig < ifmgd->rssi_min_thold &&
			   (last_sig >= ifmgd->rssi_max_thold ||
			   last_sig == 0)) {
			ifmgd->last_ave_beacon_signal = sig;
			drv_rssi_callback(local, RSSI_EVENT_LOW);
		}
	}

	if (bss_conf->cqm_rssi_thold &&
	    ifmgd->count_beacon_signal >= IEEE80211_SIGNAL_AVE_MIN_COUNT &&
	    !(local->hw.flags & IEEE80211_HW_SUPPORTS_CQM_RSSI)) {
		int sig = ifmgd->ave_beacon_signal / 16;
		int last_event = ifmgd->last_cqm_event_signal;
		int thold = bss_conf->cqm_rssi_thold;
		int hyst = bss_conf->cqm_rssi_hyst;
		if (sig < thold &&
		    (last_event == 0 || sig < last_event - hyst)) {
			ifmgd->last_cqm_event_signal = sig;
			ieee80211_cqm_rssi_notify(
				&sdata->vif,
				NL80211_CQM_RSSI_THRESHOLD_EVENT_LOW,
				GFP_KERNEL);
		} else if (sig > thold &&
			   (last_event == 0 || sig > last_event + hyst)) {
			ifmgd->last_cqm_event_signal = sig;
			ieee80211_cqm_rssi_notify(
				&sdata->vif,
				NL80211_CQM_RSSI_THRESHOLD_EVENT_HIGH,
				GFP_KERNEL);
		}
	}
	if (ifmgd->flags & IEEE80211_STA_BEACON_POLL) {
#ifdef CONFIG_MAC80211_ATBM_VERBOSE_DEBUG
		if (net_ratelimit()) {
			atbm_printk_debug("%s: cancelling probereq poll due "
			       "to a received beacon\n", sdata->name);
		}
#endif
		ifmgd->flags &= ~IEEE80211_STA_BEACON_POLL;
		mutex_lock(&local->iflist_mtx);
		ieee80211_recalc_ps(local, -1);
		mutex_unlock(&local->iflist_mtx);
	}
	/*
	 * Push the beacon loss detection into the future since
	 * we are processing a beacon from the AP just now.
	 */
	ieee80211_sta_reset_beacon_monitor(sdata);
#endif
	ncrc = crc32_be(0, (void *)&mgmt->u.beacon.beacon_int, 4);
	ncrc = atbm_ieee802_11_parse_elems_crc(mgmt->u.beacon.variable,
					  len - baselen, &elems,
					  care_about_ies, ncrc);
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	if (local->hw.flags & IEEE80211_HW_PS_NULLFUNC_STACK)
		directed_tim = ieee80211_check_tim(elems.tim, elems.tim_len,
						   ifmgd->aid);
#endif
	if (ncrc != ifmgd->beacon_crc || !ifmgd->beacon_crc_valid) {
		ieee80211_rx_bss_info(sdata, mgmt, len, rx_status, &elems,
				      true);

		ieee80211_sta_wmm_params(local, sdata, elems.wmm_param,
					 elems.wmm_param_len);
#ifdef CONFIG_ATBM_SUPPORT_P2P
		ieee80211_sta_p2p_params(local, sdata, elems.p2p_ie,
					 elems.p2p_ie_len);
#endif
	}
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	if (local->hw.flags & IEEE80211_HW_PS_NULLFUNC_STACK) {
		if (directed_tim) {
			if (sdata->vif.bss_conf.dynamic_ps_timeout > 0) {
				sdata->vif.bss_conf.ps_enabled = false;
				ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_PS);
				ieee80211_send_nullfunc(local, sdata, 0);
			} else {
				sdata->pspolling = true;

				/*
				 * Here is assumed that the driver will be
				 * able to send ps-poll frame and receive a
				 * response even though power save mode is
				 * enabled, but some drivers might require
				 * to disable power save here. This needs
				 * to be investigated.
				 */
				ieee80211_send_pspoll(local, sdata);
			}
		}
	}
#endif
	if (ncrc == ifmgd->beacon_crc && ifmgd->beacon_crc_valid)
		return;
	ifmgd->beacon_crc = ncrc;
	ifmgd->beacon_crc_valid = true;

	if (elems.erp_info && elems.erp_info_len >= 1) {
		erp_valid = true;
		erp_value = elems.erp_info[0];
	} else {
		erp_valid = false;
	}
#ifdef CONFIG_ATBM_SUPPORT_CHANSWITCH
#ifdef ATBM_SUPPORT_WIDTH_40M
	if(elems.ch_switch_elem)
	{
		struct atbm_ieee80211_channel_sw_packed_ie sw_packed_ie = {
				.chan_sw_ie = NULL,
				.ex_chan_sw_ie = NULL,
				.sec_chan_offs_ie = NULL,
				};

		sw_packed_ie.chan_sw_ie = (struct ieee80211_channel_sw_ie *)elems.ch_switch_elem;
		ieee80211_sta_process_chanswitch(sdata,
								&sw_packed_ie,
								(void *)ifmgd->associated->priv,
								rx_status->mactime);
	}
	if(elems.secondary_ch_elem){
		
		struct atbm_ieee80211_channel_sw_packed_ie sw_packed_ie = {
				.chan_sw_ie = NULL,
				.ex_chan_sw_ie = NULL,
				.sec_chan_offs_ie = NULL,
				};
		sw_packed_ie.sec_chan_offs_ie= (struct atbm_ieee80211_sec_chan_offs_ie *)elems.secondary_ch_elem;
		ieee80211_sta_process_chanswitch(sdata,
								&sw_packed_ie,
								(void *)ifmgd->associated->priv,
								rx_status->mactime);
	}
	if(elems.extended_ch_switch_elem)
	{
		struct atbm_ieee80211_channel_sw_packed_ie sw_packed_ie = {
				.chan_sw_ie = NULL,
				.ex_chan_sw_ie = NULL,
				.sec_chan_offs_ie = NULL,
				};
		sw_packed_ie.ex_chan_sw_ie= (struct atbm_ieee80211_ext_chansw_ie *)elems.extended_ch_switch_elem;
		ieee80211_sta_process_chanswitch(sdata,
								&sw_packed_ie,
								(void *)ifmgd->associated->priv,
								rx_status->mactime);

	}
#endif
#endif
	changed |= ieee80211_handle_bss_capability(sdata,
			le16_to_cpu(mgmt->u.beacon.capab_info),
			erp_valid, erp_value);


	if (elems.ht_cap_elem && elems.ht_info_elem && elems.wmm_param &&
	    !(ifmgd->flags & IEEE80211_STA_DISABLE_11N)) {
		struct sta_info *sta;
		struct ieee80211_supported_band *sband;
		u16 ap_ht_cap_flags;

		rcu_read_lock();

		sta = sta_info_get(sdata, bssid);
		if (WARN_ON(!sta)) {
			atbm_printk_err("%s() %d## ifname[%s] sdata->vif.addr:%pM MAC:%pM \n",
							__func__,__LINE__,sdata->name,sdata->vif.addr,bssid);
			rcu_read_unlock();
			return;
		}

		sband = local->hw.wiphy->bands[chan_state->conf.channel->band];

		ieee80211_ht_cap_ie_to_sta_ht_cap(sband,
				elems.ht_cap_elem, &sta->sta.ht_cap);

		ap_ht_cap_flags = sta->sta.ht_cap.cap;

		rcu_read_unlock();

		changed |= ieee80211_enable_ht(sdata, elems.ht_info_elem,
					       bssid, ap_ht_cap_flags, true);
	}

	/* Note: country IE parsing is done for us by cfg80211 */
	if (elems.country_elem) {
		/* TODO: IBSS also needs this */
		if (elems.pwr_constr_elem)
			ieee80211_handle_pwr_constr(sdata,
				le16_to_cpu(mgmt->u.probe_resp.capab_info),
				elems.pwr_constr_elem,
				elems.pwr_constr_elem_len);
	}

	ieee80211_bss_info_change_notify(sdata, changed);
}

void ieee80211_sta_rx_queued_mgmt(struct ieee80211_sub_if_data *sdata,
				  struct sk_buff *skb)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_rx_status *rx_status;
	struct atbm_ieee80211_mgmt *mgmt;
	enum rx_mgmt_action rma = RX_MGMT_NONE;
	u16 fc;
	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,11,0))
	struct wireless_dev *wdev = sdata->dev->ieee80211_ptr;
	#endif
	
	rx_status = (struct ieee80211_rx_status *) skb->cb;
	mgmt = (struct atbm_ieee80211_mgmt *) skb->data;
	fc = le16_to_cpu(mgmt->frame_control);

	mutex_lock(&ifmgd->mtx);

	if (ifmgd->associated &&
	    memcmp(ifmgd->associated->bssid, mgmt->bssid, ETH_ALEN) == 0) {
		switch (fc & IEEE80211_FCTL_STYPE) {
		case IEEE80211_STYPE_BEACON:
			ieee80211_rx_mgmt_beacon(sdata, mgmt, skb->len,
						 rx_status);
			break;
		case IEEE80211_STYPE_PROBE_RESP:
			ieee80211_rx_mgmt_probe_resp(sdata, skb);
			break;
		case IEEE80211_STYPE_DEAUTH:
			rma = ieee80211_rx_mgmt_deauth(sdata, mgmt, skb->len);
			break;
		case IEEE80211_STYPE_DISASSOC:
			rma = ieee80211_rx_mgmt_disassoc(sdata, mgmt, skb->len);
			break;
		case IEEE80211_STYPE_ACTION:
			{
#ifdef CONFIG_ATBM_SUPPORT_CHANSWITCH
#ifdef ATBM_SUPPORT_WIDTH_40M
				struct atbm_ieee80211_channel_sw_packed_ie sw_packed_ie = {
				.chan_sw_ie = NULL,
				.ex_chan_sw_ie = NULL,
				.sec_chan_offs_ie = NULL,
				};
				
				switch (mgmt->u.action.category) {
				case ATBM_WLAN_CATEGORY_SPECTRUM_MGMT:
					// add 8.5.2.6 Channel Switch Announcement frame format
					if(
					   WLAN_ACTION_SPCT_CHL_SWITCH == mgmt->u.action.u.chan_switch.action_code
					  )
					{
						sw_packed_ie.chan_sw_ie = &mgmt->u.action.u.chan_switch.sw_elem;
						if((skb->len -
							offsetof(struct atbm_ieee80211_mgmt,u.action.u.chan_switch.sw_elem))
							>
							0
						  )
						{
							u8 *tmp_ie = ((u8 *)sw_packed_ie.chan_sw_ie) + 3;//sizeof(struct ieee80211_channel_sw_ie)

							if((tmp_ie[0] == ATBM_WLAN_EID_SECONDARY_CH_OFFSET)&&(tmp_ie[1]==1))
							{
								sw_packed_ie.sec_chan_offs_ie = (struct atbm_ieee80211_sec_chan_offs_ie *)(tmp_ie+2);
							}
						}
						ieee80211_sta_process_chanswitch(sdata,
								&sw_packed_ie,
								(void *)ifmgd->associated->priv,
								rx_status->mactime);
					}
					break;
				case ATBM_WLAN_CATEGORY_PUBLIC:
					//add 8.5.8.7 Extended Channel Switch Announcement frame format
					if(
					   ATBM_WLAN_PUB_ACTION_EX_CHL_SW_ANNOUNCE == mgmt->u.action.u.ext_chan_switch.action_code
					  )
					{
						sw_packed_ie.ex_chan_sw_ie = &mgmt->u.action.u.ext_chan_switch.ext_sw_elem;

						ieee80211_sta_process_chanswitch(sdata,
								&sw_packed_ie,
								(void *)ifmgd->associated->priv,
								rx_status->mactime);
					}
					break;
				case ATBM_WLAN_CATEGORY_HT:
					{
						//add 8.5.12.2 Notify Channel Width frame format
						if(
							WLAN_HT_ACTION_NOTIFY_CHANWIDTH == mgmt->u.action.u.notify_chan_width.action_code
						  )
						{
							break;	
						}
					}
				

				default:
					break;
				}
#endif
#endif
			break;
			}
		}
		mutex_unlock(&ifmgd->mtx);

		switch (rma) {
		case RX_MGMT_NONE:
			/* no action */
			break;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,11,0))
		case RX_MGMT_CFG80211_DEAUTH:
			cfg80211_send_deauth(sdata->dev, (u8 *)mgmt, skb->len);
			break;
		case RX_MGMT_CFG80211_DISASSOC:
			cfg80211_send_disassoc(sdata->dev, (u8 *)mgmt, skb->len);
			break;
#else
		case RX_MGMT_CFG80211_DEAUTH:
		case RX_MGMT_CFG80211_DISASSOC:
			{
				atbm_wdev_lock(wdev);
				cfg80211_rx_mlme_mgmt(sdata->dev,(u8 *)mgmt,skb->len);
				atbm_wdev_unlock(wdev);
			}
			break;
#endif
		default:
			WARN(1, "unexpected: %d", rma);
		}
		return;
	}

	mutex_unlock(&ifmgd->mtx);
#ifdef CONFIG_ATBM_STA_LISTEN
	if(rx_status->flag & RX_FLAG_STA_LISTEN){
		ieee80211_sta_rx_queued_mgmt_special(sdata,skb);
		return;
	}
#endif
	if (skb->len >= 24 + 2 /* mgmt + deauth reason */ &&
	    (fc & IEEE80211_FCTL_STYPE) == IEEE80211_STYPE_DEAUTH) {
		struct ieee80211_local *local = sdata->local;
		struct ieee80211_work *wk;
#ifdef CONFIG_ATBM_SUPPORT_P2P
		u16 pre_reason = 0;
#endif

		mutex_lock(&local->mtx);
		list_for_each_entry(wk, &local->work_list, list) {
			if (wk->sdata != sdata)
				continue;

			if (wk->type != IEEE80211_WORK_ASSOC &&
			    wk->type != IEEE80211_WORK_ASSOC_BEACON_WAIT)
				continue;

			if (memcmp(mgmt->bssid, wk->filter_ta, ETH_ALEN))
				continue;
			if (memcmp(mgmt->sa, wk->filter_ta, ETH_ALEN))
				continue;

			/*
			 * Printing the message only here means we can't
			 * spuriously print it, but it also means that it
			 * won't be printed when the frame comes in before
			 * we even tried to associate or in similar cases.
			 *
			 * Ultimately, I suspect cfg80211 should print the
			 * messages instead.
			 */
			atbm_printk_mgmt(
			       "%s: deauthenticated from %pM (Reason: %u)\n",
			       sdata->name, mgmt->bssid,
			       le16_to_cpu(mgmt->u.deauth.reason_code));

			list_del_rcu(&wk->list);
			free_work(wk);
			break;
		}
		
		ieee80211_cancle_connecting_work(sdata,mgmt->bssid,true);
		/*
		*has pending so queue work
		*/
		if((list_empty(&local->work_list))&&
			(local->roc_pendding || (local->scan_req &&!local->scanning))){
			atbm_printk_mgmt("%s:queue work_work for pending scan or listenning",__func__);
			ieee80211_queue_work(&local->hw, &local->work_work);
		}
		mutex_unlock(&local->mtx);
#ifdef CONFIG_ATBM_SUPPORT_P2P
		/*
		*some phone has a bug !!!!!!
		*when after p2p provisioning wps process,go must send a deauthen with reason
		*code 23 but some phone sends deauthen code 3.
		*/
		pre_reason = le16_to_cpu(mgmt->u.deauth.reason_code);
		if((sdata->vif.p2p == true)&&(pre_reason != 23)){
			mgmt->u.deauth.reason_code = cpu_to_le16(23);
			atbm_printk_debug( "%s(%s):IEEE80211_STYPE_DEAUTH,reason(%d),pre_reason(%d)\n",
			__func__,sdata->name,le16_to_cpu(mgmt->u.deauth.reason_code),pre_reason);
		}
#endif

	if(sdata->vif.p2p == true){

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,11,0))
		cfg80211_send_deauth(sdata->dev, (u8 *)mgmt, skb->len);
#else
		{
			atbm_wdev_lock(wdev);
			cfg80211_rx_mlme_mgmt(sdata->dev,(u8 *)mgmt,skb->len);
			atbm_wdev_unlock(wdev);
		}
#endif
	}
		/*add by meng for a bug, wep change to wpa-psk , cannot connect*/
		//sta_info_flush(local, sdata);
		sta_info_destroy_addr(sdata,mgmt->bssid);
		atbm_printk_err("##### %s:%d -- da:%pM sa:%pM bssid:%pM \n",__func__,__LINE__,mgmt->da,mgmt->sa,mgmt->bssid);
		mutex_lock(&local->mtx);
		ieee80211_recalc_idle(sdata->local);
		mutex_unlock(&local->mtx);
	}
}
				  
#if defined (CONFIG_ATBM_MAC80211_NO_USE) || defined(CONFIG_PM)
static void ieee80211_sta_connection_lost(struct ieee80211_sub_if_data *sdata,
					  u8 *bssid, u8 reason)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;

	ifmgd->flags &= ~(IEEE80211_STA_CONNECTION_POLL |
			  IEEE80211_STA_BEACON_POLL);

	ieee80211_set_disassoc(sdata, true, true);
	mutex_unlock(&ifmgd->mtx);
	mutex_lock(&local->mtx);
	ieee80211_recalc_idle(local);
	mutex_unlock(&local->mtx);
	/*
	 * must be outside lock due to cfg80211,
	 * but that's not a problem.
	 */
	ieee80211_send_deauth_disassoc(sdata, bssid,
			IEEE80211_STYPE_DEAUTH, reason,
			NULL, true);
	mutex_lock(&ifmgd->mtx);
}
#endif
#if defined (CONFIG_PM) ||defined (ATBM_SUSPEND_REMOVE_INTERFACE) || defined (ATBM_SUPPORT_WOW)
void ieee80211_sta_quiesce(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;

	/*
	 * we need to use atomic bitops for the running bits
	 * only because both timers might fire at the same
	 * time -- the code here is properly synchronised.
	 */
#ifdef CONFIG_ATBM_SMPS
	atbm_cancel_work_sync(&ifmgd->request_smps_work);
#endif
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	atbm_cancel_work_sync(&ifmgd->monitor_work);
#endif
	atbm_cancel_work_sync(&ifmgd->beacon_connection_loss_work);
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	if (atbm_del_timer_sync(&ifmgd->timer))
		set_bit(TMR_RUNNING_TIMER, &ifmgd->timers_running);
#endif
#ifdef CONFIG_ATBM_SUPPORT_CHANSWITCH
	atbm_cancel_work_sync(&ifmgd->chswitch_work);
	if (atbm_del_timer_sync(&ifmgd->chswitch_timer))
		set_bit(TMR_RUNNING_CHANSW, &ifmgd->timers_running);
#endif
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	/* these will just be re-established on connection */
	atbm_del_timer_sync(&ifmgd->conn_mon_timer);
	atbm_del_timer_sync(&ifmgd->bcn_mon_timer);
#endif
}
#endif

#if defined (CONFIG_PM)
void ieee80211_sta_restart(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;

	if (!ifmgd->associated)
		return;

	if (sdata->flags & IEEE80211_SDATA_DISCONNECT_RESUME) {
		sdata->flags &= ~IEEE80211_SDATA_DISCONNECT_RESUME;
		mutex_lock(&ifmgd->mtx);
		if (ifmgd->associated) {
#ifdef CONFIG_MAC80211_ATBM_VERBOSE_DEBUG
			wiphy_debug(sdata->local->hw.wiphy,
				    "%s: driver requested disconnect after resume.\n",
				    sdata->name);
#endif
			ieee80211_sta_connection_lost(sdata,
				ifmgd->associated->bssid,
				WLAN_REASON_UNSPECIFIED);
			mutex_unlock(&ifmgd->mtx);
			return;
		}
		mutex_unlock(&ifmgd->mtx);
	}
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	if (test_and_clear_bit(TMR_RUNNING_TIMER, &ifmgd->timers_running))
		atbm_add_timer(&ifmgd->timer);
#endif
#ifdef CONFIG_ATBM_SUPPORT_CHANSWITCH
	if (test_and_clear_bit(TMR_RUNNING_CHANSW, &ifmgd->timers_running))
		atbm_add_timer(&ifmgd->chswitch_timer);
#endif
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	ieee80211_sta_reset_beacon_monitor(sdata);
	ieee80211_restart_sta_timer(sdata, true);
	ieee80211_queue_work(&sdata->local->hw, &sdata->u.mgd.monitor_work);
#endif
}
#endif

/* interface setup */
void ieee80211_sta_setup_sdata(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_if_managed *ifmgd;

	ifmgd = &sdata->u.mgd;
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	ATBM_INIT_WORK(&ifmgd->monitor_work, ieee80211_sta_monitor_work);
#endif
#ifdef CONFIG_ATBM_SUPPORT_CHANSWITCH
	ATBM_INIT_WORK(&ifmgd->chswitch_work, ieee80211_chswitch_work);
	atbm_setup_timer(&ifmgd->chswitch_timer, ieee80211_chswitch_timer,
				(unsigned long) sdata);
#endif
	ATBM_INIT_WORK(&ifmgd->beacon_connection_loss_work,
		  ieee80211_beacon_connection_loss_work);
#ifdef CONFIG_ATBM_SMPS
	ATBM_INIT_WORK(&ifmgd->request_smps_work, ieee80211_request_smps_work);
#endif
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	atbm_setup_timer(&ifmgd->timer, ieee80211_sta_timer,
		    (unsigned long) sdata);
	atbm_setup_timer(&ifmgd->bcn_mon_timer, ieee80211_sta_bcn_mon_timer,
		    (unsigned long) sdata);
	atbm_setup_timer(&ifmgd->conn_mon_timer, ieee80211_sta_conn_mon_timer,
		    (unsigned long) sdata);
#endif
	/*
	setup_timer(&ifmgd->scan_delay_timer,ieee80211_scan_delay_timer,
			(unsigned long)sdata);
	*/

	ifmgd->flags = 0;

	mutex_init(&ifmgd->mtx);

	/* Disable UAPSD for sta by default */
	sdata->local->uapsd_queues = 0;
	if (sdata->local->hw.flags & IEEE80211_HW_SUPPORTS_DYNAMIC_SMPS)
		ifmgd->req_smps = IEEE80211_SMPS_AUTOMATIC;
	else
		ifmgd->req_smps = IEEE80211_SMPS_OFF;
#ifdef CONFIG_MAC80211_ATBM_ROAMING_CHANGES
	ifmgd->roaming = 0;
#endif
}

/* scan finished notification */
void ieee80211_mlme_notify_scan_completed(struct ieee80211_local *local)
{
	struct ieee80211_sub_if_data *sdata;
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	/* Restart STA timers */
#if 0
	rcu_read_lock();
	list_for_each_entry_rcu(sdata, &local->interfaces, list){
		printk(KERN_ERR "%s:sdata(%x)++++++++\n",__func__,sdata);
		if(ieee80211_sdata_running(sdata))
			ieee80211_restart_sta_timer(sdata, false);
		printk(KERN_ERR "%s:sdata(%x)--------\n",__func__,sdata);
	}
	rcu_read_unlock();
#else
	mutex_lock(&local->iflist_mtx);
	list_for_each_entry(sdata, &local->interfaces, list) {
		if(ieee80211_sdata_running(sdata))
			ieee80211_restart_sta_timer(sdata, false);
	}
	mutex_unlock(&local->iflist_mtx);
#endif
#else
	mutex_lock(&local->iflist_mtx);
	list_for_each_entry(sdata, &local->interfaces, list) {
		if(ieee80211_sdata_running(sdata)&&(sdata->vif.type == NL80211_IFTYPE_STATION))
			ieee80211_queue_work(&sdata->local->hw, &sdata->work);
	}
	mutex_unlock(&local->iflist_mtx);
#endif
}
#ifdef CONFIG_ATBM_PM_QOS
int ieee80211_max_network_latency(struct notifier_block *nb,
				  unsigned long data, void *dummy)
{
	s32 latency_usec = (s32) data;
	struct ieee80211_local *local =
		container_of(nb, struct ieee80211_local,
			     network_latency_notifier);

	mutex_lock(&local->iflist_mtx);
	ieee80211_recalc_ps(local, latency_usec);
	mutex_unlock(&local->iflist_mtx);

	return 0;
}
#endif
/* config hooks */
static enum work_done_result
ieee80211_probe_auth_done(struct ieee80211_work *wk,
			  struct sk_buff *skb)
{
	struct ieee80211_local *local = wk->sdata->local;
	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,11,0))
	struct wireless_dev *wdev = wk->sdata->dev->ieee80211_ptr;
	#endif
	
	if (!skb) {
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,11,0))
		cfg80211_send_auth_timeout(wk->sdata->dev, wk->filter_ta);
#else
		atbm_wdev_lock(wdev);
		cfg80211_auth_timeout(wk->sdata->dev, wk->filter_ta);
		atbm_wdev_unlock(wdev);
#endif
		goto destroy;
	}

	if (wk->type == IEEE80211_WORK_AUTH) {
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,11,0))
		cfg80211_send_rx_auth(wk->sdata->dev, skb->data, skb->len);
#else
		{
			atbm_wdev_lock(wdev);
			cfg80211_rx_mlme_mgmt(wk->sdata->dev, skb->data, skb->len);
			atbm_wdev_unlock(wdev);
		}
#endif
		goto destroy;
	}
	mutex_lock(&wk->sdata->u.mgd.mtx);
	ieee80211_rx_mgmt_probe_resp(wk->sdata, skb);
	mutex_unlock(&wk->sdata->u.mgd.mtx);

	wk->type = IEEE80211_WORK_AUTH;
	wk->probe_auth.tries = 0;
	return WORK_DONE_REQUEUE;
 destroy:
	if (wk->probe_auth.synced)
		drv_finish_tx_sync(local, wk->sdata, wk->filter_ta,
				   IEEE80211_TX_SYNC_AUTH);

	return WORK_DONE_DESTROY;
}

int ieee80211_mgd_auth(struct ieee80211_sub_if_data *sdata,
		       struct cfg80211_auth_request *req)
{
	const u8 *ssid;
	struct ieee80211_work *wk;
	u16 auth_alg;
	struct ieee80211_local *local = sdata->local;
	size_t ie_len = 0;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 4, 0))
	if (req->local_state_change)
		return 0; /* no need to update mac80211 state */
#endif
	switch (req->auth_type) {
	case NL80211_AUTHTYPE_OPEN_SYSTEM:
		auth_alg = WLAN_AUTH_OPEN;
		break;
	case NL80211_AUTHTYPE_SHARED_KEY:
#ifdef CONFIG_ATBM_USE_SW_ENC
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(5, 4, 0))
		if (IS_ERR(sdata->local->wep_tx_tfm))
			return -EOPNOTSUPP;
#endif
		auth_alg = WLAN_AUTH_SHARED_KEY;
		break;
#else
		return -EOPNOTSUPP;
#endif
	case NL80211_AUTHTYPE_FT:
		auth_alg = WLAN_AUTH_FT;
		break;
	case NL80211_AUTHTYPE_NETWORK_EAP:
		auth_alg = WLAN_AUTH_LEAP;
		break;
#ifdef CONFIG_ATBM_SUPPORT_SAE
//#if (LINUX_VERSION_CODE > KERNEL_VERSION(4, 0, 0))
	case NL80211_AUTHTYPE_SAE:
		auth_alg = WLAN_AUTH_SAE;
		break;
//#endif
#endif
	default:
		return -EOPNOTSUPP;
	}
	ie_len = req->ie_len;
#ifdef CONFIG_ATBM_SUPPORT_SAE
//#if (LINUX_VERSION_CODE > KERNEL_VERSION(4, 0, 0)) && (LINUX_VERSION_CODE < KERNEL_VERSION(4, 19, 0))
#if	(LINUX_VERSION_CODE < KERNEL_VERSION(4, 19, 0))

	ie_len += req->sae_data_len;
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0))
        ie_len += req->auth_data_len;
#endif
#endif
	wk = atbm_kzalloc(sizeof(*wk) + ie_len , GFP_KERNEL);
	if (!wk)
		return -ENOMEM;

	/*
	*release prev bss , and assign current bss for later user
	*/
	mutex_lock(&local->mtx);
	ieee80211_free_authen_bss(sdata);
	ieee80211_assign_authen_bss(sdata,req->bss);
	mutex_unlock(&local->mtx);
	
	memcpy(wk->filter_ta, req->bss->bssid, ETH_ALEN);
#ifdef CONFIG_ATBM_SUPPORT_SAE
//#if (LINUX_VERSION_CODE > KERNEL_VERSION(4, 0, 0)) && (LINUX_VERSION_CODE < KERNEL_VERSION(4, 19, 0))
#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 19, 0))

	if (req->sae_data_len >= 4) {
		__le16 *pos = (__le16 *) req->sae_data;
		wk->probe_auth.sae_trans = le16_to_cpu(pos[0]);
		wk->probe_auth.sae_status = le16_to_cpu(pos[1]);
		memcpy(wk->ie, req->sae_data + 4,
		       req->sae_data_len - 4);
		wk->ie_len += req->sae_data_len - 4;
	}
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0))
	if (req->auth_data_len >= 4) {
                __le16 *pos = (__le16 *) req->auth_data;
                wk->probe_auth.sae_trans = le16_to_cpu(pos[0]);
                wk->probe_auth.sae_status = le16_to_cpu(pos[1]);
                memcpy(wk->ie, req->auth_data + 4,
                       req->auth_data_len - 4);
                wk->ie_len += req->auth_data_len - 4;
        }
#endif
#endif
	if (req->ie && req->ie_len) {
		memcpy(&wk->ie[wk->ie_len], req->ie, req->ie_len);
		wk->ie_len += req->ie_len;
	}

	if (req->key && req->key_len) {
		wk->probe_auth.key_len = req->key_len;
		wk->probe_auth.key_idx = req->key_idx;
		memcpy(wk->probe_auth.key, req->key, req->key_len);
	}

	ssid = ieee80211_bss_get_ie(req->bss, ATBM_WLAN_EID_SSID);
	if(ssid){
		memcpy(wk->probe_auth.ssid, ssid + 2, ssid[1]);
		wk->probe_auth.ssid_len = ssid[1];
	}else {
		atbm_printk_mgmt("%s:no ssid\n",__func__);
		wk->probe_auth.ssid_len = 0;
	}

	wk->probe_auth.algorithm = auth_alg;
	wk->probe_auth.privacy = req->bss->capability & WLAN_CAPABILITY_PRIVACY;
	wk->probe_auth.bss = req->bss;

	/* if we already have a probe, don't probe again */
	if (req->bss->proberesp_ies || req->bss->beacon_ies)
		wk->type = IEEE80211_WORK_AUTH;
	else
		wk->type = IEEE80211_WORK_DIRECT_PROBE;
	wk->chan = req->bss->channel;
	wk->chan_type = NL80211_CHAN_NO_HT;
	wk->sdata = sdata;
	wk->done = ieee80211_probe_auth_done;
	atbm_printk_always("authen:(%pM),ssid(%s)\n",wk->filter_ta,wk->probe_auth.ssid);
	ieee80211_add_work(wk);
	return 0;
}

/* create and insert a dummy station entry */
static int ieee80211_pre_assoc(struct ieee80211_sub_if_data *sdata,
				u8 *bssid) {
	struct sta_info *sta;
	int err;

	sta = sta_info_alloc(sdata, bssid, GFP_KERNEL);
	if (!sta)
		return -ENOMEM;

	sta->dummy = true;

	err = sta_info_insert(sta);
	sta = NULL;
	if (err) {
		atbm_printk_err( "%s: failed to insert Dummy STA(%d)\n", sdata->name, err);
		return err;
	}

	return 0;
}

static enum work_done_result ieee80211_assoc_done(struct ieee80211_work *wk,
						  struct sk_buff *skb)
{
	struct ieee80211_local *local = wk->sdata->local;
	struct atbm_ieee80211_mgmt *mgmt;
	struct ieee80211_rx_status *rx_status;
	struct ieee802_atbm_11_elems elems;
	struct cfg80211_bss *cbss = wk->assoc.bss;
	u16 status;
	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,11,0))
	struct wireless_dev *wdev = wk->sdata->dev->ieee80211_ptr;
	#endif
	
	if (!skb) {
		sta_info_destroy_addr(wk->sdata, cbss->bssid);
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,11,0))
		cfg80211_send_assoc_timeout(wk->sdata->dev, wk->filter_ta);
#else
		atbm_wdev_lock(wdev);
		cfg80211_assoc_timeout(wk->sdata->dev,cbss);
		atbm_wdev_unlock(wdev);
#endif
		mutex_lock(&local->mtx);
		atbm_printk_err( "ieee80211_assoc_done err\n");
		ieee80211_cancle_connecting_work(wk->sdata,wk->filter_ta,true);
		mutex_unlock(&local->mtx);
		goto destroy;
	}

	if (wk->type == IEEE80211_WORK_ASSOC_BEACON_WAIT) {
		mutex_lock(&wk->sdata->u.mgd.mtx);
		rx_status = (void *) skb->cb;
		ieee802_11_parse_elems(skb->data + 24 + 12, skb->len - 24 - 12, &elems);
		ieee80211_rx_bss_info(wk->sdata, (void *)skb->data, skb->len, rx_status,
				      &elems, true);
		mutex_unlock(&wk->sdata->u.mgd.mtx);

		wk->type = IEEE80211_WORK_ASSOC;
		/* not really done yet */
		return WORK_DONE_REQUEUE;
	}

	mgmt = (void *)skb->data;
	status = le16_to_cpu(mgmt->u.assoc_resp.status_code);

	if (status == WLAN_STATUS_SUCCESS) {
		if (wk->assoc.synced)
			drv_finish_tx_sync(local, wk->sdata, wk->filter_ta,
					   IEEE80211_TX_SYNC_ASSOC);

		mutex_lock(&wk->sdata->u.mgd.mtx);
		if (!ieee80211_assoc_success(wk, mgmt, skb->len)) {
			mutex_unlock(&wk->sdata->u.mgd.mtx);
			/* oops -- internal error -- send timeout for now */
			sta_info_destroy_addr(wk->sdata, cbss->bssid);
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,11,0))
			cfg80211_send_assoc_timeout(wk->sdata->dev,
						    wk->filter_ta);
#else
			atbm_wdev_lock(wdev);
			cfg80211_assoc_timeout(wk->sdata->dev,
						    cbss);
			atbm_wdev_unlock(wdev);
#endif
			return WORK_DONE_DESTROY;
		}

		mutex_unlock(&wk->sdata->u.mgd.mtx);
	} else {
		mutex_lock(&local->mtx);
		atbm_printk_err( "%s:status(%d)\n",__func__,status);
		ieee80211_cancle_connecting_work(wk->sdata,wk->filter_ta,true);
		mutex_unlock(&local->mtx);
		/* assoc failed - destroy the dummy station entry */
		sta_info_destroy_addr(wk->sdata, cbss->bssid);
	}
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,4,0))  && !defined(COMPAT_KERNEL_RELEASE)
	cfg80211_send_rx_assoc(wk->sdata->dev, skb->data, skb->len);
#elif (LINUX_VERSION_CODE < KERNEL_VERSION(3,11,0))
	cfg80211_send_rx_assoc(wk->sdata->dev, cbss,skb->data, skb->len);
#elif (LINUX_VERSION_CODE < KERNEL_VERSION(3,18,0))
/*
void cfg80211_rx_assoc_resp(struct net_device *dev, struct cfg80211_bss *bss,
			    const u8 *buf, size_t len)
*/
	atbm_wdev_lock(wdev);
	cfg80211_rx_assoc_resp(wk->sdata->dev,cbss,skb->data,skb->len);
	atbm_wdev_unlock(wdev);
#elif (LINUX_VERSION_CODE > KERNEL_VERSION(5,0,0))
	atbm_wdev_lock(wdev);
	cfg80211_rx_assoc_resp(wk->sdata->dev,cbss,skb->data,skb->len, -1, NULL, 0);
	atbm_wdev_unlock(wdev);
#else
	atbm_wdev_lock(wdev);
	cfg80211_rx_assoc_resp(wk->sdata->dev,cbss,skb->data,skb->len, -1);
	atbm_wdev_unlock(wdev);
#endif
 destroy:
	if (wk->assoc.synced)
		drv_finish_tx_sync(local, wk->sdata, wk->filter_ta,
				   IEEE80211_TX_SYNC_ASSOC);

	return WORK_DONE_DESTROY;
}

int ieee80211_mgd_assoc(struct ieee80211_sub_if_data *sdata,
			struct cfg80211_assoc_request *req)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_bss *bss = (void *)req->bss->priv;
	struct ieee80211_work *wk;
	const u8 *ssid;
	int i, err;
	struct sta_info * sta = NULL;
	
	mutex_lock(&ifmgd->mtx);
	if (ifmgd->associated) {
		if (!req->prev_bssid ||
		    memcmp(req->prev_bssid, ifmgd->associated->bssid,
			   ETH_ALEN)) {
			/*
			 * We are already associated and the request was not a
			 * reassociation request from the current BSS, so
			 * reject it.
			 */
			mutex_unlock(&ifmgd->mtx);
			return -EALREADY;
		}

#ifdef CONFIG_MAC80211_ATBM_ROAMING_CHANGES
		ifmgd->roaming = 1;
#endif
		/* Trying to reassociate - clear previous association state */
		ieee80211_set_disassoc(sdata, true, false);
#ifdef CONFIG_MAC80211_ATBM_ROAMING_CHANGES
		ifmgd->roaming = 0;
#endif
	}
	mutex_unlock(&ifmgd->mtx);

	wk = atbm_kzalloc(sizeof(*wk) + req->ie_len, GFP_KERNEL);
	if (!wk)
		return -ENOMEM;

	/*
	 * create a dummy station info entry in order
	 * to start accepting incoming EAPOL packets from the station
	 */
	err = ieee80211_pre_assoc(sdata, req->bss->bssid);
	if (err) {
		atbm_kfree(wk);
		return err;
	}

	ifmgd->flags &= ~IEEE80211_STA_DISABLE_11N;
	ifmgd->flags &= ~IEEE80211_STA_NULLFUNC_ACKED;

	ifmgd->beacon_crc_valid = false;

	for (i = 0; i < req->crypto.n_ciphers_pairwise; i++)
		if (req->crypto.ciphers_pairwise[i] == WLAN_CIPHER_SUITE_WEP40 ||
		    req->crypto.ciphers_pairwise[i] == WLAN_CIPHER_SUITE_TKIP ||
		    req->crypto.ciphers_pairwise[i] == WLAN_CIPHER_SUITE_WEP104)
			ifmgd->flags |= IEEE80211_STA_DISABLE_11N;
		
	rcu_read_lock();
	sta = sta_info_get_rx(sdata, req->bss->bssid);
	if(sta){
		sta_info_set_mgmt_suit(sta,&req->crypto);
	}else{
		WARN_ON(1);
	}
	rcu_read_unlock();
	
	if (req->ie && req->ie_len) {
		memcpy(wk->ie, req->ie, req->ie_len);
		wk->ie_len = req->ie_len;
	} else
		wk->ie_len = 0;

	wk->assoc.bss = req->bss;

	memcpy(wk->filter_ta, req->bss->bssid, ETH_ALEN);
	
	/* new association always uses requested smps mode */
	if (ifmgd->req_smps == IEEE80211_SMPS_AUTOMATIC) {
		if (ifmgd->powersave)
			ifmgd->ap_smps = IEEE80211_SMPS_DYNAMIC;
		else
			ifmgd->ap_smps = IEEE80211_SMPS_OFF;
	} else
		ifmgd->ap_smps = ifmgd->req_smps;

	wk->assoc.smps = ifmgd->ap_smps;
	/*
	 * IEEE802.11n does not allow TKIP/WEP as pairwise ciphers in HT mode.
	 * We still associate in non-HT mode (11a/b/g) if any one of these
	 * ciphers is configured as pairwise.
	 * We can set this to true for non-11n hardware, that'll be checked
	 * separately along with the peer capabilities.
	 */
	wk->assoc.use_11n = !(ifmgd->flags & IEEE80211_STA_DISABLE_11N);
	wk->assoc.capability = req->bss->capability;
	wk->assoc.wmm_used = bss->wmm_used &&
			     (local->hw.queues >= IEEE80211_NUM_ACS);
	wk->assoc.supp_rates = bss->supp_rates;
	wk->assoc.supp_rates_len = bss->supp_rates_len;
	wk->assoc.ht_information_ie =
		ieee80211_bss_get_ie(req->bss, ATBM_WLAN_EID_HT_INFORMATION);

	if (bss->wmm_used && bss->uapsd_supported &&
	    (sdata->local->hw.flags & IEEE80211_HW_SUPPORTS_UAPSD)) {
	    #if 0
		if (req->uapsd != -1)
			sdata->local->uapsd_queues = req->uapsd;
	    #endif
		wk->assoc.uapsd_used = true;
		ifmgd->flags |= IEEE80211_STA_UAPSD_ENABLED;
	} else {
		wk->assoc.uapsd_used = false;
		ifmgd->flags &= ~IEEE80211_STA_UAPSD_ENABLED;
	}

	ssid = ieee80211_bss_get_ie(req->bss, ATBM_WLAN_EID_SSID);
	memcpy(wk->assoc.ssid, ssid + 2, ssid[1]);
	wk->assoc.ssid_len = ssid[1];

	if (req->prev_bssid)
		memcpy(wk->assoc.prev_bssid, req->prev_bssid, ETH_ALEN);

	wk->chan = req->bss->channel;
	wk->chan_type = NL80211_CHAN_NO_HT;
	wk->sdata = sdata;
	wk->done = ieee80211_assoc_done;
	if (!bss->dtim_period &&
	    sdata->local->hw.flags & IEEE80211_HW_NEED_DTIM_PERIOD)
		wk->type = IEEE80211_WORK_ASSOC_BEACON_WAIT;
	else
		wk->type = IEEE80211_WORK_ASSOC;

	if (req->use_mfp) {
		ifmgd->mfp = IEEE80211_MFP_REQUIRED;
		ifmgd->flags |= IEEE80211_STA_MFP_ENABLED;
	} else {
		ifmgd->mfp = IEEE80211_MFP_DISABLED;
		ifmgd->flags &= ~IEEE80211_STA_MFP_ENABLED;
	}

	if (req->crypto.control_port)
		ifmgd->flags |= IEEE80211_STA_CONTROL_PORT;
	else
		ifmgd->flags &= ~IEEE80211_STA_CONTROL_PORT;

	sdata->control_port_protocol = req->crypto.control_port_ethertype;
	sdata->control_port_no_encrypt = req->crypto.control_port_no_encrypt;
	ieee80211_add_work(wk);
	ieee80211_start_connecting_work(wk->sdata,wk->chan,wk->filter_ta);
	/*
	*release authen_bss because at associate state cfg80211
	*has handled the bss
	*/
	mutex_lock(&local->mtx);
	ieee80211_free_authen_bss(sdata);
	mutex_unlock(&local->mtx);
	return 0;
}

int ieee80211_mgd_deauth(struct ieee80211_sub_if_data *sdata,
			 struct cfg80211_deauth_request *req,
			 void *cookie)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	u8 bssid[ETH_ALEN];
	bool assoc_bss = false;
	struct cfg80211_bss *prev_bss = NULL;
	
	LOCAL_SET_CONNECT_STOP(local);

	/*
	*deauthen ,release authen_bss
	*/
	mutex_lock(&local->mtx);
	prev_bss = rcu_dereference(ifmgd->authen_bss);
	if(prev_bss){
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 4, 0))
		if(memcmp(prev_bss->bssid,req->bss->bssid,ETH_ALEN)==0)
#else
		if(memcmp(prev_bss->bssid,req->bssid,ETH_ALEN)==0)
#endif
		{
			ieee80211_free_authen_bss(sdata);
		}
	}
	mutex_unlock(&local->mtx);

	
	mutex_lock(&ifmgd->mtx);
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 4, 0))
	memcpy(bssid, req->bss->bssid, ETH_ALEN);
	if (ifmgd->associated == req->bss)
#else
	memcpy(bssid, req->bssid, ETH_ALEN);
	if(ifmgd->associated&&(atbm_compare_ether_addr(ifmgd->associated->bssid, req->bssid)==0))
#endif
	{
		ieee80211_set_disassoc(sdata, false, true);
		mutex_unlock(&ifmgd->mtx);
		assoc_bss = true;
	} else {
		bool not_auth_yet = false;
		struct ieee80211_work *tmp, *wk = NULL;

		mutex_unlock(&ifmgd->mtx);

		mutex_lock(&local->mtx);
		list_for_each_entry(tmp, &local->work_list, list) {
			if (tmp->sdata != sdata)
				continue;

			if (tmp->type != IEEE80211_WORK_DIRECT_PROBE &&
			    tmp->type != IEEE80211_WORK_AUTH &&
			    tmp->type != IEEE80211_WORK_ASSOC &&
			    tmp->type != IEEE80211_WORK_ASSOC_BEACON_WAIT)
				continue;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 4, 0))
			if (memcmp(req->bss->bssid, tmp->filter_ta, ETH_ALEN))
#else
			if (memcmp(req->bssid, tmp->filter_ta, ETH_ALEN))
#endif
				continue;

			not_auth_yet = tmp->type == IEEE80211_WORK_DIRECT_PROBE;
			list_del_rcu(&tmp->list);
			synchronize_rcu();
			wk = tmp;
			
			break;
		}
		ieee80211_cancle_connecting_work(sdata,bssid,false);
		/*
		*has pending so queue work
		*/
		if((list_empty(&local->work_list))&&
			(local->roc_pendding || (local->scan_req &&!local->scanning))){
			atbm_printk_mgmt("%s:queue work_work for pending scan or listenning",__func__);
			ieee80211_queue_work(&local->hw, &local->work_work);
		}
		mutex_unlock(&local->mtx);

		if (wk && wk->type == IEEE80211_WORK_ASSOC) {
			/* clean up dummy sta & TX sync */
			sta_info_destroy_addr(wk->sdata, wk->filter_ta);
			if (wk->assoc.synced)
				drv_finish_tx_sync(local, wk->sdata,
						   wk->filter_ta,
						   IEEE80211_TX_SYNC_ASSOC);
		} else if (wk && wk->type == IEEE80211_WORK_AUTH) {
			if (wk->probe_auth.synced)
				drv_finish_tx_sync(local, wk->sdata,
						   wk->filter_ta,
						   IEEE80211_TX_SYNC_AUTH);
		}
		atbm_kfree(wk);

		/*
		 * If somebody requests authentication and we haven't
		 * sent out an auth frame yet there's no need to send
		 * out a deauth frame either. If the state was PROBE,
		 * then this is the case. If it's AUTH we have sent a
		 * frame, and if it's IDLE we have completed the auth
		 * process already.
		 */
		if (not_auth_yet) {
#if (LINUX_VERSION_CODE<KERNEL_VERSION(3,4,0))
			__cfg80211_auth_canceled(sdata->dev, bssid);
#endif
			return 0;
		}
	}

	atbm_printk_always( "%s: deauthenticating from %pM by local choice (reason=%d)\n",
	       sdata->name, bssid, req->reason_code);
#if((LINUX_VERSION_CODE>=KERNEL_VERSION(3,4,0))&&(LINUX_VERSION_CODE<KERNEL_VERSION(3,6,0)))
	ieee80211_send_deauth_disassoc(sdata, bssid, IEEE80211_STYPE_DEAUTH,
				       req->reason_code, cookie,
				       true);
#else
	ieee80211_send_deauth_disassoc(sdata, bssid, IEEE80211_STYPE_DEAUTH,
				       req->reason_code, cookie,
				       !req->local_state_change);
#endif
	if (assoc_bss)
		sta_info_flush(sdata->local, sdata);

	mutex_lock(&sdata->local->mtx);
	ieee80211_recalc_idle(sdata->local);
	mutex_unlock(&sdata->local->mtx);

	return 0;
}

int ieee80211_mgd_disassoc(struct ieee80211_sub_if_data *sdata,
			   struct cfg80211_disassoc_request *req,
			   void *cookie)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	u8 bssid[ETH_ALEN];

	mutex_lock(&ifmgd->mtx);

	/*
	 * cfg80211 should catch this ... but it's racy since
	 * we can receive a disassoc frame, process it, hand it
	 * to cfg80211 while that's in a locked section already
	 * trying to tell us that the user wants to disconnect.
	 */
	if (ifmgd->associated != req->bss) {
		mutex_unlock(&ifmgd->mtx);
		return -ENOLINK;
	}

	atbm_printk_mgmt( "%s: disassociating from %pM by local choice (reason=%d)\n",
	       sdata->name, req->bss->bssid, req->reason_code);

	memcpy(bssid, req->bss->bssid, ETH_ALEN);
	ieee80211_set_disassoc(sdata, false, true);

	mutex_unlock(&ifmgd->mtx);

	ieee80211_send_deauth_disassoc(sdata, req->bss->bssid,
			IEEE80211_STYPE_DISASSOC, req->reason_code,
			cookie, !req->local_state_change);
	sta_info_flush(sdata->local, sdata);
	/*
	*disassoc ,release authen_bss
	*/
	mutex_lock(&sdata->local->mtx);
	ieee80211_free_authen_bss(sdata);
	ieee80211_recalc_idle(sdata->local);
	mutex_unlock(&sdata->local->mtx);

	return 0;
}
#ifdef CONFIG_ATBM_MAC80211_NO_USE
void ieee80211_cqm_rssi_notify(struct ieee80211_vif *vif,
			       enum nl80211_cqm_rssi_threshold_event rssi_event,
			       gfp_t gfp)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);

	trace_api_cqm_rssi_notify(sdata, rssi_event);

	cfg80211_cqm_rssi_notify(sdata->dev, rssi_event, gfp);
}
#endif
//EXPORT_SYMBOL(ieee80211_cqm_rssi_notify);
#if 0
void ieee80211_cqm_beacon_miss_notify(struct ieee80211_vif *vif,
				      gfp_t gfp)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);

	cfg80211_cqm_beacon_miss_notify(sdata->dev, gfp);
}

//EXPORT_SYMBOL(ieee80211_cqm_beacon_miss_notify);

void ieee80211_cqm_tx_fail_notify(struct ieee80211_vif *vif,
				      gfp_t gfp)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);

	cfg80211_cqm_tx_fail_notify(sdata->dev, gfp);
}
//EXPORT_SYMBOL(ieee80211_cqm_tx_fail_notify);

void ieee80211_p2p_noa_notify(struct ieee80211_vif *vif,
			      struct cfg80211_p2p_ps *p2p_ps,
			      gfp_t gfp)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);

	cfg80211_p2p_noa_notify(sdata->dev, p2p_ps, gfp);
}
//EXPORT_SYMBOL(ieee80211_p2p_noa_notify);

void ieee80211_driver_hang_notify(struct ieee80211_vif *vif,
					gfp_t gfp)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);

	cfg80211_driver_hang_notify(sdata->dev, gfp);
}
//EXPORT_SYMBOL(ieee80211_driver_hang_notify);
#endif
#ifdef CONFIG_ATBM_MAC80211_NO_USE
unsigned char ieee80211_get_operstate(struct ieee80211_vif *vif)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);
	return sdata->dev->operstate;
}
#endif
#ifdef CONFIG_ATBM_RADAR_DETECT
#ifdef CONFIG_ATBM_5G_PRETEND_2G
void ieee80211_dfs_cac_timer_work(struct atbm_work_struct *work)
{
#if (LINUX_VERSION_CODE > KERNEL_VERSION(3, 9, 0))
	struct atbm_delayed_work *delayed_work = atbm_to_delayed_work(work);
	struct ieee80211_sub_if_data *sdata =
		container_of(atbm_delayed_work, struct ieee80211_sub_if_data,
			     dfs_cac_timer_work);
	mutex_lock(&sdata->local->mtx);
	sdata->radar_required = false;
	if (sdata->wdev.cac_started) {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 14, 0))
		cfg80211_cac_event(sdata->dev, &sdata->dfs_cac_chan_def,
				   NL80211_RADAR_CAC_FINISHED,
				   GFP_KERNEL);
#else
		cfg80211_cac_event(sdata->dev, NL80211_RADAR_CAC_ABORTED,
				   GFP_KERNEL);
#endif
	}
	mutex_unlock(&sdata->local->mtx);
#endif
}
void ieee80211_dfs_cac_abort(struct ieee80211_sub_if_data *sdata)
{
	atbm_cancel_delayed_work_sync(&sdata->dfs_cac_timer_work);
	sdata->radar_required = false;
#if (LINUX_VERSION_CODE > KERNEL_VERSION(3, 9, 0))
	if (sdata->wdev.cac_started) {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 14, 0))
		cfg80211_cac_event(sdata->dev,&sdata->dfs_cac_chan_def,NL80211_RADAR_CAC_ABORTED,
				   GFP_KERNEL);
#else
		cfg80211_cac_event(sdata->dev, NL80211_RADAR_CAC_ABORTED,
				   GFP_KERNEL);
#endif
	}
#endif
}
#endif
#endif
//EXPORT_SYMBOL(ieee80211_get_operstate);
