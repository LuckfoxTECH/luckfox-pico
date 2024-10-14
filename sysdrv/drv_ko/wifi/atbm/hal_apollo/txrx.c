/*
 * Datapath implementation for altobeam APOLLO mac80211 drivers
 * *
 * Copyright (c) 2016, altobeam
 * Author:
 *
 *Based on apollo code
 * Copyright (c) 2010, ST-Ericsson
 * Author: Dmitry Tarnyagin <dmitry.tarnyagin@stericsson.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <net/atbm_mac80211.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/udp.h>
#include <net/ip.h>
#include <linux/bitops.h>
#include "apollo.h"
#include "wsm.h"
#include "bh.h"
#include "ap.h"
#include "debug.h"
#include "sta.h"
#include "sbus.h"
#include "atbm_p2p.h"
#include "mac80211/ieee80211_i.h"


#if defined(CONFIG_ATBM_APOLLO_TX_POLICY_DEBUG)
#define tx_policy_printk(...) atbm_printk_always(__VA_ARGS__)
#else
#define tx_policy_printk(...)
#endif

#define ATBM_APOLLO_INVALID_RATE_ID (0xFF)

#ifdef CONFIG_ATBM_APOLLO_TESTMODE
#include "atbm_testmode.h"
#endif /* CONFIG_ATBM_APOLLO_TESTMODE */
static const struct ieee80211_rate *
atbm_get_tx_rate(const struct atbm_common *hw_priv,
		   const struct ieee80211_tx_rate *rate);
#ifdef ATBM_11W_TEST
static u8 broadcast_addr[6] = {0x01,0x00,0x02,0x03,0x04,0xff};
#endif
/* ******************************************************************** */
/* TX policy cache implementation					*/
#define IS_BOOTP_PORT(src_port,des_port) ((((src_port) == 67)&&((des_port) == 68)) || \
										   (((src_port) == 68)&&((des_port) == 67)))

#if 1
#if 0
void dhcp_hexdump(char *prefix, u8 *data, int len)
{
	int i;

	for (i = 0; i < len; i++) {
	   if((i % 16)==0)
		   atbm_printk_always("\n");
	   atbm_printk_always("%02x ", data[i]);

	}
	atbm_printk_always("\n");
}
#endif
//extern int g_connetting;
static int tx_dhcp_retry_cnt=0;


void atbm_tx_udp(struct ieee80211_hw *dev,struct sk_buff *skb)
{
	struct ieee80211_tx_info *tx_info = IEEE80211_SKB_CB(skb);
	struct atbm_vif *priv = ABwifi_get_vif_from_ieee80211(tx_info->control.vif);
	
	if(priv == NULL){
		return;
	}
	//struct ieee80211_hdr *frame = (struct ieee80211_hdr *)skb->data;
	if (atomic_read(&priv->enabled) == 0)
		return;
	if (skb->protocol == htons(ETH_P_IP))
	{
		struct iphdr *iph; //= ip_hdr(skb);
		struct udphdr *udph;
		
		//u8 mac_hdr_len=ieee80211_hdrlen(hdr->frame_control);
		iph = ip_hdr(skb);
		if (iph->protocol==IPPROTO_UDP){
			/* Send all udp frames on VO. Accordingly set TID to 7. */
			//atbm_skb_set_queue_mapping(skb, IEEE80211_AC_VI);
			//skb->priority = 5;
		}
		udph = (struct udphdr *)((u8*)iph+(iph->ihl)*4);
		if(IS_BOOTP_PORT(ntohs(udph->source),ntohs(udph->dest)))
		{
			tx_dhcp_retry_cnt = 0;
			//printk("start tx_udp %d,dhcp_retry_skb %p\n",SDATA_IS_CONNECTTING(vif_to_sdata(priv->vif)),priv->dhcp_retry_skb);
			spin_lock_bh(&priv->dhcp_retry_spinlock);
			if(priv->dhcp_retry_skb){
				atbm_kfree_skb(priv->dhcp_retry_skb);
				priv->dhcp_retry_skb=NULL;
			}
			//if(g_connetting){
			if(SDATA_IS_CONNECTTING(vif_to_sdata(priv->vif))){
			    priv->dhcp_retry_skb = atbm_skb_copy(skb, GFP_ATOMIC);
				if(priv->dhcp_retry_skb == NULL){
					spin_unlock_bh(&priv->dhcp_retry_spinlock);
					return;
				}
				atbm_hw_priv_queue_delayed_work(priv->hw_priv,&priv->dhcp_retry_work, HZ/2);
				//memcpy(priv->dhcp_retry_skb->data,skb->data,skb->len);
				memcpy(IEEE80211_SKB_CB(priv->dhcp_retry_skb),IEEE80211_SKB_CB(skb),sizeof(struct ieee80211_tx_info));
				atbm_printk_rx("start atbm_hw_priv_queue_delayed_work\n");
			}
			else {
				atbm_hw_cancel_delayed_work(&priv->dhcp_retry_work,false);
			}
			spin_unlock_bh(&priv->dhcp_retry_spinlock);
			//dhcp_hexdump("Tx Dhcp ",(u8*)skb->data,skb->len);
			
		}
	}
}

#if 0
int atbm_is_dhcp_frame(struct atbm_vif *priv,struct sk_buff *skb)
{
	//struct ieee80211_hdr *frame = (struct ieee80211_hdr *)skb->data;
	
	if (skb->protocol == htons(ETH_P_IP))	{
		struct iphdr *iph; //= ip_hdr(skb);
		struct udphdr *udph;
		
		iph = ip_hdr(skb);
		udph = (struct udphdr *)((u8*)iph+(iph->ihl)*4);
		if(IS_BOOTP_PORT(ntohs(udph->source),ntohs(udph->dest)))		{
			return 1;			
		}
	}

	return 0;
}
#endif
#else
void atbm_tx_udp(struct sk_buff *skb)
{
	const struct iphdr *ip;	
	u16 ether_type;
	struct ieee80211_hdr *hdr = (struct ieee80211_hdr *)skb->data;
	//frame_hexdump("This is Rx hdr",(u8*)skb->data,300);
	u8 mac_hdr_len=ieee80211_hdrlen(hdr->frame_control);
	if (hdr->frame_control&__cpu_to_le32(IEEE80211_FCTL_PROTECTED))
	{
		mac_hdr_len+=8;
	}
	//printk("mac_hdr_len=%x\n",mac_hdr_len);
	ip =(struct iphdr *)((u8*)skb->data+mac_hdr_len +6+2);
	ether_type=*(u16*)((u8*)skb->data+mac_hdr_len+6);
	//printk("ether_type=%x:%x\n",ether_type,htons(ETH_P_IP));
	if (ether_type==htons(ETH_P_IP))
	{
	//	printk("protocol=%x\n",ip->protocol);
		if (IPPROTO_UDP==ip->protocol){
			struct udphdr *udph=(struct udphdr *)((u8*)ip+(ip->ihl<<2));
			if(IS_BOOTP_PORT(ntohs(udph->source),ntohs(udph->dest)))
			{
				//struct bootp_pkt *b;
				//int bootp_len=236;
				//int dhcp_magic_len=4;
				b=(struct bootp_pkt *)((u8*)(udph+8));
				printk("dhcp  tx msgId=%x\n",(u32*)(b+(bootp_len+dhcp_magic_len));
				
				if ((u8*)(udph+226+mac_hdr_len)==0x2){
					printk("******Rx dhcp Offer(%l) ms****\n",jiffies_to_msecs(jiffies));
				}else if ((u8*)(udph+226+mac_hdr_len)==0x5){
					printk("******Rx dhcp Ack(%l) ms****\n",jiffies_to_msecs(jiffies));

				}
			}
		}
	}
}
#endif


void atbm_rx_udp(struct atbm_vif *priv,struct sk_buff *skb)
{
	const struct iphdr *ip;	
	u16 ether_type;
	struct ieee80211_hdr *hdr = (struct ieee80211_hdr *)skb->data;
	u8 mac_hdr_len=ieee80211_hdrlen(hdr->frame_control);

	if(!ieee80211_is_data(hdr->frame_control))
		return;
	if(ieee80211_is_nullfunc(hdr->frame_control))
		return;
	if(ieee80211_is_qos_nullfunc(hdr->frame_control))
		return;
	
	if (hdr->frame_control&__cpu_to_le32(IEEE80211_FCTL_PROTECTED))
	{
		mac_hdr_len+=8;
	}
	//printk("mac_hdr_len=%x\n",mac_hdr_len);
	ip =(struct iphdr *)((u8*)skb->data+mac_hdr_len +6+2);
	ether_type=*(u16*)((u8*)skb->data+mac_hdr_len+6);
	//printk("ether_type=%x:%x\n",ether_type,htons(ETH_P_IP));
	if (ether_type==htons(ETH_P_IP))
	{
	//	printk("protocol=%x\n",ip->protocol);
		if (IPPROTO_UDP==ip->protocol){
			struct udphdr *udph=(struct udphdr *)((u8*)ip+(ip->ihl<<2));
			if(IS_BOOTP_PORT(ntohs(udph->source),ntohs(udph->dest)))
			{
				atbm_printk_rx("cancel dhcp_retry_work\n");
				atbm_hw_cancel_delayed_work(&priv->dhcp_retry_work,false);
				
				//dhcp_hexdump("Rx Dhcp ",(u8*)udph+236+8,16);
			}
		}
	}
}
void __atbm_tx(struct ieee80211_hw *dev, struct sk_buff *skb);

void atbm_dhcp_retry_work(struct atbm_work_struct *work)
{	
	struct atbm_vif *priv =
		container_of(work, struct atbm_vif, dhcp_retry_work.work);
	struct atbm_common *hw_priv = ABwifi_vifpriv_to_hwpriv(priv);
	//int timeout; /* in beacons */
	struct sk_buff *skb;

	if(atbm_bh_is_term(hw_priv)){
		return;
	}
	atbm_printk_tx("atbm_dhcp_retry_work priv->dhcp_retry_skb %p %d\n",priv->dhcp_retry_skb,SDATA_IS_CONNECTTING(vif_to_sdata(priv->vif)));
	spin_lock_bh(&priv->dhcp_retry_spinlock);
	if((priv->dhcp_retry_skb != NULL) &&(SDATA_IS_CONNECTTING(vif_to_sdata(priv->vif)))){
		//skb = priv->dhcp_retry_skb;
		//priv->dhcp_retry_skb = NULL;
		
		skb = atbm_skb_copy(priv->dhcp_retry_skb , GFP_ATOMIC);
		if(skb == NULL){
			spin_unlock_bh(&priv->dhcp_retry_spinlock);
			atbm_hw_priv_queue_delayed_work(priv->hw_priv,&priv->dhcp_retry_work, HZ/50);
			return;
		}
		memcpy(IEEE80211_SKB_CB(skb),IEEE80211_SKB_CB(priv->dhcp_retry_skb),sizeof(struct ieee80211_tx_info));
						
		spin_unlock_bh(&priv->dhcp_retry_spinlock);
		atbm_printk_tx("atbm_dhcp_retry_work __atbm_tx\n");
		local_bh_disable();
		__atbm_tx(priv->hw,skb);
		local_bh_enable();
		spin_lock_bh(&priv->dhcp_retry_spinlock);
		tx_dhcp_retry_cnt++;
		atbm_printk_tx("restart dhcp_retry_work %d\n",tx_dhcp_retry_cnt);
		if(tx_dhcp_retry_cnt <3){
			atbm_hw_priv_queue_delayed_work(priv->hw_priv,&priv->dhcp_retry_work, HZ/2);
		}
		else {
			atbm_kfree_skb(priv->dhcp_retry_skb);
			priv->dhcp_retry_skb = NULL;
		}
	}
	else {
		atbm_kfree_skb(priv->dhcp_retry_skb);
		priv->dhcp_retry_skb = NULL;
	}
	spin_unlock_bh(&priv->dhcp_retry_spinlock);
}
#ifndef CONFIG_RATE_HW_CONTROL
static void tx_policy_dump(struct tx_policy *policy)
{
	tx_policy_printk( "[TX policy] "
		"%.1X%.1X%.1X%.1X%.1X%.1X%.1X%.1X"
		"%.1X%.1X%.1X%.1X%.1X%.1X%.1X%.1X"
		"%.1X%.1X%.1X%.1X%.1X%.1X%.1X%.1X: %d\n",
		policy->raw[0] & 0x0F,  policy->raw[0] >> 4,
		policy->raw[1] & 0x0F,  policy->raw[1] >> 4,
		policy->raw[2] & 0x0F,  policy->raw[2] >> 4,
		policy->raw[3] & 0x0F,  policy->raw[3] >> 4,
		policy->raw[4] & 0x0F,  policy->raw[4] >> 4,
		policy->raw[5] & 0x0F,  policy->raw[5] >> 4,
		policy->raw[6] & 0x0F,  policy->raw[6] >> 4,
		policy->raw[7] & 0x0F,  policy->raw[7] >> 4,
		policy->raw[8] & 0x0F,  policy->raw[8] >> 4,
		policy->raw[9] & 0x0F,  policy->raw[9] >> 4,
		policy->raw[10] & 0x0F,  policy->raw[10] >> 4,
		policy->raw[11] & 0x0F,  policy->raw[11] >> 4,
		policy->defined);
}
#endif
#ifdef CONFIG_ATBM_SUPPORT_P2P
static void atbm_check_go_neg_conf_success(struct atbm_common *hw_priv,
						u8 *action)
{
	if (action[2] == 0x50 && action[3] == 0x6F && action[4] == 0x9A &&
		action[5] == 0x09 && action[6] == 0x02) {
		if(action[17] == 0) {
			hw_priv->is_go_thru_go_neg = true;
		}
		else {
			hw_priv->is_go_thru_go_neg = false;
		}
	}
}

static void atbm_check_prov_desc_req(struct atbm_common *hw_priv,
                                                u8 *action)
{
	if (action[2] == 0x50 && action[3] == 0x6F && action[4] == 0x9A &&
                action[5] == 0x09 && action[6] == 0x07) {
                        hw_priv->is_go_thru_go_neg = false;
        }
}
#endif
#ifndef CONFIG_RATE_HW_CONTROL
static void tx_policy_build(const struct atbm_common *hw_priv,
	/* [out] */ struct tx_policy *policy,
	struct ieee80211_tx_rate *rates, size_t count)
{
	int i, j;
	unsigned limit = hw_priv->short_frame_max_tx_count;
	unsigned total = 0;
	//BUG_ON(rates[0].idx < 0);

	if(rates[0].idx < 0){
		atbm_printk_err("%s %d ,ERROR !!! rates[0].idx=%d < 0\n",__func__,__LINE__,rates[0].idx);
		return;
	}
	
	memset(policy, 0, sizeof(*policy));

	/* minstrel is buggy a little bit, so distille
	 * incoming rates first. */

	/* Sort rates in descending order. */
	for (i = 1; i < count; ++i) {
		if (rates[i].idx < 0) {
			count = i;
			break;
		}
		if (rates[i].idx > rates[i - 1].idx) {
			struct ieee80211_tx_rate tmp = rates[i - 1];
			rates[i - 1] = rates[i];
			rates[i] = tmp;
		}
	}

	/* Eliminate duplicates. */
	total = rates[0].count;
	for (i = 0, j = 1; j < count; ++j) {
		if (rates[j].idx == rates[i].idx) {
			rates[i].count += rates[j].count;
		} else if (rates[j].idx > rates[i].idx) {
			break;
		} else {
			++i;
			if (i != j)
				rates[i] = rates[j];
		}
		total += rates[j].count;
	}
	count = i + 1;

	/* Re-fill policy trying to keep every requested rate and with
	 * respect to the global max tx retransmission count. */
	if (limit < count)
		limit = count;
	if (total > limit) {
		for (i = 0; i < count; ++i) {
			int left = count - i - 1;
			if (rates[i].count > limit - left)
				rates[i].count = limit - left;
			limit -= rates[i].count;
		}
	}

	/* HACK!!! Device has problems (at least) switching from
	 * 54Mbps CTS to 1Mbps. This switch takes enormous amount
	 * of time (100-200 ms), leading to valuable throughput drop.
	 * As a workaround, additional g-rates are injected to the
	 * policy.
	 */
	if (count == 2 && !(rates[0].flags & IEEE80211_TX_RC_MCS) &&
			rates[0].idx > 4 && rates[0].count > 2 &&
			rates[1].idx < 2) {
		/* ">> 1" is an equivalent of "/ 2", but faster */
		int mid_rate = (rates[0].idx + 4) >> 1;

		/* Decrease number of retries for the initial rate */
		rates[0].count -= 2;

		if (mid_rate != 4) {
			/* Keep fallback rate at 1Mbps. */
			rates[3] = rates[1];

			/* Inject 1 transmission on lowest g-rate */
			rates[2].idx = 4;
			rates[2].count = 1;
			rates[2].flags = rates[1].flags;

			/* Inject 1 transmission on mid-rate */
			rates[1].idx = mid_rate;
			rates[1].count = 1;

			/* Fallback to 1 Mbps is a really bad thing,
			 * so let's try to increase probability of
			 * successful transmission on the lowest g rate
			 * even more */
			if (rates[0].count >= 3) {
				--rates[0].count;
				++rates[2].count;
			}

			/* Adjust amount of rates defined */
			count += 2;
		} else {
			/* Keep fallback rate at 1Mbps. */
			rates[2] = rates[1];

			/* Inject 2 transmissions on lowest g-rate */
			rates[1].idx = 4;
			rates[1].count = 2;

			/* Adjust amount of rates defined */
			count += 1;
		}
	}

	policy->defined = atbm_get_tx_rate(hw_priv, &rates[0])->hw_value + 1;

	for (i = 0; i < count; ++i) {
		register unsigned rateid, off, shift, retries;

		rateid = atbm_get_tx_rate(hw_priv, &rates[i])->hw_value;
		off = rateid >> 3;		/* eq. rateid / 8 */
		shift = (rateid & 0x07) << 2;	/* eq. (rateid % 8) * 4 */

		retries = rates[i].count;
		if (unlikely(retries > 0x0F))
			rates[i].count = retries = 0x0F;
		policy->tbl[off] |= __cpu_to_le32(retries << shift);
		policy->retry_count += retries;
	}

	tx_policy_printk( "[TX policy] Policy (%d): " \
		"%d:%d, %d:%d, %d:%d, %d:%d, %d:%d\n",
		count,
		rates[0].idx, rates[0].count,
		rates[1].idx, rates[1].count,
		rates[2].idx, rates[2].count,
		rates[3].idx, rates[3].count,
		rates[4].idx, rates[4].count);
}

static inline bool tx_policy_is_equal(const struct tx_policy *wanted,
					const struct tx_policy *cached)
{
	size_t count = wanted->defined >> 1;
	if (wanted->defined > cached->defined)
		return false;
	if (count) {
		if (memcmp(wanted->raw, cached->raw, count))
			return false;
	}
	if (wanted->defined & 1) {
		if ((wanted->raw[count] & 0x0F) != (cached->raw[count] & 0x0F))
			return false;
	}
	return true;
}

static int tx_policy_find(struct tx_policy_cache *cache,
				const struct tx_policy *wanted)
{
	/* O(n) complexity. Not so good, but there's only 8 entries in
	 * the cache.
	 * Also lru helps to reduce search time. */
	struct tx_policy_cache_entry *it;
	/* Search for policy in "used" list */
	list_for_each_entry(it, &cache->used, link) {
		if (tx_policy_is_equal(wanted, &it->policy))
			return it - cache->cache;
	}
	/* Then - in "free list" */
	list_for_each_entry(it, &cache->free, link) {
		if (tx_policy_is_equal(wanted, &it->policy))
			return it - cache->cache;
	}
	return -1;
}

static inline void tx_policy_use(struct tx_policy_cache *cache,
				 struct tx_policy_cache_entry *entry)
{
	++entry->policy.usage_count;
	list_move(&entry->link, &cache->used);
}

static inline int tx_policy_release(struct tx_policy_cache *cache,
				    struct tx_policy_cache_entry *entry)
{
	int ret = --entry->policy.usage_count;
	if (!ret)
		list_move(&entry->link, &cache->free);
	return ret;
}
/* ******************************************************************** */
/* External TX policy cache API						*/

void tx_policy_init(struct atbm_common *hw_priv)
{
	struct tx_policy_cache *cache = &hw_priv->tx_policy_cache;
	int i;

	memset(cache, 0, sizeof(*cache));

	spin_lock_init(&cache->lock);
	INIT_LIST_HEAD(&cache->used);
	INIT_LIST_HEAD(&cache->free);

	for (i = 0; i < TX_POLICY_CACHE_SIZE; ++i)
		list_add(&cache->cache[i].link, &cache->free);
}
static int tx_policy_get(struct atbm_common *hw_priv,
		  struct ieee80211_tx_rate *rates,
		  size_t count, bool *renew)
{
	int idx;
	struct tx_policy_cache *cache = &hw_priv->tx_policy_cache;
	struct tx_policy wanted;

	tx_policy_build(hw_priv, &wanted, rates, count);

	spin_lock_bh(&cache->lock);
	
	idx = tx_policy_find(cache, &wanted);
	if (idx >= 0) {
		tx_policy_printk( "[TX policy] Used TX policy: %d\n",
					idx);
		*renew = false;
	}
	else if (WARN_ON_ONCE(list_empty(&cache->free))) {
		spin_unlock_bh(&cache->lock);
		return ATBM_APOLLO_INVALID_RATE_ID;
	} else {
		struct tx_policy_cache_entry *entry;
		*renew = true;
		/* If policy is not found create a new one
		 * using the oldest entry in "free" list */
		entry = list_entry(cache->free.prev,
			struct tx_policy_cache_entry, link);
		entry->policy = wanted;
		idx = entry - cache->cache;
		tx_policy_printk( "[TX policy] New TX policy: %d\n",
					idx);
		tx_policy_dump(&entry->policy);
	}
	tx_policy_use(cache, &cache->cache[idx]);
	if (unlikely(list_empty(&cache->free))) {
		/* Lock TX queues. */
		atbm_tx_queues_lock(hw_priv);
	}
	spin_unlock_bh(&cache->lock);

	return idx;
}
static void tx_policy_put(struct atbm_common *hw_priv, int idx)
{

	int usage, locked;
	struct tx_policy_cache *cache = &hw_priv->tx_policy_cache;

	spin_lock_bh(&cache->lock);
	locked = list_empty(&cache->free);
	usage = tx_policy_release(cache, &cache->cache[idx]);
	if (unlikely(locked) && !usage) {
		/* Unlock TX queues. */
		atbm_tx_queues_unlock(hw_priv);
	}
	spin_unlock_bh(&cache->lock);
}
/*
bool tx_policy_cache_full(struct atbm_common *hw_priv)
{
	bool ret;
	struct tx_policy_cache *cache = &hw_priv->tx_policy_cache;
	spin_lock_bh(&cache->lock);
	ret = list_empty(&cache->free);
	spin_unlock_bh(&cache->lock);
	return ret;
}
*/

static int tx_policy_upload(struct atbm_common *hw_priv)
{
	struct tx_policy_cache *cache = &hw_priv->tx_policy_cache;
	int i;
	struct wsm_set_tx_rate_retry_policy arg = {
		.hdr = {
			.numTxRatePolicies = 0,
		}
	};
	int if_id = 0;
	spin_lock_bh(&cache->lock);

	/* Upload only modified entries. */
	for (i = 0; i < TX_POLICY_CACHE_SIZE; ++i) {
		struct tx_policy *src = &cache->cache[i].policy;
		if (src->retry_count && !src->uploaded) {
			struct wsm_set_tx_rate_retry_policy_policy *dst =
				&arg.tbl[arg.hdr.numTxRatePolicies];
			dst->policyIndex = i;
			dst->shortRetryCount =
				hw_priv->short_frame_max_tx_count;
			dst->longRetryCount = hw_priv->long_frame_max_tx_count;

			/* BIT(2) - Terminate retries when Tx rate retry policy
			 *          finishes.
			 * BIT(3) - Count initial frame transmission as part of
			 *          rate retry counting but not as a retry
			 *          attempt */
			dst->policyFlags = BIT(2) | BIT(3);

			memcpy(dst->rateCountIndices, src->tbl,
					sizeof(dst->rateCountIndices));
			src->uploaded = 1;
			++arg.hdr.numTxRatePolicies;
		}
	}
	spin_unlock_bh(&cache->lock);
	atbm_debug_tx_cache_miss(hw_priv);
	tx_policy_printk( "[TX policy] Upload %d policies\n",
				arg.hdr.numTxRatePolicies);
	/*TODO: COMBO*/
	return wsm_set_tx_rate_retry_policy(hw_priv, &arg, if_id);
}
void tx_policy_upload_work(struct atbm_work_struct *work)
{
	struct atbm_common *hw_priv =
		container_of(work, struct atbm_common, tx_policy_upload_work);
	if(atbm_bh_is_term(hw_priv))
		goto out;
	tx_policy_printk( "[TX] TX policy upload\n");
	if(tx_policy_upload(hw_priv)){
		if(!atbm_bh_is_term(hw_priv)){
			WARN_ON(1);
		}
	}
out:
	wsm_unlock_tx(hw_priv);
	atbm_tx_queues_unlock(hw_priv);
}
#endif
/* ******************************************************************** */
/* apollo TX implementation						*/

struct atbm_txinfo {
	struct sk_buff *skb;
	unsigned queue;
	struct ieee80211_tx_info *tx_info;
	const struct ieee80211_rate *rate;
	struct ieee80211_hdr *hdr;
	u32 hdrlen;
	const u8 *da;
	struct atbm_sta_priv *sta_priv;
	struct atbm_txpriv txpriv;
};

u32 atbm_rate_mask_to_wsm(struct atbm_common *hw_priv, u32 rates)
{
	u32 ret = 0;
	int i;
	struct ieee80211_rate * bitrates =
		hw_priv->hw->wiphy->bands[hw_priv->channel->band]->bitrates;
	for (i = 0; i < 32; ++i) {
		if (rates & BIT(i))
			ret |= BIT(bitrates[i].hw_value);
	}
	return ret;
}

static const struct ieee80211_rate *
atbm_get_tx_rate(const struct atbm_common *hw_priv,
		   const struct ieee80211_tx_rate *rate)
{
	if (rate->idx < 0)
		return NULL;
	if (rate->flags & IEEE80211_TX_RC_MCS)
		return &hw_priv->mcs_rates[rate->idx];
	return &hw_priv->hw->wiphy->bands[hw_priv->channel->band]->bitrates[rate->idx];
}
#if (OLD_RATE_POLICY==0)
static const u8
atbm_get_tx_rate_hw_value(const struct atbm_common *hw_priv,
		   const struct ieee80211_tx_rate *rate)
{
	if (rate->idx < 0){
		//WARN_ON(1);
		return 0xff;
	}
	if (rate->flags & IEEE80211_TX_RC_MCS)
		return hw_priv->mcs_rates[rate->idx].hw_value;
	return hw_priv->hw->wiphy->bands[hw_priv->channel->band]->bitrates[rate->idx].hw_value;
}
#endif
static int
atbm_tx_h_calc_link_ids(struct atbm_vif *priv,
			  struct atbm_txinfo *t)
{
	bool timestamp_update = true;
#ifdef CONFIG_ATBM_SUPPORT_P2P
#ifndef P2P_MULTIVIF
	struct atbm_common *hw_priv = ABwifi_vifpriv_to_hwpriv(priv);
	if ((t->tx_info->flags & IEEE80211_TX_CTL_TX_OFFCHAN) ||
			(hw_priv->roc_if_id == priv->if_id))
		t->txpriv.offchannel_if_id = 2;
	else
		t->txpriv.offchannel_if_id = 0;
#endif
#endif
#ifdef CONFIG_ATBM_STA_LISTEN
	if(priv->join_status == ATBM_APOLLO_JOIN_STATUS_STA_LISTEN){
		t->txpriv.raw_link_id = 0;
		t->txpriv.link_id = 0;
	}else
#endif
	if(priv->join_status == ATBM_APOLLO_JOIN_STATUS_SIMPLE_MONITOR){
		t->txpriv.raw_link_id = 0;
		t->txpriv.link_id = 0;
	}
	else if (likely(t->tx_info->control.sta && t->sta_priv->link_id))
		t->txpriv.raw_link_id =
				t->txpriv.link_id =
				t->sta_priv->link_id;
	else if (priv->mode != NL80211_IFTYPE_AP)
		t->txpriv.raw_link_id =
				t->txpriv.link_id = 0;
	else if (is_multicast_ether_addr(t->da)) {
		if (priv->enable_beacon) {
			t->txpriv.raw_link_id = 0;
			t->txpriv.link_id = atbm_dtim_virtual_linkid();
		} else {
			t->txpriv.raw_link_id = 0;
			t->txpriv.link_id = 0;
		}
	} else {
		t->txpriv.link_id =
			atbm_find_link_id(priv, t->da);
		/* Do not assign valid link id for deauth/disassoc frame being
		transmitted to an unassociated STA */
		if (!(t->txpriv.link_id) &&
			(ieee80211_is_deauth(t->hdr->frame_control) ||
			ieee80211_is_disassoc(t->hdr->frame_control) ||
			ieee80211_is_probe_resp(t->hdr->frame_control) ||
			ieee80211_is_action(t->hdr->frame_control) ||
			ieee80211_is_auth(t->hdr->frame_control) ||
			ieee80211_is_assoc_resp(t->hdr->frame_control) ||
			ieee80211_is_reassoc_resp(t->hdr->frame_control))) {
			t->txpriv.link_id = 0;
			atbm_printk_debug("%s:send no id\n",__func__);
		} else {
			if (!t->txpriv.link_id)
				t->txpriv.link_id = atbm_alloc_link_id(priv, t->da);
			if (!t->txpriv.link_id) {
				atbm_printk_err("No more link IDs available.\n");
				return -ENOENT;
			}
		}
		t->txpriv.raw_link_id = t->txpriv.link_id;
		timestamp_update = false;
	}
	if ((t->txpriv.raw_link_id) && (timestamp_update == true))
		priv->link_id_db[t->txpriv.raw_link_id - 1].timestamp =
				jiffies;

	if (t->tx_info->control.sta &&
			(t->tx_info->control.sta->uapsd_queues & BIT(t->queue)))
		t->txpriv.link_id = atbm_uapsd_virtual_linkid();
	return 0;
}

static void
atbm_tx_h_pm(struct atbm_vif *priv,
	       struct atbm_txinfo *t)
{
	if (unlikely(ieee80211_is_auth(t->hdr->frame_control))) {
		u32 mask = ~BIT(t->txpriv.raw_link_id);
		spin_lock_bh(&priv->ps_state_lock);
		priv->sta_asleep_mask &= mask;
		priv->pspoll_mask &= mask;
		spin_unlock_bh(&priv->ps_state_lock);
	}
}

static void
atbm_tx_h_calc_tid(struct atbm_vif *priv,
		     struct atbm_txinfo *t)
{
	if (ieee80211_is_data_qos(t->hdr->frame_control)) {
		u8 *qos = ieee80211_get_qos_ctl(t->hdr);
		t->txpriv.tid = qos[0] & IEEE80211_QOS_CTL_TID_MASK;
	} else if (ieee80211_is_data(t->hdr->frame_control)) {
		t->txpriv.tid = 0;
	}
}

/* IV/ICV injection. */
/* TODO: Quite unoptimal. It's better co modify mac80211
 * to reserve space for IV */
static int
atbm_tx_h_crypt(struct atbm_vif *priv,
		  struct atbm_txinfo *t)
{
	u32 iv_len;
	u32 icv_len;
	u8 *newhdr;	
	struct ieee80211_mmie *mmie;
#ifndef ATBM_11W_TEST
	if(t->tx_info->control.hw_key)
	{
		if (t->tx_info->control.hw_key->cipher == WLAN_CIPHER_SUITE_AES_CMAC){
			atbm_printk_debug("11w WLAN_CIPHER_SUITE_AES_CMAC\n");
			mmie = (struct ieee80211_mmie *) atbm_skb_put(t->skb, sizeof(*mmie));
			memset(mmie,0,sizeof(struct ieee80211_mmie));
			mmie->element_id = WLAN_EID_MMIE;
			mmie->length = sizeof(*mmie) - 2;
			mmie->key_id = cpu_to_le16(t->tx_info->control.hw_key->keyidx);
			t->hdr->duration_id = 0x00;
		}
	}
#else
{
	 struct atbm_ieee80211_mgmt *mgmt = (struct atbm_ieee80211_mgmt *)t->skb->data;
	 if(ieee80211_is_deauth(mgmt->frame_control))
	 {
	 	atbm_printk_debug("da mac %pM,protec(%d)\n",mgmt->da,t->hdr->frame_control&
	     __cpu_to_le32(IEEE80211_FCTL_PROTECTED));
	 	if(!memcmp(mgmt->da,broadcast_addr,sizeof(broadcast_addr)))
	 	{
	 		extern u8 aes_mac_key_index;
	 		atbm_printk_debug("11w WLAN_CIPHER_SUITE_AES_CMAC,key_index(%d)\n",aes_mac_key_index);
			mmie = (struct ieee80211_mmie *) atbm_skb_put(t->skb, sizeof(*mmie));
			memset(mmie,0,sizeof(struct ieee80211_mmie));
			mmie->element_id = WLAN_EID_MMIE;
			mmie->length = sizeof(*mmie) - 2;
			mmie->key_id = aes_mac_key_index;
			t->hdr->frame_control =  t->hdr->frame_control &(~ __cpu_to_le16(IEEE80211_FCTL_PROTECTED));
			t->hdr->duration_id = 0x00;
	 	}
	 }
}

#endif
	
	t->tx_info->sg_tailneed = 0;

	if (!t->tx_info->control.hw_key ||
	    !(t->hdr->frame_control &
	     __cpu_to_le32(IEEE80211_FCTL_PROTECTED)))
		return 0;

	iv_len = t->tx_info->control.hw_key->iv_len;
	icv_len = t->tx_info->control.hw_key->icv_len;

	if (t->tx_info->control.hw_key->cipher == WLAN_CIPHER_SUITE_TKIP)
		icv_len += 8; /* MIC */
	if(!skb_is_nonlinear(t->skb)){
		if ((atbm_skb_headroom(t->skb) + atbm_skb_tailroom(t->skb) <
				 iv_len + icv_len + WSM_TX_EXTRA_HEADROOM) ||
				(atbm_skb_headroom(t->skb) <
				 iv_len + WSM_TX_EXTRA_HEADROOM)) {
			atbm_printk_warn("Bug: no space allocated for crypto headers.\n"
				"headroom: %d, tailroom: %d, "
				"req_headroom: %d, req_tailroom: %d\n"
				"Please fix it in atbm_get_skb().\n",
				atbm_skb_headroom(t->skb), atbm_skb_tailroom(t->skb),
				iv_len + WSM_TX_EXTRA_HEADROOM, icv_len);
			return -ENOMEM;
		} else if (atbm_skb_tailroom(t->skb) < icv_len) {
			size_t offset = icv_len - atbm_skb_tailroom(t->skb);
			u8 *p;
			if(!(t->tx_info->control.hw_key->flags & IEEE80211_KEY_FLAG_ALLOC_IV))
			{
				p = atbm_skb_push(t->skb, offset);
				memmove(p, &p[offset], t->skb->len - offset);
				atbm_skb_trim(t->skb, t->skb->len - offset);
			}
			else
			{
				atbm_printk_warn("Slowpath: tailroom is not big enough. "
				"Req: %d, got: %d.\n",icv_len, atbm_skb_tailroom(t->skb));
				return -ENOMEM;
			}
			
		}
		atbm_skb_put(t->skb, icv_len);
	}else {
		if(atbm_skb_headroom(t->skb) < iv_len + WSM_TX_EXTRA_HEADROOM){
			atbm_printk_warn("Bug: skb_sg_no space allocated for crypto headers.\n"
				"headroom: %d"
				"req_headroom: %d, req_tailroom: %d\n"
				"Please fix it in atbm_get_skb().\n",
				atbm_skb_headroom(t->skb),iv_len + WSM_TX_EXTRA_HEADROOM, icv_len);
			return -ENOMEM;
		}
		t->tx_info->sg_tailneed = icv_len;
	}
	newhdr = atbm_skb_push(t->skb, iv_len);
	memmove(newhdr, newhdr + iv_len, t->hdrlen);
	t->hdr = (struct ieee80211_hdr *) newhdr;
	t->hdrlen += iv_len;
	return 0;
}

static int
atbm_tx_h_align(struct atbm_vif *priv,
		  struct atbm_txinfo *t,
		  u8 *flags)
{
	u32 offset = (unsigned long)(t->skb->data) & (u32)3;

	if (!offset)
		return 0;

	if (offset & 1) {
		atbm_printk_err(
			"Bug: attempt to transmit a frame "
			"with wrong alignment: %d\n",
			offset);
		return -EINVAL;
	}

	if (atbm_skb_headroom(t->skb) < offset) {
		atbm_printk_err(
			"Bug: no space allocated "
			"for DMA alignment.\n"
			"headroom: %d\n",
			atbm_skb_headroom(t->skb));
		return -ENOMEM;
	}
	atbm_skb_push(t->skb, offset);
	t->hdrlen += offset;
	t->txpriv.offset += offset;
	*flags |= WSM_TX_2BYTES_SHIFT;
	atbm_debug_tx_align(priv);
	return 0;
}
#ifdef CONFIG_ATBM_DRIVER_PROCESS_BA
static int
atbm_tx_h_action(struct atbm_vif *priv,
		   struct atbm_txinfo *t)
{
	struct atbm_ieee80211_mgmt *mgmt =
		(struct atbm_ieee80211_mgmt *)t->hdr;

	if (ieee80211_is_action(t->hdr->frame_control) &&
			mgmt->u.action.category == ATBM_WLAN_CATEGORY_BACK)
		return 1;
	else
		return 0;
}
#endif
/* Add WSM header */
static struct wsm_tx *
atbm_tx_h_wsm(struct atbm_vif *priv,
		struct atbm_txinfo *t)
{
	struct wsm_tx *wsm;

	if (atbm_skb_headroom(t->skb) < sizeof(struct wsm_tx)) {
		atbm_printk_err(
			"Bug: no space allocated "
			"for WSM header.\n"
			"headroom: %d\n",
			atbm_skb_headroom(t->skb));
		return NULL;
	}
	wsm = (struct wsm_tx *)atbm_skb_push(t->skb, sizeof(struct wsm_tx));
	t->txpriv.offset += sizeof(struct wsm_tx);
	memset(wsm, 0, sizeof(*wsm));
#ifdef USB_BUS_BUG
	if(t->skb->len + t->tx_info->sg_tailneed < 1538)
		wsm->hdr.usb_len = __cpu_to_le16(1538);//__cpu_to_le16(t->skb->len);
	else 		
		wsm->hdr.usb_len = __cpu_to_le16(t->skb->len + t->tx_info->sg_tailneed);
	wsm->hdr.usb_id = __cpu_to_le16(WSM_TRANSMIT_REQ_MSG_ID);
#endif
	wsm->hdr.len = __cpu_to_le16(t->skb->len+t->tx_info->sg_tailneed);
	wsm->hdr.id = __cpu_to_le16(WSM_TRANSMIT_REQ_MSG_ID);
	wsm->queueId =
		(t->txpriv.raw_link_id << 2) | wsm_queue_id_to_wsm(t->queue);
	return wsm;
}
#ifdef CONFIG_ATBM_BT_COMB
/* BT Coex specific handling */
static void
atbm_tx_h_bt(struct atbm_vif *priv,
	       struct atbm_txinfo *t,
	       struct wsm_tx *wsm)
{
	struct atbm_common *hw_priv = ABwifi_vifpriv_to_hwpriv(priv);

	u8 priority = 0;

	if (!hw_priv->is_BT_Present)
		return;

	if (unlikely(ieee80211_is_nullfunc(t->hdr->frame_control)))
		priority = WSM_EPTA_PRIORITY_MGT;
	else if (ieee80211_is_data(t->hdr->frame_control)) {
		/* Skip LLC SNAP header (+6) */
		u8 *payload = &t->skb->data[t->hdrlen];
		u16 *ethertype = (u16 *) &payload[6];
		if (unlikely(*ethertype == __be16_to_cpu(ETH_P_PAE)))
			priority = WSM_EPTA_PRIORITY_EAPOL;
	} else if (unlikely(ieee80211_is_assoc_req(t->hdr->frame_control) ||
		ieee80211_is_reassoc_req(t->hdr->frame_control))) {
		struct atbm_ieee80211_mgmt *mgt_frame =
				(struct atbm_ieee80211_mgmt *)t->hdr;

		if (mgt_frame->u.assoc_req.listen_interval <
						priv->listen_interval) {
			txrx_printk(
				"Modified Listen Interval to %d from %d\n",
				priv->listen_interval,
				mgt_frame->u.assoc_req.listen_interval);
			/* Replace listen interval derieved from
			 * the one read from SDD */
			mgt_frame->u.assoc_req.listen_interval =
				priv->listen_interval;
		}
	}

	if (likely(!priority)) {
		if (ieee80211_is_action(t->hdr->frame_control))
			priority = WSM_EPTA_PRIORITY_ACTION;
		else if (ieee80211_is_mgmt(t->hdr->frame_control))
			priority = WSM_EPTA_PRIORITY_MGT;
		else if ((wsm->queueId == WSM_QUEUE_VOICE))
			priority = WSM_EPTA_PRIORITY_VOICE;
		else if ((wsm->queueId == WSM_QUEUE_VIDEO))
			priority = WSM_EPTA_PRIORITY_VIDEO;
		else
			priority = WSM_EPTA_PRIORITY_DATA;
	}

	txrx_printk( "[TX] EPTA priority %d.\n",
		priority);

	wsm->flags |= priority << 1;
}
#endif
#if defined(CONFIG_NL80211_TESTMODE) || defined(CONFIG_ATBM_IOCTRL)

extern int atbm_tool_shortGi;
#endif
#if defined (CONFIG_RATE_HW_CONTROL)
static inline int atbm_rate_supported(struct ieee80211_sta *sta,
				 enum ieee80211_band band,
				 int index)
{
	return (sta == NULL || sta->supp_rates[band] & BIT(index));
}

static inline s8
atbm_rate_lowest_index(struct ieee80211_supported_band *sband,
		  struct ieee80211_sta *sta)
{
	int i;

	for (i = 0; i < sband->n_bitrates; i++)
		if (atbm_rate_supported(sta, sband->band, i))
			return i;

	/* warn when we cannot find a rate. */
	WARN_ON_ONCE(1);

	/* and return 0 (the lowest index) */
	return 0;
}
static inline s8
atbm_rate_lowest_non_cck_index(struct ieee80211_supported_band *sband,
			  struct ieee80211_sta *sta)
{
	int i;

	for (i = 0; i < sband->n_bitrates; i++) {
		struct ieee80211_rate *srate = &sband->bitrates[i];
		if ((srate->bitrate == 10) || (srate->bitrate == 20) ||
		    (srate->bitrate == 55) || (srate->bitrate == 110))
			continue;

		if (atbm_rate_supported(sta, sband->band, i))
			return i;
	}

	/* No matching rate found */
	return 0;
}
static bool atbm_rate_control_send_low(struct atbm_common *hw_priv,struct atbm_txinfo *t)
{
	struct ieee80211_supported_band *sband=hw_priv->hw->wiphy->bands[hw_priv->channel->band];
	struct ieee80211_tx_info *tx_info = t->tx_info;
	struct sta_info *info_sta = NULL;
	bool assoc = false;

	if(tx_info->control.sta)
		info_sta = container_of(tx_info->control.sta, struct sta_info, sta);
	if(info_sta)
		assoc = test_sta_flag(info_sta,WLAN_STA_ASSOC) ? true:false;
	
	if(!ieee80211_is_data(t->hdr->frame_control)){
		tx_info->flags |= IEEE80211_TX_CTL_USE_MINRATE;
	}

	if(ieee80211_is_nullfunc(t->hdr->frame_control)){
		tx_info->flags |= IEEE80211_TX_CTL_USE_MINRATE;
	}
	if(ieee80211_is_qos_nullfunc(t->hdr->frame_control)){
		tx_info->flags |= IEEE80211_TX_CTL_USE_MINRATE;
	}
	
	if((assoc == false)||
		(tx_info->flags&IEEE80211_TX_CTL_USE_MINRATE)||
		(tx_info->flags&IEEE80211_TX_CTL_NO_CCK_RATE)){
		if(!(tx_info->flags&IEEE80211_TX_CTL_NO_CCK_RATE)){
			tx_info->control.rates[0].idx = atbm_rate_lowest_index(sband, tx_info->control.sta);
		}else {
			tx_info->control.rates[0].idx = atbm_rate_lowest_non_cck_index(sband, t->tx_info->control.sta);
		}
	   	tx_info->control.rates[0].count = (tx_info->flags & IEEE80211_TX_CTL_NO_ACK) ?1 : 5/*only for test*/;

		tx_info->flags |= IEEE80211_TX_CTL_USE_MINRATE;
		/*
		*if we are station mode and not associate with the ap
		*check the rate according to the ap basic rate
		*/
		while((t->tx_info->control.vif->type == NL80211_IFTYPE_STATION)&&
		   (ieee80211_is_mgmt(t->hdr->frame_control))){
		   struct cfg80211_bss *cbss = NULL;
		   struct ieee80211_bss *bss = NULL;
		   int i = 0;
		   int j = 0;
		   u32 suport_rates = 0;
		   int min_rate = INT_MAX, min_rate_index = -1;
		   
		   cbss = ieee80211_atbm_get_bss(hw_priv->hw->wiphy,hw_priv->channel,t->hdr->addr1,NULL,0,0,0);

		   if(cbss == NULL){
		   		break;
		   }

		   bss = (struct ieee80211_bss *)cbss->priv;

		   if(bss == NULL){
				ieee80211_atbm_put_bss(hw_priv->hw->wiphy,cbss);
		   		break;
		   }
		   atbm_printk_debug("%s:supp_rates_len(%zu),rate_index(%d)\n",__func__,bss->supp_rates_len,tx_info->control.rates[0].idx);
		  	for (i = 0; i < bss->supp_rates_len; i++) {
				int rate = (bss->supp_rates[i] & 0x7f) * 5;
		
				for (j = 0; j < sband->n_bitrates; j++) {
					if (sband->bitrates[j].bitrate == rate) {
						suport_rates |= BIT(j);
						
						if (rate < min_rate) {
							min_rate = rate;
							min_rate_index = j;
						}
						break;
					}
				}
			}
			
			atbm_printk_debug("%s:suport_rates(%x),rate_index(%d)\n",__func__,suport_rates,tx_info->control.rates[0].idx);
			if(suport_rates == 0){
				ieee80211_atbm_put_bss(hw_priv->hw->wiphy,cbss);
				break;
			}
		    if(!(suport_rates&BIT(tx_info->control.rates[0].idx))){
				for(i = 0; i < IEEE80211_TX_MAX_RATES; i++) {
					tx_info->control.rates[i].idx = -1;
					tx_info->control.rates[i].flags = 0;
					tx_info->control.rates[i].count = 1;
				}

				tx_info->control.rates[0].idx = min_rate_index;
				tx_info->control.rates[0].count = 5;
		    }
			ieee80211_atbm_put_bss(hw_priv->hw->wiphy,cbss);
			break;
		}
		return true;
	}

	return false;
}
static int atbm_tx_h_rate_policy(struct atbm_common *hw_priv,
			struct atbm_txinfo *t,
			struct wsm_tx *wsm)
{
	struct ieee80211_supported_band *sband=hw_priv->hw->wiphy->bands[hw_priv->channel->band];
	struct ieee80211_tx_info *tx_info = t->tx_info;
	struct atbm_vif *priv = ABwifi_get_vif_from_ieee80211(t->tx_info->control.vif);
	int i = 0;
	
	t->txpriv.rate_id = 0;
	wsm->flags |= t->txpriv.rate_id << 4;
	/*
	*clear rate control
	*/
	for (i = 0; i < IEEE80211_TX_MAX_RATES; i++) {
		tx_info->control.rates[i].idx = -1;
		tx_info->control.rates[i].flags = 0;
		tx_info->control.rates[i].count = 1;
	}
	
	if(atbm_rate_control_send_low(hw_priv,t) == false){
		
		if(tx_info->control.sta){
			u32 rate_mask = 0;
			/*
			*ht_supported is true,get 11n rate,others 11g rate
			*/
			if((tx_info->control.sta->ht_cap.ht_supported == true)&&
			   (tx_info->control.sta->ht_cap.mcs.rx_mask[0])){
				/*
				*only surport one stream
				*/
				if(tx_info->control.sta->ht_cap.mcs.rx_mask[0] == 0xff){
					tx_info->control.rates[0].idx = 7;
				}else {
					rate_mask = tx_info->control.sta->ht_cap.mcs.rx_mask[0];
					tx_info->control.rates[0].idx = fls(rate_mask)-1;
				}
				atbm_printk_debug("%s:last set (%d)\n",__func__,fls(tx_info->control.sta->ht_cap.mcs.rx_mask[0])-1);
				tx_info->control.rates[0].flags = IEEE80211_TX_RC_MCS;
				tx_info->control.rates[0].count = 10;
#ifdef CONFIG_LMAC_RATECTRL_RATE_BUG_FIX
				if(hweight8(tx_info->control.sta->ht_cap.mcs.rx_mask[0]) <= 3){
					tx_info->flags |= IEEE80211_TX_CTL_USE_MINRATE;
				}
#endif
			}else if(tx_info->control.sta->supp_rates[hw_priv->channel->band]){
				rate_mask = tx_info->control.sta->supp_rates[hw_priv->channel->band];
				tx_info->control.rates[0].idx = fls(rate_mask)-1;
				tx_info->control.rates[0].flags = 0;
				tx_info->control.rates[0].count = 10;
				
				atbm_printk_debug("%s:last set (%d)\n",__func__,fls(rate_mask)-1);
#ifdef CONFIG_LMAC_RATECTRL_RATE_BUG_FIX
				if(hweight32(tx_info->control.sta->supp_rates[hw_priv->channel->band]) < 5){
					tx_info->flags |= IEEE80211_TX_CTL_USE_MINRATE;
				}
#endif
			}else {
				tx_info->control.rates[0].idx = 0;
				tx_info->flags |= IEEE80211_TX_CTL_USE_MINRATE;
			}
		}else {
			/*
			*no sta ,get lower rate
			*/
			if(priv->vif->p2p == false){
				tx_info->control.rates[0].idx = atbm_rate_lowest_index(sband,tx_info->control.sta);
			}else {
				tx_info->control.rates[0].idx = atbm_rate_lowest_non_cck_index(sband,tx_info->control.sta);
			}
			tx_info->flags |= IEEE80211_TX_CTL_USE_MINRATE;
		}
		
	}

	t->rate = atbm_get_tx_rate(hw_priv,
		&t->tx_info->control.rates[0]);
	wsm->maxTxRate = t->rate->hw_value;

	if(t->tx_info->flags&IEEE80211_TX_CTL_USE_MINRATE){
		wsm->htTxParameters |= __cpu_to_le32(WSM_HT_TX_USE_MINRATE);
	}

	if (t->rate->flags & IEEE80211_TX_RC_MCS) {
#ifndef CONFIG_ATBM_APOLLO_TESTMODE
		if (priv->association_mode.greenfieldMode)
			wsm->htTxParameters |=
				__cpu_to_le32(WSM_HT_TX_GREENFIELD);
		else
			wsm->htTxParameters |=
				__cpu_to_le32(WSM_HT_TX_MIXED);

#else
		if (t->tx_info->control.rates[0].flags & IEEE80211_TX_RC_GREEN_FIELD)
		{
			wsm->htTxParameters |=
				__cpu_to_le32(WSM_HT_TX_GREENFIELD);
			priv->association_mode.greenfieldMode = 1;
		}
		else
		{
			wsm->htTxParameters |=
				__cpu_to_le32(WSM_HT_TX_MIXED);
			priv->association_mode.greenfieldMode = 0;
		}
#endif //CONFIG_ATBM_APOLLO_TESTMODE
		if ((t->tx_info->control.rates[0].flags & IEEE80211_TX_RC_SHORT_GI)
#if defined(CONFIG_NL80211_TESTMODE) || defined(CONFIG_ATBM_IOCTRL)
			||(atbm_tool_shortGi == 1)
#endif
			)//IEEE80211_TX_RC_SHORT_GI
		{
			if(wsm->htTxParameters&(__cpu_to_le32(WSM_HT_TX_MIXED)))
				wsm->htTxParameters |=
					__cpu_to_le32(WSM_HT_TX_SGI);
		}
	}

	return 0;
}
#else
static int
atbm_tx_h_rate_policy(struct atbm_common *hw_priv,
			struct atbm_txinfo *t,
			struct wsm_tx *wsm)
{

	bool tx_policy_renew = false;
	struct atbm_vif *priv =
				ABwifi_get_vif_from_ieee80211(t->tx_info->control.vif);
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
	static unsigned long g_printf_time = 0;
	if (!g_printf_time)
	{
		g_printf_time= jiffies;
	}
#endif //CONFIG_ATBM_APOLLO_TX_POLICY_DEBUG
	if (!hw_priv->channel)
	{
		return -EFAULT;
	}


#if OLD_RATE_POLICY
	t->txpriv.rate_id = tx_policy_get(hw_priv,
		t->tx_info->control.rates, IEEE80211_TX_MAX_RATES,
		&tx_policy_renew);
	if (t->txpriv.rate_id == ATBM_APOLLO_INVALID_RATE_ID)
		return -EFAULT;
#else //OLD_RATE_POLICY
	t->txpriv.rate_id = 0;
#endif //OLD_RATE_POLICY

	wsm->flags |= t->txpriv.rate_id << 4;
	t->rate = atbm_get_tx_rate(hw_priv,
		&t->tx_info->control.rates[0]);
	wsm->maxTxRate = t->rate->hw_value;
	//printk("recovery wsm->TxRateRetry[0] %d\n",wsm->maxTxRate);
#if (OLD_RATE_POLICY==0)
	wsm->TxRateRetry[0]= atbm_get_tx_rate_hw_value(hw_priv,
		&t->tx_info->control.rates[1]);
	//printk("1wsm->TxRateRetry[0] %d frame_control %x\n",wsm->TxRateRetry[0],t->hdr->frame_control);
	if(wsm->TxRateRetry[0] == 0xff){
		//printk("recovery wsm->TxRateRetry[0] %d\n",wsm->maxTxRate);
		wsm->TxRateRetry[0] = wsm->maxTxRate;
		wsm->TxRateRetry[1] = wsm->maxTxRate;
		wsm->TxRateRetry[2] = wsm->maxTxRate;
		wsm->TxRateRetry[3] = wsm->maxTxRate;
		goto __rate_set_end;
	}
	wsm->TxRateRetry[1]= atbm_get_tx_rate_hw_value(hw_priv,
		&t->tx_info->control.rates[2]);
	//printk("1wsm->TxRateRetry[1] %d\n",wsm->TxRateRetry[1]);
	if(wsm->TxRateRetry[1] == 0xff){
		//printk("recovery wsm->TxRateRetry[1] %d\n",wsm->TxRateRetry[0]);
		wsm->TxRateRetry[1] = wsm->TxRateRetry[0];
		wsm->TxRateRetry[2] = wsm->TxRateRetry[0];
		wsm->TxRateRetry[3] = wsm->TxRateRetry[0];
		goto __rate_set_end;
	}
	wsm->TxRateRetry[2]= atbm_get_tx_rate_hw_value(hw_priv,
		&t->tx_info->control.rates[3]);
	//printk("1wsm->TxRateRetry[2] %d\n",wsm->TxRateRetry[2]);
	if(wsm->TxRateRetry[2] == 0xff){
		//printk("recovery wsm->TxRateRetry[2] %d\n",wsm->TxRateRetry[1]);
		wsm->TxRateRetry[2] = wsm->TxRateRetry[1];
		wsm->TxRateRetry[3] = wsm->TxRateRetry[1];
		goto __rate_set_end;
	}
	wsm->TxRateRetry[3]= atbm_get_tx_rate_hw_value(hw_priv,
		&t->tx_info->control.rates[4]);
	//printk("1wsm->TxRateRetry[3] %d\n",wsm->TxRateRetry[3]);
	if(wsm->TxRateRetry[3] == 0xff){
		//printk("recovery wsm->TxRateRetry[3] %d\n",wsm->TxRateRetry[2]);
		wsm->TxRateRetry[3] = wsm->TxRateRetry[2];
	}
__rate_set_end:
#endif //(OLD_RATE_POLICY==0)
	if(t->tx_info->flags&IEEE80211_TX_CTL_USE_MINRATE){
		wsm->htTxParameters |= __cpu_to_le32(WSM_HT_TX_USE_MINRATE);
	}
	if (t->rate->flags & IEEE80211_TX_RC_MCS) {
#ifndef CONFIG_ATBM_APOLLO_TESTMODE
		if (priv->association_mode.greenfieldMode)
			wsm->htTxParameters |=
				__cpu_to_le32(WSM_HT_TX_GREENFIELD);
		else
			wsm->htTxParameters |=
				__cpu_to_le32(WSM_HT_TX_MIXED);

#else
		if (t->tx_info->control.rates[0].flags & IEEE80211_TX_RC_GREEN_FIELD)
		{
			wsm->htTxParameters |=
				__cpu_to_le32(WSM_HT_TX_GREENFIELD);
			priv->association_mode.greenfieldMode = 1;
		}
		else
		{
			wsm->htTxParameters |=
				__cpu_to_le32(WSM_HT_TX_MIXED);
			priv->association_mode.greenfieldMode = 0;
		}
#endif //CONFIG_ATBM_APOLLO_TESTMODE
		if ((t->tx_info->control.rates[0].flags & IEEE80211_TX_RC_SHORT_GI)
#if defined(CONFIG_NL80211_TESTMODE) || defined(CONFIG_ATBM_IOCTRL)
			||(atbm_tool_shortGi == 1)
#endif
			)//IEEE80211_TX_RC_SHORT_GI
		{
			if(wsm->htTxParameters&(__cpu_to_le32(WSM_HT_TX_MIXED)))
				wsm->htTxParameters |=
					__cpu_to_le32(WSM_HT_TX_SGI);
		}
	}
#ifdef CONFIG_ATBM_APOLLO_TESTMODE1
	if(time_is_before_jiffies(g_printf_time+5*HZ)){
		atbm_printk_always("rate = %d, flags = %d\n", t->rate->bitrate, t->rate->flags);
		atbm_printk_always("wsm->maxTxRate = %d\n", wsm->maxTxRate);
		atbm_printk_always("greenfieldMode = %s,", (priv->association_mode.greenfieldMode)?"greenfield":"mixed");
		atbm_printk_always("%s GI\n", (t->tx_info->control.rates[0].flags & IEEE80211_TX_RC_SHORT_GI)?"short":"long");
		atbm_printk_always("queuePending :[VO]<%d>[Vi]<%d>[BE]<%d>[BK]<%d> \n",
			 hw_priv->tx_queue[0].num_pending,
			 hw_priv->tx_queue[1].num_pending,
			 hw_priv->tx_queue[2].num_pending,
		   	 hw_priv->tx_queue[3].num_pending);
		g_printf_time= jiffies;
	}
#endif //CONFIG_ATBM_APOLLO_TESTMODE


#if OLD_RATE_POLICY
	if (tx_policy_renew) {
		tx_policy_printk( "[TX] TX policy renew.\n");
		/* It's not so optimal to stop TX queues every now and then.
		 * Maybe it's better to reimplement task scheduling with
		 * a counter. */
		/* atbm_tx_queues_lock(priv); */
		/* Definetly better. TODO. */
		wsm_lock_tx_async(hw_priv);
		atbm_tx_queues_lock(hw_priv);
		if (atbm_hw_priv_queue_work(hw_priv,
				&hw_priv->tx_policy_upload_work) <= 0) {
			atbm_tx_queues_unlock(hw_priv);
			wsm_unlock_tx(hw_priv);
		}
	}
#endif//#if OLD_RATE_POLICY
	return 0;
}
#endif
static bool
atbm_tx_h_pm_state(struct atbm_vif *priv,
		     struct atbm_txinfo *t)
{
	int was_buffered = 1;

	if (t->txpriv.link_id == atbm_dtim_virtual_linkid() &&
			!priv->buffered_multicasts) {
		priv->buffered_multicasts = true;
		if (priv->sta_asleep_mask)
			atbm_hw_priv_queue_work(priv->hw_priv,
				&priv->multicast_start_work);
	}

	if (t->txpriv.raw_link_id && t->txpriv.tid < ATBM_APOLLO_MAX_TID)
		was_buffered = priv->link_id_db[t->txpriv.raw_link_id - 1].buffered[t->txpriv.tid]++;

	return !was_buffered;
}
#ifdef CONFIG_ATBM_BA_STATUS
static void
atbm_tx_h_ba_stat(struct atbm_vif *priv,
		    struct atbm_txinfo *t)
{
	struct atbm_common *hw_priv = priv->hw_priv;

	if (priv->join_status != ATBM_APOLLO_JOIN_STATUS_STA)
		return;
	if (!atbm_is_ht(&hw_priv->ht_info))
		return;
	if (!priv->setbssparams_done)
		return;
	if (!ieee80211_is_data(t->hdr->frame_control))
		return;

	spin_lock_bh(&hw_priv->ba_lock);
	hw_priv->ba_acc += t->skb->len - t->hdrlen;
	if (!(hw_priv->ba_cnt_rx || hw_priv->ba_cnt)) {
		atbm_mod_timer(&hw_priv->ba_timer,
			jiffies + ATBM_APOLLO_BLOCK_ACK_INTERVAL);
	}
	hw_priv->ba_cnt++;
	spin_unlock_bh(&hw_priv->ba_lock);
}
#endif
static int
atbm_tx_h_skb_pad(struct atbm_common *priv,
		    struct wsm_tx *wsm,
		    struct sk_buff *skb)
{
	u32 len = __le16_to_cpu(wsm->hdr.len);
	u32 padded_len = priv->sbus_ops->align_size(priv->sbus_priv, len);

	if (WARN_ON(atbm_skb_padto(skb, padded_len) != 0)) {
		return -EINVAL;
	}
	return 0;
}
/* ******************************************************************** */
#if 0
extern 	int atbm_tcp_ack_offload(struct atbm_queue *queue,struct atbm_txpriv *txpriv,struct sk_buff *skb_new);
#endif
static void atbm_tx_hif_xmit(struct atbm_common *hw_priv)
{
	if(!hw_priv->sbus_ops->sbus_data_write){
		atbm_bh_wakeup(hw_priv);
	}else {
		hw_priv->sbus_ops->sbus_data_write(hw_priv->sbus_priv);
	}
}
void __atbm_tx(struct ieee80211_hw *dev, struct sk_buff *skb)
{
	struct atbm_common *hw_priv = dev->priv;
	struct atbm_txinfo t = {
		.skb = skb,
		.queue = skb_get_queue_mapping(skb),
		.tx_info = IEEE80211_SKB_CB(skb),
		.hdr = (struct ieee80211_hdr *)skb->data,
		.txpriv.tid = ATBM_APOLLO_MAX_TID,
		.txpriv.rate_id = ATBM_APOLLO_INVALID_RATE_ID,
#ifdef P2P_MULTIVIF
		.txpriv.raw_if_id = 0,
#endif
	};
	struct ieee80211_sta *sta;
	struct wsm_tx *wsm;
	bool tid_update = 0;
	u8 flags = 0;
	int ret;
	struct atbm_vif *priv;
	struct ieee80211_hdr *frame = (struct ieee80211_hdr *)skb->data;
#ifdef CONFIG_ATBM_SUPPORT_P2P
    struct atbm_ieee80211_mgmt *mgmt = (struct atbm_ieee80211_mgmt *)skb->data;
#endif
	if (!skb->data){
		atbm_printk_err("%s %d ,ERROR !!! skb->data is NULL\n",__func__,__LINE__);
		return;
		//BUG_ON(1);
	}
	if (!(t.tx_info->control.vif)) {
			atbm_printk_err("[TX]:vif is NULL\n");
	        goto drop;
	}
	priv = ABwifi_get_vif_from_ieee80211(t.tx_info->control.vif);
	if (!priv)
		goto drop;
	if (atomic_read(&priv->enabled) == 0)
		goto drop;

#ifdef CONFIG_ATBM_APOLLO_TESTMODE
	if (t.queue < 4)
	{

		spin_lock_bh(&hw_priv->tsm_lock);

		hw_priv->atbm_tsm_stats[t.queue].tid = t.queue;
		hw_priv->atbm_tsm_stats[t.queue].txed_msdu_count++;
		spin_unlock_bh(&hw_priv->tsm_lock);
	}
#endif /*CONFIG_ATBM_APOLLO_TESTMODE*/
#ifdef ICMP_HIGH_PRIO
	if(ieee80211_is_data_qos(frame->frame_control))
	{
		struct iphdr *skbiphdr = NULL;
		int iphdr_index = 0;
		iphdr_index = ieee80211_hdrlen(frame->frame_control)+8;
		skbiphdr = (struct iphdr *)(skb->data+iphdr_index);

		if(skbiphdr->protocol == 1)
		{
			static int send_rate[]={4,8,8,10,10,10,10,10};
			u8 *p = ieee80211_get_qos_ctl(frame);
			u8 ack_policy, tid;
			u8 index = 0;
			t.queue = 1;
			skb->priority = 5;
			tid = skb->priority & IEEE80211_QOS_CTL_TAG1D_MASK;
			ack_policy = *p & 0x0078;
			*p++ = ack_policy | tid;
			for(index = 0;index < IEEE80211_TX_MAX_RATES;index++)
			{
				if((t.tx_info->control.rates[index].flags&IEEE80211_TX_RC_MCS)
				    &&
				    (t.tx_info->control.rates[index].idx != -1)
				    &&
				    (t.tx_info->control.rates[index].idx <= 7)
				)
				{
					u8 tmp_rate_index = t.tx_info->control.rates[index].idx;
					t.tx_info->control.rates[index].flags &= ~IEEE80211_TX_RC_MCS;
					t.tx_info->control.rates[index].idx = send_rate[tmp_rate_index];
					t.tx_info->control.rates[index].count = 10;
				}
			}
		}
	}
#endif

#ifdef CONFIG_ATBM_SUPPORT_P2P
	if ((ieee80211_is_action(frame->frame_control))
                        && (mgmt->u.action.category == ATBM_WLAN_CATEGORY_PUBLIC)) {
                u8 *action = (u8*)&mgmt->u.action.category;				
				TxRxPublicActionFrame((u8*)mgmt,skb->len,1);
                atbm_check_go_neg_conf_success(hw_priv, action);
				atbm_check_prov_desc_req(hw_priv, action);
        }
#ifdef ATBM_P2P_CHANGE
	atbm_parase_p2p_action_frame(priv,skb,true);
#endif
#endif
#ifdef ATBM_11W_TEST
	if(ieee80211_is_deauth(frame->frame_control))
	{
		memcpy(mgmt->da,broadcast_addr,6);
		atbm_printk_debug("ATBM_11W_TEST\n");
	}
#endif
#ifdef USB_BUS
	if(atomic_xchg(&hw_priv->bh_suspend_usb, 0)){
		extern 	int atbm_usb_pm_async(struct sbus_priv *self, bool	auto_suspend);
		atbm_usb_pm_async(hw_priv->sbus_priv, false);
	}
#endif
/*
* if the suppliacant and hostapd is in connecting state,
* systerm can not goto suspend.
*/
#ifdef CONFIG_PM
	if (ieee80211_is_auth(frame->frame_control)){
		atbm_printk_pm("[TX]:authen, delay suspend\n");
		atbm_pm_stay_awake(&hw_priv->pm_state, 5*HZ);
	}
#endif
	if(ieee80211_is_deauth(frame->frame_control)){
		atbm_printk_pm("[TX]:[%d]:deatuhen[%pM]\n",priv->if_id,ieee80211_get_DA(t.hdr));
	}

	t.txpriv.if_id = priv->if_id;
	t.hdrlen = ieee80211_hdrlen(t.hdr->frame_control);
	t.da = ieee80211_get_DA(t.hdr);
	t.sta_priv =
		(struct atbm_sta_priv *)&t.tx_info->control.sta->drv_priv;

	if (WARN_ON(t.queue >= 4))
		goto drop;
#ifdef MINSTREL_RSSI_USED //disable UDP drop when buffer fulled, debug UDP drop case...
    //#if 0, #endif
#else
	#if 0
  	#ifndef ATBM_WIFI_QUEUE_LOCK_BUG
	spin_lock_bh(&hw_priv->tx_queue[t.queue].lock);

	if ((priv->if_id == 0) &&
		(hw_priv->tx_queue[t.queue].num_queued_vif[0] >=
			hw_priv->vif0_throttle)) {
		spin_unlock_bh(&hw_priv->tx_queue[t.queue].lock);
		{
			extern unsigned long  atbm_queue_ttl(struct atbm_queue *queue);
		
			//printk("%s:drop %d,ttl %ld ms\n",__func__,__LINE__,atbm_queue_ttl(&hw_priv->tx_queue[t.queue]));
		}
		goto drop;
	} else if ((priv->if_id == 1) &&
		(hw_priv->tx_queue[t.queue].num_queued_vif[1] >=
			hw_priv->vif1_throttle)) {
		spin_unlock_bh(&hw_priv->tx_queue[t.queue].lock);
		goto drop;
	}

	spin_unlock_bh(&hw_priv->tx_queue[t.queue].lock);
   #endif
   #endif
#endif
	ret = atbm_tx_h_calc_link_ids(priv, &t);
	if (ret)
		goto drop;

	/*printk("=======>[TX] TX[fc=%x] %d bytes (if_id: %d,"
			" queue: %d, link_id: %d (%d)).\n",
			t.hdr->frame_control,skb->len, priv->if_id, t.queue, t.txpriv.link_id,
			t.txpriv.raw_link_id);
	*/

	atbm_tx_h_pm(priv, &t);
	atbm_tx_h_calc_tid(priv, &t);
	ret = atbm_tx_h_crypt(priv, &t);
	if (ret)
		goto drop;
	ret = atbm_tx_h_align(priv, &t, &flags);
	if (ret)
		goto drop;
#ifdef CONFIG_ATBM_DRIVER_PROCESS_BA
	ret = atbm_tx_h_action(priv, &t);
	if (ret)
		goto drop;
#endif
	wsm = atbm_tx_h_wsm(priv, &t);
	if (!wsm) {
		ret = -ENOMEM;
		goto drop;
	}
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
	flags |= WSM_TX_FLAG_EXPIRY_TIME;
#endif /*CONFIG_ATBM_APOLLO_TESTMODE*/
	wsm->flags |= flags;
#ifdef CONFIG_ATBM_BT_COMB
	atbm_tx_h_bt(priv, &t, wsm);
#endif
	ret = atbm_tx_h_rate_policy(hw_priv, &t, wsm);
	if (ret){
		ret = -1;
		goto drop;
	}
#if defined (CONFIG_TX_NO_CONFIRM)||defined(ATBM_SDIO_PATCH)
	wsm->htTxParameters &= ~(__cpu_to_le32(WSM_NEED_TX_CONFIRM));
	if((IEEE80211_TX_CTL_REQ_TX_STATUS |IEEE80211_TX_CTL_INJECTED)&t.tx_info->flags){
		wsm->htTxParameters |=__cpu_to_le32(WSM_NEED_TX_CONFIRM);
		atbm_printk_tx("tx_status(%d),ctl_injected(%d)\n",!!(t.tx_info->flags&IEEE80211_TX_CTL_REQ_TX_STATUS),
				!!(t.tx_info->flags&IEEE80211_TX_CTL_INJECTED));
	}else if(skb->protocol == cpu_to_be16(ETH_P_PAE)){
		wsm->htTxParameters |=__cpu_to_le32(WSM_NEED_TX_CONFIRM);
		atbm_printk_tx("[TX]:ETH_P_PAE(%p)\n",wsm);
	}else if(!ieee80211_is_data(t.hdr->frame_control)){
		wsm->htTxParameters |=__cpu_to_le32(WSM_NEED_TX_CONFIRM);
		atbm_printk_tx("[TX]:not data(%p)(%d)\n",wsm,wsm->htTxParameters&__cpu_to_le32(WSM_NEED_TX_CONFIRM));
	}else if(ieee80211_is_nullfunc(t.hdr->frame_control)){
		atbm_printk_tx("[TX]:null data\n");
		wsm->htTxParameters |=__cpu_to_le32(WSM_NEED_TX_CONFIRM);
	}else if( ieee80211_is_qos_nullfunc(t.hdr->frame_control)){
		wsm->htTxParameters |=__cpu_to_le32(WSM_NEED_TX_CONFIRM);
		atbm_printk_tx("[TX]:qos null data\n");
	} 
#ifdef CONFIG_TX_NO_CONFIRM_DEBUG
	wsm->htTxParameters |= __cpu_to_le32(WSM_NEED_TX_CONFIRM);
#endif
#endif //CONFIG_TX_NO_CONFIRM	
	ret = atbm_tx_h_skb_pad(hw_priv, wsm, skb);
	if (ret){
		ret = -2;
		goto drop;
	}
	/*
	ret = atbm_tcp_ack_offload(&hw_priv->tx_queue[t.queue],&t.txpriv,skb);
	if (ret){
		ret = -3;
		goto drop;
	}
	*/

	rcu_read_lock();
	sta = rcu_dereference(t.tx_info->control.sta);
	
	//atbm_tx_udp(priv,t.skb,wsm);

	//printk("%s %d fc =%x\n",__func__,__LINE__,t.hdr->frame_control);
#ifdef CONFIG_ATBM_BA_STATUS
	atbm_tx_h_ba_stat(priv, &t);
#endif
	spin_lock_bh(&priv->ps_state_lock);
	{
		tid_update = atbm_tx_h_pm_state(priv, &t);
		//BUG_ON(atbm_queue_put(&hw_priv->tx_queue[t.queue],t.skb, &t.txpriv));
		if(atbm_queue_put(&hw_priv->tx_queue[t.queue],t.skb, &t.txpriv)){
			atbm_printk_err("%s %d ,ERROR !!! atbm_queue_put error\n",__func__,__LINE__);
			
			spin_unlock_bh(&priv->ps_state_lock);
			rcu_read_unlock();
			atbm_skb_dtor(hw_priv, skb, &t.txpriv);
			return;
		}
	}
	spin_unlock_bh(&priv->ps_state_lock);

	if (tid_update && sta)
		ieee80211_sta_set_buffered(sta,
				t.txpriv.tid, true);

	rcu_read_unlock();
	atbm_tx_hif_xmit(hw_priv);

	return;

drop:
	//printk("%s drop tx ret = %d\n",__func__,ret);
//	struct ieee80211_local *local = hw_to_local(hw_priv->hw);
	atbm_skb_dtor(hw_priv, skb, &t.txpriv);
	return;
}

/* ******************************************************************** */
void atbm_tx(struct ieee80211_hw *dev, struct sk_buff *skb)
{


	atbm_tx_udp(dev,skb);
	__atbm_tx(dev, skb);

}

static int atbm_handle_pspoll(struct atbm_vif *priv,
				struct sk_buff *skb)
{
	struct atbm_common *hw_priv = ABwifi_vifpriv_to_hwpriv(priv);
	struct ieee80211_sta *sta;
	struct ieee80211_pspoll *pspoll =
		(struct ieee80211_pspoll *) skb->data;
	int link_id = 0;
	u32 pspoll_mask = 0;
	int drop = 1;
	int i;

	if (priv->join_status != ATBM_APOLLO_JOIN_STATUS_AP)
		goto done;
	if (memcmp(priv->vif->addr, pspoll->bssid, ETH_ALEN))
		goto done;

	rcu_read_lock();
	sta = ieee80211_find_sta(priv->vif, pspoll->ta);
	if (sta) {
		struct atbm_sta_priv *sta_priv;
		sta_priv = (struct atbm_sta_priv *)&sta->drv_priv;
		link_id = sta_priv->link_id;
		pspoll_mask = BIT(sta_priv->link_id);
	}
	rcu_read_unlock();
	if (!link_id)
		goto done;

	priv->pspoll_mask |= pspoll_mask;
	drop = 0;

	/* Do not report pspols if data for given link id is
	 * queued already. */
	for (i = 0; i < 4; ++i) {
		if (atbm_queue_get_num_queued(priv,
				&hw_priv->tx_queue[i],
				pspoll_mask)) {
			atbm_bh_wakeup(hw_priv);
			drop = 1;
			break;
		}
	}
	txrx_printk( "[RX] PSPOLL: %s\n", drop ? "local" : "fwd");
done:
	return drop;
}

/* ******************************************************************** */

void atbm_tx_confirm_cb(struct atbm_common *hw_priv,
			  struct wsm_tx_confirm *arg)
{
	u8 queue_id = atbm_queue_get_queue_id(arg->packetID);
	struct atbm_queue *queue = &hw_priv->tx_queue[queue_id];
	struct sk_buff *skb;
	const struct atbm_txpriv *txpriv;
	struct atbm_vif *priv;
	int tx_count;
	u8 ht_flags;
#ifndef CONFIG_TX_NO_CONFIRM
	int i;
#endif

	txrx_printk( "[TX] TX confirm: %d, %d.\n",
		arg->status, arg->ackFailures);
	if(queue_id >= 4){
		atbm_printk_tx("atbm_tx_confirm_cb %d\n",queue_id);
	}
	if(WARN_ON(!queue)){
		atbm_printk_err("atbm_tx_confirm_cb queue %p\n",queue);
		return;
	}

	priv = ABwifi_hwpriv_to_vifpriv(hw_priv, arg->if_id);
	if (unlikely(!priv)){
		atbm_printk_err("[confirm]:arg_if_id %d\n",arg->if_id);
		return;
	}
	if (unlikely(priv->mode == NL80211_IFTYPE_UNSPECIFIED)) {
		/* STA is stopped. */
		atbm_priv_vif_list_read_unlock(&priv->vif_lock);
		return;
	}

	if (WARN_ON(queue_id >= 4)) {
		atbm_priv_vif_list_read_unlock(&priv->vif_lock);
		return;
	}
	if(WARN_ON(arg ==NULL)){
		atbm_priv_vif_list_read_unlock(&priv->vif_lock);
		return;
	}
#ifdef ATBM_SDIO_PATCH
        if(arg->status == WSM_DATA_CRC_ERRO){
        #ifdef CONFIG_ATBM_APOLLO_TESTMODE
            atbm_queue_remove(hw_priv, queue, arg->packetID);
	    #else
            atbm_queue_remove(queue, arg->packetID);
        #endif /*CONFIG_ATBM_APOLLO_TESTMODE*/

                atbm_priv_vif_list_read_unlock(&priv->vif_lock);
                return ;
        }
#endif
	if (arg->status)
		txrx_printk( "TX failed: %d.\n",
				arg->status);

#ifdef CONFIG_ATBM_APOLLO_TESTMODE
	spin_lock_bh(&hw_priv->tsm_lock);
	if (arg->status)
	{
		hw_priv->atbm_tsm_stats[queue_id].msdu_failed_count++;
		hw_priv->atbm_tsm_stats[queue_id].tid = queue_id;
	}
	hw_priv->atbm_tsm_stats[queue_id].multi_retry_count += arg->ackFailures;

	if ((arg->status == WSM_STATUS_RETRY_EXCEEDED) ||
	    (arg->status == WSM_STATUS_TX_LIFETIME_EXCEEDED)) {
		hw_priv->tsm_stats.msdu_discarded_count++;
		hw_priv->atbm_tsm_stats[queue_id].msdu_discarded_count++;

	} else if ((hw_priv->start_stop_tsm.start) &&
		(arg->status == WSM_STATUS_SUCCESS)) {
		if (queue_id == hw_priv->tsm_info.ac) {
			struct timeval tmval;
			u16 pkt_delay;
			do_gettimeofday(&tmval);
			pkt_delay =
				hw_priv->start_stop_tsm.packetization_delay;
			if (hw_priv->tsm_info.sta_roamed &&
			    !hw_priv->tsm_info.use_rx_roaming) {
				hw_priv->tsm_info.roam_delay = tmval.tv_usec -
				hw_priv->tsm_info.txconf_timestamp_vo;
				if (hw_priv->tsm_info.roam_delay > pkt_delay)
					hw_priv->tsm_info.roam_delay -= pkt_delay;
				txrx_printk( "[TX] txConf"
				"Roaming: roam_delay = %u\n",
				hw_priv->tsm_info.roam_delay);
				hw_priv->tsm_info.sta_roamed = 0;
			}
			hw_priv->tsm_info.txconf_timestamp_vo = tmval.tv_usec;
		}
	}
	spin_unlock_bh(&hw_priv->tsm_lock);
#endif /*CONFIG_ATBM_APOLLO_TESTMODE*/
	if ((arg->status == WSM_REQUEUE) &&
	    (arg->flags & WSM_TX_STATUS_REQUEUE)) {
		/* "Requeue" means "implicit suspend" */
		struct wsm_suspend_resume suspend = {
			.link_id = arg->link_id,
			.stop = 1,
			.multicast = !arg->link_id,
			.if_id = arg->if_id,
		};
		atbm_suspend_resume(priv, &suspend);
		atbm_printk_warn("Requeue for link_id %d (try %d)."
			" STAs asleep: 0x%.8X\n",
			arg->link_id,
			atbm_queue_get_generation(arg->packetID) + 1,
			priv->sta_asleep_mask);
		atbm_printk_debug("<WARNING> %s 111\n",__func__);
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
		WARN_ON(atbm_queue_requeue(hw_priv, queue,
				arg->packetID, true));
#else
		WARN_ON(atbm_queue_requeue(queue,
				arg->packetID, true));
#endif
		spin_lock_bh(&priv->ps_state_lock);
		if (!arg->link_id) {
			priv->buffered_multicasts = true;
			if (priv->sta_asleep_mask) {
				atbm_hw_priv_queue_work(hw_priv,
					&priv->multicast_start_work);
			}
		}
		spin_unlock_bh(&priv->ps_state_lock);
		atbm_priv_vif_list_read_unlock(&priv->vif_lock);
	} else if (!WARN_ON(atbm_queue_get_skb(
			queue, arg->packetID, &skb, &txpriv))) {
		struct ieee80211_tx_info *tx = IEEE80211_SKB_CB(skb);
		if(tx==NULL){
			atbm_printk_err("TX IS NULL\n");
			atbm_priv_vif_list_read_unlock(&priv->vif_lock);
		//	BUG_ON(1);
			atbm_printk_err("%s %d ,ERROR !!! tx is NULL\n",__func__,__LINE__);
			return ;
		}
		tx_count = arg->ackFailures;
		ht_flags = 0;
		/*
		*retry eap frame to prevent that sta connect ap too slowly
		*/
		while(arg->status){
			if(atbm_queue_get_generation(arg->packetID) >= 2){
				atbm_printk_err( "[confirm]:eap retry to mamy(%x)\n",arg->packetID);
				break;
			}

			if(skb->protocol != cpu_to_be16(ETH_P_PAE)){
				break;
			}

			#ifdef CONFIG_ATBM_APOLLO_TESTMODE
			WARN_ON(atbm_queue_requeue(hw_priv, queue,
					arg->packetID, true));
			#else
			WARN_ON(atbm_queue_requeue(queue,
					arg->packetID, true));
			#endif

			atbm_printk_tx( "retry eap frame\n");
			atbm_priv_vif_list_read_unlock(&priv->vif_lock);

			atbm_bh_wakeup(hw_priv);
			return;
		}
#ifdef CONFIG_ATBM_SUPPORT_P2P
		/*
		*when in listen state,we send action again
		*
		*/
		while((arg->status)&&(arg->status != 1)){
			struct ieee80211_hdr *frame = (struct ieee80211_hdr *)(skb->data+txpriv->offset);
			/*
			*at t920 platform,if we retry the frame too many times maybe triger the wtd.
			*so let the top of the retry times be 5 to prevent triger the system wtd
			*/
			if(frame==NULL){
				atbm_printk_tx("frame is null\n");
				atbm_priv_vif_list_read_unlock(&priv->vif_lock);
			//	BUG_ON(1);
				atbm_printk_err("%s %d ,ERROR !!! frame is NULL\n",__func__,__LINE__);
				return ;
			}
			if(atbm_queue_get_generation(arg->packetID) >= 5){
				atbm_printk_err( "[confirm]:retry to many times(%x)\n",arg->packetID);
				break;
			}
			
			if(arg->status != WSM_STATUS_RETRY_EXCEEDED){
				atbm_printk_err("[confirm]:status(%d)\n",arg->status);
				break;
			}
			
			if(!atomic_read(&hw_priv->remain_on_channel)){
				break;
			}

			if(txpriv->if_id != hw_priv->roc_if_id){
				break;
			}
			
			if(!(tx->flags & IEEE80211_TX_INTFL_NL80211_FRAME_TX)){
				break;
			}
			if(!(tx->flags & IEEE80211_TX_CTL_REQ_TX_STATUS)){
				break;
			}
			
			if(!time_is_after_jiffies(hw_priv->roc_start_time+msecs_to_jiffies(hw_priv->roc_duration+35))){
				atbm_printk_err( "[confirm]:timeout\n");
				break;
			}

			if(!ieee80211_is_action(frame->frame_control)){
				break;
			}
			#ifdef CONFIG_ATBM_APOLLO_TESTMODE
			WARN_ON(atbm_queue_requeue(hw_priv, queue,
					arg->packetID, false));
			#else
			WARN_ON(atbm_queue_requeue(queue,
					arg->packetID, false));
			#endif

			atbm_printk_tx("%s:retry[%d][%x],start[%ld],duration[%ld],now(%ld)\n",__func__,arg->status,
					arg->packetID,hw_priv->roc_start_time,msecs_to_jiffies(hw_priv->roc_duration+35),jiffies);
			atbm_priv_vif_list_read_unlock(&priv->vif_lock);

			atbm_bh_wakeup(hw_priv);
			return;
			
		}
#endif
		if (priv->association_mode.greenfieldMode)
			ht_flags |= IEEE80211_TX_RC_GREEN_FIELD;

		if (likely(!arg->status)) {
			tx->flags |= IEEE80211_TX_STAT_ACK;
			priv->cqm_tx_failure_count = 0;
			++tx_count;
			atbm_debug_txed(priv);
			if (arg->flags & WSM_TX_STATUS_AGGREGATION) {
				/* Should report aggregation to mac80211:
				 * it can help  minstrel_ht  calc fit rate*/
				//tx->flags |= IEEE80211_TX_STAT_AMPDU;
				//tx->status.ampdu_len=hw_priv->wsm_txframe_num;
				//tx->status.ampdu_ack_len=(hw_priv->wsm_txframe_num-arg->ackFailures);
				atbm_debug_txed_agg(priv);
			}
		} else {
#ifndef CONFIG_TX_NO_CONFIRM
			spin_lock_bh(&priv->bss_loss_lock);
			if (priv->bss_loss_status ==
					ATBM_APOLLO_BSS_LOSS_CONFIRMING &&
					priv->bss_loss_confirm_id ==
					arg->packetID) {
				priv->bss_loss_status =
					ATBM_APOLLO_BSS_LOSS_CONFIRMED;
				spin_unlock_bh(&priv->bss_loss_lock);
				atbm_cancel_delayed_work(&priv->bss_loss_work);
				atbm_hw_priv_queue_delayed_work(hw_priv,
						&priv->bss_loss_work, 0);
			} else
				spin_unlock_bh(&priv->bss_loss_lock);

			/* TODO: Update TX failure counters */
			if (unlikely(priv->cqm_tx_failure_thold &&
			     (++priv->cqm_tx_failure_count >
			      priv->cqm_tx_failure_thold))) {
				priv->cqm_tx_failure_thold = 0;
				atbm_hw_priv_queue_work(hw_priv,
						&priv->tx_failure_work);
			}
			if (tx_count)
				++tx_count;
#endif
		}
		atbm_priv_vif_list_read_unlock(&priv->vif_lock);
#ifndef CONFIG_TX_NO_CONFIRM
             //  printk("ackfailures:%d tx_count:%d status:%d\n", arg->ackFailures, tx_count, arg->status);
		for (i = 0; i < IEEE80211_TX_MAX_RATES; ++i) {
			if (tx->status.rates[i].count >= tx_count) {
				tx->status.rates[i].count = tx_count;
				break;
			}
			tx_count -= tx->status.rates[i].count;
			if (tx->status.rates[i].flags & IEEE80211_TX_RC_MCS)
				tx->status.rates[i].flags |= ht_flags;
		}
/*
#ifdef MINSTREL_RSSI_USED
		if(tx_count > 0)
		{
                     if(tx_count > 20)
                     printk(KERN_ERR "%s:Error LMAC retry too may times!! rate i:%d  tx_count:%d\n",__func__, i,tx_count);
                     //tx->status.rates[i].count =  tx->status.rates[i].count  + tx_count;    
                }
#endif */
		for (++i; i < IEEE80211_TX_MAX_RATES; ++i) {
			tx->status.rates[i].count = 0;
			tx->status.rates[i].idx = -1;
		}
#endif
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
		atbm_queue_remove(hw_priv, queue, arg->packetID);
#else
		atbm_queue_remove(queue, arg->packetID);
#endif /*CONFIG_ATBM_APOLLO_TESTMODE*/
	}else
	{
		atbm_printk_err("[confirm]:<warning>\n");
		atbm_priv_vif_list_read_unlock(&priv->vif_lock);
	}
}

static void atbm_notify_buffered_tx(struct atbm_vif *priv,
			       struct sk_buff *skb, int link_id, int tid)
{
	u8 *buffered;
	u8 still_buffered = 0;

	if (link_id && tid < ATBM_APOLLO_MAX_TID) {
		buffered = priv->link_id_db[link_id - 1].buffered;

		spin_lock_bh(&priv->ps_state_lock);
		if (!WARN_ON(!buffered[tid]))
			still_buffered = --buffered[tid];
		spin_unlock_bh(&priv->ps_state_lock);

		if (!still_buffered && tid < ATBM_APOLLO_MAX_TID) {
			struct ieee80211_sta *sta;
			struct ieee80211_hdr *hdr;
			hdr = (struct ieee80211_hdr *) skb->data;
			rcu_read_lock();
			sta = ieee80211_find_sta(priv->vif, hdr->addr1);
			if (sta)
				ieee80211_sta_set_buffered(sta, tid, false);
			rcu_read_unlock();
		}
	}
}
void atbm_skb_dtor(struct atbm_common *hw_priv,
		     struct sk_buff *skb,
		     const struct atbm_txpriv *txpriv)
{
	struct atbm_vif *priv =
		__ABwifi_hwpriv_to_vifpriv(hw_priv, txpriv->if_id);

	atbm_skb_pull(skb, txpriv->offset);
	
#ifndef CONFIG_RATE_HW_CONTROL	
	if (priv && txpriv->rate_id != ATBM_APOLLO_INVALID_RATE_ID) {
		atbm_notify_buffered_tx(priv, skb,
				txpriv->raw_link_id, txpriv->tid);
		tx_policy_put(hw_priv, txpriv->rate_id);
	}
#else
	if(priv)
		atbm_notify_buffered_tx(priv, skb,txpriv->raw_link_id, txpriv->tid);
#endif
//	spin_lock_bh(&priv->ps_state_lock);
	//ieee80211_tx_status(hw_priv->hw, skb);
	atbm_ieee80211_tx_status(hw_priv->hw,skb);
//	spin_unlock_bh(&priv->ps_state_lock);

}

#ifdef CONFIG_ATBM_BA_STATUS
static void
atbm_rx_h_ba_stat(struct atbm_vif *priv,
		    size_t hdrlen, size_t skb_len )
{
	struct atbm_common *hw_priv = priv->hw_priv;
	if (priv->join_status != ATBM_APOLLO_JOIN_STATUS_STA)
		return;
	if (!atbm_is_ht(&hw_priv->ht_info))
		return;
	if (!priv->setbssparams_done)
		return;

	spin_lock_bh(&hw_priv->ba_lock);
	hw_priv->ba_acc_rx += skb_len - hdrlen;
	if (!(hw_priv->ba_cnt_rx || hw_priv->ba_cnt)) {
		atbm_mod_timer(&hw_priv->ba_timer,
			jiffies + ATBM_APOLLO_BLOCK_ACK_INTERVAL);
	}
	hw_priv->ba_cnt_rx++;
	spin_unlock_bh(&hw_priv->ba_lock);
}
#endif
void atbm_rx_cb(struct atbm_vif *priv,
		  struct wsm_rx *arg,
		  struct sk_buff **skb_p)
{
	struct atbm_common *hw_priv = ABwifi_vifpriv_to_hwpriv(priv);
	struct sk_buff *skb = *skb_p;
	struct ieee80211_rx_status *hdr = IEEE80211_SKB_RXCB(skb);
	struct ieee80211_hdr *frame = (struct ieee80211_hdr *)skb->data;
	struct atbm_ieee80211_mgmt *mgmt = (struct atbm_ieee80211_mgmt *)skb->data;
	struct atbm_link_entry *entry = NULL;
	#ifdef CONFIG_PM
	unsigned long grace_period;
	#endif
	bool early_data = false;
#ifdef CONFIG_ATBM_BA_STATUS
	size_t hdrlen = 0;
#endif

	hdr->flag = 0;
	//atbm_rx_udp(skb);
	if (unlikely(priv->mode == NL80211_IFTYPE_UNSPECIFIED)) {
		/* STA is stopped. */
		atbm_printk_err("priv->mode == NL80211_IFTYPE_UNSPECIFIED,seq(%x)\n",frame->seq_ctrl);
		goto drop;
	}
#ifdef CONFIG_ATBM_SUPPORT_P2P
	if ((ieee80211_is_action(frame->frame_control))
                        && (mgmt->u.action.category == ATBM_WLAN_CATEGORY_PUBLIC)) {
		u8 *action = (u8*)&mgmt->u.action.category;
		TxRxPublicActionFrame((u8*)mgmt,skb->len,0);
		atbm_check_go_neg_conf_success(hw_priv, action);
	}
#endif
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
	if (hw_priv->start_stop_tsm.start) {
		unsigned queue_id = skb_get_queue_mapping(skb);
		spin_lock_bh(&hw_priv->tsm_lock);
		if (queue_id == 0) {
			struct timeval tmval;
			do_gettimeofday(&tmval);
			if (hw_priv->tsm_info.sta_roamed &&
			    hw_priv->tsm_info.use_rx_roaming) {
				hw_priv->tsm_info.roam_delay = tmval.tv_usec -
					hw_priv->tsm_info.rx_timestamp_vo;
				txrx_printk( "[RX] RxInd Roaming:"
				"roam_delay = %u\n", hw_priv->tsm_info.roam_delay);
				hw_priv->tsm_info.sta_roamed = 0;
			}
			hw_priv->tsm_info.rx_timestamp_vo = tmval.tv_usec;
		}
		spin_unlock_bh(&hw_priv->tsm_lock);
	}
#endif /*CONFIG_ATBM_APOLLO_TESTMODE*/
	if (arg->link_id && (arg->link_id != ATBM_APOLLO_LINK_ID_UNMAPPED)
			&& (arg->link_id <= ATBM_APOLLO_MAX_STA_IN_AP_MODE)) {
		entry =	&priv->link_id_db[arg->link_id - 1];
		if (entry->status == ATBM_APOLLO_LINK_SOFT &&
				ieee80211_is_data(frame->frame_control))
			early_data = true;
		else if(entry->status == ATBM_APOLLO_LINK_HARD)
			entry->timestamp = jiffies;
	}
/*
*unusefull for p2p
*/

	 if (unlikely(arg->status)) {
                struct sta_info *sta;
				char *da,*sa,*bssid;
	 			if(priv->vif->type == NL80211_IFTYPE_AP || priv->vif->type == NL80211_IFTYPE_P2P_GO){
					bssid = frame->addr1;
					sa = frame->addr2;
					da = frame->addr3;
				}else{
					da = frame->addr1;
					bssid = frame->addr2;
					sa = frame->addr3;
					
				}

				if(memcmp(da,priv->vif->addr,6) != 0 && priv->join_status != ATBM_APOLLO_JOIN_STATUS_STA_LISTEN){
					atbm_printk_err("recv dest addr:[%pM] , local addr:[%pM]\n",da,priv->vif->addr);
					goto drop;
				}
				
                rcu_read_lock();
                sta = (struct sta_info *)sta_info_get_rx(vif_to_sdata(priv->vif), frame->addr2);
                if(sta){
						if(priv->join_status != ATBM_APOLLO_JOIN_STATUS_STA_LISTEN)
                        	atbm_printk_err("[%s Rx]:[%pM],PTK[%p],FLAGS[%lx],skb->len[%d]\n",
                                vif_to_sdata(priv->vif)->name,frame->addr2,sta->ptk,sta->_flags,skb->len);
                }
				else {
						if(priv->join_status != ATBM_APOLLO_JOIN_STATUS_STA_LISTEN)
                        	atbm_printk_err("[%s RX]:[%pM] is not assoctiated with us\n",
                                        vif_to_sdata(priv->vif)->name,frame->addr2);
                }
				/*
				*PMF:
				*drop the frame which decrypted fail
				*/
				if(sta && test_sta_flag(sta, WLAN_STA_MFP)){
					if(atbm_ieee80211_is_robust_mgmt_frame(skb) == true){
						 rcu_read_unlock();
						 atbm_printk_err("[RX]:drop PMF\n"); 
						 goto drop;
					}
				}
                rcu_read_unlock();
                if (arg->status == WSM_STATUS_MICFAILURE) {
						if(priv->join_status != ATBM_APOLLO_JOIN_STATUS_STA_LISTEN)
                       	 atbm_printk_err( "[RX] MIC failure. ENCRYPTION [%d]da:[%pM],sa:[%pM]\n",
                                WSM_RX_STATUS_ENCRYPTION(arg->flags),da,sa);
                        hdr->flag |= RX_FLAG_MMIC_ERROR;
                } else if (arg->status == WSM_STATUS_NO_KEY_FOUND) {
						if(priv->join_status != ATBM_APOLLO_JOIN_STATUS_STA_LISTEN)
                        	atbm_printk_err( "[RX] No key found.ENCRYPTION [%d]da:[%pM],sa:[%pM]\n",
                                        WSM_RX_STATUS_ENCRYPTION(arg->flags),da,sa);
                                hdr->flag |= RX_FLAG_UNKOWN_STA_FRAME;
                        if(priv->join_status != ATBM_APOLLO_JOIN_STATUS_AP){
                                goto drop;
                        }
                } else {
                	if(priv->join_status != ATBM_APOLLO_JOIN_STATUS_STA_LISTEN)
                        atbm_printk_err("[RX] Receive failure: %d.ENCRYPTION [%d] da:[%pM],sa:[%pM],fc(%x)\n",
                                arg->status,WSM_RX_STATUS_ENCRYPTION(arg->flags),da,sa,mgmt->frame_control);
                        hdr->flag |= RX_FLAG_UNKOWN_STA_FRAME;
                        if(priv->join_status != ATBM_APOLLO_JOIN_STATUS_AP){
                                goto drop;
                        }
                }
	 	}
#ifdef CHKSUM_HW_SUPPORT
	else {
		if (arg->flags & WSM_RX_STATUS_CHKSUM_ERROR){
			hdr->flag &=~RX_FLAG_HW_CHKSUM_ERROR;
			hdr->flag |= RX_FLAG_HW_CHKSUM_ERROR;
		}else{
			hdr->flag &=~RX_FLAG_HW_CHKSUM_ERROR;
		}
	}	
#endif
	if (skb->len < sizeof(struct ieee80211_pspoll)) {
		atbm_printk_err( "(skb->len < sizeof(struct ieee80211_pspoll),seq(%x)\n",frame->seq_ctrl);
		goto drop;
	}

	if (unlikely(ieee80211_is_pspoll(frame->frame_control))){
		if (atbm_handle_pspoll(priv, skb))
		{
			goto drop;
		}
	}
#ifdef ATBM_PKG_REORDER
	if(ieee80211_is_back_req(frame->frame_control))
	{
		struct ieee80211_bar *bar = (struct ieee80211_bar *)skb->data;
		struct atbm_ba_params ba_params;
		struct ieee80211_bar * bar_data = skb->data;
		ba_params.tid = le16_to_cpu(bar_data->control) >> 12;
		ba_params.ssn =  le16_to_cpu(bar_data->start_seq_num) >> 4;
		ba_params.action= ATBM_BA__ACTION_RX_BAR;
		ba_params.link_id = arg->link_id;
		atbm_printk_rx("rx BAR:ssn(%x),tid(%d),link_id(%d)\n",ba_params.ssn,ba_params.tid,ba_params.link_id);
		atbm_updata_ba_tid_params(priv,&ba_params);
		goto drop;
	}
	else if((ieee80211_is_action(frame->frame_control))
                        && (mgmt->u.action.category == ATBM_WLAN_CATEGORY_BACK)) 
	{
		struct atbm_ba_params ba_params;
		
		ba_params.link_id = arg->link_id;
		switch (mgmt->u.action.u.addba_req.action_code) {
		case WLAN_ACTION_ADDBA_REQ:
		{
			u16 capab;
			ba_params.action = ATBM_BA__ACTION_RX_ADDBR;
			ba_params.timeout = le16_to_cpu(mgmt->u.action.u.addba_req.timeout);
			capab = le16_to_cpu(mgmt->u.action.u.addba_req.capab);
			ba_params.tid =  (capab & IEEE80211_ADDBA_PARAM_TID_MASK) >> 2;
			ba_params.win_size =  (capab & IEEE80211_ADDBA_PARAM_BUF_SIZE_MASK) >> 6;
			ba_params.ssn = le16_to_cpu(mgmt->u.action.u.addba_req.start_seq_num) >> 4;
			atbm_updata_ba_tid_params(priv,&ba_params);
			break;
		}
		case WLAN_ACTION_DELBA:
		{
			u16 params;
			params = le16_to_cpu(mgmt->u.action.u.delba.params);
			ba_params.tid = (params & IEEE80211_DELBA_PARAM_TID_MASK) >> 12;
			ba_params.action = ATBM_BA__ACTION_RX_DELBA;
			atbm_updata_ba_tid_params(priv,&ba_params);
			break;
		}
		default:
			break;
		}

		goto drop;
	}
#endif
	hdr->mactime = 0; /* Not supported by WSM */
	hdr->band = (arg->channelNumber > 14) ?
			IEEE80211_BAND_5GHZ : IEEE80211_BAND_2GHZ;
	hdr->freq = ieee80211_channel_to_frequency(
			arg->channelNumber,
			hdr->band);

	if (arg->rxedRate >= 14) {
		hdr->flag |= RX_FLAG_HT;
		hdr->rate_idx = arg->rxedRate - 14;
	} else if (arg->rxedRate >= 4) {
		if (hdr->band == IEEE80211_BAND_5GHZ)
			hdr->rate_idx = arg->rxedRate - 6;
		else
			hdr->rate_idx = arg->rxedRate - 2;
	} else {
		hdr->rate_idx = arg->rxedRate;
	}

	hdr->signal = (s8)arg->rcpiRssi;
	hdr->antenna = 0;
	if(priv->join_status == ATBM_APOLLO_JOIN_STATUS_SIMPLE_MONITOR){
		atbm_ieee80211_rx(priv->hw, skb);
		*skb_p = NULL;
		return;
	}
#ifdef CONFIG_IEEE80211_SPECIAL_FILTER
	if(WSM_RX_STATUS_FILTER_FRAME&arg->flags){
		*skb_p = ieee80211_special_queue_package(priv->vif,skb);
		return;
	}
#endif
#ifdef CONFIG_ATBM_BA_STATUS
	hdrlen = ieee80211_hdrlen(frame->frame_control);
#endif
#ifndef ATBM_SUPPORT_WIDTH_40M
	if (WSM_RX_STATUS_ENCRYPTION(arg->flags)) {
		size_t iv_len = 0, icv_len = 0;

		hdr->flag |= RX_FLAG_DECRYPTED;

		/* Oops... There is no fast way to ask mac80211 about
		 * IV/ICV lengths. Even defineas are not exposed.*/
		switch (WSM_RX_STATUS_ENCRYPTION(arg->flags)) {
		case WSM_RX_STATUS_WEP:
			iv_len = 4 /* WEP_IV_LEN */;
			icv_len = 4 /* WEP_ICV_LEN */;
			break;
		case WSM_RX_STATUS_TKIP:
			iv_len = 8 /* TKIP_IV_LEN */;
			icv_len = 4 /* TKIP_ICV_LEN */
				+ 8 /*MICHAEL_MIC_LEN*/;
			//mic header is not stripped off by low mac,so it should not be set here
			//hdr->flag |= RX_FLAG_MMIC_STRIPPED;
			break;
		case WSM_RX_STATUS_AES:
			iv_len = 8 /* CCMP_HDR_LEN */;
			icv_len = 8 /* CCMP_MIC_LEN */;
			break;
		case WSM_RX_STATUS_WAPI:
			iv_len = 18 /* WAPI_HDR_LEN */;
			icv_len = 16 /* WAPI_MIC_LEN */;
			hdr->flag |= RX_FLAG_IV_STRIPPED;
			break;
		default:
			WARN_ON("Unknown encryption type");
			goto drop;
		}

		/* Firmware strips ICV in case of MIC failure. */
		if (arg->status == WSM_STATUS_MICFAILURE) {
			icv_len = 0;
			hdr->flag |= RX_FLAG_IV_STRIPPED;
		}

		if (skb->len < hdrlen + iv_len + icv_len) {
			atbm_printk_warn("Mailformed SDU rx'ed.Size is lesser than crypto headers.\n");
//			printk("skb->len < hdrlen + iv_len + icv_len,seq(%x)\n",frame->seq_ctrl);
			goto drop;
		}

		/* Protocols not defined in mac80211 should be
		stripped/crypted in driver/firmware */
		if (WSM_RX_STATUS_ENCRYPTION(arg->flags) ==
						WSM_RX_STATUS_WAPI) {
			/* Remove IV, ICV and MIC */
			atbm_skb_trim(skb, skb->len - icv_len);
			memmove(skb->data + iv_len, skb->data, hdrlen);
			atbm_skb_pull(skb, iv_len);
		}

	}
#else
	if(WSM_RX_STATUS_ENCRYPTION(arg->flags)){
		if(ieee80211_has_protected(frame->frame_control))
			hdr->flag |= RX_FLAG_DECRYPTED;
		else {
			atbm_printk_err("[RX]: open frame decrypted?\n");
		}
	}else {
		if(ieee80211_has_protected(frame->frame_control)){
			atbm_printk_debug("[RX]:need sw decrypted?\n");
		}
	}

	if (arg->status == WSM_STATUS_MICFAILURE) {
			hdr->flag |= RX_FLAG_IV_STRIPPED;
	}
#endif

#ifdef ATBM_SUPPORT_WIDTH_40M
	if(WSM_RX_STATUS_RX_40M&arg->flags){
		hdr->flag |= RX_FLAG_40MHZ;
		hw_priv->rx_40M_pkg_cnt++;
		hw_priv->rx_20M_pkg_detect = 0;
		hw_priv->rx_40M_pkg_detect ++;
	}else if (ieee80211_is_data(frame->frame_control) ){
	
		hw_priv->rx_20M_pkg_cnt++;
		hw_priv->rx_20M_pkg_detect++;
		hw_priv->rx_40M_pkg_detect=0;
	}
#endif
	atbm_debug_rxed(priv);
	if (arg->flags & WSM_RX_STATUS_AGGREGATE){
		printk_once("<WIFI> rx ampdu ++ \n");
		atbm_debug_rxed_agg(priv);
		hdr->flag |= RX_FLAG_AMPDU;
	}
	if (ieee80211_is_beacon(frame->frame_control) &&
			!arg->status &&
			!memcmp(ieee80211_get_SA(frame), priv->join_bssid,
				ETH_ALEN)) {
		const u8 *tim_ie;
		u8 *ies;
		size_t ies_len;
		priv->disable_beacon_filter = false;
#ifdef CONFIG_ATBM_MAC80211_NO_USE
		atbm_hw_priv_queue_work(hw_priv, &priv->update_filtering_work);
#endif
		ies = ((struct atbm_ieee80211_mgmt *)
			  (skb->data))->u.beacon.variable;
		ies_len = skb->len - (ies - (u8 *)(skb->data));

		tim_ie = atbm_get_ie(ies, ies_len, ATBM_WLAN_EID_TIM);
		if (tim_ie) {
			struct ieee80211_tim_ie *tim =
				(struct ieee80211_tim_ie *)&tim_ie[2];

			if (priv->join_dtim_period != tim->dtim_period) {
				priv->join_dtim_period = tim->dtim_period;
				atbm_hw_priv_queue_work(hw_priv,
					&priv->set_beacon_wakeup_period_work);
			}
		}
#ifdef CONFIG_ATBM_MAC80211_NO_USE
		if (unlikely(priv->disable_beacon_filter)) {
			priv->disable_beacon_filter = false;
			atbm_hw_priv_queue_work(hw_priv,
				&priv->update_filtering_work);
		}
#endif
	}
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,3,0))
    if (priv->mode == NL80211_IFTYPE_AP &&( priv->join_status == ATBM_APOLLO_JOIN_STATUS_AP )&&
                    ieee80211_is_beacon(frame->frame_control) &&
                    !arg->status){

            u8 *ies;
            size_t ies_len;
            const u8 *ht_cap;
            ies = ((struct atbm_ieee80211_mgmt *)
                      (skb->data))->u.beacon.variable;
            ies_len = skb->len - (ies - (u8 *)(skb->data));
            ht_cap = atbm_get_ie(ies, ies_len, ATBM_WLAN_EID_HT_CAPABILITY);
            if(!ht_cap){
                    priv->ht_info |= 0x0011;
            }
			if(priv->ht_info == 0x0011)
            	atbm_hw_priv_queue_work(hw_priv,
                            &priv->ht_info_update_work);

    }
#endif
#ifdef ATBM_SUPPORT_WIDTH_40M
	/*
	*Notify Channel Width frame format(8.5.12.2),handled there.
	*
	*this action frame sended when ap want to change its receive channel width.
	*
	*receive this frame , we do not send cmd to lmac to change the channel type.
	*
	*if ap only receive 20M,tx_20M_lock will be set,then frame send by 20M.stop cca check.
	*if ap can receive 40,then we will handle send 40M or 20M.
	*
	*recieve this frame , we also start chantype_change_work,and set channel_chaging.
	*/
	if(
		((priv->vif->p2p == false))
		&&
		(ieee80211_is_action(frame->frame_control))
		&&
		(ieee80211_chw_is_ht40(vif_chw(priv->vif)))
		&&
		(!arg->status))
	{
		if(
			(mgmt->u.action.category == ATBM_WLAN_CATEGORY_HT)
			&&
			(WLAN_HT_ACTION_NOTIFY_CHANWIDTH == mgmt->u.action.u.notify_chan_width.chan_width)
		  )
		{
			struct sta_info *sta;
			rcu_read_lock();
			sta = sta_info_get(vif_to_sdata(priv->vif), frame->addr2);
			if(sta == NULL){
				rcu_read_unlock();
				goto drop;
			}			
			atbm_printk_always("[RX]:CHANWIDTH,sendch(%d)\n",mgmt->u.action.u.notify_chan_width.chan_width); 
			if(mgmt->u.action.u.notify_chan_width.chan_width){
				clear_sta_flag(sta,WLAN_STA_40M_CH_SEND_20M);
			}else{
				set_sta_flag(sta,WLAN_STA_40M_CH_SEND_20M);
			}
			rcu_read_unlock();
			goto drop;
		}
	}
#endif
#ifdef CONFIG_ATBM_SUPPORT_SCHED_SCAN
#ifdef ROAM_OFFLOAD
	if ((ieee80211_is_beacon(frame->frame_control)||ieee80211_is_probe_resp(frame->frame_control)) &&
			!arg->status ) {
		if (hw_priv->auto_scanning && !atomic_read(&hw_priv->scan.in_progress))
			hw_priv->frame_rcvd = 1;

		if (!memcmp(ieee80211_get_SA(frame), priv->join_bssid, ETH_ALEN)) {
			if (hw_priv->beacon)
				atbm_dev_kfree_skb(hw_priv->beacon);
			hw_priv->beacon = atbm_skb_copy(skb, GFP_ATOMIC);
			if (!hw_priv->beacon)
				atbm_printk_err("apollo: sched_scan: own beacon storing failed\n");
		}
	}
#endif /*ROAM_OFFLOAD*/
#endif
#ifdef CONFIG_PM

	/* Stay awake for 1sec. after frame is received to give
	 * userspace chance to react and acquire appropriate
	 * wakelock. */
	if (ieee80211_is_auth(frame->frame_control))
		grace_period = 5 * HZ;
	else if (ieee80211_is_deauth(frame->frame_control))
		grace_period = 5 * HZ;
	else 
		grace_period = 0;
	if(grace_period != 0)
		atbm_pm_stay_awake(&hw_priv->pm_state, grace_period);
#endif
#ifdef CONFIG_ATBM_BA_STATUS
	if (ieee80211_is_data(frame->frame_control))
		atbm_rx_h_ba_stat(priv, hdrlen, skb->len);
#endif
#ifdef ATBM_PKG_REORDER
	if(ieee80211_is_data_qos(frame->frame_control))
	{
		if((arg->link_id == 0)||(arg->link_id-1>=ATBM_APOLLO_LINK_ID_UNMAPPED))
		{
			atbm_printk_err("link_id err\n");
			goto direct_queue;
		}
		if(atbm_reorder_skb_queue(priv,skb,arg->link_id-1) == 0)
		{
			goto direct_queue;
		}
		else
		{
			*skb_p = NULL;
			return;
		}
	}
	else
	{
		
	}
direct_queue:
#endif

	//atbm_skb_trim(skb, skb->len);
#ifdef CONFIG_ATBM_SUPPORT_P2P
#ifdef ATBM_P2P_CHANGE	
	atbm_parase_p2p_action_frame(priv,skb,false);
	atbm_parase_p2p_scan_resp(priv,skb);
#endif
#endif
#ifdef CONFIG_ATBM_STA_LISTEN
	if(priv->join_status == ATBM_APOLLO_JOIN_STATUS_STA_LISTEN){
		hdr->flag |= RX_FLAG_STA_LISTEN;
	}
#endif
	atbm_rx_udp(priv,skb);
	if (unlikely(early_data)) {
		#if 0
		spin_lock_bh(&priv->ps_state_lock);
		/* Double-check status with lock held */
		if (entry->status == ATBM_APOLLO_LINK_SOFT){
			//flags |= RX_DEBUG_FLAG_SOFTLINK;
			///atbm_skb_queue_tail(&entry->rx_queue, skb);
			spin_unlock_bh(&priv->ps_state_lock);
			return; 
		}
		else
		spin_unlock_bh(&priv->ps_state_lock);
		#endif
		atbm_ieee80211_rx(priv->hw, skb);
	} else {
		atbm_ieee80211_rx(priv->hw, skb);
			
	}
	*skb_p = NULL;

	return;

drop:
	/* TODO: update failure counters */
	return;
}

/* ******************************************************************** */
/* Security								*/

int atbm_alloc_key(struct atbm_common *hw_priv)
{
	int idx;

	idx = ffs(~hw_priv->key_map) - 1;
	if (idx < 0 || idx > WSM_KEY_MAX_INDEX)
		return -1;

	hw_priv->key_map |= BIT(idx);
	hw_priv->keys[idx].entryIndex = idx;
	return idx;
}

void atbm_free_key(struct atbm_common *hw_priv, int idx)
{
	if((hw_priv->key_map & BIT(idx))){
		memset(&hw_priv->keys[idx], 0, sizeof(hw_priv->keys[idx]));
		hw_priv->key_map &= ~BIT(idx);
	}
}

void atbm_free_keys(struct atbm_common *hw_priv)
{
	memset(&hw_priv->keys, 0, sizeof(hw_priv->keys));
	hw_priv->key_map = 0;
}

int atbm_upload_keys(struct atbm_vif *priv)
{
	struct atbm_common *hw_priv = ABwifi_vifpriv_to_hwpriv(priv);
	int idx, ret = 0;

	for (idx = 0; idx <= WSM_KEY_MAX_IDX; ++idx)
		if (hw_priv->key_map & BIT(idx)) {
			ret = wsm_add_key(hw_priv, &hw_priv->keys[idx], priv->if_id);
			if (ret < 0)
				break;
		}
	return ret;
}
#if 0
/* Workaround for WFD test case 6.1.10 */
void atbm_link_id_reset(struct atbm_work_struct *work)
{
	struct atbm_vif *priv =
		container_of(work, struct atbm_vif, linkid_reset_work);
	struct atbm_common *hw_priv = priv->hw_priv;
	
	//atbm_flush_workqueue(hw_priv->workqueue);
	if(atbm_bh_is_term(hw_priv)){
		return;
	}
	wsm_lock_tx_async(hw_priv);
	if (atbm_hw_priv_queue_work(hw_priv,
		       &priv->link_id_work) <= 0)
		wsm_unlock_tx(hw_priv);
}
#endif
