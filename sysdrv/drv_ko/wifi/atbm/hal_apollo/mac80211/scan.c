/*
 * Scanning implementation
 *
 * Copyright 2003, Jouni Malinen <jkmaline@cc.hut.fi>
 * Copyright 2004, Instant802 Networks, Inc.
 * Copyright 2005, Devicescape Software, Inc.
 * Copyright 2006-2007	Jiri Benc <jbenc@suse.cz>
 * Copyright 2007, Michael Wu <flamingice@sourmilk.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/if_arp.h>
#include <linux/rtnetlink.h>
#ifdef CONFIG_ATBM_PM_QOS
#include <linux/pm_qos.h>
#endif
#include <net/sch_generic.h>
#include <linux/slab.h>
#include <linux/export.h>
#include <net/atbm_mac80211.h>
#ifdef ATBM_CHANGE_AP_TIMESTAMP_TO_BOOTTIME
#include <linux/ktime.h>
#include <linux/hrtimer.h>
#endif
#include <linux/hash.h>
#include <linux/sched.h>

#include "ieee80211_i.h"
#include "driver-ops.h"
#include "mesh.h"


#define IEEE80211_PROBE_DELAY (HZ / 33)
#define IEEE80211_CHANNEL_TIME (HZ / 33)
#define IEEE80211_PASSIVE_CHANNEL_TIME (HZ / 8)
#define IEEE80211_SCAN_SPIT_TP_500K		500*1024
#define IEEE80211_SCAN_SPIT_TP_1M		1000*1024
#define IEEE80211_SCAN_SPIT_TP_5M		5000*1024
#define IEEE80211_SCAN_SPIT_TP_10M		10000*1024
#define IEEE80211_SCAN_SPIT_TP_20M		20000*1024
#define IEEE80211_SCAN_SPIT_TP_30M		30000*1024
#define IEEE80211_SCAN_SPIT_TP_40M		40000*1024

static  int ieee80211_scan_send_probe_req(struct ieee80211_local *local,
			      struct ieee80211_sub_if_data *sdata,
			      struct ieee80211_scan_req_wrap *req)
{
	struct sk_buff *skb = NULL;

	/*
	*receive other bss
	*/
	atbm_printk_debug("%s\n",__func__);
	
	if(sdata->vif.type != NL80211_IFTYPE_STATION){
		return -1;
	}

	if(!(req->flags & IEEE80211_SCAN_REQ_NEED_LISTEN)){
		sdata->req_filt_flags |= FIF_OTHER_BSS;
		sdata->flags |= IEEE80211_SDATA_ALLMULTI;
		ieee80211_configure_filter(sdata);
		atbm_printk_debug("%s:filter\n",__func__);
	} else {
#ifndef CONFIG_ATBM_STA_LISTEN
		return -1;
#else
		atbm_printk_debug("%s:listen\n",__func__);
		if(local->ops->sta_triger_listen)
			local->ops->sta_triger_listen(&local->hw,&sdata->vif,req->req->channels[0]);
		else 
			return -1;
#endif
	}
	skb = ieee80211_probereq_get(&local->hw, &sdata->vif, NULL, 0,
								req->req->ie, req->req->ie_len,
								req->flags & IEEE80211_SCAN_REQ_NEED_BSSID ? req->bssid:NULL);

	if(skb){
		struct ieee80211_tx_info *info;

		info = IEEE80211_SKB_CB(skb);

		info->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT;
		info->flags |= IEEE80211_TX_CTL_NO_ACK;
		info->band = req->req->channels[0]->band;
		info->control.vif = &sdata->vif;
		info->flags |= IEEE80211_TX_CTL_CLEAR_PS_FILT |
				IEEE80211_TX_CTL_ASSIGN_SEQ |
				IEEE80211_TX_CTL_FIRST_FRAGMENT |
				IEEE80211_TX_CTL_RATE_CTRL_PROBE;
		if(sdata->vif.p2p == true)
			info->flags |= IEEE80211_TX_CTL_NO_CCK_RATE;
		ieee80211_tx_skb(sdata, skb);
	}

	local->scan_idle_time = 300*req->req->n_channels > 10000 ? 10000 : 100*req->req->n_channels;
	
	ieee80211_scan_completed(&local->hw,0);

	return 0;
}
#ifndef  CONFIG_ATBM_5G_PRETEND_2G
#ifndef CONFIG_CFG80211_INTERNAL_REGDB
struct country_chan{
	  	char *country;
	  	u8 chan;
 };
struct country_chan country_t[]={
	  {"CN",13},
	  {"JP",14},
	  {"US",11},
	  {"EU",13},
	  {NULL,0}
};
#endif
#endif
/*
	return 
		0  : no limit
		>0 : limit
		<0 : limit,not allow scan
*/
static int ieee80211_check_country_limit_scan_2_4G_chan(struct ieee80211_local *local,struct cfg80211_scan_request *req)
{
#ifdef  CONFIG_ATBM_5G_PRETEND_2G
	return 0;
#else
  	int country_chan = 0,freq,scan_chan,scan_n_chans;
 
  	int i = 0,j = 0,k = 0;
	void *pptr[32];
#ifdef CONFIG_CFG80211_INTERNAL_REGDB
	country_chan = local->country_support_chan;
#else
	/*
		?D??¨º?????1¨²?¨°W
	*/
  	do{
	  	if(memcmp(local->country_code,country_t[i].country,2) == 0){
		  	country_chan = country_t[i].chan;
			break;
	  	}
	  	i++;
  	}while(country_t[i].country);
#endif
	
	
	if((country_chan > 0) && (req != NULL)){
		atbm_printk_err("%s : scan_n_channals = %d ++++++++ \n",__func__,req->n_channels);
	/*
		1?¡éD¨¨¨°a¨¨¡¤?¡§¨¦¡§?¨¨¦Ì?D?¦Ì¨¤??¨ºy
		2?¡éD¨¨¨°a¨¨¡¤¨¨?¨¦¡§?¨¨¦Ì?D?¦Ì¨¤¨º?¡¤??¨²?T??¡¤??¡ì?¨²¡ê?3?3?¡¤??¡ì?¨ª2?¨¦¡§?¨¨
		3?¡é5G D?¦Ì¨¤???¡ä?¨®¡ê??¨¨2???
		?¨´?Yn_channals ??¨¨?¦Ì?¨º¦Ì?¨ºchannal freq
		freq = channel_center_freq(req->channels[i])
		ieee80211_frequency_to_channel(freq) == 0 vail channel
	*/
		scan_n_chans = req->n_channels;
		for(j = 0; j < req->n_channels ; j++){
			if(req->channels[j] == NULL){
				atbm_printk_err("%s : channals is NULL ! not allow scan\n",__func__);
				return -1;
			}
			freq = channel_center_freq(req->channels[j]);
			scan_chan = ieee80211_frequency_to_channel(freq);//1~
			/*
				?D??¨º?¡¤?3?3?¨¢??T??¦Ì?D?¦Ì¨¤
			*/
			if((scan_chan != 0) && (scan_chan <= country_chan)){
				pptr[k++] = req->channels[j];// ?¨ºD¨ª¨¦¡§?¨¨¦Ì?D?¦Ì¨¤
				continue;
			}else{
				scan_n_chans--;
			}
		}
		/*
			¨¦?3y3?3?1¨²?¨°?T??¨¦¡§?¨¨¦Ì?D?¦Ì¨¤
		*/
		if(scan_n_chans <= 0){
			atbm_printk_err("%s : not allow scan ! \n",__func__);
			return -1;
		}
		if(scan_n_chans < req->n_channels){
			/*
				??3y¦Ì??¨´¨®D¦Ì?channals
				¨®DD?¨º?2?D¨¨¨°a¦Ì?
			*/
			for(j = 0; j < req->n_channels ; j++){
				req->channels[j] = NULL;
			}
			/*
				???¨²?T??¡¤??¡ì?¨²¨¦¡§?¨¨¦Ì?D?¦Ì¨¤??D??3?¦Ì
			*/
			for(j = 0; j < scan_n_chans ; j++){
				req->channels[j] = pptr[j];
			}
			req->n_channels = scan_n_chans;	
		}
		atbm_printk_err("%s : scan_n_channals = %d --------- \n",__func__,req->n_channels);
	}else{
		atbm_printk_err("%s : current country = %c%c , support chan=%d \n",__func__,
			local->country_code[0],local->country_code[1],local->country_support_chan);
	}

  	return country_chan;  
#endif
}
		
static int ieee80211_2_4G_scan_results_limit(struct ieee80211_local *local,
														struct sk_buff *skb,
														struct ieee80211_channel *channel)
{
#ifdef  CONFIG_ATBM_5G_PRETEND_2G
	return 0;
#else
	int freq,curr_chan,country_chan = 0,fc;
	struct atbm_ieee80211_mgmt *mgmt;
	bool presp, beacon = false;
	size_t baselen;
	u8 *elements;
	struct ieee802_atbm_11_elems elems;
	struct ieee80211_rx_status *rx_status = NULL;//IEEE80211_SKB_RXCB(skb);

#if 0	  
	  do{
	  	if(memcmp(local->country_code,country_t[i].country,2) == 0){
		  	country_chan = country_t[i].chan;
			break;
	  	}
	  	i++;
  	}while(country_t[i].country);
#endif

	country_chan = local->country_support_chan;
	
	if((country_chan > 0) && (skb != NULL)){
		rx_status = IEEE80211_SKB_RXCB(skb);
		mgmt = (struct atbm_ieee80211_mgmt *) skb->data;
		fc = mgmt->frame_control;
		presp = ieee80211_is_probe_resp(fc);
		if (presp) {
			/* ignore ProbeResp to foreign address */
			
			elements = mgmt->u.probe_resp.variable;
			baselen = offsetof(struct atbm_ieee80211_mgmt, u.probe_resp.variable);
		} else {
			beacon = ieee80211_is_beacon(fc);
			baselen = offsetof(struct atbm_ieee80211_mgmt, u.beacon.variable);
			elements = mgmt->u.beacon.variable;
		}
		if (!presp && !beacon){
			return 0;
		}
		ieee802_11_parse_elems(elements, skb->len - baselen, &elems);
		if (elems.ds_params && elems.ds_params_len == 1)
			freq = ieee80211_channel_to_frequency(elems.ds_params[0],
							      rx_status->band);
		else
			freq = rx_status->freq;
		curr_chan = ieee80211_frequency_to_channel(freq);
		//atbm_printk_err("skb : country_chan = %d curr_chan = %d\n",country_chan,curr_chan);
		if(curr_chan > country_chan){
			atbm_printk_err("%s : current scan results channals = %d,limit channel=%d ++++++++ \n",
					__func__,curr_chan,country_chan);
			return -1;
		}
	
	}else if((country_chan > 0) && (channel != NULL)){
		
		freq = channel_center_freq(channel);
		curr_chan = ieee80211_frequency_to_channel(freq);
		//atbm_printk_err("channel : country_chan = %d curr_chan = %d\n",country_chan,curr_chan);
		if(curr_chan > country_chan){
			atbm_printk_err("%s : current scan results channals = %d,limit channel=%d ++++++++ \n",
					__func__,curr_chan,country_chan);
			return -1;
		}

	}else{
		atbm_printk_err("%s : current country = %c%c , support chan=%d \n",__func__,
			local->country_code[0],local->country_code[1],local->country_support_chan);
	}
	return 0;
#endif
}
		  
static  int ieee80211_drv_hw_scan(struct ieee80211_local *local,
			      struct ieee80211_sub_if_data *sdata,
			      struct ieee80211_scan_req_wrap *req)
{
	int ret = 0;
	
	if(req->flags & IEEE80211_SCAN_REQ_ONLY_PROB){
		ret = ieee80211_scan_send_probe_req(local,sdata,req);
	}else{
		ret = drv_hw_scan(local,sdata,req);
	}

	return ret;
}
static void ieee80211_scan_try_split(struct ieee80211_local *local)
{
	struct cfg80211_scan_request *req = local->scan_req;
	struct ieee80211_sub_if_data *sdata;
	unsigned long total_rx_tp = 0;
	unsigned long total_tx_tp = 0;
	unsigned long total_tp = 0;
	bool	should_split = false;
	
	rcu_read_lock();
	list_for_each_entry_rcu(sdata, &local->interfaces, list)
		if(ieee80211_sdata_running(sdata)){
			total_rx_tp += sdata->traffic.current_rx_tp;
			total_tx_tp += sdata->traffic.current_tx_tp;
			if ((sdata->vif.type == NL80211_IFTYPE_AP)||(sdata->vif.type == NL80211_IFTYPE_P2P_GO)){
				should_split = true;
			}
		}
	rcu_read_unlock();

	total_tp = max(total_rx_tp,total_tx_tp);
	WARN_ON(local->scan_n_channels > req->n_channels);
	atbm_printk_debug("%s:tp(%ld)(%d)\n",__func__,total_tp,req->n_channels);
	if(total_tp <= IEEE80211_SCAN_SPIT_TP_500K){
		local->scan_idle_time = 0;
		local->scan_channel_space = req->n_channels-local->scan_n_channels;
	}else if(total_tp <= IEEE80211_SCAN_SPIT_TP_1M){
		local->scan_idle_time = 100;
		local->scan_channel_space = 10;
	}else if(total_tp <= IEEE80211_SCAN_SPIT_TP_5M){
		local->scan_idle_time = 100;
		local->scan_channel_space = 6;
	}else if(total_tp <= IEEE80211_SCAN_SPIT_TP_10M){
		local->scan_idle_time = 200;
		local->scan_channel_space = 2;
	}else if(total_tp <= IEEE80211_SCAN_SPIT_TP_20M){
		local->scan_idle_time = 350;
		local->scan_channel_space = 2;
	}else if(total_tp <= IEEE80211_SCAN_SPIT_TP_30M){
		local->scan_idle_time = 400;
		local->scan_channel_space = 1;
	}else {
		local->scan_idle_time = 450;
		local->scan_channel_space = 1;
	}

	if(local->scan_channel_space >= req->n_channels-local->scan_n_channels){
		local->scan_idle_time = 0;
		local->scan_channel_space = req->n_channels-local->scan_n_channels;
	}

	if((should_split == true) && (local->scan_channel_space > 1)){
		local->scan_channel_space = 1;
		local->scan_idle_time     = 80;
	}
	atbm_printk_debug("%s:space(%d),idle(%ld)\n",__func__,local->scan_channel_space,local->scan_idle_time);
}
struct ieee80211_bss *
ieee80211_rx_bss_get(struct ieee80211_local *local, u8 *bssid, int freq,
		     u8 *ssid, u8 ssid_len)
{
	struct cfg80211_bss *cbss;

	cbss = ieee80211_atbm_get_bss(local->hw.wiphy,
				ieee80211_get_channel(local->hw.wiphy, freq),
				bssid, ssid, ssid_len, 0, 0);
	if (!cbss)
		return NULL;
	return (void *)cbss->priv;
}
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 0))
static void ieee80211_rx_bss_free(struct cfg80211_bss *cbss)
{
	struct ieee80211_bss *bss = (void *)cbss->priv;

	atbm_kfree(bss_mesh_id(bss));
	atbm_kfree(bss_mesh_cfg(bss));
}
#endif
void ieee80211_rx_bss_put(struct ieee80211_local *local,
			  struct ieee80211_bss *bss)
{
	if (!bss)
		return;
	ieee80211_atbm_put_bss(local->hw.wiphy,container_of((void *)bss, struct cfg80211_bss, priv));
}

static bool is_uapsd_supported(struct ieee802_atbm_11_elems *elems)
{
	u8 qos_info;

	if (elems->wmm_info && elems->wmm_info_len == 7
	    && elems->wmm_info[5] == 1)
		qos_info = elems->wmm_info[6];
	else if (elems->wmm_param && elems->wmm_param_len == 24
		 && elems->wmm_param[5] == 1)
		qos_info = elems->wmm_param[6];
	else
		/* no valid wmm information or parameter element found */
		return false;

	return qos_info & IEEE80211_WMM_IE_AP_QOSINFO_UAPSD;
}

struct ieee80211_bss *
ieee80211_bss_info_update(struct ieee80211_local *local,
			  struct ieee80211_rx_status *rx_status,
			  struct atbm_ieee80211_mgmt *mgmt,
			  size_t len,
			  struct ieee802_atbm_11_elems *elems,
			  struct ieee80211_channel *channel,
			  bool beacon)
{
	#ifdef ATBM_CHANGE_AP_TIMESTAMP_TO_BOOTTIME
	/*
	*Fix android 7.0 bug!!!!!
	*In the framework,when it starts to scan,it will record a starttime(signed 64 bit boottime) which will be used at can end.
	*after scan end,the framework will process probe response/beacon, and compare tsf in the probe response/beacon
	*with the scan starttime, and the tsf must be greater than the scan starttime. so here change the tsf
	*to the max signed inter value(0x7fffffffffffffff).
	*/
	#define ATBM_BOOT_TIME 0x7fffffffffffffffLL//((u64)(ktime_to_ns(ktime_get_boottime())>>10))
	#endif
	
	struct cfg80211_bss *cbss;
	struct ieee80211_bss *bss;
	int clen, srlen;
	s32 signal = 0;

	if (local->hw.flags & IEEE80211_HW_SIGNAL_DBM)
		signal = rx_status->signal * 100;
	else if (local->hw.flags & IEEE80211_HW_SIGNAL_UNSPEC)
		signal = (rx_status->signal * 100) / local->hw.max_signal;
	#ifdef ATBM_CHANGE_AP_TIMESTAMP_TO_BOOTTIME
	{
		__le16 fc = mgmt->frame_control;
		if(ieee80211_is_probe_resp(fc)){
			mgmt->u.probe_resp.timestamp = cpu_to_le64(ATBM_BOOT_TIME);
		}
		else if(ieee80211_is_beacon(fc)){
			mgmt->u.beacon.timestamp = cpu_to_le64(ATBM_BOOT_TIME);
		}else{
			WARN_ON(1);
		}
	}
	#undef ATBM_BOOT_TIME
	#endif
	
	if(ieee80211_2_4G_scan_results_limit(local,NULL,channel) < 0)
		return NULL;

	cbss = cfg80211_inform_bss_frame(local->hw.wiphy, channel,
					 (struct ieee80211_mgmt*)mgmt, len, signal, GFP_ATOMIC);

	if (!cbss)
		return NULL;
	#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 0))
	cbss->free_priv = ieee80211_rx_bss_free;
	#endif
	bss = (void *)cbss->priv;

	/* save the ERP value so that it is available at association time */
	if (elems->erp_info && elems->erp_info_len >= 1) {
		bss->erp_value = elems->erp_info[0];
		bss->has_erp_value = 1;
	}

	if (elems->tim) {
		struct ieee80211_tim_ie *tim_ie =
			(struct ieee80211_tim_ie *)elems->tim;
		bss->dtim_period = tim_ie->dtim_period;
	}

	/* If the beacon had no TIM IE, or it was invalid, use 1 */
	if (beacon && !bss->dtim_period)
		bss->dtim_period = 1;

	/* replace old supported rates if we get new values */
	srlen = 0;
	if (elems->supp_rates) {
		clen = IEEE80211_MAX_SUPP_RATES;
		if (clen > elems->supp_rates_len)
			clen = elems->supp_rates_len;
		memcpy(bss->supp_rates, elems->supp_rates, clen);
		srlen += clen;
	}
	if (elems->ext_supp_rates) {
		clen = IEEE80211_MAX_SUPP_RATES - srlen;
		if (clen > elems->ext_supp_rates_len)
			clen = elems->ext_supp_rates_len;
		memcpy(bss->supp_rates + srlen, elems->ext_supp_rates, clen);
		srlen += clen;
	}
	if (srlen)
		bss->supp_rates_len = srlen;

	bss->wmm_used = elems->wmm_param || elems->wmm_info;
	bss->uapsd_supported = is_uapsd_supported(elems);
#ifdef SIGMSTAR_SCAN_FEATURE
	bss->noise_floor	 = local->noise_floor[(channel_hw_value(channel)-1)%13];//-92;
	if(bss->noise_floor > -80)
		bss->noise_floor = -80;
#endif  //#ifdef SIGMSTAR_SCAN_FEATURE
	if (!beacon)
		bss->last_probe_resp = jiffies;

	return bss;
}
bool  ieee80211_scan_internal_req_results(struct ieee80211_local *local,struct atbm_internal_scan_results_req *req)
{
	struct hlist_head *hlist;
	struct hlist_node *node;
	struct hlist_node *node_temp;
	struct atbm_internal_scan_sta_node *sta_node;
	struct ieee80211_internal_scan_sta sta;
	int hash_index = 0;
	bool ret = true;

	spin_lock_bh(&local->internal_scan_list_lock);
	for(hash_index = 0;hash_index<ATBM_COMMON_HASHENTRIES;hash_index++){
		hlist = &local->internal_scan_list[hash_index];
		hlist_for_each_safe(node,node_temp,hlist){
			
			sta_node = hlist_entry(node,struct atbm_internal_scan_sta_node,hnode);
			
			if(req->flush == true){
				local->internal_scan_n_results--;
				hlist_del(&sta_node->hnode);
			}
			
			memcpy(&sta,&sta_node->sta,sizeof(struct ieee80211_internal_scan_sta));
		//	spin_unlock_bh(&local->internal_scan_list_lock);
			if(ret == true){
				if(req->result_handle)
					ret = req->result_handle(&local->hw,req,&sta);
			}
		//	spin_lock_bh(&local->internal_scan_list_lock);
			
			if(req->flush == true){
				if(sta_node->sta.ie){
					atbm_kfree(sta_node->sta.ie);
					sta_node->sta.ie = NULL;
					sta_node->sta.ie_len = 0;
				}
				atbm_kfree(sta_node);
			}
		}
	}
	spin_unlock_bh(&local->internal_scan_list_lock);

	return ret;
}
static void  ieee80211_scan_internal_list_flush(struct ieee80211_local *local)
{
	struct hlist_head *hlist;
	struct hlist_node *node;
	struct hlist_node *node_temp;
	struct atbm_internal_scan_sta_node *sta_node;
	int hash_index = 0;

	spin_lock_bh(&local->internal_scan_list_lock);
	for(hash_index = 0;hash_index<ATBM_COMMON_HASHENTRIES;hash_index++){
		hlist = &local->internal_scan_list[hash_index];
		hlist_for_each_safe(node,node_temp,hlist){
			sta_node = hlist_entry(node,struct atbm_internal_scan_sta_node,hnode);
			local->internal_scan_n_results--;
			hlist_del(&sta_node->hnode);
			if(sta_node->sta.ie){
				atbm_kfree(sta_node->sta.ie);
				sta_node->sta.ie = NULL;
				sta_node->sta.ie_len = 0;
			}
			atbm_kfree(sta_node);
		}
	}
	spin_unlock_bh(&local->internal_scan_list_lock);
}

void ieee80211_scan_internal_deinit(struct ieee80211_local *local)
{
	u8 *ie = NULL;
	
	ieee80211_scan_internal_list_flush(local);

	mutex_lock(&local->mtx);
	
	ie = rcu_dereference(local->internal_scan_ie);
	rcu_assign_pointer(local->internal_scan_ie,NULL);
	local->internal_scan_ie_len = 0;
	synchronize_rcu();

	if(ie)
		atbm_kfree(ie);
	mutex_unlock(&local->mtx);
}

void ieee80211_scan_internal_int(struct ieee80211_local *local)
{
	/*****internal scan init **********/
	spin_lock_init(&local->internal_scan_list_lock);
	init_waitqueue_head(&local->internal_scan_wq);
	atomic_set(&local->internal_scan_status,IEEE80211_INTERNAL_SCAN_STATUS__IDLE);
	spin_lock_bh(&local->internal_scan_list_lock);
	atbm_common_hash_list_init(local->internal_scan_list,ATBM_COMMON_HASHENTRIES);
	spin_unlock_bh(&local->internal_scan_list_lock);
}
static void ieee80211_scan_rx_internal_update(struct ieee80211_local *local,
															 struct ieee80211_internal_scan_result *result)
{
	int hash_index = 0;
	struct hlist_head *hlist;
	struct hlist_node *node;
	struct atbm_internal_scan_sta_node *sta_node;
	struct atbm_internal_scan_sta_node *sta_node_target = NULL;
	
	atbm_printk_debug("%s:ssid[%s],mac[%pM],channel[%d],signal[%d],beacon[%d]\n",__func__,result->sta.ssid,result->sta.bssid,result->sta.channel,result->sta.signal,result->sta.beacon);
	spin_lock_bh(&local->internal_scan_list_lock);
	/*
	*find target sta from hash list
	*/
	if(result->sta.ssid_len)
		hash_index = atbm_hash_index(result->sta.ssid,result->sta.ssid_len,ATBM_COMMON_HASHBITS);
	else
		hash_index = 0;

	hlist = &local->internal_scan_list[hash_index];

	hlist_for_each(node,hlist){
		sta_node = hlist_entry(node,struct atbm_internal_scan_sta_node,hnode);
		if ((result->sta.ssid_len == sta_node->sta.ssid_len)&&(!memcmp(result->sta.bssid,sta_node->sta.bssid,6))){
			/*
			*hidden ssid
			*/
			if((result->sta.ssid_len == 0) || (!memcmp(sta_node->sta.ssid,result->sta.ssid,sta_node->sta.ssid_len)))
				sta_node_target = sta_node;
			break;
		}
	}
	/*
	*insert new sta to hash list
	*/
	if(sta_node_target == NULL){
		sta_node_target = atbm_kzalloc(sizeof(struct atbm_internal_scan_sta_node),GFP_ATOMIC);

		if(sta_node_target == NULL){
			spin_unlock_bh(&local->internal_scan_list_lock);
			return;
		}
		local->internal_scan_n_results++;
		hlist_add_head(&sta_node_target->hnode,hlist);
	}else {
		
		if((result->sta.ie_len != sta_node_target->sta.ie_len)){
			if((result->sta.ie_len == 0)){

				/*
						recv null special ie,not fresh
				*/

				atbm_printk_debug("%s:ie(%s)\n",__func__,sta_node_target->sta.ie);
				spin_unlock_bh(&local->internal_scan_list_lock);
				return;	
			}
		}	
		
		/*
		* only save the new special ie so need free old special ie
		*/
		
		if(sta_node_target->sta.ie){
			atbm_kfree(sta_node_target->sta.ie);
			sta_node_target->sta.ie = NULL;
			sta_node_target->sta.ie_len = 0;
		}

	
			
		
	}
	/*
	*update sta infor
	*/
	if(sta_node_target == NULL){
		atbm_printk_err("%s %d ,ERROR !!! sta_node_target is NULL\n",__func__,__LINE__);
		spin_unlock_bh(&local->internal_scan_list_lock);
		return;
	}
	memcpy(&sta_node_target->sta,&result->sta,sizeof(struct ieee80211_internal_scan_sta));
	sta_node_target->sta.ssid_len = result->sta.ssid_len;
	if(result->sta.ssid_len)
		memcpy(sta_node_target->sta.ssid,result->sta.ssid,result->sta.ssid_len);
	if(sta_node_target->sta.ie)
		atbm_printk_debug("%s:ie(%s)\n",__func__,sta_node_target->sta.ie);
	spin_unlock_bh(&local->internal_scan_list_lock);

}
bool ieee80211_scan_rx_internal_default(struct ieee80211_sub_if_data *sdata,
													   struct ieee80211_internal_scan_result *result,bool finish)
{
	struct ieee80211_local *local = sdata->local;

	if(finish == false){
		ieee80211_scan_rx_internal_update(local,result);
	}else {
	}

	return true;
}
void ieee80211_scan_cca_notify(struct ieee80211_hw *hw,struct ieee80211_internal_scan_notity *notify)
{
	struct ieee80211_local *local = hw_to_local(hw);
	struct ieee80211_scan_req_wrap *req_wrap = &local->scan_req_wrap;
	struct cfg80211_scan_request *req = req_wrap->req;
	u8 index;
	u8 len = notify->cca.val_len;
	u8* cca_val = notify->cca.val;
	/*
	*cca only can run in internal scan mode
	*/
	atbm_printk_debug("%s\n",__func__);
	WARN_ON(test_bit(SCAN_INTERNAL_SCANNING, &local->scanning) == 0);
	WARN_ON((req_wrap->flags & IEEE80211_SCAN_REQ_CCA) == 0);

	if(req == NULL){
		WARN_ON(1);
		return;
	}

	if(len > IEEE80211_ATBM_MAX_SCAN_CHANNEL_INDEX){
		WARN_ON(1);
		return;
	}
	
	for(index = 0;index<req->n_channels;index++){
		
		if(index>len){
			WARN_ON(1);
			break;
		}
		req_wrap->cca_val[channel_hw_value(req->channels[index])-1] = cca_val[index];
		
	}
}
void ieee80211_scan_cca_val_put(struct ieee80211_hw *hw)
{
	struct ieee80211_local *local = hw_to_local(hw);
	struct ieee80211_scan_req_wrap *req_wrap = &local->scan_req_wrap;

	memset(req_wrap->cca_val,0,IEEE80211_ATBM_MAX_SCAN_CHANNEL_INDEX);
}

u8* ieee80211_scan_cca_val_get(struct ieee80211_hw *hw)
{
	struct ieee80211_local *local = hw_to_local(hw);
	struct ieee80211_scan_req_wrap *req_wrap = &local->scan_req_wrap;


	return req_wrap->cca_val;
}
/*
#define ATBM_RSN_CIPHER_SUITE_NONE 					0x000fac00
#define ATBM_RSN_CIPHER_SUITE_WEP40					0x000fac01
#define ATBM_RSN_CIPHER_SUITE_TKIP 					0x000fac02
#define ATBM_RSN_CIPHER_SUITE_CCMP 					0x000fac04
#define ATBM_RSN_CIPHER_SUITE_WEP104 				0x000fac05
#define ATBM_RSN_CIPHER_SUITE_AES_128_CMAC 			0x000fac06
#define ATBM_RSN_CIPHER_SUITE_NO_GROUP_ADDRESSED 	0x000fac07
#define ATBM_RSN_CIPHER_SUITE_GCMP 					0x000fac08

struct atbm_wpa_ie_data {
	int proto;
	int pairwise_cipher; 
	int group_cipher;
	int key_mgmt;
	int wpa_capabilities;
	int rsn_capabilities;
	size_t num_pmkid;
	const u8 *pmkid;
	int mgmt_group_cipher;
};


*/

static int get_selector_to_bitfield(const u8 *s)
{
	
	if (ATBM_WPA_GET_BE32(s) == ATBM_RSN_CIPHER_SUITE_NONE)
		return ATBM_WPA_CIPHER_NONE;
	if (ATBM_WPA_GET_BE32(s) == ATBM_RSN_CIPHER_SUITE_WEP40)
		return ATBM_WPA_CIPHER_WEP40;
	if (ATBM_WPA_GET_BE32(s) == ATBM_RSN_CIPHER_SUITE_TKIP)
		return ATBM_WPA_CIPHER_TKIP;
	if (ATBM_WPA_GET_BE32(s) == ATBM_RSN_CIPHER_SUITE_CCMP)
		return ATBM_WPA_CIPHER_CCMP;
	if (ATBM_WPA_GET_BE32(s) == ATBM_RSN_CIPHER_SUITE_WEP104)
		return ATBM_WPA_CIPHER_WEP104;
	if (ATBM_WPA_GET_BE32(s) == ATBM_RSN_CIPHER_SUITE_AES_128_CMAC)
		return ATBM_WPA_CIPHER_AES_128_CMAC;
	if (ATBM_WPA_GET_BE32(s) == ATBM_RSN_CIPHER_SUITE_GCMP)
		return ATBM_WPA_CIPHER_GCMP;
	return 0;
}




int get_ap_encryption_algorithm(char *ie,int ie_len, u8 *encry_info,enum ieee80211_enc_type enc_type)
{
	char *pos = NULL;
	int encry_type_count = 0,alg_i = 0;
	int left = 0;
	int encry_flag = 0;
	


	switch(enc_type){
		case IEEE80211_ENC_OPEN:{
			encry_flag |= 1;
			}break;
		case IEEE80211_ENC_WPA:{
			/*get WPA encryption algorithm */
				//pos = ie;
				
				if(ie == NULL || ie_len <= 0){
					atbm_printk_scan("get_ap_encryption_algorithm : parameters err! %s,ie_len(%d) \n",ie == NULL?"ie is null":"",ie_len);
					return -1;
				}
				/* jump Version */
				pos = ie + 6;
				left = ie_len - 6;
				if(left >= 4){
					/*jump Multicast cipher OUI*/
					pos += 4;
					left -= 4;
				}else{
					atbm_printk_err("get_ap_encryption_algorithm : ie too short \n");
					break;
				}
				if(left >= 2){
				/* Number of Unicasts: */
					encry_type_count = ATBM_WPA_GET_LE16(pos);
					pos += 2;
					left -= 2;
					if ((encry_type_count == 0) || (left < encry_type_count * 4)) {
						atbm_printk_err("%s: ie count botch (pairwise), "
							   "count %u left %u", __func__, encry_type_count, left);
						break;
					}
					
					for(alg_i = 0;alg_i < encry_type_count ; alg_i++){
						if(left < 0){
							atbm_printk_err("%s ,out of range IEEE80211_ENC_WPA\n",__func__);
							break;
						}
						encry_flag |= get_selector_to_bitfield(pos);
						pos += 4;
						left -= 4;
					}
				}else{
					atbm_printk_err("get_ap_encryption_algorithm : ie too short \n");
				}
			}break;
		case IEEE80211_ENC_WPA2:{
			/*get WPA2 encryption algorithm */
			
			if(ie == NULL || ie_len <= 0){
				atbm_printk_scan("get_ap_encryption_algorithm IEEE80211_ENC_WPA2: parameters err! %s,ie_len(%d) \n",ie == NULL?"ie is null":"",ie_len);
				return -1;
			}
				/* jump Version */
				pos = ie + 2;
				left = ie_len - 2;
				if(left >= 4){
				/*jump	Group Cipher OUI / Group Cipher Type*/
					pos +=  4;
					left -=  4;
				}else{
					atbm_printk_err("get_ap_encryption_algorithm IEEE80211_ENC_WPA2 : ie too short \n");
					break;
				}
				if(left >= 2){
					/*Pairwise Cipher Count*/
					encry_type_count = ATBM_WPA_GET_LE16(pos);
					pos += 2;
					left -= 2;
					
					if ((encry_type_count == 0) || (left < encry_type_count * 4)) {
							atbm_printk_err("%s: IEEE80211_ENC_WPA2 ie count botch (pairwise), "
								   "count %u left %u", __func__, encry_type_count, left);
							break;
					}
					for(alg_i = 0;alg_i < encry_type_count ; alg_i++ ){
						if(left < 0){
							atbm_printk_err("%s IEEE80211_ENC_WPA2,out of range IEEE80211_ENC_WPA2\n",__func__);
							return -1;
						}
						encry_flag |= get_selector_to_bitfield(pos);
						pos += 4;
						left -= 4;
					}
				}
			}break;
		case IEEE80211_ENC_WEP:{
				encry_flag |= (1<<1);
				encry_flag |= (1<<2);
			}break;
		default:
			break;
	}


	*encry_info |= (encry_flag & 0xff);
	return 0;
}

static ieee80211_rx_result 
ieee80211_scan_rx_internal_sta_info(struct ieee80211_sub_if_data *sdata, struct sk_buff *skb)
{
	struct ieee80211_rx_status *rx_status = IEEE80211_SKB_RXCB(skb);
	struct atbm_ieee80211_mgmt *mgmt;
	u8 *elements;
	size_t baselen;
	int freq;
	__le16 fc;
	bool presp = false, beacon = false;
	struct ieee802_atbm_11_elems elems;
	bool (*func)(struct ieee80211_sub_if_data *sdata,void *fs_data,struct ieee80211_internal_scan_result *result,bool finish);
	void *scan_data;
	ieee80211_rx_result handle = RX_QUEUED;
	struct ieee80211_internal_scan_result scan_info;
	
	rcu_read_lock();
	
	memset(&scan_info,0,sizeof(struct ieee80211_internal_scan_result));
	func = rcu_dereference(sdata->local->internal_scan.req.result_handle);
	scan_data = rcu_dereference(sdata->local->internal_scan.req.priv);
	
	if (skb->len < 2){
		handle = RX_DROP_UNUSABLE;
		goto err;
	}

	mgmt = (struct atbm_ieee80211_mgmt *) skb->data;
	fc = mgmt->frame_control;

	if (ieee80211_is_ctl(fc)){
		handle = RX_CONTINUE;
		goto err;
	}
	if (skb->len < 24){
		handle = RX_CONTINUE;
		goto err;
	}

	presp = ieee80211_is_probe_resp(fc);
	if (presp) {
		/* ignore ProbeResp to foreign address */
		if (memcmp(mgmt->da, sdata->vif.addr, ETH_ALEN)){
			handle = RX_DROP_MONITOR;
			goto err;
		}

		presp = true;
		elements = mgmt->u.probe_resp.variable;
		baselen = offsetof(struct atbm_ieee80211_mgmt, u.probe_resp.variable);
	} else {
		beacon = ieee80211_is_beacon(fc);
		baselen = offsetof(struct atbm_ieee80211_mgmt, u.beacon.variable);
		elements = mgmt->u.beacon.variable;
	}
	if (!presp && !beacon){
		handle = RX_CONTINUE;
		goto err;
	}
	if (baselen > skb->len){
		handle = RX_DROP_MONITOR;
		goto err;
	}
	if(sdata->local->internal_scan.req.n_macs){
		struct ieee80211_internal_mac *sta_mac;
		struct hlist_head *hlist;
		struct hlist_node *node;
		u8 hash_index = 0;
		bool found = false;

		hash_index = atbm_hash_index(mgmt->bssid,6,IEEE80211_INTERNAL_SCAN_HASHBITS);
		hlist = &sdata->local->internal_scan.mac_hash_list[hash_index];

		hlist_for_each(node,hlist){
			sta_mac = hlist_entry(node,struct ieee80211_internal_mac,hnode);
			if (!memcmp(sta_mac->mac, mgmt->bssid, 6)){
				found = true;
				break;
			}
		}
		if(found == false){
			atbm_printk_debug("%s,mac[%pM] not in list\n",__func__,mgmt->bssid);
			handle = RX_DROP_MONITOR;
			goto err;
		}
	}

	
	memset(&elems,0,sizeof(struct ieee802_atbm_11_elems));
	ieee802_11_parse_elems(elements, skb->len - baselen, &elems);
	

	if (elems.ds_params && elems.ds_params_len == 1)
		freq = ieee80211_channel_to_frequency(elems.ds_params[0],
						      rx_status->band);
	else
		freq = rx_status->freq;
	
	scan_info.sta.beacon = beacon;
	
	if(elems.rsn && elems.rsn_len && elems.wpa && elems.wpa_len){
		scan_info.sta.enc_type = IEEE80211_ENC_WPA_WPA2;
		/*get WPA2 encryption algorithm */
		get_ap_encryption_algorithm(elems.rsn,elems.rsn_len,&elems.encry_info,IEEE80211_ENC_WPA2);
		/*get WPA encryption algorithm */
		get_ap_encryption_algorithm(elems.wpa,elems.wpa_len,&elems.encry_info,IEEE80211_ENC_WPA);
		
	}else if(elems.rsn && elems.rsn_len){
		scan_info.sta.enc_type = IEEE80211_ENC_WPA2;
		get_ap_encryption_algorithm(elems.rsn,elems.rsn_len,&elems.encry_info,IEEE80211_ENC_WPA2);
	}else if(elems.wpa && elems.wpa_len){
		scan_info.sta.enc_type = IEEE80211_ENC_WPA;
		get_ap_encryption_algorithm(elems.wpa,elems.wpa_len,&elems.encry_info,IEEE80211_ENC_WPA);
	}else if(((presp == true) && (mgmt->u.probe_resp.capab_info & ATBM_WLAN_CAPABILITY_PRIVACY)) ||
		    ((beacon == true)&& (mgmt->u.beacon.capab_info & ATBM_WLAN_CAPABILITY_PRIVACY))){
		scan_info.sta.enc_type = IEEE80211_ENC_WEP;
		get_ap_encryption_algorithm(elems.wpa,elems.wpa_len,&elems.encry_info,IEEE80211_ENC_WEP);
	}else {
		scan_info.sta.enc_type = IEEE80211_ENC_OPEN;
		get_ap_encryption_algorithm(elems.wpa,elems.wpa_len,&elems.encry_info,IEEE80211_ENC_OPEN);
	}
	
	scan_info.sta.ieee80211_enc_type_name = elems.encry_info;
	
	memcpy(scan_info.sta.bssid,mgmt->bssid,ETH_ALEN);
	scan_info.sta.ssid_len = elems.ssid_len;
	if(elems.ssid_len>0)
		memcpy(scan_info.sta.ssid,elems.ssid,elems.ssid_len);

	if(ieee80211_get_channel(sdata->local->hw.wiphy,freq))
		scan_info.sta.channel = channel_hw_value(ieee80211_get_channel(sdata->local->hw.wiphy,freq));
	else{
		atbm_printk_err("%s %d  freq = %d channel_ptr = NULL\n",__func__,__LINE__,freq);
		handle = RX_DROP_MONITOR;
		goto err;
	}
	scan_info.sta.signal = rx_status->signal;

	if(elems.atbm_special_len && elems.atbm_special){
		scan_info.sta.ie = atbm_kzalloc(elems.atbm_special_len,GFP_ATOMIC);

		if(scan_info.sta.ie == NULL){
			handle = RX_DROP_MONITOR;
			goto err;
		}
		memcpy(scan_info.sta.ie,elems.atbm_special,elems.atbm_special_len);
		scan_info.sta.ie_len = elems.atbm_special_len;
	}
	if(sdata->local->scan_req_wrap.flags & IEEE80211_SCAN_REQ_CCA){
		struct ieee80211_channel *channel = ieee80211_get_channel(sdata->local->hw.wiphy,freq);

		if(channel == NULL){
			handle = RX_DROP_MONITOR;
			if(scan_info.sta.ie)
				atbm_kfree(scan_info.sta.ie);
			goto err;
		}	

		if(channel_in_cca(channel) == false){
			handle = RX_DROP_MONITOR;
			if(scan_info.sta.ie)
				atbm_kfree(scan_info.sta.ie);
			goto err;
		}
		scan_info.sta.cca = true;
	}
	if(func){
		if(func(sdata->local->scan_sdata,scan_data,&scan_info,false) == false){
			handle = RX_DROP_MONITOR;
			if(scan_info.sta.ie)
				atbm_kfree(scan_info.sta.ie);
			goto err;
		}
	}else if(sdata->local->scan_req_wrap.flags & IEEE80211_SCAN_REQ_RESULTS_HANDLE){
			ieee80211_scan_rx_internal_default(sdata->local->scan_sdata,&scan_info,false);
	}else {
		if(scan_info.sta.ie)
			atbm_kfree(scan_info.sta.ie);
	}
	atbm_dev_kfree_skb(skb);
err:
	rcu_read_unlock();
	return handle;	
}

static ieee80211_rx_result 
ieee80211_scan_rx_internal_skb(struct ieee80211_sub_if_data *sdata, struct sk_buff *skb)
{
	struct atbm_ieee80211_mgmt *mgmt;
	ieee80211_rx_result handle = RX_QUEUED;
	bool (*func)(struct ieee80211_sub_if_data *sdata,void *fs_data,struct ieee80211_internal_scan_result *result,bool finish);
	void *scan_data;
	struct ieee80211_internal_scan_result scan_info;
	__le16 fc;
	
	rcu_read_lock();
	
	memset(&scan_info,0,sizeof(struct ieee80211_internal_scan_result));
	func = rcu_dereference(sdata->local->internal_scan.req.result_handle);
	scan_data = rcu_dereference(sdata->local->internal_scan.req.priv);
	
	if (skb->len < 2){
		handle = RX_DROP_UNUSABLE;
		goto err;
	}

	if(func==NULL){
		handle = RX_DROP_UNUSABLE;
		goto err;
	}

	mgmt = (struct atbm_ieee80211_mgmt *) skb->data;
	fc = mgmt->frame_control;

	if (ieee80211_is_ctl(fc)){
		handle = RX_CONTINUE;
		goto err;
	}
	if (skb->len < 24){
		handle = RX_CONTINUE;
		goto err;
	}

	if((!ieee80211_is_probe_resp(fc))&&(!ieee80211_is_beacon(fc))){
		handle = RX_CONTINUE;
		goto err;
	}

	if(sdata != sdata->local->scan_sdata){
		handle = RX_CONTINUE;
		goto err;
	}
	scan_info.sta.skb = skb;
	
	if(func(sdata->local->scan_sdata,scan_data,&scan_info,false) == false){
		handle = RX_CONTINUE;
		goto err;
	}	
err:
	atbm_printk_debug("%s:handle(%zu)\n",__func__,(size_t)handle);
	rcu_read_unlock();
	return handle;	
}
ieee80211_rx_result
ieee80211_scan_rx_internal(struct ieee80211_sub_if_data *sdata, struct sk_buff *skb)
{
	ieee80211_rx_result handle = RX_QUEUED;

	
	if(ieee80211_2_4G_scan_results_limit(sdata->local,skb,NULL) < 0)
		return handle;
	
	if(sdata->local->scan_req_wrap.flags & IEEE80211_SCAN_REQ_RESULTS_SKB){	
		atbm_printk_debug("%s receive skb\n",__func__);
		handle = ieee80211_scan_rx_internal_skb(sdata,skb);
	}else{		
		atbm_printk_debug("%s receive stainfo\n",__func__);
		handle = ieee80211_scan_rx_internal_sta_info(sdata,skb);
	}
	return handle;	
}
ieee80211_rx_result
ieee80211_scan_rx(struct ieee80211_sub_if_data *sdata, struct sk_buff *skb)
{
	struct ieee80211_rx_status *rx_status = IEEE80211_SKB_RXCB(skb);
	struct atbm_ieee80211_mgmt *mgmt;
	struct ieee80211_bss *bss;
	u8 *elements;
	struct ieee80211_channel *channel;
	size_t baselen;
	int freq;
	__le16 fc;
	bool presp, beacon = false;
	struct ieee802_atbm_11_elems elems;
#ifdef CONFIG_ATBM_SLOW_DOWN_SIGNAL
	struct cfg80211_bss *tmp_cbss;
	const u8 *ssidie;
#endif
	if (skb->len < 2)
		return RX_DROP_UNUSABLE;

	mgmt = (struct atbm_ieee80211_mgmt *) skb->data;
	fc = mgmt->frame_control;

	if (ieee80211_is_ctl(fc))
		return RX_CONTINUE;

	if (skb->len < 24)
		return RX_CONTINUE;

	if(sdata->local->scan_sdata != sdata){
		return RX_CONTINUE;
	}
	presp = ieee80211_is_probe_resp(fc);
	if (presp) {
		/* ignore ProbeResp to foreign address */
		if (memcmp(mgmt->da, sdata->vif.addr, ETH_ALEN))
			return RX_DROP_MONITOR;

		presp = true;
		elements = mgmt->u.probe_resp.variable;
		baselen = offsetof(struct atbm_ieee80211_mgmt, u.probe_resp.variable);
	} else {
		beacon = ieee80211_is_beacon(fc);
		baselen = offsetof(struct atbm_ieee80211_mgmt, u.beacon.variable);
		elements = mgmt->u.beacon.variable;
	}
	if (!presp && !beacon)
		return RX_CONTINUE;

	if (baselen > skb->len)
		return RX_DROP_MONITOR;

	ieee802_11_parse_elems(elements, skb->len - baselen, &elems);

	if (elems.ds_params && elems.ds_params_len == 1)
		freq = ieee80211_channel_to_frequency(elems.ds_params[0],
						      rx_status->band);
	else
		freq = rx_status->freq;

	channel = ieee80211_get_channel(sdata->local->hw.wiphy, freq);

	if (!channel || channel->flags & IEEE80211_CHAN_DISABLED)
		return RX_DROP_MONITOR;

#ifdef CONFIG_ATBM_SLOW_DOWN_SIGNAL
	ssidie = cfg80211_find_ie(WLAN_EID_SSID,elements,baselen);
	tmp_cbss = ieee80211_atbm_get_bss(sdata->local->hw.wiphy,channel,mgmt->bssid,ssidie+2,ssidie[1],0,0);
	if(tmp_cbss)
	{
		//atbm_printk_always("mgmt->bssid =%pM,ori = %d,before = %d \n",mgmt->bssid,tmp_cbss->signal/100,rx_status->signal);
		if((tmp_cbss->signal/100 - rx_status->signal)> 10){
			rx_status->signal = tmp_cbss->signal/100 - (tmp_cbss->signal/100 - rx_status->signal)/4;
			//atbm_printk_always("mgmt->bssid =%pM,after = %d \n",mgmt->bssid,rx_status->signal);
		}
		ieee80211_atbm_put_bss(sdata->local->hw.wiphy,tmp_cbss);
	}
#endif

	bss = ieee80211_bss_info_update(sdata->local, rx_status,
					mgmt, skb->len, &elems,
					channel, beacon);
	if (bss)
		ieee80211_rx_bss_put(sdata->local, bss);

	atbm_dev_kfree_skb(skb);
	return RX_QUEUED;
}
static u16  ieee80211_scan_oper_channel(struct ieee80211_local *local)
{
	u16  oper_channel = 0;
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(local, local->scan_sdata);

	switch (ieee80211_get_channel_mode(local, NULL)) {
	case CHAN_MODE_HOPPING:
	case CHAN_MODE_FIXED:
		oper_channel = channel_hw_value(chan_state->conf.channel);
		atbm_printk_debug("%s:current op channel[%d]\n",__func__,oper_channel);
		break;
	case CHAN_MODE_UNDEFINED:
		oper_channel = 0;
		break;
	}
	/*
	*p2p do not use that function
	*/
	if(local->scan_sdata->vif.p2p == true){
		oper_channel = 0;
	}
	/*
	*cca do not use that function
	*/
	if(local->scan_req_wrap.flags & IEEE80211_SCAN_REQ_CCA){
		oper_channel = 0;
	}
	/*
	*nl80211 scan do not use that function
	*/
	if(test_bit(SCAN_CFG80211_SCANNING, &local->scanning)){
		oper_channel = 0;
	}

	if(oper_channel&&local->scan_sdata && !local->scan_sdata->u.mgd.associated){
#ifndef CONFIG_ATBM_STA_LISTEN
		oper_channel = 0;	
#else
		local->scan_req_wrap.flags |= IEEE80211_SCAN_REQ_NEED_LISTEN;
#endif
	}
	return oper_channel;
}
/* return false if no more work */
static bool ieee80211_prep_hw_scan(struct ieee80211_local *local)
{
	struct cfg80211_scan_request *req = local->scan_req;
	enum ieee80211_band band = IEEE80211_BAND_2GHZ;
	int i, ielen, n_chans;
	struct ieee80211_internal_ap_conf *conf = NULL;
	bool use_req = true;
	bool abort = false;
	u16  oper_channel = 0;
	
	rcu_read_lock();
	conf = rcu_dereference(local->scan_sdata->internal_ap_conf);
	while(conf){
		struct ieee80211_channel *ap_channel = ieee8011_chnum_to_channel(&local->hw,conf->channel);

		if(local->hw_scan_band){
			abort = true;
			break;
		}

		if(ap_channel == NULL){
			break;
		}
		
		local->hw_scan_band++;
		local->hw_scan_req->channels[0] = ap_channel;
		n_chans = 1;
		use_req = false;
		band = ap_channel->band;
		atbm_printk_debug("%s: use ap channel[%d]\n",__func__,conf->channel);
		break;
	}
	rcu_read_unlock();
	
	do {
		if (local->hw_scan_band == IEEE80211_NUM_BANDS)
			return false;
		
		if (abort == true)
			return false;
		
		if (use_req == false)
			break;
#ifdef CONFIG_JUAN_MISC
		band = local->hw_scan_band;
		n_chans = 0;
		for (i = 0; i < req->n_channels; i++) {
			if (req->channels[i]->band == band) {
				local->hw_scan_req->channels[n_chans] =
							req->channels[i];
				n_chans++;
			}
		}

		local->hw_scan_band++;
	} while (!n_chans);
#else
		ieee80211_scan_try_split(local);		
		oper_channel = ieee80211_scan_oper_channel(local);
		band = local->hw_scan_band;
		n_chans = 0;
		i = local->scan_channel_idx;
		
		for (; i < req->n_channels; i++) {
			/*
			*check band;
			*/
			if(req->channels[i]->band != band){
				continue;
			}

			if(n_chans && (oper_channel == channel_hw_value(req->channels[i]))){
				goto start_scan;
			}
			
			local->hw_scan_req->channels[n_chans] = req->channels[i];
			n_chans++;
			local->scan_n_channels ++;
			local->scan_channel_idx = i+1;

			if(oper_channel == channel_hw_value(req->channels[i])){
				atbm_printk_debug("%s:only send probe\n",__func__);
				local->scan_req_wrap.flags |= IEEE80211_SCAN_REQ_ONLY_PROB;
				if(n_chans != 1){
					atbm_printk_err("%s %d ,ERROR !!! n_chans=%d,not = 1\n",__func__,__LINE__,n_chans);
					return false;
				}
				goto start_scan;
			}
			
			if(n_chans >= local->scan_channel_space){
				goto start_scan;
			}
		}
		
		local->hw_scan_band++;
		local->scan_channel_idx = 0;
	} while (!n_chans);
	
start_scan:
#endif	
	local->hw_scan_req->n_channels = n_chans;
	atbm_printk_debug("%s:n_chans(%d),space(%d),index(%d),scaned(%d)\n",__func__,n_chans,
		local->scan_channel_space,local->scan_channel_idx,local->scan_n_channels);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 1, 0))
	ielen = ieee80211_build_preq_ies(local, (u8 *)local->hw_scan_req->ie,
					 req->ie, req->ie_len, band,
					 req->rates[band], 0);
#else
	ielen = ieee80211_build_preq_ies(local, (u8 *)local->hw_scan_req->ie,
					 req->ie, req->ie_len, band,
					 (u32)(~0), 0);

#endif
	local->hw_scan_req->ie_len = ielen;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 1, 0))
	local->hw_scan_req->no_cck = req->no_cck;
#endif

	return true;
}
#ifdef CONFIG_ATBM_SUPPORT_P2P
void ieee80211_run_pending_scan(struct ieee80211_local *local)
{
	lockdep_assert_held(&local->mtx);
	if ( local->scanning)
	{
		atbm_printk_scan("%s:scanning,so cannot active pennding scan\n",__func__);
		WARN_ON(1);
		return;
	}

	
	if(local->pending_scan_req&&local->pending_scan_sdata)
	{
		if(ieee80211_sdata_running(local->pending_scan_sdata))
		{
			atbm_printk_scan( "%s\n",__func__);
			local->scan_req = local->pending_scan_req;
			local->scan_sdata = local->pending_scan_sdata;
			local->pending_scan_req = NULL;
			local->pending_scan_sdata = NULL;
			ieee80211_queue_delayed_work(&local->hw, &local->scan_work, round_jiffies_relative(0));
		}
		else
		{
			atbm_printk_scan("%s:sdata is not running,but scan,err!!!!!!\n",__func__);
			atbm_notify_scan_done(local,local->pending_scan_req, true);
			local->pending_scan_req = NULL;
			local->pending_scan_sdata = NULL;
		}
	}
}
#endif
static void ieee80211_internal_scan_completed(struct ieee80211_hw *hw,bool aborted)
{	
	struct ieee80211_local *local = hw_to_local(hw);
	struct ieee80211_internal_scan_request *req = &local->internal_scan.req;
	u8 index = 0;
	bool (*func)(struct ieee80211_sub_if_data *sdata,void *fs_data,struct ieee80211_internal_scan_result *result,bool finish);
	void *scan_data;

	lockdep_assert_held(&local->mtx);
	
	func = rcu_dereference(local->internal_scan.req.result_handle);
	scan_data = rcu_dereference(local->internal_scan.req.priv);

	rcu_assign_pointer(local->internal_scan.req.result_handle,NULL);
	rcu_assign_pointer(local->internal_scan.req.priv,NULL);
	local->scanning = 0;
	synchronize_rcu();

	for(index = 0;index<req->n_macs;index++){
		hlist_del(&req->macs[index].hnode);
	}
	if(local->scan_req_wrap.flags & IEEE80211_SCAN_REQ_CCA){
		for(index = 0;index<local->scan_req->n_channels;index++){
			channel_clear_cca(local->scan_req->channels[index]);
		}
	}
	req->channels = NULL;
	req->n_channels = 0;
	req->ies = NULL;
	req->ie_len = 0;
	req->macs = NULL;
	req->n_macs = 0;
	req->ssids = NULL;
	req->n_ssids = 0;
	req->req_flags = 0;
	
	if(func)
		func(local->scan_sdata,scan_data,NULL,true);
	else if(local->scan_req_wrap.flags & IEEE80211_SCAN_REQ_RESULTS_HANDLE){
		WARN_ON(atomic_read(&local->internal_scan_status) != IEEE80211_INTERNAL_SCAN_STATUS__WAIT);
		if(aborted == true)
			atomic_set(&local->internal_scan_status,IEEE80211_INTERNAL_SCAN_STATUS__ABORT);
		else 
			atomic_set(&local->internal_scan_status,IEEE80211_INTERNAL_SCAN_STATUS__FINISHED);

		wake_up(&local->internal_scan_wq);
	}else {
		WARN_ON(1);
	}
	synchronize_rcu();
	atbm_kfree(local->scan_req);
}
static void __ieee80211_scan_completed(struct ieee80211_hw *hw, bool aborted,
				       bool was_hw_scan)
{
	struct ieee80211_local *local = hw_to_local(hw);

	lockdep_assert_held(&local->mtx);

	/*
	 * It's ok to abort a not-yet-running scan (that
	 * we have one at all will be verified by checking
	 * local->scan_req next), but not to complete it
	 * successfully.
	 */
	if (WARN_ON(!local->scanning && !aborted))
		aborted = true;

	if (WARN_ON(!local->scan_req))
		return;
	/*
	*disable filter
	*/
	if(local->scan_req_wrap.flags & IEEE80211_SCAN_REQ_ONLY_PROB){
		local->scan_sdata->req_filt_flags &= ~(FIF_OTHER_BSS);
		local->scan_sdata->flags &= ~(IEEE80211_SDATA_ALLMULTI);
		local->scan_req_wrap.flags &= ~IEEE80211_SCAN_REQ_ONLY_PROB;
		if(!(local->scan_req_wrap.flags & IEEE80211_SCAN_REQ_NEED_LISTEN))
			ieee80211_configure_filter(local->scan_sdata);
		else {
#ifdef CONFIG_ATBM_STA_LISTEN
			local->scan_req_wrap.flags &= ~IEEE80211_SCAN_REQ_NEED_LISTEN;
			local->ops->sta_stop_listen(&local->hw,&local->scan_sdata->vif);
			atbm_printk_debug("%s:exit listen\n",__func__);
#endif
		}
	}
	if (was_hw_scan && !aborted && ieee80211_prep_hw_scan(local)) {
		int rc = ieee80211_drv_hw_scan(local, local->scan_sdata, &local->scan_req_wrap);
		if (rc == 0){
			atbm_printk_scan( "%s:%d\n",__func__,__LINE__);
			return;
		}
	}

	atbm_kfree(local->hw_scan_req);
	local->hw_scan_req = NULL;	
	local->scan_req_wrap.req = NULL;
	if (local->scan_req != local->int_scan_req){
		if(test_bit(SCAN_INTERNAL_SCANNING, &local->scanning)){
			ieee80211_internal_scan_completed(hw,aborted);
		}else 
			atbm_notify_scan_done(local,local->scan_req, aborted);
	}
	local->scan_req_wrap.flags = 0;
	local->scan_req = NULL;
	local->scan_sdata = NULL;

	local->scanning = 0;
	local->scan_channel = NULL;
	local->scan_channel_space = 0;
	local->scan_channel_idx = 0;
	local->scan_channel_space = 0;
	local->scan_n_channels = 0;
	ieee80211_hw_config(local, IEEE80211_CONF_CHANGE_CHANNEL);

	ieee80211_recalc_idle(local);

	ieee80211_mlme_notify_scan_completed(local);
#ifdef CONFIG_ATBM_SUPPORT_IBSS
	ieee80211_ibss_notify_scan_completed(local);
#endif
#ifndef CONFIG_MAC80211_ATBM_MESH
	ieee80211_mesh_notify_scan_completed(local);
#endif
#ifdef CONFIG_ATBM_SUPPORT_P2P
	ieee80211_start_next_roc(local);
#endif
	ieee80211_queue_work(&local->hw, &local->work_work);
}

void ieee80211_scan_completed(struct ieee80211_hw *hw, bool aborted)
{
	struct ieee80211_local *local = hw_to_local(hw);

	trace_api_scan_completed(local, aborted);

	set_bit(SCAN_COMPLETED, &local->scanning);
	if (aborted)
		set_bit(SCAN_ABORTED, &local->scanning);
	ieee80211_queue_delayed_work(&local->hw, &local->scan_work, msecs_to_jiffies(local->scan_idle_time));
}
//EXPORT_SYMBOL(ieee80211_scan_completed);

static int __ieee80211_start_scan(struct ieee80211_sub_if_data *sdata,
				  struct cfg80211_scan_request *req)
{
	struct ieee80211_local *local = sdata->local;
	int rc;

	lockdep_assert_held(&local->mtx);

	if (local->scan_req){
		atbm_printk_scan("%s:%d\n",__func__,__LINE__);
		return -EBUSY;
	}
	if(ieee80211_check_country_limit_scan_2_4G_chan(local,req) < 0){
		atbm_printk_scan("%s:%d\n",__func__,__LINE__);
		return -EBUSY;
	}

#ifdef CONFIG_ATBM_SUPPORT_P2P
	if (!list_empty(&local->roc_list)){
		/*
		*android 5.0 we can not return err, because the p2p function can not 
		*reset the p2p station mechine.
		*/

		if((local->pending_scan_req != NULL ) || (local->pending_scan_sdata != NULL))
		{
			atbm_printk_scan("%s:only can pending one scan request\n",__func__);
			return -EBUSY;
		}
		else
		{
			local->pending_scan_req = req;
			local->pending_scan_sdata = sdata;
			atbm_printk_scan( "%s:%d: offch runing ,so delay scanning\n",__func__,__LINE__);
		}
		return 0;
	}
#endif
	if (!list_empty(&local->work_list)) {
		/* wait for the work to finish/time out */
		local->scan_req = req;
		local->scan_sdata = sdata;
		local->pending_scan_start_time = jiffies;
		atbm_printk_scan("%s(%s):work_list is not empty,pend scan\n",__func__,sdata->name);
		return 0;
	}
	memset(&local->scan_info,0,sizeof(struct cfg80211_scan_info));
	if (local->ops->hw_scan) {
		u8 *ies;

		local->hw_scan_req = atbm_kmalloc(
				sizeof(*local->hw_scan_req) +
				req->n_channels * sizeof(req->channels[0]) +
				2 + IEEE80211_MAX_SSID_LEN + local->scan_ies_len +
				req->ie_len, GFP_KERNEL);
		if (!local->hw_scan_req)
			return -ENOMEM;

		local->hw_scan_req->ssids = req->ssids;
		local->hw_scan_req->n_ssids = req->n_ssids;
		ies = (u8 *)local->hw_scan_req +
			sizeof(*local->hw_scan_req) +
			req->n_channels * sizeof(req->channels[0]);
		local->hw_scan_req->ie = ies;

		local->hw_scan_band = 0;

		/*
		 * After allocating local->hw_scan_req, we must
		 * go through until ieee80211_prep_hw_scan(), so
		 * anything that might be changed here and leave
		 * this function early must not go after this
		 * allocation.
		 */
	}

	local->scan_req = req;
	local->scan_sdata = sdata;
	local->scan_n_channels = 0;
	local->scan_channel_idx = 0;
	local->scan_channel_space = req->n_channels;
	local->scan_idle_time = 0;
	
	if (local->ops->hw_scan)
		__set_bit(SCAN_HW_SCANNING, &local->scanning);
	else
		__set_bit(SCAN_SW_SCANNING, &local->scanning);
	/*
	*cfg80211 triger scan
	*/
	__set_bit(SCAN_CFG80211_SCANNING, &local->scanning);
	
	ieee80211_recalc_idle(local);
	
	local->scan_req_wrap.flags = 0;
	
	if (local->ops->hw_scan) {
		WARN_ON(!ieee80211_prep_hw_scan(local));
		if(sdata->last_scan_ie_len < local->hw_scan_req->ie_len){
			if(sdata->last_scan_ie){
				atbm_kfree(sdata->last_scan_ie);
				sdata->last_scan_ie = NULL;
			}
			sdata->last_scan_ie = atbm_kmalloc(local->hw_scan_req->ie_len,GFP_KERNEL);
			WARN_ON(sdata->last_scan_ie == NULL);
		}
		if(sdata->last_scan_ie&&local->hw_scan_req->ie){
			memcpy(sdata->last_scan_ie,local->hw_scan_req->ie,local->hw_scan_req->ie_len);
			sdata->last_scan_ie_len = local->hw_scan_req->ie_len;
		}
		local->scan_req_wrap.req = local->hw_scan_req;


		
		rc = ieee80211_drv_hw_scan(local, sdata, &local->scan_req_wrap);
	} else{
	//	BUG_ON(1);
		atbm_printk_err("%s %d ,ERROR !!! local->ops->hw_scan is NULL\n",__func__,__LINE__);
		return -EBUSY;
	}
	
	if (rc) {
		atbm_kfree(local->hw_scan_req);
		local->hw_scan_req = NULL;
		local->scanning = 0;

		ieee80211_recalc_idle(local);

		local->scan_req = NULL;
		local->scan_sdata = NULL;
	}

	return rc;
}
void ieee80211_scan_work(struct atbm_work_struct *work)
{
	struct ieee80211_local *local =
		container_of(work, struct ieee80211_local, scan_work.work);
	struct ieee80211_sub_if_data *sdata;
	bool aborted, hw_scan;

	mutex_lock(&local->mtx);

	sdata = local->scan_sdata;

	if (test_and_clear_bit(SCAN_COMPLETED, &local->scanning)) {
		aborted = test_and_clear_bit(SCAN_ABORTED, &local->scanning);
		goto out_complete;
	}

	if (!sdata || !local->scan_req)
		goto out;

	if (local->scan_req && !local->scanning) {
		struct cfg80211_scan_request *req = local->scan_req;
		int rc;

		local->scan_req = NULL;
		local->scan_sdata = NULL;
		atbm_printk_scan( "%s:[%s] start pending scan\n",__func__,sdata->name);
		rc = __ieee80211_start_scan(sdata, req);
		if (rc) {
			/* need to complete scan in cfg80211 */
			local->scan_req = req;
			aborted = true;
			goto out_complete;
		} else
			goto out;
	}
	if(test_bit(SCAN_HW_SCANNING,&local->scanning)){
		atbm_printk_scan( "%s:[%s]: hw scan running\n",__func__,sdata->name);
		goto out;
	}
	/*
	 * Avoid re-scheduling when the sdata is going away.
	 */
	if (!ieee80211_sdata_running(sdata)) {
		aborted = true;
		goto out_complete;
	}
	
	goto out;

out_complete:
	hw_scan = test_bit(SCAN_HW_SCANNING, &local->scanning);
	__ieee80211_scan_completed(&local->hw, aborted, hw_scan);
out:
	mutex_unlock(&local->mtx);
}

int ieee80211_request_scan(struct ieee80211_sub_if_data *sdata,
			   struct cfg80211_scan_request *req)
{
	int res;

	mutex_lock(&sdata->local->mtx);
	res = __ieee80211_start_scan(sdata, req);
	mutex_unlock(&sdata->local->mtx);

	return res;
}
bool ieee80211_internal_scan_triger(struct ieee80211_sub_if_data *sdata,struct cfg80211_scan_request *req)
{
	struct ieee80211_local *local = sdata->local;
	int rc;
	u8 *ies;
	int i = 0;
	//int ret = 0;
	
	if(ieee80211_check_country_limit_scan_2_4G_chan(local,req) < 0){
		atbm_printk_err("%s  %d\n",__func__,__LINE__);
		return false;
	}
	
	lockdep_assert_held(&local->mtx);

	if(local->internal_scan.req.req_flags & IEEE80211_INTERNAL_SCAN_FLAGS__CCA){
		
		atbm_printk_err("%s:hw.conf.flags(%x)\n",__func__,local->hw.conf.flags);
		if(!!(local->hw.conf.flags & IEEE80211_CONF_IDLE) == 0){
			atbm_printk_err("now running sta or ap,not idle, maybe have some queue! \n");
//			return false;
		}
	}else {
		if((!!(local->hw.conf.flags & IEEE80211_CONF_IDLE) == 0) && req->n_channels > 2){
			atbm_printk_debug("%s:running split scan\n",__func__);
			local->internal_scan.req.req_flags |= IEEE80211_INTERNAL_SCAN_FLAGS__SCAN_SPLIT;
		}
	}
	ieee80211_scan_internal_list_flush(local);
	memset(&local->scan_info,0,sizeof(struct cfg80211_scan_info));
	local->hw_scan_req = atbm_kmalloc(
			sizeof(*local->hw_scan_req) +
			req->n_channels * sizeof(req->channels[0]) +
			2 + IEEE80211_MAX_SSID_LEN + local->scan_ies_len +
			req->ie_len, GFP_KERNEL);
	if (!local->hw_scan_req)
		return false;

	local->hw_scan_req->ssids = req->ssids;
	local->hw_scan_req->n_ssids = req->n_ssids;
	ies = (u8 *)local->hw_scan_req +
		sizeof(*local->hw_scan_req) +
		req->n_channels * sizeof(req->channels[0]);
	local->hw_scan_req->ie = ies;

	local->hw_scan_band = 0;

	/*
	 * After allocating local->hw_scan_req, we must
	 * go through until ieee80211_prep_hw_scan(), so
	 * anything that might be changed here and leave
	 * this function early must not go after this
	 * allocation.
	 */

	local->scan_req = req;
	local->scan_sdata = sdata;
	local->scan_n_channels = 0;
	local->scan_channel_idx = 0;
	local->scan_channel_space = req->n_channels;
	local->scan_idle_time = 0;
	
	__set_bit(SCAN_HW_SCANNING, &local->scanning);
	__set_bit(SCAN_INTERNAL_SCANNING, &local->scanning);
	atomic_set(&local->internal_scan_status,IEEE80211_INTERNAL_SCAN_STATUS__IDLE);
	ieee80211_recalc_idle(local);
	
	memset(&local->scan_req_wrap,0,sizeof(struct ieee80211_scan_req_wrap));

	local->scan_req_wrap.flags = IEEE80211_SCAN_REQ_INTERNAL;
	
	if(local->internal_scan.req.req_flags & IEEE80211_INTERNAL_SCAN_FLAGS__CCA){
		local->scan_req_wrap.flags |= IEEE80211_SCAN_REQ_CCA;

		for(i = 0;i<req->n_channels;i++){
			channel_mask_cca(req->channels[i]);
		}
	}
	if(!local->internal_scan.req.result_handle)
		local->scan_req_wrap.flags |= IEEE80211_SCAN_REQ_RESULTS_HANDLE;

	if(local->internal_scan.req.req_flags & IEEE80211_INTERNAL_SCAN_FLAGS__PASSAVI_SCAN)
		local->scan_req_wrap.flags |= IEEE80211_SCAN_REQ_PASSIVE_SCAN;
	if(local->internal_scan.req.req_flags & IEEE80211_INTERNAL_SCAN_FLAGS__NEED_SKB)
		local->scan_req_wrap.flags |= IEEE80211_SCAN_REQ_RESULTS_SKB;
	if(local->internal_scan.req.req_flags & IEEE80211_INTERNAL_SCAN_FLAGS__SCAN_SPLIT){
		local->scan_req_wrap.flags |= IEEE80211_SCAN_REQ_SPILT;
	}
	if(local->internal_scan.req.req_flags & IEEE80211_INTERNAL_SCAN_FLAGS__NEED_BSSID){
		local->scan_req_wrap.flags |= IEEE80211_SCAN_REQ_NEED_BSSID;
		memcpy(local->scan_req_wrap.bssid,local->internal_scan.req.bssid,6);
	}
	local->scan_req_wrap.req = local->hw_scan_req;
	WARN_ON(!ieee80211_prep_hw_scan(local));
	rc = ieee80211_drv_hw_scan(local, sdata, &local->scan_req_wrap);

	if (rc) {
		atbm_kfree(local->hw_scan_req);
		local->hw_scan_req = NULL;
		local->scanning = 0;

		ieee80211_recalc_idle(local);
		for(i = 0;i<req->n_channels;i++){
			channel_clear_cca(req->channels[i]);
		}
		local->scan_req = NULL;
		local->scan_sdata = NULL;
		memset(&local->scan_req_wrap,0,sizeof(struct ieee80211_scan_req_wrap));
	}else{
		if(local->scan_req_wrap.flags & IEEE80211_SCAN_REQ_RESULTS_HANDLE){
			atomic_set(&local->internal_scan_status,IEEE80211_INTERNAL_SCAN_STATUS__WAIT);
		}else {
			atomic_set(&local->internal_scan_status,IEEE80211_INTERNAL_SCAN_STATUS__FINISHED);
		}
	}

	return rc == 0?true:false;
}
#ifdef CONFIG_ATBM_SUPPORT_IBSS
int ieee80211_request_internal_scan(struct ieee80211_sub_if_data *sdata,
				    const u8 *ssid, u8 ssid_len,
				    struct ieee80211_channel *chan)
{
	struct ieee80211_local *local = sdata->local;
	int ret = -EBUSY;
	enum ieee80211_band band;

	mutex_lock(&local->mtx);

	/* busy scanning */
	if (local->scan_req)
		goto unlock;

	/* fill internal scan request */
	if (!chan) {
		int i, nchan = 0;

		for (band = 0; band < IEEE80211_NUM_BANDS; band++) {
			if (!local->hw.wiphy->bands[band])
				continue;
			for (i = 0;
			     i < local->hw.wiphy->bands[band]->n_channels;
			     i++) {
				local->int_scan_req->channels[nchan] =
				    &local->hw.wiphy->bands[band]->channels[i];
				nchan++;
			}
		}

		local->int_scan_req->n_channels = nchan;
	} else {
		local->int_scan_req->channels[0] = chan;
		local->int_scan_req->n_channels = 1;
	}

	local->int_scan_req->ssids = &local->scan_ssid;
	local->int_scan_req->n_ssids = 1;
	memcpy(local->int_scan_req->ssids[0].ssid, ssid, IEEE80211_MAX_SSID_LEN);
	local->int_scan_req->ssids[0].ssid_len = ssid_len;

	ret = __ieee80211_start_scan(sdata, sdata->local->int_scan_req);
 unlock:
	mutex_unlock(&local->mtx);
	return ret;
}
#endif
/*
 * Only call this function when a scan can't be queued -- under RTNL.
 */
void ieee80211_scan_cancel(struct ieee80211_local *local)
{
	/*
	 * We are canceling software scan, or deferred scan that was not
	 * yet really started (see __ieee80211_start_scan ).
	 *
	 * Regarding hardware scan:
	 * - we can not call  __ieee80211_scan_completed() as when
	 *   SCAN_HW_SCANNING bit is set this function change
	 *   local->hw_scan_req to operate on 5G band, what race with
	 *   driver which can use local->hw_scan_req
	 *
	 * - we can not cancel scan_work since driver can schedule it
	 *   by ieee80211_scan_completed(..., true) to finish scan
	 *
	 * Hence we only call the cancel_hw_scan() callback, but the low-level
	 * driver is still responsible for calling ieee80211_scan_completed()
	 * after the scan was completed/aborted.
	 */

	mutex_lock(&local->mtx);
	if (!local->scan_req)
		goto out;

	if (test_bit(SCAN_HW_SCANNING, &local->scanning)) {
		if (local->ops->cancel_hw_scan)
			drv_cancel_hw_scan(local, local->scan_sdata);
//		goto out;
	}

	/*
	 * If the work is currently running, it must be blocked on
	 * the mutex, but we'll set scan_sdata = NULL and it'll
	 * simply exit once it acquires the mutex.
	 */
	atbm_cancel_delayed_work(&local->scan_work);
	/* and clean up */
	__ieee80211_scan_completed(&local->hw, true, test_bit(SCAN_HW_SCANNING, &local->scanning)?true:false);
out:
	mutex_unlock(&local->mtx);
}
#ifdef CONFIG_ATBM_SUPPORT_SCHED_SCAN
int ieee80211_request_sched_scan_start(struct ieee80211_sub_if_data *sdata,
				       struct cfg80211_sched_scan_request *req)
{
	struct ieee80211_local *local = sdata->local;
	int ret, i;

	mutex_lock(&sdata->local->mtx);

	if (local->sched_scanning) {
		ret = -EBUSY;
		goto out;
	}

	if (!local->ops->sched_scan_start) {
		ret = -ENOTSUPP;
		goto out;
	}

	for (i = 0; i < IEEE80211_NUM_BANDS; i++) {
#ifdef CONFIG_ATBM_SUPPORT_SCHED_SCAN
#ifdef ROAM_OFFLOAD
		if (!local->hw.wiphy->bands[i])
			continue;
#endif /*ROAM_OFFLOAD*/
#endif
		local->sched_scan_ies.ie[i] = atbm_kzalloc(2 +
						      IEEE80211_MAX_SSID_LEN +
						      local->scan_ies_len +
						      req->ie_len,
						      GFP_KERNEL);
		if (!local->sched_scan_ies.ie[i]) {
			ret = -ENOMEM;
			goto out_free;
		}

		local->sched_scan_ies.len[i] =
			ieee80211_build_preq_ies(local,
						 local->sched_scan_ies.ie[i],
						 req->ie, req->ie_len, i,
						 (u32) -1, 0);
	}

	ret = drv_sched_scan_start(local, sdata, req,
				   &local->sched_scan_ies);
	if (ret == 0) {
		local->sched_scanning = true;
		goto out;
	}

out_free:
	while (i > 0)
		atbm_kfree(local->sched_scan_ies.ie[--i]);
out:
	mutex_unlock(&sdata->local->mtx);
	return ret;
}

int ieee80211_request_sched_scan_stop(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_local *local = sdata->local;
	int ret = 0, i;

	mutex_lock(&sdata->local->mtx);

	if (!local->ops->sched_scan_stop) {
		ret = -ENOTSUPP;
		goto out;
	}

	if (local->sched_scanning) {
		for (i = 0; i < IEEE80211_NUM_BANDS; i++)
			atbm_kfree(local->sched_scan_ies.ie[i]);

		drv_sched_scan_stop(local, sdata);
		local->sched_scanning = false;
	}
out:
	mutex_unlock(&sdata->local->mtx);

	return ret;
}

void ieee80211_sched_scan_results(struct ieee80211_hw *hw)
{
	struct ieee80211_local *local = hw_to_local(hw);
#ifdef ROAM_OFFLOAD
	if(local->sched_scanning)
	{
		local->sched_scanning = false;
		return;
	}
#endif /*ROAM_OFFLOAD*/

	trace_api_sched_scan_results(local);

	cfg80211_sched_scan_results(hw->wiphy);
}
//EXPORT_SYMBOL(ieee80211_sched_scan_results);

void ieee80211_sched_scan_stopped_work(struct atbm_work_struct *work)
{
	struct ieee80211_local *local =
		container_of(work, struct ieee80211_local,
			     sched_scan_stopped_work);
	int i;

	mutex_lock(&local->mtx);

	if (!local->sched_scanning) {
		mutex_unlock(&local->mtx);
		return;
	}

	for (i = 0; i < IEEE80211_NUM_BANDS; i++)
		atbm_kfree(local->sched_scan_ies.ie[i]);

	local->sched_scanning = false;

	mutex_unlock(&local->mtx);

	cfg80211_sched_scan_stopped(local->hw.wiphy);
}

void ieee80211_sched_scan_stopped(struct ieee80211_hw *hw)
{
	struct ieee80211_local *local = hw_to_local(hw);

	trace_api_sched_scan_stopped(local);

	ieee80211_queue_work(&local->hw, &local->sched_scan_stopped_work);
}
#endif
//EXPORT_SYMBOL(ieee80211_sched_scan_stopped);
