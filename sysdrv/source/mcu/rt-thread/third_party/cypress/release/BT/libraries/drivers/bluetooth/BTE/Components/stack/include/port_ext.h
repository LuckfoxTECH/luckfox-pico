/*****************************************************************************/
/*                                                                           */
/*  Name:          port_ext.h                                                */
/*                                                                           */
/*  Description:   This file contains external definitions of Port Emulation */
/*                 entity unit                                               */
/*                                                                           */
/*                                                                           */
/*  Copyright (c) 1999-2004, WIDCOMM Inc., All Rights Reserved.              */
/*  WIDCOMM Bluetooth Core. Proprietary and confidential.                    */
/*****************************************************************************/
#ifndef PORTEXT_H
#define PORTEXT_H

#include "gki.h"

/* Port emulation entity Entry Points */
extern void rfcomm_process_timeout(TIMER_LIST_ENT  *p_tle);
#endif
