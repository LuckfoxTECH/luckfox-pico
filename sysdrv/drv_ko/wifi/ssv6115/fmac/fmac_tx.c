/*
 * Copyright (c) 2021 iComm-semi Ltd.
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

/**
 * @file fmac_tx.c
 * @brief 
 */


/*******************************************************************************
 *         Include Files
 ******************************************************************************/
#include <linux/version.h>
#include <linux/types.h>
#include <linux/dma-mapping.h>
#include <linux/if_arp.h>
#include <linux/etherdevice.h>
#include <net/ieee80211_radiotap.h>

#include "ssv_cfg.h"
#include "fmac.h"
#include "hci/drv_hci_ops.h"
#include "fmac_defs.h"
#include "fmac_tx.h"
#include "fmac_msg_tx.h"
#include "ipc_msg.h"
#include "hwif/hwif.h"
#include "hci/ssv_hci.h"
#include "hci/hctrl.h"
#include "ssv_debug.h"

#ifdef FMAC_BRIDGE
#include "fmac_bridge.h"
#endif
/*******************************************************************************
 *         Local Defines
 ******************************************************************************/
#define PRIO_STA_NULL 0xAA

#define TX_BMU_PAGE_SIZE        512

extern void ssv_hex_dump(unsigned char *data, int len);
extern u32 ssv_get_free_tx_page(struct ssv_softc *sc);
enum data_frame_types ssv_get_data_frame_type(struct sk_buff *skb);
static struct ssv_sta *ssv_get_tx_info(struct ssv_vif *ssv_vif, struct sk_buff *skb, u8 *tid);


/*******************************************************************************
 *         Local Enumerations
 ******************************************************************************/


/*******************************************************************************
 *         Local Structures
 ******************************************************************************/


/*******************************************************************************
 *         Local Variables
 ******************************************************************************/
static u16 g_pkt_sn = 0;

static const int ssv_down_hwq2tid[3] = {
    [SSV_HWQ_BK] = 2,
    [SSV_HWQ_BE] = 3,
    [SSV_HWQ_VI] = 5,
};

// static int tx_pkt_cnt;


/*******************************************************************************
 *         Global Variables
 ******************************************************************************/
extern struct ssv6xxx_cfg ssv_cfg;


/*******************************************************************************
 *         Local Functions
 ******************************************************************************/
#if 0
static void _ssv_hex_dump(unsigned char *data, int len)
{
    int i = 0;
    SSV_LOG_DBG("\n");
    for (i = 1; i <= len; i++)
    {
        if (i % 16 == 1)
        {
            SSV_LOG_DBG("0x%p : ", &data[i - 1]);
        }
        SSV_LOG_DBG("%02x ", data[i - 1]);
        if (i % 16 == 0)
            SSV_LOG_DBG("\n");
    }
    SSV_LOG_DBG("\n");
}
#endif

/**
 * ssv_get_tx_info - Get STA and tid for one skb
 *
 * @ssv_vif: vif ptr
 * @skb: skb
 * @tid: pointer updated with the tid to use for this skb
 *
 * @return: pointer on the destination STA (may be NULL)
 *
 */
static struct ssv_sta *ssv_get_tx_info(struct ssv_vif *ssv_vif,
                                         struct sk_buff *skb,
                                         u8 *tid)
{
    struct ssv_softc *sc = ssv_vif->sc;
    struct wireless_dev *wdev = &ssv_vif->wdev;
    struct ssv_sta *sta = NULL;

    switch (wdev->iftype) {
        case NL80211_IFTYPE_STATION:
        case NL80211_IFTYPE_P2P_CLIENT:
        {
            struct ethhdr *eth;
            eth = (struct ethhdr *)skb->data;
            sta = ssv_vif->sta.ap;
            break;
        }
        case NL80211_IFTYPE_AP_VLAN:
        {
            struct ssv_sta *cur;
            struct ethhdr *eth = (struct ethhdr *)skb->data;

            if (ssv_vif->ap_vlan.sta_4a) {
                sta = ssv_vif->ap_vlan.sta_4a;
                break;
            }

            /* AP_VLAN interface is not used for a 4A STA,
            fallback searching sta amongs all AP's clients */
            ssv_vif = ssv_vif->ap_vlan.master;

            if (is_multicast_ether_addr(eth->h_dest)) {
                sta = &sc->sta_table[ssv_vif->ap.bcmc_index];
            } else {
                list_for_each_entry(cur, &ssv_vif->ap.sta_list, list) {
                    if (!memcmp(cur->mac_addr, eth->h_dest, ETH_ALEN)) {
                        sta = cur;
                        break;
                    }
                }
            }

            break;
        }
        case NL80211_IFTYPE_AP:
        case NL80211_IFTYPE_P2P_GO:
        {
            struct ssv_sta *cur;
            struct ethhdr *eth = (struct ethhdr *)skb->data;

            if (is_multicast_ether_addr(eth->h_dest)) {
                sta = &sc->sta_table[ssv_vif->ap.bcmc_index];
            } else {
                list_for_each_entry(cur, &ssv_vif->ap.sta_list, list) {
                    if (!memcmp(cur->mac_addr, eth->h_dest, ETH_ALEN)) {
                        sta = cur;
                        break;
                    }
                }
            }

            break;
        }
        default:
            break;
    } /* end of switch (wdev->iftype) */

    if (sta && sta->qos)
    {
        /* use the data classifier to determine what 802.1d tag the data frame has */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 14, 0)
        skb->priority = cfg80211_classify8021d(skb, NULL) & IEEE80211_QOS_CTL_TAG1D_MASK;
#else
        skb->priority = cfg80211_classify8021d(skb) & IEEE80211_QOS_CTL_TAG1D_MASK;
#endif
        if (sta->acm)
            ssv_downgrade_ac(sta, skb);
    }
    else if (sta)
    {
        skb->priority = 0xFF;
    }
    else
    {
        /* This packet will be dropped in xmit function, still need to select
           an active queue for xmit to be called. As it most likely to happen
           for AP interface, select BCMC queue
           (TODO: select another queue if BCMC queue is stopped) */
        skb->priority = PRIO_STA_NULL;
    }
    *tid = skb->priority;

    return sta;
}

static void _ssv_tx_mib(struct ssv_softc *sc, u8 *data, bool is_80211, u8 sta_idx, u8 data_type)
{    

    if(0xFF == sta_idx){
    sta_idx = NX_REMOTE_STA_MAX + 1;  //BROADCAST/GROUP DATA TX STA Index for virtual AP
    }

    sc->tx.tx_count++;
    sc->tx_bysta[sta_idx].tx_count++;
    if(false == is_80211) {
        sc->tx.tx_data_count++; //802.3 packet is treat as data frame
        sc->tx_bysta[sta_idx].tx_data_count++;
        switch(data_type){
            case SSV_ARP_REPLY:
                sc->tx.tx_arp_reply_count++;
                sc->tx_bysta[sta_idx].tx_arp_reply_count++;
                break;
            case SSV_ARP_REQUEST:
                sc->tx.tx_arp_req_count++;
                sc->tx_bysta[sta_idx].tx_arp_req_count++;
                break;
            case SSV_ICMP_ECHO:
                sc->tx.tx_icmp_echo++;
                sc->tx_bysta[sta_idx].tx_icmp_echo++;
                break;
            case SSV_ICMP_ECHOREPLY:
                sc->tx.tx_icmp_echoreply++;
                sc->tx_bysta[sta_idx].tx_icmp_echoreply++;
                break;
            case SSV_DHCP_DISCOVER:
                sc->tx.tx_dhcp_discv++;
                sc->tx_bysta[sta_idx].tx_dhcp_discv++;
                SSV_LOG_DBG("SSV_DHCP_DISCOVER\n");
                break;
            case SSV_DHCP_OFFER:
                sc->tx.tx_dhcp_offer++;
                sc->tx_bysta[sta_idx].tx_dhcp_offer++;
                break;
            case SSV_DHCP_REQUEST:
                sc->tx.tx_dhcp_req++;
                sc->tx_bysta[sta_idx].tx_dhcp_req++;
                SSV_LOG_DBG("SSV_DHCP_REQUEST\n");
                break;
            case SSV_DHCP_ACK: 
                sc->tx.tx_dhcp_ack++;
                sc->tx_bysta[sta_idx].tx_dhcp_ack++;
                break;
            case SSV_EAPOL:
                sc->tx.tx_eapol++;
                sc->tx_bysta[sta_idx].tx_eapol++;
                SSV_LOG_DBG("SSV_EAPOL TX Packet to STA%d\n", sta_idx);
                break;  
            default:
                break;       
        }
                   
    }
    else {
        struct ieee80211_hdr *hdr = (struct ieee80211_hdr *)data;

        if (ieee80211_is_data(hdr->frame_control)) {
            sc->tx.tx_data_count++;
            sc->tx_bysta[sta_idx].tx_data_count++;
        } else if (ieee80211_is_mgmt(hdr->frame_control)) {
            sc->tx.tx_mgmt_count++;
            sc->tx_bysta[sta_idx].tx_mgmt_count++;
            
            if (ieee80211_is_auth(hdr->frame_control)) {
                sc->tx.tx_auth_count++;
                sc->tx_bysta[sta_idx].tx_auth_count++;
            } else if (ieee80211_is_deauth(hdr->frame_control)) {
                sc->tx.tx_deauth_count++;
                sc->tx_bysta[sta_idx].tx_deauth_count++;
            } else if (ieee80211_is_assoc_req(hdr->frame_control)) {
                sc->tx.tx_assoc_req_count++;
                sc->tx_bysta[sta_idx].tx_assoc_req_count++;
            } else if (ieee80211_is_assoc_resp(hdr->frame_control)) {
                sc->tx.tx_assoc_resp_count++;
                sc->tx_bysta[sta_idx].tx_assoc_resp_count++;
            } else if (ieee80211_is_probe_req(hdr->frame_control)) {
                sc->tx.tx_probe_req_count++;
                sc->tx_bysta[sta_idx].tx_probe_req_count++;
            } else if (ieee80211_is_probe_resp(hdr->frame_control)) {
                sc->tx.tx_probe_resp_count++;
                sc->tx_bysta[sta_idx].tx_probe_resp_count++;
            }
        }
    }
}



/*******************************************************************************
 *         Global Functions
 ******************************************************************************/
/******************************************************************************
 * Power Save functions
 *****************************************************************************/
/**
 * ssv_set_traffic_status - Inform FW if traffic is available for STA in PS
 *
 * @sc: Driver main data
 * @sta: Sta in PS mode
 * @available: whether traffic is buffered for the STA
 * @ps_id: type of PS data requested (@LEGACY_PS_ID or @UAPSD_ID)
  */
void ssv_set_traffic_status(struct ssv_softc *sc,
                             struct ssv_sta *sta,
                             bool available,
                             u8 ps_id)
{
    bool uapsd = (ps_id != LEGACY_PS_ID);
    ssv_send_me_traffic_ind(sc, sta->sta_idx, uapsd, available);
    // trace_ps_traffic_update(sta->sta_idx, available, uapsd);
}

/**
 * ssv_ps_bh_enable - Enable/disable PS mode for one STA
 *
 * @sc: Driver main data
 * @sta: Sta which enters/leaves PS mode
 * @enable: PS mode status
 *
 * This function will enable/disable PS mode for one STA.
 * When enabling PS mode:
 *  - Stop all STA's txq for SSV_TXQ_STOP_STA_PS reason
 *  - Count how many buffers are already ready for this STA
 *  - For BC/MC sta, update all queued SKB to use hw_queue BCMC
 *  - Update TIM if some packet are ready
 *
 * When disabling PS mode:
 *  - Start all STA's txq for SSV_TXQ_STOP_STA_PS reason
 *  - For BC/MC sta, update all queued SKB to use hw_queue AC_BE
 *  - Update TIM if some packet are ready (otherwise fw will not update TIM
 *    in beacon for this STA)
 *
 * NOTE: _bh_ in function name indicates that this function is called
 * from a bottom_half tasklet.
 */
void ssv_ps_bh_enable(struct ssv_softc *sc, struct ssv_sta *sta,
                       bool enable)
{
    if(enable)
    {
        ssv_drv_hci_tx_pause_by_sta(sc->hci_priv, sc->hci_ops, sta->sta_idx);
        sta->ps.active = true;
        sta->ps.sp_cnt[LEGACY_PS_ID] = 0;
        sta->ps.sp_cnt[UAPSD_ID] = 0;
        //SSV_LOG_DBG("sta idx = %d, vif idx = %d\n", sta->sta_idx, sta->vif_idx);
        if(_ssv_is_multicast_sta(sta->sta_idx))
        {
            bool inactive;
            bool paused;
            int locked;
            u32 qsize;
            u32 pkt_cnt;
            ssv_drv_hci_txq_st(sc->hci_priv, sc->hci_ops, sta->sta_idx, &inactive, &paused, &locked, &qsize, &pkt_cnt);
            sta->ps.pkt_ready[LEGACY_PS_ID] = qsize;
            sta->ps.pkt_ready[UAPSD_ID] = 0;
        }
        else
        {
            bool inactive;
            bool paused;
            int locked;
            u32 qsize;
            u32 pkt_cnt;
            ssv_drv_hci_txq_st(sc->hci_priv, sc->hci_ops, sta->sta_idx, &inactive, &paused, &locked, &qsize, &pkt_cnt);
            sta->ps.pkt_ready[LEGACY_PS_ID] = qsize;
            sta->ps.pkt_ready[UAPSD_ID] = 0;      
        }
        if(sta->ps.pkt_ready[LEGACY_PS_ID])
            ssv_set_traffic_status(sc, sta, true, LEGACY_PS_ID);

        if(sta->ps.pkt_ready[UAPSD_ID])
            ssv_set_traffic_status(sc, sta, true, UAPSD_ID);
        
    }
    else
    {
        sta->ps.active = false;
        ssv_drv_hci_tx_resume_by_sta(sc->hci_priv, sc->hci_ops, sta->sta_idx);
        if(sta->ps.pkt_ready[LEGACY_PS_ID])
            ssv_set_traffic_status(sc, sta, false, LEGACY_PS_ID);

        if(sta->ps.pkt_ready[UAPSD_ID])
            ssv_set_traffic_status(sc, sta, false, UAPSD_ID);
    }
}

/**
 * ssv_ps_bh_traffic_req - Handle traffic request for STA in PS mode
 *
 * @sc: Driver main data
 * @sta: Sta which enters/leaves PS mode
 * @pkt_req: number of pkt to push
 * @ps_id: type of PS data requested (@LEGACY_PS_ID or @UAPSD_ID)
 *
 * This function will make sure that @pkt_req are pushed to fw
 * whereas the STA is in PS mode.
 * If request is 0, send all traffic
 * If request is greater than available pkt, reduce request
 * Note: request will also be reduce if txq credits are not available
 *
 * NOTE: _bh_ in function name indicates that this function is called
 * from the bottom_half tasklet.
 */
void ssv_ps_bh_traffic_req(struct ssv_softc *sc, struct ssv_sta *sta,
                            u16 pkt_req, u8 ps_id)
{
    int pkt_ready_all;

    if (WARN(!sta->ps.active, "sta %pM is not in Power Save mode",
             sta->mac_addr))
        return;

    // trace_ps_traffic_req(sta, pkt_req, ps_id);


    pkt_ready_all = (sta->ps.pkt_ready[ps_id] - sta->ps.sp_cnt[ps_id]);

    /* Don't start SP until previous one is finished or we don't have
       packet ready (which must not happen for U-APSD) */
    if (sta->ps.sp_cnt[ps_id] || pkt_ready_all <= 0) {
        goto done;
    }

    /* Adapt request to what is available. */
    if (pkt_req == 0 || pkt_req > pkt_ready_all) {
        pkt_req = pkt_ready_all;
    }

    /* Reset the SP counter */
    sta->ps.sp_cnt[ps_id] = 0;

    /* "dispatch" the request*/
    if (_ssv_is_multicast_sta(sta->sta_idx)) {
        sta->ps.sp_cnt[ps_id] = pkt_req;
    } else {
        sta->ps.sp_cnt[ps_id] += pkt_req;
    }

done:
    return;
}

void ssv_downgrade_ac(struct ssv_sta *sta, struct sk_buff *skb)
{
    int8_t ac = ssv_tid2hwq[skb->priority];

    if (WARN((ac > SSV_HWQ_VO),
             "Unexepcted ac %d for skb before downgrade", ac))
        ac = SSV_HWQ_VO;

    while (sta->acm & BIT(ac)) {
        if (ac == SSV_HWQ_BK) {
            skb->priority = 1;
            return;
        }
        ac--;
        skb->priority = ssv_down_hwq2tid[ac];
    }
}

void ssv_build_tx_bmu_header(struct tx_bmu_desc *tx_bmu_hdr, int tx_len)
{
    int total_len;

    total_len = tx_len + RESERVED_TX_SIZE + TX_OFFSET_ALIGN;
    tx_bmu_hdr->upatterntx = 0xCAFEFADE;
    tx_bmu_hdr->mpudu_len = total_len;
    tx_bmu_hdr->bufctrlinfo = 0;
    tx_bmu_hdr->bufctrlinfo = (total_len - sizeof(struct tx_bmu_desc)) << 8;
    tx_bmu_hdr->dw7_reserve = 0x12345678;
}

int ssv_requeue_multicast_skb(struct sk_buff *skb, struct ssv_vif *ssv_vif)
{
    struct net_device *ndev = skb->dev;

    return ssv_start_xmit(skb, ndev);
}

void ssv_tx_flow_control(struct ssv_softc *sc, bool fc_en)
{
    int i = 0;
    int tx_frame = 0;

    if (ssv_cfg.flowctl) {
        tx_frame = ssv_drv_hci_txq_len(sc->hci_priv, sc->hci_ops);
        if (fc_en) {
            if (tx_frame > ssv_cfg.flowctl_high_threshold) {
                for(i = 0; i < NX_VIRT_DEV_MAX; i++)
                {
                    struct net_device *ndev = NULL;
                    if(NULL != sc->vif_table[i]) {
                        ndev = sc->vif_table[i]->ndev;
                    }
                    if(NULL != ndev) {
                        // SSV_LOG_DBG("to call netif_tx_stop_all_queues\n");
                        netif_tx_stop_all_queues(ndev);
                        sc->flowctl_txq_stop = true;
                        sc->tx.flowctl_stop_count++;
                    }
                }
            }
        } else {
            if (tx_frame < ssv_cfg.flowctl_low_threshold) {
                for(i = 0; i < NX_VIRT_DEV_MAX; i++)
                {
                    struct net_device *ndev = NULL;
                    if(NULL != sc->vif_table[i]) {
                        ndev = sc->vif_table[i]->ndev;
                    }
                    if(NULL != ndev) {
                        //SSV_LOG_DBG("to call netif_tx_wake_all_queues\n");
                        netif_tx_wake_all_queues(ndev);
                        sc->flowctl_txq_stop = false;
                        sc->tx.flowctl_wake_count++;
                    }
                }
            }
        }
    }
}

extern void _ssv_hex_dump(unsigned char *data, int len);
static netdev_tx_t _ssv_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
    struct ssv_vif *ssv_vif = netdev_priv(dev);
    struct ssv_softc *sc = ssv_vif->sc;
    struct tx_bmu_desc *tx_bmu_hdr;
    struct sdio_hdr *type_hdr;

    struct ethhdr *eth;
	struct ethhdr tmp_eth;
    struct txdesc_api *desc;
    struct ssv_sta *sta;
    int max_headroom;
	int hdr_pads;
    u16 frame_len;
    u8 tid;
	struct sk_buff  *copy_skb = NULL;
	enum data_frame_types data_type = SSV_DATA_UNKNOW;
    #ifdef SSV_PERFORMANCE_WATCH
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)sc->hci_priv;
    unsigned long tx_now_time = jiffies;
    static unsigned long tx_prev_runtime = 0;
    static unsigned long tx_next_runtime = 0;
    int q_num;
    struct ssv_sw_txq *sw_txq;
    u32 max_count = 0;

    if(tx_prev_runtime == 0)
        tx_prev_runtime = tx_now_time;
    if(tx_next_runtime == 0)
        tx_next_runtime = tx_now_time + msecs_to_jiffies(SSV_WATCH_CPU_THROUGHT_TIME_MS);

    if (time_after(tx_now_time, tx_prev_runtime)) {
        sc->tx_run_time += jiffies_to_msecs(tx_now_time - tx_prev_runtime);
        tx_prev_runtime = tx_now_time;
    }

    sc->tx_run_cnt++;        
    sc->tx_run_size += skb->len;

    if (time_after(tx_now_time, tx_next_runtime)) {
        sc->tx_run_cnt = 1;        
        sc->tx_run_size = skb->len;
        sc->tx_run_time = jiffies_to_msecs(tx_now_time - tx_next_runtime);
        memset(sc->sw_txq_res, 0, SSV_SW_TXQ_NUM*6*4);
        tx_next_runtime = tx_now_time + msecs_to_jiffies(SSV_WATCH_CPU_THROUGHT_TIME_MS);
    }

    for (q_num = (SSV_SW_TXQ_NUM - 1); q_num >= 0; q_num--) {
        sw_txq = &hci_ctrl->sw_txq[q_num];
        max_count = (u32)skb_queue_len(&sw_txq->qhead);
        if(max_count == 0)
            sc->sw_txq_res[q_num][0]++;
        else if(max_count <= 32)
            sc->sw_txq_res[q_num][1]++;
        else if(max_count <= 64)
            sc->sw_txq_res[q_num][2]++;
        else if(max_count <= 128)
            sc->sw_txq_res[q_num][3]++;
        else if(max_count <= 256)
            sc->sw_txq_res[q_num][4]++;
        else
            sc->sw_txq_res[q_num][5]++;
    }

    if(sc->skip_fmac_to_hci == true) {
        dev_kfree_skb_any(skb);
        skb = NULL; 
        return NETDEV_TX_OK; 
    }
#endif
    ssv_vif->tx_total_cnt++;
    ssv_vif->tx_total_byte += skb->len;
   // _ssv_tx_mib(sc, (u8 *)skb->data, false);
   
#ifdef FMAC_BRIDGE
    ssv_vif = ssv_bridge_tx_change(skb, dev);
    //SSV_LOG_DBG("%s %d,name [%s]\n",__func__,__LINE__,ssv_vif->ndev->name);
#endif /* FMAC_BRIDGE */

    ssv_tx_flow_control(sc, true);

    if (ssv_cfg.directly_ack_ctrl) {
        // create a copy of tx skb
        copy_skb = skb_copy(skb, GFP_ATOMIC);
        if (!copy_skb) {
            SSV_LOG_DBG("create TX skb copy failed!\n");
        } else {
            dev_kfree_skb_any(skb);
            skb = copy_skb;
        }
    }

    max_headroom =  SSV_TX_HDR_SIZE;

    /* check whether the current skb can be used */
    if (skb_shared(skb) || (skb_headroom(skb) < max_headroom)) {
        struct sk_buff *newskb = skb_copy_expand(skb, max_headroom, 0,
                                                 GFP_ATOMIC);
        if (unlikely(newskb == NULL))
            goto free;

        dev_kfree_skb_any(skb);
        
        skb = newskb;
    }

    /* Get the STA id and TID information */
    sta = ssv_get_tx_info(ssv_vif, skb, &tid);

    if (!sta)
        goto free;
    
    data_type = ssv_get_data_frame_type(skb);
    _ssv_tx_mib(sc, (u8 *)skb->data, false, sta->sta_idx, data_type);

    /* Retrieve the pointer to the Ethernet data */
    eth = (struct ethhdr *)skb->data;

    if((sta->port_control==true)&&(htons(ETH_P_PAE)!=eth->h_proto))
    {
        goto free;
    }

    memcpy(tmp_eth.h_dest, eth->h_dest, ETH_ALEN);
    memcpy(tmp_eth.h_source, eth->h_source, ETH_ALEN);
    tmp_eth.h_proto = eth->h_proto;

	hdr_pads = sizeof(struct ethhdr);

    /* the frame len contains actual skb->data */
    /* only MSDU frame*/
    frame_len = (u16)skb->len;

    /* |                                                                |
     * |<----------         send to HCI               ----------------->|
     * |                                                                |
     * +-------------+----------+------------+--------------------------+
     * | tx_bmu_desc | sdio_hdr | txdesc_api |     MSDU Frame           |
     * +-------------+----------+------------+--------------------------+
     *
     * |<--- skb->data
     * |<-----------  skb->len ---------------------------------------->|
     * |
     *
    */

    /* 1st. reserve the tx descriptor */
	skb_push(skb, sizeof(struct txdesc_api));
    desc = (struct txdesc_api *)skb->data;

    /* 2nd. reserve the type header*/
	skb_push(skb, sizeof(struct sdio_hdr));
    type_hdr = (struct sdio_hdr *)skb->data;

    /* 3rd. reserve the tx_bum header */
    skb_push(skb, sizeof(struct tx_bmu_desc));
    tx_bmu_hdr = (struct tx_bmu_desc *)skb->data;

    type_hdr->type = E_IPC_TYPE_DATA;
    type_hdr->len = skb->len;
    if (tid >= NX_NB_TXQ_PER_STA)
        tid = 0;
    type_hdr->queue_idx = ssv_tid2hwq[tid];
    type_hdr->reserved = g_pkt_sn;
	//SSV_DBG("xmit_pkt_sn: hw_idx=%d, frame_len=%u, sn=%u\n",txq->hwq->id, frame_len, g_pkt_sn);
    g_pkt_sn++;

    // build tx_bmu descriptor
    ssv_build_tx_bmu_header(tx_bmu_hdr, type_hdr->len);

    // debug - remvoe later
    tid = skb->priority;

    // Fill-in the descriptor
    memcpy(&desc->host.eth_dest_addr, tmp_eth.h_dest, ETH_ALEN);
    memcpy(&desc->host.eth_src_addr, tmp_eth.h_source, ETH_ALEN);
    desc->host.ethertype = tmp_eth.h_proto;
    desc->host.staid = sta->sta_idx;
    desc->host.tid = tid;
	//desc->host.host_hdr_pads = hdr_pads;
    if (unlikely(ssv_vif->wdev.iftype == NL80211_IFTYPE_AP_VLAN)) {
        desc->host.vif_idx = ssv_vif->ap_vlan.master->drv_vif_index;
    } else {
        desc->host.vif_idx = ssv_vif->drv_vif_index;
    }

    if (ssv_vif->use_4addr && (sta->sta_idx < NX_REMOTE_STA_MAX))
        desc->host.flags = TXU_CNTRL_USE_4ADDR;
    else
        desc->host.flags = 0;

    desc->host.packet_len = frame_len;

#if 0 //Debug
    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);
    _ssv_hex_dump(skb->data, skb->len);
#endif

    //for PS mode
    {
        u8 ps_id = (sta->uapsd_tids & (1 << tid)) ? UAPSD_ID : LEGACY_PS_ID;
        if (unlikely(sta->ps.active)) {
            sta->ps.pkt_ready[ps_id]++;
            if (sta->ps.pkt_ready[ps_id] == 1) {
                //SSV_LOG_DBG("unlikely, sta was in ps mode!\n, txq->ps_id=%d\n", txq->ps_id);
                ssv_set_traffic_status(sc, sta, true, ps_id);
            }
        }
    }
    
    if(-1 == ssv_fmac_hci_tx(sc, skb, sta->sta_idx, false, 0))
    {
        goto free;
    }

    return NETDEV_TX_OK;

free:
    if(skb)
    {
        dev_kfree_skb_any(skb);
        skb = NULL;
    }
    return NETDEV_TX_OK;
}

static int ssv_get_radiotap_len(unsigned char *data)
{
	struct ieee80211_radiotap_header *hdr =
		(struct ieee80211_radiotap_header *)data;

	return get_unaligned_le16(&hdr->it_len);
}

static netdev_tx_t ssv_mon_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
    struct ssv_vif *ssv_vif = netdev_priv(dev);
    struct ssv_softc *sc = ssv_vif->sc;
    struct ieee80211_radiotap_header *prthdr = 
            (struct ieee80211_radiotap_header *)skb->data;
    struct ieee80211_hdr *hdr;
    u16 len_rthdr;
    int hdrlen;
    struct ssv_vif *tmp_vif = NULL;
    struct ssv_sta *ssv_sta = NULL;
    struct ssv_mgmt_tx_params ssv_params = {0};
    struct ssv_chanctx *ctxt;
    bool found = false;
    u64 cookie = 0;
    int error = 0;

    if (!skb)
        return NETDEV_TX_OK;
        
    /* check for not even having the fixed radiotap header part */
    if (unlikely(skb->len < sizeof(struct ieee80211_radiotap_header)))
        goto fail; /* too short to be possibly valid */

    /* is it a header version we can trust to find length from? */
    if (unlikely(prthdr->it_version))
        goto fail; /* only version 0 is supported */

    /* then there must be a radiotap header with a length we can use */
    len_rthdr = ssv_get_radiotap_len(skb->data);

    /* does the skb contain enough to deliver on the alleged length? */
    if (unlikely(skb->len < len_rthdr))
        goto fail; /* skb too short for claimed rt header extent */

    /* 
     * fix up the pointers accounting for the radiotap
     * header still being in there.  We are being given
     * a precooked IEEE80211 header so no need for normal processing
     */
    skb_set_mac_header(skb, len_rthdr);

    /* 
     * these are just fixed to the end of the rt area since we
     * don't have any better information and at this point, nobody cares
     */
    skb_set_network_header(skb, len_rthdr);
    skb_set_transport_header(skb, len_rthdr);

    if (skb->len < len_rthdr + 2)
        goto fail;
  
    hdr = (struct ieee80211_hdr *)(skb->data + len_rthdr);
    hdrlen = ieee80211_hdrlen(hdr->frame_control);

    if (skb->len < len_rthdr + hdrlen)
        goto fail;

    /* remove radiotap header */
    skb_pull(skb, len_rthdr);
    hdr = (struct ieee80211_hdr *)(skb->data);
    
    // find correct vif with softap mode 
    list_for_each_entry(tmp_vif, &sc->vifs, list) {
        if (!tmp_vif->up)
            continue;
        
        if (NL80211_IFTYPE_AP != SSV_VIF_TYPE(tmp_vif))
            continue;
         
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
        if (ether_addr_equal(tmp_vif->ndev->dev_addr, hdr->addr2))
#else
        if (!compare_ether_addr(tmp_vif->ndev->dev_addr, hdr->addr2))
#endif
        {
            ssv_vif = tmp_vif;
            found = true;
            break;
        }
    }
    
    if (false == found)
        goto fail;

    /* Get STA through list of STAs linked with the provided VIF */
    found = false;
    list_for_each_entry(ssv_sta, &ssv_vif->ap.sta_list, list)
    {
        if (ssv_sta->valid &&
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
            ether_addr_equal(ssv_sta->mac_addr, hdr->addr1)
#else
            !compare_ether_addr(ssv_sta->mac_addr, hdr->addr1)
#endif
        )
        {
            /* the found STA */
            found = true;
            break;
        }
    }
    if (false == found)
        ssv_sta = NULL;

    /* Get channel context */
    ctxt = &sc->chanctx_table[ssv_vif->ch_index];

    // fill mgmt params
    ssv_params.chan = ctxt->chan_def.chan;
    ssv_params.offchan = false;
    ssv_params.wait = 0;
    ssv_params.buf = skb->data;
    ssv_params.len = skb->len;
    ssv_params.no_cck = false;
    ssv_params.dont_wait_for_ack = false;
    
    error = ssv_start_mgmt_xmit(ssv_vif, ssv_sta, &ssv_params, ssv_params.offchan, &cookie);
    if (error) {
        SSV_LOG_DBG("Fail to send 80211 frame from mon.wlan\n");
    }

fail:
    dev_kfree_skb_any(skb);
    return NETDEV_TX_OK;

}

/**
 * netdev_tx_t (*ndo_start_xmit)(struct sk_buff *skb,
 *                               struct net_device *dev);
 *	Called when a packet needs to be transmitted.
 *	Must return NETDEV_TX_OK , NETDEV_TX_BUSY.
 *        (can also return NETDEV_TX_LOCKED if NETIF_F_LLTX)
 *
 *  - Initialize the desciptor for this pkt (stored in skb before data)
 *  - Push the pkt in the corresponding Txq
 *  - If possible (i.e. credit available and not in PS) the pkt is pushed
 *    to fw
 */
netdev_tx_t ssv_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
    struct ssv_vif *ssv_vif = netdev_priv(dev);
    
    if (dev->type == ARPHRD_IEEE80211_RADIOTAP) {
        /* Just handle softap with use monitor */
        if (ssv_vif->use_monitor)
            return ssv_mon_start_xmit(skb, dev);
        else {
            dev_kfree_skb_any(skb);
            return NETDEV_TX_OK;
        }
    } else { 
        return _ssv_start_xmit(skb, dev);
    }
}

void ssv_update_mgmt_txdesc(struct ssv_vif *vif, struct ssv_sta *sta, 
                                        struct sk_buff *skb, bool robust, bool no_cck)
{
    struct tx_bmu_desc *tx_bmu_hdr;
    struct sdio_hdr *type_hdr;
    struct txdesc_api *desc;
    struct mac_hdr *mac_hdr;
    int frame_len = skb->len;
    u8 *data = skb->data;

    /* |                                                                |
     * |<----------         send to HCI               ----------------->|
     * |                                                                |
     * +-------------+----------+------------+--------------------------+
     * | tx_bmu_desc | sdio_hdr | txdesc_api |     MSDU Frame           |
     * +-------------+----------+------------+--------------------------+
     *
     * |<--- skb->data
     * |<-----------  skb->len ---------------------------------------->|
     * |
     *
    */
 	
    /* first we increase the area for tx descriptor */
	skb_push(skb, sizeof(struct txdesc_api));
    desc = (struct txdesc_api *)skb->data;
	/* then we increase the area for sdio header*/
	skb_push(skb, sizeof(struct sdio_hdr));
    type_hdr = (struct sdio_hdr *)skb->data;
    /* 3rd. reserve the tx_bum header */
    skb_push(skb, sizeof(struct tx_bmu_desc));
    tx_bmu_hdr = (struct tx_bmu_desc *)skb->data;

    /*Fill the SDIO Header*/
    type_hdr->type = E_IPC_TYPE_DATA;
    type_hdr->len = skb->len;
    type_hdr->queue_idx = ssv_tid2hwq[8];
    type_hdr->reserved = g_pkt_sn++;
	g_pkt_sn++;

    // build tx_bmu descriptor
    ssv_build_tx_bmu_header(tx_bmu_hdr, type_hdr->len);

    /* Fill the Descriptor to be provided to the MAC SW */
    mac_hdr = (struct mac_hdr *)data;
    mac_hdr->seq = 0;
    desc->host.eth_dest_addr = mac_hdr->addr1;
    desc->host.eth_src_addr = mac_hdr->addr2;
    
    desc->host.staid = (sta) ? sta->sta_idx : 0xFF;
    desc->host.vif_idx = vif->drv_vif_index;
    desc->host.tid = TID_MGT;
    desc->host.flags = TXU_CNTRL_MGMT;
    // desc->host.ethertype = 12345;

    if (robust)
        desc->host.flags |= TXU_CNTRL_MGMT_ROBUST;

    desc->host.packet_len = frame_len;

    if (no_cck)
    {
        desc->host.flags |= TXU_CNTRL_MGMT_NO_CCK;
    }
}

static int _ssv_start_probe_pkt_xmit(struct ssv_vif *vif, struct ssv_sta *sta, struct sk_buff *skb)
{
    struct ssv_softc *sc = vif->sc;
    struct sk_buff *ack_skb;
    int mgmt_txq = (0 == vif->drv_vif_index) ? SSV_SW_TXQ_ID_MNG0 : SSV_SW_TXQ_ID_MNG1;
    struct txdesc_api *desc;
    
    ssv_update_mgmt_txdesc(vif, sta, skb, false, false);
    desc = (struct txdesc_api *)((u8 *)skb->data + sizeof(struct sdio_hdr) + sizeof(struct tx_bmu_desc));

    desc->host.sw_ack = 0x1;
    desc->host.sw_seq = sc->now_ack_seq++;

    ack_skb = skb_clone(skb, GFP_KERNEL);

    if(!ack_skb)
    {
        SSV_LOG_DBG("create TX ack skb failed!\n");
    }
    else
    {
            /* 
             * Use skb->cb to make tag 
             * If sw ack, it drop the probe tx with the tag directly.
             */
            ack_skb->cb[0] = 'p';
            ack_skb->cb[1] = 'r';
            ack_skb->cb[2] = 'o';
            ack_skb->cb[3] = 'b';
            ack_skb->cb[4] = 'e';
            ack_skb->cb[5] = 't';
            ack_skb->cb[6] = 'x';
            skb_queue_tail(&sc->wait_sw_ack_q, ack_skb);
    }

    if(-1 == ssv_fmac_hci_tx(sc, skb, mgmt_txq, false, 0))
    {
        return -EINVAL;
    }

    return 0;
}

void ssv_start_probe_pkt_xmit(struct ssv_vif *vif, struct ssv_sta *sta, bool fromds)
{
#define MAX_STATION_PROBE_PERIOD        (5*1000)
    struct net_device *dev = vif->ndev;
    struct sk_buff *skb;
    unsigned long tick = jiffies;

    if (time_before(tick, (sta->probe_timestamp + msecs_to_jiffies(MAX_STATION_PROBE_PERIOD)))) {
        return;
    }

    if (fromds) {
        skb = ssv_build_nulldata_frame(sta->mac_addr, dev->dev_addr, dev->dev_addr, sta->qos, true);
    } else {
        skb = ssv_build_nulldata_frame(sta->mac_addr, dev->dev_addr, sta->mac_addr, sta->qos, false);
    }
    if (NULL == skb) {
        SSV_LOG_DBG("fail to build nulldata\n");
    }

    if (0 == _ssv_start_probe_pkt_xmit(vif, sta, skb)) {
        sta->probe_timestamp = jiffies;
    } 
}

#ifdef SEND_KEEP_ALIVE
void ssv_send_keep_alive(struct ssv_softc *sc)
{
    struct ssv_vif *vif = NULL;

    // Look for VIF entry
    list_for_each_entry(vif, &sc->vifs, list) {
        if (vif->up) {
            if (NL80211_IFTYPE_STATION == SSV_VIF_TYPE(vif)) {
                if (NULL != vif->sta.ap) {
                    ssv_start_probe_pkt_xmit(vif, vif->sta.ap, false);
                }
            }
        }
    }
}
#endif

void ssv_probe_client(struct ssv_softc *sc)
{
    struct ssv_vif *ap_vif = NULL;
    struct ssv_vif *vif = NULL;
    struct ssv_sta *ssv_sta;

    // Look for VIF entry
    list_for_each_entry(vif, &sc->vifs, list) {
        if (vif->up) {
            if (NL80211_IFTYPE_AP == vif->wdev.iftype)
                ap_vif = vif; 
        }
    }

    if (NULL == ap_vif) 
        return;

    list_for_each_entry(ssv_sta, &ap_vif->ap.sta_list, list)
    {
        if (ssv_sta->valid) {
            ssv_start_probe_pkt_xmit(ap_vif, ssv_sta, true);
        }
    }
}

static int _ssv_start_mgmt_xmit(struct ssv_vif *vif, struct ssv_sta *sta,
                         struct ssv_mgmt_tx_params *params, bool offchan,
                         u64 *cookie)
{
    struct ssv_softc *sc = vif->sc;
    struct txdesc_api *desc;
    struct sk_buff *skb;
    int max_headroom;
    u16 frame_len;
    u8 *data;
    bool robust = false;
    bool no_cck = params->no_cck;
    int mgmt_txq = (0 == vif->drv_vif_index) ? SSV_SW_TXQ_ID_MNG0 : SSV_SW_TXQ_ID_MNG1;
    struct ssv_sta *cur = NULL;
    u8 sta_num = 0;
    //u8 sta_idx = (sta) ? sta->sta_idx : 0xFF;
    //int hdr_pads;
    struct ieee80211_hdr *hdr = (struct ieee80211_hdr *)params->buf;

    /*
    softap mode:
    1.if packet is unicast probe rsp, sta = NULL, use list_for_each_entry for search sta.
    2.if packet is broadcast, sta_idx = 11
    */
    if(0 != vif->drv_vif_index){
        if (SSV_VIF_TYPE(vif) == NL80211_IFTYPE_AP || SSV_VIF_TYPE(vif) == NL80211_IFTYPE_P2P_GO || SSV_VIF_TYPE(vif) == NL80211_IFTYPE_MESH_POINT) {
            list_for_each_entry(cur, &vif->ap.sta_list, list){
                if (!memcmp(cur->mac_addr, hdr->addr1, ETH_ALEN)){
                    _ssv_tx_mib(sc, (u8 *)params->buf, true, cur->sta_idx, 0);
                    sta_num++;
                    break;
                }
            }
        } else if (SSV_VIF_TYPE(vif) == NL80211_IFTYPE_STATION || SSV_VIF_TYPE(vif) == NL80211_IFTYPE_P2P_CLIENT) {
            if (vif->sta.ap != NULL) {
                if (!memcmp(vif->sta.ap->mac_addr, hdr->addr1, ETH_ALEN)){
                    _ssv_tx_mib(sc, (u8 *)params->buf, true, vif->sta.ap->sta_idx, 0);
                    sta_num++;
                }
            }
        }
        if(0 == sta_num){
            _ssv_tx_mib(sc, (u8 *)params->buf, true, 11, 0);
        }        
    }

    //SSV_DBG(SSV_FN_ENTRY_STR);

    // max_headroom = sizeof(struct ssv_txhdr) + SSV_TX_HDR_SIZE;
    max_headroom = SSV_TX_HDR_SIZE;
    frame_len = params->len;

    /* Create a SK Buff object that will contain the provided data */
    skb = dev_alloc_skb(max_headroom + frame_len);
    if (!skb)
    {
        return -ENOMEM;
    }

    /*
     * Move skb->data pointer in order to reserve room for ssv_txhdr
     * headroom value will be equal to sizeof(struct ssv_txhdr)
     */
    skb_reserve(skb, max_headroom);

    /*
     * Extend the buffer data area in order to contain the provided packet
     * len value (for skb) will be equal to param->len
     */
    data = skb_put(skb, frame_len);
    /* Copy the provided data */
    memcpy(data, params->buf, frame_len);

    robust = ieee80211_is_robust_mgmt_frame((void *)skb); //robust = 0;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 16, 0)
    /* Update CSA counter if present */
    if (unlikely(params->n_csa_offsets) &&
        vif->wdev.iftype == NL80211_IFTYPE_AP &&
        vif->ap.csa)
    {
        int i;
        data = skb->data;
        for (i = 0; i < params->n_csa_offsets; i++)
        {
            data[params->csa_offsets[i]] = vif->ap.csa->count;
        }
    }
#endif

    ssv_update_mgmt_txdesc(vif, sta, skb, robust, no_cck);
    desc = (struct txdesc_api *)((u8 *)skb->data + sizeof(struct sdio_hdr) + sizeof(struct tx_bmu_desc));

    if(!params->dont_wait_for_ack)
    {
        struct sk_buff *ack_skb;

        desc->host.sw_ack = 0x1;
        desc->host.sw_seq = sc->now_ack_seq++;

        ack_skb = skb_clone(skb, GFP_KERNEL);

        if(!ack_skb)
        {
            SSV_LOG_DBG("create TX ack skb failed!\n");
        }
        else
        {
            *cookie = (unsigned long)ack_skb;

            //SSV_LOG_DBG("enqueue ack_skb, seq = %d, vif idx === %d\n", desc->host.sw_seq, desc->host.vif_idx);
            skb_queue_tail(&sc->wait_sw_ack_q, ack_skb);
        }
        }
    else
    {
        desc->host.sw_ack = 0;
    }

#if 0 //Debug
    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);
    _ssv_hex_dump(skb->data, skb->len);
#endif

    if (unlikely(sta && sta->ps.active)) {
        u8 tid = skb->priority;
        u8 ps_id = (sta->uapsd_tids & (1 << tid)) ? UAPSD_ID : LEGACY_PS_ID;
        sta->ps.pkt_ready[ps_id]++;
        if (sta->ps.pkt_ready[ps_id] == 1) {
			//SSV_LOG_DBG("unlikely, sta was in ps mode!\n, ps_id=%d\n", ps_id);
            ssv_set_traffic_status(sc, sta, true, ps_id);
        }
    }

    if(-1 == ssv_fmac_hci_tx(sc, skb, mgmt_txq, false, 0))
    {
        return -EINVAL;
    }

    return 0;
}

/**
 * ssv_start_mgmt_xmit - Transmit a management frame
 *
 * @vif: Vif that send the frame
 * @sta: Destination of the frame. May be NULL if the destiantion is unknown
 *       to the AP.
 * @params: Mgmt frame parameters
 * @offchan: Indicate whether the frame must be send via the offchan TXQ.
 *           (is is redundant with params->offchan ?)
 * @cookie: updated with a unique value to identify the frame with upper layer
 *
 */
int ssv_start_mgmt_xmit(struct ssv_vif *vif, struct ssv_sta *sta,
                         struct ssv_mgmt_tx_params *params, bool offchan,
                         u64 *cookie)
{
    return _ssv_start_mgmt_xmit(vif, sta, params, offchan, cookie);
}

/* return value 
 * < 0, hw error or no operation struct
 * 0, success
 * 1, no hw resource
 */
int ssv_tx_check_resource(void *app_param, struct sk_buff_head *qhead,
                        int reserved_page, int *p_max_count)
{
    #define TX_MAX_NUM                          (16) ///@TBD: Where to define this value???
    struct ssv_softc *sc = (struct ssv_softc *)app_param;
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)sc->hci_priv;
    struct sk_buff *skb = NULL;
    int tx_req_cnt =0;
    u32 free_page_cnt = 0;
    u16 tx_hdr_len = SSV_TX_HDR_SIZE;
    int data_len = 0;
    int page_count = 0;
    int tx_count = 0;
    int ret = -1;
    bool b_no_tx_resource = false;

    HCI_HWIF_GET_TX_REQ_CNT(hci_ctrl,&tx_req_cnt);
    // HCI_TX_BMU_FREE_PAGE_COUNT(0x08c10050)
    // b[8:0]:   TX_BMU_FREE_PAGE_CNT
    // b[15:9]:  reserved
    // b[23:16]: SD_TX_BMU_FREE_PAGE_CNT
    // b[29:24]: TX_INT_FIFO_CNT_RO_REG2
    // b[31:30]: reseved
    if(NULL != sc)
    {
        if ((ret = ssv_drv_hci_read_word(sc->hci_priv, sc->hci_ops, 0x08c10050, &free_page_cnt)) != 0)
        {
            goto END;
        }
    }
    else
    {
        ret = -EIO;
        goto END;
    }

#ifdef CONFIG_USB_TX_MULTI_URB
    tx_req_cnt = tx_req_cnt *4;
#endif
    free_page_cnt = (free_page_cnt & 0x00FF0000) >> 16;
#ifdef CONFIG_USB_TX_MULTI_URB
    free_page_cnt = (free_page_cnt > tx_req_cnt)?free_page_cnt-tx_req_cnt:0;
#endif
    //SSV_LOG_DBG("free page = %u, reserved_page = %d\n", free_page_cnt, reserved_page);
    free_page_cnt = (free_page_cnt > reserved_page)?free_page_cnt-reserved_page:0;
    if(0 == free_page_cnt)
    {
        b_no_tx_resource = true;
        ret = 1;
        goto END;
    }
    //Limit TX maximum number for one time.
    (*p_max_count) = (TX_MAX_NUM >= (*p_max_count))?(*p_max_count):TX_MAX_NUM;
    // SSV_LOG_DBG("[%s][%d] *p_max_count = %d\n", __FUNCTION__, __LINE__, *p_max_count);
    //Check tx resource
    {
        #define	HW_MMU_PAGE_SHIFT               (9) //9bit, 512 bytes
        #define	HW_MMU_PAGE_MASK                (0x1ff) //512 bytes
        for(tx_count = 0; tx_count < (*p_max_count); tx_count++) {
            if (0 == tx_count) {
                skb = skb_peek(qhead);
            } else {
                struct sk_buff *next = skb->next;
                if (next == (struct sk_buff *)qhead)
                {
                    next = NULL;
                }
                skb = next;
                // skb = ssv6xxx_hci_skb_peek_next(skb, qhead);
            }
            if (!skb){
                break;
            }
            data_len = skb->len-tx_hdr_len; //this should be greater than zero
            page_count = ((0 < data_len)?data_len:0);
            if (page_count & HW_MMU_PAGE_MASK)
                page_count = (page_count >> HW_MMU_PAGE_SHIFT) + 1;
            else
                page_count = page_count >> HW_MMU_PAGE_SHIFT;
            page_count += 1; ///must add 1 for headroom uses

            if ((free_page_cnt < page_count)) {
                if(0 == tx_count)
                {
                    b_no_tx_resource = true;
                }
                break;
            }
            // SSV_LOG_DBG("page_count = %u\n", page_count);
            free_page_cnt -= page_count;
        }
        (*p_max_count) = tx_count;
    }
    //SSV_LOG_DBG("tx_count = %u, b_no_tx_resource = %u, free_page_cnt = %u\n", tx_count, b_no_tx_resource, free_page_cnt);
    ret = (b_no_tx_resource ? 1 : 0);
END:
#ifdef SSV_PERFORMANCE_WATCH
    if(b_no_tx_resource == 0) {
        if(free_page_cnt == 0)
            hci_ctrl->zero_resource_cnt++;
        else if(free_page_cnt <= 12)
            hci_ctrl->less_resource_10percent_cnt++;
        else
            hci_ctrl->over_resource_10percent_cnt++;
    } else {
        hci_ctrl->no_tx_resource_cnt++;
    }
#endif
    return ret;
}

