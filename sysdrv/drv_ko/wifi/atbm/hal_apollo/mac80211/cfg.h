/*
 * mac80211 configuration hooks for cfg80211
 */
#ifndef __CFG_H
#define __CFG_H

extern struct cfg80211_ops mac80211_config_ops;
int ieee80211_start_pending_roc_work(struct ieee80211_local *local,
					struct net_device *dev,
				    struct ieee80211_sub_if_data *sdata,
				    struct ieee80211_channel *channel,
				    enum nl80211_channel_type channel_type,
				    unsigned int duration, u64 *cookie,
				    struct sk_buff *txskb);

#endif /* __CFG_H */
