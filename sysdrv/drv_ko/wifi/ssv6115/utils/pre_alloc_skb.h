#ifndef _PRE_ALLOC_SKB_H
#define _PRE_ALLOC_SKB_H

#include <linux/inetdevice.h>

struct ssv_pre_alloc {
    // rx
    struct sk_buff_head pre_rx_skb_q;
    struct sk_buff **pre_rx_skb_list;
    u32 max_running_pre_rx_skb_q_len;
    u32 total_pre_rx_skb_q_len;
    // tx
    struct sk_buff *pre_tx_skb;
};

struct sk_buff *ssv_pre_rx_skb_alloc(struct ssv_pre_alloc *pre_alloc);
int ssv_pre_rx_skb_free(struct ssv_pre_alloc *pre_alloc, struct sk_buff *skb);
int ssv_pre_rx_skb_free_all(struct ssv_pre_alloc *pre_alloc);
int ssv_pre_rx_skb_init(struct ssv_pre_alloc *pre_alloc, int rx_threshold);
int ssv_pre_rx_skb_deinit(struct ssv_pre_alloc *pre_alloc);

struct sk_buff *ssv_pre_tx_skb_alloc(struct ssv_pre_alloc *pre_alloc);
int ssv_pre_tx_skb_free(struct ssv_pre_alloc *pre_alloc, struct sk_buff *skb);
int ssv_pre_tx_skb_init(struct ssv_pre_alloc *pre_alloc);
int ssv_pre_tx_skb_deinit(struct ssv_pre_alloc *pre_alloc);

#endif //_PRE_ALLOC_SKB_H
