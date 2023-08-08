/*
 * mac80211 configuration hooks for cfg80211
 *
 * Copyright 2006-2010	Johannes Berg <johannes@sipsolutions.net>
 *
 * This file is GPLv2 as found in COPYING.
 */

#include <linux/ieee80211.h>
#include <linux/nl80211.h>
#include <linux/rtnetlink.h>
#include <linux/slab.h>
#include <net/net_namespace.h>
#include <linux/rcupdate.h>
#include <linux/if_ether.h>
#include <net/cfg80211.h>
#include <net/atbm_mac80211.h>
#include <net/ieee80211_radiotap.h>
#include <linux/if_arp.h>

#include "ieee80211_i.h"
#include "driver-ops.h"
#include "cfg.h"
#include "rate.h"
#include "mesh.h"
#include "wext_cfg.h"
#include "atbm_workqueue.h"

#ifdef CONFIG_ATBM_SUPPORT_P2P
#pragma message("Enable P2P Function")
#else
#pragma message("Disable P2P Function")
#endif
extern  void ieee80211_ap_setchannel_fail(struct ieee80211_sub_if_data *sdata,struct ieee80211_channel *chan,enum nl80211_channel_type channel_type );

extern int ieee80211_set_channel(struct wiphy *wiphy,
				 struct net_device *netdev,
				 struct ieee80211_channel *chan,
				 enum nl80211_channel_type channel_type);
int cfg80211_altmtest_reply(struct wiphy *wiphy,
				const void *data, int len)
{
	return 0;
}
//EXPORT_SYMBOL(cfg80211_altmtest_reply);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0))
static struct wireless_dev *
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,38)) || defined(COMPAT_KERNEL_RELEASE)
static struct net_device *
#else
static int
#endif
ieee80211_add_iface(struct wiphy *wiphy, 
				#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,7,0))
					const char *name,
				#else
					char *name,
				#endif
				#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0))
				unsigned char name_assign_type,
				#endif
			      enum nl80211_iftype type,
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(4, 15, 0))
			      u32 *flags,
#endif
			      struct vif_params *params)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct net_device *dev;
	struct ieee80211_sub_if_data *sdata;
	int err;
	u8  interface_cnt = 0;
#if (LINUX_VERSION_CODE > KERNEL_VERSION(4, 15, 0))
	u32 *flags = params ? &params->flags : NULL ;
#endif
	
	list_for_each_entry_rcu(sdata, &local->interfaces, list){
		
		 if (sdata->vif.type != NL80211_IFTYPE_MONITOR){
		 	interface_cnt++;
		 }else if(!(sdata->u.mntr_flags & MONITOR_FLAG_COOK_FRAMES)){
		 	interface_cnt++;
		 }
	}
	atbm_printk_init("add iftype(%d),if_cnt(%d),flags(%p)\n",type,interface_cnt,flags);
	if(interface_cnt>=2){
		if(type != NL80211_IFTYPE_MONITOR){
			err = -ENOMEM;
			atbm_printk_err("cannot add interface type(%d)\n",type);
			return ERR_PTR(err);
		}

		if(!flags || !(*flags & MONITOR_FLAG_COOK_FRAMES)){
			err = -ENOMEM;
			atbm_printk_err("cannot add interface monitor\n");
			return ERR_PTR(err);
		}
	}
	if(local->only_monitors){
		if(local->monitor_sdata){
			atbm_printk_err("[%s] in monitor mode,so can not add sdata\n",local->monitor_sdata->name);
		}
		err = -ENOMEM;
		return ERR_PTR(err);
	}
	err = ieee80211_if_add(local, name, &dev, type, params);
	if (err)
		return ERR_PTR(err);

	if (type == NL80211_IFTYPE_MONITOR && flags) {
		sdata = IEEE80211_DEV_TO_SUB_IF(dev);
		sdata->u.mntr_flags = *flags;
		/*
		*in cook frame mode ,we use radiotap header. 
		*/
		if (sdata->u.mntr_flags & MONITOR_FLAG_COOK_FRAMES) {
			sdata->dev->type = ARPHRD_IEEE80211_RADIOTAP;
		}
	}
	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0))
	return dev ? dev->ieee80211_ptr :NULL;
	#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,38)) || defined(COMPAT_KERNEL_RELEASE)
	return dev;
	#else
	return 0;
	#endif
}

static int ieee80211_del_iface(struct wiphy *wiphy, 
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0))
	struct wireless_dev *wdev
#else
	struct net_device *dev
#endif
)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0))
	struct net_device *dev = wdev_to_ndev(wdev);
#endif
	ieee80211_if_remove(IEEE80211_DEV_TO_SUB_IF(dev));

	return 0;
}

static int ieee80211_change_iface(struct wiphy *wiphy,
				  struct net_device *dev,
				  enum nl80211_iftype type,
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(4,15,0))
				  u32 *flags,
#endif
				  struct vif_params *params)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	int ret;
#if (LINUX_VERSION_CODE > KERNEL_VERSION(4,15,0))
	u32 *flags = params ? &params->flags : NULL;
#endif

	ret = ieee80211_if_change_type(sdata, type);
	if (ret)
		return ret;

	if (type == NL80211_IFTYPE_AP_VLAN &&
	    params && params->use_4addr == 0)
		RCU_INIT_POINTER(sdata->u.vlan.sta, NULL);
	else if (type == NL80211_IFTYPE_STATION &&
		 params && params->use_4addr >= 0)
		sdata->u.mgd.use_4addr = params->use_4addr;

	if (sdata->vif.type == NL80211_IFTYPE_MONITOR && flags) {

		if (ieee80211_sdata_running(sdata)) {
			/*
			 * Prohibit MONITOR_FLAG_COOK_FRAMES to be
			 * changed while the interface is up.
			 * Else we would need to add a lot of cruft
			 * to update everything:
			 *	cooked_mntrs, monitor and all fif_* counters
			 *	reconfigure hardware
			 */
			if ((*flags & MONITOR_FLAG_COOK_FRAMES) !=
			    (sdata->u.mntr_flags & MONITOR_FLAG_COOK_FRAMES))
				return -EBUSY;

			ieee80211_adjust_monitor_flags(sdata, -1);
			sdata->u.mntr_flags = *flags;
			ieee80211_adjust_monitor_flags(sdata, 1);

			ieee80211_configure_filter(sdata);
		} else {
			/*
			 * Because the interface is down, ieee80211_do_stop
			 * and ieee80211_do_open take care of "everything"
			 * mentioned in the comment above.
			 */
			sdata->u.mntr_flags = *flags;
		}

		/*
		*in cook frame mode ,we use radiotap header. 
		*/
		if (sdata->u.mntr_flags & MONITOR_FLAG_COOK_FRAMES) {
			sdata->dev->type = ARPHRD_IEEE80211_RADIOTAP;
		}
	}

	return 0;
}
int atbm_change_iface_to_monitor(struct net_device *dev)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_hw *hw = &local->hw;
	
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(4, 15, 0))
	return ieee80211_change_iface(hw->wiphy, dev, NL80211_IFTYPE_MONITOR, NULL, NULL);
#else
	return ieee80211_change_iface(hw->wiphy, dev, NL80211_IFTYPE_MONITOR, NULL);
#endif
}

static int ieee80211_add_key(struct wiphy *wiphy, struct net_device *dev,
			     u8 key_idx, bool pairwise, const u8 *mac_addr,
			     struct key_params *params)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct sta_info *sta = NULL;
	struct ieee80211_key *key;
	int err;

	if (!ieee80211_sdata_running(sdata))
		return -ENETDOWN;

	/* reject WEP and TKIP keys if WEP failed to initialize */
	switch (params->cipher) {
	case WLAN_CIPHER_SUITE_WEP40:
	case WLAN_CIPHER_SUITE_TKIP:
	case WLAN_CIPHER_SUITE_WEP104:
#ifdef CONFIG_ATBM_USE_SW_ENC
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(5, 4, 0))
		if (IS_ERR(sdata->local->wep_tx_tfm))
			return -EINVAL;
#endif
#endif
		break;
	default:
		break;
	}

	key = ieee80211_key_alloc(params->cipher, key_idx, params->key_len,
				  params->key, params->seq_len, params->seq);
	if (IS_ERR(key))
		return PTR_ERR(key);

	if (pairwise)
		key->conf.flags |= IEEE80211_KEY_FLAG_PAIRWISE;

	mutex_lock(&sdata->local->sta_mtx);

	if (mac_addr) {
#ifdef CONFIG_MAC80211_ATBM_MESH
		if (ieee80211_vif_is_mesh(&sdata->vif))
			sta = sta_info_get(sdata, mac_addr);
		else
#endif
			sta = sta_info_get_bss(sdata, mac_addr);
		if (!sta) {
			atbm_printk_err("%s:[%pM] is not in sta list\n",__func__,mac_addr);
			ieee80211_key_free(sdata->local, key);
			err = -ENOENT;
			goto out_unlock;
		}
	}

	err = ieee80211_key_link(key, sdata, sta);
	if (err)
		ieee80211_key_free(sdata->local, key);

 out_unlock:
	mutex_unlock(&sdata->local->sta_mtx);

	return err;
}

static int ieee80211_del_key(struct wiphy *wiphy, struct net_device *dev,
			     u8 key_idx, bool pairwise, const u8 *mac_addr)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct sta_info *sta;
	struct ieee80211_key *key = NULL;
	int ret;

	mutex_lock(&local->sta_mtx);
	mutex_lock(&local->key_mtx);

	if (mac_addr) {
		ret = -ENOENT;

		sta = sta_info_get_bss(sdata, mac_addr);
		if (!sta)
			goto out_unlock;

		if (pairwise)
			key = key_mtx_dereference(local, sta->ptk);
		else
			key = key_mtx_dereference(local, sta->gtk[key_idx]);
	} else
		key = key_mtx_dereference(local, sdata->keys[key_idx]);

	if (!key) {
		ret = -ENOENT;
		goto out_unlock;
	}

	__ieee80211_key_free(key);

	ret = 0;
 out_unlock:
	mutex_unlock(&local->key_mtx);
	mutex_unlock(&local->sta_mtx);

	return ret;
}

static int ieee80211_get_key(struct wiphy *wiphy, struct net_device *dev,
			     u8 key_idx, bool pairwise, const u8 *mac_addr,
			     void *cookie,
			     void (*callback)(void *cookie,
					      struct key_params *params))
{
	struct ieee80211_sub_if_data *sdata;
	struct sta_info *sta = NULL;
	u8 seq[6] = {0};
	struct key_params params;
	struct ieee80211_key *key = NULL;
	u64 pn64;
	u32 iv32;
	u16 iv16;
	int err = -ENOENT;

	sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	rcu_read_lock();

	if (mac_addr) {
		sta = sta_info_get_bss(sdata, mac_addr);
		if (!sta)
			goto out;

		if (pairwise)
			key = rcu_dereference(sta->ptk);
		else if (key_idx < NUM_DEFAULT_KEYS)
			key = rcu_dereference(sta->gtk[key_idx]);
	} else
		key = rcu_dereference(sdata->keys[key_idx]);

	if (!key)
		goto out;

	memset(&params, 0, sizeof(params));

	params.cipher = key->conf.cipher;

	switch (key->conf.cipher) {
	case WLAN_CIPHER_SUITE_TKIP:
		iv32 = key->u.tkip.tx.iv32;
		iv16 = key->u.tkip.tx.iv16;

		if (key->flags & KEY_FLAG_UPLOADED_TO_HARDWARE)
			drv_get_tkip_seq(sdata->local,
					 key->conf.hw_key_idx,
					 &iv32, &iv16);

		seq[0] = iv16 & 0xff;
		seq[1] = (iv16 >> 8) & 0xff;
		seq[2] = iv32 & 0xff;
		seq[3] = (iv32 >> 8) & 0xff;
		seq[4] = (iv32 >> 16) & 0xff;
		seq[5] = (iv32 >> 24) & 0xff;
		params.seq = seq;
		params.seq_len = 6;
		break;
	case WLAN_CIPHER_SUITE_CCMP:
		pn64 = atomic64_read(&key->u.ccmp.tx_pn);
		seq[0] = pn64;
		seq[1] = pn64 >> 8;
		seq[2] = pn64 >> 16;
		seq[3] = pn64 >> 24;
		seq[4] = pn64 >> 32;
		seq[5] = pn64 >> 40;
		params.seq = seq;
		params.seq_len = 6;
		break;
	case WLAN_CIPHER_SUITE_AES_CMAC:
		pn64 = atomic64_read(&key->u.aes_cmac.tx_pn);
		seq[0] = pn64;
		seq[1] = pn64 >> 8;
		seq[2] = pn64 >> 16;
		seq[3] = pn64 >> 24;
		seq[4] = pn64 >> 32;
		seq[5] = pn64 >> 40;
		params.seq = seq;
		params.seq_len = 6;
		break;
	}

	params.key = key->conf.key;
	params.key_len = key->conf.keylen;

	callback(cookie, &params);
	err = 0;

 out:
	rcu_read_unlock();
	return err;
}

static int ieee80211_config_default_key(struct wiphy *wiphy,
					struct net_device *dev,
					u8 key_idx, bool uni,
					bool multi)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	ieee80211_set_default_key(sdata, key_idx, uni, multi);

	return 0;
}

static int ieee80211_config_default_mgmt_key(struct wiphy *wiphy,
					     struct net_device *dev,
					     u8 key_idx)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	ieee80211_set_default_mgmt_key(sdata, key_idx);

	return 0;
}

static void rate_idx_to_bitrate(struct rate_info *rate, struct sta_info *sta, int idx)
{
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(sta->local, sta->sdata);
	if (!(rate->flags & RATE_INFO_FLAGS_MCS)) {
		struct ieee80211_supported_band *sband;
		sband = sta->local->hw.wiphy->bands[
				chan_state->conf.channel->band];
		rate->legacy = sband->bitrates[idx].bitrate;
	} else
		rate->mcs = idx;
}

static void sta_set_sinfo(struct sta_info *sta, struct station_info *sinfo)
{
	struct ieee80211_sub_if_data *sdata = sta->sdata;
	struct timespec uptime;

	sinfo->generation = sdata->local->sta_generation;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0))
	sinfo->filled = STATION_INFO_INACTIVE_TIME |
			STATION_INFO_RX_BYTES |
			STATION_INFO_TX_BYTES |
			STATION_INFO_RX_PACKETS |
			STATION_INFO_TX_PACKETS |
			STATION_INFO_TX_RETRIES |
			STATION_INFO_TX_FAILED |
			STATION_INFO_TX_BITRATE |
			STATION_INFO_RX_BITRATE |
			STATION_INFO_RX_DROP_MISC |
			STATION_INFO_BSS_PARAM |
			STATION_INFO_CONNECTED_TIME
			#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 1, 0))
			| STATION_INFO_STA_FLAGS
			#endif
			;
#else
	sinfo->filled = BIT(NL80211_STA_INFO_INACTIVE_TIME) |
			BIT(NL80211_STA_INFO_RX_BYTES) |
			BIT(NL80211_STA_INFO_TX_BYTES) |
			BIT(NL80211_STA_INFO_RX_PACKETS) |
			BIT(NL80211_STA_INFO_TX_PACKETS) |
			BIT(NL80211_STA_INFO_TX_RETRIES) |
			BIT(NL80211_STA_INFO_TX_FAILED) |
			BIT(NL80211_STA_INFO_TX_BITRATE) |
			BIT(NL80211_STA_INFO_RX_BITRATE) |
			BIT(NL80211_STA_INFO_RX_DROP_MISC) |
			BIT(NL80211_STA_INFO_BSS_PARAM) |
			BIT(NL80211_STA_INFO_CONNECTED_TIME) | 
			BIT(NL80211_STA_INFO_STA_FLAGS) 
			;
#endif
	do_posix_clock_monotonic_gettime(&uptime);
	sinfo->connected_time = uptime.tv_sec - sta->last_connected;

	sinfo->inactive_time = jiffies_to_msecs(jiffies - sta->last_rx);
	sinfo->rx_bytes = sta->rx_bytes;
	sinfo->tx_bytes = sta->tx_bytes;
	sinfo->rx_packets = sta->rx_packets;
	sinfo->tx_packets = sta->tx_packets;
	sinfo->tx_retries = sta->tx_retry_count;
	sinfo->tx_failed = sta->tx_retry_failed;
	sinfo->rx_dropped_misc = sta->rx_dropped;

	if ((sta->local->hw.flags & IEEE80211_HW_SIGNAL_DBM) ||
	    (sta->local->hw.flags & IEEE80211_HW_SIGNAL_UNSPEC)) {
	   	#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0))
		sinfo->filled |= STATION_INFO_SIGNAL | STATION_INFO_SIGNAL_AVG;
		#else
		sinfo->filled |= BIT(NL80211_STA_INFO_SIGNAL) | BIT(NL80211_STA_INFO_SIGNAL_AVG);
		#endif
		sinfo->signal = (s8)sta->last_signal;
		sinfo->signal_avg = (s8) -atbm_ewma_read(&sta->avg_signal);
	}

	sinfo->txrate.flags = 0;
	if (sta->last_tx_rate.flags & IEEE80211_TX_RC_MCS)
		sinfo->txrate.flags |= RATE_INFO_FLAGS_MCS;
	if (sta->last_tx_rate.flags & IEEE80211_TX_RC_40_MHZ_WIDTH)
#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0))		
		sinfo->txrate.flags |= RATE_INFO_FLAGS_40_MHZ_WIDTH;
#else
		sinfo->txrate.bw = RATE_INFO_BW_40;
#endif
	if (sta->last_tx_rate.flags & IEEE80211_TX_RC_SHORT_GI)
		sinfo->txrate.flags |= RATE_INFO_FLAGS_SHORT_GI;
	rate_idx_to_bitrate(&sinfo->txrate, sta, sta->last_tx_rate.idx);

	sinfo->rxrate.flags = 0;
	if (sta->last_rx_rate_flag & RX_FLAG_HT)
		sinfo->rxrate.flags |= RATE_INFO_FLAGS_MCS;
	if (sta->last_rx_rate_flag & RX_FLAG_40MHZ)
#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0))		
		sinfo->txrate.flags |= RATE_INFO_FLAGS_40_MHZ_WIDTH;
#else
		sinfo->txrate.bw = RATE_INFO_BW_40;
#endif
	if (sta->last_rx_rate_flag & RX_FLAG_SHORT_GI)
		sinfo->rxrate.flags |= RATE_INFO_FLAGS_SHORT_GI;
	rate_idx_to_bitrate(&sinfo->rxrate, sta, sta->last_rx_rate_idx);
#ifdef CONFIG_MAC80211_ATBM_MESH
	if (ieee80211_vif_is_mesh(&sdata->vif)) {
		sinfo->filled |= STATION_INFO_LLID |
				 STATION_INFO_PLID
	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 1, 0))
				 |
				 STATION_INFO_PLINK_STATE
	#endif
		;
		sinfo->llid = le16_to_cpu(sta->llid);
		sinfo->plid = le16_to_cpu(sta->plid);
		sinfo->plink_state = sta->plink_state;
	}
#endif
	
	sinfo->bss_param.flags = 0;
	if (sdata->vif.bss_conf.use_cts_prot)
		sinfo->bss_param.flags |= BSS_PARAM_FLAGS_CTS_PROT;
	if (sdata->vif.bss_conf.use_short_preamble)
		sinfo->bss_param.flags |= BSS_PARAM_FLAGS_SHORT_PREAMBLE;
	if (sdata->vif.bss_conf.use_short_slot)
		sinfo->bss_param.flags |= BSS_PARAM_FLAGS_SHORT_SLOT_TIME;
	sinfo->bss_param.dtim_period = sdata->vif.bss_conf.ps_dtim_period;
	sinfo->bss_param.beacon_interval = sdata->vif.bss_conf.beacon_int;
	
	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 1, 0))
	sinfo->sta_flags.set = 0;
	sinfo->sta_flags.mask = BIT(NL80211_STA_FLAG_AUTHORIZED) |
				BIT(NL80211_STA_FLAG_SHORT_PREAMBLE) |
				BIT(NL80211_STA_FLAG_WME) |
				BIT(NL80211_STA_FLAG_MFP) |
				BIT(NL80211_STA_FLAG_AUTHENTICATED);
	if (test_sta_flag(sta, WLAN_STA_AUTHORIZED))
		sinfo->sta_flags.set |= BIT(NL80211_STA_FLAG_AUTHORIZED);
	if (test_sta_flag(sta, WLAN_STA_SHORT_PREAMBLE))
		sinfo->sta_flags.set |= BIT(NL80211_STA_FLAG_SHORT_PREAMBLE);
	if (test_sta_flag(sta, WLAN_STA_WME))
		sinfo->sta_flags.set |= BIT(NL80211_STA_FLAG_WME);
	if (test_sta_flag(sta, WLAN_STA_MFP))
		sinfo->sta_flags.set |= BIT(NL80211_STA_FLAG_MFP);
	if (test_sta_flag(sta, WLAN_STA_AUTH))
		sinfo->sta_flags.set |= BIT(NL80211_STA_FLAG_AUTHENTICATED);
	#endif
}


static int ieee80211_dump_station(struct wiphy *wiphy, struct net_device *dev,
				 int idx, u8 *mac, struct station_info *sinfo)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct sta_info *sta;
	int ret = -ENOENT;

	rcu_read_lock();

	sta = sta_info_get_by_idx(sdata, idx);
	if (sta) {
		ret = 0;
		memcpy(mac, sta->sta.addr, ETH_ALEN);
		sta_set_sinfo(sta, sinfo);
	}

	rcu_read_unlock();

	return ret;
}
static int ieee80211_get_station(struct wiphy *wiphy, struct net_device *dev,
				 #if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 16, 0))
				 u8 *mac,
				 #else
				 const u8 *mac,
				 #endif
				 struct station_info *sinfo)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct sta_info *sta;
	int ret = -ENOENT;

	rcu_read_lock();

	sta = sta_info_get_bss(sdata, (u8*)mac);
	if (sta) {
		ret = 0;
		sta_set_sinfo(sta, sinfo);
	}

	rcu_read_unlock();

	return ret;
}

static void ieee80211_config_ap_ssid(struct ieee80211_sub_if_data *sdata,
				     struct beacon_parameters *params)
{
	struct ieee80211_bss_conf *bss_conf = &sdata->vif.bss_conf;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 1, 0))
	if(params->ssid_len&&params->ssid){
		bss_conf->ssid_len = params->ssid_len;

		if (params->ssid_len)
			memcpy(bss_conf->ssid, params->ssid, params->ssid_len);

		bss_conf->hidden_ssid =
			(params->hidden_ssid != NL80211_HIDDEN_SSID_NOT_IN_USE);
	}else
#endif
	{
		struct sk_buff *skb;
		int offset = 0;
		const u8 *ssidie = NULL;
		
		skb = ieee80211_beacon_get(&sdata->local->hw, &sdata->vif);
		if (WARN_ON(!skb))
			return ;

		offset = offsetof(struct atbm_ieee80211_mgmt, u.beacon.variable);
		ssidie = atbm_ieee80211_find_ie(ATBM_WLAN_EID_SSID, skb->data + offset,
					  skb->len - offset);

		if (ssidie) {
			bss_conf->ssid_len = ssidie[1];
			memcpy(bss_conf->ssid, &ssidie[2], bss_conf->ssid_len);
			if(IEEE80211_MAX_SSID_LEN>bss_conf->ssid_len){
				bss_conf->ssid[bss_conf->ssid_len]=0;
			}
		} else {
			bss_conf->ssid_len  = 0;
		}
		atbm_dev_kfree_skb(skb);
	}
}

static void free_old_beacon(struct rcu_head *rcu_head)
{
	struct beacon_data *old =
			container_of(rcu_head, struct beacon_data, rcu_head);

	atbm_kfree(old);
}

/*
 * This handles both adding a beacon and setting new beacon info
 */
static int ieee80211_config_beacon(struct ieee80211_sub_if_data *sdata,
				   struct beacon_parameters *params)
{
	struct beacon_data *new, *old;
	int new_head_len, new_tail_len;
	int size;
	int err = -EINVAL;

	old = rtnl_dereference(sdata->u.ap.beacon);

	/* head must not be zero-length */
	if (params->head && !params->head_len)
		return -EINVAL;

	/*
	 * This is a kludge. beacon interval should really be part
	 * of the beacon information.
	 */
	if (params->interval &&
	    (sdata->vif.bss_conf.beacon_int != params->interval)) {
		sdata->vif.bss_conf.beacon_int = params->interval;
		ieee80211_bss_info_change_notify(sdata,
						 BSS_CHANGED_BEACON_INT);
	}

	/* Need to have a beacon head if we don't have one yet */
	if (!params->head && !old)
		return err;

	/* sorry, no way to start beaconing without dtim period */
	if (!params->dtim_period && !old)
		return err;

	/* new or old head? */
	if (params->head)
		new_head_len = params->head_len;
	else
		new_head_len = old->head_len;

	/* new or old tail? */
	if (params->tail || !old)
		/* params->tail_len will be zero for !params->tail */
		new_tail_len = params->tail_len;
	else
		new_tail_len = old->tail_len;

	size = sizeof(*new) + new_head_len + new_tail_len;

	new = atbm_kzalloc(size, GFP_KERNEL);
	if (!new)
		return -ENOMEM;

	/* start filling the new info now */

	/* new or old dtim period? */
	if (params->dtim_period)
		new->dtim_period = params->dtim_period;
	else
		new->dtim_period = old->dtim_period;

	/*
	 * pointers go into the block we allocated,
	 * memory is | beacon_data | head | tail |
	 */
	new->head = ((u8 *) new) + sizeof(*new);
	new->tail = new->head + new_head_len;
	new->head_len = new_head_len;
	new->tail_len = new_tail_len;

	/* copy in head */
	if (params->head)
		memcpy(new->head, params->head, new_head_len);
	else
		memcpy(new->head, old->head, new_head_len);

	/* copy in optional tail */
	if (params->tail)
		memcpy(new->tail, params->tail, new_tail_len);
	else
		if (old)
			memcpy(new->tail, old->tail, new_tail_len);


	sdata->vif.bss_conf.dtim_period = new->dtim_period;

	rcu_assign_pointer(sdata->u.ap.beacon, new);

	if (old)
		call_rcu(&old->rcu_head, free_old_beacon);
	
	ieee80211_config_ap_ssid(sdata, params);

	ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_BEACON_ENABLED |
						BSS_CHANGED_BEACON |
						BSS_CHANGED_SSID);
	ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_ARP_FILTER);

#ifdef IPV6_FILTERING
	ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_NDP_FILTER);
#endif /*IPV6_FILTERING*/
	return 0;
}

#ifdef ATBM_PROBE_RESP_EXTRA_IE
static int ieee80211_config_proberesp(struct ieee80211_sub_if_data *sdata,
				   struct beacon_parameters *params)
{
	struct proberesp_data *new, *old;
	int new_head_len, new_tail_len, new_proberesp_data_ies_len;
	int size;
	int err = -EINVAL;
	const u8 *proberesp_ies;
	size_t proberesp_ies_len;
	old = rtnl_dereference(sdata->u.ap.proberesp);

	/* head must not be zero-length */
	if (params->head && !params->head_len)
		return -EINVAL;

	/* Need to have a probe response head if we don't have one yet */
	if (!params->head && !old)
		return err;

	/* new or old head? */
	if (params->head)
		new_head_len = params->head_len;
	else
		new_head_len = old->head_len;
	
	#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 1, 0))
		proberesp_ies = NULL;
		proberesp_ies_len = 0;
	#else
		proberesp_ies = params->proberesp_ies;
		proberesp_ies_len = params->proberesp_ies_len;
	#endif
	if (proberesp_ies || !old)
		new_proberesp_data_ies_len = proberesp_ies_len;
	else
		new_proberesp_data_ies_len = old->proberesp_data_ies_len;

	/* new or old tail? */
	if (params->tail || !old)
		/* params->tail_len will be zero for !params->tail */
		new_tail_len = params->tail_len;
	else
		new_tail_len = old->tail_len;

	size = sizeof(*new) + new_head_len + new_proberesp_data_ies_len + new_tail_len;

	new = atbm_kzalloc(size, GFP_KERNEL);
	if (!new)
		return -ENOMEM;

	new->head = ((u8 *) new) + sizeof(*new);
	new->head_len = new_head_len;
	new->tail = new->head + new->head_len;
	new->tail_len = new_tail_len;

	/* copy in head */
	if (params->head)
		memcpy(new->head, params->head, new_head_len);
	else
		memcpy(new->head, old->head, new_head_len);

	*(__le16 *)new->head = __cpu_to_le16(IEEE80211_FTYPE_MGMT | IEEE80211_STYPE_PROBE_RESP);

	/* copy in optional tail */
	if (params->tail) {
		memcpy(new->tail, params->tail, new_tail_len);
	} else {
		if (old)
			memcpy(new->tail, old->tail, new_tail_len);
	}

	/*
	 * Remove WPS IE and P2P IE from tail, as for probe response these IEs are
	 * provided as part of proberesp_ies.
	 */
#if 0
	if (params->tail) {
		int tail_len;
		volatile u8 *p = new->tail;
		u8 ie_len;

		tail_len = new->tail_len;

		while (tail_len >= 6) {
			ie_len = p[1] + 2;
			tail_len -= ie_len;
			if ((p[0] == ATBM_WLAN_EID_VENDOR_SPECIFIC)
				&& ((p[2] == 0x00 && p[3] == 0x50 && p[4] == 0xf2 && p[5] == 0x04)
				|| (p[2] == 0x50 && p[3] == 0x6f && p[4] == 0x9a && p[5] == 0x09))) {

				memmove((u8*)p, (u8*)p + ie_len, tail_len);
				new->tail_len -= ie_len;
			} else {
				p += ie_len;
			}
		}
	}
	
#endif
	new->proberesp_data_ies = new->tail + new->tail_len;
	new->proberesp_data_ies_len = new_proberesp_data_ies_len;
	

	/* copy in optional probe response ies */
	if (proberesp_ies)
		memcpy(new->proberesp_data_ies, proberesp_ies, new_proberesp_data_ies_len);
	else
		if (old)
			memcpy(new->proberesp_data_ies, old->proberesp_data_ies, new_proberesp_data_ies_len);

	rcu_assign_pointer(sdata->u.ap.proberesp, new);
	
	synchronize_rcu();

	atbm_kfree(old);
	return 0;
}
#endif
#ifdef ATBM_AP_SME
static int ieee80211_config_associate_ie(struct ieee80211_sub_if_data *sdata,
				   struct beacon_parameters *params)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 1, 0))
	struct associate_data *new, *old;
	if((!params->assocresp_ies)||(!params->assocresp_ies_len))
		return 0;
	old = rtnl_dereference(sdata->u.ap.associate_ie);

	new = atbm_kzalloc(sizeof(struct associate_data)+params->assocresp_ies_len, GFP_KERNEL);
	if(!new)
		return -ENOMEM;
	new->associate_ies = ((u8*)new)+sizeof(struct associate_data);
	new->associate_ies_len = params->assocresp_ies_len;

	memcpy(new->associate_ies,params->assocresp_ies,params->assocresp_ies_len);

	rcu_assign_pointer(sdata->u.ap.associate_ie, new);
	
	synchronize_rcu();
	if(old)
		atbm_kfree(old);
#endif
	return 0;
}
#endif

static int ieee80211_add_beacon(struct wiphy *wiphy, struct net_device *dev,
				struct beacon_parameters *params)
{
	struct ieee80211_sub_if_data *sdata;
	struct beacon_data *old;
#ifdef ATBM_PROBE_RESP_EXTRA_IE
	struct proberesp_data *old_proberesp;
	int ret =0;
#endif

	sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	old = rtnl_dereference(sdata->u.ap.beacon);
	if (old)
		return -EALREADY;
#ifdef ATBM_AP_SME
	ieee80211_ap_sme_paras_ap_info(IEEE80211_DEV_TO_SUB_IF(dev),params);
	ieee80211_config_associate_ie(IEEE80211_DEV_TO_SUB_IF(dev),params);
#endif

#ifdef ATBM_PROBE_RESP_EXTRA_IE
	old_proberesp = rtnl_dereference(sdata->u.ap.proberesp);
	if (old_proberesp)
		return -EALREADY;
	ret = ieee80211_config_proberesp(sdata, params);
	if (!ret)
		ret = ieee80211_config_beacon(sdata, params);
	return ret;
#else

	return ieee80211_config_beacon(sdata, params);
#endif
}

static int ieee80211_set_beacon(struct wiphy *wiphy, struct net_device *dev,
				struct beacon_parameters *params)
{
	struct ieee80211_sub_if_data *sdata;
	struct beacon_data *old;
#ifdef ATBM_PROBE_RESP_EXTRA_IE
	struct proberesp_data *old_proberesp;
	int ret =0;
#endif

	sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	old = rtnl_dereference(sdata->u.ap.beacon);
	if (!old)
		return -ENOENT;
#ifdef ATBM_AP_SME
	ieee80211_config_associate_ie(IEEE80211_DEV_TO_SUB_IF(dev),params);
#endif

#ifdef ATBM_PROBE_RESP_EXTRA_IE
	old_proberesp = rtnl_dereference(sdata->u.ap.proberesp);
	if (!old_proberesp)
		return -ENOENT;
	ret = ieee80211_config_proberesp(sdata, params);
	if (!ret)
		ret = ieee80211_config_beacon(sdata, params);
	return ret;
#else

	return ieee80211_config_beacon(sdata, params);
#endif
}
/******************************************************
struct cfg80211_beacon_data {
	const u8 *head, *tail;
	const u8 *beacon_ies;
	const u8 *proberesp_ies;
	const u8 *assocresp_ies;
	const u8 *probe_resp;

	size_t head_len, tail_len;
	size_t beacon_ies_len;
	size_t proberesp_ies_len;
	size_t assocresp_ies_len;
	size_t probe_resp_len;
};
struct cfg80211_ap_settings {
	struct cfg80211_chan_def chandef;

	struct cfg80211_beacon_data beacon;

	int beacon_interval, dtim_period;
	const u8 *ssid;
	size_t ssid_len;
	enum nl80211_hidden_ssid hidden_ssid;
	struct cfg80211_crypto_settings crypto;
	bool privacy;
	enum nl80211_auth_type auth_type;
	enum nl80211_smps_mode smps_mode;
	int inactivity_timeout;
	u8 p2p_ctwindow;
	bool p2p_opp_ps;
	const struct cfg80211_acl_data *acl;
};
*********************************************************
*/
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0))
#define BEACON_PARAMS_CPY(params,beacon)		\
		do											\
		{											\
			params.head = (beacon)->head;				\
			params.head_len =  (beacon)->head_len;		\
			params.tail =  (beacon)->tail;					\
			params.tail_len =  (beacon)->tail_len;			\
			params.beacon_ies = (beacon)->beacon_ies;		\
			params.beacon_ies_len= (beacon)->beacon_ies_len;	\
			params.proberesp_ies = (beacon)->proberesp_ies;		\
			params.proberesp_ies_len =  (beacon)->proberesp_ies_len;	\
			params.assocresp_ies =  (beacon)->assocresp_ies;		\
			params.assocresp_ies_len =  (beacon)->assocresp_ies_len;		\
		}while(0)
static int ieee80211_start_ap(struct wiphy *wiphy, struct net_device *dev,
							struct cfg80211_ap_settings *settings)
{
	struct beacon_parameters params;
	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0))
	enum nl80211_channel_type chtype;
	struct ieee80211_channel *chan = NULL;
	#endif
	
	memset(&params,0,sizeof(struct beacon_parameters));
	BEACON_PARAMS_CPY(params,&(settings->beacon));
	params.interval = settings->beacon_interval;
	params.dtim_period = settings->dtim_period;
	params.ssid = settings->ssid;
	params.ssid_len = settings->ssid_len;
	params.hidden_ssid = settings->hidden_ssid;
	params.privacy = settings->privacy;
	params.auth_type = settings->auth_type;
	memcpy(&params.crypto,&settings->crypto,sizeof(struct cfg80211_crypto_settings));
	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0))
	#if (LINUX_VERSION_CODE <= KERNEL_VERSION(3, 7, 2))
	chan = settings->channel;
	chtype = settings->channel_type;
	#else
	chtype = cfg80211_get_chandef_type(&settings->chandef);
	chan = settings->chandef.chan;
	#endif
	if(ieee80211_set_channel(wiphy,dev,chan,chtype))
	{
		atbm_printk_err("set channel err\n");
		return -EBUSY;
	}
	#endif
	return ieee80211_add_beacon(wiphy,dev,&params);	
}
static  int ieee80211_change_beacon(struct wiphy *wiphy, struct net_device *dev,
									   struct cfg80211_beacon_data *info)
{
	struct beacon_parameters params;
	memset(&params,0,sizeof(struct beacon_parameters));
	BEACON_PARAMS_CPY(params,info);

	return ieee80211_set_beacon(wiphy, dev,&params);
}

#endif
static int ieee80211_del_beacon(struct wiphy *wiphy, struct net_device *dev)
{
	struct ieee80211_sub_if_data *sdata;
	struct beacon_data *old;
	struct beacon_extra *old_extra;
#ifdef ATBM_PROBE_RESP_EXTRA_IE
	struct proberesp_data *old_proberesp;
#endif
#ifdef ATBM_AP_SME
	struct associate_data *old_assoc;
#endif

	sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	old = rtnl_dereference(sdata->u.ap.beacon);
	if (!old)
		return -ENOENT;

	RCU_INIT_POINTER(sdata->u.ap.beacon, NULL);
	synchronize_rcu();
	atbm_kfree(old);
#ifdef ATBM_PROBE_RESP_EXTRA_IE
	old_proberesp = rtnl_dereference(sdata->u.ap.proberesp);
	if (!old_proberesp)
		return -ENOENT;

	rcu_assign_pointer(sdata->u.ap.proberesp, NULL);
	synchronize_rcu();
	atbm_kfree(old_proberesp);
#endif

#ifdef ATBM_AP_SME
	old_assoc = rtnl_dereference(sdata->u.ap.associate_ie);
	rcu_assign_pointer(sdata->u.ap.associate_ie, NULL);
	synchronize_rcu();
	if(old_assoc)
		atbm_kfree(old_assoc);
#endif
	old_extra = rtnl_dereference(sdata->u.ap.beacon_extra);
	rcu_assign_pointer(sdata->u.ap.beacon_extra, NULL);
	synchronize_rcu();
	if(old_extra)
		atbm_kfree(old_extra);
#ifdef CONFIG_ATBM_RADAR_DETECT
#ifdef CONFIG_ATBM_5G_PRETEND_2G
	ieee80211_dfs_cac_abort(sdata);
#endif
#endif
	ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_BEACON_ENABLED);
	return 0;
}

/* Layer 2 Update frame (802.2 Type 1 LLC XID Update response) */
struct iapp_layer2_update {
	u8 da[ETH_ALEN];	/* broadcast */
	u8 sa[ETH_ALEN];	/* STA addr */
	__be16 len;		/* 6 */
	u8 dsap;		/* 0 */
	u8 ssap;		/* 0 */
	u8 control;
	u8 xid_info[3];
} __packed;

static void ieee80211_send_layer2_update(struct sta_info *sta)
{
	struct iapp_layer2_update *msg;
	struct sk_buff *skb;

	/* Send Level 2 Update Frame to update forwarding tables in layer 2
	 * bridge devices */

	skb = atbm_dev_alloc_skb(sizeof(*msg));
	if (!skb)
		return;
	msg = (struct iapp_layer2_update *)atbm_skb_put(skb, sizeof(*msg));

	/* 802.2 Type 1 Logical Link Control (LLC) Exchange Identifier (XID)
	 * Update response frame; IEEE Std 802.2-1998, 5.4.1.2.1 */

	memset(msg->da, 0xff, ETH_ALEN);
	memcpy(msg->sa, sta->sta.addr, ETH_ALEN);
	msg->len = htons(6);
	msg->dsap = 0;
	msg->ssap = 0x01;	/* NULL LSAP, CR Bit: Response */
	msg->control = 0xaf;	/* XID response lsb.1111F101.
				 * F=0 (no poll command; unsolicited frame) */
	msg->xid_info[0] = 0x81;	/* XID format identifier */
	msg->xid_info[1] = 1;	/* LLC types/classes: Type 1 LLC */
	msg->xid_info[2] = 0;	/* XID sender's receive window size (RW) */

	skb->dev = sta->sdata->dev;
	skb->protocol = eth_type_trans(skb, sta->sdata->dev);
	memset(skb->cb, 0, sizeof(skb->cb));
	atbm_netif_rx_ni(skb);
}

static void sta_apply_parameters(struct ieee80211_local *local,
				 struct sta_info *sta,
				 struct station_parameters *params)
{
	u32 rates;
	int i, j;
	struct ieee80211_supported_band *sband;
	struct ieee80211_sub_if_data *sdata = sta->sdata;
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(local, sdata);
	u32 mask, set;

	sband = local->hw.wiphy->bands[chan_state->oper_channel->band];

	mask = params->sta_flags_mask;
	set = params->sta_flags_set;

	if (mask & BIT(NL80211_STA_FLAG_AUTHORIZED)) {
		if (set & BIT(NL80211_STA_FLAG_AUTHORIZED))
			set_sta_flag(sta, WLAN_STA_AUTHORIZED);
		else
			clear_sta_flag(sta, WLAN_STA_AUTHORIZED);
	}

	if (mask & BIT(NL80211_STA_FLAG_SHORT_PREAMBLE)) {
		if (set & BIT(NL80211_STA_FLAG_SHORT_PREAMBLE))
			set_sta_flag(sta, WLAN_STA_SHORT_PREAMBLE);
		else
			clear_sta_flag(sta, WLAN_STA_SHORT_PREAMBLE);
	}
/*
	if (mask & BIT(NL80211_STA_FLAG_WME)) {
		if (set & BIT(NL80211_STA_FLAG_WME)) {
			set_sta_flag(sta, WLAN_STA_WME);
			sta->sta.wme = true;
		} else {
			clear_sta_flag(sta, WLAN_STA_WME);
			sta->sta.wme = false;
		}
	}
*/
	if (mask & BIT(NL80211_STA_FLAG_MFP)) {
		if (set & BIT(NL80211_STA_FLAG_MFP))
			set_sta_flag(sta, WLAN_STA_MFP);
		else
			clear_sta_flag(sta, WLAN_STA_MFP);
	}

	if (mask & BIT(NL80211_STA_FLAG_AUTHENTICATED)) {
		if (set & BIT(NL80211_STA_FLAG_AUTHENTICATED))
			set_sta_flag(sta, WLAN_STA_AUTH);
		else
			clear_sta_flag(sta, WLAN_STA_AUTH);
	}
	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 1, 0))
	if (mask & BIT(NL80211_STA_FLAG_TDLS_PEER)) {
		if (set & BIT(NL80211_STA_FLAG_TDLS_PEER))
			set_sta_flag(sta, WLAN_STA_TDLS_PEER);
		else
			clear_sta_flag(sta, WLAN_STA_TDLS_PEER);
	}
	
	if (params->sta_modify_mask & STATION_PARAM_APPLY_UAPSD) {
		sta->sta.uapsd_queues = params->uapsd_queues;
		sta->sta.max_sp = params->max_sp;
	}
	#endif
	/*
	 * cfg80211 validates this (1-2007) and allows setting the AID
	 * only when creating a new station entry
	 */
	if (params->aid)
		sta->sta.aid = params->aid;

	/*
	 * FIXME: updating the following information is racy when this
	 *	  function is called from ieee80211_change_station().
	 *	  However, all this information should be static so
	 *	  maybe we should just reject attemps to change it.
	 */

	if (params->listen_interval >= 0)
		sta->listen_interval = params->listen_interval;

	if (params->supported_rates) {
		rates = 0;

		for (i = 0; i < params->supported_rates_len; i++) {
			int rate = (params->supported_rates[i] & 0x7f) * 5;
			for (j = 0; j < sband->n_bitrates; j++) {
				if (sband->bitrates[j].bitrate == rate)
					rates |= BIT(j);
			}
		}
		sta->sta.supp_rates[chan_state->oper_channel->band] = rates;
	}

	if (params->ht_capa){
		ieee80211_ht_cap_ie_to_sta_ht_cap(sband,
						  params->ht_capa,
						  &sta->sta.ht_cap);
		ieee80211_ht_cap_to_sta_channel_type(sta);
	}
	if(sta->sta.ht_cap.ht_supported){
		set_sta_flag(sta, WLAN_STA_WME);
		sta->sta.wme = true;
	}
	
#ifdef IPC_AP_USED_11G_NO_RTS
	sta->sta.ht_cap.ht_supported = false;
#endif
#ifdef CONFIG_MAC80211_ATBM_MESH
	if (ieee80211_vif_is_mesh(&sdata->vif)) {
		if (sdata->u.mesh.security & IEEE80211_MESH_SEC_SECURED)
			switch (params->plink_state) {
			case NL80211_PLINK_LISTEN:
			case NL80211_PLINK_ESTAB:
			case NL80211_PLINK_BLOCKED:
				sta->plink_state = params->plink_state;
				break;
			default:
				/*  nothing  */
				break;
			}
		else
			switch (params->plink_action) {
			case PLINK_ACTION_OPEN:
				mesh_plink_open(sta);
				break;
			case PLINK_ACTION_BLOCK:
				mesh_plink_block(sta);
				break;
			}
	}
#endif
}

static int ieee80211_add_station(struct wiphy *wiphy, struct net_device *dev,
				#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 16, 0))
				u8 *mac,
				#else
				const u8 *mac,
				#endif 
				struct station_parameters *params)
{
#ifndef ATBM_AP_SME
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct sta_info *sta;
	struct ieee80211_sub_if_data *sdata;
	int err;
	int layer2_update;

	if (params->vlan) {
		sdata = IEEE80211_DEV_TO_SUB_IF(params->vlan);

		if (sdata->vif.type != NL80211_IFTYPE_AP_VLAN &&
		    sdata->vif.type != NL80211_IFTYPE_AP)
			return -EINVAL;
	} else
		sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	if (atbm_compare_ether_addr(mac, sdata->vif.addr) == 0)
		return -EINVAL;

	if (is_multicast_ether_addr(mac))
		return -EINVAL;
	
	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 1, 0))
	/* Only TDLS-supporting stations can add TDLS peers */
	if ((params->sta_flags_set & BIT(NL80211_STA_FLAG_TDLS_PEER)) &&
	    !((wiphy->flags & WIPHY_FLAG_SUPPORTS_TDLS) &&
	      sdata->vif.type == NL80211_IFTYPE_STATION))
		return -ENOTSUPP;
	#endif
	
	sta = sta_info_alloc(sdata, (u8 *)mac, GFP_KERNEL);
	if (!sta)
		return -ENOMEM;

	set_sta_flag(sta, WLAN_STA_AUTH);
	set_sta_flag(sta, WLAN_STA_ASSOC);

	sta_apply_parameters(local, sta, params);
#ifndef CONFIG_RATE_HW_CONTROL
	rate_control_rate_init(sta);
#endif
	layer2_update = sdata->vif.type == NL80211_IFTYPE_AP_VLAN ||
		sdata->vif.type == NL80211_IFTYPE_AP;

	err = sta_info_insert_rcu(sta);
	if (err) {
		rcu_read_unlock();
		return err;
	}

	if (layer2_update)
		ieee80211_send_layer2_update(sta);

	rcu_read_unlock();
#endif
	return 0;
}

static int ieee80211_del_station(struct wiphy *wiphy, struct net_device *dev,
				#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 31))
				u8 *mac
				#elif (LINUX_VERSION_CODE < KERNEL_VERSION(3, 19, 0))
				const u8 *mac
				#else
				struct station_del_parameters *params
				#endif
				)
{
#ifndef ATBM_AP_SME
	struct ieee80211_local *local = wiphy_priv(wiphy);
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 19, 0))
	const u8 *mac = params->mac;
#endif
	struct ieee80211_sub_if_data *sdata;

	sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	#ifndef ATBM_AP_SME
	if (mac)
		return sta_info_destroy_addr_bss(sdata, (u8*)mac);

	sta_info_flush(local, sdata);
	#else
	if(mac){
		struct sta_info * sta = NULL;
		bool deauthen_sta =false;
		bool free_sta = false;
		rcu_read_lock();
		sta = sta_info_get_rx(sdata, mac);
		if(sta)
		{
			atbm_printk_cfg("%s:mac(%pM),AUTH(%d),DEAUTHENNING(%d)\n",__func__,mac,test_sta_flag(sta, WLAN_STA_AUTH),
				test_sta_flag(sta, WLAN_STA_DEAUTHENNING));
			if(test_sta_flag(sta, WLAN_STA_AUTH)){
				if(!test_sta_flag(sta, WLAN_STA_DEAUTHENNING)){
					set_sta_flag(sta,WLAN_STA_DEAUTHENNING);
					deauthen_sta = true;
				}
			}
			else
				free_sta = true;
		}
		rcu_read_unlock();

		if(deauthen_sta == true){
			ieee80211_ap_sme_queue_event(sdata,STA_EVENT__SEND_DEAUTHEN,mac);
			ieee80211_ap_sme_sta_sync_lock(sdata);
		}

		if(free_sta == true)
			ieee80211_ap_sme_queue_event(sdata,STA_EVENT__FREE,mac);
	}else{
		atbm_printk_cfg("%s:deauthen all sta\n",__func__);
	}
	#endif
	return 0;
}

static int ieee80211_change_station(struct wiphy *wiphy,
				    struct net_device *dev,
				    #if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 31))
					u8 *mac,
					#else
					const u8 *mac,
					#endif
				    struct station_parameters *params)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct sta_info *sta;
	struct ieee80211_sub_if_data *vlansdata;

	rcu_read_lock();

	sta = sta_info_get_bss(sdata, (u8*)mac);
	if (!sta) {
		rcu_read_unlock();
		return -ENOENT;
	}
#ifdef ATBM_SURPORT_TDLS
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 1, 0))
	/* The TDLS bit cannot be toggled after the STA was added */
	if ((params->sta_flags_mask & BIT(NL80211_STA_FLAG_TDLS_PEER)) &&
	    !!(params->sta_flags_set & BIT(NL80211_STA_FLAG_TDLS_PEER)) !=
	    !!test_sta_flag(sta, WLAN_STA_TDLS_PEER)) {
		rcu_read_unlock();
		return -EINVAL;
	}
#endif
#endif
	if (params->vlan && params->vlan != sta->sdata->dev) {
		vlansdata = IEEE80211_DEV_TO_SUB_IF(params->vlan);

		if (vlansdata->vif.type != NL80211_IFTYPE_AP_VLAN &&
		    vlansdata->vif.type != NL80211_IFTYPE_AP) {
			rcu_read_unlock();
			return -EINVAL;
		}

		if (params->vlan->ieee80211_ptr->use_4addr) {
			if (vlansdata->u.vlan.sta) {
				rcu_read_unlock();
				return -EBUSY;
			}

			rcu_assign_pointer(vlansdata->u.vlan.sta, sta);
		}

		sta->sdata = vlansdata;
		ieee80211_send_layer2_update(sta);
	}

	sta_apply_parameters(local, sta, params);

	rcu_read_unlock();

	if (sdata->vif.type == NL80211_IFTYPE_STATION &&
	    params->sta_flags_mask & BIT(NL80211_STA_FLAG_AUTHORIZED))
		ieee80211_recalc_ps(local, -1);

	return 0;
}

#ifdef CONFIG_MAC80211_ATBM_MESH
static int ieee80211_add_mpath(struct wiphy *wiphy, struct net_device *dev,
				 u8 *dst, u8 *next_hop)
{
	struct ieee80211_sub_if_data *sdata;
	struct mesh_path *mpath;
	struct sta_info *sta;
	int err;

	sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	rcu_read_lock();
	sta = sta_info_get(sdata, next_hop);
	if (!sta) {
		rcu_read_unlock();
		return -ENOENT;
	}

	err = mesh_path_add(dst, sdata);
	if (err) {
		rcu_read_unlock();
		return err;
	}

	mpath = mesh_path_lookup(dst, sdata);
	if (!mpath) {
		rcu_read_unlock();
		return -ENXIO;
	}
	mesh_path_fix_nexthop(mpath, sta);

	rcu_read_unlock();
	return 0;
}

static int ieee80211_del_mpath(struct wiphy *wiphy, struct net_device *dev,
				 u8 *dst)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	if (dst)
		return mesh_path_del(dst, sdata);

	mesh_path_flush_by_iface(sdata);
	return 0;
}

static int ieee80211_change_mpath(struct wiphy *wiphy,
				    struct net_device *dev,
				    u8 *dst, u8 *next_hop)
{
	struct ieee80211_sub_if_data *sdata;
	struct mesh_path *mpath;
	struct sta_info *sta;

	sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	rcu_read_lock();

	sta = sta_info_get(sdata, next_hop);
	if (!sta) {
		rcu_read_unlock();
		return -ENOENT;
	}

	mpath = mesh_path_lookup(dst, sdata);
	if (!mpath) {
		rcu_read_unlock();
		return -ENOENT;
	}

	mesh_path_fix_nexthop(mpath, sta);

	rcu_read_unlock();
	return 0;
}

static void mpath_set_pinfo(struct mesh_path *mpath, u8 *next_hop,
			    struct mpath_info *pinfo)
{
	struct sta_info *next_hop_sta = rcu_dereference(mpath->next_hop);

	if (next_hop_sta)
		memcpy(next_hop, next_hop_sta->sta.addr, ETH_ALEN);
	else
		memset(next_hop, 0, ETH_ALEN);

	pinfo->generation = mesh_paths_generation;

	pinfo->filled = MPATH_INFO_FRAME_QLEN |
			MPATH_INFO_SN |
			MPATH_INFO_METRIC |
			MPATH_INFO_EXPTIME |
			MPATH_INFO_DISCOVERY_TIMEOUT |
			MPATH_INFO_DISCOVERY_RETRIES |
			MPATH_INFO_FLAGS;

	pinfo->frame_qlen = mpath->frame_queue.qlen;
	pinfo->sn = mpath->sn;
	pinfo->metric = mpath->metric;
	if (time_before(jiffies, mpath->exp_time))
		pinfo->exptime = jiffies_to_msecs(mpath->exp_time - jiffies);
	pinfo->discovery_timeout =
			jiffies_to_msecs(mpath->discovery_timeout);
	pinfo->discovery_retries = mpath->discovery_retries;
	pinfo->flags = 0;
	if (mpath->flags & MESH_PATH_ACTIVE)
		pinfo->flags |= NL80211_MPATH_FLAG_ACTIVE;
	if (mpath->flags & MESH_PATH_RESOLVING)
		pinfo->flags |= NL80211_MPATH_FLAG_RESOLVING;
	if (mpath->flags & MESH_PATH_SN_VALID)
		pinfo->flags |= NL80211_MPATH_FLAG_SN_VALID;
	if (mpath->flags & MESH_PATH_FIXED)
		pinfo->flags |= NL80211_MPATH_FLAG_FIXED;
	if (mpath->flags & MESH_PATH_RESOLVING)
		pinfo->flags |= NL80211_MPATH_FLAG_RESOLVING;

	pinfo->flags = mpath->flags;
}

static int ieee80211_get_mpath(struct wiphy *wiphy, struct net_device *dev,
			       u8 *dst, u8 *next_hop, struct mpath_info *pinfo)

{
	struct ieee80211_sub_if_data *sdata;
	struct mesh_path *mpath;

	sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	rcu_read_lock();
	mpath = mesh_path_lookup(dst, sdata);
	if (!mpath) {
		rcu_read_unlock();
		return -ENOENT;
	}
	memcpy(dst, mpath->dst, ETH_ALEN);
	mpath_set_pinfo(mpath, next_hop, pinfo);
	rcu_read_unlock();
	return 0;
}

static int ieee80211_dump_mpath(struct wiphy *wiphy, struct net_device *dev,
				 int idx, u8 *dst, u8 *next_hop,
				 struct mpath_info *pinfo)
{
	struct ieee80211_sub_if_data *sdata;
	struct mesh_path *mpath;

	sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	rcu_read_lock();
	mpath = mesh_path_lookup_by_idx(idx, sdata);
	if (!mpath) {
		rcu_read_unlock();
		return -ENOENT;
	}
	memcpy(dst, mpath->dst, ETH_ALEN);
	mpath_set_pinfo(mpath, next_hop, pinfo);
	rcu_read_unlock();
	return 0;
}

static int ieee80211_get_mesh_config(struct wiphy *wiphy,
				struct net_device *dev,
				struct mesh_config *conf)
{
	struct ieee80211_sub_if_data *sdata;
	sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	memcpy(conf, &(sdata->u.mesh.mshcfg), sizeof(struct mesh_config));
	return 0;
}

static inline bool _chg_mesh_attr(enum nl80211_meshconf_params parm, u32 mask)
{
	return (mask >> (parm-1)) & 0x1;
}

static int copy_mesh_setup(struct ieee80211_if_mesh *ifmsh,
		const struct mesh_setup *setup)
{
	u8 *new_ie;
	const u8 *old_ie;

	/* allocate information elements */
	new_ie = NULL;
	old_ie = ifmsh->ie;

	if (setup->ie_len) {
		new_ie = kmemdup(setup->ie, setup->ie_len,
				GFP_KERNEL);
		if (!new_ie)
			return -ENOMEM;
	}
	ifmsh->ie_len = setup->ie_len;
	ifmsh->ie = new_ie;
	atbm_kfree(old_ie);

	/* now copy the rest of the setup parameters */
	ifmsh->mesh_id_len = setup->mesh_id_len;
	memcpy(ifmsh->mesh_id, setup->mesh_id, ifmsh->mesh_id_len);
	ifmsh->mesh_pp_id = setup->path_sel_proto;
	ifmsh->mesh_pm_id = setup->path_metric;
	ifmsh->security = IEEE80211_MESH_SEC_NONE;
	if (setup->is_authenticated)
		ifmsh->security |= IEEE80211_MESH_SEC_AUTHED;
	if (setup->is_secure)
		ifmsh->security |= IEEE80211_MESH_SEC_SECURED;

	return 0;
}

static int ieee80211_update_mesh_config(struct wiphy *wiphy,
					struct net_device *dev, u32 mask,
					const struct mesh_config *nconf)
{
	struct mesh_config *conf;
	struct ieee80211_sub_if_data *sdata;
	struct ieee80211_if_mesh *ifmsh;

	sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	ifmsh = &sdata->u.mesh;

	/* Set the config options which we are interested in setting */
	conf = &(sdata->u.mesh.mshcfg);
	if (_chg_mesh_attr(NL80211_MESHCONF_RETRY_TIMEOUT, mask))
		conf->dot11MeshRetryTimeout = nconf->dot11MeshRetryTimeout;
	if (_chg_mesh_attr(NL80211_MESHCONF_CONFIRM_TIMEOUT, mask))
		conf->dot11MeshConfirmTimeout = nconf->dot11MeshConfirmTimeout;
	if (_chg_mesh_attr(NL80211_MESHCONF_HOLDING_TIMEOUT, mask))
		conf->dot11MeshHoldingTimeout = nconf->dot11MeshHoldingTimeout;
	if (_chg_mesh_attr(NL80211_MESHCONF_MAX_PEER_LINKS, mask))
		conf->dot11MeshMaxPeerLinks = nconf->dot11MeshMaxPeerLinks;
	if (_chg_mesh_attr(NL80211_MESHCONF_MAX_RETRIES, mask))
		conf->dot11MeshMaxRetries = nconf->dot11MeshMaxRetries;
	if (_chg_mesh_attr(NL80211_MESHCONF_TTL, mask))
		conf->dot11MeshTTL = nconf->dot11MeshTTL;
	if (_chg_mesh_attr(NL80211_MESHCONF_ELEMENT_TTL, mask))
		conf->dot11MeshTTL = nconf->element_ttl;
	if (_chg_mesh_attr(NL80211_MESHCONF_AUTO_OPEN_PLINKS, mask))
		conf->auto_open_plinks = nconf->auto_open_plinks;
	if (_chg_mesh_attr(NL80211_MESHCONF_HWMP_MAX_PREQ_RETRIES, mask))
		conf->dot11MeshHWMPmaxPREQretries =
			nconf->dot11MeshHWMPmaxPREQretries;
	if (_chg_mesh_attr(NL80211_MESHCONF_PATH_REFRESH_TIME, mask))
		conf->path_refresh_time = nconf->path_refresh_time;
	if (_chg_mesh_attr(NL80211_MESHCONF_MIN_DISCOVERY_TIMEOUT, mask))
		conf->min_discovery_timeout = nconf->min_discovery_timeout;
	if (_chg_mesh_attr(NL80211_MESHCONF_HWMP_ACTIVE_PATH_TIMEOUT, mask))
		conf->dot11MeshHWMPactivePathTimeout =
			nconf->dot11MeshHWMPactivePathTimeout;
	if (_chg_mesh_attr(NL80211_MESHCONF_HWMP_PREQ_MIN_INTERVAL, mask))
		conf->dot11MeshHWMPpreqMinInterval =
			nconf->dot11MeshHWMPpreqMinInterval;
	if (_chg_mesh_attr(NL80211_MESHCONF_HWMP_NET_DIAM_TRVS_TIME,
			   mask))
		conf->dot11MeshHWMPnetDiameterTraversalTime =
			nconf->dot11MeshHWMPnetDiameterTraversalTime;
	if (_chg_mesh_attr(NL80211_MESHCONF_HWMP_ROOTMODE, mask)) {
		conf->dot11MeshHWMPRootMode = nconf->dot11MeshHWMPRootMode;
		ieee80211_mesh_root_setup(ifmsh);
	}
	if (_chg_mesh_attr(NL80211_MESHCONF_GATE_ANNOUNCEMENTS, mask)) {
		/* our current gate announcement implementation rides on root
		 * announcements, so require this ifmsh to also be a root node
		 * */
		if (nconf->dot11MeshGateAnnouncementProtocol &&
		    !conf->dot11MeshHWMPRootMode) {
			conf->dot11MeshHWMPRootMode = 1;
			ieee80211_mesh_root_setup(ifmsh);
		}
		conf->dot11MeshGateAnnouncementProtocol =
			nconf->dot11MeshGateAnnouncementProtocol;
	}
	if (_chg_mesh_attr(NL80211_MESHCONF_HWMP_RANN_INTERVAL, mask)) {
		conf->dot11MeshHWMPRannInterval =
			nconf->dot11MeshHWMPRannInterval;
	}
	return 0;
}

static int ieee80211_join_mesh(struct wiphy *wiphy, struct net_device *dev,
			       const struct mesh_config *conf,
			       const struct mesh_setup *setup)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_if_mesh *ifmsh = &sdata->u.mesh;
	int err;

	memcpy(&ifmsh->mshcfg, conf, sizeof(struct mesh_config));
	err = copy_mesh_setup(ifmsh, setup);
	if (err)
		return err;
	ieee80211_start_mesh(sdata);

	return 0;
}

static int ieee80211_leave_mesh(struct wiphy *wiphy, struct net_device *dev)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	ieee80211_stop_mesh(sdata);

	return 0;
}
#endif
static int ieee80211_change_bss(struct wiphy *wiphy,
				struct net_device *dev,
				struct bss_parameters *params)
{
	struct ieee80211_sub_if_data *sdata;
	struct ieee80211_channel_state *chan_state;
	u32 changed = 0;

	sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	chan_state = ieee80211_get_channel_state(sdata->local, sdata);
	if (params->use_cts_prot >= 0) {
		sdata->vif.bss_conf.use_cts_prot = params->use_cts_prot;
		changed |= BSS_CHANGED_ERP_CTS_PROT;
	}
	if (params->use_short_preamble >= 0) {		
		sdata->vif.bss_conf.use_short_preamble =
			params->use_short_preamble;
		changed |= BSS_CHANGED_ERP_PREAMBLE;
	}
	if (!sdata->vif.bss_conf.use_short_slot &&
	    chan_state->conf.channel->band == IEEE80211_BAND_5GHZ) {
		sdata->vif.bss_conf.use_short_slot = true;
		changed |= BSS_CHANGED_ERP_SLOT;
	}

	if (params->use_short_slot_time >= 0) {
		sdata->vif.bss_conf.use_short_slot =
			params->use_short_slot_time;
		changed |= BSS_CHANGED_ERP_SLOT;
	}

	if (params->basic_rates) {
		int i, j;
		u32 rates = 0;
		struct ieee80211_supported_band *sband =
			wiphy->bands[chan_state->oper_channel->band];

		for (i = 0; i < params->basic_rates_len; i++) {
			int rate = (params->basic_rates[i] & 0x7f) * 5;
			for (j = 0; j < sband->n_bitrates; j++) {
				if (sband->bitrates[j].bitrate == rate)
					rates |= BIT(j);
			}
		}
		sdata->vif.bss_conf.basic_rates = rates;
		changed |= BSS_CHANGED_BASIC_RATES;
	}

	if (params->ap_isolate >= 0) {
		if (params->ap_isolate)
			sdata->flags |= IEEE80211_SDATA_DONT_BRIDGE_PACKETS;
		else
			sdata->flags &= ~IEEE80211_SDATA_DONT_BRIDGE_PACKETS;
	}

	if (params->ht_opmode >= 0) {
		sdata->vif.bss_conf.ht_operation_mode =
			(u16) params->ht_opmode;
		changed |= BSS_CHANGED_HT;
	}

	ieee80211_bss_info_change_notify(sdata, changed);

	return 0;
}

static int ieee80211_set_txq_params(struct wiphy *wiphy,
	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 1, 0))
				    struct net_device *dev,
	#endif
				    struct ieee80211_txq_params *params)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 1, 0))
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	#else
	struct ieee80211_sub_if_data *sdata = NULL;
	#endif
	struct ieee80211_tx_queue_params p;

	atbm_printk_cfg("ieee80211_set_txq_params\n");
	if (!local->ops->conf_tx)
		return -EOPNOTSUPP;

	if (local->hw.queues < IEEE80211_NUM_ACS)
		return -EOPNOTSUPP;
	
	#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 1, 0))
	list_for_each_entry(sdata, &local->interfaces, list)
		 if ((sdata->vif.type == NL80211_IFTYPE_AP)||(sdata->vif.type == NL80211_IFTYPE_P2P_GO))
		 	break;
	
	if(sdata == NULL)
	{
		atbm_printk_err("txq_params:sdata == NULL\n");
		return -EOPNOTSUPP;
	}
	#endif
	
	memset(&p, 0, sizeof(p));
	p.aifs = params->aifs;
	p.cw_max = params->cwmax;
	p.cw_min = params->cwmin;
	p.txop = params->txop;

	/*
	 * Setting tx queue params disables u-apsd because it's only
	 * called in master mode.
	 */
	p.uapsd = false;
	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0))
	sdata->tx_conf[params->ac] = p;
	if (drv_conf_tx(local, sdata, params->ac, &p)) {
		return -EINVAL;
	}
	#else
	sdata->tx_conf[params->queue] = p;
	if (drv_conf_tx(local, sdata, params->queue, &p)) {
		return -EINVAL;
	}

	#endif
	return 0;
}

static int __ieee80211_set_channel(struct wiphy *wiphy,
				 struct net_device *netdev,
				 struct ieee80211_channel *chan,
				 enum nl80211_channel_type channel_type)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct ieee80211_channel_state *chan_state;
	struct ieee80211_sub_if_data *sdata = NULL;
	struct ieee80211_channel *old_oper;
	enum nl80211_channel_type old_oper_type;
	enum nl80211_channel_type old_vif_oper_type= NL80211_CHAN_NO_HT;
	u32 changed = 0;
	if(netdev) {
		sdata = IEEE80211_DEV_TO_SUB_IF(netdev);
	}
	else {
		atbm_printk_cfg("%s:set monitor interface channel\n",__func__);
		list_for_each_entry(sdata, &local->interfaces, list){
			if ((sdata->vif.type == NL80211_IFTYPE_MONITOR) && 
			   !(sdata->u.mntr_flags & MONITOR_FLAG_COOK_FRAMES)){
				changed = IEEE80211_CONF_CHANGE_MONITOR;
				break;
			}
		}
	}

	if(sdata == NULL)
	{
		return -EOPNOTSUPP;
	}
	if((sdata->vif.type == NL80211_IFTYPE_MONITOR) &&
	  !(sdata->u.mntr_flags & MONITOR_FLAG_COOK_FRAMES)){
		changed = IEEE80211_CONF_CHANGE_MONITOR;
	}
	chan_state = ieee80211_get_channel_state(local, sdata); 

	switch (ieee80211_get_channel_mode(local, sdata)) {
	case CHAN_MODE_HOPPING:
		return -EBUSY;
	case CHAN_MODE_FIXED:
		if (!sdata && chan_state->_oper_channel_type == channel_type)
			return 0;
		if(chan_state->oper_channel != chan)
			goto __channel_busy;
		if(chan_state->_oper_channel_type != channel_type){
			char request_type = ieee80211_chw_is_ht40(channel_type);
			char oper_type = ieee80211_chw_is_ht40(chan_state->_oper_channel_type);
			if((request_type == oper_type)&&(request_type == 1))
				goto __channel_busy;
		}
		break;
	case CHAN_MODE_UNDEFINED:
		break;
	}

	if (sdata)
		old_vif_oper_type = sdata->vif.bss_conf.channel_type;
	old_oper_type = chan_state->_oper_channel_type;

	if (!ieee80211_set_channel_type(local, sdata, channel_type))
		goto __channel_busy;

	old_oper = chan_state->oper_channel;
	chan_state->oper_channel = chan;
	atbm_printk_cfg("%s:old_oper(%p),oper_channel(%p),old_oper_type %d\n",__func__,old_oper,chan_state->oper_channel,old_oper_type);
	/* Update driver if changes were actually made. */
	if ((old_oper != chan_state->oper_channel) ||
	    (old_oper_type != chan_state->_oper_channel_type) ||(changed&IEEE80211_CONF_CHANGE_MONITOR))
		ieee80211_hw_config(local, changed|IEEE80211_CONF_CHANGE_CHANNEL);

	if ((sdata && sdata->vif.type != NL80211_IFTYPE_MONITOR) &&
	    old_vif_oper_type != sdata->vif.bss_conf.channel_type)
		ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_HT);

	return 0;
__channel_busy:
#ifdef CONFIG_ATBM_AP_CHANNEL_CHANGE_EVENT
	if(sdata->vif.type == NL80211_IFTYPE_AP){
	
		//send AP channel event , this is delay work ,because hostapd may have not been setup, we must wait it

		ieee80211_queue_delayed_work(&sdata->local->hw,
				     &sdata->ap_channel_event_work,
				     msecs_to_jiffies(2000));
		//must return success .else hostapd will not setup
		 return 0;
	}
#endif //#ifdef CONFIG_ATBM_AP_CHANNEL_CHANGE_EVENT
	
	return -EBUSY;
}
int ieee80211_set_channel(struct wiphy *wiphy,
				 struct net_device *netdev,
				 struct ieee80211_channel *chan,
				 enum nl80211_channel_type channel_type)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
	int ret = 0;
	mutex_lock(&local->mtx);
	ret = __ieee80211_set_channel(wiphy,netdev,chan,channel_type);
	mutex_unlock(&local->mtx);

	return ret;
}
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0))
static int ieee80211_set_monitor_channel(struct wiphy *wiphy,
					 struct cfg80211_chan_def *chandef)
{
	 enum nl80211_channel_type chtype;
	 struct ieee80211_channel *chan = NULL;

	 chtype = cfg80211_get_chandef_type(chandef);
	 chan = chandef->chan;
	 return ieee80211_set_channel(wiphy,NULL,chan,chtype);
}
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0))
static int ieee80211_set_monitor_channel(struct wiphy *wiphy,
					 struct ieee80211_channel *chan,
					 enum nl80211_channel_type channel_type)
{
	return ieee80211_set_channel(wiphy, NULL, chan, channel_type);
}	
#endif
#ifdef CONFIG_PM
static int ieee80211_suspend(struct wiphy *wiphy,
			     struct cfg80211_wowlan *wowlan)
{
	return __ieee80211_suspend(wiphy_priv(wiphy), wowlan);
}

static int ieee80211_resume(struct wiphy *wiphy)
{
	return __ieee80211_resume(wiphy_priv(wiphy));
}
#else
#define ieee80211_suspend NULL
#define ieee80211_resume NULL
#endif
//int g_connetting =0;
static int ieee80211_scan(struct wiphy *wiphy
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 6, 0))
	, struct net_device *dev
	#endif
	,struct cfg80211_scan_request *req
	)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0))
	struct net_device *dev = wdev_to_ndev(req->wdev);
#endif
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	//if(ETF_Test_is_Start()||(g_connetting)){
	if(ETF_Test_is_Start()){
		atbm_printk_err("ieee80211_scan drop ETF_bStartTx/Rx\n");
		return -EBUSY;
	}
	if(atbm_ieee80211_suspend(sdata->local)==true){
		
		atbm_printk_err("ieee80211_scan drop:suspend\n");
		return -EBUSY;
	}
	if(sdata->local->adaptive_started == true && time_is_after_jiffies(sdata->local->adaptive_started_time+(5*60*HZ))){
		atbm_printk_err("%s:adaptive runing\n",__func__);
		return -EBUSY;
	}
	switch (ieee80211_vif_type_p2p(&sdata->vif)) {
	case NL80211_IFTYPE_STATION:
#ifdef CONFIG_ATBM_SUPPORT_IBSS
	case NL80211_IFTYPE_ADHOC:
#endif
#ifdef CONFIG_MAC80211_ATBM_MESH
	case NL80211_IFTYPE_MESH_POINT:
#endif
#ifdef CONFIG_ATBM_SUPPORT_P2P
	case NL80211_IFTYPE_P2P_CLIENT:
		break;
	case NL80211_IFTYPE_P2P_GO:
		if (sdata->local->ops->hw_scan)
        break; 
    fallthrough;
		/*
		 * FIXME: implement NoA while scanning in software,
		 * for now fall through to allow scanning only when
		 * beaconing hasn't been configured yet
		 */
#else
		break;
#endif	
	case NL80211_IFTYPE_AP:
		if (sdata->u.ap.beacon)
			return -EOPNOTSUPP;
		break;
	default:
		return -EOPNOTSUPP;
	}
	if(sdata->local->only_monitors){
		if(sdata->local->monitor_sdata){
			atbm_printk_err("[%s] in monitor mode,cannot scan\n",sdata->local->monitor_sdata->name);
		}
		return -EOPNOTSUPP;
	}
	return ieee80211_request_scan(sdata, req);
}
#ifdef CONFIG_ATBM_SUPPORT_SCHED_SCAN
static int
ieee80211_sched_scan_start(struct wiphy *wiphy,
			   struct net_device *dev,
			   struct cfg80211_sched_scan_request *req)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	if(ETF_Test_is_Start()){
		atbm_printk_err("ieee80211_sched_scan_start drop ETF_bStartTx/Rx\n");
		return -EOPNOTSUPP;
	}

	if (!sdata->local->ops->sched_scan_start)
		return -EOPNOTSUPP;

	return ieee80211_request_sched_scan_start(sdata, req);
}

static int
ieee80211_sched_scan_stop(struct wiphy *wiphy, struct net_device *dev)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	if(ETF_Test_is_Start()){
		atbm_printk_err("ieee80211_sched_scan_stop drop ETF_bStartTx/Rx\n");
		return -EOPNOTSUPP;
	}

	if (!sdata->local->ops->sched_scan_stop)
		return -EOPNOTSUPP;

	return ieee80211_request_sched_scan_stop(sdata);
}
#endif
#ifdef CONFIG_ATBM_AP_CHANNEL_CHANGE_EVENT

//add by wp, change AP channel if ap && sta channel not the same ,send event to hostapd
void ieee80211_start_ap_changechannel_work(struct ieee80211_sub_if_data *ap_sdata,int channel,u8 chann_type)
{

#ifndef  WLAN_REASON_STA_CHANNEL_CHANGE
#define  WLAN_REASON_STA_CHANNEL_CHANGE 9999
#endif  //WLAN_REASON_DRIVER_RESET_LOCAL

	u8 mac[ETH_ALEN]={0x00,0x11,0x22,0x33,0x44,0x55};
	struct sk_buff *skb;
	struct atbm_ieee80211_mgmt *deauth;
	//struct ieee80211_local *local=ap_sdata->local;


	atbm_printk_err("ieee80211_start_ap_changechannel_work chann_type %d channel %d\n",chann_type,channel);

	skb = atbm_dev_alloc_skb(sizeof(struct atbm_ieee80211_mgmt) + 64);
	atbm_skb_reserve(skb, 64);
	deauth = (struct atbm_ieee80211_mgmt *)atbm_skb_put(skb, sizeof(struct atbm_ieee80211_mgmt));
	WARN_ON(!deauth);
	deauth->duration = 0x1234;
	memcpy(deauth->da, mac, ETH_ALEN);
	memcpy(deauth->sa, mac, ETH_ALEN);
	memcpy(deauth->bssid, mac, ETH_ALEN);
	deauth->frame_control = cpu_to_le16(IEEE80211_FTYPE_MGMT |
	                                            IEEE80211_STYPE_DEAUTH |
	                                            IEEE80211_FCTL_TODS);
	deauth->u.deauth.reason_code = WLAN_REASON_STA_CHANNEL_CHANGE;
	deauth->seq_ctrl = channel;
	if(chann_type == NL80211_CHAN_HT40MINUS){
		deauth->duration = channel-4;
	}
	else if(chann_type == NL80211_CHAN_HT40PLUS){
		deauth->duration = channel+4;
	}
	else {
		deauth->duration = 0;
	}
#if (LINUX_VERSION_CODE <KERNEL_VERSION(3,11,0))
	cfg80211_send_unprot_deauth(ap_sdata->dev, (u8 *)deauth, skb->len);
#else
	cfg80211_rx_unprot_mlme_mgmt(ap_sdata->dev, (u8 *)deauth, skb->len);
#endif
	atbm_dev_kfree_skb_any(skb);


}
/*
change hostapd channel
*/
void ieee80211_start_ap_changechannel(struct wiphy *wiphy, struct ieee80211_sub_if_data *sdata,struct cfg80211_auth_request *req)
{

	struct ieee80211_local *local=sdata->local;
	struct ieee80211_sub_if_data *ap_sdata;
	struct ieee80211_channel *chan=req->bss->channel;
 	enum nl80211_channel_type channel_type = NL80211_CHAN_HT20;	
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(local, sdata);
	struct ieee80211_supported_band *sband=local->hw.wiphy->bands[chan_state->conf.channel->band];
	const u8 *ht_information_ie=NULL;
	struct ieee80211_ht_info *hti;
#if (LINUX_VERSION_CODE <KERNEL_VERSION(3,10,0))
	if(req->bss->len_proberesp_ies)
		ht_information_ie  = 	atbm_ieee80211_find_ie(ATBM_WLAN_EID_HT_INFORMATION,req->bss->proberesp_ies,req->bss->len_proberesp_ies );
	else
		ht_information_ie  = 	atbm_ieee80211_find_ie(ATBM_WLAN_EID_HT_INFORMATION,req->bss->beacon_ies,req->bss->len_beacon_ies );
#else

	if(req->bss->proberesp_ies && req->bss->proberesp_ies->len)
		ht_information_ie  =	atbm_ieee80211_find_ie(ATBM_WLAN_EID_HT_INFORMATION,req->bss->proberesp_ies->data, req->bss->proberesp_ies->len );
	else if(req->bss->beacon_ies && req->bss->beacon_ies->len)
		ht_information_ie  =	atbm_ieee80211_find_ie(ATBM_WLAN_EID_HT_INFORMATION,req->bss->beacon_ies->data,req->bss->beacon_ies->len );

#endif
	//get channel type from IE
	if(ht_information_ie){
		hti = (struct ieee80211_ht_info *)&ht_information_ie[2];
		

		if ( (sband->ht_cap.cap & IEEE80211_HT_CAP_SUP_WIDTH_20_40) &&
		    (hti->ht_param & IEEE80211_HT_PARAM_CHAN_WIDTH_ANY)) {
			switch(hti->ht_param & IEEE80211_HT_PARAM_CHA_SEC_OFFSET) {
			case IEEE80211_HT_PARAM_CHA_SEC_ABOVE:
				if (!(chan->flags & IEEE80211_CHAN_NO_HT40PLUS))
					channel_type = NL80211_CHAN_HT40PLUS;
				break;
			case IEEE80211_HT_PARAM_CHA_SEC_BELOW:
				if (!(chan->flags & IEEE80211_CHAN_NO_HT40MINUS))
					channel_type = NL80211_CHAN_HT40MINUS;
				break;
			}
		}
	}
	else {
		atbm_printk_err("ieee80211_start_ap_changechannel no HT\n");
	}

	//find AP interface , change channel
	list_for_each_entry_rcu(ap_sdata, &local->interfaces, list) {
		if (!ieee80211_sdata_running(ap_sdata))
			continue;
		if (ap_sdata==sdata)
			continue;
		if ((ap_sdata->vif.type != NL80211_IFTYPE_AP)){
			continue;
		}
		atbm_printk_err("ieee80211_start_ap_changechannel_tmp\n");
		//set channel change to hal
		ieee80211_set_channel(wiphy,ap_sdata->dev,chan,channel_type);
		//change AP beacon.prbrsp DSparam
		ieee80211_bss_info_change_notify(ap_sdata,	BSS_CHANGED_BEACON);
		//set channel change to hostapd
		ieee80211_start_ap_changechannel_work(ap_sdata,channel_hw_value(chan),channel_type);
		break;
	}	


}

/*
change hostapd channel
*/
void ieee80211_ap_setchannel_fail(struct ieee80211_sub_if_data *ap_sdata,struct ieee80211_channel *chan,enum nl80211_channel_type channel_type )
{

	//find AP interface , change channel
	do {
		
		if ((ap_sdata->vif.type != NL80211_IFTYPE_AP)){
			break;
		}
		atbm_printk_err("ieee80211_ap_setchannel_fail\n");
		//set channel change to hostapd
		ieee80211_start_ap_changechannel_work(ap_sdata,channel_hw_value(chan),channel_type);
		break;
	}	while(0);


}
//send AP channel event , this is delay work ,because hostapd may have not been setup, we must wait it
void ieee80211_ap_channel_event_work(struct atbm_work_struct *work)
{
	struct atbm_delayed_work *delayed_work = atbm_to_delayed_work(work);
	struct ieee80211_sub_if_data *sdata =container_of(delayed_work, struct ieee80211_sub_if_data, ap_channel_event_work);
	//struct ieee80211_sub_if_data *sdata =
	//	container_of(work, struct ieee80211_sub_if_data, ap_channel_event_work.work);
	struct ieee80211_local *local=sdata->local;
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(local, sdata);
	
	
	atbm_printk_err("ieee80211_ap_channel_event_work\n");
	ieee80211_ap_setchannel_fail(sdata,chan_state->oper_channel, chan_state->_oper_channel_type);
	
}
#endif  //#ifdef CONFIG_ATBM_AP_CHANNEL_CHANGE_EVENT


static int ieee80211_auth(struct wiphy *wiphy, struct net_device *dev,
			  struct cfg80211_auth_request *req)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(local, sdata);
	if(atbm_ieee80211_suspend(local)==true){
		
		atbm_printk_err("ieee80211_auth drop:suspend\n");
		return -EBUSY;
	}
	switch (ieee80211_get_channel_mode(local, sdata)) {
	case CHAN_MODE_HOPPING:
		atbm_printk_err("(%s) authen err CHAN_MODE_HOPPING\n",sdata->name);
		return -EBUSY;
	case CHAN_MODE_FIXED:
		if (chan_state->oper_channel == req->bss->channel)
			break;
		atbm_printk_err("(%s) authen err CHAN_MODE_FIXED %d  %d\n",sdata->name,
													channel_hw_value(chan_state->oper_channel),
													channel_hw_value(req->bss->channel));
#ifdef CONFIG_ATBM_AP_CHANNEL_CHANGE_EVENT
		//add by wp, change AP channel if ap && sta channel not the same
		ieee80211_start_ap_changechannel(wiphy,sdata,req);
		if (chan_state->oper_channel == req->bss->channel)
			break;
		atbm_printk_err("(%s) authen  CHAN_CHANGE_OK \n",sdata->name);
#endif  //#ifdef CONFIG_ATBM_AP_CHANNEL_CHANGE_EVENT
		return -EBUSY;
	case CHAN_MODE_UNDEFINED:
		break;
	}
	return ieee80211_mgd_auth(IEEE80211_DEV_TO_SUB_IF(dev), req);
}

static int ieee80211_assoc(struct wiphy *wiphy, struct net_device *dev,
			   struct cfg80211_assoc_request *req)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(local, sdata);

	if(atbm_ieee80211_suspend(local)==true){
		
		atbm_printk_err("ieee80211_assoc drop:suspend\n");
		return -EBUSY;
	}
	switch (ieee80211_get_channel_mode(local, sdata)) {
	case CHAN_MODE_HOPPING:
		return -EBUSY;
	case CHAN_MODE_FIXED:
		if (chan_state->oper_channel == req->bss->channel)
			break;
		return -EBUSY;
	case CHAN_MODE_UNDEFINED:
		break;
	}
//	g_connetting =1;
	LOCAL_SET_CONNECT_RUNNING(local);
	
//	del_timer_sync(&sdata->u.mgd.scan_delay_timer);
//	mod_timer(&sdata->u.mgd.scan_delay_timer,jiffies +  msecs_to_jiffies(10000));
	
	return ieee80211_mgd_assoc(IEEE80211_DEV_TO_SUB_IF(dev), req);
}

static int ieee80211_deauth(struct wiphy *wiphy, struct net_device *dev,
			    struct cfg80211_deauth_request *req
			    #if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 4, 0)) 
			    ,void *cookie
			    #endif
			    )
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)) 
	void *cookie = dev->ieee80211_ptr;
#endif
	return ieee80211_mgd_deauth(IEEE80211_DEV_TO_SUB_IF(dev),
				    req, cookie);
}

static int ieee80211_disassoc(struct wiphy *wiphy, struct net_device *dev,
			      struct cfg80211_disassoc_request *req
			    #if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 4, 0)) 
			    ,void *cookie
			    #endif
			      )
{
	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)) 
	void *cookie = dev->ieee80211_ptr;
	#endif
	return ieee80211_mgd_disassoc(IEEE80211_DEV_TO_SUB_IF(dev),
				      req, cookie);
}
#ifdef CONFIG_ATBM_SUPPORT_IBSS
static int ieee80211_join_ibss(struct wiphy *wiphy, struct net_device *dev,
			       struct cfg80211_ibss_params *params)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(local, sdata);

	switch (ieee80211_get_channel_mode(local, sdata)) {
	case CHAN_MODE_HOPPING:
		return -EBUSY;
	case CHAN_MODE_FIXED:
		if (!params->channel_fixed)
			return -EBUSY;
		#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 0))
		if (chan_state->oper_channel == params->chandef.chan)
		#else
		if (chan_state->oper_channel == params->channel)
		#endif
			break;
		return -EBUSY;
	case CHAN_MODE_UNDEFINED:
		break;
	}

	return ieee80211_ibss_join(sdata, params);
}

static int ieee80211_leave_ibss(struct wiphy *wiphy, struct net_device *dev)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	return ieee80211_ibss_leave(sdata);
}
#endif
static int ieee80211_set_wiphy_params(struct wiphy *wiphy,
		#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 0, 0))
		struct net_device *dev, 
		#endif
		u32 changed)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
	#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 0, 0))
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	#else
	struct ieee80211_sub_if_data *sdata = NULL;
	#endif
	int err;
	u32 bss_changed = 0;
	
	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 0))
	list_for_each_entry(sdata, &local->interfaces, list)
		 if ((sdata->vif.type == NL80211_IFTYPE_AP)||(sdata->vif.type == NL80211_IFTYPE_P2P_GO))
		 	break;
	
	if(sdata == NULL)
	{
		return -EOPNOTSUPP;
	}
	#endif
	atbm_printk_err("%s %d \n",__func__,__LINE__);
	if (changed & WIPHY_PARAM_FRAG_THRESHOLD) {
		err = drv_set_frag_threshold(local, wiphy->frag_threshold);

		if (err)
			return err;
	}
	atbm_printk_err("%s %d \n",__func__,__LINE__);

	if (changed & WIPHY_PARAM_COVERAGE_CLASS) {
		err = drv_set_coverage_class(local, wiphy->coverage_class);

		if (err)
			return err;
	}
	atbm_printk_err("%s %d \n",__func__,__LINE__);

	if (changed & WIPHY_PARAM_RTS_THRESHOLD) {
		err = drv_set_rts_threshold(local, sdata,
					    sdata->vif.bss_conf.rts_threshold);

		if (err)
			return err;
	}
	atbm_printk_err("%s %d \n",__func__,__LINE__);

	if ((changed & WIPHY_PARAM_RETRY_SHORT) ||
	    (changed & WIPHY_PARAM_RETRY_LONG)) {
		sdata->vif.bss_conf.retry_short = wiphy->retry_short;
		sdata->vif.bss_conf.retry_long = wiphy->retry_long;
		bss_changed |= BSS_CHANGED_RETRY_LIMITS;
	}

	drv_bss_info_changed(local, sdata, &sdata->vif.bss_conf, bss_changed);
	atbm_printk_err("%s %d \n",__func__,__LINE__);
	return 0;
}
#ifdef CONFIG_ATBM_MAC80211_NO_USE
static int ieee80211_dump_survey(struct wiphy *wiphy, struct net_device *dev,
				 int idx, struct survey_info *survey)
{
	struct ieee80211_local *local = wdev_priv(dev->ieee80211_ptr);

	return drv_get_survey(local, idx, survey);
}

static int ieee80211_set_tx_power(struct wiphy *wiphy,
	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0))
	struct wireless_dev *wdev,
	#endif
	enum nl80211_tx_power_setting type, int mbm)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct ieee80211_sub_if_data *sdata;
	struct ieee80211_channel *chan;
	u32 changes = 0;

	switch (type) {
	case NL80211_TX_POWER_AUTOMATIC:
		local->user_power_level = -1;
		break;
	case NL80211_TX_POWER_LIMITED:
		if (mbm < 0 || (mbm % 100))
			return -EOPNOTSUPP;
		local->user_power_level = MBM_TO_DBM(mbm);
		break;
	case NL80211_TX_POWER_FIXED:
		if (mbm < 0 || (mbm % 100))
			return -EOPNOTSUPP;
#ifdef CONFIG_ATBM_SUPPORT_MULTI_CHANNEL
		/* TODO: move to cfg80211 when it knows the channel */
		if (local->hw.flags & IEEE80211_HW_SUPPORTS_MULTI_CHANNEL) {
			rcu_read_lock();
			list_for_each_entry_rcu(sdata, &local->interfaces, list) {
				chan = sdata->chan_state.conf.channel;
				if (MBM_TO_DBM(mbm) > chan->max_power) {
					rcu_read_unlock();
					return -EINVAL;
				}
			}
			rcu_read_unlock();
		} else
#endif
		{
			chan = local->chan_state.conf.channel;
			if (MBM_TO_DBM(mbm) > chan->max_power)
				return -EINVAL;
		}
		local->user_power_level = MBM_TO_DBM(mbm);
		break;
	}

	ieee80211_hw_config(local, changes);

	return 0;
}

static int ieee80211_get_tx_power(struct wiphy *wiphy, 
	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0))
	struct wireless_dev *wdev,
	#endif
	int *dbm)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);

	*dbm = local->hw.conf.power_level;

	return 0;
}
#ifdef CONFIG_ATBM_SUPPORT_WDS
static int ieee80211_set_wds_peer(struct wiphy *wiphy, struct net_device *dev,
				  const u8 *addr)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	memcpy(&sdata->u.wds.remote_addr, addr, ETH_ALEN);

	return 0;
}
#endif
static void ieee80211_rfkill_poll(struct wiphy *wiphy)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);

	drv_rfkill_poll(local);
}
static int ieee80211_set_antenna(struct wiphy *wiphy, u32 tx_ant, u32 rx_ant)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);

	if (local->started)
		return -EOPNOTSUPP;

	return drv_set_antenna(local, tx_ant, rx_ant);
}

static int ieee80211_get_antenna(struct wiphy *wiphy, u32 *tx_ant, u32 *rx_ant)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);

	return drv_get_antenna(local, tx_ant, rx_ant);
}
//#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 25))
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,18,00))
static int ieee80211_set_ringparam(struct wiphy *wiphy, u32 tx, u32 rx)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);

	return drv_set_ringparam(local, tx, rx);
}

static void ieee80211_get_ringparam(struct wiphy *wiphy,
				    u32 *tx, u32 *tx_max, u32 *rx, u32 *rx_max)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);

	drv_get_ringparam(local, tx, tx_max, rx, rx_max);
}
#endif
#endif

#if defined (CONFIG_NL80211_TESTMODE) && defined (CONFIG_ATBM_TEST_TOOL)
#ifndef CONFIG_RATE_HW_CONTROL
/* altbeam test*/
extern int rate_altm_control_test(struct wiphy *wiphy, void *data, int len);
#endif
#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 9, 0))

int ieee80211_altmtest_cmd(struct wiphy *wiphy, void *data, int len)
{
	int err = 0;
	struct ieee80211_local *local = wiphy_priv(wiphy);
#ifndef CONFIG_RATE_HW_CONTROL
	err = rate_altm_control_test(wiphy, data, len);
#endif
	if (!local->ops->testmode_cmd)
		return -EOPNOTSUPP;
	err |= local->ops->testmode_cmd(&local->hw, data, len);
	return err;
}
#else
int ieee80211_altmtest_cmd(struct wiphy *wiphy, struct wireless_dev *wdev,void *data, int len)
{
	int err = 0;
	struct ieee80211_local *local = wiphy_priv(wiphy);
#ifndef CONFIG_RATE_HW_CONTROL
	err = rate_altm_control_test(wiphy, data, len);
#endif
	if (!local->ops->testmode_cmd)
		return -EOPNOTSUPP;
	err |= local->ops->testmode_cmd(&local->hw, data, len);
	return err;
}


#endif
/*altbeam test end*/

#if 0
static int ieee80211_testmode_cmd(struct wiphy *wiphy, void *data, int len)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);

	if (!local->ops->testmode_cmd)
		return -EOPNOTSUPP;

	return local->ops->testmode_cmd(&local->hw, data, len);
}
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 1, 0))
static int ieee80211_testmode_dump(struct wiphy *wiphy,
				   struct sk_buff *skb,
				   struct netlink_callback *cb,
				   void *data, int len)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);

	if (!local->ops->testmode_dump)
		return -EOPNOTSUPP;

	return local->ops->testmode_dump(&local->hw, skb, cb, data, len);
}
#endif
#else
#define ieee80211_altmtest_cmd NULL
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 1, 0))
#define ieee80211_testmode_dump NULL
#endif
#endif
#ifdef CONFIG_ATBM_SMPS
int __ieee80211_request_smps(struct ieee80211_sub_if_data *sdata,
			     enum ieee80211_smps_mode smps_mode)
{
	const u8 *ap;
	enum ieee80211_smps_mode old_req;
	int err;

	lockdep_assert_held(&sdata->u.mgd.mtx);

	old_req = sdata->u.mgd.req_smps;
	sdata->u.mgd.req_smps = smps_mode;

	if (old_req == smps_mode &&
	    smps_mode != IEEE80211_SMPS_AUTOMATIC)
		return 0;

	/*
	 * If not associated, or current association is not an HT
	 * association, there's no need to send an action frame.
	 */
	if (!sdata->u.mgd.associated ||
	    sdata->vif.bss_conf.channel_type == NL80211_CHAN_NO_HT) {
		mutex_lock(&sdata->local->iflist_mtx);
		ieee80211_recalc_smps(sdata->local);
		mutex_unlock(&sdata->local->iflist_mtx);
		return 0;
	}

	ap = sdata->u.mgd.associated->bssid;

	if (smps_mode == IEEE80211_SMPS_AUTOMATIC) {
		if (sdata->u.mgd.powersave)
			smps_mode = IEEE80211_SMPS_DYNAMIC;
		else
			smps_mode = IEEE80211_SMPS_OFF;
	}

	/* send SM PS frame to AP */
	err = ieee80211_send_smps_action(sdata, smps_mode,
					 ap, ap);
	if (err)
		sdata->u.mgd.req_smps = old_req;

	return err;
}

static int ieee80211_set_power_mgmt(struct wiphy *wiphy, struct net_device *dev,
				    bool enabled, int timeout)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = wdev_priv(dev->ieee80211_ptr);

	if (sdata->vif.type != NL80211_IFTYPE_STATION)
		return -EOPNOTSUPP;

	if (!(local->hw.flags & IEEE80211_HW_SUPPORTS_PS))
		return -EOPNOTSUPP;

	if (enabled == sdata->u.mgd.powersave &&
	    timeout == sdata->dynamic_ps_forced_timeout)
		return 0;

	sdata->u.mgd.powersave = enabled;
	sdata->dynamic_ps_forced_timeout = timeout;
#ifdef CONFIG_ATBM_SMPS
	/* no change, but if automatic follow powersave */
	mutex_lock(&sdata->u.mgd.mtx);
	__ieee80211_request_smps(sdata, sdata->u.mgd.req_smps);
	mutex_unlock(&sdata->u.mgd.mtx);
#endif
	if (local->hw.flags & IEEE80211_HW_SUPPORTS_DYNAMIC_PS)
		ieee80211_hw_config(local, IEEE80211_CONF_CHANGE_PS);

	ieee80211_recalc_ps(local, -1);

	return 0;
}
#endif
#if 0
static void ieee80211_update_p2p_ps(struct ieee80211_sub_if_data *sdata,
				    struct cfg80211_p2p_ps *new_p2p_ps)
{
	struct ieee80211_bss_conf *conf = &sdata->vif.bss_conf;
	struct cfg80211_p2p_ps *p2p_ps = &conf->p2p_ps;

	if (new_p2p_ps->count != -1)
		p2p_ps->count = new_p2p_ps->count;
	if (new_p2p_ps->start != -1)
		p2p_ps->start = new_p2p_ps->start;
	if (new_p2p_ps->duration != -1)
		p2p_ps->duration = new_p2p_ps->duration;
	if (new_p2p_ps->interval != -1)
		p2p_ps->interval = new_p2p_ps->interval;
	if (new_p2p_ps->legacy_ps != -1)
		p2p_ps->legacy_ps = new_p2p_ps->legacy_ps;
	if (new_p2p_ps->opp_ps != -1)
		p2p_ps->opp_ps = new_p2p_ps->opp_ps;
	if (new_p2p_ps->ctwindow != -1)
		p2p_ps->ctwindow = new_p2p_ps->ctwindow;
	if (new_p2p_ps->index != -1)
		p2p_ps->index = new_p2p_ps->index;

}

static int ieee80211_set_p2p_power_mgmt(struct wiphy *wiphy,
					struct net_device *dev,
					struct cfg80211_p2p_ps *p2p_ps)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = wdev_priv(dev->ieee80211_ptr);

	if (!sdata->vif.p2p)
		return -EOPNOTSUPP;

	if (!(local->hw.flags & IEEE80211_HW_SUPPORTS_P2P_PS))
		return -EOPNOTSUPP;

	ieee80211_update_p2p_ps(sdata, p2p_ps);
	ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_P2P_PS);

	return 0;
}
#endif
#ifdef CONFIG_ATBM_SUPPORT_RCPI_RSSI
static int ieee80211_set_cqm_rssi_config(struct wiphy *wiphy,
					 struct net_device *dev,
					 s32 rssi_thold, u32 rssi_hyst)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = wdev_priv(dev->ieee80211_ptr);
	struct ieee80211_vif *vif = &sdata->vif;
	struct ieee80211_bss_conf *bss_conf = &vif->bss_conf;

	if (rssi_thold == bss_conf->cqm_rssi_thold &&
	    rssi_hyst == bss_conf->cqm_rssi_hyst)
		return 0;

	bss_conf->cqm_rssi_thold = rssi_thold;
	bss_conf->cqm_rssi_hyst = rssi_hyst;

	if (!(local->hw.flags & IEEE80211_HW_SUPPORTS_CQM_RSSI)) {
		if (sdata->vif.type != NL80211_IFTYPE_STATION)
			return -EOPNOTSUPP;
		return 0;
	}

	/* tell the driver upon association, unless already associated */
	if (sdata->u.mgd.associated)
		ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_CQM);

	return 0;
}
#endif
#if 0
static int ieee80211_set_cqm_beacon_miss_config(struct wiphy *wiphy,
						struct net_device *dev,
						u32 beacon_thold)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = wdev_priv(dev->ieee80211_ptr);
	struct ieee80211_vif *vif = &sdata->vif;
	struct ieee80211_bss_conf *bss_conf = &vif->bss_conf;

	if (beacon_thold == bss_conf->cqm_beacon_miss_thold)
		return 0;

	bss_conf->cqm_beacon_miss_thold = beacon_thold;

	if (!(local->hw.flags & IEEE80211_HW_SUPPORTS_CQM_BEACON_MISS)) {
		if (sdata->vif.type != NL80211_IFTYPE_STATION)
			return -EOPNOTSUPP;
		return 0;
	}

	if (sdata->u.mgd.associated)
		ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_CQM);

	return 0;
}

static int ieee80211_set_cqm_tx_fail_config(struct wiphy *wiphy,
					    struct net_device *dev,
					    u32 tx_thold)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = wdev_priv(dev->ieee80211_ptr);
	struct ieee80211_vif *vif = &sdata->vif;
	struct ieee80211_bss_conf *bss_conf = &vif->bss_conf;

	if (tx_thold == bss_conf->cqm_tx_fail_thold)
		return 0;

	bss_conf->cqm_tx_fail_thold = tx_thold;

	if (!(local->hw.flags & IEEE80211_HW_SUPPORTS_CQM_TX_FAIL)) {
		if (sdata->vif.type != NL80211_IFTYPE_STATION)
			return -EOPNOTSUPP;
		return 0;
	}

	if (sdata->u.mgd.associated)
		ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_CQM);

	return 0;
}
#endif
#ifndef CONFIG_RATE_HW_CONTROL
static int ieee80211_set_bitrate_mask(struct wiphy *wiphy,
				      struct net_device *dev,
				      const u8 *addr,
				      const struct cfg80211_bitrate_mask *mask)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = wdev_priv(dev->ieee80211_ptr);
	int i, ret;

	if (local->hw.flags & IEEE80211_HW_HAS_RATE_CONTROL) {
		ret = drv_set_bitrate_mask(local, sdata, mask);
		if (ret)
			return ret;
	}

	for (i = 0; i < IEEE80211_NUM_BANDS; i++)
		sdata->rc_rateidx_mask[i] = mask->control[i].legacy;

	return 0;
}
#endif
#ifdef CONFIG_ATBM_SUPPORT_P2P
static u64 ieee80211_mgmt_tx_cookie(struct ieee80211_local *local)
{
	local->roc_cookie_counter++;

	/* wow, you wrapped 64 bits ... more likely a bug */
	if (WARN_ON(local->roc_cookie_counter == 0))
		local->roc_cookie_counter++;

	return local->roc_cookie_counter;
}

static int ieee80211_start_roc_work(struct ieee80211_local *local,
					struct net_device *dev,
				    struct ieee80211_sub_if_data *sdata,
				    struct ieee80211_channel *channel,
				    enum nl80211_channel_type channel_type,
				    unsigned int duration, u64 *cookie,
				    struct sk_buff *txskb)
{
	struct ieee80211_roc_work *roc, *tmp;
	bool queued = false;
	bool pended = false;
	int ret;
	int work_on = 0;

	lockdep_assert_held(&local->mtx);

#if 0
	if (local->use_chanctx && !local->ops->remain_on_channel)
		return -EOPNOTSUPP;
#endif
	if(atbm_ieee80211_suspend(local)==true){
		
		atbm_printk_err("ieee80211_start_roc_work drop:suspend\n");
		return -EBUSY;
	}
	roc = atbm_kzalloc(sizeof(*roc), GFP_KERNEL);
	if (!roc)
		return -ENOMEM;

	roc->hw_roc_dev = dev;
	roc->chan = channel;
	roc->duration = duration;
	roc->req_duration = duration;
	roc->frame = txskb;
	roc->mgmt_tx_cookie = (unsigned long)txskb;
	roc->sdata = sdata;
	ATBM_INIT_DELAYED_WORK(&roc->work, ieee80211_sw_roc_work);
	INIT_LIST_HEAD(&roc->dependents);

	/*
	 * cookie is either the roc cookie (for normal roc)
	 * or the SKB (for mgmt TX)
	 */
	if (!txskb) {
		roc->cookie = ieee80211_mgmt_tx_cookie(local);
		*cookie = roc->cookie;
	} else {
		*cookie = (unsigned long)txskb;
	}
	
	work_on = !list_empty(&local->work_list);
	/* if there's one pending or we're scanning, queue this one */
	if (!list_empty(&local->roc_list) || local->scanning || work_on)
		goto out_check_combine;

	/* if not HW assist, just queue & schedule work */
	if (!local->ops->remain_on_channel) {
		ieee80211_queue_delayed_work(&local->hw, &roc->work, 0);
		goto out_queue;
	}

	/* otherwise actually kick it off here (for error handling) */

	/*
	 * If the duration is zero, then the driver
	 * wouldn't actually do anything. Set it to
	 * 10 for now.
	 *
	 * TODO: cancel the off-channel operation
	 *       when we get the SKB's TX status and
	 *       the wait time was zero before.
	 */
	if (!duration)
		duration = 100;
	BUG_ON(local->roc_pendding != NULL);
	BUG_ON(local->roc_pendding_sdata != NULL);
	atbm_printk_cfg("%s:roc->cookie(%llx)\n",__func__,roc->cookie);
	
#ifdef CONFIG_ATBM_STA_LISTEN
	local->hw_roc_channel = channel;
	__ieee80211_recalc_idle(local);
#endif
	ret = drv_remain_on_channel(local, sdata, channel, NL80211_CHAN_NO_HT, duration,
			(txskb ? roc->mgmt_tx_cookie: (roc->cookie)));
	if (ret) {
#ifdef CONFIG_ATBM_STA_LISTEN
	local->hw_roc_channel = NULL;
	__ieee80211_recalc_idle(local);
#endif
		atbm_kfree(roc);
		return ret;
	}
	local->hw_roc_channel = channel;

	roc->started = true;
	goto out_queue;

 out_check_combine:
	list_for_each_entry(tmp, &local->roc_list, list) {
		if (tmp->chan != channel || tmp->sdata != sdata)
			continue;

		/*
		 * Extend this ROC if possible:
		 *
		 * If it hasn't started yet, just increase the duration
		 * and add the new one to the list of dependents.
		 */
		if (!tmp->started) {
			list_add_tail(&roc->list, &tmp->dependents);
			tmp->duration = max(tmp->duration, roc->duration);
			queued = true;
			atbm_printk_cfg( "%s:roc not start add dependents\n",__func__);
			break;
		}

		/* If it has already started, it's more difficult ... */
		if (local->ops->remain_on_channel) {
			unsigned long j = jiffies;

			/*
			 * In the offloaded ROC case, if it hasn't begun, add
			 * this new one to the dependent list to be handled
			 * when the the master one begins. If it has begun,
			 * check that there's still a minimum time left and
			 * if so, start this one, transmitting the frame, but
			 * add it to the list directly after this one with a
			 * a reduced time so we'll ask the driver to execute
			 * it right after finishing the previous one, in the
			 * hope that it'll also be executed right afterwards,
			 * effectively extending the old one.
			 * If there's no minimum time left, just add it to the
			 * normal list.
			 */
			if (!tmp->hw_begun) {
				list_add_tail(&roc->list, &tmp->dependents);
				queued = true;
				atbm_printk_cfg("%s:hw roc not started add dependents\n",__func__);
				break;
			}

			if (time_before(j + IEEE80211_ROC_MIN_LEFT,
					tmp->hw_start_time +
					msecs_to_jiffies(tmp->duration+tmp->hw_extend_roc_time))) {
				int new_dur;

				ieee80211_handle_roc_started(roc);

				new_dur = roc->duration -
					  jiffies_to_msecs(tmp->hw_start_time +
							   msecs_to_jiffies(
								tmp->duration+tmp->hw_extend_roc_time) -
							   j);

				if (new_dur > 0) {
					/* add right after tmp */
					list_add(&roc->list, &tmp->list);
				} else {
					list_add_tail(&roc->list,
						      &tmp->dependents);
				}
				queued = true;
				atbm_printk_cfg( "%s:hw roc has been started add dependents\n",__func__);
			}else
			{
				atbm_printk_cfg("%s:remain on channel time duration is not enough\n",__func__);
			}
		} 
#ifdef CONFIG_ATBM_MAC80211_NO_USE
		else if (atbm_del_timer_sync(&tmp->work.timer)) {
			unsigned long new_end;

			/*
			 * In the software ROC case, cancel the timer, if
			 * that fails then the finish work is already
			 * queued/pending and thus we queue the new ROC
			 * normally, if that succeeds then we can extend
			 * the timer duration and TX the frame (if any.)
			 */

			list_add_tail(&roc->list, &tmp->dependents);
			queued = true;

			new_end = jiffies + msecs_to_jiffies(roc->duration);

			/* ok, it was started & we canceled timer */
			if (time_after(new_end, tmp->work.timer.expires))
				atbm_mod_timer(&tmp->work.timer, new_end);
			else
				atbm_add_timer(&tmp->work.timer);

			ieee80211_handle_roc_started(roc);
		}
#endif
		break;
	}

 out_queue:
	if ((!queued)&&(!work_on))
		list_add_tail(&roc->list, &local->roc_list);
	else if(work_on&&(!queued)&&(list_empty(&local->roc_list)))
	{
		if((local->roc_pendding == NULL)&&(local->roc_pendding_sdata == NULL)){
			local->roc_pendding = roc;
			local->roc_pendding_sdata = sdata;
			atbm_printk_cfg("%s:work_list is not empty, pendding remain on channel skb(%p),ch(%d)\n",
				__func__,txskb,channel_hw_value(channel));
			ret = 0;
			pended = true;
			roc->pending_start_time = jiffies;
		}
		else
		{
			atbm_printk_cfg("%s:work_list is not empty, has one pending,so cancle skb(%p),ch(%d)\n",
				__func__,txskb,channel_hw_value(channel));
			atbm_kfree(roc);
			return -EBUSY;
		}
	}else if(!queued){
		atbm_printk_cfg("%s:work_list(%d), ,cancle skb(%p),ch(%d)\n",
				__func__,work_on,txskb,channel_hw_value(channel));
		atbm_kfree(roc);
		return -EBUSY;
	}
	#if 0
	/*
	 * cookie is either the roc cookie (for normal roc)
	 * or the SKB (for mgmt TX)
	 */
	if (!txskb) {
		/* local->mtx protects this */
		local->roc_cookie_counter++;
		roc->cookie = local->roc_cookie_counter;
		/* wow, you wrapped 64 bits ... more likely a bug */
		if (WARN_ON(roc->cookie == 0)) {
			roc->cookie = 1;
			local->roc_cookie_counter++;
		}
		*cookie = roc->cookie;
	} else {
		*cookie = (unsigned long)txskb;
	}
	#endif
	if(pended == true)
		atbm_printk_cfg( "%s:pended cookie(%x)\n",__func__,(unsigned int)(*cookie));
	
	atbm_printk_cfg( "%s:return cookie(%llx)\n",__func__,*cookie);
	return 0;
}
int ieee80211_start_pending_roc_work(struct ieee80211_local *local,
					struct net_device *dev,
				    struct ieee80211_sub_if_data *sdata,
				    struct ieee80211_channel *channel,
				    enum nl80211_channel_type channel_type,
				    unsigned int duration, u64 *cookie,
				    struct sk_buff *txskb)
{
	struct ieee80211_roc_work *roc;
	int ret;

	lockdep_assert_held(&local->mtx);

	roc = atbm_kzalloc(sizeof(*roc), GFP_KERNEL);
	if (!roc)
		return -ENOMEM;

	roc->hw_roc_dev = dev;
	roc->chan = channel;
	roc->duration = duration;
	roc->req_duration = duration;
	roc->frame = txskb;
	roc->mgmt_tx_cookie = (unsigned long)txskb;
	roc->sdata = sdata;
	roc->cookie = *cookie;
	
	ATBM_INIT_DELAYED_WORK(&roc->work, ieee80211_sw_roc_work);
	INIT_LIST_HEAD(&roc->dependents);
	
	BUG_ON(local->scanning);
	BUG_ON(!list_empty(&local->roc_list));

	if (!duration)
		duration = 100;

	ret = drv_remain_on_channel(local, sdata, channel, NL80211_CHAN_NO_HT, duration,
			(txskb ? roc->mgmt_tx_cookie: (roc->cookie)));
	if (ret) {
		atbm_kfree(roc);
		return ret;
	}
	local->hw_roc_channel = channel;

	roc->started = true;
	list_add_tail(&roc->list, &local->roc_list);

	return ret;
}

static int ieee80211_remain_on_channel(struct wiphy *wiphy,
					#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0))
					struct wireless_dev *wdev,
					#else
					struct net_device *dev,
					#endif
				       struct ieee80211_channel *chan,
				       #if (LINUX_VERSION_CODE < KERNEL_VERSION(3,8,0))
				       enum nl80211_channel_type channel_type,
				       #endif
				       unsigned int duration,
				       u64 *cookie)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0))
	struct net_device *dev = wdev_to_ndev(wdev);
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0))
    enum nl80211_channel_type channel_type = NL80211_CHAN_HT20;
#endif
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	int ret;

	mutex_lock(&local->mtx);
	ret = ieee80211_start_roc_work(local, dev, sdata, chan, channel_type,
			       duration, cookie, NULL);
	mutex_unlock(&local->mtx);

	return ret;
}

static int ieee80211_cancel_roc(struct ieee80211_local *local,
				u64 cookie, bool mgmt_tx)
{
	struct ieee80211_roc_work *roc, *tmp, *found = NULL;
	int ret;

	mutex_lock(&local->mtx);
	atbm_printk_cfg("%s:cookie(%llx)\n",__func__,cookie);
	while(local->roc_pendding&&local->roc_pendding_sdata){
		struct ieee80211_roc_work *pendding_roc = local->roc_pendding;

		if (!mgmt_tx && pendding_roc->cookie != cookie)
			break;
		else if (mgmt_tx && pendding_roc->mgmt_tx_cookie != cookie)
			break;

		local->roc_pendding = NULL;
		local->roc_pendding_sdata = NULL;

		mutex_unlock(&local->mtx);
		atbm_printk_cfg("%s:cancle pendding roc\n",__func__);
		ieee80211_roc_notify_destroy(pendding_roc);
		return 0;
	}
	list_for_each_entry_safe(roc, tmp, &local->roc_list, list) {
		struct ieee80211_roc_work *dep, *tmp2;

		list_for_each_entry_safe(dep, tmp2, &roc->dependents, list) {
			if (!mgmt_tx && dep->cookie != cookie)
				continue;
			else if (mgmt_tx && dep->mgmt_tx_cookie != cookie)
				continue;
			/* found dependent item -- just remove it */
			list_del(&dep->list);
			mutex_unlock(&local->mtx);

			ieee80211_roc_notify_destroy(dep);
			return 0;
		}

		if (!mgmt_tx && roc->cookie != cookie)
			continue;
		else if (mgmt_tx && roc->mgmt_tx_cookie != cookie)
			continue;

		found = roc;
		break;
	}

	if (!found) {
		mutex_unlock(&local->mtx);
		return -ENOENT;
	}

	/*
	 * We found the item to cancel, so do that. Note that it
	 * may have dependents, which we also cancel (and send
	 * the expired signal for.) Not doing so would be quite
	 * tricky here, but we may need to fix it later.
	 */

	if (local->ops->remain_on_channel) {
		if (found->started) {
			ret = drv_cancel_remain_on_channel(local);
			if (WARN_ON_ONCE(ret)) {
				mutex_unlock(&local->mtx);
				return ret;
			}
			local->hw_roc_channel = NULL;
		}

		list_del(&found->list);

		if (found->started)
			ieee80211_start_next_roc(local);
		mutex_unlock(&local->mtx);

		ieee80211_roc_notify_destroy(found);
	} else {
		/* work may be pending so use it all the time */
		found->abort = true;
		ieee80211_queue_delayed_work(&local->hw, &found->work, 0);

		mutex_unlock(&local->mtx);

		/* work will clean up etc */
		atbm_flush_delayed_work(&found->work);
	}

	return 0;
}

static int ieee80211_cancel_remain_on_channel(struct wiphy *wiphy,
					#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0))
					struct wireless_dev *wdev,
					#else
					struct net_device *dev,
					#endif
					u64 cookie)
{
	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0))
	struct net_device *dev = wdev_to_ndev(wdev);
	#endif
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;

	return ieee80211_cancel_roc(local, cookie, false);
}
/*
mgmt_tx(&rdev->wiphy, wdev, params, cookie);
*/
static int ieee80211_mgmt_tx(struct wiphy *wiphy, 
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0))
			struct wireless_dev *wdev,
			#else
			struct net_device *dev,
#endif
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,14,0))
			struct ieee80211_channel *chan, bool offchan,
			#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,8,0))
			enum nl80211_channel_type channel_type,
			bool channel_type_valid,
			#endif
		       unsigned int wait,
			const u8 *buf, size_t len,
			#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0))
			bool no_cck,
			#endif
			#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,3,0))
			bool dont_wait_for_ack,
			#endif
#else
			struct cfg80211_mgmt_tx_params *params,
#endif
			u64 *cookie)
{
	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0))
	struct net_device *dev = wdev_to_ndev(wdev);
	#endif
	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0))
	enum nl80211_channel_type channel_type = NL80211_CHAN_HT20;
	#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,14,0))
	struct ieee80211_channel *chan = params->chan;
	bool offchan = params->offchan;
	unsigned int wait = params->wait;
	const u8 *buf = params->buf;
	size_t len = params->len;
	bool no_cck = params->no_cck;
	bool dont_wait_for_ack = params->dont_wait_for_ack;
#endif
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(local, sdata);
	struct sk_buff *skb;
	struct sta_info *sta;
//	struct ieee80211_work *wk;
	const struct atbm_ieee80211_mgmt *mgmt = (void *)buf;
	u32 flags = IEEE80211_TX_INTFL_NL80211_FRAME_TX |
		    IEEE80211_TX_CTL_REQ_TX_STATUS;
	bool is_offchan = false;
	int ret;
	unsigned long j, rem=0;
	struct ieee80211_roc_work *roc = NULL;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,14,0))
	dont_wait_for_ack = dont_wait_for_ack;
#endif
//	printk( "%s:req_chan(%x),tmp_channel(%x),oper_channel(%x),hw_roc_channel(%x)\n",__func__,*cookie);
	/* Check that we are on the requested channel for transmission */
	if (chan != chan_state->tmp_channel &&
	    chan != chan_state->oper_channel)
		is_offchan = true;
	
	#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,8,0))
	if (channel_type_valid &&
	    (channel_type != chan_state->tmp_channel_type &&
	     channel_type != chan_state->_oper_channel_type))
		is_offchan = true;
	#endif
	
	if (chan == local->hw_roc_channel) {
		/* TODO: check channel type? */
		is_offchan = false;
		flags |= IEEE80211_TX_CTL_TX_OFFCHAN;
	}
	
	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0))
	if (no_cck)
		flags |= IEEE80211_TX_CTL_NO_CCK_RATE;
	#endif
	
	if (is_offchan && !offchan)
		return -EBUSY;

	switch (sdata->vif.type) {
#ifdef CONFIG_ATBM_SUPPORT_IBSS
	case NL80211_IFTYPE_ADHOC:
#endif
	case NL80211_IFTYPE_AP:
	case NL80211_IFTYPE_AP_VLAN:
#ifdef CONFIG_ATBM_SUPPORT_P2P
	case NL80211_IFTYPE_P2P_GO:
#endif
#ifdef CONFIG_MAC80211_ATBM_MESH
	case NL80211_IFTYPE_MESH_POINT:
#endif
		if (!ieee80211_is_action(mgmt->frame_control) ||
		    mgmt->u.action.category == ATBM_WLAN_CATEGORY_PUBLIC)
			break;
		rcu_read_lock();
		sta = sta_info_get(sdata, mgmt->da);
		rcu_read_unlock();
		if (!sta)
			return -ENOLINK;
		break;
	case NL80211_IFTYPE_STATION:
#ifdef CONFIG_ATBM_SUPPORT_P2P
	case NL80211_IFTYPE_P2P_CLIENT:
#endif
		if (!sdata->u.mgd.associated&&(chan != local->hw_roc_channel))
		{
			is_offchan = true;
		}
		break;
	default:
		return -EOPNOTSUPP;
	}

	skb = atbm_dev_alloc_skb(local->hw.extra_tx_headroom + len);
	if (!skb) {
		return -ENOMEM;
	}
	atbm_skb_reserve(skb, local->hw.extra_tx_headroom);

	memcpy(atbm_skb_put(skb, len), buf, len);

	IEEE80211_SKB_CB(skb)->flags = flags;

	if (flags & IEEE80211_TX_CTL_TX_OFFCHAN)
		IEEE80211_SKB_CB(skb)->hw_queue =
			local->hw.offchannel_tx_hw_queue;

	skb->dev = sdata->dev;

	mutex_lock(&local->mtx);
	/*
	 * Can transmit right away if the channel was the
	 * right one and there's no wait involved... If a
	 * wait is involved, we might otherwise not be on
	 * the right channel for long enough!
	 */
	if (!list_empty(&local->roc_list)) {
		roc = list_first_entry(&local->roc_list, struct ieee80211_roc_work,
						list);
		j = jiffies;
		if (roc->started)
			rem = jiffies_to_msecs(roc->hw_start_time + msecs_to_jiffies(roc->duration+roc->hw_extend_roc_time) - j);
	}
	//if (!is_offchan && !wait && !sdata->vif.bss_conf.idle) {
	if (!is_offchan && !ieee80211_is_probe_resp(mgmt->frame_control) &&
			roc && roc->started && (rem < 20)) {
		is_offchan = true;
	}
	mutex_unlock(&local->mtx);

	if (!is_offchan) {
		*cookie = (unsigned long) skb;
		ieee80211_tx_skb(sdata, skb);
		return 0;
	}

	mutex_lock(&local->mtx);

	if (!wait)
		wait = 100;
	#if 0
	/* This will handle all kinds of coalescing and immediate TX */
	if(!list_empty(&local->work_list)){
			printk(KERN_ERR "%s:work_list is not empty,some sdata in connect station\n",__func__);
			ret = -EBUSY;
	}
	else
	#endif
		ret = ieee80211_start_roc_work(local, dev, sdata, chan, channel_type,
				       wait, cookie, skb);
	if (ret)
		atbm_kfree_skb(skb);

	mutex_unlock(&local->mtx);

	return ret;
}

static int ieee80211_mgmt_tx_cancel_wait(struct wiphy *wiphy,
			#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0))
			struct wireless_dev *wdev,
			#else
			struct net_device *dev,
			#endif
			u64 cookie)
{
//	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = wiphy_priv(wiphy);

	return ieee80211_cancel_roc(local, cookie, true);
}
#endif
#if 0
static void ieee80211_mgmt_frame_register(struct wiphy *wiphy,
			#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0))
			struct wireless_dev *wdev,
			#else
			struct net_device *dev,
			#endif
			u16 frame_type, bool reg)
{
	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0))
	struct net_device *dev = wdev_to_ndev(wdev);
	#endif
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	if (frame_type != (IEEE80211_FTYPE_MGMT | IEEE80211_STYPE_PROBE_REQ))
		return;

	if (reg)
		sdata->req_filt_flags |= FIF_PROBE_REQ;
	else
		sdata->req_filt_flags &= ~FIF_PROBE_REQ;

	ieee80211_queue_work(&local->hw, &sdata->reconfig_filter);
}
#endif 

#if 0
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
static int ieee80211_set_acm_up(struct wiphy *wiphy,
				struct net_device *net_dev,
				u8 enable_up, u8 update_up)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(net_dev);
	struct ieee80211_local *local = sdata->local;
	unsigned int access_category = 0;
	switch (update_up) {
	case 1:
	case 2:
		access_category |= BIT(1) | BIT(2); /* BK/- */
		break;
	case 4:
	case 5:
		access_category |= BIT(4) | BIT(5); /* CL/VI */
		break;
	case 6:
	case 7:
		access_category |= BIT(6) | BIT(7); /* VO/NC */
		break;
	default:
		access_category |= BIT(0) | BIT(3); /* BE/EE */
		break;
	}

	if (enable_up == 1)
		local->wmm_admitted_ups |= access_category;
	else
		local->wmm_admitted_ups &= ~access_category;

	return 0;
}
#endif /*CONFIG_ATBM_APOLLO_TESTMODE*/
#endif
#ifdef ATBM_POWERSAVE_SAVE_GROUP_KEY
#if (LINUX_VERSION_CODE > KERNEL_VERSION(3, 0, 8))
static int ieee80211_set_rekey_data(struct wiphy *wiphy,
				    struct net_device *dev,
				    struct cfg80211_gtk_rekey_data *data)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	if (!local->ops->set_rekey_data)
		return -EOPNOTSUPP;

	drv_set_rekey_data(local, sdata, data);

	return 0;
}
#endif
#endif

#ifdef ATBM_SURPORT_TDLS
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 16, 0))&&(LINUX_VERSION_CODE > KERNEL_VERSION(3, 0, 8))

static void ieee80211_tdls_add_ext_capab(struct sk_buff *skb)
{
	u8 *pos = (void *)atbm_skb_put(skb, 7);

	*pos++ = ATBM_WLAN_EID_EXT_CAPABILITY;
	*pos++ = 5; /* len */
	*pos++ = 0x0;
	*pos++ = 0x0;
	*pos++ = 0x0;
	*pos++ = 0x0;
	*pos++ = WLAN_EXT_CAPA5_TDLS_ENABLED;
}

static u16 ieee80211_get_tdls_sta_capab(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(local, sdata);
	u16 capab;

	capab = 0;
	if (chan_state->oper_channel->band != IEEE80211_BAND_2GHZ)
		return capab;

	if (!(local->hw.flags & IEEE80211_HW_2GHZ_SHORT_SLOT_INCAPABLE))
		capab |= WLAN_CAPABILITY_SHORT_SLOT_TIME;
	if (!(local->hw.flags & IEEE80211_HW_2GHZ_SHORT_PREAMBLE_INCAPABLE))
		capab |= WLAN_CAPABILITY_SHORT_PREAMBLE;

	return capab;
}
static void ieee80211_tdls_add_link_ie(struct sk_buff *skb, u8 *src_addr,
				       u8 *peer, u8 *bssid)
{
	struct ieee80211_tdls_lnkie *lnkid;

	lnkid = (void *)atbm_skb_put(skb, sizeof(struct ieee80211_tdls_lnkie));
	lnkid->ie_type = ATBM_WLAN_EID_LINK_ID;
	lnkid->ie_len = sizeof(struct ieee80211_tdls_lnkie) - 2;

	memcpy(lnkid->bssid, bssid, ETH_ALEN);
	memcpy(lnkid->init_sta, src_addr, ETH_ALEN);
	memcpy(lnkid->resp_sta, peer, ETH_ALEN);
}

static int
ieee80211_prep_tdls_encap_data(struct wiphy *wiphy, struct net_device *dev,
			       u8 *peer, u8 action_code, u8 dialog_token,
			       u16 status_code, struct sk_buff *skb)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_tdls_data *tf;

	tf = (void *)atbm_skb_put(skb, offsetof(struct ieee80211_tdls_data, u));

	memcpy(tf->da, peer, ETH_ALEN);
	memcpy(tf->sa, sdata->vif.addr, ETH_ALEN);
	tf->ether_type = cpu_to_be16(ETH_P_TDLS);
	tf->payload_type = WLAN_TDLS_SNAP_RFTYPE;

	switch (action_code) {
	case WLAN_TDLS_SETUP_REQUEST:
		tf->category = ATBM_WLAN_CATEGORY_TDLS;
		tf->action_code = WLAN_TDLS_SETUP_REQUEST;

		atbm_skb_put(skb, sizeof(tf->u.setup_req));
		tf->u.setup_req.dialog_token = dialog_token;
		tf->u.setup_req.capability =
			cpu_to_le16(ieee80211_get_tdls_sta_capab(sdata));

		ieee80211_add_srates_ie(&sdata->vif, skb);
		ieee80211_add_ext_srates_ie(&sdata->vif, skb);
		ieee80211_tdls_add_ext_capab(skb);
		break;
	case WLAN_TDLS_SETUP_RESPONSE:
		tf->category = ATBM_WLAN_CATEGORY_TDLS;
		tf->action_code = WLAN_TDLS_SETUP_RESPONSE;

		atbm_skb_put(skb, sizeof(tf->u.setup_resp));
		tf->u.setup_resp.status_code = cpu_to_le16(status_code);
		tf->u.setup_resp.dialog_token = dialog_token;
		tf->u.setup_resp.capability =
			cpu_to_le16(ieee80211_get_tdls_sta_capab(sdata));

		ieee80211_add_srates_ie(&sdata->vif, skb);
		ieee80211_add_ext_srates_ie(&sdata->vif, skb);
		ieee80211_tdls_add_ext_capab(skb);
		break;
	case WLAN_TDLS_SETUP_CONFIRM:
		tf->category = ATBM_WLAN_CATEGORY_TDLS;
		tf->action_code = WLAN_TDLS_SETUP_CONFIRM;

		atbm_skb_put(skb, sizeof(tf->u.setup_cfm));
		tf->u.setup_cfm.status_code = cpu_to_le16(status_code);
		tf->u.setup_cfm.dialog_token = dialog_token;
		break;
	case WLAN_TDLS_TEARDOWN:
		tf->category = ATBM_WLAN_CATEGORY_TDLS;
		tf->action_code = WLAN_TDLS_TEARDOWN;

		atbm_skb_put(skb, sizeof(tf->u.teardown));
		tf->u.teardown.reason_code = cpu_to_le16(status_code);
		break;
	case WLAN_TDLS_DISCOVERY_REQUEST:
		tf->category = ATBM_WLAN_CATEGORY_TDLS;
		tf->action_code = WLAN_TDLS_DISCOVERY_REQUEST;

		atbm_skb_put(skb, sizeof(tf->u.discover_req));
		tf->u.discover_req.dialog_token = dialog_token;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 1, 0))
struct tdls_discover_resp_s{
	u8 action_code;
	u8 dialog_token;
	__le16 capability;
	u8 variable[0];
} __packed tdls_discover_resp;
#endif
static int
ieee80211_prep_tdls_direct(struct wiphy *wiphy, struct net_device *dev,
			   u8 *peer, u8 action_code, u8 dialog_token,
			   u16 status_code, struct sk_buff *skb)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct atbm_ieee80211_mgmt *mgmt;
	#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 1, 0))
	struct tdls_discover_resp_s *ptdls_discover_resp;
	#endif
	mgmt = (void *)atbm_skb_put(skb, 24);
	memset(mgmt, 0, 24);
	memcpy(mgmt->da, peer, ETH_ALEN);
	memcpy(mgmt->sa, sdata->vif.addr, ETH_ALEN);
	memcpy(mgmt->bssid, sdata->u.mgd.bssid, ETH_ALEN);

	mgmt->frame_control = cpu_to_le16(IEEE80211_FTYPE_MGMT |
					  IEEE80211_STYPE_ACTION);

	switch (action_code) {
	#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 1, 0))
	case 14:
	#else
	case WLAN_PUB_ACTION_TDLS_DISCOVER_RES:
	#endif
		#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 1, 0))
		atbm_skb_put(skb, 1 + sizeof(struct tdls_discover_resp_s));
		#else
		atbm_skb_put(skb, 1 + sizeof(mgmt->u.action.u.tdls_discover_resp));
		#endif
		mgmt->u.action.category = ATBM_WLAN_CATEGORY_PUBLIC;
		#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 1, 0))
		ptdls_discover_resp = (struct tdls_discover_resp_s *)(((u8 *)(&mgmt->u.action.category))+1);
		ptdls_discover_resp->action_code = 14;
		ptdls_discover_resp->dialog_token = dialog_token;
		ptdls_discover_resp->capability = cpu_to_le16(ieee80211_get_tdls_sta_capab(sdata));
		#else
		mgmt->u.action.u.tdls_discover_resp.action_code =
			WLAN_PUB_ACTION_TDLS_DISCOVER_RES;
		mgmt->u.action.u.tdls_discover_resp.dialog_token =
			dialog_token;
		mgmt->u.action.u.tdls_discover_resp.capability =
			cpu_to_le16(ieee80211_get_tdls_sta_capab(sdata));
		#endif
		ieee80211_add_srates_ie(&sdata->vif, skb);
		ieee80211_add_ext_srates_ie(&sdata->vif, skb);
		ieee80211_tdls_add_ext_capab(skb);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}
static int ieee80211_tdls_mgmt(struct wiphy *wiphy, struct net_device *dev,
			       u8 *peer, u8 action_code, u8 dialog_token,
			       u16 status_code, const u8 *extra_ies,
			       size_t extra_ies_len)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_tx_info *info;
	struct sk_buff *skb = NULL;
	bool send_direct;
	int ret;

	if (!(wiphy->flags & WIPHY_FLAG_SUPPORTS_TDLS))
		return -ENOTSUPP;

	/* make sure we are in managed mode, and associated */
	if (sdata->vif.type != NL80211_IFTYPE_STATION ||
	    !sdata->u.mgd.associated)
		return -EINVAL;

#ifdef CONFIG_MAC80211_ATBM_VERBOSE_TDLS_DEBUG
	atbm_printk_cfg( "TDLS mgmt action %d peer %pM\n", action_code, peer);
#endif

	skb = atbm_dev_alloc_skb(local->hw.extra_tx_headroom +
			    max(sizeof(struct atbm_ieee80211_mgmt),
				sizeof(struct ieee80211_tdls_data)) +
			    50 + /* supported rates */
			    7 + /* ext capab */
			    extra_ies_len +
			    sizeof(struct ieee80211_tdls_lnkie));
	if (!skb)
		return -ENOMEM;

	info = IEEE80211_SKB_CB(skb);
	atbm_skb_reserve(skb, local->hw.extra_tx_headroom);

	switch (action_code) {
	case WLAN_TDLS_SETUP_REQUEST:
	case WLAN_TDLS_SETUP_RESPONSE:
	case WLAN_TDLS_SETUP_CONFIRM:
	case WLAN_TDLS_TEARDOWN:
	case WLAN_TDLS_DISCOVERY_REQUEST:
		ret = ieee80211_prep_tdls_encap_data(wiphy, dev, peer,
						     action_code, dialog_token,
						     status_code, skb);
		send_direct = false;
		break;
	#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 1, 0))
	case 14:
	#else
	case WLAN_PUB_ACTION_TDLS_DISCOVER_RES:
	#endif
		ret = ieee80211_prep_tdls_direct(wiphy, dev, peer, action_code,
						 dialog_token, status_code,
						 skb);
		send_direct = true;
		break;
	default:
		ret = -ENOTSUPP;
		break;
	}

	if (ret < 0)
		goto fail;

	if (extra_ies_len)
		memcpy(atbm_skb_put(skb, extra_ies_len), extra_ies, extra_ies_len);

	/* the TDLS link IE is always added last */
	switch (action_code) {
	case WLAN_TDLS_SETUP_REQUEST:
	case WLAN_TDLS_SETUP_CONFIRM:
	case WLAN_TDLS_TEARDOWN:
	case WLAN_TDLS_DISCOVERY_REQUEST:
		/* we are the initiator */
		ieee80211_tdls_add_link_ie(skb, sdata->vif.addr, peer,
					   sdata->u.mgd.bssid);
		break;
	case WLAN_TDLS_SETUP_RESPONSE:
	#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 1, 0))
	case 14:
	#else
	case WLAN_PUB_ACTION_TDLS_DISCOVER_RES:
	#endif
		/* we are the responder */
		ieee80211_tdls_add_link_ie(skb, peer, sdata->vif.addr,
					   sdata->u.mgd.bssid);
		break;
	default:
		ret = -ENOTSUPP;
		goto fail;
	}

	if (send_direct) {
		ieee80211_tx_skb(sdata, skb);
		return 0;
	}

	/*
	 * According to 802.11z: Setup req/resp are sent in AC_BK, otherwise
	 * we should default to AC_VI.
	 */
	switch (action_code) {
	case WLAN_TDLS_SETUP_REQUEST:
	case WLAN_TDLS_SETUP_RESPONSE:
		atbm_skb_set_queue_mapping(skb, IEEE80211_AC_BK);
		skb->priority = 2;
		break;
	default:
		atbm_skb_set_queue_mapping(skb, IEEE80211_AC_VI);
		skb->priority = 5;
		break;
	}

	/* disable bottom halves when entering the Tx path */
	local_bh_disable();
	ret = ieee80211_subif_start_xmit(skb, dev);
	local_bh_enable();

	return ret;

fail:
	atbm_dev_kfree_skb(skb);
	return ret;
}

static int ieee80211_tdls_oper(struct wiphy *wiphy, struct net_device *dev,
			       u8 *peer, enum nl80211_tdls_operation oper)
{
	struct sta_info *sta;
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	if (!(wiphy->flags & WIPHY_FLAG_SUPPORTS_TDLS))
		return -ENOTSUPP;

	if (sdata->vif.type != NL80211_IFTYPE_STATION)
		return -EINVAL;

#ifdef CONFIG_MAC80211_ATBM_VERBOSE_TDLS_DEBUG
	atbm_printk_cfg( "TDLS oper %d peer %pM\n", oper, peer);
#endif

	switch (oper) {
	case NL80211_TDLS_ENABLE_LINK:
		rcu_read_lock();
		sta = sta_info_get(sdata, peer);
		if (!sta) {
			rcu_read_unlock();
			return -ENOLINK;
		}

		set_sta_flag(sta, WLAN_STA_TDLS_PEER_AUTH);
		rcu_read_unlock();
		break;
	case NL80211_TDLS_DISABLE_LINK:
		return sta_info_destroy_addr(sdata, peer);
	case NL80211_TDLS_TEARDOWN:
	case NL80211_TDLS_SETUP:
	case NL80211_TDLS_DISCOVERY_REQ:
		/* We don't support in-driver setup/teardown/discovery */
		return -ENOTSUPP;
	default:
		return -ENOTSUPP;
	}

	return 0;
}
#endif
#endif
#if 0
static void ieee80211_set_data_filter(struct wiphy *wiphy,
				      struct net_device *dev,
				      void *data, int len)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	drv_set_data_filter(local, sdata, data, len);
}
#endif
#ifdef CONFIG_ATBM_RADAR_DETECT
#ifdef CONFIG_ATBM_5G_PRETEND_2G
#if (LINUX_VERSION_CODE > KERNEL_VERSION(3, 9, 0))
static int ieee80211_start_radar_detection(struct wiphy *wiphy,
					   struct net_device *dev,
					   struct cfg80211_chan_def *chandef
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 16, 0))
					   ,u32 cac_time_ms
#endif
					   )
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	int err = 0;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 16, 0))
	u32 cac_time_ms = 100;
#endif
	if(sdata->radar_required == true){
		atbm_printk_err("radar cac is running\n");
		return -EBUSY;
	}
	sdata->radar_required = true;
	sdata->dfs_cac_chan_def = *chandef;
	ieee80211_queue_delayed_work(&sdata->local->hw,
				     &sdata->dfs_cac_timer_work,
				     msecs_to_jiffies(cac_time_ms));
	return err;
}
#endif
#endif 
#endif
struct cfg80211_ops mac80211_config_ops = {
	.add_virtual_intf = ieee80211_add_iface,
	.del_virtual_intf = ieee80211_del_iface,
	.change_virtual_intf = ieee80211_change_iface,
	.add_key = ieee80211_add_key,
	.del_key = ieee80211_del_key,
	.get_key = ieee80211_get_key,
	.set_default_key = ieee80211_config_default_key,
	.set_default_mgmt_key = ieee80211_config_default_mgmt_key,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 4, 0))
	.add_beacon = ieee80211_add_beacon,
	.set_beacon = ieee80211_set_beacon,
	.del_beacon = ieee80211_del_beacon,
#else
	.start_ap = ieee80211_start_ap,
	.change_beacon = ieee80211_change_beacon,
	.stop_ap = ieee80211_del_beacon,
#endif
	.add_station = ieee80211_add_station,
	.del_station = ieee80211_del_station,
	.change_station = ieee80211_change_station,
	.get_station = ieee80211_get_station,
	.dump_station = ieee80211_dump_station,
#ifdef CONFIG_MAC80211_ATBM_MESH
	.add_mpath = ieee80211_add_mpath,
	.del_mpath = ieee80211_del_mpath,
	.change_mpath = ieee80211_change_mpath,
	.get_mpath = ieee80211_get_mpath,
	.dump_mpath = ieee80211_dump_mpath,
	.update_mesh_config = ieee80211_update_mesh_config,
	.get_mesh_config = ieee80211_get_mesh_config,
	.join_mesh = ieee80211_join_mesh,
	.leave_mesh = ieee80211_leave_mesh,
#endif
	.change_bss = ieee80211_change_bss,
	.set_txq_params = ieee80211_set_txq_params,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 6, 0))
	.set_channel = ieee80211_set_channel,
#else
	.set_monitor_channel = ieee80211_set_monitor_channel,
#endif
	.suspend = ieee80211_suspend,
	.resume = ieee80211_resume,
	.scan = ieee80211_scan,
#ifdef CONFIG_ATBM_SUPPORT_SCHED_SCAN
	.sched_scan_start = ieee80211_sched_scan_start,
	.sched_scan_stop = ieee80211_sched_scan_stop,
#endif
	.auth = ieee80211_auth,
	.assoc = ieee80211_assoc,
	.deauth = ieee80211_deauth,
	.disassoc = ieee80211_disassoc,
#ifdef CONFIG_ATBM_SUPPORT_IBSS
	.join_ibss = ieee80211_join_ibss,
	.leave_ibss = ieee80211_leave_ibss,
#endif
	.set_wiphy_params = ieee80211_set_wiphy_params,
#ifdef CONFIG_ATBM_SUPPORT_WDS
	.set_wds_peer = ieee80211_set_wds_peer,
#endif
	CFG80211_TESTMODE_CMD(ieee80211_altmtest_cmd)
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 1, 0))
	CFG80211_TESTMODE_DUMP(ieee80211_testmode_dump)
#endif
#ifdef CONFIG_ATBM_SMPS
	.set_power_mgmt = ieee80211_set_power_mgmt,
#endif
#ifndef CONFIG_RATE_HW_CONTROL
	.set_bitrate_mask = ieee80211_set_bitrate_mask,
#endif
#ifdef CONFIG_ATBM_SUPPORT_P2P
	.remain_on_channel = ieee80211_remain_on_channel,
	.cancel_remain_on_channel = ieee80211_cancel_remain_on_channel,
	.mgmt_tx = ieee80211_mgmt_tx,
	.mgmt_tx_cancel_wait = ieee80211_mgmt_tx_cancel_wait,
#endif
#ifdef CONFIG_ATBM_SUPPORT_RCPI_RSSI
	.set_cqm_rssi_config = ieee80211_set_cqm_rssi_config,
#endif
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
	.mgmt_frame_register = ieee80211_mgmt_frame_register,
#endif
//#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 25))
#ifdef ATBM_POWERSAVE_SAVE_GROUP_KEY
#if (LINUX_VERSION_CODE > KERNEL_VERSION(3, 0, 8))
	.set_rekey_data = ieee80211_set_rekey_data,
#endif
#endif
#ifdef ATBM_SURPORT_TDLS
	#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 16, 0))&&(LINUX_VERSION_CODE > KERNEL_VERSION(3, 0, 8))
	.tdls_oper = ieee80211_tdls_oper,
	.tdls_mgmt = ieee80211_tdls_mgmt,
#endif
#endif
#ifdef CONFIG_ATBM_RADAR_DETECT
#ifdef CONFIG_ATBM_5G_PRETEND_2G
#if (LINUX_VERSION_CODE > KERNEL_VERSION(3, 9, 0))
	.start_radar_detection = ieee80211_start_radar_detection,
#endif
#endif
#endif

#ifdef CONFIG_ATBM_MAC80211_NO_USE 
	.dump_survey = ieee80211_dump_survey,
	.set_tx_power = ieee80211_set_tx_power,
	.get_tx_power = ieee80211_get_tx_power,
	.rfkill_poll = ieee80211_rfkill_poll,
	.set_antenna = ieee80211_set_antenna,
	.get_antenna = ieee80211_get_antenna,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,18,00))
	.set_ringparam = ieee80211_set_ringparam,
	.get_ringparam = ieee80211_get_ringparam,
#endif
#endif
};
