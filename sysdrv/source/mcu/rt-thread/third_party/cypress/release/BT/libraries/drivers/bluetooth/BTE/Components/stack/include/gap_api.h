/*****************************************************************************
**
**  Name:           gap_api.h
**
**  Description:    this file contains the Generic Access Profile API
**                  definitions
**
**  Copyright (c) 1999-2014, Broadcom Corp., All Rights Reserved.
**  Broadcom Bluetooth Core. Proprietary and confidential.
******************************************************************************/
#ifndef GAP_API_H
#define GAP_API_H

#include "profiles_api.h"
#include "btm_api.h"
#include "l2c_api.h"

/*****************************************************************************
**  Constants
*****************************************************************************/

/*** GAP Error and Status Codes ***/
#define GAP_UNSUPPORTED     (GAP_ERR_GRP + 0x01)    /* Unsupported call */
#define GAP_EOINQDB         (GAP_ERR_GRP + 0x02)    /* End of inquiry database marker */
#define GAP_ERR_BUSY        (GAP_ERR_GRP + 0x03)    /* The requested function was busy */
#define GAP_ERR_NO_CTRL_BLK (GAP_ERR_GRP + 0x04)    /* No control blocks available */
#define GAP_ERR_STARTING_CMD (GAP_ERR_GRP + 0x05)   /* Error occurred while initiating the command */
#define GAP_NO_BDADDR_REC   (GAP_ERR_GRP + 0x06)    /* No Inquiry DB record for BD_ADDR */
#define GAP_ERR_ILL_MODE    (GAP_ERR_GRP + 0x07)    /* An illegal mode parameter was detected */
#define GAP_ERR_ILL_INQ_TIME (GAP_ERR_GRP + 0x08)   /* An illegal time parameter was detected */
#define GAP_ERR_ILL_PARM     (GAP_ERR_GRP + 0x09)   /* An illegal parameter was detected */
#define GAP_ERR_REM_NAME    (GAP_ERR_GRP + 0x0a)    /* Error starting the remote device name request */
#define GAP_CMD_INITIATED   (GAP_ERR_GRP + 0x0b)    /* The GAP command was started (result pending) */
#define GAP_DEVICE_NOT_UP   (GAP_ERR_GRP + 0x0c)    /* The device was not up; the request was not executed */
#define GAP_BAD_BD_ADDR     (GAP_ERR_GRP + 0x0d)    /* The bd addr passed in was not found or invalid */

#define GAP_ERR_BAD_HANDLE  (GAP_ERR_GRP + 0x0e)    /* Bad GAP handle                       */
#define GAP_ERR_BUF_OFFSET  (GAP_ERR_GRP + 0x0f)    /* Buffer offset invalid                */
#define GAP_ERR_BAD_STATE   (GAP_ERR_GRP + 0x10)    /* Connection is in invalid state       */
#define GAP_NO_DATA_AVAIL   (GAP_ERR_GRP + 0x11)    /* No data available                    */
#define GAP_ERR_CONGESTED   (GAP_ERR_GRP + 0x12)    /* BT stack is congested                */
#define GAP_ERR_SECURITY    (GAP_ERR_GRP + 0x13)    /* Security failed                      */

#define GAP_ERR_PROCESSING  (GAP_ERR_GRP + 0x14)    /* General error processing BTM request */
#define GAP_ERR_TIMEOUT     (GAP_ERR_GRP + 0x15)    /* Timeout occurred while processing cmd */

/** GAP Events - definitions of GAP return events ***/
#define GAP_EVT_INQUIRY_RESULTS     0x0001
#define GAP_EVT_INQUIRY_COMPLETE    0x0002
#define GAP_EVT_DISCOVERY_COMPLETE  0x0003
#define GAP_EVT_REM_NAME_COMPLETE   0x0004
#define GAP_EVT_FIND_ADDR_COMPLETE  0x0005

#define GAP_EVT_CONN_OPENED         0x0100
#define GAP_EVT_CONN_CLOSED         0x0101
#define GAP_EVT_CONN_DATA_AVAIL     0x0102
#define GAP_EVT_CONN_CONGESTED      0x0103
#define GAP_EVT_CONN_UNCONGESTED    0x0104

/* Values for 'chan_mode_mask' field */
/* GAP_ConnOpen() - optional channels to negotiate */
#define GAP_FCR_CHAN_OPT_BASIC      L2CAP_FCR_CHAN_OPT_BASIC
#define GAP_FCR_CHAN_OPT_ERTM       L2CAP_FCR_CHAN_OPT_ERTM
#define GAP_FCR_CHAN_OPT_STREAM     L2CAP_FCR_CHAN_OPT_STREAM

/*** discovery of devices ***/
#define GAP_INQUIRY_NONE            BTM_INQUIRY_NONE
#define GAP_GENERAL_INQUIRY         BTM_GENERAL_INQUIRY
#define GAP_LIMITED_INQUIRY         BTM_LIMITED_INQUIRY

#if BLE_INCLUDED == TRUE
#define GAP_BLE_INQUIRY_NONE        BTM_BLE_INQUIRY_NONE
#define GAP_BLE_GENERAL_INQUIRY     BTM_BLE_GENERAL_INQUIRY
#define GAP_BLE_LIMITED_INQUIRY     BTM_BLE_LIMITED_INQUIRY
#endif

/*** discoverable modes ***/
#define GAP_NON_DISCOVERABLE        BTM_NON_DISCOVERABLE
#define GAP_LIMITED_DISCOVERABLE    BTM_LIMITED_DISCOVERABLE
#define GAP_GENERAL_DISCOVERABLE    BTM_GENERAL_DISCOVERABLE

/*** Inquiry Filter Condition types  (The values are defined in btm_api.h) ***/
#define GAP_CLR_INQUIRY_FILTER      BTM_CLR_INQUIRY_FILTER          /* Inquiry Filtering is turned off */
#define GAP_FILTER_COND_DEVICE_CLASS BTM_FILTER_COND_DEVICE_CLASS    /* Filter on device class */
#define GAP_FILTER_COND_BD_ADDR     BTM_FILTER_COND_BD_ADDR         /* Filter on device addr */

/*** connectability ***/
#define GAP_NON_CONNECTABLE         BTM_NON_CONNECTABLE
#define GAP_CONNECTABLE             BTM_CONNECTABLE

/*** security features ***/
#define GAP_DISALLOW_PAIRING        0
#define GAP_ALLOW_PAIRING           1

/*** class of device settings ***/
#define GAP_SET_COD_MAJOR_MINOR     0x01
#define GAP_SET_COD_SERVICE_CLASS   0x02 /* only set the bits in the input */
#define GAP_CLR_COD_SERVICE_CLASS   0x04
#define GAP_SET_COD_ALL             0x08 /* take service class as the input (may clear some set bits!!) */
#define GAP_INIT_COD                0x0a

/*** used in connection variables and functions ***/
#define GAP_INVALID_HANDLE      0xFFFF

/* This is used to change the criteria for AMP  */
#define GAP_PROTOCOL_ID         (UUID_PROTOCOL_UDP)

/* slave preferred parameter, minimum connection interval */
#ifndef GAP_SL_CONN_INT_MIN
#define GAP_SL_CONN_INT_MIN     6
#endif
/* slave preferred parameter, maximum connection interval */
#ifndef GAP_SL_CONN_INT_MAX
#define GAP_SL_CONN_INT_MAX     20
#endif
/* slave preferred parameter */
#ifndef GAP_SL_LATENCY
#define GAP_SL_LATENCY     2
#endif

#ifndef GAP_BLE_PRIVACY_RECONN_ADDR_PERM
#define GAP_BLE_PRIVACY_RECONN_ADDR_PERM        (GATT_PERM_READ|GATT_PERM_WRITE)
#endif

#ifndef  GAP_PREFER_CONN_INT_MAX
#define  GAP_PREFER_CONN_INT_MAX         BTM_BLE_CONN_INT_MIN
#endif

#ifndef  GAP_PREFER_CONN_INT_MIN
#define  GAP_PREFER_CONN_INT_MIN         BTM_BLE_CONN_INT_MIN
#endif

#ifndef  GAP_PREFER_CONN_LATENCY
#define  GAP_PREFER_CONN_LATENCY         0
#endif

#ifndef  GAP_PREFER_CONN_SP_TOUT
#define  GAP_PREFER_CONN_SP_TOUT         2000
#endif

#if BLE_INCLUDED == TRUE
#ifndef GAP_TRANSPORT_SUPPORTED
#define GAP_TRANSPORT_SUPPORTED GATT_TRANSPORT_LE_BR_EDR
#endif
#endif

/*****************************************************************************
**  Type Definitions
*****************************************************************************/
/*
** Callback function for connection services
*/
typedef void (tGAP_CONN_CALLBACK)(UINT16 gap_handle, UINT16 event);

/*
** Define the callback function prototypes.  Parameters are specific
** to each event and are described below
*/
typedef void (tGAP_CALLBACK)(UINT16 event, void *p_data);

typedef tBTM_REMOTE_DEV_NAME    tGAP_REMOTE_DEV_NAME;   /* see btm_api.h */
typedef tBTM_INQ_PARMS          tGAP_INQ_PARMS;         /* see btm_api.h */
typedef tBTM_INQ_RESULTS        tGAP_INQ_RESULTS;       /* see btm_api.h */

/* Inquiry results structure */
typedef struct
{
    UINT16  status;
    UINT8   num_results;
} tGAP_INQ_CMPL;

/* Definition of the GAP_FindAddrByName results structure */
typedef struct
{
    UINT16       status;
    BD_ADDR      bd_addr;
    tBTM_BD_NAME devname;
} tGAP_FINDADDR_RESULTS;

/** for GAP_SetDeviceClass() **/
/* Define Class of Device related values
 *
 * >>> changes to this type need to also be made to tHSP_COD in hsp2_int.h
 */
typedef struct
{
    UINT8       minor;
    UINT8       major;
    UINT16      service;
} tGAP_COD;

/*** Constants and functions for device features ***/
typedef struct
{
    UINT8   lmp_version;
    UINT16  mfc_name;
    UINT16  lmp_subversion;
} tGAP_LMP_VERSION;

typedef struct
{
    UINT8   lmp_features[8];
} tGAP_LMP_FEATURES;

typedef struct
{
    UINT16      int_min;
    UINT16      int_max;
    UINT16      latency;
    UINT16      sp_tout;
} tGAP_BLE_PREF_PARAM;

typedef union
{
    tGAP_BLE_PREF_PARAM     conn_param;
    BD_ADDR                 reconn_bda;
    UINT16                  icon;
    UINT8                   *p_dev_name;
    UINT8                   addr_resolution;

} tGAP_BLE_ATTR_VALUE;

typedef void (tGAP_BLE_CMPL_CBACK)(BOOLEAN status, BD_ADDR addr, UINT16 length, char *p_name);


/*****************************************************************************
**  External Function Declarations
*****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
**
** Function         GAP_SetDiscoverableMode
**
** Description      This function is called to allow or disallow a service to
**                  discovered (Inquiry Scans).
**
** Returns          BT_PASS (0) if successful,
**                  GAP_ERR_ILL_PARM if a bad parameter is detected,
**                  GAP_DEVICE_NOT_UP if the device is not active,
**                  GAP_ERR_PROCESSING if not enough resources to carry out request
**
*******************************************************************************/
GAP_API extern UINT16 GAP_SetDiscoverableMode(UINT8 mode, UINT16 duration,
        UINT16 interval);

/*******************************************************************************
**
** Function         GAP_ReadDiscoverableMode
**
** Description      This function is called to retrieve the current discoverable
**                  mode for the local device.
**
** Returns          GAP_NON_DISCOVERABLE, GAP_LIMITED_DISCOVERABLE, or
**                  GAP_GENERAL_DISCOVERABLE
**
*******************************************************************************/
GAP_API extern UINT16 GAP_ReadDiscoverableMode(UINT16 *duration, UINT16 *interval);

/*******************************************************************************
**
** Function         GAP_StartInquiry
**
** Description      This function initiates a single inquiry.
**
** Returns          BT_PASS (0) if successful,
**                  GAP_ERR_ILL_MODE if a bad mode parameter was passed
**                  GAP_ERR_ILL_INQ_TIME if a bad interval or duration was passed
**                  GAP_ERR_NO_CTRL_BLK if out of control blocks
**                  GAP_ERR_ILL_PARM if a bad parameter was detected in BTM
**                  GAP_ERR_BUSY if the device already has an iquiry active
**                  GAP_DEVICE_NOT_UP if the device is not initialized yet
**                  GAP_ERR_PROCESSING if any other BTM error was returned
**
*******************************************************************************/
GAP_API extern UINT16 GAP_StartInquiry(tGAP_INQ_PARMS *p_inq_parms,
                                       tGAP_CALLBACK *p_results_cb,
                                       tGAP_CALLBACK *p_cmpl_cb);

/*******************************************************************************
**
** Function         GAP_StartPeriodicInquiry
**
** Description      This function initiates a periodic inquiry.
**
** Returns          BT_PASS (0) if successful,
**                  GAP_ERR_ILL_MODE if a bad mode parameter was passed
**                  GAP_ERR_ILL_INQ_TIME if a bad interval or duration was passed
**                  GAP_ERR_NO_CTRL_BLK if out of control blocks
**                  GAP_ERR_ILL_PARM if a bad parameter was detected in BTM
**                  GAP_ERR_BUSY if the device already has an iquiry active
**                  GAP_DEVICE_NOT_UP if the device is not initialized yet
**                  GAP_ERR_PROCESSING if any other BTM error was returned
**
*******************************************************************************/
GAP_API extern UINT16 GAP_StartPeriodicInquiry(tGAP_INQ_PARMS *p_inq_parms,
        UINT16 min_time,
        UINT16 max_time,
        tGAP_CALLBACK *p_results_cb);

/*******************************************************************************
**
** Function         GAP_CancelInquiry
**
** Description      This function cancels a single inquiry (if in progress)
**
** Returns          BOOLEAN (TRUE if successful, otherwise FALSE)
**
*******************************************************************************/
GAP_API extern UINT16 GAP_CancelInquiry(void);

/*******************************************************************************
**
** Function         GAP_CancelPeriodicInquiry
**
** Description      This function cancels a periodic inquiry (if in progress)
**
** Returns          BOOLEAN: (TRUE if successful, otherwise FALSE)
**
*******************************************************************************/
GAP_API extern UINT16 GAP_CancelPeriodicInquiry(void);

/*******************************************************************************
**
** Function         GAP_GetFirstInquiryResult
**
** Description      This function retrieves the first valid inquiry result.
**
** Returns          BT_PASS (0) if successful, or a non-zero error code
**                  GAP_EOINQDB if no more entries in the database.
**
*******************************************************************************/
GAP_API extern UINT16 GAP_GetFirstInquiryResult(tGAP_INQ_RESULTS *p_results);

/*******************************************************************************
**
** Function         GAP_GetNextInquiryResult
**
** Description      This function retrieves the next valid inquiry result.
**
** Returns          BT_PASS (0) if successful, or a non-zero status code
**                  GAP_EOINQDB if no more entries in the database.
**
*******************************************************************************/
GAP_API extern UINT16 GAP_GetNextInquiryResult(tGAP_INQ_RESULTS *p_results);

/*******************************************************************************
**
** Function         GAP_FindAddrByName
**
** Description      This function is called to retrieve a device address given
**                  a device name.  It first looks in the current local inquiry
**                  database for the device with the specified name.  If not found
**                  it initiates a general inquiry.  Upon completion, it retrieves
**                  the name for each device until a match is found or all devices
**                  have been checked.  Note:  This process can take a while to
**                  complete.
**
** Returns          BT_PASS if the name was immediately available. (BD_ADDR is returned)
**                  GAP_CMD_INITIATED if an inquiry has been initiated
**
*******************************************************************************/
GAP_API extern UINT16 GAP_FindAddrByName(BD_NAME devname,
        tGAP_INQ_PARMS *p_inq_parms,
        tGAP_CALLBACK *p_addr_cb,
        BD_ADDR bd_addr);

/*******************************************************************************
**
** Function         GAP_SetConnectableMode
**
** Description      This function is called to allow or disallow a
**                  connections on the local device.
**
** Returns          BT_PASS (0) if successful,
**                  GAP_ERR_ILL_PARM if a bad parameter is detected,
**                  GAP_DEVICE_NOT_UP if the device is not active,
**                  GAP_ERR_PROCESSING if not enough resources to carry out request
**
*******************************************************************************/
GAP_API extern UINT16 GAP_SetConnectableMode(UINT8 mode, UINT16 duration,
        UINT16 interval);

/*******************************************************************************
**
** Function         GAP_ReadConnectableMode
**
** Description      This function is called to retrieve the current connectability
**                  mode for the local device.
**
** Returns          GAP_NON_CONNECTABLE, GAP_CONNECTABLE
**
*******************************************************************************/
GAP_API extern UINT16 GAP_ReadConnectableMode(UINT16 *duration, UINT16 *interval);

/*******************************************************************************
**
** Function         GAP_SetSecurityMode
**
** Description      Set security mode for the device (Service or Link level)
**
** Returns          void
**
*******************************************************************************/
GAP_API extern void GAP_SetSecurityMode(UINT8 sec_mode);

#ifndef BTEWICED
/*******************************************************************************
**
** Function         GAP_SecRegister
**
** Description      Application calls this function to register for
**                  security services.
**
** Returns          TRUE if registered OK, else FALSE
**
*******************************************************************************/
GAP_API extern BOOLEAN GAP_SecRegister(tBTM_APPL_INFO *p_cb_info);
#endif

/*******************************************************************************
**
** Function         GAP_SetPairableMode
**
** Description      This function is called to allow or disallow pairing
**                  on the local device.
**
** Returns          BT_PASS (0) if successful, or a non-zero error code
**
*******************************************************************************/
GAP_API extern UINT16 GAP_SetPairableMode(UINT16 mode, BOOLEAN connect_only_paired);

/*******************************************************************************
**
** Function         GAP_Bond
**
** Description      This function is called to initiate bonding with peer device
**
** Returns          tBTM_STATUS - BTM_CMD_STARTED of successfully initiated
**
*******************************************************************************/
GAP_API extern UINT8 GAP_Bond(BD_ADDR bd_addr, UINT8 pin_len,
                              UINT8 *p_pin, UINT32 trusted_mask[]);

/*******************************************************************************
**
** Function         GAP_PinRsp
**
** Description      This function is called from UI after Security Manager submitted
**                  PIN code request.
**
** Returns          void
**
*******************************************************************************/
GAP_API extern void GAP_PinRsp(BD_ADDR bd_addr, UINT8 res, UINT8 pin_len, UINT8 *p_pin);

/*******************************************************************************
**
** Function         GAP_AuthorizeRsp
**
** Description      This function is called from UI after Security Manager submitted
**                  an authorization request.
**
** Returns          void
**
*******************************************************************************/
GAP_API extern void GAP_AuthorizeRsp(BD_ADDR bd_addr, UINT8 res,
                                     UINT32 trusted_mask[]);

/*******************************************************************************
**
** Function         GAP_SetDeviceClass
**
** Description      This function updates the local Device Class.
**
** Returns          BT_PASS (0) if successful,
**                  GAP_ERR_BUSY if a discovery is already in progress
**                  GAP_ERR_ILL_PARM if an illegal parameter was detected
**                  GAP_ERR_PROCESSING if any other BTM error has been returned
**
*******************************************************************************/
GAP_API extern UINT16 GAP_SetDeviceClass(tGAP_COD *p_cod, UINT8 cmd);

/*******************************************************************************
**
** Function         GAP_ReadDeviceClass
**
** Description      This function reads the current local Device Class setting.
**
** Returns          BT_PASS
**
*******************************************************************************/
GAP_API extern UINT16 GAP_ReadDeviceClass(tGAP_COD *p_cod);

/*******************************************************************************
**
** Function         GAP_ReadLocalDeviceInfo
**
** Description      This function retrieves local device information to the caller.
**                  NOTE:  Return parameters that are set to NULL are not retrieved.
**
** Returns          BT_PASS (0) if successful, or a non-zero error code
**
*******************************************************************************/
GAP_API extern UINT16 GAP_ReadLocalDeviceInfo(UINT8 *name, BD_ADDR *addr,
        tGAP_LMP_VERSION *verinfo,
        tGAP_LMP_FEATURES *features);


/*******************************************************************************
**
** Function         GAP_GetRemoteDeviceName
**
** Description      The remote name is retrieved from the specified remote device.  If
**                  GAP_CMD_INITIATED is returned by the function, the command was
**                  successfully sent to the controller.  The GAP_EVT_NAME_RESP event
**                  is passed in the callback when the remote device name has been retrieved.
**
** Returns
**                  GAP_CMD_INITIATED if remote search successfully initiated
**                  GAP_ERR_BUSY if a remote name request is already in progress,
**                  GAP_ERR_NO_CTRL_BLK if out of control blocks (too many commands pending)
**                  GAP_BAD_BD_ADDR if the device address is bad,
**                  GAP_DEVICE_NOT_UP if the device has not been initialized yet
**                  GAP_ERR_PROCESSING if any other BTM error has been returned
**
*******************************************************************************/
GAP_API extern UINT16 GAP_GetRemoteDeviceName(BD_ADDR addr, tGAP_CALLBACK *callback);

/*** Functions for L2CAP connection interface ***/
/*******************************************************************************
**
** Function         GAP_ConnOpen
**
** Description      This function is called to open a generic L2CAP connection.
**
** Returns          handle of the connection if successful, else GAP_INVALID_HANDLE
**
*******************************************************************************/
GAP_API extern UINT16 GAP_ConnOpen(char *p_serv_name, UINT8 service_id, BOOLEAN is_server,
                                   BD_ADDR p_rem_bda, UINT16 psm, tL2CAP_CFG_INFO *p_cfg,
                                   UINT16 security, UINT8 chan_mode_mask, tGAP_CONN_CALLBACK *p_cb);

/*******************************************************************************
**
** Function         GAP_ConnClose
**
** Description      This function is called to close a connection.
**
** Returns          BT_PASS             - closed OK
**                  GAP_ERR_BAD_HANDLE  - invalid handle
**
*******************************************************************************/
GAP_API extern UINT16 GAP_ConnClose(UINT16 gap_handle);

/*******************************************************************************
**
** Function         GAP_ConnReadData
**
** Description      GKI buffer unaware application will call this function
**                  after receiving GAP_EVT_RXDATA event. A data copy is made
**                  into the receive buffer parameter.
**
** Returns          BT_PASS             - data read
**                  GAP_ERR_BAD_HANDLE  - invalid handle
**                  GAP_NO_DATA_AVAIL   - no data available
**
*******************************************************************************/
GAP_API extern UINT16 GAP_ConnReadData(UINT16 gap_handle, UINT8 *p_data,
                                       UINT16 max_len, UINT16 *p_len);

/*******************************************************************************
**
** Function         GAP_GetRxQueueCnt
**
** Description      This function return number of bytes on the rx queue.
**
** Parameters:      handle     - Handle returned in the GAP_ConnOpen
**                  p_rx_queue_count - Pointer to return queue count in.
**
**
*******************************************************************************/
GAP_API extern int GAP_GetRxQueueCnt(UINT16 handle, UINT32 *p_rx_queue_count);

/*******************************************************************************
**
** Function         GAP_ConnBTRead
**
** Description      GKI buffer aware applications will call this function after
**                  receiving an GAP_EVT_RXDATA event to process the incoming
**                  data buffer.
**
** Returns          BT_PASS             - data read
**                  GAP_ERR_BAD_HANDLE  - invalid handle
**                  GAP_NO_DATA_AVAIL   - no data available
**
*******************************************************************************/
GAP_API extern UINT16 GAP_ConnBTRead(UINT16 gap_handle, BT_HDR **pp_buf);

/*******************************************************************************
**
** Function         GAP_ConnBTWrite
**
** Description      GKI buffer aware applications can call this function to write data
**                  by passing a pointer to the GKI buffer of data.
**
** Returns          BT_PASS                 - data read
**                  GAP_ERR_BAD_HANDLE      - invalid handle
**                  GAP_ERR_BAD_STATE       - connection not established
**                  GAP_INVALID_BUF_OFFSET  - buffer offset is invalid
*******************************************************************************/
GAP_API extern UINT16 GAP_ConnBTWrite(UINT16 gap_handle, BT_HDR *p_buf);

/*******************************************************************************
**
** Function         GAP_ConnWriteData
**
** Description      GKI buffer unaware application will call this function
**                  to send data to the connection. A data copy is made into a GKI
**                  buffer.
**
** Returns          BT_PASS                 - data read
**                  GAP_ERR_BAD_HANDLE      - invalid handle
**                  GAP_ERR_BAD_STATE       - connection not established
**                  GAP_CONGESTION          - system is congested
**
*******************************************************************************/
GAP_API extern UINT16 GAP_ConnWriteData(UINT16 gap_handle, UINT8 *p_data,
                                        UINT16 max_len, UINT16 *p_len);

/*******************************************************************************
**
** Function         GAP_ConnReconfig
**
** Description      Applications can call this function to reconfigure the connection.
**
** Returns          BT_PASS                 - config process started
**                  GAP_ERR_BAD_HANDLE      - invalid handle
**
*******************************************************************************/
GAP_API extern UINT16 GAP_ConnReconfig(UINT16 gap_handle, tL2CAP_CFG_INFO *p_cfg);

/*******************************************************************************
**
** Function         GAP_ConnSetIdleTimeout
**
** Description      Higher layers call this function to set the idle timeout for
**                  a connection, or for all future connections. The "idle timeout"
**                  is the amount of time that a connection can remain up with
**                  no L2CAP channels on it. A timeout of zero means that the
**                  connection will be torn down immediately when the last channel
**                  is removed. A timeout of 0xFFFF means no timeout. Values are
**                  in seconds.
**
** Returns          BT_PASS                 - config process started
**                  GAP_ERR_BAD_HANDLE      - invalid handle
**
*******************************************************************************/
GAP_API extern UINT16 GAP_ConnSetIdleTimeout(UINT16 gap_handle, UINT16 timeout);

/*******************************************************************************
**
** Function         GAP_ConnGetRemoteAddr
**
** Description      This function is called to get the remote BD address
**                  of a connection.
**
** Returns          BT_PASS             - closed OK
**                  GAP_ERR_BAD_HANDLE  - invalid handle
**
*******************************************************************************/
GAP_API extern UINT8 *GAP_ConnGetRemoteAddr(UINT16 gap_handle);

/*******************************************************************************
**
** Function         GAP_ConnGetRemMtuSize
**
** Description      Returns the remote device's MTU size.
**
** Returns          UINT16 - maximum size buffer that can be transmitted to the peer
**
*******************************************************************************/
GAP_API extern UINT16 GAP_ConnGetRemMtuSize(UINT16 gap_handle);

/*******************************************************************************
**
** Function         GAP_ConnGetL2CAPCid
**
** Description      Returns the L2CAP channel id
**
** Parameters:      handle      - Handle of the connection
**
** Returns          UINT16      - The L2CAP channel id
**                  0, if error
**
*******************************************************************************/
GAP_API extern UINT16 GAP_ConnGetL2CAPCid(UINT16 gap_handle);

/*******************************************************************************
**
** Function         GAP_SetTraceLevel
**
** Description      This function sets the trace level for GAP.  If called with
**                  a value of 0xFF, it simply returns the current trace level.
**
** Returns          The new or current trace level
**
*******************************************************************************/
GAP_API extern UINT8 GAP_SetTraceLevel(UINT8 new_level);

/*******************************************************************************
**
** Function         GAP_Init
**
** Description      Initializes the control blocks used by GAP.
**                  This routine should not be called except once per
**                      stack invocation.
**
** Returns          Nothing
**
*******************************************************************************/
GAP_API extern void GAP_Init(void);

#if (BLE_INCLUDED == TRUE)
/*******************************************************************************
**
** Function         GAP_BleAttrDBUpdate
**
** Description      update GAP local BLE attribute database.
**
** Returns          Nothing
**
*******************************************************************************/
GAP_API extern void GAP_BleAttrDBUpdate(UINT16 attr_uuid, tGAP_BLE_ATTR_VALUE *p_value);


/*******************************************************************************
**
** Function         GAP_BleReadPeerPrefConnParams
**
** Description      Start a process to read a connected peripheral's preferred
**                  connection parameters
**
** Returns          TRUE if read started, else FALSE if GAP is busy
**
*******************************************************************************/
GAP_API extern BOOLEAN GAP_BleReadPeerPrefConnParams(BD_ADDR peer_bda);

/*******************************************************************************
**
** Function         GAP_BleReadPeerDevName
**
** Description      Start a process to read a connected peripheral's device name.
**
** Returns          TRUE if request accepted
**
*******************************************************************************/
GAP_API extern BOOLEAN GAP_BleReadPeerDevName(BD_ADDR peer_bda, tGAP_BLE_CMPL_CBACK *p_cback);


/*******************************************************************************
**
** Function         GAP_BleReadPeerAddressResolutionCap
**
** Description      Start a process to read peer address resolution capability
**
** Returns          TRUE if request accepted
**
*******************************************************************************/
GAP_API extern BOOLEAN GAP_BleReadPeerAddressResolutionCap(BD_ADDR peer_bda,
        tGAP_BLE_CMPL_CBACK *p_cback);
/*******************************************************************************
**
** Function         GAP_BleCancelReadPeerDevName
**
** Description      Cancel reading a peripheral's device name.
**
** Returns          TRUE if request accepted
**
*******************************************************************************/
GAP_API extern BOOLEAN GAP_BleCancelReadPeerDevName(BD_ADDR peer_bda);

extern BOOLEAN gap_ble_send_brcm_notify(BD_ADDR remote_bda, UINT16 handle, UINT16 len, UINT8 notif_msg);


#endif

#ifdef __cplusplus
}
#endif

#endif  /* GAP_API_H */

