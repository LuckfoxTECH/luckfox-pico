/*****************************************************************************
**
**  Name:          hcisl.h
**
**  Description:
**      HCI Services for the lower layer stack.
**
**      This file contains definitions used by the lower-layer HCIS,
**
**  Copyright (c) 2002-2004, WIDCOMM Inc., All Rights Reserved.
**  WIDCOMM Bluetooth Core. Proprietary and confidential.
******************************************************************************/
#ifndef HCISL_H
#define HCISL_H

#include "bt_target.h"
#include "bt_types.h"

/*****************************************************************************
** Prototypes for HCI Service interface functions
******************************************************************************/

/* Initialize transport's control block and hardware */
typedef void (*tHCISL_INIT)(UINT8 lm_task_id, UINT8 transport_task_id, UINT16 transport_task_evt);

/* Open port for HCI services */
typedef BOOLEAN(*tHCISL_OPEN)(void);

/* Close port for HCI services */
typedef void (*tHCISL_CLOSE)(void);

/* Send HCI command/data to the transport */
typedef BOOLEAN(*tHCISL_SEND)(BT_HDR *p_msg);

/* Handler for HCIS events */
typedef UINT32(*tHCISL_EVENT_HDLR)(UINT16 event);

/*****************************************************************************
** Structure of interface functions
******************************************************************************/
typedef struct
{
    tHCISL_INIT init;
    tHCISL_OPEN open;
    tHCISL_CLOSE close;
    tHCISL_SEND send;
    tHCISL_EVENT_HDLR handle_event;
} tHCISL_IF;


/* Pointer to current HCI Service functions */
extern tHCISL_IF *p_hcisl_if;   /* Pointer to current HCIS interface functions */
extern void *p_hcisl_cfg;       /* Pointer to configuration parameter of current HCIS */

#endif /* ifndef HCISL_H */
