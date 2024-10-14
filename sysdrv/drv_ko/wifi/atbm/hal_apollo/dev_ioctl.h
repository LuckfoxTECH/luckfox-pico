#ifndef __DEV_IOCTL_H__
#define __DEV_IOCTL_H__


enum atbm_msg_type{
    ATBM_DEV_IO_GET_STA_STATUS   = 0,
    ATBM_DEV_IO_GET_STA_RSSI     = 1, //STA connected AP's RSSI
    ATBM_DEV_IO_GET_AP_INFO      = 2,  //STA or AP
    ATBM_DEV_IO_GET_STA_INFO     = 3,
    ATBM_DEV_IO_SET_STA_SCAN     = 4,
    ATBM_DEV_IO_SET_FREQ         = 5,
    ATBM_DEV_IO_SET_SPECIAL_OUI  = 6, //use for Beacon and Probe package
    ATBM_DEV_IO_SET_STA_DIS      = 7,
    ATBM_DEV_IO_SET_IF_TYPE      = 8,
    ATBM_DEV_IO_SET_ADAPTIVE     = 9,
    ATBM_DEV_IO_SET_TXPWR_DCXO   = 10,
    ATBM_DEV_IO_SET_TXPWR        = 11,
    ATBM_DEV_IO_GET_WORK_CHANNEL   = 12,
    ATBM_DEV_IO_SET_BEST_CHANNEL_SCAN   = 13,
    ATBM_DEV_IO_GET_AP_LIST   = 14,
    ATBM_DEV_IO_GET_TP_RATE   = 15,
    ATBM_DEV_IO_ETF_TEST   = 16,
    ATBM_DEV_IO_ETF_GET_RESULT  = 17,
    ATBM_DEV_IO_ETF_START_TX	= 18,
    ATBM_DEV_IO_ETF_STOP_TX		= 19,
    ATBM_DEV_IO_ETF_START_RX	= 20,
    ATBM_DEV_IO_ETF_STOP_RX		= 21,
    ATBM_DEV_IO_FIX_TX_RATE		 = 22,
    ATBM_DEV_IO_MAX_TX_RATE		 = 23,
    ATBM_DEV_IO_TX_RATE_FREE	 = 24,
    ATBM_DEV_IO_SET_EFUSE_MAC    = 25,
	ATBM_DEV_IO_SET_EFUSE_DCXO   = 26,
	ATBM_DEV_IO_SET_EFUSE_DELTAGAIN = 27,
	ATBM_DEV_IO_MIN_TX_RATE		 = 28,
	ATBM_DEV_IO_SET_RATE_POWER	 = 29,
#ifdef CONFIG_IEEE80211_SPECIAL_FILTER

	ATBM_DEV_IO_SET_SPECIAL_FILTER = 30,
#endif

	ATBM_DEV_IO_SET_COUNTRY_CODE = 31,

	ATBM_DEV_IO_GET_DRIVER_VERSION = 32,
	ATBM_DEV_IO_GET_EFUSE			= 33,
	ATBM_DEV_IO_GET_ETF_START_RX_RESULTS = 34,
	ATBM_DEV_IO_SET_UPERR_PROCESS_PID = 35,
#ifdef CONFIG_ATBM_SUPPORT_AP_CONFIG
	ATBM_DEV_IO_SET_FIX_SCAN_CHANNEL = 36,
#endif
	ATBM_DEV_IO_SET_AUTO_CALI_PPM	= 37,
	ATBM_DEV_IO_GET_CALI_REAULTS	= 38,
	ATBM_DEV_IO_SET_EFUSE_GAIN_COMPENSATION_VALUE = 39,
	ATBM_DEV_IO_GET_VENDOR_SPECIAL_IE	= 40,
#ifdef CONFIG_ATBM_STA_LISTEN
	ATBM_DEV_IO_SET_STA_LISTEN_CHANNEL = 41,
#endif
#ifdef CONFIG_JUAN_MISC

	ATBM_DEV_IO_SET_AP_TIM_CONTROL = 42,
	ATBM_DEV_IO_GET_AP_TIM_CONTROL = 43,
#endif

};

#define WSM_MAX_NUM_LINK_AP 14//Lmac support station number is 4;
typedef struct _atbm_wifi_ap_info_{
    int wext_rssi;
    unsigned long rx_packets;
    unsigned long tx_packets;
    unsigned long tx_retry_count;
    int last_rx_rate_idx;
    unsigned char  wext_mac[ETH_ALEN];
    unsigned char  sta_cnt;
}atbm_wifi_ap_info;

typedef struct _atbm_wifi_sta_info_{
    int rssi;
    unsigned long rx_packets;
    unsigned long tx_packets;
    unsigned long tx_retry_count;
    u8  bssid[ETH_ALEN];
    u8  ssid[IEEE80211_MAX_SSID_LEN];
    size_t ssid_len;
}atbm_wifi_sta_info;


struct altm_wext_msg{
    int type;
    int value;
    char externData[256];
};

typedef struct dev_ioctl_cmd {
	const int msg_type;
	int (*func)(struct net_device *dev, struct altm_wext_msg *msg);
	const char *uage;
}dev_ioctl_cmd_t;



#endif
