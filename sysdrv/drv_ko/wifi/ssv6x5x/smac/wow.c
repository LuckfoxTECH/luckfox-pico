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
#include <linux/nl80211.h>
#include <linux/version.h>

#include <ssv6200.h>
#include <linux_80211.h>
#include <hci/hctrl.h>
#include "wow.h"
#include "dev.h"
#include "ssv_skb.h"
#include <hal.h>

MODULE_AUTHOR("iComm-semi, Ltd");
MODULE_DESCRIPTION("Support for SSV6xxx wireless LAN cards.");
MODULE_SUPPORTED_DEVICE("SSV6xxx 802.11n WLAN cards");
MODULE_LICENSE("Dual BSD/GPL");

#ifdef CONFIG_PM
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 11, 0))
static const struct wiphy_wowlan_support ssv6200_wowlan_support = {
    .flags = (WIPHY_WOWLAN_MAGIC_PKT | WIPHY_WOWLAN_ANY),
    //.flags = (WIPHY_WOWLAN_MAGIC_PKT),
    .n_patterns = SSV6XXX_WOW_MAX_NUM_PATTERN - 2,
    .pattern_min_len = 1,
    .pattern_max_len = SSV6XXX_WOW_MAX_PATTERN_SIZE,
};
#endif

static void ssv6xxx_wow_station_info(struct ssv_softc *sc, u8 *sta_mac, u8 *bssid, u8 *wsid, u8 *vif_idx)
{
    int i = 0;

    // find vif info
    for (i = 0; i < SSV6200_MAX_VIF; i++) {
        if (sc->vif_info[i].vif == NULL)
            continue;
        
        if (sc->vif_info[i].vif->type == NL80211_IFTYPE_STATION)
            break;
    }
    if (i == SSV6200_MAX_VIF)
        return;
     
    *vif_idx = sc->vif_info[i].vif_priv->vif_idx;
    memcpy(bssid, sc->bssid[*vif_idx], ETH_ALEN);
    memcpy(sta_mac, sc->sh->cfg.maddr[i], ETH_ALEN);

    // find station wsid
    for (i = 0; i < SSV_NUM_STA; i++) {
        if (sc->sta_info[i].s_flags & STA_FLAG_VALID) {
            *wsid = sc->sta_info[i].hw_wsid;
            break;
        }
    }
}

int ssv6xxx_trigger_pmu(struct ssv_softc *sc, u8 triggers, u32 host_ipv4_addr)
{
    struct sk_buff *skb;
    struct cfg_host_cmd *host_cmd;
    struct ssv6xxx_ps_params *ptr;
    int retval = 0;
    int dev_type = HCI_DEVICE_TYPE(sc->sh->hci.hci_ctrl);
    u8 sta_mac[ETH_ALEN], bssid[ETH_ALEN];
    u8 wsid = 0, vif_idx = 0;

    memset(&sta_mac, 0x0, ETH_ALEN);
    memset(&bssid, 0x0, ETH_ALEN);
    ssv6xxx_wow_station_info(sc, sta_mac, bssid, &wsid, &vif_idx);
    printk("%s() sta_mac %02X:%02X:%02X:%02X:%02X:%02X\n", __FUNCTION__, 
        sta_mac[0], sta_mac[1], sta_mac[2], sta_mac[3], sta_mac[4], sta_mac[5]);
    printk("%s() bssid %02X:%02X:%02X:%02X:%02X:%02X\n", __FUNCTION__, 
        bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);
    
    skb = ssv_skb_alloc(sc, HOST_CMD_HDR_LEN + sizeof(struct ssv6xxx_ps_params));
    if (!skb) {
	    printk(KERN_ERR "%s(): Fail to alloc cmd buffer.\n", __FUNCTION__);
	    return -1;
    }

    skb_put(skb, HOST_CMD_HDR_LEN + sizeof(struct ssv6xxx_ps_params));
    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
    host_cmd->c_type = HOST_CMD;
    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_PS;
    host_cmd->len = HOST_CMD_HDR_LEN + sizeof(struct ssv6xxx_ps_params);
    ptr = (struct ssv6xxx_ps_params *)host_cmd->un.dat8;
    memset(ptr, 0, sizeof(struct ssv6xxx_ps_params));
    ptr->if_type = ((dev_type == SSV_HWIF_INTERFACE_USB) ? SSV6XXX_IF_USB : SSV6XXX_IF_SDIO);
    ptr->aid = sc->ps_aid;
    ptr->ops = SSV6XXX_PS_DOZE;
    ptr->chan = sc->hw_chan;
    ptr->chan_type = sc->hw_chan_type;
    ptr->host_ipv4_addr = host_ipv4_addr; 
    ptr->wsid = wsid;
    ptr->vif_idx = vif_idx;
    ptr->powermode = sc->sh->cfg.powermode; 
    ptr->keepalive = sc->sh->cfg.lp_keepalive; 
    ptr->userdtim = (sc->sh->cfg.lp_userdtim > 63) ? 63 : sc->sh->cfg.lp_userdtim; 
    ptr->wakeuppin = (sc->sh->cfg.lp_wakeuppin == 0) ? 0 : 1; 
    ptr->ignoredisconnection = (sc->sh->cfg.lp_ignoredisconnection == 0) ? 0 : 1; 
    ptr->dataretx_guard = (u8)sc->sh->cfg.lp_dataretx_guard; 
    ptr->beaconloss_guard = (u8)sc->sh->cfg.lp_beaconloss_guard; 
    ptr->beaconloss_maxcnt = (u8)sc->sh->cfg.lp_beaconloss_maxcnt; 
    ptr->seqno = (u8)sc->ps_event_cnt;
    printk("%s() ps cmd: %d\n", __FUNCTION__, ptr->seqno);
    sc->ps_event_cnt++;
    if (128 == sc->ps_event_cnt)
        sc->ps_event_cnt = 0;

    if (triggers & SSV6XXX_WOW_ANY_PATTERN_EN)
        ptr->wowlan_type |= SSV6XXX_PS_WOWLAN_ANY; 
    if (triggers & SSV6XXX_WOW_MAGIC_PATTERN_EN)
        ptr->wowlan_type |= SSV6XXX_PS_WOWLAN_MAGIC_PKT;
    memcpy(ptr->sta_mac, sta_mac, ETH_ALEN);
    memcpy(ptr->bssid, bssid, ETH_ALEN);
    ptr->sleep_gpio_ctrl = sc->sh->cfg.sleep_gpio_ctrl;
    ptr->dormant_gpio_ctrl = sc->sh->cfg.dormant_gpio_ctrl;
    retval = HCI_SEND_CMD(sc->sh, skb);
	
    return retval;
}

static int ssv6xxx_power_sleep(struct ssv_softc *sc, u8 triggers, u32 host_ipv4_addr)
{
    int retval = 0;
    u32 regval = 0, regval1 = 0;

    printk("%s() start\n", __FUNCTION__);

    SSV_PS_HOLD_ON3(sc, 0);
    /*********************************************************************/
    /* 1. Clear trap reason.                                             */
    /*********************************************************************/
    SSV_SAVE_CLEAR_TRAP_REASON(sc);

    /*********************************************************************/
    /* 2. Save & Reset RX flow.                                          */
    /*********************************************************************/
    SSV_PS_SAVE_RESET_RX_FLOW(sc);
    
    /*********************************************************************/
    /* 3. Notify Wi-Fi F/W.                                              */
    /*********************************************************************/
    retval = ssv6xxx_trigger_pmu(sc, triggers, host_ipv4_addr);
    if (retval) {
        SSV_PS_RESTORE_RX_FLOW(sc);
        SSV_RESTORE_TRAP_REASON(sc);
	    printk("ssv6xxx_trigger_pmu fail!!\n");
	    goto fail;
    }

    /*********************************************************************/
    /* 3.5 check rx page is empty                                        */
    /*********************************************************************/
    while (1) {
        msleep(1);
        // check rx page
        SMAC_REG_READ(sc->sh, 0xcd01003c, &regval);
        regval = ((regval & 0x07fc0000) >> 18);
        if (regval == 0)
            printk("check rx page, go to the next step\n");
        
        // check fw notity
        SMAC_REG_READ(sc->sh, 0xc0000a10, &regval1);
        if (regval1 == 0xFFFFFFFF)
            printk("fw notify, go to the next step\n");

        if ((regval == 0) || (regval1 == 0xFFFFFFFF))  
            break;
    }
    
    /*********************************************************************/
    /* 4. Disable beacon loss detection.                                 */
    /*********************************************************************/
    SSV_BEACON_LOSS_DISABLE(sc->sh);

    /*********************************************************************/
    /* 5. Stop house keeping's timer, and cancel all workqueue's work    */
    /*********************************************************************/
    sc->mac80211_dev_started = false;
    ssv6xxx_cancel_work_sync(sc);

    /*********************************************************************/
    /* 6. Stop mac80211 TX queues.                                       */
    /*********************************************************************/
    ieee80211_stop_queues(sc->hw);
    /*********************************************************************/
    /* 7. Stop HCI layer.                                                */
    /*********************************************************************/
    HCI_SUSPEND(sc->sh);
    HCI_TXQ_FLUSH(sc->sh);

    /*********************************************************************/
    /* 8. Stop HWIF layer.                                               */
    /*********************************************************************/
    HCI_HWIF_SUSPEND(sc->sh->hci.hci_ctrl);

    /*********************************************************************/
    /* 9. FW will check the register to guarantee that the PMU is ok .   */
    /*********************************************************************/
    SSV_PS_HOLD_ON3(sc, 2);

fail:
    printk("%s() end\n", __FUNCTION__);
    return retval;
}

void ssv6xxx_power_awake(void *param)
{
    struct ssv_softc *sc = (struct ssv_softc *)param;
    unsigned long expire = msecs_to_jiffies(3000);
    int i = 0;

    printk("%s() start\n", __FUNCTION__);
    /*********************************************************************/
    /* 8. Start HWIF layer.                                              */
    /*********************************************************************/
    HCI_HWIF_RESUME(sc->sh->hci.hci_ctrl);

    /*********************************************************************/
    /* 7. Start HCI layer.                                               */
    /*********************************************************************/
    HCI_RESUME(sc->sh);

    /*********************************************************************/
    /* 6. Start mac80211 TX queues.                                      */
    /*********************************************************************/
    ieee80211_wake_queues(sc->hw);

    /*********************************************************************/
    /* 5.5. Hold ON3 for the following host cmd.                         */
    /*      (resume only)                                                */
    /*********************************************************************/
    SSV_PS_HOLD_ON3(sc, 1);

    if (!wait_for_completion_timeout(&sc->hold_on3, expire)) {
        printk("Can't receive holding ON3 ACK.\n");
    }

    /*********************************************************************/
    /* 5. Start house keeping's timer.                                   */
    /*********************************************************************/
    sc->mac80211_dev_started = true; 
    sc->house_keeping.timer.expires = jiffies + msecs_to_jiffies(HOUSE_KEEPING_TIMEOUT);
    if (!timer_pending(&sc->house_keeping.timer)) 
	add_timer(&sc->house_keeping.timer);

    /*********************************************************************/
    /* 4. Disable beacon loss detection.                                 */
    /*********************************************************************/
    SSV_BEACON_LOSS_ENABLE(sc->sh);

    /*********************************************************************/
    /* 3. Save & Reset RX flow.                                          */
    /*********************************************************************/
    SSV_PS_RESTORE_RX_FLOW(sc);

    /*********************************************************************/
    /* 2. Restore trap reason.                                           */
    /*********************************************************************/
    SSV_RESTORE_TRAP_REASON(sc);

    /*********************************************************************/
    /* 1. Notify Wi-Fi F/W.                                              */
    /*********************************************************************/
    for (i = 0; i < 3; i++) {
        SSV_PMU_AWAKE(sc);
        if (!wait_for_completion_timeout(&sc->wakeup_done, expire)) {
            printk("Send retry[%d] pmu awake\n", i);
        } else {
            break;
        }

    }
    if (3 == i)
        printk("Can't receive WAKEUP FIN ACK.\n");

    /* 
     * make sure that HCI layer is not pause/stop status 
     * it should be the last step 
     */
    HCI_RESUME_HWSWQ(sc->sh, (TXQ_EDCA_0|TXQ_EDCA_1|TXQ_EDCA_2|TXQ_EDCA_3| TXQ_MGMT));


    printk("%s() end\n", __FUNCTION__);

    return;
}

static u8 ssv6xxx_wow_map_triggers(struct ssv_softc *sc, struct cfg80211_wowlan *wowlan)
{
    u8 wow_triggers = 0;

    if (wowlan->any)
        wow_triggers |= SSV6XXX_WOW_ANY_PATTERN_EN;

    if (wowlan->magic_pkt)
        wow_triggers |= SSV6XXX_WOW_MAGIC_PATTERN_EN;

    return wow_triggers;
}

int ssv6xxx_suspend(struct ieee80211_hw *hw, struct cfg80211_wowlan *wowlan)
{
    struct ssv_softc *sc = hw->priv;
    struct ssv_hw *sh = sc->sh;
    u8 triggers = 0;
    int retval = 0;
    u32 host_ipv4_addr = 0;

    printk("%s() start\n", __FUNCTION__);
    mutex_lock(&sc->mutex);
    
    if (!(sh->cfg.hw_caps & SSV6200_HW_CAP_WOW)) {  
        printk(KERN_ERR "Cannot support wow\n");
        retval = -ENODEV;
        goto fail_wow;
    }

    if (WARN_ON(!wowlan)) {
        printk(KERN_ERR "None of the wow enabled\n");
        retval = -EINVAL;
        goto fail_wow;
    }

    if ((sc->ap_vif != NULL) || !ssv6200_not_dual_intf_on_line(sc)) {
        printk(KERN_ERR "None of the STA vifs are associated\n");
        retval = -EPERM;
        goto fail_wow;
    }
    
    if ((sc->vif_info[0].vif->type == NL80211_IFTYPE_STATION) && (sc->vif_info[0].vif->p2p == false))
        host_ipv4_addr = sc->vif_info[0].vif->bss_conf.arp_addr_list[0];
    else
        host_ipv4_addr = sc->vif_info[1].vif->bss_conf.arp_addr_list[0];
        
    if (wowlan->any && (host_ipv4_addr == 0)) {
        printk(KERN_ERR "Wow by unicast frame, but no ip address\n");
        //retval = -EPERM;
        //goto fail_wow;
    }

    triggers = ssv6xxx_wow_map_triggers(sc, wowlan);
    if (!triggers) {
        printk(KERN_ERR "Invalid wow trigger\n");
        retval = -EPERM;
        goto fail_wow;
    }
    
    if (ssv6xxx_power_sleep(sc, triggers, host_ipv4_addr)) {
        printk(KERN_ERR "Cannot send wow message to fw\n");
        retval = -EIO;
        goto fail_wow;
    }

fail_wow:
    mutex_unlock(&sc->mutex); 
    printk("%s() end\n", __FUNCTION__);
    return retval;
}

int ssv6xxx_resume(struct ieee80211_hw *hw)
{
    struct ssv_softc *sc = hw->priv;
    struct ssv_hw *sh=sc->sh;
    int retval = 0;
    
    printk("%s() start\n", __FUNCTION__);
    mutex_lock(&sc->mutex);
    
    if (!(sh->cfg.hw_caps & SSV6200_HW_CAP_WOW)) { 
        printk("Cannot support wow\n");
        retval = -ENODEV;
        goto fail_wow;
    }

    ssv6xxx_power_awake(sc);

fail_wow:
    mutex_unlock(&sc->mutex);
    return retval;
} 

void ssv6xxx_attach_wow(struct ssv_softc *sc)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 0))    
    struct ieee80211_hw *hw=sc->hw;
    struct ssv_hw *sh=sc->sh;
    
    if (sh->cfg.hw_caps & SSV6200_HW_CAP_WOW) {
    #if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 11, 0))    
        hw->wiphy->wowlan = &ssv6200_wowlan_support;
    #else
        hw->wiphy->wowlan.flags = (WIPHY_WOWLAN_MAGIC_PKT | WIPHY_WOWLAN_ANY);
        hw->wiphy->wowlan.n_patterns = SSV6XXX_WOW_MAX_NUM_PATTERN - 2;
        hw->wiphy->wowlan.pattern_min_len = 1;
        hw->wiphy->wowlan.pattern_max_len = SSV6XXX_WOW_MAX_PATTERN_SIZE;
    #endif
    }
#endif
}
#endif
