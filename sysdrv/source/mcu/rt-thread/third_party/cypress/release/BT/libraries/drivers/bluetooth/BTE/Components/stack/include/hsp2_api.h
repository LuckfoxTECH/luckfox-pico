/******************************************************************************
**
**  Name:          hsp2_api.h
**
**
**  Description:   This file contains the Headset Profile API definitions.
**
**
**  Copyright (c) 1999-2004, WIDCOMM Inc., All Rights Reserved.
**  WIDCOMM Bluetooth Core. Proprietary and confidential.
******************************************************************************/
#ifndef HSP2_API_H
#define HSP2_API_H

#if GAP_INCLUDED == TRUE
#include "gap_api.h"
#else
#include "btm_api.h"
#endif

/*****************************************************************************
**  Constants
*****************************************************************************/

/* Discoverability Modes */
#define HSP2_GENERAL_DISCOVERABLE        BTM_GENERAL_DISCOVERABLE
#define HSP2_NON_DISCOVERABLE            BTM_NON_DISCOVERABLE
#define HSP2_DEFAULT_DISCOVERABLE        BTM_GENERAL_DISCOVERABLE
#define HSP2_IGNORE_DISCOVERABLE         0x7E


/* Connectability Modes */
#define HSP2_NON_CONNECTABLE             BTM_NON_CONNECTABLE
#define HSP2_CONNECTABLE                 BTM_CONNECTABLE
#define HSP2_DEFAULT_CONNECTABLE         BTM_CONNECTABLE


/* Pairability Modes */
#define HSP2_DISALLOW_PAIRING            0
#define HSP2_ALLOW_PAIRING               1
#define HSP2_DEFAULT_PAIRING             HSP2_ALLOW_PAIRING


/* Security Modes */
#define HSP2_SEC_MODE_NONE               BTM_SEC_MODE_NONE
#define HSP2_SEC_MODE_SERVICE            BTM_SEC_MODE_SERVICE
#define HSP2_SEC_MODE_LINK               BTM_SEC_MODE_LINK
#define HSP2_DEFAULT_SECURITY            BTM_SEC_MODE_NONE


/* Options set at HSP2_EstablishConnection */
#define HSP2_CONN_OPT_AUTOANSWER         0x0001  /* AG: Do not ring HS when establishing connection */
#define HSP2_CONN_OPT_INBAND_RING        0x0002  /* AG: Open audio connection for inband ringing */
#define HSP2_CONN_OPT_SKIP_INQ           0x0004  /* AG/HS: Connection established w/o inquiring for peer */
#define HSP2_CONN_OPT_AUTO_CKPD          0x0010  /* HS: Send CKPD automatically on RING */
#define HSP2_CONN_OPT_FORCE_RGC          0x0020  /* AG/HS: Send VGM/VGS regardles of peer SDP database */
#define HSP2_CONN_OPT_USE_LTD_INQ        0x0040  /* AG/HS: Use limited inquiry when finding devices */

/* Options set in RegInfo at HSP2_Register */
#define HSP2_REG_OPT_REMOTE_GAIN_CTRL    0x0001  /* HS: Device supports remote gain control */
#define HSP2_REG_OPT_NO_AT_CMD_ACK       0x0002  /* AG: Do not send 'OK' when AT command received */
#define HSP2_REG_OPT_NO_CKPD_TO_CONNECT  0x0004  /* HS: Do not send CKPD when initiating call */
/* AG: Open SCO after ACL link opens */
#define HSP2_REG_OPT_PARK_MODE           0x0008  /* HS/AG: Enter park mode on disconnect */
#define HSP2_CONN_OPT_WAIT_AT_CMD_RSP    0x0010  /* HS: Do not send AT command until receive responce from previous cmd */
#define HSP2_REG_OPT_AT_INTERFACE        0x0080  /* Application is AT aware */
#define HSP2_REG_OPT_NO_AUTO_SCO         0x0020  /* Application will control the SCO connection establishment */

/* HSP internal status codes (base) */
#define HSP2_ERR_GRP 0xF00

/*****************************************************************************
**  Type Definitions
*****************************************************************************/

/* HSP role definitions */
enum
{
    HSP2_ROLE_IS_AG,
    HSP2_ROLE_IS_HS,
    HSP2_ROLE_IS_HS15
};
typedef UINT8 tHSP2_ROLE;


enum
{
    HSP2_AT_NO_QUERY,
    HSP2_AT_CMD_QUERY,
    HSP2_AT_VAL_QUERY
};
typedef UINT8 tHSP2_AT_QUERY_TYPE;


/* HSP application callback event codes */
enum
{
    HSP2_APP_EVT_RFC_OPEN,           /* RFCOMM port opened */
    HSP2_APP_EVT_RFC_CLOSE,          /* RFCOMM port closed */

    HSP2_APP_EVT_SCO_OPEN,           /* SCO channel opened */
    HSP2_APP_EVT_SCO_CLOSE,          /* SCO channel closed */

    /* The order of the application events for AT commands and Result Codes
     * must match the order of the HSP AT command and Result Code indexes into
     * the HSP Message Table (hsp2_msg_txt_tbl). This allows HSP2_MSG_IDX_TO_APP_EVT
     * to easily convert a message index into an application event
     */

    HSP2_APP_EVT_OK_RESULT,          /* Received OK result code */
    HSP2_APP_EVT_RING,               /* Received RING result code */
    HSP2_APP_EVT_ERROR_RESULT,       /* Received ERROR result code */
    HSP2_APP_EVT_CKPD,               /* Received AT+CKPD command */
    HSP2_APP_EVT_VGM,                /* Received AT+VGM command */
    HSP2_APP_EVT_VGS,                /* Received AT+VGS command */

#if (HFP_INCLUDED == TRUE)
    HFP_APP_EVT_A,
    HFP_APP_EVT_D,
    HFP_APP_EVT_CCWA,
    HFP_APP_EVT_CHLD,
    HFP_APP_EVT_CHUP,
    HFP_APP_EVT_CIND,
    HFP_APP_EVT_CLIP,
    HFP_APP_EVT_CMER,
    HFP_APP_EVT_CIEV,
    HFP_APP_EVT_VTS,
    HFP_APP_EVT_BINP,
    HFP_APP_EVT_BLDN,
    HFP_APP_EVT_BVRA,
    HFP_APP_EVT_NREC,
    HFP_APP_EVT_BSIR,
    HFP_APP_EVT_BRSF,
    HFP_APP_EVT_CMEE,
    HFP_APP_EVT_CLCC,
#endif

#if (HFP_INCLUDED == TRUE) && (CCAP_COMPLIANCE == TRUE)
    HFP_APP_EVT_BTRH,
    HFP_APP_EVT_CNUM,
#endif

    HSP2_AT_STRING,
    HFP_AT_STRING = HSP2_AT_STRING,

    HSP2_APP_EVT_AT_RSP_TOUT,        /* Response to AT command not received withing timeout */
    HSP2_APP_EVT_SNT_ERROR,          /* Sent an Error result code */

#if (HFP_INCLUDED == TRUE) && (HFP_RPT_PEER_INFO_INCLUDED == TRUE)
    HFP_APP_EVT_PEER_INFO,
#endif
    HSP2_APP_EVT_HSP2_STATUS          /* Report of HSP internal status code (see below) */

};
typedef UINT16 tHSP2_APP_CALLBACK_EVENT;

/* Macro to convert an AT Command/Result Code index into the hsp2_msg_txt_tbl
 * into an application callback event code
 */
#define HSP2_MSG_IDX_TO_APP_EVT(index)   ( HSP2_APP_EVT_OK_RESULT + (index) );

enum
{
    HSP2_SUCCESS = BTM_SUCCESS,   /* Success */
    HSP2_FAILURE = HSP2_ERR_GRP,  /* Unspecified failure. */

    HSP2_NO_RESOURCES,           /* Unspecified failure to allocate resource */
    HSP2_NO_MSG_RESOURCES,       /* Unable to allocate AT Command Message buffer */
    HSP2_NO_GKI_RESOURCES,       /* Unable to allocate GKI Buffer */
    HSP2_NO_QUEUE_RESOURCES,     /* Unable to allocate HSP Event Queue Buffer */

    HSP2_ERR_ALREADY_REGISTERED, /* Attempt to register HSP failed: Already registered */
    HSP2_ERR_NOT_REGISTERED,     /* Attempt to call API function failed: Not registered */

    HSP2_ERR_PORT,               /* Unspecified failure to open RFCOMM Port. */
    HSP2_ERR_RFC_IS_CLOSED,      /* Attempt to close RFC failed: Port closed or closing. */
    HSP2_ERR_RFC_IS_OPEN,        /* Attempt to open RFC failed: Port open or opening. */
    HSP2_ERR_RFC_DATA_ERR,       /* Attempt to read RFC Data failed. */

    HSP2_ERR_SCO_OPEN,           /* Unspecified failure to open SCO or enter SCO Listen mode */
    HSP2_ERR_SCO_IS_OPEN,        /* Attempt to open SCO failed: Connection open or opening. */

    HSP2_ERR_NO_MSG,             /* Attempt to send AT Command failed: Queue empty. */

    HSP2_ERR_INQUIRY,            /* Unspecified failure to perform Inquiry */
    HSP2_INQ_NO_DEVICES,         /* Inquiry found no devices */
    HSP2_ERR_START_INQ,          /* Attempt to start Inquiry failed. */

    HSP2_ERR_SECURITY_FAILED,    /* Attempt to set Security Level failed. */
    HSP2_ERR_SET_DISC_MODE,      /* Attempt to set Discoverability Mode failed. */
    HSP2_ERR_SET_CONN_MODE,      /* Attempt to set Connectability Mode failed. */

    HSP2_ERR_DISCOVERY,          /* Attempt to perform Discovery failed. */
    HSP2_ERR_SDP_REG,            /* Attempt to register SDP for Discovery failed. */
    HSP2_ERR_SDP_PROTO,          /* Attempt to add Protocol List to SDP failed. */
    HSP2_ERR_SDP_CLASSID,        /* Attempt to add Class ID to SDP failed */
    HSP2_ERR_SDP_PROFILE,        /* Attempt to add Profile Descriptor list to SDP failed */
    HSP2_ERR_SDP_ATTR,           /* Attempt to add Attribute to SDP failed */
    HSP2_ERR_NO_SCN,             /* Peer discovery record did not contain SCN number */
    HSP2_SERV_NOT_SUPPORTED,     /* Peer discovery failed to find required services */

    HSP2_ERR_GAIN_OUT_OF_RANGE,  /* Attempt to set gain failed: Value outside of valid range */
    HSP2_ERR_NO_GAIN_SUPPORT,    /* Attempt to set gain failed: Peer does not support
                                 * remote gain control */

    HSP2_ERR_NO_BDADDR           /* Attempt to establish connection failed: No BD Address
                                 * was specified */

};
typedef UINT16 tHSP2_STATUS;


/* Type of HSP application callback function */
typedef void (tHSP2_CALLBACK)(tHSP2_APP_CALLBACK_EVENT event, UINT32 data, void *pdata);

/* Structure used to pass information for HSP2_Register */
typedef struct
{
    tHSP2_ROLE  Role;                        /* HSP2_ROLE_IS_AG or HSP2_ROLE_IS_HS or HSP2_ROLE_IS_HS15 */

    UINT16  DiscoveryMode;                  /* Discovery Mode of device (see options above) */
    UINT8   SecurityMode;                   /* Security Mode of device (see options above) */
    UINT8   SecurityLevel;                  /* Security Level - Required Combination of BTM_SEC_IN_AUTHORIZE |
                                               BTM_SEC_IN_AUTHENTICATE | BTM_SEC_IN_ENCRYPT*/
    UINT16  ConnectibilityMode;             /* Connectibility of device (see options above) */
    BOOLEAN PairingMode;                    /* Pairing Mode of device (see options above) */
    BOOLEAN AllowOnlyPaired;                /* True if device allows only paired deviced to connect */

    UINT8   service_name[HSP2_MAX_NAME_LEN + 1]; /* ServiceName to present during Service Discovery */

    UINT16  options;                        /* HSP2_SUPPORT_REMOTE_GAIN_CTRL | HSP2_DOES_NOT_ACKNOWLEDGE_AT_COMMANDS */
    UINT16  VoiceSettings;                  /* Indicates what speech settings to use (format of samples, coding scheme, etc.) */
    UINT16  SupportedFeatures;              /* Supported Features bitmask */

    tHSP2_CALLBACK   *application_cback;     /* Callback function for notifying application of events */
} tHSP2_REG_INFO;

/* Peer device information */
typedef struct
{
    BD_ADDR     bdaddr;
    char        name[HSP2_MAX_NAME_LEN + 1];
    UINT8       scn;
    UINT16      SupportedFeatures; /* Used for SupportsRemoteAudioCtrl in case of HSP2 */
    BOOLEAN     network;
} tHSP2_PEER_INFO;

/*****************************************************************************
**  External Function Declarations
*****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
**
** Function      HSP2_Register
**
** Description   This is the HSP API call used to register an audio gateway or
**               headset.  This function must be called after HSP2_Init, but
**               before any other HSP API function is called.
**
** Return Value  tHSP2_STATUS    (See descriptions above)
**
******************************************************************************/
HSP2_API extern tHSP2_STATUS HSP2_Register(tHSP2_REG_INFO *pHspRegInfo);

/******************************************************************************
**
** Function      HSP2_Deregister
**
** Description   This is the HSP API call used to deregister an audio gateway or
**               headset.
**
** Return Value  tHSP2_STATUS    (See descriptions above)
**
******************************************************************************/
HSP2_API extern tHSP2_STATUS HSP2_Deregister(void);

/******************************************************************************
**
** Function      HSP2_EstablishConnection
**
** Description   This is the HSP API call used to establish a connection to a
**               peer headset or audio gateway.
**
** Return Value  tHSP2_STATUS    (See descriptions above)
**
******************************************************************************/
HSP2_API extern tHSP2_STATUS HSP2_EstablishConnection(BD_ADDR ServerBDAddr,
        UINT8 *ServerName,
        UINT16 Options);

/******************************************************************************
**
** Function      HSP2_ReleaseConnection
**
** Description   This is the HSP API call used to disconnect from a peer headset
**               or audio gateway.
**
** Return Value  tHSP2_STATUS    (See descriptions above)
**
******************************************************************************/
HSP2_API extern tHSP2_STATUS HSP2_ReleaseConnection(void);

/******************************************************************************
**
** Function      HSP2_SetupAudioConn
**
** Description   This API initiates SCO connection with the peer.
**
** Return Value  tHSP2_STATUS
**
******************************************************************************/
HSP2_API extern tHSP2_STATUS HSP2_SetupAudioConn(void);

/******************************************************************************
**
** Function      HSP2_ReleaseAudioConn
**
** Description   This API releases SCO connection with the peer.
**
** Return Value  tHSP2_STATUS
**
******************************************************************************/
HSP2_API extern tHSP2_STATUS HSP2_ReleaseAudioConn(void);


/******************************************************************************
**
** Function      HSP2_ReleaseServiceConn
**
** Description   This API releases SCO and RFC connections with the peer.
**
** Return Value  tHSP2_STATUS
**
******************************************************************************/
HSP2_API extern tHSP2_STATUS HSP2_ReleaseServiceConn(void);

/******************************************************************************
**
** Function      HSP2_SendATString
**
** Description   This is the HSP API call used to send any AT string
**
** Return Value  tHSP2_STATUS    (See descriptions above)
**
******************************************************************************/
HSP2_API extern tHSP2_STATUS HSP2_SendATString(BOOLEAN is_cmd, char at_cmd[]);

/******************************************************************************
**
** Function      HSP2_SendKeypress
**
** Description   This is the HSP API call used to send a keypress (AT+CKPD) to
**               an audio gateway.
**
** Return Value  tHSP2_STATUS    (See descriptions above)
**
******************************************************************************/
HSP2_API extern tHSP2_STATUS HSP2_SendKeypress(UINT16 Keypress);

/******************************************************************************
**
** Function      HSP2_SendSpkGain
**
** Description   This is the HSP API call used to send the speaker gain (AT+VGS)
**               to an audio gateway.
**
** Return Value  tHSP2_STATUS    (See descriptions above)
**
******************************************************************************/
HSP2_API extern tHSP2_STATUS HSP2_SendSpkGain(UINT8 SpkGain);

/******************************************************************************
**
** Function      HSP2_SendMicGain
**
** Description   This is the HSP API call used to send the mic gain (AT+VGM)
**               to an audio gateway.
**
** Return Value  tHSP2_STATUS    (See descriptions above)
**
******************************************************************************/
HSP2_API extern tHSP2_STATUS HSP2_SendMicGain(UINT8 MicGain);

/******************************************************************************
**
** Function      HSP2_SetConnectionOptions
**
** Description   This is the HSP API call used set the Connection Run-Time
**               options after a connection has been established.
**
** Return Value  tHSP2_STATUS    (See descriptions above)
**
******************************************************************************/
HSP2_API extern tHSP2_STATUS HSP2_SetConnectionOptions(UINT16 options);

/******************************************************************************
**
** Function      HSP2_ReadConnectionOptions
**
** Description   This is the HSP API call used read the Connection Run-Time
**               options.
**
** Return Value  UINT16 - Connection options. (See descriptions above)
**
******************************************************************************/
HSP2_API extern UINT16 HSP2_ReadConnectionOptions(void);

/*****************************************************************************
**
**  Function:    HSP2_SetTraceLevel
**
**  Purpose:     This function changes the trace level for debugging
**
**  Returns:     void
**
*****************************************************************************/
HSP2_API extern void HSP2_SetTraceLevel(UINT8 level);

/*******************************************************************************
**
** Function         HSP2_Init
**
** Description      This function is called before accessing the HSP2 APIs in
**                  order to initialize the control block.
**
** Returns          void
**
*******************************************************************************/
HSP2_API extern void HSP2_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* HSP2_API_H */
