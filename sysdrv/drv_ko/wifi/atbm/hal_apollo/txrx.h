/*
 * Datapath interface for altobeam APOLLO mac80211 drivers
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

#ifndef ATBM_APOLLO_TXRX_H
#define ATBM_APOLLO_TXRX_H

#include <linux/list.h>

/* extern */ struct ieee80211_hw;
/* extern */ struct sk_buff;
/* extern */ struct wsm_tx;
/* extern */ struct wsm_rx;
/* extern */ struct wsm_tx_confirm;
/* extern */ struct atbm_txpriv;
/* extern */ struct atbm_vif;
#ifndef CONFIG_RATE_HW_CONTROL
struct tx_policy {
	union {
		__le32 tbl[3];
		u8 raw[12];
	};
	u8  defined;		/* TODO: u32 or u8, profile and select best */
	u8  usage_count;	/* --// -- */
	u8  retry_count;	/* --// -- */
	u8  uploaded;
};

struct tx_policy_cache_entry {
	struct tx_policy policy;
	struct list_head link;
};

#define TX_POLICY_CACHE_SIZE	(8)
struct tx_policy_cache {
	struct tx_policy_cache_entry cache[TX_POLICY_CACHE_SIZE];
	struct list_head used;
	struct list_head free;
	spinlock_t lock;
};
/* ******************************************************************** */
/* TX policy cache							*/
/* Intention of TX policy cache is an overcomplicated WSM API.
 * Device does not accept per-PDU tx retry sequence.
 * It uses "tx retry policy id" instead, so driver code has to sync
 * linux tx retry sequences with a retry policy table in the device.
 */
void tx_policy_init(struct atbm_common *hw_priv);
void tx_policy_upload_work(struct atbm_work_struct *work);
#endif
/* ******************************************************************** */
/* TX implementation							*/

u32 atbm_rate_mask_to_wsm(struct atbm_common *hw_priv,
			       u32 rates);
void atbm_tx(struct ieee80211_hw *dev, struct sk_buff *skb);
void atbm_skb_dtor(struct atbm_common *hw_priv,
		     struct sk_buff *skb,
		     const struct atbm_txpriv *txpriv);

/* ******************************************************************** */
/* WSM callbacks							*/

void atbm_tx_confirm_cb(struct atbm_common *hw_priv,
			  struct wsm_tx_confirm *arg);
void atbm_rx_cb(struct atbm_vif *priv,
		  struct wsm_rx *arg,
		  struct sk_buff **skb_p);

/* ******************************************************************** */
/* Timeout								*/
/* ******************************************************************** */
/* Security								*/
int atbm_alloc_key(struct atbm_common *hw_priv);
void atbm_free_key(struct atbm_common *hw_priv, int idx);
void atbm_free_keys(struct atbm_common *hw_priv);
int atbm_upload_keys(struct atbm_vif *priv);
#if 0
/* ******************************************************************** */
/* Workaround for WFD test case 6.1.10					*/
void atbm_link_id_reset(struct atbm_work_struct *work);
#endif
#endif /* ATBM_APOLLO_TXRX_H */
