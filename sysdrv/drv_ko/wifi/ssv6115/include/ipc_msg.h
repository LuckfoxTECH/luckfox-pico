#ifndef __IPC_MSG_H__
#define __IPC_MSG_H__


/*******************************************************************************
 *         Include Files
 ******************************************************************************/
#include "rf_table.h"

/*******************************************************************************
 *         Defines
 ******************************************************************************/
#ifndef SSV_PACKED_STRUCT
//SSV PACK Definition
#define SSV_PACKED_STRUCT_BEGIN
#define SSV_PACKED_STRUCT               //__attribute__ ((packed))
#define SSV_PACKED_STRUCT_END           //__attribute__((packed))
#define SSV_PACKED_STRUCT_STRUCT        __attribute__ ((packed))
#define SSV_PACKED_STRUCT_FIELD(x)      x
#endif

#define REG_FW_VERSION      0x08D00804

/*******************************************************************************
 *         Enumerations
 ******************************************************************************/
typedef enum _en_ipc_type
{
    E_IPC_TYPE_MSG               = 0,
    E_IPC_TYPE_ACK               = 1,
    E_IPC_TYPE_DBG               = 2,
    E_IPC_TYPE_DATA              = 3,
    E_IPC_TYPE_TXCFM             = 4,
    E_IPC_TYPE_AGG_REORD_MSG     = 5,    
    E_IPC_TYPE_DBG_DUMP_START    = 6,
    E_IPC_TYPE_DBG_DUMP_END      = 7,
    E_IPC_TYPE_DBG_LA_TRACE      = 8,
    E_IPC_TYPE_DBG_RHD_DESC      = 9,
    E_IPC_TYPE_DBG_RBD_DESC      = 10,
    E_IPC_TYPE_DBG_TX_DESC       = 11,
    E_IPC_TYPE_DUMP_INFO         = 12,
    E_IPC_TYPE_TX_STOP           = 13,
    E_IPC_TYPE_TX_RESUME         = 14,
    E_IPC_TYPE_PRIV_MSG          = 15,
    E_IPC_TYPE_MAX,
} EN_IPC_TYPE;

//if change must sync to lmac_msg.h of HOST size
enum private_msg_tag
{
    PRIV_SET_AMPDU,
    PRIV_TXTPUT_REQ,
    PRIV_TXTPUT_IND,
    PRIV_RXTPUT_REQ,
    PRIV_RXTPUT_CFM,
    PRIV_MAX,
};

typedef enum en_ipc_priv_msg_type
{
    E_IPC_PRIV_MSG_TYPE_START = 0,
    E_IPC_PRIV_MSG_TYPE_SET_AMPDU,          ///< 1, Set AMPDU
    E_IPC_PRIV_MSG_TYPE_TXTPUT_REQ,         ///< 2, Txtput request
    E_IPC_PRIV_MSG_TYPE_TXTPUT_IND,         ///< 3, Txtput indication
    E_IPC_PRIV_MSG_TYPE_RXTPUT_REQ,         ///< 4, Rxtput request
    E_IPC_PRIV_MSG_TYPE_RXTPUT_IND,         ///< 5, Rxtput indication
    E_IPC_PRIV_MSG_TYPE_SW_ACK_IND,         ///< 6, sw ack ind
    E_IPC_PRIV_MSG_TYPE_FILTER_DUP_RX_REQ,          ///< 7, filter duplicate rx
    E_IPC_PRIV_MSG_TYPE_NIMBLE_OPS,         ///< 8, nimble ops
    E_IPC_PRIV_MSG_TYPE_NIMBLE_EVT,         ///< 9, nimble event
    E_IPC_PRIV_MSG_TYPE_CMD_RFPHY_SET_TABLE = 10,   ///< 10, RF set conf table
    E_IPC_PRIV_MSG_TYPE_CMD_RFPHY_OPS,      ///< 11, RF Tool ops
    E_IPC_PRIV_MSG_TYPE_CMD_EFUSE_OPS,      ///< 12, RF Tool ops
    E_IPC_PRIV_MSG_TYPE_RSP_EFUSE_IND,
    E_IPC_PRIV_MSG_TYPE_CMD_RFPHY_INIT_TABLE,   /// 14, RF set conf table and init rfphy
    E_IPC_PRIV_MSG_TYPE_CMD_AGC_REG_TABLE,  ///  15. Send agc setting
    E_IPC_PRIV_MSG_TYPE_CMD_RFPHY_REG_TABLE,    ///  16. Send rf phy setting
    E_IPC_PRIV_MSG_TYPE_RSP_RF_IND,             ///  17  Response to host
    E_IPC_PRIV_MSG_TYPE_MACADDR_REQ,        /// 18. mac address
    E_IPC_PRIV_MSG_TYPE_CMD_REG_RW,        /// 19. read/write register request from host
    E_IPC_PRIV_MSG_TYPE_RSP_REG_RW,        /// 20. read/write register reponse to host
    E_IPC_PRIV_MSG_TYPE_WMM_FOLLOW_VO_REQ,  /// 21. wmm follow by vo param
    E_IPC_PRIV_MSG_TYPE_IPC_TX_USE_ONE_HWQ = 22,    /// 22. IPC TX uses one HWQ
    E_IPC_PRIV_MSG_TYPE_SET_POLICY_TBL_REQ,  /// 23. set rts method
    E_IPC_PRIV_MSG_TYPE_CMD_RFCALIB_RESULT_BACKUP,    ///  24. Notify Fw backup rf calibration result
    E_IPC_PRIV_MSG_TYPE_TXQ_CREDIT_BOUNDARY,  /// 25. set txq credit boundary
    E_IPC_PRIV_MSG_TYPE_IO_AGGR,            ///< 26, Set IO aggregation
    E_IPC_PRIV_MSG_TYPE_PING_IPC_REQ,             /// 27, ping IPC layer request
    E_IPC_PRIV_MSG_TYPE_PING_IPC_IND,             /// 28, ping IPC layer reply
    E_IPC_PRIV_MSG_TYPE_BLE_INIT,          /// 29. ble init
    E_IPC_PRIV_MSG_TYPE_BLE_OPS,           /// 30. ble packet
    E_IPC_PRIV_MSG_TYPE_BLE_EVT,           /// 31. ble event
    E_IPC_PRIV_MSG_TYPE_SET_MIFI_FEATURE,       ///32, Set MIFI Lowpower
    E_IPC_PRIV_MSG_TYPE_ADAPTIVE_OPS_ENABLE,    ///33, adapt enable
    E_IPC_PRIV_MSG_TYPE_ADAPTIVE_OPS_DISABLE,   ///34, adapt disable
    E_IPC_PRIV_MSG_TYPE_ADAPTIVE_OPS_SET,       ///35, adapt set parameter
    E_IPC_PRIV_MSG_TYPE_MAC_ADDR_RULE,            ///< 36, Set mac addr rule
    E_IPC_PRIV_MSG_TYPE_END = 0xFFFF,
}EN_IPC_PRIV_MSG_TYPE;

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
    SSV6XXX_RFPHY_CMD_RF_TOOL_TX            = 16,             //tx 
    SSV6XXX_RFPHY_CMD_RF_TOOL_STOP          = 17,           //stop
    SSV6XXX_RFPHY_CMD_RF_TOOL_RATE          = 18,
    SSV6XXX_RFPHY_CMD_RF_TOOL_FREQ          = 19,
    SSV6XXX_RFPHY_CMD_RF_TOOL_RX_MIB        = 20,
    SSV6XXX_RFPHY_CMD_RF_TOOL_RX_RESET      = 21,        //rxreset
    SSV6XXX_RFPHY_CMD_RF_TOOL_CH            = 22,            //ch
    SSV6XXX_RFPHY_CMD_RF_TOOL_THERMAL       = 23,
    SSV6XXX_RFPHY_CMD_RF_TOOL_DCDC          = 24,
    SSV6XXX_RFPHY_CMD_RF_TOOL_DACGAIN       = 25,
    SSV6XXX_RFPHY_CMD_RF_TOOL_PADPD         = 26,
    SSV6XXX_RFPHY_CMD_RF_TOOL_WFBLE         = 27,
    SSV6XXX_RFPHY_CMD_RF_TOOL_RATEBGAIN     = 28,
    SSV6XXX_RFPHY_CMD_RF_TONE_GEN           = 29,
    SSV6XXX_RFPHY_CMD_RF_TOOL_HETBRUIDX     = 30,
    SSV6XXX_RFPHY_CMD_RF_TOOL_RATEPOWER     = 31,
    SSV6XXX_RFPHY_CMD_RF_TOOL_ENGMODE       = 32,
    SSV6XXX_RFPHY_CMD_RF_TOOL_TEMPERATURE   = 33,

    SSV6XXX_RFPHY_CMD_SCAN_START            = 50,
    SSV6XXX_RFPHY_CMD_SCAN_DONE,
    SSV6XXX_RFPHY_CMD_RFBLE_TOOL_START,
    SSV6XXX_RFPHY_CMD_RFBLE_TOOL_STOP,
    SSV6XXX_RFPHY_CMD_RFBLE_TOOL_RATE,
    SSV6XXX_RFPHY_CMD_RFBLE_TOOL_CH,
    SSV6XXX_RFPHY_CMD_RFBLE_TOOL_RESET,
    SSV6XXX_RFPHY_CMD_RFBLE_TOOL_COUNT,
    SSV6XXX_RFPHY_CMD_RFBLE_TOOL_DACGAIN,
    SSV6XXX_RFPHY_CMD_RFBLE_TOOL_PATTERN,
} ssv_rf_phy_ops;

typedef enum {
    SSV6XXX_EFUSE_CMD_READ_CHIPID                   = 0,
    SSV6XXX_EFUSE_CMD_READ_MAC                      = 1,
    SSV6XXX_EFUSE_CMD_WRITE_MAC                     = 2,
    SSV6XXX_EFUSE_CMD_READ_TX_POWER1                = 3,
    SSV6XXX_EFUSE_CMD_WRITE_TX_POWER1               = 4,
    SSV6XXX_EFUSE_CMD_READ_XTAL                     = 5,
    SSV6XXX_EFUSE_CMD_WRITE_XTAL                    = 6,
    SSV6XXX_EFUSE_CMD_READ_RATE_GAIN_B_N40          = 7,
    SSV6XXX_EFUSE_CMD_WRITE_RATE_GAIN_B_N40         = 8,
    SSV6XXX_EFUSE_CMD_READ_PSK                      = 9,
    SSV6XXX_EFUSE_CMD_WRITE_PSK                     = 10,
    SSV6XXX_EFUSE_CMD_CHECK_ITEM_FREE               = 11,
    SSV6XXX_EFUSE_CMD_READ_BLE_POWER                = 12,
    SSV6XXX_EFUSE_CMD_WRITE_BLE_POWER               = 13,

    SSV6XXX_EFUSE_CMD_READ_RATE_GAIN_G_N20          = 14,
    SSV6XXX_EFUSE_CMD_WRITE_RATE_GAIN_G_N20         = 15,
    SSV6XXX_EFUSE_CMD_READ_HETB_CAL_RATE_POWER      = 16,
    SSV6XXX_EFUSE_CMD_WRITE_HETB_CAL_RATE_POWER     = 17,
    SSV6XXX_EFUSE_CMD_READ_RATE_GAIN_SU20           = 18,
    SSV6XXX_EFUSE_CMD_WRITE_RATE_GAIN_SU20          = 19,
    SSV6XXX_EFUSE_CMD_READ_RATE_GAIN_SU40           = 20,
    SSV6XXX_EFUSE_CMD_WRITE_RATE_GAIN_SU40          = 21,
    SSV6XXX_EFUSE_CMD_READ_RATE_GAIN_TB20           = 22,
    SSV6XXX_EFUSE_CMD_WRITE_RATE_GAIN_TB20          = 23,
    SSV6XXX_EFUSE_CMD_READ_RATE_GAIN_TB40           = 24,
    SSV6XXX_EFUSE_CMD_WRITE_RATE_GAIN_TB40          = 25,
    SSV6XXX_EFUSE_CMD_READ_USB_PID_VID              = 26,
    SSV6XXX_EFUSE_CMD_WRITE_USB_PID_VID             = 27,
} ssv_efuse_ops;


/*******************************************************************************
 *         Structures
 ******************************************************************************/
SSV_PACKED_STRUCT_BEGIN
typedef struct st_ipc_set_ampdu_req
{
    u32 priv_type;
    u32 length;
    u32 hw_cap;
}SSV_PACKED_STRUCT_STRUCT ST_IPC_SET_AMPDU_REQ;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
typedef struct st_ipc_priv_msg
{
    u32 msgid;
    u32 msglen;
    u8 data[0];
}SSV_PACKED_STRUCT_STRUCT ST_IPC_PRIV_MSG;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
typedef struct st_ipc_tput_param
{
    u32 size;
    u32 looptimes;
}SSV_PACKED_STRUCT_STRUCT ST_IPC_TPUT_PARAM;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
typedef struct st_ipc_filter_dup_rx_param
{
    u32 filter_duplicate_rx;
}SSV_PACKED_STRUCT_STRUCT ST_IPC_FILTER_DUP_RX_PARAM;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
typedef struct st_ipc_wmm_follow_vo_param
{
    u32 wmm_follow_vo;
}SSV_PACKED_STRUCT_STRUCT ST_IPC_WMM_FOLLOW_VO_PARAM;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
typedef struct st_ipc_mac_addr_param
{
    u8 mac_addr0[6];
    u8 mac_addr1[6];
}SSV_PACKED_STRUCT_STRUCT ST_IPC_MAC_ADDR_PARAM;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
typedef struct st_ipc_reg_rw_param
{
    u32 rw_method;
    u32 addr;
    u32 value;
}__packed ST_IPC_REG_RW_PARAM;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
struct ssv_efuse_tool_param {
    u32 efuse_cmd;
    u8 wait_resp;
    u8 resv1;
    u8 xtal;
    u8 psk_len;
    u8 psk[32];
    u32 chip_id;
    u8 mac[6];
    u8 txpower1_gain5200;
    u8 txpower1_gain2000;

    u8 rate_gain_b;
    u8 rate_gain_n40;
    u8 rate_gain_g;
    u8 rate_gain_n20;

    u8 hetb_cal_rate;
    u8 hetb_cal_power;
    u8 rate_gain_su20;
    u8 rate_gain_su40;

    u8 rate_gain_tb20;
    u8 rate_gain_tb40;
    u8 ble_gain;
    u8 check_item_free;

    u16 usb_pid;
    u16 usb_vid;
    u32 resv2;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
struct ssv_rf_tool_param {
    u16 rf_cmd;
    u16 wait_resp;
    u32 interval;
    u32 count;
    u16 rate;
    u16 freq;
    u32 rx_reset:8;
    u32 rx_mib:8;
    u32 ch:8;
    u32 ch_type:8;
    u32 rx_count:16;
    u32 rx_err_count:16;
    u32 disable_thermal:1;
    u32 dcdc:1;
    u32 dacgain:8;
    u32 padpd:4;
    u32 wfble:1;
    u32 ratebgain:8;
    u32 tone_gen:1;
    u32 rssi:8;

    u32 hetbruid:16;
    u32 ratepower:16;

    u32 engmode:8;
    u32 temperature:8;
    u32 reserve1:8;
    u32 reserve2:8;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT
struct ssv_rfble_tool_param {
    u16 rfble_cmd;
    u16 wait_resp;
    u32 count;
    u8  packet_len;
    u8  rate;
    u8  chan;
    u8  interval;
    u16 rx_count;
    u16 rx_err_count;
    u32 dacgain:8;
    u32 pattern:8;
    u32 rsvd0:8;
    u32 rsvd1:8;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT
struct ssv_rf_cali {
    u32 xtal;
    u32 support_5g:4;
    u32 thermal:4;
    u32 greentx_en:1;
    u32 disable_cci:1;
    u32 resv1:6;
    u32 bus_clk:16;
    u32 thermal_low_thershold:16;
    u32 thermal_high_thershold:16;
    u32 resv0;
    struct st_rf_table rf_table;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
typedef struct st_ipc_set_policy_tbl_param
{
    /*set_rts_method:
    3b000: No protection; 
    3b001: Self-CTS;
    3b010: RTS/CTS with intended receiver of this PPDU;
    3b011: RTS/CTS with QAP; 
    3b100: STBC protection;*/
    u8 set_rts_method;

}SSV_PACKED_STRUCT_STRUCT ST_IPC_SET_POLICY_TBL_PARAM;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
typedef struct st_ipc_txq_credit_boundary_param
{
    u32 boundary;
}SSV_PACKED_STRUCT_STRUCT ST_IPC_TXQ_CREDIT_BOUNDARY_PARAM;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
typedef struct st_ipc_io_aggr_param
{
    u32 is_rx;
    u32 enable;
}SSV_PACKED_STRUCT_STRUCT ST_IPC_IO_AGGR_PARAM;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
typedef struct st_mac_addr_rule_param
{
    u32 mac_low_mask;
    u32 mac_high_mask;
}SSV_PACKED_STRUCT_STRUCT ST_MAC_ADDR_RULE_PARAM;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
typedef struct ST_IPC_SET_MIFI
{
    u32 status;
    u32 no_traffic_duration;
}SSV_PACKED_STRUCT_STRUCT ST_IPC_SET_MIFI;
SSV_PACKED_STRUCT_END
// hci bus defines
#define SSV_BLE_HCI_OVER_UART 0
#define SSV_BLE_HCI_OVER_HWIF 1 // USB or SDIO

SSV_PACKED_STRUCT
struct ssv_ble_cfg {
    u8              bus:1;
    u8              hci_reset:1;
    u8              dtm:1;
    u8              rsvd:5;
    u8              bdaddr[6];
    u16             replace_scan_interval;
    u16             replace_scan_win;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

//ipc memory 使用的policy
//驅動ipc msg tx rx 尚未ready前, 可以使用ipc memory和FW交換訊息
//當ipc msg tx rx ready後, 建議使用ke msg和FW交換訊息

#define SSV_IPC_MEM_SIZE 512
#define SSV_CALI_RESULT_SIZE 43
SSV_PACKED_STRUCT
struct st_ipc_mem_layout {
    u32 pattern;
    u32 rf_cali_result[SSV_CALI_RESULT_SIZE];
    u32 rf_cali_mcs9_result[SSV_CALI_RESULT_SIZE];
    u8  reserve[164];
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT
struct st_ipc_adaptive_param {
    u32 interval;
    u32 hit_count;
    u32 total_count;
    u32 threshold;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END
/*******************************************************************************
 *         Variables
 ******************************************************************************/


/*******************************************************************************
 *         Funcations
 ******************************************************************************/


#endif /* __IPC_MSG_H__ */
