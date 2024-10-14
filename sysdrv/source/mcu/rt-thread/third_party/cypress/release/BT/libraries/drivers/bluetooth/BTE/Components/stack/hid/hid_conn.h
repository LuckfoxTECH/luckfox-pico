/****************************************************************************/
/*                                                                          */
/*  Name:       hid_conn.h                                                  */
/*                                                                          */
/*  Function:   this file contains HID connection internal definitions      */
/*                                                                          */
/*                                                                          */
/*  Copyright (c) 2002-2015, Broadcom Corp., All Rights Reserved.           */
/*  Broadcom Bluetooth Core. Proprietary and confidential.                  */
/****************************************************************************/
#ifndef HID_CONN_H
#define HID_CONN_H


/* Define the HID Connection Block
*/
typedef struct hid_conn
{
#define HID_CONN_STATE_UNUSED           (0)
#define HID_CONN_STATE_CONNECTING_CTRL  (1)
#define HID_CONN_STATE_CONNECTING_INTR  (2)
#define HID_CONN_STATE_CONFIG           (3)
#define HID_CONN_STATE_CONNECTED        (4)
#define HID_CONN_STATE_DISCONNECTING    (5)
#define HID_CONN_STATE_SECURITY         (6)

    UINT8             conn_state;

#define HID_CONN_FLAGS_IS_ORIG              (0x01)
#define HID_CONN_FLAGS_CONGESTED            (0x20)
#define HID_CONN_FLAGS_INACTIVE             (0x40)

    UINT8             conn_flags;

    UINT16            ctrl_cid;
    UINT16            intr_cid;
    UINT16            rem_mtu_size;
    UINT16            disc_reason;                       /* Reason for disconnecting (for HID_HDEV_EVT_CLOSE) */
    TIMER_LIST_ENT    timer_entry;

} tHID_CONN;

#define HID_SEC_CHN   1
#define HID_NOSEC_CHN 2

#define HIDD_SEC_CHN    3
#define HIDD_NOSEC_CHN  4

#endif
