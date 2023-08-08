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

// Include defines from config.mak to feed eclipse defines from ccflags-y
#ifdef ECLIPSE
#include <ssv_mod_conf.h>
#endif // ECLIPSE

#include <linux/version.h>

#if ((defined SSV_SUPPORT_SSV6006))

#include <linux/etherdevice.h>
#include <ssv6200.h>
#include "ssv6006_mac.h"
#include "ssv6006c/ssv6006C_reg.h"
#include "ssv6006c/ssv6006C_aux.h"
#include <smac/dev.h>
#include <smac/ssv_skb.h>
#include <hal.h>
#include "ssv6006_priv.h"
#include "ssv6006_priv_normal.h"
#include <ssvdevice/ssv_cmd.h>
#include <linux_80211.h>
#include "ssv_desc.h"
#include "turismo_common.h"
#include "ssv6006_priv.h"


#define DEBUG_MITIGATE_CCI


//const size_t ssv6006_tx_desc_length = sizeof(struct ssv6006_tx_desc);
//const size_t ssv6006_rx_desc_length = sizeof(struct ssv6006_rx_desc) + sizeof (struct ssv6006_rxphy_info);

static const u32 rc_b_data_rate[4] = {1000, 2000, 5500, 11000};
static const u32 rc_g_data_rate[8] = {6000, 9000, 12000, 18000, 24000, 36000, 48000, 54000};
static const u32 rc_n_ht20_lgi_data_rate[8] = { 6500, 13000, 19500, 26000, 39000,  52000,  58500,  65000};
static const u32 rc_n_ht20_sgi_data_rate[8] = { 7200, 14400, 21700, 28900, 43300,  57800,  65000,  72200};
static const u32 rc_n_ht40_lgi_data_rate[8] = {13500, 27000, 40500, 54000, 81000, 108000, 121500, 135000};
static const u32 rc_n_ht40_sgi_data_rate[8] = {15000, 30000, 45000, 60000, 90000, 120000, 135000, 150000};

#if 0
static bool ssv6006_get_tx_desc_last_rate(struct ssv6006_tx_desc *tx_desc, int idx)
{
    switch (idx) {
        case 0:
            return tx_desc->is_last_rate0;
        case 1:
            return tx_desc->is_last_rate1;
        case 2:
            return tx_desc->is_last_rate2;
        case 3:
            return tx_desc->is_last_rate3;
        default:
            return false;
    }
}

static u32 ssv6006_get_data_rates(struct ssv6006_rc_idx *rc_word)
{
    union  { 
        struct ssv6006_rc_idx   rcword;
        u8 val;
    } u;
    u.rcword = *rc_word;

    switch (rc_word->phy_mode){
        case SSV6006RC_B_MODE:
            return rc_b_data_rate[rc_word->rate_idx];

        case SSV6006RC_G_MODE:
            return rc_g_data_rate[rc_word->rate_idx];
        
        case SSV6006RC_N_MODE:
            if (rc_word->long_short == SSV6006RC_LONG){
                if (rc_word->ht40 == SSV6006RC_HT40){
                    return rc_n_ht40_lgi_data_rate[rc_word->rate_idx];
                } else {
                    return rc_n_ht20_lgi_data_rate[rc_word->rate_idx];
                }
            } else {
                if (rc_word->ht40 == SSV6006RC_HT40){
                    return rc_n_ht40_sgi_data_rate[rc_word->rate_idx];
                } else {
                    return rc_n_ht20_sgi_data_rate[rc_word->rate_idx];
                }                           
            }
         
        default:
            printk(" %s: invalid rate control word %x\n", __func__, u.val);
            memset(rc_word, 0 , sizeof(struct ssv6006_rc_idx));
            return 1000;             
    }
}

static void ssv6006_set_frame_duration(struct ssv_softc *sc, struct ieee80211_tx_info *info,
    struct ssv6006_rc_idx *drate_idx, struct ssv6006_rc_idx *crate_idx, 
    u16 len, struct ssv6006_tx_desc *tx_desc, u32 *nav)
{
    //struct ieee80211_tx_rate *tx_drate;
    // frame_consume_time is obosulete.
    u32 frame_time=0, ack_time = 0;// , frame_consume_time=0;
    u32 drate_kbps=0, crate_kbps=0;
    u32 rts_cts_nav[SSV6006RC_MAX_RATE_SERIES] = {0, 0, 0, 0};
    u32 l_length[SSV6006RC_MAX_RATE_SERIES] = {0, 0, 0, 0};
    bool ctrl_short_preamble=false, is_sgi, is_ht40;
    bool is_ht, is_gf, do_rts_cts;
    int d_phy ,c_phy, i, mcsidx;
    struct ssv6006_rc_idx *drate, *crate;
    bool last_rate = false;

    for (i = 0; i < SSV6006RC_MAX_RATE_SERIES ;i++)
    {   
        drate = &drate_idx[i];
        mcsidx = drate->rate_idx;
        is_sgi = drate->long_short;
        ctrl_short_preamble = drate->long_short;
	    is_ht40 = drate->ht40;
        is_ht = (drate->phy_mode == SSV6006RC_N_MODE);
        is_gf = drate->mf;
        drate_kbps = ssv6006_get_data_rates(drate);
        crate = &crate_idx[i];
        crate_kbps = ssv6006_get_data_rates(crate);
        frame_time = 0 ; 
        ack_time = 0;
        *nav = 0;

        /* Calculate data frame transmission time (include SIFS) */
        if (is_ht) {
            frame_time = ssv6xxx_ht_txtime(mcsidx, 
                    len, is_ht40, is_sgi, is_gf);
            d_phy = WLAN_RC_PHY_OFDM;//no need use this flags in n mode.
        } else {
            /**
             * Calculate frame transmission time for b/g mode:
             *     frame_time = TX_TIME(frame) + SIFS
             */
            if (drate->phy_mode == SSV6006RC_B_MODE)
                d_phy = WLAN_RC_PHY_CCK;
            else
                d_phy = WLAN_RC_PHY_OFDM;

            frame_time = ssv6xxx_non_ht_txtime(d_phy, drate_kbps, 
                len, ctrl_short_preamble);
        }

        /* get control frame phy 
         *n mode data frmaes also response g mode control frames.
         */

        if (crate->phy_mode == SSV6006RC_B_MODE)
            c_phy = WLAN_RC_PHY_CCK;
        else
            c_phy = WLAN_RC_PHY_OFDM;

        /**
            * Calculate NAV duration for data frame. The NAV can be classified
            * into the following cases:
            *    [1] NAV = 0 if the frame addr1 is MC/BC or ack_policy = no_ack
            *    [2] NAV = TX_TIME(ACK) + SIFS if non-A-MPDU frame
            *    [3] NAV = TX_TIME(BA) + SIFS if A-MPDU frame
            */
        if (tx_desc->unicast) {

            if (info->flags & IEEE80211_TX_CTL_AMPDU){
                ack_time = ssv6xxx_non_ht_txtime(c_phy, 
                    crate_kbps, BA_LEN, crate->long_short);
            } else {  
                ack_time = ssv6xxx_non_ht_txtime(c_phy, 
                    crate_kbps, ACK_LEN, crate->long_short); 
            }

           // printk("ack_time[%d] d_phy[%d] drate_kbp[%d] c_phy[%d] crate_kbps[%d] \n ctrl_short_preamble[%d]\n",
           //    ack_time, d_phy, drate_kbps, c_phy, crate_kbps, ctrl_short_preamble);

            /* to do ..... */
        }

        /**
            * Calculate NAV for RTS/CTS-to-Self frame if RTS/CTS-to-Self
            * is needed for the frame transmission:
            *       RTS_NAV = cts_time + frame_time + ack_time
            *       CTS_NAV = frame_time + ack_time
            */
        switch (i){
            case 0:
                do_rts_cts = tx_desc->do_rts_cts0;
                break;
            case 1:
                do_rts_cts = tx_desc->do_rts_cts1;
                break;
            case 2:
                do_rts_cts = tx_desc->do_rts_cts2;
                break;
            case 3:
                do_rts_cts = tx_desc->do_rts_cts3;
                break;
            default:
                do_rts_cts = tx_desc->do_rts_cts0; 
                break;                               
        }   
        if (do_rts_cts & IEEE80211_TX_RC_USE_RTS_CTS) {
            rts_cts_nav[i] = frame_time;
            rts_cts_nav[i] += ack_time; 
            rts_cts_nav[i] += ssv6xxx_non_ht_txtime(c_phy, 
                crate_kbps, CTS_LEN, crate->long_short);

            /**
                    * frame consume time:
                    *     TxTime(RTS) + SIFS + TxTime(CTS) + SIFS + TxTime(DATA)
                    *     + SIFS + TxTime(ACK)
                    */
            //frame_consume_time = rts_cts_nav;
            // frame_consume_time += ssv6xxx_non_ht_txtime(c_phy, 
            //     crate_kbps, RTS_LEN, ctrl_short_preamble);
        }else if (do_rts_cts & IEEE80211_TX_RC_USE_CTS_PROTECT) {
            rts_cts_nav[i] = frame_time;
            rts_cts_nav[i] += ack_time;

            /**
                    * frame consume time:
                    *     TxTime(CTS) + SIFS + TxTime(DATA) + SIFS + TxTime(ACK)
                    */
           // frame_consume_time = rts_cts_nav;
           // frame_consume_time += ssv6xxx_non_ht_txtime(c_phy, 
           //     crate_kbps, CTS_LEN, ctrl_short_preamble);
        } else{
            rts_cts_nav[i] = 0;
        }

        /* Calculate L-Length if using HT mode */
        if (is_ht) {
            /**
                    * Calculate frame transmission time & L-Length if the 
                    * frame is transmitted using HT-MF/HT-GF format: 
                    *
                    *  [1]. ceil[TXTIME-T_SIGEXT-20)/4], plus 3 cause 
                    *         we need to get ceil
                    *  [2]. ceil[TXTIME-T_SIGEXT-20]/4]*3 -3
                    */
            l_length[i] = frame_time - HT_SIFS_TIME;
            l_length[i] = ((l_length[i]-(HT_SIGNAL_EXT+20))+3)>>2;
            l_length[i] += ((l_length[i]<<1) - 3);
        } else {
            l_length[i] = 0;
        }
        *nav++ = ack_time ;
        
        last_rate = ssv6006_get_tx_desc_last_rate(tx_desc, i);
        if (last_rate) 
            break;
    }
    
    tx_desc->rts_cts_nav0 = rts_cts_nav[0];
    tx_desc->rts_cts_nav1 = rts_cts_nav[1];
    tx_desc->rts_cts_nav2 = rts_cts_nav[2];
    tx_desc->rts_cts_nav3 = rts_cts_nav[3];
    
    //tx_desc->frame_consume_time = (frame_consume_time>>5)+1;;
    tx_desc->dl_length0 = l_length[0];
    tx_desc->dl_length1 = l_length[1];
    tx_desc->dl_length2 = l_length[2];
    tx_desc->dl_length3 = l_length[3];
}
#endif

static bool ssv6xxx_is_eapol_frame(struct ssv_softc *sc, struct ieee80211_hdr *hdr)
{
#if 1 // turn off sw ack control
    return false;
#else
#define SSV_SNAP_SIZE   6
#define SSV_PROTOC_TYPE_SIZE    2

    u8 mac_hdr_len = ieee80211_hdrlen(hdr->frame_control);
    u8 offset;
    u16 ether_type;

    offset = mac_hdr_len + SSV_SNAP_SIZE; // tx done, it need to consider txdesc
    ether_type = be16_to_cpup((__be16 *)((u8 *)hdr + offset));
    
    if (ETH_P_PAE == ether_type)
        return true;
    else
        return false;
#endif
}

static void ssv6006_update_txinfo (struct ssv_softc *sc, struct sk_buff *skb)
{
    struct ieee80211_hdr            *hdr;
    struct ieee80211_tx_info        *info = IEEE80211_SKB_CB(skb);
    struct ieee80211_vif            *vif = info->control.vif;
    struct ieee80211_sta            *sta = NULL;
    //struct ssv_sta_info             *sta_info = NULL;/* Only protect sta_info when sta is not NULL. */
    struct ssv_sta_priv_data        *ssv_sta_priv = NULL;
    struct ssv_vif_priv_data        *vif_priv = NULL;
    struct ssv6006_tx_desc          *tx_desc = (struct ssv6006_tx_desc *)skb->data;
    int                              hw_txqid = 0;
    struct SKB_info_st              *skb_info = (struct SKB_info_st *)skb->head;
    static int                       ack_ctl_idx = 0;

    /**
     * Note that the 'sta' may be NULL. In case of the NULL condition,
     * we assign WSID to 0x0F always. The NULL condition always
     * happens before associating to an AP.
     */
    sta = skb_info->sta;
    ssv_sta_priv = (NULL == sta) ? NULL : ((struct ssv_sta_priv_data *)sta->drv_priv); 
    hdr = (struct ieee80211_hdr *)(skb->data + TXPB_OFFSET);

    /**
     * Decide frame tid & hardware output queue for outgoing
     * frames. Management frames have a dedicate output queue
     * with higher priority in station mode.
     */
    if (ieee80211_is_mgmt(hdr->frame_control) ||
             ieee80211_is_nullfunc(hdr->frame_control) ||
             ieee80211_is_qos_nullfunc(hdr->frame_control)) {
        hw_txqid = 4;
    } else if(info->flags & IEEE80211_TX_CTL_SEND_AFTER_DTIM){
        /* In AP mode we use queue 4 to send broadcast frame,
           when more than one station in sleep mode */
        hw_txqid = 4;
    } else {
        /* The skb_get_queue_mapping() returns AC */
        hw_txqid = sc->tx.hw_txqid[skb_get_queue_mapping(skb)];
    }

    /**
     * Generate tx info (tx descriptor) in M2 format for outgoing frames.
     * The software MAC of ssv6200 uses M2 format.
     */
    tx_desc->len = skb->len;
    //tx_desc->len = skb->len - sc->sh->tx_desc_len; // Exclude TX descriptor length
    tx_desc->c_type = M2_TXREQ;
    tx_desc->f80211 = 1;
    tx_desc->qos = (ieee80211_is_data_qos(hdr->frame_control))? 1: 0;
    tx_desc->use_4addr = (ieee80211_has_a4(hdr->frame_control))? 1: 0;

    tx_desc->more_data = (ieee80211_has_morefrags(hdr->frame_control))? 1: 0;
    tx_desc->stype_b5b4 = (cpu_to_le16(hdr->frame_control)>>4)&0x3;

    tx_desc->frag = (tx_desc->more_data||(hdr->seq_ctrl&0xf))? 1: 0;
    tx_desc->unicast = (is_multicast_ether_addr(hdr->addr1)) ? 0: 1;
    tx_desc->security = 0;

    /* ToDo Liam tx_burst is obsolete. Should re-consider about this ???*/
    //tx_desc->tx_burst = (tx_desc->frag)? 1: 0;

    if (vif == NULL)
        tx_desc->bssidx = 0;
    else {
        vif_priv = (struct ssv_vif_priv_data *)vif->drv_priv;
        tx_desc->bssidx = vif_priv->vif_idx;
    }

    tx_desc->wsid = (NULL == ssv_sta_priv) ? 0x0F : ssv_sta_priv->sta_idx;
    tx_desc->txq_idx = hw_txqid;
    tx_desc->hdr_offset = TXPB_OFFSET; //SSV6XXX_TX_DESC_LEN
    tx_desc->hdr_len = ssv6xxx_frame_hdrlen(hdr, tx_desc->ht);

    /* ToDo Liam payload_offset is obsolet, should re-consider about this??*/
    //tx_desc->payload_offset = tx_desc->hdr_offset + tx_desc->hdr_len;

    /* for eapol, it need to check ack */
    if (ssv6xxx_is_eapol_frame(sc, hdr)) {
        ack_ctl_idx++;
        tx_desc->sw_ack_ctl = 1;
        tx_desc->sw_ack_seq = ack_ctl_idx;
        if (64 == ack_ctl_idx)
            ack_ctl_idx = 0;
    }
    if(ieee80211_is_nullfunc(hdr->frame_control)||ieee80211_is_qos_nullfunc(hdr->frame_control))
    {
        ack_ctl_idx++;
        tx_desc->sw_ack_ctl = 1;
        tx_desc->sw_ack_seq = ack_ctl_idx;
        if (64 == ack_ctl_idx)
            ack_ctl_idx = 0;
    } 
    /* set ampdu ctl flag, FW should do ampdu aggregation */
    if (info->flags & IEEE80211_TX_CTL_AMPDU)
        tx_desc->sw_ampdu_ctl = true;
#if LINUX_VERSION_CODE > KERNEL_VERSION(3,0,8)
    /* set ampdu ctl flag, FW should not use cck rate */
    if (info->flags & IEEE80211_TX_CTL_NO_CCK_RATE)
        tx_desc->sw_no_cck = true;
#endif
    /* set RTS according to upper layer setting*/
    /* Check for RTS protection */
    if (tx_desc->unicast && ieee80211_is_data(hdr->frame_control)) {
    #if 0
        if (sc->hw->wiphy->rts_threshold != (u32) -1) {
            if ((skb->len - sc->sh->tx_desc_len) > sc->hw->wiphy->rts_threshold) {
                // Enable rts/cts flag, FW should send rts/cts by flag
                tx_desc->sw_force_rts = true;
            }
        }
    #endif
        /* CTS-to-self */
        if (sc->sc_flags & SC_OP_CTS_PROT) {
            // Enable rts/cts flag, FW should send rts/cts by flag
            tx_desc->sw_force_cts2self = true;
        }
    }

    // Check if need HW encryption
    if ((vif != NULL) &&
        ieee80211_has_protected(hdr->frame_control) && 
        (ieee80211_is_data_qos(hdr->frame_control) || ieee80211_is_data(hdr->frame_control)))
    {
        vif_priv = (struct ssv_vif_priv_data *)vif->drv_priv;
        if (   (tx_desc->unicast && ssv_sta_priv && ssv_sta_priv->has_hw_encrypt)
            || (!tx_desc->unicast && vif_priv && vif_priv->has_hw_encrypt))
        {
            // For multicast frame, find first STA's WSID for group key.
            if (!tx_desc->unicast && !list_empty(&vif_priv->sta_list))
            {
                struct ssv_sta_priv_data *one_sta_priv;
                int                       hw_wsid;
                one_sta_priv = list_first_entry(&vif_priv->sta_list, struct ssv_sta_priv_data, list);
                hw_wsid = one_sta_priv->sta_info->hw_wsid;
                if (hw_wsid != (-1))
                {
                    tx_desc->wsid = hw_wsid;
                }
            }
            tx_desc->security = 1;
        }
    }

    tx_desc->fCmdIdx = 0;
    // Packet command flow
    //  - TX queue is always the last one.
    tx_desc->fCmd = (hw_txqid+M_ENG_TX_EDCA0);
    tx_desc->fCmd = (tx_desc->fCmd << 4) | M_ENG_CPU;
    tx_desc->fCmd = (tx_desc->fCmd << 4) | M_ENG_ENCRYPT;
    tx_desc->fCmd = (tx_desc->fCmd << 4) | M_ENG_MIC;
    //  - HCI is always at the first position.
    tx_desc->fCmd = (tx_desc->fCmd << 4) | M_ENG_HWHCI;
} 

static void ssv6006_dump_ssv6006_txdesc(struct sk_buff *skb)
{   // dump ssv6006_tx_desc structure.
    struct ssv6006_tx_desc *tx_desc;
    int s;
    u8 *dat;
    
    tx_desc = (struct ssv6006_tx_desc *)skb->data;

    printk("\n>> TX desc:\n");
    for(s = 0, dat= (u8 *)skb->data; s < sizeof(struct ssv6006_tx_desc) /4; s++) {
        printk("%02x%02x%02x%02x ", dat[4*s+3], dat[4*s+2], dat[4*s+1], dat[4*s]);
        if (((s+1)& 0x03) == 0)
            printk("\n");
    }    
}    

static void ssv6006_dump_ssv6006_txframe(struct sk_buff *skb)
{
    struct ssv6006_tx_desc *tx_desc;
    int s;
    u8 *dat;
    
    tx_desc = (struct ssv6006_tx_desc *)skb->data;
    printk(">> Tx Frame:\n");
    for(s = 0, dat = skb->data; s < (tx_desc->len - TXPB_OFFSET); s++) {
        printk("%02x ", dat[TXPB_OFFSET+s]);
        if (((s+1)& 0x0F) == 0)
            printk("\n");
    }    

}

static void ssv6006_dump_tx_desc(struct sk_buff *skb)
{
    struct ssv6006_tx_desc *tx_desc;
    
    tx_desc = (struct ssv6006_tx_desc *)skb->data;

    ssv6006_dump_ssv6006_txdesc(skb);

    printk("\nlength: %d, c_type=%d, f80211=%d, qos=%d, ht=%d, use_4addr=%d, sec=%d\n", 
        tx_desc->len, tx_desc->c_type, tx_desc->f80211, tx_desc->qos, tx_desc->ht,
        tx_desc->use_4addr, tx_desc->security);
    printk("more_data=%d, sub_type=%x, extra_info=%d, aggr = %d\n", tx_desc->more_data,
        tx_desc->stype_b5b4, tx_desc->extra_info, tx_desc->aggr);
    printk("fcmd=0x%08x, hdr_offset=%d, frag=%d, unicast=%d, hdr_len=%d\n",
        tx_desc->fCmd, tx_desc->hdr_offset, tx_desc->frag, tx_desc->unicast,
        tx_desc->hdr_len);
    printk("ack_policy0=%d, do_rts_cts0=%d, ack_policy1=%d, do_rts_cts1=%d, reason=%d\n", 
        tx_desc->ack_policy0, tx_desc->do_rts_cts0,tx_desc->ack_policy1, tx_desc->do_rts_cts1, 
        tx_desc->reason);
    printk("ack_policy2=%d, do_rts_cts2=%d,ack_policy3=%d, do_rts_cts3=%d\n", 
         tx_desc->ack_policy2, tx_desc->do_rts_cts2,tx_desc->ack_policy3, tx_desc->do_rts_cts3);
    printk("fcmdidx=%d, wsid=%d, txq_idx=%d\n",
         tx_desc->fCmdIdx, tx_desc->wsid, tx_desc->txq_idx);
    printk("0:RTS/CTS Nav=%d, crate_idx=%d, drate_idx=%d, dl_len=%d, retry=%d, last_rate %d \n",
        tx_desc->rts_cts_nav0, tx_desc->crate_idx0, tx_desc->drate_idx0,
        tx_desc->dl_length0, tx_desc->try_cnt0, tx_desc->is_last_rate0);
    printk("1:RTS/CTS Nav=%d, crate_idx=%d, drate_idx=%d, dl_len=%d, retry=%d, last_rate %d  \n",
        tx_desc->rts_cts_nav1, tx_desc->crate_idx1, tx_desc->drate_idx1,
        tx_desc->dl_length1, tx_desc->try_cnt1, tx_desc->is_last_rate1);
    printk("2:RTS/CTS Nav=%d, crate_idx=%d, drate_idx=%d, dl_len=%d, retry=%d, last_rate %d  \n",
        tx_desc->rts_cts_nav2, tx_desc->crate_idx2, tx_desc->drate_idx2,
        tx_desc->dl_length2, tx_desc->try_cnt2, tx_desc->is_last_rate2);
    printk("3:RTS/CTS Nav=%d, crate_idx=%d, drate_idx=%d, dl_len=%d, retry=%d , last_rate %d \n",
        tx_desc->rts_cts_nav3, tx_desc->crate_idx3, tx_desc->drate_idx3,
        tx_desc->dl_length3, tx_desc->try_cnt3, tx_desc->is_last_rate3);

}

static void ssv6006_add_txinfo (struct ssv_softc *sc, struct sk_buff *skb)
{
    struct ssv6006_tx_desc          *tx_desc;

    /* Request more spaces in front of the payload for ssv6006 tx info: */
    skb_push(skb, TXPB_OFFSET);
    tx_desc = (struct ssv6006_tx_desc *)skb->data;
    memset((void *)tx_desc, 0, TXPB_OFFSET);
    ssv6006_update_txinfo(sc, skb);
    if (sc->log_ctrl & LOG_TX_FRAME){
        ssv6006_dump_ssv6006_txframe(skb);
    }

    if (sc->log_ctrl & LOG_TX_DESC){
        printk(" dump tx desciptor after tx add info:\n");
        ssv6006_dump_tx_desc(skb);
    }
} // end of - ssv6006_add_txinfo -

static void ssv6006_update_ampdu_txinfo(struct ssv_softc *sc, struct sk_buff *ampdu_skb)
{
    struct ssv6006_tx_desc *tx_desc = (struct ssv6006_tx_desc *)ampdu_skb->data;

    tx_desc->tx_pkt_run_no = sc->tx_pkt_run_no;

}

static void ssv6006_add_ampdu_txinfo(struct ssv_softc *sc, struct sk_buff *ampdu_skb)
{
    struct ssv6006_tx_desc *tx_desc;

    /* Request more spaces in front of the payload for ssv6006 tx info: */
    skb_push(ampdu_skb, TXPB_OFFSET);
    tx_desc = (struct ssv6006_tx_desc *)ampdu_skb->data;
    memset((void *)tx_desc, 0, TXPB_OFFSET);
    tx_desc->aggr = 2;      // enable ampdu tx 1.2
    ssv6006_update_txinfo(sc, ampdu_skb);
    
    if (sc->log_ctrl & LOG_TX_FRAME){
        ssv6006_dump_ssv6006_txframe(ampdu_skb);
    }
    
    if (sc->log_ctrl & LOG_TX_DESC){
        printk(" dump tx desciptor after tx ampdu add info:\n");
        ssv6006_dump_tx_desc(ampdu_skb);
    }
}

static int ssv6006_update_null_func_txinfo(struct ssv_softc *sc, struct ieee80211_sta *sta, struct sk_buff *skb)
{

    struct ieee80211_hdr            *hdr = (struct ieee80211_hdr *)(skb->data + TXPB_OFFSET);
    struct ssv_sta_priv_data        *ssv_sta_priv = (struct ssv_sta_priv_data *)sta->drv_priv;
    struct ssv_sta_info             *sta_info = (struct ssv_sta_info *)ssv_sta_priv->sta_info;/* sta_info is already protected by ssv6xxx_beacon_miss_work(). */
    struct ssv6006_tx_desc          *tx_desc = (struct ssv6006_tx_desc *)skb->data;
    int                              hw_txqid = 4;
    
    memset(tx_desc, 0x0, sizeof(struct ssv6006_tx_desc));
    tx_desc->len = skb->len;
    tx_desc->c_type = M2_TXREQ;
    tx_desc->f80211 = 1;
    tx_desc->unicast = 1;
    /* 
     * tx_pkt_run_no
     * 0 : mpdu packet
     * 1 ~ 127 : ampdu packet
     * 128 : nullfunc packet
     */
    tx_desc->tx_pkt_run_no = SSV6XXX_PKT_RUN_TYPE_NULLFUN;  
    tx_desc->wsid = sta_info->hw_wsid;/* sta_info is already protected by ssv6xxx_beacon_miss_work(). */
    tx_desc->txq_idx = hw_txqid;
    tx_desc->hdr_offset = TXPB_OFFSET; //SSV6XXX_TX_DESC_LEN
    tx_desc->hdr_len = ssv6xxx_frame_hdrlen(hdr, false);
    tx_desc->drate_idx0 = ((SSV6006RC_G_MODE << SSV6006RC_PHY_MODE_SFT) | (SSV6006RC_G_6M << SSV6006RC_RATE_SFT));   
    tx_desc->crate_idx0 = ((SSV6006RC_G_MODE << SSV6006RC_PHY_MODE_SFT) | (SSV6006RC_G_6M << SSV6006RC_RATE_SFT));   
    tx_desc->try_cnt0 = 0xf;
    tx_desc->is_last_rate0 = 1;   
	tx_desc->rate_rpt_mode = 1;
	
    tx_desc->fCmdIdx = 0;
    tx_desc->fCmd = (hw_txqid+M_ENG_TX_EDCA0);
    tx_desc->fCmd = (tx_desc->fCmd << 4) | M_ENG_HWHCI;
    
    //set duration time
    hdr->duration_id = ssv6xxx_non_ht_txtime(WLAN_RC_PHY_OFDM, 6000, ACK_LEN, false);//always G mode, 6M


    return 0;
}

static void ssv6006_dump_rx_desc(struct sk_buff *skb)
{
    struct ssv6006_rx_desc *rx_desc;
    int s;
    u8 *dat;

    rx_desc = (struct ssv6006_rx_desc *)skb->data;

    printk(">> RX Descriptor:\n");
    for(s = 0, dat= (u8 *)skb->data; s < sizeof(struct ssv6006_rx_desc) /4; s++) {
        printk("%02x%02x%02x%02x ", dat[4*s+3], dat[4*s+2], dat[4*s+1], dat[4*s]);
        if (((s+1)& 0x03) == 0)
            printk("\n");
    }      

    printk("\nlen=%d, c_type=%d, f80211=%d, qos=%d, ht=%d, use_4addr=%d\n",
        rx_desc->len, rx_desc->c_type, rx_desc->f80211, rx_desc->qos, rx_desc->ht, rx_desc->use_4addr);
    printk("psm=%d, stype_b5b4=%d, reason=%d, rx_result=%d, channel = %d\n", 
        rx_desc->psm, rx_desc->stype_b5b4, rx_desc->reason, rx_desc->RxResult, rx_desc->channel);
    
    printk("phy_rate=0x%x, aggregate=%d, l_length=%d, l_rate=%d, rssi = %d\n", 
        rx_desc->phy_rate, rx_desc->phy_aggregation, rx_desc->phy_l_length, rx_desc->phy_l_rate, rx_desc->phy_rssi);
    printk("snr=%d, rx_freq_offset=%d\n", rx_desc->phy_snr, rx_desc->phy_rx_freq_offset);    
       
}

static int ssv6006_get_tx_desc_size(struct ssv_hw *sh)
{
	return  sizeof(struct ssv6006_tx_desc);
}

static int ssv6006_get_tx_desc_ctype(struct sk_buff *skb)
{
	struct ssv6006_tx_desc *tx_desc = (struct ssv6006_tx_desc *) skb->data;

    return tx_desc->c_type ;
}

static int ssv6006_get_tx_desc_reason(struct sk_buff *skb)
{
	struct ssv6006_tx_desc *tx_desc = (struct ssv6006_tx_desc *) skb->data;

    return tx_desc->reason ;
}

static int ssv6006_get_tx_desc_wsid(struct sk_buff *skb)
{
	struct ssv6006_tx_desc *tx_desc = (struct ssv6006_tx_desc *) skb->data;

    return tx_desc->wsid;
}

static int ssv6006_get_tx_desc_txq_idx(struct sk_buff *skb)
{
	struct ssv6006_tx_desc *tx_desc = (struct ssv6006_tx_desc *) skb->data;

    return tx_desc->txq_idx ;
}

static void ssv6006_txtput_set_desc(struct ssv_hw *sh, struct sk_buff *skb )
{
    struct ssv6006_tx_desc *tx_desc;

	tx_desc = (struct ssv6006_tx_desc *)skb->data;
	memset((void *)tx_desc, 0xff, sizeof(struct ssv6006_tx_desc));
	tx_desc->len    = skb->len;
	tx_desc->c_type = M2_TXREQ;
	tx_desc->fCmd = (M_ENG_CPU << 4) | M_ENG_HWHCI;
	tx_desc->reason = ID_TRAP_SW_TXTPUT;
}

static void ssv6006_fill_beacon_tx_desc(struct ssv_softc *sc, struct sk_buff* beacon_skb)
{
	struct ssv6006_tx_desc *tx_desc;
        
	/* Insert description space */
	skb_push(beacon_skb, TXPB_OFFSET);

    tx_desc = (struct ssv6006_tx_desc *)beacon_skb->data;
	memset(tx_desc,0, TXPB_OFFSET);

    tx_desc->len            = beacon_skb->len-TXPB_OFFSET;
	tx_desc->c_type         = M2_TXREQ;
    tx_desc->f80211         = 1;
	tx_desc->ack_policy0    = 1;//no ack;
	tx_desc->ack_policy1    = 1;//no ack;
	tx_desc->ack_policy2    = 1;//no ack;
	tx_desc->ack_policy3    = 1;//no ack;	
    tx_desc->hdr_offset 	= TXPB_OFFSET;					
    tx_desc->hdr_len 		= 24;									

	tx_desc->wsid 			= 0xf;
	if ((sc->cur_channel->band == INDEX_80211_BAND_5GHZ) || (sc->ap_vif->p2p == true)){
        tx_desc->drate_idx0 = ((SSV6006RC_G_MODE << SSV6006RC_PHY_MODE_SFT) 
                            | (SSV6006RC_G_6M << SSV6006RC_RATE_SFT));   
        tx_desc->crate_idx0 = ((SSV6006RC_G_MODE << SSV6006RC_PHY_MODE_SFT) 
                            | (SSV6006RC_G_6M << SSV6006RC_RATE_SFT));     
	}else{
	    tx_desc->drate_idx0 = ((SSV6006RC_B_MODE << SSV6006RC_PHY_MODE_SFT) 
                            | (SSV6006RC_B_1M << SSV6006RC_RATE_SFT));   
        tx_desc->crate_idx0 = ((SSV6006RC_B_MODE << SSV6006RC_PHY_MODE_SFT) 
                            | (SSV6006RC_B_1M << SSV6006RC_RATE_SFT));  
    }
	tx_desc->try_cnt0 		= 1;
	tx_desc->is_last_rate0  = 1;
}    

static int ssv6006_get_tkip_mmic_err(struct sk_buff *skb)
{
	struct ssv6006_rx_desc *rx_desc = (struct ssv6006_rx_desc *) skb->data;
	bool ret = false;

    if (rx_desc->tkip_mmic_err == 1)
        ret = true;
    
    return ret;
}

static int ssv6006_get_rx_desc_size(struct ssv_hw *sh)
{
#define RXPB_OFFSET_KRACK 80
   if (sh->cfg.hw_caps & SSV6200_HW_CAP_KRACK){
       return RXPB_OFFSET_KRACK; 
   } else {      
/*#ifdef CAL_RX_DELAY
	
    #define EXTRA_SPACE	(2 * sizeof(u32))
       //reserve extra space for log time stamp
	   return sizeof(struct ssv6006_rx_desc) + sizeof (struct ssv6006_rxphy_info) + EXTRA_SPACE;
#else
       return sizeof(struct ssv6006_rx_desc) + sizeof (struct ssv6006_rxphy_info);
#endif*/
   	   return sizeof(struct ssv6006_rx_desc);
   }
}

static int ssv6006_get_rx_desc_length(struct ssv_hw *sh)
{
	return sizeof(struct ssv6006_rx_desc);
}

static u32 ssv6006_get_rx_desc_wsid(struct sk_buff *skb)
{
    struct ssv6006_rx_desc *rx_desc = (struct ssv6006_rx_desc *)skb->data;
    
    return rx_desc->wsid;
}

static u32 ssv6006_get_rx_desc_rate_idx(struct sk_buff *skb)
{
    struct ssv6006_rx_desc *rxdesc = (struct ssv6006_rx_desc *)skb->data;
    
    return rxdesc->phy_rate;
}

static u32 ssv6006_get_rx_desc_mng_used(struct sk_buff *skb)
{
    struct ssv6006_rx_desc *rx_desc = (struct ssv6006_rx_desc *)skb->data;
    
    return rx_desc->un_w2.mng_used;
}

static bool ssv6006_is_rx_aggr(struct sk_buff *skb)
{
    struct ssv6006_rx_desc *rxdesc = (struct ssv6006_rx_desc *)skb->data;
    
    return rxdesc->phy_aggregation;
}

static void ssv6006_get_rx_desc_info_hdr(unsigned char *desc, u32 *packet_len, u32 *c_type,
        u32 *tx_pkt_run_no)
{
    struct ssv6006_rx_desc *rxdesc = (struct ssv6006_rx_desc *)desc;
    
    if ((rxdesc->c_type == M0_RXEVENT) || (rxdesc->c_type == M2_RXEVENT))
        rxdesc->len = rxdesc->rx_len;
    
    *packet_len = rxdesc->len;
    *c_type = rxdesc->c_type;
    *tx_pkt_run_no = rxdesc->rx_pkt_run_no;
}

static u32 ssv6006_get_rx_desc_ctype(struct sk_buff *skb)
{
    struct ssv6006_rx_desc *rxdesc = (struct ssv6006_rx_desc *)skb->data;
       
    return rxdesc->c_type;
}

static int ssv6006_get_rx_desc_hdr_offset(struct sk_buff *skb)
{
    struct ssv6006_rx_desc *rxdesc = (struct ssv6006_rx_desc *)skb->data;
       
    return rxdesc->hdr_offset;
}

static u32 ssv6006_get_rx_desc_phy_rssi(struct sk_buff *skb)
{
    struct ssv6006_rx_desc *rx_desc = (struct ssv6006_rx_desc *)skb->data;

    return rx_desc->phy_rssi;
}

static bool ssv6006_nullfun_frame_filter(struct sk_buff *skb)
{
    struct ssv6006_tx_desc *txdesc = (struct ssv6006_tx_desc *)skb->data;
    
    if (txdesc->tx_pkt_run_no == SSV6XXX_PKT_RUN_TYPE_NULLFUN)
        return true;

    return false;
}

static u8 ssv6006_get_sw_ack_ctl(struct ssv_hw *sh, struct sk_buff *skb)
{
    struct ssv6006_tx_desc *txdesc = (struct ssv6006_tx_desc *)skb->data;
    u8 retval = 0;

    if ((M2_TXREQ == txdesc->c_type) && (1 == txdesc->sw_ack_ctl) && (0 == txdesc->reason))
        retval = 1;

    return retval;   
}

static u8 ssv6006_get_sw_ack_seq(struct ssv_hw *sh, struct sk_buff *skb)
{
    struct ssv6006_tx_desc *txdesc = (struct ssv6006_tx_desc *)skb->data;
    return txdesc->sw_ack_seq;   
}

static void ssv6006_phy_enable(struct ssv_hw *sh, bool val)
{
	struct ssv_softc *sc = sh->sc;
    struct sk_buff *skb;
	struct cfg_host_cmd *host_cmd;

    if(sc->sc_flags & SC_OP_HW_RESET)
    {
        SMAC_REG_SET_BITS(sh, ADR_WIFI_PHY_COMMON_ENABLE_REG, (val << RG_PHY_MD_EN_SFT), RG_PHY_MD_EN_MSK);
        return;
    }

    skb = ssv_skb_alloc(sc, HOST_CMD_HDR_LEN);
	if (skb != NULL) {
	    skb_put(skb, HOST_CMD_HDR_LEN);
	    host_cmd = (struct cfg_host_cmd *)skb->data;
        memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
	    host_cmd->c_type = HOST_CMD;
	    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_RFPHY_OPS;
        if (val) {
            host_cmd->sub_h_cmd = (u32)SSV6XXX_RFPHY_CMD_PHY_ENABLE;
            host_cmd->blocking_seq_no = (((u16)SSV6XXX_HOST_CMD_RFPHY_OPS << 16) | (u16)SSV6XXX_RFPHY_CMD_PHY_ENABLE);
	    } else {
            host_cmd->sub_h_cmd = (u32)SSV6XXX_RFPHY_CMD_PHY_DISABLE;
            host_cmd->blocking_seq_no = (((u16)SSV6XXX_HOST_CMD_RFPHY_OPS << 16) | (u16)SSV6XXX_RFPHY_CMD_PHY_DISABLE);
        }
        host_cmd->len = HOST_CMD_HDR_LEN;
   
        HCI_SEND_CMD(sc->sh, skb);
    } else
        printk("%s(): Fail to alloc cmd buffer.\n", __FUNCTION__);
}

static void ssv6006_set_phy_mode(struct ssv_hw *sh, bool val)
{
    if (val) { // set phy mode on without enable
        SMAC_REG_WRITE(sh, ADR_WIFI_PHY_COMMON_ENABLE_REG,(RG_PHYRX_MD_EN_MSK | RG_PHYTX_MD_EN_MSK |
            RG_PHY11GN_MD_EN_MSK | RG_PHY11B_MD_EN_MSK | RG_PHYRXFIFO_MD_EN_MSK | 
            RG_PHYTXFIFO_MD_EN_MSK | RG_PHY11BGN_MD_EN_MSK));
    } else { //clear phy mode
        SMAC_REG_WRITE(sh, ADR_WIFI_PHY_COMMON_ENABLE_REG, 0x00000000);         
    } 
}

static void ssv6006_edca_enable(struct ssv_hw *sh, bool val)
{
    if (val) {
	    SET_RG_EDCCA_AVG_T(SSV6006_EDCCA_AVG_T_25US);	
	    SET_RG_EDCCA_STAT_EN(0);
	    udelay(100);
        SET_RG_EDCCA_STAT_EN(1);
    } else {
	    SET_RG_EDCCA_STAT_EN(0);
    }
}

static u32 ssv6006_edca_ewma(int old, int new)
{
	//weight old 7/8, new 1/8
    return ((new + ((old << 3) - old)) >> 3);
}
  
#define MAX_EDCCA   0x3000

static void ssv6006_edca_stat(struct ssv_hw *sh)
{
    struct ssv_softc *sc = sh->sc;
    u32	regval;
    int stat = 0, period = 0, percentage = 0;
    u32 current_edcca, edcca_setting;


    //primary chan
    SMAC_REG_READ(sh, ADR_WIFI_PHY_COMMON_EDCCA_1, &regval);
    period = ((regval & RO_EDCCA_PRIMARY_PRD_MSK) >> RO_EDCCA_PRIMARY_PRD_SFT);
	stat = ((regval & RO_PRIMARY_EDCCA_MSK) >> RO_PRIMARY_EDCCA_SFT);
	if (period)
	    percentage = SSV_EDCA_FRAC(stat, period);   
    sc->primary_edca_mib = ssv6006_edca_ewma(sc->primary_edca_mib, percentage);
    
    if (sh->cfg.auto_edcca){
        current_edcca = GET_PRIMARY_EDCA(sc);
        edcca_setting = GET_RG_AGC_THRESHOLD;
        
        if ((current_edcca > 10) && (edcca_setting != MAX_EDCCA)) {
            edcca_setting = (edcca_setting << 1) ;
            if (MAX_EDCCA < (edcca_setting)){
                SET_RG_AGC_THRESHOLD(MAX_EDCCA);
            } else {
                if ( edcca_setting == 0x2000) edcca_setting = 0x3000;
                SET_RG_AGC_THRESHOLD(edcca_setting);
            }
        } else if((current_edcca == 0) && (edcca_setting != sc->default_edcca)) {
            edcca_setting = (edcca_setting >> 1) ;
            if (sc->default_edcca > (edcca_setting)){
                SET_RG_AGC_THRESHOLD(sc->default_edcca);
            } else {

                SET_RG_AGC_THRESHOLD(edcca_setting);
            }
            
        }
    }
    //secondary chan
    percentage = 0;
    SMAC_REG_READ(sh, ADR_WIFI_PHY_COMMON_EDCCA_2, &regval);
    period = ((regval & RO_EDCCA_SECONDARY_PRD_MSK) >> RO_EDCCA_SECONDARY_PRD_SFT);
	stat = ((regval & RO_SECONDARY_EDCCA_MSK) >> RO_SECONDARY_EDCCA_SFT);
	if (period)
	    percentage = SSV_EDCA_FRAC(stat, period);   
    sc->secondary_edca_mib = ssv6006_edca_ewma(sc->secondary_edca_mib, percentage);

    // restart edca
    SET_RG_EDCCA_STAT_EN(0);
	MDELAY(1);
	SET_RG_EDCCA_STAT_EN(1);
}

#ifdef CONFIG_ENABLE_ACS_FUNC
static u32 _ssv6006_turismoC_covert_edcca_duration(u32 val)
{
    switch(val)
    {
        case SSV6006_EDCCA_AVG_T_6US:
        {
            return 6; //6.4us
        }
        case SSV6006_EDCCA_AVG_T_12US:
        {
            return 12; //12.8us
        }
        case SSV6006_EDCCA_AVG_T_25US:
        {
            return 25; //25.6us
        }
        case SSV6006_EDCCA_AVG_T_51US:
        {
            return 51; //51.2us
        }
        case SSV6006_EDCCA_AVG_T_102US:
        {
            return 102; //102.4us
        }
        case SSV6006_EDCCA_AVG_T_204US:
        {
            return 204; //204.8us
        }
        case SSV6006_EDCCA_AVG_T_409US:
        {
            return 409; //409.6us
        }
        case SSV6006_EDCCA_AVG_T_819US:
        default:
        {
            return 819; //819.2us
        }
    }
}

static int _ssv6006_get_channel_idx(struct ssv_softc *sc, u16 hw_value)
{
    struct ieee80211_supported_band *sband = NULL;
    int idx = -1;
    int tmp_idx = 0;
    int i = 0;

    sband = &sc->sbands[INDEX_80211_BAND_2GHZ]; //2G
    for(i = 0; i < sband->n_channels; i++)
    {
        if(hw_value == sband->channels[i].hw_value)
        {
            idx = i;
            goto EXIT;
        }

    }
    tmp_idx += sband->n_channels;
    sband = &sc->sbands[INDEX_80211_BAND_5GHZ]; //5G
    for(i = 0; i < sband->n_channels; i++)
    {
        if(hw_value == sband->channels[i].hw_value)
        {
            idx = i+tmp_idx;
            goto EXIT;
        }
    }

EXIT:
    return idx;
}

void ssv6006_edca_update_survey(struct ssv_hw *sh)
{
    struct ssv_softc *sc = sh->sc;
    int idx = _ssv6006_get_channel_idx(sc, sc->survey_cur_chan);
    u64 primary_time = 0;
    u64 primary_time_busy = 0;
    u64 secondary_time_busy = 0;
    u32 regval = 0;
    u32 duration = 0;
    if((0 <= idx) && GET_RG_EDCCA_STAT_EN)
    {
        //primary chan
        duration = _ssv6006_turismoC_covert_edcca_duration(GET_RG_EDCCA_AVG_T);
        SMAC_REG_READ(sh, ADR_WIFI_PHY_COMMON_EDCCA_1, &regval);
        primary_time = ((regval & RO_EDCCA_PRIMARY_PRD_MSK) >> RO_EDCCA_PRIMARY_PRD_SFT)*duration; //in us
        primary_time_busy = ((regval & RO_PRIMARY_EDCCA_MSK) >> RO_PRIMARY_EDCCA_SFT)*duration; //in us

        if(primary_time > primary_time_busy)
        {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,0,0)
            sc->survey[idx].filled = SURVEY_INFO_TIME | SURVEY_INFO_TIME_BUSY;
            sc->survey[idx].time = primary_time>>10; //in ms
            sc->survey[idx].time_busy = primary_time_busy>>10; //in ms
#else
            sc->survey[idx].filled = SURVEY_INFO_CHANNEL_TIME | SURVEY_INFO_CHANNEL_TIME_BUSY;
            sc->survey[idx].channel_time = primary_time>>10; //in ms
            sc->survey[idx].channel_time_busy = primary_time_busy>>10; //in ms
#endif
            if((NL80211_CHAN_HT40MINUS == sc->hw_chan_type) || (NL80211_CHAN_HT40PLUS == sc->hw_chan_type))
            {
                //secondary chan
                SMAC_REG_READ(sh, ADR_WIFI_PHY_COMMON_EDCCA_2, &regval);
                secondary_time_busy = ((regval & RO_SECONDARY_EDCCA_MSK) >> RO_SECONDARY_EDCCA_SFT)*duration; //in us
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,0,0)
                sc->survey[idx].filled |= SURVEY_INFO_TIME_EXT_BUSY;
                sc->survey[idx].time_ext_busy = secondary_time_busy>>10; //in ms
#else
                sc->survey[idx].filled |= SURVEY_INFO_CHANNEL_TIME_EXT_BUSY;
                sc->survey[idx].channel_time_ext_busy = secondary_time_busy>>10; //in ms
#endif
            }
        }
        else
        {
            //Wrap around case
            memset((void *)&sc->survey[idx], 0, sizeof(struct survey_info)); //clear survey info by idx
        }
    }
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,0,0)
   // printk(KERN_DEBUG "idx = %d, survey time = %llu, time_busy = %llu, time_ext_busy = %llu\n", idx, sc->survey[idx].time, sc->survey[idx].time_busy, sc->survey[idx].time_ext_busy);
#else
    //printk(KERN_DEBUG "idx = %d, survey time = %llu, time_busy = %llu, time_ext_busy = %llu\n", idx, sc->survey[idx].channel_time, sc->survey[idx].channel_time_busy, sc->survey[idx].channel_time_ext_busy);
#endif
}

void ssv6006_rx_fixed_gain_enable(struct ssv_hw *sh, bool enable)
{
    if(true == enable)
    {
        SET_RG_RFGC_OW(0x1); //set LNA gain manual mode
        SET_RG_RFGC_SET(0x3); //set LNA gain to maximum
        SET_RG_PGAGC_OW(0x1); //set PGA gain manual mode
        SET_RG_PGAGC_SET(0xf); //set PGA gain to maximum
    }
    else
    {
        SET_RG_RFGC_OW(0); //set LNA gain auto mode
        SET_RG_RFGC_SET(0); //set LNA gain to minimum
        SET_RG_PGAGC_OW(0); //set PGA gain auto mode
        SET_RG_PGAGC_SET(0); //set PGA gain to minimum
    }
}
#endif

static void ssv6006_reset_mib_phy(struct ssv_hw *sh)
{
    //Reset PHY MIB
    SET_RG_MRX_EN_CNT_RST_N(0);
    SET_RG_PACKET_STAT_EN_11B_RX(0);
    SET_RG_PACKET_STAT_EN_11GN_RX(0);
    //printk("%s: %d, %d, %d\n", __func__, GET_RG_MRX_EN_CNT_RST_N, GET_RG_PACKET_STAT_EN_11B_RX, GET_RG_PACKET_STAT_EN_11GN_RX);
    msleep(1);
    SET_RG_MRX_EN_CNT_RST_N(1);
    SET_RG_PACKET_STAT_EN_11B_RX(1);   
    SET_RG_PACKET_STAT_EN_11GN_RX(1);     
}

static void ssv6006_dump_mib_rx_phy(struct ssv_hw *sh)
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;
   
    snprintf_res(cmd_data, "PHY total Rx\t:[%08x]\n", GET_RO_MRX_EN_CNT );

    snprintf_res(cmd_data, "PHY B mode:\n");
    snprintf_res(cmd_data, "%-10s\t%-10s\t%-10s\t%-10s\t%-10s\n", "SFD_CNT","CRC_CNT","PKT_ERR","CCA","PKT_CNT");
    snprintf_res(cmd_data, "[%08x]\t", GET_RO_11B_SFD_CNT);
    snprintf_res(cmd_data, "[%08x]\t", GET_RO_11B_CRC_CNT);
    snprintf_res(cmd_data, "[%08x]\t", GET_RO_11B_PACKET_ERR_CNT);
    snprintf_res(cmd_data, "[%08x]\t", GET_RO_11B_CCA_CNT);
    snprintf_res(cmd_data, "[%08x]\t\n", GET_RO_11B_PACKET_CNT);

    snprintf_res(cmd_data, "PHY G/N mode:\n");

    snprintf_res(cmd_data, "%-10s\t%-10s\t%-10s\t%-10s\t%-10s\n","AMPDU ERR", "AMPDU PKT","PKT_ERR","CCA","PKT_CNT");
    snprintf_res(cmd_data, "[%08x]\t", GET_RO_AMPDU_PACKET_ERR_CNT);
    snprintf_res(cmd_data, "[%08x]\t", GET_RO_AMPDU_PACKET_CNT);
    snprintf_res(cmd_data, "[%08x]\t", GET_RO_11GN_PACKET_ERR_CNT);
    snprintf_res(cmd_data, "[%08x]\t", GET_RO_11GN_CCA_CNT);   
    snprintf_res(cmd_data, "[%08x]\t\n\n", GET_RO_11GN_PACKET_CNT);
#endif  
}

static void ssv6006_update_mac80211_chan_info(struct ssv_softc *sc, 
            struct sk_buff *skb, struct ieee80211_rx_status *rxs)
{
	struct ssv6006_rx_desc *rxdesc = (struct ssv6006_rx_desc *)skb->data;

    rxs->band = (rxdesc->channel > 14) ? INDEX_80211_BAND_5GHZ: INDEX_80211_BAND_2GHZ;
    if (rxdesc->channel == 14)
        rxs->freq = 2484;
    else if (rxdesc->channel < 14)
        rxs->freq = 2407 + rxdesc->channel * 5;
    else
        rxs->freq = 5000 + rxdesc->channel * 5;
}

void ssv6006_rc_mac80211_rate_idx(struct ssv_softc *sc, 
            int hw_rate_idx, struct ieee80211_rx_status *rxs)
{

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,12,0)
    if (((hw_rate_idx & SSV6006RC_PHY_MODE_MSK)  >>
        SSV6006RC_PHY_MODE_SFT)== SSV6006RC_N_MODE){
        rxs->encoding = RX_ENC_HT;
        
        if (((hw_rate_idx & SSV6006RC_20_40_MSK) >> 
            SSV6006RC_20_40_SFT) == SSV6006RC_HT40){
            rxs->bw = RATE_INFO_BW_40;
        } 
        
        if (((hw_rate_idx & SSV6006RC_LONG_SHORT_MSK) >> 
            SSV6006RC_LONG_SHORT_SFT) == SSV6006RC_SHORT){
            rxs->enc_flags |= RX_ENC_FLAG_SHORT_GI;
        }     
    } else {
        rxs->encoding = RX_ENC_LEGACY;
        if (((hw_rate_idx & SSV6006RC_LONG_SHORT_MSK) >> 
            SSV6006RC_LONG_SHORT_SFT) == SSV6006RC_SHORT){
            rxs->enc_flags |= RX_ENC_FLAG_SHORTPRE;
        }
    }
#else
    if (((hw_rate_idx & SSV6006RC_PHY_MODE_MSK)  >>
        SSV6006RC_PHY_MODE_SFT)== SSV6006RC_N_MODE){
        rxs->flag |= RX_FLAG_HT;
        
        if (((hw_rate_idx & SSV6006RC_20_40_MSK) >> 
            SSV6006RC_20_40_SFT) == SSV6006RC_HT40){
            rxs->flag |= RX_FLAG_40MHZ;
        } 
        
        if (((hw_rate_idx & SSV6006RC_LONG_SHORT_MSK) >> 
            SSV6006RC_LONG_SHORT_SFT) == SSV6006RC_SHORT){
            rxs->flag |= RX_FLAG_SHORT_GI;
        }     
    } else {
        if (((hw_rate_idx & SSV6006RC_LONG_SHORT_MSK) >> 
            SSV6006RC_LONG_SHORT_SFT) == SSV6006RC_SHORT){
            rxs->flag |= RX_FLAG_SHORTPRE;
        }
    }
#endif
    
    rxs->rate_idx = (hw_rate_idx & SSV6006RC_RATE_MSK) >> 
            SSV6006RC_RATE_SFT;
    /* 
     * For legacy, g mode rate index need to conside b mode rate (rate index: 0 ~ 3)
     * Therefore, g mode rate index range is from 4 to 11 for mac80211 layer in kernel.
     */
    if ((((hw_rate_idx & SSV6006RC_PHY_MODE_MSK)  >> SSV6006RC_PHY_MODE_SFT)== SSV6006RC_G_MODE) &&
        (rxs->band == INDEX_80211_BAND_2GHZ)) {
        rxs->rate_idx += DOT11_G_RATE_IDX_OFFSET;
    }
}

void ssv6006_rc_mac80211_tx_rate_idx(struct ssv_softc *sc,
            int hw_rate_idx, struct ieee80211_tx_info *tx_info)
{
    u16 hw_chan = sc->hw_chan;
    u8 rate_idx = 0;
    u8 flags = 0;

    //[7:6] phy mode
    //[5]: ht40
    //[4]: long/short
    //[3]: mf
    //[2:0]: RateIndex
    if (SSV6006RC_N_MODE == ((hw_rate_idx & SSV6006RC_PHY_MODE_MSK)>>SSV6006RC_PHY_MODE_SFT))
    {
        flags |= IEEE80211_TX_RC_MCS;

        if (SSV6006RC_HT40 == ((hw_rate_idx & SSV6006RC_20_40_MSK)>>SSV6006RC_20_40_SFT))
        {
            flags |= IEEE80211_TX_RC_40_MHZ_WIDTH;
        }

        if (SSV6006RC_SHORT == ((hw_rate_idx & SSV6006RC_LONG_SHORT_MSK)>>SSV6006RC_LONG_SHORT_SFT))
        {
            flags |= IEEE80211_TX_RC_SHORT_GI;
        }
    }
    else
    {
        if (SSV6006RC_SHORT == ((hw_rate_idx & SSV6006RC_LONG_SHORT_MSK)>>SSV6006RC_LONG_SHORT_SFT))
        {
            flags |= IEEE80211_TX_RC_USE_SHORT_PREAMBLE;
        }
    }

    rate_idx = (hw_rate_idx&SSV6006RC_RATE_MSK)>>SSV6006RC_RATE_SFT;
    /*
     * For legacy, g mode rate index need to conside b mode rate (rate index: 0 ~ 3)
     * Therefore, g mode rate index range is from 4 to 11 for mac80211 layer in kernel.
     */
    if ((SSV6006RC_G_MODE == ((hw_rate_idx & SSV6006RC_PHY_MODE_MSK)>>SSV6006RC_PHY_MODE_SFT)) &&
        (14 >= hw_chan))
    {
        rate_idx += DOT11_G_RATE_IDX_OFFSET;
    }
    //printk("cur_rate = 0x%02x, rate_idx = %u, flags = 0x%02x\n", hw_rate_idx, rate_idx, flags);
    tx_info->status.rates[0].idx = rate_idx;
    tx_info->status.rates[0].flags = flags;
}

static void _update_rx_data_rate_stats(struct ssv_softc *sc, struct ssv_sta_priv_data *sta_priv, struct ssv6006_rx_desc *rxdesc)
{
      // update rx rate statistics
     sta_priv->rxstats.phy_mode = ((rxdesc->phy_rate & SSV6006RC_PHY_MODE_MSK) >> SSV6006RC_PHY_MODE_SFT);
     sta_priv->rxstats.ht40 = ((rxdesc->phy_rate & SSV6006RC_20_40_MSK) >> SSV6006RC_20_40_SFT);
     if (sta_priv->rxstats.phy_mode == SSV6006RC_B_MODE) {
         sta_priv->rxstats.cck_pkts[(rxdesc->phy_rate) & SSV6006RC_B_RATE_MSK] ++;
     } else if (sta_priv->rxstats.phy_mode == SSV6006RC_N_MODE) {
         sta_priv->rxstats.n_pkts[(rxdesc->phy_rate) & SSV6006RC_RATE_MSK] ++;
     } else {
         sta_priv->rxstats.g_pkts[(rxdesc->phy_rate) & SSV6006RC_RATE_MSK] ++;
     }
     //printk("phy rate %x, phy mode %d, phy_rate idx  %d\n",rxphy-> phy_rate, sta_priv->rxstats.phy_mode, (rxphy-> phy_rate) & SSV6006RC_RATE_MSK);   
}

static int _check_for_krack( struct ssv_softc *sc, struct ssv_sta_priv_data *sta_priv,
    struct sk_buff *rx_skb, struct ieee80211_hdr *hdr)
{
	struct ssv6006_rx_desc *rxdesc = (struct ssv6006_rx_desc *)rx_skb->data;
    struct ssv_sta_info         *sta_info = sta_priv->sta_info;
    struct ieee80211_vif        *vif = sta_info->vif;
    struct ssv_vif_priv_data    *vif_priv = (struct ssv_vif_priv_data *)vif->drv_priv;
    u64    pn; 
                                             
//    pn = (rxpn->pn_low & 0xffff) + (u64)(rxpn->pn_high >> 16);
	pn = (u64)rxdesc->rx_pn_0 + (((u64)rxdesc->rx_pn_1) << 8) + (((u64)rxdesc->rx_pn_2) << 16)
	+ (((u64)rxdesc->rx_pn_3) << 24) + (((u64)rxdesc->rx_pn_4) << 32) + (((u64)rxdesc->rx_pn_5) << 40);

    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_KRACK,
        "last_pn %llX, last pn mcast %llx, unitcast %d, pn %llX, seqno %d\n", 
        sta_priv->last_pn, vif_priv->last_pn_mcast, is_unicast_ether_addr(hdr->addr1), pn, hdr->seq_ctrl >>4);
          
    if (is_unicast_ether_addr(hdr->addr1)) {
        if (sta_priv->last_pn != 0){
            if ((sta_priv->last_pn) >= pn) {
                if (sta_priv->unicast_key_changed) {
                    sta_priv->unicast_key_changed = false;
                } else {
                    printk("unicast resend pn\n");
                    return -1;
                }
            } 
        }
        sta_priv->last_pn = pn;
    } else {
        if (vif_priv->last_pn_mcast != 0){
            if ((vif_priv->last_pn_mcast) >= pn) {
                if (vif_priv->group_key_changed) {
                    vif_priv->group_key_changed = false;
                } else {
                    printk("multicast resend pn\n");
                    return -1;
                }
            } 
        }
        vif_priv->last_pn_mcast = pn; 
#if 0	// this patch will drop all broadcast data, not necesaary now.
        if (is_broadcast_ether_addr(hdr->addr1)) { 
            printk("broacast!! force to drop\n");
            return -1;
        }
#endif
    }
    return 0;  
}

int ssv6006_update_rxstatus(struct ssv_softc *sc,
            struct sk_buff *rx_skb, struct ieee80211_rx_status *rxs)
{
    struct ssv6006_rx_desc *rxdesc = (struct ssv6006_rx_desc *)rx_skb->data;
    struct ieee80211_sta *sta = NULL;
    struct ssv_sta_priv_data *sta_priv = NULL;
    struct ieee80211_hdr *hdr = (struct ieee80211_hdr *)(rx_skb->data + sc->sh->rx_desc_len);
    int    ret = 0;    
    rxs->signal = (-rxdesc->phy_rssi);
    sta = ssv6xxx_find_sta_by_rx_skb(sc, rx_skb);

    if (sta){
        sta_priv = (struct ssv_sta_priv_data *)sta->drv_priv;
        if(NULL!=sta_priv)
        {
            if (ieee80211_is_data(hdr->frame_control)){

                sc->rx_data_exist = true;
                _update_rx_data_rate_stats(sc, sta_priv, rxdesc);

                if ((ieee80211_has_protected(hdr->frame_control)) && (sc->sh->cfg.hw_caps & SSV6200_HW_CAP_KRACK)){
                    ret = _check_for_krack(sc, sta_priv, rx_skb, hdr);
                }
            }

            // update RSSI for beacon mode
            if(ieee80211_is_beacon(hdr->frame_control)){
                sta_priv->beacon_rssi = rxdesc->phy_rssi;
            }

            if((sta_priv->sta_idx < SSV_NUM_STA) &&(sc->sta_info[sta_priv->sta_idx].vif != NULL))
            {
                if (sc->sta_info[sta_priv->sta_idx].vif->type == NL80211_IFTYPE_AP) 
                {
                    sc->sta_info[sta_priv->sta_idx].signal = (rxdesc->phy_rssi);
                }                                                
            }
            rxs->signal = (-sta_priv->beacon_rssi);
        }
    }
    
    return ret;
}

static int _ssv6xxx_rc_opertaion(struct ssv_softc *sc, ssv6xxx_rc_ops ops, u32 val)
{
    struct sk_buff *skb = NULL;
    struct cfg_host_cmd *host_cmd = NULL;
    int ret = 0;

    skb = dev_alloc_skb(HOST_CMD_HDR_LEN + sizeof(u32));
    if (skb == NULL) {
        printk("%s(): Fail to alloc cmd buffer.\n", __FUNCTION__);
        return -1;
    }

    skb_put(skb, HOST_CMD_HDR_LEN + sizeof(u32));
    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd)+sizeof(u32));
    host_cmd->c_type = HOST_CMD;
    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_RC_OPS;
    host_cmd->sub_h_cmd = (u32)ops;
    host_cmd->un.dat32[0] = val;
    host_cmd->blocking_seq_no = (((u16)SSV6XXX_HOST_CMD_RC_OPS << 16)|(u16)ops);
    host_cmd->len = HOST_CMD_HDR_LEN + sizeof(u32);
    ret = HCI_SEND_CMD(sc->sh, skb);

    return 0;
}

static void _ssv6xxx_rc_show_rate_info(struct ssv_cmd_data *cmd_data)
{
    snprintf_res(cmd_data, " - hex value\n");
    snprintf_res(cmd_data, "   [7:6]: phy mode \n");
    snprintf_res(cmd_data, "     [5]: ht40 \n");
    snprintf_res(cmd_data, "     [4]: long/short GI \n");
    snprintf_res(cmd_data, "     [3]: mf \n");
    snprintf_res(cmd_data, "   [2:0]: rate index \n");
    snprintf_res(cmd_data, "    B mode:\n");
    snprintf_res(cmd_data, "     0x0:  1M, 0x1:    2M, 0x2: 5.5M, 0x3:  11M\n");
    snprintf_res(cmd_data, "     with short preamble\n");
    snprintf_res(cmd_data, "     0x11: 2M, 0x12: 5.5M, 0x13: 11M\n");
    snprintf_res(cmd_data, "    G mode:\n");
    snprintf_res(cmd_data, "     0x80:  6M, 0x81:  9M, 0x82: 12M, 0x83: 18M\n");
    snprintf_res(cmd_data, "     0x84: 24M, 0x85: 36M, 0x86: 48M, 0x87: 54M\n");
    snprintf_res(cmd_data, "    N mode:\n");
    snprintf_res(cmd_data, "     HT20:\n");
    snprintf_res(cmd_data, "     with long GI\n");
    snprintf_res(cmd_data, "     0xC0: MCS0, 0xC1: MCS1, 0xC2: MCS2, 0xC3: MCS3\n");
    snprintf_res(cmd_data, "     0xC4: MCS4, 0xC5: MCS5, 0xC6: MCS6, 0xC7: MCS7\n");
    snprintf_res(cmd_data, "     with short GI\n");
    snprintf_res(cmd_data, "     0xD0: MCS0, 0xD1: MCS1, 0xD2: MCS2, 0xD3: MCS3\n");
    snprintf_res(cmd_data, "     0xD4: MCS4, 0xD5: MCS5, 0xD6: MCS6, 0xD7: MCS7\n");
    snprintf_res(cmd_data, "     HT40:\n");
    snprintf_res(cmd_data, "     with long GI\n");
    snprintf_res(cmd_data, "     0xE0: MCS0, 0xE1: MCS1, 0xE2: MCS2, 0xE3: MCS3\n");
    snprintf_res(cmd_data, "     0xE4: MCS4, 0xE5: MCS5, 0xE6: MCS6, 0xE7: MCS7\n");
    snprintf_res(cmd_data, "     with short GI\n");
    snprintf_res(cmd_data, "     0xF0: MCS0, 0xF1: MCS1, 0xF2: MCS2, 0xF3: MCS3\n");
    snprintf_res(cmd_data, "     0xF4: MCS4, 0xF5: MCS5, 0xF6: MCS6, 0xF7: MCS7\n");
}

static void ssv6006_cmd_rc(struct ssv_hw *sh, int argc, char *argv[])
{
    char *endp;
    int  val;
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data; 

    if (argc < 2) {
        snprintf_res(cmd_data, "\n rc set\n");
        return;
    }
    if (!strcmp(argv[1], "set")){
        if (argc == 4){
            val = simple_strtoul(argv[3], &endp, 0);
            if (!strcmp(argv[2], "auto")){
                snprintf_res(cmd_data, " call cmd rc set auto %s\n", argv[3]);
                _ssv6xxx_rc_opertaion(sh->sc, SSV6XXX_RC_CMD_AUTO_RATE, (u32)val);
                sh->cfg.auto_rate_enable = val;
            } else if (!strcmp(argv[2], "hex")){
                snprintf_res(cmd_data, " call cmd rc set hex %s\n", argv[3]);
                _ssv6xxx_rc_show_rate_info(cmd_data);
                _ssv6xxx_rc_opertaion(sh->sc, SSV6XXX_RC_CMD_FIXED_RATE, (u32)val);
            } else {
                snprintf_res(cmd_data, "\n rc set auto | hex [val]\n");
                return;
            }
        }
        else {
            snprintf_res(cmd_data, "\n rc set auto | hex\n");
            _ssv6xxx_rc_show_rate_info(cmd_data);
            return;
        }
    }
    else
    {
        snprintf_res(cmd_data, "\n rc set\n");
    }
    return;
}

static void ssv6006_rc_algorithm(struct ssv_softc *sc)
{
    return;
}

static void ssv6006_set_80211_hw_rate_config(struct ssv_softc *sc)
{
	struct ieee80211_hw *hw=sc->hw;

	hw->max_rates = SSV6006RC_MAX_RATE_SERIES;
	hw->max_rate_tries = SSV6006RC_MAX_RATE_RETRY;
}

static void ssv6006_rc_rx_data_handler(struct ssv_softc *sc, struct sk_buff *skb, u32 rate_index)
{
    if (sc->log_ctrl & LOG_RX_DESC){
	    struct ieee80211_sta *sta;
	    struct ieee80211_hdr *hdr = (struct ieee80211_hdr *)(skb->data + sc->sh->rx_desc_len);

        if (!(ieee80211_is_beacon(hdr->frame_control))){   // log for not beacon frame
            sta = ssv6xxx_find_sta_by_rx_skb(sc, skb);
            if (sta != NULL)
                ssv6006_dump_rx_desc(skb);
        }
    }
}

void ssv6006_rc_legacy_bitrate_to_rate_desc(int bitrate, u8 *drate)
{
	/* 
	 * build the mapping with mac80211 rate index and ssv rate descript.
	 * mac80211 rate index   <---->   ssv rate descript
	 * b mode 1M             <---->   0x00
	 * b mode 2M             <---->   0x01
	 * b mode 5.5M           <---->   0x02
	 * b mode 11M            <---->   0x03
	 * g mode 6M             <---->   0x80
	 * g mode 9M             <---->   0x81
	 */
	*drate = 0;

	switch (bitrate) {
		case 10:
				*drate = ((SSV6006RC_B_MODE << SSV6006RC_PHY_MODE_SFT) | (SSV6006RC_B_1M << SSV6006RC_RATE_SFT));
			return;
		case 20:
				*drate = ((SSV6006RC_B_MODE << SSV6006RC_PHY_MODE_SFT) | (SSV6006RC_B_2M << SSV6006RC_RATE_SFT));
			return;
		case 55:
				*drate = ((SSV6006RC_B_MODE << SSV6006RC_PHY_MODE_SFT) | (SSV6006RC_B_5_5M << SSV6006RC_RATE_SFT));
			return;
		case 110:
				*drate = ((SSV6006RC_B_MODE << SSV6006RC_PHY_MODE_SFT) | (SSV6006RC_B_11M << SSV6006RC_RATE_SFT));
			return;
		case 60:
				*drate = ((SSV6006RC_G_MODE << SSV6006RC_PHY_MODE_SFT) | (SSV6006RC_G_6M << SSV6006RC_RATE_SFT));
			return;
		case 90:
				*drate = ((SSV6006RC_G_MODE << SSV6006RC_PHY_MODE_SFT) | (SSV6006RC_G_9M << SSV6006RC_RATE_SFT));
			return;
		case 120:
				*drate = ((SSV6006RC_G_MODE << SSV6006RC_PHY_MODE_SFT) | (SSV6006RC_G_12M << SSV6006RC_RATE_SFT));
			return;
		case 180:
				*drate = ((SSV6006RC_G_MODE << SSV6006RC_PHY_MODE_SFT) | (SSV6006RC_G_18M << SSV6006RC_RATE_SFT));
			return;
		case 240:
				*drate = ((SSV6006RC_G_MODE << SSV6006RC_PHY_MODE_SFT) | (SSV6006RC_G_24M << SSV6006RC_RATE_SFT));
			return;
		case 360:
				*drate = ((SSV6006RC_G_MODE << SSV6006RC_PHY_MODE_SFT) | (SSV6006RC_G_36M << SSV6006RC_RATE_SFT));
			return;
		case 480:
				*drate = ((SSV6006RC_G_MODE << SSV6006RC_PHY_MODE_SFT) | (SSV6006RC_G_48M << SSV6006RC_RATE_SFT));
			return;
		case 540:
				*drate = ((SSV6006RC_G_MODE << SSV6006RC_PHY_MODE_SFT) | (SSV6006RC_G_54M << SSV6006RC_RATE_SFT));
			return;
		default:
				printk("For B/G mode, it doesn't support the bitrate %d kbps\n", bitrate * 100);
				WARN_ON(1);
			return;
	}
}

void ssv_attach_ssv6006_phy(struct ssv_hal_ops *hal_ops)
{
    hal_ops->add_txinfo = ssv6006_add_txinfo;
    hal_ops->update_txinfo = ssv6006_update_txinfo;
    hal_ops->update_ampdu_txinfo = ssv6006_update_ampdu_txinfo;
    hal_ops->add_ampdu_txinfo = ssv6006_add_ampdu_txinfo;
    hal_ops->update_null_func_txinfo = ssv6006_update_null_func_txinfo;
    hal_ops->get_tx_desc_size = ssv6006_get_tx_desc_size;
    hal_ops->get_tx_desc_ctype = ssv6006_get_tx_desc_ctype;
    hal_ops->get_tx_desc_reason = ssv6006_get_tx_desc_reason;
    hal_ops->get_tx_desc_wsid = ssv6006_get_tx_desc_wsid;
    hal_ops->get_tx_desc_txq_idx = ssv6006_get_tx_desc_txq_idx;
    hal_ops->txtput_set_desc = ssv6006_txtput_set_desc;
    hal_ops->fill_beacon_tx_desc = ssv6006_fill_beacon_tx_desc;

    hal_ops->get_tkip_mmic_err = ssv6006_get_tkip_mmic_err;
    hal_ops->get_rx_desc_size = ssv6006_get_rx_desc_size;    
    hal_ops->get_rx_desc_length = ssv6006_get_rx_desc_length;
    hal_ops->get_rx_desc_wsid = ssv6006_get_rx_desc_wsid;
    hal_ops->get_rx_desc_rate_idx = ssv6006_get_rx_desc_rate_idx;
    hal_ops->get_rx_desc_mng_used = ssv6006_get_rx_desc_mng_used;
    hal_ops->is_rx_aggr = ssv6006_is_rx_aggr;
    hal_ops->get_rx_desc_ctype = ssv6006_get_rx_desc_ctype;
    hal_ops->get_rx_desc_hdr_offset = ssv6006_get_rx_desc_hdr_offset;
    hal_ops->get_rx_desc_info_hdr = ssv6006_get_rx_desc_info_hdr;
    hal_ops->get_rx_desc_phy_rssi = ssv6006_get_rx_desc_phy_rssi;
    hal_ops->nullfun_frame_filter = ssv6006_nullfun_frame_filter;
    hal_ops->get_sw_ack_ctl = ssv6006_get_sw_ack_ctl;
    hal_ops->get_sw_ack_seq = ssv6006_get_sw_ack_seq;

    hal_ops->phy_enable = ssv6006_phy_enable;
    hal_ops->set_phy_mode = ssv6006_set_phy_mode;
    hal_ops->edca_enable = ssv6006_edca_enable;
    hal_ops->edca_stat = ssv6006_edca_stat;
#ifdef	CONFIG_ENABLE_ACS_FUNC
	hal_ops->edca_update_survey = ssv6006_edca_update_survey;
	hal_ops->rx_fixed_gain_enable = ssv6006_rx_fixed_gain_enable;
#endif	
    hal_ops->reset_mib_phy = ssv6006_reset_mib_phy;
    hal_ops->dump_mib_rx_phy = ssv6006_dump_mib_rx_phy;
    hal_ops->update_mac80211_chan_info = ssv6006_update_mac80211_chan_info;
    hal_ops->rc_mac80211_rate_idx = ssv6006_rc_mac80211_rate_idx;
    hal_ops->rc_mac80211_tx_rate_idx = ssv6006_rc_mac80211_tx_rate_idx;
    hal_ops->update_rxstatus = ssv6006_update_rxstatus;
    hal_ops->cmd_rc = ssv6006_cmd_rc;

	hal_ops->rc_algorithm = ssv6006_rc_algorithm;
	hal_ops->set_80211_hw_rate_config = ssv6006_set_80211_hw_rate_config;
	hal_ops->rc_legacy_bitrate_to_rate_desc = ssv6006_rc_legacy_bitrate_to_rate_desc;
	hal_ops->rc_rx_data_handler = ssv6006_rc_rx_data_handler;
}
#endif
