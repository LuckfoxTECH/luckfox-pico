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
 * @file fmac_mod_params.c
 * @brief Set configuration according to modules parameters
 */


/*******************************************************************************
 *         Include Files
 ******************************************************************************/
#include <linux/module.h>

#include "fmac.h"
#include "fmac_defs.h"
#include "fmac_tx.h"
#include "fmac_msg_tx.h"
#include "hal_desc.h"
#include "ssv_debug.h"


extern struct ssv6xxx_cfg ssv_cfg;
/*******************************************************************************
 *         Local Defines
 ******************************************************************************/
#define COMMON_PARAM(name, default_softmac, default_fullmac)    \
    .name = default_fullmac,
#define SOFTMAC_PARAM(name, default)
#define FULLMAC_PARAM(name, default) .name = default,



#define __MDM_MAJOR_VERSION(v) (((v) & 0xFF000000) >> 24)
#define __MDM_MINOR_VERSION(v) (((v) & 0x00FF0000) >> 16)
#define __MDM_VERSION(v)       ((__MDM_MAJOR_VERSION(v) + 2) * 10 + __MDM_MINOR_VERSION(v))
/******************************************************************************
 * CFG80211
 *****************************************************************************/
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 1, 0)
#define WLAN_EXT_CAPA3_MULTI_BSSID_SUPPORT	0

#define IEEE80211_HE_PHY_CAP8_DCM_MAX_RU_242                    0x00
#define IEEE80211_HE_PHY_CAP8_DCM_MAX_RU_484                    0x40
#define IEEE80211_HE_PHY_CAP8_DCM_MAX_RU_996                    0x80
#define IEEE80211_HE_PHY_CAP8_DCM_MAX_RU_2x996                  0xc0
#define IEEE80211_HE_PHY_CAP8_DCM_MAX_RU_MASK                   0xc0

#define IEEE80211_HE_PHY_CAP9_NOMIMAL_PKT_PADDING_0US           0x00
#define IEEE80211_HE_PHY_CAP9_NOMIMAL_PKT_PADDING_8US           0x40
#define IEEE80211_HE_PHY_CAP9_NOMIMAL_PKT_PADDING_16US          0x80
#define IEEE80211_HE_PHY_CAP9_NOMIMAL_PKT_PADDING_RESERVED      0xc0
#define IEEE80211_HE_PHY_CAP9_NOMIMAL_PKT_PADDING_MASK          0xc0
#endif // 5.1

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 0, 0)
#define cfg80211_notify_new_peer_candidate(dev, addr, ie, ie_len, sig_dbm, gfp) \
    cfg80211_notify_new_peer_candidate(dev, addr, ie, ie_len, gfp)

#define WLAN_EXT_CAPA10_TWT_REQUESTER_SUPPORT    BIT(5)
#define WLAN_EXT_CAPA10_TWT_RESPONDER_SUPPORT    BIT(6)

#endif // 5.0

struct ssv_mod_params ssv_mod_params = {
    /* common parameters */
    COMMON_PARAM(ht_on, true, true)
    COMMON_PARAM(vht_on, false, false)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
    COMMON_PARAM(mcs_map, IEEE80211_VHT_MCS_SUPPORT_0_9, IEEE80211_VHT_MCS_SUPPORT_0_7)
#else
    COMMON_PARAM(mcs_map, 2, 0)
#endif
    COMMON_PARAM(he_on, true, true)     // overwrite by ssv_cfg
    COMMON_PARAM(twt_on, false, false)
    COMMON_PARAM(ldpc_on, true, true)
    COMMON_PARAM(vht_stbc, false, false)
    COMMON_PARAM(phy_cfg, 2, 2)
    COMMON_PARAM(uapsd_timeout, 0, 0)
    COMMON_PARAM(ap_uapsd_on, false, false)
    COMMON_PARAM(sgi, true, true)
    COMMON_PARAM(use_2040, true, true)  // overwrite by ssv_cfg
    COMMON_PARAM(nss, 1, 1)
    COMMON_PARAM(bfmee, false, false)
    COMMON_PARAM(mesh, false, false)
    COMMON_PARAM(custregd, false, false)
    COMMON_PARAM(roc_dur_max, 500, 500)
    COMMON_PARAM(listen_itv, 0, 0)
    COMMON_PARAM(listen_bcmc, true, true)
    COMMON_PARAM(lp_clk_ppm, 20, 20)
    COMMON_PARAM(ps_on, false, false)
    COMMON_PARAM(tx_lft, SSV_TX_LIFETIME_MS, SSV_TX_LIFETIME_MS)   // overwrite by ssv_cfg
    // By default, only enable UAPSD for Voice queue (see IEEE80211_DEFAULT_UAPSD_QUEUE comment)
    COMMON_PARAM(uapsd_queues, 0, 0)
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 20, 0)) || defined(IEEE80211_HE_MAC_CAP2_TRS)
    COMMON_PARAM(he_mcs_map, IEEE80211_HE_MCS_SUPPORT_0_9, IEEE80211_HE_MCS_SUPPORT_0_9)
#endif
};

module_param_named(ht_on, ssv_mod_params.ht_on, bool, S_IRUGO);
MODULE_PARM_DESC(ht_on, "Enable HT (Default: 1)");

module_param_named(vht_on, ssv_mod_params.vht_on, bool, S_IRUGO);
MODULE_PARM_DESC(vht_on, "Enable VHT (Default: 1)");

module_param_named(mcs_map, ssv_mod_params.mcs_map, int, S_IRUGO);
MODULE_PARM_DESC(mcs_map,  "VHT MCS map value  0: MCS0_7, 1: MCS0_8, 2: MCS0_9"
                 " (Default: 0)");

module_param_named(he_on, ssv_mod_params.vht_on, bool, S_IRUGO);
MODULE_PARM_DESC(he_on, "Enable HE (Default: 1)");

module_param_named(ps_on, ssv_mod_params.ps_on, bool, S_IRUGO);
MODULE_PARM_DESC(ps_on, "Enable PowerSaving (Default: 1-Enabled)");

module_param_named(tx_lft, ssv_mod_params.tx_lft, int, 0644);
MODULE_PARM_DESC(tx_lft, "Tx lifetime (ms) - setting it to 0 disables retries "
                 "(Default: "__stringify(SSV_TX_LIFETIME_MS)")");

module_param_named(ldpc_on, ssv_mod_params.ldpc_on, bool, S_IRUGO);
MODULE_PARM_DESC(ldpc_on, "Enable LDPC (Default: 1)");

module_param_named(vht_stbc, ssv_mod_params.vht_stbc, bool, S_IRUGO);
MODULE_PARM_DESC(vht_stbc, "Enable VHT STBC in RX (Default: 1)");

module_param_named(phycfg, ssv_mod_params.phy_cfg, int, S_IRUGO);
MODULE_PARM_DESC(phycfg,
                 "0 <= phycfg <= 5 : RF Channel Conf (Default: 2(C0-A1-B2))");

module_param_named(uapsd_timeout, ssv_mod_params.uapsd_timeout, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(uapsd_timeout,
                 "UAPSD Timer timeout, in ms (Default: 300). If 0, UAPSD is disabled");

module_param_named(uapsd_queues, ssv_mod_params.uapsd_queues, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(uapsd_queues, "UAPSD Queues, integer value, must be seen as a bitfield\n"
                 "        Bit 0 = VO\n"
                 "        Bit 1 = VI\n"
                 "        Bit 2 = BK\n"
                 "        Bit 3 = BE\n"
                 "     -> uapsd_queues=7 will enable uapsd for VO, VI and BK queues");

module_param_named(ap_uapsd_on, ssv_mod_params.ap_uapsd_on, bool, S_IRUGO);
MODULE_PARM_DESC(ap_uapsd_on, "Enable UAPSD in AP mode (Default: 1)");

module_param_named(sgi, ssv_mod_params.sgi, bool, S_IRUGO);
MODULE_PARM_DESC(sgi, "Advertise Short Guard Interval support (Default: 1)");

module_param_named(use_2040, ssv_mod_params.use_2040, bool, S_IRUGO);
MODULE_PARM_DESC(use_2040, "Use tweaked 20-40MHz mode (Default: 1)");

module_param_named(custregd, ssv_mod_params.custregd, bool, S_IRUGO);
MODULE_PARM_DESC(custregd,
                 "Use permissive custom regulatory rules (for testing ONLY) (Default: 0)");

module_param_named(nss, ssv_mod_params.nss, int, S_IRUGO);
MODULE_PARM_DESC(nss, "1 <= nss <= 2 : Supported number of Spatial Streams (Default: 1)");

module_param_named(bfmee, ssv_mod_params.bfmee, bool, S_IRUGO);
MODULE_PARM_DESC(bfmee, "Enable Beamformee Capability (Default: 1-Enabled)");

module_param_named(mesh, ssv_mod_params.mesh, bool, S_IRUGO);
MODULE_PARM_DESC(mesh, "Enable Meshing Capability (Default: 0-Disabled)");

module_param_named(roc_dur_max, ssv_mod_params.roc_dur_max, int, S_IRUGO);
MODULE_PARM_DESC(roc_dur_max, "Maximum Remain on Channel duration");

module_param_named(listen_itv, ssv_mod_params.listen_itv, int, S_IRUGO);
MODULE_PARM_DESC(listen_itv, "Maximum listen interval");

module_param_named(listen_bcmc, ssv_mod_params.listen_bcmc, bool, S_IRUGO);
MODULE_PARM_DESC(listen_bcmc, "Wait for BC/MC traffic following DTIM beacon");

module_param_named(lp_clk_ppm, ssv_mod_params.lp_clk_ppm, int, S_IRUGO);
MODULE_PARM_DESC(lp_clk_ppm, "Low Power Clock accuracy of the local device");

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 20, 0)) || defined(IEEE80211_HE_MAC_CAP2_TRS)
module_param_named(he_mcs_map, ssv_mod_params.he_mcs_map, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(he_mcs_map,  "HE MCS map value  0: MCS0_7, 1: MCS0_9, 2: MCS0_11"
                 " (Default: 2)");
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


/*******************************************************************************
 *         Local Variables
 ******************************************************************************/
/* Regulatory rules */
static const struct ieee80211_regdomain ssv_regdom = {
    .n_reg_rules = 3,
    .alpha2 = "99",
    .reg_rules = {
        REG_RULE(2412 - 10, 2472 + 10, 40, 0, 1000, 0),
        REG_RULE(2484 - 10, 2484 + 10, 20, 0, 1000, 0),
        REG_RULE(5150 - 10, 5850 + 10, 80, 0, 1000, 0),
    }
};


struct channel_range 
{
    u32 start_ch;  //start channel
    u32 end_ch;    //end channel
};

struct custom_channel_info
{  
    u32 n_ch_rules;   //total n channel ranges 
    struct channel_range ch_list[14];  //channel list.
};

struct custom_channel_info custom_channel_info = {0};


/*******************************************************************************
 *         Local Functions
 ******************************************************************************/
/**
 * Do some sanity check
 *
 */
static int ssv_check_fw_hw_feature(struct ssv_softc *sc,
                                    struct wiphy *wiphy)
{
    u32_l sys_feat = sc->version_cfm.features;
    u32_l phy_feat = sc->version_cfm.version_phy_1;

    if (!(sys_feat & BIT(MM_FEAT_UMAC_BIT))) {
        wiphy_err(wiphy,
                  "Loading softmac firmware with fullmac driver\n");
        return -1;
    }

    if (!(sys_feat & BIT(MM_FEAT_VHT_BIT))) {
        sc->mod_params->vht_on = false;
    }

    if (!(sys_feat & BIT(MM_FEAT_HE_BIT))) {
        sc->mod_params->he_on = false;
    }

    if (!(sys_feat & BIT(MM_FEAT_TWT_BIT))) {
        sc->mod_params->twt_on = false;
    }

    if (!(sys_feat & BIT(MM_FEAT_PS_BIT))) {
        sc->mod_params->ps_on = false;
    }

    if (!(sys_feat & BIT(MM_FEAT_UAPSD_BIT))) {
        sc->mod_params->uapsd_timeout = 0;
    }

    if (sys_feat & BIT(MM_FEAT_WAPI_BIT)) {
        ssv_enable_wapi(sc);
    }

    if (sys_feat & BIT(MM_FEAT_MFP_BIT)) {
        ssv_enable_mfp(sc);
    }

#ifdef CONFIG_SSV_RADAR
    if (sys_feat & BIT(MM_FEAT_RADAR_BIT)) {
        /* Enable combination with radar detection */
        wiphy->n_iface_combinations++;
    }
#endif /* CONFIG_SSV_RADAR */

    {
        struct ieee80211_supported_band *band_2GHz = wiphy->bands[NL80211_BAND_2GHZ];
        SSV_LOG_DBG("\n\nsys_feat = 0x%08x, phy_feat = 0x%08x\n\n\n", sys_feat, phy_feat);
        if (sys_feat & BIT(MM_AMSDU_MAX_SIZE_BIT1)) {
            band_2GHz->ht_cap.cap |= IEEE80211_HT_CAP_MAX_AMSDU;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
            band_2GHz->vht_cap.cap |= IEEE80211_VHT_CAP_MAX_MPDU_LENGTH_11454;
#endif
        }
        else if (sys_feat & BIT(MM_AMSDU_MAX_SIZE_BIT0)) {
            band_2GHz->ht_cap.cap |= IEEE80211_HT_CAP_MAX_AMSDU;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
            band_2GHz->vht_cap.cap |= IEEE80211_VHT_CAP_MAX_MPDU_LENGTH_7991;
#endif
        }
        else {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
            band_2GHz->vht_cap.cap |= IEEE80211_VHT_CAP_MAX_MPDU_LENGTH_3895;
#endif
        }
    }

#ifndef CONFIG_SSV_SDM
    switch (__MDM_PHYCFG_FROM_VERS(phy_feat)) {
        case MDM_PHY_CONFIG_TRIDENT:
        case MDM_PHY_CONFIG_ELMA:
            sc->mod_params->nss = 1;
            break;
        case MDM_PHY_CONFIG_KARST:
            {
                int nss_supp = (phy_feat & MDM_NSS_MASK) >> MDM_NSS_LSB;
                if (sc->mod_params->nss > nss_supp)
                    sc->mod_params->nss = nss_supp;
            }
            break;
        default:
            WARN_ON(1);
            break;
    }
#endif /* CONFIG_SSV_SDM */
    
    /* PHY features */
    {
        u8 phy_ch_bw = ((phy_feat & MDM_CHBW_MASK) >> MDM_CHBW_LSB);

        sc->phy_ch_bw = phy_ch_bw;
        sc->mod_params->ht_on = phy_ch_bw >= PHY_CHNL_BW_40 ? true : false;
        sc->mod_params->vht_on = phy_ch_bw >= PHY_CHNL_BW_80 ? true : false;
        if (!(phy_feat & MDM_LDPCDEC_BIT))
        {
            sc->mod_params->ldpc_on = false;
        }
        if (!(phy_feat & MDM_BFMEE_BIT))
        {
            sc->mod_params->bfmee = false;
        }
    }

    if (sc->mod_params->nss < 1 || sc->mod_params->nss > 2)
        sc->mod_params->nss = 1;

    wiphy_info(wiphy, "PHY features: [NSS=%d][CHBW=%d]%s\n",
               sc->mod_params->nss,
               20 * (1 << ((phy_feat & MDM_CHBW_MASK) >> MDM_CHBW_LSB)),
               sc->mod_params->ldpc_on ? "[LDPC]" : "");
#if (SSV_VER == 691)
#define PRINT_SSV_FEAT(feat)                                   \
    (sys_feat & BIT(MM_FEAT_##feat##_BIT) ? "["#feat"]" : "")

    wiphy_info(wiphy, "FW features: %s%s%s%s%s%s%s%s%s%s\n",
               PRINT_SSV_FEAT(BCN),
               PRINT_SSV_FEAT(RADAR),
               PRINT_SSV_FEAT(PS),
               PRINT_SSV_FEAT(UAPSD),
               PRINT_SSV_FEAT(AMPDU),
               PRINT_SSV_FEAT(AMSDU),
               PRINT_SSV_FEAT(UMAC),
               PRINT_SSV_FEAT(VHT),
               PRINT_SSV_FEAT(WAPI),
               PRINT_SSV_FEAT(MFP));
#else
#define PRINT_SSV_FEAT(feat)                                   \
    (sys_feat & BIT(MM_FEAT_##feat##_BIT) ? "["#feat"]" : "")

    wiphy_info(wiphy, "FW features: %s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n",
               PRINT_SSV_FEAT(BCN),
               PRINT_SSV_FEAT(AUTOBCN),
               PRINT_SSV_FEAT(HWSCAN),
               PRINT_SSV_FEAT(CMON),
               PRINT_SSV_FEAT(MROLE),
               PRINT_SSV_FEAT(RADAR),
               PRINT_SSV_FEAT(PS),
               PRINT_SSV_FEAT(UAPSD),
               PRINT_SSV_FEAT(DPSM),
               PRINT_SSV_FEAT(AMPDU),
               PRINT_SSV_FEAT(AMSDU),
               PRINT_SSV_FEAT(CHNL_CTXT),
               PRINT_SSV_FEAT(REORD),
               PRINT_SSV_FEAT(UMAC),
               PRINT_SSV_FEAT(VHT),
               PRINT_SSV_FEAT(WAPI),
               PRINT_SSV_FEAT(MFP));
#endif
#undef PRINT_SSV_FEAT

    return 0;
}


/*******************************************************************************
 *         Global Functions
 ******************************************************************************/
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 20, 0)) || defined(IEEE80211_HE_MAC_CAP2_TRS)
static void ssv_set_ppe_threshold(struct ssv_softc *sc,
                                   struct ieee80211_sta_he_cap *he_cap)
{
    const u8_l PPE_THRES_INFO_OFT = 7;
    const u8_l PPE_THRES_INFO_BIT_LEN = 6;
    struct ppe_thres_info_tag
    {
        u8_l ppet16 : 3;
        u8_l ppet8 : 3;
    }__packed;

    struct ppe_thres_field_tag
    {
        u8_l nsts : 3;
        u8_l ru_idx_bmp : 4;
    };
    int nss = sc->mod_params->nss;
    struct ppe_thres_field_tag* ppe_thres_field = (struct ppe_thres_field_tag*) he_cap->ppe_thres;
    struct ppe_thres_info_tag ppe_thres_info = {.ppet16 = 0, //BSPK
                                                .ppet8 = 7 //None
                                               };
    u8_l* ppe_thres_info_ptr = (u8_l*) &ppe_thres_info;
    u16_l* ppe_thres_ptr = (u16_l*) he_cap->ppe_thres;
    u8_l i, j, cnt, offset;

    if (sc->mod_params->use_2040)
    {
        ppe_thres_field->ru_idx_bmp = 3;
        cnt = 2;
    }
    else
    {
        ppe_thres_field->ru_idx_bmp = 1;
        cnt = 1;
    }
    
    ppe_thres_field->nsts = nss - 1;
    for (i = 0; i < nss ; i++)
    {
        for (j = 0; j < cnt; j++){
            offset = (i * cnt + j) * PPE_THRES_INFO_BIT_LEN + PPE_THRES_INFO_OFT;
            ppe_thres_ptr = (u16_l*)&he_cap->ppe_thres[offset / 8];
            *ppe_thres_ptr |= *ppe_thres_info_ptr << (offset % 8);
        }
    }
}
#endif // LINUX_VERSION_CODE >= 4.20



void ssv_get_custom_channle_range(u32 channel_bitwise, struct custom_channel_info *p_channel_info)
{
    int n_bit = 1; 
    int ch_list_idx = 0;
    int start_ch = 0;
    int end_ch = 0;

    u32 ch = channel_bitwise;

    memset(p_channel_info, 0x0, sizeof(struct custom_channel_info));

    while(n_bit < 16)
    {
        // SSV_LOG_DBG("n_bit %d, ch %d\n", n_bit, ch);
        if(ch & 1)
        {
            if(!start_ch)
                start_ch = n_bit;
        }
        else
        {
            if(start_ch)
            {
                end_ch = n_bit -1;
                p_channel_info->ch_list[ch_list_idx].start_ch = start_ch;
                p_channel_info->ch_list[ch_list_idx].end_ch = end_ch;
                ch_list_idx++;
                start_ch = 0;
                end_ch = 0;
            }
        }
        ch = ch>>1;
        n_bit++;
    }
    p_channel_info->n_ch_rules = ch_list_idx;

}

void dump_custom_channel_info(struct custom_channel_info *p_channel_info)
{
    int i = 0;
    SSV_LOG_DBG("n_ch_rules %d\n", p_channel_info->n_ch_rules);
    for(i=0; i<p_channel_info->n_ch_rules; i++)
    {
        SSV_LOG_DBG("i=%d, start_ch:end_ch (%d:%d)\n",
        i,
        p_channel_info->ch_list[i].start_ch, 
        p_channel_info->ch_list[i].end_ch);
    }

}

u32 channel_to_freq(u32 ch)
{
    u32 freq = 0;
    switch(ch)
    {
        case 1:
            freq = 2412;
            break;
        case 2:
            freq = 2417;
            break;
        case 3:
            freq = 2422;
            break;
        case 4:
            freq = 2427;
            break;
        case 5:
            freq = 2432;
            break;
        case 6:
            freq = 2437;
            break;
        case 7:
            freq = 2442;
            break;
        case 8:
            freq = 2447;
            break;
        case 9:
            freq = 2452;
            break;
        case 10:
            freq = 2457;
            break;
        case 11:
            freq = 2462;
            break;
        case 12:
            freq = 2467;
            break;
        case 13:
            freq = 2472;
            break;
        case 14:
            freq = 2484;
            break;
        default:
            break;
    }

    return freq;
}



void ssv_gen_custom_regulatory(struct custom_channel_info *p_channel_info, struct ieee80211_regdomain *ssv_regdom)
{
    int i;
    u8 alpha2[3] = "99";
    u32 start_ch;
    u32 end_ch;
    ssv_regdom->n_reg_rules = p_channel_info->n_ch_rules;
    memcpy(ssv_regdom->alpha2, alpha2, 3);
    for(i=0; i<ssv_regdom->n_reg_rules; i++ )
    {
        start_ch = p_channel_info->ch_list[i].start_ch;
        end_ch = p_channel_info->ch_list[i].end_ch;
        ssv_regdom->reg_rules[i].freq_range.start_freq_khz =  (channel_to_freq(start_ch) - 5)*1000;//MHZ_TO_KHZ
        ssv_regdom->reg_rules[i].freq_range.end_freq_khz = (channel_to_freq(end_ch) + 5)*1000;  //MHZ_TO_KHZ
        ssv_regdom->reg_rules[i].freq_range.max_bandwidth_khz = 40*1000; //MHZ_TO_KHZ
        ssv_regdom->reg_rules[i].power_rule.max_antenna_gain = 0;  //DBI_TO_MBI
        ssv_regdom->reg_rules[i].power_rule.max_eirp = 1000*100;  //DBM_TO_MBM
        ssv_regdom->reg_rules[i].flags = 0;
    }
}

void dump_custom_regulatory(struct ieee80211_regdomain *ssv_regdom)
{
    int i;

    SSV_LOG_DBG("n_reg_rules %d\n", ssv_regdom->n_reg_rules);
    // SSV_LOG_DBG_DUMP("aplha2", &ssv_regdom->alpha2, 3);
    //SSV_LOG_DBG("alpha2 %s\n", &ssv_regdom->alpha2);

    for(i=0; i<ssv_regdom->n_reg_rules; i++ )
    {
        SSV_LOG_DBG("i %d, freq_start %d, freq_end %d, bw %d, gain %d, eirp %d, flags %d\n",
        i,
        ssv_regdom->reg_rules[i].freq_range.start_freq_khz,
        ssv_regdom->reg_rules[i].freq_range.end_freq_khz,
        ssv_regdom->reg_rules[i].freq_range.max_bandwidth_khz,
        ssv_regdom->reg_rules[i].power_rule.max_antenna_gain,
        ssv_regdom->reg_rules[i].power_rule.max_eirp,
        ssv_regdom->reg_rules[i].flags);
    }

    SSV_LOG_DBG("\n");
}


int ssv_handle_dynparams(struct ssv_softc *sc, struct wiphy *wiphy)
{
    struct ieee80211_supported_band *band_2GHz = wiphy->bands[NL80211_BAND_2GHZ];
#ifndef CONFIG_SSV_SDM
    u32 mdm_phy_cfg;
#endif
    int i, ret;
    int nss;
    int mcs_map;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 20, 0)) || defined(IEEE80211_HE_MAC_CAP2_TRS)
    struct ieee80211_sta_he_cap *he_cap;
    int mcs_map_max_2ss = IEEE80211_HE_MCS_SUPPORT_0_11;
    u8 dcm_max_ru = IEEE80211_HE_PHY_CAP8_DCM_MAX_RU_242;
    u32_l phy_vers = sc->version_cfm.version_phy_2;
#endif

    struct ieee80211_regdomain *ssv_custom_regdom = NULL;
    u32 regdom_rule_len;
    u32 ssv_regdom_total_len;

    ret = ssv_check_fw_hw_feature(sc, wiphy);
    if (ret)
        return ret;

    /* FULLMAC specific parameters */
    wiphy->flags |= WIPHY_FLAG_REPORTS_OBSS;

    if (sc->mod_params->ap_uapsd_on)
        wiphy->flags |= WIPHY_FLAG_AP_UAPSD;

    if (sc->mod_params->phy_cfg < 0 || sc->mod_params->phy_cfg > 5)
        sc->mod_params->phy_cfg = 2;

    if (sc->mod_params->mcs_map < 0 || sc->mod_params->mcs_map > 2)
        sc->mod_params->mcs_map = 0;

#ifndef CONFIG_SSV_SDM
    mdm_phy_cfg = __MDM_PHYCFG_FROM_VERS(sc->version_cfm.version_phy_1);
#if 0 //No config file.
    if (mdm_phy_cfg == MDM_PHY_CONFIG_TRIDENT) {
        struct ssv_phy_conf_file phy_conf;
        // Retrieve the Trident configuration
        ssv_parse_phy_configfile(sc, SSV_PHY_CONFIG_TRD_NAME, &phy_conf);
        memcpy(&sc->phy_config, &phy_conf.trd, sizeof(phy_conf.trd));
    } else if (mdm_phy_cfg == MDM_PHY_CONFIG_ELMA) {
    } else if (mdm_phy_cfg == MDM_PHY_CONFIG_KARST) {
        struct ssv_phy_conf_file phy_conf;
        // We use the NSS parameter as is
        // Retrieve the Karst configuration
        ssv_parse_phy_configfile(sc, SSV_PHY_CONFIG_KARST_NAME, &phy_conf);

        memcpy(&sc->phy_config, &phy_conf.karst, sizeof(phy_conf.karst));
    } else {
        WARN_ON(1);
    }
#endif
#endif /* CONFIG_SSV_SDM */

    nss = sc->mod_params->nss;

    /*
     * MCS map:
     * This capabilities are filled according to the mcs_map module parameter.
     * However currently we have some limitations due to FPGA clock constraints
     * that prevent always using the range of MCS that is defined by the
     * parameter:
     *   - in RX, 2SS, we support up to MCS7
     *   - in TX, 2SS, we support up to MCS8
     */
    mcs_map = sc->mod_params->mcs_map;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
    band_2GHz->vht_cap.vht_mcs.rx_mcs_map = cpu_to_le16(0);
#endif
    //band_5GHz->vht_cap.vht_mcs.rx_mcs_map = cpu_to_le16(0);
    for (i = 0; i < nss; i++) {
        band_2GHz->ht_cap.mcs.rx_mask[i] = 0xFF;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
        band_2GHz->vht_cap.vht_mcs.rx_mcs_map |= cpu_to_le16(mcs_map << (i*2));
        //band_5GHz->vht_cap.vht_mcs.rx_mcs_map |= cpu_to_le16(mcs_map << (i*2));
        mcs_map = IEEE80211_VHT_MCS_SUPPORT_0_7;
#endif
    }
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
    for (; i < 8; i++) {
        band_2GHz->vht_cap.vht_mcs.rx_mcs_map |= cpu_to_le16(
            IEEE80211_VHT_MCS_NOT_SUPPORTED << (i*2));
        //band_5GHz->vht_cap.vht_mcs.rx_mcs_map |= cpu_to_le16(
            //IEEE80211_VHT_MCS_NOT_SUPPORTED << (i*2));
    }
    mcs_map = sc->mod_params->mcs_map;
    //band_5GHz->vht_cap.vht_mcs.tx_mcs_map = cpu_to_le16(0);
    for (i = 0; i < nss; i++) {
        //band_5GHz->vht_cap.vht_mcs.tx_mcs_map |= cpu_to_le16(mcs_map << (i*2));
        mcs_map = min_t(int, sc->mod_params->mcs_map,
                        IEEE80211_VHT_MCS_SUPPORT_0_8);
    }
#endif

    /*
     * LDPC capability:
     * This capability is filled according to the ldpc_on module parameter.
     * However currently we have some limitations due to FPGA clock constraints
     * that prevent correctly receiving more than MCS4-2SS when using LDPC.
     * We therefore disable the LDPC support if 2SS is supported.
     */
    sc->mod_params->ldpc_on = nss > 1 ? false: sc->mod_params->ldpc_on;

    /* HT capabilities */
    band_2GHz->ht_cap.cap |= 1 << IEEE80211_HT_CAP_RX_STBC_SHIFT;
    if (sc->mod_params->ldpc_on)
        band_2GHz->ht_cap.cap |= IEEE80211_HT_CAP_LDPC_CODING;
    if (sc->mod_params->use_2040) {
        band_2GHz->ht_cap.cap |= IEEE80211_HT_CAP_SUP_WIDTH_20_40;
        band_2GHz->ht_cap.mcs.rx_highest = cpu_to_le16(135 * nss);
    } else {
        band_2GHz->ht_cap.mcs.rx_highest = cpu_to_le16(65 * nss);
    }
    if (nss > 1)
        band_2GHz->ht_cap.cap |= IEEE80211_HT_CAP_TX_STBC;

    if (sc->mod_params->sgi) {
        band_2GHz->ht_cap.cap |= IEEE80211_HT_CAP_SGI_20;
        if (sc->mod_params->use_2040) {
            band_2GHz->ht_cap.cap |= IEEE80211_HT_CAP_SGI_40;
            band_2GHz->ht_cap.mcs.rx_highest = cpu_to_le16(150 * nss);
        } else
            band_2GHz->ht_cap.mcs.rx_highest = cpu_to_le16(72 * nss);
    }
    if (!sc->mod_params->ht_on) {
        band_2GHz->ht_cap.cap |= IEEE80211_HT_CAP_GRN_FLD;
    }
    // SSV_LOG_DBG("--->ht_on=%d\n", sc->mod_params->ht_on);
    if (!sc->mod_params->ht_on)
        band_2GHz->ht_cap.ht_supported = false;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
    /* VHT capabilities */
    {
        #define VHT_RATE(_mcs, _nss) (((u32)(vht_base_rate[(_mcs)] * vht_mult) >> 16) * (_nss))
        u16 vht_base_rate[3] = {0x4100, 0x4E00, 0x56AB}; // VHT base rate per mcs 65, 78 and 86.66 in Q8 notation
        u16 vht_mult = 0;
        u8 vht_mcs = (u8)IEEE80211_VHT_MCS_SUPPORT_0_9; //IEEE80211_VHT_MCS_SUPPORT_0_9(2)
        u8 mcs_map_tx = 0;
        u8 mcs_map_rx = 0;
        bool vht_on = sc->mod_params->vht_on;

        if (vht_on)
            vht_mult = 0x0480; // x4.5 in Q8 notation
        else if (ssv_mod_params.ht_on)
            vht_mult = 0x0214; // x2.07 in Q8 notation
        else
            vht_mult = 0x0100; // x1 in Q8 notation

        if (sc->mod_params->ldpc_on)
            band_2GHz->vht_cap.cap |= IEEE80211_VHT_CAP_RXLDPC;
        if (nss > 1)
            band_2GHz->vht_cap.cap |= IEEE80211_VHT_CAP_TXSTBC;
        if(sc->mod_params->vht_stbc)
            band_2GHz->vht_cap.cap |= IEEE80211_VHT_CAP_RXSTBC_1;
        if (sc->mod_params->bfmee)
            band_2GHz->vht_cap.cap |= IEEE80211_VHT_CAP_SU_BEAMFORMEE_CAPABLE;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 14, 0)
        band_2GHz->vht_cap.cap |= (nss - 1 ) << IEEE80211_VHT_CAP_SOUNDING_DIMENSIONS_SHIFT;
#else
        band_2GHz->vht_cap.cap |= (nss - 1 ) << 16;
#endif
        band_2GHz->vht_cap.cap |= (7 << IEEE80211_VHT_CAP_MAX_A_MPDU_LENGTH_EXPONENT_SHIFT);

        // RX/TX MCS
        if (!ssv_mod_params.ht_on)
            // MCS9 not allowed in 20MHZ
            vht_mcs = min(vht_mcs, (u8)IEEE80211_VHT_MCS_SUPPORT_0_8);

        mcs_map_rx = vht_mcs;
        mcs_map_tx = vht_mcs;
        // 1 SS
        band_2GHz->vht_cap.vht_mcs.rx_mcs_map = mcs_map_rx;
        band_2GHz->vht_cap.vht_mcs.tx_mcs_map = mcs_map_tx;
        // 2 SS and above
        if (nss > 1)
        {
            if (vht_on)
            {
                // limit rate @80MHz because of FPGA platform limitation
                mcs_map_rx = IEEE80211_VHT_MCS_SUPPORT_0_7;
                mcs_map_tx = min(mcs_map_tx, (u8)IEEE80211_VHT_MCS_SUPPORT_0_8);
            }
            for (i = 1; i < nss; i++)
            {
                band_2GHz->vht_cap.vht_mcs.rx_mcs_map |= mcs_map_rx << (2 * i);
                band_2GHz->vht_cap.vht_mcs.tx_mcs_map |= mcs_map_tx << (2 * i);
            }
        }
        // Non supported NSS
        for (i = nss; i < 8; i++)
        {
            band_2GHz->vht_cap.vht_mcs.rx_mcs_map |= IEEE80211_VHT_MCS_NOT_SUPPORTED << (2 * i);
            band_2GHz->vht_cap.vht_mcs.tx_mcs_map |= IEEE80211_VHT_MCS_NOT_SUPPORTED << (2 * i);
        }

        band_2GHz->vht_cap.vht_mcs.rx_highest = VHT_RATE(mcs_map_rx, nss);
        band_2GHz->vht_cap.vht_mcs.tx_highest = VHT_RATE(mcs_map_tx, nss);

        if (!vht_on)
            band_2GHz->vht_cap.vht_supported = false;
    }/* VHT capabilities */
#endif


    /* HE capabilities */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 20, 0)) || defined(IEEE80211_HE_MAC_CAP2_TRS)
    nss = sc->mod_params->nss;

    if (!sc->mod_params->he_on) {
        band_2GHz->iftype_data = NULL;
        band_2GHz->n_iftype_data = 0;
        //band_5GHz->iftype_data = NULL;
        //band_5GHz->n_iftype_data = 0;
        return 0;
    }

    he_cap = (struct ieee80211_sta_he_cap *) &band_2GHz->iftype_data->he_cap;
    he_cap->has_he = true;

    he_cap->he_cap_elem.mac_cap_info[0] |= IEEE80211_HE_MAC_CAP0_HTC_HE;
    if(sc->mod_params->twt_on)
    {
        sc->ext_capa[9] = WLAN_EXT_CAPA10_TWT_REQUESTER_SUPPORT;
        he_cap->he_cap_elem.mac_cap_info[0] |= IEEE80211_HE_MAC_CAP0_TWT_REQ;
    }

    //he_cap->he_cap_elem.mac_cap_info[2] |= IEEE80211_HE_MAC_CAP2_ALL_ACK;
    ssv_set_ppe_threshold(sc, he_cap);
    if (sc->mod_params->use_2040) {
        he_cap->he_cap_elem.phy_cap_info[0] |=
                        IEEE80211_HE_PHY_CAP0_CHANNEL_WIDTH_SET_40MHZ_IN_2G;
        dcm_max_ru = IEEE80211_HE_PHY_CAP8_DCM_MAX_RU_484;
    }
    he_cap->he_cap_elem.phy_cap_info[0] |=
                        IEEE80211_HE_PHY_CAP0_CHANNEL_WIDTH_SET_RU_MAPPING_IN_2G;
    
    he_cap->he_cap_elem.phy_cap_info[1] |= IEEE80211_HE_PHY_CAP1_DEVICE_CLASS_A;
    if (sc->mod_params->ldpc_on) {
        he_cap->he_cap_elem.phy_cap_info[1] |= IEEE80211_HE_PHY_CAP1_LDPC_CODING_IN_PAYLOAD;
    } else {
        // If no LDPC is supported, we have to limit to MCS0_9, as LDPC is mandatory
        // for MCS 10 and 11
        sc->mod_params->he_mcs_map = min_t(int, sc->mod_params->he_mcs_map,
                                                IEEE80211_HE_MCS_SUPPORT_0_9);
    }
    he_cap->he_cap_elem.phy_cap_info[1] |= IEEE80211_HE_PHY_CAP1_HE_LTF_AND_GI_FOR_HE_PPDUS_0_8US |
                                           IEEE80211_HE_PHY_CAP1_MIDAMBLE_RX_TX_MAX_NSTS;
    he_cap->he_cap_elem.phy_cap_info[2] |= IEEE80211_HE_PHY_CAP2_MIDAMBLE_RX_TX_MAX_NSTS |  /* me_config no setting */
                                           IEEE80211_HE_PHY_CAP2_NDP_4x_LTF_AND_3_2US |
                                           IEEE80211_HE_PHY_CAP2_DOPPLER_TX |
                                           IEEE80211_HE_PHY_CAP2_DOPPLER_RX;

    he_cap->he_cap_elem.phy_cap_info[3] |= IEEE80211_HE_PHY_CAP3_DCM_MAX_CONST_TX_16_QAM;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 13, 0) /* me_config no kernel version */
    he_cap->he_cap_elem.phy_cap_info[3] |= IEEE80211_HE_PHY_CAP3_DCM_MAX_CONST_RX_16_QAM;
#else
    he_cap->he_cap_elem.phy_cap_info[3] |= IEEE80211_HE_PHY_CAP3_DCM_MAX_CONST_RX_16_QAM |
                                           IEEE80211_HE_PHY_CAP3_RX_HE_MU_PPDU_FROM_NON_AP_STA; /* me_config always setting*/
#endif
    he_cap->he_cap_elem.phy_cap_info[3] |= IEEE80211_HE_PHY_CAP3_DCM_MAX_RX_NSS_1;

    if (sc->mod_params->bfmee) {
        he_cap->he_cap_elem.phy_cap_info[4] |= IEEE80211_HE_PHY_CAP4_SU_BEAMFORMEE;
        he_cap->he_cap_elem.phy_cap_info[4] |=
                     IEEE80211_HE_PHY_CAP4_BEAMFORMEE_MAX_STS_UNDER_80MHZ_4;
    }
    
    he_cap->he_cap_elem.phy_cap_info[5] |= IEEE80211_HE_PHY_CAP5_NG16_SU_FEEDBACK;
    he_cap->he_cap_elem.phy_cap_info[6] |= IEEE80211_HE_PHY_CAP6_PARTIAL_BW_EXT_RANGE |
                                           IEEE80211_HE_PHY_CAP6_CODEBOOK_SIZE_42_SU |
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 13, 0) /* me_config no kernel version */
                                           IEEE80211_HE_PHY_CAP6_TRIG_SU_BEAMFORMER_FB |
#endif
                                           IEEE80211_HE_PHY_CAP6_TRIG_CQI_FB | 
                                           IEEE80211_HE_PHY_CAP6_PPE_THRESHOLD_PRESENT;
    he_cap->he_cap_elem.phy_cap_info[7] |= IEEE80211_HE_PHY_CAP7_HE_SU_MU_PPDU_4XLTF_AND_08_US_GI |
                                           IEEE80211_HE_PHY_CAP7_MAX_NC_1;
    he_cap->he_cap_elem.phy_cap_info[8] |= IEEE80211_HE_PHY_CAP8_HE_ER_SU_1XLTF_AND_08_US_GI |
                                           IEEE80211_HE_PHY_CAP8_20MHZ_IN_40MHZ_HE_PPDU_IN_2G |
                                           IEEE80211_HE_PHY_CAP8_HE_ER_SU_PPDU_4XLTF_AND_08_US_GI |
                                           dcm_max_ru;
    he_cap->he_cap_elem.phy_cap_info[9] |= IEEE80211_HE_PHY_CAP9_NON_TRIGGERED_CQI_FEEDBACK |
                                           IEEE80211_HE_PHY_CAP9_RX_FULL_BW_SU_USING_MU_WITH_COMP_SIGB |
                                           IEEE80211_HE_PHY_CAP9_RX_FULL_BW_SU_USING_MU_WITH_NON_COMP_SIGB |
                                           IEEE80211_HE_PHY_CAP9_NOMIMAL_PKT_PADDING_RESERVED;

    // Starting from version v31 more HE_ER_SU modulations is supported
    if (__MDM_VERSION(phy_vers) > 30) {

        he_cap->he_cap_elem.phy_cap_info[6] |= IEEE80211_HE_PHY_CAP6_PARTIAL_BW_EXT_RANGE;
        he_cap->he_cap_elem.phy_cap_info[8] |= IEEE80211_HE_PHY_CAP8_HE_ER_SU_1XLTF_AND_08_US_GI |
                                               IEEE80211_HE_PHY_CAP8_HE_ER_SU_PPDU_4XLTF_AND_08_US_GI;
    }

    mcs_map = sc->mod_params->he_mcs_map;
    memset(&he_cap->he_mcs_nss_supp, 0, sizeof(he_cap->he_mcs_nss_supp));
    for (i = 0; i < nss; i++) {
        __le16 unsup_for_ss = cpu_to_le16(IEEE80211_HE_MCS_NOT_SUPPORTED << (i*2));
        he_cap->he_mcs_nss_supp.rx_mcs_80 |= cpu_to_le16(mcs_map << (i*2));
        he_cap->he_mcs_nss_supp.rx_mcs_160 |= unsup_for_ss;
        he_cap->he_mcs_nss_supp.rx_mcs_80p80 |= unsup_for_ss;
        mcs_map = min_t(int, sc->mod_params->he_mcs_map,
                        mcs_map_max_2ss);
    }
    for (; i < 8; i++) {
        __le16 unsup_for_ss = cpu_to_le16(IEEE80211_HE_MCS_NOT_SUPPORTED << (i*2));
        he_cap->he_mcs_nss_supp.rx_mcs_80 |= unsup_for_ss;
        he_cap->he_mcs_nss_supp.rx_mcs_160 |= unsup_for_ss;
        he_cap->he_mcs_nss_supp.rx_mcs_80p80 |= unsup_for_ss;
    }
    mcs_map = sc->mod_params->he_mcs_map;
    for (i = 0; i < nss; i++) {
        __le16 unsup_for_ss = cpu_to_le16(IEEE80211_HE_MCS_NOT_SUPPORTED << (i*2));
        he_cap->he_mcs_nss_supp.tx_mcs_80 |= cpu_to_le16(mcs_map << (i*2));
        he_cap->he_mcs_nss_supp.tx_mcs_160 |= unsup_for_ss;
        he_cap->he_mcs_nss_supp.tx_mcs_80p80 |= unsup_for_ss;
        mcs_map = min_t(int, sc->mod_params->he_mcs_map,
                        mcs_map_max_2ss);
    }
    for (; i < 8; i++) {
        __le16 unsup_for_ss = cpu_to_le16(IEEE80211_HE_MCS_NOT_SUPPORTED << (i*2));
        he_cap->he_mcs_nss_supp.tx_mcs_80 |= unsup_for_ss;
        he_cap->he_mcs_nss_supp.tx_mcs_160 |= unsup_for_ss;
        he_cap->he_mcs_nss_supp.tx_mcs_80p80 |= unsup_for_ss;
    }
#endif

    if (sc->mod_params->custregd) {
        SSV_LOG_ERR(
               "\n\n%s: CAUTION: USING PERMISSIVE CUSTOM REGULATORY RULES\n",
               __func__);

        if(ssv_cfg.channel_list_2p4g)
        {
            SSV_LOG_DBG("channel_list_2p4g 0x%X\n", ssv_cfg.channel_list_2p4g);
            ssv_get_custom_channle_range(ssv_cfg.channel_list_2p4g, &custom_channel_info);
            //dump_custom_channel_info(&custom_channel_info);

            regdom_rule_len = (u32)(sizeof(struct ieee80211_reg_rule) * custom_channel_info.n_ch_rules);
            ssv_regdom_total_len = (u32)(sizeof(struct ieee80211_regdomain) + regdom_rule_len);
            // SSV_LOG_DBG("rule len %d, total_len %d\n", regdom_rule_len, ssv_regdom_total_len);
            
            ssv_custom_regdom = kzalloc(ssv_regdom_total_len +1, GFP_KERNEL);
            if (!ssv_custom_regdom) {
                SSV_LOG_ERR("%s(): Fail to alloc ssv_regdom buffer.\n", __FUNCTION__);
                return 0; 
            }

            ssv_gen_custom_regulatory(&custom_channel_info, ssv_custom_regdom);
            dump_custom_regulatory(ssv_custom_regdom);

        #if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 14, 0)
            wiphy->regulatory_flags |= REGULATORY_CUSTOM_REG;
        #else
            wiphy->flags |= WIPHY_FLAG_CUSTOM_REGULATORY;
        #endif
            wiphy_apply_custom_regulatory(wiphy, ssv_custom_regdom);

            //SSV_LOG_DBG("Free ssv_custom_regdom\n");
            kfree(ssv_custom_regdom);
        }
        else
        {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 14, 0)
            wiphy->regulatory_flags |= REGULATORY_CUSTOM_REG;
#else
            wiphy->flags |= WIPHY_FLAG_CUSTOM_REGULATORY;
#endif
            wiphy_apply_custom_regulatory(wiphy, &ssv_regdom);
        }
    }


    wiphy->max_scan_ssids = SCAN_SSID_MAX;
    wiphy->max_scan_ie_len = IEEE80211_MAX_DATA_LEN;

    /**
     * adjust caps with lower layers sc->version_cfm
     */
#ifndef CONFIG_SSV_SDM
    switch (mdm_phy_cfg) {
        case MDM_PHY_CONFIG_TRIDENT:
        {
            // SSV_DBG("%s: found Trident phy .. using phy bw tweaks\n", __func__);
            sc->use_phy_bw_tweaks = true;
            break;
        }
        case MDM_PHY_CONFIG_ELMA:
            // SSV_DBG("%s: found ELMA phy .. disabling 2.4GHz and greenfield rx\n", __func__);
            wiphy->bands[NL80211_BAND_2GHZ] = NULL;
            band_2GHz->ht_cap.cap &= ~IEEE80211_HT_CAP_GRN_FLD;
            break;
        case MDM_PHY_CONFIG_KARST:
        {
            break;
        }
        default:
            WARN_ON(1);
            break;
    }
#endif /* CONFIG_SSV_SDM */

    return 0;
}
