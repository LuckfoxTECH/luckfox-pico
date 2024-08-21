/********************************************************************************
**                                                                              *
**  Name        brcm_int.h                                               *
**                                                                              *
**  Function    This file contains patch ram loader header file                 *
**                                                                              *
**                                                                              *
**  Copyright (c) 1999-2007, Broadcom Inc., All Rights Reserved.                *
**  Proprietary and confidential.                                               *
**                                                                              *
*********************************************************************************/


#ifndef BRCM_INT_H
#define BRCM_INT_H

#include "bt_target.h"
#include "gki.h"
#include "brcm_hcidefs.h"
#include "brcm_api.h"
#include "btu_vs_api.h"


/* Define a structure for BRCM PRM Control Block
*/
typedef struct
{
    tBRCM_PRM_STATE  state;                /* download state */
    UINT16              opcode; /* current opcode being used for the download */
    BOOLEAN             internal_patch;       /* internal patch or not */
    UINT16              patch_length;         /* total patch length need to download*/
    const UINT8        *p_patch_data;         /* pointer to orginal patch data */
    UINT16              tx_patch_length;      /* patch length send to controller */
    tBRCM_PRM_CBACK     *p_cb;                /* call back for patch ram status */
    UINT32              dest_ram;
    UINT8               format_type;          /* patch format bin/hcd/... */
#if !defined(BRCM_REGISTER_TIMER) && (BRCM_USE_DELAY == FALSE)
    /* This only applies when we use btu timer mechanism */
    TIMER_LIST_ENT      timer;
#endif
} tBRCM_PRM_CB;

typedef struct
{
#if (defined(BTM_PCM2_INCLUDED) && BTM_PCM2_INCLUDED == TRUE)
    UINT16                  sys_features;
    UINT8                   pcm2_params[BRCM_PCM2_SETUP_WRITE_SIZE];
    tBTU_PCM2_ACTION        pcm2_action;
#endif
    UINT8               empty;

} tBRCM_CB;


extern const char brcm_patch_version[];

#if (BRCM_USE_DELAY == FALSE)
extern void brcm_timer_cback(void *p_tle);
#endif

extern void l2c_brcm_init(void);

#if BTM_TBFC_INCLUDED == TRUE
extern void brcm_tbfc_init(void);
extern BOOLEAN brcm_tbfc_connect(tBTU_TBFC_CONN_PARAM *p);
extern tBTM_STATUS btm_tbfc_suspend(tBTU_TBFC_SUSPEND_PARAM *p);
extern BOOLEAN btm_tbfc_process_le_notify(tBTU_TBFC_LE_NOTIFY_PARAM *p_param);
extern BOOLEAN btm_tbfc_send_trigger(tBTU_TBFC_TRIGGER_PARAM *p_param);

#if (BTM_BI_DIR_TBFC_INCLUDED == TRUE)
extern void brcm_tbfc_process_timeout(TIMER_LIST_ENT *p_tle);
#endif

#endif

#endif

