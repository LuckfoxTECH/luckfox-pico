/****************************************************************************
**
**  Name:       hidd_api.h
**
**  Function:   this file contains HID DEVICE side API definitions
**
**
**  Copyright (c) 2002-2004, WIDCOMM Inc., All Rights Reserved.
**  WIDCOMM Bluetooth Core. Proprietary and confidential.
**
*****************************************************************************/
#ifndef HIDD_API_H
#define HIDD_API_H

#include "hiddefs.h"


/*****************************************************************************
**  Type Definitions
*****************************************************************************/

/* Define the structures that the HID Device function should use to register
** callbacks with HID.
*/

typedef struct
{
    FLOW_SPEC ctrl_ch;
    FLOW_SPEC int_ch;
    FLOW_SPEC hci;
} tHID_DEV_QOS_INFO;

typedef struct rep_data
{
    UINT8 rep_type;
    UINT8 rep_id;
} tHID_DEV_GET_REP_DATA;

/* HID-Device Callback Events
*/
enum
{
    HID_DEV_EVT_OPEN,      /*Connected to host with Interrupt and Control  Data = 1 if Virtual Cable
                             Channels in OPEN state. pdata = Host BD-Addr.*/

    HID_DEV_EVT_CLOSE,     /*Connection with host is closed.           Data=Reason Code. */
    HID_DEV_EVT_RETRYING,  /*Lost connection is being re-connected.       Data=Retrial number */
    HID_DEV_EVT_MODE_CHG,  /*Device changed power mode.                   Data=new power mode */
    HID_DEV_EVT_PM_FAILED,
    HID_DEV_EVT_CONTROL,   /*Host sent HID_CONTROL                       Data=Control Operation */
    HID_DEV_EVT_GET_REPORT,/*Host sent GET_REPORT                      Data=Length pdata=structure
                             having details of get-report.*/
    HID_DEV_EVT_SET_REPORT,/*Host sent SET_REPORT                      Data=Length pdata=details.*/
    HID_DEV_EVT_GET_PROTO, /*Host sent GET_PROTOCOL                       Data=NA*/
    HID_DEV_EVT_SET_PROTO, /*Host sent SET_PROTOCOL                       Data=1 for Report, 0 for Boot*/
    HID_DEV_EVT_GET_IDLE,  /*Host sent GET_IDLE                           Data=NA */
    HID_DEV_EVT_SET_IDLE,  /*Host sent SET_IDLE                           Data=Idle Rate */
    HID_DEV_EVT_DATA,
    HID_DEV_EVT_DATC,

    HID_DEV_EVT_L2CAP_CONGEST /* L2CAP channel congested */
};

/* Define the possible states of the HID Device power manager.
*/
enum
{
    HID_DEV_BUSY_CONN_ST,
    HID_DEV_IDLE_CONN_ST,
    HID_DEV_SUSP_CONN_ST
};

typedef union
{
    BD_ADDR host_bdaddr;
    BT_HDR  *buffer;
    tHID_DEV_GET_REP_DATA get_rep;
    UINT8   pm_err_code;
    UINT16  pm_interval;
} tHID_DEV_CBACK_DATA;

typedef void (tHID_DEV_CALLBACK)(UINT8    event,  /* Event from HID-DEVICE. */
                                 UINT32 data, /* Integer data corresponding to the event. */
                                 tHID_DEV_CBACK_DATA *pdata);  /* Pointer data corresponding to the event.*/

typedef struct
{
    BD_ADDR             host_addr;/* Host BD-ADDR */
    tHID_DEV_QOS_INFO   *qos_info; /* This info is used only if HID_DEV_USE_GLB_QOS is set to FALSE.*/
    tHID_DEV_CALLBACK   *app_cback;
} tHID_DEV_REG_INFO;

typedef struct pwr_md
{
    UINT16          max;
    UINT16          min;
    UINT16          attempt;
    UINT16          timeout;
    UINT8           mode;
} tHID_DEV_PM_PWR_MD;


/*****************************************************************************
**  External Function Declarations
*****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
**
** Function         HID_DevSetSDPRecord
**
** Description      This function should be called at startup to create the
**                  device SDP record
**
** Returns          0 if error else sdp handle for the record.
**
*******************************************************************************/
HID_API extern UINT32 HID_DevSetSDPRecord(tHID_DEV_SDP_INFO *p_sdp_info);

/*******************************************************************************
**
** Function         HID_DevRegister
**
** Description      This function must be called at startup so the device receive
**                  HID related events and call other HID API Calls.
**
** Returns          tHID_STATUS
**
*******************************************************************************/
HID_API extern tHID_STATUS HID_DevRegister(tHID_DEV_REG_INFO *pHID_DEVRegInfo);

/*******************************************************************************
**
** Function         HID_DevDeregister
**
** Description      This function may be used to remove HID service records and
**                  deregister from L2CAP.
**
** Returns          tHID_STATUS
**
*******************************************************************************/
HID_API extern tHID_STATUS HID_DevDeregister(void);

/*******************************************************************************
**
** Function         HID_DevConnect
**
** Description      This function may be used to initiate a connection to the host..
**
** Returns          tHID_STATUS
**
*******************************************************************************/
HID_API extern tHID_STATUS HID_DevConnect(void);

/*******************************************************************************
**
** Function         HID_DevDisconnect
**
** Description      This function may be used to disconnect from the host.
**
** Returns          tHID_STATUS
**
*******************************************************************************/
HID_API extern tHID_STATUS HID_DevDisconnect(void);

/*******************************************************************************
**
** Function         HID_DevHandShake
**
** Description      This function may be used to send HAND-SHAKE to host.
**
** Returns          tHID_STATUS
**
*******************************************************************************/
HID_API extern tHID_STATUS HID_DevHandShake(UINT8 res_code);

/*******************************************************************************
**
** Function         HID_DevVirtualUnplug
**
** Description      This function may be used to send VIRTUAL-UNPLUG to host.
**
** Returns          tHID_STATUS
**
*******************************************************************************/
HID_API extern tHID_STATUS HID_DevVirtualUnplug(void);

/*******************************************************************************
**
** Function         HID_DevSendData
**
** Description      This function may be used to send input reports to host.
**
** Returns          tHID_STATUS
**
*******************************************************************************/
HID_API extern tHID_STATUS HID_DevSendData(BOOLEAN control_ch, UINT8 rep_type,
        BT_HDR *data_buf);

/*******************************************************************************
**
** Function         HID_DevSetPowerMgmtParams
**
** Description      This function may be used to change power mgmt parameters.
**
** Returns          tHID_STATUS
**
*******************************************************************************/
HID_API extern tHID_STATUS HID_DevSetPowerMgmtParams(UINT8 conn_substate,
        tHID_DEV_PM_PWR_MD pm_params);

/*******************************************************************************
**
** Function         HID_DevInit
**
** Description      This function initializes the control block and trace variable
**
** Returns          void
**
*******************************************************************************/
HID_API extern void HID_DevInit(void);

/*******************************************************************************
**
** Function         HID_DevSetSecurityLevel
**
** Description      This function set security level for the Hid Device service.
**
** Returns          tHID_STATUS
**
*******************************************************************************/
HID_API extern tHID_STATUS HID_DevSetSecurityLevel(char serv_name[], UINT8 sec_lvl);


#ifdef __cplusplus
}
#endif

#endif  /* HIDD_API_H */
