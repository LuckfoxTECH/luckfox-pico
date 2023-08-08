/*
 * Copyright (c) 2015 iComm-semi Ltd.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <linux/kernel.h>
#include <linux/version.h>
#include "ssv_skb.h"
#include <ssv6200.h>
#ifdef  CONFIG_FW_ALIGNMENT_CHECK
#include <hwif/sdio/sdio_def.h>
#endif

struct sk_buff *ssv_skb_alloc(void *app_param, s32 len)
{
    struct sk_buff *skb;
    int header_len = (SSV6200_PKT_HEADROOM_RSVD > SSV_SKB_info_size) ? SSV6200_PKT_HEADROOM_RSVD : SSV_SKB_info_size;

#ifdef  CONFIG_FW_ALIGNMENT_CHECK
    skb = __dev_alloc_skb(len + SSV6200_ALLOC_RSVD + PLATFORM_DEF_DMA_ALIGN_SIZE - 1, GFP_KERNEL);
#else
    skb = __dev_alloc_skb(len + SSV6200_ALLOC_RSVD, GFP_KERNEL);
#endif
    if (skb != NULL) {
        skb_reserve(skb, header_len);
#ifdef  CONFIG_FW_ALIGNMENT_CHECK
        if ((unsigned long)skb->data % PLATFORM_DEF_DMA_ALIGN_SIZE) {
            unsigned long shift = PLATFORM_DEF_DMA_ALIGN_SIZE - ((unsigned long)skb->data % PLATFORM_DEF_DMA_ALIGN_SIZE);
            skb_reserve(skb, shift);
        }
#endif
    }
    return skb;
}

struct sk_buff *ssv_skb_alloc_ex(void *app_param, s32 len, gfp_t gfp_mask)
{
    //struct ssv_softc *sc;
    struct sk_buff *skb;
    int header_len = (SSV6200_PKT_HEADROOM_RSVD > SSV_SKB_info_size) ? SSV6200_PKT_HEADROOM_RSVD : SSV_SKB_info_size;

#ifdef  CONFIG_FW_ALIGNMENT_CHECK
    skb = __dev_alloc_skb(len + SSV6200_ALLOC_RSVD + PLATFORM_DEF_DMA_ALIGN_SIZE - 1, gfp_mask);
#else
    skb = __dev_alloc_skb(len + SSV6200_ALLOC_RSVD , gfp_mask);
#endif
    if (skb != NULL) {
        skb_reserve(skb, header_len);
#ifdef  CONFIG_FW_ALIGNMENT_CHECK
        if ((unsigned long)skb->data % PLATFORM_DEF_DMA_ALIGN_SIZE) {
            unsigned long shift = PLATFORM_DEF_DMA_ALIGN_SIZE - ((unsigned long)skb->data % PLATFORM_DEF_DMA_ALIGN_SIZE);
            skb_reserve(skb, shift);
        }
#endif
    }
    return skb;
}


void ssv_skb_free(void *app_param, struct sk_buff *skb)
{
    dev_kfree_skb_any(skb);
}
