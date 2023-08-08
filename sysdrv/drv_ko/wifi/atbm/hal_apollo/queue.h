/*
 * O(1) TX queue with built-in allocator for altobeam APOLLO drivers
 * *
 * Copyright (c) 2016, altobeam
 * Author:
 *
 * Based on apollo code
 * Copyright (c) 2010, ST-Ericsson
 * Author: Dmitry Tarnyagin <dmitry.tarnyagin@stericsson.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef ATBM_APOLLO_QUEUE_H_INCLUDED
#define ATBM_APOLLO_QUEUE_H_INCLUDED
#include "mac80211/ieee80211_i.h"

/* private */ struct atbm_queue_item;

/* extern */ struct sk_buff;
/* extern */ struct wsm_tx;
/* extern */ struct atbm_common;
/* extern */ struct atbm_vif;
/* extern */ struct ieee80211_tx_queue_stats;
/* extern */ struct atbm_txpriv;

/* forward */ struct atbm_queue_stats;

typedef void (*atbm_queue_skb_dtor_t)(struct atbm_common *priv,
					struct sk_buff *skb,
					const struct atbm_txpriv *txpriv);

struct atbm_queue {
	struct atbm_queue_stats *stats;
	u32				capacity;
	u32			num_queued;
	u32			num_queued_vif[ATBM_WIFI_MAX_VIFS];
	u32			num_pending;
	u32			num_pending_vif[ATBM_WIFI_MAX_VIFS];
	u32			num_sent;
	u32			num_put;
	struct atbm_queue_item *pool;
	struct list_head	queue;
	struct list_head	free_pool;
	struct list_head	pending;
#ifndef	ATBM_WIFI_QUEUE_LOCK_BUG
	int			tx_locked_cnt;
#else
	int			tx_locked_cnt[ATBM_WIFI_MAX_VIFS];
#endif
	int			*link_map_cache[ATBM_WIFI_MAX_VIFS];
#ifndef	ATBM_WIFI_QUEUE_LOCK_BUG
	bool			overfull;
#else
	bool			overfull[ATBM_WIFI_MAX_VIFS];
#endif
	spinlock_t		lock;
	u8			queue_id;
	u8			generation;
#ifndef	ATBM_WIFI_QUEUE_LOCK_BUG
	struct atbm_timer_list	gc;
#else
	struct atbm_timer_list	gc[ATBM_WIFI_MAX_VIFS];
	u8 timer_to_if_id[ATBM_WIFI_MAX_VIFS];
#endif
	unsigned long		ttl;
	struct sk_buff *skb_last;
};

struct atbm_queue_stats {
	spinlock_t		lock;
	int			*link_map_cache[ATBM_WIFI_MAX_VIFS];
	int			num_queued[ATBM_WIFI_MAX_VIFS];
	int			map_capacity;
	wait_queue_head_t	wait_link_id_empty;
	atbm_queue_skb_dtor_t	skb_dtor;
	struct atbm_common	*hw_priv;
};

struct atbm_txpriv {
	u8 link_id;
	u8 raw_link_id;
	u8 tid;
	u8 rate_id;
	u8 offset;
	u8 if_id;
#ifndef P2P_MULTIVIF
	u8 offchannel_if_id;
#else
	u8 raw_if_id;
#endif
};

int atbm_queue_stats_init(struct atbm_queue_stats *stats,
			    size_t map_capacity,
			    atbm_queue_skb_dtor_t skb_dtor,
			    struct atbm_common *priv);
int atbm_queue_init(struct atbm_queue *queue,
		      struct atbm_queue_stats *stats,
		      u8 queue_id,
		      size_t capacity,
		      unsigned long ttl);
int atbm_queue_clear(struct atbm_queue *queue, int if_id);
void atbm_queue_stats_deinit(struct atbm_queue_stats *stats);
void atbm_queue_deinit(struct atbm_queue *queue);

size_t atbm_queue_get_num_queued(struct atbm_vif *priv,
				   struct atbm_queue *queue,
				   u32 link_id_map);
int atbm_queue_put(struct atbm_queue *queue,
		     struct sk_buff *skb,
		     struct atbm_txpriv *txpriv);
int atbm_queue_get(struct atbm_queue *queue,
			int if_id,
		     u32 link_id_map,
		     struct wsm_tx **tx,
		     struct ieee80211_tx_info **tx_info,
		     struct atbm_txpriv **txpriv);
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
int atbm_queue_requeue(struct atbm_common *hw_priv,
			struct atbm_queue *queue,
			u32 packetID, bool check);
#else
int atbm_queue_requeue(struct atbm_queue *queue, u32 packetID, bool check);
#endif
#if 0
int atbm_queue_requeue_all(struct atbm_queue *queue);
#endif
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
int atbm_queue_remove(struct atbm_common *hw_priv,
			struct atbm_queue *queue,
			u32 packetID);
#else
int atbm_queue_remove(struct atbm_queue *queue,
			u32 packetID);
#endif /*CONFIG_ATBM_APOLLO_TESTMODE*/
int atbm_queue_get_skb(struct atbm_queue *queue, u32 packetID,
			 struct sk_buff **skb,
			 const struct atbm_txpriv **txpriv);
#ifndef ATBM_WIFI_QUEUE_LOCK_BUG
void atbm_queue_lock(struct atbm_queue *queue);
void atbm_queue_unlock(struct atbm_queue *queue);
#else
void atbm_queue_lock(struct atbm_queue *queue,int if_id);
void atbm_queue_unlock(struct atbm_queue *queue,int if_id);
#endif
bool atbm_queue_get_xmit_timestamp(struct atbm_queue *queue,
				     unsigned long *timestamp, int if_id,
				     u32 pending_frameID);


bool atbm_queue_stats_is_empty(struct atbm_queue_stats *stats,
				 u32 link_id_map, int if_id);

static inline u8 atbm_queue_get_queue_id(u32 packetID)
{
	return (packetID >> 16) & 0xF;
}

static inline u8 atbm_queue_get_if_id(u32 packetID)
{
	return (packetID >> 20) & 0xF;
}

static inline u8 atbm_queue_get_link_id(u32 packetID)
{
	return (packetID >> 24) & 0xF;
}

static inline u8 atbm_queue_get_generation(u32 packetID)
{
	return (packetID >>  8) & 0xFF;
}

#endif /* ATBM_APOLLO_QUEUE_H_INCLUDED */
