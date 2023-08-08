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
#include <linux/string.h>
#include <linux/platform_device.h>

#include <ssv_chip_id.h>
#include "ssv6006_cfg.h"
#include "ssv6006_mac.h"

#include <smac/dev.h>
#include <hal.h>
#include "ssv6006_priv.h"
#include <smac/ssv_skb.h>
#include <hci/hctrl.h>
#include <ssvdevice/ssv_cmd.h>

static struct ssv_hw * ssv6006_alloc_hw (void)
{
    struct ssv_hw *sh;

    sh = kzalloc(sizeof(struct ssv_hw), GFP_KERNEL);
    if (sh == NULL)
        goto out;

    memset((void *)sh, 0, sizeof(struct ssv_hw));

    sh->page_count = (u8 *)kzalloc(sizeof(u8) * SSV6006_ID_NUMBER, GFP_KERNEL);
    if (sh->page_count == NULL) 
        goto out;

    memset(sh->page_count, 0, sizeof(u8) * SSV6006_ID_NUMBER);

    return sh;
out:
    if (sh->page_count)
        kfree(sh->page_count);
    if (sh)
        kfree(sh);

    return NULL;
}


static bool ssv6006_use_hw_encrypt(int cipher, struct ssv_softc *sc, 
        struct ssv_sta_priv_data *sta_priv, struct ssv_vif_priv_data *vif_priv )
{

    return true;                     
}

static bool ssv6006_if_chk_mac2(struct ssv_hw *sh)
{
    printk(" %s: is not need to check MAC addres 2 for this model \n",__func__); 
    return false;
}

static int ssv6006_get_wsid(struct ssv_softc *sc, struct ieee80211_vif *vif,
        struct ieee80211_sta *sta)
{    
    struct ssv_vif_priv_data *vif_priv = (struct ssv_vif_priv_data *)vif->drv_priv;
    int     s;
    struct ssv_sta_priv_data *sta_priv_dat=NULL;
    struct ssv_sta_info *sta_info;/* sta_info is already protected by ssv6200_sta_add(). */

    for (s = 0; s < SSV_NUM_STA; s++)
    {
        sta_info = &sc->sta_info[s];/* sta_info is already protected by ssv6200_sta_add(). */
        if ((sta_info->s_flags & STA_FLAG_VALID) == 0)/* sta_info is already protected by ssv6200_sta_add(). */
        {
            sta_info->aid = sta->aid;/* sta_info is already protected by ssv6200_sta_add(). */
            sta_info->sta = sta;/* sta_info is already protected by ssv6200_sta_add(). */
            sta_info->vif = vif;/* sta_info is already protected by ssv6200_sta_add(). */
            sta_info->s_flags = STA_FLAG_VALID;/* sta_info is already protected by ssv6200_sta_add(). */

            sta_priv_dat = 
                (struct ssv_sta_priv_data *)sta->drv_priv;
            sta_priv_dat->sta_idx = s;
            sta_priv_dat->sta_info = sta_info;/* sta_info is already protected by ssv6200_sta_add(). */
            sta_priv_dat->has_hw_encrypt = false;
            sta_priv_dat->has_hw_decrypt = false;
            sta_priv_dat->need_sw_decrypt = false;
            sta_priv_dat->need_sw_encrypt = false;

            // WEP use single key for pairwise and broadcast frames.
            // In AP mode, key is only set when AP mode is initialized.
            if (   (vif_priv->pair_cipher == SECURITY_WEP40)
                    || (vif_priv->pair_cipher == SECURITY_WEP104))
            {
                sta_priv_dat->has_hw_encrypt = vif_priv->has_hw_encrypt;
                sta_priv_dat->has_hw_decrypt = vif_priv->has_hw_decrypt;
                sta_priv_dat->need_sw_encrypt = vif_priv->need_sw_encrypt;
                sta_priv_dat->need_sw_decrypt = vif_priv->need_sw_decrypt;                    
            }

            list_add_tail(&sta_priv_dat->list, &vif_priv->sta_list);

            //temp mark: PS
            //sc->ps_aid = sta->aid;
            break;
        }
    }
    return s;
}


static void ssv6006_add_fw_wsid(struct ssv_softc *sc, struct ssv_vif_priv_data *vif_priv, 
        struct ieee80211_sta *sta, struct ssv_sta_info *sta_info)
{
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
            " %s: is not need for this model \n",__func__);       
}

static void ssv6006_del_fw_wsid(struct ssv_softc *sc, struct ieee80211_sta *sta,
        struct ssv_sta_info *sta_info)
{    
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
            " %s: is not need for this model \n",__func__);
}

static void ssv6006_enable_fw_wsid(struct ssv_softc *sc, struct ieee80211_sta *sta,
        struct ssv_sta_info *sta_info, enum SSV6XXX_WSID_SEC key_type)
{       
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
            " %s: is not need for this model \n",__func__);
}

static void ssv6006_disable_fw_wsid(struct ssv_softc *sc, int key_idx,
        struct ssv_sta_priv_data *sta_priv, struct ssv_vif_priv_data *vif_priv)
{
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
            " %s: is not need for this model \n",__func__);
}

static void ssv6006_set_fw_hwwsid_sec_type(struct ssv_softc *sc, struct ieee80211_sta *sta,
        struct ssv_sta_info *sta_info, struct ssv_vif_priv_data *vif_priv)
{
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
            " %s: is not need for this model \n",__func__);
}

//  return true if wep use hw cipher, default always use hw cipher
static bool ssv6006_wep_use_hw_cipher(struct ssv_softc *sc, struct ssv_vif_priv_data *vif_priv)
{
    bool ret = false;

    if (!USE_MAC80211_CIPHER(sc->sh)) {
        ret = true;
    }

    return ret;
}

//  return true if wpa use hw cipher for pairwise, default always use hw cipher
static bool ssv6006_pairwise_wpa_use_hw_cipher(struct ssv_softc *sc, 
        struct ssv_vif_priv_data *vif_priv, enum sec_type_en cipher,
        struct ssv_sta_priv_data *sta_priv)
{
    bool ret = false;

    if (!USE_MAC80211_CIPHER(sc->sh)) {
        ret = true;
    }

    return ret;
}

//  return true if group wpa use hw cipher
static bool ssv6006_group_wpa_use_hw_cipher(struct ssv_softc *sc, 
        struct ssv_vif_priv_data *vif_priv, enum sec_type_en cipher)
{
    int     ret =false;

    if (!USE_MAC80211_CIPHER(sc->sh)) {
        ret = true;
    }

    return ret;
}


static bool ssv6006_chk_if_support_hw_bssid(struct ssv_softc *sc,
        int vif_idx)
{
    if ((vif_idx >= 0) && (vif_idx < SSV6006_NUM_HW_BSSID))
        return true;

    printk(" %s: VIF %d doesn't support HW BSSID\n", __func__, vif_idx);
    return false;
}

static void ssv6006_chk_dual_vif_chg_rx_flow(struct ssv_softc *sc, 
        struct ssv_vif_priv_data *vif_priv)
{
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
            " %s: is not need for this model \n",__func__);   
}

static void ssv6006_restore_rx_flow(struct ssv_softc *sc, 
        struct ssv_vif_priv_data *vif_priv, struct ieee80211_sta *sta)
{
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
            " %s: is not need for this model \n",__func__);   
}

// SSV6006, when CCMP security use HW cipher and ccmp header was generated from SW.
// Allocates CCMP MIC field but not process it.
static bool ssv6006_put_mic_space_for_hw_ccmp_encrypt(struct ssv_softc *sc, struct sk_buff *skb) 
{
    struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
    struct ieee80211_key_conf *hw_key = info->control.hw_key;
    struct ieee80211_hdr *hdr = (struct ieee80211_hdr *) skb->data;
    struct SKB_info_st *skb_info = (struct SKB_info_st *)skb->head;
    struct ieee80211_sta *sta = skb_info->sta;
    struct ssv_vif_priv_data *vif_priv = (struct ssv_vif_priv_data *)info->control.vif->drv_priv;
    struct ssv_sta_priv_data *ssv_sta_priv = sta ? (struct ssv_sta_priv_data *)sta->drv_priv : NULL;

    if ( (!ieee80211_is_data_qos(hdr->frame_control) 
                && !ieee80211_is_data(hdr->frame_control))
            || !ieee80211_has_protected(hdr->frame_control) )
        return false;

    if (hw_key)
    {
        if(hw_key->cipher != WLAN_CIPHER_SUITE_CCMP)
            return false;
    }

    // Broadcast or Multicast frame
    if (!is_unicast_ether_addr(hdr->addr1))
    {
        if (vif_priv->is_security_valid
                && vif_priv->has_hw_encrypt)
        {
            pskb_expand_head(skb, 0, CCMP_MIC_LEN, GFP_ATOMIC);
            skb_put(skb, CCMP_MIC_LEN);
            return true;
        }
    }
    // Unicast
    else if (ssv_sta_priv != NULL)
    {
        if (ssv_sta_priv->has_hw_encrypt)
        {
            pskb_expand_head(skb, 0, CCMP_MIC_LEN, GFP_ATOMIC);
            skb_put(skb, CCMP_MIC_LEN);
            return true;
        }
    }

    return false;
}

static bool ssv6006_use_turismo_hw_encrypt(struct ssv_softc *sc, struct ssv_vif_priv_data *vif_priv) 
{
    if ((vif_priv->pair_cipher == SECURITY_CCMP)
            && (!USE_MAC80211_CIPHER(sc->sh))) {
        return true;
    } else {
        return false;
    }
}

static void ssv6006_init_tx_cfg(struct ssv_hw *sh)
{
    u32 dev_type = HCI_DEVICE_TYPE(sh->hci.hci_ctrl);

    sh->total_tx_rx_page = SSV6006_TOTAL_PAGE;
    if ((sh->cfg.tx_page_threshold == 0) || (sh->cfg.tx_page_threshold > SSV6006_PAGE_TX_THRESHOLD)) {
        sh->tx_info.tx_page_threshold = SSV6006_PAGE_TX_THRESHOLD;
        sh->tx_info.tx_lowthreshold_page_trigger = SSV6006_TX_LOWTHRESHOLD_PAGE_TRIGGER;
    } else {
        sh->tx_info.tx_page_threshold = sh->cfg.tx_page_threshold;
        sh->tx_info.tx_lowthreshold_page_trigger = 
            (sh->tx_info.tx_page_threshold - (sh->tx_info.tx_page_threshold/SSV6006_AMPDU_DIVIDER));
    }

    /* For security,  reserve the security table*/
    sh->total_tx_rx_page = sh->total_tx_rx_page - SSV6006_RESERVED_SEC_PAGE;
    sh->tx_info.tx_page_threshold = sh->tx_info.tx_page_threshold - SSV6006_RESERVED_SEC_PAGE;
    sh->tx_page_available = sh->tx_info.tx_page_threshold;

    /* For USB, sw resoure should take care of USB FIFO */
    if (dev_type == SSV_HWIF_INTERFACE_USB) {
        sh->total_tx_rx_page = sh->total_tx_rx_page - SSV6006_USB_FIFO;
        sh->tx_info.tx_page_threshold = sh->tx_info.tx_page_threshold - SSV6006_USB_FIFO;
        sh->tx_page_available = sh->tx_info.tx_page_threshold - SSV6006_RESERVED_USB_PAGE;
    }
    
    // set txid = txpage
    sh->tx_info.tx_id_threshold = sh->tx_info.tx_page_threshold>>1; //Divide by 2, to avoid tx+rx id is bigger than 128.
    sh->tx_info.tx_lowthreshold_id_trigger = (sh->tx_info.tx_id_threshold - 1);
    
    //printk ("%s: usb_hw_resource %d, tx_id_threshold %d, tx_page threshold %d\n", __func__,sh->cfg.usb_hw_resource, 
    //    sh->tx_info.tx_id_threshold , sh->tx_info.tx_page_threshold );

    sh->tx_info.bk_txq_size = SSV6006_ID_AC_BK_OUT_QUEUE;
    sh->tx_info.be_txq_size = SSV6006_ID_AC_BE_OUT_QUEUE;
    sh->tx_info.vi_txq_size = SSV6006_ID_AC_VI_OUT_QUEUE;
    sh->tx_info.vo_txq_size = SSV6006_ID_AC_VO_OUT_QUEUE;
    sh->tx_info.manage_txq_size = SSV6006_ID_MANAGER_QUEUE;

    sh->ampdu_divider = SSV6006_AMPDU_DIVIDER;

    //info hci
    memcpy(&(sh->hci.hci_ctrl->tx_info), &(sh->tx_info), sizeof(struct ssv6xxx_tx_hw_info));
}

static void ssv6006_init_rx_cfg(struct ssv_hw *sh)
{

    // Set rx page
    sh->rx_info.rx_page_threshold = sh->total_tx_rx_page - sh->tx_info.tx_page_threshold;
    // Set rx id
    sh->rx_info.rx_id_threshold = sh->rx_info.rx_page_threshold>>1; //Divide by 2, to avoid tx+rx id is bigger than 128.

    sh->rx_info.rx_ba_ma_sessions = SSV6006_RX_BA_MAX_SESSIONS;
    //info hci
    memcpy(&(sh->hci.hci_ctrl->rx_info), &(sh->rx_info), sizeof(struct ssv6xxx_rx_hw_info));
    //printk ("%s: usb_hw_resource %d, rx_id_threshold %d, rx_page threshold %d\n", __func__,sh->cfg.usb_hw_resource, 
    //	    sh->rx_info.rx_id_threshold , sh->rx_info.rx_page_threshold );
}

int ssv6006_ampdu_rx_start(struct ieee80211_hw *hw, struct ieee80211_vif *vif, struct ieee80211_sta *sta, 
        u16 tid, u16 *ssn, u8 buf_size)
{
    struct ssv_softc *sc = hw->priv;
    struct ssv_sta_info *sta_info;/* sta_info is already protected by ssv6200_ampdu_action(). */
    int i = 0;
    bool find_peer = false;

    for (i = 0; i < SSV_NUM_STA; i++) {
        sta_info = &sc->sta_info[i];/* sta_info is already protected by ssv6200_ampdu_action(). */
        if ((sta_info->s_flags & STA_FLAG_VALID) && (sta == sta_info->sta)) {/* sta_info is already protected by ssv6200_ampdu_action(). */
            find_peer = true;
            break;
        }
    }

    if ((find_peer == false) || (sc->rx_ba_session_count > sc->sh->rx_info.rx_ba_ma_sessions)) 
        return -EBUSY;

    // notify mac80211 amdpu rx session success
    if (sta_info->s_flags & STA_FLAG_AMPDU_RX)/* sta_info is already protected by ssv6200_ampdu_action(). */
        return 0;

    printk(KERN_ERR "IEEE80211_AMPDU_RX_START %02X:%02X:%02X:%02X:%02X:%02X %d.\n",
            sta->addr[0], sta->addr[1], sta->addr[2], sta->addr[3],
            sta->addr[4], sta->addr[5], tid);

    sc->rx_ba_session_count++;
    sta_info->s_flags |= STA_FLAG_AMPDU_RX;/* sta_info is already protected by ssv6200_ampdu_action(). */

    return 0;
}

static void ssv6006_adj_config(struct ssv_hw *sh)
{
    int dev_type;

    /* 
     * SV6155P: single band HT40 USB
     * SV6156P/SV6166: single band HT40 SDIO
     * SV6255P: dual band HT40 USB
     * SV6256P: dual band HT40 SDIO
     * SV6245: 4.6G band
     */
    if (sh->cfg.force_chip_identity) 
        sh->cfg.chip_identity = sh->cfg.force_chip_identity;

    switch (sh->cfg.chip_identity) {
        case SV6255P:
        case SV6256P:
        case SV6267Q:
        case SV6245:
            sh->cfg.hw_caps |= SSV6200_HW_CAP_5GHZ;
            break;
        case SV6155P:
        case SV6156P:
        case SV6166P:
        case SV6166F:
        case SV6167Q:
        case SV6151P_SV6152P:
            printk("not support 5G for this chip!! \n");
            sh->cfg.hw_caps = sh->cfg.hw_caps & (~(SSV6200_HW_CAP_5GHZ));
            break;
        default:
            printk("unknown chip\n");
            break;
    }

    if (strstr(sh->priv->chip_id, SSV6006D)){
        printk("not support 5G for this chip!! \n");
        sh->cfg.hw_caps = sh->cfg.hw_caps & (~(SSV6200_HW_CAP_5GHZ));
    }    

    if (sh->cfg.use_wpa2_only){
        printk("%s: use_wpa2_only set to 1, force it to 0 \n", __func__);
        sh->cfg.use_wpa2_only = 0;
    }

    dev_type = HCI_DEVICE_TYPE(sh->hci.hci_ctrl);
    if (dev_type == SSV_HWIF_INTERFACE_USB){
        if (sh->cfg.hw_caps & SSV6200_HW_CAP_HCI_RX_AGGR){
            printk("%s: clear hci rx aggregation setting \n", __func__);

            sh->cfg.hw_caps = sh->cfg.hw_caps & (~(SSV6200_HW_CAP_HCI_RX_AGGR));
        }
        /* for usb , it can't use 26M or 24M xtal, change to 40M)*/
        if ((sh->cfg.crystal_type == SSV6XXX_IQK_CFG_XTAL_26M) || (sh->cfg.crystal_type == SSV6XXX_IQK_CFG_XTAL_24M)) {            
            sh->cfg.crystal_type = SSV6XXX_IQK_CFG_XTAL_40M;
            printk("%s: for USB, change iqk config crystal_type to %d \n", __func__, sh->cfg.crystal_type);
        }

        /* for usb, HCI_TX_AGGR is NOT available due to design limitation */
        if (sh->cfg.hw_caps & SSV6200_HW_CAP_HCI_TX_AGGR){
            printk("%s: clear hci tx aggregation setting \n", __func__);

            sh->cfg.hw_caps = sh->cfg.hw_caps & (~(SSV6200_HW_CAP_HCI_TX_AGGR));
        }
    }

    if ((sh->cfg.tx_stuck_detect) && (dev_type == SSV_HWIF_INTERFACE_SDIO)) {
        printk("%s: tx_stuck_detect set to 1, force it to 0 \n", __func__);
        sh->cfg.tx_stuck_detect = false;
    }

    if (sh->cfg.hw_caps & SSV6200_HW_CAP_BEACON) {
        printk("%s: clear hw beacon \n", __func__);
        sh->cfg.hw_caps &= ~SSV6200_HW_CAP_BEACON;
    }
    
    if ((sh->cfg.chip_identity == SV6167Q) || (sh->cfg.chip_identity == SV6267Q)) {
        printk("%s: support external PA for this chip\n", __func__);
    } else {
        printk("%s: not support external PA for this chip\n", __func__);
    }
    
    if (sh->cfg.scan_hc_period == 0) {
        printk("%s: change scan home channel period to default value\n", __func__);
        sh->cfg.scan_hc_period = 100;
    }
    
    if (sh->cfg.scan_oc_period == 0) {
        printk("%s: change off home channel period to default value\n", __func__);
        sh->cfg.scan_oc_period = 50;
    }
}

static void ssv6006_get_fw_name(u8 *fw_name)
{
    strcpy(fw_name, "ssv6x5x-sw.bin");
}

static bool ssv6006_need_sw_cipher(struct ssv_hw *sh)
{
    return false;
}

static void ssv6006_send_tx_poll_cmd(struct ssv_hw *sh, u32 type)
{
    struct sk_buff *skb;
    struct cfg_host_cmd *host_cmd;
    int retval = 0;

    if (!sh->cfg.tx_stuck_detect)
        return;

    skb = ssv_skb_alloc(sh->sc, HOST_CMD_HDR_LEN);
    if (skb == NULL) {
        printk("%s:_skb_alloc fail!!!\n", __func__);
        return;
    }  

    skb_put(skb, HOST_CMD_HDR_LEN);
    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
    host_cmd->c_type = HOST_CMD;
    host_cmd->RSVD0 = type;
    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_TX_POLL;
    host_cmd->len = skb->len;
    retval = HCI_SEND_CMD(sh, skb);
    if (retval)
        printk("%s(): Fail to send tx polling cmd\n", __FUNCTION__);
}

static void ssv6006_cmd_set_hwq_limit(struct ssv_hw *sh, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;
    char *endp;

    if ( argc != 3) return;

    if (!strcmp(argv[1], "bk")) {
        sh->cfg.bk_txq_size = simple_strtoul(argv[2], &endp, 0);
    } else if (!strcmp(argv[1], "be")) {
        sh->cfg.be_txq_size = simple_strtoul(argv[2], &endp, 0);
    } else if (!strcmp(argv[1], "vi")) {
        sh->cfg.vi_txq_size = simple_strtoul(argv[2], &endp, 0);
    } else if (!strcmp(argv[1], "vo")) {
        sh->cfg.vo_txq_size = simple_strtoul(argv[2], &endp, 0);
    } else if (!strcmp(argv[1], "mng")) {
        sh->cfg.manage_txq_size = simple_strtoul(argv[2], &endp, 0);
    } else {
        snprintf_res(cmd_data,"\t\t %s is unknown!\n", argv[1]);
    }
#endif
}

static void ssv6006_flash_read_all_map(struct ssv_hw *sh)
{
    struct file *fp = (struct file *)NULL;
    int rdlen = 0, i = 0;
    struct ssv6006_flash_layout_table flash_table;

    memset(&flash_table, 0, sizeof(struct ssv6006_flash_layout_table));

    if (sh->cfg.flash_bin_path[0] != 0x00)
        fp = filp_open(sh->cfg.flash_bin_path, O_RDONLY, 0);
    else 
        fp = filp_open(DEFAULT_CFG_BIN_NAME, O_RDONLY, 0);

    if (IS_ERR(fp) || fp == NULL) 
        fp = filp_open(SEC_CFG_BIN_NAME, O_RDONLY, 0);

    if (IS_ERR(fp) || fp == NULL) {
        printk("flash_file %s not found\n", DEFAULT_CFG_BIN_NAME);
        return;
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,0)
    rdlen = kernel_read(fp, (u8 *)&flash_table, sizeof(struct ssv6006_flash_layout_table), &fp->f_pos);
#else
    rdlen = kernel_read(fp, fp->f_pos, (u8 *)&flash_table, sizeof(struct ssv6006_flash_layout_table));
#endif    
    filp_close((struct file *)fp, NULL);
    if (rdlen < 0)
        return;

#define SV6155P_IC      0x6155
#define SV6156P_IC      0x6156
#define SV6255P_IC      0x6255
#define SV6256P_IC      0x6256
    if (!((flash_table.ic == SV6155P_IC) || 
                (flash_table.ic == SV6156P_IC) || 
                (flash_table.ic == SV6255P_IC) || 
                (flash_table.ic == SV6256P_IC))) {
        printk("Invalid flash table [ic=0x%04x]\n", flash_table.ic);
        BUG_ON(1);
    }

    sh->flash_config.exist = true;
    sh->flash_config.dcdc = flash_table.dcdc;
    sh->cfg.volt_regulator = ((sh->flash_config.dcdc) ? SSV6XXX_VOLT_DCDC_CONVERT : SSV6XXX_VOLT_LDO_CONVERT);
    sh->flash_config.padpd = flash_table.padpd;
    sh->flash_config.xtal_offset_tempe_state = SSV_TEMPERATURE_NORMAL; 
    sh->flash_config.xtal_offset_high_boundary = flash_table.xtal_offset_high_boundary;
    sh->flash_config.xtal_offset_low_boundary = flash_table.xtal_offset_low_boundary;
    sh->flash_config.band_gain_tempe_state = SSV_TEMPERATURE_NORMAL;
    sh->flash_config.band_gain_high_boundary = flash_table.band_gain_high_boundary;
    sh->flash_config.band_gain_low_boundary = flash_table.band_gain_low_boundary;

    sh->flash_config.rt_config.xtal_offset = flash_table.xtal_offset_rt_config; 
    sh->flash_config.lt_config.xtal_offset = flash_table.xtal_offset_lt_config; 
    sh->flash_config.ht_config.xtal_offset = flash_table.xtal_offset_ht_config; 

    sh->flash_config.g_band_pa_bias0 = flash_table.g_band_pa_bias0;
    sh->flash_config.g_band_pa_bias1 = flash_table.g_band_pa_bias1;
    sh->flash_config.a_band_pa_bias0 = flash_table.a_band_pa_bias0;
    sh->flash_config.a_band_pa_bias1 = flash_table.a_band_pa_bias1;

#define SIZE_G_BAND_GAIN   (sizeof(sh->flash_config.rt_config.g_band_gain) / sizeof((sh->flash_config.rt_config.g_band_gain)[0]))
    for (i = 0; i < SIZE_G_BAND_GAIN; i++) { 
        sh->flash_config.rt_config.g_band_gain[i] = flash_table.g_band_gain_rt[i];
        sh->flash_config.lt_config.g_band_gain[i] = flash_table.g_band_gain_lt[i];
        sh->flash_config.ht_config.g_band_gain[i] = flash_table.g_band_gain_ht[i];
    }

#define SIZE_A_BAND_GAIN   (sizeof(sh->flash_config.rt_config.a_band_gain) / sizeof((sh->flash_config.rt_config.a_band_gain)[0]))
    for (i = 0; i < SIZE_A_BAND_GAIN; i++) { 
        sh->flash_config.rt_config.a_band_gain[i] = flash_table.a_band_gain_rt[i];
        sh->flash_config.lt_config.a_band_gain[i] = flash_table.a_band_gain_lt[i];
        sh->flash_config.ht_config.a_band_gain[i] = flash_table.a_band_gain_ht[i];
    }

#define SIZE_RATE_DELTA   (sizeof(sh->flash_config.rate_delta) / sizeof((sh->flash_config.rate_delta)[0]))
    for (i = 0; i < SIZE_RATE_DELTA; i++) { 
        sh->flash_config.rate_delta[i] = flash_table.rate_delta[i];
    }

    if (!(sh->sc->log_ctrl & LOG_FLASH_BIN))
        return;

    printk("flash.bin configuration\n");

    printk("xtal_offset_boundary, high = %d, low = %d\n", 
            sh->flash_config.xtal_offset_high_boundary, sh->flash_config.xtal_offset_low_boundary);
    printk("band_gain_boundary, high = %d, low = %d\n", 
            sh->flash_config.band_gain_high_boundary, sh->flash_config.band_gain_low_boundary);

    printk("xtal_offset, rt = 0x%04x, lt = 0x%04x, ht = 0x%04x\n", 
            sh->flash_config.rt_config.xtal_offset,
            sh->flash_config.lt_config.xtal_offset,
            sh->flash_config.ht_config.xtal_offset); 

    printk("g_band_pa_bias0 = 0x%08x, g_band_pa_bias1 = 0x%08x\n", 
            sh->flash_config.g_band_pa_bias0, sh->flash_config.g_band_pa_bias1);
    printk("a_band_pa_bias0 = 0x%08x, a_band_pa_bias1 = 0x%08x\n", 
            sh->flash_config.a_band_pa_bias0, sh->flash_config.a_band_pa_bias1);

    printk("g band gain:\trt\tlt\tht\n");
    for (i = 0; i < SIZE_G_BAND_GAIN; i++) { 
        printk("\t\t\t0x%x,\t0x%x,\t0x%x", sh->flash_config.rt_config.g_band_gain[i], 
                sh->flash_config.lt_config.g_band_gain[i],
                sh->flash_config.ht_config.g_band_gain[i]);
    } 
    printk("\n");

    printk("a band gain:\trt\tlt\tht\n");
    for (i = 0; i < SIZE_A_BAND_GAIN; i++) { 
        printk("\t\t\t0x%x,\t0x%x,\t0x%x", sh->flash_config.rt_config.a_band_gain[i], 
                sh->flash_config.lt_config.a_band_gain[i],
                sh->flash_config.ht_config.a_band_gain[i]);
    } 
    printk("\n");

    printk("rate delta: ");
    for (i = 0; i < SIZE_RATE_DELTA; i++)  
        printk("%x, ", sh->flash_config.rate_delta[i]);

    return;
}

static void ssv6006_ps_evt_handler(struct ssv_softc *sc, struct sk_buff *ps_evt)
{
    struct cfg_host_event *h_evt = (struct cfg_host_event *)ps_evt->data;
    struct ssv6xxx_ps_params *ps_params = (struct ssv6xxx_ps_params *)h_evt->dat;

    switch (ps_params->ops) {
        case SSV6XXX_PS_WAKEUP:
            printk("%s(): wakeup the host by sdio\n", __FUNCTION__);
            break;
        case SSV6XXX_PS_WAKEUP_FIN_ACK:
            complete(&sc->wakeup_done);
            printk("%s(): wakeup FIN ACK is received\n", __FUNCTION__);
            break;
        case SSV6XXX_PS_HOLD_ON3_ACK:
            complete(&sc->hold_on3);
            printk("%s(): holding ON3 ACK is received\n", __FUNCTION__);
            break;
        default:
            printk("%s(): unknown ps_params->ops = %d\n", __FUNCTION__, ps_params->ops);
            break;
    }
}

void ssv_attach_ssv6006_common(struct ssv_hal_ops *hal_ops)
{
    hal_ops->alloc_hw = ssv6006_alloc_hw;
    hal_ops->use_hw_encrypt = ssv6006_use_hw_encrypt;
    hal_ops->if_chk_mac2= ssv6006_if_chk_mac2;

    hal_ops->get_wsid = ssv6006_get_wsid;    
    hal_ops->add_fw_wsid = ssv6006_add_fw_wsid;
    hal_ops->del_fw_wsid = ssv6006_del_fw_wsid;
    hal_ops->enable_fw_wsid = ssv6006_enable_fw_wsid;
    hal_ops->disable_fw_wsid = ssv6006_disable_fw_wsid;

    hal_ops->set_fw_hwwsid_sec_type = ssv6006_set_fw_hwwsid_sec_type;
    hal_ops->wep_use_hw_cipher = ssv6006_wep_use_hw_cipher;
    hal_ops->pairwise_wpa_use_hw_cipher = ssv6006_pairwise_wpa_use_hw_cipher;
    hal_ops->group_wpa_use_hw_cipher = ssv6006_group_wpa_use_hw_cipher;
    hal_ops->chk_if_support_hw_bssid = ssv6006_chk_if_support_hw_bssid;
    hal_ops->chk_dual_vif_chg_rx_flow = ssv6006_chk_dual_vif_chg_rx_flow;
    hal_ops->restore_rx_flow = ssv6006_restore_rx_flow;

    hal_ops->put_mic_space_for_hw_ccmp_encrypt = ssv6006_put_mic_space_for_hw_ccmp_encrypt;
    hal_ops->use_turismo_hw_encrpt = ssv6006_use_turismo_hw_encrypt;

    hal_ops->init_tx_cfg = ssv6006_init_tx_cfg;
    hal_ops->init_rx_cfg = ssv6006_init_rx_cfg;

    hal_ops->ampdu_rx_start = ssv6006_ampdu_rx_start;    
    hal_ops->adj_config = ssv6006_adj_config;
    hal_ops->get_fw_name = ssv6006_get_fw_name;   
    hal_ops->need_sw_cipher = ssv6006_need_sw_cipher;
    hal_ops->send_tx_poll_cmd = ssv6006_send_tx_poll_cmd;
    hal_ops->cmd_hwq_limit = ssv6006_cmd_set_hwq_limit;
    hal_ops->flash_read_all_map = ssv6006_flash_read_all_map;   
    hal_ops->ps_evt_handler = ssv6006_ps_evt_handler;
}



#endif
