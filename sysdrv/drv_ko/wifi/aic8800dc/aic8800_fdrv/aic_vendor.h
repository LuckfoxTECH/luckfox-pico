#ifndef _AIC_VENDOR_H
#define _AIC_VENDOR_H

#include <linux/types.h>

#define GOOGLE_OUI     0x001A11
#define BRCM_OUI       0x001018

typedef enum {
	START_MKEEP_ALIVE,
	STOP_MKEEP_ALIVE,
} GetCmdType;

typedef enum {
	/* don't use 0 as a valid subcommand */
	VENDOR_NL80211_SUBCMD_UNSPECIFIED,

	/* define all vendor startup commands between 0x0 and 0x0FFF */
	VENDOR_NL80211_SUBCMD_RANGE_START = 0x0001,
	VENDOR_NL80211_SUBCMD_RANGE_END   = 0x0FFF,

	/* define all GScan related commands between 0x1000 and 0x10FF */
	ANDROID_NL80211_SUBCMD_GSCAN_RANGE_START = 0x1000,
	ANDROID_NL80211_SUBCMD_GSCAN_RANGE_END   = 0x10FF,

	/* define all NearbyDiscovery related commands between 0x1100 and 0x11FF */
	ANDROID_NL80211_SUBCMD_NBD_RANGE_START = 0x1100,
	ANDROID_NL80211_SUBCMD_NBD_RANGE_END   = 0x11FF,

	/* define all RTT related commands between 0x1100 and 0x11FF */
	ANDROID_NL80211_SUBCMD_RTT_RANGE_START = 0x1100,
	ANDROID_NL80211_SUBCMD_RTT_RANGE_END   = 0x11FF,

	ANDROID_NL80211_SUBCMD_LSTATS_RANGE_START = 0x1200,
	ANDROID_NL80211_SUBCMD_LSTATS_RANGE_END   = 0x12FF,

	/* define all Logger related commands between 0x1400 and 0x14FF */
	ANDROID_NL80211_SUBCMD_DEBUG_RANGE_START = 0x1400,
	ANDROID_NL80211_SUBCMD_DEBUG_RANGE_END   = 0x14FF,

	/* define all wifi offload related commands between 0x1600 and 0x16FF */
	ANDROID_NL80211_SUBCMD_WIFI_OFFLOAD_RANGE_START = 0x1600,
	ANDROID_NL80211_SUBCMD_WIFI_OFFLOAD_RANGE_END   = 0x16FF,

	/* define all NAN related commands between 0x1700 and 0x17FF */
	ANDROID_NL80211_SUBCMD_NAN_RANGE_START = 0x1700,
	ANDROID_NL80211_SUBCMD_NAN_RANGE_END   = 0x17FF,

	/* define all Android Packet Filter related commands between 0x1800 and 0x18FF */
	ANDROID_NL80211_SUBCMD_PKT_FILTER_RANGE_START = 0x1800,
	ANDROID_NL80211_SUBCMD_PKT_FILTER_RANGE_END   = 0x18FF,

	/* This is reserved for future usage */

} ANDROID_VENDOR_SUB_COMMAND;

typedef enum {
	WIFI_OFFLOAD_SUBCMD_START_MKEEP_ALIVE = ANDROID_NL80211_SUBCMD_WIFI_OFFLOAD_RANGE_START,
	WIFI_OFFLOAD_SUBCMD_STOP_MKEEP_ALIVE,
} WIFI_OFFLOAD_SUB_COMMAND;


enum mkeep_alive_attributes {
	MKEEP_ALIVE_ATTRIBUTE_ID = 0x1,
	MKEEP_ALIVE_ATTRIBUTE_IP_PKT,
	MKEEP_ALIVE_ATTRIBUTE_IP_PKT_LEN,
	MKEEP_ALIVE_ATTRIBUTE_SRC_MAC_ADDR,
	MKEEP_ALIVE_ATTRIBUTE_DST_MAC_ADDR,
	MKEEP_ALIVE_ATTRIBUTE_PERIOD_MSEC,
	MKEEP_ALIVE_ATTRIBUTE_AFTER_LAST,
	MKEEP_ALIVE_ATTRIBUTE_MAX = MKEEP_ALIVE_ATTRIBUTE_AFTER_LAST - 1
};

enum debug_sub_command {
	LOGGER_START_LOGGING = ANDROID_NL80211_SUBCMD_DEBUG_RANGE_START,
	LOGGER_TRIGGER_MEM_DUMP,
	LOGGER_GET_MEM_DUMP,
	LOGGER_GET_VER,
	LOGGER_GET_RING_STATUS,
	LOGGER_GET_RING_DATA,
	LOGGER_GET_FEATURE,
	LOGGER_RESET_LOGGING,
	LOGGER_TRIGGER_DRIVER_MEM_DUMP,
	LOGGER_GET_DRIVER_MEM_DUMP,
	LOGGER_START_PKT_FATE_MONITORING,
	LOGGER_GET_TX_PKT_FATES,
	LOGGER_GET_RX_PKT_FATES,
	LOGGER_GET_WAKE_REASON_STATS,
	LOGGER_DEBUG_GET_DUMP,
	LOGGER_FILE_DUMP_DONE_IND,
	LOGGER_SET_HAL_START,
	LOGGER_HAL_STOP,
	LOGGER_SET_HAL_PID,
};

enum logger_attributes {
	LOGGER_ATTRIBUTE_INVALID = 0,
	LOGGER_ATTRIBUTE_DRIVER_VER,
	LOGGER_ATTRIBUTE_FW_VER,
	LOGGER_ATTRIBUTE_RING_ID,
	LOGGER_ATTRIBUTE_RING_NAME,
	LOGGER_ATTRIBUTE_RING_FLAGS,
	LOGGER_ATTRIBUTE_LOG_LEVEL,
	LOGGER_ATTRIBUTE_LOG_TIME_INTVAL,
	LOGGER_ATTRIBUTE_LOG_MIN_DATA_SIZE,
	LOGGER_ATTRIBUTE_FW_DUMP_LEN,
	LOGGER_ATTRIBUTE_FW_DUMP_DATA,
	// LOGGER_ATTRIBUTE_FW_ERR_CODE,
	LOGGER_ATTRIBUTE_RING_DATA,
	LOGGER_ATTRIBUTE_RING_STATUS,
	LOGGER_ATTRIBUTE_RING_NUM,
	LOGGER_ATTRIBUTE_DRIVER_DUMP_LEN,
	LOGGER_ATTRIBUTE_DRIVER_DUMP_DATA,
	LOGGER_ATTRIBUTE_PKT_FATE_NUM,
	LOGGER_ATTRIBUTE_PKT_FATE_DATA,
	LOGGER_ATTRIBUTE_AFTER_LAST,
	LOGGER_ATTRIBUTE_MAX = LOGGER_ATTRIBUTE_AFTER_LAST - 1,
};

enum wifi_sub_command {
	GSCAN_SUBCMD_GET_CAPABILITIES = ANDROID_NL80211_SUBCMD_GSCAN_RANGE_START,
	GSCAN_SUBCMD_SET_CONFIG,                            /* 0x1001 */
	GSCAN_SUBCMD_SET_SCAN_CONFIG,                       /* 0x1002 */
	GSCAN_SUBCMD_ENABLE_GSCAN,                          /* 0x1003 */
	GSCAN_SUBCMD_GET_SCAN_RESULTS,                      /* 0x1004 */
	GSCAN_SUBCMD_SCAN_RESULTS,                          /* 0x1005 */
	GSCAN_SUBCMD_SET_HOTLIST,                           /* 0x1006 */
	GSCAN_SUBCMD_SET_SIGNIFICANT_CHANGE_CONFIG,         /* 0x1007 */
	GSCAN_SUBCMD_ENABLE_FULL_SCAN_RESULTS,              /* 0x1008 */
	GSCAN_SUBCMD_GET_CHANNEL_LIST,                      /* 0x1009 */
	WIFI_SUBCMD_GET_FEATURE_SET,                        /* 0x100A */
	WIFI_SUBCMD_GET_FEATURE_SET_MATRIX,                 /* 0x100B */
	WIFI_SUBCMD_SET_PNO_RANDOM_MAC_OUI,                 /* 0x100C */
	WIFI_SUBCMD_NODFS_SET,                              /* 0x100D */
	WIFI_SUBCMD_SET_COUNTRY_CODE,                       /* 0x100E */
	/* Add more sub commands here */
	GSCAN_SUBCMD_SET_EPNO_SSID,                         /* 0x100F */
	WIFI_SUBCMD_SET_SSID_WHITE_LIST,                    /* 0x1010 */
	WIFI_SUBCMD_SET_ROAM_PARAMS,                        /* 0x1011 */
	WIFI_SUBCMD_ENABLE_LAZY_ROAM,                       /* 0x1012 */
	WIFI_SUBCMD_SET_BSSID_PREF,                         /* 0x1013 */
	WIFI_SUBCMD_SET_BSSID_BLACKLIST,                    /* 0x1014 */
	GSCAN_SUBCMD_ANQPO_CONFIG,                          /* 0x1015 */
	WIFI_SUBCMD_SET_RSSI_MONITOR,                       /* 0x1016 */
	WIFI_SUBCMD_CONFIG_ND_OFFLOAD,                      /* 0x1017 */
	/* Add more sub commands here */
	GSCAN_SUBCMD_MAX,
	APF_SUBCMD_GET_CAPABILITIES = ANDROID_NL80211_SUBCMD_PKT_FILTER_RANGE_START,
	APF_SUBCMD_SET_FILTER,
};

enum gscan_attributes {
	GSCAN_ATTRIBUTE_NUM_BUCKETS = 10,
	GSCAN_ATTRIBUTE_BASE_PERIOD,
	GSCAN_ATTRIBUTE_BUCKETS_BAND,
	GSCAN_ATTRIBUTE_BUCKET_ID,
	GSCAN_ATTRIBUTE_BUCKET_PERIOD,
	GSCAN_ATTRIBUTE_BUCKET_NUM_CHANNELS,
	GSCAN_ATTRIBUTE_BUCKET_CHANNELS,
	GSCAN_ATTRIBUTE_NUM_AP_PER_SCAN,
	GSCAN_ATTRIBUTE_REPORT_THRESHOLD,
	GSCAN_ATTRIBUTE_NUM_SCANS_TO_CACHE,
	GSCAN_ATTRIBUTE_BAND = GSCAN_ATTRIBUTE_BUCKETS_BAND,

	GSCAN_ATTRIBUTE_ENABLE_FEATURE = 20,
	GSCAN_ATTRIBUTE_SCAN_RESULTS_COMPLETE,              /* indicates no more results */
	GSCAN_ATTRIBUTE_FLUSH_FEATURE,                      /* Flush all the configs */
	GSCAN_ENABLE_FULL_SCAN_RESULTS,
	GSCAN_ATTRIBUTE_REPORT_EVENTS,

	/* remaining reserved for additional attributes */
	GSCAN_ATTRIBUTE_NUM_OF_RESULTS = 30,
	GSCAN_ATTRIBUTE_FLUSH_RESULTS,
	GSCAN_ATTRIBUTE_SCAN_RESULTS,                       /* flat array of wifi_scan_result */
	GSCAN_ATTRIBUTE_SCAN_ID,                            /* indicates scan number */
	GSCAN_ATTRIBUTE_SCAN_FLAGS,                         /* indicates if scan was aborted */
	GSCAN_ATTRIBUTE_AP_FLAGS,                           /* flags on significant change event */
	GSCAN_ATTRIBUTE_NUM_CHANNELS,
	GSCAN_ATTRIBUTE_CHANNEL_LIST,
	GSCAN_ATTRIBUTE_CH_BUCKET_BITMASK,

	GSCAN_ATTRIBUTE_AFTER_LAST,
	GSCAN_ATTRIBUTE_MAX = GSCAN_ATTRIBUTE_AFTER_LAST - 1,
};

enum andr_wifi_attributes {
	ANDR_WIFI_ATTRIBUTE_NUM_FEATURE_SET,
	ANDR_WIFI_ATTRIBUTE_FEATURE_SET,
	ANDR_WIFI_ATTRIBUTE_PNO_RANDOM_MAC_OUI,
	ANDR_WIFI_ATTRIBUTE_NODFS_SET,
	ANDR_WIFI_ATTRIBUTE_COUNTRY,
	ANDR_WIFI_ATTRIBUTE_ND_OFFLOAD_VALUE,
	// Add more attribute here
	ANDR_WIFI_ATTRIBUTE_AFTER_LAST,
	ANDR_WIFI_ATTRIBUTE_MAX = ANDR_WIFI_ATTRIBUTE_AFTER_LAST - 1,
};

enum wifi_support_feature {
	/* Feature enums */
	WIFI_FEATURE_INFRA              = 0x0001,      /* Basic infrastructure mode        */
	WIFI_FEATURE_INFRA_5G           = 0x0002,      /* Support for 5, GHz Band          */
	WIFI_FEATURE_HOTSPOT            = 0x0004,      /* Support for GAS/ANQP             */
	WIFI_FEATURE_P2P                = 0x0008,      /* Wifi-Direct                      */
	WIFI_FEATURE_SOFT_AP            = 0x0010,      /* Soft AP                          */
	WIFI_FEATURE_GSCAN              = 0x0020,      /* Google-Scan APIs                 */
	WIFI_FEATURE_NAN                = 0x0040,      /* Neighbor Awareness Networking    */
	WIFI_FEATURE_D2D_RTT            = 0x0080,      /* Device-to-device RTT             */
	WIFI_FEATURE_D2AP_RTT           = 0x0100,      /* Device-to-AP RTT                 */
	WIFI_FEATURE_BATCH_SCAN         = 0x0200,      /* Batched Scan (legacy)            */
	WIFI_FEATURE_PNO                = 0x0400,      /* Preferred network offload        */
	WIFI_FEATURE_ADDITIONAL_STA     = 0x0800,      /* Support for two STAs             */
	WIFI_FEATURE_TDLS               = 0x1000,      /* Tunnel directed link setup       */
	WIFI_FEATURE_TDLS_OFFCHANNEL    = 0x2000,      /* Support for TDLS off channel     */
	WIFI_FEATURE_EPR                = 0x4000,      /* Enhanced power reporting         */
	WIFI_FEATURE_AP_STA             = 0x8000,      /* Support for AP STA Concurrency   */
	WIFI_FEATURE_LINK_LAYER_STATS   = 0x10000,     /* Support for Linkstats            */
	WIFI_FEATURE_LOGGER             = 0x20000,     /* WiFi Logger                      */
	WIFI_FEATURE_HAL_EPNO           = 0x40000,     /* WiFi PNO enhanced                */
	WIFI_FEATURE_RSSI_MONITOR       = 0x80000,     /* RSSI Monitor                     */
	WIFI_FEATURE_MKEEP_ALIVE        = 0x100000,    /* WiFi mkeep_alive                 */
	WIFI_FEATURE_CONFIG_NDO         = 0x200000,    /* ND offload configure             */
	WIFI_FEATURE_TX_TRANSMIT_POWER  = 0x400000,    /* Capture Tx transmit power levels */
	WIFI_FEATURE_CONTROL_ROAMING    = 0x800000,    /* Enable/Disable firmware roaming  */
	WIFI_FEATURE_IE_WHITELIST       = 0x1000000,   /* Support Probe IE white listing   */
	WIFI_FEATURE_SCAN_RAND          = 0x2000000,   /* Support MAC & Probe Sequence Number randomization */
	WIFI_FEATURE_INVALID            = 0xFFFFFFFF,  /* Invalid Feature                  */
};

enum wifi_logger_feature {
	WIFI_LOGGER_MEMORY_DUMP_SUPPORTED = (1 << (0)),             // Memory dump of FW
	WIFI_LOGGER_PER_PACKET_TX_RX_STATUS_SUPPORTED = (1 << (1)), // PKT status
	WIFI_LOGGER_CONNECT_EVENT_SUPPORTED = (1 << (2)),           // Connectivity event
	WIFI_LOGGER_POWER_EVENT_SUPPORTED = (1 << (3)),             // POWER of Driver
	WIFI_LOGGER_WAKE_LOCK_SUPPORTED = (1 << (4)),               // WAKE LOCK of Driver
	WIFI_LOGGER_VERBOSE_SUPPORTED = (1 << (5)),                 // verbose log of FW
	WIFI_LOGGER_WATCHDOG_TIMER_SUPPORTED = (1 << (6)),          // monitor the health of FW
	WIFI_LOGGER_DRIVER_DUMP_SUPPORTED = (1 << (7)),             // dumps driver state
	WIFI_LOGGER_PACKET_FATE_SUPPORTED = (1 << (8)),             // tracks connection packets' fate
};

enum wake_stats_attributes {
	WAKE_STAT_ATTRIBUTE_TOTAL_CMD_EVENT,
	WAKE_STAT_ATTRIBUTE_CMD_EVENT_WAKE,
	WAKE_STAT_ATTRIBUTE_CMD_EVENT_COUNT,
	WAKE_STAT_ATTRIBUTE_CMD_COUNT_USED,
	WAKE_STAT_ATTRIBUTE_TOTAL_DRIVER_FW,
	WAKE_STAT_ATTRIBUTE_DRIVER_FW_WAKE,
	WAKE_STAT_ATTRIBUTE_DRIVER_FW_COUNT,
	WAKE_STAT_ATTRIBUTE_DRIVER_FW_COUNT_USED,
	WAKE_STAT_ATTRIBUTE_TOTAL_RX_DATA_WAKE,
	WAKE_STAT_ATTRIBUTE_RX_UNICAST_COUNT,
	WAKE_STAT_ATTRIBUTE_RX_MULTICAST_COUNT,
	WAKE_STAT_ATTRIBUTE_RX_BROADCAST_COUNT,
	WAKE_STAT_ATTRIBUTE_RX_ICMP_PKT,
	WAKE_STAT_ATTRIBUTE_RX_ICMP6_PKT,
	WAKE_STAT_ATTRIBUTE_RX_ICMP6_RA,
	WAKE_STAT_ATTRIBUTE_RX_ICMP6_NA,
	WAKE_STAT_ATTRIBUTE_RX_ICMP6_NS,
	WAKE_STAT_ATTRIBUTE_IPV4_RX_MULTICAST_ADD_CNT,
	WAKE_STAT_ATTRIBUTE_IPV6_RX_MULTICAST_ADD_CNT,
	WAKE_STAT_ATTRIBUTE_OTHER__RX_MULTICAST_ADD_CNT,
	WAKE_STAT_ATTRIBUTE_RX_MULTICAST_PKT_INFO,
	WAKE_STAT_ATTRIBUTE_AFTER_LAST,
	WAKE_STAT_ATTRIBUTE_MAX = WAKE_STAT_ATTRIBUTE_AFTER_LAST - 1,
};

enum vendor_nl80211_subcmd {
	/* copied from wpa_supplicant brcm definations */
	VENDOR_NL80211_SUBCMD_UNSPEC  = 0,
	VENDOR_NL80211_SUBCMD_SET_PMK = 4,
	VENDOR_NL80211_SUBCMD_SET_MAC = 6,
	VENDOR_NL80211_SCMD_ACS       = 9,
	VENDOR_NL80211_SCMD_MAX       = 10,
};

enum nl80211_vendor_subcmd_attributes {
	WIFI_VENDOR_ATTR_DRIVER_CMD        = 0,
	WIFI_VENDOR_ATTR_DRIVER_KEY_PMK    = 1,
	WIFI_VENDOR_ATTR_DRIVER_MAC_ADDR   = 3,
	WIFI_VENDOR_ATTR_DRIVER_AFTER_LAST = 5,
	WIFI_VENDOR_ATTR_DRIVER_MAX        =
	WIFI_VENDOR_ATTR_DRIVER_AFTER_LAST - 1,
};

typedef int wifi_ring_buffer_id;

struct wifi_ring_buffer_status {
	u8 name[32];
	u32 flags;
	wifi_ring_buffer_id ring_id;
	u32 ring_buffer_byte_size;
	u32 verbose_level;
	u32 written_bytes;
	u32 read_bytes;
	u32 written_records;
};

struct rx_data_cnt_details_t {
	int rx_unicast_cnt;     /*Total rx unicast packet which woke up host */
	int rx_multicast_cnt;   /*Total rx multicast packet which woke up host */
	int rx_broadcast_cnt;   /*Total rx broadcast packet which woke up host */
};

struct rx_wake_pkt_type_classification_t {
	int icmp_pkt;   /*wake icmp packet count */
	int icmp6_pkt;  /*wake icmp6 packet count */
	int icmp6_ra;   /*wake icmp6 RA packet count */
	int icmp6_na;   /*wake icmp6 NA packet count */
	int icmp6_ns;   /*wake icmp6 NS packet count */
	//ToDo: Any more interesting classification to add?
};

struct rx_multicast_cnt_t{
	int ipv4_rx_multicast_addr_cnt; /*Rx wake packet was ipv4 multicast */
	int ipv6_rx_multicast_addr_cnt; /*Rx wake packet was ipv6 multicast */
	int other_rx_multicast_addr_cnt;/*Rx wake packet was non-ipv4 and non-ipv6*/
};

struct wlan_driver_wake_reason_cnt_t {
	int total_cmd_event_wake;          /* Total count of cmd event wakes */
	int *cmd_event_wake_cnt;           /* Individual wake count array, each index a reason */
	int cmd_event_wake_cnt_sz;         /* Max number of cmd event wake reasons */
	int cmd_event_wake_cnt_used;       /* Number of cmd event wake reasons specific to the driver */

	int total_driver_fw_local_wake;    /* Total count of drive/fw wakes, for local reasons */
	int *driver_fw_local_wake_cnt;     /* Individual wake count array, each index a reason */
	int driver_fw_local_wake_cnt_sz;   /* Max number of local driver/fw wake reasons */
	int driver_fw_local_wake_cnt_used; /* Number of local driver/fw wake reasons specific to the driver */

	int total_rx_data_wake;            /* total data rx packets, that woke up host */
	struct rx_data_cnt_details_t rx_wake_details;
	struct rx_wake_pkt_type_classification_t rx_wake_pkt_classification_info;
	struct rx_multicast_cnt_t rx_multicast_wake_pkt_info;
};

typedef struct wl_mkeep_alive_pkt {
	u16 version;       /* Version for mkeep_alive */
	u16 length;        /* length of fixed parameters in the structure */
	u32 period_msec;   /* high bit on means immediate send */
	u16 len_bytes;
	u8  keep_alive_id; /* 0 - 3 for N = 4 */
	u8  data[1];
} wl_mkeep_alive_pkt_t;

#endif /* _AIC_VENDOR_H */

