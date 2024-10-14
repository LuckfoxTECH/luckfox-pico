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

/*
 * TX function declarations
 */


#ifndef __FMAC_MSG_TX_H__
#define __FMAC_MSG_TX_H__


/*******************************************************************************
 *         Include Files
 ******************************************************************************/
#include <linux/version.h>
#include "fmac_defs.h"
#include "fmac_utils.h"


/*******************************************************************************
 *         Defines
 ******************************************************************************/
/*
 * c.f LMAC/src/co/mac/mac_frame.h
 */
#define MAC_RSNIE_CIPHER_WEP40    0x00
#define MAC_RSNIE_CIPHER_TKIP     0x01
#define MAC_RSNIE_CIPHER_CCMP     0x02
#define MAC_RSNIE_CIPHER_WEP104   0x03
#define MAC_RSNIE_CIPHER_SMS4     0x04
#define MAC_RSNIE_CIPHER_AES_CMAC 0x05


/*******************************************************************************
 *         Enumerations
 ******************************************************************************/
enum ssv_chan_types {
    PHY_CHNL_BW_20,
    PHY_CHNL_BW_40,
    PHY_CHNL_BW_80,
    PHY_CHNL_BW_160,
    PHY_CHNL_BW_80P80,
    PHY_CHNL_BW_OTHER,
};

enum rc_mode_types 
{
    RC_MODE_FIXEDRATE = 1,
    RC_MODE_LDPC,
    RC_MODE_LTF_GI,
    RC_MODE_RESET_DEFAULT, 
};


/*******************************************************************************
 *         Structures
 ******************************************************************************/


/*******************************************************************************
 *         Variables
 ******************************************************************************/


/*******************************************************************************
 *         Functions
 ******************************************************************************/
int ssv_send_reset(struct ssv_softc *sc);
int ssv_send_start(struct ssv_softc *sc);
int ssv_send_version_req(struct ssv_softc *sc, struct mm_version_cfm *cfm);
int ssv_send_add_if(struct ssv_softc *sc, const unsigned char *mac,
                     enum nl80211_iftype iftype, bool p2p, struct mm_add_if_cfm *cfm);
int ssv_send_remove_if(struct ssv_softc *sc, u8 drv_vif_index);
int ssv_send_set_channel(struct ssv_softc *sc, int phy_idx,
                          struct mm_set_channel_cfm *cfm);
int ssv_send_key_add(struct ssv_softc *sc, u8 vif_idx, u8 sta_idx, bool pairwise,
                      u8 *key, u8 key_len, u8 key_idx, u8 cipher_suite,
                      struct mm_key_add_cfm *cfm);
int ssv_send_key_del(struct ssv_softc *sc, uint8_t hw_key_idx);
int ssv_send_bcn_change(struct ssv_softc *sc, u8 vif_idx, u8 *buf,
                         u16 bcn_len, u16 tim_oft, u16 tim_len, u16 *csa_oft);
int ssv_send_tim_update(struct ssv_softc *sc, u8 vif_idx, u16 aid,
                         u8 tx_status);
int ssv_send_roc(struct ssv_softc *sc, struct ssv_vif *vif,
                  struct ieee80211_channel *chan, unsigned int duration);
int ssv_send_cancel_roc(struct ssv_softc *sc);
int ssv_send_set_power(struct ssv_softc *sc,  u8 vif_idx, s8 pwr,
                        struct mm_set_power_cfm *cfm);
int ssv_send_set_edca(struct ssv_softc *sc, u8 hw_queue, u32 param,
                       bool uapsd, u8 inst_nbr);

int ssv_send_me_config_req(struct ssv_softc *sc);
int ssv_send_me_chan_config_req(struct ssv_softc *sc);
int ssv_send_me_set_control_port_req(struct ssv_softc *sc, bool opened,
                                      u8 sta_idx);
int ssv_send_me_sta_add(struct ssv_softc *sc, struct station_parameters *params,
                         const u8 *mac, u8 inst_nbr, struct me_sta_add_cfm *cfm);
int ssv_send_me_sta_del(struct ssv_softc *sc, u8 sta_idx, bool tdls_sta);
int ssv_send_me_traffic_ind(struct ssv_softc *sc, u8 sta_idx, bool uapsd, u8 tx_status);
int ssv_send_me_rc_stats(struct ssv_softc *sc, u8 sta_idx,
                          struct me_rc_stats_cfm *cfm);

int ssv_send_me_rc_set_rate(struct ssv_softc *sc,
                             u8 sta_idx,
                             u16 rate_idx);
int ssv_send_me_rc_set_ldpc(struct ssv_softc *sc,
                             u8 sta_idx,
                             bool is_enable);
int ssv_send_me_rc_set_ltf_gi(struct ssv_softc *sc,
                             u8 sta_idx,
                             u8 ltf_ig_type);

int ssv_send_me_rc_sta_reset_default(struct ssv_softc *sc,
                             u8 sta_idx);

int ssv_send_sm_connect_req(struct ssv_softc *sc,
                             struct ssv_vif *ssv_vif,
                             struct cfg80211_connect_params *sme,
                             struct sm_connect_cfm *cfm);
int ssv_send_sm_disconnect_req(struct ssv_softc *sc,
                                struct ssv_vif *ssv_vif,
                                u16 reason);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 17, 0) || defined(CONFIG_SUPPORT_WPA3)) || defined(CONFIG_WPA_SUPPLICANT_CTL)
int ssv_send_sm_external_auth_required_rsp(struct ssv_softc *sc,
                                struct ssv_vif *ssv_vif,
                                u16 status);
#endif
int ssv_send_apm_start_req(struct ssv_softc *sc, struct ssv_vif *vif,
                            struct cfg80211_ap_settings *settings,
                            struct apm_start_cfm *cfm, u8 **bcn_buf);
int ssv_send_apm_stop_req(struct ssv_softc *sc, struct ssv_vif *vif);
int ssv_send_scanu_req(struct ssv_softc *sc, struct ssv_vif *ssv_vif,
                        struct cfg80211_scan_request *param);
int ssv_send_apm_start_cac_req(struct ssv_softc *sc, struct ssv_vif *vif,
                                struct cfg80211_chan_def *chandef,
                                struct apm_start_cac_cfm *cfm);
int ssv_send_apm_stop_cac_req(struct ssv_softc *sc, struct ssv_vif *vif);
/* Debug messages */
int ssv_send_dbg_trigger_req(struct ssv_softc *sc, char *msg);
int ssv_send_dbg_mem_read_req(struct ssv_softc *sc, u32 mem_addr,
                               struct dbg_mem_read_cfm *cfm);
int ssv_send_dbg_mem_write_req(struct ssv_softc *sc, u32 mem_addr,
                                u32 mem_data);
int ssv_send_dbg_set_mod_filter_req(struct ssv_softc *sc, u32 filter);
int ssv_send_dbg_set_sev_filter_req(struct ssv_softc *sc, u32 filter);
int ssv_send_dbg_get_sys_stat_req(struct ssv_softc *sc,
                                   struct dbg_get_sys_stat_cfm *cfm);
int ssv_send_cfg_rssi_req(struct ssv_softc *sc, u8 drv_vif_index, int rssi_thold, u32 rssi_hyst);

int ssv_send_priv_msg_ampdu_setting(struct ssv_softc *sc, u32 param);

int ssv_send_me_config_monitor_req(struct ssv_softc *sc, struct cfg80211_chan_def *chandef,
                                    bool chan_set, bool uf,
                                    struct me_config_monitor_cfm *cfm);
int ssv_send_filter_duplicate_rx(struct ssv_softc *sc, u32 filter_duplicate_rx);
int ssv_send_wmm_follow_vo(struct ssv_softc *sc, u32 wmm_follow_vo);
int ssv_send_macaddr(struct ssv_softc *sc, u8 *mac0, u8 *mac1);
int ssv_send_ipc_tx_use_one_hwq(struct ssv_softc *sc, u32 ac);
int ssv_send_set_policy_tbl(struct ssv_softc *sc, u8 set_rts_method);
int ssv_send_txq_credit_boundary(struct ssv_softc *sc);
int ssv_send_io_aggr_setting(struct ssv_softc *sc, bool is_rx, bool enable);
int ssv_send_mac_addr_rule_setting(struct ssv_softc *sc, u32  mac_low_mask, u32 mac_high_mask);

#ifdef CONFIG_MIFI_LOWPOWER
int ssv_send_priv_msg_mifi_setting(struct ssv_softc *sc, u32 mifi_feature_setting, u32 mifi_no_traffic_duration_setting);
#endif
#endif /* __FMAC_MSG_TX_H__ */

