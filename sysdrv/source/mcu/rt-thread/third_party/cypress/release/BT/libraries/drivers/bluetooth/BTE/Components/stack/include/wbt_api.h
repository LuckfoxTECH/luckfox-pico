/*****************************************************************************
**
**  Name:           wbt_api.h
**
**  Description:    This file contains definitions and constants used by the
**                  Broadcom Bluetooth Extensions API software.
**
**  Copyright (c) 2003-2005, Broadcom Corp., All Rights Reserved.
**  WIDCOMM Bluetooth Core. Proprietary and confidential.
**
*****************************************************************************/
#ifndef WBT_API_H
#define WBT_API_H

#include "bt_target.h"

/*****************************************************************************
**  Constants and Types
*****************************************************************************/

/**************************
* SDP Attribute IDs *
***************************/
#define ATTR_ID_EXT_BRCM_VERSION    0x8001  /* UINT16 (0xmmnn - major, minor [0x0001]) mandatory */
#define ATTR_ID_EXT_PIN_CODE        0x8002  /* UINT32 4 - digit pin */

/**************************
* SDP Attribute ID Values *
***************************/
/* Version Attribute Value */
#define BRCM_EXT_VERSION            0x0001  /* UINT16 (0xmmnn - major, minor [0x0001]) mandatory */

/* Pin Code Attribute Value */
#define BRCM_EXT_PIN_CODE           0x00000000  /* UINT32 ('0000') */

/*****************************************************************************
**  External Function Declarations
*****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

BT_API extern BOOLEAN WBT_ExtCreateRecord(void);

/*** Features ***/
BT_API extern BOOLEAN WBT_ExtAddPinCode(void);


BT_API extern UINT32 wbt_sdp_show_ext(UINT8 scn, char *service_name,
                                      UINT8 pin_code_ext,
                                      UINT8 active_sync_ext);

#ifdef __cplusplus
}
#endif

#endif /* WBT_API_H */
