/************************************************************************************
**
**  Name:          dun_api.h
**
**  Description:   this file contains the DUN API declaration
**
**
**  Copyright (c) 1999-2004, WIDCOMM Inc., All Rights Reserved.
**  WIDCOMM Bluetooth Core. Proprietary and confidential.
*************************************************************************************/
#ifndef DUN_API_H
#define DUN_API_H

#include "bt_target.h"
#include "btm_api.h"
#include "sdp_api.h"
#include "port_api.h"

/*****************************************************************************
**  Constants
*****************************************************************************/

/* API function return values */
#define DUN_SUCCESS             0
#define DUN_FAIL                1

/* Values for the options parameter.  It is an integer bit mask that
** contains information used in the DUN or Fax SDP record.  These values
** can be or'ed together to indicate more than one option.  Fax class options
** are not allowed for the DUN profile.
*/
#define DUN_OPTIONS_AUDIO       (1<<0)      /* Audio feedback supported */
#define DUN_OPTIONS_CLASS1      (1<<1)      /* Fax class 1 supported */
#define DUN_OPTIONS_CLASS20     (1<<2)      /* Fax class 2.0 supported */
#define DUN_OPTIONS_CLASS2      (1<<3)      /* Fax class 2 supported */

/*****************************************************************************
**  Type definitions
*****************************************************************************/

/* This callback function returns service discovery information to the
** application after the DUN_FindService() API function is called.  The
** implementation of this callback function must copy the p_name parameter
** passed to it as it is not guaranteed to remain after the callback
** function exits.
*/
typedef void (tDUN_FIND_CBACK)(BOOLEAN found, UINT8 scn, char *p_name,
                               UINT16 name_len, UINT16 options);


/*****************************************************************************
**  External Function Declarations
*****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
**
** Function         DUN_Listen
**
** Description      This function opens a DUN or Fax connection in server mode.
**                  It configures the security settings for the connection,
**                  opens an RFCOMM connection in server mode, and sets the
**                  class of device as required by the profile.  It returns
**                  the handle for the RFCOMM connection.
**
** Returns          DUN_SUCCESS if function execution succeeded,
**                  DUN_FAIL if function execution failed.
**
******************************************************************************/
DUN_API extern UINT8 DUN_Listen(UINT16 service_uuid, char *p_service_name,
                                UINT8 scn, UINT16 mtu, UINT8 security_mask,
                                UINT16 *p_handle, tPORT_CALLBACK *p_cback);

/******************************************************************************
**
** Function         DUN_Connect
**
** Description      This function opens a DUN or Fax client connection.
**                  It configures the security settings for the connection
**                  and opens an RFCOMM connection in server mode.
**                  It returns the handle for the RFCOMM connection.
**
** Returns          DUN_SUCCESS if function execution succeeded,
**                  DUN_FAIL if function execution failed.
**
******************************************************************************/
DUN_API extern UINT8 DUN_Connect(UINT16 service_uuid, BD_ADDR bd_addr, UINT8 scn,
                                 UINT16 mtu, UINT8 security_mask, UINT16 *p_handle,
                                 tPORT_CALLBACK *p_cback);

/******************************************************************************
**
** Function         DUN_Close
**
** Description      This function closes a DUN or Fax client connection
**                  previously opened with DUN_Connect().
**
** Returns          DUN_SUCCESS if function execution succeeded,
**                  DUN_FAIL if function execution failed.
**
******************************************************************************/
DUN_API extern UINT8 DUN_Close(UINT16 handle);

/******************************************************************************
**
** Function         DUN_Shutdown
**
** Description      This function closes a DUN or Fax server connection
**                  previously opened with DUN_Listen().  It is called if
**                  the application wishes to close the DUN or Fax server
**                  connection at any time.
**
** Returns          DUN_SUCCESS if function execution succeeded,
**                  DUN_FAIL if function execution failed.
**
******************************************************************************/
DUN_API extern UINT8 DUN_Shutdown(UINT16 handle);

/******************************************************************************
**
** Function         DUN_AddRecord
**
** Description      This function is called by a server application to add
**                  DUN or Fax information to an SDP record.  Prior to
**                  calling this function the application must call
**                  SDP_CreateRecord() to create an SDP record.
**
** Returns          DUN_SUCCESS if function execution succeeded,
**                  DUN_FAIL if function execution failed.
**
******************************************************************************/
DUN_API extern UINT8 DUN_AddRecord(UINT16 service_uuid, char *p_service_name,
                                   UINT8 scn, UINT16 options, UINT32 sdp_handle);

/******************************************************************************
**
** Function         DUN_FindService
**
** Description      This function is called by a client application to
**                  perform service discovery and retrieve DUN or Fax SDP
**                  record information from a server.  Information is
**                  returned for the first service record found on the
**                  server that matches the service UUID.  The callback
**                  function will be executed when service discovery is
**                  complete.  There can only be one outstanding call to
**                  DUN_FindService() at a time; the application must wait
**                  for the callback before it makes another call to
**                  the function.
**
** Returns          DUN_SUCCESS if function execution succeeded,
**                  DUN_FAIL if function execution failed.
**
******************************************************************************/
DUN_API extern UINT8 DUN_FindService(UINT16 service_uuid, BD_ADDR bd_addr,
                                     tSDP_DISCOVERY_DB *p_db, UINT32 db_len,
                                     tDUN_FIND_CBACK *p_cback);

/******************************************************************************
**
** Function         DUN_SetTraceLevel
**
** Description      Sets the trace level for DUN. If 0xff is passed, the
**                  current trace level is returned.
**
**                  Input Parameters:
**                      new_level:  The level to set the DUN tracing to:
**                      0xff-returns the current setting.
**                      0-turns off tracing.
**                      >= 1-Errors.
**                      >= 2-Warnings.
**                      >= 3-APIs.
**                      >= 4-Events.
**                      >= 5-Debug.
**
** Returns          The new trace level or current trace level if
**                  the input parameter is 0xff.
**
******************************************************************************/
DUN_API extern UINT8 DUN_SetTraceLevel(UINT8 new_level);

/*******************************************************************************
**
** Function         DUN_Init
**
** Description      This function is called before accessing the DUN APIs in order
**                  to initialize the control block.
**
** Returns          void
**
*******************************************************************************/
DUN_API extern void DUN_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* DUN_API_H */

