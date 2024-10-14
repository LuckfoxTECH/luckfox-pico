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
 * @file fmac_rx.c
 * @brief FMAC RX functions.
 */


/*******************************************************************************
 *         Include Files
 ******************************************************************************/
#include <linux/version.h>
#include <net/ieee80211_radiotap.h>
#include <linux/inetdevice.h>
#include <linux/etherdevice.h>
#include <linux/jiffies.h>
#include "dbg_ansi_color_codes.h"
#include "ssv_cfg.h"
//#include "ssvdevice/ssv_efuse.h"
#include "fmac.h"
#include "hci/drv_hci_ops.h"
#include "lmac_msg.h"
#include "host_msg.h"
#include "fmac_rx.h"
#include "ssvdevice/dev.h"
#include "fmac_tx.h"
#include "fmac_utils.h"
#include "ipc_msg.h"
#include "ssv_debug.h"
#include "utils/ssv_alloc_skb.h"
#include "fmac_msg_tx.h"
#include "compat.h"

#ifdef FMAC_BRIDGE
#include "fmac_bridge.h"
#endif

/*******************************************************************************
 *         Local Defines
 ******************************************************************************/
#define PRIO_STA_NULL 0xAA

static inline
struct ssv_vif *ssv_rx_get_vif(struct ssv_softc *sc, int vif_idx);
extern enum data_frame_types ssv_get_data_frame_type(struct sk_buff *skb);

#ifndef IEEE80211_MAX_CHAINS
#define IEEE80211_MAX_CHAINS 4
#endif

/*******************************************************************************
 *         Local Enumerations
 ******************************************************************************/


/*******************************************************************************
 *         Local Structures
 ******************************************************************************/


/*******************************************************************************
 *         Global Variables
 ******************************************************************************/
const u8 legrates_lut[] = {
    0,                          /* 0 */
    1,                          /* 1 */
    2,                          /* 2 */
    3,                          /* 3 */
    -1,                         /* 4 */
    -1,                         /* 5 */
    -1,                         /* 6 */
    -1,                         /* 7 */
    10,                         /* 8 */
    8,                          /* 9 */
    6,                          /* 10 */
    4,                          /* 11 */
    11,                         /* 12 */
    9,                          /* 13 */
    7,                          /* 14 */
    5                           /* 15 */
};

extern struct ssv6xxx_cfg ssv_cfg;
extern int ssv_resp_reg_rw_handler(struct ssv_softc *sc, u8 *data);

/*******************************************************************************
 *         Local Variables
 ******************************************************************************/


/*******************************************************************************
 *         Local Functions
 ******************************************************************************/
/**
 * ssv_rx_get_vif - Return pointer to the destination vif
 *
 * @sc: main driver data
 * @vif_idx: vif index present in rx descriptor
 *
 * Select the vif that should receive this frame returns NULL if the destination
 * vif is not active.
 * If no vif is specified, this is probably a mgmt broadcast frame, so select
 * the first active interface
 */
static inline
struct ssv_vif *ssv_rx_get_vif(struct ssv_softc *sc, int vif_idx)
{
    struct ssv_vif *ssv_vif = NULL;

    if (vif_idx == 0xFF) {
        list_for_each_entry(ssv_vif, &sc->vifs, list) {
            if (ssv_vif->up)
                return ssv_vif;
        }
        return NULL;
    } else if (vif_idx < NX_VIRT_DEV_MAX) {
        ssv_vif = sc->vif_table[vif_idx];
        if (!ssv_vif || !ssv_vif->up)
            return NULL;
    }

    return ssv_vif;
}

static void _ssv_rx_mib(struct ssv_softc *sc, u8 *data, bool is_80211, u8 sta_idx, u8 data_type)
{   

    if(0xFF == sta_idx){
    sta_idx = NX_REMOTE_STA_MAX + 1;  //BROADCAST/GROUP DATA TX STA Index for virtual AP
    }

    sc->rx.rx_count++;
    sc->rx_bysta[sta_idx].rx_count++;

    if(false == is_80211) {
        sc->rx.rx_data_count++; //802.3 packet is treat as data frame.
        sc->rx_bysta[sta_idx].rx_data_count++;
        switch(data_type){
            case SSV_ARP_REPLY:
                sc->rx.rx_arp_reply_count++;
                sc->rx_bysta[sta_idx].rx_arp_reply_count++;
                break;
            case SSV_ARP_REQUEST:
                sc->rx.rx_arp_req_count++;
                sc->rx_bysta[sta_idx].rx_arp_req_count++;
                break;
            case SSV_ICMP_ECHO:
                sc->rx.rx_icmp_echo++;
                sc->rx_bysta[sta_idx].rx_icmp_echo++;
                break;
            case SSV_ICMP_ECHOREPLY:
                sc->rx.rx_icmp_echoreply++;
                sc->rx_bysta[sta_idx].rx_icmp_echoreply++;
                break;
            case SSV_DHCP_DISCOVER:
                sc->rx.rx_dhcp_discv++;
                sc->rx_bysta[sta_idx].rx_dhcp_discv++;
                break;
            case SSV_DHCP_OFFER:
                sc->rx.rx_dhcp_offer++;
                sc->rx_bysta[sta_idx].rx_dhcp_offer++;
                SSV_LOG_DBG("SSV_DHCP_OFFER\n");
                break;
            case SSV_DHCP_REQUEST:
                sc->rx.rx_dhcp_req++;
                sc->rx_bysta[sta_idx].rx_dhcp_req++;
                break;
            case SSV_DHCP_ACK:
                sc->rx.rx_dhcp_ack++;
                sc->rx_bysta[sta_idx].rx_dhcp_ack++;
                SSV_LOG_DBG("SSV_DHCP_ACK\n");
                break;
            case SSV_EAPOL:
                sc->rx.rx_eapol++;  
                sc->rx_bysta[sta_idx].rx_eapol++;
                SSV_LOG_DBG("SSV_EAPOL RX Packet from STA%d\n", sta_idx);
                break;
            default:
                break;
        }                                
    }
    else 
    {
        struct ieee80211_hdr *hdr = (struct ieee80211_hdr *)data;

        if (ieee80211_is_data(hdr->frame_control)){
            sc->rx.rx_data_count++;
            sc->rx_bysta[sta_idx].rx_data_count++;
        }else if (ieee80211_is_mgmt(hdr->frame_control)){
            sc->rx.rx_mgmt_count++;
            sc->rx_bysta[sta_idx].rx_mgmt_count++;
            
            if (ieee80211_is_beacon(hdr->frame_control)){
                sc->rx.rx_bcn_count++;
                sc->rx_bysta[sta_idx].rx_bcn_count++;
            }else if (ieee80211_is_probe_resp(hdr->frame_control)){
                sc->rx.rx_proberesp_count++;
                sc->rx_bysta[sta_idx].rx_proberesp_count++;
            }else if (ieee80211_is_probe_req(hdr->frame_control)){
                sc->rx.rx_probereq_count++;
                sc->rx_bysta[sta_idx].rx_probereq_count++;
            }else if (ieee80211_is_assoc_req(hdr->frame_control)){
                sc->rx.rx_assoc_req_count++;
                sc->rx_bysta[sta_idx].rx_assoc_req_count++;
            }else if (ieee80211_is_assoc_resp(hdr->frame_control)){
                sc->rx.rx_assoc_resp_count++;
                sc->rx_bysta[sta_idx].rx_assoc_resp_count++;
            }else if (ieee80211_is_auth(hdr->frame_control)){
                sc->rx.rx_auth_count++;
                sc->rx_bysta[sta_idx].rx_auth_count++;
            }else if (ieee80211_is_disassoc(hdr->frame_control)){
                sc->rx.rx_disassoc_count++;
                sc->rx_bysta[sta_idx].rx_disassoc_count++;
            }else if (ieee80211_is_deauth(hdr->frame_control)){
                sc->rx.rx_deauth_count++;
                sc->rx_bysta[sta_idx].rx_deauth_count++;
            }
        }
        
    }   
}

static void ssv_netif_receive_skb(struct sk_buff *skb)
{
    struct net_device *ndev = skb->dev;
    struct ssv_vif *ssv_vif = netdev_priv(ndev);
    struct ssv_softc *sc = (struct ssv_softc *)ssv_vif->sc;
    struct ethhdr *eth = (void *)skb->data;
    int mcast = is_multicast_ether_addr(eth->h_dest);
    struct sk_buff *skb_copy = NULL;
    bool resend = false, forward = true;
    
    if (!ssv_vif) {
        dev_kfree_skb_any(skb);
        return;
    }
    if ((NL80211_IFTYPE_AP == SSV_VIF_TYPE(ssv_vif) || (NL80211_IFTYPE_AP_VLAN ==  SSV_VIF_TYPE(ssv_vif))) && 
        !(ssv_vif->ap.flags & SSV_AP_ISOLATE)) {
        if (mcast) {
            /* send multicast frames both to higher layers in
             * local net stack and back to the wireless medium
             */
            resend = true;
        } else {
            struct ssv_sta *sta = ssv_get_sta(sc, eth->h_dest);            
            if (NULL != sta) {
                resend = true;
                forward = false;
            }
        }
    }
    ssv_vif->rx_total_cnt++;
    ssv_vif->rx_total_byte+=skb->len;
    if (resend) {
        /* 
         * Send to wireless media and increase priority by 256 to 
         * keep the received priority instead of reclassifying
         * the frame (see cfg80211_classify8021d).
         * */
        skb_copy = skb_copy_expand(skb, sizeof(struct tx_bmu_desc) + /* RESERVED_TX_SIZE */ +
                    sizeof(struct txdesc_api) +
                    sizeof(struct sdio_hdr) + SSV_SWTXHDR_ALIGN_SZ, 0, GFP_ATOMIC);
        if (skb_copy) {
            skb_copy->protocol = htons(ETH_P_802_3);
            skb_reset_network_header(skb_copy);
            skb_reset_mac_header(skb_copy);
            skb_copy->dev = ndev;
            ssv_requeue_multicast_skb(skb_copy, ssv_vif);
        }
    }
    
    
    if (forward) {
 #ifdef FMAC_BRIDGE
    if (ssv_bridge_rx_change(ssv_vif, skb) < 0) {
        dev_kfree_skb_any(skb);
        return;
    }                
#endif	/* FMAC_BRIDGE */
        skb->protocol = eth_type_trans(skb, ssv_vif->ndev);
        memset(skb->cb, 0, sizeof(skb->cb));
        local_bh_disable();
        netif_receive_skb(skb);
        local_bh_enable();
    } else {
        dev_kfree_skb_any(skb);
    }
}

static int _ssv_push_private_msg_to_host(struct ssv_softc *sc, u8 *data, u32 len)
{
    struct sk_buff *skb = NULL;
    struct hci_rx_aggr_info *aggr_info= NULL;
    u32 *rx_desc = NULL;
    struct rx_info *rx_info = NULL;
    u8 *rx_buf_headroom = NULL;
    u32 frame_len = 0;
    u32 frame_oft = 0;
    u16 headroom =  sizeof(u32) + sizeof(struct rx_info) + RX_BUF_HEADROOM_SIZE;
    u16 tail = 0;
    bool is_aggr = true;
    gfp_t flags;

    if(true == is_aggr) {
        headroom += sizeof(struct hci_rx_aggr_info);
        tail += 4;
    }
    frame_len = ALIGN((headroom + tail + len), 4);

    if (in_softirq())
        flags = GFP_ATOMIC;
    else
        flags = GFP_KERNEL;

    skb = ssv_rx_skb_alloc(frame_len, flags);
    if (NULL == skb)
    {
        SSV_LOG_DBG("%s(): Can't alloc skb.\n", __FUNCTION__);
        return -1;
    }
    skb_put(skb, frame_len);
    memset((void *)skb->data, 0, skb->len);

    /* add tx header and payload */
    if(true == is_aggr) {
        //build hci_rx_aggr_info
        aggr_info =(struct hci_rx_aggr_info *)(skb->data+frame_oft);
        aggr_info->jmp_mpdu_len = frame_len-tail;
        aggr_info->jmp_mpdu_len1 = aggr_info->jmp_mpdu_len;
        aggr_info->extra_info = (frame_len<<16);
        frame_oft += sizeof(struct hci_rx_aggr_info);
    }

    //build rx_desc
    rx_desc = (u32 *)(skb->data+frame_oft);
    *rx_desc = len << 8;
    *rx_desc |= (u8)E_IPC_TYPE_PRIV_MSG;
    frame_oft += sizeof(u32);

    //build rx_info (empty)
    rx_info =(struct rx_info *)(skb->data+frame_oft);
    frame_oft += sizeof(struct rx_info);

    //build rx buf headroom (empty)
    rx_buf_headroom = (u8 *)(skb->data+frame_oft);
    frame_oft += RX_BUF_HEADROOM_SIZE;

    //copy msg payload
    memcpy((void *)(skb->data+frame_oft), data, len);

#if 0 //Debug
    _ssv_hex_dump(skb->data, skb->len);
#endif

#ifdef CONFIG_HWIF_AND_HCI
    return ssv_drv_hci_rx_enqueue(sc->hci_priv, sc->hci_ops, skb, false); //enqueue to hci rxq head
#else
    ssv_rx_skb_free(skb);
    return 0;
#endif
}

static void _ssv_free_host_private_msg(u8 *msg_buf)
{
    if(NULL != msg_buf)
    {
        kfree(msg_buf);
    }
}

static int _ssv_create_host_private_msg(u32 msgid, u8 *data, u32 datalen, u8 **msg_buf, u32 *msg_buf_len)
{
    u32 msg_total_len = (u32)(sizeof(ST_IPC_PRIV_MSG)+datalen);
    ST_IPC_PRIV_MSG *msg;
    gfp_t flags;

    if (in_softirq())
        flags = GFP_ATOMIC;
    else
        flags = GFP_KERNEL;
    
    msg = kzalloc(msg_total_len+1, flags);
    if(NULL == msg)
    {
        SSV_LOG_DBG("[%s][%d] kzalloc() for msg(%u) failed!!\n", __FUNCTION__, __LINE__, msgid);
        return -ENOMEM;
    }

    msg->msgid = (EN_IPC_PRIV_MSG_TYPE)msgid;
    msg->msglen = datalen;
    if((NULL != data) && (0 != datalen))
    {
        memcpy((void *)&msg->data[0], (const void *)data, (size_t)datalen);
    }
    *msg_buf = (u8 *)msg;
    *msg_buf_len = msg_total_len;

    return 0;
}


/*******************************************************************************
 *         Global Functions
 ******************************************************************************/
//#define REODER_DROPPKT_TESTMODE
//#define REODER_DUPPPKT_TESTMODE
#if defined(REODER_DROPPKT_TESTMODE) || defined(REODER_DUPPPKT_TESTMODE)
unsigned int randNum = 0;
#endif
#if defined(REODER_DUPPPKT_TESTMODE)
struct sk_buff *dup_skb;
struct host_reorder_info dup_rx_reorder_info;
#endif
#if 0 //Debug
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

void ssv_rxreord_all_dump(struct ssv_softc *sc, uint16_t  sta_idx)
{
    uint16_t i = 0, j = 0;

    struct rxu_cntrl_reord *rx_reord = NULL;
    struct rxu_cntrl_reord_elt *elt = NULL;
   
    for(j = 0; j <= TID_7; j++) {
        rx_reord = &sc->sta_table[sta_idx].reord_info.ba_agmts_rx[j];
        SSV_LOG_DBG("sta idx: %d tid idx: %d ", sta_idx, j);
        if(rx_reord->active == true) {
            SSV_LOG_DBG("start: %d %d pos: %d pkt_cnt: %d pn: %lld\n", rx_reord->first_win_start, rx_reord->win_start, rx_reord->rx_status_pos, rx_reord->ooo_pkt_cnt, rx_reord->pn);
            for (i = 0;  i < RX_CNTRL_REORD_WIN_SIZE; i++) {
                elt = &rx_reord->elt[i];
                if(elt->skb == NULL)
                    SSV_LOG_DBG("0 ");
                else
                    SSV_LOG_DBG("%p ", elt->skb);
            }
        }
        SSV_LOG_DBG("\n");
    }
}

/**
 ****************************************************************************************
 * @brief Check if the received frame is not a replayed one
 *
 * @param[in] pn   Packet number of the frame
 * @param[in] key  Key structure attached to the frame
 * @param[in] tid  TID of the frame
 *
 * @return true if the frame shall be uploaded, false if it shall be discarded
 ****************************************************************************************
 */
static bool ssv_reord_check_pn(uint64_t pn, uint64_t old_pn)
{
    if (pn > old_pn)
    {
        return true;
    }

    return false;
}

/**
 ****************************************************************************************
 * @brief Add an Out Of Order buffer in the reordering structure
 *
 * @param[in,out] rx_reord Pointer to the reordering structure
 * @param[out] elt         Pointer to the element to be updated
 * @param[in] host_id      Host identifier of the Out Of Order buffer
 ****************************************************************************************
 */
__INLINE void ssv_reord_ooo_add(struct rxu_cntrl_reord *rx_reord,
                                     struct rxu_cntrl_reord_elt *elt,
                                     struct sk_buff *skb)
{
    ASSERT_ERR(skb != 0);
    elt->skb= skb;
    rx_reord->ooo_pkt_cnt++;
    //SSV_LOG_DBG("add skb: [%d] win_start: %d skb: %p pkt_cnt: %d\n", rx_reord->rx_status_pos, rx_reord->win_start, skb,rx_reord->ooo_pkt_cnt);
}

/**
 ****************************************************************************************
 * @brief Remove an Out Of Order buffer from the reordering structure
 *
 * @param[in,out] rx_reord Pointer to the reordering structure
 * @param[out] elt         Pointer to the element to be updated
 ****************************************************************************************
 */
__INLINE void ssv_reord_ooo_remove(struct rxu_cntrl_reord *rx_reord,
                                         struct rxu_cntrl_reord_elt *elt)
{
    //SSV_LOG_DBG("remove skb: [%d] win_start: %d %p\n", rx_reord->rx_status_pos, rx_reord->win_start, elt->skb);
    elt->skb= 0;
    rx_reord->ooo_pkt_cnt--;
}

/**
 ****************************************************************************************
 * @brief Move the RX reordering window by one
 *
 * @param[in,out] rx_reord    Pointer to the reordering structure
 ****************************************************************************************
 */
__INLINE void ssv_reord_update(struct rxu_cntrl_reord *rx_reord)
{
    // Move the windows
    rx_reord->win_start     = (rx_reord->win_start + 1) & MAC_SEQCTRL_NUM_MAX;
    rx_reord->rx_status_pos = (rx_reord->rx_status_pos + 1) % RX_CNTRL_REORD_WIN_SIZE;
    //SSV_LOG_DBG("update skb: [%d] start: [%d]\n", rx_reord->rx_status_pos, rx_reord->win_start);
}

/**
 ****************************************************************************************
 * @brief Indicates to the host all the packets that have been unblocked by the reception
 * of the next waited sequence number
 *
 * @param[in,out] rx_reord    Pointer to the reordering structure
 ****************************************************************************************
 */
static void ssv_reord_fwd(struct rxu_cntrl_reord *rx_reord)
{
    struct rxu_cntrl_reord_elt *elt = NULL;
    bool upload = true;

    while (rx_reord->elt[rx_reord->rx_status_pos].skb)
    {
        //SSV_LOG_DBG("fwd skb: [%d] %p\n", rx_reord->rx_status_pos, rx_reord->elt[rx_reord->rx_status_pos].skb);
        elt = &rx_reord->elt[rx_reord->rx_status_pos];

        ASSERT_ERR(rx_reord->ooo_pkt_cnt);

        // Perform PN replay check if required
        //[Tim] check, pn not have key how to check
        if (elt->pn_check)
        {
            //SSV_LOG_DBG("pn check: %lld %lld\n", elt->pn, rx_reord->pn);
            if(ssv_reord_check_pn(elt->pn, rx_reord->pn) == false) {
                dev_kfree_skb_any(elt->skb);
                upload = false; 
            }
        }

        if(upload ==true) {
            // Data has already been copied in host memory and can now be forwarded
            ssv_netif_receive_skb(rx_reord->elt[rx_reord->rx_status_pos].skb);
        }

        // Remove the unordered element from the structure
        ssv_reord_ooo_remove(rx_reord, &rx_reord->elt[rx_reord->rx_status_pos]);

        // Update the reordering window
        ssv_reord_update(rx_reord);
    }
}

/**
 ****************************************************************************************
 * @brief Flush a certain amount of positions of the RX reordering window
 * This function indicates to the host the packets that have been flushed.
 *
 * @param[in,out] rx_reord    Pointer to the reordering structure
 * @param[in]     sn_skipped  Number of RX reordering window positions to be flushed
 ****************************************************************************************
 */
static void ssv_reord_flush(struct rxu_cntrl_reord *rx_reord, uint16_t sn_skipped)
{
    uint16_t i = 0;
    //uint16_t status = RX_STAT_FORWARD;
    struct rxu_cntrl_reord_elt *elt = NULL;
    bool upload = true;

    //SSV_LOG_DBG("reord_flush begin\n");
    // Forward all packets that have already been received
    for (i = 0; (i < sn_skipped) && rx_reord->ooo_pkt_cnt; i++)
    {
        uint8_t index = (rx_reord->rx_status_pos + i) % RX_CNTRL_REORD_WIN_SIZE;
        elt = &rx_reord->elt[index];

        if (elt->skb)
        {
            //SSV_LOG_DBG("flush skb: [%d] %p\n", index, elt->skb);
            //[Tim] check, pn not have key how to check
            if (elt->pn_check)
            {
                //SSV_LOG_DBG("pn check: %lld %lld\n", elt->pn, rx_reord->pn);
                if(ssv_reord_check_pn(elt->pn, rx_reord->pn) == false) {
                    dev_kfree_skb_any(elt->skb);
                    upload =false;
                }
            }

            if(upload == true) {
                // Data has already been copied in host memory and can now be forwarded
                ssv_netif_receive_skb(elt->skb);
            }
            // Remove the unordered element from the structure
            ssv_reord_ooo_remove(rx_reord, elt);
        }
    }
    //SSV_LOG_DBG("reord_flush end\n");
    rx_reord->win_start     = (rx_reord->win_start + sn_skipped) & MAC_SEQCTRL_NUM_MAX;
    rx_reord->rx_status_pos = (rx_reord->rx_status_pos + sn_skipped) % RX_CNTRL_REORD_WIN_SIZE;
    //SSV_LOG_DBG("reord_flush (%d %d %d)\n", rx_reord->win_start, rx_reord->rx_status_pos, sn_skipped);
    // Forward the first packets of the new window that have already been received
    ssv_reord_fwd(rx_reord);
}

/**
 ****************************************************************************************
 * @brief Update the reordering information accordingly with the provided BlockAck Request
 * PDU
 *
 * @param[in] sta_idx  Index of the transmitter station
 * @param[in] frame    Pointer to the received frame
 ****************************************************************************************
 */
bool ssv_reord_bar_check(struct ssv_softc *sc, struct host_reorder_info *rx_reorder_info, struct sk_buff *skb)
{
    uint8_t sta_idx = rx_reorder_info->sta_idx;
    uint8_t tid = rx_reorder_info->tid;
    uint16_t ssn = rx_reorder_info->sn;

    struct rxu_cntrl_reord *reord = &sc->sta_table[sta_idx].reord_info.ba_agmts_rx[tid];

    bool upload = false;

    if ((ssn == reord->win_start) ||
        (((ssn - reord->win_start) & MAC_SEQCTRL_NUM_MAX) > (MAC_SEQCTRL_NUM_MAX >> 1))) {
        goto exit;
    }

   /*
     * Flush all needed packet so that:
     *      - WinStart = SSN
     *      - WinEnd   = WinStart + WinSize - 1
     */
    ssv_reord_flush(reord, (ssn - reord->win_start) & MAC_SEQCTRL_NUM_MAX);
exit:
    dev_kfree_skb_any(skb);
    return upload;
}

/**
 ****************************************************************************************
 * @brief Perform the reordering checks on the received frame
 * This function may decide to:
 * - Upload and forward the frame immediately to the host if it is in order
 * - Upload and not forward if the frame is not in order
 * - Discard the frame if already received or too old
 *
 * The RX window is updated according to the previous actions.
 *
 * @param[in] rxdesc   RX descriptor attached to the received frame
 * @param[in] sta_idx  Index of the transmitter station
 *
 * @return Whether the frame shall be uploaded or not
 ****************************************************************************************
 */
bool ssv_reord_data_check(struct ssv_softc *sc, struct host_reorder_info *rx_reorder_info, struct sk_buff *skb)
{
    uint8_t sta_idx = rx_reorder_info->sta_idx;
    uint8_t tid = rx_reorder_info->tid;
    uint16_t sn = rx_reorder_info->sn;

    // Returned status
    bool upload = true;
    // SN position in the sn status bit field
    uint16_t sn_pos;

    while (1)
    {
        struct rxu_cntrl_reord *reord = &sc->sta_table[sta_idx].reord_info.ba_agmts_rx[tid];

        // Check if the received packet has the lowest expected SN
        if (sn == reord->win_start) {
            //SSV_LOG_DBG("first\n");
            // Perform the PN check if required
            if (rx_reorder_info->frame_info & RXU_CNTRL_PN_CHECK_NEEDED) {
                if(ssv_reord_check_pn(rx_reorder_info->pn, reord->pn) == true) {
                    reord->pn = rx_reorder_info->pn;
                } else {
                    upload = false;
                }
            }
            //SSV_LOG_DBG("first upload: %d\n", upload);
            //Check if we need to upload the frame
            if (upload) {
                ssv_netif_receive_skb(skb);
            } else {
                dev_kfree_skb_any(skb);
            }
            // Received packet is within the reordering window, the RX agreement can be
            // considered as active
            reord->active = true;
            // Store current time
            reord->sn_rx_time = jiffies_to_msecs(jiffies);
            // Update the RX Window
            ssv_reord_update(reord);
            // And forward any ready frames following this one
            ssv_reord_fwd(reord);
            break;
        }

        // The packet is not the first expected one, check if it is in the window or not
        sn_pos = (sn - reord->win_start) & MAC_SEQCTRL_NUM_MAX;
        //SSV_LOG_DBG("sn_pos: %d ", sn_pos);
        if (sn_pos >= RX_CNTRL_REORD_WIN_SIZE) {
            //SSV_LOG_DBG("over buffer\n");
            if (sn_pos < (MAC_SEQCTRL_NUM_MAX >> 1)) {
                // Packet is outside the window, and considered as a newer one, so we have
                // to move the window and flush the old packets that were waiting for
                // reordering
                //SSV_LOG_DBG("over buffer reord_flush\n");
                ssv_reord_flush(reord, sn_pos - RX_CNTRL_REORD_WIN_SIZE + 1);
                // After flushing, the new packet might finally become the first of the
                // window, so just in case restart the process from the beginning
                continue;
            }

            // Received packet is older than expected. Two cases apply:
            //   - Either the agreement is active, and in that case the packet has to be
            //     discarded
            //   - Or the agreement is still not active, and in that case we shall
            //     just consider the packet as being sent outside the BA agreement,
            //     i.e not consider it as a old one 
           if (reord->active) { 
                //SSV_LOG_DBG("reord->active free buffer\n"); 
                // The agreement is active, discard this old packet
                dev_kfree_skb_any(skb);
                upload = false;
                break;
            }
            // The agreement is still not active, reordering should not be involved
            // Perform the duplicate check
            //[Tim] check, not info with frame cntl, so i dont know how to check 
            //upload = rxu_cntrl_duplicate_check(rx_status->frame_cntl, sta_idx, 1);
            // Perform the PN check if required
            //[Tim] check, pn not have key how to check
            if (upload && (rx_reorder_info->frame_info & RXU_CNTRL_PN_CHECK_NEEDED)) {
                if(ssv_reord_check_pn(rx_reorder_info->pn, reord->pn) == true) {
                    reord->pn = rx_reorder_info->pn;
                } else {
                    upload = false;
                }
            }
            //SSV_LOG_DBG("reord->active pn free check: %d\n", upload); 
            // Check if we need to upload the frame
            if (upload) {
                ssv_netif_receive_skb(skb);
            } else {
                dev_kfree_skb_any(skb);
            }
            break;
        }

        // Received packet is within the reordering window, the RX agreement can be
        // considered as active
        reord->active = true;

        sn_pos = (sn_pos + reord->rx_status_pos) % RX_CNTRL_REORD_WIN_SIZE;

        // Check if the packet has already been received
        if (reord->elt[sn_pos].skb)
        { 
            // Discard the MPDU
            //SSV_LOG_DBG("the same buffer\n");
            dev_kfree_skb_any(skb);
            upload = false;
            break;
        }

        // Store the PN and keys if required
        //[Tim] check, pn not have key how to check
        if (rx_reorder_info->frame_info & RXU_CNTRL_PN_CHECK_NEEDED)
        {
            reord->elt[sn_pos].pn = rx_reorder_info->pn;
            reord->elt[sn_pos].pn_check = true;
        } else {
            reord->elt[sn_pos].pn_check = false;
        }
        //SSV_LOG_DBG("add sn_pos: %d\n", sn_pos);
        // Data has been received out of order
        //rxu_msdu_upload_and_indicate(rxdesc, RX_STAT_ALLOC);

        // Store the Host ID in the reordering element
        sc->rx.rx_reord_count++; //for MIB RX
        sc->rx_bysta[sta_idx].rx_reord_count++;
        ssv_reord_ooo_add(reord, &reord->elt[sn_pos], skb);/* linux kernel alloc buffer and copy data*/
        break;
    }

    // If packet is accepted, indicate activity on this RX BlockAck agreement
    //[Tim] bam_rx_active, that bam to know time is update, i think porting to host, it not use
    if (upload) {
        //bam_rx_active(sta_idx, rx_status->tid);
    }
    return (upload);
}

void ssv_rxreord_free_all(struct ssv_softc *sc, u8 sta_idx)
{
    uint16_t i = 0, j = 0;

    struct rxu_cntrl_reord *rx_reord = NULL;
    struct rxu_cntrl_reord_elt *elt = NULL;
 
    //SSV_LOG_DBG("free all before\n");
    for(i = 0; i < TID_MAX; i++) {
        rx_reord = &sc->sta_table[sta_idx].reord_info.ba_agmts_rx[i];
        for (j = 0;  j < RX_CNTRL_REORD_WIN_SIZE; j++) {
            uint8_t index =( j % RX_CNTRL_REORD_WIN_SIZE);
            elt = &rx_reord->elt[index];
            //SSV_LOG_DBG("free skb: [%d]%p\n",index, elt->skb);            
            if (elt->skb) {
                dev_kfree_skb_any(elt->skb);
                elt->skb = NULL;
            }
        }
    }
    //SSV_LOG_DBG("free all after\n");            
}

void ssv_rxreord_flush_tid(struct ssv_softc *sc, u8 sta_idx, u8 tid)
{
    uint16_t i = 0;
    struct rxu_cntrl_reord *rx_reord = NULL;
    struct rxu_cntrl_reord_elt *elt = NULL;
    //SSV_LOG_DBG("flush tid begin: %d %d %d\n", sc->sta_table[sta_idx].reord_info.connect, sta_idx, tid);
    if(sc->sta_table[sta_idx].reord_info.connect == 1) {
        //mutex_lock(&sc->sta_table[sta_idx].reord_info.data_mutex);
        rx_reord = &sc->sta_table[sta_idx].reord_info.ba_agmts_rx[tid];
        for (i = 0;  i < RX_CNTRL_REORD_WIN_SIZE; i++) {
            elt = &rx_reord->elt[i];
            if (elt->skb) {
                //SSV_LOG_DBG("upload skb: [%d]%p\n",i, elt->skb);    
                ssv_netif_receive_skb(elt->skb);
                elt->skb = NULL;
            }
        }
        //mutex_unlock(&sc->sta_table[sta_idx].reord_info.data_mutex);
    }
    //SSV_LOG_DBG("flush tid end\n");
}

void ssv_rxreord_info_dump(struct host_reorder_info *rx_reorder_info)
{
    SSV_LOG_DBG("frame_info: %x sta_idx: %d tid: %d baw_size: %d pn: %lld win_start: %d sn: %d cookie: %d\n", 
                    rx_reorder_info->frame_info,
                    rx_reorder_info->sta_idx,
                    rx_reorder_info->tid,
                    rx_reorder_info->baw_size,
                    rx_reorder_info->pn,
                    rx_reorder_info->win_start,
                    rx_reorder_info->sn,
                    rx_reorder_info->cookie);
}

void ssv_rxreord_tid_dump(struct ssv_softc *sc, struct host_reorder_info *rx_reorder_info)
{
    uint16_t i = 0;

    struct rxu_cntrl_reord *rx_reord = NULL;
    struct rxu_cntrl_reord_elt *elt = NULL;
    uint8_t tid = (rx_reorder_info->tid > TID_7 ? TID_NOQOS: rx_reorder_info->tid);
    rx_reord = &sc->sta_table[rx_reorder_info->sta_idx].reord_info.ba_agmts_rx[tid];
    SSV_LOG_DBG("tid: %d start: %d %d pos: %d pkt_cnt: %d pn: %lld ",tid, rx_reord->first_win_start, rx_reord->win_start, rx_reord->rx_status_pos, rx_reord->ooo_pkt_cnt, rx_reord->pn);
    for (i = 0;  i < RX_CNTRL_REORD_WIN_SIZE; i++) {
        elt = &rx_reord->elt[i];
        if(elt->skb == NULL)
            SSV_LOG_DBG("0 ");
        else
            SSV_LOG_DBG("%p ", elt->skb);
    }
    SSV_LOG_DBG("\n");
}

bool ssv_rxreord_update_rxinfo_and_upload(struct ssv_softc *sc, struct host_reorder_info *rx_reorder_info)
{
    bool upload = true;
    struct rxu_cntrl_reord *rx_reord = NULL;
    uint16_t *last_seq_cntl=NULL;
    uint8_t tid = (rx_reorder_info->tid > TID_7 ? TID_NOQOS: rx_reorder_info->tid);
    rx_reord = &sc->sta_table[rx_reorder_info->sta_idx].reord_info.ba_agmts_rx[tid];
    //SSV_LOG_DBG("rxreord_update_rxinfo_and_upload flush begin\n");
    if(rx_reorder_info->frame_info & RXU_CNTRL_NO_RX_BA_TO_HOST) {
        //rece not reorder data, but qos data
        if(((tid >=TID_0) && (tid <=TID_7)) && (rx_reord->active ==true)) {
            //flush old data
            //SSV_LOG_DBG("rxreord_update_rxinfo_and_upload flush\n");

            ssv_rxreord_flush_tid(sc, rx_reorder_info->sta_idx, rx_reorder_info->tid);
            memset(rx_reord, 0, sizeof(struct rxu_cntrl_reord));
            SSV_LOG_DBG("===delete reorder tid: %d win_start: %d win_end: %d===\n", tid, rx_reord->first_win_start, rx_reord->win_end);
        }
        // Perform the PN check if required
        if (rx_reorder_info->frame_info & RXU_CNTRL_PN_CHECK_NEEDED) {
            if(ssv_reord_check_pn(rx_reorder_info->pn, rx_reord->pn) == true) {
                rx_reord->pn = rx_reorder_info->pn;
            } else {
                upload = false;
            }
        }
        if((tid >=TID_0) && (tid <=TID_7)){ //QoS Data
            last_seq_cntl = &(sc->sta_table[rx_reorder_info->sta_idx].rx_last_seqcntl[rx_reorder_info->tid]);
        }
        else{ //not QoS Data
            last_seq_cntl = &(sc->sta_table[rx_reorder_info->sta_idx].rx_last_seqcntl[TID_NOQOS]);
        }

        if((rx_reorder_info->cookie == 1)&&
            (*last_seq_cntl == rx_reorder_info->sn))
        {
            //This is a duplicate frame, we need to drop it
            upload = false;
        }
        *last_seq_cntl = rx_reorder_info->sn;
        //SSV_LOG_DBG("rxreord_update_rxinfo_and_upload upload: %d\n", upload);
    } else {
        //rece not reorder data
reorder_check:
        //rece first reorder data, need init win_start windows
        if(rx_reord->active ==false) {
            rx_reord->win_start = rx_reorder_info->win_start;
            rx_reord->first_win_start = rx_reorder_info->win_start;
            rx_reord->win_end = rx_reorder_info->baw_size;
            rx_reord->tid = tid;
            rx_reord->sn_rx_time = jiffies_to_msecs(jiffies);
            rx_reord->active = true;
            //SSV_LOG_DBG("rxreord_update_rxinfo_and_upload init: %d %d %d\n", rx_reord->win_start, rx_reord->first_win_start, rx_reord->win_end);
            SSV_LOG_DBG("===create reorder tid: %d win_start: %d win_end: %d===\n", tid, rx_reord->first_win_start, rx_reord->win_end);
        } else {
            if((rx_reorder_info->win_start != rx_reord->first_win_start) || (rx_reorder_info->baw_size != rx_reord->win_end)) {
                   //flush old data, and tid re-create
                    SSV_LOG_DBG("i think maybe not goto here\n");
                    ssv_rxreord_flush_tid(sc, rx_reorder_info->sta_idx, rx_reorder_info->tid);
                    memset(rx_reord, 0, sizeof(struct rxu_cntrl_reord));
                    SSV_LOG_DBG("===delete reorder tid: %d win_start: %d win_end: %d===\n", tid, rx_reord->first_win_start, rx_reord->win_end);
                    goto reorder_check;
            }
#ifdef REODER_DROPPKT_TESTMODE
            if(randNum == 0) {
                get_random_bytes(&randNum, 4096);
                randNum = randNum & 0Xfff;
            } else {
                randNum--;
                if(randNum == 0) {
                    upload =false;
                    SSV_LOG_DBG("#####  drop #####\n");
                }
            }
#endif
        }
        //SSV_LOG_DBG("rxreord_update_rxinfo_and_upload upload2: %d\n", upload);
    }
    return upload;
}

void ssv_rxreord_timeout_cb(struct ssv_softc *sc)
{
    //struct rxu_cntrl_reord_elt *elt = NULL;
    uint32_t now_time = jiffies_to_msecs(jiffies);
    uint16_t i = 0, j = 0;
    struct rxu_cntrl_reord *rx_reord = NULL;
    for(i = 0; i < (NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX); i++) {
        if(sc->sta_table[i].reord_info.connect == 1) {
            //SSV_LOG_DBG("=====[%d] ssv_rxreord_timeout_cb:%x %x connect: %d =====\n",i, (unsigned int)sc, (unsigned int)&sc->reord_info[0], sc->reord_info[0].connect);
            for(j = 0; j < TID_MAX; j++) {
                rx_reord = &sc->sta_table[i].reord_info.ba_agmts_rx[j];
                if(rx_reord->active == false)
                    continue;
                //mutex_lock(&sc->sta_table[i].reord_info.data_mutex);
                if(rx_reord->ooo_pkt_cnt > 0) {
#if 0
                    if(time_after((unsigned long)now_time, (unsigned long)rx_reord->sn_rx_time))
                        SSV_LOG_DBG("jiffies is overflow, we do nothing\n");
#endif
                    if((now_time - rx_reord->sn_rx_time) > HOUSE_KEEPING_TIMEOUT) {                   
                        // Consider the awaited packets as lost
                        while (!rx_reord->elt[rx_reord->rx_status_pos].skb) {
                            ssv_reord_update(rx_reord);
                        }

                        // Forward all the frames that were waiting after the missing ones
                        ssv_reord_fwd(rx_reord);  
                    }
                }
                //mutex_unlock(&sc->sta_table[i].reord_info.data_mutex);
            }
        }
    }
}

void ssv_rxreord_create(struct ssv_softc *sc, u8 sta_idx)
{
    SSV_LOG_DBG("===== rxreord create: %d lcok en: %d =====\n", sta_idx, sc->sta_table[sta_idx].reord_info.lock_en);
    if(sc->sta_table[sta_idx].reord_info.lock_en == 0) {
        mutex_init(&sc->sta_table[sta_idx].reord_info.data_mutex);
        //SSV_LOG_DBG("#####sta_idx: %d mutex init check: %p#####\n", sta_idx, &sc->sta_table[sta_idx].reord_info.data_mutex);
        sc->sta_table[sta_idx].reord_info.lock_en = 1;
    }
    memset(&sc->sta_table[sta_idx].reord_info.ba_agmts_rx, 0, sizeof(struct rxu_cntrl_reord)*TID_MAX);
    sc->sta_table[sta_idx].reord_info.connect = 1;
}

void ssv_rxreord_delete(struct ssv_softc *sc, u8 sta_idx)
{
    SSV_LOG_DBG("===== rxreord delete: %d =====\n", sta_idx);
    //mutex_lock(&sc->sta_table[sta_idx].reord_info.data_mutex);
    sc->sta_table[sta_idx].reord_info.connect = 0;
    ssv_rxreord_free_all(sc, sta_idx);
    memset(&sc->sta_table[sta_idx].reord_info.ba_agmts_rx, 0, sizeof(struct rxu_cntrl_reord)*TID_MAX);
    //mutex_unlock(&sc->sta_table[sta_idx].reord_info.data_mutex);
}
#if defined(REODER_DUPPPKT_TESTMODE)
void ssv_rxreord_duptest_into(u16  len, struct host_reorder_info *rx_reorder_info)
{
    if(randNum == 0) {
        get_random_bytes(&randNum, 4096);
        randNum = randNum & 0Xfff;
    } else {
        randNum--;
        if(randNum == 0) {
            SSV_LOG_DBG("#####  dup #####\n");
            dup_skb = __dev_alloc_skb(len, GFP_KERNEL);
            if (dup_skb) {
                memcpy(&dup_rx_reorder_info,rx_reorder_info,sizeof( struct host_reorder_info));
            } else {
                SSV_LOG_DBG("%s(): cannot alloc skb buffer\n", __FUNCTION__);
                randNum = 1;
            }
        }
    }
}

void ssv_rxreord_duptest_exit(struct ssv_softc *sc)
{
    if(randNum == 0) {
        ssv_reord_data_check(sc, &dup_rx_reorder_info, dup_skb);
    }
}
#endif

static void _ssv_rx_mgmt(struct ssv_softc *sc, struct ssv_vif *ssv_vif,
                         struct sk_buff *skb,  struct rx_info *rx_info)
{
    struct ieee80211_mgmt *mgmt = (struct ieee80211_mgmt *)skb->data;

    if ((NL80211_IFTYPE_AP == SSV_VIF_TYPE(ssv_vif))||(NL80211_IFTYPE_P2P_GO == SSV_VIF_TYPE(ssv_vif)))
    {
        if (ieee80211_is_auth(mgmt->frame_control)){ 
            struct list_head *entry=NULL;
            u8 sta_num=0;
            list_for_each(entry,&ssv_vif->ap.sta_list)
                sta_num++;
        
            if(sta_num==(NX_REMOTE_STA_MAX-1))
            {
                ssv_xmit_deauth_frame(sc, ssv_vif, NULL, mgmt->sa, mgmt->da, mgmt->da, 6);
                return;
            }
        }

    }


    // SSV_LOG_DBG("mgmt->frame_control = 0x%04x\n", mgmt->frame_control);
    if (ieee80211_is_beacon(mgmt->frame_control)) {
            cfg80211_report_obss_beacon(sc->wiphy, skb->data, skb->len,
                                        PHY_INFO_CHAN(rx_info->phy_info),
                                        rx_info->vect.rx_vec_1.rssi1
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 8, 0)
                                        , GFP_ATOMIC
#endif
                                        );
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 11, 0) || defined(CONFIG_SSV_CHANNEL_FOLLOW)
    } else if ((ieee80211_is_deauth(mgmt->frame_control) ||
                ieee80211_is_disassoc(mgmt->frame_control)) &&
               (mgmt->u.deauth.reason_code == WLAN_REASON_CLASS2_FRAME_FROM_NONAUTH_STA ||
                mgmt->u.deauth.reason_code == WLAN_REASON_CLASS3_FRAME_FROM_NONASSOC_STA)) {
        cfg80211_rx_unprot_mlme_mgmt(ssv_vif->ndev, skb->data, skb->len);
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 0)
    } else if ((SSV_VIF_TYPE(ssv_vif) == NL80211_IFTYPE_STATION) &&
               (ieee80211_is_action(mgmt->frame_control) &&
                (mgmt->u.action.category == 6))) {
        struct cfg80211_ft_event_params ft_event;
        ft_event.target_ap = (uint8_t *)&mgmt->u.action + ETH_ALEN + 2;
        ft_event.ies = (uint8_t *)&mgmt->u.action + ETH_ALEN*2 + 2;
        ft_event.ies_len = rx_info->vect.frmlen - (ft_event.ies - (uint8_t *)mgmt);
        ft_event.ric_ies = NULL;
        ft_event.ric_ies_len = 0;
        cfg80211_ft_event(ssv_vif->ndev, &ft_event);
#endif
    } else {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 18, 0)
        // SSV_LOG_DBG("to call cfg80211_rx_mgmt, rx_info->vect.rx_vec_1.rssi1 = %d\n", rx_info->vect.rx_vec_1.rssi1);
        cfg80211_rx_mgmt(&ssv_vif->wdev, PHY_INFO_CHAN(rx_info->phy_info),
                         rx_info->vect.rx_vec_1.rssi1, skb->data, skb->len, 0);
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(3, 12, 0)
        cfg80211_rx_mgmt(&ssv_vif->wdev, PHY_INFO_CHAN(rx_info->phy_info),
                         rx_info->vect.rx_vec_1.rssi1, skb->data, skb->len, 0, GFP_ATOMIC);
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
        cfg80211_rx_mgmt(&ssv_vif->wdev, PHY_INFO_CHAN(rx_info->phy_info),
                         rx_info->vect.rx_vec_1.rssi1, skb->data, skb->len, GFP_ATOMIC);
#else
        cfg80211_rx_mgmt(ssv_vif->wdev.netdev, PHY_INFO_CHAN(rx_info->phy_info),
                         rx_info->vect.rx_vec_1.rssi1, skb->data, skb->len, GFP_ATOMIC);
#endif
    }
}

/**
 * ssv_rx_mgmt - Process one 802.11 management frame
 *
 * @sc: main driver data
 * @ssv_vif: vif that received the buffer
 * @skb: skb received
 * @rxhdr: HW rx descriptor
 *
 * Process the management frame and free the corresponding skb
 */
static void ssv_rx_mgmt(struct ssv_softc *sc, struct ssv_vif *ssv_vif,
                         struct sk_buff *skb,  struct rx_info *rx_info)
{
    struct ieee80211_mgmt *mgmt = (struct ieee80211_mgmt *)skb->data;

    if (is_broadcast_ether_addr(mgmt->da)) {
        struct ssv_vif *tmp_vif = NULL;
        list_for_each_entry(tmp_vif, &sc->vifs, list) {
            if (tmp_vif->up) {
                _ssv_rx_mgmt(sc, tmp_vif, skb, rx_info);
            }
        }
    } else {
        _ssv_rx_mgmt(sc, ssv_vif, skb, rx_info);
    }
    dev_kfree_skb_any(skb);
}

int ssv_update_sta_last_rx(struct ssv_softc *sc, const u8 *mac_addr)
{
    int i;

    for (i = 0; i < NX_REMOTE_STA_MAX; i++) {
        struct ssv_sta *sta = &sc->sta_table[i];
        if (sta->valid && (memcmp(mac_addr, &sta->mac_addr, 6) == 0))
        {
            sta->last_rx = jiffies_to_msecs(jiffies);
            break;
        }
    }
    return 0;
}

void ssv_rx_sw_ack_handler(struct ssv_softc *sc, struct txdesc_api *msg_desc_api)
{
    struct sk_buff *clone_skb;
    struct ssv_vif *ssv_vif = ssv_rx_get_vif(sc, msg_desc_api->host.vif_idx);
    u8* mac_addr;
    int qidx = 0, qlen = 0;

    if (NULL == ssv_vif)
    {
        SSV_LOG_DBG("ssv_vif is NULL!!!\n");
        return;
    }
    
    mac_addr = (u8*)msg_desc_api->host.eth_dest_addr.array;
    
    qlen = skb_queue_len(&sc->wait_sw_ack_q);
    for (qidx = 0; qidx < qlen; qidx++)
    {    
        clone_skb = skb_dequeue(&sc->wait_sw_ack_q);
        {
            bool ack = 0;
            struct txdesc_api *clone_desc_api = 
                            (struct txdesc_api *)(clone_skb->data + sizeof(struct sdio_hdr)+sizeof(struct tx_bmu_desc));

            if (msg_desc_api->host.sw_ack & 0x8)
                ack = 1;
                           
            if (ack) {
                ssv_update_sta_last_rx(sc, mac_addr);
            }
          
            if (clone_desc_api->host.sw_seq == msg_desc_api->host.sw_seq)
            {
                if ((clone_skb->cb[0] == 'p') && (clone_skb->cb[1] == 'r') && (clone_skb->cb[2] == 'o') &&
                    (clone_skb->cb[3] == 'b') && (clone_skb->cb[4] == 'e') && (clone_skb->cb[5] == 't') && 
                    (clone_skb->cb[6] == 'x')) {

                    dev_kfree_skb_any(clone_skb);
		} else if ((clone_skb->cb[0] == 'p') && (clone_skb->cb[1] == 'i') && (clone_skb->cb[2] == 'n') &&
                    (clone_skb->cb[3] == 'g') && (clone_skb->cb[4] == 'm') && (clone_skb->cb[5] == 'a') && 
                    (clone_skb->cb[6] == 'c')) {

                    unsigned long end = jiffies;
                    SSV_LOG_DBG(KERN_INFO "Get ping response from MAC layer: seq=%u time=%u ms\n", msg_desc_api->host.sw_seq, jiffies_to_msecs(end - sc->ssv_ping.start));
                    sc->ssv_ping.result = true;
                    dev_kfree_skb_any(clone_skb);
                } else {
                    cfg80211_mgmt_tx_status(
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
                                            &ssv_vif->wdev,
#else
                                            ssv_vif->wdev.netdev,
#endif
                                            (unsigned long)clone_skb,
                                            (clone_skb->data + sizeof(struct txdesc_api) + 
                                                sizeof(struct sdio_hdr)+sizeof(struct tx_bmu_desc)),
                                            clone_desc_api->host.packet_len,
                                            ack,  //ack fail set 0
                                            GFP_ATOMIC);

                    dev_kfree_skb_any(clone_skb);
                }
                break;
            }
            else
            {
                //re enqueue to wait_sw_ack_q
                skb_queue_tail(&sc->wait_sw_ack_q, clone_skb);
            }
        }
    }    
}

void ssv6xxx_restart_check_rx(struct ssv_softc *sc, struct sk_buff *skb, u8 pkt_type)
{
    char *raw_data = (char *)(skb->data);
    ST_IPC_PRIV_MSG *msg_priv = (ST_IPC_PRIV_MSG *)raw_data;

    if(pkt_type == E_IPC_TYPE_MSG) {
        sc->ipc_env->cb.recv_msg_ind(sc, (void *)(skb->data));
    } else if(pkt_type == E_IPC_TYPE_PRIV_MSG){
        if((msg_priv->msgid != E_HOST_PRIV_MSG_TYPE_RX_REORD_TIMEOUT) &&
            (msg_priv->msgid != E_HOST_PRIV_MSG_TYPE_CHECK_FW_STATUS)) {
                SSV_LOG_DBG("priv msg unknown id: %d\n", msg_priv->msgid);
            }
    } else {
        SSV_LOG_DBG("unknown event: %d\n", pkt_type);
    }
}

void ssv6xxx_check_fw_status_process(struct ssv_softc *sc)
{
    u32 cur_status_cnt = 0;

    ssv_drv_hci_read_word(sc->hci_priv, sc->hci_ops, 0x08d010f8, &cur_status_cnt);
    sc->fw_cur_status_cnt = cur_status_cnt;

    if(sc->fw_cur_status_cnt != sc->fw_pre_status_cnt)
    {
        sc->fw_pre_status_cnt = sc->fw_cur_status_cnt;
        sc->fw_cur_status_idle_time = 0;
    }
    else
    {
        if(ssv_cfg.fw_status_idle_cnt <= ++sc->fw_cur_status_idle_time)
        {
            SSV_LOG_DBG("[%s] Not detect FW status for %d times(over %d ms), the FW will be reloaded.\n", __FUNCTION__, sc->fw_cur_status_idle_time, sc->fw_cur_status_idle_time*100); //100 = TIMEOUT_CHECK_FW_STATUS
            queue_work(sc->fw_reset_wq, &sc->fw_reset_work);
            sc->fw_cur_status_idle_time = 0;
        }
    }
}

static int ssv_rx_priv_msg_handler(struct ssv_softc *sc, struct sk_buff *skb)
{
    ST_IPC_PRIV_MSG *msg = (ST_IPC_PRIV_MSG *)skb->data;
    int ret=0;
    // SSV_LOG_DBG("[%s][%d] msg->msgid = %u, msg->msglen = %u\n", __FUNCTION__, __LINE__, msg->msgid, msg->msglen);
    switch(msg->msgid)
    {
        case E_IPC_PRIV_MSG_TYPE_TXTPUT_IND:
        {
            SSV_LOG_DBG(KERN_INFO "txtput result: %u kbps\n", *((u32 *)msg->data));
            dev_kfree_skb_any(skb);
            break;
        }
        case E_IPC_PRIV_MSG_TYPE_RXTPUT_IND:
        {
            // SSV_LOG_DBG(KERN_INFO "rxtput seq_no: %u\n", *((u32 *)msg->data));
            ssv_rxtput_calculation(sc, msg->msglen, *((u32 *)msg->data));
            dev_kfree_skb_any(skb);
            break;
        }
        case E_IPC_PRIV_MSG_TYPE_SW_ACK_IND:
        {
            //struct txdesc_api *msg_desc_api = (struct txdesc_api *)msg->data;
            //SSV_LOG_DBG("msg sw seq === %d, vif id = %d\n", msg_desc_api->host.sw_seq, msg_desc_api->host.vif_idx);
            ssv_rx_sw_ack_handler(sc, (struct txdesc_api*)msg->data);
			dev_kfree_skb_any(skb);
            break;
        }
        case E_HOST_PRIV_MSG_TYPE_RX_REORD_CREATE:
        {
            ssv_rxreord_create(sc, *((u8 *)(msg->data)));
            dev_kfree_skb_any(skb);
            break;
        }
        case E_HOST_PRIV_MSG_TYPE_RX_REORD_DEL:
        {
            ssv_rxreord_delete(sc, *((u8 *)(msg->data)));
            dev_kfree_skb_any(skb);
            break;
        }
        case E_HOST_PRIV_MSG_TYPE_RX_REORD_TIMEOUT:
        {
            ssv_rxreord_timeout_cb(sc);
            dev_kfree_skb_any(skb);
            break;
        }
        case E_HOST_PRIV_MSG_TYPE_PROBE_CLIENT:
        {
            ssv_probe_client(sc);
            dev_kfree_skb_any(skb);
            break;
        }
#ifdef SEND_KEEP_ALIVE
        case E_HOST_PRIV_MSG_TYPE_SEND_KEEP_ALIVE:
        {
            ssv_send_keep_alive(sc);
            dev_kfree_skb_any(skb);
            break;
        }
#endif
#if 0  //move to rftool rx callback 
        case E_IPC_PRIV_MSG_TYPE_RSP_EFUSE_IND:
        {
            struct ssv_efuse_tool_param *param = (struct ssv_efuse_tool_param *)msg->data;
            ssv_res_update_to_cfg(param);
            complete(&sc->ssv_cmd_done);
            dev_kfree_skb_any(skb);
            break;
        }
#endif
        case E_HOST_PRIV_MSG_TYPE_CHECK_FW_STATUS:
        {
            ssv6xxx_check_fw_status_process(sc);
            dev_kfree_skb_any(skb);
            break;
        }
        case E_IPC_PRIV_MSG_TYPE_RSP_REG_RW:
        {
            ssv_resp_reg_rw_handler(sc, msg->data);
            dev_kfree_skb_any(skb);
            break;            
        }
        case E_IPC_PRIV_MSG_TYPE_PING_IPC_IND:
        {
            unsigned long end = jiffies;
            SSV_LOG_DBG(KERN_INFO "Get ping response from IPC layer: seq=%u time=%u ms\n", *(unsigned int *)msg->data, jiffies_to_msecs(end - sc->ssv_ping.start));
            if (*(unsigned int *)msg->data == sc->ssv_ping.seq) {
                int i;
                bool check = true;
                for (i = sizeof(unsigned int) ; i < msg->msglen ; i++) {
                    if (msg->data[i] != i%0xFF) {
                        check =false;
                    }
                }
                sc->ssv_ping.result = check;
            }
            dev_kfree_skb_any(skb);
            break;
        }
        default:
        {
            //SSV_LOG_DBG("[%s][%d] Unknown private msg type(%u)!!\n", __FUNCTION__, __LINE__, msg->msgid);
            //dev_kfree_skb_any(skb);
            ret = -1;
            break;
        }
    }

    return ret;
}

#if 0 //TBD: for struct ieee80211_radiotap_header
#define NX_MAC_VER 21 ///@FIXME: hard-code to sync firmware version
/// Table of conversion between a RX vector rate to a MAC HW rate
const int8_t rxv2macrate[] = {
    0,                          /* 0 */
    1,                          /* 1 */
    2,                          /* 2 */
    3,                          /* 3 */
    -1,                         /* 4 */
    -1,                         /* 5 */
    -1,                         /* 6 */
    -1,                         /* 7 */
    10,                         /* 8 */
    8,                          /* 9 */
    6,                          /* 10 */
    4,                          /* 11 */
    11,                         /* 12 */
    9,                          /* 13 */
    7,                          /* 14 */
    5                           /* 15 */
};

static int _ssv_rx_get_rate_idx(struct ssv_softc *sc, struct rx_info *info)
{
    /// Number of BW
    u8 n_bw = sc->phy_ch_bw + 1;
    /// Total number of rates
    uint16_t n_rates = N_CCK + N_OFDM;
    /// First HT rate index
    u16 first_ht = n_rates;
    /// Rates per HT MCS
    u8 ht_rates_per_mcs = n_bw * 2;

    /// First VHT rate index
    u16 first_vht = 0;
    /// Max VHT MCS
    u8 max_vht_mcs;
    /// Rates per VHT MCS
    u8 vht_rates_per_mcs;

    int rate_idx = 0, mcs = 0, sgi = 0, nss = 0;
    struct rx_vector_1 *rx_vect = &info->vect.rx_vec_1;

    {
        n_rates += 8 * sc->mod_params->nss * ht_rates_per_mcs;

        max_vht_mcs = 9 + 1; //IEEE80211_VHT_MCS_SUPPORT_0_9
        first_vht = n_rates;
        vht_rates_per_mcs = n_bw * 2;

        n_rates += max_vht_mcs * sc->mod_params->nss * vht_rates_per_mcs;
    }

    switch (rx_vect->format_mod) {
        case FORMATMOD_NON_HT:
        case FORMATMOD_NON_HT_DUP_OFDM:
        {
            int idx = rxv2macrate[rx_vect->leg_rate];
            if (idx < 4) {
                rate_idx = idx * 2 + rx_vect->pre_type;
            } else {
                rate_idx = N_CCK + idx - 4;
            }
            break;
        }
        case FORMATMOD_HT_MF:
        case FORMATMOD_HT_GF:
            #if NX_MAC_VER >= 20
            mcs = rx_vect->ht.mcs % 8;
            nss = rx_vect->ht.mcs / 8;
            sgi = rx_vect->ht.short_gi;
            #else
            mcs = rx_vect->mcs % 8;
            nss = rx_vect->mcs / 8;
            sgi = rx_vect->short_gi;
            #endif
            rate_idx = first_ht + nss * (8 * ht_rates_per_mcs) +
                       mcs * ht_rates_per_mcs + rx_vect->ch_bw * 2 + sgi;
            break;
        case FORMATMOD_VHT:
            #if NX_MAC_VER < 20
            default:
            mcs = rx_vect->mcs;
            nss = rx_vect->stbc ? rx_vect->n_sts/2 : rx_vect->n_sts;
            sgi = rx_vect->short_gi;
            #else
            mcs = rx_vect->vht.mcs;
            nss = rx_vect->vht.nss;
            sgi = rx_vect->vht.short_gi;
            #endif
            rate_idx = first_vht + nss * (max_vht_mcs * vht_rates_per_mcs) +
                       mcs * vht_rates_per_mcs + rx_vect->ch_bw * 2 + sgi;
            break;
#if 0
        #if NX_MAC_VER >= 20
        case FORMATMOD_HE_SU:
            mcs = rx_vect->he.mcs;
            nss = rx_vect->he.nss;
            sgi = rx_vect->he.gi_type;
            rate_idx = fhost_conf_rx.first_he_su + nss * (fhost_conf_rx.max_he_mcs * fhost_conf_rx.he_su_rates_per_mcs) +
                       mcs * fhost_conf_rx.he_su_rates_per_mcs + rx_vect->ch_bw * 3 + sgi;
            break;
        case FORMATMOD_HE_MU:
            mcs = rx_vect->he.mcs;
            nss = rx_vect->he.nss;
            sgi = rx_vect->he.gi_type;
            rate_idx = fhost_conf_rx.first_he_mu + nss * fhost_conf_rx.max_he_mcs * fhost_conf_rx.he_mu_rates_per_mcs +
                       mcs * fhost_conf_rx.he_mu_rates_per_mcs + rx_vect->he.ru_size * 3 + sgi;
            break;
        default:
            mcs = rx_vect->he.mcs;
            nss = rx_vect->he.nss;
            sgi = rx_vect->he.gi_type;
            rate_idx = fhost_conf_rx.first_he_er + rx_vect->ch_bw * 9 +
                       mcs * fhost_conf_rx.he_er_rates_per_mcs + sgi;
        #endif
#endif
    }

    // SSV_LOG_DBG("[%s][%d] rx_vect->format_mod = %u\n", __FUNCTION__, __LINE__, rx_vect->format_mod);
#if 0 //debug
    if(0 != mcs)
    {
        SSV_LOG_DBG("[%s][%d] rate_idx = %d, mcs = %d, leg_rate = %u\n", __FUNCTION__, __LINE__, rate_idx, mcs, rx_vect->leg_rate);
    }
#endif

    return rate_idx;
}
#endif //#if 0 //TBD: for struct ieee80211_radiotap_header


struct phy_channel_info_desc {
	/** PHY channel information 1 */
	u32    phy_band           : 8;
	u32    phy_channel_type   : 8;
	u32    phy_prim20_freq    : 16;
	/** PHY channel information 2 */
	u32    phy_center1_freq   : 16;
	u32    phy_center2_freq   : 16;
};

static u8 ssv_rx_rtap_hdrlen(struct rx_vector_1 *rxvect,
							  bool has_vend_rtap)
{
	u8 rtap_len;

	/* Compute radiotap header length */
	rtap_len = sizeof(struct ieee80211_radiotap_header) + 8;

	// Check for multiple antennas
	if (hweight32(rxvect->antenna_set) > 1)
		// antenna and antenna signal fields
		rtap_len += 4 * hweight8(rxvect->antenna_set);

	// TSFT
	if (!has_vend_rtap) {
		rtap_len = ALIGN(rtap_len, 8);
		rtap_len += 8;
	}

	// IEEE80211_HW_SIGNAL_DBM
	rtap_len++;

	// Check if single antenna
	if (hweight32(rxvect->antenna_set) == 1)
		rtap_len++; //Single antenna

	// padding for RX FLAGS
	rtap_len = ALIGN(rtap_len, 2);

	// Check for HT frames
	if ((rxvect->format_mod == FORMATMOD_HT_MF) ||
		(rxvect->format_mod == FORMATMOD_HT_GF))
		rtap_len += 3;

	// Check for AMPDU
	if (!(has_vend_rtap) && ((rxvect->format_mod >= FORMATMOD_VHT) ||
							 ((rxvect->format_mod > FORMATMOD_NON_HT_DUP_OFDM) &&
													 (rxvect->ht.aggregation)))) {
		rtap_len = ALIGN(rtap_len, 4);
		rtap_len += 8;
	}

	// Check for VHT frames
	if (rxvect->format_mod == FORMATMOD_VHT) {
		rtap_len = ALIGN(rtap_len, 2);
		rtap_len += 12;
	}

	// Check for HE frames
	if (rxvect->format_mod == FORMATMOD_HE_SU) {
		rtap_len = ALIGN(rtap_len, 2);
		rtap_len += sizeof(struct ieee80211_radiotap_he);
	}

	// Check for multiple antennas
	if (hweight32(rxvect->antenna_set) > 1) {
		// antenna and antenna signal fields
		rtap_len += 2 * hweight8(rxvect->antenna_set);
	}

	// Check for vendor specific data
	if (has_vend_rtap) {
		/* vendor presence bitmap */
		rtap_len += 4;
		/* alignment for fixed 6-byte vendor data header */
		rtap_len = ALIGN(rtap_len, 2);
	}

	return rtap_len;
}


static void ssv_rx_add_rtap_hdr(struct ssv_vif *ssv_vif,
								 struct sk_buff *skb,
								 struct rx_vector_1 *rxvect,
								 struct phy_channel_info_desc *phy_info,
								 struct rx_vector *vect,
								 int rtap_len,
								 u8 vend_rtap_len,
								 u32 vend_it_present)
{
	struct ieee80211_radiotap_header *rtap;
	u8 *pos, rate_idx;
	__le32 *it_present;
	u32 it_present_val = 0;
	bool fec_coding = false;
	bool short_gi = false;
	bool stbc = false;
	bool aggregation = false;
    struct ssv_softc *sc = ssv_vif->sc;

	rtap = (struct ieee80211_radiotap_header *)skb_push(skb, rtap_len);
	memset((u8 *) rtap, 0, rtap_len);

	rtap->it_version = 0;
	rtap->it_pad = 0;
	rtap->it_len = cpu_to_le16(rtap_len + vend_rtap_len);

	it_present = &rtap->it_present;

	// Check for multiple antennas
	if (hweight32(rxvect->antenna_set) > 1) {
		int chain;
		unsigned long chains = rxvect->antenna_set;

		for_each_set_bit(chain, &chains, IEEE80211_MAX_CHAINS) {
			it_present_val |=
				BIT(IEEE80211_RADIOTAP_EXT) |
				BIT(IEEE80211_RADIOTAP_RADIOTAP_NAMESPACE);
			put_unaligned_le32(it_present_val, it_present);
			it_present++;
			it_present_val = BIT(IEEE80211_RADIOTAP_ANTENNA) |
							 BIT(IEEE80211_RADIOTAP_DBM_ANTSIGNAL);
		}
	}

	// Check if vendor specific data is present
	if (vend_rtap_len) {
		it_present_val |= BIT(IEEE80211_RADIOTAP_VENDOR_NAMESPACE) |
						  BIT(IEEE80211_RADIOTAP_EXT);
		put_unaligned_le32(it_present_val, it_present);
		it_present++;
		it_present_val = vend_it_present;
	}

	put_unaligned_le32(it_present_val, it_present);
	pos = (void *)(it_present + 1);

	// IEEE80211_RADIOTAP_TSFT
	if (vect) {
		rtap->it_present |= cpu_to_le32(1 << IEEE80211_RADIOTAP_TSFT);
		// padding
		while ((pos - (u8 *)rtap) & 7)
			*pos++ = 0;
		put_unaligned_le64((((u64)le32_to_cpu(vect->tsfhi) << 32) +
							(u64)le32_to_cpu(vect->tsflo)), pos);
        //maybe zero      
		pos += 8;
	}

	// IEEE80211_RADIOTAP_FLAGS
	rtap->it_present |= cpu_to_le32(1 << IEEE80211_RADIOTAP_FLAGS);
	//if (hwvect && (!hwvect->frm_successful_rx))
	//	*pos |= IEEE80211_RADIOTAP_F_BADFCS;
	if (!rxvect->pre_type
			&& (rxvect->format_mod <= FORMATMOD_NON_HT_DUP_OFDM))
		*pos |= IEEE80211_RADIOTAP_F_SHORTPRE;

	pos++;

	// IEEE80211_RADIOTAP_RATE
	// check for HT, VHT or HE frames
	if (rxvect->format_mod >= FORMATMOD_HE_SU) {
		rate_idx = rxvect->he.mcs;
		fec_coding = rxvect->he.fec;
		stbc = rxvect->he.stbc;
		aggregation = true;
		*pos = 0;
	} else if (rxvect->format_mod == FORMATMOD_VHT) {
		rate_idx = rxvect->vht.mcs;
		fec_coding = rxvect->vht.fec;
		short_gi = rxvect->vht.short_gi;
		stbc = rxvect->vht.stbc;
		aggregation = true;
		*pos = 0;
	} else if (rxvect->format_mod > FORMATMOD_NON_HT_DUP_OFDM) {
		rate_idx = rxvect->ht.mcs;
		fec_coding = rxvect->ht.fec;
		short_gi = rxvect->ht.short_gi;
		stbc = rxvect->ht.stbc;
		aggregation = rxvect->ht.aggregation;
		*pos = 0;
	} else {
        struct wiphy *wiphy = sc->wiphy;
		struct ieee80211_supported_band *band = wiphy->bands[NL80211_BAND_2GHZ];
		rtap->it_present |= cpu_to_le32(1 << IEEE80211_RADIOTAP_RATE);
		BUG_ON((rate_idx = legrates_lut[rxvect->leg_rate]) == -1);
		//if (phy_info->phy_band == NL80211_BAND_5GHZ)
		//	rate_idx -= 4;  /* rwnx_ratetable_5ghz[0].hw_value == 4 */
		*pos = DIV_ROUND_UP(band->bitrates[rate_idx].bitrate, 5);
	}
	pos++;

	// IEEE80211_RADIOTAP_CHANNEL
	rtap->it_present |= cpu_to_le32(1 << IEEE80211_RADIOTAP_CHANNEL);
	put_unaligned_le16(phy_info->phy_prim20_freq, pos);
	pos += 2;

	if (phy_info->phy_band == NL80211_BAND_5GHZ)
		put_unaligned_le16(IEEE80211_CHAN_OFDM | IEEE80211_CHAN_5GHZ, pos);
	else if (rxvect->format_mod > FORMATMOD_NON_HT_DUP_OFDM)
		put_unaligned_le16(IEEE80211_CHAN_DYN | IEEE80211_CHAN_2GHZ, pos);
	else
        put_unaligned_le16(IEEE80211_CHAN_OFDM | IEEE80211_CHAN_2GHZ, pos);//b to bg
		//put_unaligned_le16(IEEE80211_CHAN_CCK | IEEE80211_CHAN_2GHZ, pos);
	pos += 2;

	if (hweight32(rxvect->antenna_set) == 1) {
		// IEEE80211_RADIOTAP_DBM_ANTSIGNAL
		rtap->it_present |= cpu_to_le32(1 << IEEE80211_RADIOTAP_DBM_ANTSIGNAL);
		*pos++ = rxvect->rssi1;

		// IEEE80211_RADIOTAP_ANTENNA
		rtap->it_present |= cpu_to_le32(1 << IEEE80211_RADIOTAP_ANTENNA);
		*pos++ = rxvect->antenna_set;
	}

	// IEEE80211_RADIOTAP_LOCK_QUALITY is missing
	// IEEE80211_RADIOTAP_DB_ANTNOISE is missing

	// IEEE80211_RADIOTAP_RX_FLAGS
	rtap->it_present |= cpu_to_le32(1 << IEEE80211_RADIOTAP_RX_FLAGS);
	// 2 byte alignment
	if ((pos - (u8 *)rtap) & 1)
		*pos++ = 0;
	put_unaligned_le16(0, pos);
	//Right now, we only support fcs error (no RX_FLAG_FAILED_PLCP_CRC)
	pos += 2;

	// Check if HT
	if ((rxvect->format_mod == FORMATMOD_HT_MF)
			|| (rxvect->format_mod == FORMATMOD_HT_GF)) {
		rtap->it_present |= cpu_to_le32(1 << IEEE80211_RADIOTAP_MCS);
		*pos++ = IEEE80211_RADIOTAP_MCS_HAVE_MCS |
				 IEEE80211_RADIOTAP_MCS_HAVE_GI |
				 IEEE80211_RADIOTAP_MCS_HAVE_BW;
		*pos = 0;
		if (short_gi)
			*pos |= IEEE80211_RADIOTAP_MCS_SGI;
		if (rxvect->ch_bw  == PHY_CHNL_BW_40)
			*pos |= IEEE80211_RADIOTAP_MCS_BW_40;
		if (rxvect->format_mod == FORMATMOD_HT_GF)
			*pos |= IEEE80211_RADIOTAP_MCS_FMT_GF;
		if (fec_coding)
			*pos |= IEEE80211_RADIOTAP_MCS_FEC_LDPC;
		#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 17, 0)
		*pos++ |= stbc << 5;
		#else
		*pos++ |= stbc << IEEE80211_RADIOTAP_MCS_STBC_SHIFT;
		#endif
		*pos++ = rate_idx;
	}



	// check for HT or VHT frames
	if (aggregation && vect) {
		// 4 byte alignment
		while ((pos - (u8 *)rtap) & 3)
			pos++;
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 7, 0)
		rtap->it_present |= cpu_to_le32(1 << 20);
#else
		rtap->it_present |= cpu_to_le32(1 << IEEE80211_RADIOTAP_AMPDU_STATUS);
#endif
        //SSV_LOG_DBG("aggregation = %u \n", (vect->ampdu_stat_info) >> 14);
		put_unaligned_le32((vect->ampdu_stat_info) >> 14, pos);
		pos += 4;
		put_unaligned_le32(0, pos);
		pos += 4;
	}

	// Check for VHT frames
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
	if (rxvect->format_mod == FORMATMOD_VHT) {
		u16 vht_details = IEEE80211_RADIOTAP_VHT_KNOWN_GI |
						  IEEE80211_RADIOTAP_VHT_KNOWN_BANDWIDTH;
		u8 vht_nss = rxvect->vht.nss + 1;

		rtap->it_present |= cpu_to_le32(1 << IEEE80211_RADIOTAP_VHT);

		if ((rxvect->ch_bw == PHY_CHNL_BW_160)
				&& phy_info->phy_center2_freq)
			vht_details &= ~IEEE80211_RADIOTAP_VHT_KNOWN_BANDWIDTH;
		put_unaligned_le16(vht_details, pos);
		pos += 2;

		// flags
		if (short_gi)
			*pos |= IEEE80211_RADIOTAP_VHT_FLAG_SGI;
		if (stbc)
			*pos |= IEEE80211_RADIOTAP_VHT_FLAG_STBC;
		pos++;

		// bandwidth
		if (rxvect->ch_bw == PHY_CHNL_BW_40)
			*pos++ = 1;
		if (rxvect->ch_bw == PHY_CHNL_BW_80)
			*pos++ = 4;
		else if ((rxvect->ch_bw == PHY_CHNL_BW_160)
				&& phy_info->phy_center2_freq)
			*pos++ = 0; //80P80
		else if  (rxvect->ch_bw == PHY_CHNL_BW_160)
			*pos++ = 11;
		else // 20 MHz
			*pos++ = 0;

		// MCS/NSS
		*pos = (rate_idx << 4) | vht_nss;
		pos += 4;
		if (fec_coding)
			#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 15, 0)
			*pos |= 0x01;
			#else
			*pos |= IEEE80211_RADIOTAP_CODING_LDPC_USER0;
			#endif
		pos++;
		// group ID
		pos++;
		// partial_aid
		pos += 2;
	}
#endif

	// Check for HE frames  
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 20, 0)) || defined(IEEE80211_HE_MAC_CAP2_TRS)
	if (rxvect->format_mod == FORMATMOD_HE_SU) {
		struct ieee80211_radiotap_he he;
		#define HE_PREP(f, val) cpu_to_le16(FIELD_PREP(IEEE80211_RADIOTAP_HE_##f, val))
		#define D1_KNOWN(f) cpu_to_le16(IEEE80211_RADIOTAP_HE_DATA1_##f##_KNOWN)
		#define D2_KNOWN(f) cpu_to_le16(IEEE80211_RADIOTAP_HE_DATA2_##f##_KNOWN)

		he.data1 = D1_KNOWN(DATA_MCS) | D1_KNOWN(BSS_COLOR) | D1_KNOWN(BEAM_CHANGE) |
				   D1_KNOWN(UL_DL) | D1_KNOWN(CODING) |  D1_KNOWN(STBC) |
				   D1_KNOWN(BW_RU_ALLOC) | D1_KNOWN(DOPPLER) | D1_KNOWN(DATA_DCM);
		he.data2 = D2_KNOWN(GI) | D2_KNOWN(TXBF);

		if (stbc) {
			he.data6 |= HE_PREP(DATA6_NSTS, 2);
			he.data3 |= HE_PREP(DATA3_STBC, 1);
		} else {
			he.data6 |= HE_PREP(DATA6_NSTS, rxvect->he.nss);
		}

		he.data3 |= HE_PREP(DATA3_BSS_COLOR, rxvect->he.bss_color);
		he.data3 |= HE_PREP(DATA3_BEAM_CHANGE, rxvect->he.beam_change);
		he.data3 |= HE_PREP(DATA3_UL_DL, rxvect->he.uplink_flag);
		he.data3 |= HE_PREP(DATA3_BSS_COLOR, rxvect->he.bss_color);
		he.data3 |= HE_PREP(DATA3_DATA_MCS, rxvect->he.mcs);
		he.data3 |= HE_PREP(DATA3_DATA_DCM, rxvect->he.dcm);
		he.data3 |= HE_PREP(DATA3_CODING, rxvect->he.fec);

		he.data5 |= HE_PREP(DATA5_GI, rxvect->he.gi_type);
		he.data5 |= HE_PREP(DATA5_TXBF, rxvect->he.beamformed);
		he.data5 |= HE_PREP(DATA5_LTF_SIZE, rxvect->he.he_ltf_type + 1);

		switch (rxvect->ch_bw) {
		case PHY_CHNL_BW_20:
			he.data5 |= HE_PREP(DATA5_DATA_BW_RU_ALLOC,
						IEEE80211_RADIOTAP_HE_DATA5_DATA_BW_RU_ALLOC_20MHZ);
			break;
		case PHY_CHNL_BW_40:
			he.data5 |= HE_PREP(DATA5_DATA_BW_RU_ALLOC,
						IEEE80211_RADIOTAP_HE_DATA5_DATA_BW_RU_ALLOC_40MHZ);
			break;
		case PHY_CHNL_BW_80:
			he.data5 |= HE_PREP(DATA5_DATA_BW_RU_ALLOC,
						IEEE80211_RADIOTAP_HE_DATA5_DATA_BW_RU_ALLOC_80MHZ);
			break;
		case PHY_CHNL_BW_160:
			he.data5 |= HE_PREP(DATA5_DATA_BW_RU_ALLOC,
						IEEE80211_RADIOTAP_HE_DATA5_DATA_BW_RU_ALLOC_160MHZ);
			break;
		default:
			WARN_ONCE(1, "Invalid SU BW %d\n", rxvect->ch_bw);
		}

		he.data6 |= HE_PREP(DATA6_DOPPLER, rxvect->he.doppler);

		/* ensure 2 byte alignment */
		while ((pos - (u8 *)rtap) & 1)
			pos++;
		rtap->it_present |= cpu_to_le32(1 << IEEE80211_RADIOTAP_HE);
		memcpy(pos, &he, sizeof(he));
		pos += sizeof(he);
	}
#endif

	// Rx Chains
	if (hweight32(rxvect->antenna_set) > 1) {
		int chain;
		unsigned long chains = rxvect->antenna_set;
		u8 rssis[4] = {rxvect->rssi1, rxvect->rssi1, rxvect->rssi1, rxvect->rssi1};

		for_each_set_bit(chain, &chains, IEEE80211_MAX_CHAINS) {
			*pos++ = rssis[chain];
			*pos++ = chain;
		}
	}
}

static void ssv_rx_monitor(struct ssv_vif *ssv_vif, struct sk_buff *skb,struct rx_info *rx_info)
{
    int rtap_len = ssv_rx_rtap_hdrlen(&rx_info->vect.rx_vec_1, false);

	ssv_rx_add_rtap_hdr(ssv_vif, skb, &rx_info->vect.rx_vec_1,
						 (struct phy_channel_info_desc *)&rx_info->phy_info, /*NULL,*/&rx_info->vect,
						 rtap_len, 0, 0);

    skb->dev = ssv_vif->ndev;
    skb_reset_mac_header(skb);
    skb->ip_summed = CHECKSUM_UNNECESSARY;
    skb->pkt_type = PACKET_OTHERHOST;
    skb->protocol = htons(ETH_P_802_2);
    memset(skb->cb, 0, sizeof(skb->cb));
    netif_rx(skb);
}


int ssv_rx_packet_ind(void *app_param, struct sk_buff *skb)
{
    struct ssv_softc *sc = (struct ssv_softc *)app_param;
    struct rx_info *rx_info = NULL;
    struct host_reorder_info *rx_reorder_info = NULL;
    u32 *rx_desc = (u32 *)&skb->data[0];
    u8 pkt_type = (u8)((*rx_desc) & 0xFF);
    u16 real_pkt_len = (u16)((*rx_desc) >> 8);

    if(sc->pktrec != NULL) {
        if(((sc->dump_level & 0x1)==0x1) ||((sc->dump_level & 0x4)==0x4)) {
            sc->pktrec(skb, NULL);
        }
    }

    skb_pull(skb, 4); //Remove HWIF header(4-bytes).
    rx_info = (struct rx_info *)skb->data;
    skb_pull(skb, sizeof(struct rx_info)); //Remove HW RX information(84-bytes).
    rx_reorder_info = (struct host_reorder_info *)skb->data;
    skb_pull(skb, RX_BUF_HEADROOM_SIZE); //Remove SW RX headroom(24-bytes).
    if(sc->fw_reset_run == true) {
        ssv6xxx_restart_check_rx(sc, skb, pkt_type);
        dev_kfree_skb_any(skb);
        return 0;
    }
    switch(pkt_type)
    {
        //MSG
        case E_IPC_TYPE_MSG:
        {
            if(0==sc->ipc_env->cb.recv_msg_ind(sc, (void *)(skb->data)))
            {
		        dev_kfree_skb_any(skb);
                return 0;
            }
            else
            {
                /* free message */
                //SSV_LOG_DBG("unknown message id %d\n", ((struct ipc_e2a_msg *)(skb->data))->id);
		        dev_kfree_skb_any(skb);
                return 0;
            }
        }
        //ACK
        case E_IPC_TYPE_ACK:
        {
            void *hostid = sc->ipc_env->msga2e_hostid;
            ASSERT_ERR(hostid);
            sc->ipc_env->msga2e_hostid = NULL;
            sc->ipc_env->cb.recv_msgack_ind(sc, hostid);
            dev_kfree_skb_any(skb);
            return 0;
        }
        //DATA
        case E_IPC_TYPE_DATA:
        {
            u8 vif_idx = (u8)((rx_info->flags & RX_FLAGS_VIF_INDEX_MSK) >> RX_FLAGS_VIF_INDEX_OFT);
            u8 sta_idx = (u8)((rx_info->flags & RX_FLAGS_STA_INDEX_MSK) >> RX_FLAGS_STA_INDEX_OFT);
            struct ssv_vif *ssv_vif = ssv_rx_get_vif(sc, vif_idx);
            bool is_80211 = (rx_info->flags & RX_FLAGS_IS_MPDU_BIT)?true:false;
            enum data_frame_types data_type = SSV_DATA_UNKNOW;
        
            // SSV_LOG_DBG("[%s][%d] skb-len = %d, real_pkt_len = %u\n", __FUNCTION__, __LINE__, skb->len, real_pkt_len);

            if (NULL == ssv_vif)
            {
                dev_kfree_skb_any(skb);
                // SSV_LOG_DBG("[%s][%d] NULL == ssv_vif\n", __FUNCTION__, __LINE__);
                return 0;
            }

            if(false == is_80211){
                data_type = ssv_get_data_frame_type(skb);
            }

            _ssv_rx_mib(sc, (u8 *)skb->data, is_80211, sta_idx, data_type);
            //_ssv_rx_mib(sc, (u8 *)skb->data, is_80211);
            if (skb->len != real_pkt_len) {
                //SSV_LOG_DBG("[%s][%d] skb->len %d, real_pkt_len %d\n", __FUNCTION__, __LINE__, skb->len, real_pkt_len);
                if (real_pkt_len > skb->len) {
                    dev_kfree_skb_any(skb);
                    return 0;
                } 
                skb_trim(skb, real_pkt_len);
            }
            //skb->len = real_pkt_len;
            skb->dev = ssv_vif->ndev;

            if (true == is_80211)
            {
                if (NL80211_IFTYPE_MONITOR == SSV_VIF_TYPE(ssv_vif)){
                    ssv_rx_monitor(ssv_vif, skb, rx_info);
                }else{
                    struct ieee80211_mgmt *mgmt = (struct ieee80211_mgmt *)skb->data;
                    if(ieee80211_is_data(mgmt->frame_control)){
                        if(false==sc->is_stoping_apm)
                        {
                            SSV_LOG_DBG("unknow sta data frame to me: %x:%x:%x:%x:%x:%x\r\n",
                                mgmt->sa[0],
                                mgmt->sa[1],
                                mgmt->sa[2],
                                mgmt->sa[3],
                                mgmt->sa[4],
                                mgmt->sa[5]);
                            //TO DO: send deauth frame to this client
                            ssv_xmit_deauth_frame(sc, ssv_vif, NULL, mgmt->sa, mgmt->da, mgmt->da, 6);
                        }
                        dev_kfree_skb_any(skb);
                    }else{
                        {
                            struct ssv_vif *tmp_vif = NULL;
                            bool found = false;
                            struct sk_buff *skb2 = NULL;
                            
                            list_for_each_entry(tmp_vif, &sc->vifs, list) {
                                if (tmp_vif->use_monitor) {
                                    found = true; 
                                    break;
                                }
                            }

                            if (true == found) {
                                skb2 = skb_clone(skb, GFP_ATOMIC);
                                if (skb2) {
                                    skb2->dev = tmp_vif->ndev;
                                    ssv_rx_monitor(tmp_vif, skb2, rx_info);
                                }
                            }
                        }
                        ssv_rx_mgmt(sc, ssv_vif, skb, rx_info);
                    }
                }
                return 0;
            }
            
            // Check whether to drop the received packet.
            if(((NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX) <= sta_idx) || (false == sc->sta_table[sta_idx].valid))
            {
                dev_kfree_skb_any(skb);
                return 0;
            }

            {
                u8* mac_addr = (u8*)(skb->data+6);
                ssv_update_sta_last_rx(sc, mac_addr);
             }

            //skb->len = real_pkt_len;
            //skb->dev = ssv_vif->ndev;
        #if 0
            skb->protocol = eth_type_trans(skb, ssv_vif->ndev);
            memset(skb->cb, 0, sizeof(skb->cb));
        #endif
            //check data is reorder to host pattern 
            if(rx_reorder_info->upattern == RX_HOST_REORD_PATTERN) {
                if(sc->sta_table[rx_reorder_info->sta_idx].reord_info.level == 1) {
                    ssv_rxreord_info_dump(rx_reorder_info);
                    ssv_rxreord_tid_dump(sc, rx_reorder_info);
                }
                //SSV_LOG_DBG("check info ok upload before\n");
                if(ssv_rxreord_update_rxinfo_and_upload(sc, rx_reorder_info) == false) {
                    dev_kfree_skb_any(skb);
                    return 0;
                }
                //SSV_LOG_DBG("check info ok upload after\n");
                //check data is qos data or no qos data
                if(!(rx_reorder_info->frame_info & RXU_CNTRL_NO_RX_BA_TO_HOST)) {
                    //recv bar
                    if(sc->sta_table[rx_reorder_info->sta_idx].reord_info.connect == 1) {
                        //mutex_lock(&sc->sta_table[rx_reorder_info->sta_idx].reord_info.data_mutex);
                        if((rx_reorder_info->frame_info & RXU_CNTRL_BAR_TO_HOST)) {
                                //SSV_LOG_DBG("recv bar\n");
                            ssv_reord_bar_check(sc, rx_reorder_info, skb);
                        //recv qos data
                        } else {
                            //SSV_LOG_DBG("recv data\n");
#if defined(REODER_DUPPPKT_TESTMODE)
                            ssv_rxreord_duptest_into(skb->len, rx_reorder_info);
#endif
                            ssv_reord_data_check(sc, rx_reorder_info, skb);
#if defined(REODER_DUPPPKT_TESTMODE)
                            ssv_rxreord_duptest_exit(sc);
#endif
                        }
                        //mutex_unlock(&sc->sta_table[rx_reorder_info->sta_idx].reord_info.data_mutex);
                    } else {
                        dev_kfree_skb_any(skb);
                    }
                    return 0;
                }
            }
            ssv_netif_receive_skb(skb);
            return 0;
        }
        //PRIVATE MSG
        case E_IPC_TYPE_PRIV_MSG:
        {
            if(0==ssv_rx_priv_msg_handler(sc, skb))
            {
                return 0;
            }
            else
            {
                goto unknown_msg;
            }
        }
        default:
        {
            //SSV_LOG_ERR("[%s][%d] Unknown packet type(%u)!!\n", __FUNCTION__, __LINE__, pkt_type);
            //dev_kfree_skb_any(skb);
            //BUG_ON(1);
            goto unknown_msg;
        }
    }

unknown_msg:
    skb_push(skb, RX_BUF_HEADROOM_SIZE); //recovery SW RX headroom(24-bytes).
    skb_push(skb, sizeof(struct rx_info)); //recovery HW RX information(84-bytes).
    skb_push(skb, 4); //recovery HWIF header(4-bytes).
    return -1;
}

int ssv_push_private_msg_to_host(struct ssv_softc *sc, u32 msgid, u8 *data, u32 datalen)
{
    u8 *msg_buf = NULL;
    u32 msg_buf_len = 0;
    int ret = _ssv_create_host_private_msg(msgid, data, datalen, &msg_buf, &msg_buf_len);

    if(0 != ret)
    {
        goto END;
    }

    ret = _ssv_push_private_msg_to_host(sc, msg_buf, msg_buf_len);
    _ssv_free_host_private_msg(msg_buf);

END:
    return ret;
}
