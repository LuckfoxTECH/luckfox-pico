/*
 * mac80211 STA and AP API for mac80211 altobeam APOLLO drivers
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

#ifndef AP_H_INCLUDED
#define AP_H_INCLUDED

#define ATBM_APOLLO_NOA_NOTIFICATION_DELAY 10

int atbm_set_tim(struct ieee80211_hw *dev, struct ieee80211_sta *sta,
		   bool set);
int atbm_sta_add(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
		   struct ieee80211_sta *sta);
int atbm_sta_remove(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
		      struct ieee80211_sta *sta);
void atbm_sta_notify(struct ieee80211_hw *dev, struct ieee80211_vif *vif,
		       enum sta_notify_cmd notify_cmd,
		       struct ieee80211_sta *sta);
void atbm_bss_info_changed(struct ieee80211_hw *dev,
			     struct ieee80211_vif *vif,
			     struct ieee80211_bss_conf *info,
			     u32 changed);
int atbm_ampdu_action(struct ieee80211_hw *hw,
			struct ieee80211_vif *vif,
			enum ieee80211_ampdu_mlme_action action,
			struct ieee80211_sta *sta, u16 tid, u16 *ssn,
			u8 buf_size);

void atbm_suspend_resume(struct atbm_vif *priv,
			  struct wsm_suspend_resume *arg);
void atbm_set_tim_work(struct atbm_work_struct *work);
//void atbm_set_cts_work(struct atbm_work_struct *work);
void atbm_multicast_start_work(struct atbm_work_struct *work);
void atbm_multicast_stop_work(struct atbm_work_struct *work);
void atbm_mcast_timeout(unsigned long arg);
int atbm_find_link_id(struct atbm_vif *priv, const u8 *mac);
int atbm_alloc_link_id(struct atbm_vif *priv, const u8 *mac);
void atbm_link_id_work(struct atbm_work_struct *work);
void atbm_link_id_gc_work(struct atbm_work_struct *work);
#ifdef CONFIG_ATBM_SUPPORT_P2P
void atbm_notify_noa(struct atbm_vif *priv, int delay);
#endif
int ABwifi_unmap_link(struct atbm_vif *priv, int link_id);
void atbm_ht_info_update_work(struct atbm_work_struct *work);
int atbm_start_monitor_mode(struct atbm_vif *priv,
				struct ieee80211_channel *chan);
int atbm_stop_monitor_mode(struct atbm_vif *priv);

#endif
