/*****************************************************************************
**
**  Name:          l2c_api.h
**
**  Description:   this file contains the L2CAP API definitions
**
**
**
**  Copyright (c) 1999-2015, Broadcom Corp., All Rights Reserved.
**  Broadcom Bluetooth Core. Proprietary and confidential.
******************************************************************************/
#ifndef L2C_API_H
#define L2C_API_H

#include "bt_target.h"
#include "btm_api.h"
#include "l2cdefs.h"
#include "hcidefs.h"

/*****************************************************************************
**  Constants
*****************************************************************************/

/* Define the minimum offset that L2CAP needs in a buffer. This is made up of
** HCI type(1), len(2), handle(2), L2CAP len(2) and CID(2) => 9
*/
#define L2CAP_MIN_OFFSET    13     /* plus control(2), SDU length(2) */

#define L2CAP_BLE_CONN_MIN_OFFSET          9    /* HCI type(1), len(2), handle(2), L2CAP len(2) and CID(2) */
#define L2CAP_DEFAULT_BLE_CB_POOL_ID    0xFF    /* Use the default HCI ACL buffer pool */
#define L2CAP_BLE_COC_SDU_OFFSET           4    /* to provide upper layer some minimal offset possibly required
                                                ** to process incoming packets */
#define L2CAP_BLE_TX_CONG_START_THRESH     3
#define L2CAP_BLE_TX_CONG_STOP_THRESH      1

/* Minimum offset for broadcast needs another two bytes for the PSM */
#define L2CAP_BCST_MIN_OFFSET       11

/* ping result codes */
#define L2CAP_PING_RESULT_OK        0       /* Ping reply received OK     */
#define L2CAP_PING_RESULT_NO_LINK   1       /* Link could not be setup    */
#define L2CAP_PING_RESULT_NO_RESP   2       /* Remote L2CAP did not reply */

/* result code for L2CA_DataWrite() */
#define L2CAP_DW_FAILED        FALSE
#define L2CAP_DW_SUCCESS       TRUE
#define L2CAP_DW_CONGESTED     2

/* Values for priority parameter to L2CA_SetAclPriority */
#define L2CAP_PRIORITY_NORMAL       0
#define L2CAP_PRIORITY_HIGH         1

/* Values for direction parameter to L2CA_SetAclPriorityExt */
#define L2CAP_DIRECTION_IGNORE              0       /* Set ACL priority direction as ignore */
#define L2CAP_DIRECTION_DATA_SOURCE         1       /* Set ACL priority direction as source */
#define L2CAP_DIRECTION_DATA_SINK           2       /* Set ACL priority direction as sink */


/* Values for priority parameter to L2CA_SetTxPriority */
#define L2CAP_CHNL_PRIORITY_HIGH    0
#define L2CAP_CHNL_PRIORITY_MEDIUM  1
#define L2CAP_CHNL_PRIORITY_LOW     2

typedef UINT8 tL2CAP_CHNL_PRIORITY;

/* Values for Tx/Rx data rate parameter to L2CA_SetChnlDataRate */
#define L2CAP_CHNL_DATA_RATE_HIGH       3
#define L2CAP_CHNL_DATA_RATE_MEDIUM     2
#define L2CAP_CHNL_DATA_RATE_LOW        1
#define L2CAP_CHNL_DATA_RATE_NO_TRAFFIC 0

typedef UINT8 tL2CAP_CHNL_DATA_RATE;

/* Data Packet Flags  (bits 2-15 are reserved) */
/* layer specific 14-15 bits are used for FCR SAR */
#define L2CAP_FLUSHABLE_MASK        0x0003
#define L2CAP_FLUSHABLE_CH_BASED    0x0000
#define L2CAP_FLUSHABLE_PKT         0x0001
#define L2CAP_NON_FLUSHABLE_PKT     0x0002


/* L2CA_FlushChannel num_to_flush definitions */
#define L2CAP_FLUSH_CHANS_ALL       0xffff
#define L2CAP_FLUSH_CHANS_GET       0x0000


/* special CID for Multi-AV for reporting congestion */
#define L2CAP_MULTI_AV_CID          0

/* length of the HCI header block */
/* HCI header(4) + SNK count(1) + FCR bits(1) + AV data length(2) */
#define L2CAP_MULTI_AV_HCI_HDR_LEN  8

/* length of padding for 4 bytes align */
#define L2CAP_MULTI_AV_PADDING_LEN  2

/* length of the HCI header block with padding for FCR */
/* HCI header(4) + SNK count(1) + FCR bits(1) + AV data length(2) + padding(2) */
#define L2CAP_MULTI_AV_HCI_HDR_LEN_WITH_PADDING 10

/* length of the L2CAP header block */
/* HCI header(4) + L2CAP header(4) + padding(4) or control word(2) + FCS(2) */
#define L2CAP_MULTI_AV_L2C_HDR_LEN  12

/* definition used for L2CA_SetDesireRole */
#define L2CAP_ROLE_SLAVE            HCI_ROLE_SLAVE
#define L2CAP_ROLE_MASTER           HCI_ROLE_MASTER
#define L2CAP_ROLE_ALLOW_SWITCH     0x80    /* set this bit to allow switch at create conn */
#define L2CAP_ROLE_DISALLOW_SWITCH  0x40    /* set this bit to disallow switch at create conn */
#define L2CAP_ROLE_CHECK_SWITCH     0xC0


/* Values for 'allowed_modes' field passed in structure tL2CAP_ERTM_INFO
*/
#define L2CAP_FCR_CHAN_OPT_BASIC    (1 << L2CAP_FCR_BASIC_MODE)
#define L2CAP_FCR_CHAN_OPT_ERTM     (1 << L2CAP_FCR_ERTM_MODE)
#define L2CAP_FCR_CHAN_OPT_STREAM   (1 << L2CAP_FCR_STREAM_MODE)

#define L2CAP_FCR_CHAN_OPT_ALL_MASK (L2CAP_FCR_CHAN_OPT_BASIC | L2CAP_FCR_CHAN_OPT_ERTM | L2CAP_FCR_CHAN_OPT_STREAM)

/* Validity check for PSM.  PSM values must be odd.  Also, all PSM values must
** be assigned such that the least significant bit of the most sigificant
** octet equals zero.
*/
#define L2C_INVALID_PSM(psm)    (((psm) & 0x0101) != 0x0001)
#define L2C_IS_VALID_PSM(psm)   (((psm) & 0x0101) == 0x0001)

/* Validity check for LE_PSM.
** Fixed LE_PSMs are in the range 0x0001 - 0x007F.
** Dynamic LE_PSM are in the range 0x0080 - 0x00FF.
** The values 0x0000 and 0x0100 - 0xFFFF are reserved.
*/
#define MIN_DYNAMIC_LE_PSM  0x0080
#define MAX_LE_PSM          0x00FF
#define L2C_BLE_INVALID_PSM(le_psm) (!(le_psm) || (le_psm) > MAX_LE_PSM)
#define L2C_BLE_IS_VALID_PSM(le_psm)   (((le_psm) != 0) && ((le_psm) <= MAX_LE_PSM))

#if (BTU_DUAL_STACK_BTC_INCLUDED == TRUE)
/* result code for L2CA_ReserveACLBuffer() */
#define L2CAP_RESERVE_FAILED        FALSE
#define L2CAP_RESERVE_SUCCESS       TRUE
#define L2CAP_RESERVE_PENDING       2
#endif

#if (BTA_MIP_INCLUDED == TRUE)
/* L2CAP CID for streaming  */
#define L2CAP_BASE_MIP_CID          0x00C0
#endif

/* Registration options */
#define L2CAP_REG_OPTS_NONE         0x00   /* No additional options */
#define L2CAP_REG_OPTS_NO_BT_HDR    0x01   /* (for BTEWICED wrapper only) Data callbacks use buffer-pointer + len instead of BT_HDR */

/* Valid range is 0x1 - 0x3F */
#define L2CAP_INVALID_FIXED_CID     0xFFFF

/*****************************************************************************
**  Type Definitions
*****************************************************************************/

typedef struct
{
#define L2CAP_FCR_BASIC_MODE    0x00
#define L2CAP_FCR_ERTM_MODE     0x03
#define L2CAP_FCR_STREAM_MODE   0x04

    UINT8  mode;

    UINT8  tx_win_sz;
    UINT8  max_transmit;
    UINT16 rtrans_tout;
    UINT16 mon_tout;
    UINT16 mps;
} tL2CAP_FCR_OPTS;

/* Define a structure to hold the configuration parameters. Since the
** parameters are optional, for each parameter there is a boolean to
** use to signify its presence or absence.
*/
typedef struct
{
    UINT16      result;                 /* Only used in confirm messages */
    BOOLEAN     mtu_present;
    UINT16      mtu;
    BOOLEAN     qos_present;
    FLOW_SPEC   qos;
    BOOLEAN     flush_to_present;
    UINT16      flush_to;
    BOOLEAN     fcr_present;
    tL2CAP_FCR_OPTS fcr;
    BOOLEAN     fcs_present;            /* Optionally bypasses FCS checks */
    UINT8       fcs;                    /* '0' if desire is to bypass FCS, otherwise '1' */
    UINT16      flags;                  /* bit 0: 0-no continuation, 1-continuation */
} tL2CAP_CFG_INFO;

/* L2CAP channel configured field bitmap */
#define L2CAP_CH_CFG_MASK_MTU           0x0001
#define L2CAP_CH_CFG_MASK_QOS           0x0002
#define L2CAP_CH_CFG_MASK_FLUSH_TO      0x0004
#define L2CAP_CH_CFG_MASK_FCR           0x0008
#define L2CAP_CH_CFG_MASK_FCS           0x0010
#define L2CAP_CH_CFG_MASK_EXT_FLOW_SPEC 0x0020

typedef UINT16 tL2CAP_CH_CFG_BITS;

/*********************************
**  Callback Functions Prototypes
**********************************/

/* Connection established callback prototype. Parameters are:
**              Caller context passed with L2CA_Register()
**              BD Address of remote
**              Local CID assigned to the connection
**              Peer MTU
*/
typedef void (tL2CA_CONNECTED_CB)(void *, BD_ADDR, UINT16, UINT16);


/* Disconnect indication callback prototype. Parameters are
**              Caller context passed with L2CA_Register()
**              Local CID
**              Boolean whether upper layer should ack this
*/
typedef void (tL2CA_DISCONNECT_IND_CB)(void *, UINT16, BOOLEAN);


/* Disconnect confirm callback prototype. Parameters are
**              Caller context passed with L2CA_Register()
**              Local CID
**              Result
*/
typedef void (tL2CA_DISCONNECT_CFM_CB)(void *, UINT16, UINT16);


/* Data received indication callback prototype. Parameters are
**              Caller context passed with L2CA_Register()
**              Local CID
**              Address of buffer
*/
typedef void (tL2CA_DATA_IND_CB)(void *, UINT16, BT_HDR *);


/* Congestion status callback protype. This callback is optional. If
** an application tries to send data when the transmit queue is full,
** the data will anyways be dropped. The parameter is:
**              Caller context passed with L2CA_Register()
**              Local CID
**              TRUE if congested, FALSE if uncongested
*/
typedef void (tL2CA_CONGESTION_STATUS_CB)(void *, UINT16, BOOLEAN);

/* Callback prototype for number of packets completed events.
** This callback notifies the application when Number of Completed Packets
** event has been received.
** This callback is originally designed for 3DG devices.
** The parameter is:
**          peer BD_ADDR
*/
typedef void (tL2CA_NOCP_CB)(BD_ADDR);

/* Transmit complete callback protype. This callback is optional. If
** set, L2CAP will call it when packets are sent or flushed. If the
** count is 0xFFFF, it means all packets are sent for that CID (eRTM
** mode only). The parameters are:
**              Caller context passed with L2CA_Register()
**              Local CID
**              Number of SDUs sent or dropped
*/
typedef void (tL2CA_TX_COMPLETE_CB)(void *, UINT16, UINT16);

/* Define the structure that applications use to register with
** L2CAP. This structure includes callback functions. All functions
** MUST be provided, with the exception of the "connect pending"
** callback and "congestion status" callback.
** Additionally, if registering client for dynamic PSM, pL2CA_ConnectInd_Cb() must
** be NULL since dynamic PSMs use this as a flag for "virtual PSM".
*/
typedef struct
{
    tL2CA_CONNECTED_CB          *pL2CA_Connected_Cb;
    tL2CA_DISCONNECT_IND_CB     *pL2CA_DisconnectInd_Cb;
    tL2CA_DISCONNECT_CFM_CB     *pL2CA_DisconnectCfm_Cb;
    tL2CA_DATA_IND_CB           *pL2CA_DataInd_Cb;
    tL2CA_CONGESTION_STATUS_CB  *pL2CA_CongestionStatus_Cb;
    tL2CA_TX_COMPLETE_CB        *pL2CA_TxComplete_Cb;

    UINT16                      mtu;
    BOOLEAN                     qos_present;
    FLOW_SPEC                   qos;
    BOOLEAN                     flush_to_present;
    UINT16                      flush_to;
    UINT8                       fcr_allowed_modes;      /* Set to 0 or L2CAP_FCR_CHAN_OPT_BASIC for no FCR */
    tL2CAP_FCR_OPTS             fcr;
    BOOLEAN                     fcs_present;
    UINT8                       fcs;                    /* '0' if desire is to bypass FCS, otherwise '1' */

    BOOLEAN                     is_ob_only;             /* Set to TRUE if registration is for outbound only to a dynamic PSM */
} tL2CAP_APPL_INFO;

/* Define the structure that applications use to create or accept
** connections with enhanced retransmission mode.
*/
typedef struct
{
    UINT8       preferred_mode;
    UINT8       allowed_modes;
    UINT8       user_rx_pool_id;
    UINT8       user_tx_pool_id;
    UINT8       fcr_rx_pool_id;
    UINT8       fcr_tx_pool_id;

} tL2CAP_ERTM_INFO;

#define L2CA_REGISTER(a,b,c)        L2CA_Register(a,(tL2CAP_APPL_INFO *)b, c)
#define L2CA_DEREGISTER(a)          L2CA_Deregister(a)
#define L2CA_CONNECT_REQ(a,b,c)   L2CA_ErtmConnectReq(a,b,c)
#define L2CA_CONNECT_RSP(a,b,c,d,e,f) L2CA_ErtmConnectRsp(a,b,c,d,e,f)
#define L2CA_CONFIG_REQ(a,b)        L2CA_ConfigReq(a,b)
#define L2CA_CONFIG_RSP(a,b)        L2CA_ConfigRsp(a,b)
#define L2CA_DISCONNECT_REQ(a)      L2CA_DisconnectReq(a)
#define L2CA_DISCONNECT_RSP(a)      L2CA_DisconnectRsp(a)
#define L2CA_DATA_WRITE(a, b)       L2CA_DataWrite(a, b)

/*****************************************************************************
**  External Function Declarations
*****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
**
** Function         L2CA_Register
**
** Description      Other layers call this function to register for L2CAP
**                  services.
**
**                  See L2CAP_REG_OPTS_* for registration options
**
** Returns          PSM to use or zero if error. Typically, the PSM returned
**                  is the same as was passed in, but for an outgoing-only
**                  connection to a dynamic PSM, a "virtual" PSM is returned
**                  and should be used in the calls to L2CA_ConnectReq() and
**                  BTM_SetSecurityLevel().
**
*******************************************************************************/
L2C_API extern UINT16 L2CA_Register(UINT16 psm, tL2CAP_APPL_INFO *p_cb_info, UINT8 reg_opts, void *context);

/*******************************************************************************
**
** Function         L2CA_Deregister
**
** Description      Other layers call this function to deregister for L2CAP
**                  services.
**
** Returns          void
**
*******************************************************************************/
L2C_API extern void L2CA_Deregister(UINT16 psm);

/*******************************************************************************
**
** Function         L2CA_AllocatePSM
**
** Description      Other layers call this function to find an unused PSM for L2CAP
**                  services.
**
** Returns          PSM to use.
**
*******************************************************************************/
L2C_API extern UINT16 L2CA_AllocatePSM(void);

/*******************************************************************************
**
** Function         L2CA_ConnectReq
**
** Description      Higher layers call this function to create an L2CAP connection.
**                  Note that the connection is not established at this time, but
**                  connection establishment gets started. The callback function
**                  will be invoked when connection establishes or fails.
**
** Returns          the CID of the connection, or 0 if it failed to start
**
*******************************************************************************/
L2C_API extern UINT16 L2CA_ConnectReq(UINT16 psm, BD_ADDR p_bd_addr);

/*******************************************************************************
**
** Function         L2CA_ConnectRsp
**
** Description      Higher layers call this function to accept an incoming
**                  L2CAP connection, for which they had gotten an connect
**                  indication callback.
**
** Returns          TRUE for success, FALSE for failure
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_ConnectRsp(BD_ADDR p_bd_addr, UINT8 id, UINT16 lcid,
                                       UINT16 result, UINT16 status);


/*******************************************************************************
**
** Function         L2CA_ErtmEnable
**
** Description      Enable ERTM.
**
**                  Calling this function will cause the linker to include
**                  ERTM related functions.
**
** Parameters
**                  tL2C_CCB
**
** Returns          void
**
*******************************************************************************/
void L2CA_ErtmEnable(void);

/*******************************************************************************
**
** Function         L2CA_ErtmConnectReq
**
** Description      Higher layers call this function to create an L2CAP connection
**                  that needs to use Enhanced Retransmission Mode.
**                  Note that the connection is not established at this time, but
**                  connection establishment gets started. The callback function
**                  will be invoked when connection establishes or fails.
**
** Returns          the CID of the connection, or 0 if it failed to start
**
*******************************************************************************/
L2C_API extern UINT16 L2CA_ErtmConnectReq(UINT16 psm, BD_ADDR p_bd_addr,
        tL2CAP_ERTM_INFO *p_ertm_info);

/*******************************************************************************
**
** Function         L2CA_ErtmConnectRsp
**
** Description      Higher layers call this function to accept an incoming
**                  L2CAP connection, for which they had gotten an connect
**                  indication callback, and for which the higher layer wants
**                  to use Enhanced Retransmission Mode.
**
** Returns          TRUE for success, FALSE for failure
**
*******************************************************************************/
L2C_API extern BOOLEAN  L2CA_ErtmConnectRsp(BD_ADDR p_bd_addr, UINT8 id, UINT16 lcid,
        UINT16 result, UINT16 status,
        tL2CAP_ERTM_INFO *p_ertm_info);

/*******************************************************************************
**
** Function         L2CA_DisconnectReq
**
** Description      Higher layers call this function to disconnect a channel.
**
** Returns          TRUE if disconnect sent, else FALSE
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_DisconnectReq(UINT16 cid);

/*******************************************************************************
**
** Function         L2CA_DisconnectRsp
**
** Description      Higher layers call this function to acknowledge the
**                  disconnection of a channel.
**
** Returns          void
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_DisconnectRsp(UINT16 cid);

/*******************************************************************************
**
** Function         L2CA_DataWrite
**
** Description      Higher layers call this function to write data.
**
** Returns          L2CAP_DW_SUCCESS, if data accepted, else FALSE
**                  L2CAP_DW_CONGESTED, if data accepted and the channel is congested
**                  L2CAP_DW_FAILED, if error
**
*******************************************************************************/
L2C_API extern UINT8 L2CA_DataWrite(UINT16 cid, BT_HDR *p_data);

/*******************************************************************************
**
** Function         L2CA_SetIdleTimeout
**
** Description      Higher layers call this function to set the idle timeout for
**                  a connection, or for all future connections. The "idle timeout"
**                  is the amount of time that a connection can remain up with
**                  no L2CAP channels on it. A timeout of zero means that the
**                  connection will be torn down immediately when the last channel
**                  is removed. A timeout of 0xFFFF means no timeout. Values are
**                  in seconds.
**
** Returns          TRUE if command succeeded, FALSE if failed
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_SetIdleTimeout(UINT16 cid, UINT16 timeout,
        BOOLEAN is_global);

/*******************************************************************************
**
** Function         L2CA_SetIdleTimeoutByBdAddr
**
** Description      Higher layers call this function to set the idle timeout for
**                  a connection. The "idle timeout" is the amount of time that
**                  a connection can remain up with no L2CAP channels on it.
**                  A timeout of zero means that the connection will be torn
**                  down immediately when the last channel is removed.
**                  A timeout of 0xFFFF means no timeout. Values are in seconds.
**                  A bd_addr is the remote BD address. If bd_addr = BT_BD_ANY,
**                  then the idle timeouts for all active l2cap links will be
**                  changed.
**
** Returns          TRUE if command succeeded, FALSE if failed
**
** NOTE             This timeout applies to all logical channels active on the
**                  ACL link.
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_SetIdleTimeoutByBdAddr(BD_ADDR bd_addr, UINT16 timeout,
        tBT_TRANSPORT transport);

/*******************************************************************************
**
** Function         L2CA_SetTraceLevel
**
** Description      This function sets the trace level for L2CAP. If called with
**                  a value of 0xFF, it simply reads the current trace level.
**
** Returns          the new (current) trace level
**
*******************************************************************************/
L2C_API extern UINT8 L2CA_SetTraceLevel(UINT8 trace_level);

/*******************************************************************************
**
** Function     L2CA_SetDesireRole
**
** Description  This function sets the desire role for L2CAP.
**              If the new role is L2CAP_ROLE_ALLOW_SWITCH, allow switch on
**              HciCreateConnection.
**              If the new role is L2CAP_ROLE_DISALLOW_SWITCH, do not allow switch on
**              HciCreateConnection.
**
**              If the new role is a valid role (HCI_ROLE_MASTER or HCI_ROLE_SLAVE),
**              the desire role is set to the new value. Otherwise, it is not changed.
**
** Returns      the new (current) role
**
*******************************************************************************/
L2C_API extern UINT8 L2CA_SetDesireRole(UINT8 new_role);

/*******************************************************************************
**
** Function     L2CA_LocalLoopbackReq
**
** Description  This function sets up a CID for local loopback
**
** Returns      CID of 0 if none.
**
*******************************************************************************/
L2C_API extern UINT16 L2CA_LocalLoopbackReq(UINT16 psm, UINT16 handle, BD_ADDR p_bd_addr);

/*******************************************************************************
**
** Function     L2CA_FlushChannel
**
** Description  This function flushes none, some or all buffers queued up
**              for xmission for a particular CID. If called with
**              L2CAP_FLUSH_CHANS_GET (0), it simply returns the number
**              of buffers queued for that CID L2CAP_FLUSH_CHANS_ALL (0xffff)
**              flushes all buffers.  All other values specifies the maximum
**              buffers to flush.
**
** Returns      Number of buffers left queued for that CID
**
*******************************************************************************/
L2C_API extern UINT16   L2CA_FlushChannel(UINT16 lcid, UINT16 num_to_flush);


/*******************************************************************************
**
** Function         L2CA_SetAclPriority
**
** Description      Sets the transmission priority for an ACL channel.
**                  (For initial implementation only two values are valid.
**                  L2CAP_PRIORITY_NORMAL and L2CAP_PRIORITY_HIGH).
**
** Returns          TRUE if a valid channel, else FALSE
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_SetAclPriority(BD_ADDR bd_addr, UINT8 priority);

/*******************************************************************************
**
** Function         L2CA_SetAclPriorityExt
**
** Description      Sets the priority for an ACL channel
**                  with extended parameters.
**                  Priority level values:
**                  L2CAP_PRIORITY_NORMAL and L2CAP_PRIORITY_HIGH
**                  Direction of data:
**                  L2CAP_DIRECTION_DATA_SOURCE and L2CAP_DIRECTION_DATA_SINK
** Returns          TRUE if a valid channel, else FALSE
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_SetAclPriorityExt(BD_ADDR bd_addr, UINT8 priority, UINT8 direction);

/*******************************************************************************
**
** Function         L2CA_FlowControl
**
** Description      Higher layers call this function to flow control a channel.
**
**                  data_enabled - TRUE data flows, FALSE data is stopped
**
** Returns          TRUE if valid channel, else FALSE
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_FlowControl(UINT16 cid, BOOLEAN data_enabled);

/*******************************************************************************
**
** Function         L2CA_SendTestSFrame
**
** Description      Higher layers call this function to send a test S-frame.
**
** Returns          TRUE if valid Channel, else FALSE
**
*******************************************************************************/
L2C_API extern  BOOLEAN L2CA_SendTestSFrame(UINT16 cid, UINT8 sup_type, UINT8 back_track);

/*******************************************************************************
**
** Function         L2CA_SetTxPriority
**
** Description      Sets the transmission priority for a channel. (FCR Mode)
**
** Returns          TRUE if a valid channel, else FALSE
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_SetTxPriority(UINT16 cid, tL2CAP_CHNL_PRIORITY priority);

/*******************************************************************************
**
** Function         L2CA_RegForNoCPEvt
**
** Description      Register callback for Number of Completed Packets event.
**
** Input Param      p_cb - callback for Number of completed packets event
**                  p_bda - BT address of remote device
**
** Returns
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_RegForNoCPEvt(tL2CA_NOCP_CB *p_cb, BD_ADDR p_bda);

/*******************************************************************************
**
** Function         L2CA_SetChnlDataRate
**
** Description      Sets the tx/rx data rate for a channel.
**
** Returns          TRUE if a valid channel, else FALSE
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_SetChnlDataRate(UINT16 cid, tL2CAP_CHNL_DATA_RATE tx, tL2CAP_CHNL_DATA_RATE rx);

typedef void (tL2CA_RESERVE_CMPL_CBACK)(void);

#if (BTU_DUAL_STACK_BTC_INCLUDED == TRUE)
/*******************************************************************************
**
** Function         L2CA_ReserveACLBuffer
**
** Description
**
** Returns
**
*******************************************************************************/
L2C_API extern UINT8 L2CA_ReserveACLBuffer(UINT8 num_buffer, tL2CA_RESERVE_CMPL_CBACK *p_cback);

/*******************************************************************************
**
** Function         L2CA_UnreservedACLBuffer
**
** Description
**
** Returns
**
*******************************************************************************/
L2C_API extern void L2CA_UnreserveACLBuffer(UINT8 num_buffer);

#endif

/*******************************************************************************
**
** Function         L2CA_SetFlushTimeout
**
** Description      This function set the automatic flush time out in Baseband
**                  for ACL-U packets.
**                  BdAddr : the remote BD address of ACL link. If it is BT_DB_ANY
**                           then the flush time out will be applied to all ACL link.
**                  FlushTimeout: flush time out in ms
**                           0x0000 : No automatic flush
**                           L2CAP_NO_RETRANSMISSION : No retransmission
**                           0x0002 - 0xFFFE : flush time out, if (flush_tout*8)+3/5)
**                                    <= HCI_MAX_AUTO_FLUSH_TOUT (in 625us slot).
**                                    Otherwise, return FALSE.
**                           L2CAP_NO_AUTOMATIC_FLUSH : No automatic flush
**
** Returns          TRUE if command succeeded, FALSE if failed
**
** NOTE             This flush timeout applies to all logical channels active on the
**                  ACL link.
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_SetFlushTimeout(BD_ADDR bd_addr, UINT16 flush_tout);

/*******************************************************************************
**
** Function         L2CA_DataWriteEx
**
** Description      Higher layers call this function to write data with extended
**                  flags.
**                  flags : L2CAP_FLUSHABLE_CH_BASED
**                          L2CAP_FLUSHABLE_PKT
**                          L2CAP_NON_FLUSHABLE_PKT
**
** Returns          L2CAP_DW_SUCCESS, if data accepted, else FALSE
**                  L2CAP_DW_CONGESTED, if data accepted and the channel is congested
**                  L2CAP_DW_FAILED, if error
**
*******************************************************************************/
L2C_API extern UINT8 L2CA_DataWriteEx(UINT16 cid, BT_HDR *p_data, UINT16 flags);

/*******************************************************************************
**
** Function         L2CA_SetChnlFlushability
**
** Description      Higher layers call this function to set a channels
**                  flushability flags
**
** Returns          TRUE if CID found, else FALSE
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_SetChnlFlushability(UINT16 cid, BOOLEAN is_flushable);

#if (BTU_MULTI_AV_INCLUDED == TRUE)
/*******************************************************************************
**
** Function         L2CA_MultiAvCid
**
** Description      Add or remove L2CAP header in Multi-AV packet
**                  Set is_add to TRUE to add
**                  Set is_add to FALSE to remove
**
** Returns          None
**
*******************************************************************************/
L2C_API extern void L2CA_MultiAvCid(UINT16 lcid, BOOLEAN is_add);

/*******************************************************************************
**
** Function         L2CA_MultiAvWrite
**
** Description      Sending Multi-AV packet
**                  p_data must have enough offset to add Multi-AV packet header
**
** Returns          L2CAP_DW_SUCCESS, if data accepted
**                  L2CAP_DW_CONGESTED, if data accepted and the channel is congested
**                  L2CAP_DW_FAILED, if error
**
*******************************************************************************/
L2C_API extern UINT8 L2CA_MultiAvWrite(BT_HDR *p_data);
#endif /*(BTU_MULTI_AV_INCLUDED == TRUE)*/

/*******************************************************************************
**
**  Function         L2CA_GetPeerFeatures
**
**  Description      Get a peers features and fixed channel map
**
**  Parameters:      BD address of the peer
**                   Pointers to features and channel mask storage area
**
**  Return value:    TRUE if peer is connected
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_GetPeerFeatures(BD_ADDR bd_addr, UINT32 *p_ext_feat, UINT8 *p_chnl_mask);

/*******************************************************************************
**
**  Function         L2CA_GetBDAddrbyHandle
**
**  Description      Get BD address for the given HCI handle
**
**  Parameters:      HCI handle
**                   BD address of the peer
**
**  Return value:    TRUE if found lcb for the given handle, FALSE otherwise
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_GetBDAddrbyHandle(UINT16 handle, BD_ADDR bd_addr);

/*******************************************************************************
**
**  Function         L2CA_GetChnlFcrMode
**
**  Description      Get the channel FCR mode
**
**  Parameters:      Local CID
**
**  Return value:    Channel mode
**
*******************************************************************************/
L2C_API extern UINT8 L2CA_GetChnlFcrMode(UINT16 lcid);


/*******************************************************************************
**
**                      UCD callback prototypes
**
*******************************************************************************/

/* UCD discovery. Parameters are
**      BD Address of remote
**      Data Type
**      Data
*/
#define L2CAP_UCD_INFO_TYPE_RECEPTION   0x01
#define L2CAP_UCD_INFO_TYPE_MTU         0x02

typedef void (tL2CA_UCD_DISCOVER_CB)(BD_ADDR, UINT8, UINT32);

/* UCD data received. Parameters are
**      BD Address of remote
**      Pointer to buffer with data
*/
typedef void (tL2CA_UCD_DATA_CB)(BD_ADDR, BT_HDR *);

/* Congestion status callback protype. This callback is optional. If
** an application tries to send data when the transmit queue is full,
** the data will anyways be dropped. The parameter is:
**              remote BD_ADDR
**              TRUE if congested, FALSE if uncongested
*/
typedef void (tL2CA_UCD_CONGESTION_STATUS_CB)(BD_ADDR, BOOLEAN);

/* UCD registration info (the callback addresses and PSM)
*/
typedef struct
{
    tL2CA_UCD_DISCOVER_CB           *pL2CA_UCD_Discover_Cb;
    tL2CA_UCD_DATA_CB               *pL2CA_UCD_Data_Cb;
    tL2CA_UCD_CONGESTION_STATUS_CB  *pL2CA_UCD_Congestion_Status_Cb;
} tL2CAP_UCD_CB_INFO;

/*******************************************************************************
**
**  Function        L2CA_UcdRegister
**
**  Description     Register PSM on UCD.
**
**  Parameters:     tL2CAP_UCD_CB_INFO
**
**  Return value:   TRUE if successs
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_UcdRegister(UINT16 psm, tL2CAP_UCD_CB_INFO *p_cb_info);

/*******************************************************************************
**
**  Function        L2CA_UcdDeregister
**
**  Description     Deregister PSM on UCD.
**
**  Parameters:     PSM
**
**  Return value:   TRUE if successs
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_UcdDeregister(UINT16 psm);

/*******************************************************************************
**
**  Function        L2CA_UcdDiscover
**
**  Description     Discover UCD of remote device.
**
**  Parameters:     PSM
**                  BD_ADDR of remote device
**                  info_type : L2CAP_UCD_INFO_TYPE_RECEPTION
**                              L2CAP_UCD_INFO_TYPE_MTU
**
**
**  Return value:   TRUE if successs
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_UcdDiscover(UINT16 psm, BD_ADDR rem_bda, UINT8 info_type);

/*******************************************************************************
**
**  Function        L2CA_UcdDataWrite
**
**  Description     Send UCD to remote device
**
**  Parameters:     PSM
**                  BD Address of remote
**                  Pointer to buffer of type BT_HDR
**                  flags : L2CAP_FLUSHABLE_CH_BASED
**                          L2CAP_FLUSHABLE_PKT
**                          L2CAP_NON_FLUSHABLE_PKT
**
** Return value     L2CAP_DW_SUCCESS, if data accepted
**                  L2CAP_DW_FAILED,  if error
**
*******************************************************************************/
L2C_API extern UINT16 L2CA_UcdDataWrite(UINT16 psm, BD_ADDR rem_bda, BT_HDR *p_buf, UINT16 flags);

/*******************************************************************************
**
**  Function        L2CA_UcdSetIdleTimeout
**
**  Description     Set UCD Idle timeout.
**
**  Parameters:     BD Addr
**                  Timeout in second
**
**  Return value:   TRUE if successs
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_UcdSetIdleTimeout(BD_ADDR rem_bda, UINT16 timeout);

/*******************************************************************************
**
** Function         L2CA_UCDSetTxPriority
**
** Description      Sets the transmission priority for a connectionless channel.
**
** Returns          TRUE if a valid channel, else FALSE
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_UCDSetTxPriority(BD_ADDR rem_bda, tL2CAP_CHNL_PRIORITY priority);


/*******************************************************************************
**
**                      Fixed Channel callback prototypes
**
*******************************************************************************/

/* Fixed channel connected and disconnected. Parameters are
**      BD Address of remote
**      TRUE if channel is connected, FALSE if disconnected
**      Reason for connection failure
*/
#if BLE_INCLUDED == TRUE
typedef void (tL2CA_FIXED_CHNL_CB)(BD_ADDR, BOOLEAN, UINT16, tBT_TRANSPORT);
#else
typedef void (tL2CA_FIXED_CHNL_CB)(BD_ADDR, BOOLEAN, UINT16);
#endif
/* Signalling data received. Parameters are
**      BD Address of remote
**      Pointer to buffer with data
*/
typedef void (tL2CA_FIXED_DATA_CB)(BD_ADDR, BT_HDR *);

/* Congestion status callback protype. This callback is optional. If
** an application tries to send data when the transmit queue is full,
** the data will anyways be dropped. The parameter is:
**              remote BD_ADDR
**              TRUE if congested, FALSE if uncongested
*/
typedef void (tL2CA_FIXED_CONGESTION_STATUS_CB)(BD_ADDR, BOOLEAN);

/* Fixed channel registration info (the callback addresses and channel config)
*/
typedef struct
{
    UINT16                              channel_id;
    BOOLEAN                             in_use;
    tL2CA_FIXED_CHNL_CB                 *pL2CA_FixedConn_Cb;
    tL2CA_FIXED_DATA_CB                 *pL2CA_FixedData_Cb;
    tL2CA_FIXED_CONGESTION_STATUS_CB    *pL2CA_FixedCong_Cb;
    UINT16                              default_idle_tout;
    tL2CA_TX_COMPLETE_CB                *pL2CA_FixedTxComplete_Cb;            /* fixed channel tx complete callback */
    void                                *context;
} tL2CAP_FIXED_CHNL_REG;


#if ( defined(L2CAP_FIXED_CHANNELS_SUPPORTED) && ( L2CAP_FIXED_CHANNELS_SUPPORTED == TRUE ))
/*******************************************************************************
**
**  Function        L2CA_RegisterFixedChannel
**
**  Description     Register a fixed channel.
**
**  Parameters:     Fixed Channel #
**                  Channel Callbacks and config
**                  Options (see L2CAP_REG_OPTS_*)
**
**  Return value:   TRUE if registered OK
**
*******************************************************************************/
L2C_API extern BOOLEAN  L2CA_RegisterFixedChannel(UINT16 fixed_cid, tL2CAP_FIXED_CHNL_REG *p_freg, UINT8 reg_opts);

/*******************************************************************************
**
**  Function        L2CA_GetFixedCidIdx
**
**  Description     Get fixed channel index to access registration info. pointer for given channel id
**
**                  This function can be used to access the following structures,
**                      1) tL2C_CCB  **p_fixed_ccbs (from tL2C_LCB)
**                      2) tL2CAP_FIXED_CHNL_REG   *fixed_reg (from tL2C_CB)
**                      3) UINT8                   *fixed_reg_opts (from tL2C_CB)
**
**  NOTE: The reason to have a separate function to find index is that, we are required to support
**  fixed channels 4, 5 and 6 mandatorily for LE. And in future we may require to allow data transfer
**  over other fixed channels as well. As per spec. the range of valid fixed channels if from
**  (0x0001 to 0x003F). So we always allocate memory and reserve the first three slots of the
**  L2CAP fixed channel control blocks to 4, 5 and 6. The rest of the slots (depends on the app.
**  configuartion through "max_le_l2cap_fixed_channels") are used for other fixed channels.
**  Hence if the cid is 4, 5 or 6 the index is relative. For the others we have to loop through
**  all th available slots if valid entry exists.
**
**  Parameters:     Fixed Channel #
**
**  Return value:   indec corresponding to the fixed channel id or 0xFFFF if info does not exist
**
*******************************************************************************/
UINT16 L2CA_GetFixedCidIdx(UINT16 fixed_cid);

/*******************************************************************************
**
**  Function        L2CA_ConnectFixedChnl
**
**  Description     Connect an fixed signalling channel to a remote device.
**
**  Parameters:     Fixed CID
**                  BD Address of remote
**
**  Return value:   TRUE if connection started
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_ConnectFixedChnl(UINT16 fixed_cid, BD_ADDR bd_addr);

/*******************************************************************************
**
**  Function        L2CA_SendFixedChnlData
**
**  Description     Write data on a fixed signalling channel.
**
**  Parameters:     Fixed CID
**                  BD Address of remote
**                  Pointer to buffer of type BT_HDR
**
** Return value     L2CAP_DW_SUCCESS, if data accepted
**                  L2CAP_DW_FAILED,  if error
**
*******************************************************************************/
L2C_API extern UINT16 L2CA_SendFixedChnlData(UINT16 fixed_cid, BD_ADDR rem_bda, BT_HDR *p_buf);

/*******************************************************************************
**
**  Function        L2CA_RemoveFixedChnl
**
**  Description     Remove a fixed channel to a remote device.
**
**  Parameters:     Fixed CID
**                  BD Address of remote
**                  Idle timeout to use (or 0xFFFF if don't care)
**
**  Return value:   TRUE if channel removed
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_RemoveFixedChnl(UINT16 fixed_cid, BD_ADDR rem_bda);

/*******************************************************************************
**
** Function         L2CA_SetFixedChannelTout
**
** Description      Higher layers call this function to set the idle timeout for
**                  a fixed channel. The "idle timeout" is the amount of time that
**                  a connection can remain up with no L2CAP channels on it.
**                  A timeout of zero means that the connection will be torn
**                  down immediately when the last channel is removed.
**                  A timeout of 0xFFFF means no timeout. Values are in seconds.
**                  A bd_addr is the remote BD address. If bd_addr = BT_BD_ANY,
**                  then the idle timeouts for all active l2cap links will be
**                  changed.
**
** Returns          TRUE if command succeeded, FALSE if failed
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_SetFixedChannelTout(BD_ADDR rem_bda, UINT16 fixed_cid, UINT16 idle_tout);

#endif /* (L2CAP_FIXED_CHANNELS_SUPPORTED > 0) */

/*******************************************************************************
**
** Function     L2CA_GetCurrentConfig
**
** Description  This function returns configurations of L2CAP channel
**              pp_our_cfg : pointer of our saved configuration options
**              p_our_cfg_bits : valid config in bitmap
**              pp_peer_cfg: pointer of peer's saved configuration options
**              p_peer_cfg_bits : valid config in bitmap
**
** Returns      TRUE if successful
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_GetCurrentConfig(UINT16 lcid,
        tL2CAP_CFG_INFO **pp_our_cfg,  tL2CAP_CH_CFG_BITS *p_our_cfg_bits,
        tL2CAP_CFG_INFO **pp_peer_cfg, tL2CAP_CH_CFG_BITS *p_peer_cfg_bits);

#if (L2CAP_CORRUPT_ERTM_PKTS == TRUE)
/*******************************************************************************
**
** Function         L2CA_SetupErtmTest
**
** Description      This function is used for testing purposes only.
**                  It corrupts or drops one or more packets used with ERTM channels.
**
** Parameters
**                  cid - channel ID  (0 uses RFCOMM PSM's CID)
**
**                  type - type of test to run (L2CAP_FCR_TTYPE_CORR_IFRAMES
**                                              L2CAP_FCR_TTYPE_CORR_SFRAME
**                                              L2CAP_FCR_TTYPE_STOP_TEST
**                                              L2CAP_FCR_TTYPE_GET_CID - returns rfcomm cid only)
**
**                  is_rx  - TRUE to corrupt Rx packet, FALSE for Tx packet)
**
**                  freq - L2CAP_FCR_FREQ_RANDOM    (turns on random corruptions/drops)
**                         L2CAP_FCR_FREQ_NORMAL    (turns on test with "count" corruptions/drops)
**
**                  count - number of packets in a row to drop or corrupt
**
** Returns          CID of channel running test
**
*******************************************************************************/
L2C_API extern UINT16 L2CA_SetupErtmTest(UINT16 cid, UINT8 type, BOOLEAN is_rx, UINT8 freq, UINT16 count);

/*******************************************************************************
**
** Function         L2CA_SendPolledSFrame
**
** Description      This function is used for testing purposes only.
**                  It Sends a Polled RR or RNR to the peer
**
** Parameters
**                  cid - channel ID
**
**                  sup_type - (L2CAP_FCR_SUP_RR or L2CAP_FCR_SUP_RNR)
**
** Returns          void
**
*******************************************************************************/
L2C_API extern void L2CA_SendPolledSFrame(UINT16 cid, UINT16 sup_type);

/*******************************************************************************
**
** Function         L2CA_BypassSFrame
**
** Description      This function is used for testing purposes only.
**                  It skips sending 'count' S-Frames.
**
** Parameters
**                  cid - channel ID
**
**                  count - Number of S-Frames to skip sending
**
** Returns          void
**
*******************************************************************************/
L2C_API extern void L2CA_BypassSFrame(UINT16 cid, UINT8 count);

#endif /* (L2CAP_CORRUPT_ERTM_PKTS == TRUE) */

/*******************************************************************************
**
**                              AMP callback prototypes
**
*******************************************************************************/

/* AMP Channel Creation Indication. Parameters are
**      BD Address of remote
**      Local CID assigned to the connection
**      PSM that the remote wants to connect to
**      Identifier that the remote sent
**      Local controller ID the remote wants to connect to
*/
typedef void (tL2CA_AMP_CHNL_IND_CB)(BD_ADDR, UINT16, UINT16, UINT8, UINT8);

/* AMP Channel Creation Confirmation. Parameters are
**      Local CID
**      Result - 0 = connected, non-zero means failure reason
*/
typedef void (tL2CA_AMP_CHNL_CFM_CB)(UINT16, UINT16);

/* AMP Channel Move Indication. Parameters are
**      Local CID
**      Local controller ID the remote wants to move to
*/
typedef void (tL2CA_AMP_MOVE_IND_CB)(UINT16, UINT8);

/* AMP Channel Move Response and Confirm. Parameters are
**      Local CID
**      Result - 0 = OK, non-zero means failure reason
**      TRUE if response, FALSE if confirm
*/
typedef void (tL2CA_AMP_MOVE_RSPCFM_CB)(UINT16, UINT16, BOOLEAN);

/* AMP Channel Move Confirmation Response. Parameter is
**      Local CID
*/
typedef void (tL2CA_AMP_MOVE_CNF_RSP_CB)(UINT16);

/* AMP Get Tx Extened Flow Spec of logical link. Parameters are
**      Local controller ID for the logical link
**      Logical link handle
**      Pointer to the place to put Tx Extened Flow Spec
*/
typedef BOOLEAN(tL2CA_AMP_GET_TX_FLOW)(UINT8 amp_idx, UINT16 ll_hndl,
                                       tHCI_EXT_FLOW_SPEC *p_tx_ext_flow_spec);


/* AMP registration info (basically, all the callback addresses)
*/
typedef struct
{
    tL2CA_AMP_CHNL_IND_CB           *pL2CA_AmpChnlInd_Cb;
    tL2CA_AMP_CHNL_CFM_CB           *pL2CA_AmpChnlCfm_Cb;
    tL2CA_AMP_MOVE_IND_CB           *pL2CA_AmpMoveInd_Cb;
    tL2CA_AMP_MOVE_RSPCFM_CB        *pL2CA_AmpMoveRspCfm_Cb;
    tL2CA_AMP_MOVE_CNF_RSP_CB       *pL2CA_AmpMoveCfmRsp_Cb;
    tL2CA_AMP_GET_TX_FLOW           *pL2CA_AmpGetTxFlow_Cb;
} tL2CAP_AMP_REG_INFO;


#if (AMP_INCLUDED == TRUE)
/*******************************************************************************
**
**                     API Functions provided by L2CAP
**
*******************************************************************************/

/*******************************************************************************
**
** Function         L2CA_AmpRegister
**
** Description      AMP registration.
**
** Parameters
**                  List of function pointers to be called
**
** Returns          void
**
*******************************************************************************/
L2C_API extern void L2CA_AmpRegister(tL2CAP_AMP_REG_INFO *p_cb_info,
                                     tL2CA_FIXED_CHNL_CB *pAmpSigConnCb,
                                     tL2CA_FIXED_DATA_CB *pAmpSignalDataCb);

/*******************************************************************************
**
**  Function         L2CA_AmpCreateChannel
**
**  Description      Create an L2CAP channel on a non-BR-EDR physical link
**
**  Parameters:
**                   PSM to connect to on remote
**                   BD Address of remote
**                   Remote controller ID to connect to
**                   Pre-reserved CID to use, or 0 if new CID to be gotten
**
**  Return value:
**                   CID assigned to local endpoint of the channel or 0 if error
**
*******************************************************************************/
L2C_API extern UINT16 L2CA_AmpCreateChannel(UINT16 psm, BD_ADDR bd_addr, UINT8 rem_ctrlr,
        UINT16 reserved_cid, tL2CAP_ERTM_INFO *p_ertm_info);

/*******************************************************************************
**
**  Function         L2CA_AmpAssignment
**
**  Description      Assign an L2CAP channel to an AMP PAL and AMP logical Channel
**
**  Parameters:
**                   Local CID assigned to the connection
**                   Index of local controller to be used on the connection
**                   For moving to BR/EDR controller, use LOCAL_BR_EDR_CONTROLLER_ID
**                   Logical link handle to be used on the connection
**  Return value:
**                   TRUE if successful
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_AmpAssignment(UINT16 lcid, UINT8 local_controller_inx, UINT16 ll_handle);

/*******************************************************************************
**
**  Function         L2CA_AmpRemoveChannel
**
**  Description      Function is called when Logical Channel is released
**
**  Parameters:
**                   Index of local controller used on the connection
**                   Logical link handle
**  Return value:
**                   TRUE if successful
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_AmpRemoveChannel(UINT8 local_amp_id, UINT16 amp_llc);

/*******************************************************************************
**
**  Function         L2CA_AmpMoveChannel
**
**  Description      Move channel request
**
**  Parameters:
**                   Local CID assigned to the connection
**                   New remote controller ID to connect to
**                   Index of new local AMP controller to be used on the connection
**  Return value:
**                   TRUE if successful
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_AmpMoveChannel(UINT16 lcid, UINT8 rem_ctrlr);

/*******************************************************************************
**
**  Function         L2CA_AmpMoveChanRsp
**
**  Description      Move channel response
**
**  Parameters:
**                   Local CID assigned to the connection
**                   Result of move request
**
**  Return value:
**                   TRUE if successful
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_AmpMoveChanRsp(UINT16 lcid, UINT16 result);

/*******************************************************************************
**
**  Function         L2CA_AmpMoveChanCfm
**
**  Description      Move channel confirm
**
**  Parameters:
**      Local CID assigned to the connection
**      Result of move request
**
**  Return value:
**                   TRUE if successful
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_AmpMoveChanCfm(UINT16 lcid, UINT16 result);

/*******************************************************************************
**
**  Function         L2CA_ReserveCID
**
**  Description      Reserve a Channel ID
**
**  Parameters:      ERTM info as passed from the application
**
**  Return value:
**                   CID that was reserved
**
*******************************************************************************/
L2C_API extern UINT16 L2CA_ReserveCID(tL2CAP_ERTM_INFO *p_ertm_info);

/*******************************************************************************
**
**  Function         L2CA_UnreserveCID
**
**  Description      Unreserve a Channel ID
**
**  Parameters:
**                   CID that was reserved
**                   Pointer to output ertm_info
**
**  Return value:
**                   None
**
*******************************************************************************/
L2C_API extern void L2CA_UnreserveCID(UINT16 lcid, tL2CAP_ERTM_INFO *p_ertm_info);

/*******************************************************************************
**
** Function         L2CA_AmpAddLc
**
** Description      Set tells L2CAP about new controller
**
** Parameters       local_amp_id - local AMP controller ID
**
** Returns          TRUE if success
**                  FALSE if fail
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_AmpAddLc(UINT8 local_amp_id);

#endif /* (AMP_INCLUDED == TRUE) */


/* LE Connection indication callback prototype. Parameters are
**              BD Address of remote
**              Local CID assigned to the connection
**              PSM that the remote wants to connect to
**              Identifier that the remote sent
**              MTU of the peer
**
*/
typedef void (tL2CA_LE_CONNECT_IND_CB)(void *, BD_ADDR, UINT16, UINT16, UINT8, UINT16);


/* LE Connection confirmation callback prototype. Parameters are
**              Local CID
**              Result - 0 = connected, non-zero means failure reason
**              MTU of the peer
*/
typedef void (tL2CA_LE_CONNECT_CFM_CB)(void *, UINT16, UINT16, UINT16);

/* LE TX complete
** Notification after L2CA_LeDataWrite has sent buffer to transport layer
**
** Parameters:
**              Local CID
**              Number of buffers sent
*/
typedef void (tL2CA_LE_TX_COMPLETE_CB)(void *, UINT16, UINT16);

#if (L2CAP_LE_COC_CONFORMANCE_TESTING == TRUE)
typedef void (tL2CA_LE_CONF_TEST_CB)(UINT16);
#endif


/* Define the structure that applications use to register with
** LE L2CAP. This structure includes callback functions. All functions
** MUST be provided, with the exception of the "connect pending"
** callback and "congestion status" callback.
** Additionally, if registering client for dynamic PSM, pL2CA_ConnectInd_Cb() must
** be NULL since dynamic PSMs use this as a flag for "virtual PSM".
*/
typedef struct
{
    tL2CA_LE_CONNECT_IND_CB     *pL2CA_LeConnectInd_Cb;
    tL2CA_LE_CONNECT_CFM_CB     *pL2CA_LeConnectCfm_Cb;
    tL2CA_DISCONNECT_IND_CB     *pL2CA_DisconnectInd_Cb;
    tL2CA_DISCONNECT_CFM_CB     *pL2CA_DisconnectCfm_Cb;
    tL2CA_DATA_IND_CB           *pL2CA_DataInd_Cb;
    tL2CA_CONGESTION_STATUS_CB  *pL2CA_CongestionStatus_Cb;
    tL2CA_LE_TX_COMPLETE_CB     *pL2CA_LeTxComplete_Cb;
#if (L2CAP_LE_COC_CONFORMANCE_TESTING == TRUE)
    tL2CA_LE_CONF_TEST_CB       *pL2CA_ConformTest_Cb;
#endif

} tL2CAP_LE_APPL_INFO;

#if (BLE_INCLUDED == TRUE)

/*******************************************************************************
**
**  Function        L2CA_CancelBleConnectReq
**
**  Description     Cancel a pending connection attempt to a BLE device.
**
**  Parameters:     BD Address of remote
**
**  Return value:   TRUE if connection was cancelled
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_CancelBleConnectReq(BD_ADDR rem_bda);

/*******************************************************************************
**
**  Function        L2CA_UpdateBleConnParams
**
**  Description     Update BLE connection parameters.
**
**  Parameters:     BD Address of remote
**
**  Return value:   TRUE if update started
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_UpdateBleConnParams(BD_ADDR rem_bdRa, UINT16 min_int, UINT16 max_int, UINT16 latency, UINT16 timeout);

/*******************************************************************************
**
**  Function        L2CA_EnableUpdateBleConnParams
**
**  Description     Update BLE connection parameters.
**
**  Parameters:     BD Address of remote
**                  enable flag
**
**  Return value:   TRUE if update started
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_EnableUpdateBleConnParams(BD_ADDR rem_bda, BOOLEAN enable);

/*******************************************************************************
**
** Function         L2CA_GetBleConnRole
**
** Description      This function returns the connection role.
**
** Returns          link role.
**
*******************************************************************************/
L2C_API extern UINT8 L2CA_GetBleConnRole(BD_ADDR bd_addr);

/*******************************************************************************
**
** Function         L2CA_GetDisconnectReason
**
** Description      This function returns the disconnect reason code.
**
** Returns          disconnect reason
**
*******************************************************************************/
L2C_API extern UINT16 L2CA_GetDisconnectReason(BD_ADDR remote_bda, tBT_TRANSPORT transport);

/*******************************************************************************
**
** Function         L2CA_LeRegister
**
** Description      Other layers call this function to register L2CAP services
**                  for LE_PSM.
**
**                  Options (see L2CAP_REG_OPTS_*)
**
** Returns          LE_PSM to use or zero if error. Typically the LE_PSM returned
**                  is the same as was passed in, but for an outgoing-only
**                  connection a "virtual" LE_PSM is returned  and should be used
**                  in the calls to L2CA_LeConnectReq() and L2CA_LeDeregister().
**
*******************************************************************************/
L2C_API extern UINT16 L2CA_LeRegister(UINT16 le_psm, tL2CAP_LE_APPL_INFO *p_cb_info, UINT8 reg_opts, void *context);

/*******************************************************************************
**
** Function         L2CA_LeDeregister
**
** Description      Other layers call this function to deregister L2CAP services
**                  for LE_PSM.
**
** Returns          void
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_LeDeregister(UINT16 le_psm);

/*******************************************************************************
**
** Function         L2CA_LeConnectReq
**
** Description      Higher layers call this function to create an L2CAP connection
**                  for LE_PSM.
**                  Note that the connection is not established at this time, but
**                  connection establishment gets started. The callback function
**                  will be invoked when connection establishes or fails.
**
** Returns          the CID of the connection, or 0 if it failed to start
**
*******************************************************************************/
L2C_API extern UINT16 L2CA_LeConnectReq(UINT16 le_psm, BD_ADDR p_bd_addr,
                                        tBLE_ADDR_TYPE bd_addr_type,
                                        tBLE_CONN_MODE conn_mode,
                                        UINT16 rx_mtu, UINT8 rx_sdu_pool_id,
                                        UINT8 req_security, UINT8 req_encr_key_size);

/*******************************************************************************
**
** Function         L2CA_LeConnectRsp
**
** Description      Higher layers call this function to accept an incoming
**                  LE L2CAP connection, for which they had gotten an connect
**                  indication callback.
**
** Returns          TRUE for success, FALSE for failure
**
*******************************************************************************/
L2C_API extern BOOLEAN  L2CA_LeConnectRsp(BD_ADDR p_bd_addr, UINT8 id, UINT16 lcid,
        UINT16 result, UINT16 rx_mtu, UINT8 rx_sdu_pool_id);

/*******************************************************************************
**
** Function         L2CA_LeDisconnectReq
**
** Description      Higher layers call this function to disconnect a LE COC
**                  channel.
**
** Returns          TRUE if disconnect sent, else FALSE
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_LeDisconnectReq(UINT16 lcid);

/*******************************************************************************
**
** Function         L2CA_LeDisconnectRsp
**
** Description      Higher layers call this function to acknowledge the
**                  disconnection of a LE COC channel.
**
** Returns          void
**
*******************************************************************************/
L2C_API extern BOOLEAN L2CA_LeDisconnectRsp(UINT16 lcid);

/*******************************************************************************
**
** Function         L2CA_LeDataWrite
**
** Description      Higher layers call this function to write data.
**
** Returns          L2CAP_DW_SUCCESS, if data accepted, else FALSE
**                  L2CAP_DW_CONGESTED, if data accepted and the channel is congested
**                  L2CAP_DW_FAILED, if error
**
*******************************************************************************/
L2C_API UINT8 L2CA_LeDataWrite(UINT16 cid, BT_HDR *p_data);

/*******************************************************************************
**
** Function         L2CA_LeSetUserCongestion
**
** Description      Higher layers call this function to tell if the connection
**                  is congested or not
**
** Returns          TRUE if command processed OK
**
*******************************************************************************/
L2C_API extern BOOLEAN  L2CA_LeSetUserCongestion(UINT16 lcid, BOOLEAN is_congested);

/*******************************************************************************
**
** Function         L2CA_LeGetPeerMTU
**
** Description      Higher layers call this function to get peer MTU.
**
** Returns          Peer MTU or 0.
**
*******************************************************************************/
L2C_API extern UINT16 L2CA_LeGetPeerMTU(UINT16 lcid);

/*******************************************************************************
**
** Function         L2CA_LeDetermSecurRsp
**
** Description      Higher layers call this function to check if the current
**                  device security settings are sufficient to continue with
**                  call establishment.
**                  It is called by call acceptor on reception of LE Credit
**                  Based Connection Request.
**
** Returns          L2CAP_CONN_OK/L2CAP_BLE_CONN_BAD_AUTHENT/
**                  L2CAP_BLE_CONN_BAD_KEY_SIZE/L2CAP_BLE_CONN_BAD_ENCRYPT/
**                  L2CAP_CONN_NO_RESOURCES.
**
*******************************************************************************/
L2C_API extern UINT16 L2CA_LeDetermSecurRsp(BD_ADDR bd_addr, UINT8 req_secur, UINT8 req_encr_key_size);

#endif /* (BLE_INCLUDED == TRUE) */

#ifdef __cplusplus
}
#endif

#endif  /* L2C_API_H */
