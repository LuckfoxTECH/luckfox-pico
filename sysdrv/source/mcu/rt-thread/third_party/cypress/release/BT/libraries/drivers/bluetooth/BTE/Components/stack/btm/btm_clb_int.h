/*****************************************************************************
**
**  Name:          btm_cbl_int.h
**
**  Description:   this file contains the main Bluetooth Manager (BTM)
**                 internal definitions for ConnectionLess Broadcast.
**
**  Copyright (c) 2012-2015, Broadcom Corp., All Rights Reserved
**  Broadcom Bluetooth Core. Proprietary and confidential.
******************************************************************************/
#ifndef BTM_CLB_INT_H
#define BTM_CLB_INT_H

#include "btm_clb_api.h"

/*
 * Definitions
 */
#ifndef BTM_CLB_SYNC_TRAIN_INT_MIN
#define BTM_CLB_SYNC_TRAIN_INT_MIN      128     /* Number of Slots */
#endif
#ifndef BTM_CLB_SYNC_TRAIN_INT_MAX
#define BTM_CLB_SYNC_TRAIN_INT_MAX      128     /* Number of Slots */
#endif
#ifndef BTM_CLB_SYNC_TRAIN_TIMEOUT
#define BTM_CLB_SYNC_TRAIN_TIMEOUT      0x07FFFFFE  /* Duration in Number of Slots */
#endif
#ifndef BTM_CLB_SYNC_TRAIN_RX_TOUT
#define BTM_CLB_SYNC_TRAIN_RX_TOUT      0x2000  /* Sync Train Timeout */
#endif
#ifndef BTM_CLB_SYNC_TRAIN_RX_WIND
#define BTM_CLB_SYNC_TRAIN_RX_WIND      0x86    /* Sync Train Window */
#endif
#ifndef BTM_CLB_SYNC_TRAIN_RX_INT
#define BTM_CLB_SYNC_TRAIN_RX_INT       0       /* Sync Train Interval */
#endif
#ifndef BTM_CLB_SYNC_TRAIN_SERVICE_DATA
#define BTM_CLB_SYNC_TRAIN_SERVICE_DATA 0x01    /* 3D Service Data */
#endif

#ifndef BTM_CLB_TX_TIMEOUT
#define BTM_CLB_TX_TIMEOUT              0x0000  /* CLB Set Timeout (unused currently) */
#endif

/* Channel connectionless broadcast state */
#define BTM_CLB_CHANNEL_STATE_IDLE              0   /* Inactive */
#define BTM_CLB_CHANNEL_STATE_TX_STARTING       1   /* Initiating Tx Start */
#define BTM_CLB_CHANNEL_STATE_TX_STOPPING       2   /* Initiating Tx Stop */
#define BTM_CLB_CHANNEL_STATE_TX                3   /* Currently Tx */
#define BTM_CLB_CHANNEL_STATE_RX_TRAIN          4   /* Dealing with SyncTrain */
#define BTM_CLB_CHANNEL_STATE_RX_STARTING       5   /* Initiating Rx Start */
#define BTM_CLB_CHANNEL_STATE_RX_STOPPING       6   /* Initiating Rx Stop */
#define BTM_CLB_CHANNEL_STATE_RX                7   /* Currently Rx */
typedef UINT8 tBTM_CLB_CHANNEL_STATE;

/* Global connectionless broadcast state */
#define BTM_CLB_STATE_IDLE                      0   /* Inactive */
#define BTM_CLB_STATE_SYNC_TRAIN_RX1            1   /* First Rx SyncTrain */
#define BTM_CLB_STATE_SYNC_TRAIN_REQ            2   /* SyncTrain Request */
#define BTM_CLB_STATE_SYNC_TRAIN_RX2            3   /* Second Rx SyncTrain */
typedef UINT8 tBTM_CLB_STATE;

/*
 * Defines the ConnectionLessBroadcast Control Block
 */
typedef struct
{
    BOOLEAN in_use;
    tBTM_CLB_EVENTS_CB *p_callback;
    tBTM_CLB_MODE mode;
    tBTM_CLB_CHANNEL_STATE state;
    UINT8 lt_addr;
    tBTM_CLB_HANDLE handle;
    BD_ADDR bd_addr;
    UINT8 nb_pending_packets;
} tBTM_CLB_CCB;

typedef struct
{
    tBTM_CLB_HANDLE handle;
    BD_ADDR bd_addr;
    UINT8 lt_addr;
    UINT16 timeout;
    UINT8 remote_timing_accuracy;
    UINT8 skip;
    UINT16 packet_type;
} tBTM_CLB_RX_PARAM;

typedef struct
{
    BOOLEAN sync_train_param_done;              /* TRUE if the SyncTrainParameters have been sent */
    tBTM_CLB_STATE state;                       /* Global CLB State */
    UINT8 packet_id;
    tBTM_CLB_RX_PARAM rx_param;                 /* Saved Parameters for the Channel initiating Rx */
    tBTM_CLB_CCB ccb[BTM_CLB_CHANNEL_MAX];
} tBTM_CLB_CB;

/*******************************************************************************
**
** Function         btm_clb_init
**
** Description      This function is called at BTM startup to initialize CLB
**
** Returns          void
**
*******************************************************************************/
extern void btm_clb_init(void);

/*******************************************************************************
**
** Function         btm_clb_set_reserved_lt_addr_complete
**
** Description      Function called when the HCI_SetReservedLtAddr complete is
**                  received
**
** Parameters       p: pointer on event data
**                  event_length: Length of event data
**
** Returns          None
**
*******************************************************************************/
extern void btm_clb_set_reserved_lt_addr_complete(UINT8 *p, UINT16 event_length);

/*******************************************************************************
**
** Function         btm_clb_delete_reserved_lt_addr_complete
**
** Description      Function called when the HCI_DeleteReservedLtAddr complete
**
** Parameters       p: pointer on event data
**                  event_length: Length of event data
**
** Returns          None
**
*******************************************************************************/
extern void btm_clb_delete_reserved_lt_addr_complete(UINT8 *p, UINT16 event_length);

/*******************************************************************************
**
** Function         btm_clb_write_clb_data_complete
**
** Description      Function called when the HCI_WriteConnectionLessBroadcastData completes
**
** Parameters       p: pointer on event data
**                  event_length: Length of event data
**
** Returns          None
**
*******************************************************************************/
extern void btm_clb_write_clb_data_complete(UINT8 *p, UINT16 event_length);

/*******************************************************************************
**
** Function         btm_clb_write_sync_train_param_complete
**
** Description      Function called when the HCI_WriteSyncTrainParam complete
**
** Parameters       p: pointer on event data
**                  event_length: Length of event data
**
** Returns          None
**
*******************************************************************************/
extern void btm_clb_write_sync_train_param_complete(UINT8 *p, UINT16 event_length);

/*******************************************************************************
**
** Function         btm_clb_read_sync_train_param_complete
**
** Description      Function called when the HCI_ReadSyncTrainParam completes
**
** Parameters       p: pointer on event data
**                  event_length: Length of event data
**
** Returns          None
**
*******************************************************************************/
extern void btm_clb_read_sync_train_param_complete(UINT8 *p, UINT16 event_length);

/*******************************************************************************
**
** Function         btm_clb_start_sync_train_status
**
** Description      Function called when the HCI_StartSyncTrain returns a status
**
** Parameters       status: HCI Status returned
**
** Returns          None
**
*******************************************************************************/
extern void btm_clb_start_sync_train_status(UINT8 status);

/*******************************************************************************
**
** Function         btm_clb_sync_train_complete
**
** Description      Function called when the HCI_StartSyncTrain complete
**
** Parameters       p: pointer on event data
**                  event_length: Length of event data
**
** Returns          None
**
*******************************************************************************/
extern void btm_clb_sync_train_complete(UINT8 *p, UINT16 event_length);

/*******************************************************************************
**
** Function         btm_clb_truncated_page_status
**
** Description      Function called when the HCI_TruncatedPage returns a status
**
** Parameters       status: HCI Status returned
**
** Returns          None
**
*******************************************************************************/
extern void btm_clb_truncated_page_status(UINT8 status);

/*******************************************************************************
**
** Function         btm_clb_truncated_page_complete
**
** Description      Function called when the HCI_TruncatedPage completes
**
** Parameters       p: pointer on event data
**                  event_length: Length of event data
**
** Returns          None
**
*******************************************************************************/
extern void btm_clb_truncated_page_complete(UINT8 *p, UINT16 event_length);

/*******************************************************************************
**
** Function         btm_clb_truncated_page_cancel_complete
**
** Description      Function called when the HCI_TruncatedPageCancel completes
**
** Parameters       p: pointer on event data
**                  event_length: Length of event data
**
** Returns          None
**
*******************************************************************************/
extern void btm_clb_truncated_page_cancel_complete(UINT8 *p, UINT16 event_length);

/*******************************************************************************
**
** Function         btm_clb_receive_sync_train_status
**
** Description      Function called when the HCI_RxSyncTrain returns a status
**
** Parameters       status: HCI Status returned
**
** Returns          None
**
*******************************************************************************/
extern void btm_clb_receive_sync_train_status(UINT8 hci_status);

/*******************************************************************************
**
** Function         btm_clb_sync_train_received_evt
**
** Description      Function called when the HCI_RxSyncTrain completes
**
** Parameters       p: pointer on event data
**                  event_length: Length of event data
**
** Returns          None
**
*******************************************************************************/
extern void btm_clb_sync_train_received_evt(UINT8 *p, UINT16 event_length);

/*******************************************************************************
**
** Function         btm_clb_set_clb_complete
**
** Description      Function called when the HCI_SetConnectionlessBroadcast complete
**
** Parameters       p: pointer on event data
**                  event_length: Length of event data
**
** Returns          None
**
*******************************************************************************/
extern void btm_clb_set_clb_complete(UINT8 *p, UINT16 event_length);

/*******************************************************************************
**
** Function         btm_clb_receive_clb_complete
**
** Description      Function called when the HCI_ReceiveConnectionlessBroadcast
** completes
**
** Parameters       p: pointer on event data
**                  event_length: Length of event data
**
** Returns          None
**
*******************************************************************************/
extern void btm_clb_receive_clb_complete(UINT8 *p, UINT16 event_length);

/*******************************************************************************
**
** Function         btm_clb_slave_page_resp_timeout_evt
**
** Description      Function called when the HCI_SLAVE_PAGE_RESP_TIMEOUT_EVT event
**                  is received
**
** Parameters       p: pointer on event data
**                  event_length: Length of event data
**
** Returns          None
**
*******************************************************************************/
extern void btm_clb_slave_page_resp_timeout_evt(UINT8 *p, UINT16 event_length);

/*******************************************************************************
**
** Function         btm_clb_data_received_evt
**
** Description      Function called when the CLB data is received event
**                  is received
**
** Parameters       p: pointer on event data
**                  event_len: Length of event data
**
** Returns          None
**
*******************************************************************************/
extern void btm_clb_data_received_evt(UINT8 *p, UINT16 event_length);

/*******************************************************************************
**
** Function         btm_clb_rx_timeout_evt
**
** Description      Function called when the CLB Rx timeout event is received
**
** Parameters       p: pointer on event data
**                  event_len: Length of event data
**
** Returns          None
**
*******************************************************************************/
extern void btm_clb_rx_timeout_evt(UINT8 *p, UINT16 event_length);

#endif /* BTM_CLB_INT_H */

