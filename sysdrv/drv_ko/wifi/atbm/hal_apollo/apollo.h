/*
 * Common private data for altobeam APOLLO drivers
 *
 * Copyright (c) 2016, altobeam
 * Author:
 *
 *  Based on apollo  Copyright (c) 2010, ST-Ericsson
 * Author: Dmitry Tarnyagin <dmitry.tarnyagin@stericsson.com>
 *
 * Based on the mac80211 Prism54 code, which is
 * Copyright (c) 2006, Michael Wu <flamingice@sourmilk.net>
 *
 * Based on the islsm (softmac prism54) driver, which is:
 * Copyright 2004-2006 Jean-Baptiste Note <jbnote@gmail.com>, et al.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef ATBM_APOLLO_H
#define ATBM_APOLLO_H
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/mutex.h>
#include <linux/workqueue.h>
#include <linux/sched.h>
#include <linux/atomic.h>
#include <net/atbm_mac80211.h>
#include "apollo_plat.h"

#ifdef P2P_MULTIVIF
#define ATBM_WIFI_MAX_VIFS			(3)
#else
#define ATBM_WIFI_MAX_VIFS			(2)
#endif
#define ATBM_WIFI_GENERIC_IF_ID		(2)
#define ATBM_WIFI_HOST_VIF0_11N_THROTTLE	(ATBM_WIFI_MAX_QUEUE_SZ*3/4-1)
#define ATBM_WIFI_HOST_VIF1_11N_THROTTLE	(ATBM_WIFI_MAX_QUEUE_SZ*3/4-1)
#define ATBM_WIFI_HOST_VIF0_11BG_THROTTLE	(63)
#define ATBM_WIFI_HOST_VIF1_11BG_THROTTLE	(63)


#define ATBM_WIFI_MAX_QUEUE_SZ		(256)

#define ATBM_QUEUE_DEFAULT_CAP		(ATBM_WIFI_MAX_QUEUE_SZ>>4)
#define ATBM_QUEUE_SINGLE_CAP		(ATBM_WIFI_MAX_QUEUE_SZ*7/8)
#define ATBM_QUEUE_COMB_CAP			(ATBM_WIFI_MAX_QUEUE_SZ/2-8)


#define ST_PAYLOAD_BUF_SZ			(256)

#define IEEE80211_FCTL_WEP      0x4000
#define IEEE80211_QOS_DATAGRP   0x0080
#define WSM_KEY_MAX_IDX		20

#include "queue.h"
#include "wsm.h"
#include "scan.h"
#include "txrx.h"
#include "ht.h"
#include "pm.h"
#include "fwio.h"
//#ifdef CONFIG_ATBM_APOLLO_TESTMODE
#include "atbm_testmode.h"
//#endif /*CONFIG_ATBM_APOLLO_TESTMODE*/
#ifdef ATBM_SUPPORT_SMARTCONFIG
#include "smartconfig.h"
#endif

/* extern */ struct sbus_ops;
/* extern */ struct task_struct;
/* extern */ struct atbm_debug_priv;
/* extern */ struct atbm_debug_common;
/* extern */ struct firmware;

/* #define ROC_DEBUG */

/* hidden ssid is only supported when separate probe resp IE
   configuration is supported */
#ifdef ATBM_PROBE_RESP_EXTRA_IE
#define HIDDEN_SSID	1
#endif

#if defined(CONFIG_ATBM_APOLLO_TXRX_DEBUG)
#define txrx_printk(...) atbm_printk_always(__VA_ARGS__)
#else
#define txrx_printk(...)
#endif

#define ATBM_APOLLO_MAX_CTRL_FRAME_LEN	(0x1000)

#define ATBMWIFI_MAX_STA_IN_AP_MODE	(14)
#define WLAN_LINK_ID_MAX		(ATBMWIFI_MAX_STA_IN_AP_MODE + 3)
#define ATBM_UAPSD_VIRTUAL_LINKID					(WLAN_LINK_ID_MAX - 1)
#define ATBM_DTIM_VIRTUAL_LINKID					(WLAN_LINK_ID_MAX - 2)
static inline u32 atbm_uapsd_virtual_linkid(void)
{
	return ATBM_UAPSD_VIRTUAL_LINKID;
}
static inline u32 atbm_dtim_virtual_linkid(void)
{
	return ATBM_DTIM_VIRTUAL_LINKID; 
}

#define ATBM_APOLLO_MAX_STA_IN_AP_MODE		 ATBMWIFI_MAX_STA_IN_AP_MODE
#define ATBM_APOLLO_MAX_REQUEUE_ATTEMPTS	(5)
#define ATBM_APOLLO_LINK_ID_UNMAPPED		(15)

#define ATBM_APOLLO_MAX_TID			(8)

#define ATBM_APOLLO_TX_BLOCK_ACK_ENABLED_FOR_ALL_TID         (0xff)
#define ATBM_APOLLO_RX_BLOCK_ACK_ENABLED_FOR_ALL_TID         (0xff)
#define ATBM_APOLLO_RX_BLOCK_ACK_ENABLED_FOR_BE_TID \
	(ATBM_APOLLO_TX_BLOCK_ACK_ENABLED_FOR_ALL_TID & 0x01)
#define ATBM_APOLLO_TX_BLOCK_ACK_DISABLED_FOR_ALL_TID	(0)
#define ATBM_APOLLO_RX_BLOCK_ACK_DISABLED_FOR_ALL_TID	(0)

#define ATBM_APOLLO_BLOCK_ACK_CNT		(30)
#define ATBM_APOLLO_BLOCK_ACK_THLD		(800)
#define ATBM_APOLLO_BLOCK_ACK_HIST		(3)
#define ATBM_APOLLO_BLOCK_ACK_INTERVAL	(1 * HZ / ATBM_APOLLO_BLOCK_ACK_HIST)
#define ATBM_WIFI_ALL_IFS			(-1)
#ifdef CONFIG_ATBM_SUPPORT_SCHED_SCAN
#ifdef ROAM_OFFLOAD
#define ATBM_APOLLO_SCAN_TYPE_ACTIVE 0x1000
#define ATBM_APOLLO_SCAN_BAND_5G 0X2000
#endif /*ROAM_OFFLOAD*/
#endif
#define IEEE80211_FCTL_WEP      0x4000
#define IEEE80211_QOS_DATAGRP   0x0080
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
#define ATBM_APOLLO_SCAN_MEASUREMENT_PASSIVE (0)
#define ATBM_APOLLO_SCAN_MEASUREMENT_ACTIVE  (1)
#endif

#ifdef MCAST_FWDING
#define WSM_MAX_BUF		30
#endif

/* Please keep order */
enum atbm_join_status {
	ATBM_APOLLO_JOIN_STATUS_PASSIVE = 0,	
	ATBM_APOLLO_JOIN_STATUS_STA_LISTEN,
	ATBM_APOLLO_JOIN_STATUS_MONITOR,
	ATBM_APOLLO_JOIN_STATUS_IBSS,
	ATBM_APOLLO_JOIN_STATUS_STA,
	ATBM_APOLLO_JOIN_STATUS_AP,
	ATBM_APOLLO_JOIN_STATUS_SIMPLE_MONITOR,
};

enum atbm_link_status {
	ATBM_APOLLO_LINK_OFF,
	ATBM_APOLLO_LINK_RESERVE,
	ATBM_APOLLO_LINK_SOFT,
	ATBM_APOLLO_LINK_HARD,
	ATBM_APOLLO_LINK_RESET,
	ATBM_APOLLO_LINK_RESET_REMAP,
};

enum atbm_bss_loss_status {
	ATBM_APOLLO_BSS_LOSS_NONE,
	ATBM_APOLLO_BSS_LOSS_CHECKING,
	ATBM_APOLLO_BSS_LOSS_CONFIRMING,
	ATBM_APOLLO_BSS_LOSS_CONFIRMED,
};

struct atbm_link_entry {
	unsigned long			timestamp;
	enum atbm_link_status		status;
	enum atbm_link_status		prev_status;
	u8				mac[ETH_ALEN];
	u8				buffered[ATBM_APOLLO_MAX_TID];
	struct sk_buff_head		rx_queue;
};
#ifdef CONFIG_ATBM_APOLLO_DEBUG
struct atbm_debug_common {
#ifdef CONFIG_ATBM_APOLLO_DEBUGFS
	struct dentry *debugfs_phy;
#endif
	int tx_cache_miss;
	int tx_burst;
	int rx_burst;
	int ba_cnt;
	int ba_acc;
	int ba_cnt_rx;
	int ba_acc_rx;
};

struct atbm_debug_priv {
#ifdef CONFIG_ATBM_APOLLO_DEBUGFS
	struct dentry *debugfs_phy;
#endif
	int tx;
	int tx_agg;
	int rx;
	int rx_agg;
	int tx_multi;
	int tx_multi_frames;
	int tx_align;
	int tx_ttl;
};
#endif
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
struct advance_scan_elems {
	u8 scanMode;
	u16 duration;
};
/**
 * atbm_tsm_info - Keeps information about ongoing TSM collection
 * @ac: Access category for which metrics to be collected
 * @use_rx_roaming: Use received voice packets to compute roam delay
 * @sta_associated: Set to 1 after association
 * @sta_roamed: Set to 1 after successful roaming
 * @roam_delay: Roam delay
 * @rx_timestamp_vo: Timestamp of received voice packet
 * @txconf_timestamp_vo: Timestamp of received tx confirmation for
 * successfully transmitted VO packet
 * @sum_pkt_q_delay: Sum of packet queue delay
 * @sum_media_delay: Sum of media delay
 *
 */
struct atbm_tsm_info {
	u8 ac;
	u8 use_rx_roaming;
	u8 sta_associated;
	u8 sta_roamed;
	u16 roam_delay;
	u32 rx_timestamp_vo;
	u32 txconf_timestamp_vo;
	u32 sum_pkt_q_delay;
	u32 sum_media_delay;
};

/**
 * atbm_start_stop_tsm - To start or stop collecting TSM metrics in
 * apollo driver
 * @start: To start or stop collecting TSM metrics
 * @up: up for which metrics to be collected
 * @packetization_delay: Packetization delay for this TID
 *
 */
struct atbm_start_stop_tsm {
	u8 start;       /*1: To start, 0: To stop*/
	u8 up;
	u16 packetization_delay;
};

#endif /* CONFIG_ATBM_APOLLO_TESTMODE */
#define MAX_SCAN_INFO_NUM 40
struct atbmwifi_scan_result_info{
	u8 ssid[33];
	u8 ssidlen;
	u8 channel;
	u8 ht:1,
	   wpa:1,
	   rsn:1,
	   wps:1,
	   p2p:1,
	   b40M:1,
	   encrypt:1;
};

struct atbmwifi_scan_result{
	int len;
	struct atbmwifi_scan_result_info *info;
};
struct hmac_configure {
	u8 ssid[32];
	u8 ssid_len;
	u8 bssid[ETH_ALEN];
	u8 password_len;
	u8 password[64];
	u8 privacy:1,
		psk_set:1,
		bssid_set:1,
		fixrate_set:1;
	u8 ap_mode;
	u8 hw_fix_rate_id;
	u8 reserve;

	u8 key_mgmt;
	u8 key_id;
	u16 auth_alg;
};
struct atbm_common {
	struct atbm_debug_common	*debug;
	struct atbm_queue		tx_queue[4];
	struct atbm_queue_stats	tx_queue_stats;

	struct ieee80211_hw		*hw;
	struct mac_address		addresses[ATBM_WIFI_MAX_VIFS];

	/*Will be a pointer to a list of VIFs - Dynamically allocated */
	#ifdef  ATBM_VIF_LIST_USE_RCU_LOCK
	struct ieee80211_vif		__rcu *vif_list[ATBM_WIFI_MAX_VIFS+1];
	#else
	struct ieee80211_vif		*vif_list[ATBM_WIFI_MAX_VIFS+1];
	#endif
	atomic_t			num_vifs;
	spinlock_t			vif_list_lock;
	u32				if_id_slot;
	struct device			*pdev;
	struct atbm_workqueue_struct		*workqueue;
	struct sk_buff_head			rx_frame_queue;
	struct sk_buff_head			rx_frame_free;
	struct mutex			conf_mutex;

	const struct sbus_ops		*sbus_ops;
	struct sbus_priv		*sbus_priv;
	int init_done;
	/* HW/FW type (HIF_...) */
	int				hw_type;
	int				hw_revision;
	int				fw_revision;
	int             chip_version;
	/* firmware/hardware info */
	unsigned int tx_hdr_len;

	/* Radio data */
	int output_power;
	int noise;
#ifdef ATBM_SUPPORT_SMARTCONFIG
	spinlock_t	spinlock_smart;
	struct atbmwifi_scan_result scan_ret;
	struct smartconfig_config st_cfg;
	int st_status;
	int st_configchannel;
	struct atbm_timer_list smartconfig_expire_timer;
	struct hmac_configure *config;
#endif
	/* calibration, output power limit and rssi<->dBm conversation data */

	/* BBP/MAC state */
	const struct firmware		*sdd;
	struct ieee80211_rate		*rates;
	struct ieee80211_rate		*mcs_rates;
	u8 mac_addr[ETH_ALEN];
	/*TODO:COMBO: To be made per VIFF after mac80211 support */
	struct ieee80211_channel	*channel;
#ifdef CONFIG_ATBM_STA_LISTEN
	/*sta listen function*/
	struct ieee80211_channel	__rcu *sta_listen_channel;
	int 						sta_listen_if;
	int 						sta_listen_if_save;
#endif
#ifdef ATBM_SUPPORT_WIDTH_40M	

	/*
	*channel_type :record current channel type.
	*maybe changed by action frame.
	*/
	spinlock_t				  spinlock_channel_type;
	enum nl80211_channel_type channel_type;
	/*
	*chantype_change_work:
	*/
	struct mutex			  chantype_mutex;
	atomic_t				  channel_chaging;
	unsigned long						  change_bit;
	
	struct atbm_timer_list		  chantype_timer;
	struct atbm_work_struct		  get_cca_work;
	atomic_t				  phy_chantype;/*0:20M 1:40M*/
	atomic_t				  tx_20M_lock;
	atomic_t				  cca_detect_running;
	atomic_t					  cca_interval_ms;
	atomic_t					  chw_sw_20M_level;
	atomic_t				         chw_sw_40M_level;
	atomic_t				  atbm_pluged;
	u32						  rx_phy_enable_num_req;
	//u32						  last_cca;
	//u32						  cur_cca;
	u32					rx_40M_pkg_cnt;
	u32					rx_20M_pkg_cnt;
	u32					rx_40M_pkg_detect;
	u32					rx_20M_pkg_detect;
	u8					chanch_if_id;
#endif

	int				channel_switch_in_progress;
	wait_queue_head_t		channel_switch_done;
	u8				long_frame_max_tx_count;
	u8				short_frame_max_tx_count;
	/* TODO:COMBO: According to Hong aggregation will happen per VIFF.
	* Keeping in common structure for the time being. Will be moved to VIFF
	* after the mechanism is clear */
	//u8				ba_tid_mask;
	u8 				ba_tid_tx_mask;
	u8				ba_tid_rx_mask;
#ifdef CONFIG_ATBM_BA_STATUS
	int				ba_acc; /*TODO: Same as above */
	int				ba_cnt; /*TODO: Same as above */
	int				ba_cnt_rx; /*TODO: Same as above */
	int				ba_acc_rx; /*TODO: Same as above */
	int				ba_hist; /*TODO: Same as above */
	struct atbm_timer_list		ba_timer;/*TODO: Same as above */
	spinlock_t			ba_lock; /*TODO: Same as above */
	bool				ba_ena; /*TODO: Same as above */
	struct atbm_work_struct              ba_work; /*TODO: Same as above */
#endif
	#ifdef CONFIG_PM
	struct atbm_pm_state		pm_state;
	#endif
	bool				is_BT_Present;
	bool				is_go_thru_go_neg;
	u8				conf_listen_interval;

	/* BH */
	atomic_t			bh_rx;
	atomic_t			bh_tx;
	atomic_t			bh_term;
#ifdef SPI_BUS
	atomic_t			hw_ready;
#endif
	atomic_t			bh_suspend;
	atomic_t			bh_suspend_usb;
	atomic_t			bh_halt;
#ifdef ATBM_USB_RESET
	atomic_t			usb_reset;
#endif
#ifdef SDIO_BUS
	bool				hard_irq;
#endif
	struct task_struct		*bh_thread;
	int				bh_error;
	wait_queue_head_t		bh_wq;
	wait_queue_head_t		bh_evt_wq;
	int 			buf_id_offset;
	int				buf_id_tx;	/* byte */
	int				buf_id_rx;	/* byte */
	int				wsm_rx_seq;	/* byte */
	int				wsm_tx_seq;	/* byte */
#ifdef USB_CMD_UES_EP0
	int				wsm_tx_seq_ep0;	/* byte */
#endif
	int				wsm_txframe_num;	/* byte */
	int				wsm_txconfirm_num;	/* byte */
	int				wsm_hifconfirm_num;	/* byte */
	int				wsm_hiftx_num;	/* byte */
	u8 				*save_buf;
	u32 			save_buf_len;
	int 			save_buf_vif_selected;
	int             buf_id_tx_small;
	int				hw_bufs_used;
	int             hw_bufs_free;
	int				hw_bufs_free_init;
	u32 			n_xmits;
	u32				hw_xmits;
	int 			hw_noconfirm_tx;
	int				hw_bufs_used_vif[ATBM_WIFI_MAX_VIFS];
	struct sk_buff			*skb_cache;
	bool				device_can_sleep;
	/* Keep apollo awake (WUP = 1) 1 second after each scan to avoid
	 * FW issue with sleeping/waking up. */
	atomic_t			recent_scan;

	/* WSM */
	struct wsm_caps			wsm_caps;
	struct mutex			wsm_cmd_mux;
	struct wsm_buf			wsm_cmd_buf;
	struct wsm_cmd			wsm_cmd;
	wait_queue_head_t		wsm_cmd_wq;
	wait_queue_head_t		wsm_startup_done;
	struct wsm_cbc			wsm_cbc;
	atomic_t			tx_lock;
	u32				pending_frame_id;
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
	/* Device Power Range */
	struct wsm_tx_power_range       txPowerRange[2];
	/* Advance Scan */
	struct advance_scan_elems	advanceScanElems;
	bool				enable_advance_scan;
	struct atbm_delayed_work		advance_scan_timeout;
#endif /* CONFIG_ATBM_APOLLO_TESTMODE */

	/* WSM debug */
	int				wsm_enable_wsm_dumps;
	u32				wsm_dump_max_size;

	/* Scan status */
	struct atbm_scan scan;

	/* TX/RX */
	unsigned long		rx_timestamp;
	unsigned long       tx_timestamp;
	unsigned long       irq_timestamp;
	/* Scan Timestamp */
	unsigned long		scan_timestamp; 

	/* WSM events */
	spinlock_t		event_queue_lock;
	struct list_head	event_queue;
	struct atbm_work_struct	event_handler;
#ifndef CONFIG_RATE_HW_CONTROL
	/* TX rate policy cache */
	struct tx_policy_cache tx_policy_cache;
	struct atbm_work_struct tx_policy_upload_work;
#endif
	spinlock_t		tx_com_lock;
	spinlock_t		rx_com_lock;

	/* cryptographic engine information */

	/* bit field of glowing LEDs */
	u16 softled_state;

	/* statistics */
	struct ieee80211_low_level_stats stats;

	struct atbm_ht_info		ht_info;
	int				tx_burst_idx;

	struct ieee80211_iface_limit		if_limits1[2];
	struct ieee80211_iface_limit		if_limits2[2];
	struct ieee80211_iface_limit		if_limits3[2];
	struct ieee80211_iface_combination	if_combs[3];
	#ifndef OPER_CLOCK_USE_SEM
	struct mutex			wsm_oper_lock;
	#else
	struct semaphore		wsm_oper_lock;
	struct atbm_timer_list		wsm_pm_timer;
	spinlock_t				wsm_pm_spin_lock;
	atomic_t				wsm_pm_running;
	#endif
#ifdef CONFIG_ATBM_SUPPORT_P2P
	struct atbm_delayed_work		rem_chan_timeout;
	atomic_t			remain_on_channel;
	unsigned long roc_start_time;
	unsigned long roc_duration;
	int				roc_if_id;
	int				roc_not_send;
	u64				roc_cookie;
#endif
	int 			monitor_if_id;
	struct wsm_set_pm	roc_wlan_pm;
	wait_queue_head_t		offchannel_wq;
	u16				offchannel_done;
	u16				prev_channel;
	int       if_id_selected;
	u32				key_map;
	struct wsm_add_key		keys[WSM_KEY_MAX_INDEX + 1];
#ifdef MCAST_FWDING
	struct wsm_buf		wsm_release_buf[WSM_MAX_BUF];
	u8			buf_released;
#endif
#ifdef CONFIG_ATBM_SUPPORT_SCHED_SCAN
#ifdef ROAM_OFFLOAD
	u8				auto_scanning;
	u8				frame_rcvd;
	u8				num_scanchannels;
	u8				num_2g_channels;
	u8				num_5g_channels;
	struct wsm_scan_ch		scan_channels[WSM_SCAN_MAX_NUM_OF_CHANNELS];
	struct sk_buff 			*beacon;
	struct sk_buff 			*beacon_bkp;
#endif /*ROAM_OFFLOAD*/
#endif
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
	struct atbm_tsm_stats		tsm_stats;	
	struct atbm_tsm_stats		atbm_tsm_stats[4];
	struct atbm_tsm_info		tsm_info;
	spinlock_t			tsm_lock;
	struct atbm_start_stop_tsm	start_stop_tsm;
#endif /* CONFIG_ATBM_APOLLO_TESTMODE */
	u8          connected_sta_cnt;
	u16			vif0_throttle;
	u16			vif1_throttle;
#ifdef RESET_CHANGE
	atomic_t 	reset_flag;
	atomic_t	ap_restart;
	atomic_t	fw_not_ready;
	atomic_t	fw_reloading;
	atomic_t	reset_conter;
	atomic_t	reset_wake_up;
	spinlock_t			send_deauthen_lock;
	struct 		semaphore reset_lock;
#endif
#ifdef SPI_BUS
	struct atbm_timer_list spi_status_rx_ready_timer;
#endif
	//WAKELOCK for android
	spinlock_t wakelock_spinlock;
	u32 wakelock_hw_counter;
	u32 wakelock_bh_counter;
#if defined(CONFIG_HAS_WAKELOCK) && (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 27))
	struct wake_lock hw_wake;	/* Wifi hw wakelock */
	struct wake_lock bh_wake;   /* Wifi bh_wake wakelock */
#endif

//ETF
	u32 etf_channel;
	u32 etf_channel_type;
	u32 etf_rate;
	u32 etf_len;	
	u32 etf_greedfiled;
	u8 bStartTx;
	u8 bStartTxWantCancel;	
	u8 etf_test_v2;
	//struct atbm_timer_list etf_expire_timer;
	struct atbm_work_struct etf_tx_end_work;

	
	struct task_struct		*send_prbresp_work;
	wait_queue_head_t		send_prbresp_wq;
	bool start_send_prbresp,stop_prbresp_thread;
//	struct mutex stop_send_prbresp_lock;	
	struct atbm_vendor_cfg_ie private_ie;
	
#ifdef SDIO_BUS
#ifdef ATBM_SDIO_PATCH
	int dataFlag;
	atomic_t SyncChannl;
	struct list_head SeqBitMapList;
	spinlock_t SeqBitMapLock;
	u32 SdioSeq;
	atomic_t flushed;
#endif
	struct atbm_work_struct wsm_sync_channl;
	int syncChanl_done;
	wait_queue_head_t		wsm_synchanl_done;
	int is_sdio_hibernated;
#endif

#ifdef ATBM_P2P_CHANGE
/*
*only use when if_id = 1
*/
	u8 go_bssid[6];
	atomic_t go_bssid_set;
	atomic_t p2p_oper_channel;
	atomic_t receive_go_resp;
	atomic_t combination;
	atomic_t operating_channel_combination;
	unsigned long p2p_scan_start_time;
#endif
	struct efuse_headr efuse;
/*
*	ieee80211 rx tasklet schedule
*/
	u8 *xmit_buff;
	bool bh_running;
	u8 multrx_trace[64];
	u8 multrx_index;
#ifdef SDIO_BUS
	int sdio_status;
#endif
};

#ifdef ATBM_SDIO_PATCH
typedef enum{
	IS_CMD,
	IS_MGMT,
	IS_DATA
}TxTypeFlag;
#endif
#ifdef ATBM_SUPPORT_WIDTH_40M
typedef enum {
		notifyPeer20M,
		notifyPeer40M,
		notifyPeerMax
} notifyPeerAction_t;
#endif
#ifdef ATBM_PKG_REORDER
#define BUFF_STORED_LEN		64
#define ATBM_RX_DATA_QUEUES	8
#define REORDER_TIMER_RUNING	(0)
#define BAR_TID_EN				(0)
#define REORDER_TIMEOUT				(HZ/10)
#define SEQ_NUM_MASKER		(0xfff)
#define PER_PKG_TIME_INTERVAL	(10)

#define ATBM_BA__ACTION_RX_ADDBR		(1)
#define ATBM_BA__ACTION_RX_DELBA		(2)
#define ATBM_BA__ACTION_RX_BAR		(3)
struct atbm_ba_params
{
	u8 action;
	u8 link_id;
	u8 win_size;
	u8 tid;
	u16 ssn;
	u16 timeout;
};

struct atbm_ba_tid_params
{
	struct sk_buff *skb_reorder_buff[BUFF_STORED_LEN];
	struct sk_buff_head header;
	unsigned long frame_rx_time[BUFF_STORED_LEN];
	struct atbm_timer_list		  overtime_timer;
	spinlock_t	skb_reorder_spinlock;
	unsigned long timer_running;
	u16 ssn;
	u8  wind_size;
	u8  index_tail;
	unsigned long  tid_en;
	u8 skb_buffed;
	u16	start_seq;
	u16 timeout;
	void *reorder_priv;
};
struct atbm_reorder_queue_comm
{	
	struct atbm_ba_tid_params		atbm_rx_tid[ATBM_RX_DATA_QUEUES];
	u8			link_id;
	struct mutex	reorder_mutex;
	struct atbm_work_struct		  reorder_work;
};
void atbm_updata_ba_tid_params(struct atbm_vif *priv,struct atbm_ba_params *ba_params);
#endif
/* Virtual Interface State. One copy per VIF */
struct atbm_vif {
	atomic_t			enabled;
	spinlock_t			vif_lock;
	int				if_id;
	#ifndef ATBM_P2P_CHANGE
	u8 				scan_again;
	#endif
	/*TODO: Split into Common and VIF parts */
	#ifdef CONFIG_ATBM_APOLLO_DEBUG
	struct atbm_debug_priv	debug;
	#endif
	/* BBP/MAC state */
	u8 bssid[ETH_ALEN];
	struct wsm_edca_params		edca;
	struct wsm_tx_queue_params	tx_queue_params;
	struct wsm_association_mode	association_mode;
	struct wsm_set_bss_params	bss_params;
	struct wsm_set_pm		powersave_mode;
	struct wsm_set_pm		firmware_ps_mode;
	struct wsm_set_pm       saved_ps_mode;
	int				power_set_true;
	int				user_power_set_true;
	u8				user_pm_mode;
	bool			tmpframe_probereq_set;
	int				cqm_rssi_thold;
	unsigned			cqm_rssi_hyst;
	unsigned			cqm_tx_failure_thold;
	unsigned			cqm_tx_failure_count;
	bool				cqm_use_rssi;
	int				cqm_link_loss_count;
	int				cqm_beacon_loss_count;
	int				mode;
	bool				enable_beacon;
	int				beacon_int;
	int				ssid_length;
	u8				ssid[IEEE80211_MAX_SSID_LEN];
#ifdef HIDDEN_SSID
	bool				hidden_ssid;
#endif
	bool				listening;
	struct wsm_rx_filter		rx_filter;
	struct wsm_beacon_filter_table	bf_table;
	struct wsm_beacon_filter_control bf_control;
	struct wsm_multicast_filter	multicast_filter;
	bool				has_multicast_subscription;
	struct wsm_broadcast_addr_filter	broadcast_filter;
	bool				disable_beacon_filter;
	struct wsm_arp_ipv4_filter      filter4;
#ifdef IPV6_FILTERING
	struct wsm_ndp_ipv6_filter 	filter6;
#endif /*IPV6_FILTERING*/
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	struct atbm_work_struct		update_filtering_work;
#endif
	struct atbm_work_struct		set_beacon_wakeup_period_work;
#ifdef CONFIG_PM
	struct atbm_pm_state_vif	pm_state_vif;
#endif
	/*TODO: Add support in mac80211 for psmode info per VIF */
	struct wsm_p2p_ps_modeinfo	p2p_ps_modeinfo;
	struct wsm_uapsd_info		uapsd_info;
	bool				setbssparams_done;
	u32				listen_interval;
	u32				erp_info;
	bool				powersave_enabled;

	/* WSM Join */
	enum atbm_join_status	join_status;
	u8			join_bssid[ETH_ALEN];
	struct atbm_work_struct	join_work;
	struct atbm_delayed_work	join_timeout;
	struct atbm_work_struct	unjoin_work;
	
	/* ROC implementation */
#ifdef CONFIG_ATBM_SUPPORT_P2P
	struct atbm_work_struct	offchannel_work;
	struct atbm_delayed_work		pending_offchanneltx_work;
#endif
	int			join_dtim_period;
	bool			delayed_unjoin;

	/* Security */
	s8			wep_default_key_id;
	struct atbm_work_struct	wep_key_work;
        unsigned long           rx_timestamp;
        u32                     cipherType;


	/* AP powersave */
	u32			link_id_map;
	u32			max_sta_ap_mode;
	u32			link_id_after_dtim;
	u32			link_id_uapsd;
	u32			link_id_max;
	u32			wsm_key_max_idx;
	struct atbm_link_entry link_id_db[ATBMWIFI_MAX_STA_IN_AP_MODE];
	struct atbm_work_struct	link_id_work;
	struct atbm_delayed_work	link_id_gc_work;
	u32			sta_asleep_mask;
	u32			pspoll_mask;
	bool			aid0_bit_set;
	spinlock_t		ps_state_lock;
	bool			buffered_multicasts;
	bool			tx_multicast;
	struct atbm_work_struct	set_tim_work;
//	struct atbm_delayed_work	set_cts_work;
	struct atbm_work_struct	multicast_start_work;
	struct atbm_work_struct	multicast_stop_work;
	struct atbm_timer_list	mcast_timeout;
	//dhcp
	struct atbm_delayed_work	dhcp_retry_work;
	struct sk_buff *dhcp_retry_skb;
	spinlock_t		dhcp_retry_spinlock;
#ifndef CONFIG_TX_NO_CONFIRM
	/* CQM Implementation */
	struct atbm_delayed_work	bss_loss_work;
	struct atbm_delayed_work	connection_loss_work;
	struct atbm_work_struct	tx_failure_work;
	int			delayed_link_loss;
	spinlock_t		bss_loss_lock;
	int			bss_loss_status;
	int			bss_loss_confirm_id;
#endif
	struct ieee80211_vif	*vif;
	struct atbm_common	*hw_priv;
	struct ieee80211_hw	*hw;

	/* Workaround for WFD testcase 6.1.10*/
	struct atbm_work_struct	linkid_reset_work;
	u8			action_frame_sa[ETH_ALEN];
	u8			action_linkid;
	bool			htcap;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,3,0))
    u16                     ht_info;
    struct atbm_work_struct      ht_info_update_work;
#endif
#ifdef ATBM_PKG_REORDER
	struct atbm_reorder_queue_comm atbm_reorder_link_id[ATBM_APOLLO_LINK_ID_UNMAPPED];
#endif

#ifdef ATBM_SUPPORT_WIDTH_40M	
#ifdef CONFIG_ATBM_40M_AUTO_CCA
     struct atbm_delayed_work		  chantype_change_work;
#endif
#endif
#ifdef ATBM_SUPPORT_SMARTCONFIG
	u8 scan_expire;
	u8 scan_no_connect_back;
	u8 scan_no_connect;
	u8 auto_connect_when_lost;
#endif

#ifdef	ATBM_WIFI_QUEUE_LOCK_BUG
	u16 queue_cap;
#endif
};
struct atbm_sta_priv {
	int link_id;
	struct atbm_vif *priv;
};
enum atbm_data_filterid {
	IPV4ADDR_FILTER_ID = 0,
#ifdef IPV6_FILTERING
	IPV6ADDR_FILTER_ID,
#endif /*IPV6_FILTERING*/
};

static inline
struct atbm_common *ABwifi_vifpriv_to_hwpriv(struct atbm_vif *priv)
{
	return priv->hw_priv;
}


static inline
struct atbm_vif *ABwifi_get_vif_from_ieee80211(struct ieee80211_vif *vif)
{
	return  vif ? (struct atbm_vif *)vif->drv_priv:NULL;
}
static inline void atbm_hw_vif_read_lock(spinlock_t *lock)
{
	#ifndef  ATBM_VIF_LIST_USE_RCU_LOCK
	spin_lock_bh(lock);
	#else
//	BUG_ON(!lock);
	if(!lock){
		atbm_printk_err("%s %d ,ERROR !!! lock is NULL\n",__func__,__LINE__);
			
	}else{
		rcu_read_lock();
	}
	#endif
}
static inline void atbm_hw_vif_read_unlock(spinlock_t *lock)
{
	#ifndef  ATBM_VIF_LIST_USE_RCU_LOCK
	spin_unlock_bh(lock);
	#else
//	BUG_ON(!lock);
	if(!lock){
		atbm_printk_err("%s %d ,ERROR !!! unlock is NULL\n",__func__,__LINE__);
			
	}else{
		rcu_read_unlock();
	}
	
	#endif
}

static inline void atbm_hw_vif_write_lock(spinlock_t *lock)
{
	spin_lock_bh(lock);
}
static inline void atbm_hw_vif_write_unlock(spinlock_t *lock)
{
	spin_unlock_bh(lock);
}

static inline void atbm_priv_vif_list_read_lock(spinlock_t *lock)
{
	#ifndef  ATBM_VIF_LIST_USE_RCU_LOCK
	spin_lock_bh(lock);
	#else
	//BUG_ON(!lock);
	//rcu_read_lock();
	if(!lock){
		atbm_printk_err("%s %d ,ERROR !!! lock is NULL\n",__func__,__LINE__);
			
	}else{
		rcu_read_lock();
	}
	#endif
}

static inline void atbm_priv_vif_list_read_unlock(spinlock_t *lock)
{
	#ifndef  ATBM_VIF_LIST_USE_RCU_LOCK
	spin_unlock_bh(lock);
	#else
//	BUG_ON(!lock);
//	rcu_read_unlock();
	if(!lock){
		atbm_printk_err("%s %d ,ERROR !!! unlock is NULL\n",__func__,__LINE__);
			
	}else{
		rcu_read_unlock();
	}
	#endif
}

static inline void atbm_priv_vif_list_write_lock(spinlock_t *lock)
{
	#ifndef  ATBM_VIF_LIST_USE_RCU_LOCK
	spin_lock_bh(lock);
	#else
	//BUG_ON(!lock);
	if(!lock){
		atbm_printk_err("%s %d ,ERROR !!! lock is NULL\n",__func__,__LINE__);
			
	}
	#endif
}

static inline void atbm_priv_vif_list_write_unlock(spinlock_t *lock)
{
	#ifndef  ATBM_VIF_LIST_USE_RCU_LOCK
	spin_unlock_bh(lock);
	#else
//	BUG_ON(!lock);
	if(!lock){
			atbm_printk_err("%s %d ,ERROR !!! unlock is NULL\n",__func__,__LINE__);
				
	}

	#endif
}

#ifndef  ATBM_VIF_LIST_USE_RCU_LOCK

#define ATBM_HW_VIF_SET(vif_list,vif)			vif_list = vif
#define ATBM_HW_VIF_GET(vif_list)				(vif_list)	

static inline
struct atbm_vif *ABwifi_hwpriv_to_vifpriv(struct atbm_common *hw_priv,
						int if_id)
{
	struct atbm_vif *vif;

	if (((-1 == if_id) || (if_id > ATBM_WIFI_MAX_VIFS))){
		atbm_printk_err("if_id = %d\n", if_id);
		return NULL;
	}
	/* TODO:COMBO: During scanning frames can be received
	 * on interface ID 3 */
	atbm_hw_vif_read_lock(&hw_priv->vif_list_lock);
	if (!ATBM_HW_VIF_GET(hw_priv->vif_list[if_id])) {
		atbm_hw_vif_read_unlock(&hw_priv->vif_list_lock);
		return NULL;
	}

	vif = ABwifi_get_vif_from_ieee80211(ATBM_HW_VIF_GET(hw_priv->vif_list[if_id]));
	WARN_ON(!vif);
	if (vif)
		atbm_priv_vif_list_read_lock(&vif->vif_lock);
	atbm_hw_vif_read_unlock(&hw_priv->vif_list_lock);
	return vif;
}
#else			

#define ATBM_HW_VIF_SET(vif_list,vif)			rcu_assign_pointer(vif_list,vif)	
#define ATBM_HW_VIF_GET(vif_list)				rcu_dereference(vif_list)


static inline
struct atbm_vif *ABwifi_hwpriv_to_vifpriv(struct atbm_common *hw_priv,
						int if_id)
{
	struct atbm_vif *priv;
	struct ieee80211_vif *vif;
	
	atbm_hw_vif_read_lock(&hw_priv->vif_list_lock);
	if (((-1 == if_id) || (if_id > ATBM_WIFI_MAX_VIFS))){
		atbm_printk_err("if_id = %d\n", if_id);
		atbm_hw_vif_read_unlock(&hw_priv->vif_list_lock);
		return NULL;
	}

	vif = ATBM_HW_VIF_GET(hw_priv->vif_list[if_id]);
	if(!vif){
		atbm_hw_vif_read_unlock(&hw_priv->vif_list_lock);
		return NULL;
	}

	priv = ABwifi_get_vif_from_ieee80211(vif);
	WARN_ON(!priv);
	if(!priv)
		atbm_hw_vif_read_unlock(&hw_priv->vif_list_lock);
	return priv;
}
#endif
static inline
struct atbm_vif *__ABwifi_hwpriv_to_vifpriv(struct atbm_common *hw_priv,
					      int if_id)
{
	WARN_ON((-1 == if_id) || (if_id > ATBM_WIFI_MAX_VIFS));
	/* TODO:COMBO: During scanning frames can be received
	 * on interface ID 3 */
	if (!hw_priv->vif_list[if_id]) {
		return NULL;
	}

	return ABwifi_get_vif_from_ieee80211(hw_priv->vif_list[if_id]);
}

static inline
struct atbm_vif *ABwifi_get_activevif(struct atbm_common *hw_priv)
{
	return ABwifi_hwpriv_to_vifpriv(hw_priv, ffs(hw_priv->if_id_slot)-1);
}



static inline bool is_hardware_APOLLO(struct atbm_common *hw_priv)
{
	return (hw_priv->hw_revision == ATBM_APOLLO_REV_1601);
}

static inline int ABwifi_get_nr_hw_ifaces(struct atbm_common *hw_priv)
{
	return 1;
}

#ifdef IPV6_FILTERING
/* IPV6 host addr info */
struct ipv6_addr_info {
	u8 filter_mode;
	u8 address_mode;
	u16 ipv6[8];
};
#endif /*IPV6_FILTERING*/
#ifndef USB_BUS
extern int atbm_wtd_term(struct atbm_common *hw_priv);
#endif

/* interfaces for the drivers */
int atbm_core_probe(const struct sbus_ops *sbus_ops,
		      struct sbus_priv *sbus,
		      struct device *pdev,
		      struct atbm_common **pself);
void atbm_core_release(struct atbm_common *self);
#ifdef ATBM_WIFI_QUEUE_LOCK_BUG
static inline void atbm_stop_active_vif_queues(struct ieee80211_vif *vif)
{
	struct atbm_vif *priv = ABwifi_get_vif_from_ieee80211(vif);
	int i = 0;
	struct atbm_common *hw_priv = priv->hw_priv;
	//BUG_ON(hw_priv == NULL);
	if(hw_priv == NULL){
		atbm_printk_err("%s %d ,ERROR !!! hw_priv is NULL\n",__func__,__LINE__);
		return;
	}
	for (i = 0; i < 4; ++i){
		atbm_queue_lock(&hw_priv->tx_queue[i],priv->if_id);
	}
} 

static inline void atbm_wake_active_vif_queues(struct ieee80211_vif *vif)
{
	struct atbm_vif *priv = ABwifi_get_vif_from_ieee80211(vif);
	int i = 0;
	struct atbm_common *hw_priv = priv->hw_priv;
	//BUG_ON(hw_priv == NULL);
	if(hw_priv == NULL){
		atbm_printk_err("%s %d ,ERROR !!! hw_priv is NULL\n",__func__,__LINE__);
		return;
	}
	for (i = 0; i < 4; ++i)
		atbm_queue_unlock(&hw_priv->tx_queue[i],priv->if_id);
}

static inline void atbm_process_active_vif_queues(void *data, u8 *mac,struct ieee80211_vif *vif)
{
	int lock = (*(int *)data);

	if(lock)
		atbm_stop_active_vif_queues(vif);
	else
		atbm_wake_active_vif_queues(vif);
}
#endif
static inline void atbm_tx_queues_lock(struct atbm_common *hw_priv)
{
	#ifndef ATBM_WIFI_QUEUE_LOCK_BUG
	int i;
	for (i = 0; i < 4; ++i)
		atbm_queue_lock(&hw_priv->tx_queue[i]);
	#else
	#if 0
	int i = 1;
	ieee80211_iterate_active_interfaces_atomic(hw_priv->hw,atbm_process_active_vif_queues,(void*)(&i));
	#else
	int if_id = 0;
	int i = 0;

	for(if_id = 0;if_id<ATBM_WIFI_MAX_VIFS;if_id++){
		for (i = 0; i < 4; ++i){
			atbm_queue_lock(&hw_priv->tx_queue[i],if_id);
		}
	}
	#endif
	#endif
}

static inline void atbm_tx_queues_unlock(struct atbm_common *hw_priv)
{
	#ifndef ATBM_WIFI_QUEUE_LOCK_BUG
	int i;
	for (i = 0; i < 4; ++i)
		atbm_queue_unlock(&hw_priv->tx_queue[i]);
	#else
	#if 0
	int i = 0;
	ieee80211_iterate_active_interfaces_atomic(hw_priv->hw,atbm_process_active_vif_queues,(void*)(&i));
	#else
	int if_id = 0;
	int i = 0;
	for(if_id = 0;if_id<ATBM_WIFI_MAX_VIFS;if_id++){
		for (i = 0; i < 4; ++i){
			atbm_queue_unlock(&hw_priv->tx_queue[i],if_id);
		}
	}
	#endif
	#endif
}

/* Datastructure for LLC-SNAP HDR */
#define P80211_OUI_LEN  3

struct ieee80211_snap_hdr {
        u8    dsap;   /* always 0xAA */
        u8    ssap;   /* always 0xAA */
        u8    ctrl;   /* always 0x03 */
        u8    oui[P80211_OUI_LEN];    /* organizational universal id */
} __packed;



#define atbm_for_each_vif(_hw_priv, _priv, _i)			\
for (									\
	_i = 0;								\
	_priv = _hw_priv->vif_list[_i]? 				\
	ABwifi_get_vif_from_ieee80211(_hw_priv->vif_list[_i]) : NULL,	\
	_i < ATBM_WIFI_MAX_VIFS;						\
	_i++								\
)

#define atbm_for_each_vif_safe(_hw_priv, _priv, _i)			\
for (									\
	_i = 0;								\
	_priv = ABwifi_get_vif_from_ieee80211(ATBM_HW_VIF_GET(_hw_priv->vif_list[_i])),	\
	_i < ATBM_WIFI_MAX_VIFS;						\
	_i++								\
)
#define ATBM_VOLATILE_SET(s,v)	*((volatile typeof(*(s)) *)(s)) = (v);smp_mb()
#define ATBM_VOLATILE_GET(s)	(*((volatile typeof(*(s)) *)(s)))

#define atbm_hw_priv_assign_pointer(p)	ATBM_VOLATILE_SET(&g_hw_priv,p)
#define atbm_hw_priv_dereference()		ATBM_VOLATILE_GET(&g_hw_priv)
#ifdef ATBM_SUPPORT_WIDTH_40M
#ifdef CONFIG_ATBM_40M_AUTO_CCA
void atbm_channel_type_change_work(struct atbm_work_struct *work);
#endif
#ifdef CONFIG_ATBM_SUPPORT_P2P
void atbm_clear_wpas_p2p_40M_ie(struct atbm_ieee80211_mgmt *mgmt,u32 pkg_len);
#endif
#endif
#ifdef ATBM_PKG_REORDER
#define skb_move_to(src,des,tskb)		\
			while((tskb = __atbm_skb_dequeue(src)))	\
				__atbm_skb_queue_tail(des, tskb)
int atbm_reorder_skb_queue(struct atbm_vif *priv,struct sk_buff *skb,u8 link_id);
void atbm_reorder_func_init(struct atbm_vif *priv);
void atbm_reorder_func_reset(struct atbm_vif *priv,u8 link_id);
#endif
#include  "debug.h"
#ifdef CONFIG_ATBM_APOLLO_USE_GPIO_IRQ
void atbm_oob_intr_set(struct sbus_priv *self, bool enable);
#endif

#ifdef	ATBM_WIFI_QUEUE_LOCK_BUG
extern void atbm_set_priv_queue_cap(struct atbm_vif *priv);
extern void atbm_clear_priv_queue_cap(struct atbm_vif *priv);
#endif

extern void atbm_xmit_linearize(struct atbm_common	*hw_priv,
	 struct wsm_tx *wsm,char *xmit,int xmit_len);
extern int atbm_save_efuse(struct atbm_common *hw_priv,struct efuse_headr *efuse_save);
extern void atbm_destroy_wsm_cmd(struct atbm_common *hw_priv);
extern int atbm_reinit_firmware(struct atbm_common *hw_priv);
extern int atbm_rx_bh_flush(struct atbm_common *hw_priv);
extern void atbm_bh_halt(struct atbm_common *hw_priv);
extern void atbm_hif_status_set(int status);
extern void  atbm_bh_multrx_trace(struct atbm_common *hw_priv,u8 n_rx);
void atbm_get_drv_version(short *p);


#define CAPABILITIES_ATBM_PRIVATE_IE 		BIT(1)
#define CAPABILITIES_NVR_IPC 				BIT(2)
#define CAPABILITIES_NO_CONFIRM 			BIT(3)
#define CAPABILITIES_SDIO_PATCH 			BIT(4)
#define CAPABILITIES_NO_BACKOFF 			BIT(5)
#define CAPABILITIES_CFO 					BIT(6)
#define CAPABILITIES_AGC 					BIT(7)
#define CAPABILITIES_TXCAL 					BIT(8)
#define CAPABILITIES_MONITOR 				BIT(9)
#define CAPABILITIES_CUSTOM 				BIT(10)
#define CAPABILITIES_SMARTCONFIG			BIT(11)
#define CAPABILITIES_ETF					BIT(12)
#define CAPABILITIES_LMAC_RATECTL			BIT(13)
#define CAPABILITIES_LMAC_TPC				BIT(14)
#define CAPABILITIES_LMAC_TEMPC				BIT(15)
#define CAPABILITIES_CTS_BUG				BIT(16)
#define CAPABILITIES_USB_RECOVERY_BUG		BIT(17)
#define CAPABILITIES_VIFADDR_LOCAL_BIT		BIT(18)
#define CAPABILITIES_USE_IPC 				BIT(19)
#define CAPABILITIES_OUTER_PA 				BIT(20)
#define CAPABILITIES_POWER_CONSUMPTION 		BIT(21)
#define CAPABILITIES_RSSI_DECIDE_TXPOWER 	BIT(22)
#define CAPABILITIES_RTS_LONG_DURATION 		BIT(23)
#define CAPABILITIES_TX_CFO_PPM_CORRECTION 	BIT(24)
#define CAPABILITIES_NOISE_SET_DCXO 		BIT(25)
#define CAPABILITIES_HW_CHECKSUM  			BIT(26)
#define CAPABILITIES_SINGLE_CHANNEL_MULTI_RX BIT(27)
#define CAPABILITIES_CFO_DCXO_CORRECTION BIT(28)
#define CAPABILITIES_EFUSE8 BIT(29)
#define CAPABILITIES_EFUSEI BIT(30)
#define CAPABILITIES_EFUSEB BIT(31)

#define EX_CAPABILITIES_TWO_CHIP_ONE_SOC			BIT(0)
#define EX_CAPABILITIES_MANUAL_SET_AC				BIT(1)
#define EX_CAPABILITIES_LMAC_BW_CONTROL				BIT(2)
#define EX_CAPABILITIES_SUPPORT_TWO_ANTENNA			BIT(3)
#define EX_CAPABILITIES_ENABLE_STA_REMAIN_ON_CHANNEL BIT(4)
#define EX_CAPABILITIES_ENABLE_PS					BIT(5)
#define EX_CAPABILITIES_TX_REQUEST_FIFO_LINK		BIT(6)
#define EX_CAPABILITIES_CHIP_TYPE					(BIT(7)|BIT(8)|BIT(9))

#endif /* ATBM_APOLLO_H */
