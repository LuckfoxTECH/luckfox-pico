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
#include <linux/inetdevice.h>
#include <linux/of.h>
#include <net/cfg80211.h>
#include <linux/etherdevice.h>

#include "dev.h"
#include <hwif/hwif.h>
#include <hci/ssv_hci.h>
#include <fmac/fmac.h>
#include <fmac/fmac_tx.h>
#include <fmac/fmac_rx.h>
#include <fmac/host_msg.h>
#include "ssv_cfg.h"
#include "rftool/ssv_phy_rf.h"
#include "ssv_custom_func.h"
#include "rftool/ssv_efuse.h"
#include "ipc_msg.h"
#include "hci/drv_hci_ops.h"
#include "ssv_firmware_version.h"

#include "ssv_debug.h"

#include <fmac/fmac_msg_rx.h>
#include <fmac/fmac_msg_tx.h>
#include <fmac/fmac_utils.h>

MODULE_AUTHOR("iComm-semi, Ltd");
MODULE_DESCRIPTION("Support for SSV6xxx wireless LAN cards.");
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 12, 0))
MODULE_SUPPORTED_DEVICE("SSV6xxx 802.11n WLAN cards");
#endif
MODULE_LICENSE("Dual BSD/GPL");

extern struct ssv6xxx_cfg ssv_cfg;
struct ssv_hw *g_ssv_hw = NULL;
EXPORT_SYMBOL(g_ssv_hw);

extern int ssv_ctl_init(void);
#if defined(CONFIG_WPA_SUPPLICANT_CTL)
extern int ssv_wpas_ctl_init(void);
#endif
extern int ssv_init_cli(const char *dev_name, struct ssv_cmd_data *cmd_data);
extern int ssv6xxx_fmac_init(void **plat_sc, struct ssv6xxx_hci_ops *hci_ops, void *hci_priv, u8 *maddr, struct device *dev);
extern int ssv_nimble_init(void **plat_sbc, struct ssv6xxx_hci_ops *hci_ops, void *hci_priv, struct device *dev);
extern int ssv_rftool_init(void **plat_srfc, struct ssv6xxx_hci_ops *hci_ops, void *hci_priv, struct device *dev);
#ifdef CONFIG_BLE
extern int ssv_ble_hci_init(void **plat_sbsc, struct ssv6xxx_hci_ops *hci_ops, void *hci_priv, u8 *maddr, struct device *dev);
#endif
extern void ssv_ctl_deinit(void);
#if defined(CONFIG_WPA_SUPPLICANT_CTL)
extern void ssv_wpas_ctl_deinit(void);
#endif
extern void ssv_deinit_cli(const char *dev_name, struct ssv_cmd_data *cmd_data);
extern void ssv6xxx_fmac_deinit(void *plat_sc);
extern void ssv_nimble_deinit(void *plat_sbc);
extern void ssv_rftool_deinit(void *plat_srfc);
#ifdef CONFIG_BLE
extern void ssv_ble_hci_deinit(void *plat_sbsc);
#endif
#if (CONFIG_PRE_ALLOC_SKB == 1)
extern int ssv_pre_allocate_init(void);
extern void ssv_pre_allocate_exit(void);
#elif (CONFIG_PRE_ALLOC_SKB == 2)
extern u32 ssv_pre_allocate_rx_skb_get_max_running(void);
extern u32 ssv_pre_allocate_rx_skb_get_current_q_len(void);
extern int ssv_pre_allocate_rx_skb_free_all(void);
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
static inline void house_keeping_timer_hdl(struct timer_list *in_timer)
{
	struct ssv_timer_list *ptimer = from_timer(ptimer, in_timer, timer);
	ptimer->function((unsigned long)ptimer->arg);
}
#endif

void ssv6xxx_fw_reset_softap_deauth_to_host(struct ssv_softc *sc)
{
    int i = 0;
    struct ssv_vif *vif = NULL;

    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);
    for(i = 0; i < (NX_VIRT_DEV_MAX); i++) {
        if (sc->vif_table[i] == NULL)
            continue;
        vif = sc->vif_table[i];
        if (NL80211_IFTYPE_AP == SSV_VIF_TYPE(vif)) {
            if (sc->sta_table[vif->ap.bcmc_index].valid) {
                if (vif->ap.bcmc_index == sc->sta_table[vif->ap.bcmc_index].sta_idx) {
                    if (i != sc->if_mode[i][1]) {
                        SSV_LOG_DBG("!!!!!!!!! err1 !!!!!!!!\n");
                    }
                    //SSV_LOG_DBG("vif->ap.bcmc_index = %d,vif->drv_vif_index = %d \n",vif->ap.bcmc_index, vif->drv_vif_index);
                    ssv6xxx_fw_reset_send_deauth_check(sc, vif);
                }
            }
        }
    }
}

void ssv6xxx_fw_reset_sta_dis_to_host(struct ssv_softc *sc)
{
    int i = 0;
    struct ssv_vif *vif = NULL;
    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);
    for(i = 0; i < (NX_VIRT_DEV_MAX); i++) {
        if (sc->vif_table[i] == NULL)
            continue;
        vif = sc->vif_table[i];
        if (NL80211_IFTYPE_STATION != SSV_VIF_TYPE(vif))
            continue;
        if (vif->sta.ap->valid == true) {            
            ssv_rx_sm_disconnect_ind_for_fw_reset(sc, i);
        }
    }
}

static void ssv6xxx_fw_reset_scan_done(struct ssv_softc *sc)
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
        SSV_LOG_DBG("ssv6xxx_fw_reset_scan_done: scan done\n");
    }

    sc->scan_request = NULL;
}

void _ssv_fwreset_msg_to_hci(struct ssv_softc *sc, u8 *msg_buffer, u32 msg_len, u32 msg_type);

void ssv6xxx_fw_reset_process(struct work_struct *work)
{
    int i = 0;
    struct ssv_softc *sc = container_of(work, struct ssv_softc, fw_reset_work);
    struct fw_reset_cmd *cur, *tmp;

    if(sc->fw_reset_run == true) {
        SSV_LOG_DBG("reset process already run\n");
        return;
    }

    sc->fw_reset_run = true;

    SSV_LOG_DBG("**************************\n");
    SSV_LOG_DBG("*** Software MAC reset ***\n");
    SSV_LOG_DBG("**************************\n");
    SSV_LOG_DBG("**************************\n");
    SSV_LOG_DBG("*** wait MM_RESET_CFM ***\n");
    SSV_LOG_DBG("**************************\n");
    for(i = 0; i < (SSV_SW_TXQ_NUM -1); i++) {
        ssv_drv_hci_tx_pause_by_txqid(sc->hci_priv, sc->hci_ops, i);
        ssv_drv_hci_tx_inactive_by_txqid(sc->hci_priv, sc->hci_ops, i);
    }

    ssv_drv_hci_fw_reset(sc->hci_priv, sc->hci_ops);
    //mdelay(500);
#if 1
    list_for_each_entry_safe(cur, tmp, &sc->reset_cmd.list, list) {
        _ssv_fwreset_msg_to_hci(sc, cur->data, cur->msg_len, cur->msg_type);
    }
#endif
    SSV_LOG_DBG("**********************************\n");
    SSV_LOG_DBG("*** Software MAC reset ready ***\n");
    SSV_LOG_DBG("**********************************\n");
    for(i = 0; i < (SSV_SW_TXQ_NUM -1); i++) {
        ssv_drv_hci_tx_active_by_txqid(sc->hci_priv, sc->hci_ops, i);
        ssv_drv_hci_tx_resume_by_txqid(sc->hci_priv, sc->hci_ops, i);
    }
  
   ssv6xxx_fw_reset_sta_dis_to_host(sc);
   ssv6xxx_fw_reset_scan_done(sc);
   ssv6xxx_fw_reset_softap_deauth_to_host(sc);
   sc->fw_reset_run = false; 
}

void ssv6xxx_house_keeping(unsigned long argv)
{
#define TIMEOUT_REORDER_WORK    50 
#define TIMEOUT_PROBE_STA_WORK    1000 
#define TIMEOUT_SEND_KEEP_ALIVE_WORK     30000 
#define TIMEOUT_CHECK_FW_STATUS   100

    struct ssv_hw *ssv_hw = (struct ssv_hw *)argv;
    unsigned long tick = jiffies;
    u32 timestamp = 0;

    if (time_after(tick, (ssv_hw->reorder_work_timestamp + msecs_to_jiffies(TIMEOUT_REORDER_WORK)))) {
        ssv_hw->reorder_work_timestamp = tick;
        ssv_push_private_msg_to_host(ssv_hw->sc, (u32)E_HOST_PRIV_MSG_TYPE_RX_REORD_TIMEOUT, (u8 *)NULL, (u32)0);
    }

    if (time_after(tick, (ssv_hw->probe_sta_work_timestamp + msecs_to_jiffies(TIMEOUT_PROBE_STA_WORK)))) {
        ssv_hw->probe_sta_work_timestamp = tick;
        ssv_push_private_msg_to_host(ssv_hw->sc, (u32)E_HOST_PRIV_MSG_TYPE_PROBE_CLIENT, (u8 *)NULL, (u32)0);
    }
    
#ifdef SEND_KEEP_ALIVE
    if (time_after(tick, (ssv_hw->send_keep_alive_timestamp + msecs_to_jiffies(TIMEOUT_SEND_KEEP_ALIVE_WORK)))) {
        ssv_hw->send_keep_alive_timestamp = tick;
        ssv_push_private_msg_to_host(ssv_hw->sc, (u32)E_HOST_PRIV_MSG_TYPE_SEND_KEEP_ALIVE, (u8 *)NULL, (u32)0);
    }
#endif

    if(ssv_cfg.fw_reset) {
        if (time_after(tick, (ssv_hw->check_fw_status_timestamp + msecs_to_jiffies(TIMEOUT_CHECK_FW_STATUS)))) {
            ssv_hw->check_fw_status_timestamp = tick;
            ssv_push_private_msg_to_host(ssv_hw->sc, (u32)E_HOST_PRIV_MSG_TYPE_CHECK_FW_STATUS, (u8 *)NULL, (u32)0);
        }
    }
 
    if (ssv_hw->hci_ops->hci_get_tx_timestamp)
        ssv_hw->hci_ops->hci_get_tx_timestamp(ssv_hw->hci_priv, &timestamp);
    if (time_after(tick, timestamp + msecs_to_jiffies(ssv_cfg.hci_idle_period))) {
        if (ssv_hw->hci_ops->hci_set_tx_timestamp)
            ssv_hw->hci_ops->hci_set_tx_timestamp(ssv_hw->hci_priv, tick);
        if (ssv_hw->hci_ops->hci_trigger_tx)
            ssv_hw->hci_ops->hci_trigger_tx(ssv_hw->hci_priv);
    }

    //flow control
    ssv_tx_flow_control(ssv_hw->sc, false);

    // be the last
    mod_timer(&ssv_hw->house_keeping.time.timer, jiffies + msecs_to_jiffies(HOUSE_KEEPING_TIMEOUT));
}

int ssv6xxx_housekeeping_init(struct ssv_hw *ssv_hw)
{
    struct ssv_softc *sc = ssv_hw->sc;
    
    ssv_hw->reorder_work_timestamp = jiffies;
    ssv_hw->check_fw_status_timestamp = jiffies;
    ssv_hw->reorder_work_timestamp = jiffies;
    
    ssv_hw->house_keeping.time.function = ssv6xxx_house_keeping;
    ssv_hw->house_keeping.time.arg = (void *)ssv_hw;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
    timer_setup(&ssv_hw->house_keeping.time.timer, house_keeping_timer_hdl, 0);
    ssv_hw->house_keeping.time.timer.expires = jiffies + msecs_to_jiffies(HOUSE_KEEPING_TIMEOUT);
#else
    init_timer(&ssv_hw->house_keeping.time.timer);
    ssv_hw->house_keeping.time.timer.expires = jiffies + msecs_to_jiffies(HOUSE_KEEPING_TIMEOUT);
    ssv_hw->house_keeping.time.timer.function = ssv6xxx_house_keeping;
    ssv_hw->house_keeping.time.timer.data = (unsigned long)ssv_hw;
#endif
    sc->fw_reset_wq= create_singlethread_workqueue("fw_reset_wq");
    INIT_WORK(&sc->fw_reset_work, ssv6xxx_fw_reset_process);
    add_timer(&ssv_hw->house_keeping.time.timer);
    return 0;
}

int ssv6xxx_housekeeping_deinit(struct ssv_hw *ssv_hw)
{
    struct ssv_softc *sc = ssv_hw->sc;
    // remove house keeping
    sc->fw_reset_run= false;
    del_timer_sync(&ssv_hw->house_keeping.time.timer);
    destroy_workqueue(sc->fw_reset_wq);
    return 0;
}
extern const char *sw_driver_version;
extern const char *driver_build_date;

void ssv6xxx_check_fw_version(struct ssv_softc *sc)
{
    u32 regval = 0;
    if(sc->hci_ops->hci_read_word)
        sc->hci_ops->hci_read_word(sc->hci_priv, REG_FW_VERSION, &regval);
    
    sc->fw_version=regval;
    SSV_LOG_DBG("SSV firmware version %d\n", regval);
    if (regval != ssv_firmware_version)
    {
        if (ssv_cfg.ignore_firmware_version == 0) {
            SSV_LOG_ERR("Firmware version mapping not match[%d(0x%08x)]!!\n",regval, regval);
            SSV_LOG_ERR("It's should be [%d(0x%08x)]!!\n",ssv_firmware_version, ssv_firmware_version);
            BUG_ON(1);
        }
        else
            SSV_LOG_DBG("Force ignore_firmware_version\n");
    }
}

int ssv6xxx_platform_init(struct device *dev, struct ssv6xxx_hwif_ops *hwif_ops, void **plat_hw)
{
    struct ssv_hw *ssv_hw = NULL;
#ifdef CONFIG_FMAC
    struct ssv_softc *sc = NULL;
#endif
#ifdef CONFIG_HWIF_AND_HCI
    void *hci_priv = NULL;
#endif

    struct ssv_rftool_softc *srfc = NULL;

#ifdef CONFIG_NIMBLE
    struct ssv_nimble_softc *snc = NULL;
#endif
    #ifdef CONFIG_BLE
    struct ssv_ble_softc *sbsc = NULL;
#endif
    int ret = 0;
    int error = 0;


    ssv_hw = kzalloc(sizeof(struct ssv_hw), GFP_KERNEL);
    if (NULL == ssv_hw) {
        SSV_LOG_DBG(KERN_ERR "Fail to alloc ssv_hw\n");
        ret = -ENOMEM;
        goto out;
    }
    g_ssv_hw = ssv_hw;

    ssv_hw->dev = dev;
    ssv_hw->hwif_ops = hwif_ops;
    
    efuse_read_all_map(ssv_hw);
    ssv_custom_modify_macaddr(ssv_hw);

#if (CONFIG_PRE_ALLOC_SKB == 1)
    ret = ssv_pre_allocate_init();
    if (ret != 0) {
        SSV_LOG_DBG(KERN_ERR "Fail to init pre-alloc!\n");
        goto err_pre_alloc;
    }
#endif

#ifdef CONFIG_HWIF_AND_HCI
    ret = ssv6xxx_hci_init((void *)&hci_priv, hwif_ops, dev);
    if (ret) { 
        SSV_LOG_DBG(KERN_ERR "Fail to alloc hci\n");
        ret = -ENOMEM;
        goto err_hci;
    }
#endif

#ifdef CONFIG_FMAC
    ret = ssv6xxx_fmac_init((void *)&sc, &ssv_hci_ops, hci_priv, (u8 *)ssv_hw->maddr, dev); 
    if (ret) {
        SSV_LOG_DBG("Fail to fmac init\n");
        ret = -ENODEV;
        goto err_fmac;
    }
#endif
    SSV_LOG_DBG("SSV host driver version %s，Build Data %s\n", sw_driver_version,driver_build_date);
    ssv6xxx_check_fw_version(sc);

    ret = ssv_rftool_init((void *)&srfc, &ssv_hci_ops, hci_priv, dev); 
    if (ret) {
        SSV_LOG_DBG("Fail to rftool init\n");
        ret = -ENODEV;
        goto err_fmac;
    }
#ifdef CONFIG_NIMBLE
    ret = ssv_nimble_init((void *)&snc, &ssv_hci_ops, hci_priv, dev); 
    if (ret) {
        SSV_LOG_DBG("Fail to nimble init\n");
        ret = -ENODEV;
        goto err_ble;
    }
#endif
#ifdef CONFIG_BLE
    ret = ssv_ble_hci_init((void*)&sbsc, &ssv_hci_ops, hci_priv, (u8 *)&ssv_hw->maddr[1], dev);
    if (ret) {
        SSV_LOG_DBG("Fail to ble init\n");
        ret = -ENODEV;
        goto err_ble;
    }
#endif
#ifdef CONFIG_HWIF_AND_HCI
    ssv_hw->hci_priv = hci_priv;
    ssv_hw->hci_ops = &ssv_hci_ops;
#endif
#ifdef CONFIG_FMAC
    ssv_hw->sc = sc;
#endif

    ssv_hw->srfc = srfc;
    
#ifdef CONFIG_NIMBLE
    ssv_hw->snc = snc;
#endif
#ifdef CONFIG_BLE
    ssv_hw->sbsc = sbsc;
#endif
    mutex_init(&ssv_hw->mutex);
    ssv6xxx_housekeeping_init(ssv_hw);
    /* wifi_ctl init*/
    ssv_ctl_init();
#if defined(CONFIG_WPA_SUPPLICANT_CTL)
    /* wpas_ctl init*/
    ssv_wpas_ctl_init();
#endif
    /* create debug fs */
    ssv_init_cli("ssv", &ssv_hw->cmd_data);
    
    *plat_hw = ssv_hw;
    SSV_LOG_DBG("SSV6X5X of iComm-semi");

#if 1
    //do calibration first then send table to fw
    ssv_update_rf_conf_table(ssv_hw);
    ssv_custom_modify_rf_conf_table(ssv_hw);
    ssv_kdump_rf_conf_table(ssv_hw);
    sc->recovery_flag = true;
    ssv_set_pll_phy_rf(ssv_hw);
    sc->recovery_flag = false;
#endif
    if(1 == ssv_cfg.firmware_choice) //Normal mode
    {
            /*Send RF_CALIBRATION_RESULT to FW*/
        // SSV_LOG_DBG("Send RF_CALIBRATION_RESULT to FW*\n");
        error = ssv_send_priv_msg_rf_calib_result_backup(sc);
        if(error)
        {
            SSV_LOG_DBG("send rf calibration result error %d\n", error);
#ifdef CONFIG_NIMBLE
            goto err_ble;
#endif

#ifdef CONFIG_FMAC
            goto err_fmac;
#endif
        }
        #if 0 //reserved for low power
        /*Send RF_AGC_Setting and RF_PHY_SETTING to FW*/
        SSV_LOG_DBG("Send RF_PYH_TABLE to FW*\n");
        error = ssv_send_priv_msg_agc_reg_table(sc);
        if(error)
        {
            SSV_LOG_DBG("send agc error %d\n", error);
            return error;
        }

        error = ssv_send_priv_msg_rfphy_reg_talbe(sc);
        if(error)
        {
            SSV_LOG_DBG("send rfphy error %d\n", error);
            return error;
        }
        #endif
    }


    return 0;

#if defined(CONFIG_NIMBLE) || defined(CONFIG_BLE)
err_ble:
#ifdef CONFIG_FMAC
    ssv6xxx_fmac_deinit((void *)sc); 
#endif
#endif

#ifdef CONFIG_FMAC
err_fmac:
#ifdef CONFIG_HWIF_AND_HCI
    ssv6xxx_hci_deinit((void *)hci_priv); 
#endif
#endif

#ifdef CONFIG_HWIF_AND_HCI
err_hci:
#endif
#if (CONFIG_PRE_ALLOC_SKB == 1)
    ssv_pre_allocate_exit();

err_pre_alloc:
#endif
    kfree(ssv_hw);

out:
    return ret;
}
EXPORT_SYMBOL(ssv6xxx_platform_init);

void ssv6xxx_platform_deinit(void *plat_hw)
{
    struct ssv_hw *ssv_hw = (struct ssv_hw *)plat_hw;
    ssv6xxx_housekeeping_deinit(ssv_hw);
    ssv_deinit_cli("ssv", &ssv_hw->cmd_data);

    ssv_ctl_deinit();
#if defined(CONFIG_WPA_SUPPLICANT_CTL)
    ssv_wpas_ctl_deinit();
#endif
    ssv_rftool_deinit((void *)ssv_hw->srfc);

#ifdef CONFIG_NIMBLE
    ssv_nimble_deinit((void *)ssv_hw->snc);
#endif
#ifdef CONFIG_BLE
    ssv_ble_hci_deinit((void *)ssv_hw->sbsc);
#endif
#ifdef CONFIG_FMAC
    ssv6xxx_fmac_deinit((void *)ssv_hw->sc);
#endif
#ifdef CONFIG_HWIF_AND_HCI
    ssv6xxx_hci_deinit((void *)ssv_hw->hci_priv); 
#endif
#if (CONFIG_PRE_ALLOC_SKB == 1)
    ssv_pre_allocate_exit();
#elif (CONFIG_PRE_ALLOC_SKB == 2)
    ssv_pre_allocate_rx_skb_free_all();
    SSV_LOG_DBG("max_running:%u, current_qlen:%u\n",
           ssv_pre_allocate_rx_skb_get_max_running(),
           ssv_pre_allocate_rx_skb_get_current_q_len());
#endif

    kfree(ssv_hw);
    return;
}
EXPORT_SYMBOL(ssv6xxx_platform_deinit);
