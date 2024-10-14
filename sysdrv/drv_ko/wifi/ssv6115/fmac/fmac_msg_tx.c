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
 * @file fmac_msg_tx.c
 * @brief TX function definitions
 */


/*******************************************************************************
 *         Include Files
 ******************************************************************************/
#include <linux/version.h>
#include <linux/types.h>
#include <linux/kthread.h>

#include "ssvdevice/rftool/ssv_efuse.h"
#include "ssv_cfg.h"
#include "lmac_types.h"
#include "lmac_mac.h"
#include "lmac_msg.h"
#include "fmac.h"
#include "fmac_utils.h"
#include "fmac_msg_tx.h"
#include "ipc_msg.h"
#include "hci/drv_hci_ops.h"
#include "ssv_debug.h"

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
extern struct ssv6xxx_cfg ssv_cfg;


/*******************************************************************************
 *         Local Variables
 ******************************************************************************/
const struct mac_addr mac_addr_bcst = {{0xFFFF, 0xFFFF, 0xFFFF}};

static const int bw2chnl[] = {
    [NL80211_CHAN_WIDTH_20_NOHT] = PHY_CHNL_BW_20,
    [NL80211_CHAN_WIDTH_20]      = PHY_CHNL_BW_20,
    [NL80211_CHAN_WIDTH_40]      = PHY_CHNL_BW_40,
    [NL80211_CHAN_WIDTH_80]      = PHY_CHNL_BW_80,
    [NL80211_CHAN_WIDTH_160]     = PHY_CHNL_BW_160,
    [NL80211_CHAN_WIDTH_80P80]   = PHY_CHNL_BW_80P80,
};

static const int chnl2bw[] = {
    [PHY_CHNL_BW_20]      = NL80211_CHAN_WIDTH_20,
    [PHY_CHNL_BW_40]      = NL80211_CHAN_WIDTH_40,
    [PHY_CHNL_BW_80]      = NL80211_CHAN_WIDTH_80,
    [PHY_CHNL_BW_160]     = NL80211_CHAN_WIDTH_160,
    [PHY_CHNL_BW_80P80]   = NL80211_CHAN_WIDTH_80P80,
};


/*******************************************************************************
 *         Local Functions
 ******************************************************************************/
/*****************************************************************************/
/*
 * Parse the ampdu density to retrieve the value in usec, according to the
 * values defined in ieee80211.h
 */
static inline u8 ssv_ampdudensity2usec(u8 ampdudensity)
{
    switch (ampdudensity) {
    case IEEE80211_HT_MPDU_DENSITY_NONE:
        return 0;
        /* 1 microsecond is our granularity */
    case IEEE80211_HT_MPDU_DENSITY_0_25:
    case IEEE80211_HT_MPDU_DENSITY_0_5:
    case IEEE80211_HT_MPDU_DENSITY_1:
        return 1;
    case IEEE80211_HT_MPDU_DENSITY_2:
        return 2;
    case IEEE80211_HT_MPDU_DENSITY_4:
        return 4;
    case IEEE80211_HT_MPDU_DENSITY_8:
        return 8;
    case IEEE80211_HT_MPDU_DENSITY_16:
        return 16;
    default:
        return 0;
    }
}

static inline bool _ssv_use_pairwise_key(struct cfg80211_crypto_settings *crypto)
{
    if ((crypto->cipher_group ==  WLAN_CIPHER_SUITE_WEP40) ||
        (crypto->cipher_group ==  WLAN_CIPHER_SUITE_WEP104))
        return false;

    return true;
}

static inline bool _ssv_is_non_blocking_msg(int id) {
    return ((id == MM_TIM_UPDATE_REQ) || (id == ME_RC_SET_RATE_REQ) ||
            (id == ME_TRAFFIC_IND_REQ));
}

static inline uint8_t _ssv_passive_scan_flag(uint32_t flags) {
	SSV_LOG_ERR("%s: return 0\n", __func__);
	return 0;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,14,0)
    if (flags & (IEEE80211_CHAN_NO_IR | IEEE80211_CHAN_RADAR))
#else
    if (flags & (IEEE80211_CHAN_RADAR))
#endif
        return SCAN_PASSIVE_BIT;
    return 0;
}

static inline void ssv_msg_free(void *pParam)
{
    struct lmac_msg *msg = NULL;
    
    msg = container_of((void *)pParam, struct lmac_msg, param);
    if (msg)
        kfree(msg);
}

/**
 ******************************************************************************
 * @brief Allocate memory for a message
 *
 * This primitive allocates memory for a message that has to be sent. The memory
 * is allocated dynamically on the heap and the length of the variable parameter
 * structure has to be provided in order to allocate the correct size.
 *
 * Several additional parameters are provided which will be preset in the message
 * and which may be used internally to choose the kind of memory to allocate.
 *
 * The memory allocated will be automatically freed by the kernel, after the
 * pointer has been sent to ke_msg_send(). If the message is not sent, it must
 * be freed explicitly with ke_msg_free().
 *
 * Allocation failure is considered critical and should not happen.
 *
 * @param[in] id        Message identifier
 * @param[in] dest_id   Destination Task Identifier
 * @param[in] src_id    Source Task Identifier
 * @param[in] param_len Size of the message parameters to be allocated
 *
 * @return Pointer to the parameter member of the ke_msg. If the parameter
 *         structure is empty, the pointer will point to the end of the message
 *         and should not be used (except to retrieve the message pointer or to
 *         send the message)
 ******************************************************************************
 */
static inline void *ssv_msg_zalloc(lmac_msg_id_t const id,
                                    lmac_task_id_t const dest_id,
                                    lmac_task_id_t const src_id,
                                    uint16_t const param_len)
{
    struct lmac_msg *msg;
    gfp_t flags;

    if (_ssv_is_non_blocking_msg(id) && in_softirq())
        flags = GFP_ATOMIC;
    else
        flags = GFP_KERNEL;

    msg = (struct lmac_msg *)kzalloc(sizeof(struct lmac_msg) + param_len,
                                     flags);
    if (msg == NULL) {
        SSV_LOG_ERR("%s: msg allocation failed\n", __func__);
        return NULL;
    }

    msg->dummy = 0;
    msg->id = id;
    msg->dest_id = dest_id;
    msg->src_id = src_id;
    msg->param_len = param_len;

    return msg->param;
}

static int ssv_send_msg(struct ssv_softc *sc, const void *msg_params,
                         int reqcfm, lmac_msg_id_t reqid, void *cfm)
{
    struct lmac_msg *msg;
    struct ssv_cmd *cmd;
    bool nonblock;
    int ret;

    msg = container_of((void *)msg_params, struct lmac_msg, param);
    
    if ((SSV_DEV_STARTED < (sizeof(sc->drv_flags) * 8)) &&
        !test_bit(SSV_DEV_STARTED, &sc->drv_flags) &&
        reqid != MM_RESET_CFM && reqid != MM_VERSION_CFM &&
        reqid != MM_START_CFM && reqid != MM_SET_IDLE_CFM &&
        reqid != ME_CONFIG_CFM && reqid != MM_SET_PS_MODE_CFM &&
        reqid != ME_CHAN_CONFIG_CFM && reqid != 0) {
        SSV_LOG_ERR("%s: bypassing (SSV_DEV_RESTARTING set) 0x%02x\n",
               __func__, reqid);
        kfree(msg);
        return -EBUSY;
    } else if (!sc->ipc_env) {
        SSV_LOG_DBG("%s: bypassing (restart must have failed)\n", __func__);
        kfree(msg);
        return -EBUSY;
    }

    nonblock = _ssv_is_non_blocking_msg(msg->id);

    cmd = kzalloc(sizeof(struct ssv_cmd), nonblock ? GFP_ATOMIC : GFP_KERNEL);
    cmd->result  = -EINTR;
    cmd->id      = msg->id;
    cmd->reqid   = reqid;
    cmd->a2e_msg = msg;
    cmd->e2a_msg = cfm;
    if (nonblock)
        cmd->flags = SSV_CMD_FLAG_NONBLOCK;
    if (reqcfm)
        cmd->flags |= SSV_CMD_FLAG_REQ_CFM;
    // SSV_LOG_DBG("[%s][%d] cmd = %p\n", __FUNCTION__, __LINE__, cmd);
    ret = sc->cmd_mgr.queue(&sc->cmd_mgr, cmd);

    if (!nonblock)
        kfree(cmd);
    else
        ret = cmd->result;

    return ret;
}


/*******************************************************************************
 *         Global Functions
 ******************************************************************************/
/******************************************************************************
 *    Control messages handling functions (SOFTMAC and  FULLMAC)
 *****************************************************************************/
int ssv_send_reset(struct ssv_softc *sc)
{
    void *void_param;


    /* RESET REQ has no parameter */
    void_param = ssv_msg_zalloc(MM_RESET_REQ, TASK_MM, DRV_TASK_ID, 0);
    if (!void_param)
        return -ENOMEM;

    return ssv_send_msg(sc, void_param, 1, MM_RESET_CFM, NULL);
}

int ssv_send_start(struct ssv_softc *sc)
{
    struct mm_start_req *start_req_param;

    /* Build the START REQ message */
    start_req_param = ssv_msg_zalloc(MM_START_REQ, TASK_MM, DRV_TASK_ID,
                                      sizeof(struct mm_start_req));
    if (!start_req_param)
        return -ENOMEM;

    /* Set parameters for the START message */
    memcpy(&start_req_param->phy_cfg, &sc->phy_config, sizeof(sc->phy_config));
    start_req_param->uapsd_timeout = (u32_l)sc->mod_params->uapsd_timeout;
    start_req_param->lp_clk_accuracy = (u16_l)sc->mod_params->lp_clk_ppm;

    /* Send the START REQ message to LMAC FW */
    return ssv_send_msg(sc, start_req_param, 1, MM_START_CFM, NULL);
}

int ssv_send_version_req(struct ssv_softc *sc, struct mm_version_cfm *cfm)
{
    void *void_param;



    /* VERSION REQ has no parameter */
    void_param = ssv_msg_zalloc(MM_VERSION_REQ, TASK_MM, DRV_TASK_ID, 0);
    if (!void_param)
        return -ENOMEM;

    return ssv_send_msg(sc, void_param, 1, MM_VERSION_CFM, cfm);
}

int ssv_send_add_if(struct ssv_softc *sc, const unsigned char *mac,
                     enum nl80211_iftype iftype, bool p2p, struct mm_add_if_cfm *cfm)
{
    struct mm_add_if_req *add_if_req_param;



    /* Build the ADD_IF_REQ message */
    add_if_req_param = ssv_msg_zalloc(MM_ADD_IF_REQ, TASK_MM, DRV_TASK_ID,
                                       sizeof(struct mm_add_if_req));
    if (!add_if_req_param)
        return -ENOMEM;

    /* Set parameters for the ADD_IF_REQ message */
    memcpy(&(add_if_req_param->addr.array[0]), mac, ETH_ALEN);
    switch (iftype) {
    case NL80211_IFTYPE_P2P_CLIENT:
        add_if_req_param->p2p = true;
        add_if_req_param->type = VIF_STA;
        break;
    case NL80211_IFTYPE_STATION:
        add_if_req_param->type = VIF_STA;
        break;

    case NL80211_IFTYPE_ADHOC:
        add_if_req_param->type = VIF_IBSS;
        break;

    case NL80211_IFTYPE_P2P_GO:
        add_if_req_param->p2p = true;
        add_if_req_param->type = VIF_AP;
        break;
    case NL80211_IFTYPE_AP:
        add_if_req_param->type = VIF_AP;
        break;
    case NL80211_IFTYPE_MESH_POINT:
        add_if_req_param->type = VIF_MESH_POINT;
        break;
    case NL80211_IFTYPE_AP_VLAN:
        ssv_msg_free((void *)add_if_req_param);
        return -1;
    case NL80211_IFTYPE_MONITOR:
        add_if_req_param->type = VIF_MONITOR;
        break;
    default:
        add_if_req_param->type = VIF_STA;
        break;
    }

    /* Send the ADD_IF_REQ message to LMAC FW */
    return ssv_send_msg(sc, add_if_req_param, 1, MM_ADD_IF_CFM, cfm);
}

int ssv_send_remove_if(struct ssv_softc *sc, u8 drv_vif_index)
{
    struct mm_remove_if_req *remove_if_req;

    SSV_LOG_DBG("[%s][%d] drv_vif_index = %u\n", __FUNCTION__, __LINE__, drv_vif_index);

    /* Build the MM_REMOVE_IF_REQ message */
    remove_if_req = ssv_msg_zalloc(MM_REMOVE_IF_REQ, TASK_MM, DRV_TASK_ID,
                                    sizeof(struct mm_remove_if_req));
    if (!remove_if_req)
        return -ENOMEM;

    /* Set parameters for the MM_REMOVE_IF_REQ message */
    remove_if_req->inst_nbr = drv_vif_index;

    /* Send the MM_REMOVE_IF_REQ message to LMAC FW */
    return ssv_send_msg(sc, remove_if_req, 1, MM_REMOVE_IF_CFM, NULL);
}

int ssv_send_set_channel(struct ssv_softc *sc, int phy_idx,
                          struct mm_set_channel_cfm *cfm)
{
    struct mm_set_channel_req *set_chnl_par;
    enum nl80211_chan_width width;
    u16 center_freq, center_freq1, center_freq2;
    s8 tx_power = 0;
    enum nl80211_band band;

    if (phy_idx >= sc->phy_cnt)
        return -ENOTSUPP;

    set_chnl_par = ssv_msg_zalloc(MM_SET_CHANNEL_REQ, TASK_MM, DRV_TASK_ID,
                                   sizeof(struct mm_set_channel_req));
    if (!set_chnl_par)
        return -ENOMEM;

    if (phy_idx == 0) {
        /* On FULLMAC only setting channel of secondary chain */
        wiphy_err(sc->wiphy, "Trying to set channel of primary chain");
        ssv_msg_free((void *)set_chnl_par);
        return 0;
    } else {
        struct ssv_sec_phy_chan *chan = &sc->sec_phy_chan;

        width = chnl2bw[chan->type];
        band  = chan->band;
        center_freq  = chan->prim20_freq;
        center_freq1 = chan->center_freq1;
        center_freq2 = chan->center_freq2;
    }

    set_chnl_par->chan.band = band;
    set_chnl_par->chan.type = bw2chnl[width];
    set_chnl_par->chan.prim20_freq = center_freq;
    set_chnl_par->chan.center1_freq = center_freq1;
    set_chnl_par->chan.center2_freq = center_freq2;
    set_chnl_par->chan.tx_power = tx_power;
    set_chnl_par->index = phy_idx;

    if (sc->use_phy_bw_tweaks) {
        /* XXX Tweak for 80MHz VHT */
        if (width > NL80211_CHAN_WIDTH_40) {
            int _offs = center_freq1 - center_freq;
            set_chnl_par->chan.type = PHY_CHNL_BW_40;
            set_chnl_par->chan.center1_freq = center_freq + 10 *
                (_offs > 0 ? 1 : -1) * (abs(_offs) > 10 ? 1 : -1);

        }
    }

    SSV_LOG_DBG("mac80211:   freq=%d(c1:%d - c2:%d)/width=%d - band=%d\n"
             "   hw(%d): prim20=%d(c1:%d - c2:%d)/ type=%d - band=%d\n",
             center_freq, center_freq1,
             center_freq2, width, band,
             phy_idx, set_chnl_par->chan.prim20_freq, set_chnl_par->chan.center1_freq,
             set_chnl_par->chan.center2_freq, set_chnl_par->chan.type, set_chnl_par->chan.band);

    /* Send the MM_SET_CHANNEL_REQ REQ message to LMAC FW */
    return ssv_send_msg(sc, set_chnl_par, 1, MM_SET_CHANNEL_CFM, cfm);
}


int ssv_send_key_add(struct ssv_softc *sc, u8 vif_idx, u8 sta_idx, bool pairwise,
                      u8 *key, u8 key_len, u8 key_idx, u8 cipher_suite,
                      struct mm_key_add_cfm *cfm)
{
    struct mm_key_add_req *key_add_req;



    /* Build the MM_KEY_ADD_REQ message */
    key_add_req = ssv_msg_zalloc(MM_KEY_ADD_REQ, TASK_MM, DRV_TASK_ID,
                                  sizeof(struct mm_key_add_req));
    if (!key_add_req)
        return -ENOMEM;

    /* Set parameters for the MM_KEY_ADD_REQ message */
    if (sta_idx != 0xFF) {
        /* Pairwise key */
        key_add_req->sta_idx = sta_idx;
    } else {
        /* Default key */
        key_add_req->sta_idx = sta_idx;
        key_add_req->key_idx = (u8_l)key_idx; /* only useful for default keys */
    }
    key_add_req->pairwise = pairwise;
    key_add_req->inst_nbr = vif_idx;
    key_add_req->key.length = key_len;
    memcpy(&(key_add_req->key.array[0]), key, key_len);

    key_add_req->cipher_suite = cipher_suite;

#if 0
    SSV_LOG_DBG("%s: sta_idx:%d key_idx:%d inst_nbr:%d cipher:%d key_len:%d\n", __func__,
             key_add_req->sta_idx, key_add_req->key_idx, key_add_req->inst_nbr,
             key_add_req->cipher_suite, key_add_req->key.length);
#if defined(CONFIG_SSV_DBG) || defined(CONFIG_DYNAMIC_DEBUG)
    print_ssv_hex_dump_bytes("key: ", DUMP_PREFIX_OFFSET, key_add_req->key.array, key_add_req->key.length);
#endif
#endif

    /* Send the MM_KEY_ADD_REQ message to LMAC FW */
    return ssv_send_msg(sc, key_add_req, 1, MM_KEY_ADD_CFM, cfm);
}

int ssv_send_key_del(struct ssv_softc *sc, uint8_t hw_key_idx)
{
    struct mm_key_del_req *key_del_req;

    /* Build the MM_KEY_DEL_REQ message */
    key_del_req = ssv_msg_zalloc(MM_KEY_DEL_REQ, TASK_MM, DRV_TASK_ID,
                                  sizeof(struct mm_key_del_req));
    if (!key_del_req)
        return -ENOMEM;

    /* Set parameters for the MM_KEY_DEL_REQ message */
    key_del_req->hw_key_idx = hw_key_idx;

    /* Send the MM_KEY_DEL_REQ message to LMAC FW */
#if 0
    ///@FIXME: Does it need cfm?
    return ssv_send_msg(sc, key_del_req, 0, 0, NULL);
#else
    return ssv_send_msg(sc, key_del_req, 1, MM_KEY_DEL_CFM, NULL);
#endif
}

int ssv_send_bcn_change(struct ssv_softc *sc, u8 vif_idx, u8 *buf,
                         u16 bcn_len, u16 tim_oft, u16 tim_len, u16 *csa_oft)
{
    struct mm_bcn_change_req *req;

    /* Build the MM_BCN_CHANGE_REQ message */
    req = ssv_msg_zalloc(MM_BCN_CHANGE_REQ, TASK_MM, DRV_TASK_ID,
                          sizeof(struct mm_bcn_change_req) + bcn_len);
    if (!req)
        return -ENOMEM;

    memcpy(req->bcn_buf, buf, bcn_len);	

    /* Set parameters for the MM_BCN_CHANGE_REQ message */
    req->bcn_len = bcn_len;
    req->tim_oft = tim_oft;
    req->tim_len = tim_len;
    req->inst_nbr = vif_idx;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 16, 0)
    if (csa_oft) {
        int i;
        for (i = 0; i < BCN_MAX_CSA_CPT; i++) {
            req->csa_oft[i] = csa_oft[i];
        }
    }
#endif /* VERSION >= 3.16.0 */

    /* Send the MM_BCN_CHANGE_REQ message to LMAC FW */
    return ssv_send_msg(sc, req, 1, MM_BCN_CHANGE_CFM, NULL);
}

int ssv_send_roc(struct ssv_softc *sc, struct ssv_vif *vif,
                  struct ieee80211_channel *chan, unsigned  int duration)
{
    struct mm_remain_on_channel_req *req;
    struct cfg80211_chan_def chandef;



#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
    /* Create channel definition structure */
    cfg80211_chandef_create(&chandef, chan, NL80211_CHAN_NO_HT);
#endif

    /* Build the MM_REMAIN_ON_CHANNEL_REQ message */
    req = ssv_msg_zalloc(MM_REMAIN_ON_CHANNEL_REQ, TASK_MM, DRV_TASK_ID,
                          sizeof(struct mm_remain_on_channel_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the MM_REMAIN_ON_CHANNEL_REQ message */
    req->op_code      = MM_ROC_OP_START;
    req->vif_index    = vif->drv_vif_index;
    req->duration_ms  = duration;
    req->chan.band = chan->band;
    req->chan.type = bw2chnl[chandef.width];
    req->chan.prim20_freq = chan->center_freq;
    req->chan.center1_freq = chandef.center_freq1;
    req->chan.center2_freq = chandef.center_freq2;
    req->chan.tx_power = chan->max_power;

    /* Send the MM_REMAIN_ON_CHANNEL_REQ message to LMAC FW */
    return ssv_send_msg(sc, req, 1, MM_REMAIN_ON_CHANNEL_CFM, NULL);
}

int ssv_send_cancel_roc(struct ssv_softc *sc)
{
    struct mm_remain_on_channel_req *req;



    /* Build the MM_REMAIN_ON_CHANNEL_REQ message */
    req = ssv_msg_zalloc(MM_REMAIN_ON_CHANNEL_REQ, TASK_MM, DRV_TASK_ID,
                          sizeof(struct mm_remain_on_channel_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the MM_REMAIN_ON_CHANNEL_REQ message */
    req->op_code = MM_ROC_OP_CANCEL;

    /* Send the MM_REMAIN_ON_CHANNEL_REQ message to LMAC FW */
    return ssv_send_msg(sc, req, 0, 0, NULL);
}

int ssv_send_set_power(struct ssv_softc *sc, u8 vif_idx, s8 pwr,
                        struct mm_set_power_cfm *cfm)
{
    struct mm_set_power_req *req;



    /* Build the MM_SET_POWER_REQ message */
    req = ssv_msg_zalloc(MM_SET_POWER_REQ, TASK_MM, DRV_TASK_ID,
                          sizeof(struct mm_set_power_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the MM_SET_POWER_REQ message */
    req->inst_nbr = vif_idx;
    req->power = pwr;

    /* Send the MM_SET_POWER_REQ message to LMAC FW */
    return ssv_send_msg(sc, req, 1, MM_SET_POWER_CFM, cfm);
}

int ssv_send_set_edca(struct ssv_softc *sc, u8 hw_queue, u32 param,
                       bool uapsd, u8 inst_nbr)
{
    struct mm_set_edca_req *set_edca_req;



    /* Build the MM_SET_EDCA_REQ message */
    set_edca_req = ssv_msg_zalloc(MM_SET_EDCA_REQ, TASK_MM, DRV_TASK_ID,
                                   sizeof(struct mm_set_edca_req));
    if (!set_edca_req)
        return -ENOMEM;

    /* Set parameters for the MM_SET_EDCA_REQ message */
    set_edca_req->ac_param = param;
    set_edca_req->uapsd = uapsd;
    set_edca_req->hw_queue = hw_queue;
    set_edca_req->inst_nbr = inst_nbr;

    /* Send the MM_SET_EDCA_REQ message to LMAC FW */
    return ssv_send_msg(sc, set_edca_req, 1, MM_SET_EDCA_CFM, NULL);
}

/******************************************************************************
 *    Control messages handling functions (FULLMAC only)
 *****************************************************************************/
#if 0 //Debug
__INLINE void _ssv_me_config_dump_cap(struct me_config_req *req)
{

    int i = 0;
    SSV_LOG_DBG("\n===== [%s][%d] =====\n", __FUNCTION__, __LINE__);
    SSV_LOG_DBG("req->phy_bw_max = %u\n", req->phy_bw_max);
    SSV_LOG_DBG("===== HT capabilities =====\n") ;
    SSV_LOG_DBG("req->ht_supp = %u\n", req->ht_supp);
    SSV_LOG_DBG("req->ht_cap.ht_capa_info = 0x%04x\n", req->ht_cap.ht_capa_info);
    SSV_LOG_DBG("req->ht_cap.a_mpdu_param = 0x%02x\n", req->ht_cap.a_mpdu_param);
    for (i = 0; i < MAX_MCS_LEN; i++)
    {
        SSV_LOG_DBG("req->ht_cap.mcs_rate[%d] = 0x%02x\n", i, req->ht_cap.mcs_rate[i]);
    }
    SSV_LOG_DBG("===== VHT capabilities =====\n");
    SSV_LOG_DBG("req->vht_supp = %u\n", req->vht_supp);
    SSV_LOG_DBG("req->vht_cap.vht_capa_info = 0x%08x\n", req->vht_cap.vht_capa_info);
    SSV_LOG_DBG("req->vht_cap.rx_mcs_map = 0x%04x\n", req->vht_cap.rx_mcs_map);
    SSV_LOG_DBG("req->vht_cap.rx_highest = 0x%04x\n", req->vht_cap.rx_highest);
    SSV_LOG_DBG("req->vht_cap.tx_mcs_map = 0x%04x\n", req->vht_cap.tx_mcs_map);
    SSV_LOG_DBG("req->vht_cap.tx_highest = 0x%04x\n", req->vht_cap.tx_highest);
    SSV_LOG_DBG("===== [%s][%d] =====\n\n", __FUNCTION__, __LINE__);
    SSV_LOG_DBG("===== HE capabilities =====\n");
    SSV_LOG_DBG("[%s][%d] req->he_supp = %u\n", __FUNCTION__, __LINE__, req->he_supp);
    for (i = 0; i < MAC_HE_MAC_CAPA_LEN; i++)
    {
        SSV_LOG_DBG("req->he_cap.mac_cap_info[%d] = 0x%02x\n", i, req->he_cap.mac_cap_info[i]);
    }
    for (i = 0; i < MAC_HE_PHY_CAPA_LEN; i++)
    {
        SSV_LOG_DBG("req->he_cap.phy_cap_info[%d] = 0x%02x\n", i, req->he_cap.phy_cap_info[i]);
    }
    SSV_LOG_DBG("[%s][%d] req->he_cap.mcs_supp.rx_mcs_80 = 0x%04x\n", __FUNCTION__, __LINE__, req->he_cap.mcs_supp.rx_mcs_80);
    SSV_LOG_DBG("[%s][%d] req->he_cap.mcs_supp.tx_mcs_80 = 0x%04x\n", __FUNCTION__, __LINE__, req->he_cap.mcs_supp.tx_mcs_80);
    SSV_LOG_DBG("[%s][%d] req->he_cap.mcs_supp.rx_mcs_160 = 0x%04x\n", __FUNCTION__, __LINE__, req->he_cap.mcs_supp.rx_mcs_160);
    SSV_LOG_DBG("[%s][%d] req->he_cap.mcs_supp.tx_mcs_160 = 0x%04x\n", __FUNCTION__, __LINE__, req->he_cap.mcs_supp.tx_mcs_160);
    SSV_LOG_DBG("[%s][%d] req->he_cap.mcs_supp.rx_mcs_80p80 = 0x%04x\n", __FUNCTION__, __LINE__, req->he_cap.mcs_supp.rx_mcs_80p80);
    SSV_LOG_DBG("[%s][%d] req->he_cap.mcs_supp.tx_mcs_80p80 = 0x%04x\n", __FUNCTION__, __LINE__, req->he_cap.mcs_supp.tx_mcs_80p80);
    for (i = 0; i < MAC_HE_PPE_THRES_MAX_LEN; i++)
    {
        SSV_LOG_DBG("req->he_cap.ppe_thres[%d] = 0x%02x\n", i, req->he_cap.ppe_thres[i]);
    }
}
#endif

__INLINE void _ssv_me_config_set_he_cap(struct ssv_softc *sc, struct me_config_req *req)
{
    #define NX_MDM_VER 32 ///@FIXME: hard-code to sync firmware version
    struct mac_hecapability *he_cap = &req->he_cap;
    int ppe_ru_cnt = 1;
    int i = 0;
    u8 he_mcs = (u8)MAC_HE_MCS_MAP_0_9; //MAC_HE_MCS_MAP_0_9(0x01)
    u8 nss = sc->mod_params->nss;
    //bool stbc = true;

    HE_MAC_CAPA_BIT_SET(he_cap, HTC_HE);
    if(sc->mod_params->twt_on)
        HE_MAC_CAPA_BIT_SET(he_cap, TWT_REQ);

    if (sc->mod_params->use_2040) {
        HE_PHY_CAPA_VAL_SET(he_cap, CHAN_WIDTH_SET, RU_MAPPING_40MHZ_IN_2G);
        HE_PHY_CAPA_VAL_SET(he_cap, DCM_MAX_BW, 40MHZ);
    }else
    {
        HE_PHY_CAPA_VAL_SET(he_cap, CHAN_WIDTH_SET, RU_MAPPING_IN_2G);
        HE_PHY_CAPA_VAL_SET(he_cap, DCM_MAX_BW, 20MHZ);
    }
    HE_PHY_CAPA_BIT_SET(he_cap, DEVICE_CLASS_A);

    if(sc->mod_params->ldpc_on)
        HE_PHY_CAPA_BIT_SET(he_cap, LDPC_CODING_IN_PAYLOAD);

    HE_PHY_CAPA_BIT_SET(he_cap, HE_SU_PPDU_1x_LTF_AND_GI_0_8US);
    HE_PHY_CAPA_VAL_SET(he_cap, MIDAMBLE_RX_MAX_NSTS, 4_STS);
    HE_PHY_CAPA_BIT_SET(he_cap, NDP_4x_LTF_AND_3_2US);
    HE_PHY_CAPA_BIT_SET(he_cap, DOPPLER_TX);
    HE_PHY_CAPA_BIT_SET(he_cap, DOPPLER_RX);

    HE_PHY_CAPA_VAL_SET(he_cap, DCM_MAX_CONST_TX, 16_QAM);
    HE_PHY_CAPA_VAL_SET(he_cap, DCM_MAX_CONST_RX, 16_QAM);
    HE_PHY_CAPA_BIT_SET(he_cap, RX_HE_MU_PPDU_FRM_NON_AP);
    // HE_PHY_CAPA_BIT_SET(he_cap, DCM_MAX_NSS_RX);
    
        // HE_PHY_CAPA_BIT_SET(he_cap, SU_BEAMFORMER);
    if (sc->mod_params->bfmee)
    {
        HE_PHY_CAPA_BIT_SET(he_cap, SU_BEAMFORMEE);
        HE_PHY_CAPA_VAL_SET(he_cap, BFMEE_MAX_STS_UNDER_80MHZ, 4);
    }
    HE_PHY_CAPA_BIT_SET(he_cap, NG16_SU_FEEDBACK);
    #if (NX_MDM_VER > 30)
    HE_PHY_CAPA_BIT_SET(he_cap, PARTIAL_BW_EXT_RANGE);
    #endif
    HE_PHY_CAPA_BIT_SET(he_cap, CODEBOOK_SIZE_42_SU);
    HE_PHY_CAPA_BIT_SET(he_cap, TRIG_SU_BEAMFORMER_FB);
    HE_PHY_CAPA_BIT_SET(he_cap, TRIG_CQI_FB);
    HE_PHY_CAPA_BIT_SET(he_cap, PPE_THRESHOLD_PRESENT);

    HE_PHY_CAPA_BIT_SET(he_cap, HE_SU_MU_PPDU_4x_LTF_AND_08_US_GI);
    HE_PHY_CAPA_VAL_SET(he_cap, MAX_NC, 1);

    #if (NX_MDM_VER > 30)
    HE_PHY_CAPA_BIT_SET(he_cap, HE_ER_SU_PPDU_4x_LTF_AND_08_US_GI);
    #endif
    HE_PHY_CAPA_BIT_SET(he_cap, 20MHZ_IN_40MHZ_HE_PPDU_IN_2G);
    #if (NX_MDM_VER > 30)
    HE_PHY_CAPA_BIT_SET(he_cap, HE_ER_SU_PPDU_4x_LTF_AND_08_US_GI);
    #endif

    HE_PHY_CAPA_BIT_SET(he_cap, NON_TRIG_CQI_FEEDBACK);
    HE_PHY_CAPA_BIT_SET(he_cap, RX_FULL_BW_SU_COMP_SIGB);
    HE_PHY_CAPA_BIT_SET(he_cap, RX_FULL_BW_SU_NON_COMP_SIGB);
    HE_PHY_CAPA_VAL_SET(he_cap, NOMINAL_PACKET_PADDING, RESERVED);

    if (!sc->mod_params->ldpc_on)
        // If no LDPC is supported, we have to limit to MCS0_9, as LDPC is mandatory
        // for MCS 10 and 11
        he_mcs = min(he_mcs, (u8)MAC_HE_MCS_MAP_0_7);

    memset(&he_cap->mcs_supp, 0, sizeof(he_cap->mcs_supp));

    he_cap->mcs_supp.rx_mcs_80 = he_mcs;
    for (i = 1; i < nss; i++) {
        uint16_t unsup_for_ss = MAC_HE_MCS_MAP_NONE << (i*2);
        he_cap->mcs_supp.rx_mcs_80 |= MAC_HE_MCS_MAP_0_7 << (i*2);
        he_cap->mcs_supp.rx_mcs_160 |= unsup_for_ss;
        he_cap->mcs_supp.rx_mcs_80p80 |= unsup_for_ss;
    }
    for (; i < 8; i++) {
        uint16_t unsup_for_ss = MAC_HE_MCS_MAP_NONE << (i*2);
        he_cap->mcs_supp.rx_mcs_80 |= unsup_for_ss;
        he_cap->mcs_supp.rx_mcs_160 |= unsup_for_ss;
        he_cap->mcs_supp.rx_mcs_80p80 |= unsup_for_ss;
    }
    he_cap->mcs_supp.tx_mcs_80 = he_mcs;
    for (i = 1; i < nss; i++) {
        uint16_t unsup_for_ss = MAC_HE_MCS_MAP_NONE << (i*2);
        he_cap->mcs_supp.tx_mcs_80 |= MAC_HE_MCS_MAP_0_7 << (i*2);
        he_cap->mcs_supp.tx_mcs_160 |= unsup_for_ss;
        he_cap->mcs_supp.tx_mcs_80p80 |= unsup_for_ss;
    }
    for (; i < 8; i++) {
        uint16_t unsup_for_ss = MAC_HE_MCS_MAP_NONE << (i*2);
        he_cap->mcs_supp.tx_mcs_80 |= unsup_for_ss;
        he_cap->mcs_supp.tx_mcs_160 |= unsup_for_ss;
        he_cap->mcs_supp.tx_mcs_80p80 |= unsup_for_ss;
    }

    // PPE threshold
    if (sc->mod_params->vht_on)
        ppe_ru_cnt = 3;
    else if (sc->mod_params->ht_on)
        ppe_ru_cnt = 2;
    else
        ppe_ru_cnt = 1;
    _ssv_co_val_set(he_cap->ppe_thres, HE_PPE_CAPA_NSTS_OFT, HE_PPE_CAPA_NSTS_WIDTH, nss - 1);
    _ssv_co_val_set(he_cap->ppe_thres, HE_PPE_CAPA_RU_INDEX_BITMAP_OFT,
                HE_PPE_CAPA_RU_INDEX_BITMAP_WIDTH, (1 << ppe_ru_cnt) - 1);

    for (i = HE_PPE_CAPA_PPE_THRES_INFO_OFT;
            i < HE_PPE_CAPA_PPE_THRES_INFO_OFT + (ppe_ru_cnt * 6 * nss);
            i += 6)
    {
        _ssv_co_val_set(he_cap->ppe_thres, i, 6, HE_PPE_CAPA_BPSK | (HE_PPE_CAPA_NONE << 3));
    }

    if(sc->mod_params->he_on)
        req->he_supp = true;
}

int ssv_send_me_config_req(struct ssv_softc *sc)
{
    struct me_config_req *req;
    struct wiphy *wiphy = sc->wiphy;
    struct ieee80211_sta_ht_cap *ht_cap = &wiphy->bands[NL80211_BAND_2GHZ]->ht_cap;
    uint8_t *ht_mcs = (uint8_t *)&ht_cap->mcs;
    int i;

    /* Build the ME_CONFIG_REQ message */
    req = ssv_msg_zalloc(ME_CONFIG_REQ, TASK_ME, DRV_TASK_ID,
                                   sizeof(struct me_config_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the ME_CONFIG_REQ message */
    req->ps_on = sc->mod_params->ps_on;
    req->tx_lft = sc->mod_params->tx_lft;
    if (ssv_mod_params.vht_on)
    {
        req->phy_bw_max = PHY_CHNL_BW_80;
    }
    else if (ssv_mod_params.ht_on)
    {
        req->phy_bw_max = PHY_CHNL_BW_40;
    }
    else
    {
        req->phy_bw_max = PHY_CHNL_BW_20;
    }
    
    { // HT capabilities
        req->ht_supp = ht_cap->ht_supported;
        req->ht_cap.ht_capa_info = cpu_to_le16(ht_cap->cap);
        req->ht_cap.a_mpdu_param = ht_cap->ampdu_factor |
                                     (ht_cap->ampdu_density <<
                                         IEEE80211_HT_AMPDU_PARM_DENSITY_SHIFT);
        for (i = 0; i < sizeof(ht_cap->mcs); i++)
            req->ht_cap.mcs_rate[i] = ht_mcs[i];
        req->ht_cap.ht_extended_capa = 0;
        req->ht_cap.tx_beamforming_capa = 0;
        req->ht_cap.asel_capa = 0;
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0) //VHT capabilities
    {
        struct ieee80211_sta_vht_cap *vht_cap = &wiphy->bands[NL80211_BAND_2GHZ]->vht_cap;

        req->vht_supp = vht_cap->vht_supported;
        memcpy(&req->vht_cap, &vht_cap->cap, sizeof(struct mac_vhtcapability));
    }
#endif

    _ssv_me_config_set_he_cap(sc, req);
    //Dump capabilites.
    // _ssv_me_config_dump_cap(req);

    //Display chip ID, HT40, and HE
    {
        const char *chip_id = efuse_get_chip_id();
        SSV_LOG_INFO("\n\033[1;33mCHIP ID: \033[1;35m%s\033[0m, \033[1;33mHT40: %s\033[0m, \033[1;33mHE(802.11ax): %s\033[0m\n"
            , chip_id
            , (req->ht_cap.ht_capa_info & IEEE80211_HT_CAP_SUP_WIDTH_20_40)?"\033[1;32mON":"\033[1;31mOFF"
            , (req->he_supp)?"\033[1;32mON":"\033[1;31mOFF");
        SSV_LOG_INFO("\033[1;33mCHIP ID: \033[1;35m%s\033[0m, \033[1;33mHT40: %s\033[0m, \033[1;33mHE(802.11ax): %s\033[0m\n"
            , chip_id
            , (req->ht_cap.ht_capa_info & IEEE80211_HT_CAP_SUP_WIDTH_20_40)?"\033[1;32mON":"\033[1;31mOFF"
            , (req->he_supp)?"\033[1;32mON":"\033[1;31mOFF");
        SSV_LOG_INFO("\033[1;33mCHIP ID: \033[1;35m%s\033[0m, \033[1;33mHT40: %s\033[0m, \033[1;33mHE(802.11ax): %s\033[0m\n\n"
            , chip_id
            , (req->ht_cap.ht_capa_info & IEEE80211_HT_CAP_SUP_WIDTH_20_40)?"\033[1;32mON":"\033[1;31mOFF"
            , (req->he_supp)?"\033[1;32mON":"\033[1;31mOFF");
    }

    /* Send the ME_CONFIG_REQ message to LMAC FW */
    return ssv_send_msg(sc, req, 1, ME_CONFIG_CFM, NULL);
}

int ssv_send_me_chan_config_req(struct ssv_softc *sc)
{
    struct me_chan_config_req *req;
    struct wiphy *wiphy = sc->wiphy;
    int i;



    /* Build the ME_CHAN_CONFIG_REQ message */
    req = ssv_msg_zalloc(ME_CHAN_CONFIG_REQ, TASK_ME, DRV_TASK_ID,
                                            sizeof(struct me_chan_config_req));
    if (!req)
        return -ENOMEM;

    req->chan2G4_cnt=  0;
    if (wiphy->bands[NL80211_BAND_2GHZ] != NULL) {
        struct ieee80211_supported_band *b = wiphy->bands[NL80211_BAND_2GHZ];
        for (i = 0; i < b->n_channels; i++) {
            req->chan2G4[req->chan2G4_cnt].flags = 0;
            if (b->channels[i].flags & IEEE80211_CHAN_DISABLED)
                req->chan2G4[req->chan2G4_cnt].flags |= SCAN_DISABLED_BIT;
            req->chan2G4[req->chan2G4_cnt].flags |= _ssv_passive_scan_flag(b->channels[i].flags);
            req->chan2G4[req->chan2G4_cnt].band = NL80211_BAND_2GHZ;
            req->chan2G4[req->chan2G4_cnt].freq = b->channels[i].center_freq;
            req->chan2G4[req->chan2G4_cnt].tx_power = b->channels[i].max_power;
            req->chan2G4_cnt++;
            if (req->chan2G4_cnt == SCAN_CHANNEL_2G4)
                break;
        }
    }

    req->chan5G_cnt = 0;
    if (wiphy->bands[NL80211_BAND_5GHZ] != NULL) {
        struct ieee80211_supported_band *b = wiphy->bands[NL80211_BAND_5GHZ];
        for (i = 0; i < b->n_channels; i++) {
            req->chan5G[req->chan5G_cnt].flags = 0;
            if (b->channels[i].flags & IEEE80211_CHAN_DISABLED)
                req->chan5G[req->chan5G_cnt].flags |= SCAN_DISABLED_BIT;
            req->chan5G[req->chan5G_cnt].flags |= _ssv_passive_scan_flag(b->channels[i].flags);
            req->chan5G[req->chan5G_cnt].band = NL80211_BAND_5GHZ;
            req->chan5G[req->chan5G_cnt].freq = b->channels[i].center_freq;
            req->chan5G[req->chan5G_cnt].tx_power = b->channels[i].max_power;
            req->chan5G_cnt++;
            if (req->chan5G_cnt == SCAN_CHANNEL_5G)
                break;
        }
    }

    /* Send the ME_CHAN_CONFIG_REQ message to LMAC FW */
    return ssv_send_msg(sc, req, 1, ME_CHAN_CONFIG_CFM, NULL);
}

int ssv_send_me_set_control_port_req(struct ssv_softc *sc, bool opened, u8 sta_idx)
{
    struct me_set_control_port_req *req;



    /* Build the ME_SET_CONTROL_PORT_REQ message */
    req = ssv_msg_zalloc(ME_SET_CONTROL_PORT_REQ, TASK_ME, DRV_TASK_ID,
                                   sizeof(struct me_set_control_port_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the ME_SET_CONTROL_PORT_REQ message */
    req->sta_idx = sta_idx;
    req->control_port_open = opened;

    /* Send the ME_SET_CONTROL_PORT_REQ message to LMAC FW */
    return ssv_send_msg(sc, req, 1, ME_SET_CONTROL_PORT_CFM, NULL);
}

int ssv_send_me_sta_add(struct ssv_softc *sc, struct station_parameters *params,
                         const u8 *mac, u8 inst_nbr, struct me_sta_add_cfm *cfm)
{
    struct me_sta_add_req *req;
    u8 *ht_mcs = (u8 *)&params->ht_capa->mcs;
    int i;



    /* Build the MM_STA_ADD_REQ message */
    req = ssv_msg_zalloc(ME_STA_ADD_REQ, TASK_ME, DRV_TASK_ID,
                                  sizeof(struct me_sta_add_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the MM_STA_ADD_REQ message */
    memcpy(&(req->mac_addr.array[0]), mac, ETH_ALEN);

    req->rate_set.length = params->supported_rates_len;
    for (i = 0; i < params->supported_rates_len; i++)
        req->rate_set.array[i] = params->supported_rates[i];

    req->flags = 0;
    if (params->ht_capa) {
        const struct ieee80211_ht_cap *ht_capa = params->ht_capa;

        req->flags |= STA_HT_CAPA;
        req->ht_cap.ht_capa_info = cpu_to_le16(ht_capa->cap_info);
        req->ht_cap.a_mpdu_param = ht_capa->ampdu_params_info;

        //this is an error handler for samsung smart phone, it sends the wrong ht_capa with assoc request frame sometimes
        //so we force to hack the content
        if(0==ht_mcs[0])
        {
            memset(ht_mcs,0,MAX_MCS_LEN);
            ht_mcs[0]=0xFF; //spactial stream 1
            ht_mcs[MAX_MCS_LEN-4]=0x01; //spactial stream
        }

        for (i = 0; i < sizeof(ht_capa->mcs); i++){
            req->ht_cap.mcs_rate[i] = ht_mcs[i];
        }

        req->ht_cap.ht_extended_capa = cpu_to_le16(ht_capa->extended_ht_cap_info);
        req->ht_cap.tx_beamforming_capa = cpu_to_le32(ht_capa->tx_BF_cap_info);
        req->ht_cap.asel_capa = ht_capa->antenna_selection_info;
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
    if (params->vht_capa) {
        const struct ieee80211_vht_cap *vht_capa = params->vht_capa;

        req->flags |= STA_VHT_CAPA;
        req->vht_cap.vht_capa_info = cpu_to_le32(vht_capa->vht_cap_info);
        req->vht_cap.rx_highest = cpu_to_le16(vht_capa->supp_mcs.rx_highest);
        req->vht_cap.rx_mcs_map = cpu_to_le16(vht_capa->supp_mcs.rx_mcs_map);
        req->vht_cap.tx_highest = cpu_to_le16(vht_capa->supp_mcs.tx_highest);
        req->vht_cap.tx_mcs_map = cpu_to_le16(vht_capa->supp_mcs.tx_mcs_map);
    }
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 20, 0)) || defined(IEEE80211_HE_MAC_CAP2_TRS)
    if (params->he_capa) {
        const struct ieee80211_he_cap_elem *he_capa = params->he_capa;
        struct ieee80211_he_mcs_nss_supp *mcs_nss_supp =
                                (struct ieee80211_he_mcs_nss_supp *)(he_capa + 1);

        req->flags |= STA_HE_CAPA;
        for (i = 0; i < ARRAY_SIZE(he_capa->mac_cap_info); i++) {
            req->he_cap.mac_cap_info[i] = he_capa->mac_cap_info[i];
        }
        for (i = 0; i < ARRAY_SIZE(he_capa->phy_cap_info); i++) {
            req->he_cap.phy_cap_info[i] = he_capa->phy_cap_info[i];
        }
        req->he_cap.mcs_supp.rx_mcs_80 = mcs_nss_supp->rx_mcs_80;
        req->he_cap.mcs_supp.tx_mcs_80 = mcs_nss_supp->tx_mcs_80;
        req->he_cap.mcs_supp.rx_mcs_160 = mcs_nss_supp->rx_mcs_160;
        req->he_cap.mcs_supp.tx_mcs_160 = mcs_nss_supp->tx_mcs_160;
        req->he_cap.mcs_supp.rx_mcs_80p80 = mcs_nss_supp->rx_mcs_80p80;
        req->he_cap.mcs_supp.tx_mcs_80p80 = mcs_nss_supp->tx_mcs_80p80;
    }
#endif

    if (params->sta_flags_set & BIT(NL80211_STA_FLAG_WME))
        req->flags |= STA_QOS_CAPA;

    if (params->sta_flags_set & BIT(NL80211_STA_FLAG_MFP))
        req->flags |= STA_MFP_CAPA;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 14, 0)
    if (params->opmode_notif_used) {
        req->flags |= STA_OPMOD_NOTIF;
        req->opmode = params->opmode_notif;
    }
#endif

    req->aid = cpu_to_le16(params->aid);
    req->uapsd_queues = params->uapsd_queues;
    req->max_sp_len = params->max_sp * 2;
    req->vif_idx = inst_nbr;

    if (params->sta_flags_set & BIT(NL80211_STA_FLAG_TDLS_PEER))
        req->tdls_sta = true;

    /* Send the ME_STA_ADD_REQ message to LMAC FW */
    return ssv_send_msg(sc, req, 1, ME_STA_ADD_CFM, cfm);
}

int ssv_send_me_sta_del(struct ssv_softc *sc, u8 sta_idx, bool tdls_sta)
{
    struct me_sta_del_req *req;



    /* Build the MM_STA_DEL_REQ message */
    req = ssv_msg_zalloc(ME_STA_DEL_REQ, TASK_ME, DRV_TASK_ID,
                          sizeof(struct me_sta_del_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the MM_STA_DEL_REQ message */
    req->sta_idx = sta_idx;
    req->tdls_sta = tdls_sta;

    /* Send the ME_STA_DEL_REQ message to LMAC FW */
    return ssv_send_msg(sc, req, 1, ME_STA_DEL_CFM, NULL);
}

int ssv_send_me_traffic_ind(struct ssv_softc *sc, u8 sta_idx, bool uapsd, u8 tx_status)
{
    struct me_traffic_ind_req *req;

    /* Build the ME_UTRAFFIC_IND_REQ message */
    req = ssv_msg_zalloc(ME_TRAFFIC_IND_REQ, TASK_ME, DRV_TASK_ID,
                          sizeof(struct me_traffic_ind_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the ME_TRAFFIC_IND_REQ message */
    req->sta_idx = sta_idx;
    req->tx_avail = tx_status;
    req->uapsd = uapsd;

    /* Send the ME_TRAFFIC_IND_REQ to UMAC FW */
    ssv_msg_to_hci(sc, (void *)req); 
    return 0;
} 

int ssv_send_me_rc_stats(struct ssv_softc *sc,
                          u8 sta_idx,
                          struct me_rc_stats_cfm *cfm)
{
    struct me_rc_stats_req *req;



    /* Build the ME_RC_STATS_REQ message */
    req = ssv_msg_zalloc(ME_RC_STATS_REQ, TASK_ME, DRV_TASK_ID,
                                  sizeof(struct me_rc_stats_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the ME_RC_STATS_REQ message */
    req->sta_idx = sta_idx;

    /* Send the ME_RC_STATS_REQ message to LMAC FW */
    return ssv_send_msg(sc, req, 1, ME_RC_STATS_CFM, cfm);
}

int ssv_send_me_rc_set_rate(struct ssv_softc *sc,
                             u8 sta_idx,
                             u16 rate_cfg)
{
    struct me_rc_set_rate_req *req;



    /* Build the ME_RC_SET_RATE_REQ message */
    req = ssv_msg_zalloc(ME_RC_SET_RATE_REQ, TASK_ME, DRV_TASK_ID,
                          sizeof(struct me_rc_set_rate_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the ME_RC_SET_RATE_REQ message */
    req->sta_idx = sta_idx;
    req->mode = RC_MODE_FIXEDRATE;
    req->fixed_rate_cfg = rate_cfg;

    /* Send the ME_RC_SET_RATE_REQ message to FW */
    return ssv_send_msg(sc, req, 0, 0, NULL);
}


int ssv_send_me_rc_set_ldpc(struct ssv_softc *sc,
                             u8 sta_idx,
                             bool is_enable)
{
    struct me_rc_set_rate_req *req;



    /* Build the ME_RC_SET_RATE_REQ message */
    req = ssv_msg_zalloc(ME_RC_SET_RATE_REQ, TASK_ME, DRV_TASK_ID,
                          sizeof(struct me_rc_set_rate_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the ME_RC_SET_RATE_REQ message */
    req->sta_idx = sta_idx;
    req->mode = RC_MODE_LDPC;
    req->fixed_rate_cfg = is_enable;

    /* Send the ME_RC_SET_RATE_REQ message to FW */
    return ssv_send_msg(sc, req, 0, 0, NULL);
}


int ssv_send_me_rc_set_ltf_gi(struct ssv_softc *sc,
                             u8 sta_idx,
                             u8 ltf_ig_type)
{
    struct me_rc_set_rate_req *req;



    /* Build the ME_RC_SET_RATE_REQ message */
    req = ssv_msg_zalloc(ME_RC_SET_RATE_REQ, TASK_ME, DRV_TASK_ID,
                          sizeof(struct me_rc_set_rate_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the ME_RC_SET_RATE_REQ message */
    req->sta_idx = sta_idx;
    req->mode = RC_MODE_LTF_GI;
    req->fixed_rate_cfg = ltf_ig_type;

    /* Send the ME_RC_SET_RATE_REQ message to FW */
    return ssv_send_msg(sc, req, 0, 0, NULL);
}

int ssv_send_me_rc_sta_reset_default(struct ssv_softc *sc,
                             u8 sta_idx)
{
    struct me_rc_set_rate_req *req;



    /* Build the ME_RC_SET_RATE_REQ message */
    req = ssv_msg_zalloc(ME_RC_SET_RATE_REQ, TASK_ME, DRV_TASK_ID,
                          sizeof(struct me_rc_set_rate_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the ME_RC_SET_RATE_REQ message */
    req->sta_idx = sta_idx;
    req->mode = RC_MODE_RESET_DEFAULT;

    /* Send the ME_RC_SET_RATE_REQ message to FW */
    return ssv_send_msg(sc, req, 0, 0, NULL);
}

int ssv_send_sm_connect_req(struct ssv_softc *sc,
                             struct ssv_vif *ssv_vif,
                             struct cfg80211_connect_params *sme,
                             struct sm_connect_cfm *cfm)
{
    struct sm_connect_req *req;
    int i, auth_type = MAC_AUTH_ALGO_OPEN;
    u32_l flags = 0;


    if (WARN_ON(sme->ie_len > MAX_SM_CONNECT_REQ_IE_LEN))
        return -EINVAL;
    
    switch (sme->auth_type) {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0))
        case NL80211_AUTHTYPE_AUTOMATIC:
#endif
        case NL80211_AUTHTYPE_OPEN_SYSTEM:
            auth_type = MAC_AUTH_ALGO_OPEN;
            break;
        case NL80211_AUTHTYPE_SHARED_KEY:
            auth_type = MAC_AUTH_ALGO_SHARED;
            break;
        case NL80211_AUTHTYPE_FT:
            auth_type = MAC_AUTH_ALGO_FT;
            break;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0))
        case NL80211_AUTHTYPE_SAE:
            auth_type = MAC_AUTH_ALGO_SAE;
            break;
#elif defined(CONFIG_WPA_SUPPLICANT_CTL)
        case SSV_NL80211_AUTHTYPE_SAE:
            if (sme->auth_type != NL80211_AUTHTYPE_AUTOMATIC)
                auth_type = MAC_AUTH_ALGO_SAE;
            else
                auth_type = MAC_AUTH_ALGO_OPEN;
            break;
#endif
        case NL80211_AUTHTYPE_NETWORK_EAP:
        default:
            SSV_LOG_DBG("%s: Cannot support auth type %d\n", __FUNCTION__, (int)sme->auth_type);
            return -EINVAL;
    }
    
    /* Build the SM_CONNECT_REQ message */
    req = ssv_msg_zalloc(SM_CONNECT_REQ, TASK_SM, DRV_TASK_ID,
                                   sizeof(struct sm_connect_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the SM_CONNECT_REQ message */
    if (sme->crypto.n_ciphers_pairwise &&
        ((sme->crypto.ciphers_pairwise[0] == WLAN_CIPHER_SUITE_WEP40) ||
         (sme->crypto.ciphers_pairwise[0] == WLAN_CIPHER_SUITE_TKIP) ||
         (sme->crypto.ciphers_pairwise[0] == WLAN_CIPHER_SUITE_WEP104)))
        flags |= DISABLE_HT;

    if (sme->crypto.control_port)
        flags |= CONTROL_PORT_HOST;

    if (sme->crypto.control_port_no_encrypt)
        flags |= CONTROL_PORT_NO_ENC;

    if (_ssv_use_pairwise_key(&sme->crypto))
        flags |= WPA_WPA2_IN_USE;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)
    if (sme->mfp == NL80211_MFP_REQUIRED)
        flags |= MFP_IN_USE;
#endif

    if (sme->crypto.control_port_ethertype)
        req->ctrl_port_ethertype = sme->crypto.control_port_ethertype;
    else
        req->ctrl_port_ethertype = ETH_P_PAE;

    if (sme->bssid)
        memcpy(&req->bssid, sme->bssid, ETH_ALEN);
    else
        req->bssid = mac_addr_bcst;
    req->vif_idx = ssv_vif->drv_vif_index;
    if (sme->channel) {
        req->chan.band = sme->channel->band;
        req->chan.freq = sme->channel->center_freq;
        req->chan.flags = _ssv_passive_scan_flag(sme->channel->flags);
    } else {
        req->chan.freq = (u16_l)-1;
    }
    for (i = 0; i < sme->ssid_len; i++)
        req->ssid.array[i] = sme->ssid[i];
    req->ssid.length = sme->ssid_len;
    req->flags = flags;
    if (sme->ie_len)
        memcpy(req->ie_buf, sme->ie, sme->ie_len);
    req->ie_len = sme->ie_len;
#if 0 //Debug
    SSV_LOG_DBG("\n\nreq->ssid = ");
    for (i = 0; i < req->ssid.length; i++)
    {
        SSV_LOG_DBG("%c", req->ssid.array[i]);
    }
    SSV_LOG_DBG("\n\n");
    SSV_LOG_DBG("req->vif_idx = %u\n", req->vif_idx);
    SSV_LOG_DBG("sme->ie_len = %u, req->ie_len = %u\n", (u16_l)sme->ie_len, (u16_l)req->ie_len);
#endif
    req->listen_interval = ssv_mod_params.listen_itv;
    req->dont_wait_bcmc = !ssv_mod_params.listen_bcmc;

    /* Set auth_type */
    req->auth_type = auth_type;
    /* Set UAPSD queues */
    req->uapsd_queues = ssv_mod_params.uapsd_queues;

    /* Send the SM_CONNECT_REQ message to LMAC FW */
    return ssv_send_msg(sc, req, 1, SM_CONNECT_CFM, cfm);
}

int ssv_send_sm_disconnect_req(struct ssv_softc *sc,
                                struct ssv_vif *ssv_vif,
                                u16 reason)
{
    struct sm_disconnect_req *req;
    u8 sta_idx=0;

    mutex_lock(&sc->cb_lock);
    if(NULL==ssv_vif->sta.ap){
        mutex_unlock(&sc->cb_lock);
        return 0;
    }
    sta_idx=ssv_vif->sta.ap->sta_idx;
    mutex_unlock(&sc->cb_lock);

    ssv_drv_hci_tx_pause_by_sta(sc->hci_priv, sc->hci_ops, sta_idx);     //pause hci tx queue by sta
    ssv_drv_hci_tx_inactive_by_sta(sc->hci_priv, sc->hci_ops, sta_idx);  //inactive hci tx queue by sta

    memset(&sc->rx_bysta[sta_idx], 0 , sizeof(struct ssv_rx));
    memset(&sc->tx_bysta[sta_idx], 0 , sizeof(struct ssv_tx));

    /* Build the SM_DISCONNECT_REQ message */
    req = ssv_msg_zalloc(SM_DISCONNECT_REQ, TASK_SM, DRV_TASK_ID,
                                   sizeof(struct sm_disconnect_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the SM_DISCONNECT_REQ message */
    req->reason_code = reason;
    req->vif_idx = ssv_vif->drv_vif_index;
    SSV_LOG_DBG("\33[31m%s():vif_idx=%d sta_idx=%d\33[0m\r\n",__FUNCTION__ ,req->vif_idx,sta_idx);
    /* Send the SM_DISCONNECT_REQ message to LMAC FW */
    //return ssv_send_msg(sc, req, 1, SM_DISCONNECT_IND, NULL);
    return ssv_send_msg(sc, req, 1, SM_DISCONNECT_CFM, NULL);
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 17, 0) || defined(CONFIG_SUPPORT_WPA3)) || defined(CONFIG_WPA_SUPPLICANT_CTL)
int ssv_send_sm_external_auth_required_rsp(struct ssv_softc *sc,
                                struct ssv_vif *ssv_vif,
                                u16 status)
{
    struct sm_external_auth_required_rsp *req;

    /* Build the SM_EXTERNAL_AUTH_REQUIRED_RSP message */
    req = ssv_msg_zalloc(SM_EXTERNAL_AUTH_REQUIRED_RSP, TASK_SM, DRV_TASK_ID,
                                   sizeof(struct sm_external_auth_required_rsp));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the SM_DISCONNECT_REQ message */
    req->status = status;
    req->vif_idx = ssv_vif->drv_vif_index;

    /* Send the SM_EXTERNAL_AUTH_REQUIRED_RSP message to LMAC FW */
    return ssv_send_msg(sc, req, 0, 0, NULL);
}
#endif

int ssv_send_apm_start_req(struct ssv_softc *sc, struct ssv_vif *vif,
                            struct cfg80211_ap_settings *settings,
                            struct apm_start_cfm *cfm, u8 **bcn_buf)
{
    struct apm_start_req *req;
    struct ssv_bcn *bcn = &vif->ap.bcn;
    u8 *buf;
    u32 flags = 0;
    const u8 *rate_ie;
    u8 rate_len = 0;
    int var_offset = offsetof(struct ieee80211_mgmt, u.beacon.variable);
    const u8 *var_pos;
    int len, i;

    // Build the beacon
    bcn->dtim = (u8)settings->dtim_period;
    buf = _ssv_build_bcn(bcn, &settings->beacon);
    if (!buf) {
        return -ENOMEM;
    }
    *bcn_buf = buf;

    /* Build the APM_START_REQ message */
    req = ssv_msg_zalloc(APM_START_REQ, TASK_APM, DRV_TASK_ID,
                                   sizeof(struct apm_start_req) + bcn->len);
    if (!req)
        return -ENOMEM;

    // Retrieve the basic rate set from the beacon buffer
    len = bcn->len - var_offset;
    var_pos = buf + var_offset;

    rate_ie = cfg80211_find_ie(WLAN_EID_SUPP_RATES, var_pos, len);
    if (rate_ie) {
        const u8 *rates = rate_ie + 2;
        for (i = 0; i < rate_ie[1]; i++) {
            if (rates[i] & 0x80)
                req->basic_rates.array[rate_len++] = rates[i];
        }
    }
    rate_ie = cfg80211_find_ie(WLAN_EID_EXT_SUPP_RATES, var_pos, len);
    if (rate_ie) {
        const u8 *rates = rate_ie + 2;
        for (i = 0; i < rate_ie[1]; i++) {
            if (rates[i] & 0x80)
                req->basic_rates.array[rate_len++] = rates[i];
        }
    }
    req->basic_rates.length = rate_len;

	memcpy(req->bcn_buf, buf, bcn->len);
    /* Set parameters for the APM_START_REQ message */
    req->vif_idx = vif->drv_vif_index;
    //req->bcn_addr = elem->dma_addr;
    req->bcn_len = bcn->len;
    req->tim_oft = bcn->head_len;
    req->tim_len = bcn->tim_len;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
    req->chan.band = settings->chandef.chan->band;
    req->chan.prim20_freq = settings->chandef.chan->center_freq;
    req->chan.flags = 0;
    req->chan.tx_power = settings->chandef.chan->max_power;
    req->chan.center1_freq = settings->chandef.center_freq1;
    req->chan.center2_freq = settings->chandef.center_freq2;
    req->chan.type = bw2chnl[settings->chandef.width];
    //Store ap setting param for fw reset use
    sc->chan_type = cfg80211_get_chandef_type(&settings->chandef); 
#else
    if (vif->bchan_setting) {
        req->chan.band = vif->chan_setting.band;
        req->chan.prim20_freq = vif->chan_setting.center_freq;
        req->chan.flags = 0;
        req->chan.tx_power = vif->chan_setting.max_power;
        if ((NL80211_CHAN_NO_HT == vif->chan_type_setting) || (NL80211_CHAN_HT20 == vif->chan_type_setting)) {
            req->chan.center1_freq = vif->chan_setting.center_freq;
            req->chan.center2_freq = 0;
            req->chan.type = PHY_CHNL_BW_20;
        } else if (NL80211_CHAN_HT40MINUS == vif->chan_type_setting) {
            req->chan.center1_freq = vif->chan_setting.center_freq - 5 * 2;
            req->chan.center2_freq = 0;
            req->chan.type = PHY_CHNL_BW_40;
        } else {
            req->chan.center1_freq = vif->chan_setting.center_freq + 5 * 2;
            req->chan.center2_freq = 0;
            req->chan.type = PHY_CHNL_BW_40;
        }
        //Store ap setting param for fw reset use
        sc->chan_type = vif->chan_type_setting;
    } else {
        BUG_ON(1);
    }
#endif
    //Store ap setting param for fw reset use
    sc->center_freq = req->chan.prim20_freq;

    req->bcn_int = settings->beacon_interval;
    if (settings->crypto.control_port)
        flags |= CONTROL_PORT_HOST;

    if (settings->crypto.control_port_no_encrypt)
        flags |= CONTROL_PORT_NO_ENC;

    if (_ssv_use_pairwise_key(&settings->crypto))
        flags |= WPA_WPA2_IN_USE;

    if (settings->crypto.control_port_ethertype)
        req->ctrl_port_ethertype = settings->crypto.control_port_ethertype;
    else
        req->ctrl_port_ethertype = ETH_P_PAE;
    req->flags = flags;

    /* Send the APM_START_REQ message to LMAC FW */
    return ssv_send_msg(sc, req, 1, APM_START_CFM, cfm);
}

int ssv_send_apm_stop_req(struct ssv_softc *sc, struct ssv_vif *vif)
{
    struct apm_stop_req *req;



    /* Build the APM_STOP_REQ message */
    req = ssv_msg_zalloc(APM_STOP_REQ, TASK_APM, DRV_TASK_ID,
                                   sizeof(struct apm_stop_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the APM_STOP_REQ message */
    req->vif_idx = vif->drv_vif_index;

    /* Send the APM_STOP_REQ message to LMAC FW */
    return ssv_send_msg(sc, req, 1, APM_STOP_CFM, NULL);
}

int ssv_send_scanu_req(struct ssv_softc *sc, struct ssv_vif *ssv_vif,
                        struct cfg80211_scan_request *param)
{
    struct scanu_start_req *req;
    int i;
    uint8_t chan_flags = 0;
	u16_l len;

	len = (param->ie == NULL) ? 0: param->ie_len;

    /* Build the SCANU_START_REQ message */
    req = ssv_msg_zalloc(SCANU_START_REQ, TASK_SCANU, DRV_TASK_ID,
                          sizeof(struct scanu_start_req) + len);
    if (!req)
        return -ENOMEM;

    /* Set parameters */
    req->vif_idx = ssv_vif->drv_vif_index;
    req->chan_cnt = (u8)min_t(int, SCAN_CHANNEL_MAX, param->n_channels);
    req->ssid_cnt = (u8)min_t(int, SCAN_SSID_MAX, param->n_ssids);
    req->bssid = mac_addr_bcst;
    req->no_cck = param->no_cck;

    if (req->ssid_cnt == 0)
        chan_flags |= SCAN_PASSIVE_BIT;
    for (i = 0; i < req->ssid_cnt; i++) {
        int j;
        for (j = 0; j < param->ssids[i].ssid_len; j++)
            req->ssid[i].array[j] = param->ssids[i].ssid[j];
        req->ssid[i].length = param->ssids[i].ssid_len;
    }

	if (param->ie){
		memcpy(req->add_ies_buf, param->ie, param->ie_len);	
		req->add_ie_len = param->ie_len;
	}
	
    for (i = 0; i < req->chan_cnt; i++) {
        struct ieee80211_channel *chan = param->channels[i];

        req->chan[i].band = chan->band;
        req->chan[i].freq = chan->center_freq;
        req->chan[i].flags = chan_flags | _ssv_passive_scan_flag(chan->flags);
        req->chan[i].tx_power = chan->max_reg_power;
    }

    req->duration = ssv_cfg.scan_period*1000; //us

    /* Send the SCANU_START_REQ message to LMAC FW */
    // sc->cmd_mgr.state = SSV_CMD_MGR_STATE_INITED;
    return ssv_send_msg(sc, req, 0, 0, NULL);
}

int ssv_send_apm_start_cac_req(struct ssv_softc *sc, struct ssv_vif *vif,
                                struct cfg80211_chan_def *chandef,
                                struct apm_start_cac_cfm *cfm)
{
    struct apm_start_cac_req *req;



    /* Build the APM_START_CAC_REQ message */
    req = ssv_msg_zalloc(APM_START_CAC_REQ, TASK_APM, DRV_TASK_ID,
                          sizeof(struct apm_start_cac_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the APM_START_CAC_REQ message */
    req->vif_idx = vif->drv_vif_index;
    req->chan.band = chandef->chan->band;
    req->chan.prim20_freq = chandef->chan->center_freq;
    req->chan.flags = 0;
    req->chan.center1_freq = chandef->center_freq1;
    req->chan.center2_freq = chandef->center_freq2;
    req->chan.type = bw2chnl[chandef->width];

    /* Send the APM_START_CAC_REQ message to LMAC FW */
    return ssv_send_msg(sc, req, 1, APM_START_CAC_CFM, cfm);
}

int ssv_send_apm_stop_cac_req(struct ssv_softc *sc, struct ssv_vif *vif)
{
    struct apm_stop_cac_req *req;



    /* Build the APM_STOP_CAC_REQ message */
    req = ssv_msg_zalloc(APM_STOP_CAC_REQ, TASK_APM, DRV_TASK_ID,
                          sizeof(struct apm_stop_cac_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the APM_STOP_CAC_REQ message */
    req->vif_idx = vif->drv_vif_index;

    /* Send the APM_STOP_CAC_REQ message to LMAC FW */
    return ssv_send_msg(sc, req, 1, APM_STOP_CAC_CFM, NULL);
}

/**********************************************************************
 *    Debug Messages
 *********************************************************************/
int ssv_send_dbg_trigger_req(struct ssv_softc *sc, char *msg)
{
    struct mm_dbg_trigger_req *req;



    /* Build the MM_DBG_TRIGGER_REQ message */
    req = ssv_msg_zalloc(MM_DBG_TRIGGER_REQ, TASK_MM, DRV_TASK_ID,
                          sizeof(struct mm_dbg_trigger_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the MM_DBG_TRIGGER_REQ message */
    memcpy(req->error, msg, sizeof(req->error));

    /* Send the MM_DBG_TRIGGER_REQ message to LMAC FW */
    return ssv_send_msg(sc, req, 0, -1, NULL);
}

int ssv_send_dbg_mem_read_req(struct ssv_softc *sc, u32 mem_addr,
                               struct dbg_mem_read_cfm *cfm)
{
    struct dbg_mem_read_req *mem_read_req;



    /* Build the DBG_MEM_READ_REQ message */
    mem_read_req = ssv_msg_zalloc(DBG_MEM_READ_REQ, TASK_DBG, DRV_TASK_ID,
                                   sizeof(struct dbg_mem_read_req));
    if (!mem_read_req)
        return -ENOMEM;

    /* Set parameters for the DBG_MEM_READ_REQ message */
    mem_read_req->memaddr = mem_addr;

    /* Send the DBG_MEM_READ_REQ message to LMAC FW */
    return ssv_send_msg(sc, mem_read_req, 1, DBG_MEM_READ_CFM, cfm);
}

int ssv_send_dbg_mem_write_req(struct ssv_softc *sc, u32 mem_addr,
                                u32 mem_data)
{
    struct dbg_mem_write_req *mem_write_req;



    /* Build the DBG_MEM_WRITE_REQ message */
    mem_write_req = ssv_msg_zalloc(DBG_MEM_WRITE_REQ, TASK_DBG, DRV_TASK_ID,
                                    sizeof(struct dbg_mem_write_req));
    if (!mem_write_req)
        return -ENOMEM;

    /* Set parameters for the DBG_MEM_WRITE_REQ message */
    mem_write_req->memaddr = mem_addr;
    mem_write_req->memdata = mem_data;

    /* Send the DBG_MEM_WRITE_REQ message to LMAC FW */
    return ssv_send_msg(sc, mem_write_req, 1, DBG_MEM_WRITE_CFM, NULL);
}

int ssv_send_dbg_set_mod_filter_req(struct ssv_softc *sc, u32 filter)
{
    struct dbg_set_mod_filter_req *set_mod_filter_req;



    /* Build the DBG_SET_MOD_FILTER_REQ message */
    set_mod_filter_req =
        ssv_msg_zalloc(DBG_SET_MOD_FILTER_REQ, TASK_DBG, DRV_TASK_ID,
                        sizeof(struct dbg_set_mod_filter_req));
    if (!set_mod_filter_req)
        return -ENOMEM;

    /* Set parameters for the DBG_SET_MOD_FILTER_REQ message */
    set_mod_filter_req->mod_filter = filter;

    /* Send the DBG_SET_MOD_FILTER_REQ message to LMAC FW */
    return ssv_send_msg(sc, set_mod_filter_req, 1, DBG_SET_MOD_FILTER_CFM, NULL);
}

int ssv_send_dbg_set_sev_filter_req(struct ssv_softc *sc, u32 filter)
{
    struct dbg_set_sev_filter_req *set_sev_filter_req;



    /* Build the DBG_SET_SEV_FILTER_REQ message */
    set_sev_filter_req =
        ssv_msg_zalloc(DBG_SET_SEV_FILTER_REQ, TASK_DBG, DRV_TASK_ID,
                        sizeof(struct dbg_set_sev_filter_req));
    if (!set_sev_filter_req)
        return -ENOMEM;

    /* Set parameters for the DBG_SET_SEV_FILTER_REQ message */
    set_sev_filter_req->sev_filter = filter;

    /* Send the DBG_SET_SEV_FILTER_REQ message to LMAC FW */
    return ssv_send_msg(sc, set_sev_filter_req, 1, DBG_SET_SEV_FILTER_CFM, NULL);
}

int ssv_send_dbg_get_sys_stat_req(struct ssv_softc *sc,
                                   struct dbg_get_sys_stat_cfm *cfm)
{
    void *req;



    /* Allocate the message */
    req = ssv_msg_zalloc(DBG_GET_SYS_STAT_REQ, TASK_DBG, DRV_TASK_ID, 0);
    if (!req)
        return -ENOMEM;

    /* Send the DBG_MEM_READ_REQ message to LMAC FW */
    return ssv_send_msg(sc, req, 1, DBG_GET_SYS_STAT_CFM, cfm);
}

int ssv_send_cfg_rssi_req(struct ssv_softc *sc, u8 vif_index, int rssi_thold, u32 rssi_hyst)
{
    struct mm_cfg_rssi_req *req;



    /* Build the MM_CFG_RSSI_REQ message */
    req = ssv_msg_zalloc(MM_CFG_RSSI_REQ, TASK_MM, DRV_TASK_ID,
                          sizeof(struct mm_cfg_rssi_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters for the MM_CFG_RSSI_REQ message */
    req->vif_index = vif_index;
    req->rssi_thold = (s8)rssi_thold;
    req->rssi_hyst = (u8)rssi_hyst;

    /* Send the MM_CFG_RSSI_REQ message to LMAC FW */
    return ssv_send_msg(sc, req, 0, 0, NULL);
}

int ssv_private_msg_to_hci(struct ssv_softc *sc, u8 *msg_buffer, u32 msg_len);
int ssv_send_priv_msg_ampdu_setting(struct ssv_softc *sc, u32 param)
{
    ST_IPC_SET_AMPDU_REQ req;
    req.priv_type = E_IPC_PRIV_MSG_TYPE_SET_AMPDU;
    req.length = sizeof(u32_l);
    req.hw_cap = param;

    ssv_private_msg_to_hci(sc, (u8*)&req, sizeof(ST_IPC_SET_AMPDU_REQ));
    return 0;
}

#ifdef CONFIG_MIFI_LOWPOWER

int ssv_send_priv_msg_mifi_setting(struct ssv_softc *sc, u32 mifi_feature_setting, u32 mifi_no_traffic_duration_setting)
{
    ST_IPC_PRIV_MSG *msg = NULL;
    ST_IPC_SET_MIFI param = {0};
    u32 msg_total_len = 0;

    msg_total_len = (u32)(sizeof(ST_IPC_PRIV_MSG)+sizeof(ST_IPC_SET_MIFI));
    msg = kzalloc(msg_total_len+1, GFP_KERNEL);

    if (!msg) {
        SSV_LOG_DBG("Fail to alloc duplicate rx msg\n");
        return 0; 
    }
    
    // set param 
    param.status = mifi_feature_setting; 
    param.no_traffic_duration = mifi_no_traffic_duration_setting;
    // set msg id
    msg->msgid = E_IPC_PRIV_MSG_TYPE_SET_MIFI_FEATURE;
    msg->msglen = sizeof(ST_IPC_SET_MIFI);
    memcpy((void *)&msg->data[0], (const void *)&param, sizeof(ST_IPC_SET_MIFI));

    if (0 > ssv_private_msg_to_hci(sc, (u8*)msg, msg_total_len)) {
        SSV_LOG_DBG("Fail to send private command\n");
    }

    kfree(msg);
	return 0;
}
#endif

int ssv_send_me_config_monitor_req(struct ssv_softc *sc, struct cfg80211_chan_def *chandef,
                                    bool chan_set, bool uf,
                                    struct me_config_monitor_cfm *cfm)
{
    struct me_config_monitor_req *req = NULL;
    struct ieee80211_channel *chan = chandef->chan;

    if (!chan)
        return -EINVAL;
    
    /* Build the ME_CHAN_CONFIG_REQ message */
    req = ssv_msg_zalloc(ME_CONFIG_MONITOR_REQ, TASK_ME, DRV_TASK_ID,
                            sizeof(struct me_config_monitor_req));

    if (!req)
        return -ENOMEM;

    req->chan.band = chan->band;
    req->chan.type = bw2chnl[chandef->width];
    req->chan.prim20_freq = chan->center_freq;
    req->chan.center1_freq = chandef->center_freq1;
    req->chan.center2_freq = chandef->center_freq2;
    req->chan.tx_power = chan->max_power;
    req->chan_set = chan_set;
    req->uf = uf;

    /* Send the ME_CHAN_CONFIG_REQ message to LMAC FW */
    return ssv_send_msg(sc, req, 1, ME_CONFIG_MONITOR_CFM, cfm);
}

int ssv_send_filter_duplicate_rx(struct ssv_softc *sc, u32 filter_duplicate_rx)
{
    ST_IPC_PRIV_MSG *msg = NULL;
    ST_IPC_FILTER_DUP_RX_PARAM param = {0};
    u32 msg_total_len = 0;

    msg_total_len = (u32)(sizeof(ST_IPC_PRIV_MSG)+sizeof(ST_IPC_FILTER_DUP_RX_PARAM));
    msg = kzalloc(msg_total_len+1, GFP_KERNEL);

    if (!msg) {
        SSV_LOG_DBG("Fail to alloc duplicate rx msg\n");
        return 0; 
    }
    
    // set param 
    param.filter_duplicate_rx = filter_duplicate_rx; 
    // set msg id
    msg->msgid = E_IPC_PRIV_MSG_TYPE_FILTER_DUP_RX_REQ;
    msg->msglen = sizeof(ST_IPC_FILTER_DUP_RX_PARAM);
    memcpy((void *)&msg->data[0], (const void *)&param, sizeof(ST_IPC_FILTER_DUP_RX_PARAM));

    if (0 > ssv_private_msg_to_hci(sc, (u8*)msg, msg_total_len)) {
        SSV_LOG_DBG("Fail to send private command\n");
    }

    kfree(msg);
	return 0;
}

int ssv_send_wmm_follow_vo(struct ssv_softc *sc, u32 wmm_follow_vo)
{
    ST_IPC_PRIV_MSG *msg = NULL;
    ST_IPC_WMM_FOLLOW_VO_PARAM param = {0};
    u32 msg_total_len = 0;

    msg_total_len = (u32)(sizeof(ST_IPC_PRIV_MSG)+sizeof(ST_IPC_WMM_FOLLOW_VO_PARAM));
    msg = kzalloc(msg_total_len+1, GFP_KERNEL);

    if (!msg) {
        SSV_LOG_DBG("Fail to wmm follow vo msg\n");
        return 0; 
    }
    
    // set param 
    param.wmm_follow_vo = wmm_follow_vo; 
    // set msg id
    msg->msgid = E_IPC_PRIV_MSG_TYPE_WMM_FOLLOW_VO_REQ;
    msg->msglen = sizeof(ST_IPC_WMM_FOLLOW_VO_PARAM);
    memcpy((void *)&msg->data[0], (const void *)&param, sizeof(ST_IPC_WMM_FOLLOW_VO_PARAM));

    if (0 > ssv_private_msg_to_hci(sc, (u8*)msg, msg_total_len)) {
        SSV_LOG_DBG("Fail to send private command\n");
    }

    kfree(msg);
	return 0;
}

int ssv_send_macaddr(struct ssv_softc *sc, u8 *mac0, u8 *mac1)
{
    ST_IPC_PRIV_MSG *msg = NULL;
    ST_IPC_MAC_ADDR_PARAM *param = NULL;
    u32 msg_total_len = 0;

    msg_total_len = (u32)(sizeof(ST_IPC_PRIV_MSG)+sizeof(ST_IPC_MAC_ADDR_PARAM));
    msg = kzalloc(msg_total_len+1, GFP_KERNEL);

    if (!msg) {
        SSV_LOG_DBG("Fail to alloc duplicate rx msg\n");
        return 0; 
    }
    
    // set msg id
    msg->msgid = E_IPC_PRIV_MSG_TYPE_MACADDR_REQ;
    msg->msglen = sizeof(ST_IPC_MAC_ADDR_PARAM);
    // set param
    param = (ST_IPC_MAC_ADDR_PARAM *)msg->data;
    memcpy(param->mac_addr0, mac0, ETH_ALEN);
    memcpy(param->mac_addr1, mac1, ETH_ALEN);

    if (0 > ssv_private_msg_to_hci(sc, (u8*)msg, msg_total_len)) {
        SSV_LOG_DBG("Fail to send private command\n");
    }

    kfree(msg);
	return 0;

}

int ssv_send_ipc_tx_use_one_hwq(struct ssv_softc *sc, u32 ac)
{
    ST_IPC_PRIV_MSG *msg = NULL;
    u32 *msg_data = NULL;
    u32 msg_total_len = 0;

    msg_total_len = (u32)(sizeof(ST_IPC_PRIV_MSG)+sizeof(u32));
    msg = kzalloc(msg_total_len+1, GFP_KERNEL);

    if (!msg) {
        SSV_LOG_DBG("Fail to set ipc tx use one hwq\n");
        return -1; 
    }

    // set msg id
    msg->msgid = E_IPC_PRIV_MSG_TYPE_IPC_TX_USE_ONE_HWQ;
    msg->msglen = sizeof(u32);
    // set param 
    msg_data = (u32 *)&msg->data[0];
    *msg_data = ac;
    //SSV_LOG_DBG("[%s][%d] *msg_data = %u\n", __FUNCTION__, __LINE__, *msg_data);


    if (0 > ssv_private_msg_to_hci(sc, (u8*)msg, msg_total_len)) {
        SSV_LOG_DBG("Fail to send private command\n");
    }

    kfree(msg);
    return 0;
}


int ssv_send_set_policy_tbl(struct ssv_softc *sc, u8 set_rts_method)
{
    ST_IPC_PRIV_MSG *msg = NULL;
    ST_IPC_SET_POLICY_TBL_PARAM param = {0};
    u32 msg_total_len = 0;

    msg_total_len = (u32)(sizeof(ST_IPC_PRIV_MSG)+sizeof(ST_IPC_SET_POLICY_TBL_PARAM));
    msg = kzalloc(msg_total_len+1, GFP_KERNEL);

    if (!msg) {
        SSV_LOG_DBG("Fail to alloc set rts method msg\n");
        return 0; 
    }
    
    // set param 
    param.set_rts_method = set_rts_method; 
    // set msg id
    msg->msgid = E_IPC_PRIV_MSG_TYPE_SET_POLICY_TBL_REQ;
    msg->msglen = sizeof(ST_IPC_SET_POLICY_TBL_PARAM);
    memcpy((void *)&msg->data[0], (const void *)&param, sizeof(ST_IPC_SET_POLICY_TBL_PARAM));

    if (0 > ssv_private_msg_to_hci(sc, (u8*)msg, msg_total_len)) {
        SSV_LOG_DBG("Fail to set_rts_method command\n");
    }

    kfree(msg);
	return 0;
}
        
int ssv_send_txq_credit_boundary(struct ssv_softc *sc)
{
    ST_IPC_PRIV_MSG *msg = NULL;
    ST_IPC_TXQ_CREDIT_BOUNDARY_PARAM *param = NULL;
    u32 msg_total_len = 0;

    msg_total_len = (u32)(sizeof(ST_IPC_PRIV_MSG)+sizeof(ST_IPC_TXQ_CREDIT_BOUNDARY_PARAM));
    msg = kzalloc(msg_total_len+1, GFP_KERNEL);

    if (!msg) {
        SSV_LOG_DBG("Fail to alloc duplicate rx msg\n");
        return 0; 
    }
    
    // set msg id
    msg->msgid = E_IPC_PRIV_MSG_TYPE_TXQ_CREDIT_BOUNDARY;
    msg->msglen = sizeof(ST_IPC_TXQ_CREDIT_BOUNDARY_PARAM);
    // set param
    param = (ST_IPC_TXQ_CREDIT_BOUNDARY_PARAM *)msg->data;
    if(ssv_cfg.txq_credit_boundary<=32)
    {
        param->boundary=ssv_cfg.txq_credit_boundary;
    }
    else
    {
        param->boundary=0xFF;
    }

    if (0 > ssv_private_msg_to_hci(sc, (u8*)msg, msg_total_len)) {
        SSV_LOG_DBG("Fail to send private command\n");
    }

    kfree(msg);
	return 0;
}

int ssv_send_io_aggr_setting(struct ssv_softc *sc, bool is_rx, bool enable)
{
    ST_IPC_PRIV_MSG *msg = NULL;
    ST_IPC_IO_AGGR_PARAM *param = NULL;
    u32 msg_total_len = 0;

    msg_total_len = (u32)(sizeof(ST_IPC_PRIV_MSG)+sizeof(ST_IPC_IO_AGGR_PARAM));
    msg = kzalloc(msg_total_len+1, GFP_KERNEL);

    if (!msg) {
        SSV_LOG_DBG("Fail to alloc duplicate rx msg\n");
        return 0; 
    }
    
    // set msg id
    msg->msgid = E_IPC_PRIV_MSG_TYPE_IO_AGGR;
    msg->msglen = sizeof(ST_IPC_IO_AGGR_PARAM);
    // set param
    param = (ST_IPC_IO_AGGR_PARAM *)msg->data;
    param->is_rx = (u32)is_rx;
    param->enable = (u32)enable;

    if (0 > ssv_private_msg_to_hci(sc, (u8*)msg, msg_total_len)) {
        SSV_LOG_DBG("Fail to send private command\n");
    }

    kfree(msg);
    return 0;
}

static u32 _check_mask(u32 value)
{
    u32 i=0;
    u8 count=0;
    for(i=0;i<32;i++)
    {
        if((value&(1<<i)))
            count++;
    }
    return count;
}
int ssv_send_mac_addr_rule_setting(struct ssv_softc *sc, u32  mac_low_mask, u32 mac_high_mask)
{
    ST_IPC_PRIV_MSG *msg = NULL;
    ST_MAC_ADDR_RULE_PARAM *param = NULL;
    u32 msg_total_len = 0;
    if((_check_mask(mac_low_mask)+_check_mask(mac_high_mask))>1)    
    {
        SSV_LOG_ERR("In mac_low_mask and mac_high_mask, just only one bit can be 1\n");
        return -1;
    }
    msg_total_len = (u32)(sizeof(ST_IPC_PRIV_MSG)+sizeof(ST_MAC_ADDR_RULE_PARAM));
    msg = kzalloc(msg_total_len+1, GFP_KERNEL);

    if (!msg) {
        SSV_LOG_DBG("Fail to alloc duplicate rx msg\n");
        return 0; 
    }
    
    // set msg id
    msg->msgid = E_IPC_PRIV_MSG_TYPE_MAC_ADDR_RULE;
    msg->msglen = sizeof(ST_MAC_ADDR_RULE_PARAM);
    // set param
    param = (ST_MAC_ADDR_RULE_PARAM *)msg->data;
    param->mac_low_mask = mac_low_mask;
    param->mac_high_mask = mac_high_mask;
    if (0 > ssv_private_msg_to_hci(sc, (u8*)msg, msg_total_len)) {
        SSV_LOG_DBG("Fail to send private command\n");
    }

    kfree(msg);
    return 0;
}

