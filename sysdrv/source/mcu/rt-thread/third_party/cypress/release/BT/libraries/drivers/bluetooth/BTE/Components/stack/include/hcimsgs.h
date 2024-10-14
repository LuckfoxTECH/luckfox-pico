/*****************************************************************************
**
**  Name        hcimsgs.h
**
**  Function    this file defines Host Controller Interface messages
**
**
**  Copyright (c) 1999-2015, Broadcom Corp., All Rights Reserved.
**  Broadcom Bluetooth Core. Proprietary and confidential.
**
******************************************************************************/

#ifndef HCIMSGS_H
#define HCIMSGS_H

#include "bt_target.h"
#include "hcidefs.h"
#include "bt_types.h"


#ifdef __cplusplus
extern "C" {
#endif

/* Message by message.... */

#define HCIC_GET_UINT8(p, off)    (UINT8)(*((UINT8 *)((p) + 1) + p->offset + 3 + (off)))

#define HCIC_GET_UINT16(p, off)  (UINT16)((*((UINT8 *)((p) + 1) + p->offset + 3 + (off)) + \
                                          (*((UINT8 *)((p) + 1) + p->offset + 3 + (off) + 1) << 8)))

#define HCIC_GET_UINT32(p, off)  (UINT32)((*((UINT8 *)((p) + 1) + p->offset + 3 + (off)) + \
                                          (*((UINT8 *)((p) + 1) + p->offset + 3 + (off) + 1) << 8) + \
                                          (*((UINT8 *)((p) + 1) + p->offset + 3 + (off) + 2) << 16) + \
                                          (*((UINT8 *)((p) + 1) + p->offset + 3 + (off) + 3) << 24)))

#define HCIC_GET_ARRAY(p, off, x, len) \
{ \
    UINT8 *qq = ((UINT8 *)((p) + 1) + p->offset + 3 + (off)); UINT8 *rr = (UINT8 *)x; \
    int ii; for (ii = 0; ii < len; ii++) *rr++ = *qq++; \
}

#define HCIC_GET_ARRAY16(p, off, x) \
{ \
    UINT8 *qq = ((UINT8 *)((p) + 1) + p->offset + 3 + (off)); UINT8 *rr = (UINT8 *)x + 15; \
    int ii; for (ii = 0; ii < 16; ii++) *rr-- = *qq++; \
}

#define HCIC_GET_BDADDR(p, off, x) \
{ \
    UINT8 *qq = ((UINT8 *)((p) + 1) + p->offset + 3 + (off)); UINT8 *rr = (UINT8 *)x + BD_ADDR_LEN - 1; \
    int ii; for (ii = 0; ii < BD_ADDR_LEN; ii++) *rr-- = *qq++; \
}

#define HCIC_GET_DEVCLASS(p, off, x) \
{ \
    UINT8 *qq = ((UINT8 *)((p) + 1) + p->offset + 3 + (off)); UINT8 *rr = (UINT8 *)x + DEV_CLASS_LEN - 1; \
    int ii; for (ii = 0; ii < DEV_CLASS_LEN; ii++) *rr-- = *qq++; \
}

#define HCIC_GET_LAP(p, off, x) \
{ \
    UINT8 *qq = ((UINT8 *)((p) + 1) + p->offset + 3 + (off)); UINT8 *rr = (UINT8 *)x + LAP_LEN - 1; \
    int ii; for (ii = 0; ii < LAP_LEN; ii++) *rr-- = *qq++; \
}

#define HCIC_GET_POINTER(p, off) ((UINT8 *)((p) + 1) + p->offset + 3 + (off))


HCI_API extern BOOLEAN btsnd_hcic_inquiry(const LAP inq_lap, UINT8 duration,
        UINT8 response_cnt);

#define HCIC_PARAM_SIZE_INQUIRY 5


#define HCIC_INQ_INQ_LAP_OFF    0
#define HCIC_INQ_DUR_OFF        3
#define HCIC_INQ_RSP_CNT_OFF    4
/* Inquiry */

/* Inquiry Cancel */
HCI_API extern BOOLEAN btsnd_hcic_inq_cancel(void);

#define HCIC_PARAM_SIZE_INQ_CANCEL   0

/* Periodic Inquiry Mode */
HCI_API extern BOOLEAN btsnd_hcic_per_inq_mode(UINT16 max_period, UINT16 min_period,
        const LAP inq_lap, UINT8 duration,
        UINT8 response_cnt);

#define HCIC_PARAM_SIZE_PER_INQ_MODE    9

#define HCI_PER_INQ_MAX_INTRVL_OFF  0
#define HCI_PER_INQ_MIN_INTRVL_OFF  2
#define HCI_PER_INQ_INQ_LAP_OFF     4
#define HCI_PER_INQ_DURATION_OFF    7
#define HCI_PER_INQ_RSP_CNT_OFF     8
/* Periodic Inquiry Mode */

/* Exit Periodic Inquiry Mode */
HCI_API extern BOOLEAN btsnd_hcic_exit_per_inq(void);

#define HCIC_PARAM_SIZE_EXIT_PER_INQ   0
/* Create Connection */
HCI_API extern BOOLEAN btsnd_hcic_create_conn(BD_ADDR dest, UINT16 packet_types,
        UINT8 page_scan_rep_mode,
        UINT8 page_scan_mode,
        UINT16 clock_offset,
        UINT8 allow_switch);

#define HCIC_PARAM_SIZE_CREATE_CONN  13

#define HCIC_CR_CONN_BD_ADDR_OFF        0
#define HCIC_CR_CONN_PKT_TYPES_OFF      6
#define HCIC_CR_CONN_REP_MODE_OFF       8
#define HCIC_CR_CONN_PAGE_SCAN_MODE_OFF 9
#define HCIC_CR_CONN_CLK_OFF_OFF        10
#define HCIC_CR_CONN_ALLOW_SWITCH_OFF   12
/* Create Connection */

/* Disconnect */
HCI_API extern BOOLEAN btsnd_hcic_disconnect(UINT16 handle, UINT8 reason);

#define HCIC_PARAM_SIZE_DISCONNECT 3

#define HCI_DISC_HANDLE_OFF             0
#define HCI_DISC_REASON_OFF             2
/* Disconnect */

#if BTM_SCO_INCLUDED == TRUE
/* Add SCO Connection */
HCI_API extern BOOLEAN btsnd_hcic_add_SCO_conn(UINT16 handle, UINT16 packet_types);
#endif /* BTM_SCO_INCLUDED */

#define HCIC_PARAM_SIZE_ADD_SCO_CONN    4

#define HCI_ADD_SCO_HANDLE_OFF          0
#define HCI_ADD_SCO_PACKET_TYPES_OFF    2
/* Add SCO Connection */

/* Create Connection Cancel */
HCI_API extern BOOLEAN btsnd_hcic_create_conn_cancel(BD_ADDR dest);

#define HCIC_PARAM_SIZE_CREATE_CONN_CANCEL  6

#define HCIC_CR_CONN_CANCEL_BD_ADDR_OFF     0
/* Create Connection Cancel */

/* Accept Connection Request */
HCI_API extern BOOLEAN btsnd_hcic_accept_conn(BD_ADDR bd_addr, UINT8 role);

#define HCIC_PARAM_SIZE_ACCEPT_CONN     7

#define HCI_ACC_CONN_BD_ADDR_OFF        0
#define HCI_ACC_CONN_ROLE_OFF           6
/* Accept Connection Request */

/* Reject Connection Request */
HCI_API extern BOOLEAN btsnd_hcic_reject_conn(BD_ADDR bd_addr, UINT8 reason);

#define HCIC_PARAM_SIZE_REJECT_CONN      7

#define HCI_REJ_CONN_BD_ADDR_OFF        0
#define HCI_REJ_CONN_REASON_OFF         6
/* Reject Connection Request */

/* Link Key Request Reply */
HCI_API extern BOOLEAN btsnd_hcic_link_key_req_reply(BD_ADDR bd_addr,
        LINK_KEY link_key);

#define HCIC_PARAM_SIZE_LINK_KEY_REQ_REPLY   22

#define HCI_LINK_KEY_REPLY_BD_ADDR_OFF  0
#define HCI_LINK_KEY_REPLY_LINK_KEY_OFF 6
/* Link Key Request Reply  */

/* Link Key Request Neg Reply */
HCI_API extern BOOLEAN btsnd_hcic_link_key_neg_reply(BD_ADDR bd_addr);

#define HCIC_PARAM_SIZE_LINK_KEY_NEG_REPLY   6

#define HCI_LINK_KEY_NEG_REP_BD_ADR_OFF 0
/* Link Key Request Neg Reply  */

/* PIN Code Request Reply */
HCI_API extern BOOLEAN btsnd_hcic_pin_code_req_reply(BD_ADDR bd_addr,
        UINT8 pin_code_len,
        PIN_CODE pin_code);

#define HCIC_PARAM_SIZE_PIN_CODE_REQ_REPLY   23

#define HCI_PIN_CODE_REPLY_BD_ADDR_OFF  0
#define HCI_PIN_CODE_REPLY_PIN_LEN_OFF  6
#define HCI_PIN_CODE_REPLY_PIN_CODE_OFF 7
/* PIN Code Request Reply  */

/* Link Key Request Neg Reply */
HCI_API extern BOOLEAN btsnd_hcic_pin_code_neg_reply(BD_ADDR bd_addr);

#define HCIC_PARAM_SIZE_PIN_CODE_NEG_REPLY   6

#define HCI_PIN_CODE_NEG_REP_BD_ADR_OFF 0
/* Link Key Request Neg Reply  */

/* Change Connection Type */
HCI_API extern BOOLEAN btsnd_hcic_change_conn_type(UINT16 handle, UINT16 packet_types);

#define HCIC_PARAM_SIZE_CHANGE_CONN_TYPE     4

#define HCI_CHNG_PKT_TYPE_HANDLE_OFF    0
#define HCI_CHNG_PKT_TYPE_PKT_TYPE_OFF  2
/* Change Connection Type */

#define HCIC_PARAM_SIZE_CMD_HANDLE      2

#define HCI_CMD_HANDLE_HANDLE_OFF       0

HCI_API extern BOOLEAN btsnd_hcic_auth_request(UINT16 handle);      /* Authentication Request */

/* Set Connection Encryption */
HCI_API extern BOOLEAN btsnd_hcic_set_conn_encrypt(UINT16 handle, BOOLEAN enable);
#define HCIC_PARAM_SIZE_SET_CONN_ENCRYPT     3


#define HCI_SET_ENCRYPT_HANDLE_OFF      0
#define HCI_SET_ENCRYPT_ENABLE_OFF      2
/* Set Connection Encryption */

HCI_API extern BOOLEAN btsnd_hcic_change_link_key(UINT16 handle);   /* Change Connection Link Key */

/* Master Link Key */
HCI_API extern BOOLEAN btsnd_hcic_master_link_key(BOOLEAN key_flag);

#define HCIC_PARAM_SIZE_MASTER_LINK_KEY 1

#define HCI_MASTER_KEY_FLAG_OFF         0
/* Master Link Key */

/* Remote Name Request */
HCI_API extern BOOLEAN btsnd_hcic_rmt_name_req(BD_ADDR bd_addr,
        UINT8 page_scan_rep_mode,
        UINT8 page_scan_mode,
        UINT16 clock_offset);

#define HCIC_PARAM_SIZE_RMT_NAME_REQ   10

#define HCI_RMT_NAME_BD_ADDR_OFF        0
#define HCI_RMT_NAME_REP_MODE_OFF       6
#define HCI_RMT_NAME_PAGE_SCAN_MODE_OFF 7
#define HCI_RMT_NAME_CLK_OFF_OFF        8
/* Remote Name Request */

/* Remote Name Request Cancel */
HCI_API extern BOOLEAN btsnd_hcic_rmt_name_req_cancel(BD_ADDR bd_addr);

#define HCIC_PARAM_SIZE_RMT_NAME_REQ_CANCEL   6

#define HCI_RMT_NAME_CANCEL_BD_ADDR_OFF       0
/* Remote Name Request Cancel */

HCI_API extern BOOLEAN btsnd_hcic_rmt_features_req(UINT16 handle);      /* Remote Features Request */

/* Remote Extended Features */
HCI_API extern BOOLEAN btsnd_hcic_rmt_ext_features(UINT16 handle, UINT8 page_num);

#define HCIC_PARAM_SIZE_RMT_EXT_FEATURES   3

#define HCI_RMT_EXT_FEATURES_HANDLE_OFF    0
#define HCI_RMT_EXT_FEATURES_PAGE_NUM_OFF  2
/* Remote Extended Features */


/* Local Extended Features */
HCI_API extern BOOLEAN btsnd_hcic_read_local_ext_features(UINT8 page_num);

#define HCIC_PARAM_SIZE_LOCAL_EXT_FEATURES     1

#define HCI_LOCAL_EXT_FEATURES_PAGE_NUM_OFF    0
/* Local Extended Features */


HCI_API extern BOOLEAN btsnd_hcic_rmt_ver_req(UINT16 handle);           /* Remote Version Info Request */
HCI_API extern BOOLEAN btsnd_hcic_read_rmt_clk_offset(UINT16 handle);   /* Remote Clock Offset */
HCI_API extern BOOLEAN btsnd_hcic_read_lmp_handle(UINT16 handle);       /* Remote LMP Handle */

HCI_API extern BOOLEAN btsnd_hcic_setup_esco_conn(UINT16 handle,
        UINT32 tx_bw, UINT32 rx_bw,
        UINT16 max_latency, UINT16 voice,
        UINT8 retrans_effort,
        UINT16 packet_types);
#define HCIC_PARAM_SIZE_SETUP_ESCO      17

#define HCI_SETUP_ESCO_HANDLE_OFF       0
#define HCI_SETUP_ESCO_TX_BW_OFF        2
#define HCI_SETUP_ESCO_RX_BW_OFF        6
#define HCI_SETUP_ESCO_MAX_LAT_OFF      10
#define HCI_SETUP_ESCO_VOICE_OFF        12
#define HCI_SETUP_ESCO_RETRAN_EFF_OFF   14
#define HCI_SETUP_ESCO_PKT_TYPES_OFF    15


HCI_API extern BOOLEAN btsnd_hcic_accept_esco_conn(BD_ADDR bd_addr,
        UINT32 tx_bw, UINT32 rx_bw,
        UINT16 max_latency,
        UINT16 content_fmt,
        UINT8 retrans_effort,
        UINT16 packet_types);
#define HCIC_PARAM_SIZE_ACCEPT_ESCO     21

#define HCI_ACCEPT_ESCO_BDADDR_OFF      0
#define HCI_ACCEPT_ESCO_TX_BW_OFF       6
#define HCI_ACCEPT_ESCO_RX_BW_OFF       10
#define HCI_ACCEPT_ESCO_MAX_LAT_OFF     14
#define HCI_ACCEPT_ESCO_VOICE_OFF       16
#define HCI_ACCEPT_ESCO_RETRAN_EFF_OFF  18
#define HCI_ACCEPT_ESCO_PKT_TYPES_OFF   19


HCI_API extern BOOLEAN btsnd_hcic_reject_esco_conn(BD_ADDR bd_addr, UINT8 reason);
#define HCIC_PARAM_SIZE_REJECT_ESCO     7

#define HCI_REJECT_ESCO_BDADDR_OFF      0
#define HCI_REJECT_ESCO_REASON_OFF      6

/* Hold Mode */
HCI_API extern BOOLEAN btsnd_hcic_hold_mode(UINT16 handle, UINT16 max_hold_period,
        UINT16 min_hold_period);

#define HCIC_PARAM_SIZE_HOLD_MODE       6

#define HCI_HOLD_MODE_HANDLE_OFF        0
#define HCI_HOLD_MODE_MAX_PER_OFF       2
#define HCI_HOLD_MODE_MIN_PER_OFF       4
/* Hold Mode */

/* Sniff Mode */
HCI_API extern BOOLEAN btsnd_hcic_sniff_mode(UINT16 handle,
        UINT16 max_sniff_period,
        UINT16 min_sniff_period,
        UINT16 sniff_attempt,
        UINT16 sniff_timeout);

#define HCIC_PARAM_SIZE_SNIFF_MODE      10


#define HCI_SNIFF_MODE_HANDLE_OFF       0
#define HCI_SNIFF_MODE_MAX_PER_OFF      2
#define HCI_SNIFF_MODE_MIN_PER_OFF      4
#define HCI_SNIFF_MODE_ATTEMPT_OFF      6
#define HCI_SNIFF_MODE_TIMEOUT_OFF      8
/* Sniff Mode */

HCI_API extern BOOLEAN btsnd_hcic_exit_sniff_mode(UINT16 handle);       /* Exit Sniff Mode */

/* Park Mode */
HCI_API extern BOOLEAN btsnd_hcic_park_mode(UINT16 handle,
        UINT16 beacon_max_interval,
        UINT16 beacon_min_interval);

#define HCIC_PARAM_SIZE_PARK_MODE       6

#define HCI_PARK_MODE_HANDLE_OFF        0
#define HCI_PARK_MODE_MAX_PER_OFF       2
#define HCI_PARK_MODE_MIN_PER_OFF       4
/* Park Mode */

HCI_API extern BOOLEAN btsnd_hcic_exit_park_mode(UINT16 handle);  /* Exit Park Mode */

/* QoS Setup */
HCI_API extern BOOLEAN btsnd_hcic_qos_setup(UINT16 handle, UINT8 flags,
        UINT8 service_type,
        UINT32 token_rate, UINT32 peak,
        UINT32 latency, UINT32 delay_var);

#define HCIC_PARAM_SIZE_QOS_SETUP       20

#define HCI_QOS_HANDLE_OFF              0
#define HCI_QOS_FLAGS_OFF               2
#define HCI_QOS_SERVICE_TYPE_OFF        3
#define HCI_QOS_TOKEN_RATE_OFF          4
#define HCI_QOS_PEAK_BANDWIDTH_OFF      8
#define HCI_QOS_LATENCY_OFF             12
#define HCI_QOS_DELAY_VAR_OFF           16
/* QoS Setup */

HCI_API extern BOOLEAN btsnd_hcic_role_discovery(UINT16 handle);        /* Role Discovery */

/* Switch Role Request */
HCI_API extern BOOLEAN btsnd_hcic_switch_role(BD_ADDR bd_addr, UINT8 role);

#define HCIC_PARAM_SIZE_SWITCH_ROLE  7

#define HCI_SWITCH_BD_ADDR_OFF          0
#define HCI_SWITCH_ROLE_OFF             6
/* Switch Role Request */

HCI_API extern BOOLEAN btsnd_hcic_read_policy_set(UINT16 handle);       /* Read Policy Settings */

/* Write Policy Settings */
HCI_API extern BOOLEAN btsnd_hcic_write_policy_set(UINT16 handle, UINT16 settings);

#define HCIC_PARAM_SIZE_WRITE_POLICY_SET     4

#define HCI_WRITE_POLICY_HANDLE_OFF          0
#define HCI_WRITE_POLICY_SETTINGS_OFF        2
/* Write Policy Settings */

/* Read Default Policy Settings */
HCI_API extern BOOLEAN btsnd_hcic_read_def_policy_set(void);

#define HCIC_PARAM_SIZE_READ_DEF_POLICY_SET           0
/* Read Default Policy Settings */

/* Write Default Policy Settings */
HCI_API extern BOOLEAN btsnd_hcic_write_def_policy_set(UINT16 settings);

#define HCIC_PARAM_SIZE_WRITE_DEF_POLICY_SET     2

#define HCI_WRITE_DEF_POLICY_SETTINGS_OFF        0
/* Write Default Policy Settings */

/* Flow Specification */
HCI_API extern BOOLEAN btsnd_hcic_flow_specification(UINT16 handle, UINT8 flags,
        UINT8 flow_direct,
        UINT8 service_type,
        UINT32 token_rate,
        UINT32 token_bucket_size,
        UINT32 peak, UINT32 latency);

#define HCIC_PARAM_SIZE_FLOW_SPEC             21

#define HCI_FLOW_SPEC_HANDLE_OFF              0
#define HCI_FLOW_SPEC_FLAGS_OFF               2
#define HCI_FLOW_SPEC_FLOW_DIRECT_OFF         3
#define HCI_FLOW_SPEC_SERVICE_TYPE_OFF        4
#define HCI_FLOW_SPEC_TOKEN_RATE_OFF          5
#define HCI_FLOW_SPEC_TOKEN_BUCKET_SIZE_OFF   9
#define HCI_FLOW_SPEC_PEAK_BANDWIDTH_OFF      13
#define HCI_FLOW_SPEC_LATENCY_OFF             17
/* Flow Specification */

/******************************************
**    Lisbon Features
*******************************************/
#if BTM_SSR_INCLUDED == TRUE
/* Sniff Subrating */
HCI_API extern BOOLEAN btsnd_hcic_sniff_sub_rate(UINT16 handle, UINT16 max_lat,
        UINT16 min_remote_lat,
        UINT16 min_local_lat);

#define HCIC_PARAM_SIZE_SNIFF_SUB_RATE             8

#define HCI_SNIFF_SUB_RATE_HANDLE_OFF              0
#define HCI_SNIFF_SUB_RATE_MAX_LAT_OFF             2
#define HCI_SNIFF_SUB_RATE_MIN_REM_LAT_OFF         4
#define HCI_SNIFF_SUB_RATE_MIN_LOC_LAT_OFF         6
/* Sniff Subrating */

#else   /* BTM_SSR_INCLUDED == FALSE */

#define btsnd_hcic_sniff_sub_rate(handle, max_lat, min_remote_lat, min_local_lat) FALSE

#endif  /* BTM_SSR_INCLUDED */

/* Extended Inquiry Response */
#if (BTM_EIR_SERVER_INCLUDED == TRUE)
HCI_API extern void btsnd_hcic_write_ext_inquiry_response(void *buffer, UINT8 fec_req);

#define HCIC_PARAM_SIZE_EXT_INQ_RESP        241

#define HCIC_EXT_INQ_RESP_FEC_OFF     0
#define HCIC_EXT_INQ_RESP_RESPONSE    1

HCI_API extern BOOLEAN btsnd_hcic_read_ext_inquiry_response(void);   /* Read Extended Inquiry Response */
#else
#define btsnd_hcic_write_ext_inquiry_response(buffer, fec_req)
#define btsnd_hcic_read_ext_inquiry_response() FALSE
#endif
/* Write Simple Pairing Mode */
/**** Simple Pairing Commands ****/
HCI_API extern BOOLEAN btsnd_hcic_write_simple_pairing_mode(UINT8 mode);

#define HCIC_PARAM_SIZE_W_SIMP_PAIR     1

#define HCIC_WRITE_SP_MODE_OFF          0


HCI_API extern BOOLEAN btsnd_hcic_read_simple_pairing_mode(void);

#define HCIC_PARAM_SIZE_R_SIMP_PAIR     0

/* Write Simple Pairing Debug Mode */
HCI_API extern BOOLEAN btsnd_hcic_write_simp_pair_debug_mode(UINT8 debug_mode);

#define HCIC_PARAM_SIZE_SIMP_PAIR_DBUG  1

#define HCIC_WRITE_SP_DBUG_MODE_OFF     0

/* IO Capabilities Response */
HCI_API extern BOOLEAN btsnd_hcic_io_cap_req_reply(BD_ADDR bd_addr, UINT8 capability,
        UINT8 oob_present, UINT8 auth_req);

#define HCIC_PARAM_SIZE_IO_CAP_RESP     9

#define HCI_IO_CAP_BD_ADDR_OFF          0
#define HCI_IO_CAPABILITY_OFF           6
#define HCI_IO_CAP_OOB_DATA_OFF         7
#define HCI_IO_CAP_AUTH_REQ_OFF         8

/* IO Capabilities Req Neg Reply */
HCI_API extern BOOLEAN btsnd_hcic_io_cap_req_neg_reply(BD_ADDR bd_addr, UINT8 err_code);

#define HCIC_PARAM_SIZE_IO_CAP_NEG_REPLY 7

#define HCI_IO_CAP_NR_BD_ADDR_OFF        0
#define HCI_IO_CAP_NR_ERR_CODE           6


typedef struct
{
    UINT8 channel_enable;
    UINT16 rx_center_frequency;
    UINT16 tx_center_frequency;
    UINT16 rx_channel_bandwidth;
    UINT16 tx_channel_bandwidth;
    UINT8 channel_type;
} tHCI_SET_MWS_CHAN_CMD_PARAMS;

HCI_API extern BOOLEAN btsnd_hcic_set_mws_chann_param(const UINT8 local_controller_id,
        const tHCI_SET_MWS_CHAN_CMD_PARAMS *p_hci_set_mws_chan_cmd_params);
#define HCIC_PARAM_SIZE_SET_MWS_CHANN_PARAM     10


typedef struct
{
    UINT16 ext_frame_duration;
    UINT16 ext_frame_sync_assert_offset;
    UINT16 ext_frame_sync_assert_jitter;
    UINT8 ext_num_periods;
    UINT16 duration_array[HCI_MWS_NUM_PERIODS_SUPPORTED];
    UINT8 type_array[HCI_MWS_NUM_PERIODS_SUPPORTED];
}  tHCI_SET_EXT_FRAME_CFG_CMD_PARAMS;

HCI_API extern BOOLEAN btsnd_hcic_set_ext_frame_config(const UINT8 local_controller_id,
        const tHCI_SET_EXT_FRAME_CFG_CMD_PARAMS *p_hci_set_ext_frame_cfg_cmd_params);

#define HCIC_PARAM_SIZE_SET_EXT_FRAME_CONFIG_FIXED  7
#define HCIC_ONE_PERIOD_DESCR_SET_EXT_FRAME_CONFIG  3   /* 2 bytes duration, 1 byte type */


typedef struct
{
    UINT16 rx_assert_offset;
    UINT16 rx_assert_jitter;
    UINT16 rx_deassert_offset;
    UINT16 rx_deassert_jitter;
    UINT16 tx_assert_offset;
    UINT16 tx_assert_jitter;
    UINT16 tx_deassert_offset;
    UINT16 tx_deassert_jitter;
    UINT16 pattern_assert_offset;
    UINT16 pattern_assert_jitter;
    UINT16 inactivity_duration_assert_offset;
    UINT16 inactivity_duration_assert_jitter;
    UINT16 scan_frequency_assert_offset;
    UINT16 scan_frequency_assert_jitter;
    UINT16 priority_assert_offset_request;
} tHCI_SET_MWS_SIGNALING_CMD_PARAMS;

HCI_API extern BOOLEAN btsnd_hcic_set_mws_signaling(const UINT8 local_controller_id,
        const tHCI_SET_MWS_SIGNALING_CMD_PARAMS *p_hci_set_mws_signaling_cmd_params);
#define HCIC_PARAM_SIZE_SET_MWS_SIGNALING     30


typedef struct
{
    UINT8 transport_layer;
    UINT32 to_MWS_baud_rate;
    UINT32 from_MWS_baud_rate;
} tHCI_SET_MWS_TRANSP_LAYER_CMD_PARAMS;

HCI_API extern BOOLEAN btsnd_hcic_set_mws_trans_layer(const UINT8 local_controller_id,
        const tHCI_SET_MWS_TRANSP_LAYER_CMD_PARAMS *p_hci_set_mws_transp_layer_cmd_params);

#define HCIC_PARAM_SIZE_SET_MWS_TRANS_LAYER     9


typedef struct
{
    UINT8 num_scan_frequencies;
    UINT16 scan_frequency_low[HCI_MWS_NUM_SCAN_FREQS_SUPPORTED];
    UINT16 scan_frequency_high[HCI_MWS_NUM_SCAN_FREQS_SUPPORTED];
} tHCI_SET_MWS_SCAN_FREQ_TABLE_CMD_PARAMS;

HCI_API extern BOOLEAN btsnd_hcic_set_mws_scan_freq_table(const UINT8 local_controller_id,
        const tHCI_SET_MWS_SCAN_FREQ_TABLE_CMD_PARAMS *p_hci_set_mws_scan_freq_table_cmd_params);

#define HCIC_PARAM_SIZE_SET_MWS_SCAN_FREQ_TABLE_FIXED   1
#define HCIC_PAIR_FREQS_SET_MWS_SCAN_FREQ_TABLE         4   /* 2 bytes low, 2 bytes high */


typedef struct
{
    UINT8 pattern_index;
    UINT8 pattern_num_intervals;
    UINT16 pattern_interval_duration[HCI_MWS_PATTERNS_NUM_INTERVS_SUPPORTED];
    UINT8 pattern_interval_type[HCI_MWS_PATTERNS_NUM_INTERVS_SUPPORTED];
} tHCI_SET_MWS_PATTERN_CMD_PARAMS;

HCI_API extern BOOLEAN btsnd_hcic_set_mws_pattern_config(const UINT8 local_controller_id,
        const tHCI_SET_MWS_PATTERN_CMD_PARAMS *p_hci_set_mws_pattern_cmd_params);

#define HCIC_PARAM_SIZE_SET_MWS_PATTERN_CONFIG_FIXED    2
#define HCIC_ONE_INTERV_DESCR_SET_MWS_PATTERN_CONFIG    3   /* 2 bytes duration, 1 byte type */


HCI_API extern BOOLEAN btsnd_hcic_get_mws_trans_layer_config(UINT8 local_controller_id);

#define HCIC_PARAM_SIZE_GET_MWS_TRANS_LAYER_CONFIG     0

/* Read Local OOB Data */
HCI_API extern BOOLEAN btsnd_hcic_read_local_oob_data(void);

#define HCIC_PARAM_SIZE_R_LOCAL_OOB     0


#if (BTM_BR_SC_INCLUDED == TRUE)
/* Read Local OOB Extended Data */
HCI_API extern BOOLEAN btsnd_hcic_read_local_oob_ext_data(void);

#define HCIC_PARAM_SIZE_R_LOCAL_OOB_EXT 0
#endif


HCI_API extern BOOLEAN btsnd_hcic_user_conf_reply(BD_ADDR bd_addr, BOOLEAN is_yes);

#define HCIC_PARAM_SIZE_UCONF_REPLY     6

#define HCI_USER_CONF_BD_ADDR_OFF       0


HCI_API extern BOOLEAN btsnd_hcic_user_passkey_reply(BD_ADDR bd_addr, UINT32 value);

#define HCIC_PARAM_SIZE_U_PKEY_REPLY    10

#define HCI_USER_PASSKEY_BD_ADDR_OFF    0
#define HCI_USER_PASSKEY_VALUE_OFF      6


HCI_API extern BOOLEAN btsnd_hcic_user_passkey_neg_reply(BD_ADDR bd_addr);

#define HCIC_PARAM_SIZE_U_PKEY_NEG_REPLY 6

#define HCI_USER_PASSKEY_NEG_BD_ADDR_OFF 0

/* Remote OOB Data Request Reply */
HCI_API extern BOOLEAN btsnd_hcic_rem_oob_reply(BD_ADDR bd_addr, UINT8 *p_c,
        UINT8 *p_r);

#define HCIC_PARAM_SIZE_REM_OOB_REPLY   38

#define HCI_REM_OOB_DATA_BD_ADDR_OFF    0
#define HCI_REM_OOB_DATA_C_OFF          6
#define HCI_REM_OOB_DATA_R_OFF          22

/* Remote OOB Data Request Negative Reply */
HCI_API extern BOOLEAN btsnd_hcic_rem_oob_neg_reply(BD_ADDR bd_addr);

#define HCIC_PARAM_SIZE_REM_OOB_NEG_REPLY   6

#define HCI_REM_OOB_DATA_NEG_BD_ADDR_OFF    0

#if (BTM_BR_SC_INCLUDED == TRUE)
/* Remote OOB Extended Data Request Reply */
HCI_API extern BOOLEAN btsnd_hcic_rem_oob_ext_reply(BD_ADDR bd_addr, UINT8 *p_c_192, UINT8 *p_r_192,
        UINT8 *p_c_256, UINT8 *p_r_256);

#define HCIC_PARAM_SIZE_REM_OOB_EXT_REPLY   70

#define HCI_REM_OOB_EXT_DATA_BD_ADDR_OFF    0
#define HCI_REM_OOB_EXT_DATA_C_192_OFF      6
#define HCI_REM_OOB_EXT_DATA_R_192_OFF      22
#define HCI_REM_OOB_EXT_DATA_C_256_OFF      38
#define HCI_REM_OOB_EXT_DATA_R_256_OFF      54
#endif

/* Read Tx Power Level */
HCI_API extern BOOLEAN btsnd_hcic_read_inq_tx_power(void);

#define HCIC_PARAM_SIZE_R_TX_POWER      0

/* Write Tx Power Level */
HCI_API extern BOOLEAN btsnd_hcic_write_inq_tx_power(INT8 level);

#define HCIC_PARAM_SIZE_W_TX_POWER      1

#define HCIC_WRITE_TX_POWER_LEVEL_OFF   0
/* Read Default Erroneous Data Reporting */
HCI_API extern BOOLEAN btsnd_hcic_read_default_erroneous_data_rpt(void);

#define HCIC_PARAM_SIZE_R_ERR_DATA_RPT      0

/* Write Default Erroneous Data Reporting */
HCI_API extern BOOLEAN btsnd_hcic_write_default_erroneous_data_rpt(UINT8 level);

#define HCIC_PARAM_SIZE_W_ERR_DATA_RPT      1

#define HCIC_WRITE_ERR_DATA_RPT_OFF   0


#if L2CAP_NON_FLUSHABLE_PB_INCLUDED == TRUE
HCI_API extern BOOLEAN btsnd_hcic_enhanced_flush(UINT16 handle, UINT8 packet_type);

#define HCIC_PARAM_SIZE_ENHANCED_FLUSH  3
#endif


HCI_API extern BOOLEAN btsnd_hcic_send_keypress_notif(BD_ADDR bd_addr, UINT8 notif);

#define HCIC_PARAM_SIZE_SEND_KEYPRESS_NOTIF    7

#define HCI_SEND_KEYPRESS_NOTIF_BD_ADDR_OFF    0
#define HCI_SEND_KEYPRESS_NOTIF_NOTIF_OFF      6


HCI_API extern BOOLEAN btsnd_hcic_refresh_encryption_key(UINT16 handle);       /* Refresh Encryption Key */

/**** end of Simple Pairing Commands ****/


HCI_API extern BOOLEAN btsnd_hcic_set_event_mask(UINT8 local_controller_id, BT_EVENT_MASK evt_mask);

#define HCIC_PARAM_SIZE_SET_EVENT_MASK  8
#define HCI_EVENT_MASK_MASK_OFF         0
/* Set Event Mask */

/* Reset */
HCI_API extern BOOLEAN btsnd_hcic_set_event_mask_page_2(UINT8 local_controller_id,
        BT_EVENT_MASK event_mask);

#define HCIC_PARAM_SIZE_SET_EVENT_MASK_PAGE_2   8
#define HCI_EVENT_MASK_MASK_OFF                 0
/* Set Event Mask Page 2 */

/* Reset */
HCI_API extern BOOLEAN btsnd_hcic_reset(UINT8 local_controller_id);

#define HCIC_PARAM_SIZE_RESET           0
/* Reset */

/* Store Current Settings */
#define MAX_FILT_COND   (sizeof (BD_ADDR) + 1)

HCI_API extern BOOLEAN btsnd_hcic_set_event_filter(UINT8 filt_type,
        UINT8 filt_cond_type,
        UINT8 *filt_cond,
        UINT8 filt_cond_len);

#define HCIC_PARAM_SIZE_SET_EVT_FILTER  9

#define HCI_FILT_COND_FILT_TYPE_OFF     0
#define HCI_FILT_COND_COND_TYPE_OFF     1
#define HCI_FILT_COND_FILT_OFF          2
/* Set Event Filter */

HCI_API extern BOOLEAN btsnd_hcic_flush(UINT8 local_controller_id, UINT16 handle);                 /* Flush */

/* Create New Unit Type */
HCI_API extern BOOLEAN btsnd_hcic_new_unit_key(void);

#define HCIC_PARAM_SIZE_NEW_UNIT_KEY     0
/* Create New Unit Type */

/* Read Stored Key */
HCI_API extern BOOLEAN btsnd_hcic_read_stored_key(BD_ADDR bd_addr,
        BOOLEAN read_all_flag);

#define HCIC_PARAM_SIZE_READ_STORED_KEY 7

#define HCI_READ_KEY_BD_ADDR_OFF        0
#define HCI_READ_KEY_ALL_FLAG_OFF       6
/* Read Stored Key */

#define MAX_WRITE_KEYS 10
/* Write Stored Key */
HCI_API extern BOOLEAN btsnd_hcic_write_stored_key(UINT8 num_keys, BD_ADDR *bd_addr,
        LINK_KEY *link_key);

#define HCIC_PARAM_SIZE_WRITE_STORED_KEY  sizeof(btmsg_hcic_write_stored_key_t)

#define HCI_WRITE_KEY_NUM_KEYS_OFF          0
#define HCI_WRITE_KEY_BD_ADDR_OFF           1
#define HCI_WRITE_KEY_KEY_OFF               7
/* only 0x0b keys cab be sent in one HCI command */
#define HCI_MAX_NUM_OF_LINK_KEYS_PER_CMMD   0x0b
/* Write Stored Key */

/* Delete Stored Key */
HCI_API extern BOOLEAN btsnd_hcic_delete_stored_key(BD_ADDR bd_addr, BOOLEAN delete_all_flag);

#define HCIC_PARAM_SIZE_DELETE_STORED_KEY        7

#define HCI_DELETE_KEY_BD_ADDR_OFF      0
#define HCI_DELETE_KEY_ALL_FLAG_OFF     6
/* Delete Stored Key */

/* Change Local Name */
HCI_API extern BOOLEAN btsnd_hcic_change_name(BD_NAME name);

#define HCIC_PARAM_SIZE_CHANGE_NAME     BD_NAME_LEN

#define HCI_CHANGE_NAME_NAME_OFF        0
/* Change Local Name */


#define HCIC_PARAM_SIZE_READ_CMD     0

#define HCIC_PARAM_SIZE_WRITE_PARAM1     1

#define HCIC_WRITE_PARAM1_PARAM_OFF      0

#define HCIC_PARAM_SIZE_WRITE_PARAM2     2

#define HCIC_WRITE_PARAM2_PARAM_OFF      0

#define HCIC_PARAM_SIZE_WRITE_PARAM3     3

#define HCIC_WRITE_PARAM3_PARAM_OFF      0

#define HCIC_PARAM_SIZE_SET_AFH_CHANNELS    10

#define HCIC_PARAM_SIZE_ENH_SET_ESCO_CONN   59
#define HCIC_PARAM_SIZE_ENH_ACC_ESCO_CONN   63

HCI_API extern BOOLEAN btsnd_hcic_read_pin_type(void);                          /* Read PIN Type */
HCI_API extern BOOLEAN btsnd_hcic_write_pin_type(UINT8 type);                   /* Write PIN Type */
HCI_API extern BOOLEAN btsnd_hcic_read_auto_accept(void);                       /* Read Auto Accept */
HCI_API extern BOOLEAN btsnd_hcic_write_auto_accept(UINT8 flag);                /* Write Auto Accept */
HCI_API extern BOOLEAN btsnd_hcic_read_name(void);                              /* Read Local Name */
HCI_API extern BOOLEAN btsnd_hcic_read_conn_acc_tout(UINT8 local_controller_id);       /* Read Connection Accept Timout */
HCI_API extern BOOLEAN btsnd_hcic_write_conn_acc_tout(UINT8 local_controller_id, UINT16 tout); /* Write Connection Accept Timout */
HCI_API extern BOOLEAN btsnd_hcic_read_page_tout(void);                         /* Read Page Timout */
HCI_API extern BOOLEAN btsnd_hcic_write_page_tout(UINT16 timeout);              /* Write Page Timout */
HCI_API extern BOOLEAN btsnd_hcic_read_scan_enable(void);                       /* Read Scan Enable */
HCI_API extern BOOLEAN btsnd_hcic_write_scan_enable(UINT8 flag);                /* Write Scan Enable */
HCI_API extern BOOLEAN btsnd_hcic_read_pagescan_cfg(void);                      /* Read Page Scan Activity */

HCI_API extern BOOLEAN btsnd_hcic_write_pagescan_cfg(UINT16 interval,
        UINT16 window);            /* Write Page Scan Activity */

#define HCIC_PARAM_SIZE_WRITE_PAGESCAN_CFG  4

#define HCI_SCAN_CFG_INTERVAL_OFF       0
#define HCI_SCAN_CFG_WINDOW_OFF         2
/* Write Page Scan Activity */

HCI_API extern BOOLEAN btsnd_hcic_read_inqscan_cfg(void);       /* Read Inquiry Scan Activity */

/* Write Inquiry Scan Activity */
HCI_API extern BOOLEAN btsnd_hcic_write_inqscan_cfg(UINT16 interval, UINT16 window);

#define HCIC_PARAM_SIZE_WRITE_INQSCAN_CFG    4

#define HCI_SCAN_CFG_INTERVAL_OFF       0
#define HCI_SCAN_CFG_WINDOW_OFF         2
/* Write Inquiry Scan Activity */

HCI_API extern BOOLEAN btsnd_hcic_read_auth_enable(void);                        /* Read Authentication Enable */
HCI_API extern BOOLEAN btsnd_hcic_write_auth_enable(UINT8 flag);                 /* Write Authentication Enable */
HCI_API extern BOOLEAN btsnd_hcic_read_encr_mode(void);                          /* Read encryption mode */
HCI_API extern BOOLEAN btsnd_hcic_write_encr_mode(UINT8 mode);                   /* Write encryption mode */
HCI_API extern BOOLEAN btsnd_hcic_read_dev_class(void);                          /* Read Class of Device */
HCI_API extern BOOLEAN btsnd_hcic_write_dev_class(DEV_CLASS dev);                /* Write Class of Device */
HCI_API extern BOOLEAN btsnd_hcic_read_voice_settings(void);                     /* Read Voice Settings */
HCI_API extern BOOLEAN btsnd_hcic_write_voice_settings(UINT16 flags);            /* Write Voice Settings */

/* Host Controller to Host flow control */
#define HCI_HOST_FLOW_CTRL_OFF          0
#define HCI_HOST_FLOW_CTRL_ACL_ON       1
#define HCI_HOST_FLOW_CTRL_SCO_ON       2
#define HCI_HOST_FLOW_CTRL_BOTH_ON      3

HCI_API extern BOOLEAN btsnd_hcic_set_host_flow_ctrl(UINT8 value);          /* Enable/disable flow control toward host */


HCI_API extern BOOLEAN btsnd_hcic_read_auto_flush_tout(UINT16 handle);      /* Read Retransmit Timout */

HCI_API extern BOOLEAN btsnd_hcic_write_auto_flush_tout(UINT16 handle,
        UINT16 timeout);    /* Write Retransmit Timout */

#define HCIC_PARAM_SIZE_WRITE_AUTO_FLUSH_TOUT    4

#define HCI_FLUSH_TOUT_HANDLE_OFF       0
#define HCI_FLUSH_TOUT_TOUT_OFF         2

HCI_API extern BOOLEAN btsnd_hcic_read_num_bcast_xmit(void);                    /* Read Num Broadcast Retransmits */
HCI_API extern BOOLEAN btsnd_hcic_write_num_bcast_xmit(UINT8 num);              /* Write Num Broadcast Retransmits */
HCI_API extern BOOLEAN btsnd_hcic_read_hold_mode_act(void);                     /* Read Hold Mode Activity */
HCI_API extern BOOLEAN btsnd_hcic_write_hold_mode_act(UINT8 flags);             /* Write Hold Mode Activity */

HCI_API extern BOOLEAN btsnd_hcic_read_tx_power(UINT16 handle, UINT8 type);     /* Read Tx Power */

#define HCIC_PARAM_SIZE_READ_TX_POWER    3

#define HCI_READ_TX_POWER_HANDLE_OFF    0
#define HCI_READ_TX_POWER_TYPE_OFF      2

/* Read transmit power level parameter */
#define HCI_READ_CURRENT                0x00
#define HCI_READ_MAXIMUM                0x01

HCI_API extern BOOLEAN btsnd_hcic_read_sco_flow_enable(void);                       /* Read Authentication Enable */
HCI_API extern BOOLEAN btsnd_hcic_write_sco_flow_enable(UINT8 flag);                /* Write Authentication Enable */

/* Set Host Buffer Size */
HCI_API extern BOOLEAN btsnd_hcic_set_host_buf_size(UINT16 acl_len,
        UINT8 sco_len,
        UINT16 acl_num,
        UINT16 sco_num);

#define HCIC_PARAM_SIZE_SET_HOST_BUF_SIZE    7

#define HCI_HOST_BUF_SIZE_ACL_LEN_OFF   0
#define HCI_HOST_BUF_SIZE_SCO_LEN_OFF   2
#define HCI_HOST_BUF_SIZE_ACL_NUM_OFF   3
#define HCI_HOST_BUF_SIZE_SCO_NUM_OFF   5


HCI_API extern BOOLEAN btsnd_hcic_host_num_xmitted_pkts(UINT8 num_handles,
        UINT16 *handle,
        UINT16 *num_pkts);         /* Set Host Buffer Size */

#define HCIC_PARAM_SIZE_NUM_PKTS_DONE_SIZE    sizeof(btmsg_hcic_num_pkts_done_t)

#define MAX_DATA_HANDLES        10

#define HCI_PKTS_DONE_NUM_HANDLES_OFF   0
#define HCI_PKTS_DONE_HANDLE_OFF        1
#define HCI_PKTS_DONE_NUM_PKTS_OFF      3

HCI_API extern BOOLEAN btsnd_hcic_read_link_super_tout(UINT8 local_controller_id, UINT16 handle);  /* Read Link Supervision Timeout */

/* Write Link Supervision Timeout */
HCI_API extern BOOLEAN btsnd_hcic_write_link_super_tout(UINT8 local_controller_id, UINT16 handle, UINT16 timeout);

#define HCIC_PARAM_SIZE_WRITE_LINK_SUPER_TOUT        4

#define HCI_LINK_SUPER_TOUT_HANDLE_OFF  0
#define HCI_LINK_SUPER_TOUT_TOUT_OFF    2
/* Write Link Supervision Timeout */

HCI_API extern BOOLEAN btsnd_hcic_read_max_iac(void);                       /* Read Num Supported IAC */
HCI_API extern BOOLEAN btsnd_hcic_read_cur_iac_lap(void);                   /* Read Current IAC LAP */

HCI_API extern BOOLEAN btsnd_hcic_write_cur_iac_lap(UINT8 num_cur_iac,
        const LAP *iac_lap);   /* Write Current IAC LAP */

#define MAX_IAC_LAPS    0x40

#define HCI_WRITE_IAC_LAP_NUM_OFF       0
#define HCI_WRITE_IAC_LAP_LAP_OFF       1
/* Write Current IAC LAP */

/* Read Clock */
HCI_API extern BOOLEAN btsnd_hcic_read_clock(UINT16 handle, UINT8 which_clock);

#define HCIC_PARAM_SIZE_READ_CLOCK      3

#define HCI_READ_CLOCK_HANDLE_OFF       0
#define HCI_READ_CLOCK_WHICH_CLOCK      2
/* Read Clock */

#ifdef TESTER_ENABLE

#define HCIC_PARAM_SIZE_ENTER_TEST_MODE  2

#define HCI_ENTER_TEST_HANDLE_OFF        0

#define HCIC_PARAM_SIZE_TEST_CNTRL          10
#define HCI_TEST_CNTRL_HANDLE_OFF           0
#define HCI_TEST_CNTRL_SCENARIO_OFF         2
#define HCI_TEST_CNTRL_HOPPINGMODE_OFF      3
#define HCI_TEST_CNTRL_TX_FREQ_OFF          4
#define HCI_TEST_CNTRL_RX_FREQ_OFF          5
#define HCI_TEST_CNTRL_PWR_CNTRL_MODE_OFF   6
#define HCI_TEST_CNTRL_POLL_PERIOD_OFF      7
#define HCI_TEST_CNTRL_PKT_TYPE_OFF         8
#define HCI_TEST_CNTRL_LENGTH_OFF           9

#endif

/* type definition */

typedef struct
{
    UINT8   coding_format;      /* Coding Format see hcidefs.h */
    UINT16  company_id;         /* Company ID from BT SIG Assigned Numbers */
    UINT16  vs_codec_id;        /* Vendor Specific Codec ID:  */
} tHCI_CODING_FORMAT;

/* Enhanced setup/accept synchronous connection See BT 4.1 or later HCI spec for details */
typedef struct
{
    UINT32 tx_bw;                           /* TX BW (# of octets/second) */
    UINT32 rx_bw;                           /* RX BW (# of octets/second) */
    tHCI_CODING_FORMAT tx_cf;               /* TX coding format */
    tHCI_CODING_FORMAT rx_cf;               /* RX coding format */
    UINT16 tx_codec_frame_size;             /* TX CODEC frame size (OTA frame size) */
    UINT16 rx_codec_frame_size;             /* RX CODEC frame size (OTA frame size) */
    UINT32 input_bandwidth;                 /* Input BW (nominal rate octets/sec) */
    UINT32 output_bandwidth;                /* Output BW (nominal rate octets/sec) */
    tHCI_CODING_FORMAT input_cf;            /* Input coding format */
    tHCI_CODING_FORMAT output_cf;           /* Output coding format */
    UINT16 input_coded_data_size;           /* Input coded data size (in bits) */
    UINT16 output_coded_data_size;          /* Output coded data size (in bits) */
    UINT8 input_pcm_data_format;            /* Input PCM data format (see hcidefs.h) */
    UINT8 output_pcm_data_format;           /* Output PCM data format (see hcidefs.h) */
    UINT8 input_pcm_payload_msb_position;   /* Input PCM sample payload MSB position */
    UINT8 output_pcm_payload_msb_position;  /* Output PCM sample payload MSB position */
    UINT8 input_data_path;                  /* 0x00 - HCI, or 0x01-0xFE for VS) */
    UINT8 output_data_path;                 /* 0x00 - HCI, or 0x01-0xFE for VS) */
    UINT8 input_transport_unit_size;        /* Input transport unit size */
    UINT8 output_transport_unit_size;       /* Output transport unit size */
    UINT16 max_latency;                     /* Maximum latency (0x4-0xFFFE in msecs) */
    UINT16 packet_types;                    /* Packet Types */
    UINT8 retrans_effort;                   /* 0x00-0x02, 0xFF don't care */
} tHCI_ENH_ESCO_PARAMS;



HCI_API extern BOOLEAN btsnd_hcic_read_page_scan_per(void);                    /* Read Page Scan Period Mode */
HCI_API extern BOOLEAN btsnd_hcic_write_page_scan_per(UINT8 mode);             /* Write Page Scan Period Mode */
HCI_API extern BOOLEAN btsnd_hcic_read_page_scan_mode(void);                   /* Read Page Scan Mode */
HCI_API extern BOOLEAN btsnd_hcic_write_page_scan_mode(UINT8 mode);            /* Write Page Scan Mode */
HCI_API extern BOOLEAN btsnd_hcic_read_local_ver(UINT8 local_controller_id);          /* Read Local Version Info */
HCI_API extern BOOLEAN btsnd_hcic_read_local_supported_cmds(UINT8 local_controller_id);  /* Read Local Supported Commands */
HCI_API extern BOOLEAN btsnd_hcic_read_local_features(void);                   /* Read Local Supported Features */
HCI_API extern BOOLEAN btsnd_hcic_read_buffer_size(void);                      /* Read Local buffer sizes */
HCI_API extern BOOLEAN btsnd_hcic_read_country_code(void);                     /* Read Country Code */
HCI_API extern BOOLEAN btsnd_hcic_read_bd_addr(void);                          /* Read Local BD_ADDR */
HCI_API extern BOOLEAN btsnd_hcic_read_local_supported_codecs(void);            /* Read Local supported codecs (CSA2) */

/* Enhanced setup SCO connection (CSA2) */
HCI_API extern BOOLEAN btsnd_hcic_enh_set_syn_conn(UINT16 conn_handle,
        tHCI_ENH_ESCO_PARAMS *p_parms);

/* Enhanced accept SCO connection request (CSA2) */
HCI_API extern BOOLEAN btsnd_hcic_enh_acc_syn_conn(BD_ADDR bd_addr,
        tHCI_ENH_ESCO_PARAMS *p_parms);

/* Read Failed Contact Counter */
HCI_API extern BOOLEAN btsnd_hcic_read_fail_contact_count(UINT8 local_controller_id,
        UINT16 handle);
/* Reset Failed Contact Counter */
HCI_API extern BOOLEAN btsnd_hcic_reset_fail_contact_count(UINT8 local_controller_id,
        UINT16 handle);
HCI_API extern BOOLEAN btsnd_hcic_get_link_quality(UINT16 handle);             /* Get Link Quality */
HCI_API extern BOOLEAN btsnd_hcic_read_rssi(UINT16 handle);                    /* Read RSSI */
HCI_API extern BOOLEAN btsnd_hcic_read_loopback_mode(void);                    /* Read Loopback Mode */
HCI_API extern BOOLEAN btsnd_hcic_write_loopback_mode(UINT8 mode);             /* Write Loopback Mode */
HCI_API extern BOOLEAN btsnd_hcic_enable_test_mode(void);                      /* Enable Device Under Test Mode */
HCI_API extern BOOLEAN btsnd_hcic_write_pagescan_type(UINT8 type);             /* Write Page Scan Type */
HCI_API extern BOOLEAN btsnd_hcic_read_pagescan_type(void);                    /* Read Page Scan Type */
HCI_API extern BOOLEAN btsnd_hcic_write_inqscan_type(UINT8 type);              /* Write Inquiry Scan Type */
HCI_API extern BOOLEAN btsnd_hcic_read_inqscan_type(void);                     /* Read Inquiry Scan Type */
HCI_API extern BOOLEAN btsnd_hcic_write_inquiry_mode(UINT8 type);              /* Write Inquiry Mode */
HCI_API extern BOOLEAN btsnd_hcic_read_inquiry_mode(void);                     /* Read Inquiry Mode */
HCI_API extern BOOLEAN btsnd_hcic_set_afh_channels(UINT8 first, UINT8 last);
HCI_API extern BOOLEAN btsnd_hcic_write_afh_channel_assessment_mode(UINT8 mode);
HCI_API extern BOOLEAN btsnd_hcic_set_afh_host_channel_class(const UINT8 afhchannelmap[HCI_AFH_CHANNEL_MAP_LEN]);
HCI_API extern BOOLEAN btsnd_hcic_read_afh_channel_assessment_mode(void);
HCI_API extern BOOLEAN btsnd_hcic_read_afh_channel_map(UINT16 handle);
HCI_API extern BOOLEAN btsnd_hcic_nop(void);                               /* NOP */

#define HCIC_PARAM_SIZE_SET_TRIG_CLOCK_CAP      6

HCI_API extern BOOLEAN btsnd_hcic_set_trig_clock_cap(UINT16 conn_handle, UINT8 enable,
        UINT8 which_clock, UINT8 lpo_allowed,
        UINT8 num_events_to_filter);

/* Send HCI Data */
HCI_API extern void btsnd_hcic_data(BT_HDR *p_buf, UINT16 len, UINT16 handle, UINT8 boundary, UINT8 broadcast);

#define HCI_DATA_HANDLE_MASK 0x0FFF

#define HCID_GET_HANDLE_EVENT(p)  (UINT16)((*((UINT8 *)((p) + 1) + p->offset) + \
                                           (*((UINT8 *)((p) + 1) + p->offset + 1) << 8)))

#define HCID_GET_HANDLE(u16) (UINT16)((u16) & HCI_DATA_HANDLE_MASK)

#define HCI_DATA_EVENT_MASK   3
#define HCI_DATA_EVENT_OFFSET 12
#define HCID_GET_EVENT(u16)   (UINT8)(((u16) >> HCI_DATA_EVENT_OFFSET) & HCI_DATA_EVENT_MASK)

#define HCI_DATA_BCAST_MASK   3
#define HCI_DATA_BCAST_OFFSET 10
#define HCID_GET_BCAST(u16)   (UINT8)(((u16) >> HCI_DATA_BCAST_OFFSET) & HCI_DATA_BCAST_MASK)

#define HCID_GET_ACL_LEN(p)     (UINT16)((*((UINT8 *)((p) + 1) + p->offset + 2) + \
                                         (*((UINT8 *)((p) + 1) + p->offset + 3) << 8)))

#define HCID_HEADER_SIZE      4
/*  Send HCI Data */

#define HCID_GET_SCO_LEN(p)  (*((UINT8 *)((p) + 1) + p->offset + 2))

HCI_API extern void btsnd_hcic_vendor_spec_cmd(
    void *buffer, UINT16 opcode,
    UINT8 len, UINT8 *p_data,
    void *p_cmd_cplt_cback);


/*********************************************************************************
**                                                                              **
**                          H C I    E V E N T S                                **
**                                                                              **
*********************************************************************************/

/* Inquiry Complete Event */
HCI_API extern void btsnd_hcie_inq_comp(void *buffer, UINT8 status);

#define HCIE_PARAM_SIZE_INQ_COMP  1

/* Inquiry Response Event */
HCI_API extern void btsnd_hcie_inq_res(void *buffer, UINT8 num_resp, UINT8 **bd_addr,
                                       UINT8 *page_scan_rep_mode, UINT8 *page_scan_per_mode,
                                       UINT8 *page_scan_mode, UINT8 **dev_class,
                                       UINT16 *clock_offset);

/* Connection Complete Event */
HCI_API extern void btsnd_hcie_connection_comp(void *buffer, UINT8 status, UINT16 handle,
        BD_ADDR bd_addr, UINT8 link_type, UINT8 encr_mode);

#define HCIE_PARAM_SIZE_CONNECTION_COMP    11


#define HCI_LINK_TYPE_SCO               0x00
#define HCI_LINK_TYPE_ACL               0x01

#define HCI_ENCRYPT_MODE_DISABLED       0x00
#define HCI_ENCRYPT_MODE_POINT_TO_POINT 0x01
#define HCI_ENCRYPT_MODE_ALL            0x02


/* Connection Request Event */
HCI_API extern void btsnd_hcie_connection_req(void *buffer, BD_ADDR bd_addr, DEV_CLASS dev_class, UINT8 link_type);

#define HCIE_PARAM_SIZE_CONNECTION_REQ  10

#define HCI_LINK_TYPE_SCO               0x00
#define HCI_LINK_TYPE_ACL               0x01


/* Disonnection Complete Event */
HCI_API extern void btsnd_hcie_disc_comp(void *buffer, UINT8 status, UINT16 handle, UINT8 reason);

#define HCIE_PARAM_SIZE_DISC_COMP  4


/* Authentication Complete Event */
HCI_API extern void btsnd_hcie_auth_comp(void *buffer, UINT8 status, UINT16 handle);

#define HCIE_PARAM_SIZE_AUTH_COMP  3


/* Remote Name Request Complete Event */
HCI_API extern void btsnd_hcie_rmt_name_req_comp(void *buffer, UINT8 status, BD_ADDR bd_addr, BD_NAME name);

#define HCIE_PARAM_SIZE_RMT_NAME_REQ_COMP  (1 + BD_ADDR_LEN + BD_NAME_LEN)


/* Encryption Change Event */
HCI_API extern void btsnd_hcie_encryption_change(void *buffer, UINT8 status, UINT16 handle, BOOLEAN enable);

#define HCIE_PARAM_SIZE_ENCR_CHANGE  4


/* Connection Link Key Change Event */
HCI_API extern void btsnd_hcie_conn_link_key_change(void *buffer, UINT8 status, UINT16 handle);

#define HCIE_PARAM_SIZE_LINK_KEY_CHANGE  3


/* Encryption Key Refresh Complete Event */
HCI_API extern void btsnd_hcie_encrypt_key_refresh(void *buffer, UINT8 status, UINT16 handle);

#define HCIE_PARAM_SIZE_ENCRYPT_KEY_REFRESH  3


/* Master Link Key Complete Event */
HCI_API extern void btsnd_hcie_master_link_key(void *buffer, UINT8 status, UINT16 handle, UINT8 flag);

#define HCIE_PARAM_SIZE_MASTER_LINK_KEY  4


/* Read Remote Supported Features Complete Event */
HCI_API extern void btsnd_hcie_read_rmt_features(void *buffer, UINT8 status, UINT16 handle, UINT8 *features);

#define LMP_FEATURES_SIZE   8
#define HCIE_PARAM_SIZE_READ_RMT_FEATURES  11


/* Read Remote Extended Features Complete Event */
HCI_API extern void btsnd_hcie_read_rmt_ext_features(void *buffer, UINT8 status, UINT16 handle, UINT8 page_num,
        UINT8 max_page_num, UINT8 *features);

#define EXT_LMP_FEATURES_SIZE   8
#define HCIE_PARAM_SIZE_READ_RMT_EXT_FEATURES  13


/* Read Remote Version Complete Event */
HCI_API extern void btsnd_hcie_read_rmt_version(void *buffer, UINT8 status, UINT16 handle, UINT8 version,
        UINT16 comp_name, UINT16 sub_version);

#define HCIE_PARAM_SIZE_READ_RMT_VERSION  8


/* QOS setup complete */
HCI_API extern void btsnd_hcie_qos_setup_compl(void *buffer, UINT8 status, UINT16 handle, UINT8 flags,
        UINT8 service_type, UINT32 token_rate, UINT32 peak,
        UINT32 latency, UINT32 delay_var);

#define HCIE_PARAM_SIZE_QOS_SETUP_COMP 21


/* Flow Specification complete */
HCI_API extern void btsnd_hcie_flow_spec_compl(void *buffer, UINT8 status, UINT16 handle, UINT8 flags,
        UINT8 flow_direction, UINT8 service_type, UINT32 token_rate, UINT32 token_bucket_size,
        UINT32 peak, UINT32 latency);

#define HCIE_PARAM_SIZE_FLOW_SPEC_COMP 22


/*  Command Complete Event */
HCI_API extern void btsnd_hcie_cmd_comp(void *buffer, UINT8 max_host_cmds, UINT16 opcode, UINT8 status);

#define HCIE_PARAM_SIZE_CMD_COMP  4


/*  Command Complete with pre-filled in parameters */
HCI_API extern void btsnd_hcie_cmd_comp_params(void *buffer, UINT8 max_host_cmds, UINT16 cmd_opcode, UINT8 status);

#define HCI_CMD_COMPL_PARAM_OFFSET 4


/*  Command Complete Event with 1-byte param */
HCI_API extern void btsnd_hcie_cmd_comp_param1(void *buffer, UINT8 max_host_cmds, UINT16 opcode,
        UINT8 status, UINT8 param1);

#define HCIE_PARAM_SIZE_CMD_COMP_PARAM1  5

/*  Command Complete Event with 2-byte param */
HCI_API extern void btsnd_hcie_cmd_comp_param2(void *buffer, UINT8 max_host_cmds, UINT16 opcode,
        UINT8 status, UINT16 param2);

#define HCIE_PARAM_SIZE_CMD_COMP_PARAM2  6


/*  Command Complete Event with BD-addr as param */
HCI_API extern void btsnd_hcie_cmd_comp_bd_addr(void *buffer, UINT8 max_host_cmds, UINT16 opcode,
        UINT8 status, BD_ADDR bd_addr);

#define HCIE_PARAM_SIZE_CMD_COMP_BD_ADDR  10

/*  Command Complete Event with supported codecs as param */
HCI_API extern void btsnd_hcie_cmd_comp_read_loc_supp_codecs(void *buffer, UINT8 max_host_cmds, UINT16 opcode,
        UINT8 status);

/* The following para size for test purpose, it should match the size in btsnd_hcie_cmd_comp_read_loc_supp_codecs */
#define HCIE_PARAM_SIZE_CMD_COMP_SUP_CODECS  16


HCI_API extern void btsnd_hcie_cmd_comp_get_WMS_trans_layer_config(void *buffer, UINT8 max_host_cmds, UINT16 opcode, UINT8 status);

#define HCIE_PARAM_SIZE_CMD_COMP_GET_WMS_TRANS_LAYER_CONFIG  33

HCI_API extern void btsnd_hcie_cmd_comp_set_WMS_signaling(void *buffer, UINT8 max_host_cmds, UINT16 opcode, UINT8 status);

#define HCIE_PARAM_SIZE_CMD_COMP_SET_WMS_SIGNALING  36

/*  Command Pending Event */
HCI_API extern void btsnd_hcie_cmd_status(void *buffer, UINT8 status, UINT8 max_host_cmds, UINT16 opcode);

#define HCIE_PARAM_SIZE_CMD_STATUS  4


/*  HW failure Event */
HCI_API extern void btsnd_hcie_hw_failure(void *buffer, UINT8 code);

#define HCIE_PARAM_SIZE_HW_FAILURE 1


/*  Flush Occured Event */
HCI_API extern void btsnd_hcie_flush_occured(void *buffer, UINT16 handle);

#define HCIE_PARAM_SIZE_FLUSH_OCCURED  2


/*  Role Changed Event */
HCI_API extern void btsnd_hcie_role_change(void *buffer, UINT8 status, BD_ADDR bd_addr, UINT8 role);

#define HCIE_PARAM_SIZE_ROLE_CHANGE  8


/* Ready for Data Packets Event */
HCI_API extern void btsnd_hcie_num_compl_pkts(void *buffer, UINT8 num_handles, UINT16 *p_handle, UINT16 *num_pkts);

#define MAX_DATA_HANDLES        10


/* Mode Change Event */
HCI_API extern void btsnd_hcie_mode_change(void *buffer, UINT8 status, UINT16 handle,
        UINT8 mode, UINT16 interval);

#define HCIE_PARAM_SIZE_MODE_CHANGE  6
#define MAX_DATA_HANDLES        10



/* Return Link Keys Event */
HCI_API extern void btsnd_hcie_return_link_keys(void *buffer, UINT8 num_keys, BD_ADDR *bd_addr, LINK_KEY *link_key);

/* This should not be more than 0x0b */
#define MAX_LINK_KEYS 10



/* PIN Code Request Event */
HCI_API extern void btsnd_hcie_pin_code_req(void *buffer, BD_ADDR bd_addr);

#define HCIE_PARAM_SIZE_PIN_CODE_REQ  6



/* Link Key Request Event */
HCI_API extern void btsnd_hcie_link_key_req(void *buffer, BD_ADDR bd_addr);

#define HCIE_PARAM_SIZE_LINK_KEY_REQ  6



/* Link Key Notification Event */
HCI_API extern void btsnd_hcie_link_key_notify(void *buffer, BD_ADDR bd_addr, LINK_KEY link_key, UINT8 key_type);

#define HCIE_PARAM_SIZE_LINK_KEY_NOTIFY  23



/* Loopback Command Event */
HCI_API extern void btsnd_hcie_loopback_command(void *buffer, UINT8 data_len, UINT8 *data);

#define HCIE_PARAM_SIZE_LOOPBACK_COMMAND  sizeof(btmsg_hcie_loopback_cmd_t)



/* Data Buffer Overflow Event */
HCI_API extern void btsnd_hcie_data_buf_overflow(void *buffer, UINT8 link_type);

#define HCIE_PARAM_SIZE_DATA_BUF_OVERFLOW  1



/* Max Slots Change Event */
HCI_API extern void btsnd_hcie_max_slots_change(void *buffer, UINT16 handle, UINT8 max_slots);

#define HCIE_PARAM_SIZE_MAX_SLOTS_CHANGE  3


/* Read Clock Offset Complet Event */
HCI_API extern void btsnd_hcie_read_clock_off_comp(void *buffer, UINT8 status, UINT16 handle,
        UINT16 clock_offset);

#define HCIE_PARAM_SIZE_READ_CLOCK_OFF_COMP  5



/* Connection Packet Type Change Event */
HCI_API extern void btsnd_hcie_pkt_type_change(void *buffer, UINT8 status, UINT16 handle, UINT16 pkt_type);

#define HCIE_PARAM_SIZE_PKT_TYPE_CHANGE  5



/* QOS violation Event */
HCI_API extern void btsnd_hcie_qos_violation(void *buffer, UINT16 handle);

#define HCIE_PARAM_SIZE_QOS_VIOLATION  2



/* Page Scan Mode Change Event */
HCI_API extern void btsnd_hcie_pagescan_mode_chng(void *buffer, BD_ADDR bd_addr, UINT8 mode);

#define HCIE_PARAM_SIZE_PAGE_SCAN_MODE_CHNG  7


/* Page Scan Repetition Mode Change Event */
HCI_API extern void btsnd_hcie_pagescan_rep_mode_chng(void *buffer, BD_ADDR bd_addr, UINT8 mode);

#define HCIE_PARAM_SIZE_PAGE_SCAN_REP_MODE_CHNG  7


/* Sniff Sub Rate Event */
HCI_API extern void btsnd_hcie_sniff_sub_rate(void *buffer, UINT8 status, UINT16 handle, UINT16 max_tx_lat, UINT16 max_rx_lat,
        UINT16 min_remote_timeout, UINT16 min_local_timeout);

#define HCIE_PARAM_SIZE_SNIFF_SUB_RATE  11



/* Extended Inquiry Result Event */
HCI_API extern void btsnd_hcie_ext_inquiry_result(void *buffer, UINT8 num_resp, UINT8 **bd_addr,
        UINT8 *page_scan_rep_mode, UINT8 *reserved,
        UINT8 **dev_class, UINT16 *clock_offset, UINT8 *rssi, UINT8 *p_data);


#if (BLE_INCLUDED == TRUE)
/********************************************************************************
** BLE Commands
**      Note: "local_controller_id" is for transport, not counted in HCI message size
*********************************************************************************/
#define HCIC_BLE_RAND_DI_SIZE                   8
#define HCIC_BLE_ENCRYT_KEY_SIZE                16
#define HCIC_BLE_IRK_SIZE                       16

#define HCIC_PARAM_SIZE_SET_USED_FEAT_CMD       8
#define HCIC_PARAM_SIZE_WRITE_RANDOM_ADDR_CMD    6
#define HCIC_PARAM_SIZE_BLE_WRITE_ADV_PARAMS    15
#define HCIC_PARAM_SIZE_BLE_WRITE_SCAN_RSP      31
#define HCIC_PARAM_SIZE_WRITE_ADV_ENABLE        1
#define HCIC_PARAM_SIZE_BLE_WRITE_SCAN_PARAM    7
#define HCIC_PARAM_SIZE_BLE_WRITE_SCAN_ENABLE   2
#define HCIC_PARAM_SIZE_BLE_CREATE_LL_CONN      25
#define HCIC_PARAM_SIZE_BLE_CREATE_CONN_CANCEL  0
#define HCIC_PARAM_SIZE_CLEAR_WHITE_LIST        0
#define HCIC_PARAM_SIZE_ADD_WHITE_LIST          7
#define HCIC_PARAM_SIZE_REMOVE_WHITE_LIST       7
#define HCIC_PARAM_SIZE_BLE_UPD_LL_CONN_PARAMS  14
#define HCIC_PARAM_SIZE_SET_HOST_CHNL_CLASS     5
#define HCIC_PARAM_SIZE_READ_CHNL_MAP           2
#define HCIC_PARAM_SIZE_BLE_READ_REMOTE_FEAT    2
#define HCIC_PARAM_SIZE_BLE_ENCRYPT             32
#define HCIC_PARAM_SIZE_BLE_RAND                0
#define HCIC_PARAM_SIZE_WRITE_LE_HOST_SUPPORTED 2

#define HCIC_BLE_RAND_DI_SIZE                           8
#define HCIC_BLE_ENCRYT_KEY_SIZE                        16
#define HCIC_PARAM_SIZE_BLE_START_ENC                   (4 + HCIC_BLE_RAND_DI_SIZE + HCIC_BLE_ENCRYT_KEY_SIZE)
#define HCIC_PARAM_SIZE_LTK_REQ_REPLY                   (2 + HCIC_BLE_ENCRYT_KEY_SIZE)
#define HCIC_PARAM_SIZE_LTK_REQ_NEG_REPLY               2
#define HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA              31
#define HCIC_PARAM_SIZE_BLE_RECEIVER_TEST               1
#define HCIC_PARAM_SIZE_BLE_TRANSMITTER_TEST            3
#define HCIC_PARAM_SIZE_BLE_TEST_END                    0
#define HCIC_PARAM_SIZE_BLE_SET_DATA_LENGTH             6
#define HCIC_PARAM_SIZE_BLE_READ_DFLT_DATA_LENGTH       0
#define HCIC_PARAM_SIZE_BLE_WRITE_DFLT_DATA_LENGTH      4
#define HCIC_PARAM_SIZE_BLE_ADD_DEV_RESOLVING_LIST      (7 + HCIC_BLE_IRK_SIZE * 2)
#define HCIC_PARAM_SIZE_BLE_RM_DEV_RESOLVING_LIST       7
#define HCIC_PARAM_SIZE_BLE_CLEAR_RESOLVING_LIST        0
#define HCIC_PARAM_SIZE_BLE_READ_RESOLVING_LIST_SIZE    0
#define HCIC_PARAM_SIZE_BLE_READ_RESOLVABLE_ADDR_PEER   7
#define HCIC_PARAM_SIZE_BLE_READ_RESOLVABLE_ADDR_LOCAL  7
#define HCIC_PARAM_SIZE_BLE_SET_ADDR_RESOLUTION_ENABLE  1
#define HCIC_PARAM_SIZE_BLE_SET_RAND_PRIV_ADDR_TIMOUT   2
#define HCIC_PARAM_SIZE_BLE_SET_PHY                     7

/* ULP HCI command */
HCI_API extern BOOLEAN btsnd_hcic_ble_set_evt_mask(BT_EVENT_MASK event_mask);

HCI_API extern BOOLEAN btsnd_hcic_ble_read_buffer_size(void);

HCI_API extern BOOLEAN btsnd_hcic_ble_read_local_spt_feat(void);

HCI_API extern BOOLEAN btsnd_hcic_ble_set_local_used_feat(UINT8 feat_set[8]);

HCI_API extern BOOLEAN btsnd_hcic_ble_set_random_addr(BD_ADDR random_addr);

HCI_API extern BOOLEAN btsnd_hcic_ble_write_adv_params(UINT16 adv_int_min, UINT16 adv_int_max,
        UINT8 adv_type, UINT8 addr_type_own,
        UINT8 addr_type_dir, BD_ADDR direct_bda,
        UINT8 channel_map, UINT8 adv_filter_policy);

HCI_API extern BOOLEAN btsnd_hcic_ble_read_adv_chnl_tx_power(void);

HCI_API extern BOOLEAN btsnd_hcic_ble_set_adv_data(UINT8 data_len, UINT8 *p_data);

HCI_API extern BOOLEAN btsnd_hcic_ble_set_scan_rsp_data(UINT8 data_len, UINT8 *p_scan_rsp);

HCI_API extern BOOLEAN btsnd_hcic_ble_set_adv_enable(UINT8 adv_enable);

HCI_API extern BOOLEAN btsnd_hcic_ble_set_scan_params(UINT8 scan_type,
        UINT16 scan_int, UINT16 scan_win,
        UINT8 addr_type, UINT8 scan_filter_policy);

HCI_API extern BOOLEAN btsnd_hcic_ble_set_scan_enable(UINT8 scan_enable, UINT8 duplicate);

HCI_API extern BOOLEAN btsnd_hcic_ble_create_ll_conn(UINT16 scan_int, UINT16 scan_win,
        UINT8 init_filter_policy, UINT8 addr_type_peer, BD_ADDR bda_peer, UINT8 addr_type_own,
        UINT16 conn_int_min, UINT16 conn_int_max, UINT16 conn_latency, UINT16 conn_timeout,
        UINT16 min_ce_len, UINT16 max_ce_len);

HCI_API extern BOOLEAN btsnd_hcic_ble_create_conn_cancel(void);

HCI_API extern BOOLEAN btsnd_hcic_ble_read_white_list_size(void);

HCI_API extern BOOLEAN btsnd_hcic_ble_clear_white_list(void);

HCI_API extern BOOLEAN btsnd_hcic_ble_add_white_list(UINT8 addr_type, BD_ADDR bda);

HCI_API extern BOOLEAN btsnd_hcic_ble_remove_from_white_list(UINT8 addr_type, BD_ADDR bda);

HCI_API extern BOOLEAN btsnd_hcic_ble_upd_ll_conn_params(UINT16 handle, UINT16 conn_int_min, UINT16 conn_int_max,
        UINT16 conn_latency, UINT16 conn_timeout, UINT16 min_len, UINT16 max_len);

HCI_API extern BOOLEAN btsnd_hcic_ble_set_host_chnl_class(const UINT8 chnl_map[HCI_BLE_CHNL_MAP_SIZE]);

HCI_API extern BOOLEAN btsnd_hcic_ble_read_chnl_map(UINT16 handle);

HCI_API extern BOOLEAN btsnd_hcic_ble_read_remote_feat(UINT16 handle);

HCI_API extern BOOLEAN btsnd_hcic_ble_encrypt(UINT8 *key, UINT8 key_len, UINT8 *plain_text, UINT8 pt_len, void *p_cmd_cplt_cback);

HCI_API extern BOOLEAN btsnd_hcic_ble_rand(void *p_cmd_cplt_cback);

HCI_API extern BOOLEAN btsnd_hcic_ble_start_enc(UINT16 handle,
        UINT8 rand[HCIC_BLE_RAND_DI_SIZE],
        UINT16 ediv, UINT8 ltk[HCIC_BLE_ENCRYT_KEY_SIZE]);

HCI_API extern BOOLEAN btsnd_hcic_ble_ltk_req_reply(UINT16 handle, UINT8 ltk[HCIC_BLE_ENCRYT_KEY_SIZE]);

HCI_API extern BOOLEAN btsnd_hcic_ble_ltk_req_neg_reply(UINT16 handle);

HCI_API extern BOOLEAN btsnd_hcic_ble_read_supported_states(void);

HCI_API extern BOOLEAN btsnd_hcic_ble_write_host_supported(UINT8 le_host_spt, UINT8 simul_le_host_spt);

HCI_API extern BOOLEAN btsnd_hcic_ble_read_host_supported(void);

HCI_API extern BOOLEAN btsnd_hcic_ble_receiver_test(UINT8 rx_channel);

/**
*   Packet Payload
*/
enum
{
    HCI_BLE_TRANS_TEST_PP_PSEUDO_RAND_SEQ_9 = 0x00, /**< Pseudo-Random bit sequence 9 */
    HCI_BLE_TRANS_TEST_PP_ALTERNATE_11110000,       /**< Pattern of alternating bits 11110000 */
    HCI_BLE_TRANS_TEST_PP_ALTERNATE_1010,           /**< Pattern of alternating bits 10101010 */
    HCI_BLE_TRANS_TEST_PP_PSEUDO_RAND_SEQ_15,       /**< Pseudo-Random bit sequence 15 */
    HCI_BLE_TRANS_TEST_PP_ALL_1,                    /**< Pattern of All 1 bits */
    HCI_BLE_TRANS_TEST_PP_ALL_0,                    /**< Pattern of All 0 bits */
    HCI_BLE_TRANS_TEST_PP_ALTERNATE_00001111,       /**< Pattern of alternating bits 00001111 */
    HCI_BLE_TRANS_TEST_PP_ALTERNATE_0101            /**< Pattern of alternating bits 0101 */
};
typedef UINT8 HCI_BLE_TRANS_TEST_PP;

HCI_API extern BOOLEAN btsnd_hcic_ble_transmitter_test(UINT8 rx_channel, UINT8 length_of_test_data, HCI_BLE_TRANS_TEST_PP packet_payload);

HCI_API extern BOOLEAN btsnd_hcic_ble_test_end(void);

#if (defined BLE_LLT_INCLUDED) && (BLE_LLT_INCLUDED == TRUE)

#define HCIC_PARAM_SIZE_BLE_RC_PARAM_REQ_REPLY           14
HCI_API extern BOOLEAN btsnd_hcic_ble_rc_param_req_reply(UINT16 handle,
        UINT16 conn_int_min, UINT16 conn_int_max,
        UINT16 conn_latency, UINT16 conn_timeout,
        UINT16 min_ce_len, UINT16 max_ce_len);

#define HCIC_PARAM_SIZE_BLE_RC_PARAM_REQ_NEG_REPLY       3
HCI_API extern BOOLEAN btsnd_hcic_ble_rc_param_req_neg_reply(UINT16 handle, UINT8 reason);

#endif /* BLE_LLT_INCLUDED */

HCI_API extern BOOLEAN btsnd_hcic_ble_set_data_length(UINT16 conn_handle, UINT16 tx_octets, UINT16 tx_time);

HCI_API extern BOOLEAN btsnd_hcic_ble_read_dflt_data_length(void);

HCI_API extern BOOLEAN btsnd_hcic_ble_write_dflt_data_length(UINT16 tx_octets, UINT16 tx_time);

HCI_API extern BOOLEAN btsnd_hcic_ble_add_device_resolving_list(UINT8 addr_type_peer, BD_ADDR bda_peer,
        UINT8 irk_peer[HCIC_BLE_IRK_SIZE],
        UINT8 irk_local[HCIC_BLE_IRK_SIZE]);

HCI_API extern BOOLEAN btsnd_hcic_ble_rm_device_resolving_list(UINT8 addr_type_peer, BD_ADDR bda_peer);

HCI_API extern BOOLEAN btsnd_hcic_ble_clear_resolving_list(void);

HCI_API extern BOOLEAN btsnd_hcic_ble_read_resolving_list_size(void);

HCI_API extern BOOLEAN btsnd_hcic_ble_read_resolvable_addr_peer(UINT8 addr_type_peer, BD_ADDR bda_peer);

HCI_API extern BOOLEAN btsnd_hcic_ble_read_resolvable_addr_local(UINT8 addr_type_peer, BD_ADDR bda_peer);

HCI_API extern BOOLEAN btsnd_hcic_ble_set_addr_resolution_enable(UINT8 addr_resolution_enable);

HCI_API extern BOOLEAN btsnd_hcic_ble_set_rand_priv_addr_timeout(UINT16 rpa_timout);

HCI_API extern BOOLEAN btsnd_hcic_ble_set_phy(UINT16 handle, UINT8 all_phys, UINT8 tx_phys, UINT8 rx_phys, UINT16 phy_options);
#endif /* BLE_INCLUDED */


/* Connectionless Slave Broadcast */
#if (BTM_CLB_INCLUDED == TRUE)

#define HCIC_PARAM_SIZE_TRUNCATED_PAGE  9
HCI_API extern BOOLEAN btsnd_hcic_truncated_page(BD_ADDR bd_addr,
        UINT8 page_scan_rep_mode, UINT16 clock_offset);

#define HCIC_PARAM_SIZE_TRUNCATED_PAGE_CANCEL 6
HCI_API extern BOOLEAN btsnd_hcic_truncated_page_cancel(BD_ADDR bd_addr);

#define HCIC_PARAM_SIZE_SET_RESERVED_LT_ADDR    1
HCI_API extern BOOLEAN btsnd_hcic_set_reserved_lt_addr(UINT8 lt_addr);

#define HCIC_PARAM_SIZE_DELETE_RESERVED_LT_ADDR    1
HCI_API extern BOOLEAN btsnd_hcic_delete_reserved_lt_addr(UINT8 lt_addr);

#define HCIC_PARAM_SIZE_WRITE_CLB_DATA 3
HCI_API extern BOOLEAN btsnd_hcic_write_clb_data(UINT8 lt_addr, UINT8 fragment,
        UINT8 data_len, UINT8 *p_data);

#define HCIC_PARAM_SIZE_SET_CLB 11
HCI_API extern BOOLEAN btsnd_hcic_set_clb(UINT8 enable, UINT8 lt_addr, UINT8 lpo_allowed,
        UINT16 packet_type, UINT16 int_min, UINT16 int_max, UINT16 timeout);

#define HCIC_PARAM_SIZE_RECEIVE_CLB 34
HCI_API extern BOOLEAN btsnd_hcic_receive_clb(BD_ADDR bd_addr, UINT8 enable, UINT8 lt_addr,
        UINT16 interval, UINT32 clock_offset, UINT32 next_clb_clock,
        UINT16 timeout, UINT8 remote_timing_accuracy, UINT8 skip, UINT16 packet_type,
        UINT8 *p_afhchannelmap);

#define HCIC_PARAM_SIZE_WRITE_SYNC_TRAIN_PARAM 9
HCI_API extern BOOLEAN btsnd_hcic_write_sync_train_param(UINT16 int_min, UINT16 int_max,
        UINT32 timeout, UINT8 service_data);

#define HCIC_PARAM_SIZE_READ_SYNC_TRAIN_PARAM 0
HCI_API extern BOOLEAN btsnd_hcic_read_sync_train_param(void);

#define HCIC_PARAM_SIZE_START_SYNC_TRAIN 0
HCI_API extern BOOLEAN btsnd_hcic_start_sync_train(void);

#define HCI_PARAM_SIZE_RX_SYNC_TRAIN 12
HCI_API extern BOOLEAN btsnd_hcic_receive_sync_train(BD_ADDR bd_addr, UINT16 sync_timeout, UINT16 sync_window,
        UINT16 sync_train_scan_interval);

#endif /* (BTM_CLB_INCLUDED == TRUE) */


#if (BTM_BR_SC_INCLUDED == TRUE)
HCI_API extern BOOLEAN btsnd_hcic_read_secure_conns_support(void);

#define HCIC_PARAM_SIZE_R_SECURE_CONNS  0


HCI_API extern BOOLEAN btsnd_hcic_write_secure_conns_support(UINT8 support);

#define HCIC_PARAM_SIZE_W_SECURE_CONNS  1

#define HCIC_W_SECURE_CONNS_SUPPORT_OFF 0


HCI_API extern BOOLEAN btsnd_hcic_write_secure_conns_test_mode(UINT16 handle, UINT8 dm1_acl_u_mode, UINT8 esco_loopback_mode);

#define HCIC_PARAM_SIZE_W_SECURE_CONNS_TEST             4

#define HCIC_W_SECURE_CONNS_TEST_HANDLE_OFF             0
#define HCIC_W_SECURE_CONNS_TEST_DM1_ACL_U_MODE_OFF     2
#define HCIC_W_SECURE_CONNS_TEST_ESCO_LOOPBACK_MODE_OFF 3
#endif /* BTM_BR_SC_INCLUDED */

HCI_API extern BOOLEAN btsnd_hcic_read_authenticated_payload_tout(UINT16 handle);

HCI_API extern BOOLEAN btsnd_hcic_write_authenticated_payload_tout(UINT16 handle, UINT16 timeout);

#define HCIC_PARAM_SIZE_WRITE_AUTHENT_PAYLOAD_TOUT  4

#define HCI__WRITE_AUTHENT_PAYLOAD_TOUT_HANDLE_OFF  0
#define HCI__WRITE_AUTHENT_PAYLOAD_TOUT_TOUT_OFF    2

#define HCIC_PARAM_SIZE_LINK_QUALITY_STATS             3

#ifdef __cplusplus
}
#endif

#endif
