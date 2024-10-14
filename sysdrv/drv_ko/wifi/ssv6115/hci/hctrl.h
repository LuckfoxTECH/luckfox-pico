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

#ifndef _HCTRL_H_
#define _HCTRL_H_

#define IFDEV(_ct)                              ((_ct)->dev)
#define IFOPS(_ct)                              ((_ct)->if_ops)
#define HCI_REG_READ(_ct, _adr, _val)           IFOPS(_ct)->readreg(IFDEV(_ct), _adr, _val)
#define HCI_REG_WRITE(_ct,_adr, _val)           IFOPS(_ct)->writereg(IFDEV(_ct), _adr, _val)
#define IF_SEND(_ct, _bf, _len, _qid)   		IFOPS(_ct)->write(IFDEV(_ct), _bf, _len, _qid)
#define IF_RECV(_ct, _bf, _len, _mode)     		IFOPS(_ct)->read(IFDEV(_ct), _bf, _len, _mode)
#define IF_RESET(_ct)     		IFOPS(_ct)->fw_reset(IFDEV(_ct))
#ifdef SSV_PERFORMANCE_WATCH
#define IF_GET_INFO(_ct)     		IFOPS(_ct)->get_info(IFDEV(_ct), _info)
#define IF_CLR_INFO(_ct)     		IFOPS(_ct)->clr_info(IFDEV(_ct))
#endif
#define HCI_TX_AGGR_SKB_LEN (32 * 1024)

/**
* Define flags for enqueue API 
*/
#define HCI_FLAGS_ENQUEUE_HEAD	   0x00000001
struct ssv_sw_txq {
    u32 txq_no;
    struct mutex txq_lock;
    struct sk_buff_head qhead;
    bool inactive;              ///< Inactive status, true: drop packets; false: not to drop packets
    bool paused;                ///< Pasued status, true: keep packets(do nothing); false: process packets

    /* statistic counters: */
    u32 tx_pkt;
    u32 tx_flags;
};


struct ssv6xxx_hci_ctrl {

    struct ssv6xxx_hci_info *shi;
    struct device *dev;
    struct ssv6xxx_hwif_ops *if_ops;

    struct mutex hci_mutex;

    struct ssv_sw_txq sw_txq[SSV_SW_TXQ_NUM];
    /* hardware tx queue packet counter */
    atomic_t sw_txq_cnt;
    wait_queue_head_t   tx_wait_q;
    struct task_struct *hci_tx_task;
    unsigned long tx_timestamp;
    unsigned long rx_timestamp;
    bool tx_trigger_en;
    u32 tx_trigger_qlen;

    u32 rx_pkt;         // receive rx packet 
    u32 post_rx_pkt;    // de-aggr rx packet
    wait_queue_head_t   rx_wait_q;
    struct sk_buff_head rx_skb_q;
    struct task_struct *hci_rx_task;

    bool hw_tx_resource_full;
    u32  hw_tx_resource_full_cnt;
#ifdef CONFIG_SDIO_FAVOR_RX
    volatile bool hw_sdio_rx_available;
#endif
    /* capability inherit from smac */
    u32 hci_cap;

    /* hci tx aggr pre-alloc skb */
    struct sk_buff *p_tx_aggr_skb;

    /* Post Tx callback function */
    void (*hci_post_tx_cb)(struct sk_buff_head *, void *);
    void *hci_post_tx_cb_args;

    /* check resource */
    int  (*check_resource_cb)(void *app_param, struct sk_buff_head *, int reserved_page, int *p_max_count);
    void *check_resource_cb_args;
    
    /* proc_rx */
    proc_rx_cb rx_cb[MAX_RX_CB_NUM];
    void *rx_cb_args[MAX_RX_CB_NUM];
#ifdef SSV_PERFORMANCE_WATCH
    unsigned long skip_fmac_to_hci;
    unsigned long skip_hci_to_hwif;
    unsigned long tx_run_time;
    unsigned long tx_run_size;
    unsigned long tx_run_cnt;
    unsigned long no_tx_resource_cnt;
    unsigned long zero_resource_cnt;
    unsigned long less_resource_10percent_cnt;
    unsigned long over_resource_10percent_cnt;
#endif
};


static inline void ssv6xxx_hwif_rx_task(struct ssv6xxx_hci_ctrl *hctrl, int (*rx_cb)(struct sk_buff *rx_skb, void *args), 
                    int (*is_rx_q_full)(void *args), u32 recv_cnt)
{
    if(hctrl->if_ops->hwif_rx_task)
        hctrl->if_ops->hwif_rx_task(IFDEV(hctrl), rx_cb, is_rx_q_full, (void *)hctrl, recv_cnt);
}

static inline void ssv6xxx_hwif_get_tx_req_cnt(struct ssv6xxx_hci_ctrl *hctrl, int *tx_reg_cnt)
{
    if(hctrl->if_ops->get_tx_req_cnt)
        hctrl->if_ops->get_tx_req_cnt(IFDEV(hctrl), tx_reg_cnt);
}

#define HCI_RX_TASK(ct, rx_cb, is_rx_q_full, recv_cnt)          ssv6xxx_hwif_rx_task(ct, rx_cb, is_rx_q_full, recv_cnt)
#define HCI_HWIF_GET_TX_REQ_CNT(ct, value)                      ssv6xxx_hwif_get_tx_req_cnt(ct, value)

#endif
