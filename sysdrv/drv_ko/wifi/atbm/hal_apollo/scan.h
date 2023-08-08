/*
 * Scan interface for altobeam APOLLO mac80211 drivers
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

#ifndef SCAN_H_INCLUDED
#define SCAN_H_INCLUDED

#include <linux/semaphore.h>
#include "wsm.h"

/* external */ struct sk_buff;
/* external */ struct cfg80211_scan_request;
/* external */ struct ieee80211_channel;
/* external */ struct ieee80211_hw;
/* external */ struct atbm_work_struct;

struct atbm_scan {
	struct semaphore lock;
	struct atbm_work_struct work;
#ifdef CONFIG_ATBM_SUPPORT_SCHED_SCAN
#ifdef ROAM_OFFLOAD
	struct atbm_work_struct swork; /* scheduled scan work */
	struct cfg80211_sched_scan_request *sched_req;
#endif /*ROAM_OFFLOAD*/
#endif
	struct atbm_delayed_work timeout;
#ifdef CONFIG_ATBM_SCAN_SPLIT
	struct atbm_delayed_work scan_spilt;
	u8	   split;
#endif
	struct cfg80211_scan_request *req;
	struct ieee80211_scan_req_wrap *req_wrap;
	struct ieee80211_channel **begin;
	struct ieee80211_channel **curr;
	struct ieee80211_channel **end;
	struct wsm_ssid ssids[WSM_SCAN_MAX_NUM_OF_SSIDS];
	int output_power;
	int n_ssids;
	int status;
	atomic_t in_progress;
	/* Direct probe requests workaround */
	struct atbm_delayed_work probe_work;
	int direct_probe;
	u8 if_id;
	 int wait_complete;
	 struct atbm_work_struct smartwork;
	 struct atbm_work_struct smartsetChanwork;
	 struct atbm_work_struct smartstopwork;
	u8 scan_smartconfig;
	u8 cca;
	u8 passive;
};

int atbm_hw_scan(struct ieee80211_hw *hw,
			struct ieee80211_vif *vif,
			struct ieee80211_scan_req_wrap *req_wrap);
#ifdef CONFIG_ATBM_SUPPORT_SCHED_SCAN
#ifdef ROAM_OFFLOAD
int atbm_hw_sched_scan_start(struct ieee80211_hw *hw,
			struct ieee80211_vif *vif,
			struct cfg80211_sched_scan_request *req,
			struct ieee80211_sched_scan_ies *ies);
void atbm_hw_sched_scan_stop(struct atbm_common *priv);
void atbm_sched_scan_work(struct atbm_work_struct *work);
#endif /*ROAM_OFFLOAD*/
#endif
void atbm_scan_work(struct atbm_work_struct *work);
void atbm_scan_timeout(struct atbm_work_struct *work);
void etf_scan_end_work(struct atbm_work_struct *work);
void atbm_scan_complete_cb(struct atbm_common *priv,
				struct wsm_scan_complete *arg);
#ifdef CONFIG_ATBM_SCAN_SPLIT
void atbm_scan_split_work(struct atbm_work_struct *work);
#endif

/* ******************************************************************** */
/* Raw probe requests TX workaround					*/
void atbm_probe_work(struct atbm_work_struct *work);
#ifdef CONFIG_ATBM_SUPPORT_P2P
void atbm_scan_listenning_restart_delayed(struct atbm_vif *priv);
#endif

#ifdef CONFIG_ATBM_APOLLO_TESTMODE
/* Advance Scan Timer							*/
void atbm_advance_scan_timeout(struct atbm_work_struct *work);
#endif
void atbm_cancel_hw_scan(struct ieee80211_hw *hw,struct ieee80211_vif *vif);
void atbm_wait_scan_complete_sync(struct atbm_common *hw_priv);

#endif
