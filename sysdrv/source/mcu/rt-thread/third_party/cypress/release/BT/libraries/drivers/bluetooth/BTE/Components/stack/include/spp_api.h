/*****************************************************************************
**
**  Name:          spp_api.h
**
**  Description:   this file contains the Serial Port API definitions
**
**
**  Copyright (c) 1999-2004, WIDCOMM Inc., All Rights Reserved.
**  WIDCOMM Bluetooth Core. Proprietary and confidential.
******************************************************************************/
#ifndef SPP_API_H
#define SPP_API_H

#include "bt_target.h"
#include "gap_api.h"
#include "port_api.h"

/*****************************************************************************
**  Constants
*****************************************************************************/
#define SPP_MAX_PORT_NAME_LEN   33              /* Maximum length of the local port name */

#ifndef SPP_RFCOMM_SCN
#define SPP_RFCOMM_SCN          1
#endif

/*****************************************************************************
**  Type Definitions
*****************************************************************************/

/* The following are general status codes */
enum
{
    SPP_SUCCESS = BT_PASS,

    /* The following are errors returned by SPP */
    SPP_NO_RESOURCES = SPP_ERR_GRP,
    SPP_CMD_PENDING,                /* Request is being processed, results are passed back in a callback */
    SPP_CONN_PENDING,               /* (Client) The connection has not been established with the service (port_handle not valid) */
    SPP_NO_SERV_INQ_EN,             /* Error storing or retrieving SDP information (client and server) */
    SPP_ERR_BAD_NAME,               /* Port name specified was not found, or unusable */
    SPP_ERR_SDP_REG,                /* Could not register the service */
    SPP_ERR_SDP_ATTR,               /* Could not add attribute to SDP */
    SPP_ERR_SDP_CLASSID,            /* Could not add class id to SDP */
    SPP_ERR_SDP_PROTO,              /* Could not add protocol list to SDP */
    SPP_ERR_NOT_SUPPORTED,          /* Condition is not supported */
    SPP_ERR_NO_SCN,                 /* No SCN's available */
    SPP_ERR_NSERV,                  /* Not a server */
    SPP_ERR_SERV_EXISTS,            /* The service is already open */
    SPP_ERR_PORT_EXISTS,            /* Port name already exists */
    SPP_ERR_PORT_PROC,              /* Error returned from port processing */
    SPP_ERR_CONN_IS_OPEN,           /* The desired connection is open */
    SPP_ERR_CONN_ALREADY_CLOSED,    /* The connection being closed is not currently being used */
    SPP_ERR_DISCOVER_MODE,          /* An error occurred starting page or inq scans */
    SPP_ERR_RELEASING_SCN,          /* Error while releasing the SCN */
    SPP_ERR_REG_SECURITY,           /* Error while registering security */
    SPP_ERR_ILLEGAL_PARM,           /* Illegal Parameter */
    SPP_ERR_PORT_OPEN,              /* Error Opening RFCOMM Port */
    SPP_ERR_NO_DEVICES,             /* No devices located during inquiry */
    SPP_ERR_DISCOVERY,              /* Error occurred while initiating device or service discovery */
    SPP_ERR_DISC_BUSY               /* Discovery is already in progress */
};
typedef UINT16 tSPP_STATUS;


/* Connection status field passed in callback and tSPP_CONN_INFO */
typedef enum
{
    SPP_CONN_ERROR = 0,             /* An error occurred, look at general status for details */
    SPP_CONN_DOWN,                  /* The connection has gone down (client) */
    SPP_CONN_UP                     /* The connection is active (client and server) */
} tspp_conn_status;
typedef UINT8 tSPP_CONN_STATUS;


typedef struct
{
    tSPP_CONN_STATUS    status;
    BOOLEAN             is_open;                                /* TRUE when an application is using the connection */
    UINT16              port_handle;                            /* NULL if no RFCOMM port opened */
    UINT16              cid;                                    /* Channel ID of the connection */
    BD_ADDR             peer_bd_addr;                           /* NULL if server is not connected with a peer device */
    char                service_name[BT_MAX_SERVICE_NAME_LEN];  /* Name of the service to be stored in SDP record */
} tSPP_CONN_INFO;


/*
**  SPP_CALLBACK
**      When the connection status is SPP_CONN_ERROR, status contains the details.
**      BD_ADDR is NULL when the server is not connected to a peer device.
*/
typedef void (tSPP_CALLBACK)(char *port_name, tSPP_STATUS status, tSPP_CONN_STATUS conn_status,
                             BD_ADDR peer_bd_addr, UINT16 port_handle);


/*****************************************************************************
**  External Function Declarations
*****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
**
** Function         SPP_Listen
**
** Description      SPP_Listen is called by an application or management entity
**                  to link a virtual serial port to a Bluetooth RFCOMM channel,
**                  and place the connection into listen mode.  An application
**                  may then use the virtual serial connection by calling
**                  SPP_Open which returns the handle used to access the
**                  connection.  Two functions are used to access the serial
**                  port for systems that have a separate management entity that
**                  controls the port, and an application that uses the port.
**
**                  NOTE:   After a successful connection is made with a client,
**                          the application uses the port handle returned in the
**                          SPP_Open call to access the port. Upon receipt of the
**                          SPP_CONN_DOWN status (the result of a disconnect or
**                          other error), the port is closed (port_handle is
**                          invalid), and the connection removed.
**
** Returns          SPP_SUCCESS (0) is returned upon successful registration.
**
*******************************************************************************/
SPP_API extern tSPP_STATUS SPP_Listen(char *port_name, char *service_name,
                                      UINT16 mtu, tBT_SECURITY *security,
                                      tSPP_CALLBACK *p_conn_status);


/*******************************************************************************
**
** Function         SPP_Connect
**
** Description      This function is called by an application or management
**                  entity to link a virtual serial port to a Bluetooth RFCOMM
**                  channel, and connect to a service on a remote device.  An
**                  application may then use the virtual serial connection by
**                  calling SPP_Open which returns the handle used to access the
**                  service.  Two functions are used to access the serial port
**                  for the systems that have a separate management entity that
**                  controls the port, while an application uses the port.
**
**                  NOTE:   After a successful connection is made with a server, the application
**                          uses the port handle returned in the SPP_Open call to access the port.
**                          Upon receipt of the SPP_CONN_DOWN status (the result of a disconnect or
**                          other error), the port is closed (port_handle is invalid), and the
**                          connection removed.
**
**
** Returns      SPP_CMD_PENDING is returned upon successfully initiating service
**                              discovery.
**
*******************************************************************************/
SPP_API extern tSPP_STATUS SPP_Connect(char *port_name, BD_ADDR peer_bd_addr,
                                       char *service_name, UINT16 mtu,
                                       tBT_SECURITY *security,
                                       tSPP_CALLBACK *p_conn_status);


/*******************************************************************************
**
** Function         SPP_Remove
**
** Description      This function is called to remove a virtual serial port
**                  connection.
**
** Returns          SPP_SUCCESS if successful (No close event is returned).
**                  SPP_CMD_PENDING if the RFCOMM disconnect is being initiated
**                                  (connection open). In this case the
**                                  connection down event will occur to signify
**                                  when the connection has been closed.
**                  SPP_ERR_CONN_IS_OPEN if an application is still using the
**                                  connection.
**                  SPP_ERR_BAD_NAME if an unrecognizable port name is passed
**
*******************************************************************************/
SPP_API extern tSPP_STATUS SPP_Remove(char *port_name);


/*******************************************************************************
**
** Function         SPP_Open
**
** Description      This function is called by an application to use a virtual
**                  serial port connection that has been previously created.
**                  The application passes the port callback and event mask for
**                  the events that it wishes to receive.  If the caller is a
**                  server, OR if the caller is a client and the connection
**                  is established, the port handle is returned for use for
**                  controlling the port.  After receiving the port handle,
**                  the PORT API routines are used to access the port.
**
** Returns          port_handle (upon success) - handle used to access RFCOMM
**                  "0" if an error occurred, or the client does not have a
**                  connection to the service
**                      (check return status parameter for details)
**
*******************************************************************************/
SPP_API extern UINT16 SPP_Open(char *port_name, tPORT_CALLBACK *p_port_cb,
                               UINT32 event_mask, tSPP_STATUS *p_status);


/*******************************************************************************
**
** Function         SPP_Close
**
** Description      This function is called by an application to end a session
**                  with a virtual serial port connection.
**                  This function must be called prior to removing a serial port.
**
** Returns          SPP_SUCCESS if port has been closed for the application,
**                  SPP_ERR_BAD_NAME if unrecognized port name specified,
**                  SPP_ERR_CONN_ALREADY_CLOSED if the port is not open.
**
*******************************************************************************/
SPP_API extern tSPP_STATUS SPP_Close(char *port_name);


/*******************************************************************************
**
** Function         SPP_GetConnectionInfo
**
** Description      This function is called by an application or management
**                  entity to retrieve the current status of the serial port
**                  connection.  The name of the service, device address of
**                  the peer (If connected), and the current status of the
**                  connection are copied into the caller's memory.
**
** Returns          SPP_SUCCESS if successful
**                  SPP_ERR_BAD_NAME if port_name was not found
**
*******************************************************************************/
SPP_API extern tSPP_STATUS SPP_GetConnectionInfo(const char *port_name,
        tSPP_CONN_INFO *p_conn_info);


/*******************************************************************************
**
** Function         SPP_GetSDPServiceHandle
**
** Description      This function is called by an application or management entity to retrieve
**                  the SDP service record handle of the specified SPP connection.  It is
**                  typically called for the purpose of adding additional attributes to the
**                  service record.  It only applies to a channel that provides a service (server).
**
** Returns          SPP_SUCCESS if successful, otherwise an error code.
**
*******************************************************************************/
SPP_API extern tSPP_STATUS SPP_GetSDPServiceHandle(const char *port_name,
        UINT32 *p_handle);


/*******************************************************************************
**
** Function         SPP_Init
**
** Description      Initializes the control blocks used by SPP. It must be called
**                  before any other SPP API function is called.
**
**                  NOTE: This routine should not be called except once per
**                      stack invocation.  It assumes that the inquiry and
**                      service databases have never been allocated.
**
** Returns          void
**
*******************************************************************************/
SPP_API extern void SPP_Init(void);


/*******************************************************************************
**
** Function         SPP_SetTraceLevel
**
** Description      This function sets the trace level for SPP. If called with
**                  a value of 0xFF, it simply reads the current trace level.
**
** Returns          the new (current) trace level
**
*******************************************************************************/
SPP_API extern UINT8 SPP_SetTraceLevel(UINT8 new_level);

#ifdef __cplusplus
}
#endif

#endif  /* SPP_API_H */
