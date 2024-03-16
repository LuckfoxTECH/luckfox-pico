#ifndef __RWNX_GKI_H
#define __RWNX_GKI_H

#ifdef ANDROID_PLATFORM
#include "net/wireless/core.h"
#endif

//#if IS_ENABLED(CONFIG_GKI_OPT_FEATURES) && IS_ENABLED(CONFIG_ANDROID) && (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))


bool rwnx_cfg80211_rx_spurious_frame(struct net_device *dev,
				const u8 *addr, gfp_t gfp);

bool rwnx_cfg80211_rx_unexpected_4addr_frame(struct net_device *dev,
				const u8 *addr, gfp_t gfp);

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0))
void rwnx_cfg80211_notify_new_peer_candidate(struct net_device *dev, const u8 *addr,
				const u8 *ie, u8 ie_len,
				int sig_dbm, gfp_t gfp);
#endif

void rwnx_cfg80211_report_obss_beacon(struct wiphy *wiphy,
				const u8 *frame, size_t len,
				int freq, int sig_dbm);

void rwnx_cfg80211_ch_switch_notify(struct cfg80211_registered_device *rdev,
				struct net_device *netdev,
				struct cfg80211_chan_def *chandef,
				gfp_t gfp,
				enum nl80211_commands notif,
				u8 count);

void rwnx_cfg80211_ch_switch_started_notify(struct net_device *dev,
				struct cfg80211_chan_def *chandef,
				u8 count
			#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 11, 0)
				, bool quiet
			#endif
				);

int rwnx_regulatory_set_wiphy_regd_sync_rtnl(struct wiphy *wiphy,
				struct ieee80211_regdomain *rd);

void rwnx_skb_append(struct sk_buff *old, struct sk_buff *newsk, struct sk_buff_head *list);

bool rwnx_ieee80211_chandef_to_operating_class(struct cfg80211_chan_def *chandef,
				u8 *op_class);

int rwnx_call_usermodehelper(const char *path, char **argv, char **envp, int wait);

#else

#define rwnx_cfg80211_rx_spurious_frame           cfg80211_rx_spurious_frame
#define rwnx_cfg80211_rx_unexpected_4addr_frame   cfg80211_rx_unexpected_4addr_frame

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0))
#define rwnx_cfg80211_notify_new_peer_candidate   cfg80211_notify_new_peer_candidate
#endif

#define rwnx_cfg80211_report_obss_beacon          cfg80211_report_obss_beacon
#define rwnx_cfg80211_ch_switch_notify            cfg80211_ch_switch_notify
#define rwnx_cfg80211_ch_switch_started_notify    cfg80211_ch_switch_started_notify
#define rwnx_regulatory_set_wiphy_regd_sync_rtnl  regulatory_set_wiphy_regd_sync_rtnl
#define rwnx_skb_append                           skb_append
#define rwnx_ieee80211_chandef_to_operating_class ieee80211_chandef_to_operating_class
#define rwnx_call_usermodehelper                  call_usermodehelper

#endif

#endif
