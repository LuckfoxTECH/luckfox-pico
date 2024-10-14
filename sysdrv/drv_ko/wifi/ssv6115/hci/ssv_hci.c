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
#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include<linux/delay.h>
#include "ssv_cfg.h"
#include <hwif/hwif.h>
#include "ssv_hci.h"
#include "hctrl.h"
#include "ssv_debug.h"
#include "utils/ssv_alloc_skb.h"

#define RX_SKB_FULL_TO_NOFULL_TIME 250
#define MAX_HW_RESOURCE_FULL_CNT  100 


extern struct ssv6xxx_cfg ssv_cfg;

static void ssv6xxx_hci_sw_txq_flush(void *hci_priv);

static unsigned long ssv6xxx_hci_get_sys_mstime(void)
{
    return jiffies_to_msecs(jiffies);
}

static void _ssv6xxx_hci_txq_enqueue(struct ssv6xxx_hci_ctrl *hci_ctrl, struct sk_buff *skb, struct ssv_sw_txq *sw_txq, bool bToHead)
{
    if(bToHead){
        skb_queue_head(&sw_txq->qhead, skb);
    }else{
        skb_queue_tail(&sw_txq->qhead, skb);
    }
    atomic_inc(&hci_ctrl->sw_txq_cnt);

}

static struct sk_buff * _ssv6xxx_hci_txq_dequeue(struct ssv6xxx_hci_ctrl *hci_ctrl, struct ssv_sw_txq *sw_txq)
{
    struct sk_buff *skb=NULL;

    skb = skb_dequeue(&sw_txq->qhead);
	if(NULL!=skb){
		atomic_dec(&hci_ctrl->sw_txq_cnt);
	}
    return skb;
}


static bool ssv6xxx_hci_is_sw_sta_txq(int txqid)
{
    return (((txqid <= SSV_SW_TXQ_ID_STAMAX) && (txqid >= 0)) ? true : false);
}

static bool ssv6xxx_hci_is_sw_txq(int txqid)
{
    return (((txqid <= SSV_SW_TXQ_NUM) && (txqid >= 0)) ? true : false);
}

static int ssv6xxx_reset_skb(struct sk_buff *skb)
{
    struct skb_shared_info *s = skb_shinfo(skb);
    memset(s, 0, offsetof(struct skb_shared_info, dataref));
    atomic_set(&s->dataref, 1);
    memset(skb, 0, offsetof(struct sk_buff, tail));
    skb_reset_tail_pointer(skb);
    return 0;
}

static void ssv6xxx_hci_txq_st(void *hci_priv, int txqid, bool *inactive, bool *paused, int *locked, u32 *qsize, u32 *pkt_cnt)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    struct ssv_sw_txq       *txq;

    BUG_ON(txqid >= SSV_SW_TXQ_NUM || txqid < 0);

    txq = &hci_ctrl->sw_txq[txqid];
    *inactive = txq->inactive;
    *paused = txq->paused;
    *locked = mutex_is_locked(&txq->txq_lock);
    *qsize = skb_queue_len(&txq->qhead);
    *pkt_cnt = txq->tx_pkt;
}

static void ssv6xxx_hci_rxq_st(void *hci_priv, u32 *qsize, u32 *pkt_cnt, u32 *post_rx_pkt)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    
    *qsize = skb_queue_len(&hci_ctrl->rx_skb_q);
    *pkt_cnt = hci_ctrl->rx_pkt;
    *post_rx_pkt = hci_ctrl->post_rx_pkt;
}

static void ssv6xxx_hci_fw_reset(void *hci_priv)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    
    IF_RESET(hci_ctrl);
}

static int ssv6xxx_hci_start(void* hci_priv)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    int i = 0; 

    // for all wifi station sw txq
    for (i = 0; i <= SSV_SW_TXQ_ID_MNGMAX; i++)
    {
        hci_ctrl->sw_txq[i].inactive = false;
        hci_ctrl->sw_txq[i].paused = false;
    }

    return 0;
}

static int ssv6xxx_hci_stop(void* hci_priv)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    int i = 0; 

    // for all wifi sw txq
    for (i = 0; i <= SSV_SW_TXQ_ID_MNGMAX; i++)
    {
        hci_ctrl->sw_txq[i].inactive = true;
        hci_ctrl->sw_txq[i].paused = true;
    }

    return 0;
}

static int ssv6xxx_hci_hcmd_start(void* hci_priv)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    
    // fow cmd sw txq
    hci_ctrl->sw_txq[SSV_SW_TXQ_ID_MNG0].inactive = false;
    hci_ctrl->sw_txq[SSV_SW_TXQ_ID_MNG0].paused = false;
    hci_ctrl->sw_txq[SSV_SW_TXQ_ID_MNG1].inactive = false;
    hci_ctrl->sw_txq[SSV_SW_TXQ_ID_MNG1].paused = false;
    hci_ctrl->sw_txq[SSV_SW_TXQ_ID_WIFI_CMD].inactive = false;
    hci_ctrl->sw_txq[SSV_SW_TXQ_ID_WIFI_CMD].paused = false;
    return 0;
}

static int ssv6xxx_hci_ble_start(void* hci_priv)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    
    // fow ble sw txq
    hci_ctrl->sw_txq[SSV_SW_TXQ_ID_BLE_PDU].inactive = false;
    hci_ctrl->sw_txq[SSV_SW_TXQ_ID_BLE_PDU].paused = false;
    return 0;
}

static int ssv6xxx_hci_ble_stop(void* hci_priv)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    
    // fow ble sw txq
    hci_ctrl->sw_txq[SSV_SW_TXQ_ID_BLE_PDU].inactive = true;
    hci_ctrl->sw_txq[SSV_SW_TXQ_ID_BLE_PDU].paused = true;
    return 0;
}

static int ssv6xxx_hci_read_word(void* hci_priv, u32 addr, u32 *regval)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    
    return HCI_REG_READ(hci_ctrl, addr, regval);
}

static int ssv6xxx_hci_write_word(void* hci_priv, u32 addr, u32 regval)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    
    return HCI_REG_WRITE(hci_ctrl, addr, regval);
}

static int ssv6xxx_hci_set_cap(void* hci_priv, enum ssv_hci_cap cap, bool enable)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    
    if (enable) {
        hci_ctrl->hci_cap |= BIT(cap);
    } else {
        hci_ctrl->hci_cap &= (~BIT(cap));
    }

    return 0;
}

static void ssv6xxx_hci_set_trigger_conf(void* hci_priv, bool en, u32 qlen)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    
    hci_ctrl->tx_trigger_en = en;
    hci_ctrl->tx_trigger_qlen = qlen;
}

static void ssv6xxx_hci_set_tx_timestamp(void* hci_priv, u32 timestamp)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    
    hci_ctrl->tx_timestamp = timestamp;
}

static void ssv6xxx_hci_get_tx_timestamp(void* hci_priv, u32 *timestamp)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    
    *timestamp = hci_ctrl->tx_timestamp;
}


static void ssv6xxx_hci_trigger_tx(void* hci_priv)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    wake_up_interruptible(&hci_ctrl->tx_wait_q);
}

static int ssv6xxx_hci_enqueue(void* hci_priv, struct sk_buff *skb, int txqid,
            bool force_trigger, u32 tx_flags)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    struct ssv_sw_txq *sw_txq;
    int qlen = 0,sw_txq_count=0;

    BUG_ON(txqid >= SSV_SW_TXQ_NUM || txqid < 0);
    if (txqid >= SSV_SW_TXQ_NUM || txqid < 0)
        return -1;
#ifdef SSV_PERFORMANCE_WATCH
    if(hci_ctrl->skip_fmac_to_hci == true) {
        dev_kfree_skb_any(skb);
        return 0;
    }
#endif
    sw_txq = &hci_ctrl->sw_txq[txqid];
    sw_txq->tx_flags = tx_flags;
    if (tx_flags & HCI_FLAGS_ENQUEUE_HEAD){
        _ssv6xxx_hci_txq_enqueue(hci_ctrl,skb,sw_txq,true);
    }
    else{
        _ssv6xxx_hci_txq_enqueue(hci_ctrl,skb,sw_txq,false);
    }

    qlen = (int)skb_queue_len(&sw_txq->qhead);
    sw_txq_count=atomic_read(&hci_ctrl->sw_txq_cnt);
    hci_ctrl->tx_timestamp = jiffies;
    if (0 != hci_ctrl->tx_trigger_en) {
        if ((txqid <= (SSV_SW_TXQ_ID_STAMAX)) &&
            (false == force_trigger) &&
            (sw_txq_count < hci_ctrl->tx_trigger_qlen)) {
                
            return qlen;
        }
    }

    ssv6xxx_hci_trigger_tx((void *)hci_ctrl);
    return qlen;
}

static int ssv6xxx_hci_txq_len(void* hci_priv)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    int tx_count = 0;

    tx_count = atomic_read(&hci_ctrl->sw_txq_cnt);
    return tx_count;
}

static bool ssv6xxx_hci_is_txq_empty(void* hci_priv, int txqid)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    struct ssv_sw_txq *sw_txq;
    
    BUG_ON(txqid >= SSV_SW_TXQ_NUM);
    if (txqid >= SSV_SW_TXQ_NUM)
        return false;

    sw_txq = &hci_ctrl->sw_txq[txqid];
    if (skb_queue_len(&sw_txq->qhead) <= 0)
        return true;
    
    return false;
}

static int ssv6xxx_hci_ble_txq_flush(void* hci_priv)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    struct ssv_sw_txq *sw_txq;
    struct sk_buff *skb = NULL;
    
    // flush ble sw_txq
    sw_txq = &hci_ctrl->sw_txq[SSV_SW_TXQ_ID_BLE_PDU];
    while((skb = _ssv6xxx_hci_txq_dequeue(hci_ctrl,sw_txq))) {
        dev_kfree_skb_any(skb);
    }
    
    return 0;
}

static int ssv6xxx_hci_txq_flush_by_sta(void* hci_priv, int txqid)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    struct ssv_sw_txq *sw_txq;
    struct sk_buff *skb = NULL;

    if (false == ssv6xxx_hci_is_sw_sta_txq(txqid))
        return -1;

    sw_txq = &hci_ctrl->sw_txq[txqid];
    while((skb = _ssv6xxx_hci_txq_dequeue(hci_ctrl,sw_txq))) {
        dev_kfree_skb_any(skb);
    }

    return 0;
}

static void ssv6xxx_hci_txq_lock_by_sta(void* hci_priv, int txqid)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    
    if (false == ssv6xxx_hci_is_sw_sta_txq(txqid))
        return;

    mutex_lock(&hci_ctrl->sw_txq[txqid].txq_lock);
}

static void ssv6xxx_hci_txq_unlock_by_sta(void* hci_priv, int txqid)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    
    if (false == ssv6xxx_hci_is_sw_sta_txq(txqid))
        return;
    
    mutex_unlock(&hci_ctrl->sw_txq[txqid].txq_lock);
}

static void ssv6xxx_hci_txq_pause_by_sta(void* hci_priv, int txqid)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    struct ssv_sw_txq *sw_txq;    

    if (false == ssv6xxx_hci_is_sw_sta_txq(txqid))
        return;

    sw_txq = &hci_ctrl->sw_txq[txqid];
    sw_txq->paused = true;
}

static void ssv6xxx_hci_txq_resume_by_sta(void* hci_priv, int txqid)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    struct ssv_sw_txq *sw_txq;
    
    if (false == ssv6xxx_hci_is_sw_sta_txq(txqid))
        return;

    sw_txq = &hci_ctrl->sw_txq[txqid];
    sw_txq->paused = false;
}

static void ssv6xxx_hci_txq_pause_by_txqid(void* hci_priv, int txqid)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    struct ssv_sw_txq *sw_txq;    

    if (false == ssv6xxx_hci_is_sw_txq(txqid))
        return;

    sw_txq = &hci_ctrl->sw_txq[txqid];
    sw_txq->paused = true;
}

static void ssv6xxx_hci_txq_resume_by_txqid(void* hci_priv, int txqid)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    struct ssv_sw_txq *sw_txq;
    
    if (false == ssv6xxx_hci_is_sw_txq(txqid))
        return;

    sw_txq = &hci_ctrl->sw_txq[txqid];
    sw_txq->paused = false;
}

static void ssv6xxx_hci_txq_active_by_sta(void* hci_priv, int txqid)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    struct ssv_sw_txq *sw_txq;

    if (false == ssv6xxx_hci_is_sw_sta_txq(txqid))
        return;

    sw_txq = &hci_ctrl->sw_txq[txqid];
    sw_txq->inactive = false;
}

static void ssv6xxx_hci_txq_inactive_by_sta(void* hci_priv, int txqid)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    struct ssv_sw_txq *sw_txq;

    if (false == ssv6xxx_hci_is_sw_sta_txq(txqid))
        return;

    sw_txq = &hci_ctrl->sw_txq[txqid];
    sw_txq->inactive = true;
}

static void ssv6xxx_hci_txq_active_by_txqid(void* hci_priv, int txqid)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    struct ssv_sw_txq *sw_txq;

    if (false == ssv6xxx_hci_is_sw_txq(txqid))
        return;

    sw_txq = &hci_ctrl->sw_txq[txqid];
    sw_txq->inactive = false;
}

static void ssv6xxx_hci_txq_inactive_by_txqid(void* hci_priv, int txqid)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    struct ssv_sw_txq *sw_txq;

    if (false == ssv6xxx_hci_is_sw_txq(txqid))
        return;

    sw_txq = &hci_ctrl->sw_txq[txqid];
    sw_txq->inactive = true;
}

static int _ssv6xxx_hci_rx_enqueue(struct ssv6xxx_hci_ctrl *hci_ctrl, struct sk_buff *skb, bool is_tail)
{
    if(true == is_tail) {
        skb_queue_tail(&hci_ctrl->rx_skb_q, skb);
    }
    else {
        skb_queue_head(&hci_ctrl->rx_skb_q, skb);
    }
    hci_ctrl->rx_pkt++;

    wake_up_interruptible(&hci_ctrl->rx_wait_q);

    return 0;
}

static int ssv6xxx_hci_rx_enqueue(void* hci_priv, struct sk_buff *skb, bool is_tail)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    return _ssv6xxx_hci_rx_enqueue(hci_ctrl, skb, is_tail);
}


/**
 * int ssv6xxx_hci_aggr_xmit() - send the specified number of frames for a specified 
 *                          tx queue to SDIO with hci_tx_aggr enabled.
 *
 * @ struct ssv_sw_txq *sw_txq: the output queue to send.
 * @ int max_count: the maximal number of frames to send.
 */
static int ssv6xxx_hci_aggr_xmit(struct ssv6xxx_hci_ctrl *hci_ctrl, struct ssv_sw_txq *sw_txq, int max_count)
{
    struct sk_buff_head tx_cb_list;
    struct sk_buff *skb = NULL;
    int tx_count = 0, ret = 0,retry_count=0;
    u32 regval = 0 ;
    // start of variable for hci tx aggr
    struct sk_buff *p_aggr_skb = NULL;
    static u32 left_len = 0;
    u32 cpy_len = 0; 
    static u8 *p_cpy_dest = NULL;
    static bool aggr_done = false;
    static bool new_round  = 1;
    // end of variable for hci tx aggr

    skb_queue_head_init(&tx_cb_list);
    if(hci_ctrl->p_tx_aggr_skb ==NULL){
        hci_ctrl->p_tx_aggr_skb = ssv_tx_skb_alloc(HCI_TX_AGGR_SKB_LEN, GFP_KERNEL);
        if (NULL == hci_ctrl->p_tx_aggr_skb) {
            SSV_LOG_ERR("Cannot alloc tx aggr skb size %d\n", HCI_TX_AGGR_SKB_LEN);
            BUG_ON(1);
        }
    }

    p_aggr_skb = hci_ctrl->p_tx_aggr_skb;

    // memset((void *)p_aggr_skb->data, 0, HCI_TX_AGGR_SKB_LEN);

    for(tx_count=0; tx_count < max_count; tx_count++) {
        if (sw_txq->inactive) {
            while((skb = _ssv6xxx_hci_txq_dequeue(hci_ctrl,sw_txq))) {
                /*
                * Tx frame should enqueue tx_cb_list after IF_SEND()
                * hci_post_tx_cb() will handle tx frame. ex: drop, send to mac80211 layer
                */
                if (hci_ctrl->hci_post_tx_cb) {
                    skb_queue_tail(&tx_cb_list, skb);
                } else {
                    dev_kfree_skb_any(skb);
                }
            }
            aggr_done = false;
            ssv6xxx_reset_skb(p_aggr_skb);
            new_round = 1;
            goto xmit_out;
        }
        if (sw_txq->paused) {
            // Is it possible to stop/paused in middle of for loop?
            //SSV_LOG_DBG("ssv6xxx_hci_aggr_xmit - sw_txq->pause = false\n");
            aggr_done = true;
            goto xmit_out;
        }

        skb = _ssv6xxx_hci_txq_dequeue(hci_ctrl,sw_txq);
        if (!skb){
            SSV_LOG_DBG("ssv6xxx_hci_xmit - queue empty\n");
            goto xmit_out;
        }
        // SSV_LOG_DBG("[%s][%d] skb->len = %d", __FUNCTION__, __LINE__, skb->len);

        //ampdu1.3
        if(1 == new_round) {
            p_cpy_dest = p_aggr_skb->data;
            left_len = HCI_TX_AGGR_SKB_LEN;
            new_round = 0;
        }

        //4-bytes alignment.
        if ((skb->len) & 0x3) {
            cpy_len = (skb->len) + (4 - ((skb->len) & 0x3));
        } else {
            cpy_len = skb->len;
        }

        if (left_len > (cpy_len + 8)) {
            *((u32 *)(p_cpy_dest)) = ((cpy_len + 8) | ((cpy_len + 8) << 16));
            *((u32 *)(p_cpy_dest+4)) = 0;
            p_cpy_dest += 8;
            // *((u32 *)(p_cpy_dest+skb->len)) = 0; //Set to 0 at the tail 4-bytes.
            // if(cpy_len != skb->len)
            {
                 //Zero padding at the tail 4-bytes.
                *((u32 *)(p_cpy_dest+(cpy_len-4))) = 0;
            }
            memcpy(p_cpy_dest, skb->data, skb->len);
            //memcpy(p_cpy_dest, skb->data, cpy_len);
            p_cpy_dest +=  cpy_len;

            left_len -= (8 + cpy_len);

            skb_put(p_aggr_skb, 8 + cpy_len);
            
            /*  
            * Tx frame should enqueue tx_cb_list after IF_SEND()
            * hci_post_tx_cb() will handle tx frame. ex: drop, send to mac80211 layer 
            */
            if (hci_ctrl->hci_post_tx_cb) {
                skb_queue_tail(&tx_cb_list, skb);
            } else {
                dev_kfree_skb_any(skb);
            }

            sw_txq->tx_pkt++;
        } else {
            // packet will be add again in next round
            _ssv6xxx_hci_txq_enqueue(hci_ctrl,skb,sw_txq,true);
            aggr_done = true;
            break;
        }

    } //end of for(tx_count=0; tx_count<max_count; tx_count++) 
    if (tx_count == max_count) {
        aggr_done = true;
    }
xmit_out:
    if (aggr_done) {
        if ((p_aggr_skb) && (new_round==0)) {
            *((u32 *)(p_cpy_dest)) = 0;
            p_cpy_dest += 4;
            skb_put(p_aggr_skb, 4);
#if 0
            // SSV_LOG_DBG("hci tx aggr len=%d\n", p_aggr_skb->len);
            ret = IF_SEND(hci_ctrl, (void *)p_aggr_skb, p_aggr_skb->len, sw_txq->txq_no);
            if (ret < 0) {
                //BUG_ON(1);
                SSV_LOG_DBG("ssv6xxx_hci_aggr_xmit fail, err %d\n", ret);
                //ret = -1;
            } 
#else
            for(retry_count =0 ;retry_count<3;retry_count++){
#ifdef SSV_PERFORMANCE_WATCH
                hci_ctrl->tx_run_size += p_aggr_skb->len;
                if(hci_ctrl->skip_hci_to_hwif == true)
                    break;
#endif
                ret = IF_SEND(hci_ctrl, (void *)p_aggr_skb, p_aggr_skb->len, sw_txq->txq_no);
                if (ret < 0) {
                    SSV_LOG_DBG("ssv6xxx_hci_aggr_xmit fail, err %d,retry_count =%d, len %d\n", ret,retry_count,p_aggr_skb->len);
                        continue;
                }else{
                    break;
                 } 
              }
			
              if(retry_count == 3){			
                  ret = HCI_REG_READ(hci_ctrl, 0xc0000008, &regval);
                  SSV_LOG_DBG("ssv6xxx_hci_aggr_xmit 3 time, err %d ,0xC0000008 = %08x",ret,regval);
                  ret = 0;
              }
#endif	
            aggr_done = false;
            ssv6xxx_reset_skb(p_aggr_skb);
            new_round = 1;
        }
    }

    if (hci_ctrl->hci_post_tx_cb) {
        hci_ctrl->hci_post_tx_cb (&tx_cb_list, hci_ctrl->hci_post_tx_cb_args);
    }

    return (ret == 0) ? tx_count : ret;
}
/**
 * int ssv6xxx_hci_xmit() - send the specified number of frames for a specified 
 *                          tx queue to SDIO.
 *
 * @ struct ssv_sw_txq *sw_txq: the output queue to send.
 * @ int max_count: the maximal number of frames to send.
 */
static int ssv6xxx_hci_xmit(struct ssv6xxx_hci_ctrl *hci_ctrl, struct ssv_sw_txq *sw_txq, int max_count)
{
    struct sk_buff_head tx_cb_list;
    struct sk_buff *skb = NULL;
    int tx_count = 0, ret = 0,retry_count=0;
    u32 regval = 0 ;

    skb_queue_head_init(&tx_cb_list);

    for(tx_count=0; tx_count<max_count; tx_count++) {
        if (sw_txq->inactive) {
            while((skb = _ssv6xxx_hci_txq_dequeue(hci_ctrl,sw_txq))) {
                /*
                * Tx frame should enqueue tx_cb_list after IF_SEND()
                * hci_post_tx_cb() will handle tx frame. ex: drop, send to mac80211 layer
                */
                if (hci_ctrl->hci_post_tx_cb) {
                    skb_queue_tail(&tx_cb_list, skb);
                } else {
                    dev_kfree_skb_any(skb);
                }
            }
            goto xmit_out;
        }

        if (sw_txq->paused) {
            //SSV_LOG_DBG("ssv6xxx_hci_xmit - sw_txq->pause = false\n");
            goto xmit_out;
        }

        skb = _ssv6xxx_hci_txq_dequeue(hci_ctrl,sw_txq);
        if (!skb){
            SSV_LOG_DBG("ssv6xxx_hci_xmit - queue empty\n");
            goto xmit_out;
        }

        //ampdu1.3
#if 0
        //ampdu1.3
        ret = IF_SEND(hci_ctrl, (void *)skb, skb->len, sw_txq->txq_no);

        if (ret < 0) {
            //BUG_ON(1);
            SSV_LOG_DBG("ssv6xxx_hci_xmit fail, err %d\n", ret);
            // remove tx desc, which will be add again in next round
            //skb_pull(skb, TXPB_OFFSET);
            //skb_queue_head(&sw_txq->qhead, skb);
            //ret = -1;
            break;
        }
#else
        for(retry_count =0 ;retry_count<3;retry_count++){
#ifdef SSV_PERFORMANCE_WATCH
            hci_ctrl->tx_run_size += skb->len;
            if(hci_ctrl->skip_hci_to_hwif == true)
                break;
#endif
            ret = IF_SEND(hci_ctrl, (void *)skb, skb->len, sw_txq->txq_no);
            if (ret < 0) {
                SSV_LOG_DBG("ssv6xxx_hci_xmit fail, err %d,retry_count =%d, len %d\n", ret,retry_count,skb->len);
                continue;
            }else{
                break;
            } 
        }
		
        if(retry_count == 3){			
            ret = HCI_REG_READ(hci_ctrl, 0xc0000008, &regval);
            SSV_LOG_DBG("ssv6xxx_hci_aggr_xmit 3 time, err %d ,0xC0000008 = %08x",ret,regval);
            ret = 0;
        }
#endif
        
        /*  
         * Tx frame should enqueue tx_cb_list after IF_SEND()
         * hci_post_tx_cb() will handle tx frame. ex: drop, send to mac80211 layer 
         */
        if (hci_ctrl->hci_post_tx_cb) {
            skb_queue_tail(&tx_cb_list, skb);
        } else {
            dev_kfree_skb_any(skb);
        }
        sw_txq->tx_pkt++;
    } //end of for(tx_count=0; tx_count<max_count; tx_count++) 
xmit_out:

    if (hci_ctrl->hci_post_tx_cb) {
        hci_ctrl->hci_post_tx_cb (&tx_cb_list, hci_ctrl->hci_post_tx_cb_args);
    }

    return (ret == 0) ? tx_count : ret;
}

static bool ssv6xxx_hci_is_frame_send(struct ssv6xxx_hci_ctrl *hci_ctrl)
{
    int q_num;
    struct ssv_sw_txq *sw_txq;

    if (hci_ctrl->hw_tx_resource_full)
        return false;

#if defined(CONFIG_PREEMPT_NONE) && defined(CONFIG_SDIO_FAVOR_RX)
    if (hci_ctrl->hw_sdio_rx_available) {
        hci_ctrl->hw_sdio_rx_available = false;
        return false;
    }
#endif
    for (q_num = (SSV_SW_TXQ_NUM - 1); q_num >= 0; q_num--) {
        sw_txq = &hci_ctrl->sw_txq[q_num];

        if (!sw_txq->paused && !ssv6xxx_hci_is_txq_empty(hci_ctrl, q_num))
            return true;
    }

    return false;
}

static int ssv6xxx_hci_tx_task (void *data)
{
#define TX_RESOURCE_FULL_WAIT_PERIOND       3
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)data;
    int err = 0;
    unsigned long     wait_period = msecs_to_jiffies(TX_RESOURCE_FULL_WAIT_PERIOND);
    int q_num;
    struct ssv_sw_txq *sw_txq;
    u32 max_count = 0;
    int reserved_page=0;
    int timeout = 0;
#ifdef SSV_PERFORMANCE_WATCH
    unsigned long tx_now_time;
    static unsigned long tx_next_runtime = 0;
    static unsigned long tx_base_runtime = 0;
#endif
    SSV_LOG_DBG("SSV6XXX HCI TX Task started.\n");

    while (!kthread_should_stop())
    {
        hci_ctrl->tx_timestamp = jiffies;
#ifdef SSV_PERFORMANCE_WATCH
        tx_now_time = jiffies;
        if(tx_base_runtime == 0)
            tx_base_runtime = tx_now_time;
        
        if(tx_next_runtime == 0)
            tx_next_runtime = tx_now_time + msecs_to_jiffies(SSV_WATCH_CPU_THROUGHT_TIME_MS);

        if (time_after(tx_now_time, tx_base_runtime)) {
            hci_ctrl->tx_run_time = jiffies_to_msecs(tx_now_time - tx_base_runtime);
        }
        if (time_after(tx_now_time, tx_next_runtime)) {
            tx_base_runtime = tx_now_time;
            tx_next_runtime = tx_now_time + msecs_to_jiffies(SSV_WATCH_CPU_THROUGHT_TIME_MS);
            hci_ctrl->tx_run_cnt = 0;
            hci_ctrl->tx_run_size = 0;
            hci_ctrl->no_tx_resource_cnt = 0;
            hci_ctrl->zero_resource_cnt = 0;
            hci_ctrl->less_resource_10percent_cnt = 0;
            hci_ctrl->over_resource_10percent_cnt = 0;
            hci_ctrl->tx_run_time = 0;
            IF_CLR_INFO(hci_ctrl);
        }
#endif
        if (false == hci_ctrl->hw_tx_resource_full) {
            wait_event_interruptible(hci_ctrl->tx_wait_q,
                (  kthread_should_stop()
                   || ssv6xxx_hci_is_frame_send(hci_ctrl)));
        } else {
            timeout = wait_event_interruptible_timeout(hci_ctrl->tx_wait_q,
                (  kthread_should_stop()
                   || ssv6xxx_hci_is_frame_send(hci_ctrl)), 
                   wait_period);
            
        }

#ifdef SSV_MODULE_TEST
        if(ssv_cfg.mod_test_mask & SSV_MOD_TEST_HCI_TX)
        {
            u32 delay = 1;
            if(SSV_MOD_TEST_DELAY_FIXED == ssv_cfg.mod_test_delay_mode)
            {
                delay = ((ssv_cfg.mod_test_delay_min+ssv_cfg.mod_test_delay_max)>>1); //average min + max
            }
            else if(SSV_MOD_TEST_DELAY_RANDOM == ssv_cfg.mod_test_delay_mode)
            {
                u32 rand = 0;
                get_random_bytes(&rand, sizeof(rand));
                delay = ssv_cfg.mod_test_delay_min + (rand % (ssv_cfg.mod_test_delay_max - ssv_cfg.mod_test_delay_min + 1));
            }
            msleep(delay);
        }
#endif

        if (kthread_should_stop())
        {
            hci_ctrl->hci_tx_task = NULL;
            SSV_LOG_DBG("Quit HCI TX task loop...\n");
            break;
        }

        {
            err = 0;
            hci_ctrl->hw_tx_resource_full = false;
            for (q_num = (SSV_SW_TXQ_NUM - 1); q_num >= 0; q_num--)
            {
                sw_txq = &hci_ctrl->sw_txq[q_num];
                max_count = (u32)skb_queue_len(&sw_txq->qhead);
                if((0 < max_count)) {
                    if(true == sw_txq->inactive) {
                        struct sk_buff *skb = NULL;
                        //drop all packets in the queue
                        while((skb = _ssv6xxx_hci_txq_dequeue(hci_ctrl,sw_txq))) {
                            dev_kfree_skb_any(skb);
                        }
                        continue;
                    }
                    else if(true == sw_txq->paused) {
                        //do nothing
                        continue;
                    }
                }
                else // (0 == max_count)
                {
                    continue;
                }
                //HCI_HWIF_GET_TX_REQ_CNT(hci_ctrl,&tx_req_cnt);
                if(q_num==SSV_SW_TXQ_ID_WIFI_CMD)
                    reserved_page = 0;
                else
                    reserved_page = 6;
#ifdef SSV_PERFORMANCE_WATCH
                hci_ctrl->tx_run_cnt++;
#endif
                if (hci_ctrl->check_resource_cb) {
                    err = hci_ctrl->check_resource_cb(hci_ctrl->check_resource_cb_args,
                                                &sw_txq->qhead, reserved_page, &max_count);
                } else {
                    /* no check resource, it just send one packet */
                    err = 0;
                    max_count = 1;
                }
                /* err
                 * < 0, hw error or no operation struct
                 * 0, success
                 * 1, no hw resource
                 */
                if (err == 0) {
                    if (hci_ctrl->hci_cap & BIT(HCI_CAP_TX_AGGR)) { 
                        ssv6xxx_hci_aggr_xmit(hci_ctrl, sw_txq, max_count);
                    } else {
                        ssv6xxx_hci_xmit(hci_ctrl, sw_txq, max_count);
                    }
                    hci_ctrl->hw_tx_resource_full_cnt = 0;
                } else if (err == 1){
                    hci_ctrl->hw_tx_resource_full_cnt++;
                        if(hci_ctrl->hw_tx_resource_full_cnt > MAX_HW_RESOURCE_FULL_CNT){
                            hci_ctrl->hw_tx_resource_full = true;
                            hci_ctrl->hw_tx_resource_full_cnt = 0;
                        }
                } else if (err < 0) {
                    if (err == -ENODEV) {
                        ssv6xxx_hci_sw_txq_flush(hci_ctrl);
                    }
                    break;
                }
            }
        }
    }

    hci_ctrl->hci_tx_task = NULL;

    SSV_LOG_DBG("SSV6XXX HCI TX Task end.\n");
    return 0;
}

void ssv6xxx_hci_process_rx_q(struct ssv6xxx_hci_ctrl *hci_ctrl, struct sk_buff_head *rx_q)
{
    struct sk_buff                *skb, *sskb;
    int                            data_offset, data_length;
    unsigned char                 *pdata, *psdata;
    struct hci_rx_aggr_info       *rx_aggr_info;
#ifdef CONFIG_PREEMPT_NONE
    u32 max_rx_num = 32;//hardcode
    u32 cur_rx_num = 0;
#endif //CONFIG_PREEMPT_NONE
    int i=0;

    while (1) {
#ifdef CONFIG_PREEMPT_NONE
        if ((cur_rx_num++) > max_rx_num)
        {
            cur_rx_num = 0;
            schedule();
        }
#endif //CONFIG_PREEMPT_NONE

        skb = skb_dequeue(rx_q);
        if (!skb) {
            break;
        }

        data_length = skb->len;
        if (hci_ctrl->hci_cap & BIT(HCI_CAP_RX_AGGR))
        {
            struct hci_rx_aggr_info *rx_first_aggr_info = (struct hci_rx_aggr_info *)skb->data;
            if (0 == ((rx_first_aggr_info->extra_info >> 16) % 512))
            {
                data_length -= 4; ///@FIXME: Remove 4-bytes if data length is a multiple of 512-bytes.
            }
            data_length -= 4; ///@FIXME: Remove tail. AGG_TAILER_SIZE(4-bytes)
        } else {
            SSV_LOG_ERR("Err: HCI doesn't support rx aggr.\n");
            ssv_rx_skb_free(skb);
            break;
            //BUG_ON(1);
        }

        pdata = skb->data;
        for (data_offset = 0; data_offset < data_length; ) {
            
            { /* disassamble rx aggr frame */

                if ((data_offset + sizeof(struct hci_rx_aggr_info)) > data_length) {
                    SSV_LOG_DBG("wrong data length, data_offset %d, data_length %d\n",
                        data_offset, data_length);
                    break; 
                }

                rx_aggr_info = (struct hci_rx_aggr_info *)pdata;
                if ((rx_aggr_info->jmp_mpdu_len == 0) ||
                        ((rx_aggr_info->jmp_mpdu_len - sizeof(struct hci_rx_aggr_info)) > MAX_FRAME_SIZE) ||
                        ((data_offset + rx_aggr_info->jmp_mpdu_len) > data_length)) {

                    break;
                }
                
                sskb = ssv_rx_skb_alloc(rx_aggr_info->jmp_mpdu_len+(PLATFORM_DEF_DMA_ALIGN_SIZE-1), GFP_KERNEL);
                if (!sskb) {
                    SSV_LOG_DBG("%s(): cannot alloc skb buffer\n", __FUNCTION__);
                    data_offset += rx_aggr_info->jmp_mpdu_len;
                    pdata = (u8 *)skb->data + data_offset;
                    continue;
                }
               
                /* it consider that rx desc header will be discard. 
                 * magic number 112 is rx desc header length
                 * ETH_HLEN is used to align ethernet skb->data
                 * */
                if ((unsigned long)(sskb->data+112+ETH_HLEN) % PLATFORM_DEF_DMA_ALIGN_SIZE) {
                        unsigned long shift = PLATFORM_DEF_DMA_ALIGN_SIZE - 
                                ((unsigned long)(sskb->data+112+ETH_HLEN) % PLATFORM_DEF_DMA_ALIGN_SIZE);
            
                    skb_reserve(sskb, shift);
                }
            
                psdata = skb_put(sskb, (rx_aggr_info->jmp_mpdu_len - sizeof(struct hci_rx_aggr_info)));
                memcpy(psdata, pdata+sizeof(struct hci_rx_aggr_info),
                        (rx_aggr_info->jmp_mpdu_len - sizeof(struct hci_rx_aggr_info)));

                /* relocate next packet header */
                data_offset += rx_aggr_info->jmp_mpdu_len;
                pdata = (u8 *)skb->data + data_offset;
            }
            
            for(i=0;i<MAX_RX_CB_NUM;i++)
            {       
                if (hci_ctrl->rx_cb[i]) {
                    if(0==hci_ctrl->rx_cb[i](hci_ctrl->rx_cb_args[i], sskb))
                    {
                        break;
                    }
                } 
            }

            if(i==MAX_RX_CB_NUM)
            {
                dev_kfree_skb_any(sskb);
            }

            hci_ctrl->post_rx_pkt++;
        }

        ssv_rx_skb_free(skb);
    }// end of while (1)

} // end of - ssv6xxx_hci_process_rx_q -


int ssv6xxx_hci_rx_task (void *data)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)data;
    unsigned long     wait_period = msecs_to_jiffies(3);

    SSV_LOG_DBG("SSV6XXX HCI RX Task started.\n");
    while (!kthread_should_stop())
    {
        u32 before_timeout = (-1);

        before_timeout = wait_event_interruptible_timeout(hci_ctrl->rx_wait_q,
                (   skb_queue_len(&hci_ctrl->rx_skb_q)
                    || kthread_should_stop()),
                wait_period);

        if (kthread_should_stop())
        {
            SSV_LOG_DBG("Quit RX task loop...\n");
            break;
        }

#ifdef SSV_MODULE_TEST
        if(ssv_cfg.mod_test_mask & SSV_MOD_TEST_HCI_RX)
        {
            u32 delay = 1;
            if(SSV_MOD_TEST_DELAY_FIXED == ssv_cfg.mod_test_delay_mode)
            {
                delay = ((ssv_cfg.mod_test_delay_min+ssv_cfg.mod_test_delay_max)>>1); //average min + max
            }
            else if(SSV_MOD_TEST_DELAY_RANDOM == ssv_cfg.mod_test_delay_mode)
            {
                u32 rand = 0;
                get_random_bytes(&rand, sizeof(rand));
                delay = ssv_cfg.mod_test_delay_min + (rand % (ssv_cfg.mod_test_delay_max - ssv_cfg.mod_test_delay_min + 1));
            }
            msleep(delay);
        }
#endif

        // Take out RX skb from RX Q and process it.
        if (skb_queue_len(&hci_ctrl->rx_skb_q)) {
            ssv6xxx_hci_process_rx_q(hci_ctrl, &hci_ctrl->rx_skb_q);
        }
    }

    hci_ctrl->hci_rx_task = NULL;

    return 0;
} // end of - ssv6xxx_rx_task -

struct ssv6xxx_hci_ops ssv_hci_ops = 
{
    /* public operation */
    .hci_start            = ssv6xxx_hci_start,
    .hci_stop             = ssv6xxx_hci_stop,
    .hci_ble_start        = ssv6xxx_hci_ble_start,
    .hci_ble_stop         = ssv6xxx_hci_ble_stop,
    .hci_read_word        = ssv6xxx_hci_read_word,
    .hci_write_word       = ssv6xxx_hci_write_word,
    .hci_trigger_tx       = ssv6xxx_hci_trigger_tx,
    .hci_tx               = ssv6xxx_hci_enqueue,
    .hci_tx_pause_by_sta  = ssv6xxx_hci_txq_pause_by_sta,
    .hci_tx_resume_by_sta = ssv6xxx_hci_txq_resume_by_sta,
    .hci_tx_pause_by_txqid  = ssv6xxx_hci_txq_pause_by_txqid,
    .hci_tx_resume_by_txqid = ssv6xxx_hci_txq_resume_by_txqid,
    .hci_tx_active_by_sta  = ssv6xxx_hci_txq_active_by_sta,
    .hci_tx_inactive_by_sta = ssv6xxx_hci_txq_inactive_by_sta,
    .hci_tx_active_by_txqid  = ssv6xxx_hci_txq_active_by_txqid,
    .hci_tx_inactive_by_txqid = ssv6xxx_hci_txq_inactive_by_txqid,
    .hci_ble_txq_flush    = ssv6xxx_hci_ble_txq_flush,
    .hci_txq_flush_by_sta = ssv6xxx_hci_txq_flush_by_sta,
    .hci_txq_lock_by_sta  = ssv6xxx_hci_txq_lock_by_sta,
    .hci_txq_unlock_by_sta= ssv6xxx_hci_txq_unlock_by_sta,
    .hci_txq_len          = ssv6xxx_hci_txq_len,
    .hci_txq_empty        = ssv6xxx_hci_is_txq_empty,
    .hci_set_cap          = ssv6xxx_hci_set_cap,
    .hci_set_trigger_conf = ssv6xxx_hci_set_trigger_conf,
    .hci_set_tx_timestamp = ssv6xxx_hci_set_tx_timestamp,
    .hci_get_tx_timestamp = ssv6xxx_hci_get_tx_timestamp,
    .hci_rx_enqueue       = ssv6xxx_hci_rx_enqueue,
    /* for debug operation */
    .hci_txq_st           = ssv6xxx_hci_txq_st,
    .hci_rxq_st           = ssv6xxx_hci_rxq_st,

    /* for fw reset operation */
    .fw_reset              = ssv6xxx_hci_fw_reset,
};

int ssv6xxx_hci_post_tx_register(void* hci_priv,void (*hci_post_tx_cb)(struct sk_buff *, void *), void *args)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl;

    hci_ctrl = hci_priv;
    hci_ctrl->hci_post_tx_cb = (void *)hci_post_tx_cb;
    hci_ctrl->hci_post_tx_cb_args = args;
    return 0;
}

int ssv6xxx_check_resource_register(void* hci_priv, 
        int (*check_resource_cb)(void *app_param, struct sk_buff_head *qhead,
                             int reserved_page, int *p_max_count),
        void *args)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl;

    hci_ctrl = hci_priv;
    hci_ctrl->check_resource_cb = check_resource_cb;
    hci_ctrl->check_resource_cb_args = args;
    return 0;
}

int ssv6xxx_hci_proc_rx_unregister(void* hci_priv,proc_rx_cb rx_cb, void *args)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl;
    u8 i=0;
    hci_ctrl = hci_priv;
    for(i=0;i<MAX_RX_CB_NUM;i++)
    {
        if(rx_cb == hci_ctrl->rx_cb[i]){
            hci_ctrl->rx_cb[i] = NULL;
            hci_ctrl->rx_cb_args[i] = args;
            break;
        }
    }

    if(i==MAX_RX_CB_NUM)
        return -1;
    else
        return 0;
}

int ssv6xxx_hci_proc_rx_register(void* hci_priv,proc_rx_cb rx_cb, void *args)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl;
    u8 i=0;
    hci_ctrl = hci_priv;
    for(i=0;i<MAX_RX_CB_NUM;i++)
    {
        if(NULL==hci_ctrl->rx_cb[i]){
            hci_ctrl->rx_cb[i] = (void *)rx_cb;
            hci_ctrl->rx_cb_args[i] = args;
            break;
        }
    }

    if(i==MAX_RX_CB_NUM)
        return -1;
    else
        return 0;
}


int ssv6xxx_hci_is_rx_q_full(void *args)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl=(struct ssv6xxx_hci_ctrl *)args;
    unsigned long rx_now = 0;
    int rx_threshold = 128;

#if (CONFIG_PRE_ALLOC_SKB != 0)
    extern int ssv_pre_allocate_get_qlen(void);

    rx_threshold = ssv_pre_allocate_get_qlen();
#endif

    if (skb_queue_len(&hci_ctrl->rx_skb_q) > rx_threshold) {
        if(hci_ctrl->rx_timestamp == 0) {
            hci_ctrl->rx_timestamp = ssv6xxx_hci_get_sys_mstime();
        }
        return 1;
    } else {
        if(hci_ctrl->rx_timestamp != 0) {
            rx_now = ssv6xxx_hci_get_sys_mstime();
            if((rx_now- hci_ctrl->rx_timestamp) > RX_SKB_FULL_TO_NOFULL_TIME) {
                SSV_LOG_DBG("============maybe fw assert happen: %ld============\n",(rx_now- hci_ctrl->rx_timestamp));
            }
        }
        hci_ctrl->rx_timestamp = 0;
        return 0;
    }
    
}

int ssv6xxx_hci_rx(struct sk_buff *rx_skb, void *args)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)args;
    return _ssv6xxx_hci_rx_enqueue(hci_ctrl, rx_skb, true);
}

static void ssv6xxx_hci_sw_txq_flush(void *hci_priv)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    struct ssv_sw_txq *sw_txq;
    struct sk_buff *skb = NULL;
    int txqid = 0;

    for (txqid = 0; txqid < SSV_SW_TXQ_NUM; txqid++) {
        sw_txq = &hci_ctrl->sw_txq[txqid];
        while((skb = _ssv6xxx_hci_txq_dequeue(hci_ctrl,sw_txq))) {
            dev_kfree_skb_any(skb);
        }
    }
}

static void ssv6xxx_hci_sw_rxq_flush(void *hci_priv)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)hci_priv;
    struct sk_buff *skb = NULL;
        
    while((skb = skb_dequeue(&hci_ctrl->rx_skb_q))) { 
        dev_kfree_skb_any(skb);
    }

}

int ssv6xxx_hci_deinit(void *hci_priv)
{
    struct ssv6xxx_hci_ctrl *hci_ctrl;
    
    SSV_LOG_DBG("%s(): \n", __FUNCTION__);

    hci_ctrl = hci_priv;

    if (hci_ctrl->hci_rx_task != NULL)
    {
        SSV_LOG_DBG("Stopping HCI RX task...\n");
        kthread_stop(hci_ctrl->hci_rx_task);
        while(hci_ctrl->hci_rx_task != NULL) {
            msleep(1);
        }
        SSV_LOG_DBG("Stopped HCI RX task.\n");
    }
    if (hci_ctrl->hci_tx_task != NULL)
    {
        SSV_LOG_DBG("Stopping HCI TX task...\n");
        kthread_stop(hci_ctrl->hci_tx_task);
        while(hci_ctrl->hci_tx_task != NULL) {
            msleep(1);
        }
        SSV_LOG_DBG("Stopped HCI TX task.\n");
    }
    
    // flush txq/rxq packet 
    ssv6xxx_hci_sw_txq_flush(hci_ctrl);
    ssv6xxx_hci_sw_rxq_flush(hci_ctrl);

    // free hci tx aggr buffer
    if (hci_ctrl->p_tx_aggr_skb) {
        ssv_tx_skb_free(hci_ctrl->p_tx_aggr_skb);
    }
    
    kfree(hci_ctrl);
    hci_priv = NULL;

    return 0;
}
EXPORT_SYMBOL(ssv6xxx_hci_deinit);

int ssv6xxx_hci_init(void **hci_priv, void* hwif_priv,struct device *dev)
{
    int i;
    struct ssv6xxx_hci_ctrl *hci_ctrl;

    hci_ctrl = kzalloc(sizeof(*hci_ctrl), GFP_KERNEL);
    if (hci_ctrl == NULL){
        SSV_LOG_DBG("Fail to alloc hci_ctrl\n");
        return -ENOMEM;
    }

    memset((void *)hci_ctrl, 0, sizeof(*hci_ctrl));
    *hci_priv = hci_ctrl;

    // hwif ops
    hci_ctrl->dev = dev;
    hci_ctrl->if_ops = (struct ssv6xxx_hwif_ops *)hwif_priv;

    mutex_init(&hci_ctrl->hci_mutex);

    /* TX queue initialization */
    for (i=0; i < SSV_SW_TXQ_NUM; i++) {
        memset(&hci_ctrl->sw_txq[i], 0, sizeof(struct ssv_sw_txq));
        skb_queue_head_init(&hci_ctrl->sw_txq[i].qhead);
        mutex_init(&hci_ctrl->sw_txq[i].txq_lock);
        hci_ctrl->sw_txq[i].txq_no = (u32)i;
        hci_ctrl->sw_txq[i].inactive = true;
        hci_ctrl->sw_txq[i].paused = true;
    }

    /* TX Task initialization */
    init_waitqueue_head(&hci_ctrl->tx_wait_q);
    hci_ctrl->hci_tx_task = kthread_run(ssv6xxx_hci_tx_task, hci_ctrl, "ssv6xxx_hci_tx_task");

    /* RX Task initialization */
    init_waitqueue_head(&hci_ctrl->rx_wait_q);
    skb_queue_head_init(&hci_ctrl->rx_skb_q);
    hci_ctrl->hci_rx_task = kthread_run(ssv6xxx_hci_rx_task, hci_ctrl, "ssv6xxx_hci_rx_task");
    HCI_RX_TASK(hci_ctrl, ssv6xxx_hci_rx, ssv6xxx_hci_is_rx_q_full, 2);
    
    /* HostCmd start initialization */
    ssv6xxx_hci_hcmd_start(hci_ctrl);
    
    return 0;
}


