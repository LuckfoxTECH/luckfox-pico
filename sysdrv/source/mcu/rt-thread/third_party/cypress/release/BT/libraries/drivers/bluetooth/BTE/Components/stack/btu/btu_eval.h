/*****************************************************************************
**
**  Name:          btu_eval.h
**
**  Description:   This file contains the data structures and function
**                 prototypes to check that the stack is running on a BTE
**                 board.
**
**  Copyright (c) 2002-2004, WIDCOMM Inc., All Rights Reserved.
**  WIDCOMM Bluetooth Core. Proprietary and confidential.
******************************************************************************/
#ifndef BTU_EVAL_H
#define BTU_EVAL_H

#include "piodev.h"

#define BTU_NUM_BOOT_BYTES_TO_CHECK     2000

/* Function for verifying that we are running on BTE board */
BTU_API extern BOOLEAN btu_eval_authenticate(void);

#endif
