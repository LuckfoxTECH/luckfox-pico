#include <linux/version.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/skbuff.h>
#include <linux/pm_runtime.h>

#include "pre_alloc_skb.h"
#include "ssv_debug.h"


#define SSV_PRE_RX_ALLOC_LEN		32

static int qlen = SSV_PRE_RX_ALLOC_LEN;
static struct ssv_pre_alloc g_pre_alloc;

u32 ssv_pre_allocate_rx_skb_get_max_running(void)
{
    return g_pre_alloc.max_running_pre_rx_skb_q_len;
}
EXPORT_SYMBOL(ssv_pre_allocate_rx_skb_get_max_running);

u32 ssv_pre_allocate_rx_skb_get_current_q_len(void)
{
    return skb_queue_len(&g_pre_alloc.pre_rx_skb_q);
}
EXPORT_SYMBOL(ssv_pre_allocate_rx_skb_get_current_q_len);

u32 ssv_pre_allocate_rx_skb_get_total_q_len(void)
{
    return g_pre_alloc.total_pre_rx_skb_q_len;
}
EXPORT_SYMBOL(ssv_pre_allocate_rx_skb_get_total_q_len);

struct sk_buff *ssv_pre_allocate_rx_skb_alloc(void)
{
    return ssv_pre_rx_skb_alloc(&g_pre_alloc);
}
EXPORT_SYMBOL(ssv_pre_allocate_rx_skb_alloc);

int ssv_pre_allocate_rx_skb_free(struct sk_buff *skb)
{
    return ssv_pre_rx_skb_free(&g_pre_alloc, skb);
}
EXPORT_SYMBOL(ssv_pre_allocate_rx_skb_free);

int ssv_pre_allocate_rx_skb_free_all(void)
{
    return ssv_pre_rx_skb_free_all(&g_pre_alloc);
}
EXPORT_SYMBOL(ssv_pre_allocate_rx_skb_free_all);

struct sk_buff *ssv_pre_allocate_tx_skb_alloc(void)
{
    return ssv_pre_tx_skb_alloc(&g_pre_alloc);
}
EXPORT_SYMBOL(ssv_pre_allocate_tx_skb_alloc);

int ssv_pre_allocate_tx_skb_free(struct sk_buff *skb)
{
    return ssv_pre_tx_skb_free(&g_pre_alloc, skb);
}
EXPORT_SYMBOL(ssv_pre_allocate_tx_skb_free);

int ssv_pre_allocate_get_qlen(void)
{
    return qlen;
}
EXPORT_SYMBOL(ssv_pre_allocate_get_qlen);

int ssv_pre_allocate_init(void)
{
    int ret;

    SSV_LOG_DBG("%s\n", __func__);
    memset(&g_pre_alloc, 0, sizeof(struct ssv_pre_alloc));

    if (qlen <= 0) {
        SSV_LOG_DBG("Pre-alloc init error: qlen should greater than 0 !!!\n");
        return -EINVAL;
    }

    ret = ssv_pre_tx_skb_init(&g_pre_alloc);
    if (ret != 0) {
        SSV_LOG_DBG("pre-alloc tx skb init fail!\n");
        return ret;
    }

    return ssv_pre_rx_skb_init(&g_pre_alloc, qlen + 2); //pre-alloc rx skb len = qlen + 2
}

void ssv_pre_allocate_exit(void)
{
    int ret;

    SSV_LOG_DBG("%s\n", __func__);

    if (qlen <= 0)
        return;

    ret = ssv_pre_tx_skb_deinit(&g_pre_alloc);
    if (ret != 0) {
        SSV_LOG_DBG("pre-alloc tx skb deinit fail!\n");
        return;
    }

    ssv_pre_rx_skb_deinit(&g_pre_alloc);
}

module_param(qlen, int, 0644);
MODULE_PARM_DESC(qlen, "pre-alloc rx buffer queue len.");

#if (CONFIG_PRE_ALLOC_SKB == 1)
EXPORT_SYMBOL(ssv_pre_allocate_init);
EXPORT_SYMBOL(ssv_pre_allocate_exit);
#elif (CONFIG_PRE_ALLOC_SKB == 2)
module_init(ssv_pre_allocate_init);
module_exit(ssv_pre_allocate_exit);

MODULE_LICENSE("Dual BSD/GPL");
#endif

