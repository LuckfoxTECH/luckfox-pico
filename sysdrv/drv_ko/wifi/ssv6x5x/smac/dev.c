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

#include <linux/nl80211.h>
#include <linux/etherdevice.h>
#include <linux/delay.h>
#include <linux/version.h>
#include <linux/time.h>
#include <linux/kthread.h>
#include <linux/ip.h>
#include <net/udp.h>
#ifdef SSV_MAC80211
#include "ssv_mac80211.h"
#else
#include <net/mac80211.h>
#endif
#include <net/cfg80211.h>
#include <ssv6200.h>
#include <hci/hctrl.h>

#include "lib.h"
#include "wow.h"
#include "dev.h"
#include "ap.h"
#include "init.h"
#include "ssv_skb.h"
#include "hw_scan.h"
#include <hal.h>
#include <linux_80211.h>

#ifdef CONFIG_SSV_SUPPORT_ANDROID
#include "ssv_pm.h"
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,2,0) && LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,0)
#include "linux_3_0_0.h"
#endif

#ifdef CONFIG_SSV6XXX_DEBUGFS
#include "ssv6xxx_debugfs.h"
#endif // CONFIG_SSV6XXX_DEBUGFS

//#define IRQ_PROC_RX_DATA

static u16 bits_per_symbol[][2] =
{
    /* 20MHz 40MHz */
    {    26,   54 },     /*  0: BPSK */
    {    52,  108 },     /*  1: QPSK 1/2 */
    {    78,  162 },     /*  2: QPSK 3/4 */
    {   104,  216 },     /*  3: 16-QAM 1/2 */
    {   156,  324 },     /*  4: 16-QAM 3/4 */
    {   208,  432 },     /*  5: 64-QAM 2/3 */
    {   234,  486 },     /*  6: 64-QAM 3/4 */
    {   260,  540 },     /*  7: 64-QAM 5/6 */
};

static void ssv6xxx_process_rx_q(struct ssv_softc *sc, struct sk_buff_head *rx_q);

static int ssv6xxx_push_log_to_circbuf(struct ssv_dbg_log *dbg_log, const char *src, unsigned int len) 
{
    int i;
    char *p = dbg_log->tail;

    for (i = 0; i < len; i++) {
        *p++ = src[i];
        if (p == dbg_log->end) {
            p = dbg_log->data;
        }
        /* Make sure pushing too much data just replaces old data */
        if (dbg_log->size < dbg_log->totalsize) {
            dbg_log->size++;
        } else {
            dbg_log->top++;
            if (dbg_log->top == dbg_log->end) {
                dbg_log->top = dbg_log->data;
            }
        }
    }

    dbg_log->tail = p;
    return len;
}

void dbgprint(struct ssv_cmd_data *cmd_data, u32 log_ctrl, u32 log_id, const char *fmt,...)
{
    char tbuf[32], log[256], buf[300];
    va_list args;
    int buf_len = 0, log_len = 0;
    unsigned long long t;
    unsigned long nanosec_rem;

    if (log_ctrl & log_id){
        if (!cmd_data->log_to_ram) {
            va_start(args, fmt);
            vprintk(fmt, args);
            va_end(args);
        } else {
            va_start(args, fmt);
            log_len = vsnprintf(log, sizeof(log)-1, fmt, args);
            va_end(args);
            if (log_len == (sizeof(log) - 1))
                printk("%s(): log message is too long\n", __FUNCTION__);
            log[log_len] = '\0';    

            /* add current time stamp */
            t = cpu_clock(UINT_MAX);
            nanosec_rem = do_div(t, 1000000000);
            sprintf(tbuf, "[%5lu.%06lu]", (unsigned long)t, nanosec_rem / 1000);
            buf_len = sprintf(buf, "%s %s", tbuf, log);
            if ((cmd_data->dbg_log.data != NULL) && (cmd_data->dbg_log.totalsize != 0)) { 
                ssv6xxx_push_log_to_circbuf(&cmd_data->dbg_log, buf, buf_len);
            }
        }
    }
}

void ssv6xxx_hci_dbgprint(void *argc, u32 log_id, const char *fmt,...)
{

    struct ssv_softc *sc = (struct ssv_softc *)argc;
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;
    char log[256];
    va_list args;
    int log_len = 0;

    if (sc->log_ctrl & log_id) {
        va_start(args, fmt);
        log_len = vsnprintf(log, sizeof(log)-1, fmt, args);
        va_end(args);
        if (log_len == (sizeof(log) - 1))
            printk("%s(): log message is too long\n", __FUNCTION__);
        log[log_len] = '\0';    
        dbgprint(cmd_data, sc->log_ctrl, log_id, "%s", log);

    }
}

#if 1
void _ssv6xxx_hexdump(const char *title, const u8 *buf,
        size_t len)
{
    size_t i;
    if (buf == NULL) {
        printk(" [NULL]");
    }else{
        printk(KERN_CONT"%s - hexdump(len=%lu):\n", title, (unsigned long) len);
        for (i = 0; i < len; i++){

            printk(KERN_CONT" %02x", buf[i]);
            if(((i+1)%16) ==0)
                printk(KERN_CONT"\n");
        }
    }
    printk(KERN_CONT"\n-----------------------------\n");
}
#endif


void ssv6xxx_txbuf_free_skb(struct sk_buff *skb, void *args)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
    struct ssv_softc *sc = (struct ssv_softc *)args;
#endif

    if (!skb)
        return;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
    ieee80211_free_txskb(sc->hw, skb);
#else
    dev_kfree_skb_any(skb);
#endif

}

int ssv6xxx_frame_hdrlen(struct ieee80211_hdr *hdr, bool is_ht)
{
#define CTRL_FRAME_INDEX(fc) ((hdr->frame_control-IEEE80211_STYPE_BACK_REQ)>>4)
    /* Control Length: BAR, BA, PS-Poll, RTS, CTS, ACK, CF-End, CF-End+CF-Ack */
    u16 fc, CTRL_FLEN[]= { 16, 16, 16, 16, 10, 10, 16, 16 };
    int hdr_len = 24;

    fc = hdr->frame_control;
    if (ieee80211_is_ctl(fc))
        hdr_len = CTRL_FLEN[CTRL_FRAME_INDEX(fc)];
    else if (ieee80211_is_mgmt(fc)) {
        if (ieee80211_has_order(fc))
            hdr_len += ((is_ht==1)? 4: 0);
    }
    else {
        if (ieee80211_has_a4(fc))
            hdr_len += 6;
        if (ieee80211_is_data_qos(fc)) {
            hdr_len += 2;
            if (ieee80211_has_order(hdr->frame_control) &&
                    is_ht==true)
                hdr_len += 4;
        }
    }


    return hdr_len;
}

/*
 * rix - rate index
 * pktlen - total bytes (delims + data + fcs + pads + pad delims)
 * width  - 0 for 20 MHz, 1 for 40 MHz
 * half_gi - to use 4us v/s 3.6 us for symbol time
 */
u32 ssv6xxx_ht_txtime(u8 rix, int pktlen, int width, 
        int half_gi, bool is_gf)
{
    u32 nbits, nsymbits, duration, nsymbols;
    int streams;

    /* find number of symbols: PLCP + data */
    streams = 1; /* we only support 1 spatial stream */
    nbits = (pktlen << 3) + OFDM_PLCP_BITS;
    nsymbits = bits_per_symbol[rix % 8][width] * streams;
    nsymbols = (nbits + nsymbits - 1) / nsymbits;

    if (!half_gi)
        duration = SYMBOL_TIME(nsymbols);
    else
    {
        if (!is_gf)
            duration = DIV_ROUND_UP(SYMBOL_TIME_HALFGI(nsymbols), 4)<<2;
        else
            duration = SYMBOL_TIME_HALFGI(nsymbols);
    }

    /* addup duration for legacy/ht training and signal fields */
    duration += L_STF + L_LTF + L_SIG + HT_SIG + HT_STF + HT_LTF(streams)+HT_SIGNAL_EXT;

    if (is_gf)
        duration -=12;

    duration += HT_SIFS_TIME;

    return duration;
}



u32 ssv6xxx_non_ht_txtime(u8 phy, int kbps,
        u32 frameLen, bool shortPreamble)
{
    u32 bits_per_symbol, num_bits, num_symbols; 
    u32 phy_time, tx_time;

    if (kbps == 0)
        return 0;

    switch (phy) {
        case WLAN_RC_PHY_CCK:
            phy_time = CCK_PREAMBLE_BITS + CCK_PLCP_BITS;
            if (shortPreamble)
                phy_time >>= 1;
            num_bits = frameLen << 3;
            tx_time = CCK_SIFS_TIME + phy_time + ((num_bits * 1000) / kbps);
            break;
        case WLAN_RC_PHY_OFDM:
            bits_per_symbol = (kbps * OFDM_SYMBOL_TIME) / 1000;
            num_bits = OFDM_PLCP_BITS + (frameLen << 3);
            num_symbols = DIV_ROUND_UP(num_bits, bits_per_symbol);
            tx_time = OFDM_SIFS_TIME + OFDM_PREAMBLE_TIME
                + (num_symbols * OFDM_SYMBOL_TIME);
            break;
        default:
            printk("Unknown phy %u\n", phy);
            BUG_ON(1);
            tx_time = 0;
            break;
    }

    return tx_time;
}

static void hw_crypto_key_clear(struct ieee80211_hw *hw, int index, struct ieee80211_key_conf *key,
        struct ssv_vif_priv_data *vif_priv, struct ssv_sta_priv_data *sta_priv)
{
    if ((index < 0) || (index >= 4))
        return;
    
    /*reset group key index*/
    if (index > 0)
    {
        if (vif_priv)
            vif_priv->group_key_idx = 0;
        if (sta_priv)
            sta_priv->group_key_idx = 0;
    }

}

void _set_wep_sw_crypto_key (struct ssv_softc *sc,
        struct ssv_vif_info *vif_info,
        struct ssv_sta_info *sta_info,
        void *param)
{
    struct ssv_sta_priv_data *sta_priv = (struct ssv_sta_priv_data *)sta_info->sta->drv_priv;
    struct ssv_vif_priv_data *vif_priv = (struct ssv_vif_priv_data *)vif_info->vif->drv_priv;

    sta_priv->has_hw_encrypt = vif_priv->has_hw_encrypt;
    sta_priv->has_hw_decrypt = vif_priv->has_hw_decrypt;
    sta_priv->need_sw_encrypt = vif_priv->need_sw_encrypt;
    sta_priv->need_sw_decrypt = vif_priv->need_sw_decrypt;
} // end of - _set_wep_sw_crypto_key -

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,0)
static enum sec_type_en _prepare_key (struct ieee80211_key_conf *key, struct ssv_softc *sc)
{
    enum sec_type_en cipher;

    switch (key->cipher) {
        case WLAN_CIPHER_SUITE_WEP40:
            cipher = SECURITY_WEP40;
            break;
        case WLAN_CIPHER_SUITE_WEP104:
            cipher = SECURITY_WEP104;
            break;
        case WLAN_CIPHER_SUITE_TKIP:
            if (USE_MAC80211_CIPHER(sc->sh))
                key->flags |= IEEE80211_KEY_FLAG_GENERATE_MMIC;
            cipher = SECURITY_TKIP;
            break;
        case WLAN_CIPHER_SUITE_CCMP:
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
            if(sc->ccmp_h_sel)
                key->flags |= (IEEE80211_KEY_FLAG_SW_MGMT | IEEE80211_KEY_FLAG_GENERATE_IV);
            else
                key->flags |= IEEE80211_KEY_FLAG_SW_MGMT;
#else
            if(sc->ccmp_h_sel)
                key->flags |= (IEEE80211_KEY_FLAG_SW_MGMT_TX | IEEE80211_KEY_FLAG_RX_MGMT | IEEE80211_KEY_FLAG_GENERATE_IV);
            else
                key->flags |= (IEEE80211_KEY_FLAG_SW_MGMT_TX | IEEE80211_KEY_FLAG_RX_MGMT);
#endif
            if (USE_MAC80211_CIPHER(sc->sh))
                key->flags |= IEEE80211_KEY_FLAG_GENERATE_MMIC; 
            cipher = SECURITY_CCMP;
            break;
        default:
            cipher = SECURITY_CIPHER_INVALID;
            break;
    }

    return cipher;
} // end of - _prepare_key -
#else
static enum sec_type_en _prepare_key (struct ieee80211_key_conf *key, struct ssv_softc *sc)
{
    enum sec_type_en cipher;

    switch (key->alg) {
        case ALG_WEP:
            if(key->keylen == 5) // 40 bits / 8 bit_per_byte = 5 bytes
                cipher = SECURITY_WEP40;
            else
                cipher = SECURITY_WEP104;
            break;
        case ALG_TKIP:
            cipher = SECURITY_TKIP;
            key->flags |= IEEE80211_KEY_FLAG_GENERATE_MMIC;
            break;
        case ALG_CCMP:
            cipher = SECURITY_CCMP;
            key->flags |= IEEE80211_KEY_FLAG_SW_MGMT;
            break;
        default:
            cipher = SECURITY_CIPHER_INVALID;
            break;
    }

    return cipher;
} // end of - _prepare_key -
#endif // LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,0)

int _set_key_wep (struct ssv_softc *sc, struct ssv_vif_priv_data *vif_priv,
        struct ssv_sta_priv_data *sta_priv, enum sec_type_en cipher,
        struct ieee80211_key_conf *key)
{
    int                            ret = 0;
    struct ssv_vif_info           *vif_info = &sc->vif_info[vif_priv->vif_idx];

    printk(KERN_ERR "Set WEP %02X %02X %02X %02X %02X %02X %02X %02X... (%d %d)\n",
            key->key[0], key->key[1], key->key[2], key->key[3],
            key->key[4], key->key[5], key->key[6], key->key[7],
            key->keyidx, key->keylen);    

    if ( SSV_WEP_USE_HW_CIPHER(sc, vif_priv))
    {
        vif_priv->has_hw_decrypt = true;
        vif_priv->has_hw_encrypt = true;//(vif_priv->force_sw_encrypt == false);
        vif_priv->need_sw_decrypt = false;
        vif_priv->need_sw_encrypt = false;
        vif_priv->use_mac80211_decrypt = false;
    } else {
        if (USE_MAC80211_CIPHER(sc->sh)){

            vif_priv->need_sw_encrypt = false;
            vif_priv->need_sw_decrypt = false;

            if (USE_MAC80211_RX(sc->sh)) {
                vif_priv->has_hw_decrypt = false;
                vif_priv->use_mac80211_decrypt = true;                        
                ssv6xxx_foreach_vif_sta(sc, vif_info, _set_wep_sw_crypto_key, NULL);
            } else {
                vif_priv->has_hw_decrypt = true;
                vif_priv->use_mac80211_decrypt = false;
            }
            vif_priv->has_hw_encrypt = false;
            dev_info(sc->dev, "[Local Crypto]: Use MAC80211's encrypter.\n");
            ret = -EOPNOTSUPP;                 
        } else {
            vif_priv->has_hw_decrypt = false;
            vif_priv->has_hw_encrypt = false;
            vif_priv->need_sw_decrypt = false;
            vif_priv->need_sw_encrypt = false;
            vif_priv->use_mac80211_decrypt = true;

            ssv6xxx_foreach_vif_sta(sc, vif_info, _set_wep_sw_crypto_key, NULL);

            ret = -EOPNOTSUPP;
        }
    }

    vif_priv->pair_cipher = vif_priv->group_cipher = cipher;
    vif_priv->wep_cipher = cipher;
    vif_priv->wep_idx = key->keyidx; 
    SSV_SET_WEP_KEY(sc, vif_priv, sta_priv, cipher, key);

    vif_priv->is_security_valid = true;

    return ret;
} // end of - _set_key_wep -

static int _set_pairwise_key_tkip_ccmp (struct ssv_softc *sc, struct ssv_vif_priv_data *vif_priv,
        struct ssv_sta_priv_data *sta_priv, enum sec_type_en cipher,
        struct ieee80211_key_conf *key)
{
    int ret = 0;
    const char *cipher_name = (cipher == SECURITY_CCMP) ? "CCMP" : "TKIP";

    if (sta_priv == NULL) {
        dev_err(sc->dev, "Setting pairwise TKIP/CCMP key to NULL STA.\n");
        return -EOPNOTSUPP;
    }

    // this is a routine for judge hw can use hw cipher for wpa.
    if (SSV_PAIRWISE_WPA_USE_HW_CIPHER( sc, vif_priv, cipher, sta_priv))
    {
        sta_priv->has_hw_decrypt = true;
        sta_priv->need_sw_decrypt = false;
        sta_priv->use_mac80211_decrypt = false;

        if (SSV_USE_HW_ENCRYPT(cipher, sc, sta_priv, vif_priv))
        {
            dev_info(sc->dev, "STA %d uses HW encrypter for pairwise.\n", sta_priv->sta_idx);
            sta_priv->has_hw_encrypt = true;
            sta_priv->need_sw_encrypt = false;
            sta_priv->use_mac80211_decrypt = false;
            ret = 0;
        }
        else
        {
            sta_priv->has_hw_encrypt = false;
            sta_priv->need_sw_encrypt = false;
            sta_priv->use_mac80211_decrypt = true;
            ret = -EOPNOTSUPP;
        }
    } else {
        if ( USE_MAC80211_CIPHER(sc->sh) ){
            vif_priv->need_sw_encrypt = false;
            vif_priv->need_sw_decrypt = false;

            if (USE_MAC80211_RX(sc->sh)) {
                vif_priv->has_hw_decrypt = false;
                vif_priv->use_mac80211_decrypt = true;

            } else {
                vif_priv->has_hw_decrypt = true;
                vif_priv->use_mac80211_decrypt = false;
            }             
            vif_priv->has_hw_encrypt = false;
            dev_info(sc->dev, "[Local Crypto]: Use MAC80211's encrypter.\n");               
            ret = -EOPNOTSUPP;                

        } else
        {
            sta_priv->has_hw_encrypt = false;
            sta_priv->has_hw_decrypt = false;
            dev_err(sc->dev, "STA %d MAC80211's %s cipher.\n", sta_priv->sta_idx, cipher_name);
            sta_priv->need_sw_encrypt = false;
            sta_priv->need_sw_decrypt = false;
            sta_priv->use_mac80211_decrypt = true;
            ret = -EOPNOTSUPP;
        }
    }

    if (sta_priv->has_hw_encrypt || sta_priv->has_hw_decrypt)
    {
        SSV_SET_PAIRWISE_CIPHER_TYPE(sc->sh, cipher, sc->sta_info[sta_priv->sta_idx].hw_wsid);/* sta_info is already protected by ssv6200_set_key(). */
        SSV_WRITE_PAIRWISE_KEY_TO_HW(sc, key->keyidx, cipher,
                key->key, key->keylen, key,
                vif_priv, sta_priv);
    }

    if (   (vif_priv->has_hw_encrypt || vif_priv->has_hw_decrypt)
            && (vif_priv->group_key_idx > 0))
    {
        SSV_SET_AES_TKIP_HW_CRYPTO_GROUP_KEY(sc, &sc->vif_info[vif_priv->vif_idx],
                &sc->sta_info[sta_priv->sta_idx], &vif_priv->group_key_idx);       
    }
    return ret;
} // end of - _set_pairwise_key_tkip_ccmp -

static int _set_group_key_tkip_ccmp (struct ssv_softc *sc, struct ssv_vif_priv_data *vif_priv,
        struct ssv_sta_priv_data *sta_priv, enum sec_type_en cipher,
        struct ieee80211_key_conf *key)
{
    int ret = 0;
    const char *cipher_name = (cipher == SECURITY_CCMP) ? "CCMP" : "TKIP";

    vif_priv->group_cipher = cipher;

    if (SSV_GROUP_WPA_USE_HW_CIPHER( sc, vif_priv, cipher))
    {
        // Hardware encryption is only supported for MPDU. AMPDU connection requires
        // software encryption solution.
        dev_info(sc->dev, "VIF %d uses HW %s cipher for group.\n", vif_priv->vif_idx, cipher_name);
#ifdef USE_MAC80211_DECRYPT_BROADCAST
        vif_priv->has_hw_decrypt = false;
        ret = -EOPNOTSUPP;
#else
        vif_priv->has_hw_decrypt = true;
#endif // USE_MAC80211_DECRYPT_BROADCAST
        vif_priv->has_hw_encrypt = true;
        vif_priv->need_sw_decrypt = false;
        vif_priv->need_sw_encrypt = false;
        vif_priv->use_mac80211_decrypt = false;
    }
    else {

        if (USE_MAC80211_CIPHER(sc->sh) ){
            vif_priv->need_sw_encrypt = false;
            vif_priv->need_sw_decrypt = false;
            if (USE_MAC80211_RX(sc->sh)) {
                vif_priv->has_hw_decrypt = false;
                vif_priv->use_mac80211_decrypt = true;

            } else {
                vif_priv->has_hw_decrypt = true;
                vif_priv->use_mac80211_decrypt = false;
            }            
            vif_priv->has_hw_encrypt = false;
            dev_info(sc->dev, "[Local Crypto]: Use MAC80211's encrypter.\n");               
            ret = -EOPNOTSUPP;

        } else
        {
            vif_priv->has_hw_decrypt = false;
            vif_priv->has_hw_encrypt = false;
            dev_err(sc->dev, "VIF %d uses MAC80211's %s cipher.\n", vif_priv->vif_idx, cipher_name);
            vif_priv->need_sw_encrypt = false;
            vif_priv->need_sw_decrypt = false;
            vif_priv->use_mac80211_decrypt = true;
            ret = -EOPNOTSUPP;
        }
    }

    if (vif_priv->has_hw_encrypt || vif_priv->has_hw_decrypt)
    {
        int cipher_type;

#ifdef USE_MAC80211_DECRYPT_BROADCAST
        cipher_type = SECURITY_NONE;
#else
        cipher_type = cipher;
#endif // USE_MAC80211_DECRYPT_BROADCAST

        SSV_SET_GROUP_CIPHER_TYPE(sc->sh, vif_priv, cipher_type);

        key->hw_key_idx = key->keyidx;

        SSV_WRITE_GROUP_KEY_TO_HW(sc, key->keyidx, cipher,
                key->key, key->keylen, key,
                vif_priv, sta_priv);
    }

    vif_priv->is_security_valid = true;

    SSV_CHK_DUAL_VIF_CHG_RX_FLOW( sc, vif_priv);
    return ret;
} // end of - _set_group_key_tkip_ccmp -

static int _set_key_tkip_ccmp (struct ssv_softc *sc, struct ssv_vif_priv_data *vif_priv,
        struct ssv_sta_priv_data *sta_priv, enum sec_type_en cipher,
        struct ieee80211_key_conf *key)
{
    if (key->keyidx == 0) {
        if (sta_priv->last_pn != 0)
            sta_priv->unicast_key_changed = true;
        return _set_pairwise_key_tkip_ccmp(sc, vif_priv, sta_priv, cipher, key);
    } else {
        if (vif_priv->last_pn_mcast != 0)
            vif_priv->group_key_changed = true;
        return _set_group_key_tkip_ccmp(sc, vif_priv, sta_priv, cipher, key);
    }        
} // end of - _set_key_tkip_ccmp -

/*
 * The difference of Set key flow between STA and AP mode:
 * STA:              AP:
 * add_vif           add_vif
 * add_sta           set_key (group)
 * set_key(pairwise) add_sta
 * set_key(group)    set_key (pairwise)
 */
static int ssv6200_set_key(struct ieee80211_hw *hw,
        enum set_key_cmd cmd,
        struct ieee80211_vif *vif,
        struct ieee80211_sta *sta,
        struct ieee80211_key_conf *key)
{
    struct ssv_softc         *sc = hw->priv;
    int                       ret = 0;
    enum sec_type_en         cipher = SECURITY_NONE;
    int                       sta_idx = (-1);
    struct ssv_sta_info      *sta_info = NULL;/* For mac80211 cb, we use sc->mutex to protect sta_info instead of sta_info_sem. */
    struct ssv_sta_priv_data *sta_priv = NULL;
    struct ssv_vif_priv_data *vif_priv = (struct ssv_vif_priv_data *)vif->drv_priv;
    struct ssv_vif_info *vif_info = &sc->vif_info[vif_priv->vif_idx];
	
    mutex_lock(&sc->mutex);
 
    if ((vif_info->if_type != NL80211_IFTYPE_AP) && (vif_info->if_type != NL80211_IFTYPE_P2P_GO) && (NULL==sta) && 
	    ((key->keyidx == 0) && (key->cipher != WLAN_CIPHER_SUITE_WEP40) &&  (key->cipher != WLAN_CIPHER_SUITE_WEP104)))
    {
        printk("Warning: ssv6200_set_key return; key->cipher=0x%x\r\n", key->cipher);
        mutex_unlock(&sc->mutex);
        return 0;    
    }

    if (sta)
    {
        sta_priv = (struct ssv_sta_priv_data *)sta->drv_priv;
        if(sta_priv->sta_idx == -1)
        {
            dev_warn(sc->dev, "%s(): sta_info is gone.\n", __func__);
            if(!(sc->sc_flags & SC_OP_HW_RESET))
            {
                ret = -ENODATA;
            }
            goto out;
        }
        sta_idx = sta_priv->sta_idx;
        //down_read(&sc->sta_info_sem);/* For mac80211 cb, we use sc->mutex to protect sta_info instead of sta_info_sem. */
        sta_info = &sc->sta_info[sta_idx];
        if ((sta_info->s_flags & STA_FLAG_VALID) == 0) {
            dev_warn(sc->dev, "%s(): sta_info is gone.\n", __func__);
            ret = -ENODATA;
            goto out;
        }
    }

    BUG_ON((cmd!=SET_KEY) && (cmd!=DISABLE_KEY));

    if (!(sc->sh->cfg.hw_caps & SSV6200_HW_CAP_SECURITY))
    {
        dev_warn(sc->dev, "HW does not support security.\n");
        ret = -EOPNOTSUPP;
        goto out;
    }

    /* hw security just support in valid wsid */
    if (sta_info && (sta_info->hw_wsid == (-1)))
    {
        dev_warn(sc->dev, "Add STA without HW resource. Use MAC80211's solution.\n");
        ret = -EOPNOTSUPP;
        goto out;
    }

    cipher = _prepare_key(key, sc);

    dev_err(sc->dev,"Set key VIF %d VIF type %d STA %d algorithm = %d, key->keyidx = %d, cmd = %d\n",
            vif_priv->vif_idx, vif->type, sta_idx, cipher, key->keyidx, cmd);

    if (cipher == SECURITY_CIPHER_INVALID)
    {
        dev_warn(sc->dev, "Unsupported cipher type.\n");
        ret = -EOPNOTSUPP;
        goto out;
    }

    switch (cmd)
    {
        case SET_KEY:
            {
                // Debug code
#if 0
                int i;
                printk("================================SET KEY=======================================\n");

                if (sta_info == NULL)
                {
                    printk("NULL STA cmd[%d] alg[%d] keyidx[%d] ", cmd, algorithm, key->keyidx);                
                }
                else
                {
                    printk("STA WSID[%d] cmd[%d] alg[%d] keyidx[%d] ", sta_info->hw_wsid, cmd, algorithm, key->keyidx);                
                }

                printk("SET_KEY index[%d] flags[0x%x] algorithm[%d] key->keylen[%d]\n",
                        key->keyidx, key->flags, algorithm, key->keylen);                
                for(i = 0; i < key->keylen; i++)
                {
                    printk("[%02x]", key->key[i]);
                }
                printk("\n");
                printk("===============================================================================\n");
#endif // 0
                // dev_info(sc->dev, "Set key %d VIF %p, STA %p\n", key->keyidx, vif, sta);
                switch (cipher)
                {
                    case SECURITY_WEP40:
                    case SECURITY_WEP104:
                        ret = _set_key_wep(sc, vif_priv, sta_priv, cipher, key);
                        key->hw_key_idx = key->keyidx;
                        break;
                    case SECURITY_TKIP:
                    case SECURITY_CCMP:
                        ret = _set_key_tkip_ccmp(sc, vif_priv, sta_priv, cipher, key);
                        break;
                    default:
                        break;
                }
                if (sta){
                    struct ssv_sta_priv_data *first_sta_priv = 
                        list_first_entry(&vif_priv->sta_list, struct ssv_sta_priv_data, list);
                    if (first_sta_priv->sta_idx == sta_priv->sta_idx){
                        vif_priv->pair_cipher = cipher;      // keep first station cipher mode for TDLS check
                    }
                }
            }
            break;
        case DISABLE_KEY:
            {
                int  another_vif_idx = ((vif_priv->vif_idx + 1) % 2);
                struct ssv_vif_priv_data *another_vif_priv =
                    (struct ssv_vif_priv_data *)sc->vif_info[another_vif_idx].vif_priv;
#if 0
                printk("================================DEL KEY=======================================\n");
                if(sta_info == NULL){    
                    printk("NULL STA cmd[%d] alg[%d] keyidx[%d] ", cmd, cipher, key->keyidx);                
                }
                else{
                    printk("STA WSID[%d] cmd[%d] alg[%d] keyidx[%d] ", sta_info->hw_wsid, cmd, cipher, key->keyidx);
                }

                printk("DISABLE_KEY index[%d]\n",key->keyidx);
                printk("==============================================================================\n");
#endif
#if 0
                if(key->keyidx == 0)
                {
                    sta_info->ampdu_ccmp_encrypt = false;
                }
#endif // 0


                if (another_vif_priv != NULL) { // for dual vif 
                    SSV_RESTORE_RX_FLOW(sc, vif_priv, sta);
                }

                // clear cipher type.
                if ( sta == NULL){
                    vif_priv->group_cipher = SECURITY_NONE;
                    if ((another_vif_priv == NULL) 
                            || ((another_vif_priv != NULL) && (!SSV6XXX_USE_HW_DECRYPT(another_vif_priv)))){
                        SSV_SET_GROUP_CIPHER_TYPE(sc->sh, vif_priv, SECURITY_NONE);
                    }  
                } else {

                    if ((vif_info->if_type != NL80211_IFTYPE_AP) && (vif_info->if_type != NL80211_IFTYPE_P2P_GO) && (another_vif_priv == NULL)){
                        struct ssv_sta_priv_data *first_sta_priv = 
                            list_first_entry(&vif_priv->sta_list, struct ssv_sta_priv_data, list);
                        if (sta_priv == first_sta_priv){
                            SSV_SET_PAIRWISE_CIPHER_TYPE(sc->sh, SECURITY_NONE, sta_info->hw_wsid);
                        }
                    }  
                    //vif_priv->pair_cipher = SECURITY_NONE; // should not clear cipher type, since other sta may stay in security mode
                }

                if ((cipher == SECURITY_WEP40) || (cipher == SECURITY_WEP104))
                {                    
                    vif_priv->wep_idx = -1;
                    vif_priv->wep_cipher = -1;
                }

                if ((cipher == SECURITY_TKIP) || (cipher == SECURITY_CCMP))
                {
                    printk(KERN_ERR "Clear key %d VIF %d, STA %d\n",
                            key->keyidx, (vif != NULL), (sta != NULL));
                    hw_crypto_key_clear(hw, key->keyidx, key, vif_priv, sta_priv);
                }
                // if (key->keyidx == 0)
                {
                    if ((key->keyidx == 0) && (sta_priv != NULL))
                    {
                        sta_priv->has_hw_decrypt = false;
                        sta_priv->has_hw_encrypt = false;
                        sta_priv->need_sw_encrypt = false;
                        sta_priv->use_mac80211_decrypt = false;

                    }

                    if ((vif_priv->is_security_valid) && (key->keyidx != 0))
                    {
                        vif_priv->is_security_valid = false;
                    }
                }
                ret = 0;
            }
            break;
        default:
            ret = -EINVAL;
    }

    if(sta_priv != NULL)
    {
        printk("sta: hw_en:%d, sw_en:%d, hw_de:%d, sw_de:%d,\n", 
                (sta_priv->has_hw_encrypt==true),(sta_priv->need_sw_encrypt==true),
                (sta_priv->has_hw_decrypt==true),(sta_priv->need_sw_decrypt==true));
    }

    if(vif_priv)
    {
        printk("vif: hw_en:%d, sw_en:%d, hw_de:%d, sw_de:%d, valid:%d\n", 
                (vif_priv->has_hw_encrypt==true),(vif_priv->need_sw_encrypt==true),
                (vif_priv->has_hw_decrypt==true),(vif_priv->need_sw_decrypt==true), (vif_priv->is_security_valid==true));
    }


out:
    //if (sta) {
    //up_read(&sc->sta_info_sem);/* For mac80211 cb, we use sc->mutex to protect sta_info instead of sta_info_sem. */
    //}
    mutex_unlock(&sc->mutex);
    printk(KERN_ERR "SET KEY %d\n", ret);
    return ret;
}

static void _ssv6xxx_get_current_rate(struct ssv_softc *sc, struct ieee80211_tx_info *tx_info, 
        struct sk_buff *skb, int wsid)
{
    if (wsid >= SSV_NUM_STA) {
        return;
    }

    if (sc->sta_info[wsid].s_flags & STA_FLAG_VALID)
        SSV_RC_MAC80211_TX_RATE_IDX(sc, (int)sc->hw_cur_rate[wsid], tx_info);
}

void ssvxxx_complete_tx_skb(struct ssv_softc *sc, struct ieee80211_tx_info *tx_info, 
        struct sk_buff *skb, int wsid)
{
    ieee80211_tx_info_clear_status(tx_info);
    tx_info->flags |= IEEE80211_TX_STAT_ACK;
    tx_info->status.ack_signal = 100; /* ???? */
    tx_info->status.rates[1].idx = -1;
    _ssv6xxx_get_current_rate(sc, tx_info, skb, wsid);
    ieee80211_tx_status(sc->hw, skb);
}

static void ssv6xxx_setup_ampdu_session(void *priv, struct ieee80211_sta *sta, struct sk_buff *skb)
{
    struct ssv_softc *sc = (struct ssv_softc *)priv;
    struct ssv_sta_priv_data *ssv_sta_priv = NULL;
    struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
    struct ieee80211_hdr *hdr = (struct ieee80211_hdr *)skb->data;
    u8 *skb_qos_ctl = ieee80211_get_qos_ctl(hdr);
    u8 tid_no = skb_qos_ctl[0] & 0xf;

    ssv_sta_priv = (struct ssv_sta_priv_data *)sta->drv_priv;
    if (ssv_sta_priv == NULL)
        return;

    if (!ieee80211_is_data_qos(hdr->frame_control))
        return;

    if ((sta->ht_cap.ht_supported == false) || (info->flags & IEEE80211_TX_CTL_AMPDU))
        return;

    if ((!(skb->protocol == cpu_to_be16(ETH_P_PAE))) && 
            conf_is_ht(&sc->hw->conf)) {


        if (skb_get_queue_mapping(skb) != IEEE80211_AC_VO) { // Not VO AC
            if ((sta->ht_cap.ht_supported == true) && 
                    (ssv_sta_priv->ampdu_tid_state[tid_no] == SSV_AMPDU_1_3_TX_STOP)) {

                //spin_lock_bh(&ssv_sta_priv->ampdu_ctrl_lock);
                ssv_sta_priv->ampdu_tid_state[tid_no] = SSV_AMPDU_1_3_TX_PROGRESS;
                //spin_unlock_bh(&ssv_sta_priv->ampdu_ctrl_lock);
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,32)
                ieee80211_start_tx_ba_session(sc->hw, (u8*)(sta->addr), (u16)tid_no);
#elif LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,37)
                ieee80211_start_tx_ba_session(sta, tid_no);
#else
                ieee80211_start_tx_ba_session(sta, tid_no, 0);
#endif
            }
        }
    }
}

void ssv6xxx_tx_done_process(struct work_struct *work)
{
    struct ssv_softc *sc = container_of(work, struct ssv_softc, tx_done_work);
    struct ieee80211_tx_info *tx_info;
    struct sk_buff *skb;
    struct ieee80211_hdr *hdr;
    struct ieee80211_sta *sta = NULL;
    int reason = -1;
    int wsid = -1;

    while ((skb = skb_dequeue(&sc->tx_done_q)))
    {
        struct SKB_info_st *mpdu_skb_info_p = (SKB_info *)(skb->head);

        if (SSV_GET_TX_DESC_CTYPE(sc->sh, skb) != M2_TXREQ)
        {
            ssv_skb_free(sc, skb);
            continue;
        }

        /* filter out self-generate nullfun frame */
        if (SSV_NULLFUN_FRAME_FILTER(sc->sh, skb)) {
            ssv_skb_free(sc, skb);
            continue;
        }
        
        if (!(sc->sc_flags & SC_OP_IF_UP))
        {
            ssv_skb_free(sc, skb);
            continue;
        }


        if (true == mpdu_skb_info_p->raw_data)
        {
            ssv_skb_free(sc, skb);
            continue;
        }

        reason = HAL_GET_TX_DESC_REASON(sc->sh, skb);
        if (reason == ID_TRAP_SW_TXTPUT)
        {
            ssv_skb_free(sc, skb);
            continue;
        }

        wsid = HAL_GET_TX_DESC_WSID(sc->sh, skb);
        tx_info = IEEE80211_SKB_CB(skb);
        skb_pull(skb, SSV_GET_TX_DESC_SIZE(sc->sh));

        /* update ampdu state */
        if (wsid <= SSV_SW_TXQ_ID_STAMAX)
            HCI_TXQ_LOCK_BY_STA(sc->sh, wsid);
        
        sta = (struct ieee80211_sta *)mpdu_skb_info_p->sta;
        hdr = (struct ieee80211_hdr *)skb->data;
        if (sta) {
            if (ssv6xxx_compare_ether_addr(hdr->addr1, sta->addr)) {
				if (wsid <= SSV_SW_TXQ_ID_STAMAX){
                HCI_TXQ_UNLOCK_BY_STA(sc->sh, wsid);}
                ssv_skb_free(sc, skb);
                continue;
            } else {
                ssv6xxx_setup_ampdu_session((void *)sc, sta, skb);
            }
        }
        if (wsid <= SSV_SW_TXQ_ID_STAMAX)
            HCI_TXQ_UNLOCK_BY_STA(sc->sh, wsid);
        
        if (mpdu_skb_info_p->directly_ack) {
            ssv_skb_free(sc, skb);
        } else {
            ssvxxx_complete_tx_skb(sc, tx_info, skb, wsid);
        }
        ssv6200_tx_flow_control(sc, false, false);
    }
}

void ssv6xxx_post_tx_cb(struct sk_buff_head *skb_head, void *args)
{
    struct ssv_softc *sc=(struct ssv_softc *)args;
    struct ssv6xxx_hci_ctrl *hci_ctrl = sc->sh->hci.hci_ctrl;
    struct sk_buff *skb;
    bool tx_done = false;
    
    /* the function should not drop frame
     * it just distinguish frame type to wakeup different task*/
    while ((skb=skb_dequeue(skb_head)))
    {
        atomic_dec(&hci_ctrl->sw_txq_cnt);
        if (SSV_GET_SW_ACK_CTL(sc->sh, skb)) {
            skb_queue_tail(&sc->tx_ack_ctl_q, skb);
        } else {
            skb_queue_tail(&sc->tx_done_q, skb);
            tx_done = true;
        }
    }
    // start workqueue to handle tx done    
    if (tx_done) 
        queue_work(sc->tx_done_wq, &sc->tx_done_work);
}

void ssv6xxx_hci_update_flowctl_cb(void *args)
{
    struct ssv_softc *sc=(struct ssv_softc *)args;
    ssv6200_tx_flow_control(sc, false, false);
}

static bool ssv6xxx_special_tx_frame(struct sk_buff *skb)
{
#define SSV_SNAP_SIZE   6
#define SSV_PROTOC_TYPE_SIZE    2

    u8 mac_hdr_len = ieee80211_get_hdrlen_from_skb(skb);
    u8 offset;
    u16 ether_type;
    const struct iphdr *ip;
    struct udphdr *udp;
    bool retval = false;

    offset = mac_hdr_len + SSV_SNAP_SIZE;
    ether_type = be16_to_cpup((__be16 *)(skb->data + offset));

    switch (ether_type) {
        case ETH_P_PAE:
        case ETH_P_ARP:
            retval = true;
            break;
        case ETH_P_IP:
            ip = (struct iphdr *)((u8 *)skb->data + offset + SSV_PROTOC_TYPE_SIZE);
            if (IPPROTO_UDP == ip->protocol) {
                udp = (struct udphdr *)((u8 *)ip + (ip->ihl << 2));
                //DHCP, source port = 68(bootpc) & dest. port = 67(bootps)
                //      source port = 67(bootps) & dest. port = 68(bootpc)
                if (((ntohs(udp->dest) == 67) && (ntohs(udp->source) == 68)) ||
                        ((ntohs(udp->source) == 67) && (ntohs(udp->dest) == 68))) {
                    retval = true;
                }
            }
            break;
        default:
            break;
    }

    return retval;
}

static int ssv6xxx_txq_id_mapping(struct ssv_softc *sc, struct sk_buff *skb)
{
    struct ieee80211_hdr *hdr = (struct ieee80211_hdr *)skb->data;
    //struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
    struct SKB_info_st *skb_info = (struct SKB_info_st *)skb->head;
    struct ieee80211_sta *sta = NULL;
    struct ssv_sta_priv_data *ssv_sta_priv = NULL;
    int txq_idx = SSV_SW_TXQ_ID_MNG;

    /**
     * Decide frame tid & hardware output queue for outgoing
     * frames. Management frames have a dedicate output queue
     * with higher priority in station mode.
     */
    if (ieee80211_is_mgmt(hdr->frame_control) || 
             ieee80211_is_nullfunc(hdr->frame_control) ||
             ieee80211_is_qos_nullfunc(hdr->frame_control)) {
        txq_idx = SSV_SW_TXQ_ID_MNG;
    } else {
        sta = skb_info->sta;
        if (sta == NULL) {
            txq_idx = SSV_SW_TXQ_ID_MNG;
        } else {
            ssv_sta_priv = (struct ssv_sta_priv_data *)sta->drv_priv;
            if (ssv_sta_priv->sta_idx <= SSV_SW_TXQ_ID_STAMAX)
                txq_idx = ssv_sta_priv->sta_idx;
            else
                txq_idx = SSV_SW_TXQ_ID_MNG;
        }
    }

    return txq_idx;
}

static bool ssv6xxx_clear_ampdu_ctl_frame(struct sk_buff *skb)
{
    return ssv6xxx_special_tx_frame(skb);
}

static void ssv6xxx_tx_mib(struct ssv_softc *sc, struct sk_buff *skb)
{
    struct ieee80211_hdr *hdr = (struct ieee80211_hdr *)skb->data;

    sc->tx.tx_count++;
    if (ieee80211_is_data(hdr->frame_control)) {
        sc->tx.data_count++;
    } else if (ieee80211_is_mgmt(hdr->frame_control)) {
        sc->tx.mgmt_count++;
        
        if (ieee80211_is_auth(hdr->frame_control)) {
            sc->tx.auth_count++;
        } else if (ieee80211_is_deauth(hdr->frame_control)) {
            sc->tx.deauth_count++;
        } else if (ieee80211_is_assoc_req(hdr->frame_control)) {
            sc->tx.assoc_req_count++;
        } else if (ieee80211_is_assoc_resp(hdr->frame_control)) {
            sc->tx.assoc_resp_count++;
        } else if (ieee80211_is_probe_req(hdr->frame_control)) {
            sc->tx.probe_req_count++;
        } else if (ieee80211_is_probe_resp(hdr->frame_control)) {
            sc->tx.probe_resp_count++;
        }
    }
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,0,0)
static int ssv6200_tx(struct ieee80211_hw *hw, struct sk_buff *skb)
#elif LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
static void ssv6200_tx(struct ieee80211_hw *hw, struct sk_buff *skb)
#else 
static void ssv6200_tx(struct ieee80211_hw *hw, struct ieee80211_tx_control *control, struct sk_buff *skb)
#endif 
{
    struct ssv_softc *sc = (struct ssv_softc *)hw->priv;
    struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
    struct SKB_info_st *skb_info = (struct SKB_info_st *)skb->head;
    struct ieee80211_hdr *hdr = (struct ieee80211_hdr *)skb->data;
    int txq_idx = 0;
    bool unicast = false, force_trigger = false;
    int wsid = 0;

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
    memset(skb_info, 0, sizeof (struct SKB_info_st));
    skb_info->sta = info->control.sta;
#else
    memset(skb_info, 0, sizeof (struct SKB_info_st));
    skb_info->sta = control ? control->sta : NULL;
#endif
    
    ssv6xxx_tx_mib(sc, skb);
    ssv6200_tx_flow_control(sc, true, false);

    if ((sc->sc_flags & SC_OP_HW_RESET) ||
        (!(sc->sc_flags & SC_OP_IF_UP)) ||
        (sc->sh->cfg.flowctl && ((sc->flowctl_frame_cnt) > (sc->sh->cfg.flowctl_high_threshold << 3)))) {
        //printk(KERN_ERR "******** drop packet in driver **********\n");
        dev_kfree_skb_any(skb);
    } else {

        if (info->flags & IEEE80211_TX_CTL_ASSIGN_SEQ) {
            if (info->flags & IEEE80211_TX_CTL_FIRST_FRAGMENT)
                sc->tx.seq_no += 0x10;
            hdr->seq_ctrl &= cpu_to_le16(IEEE80211_SCTL_FRAG);
            hdr->seq_ctrl |= cpu_to_le16(sc->tx.seq_no);
        }

        if (info->flags & IEEE80211_TX_CTL_AMPDU) {
            if (ssv6xxx_clear_ampdu_ctl_frame(skb))    
                info->flags &= (~IEEE80211_TX_CTL_AMPDU);
        }

        txq_idx = ssv6xxx_txq_id_mapping(sc, skb);

        if (true == ssv6xxx_special_tx_frame(skb)) {
            force_trigger = true; // force trigger
        }

        // update tx desc
        SSV_ADD_TXINFO(sc, skb);

        skb_info->directly_ack = false;
        if (sc->sh->cfg.directly_ack_ctrl) {

            unicast = (is_multicast_ether_addr(hdr->addr1)) ? 0: 1;
            if (unicast &&
                ieee80211_is_data(hdr->frame_control) && 
                (!(ieee80211_is_nullfunc(hdr->frame_control)||ieee80211_is_qos_nullfunc(hdr->frame_control)))) {

                struct sk_buff  *copy_skb = NULL;

                // 1. create a copy of tx skb
                copy_skb = skb_copy(skb, GFP_ATOMIC);
                if (!copy_skb) {
                    printk("create TX skb copy failed!\n");
                    HCI_SEND(sc->sh, skb, txq_idx, force_trigger);
                } else {
                    skb_info = (struct SKB_info_st *)copy_skb->head;

                    info = IEEE80211_SKB_CB(skb);
                    wsid = HAL_GET_TX_DESC_WSID(sc->sh, skb);
                    skb_pull(skb, SSV_GET_TX_DESC_SIZE(sc->sh));
                    ssvxxx_complete_tx_skb(sc, info, skb, wsid);
                    skb_info->directly_ack = true;
                    HCI_SEND(sc->sh, copy_skb, txq_idx, force_trigger);
                }
            } else {
                HCI_SEND(sc->sh, skb, txq_idx, force_trigger);
            }

        } else {
            HCI_SEND(sc->sh, skb, txq_idx, force_trigger);
        }
    }

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,0,0)   
    //If kernel version less than 2.6.38. and got error. no need to free sk_buff in this function    
    return NETDEV_TX_OK;
#endif

}

int ssv6xxx_rx_task (void *data)
{
    struct ssv_softc *sc = (struct ssv_softc *)data;
    unsigned long     wait_period = msecs_to_jiffies(50);

    printk("SSV6XXX RX Task started.\n");
    while (!kthread_should_stop())
    {
        u32 before_timeout = (-1);

        before_timeout = wait_event_interruptible_timeout(sc->rx_wait_q,
                (   skb_queue_len(&sc->rx_skb_q)
                    || kthread_should_stop()),
                wait_period);

        if (kthread_should_stop())
        {
            printk("Quit RX task loop...\n");
            break;
        }

        // Take out RX skb from RX Q and process it.
        if (skb_queue_len(&sc->rx_skb_q)) {
            ssv6xxx_process_rx_q(sc, &sc->rx_skb_q);
        }
    }

    sc->rx_task = NULL;

    return 0;
} // end of - ssv6xxx_rx_task -

void ssv6xxx_house_keeping(unsigned long argv)
{
    struct ssv_softc *sc = (struct ssv_softc *)argv;

    /* 
     * It is necessary to wait for HW ready.
     * Otherwise, there is the conflict.
     * Ex: Register read/write is conflict with firmware download by USB.
     */
    if (!sc->mac80211_dev_started || 
            (sc->sc_flags & SC_OP_HW_RESET) ||
            (sc->sc_flags & SC_OP_BLOCK_CNTL))
        goto next_round;

    // edca wq
    if((sc->bScanning == false) && (sc->bAdapt == false))
    {
        queue_work(sc->house_keeping_wq, &sc->mib_edca_work);
    }

    // tx poll
    if (sc->sh->cfg.tx_stuck_detect) 
        queue_work(sc->house_keeping_wq, &sc->tx_poll_work);

    // flow control
    if (sc->sh->cfg.flowctl && (sc->flowctl_frame_cnt == 0))
        queue_work(sc->house_keeping_wq, &sc->flowctl_work);
    
    if (sc->sh->cfg.fw_reset) {
        queue_work(sc->house_keeping_wq, &sc->check_fw_status_work);
    }

next_round:
#ifdef CONFIG_ENABLE_HOST_THERMAL
    if (sc->thermal_monitor_enable) {
        sc->thermal_monitor_counter ++;
        if (sc->thermal_monitor_counter > HOUSE_KEEPING_10_SEC){
            queue_work(sc->house_keeping_wq, &sc->thermal_monitor_work);
            sc->thermal_monitor_counter = 0;
        }
    }
#endif
    mod_timer(&sc->house_keeping.timer, jiffies + msecs_to_jiffies(HOUSE_KEEPING_TIMEOUT));
} // end of - ssv6xxx_house_keeping -

int ssv6xxx_get_channel(struct ssv_softc *sc, int *pch)
{
    *pch = sc->hw_chan;

    return 0;
}

int ssv6xxx_set_promisc(struct ssv_softc *sc, int accept)
{
    u32 val=0;

    if (accept) //promiscuous mode
        val = MRX_MODE_PROMISCUOUS;
    else //normal mode
        val = MRX_MODE_NORMAL;

    HAL_SET_MRX_MODE(sc->sh, val);
    return 0;
}

int ssv6xxx_get_promisc(struct ssv_softc *sc, int *paccept)
{
    u32 val=0;

    HAL_GET_MRX_MODE(sc->sh, &val);

    if (val == MRX_MODE_PROMISCUOUS) //promiscuous mode
        *paccept = 1;
    else //normal mode
        *paccept = 0;

    return 0;
}


static int ssv6200_start(struct ieee80211_hw *hw)
{
    struct ssv_softc *sc = hw->priv;
    struct ssv_hw *sh = sc->sh;
    struct ieee80211_channel *chan;
    enum nl80211_channel_type     channel_type;

    mutex_lock(&sc->mutex);

    /* Reset MAC & Re-Init */
    /* Initialize ssv6200 mac */
    if (ssv6xxx_init_mac(sc->sh) != 0) {
        mutex_unlock(&sc->mutex);
        printk("Initialize ssv6200 mac fail!!\n");
        // ssv6xxx_deinit_mac(sc);
        return -1;
    }

    HCI_START(sh);

#ifndef SSV_SUPPORT_USB_LPM
    // Do not support USB LPM, so disable it
    SSV_SET_USB_LPM(sc, 0);
#endif

    // edca enable
    SSV_EDCA_ENABLE(sc->sh, true);

    /* get the current channel */
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
    chan = hw->conf.channel;
    channel_type = hw->conf.channel_type;
#else
    chan = hw->conf.chandef.chan;
    channel_type = cfg80211_get_chandef_type(&hw->conf.chandef);
#endif
    sc->cur_channel = chan;
    printk("%s(): current channel: %d,sc->ps_status=%d\n", __FUNCTION__, sc->cur_channel->hw_value,sc->ps_status);

    HAL_SET_CHANNEL(sc, chan, channel_type, false);   

    /* reset hardware to apply the configuration from mac80211 */


    /* setup interrupt mask of hardware interface (SDIO/SPI) */

    ieee80211_wake_queues(hw);

    SSV_AMPDU_AUTO_CRC_EN(sc->sh);
    SSV_SET_RF_ENABLE(sh);

    // start house keeping & tx stuck detection
    sc->mac80211_dev_started = true;
    SSV_SEND_TX_POLL_CMD(sc->sh, SSV6XXX_TX_POLL_START);

    sc->sc_flags |= SC_OP_IF_UP;
    mutex_unlock(&sc->mutex);
    return 0;
}


static void ssv6200_stop(struct ieee80211_hw *hw)
{
    struct ssv_softc *sc=hw->priv;

    printk(KERN_INFO "%s(): sc->ps_status=%d\n", __FUNCTION__,sc->ps_status);
    mutex_lock(&sc->mutex);

    sc->mac80211_dev_started = false;
    sc->sc_flags &= ~SC_OP_IF_UP;

    // stop tx stuck detection for fw
    SSV_SEND_TX_POLL_CMD(sc->sh, SSV6XXX_TX_POLL_STOP);

    SSV_SET_RF_DISABLE(sc->sh);

    HCI_STOP(sc->sh);
    /* flush tx queue */
    HCI_TXQ_FLUSH(sc->sh);

    // Fredie ToDo: Remove RX worker? Notify RX thread to flush out queued packets?
#if 0
    //#ifndef DCONFIG_SSV_RX_NO_WORKER
    cancel_work_sync(&sc->rx_work);
#endif

    if((sc->ps_status == PWRSV_PREPARE)||(sc->ps_status == PWRSV_ENABLE)){
        ssv6xxx_enable_ps(sc);
        SSV_SET_RF_ENABLE(sc->sh);
    }

    ssv6xxx_beacon_release(sc);
    mutex_unlock(&sc->mutex);

    printk("%s(): leave, hci_txq_len %d\n", __FUNCTION__, HCI_TXQ_LEN(sc->sh));
}

static int ssv6xxx_interface_opertaion(struct ssv_softc *sc, ssv6xxx_vif_ops ops, 
        u8 vif_idx, u8 *mac, u8 type, bool p2p, bool assoc)
{
    struct sk_buff *skb = NULL;
    struct cfg_host_cmd *host_cmd = NULL;
    struct ssv_vif_param *ptr = NULL;
    int ret = 0;
    u8 vif_type = 0;

    skb = ssv_skb_alloc(sc, HOST_CMD_HDR_LEN + sizeof(struct ssv_vif_param));
    if (skb == NULL) {
        printk("%s(): Fail to alloc cmd buffer.\n", __FUNCTION__);
        return -1;
    }

    skb_put(skb, HOST_CMD_HDR_LEN + sizeof(struct ssv_vif_param));
    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
    host_cmd->c_type = HOST_CMD;
    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_VIF_OPS;
    host_cmd->sub_h_cmd = (u32)ops;
    host_cmd->blocking_seq_no = (((u16)SSV6XXX_HOST_CMD_VIF_OPS << 16)|(u16)ops);
    host_cmd->len = HOST_CMD_HDR_LEN + sizeof(struct ssv_vif_param);

    ptr = (struct ssv_vif_param *)host_cmd->un.dat8;
    memset(ptr, 0x0, sizeof(struct ssv_vif_param));
    if ((type == NL80211_IFTYPE_AP) || (type == NL80211_IFTYPE_P2P_GO) || (type == NL80211_IFTYPE_ADHOC))
        vif_type = SSV6XXX_VIF_TYPE_AP; 
    else if ((type == NL80211_IFTYPE_STATION) || (type == NL80211_IFTYPE_P2P_CLIENT)) 
        vif_type = SSV6XXX_VIF_TYPE_STA;
    else
        vif_type = SSV6XXX_VIF_TYPE_NONE;

    switch (ops) {
        case SSV6XXX_VIF_CMD_ADD:
            ptr->vif_idx = vif_idx;
            memcpy(ptr->mac, mac, ETH_ALEN);
            ptr->type = vif_type;
            break;
        case SSV6XXX_VIF_CMD_DEL:
            ptr->vif_idx = vif_idx;
            break;
        case SSV6XXX_VIF_CMD_CHG:
            ptr->vif_idx = vif_idx;
            ptr->type = vif_type;
            ptr->assoc = assoc;
            ptr->p2p = p2p;
            break;
    }
    ret = HCI_SEND_CMD(sc->sh, skb);

    return 0;
}

struct ssv_vif_priv_data * ssv6xxx_config_vif_res(struct ssv_softc *sc, 
        struct ieee80211_vif *vif)
{
    int                       i, vif_idx = -1;
    struct ssv_vif_priv_data *priv_vif;
    struct ssv_vif_info      *vif_info;
    bool                      find_empty_vif_idx = false;

    lockdep_assert_held(&sc->mutex);

    //find vif 
    for (i=0 ; i<SSV6200_MAX_VIF ;i++) {
        if ((sc->vif_info[i].vif != NULL) && (!memcmp(vif->addr, sc->vif_info[i].vif->addr, ETH_ALEN))) {
            vif_idx = i;
            break;
        }

        if ((sc->vif_info[i].vif == NULL) && !find_empty_vif_idx) {
            find_empty_vif_idx = true;
            vif_idx = i;
            sc->nvif++;
        }
    }

    BUG_ON(vif_idx < 0);

    printk("ssv6xxx_config_vif_res id[%d].\n", vif_idx);

    priv_vif = (struct ssv_vif_priv_data *)vif->drv_priv;
    memset(priv_vif, 0, sizeof(struct ssv_vif_priv_data));

    priv_vif->vif_idx = vif_idx;

    memset(&sc->vif_info[vif_idx], 0, sizeof(sc->vif_info[0]));

    sc->vif_info[vif_idx].vif = vif;
    sc->vif_info[vif_idx].vif_priv = priv_vif;

    INIT_LIST_HEAD(&priv_vif->sta_list);

    priv_vif->pair_cipher = SECURITY_NONE;  // Security type for unicast data of this VIF.
    priv_vif->group_cipher = SECURITY_NONE; // Security type for multicast data of this VIF.

    priv_vif->has_hw_decrypt = false;
    priv_vif->has_hw_encrypt = false;
    priv_vif->need_sw_encrypt = false;
    priv_vif->need_sw_decrypt = false;
    priv_vif->use_mac80211_decrypt = false;
    priv_vif->is_security_valid = false;
    priv_vif->force_sw_encrypt = (vif->type == NL80211_IFTYPE_AP);

    priv_vif->wep_idx = -1;           

    vif_info = &sc->vif_info[priv_vif->vif_idx];
    vif_info->if_type = vif->type;
    vif_info->vif = vif;

    return priv_vif;
}

static void _if_set_apmode(struct ieee80211_hw *hw,
        struct ieee80211_vif *vif)
{
    struct ssv_softc *sc=hw->priv;
    struct ieee80211_channel *chan;
    struct ssv_vif_priv_data *vif_priv = (struct ssv_vif_priv_data *)vif->drv_priv;        

    SSV_SET_BSSID(sc->sh, vif->addr, vif_priv->vif_idx);
    SSV_SET_OP_MODE(sc->sh, SSV6XXX_OPMODE_AP, vif_priv->vif_idx);

    BUG_ON(sc->ap_vif != NULL);
    sc->ap_vif = vif;

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
    chan = hw->conf.channel;
#else
    chan = hw->conf.chandef.chan;
#endif
    printk("AP created at ch %d \n", chan->hw_value);

#ifdef CONFIG_SSV_SUPPORT_ANDROID
    if(vif->p2p == 0)
    {
        printk(KERN_INFO "AP mode init wifi_alive_lock\n");
        ssv_wake_lock(sc);
    }
#endif    
}

static int ssv6200_add_interface(struct ieee80211_hw *hw,
        struct ieee80211_vif *vif)
{
    struct ssv_softc *sc=hw->priv;
    int ret=0;
    struct ssv_vif_priv_data *vif_priv = NULL;

    printk("[I] %s(): \n", __FUNCTION__);

    // AP mode can only exist with managed(station) mode.
    if (   (sc->nvif >= SSV6200_MAX_VIF)
            || (   (   (vif->type == NL80211_IFTYPE_AP)
                    || (vif->p2p))
                && (sc->ap_vif != NULL)))
    {
        dev_err(sc->dev, "Add interface of type %d (p2p: %d) failed.\n", vif->type, vif->p2p);
        return -EOPNOTSUPP;
    }

    mutex_lock(&sc->mutex);

    vif_priv = ssv6xxx_config_vif_res(sc, vif);

    if ((vif->addr[0] == 0) && (vif->addr[1] == 0) && (vif->addr[2] == 0) &&
       (vif->addr[3] == 0) && (vif->addr[4] == 0) && (vif->addr[5] == 0))  
    {
        SSV_SET_MACADDR(sc->sh, vif_priv->vif_idx);
    }
    else
    {
        dev_err(sc->dev, "Set new macaddr\n");
        SSV_SET_MACADDR_2(sc->sh, vif_priv->vif_idx, vif->addr);
    }
    /*it is first entity, and in ap mode*/

    if ((vif->type == NL80211_IFTYPE_AP) || (vif->type == NL80211_IFTYPE_P2P_GO) || (vif->type == NL80211_IFTYPE_ADHOC))
        _if_set_apmode(hw, vif);

    /* Send Host Cmd to notify interface information */
    ssv6xxx_interface_opertaion(sc, SSV6XXX_VIF_CMD_ADD, vif_priv->vif_idx, vif->addr, vif->type, false, false);

    dev_err(sc->dev, "VIF %02x:%02x:%02x:%02x:%02x:%02x of type %d is added.\n",
            vif->addr[0], vif->addr[1], vif->addr[2],
            vif->addr[3], vif->addr[4], vif->addr[5], vif->type);

#ifdef CONFIG_SSV6XXX_DEBUGFS
    ssv6xxx_debugfs_add_interface(sc, vif);
#endif
#ifdef CONFIG_STA_BCN_FILTER
    if (1 < sc->nvif) {
        //disable filter for beacon and probe request
        HAL_SET_MRX_FILTER(sc->sh, 3, false, BIT(4)|BIT(5));
        sc->sta_bcn_filter = false;
    }
#endif

    mutex_unlock(&sc->mutex);
    return ret;
}

static int ssv6200_change_interface(struct ieee80211_hw *hw,
        struct ieee80211_vif *vif,
        enum nl80211_iftype new_type,
        bool p2p)
{
    struct ssv_softc         *sc = hw->priv;
    struct ssv_vif_priv_data *vif_priv = (struct ssv_vif_priv_data *)vif->drv_priv;
    int                       vif_idx = vif_priv->vif_idx;

    mutex_lock(&sc->mutex);
    printk("@@@@@@ change id[%d] type %d to %d, p2p=%d\n", vif_idx, sc->vif_info[vif_idx].if_type, new_type, p2p);
    sc->vif_info[vif_idx].if_type = new_type;
    sc->force_disable_directly_ack_tx = p2p;

    /* Add new settings */
    vif->type = new_type;
    if ((vif->type == NL80211_IFTYPE_AP) || (vif->type == NL80211_IFTYPE_P2P_GO) || (vif->type == NL80211_IFTYPE_ADHOC))
        _if_set_apmode(hw, vif);
    else {
        u8 null_addr[6]={0, 0, 0, 0, 0, 0};
        sc->ap_vif = NULL;
        SSV_SET_BSSID(sc->sh, null_addr, vif_priv->vif_idx);
        SSV_SET_OP_MODE(sc->sh, SSV6XXX_OPMODE_STA, vif_priv->vif_idx);        
    }

    vif->p2p = p2p;

    /* Send Host Cmd to notify interface information 
     * if p2p is true, it should be association state
     */
    ssv6xxx_interface_opertaion(sc, SSV6XXX_VIF_CMD_CHG, vif_priv->vif_idx, NULL, vif->type, vif->p2p, vif->p2p);
    mutex_unlock(&sc->mutex);

    return 0;
}

static void ssv6200_remove_interface(struct ieee80211_hw *hw,
        struct ieee80211_vif *vif)
{
    struct ssv_softc         *sc = hw->priv;
    struct ssv_vif_priv_data *vif_priv = (struct ssv_vif_priv_data *)vif->drv_priv;
    int vif_idx = 0;

    dev_err(sc->dev,
            "Removing interface %02x:%02x:%02x:%02x:%02x:%02x. PS=%d\n",
            vif->addr[0], vif->addr[1], vif->addr[2],
            vif->addr[3], vif->addr[4], vif->addr[5], sc->ps_status);
    /** 
     * STA mode decision table is the default table for ssv6xxx.
     * Set the table to default when interface is removed.
     */
    mutex_lock(&sc->mutex);

    if (sc->nvif == 0)
    {
        mutex_unlock(&sc->mutex);
        return;
    }

    /* Send Host Cmd to notify interface information */
    vif_idx = vif_priv->vif_idx;
    ssv6xxx_interface_opertaion(sc, SSV6XXX_VIF_CMD_DEL, vif_priv->vif_idx, NULL, 0, false, false);

#ifdef CONFIG_SSV6XXX_DEBUGFS
    ssv6xxx_debugfs_remove_interface(sc, vif);
#endif

    if ((vif->type == NL80211_IFTYPE_AP) || (vif->type == NL80211_IFTYPE_ADHOC))
    {
        /* In normal ap mode, release bcast frame and stop worker */
        //ssv6200_release_bcast_frame_res(sc, vif);
        //printk("Config Q4 to normal Q \n");

        /* Relase skb of beacon frame */
        ssv6xxx_beacon_release(sc);
        sc->ap_vif = NULL;

#ifdef CONFIG_SSV_SUPPORT_ANDROID
        if(vif->p2p == 0)
        {
            ssv_wake_unlock(sc);
            printk(KERN_INFO "AP mode destroy wifi_alive_lock\n");
        }
#endif

    }

    //disable beacon loss
    if (vif->type == NL80211_IFTYPE_STATION)
        SSV_BEACON_LOSS_DISABLE(sc->sh);

    memset(&sc->vif_info[vif_priv->vif_idx], 0, sizeof(struct ssv_vif_info));
    //sc->vif[vif_priv->vif_idx] = NULL;
    sc->nvif--;

#ifdef CONFIG_STA_BCN_FILTER
    if (1 == sc->nvif) {
        if (vif_idx == 0) { // check station and enable beacon filter for vif_idx 1
            if (NULL != sc->vif_info[1].vif) {
                if ((NL80211_IFTYPE_STATION == (sc->vif_info[1].vif)->type) && 
                    (true == (sc->vif_info[1].vif)->bss_conf.assoc) &&
                    (false == (sc->vif_info[1].vif)->p2p)) {
                    
                    //enable filter for beacon and probe request
                    HAL_SET_MRX_FILTER(sc->sh, 3, true, BIT(4)|BIT(5));
                    sc->sta_bcn_filter = true;
                }
            }
        }
        
        if (vif_idx == 1) { // check station and enable beacon filter for vif_idx 0
            if (NULL != sc->vif_info[1].vif) {
                if ((NL80211_IFTYPE_STATION == (sc->vif_info[0].vif)->type) && 
                    (true == (sc->vif_info[0].vif)->bss_conf.assoc) &&
                    (false == (sc->vif_info[0].vif)->p2p)) {
                    
                    //enable filter for beacon and probe request
                    HAL_SET_MRX_FILTER(sc->sh, 3, true, BIT(4)|BIT(5));
                    sc->sta_bcn_filter = true;
                }
            }
        }
    }
#endif
    mutex_unlock(&sc->mutex);
}

void ssv6xxx_enable_ps(struct ssv_softc *sc)
{
    sc->ps_status = PWRSV_ENABLE;
    //printk(KERN_INFO "PowerSave enabled\n");
}

void ssv6xxx_disable_ps(struct ssv_softc *sc)
{
    sc->ps_status = PWRSV_DISABLE;        
    printk(KERN_INFO "PowerSave disabled\n");
}

/* Return false, it match the following condition
 * 1. P2P_GO and station connection
 * 2. dual interface and STATION/P2P_CLINET association 
 */
bool ssv6200_not_dual_intf_on_line(struct ssv_softc *sc)
{
    struct ieee80211_vif *vif;
    int i = 0, assoc = 0, p2p = 0;

    for (i = 0; i < SSV6200_MAX_VIF; i++) {
        if (sc->vif_info[i].vif != NULL) {
            vif = sc->vif_info[i].vif;

            if ((vif->type == NL80211_IFTYPE_STATION) || (vif->type == NL80211_IFTYPE_P2P_CLIENT)) {
                if (vif->bss_conf.assoc)
                    assoc++;
                if (vif->p2p)
                    p2p++;
            }
        }
    }

    if ((p2p > 0) || (assoc == 2))
        return false;

    return true;
}

//
// return sta or GC associate count
int ssvxxx_get_sta_assco_cnt(struct ssv_softc *sc)
{
    struct ieee80211_vif *vif;
    int i = 0, assoc = 0;

    for (i = 0; i < SSV6200_MAX_VIF; i++) {
        if (sc->vif_info[i].vif != NULL) {
            vif = sc->vif_info[i].vif;

            if ((vif->type == NL80211_IFTYPE_STATION) || (vif->type == NL80211_IFTYPE_P2P_CLIENT)) {
                if (vif->bss_conf.assoc)
                    assoc++;
            }
        }
    }

    return assoc;
}

static int ssv6200_config(struct ieee80211_hw *hw, u32 changed)
{
    struct ssv_softc *sc=hw->priv;
    int ret=0;

    //    printk("%s(): changed: 0x%08x\n", __FUNCTION__, changed);
    mutex_lock(&sc->mutex);

    if (changed & IEEE80211_CONF_CHANGE_POWER) {
        struct ieee80211_conf *conf = &hw->conf;
        printk("IEEE80211_CONF_CHANGE_POWER change power level to %d\n", conf->power_level);
    }


    if (changed & IEEE80211_CONF_CHANGE_PS) {
        struct ieee80211_conf *conf = &hw->conf;
        if (conf->flags & IEEE80211_CONF_PS) {
            printk("Enable IEEE80211_CONF_PS ps_aid=%d\n",sc->ps_aid);
        }else{
            printk("Disable IEEE80211_CONF_PS ps_aid=%d\n",sc->ps_aid);
        }
    }

    if (changed & IEEE80211_CONF_CHANGE_MONITOR) {
        struct ieee80211_conf *conf = &hw->conf;
        if (conf->flags & IEEE80211_CONF_MONITOR) {
            printk("Enable IEEE80211_CONF_MONITOR\n");
            ret=ssv6xxx_set_promisc(sc,1);
            sc->sc_flags |= SC_OP_MONITOR;
        }else{
            printk("Disable IEEE80211_CONF_MONITOR\n"); 
            ret=ssv6xxx_set_promisc(sc,0);
            sc->sc_flags &= ~SC_OP_MONITOR;
        }
    }

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,0,0)
    if (changed & IEEE80211_CONF_CHANGE_QOS) {
        struct ieee80211_conf *conf = &hw->conf;
        bool qos_active = !!(conf->flags & IEEE80211_CONF_QOS);

        //set QoS status
        SMAC_REG_SET_BITS(sc->sh, ADR_GLBLE_SET, 
                (qos_active<<QOS_EN_SFT), QOS_EN_MSK);     
    }    
#endif

    if (changed & IEEE80211_CONF_CHANGE_CHANNEL) {
        struct ieee80211_channel *chan;
        enum nl80211_channel_type channel_type;

        /* 
         * 1. If hw reset, channel setting may cause that IO fail to read/write register
         * 2. fixed channel for debug command
         */
        if ((sc->sc_flags & SC_OP_HW_RESET) ||
                (sc->sc_flags & SC_OP_CHAN_FIXED))
            goto out;

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
        chan = hw->conf.channel;
        channel_type = hw->conf.channel_type;
#else
        chan = hw->conf.chandef.chan;
        channel_type = cfg80211_get_chandef_type(&hw->conf.chandef);
#endif

#if defined (CONFIG_SSV_CTL)
        //printk("@_@ %d\n",sc->ssv_smartlink_status);
        if (sc->ssv_smartlink_status)
        {
            printk("@@ %d\n",sc->ssv_smartlink_status);
            goto out;
        }
#endif

        //        struct ieee80211_channel *curchan = hw->conf.channel;
        //        printk("%s(): Set channel to %d (%d MHz), sc->ch=%d, (%s)\n", __FUNCTION__, curchan->hw_value+1, 
        //            curchan->center_freq, sc->cur_channel->hw_value+1,
        //        ((hw->conf.flags&IEEE80211_CONF_OFFCHANNEL)? "off channel": "on channel"));

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,1,0)
        {
            struct ieee80211_channel *curchan = hw->conf.channel;

            if(sc->bScanning == true && 
                    sc->channel_center_freq != curchan->center_freq && sc->isAssoc){

                hw->conf.flags |= IEEE80211_CONF_OFFCHANNEL;
            }
            else{

                hw->conf.flags &= ~IEEE80211_CONF_OFFCHANNEL;
            }
        }
#endif


        /**
         * If the current channel is off channel, pause all tx queue except 
         * management queue.
         */  
        if (hw->conf.flags & IEEE80211_CONF_OFFCHANNEL)
        {
            // If AP mode is enabled, ignore off channel config.
            if (/* (IS_ALLOW_SCAN || IS_NON_AP_MODE || IS_NONE_STA_CONNECTED_IN_AP_MODE)
                    && ssv6200_not_dual_intf_on_line(sc) 
                    && */((sc->hw_chan != chan->hw_value) || (sc->hw_chan_type != channel_type)) )
            { 
                int i = 1;

                //printk("off channel setting %p!\n", sc->ap_vif);
                sc->sc_flags |= SC_OP_OFFCHAN;
                HCI_PAUSE_HWSWQ(sc->sh, (TXQ_EDCA_0|TXQ_EDCA_1|TXQ_EDCA_2|TXQ_EDCA_3));

                sc->rx_data_exist = false;
                mdelay(1);
                while ((sc->rx_data_exist == true) && (i < 200)){
                    i++;
                    sc->rx_data_exist = false;
                    mdelay(1);
                }
                sc->boffchan = true;              
                HAL_SET_CHANNEL(sc, chan, channel_type, true);
                sc->boffchan = false;
            }
            else
            {
                dev_dbg(sc->dev, "Off-channel to %d is ignored\n", chan->hw_value);
            }
            /* if it is not connect to any device, let it takes more time to stay in a channel */
            //            if(!sc->isAssoc)
            //                msleep(800);
        }
        else {
            if (   ((sc->cur_channel == NULL)
                        || (sc->sc_flags & SC_OP_OFFCHAN)
                        || (sc->hw_chan != chan->hw_value)
                        || (sc->hw_chan_type != channel_type)) )
            {
                HAL_SET_CHANNEL(sc, chan, channel_type, false);
                sc->cur_channel = chan;
                HCI_RESUME_HWSWQ(sc->sh, (TXQ_EDCA_0|TXQ_EDCA_1|TXQ_EDCA_2|TXQ_EDCA_3| TXQ_MGMT));
                sc->sc_flags &= ~SC_OP_OFFCHAN;
            }
            else
            {
                dev_dbg(sc->dev, "Change to the same channel %d\n", chan->hw_value);
            }
            // printk("on channel setting !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!: ch=%d\n", sc->cur_channel->hw_value);
        }
    }

out:
    mutex_unlock(&sc->mutex);
    return ret;
}


#if 0
static int sv6200_conf_tx(struct ieee80211_hw *hw,
        struct ieee80211_vif *vif, u16 queue,
        const struct ieee80211_tx_queue_params *params)
{
    struct ssv_softc *sc = hw->priv;
    u32 cw;
    u8 hw_txqid = sc->tx.hw_txqid[queue]; 

    printk("[I] sv6200_conf_tx qos[%d] queue[%d] aifsn[%d] cwmin[%d] cwmax[%d] txop[%d] \n",
            vif->bss_conf.qos, queue, params->aifs, params->cw_min, params->cw_max, params->txop);

    if (queue > NL80211_TXQ_Q_BK)
        return 1;
    mutex_lock(&sc->mutex);


    //set QoS status
#define QOS_EN_MSK                            0x00000010
#define QOS_EN_I_MSK                          0xffffffef
#define QOS_EN_SFT                            4         
#define QOS_EN_HI                             4         
#define QOS_EN_SZ                             1
    SMAC_REG_SET_BITS(sc->sh, ADR_GLBLE_SET, (vif->bss_conf.qos<<QOS_EN_SFT), QOS_EN_MSK);


    //set wmm parameter
    cw = params->aifs&0xf;
    cw|= ((ilog2(params->cw_min+1))&0xf)<<8;
    cw|= ((ilog2(params->cw_max+1))&0xf)<<12;
    cw|= ((params->txop)&0xff)<<16;

    SMAC_REG_WRITE(sc->sh, ADR_TXQ0_MTX_Q_AIFSN+0x100*hw_txqid, cw);

    mutex_unlock(&sc->mutex);
    return 0;
}

#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,2,0)
#define SUPPORTED_FILTERS            \
    (FIF_PROMISC_IN_BSS |            \
     FIF_ALLMULTI |                   \
     FIF_CONTROL |                    \
     FIF_PSPOLL |                     \
     FIF_OTHER_BSS |                  \
     FIF_BCN_PRBRESP_PROMISC |        \
     FIF_PROBE_REQ |                  \
     FIF_FCSFAIL)
#else
#define SUPPORTED_FILTERS            \
    (FIF_ALLMULTI |                  \
     FIF_CONTROL |                    \
     FIF_PSPOLL |                     \
     FIF_OTHER_BSS |                  \
     FIF_BCN_PRBRESP_PROMISC |        \
     FIF_PROBE_REQ |                  \
     FIF_FCSFAIL)
#endif

static void ssv6200_config_filter(struct ieee80211_hw *hw,
        unsigned int changed_flags,
        unsigned int *total_flags,
        u64 multicast)
{
    //    struct ssv_softc *sc=hw->priv;
    //
    //    printk("%s(): changed_flags: 0x%08x, total_flags: 0x%08x\n", 
    //    __FUNCTION__, changed_flags, *total_flags);
    //
    //    mutex_lock(&sc->mutex);

    /**
     * Note ??????????????
     * Modify this flag for AP mode ?????????
     */
    changed_flags &= SUPPORTED_FILTERS;
    *total_flags &= SUPPORTED_FILTERS;


    //    mutex_unlock(&sc->mutex);

}


static void ssv6200_bss_info_changed(struct ieee80211_hw *hw,
        struct ieee80211_vif *vif, struct ieee80211_bss_conf *info,
        u32 changed)
{
    struct ssv_vif_priv_data *priv_vif = (struct ssv_vif_priv_data *)vif->drv_priv;
    struct ssv_softc *sc = hw->priv;
    int i = 0;

    //    printk("[I] %s(): VIF[%d] changed 0x[%08x]\n", __FUNCTION__, priv_vif->vif_idx, changed);
    mutex_lock(&sc->mutex);

    //temp mark: RATE CTRL
    if (changed & BSS_CHANGED_ERP_PREAMBLE) {
        printk("BSS Changed use_short_preamble[%d]\n", info->use_short_preamble);
        if (info->use_short_preamble)
            sc->sc_flags |= SC_OP_SHORT_PREAMBLE;
        else
            sc->sc_flags &= ~SC_OP_SHORT_PREAMBLE;
    }

    if (changed & BSS_CHANGED_ERP_CTS_PROT) {
        printk("BSS Changed use_cts_prot[%d]\n", info->use_cts_prot);
        if (info->use_cts_prot)
            sc->sc_flags |= SC_OP_CTS_PROT;
        else
            sc->sc_flags &= ~SC_OP_CTS_PROT;
    }

    if (SSV_CHK_IF_SUPPORT_HW_BSSID(sc, priv_vif->vif_idx))
    {
        if (changed & BSS_CHANGED_BSSID) 
        {
            /* Set BSSID to hardware and enable WSID entry 0 */
            SSV_SET_BSSID(sc->sh, (u8*)info->bssid, priv_vif->vif_idx);     

            printk("BSS_CHANGED_BSSID: %02x:%02x:%02x:%02x:%02x:%02x\n",
                    info->bssid[0], info->bssid[1], info->bssid[2],
                    info->bssid[3], info->bssid[4], info->bssid[5]);
        }

        if (changed & BSS_CHANGED_ERP_SLOT)
        {
            printk("BSS_CHANGED_ERP_SLOT: use_short_slot[%d]\n", info->use_short_slot);

            /*
               Fix MAC TX backoff issue.
                http://192.168.1.30/mantis/view.php?id=36
             */
            SSV_SET_DUR_BURST_SIFS_G(sc->sh, 0xa);

            if (info->use_short_slot) {
                SSV_SET_DUR_SLOT(sc->sh, 0x9);
                //slottime = 9;
            } else {
                SSV_SET_DUR_SLOT(sc->sh, 20);
                //slottime = 20;
            }
        }
    }

    if (changed & BSS_CHANGED_HT) {
        printk("BSS_CHANGED_HT: Untreated!!\n");
    }


    if (changed & BSS_CHANGED_BASIC_RATES)
    {
        printk("ssv6xxx_rc_update_basic_rate!!\n");
    }


    if (vif->type == NL80211_IFTYPE_STATION){
        struct ieee80211_channel *curchan;
        int dual_if_vif_idx = 0;

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
        curchan = hw->conf.channel;
#else
        curchan = hw->conf.chandef.chan;
#endif        
        printk("NL80211_IFTYPE_STATION!!\n");
        // find the other interface vif idx
        if (priv_vif->vif_idx == 0)
            dual_if_vif_idx = 1;

        if (changed & BSS_CHANGED_ASSOC) {
            sc->isAssoc = info->assoc;
            /* Send HostCmd to notify association status */
            ssv6xxx_interface_opertaion(sc, SSV6XXX_VIF_CMD_CHG, priv_vif->vif_idx, NULL, vif->type, false, info->assoc);

            if (!sc->isAssoc) { // station mode disassociation

#ifdef CONFIG_STA_BCN_FILTER
                //disable filter for beacon and probe request
                HAL_SET_MRX_FILTER(sc->sh, 3, false, BIT(4)|BIT(5));
                sc->sta_bcn_filter = false;
#endif

                sc->channel_center_freq = 0;
                sc->ps_aid = 0;
#ifdef SSV_SUPPORT_USB_LPM
                // Enable LPM at disconnection state
                SSV_SET_USB_LPM(sc, 1);
#endif
            }
            else{
                sc->channel_center_freq = curchan->center_freq;
                printk(KERN_INFO "!!info->aid = %d\n",info->aid);
                for (i = 0; i < SSV_NUM_STA; i++) {
                    if (sc->sta_info[i].s_flags & STA_FLAG_VALID) {
                        if (vif == sc->sta_info[i].vif) {
                            sc->sta_info[i].aid = info->aid;
                            break;
                        }
                    }
                }
               
                sc->ps_aid = info->aid;
#ifdef SSV_SUPPORT_USB_LPM
                /* 
                   Enable LPM will frequently swicth LPM power state and generate latency.
                   When traffic loading is high, LPM makes throughput low.
                   So disable LPM at connection state.
                 */
                SSV_SET_USB_LPM(sc, 0);
#endif
#ifdef CONFIG_STA_BCN_FILTER
                if ((sc->vif_info[dual_if_vif_idx].vif == NULL)) {
                    //enable filter for beacon and probe request
                    HAL_SET_MRX_FILTER(sc->sh, 3, true, BIT(4)|BIT(5));  
                    sc->sta_bcn_filter = true;
                }
#endif
            }
        }

        if ((changed & BSS_CHANGED_BEACON_INT) && (info->beacon_int != 0)
                /*&& (curchan->band == INDEX_80211_BAND_2GHZ)*/)
            SSV_BEACON_LOSS_CONFIG(sc->sh, info->beacon_int, info->bssid);
    }

    //--------------------------------------------------------------
    if ((vif->type == NL80211_IFTYPE_AP) || (vif->type == NL80211_IFTYPE_ADHOC))
    {

        if (changed & (  BSS_CHANGED_BEACON
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
                    | BSS_CHANGED_SSID
#endif                
                    | BSS_CHANGED_BSSID
                    | BSS_CHANGED_BASIC_RATES))
        {
#ifdef BROADCAST_DEBUG
            printk("[A] ssv6200_bss_info_changed:beacon changed\n");
#endif
            ssv6200_set_tim_work(sc);
        }

        if (changed & BSS_CHANGED_BEACON_INT)
        {
            printk("[A] BSS_CHANGED_BEACON_INT beacon_interval(%d)\n", info->beacon_int);
            if (sc->beacon_interval != info->beacon_int)
            {
                sc->beacon_interval = info->beacon_int;
                ssv6x5x_beacon_set_interval(sc->sh, sc->beacon_interval, sc->beacon_dtim_cnt);
            }

        }

        if (changed & BSS_CHANGED_BEACON_ENABLED)
        {           
            dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_BEACON,
                    "[A] BSS_CHANGED_BEACON_ENABLED (0x%x)\n", info->enable_beacon);

            ssv6x5x_beacon_enable(sc->sh, info->enable_beacon);
        }
    }

    mutex_unlock(&sc->mutex);
    printk("[I] %s(): leave\n", __FUNCTION__);
}

static int _ssv6200_sta_operation(struct ssv_softc *sc, struct ieee80211_sta *sta, int wsid, 
            struct ieee80211_supported_band *sband, ssv6xxx_rc_ops ops)
{
    struct sk_buff *skb = NULL;
    struct cfg_host_cmd *host_cmd = NULL;
    struct ssv_rc_param *ptr = NULL;
    int ret = 0;

    skb = ssv_skb_alloc(sc, HOST_CMD_HDR_LEN + sizeof(struct ssv_rc_param));
    if (skb == NULL) {
        printk("%s(): Fail to alloc cmd buffer.\n", __FUNCTION__);
        return -1;
    }
    
    skb_put(skb, HOST_CMD_HDR_LEN + sizeof(struct ssv_rc_param));
    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
    host_cmd->c_type = HOST_CMD;
    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_RC_OPS;
    host_cmd->sub_h_cmd = (u32)ops;
    host_cmd->blocking_seq_no = (((u16)SSV6XXX_HOST_CMD_RC_OPS << 16)|(u16)ops);
    host_cmd->len = HOST_CMD_HDR_LEN+sizeof(struct ssv_rc_param);
    ptr = (struct ssv_rc_param *)host_cmd->un.dat8;
    memset((void *)ptr, 0x0, sizeof(struct ssv_rc_param));
    
    ptr->wsid = wsid;
    if(SSV6XXX_RC_CMD_INIT == ops)
    {
#if 0 //debug
        printk(KERN_ERR "\n\nsta->supp_rates[%u] = 0x%08x\n", sband->band, sta->supp_rates[sband->band]);
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
        printk(KERN_ERR "sc->hw->conf.channel_type = 0x%02x\n", sc->hw->conf.channel_type);
#else
        printk(KERN_ERR "cfg80211_get_chandef_type(&(sc->hw->conf.chandef)) = 0x%02x\n", cfg80211_get_chandef_type(&(sc->hw->conf.chandef)));
#endif
        printk(KERN_ERR "sta->ht_cap.cap = 0x%04x\n", sta->ht_cap.cap);
        printk(KERN_ERR "sta->ht_cap.mcs.rx_mask[0] = 0x%02x\n", sta->ht_cap.mcs.rx_mask[0]);
        printk(KERN_ERR "sta->ht_cap.mcs.rx_mask[1] = 0x%02x\n", sta->ht_cap.mcs.rx_mask[1]);
        printk(KERN_ERR "sta->ht_cap.mcs.rx_mask[2] = 0x%02x\n", sta->ht_cap.mcs.rx_mask[2]);
        printk(KERN_ERR "sta->ht_cap.mcs.rx_mask[3] = 0x%02x\n\n", sta->ht_cap.mcs.rx_mask[3]);
#endif
        ptr->supp_rates = sta->supp_rates[sband->band];
        if(INDEX_80211_BAND_5GHZ == sband->band)
        {
            ptr->supp_rates = ptr->supp_rates << 4; //5G rate starts from 6Mbps.
        }
        ptr->rc_mask = (u16)sc->sh->cfg.rc_mask;
        ptr->ht_capabilities_info = sta->ht_cap.cap;
        memcpy((void *)&ptr->supported_mcs_set[0], (const void *)&sta->ht_cap.mcs, 16);
        ptr->ht_support = sta->ht_cap.ht_supported;
        ptr->short_preamble = (sc->sc_flags & SC_OP_SHORT_PREAMBLE)?1:0;
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
        ptr->rctype = sc->hw->conf.channel_type;
#else
        ptr->rctype = cfg80211_get_chandef_type(&(sc->hw->conf.chandef));
#endif
    }
    ret = HCI_SEND_CMD(sc->sh, skb);

    return 0;
}

static int ssv6200_sta_add(struct ieee80211_hw *hw,
        struct ieee80211_vif *vif,
        struct ieee80211_sta *sta)
{
    struct ssv_sta_priv_data *sta_priv_dat=NULL;
    struct ssv_softc *sc=hw->priv;
    struct ssv_sta_info *sta_info;
    //u32 reg_val;
    int s;
    unsigned long flags;
    int ret = 0;
    struct ssv_vif_priv_data *vif_priv = (struct ssv_vif_priv_data *)vif->drv_priv;
    struct ieee80211_supported_band *sband;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0)
    struct ieee80211_chanctx_conf *chanctx_conf;
#endif

    printk("[I] %s(): vif[%d] ", __FUNCTION__, vif_priv->vif_idx);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0)
    chanctx_conf = vif->chanctx_conf;
    if (chanctx_conf == NULL) {
        WARN_ON(1);
        return -EINVAL;
    }
#endif

    mutex_lock(&sc->mutex);
    //down_read(&sc->sta_info_sem);/* For mac80211 cb, we use sc->mutex to protect sta_info instead of sta_info_sem. */
    do {
        spin_lock_irqsave(&sc->ps_state_lock, flags);

        s = HAL_GET_WSID(sc, vif, sta);
        sta_info = &sc->sta_info[s];
        sta_priv_dat = (struct ssv_sta_priv_data *)sta->drv_priv;
        spin_unlock_irqrestore(&sc->ps_state_lock, flags);

        if (s >= SSV_NUM_STA)
        {
            dev_err(sc->dev, "Number of STA exceeds driver limitation %d\n.", SSV_NUM_STA);
            ret = -1;
            break;
        }

#ifdef CONFIG_SSV6XXX_DEBUGFS
        ssv6xxx_debugfs_add_sta(sc, sta_info);
#endif // CONFIG_SSV6XXX_DEBUGFS

        sta_priv_dat->wep_key_update = false;
        /* Set WSID to default*/
        sta_info->hw_wsid = -1;  
        SSV_SET_HW_WSID(sc, vif, sta, s);

        if ((sta_priv_dat->has_hw_encrypt || sta_priv_dat->has_hw_decrypt) &&
                ((vif_priv->pair_cipher == SECURITY_WEP40) || (vif_priv->pair_cipher == SECURITY_WEP104)))
        {                    
            HAL_SET_WEP_HW_CRYPTO_KEY(sc, sta_priv_dat, vif_priv);
        }

        spin_lock_init(&sta_priv_dat->ampdu_ctrl_lock);
        sband = sc->hw->wiphy->bands[sc->cur_channel->band];
        _ssv6200_sta_operation(sc, sta, sta_info->hw_wsid, sband, SSV6XXX_RC_CMD_INIT);
        
        printk("Add %02x:%02x:%02x:%02x:%02x:%02x to VIF %d sw_idx=%d, wsid=%d\n",
                sta->addr[0], sta->addr[1], sta->addr[2],
                sta->addr[3], sta->addr[4], sta->addr[5],
                vif_priv->vif_idx,
                sta_priv_dat->sta_idx, sta_info->hw_wsid);
    } while (0);
    //up_read(&sc->sta_info_sem);/* For mac80211 cb, we use sc->mutex to protect sta_info instead of sta_info_sem. */
    mutex_unlock(&sc->mutex);

    return ret;
}

/* Use original Rx flow while wsid 0 and wsid 1 not use SW cipher*/ 
void ssv6200_rx_flow_check(struct ssv_sta_priv_data *sta_priv_dat,
        struct ssv_softc *sc)
{       
    if (SSV6200_USE_HW_WSID(sta_priv_dat->sta_idx) && (sta_priv_dat->need_sw_decrypt)){
        int other_hw_wsid = (sta_priv_dat->sta_idx+ 1) & 1;
        struct ssv_sta_info *sta_info = &sc->sta_info[other_hw_wsid];/* sta_info is already protected by ssv6200_sta_remove(). */
        struct ieee80211_sta *sta = sta_info->sta;/* sta_info is already protected by ssv6200_sta_remove(). */
        struct ssv_sta_priv_data *sta_priv = (struct ssv_sta_priv_data *) sta->drv_priv;

        if ((sta_info-> s_flags == 0) 
                || ((sta_info-> s_flags & STA_FLAG_VALID) && (sta_priv->has_hw_decrypt))){/* sta_info is already protected by ssv6200_sta_remove(). */
            // restore original rx flow at removing sta when other sta not used or use hw cipher 

            if (USE_MAC80211_RX(sc->sh)){
                HAL_SET_RX_FLOW(sc->sh, RX_DATA_FLOW, RX_HCI); 
            } else {
                HAL_SET_RX_FLOW(sc->sh, RX_DATA_FLOW, RX_CIPHER_MIC_HCI);
            }
            printk("redirect Rx flow for sta %d  disconnect\n",sta_priv_dat->sta_idx);
        }      
    } 
}
static int ssv6200_sta_remove(struct ieee80211_hw *hw,
        struct ieee80211_vif *vif,
        struct ieee80211_sta *sta)
{
    struct ssv_sta_priv_data   *sta_priv_dat = (struct ssv_sta_priv_data *)sta->drv_priv;
    struct ssv_softc           *sc = hw->priv;
    struct ssv_sta_info        *sta_info = NULL;
    unsigned long               flags;
    u32                         bit;
    struct ssv_vif_priv_data   *priv_vif = (struct ssv_vif_priv_data *)vif->drv_priv;
    u8                          hw_wsid = -1;

    BUG_ON(sta_priv_dat->sta_idx >= SSV_NUM_STA);

    dev_notice(sc->dev,
            "Removing STA %d (%02X:%02X:%02X:%02X:%02X:%02X) from VIF %d\n.",
            sta_priv_dat->sta_idx, sta->addr[0], sta->addr[1], sta->addr[2],
            sta->addr[3], sta->addr[4], sta->addr[5], priv_vif->vif_idx);

#if defined(IRQ_PROC_RX_DATA)
    local_bh_disable();/* Prevent HWIF layer from using tasklet/workqueue to call ssv6200_rx(). */
#endif
    mutex_lock(&sc->mutex);
    if(sta_priv_dat->sta_idx == -1)
    {
        mutex_unlock(&sc->mutex);
#if defined(IRQ_PROC_RX_DATA)
        local_bh_enable();
#endif
        return 0;
    }
    sta_info = &sc->sta_info[sta_priv_dat->sta_idx];
    HCI_TXQ_LOCK_BY_STA(sc->sh, sta_info->hw_wsid);

    _ssv6200_sta_operation(sc, sta, sta_info->hw_wsid, NULL, SSV6XXX_RC_CMD_DEINIT);

    down_write(&sc->sta_info_sem);

    ssv6200_rx_flow_check(sta_priv_dat, sc);        

    spin_lock_irqsave(&sc->ps_state_lock, flags);
#ifdef CONFIG_SSV6XXX_DEBUGFS
    // Remove STA debugfs during sta remove callback could results in deadlock.
    //ssv6xxx_debugfs_remove_sta(sc, sta_info);
#endif // CONFIG_SSV6XXX_DEBUGFS

#if 0
    // Freddie ToDo: Remove when power saving?
    if ((sc->ps_status == PWRSV_PREPARE)||(sc->ps_status == PWRSV_ENABLE)) {
        //ssv6xxx_enable_ps(sc);
        memset(sta_info, 0, sizeof(*sta_info));
        sta_priv_dat->sta_idx = -1;

        list_del(&sta_priv_dat->list);

        spin_unlock_irqrestore(&sc->ps_state_lock, flags);
        return 0;
    }
#endif
    /*remove this sleep bit*/
    bit = BIT(sta_priv_dat->sta_idx);
    priv_vif->sta_asleep_mask &= ~bit;

    /* Remove invalid wsid entry */
    spin_unlock_irqrestore(&sc->ps_state_lock, flags);
    HAL_DEL_FW_WSID(sc, sta, sta_info);
    spin_lock_irqsave(&sc->ps_state_lock, flags);
    hw_wsid = sta_info->hw_wsid;

#if 0    
    printk("%s(): sw_idx=%d, hw_idx=%d sta_asleep_mask[%08x]\n", __FUNCTION__,
            sta_priv_dat->sta_idx , sta_info->hw_wsid, sc->sta_asleep_mask);
    printk("Remove %02x:%02x:%02x:%02x:%02x:%02x to sw_idx=%d, wsid=%d\n", 
            sta->addr[0], sta->addr[1], sta->addr[2],
            sta->addr[3], sta->addr[4], sta->addr[5], sta_priv_dat->sta_idx, sta_info->hw_wsid); 
#endif

#ifdef CONFIG_SSV6XXX_DEBUGFS
    {
        spin_unlock_irqrestore(&sc->ps_state_lock, flags);
        // Remove STA debugfs during sta remove callback could results in deadlock.
        ssv6xxx_debugfs_remove_sta(sc, sta_info);
        spin_lock_irqsave(&sc->ps_state_lock, flags);
    }
#endif // CONFIG_SSV6XXX_DEBUGFS

    memset(sta_info, 0, sizeof(*sta_info));
    sta_priv_dat->sta_idx = -1;
    list_del(&sta_priv_dat->list);
    if (list_empty(&priv_vif->sta_list) && vif->type == NL80211_IFTYPE_STATION)
    {
        priv_vif->pair_cipher = 0;
        priv_vif->group_cipher = 0;
        sc->ps_aid = 0;
    }

    spin_unlock_irqrestore(&sc->ps_state_lock, flags);

#if 0
    /**
     * Remove the specified station from the driver data structure.
     * Driver keeps this information for AMPDU use.
     */
    sta_info = sc->sta_info;
    for(s=0; s<SSV_NUM_STA; s++, sta_info++) {
        if (sta_info->s_flags & STA_FLAG_VALID)
            continue;
        if (sta_info->sta == sta && 
                sta_info->vif == vif)
            sta_info->s_flags = 0;
    }
#endif

    HCI_TX_PAUSE_BY_STA(sc->sh, hw_wsid);
    HCI_TXQ_FLUSH_BY_STA(sc->sh, hw_wsid);
    HCI_TX_RESUME_BY_STA(sc->sh, hw_wsid);
    SSV_DEL_HW_WSID(sc, hw_wsid);
    printk("hw wsid %u is removed.\n", hw_wsid);
    up_write(&sc->sta_info_sem);
    HCI_TXQ_UNLOCK_BY_STA(sc->sh, hw_wsid);
    mutex_unlock(&sc->mutex);
#if defined(IRQ_PROC_RX_DATA)
    local_bh_enable();
#endif
    return 0;
}


static void ssv6200_sta_notify(struct ieee80211_hw *hw,
        struct ieee80211_vif *vif,
        enum sta_notify_cmd cmd,
        struct ieee80211_sta *sta)
{
    struct ssv_softc *sc = hw->priv;
    struct ssv_sta_priv_data *sta_priv_dat = (struct ssv_sta_priv_data *)sta->drv_priv;
    u32 txqid = 0;
    unsigned long flags;

    if (vif->type != NL80211_IFTYPE_AP)
        return;

    spin_lock_irqsave(&sc->ps_state_lock, flags);
    if(sta_priv_dat->sta_idx == -1)
    {
        spin_unlock_irqrestore(&sc->ps_state_lock, flags);
        return;
    }
    txqid = sta_priv_dat->sta_idx;

    switch (cmd)
    {
        case STA_NOTIFY_SLEEP:
            HCI_TX_PAUSE_BY_STA(sc->sh, txqid);
            break;
        case STA_NOTIFY_AWAKE:
            HCI_TX_RESUME_BY_STA(sc->sh, txqid);
            break;
        default:
            break;
    }

    spin_unlock_irqrestore(&sc->ps_state_lock, flags);
    return;
}


#if LINUX_VERSION_CODE < KERNEL_VERSION(3,2,0)   
static u64 ssv6200_get_tsf(struct ieee80211_hw *hw)
#else
static u64 ssv6200_get_tsf(struct ieee80211_hw *hw, 
        struct ieee80211_vif *vif)
#endif
{
    printk("%s(): \n", __FUNCTION__);
    return 0;
}

void ssv6xxx_scan_opertaion(struct ssv_softc *sc, bool start)
{
    struct sk_buff *skb = NULL;
    struct cfg_host_cmd *host_cmd = NULL;
    int ret = 0;

    skb = ssv_skb_alloc(sc, HOST_CMD_HDR_LEN);
    if (skb == NULL) {
        printk("%s(): Fail to alloc cmd buffer.\n", __FUNCTION__);
        return;
    }

    skb_put(skb, HOST_CMD_HDR_LEN);
    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
    host_cmd->c_type = HOST_CMD;
    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_RFPHY_OPS;
    host_cmd->sub_h_cmd = ((start == true) ? (u32)SSV6XXX_RFPHY_CMD_SCAN_START : (u32)SSV6XXX_RFPHY_CMD_SCAN_DONE);
    host_cmd->len = HOST_CMD_HDR_LEN;
    ret = HCI_SEND_CMD(sc->sh, skb);
    if (ret)
        printk("Fail to send scan command\n");
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,19,0)
static void ssv6200_sw_scan_start(struct ieee80211_hw *hw,
        struct ieee80211_vif *vif, const u8 *mac_addr)
#else                 
static void ssv6200_sw_scan_start(struct ieee80211_hw *hw)
#endif
{
    struct ssv_softc *sc = hw->priv;
    u32 dev_type = HCI_DEVICE_TYPE(sc->sh->hci.hci_ctrl);

    ssv6xxx_scan_opertaion(sc, true);
#ifdef CONFIG_STA_BCN_FILTER
    if (sc->sta_bcn_filter) {
        //disable filter for beacon and probe request
        HAL_SET_MRX_FILTER(sc->sh, 3, false, BIT(4)|BIT(5));
    }
#endif

    //disable beacon loss detect in scanning
    SSV_BEACON_LOSS_DISABLE(sc->sh);

    if (dev_type == SSV_HWIF_INTERFACE_USB) 
        sc->sh->cfg.usb_hw_resource |= USB_HW_RESOURCE_CHK_SCAN;

#ifdef CONFIG_ENABLE_ACS_FUNC
   //Enable RX fixed gain setting for EDCCA.
    if(!IS_NON_AP_MODE && !sc->ap_vif->p2p && IS_NONE_STA_CONNECTED_IN_AP_MODE)
    {
        SSV_RX_FIXED_GAIN_ENABLE(sc->sh, true);
    }

    //Disable RX fixed gain setting for EDCCA
    if(!IS_NON_AP_MODE && !sc->ap_vif->p2p && IS_NONE_STA_CONNECTED_IN_AP_MODE)
    {
        SSV_RX_FIXED_GAIN_ENABLE(sc->sh, false);
    }
#endif

    sc->bScanning = true;

    printk("--------------%s(): \n", __FUNCTION__);
}


#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,19,0)
static void ssv6200_sw_scan_complete(struct ieee80211_hw *hw,
        struct ieee80211_vif *vif)
#else                 
static void ssv6200_sw_scan_complete(struct ieee80211_hw *hw)
#endif
{
    struct ssv_softc *sc = hw->priv;
    u32 dev_type = HCI_DEVICE_TYPE(sc->sh->hci.hci_ctrl);

    ssv6xxx_scan_opertaion(sc, false);
#ifdef CONFIG_STA_BCN_FILTER
    if (sc->sta_bcn_filter) {
        //enable filter for beacon and probe request
        HAL_SET_MRX_FILTER(sc->sh, 3, true, BIT(4)|BIT(5));
    }
#endif

    //enable beacon loss detect after scanning
    if (sc->isAssoc)
        SSV_BEACON_LOSS_ENABLE(sc->sh);

    if (dev_type == SSV_HWIF_INTERFACE_USB) 
        sc->sh->cfg.usb_hw_resource &= (~USB_HW_RESOURCE_CHK_SCAN);

    sc->bScanning = false;

    printk("==============%s(): \n", __FUNCTION__);
}


static int ssv6200_set_tim(struct ieee80211_hw *hw, struct ieee80211_sta *sta,
        bool set)
{
#if 1
    struct ssv_softc *sc = hw->priv;
    
    ssv6200_set_tim_work(sc);
    return 0;
#else
    struct ssv_softc *sc = hw->priv;
    //    int sta_idx = 0;
    struct ssv_sta_info *sta_info = NULL;/* For mac80211 cb, we use sc->mutex to protect sta_info instead of sta_info_sem. */

    mutex_lock(&sc->mutex);
    if (sta) {
        //while (!down_read_trylock(&sc->sta_info_sem));/* For performance concern, don't lock this code. Accessing tim_set is safe enough. */
        if(((struct ssv_sta_priv_data *)sta->drv_priv)->sta_idx == -1)
        {
            printk("%s(): sta_info is gone.\n", __func__);
            goto out;
        }
        sta_info = &sc->sta_info[((struct ssv_sta_priv_data *)sta->drv_priv)->sta_idx];
        if ((sta_info->s_flags & STA_FLAG_VALID) == 0) {
            printk("%s(): sta_info is gone.\n", __func__);
            goto out;
        }
    }

    /* Call set beacon if tim bit is changed */
    if (sta_info && (sta_info->tim_set^set))
    {
#ifdef BROADCAST_DEBUG
        printk("[I] [A] ssv6200_set_tim");
#endif
        sta_info->tim_set = set;
        ssv6200_set_tim_work(sc);
    }

out:

    mutex_unlock(&sc->mutex);
    return 0;
#endif
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,2,0)
static int ssv6200_conf_tx(struct ieee80211_hw *hw, u16 queue,
        const struct ieee80211_tx_queue_params *params)
#else
static int ssv6200_conf_tx(struct ieee80211_hw *hw,
        struct ieee80211_vif *vif, u16 queue,
        const struct ieee80211_tx_queue_params *params)
#endif
{
    struct ssv_softc *sc = hw->priv;
    int ret = 0; 
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
    struct ssv_vif_priv_data *priv_vif = (struct ssv_vif_priv_data *)vif->drv_priv;

    printk("[I] sv6200_conf_tx vif[%d] qos[%d] queue[%d] aifsn[%d] cwmin[%d] cwmax[%d] txop[%d] \n",
            priv_vif->vif_idx ,vif->bss_conf.qos, queue, params->aifs, params->cw_min, params->cw_max, params->txop);
#else
    printk("[I] sv6200_conf_tx queue[%d] aifsn[%d] cwmin[%d] cwmax[%d] txop[%d] \n",
            queue, params->aifs, params->cw_min, params->cw_max, params->txop);
#endif

    if (queue > NL80211_TXQ_Q_BK) {
        ret = 1;
        goto out;
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)    
    if ((priv_vif->vif_idx != 0) && (vif->p2p == 0) ) {
        dev_warn(sc->dev, "WMM setting applicable to primary interface only.\n");
        ret = 1;
        goto out;
    }
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
    //set QoS status
    SSV_SET_QOS_ENABLE(sc->sh, vif->bss_conf.qos);
#endif

    //set wmm parameter
    SSV_SET_WMM_PARAM(sc, params, queue);
out:
    return ret;
}


static bool ssv6xxx_is_ampdu_rx_sta(struct ssv_softc *sc, struct ieee80211_sta *sta)
{
    struct ssv_sta_info *sta_info;
    int i = 0;
    bool find_peer = false;

    for (i = 0; i < SSV_NUM_STA; i++) {
        sta_info = &sc->sta_info[i];
        if ((sta_info->s_flags & STA_FLAG_VALID) && (sta == sta_info->sta)) {/* sta_info is already protected by ssv6200_ampdu_action(). */
            if (sta_info->s_flags & STA_FLAG_AMPDU_RX) {/* sta_info is already protected by ssv6200_ampdu_action(). */
                find_peer = true;
            }
            break;
        }
    }

    return find_peer;
}

static int ssv6xxx_ampdu_opertaion(struct ssv_softc *sc, ssv6xxx_ampdu_ops ops, u16 tid, int wsid)
{
    struct sk_buff *skb = NULL;
    struct cfg_host_cmd *host_cmd = NULL;
    struct ssv_ampdu_param *ptr = NULL;
    int ret = 0;

    printk("%s() tid %d, wsid %d\n", __FUNCTION__, tid, wsid);
    skb = ssv_skb_alloc(sc, HOST_CMD_HDR_LEN + sizeof(struct ssv_ampdu_param));
    if (skb == NULL) {
        printk("%s(): Fail to alloc cmd buffer.\n", __FUNCTION__);
        return -1;
    }

    skb_put(skb, HOST_CMD_HDR_LEN + sizeof(struct ssv_ampdu_param));
    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
    host_cmd->c_type = HOST_CMD;
    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_AMPDU_OPS;
    host_cmd->sub_h_cmd = (u32)ops;
    host_cmd->blocking_seq_no = (((u16)SSV6XXX_HOST_CMD_AMPDU_OPS << 16)|(u16)ops);
    host_cmd->len = HOST_CMD_HDR_LEN + sizeof(struct ssv_ampdu_param);
    ptr = (struct ssv_ampdu_param *)host_cmd->un.dat8;
    memset(ptr, 0x0, sizeof(struct ssv_ampdu_param));
    ptr->tid = tid;
    ptr->wsid = (u8)wsid;
    ret = HCI_SEND_CMD(sc->sh, skb);

    return 0;
}

void ssv6xxx_set_ampdu_rx_add_work(struct work_struct *work)
{
    struct ssv_softc *sc = container_of(work, struct ssv_softc, set_ampdu_rx_add_work);
    SSV_SET_RX_BA(sc->sh, true, sc->ba_ra_addr, sc->ba_tid, sc->ba_ssn, 64);
}

void ssv6xxx_set_ampdu_rx_del_work(struct work_struct *work)
{
    struct ssv_softc*sc = container_of(work, struct ssv_softc, set_ampdu_rx_del_work);
    u8 addr[6] = { 0 };

    SSV_SET_RX_BA(sc->sh, false, addr, 0, 0, 0);
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,0,0)
static int ssv6200_ampdu_action(struct ieee80211_hw *hw,
        struct ieee80211_vif *vif,
        enum ieee80211_ampdu_mlme_action action,
        struct ieee80211_sta *sta,
        u16 tid, u16 *ssn)

#elif LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,0) && LINUX_VERSION_CODE < KERNEL_VERSION(4,4,0)
static int ssv6200_ampdu_action(struct ieee80211_hw *hw,
        struct ieee80211_vif *vif,
        enum ieee80211_ampdu_mlme_action action,
        struct ieee80211_sta *sta,
        u16 tid, u16 *ssn, u8 buf_size)
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(4,4,0) && LINUX_VERSION_CODE < KERNEL_VERSION(4,4,69)
static int ssv6200_ampdu_action(struct ieee80211_hw *hw,
        struct ieee80211_vif *vif,
        enum ieee80211_ampdu_mlme_action action,
        struct ieee80211_sta *sta,
        u16 tid, u16 *ssn, u8 buf_size, bool amsdu)
#else
static int ssv6200_ampdu_action(struct ieee80211_hw *hw,
        struct ieee80211_vif *vif,
        struct ieee80211_ampdu_params *params)
#endif


{
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,0,0)
    u8 buf_size = 32;
#endif
    struct ssv_softc *sc = hw->priv;
    struct ssv_sta_priv_data *sta_priv;
    struct ssv_sta_info *sta_info;
    bool find_peer = false;
    int ret = 0;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,4,69)
    struct ieee80211_sta *sta = params->sta;
    enum ieee80211_ampdu_mlme_action action = params->action;
    u16 tid = params->tid;
    u16 *ssn = &(params->ssn);
    u8 buf_size = params->buf_size;
#endif

    //printk("[I] %s(): \n", __FUNCTION__);

    if(sta == NULL)
        return ret;

    if((action == IEEE80211_AMPDU_RX_START || action == IEEE80211_AMPDU_RX_STOP ) &&
            (!(sc->sh->cfg.hw_caps & SSV6200_HW_CAP_AMPDU_RX)))
    {
        printk("Disable AMPDU_RX(2).\n");
        return -EOPNOTSUPP;
    }

    if(   (   action == IEEE80211_AMPDU_TX_START
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
                || action == IEEE80211_AMPDU_TX_STOP
#else
                || action == IEEE80211_AMPDU_TX_STOP_CONT
                || action == IEEE80211_AMPDU_TX_STOP_FLUSH
                || action == IEEE80211_AMPDU_TX_STOP_FLUSH_CONT
#endif
                || action == IEEE80211_AMPDU_TX_OPERATIONAL )
            && (!(sc->sh->cfg.hw_caps & SSV6200_HW_CAP_AMPDU_TX)))
    {
        printk("Disable AMPDU_TX(2).\n");
        return -EOPNOTSUPP;
    }

    mutex_lock(&sc->mutex);
    //down_read(&sc->sta_info_sem);/* For mac80211 cb, we use sc->mutex to protect sta_info instead of sta_info_sem. */
    if ((((struct ssv_sta_priv_data *)sta->drv_priv)->sta_idx == -1) ||
        ((sc->sta_info[((struct ssv_sta_priv_data *)sta->drv_priv)->sta_idx].s_flags & STA_FLAG_VALID) == 0)) {
        mutex_unlock(&sc->mutex);
        //up_read(&sc->sta_info_sem);/* For mac80211 cb, we use sc->mutex to protect sta_info instead of sta_info_sem. */
        if(!(sc->sc_flags & SC_OP_HW_RESET))
        {
            printk(KERN_WARNING "%s(): sta_info is gone.\n", __func__);
            return -ENODATA;
        }
        return ret;
    }

    switch (action)
    {
        case IEEE80211_AMPDU_RX_START:
            ret = SSV_AMPDU_RX_START(sc, hw, vif, sta, tid, ssn, buf_size);
            if (!ret)
                queue_work(sc->config_wq, &sc->set_ampdu_rx_add_work);
            break;

        case IEEE80211_AMPDU_RX_STOP:
            find_peer = ssv6xxx_is_ampdu_rx_sta(sc, sta);
            if (!find_peer)
                break;

            sta_priv = (struct ssv_sta_priv_data *)sta->drv_priv;
            sta_info = &sc->sta_info[sta_priv->sta_idx];

            sta_info->s_flags &= ~(STA_FLAG_AMPDU_RX);
            sc->rx_ba_session_count--;
            if (sc->rx_ba_session_count <= 0) {
                sc->rx_ba_sta = NULL;
                sc->rx_ba_session_count = 0;
            }

            queue_work(sc->config_wq, &sc->set_ampdu_rx_del_work);
            break;

        case IEEE80211_AMPDU_TX_START:
            printk(KERN_ERR "AMPDU_TX_START %02X:%02X:%02X:%02X:%02X:%02X %d.\n",
                    sta->addr[0], sta->addr[1], sta->addr[2], sta->addr[3],
                    sta->addr[4], sta->addr[5], tid);

            sta_priv = (struct ssv_sta_priv_data *)sta->drv_priv;
            if (sta_priv->ampdu_tid_state[tid] != SSV_AMPDU_1_3_TX_START) {
                //spin_lock_bh(&sta_priv->ampdu_ctrl_lock);
                sta_priv->ampdu_tid_state[tid] = SSV_AMPDU_1_3_TX_START;
                //spin_unlock_bh(&sta_priv->ampdu_ctrl_lock);
                ssv6xxx_ampdu_opertaion(sc, SSV6XXX_AMPDU_CMD_START, tid, sta_priv->sta_info->hw_wsid);
            }
            ieee80211_start_tx_ba_cb_irqsafe(vif, sta->addr, tid);
            break;

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
        case IEEE80211_AMPDU_TX_STOP:
#else
            /* @IEEE80211_AMPDU_TX_STOP_CONT: stop TX aggregation but continue transmitting
             *  queued packets, now unaggregated. After all packets are transmitted the
             *  driver has to call ieee80211_stop_tx_ba_cb_irqsafe().
             * @IEEE80211_AMPDU_TX_STOP_FLUSH: stop TX aggregation and flush all packets,
             *  called when the station is removed. There's no need or reason to call
             *  ieee80211_stop_tx_ba_cb_irqsafe() in this case as mac80211 assumes the
             *  session is gone and removes the station.
             * @IEEE80211_AMPDU_TX_STOP_FLUSH_CONT: called when TX aggregation is stopped
             *  but the driver hasn't called ieee80211_stop_tx_ba_cb_irqsafe() yet and
             *  now the connection is dropped and the station will be removed. Drivers
             *  should clean up and drop remaining packets when this is called.
             */
            // Freddie ToDo:
        case IEEE80211_AMPDU_TX_STOP_CONT:
        case IEEE80211_AMPDU_TX_STOP_FLUSH:
        case IEEE80211_AMPDU_TX_STOP_FLUSH_CONT:
#endif
            printk(KERN_ERR "AMPDU_TX_STOP %02X:%02X:%02X:%02X:%02X:%02X %d.\n",
                    sta->addr[0], sta->addr[1], sta->addr[2], sta->addr[3],
                    sta->addr[4], sta->addr[5], tid);
            sta_priv = (struct ssv_sta_priv_data *)sta->drv_priv;
            //spin_lock_bh(&sta_priv->ampdu_ctrl_lock);
            sta_priv->ampdu_tid_state[tid] = SSV_AMPDU_1_3_TX_STOP;
            //spin_unlock_bh(&sta_priv->ampdu_ctrl_lock);
            ssv6xxx_ampdu_opertaion(sc, SSV6XXX_AMPDU_CMD_STOP, tid, sta_priv->sta_info->hw_wsid);
            ieee80211_stop_tx_ba_cb_irqsafe(vif, sta->addr, tid);
            break;

        case IEEE80211_AMPDU_TX_OPERATIONAL:
            printk(KERN_ERR "AMPDU_TX_OPERATIONAL %02X:%02X:%02X:%02X:%02X:%02X %d.\n",
                    sta->addr[0], sta->addr[1], sta->addr[2], sta->addr[3],
                    sta->addr[4], sta->addr[5], tid);
            break;

        default:
            ret = -EOPNOTSUPP;//not support.
            break;
    }

    //up_read(&sc->sta_info_sem);/* For mac80211 cb, we use sc->mutex to protect sta_info instead of sta_info_sem. */
    mutex_unlock(&sc->mutex);

    return ret;
}

#ifdef CONFIG_ENABLE_ACS_FUNC
static int ssv6200_get_survey(struct ieee80211_hw *hw, int idx, struct survey_info *survey)
{
    struct ssv_softc *sc = hw->priv;
    struct ieee80211_supported_band *sband = NULL;
    struct survey_info *ssv_survey = &sc->survey[idx];
    int ret = 0;

    if(idx == 0)
    {
        SSV_EDCA_UPDATE_SURVEY(sc->sh);
    }

    sband = &sc->sbands[INDEX_80211_BAND_2GHZ]; //2G
    if (idx >= sband->n_channels)
    {
        idx -= sband->n_channels;
        sband = &sc->sbands[INDEX_80211_BAND_5GHZ]; //5G
    }

    if (idx >= sband->n_channels)
    {
        ret = -ENOENT;
        goto EXIT;
    }

    /* Fill survey information */
    memcpy((void *)survey, (const void *)ssv_survey, sizeof(struct survey_info));

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,0,0)
    //printk("channel(%u) time = %lld, time_busy = %lld, time_ext_busy = %lld\n", sband->channels[idx].hw_value, survey->time, survey->time_busy, survey->time_ext_busy);
#else
    //printk("channel(%u) time = %lld, time_busy = %lld, time_ext_busy = %lld\n", sband->channels[idx].hw_value, survey->channel_time, survey->channel_time_busy, survey->channel_time_ext_busy);
#endif
    //To avoid complains there is insufficient survey data.
    survey->filled |= SURVEY_INFO_NOISE_DBM;
    survey->noise = -92;

    survey->channel = &sband->channels[idx];

    if(sc->cur_channel == survey->channel)
    {
        survey->filled |= SURVEY_INFO_IN_USE;
    }


EXIT:
    return ret;
}
#endif

static int ssv6200_set_bitrate_mask(struct ieee80211_hw *hw,
        struct ieee80211_vif *vif, const struct cfg80211_bitrate_mask *mask)
{
    printk("%s start\n", __FUNCTION__);
    return 0;
}

static int ssv6200_set_rts_threshold(struct ieee80211_hw *hw, u32 value)
{
    struct ssv_softc *sc=hw->priv;
    struct sk_buff *skb = NULL;
    struct cfg_host_cmd *host_cmd = NULL;
    struct ssv6xxx_rts_param *ptr = NULL;
    int ret = 0;

    mutex_lock(&sc->mutex);
    skb = ssv_skb_alloc(sc, HOST_CMD_HDR_LEN + sizeof(struct ssv6xxx_rts_param));
    if (skb == NULL) {
        printk("%s(): Fail to alloc cmd buffer.\n", __FUNCTION__);
        goto out;
    }

    skb_put(skb, HOST_CMD_HDR_LEN + sizeof(struct ssv6xxx_rts_param));
    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
    host_cmd->c_type = HOST_CMD;
    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_RTS_THRESHOLD;
    host_cmd->len = HOST_CMD_HDR_LEN + sizeof(struct ssv6xxx_rts_param);

    ptr = (struct ssv6xxx_rts_param *)host_cmd->un.dat8;
    memset(ptr, 0x0, sizeof(struct ssv6xxx_rts_param));
    ptr->rts_threshold = value;
    ret = HCI_SEND_CMD(sc->sh, skb);
    if (ret)
        printk("Fail to send rts command\n");
    
out: 
    mutex_unlock(&sc->mutex);
    return 0;
}

int ssv6xxx_send_rawdata_packet(char *data, int len)
{
    struct ssv_softc *sc = ssv6xxx_driver_attach(SSV_DRVER_NAME);
    struct SKB_info_st *skb_info = NULL;
    struct sk_buff *tx_frame = NULL;
    
    if (NULL == sc) {
        printk("%s() Cannot find ssv driver\n", __FUNCTION__);
        return -EINVAL;
    }

    tx_frame = ssv_skb_alloc_ex(sc, len + TXPB_OFFSET, GFP_ATOMIC);
    if (NULL == tx_frame) 
        return  -EINVAL;
    
    if (skb_headroom(tx_frame) < sizeof(struct SKB_info_st)) {
        skb_reserve(tx_frame, sizeof(struct SKB_info_st));
    }
    
    // clear skb->cb, mac80211 store ieee80211_tx_info in skb->cb 
    memset(tx_frame->cb, 0, sizeof(tx_frame->cb)); 
    // set skb info
    skb_info = (struct SKB_info_st *)tx_frame->head;
    memset(skb_info, 0, sizeof (struct SKB_info_st));
    skb_info->sta = NULL;
    skb_info->raw_data = true;
    
    skb_put(tx_frame, len + TXPB_OFFSET);
    skb_pull(tx_frame, TXPB_OFFSET);
    memcpy(tx_frame->data, data, len);
    SSV_ADD_TXINFO(sc, tx_frame);
    // tx done task will free tx_frame
    HCI_SEND(sc->sh, tx_frame, SSV_SW_TXQ_ID_MNG, false);

    return 0;
}
//EXPORT_SYMBOL(ssv6xxx_send_rawdata_packet);

struct ieee80211_ops ssv6200_ops =
{
    /* MUST callback function: */
    .tx                 = ssv6200_tx,
    .start              = ssv6200_start,
    .stop               = ssv6200_stop,
    .add_interface      = ssv6200_add_interface,
    .change_interface   = ssv6200_change_interface,
    .remove_interface   = ssv6200_remove_interface,
    .config             = ssv6200_config,
    .configure_filter   = ssv6200_config_filter,


    /* OPTIONAL callback function: */
    .bss_info_changed   = ssv6200_bss_info_changed,
    .sta_add            = ssv6200_sta_add,
    .sta_remove         = ssv6200_sta_remove,
    .sta_notify         = ssv6200_sta_notify,
    .set_key            = ssv6200_set_key,
    .sw_scan_start      = ssv6200_sw_scan_start,
    .sw_scan_complete   = ssv6200_sw_scan_complete,
#ifdef CONFIG_SSV_VENDOR_OPS
    .send_rawdata       = ssv6xxx_send_rawdata_packet,
#endif
#if ((LINUX_VERSION_CODE >= KERNEL_VERSION(3,17,0)) && defined(CONFIG_HW_SCAN))
    .hw_scan            = ssv6200_hw_scan,
    .cancel_hw_scan     = ssv6200_cancel_hw_scan,
#endif
    .get_tsf            = ssv6200_get_tsf,
    .set_tim            = ssv6200_set_tim,
    .conf_tx            = ssv6200_conf_tx,
    .ampdu_action       = ssv6200_ampdu_action,
#ifdef CONFIG_ENABLE_ACS_FUNC	
	.get_survey         = ssv6200_get_survey,
#endif	
    .set_bitrate_mask   = ssv6200_set_bitrate_mask,
    .set_rts_threshold  = ssv6200_set_rts_threshold,
#ifdef CONFIG_PM
    .suspend            = ssv6xxx_suspend,
    .resume             = ssv6xxx_resume,
#endif
};

void ssv6xxx_enable_usb_acc(void *param, u8 epnum)
{
    struct ssv_softc *sc = (struct ssv_softc *)param;

    SSV_ENABLE_USB_ACC(sc, epnum);  
    return;
}

void ssv6xxx_disable_usb_acc(void *param, u8 epnum)
{
    struct ssv_softc *sc = (struct ssv_softc *)param;

    SSV_DISABLE_USB_ACC(sc, epnum); 
    return;
}

void ssv6xxx_jump_to_rom(void *param)
{
    struct ssv_softc *sc = (struct ssv_softc *)param;

    SSV_JUMP_TO_ROM(sc);    
    return;
}

int ssv6xxx_rx_mode(void *param)
{
    struct ssv_softc *sc = (struct ssv_softc *)param;
    struct ssv_hw *sh = sc->sh;

    return sh->rx_mode;
}

void ssv6200_tx_flow_control(struct ssv_softc *sc, bool fc_en, bool force_stop)
{
    int tx_frame = 0;

    if (sc->sh->cfg.flowctl) {

        if (sc->log_ctrl & LOG_FLOWCTL) {
            printk(KERN_ERR "hci_tx_frame = %d\n", HCI_TXQ_LEN(sc->sh));
        }

        sc->flowctl_hci_cnt = HCI_TXQ_LEN(sc->sh);   
        tx_frame = sc->flowctl_hci_cnt;   
        sc->flowctl_frame_cnt = tx_frame;

        if (fc_en) {
            if (!(sc->sc_flags & SC_OP_FLOWCTL)) {
                if (force_stop || (tx_frame > sc->sh->cfg.flowctl_high_threshold)) {
                    ieee80211_stop_queues(sc->hw);
                    sc->sc_flags |= SC_OP_FLOWCTL;
                    //printk("TXQ enable tx flow\n");
                }
            }
        } else {
            if (sc->sc_flags & SC_OP_FLOWCTL) {
                if (tx_frame < sc->sh->cfg.flowctl_low_threshold) {
                    ieee80211_wake_queues(sc->hw);
                    sc->sc_flags &= ~SC_OP_FLOWCTL;
                    //printk("TXQ disable tx flow\n");
                }
            }
        }
    }
}

#if 1
static u64 ssv6200_get_systime_us(void)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 20, 0))
        return ktime_to_us(ktime_get_boottime());
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 39))
        struct timespec ts;
        get_monotonic_boottime(&ts);
        return ((u64)ts.tv_sec * 1000000) + ts.tv_nsec / 1000;
#else
        struct timeval tv;
        do_gettimeofday(&tv);
        return ((u64)tv.tv_sec * 1000000) + tv.tv_usec;
#endif
}

#else
static u64 ssv6200_get_systime_us(void)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,39))
    struct timespec ts; 
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4,15,0))
	getnstimeofday(&ts);
#else
	get_monotonic_boottime(&ts);
#endif
    return ((u64)ts.tv_sec*1000000)+ts.tv_nsec/1000;
#else
    struct timeval tv;
    do_gettimeofday(&tv);
    return ((u64)tv.tv_sec*1000000)+tv.tv_usec;
#endif
}
#endif
static void _proc_data_rx_mib(struct ssv_softc *sc, struct sk_buff *skb)
{
    struct ieee80211_hdr *hdr = (struct ieee80211_hdr *)skb->data;
    
    if (ieee80211_is_data(hdr->frame_control))
        sc->rx.rx_data_count++;
    else if (ieee80211_is_mgmt(hdr->frame_control)) {
        sc->rx.rx_mgmt_count++;
        if (ieee80211_is_beacon(hdr->frame_control))
            sc->rx.rx_bcn_count++;
        else if (ieee80211_is_probe_resp(hdr->frame_control))
            sc->rx.rx_proberesp_count++;
        else if (ieee80211_is_probe_req(hdr->frame_control))
            sc->rx.rx_probereq_count++;
        else if (ieee80211_is_assoc_req(hdr->frame_control))
            sc->rx.rx_assoc_req_count++;
        else if (ieee80211_is_assoc_resp(hdr->frame_control))
            sc->rx.rx_assoc_resp_count++;
        else if (ieee80211_is_auth(hdr->frame_control))
            sc->rx.rx_auth_count++;
        else if (ieee80211_is_disassoc(hdr->frame_control))
            sc->rx.rx_disassoc_count++;
        else if (ieee80211_is_deauth(hdr->frame_control))
            sc->rx.rx_deauth_count++;
    }
}

//static int rx_count = 0;
// Process RX SKB, i.e. strip SSV header and appended data and send it to upper layer
static void ssv6xxx_proc_data_rx_skb (struct ssv_softc *sc, struct sk_buff *rx_skb)
{
    struct ieee80211_rx_status             *rxs;
    struct ieee80211_hdr                   *hdr;
    __le16                                  fc;
    struct ieee80211_vif                   *vif = NULL;
    struct ieee80211_sta                   *sta = NULL;
    bool                                    rx_hw_dec = false;
    bool                                    do_sw_dec = false;
    struct ssv_sta_priv_data               *sta_priv = NULL;
    struct ssv_vif_priv_data               *vif_priv = NULL;
    SKB_info *skb_info = NULL;
    u32 wsid, rate_idx, mng_used;
    bool aggr, tkip_mmic_err;
    
    /* extract headers */
    hdr = (struct ieee80211_hdr *)(rx_skb->data + HAL_GET_RX_DESC_SIZE(sc->sh));
    wsid = HAL_GET_RX_DESC_WSID(sc->sh, rx_skb);
    rate_idx= HAL_GET_RX_DESC_RATE_IDX(sc->sh, rx_skb);
    aggr = HAL_IS_RX_AGGR(sc->sh, rx_skb);
    mng_used = HAL_GET_RX_DESC_MNG_USED(sc->sh, rx_skb);

    fc = hdr->frame_control;
    skb_info = (SKB_info *)rx_skb->head;

    if(ieee80211_is_beacon(hdr->frame_control))
    {
        struct ieee80211_mgmt *mgmt_tmp = NULL;
        mgmt_tmp = (struct ieee80211_mgmt *)(rx_skb->data+HAL_GET_RX_DESC_SIZE(sc->sh));
        mgmt_tmp->u.beacon.timestamp = cpu_to_le64(ssv6200_get_systime_us()+(10*1000*1000));
    }

    if(ieee80211_is_probe_resp(hdr->frame_control))
    {
        struct ieee80211_mgmt *mgmt_tmp = NULL;
        mgmt_tmp = (struct ieee80211_mgmt *)(rx_skb->data+HAL_GET_RX_DESC_SIZE(sc->sh));
        mgmt_tmp->u.probe_resp.timestamp = cpu_to_le64(ssv6200_get_systime_us()+(10*1000*1000));
    }

    /*
    // Parser connected STA data rate(Rate control)
    WSID0 & WSID1 we use rate control alogrithm.
    WSID2 - WSID7 based on the other side of the data rate.
     */
    HAL_RC_RX_DATA_HANDLER(sc, rx_skb, rate_idx);

    /* prepare rx status for mac80211 */
    rxs = IEEE80211_SKB_RXCB(rx_skb);
    memset(rxs, 0, sizeof(struct ieee80211_rx_status));

    SSV_UPDATE_MAC80211_CHAN_INFO(sc, rx_skb, rxs);
    rxs->antenna = 1;
    SSV_RC_MAC80211_RATE_IDX(sc, rate_idx, rxs);

    #ifdef CONFIG_SSV_CTL
    {
        void ssv_ctl_nl_send_msg(struct sk_buff *skb);

        if (sc->ssv_smartlink_status)
        {
            /* remove ssv6xxx headers before passing to mac80211 */
            skb_pull(rx_skb,  HAL_GET_RX_DESC_SIZE(sc->sh));
            skb_trim(rx_skb, rx_skb->len-sc->sh->rx_pinfo_pad);

            ssv_ctl_nl_send_msg(rx_skb);
            goto drop_rx;
        }
    }
    #endif

    if(sc->sc_flags & SC_OP_MONITOR)
    {
        /* remove ssv6xxx headers before passing to mac80211 */
        skb_pull(rx_skb,  HAL_GET_RX_DESC_SIZE(sc->sh));
        skb_trim(rx_skb, rx_skb->len-sc->sh->rx_pinfo_pad);
        goto monitor;
    }

    sta = ssv6xxx_find_sta_by_rx_skb(sc, rx_skb);
    if (sta){
        struct ieee80211_hdr *hdr = (struct ieee80211_hdr *)(rx_skb->data + sc->sh->rx_desc_len);
        if (ieee80211_is_data(hdr->frame_control)){
            sc->rx_data_exist = true;
        }
    }
    if (HAL_UPDATE_RXSTATUS(sc, rx_skb, rxs) == -1){
        printk(" KRACK detected!!\n");
        goto drop_rx;
    }

#if LINUX_VERSION_CODE >= 0x030400
    if (aggr)
        rxs->flag |= RX_FLAG_NO_SIGNAL_VAL;
#endif

    /* update Mng queue status to let host know*/
    sc->hw_mng_used = mng_used;

    // Drop frame beacon for disassoc with station info
    if (ieee80211_is_mgmt(fc) && (sta != NULL)) {
        sta_priv = (struct ssv_sta_priv_data *)sta->drv_priv;
#ifndef IRQ_PROC_RX_DATA
        down_read(&sc->sta_info_sem);/* START protect sta_info */
#else
        while(!down_read_trylock(&sc->sta_info_sem));/* START protect sta_info */
#endif
        if ((sta_priv->sta_idx == -1) ||
            ((sc->sta_info[sta_priv->sta_idx].s_flags & STA_FLAG_VALID) == 0)) {
            up_read(&sc->sta_info_sem);/* END protect sta_info */
            goto drop_rx;
        }
        vif = sc->sta_info[sta_priv->sta_idx].vif;
        if (vif == NULL) {
            up_read(&sc->sta_info_sem);/* END protect sta_info */
            goto drop_rx;
        }
        if ((vif->type == NL80211_IFTYPE_STATION) && (vif->bss_conf.assoc == false)) {
            up_read(&sc->sta_info_sem);/* END protect sta_info */
            goto drop_rx;

        }
        up_read(&sc->sta_info_sem);/* END protect sta_info */
    }

    /*
     * Set protect bit of IEEE 802.11 data frame header  to 0 if
     * hardware security offload engine is enabled for security mode.
     */
    // Check decryption status of the protected data frame
    if (   (ieee80211_is_data(fc) || ieee80211_is_data_qos(fc))
            && ieee80211_has_protected(fc))
    {
        // for NL80211_IFTYPE_STATION, wsid would be 0, 1, e.
        // for NL80211_IFTYPE_AP, wisd would be 0, 1, f for no WSID WATCH LIST
        // for NL80211_IFTYPE_AP, wisd would be 0~7 for WSID WATCH list_head
        sta = ssv6xxx_find_sta_by_rx_skb(sc, rx_skb);

        // Encrypted RX packet must be for connected STA.
        if (sta == NULL)
            goto drop_rx;

        sta_priv = (struct ssv_sta_priv_data *)sta->drv_priv;
        //tx_desc->tx_report = 1;

#ifndef IRQ_PROC_RX_DATA
        down_read(&sc->sta_info_sem);/* START protect sta_info */
#else
        while(!down_read_trylock(&sc->sta_info_sem));/* START protect sta_info */
#endif
        if ((sta_priv->sta_idx == -1) ||
            ((sc->sta_info[sta_priv->sta_idx].s_flags & STA_FLAG_VALID) == 0)) {
            up_read(&sc->sta_info_sem);/* END protect sta_info */
            goto drop_rx;
        }
        vif = sc->sta_info[sta_priv->sta_idx].vif;
        if (vif == NULL) {
            up_read(&sc->sta_info_sem);/* END protect sta_info */
            goto drop_rx;
        }
        if ((vif->type == NL80211_IFTYPE_STATION) && (vif->bss_conf.assoc == false)) {
            up_read(&sc->sta_info_sem);/* END protect sta_info */
            goto drop_rx;
        }

        if (is_broadcast_ether_addr(hdr->addr1))
        {
            vif_priv = (struct ssv_vif_priv_data *)vif->drv_priv;
            rx_hw_dec = vif_priv->has_hw_decrypt;
            do_sw_dec = vif_priv->need_sw_decrypt;
        }
        else
        {
            rx_hw_dec = sta_priv->has_hw_decrypt;
            do_sw_dec = sta_priv->need_sw_decrypt;
        }
        up_read(&sc->sta_info_sem);/* END protect sta_info */
#if 0
        if (rx_count++ < 20)
        {
            printk(KERN_ERR "HW DEC (%d - %d) %d %02X:%02X:%02X:%02X:%02X:%02X\n",
                    rx_hw_dec, do_sw_dec, rxdesc->wsid,
                    hdr->addr1[0], hdr->addr1[1], hdr->addr1[2],
                    hdr->addr1[3], hdr->addr1[4], hdr->addr1[5]);
            _ssv6xxx_hexdump("M ", (const u8 *)rx_skb->data, (rx_skb->len > 128) ? 128 : rx_skb->len);
        }
#endif // 0
#if 0
        dev_err(sc->dev, "R %02X:%02X:%02X:%02X:%02X:%02X %d %d\n",
                hdr->addr2[0], hdr->addr2[1], hdr->addr2[2],
                hdr->addr2[3], hdr->addr2[4], hdr->addr2[5],
                rx_hw_dec, do_sw_dec);
        _ssv6xxx_hexdump("R ", (const u8 *)rx_skb->data,
                (rx_skb->len > 128) ? 128 : rx_skb->len);
#endif // 0
    }

    if (sc->dbg_rx_frame)
    {
        _ssv6xxx_hexdump("================================================================\n"
                "RX frame", (const u8 *)rx_skb->data, rx_skb->len);
    }

    tkip_mmic_err = SSV_GET_TKIP_MMIC_ERR(sc->sh, rx_skb);
    /* remove ssv6xxx headers before passing to mac80211 */
    skb_pull(rx_skb, sc->sh->rx_desc_len);
    skb_trim(rx_skb, rx_skb->len - sc->sh->rx_pinfo_pad);

    if (rx_hw_dec || do_sw_dec)
    {

        hdr = (struct ieee80211_hdr *)rx_skb->data;
        rxs = IEEE80211_SKB_RXCB(rx_skb);
        hdr->frame_control = hdr->frame_control & ~(cpu_to_le16(IEEE80211_FCTL_PROTECTED));
        rxs->flag |= (RX_FLAG_DECRYPTED|RX_FLAG_IV_STRIPPED);

        if (tkip_mmic_err) {
            rxs->flag |= RX_FLAG_MMIC_ERROR;
            printk("TKIP MMIC error, set RX_FLAG_MMIC_ERROR flag\n");
        }
    }

    // Debug code to check received broadcast frame.
#if 0
    if (   is_broadcast_ether_addr(hdr->addr1)
            && (ieee80211_is_data_qos(fc) || ieee80211_is_data(fc)))
#endif
#if 0
        if (ieee80211_is_probe_req(fc))
        {
#if 0
            printk(KERN_ERR "RX M: 1 %02X:%02X:%02X:%02X:%02X:%02X  (%d - %d - %d)\n",
                    hdr->addr1[0], hdr->addr1[1], hdr->addr1[2],
                    hdr->addr1[3], hdr->addr1[4], hdr->addr1[5],
                    (le16_to_cpu(hdr->seq_ctrl) >> 4),
                    rxdesc->wsid, ieee80211_has_protected(fc));
#endif
            //printk(KERN_ERR "RX M: 2 %02X:%02X:%02X:%02X:%02X:%02X\n",
            printk(KERN_ERR "Probe Req: 2 %02X:%02X:%02X:%02X:%02X:%02X\n",
                    hdr->addr2[0], hdr->addr2[1], hdr->addr2[2],
                    hdr->addr2[3], hdr->addr2[4], hdr->addr2[5]);
#if 0
            printk(KERN_ERR "RX M: 3 %02X:%02X:%02X:%02X:%02X:%02X\n",
                    hdr->addr3[0], hdr->addr3[1], hdr->addr3[2],
                    hdr->addr3[3], hdr->addr3[4], hdr->addr3[5]);
#endif
            _ssv6xxx_hexdump("RX frame", (const u8 *)rx_skb->data,
                    (rx_skb->len > 128) ? 128 : rx_skb->len);
        }
#endif // DEBUG CODE

monitor:
    if ((sc->sc_flags & SC_OP_DEV_READY) && (sc->sc_flags & SC_OP_IF_UP))
    {
        if (sc->sh->cfg.rx_ip_csum_off) {
            /* Turn off IP checksum to improve Rx throughput */
            rx_skb->ip_summed = CHECKSUM_UNNECESSARY;
        }
        // rx frame mib
        _proc_data_rx_mib(sc, rx_skb);
#if defined(USE_THREAD_RX) && !defined(IRQ_PROC_RX_DATA)

            local_bh_disable();
            ieee80211_rx(sc->hw, rx_skb);
            local_bh_enable();
#else
            ieee80211_rx_irqsafe(sc->hw, rx_skb);
#endif // USE_THREAD_RX
    }
    else
        goto drop_rx;

    return;

drop_rx:
#if 0
    dev_err(sc->dev, "D %02X:%02X:%02X:%02X:%02X:%02X\n",
            hdr->addr2[0], hdr->addr2[1], hdr->addr2[2],
            hdr->addr2[3], hdr->addr2[4], hdr->addr2[5]);
#endif
    dev_kfree_skb_any(rx_skb);
} // end of - ssv6xxx_proc_data_rx_skb -


#ifdef IRQ_PROC_RX_DATA
static int ssv6xxx_proc_rx_skb (struct ssv_softc *sc, struct sk_buff *rx_skb)
{
    u32 packet_len = 0, c_type = 0, tx_pkt_run_no = 0;

    /* extract packet_length */
    SSV_GET_RX_DESC_INFO_HDR(sc->sh, rx_skb->data, &packet_len, &c_type, &tx_pkt_run_no);

    if ((M0_RXEVENT != c_type) && (M2_RXEVENT != c_type)) {
        return EPERM;
    }
    
    /* If hwif is 4byte alignment, we must remove the padding */
    if(rx_skb->len != packet_len) {
        if (rx_skb->len < packet_len) {
            dev_warn(sc->dev, "%s(): incorrect frame length %d[%d][%d]\n", __FUNCTION__, rx_skb->len, packet_len, c_type);
            dev_kfree_skb_any(rx_skb);
            return 0;
        }
        skb_trim(rx_skb, packet_len);
    }

    ssv6xxx_proc_data_rx_skb(sc, rx_skb);
    return 0;
} // end of - _proc_rx_skb -
#endif // IRQ_PROC_RX_DATA

#define BEACON_MISS_RSSI_THRESHOLD (-80)
void ssv6xxx_beacon_miss_work(struct work_struct *work)
{
    struct ssv_softc *sc = container_of(work, struct ssv_softc, beacon_miss_work);
    struct ssv_vif_priv_data *vif_priv = NULL;
    struct sk_buff *skb = NULL;
    struct ieee80211_hdr_3addr *nullfunc = NULL;
    int tx_desc_size = 0, vif_idx = 0, signal = 0;
    int ret = 0;
    int i = 0;
    struct ieee80211_sta *sta = NULL;
    struct ssv_sta_priv_data *sta_priv_iter;
    u8 macaddr[ETH_ALEN];
    struct SKB_info_st *skb_info;

    memcpy(macaddr, sc->sh->cfg.maddr[0], ETH_ALEN);
    skb = ssv_skb_alloc(sc, sizeof(struct ieee80211_hdr_3addr));
    if (!skb)
        return;

    down_read(&sc->sta_info_sem);/* START protect sta_info */

    //It must be station mode, so find the first station info
    for (i = 0; i < SSV6200_MAX_VIF; i++) {
        if (sc->vif_info[i].vif == NULL)
            continue;

        if ((sc->vif_info[i].vif->type == NL80211_IFTYPE_STATION) &&
                (sc->vif_info[i].vif->p2p == false) &&
                (sc->vif_info[i].vif->bss_conf.assoc == true))
            break;
    }
    if (i == SSV6200_MAX_VIF)
        goto out;

    vif_priv = (struct ssv_vif_priv_data *)sc->vif_info[i].vif->drv_priv;

    if (list_empty(&vif_priv->sta_list)) {
        printk("%s(): sta_list is empty.\n", __func__);
        goto out;
    }

    list_for_each_entry(sta_priv_iter, &vif_priv->sta_list, list)
    {
        if (sta_priv_iter->sta_idx == -1)
            continue;
        if (sc->sta_info[sta_priv_iter->sta_idx].s_flags & STA_FLAG_VALID)
            break;
    }
    if(sta_priv_iter->sta_idx == -1)
        goto out;

    /* If rssi < threshold, it should not to send fake beacon to mac80211 */
    signal = -(sta_priv_iter->beacon_rssi >> RSSI_DECIMAL_POINT_SHIFT);
    if (signal < BEACON_MISS_RSSI_THRESHOLD)
        goto out;

    sta = sc->sta_info[sta_priv_iter->sta_idx].sta; 
    if (!sta)
        goto out;

    // should clean skb info to let tx_frame count correct.
    skb_info = (struct SKB_info_st *)skb->head;
    memset(skb_info, 0, sizeof (struct SKB_info_st));   

    vif_idx = sc->vif_info[i].vif_priv->vif_idx;
    tx_desc_size = SSV_GET_TX_DESC_SIZE(sc->sh);
    skb_put(skb, tx_desc_size+sizeof(struct ieee80211_hdr_3addr));

    //update null data header
    nullfunc = (struct ieee80211_hdr_3addr *)(skb->data + tx_desc_size);
    memset(nullfunc, 0, sizeof(struct ieee80211_hdr_3addr));
    // update sequence
    sc->tx.seq_no += 0x10;
    nullfunc->seq_ctrl &= cpu_to_le16(IEEE80211_SCTL_FRAG);
    nullfunc->seq_ctrl |= cpu_to_le16(sc->tx.seq_no);
    // update frame control & mac addr
    nullfunc->frame_control = cpu_to_le16(IEEE80211_FTYPE_DATA |
            IEEE80211_STYPE_NULLFUNC |
            IEEE80211_FCTL_TODS);
    memcpy(nullfunc->addr1, sc->bssid[vif_idx], ETH_ALEN);
    memcpy(nullfunc->addr2, macaddr, ETH_ALEN);
    memcpy(nullfunc->addr3, sc->bssid[vif_idx], ETH_ALEN);

    //update tx description
    ret = SSV_UPDATE_NULL_FUNC_TXINFO(sc, sta, skb);
    if (ret < 0) 
        goto out;

    ret = HCI_SEND(sc->sh, skb, SSV_SW_TXQ_ID_MNG, false);
    if (ret < 0)
        goto out;

    up_read(&sc->sta_info_sem);/* END protect sta_info */
    return;

out:
    up_read(&sc->sta_info_sem);/* END protect sta_info */
    dev_kfree_skb_any(skb);
}

static void ssv6xxx_ack_ctl_notify_process(struct ssv_softc *sc, u8 ack, u8 seq_no)
{
    struct sk_buff *skb = NULL;
    struct ieee80211_tx_info *tx_info;
    struct ieee80211_hdr *hdr = NULL;
    int qlen = 0;
    int i = 0;
    int wsid = 0;
    u8 sw_ack_seq = 0;

    qlen = (int)skb_queue_len(&sc->tx_ack_ctl_q);

    for (i = 0; i < qlen; i++) {
        skb = skb_dequeue(&sc->tx_ack_ctl_q);
        if (skb) {
            struct SKB_info_st *mpdu_skb_info_p = (SKB_info *)(skb->head);
            if (mpdu_skb_info_p->directly_ack) {
                printk("sw ack use directack\n");
                ssv_skb_free(sc, skb);
                ssv6200_tx_flow_control(sc, false, false);
                continue;
            }
            sw_ack_seq = SSV_GET_SW_ACK_SEQ(sc->sh, skb);
            if (seq_no == sw_ack_seq) {
                
                tx_info = IEEE80211_SKB_CB(skb);
                wsid = HAL_GET_TX_DESC_WSID(sc->sh, skb);
                skb_pull(skb, SSV_GET_TX_DESC_SIZE(sc->sh));
                
                if (0 == ack) {
                    //ssv_skb_free(sc, skb);
                    // show nack eapol frame seqno for debug
                    hdr = (struct ieee80211_hdr *)skb->data;
                    printk("Ack ctl frame[%d] cannot receive ack\n", (le16_to_cpu(hdr->seq_ctrl) >> 4));
               #if 0
                    ssv_skb_free(sc, skb);
               #else
                    ieee80211_tx_info_clear_status(tx_info);
                    tx_info->flags &= ~IEEE80211_TX_STAT_ACK;
                    tx_info->flags |=IEEE80211_TX_CTL_INJECTED;
                    tx_info->status.ack_signal = 100; /* ???? */
                    tx_info->status.rates[1].idx = -1;
                    ieee80211_tx_status(sc->hw, skb);
               #endif 
                } else {
                    hdr = (struct ieee80211_hdr *)skb->data;
                    printk("Ack ctl frame[%d] receive ack\n", (le16_to_cpu(hdr->seq_ctrl) >> 4));
                    ssvxxx_complete_tx_skb(sc, tx_info, skb, wsid);
                }
                ssv6200_tx_flow_control(sc, false, false);
                break;
            } else {
                skb_queue_tail(&sc->tx_ack_ctl_q, skb);
            }
        }
    }

    if (i == qlen) 
        printk("Cannot find eapol frame\n");
}

void process_host_cmd_done_event(struct ssv_softc *sc, u8 *data, int len)
{
    struct cfg_host_event *h_evt = (struct cfg_host_event *)data;
   
    switch (h_evt->h_event) {
        case SOC_EVT_RFPHY_OPS:
            HCI_CMD_DONE(sc->sh, data, len);
            break;
        case SOC_EVT_SECURITY:
            HCI_CMD_DONE(sc->sh, data, len);
            break;
        case SOC_EVT_AMPDU_OPS:
            HCI_CMD_DONE(sc->sh, data, len);
            break;
        case SOC_EVT_RC_OPS:
            HCI_CMD_DONE(sc->sh, data, len);
            break;
        case SOC_EVT_TX_OPS:
            HCI_CMD_DONE(sc->sh, data, len);
            break;
        case SOC_EVT_VIF_OPS:
            HCI_CMD_DONE(sc->sh, data, len);
            break;
        case SOC_EVT_ADAPTIVE:
            HCI_CMD_DONE(sc->sh, data, len);
            break;
        default:
            break;
    }
}

static bool _process_host_event(struct ssv_softc *sc, struct sk_buff *skb)
{
#define MAX_RFPHY_REG_VERSION_LEN   8
    u32                           c_type;
    struct cfg_host_event         *h_evt;
    int i = 0;
    u8 *ptr = NULL;
    u8 version[MAX_RFPHY_REG_VERSION_LEN];
    u32 addr = 0, value = 0;

    c_type = HAL_GET_RX_DESC_CTYPE(sc->sh, skb);

    if (c_type != HOST_EVENT)
        return false;

    h_evt = (struct cfg_host_event *)skb->data;
    switch (h_evt->h_event) {
        case SOC_EVT_SDIO_TEST_COMMAND:
            //printk("h_evt->evt_seq_no=%d\n", h_evt->evt_seq_no);
            if (h_evt->evt_seq_no == 0) {
                printk("SOC_EVT_SDIO_TEST_COMMAND\n");
                sc->sdio_rx_evt_size = h_evt->len;
                sc->sdio_throughput_timestamp = jiffies;
            } else {
                sc->sdio_rx_evt_size += h_evt->len;
                if (time_after(jiffies, 
                            (  sc->sdio_throughput_timestamp 
                               + msecs_to_jiffies(1000)))) {
                    printk("data[%ld] SDIO RX throughput %ld Kbps\n",
                            sc->sdio_rx_evt_size,
                            (  (sc->sdio_rx_evt_size << 3) 
                               / jiffies_to_msecs(  jiffies 
                                   - sc->sdio_throughput_timestamp)));
                    sc->sdio_throughput_timestamp = jiffies;
                    sc->sdio_rx_evt_size = 0;
                }
            }
            dev_kfree_skb_any(skb);
            break;
        case SOC_EVT_RESET_HOST:
            dev_kfree_skb_any(skb);
            if ((sc->ap_vif == NULL) && (sc->sh->cfg.online_reset & ONLINE_RESET_ENABLE)) {
                queue_work(sc->config_wq, &sc->hw_restart_work);
            } else {
                dev_warn(sc->dev, "Reset event ignored.\n");
            }
            break;
        case SOC_EVT_SDIO_TXTPUT_RESULT:
            printk("data SDIO TX throughput %d Kbps\n", h_evt->evt_seq_no);
            dev_kfree_skb_any(skb);
            break;
        case SOC_EVT_TXLOOPBK_RESULT:
            if (h_evt->evt_seq_no == SSV6XXX_STATE_OK) {
                printk("FW TX LOOPBACK OK\n");
                sc->iq_cali_done = IQ_CALI_OK;
            } else {
                printk(KERN_ERR "FW TX LOOPBACK FAILED\n");
                sc->iq_cali_done = IQ_CALI_FAILED;
            }
            dev_kfree_skb_any(skb);
            wake_up_interruptible(&sc->fw_wait_q);
            break;
        case SOC_EVT_BEACON_LOSS:
            dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_BEACON, "beacon dectection trigger\n");
            dev_kfree_skb_any(skb);
            queue_work(sc->config_wq, &sc->beacon_miss_work);

            if (sc->isAssoc)
                SSV_BEACON_LOSS_ENABLE(sc->sh);

            break;
        case SOC_EVT_TX_STUCK_RESP:
            printk("receive event tx_stuck!!\n");
            dev_kfree_skb_any(skb);
            break;
        case SOC_EVT_SW_BEACON_RESP:
            printk("soft-beacon start!!\n");
            dev_kfree_skb_any(skb);
            break;
        case SOC_EVT_INCORRECT_CHAN_BW:
            printk("The chip cannot support HT40.\n");
            dev_kfree_skb_any(skb);
            break;
        case SOC_EVT_PS:
            SSV_PS_EVT_HANDLER(sc, skb);
            dev_kfree_skb_any(skb);
            break;
        case SOC_EVT_DUMP_PHY_REG:
        case SOC_EVT_DUMP_RF_REG:
            ptr = (u8 *)h_evt->dat;

            memset(version, 0x0, sizeof(version));
            snprintf(version, sizeof(version) - 1, "%s", ptr);
            printk("rf/phy version %s\n", version);

            ptr += MAX_RFPHY_REG_VERSION_LEN;
            for (i = 0; i < (skb->len - sizeof(struct cfg_host_event) - MAX_RFPHY_REG_VERSION_LEN) / 8; i++) {
                memcpy(&addr, ptr, 4);
                ptr += 4;
                memcpy(&value, ptr, 4);
                ptr += 4;
                printk("0x%08x = 0x%08x\n", addr, value);
            }
            dev_kfree_skb_any(skb);
            break;
        case SOC_EVT_FW_NOTIFY:
            ptr = (u8 *)h_evt->dat;
            if(true == ((struct ssv_fw_notify_param *)ptr)->rate_update)
            {
                memcpy((void *)&(sc->hw_cur_rate[0]), (const void *)&(((struct ssv_fw_notify_param *)ptr)->cur_rate[0]), MAX_STA_NUM);
            }
            if(true == ((struct ssv_fw_notify_param *)ptr)->status_cnt_update)
            {
                sc->fw_cur_status_cnt = ((struct ssv_fw_notify_param *)ptr)->cur_status_cnt;
            }
            dev_kfree_skb_any(skb);
            break;
        case SOC_EVT_SNIFFER_NOTIFY:
            dev_kfree_skb_any(skb);
            break;
        case SOC_EVT_SW_ACK_NOTIFY:
            ptr = (u8 *)h_evt->dat;
            ssv6xxx_ack_ctl_notify_process(sc, ((struct ssv6xxx_sw_ack_evt *)ptr)->success, 
                        ((struct ssv6xxx_sw_ack_evt *)ptr)->seq_no);
            dev_kfree_skb_any(skb);
            break;
        default:
            dev_kfree_skb_any(skb);
            break;
    }

    return true;
} 

static int ssv6xxx_rx_skb_reserved_room(struct ssv_softc *sc, u8 *pbdata)
{
    u32 packet_len = 0, c_type = 0, tx_pkt_run_no = 0, hdrlen = 0;
    struct ieee80211_hdr *rhdr = NULL;
    int reserved_room = 0;
     
    if (sc->sh->cfg.rx_ip_align) {
        /* Align 802.3 payload to 4n+2 to improve Rx throughput.
         * mac80211 header translate from 802.11 to 802.3(header len is 14)
         * 802.3 packet address will be 4n+2+14 is 4-align to prevent mac80211 from memmove.
         */
        SSV_GET_RX_DESC_INFO_HDR(sc->sh, pbdata, &packet_len, &c_type, &tx_pkt_run_no); 
        if (M0_RXEVENT == c_type) { 
            
            rhdr = (struct ieee80211_hdr *)(pbdata + RXPB_OFFSET);
            hdrlen = ieee80211_hdrlen(rhdr->frame_control);
            if (0 == (hdrlen & 0x03)) {
                reserved_room = 2;
            }
        }
    }
    
    return reserved_room;
}

void ssv6xxx_process_rx_q(struct ssv_softc *sc, struct sk_buff_head *rx_q)
{
    struct sk_buff                *skb, *sskb;
    u32                           packet_len, c_type, tx_pkt_run_no;
    int                            rx_mode = sc->sh->rx_mode;
    int                            data_offset, data_length, reserved_room;
    unsigned char                 *pdata, *psdata;
    struct hci_rx_aggr_info       *rx_aggr_info;
#ifdef CONFIG_PREEMPT_NONE
    u32 max_rx_num = sc->sh->cfg.rx_non_preempt_num;
    u32 cur_rx_num = 0;
#endif //CONFIG_PREEMPT_NONE


    while (1) {
#ifdef CONFIG_PREEMPT_NONE
        if ((cur_rx_num++) > max_rx_num)
        {
            cur_rx_num = 0;
            schedule();
        }
#endif //CONFIG_PREEMPT_NONE

        skb = skb_dequeue(rx_q);
        if (!skb) {
            break;
        }
        
        sc->rx.rx_count++;
        
        data_length = skb->len;
        pdata = skb->data;
        for (data_offset = 0; data_offset < data_length; ) {
            if (rx_mode == RX_NORMAL_MODE) {
                sskb = skb;
                data_offset = skb->len;
            } else {
                
                if ((data_offset + sizeof(struct hci_rx_aggr_info)) > data_length) {
                    //printk("wrong data length, data_offset %d, data_length %d\n",
                    //    data_offset, data_length);
                    break; 
                }

                rx_aggr_info = (struct hci_rx_aggr_info *)pdata;
                if ((rx_aggr_info->jmp_mpdu_len == 0) ||
                        ((rx_aggr_info->jmp_mpdu_len - sizeof(struct hci_rx_aggr_info)) > MAX_FRAME_SIZE) ||
                        ((data_offset + rx_aggr_info->jmp_mpdu_len) > data_length)) {

                    break;
                }
                
                sskb = ssv_skb_alloc(sc, rx_aggr_info->jmp_mpdu_len);
                if (!sskb) {
                    dev_warn(sc->dev, "%s(): cannot alloc skb buffer\n", __FUNCTION__);
                    data_offset += rx_aggr_info->jmp_mpdu_len;
                    pdata = (u8 *)skb->data + data_offset;
                    continue;
                }

                reserved_room = ssv6xxx_rx_skb_reserved_room(sc, (u8 *)pdata+sizeof(struct hci_rx_aggr_info));
                if (0 != reserved_room)
                    skb_reserve(sskb, reserved_room);
                
                psdata = skb_put(sskb, (rx_aggr_info->jmp_mpdu_len - sizeof(struct hci_rx_aggr_info)));
                memcpy(psdata, pdata+sizeof(struct hci_rx_aggr_info),
                        (rx_aggr_info->jmp_mpdu_len - sizeof(struct hci_rx_aggr_info)));

                /* relocate next packet header */
                data_offset += rx_aggr_info->jmp_mpdu_len;
                pdata = (u8 *)skb->data + data_offset;
            }

            /* extract packet_length */
            SSV_GET_RX_DESC_INFO_HDR(sc->sh, sskb->data, &packet_len, &c_type, &tx_pkt_run_no);

            /* If hwif is 4byte alignment, we must remove the padding */
            if(sskb->len != packet_len) {
                if (sskb->len < packet_len) {
                    dev_warn(sc->dev, "Incorrect frame length %d[%d][%d]\n", sskb->len, packet_len, c_type);
                    dev_kfree_skb_any(sskb);
                    continue;
                }
                skb_trim(sskb, packet_len);
            }

            if (c_type == RATE_RPT) {
                dev_kfree_skb_any(sskb);
                continue;
            }

            if (_process_host_event(sc, sskb))
                continue;

            ssv6xxx_proc_data_rx_skb(sc, sskb);
        }
        
        if (rx_mode != RX_NORMAL_MODE)
            ssv_skb_free((void *)sc, skb);
    }// end of while (1)

} // end of - ssv6xxx_process_rx_q -

int ssv6200_is_rx_q_full(void *args)
{
    struct ssv_softc *sc=args;

    if (skb_queue_len(&sc->rx_skb_q) > sc->sh->cfg.rx_threshold) 
        return 1;
    else
        return 0;
    
}

int ssv6200_rx(struct sk_buff *rx_skb, void *args)
{
    struct ssv_softc *sc=args;
    int rx_mode = sc->sh->rx_mode;
    int data_offset, data_length;
    unsigned char *pdata;
    u32 packet_len, c_type, tx_pkt_run_no;
    struct hci_rx_aggr_info *rx_aggr_info;

    data_length = rx_skb->len;
    pdata = rx_skb->data;
    for (data_offset = 0; data_offset < data_length; ) {
        if (rx_mode == RX_NORMAL_MODE) {
             
             SSV_GET_RX_DESC_INFO_HDR(sc->sh, rx_skb->data, &packet_len, &c_type, &tx_pkt_run_no);
             if (HOST_EVENT == c_type) {
                process_host_cmd_done_event(sc, rx_skb->data, rx_skb->len);
             }
             break;

        } else {
            
            if ((data_offset + sizeof(struct hci_rx_aggr_info)) > data_length) {
                //printk("wrong data length, data_offset %d, data_length %d\n",
                //    data_offset, data_length);
                break; 
            }

            rx_aggr_info = (struct hci_rx_aggr_info *)pdata;
            if ((rx_aggr_info->jmp_mpdu_len == 0) ||
                    ((rx_aggr_info->jmp_mpdu_len - sizeof(struct hci_rx_aggr_info)) > MAX_FRAME_SIZE) ||
                    ((data_offset + rx_aggr_info->jmp_mpdu_len) > data_length)) {
                break;
            }
            
             SSV_GET_RX_DESC_INFO_HDR(sc->sh, (u8 *)pdata+sizeof(struct hci_rx_aggr_info), 
                    &packet_len, &c_type, &tx_pkt_run_no);
             
             if (HOST_EVENT == c_type) {
                process_host_cmd_done_event(sc, pdata+sizeof(struct hci_rx_aggr_info), 
                        (rx_aggr_info->jmp_mpdu_len - sizeof(struct hci_rx_aggr_info)));
             }

            /* relocate next packet header */
            data_offset += rx_aggr_info->jmp_mpdu_len;
            pdata = (u8 *)rx_skb->data + data_offset;
        }
    }

#ifdef IRQ_PROC_RX_DATA
    if (0 == ssv6xxx_proc_rx_skb(sc, rx_skb))
        return 0;
#endif // IRQ_PROC_TX_DATA

    skb_queue_tail(&sc->rx_skb_q, rx_skb);
    wake_up_interruptible(&sc->rx_wait_q);

    return 0;
}

void ssv6xxx_check_fw_status_process(struct work_struct *work)
{
    struct ssv_softc *sc = container_of(work, struct ssv_softc, check_fw_status_work);

    if(sc->fw_cur_status_cnt != sc->fw_pre_status_cnt)
    {
        sc->fw_pre_status_cnt = sc->fw_cur_status_cnt;
        sc->fw_cur_status_idle_time = 0;
    }
    else
    {
        if(sc->sh->cfg.fw_status_idle_cnt <= ++sc->fw_cur_status_idle_time)
        {
            printk(KERN_ERR "[%s] Not detect FW status for %d times(over %d ms), the FW will be reloaded.\n", __FUNCTION__, sc->fw_cur_status_idle_time, sc->fw_cur_status_idle_time*HOUSE_KEEPING_TIMEOUT);
            queue_work(sc->config_wq, &sc->hw_restart_work);
            sc->fw_cur_status_idle_time = 0;
        }
    }
}

void ssv6xxx_mib_edca_process(struct work_struct *work)
{
    struct ssv_softc *sc = container_of(work, struct ssv_softc, mib_edca_work);

    if (!sc->sh)
        return;

    SSV_EDCA_STAT(sc->sh);
}

void ssv6xxx_tx_poll_process(struct work_struct *work)
{
    struct ssv_softc *sc = container_of(work, struct ssv_softc, tx_poll_work);

    if (!sc->sh) 
        return;

    SSV_SEND_TX_POLL_CMD(sc->sh, SSV6XXX_TX_POLL_RESET);
}

void ssv6xxx_flowctl_process(struct work_struct *work)
{
    struct ssv_softc *sc = container_of(work, struct ssv_softc, flowctl_work);
    ieee80211_wake_queues(sc->hw);
}

#ifdef CONFIG_ENABLE_HOST_THERMAL
void ssv6xxx_thermal_monitor_process(struct work_struct *work)
{
    struct ssv_softc *sc = container_of(work, struct ssv_softc, thermal_monitor_work);
    if (!sc->sh)
        return;
    SSV_DO_TEMPERATURE_COMPENSATION(sc->sh);
}
#endif

struct ieee80211_sta *ssv6xxx_find_sta_by_rx_skb (struct ssv_softc *sc, struct sk_buff *skb)
{
    struct ieee80211_hdr     *hdr = (struct ieee80211_hdr *)(skb->data + sc->sh->rx_desc_len);
    u32    wsid;

    wsid = HAL_GET_RX_DESC_WSID(sc->sh, skb);
    if ((wsid >= 0) && (wsid < SSV_NUM_STA))
        return sc->sta_info[wsid].sta;
    else
        return ssv6xxx_find_sta_by_addr(sc, hdr->addr2);
}

struct ieee80211_sta *ssv6xxx_find_sta_by_addr (struct ssv_softc *sc, u8 addr[6])
{
    struct ieee80211_sta *sta;
    int i;

    for (i = 0; i < SSV6200_MAX_VIF; i++)
    {
        if (sc->vif_info[i].vif == NULL)
            continue;

        sta = ieee80211_find_sta(sc->vif_info[i].vif, addr);
        if (sta != NULL)
            return sta;
    }
    return NULL;
} // end of - ssv6xxx_find_sta -


void ssv6xxx_foreach_sta (struct ssv_softc *sc, void (*sta_func)(struct ssv_softc *, struct ssv_sta_info *, void *), void *param)
{
    int i;

    BUG_ON(sta_func == NULL);

#if 0
    for (i = 0; i < SSV6200_MAX_VIF; i++)
    {
        struct ssv_vif_priv_data *vif_priv;
        int j;

        if (sc->vif_info[i].vif == NULL)
            continue;

        vif_priv = (struct ssv_vif_priv_data *)sc->vif[i]->drv_priv;
        for (j = 0; j < SSV_NUM_STA; j++)
        {
            if ((vif_priv->sta_info[j].s_flags & STA_FLAG_VALID) == 0)
                continue;
            (*sta_func)(sc, &vif_priv->sta_info[j], param);
        }
    }
#else
    down_read(&sc->sta_info_sem);/* START protect sta_info */
    for (i = 0; i < SSV_NUM_STA; i++)
    {
        if ((sc->sta_info[i].s_flags & STA_FLAG_VALID) == 0)
            continue;

        (*sta_func)(sc, &sc->sta_info[i], param);
    }
    up_read(&sc->sta_info_sem);/* END protect sta_info */
#endif
}


void ssv6xxx_foreach_vif_sta (struct ssv_softc *sc,
        struct ssv_vif_info *vif_info,
        void (*sta_func)(struct ssv_softc *,
            struct ssv_vif_info *,
            struct ssv_sta_info *,
            void *),
        void *param)
{
    struct ssv_vif_priv_data *vif_priv;
    struct ssv_sta_priv_data *sta_priv_iter;

    BUG_ON(vif_info == NULL);
    BUG_ON((size_t)vif_info < 0x30000);

    vif_priv = (struct ssv_vif_priv_data *)vif_info->vif->drv_priv;
    BUG_ON((size_t)vif_info->vif < 0x30000);
    BUG_ON((size_t)vif_priv < 0x30000);

    //down_read(&sc->sta_info_sem);/* Don't protect sta_info, already protected by ssv6200_set_key(). */
    list_for_each_entry(sta_priv_iter, &vif_priv->sta_list, list)
    {
        BUG_ON(sta_priv_iter == NULL);
        BUG_ON((size_t)sta_priv_iter < 0x30000);

        if ((sta_priv_iter->sta_idx == -1) ||
            ((sc->sta_info[sta_priv_iter->sta_idx].s_flags & STA_FLAG_VALID) == 0))
            continue;
        (*sta_func)(sc, vif_info, &sc->sta_info[sta_priv_iter->sta_idx], param);
    }
    //up_read(&sc->sta_info_sem);/* Don't protect sta_info, already protected by ssv6200_set_key(). */
}

#ifdef CONFIG_SSV6XXX_DEBUGFS
ssize_t ssv6xxx_tx_queue_status_dump (struct ssv_softc *sc, char *status_buf,
        ssize_t length)
{
    ssize_t buf_size = length;
    ssize_t prt_size;

    prt_size = snprintf(status_buf, buf_size, "\nSMAC driver queue status:.\n");
    status_buf += prt_size;
    buf_size -= prt_size;

    return (length - buf_size);
} // end of - tx_queue_status_dump -

#endif // CONFIG_SSV6XXX_DEBUGFS
