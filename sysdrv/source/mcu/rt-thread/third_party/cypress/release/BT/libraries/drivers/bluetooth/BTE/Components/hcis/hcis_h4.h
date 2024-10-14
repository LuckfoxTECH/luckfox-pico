/*****************************************************************************
**
**  Name:          hcis_h4.h
**
**  Description:
**      H4 HCI Services
**
**      This file contains definitions and function prototypes for the H4.
**      HCI services.
**
**  Copyright (c) 2002-2013, Broadcom Corp., All Rights Reserved.
**  Broadcom Bluetooth Core. Proprietary and confidential.
******************************************************************************/
#ifndef HCIS_H4_H
#define HCIS_H4_H

#include "bt_target.h"
#include "gki.h"
#include "hcidefs.h"

/*****************************************************************************
** Definitions
******************************************************************************/


/*****************************************************************************/
/* HCISU reconfiguration defintions                                          */

/* tHCISU_CFG_ID definitions (used for reconfiguration of H4 transport) */
/* ID 0x0000 to 0x00FF are for transport independent config items */
/* ID 0x0100 to 0x01FF are for H4 config items */
#define HCISU_CFG_ID_H4_BAUD    0x00000100  /* Reconfigure H4 baud */


/* Config info for HCISU_CFG_ID_H4_BAUD */
typedef struct
{
    UINT8  baud;
    UINT32 clock_rate;
} tHCIS_H4_CFG_BAUD;



/* Feature flags for H4 configuration */
#define HCISU_H4_CFG_FL_FLOW    0x00000001      /* Hardware flow control */
#define HCISU_H4_CFG_FL_SLIP    0x00000002      /* SLIP mode */
#define HCISU_H4_CFG_FL_LPM0    0x00000004      /* Sleep Mode 0 - Disable */
#define HCISU_H4_CFG_FL_LPM1    0x00000008      /* Sleep Mode 1 - UART */
#define HCISU_H4_CFG_FL_LPM4    0x00000010      /* Sleep Mode 4 - H4IBSS */
#define HCISU_H4_CFG_FL_LPM9    0x00000020      /* Sleep Mode 9 - H5 */

/* Configuration structure for H4 (used for opening H4 transport) */
typedef struct
{
    UINT8   port_id;            /* Port ID */
    UINT8   baud;               /* Baud rate */
    UINT16  data_fmt;           /* Data format (parity, data bits, and stop bits) */
    UINT32  cfg_flags;          /* Optional H4 features */
} tHCIS_H4_CFG;

enum
{
    HCISU_H4_CLOSED_ST,
    HCISU_H4_OPENING_ST,
    HCISU_H4_OPENED_ST,
    HCISU_H4_CLOSING_ST
};
typedef UINT8 tHCISU_H4_STATE;


/* Trasnprt receive states */
enum
{
    HCISU_H4_MSGTYPE_ST,
    HCISU_H4_LEN_ST,
    HCISU_H4_NOBUF_LEN_ST,
    HCISU_H4_DATA_ST,
    HCISU_H4_NFC_HDR_ST,
    HCISU_H4_NFC_NOBUF_HDR_ST,
    HCISU_H4_IGNORE_ST
};
typedef UINT8 tHCISU_H4_RCV_STATE;

/* Control block for HCISU_H4 */
typedef struct
{
    BT_HDR  *p_rcv_msg;             /* Buffer for holding current incoming HCI message */
    UINT16  rcv_len;                /* Size of current incoming message */
    UINT16  transport_task_evt;     /* Event used to indicate an HCIS event */
    UINT8   rcv_msg_type;           /* Current incoming message type */
    UINT8   previous_rcv_byte;      /* Used to accumulate message length from stream */
    tHCISU_H4_RCV_STATE rcv_state;  /* Receive state of current incoming message */
    UINT8 port_id;                  /* Port ID of serial port */
    tHCISU_H4_STATE h4_state;       /* State of serial port */
    UINT8 btu_task_id;              /* Task that is handling incoming HCI messages */
    UINT8 transport_task_id;        /* Task that is handling HCIS evnets */
    UINT32  cfg_flags;              /* whether slip mode support */
#if (defined(H4IBSS_INCLUDED) && H4IBSS_INCLUDED == TRUE)
    BUFFER_Q h4ibss_q;              /* holding packet while wakeup procedure */
#endif
#if (defined(BTU_STACK_LITE_ENABLED) && BTU_STACK_LITE_ENABLED == TRUE)
    UINT8 tempAclBuf[HCI_DATA_PREAMBLE_SIZE];
    UINT8 num_temp;
#endif
} tHCISU_H4_CB;


/*****************************************************************************
** External Declarations
******************************************************************************/

extern const tHCISU_IF hcisu_h4;

#endif      /* #ifndef HCIS_H4_H */
