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
 * @file fmac_cmds.c
 * @brief Handles queueing (push to IPC, ack/cfm from IPC) of commands issued to LMAC FW
 */


/*******************************************************************************
 *         Include Files
 ******************************************************************************/
#include <linux/device.h>
#include <linux/dmapool.h>
#include <linux/moduleparam.h>
#include <net/cfg80211.h>

#include "fmac.h"
#include "fmac_utils.h"
#include "fmac_defs.h"
#include "fmac_rx.h"
#include "fmac_tx.h"
#include "fmac_msg_rx.h"
#include "hci/drv_hci_ops.h"
#include "ssv_debug.h"
#include "fmac_msg_tx.h"

/*******************************************************************************
 *         Local Defines
 ******************************************************************************/


/*******************************************************************************
 *         Local Enumerations
 ******************************************************************************/


/*******************************************************************************
 *         Local Structures
 ******************************************************************************/


/*******************************************************************************
 *         Global Variables
 ******************************************************************************/


/*******************************************************************************
 *         Local Variables
 ******************************************************************************/


/*******************************************************************************
 *         Local Functions
 ******************************************************************************/
/**
 *
 */
static int _ssv_dbginfo_allocs(struct ssv_softc *sc)
{
    return 0;
}

/**
 *
 */
static void ssv_dbginfo_deallocs(struct ssv_softc *sc)
{
}

/**
 * @brief Deallocate storage elements.
 *
 *  This function deallocates all the elements required for communications with LMAC,
 *  such as Rx Data elements, MSGs elements, ...
 *
 * This function should be called in correspondence with the allocation function.
 *
 * @param[in]   sc      Pointer to main structure storing all the relevant information
 */
static void ssv_elems_deallocs(struct ssv_softc *sc)
{
    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);

    ssv_dbginfo_deallocs(sc);
}

/**
 * @brief Allocate storage elements.
 *
 *  This function allocates all the elements required for communications with LMAC,
 *  such as Rx Data elements, MSGs elements, ...
 *
 * This function should be called in correspondence with the deallocation function.
 *
 * @param[in]   sc      Pointer to main structure storing all the relevant information
 */
static int ssv_elems_allocs(struct ssv_softc *sc)
{
	SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);
    /* Initialize the debug information buffer */
    if (_ssv_dbginfo_allocs(sc)) {
        SSV_LOG_ERR("%s:%d: ALLOCATIONS FAILED !\n", __func__,
                 __LINE__);
        goto err_alloc;
    }

    return 0;

err_alloc:
    ssv_elems_deallocs(sc);
    return -ENOMEM;
}


/*******************************************************************************
 *         Global Functions
 ******************************************************************************/
/**
 * WLAN driver call-back function for message reception indication
 */
u8 ssv_msgind(void *pthis, void *hostid)
{
    struct ssv_softc *sc = (struct ssv_softc *)pthis;
    struct ipc_e2a_msg *msg = (struct ipc_e2a_msg *)hostid;

	// SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);

    /* Relay further actions to the msg parser */
    if(0==ssv_rx_handle_msg(sc, msg))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/**
 * FIXME
 *
 */
u8 ssv_msgackind(void *pthis, void *hostid)
{
    struct ssv_softc *sc = (struct ssv_softc *)pthis;

    sc->cmd_mgr.llind(&sc->cmd_mgr, (struct ssv_cmd *)hostid);

    return 0;
}

int ssv_ipc_init(struct ssv_softc *sc)
{
    struct ipc_host_cb_tag cb;

    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);

    /* initialize the API interface */
    cb.recv_msg_ind    = ssv_msgind;
    cb.recv_msgack_ind = ssv_msgackind;

    /* set the IPC environment */
    sc->ipc_env = (struct ipc_host_env_tag *)
                       kzalloc(sizeof(struct ipc_host_env_tag), GFP_KERNEL);

    /* call the initialization of the IPC */
    ssv_ipc_host_init(sc->ipc_env, &cb, sc);

    ssv_cmd_mgr_init(&sc->cmd_mgr);

    return ssv_elems_allocs(sc);
}

/**
 *
 */
void ssv_ipc_deinit(struct ssv_softc *sc)
{
    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);

    ssv_cmd_mgr_deinit(&sc->cmd_mgr);
    ssv_elems_deallocs(sc);
    kfree(sc->ipc_env);
    sc->ipc_env = NULL;
}

void ssv_error_ind(struct ssv_softc *sc)
{
#if 0
    SSV_LOG_DBG("%s (type %d): dump received\n", __func__,
           sc->dbginfo.buf->dbg_info.error_type);
#endif
    // sc->debugfs.trace_prst = true;
}

/**
 * Link channel ctxt to a vif and thus increments count for this context.
 */
void ssv_chanctx_link(struct ssv_vif *vif, u8 ch_idx,
                       struct cfg80211_chan_def *chandef)
{
    struct ssv_chanctx *ctxt;

    if (ch_idx >= NX_CHAN_CTXT_CNT) {
        WARN(1, "Invalid channel ctxt id %d", ch_idx);
        return;
    }

    vif->ch_index = ch_idx;
    ctxt = &vif->sc->chanctx_table[ch_idx];
    ctxt->count++;

    // For now chandef is NULL for STATION interface
    if (chandef) {
        if (!ctxt->chan_def.chan)
            ctxt->chan_def = *chandef;
        else {
            // TODO. check that chandef is the same as the one already
            // set for this ctxt
        }
    }
}

/**
 * Unlink channel ctxt from a vif and thus decrements count for this context
 */
void ssv_chanctx_unlink(struct ssv_vif *vif)
{
    struct ssv_chanctx *ctxt;

    if (vif->ch_index == SSV_CH_NOT_SET)
        return;

    ctxt = &vif->sc->chanctx_table[vif->ch_index];

    if (ctxt->count == 0) {
        WARN(1, "Chan ctxt ref count is already 0");
    } else {
        ctxt->count--;
    }

    if (ctxt->count == 0) {
        /* set chan to null, so that if this ctxt is relinked to a vif that
           don't have channel information, don't use wrong information */
        ctxt->chan_def.chan = NULL;
    }
    vif->ch_index = SSV_CH_NOT_SET;
}

int ssv_chanctx_valid(struct ssv_softc *sc, u8 ch_idx)
{
    if (ch_idx >= NX_CHAN_CTXT_CNT ||
        sc->chanctx_table[ch_idx].chan_def.chan == NULL) {
        return 0;
    }

    return 1;
}

void ssv_rxtput_calculation(struct ssv_softc *sc, u32 len, u32 seq_no)
{
    unsigned long rx_result = 0;

    if (seq_no == 0) {
        sc->rx_evt_size = len;
        sc->throughput_timestamp = jiffies;
        rx_result = 0;
    } else {
        sc->rx_evt_size += len;
        if (time_after(jiffies, (sc->throughput_timestamp + msecs_to_jiffies(1000)))) {
            rx_result = ((sc->rx_evt_size << 3) / jiffies_to_msecs(jiffies - sc->throughput_timestamp));
            SSV_LOG_DBG("data[%ld] RX throughput %ld Kbps\n", sc->rx_evt_size, rx_result);
            sc->throughput_timestamp = jiffies;
            sc->rx_evt_size = 0;
        }
    }

}

extern void ssv_update_mgmt_txdesc(struct ssv_vif *vif, struct ssv_sta *sta, 
                                        struct sk_buff *skb, bool robust, bool no_cck);
int ssv_xmit_deauth_frame(struct ssv_softc *sc, struct ssv_vif *vif, struct ssv_sta *sta, 
        u8 *da, u8 *sa, u8 *bssid, u16 reason)
{
    #define IEEE80211_DEAUTH_FRAME_LEN  (24 /* hdr */ + 2 /* reason */)
    struct ieee80211_mgmt *mgmt = NULL;
    struct sk_buff *skb=NULL;
    u8 *data = NULL;
    struct txdesc_api *desc = NULL;
    int mgmt_txq = (0 == vif->drv_vif_index) ? SSV_SW_TXQ_ID_MNG0 : SSV_SW_TXQ_ID_MNG1;

    
    skb = dev_alloc_skb(SSV_TX_HDR_SIZE + IEEE80211_DEAUTH_FRAME_LEN);
    if(!skb){
        return -1;
    }
    
    /*
     * Move skb->data pointer in order to reserve room for ssv_txhdr
     * headroom value will be equal to sizeof(struct ssv_txhdr)
     */
    skb_reserve(skb, SSV_TX_HDR_SIZE);

    /*
     * Extend the buffer data area in order to contain the provided packet
     * len value (for skb) will be equal to param->len
     */
    data = skb_put(skb, IEEE80211_DEAUTH_FRAME_LEN);

    mgmt=(struct ieee80211_mgmt *)data;
    mgmt->frame_control = cpu_to_le16(IEEE80211_FTYPE_MGMT | IEEE80211_STYPE_DEAUTH);
    mgmt->duration = 0; /* initialize only */
    mgmt->seq_ctrl = 0; /* initialize only */
    memcpy(mgmt->da, da, ETH_ALEN);
    memcpy(mgmt->sa, sa, ETH_ALEN);
    memcpy(mgmt->bssid, bssid, ETH_ALEN);
    /* u.deauth.reason_code == u.disassoc.reason_code */
    mgmt->u.deauth.reason_code = cpu_to_le16(reason);

    ssv_update_mgmt_txdesc(vif, sta, skb, 0, 0);

    desc = (struct txdesc_api *)((u8 *)skb->data + sizeof(struct sdio_hdr) + sizeof(struct tx_bmu_desc));

    desc->host.sw_ack = 0;
    //ssv_hex_dump(skb->data,SSV_TX_HDR_SIZE + IEEE80211_DEAUTH_FRAME_LEN);
#if 1
    if(-1 == ssv_fmac_hci_tx(sc, skb, mgmt_txq, false, 0))
    {
        return -EINVAL;
    }
#else

    dev_kfree_skb_any(skb);
#endif
    return 0;

}

struct sk_buff *ssv_build_nulldata_frame(u8 *da, u8 *sa, u8 *bssid, bool qos, bool fromds)
{
    struct ieee80211_qos_hdr *nullfunc;
    struct sk_buff *skb;
    int size = sizeof(struct ieee80211_qos_hdr);
    __le16 fc;
    int max_headroom = SSV_TX_HDR_SIZE;
    u8 *data; 
    
    if (qos) {
        fc = cpu_to_le16(IEEE80211_FTYPE_DATA |
                    IEEE80211_STYPE_QOS_NULLFUNC |
                    (fromds ? IEEE80211_FCTL_FROMDS : IEEE80211_FCTL_TODS));
    } else {
        size -= 2;
        fc = cpu_to_le16(IEEE80211_FTYPE_DATA |
                    IEEE80211_STYPE_NULLFUNC |
                    (fromds ? IEEE80211_FCTL_FROMDS : IEEE80211_FCTL_TODS));
    }
        
    skb = dev_alloc_skb(max_headroom + size);
    if (!skb) {
        SSV_LOG_DBG("Cannot alloc probe station packet\n");
        return NULL;
    }

    skb_reserve(skb, max_headroom);

    data = skb_put(skb, size);
    nullfunc = (struct ieee80211_qos_hdr *)data;
    nullfunc->frame_control = fc;
    nullfunc->duration_id = 0;
    memcpy(nullfunc->addr1, da, ETH_ALEN);
    memcpy(nullfunc->addr2, sa, ETH_ALEN);
    memcpy(nullfunc->addr3, bssid, ETH_ALEN);
    nullfunc->seq_ctrl = 0;
    if (qos)
        nullfunc->qos_ctrl = cpu_to_le16(7);

    return skb;
}

int ssv_fmac_hci_tx(struct ssv_softc *sc, struct sk_buff *skb, int txqid, bool force_trigger, u32 tx_flags)
{
    if ((NULL == sc->hci_priv) || (NULL == sc->hci_ops->hci_tx))
        return -1;

     if(sc->pktrec != NULL) {
        if(((sc->dump_level & 0x2)==0x2) ||((sc->dump_level & 0x8)==0x8)) {
            sc->pktrec(NULL, skb);
        }
    }
    return ssv_drv_hci_tx(sc->hci_priv, sc->hci_ops, skb, txqid, force_trigger, tx_flags);

}


#ifndef  WLAN_REASON_STA_CHANNEL_CHANGE
#define  WLAN_REASON_STA_CHANNEL_CHANGE 9999
#endif

//softap_send_deauth_to_hostapd, hostapd will disable/enable iface
static void ssv6xxx_softap_send_deauth_to_hostapd(struct ssv_vif *vif, u16 freq, enum nl80211_channel_type chann_type)
{
	u8 mac[ETH_ALEN] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
	struct sk_buff *skb;
	struct ieee80211_mgmt *deauth;

	skb = dev_alloc_skb(sizeof(struct ieee80211_mgmt) + 64);
	if(skb == NULL) {
		SSV_LOG_DBG("[%s][%d] : deauth skb allocate fail!\n", __FUNCTION__, __LINE__);
		return;
	}
	skb_reserve(skb, 64);
	deauth = (struct ieee80211_mgmt *)skb_put(skb, sizeof(struct ieee80211_mgmt));
	memcpy(deauth->da, mac, ETH_ALEN);
	memcpy(deauth->sa, mac, ETH_ALEN);
	memcpy(deauth->bssid, mac, ETH_ALEN);
	deauth->frame_control = cpu_to_le16(IEEE80211_FTYPE_MGMT |
	                                            IEEE80211_STYPE_DEAUTH |
	                                            IEEE80211_FCTL_TODS);
	deauth->u.deauth.reason_code = WLAN_REASON_STA_CHANNEL_CHANGE;
	deauth->seq_ctrl = ieee80211_frequency_to_channel(freq);
	if(NL80211_CHAN_HT40MINUS == chann_type) {
		deauth->duration = ieee80211_frequency_to_channel(freq) - 4; //use duration to send second channel
	}
	else if(NL80211_CHAN_HT40PLUS == chann_type) {
		deauth->duration = ieee80211_frequency_to_channel(freq) + 4; //use duration to send second channel
	}
	else {
		deauth->duration = 0;
	}
	
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 11, 0)  || defined(CONFIG_SSV_CHANNEL_FOLLOW)
    //only support kernel > 3.11 or cfg80211 patch
	cfg80211_rx_unprot_mlme_mgmt(vif->ndev, (u8 *)deauth, skb->len); 
    SSV_LOG_DBG("SSV send unprotected deauth to hostapd with ch_type = %d, freq = %d\n", chann_type, freq);
#endif    
	kfree_skb(skb);
}

#ifdef CONFIG_SSV_CHANNEL_FOLLOW
//check softap and station's channel  
void ssv_channel_switch_check(struct ssv_softc *sc, u16 freq, enum nl80211_channel_type chann_type)
{
    struct ssv_vif *sta_vif = NULL, *ap_vif = NULL;
    struct ssv_vif *vif;
    struct ssv_chanctx *ap_ctxt = NULL, *sta_ctxt = NULL;
    enum nl80211_channel_type ch_type = NL80211_CHAN_NO_HT;

    // Look for VIF entry
    list_for_each_entry(vif, &sc->vifs, list) {
        if (vif->up) {
            if (NL80211_IFTYPE_STATION == SSV_VIF_TYPE(vif))
                sta_vif = vif; 
            
            if (NL80211_IFTYPE_AP == SSV_VIF_TYPE(vif))
                ap_vif = vif; 
        }
    }

    if ((NULL == sta_vif) || (NULL == ap_vif)) {
        SSV_LOG_DBG("Not concurrency mode\n");
        return;
    }
    ap_ctxt = &ap_vif->sc->chanctx_table[ap_vif->ch_index];
    sta_ctxt = &sta_vif->sc->chanctx_table[sta_vif->ch_index];
    if (NULL == ap_ctxt->chan_def.chan) {
        SSV_LOG_DBG("Cannot find softap's chandef\n");
        return;
    }

    ch_type = cfg80211_get_chandef_type(&ap_ctxt->chan_def); 
    if (freq != ap_ctxt->chan_def.chan->center_freq || chann_type != ch_type) { //homeAP only change ch_type.        
            
        ssv6xxx_softap_send_deauth_to_hostapd(ap_vif, freq, chann_type);
        SSV_LOG_DBG("SSV notify channel switch.\n");
    }
}

#endif /*CONFIG_SSV_CHANNEL_FOLLOW */

void ssv6xxx_fw_reset_send_deauth_check(struct ssv_softc *sc, struct ssv_vif *vif)
{
    enum nl80211_channel_type channel_type = sc->chan_type;
    u16 center_freq = sc->center_freq;

    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);
    if (NL80211_IFTYPE_AP != SSV_VIF_TYPE(vif)) {
        SSV_LOG_DBG("error iftype!!!\n");
        return;
    }

	ssv6xxx_softap_send_deauth_to_hostapd(vif, center_freq, channel_type);
    SSV_LOG_DBG("SSV notify fw reset.\n");

}