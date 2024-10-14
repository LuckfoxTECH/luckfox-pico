/*****************************************************************************
**
**  Name:          bte.h
**
**  Description:   this file contains constants and definitions for the bte project
**
**
**  Copyright (c) 2001-2004, WIDCOMM Inc., All Rights Reserved.
**  WIDCOMM Bluetooth Core. Proprietary and confidential.
******************************************************************************/
#ifndef BTE_H
#define BTE_H

#include "bt_target.h"

/* Target Modes (based on jumper settings on hardware [see user manual]) */
enum
{
    /* BTE                  BBY                     */
    /* J3   J4              SW3-3   SW3-2   SW3-1   */
    /* -------------------------------------------- */
    BTE_MODE_SERIAL_APP,    /* OUT  OUT             OFF     OFF     OFF     Sample serial port application      */
    BTE_MODE_APPL,          /* IN   OUT             OFF     OFF     ON      Target used with Tester through RPC */
    BTE_MODE_RESERVED,      /* OUT  IN              OFF     ON      OFF     Reserved                            */
    BTE_MODE_SAMPLE_APPS,   /* IN   IN              OFF     ON      ON      Sample applications (HSP)           */
    BTE_MODE_DONGLE,        /* not yet supported    ON      OFF     OFF     Dongle mode                         */
    BTE_MODE_INVALID
};

extern volatile UINT8    bte_target_mode;    /* indicates the mode that the board is running in */

/* Startup options */
extern UINT32 bte_startup_options;                      /* Switch and jumper settings at startup */
void bte_get_startup_options(UINT32 *p_options);        /* Platform specific function for getting startup options */

#define BTE_OPTIONS_TARGET_MODE_MASK    0x00000007      /* bits 2-0 indicate target mode (QuickConnect: jp3 & jp4, BBY: SW3-1 & SW3-2)*/


/* Pointer to function for sending HCI commands and data to the HCI tranport */
extern int (*p_bte_hci_send)(UINT16 port, BT_HDR *p_msg);


/* Protocol trace mask */
extern UINT32 bte_proto_trace_mask;

/* Version string */
extern const UINT8 bte_version_string[];

#endif  /* BTE_H */
