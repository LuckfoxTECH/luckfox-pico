/*
 * Copyright 2002-2005, Instant802 Networks, Inc.
 * Copyright 2005, Devicescape Software, Inc.
 * Copyright 2006-2007	Jiri Benc <jbenc@suse.cz>
 * Copyright 2007-2010	Johannes Berg <johannes@sipsolutions.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef IEEE80211_I_H
#define IEEE80211_I_H

#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/if_ether.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/hash.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/workqueue.h>
#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/etherdevice.h>
#include <linux/leds.h>
#include <net/ieee80211_radiotap.h>
#include <net/cfg80211.h>
#include <net/atbm_mac80211.h>
#include "key.h"
#include "sta_info.h"
#include "ieee80211_atbm_mem.h"
#include "ieee80211_atbm_skb.h"
#include "atbm_workqueue.h"

#ifdef SIGMSTAR_SCAN_FEATURE
#define CHANNEL_NUM 14
#endif  //SIGMSTAR_SCAN_FEATURE
#define IEEE80211_KEEP_WAKEUP_TP_PER_SECOND		(1000*1024)

enum ATBM_TEST_TYPE{
	TXRX_TEST_REQ,
	TXRX_TEST_RSP,
	TXRX_TEST_RESULT,
};

enum ATBM_TEST_RESULT{
	TXRX_TEST_NONE,
	TXRX_TEST_PASS,
	TXRX_TEST_FAIL,
};


struct ATBM_TEST_IE {
	u8 ie_id;//D11_WIFI_ELT_ID
	u8 len;
	u8 oui[3]; //ATBM_OUI
	u8 oui_type; //WIFI_ATBM_IE_OUI_TYPE
	u8 test_type;
	u8 resverd;	
#ifdef ATBM_PRODUCT_TEST_USE_FEATURE_ID
	u32 featureid;
#endif
	s32 result[16];
}__packed;

struct etf_test_config{
	int freq_ppm;
	int txevm;//txevm filter
	int rxevm;//send to lmac,rxevm filter
	int txevmthreshold;//test threshold
	int rxevmthreshold;//test threshold
	int txpwrmax;
	int txpwrmin;
	int rxpwrmax;
	int rxpwrmin;
	u32 featureid;
	int rssifilter;
	int cableloss;
	int default_dcxo;
	u8  noFfreqCaliFalg;//no freqOffset Calibration flag,
	u8  chip_crystal_type;//2:share crystal chip
	u8  dcxo_code_max;//config_param.txt  config dcxo max value
	u8	dcxo_code_min;//config_param.txt  config dcxo min value
	u8 	writemacflag;
	u8  writemac[6];
};

struct rxstatus_signed{
	u8 valid;
	s32 GainImb;
	s32 PhaseImb;
	s32 Cfo;
	s32 txevm;
	s32 evm;
	s32  RxRSSI;
	s32  TxRSSI;
	u16 dcxo;
	s32 result;
};

struct Tjroom_temperature_t{
	u32 tempC;
	s32 stempC;
	u32 Tjroom;
	u32 reserverd;
};


#define IEEE80211_MEDIUM_TRAFFIC_PRIOD	1000
/* OUI's */
#define WIFI_OUI			{0x00, 0x50, 0xF2}
#define WFA_OUI			 {0x50, 0x6F, 0x9A}
#define ATBM_OUI			{0x00, 0xAA, 0xBB}
#define WIFI_WPA_OUI_TYPE	       0x01
#define WIFI_WME_OUI_TYPE	       0x02
#define WIFI_WPS_OUI_TYPE	       0x04
#define WIFI_P2P_IE_OUI_TYPE	    0x09
#define WIFI_ATBM_IE_OUI_TYPE	    0x0a

#define D11_WIFI_ELT_ID		0xDD 

#define wdev_to_ndev(w) ((w)->netdev)
struct ieee80211_local;

/* Maximum number of broadcast/multicast frames to buffer when some of the
 * associated stations are using power saving. */
#define AP_MAX_BC_BUFFER 128

/* Maximum number of frames buffered to all STAs, including multicast frames.
 * Note: increasing this limit increases the potential memory requirement. Each
 * frame can be up to about 2 kB long. */
#define TOTAL_MAX_TX_BUFFER 512

/* Required encryption head and tailroom */
#define IEEE80211_ENCRYPT_HEADROOM 20
#define IEEE80211_ENCRYPT_TAILROOM 18

/* IEEE 802.11 (Ch. 9.5 Defragmentation) requires support for concurrent
 * reception of at least three fragmented frames. This limit can be increased
 * by changing this define, at the cost of slower frame reassembly and
 * increased memory use (about 2 kB of RAM per entry). */
#define IEEE80211_FRAGMENT_MAX 4
#ifndef TU_TO_EXP_TIME
#define TU_TO_EXP_TIME(x)	(jiffies + usecs_to_jiffies((x) * 1024))
#endif

#define IEEE80211_DEFAULT_UAPSD_QUEUES \
	(IEEE80211_WMM_IE_STA_QOSINFO_AC_BK |	\
	 IEEE80211_WMM_IE_STA_QOSINFO_AC_BE |	\
	 IEEE80211_WMM_IE_STA_QOSINFO_AC_VI |	\
	 IEEE80211_WMM_IE_STA_QOSINFO_AC_VO)

#define IEEE80211_DEFAULT_MAX_SP_LEN		\
	IEEE80211_WMM_IE_STA_QOSINFO_SP_ALL
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 7, 2))
/**
 * struct ieee80211_ht_info - HT information
 *
 * This structure is the "HT information element" as
 * described in 802.11n D5.0 7.3.2.58
 */
struct ieee80211_ht_info {
	u8 control_chan;
	u8 ht_param;
	__le16 operation_mode;
	__le16 stbc_param;
	u8 basic_set[16];
} __attribute__ ((packed));
#endif
struct ieee80211_fragment_entry {
	unsigned long first_frag_time;
	unsigned int seq;
	unsigned int rx_queue;
	unsigned int last_frag;
	unsigned int extra_len;
	struct sk_buff_head skb_list;
	int ccmp; /* Whether fragments were encrypted with CCMP */
	u8 last_pn[6]; /* PN of the last fragment if CCMP was used */
	struct ieee80211_key *first_key;
};

/* Parsed Information Elements */
struct ieee802_atbm_11_elems {
	u8 *ie_start;
	size_t total_len;

	/* pointers to IEs */
	u8 *ssid;
	u8 *supp_rates;
	u8 *fh_params;
	u8 *ds_params;
	u8 *cf_params;
	struct ieee80211_tim_ie *tim;
	u8 *ibss_params;
	u8 *challenge;
	u8 *wpa;
	u8 *rsn;
	u8 encry_info;
	u8 *erp_info;
	u8 *ext_supp_rates;
	u8 *wmm_info;
	u8 *wmm_param;
	u8 *p2p_ie;
	const u8 *wfd;
	const u8 *hs20;
	struct ieee80211_ht_cap *ht_cap_elem;
	struct ieee80211_ht_info *ht_info_elem;
	struct ieee80211_meshconf_ie *mesh_config;
	u8 *mesh_id;
	u8 *peering;
	u8 *preq;
	u8 *prep;
	u8 *perr;
	struct ieee80211_rann_ie *rann;
	u8 *ch_switch_elem;
	u8 *country_elem;
	u8 *pwr_constr_elem;
	u8 *quiet_elem;	/* first quite element */
	u8 *timeout_int;
	const u8 *wps_ie;
	u8 *extended_ch_switch_elem;
	u8 * secondary_ch_elem;
	u8 *atbm_special;
	/* length of them, respectively */
	u8 ssid_len;
	u8 supp_rates_len;
	u8 fh_params_len;
	u8 ds_params_len;
	u8 cf_params_len;
	u8 tim_len;
	u8 ibss_params_len;
	u8 challenge_len;
	u8 wpa_len;
	u8 rsn_len;
	u8 erp_info_len;
	u8 ext_supp_rates_len;
	u8 wmm_info_len;
	u8 wmm_param_len;
	u8 p2p_ie_len;
	u8 mesh_id_len;
	u8 peering_len;
	u8 preq_len;
	u8 prep_len;
	u8 perr_len;
	u8 ch_switch_elem_len;
	u8 country_elem_len;
	u8 pwr_constr_elem_len;
	u8 quiet_elem_len;
	u8 num_of_quiet_elem;	/* can be more the one */
	u8 timeout_int_len;
	u8 wfd_len;
	u8 wps_ie_len;	
	u8 extended_ch_switch_elem_len;
	u8 secondary_ch_elem_len;
	u8 atbm_special_len;
};
struct ieee80211_bss {
	/* don't want to look up all the time */
#ifdef SIGMSTAR_SCAN_FEATURE
	s8 noise_floor;
#endif  //SIGMSTAR_SCAN_FEATURE
	size_t ssid_len;
	u8 ssid[IEEE80211_MAX_SSID_LEN];

	u8 dtim_period;

	bool wmm_used;
	bool uapsd_supported;

	unsigned long last_probe_resp;

#ifdef CONFIG_MAC80211_ATBM_MESH
	u8 *mesh_id;
	size_t mesh_id_len;
	u8 *mesh_cfg;
#endif

#define IEEE80211_MAX_SUPP_RATES 32
	u8 supp_rates[IEEE80211_MAX_SUPP_RATES];
	size_t supp_rates_len;

	/*
	 * During association, we save an ERP value from a probe response so
	 * that we can feed ERP info to the driver when handling the
	 * association completes. these fields probably won't be up-to-date
	 * otherwise, you probably don't want to use them.
	 */
	bool has_erp_value;
	u8 erp_value;
};

static inline u8 *bss_mesh_cfg(struct ieee80211_bss *bss)
{
#ifdef CONFIG_MAC80211_ATBM_MESH
	return bss->mesh_cfg;
#endif
	return NULL;
}

static inline u8 *bss_mesh_id(struct ieee80211_bss *bss)
{
#ifdef CONFIG_MAC80211_ATBM_MESH
	return bss->mesh_id;
#endif
	return NULL;
}

static inline u8 bss_mesh_id_len(struct ieee80211_bss *bss)
{
#ifdef CONFIG_MAC80211_ATBM_MESH
	return bss->mesh_id_len;
#endif
	return 0;
}


typedef unsigned __bitwise__ ieee80211_tx_result;
#define TX_CONTINUE	((__force ieee80211_tx_result) 0u)
#define TX_DROP		((__force ieee80211_tx_result) 1u)
#define TX_QUEUED	((__force ieee80211_tx_result) 2u)

#define IEEE80211_TX_UNICAST		BIT(1)
#define IEEE80211_TX_PS_BUFFERED	BIT(2)

struct ieee80211_tx_data {
	struct sk_buff *skb;
	struct ieee80211_local *local;
	struct ieee80211_sub_if_data *sdata;
	struct sta_info *sta;
	struct ieee80211_key *key;

	struct ieee80211_channel *channel;

	u16 ethertype;
	unsigned int flags;
};

/* TDLS action codes */
struct atbm_ieee802_1x_hdr {
	u8 version;
	u8 type;
	__le16 length;
	/* followed by length octets of data */
} __attribute__ ((packed));

struct atbm_wpa_eapol_key {
	u8 type;
	/* Note: key_info, key_length, and key_data_length are unaligned */
	u8 key_info[2]; /* big endian */
	u8 key_length[2]; /* big endian */
	u8 replay_counter[ATBM_WPA_REPLAY_COUNTER_LEN];
	u8 key_nonce[ATBM_WPA_NONCE_LEN];
	u8 key_iv[16];
	u8 key_rsc[ATBM_WPA_KEY_RSC_LEN];
	u8 key_id[8]; /* Reserved in IEEE 802.11i/RSN */
	/* variable length Key MIC field */
	/* big endian 2-octet Key Data Length field */
	/* followed by Key Data Length bytes of Key Data */
} __attribute__ ((packed));

typedef unsigned __bitwise__ ieee80211_rx_result;
#define RX_CONTINUE		((__force ieee80211_rx_result) 0u)
#define RX_DROP_UNUSABLE	((__force ieee80211_rx_result) 1u)
#define RX_DROP_MONITOR		((__force ieee80211_rx_result) 2u)
#define RX_QUEUED		((__force ieee80211_rx_result) 3u)

/**
 * enum ieee80211_packet_rx_flags - packet RX flags
 * @IEEE80211_RX_RA_MATCH: frame is destined to interface currently processed
 *	(incl. multicast frames)
 * @IEEE80211_RX_IN_SCAN: received while scanning
 * @IEEE80211_RX_FRAGMENTED: fragmented frame
 * @IEEE80211_RX_AMSDU: a-MSDU packet
 * @IEEE80211_RX_MALFORMED_ACTION_FRM: action frame is malformed
 * @IEEE80211_RX_DEFERRED_RELEASE: frame was subjected to receive reordering
 *
 * These are per-frame flags that are attached to a frame in the
 * @rx_flags field of &struct ieee80211_rx_status.
 */
enum ieee80211_packet_rx_flags {
	IEEE80211_RX_IN_SCAN			= BIT(0),
	IEEE80211_RX_RA_MATCH			= BIT(1),
	IEEE80211_RX_FRAGMENTED			= BIT(2),
	IEEE80211_RX_AMSDU			= BIT(3),
	IEEE80211_RX_MALFORMED_ACTION_FRM	= BIT(4),
	IEEE80211_RX_DEFERRED_RELEASE		= BIT(5),
	IEEE80211_RX_ERP_BEACON			= BIT(6),
	IEEE80211_RX_SPECIAL_PROBE_REQ  = BIT(7),
};

/**
 * enum ieee80211_rx_flags - RX data flags
 *
 * @IEEE80211_RX_CMNTR: received on cooked monitor already
 *
 * These flags are used across handling multiple interfaces
 * for a single frame.
 */
enum ieee80211_rx_flags {
	IEEE80211_RX_CMNTR		= BIT(0),
};

struct ieee80211_rx_data {
	struct sk_buff *skb;
	struct ieee80211_local *local;
	struct ieee80211_sub_if_data *sdata;
	struct sta_info *sta;
	struct ieee80211_key *key;

	unsigned int flags;

	/*
	 * Index into sequence numbers array, 0..16
	 * since the last (16) is used for non-QoS,
	 * will be 16 on non-QoS frames.
	 */
	int seqno_idx;

	/*
	 * Index into the security IV/PN arrays, 0..16
	 * since the last (16) is used for CCMP-encrypted
	 * management frames, will be set to 16 on mgmt
	 * frames and 0 on non-QoS frames.
	 */
	int security_idx;

	u32 tkip_iv32;
	u16 tkip_iv16;
};

struct beacon_data {
	u8 *head, *tail;
	int head_len, tail_len;
	int dtim_period;
	struct rcu_head rcu_head;
};

#ifdef ATBM_PROBE_RESP_EXTRA_IE
struct proberesp_data {
	u8 *head, *tail;
	int head_len, tail_len;
	u8 *proberesp_data_ies;
	int proberesp_data_ies_len;
};
#endif
#ifdef ATBM_AP_SME
struct associate_data{
	u8 *associate_ies;
	int associate_ies_len;
};
#endif
struct beacon_extra{
	u8    *beacon_extra_ie;
	size_t beacon_extra_len;
};
struct probe_response_extra{
	u8    *probe_response_extra_ie;
	size_t probe_response_extra_len;
};
struct probe_request_extra{
	u8    *probe_request_extra_ie;
	size_t probe_request_extra_len;
};
struct common_extra{
	u8     *extra;
	size_t extra_len;
};
union iee80211_extra_ie{
	struct beacon_extra 			beacon;
	struct probe_response_extra		proberesponse;
	struct probe_request_extra		proberequest;
	struct common_extra				extra;
};
struct ieee80211_if_ap {
	struct beacon_data __rcu *beacon;
#ifdef ATBM_PROBE_RESP_EXTRA_IE
	struct proberesp_data __rcu *proberesp;
#endif
#ifdef ATBM_AP_SME
	struct associate_data __rcu *associate_ie;
#endif
	struct beacon_extra __rcu *beacon_extra;
	struct probe_response_extra __rcu *probe_response_extra;
	struct list_head vlans;

	/* yes, this looks ugly, but guarantees that we can later use
	 * bitmap_empty :)
	 * NB: don't touch this bitmap, use sta_info_{set,clear}_tim_bit */
	u8 tim[sizeof(unsigned long) * BITS_TO_LONGS(IEEE80211_MAX_AID + 1)];
	struct sk_buff_head ps_bc_buf;
	atomic_t num_sta_ps; /* number of stations in PS mode */
	int dtim_count;
	bool dtim_bc_mc;
};

struct ieee80211_if_wds {
	struct sta_info *sta;
	u8 remote_addr[ETH_ALEN];
};

struct ieee80211_if_vlan {
	struct list_head list;

	/* used for all tx if the VLAN is configured to 4-addr mode */
	struct sta_info __rcu *sta;
};

struct mesh_stats {
	__u32 fwded_mcast;		/* Mesh forwarded multicast frames */
	__u32 fwded_unicast;		/* Mesh forwarded unicast frames */
	__u32 fwded_frames;		/* Mesh total forwarded frames */
	__u32 dropped_frames_ttl;	/* Not transmitted since mesh_ttl == 0*/
	__u32 dropped_frames_no_route;	/* Not transmitted, no route found */
	__u32 dropped_frames_congestion;/* Not forwarded due to congestion */
	atomic_t estab_plinks;
};

#define PREQ_Q_F_START		0x1
#define PREQ_Q_F_REFRESH	0x2
struct mesh_preq_queue {
	struct list_head list;
	u8 dst[ETH_ALEN];
	u8 flags;
};

#if HZ/100 == 0
#define IEEE80211_ROC_MIN_LEFT	1
#else
#define IEEE80211_ROC_MIN_LEFT	(HZ/100)
#endif

struct ieee80211_roc_work {
	struct list_head list;
	struct list_head dependents;

	struct atbm_delayed_work work;

	struct ieee80211_sub_if_data *sdata;

	struct ieee80211_channel *chan;

	bool started, abort, hw_begun, notified;

	unsigned long hw_start_time;
	unsigned long hw_extend_roc_time;

	u32 duration, req_duration;
	struct sk_buff *frame;
	u64 cookie, mgmt_tx_cookie;
	struct ieee80211_local *local;
	struct net_device *hw_roc_dev;
	enum nl80211_channel_type chan_type;
	unsigned long pending_start_time;
};
enum ieee80211_work_type {
	IEEE80211_WORK_ABORT,
	IEEE80211_WORK_DIRECT_PROBE,
	IEEE80211_WORK_AUTH,
	IEEE80211_WORK_ASSOC_BEACON_WAIT,
	IEEE80211_WORK_ASSOC,
	IEEE80211_WORK_REMAIN_ON_CHANNEL,
	IEEE80211_WORK_OFFCHANNEL_TX,
	IEEE80211_WORK_CONNECTTING,
	IEEE80211_WORK_DISCONNECTTING,
#ifdef CONFIG_ATBM_STA_DYNAMIC_PS
	IEEE80211_WORK_PS_RECAL,
#endif
};

/**
 * enum work_done_result - indicates what to do after work was done
 *
 * @WORK_DONE_DESTROY: This work item is no longer needed, destroy.
 * @WORK_DONE_REQUEUE: This work item was reset to be reused, and
 *	should be requeued.
 */
enum work_done_result {
	WORK_DONE_DESTROY,
	WORK_DONE_REQUEUE,
};

struct ieee80211_work {
	struct list_head list;

	struct rcu_head rcu_head;

	struct ieee80211_sub_if_data *sdata;

	enum work_done_result (*done)(struct ieee80211_work *wk,
				      struct sk_buff *skb);

	struct ieee80211_channel *chan;
	enum nl80211_channel_type chan_type;

	unsigned long timeout;
	enum ieee80211_work_type type;

	u8 filter_ta[ETH_ALEN];

	bool started;

	union {
		struct {
			int tries;
			u16 algorithm, transaction;
			u8 ssid[IEEE80211_MAX_SSID_LEN];
			u8 ssid_len;
			u8 key[WLAN_KEY_LEN_WEP104];
			u8 key_len, key_idx;
			bool privacy;
			bool synced;
#ifdef CONFIG_ATBM_SUPPORT_SAE
			u16 sae_trans, sae_status;
#endif
			struct cfg80211_bss *bss;
		} probe_auth;
		struct {
			struct cfg80211_bss *bss;
			const u8 *supp_rates;
			const u8 *ht_information_ie;
			enum ieee80211_smps_mode smps;
			int tries;
			u16 capability;
			u8 prev_bssid[ETH_ALEN];
			u8 ssid[IEEE80211_MAX_SSID_LEN];
			u8 ssid_len;
			u8 supp_rates_len;
			bool wmm_used, use_11n, uapsd_used;
			bool synced;
		} assoc;
		struct {
			u32 duration;
		} remain;
		struct {
			struct sk_buff *frame;
			u32 wait;
			bool status;
		} offchan_tx;
		struct {
			int tries;
			int retry_max;
			int scan_reties;
		}connecting;
		struct {
			int tries;
			int retry_max;
			u16 reason_code;
			bool mfp;
			bool send_frame;
		}disconnecting;
	};

	int ie_len;
	/* must be last */
	u8 ie[0];
};

/* flags used in struct ieee80211_if_managed.flags */
enum ieee80211_sta_flags {
	IEEE80211_STA_BEACON_POLL	= BIT(0),
	IEEE80211_STA_CONNECTION_POLL	= BIT(1),
	IEEE80211_STA_CONTROL_PORT	= BIT(2),
	IEEE80211_STA_DISABLE_11N	= BIT(4),
	IEEE80211_STA_CSA_RECEIVED	= BIT(5),
	IEEE80211_STA_MFP_ENABLED	= BIT(6),
	IEEE80211_STA_UAPSD_ENABLED	= BIT(7),
	IEEE80211_STA_NULLFUNC_ACKED	= BIT(8),
	IEEE80211_STA_RESET_SIGNAL_AVE	= BIT(9),
};

struct ieee80211_if_managed {
#ifdef CONFIG_ATBM_MAC80211_NO_USE		
	struct atbm_timer_list timer;
	struct atbm_timer_list conn_mon_timer;
	struct atbm_timer_list bcn_mon_timer;
	struct atbm_work_struct monitor_work;
#endif
#ifdef CONFIG_ATBM_SUPPORT_CHANSWITCH
	struct atbm_timer_list chswitch_timer;
//	struct timer_list scan_delay_timer;
	struct atbm_work_struct chswitch_work;
#endif
	struct atbm_work_struct beacon_connection_loss_work;

	unsigned long beacon_timeout;
	unsigned long probe_timeout;
	int probe_send_count;
	bool nullfunc_failed;

	struct mutex mtx;
	struct cfg80211_bss *associated;
	struct cfg80211_bss __rcu *authen_bss;
	struct probe_request_extra __rcu *probe_request_extra;
	
	u8 bssid[ETH_ALEN];

	u16 aid;

	unsigned long timers_running; /* used for quiesce/restart */
	bool powersave; /* powersave requested for this iface */
	bool broken_ap; /* AP is broken -- turn off powersave */

	enum ieee80211_smps_mode req_smps, /* requested smps mode */
				 ap_smps, /* smps mode AP thinks we're in */
				 driver_smps_mode; /* smps mode request */
#ifdef	CONFIG_ATBM_SMPS
	struct atbm_work_struct request_smps_work;
#endif
	unsigned int flags;

	bool beacon_crc_valid;
	u32 beacon_crc;

	enum {
		IEEE80211_MFP_DISABLED,
		IEEE80211_MFP_OPTIONAL,
		IEEE80211_MFP_REQUIRED
	} mfp; /* management frame protection */

	int wmm_last_param_set;

	char	p2p_last_ie[255];
	u8	p2p_last_ie_len;

	u8 use_4addr;

	/* Signal strength from the last Beacon frame in the current BSS. */
	int last_beacon_signal;

	/*
	 * Weighted average of the signal strength from Beacon frames in the
	 * current BSS. This is in units of 1/16 of the signal unit to maintain
	 * accuracy and to speed up calculations, i.e., the value need to be
	 * divided by 16 to get the actual value.
	 */
	int ave_beacon_signal;

	/*
	 * Number of Beacon frames used in ave_beacon_signal. This can be used
	 * to avoid generating less reliable cqm events that would be based
	 * only on couple of received frames.
	 */
	unsigned int count_beacon_signal;

	/*
	 * Last Beacon frame signal strength average (ave_beacon_signal / 16)
	 * that triggered a cqm event. 0 indicates that no event has been
	 * generated for the current association.
	 */
	int last_cqm_event_signal;

	/*
	 * State variables for keeping track of RSSI of the AP currently
	 * connected to and informing driver when RSSI has gone
	 * below/above a certain threshold.
	 */
	int rssi_min_thold, rssi_max_thold;
	int last_ave_beacon_signal;
#ifdef CONFIG_MAC80211_ATBM_ROAMING_CHANGES
	/*
	 * Set during reassociation to enable faster key removal.
	 */
	int roaming;
#endif
};

struct ieee80211_if_ibss {
	struct atbm_timer_list timer;

	struct mutex mtx;

	unsigned long last_scan_completed;

	u32 basic_rates;

	bool timer_running;

	bool fixed_bssid;
	bool fixed_channel;
	bool privacy;

	u8 bssid[ETH_ALEN];
	u8 ssid[IEEE80211_MAX_SSID_LEN];
	u8 ssid_len, ie_len;
	u8 *ie;
	struct ieee80211_channel *channel;

	unsigned long ibss_join_req;
	/* probe response/beacon for IBSS */
	struct sk_buff __rcu *presp;
	struct sk_buff *skb;

	enum {
		IEEE80211_IBSS_MLME_SEARCH,
		IEEE80211_IBSS_MLME_JOINED,
	} state;
};

struct ieee80211_if_mesh {
	struct atbm_timer_list housekeeping_timer;
	struct atbm_timer_list mesh_path_timer;
	struct atbm_timer_list mesh_path_root_timer;

	unsigned long timers_running;

	unsigned long wrkq_flags;

	u8 mesh_id[IEEE80211_MAX_MESH_ID_LEN];
	size_t mesh_id_len;
	/* Active Path Selection Protocol Identifier */
	u8 mesh_pp_id;
	/* Active Path Selection Metric Identifier */
	u8 mesh_pm_id;
	/* Congestion Control Mode Identifier */
	u8 mesh_cc_id;
	/* Synchronization Protocol Identifier */
	u8 mesh_sp_id;
	/* Authentication Protocol Identifier */
	u8 mesh_auth_id;
	/* Local mesh Sequence Number */
	u32 sn;
	/* Last used PREQ ID */
	u32 preq_id;
	atomic_t mpaths;
	/* Timestamp of last SN update */
	unsigned long last_sn_update;
	/* Timestamp of last SN sent */
	unsigned long last_preq;
	struct mesh_rmc *rmc;
	spinlock_t mesh_preq_queue_lock;
	struct mesh_preq_queue preq_queue;
	int preq_queue_len;
	struct mesh_stats mshstats;
	struct mesh_config mshcfg;
	u32 mesh_seqnum;
	bool accepting_plinks;
	int num_gates;
	const u8 *ie;
	u8 ie_len;
	enum {
		IEEE80211_MESH_SEC_NONE = 0x0,
		IEEE80211_MESH_SEC_AUTHED = 0x1,
		IEEE80211_MESH_SEC_SECURED = 0x2,
	} security;
};

#ifdef CONFIG_MAC80211_ATBM_MESH
#define IEEE80211_IFSTA_MESH_CTR_INC(msh, name)	\
	do { (msh)->mshstats.name++; } while (0)
#else
#define IEEE80211_IFSTA_MESH_CTR_INC(msh, name) \
	do { } while (0)
#endif

struct ieee80211_channel_state {
	struct ieee80211_channel_conf conf;

	/* Operational channel */
	enum nl80211_channel_type _oper_channel_type;
	struct ieee80211_channel *oper_channel;

	/* CSA channel .e.g for STA when AP switches channels */
	struct ieee80211_channel *csa_channel;

	/* Temporary remain-on-channel for off-channel operations */
	struct ieee80211_channel *tmp_channel;
	enum nl80211_channel_type tmp_channel_type;
};

/**
 * enum ieee80211_sub_if_data_flags - virtual interface flags
 *
 * @IEEE80211_SDATA_ALLMULTI: interface wants all multicast packets
 * @IEEE80211_SDATA_PROMISC: interface is promisc
 * @IEEE80211_SDATA_OPERATING_GMODE: operating in G-only mode
 * @IEEE80211_SDATA_DONT_BRIDGE_PACKETS: bridge packets between
 *	associated stations and deliver multicast frames both
 *	back to wireless media and to the local net stack.
 * @IEEE80211_SDATA_DISCONNECT_RESUME: Disconnect after resume.
 */
enum ieee80211_sub_if_data_flags {
	IEEE80211_SDATA_ALLMULTI		= BIT(0),
	IEEE80211_SDATA_PROMISC			= BIT(1),
	IEEE80211_SDATA_OPERATING_GMODE		= BIT(2),
	IEEE80211_SDATA_DONT_BRIDGE_PACKETS	= BIT(3),
	IEEE80211_SDATA_DISCONNECT_RESUME	= BIT(4),
};

/**
 * enum ieee80211_sdata_state_bits - virtual interface state bits
 * @SDATA_STATE_RUNNING: virtual interface is up & running; this
 *	mirrors netif_running() but is separate for interface type
 *	change handling while the interface is up
 * @SDATA_STATE_OFFCHANNEL: This interface is currently in offchannel
 *	mode, so queues are stopped
 */
enum ieee80211_sdata_state_bits {
	SDATA_STATE_RUNNING,
	SDATA_STATE_OFFCHANNEL,
};
#ifdef ATBM_AP_SME
enum ap_sme_event_id{
	STA_EVENT__ASSOCED,
	STA_EVENT__DEAUTHEN,
	STA_EVENT__FREE,
	STA_EVENT__SEND_DEAUTHEN,
	STA_EVENT__SEND_DEASSOC,
	STA_EVENT__MAX,
};

struct ap_sme_event{
	struct list_head list;
	enum ap_sme_event_id event;
	u8 sta_addr[ETH_ALEN];
};
#endif

#ifdef CONFIG_MAC80211_BRIDGE
#include "mac80211_bridge.h"
#endif	// CONFIG_MAC80211_BRIDGE

#define IEEE80211_ATBM_CONNECT_DONE (0)
#define IEEE80211_ATBM_CONNECT_SET  (1)
#define IEEE80211_ATBM_CONNECT_RUN	(2)

struct ieee80211_internal_ap_conf{
	u8 bssid[ETH_ALEN];
	u8 ssid[IEEE80211_MAX_SSID_LEN];
	u8 ssid_len;
	u8 channel;
	/*others password or enc type*/
};
struct ieee80211_medium_traffic{
	struct atbm_timer_list		traffic_timer;
	unsigned long				last_rx_bytes;
	unsigned long				last_tx_bytes;
	unsigned long				current_tx_tp;
	unsigned long				current_rx_tp;
};
struct ieee80211_sub_if_data {
	struct list_head list;

	struct wireless_dev wdev;

	/* keys */
	struct list_head key_list;

	/* count for keys needing tailroom space allocation */
	int crypto_tx_tailroom_needed_cnt;

	struct net_device *dev;
	struct ieee80211_local *local;

	unsigned int flags;

	unsigned long state;

	int drop_unencrypted;
#ifdef CONFIG_MAC80211_ATBM_ROAMING_CHANGES
	int queues_locked;
#endif

	char name[IFNAMSIZ];

	/*
	 * keep track of whether the HT opmode (stored in
	 * vif.bss_info.ht_operation_mode) is valid.
	 */
	bool ht_opmode_valid;

	/* to detect idle changes */
	bool old_idle;

	bool pspolling;
	bool offchannel_ps_enabled;
	bool ps_allowed;
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	struct atbm_work_struct dynamic_ps_enable_work;
	struct atbm_work_struct dynamic_ps_disable_work;
	struct atbm_timer_list dynamic_ps_timer;
#endif
	struct ieee80211_medium_traffic traffic;
	/*
	 * The dynamic ps timeout configured from user space via WEXT -
	 * this will override whatever chosen by mac80211 internally.
	 */
	int dynamic_ps_forced_timeout;
	int dynamic_ps_user_timeout;
	bool disable_dynamic_ps;

	/* Fragment table for host-based reassembly */
	struct ieee80211_fragment_entry	fragments[IEEE80211_FRAGMENT_MAX];
	unsigned int fragment_next;

	struct ieee80211_key __rcu *keys[NUM_DEFAULT_KEYS + NUM_DEFAULT_MGMT_KEYS];
	struct ieee80211_key __rcu *default_unicast_key;
	struct ieee80211_key __rcu *default_multicast_key;
	struct ieee80211_key __rcu *default_mgmt_key;

	u16 sequence_number;
	__be16 control_port_protocol;
	bool control_port_no_encrypt;

	struct ieee80211_tx_queue_params tx_conf[IEEE80211_NUM_ACS];

	struct atbm_work_struct work;
	struct sk_buff_head skb_queue;

	bool arp_filter_state;

#ifdef IPV6_FILTERING
	bool ndp_filter_state;
#endif /*IPV6_FILTERING*/

	/*
	 * AP this belongs to: self in AP mode and
	 * corresponding AP in VLAN mode, NULL for
	 * all others (might be needed later in IBSS)
	 */
	struct ieee80211_if_ap *bss;

	/* Channel state */
	struct ieee80211_channel_state chan_state;

	/* bitmap of allowed (non-MCS) rate indexes for rate control */
	u32 rc_rateidx_mask[IEEE80211_NUM_BANDS];

	union {
		struct ieee80211_if_ap ap;
		struct ieee80211_if_wds wds;
		struct ieee80211_if_vlan vlan;
		struct ieee80211_if_managed mgd;
		struct ieee80211_if_ibss ibss;
		struct ieee80211_if_mesh mesh;
		u32 mntr_flags;
	} u;

#ifdef CONFIG_MAC80211_ATBM_DEBUGFS
	struct {
		struct dentry *dir;
		struct dentry *subdir_stations;
		struct dentry *default_unicast_key;
		struct dentry *default_multicast_key;
		struct dentry *default_mgmt_key;
	} debugfs;
#endif

	/* used for uploading changed mc list */
	struct atbm_work_struct reconfig_filter;

	unsigned int filter_flags, req_filt_flags; /* FIF_* */

	/* aggregated multicast list */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,35))
	struct netdev_hw_addr_list mc_list;
#else
	struct dev_addr_list *mc_list;
	int mc_count;
#endif

#ifdef ATBM_AP_SME
	struct list_head ap_sme_event;
	struct mutex ap_sme_event_lock;
	struct mutex ap_sme_mlme_lock;
	struct atbm_work_struct ap_sme_event_work;
	struct sk_buff_head ap_sme_skb_queue;
	struct atbm_work_struct ap_sme_work;
	struct semaphore   sta_mlme_lock;
#endif
	atomic_t connectting;
#ifdef 	CONFIG_MAC80211_BRIDGE
	void * bridge_priv;
#endif //
	u8 *last_scan_ie;
	u32 last_scan_ie_len;

#ifdef	CONFIG_ATBM_RADAR_DETECT
#ifdef CONFIG_ATBM_5G_PRETEND_2G
	bool radar_required;
	struct atbm_delayed_work dfs_cac_timer_work;
#if (LINUX_VERSION_CODE > KERNEL_VERSION(3, 9, 0))
	struct cfg80211_chan_def dfs_cac_chan_def;
#endif
#endif
#endif


#ifdef CONFIG_ATBM_AP_CHANNEL_CHANGE_EVENT
		struct atbm_delayed_work ap_channel_event_work;
#endif
	struct ieee80211_internal_ap_conf *__rcu internal_ap_conf;
	/* must be last, dynamically sized area in this! */
#ifdef CONFIG_IEEE80211_SPECIAL_FILTER
	struct list_head filter_list;
	struct sk_buff_head special_filter_skb_queue;
	struct atbm_work_struct special_filter_work;
	atomic_t special_enable;
	bool special_running;
#endif
	struct ieee80211_vif vif;
};

static inline
struct ieee80211_sub_if_data *vif_to_sdata(struct ieee80211_vif *p)
{
	return container_of(p, struct ieee80211_sub_if_data, vif);
}
static inline enum nl80211_channel_type vif_chw(struct ieee80211_vif *p)
{
	return p->bss_conf.channel_type;
}
enum sdata_queue_type {
	IEEE80211_SDATA_QUEUE_TYPE_FRAME	= 0,
	IEEE80211_SDATA_QUEUE_AGG_START		= 1,
	IEEE80211_SDATA_QUEUE_AGG_STOP		= 2,
	IEEE80211_SDATA_QUEUE_SEND_STATUS   = 3,
	IEEE80211_SDATA_QUEUE_TIME_OUT		= 4,
	IEEE80211_SDATA_QUEUE_FLUSH			= 5,
	IEEE80211_SDATA_IDLE_RECAL			= 6,
};
enum ieee80211_special_work_type{
	IEEE80211_SPECIAL_NONE_TYPE			 = 0,
	IEEE80211_SPECIAL_AP_SPECIAL_BEACON	 = 1,
	IEEE80211_SPECIAL_AP_SPECIAL_PROBRSP = 2,
	IEEE80211_SPECIAL_STA_SPECIAL_PROBR	 = 3,
	IEEE80211_SPECIAL_STA_PASSICE_SCAN   = 4,
	IEEE80211_SPECIAL_STA_POSITIVE_SCAN  = 5,
	IEEE80211_SPECIAL_CHANGE_CHANNEL_FREQ= 6,
	IEEE80211_SPECIAL_STA_SET_AP_CONFIG  = 7,
};

struct ieee80211_special_work_common{
	struct ieee80211_sub_if_data *req_sdata;
	void *priv;
};
struct ieee80211_update_special{
	struct ieee80211_sub_if_data *req_sdata;
	u8 *special_ie;
	size_t special_len;
};
struct ieee80211_special_work_scan{
	struct ieee80211_sub_if_data *scan_sdata;
	u8 *channels;
	u8 *ie;
	u8 *bssid;
	struct cfg80211_ssid *ssid;
	u8 n_channels;
	u16 ie_len;
};
struct ieee80211_special_work_ap_conf{
	struct ieee80211_sub_if_data *sdata;
	struct ieee80211_internal_ap_conf *conf_req;
};
enum {
	IEEE80211_RX_MSG	= 1,
	IEEE80211_TX_STATUS_MSG	= 2,
	IEEE80211_EOSP_MSG	= 3,
};

struct skb_eosp_msg_data {
	u8 sta[ETH_ALEN], iface[ETH_ALEN];
};

enum queue_stop_reason {
	IEEE80211_QUEUE_STOP_REASON_DRIVER,
	IEEE80211_QUEUE_STOP_REASON_PS,
	IEEE80211_QUEUE_STOP_REASON_CSA,
	IEEE80211_QUEUE_STOP_REASON_AGGREGATION,
	IEEE80211_QUEUE_STOP_REASON_SUSPEND,
	IEEE80211_QUEUE_STOP_REASON_SKB_ADD,
	IEEE80211_QUEUE_STOP_REASON_CHTYPE_CHANGE,
};

#ifdef CONFIG_MAC80211_ATBM_LEDS
struct tpt_led_trigger {
	struct led_trigger trig;
	char name[32];
	const struct ieee80211_tpt_blink *blink_table;
	unsigned int blink_table_len;
	struct atbm_timer_list timer;
	unsigned long prev_traffic;
	unsigned long tx_bytes, rx_bytes;
	unsigned int active, want;
	bool running;
};
#endif

/**
 * mac80211 scan flags - currently active scan mode
 *
 * @SCAN_SW_SCANNING: We're currently in the process of scanning but may as
 *	well be on the operating channel
 * @SCAN_HW_SCANNING: The hardware is scanning for us, we have no way to
 *	determine if we are on the operating channel or not
 * @SCAN_OFF_CHANNEL: We're off our operating channel for scanning,
 *	gets only set in conjunction with SCAN_SW_SCANNING
 * @SCAN_COMPLETED: Set for our scan work function when the driver reported
 *	that the scan completed.
 * @SCAN_ABORTED: Set for our scan work function when the driver reported
 *	a scan complete for an aborted scan.
 */
enum {
	SCAN_SW_SCANNING,
	SCAN_HW_SCANNING,
	SCAN_OFF_CHANNEL,
	SCAN_COMPLETED,
	SCAN_ABORTED,
	SCAN_INTERNAL_SCANNING,
	SCAN_CFG80211_SCANNING,
};

/**
 * enum mac80211_scan_state - scan state machine states
 *
 * @SCAN_DECISION: Main entry point to the scan state machine, this state
 *	determines if we should keep on scanning or switch back to the
 *	operating channel
 * @SCAN_SET_CHANNEL: Set the next channel to be scanned
 * @SCAN_SEND_PROBE: Send probe requests and wait for probe responses
 * @SCAN_LEAVE_OPER_CHANNEL: Leave the operating channel, notify the AP
 *	about us leaving the channel and stop all associated STA interfaces
 * @SCAN_ENTER_OPER_CHANNEL: Enter the operating channel again, notify the
 *	AP about us being back and restart all associated STA interfaces
 */
enum mac80211_scan_state {
	SCAN_DECISION,
	SCAN_SET_CHANNEL,
	SCAN_SEND_PROBE,
	SCAN_LEAVE_OPER_CHANNEL,
	SCAN_ENTER_OPER_CHANNEL,
};
enum ieee80211_ch_reqtype{
	MULT_CH_SET_CH,
	MULT_CH_SET_BEACON,
	MULT_CH_AUTHEN,
	MULT_CH_ASSOC,
	MULT_CH_P2P,
};
enum ieee80211_enc_type{
	IEEE80211_ENC_OPEN,
	IEEE80211_ENC_WPA,
	IEEE80211_ENC_WPA2,
	IEEE80211_ENC_WPA_WPA2,
	IEEE80211_ENC_WEP,
};
struct ieee80211_mult_channel_req{
	struct list_head list;
	struct ieee80211_sub_if_data *req_sdata;
	struct ieee80211_sub_if_data *sdata;
	enum ieee80211_ch_reqtype req_type;
	void *cookie;
};

#define IEEE80211_INTERNAL_SCAN_HASHBITS    4
#define IEEE80211_INTERNAL_SCAN_HASHENTRIES (1 << IEEE80211_INTERNAL_SCAN_HASHBITS)

struct ieee80211_internal_scan_sta{
	u8 bssid[ETH_ALEN];
	u8 ssid[IEEE80211_MAX_SSID_LEN];
	u8 ssid_len;
	u8 channel;
	u8 signal;
	enum ieee80211_enc_type enc_type;
	u8 ieee80211_enc_type_name;
	bool beacon;
	bool cca;
	u8 ie_len;
	u8 *ie;
	struct sk_buff *skb;
};
struct ieee80211_internal_scan_result{
	struct ieee80211_internal_scan_sta sta;
};

struct ieee80211_internal_mac{
	u8 mac[6];
	struct hlist_node hnode;
};

struct atbm_internal_scan_sta_node{
	struct ieee80211_internal_scan_sta sta;
	struct hlist_node hnode;
};
struct atbm_internal_scan_results_req{	
	bool (__rcu *result_handle)(struct ieee80211_hw *hw,struct atbm_internal_scan_results_req *req,struct ieee80211_internal_scan_sta *sta);
	bool flush;
	size_t n_stas;
	void *priv;
};
enum ieee80211_internal_scan_flags{
	IEEE80211_INTERNAL_SCAN_FLAGS__CCA             = BIT(0),
	IEEE80211_INTERNAL_SCAN_FLAGS__PASSAVI_SCAN    = BIT(1),
	IEEE80211_INTERNAL_SCAN_FLAGS__NEED_SKB		   = BIT(2),
	IEEE80211_INTERNAL_SCAN_FLAGS__SCAN_SPLIT	   = BIT(3),
	IEEE80211_INTERNAL_SCAN_FLAGS__NEED_BSSID      = BIT(4),
};
struct ieee80211_internal_scan_request{
	bool (__rcu *result_handle)(struct ieee80211_sub_if_data *sdata,void *priv,struct ieee80211_internal_scan_result *result,bool finish);
	void __rcu *priv;
	u8 *channels;
	u8 n_channels;
	u8 *ies;
	int ie_len;
	struct cfg80211_ssid *ssids;
	u8 n_ssids;
	bool no_cck;
	struct ieee80211_internal_mac *macs;
	u8 n_macs;
	u32 req_flags;
	u8 bssid[6];
};

struct ieee80211_internal_scan{
	struct ieee80211_internal_scan_request req;
	struct hlist_head mac_hash_list[IEEE80211_INTERNAL_SCAN_HASHENTRIES];
};

struct ieee80211_cca_val{
	u8 *val;
	u8 val_len;
};
struct ieee80211_internal_scan_notity{
	bool success;
	struct ieee80211_cca_val cca;
};
struct ieee80211_special_freq{
	struct ieee80211_channel *channel;
	int freq;
	struct hlist_node hnode;
};
struct ieee80211_internal_channel_auto_select_req{	
	struct ieee80211_internal_mac *macs;
	u8 n_macs;
	u8 *channels;
	u8 n_channels;
};

struct ieee80211_internal_channel_auto_select_results{
	unsigned int n_aps[IEEE80211_ATBM_MAX_SCAN_CHANNEL_INDEX];
	unsigned int weight[IEEE80211_ATBM_MAX_SCAN_CHANNEL_INDEX];
	u8 busy_ratio[IEEE80211_ATBM_MAX_SCAN_CHANNEL_INDEX];
	u8 susgest_channel;
	u8 *ignore_channels;
	u8 ignore_n_channels;
	u8 *channels;
	u8 n_channels;
	u8 version;
};
#define IEEE80211_INTERNAL_STA_FLAGS_AP				BIT(0)
#define IEEE80211_INTERNAL_STA_FLAGS_STA			BIT(1)
#define IEEE80211_INTERNAL_STA_FLAGS_MAC			BIT(2)
#define IEEE80211_INTERNAL_STA_FLAGS_TXRXBYTE		BIT(3)
#define IEEE80211_INTERNAL_STA_FLAGS_RXIDLE			BIT(4)
#define IEEE80211_INTERNAL_STA_FLAGS_SIGNAL			BIT(5)
#define IEEE80211_INTERNAL_STA_FLAGS_TOPRATE		BIT(6)
#define IEEE80211_INTERNAL_STA_FLAGS_CHANNEL		BIT(7)
#define IEEE80211_INTERNAL_STA_FLAGS_SSID			BIT(8)

struct ieee80211_internal_sta_info{
	u8 mac[6];
	u8 channel;
	u8 channel_type;
	int signal;
	int avg_signal;
	int top_rate;
	unsigned long tx_bytes;
	unsigned long rx_bytes;
	u8 ssid[IEEE80211_MAX_SSID_LEN];
	u8 ssid_len;
	struct ieee80211_sub_if_data *sdata;
	u32 filled;
};

struct ieee80211_internal_sta_req{
	u32 req_flag;
	enum nl80211_iftype type;
	void *priv;
	bool (__rcu *sta_handle)(struct ieee80211_internal_sta_info *stainfo,void *priv);
	u8 n_macs;
	struct ieee80211_internal_mac *macs;
	
};

struct ieee80211_internal_monitor_req{
	u8 ch;
	enum nl80211_channel_type chtype;
	void *priv;
	bool (__rcu *monitor_rx)(struct ieee80211_sub_if_data *sdata,void *priv,struct sk_buff *skb);
};

struct ieee80211_internal_monitor{
	struct ieee80211_internal_monitor_req req;
	struct sk_buff_head rx_skb;
};

struct ieee80211_internal_wsm_txpwr_dcxo{
	int txpwr_L; 
	int txpwr_M; 
	int txpwr_H;
	int dcxo;
};
struct ieee80211_internal_wsm_txpwr{
	int txpwr_indx;
};
struct ieee80211_internal_wsm_adaptive{
	int enable;
};

#define IEEE80211_INTERNAL_RATE_FLAGS_SET_TX_RATE		BIT(0)
#define IEEE80211_INTERNAL_RATE_FLAGS_SET_TOP_RATE		BIT(1)
#define IEEE80211_INTERNAL_RATE_FLAGS_CLEAR_TX_RATE		BIT(2)
#define IEEE80211_INTERNAL_RATE_FLAGS_CLEAE_TOP_RATE	BIT(3)
#define IEEE80211_INTERNAL_RATE_FLAGS_SET_MIN_RATE		BIT(4)
#define IEEE80211_INTERNAL_RATE_FLAGS_CLEAR_MIN_RATE	BIT(5)



struct ieee80211_internal_rate_req{
	int rate;
	int flags;
};
struct ieee80211_internal_rate_power_req{
	char rate_index;
	char power;
};
struct ieee80211_internal_set_freq_req{
	u16 	channel_num;
	size_t	freq;
	bool    set;
};
enum ieee80211_internal_req_chip_flag{
	IEEE80211_INTERNAL_REQ_CHIP_FLAGS__CHIP_VER 		= BIT(0),
	IEEE80211_INTERNAL_REQ_CHIP_FLAGS__DRIVER_VER		= BIT(1),
	IEEE80211_INTERNAL_REQ_CHIP_FLAGS__FIRMWARE_VER		= BIT(2),
	IEEE80211_INTERNAL_REQ_CHIP_FLAGS__BUS_TYPE			= BIT(3),
	IEEE80211_INTERNAL_REQ_CHIP_FLAGS__CAP				= BIT(4),
};
struct ieee80211_internal_req_chip{
	const char *chip_version;
	size_t     driver_version;
	size_t     firmware_version;
	size_t     bus_type;
	u32        cap;
	u32        flags;
};

struct ieee80211_internal_mac_monitor_results{
	u8  found;
	u8	rssi;
	u8  forcestop;
	u8  used;
	u8  index;
	u8  enabled;
	u8  mac[6];
	u32 delta_time;
};
enum ieee80211_internal_iftype{
	IEEE80211_INTERNAL_IFTYPE_REQ__MANAGED,
	IEEE80211_INTERNAL_IFTYPE_REQ__MONITOR,
};
struct ieee80211_internal_iftype_req{
	enum ieee80211_internal_iftype if_type;
	u8 channel;
};
#define IEEE80211_INTERNAL_MAC_MONITOR_START		BIT(0)
#define IEEE80211_INTERNAL_MAC_MONITOR_STOP			BIT(1)
#define IEEE80211_INTERNAL_MAC_MONITOR_RESULTS		BIT(2)

#define IEEE80211_INTERNAL_MAX_STA_MONITOR_SUPPORT	8
struct ieee80211_internal_mac_monitor{
	u32 flags;
	u8 mac[ETH_ALEN];
	u8 index;
	struct ieee80211_internal_mac_monitor_results reults[IEEE80211_INTERNAL_MAX_STA_MONITOR_SUPPORT];
};
enum ieee80211_internal_scan_status{
	IEEE80211_INTERNAL_SCAN_STATUS__IDLE     = 0,
	IEEE80211_INTERNAL_SCAN_STATUS__FINISHED = 1,
	IEEE80211_INTERNAL_SCAN_STATUS__ABORT    = 2,
	IEEE80211_INTERNAL_SCAN_STATUS__WAIT     = 3,
};
struct ieee80211_name_def{
	struct list_head list;
	const char *s_name;
	char *d_name;
	u16	 name_size;
	u8 mem[0] __attribute__((__aligned__(32)));
};
struct ieee80211_local {
	/* embed the driver visible part.
	 * don't cast (use the static inlines below), but we keep
	 * it first anyway so they become a no-op */
	struct ieee80211_hw hw;

	const struct ieee80211_ops *ops;

	/*
	 * work stuff, potentially off-channel (in the future)
	 */
	struct list_head work_list;
	struct atbm_timer_list work_timer;
	struct atbm_work_struct work_work;
	struct sk_buff_head work_skb_queue;

	/*
	 * private workqueue to mac80211. mac80211 makes this accessible
	 * via ieee80211_queue_work()
	 */
	struct atbm_workqueue_struct *workqueue;

	unsigned long queue_stop_reasons[IEEE80211_MAX_QUEUES];
	/* also used to protect ampdu_ac_queue and amdpu_ac_stop_refcnt */
	spinlock_t queue_stop_reason_lock;

	int open_count;
	int monitors, cooked_mntrs;
	int only_monitors;
	struct ieee80211_sub_if_data *monitor_sdata;
	bool wiphy_ciphers_allocated;

	/* protects the aggregated multicast list and filter calls */
	spinlock_t filter_lock;

	/* used to reconfigure hardware SM PS */
	struct atbm_work_struct recalc_smps;

	bool tim_in_locked_section; /* see ieee80211_beacon_get() */

	/*
	 * suspended is true if we finished all the suspend _and_ we have
	 * not yet come up from resume. This is to be used by mac80211
	 * to ensure driver sanity during suspend and mac80211's own
	 * sanity. It can eventually be used for WoW as well.
	 */
	bool suspended;

	/*
	 * Resuming is true while suspended, but when we're reprogramming the
	 * hardware -- at that time it's allowed to use ieee80211_queue_work()
	 * again even though some other parts of the stack are still suspended
	 * and we still drop received frames to avoid waking the stack.
	 */
	bool resuming;

	/*
	 * quiescing is true during the suspend process _only_ to
	 * ease timer cancelling etc.
	 */
	bool quiescing;

	/* device is started */
	bool started;

	/* wowlan is enabled -- don't reconfig on resume */
	bool wowlan;

	int tx_headroom; /* required headroom for hardware/radiotap */

	/* Tasklet and skb queue to process calls from IRQ mode. All frames
	 * added to skb_queue will be processed, but frames in
	 * skb_queue_unreliable may be dropped if the total length of these
	 * queues increases over the limit. */
#define IEEE80211_IRQSAFE_QUEUE_LIMIT 128
	struct tasklet_struct tasklet;
	struct sk_buff_head skb_queue;
	struct sk_buff_head skb_queue_unreliable;
	bool tasklet_running;
	/*
	 * Internal FIFO queue which is shared between multiple rx path
	 * stages. Its main task is to provide a serialization mechanism,
	 * so all rx handlers can enjoy having exclusive access to their
	 * private data structures.
	 */
	struct sk_buff_head rx_skb_queue;
	bool running_rx_handler;	/* protected by rx_skb_queue.lock */
	spinlock_t rx_path_lock;
	/* Station data */
	/*
	 * The mutex only protects the list and counter,
	 * reads are done in RCU.
	 * Additionally, the lock protects the hash table,
	 * the pending list and each BSS's TIM bitmap.
	 */
	struct mutex sta_mtx;
	spinlock_t sta_lock;
	unsigned long num_sta;
	struct list_head sta_list, sta_pending_list;
	struct sta_info __rcu *sta_hash[STA_HASH_SIZE];
	atomic_t resume_timer_start;
	struct atbm_timer_list resume_timer;
	struct atbm_timer_list sta_cleanup;
	struct atbm_work_struct sta_finish_work;
	int sta_generation;

	struct sk_buff_head pending[IEEE80211_MAX_QUEUES];
	struct tasklet_struct tx_pending_tasklet;

	atomic_t agg_queue_stop[IEEE80211_MAX_QUEUES];

	struct rate_control_ref *rate_ctrl;
#ifdef CONFIG_ATBM_USE_SW_ENC
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 4, 0))
	struct arc4_ctx wep_tx_ctx;
	struct arc4_ctx wep_rx_ctx;
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0))
	struct crypto_skcipher *wep_tx_tfm;
	struct crypto_skcipher *wep_rx_tfm;
#else
	struct crypto_cipher *wep_tx_tfm;
	struct crypto_cipher *wep_rx_tfm;
#endif
	u32 wep_iv;
#endif
	/* see iface.c */
	struct list_head interfaces;
	struct mutex iflist_mtx;

	/*
	 * Key mutex, protects sdata's key_list and sta_info's
	 * key pointers (write access, they're RCU.)
	 */
	struct mutex key_mtx;

	/* mutex for scan and work locking */
	struct mutex mtx;

	/* Scanning and BSS list */
	unsigned long scan_idle_time;
	unsigned long scanning;
	struct cfg80211_ssid scan_ssid;
	struct cfg80211_scan_request *int_scan_req;
	struct ieee80211_internal_monitor internal_monitor;
	struct cfg80211_scan_request *scan_req, *hw_scan_req,*pending_scan_req;
	struct ieee80211_channel *scan_channel;
	struct cfg80211_scan_info scan_info;
	unsigned long pending_scan_start_time;
	struct ieee80211_roc_work *roc_pendding;
	struct ieee80211_sub_if_data *roc_pendding_sdata;
	enum ieee80211_band hw_scan_band;
	int scan_channel_idx;
	int scan_channel_space;
	int scan_ies_len;
	int scan_n_channels;
	struct ieee80211_scan_req_wrap scan_req_wrap;
	/* internal scan parmas */
	struct ieee80211_internal_scan internal_scan;
	struct hlist_head internal_scan_list[ATBM_COMMON_HASHENTRIES];
	spinlock_t	internal_scan_list_lock;
	wait_queue_head_t internal_scan_wq;
	atomic_t internal_scan_status;
	u32 internal_scan_n_results;
	u8 __rcu *internal_scan_ie;
	u32 internal_scan_ie_len;
#ifdef CONFIG_ATBM_SUPPORT_SCHED_SCAN	
	bool sched_scanning;
	struct ieee80211_sched_scan_ies sched_scan_ies;
	struct atbm_work_struct sched_scan_stopped_work;
#endif
	char country_code[2];
	unsigned long leave_oper_channel_time;
	enum mac80211_scan_state next_scan_state;
	struct atbm_delayed_work scan_work;
	struct ieee80211_sub_if_data *scan_sdata,*pending_scan_sdata;

	/* Channel state */
	struct ieee80211_channel_state chan_state;

	/* SNMP counters */
	/* dot11CountersTable */
	u32 dot11TransmittedFragmentCount;
	u32 dot11MulticastTransmittedFrameCount;
	u32 dot11FailedCount;
	u32 dot11RetryCount;
	u32 dot11MultipleRetryCount;
	u32 dot11FrameDuplicateCount;
	u32 dot11ReceivedFragmentCount;
	u32 dot11MulticastReceivedFrameCount;
	u32 dot11TransmittedFrameCount;

#ifdef CONFIG_MAC80211_ATBM_LEDS
	int tx_led_counter, rx_led_counter;
	struct led_trigger *tx_led, *rx_led, *assoc_led, *radio_led;
	struct tpt_led_trigger *tpt_led_trigger;
	char tx_led_name[32], rx_led_name[32],
	     assoc_led_name[32], radio_led_name[32];
#endif

#ifdef CONFIG_MAC80211_ATBM_DEBUG_COUNTERS
	/* TX/RX handler statistics */
	unsigned int tx_handlers_drop;
	unsigned int tx_handlers_queued;
	unsigned int tx_handlers_drop_unencrypted;
	unsigned int tx_handlers_drop_fragment;
	unsigned int tx_handlers_drop_wep;
	unsigned int tx_handlers_drop_not_assoc;
	unsigned int tx_handlers_drop_unauth_port;
	unsigned int rx_handlers_drop;
	unsigned int rx_handlers_queued;
	unsigned int rx_handlers_drop_nullfunc;
	unsigned int rx_handlers_drop_defrag;
	unsigned int rx_handlers_drop_short;
	unsigned int rx_handlers_drop_passive_scan;
	unsigned int tx_expand_skb_head;
	unsigned int tx_expand_skb_head_cloned;
	unsigned int rx_expand_skb_head;
	unsigned int rx_expand_skb_head2;
	unsigned int rx_handlers_fragments;
	unsigned int tx_status_drop;
#define I802_DEBUG_INC(c) (c)++
#else /* CONFIG_MAC80211_ATBM_DEBUG_COUNTERS */
#define I802_DEBUG_INC(c) do { } while (0)
#endif /* CONFIG_MAC80211_ATBM_DEBUG_COUNTERS */


	int total_ps_buffered; /* total number of all buffered unicast and
				* multicast packets for power saving stations
				*/
	int wifi_wme_noack_test;
	unsigned int wmm_acm; /* bit field of ACM bits (BIT(802.1D tag)) */

#ifdef CONFIG_ATBM_APOLLO_TESTMODE
	unsigned int wmm_admitted_ups; /* bit field of admitted priorities */
#endif /*CONFIG_ATBM_APOLLO_TESTMODE*/
	/*
	 * Bitmask of enabled u-apsd queues,
	 * IEEE80211_WMM_IE_STA_QOSINFO_AC_BE & co. Needs a new association
	 * to take effect.
	 */
	unsigned int uapsd_queues;

	/*
	 * Maximum number of buffered frames AP can deliver during a
	 * service period, IEEE80211_WMM_IE_STA_QOSINFO_SP_ALL or similar.
	 * Needs a new association to take effect.
	 */
	unsigned int uapsd_max_sp_len;
#ifdef CONFIG_ATBM_PM_QOS
	struct notifier_block network_latency_notifier;
#endif
	struct notifier_block ifa_notifier;

#ifdef IPV6_FILTERING
	struct notifier_block ifa6_notifier;
#endif /*IPV6_FILTERING*/

	int user_power_level; /* in dBm */
	int power_constr_level; /* in dBm */

	enum ieee80211_smps_mode smps_mode;
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	struct atbm_work_struct restart_work;
#endif

#ifdef IPV6_FILTERING
#ifdef CONFIG_INET
	struct atbm_work_struct ifa6_changed_work;
	struct ieee80211_sub_if_data *ifa6_sdata;
#endif
#endif /*IPV6_FILTERING*/

#ifdef CONFIG_MAC80211_ATBM_DEBUGFS
	struct local_debugfsdentries {
		struct dentry *rcdir;
		struct dentry *keys;
	} debugfs;
#endif


#ifdef CONFIG_ATBM_SUPPORT_P2P
	/*
	 * Remain-on-channel support
	 */
	struct list_head roc_list;
	struct atbm_work_struct hw_roc_start, hw_roc_done;
	unsigned long hw_roc_start_time;
	unsigned long hw_roc_extend_time;
	u64 roc_cookie_counter;
	struct ieee80211_channel *hw_roc_channel;	
	u64 roc_cookie;
#endif
	/* Temporary remain-on-channel for off-channel operations */
	struct ieee80211_channel *tmp_channel;

	/*
	*Sta in listen state support
	*/
#ifdef CONFIG_ATBM_STA_LISTEN
	struct ieee80211_channel *listen_channel;
	struct ieee80211_sub_if_data *listen_sdata;
	bool listen_started;
#endif
	
#ifdef IEEE80211_SUPPORT_NAPI
	/* dummy netdev for use w/ NAPI */
	struct net_device napi_dev;

	struct napi_struct napi;
#endif
#ifdef ATBM_CONNECT_WITHOUT_SCAN
	atomic_t connectting;
	unsigned long con_start_time;
#endif
	atomic_t connectting;
	struct list_head channel_req;
	/*
	*special beacon and probe request update
	*/
	struct atbm_work_struct special_work;
	struct sk_buff_head special_req_list;
	/*
	*special channel params
	*/
	struct hlist_head special_freq_list[ATBM_COMMON_HASHENTRIES];
	struct list_head ieee80211_name_list;
	char *ieee80211_name_base;
	int  ieee80211_name_len;
	spinlock_t ieee80211_name_lock;
#ifdef SIGMSTAR_SCAN_FEATURE
	s8 noise_floor[CHANNEL_NUM];
#endif  //SIGMSTAR_SCAN_FEATURE
	bool adaptive_started; // adaptive mode stop scan and beacon lost
	unsigned long   adaptive_started_time;

	/*
	 * add by yuzhihuang
	 * Notify the upper process wifi of some anomalies
	 */
	int upper_pid;
	
};

/*
*data struct for prism header
*/
enum {
	ieee80211_prism_sniffrm		    	= 0x00000044,
	ieee80211_prism_sniffrm_hosttime	= 0x00010044,
	ieee80211_prism_sniffrm_mactime		= 0x00020044,
	ieee80211_prism_sniffrm_channel		= 0x00030044,
	ieee80211_prism_sniffrm_rssi		= 0x00040044,
	ieee80211_prism_sniffrm_sq			= 0x00050044,
	ieee80211_prism_sniffrm_signal		= 0x00060044,
	ieee80211_prism_sniffrm_noise		= 0x00070044,
	ieee80211_prism_sniffrm_rate		= 0x00080044,
	ieee80211_prism_sniffrm_istx		= 0x00090044,
	ieee80211_prism_sniffrm_frmlen		= 0x000A0044
};
struct ieee80211_prism_u32{
    u32 did;
    u16 status;
    u16 len;
    u32 data;
};
struct ieee80211_prism_header{
        u32 msg_code;
        u32 msg_len;
#define WLAN_DEVNAMELEN_MAX 16
        u8  dev_name[WLAN_DEVNAMELEN_MAX];
        struct ieee80211_prism_u32 hosttime;
        struct ieee80211_prism_u32 mactime;
        struct ieee80211_prism_u32 channel;
        struct ieee80211_prism_u32 rssi;
        struct ieee80211_prism_u32 sq;
        struct ieee80211_prism_u32 signal;
        struct ieee80211_prism_u32 noise;
        struct ieee80211_prism_u32 rate;
        struct ieee80211_prism_u32 istx;
        struct ieee80211_prism_u32 frmlen;
} ;

static inline struct ieee80211_sub_if_data *
IEEE80211_DEV_TO_SUB_IF(struct net_device *dev)
{
	return netdev_priv(dev);
}

/* this struct represents 802.11n's RA/TID combination */
struct ieee80211_ra_tid {
	u8 ra[ETH_ALEN];
	u16 tid;
};

static inline struct ieee80211_local *hw_to_local(
	struct ieee80211_hw *hw)
{
	return container_of(hw, struct ieee80211_local, hw);
}

static inline struct ieee80211_hw *local_to_hw(
	struct ieee80211_local *local)
{
	return &local->hw;
}

static inline int ieee80211_bssid_match(const u8 *raddr, const u8 *addr)
{
	return atbm_compare_ether_addr(raddr, addr) == 0 ||
	       is_broadcast_ether_addr(raddr);
}

//#define ATBM_CONNECT_WITHOUT_SCAN
#ifdef ATBM_CONNECT_WITHOUT_SCAN
#define LOCAL_CONNECT_INIT(local)			\
	do{										\
		atomic_set(&local->connectting,0);		\
		local->con_start_time = jiffies;		\
	}while(0)
#else
#define LOCAL_CONNECT_INIT(local)
#endif

#ifdef ATBM_CONNECT_WITHOUT_SCAN
#define LOCAL_SET_CONNECT_RUNNING(local) 		\
	do{											\
		atomic_set(&(local)->connectting,1);		\
		local->con_start_time = jiffies;		\
	}while(0)
#else
#define LOCAL_SET_CONNECT_RUNNING(local)
#endif

#ifdef ATBM_CONNECT_WITHOUT_SCAN

#define LOCAL_SET_CONNECT_STOP(local)			\
	do{											\
		atomic_set(&(local)->connectting,0);		\
		local->con_start_time = 0;				\
	}while(0)
#else
#define LOCAL_SET_CONNECT_STOP(local)
#endif

#ifdef ATBM_CONNECT_WITHOUT_SCAN
#define LOCAL_IS_CONNECTTING(local)							\
	((atomic_read(&(local)->connectting)&&time_is_after_jiffies((local)->con_start_time+10*HZ))?1:(atomic_xchg(&(local)->connectting, 0),0))
#else
#define LOCAL_IS_CONNECTTING(local)		(atomic_read(&(local)->connectting))
#endif
#define SDATA_IS_CONNECTTING(sdata)		(atomic_read(&(sdata)->connectting))	
#ifdef CONFIG_ATBM_MAC80211_NO_USE
void ieee80211_notify_channel_change(struct ieee80211_local *local,
				     struct ieee80211_sub_if_data *sdata);
#endif
int ieee80211_hw_config(struct ieee80211_local *local, u32 changed);
void ieee80211_tx_set_protected(struct ieee80211_tx_data *tx);
void ieee80211_bss_info_change_notify(struct ieee80211_sub_if_data *sdata,
				      u32 changed);
void ieee80211_configure_filter(struct ieee80211_sub_if_data *sdata);
u32 ieee80211_reset_erp_info(struct ieee80211_sub_if_data *sdata);

/* STA code */
void ieee80211_sta_setup_sdata(struct ieee80211_sub_if_data *sdata);
int ieee80211_mgd_auth(struct ieee80211_sub_if_data *sdata,
		       struct cfg80211_auth_request *req);
int ieee80211_mgd_assoc(struct ieee80211_sub_if_data *sdata,
			struct cfg80211_assoc_request *req);
int ieee80211_mgd_deauth(struct ieee80211_sub_if_data *sdata,
			 struct cfg80211_deauth_request *req,
			 void *cookie);
int ieee80211_mgd_disassoc(struct ieee80211_sub_if_data *sdata,
			   struct cfg80211_disassoc_request *req,
			   void *cookie);
#ifdef CONFIG_ATBM_MAC80211_NO_USE
void ieee80211_send_pspoll(struct ieee80211_local *local,
			   struct ieee80211_sub_if_data *sdata);
#endif
void ieee80211_recalc_ps(struct ieee80211_local *local, s32 latency);
void ieee80211_recalc_ps_vif(struct ieee80211_local *local, s32 latency);

#ifdef CONFIG_ATBM_PM_QOS
int ieee80211_max_network_latency(struct notifier_block *nb,
				  unsigned long data, void *dummy);
#endif
int ieee80211_set_arp_filter(struct ieee80211_sub_if_data *sdata);
#ifdef CONFIG_ATBM_SUPPORT_CHANSWITCH
#ifdef ATBM_SUPPORT_WIDTH_40M

void ieee80211_sta_process_chanswitch(struct ieee80211_sub_if_data *sdata,
                                          struct atbm_ieee80211_channel_sw_packed_ie *sw_packed_ie,
                                          struct ieee80211_bss *bss,
                                          u64 timestamp);
#else
void ieee80211_sta_process_chanswitch(struct ieee80211_sub_if_data *sdata,
				      struct ieee80211_channel_sw_ie *sw_elem,
				      struct ieee80211_bss *bss,
				      u64 timestamp);
#endif
#endif
#if defined (CONFIG_PM) ||defined (ATBM_SUSPEND_REMOVE_INTERFACE) || defined (ATBM_SUPPORT_WOW)
void ieee80211_sta_quiesce(struct ieee80211_sub_if_data *sdata);
#endif
void ieee80211_sta_restart(struct ieee80211_sub_if_data *sdata);
void ieee80211_sta_rx_queued_mgmt(struct ieee80211_sub_if_data *sdata,
				  struct sk_buff *skb);
#ifdef CONFIG_ATBM_MAC80211_NO_USE
void ieee80211_sta_work(struct ieee80211_sub_if_data *sdata);
void ieee80211_sta_reset_beacon_monitor(struct ieee80211_sub_if_data *sdata);
void ieee80211_sta_reset_conn_monitor(struct ieee80211_sub_if_data *sdata);
#endif
#ifdef CONFIG_ATBM_SUPPORT_IBSS
/* IBSS code */
void ieee80211_ibss_notify_scan_completed(struct ieee80211_local *local);
void ieee80211_ibss_setup_sdata(struct ieee80211_sub_if_data *sdata);

struct sta_info *ieee80211_ibss_add_sta(struct ieee80211_sub_if_data *sdata,
					u8 *bssid, u8 *addr, u32 supp_rates,
					gfp_t gfp);
int ieee80211_ibss_join(struct ieee80211_sub_if_data *sdata,
			struct cfg80211_ibss_params *params);
int ieee80211_ibss_leave(struct ieee80211_sub_if_data *sdata);
void ieee80211_ibss_quiesce(struct ieee80211_sub_if_data *sdata);
void ieee80211_ibss_restart(struct ieee80211_sub_if_data *sdata);
void ieee80211_ibss_work(struct ieee80211_sub_if_data *sdata);
void ieee80211_ibss_rx_queued_mgmt(struct ieee80211_sub_if_data *sdata,
				   struct sk_buff *skb);
#endif
#ifdef CONFIG_MAC80211_ATBM_MESH
/* mesh code */
void ieee80211_mesh_work(struct ieee80211_sub_if_data *sdata);
void ieee80211_mesh_rx_queued_mgmt(struct ieee80211_sub_if_data *sdata,
				   struct sk_buff *skb);
#endif
/* scan/BSS handling */
void ieee80211_scan_work(struct atbm_work_struct *work);
#ifdef CONFIG_ATBM_SUPPORT_IBSS
int ieee80211_request_internal_scan(struct ieee80211_sub_if_data *sdata,
				    const u8 *ssid, u8 ssid_len,
				    struct ieee80211_channel *chan);
#endif
int ieee80211_request_scan(struct ieee80211_sub_if_data *sdata,
			   struct cfg80211_scan_request *req);
bool ieee80211_internal_scan_triger(struct ieee80211_sub_if_data *sdata,struct cfg80211_scan_request *req);
void ieee80211_scan_internal_deinit(struct ieee80211_local *local);
void ieee80211_scan_internal_int(struct ieee80211_local *local);
bool  ieee80211_scan_internal_req_results(struct ieee80211_local *local,struct atbm_internal_scan_results_req *req);
#ifdef CONFIG_ATBM_SUPPORT_AP_CONFIG
bool ieee80211_update_ap_config(struct ieee80211_sub_if_data *sdata,struct ieee80211_internal_ap_conf *conf,bool clear);
#endif
void ieee80211_scan_cancel(struct ieee80211_local *local);
ieee80211_rx_result
ieee80211_scan_rx(struct ieee80211_sub_if_data *sdata, struct sk_buff *skb);
ieee80211_rx_result
ieee80211_scan_rx_internal(struct ieee80211_sub_if_data *sdata, struct sk_buff *skb);
u8* ieee80211_scan_cca_val_get(struct ieee80211_hw *hw);
void ieee80211_scan_cca_val_put(struct ieee80211_hw *hw);
void ieee80211_scan_cca_notify(struct ieee80211_hw *hw,struct ieee80211_internal_scan_notity *notify);
void ieee80211_mlme_notify_scan_completed(struct ieee80211_local *local);
struct ieee80211_bss *
ieee80211_bss_info_update(struct ieee80211_local *local,
			  struct ieee80211_rx_status *rx_status,
			  struct atbm_ieee80211_mgmt *mgmt,
			  size_t len,
			  struct ieee802_atbm_11_elems *elems,
			  struct ieee80211_channel *channel,
			  bool beacon);
struct ieee80211_bss *
ieee80211_rx_bss_get(struct ieee80211_local *local, u8 *bssid, int freq,
		     u8 *ssid, u8 ssid_len);
void ieee80211_rx_bss_put(struct ieee80211_local *local,
			  struct ieee80211_bss *bss);
#ifdef CONFIG_ATBM_SUPPORT_SCHED_SCAN
/* scheduled scan handling */
int ieee80211_request_sched_scan_start(struct ieee80211_sub_if_data *sdata,
				       struct cfg80211_sched_scan_request *req);
int ieee80211_request_sched_scan_stop(struct ieee80211_sub_if_data *sdata);
void ieee80211_sched_scan_stopped_work(struct atbm_work_struct *work);
#endif
#ifdef CONFIG_ATBM_SUPPORT_P2P
/* off-channel helpers */
void ieee80211_offchannel_stop_beaconing(struct ieee80211_local *local);
void ieee80211_offchannel_stop_station(struct ieee80211_local *local);
void ieee80211_offchannel_return(struct ieee80211_local *local,
				 bool enable_beaconing);
void ieee80211_hw_roc_setup(struct ieee80211_local *local);
void ieee80211_start_next_roc(struct ieee80211_local *local);
void ieee80211_roc_purge(struct ieee80211_sub_if_data *sdata);
void ieee80211_roc_notify_destroy(struct ieee80211_roc_work *roc);
void ieee80211_sw_roc_work(struct atbm_work_struct *work);
void ieee80211_handle_roc_started(struct ieee80211_roc_work *roc);
#endif
/* interface handling */
int ieee80211_iface_init(void);
void ieee80211_iface_exit(void);
int ieee80211_if_add(struct ieee80211_local *local, const char *name,
		     struct net_device **new_dev, enum nl80211_iftype type,
		     struct vif_params *params);
int ieee80211_if_change_type(struct ieee80211_sub_if_data *sdata,
			     enum nl80211_iftype type);
void ieee80211_if_remove(struct ieee80211_sub_if_data *sdata);
void ieee80211_remove_interfaces(struct ieee80211_local *local);
u32 __ieee80211_recalc_idle(struct ieee80211_local *local);
void ieee80211_recalc_idle(struct ieee80211_local *local);
void ieee80211_adjust_monitor_flags(struct ieee80211_sub_if_data *sdata,
				    const int offset);
static inline u8 *ieee8011_get_channel_list(struct ieee80211_hw *hw,size_t *n_channels)
{
	enum ieee80211_band band;
	int  i = 0;
	struct wiphy *wiphy = hw->wiphy;
	u8* channels = NULL;

	if(n_channels == NULL){
		goto exit;
	}
	
	/* all channels */
	for (band = 0; band < IEEE80211_NUM_BANDS; band++) {
		int j;
		if (!wiphy->bands[band])
			continue;
		for (j = 0; j < wiphy->bands[band]->n_channels; j++) {
			i++;
		}
	}
	
	*n_channels = i;
	
	if(i == 0){
		goto exit;
	}
	
	channels = atbm_kzalloc(i, GFP_ATOMIC);

	if(channels == NULL){
		*n_channels = 0;
		goto exit;
	}
	
	i = 0;
	/* all channels */
	for (band = 0; band < IEEE80211_NUM_BANDS; band++) {
		int j;
		if (!wiphy->bands[band])
			continue;
		for (j = 0; j < wiphy->bands[band]->n_channels; j++) {
			channels[i] = channel_hw_value(&wiphy->bands[band]->channels[j]);
			i++;
		}
	}
exit:
	return channels;
}

static inline bool ieee80211_sdata_running(struct ieee80211_sub_if_data *sdata)
{
	return test_bit(SDATA_STATE_RUNNING, &sdata->state);
}

static inline struct ieee80211_channel_state *ieee80211_get_channel_state(
			struct ieee80211_local *local,
			struct ieee80211_sub_if_data *sdata) {
#ifdef CONFIG_ATBM_SUPPORT_MULTI_CHANNEL
	if (local->hw.flags & IEEE80211_HW_SUPPORTS_MULTI_CHANNEL) {
		BUG_ON(!sdata);
		return &sdata->chan_state;
	} else
#endif
	{
		return &local->chan_state;
	}
}
static inline bool atbm_ieee80211_suspend(struct ieee80211_local *local)
{
	if ((local->quiescing == true)||(local->suspended && !local->resuming)||(atomic_read(&local->resume_timer_start))){
		pr_warn("atbm_ieee80211_suspend can not process cfg cmd\n");
		return true;
	}
	return false;
}
/* tx handling */
void ieee80211_clear_tx_pending(struct ieee80211_local *local);
void ieee80211_tx_pending(unsigned long data);
netdev_tx_t ieee80211_monitor_start_xmit(struct sk_buff *skb,
					 struct net_device *dev);
netdev_tx_t ieee80211_subif_start_xmit(struct sk_buff *skb,
				       struct net_device *dev);

/* HT */
void ieee80211_ht_cap_ie_to_sta_ht_cap(struct ieee80211_supported_band *sband,
				       const struct ieee80211_ht_cap *ht_cap_ie,
				       struct ieee80211_sta_ht_cap *ht_cap);
void ieee80211_ht_cap_to_sta_channel_type(struct sta_info *sta);
#ifdef CONFIG_ATBM_DRIVER_PROCESS_BA
void ieee80211_send_delba(struct ieee80211_sub_if_data *sdata,
			  const u8 *da, u16 tid,
			  u16 initiator, u16 reason_code);
#endif
#ifdef CONFIG_ATBM_SMPS
int ieee80211_send_smps_action(struct ieee80211_sub_if_data *sdata,
			       enum ieee80211_smps_mode smps, const u8 *da,
			       const u8 *bssid);
void ieee80211_request_smps_work(struct atbm_work_struct *work);
#endif
void ___ieee80211_stop_rx_ba_session(struct sta_info *sta, u16 tid,
				     u16 initiator, u16 reason, bool stop);
void __ieee80211_stop_rx_ba_session(struct sta_info *sta, u16 tid,
				    u16 initiator, u16 reason, bool stop);
void ieee80211_sta_tear_down_BA_sessions(struct sta_info *sta, bool tx);
void ieee80211_process_delba(struct ieee80211_sub_if_data *sdata,
			     struct sta_info *sta,
			     struct atbm_ieee80211_mgmt *mgmt, size_t len);
void ieee80211_process_addba_request(struct ieee80211_local *local,
				     struct sta_info *sta,
				     struct atbm_ieee80211_mgmt *mgmt,
				     size_t len);
#ifdef CONFIG_ATBM_SW_AGGTX
void ieee80211_process_addba_resp(struct ieee80211_local *local,
				  struct sta_info *sta,
				  struct atbm_ieee80211_mgmt *mgmt,
				  size_t len);
int __ieee80211_stop_tx_ba_session(struct sta_info *sta, u16 tid,
				   enum ieee80211_back_parties initiator,
				   bool tx);
int ___ieee80211_stop_tx_ba_session(struct sta_info *sta, u16 tid,
				    enum ieee80211_back_parties initiator,
				    bool tx);
void ieee80211_start_tx_ba_cb(struct ieee80211_vif *vif, u8 *ra, u16 tid);
void ieee80211_stop_tx_ba_cb(struct ieee80211_vif *vif, u8 *ra, u8 tid);
void ieee80211_tx_ba_session_handle_start(struct sta_info *sta, int tid);
#endif
void ieee80211_ba_session_work(struct atbm_work_struct *work);
void ieee80211_release_reorder_timeout(struct sta_info *sta, int tid);
#ifdef CONFIG_ATBM_SPECTRUM_MGMT
/* Spectrum management */
void ieee80211_process_measurement_req(struct ieee80211_sub_if_data *sdata,
				       struct atbm_ieee80211_mgmt *mgmt,
				       size_t len);
#endif
#if defined (CONFIG_PM)
/* Suspend/resume and hw reconfiguration */
int ieee80211_reconfig(struct ieee80211_local *local);
#endif
void ieee80211_stop_device(struct ieee80211_local *local);

#ifdef CONFIG_PM
int __ieee80211_suspend(struct ieee80211_hw *hw,
			struct cfg80211_wowlan *wowlan);

static inline int __ieee80211_resume(struct ieee80211_hw *hw)
{
	struct ieee80211_local *local = hw_to_local(hw);

	WARN(test_bit(SCAN_HW_SCANNING, &local->scanning),
		"%s: resume scan running\n",
		wiphy_name(hw->wiphy));

	return ieee80211_reconfig(hw_to_local(hw));
}
#else
static inline int __ieee80211_suspend(struct ieee80211_hw *hw,
				      struct cfg80211_wowlan *wowlan)
{
	return 0;
}

static inline int __ieee80211_resume(struct ieee80211_hw *hw)
{
	return 0;
}
#endif

/* utility functions/constants */
extern void *mac80211_wiphy_privid; /* for wiphy privid */
u8 *ieee80211_get_bssid(struct ieee80211_hdr *hdr, size_t len,
			enum nl80211_iftype type);
int ieee80211_frame_duration(enum ieee80211_band band, size_t len,
			     int rate, int erp, int short_preamble);
#ifdef CONFIG_ATBM_SUPPORT_MIC_FAILURE_REPORT
void mac80211_ev_michael_mic_failure(struct ieee80211_sub_if_data *sdata, int keyidx,
				     struct ieee80211_hdr *hdr, const u8 *tsc,
				     gfp_t gfp);
#endif
void ieee80211_set_wmm_default(struct ieee80211_sub_if_data *sdata);
void ieee80211_xmit(struct ieee80211_sub_if_data *sdata, struct sk_buff *skb);
void ieee80211_tx_skb(struct ieee80211_sub_if_data *sdata, struct sk_buff *skb);
void ieee802_11_parse_elems(u8 *start, size_t len,
			    struct ieee802_atbm_11_elems *elems);
u32 atbm_ieee802_11_parse_elems_crc(u8 *start, size_t len,
			       struct ieee802_atbm_11_elems *elems,
			       u64 filter, u32 crc);
u32 ieee80211_atbm_mandatory_rates(struct ieee80211_local *local,
			      enum ieee80211_band band);
#ifdef CONFIG_ATBM_MAC80211_NO_USE
void ieee80211_dynamic_ps_enable_work(struct atbm_work_struct *work);
void ieee80211_dynamic_ps_disable_work(struct atbm_work_struct *work);
void ieee80211_dynamic_ps_timer(unsigned long data);
void ieee80211_send_nullfunc(struct ieee80211_local *local,
			     struct ieee80211_sub_if_data *sdata,
			     int powersave);
void ieee80211_sta_rx_notify(struct ieee80211_sub_if_data *sdata,
			     struct ieee80211_hdr *hdr);
void ieee80211_sta_tx_notify(struct ieee80211_sub_if_data *sdata,
			     struct ieee80211_hdr *hdr, bool ack);
#endif
void ieee80211_beacon_connection_loss_work(struct atbm_work_struct *work);

void ieee80211_wake_queues_by_reason(struct ieee80211_hw *hw,
				     enum queue_stop_reason reason);
void ieee80211_stop_queues_by_reason(struct ieee80211_hw *hw,
				     enum queue_stop_reason reason);
void ieee80211_wake_queue_by_reason(struct ieee80211_hw *hw, int queue,
				    enum queue_stop_reason reason);
void ieee80211_stop_queue_by_reason(struct ieee80211_hw *hw, int queue,
				    enum queue_stop_reason reason);
void ieee80211_propagate_queue_wake(struct ieee80211_local *local, int queue);
void ieee80211_add_pending_skb(struct ieee80211_local *local,
			       struct sk_buff *skb);
void ieee80211_add_pending_skbs(struct ieee80211_local *local,
				struct sk_buff_head *skbs);
void ieee80211_add_pending_skbs_fn(struct ieee80211_local *local,
				   struct sk_buff_head *skbs,
				   void (*fn)(void *data), void *data);

void ieee80211_send_auth(struct ieee80211_sub_if_data *sdata,
			 u16 transaction, u16 auth_alg, u16 status,
			 u8 *extra, size_t extra_len, const u8 *bssid,
			 const u8 *key, u8 key_len, u8 key_idx);
int ieee80211_build_preq_ies(struct ieee80211_local *local, u8 *buffer,
			     const u8 *ie, size_t ie_len,
			     enum ieee80211_band band, u32 rate_mask,
			     u8 channel);
struct sk_buff *ieee80211_build_probe_req(struct ieee80211_sub_if_data *sdata,
					  u8 *dst, u32 ratemask,
					  const u8 *ssid, size_t ssid_len,
					  const u8 *ie, size_t ie_len,
					  bool directed);
void ieee80211_send_probe_req(struct ieee80211_sub_if_data *sdata, u8 *dst,
			      const u8 *ssid, size_t ssid_len,
			      const u8 *ie, size_t ie_len,
			      u32 ratemask, bool directed, bool no_cck);

void ieee80211_sta_def_wmm_params(struct ieee80211_sub_if_data *sdata,
				  const size_t supp_rates_len,
				  const u8 *supp_rates);
u32 ieee80211_sta_get_rates(struct ieee80211_local *local,
			    struct ieee802_atbm_11_elems *elems,
			    enum ieee80211_band band);
#ifdef CONFIG_ATBM_SMPS
int __ieee80211_request_smps(struct ieee80211_sub_if_data *sdata,
			     enum ieee80211_smps_mode smps_mode);
void ieee80211_recalc_smps(struct ieee80211_local *local);
#endif
size_t atbm_ieee80211_ie_split(const u8 *ies, size_t ielen,
			  const u8 *ids, int n_ids, size_t offset);
size_t ieee80211_ie_split_vendor(const u8 *ies, size_t ielen, size_t offset);

/* internal work items */
void ieee80211_work_init(struct ieee80211_local *local);
void ieee80211_add_work(struct ieee80211_work *wk);
void ieee80211_start_connecting_work(struct ieee80211_sub_if_data *sdata,struct ieee80211_channel *chan,
	u8* bssid);
void ieee80211_cancle_connecting_work(struct ieee80211_sub_if_data *sdata,u8* bssid,bool delayed);

void free_work(struct ieee80211_work *wk);
void ieee80211_work_purge(struct ieee80211_sub_if_data *sdata);
ieee80211_rx_result ieee80211_work_rx_mgmt(struct ieee80211_sub_if_data *sdata,
					   struct sk_buff *skb);
int ieee80211_wk_remain_on_channel(struct ieee80211_sub_if_data *sdata,
				   struct ieee80211_channel *chan,
				   enum nl80211_channel_type channel_type,
				   unsigned int duration, u64 *cookie);
int ieee80211_wk_cancel_remain_on_channel(
	struct ieee80211_sub_if_data *sdata, u64 cookie);
void ieee80211_assign_authen_bss(struct ieee80211_sub_if_data *sdata,struct cfg80211_bss *pub);
void ieee80211_free_authen_bss(struct ieee80211_sub_if_data *sdata);
void ieee80211_ap_rx_queued_mgmt_special(struct ieee80211_sub_if_data *sdata,
				  struct sk_buff *skb);
#ifdef CONFIG_ATBM_STA_LISTEN
void ieee80211_sta_rx_queued_mgmt_special(struct ieee80211_sub_if_data *sdata,
				  struct sk_buff *skb);
#endif
bool ieee80211_send_special_probe_req(struct ieee80211_sub_if_data *sdata, u8 *dst,
			      const u8 *ssid, size_t ssid_len,
			      const u8 *special_ie, size_t special_ie_len);
bool ieee80211_ap_update_special_beacon(struct ieee80211_sub_if_data *sdata,
		const u8 *special_ie, size_t special_ie_len);
bool ieee80211_ap_update_special_probe_response(struct ieee80211_sub_if_data *sdata,
		const u8 *special_ie, size_t special_ie_len);
bool ieee80211_ap_update_special_probe_request(struct ieee80211_sub_if_data *sdata,
		const u8 *special_ie, size_t special_ie_len);

bool ieee80211_updata_extra_ie(struct ieee80211_sub_if_data *sdata,enum ieee80211_special_work_type type,
		union iee80211_extra_ie *extra);
bool ieee80211_special_freq_update(struct ieee80211_local *local,struct ieee80211_special_freq *special);
void ieee80211_special_freq_clear(struct ieee80211_local *local,struct ieee80211_special_freq *special);
#ifdef CONFIG_ATBM_STA_LISTEN
int ieee80211_set_sta_channel(struct ieee80211_sub_if_data *sdata,int channel);
#endif
bool ieee80211_sta_triger_passive_scan(struct ieee80211_sub_if_data *sdata,
													u8 *channels,size_t n_channels);
bool ieee80211_sta_triger_positive_scan(struct ieee80211_sub_if_data *sdata,
													  u8 *channels,size_t n_channels,
													  u8 *ssid,size_t ssid_len,
													  u8 *ie,u16 ie_len,u8 *bssid);
/* channel management */
enum ieee80211_chan_mode {
	CHAN_MODE_UNDEFINED,
	CHAN_MODE_HOPPING,
	CHAN_MODE_FIXED,
};

enum ieee80211_chan_mode
ieee80211_get_channel_mode(struct ieee80211_local *local,
			   struct ieee80211_sub_if_data *ignore);
bool ieee80211_set_channel_type(struct ieee80211_local *local,
				struct ieee80211_sub_if_data *sdata,
				enum nl80211_channel_type chantype);
#ifdef CONFIG_ATBM_SUPPORT_P2P
void ieee80211_run_pending_scan(struct ieee80211_local *local);
#endif
bool ieee80211_rx_sta_cook_deauthen(struct sta_info *sta);
bool ieee80211_tx_multicast_deauthen(struct ieee80211_sub_if_data *sdata);
bool ieee80211_tx_sta_deauthen(struct sta_info *sta);

#ifdef CONFIG_MAC80211_ATBM_NOINLINE
#define debug_noinline noinline
#else
#define debug_noinline
#endif

#ifdef ATBM_AP_SME
void ieee80211_ap_sme_queue_mgmt_init(struct ieee80211_sub_if_data *sdata);
void ieee80211_ap_sme_event_free(struct ieee80211_sub_if_data *sdata,
	struct list_head *);

void ieee80211_ap_sme_event_init(struct ieee80211_sub_if_data *sdata);

int ieee80211_ap_sme_queue_event(struct ieee80211_sub_if_data *sdata,
											  enum ap_sme_event_id event_id,u8* addr);
int ieee80211_ap_sme_rx_mgmt(struct ieee80211_sub_if_data *sdata,
					   struct sk_buff *skb);
int ieee80211_ap_sme_paras_ap_info(struct ieee80211_sub_if_data *sdata,
	struct beacon_parameters *ap_info);
int ieee80211_ap_sme_tx_mgmt_status(struct ieee80211_sub_if_data *sdata,
					   struct sk_buff *skb);

void ieee80211_ap_sme_sta_session_timer_init(struct sta_info *sta);
u16 ieee80211_ap_sme_free_aid(struct ieee80211_sub_if_data *sdata, struct sta_info *sta);
void ieee80211_ap_sme_sta_sync_lock(struct ieee80211_sub_if_data *sdata);
void ieee80211_ap_sme_sta_sync_unlock(struct ieee80211_sub_if_data *sdata);
void ieee80211_ap_sme_mlme_purge(struct ieee80211_sub_if_data *sdata);
void ieee80211_ap_sme_event_purge(struct ieee80211_sub_if_data *sdata);

#define IEEE80211_AP_SME_STA_LOCK

#endif
#ifdef CONFIG_IEEE80211_SPECIAL_FILTER
void ieee80211_special_filter_init(struct ieee80211_sub_if_data *sdata);
void ieee80211_special_filter_exit(struct ieee80211_sub_if_data *sdata);
bool ieee80211_special_filter_register(struct ieee80211_sub_if_data *sdata,
					struct ieee80211_special_filter *filter);
bool ieee80211_special_filter_clear(struct ieee80211_sub_if_data *sdata);
bool ieee80211_special_filter_request(struct ieee80211_sub_if_data *sdata,
		struct ieee80211_special_filter_table *tables);
void ieee80211_special_check_package(struct ieee80211_local *local,struct sk_buff *skb);

#else
static inline void ieee80211_special_filter_init(struct ieee80211_sub_if_data *sdata) {}
static inline void ieee80211_special_filter_exit(struct ieee80211_sub_if_data *sdata) {}
#endif

#ifdef CONFIG_ATBM_RADAR_DETECT
#ifdef CONFIG_ATBM_5G_PRETEND_2G
void ieee80211_dfs_cac_timer_work(struct atbm_work_struct *work);
void ieee80211_dfs_cac_abort(struct ieee80211_sub_if_data *sdata);
#endif
#endif
void ieee80211_ap_channel_event_work(struct atbm_work_struct *work);

void ieee80211_medium_traffic_start(struct ieee80211_sub_if_data *sdata);
void ieee80211_medium_traffic_concle(struct ieee80211_sub_if_data *sdata);
#ifdef CONFIG_ATBM_STA_DYNAMIC_PS
void ieee80211_start_ps_recal_work(struct ieee80211_sub_if_data *sdata);
#endif

#endif /* IEEE80211_I_H */
