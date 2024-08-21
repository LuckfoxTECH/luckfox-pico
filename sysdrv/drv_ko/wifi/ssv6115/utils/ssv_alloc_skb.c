#include <linux/version.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/skbuff.h>
#include <linux/pm_runtime.h>

#include "ssv_alloc_skb.h"
#include "hwif/hwif.h"
#include "ssv_debug.h"

#if (CONFIG_PRE_ALLOC_SKB != 0)
#define PRE_ALLOC_RX_USED_LEN_THRESHOLD     2304 
extern struct sk_buff *ssv_pre_allocate_rx_skb_alloc(void);
extern int ssv_pre_allocate_rx_skb_free(struct sk_buff *skb);
extern struct sk_buff *ssv_pre_allocate_tx_skb_alloc(void);
extern int ssv_pre_allocate_tx_skb_free(struct sk_buff *skb);
#endif

struct sk_buff *ssv_rx_skb_alloc(unsigned int frame_len, gfp_t flags)
{
    struct sk_buff *skb;
    int size = frame_len + PLATFORM_DEF_DMA_ALIGN_SIZE-1;

#if (CONFIG_PRE_ALLOC_SKB != 0)
    if (frame_len < PRE_ALLOC_RX_USED_LEN_THRESHOLD) {
        skb = __dev_alloc_skb(size, flags);
        skb->cb[0] = 0x0;
    } else {
        skb = ssv_pre_allocate_rx_skb_alloc();
        // tag pre alloc rx
        skb->cb[0] = 'p';
        skb->cb[1] = 'r';
        skb->cb[2] = 'e';
        skb->cb[3] = 'r';
        skb->cb[4] = 'x';
    }
#else
    skb = __dev_alloc_skb(size, flags);
    skb->cb[0] = 0x0;
#endif

    if (skb) {
        if ((unsigned long)(skb->data) % PLATFORM_DEF_DMA_ALIGN_SIZE) {
            unsigned long shift = PLATFORM_DEF_DMA_ALIGN_SIZE - 
                                ((unsigned long)(skb->data) % PLATFORM_DEF_DMA_ALIGN_SIZE);
            
            skb_reserve(skb, shift);
        }
    }

    return skb;
}

void ssv_rx_skb_free(struct sk_buff *skb)
{
#if (CONFIG_PRE_ALLOC_SKB != 0)
    if ((skb->cb[0] == 'p') && (skb->cb[1] == 'r') && (skb->cb[2] == 'e') && 
        (skb->cb[3] == 'r') && (skb->cb[4] == 'x')) {
    
        if (ssv_pre_allocate_rx_skb_free(skb) == -EFAULT) {
            //dev_kfree_skb_any(skb);
            BUG_ON(1);
        }
    } else {
        dev_kfree_skb_any(skb);
    }
#else
    dev_kfree_skb_any(skb);
#endif
}

struct sk_buff *ssv_tx_skb_alloc(unsigned int frame_len, gfp_t flags)
{
    struct sk_buff *skb;

#if (CONFIG_PRE_ALLOC_SKB != 0)
    skb = ssv_pre_allocate_tx_skb_alloc();
#else 
    {
        int size = frame_len + PLATFORM_DEF_DMA_ALIGN_SIZE-1;
        skb = __dev_alloc_skb(size, flags);
    }
#endif
    
    if (skb) {
        if ((unsigned long)skb->data % PLATFORM_DEF_DMA_ALIGN_SIZE) {
            unsigned long shift = PLATFORM_DEF_DMA_ALIGN_SIZE - ((unsigned long)skb->data % PLATFORM_DEF_DMA_ALIGN_SIZE);
            skb_reserve(skb, shift);
        }
    }

    return skb;
}

void ssv_tx_skb_free(struct sk_buff *skb)
{
#if (CONFIG_PRE_ALLOC_SKB != 0)
    ssv_pre_allocate_tx_skb_free(skb);
#else
    dev_kfree_skb_any(skb);
#endif
}
