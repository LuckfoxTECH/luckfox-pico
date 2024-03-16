#include <linux/version.h>
#ifdef ANDROID_PLATFORM
#include "net/wireless/core.h"
#endif
#include <include/linux/types.h>

#undef NL80211_MCGRP_MLME
#define NL80211_MCGRP_MLME 3
//#if IS_ENABLED(CONFIG_GKI_OPT_FEATURES) && IS_ENABLED(CONFIG_ANDROID) && (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))

static struct genl_family rwnx_nl80211_fam;

static bool __rwnx_cfg80211_unexpected_frame(struct net_device *dev, u8 cmd,
				const u8 *addr, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wdev->wiphy);
	struct sk_buff *msg;
	void *hdr;
	u32 nlportid = READ_ONCE(wdev->ap_unexpected_nlportid);

	if (!nlportid)
		return false;

	msg = nlmsg_new(100, gfp);
	if (!msg)
		return true;

	hdr = genlmsg_put(msg, 0, 0, &rwnx_nl80211_fam, 0, cmd);
	if (!hdr) {
		nlmsg_free(msg);
		return true;
	}

	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
	    nla_put_u32(msg, NL80211_ATTR_IFINDEX, dev->ifindex) ||
	    nla_put(msg, NL80211_ATTR_MAC, ETH_ALEN, addr))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);
	genlmsg_unicast(wiphy_net(&rdev->wiphy), msg, nlportid);
	return true;

 nla_put_failure:
	nlmsg_free(msg);
	return true;
}

bool rwnx_cfg80211_rx_spurious_frame(struct net_device *dev,
				const u8 *addr, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	bool ret;

	if (WARN_ON(wdev->iftype != NL80211_IFTYPE_AP &&
		    wdev->iftype != NL80211_IFTYPE_P2P_GO)) {
		return false;
	}
	ret = __rwnx_cfg80211_unexpected_frame(dev, NL80211_CMD_UNEXPECTED_FRAME,
					addr, gfp);
	return ret;
}

bool rwnx_cfg80211_rx_unexpected_4addr_frame(struct net_device *dev,
				const u8 *addr, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	bool ret;

	if (WARN_ON(wdev->iftype != NL80211_IFTYPE_AP &&
		    wdev->iftype != NL80211_IFTYPE_P2P_GO &&
		    wdev->iftype != NL80211_IFTYPE_AP_VLAN)) {
		return false;
	}
	ret = __rwnx_cfg80211_unexpected_frame(dev,
					 NL80211_CMD_UNEXPECTED_4ADDR_FRAME,
					 addr, gfp);
	return ret;
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0))
void rwnx_cfg80211_notify_new_peer_candidate(struct net_device *dev, const u8 *addr,
				const u8 *ie, u8 ie_len,
				int sig_dbm, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wdev->wiphy);
	struct sk_buff *msg;
	void *hdr;

	if (WARN_ON(wdev->iftype != NL80211_IFTYPE_MESH_POINT))
		return;

	msg = nlmsg_new(100 + ie_len, gfp);
	if (!msg)
		return;

	hdr = genlmsg_put(msg, 0, 0, &rwnx_nl80211_fam, 0, NL80211_CMD_NEW_PEER_CANDIDATE);
	if (!hdr) {
		nlmsg_free(msg);
		return;
	}

	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
	    nla_put_u32(msg, NL80211_ATTR_IFINDEX, dev->ifindex) ||
	    nla_put(msg, NL80211_ATTR_MAC, ETH_ALEN, addr) ||
	    (ie_len && ie &&
	     nla_put(msg, NL80211_ATTR_IE, ie_len, ie)) ||
	    (sig_dbm &&
	     nla_put_u32(msg, NL80211_ATTR_RX_SIGNAL_DBM, sig_dbm)))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);

#define NL80211_MCGRP_MLME 3
	genlmsg_multicast_netns(&rwnx_nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_MLME, gfp);
	return;

 nla_put_failure:
	nlmsg_free(msg);
}
#endif

void rwnx_cfg80211_report_obss_beacon(struct wiphy *wiphy,
				const u8 *frame, size_t len,
				int freq, int sig_dbm)
{
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wiphy);
	struct sk_buff *msg;
	void *hdr;
	struct cfg80211_beacon_registration *reg;

	spin_lock_bh(&rdev->beacon_registrations_lock);
	list_for_each_entry(reg, &rdev->beacon_registrations, list) {
		msg = nlmsg_new(len + 100, GFP_ATOMIC);
		if (!msg) {
			spin_unlock_bh(&rdev->beacon_registrations_lock);
			return;
		}

		hdr = genlmsg_put(msg, 0, 0, &rwnx_nl80211_fam, 0, NL80211_CMD_FRAME);
		if (!hdr)
			goto nla_put_failure;

		if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
		    (freq &&
		     nla_put_u32(msg, NL80211_ATTR_WIPHY_FREQ, freq)) ||
		    (sig_dbm &&
		     nla_put_u32(msg, NL80211_ATTR_RX_SIGNAL_DBM, sig_dbm)) ||
		    nla_put(msg, NL80211_ATTR_FRAME, len, frame))
			goto nla_put_failure;

		genlmsg_end(msg, hdr);

		genlmsg_unicast(wiphy_net(&rdev->wiphy), msg, reg->nlportid);
	}
	spin_unlock_bh(&rdev->beacon_registrations_lock);
	return;

 nla_put_failure:
	spin_unlock_bh(&rdev->beacon_registrations_lock);
	nlmsg_free(msg);
}

static int rwnx_nl80211_send_chandef(struct sk_buff *msg,
				const struct cfg80211_chan_def *chandef)
{
	if (WARN_ON(!cfg80211_chandef_valid(chandef)))
		return -EINVAL;

	if (nla_put_u32(msg, NL80211_ATTR_WIPHY_FREQ,
			chandef->chan->center_freq))
		return -ENOBUFS;
	switch (chandef->width) {
	case NL80211_CHAN_WIDTH_20_NOHT:
	case NL80211_CHAN_WIDTH_20:
	case NL80211_CHAN_WIDTH_40:
		if (nla_put_u32(msg, NL80211_ATTR_WIPHY_CHANNEL_TYPE,
				cfg80211_get_chandef_type(chandef)))
			return -ENOBUFS;
		break;
	default:
		break;
	}
	if (nla_put_u32(msg, NL80211_ATTR_CHANNEL_WIDTH, chandef->width))
		return -ENOBUFS;
	if (nla_put_u32(msg, NL80211_ATTR_CENTER_FREQ1, chandef->center_freq1))
		return -ENOBUFS;
	if (chandef->center_freq2 &&
	    nla_put_u32(msg, NL80211_ATTR_CENTER_FREQ2, chandef->center_freq2))
		return -ENOBUFS;
	return 0;
}

void rwnx_cfg80211_ch_switch_notify(struct cfg80211_registered_device *rdev,
				struct net_device *netdev,
				struct cfg80211_chan_def *chandef,
				gfp_t gfp,
				enum nl80211_commands notif,
				u8 count)
{
	struct sk_buff *msg;
	void *hdr;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, gfp);
	if (!msg)
		return;

	hdr = genlmsg_put(msg, 0, 0, &rwnx_nl80211_fam, 0, notif);
	if (!hdr) {
		nlmsg_free(msg);
		return;
	}

	if (nla_put_u32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex))
		goto nla_put_failure;

	if (rwnx_nl80211_send_chandef(msg, chandef))
		goto nla_put_failure;

	if ((notif == NL80211_CMD_CH_SWITCH_STARTED_NOTIFY) &&
	    (nla_put_u32(msg, NL80211_ATTR_CH_SWITCH_COUNT, count)))
			goto nla_put_failure;

	genlmsg_end(msg, hdr);

	genlmsg_multicast_netns(&rwnx_nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_MLME, gfp);
	return;

 nla_put_failure:
	nlmsg_free(msg);
}

void rwnx_cfg80211_ch_switch_started_notify(struct net_device *dev,
				struct cfg80211_chan_def *chandef,
				u8 count
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 11, 0)
				, bool quiet
	#endif
				)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wiphy);

	rwnx_cfg80211_ch_switch_notify(rdev, dev, chandef, GFP_KERNEL,
				NL80211_CMD_CH_SWITCH_STARTED_NOTIFY, count);
}

int rwnx_regulatory_set_wiphy_regd_sync_rtnl(struct wiphy *wiphy,
				struct ieee80211_regdomain *rd)
{
	wiphy_apply_custom_regulatory(wiphy, rd);
	return 0;
}

void rwnx_skb_append(struct sk_buff *old, struct sk_buff *newsk, struct sk_buff_head *list)
{
	unsigned long flags;
	struct sk_buff *prev = old;
	struct sk_buff *next = prev->next;
	spin_lock_irqsave(&list->lock, flags);
	WRITE_ONCE(newsk->next, next);
	WRITE_ONCE(newsk->prev, prev);
	WRITE_ONCE(next->prev, newsk);
	WRITE_ONCE(prev->next, newsk);
	list->qlen++;
	spin_unlock_irqrestore(&list->lock, flags);
}

bool rwnx_ieee80211_chandef_to_operating_class(struct cfg80211_chan_def *chandef,
					  u8 *op_class)
{
	u8 vht_opclass;
	u32 freq = chandef->center_freq1;

	if (freq >= 2412 && freq <= 2472) {
		if (chandef->width > NL80211_CHAN_WIDTH_40)
			return false;

		/* 2.407 GHz, channels 1..13 */
		if (chandef->width == NL80211_CHAN_WIDTH_40) {
			if (freq > chandef->chan->center_freq)
				*op_class = 83; /* HT40+ */
			else
				*op_class = 84; /* HT40- */
		} else {
			*op_class = 81;
		}

		return true;
	}

	if (freq == 2484) {
		/* channel 14 is only for IEEE 802.11b */
		if (chandef->width != NL80211_CHAN_WIDTH_20_NOHT)
			return false;

		*op_class = 82; /* channel 14 */
		return true;
	}

	switch (chandef->width) {
	case NL80211_CHAN_WIDTH_80:
		vht_opclass = 128;
		break;
	case NL80211_CHAN_WIDTH_160:
		vht_opclass = 129;
		break;
	case NL80211_CHAN_WIDTH_80P80:
		vht_opclass = 130;
		break;
	case NL80211_CHAN_WIDTH_10:
	case NL80211_CHAN_WIDTH_5:
		return false; /* unsupported for now */
	default:
		vht_opclass = 0;
		break;
	}

	/* 5 GHz, channels 36..48 */
	if (freq >= 5180 && freq <= 5240) {
		if (vht_opclass) {
			*op_class = vht_opclass;
		} else if (chandef->width == NL80211_CHAN_WIDTH_40) {
			if (freq > chandef->chan->center_freq)
				*op_class = 116;
			else
				*op_class = 117;
		} else {
			*op_class = 115;
		}

		return true;
	}

	/* 5 GHz, channels 52..64 */
	if (freq >= 5260 && freq <= 5320) {
		if (vht_opclass) {
			*op_class = vht_opclass;
		} else if (chandef->width == NL80211_CHAN_WIDTH_40) {
			if (freq > chandef->chan->center_freq)
				*op_class = 119;
			else
				*op_class = 120;
		} else {
			*op_class = 118;
		}

		return true;
	}

	/* 5 GHz, channels 100..144 */
	if (freq >= 5500 && freq <= 5720) {
		if (vht_opclass) {
			*op_class = vht_opclass;
		} else if (chandef->width == NL80211_CHAN_WIDTH_40) {
			if (freq > chandef->chan->center_freq)
				*op_class = 122;
			else
				*op_class = 123;
		} else {
			*op_class = 121;
		}

		return true;
	}

	/* 5 GHz, channels 149..169 */
	if (freq >= 5745 && freq <= 5845) {
		if (vht_opclass) {
			*op_class = vht_opclass;
		} else if (chandef->width == NL80211_CHAN_WIDTH_40) {
			if (freq > chandef->chan->center_freq)
				*op_class = 126;
			else
				*op_class = 127;
		} else if (freq <= 5805) {
			*op_class = 124;
		} else {
			*op_class = 125;
		}

		return true;
	}

	/* 56.16 GHz, channel 1..4 */
	if (freq >= 56160 + 2160 * 1 && freq <= 56160 + 2160 * 6) {
		if (chandef->width >= NL80211_CHAN_WIDTH_40)
			return false;

		*op_class = 180;
		return true;
	}

	/* not supported yet */
	return false;
}

int rwnx_call_usermodehelper(const char *path, char **argv, char **envp, int wait)
{
	return -1;
}

#endif
