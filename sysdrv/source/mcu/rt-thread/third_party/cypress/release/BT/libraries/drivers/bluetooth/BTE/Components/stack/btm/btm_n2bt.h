/*****************************************************************************
**
**  Name:          btm_n2bt.h
**
**  Description:   Support for N2BT
**
**  Copyright (c) 1999-2007, Broadcom Corp., All Rights Reserved.
**  WIDCOMM Bluetooth Core. Proprietary and confidential.
******************************************************************************/
#ifndef BTM_N2BT_H
#define BTM_N2BT_H
#include "btm_api.h"

/*****************************************************************************\
**  CONSTANT AND STRUCTURE DEFINITIONS
\*****************************************************************************/
#define HCI_BRCM_ACQUIRE_N2BT_SENSOR    (0x86 | HCI_GRP_VENDOR_SPECIFIC)

/* Maximum simultaneous sensors connected */
#ifndef BTM_N2BT_MAX_DEV
#define BTM_N2BT_MAX_DEV    (5)
#endif

/* tBTM_N2BT_DEVINFO: sensor device info */
typedef struct
{
    BD_ADDR bda;
    UINT16  hci_handle;
    BOOLEAN in_use;
} tBTM_N2BT_DEVINFO;

typedef struct
{
    BOOLEAN enabled;
    tBTM_N2BT_DATA_CB *p_data_cb;
    tBTM_N2BT_DEVINFO dev[BTM_N2BT_MAX_DEV];
} tBTM_N2BT_CB;

BOOLEAN btm_n2bt_handle_connection_comp(BD_ADDR bda, UINT16 hci_handle, UINT8 status);
BOOLEAN btm_n2bt_handle_acl(BT_HDR *p_msg);

#endif   /* BTM_N2BT_H */
