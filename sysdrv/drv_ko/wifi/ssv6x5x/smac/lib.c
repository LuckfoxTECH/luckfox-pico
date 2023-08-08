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


#include <linux/version.h>
#include <ssv6200.h>
#include "lib.h"

/*

struct sk_buff *ssv_skb_alloc(s32 len)
{
    struct sk_buff *skb;

    skb = __dev_alloc_skb(len + 128 , GFP_KERNEL);
    if (skb != NULL) {
        skb_put(skb,0x20);
        skb_pull(skb,0x20);
    }
    return skb;
}
//EXPORT_SYMBOL(ssv_skb_alloc);



void ssv_skb_free(struct sk_buff *skb)
{
    dev_kfree_skb_any(skb);
}
//EXPORT_SYMBOL(ssv_skb_free);


*/




