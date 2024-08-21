/*****************************************************************************
**
**  Name:         sap_api.h
**
**  File:         SIM Access Profile Application Programming Interface
**
**  Copyright (c) 2003-2004, WIDCOMM Inc., All Rights Reserved.
**  WIDCOMM Bluetooth Core. Proprietary and confidential.
**
*****************************************************************************/
#ifndef SAP_API_H
#define SAP_API_H

#include "bt_target.h"

/*****************************************************************************
**  Constants and Types
*****************************************************************************/

/* tSAP_STATUS: Return value for API functions (if applicable) */
#define SAP_OK                          0
#define SAP_ERR_NO_RESOURCES            1   /* Out of resources */
#define SAP_ERR_SERVER_ALREADY_STARTED  2   /* Calling SAP_StartServer when server is already started */
#define SAP_ERR_SDP                     3   /* Problem added information to SDP service record */
#define SAP_ERR_SECURITY                4   /* Problem setting up security for SAP service */
#define SAP_ERR_GENERAL                 5   /* General SAP error */
#define SAP_ERR_BAD_HANDLE              6   /* Bad connection handle */
typedef UINT8 tSAP_STATUS;

/* tSAP_CONN_STATUS: Connection status for CONNECT_RESP message */
#define SAP_CONN_OK                     0   /* Connection can successfully be opened. */
#define SAP_CONN_ERR                    1   /* Unable to open connection. */
#define SAP_CONN_MSG_SIZE_UNSUPPORTED   2   /* Message size requested by the client is too big. Alternate message size will be sent in CONNECT_RESP. */
#define SAP_CONN_MSG_SIZE_TOO_SMALL     3   /* Message size requested by the client is too small. */
#define SAP_CONN_OK_ONGOING_CALL        4   /* OK, ongoing call */
typedef UINT8 tSAP_CONN_STATUS;

/* tSAP_DISCONNECT_TYPE: Disconnection type for DISCONNECT_IND message */
#define SAP_DISCONNECT_GRACEFUL         0   /* Server will wait send DISCCONECT_IND, and for client to send DISCONNECT_REQ message. */
#define SAP_DISCONNECT_IMMEDIATE        1   /* Server will close the connection after sending DISCONNECT_IND message. */
typedef UINT8 tSAP_DISCONNECT_TYPE;

/* tSAP_TRANS_PROCOTOL: Transport protocol used between client and server */
#define SAP_TRANS_PROCOTOL_T_0          0   /* Type 0 */
#define SAP_TRANS_PROCOTOL_T_1          1   /* Type 1 */
typedef UINT8 tSAP_TRANS_PROCOTOL;

/* tSAP_RESULT_CODE: Result codes for response messages */
#define SAP_RESULT_OK                   0   /* Request processed correctly. */
#define SAP_RESULT_ERROR                1   /* Error - no reason specified. */
#define SAP_RESULT_CARD_NOT_ACCESSIBLE  2   /* Error - card inserted but not accessible. */
#define SAP_RESULT_CARD_ALREADY_OFF     3   /* Error - card is already powered off. */
#define SAP_RESULT_CARD_REMOVED         4   /* Error - card is not inserted. */
#define SAP_RESULT_CARD_ALREADY_ON      5   /* Error - card is already powered on. */
#define SAP_RESULT_DATA_NOT_AVAILABLE   6   /* Error - data not available. */
#define SAP_RESULT_NOT_SUPPORTED        7   /* Error - not supported */
typedef UINT8 tSAP_RESULT_CODE;

/* tSAP_CARD_STATUS: SIM card status for STATUS_IND message */
#define SAP_CARD_UNKNOWN_ERROR  0   /* Unknown problem with SIM card. */
#define SAP_CARD_RESET          1   /* SIM inserted and powered on prior to SIM Access Profile connection. */
#define SAP_CARD_NOT_ACCESSIBLE 2   /* SIM inserted, but not accessible. */
#define SAP_CARD_REMOVED        3   /* SIM not inserted, or has been removed. */
#define SAP_CARD_INSERTED       4   /* SIM inserted, but not powered on. Client needs to power on the SIM before using it. */
#define SAP_CARD_RECOVERED      5   /* Previously not accessible card has been made accessible again, and is powered on by server. */
typedef UINT8 tSAP_CARD_STATUS;

/* tSAP_EVT: SIM Access callback event codes */
#define SAP_RFCOMM_OPEN_EVT             0x00    /* RFCOMM connection opened. */
#define SAP_RFCOMM_CLOSE_EVT            0x01    /* RFCOMM connection closed. */
#define SAP_CONNECT_REQ_EVT             0x02    /* CONNECT_REQ received. */
#define SAP_DISCONNECT_REQ_EVT          0x03    /* DISCONNECT_REQ received. */
#define SAP_TRANSFER_APDU_REQ_EVT       0x04    /* TRANSFER_APDU_REQ received. */
#define SAP_TRANSFER_ATR_REQ_EVT        0x05    /* TRANSFER_ATR_REQ received. */
#define SAP_POWER_SIM_ON_REQ_EVT        0x06    /* POWER_SIM_ON_REQ received. */
#define SAP_POWER_SIM_OFF_REQ_EVT       0x07    /* POWER_SIM_OFF_REQ received. */
#define SAP_RESET_SIM_REQ_EVT           0x08    /* RESET_SIM_REQ_EVT received. */
#define SAP_CARD_READER_STATUS_REQ_EVT  0x09    /* TRANSFER_CARD_READER_STATUS_REQ received. */
#define SAP_CONNECT_RESP_EVT            0x0A    /* CONNECT_RESP received. */
#define SAP_DISCONNECT_RESP_EVT         0x0B    /* DISCONNECT_RESP received. */
#define SAP_TRANSFER_APDU_RESP_EVT      0x0C    /* TRANSFER_APDU_RESP received. */
#define SAP_TRANSFER_ATR_RESP_EVT       0x0D    /* TRANSFER_ATR_RESP received. */
#define SAP_POWER_SIM_ON_RESP_EVT       0x0E    /* POWER_SIM_ON_RESP received. */
#define SAP_POWER_SIM_OFF_RESP_EVT      0x0F    /* POWER_SIM_OFF_RESP received. */
#define SAP_RESET_SIM_RESP_EVT          0x10    /* RESET_SIM_RESP received. */
#define SAP_CARD_READER_STATUS_RESP_EVT 0x11    /* TRANSFER_CARD_READER_STATUS_RESP received. */
#define SAP_STATUS_IND_EVT              0x12    /* STATUS_IND received. */
#define SAP_DISCONNECT_IND_EVT          0x13    /* DISCONNECT_IND received. */
#define SAP_ERROR_RESP_EVT              0x14    /* ERROR_RESP received. */
#define SAP_SET_TRANS_PROTOCOL_RESP_EVT 0x15    /* SET_TRANSPORT_PROTOCOL_RESP received */
typedef UINT8 tSAP_EVT;



/* Callback structures for notification of SIM Access server events */

/* Data for SAP_CONNECT_REQ_EVT */
typedef struct
{
    UINT16  msg_size;           /* Message size requested by client in CONNECT_REQ message */
} tSAP_CONNECT_REQ_DATA;

/* Data for SAP_TRANSFER_APDU_REQ_EVT */
typedef struct
{
    BT_HDR *p_data;
    BOOLEAN bIsApdu7816;
} tSAP_TRANSFER_APDU_REQ_DATA;

/* Data for SAP_RFCOMM_OPEN_EVT */
typedef struct
{
    tSAP_STATUS status;
    BD_ADDR     peer_bda;       /* Address of peer device */
} tSAP_RFCOMM_OPEN_DATA;

#if (defined(SAP_CLIENT_INCLUDED) && SAP_CLIENT_INCLUDED == TRUE)
/* Data for SAP_CONNECT_RESP_EVT */
typedef struct
{
    tSAP_CONN_STATUS    conn_status;    /* Connection status returned by server in CONNECT_RESP message */
    UINT16              max_msg_size;   /* Max Message size returned by server in CONNECT_RESP message */
} tSAP_CONNECT_RESP_DATA;

/* Data for SAP_STATUS_IND_EVT */
typedef struct
{
    tSAP_CARD_STATUS    status_change;  /* Indicate the reason for the change in the status of the subs module */
} tSAP_STATUS_IND;

/* Data for SAP_TRANSFER_ATR_RESP_EVT */
typedef struct
{
    tSAP_RESULT_CODE    result;         /* Indicate result code for the TRANSFER_ATR */
    void                *p_buf;         /* pointer to the start of the ATR data */
} tSAP_ATR_RESP;

/* Data for SAP_DISCONNECT_IND_EVT */
typedef struct
{
    tSAP_DISCONNECT_TYPE    type;       /* Disconnection type for DISCONNECT_IND message */
} tSAP_DISCONNECT_IND;

/* Data for SAP_POWER_SIM_OFF_RESP_EVT / SAP_POWER_SIM_ON_RESP_EVT */
typedef struct
{
    tSAP_RESULT_CODE    result;         /* Indicate result code for the POWER_SIM */
} tSAP_POWER_ON_OFF_RESP;

/* Data for SAP_RESET_SIM_RESP_EVT */
typedef struct
{
    tSAP_RESULT_CODE    result;         /* Indicate result code for the RESET_SIM */
} tSAP_RESET_SIM_RESP;

/* Data for SAP_CARD_READER_STATUS_RESP_EVT */
typedef struct
{
    tSAP_RESULT_CODE    result;         /* Indicate result code for the CARD_READER_STATUS */
    UINT8               reader_status;  /* Indicate card reader status */
} tSAP_READER_STATUS_RESP;

/* Data for SAP_TRANSFER_APDU_RESP_EVT */
typedef struct
{
    tSAP_RESULT_CODE    result;         /* Indicate result code for the TRANSFER_APDU */
    void                *p_buf;         /* pointer to the start of the APDU data */
} tSAP_APDU_RESP;

/* Data for SAP_SET_TRANS_PROTOCOL_RESP_EVT */
typedef struct
{
    tSAP_RESULT_CODE    result;         /* Indicate result code for the SET_TRANS_PROTOCOL */
} tSAP_SET_TRANS_PROTOCOL_RESP;

#endif



/* Union of server callback structures */
typedef union
{
    tSAP_CONNECT_REQ_DATA           con_req;            /* Data for SAP_CONNECT_REQ_EVT */
    tSAP_TRANSFER_APDU_REQ_DATA     apdu_req;           /* Data for SAP_TRANSFER_APDU_REQ_EVT */
    tSAP_RFCOMM_OPEN_DATA           rfc_open;           /* Data for SAP_RFCOMM_OPEN_EVT */

#if (defined(SAP_CLIENT_INCLUDED) && SAP_CLIENT_INCLUDED == TRUE)
    tSAP_CONNECT_RESP_DATA          con_resp;           /* Data for SAP_CONNECT_RESP_EVT */
    tSAP_STATUS_IND                 status_ind;         /* Data for SAP_STATUS_IND_EVT */
    tSAP_ATR_RESP                   atr_resp;           /* Data for SAP_TRANSFER_ATR_RESP_EVT */
    tSAP_DISCONNECT_IND             disconn_ind;        /* Data for SAP_DISCONNECT_IND_EVT */
    tSAP_POWER_ON_OFF_RESP          power_resp;         /* Data for SAP_POWER_SIM_OFF_RESP_EVT / SAP_POWER_SIM_ON_RESP_EVT*/
    tSAP_RESET_SIM_RESP             reset_sim;          /* Data for SAP_RESET_SIM_RESP_EVT */
    tSAP_READER_STATUS_RESP         reader_status;      /* Data for TRANSFER_CARD_READER_STATUS_RESP */
    tSAP_APDU_RESP                  apdu_resp;          /* Data for SAP_TRANSFER_APDU_RESP_EVT */
    tSAP_SET_TRANS_PROTOCOL_RESP    set_trans_protocol; /* Data for SAP_SET_TRANS_PROTOCOL_RESP_EVT */
#endif

} tSAP_EVT_DATA;

typedef void (tSAP_SERVER_CBACK)(tSAP_EVT event, tSAP_EVT_DATA *p_evt_data);

/* tSAP_SERVER_INFO: Server information for SAP_StartServer */
typedef struct
{
    tSAP_SERVER_CBACK   *p_server_cback;            /* Callback for event notification */
    UINT16              mtu;                        /* RFCOMM mtu size */
    UINT8               scn;                        /* SCN for SIM Access server */
    UINT8               security_mask;              /* Security options for SAP service */
    char                *p_service_name;            /* Service name for SDP record */
} tSAP_SERVER_INFO;


typedef void (tSAP_CLIENT_CBACK)(tSAP_EVT event, tSAP_EVT_DATA *p_evt_data);


/*****************************************************************************
**  External Function Declarations
*****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
**
** Function     SAP_Init
**
** Description  Initialize the SIM Access Profile sub-system.
**
** Returns      void.
**
*******************************************************************************/
BT_API void SAP_Init(void);


/*******************************************************************************
**
** Function     SAP_StartServer
**
** Description  Initialize the SIM Access server.
**
** Returns      tSAP_STATUS.
**
*******************************************************************************/
BT_API extern tSAP_STATUS SAP_StartServer(tSAP_SERVER_INFO *p_server_info);


/*******************************************************************************
**
** Function     SAP_StopServer
**
** Description  Take down the server.
**
** Returns      void.
**
*******************************************************************************/
BT_API extern void SAP_StopServer(void);


/*******************************************************************************
**
** Function     SAP_DisconnectInd
**
** Description  Stop an active client connection.
**
**              A DISCONNECT_IND message will be sent to the client. The
**              disconnection can be either immediate,or graceful.
**              If immediate, the server will close the connection after
**              sending DISCONNECT_IND;if graceful, the server will wait
**              for client to send DISCONNECT_REQ.
**
** Returns      void.
**
*******************************************************************************/
BT_API extern void SAP_DisconnectInd(tSAP_DISCONNECT_TYPE disconnect_type);


/*******************************************************************************
**
** Function     SAP_ConnectRsp
**
** Description  SAP_ConnectRsp is called to respond to a connection request
**              from the client. This function is called after receiving
**              a SAP_CONNECT_REQ_EVT.
**
** Returns      void.
**
*******************************************************************************/
BT_API extern void SAP_ConnectRsp(tSAP_CONN_STATUS conn_status, UINT16 message_size, tSAP_CARD_STATUS card_status);

/*******************************************************************************
**
** Function     SAP_DisconnectRsp
**
** Description  SAP_ConnectRsp is called to respond to a disconnection request
**              from the client. This function is called after receiving
**              a SAP_DISCONNECT_REQ_EVT.
**
** Returns      void.
**
*******************************************************************************/
BT_API extern void SAP_DisconnectRsp(void);

/*******************************************************************************
**
** Function     SAP_TransferApduRsp
**
** Description  Send response for transfer APDU request from the client.
**
**              This API function is called after receiving a SAP_TRANFER_APDU_REQ_EVT.
**              The input buffer must be obtained using SAP_GetMsgBuf. The start
**              of the APDU response must be at the offset specified in the BT_HDR.
**              Buffer will be freed by SAP_TransferApduRsp.
**
** Returns      void.
**
*******************************************************************************/
BT_API extern void SAP_TransferApduRsp(tSAP_RESULT_CODE result, BT_HDR *p_rspbuf);

/*******************************************************************************
**
** Function     SAP_TransferApduReq
**
** Description  Send transfer APDU request from the client.
**
**              This API function is called when application want to send APDU request.
**              The input buffer must be obtained using SAP_GetMsgBuf. The start
**              of the APDU request must be at the offset specified in the BT_HDR.
**              Buffer will be freed by SAP_TransferApduReq.
**
** Returns      void.
**
*******************************************************************************/
BT_API extern void SAP_TransferApduReq(BT_HDR *p_reqbuf, BOOLEAN is_apdu_7816);


/*******************************************************************************
**
** Function     SAP_TransferAtrRsp
**
** Description  Send response for transfer ATR request from the client.
**
**              This API function is called after receiving a SAP_TRANFER_ATR_REQ_EVT.
**              The input buffer must be obtained using SAP_GetMsgBuf. The start
**              of the ATR response must be at the offset specified in the BT_HDR.
**              Buffer will be freed by SAP_TransferAtrRsp.
**
** Returns      void.
**
*******************************************************************************/
BT_API extern void SAP_TransferAtrRsp(tSAP_RESULT_CODE result, BT_HDR *p_rspbuf);


/*******************************************************************************
**
** Function     SAP_PowerSimOnRsp
**
** Description  Send POWER_SIM_ON_RESP message to client.
**
**              This API function is called after receiving a SAP_POWER_SIM_ON_REQ_EVT.
**
** Returns      void.
**
*******************************************************************************/
BT_API extern void SAP_PowerSimOnRsp(tSAP_RESULT_CODE result);


/*******************************************************************************
**
** Function     SAP_PowerSimOffRsp
**
** Description  Send POWER_SIM_OFF_RESP message to client.
**
**              This API function is called after receiving a SAP_POWER_SIM_OFF_REQ_EVT.
**
** Returns      void.
**
*******************************************************************************/
BT_API extern void SAP_PowerSimOffRsp(tSAP_RESULT_CODE result);


/*******************************************************************************
**
** Function     SAP_ResetSimRsp
**
** Description  Send RESET_SIM_RESP message to client.
**
**              This API function is called after receiving a SAP_RESET_SIM_RESP_EVT.
**
** Returns      void.
**
*******************************************************************************/
BT_API extern void SAP_ResetSimRsp(tSAP_RESULT_CODE result);


/*******************************************************************************
**
** Function     SAP_CardReaderStatusRsp
**
** Description  Send TRANSFER_CARD_READER_STATUS_RESP from the client.
**
**              This API function is called after receiving a
**              TRANSFER_CARD_READER_STATUS_REQ.
**
** Returns      void.
**
*******************************************************************************/
BT_API extern void SAP_CardReaderStatusRsp(tSAP_RESULT_CODE result, UINT8 reader_status);


/*******************************************************************************
**
** Function     SAP_StatusInd
**
** Description  Inform the client of a change in the SIM card status.
**
**              If a client connection is active, a STATUS_IND message will be sent.
**
** Returns      void.
**
*******************************************************************************/
BT_API extern void SAP_StatusInd(tSAP_CARD_STATUS card_status);


/*******************************************************************************
**
** Function     SAP_GetMsgBuf
**
** Description  Obtain a message buffer for use with SAP_TransferApduRsp or
**              SAP_TransferAtrRsp. The offset specified in BT_HDR indicates
**              the location where the message is to be copied.
**
** Returns      If successful, pointer to BT_HDR buffer;
**              otherwise NULL.
**
*******************************************************************************/
BT_API extern BT_HDR *SAP_GetMsgBuf(UINT16 msg_size);


/******************************************************************************
**
** Function         SAP_AddRecord
**
** Description      This function is called by a server application to add
**                  SAP server information to an SDP record.  Prior to
**                  calling this function the application must call
**                  SDP_CreateRecord() to create an SDP record.
**
**                  Input Parameters:
**                      p_service_name:  Pointer to a null-terminated character
**                          string containing the service name. (May be NULL)
**
**                      scn: server channel number of the RFCOMM connection.
**
**                      sdp_handle:  SDP handle returned by SDP_CreateRecord().
**
**                  Output Parameters:
**                      None.
**
** Returns          SAP_OK      Successful
**                  SAP_ERR_SDP Not successful
**
******************************************************************************/
BT_API extern tSAP_STATUS SAP_AddRecord(char *p_service_name, UINT8 scn, UINT32 sdp_handle);

/*****************************************************************************
**  Client APIs
*****************************************************************************/
#if (defined(SAP_CLIENT_INCLUDED) && SAP_CLIENT_INCLUDED == TRUE)

/*******************************************************************************
**
** Function     SAC_OpenRfcomm
**
** Description  SAC_OpenRfcomm is called to open a RFCOMM connection for the SAP
**              to the server.
**
** Returns      tSAP_STATUS.
**
*******************************************************************************/
BT_API extern tSAP_STATUS SAC_OpenRfcomm(BD_ADDR bd_addr, UINT8 sec_mask, UINT8 scn,
        UINT16 mtu, tSAP_CLIENT_CBACK *p_cback,
        UINT16 *p_handle);

/*******************************************************************************
**
** Function     SAC_CloseRfcomm
**
** Description  SAC_CloseRfcomm is called to close a RFCOMM connection for the SAP
**              to the server.
**
** Returns      tSAP_STATUS.
**
*******************************************************************************/
BT_API extern tSAP_STATUS SAC_CloseRfcomm(UINT16 handle);

/*******************************************************************************
**
** Function     SAC_ConnectReq
**
** Description  SAC_ConnectReq is called to send a connection request
**              to the server.
**
** Returns      tSAP_STATUS.
**
*******************************************************************************/
BT_API extern tSAP_STATUS SAC_ConnectReq(UINT16 conn_handle, UINT16 max_msg_size);

/*******************************************************************************
**
** Function     SAC_DisConnectReq
**
** Description  SAC_DisConnectReq is called to send a disconnection request
**              to the server.
**
** Returns      tSAP_STATUS.
**
*******************************************************************************/
BT_API extern tSAP_STATUS SAC_DisConnectReq(UINT16 conn_handle);

/*******************************************************************************
**
** Function     SAC_TransferAtrReq
**
** Description  SAC_TransferAtrReq is called to send a ATR request
**              to the server.
**
** Returns      tSAP_STATUS.
**
*******************************************************************************/
BT_API extern tSAP_STATUS SAC_TransferAtrReq(UINT16 conn_handle);

/*******************************************************************************
**
** Function     SAC_PowerSimReq
**
** Description  SAC_PowerSimReq is called to send a POWER_SIM_ON/OFF request
**              to the server.
**
** Returns      tSAP_STATUS.
**
*******************************************************************************/
BT_API extern tSAP_STATUS SAC_PowerSimReq(UINT16 conn_handle, BOOLEAN is_on);

/*******************************************************************************
**
** Function     SAC_ResetSimReq
**
** Description  SAC_ResetSimReq is called to send a RESET_SIM request
**              to the server.
**
** Returns      tSAP_STATUS.
**
*******************************************************************************/
BT_API extern tSAP_STATUS SAC_ResetSimReq(UINT16 conn_handle);

/*******************************************************************************
**
** Function     SAC_TransReaderStatusReq
**
** Description  SAC_TransReaderStatusReq is called to send a
**              TRANSPORT_CARD_READER_STATUS request
**              to the server.
**
** Returns      tSAP_STATUS.
**
*******************************************************************************/
BT_API extern tSAP_STATUS SAC_TransReaderStatusReq(UINT16 conn_handle);

/*******************************************************************************
**
** Function     SAC_TransferApduReq
**
** Description  SAC_TransferApduReq is called to send a
**              TRANSFER_APDU request
**              to the server.
**
** Returns      tSAP_STATUS.
**
*******************************************************************************/
BT_API extern tSAP_STATUS SAC_TransferApduReq(UINT16 conn_handle, BT_HDR *p_buf);

/*******************************************************************************
**
** Function     SAC_SetTransportProtocolReq
**
** Description  SAC_SetTransportProtocolReq is called to send a
**              SET_TRANSPORT_PROTOCOL request
**              to the server.
**
** Returns      tSAP_STATUS.
**
*******************************************************************************/
BT_API extern tSAP_STATUS SAC_SetTransportProtocolReq(UINT16 conn_handle,
        tSAP_TRANS_PROCOTOL protocol);

#endif /* SAP_CLIENT_INCLUDED */

#ifdef __cplusplus
}
#endif

#endif /* SAP_API_H */
