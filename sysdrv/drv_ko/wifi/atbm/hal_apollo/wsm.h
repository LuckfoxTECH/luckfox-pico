/*
 * WSM host interface (HI) interface for APOLLO mac80211 drivers
 *
 * Copyright (c) 2016, altobeam
 * Author:
 *
 * Based on CW1200 UMAC WSM API, which is
 * Copyright (C) ST-Ericsson SA 2010
 * Author: Stewart Mathers <stewart.mathers@stericsson.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef ATBM_APOLLO_WSM_H_INCLUDED
#define ATBM_APOLLO_WSM_H_INCLUDED

#include <linux/spinlock.h>

struct atbm_common;

/* Bands */
/* Radio band 2.412 -2.484 GHz. */
#define WSM_PHY_BAND_2_4G		(0)

/* Radio band 4.9375-5.8250 GHz. */
#define WSM_PHY_BAND_5G			(1)

/* Transmit rates */
/* 1   Mbps            ERP-DSSS */
#define WSM_TRANSMIT_RATE_1		(0)

/* 2   Mbps            ERP-DSSS */
#define WSM_TRANSMIT_RATE_2		(1)

/* 5.5 Mbps            ERP-CCK, ERP-PBCC (Not supported) */
 #define WSM_TRANSMIT_RATE_5		(2) 

/* 11  Mbps            ERP-CCK, ERP-PBCC (Not supported) */
 #define WSM_TRANSMIT_RATE_11		(3) 

/* 22  Mbps            ERP-PBCC (Not supported) */
/* #define WSM_TRANSMIT_RATE_22		(4) */

/* 33  Mbps            ERP-PBCC (Not supported) */
/* #define WSM_TRANSMIT_RATE_33		(5) */

/* 6   Mbps   (3 Mbps) ERP-OFDM, BPSK coding rate 1/2 */
#define WSM_TRANSMIT_RATE_6		(6)

/* 9   Mbps (4.5 Mbps) ERP-OFDM, BPSK coding rate 3/4 */
#define WSM_TRANSMIT_RATE_9		(7)

/* 12  Mbps  (6 Mbps)  ERP-OFDM, QPSK coding rate 1/2 */
#define WSM_TRANSMIT_RATE_12		(8)

/* 18  Mbps  (9 Mbps)  ERP-OFDM, QPSK coding rate 3/4 */
#define WSM_TRANSMIT_RATE_18		(9)

/* 24  Mbps (12 Mbps)  ERP-OFDM, 16QAM coding rate 1/2 */
#define WSM_TRANSMIT_RATE_24		(10)

/* 36  Mbps (18 Mbps)  ERP-OFDM, 16QAM coding rate 3/4 */
#define WSM_TRANSMIT_RATE_36		(11)

/* 48  Mbps (24 Mbps)  ERP-OFDM, 64QAM coding rate 1/2 */
#define WSM_TRANSMIT_RATE_48		(12)

/* 54  Mbps (27 Mbps)  ERP-OFDM, 64QAM coding rate 3/4 */
#define WSM_TRANSMIT_RATE_54		(13)

/* 6.5 Mbps            HT-OFDM, BPSK coding rate 1/2 */
#define WSM_TRANSMIT_RATE_HT_6		(14)

/* 13  Mbps            HT-OFDM, QPSK coding rate 1/2 */
#define WSM_TRANSMIT_RATE_HT_13		(15)

/* 19.5 Mbps           HT-OFDM, QPSK coding rate 3/4 */
#define WSM_TRANSMIT_RATE_HT_19		(16)

/* 26  Mbps            HT-OFDM, 16QAM coding rate 1/2 */
#define WSM_TRANSMIT_RATE_HT_26		(17)

/* 39  Mbps            HT-OFDM, 16QAM coding rate 3/4 */
#define WSM_TRANSMIT_RATE_HT_39		(18)

/* 52  Mbps            HT-OFDM, 64QAM coding rate 2/3 */
#define WSM_TRANSMIT_RATE_HT_52		(19)

/* 58.5 Mbps           HT-OFDM, 64QAM coding rate 3/4 */
#define WSM_TRANSMIT_RATE_HT_58		(20)

/* 65  Mbps            HT-OFDM, 64QAM coding rate 5/6 */
#define WSM_TRANSMIT_RATE_HT_65		(21)

/* Scan types */
/* Foreground scan */
#define WSM_SCAN_TYPE_FOREGROUND	(0)

/* Background scan */
#define WSM_SCAN_TYPE_BACKGROUND	(1)

/* Auto scan */
#define WSM_SCAN_TYPE_AUTO		(2)

/* Scan flags */
/* Forced background scan means if the station cannot */
/* enter the power-save mode, it shall force to perform a */
/* background scan. Only valid when ScanType is */
/* background scan. */
#define WSM_SCAN_FLAG_FORCE_BACKGROUND	(BIT(0))

/* The WLAN device scans one channel at a time so */
/* that disturbance to the data traffic is minimized. */
#define WSM_SCAN_FLAG_SPLIT_METHOD	(BIT(1))

/* Preamble Type. Long if not set. */
#define WSM_SCAN_FLAG_SHORT_PREAMBLE	(BIT(2))

/* 11n Tx Mode. Mixed if not set. */
#define WSM_SCAN_FLAG_11N_GREENFIELD	(BIT(3))

#define WSM_FLAG_MAC_INSTANCE_1	(BIT(4))

#define WSM_FLAG_MAC_INSTANCE_0	(~(BIT(4)))

/* Scan constraints */
/* Maximum number of channels to be scanned. */
#define WSM_SCAN_MAX_NUM_OF_CHANNELS	(14)
#define WSM_FLAG_START_SMARTCONFIG    BIT(5)

#define WSM_FLAG_BEST_CHANNEL_START		BIT(6)
#define WSM_FLAG_BEST_CHANNEL_END		(~(BIT(6)))

/* The maximum number of SSIDs that the device can scan for. */
#define WSM_SCAN_MAX_NUM_OF_SSIDS	(2)
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
/* Transmit flags */
/* Start Expiry time from the receipt of tx request */
#define WSM_TX_FLAG_EXPIRY_TIME		(BIT(0))
#endif /*CONFIG_ATBM_APOLLO_TESTMODE*/

/* Power management modes */
/* 802.11 Active mode */
#define WSM_PSM_ACTIVE			(0)

/* 802.11 PS mode */
#define WSM_PSM_PS			BIT(0)

/* Fast Power Save bit */
#define WSM_PSM_FAST_PS_FLAG		BIT(7)

/* Dynamic aka Fast power save */
#define WSM_PSM_FAST_PS			(BIT(0) | BIT(7))

/* Undetermined */
/* Note : Undetermined status is reported when the */
/* NULL data frame used to advertise the PM mode to */
/* the AP at Pre or Post Background Scan is not Acknowledged */
#define WSM_PSM_UNKNOWN			BIT(1)

/* Queue IDs */
/* best effort/legacy */
#define WSM_QUEUE_BEST_EFFORT		(0)

/* background */
#define WSM_QUEUE_BACKGROUND		(1)

/* video */
#define WSM_QUEUE_VIDEO			(2)

/* voice */
#define WSM_QUEUE_VOICE			(3)

/* HT TX parameters */
/* Non-HT */
#define WSM_HT_TX_NON_HT		(0)

/* Mixed format */
#define WSM_HT_TX_MIXED			(1)

/* Greenfield format */
#define WSM_HT_TX_GREENFIELD		(2)

#define WSM_HT_TX_SGI				(4)
#ifdef ATBM_SUPPORT_WIDTH_40M

#define WSM_HT_TX_WIDTH_40M			(BIT(3))

#endif
#define WSM_HT_TX_USE_MINRATE		(BIT(4))
/* STBC allowed */
#define WSM_HT_TX_STBC			(BIT(7))
#define WSM_NEED_TX_CONFIRM		(BIT(8))

/* EPTA prioirty flags for BT Coex */
/* default epta priority */
#define WSM_EPTA_PRIORITY_DEFAULT	4
/* use for normal data */
#define WSM_EPTA_PRIORITY_DATA		4
/* use for connect/disconnect/roaming*/
#define WSM_EPTA_PRIORITY_MGT		5
/* use for action frames */
#define WSM_EPTA_PRIORITY_ACTION	5
/* use for AC_VI data */
#define WSM_EPTA_PRIORITY_VIDEO		5
/* use for AC_VO data */
#define WSM_EPTA_PRIORITY_VOICE		6
/* use for EAPOL exchange */
#define WSM_EPTA_PRIORITY_EAPOL		7

/* TX status */
/* Frame was sent aggregated */
/* Only valid for WSM_SUCCESS status. */
#define WSM_TX_STATUS_AGGREGATION	(BIT(0))

/* Host should requeue this frame later. */
/* Valid only when status is WSM_REQUEUE. */
#define WSM_TX_STATUS_REQUEUE		(BIT(1))

/* Normal Ack */
#define WSM_TX_STATUS_NORMAL_ACK	(0<<2)

/* No Ack */
#define WSM_TX_STATUS_NO_ACK		(1<<2)

/* No explicit acknowledgement */
#define WSM_TX_STATUS_NO_EXPLICIT_ACK	(2<<2)

/* Block Ack */
/* Only valid for WSM_SUCCESS status. */
#define WSM_TX_STATUS_BLOCK_ACK		(3<<2)

/* RX status */
/* Unencrypted */
#define WSM_RX_STATUS_UNENCRYPTED	(0<<0)

/* WEP */
#define WSM_RX_STATUS_WEP		(1<<0)

/* TKIP */
#define WSM_RX_STATUS_TKIP		(2<<0)

/* AES */
#define WSM_RX_STATUS_AES		(3<<0)

/* WAPI */
#define WSM_RX_STATUS_WAPI		(4<<0)

/* Macro to fetch encryption subfield. */
#define WSM_RX_STATUS_ENCRYPTION(status) ((status) & 0x07)

/* Frame was part of an aggregation */
#define WSM_RX_STATUS_AGGREGATE		(BIT(3))

/* Frame was first in the aggregation */
#define WSM_RX_STATUS_AGGREGATE_FIRST	(BIT(4))

/* Frame was last in the aggregation */
#define WSM_RX_STATUS_AGGREGATE_LAST	(BIT(5))

/* Indicates a defragmented frame */
#define WSM_RX_STATUS_DEFRAGMENTED	(BIT(6))

/* Indicates a Beacon frame */
#define WSM_RX_STATUS_BEACON		(BIT(7))

/* Indicates STA bit beacon TIM field */
#define WSM_RX_STATUS_TIM		(BIT(8))

/* Indicates Beacon frame's virtual bitmap contains multicast bit */
#define WSM_RX_STATUS_MULTICAST		(BIT(9))

/* Indicates frame contains a matching SSID */
#define WSM_RX_STATUS_MATCHING_SSID	(BIT(10))

/* Indicates frame contains a matching BSSI */
#define WSM_RX_STATUS_MATCHING_BSSI	(BIT(11))

/* Indicates More bit set in Framectl field */
#define WSM_RX_STATUS_SMARTCONFIG		(BIT(12))

/* Indicates frame received during a measurement process */
#define WSM_RX_STATUS_MEASUREMENT	(BIT(13))

/* Indicates frame received as an HT packet */
#define WSM_RX_STATUS_HT		(BIT(14))

/* Indicates frame received with STBC */
#define WSM_RX_STATUS_STBC		(BIT(15))

/* Indicates Address 1 field matches dot11StationId */
#define WSM_RX_STATUS_ADDRESS1		(BIT(16))

/* Indicates Group address present in the Address 1 field */
#define WSM_RX_STATUS_GROUP		(BIT(17))

/* Indicates Broadcast address present in the Address 1 field */
#define WSM_RX_STATUS_BROADCAST		(BIT(18))

/* Indicates group key used with encrypted frames */
#define WSM_RX_STATUS_GROUP_KEY		(BIT(19))
#define WSM_RX_STATUS_CHKSUM_ERROR (BIT(21))
/*indicates rx 40M*/
#define WSM_RX_STATUS_RX_40M		(BIT(22))
/* Macro to fetch encryption key index. */
#define WSM_RX_STATUS_KEY_IDX(status)	(((status >> 20)) & 0x0F)
#define WSM_RX_STATUS_SHORT_GI      (BIT(24))
#define WSM_RX_STATUS_FILTER_FRAME	(BIT(29))

/* Frame Control field starts at Frame offset + 2 */
#define WSM_TX_2BYTES_SHIFT		(BIT(7))

/* Join mode */
/* IBSS */
#define WSM_JOIN_MODE_IBSS		(0)

/* BSS */
#define WSM_JOIN_MODE_BSS		(1)

/* PLCP preamble type */
/* For long preamble */
#define WSM_JOIN_PREAMBLE_LONG		(0)

/* For short preamble (Long for 1Mbps) */
#define WSM_JOIN_PREAMBLE_SHORT		(1)

/* For short preamble (Long for 1 and 2Mbps) */
#define WSM_JOIN_PREAMBLE_SHORT_2	(2)

/* Join flags */
/* Unsynchronized */
#define WSM_JOIN_FLAGS_UNSYNCRONIZED	BIT(0)
/* The BSS owner is a P2P GO */
#define WSM_JOIN_FLAGS_P2P_GO		BIT(1)
/* Force to join BSS with the BSSID and the
 * SSID specified without waiting for beacons. The
 * ProbeForJoin parameter is ignored. */
#define WSM_JOIN_FLAGS_FORCE		BIT(2)
/* Give probe request/response higher
 * priority over the BT traffic */
#define WSM_JOIN_FLAGS_PRIO		BIT(3)

/* Key types */
#define WSM_KEY_TYPE_WEP_DEFAULT	(0)
#define WSM_KEY_TYPE_WEP_PAIRWISE	(1)
#define WSM_KEY_TYPE_TKIP_GROUP		(2)
#define WSM_KEY_TYPE_TKIP_PAIRWISE	(3)
#define WSM_KEY_TYPE_AES_GROUP		(4)
#define WSM_KEY_TYPE_AES_PAIRWISE	(5)
#define WSM_KEY_TYPE_WAPI_GROUP		(6)
#define WSM_KEY_TYPE_WAPI_PAIRWISE	(7)
#define WSM_KEY_TYPE_IGTK_GROUP	    (8)

/* Key indexes */
#define ATBMWIFI_WSM_KEY_MAX_INDEX	(15)
#define WSM_KEY_MAX_INDEX		(10)

/* ACK policy */
#define WSM_ACK_POLICY_NORMAL		(0)
#define WSM_ACK_POLICY_NO_ACK		(1)

/* Start modes */
#define WSM_START_MODE_AP		(0)	/* Mini AP */
#define WSM_START_MODE_P2P_GO		(1)	/* P2P GO */
#define WSM_START_MODE_P2P_DEV		(2)	/* P2P device */
#define WSM_START_MODE_MONITOR_DEV	(3)

/* SetAssociationMode MIB flags */
#define WSM_ASSOCIATION_MODE_USE_PREAMBLE_TYPE		(BIT(0))
#define WSM_ASSOCIATION_MODE_USE_HT_MODE		(BIT(1))
#define WSM_ASSOCIATION_MODE_USE_BASIC_RATE_SET		(BIT(2))
#define WSM_ASSOCIATION_MODE_USE_MPDU_START_SPACING	(BIT(3))
#define WSM_ASSOCIATION_MODE_SNOOP_ASSOC_FRAMES		(BIT(4))

/* RcpiRssiThreshold MIB flags */
#define WSM_RCPI_RSSI_THRESHOLD_ENABLE	(BIT(0))
#define WSM_RCPI_RSSI_USE_RSSI		(BIT(1))
#define WSM_RCPI_RSSI_DONT_USE_UPPER	(BIT(2))
#define WSM_RCPI_RSSI_DONT_USE_LOWER	(BIT(3))

/* Update-ie constants */
#define WSM_UPDATE_IE_BEACON		(BIT(0))
#define WSM_UPDATE_IE_PROBE_RESP	(BIT(1))
#define WSM_UPDATE_IE_PROBE_REQ		(BIT(2))

/* WSM events */
/* Error */
#define WSM_EVENT_ERROR			(0)

/* BSS lost */
#define WSM_EVENT_BSS_LOST		(1)

/* BSS regained */
#define WSM_EVENT_BSS_REGAINED		(2)

/* Radar detected */
#define WSM_EVENT_RADAR_DETECTED	(3)

/* RCPI or RSSI threshold triggered */
#define WSM_EVENT_RCPI_RSSI		(4)

/* BT inactive */
#define WSM_EVENT_BT_INACTIVE		(5)

/* BT active */
#define WSM_EVENT_BT_ACTIVE		(6)

#define WSM_EVENT_PS_MODE_ERROR         (7)

#define WSM_EVENT_INACTIVITY		(9)

/* MAC Addr Filter */
#define WSM_MIB_ID_MAC_ADDR_FILTER	0x1030

/* MIB IDs */
/* 4.1  dot11StationId */
#define WSM_MIB_ID_DOT11_STATION_ID		0x0000

/* 4.2  dot11MaxtransmitMsduLifeTime */
#define WSM_MIB_ID_DOT11_MAX_TRANSMIT_LIFTIME	0x0001

/* 4.3  dot11MaxReceiveLifeTime */
#define WSM_MIB_ID_DOT11_MAX_RECEIVE_LIFETIME	0x0002

/* 4.4  dot11SlotTime */
#define WSM_MIB_ID_DOT11_SLOT_TIME		0x0003

/* 4.5  dot11GroupAddressesTable */
#define WSM_MIB_ID_DOT11_GROUP_ADDRESSES_TABLE	0x0004
#define WSM_MAX_GRP_ADDRTABLE_ENTRIES		8

/* 4.6  dot11WepDefaultKeyId */
#define WSM_MIB_ID_DOT11_WEP_DEFAULT_KEY_ID	0x0005

/* 4.7  dot11CurrentTxPowerLevel */
#define WSM_MIB_ID_DOT11_CURRENT_TX_POWER_LEVEL	0x0006

/* 4.8  dot11RTSThreshold */
#define WSM_MIB_ID_DOT11_RTS_THRESHOLD		0x0007
#define WSM_MIB_ID_DOT11_TRANSMIT_RETRY_CNT		0x0008

/* 4.9  NonErpProtection */
#define WSM_MIB_ID_NON_ERP_PROTECTION		0x1000

/* 4.10 ArpIpAddressesTable */
#define WSM_MIB_ID_ARP_IP_ADDRESSES_TABLE	0x1001
#define WSM_MAX_ARP_IP_ADDRTABLE_ENTRIES	1

/* 4.11 TemplateFrame */
#define WSM_MIB_ID_TEMPLATE_FRAME		0x1002

/* 4.12 RxFilter */
#define WSM_MIB_ID_RX_FILTER			0x1003

/* 4.13 BeaconFilterTable */
#define WSM_MIB_ID_BEACON_FILTER_TABLE		0x1004

/* 4.14 BeaconFilterEnable */
#define WSM_MIB_ID_BEACON_FILTER_ENABLE		0x1005

/* 4.15 OperationalPowerMode */
#define WSM_MIB_ID_OPERATIONAL_POWER_MODE	0x1006

/* 4.16 BeaconWakeUpPeriod */
#define WSM_MIB_ID_BEACON_WAKEUP_PERIOD		0x1007

/* 4.17 RcpiRssiThreshold */
#define WSM_MIB_ID_RCPI_RSSI_THRESHOLD		0x1009

/* 4.18 StatisticsTable */
#define WSM_MIB_ID_STATISTICS_TABLE		0x100A

/* 4.19 IbssPsConfig */
#define WSM_MIB_ID_IBSS_PS_CONFIG		0x100B

/* 4.20 CountersTable */
#define WSM_MIB_ID_COUNTERS_TABLE		0x100C

/* 4.21 BlockAckPolicy */
#define WSM_MIB_ID_BLOCK_ACK_POLICY		0x100E

/* 4.22 OverrideInternalTxRate */
#define WSM_MIB_ID_OVERRIDE_INTERNAL_TX_RATE	0x100F

/* 4.23 SetAssociationMode */
#define WSM_MIB_ID_SET_ASSOCIATION_MODE		0x1010

/* 4.24 UpdateEptaConfigData */
#define WSM_MIB_ID_UPDATE_EPTA_CONFIG_DATA	0x1011

/* 4.25 SelectCcaMethod */
#define WSM_MIB_ID_SELECT_CCA_METHOD		0x1012

/* 4.26 SetUpasdInformation */
#define WSM_MIB_ID_SET_UAPSD_INFORMATION	0x1013

/* 4.27 SetAutoCalibrationMode  WBF00004073 */
#define WSM_MIB_ID_SET_AUTO_CALIBRATION_MODE	0x1015

/* 4.28 SetTxRateRetryPolicy */
#define WSM_MIB_ID_SET_TX_RATE_RETRY_POLICY	0x1016

/* 4.29 SetHostMessageTypeFilter */
#define WSM_MIB_ID_SET_HOST_MSG_TYPE_FILTER	0x1017

/* 4.30 P2PFindInfo */
#define WSM_MIB_ID_P2P_FIND_INFO		0x1018

/* 4.31 P2PPsModeInfo */
#define WSM_MIB_ID_P2P_PS_MODE_INFO		0x1019

/* 4.32 SetEtherTypeDataFrameFilter */
#define WSM_MIB_ID_SET_ETHERTYPE_DATAFRAME_FILTER 0x101A

/* 4.33 SetUDPPortDataFrameFilter */
#define WSM_MIB_ID_SET_UDPPORT_DATAFRAME_FILTER	0x101B

/* 4.34 SetMagicDataFrameFilter */
#define WSM_MIB_ID_SET_MAGIC_DATAFRAME_FILTER	0x101C

/* This is the end of specification. */

/* 4.35 P2PDeviceInfo */
#define WSM_MIB_ID_P2P_DEVICE_INFO		0x101D

/* 4.36 SetWCDMABand */
#define WSM_MIB_ID_SET_WCDMA_BAND		0x101E

/* 4.37 GroupTxSequenceCounter */
#define WSM_MIB_ID_GRP_SEQ_COUNTER		0x101F

/* 4.38 ProtectedMgmtPolicy */
#define WSM_MIB_ID_PROTECTED_MGMT_POLICY	0x1020

/* 4.39 SetHtProtection */
#define WSM_MIB_ID_SET_HT_PROTECTION		0x1021

/* 4.40 GPIO Command */
#define WSM_MIB_ID_GPIO_COMMAND			0x1022

/* 4.41 TSF Counter Value */
#define WSM_MIB_ID_TSF_COUNTER			0x1023

/* Test Purposes Only */
#define WSM_MIB_ID_BLOCK_ACK_INFO		0x100D

/* 4.42 UseMultiTxConfMessage */
#define WSM_MIB_USE_MULTI_TX_CONF		0x1024

/* 4.43 Keep-alive period */
#define WSM_MIB_ID_KEEP_ALIVE_PERIOD		0x1025

/* 4.44 Disable BSSID filter */
#define WSM_MIB_ID_DISABLE_BSSID_FILTER		0x1026

/* Inactivity */
#define WSM_MIB_ID_SET_INACTIVITY		0x1035

/* MAC Addr Filter */
#define WSM_MIB_ID_MAC_ADDR_FILTER		0x1030

#ifdef MCAST_FWDING
/* 4.51 Set Forwarding Offload */
#define WSM_MIB_ID_FORWARDING_OFFLOAD		0x1033
#endif

#ifdef IPV6_FILTERING
/* IpV6 Addr Filter */
/* 4.52 Neighbor solicitation IPv6 address table */
#define WSM_MIB_IP_IPV6_ADDR_FILTER		0x1032
#define WSM_MIB_ID_NS_IP_ADDRESSES_TABLE	0x1034
#define WSM_MAX_NDP_IP_ADDRTABLE_ENTRIES	1
#endif /*IPV6_FILTERING*/

#define WSM_MIB_ID_ZERO_COUNTERS_TABLE		0x1037
#define WSM_MIB_ID_DBG_PRINT_TO_HOST		0x1038
#define WSM_MIB_ID_FW_CMD					0x1039
#define WSM_MIB_ID_MAC_ADDRESS_FROM_EFUSE   0x103a
#define WSM_MIB_ID_SET_EFUSE_DATA			0x103b
#define WSM_MIB_ID_GET_DATA_FROM_EFUSE		0x103c
#define WSM_MIB_ID_SET_EFUSE_MAC			0x103d
#define WSM_MIB_ID_START_WOL				0x103e
#define WSM_MIB_ID_GET_ETF_RX_STATS			0x103f
#define WSM_MIB_ID_SET_EFUSE_DATA_ZHUANG	0x1040
#define WSM_MIB_ID_SMARTCONFIG_START		0x1041
#define WSM_MIB_ID_GET_EFUSE_CUR_STATUS		0x1042
#define WSM_MIB_ID_SET_DUTY_RATIO			0x1043
#define WSM_MIB_ID_GET_EFUSE_LAST_MAC		0x1044
#define WSM_MIB_ID_GET_EFUSE_FIRST_MAC		0x1045
#define WSM_MIB_ID_GET_TEMP_TX_REQ_ID		0x1046


#define WSM_MIB_ID_GET_RATE					0x1050

#define WSM_MIB_ID_GET_SIGMSTAR_256BITSEFUSE  0x1051
#define WSM_MIB_ID_SET_SIGMSTAR_256BITSEFUSE  0x1052

#define WSM_MIB_ID_GET_CHANNEL_IDLE			0x1053
#define WSM_MIB_ID_CHANNEL_TEST_START		0x1054

#define WSM_MIB_ID_GET_MONITOR_MAC_STATUS	0x1055
#define WSM_MIB_ID_GET_FIRST_BLOCK_EFUSE		0x1056
#define WSM_MIB_ID_GET_TX_POWER_STATUS			0x1057//get tx power status currently
#define WSM_MIB_ID_SET_RATE_TX_POWER			0x1058
#define WSM_MIB_ID_SET_BEACON_RATE				0x1059
#define WSM_MIB_ID_GET_CUR_MAX_RATE				0x105a
#define WSM_MIB_ID_GET_EFUSE_ALL_DATA			0x105b
#define WSM_MIB_ID_GET_CONFIG_TX_POWER			0x105c






//#define WSM_START_FIND_ID                 0x0019 //used by efuse change data
//#define WSM_START_FIND_RESP_ID            0x0419 //used by efuse change data
#define WSM_STOP_FIND_ID					 0x001A
#define WSM_STOP_FIND_RESP_ID                0x041A

/*4.54 request buffer id*/
#define WSM_REQUEST_BUFFER_REQ_ID           0x0023
#define WSM_REQUEST_BUFFER_REQ_CNF_ID       0X0423

/*4.55 This device-to-host message returns the */
/*results for a previously queued transmit request */

#define WSM_CNF_BASE                      0x0400
#define WSM_TX_REQ_ID                     0x0404
#define WSM_GIVE_BUFFER_REQ_ID            0x0422
#define WSM_LEGACY_MULTI_TX_CNF_ID        0x041E

#define WSM_IND_BASE 					   0x0800
#define WSM_STARTUP_IND_ID 					0x0801
#define WSM_RECEIVE_INDICATION_ID           0x0804
#define WSM_EVENT_INDICATION_ID             0x0805
#define WSM_SCAN_COMPLETE_IND_ID 			0x0806
#define WSM_SWITCH_CHANNLE_IND_ID           0x080A
#define WSM_SET_PM_MODE_CMPL_IND_ID          0x0809
#define WSM_FIND_CMPL_IND_ID                 0x080B
#define WSM_SUSP_RESUME_TX_IND_ID            0x080C
#define WSM_DEBUG_IND_ID 					0x080E
#define WSM_DEBUG_PRINT_IND_ID				0x0810
#define WSM_MULTI_RECEIVE_INDICATION_ID		 0x0811
#define WSM_SMARTCONFIG_INDICATION_ID		 0x0812
#define WSM_SINGLE_CHANNEL_MULTI_RECEIVE_INDICATION_ID	 0x0813


/* Frame template types */
#define WSM_FRAME_TYPE_PROBE_REQUEST	(0)
#define WSM_FRAME_TYPE_BEACON		(1)
#define WSM_FRAME_TYPE_NULL		(2)
#define WSM_FRAME_TYPE_QOS_NULL		(3)
#define WSM_FRAME_TYPE_PS_POLL		(4)
#define WSM_FRAME_TYPE_PROBE_RESPONSE	(5)
#define WSM_FRAME_TYPE_ARP_REPLY        (6)

#ifdef IPV6_FILTERING
#define WSM_FRAME_TYPE_NA               (7)
#endif /*IPV6_FILTERING*/

#define WSM_FRAME_GREENFIELD		(0x80)	/* See 4.11 */

/* Status */
/* The WSM firmware has completed a request */
/* successfully. */
#define WSM_STATUS_SUCCESS              (0)

/* This is a generic failure code if other error codes do */
/* not apply. */
#define WSM_STATUS_FAILURE              (1)

/* A request contains one or more invalid parameters. */
#define WSM_INVALID_PARAMETER           (2)

/* The request cannot perform because the device is in */
/* an inappropriate mode. */
#define WSM_ACCESS_DENIED               (3)

/* The frame received includes a decryption error. */
#define WSM_STATUS_DECRYPTFAILURE       (4)

/* A MIC failure is detected in the received packets. */
#define WSM_STATUS_MICFAILURE           (5)

/* The transmit request failed due to retry limit being */
/* exceeded. */
#define WSM_STATUS_RETRY_EXCEEDED       (6)

/* The transmit request failed due to MSDU life time */
/* being exceeded. */
#define WSM_STATUS_TX_LIFETIME_EXCEEDED (7)

/* The link to the AP is lost. */
#define WSM_STATUS_LINK_LOST            (8)

/* No key was found for the encrypted frame */
#define WSM_STATUS_NO_KEY_FOUND         (9)

/* Jammer was detected when transmitting this frame */
#define WSM_STATUS_JAMMER_DETECTED      (10)

#define WSM_STATUS_NOEFFECT              (14)

/* The message should be requeued later. */
/* This is applicable only to Transmit */
#define WSM_REQUEUE                     (11)
/*The status is cmdCrc Error */
#define WSM_CMD_CRC_ERRO              (14)
/*The status is dataCrc Error */
#define WSM_DATA_CRC_ERRO              (15)
/* Advanced filtering options */
#define WSM_MAX_FILTER_ELEMENTS		(4)

#define WSM_FILTER_ACTION_IGNORE	(0)
#define WSM_FILTER_ACTION_FILTER_IN	(1)
#define WSM_FILTER_ACTION_FILTER_OUT	(2)

#define WSM_FILTER_PORT_TYPE_DST	(0)
#define WSM_FILTER_PORT_TYPE_SRC	(1)


struct wsm_hdr {
	__le16 len;
	__le16 id;
};
struct wsm_hdr_tx {
#ifndef SPI_BUS
	__le32 flag;
#endif
#ifdef ATBM_SDIO_PATCH
	__le32 headCsm;
	__le32 sdioSeq;
#endif
#ifdef  ATBM_WSM_SDIO_TX_MULT
	__le16 tx_len;
	__le16 tx_id;
#endif
#ifdef USB_BUS_BUG
	__le16 usb_len;
	__le16 usb_id;
#endif	
	__le16 len;
	__le16 id;
};
#define WSM_TX_SEQ_MAX			(7)
#define WSM_RX_SEQ_MAX			(7)
#define WSM_TX_SDIO_SEQ_MAX			(0x3f)
#define WSM_TX_SEQ(seq)			\
		((seq & WSM_TX_SEQ_MAX) << 13)
#define WSM_TX_FLAG(channel) (channel &0x3f)
#define WSM_TX_FLAG_CHECK(id) ((id &0xffff)<<16)
#define WSM_TX_LINK_ID_MAX		(0x0F)
#define WSM_TX_LINK_ID(link_id)		\
		((link_id & WSM_TX_LINK_ID_MAX) << 6)

#define WSM_TX_IF_ID_MAX		(0x0F)
#define WSM_TX_IF_ID(if_id)		\
		((if_id & WSM_TX_IF_ID_MAX) << 6)

#define MAX_BEACON_SKIP_TIME_MS 1000


#define WSM_CMD_LAST_CHANCE_TIMEOUT (HZ * 20/2)
#define WSM_CMD_EXTENDED_TIMEOUT (HZ * 20 / 2)

#define WSM_RI_GET_PEER_ID_FROM_FLAGS(_f)         (((_f)&(0xF<<25)>>25))


/* ******************************************************************** */
/* WSM capcbility							*/

//#define WSM_STARTUP_IND_ID 0x0801
struct wsm_caps {
	u16 numInpChBufs;
	u16 sizeInpChBuf;
	u16 hardwareId;
	u16 hardwareSubId;
	u32 firmwareCap;
	u16 firmwareType;
	u16 firmwareApiVer;
	u16 firmwareBuildNumber;
	u16 firmwareVersion;
	u32 HiHwCnfBufaddr;
	int firmwareReady;
	u32 exceptionaddr;
	u16 NumOfInterfaces;
	u16 NumOfStations;
	u32 NumOfHwXmitedAddr;
	u32 firmeareExCap;
};

/* ******************************************************************** */
/* WSM commands								*/

#define WSM_CONFIGURATION_REQ_ID 0x0009
#define WSM_CONFIGURATION_RESP_ID 0x0409
struct wsm_tx_power_range {
	int min_power_level;
	int max_power_level;
	u32 stepping;
};

/* 3.1 */
struct wsm_configuration {
	/* [in] */ u32 dot11MaxTransmitMsduLifeTime;
	/* [in] */ u32 dot11MaxReceiveLifeTime;
	/* [in] */ u32 dot11RtsThreshold;
	/* [in, out] */ u8 *dot11StationId;
	/* [in] */ u8 *dpdData;
	/* [in] */ u32 dpdData_size;
	/* [out] */ u8 dot11FrequencyBandsSupported;
	/* [out] */ u32 supportedRateMask;
	/* [out] */ struct wsm_tx_power_range txPowerRange[2];
};

int wsm_configuration(struct atbm_common *hw_priv,
		      struct wsm_configuration *arg,
		      int if_id);

#define WSM_FIRMWARE_CHECK_ID 0x0002
#define WSM_FIRMWARE_CHECK_CONFIRM_ID 0x0402

/* 3.3 */
#define WSM_RESET_REQ_ID 0x000A
#define WSM_RESET_RESP_ID 0x040A
struct wsm_reset {
	/* [in] */ int link_id;
	/* [in] */ bool reset_statistics;
};

int wsm_reset(struct atbm_common *hw_priv, const struct wsm_reset *arg,
	      int if_id);

/* 3.5 */
#define WSM_READ_MIB_REQ_ID 0x0005
#define WSM_READ_MIB_RESP_ID 0x0405
int wsm_read_mib(struct atbm_common *hw_priv, u16 mibId, void *buf,
		 size_t buf_size,int if_id);

/* 3.7 */
#define WSM_WRITE_MIB_REQ_ID 0x0006
#define WSM_WRITE_MIB_RESP_ID 0x0406
int wsm_write_mib(struct atbm_common *hw_priv, u16 mibId, void *buf,
		  size_t buf_size, int if_id);

/* 3.9 */
#define WSM_START_SCAN_REQ_ID 0x0007
#define WSM_START_SCAN_RESP_ID 0x0407
struct wsm_ssid {
	u8 ssid[32];
	u32 length;
};

struct wsm_scan_ch {
	u16 number;
	u32 minChannelTime;
	u32 maxChannelTime;
	u32 txPowerLevel;
};

/* 3.13 */
//#define WSM_SCAN_COMPLETE_IND_ID 0x0806
struct wsm_scan_complete {
	/* WSM_STATUS_... */
	u32 status;

	/* WSM_PSM_... */
	u8 psm;

	/* Number of channels that the scan operation completed. */
	u8 numChannels;

	/* Duty Ratio for per channel */
	u8 busy_ratio[14];
#ifdef CONFIG_ATBM_SUPPORT_SCHED_SCAN	
#ifdef ROAM_OFFLOAD
	u16 reserved;
#endif /*ROAM_OFFLOAD*/
#endif
};

typedef void (*wsm_scan_complete_cb) (struct atbm_common *hw_priv,
				      struct wsm_scan_complete *arg);

/* 3.9 */
struct wsm_scan {
	/* WSM_PHY_BAND_... */
	/* [in] */ u8 band;

	/* WSM_SCAN_TYPE_... */
	/* [in] */ u8 scanType;

	/* WSM_SCAN_FLAG_... */
	/* [in] */ u8 scanFlags;

	/* WSM_TRANSMIT_RATE_... */
	/* [in] */ u8 maxTransmitRate;

	/* Interval period in TUs that the device shall the re- */
	/* execute the requested scan. Max value supported by the device */
	/* is 256s. */
	/* [in] */ u32 autoScanInterval;

	/* Number of probe requests (per SSID) sent to one (1) */
	/* channel. Zero (0) means that none is send, which */
	/* means that a passive scan is to be done. Value */
	/* greater than zero (0) means that an active scan is to */
	/* be done. */
	/* [in] */ u32 numOfProbeRequests;

	/* Number of channels to be scanned. */
	/* Maximum value is WSM_SCAN_MAX_NUM_OF_CHANNELS. */
	/* [in] */ u8 numOfChannels;

	/* Number of SSID provided in the scan command (this */
	/* is zero (0) in broadcast scan) */
	/* The maximum number of SSIDs is WSM_SCAN_MAX_NUM_OF_SSIDS. */
	/* [in] */ u8 numOfSSIDs;

	/* The delay time (in microseconds) period */
	/* before sending a probe-request. */
	/* [in] */ u8 probeDelay;

	/* SSIDs to be scanned [numOfSSIDs]; */
	/* [in] */ struct wsm_ssid *ssids;

	/* Channels to be scanned [numOfChannels]; */
	/* [in] */ struct wsm_scan_ch *ch;
};

int wsm_scan(struct atbm_common *hw_priv, const struct wsm_scan *arg,
			int if_id);

/* 3.11 */
#define WSM_STOP_SCAN_REQ_ID 0x0008
#define WSM_STOP_SCAN_RESP_ID 0x0408
int wsm_stop_scan(struct atbm_common *hw_priv, int if_id);

/* 3.14 */
struct wsm_tx_confirm {
	/* Packet identifier used in wsm_tx. */
	/* [out] */ u32 packetID;

	/* WSM_STATUS_... */
	/* [out] */ u32 status;

	/* WSM_TRANSMIT_RATE_... */
	/* [out] */ u8 txedRate;

	/* The number of times the frame was transmitted */
	/* without receiving an acknowledgement. */
	/* [out] */ u8 ackFailures;

	/* WSM_TX_STATUS_... */
	/* [out] */ u16 flags;

	/* The total time in microseconds that the frame spent in */
	/* the WLAN device before transmission as completed. */
	/* [out] */ u32 mediaDelay;

	/* The total time in microseconds that the frame spent in */
	/* the WLAN device before transmission was started. */
	/* [out] */ u32 txQueueDelay;

	/* [out]*/ u32 link_id;

	/*[out]*/ int if_id;
};

/* 3.15 */
typedef void (*wsm_tx_confirm_cb) (struct atbm_common *hw_priv,
				   struct wsm_tx_confirm *arg);

#if OLD_RATE_POLICY
#define WSM_TRANSMIT_REQ_MSG_ID		0x0004
#else
#define WSM_TRANSMIT_REQ_MSG_ID		0x0004
#endif//OLD_RATE_POLICY
/* Note that ideology of wsm_tx struct is different against the rest of
 * WSM API. wsm_hdr is /not/ a caller-adapted struct to be used as an input
 * argument for WSM call, but a prepared bytestream to be sent to firmware.
 * It is filled partly in atbm_tx, partly in low-level WSM code.
 * Please pay attention once again: ideology is different.
 *
 * Legend:
 * - [in]: atbm_tx must fill this field.
 * - [wsm]: the field is filled by low-level WSM.
 */
struct wsm_tx {
	/* common WSM header */
	/* [in/wsm] */ struct wsm_hdr_tx hdr;

	/* Packet identifier that meant to be used in completion. */
	/* [in] */ __le32 packetID;

	/* WSM_TRANSMIT_RATE_... */
	/* [in] */ u8 maxTxRate;
#if (OLD_RATE_POLICY==0)
	/* WSM_TRANSMIT_RATE_RETRY... */
	/* [in] */ u8 TxRateRetry[4];
#endif //#if (OLD_RATE_POLICY==0)

	/* WSM_QUEUE_... */
	/* [in] */ u8 queueId;

	/* True: another packet is pending on the host for transmission. */
	/* [wsm] */ u8 more;

	/* Bit 0 = 0 - Start expiry time from first Tx attempt (default) */
	/* Bit 0 = 1 - Start expiry time from receipt of Tx Request */
	/* Bits 3:1  - PTA Priority */
	/* Bits 6:4  - Tx Rate Retry Policy */
	/* Bit 7 - Reserved */
	/* [in] */ u8 flags;

	/* Should be 0. */
	/* [in] */ __le32 reserved;

	/* The elapsed time in TUs, after the initial transmission */
	/* of an MSDU, after which further attempts to transmit */
	/* the MSDU shall be terminated. Overrides the global */
	/* dot11MaxTransmitMsduLifeTime setting [optional] */
	/* Device will set the default value if this is 0. */
	/* [wsm] */ __le32 expireTime;

	/* WSM_HT_TX_... */
	/* [in] */ __le32 htTxParameters;
	
};

/* = sizeof(generic hi hdr) + sizeof(wsm hdr) + sizeof(alignment) */
#define WSM_TX_EXTRA_HEADROOM (28+4)

/* 3.16 */
struct wsm_rx {
	/* WSM_STATUS_... */
	/* [out] */ u32 status;

	/* Specifies the channel of the received packet. */
	/* [out] */ u16 channelNumber;

	/* WSM_TRANSMIT_RATE_... */
	/* [out] */ u8 rxedRate;

	/* This value is expressed in signed Q8.0 format for */
	/* RSSI and unsigned Q7.1 format for RCPI. */
	/* [out] */ u8 rcpiRssi;

	/* WSM_RX_STATUS_... */
	/* [out] */ u32 flags;
#ifdef ATBM_SUPPORT_WIDTH_40M
	/*width 40M params*/
	/*[in]*/	u32 channel_type;
#endif

	/* An 802.11 frame. */
	/* [out] */ void *frame;

	/* Size of the frame */
	/* [out] */ size_t frame_size;

	/* Link ID */
	/* [out] */ int link_id;
	/* [out] */ int if_id;
};
struct wsm_multi_rx {
	u16 MsgLen;
	u16 MsgId;
	u8  RxFrameNum;
	u8  reserved[3];	
};
/* = sizeof(generic hi hdr) + sizeof(wsm hdr) */
#define WSM_RX_EXTRA_HEADROOM (16)

typedef void (*wsm_rx_cb) (struct atbm_vif *priv, struct wsm_rx *arg,
			   struct sk_buff **skb_p);

/* 3.17 */
struct wsm_event {
	/* WSM_STATUS_... */
	/* [out] */ u32 eventId;

	/* Indication parameters. */
	/* For error indication, this shall be a 32-bit WSM status. */
	/* For RCPI or RSSI indication, this should be an 8-bit */
	/* RCPI or RSSI value. */
	/* [out] */ u32 eventData;
};

struct atbm_wsm_event {
	struct list_head link;
	struct wsm_event evt;
	u8 if_id;
};

/* 3.18 - 3.22 */
/* Measurement. Skipped for now. Irrelevent. */

typedef void (*wsm_event_cb) (struct atbm_common *hw_priv,
			      struct wsm_event *arg);

/* 3.23 */
#define WSM_JOIN_REQ_ID 0x000B
#define WSM_JOIN_RESP_ID 0x040B
struct wsm_join {
	/* WSM_JOIN_MODE_... */
	/* [in] */ u8 mode;

	/* WSM_PHY_BAND_... */
	/* [in] */ u8 band;

	/* Specifies the channel number to join. The channel */
	/* number will be mapped to an actual frequency */
	/* according to the band */
	/* [in] */ u16 channelNumber;

	/* Specifies the BSSID of the BSS or IBSS to be joined */
	/* or the IBSS to be started. */
	/* [in] */ u8 bssid[6];

	/* ATIM window of IBSS */
	/* When ATIM window is zero the initiated IBSS does */
	/* not support power saving. */
	/* [in] */ u16 atimWindow;

	/* WSM_JOIN_PREAMBLE_... */
	/* [in] */ u8 preambleType;

	/* Specifies if a probe request should be send with the */
	/* specified SSID when joining to the network. */
	/* [in] */ u8 probeForJoin;

	/* DTIM Period (In multiples of beacon interval) */
	/* [in] */ u8 dtimPeriod;

	/* WSM_JOIN_FLAGS_... */
	/* [in] */ u8 flags;

	/* Length of the SSID */
	/* [in] */ u32 ssidLength;

	/* Specifies the SSID of the IBSS to join or start */
	/* [in] */ u8 ssid[32];

	/* Specifies the time between TBTTs in TUs */
	/* [in] */ u32 beaconInterval;

	/* A bit mask that defines the BSS basic rate set. */
	/* [in] */ u32 basicRateSet;
#ifdef ATBM_SUPPORT_WIDTH_40M
	/*width 40M params*/
	/*[in]*/	u32 channel_type;
#endif
	/* Minimum transmission power level in units of 0.1dBm */
	/* [out] */ int minPowerLevel;

	/* Maximum transmission power level in units of 0.1dBm */
	/* [out] */ int maxPowerLevel;
};

int wsm_join(struct atbm_common *hw_priv, struct wsm_join *arg, int if_id);

/* 3.25 */
#define WSM_SET_PM_REQ_ID 0x0010
#define WSM_SET_PM_RESP_ID 0x0410
struct wsm_set_pm {
	/* WSM_PSM_... */
	/* [in] */ u8 pmMode;

	/* in unit of 500us; 0 to use default */
	/* [in] */ u8 fastPsmIdlePeriod;

	/* in unit of 500us; 0 to use default */
	/* [in] */ u8 apPsmChangePeriod;

	/* in unit of 500us; 0 to disable auto-pspoll */
	/* [in] */ u8 minAutoPsPollPeriod;
};

int wsm_set_pm(struct atbm_common *hw_priv, const struct wsm_set_pm *arg,
	       int if_id);

/* 3.27 */
struct wsm_set_pm_complete {
	u8 psm;			/* WSM_PSM_... */
};

typedef void (*wsm_set_pm_complete_cb) (struct atbm_common *hw_priv,
					struct wsm_set_pm_complete *arg);

/* 3.28 */
#define WSM_SET_BSS_PARAMS_REQ_ID 0x0011
#define WSM_SET_BSS_PARAMS_RESP_ID 0x0411
struct wsm_set_bss_params {
	/* The number of lost consecutive beacons after which */
	/* the WLAN device should indicate the BSS-Lost event */
	/* to the WLAN host driver. */
	u8 beaconLostCount;

	/* The AID received during the association process. */
	u16 aid;

	/* The operational rate set mask */
	u32 operationalRateSet;
};

int wsm_set_bss_params(struct atbm_common *hw_priv,
		       const struct wsm_set_bss_params *arg, int if_id);

/* 3.30 */
#define WSM_ADD_KEY_REQ_ID         0x000C
#define WSM_ADD_KEY_RESP_ID        0x040C
struct wsm_add_key {
	u8 type;		/* WSM_KEY_TYPE_... */
	u8 entryIndex;		/* Key entry index: 0 -- WSM_KEY_MAX_INDEX */
	u16 reserved;
	union {
		struct {
			u8 peerAddress[6];	/* MAC address of the
						 * peer station */
			u8 reserved;
			u8 keyLength;		/* Key length in bytes */
			u8 keyData[16];		/* Key data */
		} __packed wepPairwiseKey;
		struct {
			u8 keyId;		/* Unique per key identifier
						 * (0..3) */
			u8 keyLength;		/* Key length in bytes */
			u16 reserved;
			u8 keyData[16];		/* Key data */
		} __packed wepGroupKey;
		struct {
			u8 peerAddress[6];	/* MAC address of the
						 * peer station */
			u8 reserved[2];
			u8 tkipKeyData[16];	/* TKIP key data */
			u8 rxMicKey[8];		/* Rx MIC key */
			u8 txMicKey[8];		/* Tx MIC key */
		} __packed tkipPairwiseKey;
		struct {
			u8 tkipKeyData[16];	/* TKIP key data */
			u8 rxMicKey[8];		/* Rx MIC key */
			u8 keyId;		/* Key ID */
			u8 reserved[3];
			u8 rxSeqCounter[8];	/* Receive Sequence Counter */
		} __packed tkipGroupKey;
		struct {
			u8 peerAddress[6];	/* MAC address of the
						 * peer station */
			u16 reserved;
			u8 aesKeyData[16];	/* AES key data */
		} __packed aesPairwiseKey;
		struct {
			u8 aesKeyData[16];	/* AES key data */
			u8 keyId;		/* Key ID */
			u8 reserved[3];
			u8 rxSeqCounter[8];	/* Receive Sequence Counter */
		} __packed aesGroupKey;
		/*11w */
		struct {
			u8 igtKeyData[16];
			u8 keyId;
			u8 reserved[3];
			u8 ipn[8]; /* actual IPN is only 6 bytes long*/
		}__packed igtkGroupKey;

		struct {
			u8 peerAddress[6];	/* MAC address of the
						 * peer station */
			u8 keyId;		/* Key ID */
			u8 reserved;
			u8 wapiKeyData[16];	/* WAPI key data */
			u8 micKeyData[16];	/* MIC key data */
		} __packed wapiPairwiseKey;
		struct {
			u8 wapiKeyData[16];	/* WAPI key data */
			u8 micKeyData[16];	/* MIC key data */
			u8 keyId;		/* Key ID */
			u8 reserved[3];
		} __packed wapiGroupKey;
	} __packed;
} __packed;

int wsm_add_key(struct atbm_common *hw_priv, const struct wsm_add_key *arg,
			int if_id);

/* 3.32 */
#define WSM_REMOVE_KEY_REQ_ID         0x000D
#define WSM_REMOVE_KEY_RESP_ID        0x040D
struct wsm_remove_key {
	/* Key entry index : 0-10 */
	u8 entryIndex;
};
#if 0
int wsm_remove_key(struct atbm_common *hw_priv,
		   const struct wsm_remove_key *arg, int if_id);
#else
int wsm_remove_key(struct atbm_common *hw_priv, const struct wsm_add_key *arg,
			int if_id);

#endif
/* 3.34 */
#define WSM_QUEUE_PARAMS_REQ_ID 0x0012
#define WSM_QUEUE_PARAMS_RESP_ID 0x0412
struct wsm_set_tx_queue_params {
	/* WSM_ACK_POLICY_... */
	u8 ackPolicy;

	/* Medium Time of TSPEC (in 32us units) allowed per */
	/* One Second Averaging Period for this queue. */
	u16 allowedMediumTime;

	/* dot11MaxTransmitMsduLifetime to be used for the */
	/* specified queue. */
	u32 maxTransmitLifetime;
};

struct wsm_tx_queue_params {
	/* NOTE: index is a linux queue id. */
	struct wsm_set_tx_queue_params params[4];
};

#define WSM_TX_QUEUE_SET(queue_params, queue, ack_policy, allowed_time,     \
			 max_life_time)					    \
do {									    \
	struct wsm_set_tx_queue_params *p = &(queue_params)->params[queue]; \
	p->ackPolicy = (ack_policy);				\
	p->allowedMediumTime = (allowed_time);				\
	p->maxTransmitLifetime = (max_life_time);			\
} while (0)

int wsm_set_tx_queue_params(struct atbm_common *hw_priv,
			    const struct wsm_set_tx_queue_params *arg,
			    u8 id, int if_id);

/* 3.36 */
#define WSM_EDCA_PARAMS_REQ_ID 0x0013
#define WSM_EDCA_PARAMS_RESP_ID 0x0413
struct wsm_edca_queue_params {
	/* CWmin (in slots) for the access class. */
	/* [in] */ u16 cwMin;

	/* CWmax (in slots) for the access class. */
	/* [in] */ u16 cwMax;

	/* AIFS (in slots) for the access class. */
	/* [in] */ u8 aifns;

	/* TX OP Limit (in microseconds) for the access class. */
	/* [in] */ u16 txOpLimit;

	/* dot11MaxReceiveLifetime to be used for the specified */
	/* the access class. Overrides the global */
	/* dot11MaxReceiveLifetime value */
	/* [in] */ u32 maxReceiveLifetime;

	/* UAPSD trigger support for the access class. */
	/* [in] */ bool uapsdEnable;
};

struct wsm_edca_params {
	/* NOTE: index is a linux queue id. */
	struct wsm_edca_queue_params params[4];
};

#define TXOP_UNIT 32
#define WSM_EDCA_SET(edca, queue, aifs, cw_min, cw_max, txop, life_time,\
		uapsd)	\
	do {							\
		struct wsm_edca_queue_params *p = &(edca)->params[queue]; \
		p->cwMin = (cw_min);				\
		p->cwMax = (cw_max);				\
		p->aifns = (aifs);				\
		p->txOpLimit = ((txop) * TXOP_UNIT);		\
		p->maxReceiveLifetime = (life_time);		\
		p->uapsdEnable = (uapsd);			\
	} while (0)

int wsm_set_edca_params(struct atbm_common *hw_priv,
			const struct wsm_edca_params *arg, int if_id);

int wsm_set_uapsd_param(struct atbm_common *hw_priv,
			const struct wsm_edca_params *arg);

/* 3.38 */
/* Set-System info. Skipped for now. Irrelevent. */

/* 3.40 */
#define WSM_SWITCH_CHANNEL_REQ_ID 0x0016
#define WSM_SWITCH_CHANNEL_RESP_ID 0x0416
struct wsm_switch_channel {
	/* 1 - means the STA shall not transmit any further */
	/* frames until the channel switch has completed */
	/* [in] */ u8 channelMode;

	/* Number of TBTTs until channel switch occurs. */
	/* 0 - indicates switch shall occur at any time */
	/* 1 - occurs immediately before the next TBTT */
	/* [in] */ u8 channelSwitchCount;

	/* The new channel number to switch to. */
	/* Note this is defined as per section 2.7. */
	/* [in] */ u16 newChannelNumber;
};

int wsm_switch_channel(struct atbm_common *hw_priv,
		       const struct wsm_switch_channel *arg, int if_id);

typedef void (*wsm_channel_switch_cb) (struct atbm_common *hw_priv);

#define WSM_START_REQ_ID 0x0017
#define WSM_START_RESP_ID 0x0417
struct wsm_start {
	/* WSM_START_MODE_... */
	/* [in] */ u8 mode;

	/* WSM_PHY_BAND_... */
	/* [in] */ u8 band;

	/* Channel number */
	/* [in] */ u16 channelNumber;

	/* Client Traffic window in units of TU */
	/* Valid only when mode == ..._P2P */
	/* [in] */ u32 CTWindow;

	/* Interval between two consecutive */
	/* beacon transmissions in TU. */
	/* [in] */ u32 beaconInterval;

	/* DTIM period in terms of beacon intervals */
	/* [in] */ u8 DTIMPeriod;

	/* WSM_JOIN_PREAMBLE_... */
	/* [in] */ u8 preambleType;

	/* The delay time (in microseconds) period */
	/* before sending a probe-request. */
	/* [in] */ u8 probeDelay;

	/* Length of the SSID */
	/* [in] */ u8 ssidLength;

	/* SSID of the BSS or P2P_GO to be started now. */
	/* [in] */ u8 ssid[32];

	/* The basic supported rates for the MiniAP. */
	/* [in] */ u32 basicRateSet;
#ifdef ATBM_SUPPORT_WIDTH_40M
			   u32	channel_type;
#endif
};

int wsm_start(struct atbm_common *hw_priv, const struct wsm_start *arg,
		int if_id);

#define WSM_BEACON_TRANSMIT_REQ_ID 0x0018
#define WSM_BEACON_TRANSMIT_RESP_ID 0x0418
#if 0
struct wsm_beacon_transmit {
	/* 1: enable; 0: disable */
	/* [in] */ u8 enableBeaconing;
};

int wsm_beacon_transmit(struct atbm_common *hw_priv,
			const struct wsm_beacon_transmit *arg,
			int if_id);
#endif
//[ Notice ] this msgid used by efuse change data
//int wsm_start_find(struct atbm_common *hw_priv, int if_id);

int wsm_stop_find(struct atbm_common *hw_priv, int if_id);

typedef void (*wsm_find_complete_cb) (struct atbm_common *hw_priv,
				      u32 status);

struct wsm_suspend_resume {
	/* See 3.52 */
	/* Link ID */
	/* [out] */ int link_id;
	/* Stop sending further Tx requests down to device for this link */
	/* [out] */ bool stop;
	/* Transmit multicast Frames */
	/* [out] */ bool multicast;
	/* The AC on which Tx to be suspended /resumed. */
	/* This is applicable only for U-APSD */
	/* WSM_QUEUE_... */
	/* [out] */ int queue;
	/* [out] */ int if_id;
};

typedef void (*wsm_suspend_resume_cb) (struct atbm_vif *priv,
				       struct wsm_suspend_resume *arg);

/* 3.54 Update-IE request. */
#define WSM_UPDATE_IE_REQ_ID     0x001B
#define WSM_UPDATE_IE_RESP_ID    0x041B
struct wsm_update_ie {
	/* WSM_UPDATE_IE_... */
	/* [in] */ u16 what;
	/* [in] */ u16 count;
	/* [in] */ u8 *ies;
	/* [in] */ size_t length;
};

int wsm_update_ie(struct atbm_common *hw_priv,
		  const struct wsm_update_ie *arg, int if_id);

/* 3.56 */
struct wsm_map_link {
	/* MAC address of the remote device */
	/* [in] */ u8 mac_addr[6];
	/* [in] */ u8 unmap;
	/* [in] */ u8 link_id;
};
#define WSM_MAP_LINK_REQ_ID       0x001C
#define WSM_MAP_LINK_RESP_ID       0x041C
int wsm_map_link(struct atbm_common *hw_priv, const struct wsm_map_link *arg,
		int if_id);

struct wsm_cbc {
	wsm_scan_complete_cb scan_complete;
	wsm_tx_confirm_cb tx_confirm;
	wsm_rx_cb rx;
	wsm_event_cb event;
	wsm_set_pm_complete_cb set_pm_complete;
	wsm_channel_switch_cb channel_switch;
	wsm_find_complete_cb find_complete;
	wsm_suspend_resume_cb suspend_resume;
};
#ifdef MCAST_FWDING

/* 3.65	Give Buffer Request */
int wsm_init_release_buffer_request(struct atbm_common *priv, u8 index);

/* 3.67	Request Buffer Request */
int wsm_request_buffer_request(struct atbm_vif *priv,
                                u8 *arg);
#endif
/* ******************************************************************** */
/* MIB shortcats							*/

static inline int wsm_set_output_power(struct atbm_common *hw_priv,
				       int power_level, int if_id)
{
	__le32 val = __cpu_to_le32(power_level);
	return wsm_write_mib(hw_priv, WSM_MIB_ID_DOT11_CURRENT_TX_POWER_LEVEL,
			     &val, sizeof(val), if_id);
}

static inline int wsm_set_beacon_wakeup_period(struct atbm_common *hw_priv,
					       unsigned dtim_interval,
					       unsigned listen_interval,
					       int if_id)
{
	struct {
		u8 numBeaconPeriods;
		u8 reserved;
		__le16 listenInterval;
	} val = {
	dtim_interval, 0, __cpu_to_le16(listen_interval)};
	if (dtim_interval > 0xFF || listen_interval > 0xFFFF)
		return -EINVAL;
	else
		return wsm_write_mib(hw_priv, WSM_MIB_ID_BEACON_WAKEUP_PERIOD,
				     &val, sizeof(val), if_id);
}

struct wsm_rcpi_rssi_threshold {
	u8 rssiRcpiMode;	/* WSM_RCPI_RSSI_... */
	u8 lowerThreshold;
	u8 upperThreshold;
	u8 rollingAverageCount;
};

static inline int wsm_set_rcpi_rssi_threshold(struct atbm_common *hw_priv,
					struct wsm_rcpi_rssi_threshold *arg,
					int if_id)
{
	return wsm_write_mib(hw_priv, WSM_MIB_ID_RCPI_RSSI_THRESHOLD, arg,
			     sizeof(*arg), if_id);
}

struct wsm_counters_table {
	__le32 countPlcpErrors;
	__le32 countFcsErrors;
	__le32 countTxPackets;
	__le32 countRxPackets;
	__le32 countRxPacketErrors;
	__le32 countRtsSuccess;
	__le32 countRtsFailures;
	__le32 countRxFramesSuccess;
	__le32 countRxDecryptionFailures;
	__le32 countRxMicFailures;
	__le32 countRxNoKeyFailures;
	__le32 countTxMulticastFrames;
	__le32 countTxFramesSuccess;
	__le32 countTxFrameFailures;
	__le32 countTxFramesRetried;
	__le32 countTxFramesMultiRetried;
	__le32 countRxFrameDuplicates;
	//__le32 countRtsSuccess;
	//__le32 countRtsFailures;
	__le32 countAckFailures;
	__le32 countRxMulticastFrames;
	//__le32 countRxFramesSuccess;
	__le32 countRxCMACICVErrors;
	__le32 countRxCMACReplays;
	__le32 countRxMgmtCCMPReplays;
	__le32 countRxBIPMICErrors;
};

struct wsm_statistics_table {
	__le16 latestSnr;
	u8     latestRcpi;
	char   latestRssi;
};
static inline int wsm_get_counters_table(struct atbm_common *hw_priv,
					 struct wsm_counters_table *arg)
{
	return wsm_read_mib(hw_priv, WSM_MIB_ID_COUNTERS_TABLE,
			arg, sizeof(*arg),-1);
}

static inline int wsm_get_statistics_table(struct atbm_common *hw_priv,
					 struct wsm_statistics_table *arg)
{
	return wsm_read_mib(hw_priv, WSM_MIB_ID_STATISTICS_TABLE,
			arg, sizeof(*arg),-1);
}

static inline int wsm_get_station_id(struct atbm_common *hw_priv, u8 *mac)
{
	return wsm_read_mib(hw_priv, WSM_MIB_ID_DOT11_STATION_ID, mac,
			    ETH_ALEN,-1);
}

static inline int wsm_get_mac_address(struct atbm_common *hw_priv, u8 *mac)
{
	return wsm_read_mib(hw_priv, WSM_MIB_ID_MAC_ADDRESS_FROM_EFUSE, mac,
			    ETH_ALEN,-1);
}

static inline int wsm_set_efuse_mac_address(struct atbm_common *hw_priv, u8 *mac)
{
	return wsm_write_mib(hw_priv, WSM_MIB_ID_SET_EFUSE_MAC, mac,
			    ETH_ALEN,-1);
}
//notice!!!this function no write data to efuse register, but write data to variable
static inline int wsm_set_efuse_data(struct atbm_common *hw_priv, u8 *efuse, int len)
{
	return wsm_write_mib(hw_priv, WSM_MIB_ID_SET_EFUSE_DATA_ZHUANG, efuse,
			    len,-1);
}
static inline int wsm_get_SIGMSTAR_256BITSEFUSE(struct atbm_common *hw_priv, u8 *efuse, int len)
{
	return wsm_read_mib(hw_priv, WSM_MIB_ID_GET_SIGMSTAR_256BITSEFUSE, efuse,
			    len,-1);
}
static inline int wsm_set_SIGMSTAR_256BITSEFUSE(struct atbm_common *hw_priv, u8 *efuse, int len)
{
	return wsm_write_mib(hw_priv, WSM_MIB_ID_SET_SIGMSTAR_256BITSEFUSE, efuse,
			    len,-1);
}

struct power_status_t{
	s16 b_txpower_delta_value_multiple_10;
	s16 gn_20m_txpower_delta_value_multiple_10;
	s16 gn_40m_txpower_delta_value_multiple_10;
	s16 reserve;
};
struct cfg_txpower_t{
	s8 set_txpwr_delta_gain[3];
	s8 set_b_txpwr_delta_gain[3];
	s8 set_gn_txpwr_delta_gain[3];
	s8 different_rate_txpower_mode[11];
	s8 different_rate_txpower_mode_40M[11];
	s8 reserve;
};

static inline int wsm_get_tx_power_status(struct atbm_common *hw_priv, void *pwrStatus, int len)
{
	return wsm_read_mib(hw_priv, WSM_MIB_ID_GET_TX_POWER_STATUS, pwrStatus,
			    len,-1);
}


struct efuse_headr{
	u8 specific;
	u8 version;
	u8 dcxo_trim;
	u8 delta_gain1;
	u8 delta_gain2;
	u8 delta_gain3;
	u8 Tj_room;
	u8 topref_ctrl_bias_res_trim;
	u8 PowerSupplySel;
	u8 mac[6];
};
//wsm_efuse_change_data_cmd  return value
#define LMC_STATUS_CODE__EFUSE_VERSION_CHANGE	96
#define LMC_STATUS_CODE__EFUSE_FIRST_WRITE 		97
#define LMC_STATUS_CODE__EFUSE_PARSE_FAILED 	98
#define LMC_STATUS_CODE__EFUSE_FULL 			99

#define WSM_HI_EFUSE_CHANGE_DATA_REQ_ID 0x0019
#define WSM_HI_EFUSE_CHANGE_DATA_CNF_ID 0x0419
int wsm_efuse_change_data_cmd(struct atbm_common *hw_priv, const struct efuse_headr *arg,
		int if_id);

static inline int wsm_get_efuse_data(struct atbm_common *hw_priv, void *efuse, int len)
{
	return wsm_read_mib(hw_priv, WSM_MIB_ID_GET_DATA_FROM_EFUSE, efuse,
			    len,-1);
}

static inline int wsm_get_efuse_first_data(struct atbm_common *hw_priv, void *efuse, int len)
{
	return wsm_read_mib(hw_priv, WSM_MIB_ID_GET_FIRST_BLOCK_EFUSE, efuse,
			    len,-1);
}


static inline int wsm_get_efuse_remain_bit(struct atbm_common *hw_priv, void *remainBit, int len)
{
	return wsm_read_mib(hw_priv, WSM_MIB_ID_GET_EFUSE_CUR_STATUS, remainBit,
				len, -1);
}

static inline int wsm_get_efuse_last_mac(struct atbm_common *hw_priv, u8 *mac)
{
	return wsm_read_mib(hw_priv, WSM_MIB_ID_GET_EFUSE_LAST_MAC, mac,
				ETH_ALEN, -1);
}

static inline int wsm_get_efuse_first_mac(struct atbm_common *hw_priv, u8 *mac)
{
	return wsm_read_mib(hw_priv, WSM_MIB_ID_GET_EFUSE_FIRST_MAC, mac,
				ETH_ALEN, -1);
}

static inline int wsm_get_Tjroom_temperature(struct atbm_common *hw_priv, void *buf, int len)
{
	return wsm_read_mib(hw_priv, WSM_MIB_ID_GET_TEMP_TX_REQ_ID, buf,
				len, -1);
}

static inline int wsm_get_efuse_all_data(struct atbm_common *hw_priv, void *buf, int len)
{
	if(buf == NULL)
		return -1;
	return wsm_read_mib(hw_priv, WSM_MIB_ID_GET_EFUSE_ALL_DATA, buf,
				len, -1);
}

static inline int wsm_get_cfg_txpower(struct atbm_common *hw_priv, void *buf, int len)
{
	if(buf == NULL)
		return -1;
	return wsm_read_mib(hw_priv, WSM_MIB_ID_GET_CONFIG_TX_POWER, buf,
				len, -1);
}


struct wsm_rx_filter {
	bool promiscuous;
	bool bssid;
	bool fcs;
	bool probeResponder;
	bool keepalive;
};

static inline int wsm_set_rx_filter(struct atbm_common *hw_priv,
				    const struct wsm_rx_filter *arg,
				    int if_id)
{
	__le32 val = 0;
	if (arg->promiscuous)
		val |= __cpu_to_le32(BIT(0));
	if (arg->bssid)
		val |= __cpu_to_le32(BIT(1));
	if (arg->fcs)
		val |= __cpu_to_le32(BIT(2));
	if (arg->probeResponder)
		val |= __cpu_to_le32(BIT(3));
	if (arg->keepalive)
		val |= __cpu_to_le32(BIT(4));
	return wsm_write_mib(hw_priv, WSM_MIB_ID_RX_FILTER, &val, sizeof(val),
			if_id);
}

int wsm_set_probe_responder(struct atbm_vif *priv, bool enable);
#ifdef ATBM_SUPPORT_WOW
int wsm_set_keepalive_filter(struct atbm_vif *priv, bool enable);
#endif

#define WSM_BEACON_FILTER_IE_HAS_CHANGED		BIT(0)
#define WSM_BEACON_FILTER_IE_NO_LONGER_PRESENT	BIT(1)
#define WSM_BEACON_FILTER_IE_HAS_APPEARED		BIT(2)
#define WSM_BEACON_FILTER_FRAME_TYPE			BIT(3)
#define WSM_BEACON_FILTER_ACTION_ENABLE			BIT(4)
#define WSM_BEACON_FILTER_OUI					BIT(5)

struct wsm_beacon_filter_table_entry {
	u8	ieId;
	u8	actionFlags;
	u8	oui[3];
	u8	matchData[3];
} __packed;

struct wsm_beacon_filter_table {
	__le32 numOfIEs;
	struct wsm_beacon_filter_table_entry entry[16];
} __packed;

static inline int wsm_set_beacon_filter_table(struct atbm_common *hw_priv,
					struct wsm_beacon_filter_table *ft,
					int if_id)
{
	size_t size = __le32_to_cpu(ft->numOfIEs) *
		     sizeof(struct wsm_beacon_filter_table_entry) +
		     sizeof(__le32);

	return wsm_write_mib(hw_priv, WSM_MIB_ID_BEACON_FILTER_TABLE, ft, size,
			if_id);
}

#define WSM_BEACON_FILTER_ENABLE	BIT(0) /* Enable/disable beacon filtering */
#define WSM_BEACON_FILTER_AUTO_ERP	BIT(1) /* If 1 FW will handle ERP IE changes internally */

struct wsm_beacon_filter_control {
	int enabled;
	int bcn_count;
};

static inline int wsm_beacon_filter_control(struct atbm_common *hw_priv,
					struct wsm_beacon_filter_control *arg,
					int if_id)
{
	struct {
		__le32 enabled;
		__le32 bcn_count;
	} val;
	val.enabled = __cpu_to_le32(arg->enabled);
	val.bcn_count = __cpu_to_le32(arg->bcn_count);
	return wsm_write_mib(hw_priv, WSM_MIB_ID_BEACON_FILTER_ENABLE, &val,
			     sizeof(val), if_id);
}

enum wsm_power_mode {
	wsm_power_mode_active = 0,
	wsm_power_mode_doze = 1,
	wsm_power_mode_quiescent = 2,
};

struct wsm_operational_mode {
	enum wsm_power_mode power_mode;
	int disableMoreFlagUsage;
	int performAntDiversity;
};

static inline int wsm_set_operational_mode(struct atbm_common *hw_priv,
					const struct wsm_operational_mode *arg,
					int if_id)
{
	u32 val = arg->power_mode;
	if (arg->disableMoreFlagUsage)
		val |= BIT(4);
	if (arg->performAntDiversity)
		val |= BIT(5);
	return wsm_write_mib(hw_priv, WSM_MIB_ID_OPERATIONAL_POWER_MODE, &val,
			     sizeof(val), if_id);
}

struct wsm_inactivity {
	u8 max_inactivity;
	u8 min_inactivity;
};

static inline int wsm_set_inactivity(struct atbm_common *hw_priv,
					const struct wsm_inactivity *arg,
					int if_id)
{
	struct {
	       u8	min_inactive;
	       u8	max_inactive;
	       u16	reserved;
	} val;

	val.max_inactive = arg->max_inactivity;
	val.min_inactive = arg->min_inactivity;
	val.reserved = 0;

	return wsm_write_mib(hw_priv, WSM_MIB_ID_SET_INACTIVITY, &val,
			     sizeof(val), if_id);
}

struct wsm_template_frame {
	u8 frame_type;
	u8 rate;
	bool disable;
	struct sk_buff *skb;
};

static inline int wsm_set_template_frame(struct atbm_common *hw_priv,
					 struct wsm_template_frame *arg,
					 int if_id)
{
	int ret;
	u8 *p = atbm_skb_push(arg->skb, 4);
	p[0] = arg->frame_type;
	p[1] = arg->rate;

	if (arg->disable)
		((u16 *) p)[1] = 0;
	else
		((u16 *) p)[1] = __cpu_to_le16(arg->skb->len - 4);

	ret = wsm_write_mib(hw_priv, WSM_MIB_ID_TEMPLATE_FRAME, p,
			    arg->skb->len, if_id);
	
	atbm_skb_pull(arg->skb, 4);
	return ret;
}


struct wsm_protected_mgmt_policy {
	bool protectedMgmtEnable;
	bool unprotectedMgmtFramesAllowed;
	bool encryptionForAuthFrame;
};

static inline int
wsm_set_protected_mgmt_policy(struct atbm_common *hw_priv,
			      struct wsm_protected_mgmt_policy *arg,
			      int if_id)
{
	__le32 val = 0;
	int ret;
	if (arg->protectedMgmtEnable)
		val |= __cpu_to_le32(BIT(0));
	if (arg->unprotectedMgmtFramesAllowed)
		val |= __cpu_to_le32(BIT(1));
	if (arg->encryptionForAuthFrame)
		val |= __cpu_to_le32(BIT(2));
	ret = wsm_write_mib(hw_priv, WSM_MIB_ID_PROTECTED_MGMT_POLICY, &val,
			    sizeof(val), if_id);
	return ret;
}

static inline int wsm_set_block_ack_policy(struct atbm_common *hw_priv,
					   u8 blockAckTxTidPolicy,
					   u8 blockAckRxTidPolicy,
					   int if_id)
{
	struct {
		u8 blockAckTxTidPolicy;
		u8 reserved1;
		u8 blockAckRxTidPolicy;
		u8 reserved2;
	} val = {
		.blockAckTxTidPolicy = blockAckTxTidPolicy,
		.blockAckRxTidPolicy = blockAckRxTidPolicy,
	};
	//printk("blockAckTxTidPolicy=%0x\n",blockAckTxTidPolicy);
	//printk("blockAckRxTidPolicy=%0x\n",blockAckRxTidPolicy);
	atbm_printk_mgmt("%s:blockAckTxTidPolicy(%x),blockAckRxTidPolicy(%x),if_id(%d)\n",__func__,
	blockAckTxTidPolicy,blockAckRxTidPolicy,if_id);
	return wsm_write_mib(hw_priv, WSM_MIB_ID_BLOCK_ACK_POLICY, &val,
			     sizeof(val), if_id);
}

struct wsm_association_mode {
	u8 flags;		/* WSM_ASSOCIATION_MODE_... */
	u8 preambleType;	/* WSM_JOIN_PREAMBLE_... */
	u8 greenfieldMode;	/* 1 for greenfield */
	u8 mpduStartSpacing;
	__le32 basicRateSet;
};

static inline int wsm_set_association_mode(struct atbm_common *hw_priv,
					   struct wsm_association_mode *arg,
					   int if_id)
{
	return wsm_write_mib(hw_priv, WSM_MIB_ID_SET_ASSOCIATION_MODE, arg,
			     sizeof(*arg), if_id);
}
#ifndef CONFIG_RATE_HW_CONTROL
#define WSM_TX_RATE_POLICY_FLAG_TERMINATE_WHEN_FINISHED BIT(2)
#define WSM_TX_RATE_POLICY_FLAG_COUNT_INITIAL_TRANSMIT BIT(3)
struct wsm_set_tx_rate_retry_policy_header {
	u8 numTxRatePolicies;
	u8 reserved[3];
} __packed;

struct wsm_set_tx_rate_retry_policy_policy {
	u8 policyIndex;
	u8 shortRetryCount;
	u8 longRetryCount;
	u8 policyFlags;
	u8 rateRecoveryCount;
	u8 reserved[3];
	__le32 rateCountIndices[3];
} __packed;


struct wsm_set_tx_rate_retry_policy {
	struct wsm_set_tx_rate_retry_policy_header hdr;
	struct wsm_set_tx_rate_retry_policy_policy tbl[8];
} __packed;

static inline int wsm_set_tx_rate_retry_policy(struct atbm_common *hw_priv,
				struct wsm_set_tx_rate_retry_policy *arg,
				int if_id)
{
	size_t size = sizeof(struct wsm_set_tx_rate_retry_policy_header) +
	    arg->hdr.numTxRatePolicies *
	    sizeof(struct wsm_set_tx_rate_retry_policy_policy);
	return wsm_write_mib(hw_priv, WSM_MIB_ID_SET_TX_RATE_RETRY_POLICY, arg,
			     size, if_id);
}
#endif
#ifdef ATBM_SUPPORT_WOW
/* 4.32 SetEtherTypeDataFrameFilter */
struct wsm_ether_type_filter_hdr {
	u8 nrFilters;		/* Up to WSM_MAX_FILTER_ELEMENTS */
	u8 reserved[3];
} __packed;

struct wsm_ether_type_filter {
	u8 filterAction;	/* WSM_FILTER_ACTION_XXX */
	u8 reserved;
	__le16 etherType;	/* Type of ethernet frame */
} __packed;

static inline int wsm_set_ether_type_filter(struct atbm_common *hw_priv,
				struct wsm_ether_type_filter_hdr *arg,
				int if_id)
{
	size_t size = sizeof(struct wsm_ether_type_filter_hdr) +
		arg->nrFilters * sizeof(struct wsm_ether_type_filter);
	return wsm_write_mib(hw_priv, WSM_MIB_ID_SET_ETHERTYPE_DATAFRAME_FILTER,
		arg, size, if_id);
}


/* 4.33 SetUDPPortDataFrameFilter */
struct wsm_udp_port_filter_hdr {
	u8 nrFilters;		/* Up to WSM_MAX_FILTER_ELEMENTS */
	u8 reserved[3];
} __packed;

struct wsm_udp_port_filter {
	u8 filterAction;	/* WSM_FILTER_ACTION_XXX */
	u8 portType;		/* WSM_FILTER_PORT_TYPE_XXX */
	__le16 udpPort;		/* Port number */
} __packed;

static inline int wsm_set_udp_port_filter(struct atbm_common *hw_priv,
				struct wsm_udp_port_filter_hdr *arg,
				int if_id)
{
	size_t size = sizeof(struct wsm_udp_port_filter_hdr) +
		arg->nrFilters * sizeof(struct wsm_udp_port_filter);
	return wsm_write_mib(hw_priv, WSM_MIB_ID_SET_UDPPORT_DATAFRAME_FILTER,
		arg, size, if_id);
}
#endif
/* Undocumented MIBs: */
/* 4.35 P2PDeviceInfo */
#define D11_MAX_SSID_LEN		(32)

struct wsm_p2p_device_type {
	__le16 categoryId;
	u8 oui[4];
	__le16 subCategoryId;
} __packed;

struct wsm_p2p_device_info {
	struct wsm_p2p_device_type primaryDevice;
	u8 reserved1[3];
	u8 devNameSize;
	u8 localDevName[D11_MAX_SSID_LEN];
	u8 reserved2[3];
	u8 numSecDevSupported;
	struct wsm_p2p_device_type secondaryDevices[0];
} __packed;

/* 4.36 SetWCDMABand - WO */
struct wsm_cdma_band {
	u8 WCDMA_Band;
	u8 reserved[3];
} __packed;

/* 4.37 GroupTxSequenceCounter - RO */
struct wsm_group_tx_seq {
	__le32 bits_47_16;
	__le16 bits_15_00;
	__le16 reserved;
} __packed;

/* 4.39 SetHtProtection - WO */
#define WSM_DUAL_CTS_PROT_ENB		(1 << 0)
#define WSM_NON_GREENFIELD_STA		PRESENT(1 << 1)
#define WSM_HT_PROT_MODE__NO_PROT	(0 << 2)
#define WSM_HT_PROT_MODE__NON_MEMBER	(1 << 2)
#define WSM_HT_PROT_MODE__20_MHZ	(2 << 2)
#define WSM_HT_PROT_MODE__NON_HT_MIXED	(3 << 2)
#define WSM_LSIG_TXOP_PROT_FULL		(1 << 4)
#define WSM_LARGE_L_LENGTH_PROT		(1 << 5)

struct wsm_ht_protection {
	__le32 flags;
} __packed;

/* 4.40 GPIO Command - R/W */
#define WSM_GPIO_COMMAND_SETUP	0
#define WSM_GPIO_COMMAND_READ	1
#define WSM_GPIO_COMMAND_WRITE	2
#define WSM_GPIO_COMMAND_RESET	3
#define WSM_GPIO_ALL_PINS	0xFF

struct wsm_gpio_command {
	u8 GPIO_Command;
	u8 pin;
	__le16 config;
} __packed;

/* 4.41 TSFCounter - RO */
struct wsm_tsf_counter {
	__le64 TSF_Counter;
} __packed;

/* 4.43 Keep alive period */
struct wsm_keep_alive_period {
	__le16 keepAlivePeriod;
	u8 reserved[2];
} __packed;

static inline int wsm_keep_alive_period(struct atbm_common *hw_priv,
					int period, int if_id)
{
	struct wsm_keep_alive_period arg = {
		.keepAlivePeriod = __cpu_to_le16(period),
	};
	return wsm_write_mib(hw_priv, WSM_MIB_ID_KEEP_ALIVE_PERIOD,
			&arg, sizeof(arg), if_id);
};

/* BSSID filtering */
struct wsm_set_bssid_filtering {
	u8 filter;
	u8 reserved[3];
} __packed;

static inline int wsm_set_bssid_filtering(struct atbm_common *hw_priv,
					  bool enabled, int if_id)
{
	struct wsm_set_bssid_filtering arg = {
		.filter = !enabled,
	};
	return wsm_write_mib(hw_priv, WSM_MIB_ID_DISABLE_BSSID_FILTER,
			&arg, sizeof(arg), if_id);
}

/* Multicat filtering - 4.5 */
struct wsm_multicast_filter {
	__le32 enable;
	__le32 numOfAddresses;
	u8 macAddress[WSM_MAX_GRP_ADDRTABLE_ENTRIES][ETH_ALEN];
} __packed;

/* Mac Addr Filter Info */
struct wsm_mac_addr_info {
	u8 filter_mode;
	u8 address_mode;
	u8 MacAddr[6];
} __packed;

/* Mac Addr Filter */
struct wsm_mac_addr_filter {
	u8 numfilter;
	u8 action_mode;
	u8 Reserved[2];
	struct wsm_mac_addr_info macaddrfilter[0];
} __packed;

/* Broadcast Addr Filter */
struct wsm_broadcast_addr_filter {
	u8 action_mode;
	u8 nummacaddr;
	u8 filter_mode;
	u8 address_mode;
	u8 MacAddr[6];
} __packed;

static inline int wsm_set_multicast_filter(struct atbm_common *hw_priv,
					   struct wsm_multicast_filter *fp,
					   int if_id)
{
	return wsm_write_mib(hw_priv, WSM_MIB_ID_DOT11_GROUP_ADDRESSES_TABLE,
			     fp, sizeof(*fp), if_id);
}
/* ARP IPv4 filtering - 4.10 */
struct wsm_arp_ipv4_filter {
	__le32 enable;
	__be32 ipv4Address[WSM_MAX_ARP_IP_ADDRTABLE_ENTRIES];
} __packed;

#ifdef IPV6_FILTERING
/* NDP IPv6 filtering */
struct wsm_ndp_ipv6_filter {
	__le32 enable;
	struct in6_addr ipv6Address[WSM_MAX_NDP_IP_ADDRTABLE_ENTRIES];
} __packed;
/* IPV6 Addr Filter Info */
struct wsm_ip6_addr_info {
	u8 filter_mode;
	u8 address_mode;
	u8 Reserved[2];
	u8 ipv6[16];
};

/* IPV6 Addr Filter */
struct wsm_ipv6_filter {
	u8 numfilter;
	u8 action_mode;
	u8 Reserved[2];
	struct wsm_ip6_addr_info ipv6filter[0];
} __packed;
#endif /*IPV6_FILTERING*/
#ifdef CONFIG_ATBM_LMAC_FILTER_IP_FRAME
static inline int wsm_set_arp_ipv4_filter(struct atbm_common *hw_priv,
					  struct wsm_arp_ipv4_filter *fp,
					  int if_id)
{
	return wsm_write_mib(hw_priv, WSM_MIB_ID_ARP_IP_ADDRESSES_TABLE,
			    fp, sizeof(*fp), if_id);
}

#ifdef IPV6_FILTERING
static inline int wsm_set_ndp_ipv6_filter(struct atbm_common *priv,
					  struct wsm_ndp_ipv6_filter *fp,
					  int if_id)
{
	return wsm_write_mib(priv, WSM_MIB_ID_NS_IP_ADDRESSES_TABLE,
			    fp, sizeof(*fp), if_id);
}
#endif /*IPV6_FILTERING*/
#endif
/* P2P Power Save Mode Info - 4.31 */
struct wsm_p2p_ps_modeinfo {
	u8	oppPsCTWindow;
	u8	count;
	u8	reserved;
	u8	dtimCount;
	__le32	duration;
	__le32	interval;
	__le32	startTime;
} __packed;
#ifdef CONFIG_ATBM_SUPPORT_P2P
static inline int wsm_set_p2p_ps_modeinfo(struct atbm_common *hw_priv,
					  struct wsm_p2p_ps_modeinfo *mi,
					  int if_id)
{
	return wsm_write_mib(hw_priv, WSM_MIB_ID_P2P_PS_MODE_INFO,
			     mi, sizeof(*mi), if_id);
}

static inline int wsm_get_p2p_ps_modeinfo(struct atbm_common *hw_priv,
					  struct wsm_p2p_ps_modeinfo *mi,int if_id)
{
	return wsm_read_mib(hw_priv, WSM_MIB_ID_P2P_PS_MODE_INFO,
			    mi, sizeof(*mi),if_id);
}
#endif
/* UseMultiTxConfMessage */

static inline int wsm_use_multi_tx_conf(struct atbm_common *hw_priv,
					bool enabled, int if_id)
{
	__le32 arg = enabled ? __cpu_to_le32(1) : 0;

	return wsm_write_mib(hw_priv, WSM_MIB_USE_MULTI_TX_CONF,
			&arg, sizeof(arg), if_id);
}


/* 4.26 SetUpasdInformation */
struct wsm_uapsd_info {
	__le16 uapsdFlags;
	__le16 minAutoTriggerInterval;
	__le16 maxAutoTriggerInterval;
	__le16 autoTriggerStep;
};

static inline int wsm_set_uapsd_info(struct atbm_common *hw_priv,
				     struct wsm_uapsd_info *arg,
				     int if_id)
{
	/* TODO:COMBO:UAPSD will be supported only on one interface */
	return wsm_write_mib(hw_priv, WSM_MIB_ID_SET_UAPSD_INFORMATION,
				arg, sizeof(*arg), if_id);
}

/* 4.22 OverrideInternalTxRate */
struct wsm_override_internal_txrate {
	u8 internalTxRate;
	u8 nonErpInternalTxRate;
	u8 reserved[2];
} __packed;
static inline int
wsm_set_override_internal_txrate(struct atbm_common *hw_priv,
				     struct wsm_override_internal_txrate *arg,
				     int if_id)
{
	return wsm_write_mib(hw_priv, WSM_MIB_ID_OVERRIDE_INTERNAL_TX_RATE,
				arg, sizeof(*arg), if_id);
}
#ifdef MCAST_FWDING
/* 4.51 SetForwardingOffload */
struct wsm_forwarding_offload {
	u8 fwenable;
	u8 flags;
	u8 reserved[2];
} __packed;

static inline int wsm_set_forwarding_offlad(struct atbm_common *hw_priv,
				     struct wsm_forwarding_offload *arg,int if_id)
{
	return wsm_write_mib(hw_priv, WSM_MIB_ID_FORWARDING_OFFLOAD,
				arg, sizeof(*arg),if_id);
}

#endif
#ifdef IPC_AP_USED_11G_NO_RTS
static inline int wsm_set_rts_threshold(struct atbm_common *hw_priv,int if_id)
{
	int ret;
	int val32=2347;
	ret = WARN_ON(wsm_write_mib(hw_priv, WSM_MIB_ID_DOT11_RTS_THRESHOLD,
		&val32, sizeof(val32), if_id));
	return 0;
}
#endif

#define WSM_GENERIC_REQ_ID 0x0003
#define WSM_GENERIC_RESP_ID 0x0403

#define WSM_GENERIC_REQ_CHILD__STA_INFO			(4)	
#define WSM_STA_REQ_FLAGS__TXRATE				BIT(0)
struct wsm_gen_req{
	__le32 req_id;
	u32	   req_len;
	u8	   *params;
};
struct wsm_sta_info_req{
	__le16 flags;
	u8	   mac[6];
}__packed;
struct wsm_sta_info{
	u32 	tx_rate;
}__packed;
int wsm_generic_req(struct atbm_common *hw_priv,const struct wsm_gen_req *arg,void *_buf,size_t buf_size,int if_id);
static inline int atbm_req_sta_info(struct atbm_common *hw_priv,struct wsm_sta_info_req *req,struct wsm_sta_info *info,int if_id)
{
	struct wsm_gen_req arg;
	
	arg.req_id = WSM_GENERIC_REQ_CHILD__STA_INFO;
	arg.params = (u8*)req;
	arg.req_len = sizeof(struct wsm_sta_info_req);

	return wsm_generic_req(hw_priv,&arg,(void *)info,sizeof(struct wsm_sta_info),if_id);
}
/* ******************************************************************** */
/* WSM TX port control							*/

void wsm_lock_tx(struct atbm_common *hw_priv);
void wsm_vif_lock_tx(struct atbm_vif *priv);
void wsm_lock_tx_async(struct atbm_common *hw_priv);
bool wsm_flush_tx(struct atbm_common *hw_priv);
bool wsm_vif_flush_tx(struct atbm_vif *priv);
void wsm_unlock_tx(struct atbm_common *hw_priv);

/* ******************************************************************** */
/* WSM / BH API								*/

int wsm_handle_exception(struct atbm_common *hw_priv, u8 * data, u32 len);
int wsm_handle_rx(struct atbm_common *hw_priv, int id, struct wsm_hdr *wsm,
		  struct sk_buff **skb_p);

/* ******************************************************************** */
/* wsm_buf API								*/

struct wsm_buf {
	u8 *begin;
	u8 *data;
	u8 *end;
};

void wsm_buf_init(struct wsm_buf *buf);
void wsm_buf_deinit(struct wsm_buf *buf);

/* ******************************************************************** */
/* wsm_cmd API								*/

struct wsm_cmd {
	spinlock_t lock;
	int done;
	int last_send_cmd;
//	int last_send_end_cmd;
	u8 *ptr;
	u32 len;
	void *arg;
	int ret;
	u16 cmd;
};

/* ******************************************************************** */
/* WSM TX buffer access							*/

int wsm_get_tx(struct atbm_common *hw_priv, u8 **data,
	       u32 *tx_len, int *burst, int *vif_selected);

int wsm_txed(struct atbm_common *hw_priv, u8 *data);

/* ******************************************************************** */
/* Queue mapping: WSM <---> linux					*/
/* Linux: VO VI BE BK							*/
/* WSM:   BE BK VI VO							*/

static inline u8 wsm_queue_id_to_linux(u8 queueId)
{
	static const u8 queue_mapping[] = {
		2, 3, 1, 0
	};
	return queue_mapping[queueId];
}

static inline u8 wsm_queue_id_to_wsm(u8 queueId)
{
	static const u8 queue_mapping[] = {
		3, 2, 0, 1
	};
	return queue_mapping[queueId];
}

int wsm_read_shmem(struct atbm_common *hw_priv, u32 address, void *buffer, size_t buf_size);
int wsm_write_shmem(struct atbm_common *hw_priv, u32 address,size_t size, void *buffer);
#ifdef OPER_CLOCK_USE_SEM
void atbm_pm_timer(unsigned long arg);
#endif
#ifdef ATBM_SUPPORT_WIDTH_40M
#define WSM_SET_CHANTYPE_ID			(0x24)
#define WSM_SET_CHANTYPE_RESP_ID	(0x424)
//used hmac change_bit
#define WSM_SET_CHANTYPECHANGE_FLAGS__CHANNUM_CHANGE		(0)
#define WSM_SET_CHANTYPECHANGE_FLAGS__CHANTYPE_CHANGE		(1)
#define WSM_SET_CHANTYPECHANGE_FLAGS__TOOL_SET_FORCE		(2)
#define WSM_SET_CHANTYPECHANGE_FLAGS__CCA_LEVEL_CHANGE	(3)
#define WSM_SET_CHANTYPECHANGE_FLAGS__CHANGE_PHY_CHTYPE	(4)



//////set to lmac
#define WSM_SET_CHANTYPE_FLAGS__SMARTCONFIG			(4)
#define WSM_SET_CHANTYPE_FLAGS__ETF_GREEDFILED		(6)
#define WSM_SET_CHANTYPE_FLAGS__ETF_TEST_START		(7)
#define WSM_SET_CHANTYPE_PRB_TPC					(5)
struct wsm_set_chantype
{
	u8		band;			//0:2.4G,1:5G
	u8		flag;			//no use
	u16		channelNumber;	// channel number
	u32		channelType;	// channel type
};

#define WSM_GET_CCA_ID			(0x1F)
#define WSM_GET_CCA_RESP_ID		(0x41F)
#define WSM_GET_CCA_FLAGS__START_CCA			(BIT(0))
#define WSM_GET_CCA_FLAGS__STOP_CCA				(BIT(1))
#define WSM_GET_CCA_FLAGS__SET_INIT				(BIT(2))
struct wsm_get_cca_req
{
	u32		flags;
	u32		rx_phy_enable_num_req;
};
struct wsm_get_cca_resp
{
	u32		status;
	u32	    rx_phy_enable_num_cnf;//
	u32	    pri_channel_idle_cnt;//priv channel idle time (util 50us)
	u32	    pri_snd_channel_idle_cnt;//priv channel and snd channel at the same time  idle time (util 50us)
};
#define WSM_SEND_CHTYPE_CHG_REQUEST__FLAGS_SEN_40M	(BIT(1))
#define WSM_SEND_CHTYPE_CHG_REQUEST__FLAGS_SEN_20M	(BIT(0))
struct wsm_req_chtype_change
{
	u8 MacAddr[6];
	u16 flags;
};
struct wsm_req_chtype_change_ind
{
	u32 status;
};
#define WSM_SEND_CHTYPE_CHG_REQUEST_RESP_ID	(0x425)
#define WSM_SEND_CHTYPE_CHG_REQUEST_ID			(0x25)
#define WSM_SEND_CHTYPE_CHG_REQUEST_IND_ID		(0x825)
int wsm_set_chantype_func(struct atbm_common *hw_priv,
				    struct wsm_set_chantype *arg,int if_id);
#ifdef CONFIG_ATBM_40M_AUTO_CCA
int wsm_get_cca(struct atbm_common *hw_priv,struct wsm_get_cca_req *arg,
				struct wsm_get_cca_resp *cca_res,
				int if_id);
int wsm_get_cca_confirm(struct atbm_common *hw_priv,
                                struct wsm_get_cca_resp *arg, struct wsm_buf *buf);
extern int wsm_req_chtype_change_func(struct atbm_common *hw_priv,
											struct wsm_req_chtype_change *arg,int if_id);
int wsm_req_chtype_indication(struct atbm_common *hw_priv,
					 struct wsm_buf *buf);
#endif
void wsm_sync_channl_reset(struct atbm_work_struct *work);
int  sdio_sync_channle_process(struct atbm_common *hw_priv);
#endif
#ifdef ATBM_SDIO_PATCH
#define WSM_SYNC_CHANNLE_ID  0x829
#define WSM_SEQ_BIT_MAP_ID  0x828
struct wsm_tx_release_bufused{
	u32 LmacEsn;
	u32 LmacSsn;
	u32 BitMap_l;
	u32 BitMap_h;
};
struct wsm_bitmap
{
	u32 packetId;
	u32 HmacSsn;
	u32 DataFlag;
};
struct atbm_seq_bit_map 
{
	struct list_head link;
	struct wsm_bitmap bitm;
};

void wsm_sync_channl(struct atbm_work_struct *work);
static inline int ALINE_BYTE(int len,int offset){
	if(len%offset){ 
		len-=(len%offset);
		len+=offset;
	} 
	return len;
}
#endif

#define RECOVERY_STEP1_SUCCESS 1
#define RECOVERY_STEP2_SUCCESS 2
#define RECOVERY_BH_HALT	   3
#define RECOVERY_ERR 			-1
typedef enum{
	OUT_BH=1,
	IN_BH
}BH;
int  wsm_sync_channle_process(struct atbm_common *hw_priv,int type) ;
int wsm_recovery(struct atbm_common *hw_priv);
int  wsm_recovery_done(struct atbm_common *hw_priv,int type);
void wsm_oper_unlock(struct atbm_common *hw_priv);
void wsm_unlock_tx_async(struct atbm_common *hw_priv);
void wsm_wait_pm_sync(struct atbm_common *hw_priv);
int wsm_set_rate_power(struct atbm_common *hw_priv,int use_flag);

#endif /* ATBM_APOLLO_HWIO_H_INCLUDED */
