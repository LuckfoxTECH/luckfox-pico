/****************************************************************************/
/*                                                                          */
/*  Name:       hidd_int.h                                                  */
/*                                                                          */
/*  Function:   this file contains HID DEVICE internal definitions          */
/*                                                                          */
/*                                                                          */
/*  Copyright (c) 2002-2015, Broadcom Corp., All Rights Reserved.           */
/*  Broadcom Bluetooth Core. Proprietary and confidential.                  */
/****************************************************************************/

#ifndef HIDD_INT_H
#define HIDD_INT_H

#include "hid_conn.h"
#include "l2c_api.h"
#include "wiced_bt_types.h"
#include "wiced_bt_hidd.h"

/* Define the possible events of the HID Device state machine.
*/
enum
{
    HOST_CONN_OPEN,
    HOST_CONN_CLOSE,
    HOST_CONN_LOST,
    HOST_CONN_FAIL,
    HID_API_CONNECT,
    HID_API_DISCONNECT,
    HID_API_SEND_DATA
};

/* Define the possible states of the HID Device.
*/
enum
{
    HID_DEV_ST_NO_CONN,
    HID_DEV_ST_CONNECTING,
    HID_DEV_ST_CONNECTED,
    HID_DEV_ST_DISC_ING
};

/* To remember the power mode and setting */
typedef struct curr_pm_setting
{
    UINT8  mode;
    UINT16 interval;
} tHID_DEV_PM_CURR;

/* Define the HID management control block.
*/
typedef struct hid_control_block
{
    BD_ADDR        host_addr;   /* BD-Addr of the host device */
    BOOLEAN        host_known;  /* Mode */
    BOOLEAN        virtual_cable;/* If the device is to behave as virtual cable */
    UINT8          dev_state;   /* Device state if in HOST-KNOWN mode */
    UINT8          conn_tries;  /* Remembers to the number of connection attempts while CONNECTING */
    UINT8          sec_mask;
    UINT16         get_rep_buf_sz;
    tHID_CONN      conn;        /* L2CAP channel info */

#if HID_DEV_PM_INCLUDED == TRUE
    TIMER_LIST_ENT idle_tle;    /* Timer used for inactivity timing */
    wiced_bt_hidd_pm_pwr_md_t pm_params[3]; /* Power management parameters for the three possible states */
    tHID_DEV_PM_CURR   curr_pm; /* Current power mode */
    BOOLEAN        pm_ctrl_busy;/* A power mode transition is going on */
    UINT8          conn_substate;
    wiced_bt_hidd_pm_pwr_md_t final_pm;/* To remember the power mode while a power mode change is ongoing */
#endif

    BOOLEAN        use_qos_flg; /* Qos information provided by application or not */
    BOOLEAN        unplug_on;   /* Virtual unplug has been sent or received */
    FLOW_SPEC      hci_qos;     /* Storage for QoS provided by application */

    /* L2CAP registration structures */
    tL2CAP_APPL_INFO l2cap_ctrl_appl_info;
    tL2CAP_APPL_INFO l2cap_intr_appl_info;

    wiced_bt_hidd_callback_t   *callback; /* Application callbacks */
    BOOLEAN         reg_flag;
    UINT8           trace_level;
} tHIDDEV_CB;

typedef struct snd_data_params
{
    BOOLEAN ctrl_ch; /* TRUE if control channel, FALSE if interrupt */
    UINT8   trans_type; /* Transaction type */
    UINT8   param;   /* Second byte after trans type */
    BT_HDR  *buf ;   /* Data that comes after param */
} tHID_SND_DATA_PARAMS; /* Is defined for hidd_conn_snd_data */

/* HID management function prototype
*/
typedef tHID_STATUS(tHIDD_MGMT_EVT_HDLR)(UINT8, void *);

/* HID Globals
*/
#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
** Main Control Block
*******************************************************************************/
HID_API extern tHIDDEV_CB  hidd_cb;

extern tHID_STATUS hidd_conn_reg(void);
extern void hidd_conn_dereg(void);
extern tHID_STATUS hidd_conn_initiate(void);
extern void hidd_conn_disconnect(void);
extern tHID_STATUS hidd_conn_snd_data(tHID_SND_DATA_PARAMS *p_data);

extern tHID_STATUS hidd_mgmt_process_evt(UINT8 event, void *data);
extern void hidd_proc_repage_timeout(TIMER_LIST_ENT *p_tle);

#if HID_DEV_PM_INCLUDED == TRUE
extern tHID_STATUS hidd_pm_start(void) ;
extern tHID_STATUS hidd_pm_stop(void);
extern tHID_STATUS hidd_pm_activity_evt(void);
extern tHID_STATUS hidd_pm_suspend_evt(void);
extern tHID_STATUS hidd_pm_unsuspend_evt(void);
extern void hidd_pm_init(void);
extern BOOLEAN hidd_pm_set_power_mode(wiced_bt_hidd_pm_pwr_md_t *pm) ;

extern void hidd_pm_proc_mode_change(UINT8 hci_status, UINT8 mode, UINT16 interval);
#endif /* HID_DEV_PM_INCLUDED == TRUE */

#ifdef __cplusplus
}
#endif

#endif
