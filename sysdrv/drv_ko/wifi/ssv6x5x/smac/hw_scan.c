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

#include <linux/module.h>
#include <linux/version.h>
#include <linux/nl80211.h>
#include <net/mac80211.h>
#include <net/cfg80211.h>
#include <linux_80211.h>
#include <ssv6200.h>
#include <hal.h>
#include "dev.h"
#include "ssv_skb.h"
#include "hw_scan.h"

MODULE_AUTHOR("iComm-semi, Ltd");
MODULE_DESCRIPTION("Support for SSV6xxx wireless LAN cards.");
MODULE_SUPPORTED_DEVICE("SSV6xxx 802.11n WLAN cards");
MODULE_LICENSE("Dual BSD/GPL");

#define SSV_SCAN_MAX_TIME (12000)
#define SSV_SCAN_PASSIVE_CHANNEL_TIME (110)
#define SSV_SCAN_NON_ASSOCIATION_TIME (100)

void ssv6xxx_scan_complete(struct ssv_softc *sc, bool abort)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,8,0)
    struct cfg80211_scan_info info = {
        .aborted = abort,
    };
#endif

   // mutex_lock(&sc->mutex);
    sc->hw_scan_start = false;
    sc->bScanning = false;
    sc->hw_scan_cancel = false;
    
    /* set home channel & resume hci txq */
    HAL_SET_CHANNEL(sc, sc->hw_scan_restore_channel, sc->hw_scan_restore_chan_type, false);
    HCI_TX_RESUME_BY_STA(sc->sh, sc->scan_wsid);
   
    /* cci control & beacon filter */
    ssv6xxx_scan_opertaion(sc, false);
#ifdef CONFIG_STA_BCN_FILTER
    if (sc->sta_bcn_filter) {
        //enable filter for beacon and probe request
        HAL_SET_MRX_FILTER(sc->sh, 3, true, BIT(4)|BIT(5));
    }
#endif
    
    sc->hw_scan_done = true;
    
    if (abort)
        printk("HW scan aborted\n");
    else
        printk("HW scan complete\n");

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,8,0)
    ieee80211_scan_completed(sc->hw, &info);
#else
    ieee80211_scan_completed(sc->hw, abort);
#endif
    
    //mutex_unlock(&sc->mutex);
}

static int ssv6xxx_parse_duplicate_ie_elems(const u8 *start, int len)
{
    size_t left = len;
    const u8 *pos = start;
    u8 first_id = *pos;
    int ie_offset = 0;

    while (left >= 2) {
        u8 id, elen;

        id = *pos++;
        elen = *pos++;
        left -= 2;
        
        if (elen > left) {
            return -1;
        }

        if ((id == first_id) && (left < (len - 2))) {
            break;
        }

        left -= elen;
        pos += elen;
        ie_offset += (2+elen);
    }

    return ie_offset;
}

static void ssv6xxx_build_hw_scan_param(struct ssv_softc *sc, u8 *src_addr, int vif_idx, const u8 *bssid)
{
	struct cfg80211_scan_request *req = sc->scan_req;
    struct ssv_scan_param *scan_param = &sc->scan_param;
    int i = 0;
	
    memset(scan_param, 0x0, sizeof(struct ssv_scan_param));
    memcpy(scan_param->src_addr, src_addr, ETH_ALEN);
    memcpy(scan_param->bssid, bssid, ETH_ALEN);
    if (req->n_ssids == 0) { // passive scan
        sc->scan_is_passive = true; 
        scan_param->ssid_len = 0;
    } else {
        sc->scan_is_passive = false; 
        for(i=0;i<req->n_ssids;i++)
        {    
            if (req->ssids[i].ssid_len==0) 
            {
                continue;
            }
            scan_param->ssid_len = req->ssids[i].ssid_len;
            memcpy(scan_param->ssid, req->ssids[i].ssid, req->ssids[i].ssid_len);
            break;
        }
    }
#if LINUX_VERSION_CODE > KERNEL_VERSION(3,0,8)
    scan_param->no_cck = req->no_cck;
#endif
    if (req->ie_len > 512) {
        printk("Incorrect scan ie len = %d\n", (int)req->ie_len);
        WARN_ON(1);
    }
    scan_param->vif_idx = vif_idx;
    scan_param->wsid = sc->scan_wsid;
   
    if (sc->sh->cfg.hw_caps & SSV6200_HW_CAP_5GHZ) {
        int ie_offset = ssv6xxx_parse_duplicate_ie_elems(req->ie, req->ie_len);
        
        if (ie_offset > 0) {
            sc->scan_ie_len_band2g = ie_offset;
            memcpy(sc->scan_ie_band2g, req->ie, sc->scan_ie_len_band2g);
            sc->scan_ie_len_band5g = req->ie_len - ie_offset;
            memcpy(sc->scan_ie_band5g, (u8 *)req->ie + ie_offset, sc->scan_ie_len_band5g);
        } else {
            printk("ie_offset %d, use original ie\n", ie_offset);
            sc->scan_ie_len_band2g = req->ie_len;
            memcpy(sc->scan_ie_band2g, req->ie, req->ie_len);
            sc->scan_ie_len_band5g = req->ie_len;
            memcpy(sc->scan_ie_band5g, req->ie, req->ie_len);
        }

    } else {
        sc->scan_ie_len_band2g = req->ie_len;
        memcpy(sc->scan_ie_band2g, req->ie, req->ie_len);
    }
}

static void ssv6xxx_scan_state_decision(struct ssv_softc *sc, unsigned long *next_delay)
{
    bool associated = false;
    u8 next_scan_state;
  
    if (sc->hw_scan_cancel || time_after(jiffies, (sc->scan_time + sc->scan_wait_period))) {
        sc->next_hw_scan_state = SSV_SCAN_ABORT; 
        *next_delay = 0;
        return;
    }
        
    associated = sc->scan_vif->bss_conf.assoc;
    if (associated)
        next_scan_state = SSV_SCAN_SUSPEND;
    else
        next_scan_state = SSV_SCAN_RESUME;

    sc->next_hw_scan_state = next_scan_state; 
    *next_delay = 0;
}

static void ssv6xxx_scan_state_suspend(struct ssv_softc *sc, unsigned long *next_delay)
{
    u8 next_scan_state;
     
    if (sc->hw_scan_cancel || time_after(jiffies, (sc->scan_time + sc->scan_wait_period))) {
        sc->next_hw_scan_state = SSV_SCAN_ABORT; 
        *next_delay = 0;
        return;
    }
    
    /* set home channel & resume hci txq */
    HAL_SET_CHANNEL(sc, sc->hw_scan_restore_channel, sc->hw_scan_restore_chan_type, false);
    HCI_TX_RESUME_BY_STA(sc->sh, sc->scan_wsid);
    
    next_scan_state = SSV_SCAN_RESUME;
    sc->next_hw_scan_state = next_scan_state; 
    *next_delay = msecs_to_jiffies(sc->sh->cfg.scan_hc_period);
}

static void ssv6xxx_scan_state_resume(struct ssv_softc *sc, unsigned long *next_delay)
{
    u8 next_scan_state;
	struct cfg80211_scan_request *req = sc->scan_req;
    struct ieee80211_channel *chan;

    if (sc->hw_scan_cancel || time_after(jiffies, (sc->scan_time + sc->scan_wait_period))) {
        sc->next_hw_scan_state = SSV_SCAN_ABORT; 
        *next_delay = 0;
        return;
    }

#if 0
    if (!ssv6200_not_dual_intf_on_line(sc)) {
        printk("dual interface, don't set offchan\n");
        sc->scan_channel_idx++; 
        sc->next_hw_scan_state = SSV_SCAN_DECISION; 
        *next_delay = 0;
        return;
    }
#endif

    /* pause hci txq & set off channel */ 
    HCI_TX_PAUSE_BY_STA(sc->sh, sc->scan_wsid);
    chan = req->channels[sc->scan_channel_idx];
    HAL_SET_CHANNEL(sc, chan, NL80211_CHAN_NO_HT, true);
    
    sc->scan_channel_idx++; 
    next_scan_state = SSV_SCAN_DECISION;
    sc->next_hw_scan_state = next_scan_state;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,14,0)
    if (chan->flags & (IEEE80211_CHAN_NO_IR | IEEE80211_CHAN_RADAR)) {
#else
    if (chan->flags & (IEEE80211_CHAN_RADAR)) {
#endif
        *next_delay = msecs_to_jiffies(SSV_SCAN_PASSIVE_CHANNEL_TIME);
    } else {
        if (sc->scan_vif->bss_conf.assoc) {
            *next_delay = msecs_to_jiffies(sc->sh->cfg.scan_oc_period);
        } else {
            *next_delay = msecs_to_jiffies(SSV_SCAN_NON_ASSOCIATION_TIME);
        }
    }
}

void ssv6xxx_scan_ignore_process(struct work_struct *work)
{
    struct ssv_softc *sc = container_of(work, struct ssv_softc, scan_ignore_work.work);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,8,0)
    struct cfg80211_scan_info info = {
        .aborted = false,
    };
#endif
    
    printk("scan_ignore_process done\n");
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,8,0)
    ieee80211_scan_completed(sc->hw, &info);
#else
    ieee80211_scan_completed(sc->hw, false);
#endif
}

void ssv6xxx_scan_process(struct work_struct *work)
{
    struct ssv_softc *sc = container_of(work, struct ssv_softc, scan_work.work);
	struct cfg80211_scan_request *req = sc->scan_req;
    unsigned long next_delay = 0; 
    bool aborted = false;
    
    do {
        switch (sc->next_hw_scan_state) {
            case SSV_SCAN_DECISION:
                if (sc->scan_channel_idx >= req->n_channels) {
                    aborted = false;
                    goto out_complete;
                }
                ssv6xxx_scan_state_decision(sc, &next_delay);
                break;
            case SSV_SCAN_SUSPEND: // home channel
                ssv6xxx_scan_state_suspend(sc, &next_delay);
                break;
            case SSV_SCAN_RESUME: // off channel
                ssv6xxx_scan_state_resume(sc, &next_delay);
                break;
            case SSV_SCAN_ABORT:
                goto out_complete;
        }
    } while (next_delay == 0);

    queue_delayed_work(sc->scan_wq, &sc->scan_work, next_delay);
    goto out;

out_complete:
    ssv6xxx_scan_complete(sc, aborted);
out:
    return; 
}
#if 0
static bool ssv6xxx_scan_check_p2p_interface(struct ssv_softc *sc)
{
    struct ieee80211_vif *vif;
    int i = 0;

    for (i = 0; i < SSV6200_MAX_VIF; i++) {
        if (sc->vif_info[i].vif != NULL) {
            vif = sc->vif_info[i].vif;
            if (vif->p2p)
                return true;
        }
    }

    return false;
}
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 17, 0))    
int ssv6200_hw_scan(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
                struct ieee80211_scan_request *hw_req)
#else
int ssv6200_hw_scan(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
                struct cfg80211_scan_request *hw_req)
#endif
{
    struct ssv_softc *sc = hw->priv;
    struct ssv_vif_priv_data *vif_priv = (struct ssv_vif_priv_data *)vif->drv_priv;
	bool assoc = vif->bss_conf.assoc;
    int i = 0;

    mutex_lock(&sc->mutex);
    printk("scan on vif: %pM\n", vif->addr);

    if (sc->bScanning == true) {
        printk("scan is running, ignore the scan request\n");
        goto out;
    }

	if ((sc->sc_flags & SC_OP_BLOCK_CNTL) && (sc->sc_flags & SC_OP_CHAN_FIXED)) {
        printk("rf tool, ignore the scan request\n");
        queue_delayed_work(sc->scan_wq, &sc->scan_ignore_work, 0);
        goto out;
	}

#if 0
    // if p2p interface up, ignore non-p2p scan request
    if (true == ssv6xxx_scan_check_p2p_interface(sc)) {
        if (false == vif->p2p) {
            printk("p2p interface up, non-p2p scan request\n");
            queue_delayed_work(sc->scan_wq, &sc->scan_ignore_work, 0);
            goto out;
        }
    }
#endif
    
    /* cci control & beacon filter */
    ssv6xxx_scan_opertaion(sc, true);
#ifdef CONFIG_STA_BCN_FILTER
    if (sc->sta_bcn_filter) {
        //disable filter for beacon and probe request
        HAL_SET_MRX_FILTER(sc->sh, 3, false, BIT(4)|BIT(5));
    }
#endif
    /* build scan param */
    sc->bScanning = true;
    sc->hw_scan_start = true;
    sc->hw_scan_cancel = false;
    sc->hw_scan_done = false;
    sc->hw_scan_restore_channel = sc->cur_channel;
    sc->hw_scan_restore_chan_type = sc->hw_chan_type;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 17, 0))    
    sc->scan_req = &hw_req->req;
#else
    sc->scan_req = hw_req;
#endif
    sc->scan_vif = vif;
    sc->next_hw_scan_state = SSV_SCAN_DECISION;
    sc->scan_channel_idx = 0;
    sc->scan_wsid = 0xf;
    if (assoc == true) {
        // find station wsid
        for (i = 0; i < SSV_NUM_STA; i++) {
            if (sc->sta_info[i].s_flags & STA_FLAG_VALID) {
                if (vif->bss_conf.aid == sc->sta_info[i].aid) {
                    sc->scan_wsid = i;
                    break;
                }
            }
        }
    }
    sc->scan_wait_period = msecs_to_jiffies(SSV_SCAN_MAX_TIME);
    sc->scan_time = jiffies;
    
    ssv6xxx_build_hw_scan_param(sc, vif->addr, vif_priv->vif_idx, vif->bss_conf.bssid);
    queue_delayed_work(sc->scan_wq, &sc->scan_work, 0);

out:
    mutex_unlock(&sc->mutex);
    return 0;
}

void ssv6xxx_cancel_hw_scan(struct ssv_softc *sc)
{
    cancel_delayed_work_sync(&sc->scan_work);
    sc->hw_scan_cancel = true;
    ssv6xxx_scan_complete(sc, true);
}

void ssv6200_cancel_hw_scan(struct ieee80211_hw *hw, struct ieee80211_vif *vif)
{
    struct ssv_softc *sc = hw->priv;
    
    printk("cancel HW scan on vif: %pM\n", vif->addr);
    mutex_lock(&sc->mutex);
    ssv6xxx_cancel_hw_scan(sc);
    mutex_unlock(&sc->mutex);
}
