/*
 * Copyright 2002-2005, Instant802 Networks, Inc.
 * Copyright 2005-2006, Devicescape Software, Inc.
 * Copyright (c) 2006 Jiri Benc <jbenc@suse.cz>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/rtnetlink.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/debugfs.h>
#include <net/atbm_mac80211.h>
#include "rate.h"
#include "ieee80211_i.h"
#include "debugfs.h"
#include <net/netlink.h>
#define CONFIG_COMPAT_MAC80211_RC_DEFAULT "minstrel_ht"
struct rate_control_alg {
	struct list_head list;
	struct rate_control_ops *ops;
};

static LIST_HEAD(rate_ctrl_algs);
static DEFINE_MUTEX(rate_ctrl_mutex);

static char *ieee80211_default_rc_algo = CONFIG_COMPAT_MAC80211_RC_DEFAULT;
module_param(ieee80211_default_rc_algo, charp, 0644);
MODULE_PARM_DESC(ieee80211_default_rc_algo,
		 "Default rate control algorithm for mac80211 to use");

int ieee80211_rate_control_register(struct rate_control_ops *ops)
{
	struct rate_control_alg *alg;

	if (!ops->name)
		return -EINVAL;

	mutex_lock(&rate_ctrl_mutex);
	list_for_each_entry(alg, &rate_ctrl_algs, list) {
		if (!strcmp(alg->ops->name, ops->name)) {
			/* don't register an algorithm twice */
			WARN_ON(1);
			mutex_unlock(&rate_ctrl_mutex);
			return -EALREADY;
		}
	}

	alg = atbm_kzalloc(sizeof(*alg), GFP_KERNEL);
	if (alg == NULL) {
		mutex_unlock(&rate_ctrl_mutex);
		return -ENOMEM;
	}
	alg->ops = ops;

	list_add_tail(&alg->list, &rate_ctrl_algs);
	mutex_unlock(&rate_ctrl_mutex);

	return 0;
}
//EXPORT_SYMBOL(ieee80211_rate_control_register);

void ieee80211_rate_control_unregister(struct rate_control_ops *ops)
{
	struct rate_control_alg *alg;

	mutex_lock(&rate_ctrl_mutex);
	list_for_each_entry(alg, &rate_ctrl_algs, list) {
		if (alg->ops == ops) {
			list_del(&alg->list);
			atbm_kfree(alg);
			break;
		}
	}
	mutex_unlock(&rate_ctrl_mutex);
}
//EXPORT_SYMBOL(ieee80211_rate_control_unregister);

static struct rate_control_ops *
ieee80211_try_rate_control_ops_get(const char *name)
{
	struct rate_control_alg *alg;
	struct rate_control_ops *ops = NULL;

	if (!name)
		return NULL;

	mutex_lock(&rate_ctrl_mutex);
	list_for_each_entry(alg, &rate_ctrl_algs, list) {
		if (!strcmp(alg->ops->name, name))
			if (try_module_get(alg->ops->module)) {
				ops = alg->ops;
				break;
			}
	}
	mutex_unlock(&rate_ctrl_mutex);
	return ops;
}

/* Get the rate control algorithm. */
static struct rate_control_ops *
ieee80211_rate_control_ops_get(const char *name)
{
	struct rate_control_ops *ops;
	const char *alg_name;
	#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 2, 0))
	kparam_block_sysfs_write(ieee80211_default_rc_algo);
	#else
	kernel_param_lock(THIS_MODULE);
	#endif
	if (!name)
		alg_name = ieee80211_default_rc_algo;
	else
		alg_name = name;

	ops = ieee80211_try_rate_control_ops_get(alg_name);
	if (!ops) {
		request_module("rc80211_%s", alg_name);
		ops = ieee80211_try_rate_control_ops_get(alg_name);
	}
	if (!ops && name)
		/* try default if specific alg requested but not found */
		ops = ieee80211_try_rate_control_ops_get(ieee80211_default_rc_algo);

	/* try built-in one if specific alg requested but not found */
	if (!ops && strlen(CONFIG_COMPAT_MAC80211_RC_DEFAULT))
		ops = ieee80211_try_rate_control_ops_get(CONFIG_COMPAT_MAC80211_RC_DEFAULT);
	#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 2, 0))
	kparam_unblock_sysfs_write(ieee80211_default_rc_algo);
	#else
	kernel_param_unlock(THIS_MODULE);
	#endif

	return ops;
}

static void ieee80211_rate_control_ops_put(struct rate_control_ops *ops)
{
	module_put(ops->module);
}

#ifdef CONFIG_MAC80211_ATBM_DEBUGFS
static ssize_t rcname_read(struct file *file, char __user *userbuf,
			   size_t count, loff_t *ppos)
{
	struct rate_control_ref *ref = file->private_data;
	int len = strlen(ref->ops->name);

	return simple_read_from_buffer(userbuf, count, ppos,
				       ref->ops->name, len);
}

static const struct file_operations rcname_ops = {
	.read = rcname_read,
	.open = mac80211_open_file_generic,
	.llseek = default_llseek,
};
#endif

static struct rate_control_ref *rate_control_alloc(const char *name,
					    struct ieee80211_local *local)
{
	struct dentry *debugfsdir = NULL;
	struct rate_control_ref *ref;

	ref = atbm_kmalloc(sizeof(struct rate_control_ref), GFP_KERNEL);
	if (!ref)
		goto fail_ref;
	kref_init(&ref->kref);
	ref->local = local;
	ref->ops = ieee80211_rate_control_ops_get(name);
	if (!ref->ops)
		goto fail_ops;

#ifdef CONFIG_MAC80211_ATBM_DEBUGFS
	debugfsdir = debugfs_create_dir("rc", local->hw.wiphy->debugfsdir);
	local->debugfs.rcdir = debugfsdir;
	debugfs_create_file("name", 0400, debugfsdir, ref, &rcname_ops);
#endif

	ref->priv = ref->ops->alloc(&local->hw, debugfsdir);
	if (!ref->priv)
		goto fail_priv;
	return ref;

fail_priv:
	ieee80211_rate_control_ops_put(ref->ops);
fail_ops:
	atbm_kfree(ref);
fail_ref:
	return NULL;
}

static void rate_control_release(struct kref *kref)
{
	struct rate_control_ref *ctrl_ref;

	ctrl_ref = container_of(kref, struct rate_control_ref, kref);
	ctrl_ref->ops->free(ctrl_ref->priv);

#ifdef CONFIG_MAC80211_ATBM_DEBUGFS
	debugfs_remove_recursive(ctrl_ref->local->debugfs.rcdir);
	ctrl_ref->local->debugfs.rcdir = NULL;
#endif

	ieee80211_rate_control_ops_put(ctrl_ref->ops);
	atbm_kfree(ctrl_ref);
}

static bool rc_no_data_or_no_ack_use_min(struct ieee80211_tx_rate_control *txrc)
{
	struct sk_buff *skb = txrc->skb;
	struct ieee80211_hdr *hdr = (struct ieee80211_hdr *) skb->data;
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	__le16 fc;

	fc = hdr->frame_control;

	return (info->flags & (IEEE80211_TX_CTL_NO_ACK |
			       IEEE80211_TX_CTL_USE_MINRATE)) ||
		!ieee80211_is_data(fc);
}

static void rc_send_low_broadcast(s8 *idx, u32 basic_rates,
				  struct ieee80211_supported_band *sband)
{
	u8 i;

	if (basic_rates == 0)
		return; /* assume basic rates unknown and accept rate */
	if (*idx < 0)
		return;
	if (basic_rates & (1 << *idx))
		return; /* selected rate is a basic rate */

	for (i = *idx + 1; i <= sband->n_bitrates; i++) {
		if (basic_rates & (1 << i)) {
			*idx = i;
			return;
		}
	}

	/* could not find a basic rate; use original selection */
}

static inline s8
rate_lowest_non_cck_index(struct ieee80211_supported_band *sband,
			  struct ieee80211_sta *sta)
{
	int i;

	for (i = 0; i < sband->n_bitrates; i++) {
		struct ieee80211_rate *srate = &sband->bitrates[i];
		if ((srate->bitrate == 10) || (srate->bitrate == 20) ||
		    (srate->bitrate == 55) || (srate->bitrate == 110))
			continue;

		if (rate_supported(sta, sband->band, i))
			return i;
	}

	/* No matching rate found */
	return 0;
}


bool rate_control_send_low(struct ieee80211_sta *sta,
			   void *priv_sta,
			   struct ieee80211_tx_rate_control *txrc)
{
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(txrc->skb);
	struct ieee80211_supported_band *sband = txrc->sband;
	int mcast_rate;

	if (!sta || !priv_sta || rc_no_data_or_no_ack_use_min(txrc)) {
		if ((sband->band != IEEE80211_BAND_2GHZ) ||
		    !(info->flags & IEEE80211_TX_CTL_NO_CCK_RATE))
			info->control.rates[0].idx =
				rate_lowest_index(txrc->sband, sta);
		else
			info->control.rates[0].idx =
				rate_lowest_non_cck_index(txrc->sband, sta);
		info->control.rates[0].count =
			(info->flags & IEEE80211_TX_CTL_NO_ACK) ?
			1 : txrc->hw->max_rate_tries;
		if (!sta && txrc->bss) {
			mcast_rate = txrc->bss_conf->mcast_rate[sband->band];
			if (mcast_rate > 0) {
				info->control.rates[0].idx = mcast_rate - 1;
				return true;
			}

			rc_send_low_broadcast(&info->control.rates[0].idx,
					      txrc->bss_conf->basic_rates,
					      sband);
		}
		return true;
	}
	return false;
}
//EXPORT_SYMBOL(rate_control_send_low);

static void rate_idx_match_mask(struct ieee80211_tx_rate *rate,
				int n_bitrates, u32 mask)
{
	int j;

	/* See whether the selected rate or anything below it is allowed. */
	for (j = rate->idx; j >= 0; j--) {
		if (mask & (1 << j)) {
			/* Okay, found a suitable rate. Use it. */
			rate->idx = j;
			return;
		}
	}

	/* Try to find a higher rate that would be allowed */
	for (j = rate->idx + 1; j < n_bitrates; j++) {
		if (mask & (1 << j)) {
			/* Okay, found a suitable rate. Use it. */
			rate->idx = j;
			return;
		}
	}

	/*
	 * Uh.. No suitable rate exists. This should not really happen with
	 * sane TX rate mask configurations. However, should someone manage to
	 * configure supported rates and TX rate mask in incompatible way,
	 * allow the frame to be transmitted with whatever the rate control
	 * selected.
	 */
}
#if defined(CONFIG_NL80211_TESTMODE) || defined(CONFIG_ATBM_IOCTRL)


/*g_atbm_rate_Ctl[0]  multicast rate control
**g_atbm_rate_Ctl[1]  unicast rate control */
struct tagAtbmRateControl g_atbm_rate_Ctl; 

#define RATEFIXED 1
#define RATEUNFIXED 0
#define INDEX_MAX 11
#define INDEX_MIN 0
#define INDEX_MCS_MAX 7
#define INDEX_MCS_MIN 0
#define RATE_CONTROL_MUTICAST 0
#define RATE_CONTROL_UNICAST 1
#endif
void rate_control_get_rate(struct ieee80211_sub_if_data *sdata,
			   struct sta_info *sta,
			   struct ieee80211_tx_rate_control *txrc)
{
	struct rate_control_ref *ref = sdata->local->rate_ctrl;
	void *priv_sta = NULL;
	struct ieee80211_sta *ista = NULL;
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(txrc->skb);
	int i;
	u32 mask;
#if defined(CONFIG_NL80211_TESTMODE) || defined(CONFIG_ATBM_IOCTRL)

	struct ieee80211_hdr *hdr = (void *)txrc->skb->data;
	int index = 0;
#endif
	if (sta) {
		ista = &sta->sta;
		priv_sta = sta->rate_ctrl_priv;
	}

	for (i = 0; i < IEEE80211_TX_MAX_RATES; i++) {
		info->control.rates[i].idx = -1;
		info->control.rates[i].flags = 0;
		info->control.rates[i].count = 1;
	}

	if (sdata->local->hw.flags & IEEE80211_HW_HAS_RATE_CONTROL)
		return;

	ref->ops->get_rate(ref->priv, ista, priv_sta, txrc);

	/*
	 * Try to enforce the rateidx mask the user wanted. skip this if the
	 * default mask (allow all rates) is used to save some processing for
	 * the common case.
	 */
	mask = sdata->rc_rateidx_mask[info->band];
	if (mask != (1 << txrc->sband->n_bitrates) - 1) {
		if (sta) {
			/* Filter out rates that the STA does not support */
			mask &= sta->sta.supp_rates[info->band];
		}
		/*
		 * Make sure the rate index selected for each TX rate is
		 * included in the configured mask and change the rate indexes
		 * if needed.
		 */
		for (i = 0; i < IEEE80211_TX_MAX_RATES; i++) {
			/* Skip invalid rates */
			if (info->control.rates[i].idx < 0)
				break;
			/* Rate masking supports only legacy rates for now */
			if (info->control.rates[i].flags & IEEE80211_TX_RC_MCS)
				continue;
			rate_idx_match_mask(&info->control.rates[i],
					    txrc->sband->n_bitrates, mask);
		}
	}

	BUG_ON(info->control.rates[0].idx < 0);
#if defined(CONFIG_NL80211_TESTMODE) || defined(CONFIG_ATBM_IOCTRL)


	/*g_atbm_rate_Ctl[0]  multicast rate control
	**g_atbm_rate_Ctl[1]  unicast rate control */
	if (is_multicast_ether_addr(hdr->addr1)) {
		index = RATE_CONTROL_MUTICAST;
	}else{
		index = RATE_CONTROL_UNICAST;
	}
	/* rate fixed*/
	if (0 != g_atbm_rate_Ctl.rate_fix[index])
	{
		if (g_atbm_rate_Ctl.my_flags[index] & IEEE80211_TX_RC_MCS)
		{
			if (g_atbm_rate_Ctl.my_index[index] > INDEX_MCS_MAX || g_atbm_rate_Ctl.my_index[index] < INDEX_MCS_MIN)
			{
				atbm_printk_err("Rate fix to %d wrong\n", g_atbm_rate_Ctl.my_index[index]);
				return;
			}
		}
		else
		{
			if (g_atbm_rate_Ctl.my_index[index] > INDEX_MAX || g_atbm_rate_Ctl.my_index[index] < INDEX_MIN)
			{
				atbm_printk_err("Rate fix to %d wrong\n", g_atbm_rate_Ctl.my_index[index]);
				return;
			}
		}

		for (i = 0; i < IEEE80211_TX_MAX_RATES; i++) {
			info->control.rates[i].idx = g_atbm_rate_Ctl.my_index[index];
			info->control.rates[i].flags = g_atbm_rate_Ctl.my_flags[index];
			if(info->flags & IEEE80211_TX_CTL_NO_ACK)
				info->control.rates[i].count = 1;
			else
				info->control.rates[i].count = 7;
		}
	}
#endif
}

struct rate_control_ref *rate_control_get(struct rate_control_ref *ref)
{
	kref_get(&ref->kref);
	return ref;
}

void rate_control_put(struct rate_control_ref *ref)
{
	kref_put(&ref->kref, rate_control_release);
}
#if defined(CONFIG_NL80211_TESTMODE) && defined(CONFIG_ATBM_TEST_TOOL)


enum altm_rate_control{
	ALTM_RATE_SET_FIX = 1,
	ALTM_RATE_SET_FLAGS = 2,
	ALTM_RATE_SET_INDEX = 3,
	ALTM_RATE_GET_FIX = 4,
	ALTM_RATE_GET_FLAGS = 5,
	ALTM_RATE_GET_INDEX = 6,
	ALTM_RATE_SET_FIX_MULTI = 31,
	ALTM_RATE_SET_FLAGS_MULTI = 32,
	ALTM_RATE_SET_INDEX_MULTI = 33,
	ALTM_RATE_GET_FIX_MULTI = 34,
	ALTM_RATE_GET_FLAGS_MULTI = 35,
	ALTM_RATE_GET_INDEX_MULTI = 36,

};
enum atbm_nl80211_testmode_data_attributes {
	ATBM_TM_MSG_ID = 0x0001,	/* u32 type containing the ATBM message ID */
	ATBM_TM_MSG_DATA,	/* message payload */

	/* Max indicator so module test may add its own attributes */
	ATBM_TM_MSG_ATTR_MAX,
};
extern int atbm_tesmode_reply(struct wiphy *wiphy,
				const void *data, int len);
int rate_altm_control_test(struct wiphy *wiphy, void *data, int len)
{
	struct altm_msg{
		u8 type;
		u32 value;
	};
	int ret = 0;
	struct altm_msg *msg = 0;
	struct ieee80211_local *local = wiphy_priv(wiphy);
	if(local->hw.vendcmd_nl80211 == 0)
	{
		struct nlattr *data_p = nla_find(data, len, ATBM_TM_MSG_DATA);
		if (!data_p)
			return -EINVAL;
		msg = (struct altm_msg *)nla_data(data_p);
	}
	else
		msg = (struct altm_msg *)data;

	atbm_printk_always("type:%d value:%d\n", msg->type, msg->value);
	switch (msg->type)
	{
		case ALTM_RATE_SET_FIX:
			//test_debug_enable = 1;
			g_atbm_rate_Ctl.rate_fix[RATE_CONTROL_UNICAST] = msg->value;
			break;
		case ALTM_RATE_SET_FLAGS:
			g_atbm_rate_Ctl.my_flags[RATE_CONTROL_UNICAST] = msg->value;
			break;
		case ALTM_RATE_SET_INDEX:
			g_atbm_rate_Ctl.my_index[RATE_CONTROL_UNICAST] = msg->value;
			break;
		case ALTM_RATE_GET_FIX:
			ret = atbm_tesmode_reply(wiphy, &g_atbm_rate_Ctl.rate_fix[RATE_CONTROL_UNICAST], sizeof(u32));
			break;
		case ALTM_RATE_GET_FLAGS:
			ret = atbm_tesmode_reply(wiphy, &g_atbm_rate_Ctl.my_flags[RATE_CONTROL_UNICAST], sizeof(u8));
			break;
		case ALTM_RATE_GET_INDEX:
			ret = atbm_tesmode_reply(wiphy, &g_atbm_rate_Ctl.my_index[RATE_CONTROL_UNICAST], sizeof(u8));
			break;
		case ALTM_RATE_SET_FIX_MULTI:
			g_atbm_rate_Ctl.rate_fix[RATE_CONTROL_MUTICAST] = msg->value;
			break;
		case ALTM_RATE_SET_FLAGS_MULTI:
			g_atbm_rate_Ctl.my_flags[RATE_CONTROL_MUTICAST] = msg->value;
			break;
		case ALTM_RATE_SET_INDEX_MULTI:
			g_atbm_rate_Ctl.my_index[RATE_CONTROL_MUTICAST] = msg->value;
			break;
		case ALTM_RATE_GET_FIX_MULTI:
			ret = atbm_tesmode_reply(wiphy, &g_atbm_rate_Ctl.rate_fix[RATE_CONTROL_MUTICAST], sizeof(u32));
			break;
		case ALTM_RATE_GET_FLAGS_MULTI:
			ret = atbm_tesmode_reply(wiphy, &g_atbm_rate_Ctl.my_flags[RATE_CONTROL_MUTICAST], sizeof(u8));
			break;
		case ALTM_RATE_GET_INDEX_MULTI:
			ret = atbm_tesmode_reply(wiphy, &g_atbm_rate_Ctl.my_index[RATE_CONTROL_MUTICAST], sizeof(u8));
			break;
		default:
			break;
	}

	return ret;
}
#endif
int test_debug_enable = 0;

void frame_hexdump(char *prefix, u8 *data, int len)
{

	int i;
	atbm_printk_always( "%s hexdump:\n", prefix);
	for (i = 0; i < len; i++) {
	   if((i % 16)==0)
		   atbm_printk_always("\n");
	   atbm_printk_always("%02x ", data[i]);

	}
}

void ATBMWIFI_DBG_PRINT(const char * func,const int line) { \
	if(test_debug_enable)
		atbm_printk_always("%s %d \n",func,line);
}
//EXPORT_SYMBOL(ATBMWIFI_DBG_PRINT);

void ATBMWIFI_DBG_PRINT2(const char * func,const int line,unsigned int data) { \
	if(test_debug_enable)
		atbm_printk_always("%s %d data[%x]\n",func,line,data);
}
//EXPORT_SYMBOL(ATBMWIFI_DBG_PRINT2);


void ATBMWIFI_DBG_DUMP(char *prefix, u8 *data, int len)
{
	if(test_debug_enable)
		frame_hexdump(prefix,data,len);
}
//EXPORT_SYMBOL(ATBMWIFI_DBG_DUMP);



int atbm_ieee80211_init_rate_ctrl_alg(struct ieee80211_local *local,
				 const char *name)
{
	struct rate_control_ref *ref, *old;

	ASSERT_RTNL();

	if (local->open_count)
		return -EBUSY;

	if (local->hw.flags & IEEE80211_HW_HAS_RATE_CONTROL) {
		if (WARN_ON(!local->ops->set_rts_threshold))
			return -EINVAL;
		return 0;
	}

	ref = rate_control_alloc(name, local);
	if (!ref) {
		atbm_printk_warn("Failed to select rate control algorithm,%s\n",name);
		return -ENOENT;
	}

	old = local->rate_ctrl;
	local->rate_ctrl = ref;
	if (old) {
		rate_control_put(old);
		sta_info_flush(local, NULL);
	}

	wiphy_debug(local->hw.wiphy, "Selected rate control algorithm '%s'\n",
		    ref->ops->name);
#if defined(CONFIG_NL80211_TESTMODE) || defined(CONFIG_ATBM_IOCTRL)
	memset(&g_atbm_rate_Ctl, 0, sizeof(g_atbm_rate_Ctl));
#endif
	return 0;
}

void rate_control_deinitialize(struct ieee80211_local *local)
{
	struct rate_control_ref *ref;

	ref = local->rate_ctrl;

	if (!ref)
		return;

	local->rate_ctrl = NULL;
	rate_control_put(ref);
}

