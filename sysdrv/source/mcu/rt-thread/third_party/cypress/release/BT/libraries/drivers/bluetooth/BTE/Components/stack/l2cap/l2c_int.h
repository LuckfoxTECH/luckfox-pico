/*****************************************************************************
 **
 **  Name:          l2c_int.h
 **
 **  Description:   This file contains L2CAP internal definitions
 **
 **  Copyright (c) 1999-2015, Broadcom Corp., All Rights Reserved.
 **  Broadcom Bluetooth Core. Proprietary and confidential.
 ******************************************************************************/
#ifndef L2C_INT_H
#define L2C_INT_H

#include "l2c_api.h"
#include "l2cdefs.h"
#include "btm_cfg.h"
#include "gki.h"
#if defined(UCD_HID_INCLUDED) && (UCD_HID_INCLUDED == TRUE)
#include "btm_int.h"
#endif
#include "btu_vs_api.h"

#if (BTU_DUAL_STACK_MM_INCLUDED == TRUE) || (BTU_DUAL_STACK_BTC_INCLUDED == TRUE) || (BTU_STACK_LITE_ENABLED == TRUE)
#include "uipc_msg.h"
#endif

#define L2CAP_MIN_MTU   48      /* Minimum acceptable MTU is 48 bytes */

/* Timeouts. Since L2CAP works off a 1-second list, all are in seconds.
 */
#define L2CAP_LINK_ROLE_SWITCH_TOUT  10           /* 10 seconds */
#define L2CAP_LINK_CONNECT_TOUT      60           /* 30 seconds */
#define L2CAP_LINK_CONNECT_TOUT_EXT  120          /* 120 seconds */
#define L2CAP_ECHO_RSP_TOUT          30           /* 30 seconds */
#define L2CAP_LINK_FLOW_CONTROL_TOUT 2            /* 2  seconds */
#define L2CAP_LINK_DISCONNECT_TOUT   30           /* 30 seconds */

#ifndef L2CAP_CHNL_CONNECT_TOUT      /* BSA needs to override for internal project needs */
#define L2CAP_CHNL_CONNECT_TOUT      60           /* 60 seconds */
#endif

#define L2CAP_CHNL_CONNECT_TOUT_EXT  120          /* 120 seconds */
#define L2CAP_CHNL_CFG_TIMEOUT       30           /* 30 seconds */
#define L2CAP_CHNL_DISCONNECT_TOUT   10           /* 10 seconds */
#define L2CAP_DELAY_CHECK_SM4        2            /* 2 seconds */
#define L2CAP_WAIT_INFO_RSP_TOUT     3            /* 3 seconds */
#define L2CAP_WAIT_UNPARK_TOUT       2            /* 2 seconds */
#define L2CAP_LINK_INFO_RESP_TOUT    2            /* 2  seconds */
#define L2CAP_AMP_MOVE_TOUT          60           /* 60 seconds */
#define L2CAP_BLE_LINK_CONNECT_TOUT  30           /* 30 seconds */
#define L2CAP_BLE_CONN_PARAM_UPD_TOUT   30           /* 30 seconds */

/* quick timer uses millisecond unit */
#define L2CAP_DEFAULT_RETRANS_TOUT   2000         /* 2000 milliseconds */
#define L2CAP_DEFAULT_MONITOR_TOUT   12000        /* 12000 milliseconds */
#define L2CAP_FCR_ACK_TOUT           200          /* 200 milliseconds */

/* Define the possible L2CAP channel states. The names of
 ** the states may seem a bit strange, but they are taken from
 ** the Bluetooth specification.
 */
typedef enum
{
    CST_CLOSED, /* Channel is in clodes state           */
    CST_ORIG_W4_SEC_COMP, /* Originator waits security clearence  */
    CST_TERM_W4_SEC_COMP, /* Acceptor waits security clearence    */
    CST_W4_L2CAP_CONNECT_RSP, /* Waiting for peer connect response    */
    CST_W4_L2CA_CONNECT_RSP, /* Waiting for upper layer connect rsp (LE connections only) */
    CST_CONFIG, /* Negotiating configuration            */
    CST_OPEN, /* Data transfer state                  */
    CST_W4_L2CAP_DISCONNECT_RSP, /* Waiting for peer disconnect rsp      */
    CST_W4_L2CA_DISCONNECT_RSP /* Waiting for upper layer disc rsp     */
} tL2C_CHNL_STATE;

/* Define the possible L2CAP link states
*/
typedef enum
{
    LST_DISCONNECTED,
    LST_CONNECT_HOLDING,
    LST_CONNECTING_WAIT_SWITCH,
    LST_CONNECTING,
    LST_CONNECTED,
    LST_DISCONNECTING
#if (defined BTM_TBFC_INCLUDED && BTM_TBFC_INCLUDED == TRUE)
    , LST_SUSPENDED
#endif
} tL2C_LINK_STATE;



/* possible security (authentication/encryption) related actions for L2CAP LE COCs */
typedef enum
{
    L2CAP_BLE_SEC_NONE,             /* 0    no action */
    L2CAP_BLE_SEC_ENCR,             /* 1    encryption on already authenticated link */
    L2CAP_BLE_SEC_ENCR_NO_MITM,     /* 2    encryption after no MITM authentication */
    L2CAP_BLE_SEC_ENCR_MITM,        /* 3    encryption after MITM authentication */
    L2CAP_BLE_SEC_PEND_ENCR         /* 4    wait for current authentication/encryption completion */
} tL2C_BLE_SEC_ACTION;



/* Define input events to the L2CAP link and channel state machines. The names
** of the events may seem a bit strange, but they are taken from
** the Bluetooth specification.
*/
#define L2CEVT_LP_CONNECT_CFM          0          /* Lower layer connect confirm          */
#define L2CEVT_LP_CONNECT_CFM_NEG      1          /* Lower layer connect confirm (failed) */
#define L2CEVT_LP_CONNECT_IND          2          /* Lower layer connect indication       */
#define L2CEVT_LP_DISCONNECT_IND       3          /* Lower layer disconnect indication    */
#define L2CEVT_LP_QOS_CFM              4          /* Lower layer QOS confirmation         */
#define L2CEVT_LP_QOS_CFM_NEG          5          /* Lower layer QOS confirmation (failed)*/
#define L2CEVT_LP_QOS_VIOLATION_IND    6          /* Lower layer QOS violation indication */

#define L2CEVT_SEC_COMP                7          /* Security cleared successfully        */
#define L2CEVT_SEC_COMP_NEG            8          /* Security procedure failed            */

#define L2CEVT_L2CAP_CONNECT_REQ      10          /* Peer connection request              */
#define L2CEVT_L2CAP_CONNECT_RSP      11          /* Peer connection response             */
#define L2CEVT_L2CAP_CONNECT_RSP_PND  12          /* Peer connection response pending     */
#define L2CEVT_L2CAP_CONNECT_RSP_NEG  13          /* Peer connection response (failed)    */
#define L2CEVT_L2CAP_CONFIG_REQ       14          /* Peer configuration request           */
#define L2CEVT_L2CAP_CONFIG_RSP       15          /* Peer configuration response          */
#define L2CEVT_L2CAP_CONFIG_RSP_NEG   16          /* Peer configuration response (failed) */
#define L2CEVT_L2CAP_DISCONNECT_REQ   17          /* Peer disconnect request              */
#define L2CEVT_L2CAP_DISCONNECT_RSP   18          /* Peer disconnect response             */
#define L2CEVT_L2CAP_INFO_RSP         19          /* Peer information response            */
#define L2CEVT_L2CAP_DATA             20          /* Peer data                            */

#define L2CEVT_L2CA_CONNECT_REQ       21          /* Upper layer connect request          */
#define L2CEVT_L2CA_DISCONNECT_REQ    27          /* Upper layer disconnect request       */
#define L2CEVT_L2CA_DISCONNECT_RSP    28          /* Upper layer disconnect response      */
#define L2CEVT_L2CA_DATA_READ         29          /* Upper layer data read                */
#define L2CEVT_L2CA_DATA_WRITE        30          /* Upper layer data write               */
#define L2CEVT_L2CA_FLUSH_REQ         31          /* Upper layer flush                    */

#define L2CEVT_TIMEOUT                32          /* Timeout                              */
#define L2CEVT_SEC_RE_SEND_CMD        33          /* btm_sec has enough info to proceed   */

#define L2CEVT_ACK_TIMEOUT            34          /* RR delay timeout                     */
#define L2CEVT_TX_COMPLETE            35          /* Transmitted packet has been sent out */

#define L2CEVT_BLE_LP_CONNECT_CFM      0          /* Lower layer connect confirm          */
#define L2CEVT_BLE_LP_CONNECT_CFM_NEG  1          /* Lower layer connect confirm (failed) */
#define L2CEVT_BLE_LP_DISCONNECT_IND   3          /* Lower layer disconnect indication    */

#define L2CEVT_BLE_P_CONNECT_REQ       4          /* Peer connection request              */
#define L2CEVT_BLE_P_CONNECT_RSP       5          /* Peer connection response             */
#define L2CEVT_BLE_P_CONNECT_RSP_NEG   6          /* Peer connection response (failed)    */
#define L2CEVT_BLE_P_DISCONNECT_REQ    7          /* Peer disconnect request              */
#define L2CEVT_BLE_P_DISCONNECT_RSP    8          /* Peer disconnect response             */
#define L2CEVT_BLE_P_DATA              9          /* Peer data                            */
#define L2CEVT_BLE_P_CREDITS           10         /* Peer LE Flow Control Credits         */
#define L2CEVT_BLE_P_CMD_REJ           11         /* Peer rejects command                 */

#define L2CEVT_BLE_A_CONNECT_REQ       21         /* Upper layer connect request          */
#define L2CEVT_BLE_A_CONNECT_RSP       22         /* Upper layer connect response         */
#define L2CEVT_BLE_A_CONNECT_RSP_NEG   23         /* Upper layer connect response (failed)*/
#define L2CEVT_BLE_A_DISCONNECT_REQ    24         /* Upper layer disconnect request       */
#define L2CEVT_BLE_A_DISCONNECT_RSP    25         /* Upper layer disconnect response      */
#define L2CEVT_BLE_A_DATA_WRITE        26         /* Upper layer data write               */
#define L2CEVT_BLE_A_CREDITS           27         /* Local Application Flow control credits */

#define L2CEVT_BLE_SEC_COMP            28         /* Security cleared successfully        */
#define L2CEVT_BLE_SEC_COMP_NEG        29         /* Security procedure failed            */

/* Bitmask to skip over Broadcom feature reserved (ID) to avoid sending two
 successive ID values, '0' id only or both */
#define L2CAP_ADJ_BRCM_ID           0x1
#define L2CAP_ADJ_ZERO_ID           0x2
#define L2CAP_ADJ_ID                0x3

/* Return values for l2cu_process_peer_cfg_req() */
#define L2CAP_PEER_CFG_UNACCEPTABLE     0
#define L2CAP_PEER_CFG_OK               1
#define L2CAP_PEER_CFG_DISCONNECT       2

#define L2CAP_BR_EDR_FIXED_CHNL_ARRAY_SIZE  1   /* Even though 8 bytes fixed channels supported
                                                ** bit mask is exchanged with the peer we use
                                                ** only the data in the first byte
                                                */

#if (L2CAP_FCR_INCLUDED == TRUE)
/* eL2CAP option constants */
#define L2CAP_MIN_RETRANS_TOUT          2000    /* Min retransmission timeout if no flush timeout or PBF */
#define L2CAP_MIN_MONITOR_TOUT          12000   /* Min monitor timeout if no flush timeout or PBF */

#define L2CAP_MAX_FCR_CFG_TRIES         2       /* Config attempts before disconnecting */

/* Only compiled in when in test mode. Production devices must not include
 */
#if L2CAP_CORRUPT_ERTM_PKTS == TRUE

/* These are used for conformance and corruption testing only */
typedef struct
{
    BOOLEAN in_use; /* TRUE if test in progress */
    UINT8 type; /* Type of test to run or turns off random test */
    UINT8 freq; /* One-shot or random */
    BOOLEAN is_rx; /* TRUE if incoming packets */
    UINT16 count; /* How many I-frames to drop in a row; used only with one-shot tests */
} tL2C_FCR_TEST_CFG;

typedef struct
{
    BOOLEAN in_use; /* TRUE if test in progress */
    UINT8 skip_sframe_count; /* Number of S-Frames to skip sending */
} tL2C_FCR_CFM_TEST_CB;

typedef struct
{
    tL2C_FCR_TEST_CFG cfg; /* Current corruption test configuration */
    tL2C_FCR_CFM_TEST_CB cfm; /* Conformance test structure */
} tL2C_FCR_TEST_CB;

#endif /* L2CAP_CORRUPT_ERTM_PKTS == TRUE */

typedef struct
{
    UINT8 next_tx_seq; /* Next sequence number to be Tx'ed         */
    UINT8 last_rx_ack; /* Last sequence number ack'ed by the peer  */
    UINT8 next_seq_expected; /* Next peer sequence number expected       */
    UINT8 last_ack_sent; /* Last peer sequence number ack'ed         */
    UINT8 num_tries; /* Number of retries to send a packet       */
    UINT8 max_held_acks; /* Max acks we can hold before sending      */

    BOOLEAN remote_busy; /* TRUE if peer has flowed us off           */
    BOOLEAN local_busy; /* TRUE if we have flowed off the peer      */

    BOOLEAN rej_sent; /* Reject was sent                          */
    BOOLEAN srej_sent; /* Selective Reject was sent                */
    BOOLEAN wait_ack; /* Transmitter is waiting ack (poll sent)   */
    BOOLEAN rej_after_srej; /* Send a REJ when SREJ clears              */

    BOOLEAN send_f_rsp; /* We need to send an F-bit response        */

    UINT16 rx_sdu_len; /* Length of the SDU being received         */
    BT_HDR *p_rx_sdu; /* Buffer holding the SDU being received    */
    BUFFER_Q waiting_for_ack_q; /* Buffers sent and waiting for peer to ack */
    BUFFER_Q srej_rcv_hold_q; /* Buffers rcvd but held pending SREJ rsp   */
    BUFFER_Q retrans_q; /* Buffers being retransmitted              */

    TIMER_LIST_ENT ack_timer; /* Timer delaying RR                        */
    TIMER_LIST_ENT mon_retrans_timer; /* Timer Monitor or Retransmission          */

#if (L2CAP_ERTM_STATS == TRUE)
    UINT32 connect_tick_count; /* Time channel was established             */
    UINT32 ertm_pkt_counts[2]; /* Packets sent and received                */
    UINT32 ertm_byte_counts[2]; /* Bytes   sent and received                */
    UINT32 s_frames_sent[4]; /* S-frames sent (RR, REJ, RNR, SREJ)       */
    UINT32 s_frames_rcvd[4]; /* S-frames rcvd (RR, REJ, RNR, SREJ)       */
    UINT32 xmit_window_closed; /* # of times the xmit window was closed    */
    UINT32 controller_idle; /* # of times less than 2 packets in controller */
    /* when the xmit window was closed          */
    UINT32 pkts_retransmitted; /* # of packets that were retransmitted     */
    UINT32 retrans_touts; /* # of retransmission timouts              */
    UINT32 xmit_ack_touts; /* # of xmit ack timouts                    */

#define L2CAP_ERTM_STATS_NUM_AVG 10
#define L2CAP_ERTM_STATS_AVG_NUM_SAMPLES 100
    UINT32 ack_delay_avg_count;
    UINT32 ack_delay_avg_index;
    UINT32 throughput_start;
    UINT32 throughput[L2CAP_ERTM_STATS_NUM_AVG];
    UINT32 ack_delay_avg[L2CAP_ERTM_STATS_NUM_AVG];
    UINT32 ack_delay_min[L2CAP_ERTM_STATS_NUM_AVG];
    UINT32 ack_delay_max[L2CAP_ERTM_STATS_NUM_AVG];
    UINT32 ack_q_count_avg[L2CAP_ERTM_STATS_NUM_AVG];
    UINT32 ack_q_count_min[L2CAP_ERTM_STATS_NUM_AVG];
    UINT32 ack_q_count_max[L2CAP_ERTM_STATS_NUM_AVG];
#endif

#if L2CAP_CORRUPT_ERTM_PKTS == TRUE
    tL2C_FCR_TEST_CB test_cb; /* Used for SVT and UPF testing */
#endif

} tL2C_FCRB;

#endif  /* L2CAP_FCR_INCLUDED == TRUE */

/* Define a registration control block. Every application (e.g. RFCOMM, SDP,
 ** etc) that registers with L2CAP is assigned one of these.
 */
#if (L2CAP_UCD_INCLUDED == TRUE)
#define L2C_UCD_RCB_ID              0x00
#define L2C_UCD_STATE_UNUSED        0x00
#define L2C_UCD_STATE_W4_DATA       0x01
#define L2C_UCD_STATE_W4_RECEPTION  0x02
#define L2C_UCD_STATE_W4_MTU        0x04

typedef struct
{
    UINT8 state;
    tL2CAP_UCD_CB_INFO cb_info;
} tL2C_UCD_REG;
#endif

typedef struct
{
    BOOLEAN in_use;
    UINT16 psm;
    UINT16 real_psm; /* This may be a dummy RCB for an o/b connection but */
    /* this is the real PSM that we need to connect to   */
#if (L2CAP_UCD_INCLUDED == TRUE)
    tL2C_UCD_REG ucd;
#endif

    tL2CAP_APPL_INFO api;
    UINT8 reg_opts; /* Registration options (see L2CAP_REG_OPTS_*) */
    void *context; /* Required to store the application context (HIDD APP) */
} tL2C_RCB;

/* Define a channel control block (CCB). There may be many channel control blocks
 ** between the same two Bluetooth devices (i.e. on the same link).
 ** Each CCB has unique local and remote CIDs. All channel control blocks on
 ** the same physical link and are chained together.
 */
typedef struct t_l2c_ccb
{
    BOOLEAN in_use; /* TRUE when in use, FALSE when not */
    tL2C_CHNL_STATE chnl_state; /* Channel state                    */
    struct t_l2c_ccb *p_next_ccb; /* Next CCB in the chain            */
    struct t_l2c_ccb *p_prev_ccb; /* Previous CCB in the chain        */
    struct t_l2c_linkcb *p_lcb; /* Link this CCB is assigned to     */

    UINT16 local_cid; /* Local CID                        */
    UINT16 remote_cid; /* Remote CID                       */

    TIMER_LIST_ENT timer_entry; /* CCB Timer List Entry             */

    tL2C_RCB *p_rcb; /* Registration CB for this Channel */

#define IB_CFG_DONE     0x01
#define OB_CFG_DONE     0x02
#define RECONFIG_FLAG   0x04                    /* True after initial configuration */
#define CFG_DONE_MASK   (IB_CFG_DONE | OB_CFG_DONE)

    UINT8 config_done; /* Configuration flag word         */
    UINT8 local_id; /* Transaction ID for local trans  */
    UINT8 remote_id; /* Transaction ID for local  */

#define CCB_FLAG_NO_RETRY       0x01            /* no more retry */
#define CCB_FLAG_SENT_PENDING   0x02            /* already sent pending response */
    UINT8 flags;

    tL2CAP_CFG_INFO our_cfg; /* Our saved configuration options    */
    tL2CAP_CH_CFG_BITS peer_cfg_bits; /* Store what peer wants to configure */
    tL2CAP_CFG_INFO peer_cfg; /* Peer's saved configuration options */

#if (BTU_STACK_LITE_ENABLED == TRUE)
    UINT16 out_mtu; /* Max MTU we will send (light stack variable) */
#endif /* (BTU_STACK_LITE_ENABLED == FALSE) */

    BUFFER_Q xmit_hold_q; /* Transmit data hold queue         */

    BOOLEAN cong_sent; /* Set when congested status sent   */

#if (L2CAP_FCR_INCLUDED == TRUE)
    /* Fields used for eL2CAP */
    tL2CAP_ERTM_INFO ertm_info;
    tL2C_FCRB fcrb;
    UINT16 tx_mps; /* TX MPS adjusted based on current controller */
    UINT16 max_rx_mtu;
    UINT8 fcr_cfg_tries; /* Max number of negotiation attempts */
    BOOLEAN peer_cfg_already_rejected; /* If mode rejected once, set to TRUE */
    BOOLEAN out_cfg_fcr_present; /* TRUE if cfg response shoulkd include fcr options */

#define L2CAP_CFG_FCS_OUR   0x01                /* Our desired config FCS option */
#define L2CAP_CFG_FCS_PEER  0x02                /* Peer's desired config FCS option */
#define L2CAP_BYPASS_FCS    (L2CAP_CFG_FCS_OUR | L2CAP_CFG_FCS_PEER)
    UINT8 bypass_fcs;
#endif

#if (L2CAP_NON_FLUSHABLE_PB_INCLUDED == TRUE)
    BOOLEAN is_flushable; /* TRUE if channel is flushable     */
#endif

#if (BTU_DUAL_STACK_MM_INCLUDED == TRUE) || (BTU_DUAL_STACK_BTC_INCLUDED == TRUE)
    BOOLEAN switched; /* TRUE if MM is active */
    BOOLEAN sw_cong_sent; /* TRUE if congested by switching */
#endif

#if (BTU_MULTI_AV_INCLUDED == TRUE)
    BOOLEAN is_multi_av; /* TRUE if Multi-AV */
#endif

#if (defined(L2CAP_FIXED_CHANNELS_SUPPORTED) && ( L2CAP_FIXED_CHANNELS_SUPPORTED == TRUE )) || (L2CAP_UCD_INCLUDED == TRUE)
    UINT16 fixed_chnl_idle_tout; /* Idle timeout to use for the fixed channel       */
#endif

#if (BLE_INCLUDED == TRUE) && (BLE_DATA_LEN_EXT_INCLUDED == TRUE)
    UINT16 tx_data_len;
#endif

} tL2C_CCB;

/***********************************************************************
 ** Define a queue of linked CCBs.
 */
typedef struct
{
    tL2C_CCB *p_first_ccb; /* The first channel in this queue */
    tL2C_CCB *p_last_ccb; /* The last  channel in this queue */
} tL2C_CCB_Q;

/* Define a BLE channel control block (LECB). There may be many BLE channels
 ** between the same two Bluetooth LE devices (i.e. on the same link).
 ** Each LECB has unique local. All LE channel control blocks on
 ** the same physical link and are chained together.
 */
#if (L2CAP_LE_COC_INCLUDED == TRUE)

typedef struct
{
    BOOLEAN in_use;
    UINT16 psm;
    UINT16 real_psm; /* This may be a dummy PSM for an o/b connection but */
    /* this is the real PSM that we need to connect to   */
    tL2CAP_LE_APPL_INFO api;
    UINT8 reg_opts; /* Registration options (see L2CAP_REG_OPTS_*) */
    void *context; /* Required to store the application context */
} tL2C_LE_RCB;

typedef enum
{
    L2CAP_BLE_NOT_INVOLVED, /* 0    connection is not involved with encryption/pairing */
    L2CAP_BLE_WAIT_OWN_COMPLETION, /* 1    connection awaiting completion of its own encryption/pairing */
    L2CAP_BLE_WAIT_OTHER_COMPLETION /* 2    connection awaiting completion of other connection encryption/pairing to start its own */
} tL2C_BLE_ENC_WAIT;

/* Define the structure uses to exchange channel data in LE Credit Based Flow Control mode (lecbm)
 */
typedef struct t_l2c_lecb
{
    BOOLEAN in_use; /* TRUE when in use, FALSE when not */
    tL2C_CHNL_STATE chnl_state; /* Channel state                    */

    struct t_l2c_lecb *p_next_lecb; /* Next LECB in the chain            */
    struct t_l2c_lecb *p_prev_lecb; /* Previous LECB in the chain        */
    struct t_l2c_linkcb *p_lcb; /* Link this LECB is assigned to     */

    UINT16 local_cid; /* Local CID                        */
    UINT16 remote_cid; /* Remote CID                       */

    TIMER_LIST_ENT timer_entry; /* CCB Timer List Entry             */

    tL2C_LE_RCB *p_le_rcb; /* Registration CB for this Channel */

    UINT8 local_id; /* Transaction ID for local trans  */
    UINT8 remote_id; /* Transaction ID for local  */

    UINT16 rx_mps; /* MPS to accept from the peer */
    UINT16 rx_mtu; /* MTU to accept from peer */

    UINT16 max_rx_credits; /* Max Rx credits we will give */
    UINT16 rx_credits_left; /* Number or Rx credits left */

    BOOLEAN is_rx_congested; /* Set TRUE if user rx is congested */
    UINT8 rx_sdu_pool_id; /* Pool used to assemble SDU to pass up */
    UINT16 rx_sdu_len; /* SDU length remaining to assemble */
    BT_HDR *p_rx_sdu; /* Buffer holding SDU currently being received */

    BUFFER_Q xmit_hold_q; /* Transmit data hold queue         */

    UINT16 tx_mtu; /* MTU peer is ready to accept */
    UINT16 tx_mps; /* MPS peer is ready to accept */
    UINT16 tx_credits; /* Number of LE-frames L2CAP can send to peer */

    BOOLEAN tx_cong_sent; /* Set when tx congested status sent to app      */
    UINT16 tx_cong_start_thresh; /* Buffer quota before sending congestion start  */
    UINT16 tx_cong_stop_thresh; /* Buffer quota before sending congestion stop   */

    UINT8 req_security; /* bitmask of required LE security settings used with */
    /* L2CAP LE COC */
    tL2C_BLE_ENC_WAIT wait_encr_compl;

} tL2C_LECB;

/***********************************************************************
 ** Define a queue of linked LECBs.
 */
typedef struct
{
    tL2C_LECB *p_first_lecb; /* The first channel in this queue */
    tL2C_LECB *p_last_lecb; /* The last  channel in this queue */
} tL2C_LECB_Q;

#endif

/* Define a link control block. There is one link control block between
 ** this device and any other device (i.e. BD ADDR).
 */
typedef struct t_l2c_linkcb
{
    BOOLEAN in_use; /* TRUE when in use, FALSE when not */
    tL2C_LINK_STATE link_state;

    TIMER_LIST_ENT timer_entry; /* Timer list entry for timeout evt */
    UINT16 handle; /* The handle used with LM          */

    tL2C_CCB_Q ccb_queue; /* Queue of CCBs on this LCB        */

#if (BTU_STACK_LITE_ENABLED == FALSE)
    tL2C_CCB *p_pending_ccb; /* ccb of waiting channel during link disconnect */
    TIMER_LIST_ENT info_timer_entry; /* Timer entry for info resp timeout evt */
    BD_ADDR remote_bd_addr; /* The BD address of the remote     */

    UINT8 link_role; /* Master or slave                  */
    UINT8 id;
    UINT8 cur_echo_id; /* Current id value for echo request */
    UINT16 idle_timeout; /* Idle timeout                     */
    BOOLEAN is_bonding; /* True - link active only for bonding */

    UINT16 link_flush_tout; /* Flush timeout used               */
#endif

    UINT16 sent_not_acked; /* Num packets sent but not acked   */
    BOOLEAN partial_segment_being_sent; /* Set TRUE when a partial segment  */
    UINT16 partial_segment_local_cid; /* local_cid of channel currently sending partial segment */
    /* is being sent.                   */
    BOOLEAN w4_info_rsp; /* TRUE when info request is active */
    UINT8 info_rx_bits; /* set 1 if received info type */
    UINT32 peer_ext_fea; /* Peer's extended features mask    */
    BUFFER_Q link_xmit_data_q; /* Transmit data buffer queue       */

    UINT8 peer_chnl_mask[ L2CAP_BR_EDR_FIXED_CHNL_ARRAY_SIZE ];
#if (L2CAP_UCD_INCLUDED == TRUE)
    UINT16 ucd_mtu; /* peer MTU on UCD */
    BUFFER_Q ucd_out_sec_pending_q; /* Security pending outgoing UCD packet  */
    BUFFER_Q ucd_in_sec_pending_q; /* Security pending incoming UCD packet  */
#endif

#if (L2CAP_HOST_FLOW_CTRL == TRUE)
    UINT16 link_pkts_unacked; /* Packets received but not acked   */
    UINT16 link_ack_thresh; /* Threshold at which to ack pkts   */
#endif

    BT_HDR *p_hcit_rcv_acl; /* Current HCIT ACL buf being rcvd  */
    UINT16 idle_timeout_sv; /* Save current Idle timeout        */
    UINT8 acl_priority; /* L2C_PRIORITY_NORMAL or L2C_PRIORITY_HIGH */
    UINT8 acl_direction; /* L2CAP_DIRECTION_IGNORE,
         L2CAP_DIRECTION_DATA_SOURCE
         or L2CAP_DIRECTION_DATA_SINK */
#ifndef BTEWICED
    tL2CA_NOCP_CB *p_nocp_cb; /* Num Cmpl pkts callback           */
#endif
#if (BTU_DUAL_STACK_MM_INCLUDED == TRUE) || (BTU_DUAL_STACK_BTC_INCLUDED == TRUE)
#define LCB_FULL_STACK_ACTIVE               0
#define LCB_LITE_STACK_ACTIVE               1
#define LCB_FULL_TO_LITE_TRANSPORT_HOLD     2
#define LCB_LITE_TO_FULL_TRANSPORT_HOLD     3
    UINT8 transport_state;
    UINT8 light_xmit_quota;
#endif
#if (BTU_STACK_LITE_ENABLED == TRUE)
    UINT8 full_stack_sent;
    UINT8 full_stack_ack_delay;
#endif

#if ( defined(L2CAP_FIXED_CHANNELS_SUPPORTED) && ( L2CAP_FIXED_CHANNELS_SUPPORTED == TRUE ))
    tL2C_CCB **p_fixed_ccbs;
    UINT16 disc_reason;
#endif

#if (BLE_INCLUDED == TRUE)
    tBT_TRANSPORT transport;
    tBLE_ADDR_TYPE ble_addr_type;
    BOOLEAN w4_param_upd_rsp; /* TRUE when connection parameter update request is active */
#if (BLE_DATA_LEN_EXT_INCLUDED == TRUE)
    UINT16 tx_data_len; /* tx data length used in data length extension */
#endif
#define L2C_BLE_CONN_UPDATE_DISABLE 0x1  /* disable update connection parameters */
#define L2C_BLE_NEW_CONN_PARAM      0x2  /* new connection parameter to be set */
#define L2C_BLE_UPDATE_PENDING      0x4  /* waiting for connection update finished */
#define L2C_BLE_NOT_DEFAULT_PARAM   0x8  /* not using default connection parameters */

    UINT8 conn_update_mask;

    UINT16 min_interval; /* parameters as requested by peripheral */
    UINT16 max_interval;
    UINT16 latency;
    UINT16 timeout;

#if (L2CAP_LE_COC_INCLUDED == TRUE)
    tL2C_LECB_Q lecb_queue; /* Queue of LECBs on this LCB        */
    tL2C_LECB *p_pending_lecb; /* lecb of channel waiting during link disconnect */

    /* LECBs within the same LCB are served in round robin */
    tL2C_LECB *p_rr_serve_lecb; /* current serving lecb */
    UINT8 num_rr_lecbs; /* number of lecbs */
#endif

#endif

} tL2C_LCB;

/* The events reported on tBTM_VS_EVT_HDLR */
enum
{
    L2C_VS_SET_ACL_PRIORITY_EVT = 1
};
typedef UINT16 tL2C_INT_EVT;
typedef BOOLEAN(tL2C_VS_EVT_HDLR)(tL2C_INT_EVT event, void *p);

#if (BTU_DUAL_STACK_MM_INCLUDED == TRUE) || (BTU_DUAL_STACK_BTC_INCLUDED == TRUE)
typedef enum
{
    L2C_SYNC_STATE_COMPLETED,
    L2C_SYNC_STATE_NOTIFIED,
    L2C_SYNC_STATE_SYNC_AUTO_REQUESTING,
    L2C_SYNC_STATE_SYNC_AUTO_REQUESTED,
    L2C_SYNC_STATE_SYNC_REQUESTED,
    L2C_SYNC_STATE_SYNC_RESPONDED
} tL2CA_SYNC_STATE;
#endif

/* Table entry for keeping track of pending NUMBER_OF_COMPLETED_PACKET acknowledgments from controller */
typedef struct
{
    UINT8 local_cid; /* CCB associated with buffer */
    UINT8 lcb_idx; /* LCB associated with buffer (for obtaining HCI handle) */
    UINT8 pending_acl_bufs; /* NUMBER_OF_COMPLETED_PACKET buffers pending */
} tL2C_TX_ACK_PENDING;

#define L2C_TX_ACK_NOT_NEEDED   0xFF            /* Special CID index to indicate tx_completed not needed */

#if (L2CAP_FCR_INCLUDED == TRUE)
/* Dynamically linked ERTM functions */
typedef void (tL2CAP_ERTM_PROC_PDU_FCN)(tL2C_CCB *p_ccb, BT_HDR *p_buf);
typedef void (tL2CAP_ERTM_SEND_S_FRAME_FCN)(tL2C_CCB *p_ccb, UINT16 function_code, UINT16 pf_bit);
typedef BT_HDR *(tL2CAP_ERTM_GET_NEXT_XMIT_SDU_FCN)(tL2C_CCB *p_ccb, UINT16 max_packet_length);

#endif /* L2CAP_FCR_INCLUDED */

/* Define the L2CAP control structure
 */
typedef struct
{
    UINT8 l2cap_trace_level;
    UINT16 controller_xmit_window; /* Total ACL window for all links   */

    UINT16 round_robin_quota; /* Round-robin link quota           */
    UINT16 round_robin_unacked; /* Round-robin unacked              */
    BOOLEAN check_round_robin; /* Do a round robin check           */

    BOOLEAN is_cong_cback_context;

#if (defined(BTU_DYNAMIC_CB_INCLUDED) && (BTU_DYNAMIC_CB_INCLUDED == TRUE))
    tL2C_LCB *lcb_pool; /* Link Control Block pool          */
    tL2C_CCB *ccb_pool; /* Channel Control Block pool       */
    tL2C_CCB **ccb_ptr_pool; /* pool to hold the pointers to Channel Control Block in a lcb.
         size depends on the number of links supported and fixed channels per link */
    tL2C_RCB *rcb_pool; /* Registration info pool           */
#else
    tL2C_LCB lcb_pool[ MAX_L2CAP_LINKS ]; /* Link Control Block pool          */
    tL2C_CCB ccb_pool[ MAX_L2CAP_CHANNELS ]; /* Channel Control Block pool       */
    tL2C_CCB *ccb_ptr_pool[ MAX_L2CAP_LINKS ][ MAX_L2CAP_CHANNELS ] /* pool to hold the pointers to Channel Control Block in a lcb.
         size depends on the number of links supported and fixed channels per link */
    tL2C_RCB rcb_pool[ MAX_L2CAP_CLIENTS ]; /* Registration info pool           */
#endif /* BTU_DYNAMIC_CB_INCLUDED */

#if (L2CAP_LE_COC_INCLUDED == TRUE)
#if (defined(BTU_DYNAMIC_CB_INCLUDED) && (BTU_DYNAMIC_CB_INCLUDED == TRUE))
    tL2C_LECB *lecb_pool; /* Channel Control Block pool       */
    tL2C_LE_RCB *le_rcb_pool; /* Registration info pool           */
#else
    tL2C_LECB lecb_pool[MAX_L2CAP_BLE_CHANNELS]; /* Channel Control Block pool       */
    tL2C_LE_RCB le_rcb_pool[MAX_L2CAP_BLE_CLIENTS]; /* Registration info pool           */
#endif
    tL2C_LECB *p_free_lecb_first; /* Pointer to first free LECB       */
    tL2C_LECB *p_free_lecb_last; /* Pointer to last  free LECB       */

#if (L2CAP_LE_COC_CONFORMANCE_TESTING == TRUE)
    BOOLEAN conformance_reject; /* if TRUE supports reject functionality required by conformance tests */
    BOOLEAN conform_increm_credits; /* if TRUE supports LT functionality required by conformance test TP/LE/CFC/BV-06-C */
    UINT16 psm_to_use_in_test;
    BOOLEAN only_one_chan_at_a_time; /* if TRUE IUT doesn't support multiple simultaneous channels */
    UINT8 num_active_channels;
    /* on LT for the test TP/LE/CID/BV-01-C */
    BOOLEAN same_cid_on_br_edr_and_le; /* if TRUE then LT has to assign to incoming LE connection the same DCID it assigned
         to the last incoming BR/EDR connection) */
    UINT16 cid_to_assign;
    /* on LT for the test TP/LE/CID/BV-02-C */
    BOOLEAN same_cid_outg; /* if TRUE then LT has to assign to outgoing LE connection the same SCID it assigned
         to the last outgoing BR/EDR connection) */
    UINT16 cid_to_assign_outg;

#endif
#endif

    tL2C_CCB *p_free_ccb_first; /* Pointer to first free CCB        */
    tL2C_CCB *p_free_ccb_last; /* Pointer to last  free CCB        */

    UINT8 desire_role; /* desire to be master/slave when accepting a connection */
    BOOLEAN disallow_switch; /* FALSE, to allow switch at create conn */
    UINT16 num_lm_acl_bufs; /* # of ACL buffers on controller   */
    UINT16 idle_timeout; /* Idle timeout                     */

    BUFFER_Q rcv_hold_q; /* Recv pending queue               */
    TIMER_LIST_ENT rcv_hold_tle; /* Timer list entry for rcv hold    */

    tL2C_LCB *p_cur_hcit_lcb; /* Current HCI Transport buffer     */
    UINT16 num_links_active; /* Number of links active           */

#if (L2CAP_NON_FLUSHABLE_PB_INCLUDED == TRUE)
    UINT16 non_flushable_pbf; /* L2CAP_PKT_START_NON_FLUSHABLE if controller supports */
    /* Otherwise, L2CAP_PKT_START */
    BOOLEAN is_flush_active; /* TRUE if an HCI_Enhanced_Flush has been sent */
#endif

#if L2CAP_CONFORMANCE_TESTING == TRUE
    UINT32 test_info_resp; /* Conformance testing needs a dynamic response */
#endif

    tL2CAP_FIXED_CHNL_REG *p_fixed_reg; /* Reg info for fixed channels */
    UINT8 *p_fixed_reg_opts; /* Registration options (see L2CAP_REG_OPTS_*) */

    UINT16 num_ble_links_active; /* Number of LE links active           */
    BOOLEAN is_ble_connecting;
    BD_ADDR ble_connecting_bda;
    UINT16 controller_le_xmit_window; /* Total ACL window for all links   */
    UINT16 num_lm_ble_bufs; /* # of ACL buffers on controller   */
//    UINT8                    l2c_ble_fixed_chnls_mask;          /* LE fixed channels mask           */
//    UINT16                  ble_round_robin_quota;              /* Round-robin link quota           */
//    UINT16                  ble_round_robin_unacked;            /* Round-robin unacked              */
//    BOOLEAN                 ble_check_round_robin;              /* Do a round robin check           */

    UINT16 dyn_psm;

    tL2C_CCB *p_hi_pri_ccb;

    /* Following elements are used to send TX_Complete event to the appropriate CCB when number of comleted event is received */
    tL2C_TX_ACK_PENDING *p_tx_ack_pending; /* List of l2cap buffers pending ack (NUMBER_OF_COMPLETED_PACKETS) from controller */
    UINT8 tx_ack_pending_count; /* Number of entries in tx_ack_pending list */

#if (L2CAP_FCR_INCLUDED == TRUE)
    /* Dynamically linked ERTM functions */
    tL2CAP_ERTM_PROC_PDU_FCN *p_fcr_proc_pdu;
    tL2CAP_ERTM_SEND_S_FRAME_FCN *p_fcr_send_S_frame;
    tL2CAP_ERTM_GET_NEXT_XMIT_SDU_FCN *p_fcr_get_next_xmit_sdu_seg;
#endif /* L2CAP_FCR_INCLUDED */

} tL2C_CB;

/* Define a structure that contains the information about a connection.
 ** This structure is used to pass between functions, and not all the
 ** fields will always be filled in.
 */
typedef struct
{
    BD_ADDR bd_addr; /* Remote BD address        */
    tBTM_STATUS status; /* Connection status        */
    UINT16 psm; /* PSM of the connection    */
    UINT16 l2cap_result; /* L2CAP result             */
    UINT16 l2cap_status; /* L2CAP status             */
    UINT16 remote_cid; /* Remote CID               */
} tL2C_CONN_INFO;

/* Non GKI-aware data callbacks */
typedef void (tL2CA_DATA_IND_NO_BT_HDR_CBACK)(void *context, UINT16 cid, UINT8 *p_data, UINT16 len);
typedef void (tL2CA_FIXED_CHNL_DATA_IND_NO_BT_HDR_CBACK)(BD_ADDR bd_addr, UINT8 *p_data, UINT16 len);

#if (L2CAP_LE_COC_INCLUDED == TRUE)
/* Define a structure that contains the information about a BLE
 ** dynamic connection. This structure is used to pass between functions,
 ** and not all the fields will always be filled in.
 */
typedef struct
{
    UINT16 le_psm;
    UINT16 cid;
    UINT16 mtu;
    UINT16 mps;
    UINT16 credits;
    UINT16 l2cap_result;
} tL2C_BLE_CONN_INFO;
#endif

#if (L2CAP_FCR_INCLUDED == TRUE)
typedef void (tL2C_FCR_MGMT_EVT_HDLR)(UINT8, tL2C_CCB *);
#endif

/* The offset in a buffer that L2CAP will use when building commands.
 */
#define L2CAP_SEND_CMD_OFFSET       0

/* Number of ACL buffers to use for high priority channel
 */
#if (!defined(L2CAP_HIGH_PRI_CHAN_QUOTA_IS_CONFIGURABLE) || (L2CAP_HIGH_PRI_CHAN_QUOTA_IS_CONFIGURABLE == FALSE))
#define L2CAP_HIGH_PRI_MIN_XMIT_QUOTA_A     (L2CAP_HIGH_PRI_MIN_XMIT_QUOTA)
#else
#define L2CAP_HIGH_PRI_MIN_XMIT_QUOTA_A     (l2cb.high_pri_min_xmit_quota)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/* L2CAP global data
 ************************************
 */
#if (!defined L2C_DYNAMIC_MEMORY) || (L2C_DYNAMIC_MEMORY == FALSE)
L2C_API extern tL2C_CB l2cb;
#else
L2C_API extern tL2C_CB *l2c_cb_ptr;
#define l2cb (*l2c_cb_ptr)
#endif

/* Functions provided by l2c_main.c
 ************************************
 */
extern void l2c_init(void);
extern void l2c_process_timeout(TIMER_LIST_ENT *p_tle);
extern UINT8 l2c_data_write(UINT16 cid, BT_HDR *p_data, UINT16 flag);
#if (BTU_STACK_LITE_ENABLED == FALSE)
extern void l2c_rcv_acl_data(BT_HDR *p_msg);
extern void l2c_process_held_packets(BOOLEAN timed_out);
#endif /* (BTU_STACK_LITE_ENABLED == FALSE) */

/* Functions provided by l2c_sync.c
 ************************************
 */
#if (BTU_DUAL_STACK_MM_INCLUDED == TRUE) || (BTU_DUAL_STACK_BTC_INCLUDED == TRUE)
extern void l2c_send_congestion_to_all_ccb(BOOLEAN congestion);
extern BOOLEAN l2c_sync_notify(UINT8 event);
extern void l2c_sync_request(tBTM_SYNC_INFO *p_btm_sync_info, tBTM_SYNC_CPLT_CBACK p_sync_cplt_cback);
extern void l2c_sync_reset(void);
extern void l2c_send_sync_request(tL2C_CCB *p_ccb[], UINT8 op_code);
extern void l2c_rcv_sync_reply(tL2C_SYNC_MSG *p_sync_msg);
extern void l2c_sync_after_link_adjust_allocation(void);
extern BOOLEAN l2c_sync_check_transport_active(void);
extern BOOLEAN l2c_sync_get_num_links_and_quota(UINT16 *p_num_lowpri_links,
        UINT16 *p_num_hipri_links,
        UINT16 *p_high_pri_link_quota);
extern void l2c_sync_send_acl_data(BT_HDR *p_buf);
#endif /* (BTU_DUAL_STACK_MM_INCLUDED == TRUE) || (BTU_DUAL_STACK_BTC_INCLUDED == TRUE) */

#if (BTU_STACK_LITE_ENABLED == TRUE)
extern void l2c_rcv_sync_request(tL2C_SYNC_MSG *p_sync_msg);
extern void l2c_send_sync_reply(UINT8 op_code, tL2C_LCB *p_lcb[], tL2C_CCB *p_ccb[], UINT8 status);
extern void l2c_rcv_acl_data(BT_HDR *p_msg);
extern void l2c_sync_request_flush(UINT16 cid);
extern void l2c_sync_send_acl_data_to_lower(BT_HDR *p_msg);
extern void l2c_sync_check_full_stack_acked(UINT16 handle, tL2C_LCB *p_lcb,
        UINT16 *p_num_sent, BT_HDR **p_event_msg);
extern void l2c_sync_forward_num_completed_pkts(BT_HDR *p_msg);
#endif

#if (BTU_DUAL_STACK_MM_INCLUDED == TRUE) || (BTU_DUAL_STACK_BTC_INCLUDED == TRUE) || (BTU_STACK_LITE_ENABLED == TRUE)
extern void l2c_sync_process_xmit_window(UINT16 num_sent);
extern void l2c_sync_send_sync_info(tL2C_SYNC_MSG *p_sync_msg);
extern void l2c_sync_rcv_sync_info(BT_HDR *p_buf);
extern void l2c_sync_update_xmit_window(UINT16 old_quota, UINT16 new_quota);
#endif

/* this function is used for dual stack and exported for UIPC */
L2C_API extern BOOLEAN l2c_sync_check_allocating_acl_pool(UINT8 *p_msg);

/* Functions provided by l2c_utils.c
 ************************************
 */
extern tL2C_LCB *l2cu_allocate_lcb(BD_ADDR p_bd_addr, BOOLEAN is_bonding, tBT_TRANSPORT transport);
extern BOOLEAN l2cu_start_post_bond_timer(UINT16 handle);
extern void l2cu_release_lcb(tL2C_LCB *p_lcb, UINT8 reason);
extern tL2C_LCB *l2cu_find_lcb_by_bd_addr(BD_ADDR p_bd_addr, tBT_TRANSPORT transport);
extern tL2C_LCB *l2cu_find_lcb_by_handle(UINT16 handle);
extern void l2cu_update_lcb_4_bonding(BD_ADDR p_bd_addr, BOOLEAN is_bonding);

extern UINT8 l2cu_get_conn_role(tL2C_LCB *p_this_lcb);

extern void l2cu_enqueue_ccb(tL2C_CCB *p_ccb);
extern void l2cu_dequeue_ccb(tL2C_CCB *p_ccb);

extern tL2C_CCB *l2cu_allocate_ccb(tL2C_LCB *p_lcb, UINT16 cid);
extern void l2cu_release_ccb(tL2C_CCB *p_ccb);
extern tL2C_CCB *l2cu_find_ccb_by_cid(tL2C_LCB *p_lcb, UINT16 local_cid);
extern tL2C_CCB *l2cu_find_ccb_by_remote_cid(tL2C_LCB *p_lcb, UINT16 remote_cid);
extern void l2cu_adj_id(tL2C_LCB *p_lcb, UINT8 adj_mask);
extern BOOLEAN l2c_is_cmd_rejected(UINT8 cmd_code, UINT8 id, tL2C_LCB *p_lcb);

extern void     l2cu_send_peer_cmd_reject(tL2C_LCB *p_lcb, UINT16 reason,
        UINT8 rem_id, UINT16 p1, UINT16 p2);
extern void     l2cu_send_peer_connect_req(tL2C_CCB *p_ccb);
extern void     l2cu_send_peer_connect_rsp(tL2C_CCB *p_ccb, UINT16 result, UINT16 status);
extern void     l2cu_send_peer_config_req(tL2C_CCB *p_ccb, tL2CAP_CFG_INFO *p_cfg);
extern void     l2cu_send_peer_config_rsp(tL2C_CCB *p_ccb, tL2CAP_CFG_INFO *p_cfg);
extern void     l2cu_send_peer_config_rej(tL2C_CCB *p_ccb, UINT8 *p_data, UINT16 data_len, UINT16 rej_len);
extern void     l2cu_send_peer_disc_req(tL2C_CCB *p_ccb);
extern void     l2cu_send_peer_disc_rsp(tL2C_LCB *p_lcb, UINT8 remote_id, UINT16 local_cid, UINT16 remote_cid);
extern void     l2cu_send_peer_echo_req(tL2C_LCB *p_lcb, UINT8 *p_data, UINT16 data_len);
extern void     l2cu_send_peer_echo_rsp(tL2C_LCB *p_lcb, UINT8 id, UINT8 *p_data, UINT16 data_len);
extern void     l2cu_send_peer_info_rsp(tL2C_LCB *p_lcb, UINT8 id, UINT16 info_type);
extern void     l2cu_reject_connection(tL2C_LCB *p_lcb, UINT16 remote_cid, UINT8 rem_id, UINT16 result);
extern void     l2cu_send_peer_info_req(tL2C_LCB *p_lcb, UINT16 info_type);
extern void     l2cu_set_acl_hci_header(BT_HDR *p_buf, tL2C_CCB *p_ccb);
extern void     l2cu_check_channel_congestion(tL2C_CCB *p_ccb);
extern void     l2cu_disconnect_chnl(tL2C_CCB *p_ccb);
extern BT_HDR *l2cu_build_header(tL2C_LCB *p_lcb, UINT16 len, UINT8 cmd, UINT8 id);

extern void l2cu_process_peer_data(tL2C_CCB *p_ccb, BT_HDR *p_data);
extern void l2cu_process_fixed_channel_data(UINT16 fixed_cid, BD_ADDR bd_addr, BT_HDR *p_data);
extern void l2cu_send_tx_complete(tL2C_CCB *p_ccb);

#if (L2CAP_NON_FLUSHABLE_PB_INCLUDED == TRUE)
extern void l2cu_set_non_flushable_pbf(BOOLEAN);
#endif

#if (BLE_INCLUDED == TRUE)
extern void l2cu_ble_send_peer_par_req(tL2C_LCB *p_lcb, UINT16 min_int, UINT16 max_int, UINT16 latency, UINT16 timeout);
extern void l2cu_ble_send_peer_par_rsp(tL2C_LCB *p_lcb, UINT16 reason, UINT8 rem_id);
#endif

extern BOOLEAN l2cu_initialize_fixed_ccb(tL2C_LCB *p_lcb, UINT16 fixed_cid);
extern void l2cu_no_dynamic_ccbs(tL2C_LCB *p_lcb);
extern void l2cu_process_fixed_chnl_resp(tL2C_LCB *p_lcb);

/* Functions provided by l2c_ucd.c
 ************************************
 */
#if (L2CAP_UCD_INCLUDED == TRUE)
void l2c_ucd_delete_sec_pending_q(tL2C_LCB *p_lcb);
void l2c_ucd_enqueue_pending_out_sec_q(tL2C_CCB *p_ccb, void *p_data);
BOOLEAN l2c_ucd_check_pending_info_req(tL2C_CCB *p_ccb);
BOOLEAN l2c_ucd_check_pending_out_sec_q(tL2C_CCB *p_ccb);
void l2c_ucd_send_pending_out_sec_q(tL2C_CCB *p_ccb);
void l2c_ucd_discard_pending_out_sec_q(tL2C_CCB *p_ccb);
BOOLEAN l2c_ucd_check_pending_in_sec_q(tL2C_CCB *p_ccb);
void l2c_ucd_send_pending_in_sec_q(tL2C_CCB *p_ccb);
void l2c_ucd_discard_pending_in_sec_q(tL2C_CCB *p_ccb);
BOOLEAN l2c_ucd_check_rx_pkts(tL2C_LCB *p_lcb, BT_HDR *p_msg);
BOOLEAN l2c_ucd_process_event(tL2C_CCB *p_ccb, UINT16 event, void *p_data);
#endif

#if (BLE_INCLUDED == TRUE)
extern void l2cu_ble_send_peer_par_req(tL2C_LCB *p_lcb, UINT16 min_int, UINT16 max_int, UINT16 latency, UINT16 timeout);
extern void l2cu_ble_send_peer_par_rsp(tL2C_LCB *p_lcb, UINT16 reason, UINT8 rem_id);
#endif

extern void l2cu_no_dynamic_ccbs(tL2C_LCB *p_lcb);
extern void l2cu_process_fixed_chnl_resp(tL2C_LCB *p_lcb);

/* Functions provided for Broadcom Aware
 ****************************************
 */
extern BOOLEAN l2cu_check_feature_req(tL2C_LCB *p_lcb, UINT8 id, UINT8 *p_data, UINT16 data_len);
extern void l2cu_check_feature_rsp(tL2C_LCB *p_lcb, UINT8 id, UINT8 *p_data, UINT16 data_len);
extern void l2cu_send_feature_req(tL2C_CCB *p_ccb);

extern tL2C_RCB *l2cu_allocate_rcb(UINT16 psm);
extern tL2C_RCB *l2cu_find_rcb_by_psm(UINT16 psm);
extern void l2cu_release_rcb(tL2C_RCB *p_rcb);

#if (L2CAP_LE_COC_INCLUDED == TRUE)
extern tL2C_LE_RCB *l2cu_ble_allocate_rcb(UINT16 le_psm);
extern tL2C_LE_RCB *l2cu_ble_find_rcb_by_psm(UINT16 le_psm);
extern void l2cu_ble_release_rcb(tL2C_LE_RCB *p_le_rcb);
#endif /* (L2CAP_LE_COC_INCLUDED == TRUE) */

extern UINT8 l2cu_process_peer_cfg_req(tL2C_CCB *p_ccb, tL2CAP_CFG_INFO *p_cfg);
extern void l2cu_process_peer_cfg_rsp(tL2C_CCB *p_ccb, tL2CAP_CFG_INFO *p_cfg);
extern void l2cu_process_our_cfg_req(tL2C_CCB *p_ccb, tL2CAP_CFG_INFO *p_cfg);
extern void l2cu_process_our_cfg_rsp(tL2C_CCB *p_ccb, tL2CAP_CFG_INFO *p_cfg);

extern void l2cu_device_reset(void);
extern tL2C_LCB *l2cu_find_lcb_by_state(tL2C_LINK_STATE state);
extern BOOLEAN l2cu_lcb_disconnecting(void);

extern BOOLEAN l2cu_create_conn(tL2C_LCB *p_lcb);
extern BOOLEAN l2cu_create_conn_after_switch(tL2C_LCB *p_lcb);
extern BT_HDR *l2cu_get_next_buffer_to_send(tL2C_LCB *p_lcb);
extern void l2cu_resubmit_pending_sec_req(BD_ADDR p_bda);
extern void l2cu_initialize_amp_ccb(tL2C_LCB *p_lcb);
extern void l2cu_adjust_out_mps(tL2C_CCB *p_ccb);

/* Functions provided by l2c_link.c
 ************************************
 */
extern BOOLEAN l2c_link_hci_conn_req(BD_ADDR bd_addr);
extern BOOLEAN l2c_link_hci_conn_comp(UINT8 status, UINT16 handle, BD_ADDR p_bda);
extern BOOLEAN l2c_link_hci_disc_comp(UINT16 handle, UINT8 reason);
extern BOOLEAN l2c_link_hci_qos_violation(UINT16 handle);
extern void l2c_link_timeout(tL2C_LCB *p_lcb);
extern void l2c_info_timeout(tL2C_LCB *p_lcb);
extern void l2c_link_check_send_pkts(tL2C_LCB *p_lcb, tL2C_CCB *p_ccb, BT_HDR *p_buf);
extern void l2c_link_process_num_completed_pkts(UINT8 *p);
extern void l2c_link_process_num_completed_blocks(UINT8 controller_id, UINT8 *p, UINT16 evt_len);
extern void l2c_link_processs_num_bufs(UINT16 num_lm_acl_bufs);
extern UINT8 l2c_link_pkts_rcvd(UINT16 *num_pkts, UINT16 *handles);
extern void l2c_link_role_changed(BD_ADDR bd_addr, UINT8 new_role, UINT8 hci_status);
extern tBTM_STATUS l2c_link_sec_comp(tBTM_EVENT btm_event, tBTM_EVENT_DATA *p_event_data);
extern void l2c_link_segments_xmitted(BT_HDR *p_msg);
extern void l2c_link_allocate_tx_complete_pending_table(void);
extern void l2cap_link_flush_tx_complete_pending_table(tL2C_LCB *p_lcb);
extern void l2c_link_free_tx_complete_pending_table(void);
extern void l2c_pin_code_request(BD_ADDR bd_addr);

#if (BLE_INCLUDED == TRUE)
extern void l2c_link_processs_ble_num_bufs(UINT16 num_lm_acl_bufs);
#endif

#if ((BTM_PWR_MGR_INCLUDED == TRUE) && L2CAP_WAKE_PARKED_LINK == TRUE)
extern BOOLEAN l2c_link_check_power_mode(tL2C_LCB *p_lcb);
#define L2C_LINK_CHECK_POWER_MODE(x) l2c_link_check_power_mode ((x))
#else
#define L2C_LINK_CHECK_POWER_MODE(x) (FALSE)
#endif

#if L2CAP_CONFORMANCE_TESTING == TRUE
/* Used only for conformance testing */
L2C_API extern void l2cu_set_info_rsp_mask(UINT32 mask);
#endif

#if defined(UCD_HID_INCLUDED) && (UCD_HID_INCLUDED == TRUE) && \
    defined(BRR_HID_INCLUDED) && (BRR_HID_INCLUDED == TRUE)
extern void l2c_deferred_ucd_timeout(tACL_CONN *p);
#endif

/* Functions provided by l2c_csm.c
 ************************************
 */
extern void l2c_csm_execute(tL2C_CCB *p_ccb, UINT16 event, void *p_data);

#ifdef MPAF_CUSTOM_STACK
L2C_API extern BT_HDR *l2cap_link_chk_pkt_start(UINT8 *);  /* Called at start of rcv to check L2CAP segmentation */
#else
L2C_API extern BT_HDR *l2cap_link_chk_pkt_start(BT_HDR *);   /* Called at start of rcv to check L2CAP segmentation */
#endif
L2C_API extern BOOLEAN l2cap_link_chk_pkt_end(void);   /* Called at end   of rcv to check L2CAP segmentation */

L2C_API extern void l2c_enqueue_peer_data(tL2C_CCB *p_ccb, BT_HDR *p_buf);

/* Functions provided by l2c_fcr.c
 ************************************
 */
#if (L2CAP_FCR_INCLUDED == TRUE)
extern void l2c_fcr_cleanup(tL2C_CCB *p_ccb);
extern void l2c_fcr_proc_pdu(tL2C_CCB *p_ccb, BT_HDR *p_buf);
extern void l2c_fcr_proc_tout(tL2C_CCB *p_ccb);
extern void l2c_fcr_proc_ack_tout(tL2C_CCB *p_ccb);
extern void l2c_fcr_send_S_frame(tL2C_CCB *p_ccb, UINT16 function_code, UINT16 pf_bit);
extern BT_HDR *l2c_fcr_clone_buf(BT_HDR *p_buf, UINT16 new_offset, UINT16 no_of_bytes, UINT8 pool);
extern BOOLEAN l2c_fcr_is_flow_controlled(tL2C_CCB *p_ccb);
extern BT_HDR *l2c_fcr_get_next_xmit_sdu_seg(tL2C_CCB *p_ccb, UINT16 max_packet_length);
extern void l2c_fcr_start_timer(tL2C_CCB *p_ccb);

/* Configuration negotiation */
extern UINT8 l2c_fcr_chk_chan_modes(tL2C_CCB *p_ccb);
extern BOOLEAN l2c_fcr_adj_our_req_options(tL2C_CCB *p_ccb, tL2CAP_CFG_INFO *p_cfg);
extern void l2c_fcr_adj_our_rsp_options(tL2C_CCB *p_ccb, tL2CAP_CFG_INFO *p_peer_cfg);
extern UINT8 l2c_fcr_process_peer_cfg_req(tL2C_CCB *p_ccb, tL2CAP_CFG_INFO *p_cfg);
extern void l2c_fcr_adj_monitor_retran_timeout(tL2C_CCB *p_ccb);
extern void l2c_fcr_stop_timer(tL2C_CCB *p_ccb);
extern BOOLEAN l2c_fcr_renegotiate_chan(tL2C_CCB *p_ccb, tL2CAP_CFG_INFO *p_cfg);

/* ERTM functions pointers from l2c_fcr_patch.c */
extern tL2CAP_ERTM_PROC_PDU_FCN *p_l2c_fcr_proc_pdu;
extern tL2CAP_ERTM_SEND_S_FRAME_FCN *p_l2c_fcr_send_S_frame;
extern tL2CAP_ERTM_GET_NEXT_XMIT_SDU_FCN *p_l2c_fcr_get_next_xmit_sdu_seg;

#endif /* L2CAP_FCR_INCLUDED */

/* Functions provided by l2c_mav.c
 ************************************
 */
#if (BTU_MULTI_AV_INCLUDED == TRUE)
extern void l2c_mav_process_multi_av(void);
extern void l2c_mav_check_send_multi_av(void);
extern void l2c_mav_set_multi_av_cong(UINT16 high_pri_link_quota);
extern UINT16 l2c_mav_flush_mav_data_q(UINT16 num_to_flush);
extern void l2c_mav_flush_channel(UINT16 lcid, UINT16 num_to_flush);
#endif

/* Functions provided by l2c_ble.c
 ************************************
 */
#if (BLE_INCLUDED == TRUE)
extern BOOLEAN l2cble_create_conn(tL2C_LCB *p_lcb);
extern void l2cble_process_sig_cmd(tL2C_LCB *p_lcb, UINT8 *p, UINT16 pkt_len);
extern void l2cble_conn_comp(UINT16 handle, UINT8 role, BD_ADDR bda, tBLE_ADDR_TYPE type,
                             UINT16 conn_interval, UINT16 conn_latency, UINT16 conn_timeout);
extern BOOLEAN l2cble_init_direct_conn(tL2C_LCB *p_lcb);
extern void l2c_ble_link_adjust_allocation(void);
extern void l2cble_notify_le_connection(BD_ADDR bda);
extern void l2cble_process_conn_update_evt(UINT16 handle, UINT8 status);
extern void l2cble_fast_slow_scan_tout(tL2C_LCB *p_lcb);
#if (defined BLE_DATA_LEN_EXT_INCLUDED) && (BLE_DATA_LEN_EXT_INCLUDED == TRUE)
extern void l2cble_update_data_pdu_length(tL2C_LCB *p_lcb);
extern void l2cble_set_fix_chnl_tx_data_len(BD_ADDR remote_bda, UINT16 fix_cid, UINT16 tx_mtu);
extern void l2cble_process_data_length_change_evt(UINT16 handle, UINT16 tx_data_len, UINT16 rx_data_len);
#endif

#if (L2CAP_LE_COC_INCLUDED == TRUE)
/* Functions provided by l2c_ble_utils.c
 ****************************************
 */
extern tL2C_LECB *l2cu_ble_allocate_lecb(tL2C_LCB *p_lcb);
extern void l2cu_ble_release_lecb(tL2C_LECB *p_lecb);
extern tL2C_LECB *l2cu_ble_find_lecb_by_local_id(tL2C_LCB *p_lcb, UINT8 local_id);
extern tL2C_LECB *l2cu_ble_find_lecb_by_cid(tL2C_LCB *p_lcb, UINT16 local_cid);
extern tL2C_LECB *l2cu_ble_find_lecb_by_remote_cid(tL2C_LCB *p_lcb, UINT16 remote_cid);
extern void l2cu_ble_enqueue_lecb(tL2C_LECB *p_lecb);
extern void l2cu_ble_dequeue_lecb(tL2C_LECB *p_lecb);

extern BT_HDR *l2cu_ble_get_next_buffer_to_send(tL2C_LCB *p_lcb);

extern void l2cu_ble_send_peer_disc_req(tL2C_LECB *p_lecb);
extern void l2cu_ble_send_peer_conn_req(tL2C_LECB *p_lecb);
extern void l2cu_ble_send_peer_conn_rsp(tL2C_LECB *p_lecb, UINT16 result);
extern void l2cu_ble_send_peer_credits(tL2C_LECB *p_lecb, UINT16 credit);
extern void l2cu_ble_send_peer_reject(tL2C_LCB *p_lcb, UINT16 remote_cid, UINT8 rem_id, UINT16 result);

extern BT_HDR *l2cu_ble_get_next_xmit_sdu_seg(tL2C_LECB *p_lecb);
extern void l2cu_ble_proc_peer_pdu(tL2C_LECB *p_lecb, BT_HDR *p_msg);
extern void l2c_ble_check_channel_congestion(tL2C_LECB *p_lecb);

extern tL2C_BLE_SEC_ACTION l2cu_ble_determ_security_act(tL2C_LECB *p_lecb);

#if (L2CAP_LE_COC_CONFORMANCE_TESTING == TRUE)
L2C_API extern void l2cu_ble_set_conformance_reject(BOOLEAN conformance_reject, UINT8 psm_to_use_in_test);
L2C_API extern void l2cu_ble_set_conf_incr_creds(void);
L2C_API extern void l2cu_ble_clear_conform_settings(void);
L2C_API extern BOOLEAN l2cu_ble_add_credits(UINT16 lcid, UINT16 credits);
L2C_API extern UINT16 l2cu_ble_unknown_cmd(UINT8 cmd, UINT16 le_psm, BD_ADDR bd_addr);
L2C_API extern void l2cu_ble_no_multiple_simult_channels(void);
L2C_API extern void l2cu_ble_same_cid_on_br_edr_and_le(void);
L2C_API extern void l2cu_ble_same_cid_outg(void);

extern tL2C_LECB *l2cu_ble_allocate_lecb_lt_conform(tL2C_LCB *p_lcb, UINT16 cid);
extern void l2cu_ble_send_unknown_cmd(tL2C_LECB *p_lecb, UINT8 cmd);
#endif

/* Functions provided by l2c_ble_conn.c
 ****************************************
 */
extern void l2c_bleconn_proc_peer_pdu(BT_HDR *p_msg, tL2C_LCB *p_lcb, UINT16 l2cap_len, UINT16 rcv_cid);
extern void l2c_bleconn_execute(tL2C_LECB *p_lecb, UINT16 event, void *p_data);
extern UINT16 l2c_bleconn_proc_reject(UINT8 *p_data, tL2C_LCB *p_lcb);
extern void l2c_bleconn_proc_link_down(tL2C_LCB *p_lcb, UINT8 reason);
extern void l2c_notify_smp_cmpl(BD_ADDR bd_addr, tBTM_STATUS result);

#endif

#endif
extern void l2cu_process_fixed_disc_cback(tL2C_LCB *p_lcb);
#if (defined BLE_LLT_INCLUDED) && (BLE_LLT_INCLUDED == TRUE)
extern void l2cble_process_rc_param_request_evt(UINT16 handle, UINT16 int_min, UINT16 int_max,
        UINT16 latency, UINT16 timeout);
#endif

#ifdef __cplusplus
}
#endif

#endif
