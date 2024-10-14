#ifndef __RWNX_GKI_H
#define __RWNX_GKI_H

#ifdef CONFIG_GKI
void rwnx_gki_skb_append(struct sk_buff *old, struct sk_buff *newsk, struct sk_buff_head *list);
#define rwnx_skb_append                           rwnx_gki_skb_append
#else
#define rwnx_skb_append                           skb_append
#endif//CONFIG_GKI

#endif
