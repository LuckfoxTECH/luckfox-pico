/****************************************************************************
**
**  Name:       hidd_ble_api.h
**
**  Function:   this file contains HID LE DEVICE side API definitions
**
**
**  Copyright (c) 2002-2004, WIDCOMM Inc., All Rights Reserved.
**  WIDCOMM Bluetooth Core. Proprietary and confidential.
**
*****************************************************************************/
#ifndef HIDD_BLE_API_H
#define HIDD_BLE_API_H

#include "hiddefs.h"
#include "bt_target.h"

/*****************************************************************************
**  Type Definitions
*****************************************************************************/

/* Define the structures that the HID Device function should use to register
** callbacks with HID.
*/

#ifndef HID_LE_INCLUDED
#define HID_LE_INCLUDED         FALSE
#endif

#if (BLE_INCLUDED == FALSE)
#undef HID_LE_INCLUDED
#define HID_LE_INCLUDED     FALSE
#endif

/* HID-LE-Device Callback Events
*/
enum
{
    HID_LE_DEV_EVT_OPEN,      /*Connected to host with Interrupt and Control  Data = 1 if Virtual Cable
                             Channels in OPEN state. pdata = Host BD-Addr.*/

    HID_LE_DEV_EVT_CLOSE,     /*Connection with host is closed.           Data=Reason Code. */
    //HID_LE_DEV_EVT_RETRYING,  /*Lost connection is being re-connected.       Data=Retrial number */
    HID_LE_DEV_EVT_GET_REPORT,/*Host sent GET_REPORT                      Data=Length pdata=structure
                             having details of get-report.*/
    HID_LE_DEV_EVT_SET_REPORT,/*Host sent SET_REPORT                      Data=Length pdata=details.*/
    HID_LE_DEV_EVT_GET_PROTO, /*Host sent GET_PROTOCOL                       Data=NA*/
    HID_LE_DEV_EVT_SET_PROTO, /*Host sent SET_PROTOCOL                       Data=1 for Report, 0 for Boot*/
    HID_LE_DEV_EVT_DATA
};

/* GATT  application error code for HID profile */
#define HIDD_LE_RPT_NOT_SUPT    0x8F

/* HID device type */
#define HIDD_LE_KB_TYPE         0x01    /* bit 0 */
#define HIDD_LE_MICE_TYPE       0x02    /* bit 1 */
#define HIDD_LE_OTHER_TYPE      0x80    /* bit 7 */
typedef UINT8 tHIDD_DEV_TYPE;


#define HIDD_LE_PROTO_MODE_RPT        0x00
#define HIDD_LE_PROTO_MODE_BOOT       0x01
typedef UINT8 tHIDD_LE_PROTO;

/* LE HID report type */
#define HID_LE_RPT_TYPE_INPUT       0x01
#define HID_LE_RPT_TYPE_OUTPUT      0x02
#define HID_LE_RPT_TYPE_FEATURE     0x03
#define HID_LE_RPT_TYPE_KB_INPUT    0x04
#define HID_LE_RPT_TYPE_KB_OUTPUT   0x05
#define HID_LE_RPT_TYPE_MI_INPUT    0x06
typedef UINT8   tHID_LE_RPT_TYPE;
#define HID_LE_RPT_TYPE_MAX         HID_LE_RPT_TYPE_FEATURE

/* report reference descriptor value */
typedef struct
{
    UINT8                   rpt_id;
    tHID_LE_RPT_TYPE        rpt_type;
} tHIDD_LE_RPT_REF;

/* LE HID device registration information */
typedef struct
{
    tHIDD_DEV_TYPE      dev_type;
    UINT8               num_rpt;
    UINT16              battery_handle;
    tHIDD_LE_RPT_REF    *p_rpt_lst;
    tHIDD_LE_PROTO      proto_cap;

} tHIDD_LE_DEV_INFO;

#define HIDD_LE_REMOTE_WAKE   0x01
#define HIDD_LE_NORMAL_CONN   0x02

typedef struct
{
    UINT16                  bcdHID;
    UINT8                   contry_code;
    UINT8                   flags;
    tHID_DEV_DSCP_INFO      rpt_map;
} tHIDD_LE_RPT_MAP_INFO;

#define HIDD_REPT_ID_BOOT_KB        1
#define HIDD_REPT_ID_BOOT_MOUSE     2


typedef struct
{
    BT_HDR  hdr;     /* report data, assuming the first byte of data is report ID */
    UINT8   rpt_id;

} tHIDD_LE_RPT_DATA;

typedef struct
{
    UINT8 rep_type;
    UINT8 rep_id;
} tHIDD_LE_GET_REP_DATA;


typedef union
{
    BD_ADDR             host_bdaddr;
    tHIDD_LE_GET_REP_DATA   get_rpt;
    tHIDD_LE_RPT_DATA  *buffer;
} tHIDD_LE_CBACK_DATA;

typedef void (tHIDD_LE_CBACK)(UINT8  event,  /* Event from HID-DEVICE. */
                              UINT32 data, /* Integer data corresponding to the event. */
                              tHIDD_LE_CBACK_DATA *pdata);  /* Pointer data corresponding to the event.*/
typedef struct
{
    BD_ADDR                 host_addr;/* Host BD-ADDR */
    tHIDD_LE_DEV_INFO       dev_info;
    tHIDD_LE_CBACK         *app_cback;
} tHIDD_LE_REG_INFO;




/*****************************************************************************
**  External Function Declarations
*****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif
/*******************************************************************************
**
** Function         HID_LeDevInit
**
** Description      This function initializes the control block and trace variable
**
** Returns          void
**
*******************************************************************************/
HID_API extern void HID_BleDevInit(void);

/*******************************************************************************
**
** Function         HID_BleDevRegister
**
** Description      This function must be called at startup so the device receive
**                  HID over LE related events and call other HID BLE API Calls.
**
** Returns          tHID_STATUS
**
*******************************************************************************/
HID_API extern tHID_STATUS HID_BleDevRegister(tHIDD_LE_REG_INFO *p_reg_info);

/*******************************************************************************
**
** Function         HID_BleDevDeregister
**
** Description      This function may be used to disable the HID service on the
**                  local device.
**
** Returns          tHID_STATUS
**
*******************************************************************************/
HID_API extern tHID_STATUS HID_BleDevDeregister(void);

/*******************************************************************************
**
** Function         HID_BleDevConnect
**
** Description      This function may be used to initiate a connection to the host..
**
** Returns          tHID_STATUS
**
*******************************************************************************/
HID_API extern tHID_STATUS HID_BleDevConnect(void);

/*******************************************************************************
**
** Function         HID_BleDevDisconnect
**
** Description      This function may be used to disconnect from the host.
**
** Returns          tHID_STATUS
**
*******************************************************************************/
HID_API extern tHID_STATUS HID_BleDevDisconnect(void);

/*******************************************************************************
**
** Function         HID_BleDevSendReport
**
** Description      This function may be used to send report data.
**                  Assuming the first byte of the report data is the report ID.
**
** Returns          tHID_STATUS
**
*******************************************************************************/
HID_API extern tHID_STATUS HID_BleDevSendReport(UINT8 rep_type, UINT8 rpt_id,
        UINT16 len, UINT16 offset, UINT8 *p_rpt);

/*******************************************************************************
**
** Function         HID_BleDevHandshake
**
** Description      This function is used to ack a Set report action.
**
** Returns          tHID_STATUS
**
*******************************************************************************/
HID_API extern tHID_STATUS HID_BleDevHandshake(tHID_STATUS status);

/*******************************************************************************
**
** Function         HID_BleRspGetProtocol
**
** Description      This function is used to respond to a GET Protocol mode request
**
** Returns          tHID_STATUS
**
*******************************************************************************/
HID_API extern tHID_STATUS HID_BleRspGetProtocol(tHIDD_LE_PROTO cur_mode);

/*******************************************************************************
**
** Function         HID_BleDevInit
**
** Description      This function initializes the control block and trace variable
**                  for LE HID module.
**
** Returns          void
**
*******************************************************************************/
HID_API extern void HID_BleDevInit(void);

/*******************************************************************************
**
** Function         HID_BleSetRptMapInfo
**
** Description      This function should be called at startup to configure the device
**                  HID information and report MAP.
**
** Returns          0 if error else sdp handle for the record.
**
*******************************************************************************/
HID_API extern tHID_STATUS HID_BleSetRptMapInfo(tHIDD_LE_RPT_MAP_INFO *p_dev_info);


#ifdef __cplusplus
}
#endif

#endif  /* HIDD_BLE_API_H */
