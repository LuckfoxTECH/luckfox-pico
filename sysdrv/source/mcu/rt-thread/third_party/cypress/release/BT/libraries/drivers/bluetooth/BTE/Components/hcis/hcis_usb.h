/*****************************************************************************
**
**  Name:          hcis_usb.h
**
**  Description:
**      USB HCI Services
**
**      This file contains definitions and function prototypes for the USB.
**      HCI services.
**
**  Copyright (c) 2002-2004, WIDCOMM Inc., All Rights Reserved.
**  WIDCOMM Bluetooth Core. Proprietary and confidential.
******************************************************************************/
#ifndef HCIS_USB_H
#define HCIS_USB_H

/*****************************************************************************
** Definitions
******************************************************************************/

/* Configuration structure for USB */
typedef struct
{
    UINT8   port_id;            /* Port ID */
    UINT8   baud;               /* Baud rate */
    UINT16  data_fmt;           /* Data format (parity, data bits, and stop bits) */
} tHCIS_USB_CFG;

enum
{
    HCISU_USB_CLOSED_ST,
    HCISU_USB_OPENING_ST,
    HCISU_USB_OPENED_ST,
    HCISU_USB_CLOSING_ST
};
typedef UINT8 tHCISU_USB_STATE;


/* Trasnprt receive states */
enum
{
    HCISU_USB_MSGTYPE_ST,
    HCISU_USB_LEN_ST,
    HCISU_USB_NOBUF_LEN_ST,
    HCISU_USB_DATA_ST,
    HCISU_USB_IGNORE_ST
};
typedef UINT8 tHCISU_USB_RCV_STATE;

/* Control block for HCISU_USB */
typedef struct
{
    BT_HDR  *p_rcv_msg;             /* Buffer for holding current incoming HCI message */
    UINT16  rcv_len;                /* Size of current incoming message */
    UINT16  transport_task_evt;     /* Event used to indicate an HCIS event */
    UINT8   rcv_msg_type;           /* Current incoming message type */
    UINT8   previous_rcv_byte;      /* Used to accumulate message length from stream */
    tHCISU_USB_RCV_STATE rcv_state;  /* Receive state of current incoming message */
    UINT8 port_id;                  /* Port ID of serial port */
    tHCISU_USB_STATE usb_state;       /* State of serial port */
    UINT8 btu_task_id;              /* Task that is handling incoming HCI messages */
    UINT8 transport_task_id;        /* Task that is handling HCIS evnets */
} tHCISU_USB_CB;


/*****************************************************************************
** External Declarations
******************************************************************************/

extern const tHCISU_IF hcisu_usb;

#endif      /* #ifndef HCIS_USB_H */
