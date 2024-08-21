/*****************************************************************************
**
**  Name:          btm_cbl_api.h
**
**  Description:   this file contains the main Bluetooth Manager (BTM)
**                 definitions for ConnectionLess Broadcast.
**
**  Copyright (c) 2012, Broadcom Corp., All Rights Reserved
**  Broadcom Bluetooth Core. Proprietary and confidential.
******************************************************************************/
#ifndef BTM_CLB_API_H
#define BTM_CLB_API_H

/*
 * Definitions
 */
/* CLB Events */
#define BTM_CLB_REGISTER_EVT            1
#define BTM_CLB_DEREGISTER_EVT          2
#define BTM_CLB_TX_START_EVT            3
#define BTM_CLB_TX_STOP_EVT             4
#define BTM_CLB_RX_START_EVT            5
#define BTM_CLB_RX_STOP_EVT             6
#define BTM_CLB_RX_TIMEOUT_EVT          7
#define BTM_CLB_RX_DATA_EVT             8
#define BTM_CLB_STREAM_TX_CFM_EVT       9    /* CLB Stream Tx cfm (result of BTM_ClbWrite on a Tx Stream channel) */
#define BTM_CLB_TRUNC_PAGE_RCV_EVT      10   /* A peer device performed Truncated Page (Sync Train Req) */
typedef UINT8 tBTM_CLB_EVT;

/* 4 ConnectionLess Channels by default (3DS Tx, 3DS Rx, AV1, AV2) */
#ifndef BTM_CLB_CHANNEL_MAX
#define BTM_CLB_CHANNEL_MAX             4
#endif

/* CLB Registration modes */
#define BTM_CLB_MODE_TX                 0x01    /* Lt_addr will be reserved in the local BT Controller*/
#define BTM_CLB_MODE_RX                 0x02    /* */
#define BTM_CLB_MODE_STREAM             0x04    /* */
typedef UINT8 tBTM_CLB_MODE;

/* ConnectionLess Broadcast Handle used to distinguish channels */
#define BTM_CLB_HANDLE_BAD              0xFF
typedef UINT8 tBTM_CLB_HANDLE;

/* BTM_CLB_REGISTER_EVT Data */
typedef struct
{
    tBTM_STATUS status;
    tBTM_CLB_HANDLE handle;
    UINT8 lt_addr;
    tBTM_CLB_MODE mode;
} tBTM_CLB_REGISTER_DATA;

/* BTM_CLB_DEREGISTER_EVT Data */
typedef struct
{
    tBTM_STATUS status;
    tBTM_CLB_HANDLE handle;
    UINT8 lt_addr;
} tBTM_CLB_DEREGISTER_DATA;

/* BTM_CLB_TX_START_EVT Data */
typedef struct
{
    UINT8 status;
    tBTM_CLB_HANDLE handle;
    UINT8 lt_addr;
    UINT16 interval;
} tBTM_CLB_TX_START_DATA;

/* BTM_CLB_TX_STOP_EVT Data */
typedef struct
{
    UINT8 status;
    tBTM_CLB_HANDLE handle;
    UINT8 lt_addr;
} tBTM_CLB_TX_STOP_DATA;

/* BTM_CLB_STREAM_TX_CFM_EVT Data */
typedef struct
{
    UINT8 status;
    tBTM_CLB_HANDLE handle;
    UINT8 lt_addr;
} tBTM_CLB_STREAM_TX_CFM_DATA;


/* BTM_CLB_RX_START_EVT Data */
typedef struct
{
    UINT8 status;
    tBTM_CLB_HANDLE handle;
    UINT8 lt_addr;
} tBTM_CLB_RX_START_DATA;

/* BTM_CLB_RX_STOP_EVT Data */
typedef struct
{
    UINT8 status;
    tBTM_CLB_HANDLE handle;
    UINT8 lt_addr;
} tBTM_CLB_RX_STOP_DATA;

/* BTM_CLB_RX_TIMEOUT_EVT Data */
typedef struct
{
    tBTM_CLB_HANDLE handle;
    UINT8 lt_addr;
} tBTM_CLB_RX_TIMEOUT_DATA;

/* BTM_CLB_RX_DATA_EVT Data */
typedef struct
{
    tBTM_CLB_HANDLE handle;
    UINT8 lt_addr;
    UINT8 length;
    UINT8 fragment;
    UINT8 *p_data;
} tBTM_CLB_RX_DATA_DATA;


/* CLB Message data definitions */
typedef union
{
    tBTM_CLB_REGISTER_DATA register_cplt;
    tBTM_CLB_DEREGISTER_DATA deregister_cplt;
    tBTM_CLB_TX_START_DATA tx_start;
    tBTM_CLB_TX_STOP_DATA tx_stop;
    tBTM_CLB_STREAM_TX_CFM_DATA tx_cfm;
    tBTM_CLB_RX_START_DATA rx_start;
    tBTM_CLB_RX_STOP_DATA rx_stop;
    tBTM_CLB_RX_TIMEOUT_DATA rx_timeout;
    tBTM_CLB_RX_DATA_DATA rx_data;
} tBTM_CLB_EVENT_DATA;

/* BTM CLB event callback function  */
typedef void (tBTM_CLB_EVENTS_CB)(tBTM_CLB_EVT event, tBTM_CLB_EVENT_DATA *p_data);

/*
 * Function definitions
 */

/*******************************************************************************
**
** Function         BTM_ClbRegister
**
** Description      Register a ConnectionLess Broadcast channel
**
** Parameters       mode: Combinaison of BTM_CLB_MODE_TX, BTM_CLB_MODE_RX and
**                      BTM_CLB_MODE_STREAM
**                  lt_addr: LtAddr to register (if BTM_CLB_MODE_TX set in mode)
**                  p_callback: CLB  Callback function
**
** Returns          BTM_CMD_STARTED if command is successfully initiated.
**                      In this case, the callback will be called later
**                  BTM_SUCCESS if command is successfully finished.
**                      In this case, the callback has already been called
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_ClbRegister(tBTM_CLB_MODE mode, UINT8 lt_addr, tBTM_CLB_EVENTS_CB *p_callback);

/*******************************************************************************
**
** Function         BTM_ClbDeregister
**
** Description      Deregister a ConnectionLess Broadcast channel
**
** Parameters       handle: CLB Handle to deregister
**
** Returns          BTM_CMD_STARTED if command is successfully initiated.
**                      In this case, the callback will be called later
**                  BTM_SUCCESS if command is successfully finished.
**                      In this case, the callback has already been called
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_ClbDeregister(tBTM_CLB_HANDLE handle);

/*******************************************************************************
**
** Function         BTM_ClbTxStart
**
** Description      Start a Tx ConnectionLess Broadcast channel
**
** Parameters       handle: CLB Handle
**                  lpo_allowed: Indicates if the controller may sleep
**                  packet_type: allowed packet types
**                  interval_min:Minimum Interval between Beacon slots
**                  interval_max:Minimum Interval between Beacon slots
**                  nb_retransmission: Number of retransmission
**
** Returns          BTM_CMD_STARTED if command is successfully initiated
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_ClbTxStart(tBTM_CLB_HANDLE handle, UINT8 lpo_allowed,
        UINT16 packet_type, UINT16 interval_min, UINT16 interval_max,
        UINT8 nb_retransmission);
/*******************************************************************************
**
** Function         BTM_ClbTxStop
**
** Description      Stop a Tx ConnectionLess Broadcast channel
**
** Parameters       handle: CLB Handle
**
** Returns          BTM_CMD_STARTED if command is successfully initiated
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_ClbTxStop(tBTM_CLB_HANDLE handle);

/*******************************************************************************
**
** Function         BTM_ClbTxData
**
** Description      Send Data on Tx ConnectionLess Broadcast channel
**
** Parameters       handle: CLB Handle
**                  p_data: Pointer to data to send
**                  length: length of the data to send
**
** Returns          BTM_CMD_STARTED if command is successfully initiated
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_ClbTxData(tBTM_CLB_HANDLE handle, UINT8 *p_data, UINT16 length);

/*******************************************************************************
**
** Function         BTM_ClbTxBuf
**
** Description      Send a GKI buffer on Tx ConnectionLess Broadcast channel
**
** Parameters       handle: CLB Handle
**                  p_data: Pointer to data to send
**                  length: length of the data to send
**
** Returns          BTM_SUCCESS if command is successful
**
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_ClbTxBuf(tBTM_CLB_HANDLE handle, BT_HDR *p_buf);

/*******************************************************************************
**
** Function         BTM_ClbSyncTrainStart
**
** Description      Start the Synchronization TRrin
**
** Parameters       None
**
** Returns          BTM_CMD_SUCCESS if command is successfully initiated
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_ClbSyncTrainStart(void);

/*******************************************************************************
**
** Function         BTM_ClbRxStart
**
** Description      Start a Rx ConnectionLess Broadcast channel
**
** Parameters       handle: CLB Handle
**                  lt_addr: LtAddr to lock on
**                  timeout: Link timeout
**                  remote_timing_accuracy: Accuracy of the broadcaster (ppm)
**                  skip: number of Rx to skip after each successful Rx
**                  packet_type: allowed packet types
**
** Returns          BTM_CMD_STARTED if command is successfully initiated
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_ClbRxStart(BD_ADDR bd_addr, tBTM_CLB_HANDLE handle, UINT8 lt_addr,
        UINT16 timeout, UINT8 remote_timing_accuracy, UINT8 skip, UINT16 packet_type);

/*******************************************************************************
**
** Function         BTM_ClbRxStop
**
** Description      Stop a Rx ConnectionLess Broadcast channel
**
** Parameters       handle: CLB Handle
**
** Returns          BTM_CMD_STARTED if command is successfully initiated
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_ClbRxStop(tBTM_CLB_HANDLE handle);

#endif /* BTM_CLB_API_H */
