#include <linux/version.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/skbuff.h>
#include <linux/pm_runtime.h>

#include "hwif/hwif.h"
#include "hci/ssv_hci.h"
#include "hci/hctrl.h"
#include "pre_alloc_skb.h"
 #include "ssv_debug.h"

static bool ssv_is_pre_rx_skb(struct ssv_pre_alloc *pre_alloc, struct sk_buff *skb)
{
    int i = 0;
    int q_len = pre_alloc->total_pre_rx_skb_q_len;
    struct sk_buff **skb_list = pre_alloc->pre_rx_skb_list;

    if (q_len > 0) {
        for (i = q_len - 1; i >= 0; i--) {
            if (skb == *(skb_list + i))
                return true;
        }
    }

    return false;
}

struct sk_buff *ssv_pre_rx_skb_alloc(struct ssv_pre_alloc *pre_alloc)
{
    struct sk_buff *skb = NULL;
    int use_skb_q_len = 0;

    if (NULL == pre_alloc) {
        SSV_LOG_DBG("%s: Parameter is invalid!\n", __func__);
        return NULL;
    }

    skb = skb_dequeue_tail(&pre_alloc->pre_rx_skb_q);

    use_skb_q_len = pre_alloc->total_pre_rx_skb_q_len - skb_queue_len(&pre_alloc->pre_rx_skb_q);
    if (use_skb_q_len > pre_alloc->max_running_pre_rx_skb_q_len)
        pre_alloc->max_running_pre_rx_skb_q_len = use_skb_q_len;

    return skb;
}

int ssv_pre_rx_skb_free(struct ssv_pre_alloc *pre_alloc, struct sk_buff *skb)
{
    struct skb_shared_info *s = skb_shinfo(skb);

    if ((NULL == pre_alloc) || (NULL == skb)) {
        SSV_LOG_DBG("%s: Parameter is invalid!\n", __func__);
        return -EINVAL;
    }

    if (false == ssv_is_pre_rx_skb(pre_alloc, skb))
        return -EFAULT;

    // reset skb
    memset(s, 0, offsetof(struct skb_shared_info, dataref));
    atomic_set(&s->dataref, 1);
    memset(skb, 0, offsetof(struct sk_buff, tail));
    skb_reset_tail_pointer(skb);
    // enqueue rxq
    skb_queue_tail(&pre_alloc->pre_rx_skb_q, skb);

    return 0;
}

int ssv_pre_rx_skb_free_all(struct ssv_pre_alloc *pre_alloc)
{
    int i = 0, total_q_len = 0, q_len = 0;
    struct sk_buff *skb = NULL;

    if (NULL == pre_alloc) {
        SSV_LOG_DBG("%s: Parameter is invalid!\n", __func__);
        return -EINVAL;
    }

    q_len = skb_queue_len(&pre_alloc->pre_rx_skb_q);
    total_q_len = pre_alloc->total_pre_rx_skb_q_len;
    if (total_q_len > q_len) {
        for (i = 0; i < q_len; i++)
            skb_dequeue(&pre_alloc->pre_rx_skb_q);

        for (i = 0; i < total_q_len; i++) {
            skb = *(pre_alloc->pre_rx_skb_list + i);
            ssv_pre_rx_skb_free(pre_alloc, skb);
        }
    }

    return 0;
}

int ssv_pre_rx_skb_init(struct ssv_pre_alloc *pre_alloc, int rx_threshold)
{
    int i = 0;
    struct sk_buff *skb = NULL;

    SSV_LOG_DBG("%s\n", __func__);
    if (NULL == pre_alloc) {
        SSV_LOG_DBG("%s: Parameter is invalid!\n", __func__);
        return -EINVAL;
    }

    if (rx_threshold > 0) {
        // create a list to record each pre alloc skb
        pre_alloc->pre_rx_skb_list = kmalloc(rx_threshold * sizeof(struct sk_buff *), GFP_KERNEL);
        if (NULL == pre_alloc->pre_rx_skb_list)
            return -ENOMEM;
        memset(pre_alloc->pre_rx_skb_list, 0, rx_threshold * sizeof(struct sk_buff *));

        skb_queue_head_init(&pre_alloc->pre_rx_skb_q);
        pre_alloc->max_running_pre_rx_skb_q_len = 0;
        // pre alloc rx skb for hci rx aggr frame
        for (i = 0; i < rx_threshold; i++) {
            skb = __dev_alloc_skb(MAX_HCI_RX_AGGR_SIZE+PLATFORM_DEF_DMA_ALIGN_SIZE-1, GFP_KERNEL);
            if (NULL == skb)
                break;
            
            skb_queue_tail(&pre_alloc->pre_rx_skb_q, skb);
            *(pre_alloc->pre_rx_skb_list + i) = skb;
        }
        pre_alloc->total_pre_rx_skb_q_len = skb_queue_len(&pre_alloc->pre_rx_skb_q);
        SSV_LOG_DBG("alloc %d rx skb_buff\n", pre_alloc->total_pre_rx_skb_q_len);
        if (pre_alloc->total_pre_rx_skb_q_len != rx_threshold) {
            BUG_ON(1);
        }
    }

    return 0;
}

int ssv_pre_rx_skb_deinit(struct ssv_pre_alloc *pre_alloc)
{
    int i = 0;
    struct sk_buff *skb = NULL;
    u32 q_len = 0;

    SSV_LOG_DBG("%s\n", __func__);
    if (NULL == pre_alloc) {
        SSV_LOG_DBG("%s: Parameter is invalid!\n", __func__);
        return -EINVAL;
    }

    q_len = pre_alloc->total_pre_rx_skb_q_len;
    if (pre_alloc->pre_rx_skb_list) {
        for (i = 0; i < q_len; i++) {
            skb = *(pre_alloc->pre_rx_skb_list + i);
            if (skb != NULL)
                dev_kfree_skb_any(skb);
        }
        kfree(pre_alloc->pre_rx_skb_list);
    }
    SSV_LOG_DBG("max_running: %u\n", pre_alloc->max_running_pre_rx_skb_q_len);

    return 0;
}

struct sk_buff *ssv_pre_tx_skb_alloc(struct ssv_pre_alloc *pre_alloc)
{
    struct sk_buff *skb = NULL;

    if (NULL == pre_alloc) {
        SSV_LOG_DBG("%s: Parameter is invalid!\n", __func__);
        return NULL;
    }

    skb = pre_alloc->pre_tx_skb;

    return skb;
}

int ssv_pre_tx_skb_free(struct ssv_pre_alloc *pre_alloc, struct sk_buff *skb)
{
    struct skb_shared_info *s = skb_shinfo(skb);

    if ((NULL == pre_alloc) || (skb == NULL)) {
        SSV_LOG_DBG("%s: Parameter is invalid!\n", __func__);
        return -EINVAL;
    }

    if (skb == pre_alloc->pre_tx_skb) {
        // reset skb
        memset(s, 0, offsetof(struct skb_shared_info, dataref));
        atomic_set(&s->dataref, 1);
        memset(skb, 0, offsetof(struct sk_buff, tail));
        skb_reset_tail_pointer(skb);
    }

    return 0;
}

int ssv_pre_tx_skb_init(struct ssv_pre_alloc *pre_alloc)
{
    SSV_LOG_DBG("%s\n", __func__);
    if (NULL == pre_alloc) {
        SSV_LOG_DBG("%s: Parameter is invalid!\n", __func__);
        return -EINVAL;
    }

    pre_alloc->pre_tx_skb = __dev_alloc_skb(HCI_TX_AGGR_SKB_LEN+PLATFORM_DEF_DMA_ALIGN_SIZE-1, GFP_KERNEL);
    if (NULL == pre_alloc->pre_tx_skb) {
        SSV_LOG_DBG("%s: Pre-alloc tx skb error!\n", __func__);
        return -ENOMEM;
    }
    
    return 0;
}

int ssv_pre_tx_skb_deinit(struct ssv_pre_alloc *pre_alloc)
{
    SSV_LOG_DBG("%s\n", __func__);
    if (NULL == pre_alloc) {
        SSV_LOG_DBG("%s: Parameter is invalid!\n", __func__);
        return -EINVAL;
    }

    if (NULL != pre_alloc->pre_tx_skb) {
        dev_kfree_skb_any(pre_alloc->pre_tx_skb);
        pre_alloc->pre_tx_skb = NULL;
    }

    return 0;
}
