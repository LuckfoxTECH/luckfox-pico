/*****************************************************************************
**
**  Name:          hcisu.h
**
**  Description:
**      HCI Services for the upper layer stack.
**
**      This file contains definitions used by the lower-layer HCIS,
**
**  Copyright (c) 2002-2004, WIDCOMM Inc., All Rights Reserved.
**  WIDCOMM Bluetooth Core. Proprietary and confidential.
******************************************************************************/
#ifndef HCISU_H
#define HCISU_H

#include "bt_target.h"
#include "bt_types.h"
#include "hcilp.h"
#include "hciutil.h"
#include "gki.h"

/*****************************************************************************
** Definitions for HCISU interface
******************************************************************************/

/* HCISU reconfiguration defintions */

/* tHCISU_CFG_ID definitions */
/* ID 0x0000 to 0x00FF are for transport independent config items */
/* ID 0x0100 to 0x01FF are for H4 config items (see hcis_h4.h for H4 config definitions) */

#define HCISU_CFG_ID_LPM    0x00000000  /* Reconfigure low power mode */
typedef UINT32 tHCISU_CFG_ID;


/*****************************************************************************
** Prototypes for HCI Service interface functions
******************************************************************************/

/* Initialize transport's control block and hardware */
typedef void (*tHCISU_INIT)(UINT8 btu_task_id,
                            UINT8 transport_task_id,
                            UINT16 transport_task_evt);

/* Open port for HCI services */
typedef BOOLEAN(*tHCISU_OPEN)(void *p_cfg);

/* Close port for HCI services */
typedef void (*tHCISU_CLOSE)(void);

/* Send HCI command/data to the transport */
typedef BOOLEAN(*tHCISU_SEND)(BT_HDR *p_msg);

/* Handler for HCIS events */
typedef UINT32(*tHCISU_EVENT_HDLR)(UINT16 event);

#if defined (SLIP_INCLUDED) && SLIP_INCLUDED == TRUE
typedef void (*tHCISU_RESET_SLIP)(void *p_cfg);
#endif

/* Start quick timer on HCISU task or BTU task */
#ifndef HCI_LP_START_QUICK_TIMER
#if (HCISU_H4_INCLUDED == TRUE)
#define HCI_START_QUICK_TIMER(p1, p2, p3)  bte_hcisu_start_quick_timer((TIMER_LIST_ENT*)(p1), (UINT16)(p2), (UINT32)(p3))
#else
#define HCI_START_QUICK_TIMER(p1, p2, p3)  btu_start_quick_timer((TIMER_LIST_ENT*)(p1), (UINT16)(p2), (UINT32)(p3))
#endif
#endif

/* Stop quick timer on HCISU task or BTU task */
#ifndef HCI_LP_STOP_QUICK_TIMER
#if (HCISU_H4_INCLUDED == TRUE)
#define HCI_STOP_QUICK_TIMER(p1)  bte_hcisu_stop_quick_timer((TIMER_LIST_ENT*)(p1))
#else
#define HCI_STOP_QUICK_TIMER(p1)  btu_stop_quick_timer((TIMER_LIST_ENT*)(p1))
#endif
#endif

/*****************************************************************************
** Structure of interface functions
******************************************************************************/
typedef struct
{
    tHCISU_INIT init;
    tHCISU_OPEN open;
    tHCISU_CLOSE close;
    tHCISU_SEND send;
    tHCISU_EVENT_HDLR handle_event;
#if defined (SLIP_INCLUDED) && SLIP_INCLUDED == TRUE
    tHCISU_RESET_SLIP reset_slip;
#endif
} tHCISU_IF;

#if (defined(HCILP_INCLUDED) && HCILP_INCLUDED == TRUE)
typedef struct
{
    BT_HDR          hdr;
#if (defined(H4IBSS_INCLUDED) && H4IBSS_INCLUDED == TRUE)
    UINT8           h4ibss_evt;
#endif
} tHCISU_LP_IF_MSG;
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/* Pointer to current HCI Service functions */
extern tHCISU_IF *p_hcisu_if;   /* Pointer to current HCIS interface functions */
extern void *p_hcisu_cfg;       /* Pointer to configuration parameter of current HCIS */
BT_API extern void bte_hcisu_task(UINT32 param);

#if defined (SLIP_INCLUDED) && SLIP_INCLUDED == TRUE
BT_API extern void bte_hcisu_reset_slip(void);
#endif

#if (HCISU_H4_INCLUDED == TRUE)
BT_API extern void bte_hcisu_start_quick_timer(TIMER_LIST_ENT *p_tle, UINT16 type, UINT32 timeout);
BT_API extern void bte_hcisu_stop_quick_timer(TIMER_LIST_ENT *p_tle);
#endif


#ifdef __cplusplus
}
#endif

#endif /* ifndef HCISU_H */
