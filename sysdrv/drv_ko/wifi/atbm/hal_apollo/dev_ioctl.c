
/*
 * Mac80211 STA API for altobeam APOLLO drivers
 *
 * Copyright (c) 2016, altobeam
 *
 * Based on:
 * Copyright (c) 2010, stericsson
 * Author: Dmitry Tarnyagin <dmitry.tarnyagin@stericsson.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/sched.h>
#include <linux/firmware.h>
#include <linux/if_arp.h>
#include <linux/ipv6.h>
#include <linux/icmpv6.h>
#include <net/ndisc.h>

#include "apollo.h"
#include "sta.h"
#include "ap.h"
#include "fwio.h"
#include "bh.h"
#include "debug.h"
#include "wsm.h"
#include "hwio.h"
#ifdef CONFIG_ATBM_SUPPORT_SCHED_SCAN
#ifdef ROAM_OFFLOAD
#include <net/netlink.h>
#endif /*ROAM_OFFLOAD*/
#endif
//#ifdef CONFIG_ATBM_APOLLO_TESTMODE
#include "atbm_testmode.h"
#include <net/netlink.h>
//#endif /* CONFIG_ATBM_APOLLO_TESTMODE */

#include "net/atbm_mac80211.h"
#include "dbg_event.h"
#include "smartconfig.h"

#include "mac80211/ieee80211_i.h"
#include "mac80211/atbm_common.h"
#include "internal_cmd.h"
#ifdef CONFIG_ATBM_DEV_IOCTL
#include "dev_ioctl.h"

#define ATBM_DEV_IOCTL_DEBUG 1

#if ATBM_DEV_IOCTL_DEBUG
#define dev_printk(...) atbm_printk_always(__VA_ARGS__)
#else
#define dev_printk(...)
#endif

#define DEV_MAC2STR(a) (a)[0],(a)[1],(a)[2],(a)[3],(a)[4],(a)[5]
#define DEV_MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"



extern unsigned int atbm_wifi_status_get(void);
extern int atbm_change_iface_to_monitor(struct net_device *dev);
extern void ieee80211_connection_loss(struct ieee80211_vif *vif);
extern int ieee80211_set_channel(struct wiphy *wiphy,
                 struct net_device *netdev,
                 struct ieee80211_channel *chan,
                 enum nl80211_channel_type channel_type);
extern int atbm_find_link_id(struct atbm_vif *priv, const u8 *mac);
extern int str2mac(char *dst_mac, char *src_str);
extern void atbm_set_freq(struct ieee80211_local *local);
extern void atbm_set_tx_power(struct atbm_common *hw_priv, int txpw);
extern u8 ETF_bStartTx;
extern u8 ETF_bStartRx;
extern char ch_and_type[20];

extern u8 ucWriteEfuseFlag;
extern u32 chipversion;
extern struct rxstatus_signed gRxs_s;
extern struct etf_test_config etf_config;
extern u32 MyRand(void);
extern int wsm_start_tx_v2(struct atbm_common *hw_priv, struct ieee80211_vif *vif );
extern int wsm_start_tx(struct atbm_common *hw_priv, struct ieee80211_vif *vif);
extern int wsm_stop_tx(struct atbm_common *hw_priv);
extern u32 GetChipVersion(struct atbm_common *hw_priv);
extern void atbm_set_special_oui(struct atbm_common *hw_priv, char *pdata, int len);
extern int DCXOCodeWrite(struct atbm_common *hw_priv,u8 data);
extern u8 DCXOCodeRead(struct atbm_common *hw_priv);

static int atbm_dev_ioctl_sta_get_status(struct net_device *dev, struct altm_wext_msg *msg);
static int atbm_dev_ioctl_sta_get_rssi(struct net_device *dev, struct altm_wext_msg *msg);
static int atbm_dev_ioctl_set_txpower(struct net_device *dev, struct altm_wext_msg *msg);
static int atbm_dev_ioctl_set_txrate(struct net_device *dev, struct altm_wext_msg *msg);
static int atbm_dev_ioctl_set_ratePower(struct net_device *dev, struct altm_wext_msg *msg);
static int atbm_dev_ioctl_set_efuse_mac(struct net_device *dev, struct altm_wext_msg *msg);
static int atbm_dev_ioctl_set_efuse_dcxo(struct net_device *dev, struct altm_wext_msg *msg);
static int atbm_dev_ioctl_set_efuse_deltagain(struct net_device *dev, struct altm_wext_msg *msg);
static int atbm_dev_get_ap_info(struct net_device *dev, struct altm_wext_msg *msg);
static int atbm_dev_get_sta_info(struct net_device *dev, struct altm_wext_msg *msg);
static int atbm_dev_set_sta_scan(struct net_device *dev, struct altm_wext_msg *msg);
static int atbm_dev_set_freq(struct net_device *dev, struct altm_wext_msg *msg);
static int atbm_dev_set_special_oui(struct net_device *dev, struct altm_wext_msg *msg);
static int atbm_dev_set_sta_dis(struct net_device *dev, struct altm_wext_msg *msg);
static int atbm_dev_set_iftype(struct net_device *dev, struct altm_wext_msg *msg);
static int atbm_dev_set_adaptive(struct net_device *dev, struct altm_wext_msg *msg);
static int atbm_dev_set_txpwr_dcxo(struct net_device *dev, struct altm_wext_msg *msg);
static int atbm_dev_get_work_channel(struct net_device *dev, struct altm_wext_msg *msg);
static int atbm_dev_set_best_channel_scan(struct net_device *dev, struct altm_wext_msg *msg);
int atbm_dev_get_ap_list(struct net_device *dev, struct altm_wext_msg *msg);
int atbm_dev_get_tp_rate(struct net_device *dev, struct altm_wext_msg *msg);
extern void etf_param_init(struct atbm_common *hw_priv);
int atbm_dev_etf_test(struct net_device *dev, struct altm_wext_msg *msg);
int atbm_dev_etf_get_result(struct net_device *dev, struct altm_wext_msg *msg);
static int atbm_dev_stop_tx(struct net_device *dev, struct altm_wext_msg *msg);

static int atbm_dev_start_tx(struct net_device *dev, struct altm_wext_msg *msg);
static int atbm_dev_stop_rx(struct net_device *dev, struct altm_wext_msg *msg);

static int atbm_dev_start_rx(struct net_device *dev, struct altm_wext_msg *msg);

#ifdef CONFIG_IEEE80211_SPECIAL_FILTER

static int atbm_dev_set_special_filter(struct net_device *dev, struct altm_wext_msg *msg);
#endif
static int atbm_dev_set_country_code(struct net_device *dev, struct altm_wext_msg *msg);
static int atbm_dev_get_driver_version(struct net_device *dev, struct altm_wext_msg *msg);
static int atbm_dev_get_efuse(struct net_device *dev, struct altm_wext_msg *msg);
static int  atbm_dev_set_cail_auto_ppm(struct net_device *dev, struct altm_wext_msg *msg);

static int atbm_dev_get_vendor_special_ie(struct net_device *dev, struct altm_wext_msg *msg);

#ifdef CONFIG_ATBM_STA_LISTEN
extern int ieee80211_set_sta_channel(struct ieee80211_sub_if_data *sdata,int channel);
static int atbm_dev_set_sta_listen_channel(struct net_device *dev, struct altm_wext_msg *msg);
#endif
extern 	u32 GetChipCrystalType(struct atbm_common *hw_priv);
static int atbm_dev_get_cail_ppm_results(struct net_device *dev, struct altm_wext_msg *msg);
static int atbm_dev_set_efuse_dcxo(struct net_device *dev, struct altm_wext_msg *msg);
static int atbm_dev_set_efuse_deltagain(struct net_device *dev, struct altm_wext_msg *msg);
static int atbm_dev_set_efuse_mac(struct net_device *dev, struct altm_wext_msg *msg);
static int atbm_dev_set_efuse_gain_compensation_value(struct net_device *dev, struct altm_wext_msg *msg);
static int atbm_dev_get_etf_rx_results(struct net_device *dev, struct altm_wext_msg *msg);
#ifdef CONFIG_ATBM_SUPPORT_AP_CONFIG

static int atbm_dev_set_fix_scan_channel(struct net_device *dev, struct altm_wext_msg *msg);
#endif

#ifdef CONFIG_JUAN_MISC

static int atbm_dev_set_ap_tim_control(struct net_device *dev, struct altm_wext_msg *msg);
static int atbm_dev_get_ap_tim_control(struct net_device *dev, struct altm_wext_msg *msg);

#endif


dev_ioctl_cmd_t dev_cmd[]={
	{ATBM_DEV_IO_GET_STA_STATUS,atbm_dev_ioctl_sta_get_status,
				"ATBM_DEV_IO_GET_STA_STATUS:sta mode get connect status"},
	{ATBM_DEV_IO_GET_STA_RSSI,atbm_dev_ioctl_sta_get_rssi,
				"ATBM_DEV_IO_GET_STA_RSSI:sta mode get rssi"},
	{ATBM_DEV_IO_GET_AP_INFO,atbm_dev_get_ap_info,
				"ATBM_DEV_IO_GET_AP_INFO : sta mode get connect ap information "},
	{ATBM_DEV_IO_GET_STA_INFO,atbm_dev_get_sta_info,
				"ATBM_DEV_IO_GET_STA_INFO:ap mode get connect sta information"},
	{ATBM_DEV_IO_SET_STA_SCAN,atbm_dev_set_sta_scan,
				"ATBM_DEV_IO_SET_STA_SCAN:sta mode , Scan private data,You can specify channel and mac address scanning"},
	{ATBM_DEV_IO_SET_FREQ,atbm_dev_set_freq,
				"ATBM_DEV_IO_SET_FREQ:any mode,Change the frequency corresponding to a channel"},
	{ATBM_DEV_IO_SET_SPECIAL_OUI,atbm_dev_set_special_oui,
				"ATBM_DEV_IO_SET_SPECIAL_OUI:Insert private data into the Management frame"},
	{ATBM_DEV_IO_SET_STA_DIS,atbm_dev_set_sta_dis,
				"ATBM_DEV_IO_SET_STA_DIS:sta mode , disconnect form"},
	{ATBM_DEV_IO_SET_IF_TYPE,atbm_dev_set_iftype,
				"ATBM_DEV_IO_SET_IF_TYPE:The manager mode is switched to the monitor mode"},
	{ATBM_DEV_IO_SET_ADAPTIVE,atbm_dev_set_adaptive,
				"ATBM_DEV_IO_SET_ADAPTIVE:Set the adaptive mode"},
	{ATBM_DEV_IO_SET_TXPWR_DCXO,atbm_dev_set_txpwr_dcxo,
				"ATBM_DEV_IO_SET_TXPWR_DCXO:set efuse deltagain & dcxo"},
	{ATBM_DEV_IO_SET_TXPWR,atbm_dev_ioctl_set_txpower,
				"ATBM_DEV_IO_SET_TXPWR:add tx power with 0,3,15,63"},
	{ATBM_DEV_IO_GET_WORK_CHANNEL,atbm_dev_get_work_channel,
				"ATBM_DEV_IO_GET_WORK_CHANNEL:Gets the current working channel"},
	{ATBM_DEV_IO_SET_BEST_CHANNEL_SCAN,atbm_dev_set_best_channel_scan,
				"ATBM_DEV_IO_SET_BEST_CHANNEL_SCAN:Channels can be restricted for scanning to determine the best channel "},
	{ATBM_DEV_IO_GET_AP_LIST,atbm_dev_get_ap_list,
				"ATBM_DEV_IO_GET_AP_LIST:sta mode to scan"},
	{ATBM_DEV_IO_GET_TP_RATE,atbm_dev_get_tp_rate,
				"ATBM_DEV_IO_GET_TP_RATE:Gets the throughput of the current link"},
	{ATBM_DEV_IO_ETF_TEST,atbm_dev_etf_test,"ATBM_DEV_IO_ETF_TEST"},
	{ATBM_DEV_IO_ETF_GET_RESULT,atbm_dev_etf_get_result,"ATBM_DEV_IO_ETF_GET_RESULT"},
	{ATBM_DEV_IO_ETF_START_TX,atbm_dev_start_tx,"ATBM_DEV_IO_ETF_START_TX"},
	{ATBM_DEV_IO_ETF_STOP_TX,atbm_dev_stop_tx,"ATBM_DEV_IO_ETF_STOP_TX"},
	{ATBM_DEV_IO_ETF_START_RX,atbm_dev_start_rx,"ATBM_DEV_IO_ETF_START_RX"},
	{ATBM_DEV_IO_ETF_STOP_RX,atbm_dev_stop_rx,"ATBM_DEV_IO_ETF_STOP_RX"},
	{ATBM_DEV_IO_FIX_TX_RATE,atbm_dev_ioctl_set_txrate,"ATBM_DEV_IO_FIX_TX_RATE"},
	{ATBM_DEV_IO_MAX_TX_RATE,atbm_dev_ioctl_set_txrate,"ATBM_DEV_IO_MAX_TX_RATE"},
	{ATBM_DEV_IO_TX_RATE_FREE,atbm_dev_ioctl_set_txrate,"ATBM_DEV_IO_TX_RATE_FREE"},
	{ATBM_DEV_IO_SET_EFUSE_MAC,atbm_dev_ioctl_set_efuse_mac,"ATBM_DEV_IO_SET_EFUSE_MAC"},
	{ATBM_DEV_IO_SET_EFUSE_DCXO,atbm_dev_ioctl_set_efuse_dcxo,"ATBM_DEV_IO_SET_EFUSE_DCXO"},
	{ATBM_DEV_IO_SET_EFUSE_DELTAGAIN,atbm_dev_ioctl_set_efuse_deltagain,"ATBM_DEV_IO_SET_EFUSE_DELTAGAIN"},
	{ATBM_DEV_IO_MIN_TX_RATE,atbm_dev_ioctl_set_txrate,"ATBM_DEV_IO_MIN_TX_RATE"},
	{ATBM_DEV_IO_SET_RATE_POWER,atbm_dev_ioctl_set_ratePower,"ATBM_DEV_IO_SET_RATE_POWER"},
#ifdef CONFIG_IEEE80211_SPECIAL_FILTER

	{ATBM_DEV_IO_SET_SPECIAL_FILTER,atbm_dev_set_special_filter,"ATBM_DEV_IO_SET_SPECIAL_FILTER"},
#endif
	{ATBM_DEV_IO_SET_COUNTRY_CODE,atbm_dev_set_country_code,"ATBM_DEV_IO_SET_COUNTRY_CODE"},
	{ATBM_DEV_IO_GET_DRIVER_VERSION,atbm_dev_get_driver_version,"ATBM_DEV_IO_GET_DRIVER_VERSION"},
	{ATBM_DEV_IO_GET_EFUSE,atbm_dev_get_efuse,"ATBM_DEV_IO_GET_EFUSE"},
	{ATBM_DEV_IO_GET_ETF_START_RX_RESULTS,atbm_dev_get_etf_rx_results,"ATBM_DEV_IO_GET_ETF_START_RX_RESULTS"},
//	{ATBM_DEV_IO_SET_UPERR_PROCESS_PID,,},
#ifdef CONFIG_ATBM_SUPPORT_AP_CONFIG

	{ATBM_DEV_IO_SET_FIX_SCAN_CHANNEL,atbm_dev_set_fix_scan_channel,"ATBM_DEV_IO_SET_FIX_SCAN_CHANNEL"},
#endif
	{ATBM_DEV_IO_SET_AUTO_CALI_PPM,atbm_dev_set_cail_auto_ppm,"ATBM_DEV_IO_SET_AUTO_CALI_PPM"},
	{ATBM_DEV_IO_GET_CALI_REAULTS,atbm_dev_get_cail_ppm_results,"ATBM_DEV_IO_GET_CALI_REAULTS"},
	{ATBM_DEV_IO_SET_EFUSE_GAIN_COMPENSATION_VALUE,atbm_dev_set_efuse_gain_compensation_value,"ATBM_DEV_IO_SET_EFUSE_GAIN_COMPENSATION_VALUE"},
	{ATBM_DEV_IO_GET_VENDOR_SPECIAL_IE,atbm_dev_get_vendor_special_ie,"ATBM_DEV_IO_GET_VENDOR_SPECIAL_IE"},
#ifdef CONFIG_ATBM_STA_LISTEN

	{ATBM_DEV_IO_SET_STA_LISTEN_CHANNEL,atbm_dev_set_sta_listen_channel,"ATBM_DEV_IO_SET_STA_LISTEN_CHANNEL"},
#endif
#ifdef CONFIG_JUAN_MISC

	{ATBM_DEV_IO_SET_AP_TIM_CONTROL,atbm_dev_set_ap_tim_control,"ATBM_DEV_IO_SET_AP_TIM_CONTROL"},
	{ATBM_DEV_IO_GET_AP_TIM_CONTROL,atbm_dev_get_ap_tim_control,"ATBM_DEV_IO_GET_AP_TIM_CONTROL"},
#endif	
	{-1,NULL,NULL},

};


static int atbm_dev_ioctl_sta_get_status(struct net_device *dev, struct altm_wext_msg *msg)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	memcpy(msg->externData, &sdata->vif.bss_conf.assoc, 4);
	atbm_printk_err("%s : %s \n",__func__,sdata->vif.bss_conf.assoc?"ASSOC SUCCESS":"NOT CONNECT");
	return 0;
}

static int atbm_dev_ioctl_sta_get_rssi(struct net_device *dev, struct altm_wext_msg *msg)
{
	
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
    struct ieee80211_local *local = sdata->local;
	struct sta_info *sta = NULL;
    int rssi = 0;
	
    rcu_read_lock();
    list_for_each_entry_rcu(sta, &local->sta_list, list) {					
        if (sta->sdata->vif.type == NL80211_IFTYPE_STATION){	
            rssi = (s8) -atbm_ewma_read(&sta->avg_signal);
            memcpy(msg->externData, &rssi, 4);
            break;
        }
    }
    rcu_read_unlock();
	atbm_printk_err("%s : rssi = %d \n",__func__,rssi);
	return 0;
}
static int atbm_dev_ioctl_set_txpower(struct net_device *dev, struct altm_wext_msg *msg)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	struct atbm_vif *priv = (struct atbm_vif *)sdata->vif.drv_priv;

    struct atbm_common *hw_priv = priv->hw_priv;
	struct ieee80211_internal_wsm_txpwr txpwr;
	int ret = 0;
	memcpy(&txpwr.txpwr_indx, &msg->externData[0], sizeof(int)); 
	if(atbm_internal_wsm_txpwr(hw_priv,&txpwr) == false){
		ret = -EINVAL;
	}

	return 0;
}

static int atbm_dev_ioctl_set_txrate(struct net_device *dev, struct altm_wext_msg *msg)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	struct atbm_vif *priv = (struct atbm_vif *)sdata->vif.drv_priv;

    struct atbm_common *hw_priv = priv->hw_priv;
	int ret = 0;
	struct ieee80211_internal_rate_req req;
	req.flags = 0;
	req.rate  = 0;

	if(msg->type == ATBM_DEV_IO_FIX_TX_RATE){
		req.flags |= IEEE80211_INTERNAL_RATE_FLAGS_SET_TX_RATE;
		req.flags |= IEEE80211_INTERNAL_RATE_FLAGS_CLEAE_TOP_RATE;
	}

	if(msg->type == ATBM_DEV_IO_MAX_TX_RATE){
		req.flags |= IEEE80211_INTERNAL_RATE_FLAGS_SET_TOP_RATE;
		req.flags |= IEEE80211_INTERNAL_RATE_FLAGS_CLEAR_TX_RATE;
	}

	if(msg->type == ATBM_DEV_IO_TX_RATE_FREE){
		req.flags |= IEEE80211_INTERNAL_RATE_FLAGS_CLEAE_TOP_RATE;
		req.flags |= IEEE80211_INTERNAL_RATE_FLAGS_CLEAR_TX_RATE;
	}

	if(msg->type == ATBM_DEV_IO_MIN_TX_RATE){
		req.flags |= IEEE80211_INTERNAL_RATE_FLAGS_SET_MIN_RATE;
	}
	req.rate = msg->value;

	if(atbm_internal_wsm_set_rate(hw_priv,&req) == false){
		ret = -EINVAL;
	}else {
		ret = 0;
	}
	
	return ret;
}

static int atbm_dev_ioctl_set_ratePower(struct net_device *dev, struct altm_wext_msg *msg)
{

	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	struct atbm_vif *priv = (struct atbm_vif *)sdata->vif.drv_priv;

    struct atbm_common *hw_priv = priv->hw_priv;
	int ret = 0;
	
	struct ieee80211_internal_rate_power_req power_req;

	memset(&power_req,0,sizeof(struct ieee80211_internal_rate_power_req));

	power_req.rate_index = msg->externData[0];
	power_req.power		 = msg->externData[1];

	if(atbm_internal_wsm_set_rate_power(hw_priv,&power_req) == false){
		ret = -EINVAL;
	}else {
		ret = 0;
	}

	return 0;
}

static int atbm_dev_ioctl_set_efuse_mac(struct net_device *dev, struct altm_wext_msg *msg)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct atbm_vif *priv = (struct atbm_vif *)sdata->vif.drv_priv;
    struct atbm_common *hw_priv = priv->hw_priv;
	int ret = 0;
	u8 prv_mac[ETH_ALEN];
	
	memcpy(prv_mac,hw_priv->efuse.mac,ETH_ALEN);
	memcpy(hw_priv->efuse.mac,msg->externData,ETH_ALEN);

	if(atbm_save_efuse(hw_priv,&hw_priv->efuse) != 0){
		memcpy(hw_priv->efuse.mac,prv_mac,ETH_ALEN);
		ret = -EINVAL;
	}

	return ret;
}

static int atbm_dev_ioctl_set_efuse_dcxo(struct net_device *dev, struct altm_wext_msg *msg)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct atbm_vif *priv = (struct atbm_vif *)sdata->vif.drv_priv;
    struct atbm_common *hw_priv = priv->hw_priv;
	int ret = 0;
	u8 prv_dcxo = hw_priv->efuse.dcxo_trim;

	hw_priv->efuse.dcxo_trim = msg->externData[0];
	
	if(atbm_save_efuse(hw_priv,&hw_priv->efuse) != 0){
		hw_priv->efuse.dcxo_trim = prv_dcxo;
		ret = -EINVAL;
	}
	
	return ret;
}

static int atbm_dev_ioctl_set_efuse_deltagain(struct net_device *dev, struct altm_wext_msg *msg)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct atbm_vif *priv = (struct atbm_vif *)sdata->vif.drv_priv;
    struct atbm_common *hw_priv = priv->hw_priv;
	int ret = 0;
	
	u8 prv_deltagain1 = hw_priv->efuse.delta_gain1;
	u8 prv_deltagain2 = hw_priv->efuse.delta_gain2;
	u8 prv_deltagain3 = hw_priv->efuse.delta_gain3;

	hw_priv->efuse.delta_gain1 = msg->externData[0];
	hw_priv->efuse.delta_gain2 = msg->externData[1];
	hw_priv->efuse.delta_gain3 = msg->externData[2];

	if(atbm_save_efuse(hw_priv,&hw_priv->efuse) != 0){
		hw_priv->efuse.delta_gain1 = prv_deltagain1;
		hw_priv->efuse.delta_gain2 = prv_deltagain2;
		hw_priv->efuse.delta_gain3 = prv_deltagain3;
		ret = -EINVAL;
	}

	return ret;
}



static int atbm_dev_get_ap_info(struct net_device *dev, struct altm_wext_msg *msg)
{
    int i = 0;
    int ret = 0;
    struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
    struct ieee80211_local *local = sdata->local;
    struct atbm_vif *priv =  (struct atbm_vif *)sdata->vif.drv_priv;
    struct atbm_common *hw_priv = priv->hw_priv;	
    struct sta_info *sta = NULL;
    atbm_wifi_ap_info *p_info;
    
    char *pdata = NULL;

    dev_printk("%s\n", __func__);
    
    if(msg == NULL){
        return -1;
    }
    if(!(p_info = (atbm_wifi_ap_info *)atbm_kzalloc(WSM_MAX_NUM_LINK_AP*sizeof(atbm_wifi_ap_info), GFP_KERNEL))){
        return -ENOMEM;
    }

    dev_printk("@@@ sta cnt %d\n", hw_priv->connected_sta_cnt);
    
    rcu_read_lock();
        
    list_for_each_entry_rcu(sta, &local->sta_list, list) {

        if(sta != NULL){
            if (sta->sdata->vif.type == NL80211_IFTYPE_AP){
                p_info[i].wext_rssi = (s8) -atbm_ewma_read(&sta->avg_signal);
                p_info[i].rx_packets = sta->rx_packets;
                p_info[i].tx_packets = sta->tx_packets;
                p_info[i].tx_retry_count = sta->tx_retry_count;
                p_info[i].last_rx_rate_idx = sta->last_rx_rate_idx;
                
                memcpy(p_info[i].wext_mac, sta->sta.addr, ETH_ALEN);
                p_info[i].sta_cnt = hw_priv->connected_sta_cnt;
            
                dev_printk("%d: MAC "DEV_MACSTR"\n", i, DEV_MAC2STR(p_info[i].wext_mac));
                dev_printk("    RSSI %d\n", p_info[i].wext_rssi);
                dev_printk("    RX Pkts %ld\n", p_info[i].rx_packets);
                dev_printk("    TX Pkts %ld\n", p_info[i].tx_packets);
                dev_printk("    TX Retry %ld\n", p_info[i].tx_retry_count);
                
                ++i;
            }else{
                msg->value = (s8) -atbm_ewma_read(&sta->avg_signal);
                atbm_printk_wext("# rssi %d\n", msg->value);
                break;
            }
        }

    }

    rcu_read_unlock();

    memcpy(&pdata, &msg->externData[0], 4);

    if(pdata != NULL){
        if (copy_to_user(pdata, p_info, WSM_MAX_NUM_LINK_AP*sizeof(atbm_wifi_ap_info))){
            ret = -1;
        }
    }
    
    if(p_info != NULL)
        atbm_kfree(p_info);
    
    return ret;
}

static int atbm_dev_get_sta_info(struct net_device *dev, struct altm_wext_msg *msg)
{
    int ret = 0;
    struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
    //struct ieee80211_bss_conf *p_bss_conf = &sdata->vif.bss_conf;
    struct ieee80211_local *local = sdata->local;
    struct atbm_vif *priv =  (struct atbm_vif *)sdata->vif.drv_priv;
    //struct atbm_common *hw_priv = priv->hw_priv;	
    struct sta_info *sta = NULL;
    atbm_wifi_sta_info *p_info;

    printk("%s\n", __func__);

    if(msg == NULL){
        return -1;
    }

    if (priv->mode != NL80211_IFTYPE_STATION){
        return -1;
    }

    if(priv->join_status != ATBM_APOLLO_JOIN_STATUS_STA){
        return -1;
    }
    
    if(!(p_info = (atbm_wifi_sta_info *)atbm_kzalloc(sizeof(atbm_wifi_sta_info), GFP_KERNEL))){
        return -ENOMEM;
    }

    rcu_read_lock();
    list_for_each_entry_rcu(sta, &local->sta_list, list) {

        if(sta != NULL){
            if (sta->sdata->vif.type == NL80211_IFTYPE_STATION){	
                //packets num
                struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
                struct cfg80211_bss *cbss;

                cbss = ifmgd->associated;

                if(cbss){
                    const char *ssid = NULL;

                    ssid = ieee80211_bss_get_ie(cbss, ATBM_WLAN_EID_SSID);

                    if(ssid){						
                        memcpy(p_info->ssid, &ssid[2], ssid[1]);
                        p_info->ssid_len = ssid[1];
                    }
                }
                memcpy(p_info->bssid, sta->sta.addr, ETH_ALEN);
                p_info->rssi = (s8) -atbm_ewma_read(&sta->avg_signal);
                p_info->rx_packets = sta->rx_packets;
                p_info->tx_packets = sta->tx_packets;
                p_info->tx_retry_count = sta->tx_retry_count;
                break;
            }
        }

    }
    rcu_read_unlock();

    dev_printk("    SSID %s\n", p_info->ssid);
    dev_printk("    BSSID "DEV_MACSTR"\n", DEV_MAC2STR(p_info->bssid));
    dev_printk("    RSSI %d\n", p_info->rssi);
    dev_printk("    RX Pkts %ld\n", p_info->rx_packets);
    dev_printk("    TX Pkts %ld\n", p_info->tx_packets);
    dev_printk("    TX Retry %ld\n", p_info->tx_retry_count);

    memcpy((u8*)msg->externData, p_info, sizeof(atbm_wifi_sta_info));

    if(p_info != NULL)
        atbm_kfree(p_info);

    return ret;
}
static bool atbm_dev_handle_scan_sta(struct ieee80211_hw *hw,struct atbm_internal_scan_results_req *req,
											   struct ieee80211_internal_scan_sta *sta)
{
	Wifi_Recv_Info_t *info     = (Wifi_Recv_Info_t *)req->priv;
	Wifi_Recv_Info_t *pos_info = NULL;
	
	if(req->n_stas >= MAC_FILTER_NUM){
		return false;
	}

	if((sta->ie == NULL) || (sta->ie_len == 0)){
		return true;
	}
	pos_info = info+req->n_stas;
	req->n_stas ++;
	pos_info->channel = sta->channel;
	pos_info->Rssi = sta->signal;
	memcpy(pos_info->Bssid,sta->bssid,6);
	if(sta->ssid_len && sta->ssid)
		memcpy(pos_info->Ssid,sta->ssid,sta->ssid_len);
	if(sta->ie&&sta->ie_len)
		memcpy(pos_info->User_data,sta->ie,sta->ie_len);
	return true;
}
static int scan_result_filter_single_channel(u8 * recv_info,int channel)
{
	Wifi_Recv_Info_t *recv = NULL;
	//Wifi_Recv_Info_t recv_bak[MAC_FILTER_NUM];
	Wifi_Recv_Info_t *recv_bak = NULL;
	int i = 0,j = 0;
	if(!recv_info){
		atbm_printk_err("scan_result_filter_channel recv_info NULL \n");
		return -1;
	}
	recv_bak = atbm_kmalloc(sizeof(Wifi_Recv_Info_t) * MAC_FILTER_NUM , GFP_KERNEL);
	if(!recv_bak){
		atbm_printk_err("scan_result_filter_channel recv_bak NULL \n");
		return -1;
	}
	memset(recv_bak,0,sizeof(Wifi_Recv_Info_t)*MAC_FILTER_NUM);
	recv = (Wifi_Recv_Info_t *)recv_info;
	
	for(i = 0;i < MAC_FILTER_NUM;i++){
		if(recv[i].channel == channel){
			memcpy(&recv_bak[j],&recv[i],sizeof(Wifi_Recv_Info_t));
			j++;
		}
	}
	memcpy(recv_info,recv_bak,sizeof(Wifi_Recv_Info_t)*MAC_FILTER_NUM);
	if(recv_bak)
		atbm_kfree(recv_bak);
	return 0;
}

static int atbm_dev_set_sta_scan(struct net_device *dev, struct altm_wext_msg *msg)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	char zero_mac[ETH_ALEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	unsigned short channel;
	int ret = 0;
	struct ieee80211_internal_scan_request internal_scan;
	struct atbm_internal_scan_results_req req;
	//Wifi_Recv_Info_t *recv_data = NULL;
	u8 *user_pos = NULL;
	u8 *recv_info = NULL;
	u8 scan_ch = 0,scan_ch_list[3];
	
	memset(&internal_scan,0,sizeof(struct ieee80211_internal_scan_request));
	
	if(!ieee80211_sdata_running(sdata)){
		ret = -ENETDOWN;
		goto exit;
	}

	if(sdata->vif.type != NL80211_IFTYPE_STATION){
		ret = -EOPNOTSUPP;
		goto exit;  
    }
	recv_info = atbm_kzalloc(sizeof(Wifi_Recv_Info_t)*MAC_FILTER_NUM, GFP_KERNEL);

	if(recv_info == NULL){
		ret =  -ENOMEM;
		goto exit;
	}
	
	memcpy(&channel, &msg->externData[0], sizeof(channel));
	scan_ch = (u8)channel;
	
	if(memcmp(&msg->externData[2], zero_mac, ETH_ALEN) != 0){
		u8* mac = &msg->externData[2];
		u8 index = 0;
	
        internal_scan.macs = atbm_kzalloc(sizeof(struct ieee80211_internal_mac)*MAC_FILTER_NUM, GFP_KERNEL);
		
		if(internal_scan.macs == NULL){
			ret = -ENOMEM;
			goto exit;
		}

		for(index = 0;index<MAC_FILTER_NUM;index++){
			memcpy(internal_scan.macs[index].mac,&mac[6*index],6);
		}
		internal_scan.n_macs = MAC_FILTER_NUM;
    }
	if(scan_ch != 0){

		scan_ch_list[0] = scan_ch_list[1] = scan_ch_list[2] = scan_ch;
		internal_scan.channels = scan_ch_list;
		internal_scan.n_channels = 3;
		
	}else {
		internal_scan.channels = NULL;
		internal_scan.n_channels = 0;
	}
	

	if(atbm_internal_cmd_scan_triger(sdata,&internal_scan) == false){
		ret =  -EOPNOTSUPP;
		goto exit;
    }

	memcpy(&user_pos, &msg->externData[50], sizeof(void*));

	req.flush = true;
	req.n_stas = 0;
	req.priv = recv_info;
	req.result_handle = atbm_dev_handle_scan_sta;

	ieee80211_scan_internal_req_results(sdata->local,&req);
	
	if(internal_scan.n_channels == 1)
		scan_result_filter_single_channel(recv_info,scan_ch);
	
	if(copy_to_user(user_pos, recv_info, MAC_FILTER_NUM*sizeof(Wifi_Recv_Info_t)) != 0){
		ret = -EINVAL;
	}
exit:

	if(internal_scan.macs)
		atbm_kfree(internal_scan.macs);
	if(recv_info){
		atbm_kfree(recv_info);
	}
	return ret;
}
static int atbm_dev_set_freq(struct net_device *dev, struct altm_wext_msg *msg)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_internal_set_freq_req req;
	int ret = 0;
	unsigned short channel = 0;
	int freq;
	
	if(!ieee80211_sdata_running(sdata)){
		ret = -ENETDOWN;
		goto exit;
	}

	memcpy(&channel, &msg->externData[0], sizeof(unsigned short));
	memcpy(&freq, &msg->externData[2], sizeof(int));

	req.channel_num = channel;
	req.freq = freq;
	req.set = true;

	if(atbm_internal_freq_set(&sdata->local->hw,&req)==false){
		ret =  -EINVAL;
	}

	if(ret == 0)
		atbm_set_freq(sdata->local);
exit:
	return ret;
}
static int atbm_dev_set_special_oui(struct net_device *dev, struct altm_wext_msg *msg)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_sub_if_data *sdata_update;
	
	int ret = 0;
	char *special = NULL;
	int len = 0;
	
	if(!ieee80211_sdata_running(sdata)){
		ret = -ENETDOWN;
		goto exit;
	}

	len =strlen(msg->externData);

	if((len<0)||(len>255)){
		ret = -EINVAL;
		goto exit;
	}

	special = atbm_kzalloc(len, GFP_KERNEL);

	if(special == NULL){
		ret = -EINVAL;
		goto exit;
	}
	memcpy(special,msg->externData, len);

    {
  //  struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
    struct atbm_vif *priv = (struct atbm_vif *)sdata->vif.drv_priv;
  	  atbm_set_special_oui(priv->hw_priv, msg->externData, len);
    }

	list_for_each_entry(sdata_update, &local->interfaces, list){
		bool res = true;
		
		if(!ieee80211_sdata_running(sdata_update)){
			continue;
		}

		if(sdata_update->vif.type == NL80211_IFTYPE_STATION){
			res = ieee80211_ap_update_special_probe_request(sdata_update,special,len);
		}else if((sdata_update->vif.type == NL80211_IFTYPE_AP)&&
		         (rtnl_dereference(sdata_update->u.ap.beacon))){
		    res = ieee80211_ap_update_special_beacon(sdata_update,special,len);
			if(res == true){
				res = ieee80211_ap_update_special_probe_response(sdata_update,special,len);
			}
		}
		if(res == false){
			ret = -EOPNOTSUPP;
			goto exit;
		}
	}
exit:
	if(special)
		atbm_kfree(special);
	return ret;
}

static int atbm_dev_set_sta_dis(struct net_device *dev, struct altm_wext_msg *msg)
{
    struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
    struct atbm_vif *priv = (struct atbm_vif *)sdata->vif.drv_priv;
    
    dev_printk("%s\n", __func__);

    ieee80211_connection_loss(priv->vif);
    return 0;
}

static int atbm_dev_set_iftype(struct net_device *dev, struct altm_wext_msg *msg)
{
    struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_internal_iftype_req req;
    
    dev_printk("%s\n", __func__);

	req.if_type = (enum ieee80211_internal_iftype)msg->externData[0];
	req.channel = msg->externData[1];

	if(atbm_internal_cmd_req_iftype(sdata,&req) == false)
		return -1;

	return 0;
}
/*
msg.type:
	1 filter_frame 

	2 filter_ie 
	
	3 filter clean

	4 filter show

msg.value:
	filter_frame : 80 or 40
	filter_ie	 : ie

msg.externData:	
	filter_ie	 : oui1 oui2 pui3
*/
#ifdef CONFIG_IEEE80211_SPECIAL_FILTER

enum SPECIAL_FILTER_TYPE{
	FILTER_FRAME = 1,
	FILTER_IE	 = 2,
	FILTER_CLEAR = 3,
	FILTER_SHOW	 = 4,
};
static int atbm_dev_set_special_filter(struct net_device *dev, struct altm_wext_msg *msg)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_special_filter filter;
	struct ieee80211_special_filter_table *tables = NULL;
	char *results = NULL;
	int copy_len = 0;
	int total_len = 0,i = 0;
	int ret = -1;
	
	memset(&filter,0,sizeof(struct ieee80211_special_filter));
	switch(msg->value){
		case FILTER_FRAME:{
			filter.filter_action = (msg->externData[0]/10*16) + (msg->externData[0]%10) ;
			filter.flags = SPECIAL_F_FLAGS_FRAME_TYPE;
			atbm_printk_err("%s:action(%x)\n",__func__,filter.filter_action);
			ret = ieee80211_special_filter_register(sdata,&filter);
		}break;
		case FILTER_IE:{
			filter.filter_action = msg->externData[0];
			filter.flags = SPECIAL_F_FLAGS_FRAME_IE;
			
			if(msg->externData[1] > 0 ||
			   msg->externData[2] > 0 ||
			   msg->externData[3] > 0){
				filter.oui[0] = msg->externData[1];
				filter.oui[1] = msg->externData[2];
				filter.oui[2] = msg->externData[3];
				filter.flags |= SPECIAL_F_FLAGS_FRAME_OUI;
			}
			atbm_printk_err("%s:ie[%d],oui[%d:%d:%d]\n",__func__,filter.filter_action,
											filter.oui[0],filter.oui[1],filter.oui[2]);
			ret = ieee80211_special_filter_register(sdata,&filter);
		}break;
		case FILTER_CLEAR:{
			ret = ieee80211_special_filter_clear(sdata);
		}break;
		case FILTER_SHOW:{
			results = atbm_kzalloc(255,GFP_KERNEL);
	
			if(results == NULL){
				ret = -ENOMEM;
				atbm_printk_err("FILTER_SHOW :results malloc err! \n ");
				goto exit;
			}
			tables = atbm_kzalloc(sizeof(struct ieee80211_special_filter_table),GFP_KERNEL);
	
			if(tables == NULL){
				ret = -ENOMEM;
				atbm_printk_err("FILTER_SHOW :tables malloc err! \n ");
				goto exit;
			}
			ret = ieee80211_special_filter_request(sdata,tables);

			copy_len = scnprintf(results+total_len,255-total_len,"filter table --->\n");
			total_len += copy_len;
			
			for(i = 0;i < tables->n_filters;i++){

				if((tables->table[i].flags & IEEE80211_SPECIAL_FILTER_MASK) == SPECIAL_F_FLAGS_FRAME_TYPE)
					copy_len = scnprintf(results+total_len,255-total_len,"filter[%d]: frame [%x]\n",i,tables->table[i].filter_action);
				else if((tables->table[i].flags & IEEE80211_SPECIAL_FILTER_MASK) == SPECIAL_F_FLAGS_FRAME_IE)
					copy_len = scnprintf(results+total_len,255-total_len,"filter[%d]: ie[%d]\n",i,tables->table[i].filter_action);
				else if((tables->table[i].flags & IEEE80211_SPECIAL_FILTER_MASK) == (SPECIAL_F_FLAGS_FRAME_IE | SPECIAL_F_FLAGS_FRAME_OUI)){
					copy_len = scnprintf(results+total_len,255-total_len,"filter[%d]: ie[%d] oui[%d:%d:%d]\n",i,tables->table[i].filter_action,
						tables->table[i].oui[0],tables->table[i].oui[1],tables->table[i].oui[2]);
				}else {
					copy_len = scnprintf(results+total_len,255-total_len,"filter[%d]: unkown\n",i);
				}
				if(copy_len > 0){
					total_len+=copy_len;
					memcpy(msg->externData,results,total_len);
				}
			}
		}break;
		default:{
			atbm_printk_err("msg->value[%d] data err! \n",msg->value);
			ret = -ENOMEM;
		}break;
	}
exit:
	if(results)
		atbm_kfree(results);
	if(tables)
		atbm_kfree(tables);
	if(ret > 0)
		ret = 0;
	else
		ret = -1;
	
	return ret;
	
	
}
#endif
static int atbm_dev_set_adaptive(struct net_device *dev, struct altm_wext_msg *msg)
{
    int ret = 0;
    struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
    struct atbm_vif *priv = (struct atbm_vif *)sdata->vif.drv_priv;
    struct atbm_common *hw_priv = priv->hw_priv;

    int adaptive;
    char cmd[32];

    dev_printk("%s\n", __func__);

    /*
    0: disable
    1: enable
    */
    //adaptive = msg->value;
    memcpy(&adaptive, msg->externData, sizeof(int));

    memset(cmd, 0, sizeof(cmd));
    sprintf(cmd, "set_adaptive %d ", adaptive);
    
    dev_printk("atbm: %s\n", cmd);
    ret = wsm_write_mib(hw_priv, WSM_MIB_ID_FW_CMD, cmd, strlen(cmd), priv->if_id);
    if(ret < 0){
    }

    return ret;

}
static int atbm_dev_set_txpwr_dcxo(struct net_device *dev, struct altm_wext_msg *msg)
{
    int ret = 0;
    struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
    struct atbm_vif *priv = (struct atbm_vif *)sdata->vif.drv_priv;
    struct atbm_common *hw_priv = priv->hw_priv;

    int txpwr_L, txpwr_M, txpwr_H;
    int dcxo;
    char cmd[32];

    dev_printk("%s\n", __func__);
    
    memcpy(&txpwr_L, &msg->externData[0], 4);/*detla gain & txpwr*/
    if(txpwr_L > 32 || txpwr_L < -32){
        atbm_printk_err("error, txpwr_L %d\n", txpwr_L);
        return -1;
    }
    
    memcpy(&txpwr_M, &msg->externData[4], 4);/*detla gain & txpwr*/
    if(txpwr_M > 32 || txpwr_M < -32){
        atbm_printk_err("error, txpwr_M %d\n", txpwr_M);
        return -1;
    }	

    memcpy(&txpwr_H, &msg->externData[8], 4);/*detla gain & txpwr*/
    if(txpwr_H > 32 || txpwr_H < -32){
        atbm_printk_err("error, txpwr_H %d\n", txpwr_H);
        return -1;
    }

    memcpy(&dcxo, &msg->externData[12], 4);
    if(dcxo > 127 || dcxo < 0){
        atbm_printk_err("error, dcxo %d\n", dcxo);
        return -1;
    }
    
    memset(cmd, 0, sizeof(cmd));
    sprintf(cmd, "set_txpwr_and_dcxo,%d,%d,%d,%d ", txpwr_L, txpwr_M, txpwr_H, dcxo);
    
    dev_printk("atbm: %s\n", cmd);
    ret = wsm_write_mib(hw_priv, WSM_MIB_ID_FW_CMD, cmd, strlen(cmd), priv->if_id);
    if(ret < 0){
    }

    return ret;

}

int atbm_dev_set_txpwr(struct net_device *dev, struct altm_wext_msg *msg)
{
    int ret = 0;
    struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
    struct atbm_vif *priv = (struct atbm_vif *)sdata->vif.drv_priv;
    struct atbm_common *hw_priv = priv->hw_priv;

    int txpwr_indx = 0;
   // int txpwr = 0;
    char cmd[32];

    dev_printk("%s\n", __func__);
    
   // memcpy(&txpwr_indx, &msg->externData[0], sizeof(int));
    txpwr_indx = msg->externData[0];

	if(txpwr_indx > 127){
		txpwr_indx = txpwr_indx - 256;
		
	}

	if(txpwr_indx > 16 || txpwr_indx< -16){
		 dev_printk("txpwr_indx = %d , super range\n",txpwr_indx);
		 return -1;
	}
	
    memset(cmd, 0, sizeof(cmd));

    /*
    *0,3,15,63
    */
    sprintf(cmd, "set_rate_txpower_mode %d ", txpwr_indx);
    
    atbm_printk_err("atbm: %s , %d,txpwr_indx = %d\n", cmd,strlen(cmd),txpwr_indx);
    ret = wsm_write_mib(hw_priv, WSM_MIB_ID_FW_CMD, cmd, strlen(cmd), priv->if_id);
    if(ret < 0){
    }

    return ret;

}
static int atbm_dev_get_work_channel(struct net_device *dev, struct altm_wext_msg *msg)
{
	unsigned short channel = 0;
    struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
    struct ieee80211_local *local = sdata->local;
	struct ieee80211_channel_state *chan_state = NULL;
	struct ieee80211_sub_if_data *sdata_update;
	mutex_lock(&local->mtx);
		switch(sdata->vif.type){
			case NL80211_IFTYPE_STATION:{
				if(atbm_wifi_status_get() == 1){
					chan_state = ieee80211_get_channel_state(local, sdata_update);
					if(chan_state)
						channel = channel_hw_value(chan_state->oper_channel);
					else{
						
						channel = 0;
					}
				}else{
#ifdef CONFIG_ATBM_STA_LISTEN				
					if(local->listen_channel)
						channel = channel_hw_value(local->listen_channel);
					else
						channel = 0;
#endif					
				}
			}break;
			case NL80211_IFTYPE_AP:{
				chan_state = ieee80211_get_channel_state(local, sdata_update);
				if(chan_state)
					channel = channel_hw_value(chan_state->oper_channel);
				else
					channel = 0;
			}break;
			case NL80211_IFTYPE_MONITOR:{
				atbm_printk_err("NL80211_IFTYPE_MONITOR \n");
				chan_state = ieee80211_get_channel_state(local, sdata_update);
				if(chan_state)
					channel = channel_hw_value(chan_state->oper_channel);
				else
					channel = 0;
			}break;
			default:
				atbm_printk_err("other type(%d) \n",sdata->vif.type);
			break;
		}

	mutex_unlock(&local->mtx);
	atbm_printk_err("current work channel is [%d] \n",channel);
    //msg->value = hw_priv->channel->hw_value;
    memcpy(&msg->externData[0], &channel, sizeof(unsigned short));
    
    return 0;
}

static int atbm_dev_set_best_channel_scan(struct net_device *dev, struct altm_wext_msg *msg)
{
	int ret = 0;
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_internal_channel_auto_select_req req;
	struct ieee80211_internal_channel_auto_select_results results;
	Best_Channel_Scan_Result scan_result;
	u8 i = 0,j=0,k=0,start_channel,end_channel;
	u8 all_channels[18]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,36,38,40,42};
//	bool support_special=false;
	u8 all_n_channels = 0;
	u8 *ignor_channels = NULL,*vaid_channels = NULL;
	if(!ieee80211_sdata_running(sdata)){
		ret = -ENETDOWN;
		goto exit;
	}
	if(ieee8011_channel_valid(&local->hw,36) == true){
//		support_special = true;	
		all_n_channels = 18;
		atbm_printk_err("support_special! \n");
	}else{
//		support_special = false;	
		all_n_channels = 14;
		atbm_printk_err("not support_special! \n");
	}
	if(sdata->vif.type != NL80211_IFTYPE_STATION){
		ret = -EOPNOTSUPP;
		goto exit;
	}
	memset(&req,0,sizeof(struct ieee80211_internal_channel_auto_select_req));

	if(atbm_internal_channel_auto_select(sdata,&req) == false){
		ret = -EOPNOTSUPP;
	}
	
	memset(&scan_result,0,sizeof(Best_Channel_Scan_Result));
	memset(&results,0,sizeof(struct ieee80211_internal_channel_auto_select_results));
	results.version = 0;//use version 0

	/*
		start_channel
		end_channel
		2.4G & 5G
		if support 5G but not confine channel, default return 1~14 channel value , suggest_ch range 1~14
		
	*/
	start_channel = msg->externData[0];
	end_channel = msg->externData[1];
	//Determine channel validity
	
	for(i = 0; i < 18; i++){
		if(all_channels[i] == start_channel){
			j = i+1; //start channel valid
		}else if((all_channels[i] == end_channel) && (i > j)){
			k = i+1;//end_channel valid
		}
	}
	atbm_printk_err("start_channel index[%d] end_index[%d] \n",j,k);
	// channel valid 
	if((j != 0) && (k != 0)){
		results.ignore_n_channels = all_n_channels - (k - j + 1);
		ignor_channels = (u8 *)atbm_kmalloc(results.ignore_n_channels,GFP_KERNEL);
		if(ignor_channels == NULL){
			ret = false;
			goto exit;
		}
		results.n_channels = k - j + 1;
		vaid_channels = (u8 *)atbm_kmalloc(results.n_channels,GFP_KERNEL);
		if(vaid_channels == NULL){
			ret = false;
			goto exit;
		}
		j = 0;
		k = 0;
		for(i = 0 ; i < all_n_channels ; i++){
			if((all_channels[i] < start_channel) || (all_channels[i] > end_channel)){
				ignor_channels[j++] = all_channels[i];
				
				atbm_printk_err("ignor_channels[%d] : %d \n",j-1,ignor_channels[j-1]);
			}else{
				vaid_channels[k++] = all_channels[i];
				
				atbm_printk_err("vaid_channels[%d] : %d \n",k-1,vaid_channels[k-1]);
			}
				
		}
		results.ignore_channels = ignor_channels;
		//results.ignore_n_channels = all_n_channels - (end_channel - start_channel + 1);
		results.channels = vaid_channels;
		//results.n_channels = end_channel - start_channel + 1;
		
	}else{
		start_channel = 1;
		end_channel = 14;
		if(all_n_channels == 18){
			ignor_channels = (u8 *)atbm_kmalloc(all_n_channels - 14,GFP_KERNEL);
			if(ignor_channels == NULL){
				ret = false;
				goto exit;
			}
		}
			
		
		vaid_channels = (u8 *)atbm_kmalloc(14,GFP_KERNEL);
		if(vaid_channels == NULL){
			ret = false;
			goto exit;
		}
		j = 0;
		k = 0;
		for(i = 0 ; i < all_n_channels ; i++){
			if((all_channels[i] < start_channel) || (all_channels[i] > end_channel)){
				if(all_n_channels == 18){
					ignor_channels[j++] = all_channels[i];
					
					atbm_printk_err("ignor_channels[%d] : %d \n",j-1,ignor_channels[j-1]);
				}
			}else{
				vaid_channels[k++] = all_channels[i];
				
				atbm_printk_err("vaid_channels[%d] : %d \n",k-1,vaid_channels[k-1]);
			}
				
		}
		results.ignore_channels = ignor_channels;
		results.ignore_n_channels = all_n_channels - 14;
		results.channels = vaid_channels;
		results.n_channels = 14;
	}

	


	
	
	atbm_printk_err("[%s] start channel[%d] end channel[%d] results.ignore_n_channels[%d] results.n_channels[%d]\n",__func__,
											start_channel,end_channel,
											results.ignore_n_channels,results.n_channels);
	
	
	
	if(atbm_internal_channel_auto_select_results(sdata,&results) == false){
		ret = -EINVAL;
		goto exit;
	}
	
	

	for(i = 0;i<all_n_channels;i++){
		scan_result.channel_ap_num[i] = results.n_aps[i];
		scan_result.busy_ratio[i] = results.busy_ratio[i];
		scan_result.weight[i] = results.weight[i];
	}
	scan_result.suggest_ch = results.susgest_channel;
	
	
	atbm_printk_err("auto_select channel %d\n",scan_result.suggest_ch);
	memcpy(msg->externData, &scan_result, sizeof(scan_result));
exit:
	if(results.ignore_channels)
		atbm_kfree(results.ignore_channels);
	if(results.channels)
		atbm_kfree(results.channels);
	return ret;
}
static bool atbm_dev_handle_ap_list(struct ieee80211_hw *hw,struct atbm_internal_scan_results_req *req,
											   struct ieee80211_internal_scan_sta *sta)
{
	BEST_CHANNEL_INFO *info     = (BEST_CHANNEL_INFO *)req->priv;
	BEST_CHANNEL_INFO *pos_info = NULL;
	int i = 0;
	
	if(req->n_stas >= CHANNEL_NUM*AP_SCAN_NUM_MAX){
		return false;
	}

	if(sta->channel > 14){
		return false;
	}
	
	pos_info = info+(sta->channel -1)*AP_SCAN_NUM_MAX;

	for(i = 0;i<AP_SCAN_NUM_MAX;i++){
		
		if(pos_info->flag == 1){
			pos_info++;
			continue;
		}

		req->n_stas ++;
		pos_info->enc_type = (u8)sta->enc_type;
		pos_info->enc_type_name = sta->ieee80211_enc_type_name;
		pos_info->rssi = sta->signal;	
		memcpy(pos_info->mac_addr,sta->bssid,6);
		if(sta->ssid_len && sta->ssid)
			memcpy(pos_info->ssid,sta->ssid,sta->ssid_len);
		pos_info->flag = 1;

		break;
	}

	return true;
}

int atbm_dev_get_ap_list(struct net_device *dev, struct altm_wext_msg *msg)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	int ret = 0;
	struct ieee80211_internal_scan_request internal_scan;
	struct atbm_internal_scan_results_req req;
	u8 *user_pos = NULL;
	u8 *recv_info = NULL;

	memset(&internal_scan,0,sizeof(struct ieee80211_internal_scan_request));
	
	if(!ieee80211_sdata_running(sdata)){
		ret = -ENETDOWN;
		goto exit;
	}

	if(sdata->vif.type != NL80211_IFTYPE_STATION){
		ret = -EOPNOTSUPP;
		goto exit;  
    }
	recv_info = atbm_kzalloc(sizeof(BEST_CHANNEL_INFO)*CHANNEL_NUM*AP_SCAN_NUM_MAX, GFP_KERNEL);
	if(recv_info == NULL){
		ret =  -ENOMEM;
		goto exit;
	}

	if(atbm_internal_cmd_scan_triger(sdata,&internal_scan) == false){
		ret =  -EOPNOTSUPP;
		goto exit;
    }

	memcpy(&user_pos, &msg->externData[0], sizeof(void*));

	req.flush = true;
	req.n_stas = 0;
	req.priv = recv_info;
	req.result_handle = atbm_dev_handle_ap_list;

	ieee80211_scan_internal_req_results(sdata->local,&req);

	if(copy_to_user(user_pos, recv_info, sizeof(BEST_CHANNEL_INFO)*CHANNEL_NUM*AP_SCAN_NUM_MAX) != 0){
		ret = -EINVAL;
	}
exit:

	if(internal_scan.macs)
		atbm_kfree(internal_scan.macs);
	if(recv_info){
		atbm_kfree(recv_info);
	}
	return ret;
}
int atbm_dev_get_tp_rate(struct net_device *dev, struct altm_wext_msg *msg)
{
    int ret = 0;
    struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
    struct atbm_vif *priv = (struct atbm_vif *)sdata->vif.drv_priv;
    struct atbm_common *hw_priv = priv->hw_priv;
    char mac_addr[6];
    int sta_id = 0;
    unsigned int rate_val = 0;
    
    dev_printk("%s sdata->vif.type = %d NL80211_IFTYPE_STATION = %d \n", __func__,sdata->vif.type,NL80211_IFTYPE_STATION);

    //ap mode
    if(sdata->vif.type != NL80211_IFTYPE_STATION){
        //clear mac addr buffer
        memset(mac_addr, 0, 6);
        
        //convert mac string to mac hex format
        str2mac(mac_addr, msg->externData);
        
        //according to mac hex, find out sta link id
        sta_id = atbm_find_link_id(priv, mac_addr);

        atbm_printk_wext("sta_id %d\n", sta_id);
        wsm_write_mib(hw_priv, WSM_MIB_ID_GET_RATE, &sta_id, 1, priv->if_id);
    }
    
    wsm_read_mib(hw_priv, WSM_MIB_ID_GET_RATE, &rate_val, sizeof(unsigned int), priv->if_id);

    //convert bits/s
    rate_val = rate_val/2;
    atbm_printk_wext("rate: %d bits/s\n", rate_val);

    memcpy(&msg->externData[0], &rate_val, sizeof(unsigned int));

    return ret;
}
//extern void etf_param_init(struct atbm_common *hw_priv);
int atbm_dev_etf_test(struct net_device *dev, struct altm_wext_msg *msg)
{
    int i =0 ;
	u8 chipid = 0;
    struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
    struct atbm_vif *priv = (struct atbm_vif *)sdata->vif.drv_priv;
    struct atbm_common *hw_priv = ABwifi_vifpriv_to_hwpriv(priv);

    etf_param_init(hw_priv);
	chipid = GetChipVersion(hw_priv);

	ucWriteEfuseFlag = msg->value;
	atbm_printk_always("ucWriteEfuseFlag:%d\n", ucWriteEfuseFlag);

    etf_config.freq_ppm = 7000;
    etf_config.rssifilter = -100;
    etf_config.txevm = 400;
    etf_config.txevmthreshold = 400;
	if(chipid == 0x24)//athenaB(6022)
	{
		etf_config.rxevm = 400;
    	etf_config.rxevmthreshold = 400;
	}
	else if(chipid == 0x49)//AresB(6032)
	{
		etf_config.rxevm = 25;
    	etf_config.rxevmthreshold = 25;
	}
    etf_config.cableloss = 30*4;
    etf_config.featureid = MyRand();

	atbm_printk_always("featureid:%d\n", etf_config.featureid);
	atbm_printk_always("Freq:%d,txEvm:%d,rxEvm:%d,txevmthreshold:%d,rxevmthreshold:%d,Txpwrmax:%d,Txpwrmin:%d,Rxpwrmax:%d,Rxpwrmin:%d,rssifilter:%d,cableloss:%d,default_dcxo:%d\n",
		etf_config.freq_ppm,etf_config.txevm,etf_config.rxevm,etf_config.txevmthreshold,etf_config.rxevmthreshold,
		etf_config.txpwrmax,etf_config.txpwrmin,etf_config.rxpwrmax,
		etf_config.rxpwrmin,etf_config.rssifilter,etf_config.cableloss,etf_config.default_dcxo);

    hw_priv->etf_channel = 7;
    hw_priv->etf_channel_type = 0;
    hw_priv->etf_rate = 21;
    hw_priv->etf_len = 1000; 
    hw_priv->etf_greedfiled = 0;

    atbm_for_each_vif(hw_priv,priv,i){
        if((priv != NULL))
        {
            atbm_printk_wext("device ioctl etf test\n");
            down(&hw_priv->scan.lock);
            mutex_lock(&hw_priv->conf_mutex);
            //ETF_bStartTx = 1;
            wsm_start_tx_v2(hw_priv, priv->vif);
            
            mutex_unlock(&hw_priv->conf_mutex);
            break;
        }
    }

    return 0;
}

int atbm_dev_etf_get_result(struct net_device *dev, struct altm_wext_msg *msg)
{
    int ret = 0;
    u8 chipid = 0;
    char *buff = NULL;
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct atbm_common *hw_priv=local->hw.priv;
	
	chipid = GetChipVersion(hw_priv);

	if(!(buff = (char *)atbm_kzalloc(256, GFP_KERNEL))){
        return -ENOMEM;
    }
    memset(buff, 0, 256);

    sprintf(buff, "%dcfo:%d,txevm:%d,rxevm:%d,dcxo:%d,txrssi:%d,rxrssi:%d,result:%d (0:OK; -1:FreqOffset Error; -2:efuse hard error;"
        " -3:efuse no written; -4:efuse anaysis failed; -5:efuse full; -6:efuse version change; -7:rx null)",
	chipid,
	gRxs_s.Cfo,
    gRxs_s.txevm,
    gRxs_s.evm,
    gRxs_s.dcxo,
    gRxs_s.TxRSSI,
    gRxs_s.RxRSSI,
    gRxs_s.result
    );

	if(&msg->externData[0] != NULL)
	{
		memcpy(&msg->externData[0], buff, strlen(buff));
	}
	else
	{
		atbm_printk_always("error:msg->externData[0] is null\n");
	}
    

	if(buff)
		atbm_kfree(buff);

    
    return ret;
}
/*

channel = msg->externData[0];
band_value = msg->externData[1~2];
len = msg->externData[3~4];
is_40M = msg->externData[5];
greedfiled = msg->externData[6];

*/
static int atbm_dev_stop_tx(struct net_device *dev, struct altm_wext_msg *msg);

static int atbm_dev_start_tx(struct net_device *dev, struct altm_wext_msg *msg)
{
//	int i = 0;
	int ret = 0;
	int len = 0;
	int channel = 0;
//	u32 rate = 0;
	u32 is_40M = 0;
	int band_value = 0;
	int greedfiled = 0;
//	u8 ucDbgPrintOpenFlag = 1;
	short *len_p;
	start_tx_param_t tx_param;
//	char *extra = NULL;
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct atbm_common *hw_priv=local->hw.priv;
//	struct atbm_vif *vif;

	if(strlen(msg->externData) == 0)
	{
		atbm_printk_err("Invalid parameters\n");
		return 0;
	}
	/*
	if(!(extra = atbm_kmalloc(strlen(msg->externData), GFP_KERNEL)))
		return -ENOMEM;

	memcpy(extra, msg->externData, strlen(msg->externData));
	atbm_printk_err("%s\n", extra);

	sscanf(extra, "%d %d %d %d %d", &channel, &band_value, &len, &is_40M, &greedfiled);
*/
	channel = msg->externData[0];
	len_p = (short *)&msg->externData[1];
	band_value = *len_p;
	len_p = (short *)&msg->externData[3];
	len = *len_p;
	is_40M = msg->externData[5];
	greedfiled = msg->externData[6];
	
	atbm_printk_err("atbm_dev_start_tx:channel[%d],rate[%d].len[%d],is_40M[%d],greedfiled[%d]\n",
		channel,band_value,len,is_40M,greedfiled);
	
	if(channel < 0 || channel > 14){
		atbm_printk_err("invalid channel[%d]!\n",channel);
		ret = -EINVAL;
		goto exit;
	}
	if(is_40M)
		is_40M = 1;
	else
		is_40M = 0;


	tx_param.channel = channel;
	tx_param.rate = band_value;
	tx_param.is_40M = is_40M;
	tx_param.pktlen = len;
	tx_param.greedfiled = greedfiled;
	
	ret = atbm_internal_start_tx(hw_priv,&tx_param);

exit:
	return ret;
}

static int atbm_dev_stop_tx(struct net_device *dev, struct altm_wext_msg *msg)
{
//	int i = 0;
	int ret = 0;
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct atbm_common *hw_priv=local->hw.priv;
//	struct atbm_vif *vif;

	ret = atbm_internal_stop_tx(hw_priv);


	return ret;
}
/*
channel ----- msg->externData[0]
is_40M ------ msg->externData[1]

*/
static int atbm_dev_stop_rx(struct net_device *dev, struct altm_wext_msg *msg);

static int atbm_dev_start_rx(struct net_device *dev, struct altm_wext_msg *msg)
{
	
	int ret = 0;

	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct atbm_common *hw_priv=local->hw.priv;

	int channel,is_40M;


	if(strlen(msg->externData) == 0)
	{
		atbm_printk_err("Invalid parameters\n");
		return 0;
	}
	channel = msg->externData[0];
	is_40M = msg->externData[1];
	if(channel < 0 || channel > 14){
		atbm_printk_err("channel[%d] not support \n",channel);
		return -EINVAL;
	}
	if(is_40M)
		is_40M = 1;
	else
		is_40M = 0;

	ret = atbm_internal_start_rx(hw_priv,channel,is_40M);

	return ret;
}


static int atbm_dev_stop_rx(struct net_device *dev, struct altm_wext_msg *msg)
{

	int ret = 0;

	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct atbm_common *hw_priv=local->hw.priv;
	

	
	ret = atbm_internal_stop_rx(hw_priv,NULL);
	

	return ret;
}

static int atbm_dev_get_etf_rx_results(struct net_device *dev, struct altm_wext_msg *msg)
{

	u32 rx_status[3] = {0,0,0};
//	int len = 0;
	int ret = 0;
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct atbm_common *hw_priv=local->hw.priv;
	struct rx_results{
		u32  rxSuccess;
		u32 FcsErr;
		u32 PlcpErr;
		};
	struct rx_results rx_results_t;
	if(ETF_bStartRx == 0){
		atbm_printk_wext("%s:start rx not running\n",__func__);
		ret = -EOPNOTSUPP;
		goto exit;
	}

	ret = wsm_read_shmem(hw_priv,(u32)0x161001d8/*RX_STATUS_ADDR*/,rx_status,sizeof(rx_status));

	if(ret != 0){
		ret = -EINVAL;
		goto exit;
	}


	rx_results_t.rxSuccess = rx_status[0]-rx_status[1];
	rx_results_t.FcsErr = rx_status[1];
	rx_results_t.PlcpErr = rx_status[2];
	memcpy(msg->externData,&rx_results_t,sizeof(struct rx_results));
	
exit:
	return ret;
}
#ifdef CONFIG_ATBM_SUPPORT_AP_CONFIG

static int atbm_dev_set_fix_scan_channel(struct net_device *dev, struct altm_wext_msg *msg)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_internal_ap_conf conf_req,*conf_req_temp = NULL;
	int ret = 0;

	
	conf_req_temp = (struct ieee80211_internal_ap_conf *)msg->externData;


	memset(&conf_req,0,sizeof(struct ieee80211_internal_ap_conf));
	conf_req.channel = conf_req_temp->channel;
	atbm_printk_err("atbm_dev_set_fix_scan_channel : set fix scan channel =%d ! \n",conf_req.channel);
	if(conf_req.channel < 0 || conf_req.channel > 14){
			conf_req.channel = 0;
			atbm_printk_err("atbm_dev_set_fix_scan_channel : clear fix scan channel  ! \n");
	}
	
	if(atbm_internal_update_ap_conf(sdata,&conf_req,conf_req.channel == 0?true:false) == false)
		ret = -1;
	

	return ret;

}

#endif

static int atbm_dev_set_country_code(struct net_device *dev, struct altm_wext_msg *msg)
{
#ifdef  CONFIG_ATBM_5G_PRETEND_2G
	atbm_printk_err("this mode not support! \n");

	return 0;
#else
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	unsigned char country_code[3] = {0};
	int ret = 0;
	
	if(!ieee80211_sdata_running(sdata)){
		ret = -ENETDOWN;
		goto exit;
	}
	memcpy(country_code,&msg->externData[0],2);
	atbm_printk_err("atbm_dev_set_country_code:country_code = %c%c---------------\n",country_code[0],country_code[1]);
#if CONFIG_CFG80211_INTERNAL_REGDB
		

	if(atbm_set_country_code_to_cfg80211(local,country_code) < 0){
		ret = -EINVAL;
		goto exit;
	}
#else
	memcpy(local->country_code,country_code,2);
#endif
	
	ret = 0;
exit:
	return ret;
#endif	
}




static int atbm_dev_get_driver_version(struct net_device *dev, struct altm_wext_msg *msg)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct atbm_common *hw_priv=local->hw.priv;
	short *p;
	p = (short *)&msg->externData[0];
	atbm_get_drv_version(p);
	p = (short *)&msg->externData[2];
	*p = (short)hw_priv->wsm_caps.firmwareVersion;

	return 0;
}
static int atbm_dev_get_efuse(struct net_device *dev, struct altm_wext_msg *msg)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct atbm_common *hw_priv=local->hw.priv;
	struct efuse_headr efuse_data;
	int ret = -1;
	


	if ((ret = wsm_get_efuse_data(hw_priv, &efuse_data, sizeof(efuse_data))) == 0){	
		atbm_printk_init("Get efuse data is [%d,%d,%d,%d,%d,%d,%d,%d,%02x:%02x:%02x:%02x:%02x:%02x]\n",
				efuse_data.version,efuse_data.dcxo_trim,efuse_data.delta_gain1,efuse_data.delta_gain2,efuse_data.delta_gain3,
				efuse_data.Tj_room,efuse_data.topref_ctrl_bias_res_trim,efuse_data.PowerSupplySel,efuse_data.mac[0],efuse_data.mac[1],
				efuse_data.mac[2],efuse_data.mac[3],efuse_data.mac[4],efuse_data.mac[5]);
		memcpy(&hw_priv->efuse, &efuse_data, sizeof(struct efuse_headr));
	}
	else{
		atbm_printk_err("read efuse failed\n");
		return -1;
	}

	memcpy(msg->externData,&efuse_data,sizeof(struct efuse_headr));

	return 0;
}

static int  atbm_dev_set_cail_auto_ppm(struct net_device *dev, struct altm_wext_msg *msg)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct atbm_common *hw_priv=local->hw.priv;
	int i = 0;
	struct atbm_vif *vif;
	char buff_on[10] = "cfo 1";
	char buff_off[10] = "cfo 0";
	i = 0;
	char *ptr = NULL;

	if(msg->value == 0){
		ptr = buff_off;
	}else{
		ptr = buff_on;
	}

	
	atbm_for_each_vif(hw_priv,vif,i){
		if (vif != NULL)
		{
			
			WARN_ON(wsm_write_mib(hw_priv, WSM_MIB_ID_FW_CMD,
				ptr, 5, vif->if_id));
			break;
		}
	}
	return 0;
}

static int atbm_dev_get_vendor_special_ie(struct net_device *dev, struct altm_wext_msg *msg)
{
	struct atbm_vendor_cfg_ie * private_ie = NULL;

	private_ie = atbm_internal_get_6441_vendor_ie();
	if(private_ie){
		memcpy(msg->externData,private_ie,sizeof(struct atbm_vendor_cfg_ie));
	}

	return 0;
}
#ifdef CONFIG_ATBM_STA_LISTEN
//extern int ieee80211_set_sta_channel(struct ieee80211_sub_if_data *sdata,int channel);
static int atbm_dev_set_sta_listen_channel(struct net_device *dev, struct altm_wext_msg *msg)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
//	struct ieee80211_local *local = sdata->local;
//	struct atbm_common *hw_priv=local->hw.priv;
	int channel = 0,ret = 0;
	

	channel = msg->value;
	if(channel < 0 || channel > 14){
		atbm_printk_err("atbm_dev_set_sta_listen_channel , channel = %d err\n",channel);
		return -1;
	}
	atbm_printk_err("listen ++++++++++++++++++++++++++ \n");
	ret = ieee80211_set_sta_channel(sdata,channel);
	atbm_printk_err("listen --------------------------,ret = %d \n",ret);
	
	return 0;
}
#endif
#ifdef CONFIG_JUAN_MISC

static int atbm_dev_set_ap_tim_control(struct net_device *dev, struct altm_wext_msg *msg)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct atbm_common *hw_priv=local->hw.priv;
	struct TIM_Parameters tim_val;
	int ret = 0;
	if(!ieee80211_sdata_running(sdata) && (sdata->vif.type != NL80211_IFTYPE_AP)){
		atbm_printk_err("atbm_dev_set_ap_tim_control : not ap mode! \n");
		return -ENETDOWN;
	}
	tim_val.tim_user_control_ena = msg->externData[0];
	tim_val.tim_val = msg->value;

	atbm_printk_err("tim_control=%x , Bitmap control:0x%02x , Partial Virtual Bitmap:0x%x ! \n",
				tim_val.tim_user_control_ena,tim_val.tim_val & 0xff,tim_val.tim_val>>8);
	
	ret = wsm_set_tim(hw_priv,&tim_val,sizeof(struct TIM_Parameters));
	
	return ret;
	
}

static int atbm_dev_get_ap_tim_control(struct net_device *dev, struct altm_wext_msg *msg)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct atbm_common *hw_priv=local->hw.priv;
	struct TIM_Parameters tim_val;
	int ret;
	if(!ieee80211_sdata_running(sdata) && (sdata->vif.type != NL80211_IFTYPE_AP)){
		atbm_printk_err("atbm_dev_get_ap_tim_control : not ap mode! \n");
		return -ENETDOWN;
	}

	ret = wsm_get_tim(hw_priv,&tim_val,sizeof(struct TIM_Parameters));
	msg->externData[0] = tim_val.tim_user_control_ena;
	msg->value = tim_val.tim_val;
	atbm_printk_err("beacon tim :%s ,tim_control=%x , Bitmap control:0x%02x , Partial Virtual Bitmap:0x%x ! \n",
		tim_val.tim_user_control_ena==0?"auto control tim":"customer control tim",
		tim_val.tim_user_control_ena,tim_val.tim_val & 0xff,tim_val.tim_val>>8);

	return ret;
}

#endif

/*
	crystal_type : 
		2 ?a12?¨ª?¡ì¨¬?
		?????¦Ì?¨´?a?¨¤¨¢¡é?¡ì¨¬?

	??¨®D?¨¤¨¢¡é?¡ì¨¬?dcxo2?¨®DD¡ì¡ê?12?¨ª?¡ì¨¬?ppm2?¨º¨¹dcxo????
	
*/	
//extern 	u32 GetChipCrystalType(struct atbm_common *hw_priv);
static int atbm_dev_get_cail_ppm_results(struct net_device *dev, struct altm_wext_msg *msg)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct atbm_common *hw_priv=local->hw.priv;
	u8 dcxo = 0;
	u8 crystal_type = 0;
	int cfo_val = 0;
	
	crystal_type = GetChipCrystalType(hw_priv);
	
	dcxo = DCXOCodeRead(hw_priv);

	wsm_get_cfo_ppm_correction_value(hw_priv,&cfo_val,sizeof(int));
	
	atbm_printk_always("%s,dcxo:%d,cfo_val:%d ppm\n", crystal_type==2?"share crystal":"independent crystal", dcxo,cfo_val);
	msg->externData[0] = crystal_type;
	msg->externData[1] = dcxo;	
	msg->externData[2] = cfo_val;
	
	return  0;
}
static int atbm_dev_set_efuse_dcxo(struct net_device *dev, struct altm_wext_msg *msg)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct atbm_common *hw_priv=local->hw.priv;
	struct efuse_headr efuse_data;
	int ret = 0;
	int write_rom;

	memcpy(&efuse_data,&hw_priv->efuse,sizeof(struct efuse_headr));		
	efuse_data.dcxo_trim = msg->externData[0];
	write_rom = msg->value;
	
	DCXOCodeWrite(hw_priv,efuse_data.dcxo_trim);
	
	if(write_rom){
		if(atbm_save_efuse(hw_priv,&efuse_data) != 0){
			
			ret = -EINVAL;
		}else{
			hw_priv->efuse.dcxo_trim = efuse_data.dcxo_trim;
			memcpy(msg->externData,&hw_priv->efuse,sizeof(struct efuse_headr));
			ret = 0;
		}
	}
	
	return ret;
}

static int atbm_dev_set_efuse_deltagain(struct net_device *dev, struct altm_wext_msg *msg)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct atbm_common *hw_priv=local->hw.priv;
	struct efuse_headr efuse_data;
	int ret;
//	struct cfg_txpower_t configured_txpower;
	int write_rom;
	char writebuf[128] = "";

	memcpy(&efuse_data,&hw_priv->efuse,sizeof(struct efuse_headr));
	efuse_data.delta_gain1 = msg->externData[0];
	efuse_data.delta_gain2 = msg->externData[1];
	efuse_data.delta_gain3 = msg->externData[2];
	
	write_rom = msg->value;
		
	memset(writebuf, 0, sizeof(writebuf));
	sprintf(writebuf, "set_txpwr_and_dcxo,%d,%d,%d,%d ", efuse_data.delta_gain1,efuse_data.delta_gain2,
														 efuse_data.delta_gain1,efuse_data.dcxo_trim);
	
	atbm_printk_init("cmd: %s\n", writebuf);
	ret = wsm_write_mib(hw_priv, WSM_MIB_ID_FW_CMD, writebuf, strlen(writebuf), 0);
	if(ret < 0){
		atbm_printk_err("%s: write mib failed(%d). \n",__func__, ret);
		return -EINVAL;
	}
	
	if(write_rom == 1){
		if(atbm_save_efuse(hw_priv,&efuse_data) != 0){
			atbm_printk_err("%s: atbm_save_efuse failed(%d). \n",__func__, ret);
			ret = -EINVAL;
		}else{
			hw_priv->efuse.delta_gain1 = efuse_data.delta_gain1;
			hw_priv->efuse.delta_gain2 = efuse_data.delta_gain2;
			hw_priv->efuse.delta_gain3 = efuse_data.delta_gain3;
			memcpy(msg->externData,&hw_priv->efuse,sizeof(struct efuse_headr));
			ret = 0;
		}
	}
	return ret;
}
static int atbm_dev_set_efuse_mac(struct net_device *dev, struct altm_wext_msg *msg)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct atbm_common *hw_priv=local->hw.priv;
	struct efuse_headr efuse_data;
	int ret;

	/*
		Make sure the data is consistent with EFUSE
		Values in other sections are not modified
	*/
	if ((ret = wsm_get_efuse_data(hw_priv, &efuse_data, sizeof(efuse_data))) == 0){ 
			atbm_printk_init("Get efuse data is [%d,%d,%d,%d,%d,%d,%d,%d,%02x:%02x:%02x:%02x:%02x:%02x]\n",
					efuse_data.version,efuse_data.dcxo_trim,efuse_data.delta_gain1,efuse_data.delta_gain2,efuse_data.delta_gain3,
					efuse_data.Tj_room,efuse_data.topref_ctrl_bias_res_trim,efuse_data.PowerSupplySel,efuse_data.mac[0],efuse_data.mac[1],
					efuse_data.mac[2],efuse_data.mac[3],efuse_data.mac[4],efuse_data.mac[5]);
		}
		else{
			atbm_printk_err("ATBM_DEV_IO_SET_EFUSE_MAC : read efuse failed\n");
			return -1;
		}
		
		
		memcpy(efuse_data.mac,msg->externData,ETH_ALEN);

		if(atbm_save_efuse(hw_priv,&efuse_data) != 0){
		
			ret = -EINVAL;
		}else{
			memcpy(hw_priv->efuse.mac,efuse_data.mac,ETH_ALEN);
			memcpy(msg->externData,&hw_priv->efuse,sizeof(struct efuse_headr));
			ret = 0;
		}
	return ret;
}

/*
hw_priv->efuse ?????o??¡ä?¡ª??€???????????¡±1???????????????¨¨??efuse??????efuse¨¨¡¥???o??£¤¨¨??¨¨????¡ä?¨C¡ã

*/
static int atbm_dev_set_efuse_gain_compensation_value(struct net_device *dev, struct altm_wext_msg *msg)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct atbm_common *hw_priv=local->hw.priv;
	struct efuse_headr efuse_data;
	int compensation_val = 0,ret,write_rom;
	char writebuf[128] = "";

	struct cfg_txpower_t configured_txpower,configured_txpower_temp;
	int prv_deltagain;
	int delta_gain[3],deal_gain[3];
	int gain_for = 0;
//	static int read_flag = 0;
	/*
		Get efuse deltAgain of the register
	*/
	memset(&configured_txpower_temp,0,sizeof(configured_txpower_temp));
	memset(&configured_txpower,0,sizeof(configured_txpower));
	compensation_val = msg->externData[0];
	if((ret = wsm_get_cfg_txpower(hw_priv,&configured_txpower,sizeof(struct cfg_txpower_t))) != 0){
		atbm_printk_err("wsm_get_cfg_txpower failed\n");
		return -1;
	}
	if(memcmp(&configured_txpower_temp,&configured_txpower,sizeof(configured_txpower)) != 0){
		atbm_printk_err("gain1:%d gain2:%d gain3:%d \n",
					configured_txpower.set_txpwr_delta_gain[0],
					configured_txpower.set_txpwr_delta_gain[1],
					configured_txpower.set_txpwr_delta_gain[2]);
		delta_gain[0] = configured_txpower.set_txpwr_delta_gain[0];//hw_priv->efuse.delta_gain1;
		delta_gain[1] = configured_txpower.set_txpwr_delta_gain[1];//hw_priv->efuse.delta_gain2;
		delta_gain[2] = configured_txpower.set_txpwr_delta_gain[2];//hw_priv->efuse.delta_gain3;

		delta_gain[0] > 127?delta_gain[0]-=256:delta_gain[0];
		delta_gain[1] > 127?delta_gain[1]-=256:delta_gain[0];
		delta_gain[2] > 127?delta_gain[2]-=256:delta_gain[0];
		
		delta_gain[0] < 0?delta_gain[0]+=32:delta_gain[0];
		delta_gain[1] < 0?delta_gain[1]+=32:delta_gain[1];
		delta_gain[2] < 0?delta_gain[2]+=32:delta_gain[2];
		
		atbm_printk_err("gain1:%d gain2:%d gain3:%d \n",
					delta_gain[0],
					delta_gain[1],
					delta_gain[2]);
	}
	else{
		memset(&efuse_data,0,sizeof(struct efuse_headr));
		if ((ret = wsm_get_efuse_data(hw_priv, &efuse_data, sizeof(efuse_data))) == 0){	
			atbm_printk_init("Get efuse data is [%d,%d,%d,%d,%d,%d,%d,%d,%02x:%02x:%02x:%02x:%02x:%02x]\n",
					efuse_data.version,efuse_data.dcxo_trim,efuse_data.delta_gain1,efuse_data.delta_gain2,efuse_data.delta_gain3,
					efuse_data.Tj_room,efuse_data.topref_ctrl_bias_res_trim,efuse_data.PowerSupplySel,efuse_data.mac[0],efuse_data.mac[1],
					efuse_data.mac[2],efuse_data.mac[3],efuse_data.mac[4],efuse_data.mac[5]);

		}else{
			atbm_printk_err("wsm_get_efuse_data failed\n");
			return -1;
		}
		delta_gain[0] = efuse_data.delta_gain1;
		delta_gain[1] = efuse_data.delta_gain2;
		delta_gain[2] = efuse_data.delta_gain3;
		if(compensation_val == 0){
			atbm_printk_err("compensation_val == 0, not change! \n");
			return 0;
		}
	}	


	
	
	if(compensation_val > 128)
		compensation_val -= 256;
	atbm_printk_err("compensation_val = %d\n",compensation_val);

	for(gain_for = 0;gain_for<3;gain_for++)	{
		prv_deltagain = delta_gain[gain_for];
		if(prv_deltagain < 16){//?y¨ºy
			
			if(compensation_val >= 0){// gain add
				prv_deltagain = prv_deltagain + compensation_val;
				if(prv_deltagain > 128)
					prv_deltagain -= 256;
			
				if(prv_deltagain > 15){
					prv_deltagain = 15;
				}
				
			}else{//gain sub
				prv_deltagain = prv_deltagain + compensation_val;
				
				if(prv_deltagain > 128)
					prv_deltagain -= 256;
				
				if(prv_deltagain < 0){
					prv_deltagain += 32;
					if(prv_deltagain < 17)
						prv_deltagain = 17;
				}
				
			}

		}else if(prv_deltagain >= 16){//?o¨ºy
		
			if(compensation_val >= 0){// gain add
				prv_deltagain += compensation_val;
				if(prv_deltagain > 128)
					prv_deltagain -= 256;
		
				if(prv_deltagain >= 32){
					prv_deltagain = (prv_deltagain - 32);
					prv_deltagain > 15?prv_deltagain=15:prv_deltagain;
				}
				
			}else{//gain sub
				prv_deltagain += compensation_val;
				if(prv_deltagain > 128)
					prv_deltagain -= 256;
				if(prv_deltagain < 17){
					prv_deltagain = 17;
					
				}
				
			}

		}
		if(prv_deltagain > 128)
				prv_deltagain -= 256;
		deal_gain[gain_for] = prv_deltagain;

		atbm_printk_err(" prv_deltagain = %d ,gain[%d] = %d compensation_val = %d\n",
								prv_deltagain,gain_for,deal_gain[gain_for],compensation_val);
	}

	write_rom = msg->value;
		
	memset(writebuf, 0, sizeof(writebuf));

	sprintf(writebuf, "set_txpwr_and_dcxo,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d ", deal_gain[0],deal_gain[1], deal_gain[2],hw_priv->efuse.dcxo_trim,
		configured_txpower.set_b_txpwr_delta_gain[0],configured_txpower.set_b_txpwr_delta_gain[1],configured_txpower.set_b_txpwr_delta_gain[2],
		configured_txpower.set_gn_txpwr_delta_gain[0],configured_txpower.set_gn_txpwr_delta_gain[1],configured_txpower.set_gn_txpwr_delta_gain[2]);
	
	atbm_printk_init("cmd: %s\n", writebuf);
	ret = wsm_write_mib(hw_priv, WSM_MIB_ID_FW_CMD, writebuf, strlen(writebuf), 0);
	if(ret < 0){
		atbm_printk_err("%s: write mib failed(%d). \n",__func__, ret);
		return -EINVAL;
	}
	
	if(write_rom == 1){
		delta_gain[0] = hw_priv->efuse.delta_gain1;
		delta_gain[1] = hw_priv->efuse.delta_gain2;
		delta_gain[2] = hw_priv->efuse.delta_gain3;
		hw_priv->efuse.delta_gain1 = deal_gain[0];
		hw_priv->efuse.delta_gain2 = deal_gain[1];
		hw_priv->efuse.delta_gain3 = deal_gain[2];	
		if((ret = atbm_save_efuse(hw_priv,&hw_priv->efuse)) != 0){
			atbm_printk_err("%s: atbm_save_efuse failed(%d). \n",__func__, ret);
			hw_priv->efuse.delta_gain1 = delta_gain[0];
			hw_priv->efuse.delta_gain2 = delta_gain[1];
			hw_priv->efuse.delta_gain3 = delta_gain[2];
			ret = -EINVAL;
		}

		atbm_printk_init("dcxo:%d,gain1:%d,gain2:%d,gain3:%d,[%02x:%02x:%02x:%02x:%02x:%02x]\n",
					hw_priv->efuse.dcxo_trim,hw_priv->efuse.delta_gain1,hw_priv->efuse.delta_gain2,hw_priv->efuse.delta_gain3,
					hw_priv->efuse.mac[0],hw_priv->efuse.mac[1],hw_priv->efuse.mac[2],hw_priv->efuse.mac[3],
					hw_priv->efuse.mac[4],hw_priv->efuse.mac[5]);
	}
	
	return ret;
}




int atbm_wext_cmd(struct net_device *dev, void *data, int len)
{
    int ret = 0;
    struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
    struct ieee80211_local *local = sdata->local;
    struct atbm_vif *priv = (struct atbm_vif *)sdata->vif.drv_priv;
    struct ieee80211_hw *hw = &local->hw;

    struct atbm_common *hw_priv = priv->hw_priv;
    struct altm_wext_msg *msg = NULL;
	dev_ioctl_cmd_t *cmd_t = dev_cmd;
	int i = 0,found = 0;
    if(atomic_read(&priv->enabled)==0){
        atbm_printk_err("atbm_wext_cmd() priv is disabled\n");
        ret = -1;
        goto __Exit__;
    }

    if(hw_priv == NULL){
        atbm_printk_err("error, netdev NULL\n");
        ret = -1;
        goto __Exit__;
    }

    if(hw->vendcmd_nl80211 == 0)
    {
        struct nlattr *data_p = nla_find(data, len, ATBM_TM_MSG_DATA);
        if (!data_p){
            ret = -1;
            goto __Exit__;
        }
        msg = (struct altm_wext_msg *)nla_data(data_p);
    }
    else
        msg = (struct altm_wext_msg *)data;
        
    dev_printk("cmd type: %d\n", msg->type);


	for(i = 0;cmd_t[i].msg_type != -1;i++){
		if(msg->type == cmd_t[i].msg_type){
			found = 1;
			break;
		}
	}
	if(found){
		ret = cmd_t[i].func(dev, msg);
	}else{
		atbm_printk_err("cmd not found!\n");
		for(i = 0;cmd_t[i].msg_type != -1;i++)
			atbm_printk_err("supporrt cmd[%d] , %s \n",i,cmd_t[i].uage);
	}
#if 0
	
    switch (msg->type)
    {
        case ATBM_DEV_IO_GET_STA_STATUS:
            memcpy(msg->externData, &sdata->vif.bss_conf.assoc, 4);
            break;
        case ATBM_DEV_IO_GET_STA_RSSI:
            {
                struct sta_info *sta = NULL;
                int rssi = 0;
                rcu_read_lock();
                list_for_each_entry_rcu(sta, &local->sta_list, list) {					
                    if (sta->sdata->vif.type == NL80211_IFTYPE_STATION){	
                        rssi = (s8) -atbm_ewma_read(&sta->avg_signal);
                        memcpy(msg->externData, &rssi, 4);
                        break;
                    }
                }
                rcu_read_unlock();
            }
            break;
        case ATBM_DEV_IO_GET_AP_INFO:
            atbm_printk_wext("%s: get sta info(%d)\n",__func__, hw_priv->connected_sta_cnt);
            atbm_dev_get_ap_info(dev, msg);
            break;
        case ATBM_DEV_IO_GET_STA_INFO:
            atbm_dev_get_sta_info(dev, msg);
            break;
        case ATBM_DEV_IO_SET_STA_SCAN:
            atbm_dev_set_sta_scan(dev, msg);
            break;
        case ATBM_DEV_IO_SET_FREQ:
            atbm_dev_set_freq(dev, msg);
            break;
        case ATBM_DEV_IO_SET_SPECIAL_OUI:
            atbm_dev_set_special_oui(dev, msg);
            break;
        case ATBM_DEV_IO_SET_STA_DIS:
            atbm_dev_set_sta_dis(dev, msg);
            break;
        case ATBM_DEV_IO_SET_IF_TYPE:
            atbm_dev_set_iftype(dev, msg);
            break;
        case ATBM_DEV_IO_SET_ADAPTIVE:
            atbm_dev_set_adaptive(dev, msg);
            break;
        case ATBM_DEV_IO_SET_TXPWR_DCXO:
            atbm_dev_set_txpwr_dcxo(dev, msg);
            break;
#ifdef CONFIG_IEEE80211_SPECIAL_FILTER

		case ATBM_DEV_IO_SET_SPECIAL_FILTER:
			atbm_dev_set_special_filter(dev, msg);
			break;
#endif
        case ATBM_DEV_IO_SET_TXPWR:
        {
#ifdef CONFIG_JUAN_MISC
        	atbm_dev_set_txpwr(dev,msg);
#else
        	struct ieee80211_internal_wsm_txpwr txpwr;

			memcpy(&txpwr.txpwr_indx, &msg->externData[0], sizeof(int)); 
        	if(atbm_internal_wsm_txpwr(hw_priv,&txpwr) == false){
				ret = -EINVAL;
        	}
#endif
            break;
        }
        case ATBM_DEV_IO_GET_WORK_CHANNEL:
            atbm_dev_get_work_channel(dev, msg);
            break;
        case ATBM_DEV_IO_SET_BEST_CHANNEL_SCAN:
            atbm_dev_set_best_channel_scan(dev, msg);
            break;
        case ATBM_DEV_IO_GET_AP_LIST:
            atbm_dev_get_ap_list(dev, msg);
            break;
        case ATBM_DEV_IO_GET_TP_RATE:
            atbm_dev_get_tp_rate(dev, msg);
			break;
		case ATBM_DEV_IO_ETF_TEST:
            atbm_dev_etf_test(dev, msg);
            break;
        case ATBM_DEV_IO_ETF_GET_RESULT:
            atbm_dev_etf_get_result(dev, msg);
            break;
		case ATBM_DEV_IO_ETF_START_TX:
			atbm_dev_start_tx(dev, msg);
			break;
		case ATBM_DEV_IO_ETF_STOP_TX:
			atbm_dev_stop_tx(dev, msg);
			break;
		case ATBM_DEV_IO_ETF_START_RX:
			atbm_dev_start_rx(dev, msg);
			break;
   		case ATBM_DEV_IO_ETF_STOP_RX:
			atbm_dev_stop_rx(dev, msg);
			break;
		case ATBM_DEV_IO_FIX_TX_RATE:
		case ATBM_DEV_IO_MAX_TX_RATE:
		case ATBM_DEV_IO_TX_RATE_FREE:
		case ATBM_DEV_IO_MIN_TX_RATE:
		{
			struct ieee80211_internal_rate_req req;
			req.flags = 0;
			req.rate  = 0;

			if(msg->type == ATBM_DEV_IO_FIX_TX_RATE){
				req.flags |= IEEE80211_INTERNAL_RATE_FLAGS_SET_TX_RATE;
				req.flags |= IEEE80211_INTERNAL_RATE_FLAGS_CLEAE_TOP_RATE;
			}

			if(msg->type == ATBM_DEV_IO_MAX_TX_RATE){
				req.flags |= IEEE80211_INTERNAL_RATE_FLAGS_SET_TOP_RATE;
				req.flags |= IEEE80211_INTERNAL_RATE_FLAGS_CLEAR_TX_RATE;
			}

			if(msg->type == ATBM_DEV_IO_TX_RATE_FREE){
				req.flags |= IEEE80211_INTERNAL_RATE_FLAGS_CLEAE_TOP_RATE;
				req.flags |= IEEE80211_INTERNAL_RATE_FLAGS_CLEAR_TX_RATE;
			}

			if(msg->type == ATBM_DEV_IO_MIN_TX_RATE){
				req.flags |= IEEE80211_INTERNAL_RATE_FLAGS_SET_MIN_RATE;
			}
			req.rate = msg->value;

			if(atbm_internal_wsm_set_rate(hw_priv,&req) == false){
				ret = -EINVAL;
			}else {
				ret = 0;
			}
			break;
		}
		case ATBM_DEV_IO_SET_RATE_POWER:
		{
			struct ieee80211_internal_rate_power_req power_req;

			memset(&power_req,0,sizeof(struct ieee80211_internal_rate_power_req));

			power_req.rate_index = msg->externData[0];
			power_req.power		 = msg->externData[1];

			if(atbm_internal_wsm_set_rate_power(hw_priv,&power_req) == false){
				ret = -EINVAL;
			}else {
				ret = 0;
			}

			break;
		}
		case ATBM_DEV_IO_SET_EFUSE_MAC:
		{
			ret = atbm_dev_set_efuse_mac(dev,msg);
			break;
		}

		case ATBM_DEV_IO_SET_EFUSE_DCXO:
		{
			ret = atbm_dev_set_efuse_dcxo(dev,msg);
			break;
		}

		case ATBM_DEV_IO_SET_EFUSE_DELTAGAIN:
		{
			ret = atbm_dev_set_efuse_deltagain(dev,msg);
			
			break;
		}
		case ATBM_DEV_IO_SET_COUNTRY_CODE:
		{
			atbm_dev_set_country_code(dev, msg);
		}break;
		case ATBM_DEV_IO_GET_DRIVER_VERSION:{
			if(atbm_dev_get_driver_version(dev, msg) != 0)
				ret = -EINVAL;
			}break;
		case ATBM_DEV_IO_GET_EFUSE:{
			if(atbm_dev_get_efuse(dev, msg) != 0)
				ret = -EINVAL; 
			
			}break;
		case ATBM_DEV_IO_GET_ETF_START_RX_RESULTS:{
			if(atbm_dev_get_etf_rx_results(dev, msg) != 0)
				ret = -EINVAL; 
			}break;
		case ATBM_DEV_IO_SET_UPERR_PROCESS_PID:{
				local->upper_pid = msg->value;
				atbm_printk_err("%s : set upper process pid = %d \n",__func__, local->upper_pid);
	
				
			}break;
#ifdef CONFIG_ATBM_SUPPORT_AP_CONFIG

		case ATBM_DEV_IO_SET_FIX_SCAN_CHANNEL:{
				ret = atbm_dev_set_fix_scan_channel(dev,msg);
			}break;
#endif
		case ATBM_DEV_IO_SET_AUTO_CALI_PPM:{
				atbm_dev_set_cail_auto_ppm(dev,msg);
			}break;
		case ATBM_DEV_IO_GET_CALI_REAULTS:{
				ret = atbm_dev_get_cail_ppm_results(dev,msg);
			}break;
		case ATBM_DEV_IO_SET_EFUSE_GAIN_COMPENSATION_VALUE:{
				ret = atbm_dev_set_efuse_gain_compensation_value(dev,msg);
			}break;
		case ATBM_DEV_IO_GET_VENDOR_SPECIAL_IE:{
				ret = atbm_dev_get_vendor_special_ie(dev,msg);
			}break;
#ifdef CONFIG_ATBM_STA_LISTEN
		case ATBM_DEV_IO_SET_STA_LISTEN_CHANNEL:{
				ret = atbm_dev_set_sta_listen_channel(dev,msg);
			}break;
#endif
#ifdef CONFIG_JUAN_MISC

		case ATBM_DEV_IO_SET_AP_TIM_CONTROL:{
				ret = atbm_dev_set_ap_tim_control(dev,msg);
			}break;
		case ATBM_DEV_IO_GET_AP_TIM_CONTROL:{
				ret = atbm_dev_get_ap_tim_control(dev,msg);
			}break;
#endif
        default:
            atbm_printk_err("%s: not found. %d\n",__func__, msg->type);
            break;
    }
#endif
__Exit__:
    return ret;
}
#endif
