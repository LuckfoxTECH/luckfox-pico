/******************************************************************************
**
**  Name:          hfp_api.h
**
**
**  Description:   This file contains the Headset Profile API definitions.
**
**
**  Copyright (c) 1999-2004, WIDCOMM Inc., All Rights Reserved.
**  WIDCOMM Bluetooth Core. Proprietary and confidential.
******************************************************************************/
#ifndef HFP_API_H
#define HFP_API_H

#include "gap_api.h"
#include "hsp2_api.h"

/*****************************************************************************
**  Constants
*****************************************************************************/

/* Discoverability Modes */
#define HFP_GENERAL_DISCOVERABLE        BTM_GENERAL_DISCOVERABLE
#define HFP_NON_DISCOVERABLE            BTM_NON_DISCOVERABLE
#define HFP_DEFAULT_DISCOVERABLE        BTM_GENERAL_DISCOVERABLE
#define HFP_IGNORE_DISCOVERABLE         0x7E


/* Connectability Modes */
#define HFP_NON_CONNECTABLE             BTM_NON_CONNECTABLE
#define HFP_CONNECTABLE                 BTM_CONNECTABLE
#define HFP_DEFAULT_CONNECTABLE         BTM_CONNECTABLE


/* Pairability Modes */
#define HFP_DISALLOW_PAIRING            0
#define HFP_ALLOW_PAIRING               1
#define HFP_DEFAULT_PAIRING             HFP_ALLOW_PAIRING

/* Options set at HFP_EstablishConnection */
#define HFP_CONN_OPT_SKIP_INQ           HSP2_CONN_OPT_SKIP_INQ  /* AG/HF: Connection established w/o inquiring for peer */
#define HFP_CONN_OPT_USE_LTD_INQ        HSP2_CONN_OPT_USE_LTD_INQ  /* AG/HF: Use limited inquiry when finding devices */

#define HFP_AG_THREE_WAY_CALLS              0x0001
#define HFP_AG_EC_ANDOR_NR_FUNC             0x0002
#define HFP_AG_VOICE_REC_FUNC               0x0004
#define HFP_AG_INBAND_RING_TONE             0x0008
#define HFP_AG_ATT_PHNUM_VTAG               0x0010
#define HFP_AG_REJECT_CALL                  0x0020

#define HFP_HF_EC_ANDOR_NR_FUNC             0x0001
#define HFP_HF_CW_THREE_WAY_CALLS           0x0002
#define HFP_HF_CLI_CAPABILITY               0x0004
#define HFP_HF_VOICE_REC_ACT                0x0008
#define HFP_HF_REMOTE_VOL_CTRL              0x0010

/* Options set in RegInfo at HFP_Register */
#define HFP_REG_OPT_NO_AT_CMD_ACK       HSP2_REG_OPT_NO_AT_CMD_ACK  /* AG: Do not send 'OK' when AT command received */
#define HFP_REG_OPT_PARK_MODE           HSP2_REG_OPT_PARK_MODE  /* HF/AG: Enter park mode on disconnect */
#define HFP_CONN_OPT_WAIT_AT_CMD_RSP    HSP2_CONN_OPT_WAIT_AT_CMD_RSP  /* HF: Do not send AT command until receive responce from previous cmd */

#define HFP_REG_OPT_HF_RX_OK_EVT        0x0020  /* HF: To Receive HFP_APP_EVT_OK_RESULT event through the callback */
#define HFP_REG_OPT_SEND_CHLD_QUERY     0x0040  /* HF to send AT+CHLD=? at service level connection */
#define HFP_REG_OPT_AT_INTERFACE        HSP2_REG_OPT_AT_INTERFACE  /* Application is AT aware */
#define HFP_REG_OPT_NO_CALL_IND         0x0100  /* AG: don't send +CIEV call status indicator */

/* Supported Features Bitmask */
#define HFP_AG_3WAY_SUPPORTED 0x0001    /* 3-way calling */
#define HFP_AG_ECNR_SUPPORTED 0x0002    /* EC and/or NR function */
#define HFP_AG_VREC_SUPPORTED 0x0004    /* Voice Recognition function */
#define HFP_AG_INBAND_SUPPORTED 0x0008  /* Inband ring-tone */
#define HFP_AG_VTAG_SUPPORTED 0x0010    /* Attach a phone number to a voice tag */

#define HFP_HF_ECNR_SUPPORTED 0x0001    /* EC and/or NR function */
#define HFP_HF_CW_3WAY_SUPPORTED 0x0002 /* Call Waiting and 3 way calling */
#define HFP_HF_CLIP_SUPPORTED 0x0004    /* CLI presentation capability */
#define HFP_HF_VREC_SUPPORTED 0x0008    /* Voice Recognition activation */
#define HFP_HF_RVOL_SUPPORTED 0x0010    /* Remote Volume */

#define HFP_ERR_GRP 0xF00
#define HFP_MAX_NAME_LEN HSP2_MAX_NAME_LEN


/*****************************************************************************
**  Type Definitions
*****************************************************************************/

/* HFP role definitions */
enum
{
    HFP_ROLE_IS_AG,
    HFP_ROLE_IS_HF,
    HFP_ROLE_IS_HF15
};
typedef UINT8 tHFP_ROLE;

/* Application event codes (passed in callback functions) */
enum
{
    HFP_APP_EVT_RFC_OPEN,           /* RFCOMM port opened */
    HFP_APP_EVT_RFC_CLOSE,          /* RFCOMM port closed */

    HFP_APP_EVT_SCO_OPEN,           /* SCO channel opened */
    HFP_APP_EVT_SCO_CLOSE,          /* SCO channel closed */

    HFP_APP_EVT_OK_RESULT,          /* Received OK result code */
    HFP_APP_EVT_RING,               /* Received RING result code */
    HFP_APP_EVT_ERROR_RESULT,       /* Received ERROR result code */
    HFP_APP_EVT_CKPD,               /* Received AT+CKPD command */
    HFP_APP_EVT_VGM,                /* Received AT+VGM command */
    HFP_APP_EVT_VGS,                /* Received AT+VGS command */


    HFP_APP_EVT_AT_RSP_TOUT = HSP2_APP_EVT_AT_RSP_TOUT,        /* Response to AT command not received withing timeout */
    HFP_APP_EVT_SNT_ERROR = HSP2_APP_EVT_SNT_ERROR,          /* Sent an Error result code */

    HFP_APP_EVT_HFP_STATUS = HSP2_APP_EVT_HSP2_STATUS,       /* Report of internal status code (see below) */
    HFP_APP_EVT_DTMF_COMPLETE

};
typedef UINT16 tHFP_APP_CALLBACK_EVENT;

/* Application result codes */
enum
{
    HFP_SUCCESS = BT_PASS,      /* Success */
    HFP_FAILURE = HFP_ERR_GRP,  /* Unspecified failure. */

    HFP_NO_RESOURCES,           /* Unspecified failure to allocate resource */
    HFP_NO_MSG_RESOURCES,       /* Unable to allocate AT Command Message buffer */
    HFP_NO_GKI_RESOURCES,       /* Unable to allocate GKI Buffer */
    HFP_NO_QUEUE_RESOURCES,     /* Unable to allocate Event Queue Buffer */

    HFP_ERR_ALREADY_REGISTERED, /* Attempt to register failed: Already registered */
    HFP_ERR_NOT_REGISTERED,     /* Attempt to call API function failed: Not registered */

    HFP_ERR_PORT_OPEN,          /* Unspecified failure to open RFCOMM Port. */
    HFP_ERR_RFC_IS_CLOSED,      /* Attempt to close RFC failed: Port closed or closing. */
    HFP_ERR_RFC_IS_OPEN,        /* Attempt to open RFC failed: Port open or opening. */
    HFP_ERR_RFC_DATA_ERR,       /* Attempt to read RFC Data failed. */

    HFP_ERR_SCO_OPEN,           /* Unspecified failure to open SCO or enter SCO Listen mode */
    HFP_ERR_SCO_IS_OPEN,        /* Attempt to open SCO failed: Connection open or opening. */

    HFP_ERR_NO_MSG,             /* Attempt to send AT Command failed: Queue empty. */

    HFP_ERR_INQUIRY,            /* Unspecified failure to perform Inquiry */
    HFP_INQ_NO_DEVICES,         /* Inquiry found no devices */
    HFP_ERR_START_INQ,          /* Attempt to start Inquiry failed. */

    HFP_ERR_SECURITY_FAILED,    /* Attempt to set Security Level failed. */
    HFP_ERR_SET_DISC_MODE,      /* Attempt to set Discoverability Mode failed. */
    HFP_ERR_SET_CONN_MODE,      /* Attempt to set Connectability Mode failed. */

    HFP_ERR_DISCOVERY,          /* Attempt to perform Discovery failed. */
    HFP_ERR_SDP_REG,            /* Attempt to register SDP for Discovery failed. */
    HFP_ERR_SDP_PROTO,          /* Attempt to add Protocol List to SDP failed. */
    HFP_ERR_SDP_CLASSID,        /* Attempt to add Class ID to SDP failed */
    HFP_ERR_SDP_PROFILE,        /* Attempt to add Profile Descriptor list to SDP failed */
    HFP_ERR_SDP_ATTR,           /* Attempt to add Attribute to SDP failed */
    HFP_ERR_NO_SCN,             /* Peer discovery record did not contain SCN number */
    HFP_SERV_NOT_SUPPORTED,     /* Peer discovery failed to find required services */

    HFP_ERR_GAIN_OUT_OF_RANGE,  /* Attempt to set gain failed: Value outside of valid range */
    HFP_ERR_NO_GAIN_SUPPORT,    /* Attempt to set gain failed: Peer does not support
                                 * remote gain control */

    HFP_ERR_NO_BDADDR           /* Attempt to establish connection failed: No BD Address
                                 * was specified */

};
typedef UINT16 tHFP_STATUS;


enum
{
    HFP_AT_NO_QUERY,
    HFP_AT_CMD_QUERY,
    HFP_AT_VAL_QUERY
};
typedef UINT8 tHFP_AT_QUERY_TYPE;

/* Type of HFP application callback function */
typedef void (tHFP_CALLBACK)(tHFP_APP_CALLBACK_EVENT event, UINT32 data, void *pdata);

typedef tHSP2_REG_INFO tHFP_REG_INFO;
typedef tHSP2_PEER_INFO tHFP_PEER_INFO;

typedef struct
{
    UINT8  index_of_call;
    char   cind_info[HSP2_MAX_AT_VAL_LENGTH]; /* indicators supported in AG */
    char   chld_info[HSP2_MAX_AT_VAL_LENGTH]; /* call hold multi-party support in AG*/
    BOOLEAN network; /* Used to set the network attribute in SDP */
} tHFP_AG_INFO;

typedef struct
{
    char clip_info[HFP_MAX_CLIP_INFO];
    BOOLEAN inband_ring;
} tHFP_IND_INCALL_PARAMS;

enum
{
    HFP_CONNECT_CALL,
    HFP_AG_ANSWERED_CALL,
    HFP_AG_REJECTED_CALL,
    HFP_AG_CALL_HELD,       /* ME has put the call on hold as instructed from TE */
    HFP_AG_CALL_HELD_ACCEPTED, /* ME has connected to held call as instructed by TE */
    HFP_AG_CALL_HELD_UNAVLBL    /* The held call is no longer available to TE */
};
typedef UINT8 tHFP_IN_CALL_STATUS;

enum
{
    HFP_CALL_SETUP_FAILURE,
    HFP_REMOTE_PARTY_REACHED
};
typedef UINT8 tHFP_OUT_CALL_STATUS;

enum
{
    HFP_PHONE_NUM_SUPPLIED,
    HFP_MEMORY_DIALING,
    HFP_LAST_NUMBER_REDIAL
};
typedef UINT8 tHFP_PH_NUM_SRC;

enum
{
    HFP_CALL_WAITING,
    HFP_CALLER_ID,
    HFP_EC_NR_FUNC,
    HFP_VOICE_REC,
    HFP_INBAND_RING,
    HFP_REGISTRATION_IND, /* An indicator registered through CIND command */
    HFP_REG_IND_REPORTING
};
typedef UINT8 tHFP_FEATURE_CODE;


/*****************************************************************************
**  External Function Declarations
*****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************
**
**  Function        HFP_SetTraceLevel
**
**  Description     This function changes the trace level (debugging levels)
**
**  Returns         void
**
*****************************************************************************/
HFP_API extern void HFP_SetTraceLevel(UINT8 level);

/*******************************************************************************
**
** Function         HFP_Init
**
** Description      This function is called before accessing the HFP APIs in
**                  order to initialize the control block.
**
** Returns          void
**
*******************************************************************************/
HFP_API extern void HFP_Init(void);

/******************************************************************************
**
** Function      HFP_Register
**
** Description   This is the HFP API call used to register an audio gateway or
**               HFK.  This function must be called before any other HFP
**               API function is called (other than HFP_Init).
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_Register(tHFP_REG_INFO *pHFPRegInfo,
                                        tHFP_AG_INFO *inds);

/******************************************************************************
**
** Function      HFP_Deregister
**
** Description   This is the HFP API call used to deregister an audio gateway or
**               HFK.
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_Deregister(void);

/******************************************************************************
**
** Function      HFP_EstablishServLevelConn
**
** Description   This is the HFP API call used to establish a connection to a
**               peer HFK or audio gateway.
**
** Returns       tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_EstablishServLevelConn(BD_ADDR ServerBDAddr,
        UINT8 *ServerName,
        UINT16 Options);

/******************************************************************************
**
** Function      HFP_ReleaseServLevelConn
**
** Description   This is the HFP API call used to disconnect froma peer HFK
**               or audio gateway.
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_ReleaseServLevelConn(void);

/******************************************************************************
**
** Function      HFP_SetupAudioConn
**
** Description   This API initiates SCO connection with the peer.
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_SetupAudioConn(void);

/******************************************************************************
**
** Function      HFP_ReleaseAudioConn
**
** Description   This API releases SCO connection with the peer.
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_ReleaseAudioConn(void);

/******************************************************************************
**
** Function      HFP_SendATString
**
** Description   This API sends AT commands or response codes to the peer when
**               a service level connection has been established.
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_SendATString(BOOLEAN is_cmd, char at_cmd[]);

/******************************************************************************
**
** Function      HFP_SendMicGain
**
** Description   This is the HFP API call used to send the mic gain
**
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_SendMicGain(UINT8 MicGain);

/******************************************************************************
**
** Function      HFP_SendSpkGain
**
** Description   This is the HFP API call used to send the speaker gain
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_SendSpkGain(UINT8 SpkGain);

/******************************************************************************
**
** Function      HFP_GetPeerSupportedFeatures
**
** Description   This is the HFP API call used read the Connection Supported
**               Features of peer.
**
** Return Value  UINT16 - Supported Features Bit-Mask. (See descriptions above)
**
******************************************************************************/
HFP_API extern UINT16 HFP_GetPeerSupportedFeatures(void);

/******************************************************************************
**
** Function      HFP_SetPeerSupportedFeatures
**
** Description   This is the HFP API call used set the Supported Features
**               information for the peer.
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_SetPeerSupportedFeatures(UINT16 options);

/******************************************************************************
**
** Function      HFP_AG_IndicateInCall
**
** Description   This API, to be used by the AG application, indicates to the
**               hands-free kit presence of an incoming call. If in-band ring
**               option is chosen, and SCO hasn't been already established, it
**               initiates a SCO connection. Upon SCO connection establishment,
**               it sends RING and optionally +CLIP unsolicited result codes to
**               hands-free kit repeatedly.
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_AG_IndicateInCall(tHFP_IND_INCALL_PARAMS params);

/******************************************************************************
**
** Function      HFP_AG_ProceedInCall
**
** Description   This API, to be used by the AG application, indicates the status
**               of the incoming call (delivered using HFP_AG_IndicateInCall) to
**               the hands-free kit. This causes an appropriate +CIEV: unsolicited
**               response code to be sent to the hands free kit. If call is to be
**               connected, it initiates SCO connection (if not there already.)
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_AG_ProceedInCall(tHFP_IN_CALL_STATUS status);

/******************************************************************************
**
** Function      HFP_AG_ProceedOutCall
**
** Description   This API, to be used by the AG application, indicates the status
**               of the outgoing call (requested using HFP_HF_PlaceOutCall) to the
**               hands-free kit. This causes an appropriate +CIEV: unsolicited
**               response code to be sent to the hands free kit. If remote party
**               is reached, it initiates SCO connection (if not there already.)
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_AG_ProceedOutCall(tHFP_OUT_CALL_STATUS status);

/******************************************************************************
**
** Function      HFP_AG_TerminateCall
**
** Description   This API, to be used by the AG application, terminates an on-going
**               call with the hands-free kit. This causes an appropriate +CIEV:
**               unsolicited response code to be sent to the hands free kit.
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_AG_TerminateCall(void);

/******************************************************************************
**
** Function      HFP_AG_IndicatorStatus
**
** Description   This API, to be used by AG application, sends the current
**               status of Indicators to the Hands Free Kit.
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_AG_IndicatorStatus(char ind_status[]);

/******************************************************************************
**
** Function      HFP_AG_SendCmdStatus
**
** Description   This API, to be used by AG application, sends the status of the
**               commands issued by hands-free kit to enable or disable functions
**               such as Enabling of Indicator Status Update (CMER), Call Waiting
**               (CCWA), Caller Id (CLIP), EC and/or NR function (NREC), Voice
**               Recognition (BVRA) etc.
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_AG_SendCmdStatus(BOOLEAN status);

/******************************************************************************
**
** Function      HFP_AG_VrecReplyForPhoneNum
**
** Description   This API, to be used by AG application, sends the response to
**               the AT+BINP command from HFK (request phone number for the
**               procedure to attach number to a voice tag.).
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_AG_VrecReplyForPhoneNum(char ph_num[]);

/******************************************************************************
**
** Function      HFP_AG_SendCWNotification
**
** Description   This API, to be used by AG application, sends call waiting noti-
**               fication to the Hands Free Kit.
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_AG_SendCWNotification(char cw_info[]);

/******************************************************************************
**
** Function      HFP_AG_StatusUpdate
**
** Description   This API, to be used by AG application, informs HFK of the
**               changes status of functions such as Voice Recognition and In-band Ringing.
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_AG_StatusUpdate(tHFP_FEATURE_CODE code,
        UINT8 status, UINT8 reg_ind);

#if CCAP_COMPLIANCE==TRUE
/******************************************************************************
**
** Function      HFP_AG_ReplySubNumInfo
**
** Description   This API, to be used by AG application, sends the response to
**               the AT+CNUM=? from HF (to get subscriber info).
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_AG_ReplySubNumInfo(char sub_num[]);

/******************************************************************************
**
** Function      HFP_HF_HoldInCall
**
** Description   This API, to be used by the HF application, holds a call
**               indicated by RING(s) from AG. This causes the at command AT+BTRH
**               to peer.
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_HF_HoldInCall(void);

/******************************************************************************
**
** Function      HFP_HF_ProceedHeldInCall
**
** Description   This API, to be used by the HF application, proceeds on a held
**               incoming call This causes the at command AT+BTRH=1 or 2
**               to be sent to the AG.
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_HF_ProceedHeldInCall(BOOLEAN accept_flag);

/******************************************************************************
**
** Function      HFP_HF_GetSubNumInfo
**
** Description   This API can be used to access to the phone number of the ME
**               for Voice Calls Data Calls and Fax Calls. This results in
**               sending of AT+CNUM command.
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_HF_GetSubNumInfo(void);

#endif  /* CCAP_COMPLIANCE */


/******************************************************************************
**
** Function      HFP_HF_AnswerInCall
**
** Description   This API, to be used by the HF application, answers a call
**               indicated by RING(s) from AG. This causes the at command ATA
**               to be sent to the AG.
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_HF_AnswerInCall(void);

/******************************************************************************
**
** Function      HFP_HF_TerminateCall
**
** Description   This API, to be used by the HF application, terminates an on-
**               going call with the AG. This causes the command AT+CHUP to be
**               sent to the AG.
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_HF_TerminateCall(void);

/******************************************************************************
**
** Function      HFP_HF_PlaceOutCall
**
** Description   This API, to be used by HF application, initiates an outgoing call.
**               It sends an appropriate command ATD, ATD> or AT+BLDN to AG.
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_HF_PlaceOutCall(tHFP_PH_NUM_SRC src, char num[]);

/******************************************************************************
**
** Function      HFP_HF_ToggleFeature
**
** Description   This API, to be used by HF application, enables or disables
**               functions such as Call Waiting, Caller Id, EC and/or NR function,
**               and Voice Recognition.
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_HF_ToggleFeature(tHFP_FEATURE_CODE code,
        BOOLEAN enable_flag);

/******************************************************************************
**
** Function      HFP_HF_HookFlash
**
** Description   This API, to be used by HF application, indicates desire to place
**               a call on hold and go to the other call for the three-way calling
**               or call waiting functions.
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_HF_HookFlash(UINT8 call_num);

/******************************************************************************
**
** Function      HFP_HF_VrecReqForPhoneNum
**
** Description   This API, to be used by HF application, indicates desire to
**               place a voice tag to a phone number.
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_HF_VrecReqForPhoneNum(void);

/******************************************************************************
**
** Function      HFP_HF_TxDTMFCode
**
** Description   This API, to be used by HF application, transmits DTMF code.
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_HF_TxDTMFCode(char dtmf[]);

/******************************************************************************
**
** Function      HFP_HF_QueryCurrIndStatus
**
** Description   This API, to be used by HF application, sends AT+CIND? to query
**               current status of all indicators.
**
** Return Value  tHFP_STATUS    (See descriptions above)
**
******************************************************************************/
HFP_API extern tHFP_STATUS HFP_HF_QueryCurrIndStatus(void);

#ifdef __cplusplus
}
#endif

#endif /* HFP_API_H */
