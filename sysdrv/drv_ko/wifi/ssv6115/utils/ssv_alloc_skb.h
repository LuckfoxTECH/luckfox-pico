#ifndef _SSV_ALLOC_SKB_H
#define _SSV_ALLOC_SKB_H

#include <linux/inetdevice.h>

struct sk_buff *ssv_rx_skb_alloc(unsigned int frame_len, gfp_t flags);
void ssv_rx_skb_free(struct sk_buff *skb);
struct sk_buff *ssv_tx_skb_alloc(unsigned int frame_len, gfp_t flags);
void ssv_tx_skb_free(struct sk_buff *skb);

#endif
