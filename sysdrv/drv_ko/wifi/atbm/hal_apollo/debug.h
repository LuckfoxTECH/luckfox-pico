/*
 * DebugFS code for altobeam APOLLO mac80211 driver
 * *
 * Copyright (c) 2016, altobeam
 * Author:
 *
 * Based on apollo code
 * Copyright (c) 2011, ST-Ericsson
 * Author: Dmitry Tarnyagin <dmitry.tarnyagin@stericsson.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef ATBM_APOLLO_DEBUG_H_INCLUDED
#define ATBM_APOLLO_DEBUG_H_INCLUDED


struct cw200_common;


#define ATBM_APOLLO_DBG_MSG		0x00000001
#define ATBM_APOLLO_DBG_NIY		0x00000002
#define ATBM_APOLLO_DBG_SBUS		0x00000004
#define ATBM_APOLLO_DBG_INIT		0x00000008
#define ATBM_APOLLO_DBG_ERROR	0x00000010
#define ATBM_APOLLO_DBG_DCXO_DPLL  0x00000020
#define ATBM_APOLLO_DBG_LEVEL	(ATBM_APOLLO_DBG_INIT|ATBM_APOLLO_DBG_NIY|ATBM_APOLLO_DBG_ERROR)


#define FUNC_ENTER() //printk("%s %d++\n",__func__,__LINE__)
#define FUNC_EXIT() //printk("%s %d--\n",__func__,__LINE__)


#define atbm_dbg(level, ...)				\
 if ((level) & ATBM_APOLLO_DBG_LEVEL)		\
		atbm_printk_init(__VA_ARGS__);	\


 /* TODO It should be removed before official delivery */
 static __inline void frame_hexdump(char *prefix, u8 *data, int len)
 {
	 int i;

	atbm_printk_always("%s hexdump:\n", prefix);
	 for (i = 0; i < len; i++) {
	 	if((i % 16)==0)
			atbm_printk_always("\n");
		atbm_printk_always("%02x ", data[i]);

	 }
	atbm_printk_always("\n");
 }
struct atbm_debug_param{
	void *private;
	char *buff;
	int size;
	int cout;
};

#ifdef CONFIG_ATBM_APOLLO_DEBUGFS
typedef struct seq_file *P_VDEBUG_SEQFILE;
#define VDEBUG_SEQFILE         struct seq_file 
#define VDEBUG_PRINTF(...)  	seq_printf(__VA_ARGS__)
#define VDEBUG_PUTS(a,b)		seq_puts(a,b)
#define VDEBUG_PRIV(seq)		((seq)->private)
#else

typedef struct atbm_debug_param *P_VDEBUG_SEQFILE;
#define VDEBUG_SEQFILE         struct atbm_debug_param
#define VDEBUG_PRINTF(a,...)  	a->cout += snprintf(a->buff + a->cout, a->size - a->cout, __VA_ARGS__)
#define VDEBUG_PUTS(a,b)		a->cout += snprintf(a->buff + a->cout, a->size - a->cout, b)
#define VDEBUG_PRIV(seq)		((seq)->private)
#endif
#ifdef CONFIG_ATBM_APOLLO_DEBUG
int atbm_debug_init_common(struct atbm_common *hw_priv);
int atbm_debug_init_priv(struct atbm_common *hw_priv,
			   struct atbm_vif *priv);
void atbm_debug_release_common(struct atbm_common *hw_priv);
void atbm_debug_release_priv(struct atbm_vif *priv);

static inline void atbm_debug_txed(struct atbm_vif *priv)
{
	++priv->debug.tx;
}

static inline void atbm_debug_txed_agg(struct atbm_vif *priv)
{
	++priv->debug.tx_agg;
}

static inline void atbm_debug_txed_multi(struct atbm_vif *priv,
					   int count)
{
	++priv->debug.tx_multi;
	priv->debug.tx_multi_frames += count;
}

static inline void atbm_debug_rxed(struct atbm_vif *priv)
{
	++priv->debug.rx;
}

static inline void atbm_debug_rxed_agg(struct atbm_vif *priv)
{
	++priv->debug.rx_agg;
}

static inline void atbm_debug_tx_cache_miss(struct atbm_common *common)
{
	++common->debug->tx_cache_miss;
}

static inline void atbm_debug_tx_align(struct atbm_vif *priv)
{
	++priv->debug.tx_align;
}

static inline void atbm_debug_tx_ttl(struct atbm_vif *priv)
{
	++priv->debug.tx_ttl;
}

static inline void atbm_debug_tx_burst(struct atbm_common *hw_priv)
{
	++hw_priv->debug->tx_burst;
}

static inline void atbm_debug_rx_burst(struct atbm_common *hw_priv)
{
	++hw_priv->debug->rx_burst;
}

static inline void atbm_debug_ba(struct atbm_common *hw_priv,
				   int ba_cnt, int ba_acc, int ba_cnt_rx,
				   int ba_acc_rx)
{
	hw_priv->debug->ba_cnt = ba_cnt;
	hw_priv->debug->ba_acc = ba_acc;
	hw_priv->debug->ba_cnt_rx = ba_cnt_rx;
	hw_priv->debug->ba_acc_rx = ba_acc_rx;
}

int atbm_print_fw_version(struct atbm_common *hw_priv, u8* buf, size_t len);
int atbm_status_show_priv(VDEBUG_SEQFILE * seq, void *v);
int atbm_ht_show_info(VDEBUG_SEQFILE * seq, void *v);
int atbm_wifi_show_status(VDEBUG_SEQFILE * seq, void *v);
int atbm_status_show_common(VDEBUG_SEQFILE * seq, void *v);
int atbm_counters_show(VDEBUG_SEQFILE * seq, void *v);
int atbm_statistics_show(P_VDEBUG_SEQFILE seq, void *v);
int atbm_pkt_show(P_VDEBUG_SEQFILE seq, void *v);


#else /* CONFIG_ATBM_APOLLO_DEBUGFS */

static inline int atbm_debug_init_common(struct atbm_common *hw_priv)
{
	return 0;
}

static inline int atbm_debug_init_priv(struct atbm_common *hw_priv,
			   struct atbm_vif *priv)
{
	return 0;
}

static inline void atbm_debug_release_common(struct atbm_common *hw_priv)
{
}

static inline void atbm_debug_release_priv(struct atbm_vif *priv)
{
}

static inline void atbm_debug_txed(struct atbm_vif *priv)
{
}

static inline void atbm_debug_txed_agg(struct atbm_vif *priv)
{
}

static inline void atbm_debug_txed_multi(struct atbm_vif *priv,
					   int count)
{
}

static inline void atbm_debug_rxed(struct atbm_vif *priv)
{
}

static inline void atbm_debug_rxed_agg(struct atbm_vif *priv)
{
}

static inline void atbm_debug_tx_cache_miss(struct atbm_common *priv)
{
}

static inline void atbm_debug_tx_align(struct atbm_vif *priv)
{
}

static inline void atbm_debug_tx_ttl(struct atbm_vif *priv)
{
}

static inline void atbm_debug_tx_burst(struct atbm_common *priv)
{
}

static inline void atbm_debug_rx_burst(struct atbm_common *priv)
{
}

static inline void atbm_debug_ba(struct atbm_common *hw_priv,
				   int ba_cnt, int ba_acc, int ba_cnt_rx,
				   int ba_acc_rx)
{
}


static inline int atbm_status_show_priv(VDEBUG_SEQFILE * seq, void *v)
{
	return 0;
}
static inline int atbm_ht_show_info(VDEBUG_SEQFILE * seq, void *v)
{
	return 0;
}
static inline int atbm_wifi_show_status(VDEBUG_SEQFILE * seq, void *v)
{
	return 0;
}

static inline int atbm_status_show_common(VDEBUG_SEQFILE * seq, void *v)
{
	return 0;
}
static inline int atbm_counters_show(VDEBUG_SEQFILE * seq, void *v)
{
	return 0;
}
static inline int atbm_statistics_show(P_VDEBUG_SEQFILE seq, void *v)
{
	return 0;
}
static inline int atbm_pkt_show(P_VDEBUG_SEQFILE seq, void *v)
{
	return 0;
}

//int atbm_print_fw_version(struct atbm_vif *priv, u8* buf, size_t len)
//{
//}

#endif /* CONFIG_ATBM_APOLLO_DEBUGFS */

#endif /* ATBM_APOLLO_DEBUG_H_INCLUDED */
