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
 * @file fmac_msg_rx.c
 * @brief RX function definitions
 */


/*******************************************************************************
 *         Include Files
 ******************************************************************************/
#include <linux/version.h>
#include <linux/types.h>

#include "fmac.h"
#include "hci/drv_hci_ops.h"
#include "fmac_defs.h"
#include "fmac_strs.h"
#include "fmac_tx.h"
#include "fmac_rx.h"
#include "fmac_msg_tx.h"

#include "ssv_cfg.h"
#include "ssv_debug.h"
#include <linux/semaphore.h>


/*******************************************************************************
 *         Local Defines
 ******************************************************************************/
static inline int ssv_rx_chan_switch_ind(struct ssv_softc *sc, struct ssv_cmd *cmd, struct ipc_e2a_msg *msg);
static inline int ssv_rx_chan_pre_switch_ind(struct ssv_softc *sc, struct ssv_cmd *cmd, struct ipc_e2a_msg *msg);
static inline int ssv_rx_remain_on_channel_exp_ind(struct ssv_softc *sc, struct ssv_cmd *cmd, struct ipc_e2a_msg *msg);
static inline int ssv_rx_ps_change_ind(struct ssv_softc *sc, struct ssv_cmd *cmd, struct ipc_e2a_msg *msg);
static inline int ssv_rx_traffic_req_ind(struct ssv_softc *sc, struct ssv_cmd *cmd, struct ipc_e2a_msg *msg);
static inline int ssv_rx_csa_counter_ind(struct ssv_softc *sc, struct ssv_cmd *cmd, struct ipc_e2a_msg *msg);
static inline int ssv_rx_csa_finish_ind(struct ssv_softc *sc, struct ssv_cmd *cmd, struct ipc_e2a_msg *msg);
static inline int ssv_rx_csa_traffic_ind(struct ssv_softc *sc, struct ssv_cmd *cmd, struct ipc_e2a_msg *msg);
static inline int ssv_rx_channel_survey_ind(struct ssv_softc *sc, struct ssv_cmd *cmd, struct ipc_e2a_msg *msg);
static inline int ssv_rx_rssi_status_ind(struct ssv_softc *sc, struct ssv_cmd *cmd, struct ipc_e2a_msg *msg);
static inline int ssv_rx_scanu_start_cfm(struct ssv_softc *sc, struct ssv_cmd *cmd, struct ipc_e2a_msg *msg);
static inline int ssv_rx_scanu_result_ind(struct ssv_softc *sc, struct ssv_cmd *cmd, struct ipc_e2a_msg *msg);
static inline int ssv_rx_me_tkip_mic_failure_ind(struct ssv_softc *sc, struct ssv_cmd *cmd, struct ipc_e2a_msg *msg);
static inline int ssv_rx_me_tx_credits_update_ind(struct ssv_softc *sc, struct ssv_cmd *cmd, struct ipc_e2a_msg *msg);
static inline int ssv_rx_sm_connect_ind(struct ssv_softc *sc, struct ssv_cmd *cmd, struct ipc_e2a_msg *msg);
static inline int ssv_rx_sm_disconnect_ind(struct ssv_softc *sc, struct ssv_cmd *cmd, struct ipc_e2a_msg *msg);
static inline int ssv_rx_sm_external_auth_required_ind(struct ssv_softc *sc, struct ssv_cmd *cmd, struct ipc_e2a_msg *msg);
static inline int ssv_rx_dbg_error_ind(struct ssv_softc *sc, struct ssv_cmd *cmd, struct ipc_e2a_msg *msg);

#ifdef CONFIG_SSV_CHANNEL_FOLLOW
extern void ssv_channel_switch_check(struct ssv_softc *sc, u16 freq, enum nl80211_channel_type chann_type);
#endif /*CONFIG_SSV_CHANNEL_FOLLOW */

/*******************************************************************************
 *         Local Enumerations
 ******************************************************************************/
#if defined(CONFIG_WPA_SUPPLICANT_CTL)
enum ssv_external_auth_action {
    SSV_EXTERNAL_AUTH_START,
    SSV_EXTERNAL_AUTH_ABORT,
};
#endif


/*******************************************************************************
 *         Local Structures
 ******************************************************************************/
#if defined(CONFIG_WPA_SUPPLICANT_CTL)
struct ssv_external_auth_params {
    enum ssv_external_auth_action action;
    u8 bssid[ETH_ALEN] __aligned(2);
    struct cfg80211_ssid ssid;
    unsigned int key_mgmt_suite;
    u16 status;
};
#endif


/*******************************************************************************
 *         Global Variables
 ******************************************************************************/
extern struct ssv6xxx_cfg ssv_cfg;

/*******************************************************************************
 *         Local Variables
 ******************************************************************************/
static msg_cb_fct mm_hdlrs[MSG_I(MM_MAX)] = {
    [MSG_I(MM_CHANNEL_SWITCH_IND)]     = ssv_rx_chan_switch_ind,
    [MSG_I(MM_CHANNEL_PRE_SWITCH_IND)] = ssv_rx_chan_pre_switch_ind,
    [MSG_I(MM_REMAIN_ON_CHANNEL_EXP_IND)] = ssv_rx_remain_on_channel_exp_ind,
    [MSG_I(MM_PS_CHANGE_IND)]          = ssv_rx_ps_change_ind,
    [MSG_I(MM_TRAFFIC_REQ_IND)]        = ssv_rx_traffic_req_ind,
    [MSG_I(MM_CSA_COUNTER_IND)]        = ssv_rx_csa_counter_ind,
    [MSG_I(MM_CSA_FINISH_IND)]         = ssv_rx_csa_finish_ind,
    [MSG_I(MM_CSA_TRAFFIC_IND)]        = ssv_rx_csa_traffic_ind,
    [MSG_I(MM_CHANNEL_SURVEY_IND)]     = ssv_rx_channel_survey_ind,
    [MSG_I(MM_RSSI_STATUS_IND)]        = ssv_rx_rssi_status_ind,
};

static msg_cb_fct scan_hdlrs[MSG_I(SCANU_MAX)] = {
    [MSG_I(SCANU_START_CFM)]           = ssv_rx_scanu_start_cfm,
    [MSG_I(SCANU_RESULT_IND)]          = ssv_rx_scanu_result_ind,
};

static msg_cb_fct me_hdlrs[MSG_I(ME_MAX)] = {
    [MSG_I(ME_TKIP_MIC_FAILURE_IND)] = ssv_rx_me_tkip_mic_failure_ind,
    [MSG_I(ME_TX_CREDITS_UPDATE_IND)] = ssv_rx_me_tx_credits_update_ind,
};

static msg_cb_fct sm_hdlrs[MSG_I(SM_MAX)] = {
    [MSG_I(SM_CONNECT_IND)]    = ssv_rx_sm_connect_ind,
    [MSG_I(SM_DISCONNECT_IND)] = ssv_rx_sm_disconnect_ind,
    [MSG_I(SM_EXTERNAL_AUTH_REQUIRED_IND)] = ssv_rx_sm_external_auth_required_ind,
};

static msg_cb_fct apm_hdlrs[MSG_I(APM_MAX)] = {
};

static msg_cb_fct dbg_hdlrs[MSG_I(DBG_MAX)] = {
    [MSG_I(DBG_ERROR_IND)]                = ssv_rx_dbg_error_ind,
};

static msg_cb_fct *msg_hdlrs[] = {
    [TASK_MM]    = mm_hdlrs,
    [TASK_DBG]   = dbg_hdlrs,
    [TASK_SCANU] = scan_hdlrs,
    [TASK_ME]    = me_hdlrs,
    [TASK_SM]    = sm_hdlrs,
    [TASK_APM]   = apm_hdlrs,
};


/*******************************************************************************
 *         Local Functions
 ******************************************************************************/
static int ssv_freq_to_idx(struct ssv_softc *sc, int freq)
{
    struct ieee80211_supported_band *sband;
    int band, ch, idx = 0;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4,7,0))
    for (band = NL80211_BAND_2GHZ; band < NUM_NL80211_BANDS; band++) {
#else
    for (band = IEEE80211_BAND_2GHZ; band < IEEE80211_NUM_BANDS; band++) {
#endif
        sband = sc->wiphy->bands[band];
        if (!sband) {
            continue;
        }

        for (ch = 0; ch < sband->n_channels; ch++, idx++) {
            if (sband->channels[ch].center_freq == freq) {
                goto exit;
            }
        }
    }

    BUG_ON(1);

exit:
    // Channel has been found, return the index
    return idx;
}

/***************************************************************************
 * Messages from MM task
 **************************************************************************/
static inline int ssv_rx_chan_pre_switch_ind(struct ssv_softc *sc,
                                              struct ssv_cmd *cmd,
                                              struct ipc_e2a_msg *msg)
{
    //do nothing
    return 0;
}

static inline int ssv_rx_chan_switch_ind(struct ssv_softc *sc,
                                          struct ssv_cmd *cmd,
                                          struct ipc_e2a_msg *msg)
{
    int chan_idx = ((struct mm_channel_switch_ind *)msg->param)->chan_index;
    struct ssv_vif *ssv_vif;
    bool roc     = ((struct mm_channel_switch_ind *)msg->param)->roc;

    if (roc) {
        /* Retrieve the allocated RoC element */
        struct ssv_roc_elem *roc_elem;

        roc_elem = sc->roc_elem;

        if (!roc_elem) {
            goto out;
        }

        /* Get VIF on which RoC has been started */
        ssv_vif = netdev_priv(roc_elem->wdev->netdev);

        /* Keep in mind that we have switched on the channel */
        roc_elem->on_chan = true;

        /* If mgmt_roc is true, remain on channel has been started by ourself */
        if (!roc_elem->mgmt_roc) {
            /* Inform the host that we have switch on the indicated off-channel */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0))
            cfg80211_ready_on_channel(roc_elem->wdev, (u64)(sc->roc_cookie_cnt),
                                      roc_elem->chan, roc_elem->duration, GFP_KERNEL);
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0))
            cfg80211_ready_on_channel(roc_elem->wdev, (u64)(sc->roc_cookie_cnt),
                                      roc_elem->chan, NL80211_CHAN_NO_HT,
                                      roc_elem->duration, GFP_KERNEL);
#else
            cfg80211_ready_on_channel(roc_elem->wdev->netdev, (u64)(sc->roc_cookie_cnt),
                                      roc_elem->chan, NL80211_CHAN_NO_HT,
                                      roc_elem->duration, GFP_KERNEL);
#endif
        }
    }

out:
    sc->cur_chanctx = chan_idx;

    return 0;
}

static inline int ssv_rx_remain_on_channel_exp_ind(struct ssv_softc *sc,
                                                    struct ssv_cmd *cmd,
                                                    struct ipc_e2a_msg *msg)
{
    /* Retrieve the allocated RoC element */
    struct ssv_roc_elem *roc_elem;
    struct wireless_dev *wdev;
    struct ieee80211_channel *chan;
    bool mgmt_roc, on_chan;
    u32 roc_cookie_cnt;

    roc_elem = sc->roc_elem;

    if (!roc_elem) {
        SSV_LOG_DBG("%s() roc_elem is null\n", __FUNCTION__);
        return 0;
    }

    wdev = roc_elem->wdev;
    chan = roc_elem->chan;
    mgmt_roc = roc_elem->mgmt_roc;
    on_chan = roc_elem->on_chan;
    roc_cookie_cnt = sc->roc_cookie_cnt;

    /* Free the allocated RoC element */
    kfree(sc->roc_elem);
    sc->roc_elem = NULL;

    /* Increase the cookie counter cannot be zero */
    sc->roc_cookie_cnt++;

    if (sc->roc_cookie_cnt == 0) {
        sc->roc_cookie_cnt = 1;
    }

    /* If mgmt_roc is true, remain on channel has been started by ourself */
    /* If RoC has been cancelled before we switched on channel, do not call cfg80211 */
    if (!mgmt_roc && on_chan) {
        /* Inform the host that off-channel period has expired */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0))
        cfg80211_remain_on_channel_expired(wdev, (u64)roc_cookie_cnt,
                                           chan, GFP_KERNEL);
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0))
        cfg80211_remain_on_channel_expired(wdev, (u64)roc_cookie_cnt,
                                           chan, NL80211_CHAN_NO_HT, GFP_KERNEL);
#else
        cfg80211_remain_on_channel_expired(wdev->netdev, (u64)roc_cookie_cnt,
                                           chan, NL80211_CHAN_NO_HT, GFP_KERNEL);
#endif
    }

    return 0;
}

static inline int ssv_rx_p2p_vif_ps_change_ind(struct ssv_softc *sc,
                                                struct ssv_cmd *cmd,
                                                struct ipc_e2a_msg *msg)
{
    int vif_idx  = ((struct mm_p2p_vif_ps_change_ind *)msg->param)->vif_index;
    struct ssv_vif *vif_entry;

    vif_entry = sc->vif_table[vif_idx];

    if (vif_entry) {
        goto found_vif;
    }

    goto exit;

found_vif:

exit:
    return 0;
}

static inline int ssv_rx_channel_survey_ind(struct ssv_softc *sc,
                                             struct ssv_cmd *cmd,
                                             struct ipc_e2a_msg *msg)
{
    struct mm_channel_survey_ind *ind = (struct mm_channel_survey_ind *)msg->param;
    // Get the channel index
    int idx = ssv_freq_to_idx(sc, ind->freq);
    // Get the survey
    struct ssv_survey_info *ssv_survey = &sc->survey[idx];

    

    // Store the received parameters
    ssv_survey->chan_time_ms = ind->chan_time_ms;
    ssv_survey->chan_time_busy_ms = ind->chan_time_busy_ms;
    ssv_survey->noise_dbm = ind->noise_dbm;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,0,0)
    ssv_survey->filled = (SURVEY_INFO_TIME |
                           SURVEY_INFO_TIME_BUSY);
#else
    ssv_survey->filled = (SURVEY_INFO_CHANNEL_TIME |
                           SURVEY_INFO_CHANNEL_TIME_BUSY);
#endif

    if (ind->noise_dbm != 0) {
        ssv_survey->filled |= SURVEY_INFO_NOISE_DBM;
    }

    return 0;
}

static inline int ssv_rx_rssi_status_ind(struct ssv_softc *sc,
                                          struct ssv_cmd *cmd,
                                          struct ipc_e2a_msg *msg)
{
#define RX_RSSI_NOTIFY_CFG80211_ENABLE  (0)
    struct mm_rssi_status_ind *ind = (struct mm_rssi_status_ind *)msg->param;
    int vif_idx  = ind->vif_index;
#if (RX_RSSI_NOTIFY_CFG80211_ENABLE == 1)
    bool rssi_status = ind->rssi_status;
#endif
    int8_t rssi = ind->rssi;
    struct ssv_vif *vif_entry;
    int  i = 0 ;

    vif_entry = sc->vif_table[vif_idx];
    if(sc->rssicnt == 0) {
        sc->rssicnt = RSSI_MAX;
        //memset(sc->rssival, rssi, RSSI_MAX);
        for (i = 0; i < RSSI_MAX; i++)
        {
             sc->rssival[i]=rssi;
        }  
        sc->rssiaccu = rssi * RSSI_MAX;
        sc->rssiidx = 0;
    } else {
        sc->rssiaccu = sc->rssiaccu - sc->rssival[sc->rssiidx] + rssi;
        sc->rssival[sc->rssiidx] = rssi;
        sc->rssiidx = ((sc->rssiidx + 1) % RSSI_MAX);
    }

#if (RX_RSSI_NOTIFY_CFG80211_ENABLE == 1)
    if (vif_entry) {
        cfg80211_cqm_rssi_notify(vif_entry->ndev,
                                 rssi_status ? NL80211_CQM_RSSI_THRESHOLD_EVENT_LOW :
                                               NL80211_CQM_RSSI_THRESHOLD_EVENT_HIGH,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
				 0,
#endif
                                 GFP_KERNEL);
    }
#endif
    return 0;
}

static inline int ssv_rx_csa_counter_ind(struct ssv_softc *sc,
                                          struct ssv_cmd *cmd,
                                          struct ipc_e2a_msg *msg)
{
    struct mm_csa_counter_ind *ind = (struct mm_csa_counter_ind *)msg->param;
    struct ssv_vif *vif;
    bool found = false;

    

    // Look for VIF entry
    list_for_each_entry(vif, &sc->vifs, list) {
        if (vif->drv_vif_index == ind->vif_index) {
            found=true;
            break;
        }
    }

    if (found) {
        if (vif->ap.csa)
            vif->ap.csa->count = ind->csa_count;
        else
            netdev_err(vif->ndev, "CSA counter update but no active CSA");
    }

    return 0;
}

static inline int ssv_rx_csa_finish_ind(struct ssv_softc *sc,
                                         struct ssv_cmd *cmd,
                                         struct ipc_e2a_msg *msg)
{
    struct mm_csa_finish_ind *ind = (struct mm_csa_finish_ind *)msg->param;
    struct ssv_vif *vif;
    bool found = false;

    

    // Look for VIF entry
    list_for_each_entry(vif, &sc->vifs, list) {
        if (vif->drv_vif_index == ind->vif_index) {
            found=true;
            break;
        }
    }

    if (found) {
        if (SSV_VIF_TYPE(vif) == NL80211_IFTYPE_AP ||
            SSV_VIF_TYPE(vif) == NL80211_IFTYPE_P2P_GO) {
            if (vif->ap.csa) {
                vif->ap.csa->status = ind->status;
                vif->ap.csa->ch_idx = ind->chan_idx;
                schedule_work(&vif->ap.csa->work);
            } else
                netdev_err(vif->ndev, "CSA finish indication but no active CSA");
        } else {
            if (ind->status == 0) {
                ssv_chanctx_unlink(vif);
                ssv_chanctx_link(vif, ind->chan_idx, NULL);
            }
        }
    }

    return 0;
}

static inline int ssv_rx_csa_traffic_ind(struct ssv_softc *sc,
                                          struct ssv_cmd *cmd,
                                          struct ipc_e2a_msg *msg)
{
    struct mm_csa_traffic_ind *ind = (struct mm_csa_traffic_ind *)msg->param;
    struct ssv_vif *vif;
    bool found = false;

    

    // Look for VIF entry
    list_for_each_entry(vif, &sc->vifs, list) {
        if (vif->drv_vif_index == ind->vif_index) {
            found=true;
            break;
        }
    }

    if (found) {

    }

    return 0;
}

static inline int ssv_rx_ps_change_ind(struct ssv_softc *sc,
                                        struct ssv_cmd *cmd,
                                        struct ipc_e2a_msg *msg)
{
    struct mm_ps_change_ind *ind = (struct mm_ps_change_ind *)msg->param;
    struct ssv_sta *sta = &sc->sta_table[ind->sta_idx];

#if 0
    SSV_LOG_DBG(KERN_ERR"Sta %d, change PS mode to %s", sta->sta_idx,
               ind->ps_state ? "ON" : "OFF");
#endif

    if (sta->valid) {
        ssv_ps_bh_enable(sc, sta, ind->ps_state);
    } else if (sc->adding_sta) {
        sta->ps.active = ind->ps_state ? true : false;
    } else {
        SSV_LOG_DBG(KERN_ERR"Ignore PS mode change on invalid sta\n");
    }

    return 0;
}


static inline int ssv_rx_traffic_req_ind(struct ssv_softc *sc,
                                          struct ssv_cmd *cmd,
                                          struct ipc_e2a_msg *msg)
{
    struct mm_traffic_req_ind *ind = (struct mm_traffic_req_ind *)msg->param;
    struct ssv_sta *sta = &sc->sta_table[ind->sta_idx];

#if 0
    SSV_LOG_DBG(KERN_ERR"Sta %d, asked for %d pkt", sta->sta_idx, ind->pkt_cnt);
#endif

    ssv_ps_bh_traffic_req(sc, sta, ind->pkt_cnt,
                           ind->uapsd ? UAPSD_ID : LEGACY_PS_ID);

    return 0;
}

/***************************************************************************
 * Messages from SCANU task
 **************************************************************************/
static inline int ssv_rx_scanu_start_cfm(struct ssv_softc *sc,
                                          struct ssv_cmd *cmd,
                                          struct ipc_e2a_msg *msg)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 8, 0))
	struct cfg80211_scan_info info = {
    	.aborted = false,
	};
#endif
    

    if (sc->scan_request)
    {
#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 8, 0))
        cfg80211_scan_done(sc->scan_request, false);
#else
        cfg80211_scan_done(sc->scan_request, &info);
#endif
    }

    sc->scan_request = NULL;

    return 0;
}

static inline int ssv_rx_scanu_result_ind(struct ssv_softc *sc,
                                           struct ssv_cmd *cmd,
                                           struct ipc_e2a_msg *msg)
{
    struct cfg80211_bss *bss = NULL;
    struct ieee80211_channel *chan;
    struct scanu_result_ind *ind = (struct scanu_result_ind *)msg->param;

    chan = ieee80211_get_channel(sc->wiphy, ind->center_freq);

    if (chan != NULL)
        bss = cfg80211_inform_bss_frame(sc->wiphy, chan,
                                        (struct ieee80211_mgmt *)ind->payload,
                                        ind->length, ind->rssi * 100, GFP_ATOMIC);

    if (bss != NULL)
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0))
        cfg80211_put_bss(sc->wiphy, bss);
#else
        cfg80211_put_bss(bss);
#endif

    return 0;
}

/***************************************************************************
 * Messages from ME task
 **************************************************************************/
static inline int ssv_rx_me_tkip_mic_failure_ind(struct ssv_softc *sc,
                                                  struct ssv_cmd *cmd,
                                                  struct ipc_e2a_msg *msg)
{
    struct me_tkip_mic_failure_ind *ind = (struct me_tkip_mic_failure_ind *)msg->param;
    struct ssv_vif *ssv_vif = sc->vif_table[ind->vif_idx];
    struct net_device *dev = ssv_vif->ndev;

    

    cfg80211_michael_mic_failure(dev, (u8 *)&ind->addr, (ind->ga?NL80211_KEYTYPE_GROUP:
                                 NL80211_KEYTYPE_PAIRWISE), ind->keyid,
                                 (u8 *)&ind->tsc, GFP_ATOMIC);

    return 0;
}

static inline int ssv_rx_me_tx_credits_update_ind(struct ssv_softc *sc,
                                                   struct ssv_cmd *cmd,
                                                   struct ipc_e2a_msg *msg)
{
    // struct me_tx_credits_update_ind *ind = (struct me_tx_credits_update_ind *)msg->param;
    return 0;
}

static const int chnl2bw[] = {
    [PHY_CHNL_BW_20]      = NL80211_CHAN_WIDTH_20,
    [PHY_CHNL_BW_40]      = NL80211_CHAN_WIDTH_40,
    [PHY_CHNL_BW_80]      = NL80211_CHAN_WIDTH_80,
    [PHY_CHNL_BW_160]     = NL80211_CHAN_WIDTH_160,
    [PHY_CHNL_BW_80P80]   = NL80211_CHAN_WIDTH_80P80,
};
/***************************************************************************
 * Messages from SM task
 **************************************************************************/
static inline int ssv_rx_sm_connect_ind(struct ssv_softc *sc,
                                         struct ssv_cmd *cmd,
                                         struct ipc_e2a_msg *msg)
{
    struct sm_connect_ind *ind = (struct sm_connect_ind *)msg->param;
    struct ssv_vif *ssv_vif = sc->vif_table[ind->vif_idx];
    struct net_device *dev = ssv_vif->ndev;
    const u8 *req_ie, *rsp_ie;

    #ifdef CONFIG_SSV_CHANNEL_FOLLOW
        u16 freq = -1;
        enum nl80211_channel_type ch_type = NL80211_CHAN_NO_HT;
        struct cfg80211_chan_def chandef;

        memset((void *)&chandef, 0, sizeof(struct cfg80211_chan_def));
        if(ind->chan.prim20_freq == 0) { // if connect fail, find freq from cfg80211_connect_params.
            if(NL80211_IFTYPE_STATION == SSV_VIF_TYPE(ssv_vif) && ssv_vif->ch_hint) {
                freq = ssv_vif->ch_hint;
                ch_type = ssv_vif->chtype_hint;
            }
        } else {
            freq = ind->chan.prim20_freq;
            chandef.width = chnl2bw[ind->chan.type];
            chandef.chan = ieee80211_get_channel(sc->wiphy, freq);
            chandef.center_freq1 = ind->chan.center1_freq;
            chandef.center_freq2 = ind->chan.center2_freq;        
            ch_type = cfg80211_get_chandef_type(&chandef);
            
        }
       
        SSV_LOG_DBG("[%s][%d] freq = %u, ch_type = %d\n", __FUNCTION__, __LINE__, freq, ch_type);           
        ssv_channel_switch_check(sc, freq, ch_type);
    #endif /*CONFIG_SSV_CHANNEL_FOLLOW */

    SSV_LOG_DBG("[%s][%d] status = %u\n", __FUNCTION__, __LINE__, ind->status_code);
    SSV_LOG_DBG("[%s][%d] ind->ap_idx = %u\n", __FUNCTION__, __LINE__, ind->ap_idx);


    if(ssv_cfg.sta_max_reconnect_times)
    {
        //Retry_Connection doesn't support WPA3
        if(sc->sta_reconnect_info.auth_type != NL80211_AUTHTYPE_SAE)
        {
            /*If connection failed, retry again */
            if(ind->status_code && 
                ( sc->sta_reconnect_info.retry_times < ssv_cfg.sta_max_reconnect_times))
            {
                sc->sta_reconnect_info.connect_retry = true;
                sc->sta_reconnect_info.retry_times++;
                SSV_LOG_DBG("Connection retry_times %d\n", sc->sta_reconnect_info.retry_times);
            }else{
                sc->sta_reconnect_info.connect_retry = false;
                sc->sta_reconnect_info.retry_times = 0;
            }
            up(&sc->reconnect_sem);

            if(sc->sta_reconnect_info.connect_retry)
                return 0;
        }
    }

    /* Retrieve IE addresses and lengths */
    req_ie = (const u8 *)ind->assoc_ie_buf;
    rsp_ie = req_ie + ind->assoc_req_ie_len;


    // Fill-in the AP information
    if (ind->status_code == 0)
    {
        struct ssv_sta *sta = &sc->sta_table[ind->ap_idx];
        sta->valid = true;
        sta->sta_idx = ind->ap_idx;
        sta->ch_idx = ind->ch_idx;
        sta->vif_idx = ind->vif_idx;
        sta->vlan_idx = sta->vif_idx;
        sta->qos = ind->qos;
        sta->acm = ind->acm;
        sta->ps.active = false;
        sta->aid = ind->aid;
        sta->band = ind->chan.band;
        sta->width = chnl2bw[ind->chan.type];
        sta->center_freq = ind->chan.prim20_freq;
        sta->center_freq1 = ind->chan.center1_freq;
        sta->center_freq2 = ind->chan.center2_freq;
        sta->probe_timestamp = 0;
        ssv_vif->sta.ap = sta;
        // @TODO: Get chan def in this case (add params in cfm ??)
        {
            struct cfg80211_chan_def chan_def;
            memset((void *)&chan_def, 0, sizeof(struct cfg80211_chan_def));
            chan_def.chan = ieee80211_get_channel(sc->wiphy, sta->center_freq);
            chan_def.width = sta->width;
            chan_def.center_freq1 = sta->center_freq1;
            chan_def.center_freq2 = sta->center_freq2;
            ssv_chanctx_link(ssv_vif, ind->ch_idx, &chan_def);
        }
        memcpy(sta->mac_addr, ind->bssid.array, ETH_ALEN);
        memcpy(sta->ac_param, ind->ac_param, sizeof(sta->ac_param));
        ssv_rxreord_create(sc, sta->sta_idx);
        sta->port_control=(true==ssv_vif->need_port_control)?true:false;
        // ssv_dbgfs_register_rc_stat(sc, sta);

        ssv_drv_hci_tx_active_by_sta(sc->hci_priv, sc->hci_ops, sta->sta_idx);    //active hci mng. queue
        ssv_drv_hci_tx_resume_by_sta(sc->hci_priv, sc->hci_ops, sta->sta_idx);    //resume hci mng. queue
        // WMM
        if (1 == ssv_cfg.wmm_follow_vo) {
            u32 wmm_param = 0;
            ssv_drv_hci_read_word(sc->hci_priv, sc->hci_ops, 0x0800020c, &wmm_param);
            SSV_LOG_DBG("##### VO wmm param 0x%08x\n", wmm_param);
            ssv_drv_hci_write_word(sc->hci_priv, sc->hci_ops, 0x08000200, wmm_param);
            ssv_drv_hci_write_word(sc->hci_priv, sc->hci_ops, 0x08000204, wmm_param);
            ssv_drv_hci_write_word(sc->hci_priv, sc->hci_ops, 0x08000208, wmm_param);
        }
    }

    SSV_LOG_DBG("[%s][%d] ind->status_code = %u\n", __FUNCTION__, __LINE__, ind->status_code);
    if (!ind->roamed)
    {
        SSV_LOG_DBG("[%s][%d] ind->status_code = %u\n", __FUNCTION__, __LINE__, ind->status_code);
        cfg80211_connect_result(dev, (const u8 *)ind->bssid.array, req_ie,
                                ind->assoc_req_ie_len, rsp_ie,
                                ind->assoc_rsp_ie_len, ind->status_code,
                                GFP_ATOMIC);
    }

    if (ind->status_code == 0)
    {
        netif_tx_start_all_queues(dev);
        netif_carrier_on(dev);
    }

    return 0;
}

static inline int ssv_rx_sm_disconnect_ind(struct ssv_softc *sc,
                                            struct ssv_cmd *cmd,
                                            struct ipc_e2a_msg *msg)
{
#define SSV_RC_BEACON_MISS     5000
    struct sm_disconnect_ind *ind = (struct sm_disconnect_ind *)msg->param;
    struct ssv_vif *ssv_vif = sc->vif_table[ind->vif_idx];
    struct net_device *dev = ssv_vif->ndev;
    int j = 0;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 11, 0)
    bool bConned = true;
#endif
    struct cfg80211_bss *bss;

    /* if vif is not up, ssv_close has already been called */
    if (ssv_vif->up) {
        netif_tx_stop_all_queues(dev);
        netif_carrier_off(dev);
    }

    ssv_drv_hci_tx_pause_by_sta(sc->hci_priv, sc->hci_ops, ssv_vif->sta.ap->sta_idx);     //pause hci tx queue by sta
    ssv_drv_hci_tx_inactive_by_sta(sc->hci_priv, sc->hci_ops, ssv_vif->sta.ap->sta_idx);  //inactive hci tx queue by sta

    memset(&sc->rx_bysta[ssv_vif->sta.ap->sta_idx], 0 , sizeof(struct ssv_rx));
    memset(&sc->tx_bysta[ssv_vif->sta.ap->sta_idx], 0 , sizeof(struct ssv_tx));

    ssv_rxreord_delete(sc, ssv_vif->sta.ap->sta_idx);
    // ssv_dbgfs_unregister_rc_stat(sc, ssv_vif->sta.ap);
    SSV_LOG_DBG("[%s][%d] ssv_vif->sta.ap->sta_idx = %u\n", __FUNCTION__, __LINE__, ssv_vif->sta.ap->sta_idx);
    SSV_LOG_DBG("disconnect reason code = %d\n", ind->reason_code);
    if(ind->reason_code == SSV_RC_BEACON_MISS)
        SSV_LOG_DBG("beacon miss!\n");

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4,1,0))
    bss = cfg80211_get_bss(sc->wiphy, NULL, ssv_vif->sta.ap->mac_addr, NULL, 0, IEEE80211_BSS_TYPE_ANY, IEEE80211_PRIVACY_ANY);
#else
    bss = cfg80211_get_bss(sc->wiphy, NULL, ssv_vif->sta.ap->mac_addr, NULL, 0, 0, 0);
#endif
    if (bss) {
        /* AP probably changes its channel, so remove the bss struct for that AP. */
        cfg80211_unlink_bss(sc->wiphy, bss);
        cfg80211_put_bss(sc->wiphy, bss);
    }

    ssv_vif->sta.ap->valid = false;
    //Reset station last sequence 
    for(j = 0; j < TID_MAX ; j++){
        ssv_vif->sta.ap->rx_last_seqcntl[j] = 0xFFFF;
    }
    ssv_vif->sta.ap->port_control=true;
    // memset(&ssv_vif->sta.ap->mac_addr[0], 0, ETH_ALEN);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 11, 0)
    bConned=(ssv_vif->sta.ap!=NULL)?true:false;
#endif
    ssv_vif->sta.ap = NULL;
    ssv_external_auth_disable(ssv_vif);
    ssv_chanctx_unlink(ssv_vif);
    
    /* if vif is not up, ssv_close has already been called */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 11, 0)
    if (ssv_vif->up && (bConned == true)) {
#else
    if (ssv_vif->up && (ssv_vif->wdev.sme_state == CFG80211_SME_CONNECTED)) {
#endif
        //if (!ind->ft_over_ds) {
        if (!ind->reassoc)
        {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 2, 0)
            cfg80211_disconnected(dev, ind->reason_code, NULL, 0, true, GFP_ATOMIC);
#else
            cfg80211_disconnected(dev, ind->reason_code, NULL, 0, GFP_ATOMIC);
#endif
        }
    }
    return 0;
}

void ssv_rx_sm_disconnect_ind_for_fw_reset(struct ssv_softc *sc, int idx)
{
    struct sm_disconnect_ind *ind;
    struct ipc_e2a_msg *msg;
    struct ssv_vif *vif = sc->vif_table[sc->if_mode[idx][1]];
    u16 ind_len;
   
    SSV_LOG_DBG("[%s][%d] index = %d\n", __FUNCTION__, __LINE__, idx);
    ind = kmalloc(sizeof(struct sm_disconnect_ind), GFP_KERNEL);
    memset(ind, 0, sizeof(struct sm_disconnect_ind));
    ind->reason_code = 0x3;
    ind->vif_idx = vif->drv_vif_index;
    ind->reassoc = 0x0;
    ind_len = sizeof(struct sm_disconnect_ind);

    msg = kmalloc((sizeof (struct ipc_e2a_msg) + ind_len), GFP_KERNEL); //造成memory leak?                    
    if (!msg) {
        SSV_LOG_DBG("allocate fw reset msg fail.\n");
        kfree(ind);
        return;
    }

    memcpy(msg->param, ind, ind_len);	
    msg->param_len = ind_len;

    SSV_LOG_DBG("FW RESET disconnect.\n");
    ssv_rx_sm_disconnect_ind(sc, NULL, msg);
    kfree(msg);
    kfree(ind);
}

#if defined (CONFIG_WPA_SUPPLICANT_CTL)
extern int ssv_wpas_ctl_extauth_req(void *data, int size);
#endif
static inline int ssv_rx_sm_external_auth_required_ind(struct ssv_softc *sc,
                                            struct ssv_cmd *cmd,
                                            struct ipc_e2a_msg *msg)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 17, 0) || defined(CONFIG_SUPPORT_WPA3))    
    struct sm_external_auth_required_ind *ind = (struct sm_external_auth_required_ind *)msg->param;
    struct ssv_vif *ssv_vif = sc->vif_table[ind->vif_idx];
    struct net_device *dev = ssv_vif->ndev;
    struct cfg80211_external_auth_params params;
   
    if (IEEE80211_MAX_SSID_LEN < ind->ssid.length) {
        SSV_LOG_ERR("Invild ssid len %d\n", ind->ssid.length);
        WARN_ON(1);
        return 0; 
    }
    
    memset(&params, 0, sizeof(struct cfg80211_external_auth_params));
    params.action = NL80211_EXTERNAL_AUTH_START;
    params.key_mgmt_suite = ind->akm;
    memcpy(params.bssid, ind->bssid.array, ETH_ALEN);
    params.ssid.ssid_len = ind->ssid.length;
    memcpy(params.ssid.ssid, ind->ssid.array, ind->ssid.length);

    cfg80211_external_auth_request(dev, &params, GFP_ATOMIC);
    ssv_external_auth_enable(ssv_vif);
#elif defined(CONFIG_WPA_SUPPLICANT_CTL)
    struct sm_external_auth_required_ind *ind = (struct sm_external_auth_required_ind *)msg->param;
    struct ssv_vif *ssv_vif = sc->vif_table[ind->vif_idx];
    struct ssv_external_auth_params params;

    SSV_LOG_DBG("External auth required.\n");
    if (IEEE80211_MAX_SSID_LEN < ind->ssid.length) {
        SSV_LOG_ERR("Invild ssid len %d\n", ind->ssid.length);
        WARN_ON(1);
        return 0; 
    }

    memset(&params, 0, sizeof(struct ssv_external_auth_params));
    params.action = SSV_EXTERNAL_AUTH_START;
    params.key_mgmt_suite = ind->akm;
    memcpy(params.bssid, ind->bssid.array, ETH_ALEN);
    params.ssid.ssid_len = ind->ssid.length;
    memcpy(params.ssid.ssid, ind->ssid.array, ind->ssid.length);
    ssv_wpas_ctl_extauth_req(&params, sizeof(struct ssv_external_auth_params));
    ssv_external_auth_enable(ssv_vif);
#else
    SSV_LOG_DBG("External auth required, checking kernel version or config\n");
#endif
    return 0;
}

static inline int ssv_rx_dbg_error_ind(struct ssv_softc *sc,
                                        struct ssv_cmd *cmd,
                                        struct ipc_e2a_msg *msg)
{
    return 0;
}

static inline int _ssv_rx_check_msg(struct ipc_e2a_msg *msg)
{
    if (strcmp("unknown", SSV_ID2STR(msg->id)) == 0)
    {
        SSV_LOG_ERR("no msg(%4d) handler, pass the msg!!\n", msg->id);
        //BUG_ON(1);
        return -1;
    }
    
    if(ssv_cfg.dump_fmac_msg_rx)
        SSV_LOG_DBG_DUMP("recv msg:", msg->param, msg->param_len);

    return 0;
#if 0 //for debug
    switch(msg->id)
    {
        case MM_RSSI_STATUS_IND:
        {
            // SSV_LOG_DBG("[%s] MM_RSSI_STATUS_IND\n", __FUNCTION__);
            break;
        }
        default:
        {
            SSV_LOG_DBG("[%s] recv: msg:%4d-%-24s\n", __FUNCTION__, msg->id, SSV_ID2STR(msg->id));
            break;
        }
    }
#endif
}


/*******************************************************************************
 *         Global Functions
 ******************************************************************************/
/**
 *
 */
int ssv_rx_handle_msg(struct ssv_softc *sc, struct ipc_e2a_msg *msg)
{
    if(_ssv_rx_check_msg(msg) == -1) {
        SSV_LOG_ERR("unknown msg: %x, drop!!\n", msg->id);
        return 0;
    }

    return sc->cmd_mgr.msgind(&sc->cmd_mgr, msg,
                            msg_hdlrs[MSG_T(msg->id)][MSG_I(msg->id)]);
}

