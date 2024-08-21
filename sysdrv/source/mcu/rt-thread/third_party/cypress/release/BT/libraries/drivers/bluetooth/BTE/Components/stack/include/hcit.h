/****************************************************************************/
/*                                                                          */
/*  Name        Hcit.h                                                      */
/*                                                                          */
/*  Function    this file contains Host Controller Transport externals      */
/*                                                                          */
/*  Date        Modification                                                */
/*  ------------------------                                                */
/*  10/19/99    Victor  Create                                              */
/*                                                                          */
/*  Copyright (c) 1999-2004, WIDCOMM Inc., All Rights Reserved.             */
/*  WIDCOMM Bluetooth Core. Proprietary and confidential.                   */
/*                                                                          */
/****************************************************************************/

#ifndef HCIT_H
#define HCIT_H

#include "bt_target.h"

/* Define the possible error codes from the transport
*/
#define HCIT_NO_ERRS                    0
#define HCIT_ERR_NO_BUFFERS_COMMAND     1
#define HCIT_ERR_NO_BUFFERS_ACL_DATA    2
#define HCIT_ERR_NO_BUFFERS_SCO_DATA    3
#define HCIT_ERR_NO_BUFFERS_EVENT       4
#define HCIT_ERR_BAD_TYPE               5
#define HCIT_ERR_BAD_LEN                6
#define HCIT_ERR_OVERRUN                7
#define HCIT_ERR_PARITY                 8
#define HCIT_ERR_FRAMING                9
#define HCIT_ERR_BREAK                  10
#define HCIT_ERR_LOCAL_RESET            11



/*****************************************************************************/
/*                            C O N S T A N T S                              */
/*****************************************************************************/
enum
{
#if defined(HCI_DIRECT) && (HCI_DIRECT == TRUE)
    HCIT_TYPE_DIRECT,
#endif
#if defined(USB_HCI_INCLUDED) && (USB_HCI_INCLUDED == TRUE)
    HCIT_TYPE_USB,
#endif
#if defined(USB_HCI_INCLUDED) && (UART_HCI_INCLUDED == TRUE)
    HCIT_TYPE_UART,
#endif
    HCIT_TYPE_NUM
};


typedef UINT8 tHCIT_TRANS_TYPE;


extern void HCIT_SetTransportType(tHCIT_TRANS_TYPE TranportType);
extern void HCIT_RestartRcv(void);
#endif

