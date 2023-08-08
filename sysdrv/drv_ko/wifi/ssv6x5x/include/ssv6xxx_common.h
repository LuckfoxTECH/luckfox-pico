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

/*
 * Common defines and declarations for host driver and firmware for all
 * platforms.
 */

#ifndef __SSV6XXX_COMMON_H__
#define __SSV6XXX_COMMON_H__


#include <rf_table.h>
#include <drv_security_def.h>

#ifndef SSV_PACKED_STRUCT
//SSV PACK Definition
#define SSV_PACKED_STRUCT_BEGIN
#define SSV_PACKED_STRUCT               //__attribute__ ((packed))
#define SSV_PACKED_STRUCT_END           //__attribute__((packed))
#define SSV_PACKED_STRUCT_STRUCT        __attribute__ ((packed))
#define SSV_PACKED_STRUCT_FIELD(x)      x
#endif

#define SSV_RC_MAX_HARDWARE_SUPPORT         1
#define RC_FIRMWARE_REPORT_FLAG             0x80

#define FIRWARE_NOT_MATCH_CODE                  0xF1F1F1F1

/* reserved max sdio align */
#define MAX_RX_PKT_RSVD                     512
/* 30 byte 4 addr hdr, 2 byte QoS, 2304 byte MSDU, 12 byte crypt, 4 byte FCS, 80 byte rx_desc */
#define MAX_FRAME_SIZE                      2432
/* 802.11ad extends maximum MSDU size for DMG (freq > 40Ghz) networks
 * to 3839 or 7920 bytes, see 8.2.3 General frame format
 */
#define MAX_FRAME_SIZE_DMG            		4096

//HCI RX AGG
#define HCI_RX_AGGR_SIZE                   0x2710
#define MAX_HCI_RX_AGGR_SIZE                (HCI_RX_AGGR_SIZE+MAX_FRAME_SIZE)  //AGGR_SIZE+MPDU

// RX mode
#define RX_NORMAL_MODE                  0x0001
#define RX_HW_AGG_MODE                  0x0002
#define RX_HW_AGG_MODE_METH3            0x0004

// Log category
#define LOG_TX_DESC     		0x0001
#define LOG_AMPDU_SSN   		0x0002
#define LOG_AMPDU_DBG   		0x0004
#define LOG_AMPDU_ERR   		0x0008
#define LOG_BEACON      		0x0010
#define LOG_RATE_CONTROL      	0x0020
#define LOG_RATE_REPORT      	0x0040
#define LOG_TX_FRAME            0x0080
#define LOG_RX_DESC             0x0100
#define LOG_HCI             	0x0200
#define LOG_HWIF             	0x0400
#define LOG_HAL                 0x0800
#define LOG_REGW                0x1000
#define LOG_FLASH_BIN           0x2000
#define LOG_KRACK               0x4000
#define LOG_FLOWCTL             0x8000

// Maximum number of frames in AMPDU
#define MAX_AGGR_NUM                            (24)
#define MAX_AGGR_NUM_SETTING(_sh)               (_sh->cfg.max_aggr_size)

// Freddie ToDo: Move firmware multi-rate retry to ssv6200.
#define SSV62XX_TX_MAX_RATES    3
SSV_PACKED_STRUCT_BEGIN
struct fw_rc_retry_params {
    u32 count:4;
    u32 drate:6;
    u32 crate:6;
    u32 rts_cts_nav:16;
    u32 frame_consume_time:10;
    u32 dl_length:12;
    u32 RSVD:10;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

#define TXPB_OFFSET		80
#define RXPB_OFFSET		80
/* Maximum chip ID length */
#define SSV6XXX_CHIP_ID_LENGTH                  (24)
#define SSV6XXX_CHIP_ID_SHORT_LENGTH            (8)

/**
 *  The flag definition for c_type (command type) field of PKTInfo:
 *
 *      @ M0_TXREQ:
 *      @ M1_TXREQ
 *      @ M2_TXREQ
 *      @ M0_RXEVENT
 *      @ M1_RXEVENT
 *      @ HOST_CMD
 *      @ HOST_EVENT
 *
 */
#define M0_TXREQ                            0
#define M1_TXREQ                            1
#define M2_TXREQ                            2
#define M0_RXEVENT                          3
#define M2_RXEVENT                          4
#define HOST_CMD                            5
#define HOST_EVENT                          6
#define RATE_RPT                            7
#ifndef SSV_SUPPORT_HAL
#define SSV6XXX_RX_DESC_LEN                     \
        (sizeof(struct ssv6200_rx_desc) +       \
         sizeof(struct ssv6200_rxphy_info))

#define SSV6XXX_TX_DESC_LEN                     \
        (sizeof(struct ssv6200_tx_desc) + 0)
#endif

#define SSV6XXX_PKT_RUN_TYPE_NOTUSED                0x0     /* 0 */
#define SSV6XXX_PKT_RUN_TYPE_AMPDU_START            0x1     /* 0x1 ~ 0x7f */
#define SSV6XXX_PKT_RUN_TYPE_AMPDU_END              0x7f
#define SSV6XXX_PKT_RUN_TYPE_NULLFUN                0x80    /* 0x80 */
#if 0
typedef enum __PBuf_Type_E {
    NOTYPE_BUF  = 0,
    TX_BUF      = 1,
    RX_BUF      = 2
} PBuf_Type_E;
#endif

#define MAX_STA_NUM                         (8)


/*************************************************************************
 * Host Command
 *************************************************************************/
/**
 *  struct cfg_host_cmd - Host Command Header Format description
 *
 */
SSV_PACKED_STRUCT_BEGIN
typedef struct cfg_host_cmd {
    u32             len:16;
    u32             c_type:3;
    u32             RSVD0:5;//It will be used as command index eg.  STA-WSID[0]-->RSVD0=0, STA-WSID[1]-->RSVD0=1
    u32             h_cmd:8;//------------------------->ssv_host_cmd/command id
    u32             sub_h_cmd;
    u32             cmd_seq_no;
    u32             blocking_seq_no; // If block seq is not zero, it will use blocking mode
    SSV_PACKED_STRUCT_BEGIN
    union { /*lint -save -e157 */
        u32         dummy; // Put a u32 dummy to make MSVC and GCC treat HDR_HostCmd as the same size.
        u8          dat8[4];
        u16         dat16[2];
        u32         dat32[1];
    }SSV_PACKED_STRUCT_STRUCT un; /*lint -restore */
    SSV_PACKED_STRUCT_END
}SSV_PACKED_STRUCT_STRUCT HDR_HostCmd;
SSV_PACKED_STRUCT_END


// Use 100 instead of 0 to get header size to avoid lint from reporting null pointer access.
#define HOST_CMD_HDR_LEN        ((size_t)(((HDR_HostCmd *)100)->un.dat8)-100U)
#define HOST_CMD_DUMMY_LEN 4

SSV_PACKED_STRUCT_BEGIN
struct sdio_rxtput_cfg {
    u32 size_per_frame;
	u32 total_frames;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END


typedef enum {
//===========================================================================
    //Public command
    SSV6XXX_HOST_CMD_START                  = 0,
    SSV6XXX_HOST_CMD_LOG                    = 1, // Firmware log message
    SSV6XXX_HOST_CMD_PS                     = 2, // Power saving
    SSV6XXX_HOST_CMD_INIT_CALI              = 3, // Initial calibration
    SSV6XXX_HOST_CMD_RX_TPUT                = 4, // Interface RX throughput test
    SSV6XXX_HOST_CMD_TX_TPUT		        = 5, // Interface TX throughput test
    SSV6XXX_HOST_CMD_SMART_ICOMM            = 6, // iComm Smartlink
    SSV6XXX_HOST_CMD_WSID_OP                = 7, // Setting software WSID mapping. For Cabrio, only.
    SSV6XXX_HOST_CMD_SET_NOA                = 8, // P2P NoA (Notice of Absence)
    SSV6XXX_HOST_CMD_TX_POLL                = 9, // Regular tx polling
    SSV6XXX_HOST_CMD_SOFT_BEACON            = 10, // software beacon
    SSV6XXX_HOST_CMD_MRX_MODE               = 11, // mrx normal, promiscuous
    SSV6XXX_HOST_CMD_EXTERNAL_PA            = 12, // Set external PA
    SSV6XXX_HOST_CMD_RFPHY_OPS              = 13, // RF/PHY operation
    SSV6XXX_HOST_CMD_SECURITY               = 14, // Security
    SSV6XXX_HOST_CMD_AMPDU_OPS              = 15, // AMPDU operation
    SSV6XXX_HOST_CMD_RC_OPS                 = 16, // RC operation
    SSV6XXX_HOST_CMD_TX_OPS                 = 17, // TX operation
    SSV6XXX_HOST_CMD_VIF_OPS                = 18, // interface operation
    SSV6XXX_HOST_CMD_LOOPBACK               = 19, // loopback operation
    SSV6XXX_HOST_CMD_SNIFFER_CFG            = 20, // Sniffer mode configuration
    SSV6XXX_HOST_CMD_ADAPTIVE               = 21, // adaptive test
	SSV6XXX_HOST_CMD_RTS_THRESHOLD          = 22, // rts threshold
    SSV6XXX_HOST_SOC_CMD_MAXID              = 23  // Not valid host command
//===========================================================================
} ssv6xxx_host_cmd_id;

//-------------------------------------------------------------------------

/*************************************************************************
 * Host Event
 *************************************************************************/

/**
 *  struct cfg_host_event - Host Event Header Format description
 *
 */
SSV_PACKED_STRUCT_BEGIN
typedef struct cfg_host_event {
    u32             len      :16;
    u32             c_type   :3;
    u32             RSVD0    :5;
    u32             h_event  :8;//------------------>ssv_host_evt
    u32             evt_seq_no;
    u32             blocking_seq_no;
    u8              dat[0];
}SSV_PACKED_STRUCT_STRUCT HDR_HostEvent;
SSV_PACKED_STRUCT_END

typedef enum{
//===========================================================================
    //Public event
    SOC_EVT_CMD_RESP                        = 0, // Response of a host command.
    SOC_EVT_SCAN_RESULT                     = 1, // Scan result from probe response or beacon
    SOC_EVT_DEAUTH                          = 2, // Deauthentication received but not for leave command
    SOC_EVT_GET_REG_RESP                    = 3,
    SOC_EVT_NO_BA                           = 4, // BA of an AMPDU is not received.
    SOC_EVT_RC_MPDU_REPORT                  = 5,
    SOC_EVT_RC_AMPDU_REPORT                 = 6,
    SOC_EVT_LOG                             = 7, // Firmware log module soc event
    SOC_EVT_NOA                             = 8,
    SOC_EVT_USER_END                        = 9,
    SOC_EVT_SDIO_TEST_COMMAND               = 10,
    SOC_EVT_RESET_HOST                      = 11,
    SOC_EVT_SDIO_TXTPUT_RESULT              = 12,
    SOC_EVT_TXLOOPBK_RESULT                 = 13,
    SOC_EVT_SMART_ICOMM                     = 14,
    SOC_EVT_BEACON_LOSS                     = 15,
    SOC_EVT_TX_STUCK_RESP                   = 16,
    SOC_EVT_SW_BEACON_RESP                  = 17,
    SOC_EVT_INCORRECT_CHAN_BW               = 18,
    SOC_EVT_PS                              = 19,
    SOC_EVT_RFPHY_OPS                       = 20,
    SOC_EVT_DUMP_PHY_REG                    = 21,
    SOC_EVT_DUMP_RF_REG                     = 22,
    SOC_EVT_SECURITY                        = 23,
    SOC_EVT_AMPDU_OPS                       = 24,
    SOC_EVT_RC_OPS                          = 25,
    SOC_EVT_TX_OPS                          = 26,
    SOC_EVT_VIF_OPS                         = 27,
    SOC_EVT_FW_NOTIFY                       = 28,
    SOC_EVT_LOOPBACK                        = 29,
    SOC_EVT_SNIFFER_NOTIFY                  = 30,
    SOC_EVT_SW_ACK_NOTIFY                   = 31,
    SOC_EVT_ADAPTIVE                        = 32,
    //===========================================================================
    //Private    event
    SOC_EVT_MAXID                           = 33
} ssv6xxx_soc_event;


SSV_PACKED_STRUCT_BEGIN
struct ssv6xxx_rts_param {
    u32 rts_threshold;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
struct ssv6xxx_sw_ack_evt {
    u8 success;
    u8 seq_no;
    u16 rsvd;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

enum SSV6XXX_SOFT_BEACON_TYPE
{
    SSV6XXX_SOFT_BEACON_START = 0,
    SSV6XXX_SOFT_BEACON_STOP = 1,
    SSV6XXX_SOFT_BEACON_SET_INTERVAL = 2,
    SSV6XXX_SOFT_BEACON_FILL_CONTEXT = 3
};

SSV_PACKED_STRUCT_BEGIN
struct ssv6xxx_sw_beacon_params {
    u32              beacon_id_maps:16;
    u32              force_tim_to_high:1;
    u32              rsvd:15;
    u32              interval:16;
    u32              dtim_cnt:8;
    u32              rsvd1:8;
    u32              dtim_offset:16;
    u32              beacon_len:16;
    u8               beacon_context[0]; // must be last
} SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

enum SSV6XXX_IF_TYPE {
    SSV6XXX_IF_NONE     = 0,
    SSV6XXX_IF_USB      = 1,
    SSV6XXX_IF_SDIO     = 2,
    SSV6XXX_IF_SPI      = 3
};

typedef enum {
    SSV6XXX_PS_DOZE,
    SSV6XXX_PS_WAKEUP,
    SSV6XXX_PS_WAKEUP_FINISH,
    SSV6XXX_PS_WAKEUP_FIN_ACK,
    SSV6XXX_PS_HOLD_ON3_ACK
} SSV6XXX_PS_OPS;

enum SSV6XXX_PS_WOWLAN {
    SSV6XXX_PS_WOWLAN_ANY = (1<<0),
    SSV6XXX_PS_WOWLAN_MAGIC_PKT = (1<<1)
};

SSV_PACKED_STRUCT_BEGIN
struct ssv6xxx_ps_params {
    u32              if_type;
    u16              ops;
    u16              aid;
    u32              wowlan_type;
    u8               chan;
    u8               chan_type;
    u8               sta_mac[6];
    u8               bssid[6];
    u32              host_ipv4_addr;
    u8               wsid;
    u8               vif_idx;
    u8               powermode;
    u32              wakeuppin:1;
    u32              userdtim:6;
    u32              ignoredisconnection:1;
    u32              keepalive;
    u32              sleep_gpio_ctrl;
    u32              dormant_gpio_ctrl;
    u32              dataretx_guard:8;
    u32              beaconloss_guard:8;
    u32              beaconloss_maxcnt:8;
    u32              seqno:8;
    u32              reserved2;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

enum SSV6XXX_RETURN_STATE
{
    SSV6XXX_STATE_OK,
    SSV6XXX_STATE_NG,
    SSV6XXX_STATE_MAX
};

#ifdef FW_WSID_WATCH_LIST
enum SSV6XXX_WSID_OPS
{
    SSV6XXX_WSID_OPS_ADD,
    SSV6XXX_WSID_OPS_DEL,
    SSV6XXX_WSID_OPS_RESETALL,
    SSV6XXX_WSID_OPS_ENABLE_CAPS,
    SSV6XXX_WSID_OPS_DISABLE_CAPS,
    SSV6XXX_WSID_OPS_HWWSID_PAIRWISE_SET_TYPE,
    SSV6XXX_WSID_OPS_HWWSID_GROUP_SET_TYPE,
    SSV6XXX_WSID_OPS_MAX
};

enum SSV6XXX_WSID_SEC_TYPE
{
    SSV6XXX_WSID_SEC_SW,
    SSV6XXX_WSID_SEC_HW,
    SSV6XXX_WSID_SEC_TYPE_MAX
};

SSV_PACKED_STRUCT_BEGIN
struct ssv6xxx_wsid_params
{
    u8 cmd;
    u8 wsid_idx;
    u8 target_wsid[6];
    u8 hw_security;
    //identify if the target need hw security. So far it only decribes
    //the rx hw security support. It may extend to TX/RX for different security methods
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END
#endif // FW_WSID_WATCH_LIST

enum SSV6XXX_TX_POLL_TYPE
{
    SSV6XXX_TX_POLL_START = 0,
    SSV6XXX_TX_POLL_RESET = 1,
    SSV6XXX_TX_POLL_STOP = 2
};

enum SSV6XXX_MRX_MODE_TYPE
{
    SSV6XXX_MRX_NORMAL = 0,
    SSV6XXX_MRX_PROMISCUOUS = 1
};

SSV_PACKED_STRUCT_BEGIN
struct ssv6xxx_sniffer_notify_params
{
    u32 channel;
    u32 pkt_len;
    u32 ht_signal_23_0;
    u32 ht_signal_47_24;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

#define PHY_SETTING_SIZE sizeof(phy_setting)

SSV_PACKED_STRUCT_BEGIN
struct ssv6xxx_ch_cfg {
	u32 reg_addr;
	u32 ch1_12_value;
	u32 ch13_14_value;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

#define RF_SETTING_SIZE     (sizeof(asic_rf_setting))

/*
    If change defallt value .please recompiler firmware image.
*/
#define MAX_PHY_SETTING_TABLE_SIZE    1920
#define MAX_RF_SETTING_TABLE_SIZE    512

typedef enum {
    SSV6XXX_VOLT_DCDC_CONVERT = 0,
    SSV6XXX_VOLT_LDO_CONVERT = 1
} ssv6xxx_cfg_volt;

typedef enum {
    SSV6XXX_VOLT_33V = 0,
    SSV6XXX_VOLT_42V
} ssv6xxx_cfg_volt_value;

typedef enum {
    SSV6XXX_IQK_CFG_XTAL_26M = 0,
    SSV6XXX_IQK_CFG_XTAL_40M,
    SSV6XXX_IQK_CFG_XTAL_24M,
    SSV6XXX_IQK_CFG_XTAL_25M,
    SSV6XXX_IQK_CFG_XTAL_12M,
    SSV6XXX_IQK_CFG_XTAL_16M,
    SSV6XXX_IQK_CFG_XTAL_20M,
    SSV6XXX_IQK_CFG_XTAL_32M,
    SSV6XXX_IQK_CFG_XTAL_MAX
} ssv6xxx_iqk_cfg_xtal;

typedef enum {
    SSV6XXX_RFPHY_CMD_INIT_PLL_PHY_RF       = 0,
    SSV6XXX_RFPHY_CMD_CHAN                  = 1,
    SSV6XXX_RFPHY_CMD_RF_ENABLE             = 2,
    SSV6XXX_RFPHY_CMD_RF_DISABLE            = 3,
    SSV6XXX_RFPHY_CMD_PHY_ENABLE            = 4,
    SSV6XXX_RFPHY_CMD_PHY_DISABLE           = 5,
    SSV6XXX_RFPHY_CMD_DUMP_PHY_REG          = 6,
    SSV6XXX_RFPHY_CMD_DUMP_RF_REG           = 7,
    SSV6XXX_RFPHY_CMD_TX_PWR                = 8,
    SSV6XXX_RFPHY_CMD_INIT_CALI             = 9,
    SSV6XXX_RFPHY_CMD_RESTORE_CALI          = 10,
    SSV6XXX_RFPHY_CMD_DPD_ENABLE            = 11,
    SSV6XXX_RFPHY_CMD_DPD_DISABLE           = 12,
    SSV6XXX_RFPHY_CMD_RESTORE_DPD           = 13,
    SSV6XXX_RFPHY_CMD_INIT_DPD              = 14,
    SSV6XXX_RFPHY_CMD_INIT_PLL              = 15,
    SSV6XXX_RFPHY_CMD_RF_TOOL_TX            = 16,
    SSV6XXX_RFPHY_CMD_RF_TOOL_STOP          = 17,
    SSV6XXX_RFPHY_CMD_RF_TOOL_RATE          = 18,
    SSV6XXX_RFPHY_CMD_RF_TOOL_FREQ          = 19,
    SSV6XXX_RFPHY_CMD_SCAN_START            = 20,
    SSV6XXX_RFPHY_CMD_SCAN_DONE             = 21,
    SSV6XXX_RFPHY_CMD_RF_TOOL_THERMAL       = 22
} ssv_rf_phy_ops;

typedef enum {
    SSV6XXX_AMPDU_CMD_NONE                  = 0,
    SSV6XXX_AMPDU_CMD_START                 = 1,
    SSV6XXX_AMPDU_CMD_STOP                  = 2,
    SSV6XXX_AMPDU_CMD_MAX                   = 0xffff
} ssv6xxx_ampdu_ops;

SSV_PACKED_STRUCT_BEGIN
struct ssv_ampdu_param {
    u16 tid;
    u8  wsid;
    u8  resv;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

typedef enum {
    SSV6XXX_RC_CMD_NONE                     = 0,
    SSV6XXX_RC_CMD_INIT                     = 1,
    SSV6XXX_RC_CMD_DEINIT                   = 2,
    SSV6XXX_RC_CMD_AUTO_RATE                = 3,
    SSV6XXX_RC_CMD_FIXED_RATE               = 4,
    SSV6XXX_RC_CMD_MAX                      = 0xffff
} ssv6xxx_rc_ops;

typedef enum {
    SSV6XXX_TX_CMD_NONE                     = 0,
    SSV6XXX_TX_CMD_WAIT_NUM                 = 1,
    SSV6XXX_TX_CMD_WAIT_TIME                = 2,
    SSV6XXX_TX_CMD_WAIT_TIME_HT40           = 3,
    SSV6xxx_TX_CMD_CHECK_POINT              = 4,
    SSV6XXX_TX_CMD_CHECK_HWQ_NUM            = 5,
    SSV6XXX_TX_CMD_DURATION                 = 6,
    SSV6XXX_TX_CMD_MAX                      = 0xffff
} ssv6xxx_tx_ops;

SSV_PACKED_STRUCT_BEGIN
struct ssv_tx_duration {
    u32 duration:16;
    u32 period:16;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
struct ssv_rf_cali {
    u32 xtal;
    u32 support_5g:4;
    u32 thermal:4;
    u32 disable_greentx:1;
    u32 disable_cci:1;
    u32 resv0:6;
    u32 bus_clk:16;
    u32 thermal_low_thershold:16;
    u32 thermal_high_thershold:16;
    struct st_rf_table rf_table;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
struct ssv_scan_param {
    u8          vif_idx;
    u8          wsid;
    u8          src_addr[6];
    u8          bssid[6];
    u8          ssid[64];
    u32         ssid_len;
    bool        no_cck;
    u32         ie_len;
    u8          ie[512];
    u32         reserved1;
    u32         reserved2;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
struct ssv_rf_chan {
    u16 chan;
    u8  chan_type;
    u8  off_chan;
    u8  scan;
    u8  passive_chan;
    struct ssv_scan_param scan_param;
    u32 reserved1;
    u32 reserved2;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
struct ssv_rf_tx_pwr {
    u32 band;
    u32 pwr;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
struct ssv_rf_cali_param {
    u8 support_5g;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
struct ssv_rf_tool_param {
    u32 interval;
    u32 count;
    u16 rate;
    u16 freq;
    u32 rsvd0;
    u32 rsvd1;
    u32 disable_thermal:1;
    u32 rsvd2:31;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

typedef enum {
    SSV6XXX_SECURITY_CMD_INIT                           = 0,
    SSV6XXX_SECURITY_CMD_PAIRWIRE_CIPHER_TYPE           = 1,
    SSV6XXX_SECURITY_CMD_GROUP_CIPHER_TYPE              = 2,
    SSV6XXX_SECURITY_CMD_PAIRWIRE_KEY_IDX               = 3,
    SSV6XXX_SECURITY_CMD_GROUP_KEY_IDX                  = 4,
    SSV6XXX_SECURITY_CMD_PAIRWIRE_KEY                   = 5,
    SSV6XXX_SECURITY_CMD_GROUP_KEY                      = 6
} ssv_security_ops;

SSV_PACKED_STRUCT_BEGIN
struct ssv_sec_param {
    u8      vif_idx;
    u8      wsid_idx;
    u8      cipher;
    u8      key_idx;
    u16     alg;
    u16     key_len;
    u8      key[32];
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
struct ssv_rc_param {
    u32 supp_rates;
    u16 rc_mask;
    u16 ht_capabilities_info;
    u8 short_preamble;
    u8 supported_mcs_set[16];
    u8 ht_support;
    u8 rctype;
    u8 wsid;
    u32 reserved1;
    u32 reserved2;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
struct ssv_fw_notify_param {
    bool rate_update;
    u8 cur_rate[MAX_STA_NUM];
    bool status_cnt_update;
    u32 cur_status_cnt;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

typedef enum {
    SSV6XXX_VIF_CMD_ADD                         = 0,
    SSV6XXX_VIF_CMD_DEL                         = 1,
    SSV6XXX_VIF_CMD_CHG                         = 2
} ssv6xxx_vif_ops;

#define SSV6XXX_VIF_TYPE_NONE                   0
#define SSV6XXX_VIF_TYPE_STA                    1
#define SSV6XXX_VIF_TYPE_AP                     2

SSV_PACKED_STRUCT_BEGIN
struct ssv_vif_param {
    u8          mac[6];
    u8          vif_idx;
    u8          type;
    bool        p2p;
    bool        assoc;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

#define SSV_FIRMWARE_PATH_MAX   256
#define SSV_FIRMWARE_MAX        32

#ifdef CONFIG_SSV_SMARTLINK
enum ssv_smart_icomm_cmd
{
    STOP_SMART_ICOMM,
    START_SMART_ICOMM,
    RESET_SMART_ICOMM,
    MAX_SMART_ICOMM
};

SSV_PACKED_STRUCT_BEGIN
struct ssv6xxx_si_cfg {
    u8 ssid[32];
    u8 password[64];
    s32 ssid_len;
    s32 password_len;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END
#endif //CONFIG_SSV_SMARTLINK

/*
 * struct hci_rx_aggr_info - HCI RX Aggregation Format description
 */
SSV_PACKED_STRUCT_BEGIN
struct hci_rx_aggr_info {
    u32             jmp_mpdu_len:16;
    u32             accu_rx_len:16;

    u32             RSVD0:15;
    u32             tx_page_remain:9;
    u32             tx_id_remain:8;

    u32             edca0:4;
    u32             edca1:5;
    u32             edca2:5;
    u32             edca3:5;
    u32             edca4:4;
    u32             edca5:5;
    u32             RSVD1:4;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
struct ssv6xxx_tx_hw_info {
	u32 tx_id_threshold;
	u32 tx_page_threshold;
	u32 tx_lowthreshold_page_trigger;
	u32 tx_lowthreshold_id_trigger;
	u32 bk_txq_size;
	u32 be_txq_size;
	u32 vi_txq_size;
	u32 vo_txq_size;
	u32 manage_txq_size;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
struct ssv6xxx_rx_hw_info {
	u32 rx_id_threshold;
	u32 rx_page_threshold;
	u32 rx_ba_ma_sessions;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

typedef enum {
    SSV6XXX_SNIFFER_MASK_NONE                       = 0x00000000,
    SSV6XXX_SNIFFER_MASK_BEACON                     = 0x00000001,
    SSV6XXX_SNIFFER_MASK_MGMT                       = 0x00000002,
    SSV6XXX_SNIFFER_MASK_BROADCAST_DATA             = 0x00000004,
    SSV6XXX_SNIFFER_MASK_DATA                       = 0x00000008,
    SSV6XXX_SNIFFER_MASK_MAX                        = 0xffffffff
} ssv6xxx_sniffer_mask;

SSV_PACKED_STRUCT_BEGIN
struct ssv6xxx_sniffer_config {
    u32 recv_mask;
    u32 recv_max_len;
    u32 recv_min_len;
    u8 recv_mac_addr[6];
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

typedef enum {
    SSV6XXX_ADAPTIVE_OPS_NONE                   = 0,
    SSV6XXX_ADAPTIVE_OPS_ENABLE                 = 1,
    SSV6XXX_ADAPTIVE_OPS_DISABLE                = 2,
    SSV6XXX_ADAPTIVE_OPS_SET                    = 3,
    SSV6XXX_ADAPTIVE_OPS_MAX                    = 0xffff
} ssv6xxx_adaptive_ops;

SSV_PACKED_STRUCT_BEGIN
struct ssv6xxx_adaptive_param {
	u32  interval;
    u32  hit_count;
    u32  total_count;
    u32  threshold;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

/* Firmware Checksum */
#define ENABLE_FW_SELF_CHECK                1
#define FW_START_SRAM_ADDR                  0x00000000
#define FW_BLOCK_SIZE                       0x800
#define CHECKSUM_BLOCK_SIZE                 1024
#define FW_CHECKSUM_INIT                    (0x12345678)
#define FW_STATUS_MASK                      (0x00FF0000)

//SRAM mode selection
enum SSV_SRAM_MODE{
    SRAM_MODE_ILM_64K_DLM_128K = 0,
    SRAM_MODE_ILM_160K_DLM_32K
};

#endif /* __SSV6XXX_COMMON_H__ */
