/*****************************************************************************
**
**  Name:          hciutil.h
**
**  Description:
**      This file contains definitions and function prototypes for the.
**      HCI Utility module.
**
**  Copyright (c) 2002-2006, WIDCOMM Inc., All Rights Reserved.
**  WIDCOMM Bluetooth Core. Proprietary and confidential.
******************************************************************************/
#ifndef HCIUTIL_H
#define HCIUTIL_H

#include "bt_target.h"

/*******************************************************************************
**  HCIUTIL_UpdateBaudRate Callback Functions
**  Input Param     status: 0 for success, 1 for fail
**
*******************************************************************************/
#define HCIUTIL_STATUS_SUCCESS    0
#define HCIUTIL_STATUS_FAILURE    1

typedef void (tHCIUTIL_UPDATEBAUDRATE_CBACK)(UINT8 status);

/*****************************************************************************
**  External Function Declarations
*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 **
 ** Function           HCIUTIL_UpdateBaudRate
 **
 ** Description       Resets the baud rate for this HCI connection
 ** Input Param       baudrate - uart baudrate
 **                   clockrate - uart clockrate (24M or 48M)
 **                   p_cb - callback for update baudrate
 **
 ** Returns      TRUE:  Command was sent
 **              FALSE:  Command not supported or transport busy
 **
 *******************************************************************************/
BT_API extern BOOLEAN HCIUTIL_UpdateBaudRate(UINT8 baudrate, UINT32 clockrate,
        tHCIUTIL_UPDATEBAUDRATE_CBACK *p_cb);

#ifdef __cplusplus
}
#endif

#endif
