/*****************************************************************************
**
**  Name:         goep_util.h
**
**  File:         Generic Object Exchange Profile utility API
**
**  Copyright (c) 2000-2008, Broadcom Corp., All Rights Reserved.
**  WIDCOMM Bluetooth Core. Proprietary and confidential.
**
*****************************************************************************/
#ifndef GOEP_UTIL_H
#define GOEP_UTIL_H

#include "bt_target.h"
#include "sdp_api.h"   /* for tSDP_DISC_REC */


/*****************************************************************************
**  Constants
*****************************************************************************/

#define GOEP_ENHANCED_VERSION       0x0102
#define GOEP_LEGACY_VERSION         0x0100


/*****************************************************************************
**  Type Definitions
*****************************************************************************/

/*** Define goep error codes ***/
enum
{
    GOEP_SUCCESS = 0,                   /* success                          */
    GOEP_ERROR,                         /* general failure                  */
    GOEP_RESOURCES,                     /* resources not available          */
    GOEP_INVALID_PARAM                  /* bad parameter(s)                 */
} ;
typedef UINT8 tGOEP_ERRORS;





/*****************************************************************************
**  External Function Declarations
*****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/*** GOEP utility functions ***/
/*****************************************************************************
**
**  Function        GOEP_SetSecurityLevel()
**
**  Description     Register security & encryption level for GOEP server.
**                  This is not to be used for the GOEP command server.
**
**  Returns         TRUE if OK, FALSE if a bad parameter
**
*****************************************************************************/
GOEP_API extern BOOLEAN GOEP_SetSecurityLevel(BOOLEAN b_orig,
        char    *p_sz_name,
        UINT32  service,
        UINT8   sec,
        UINT8   scn);

/*****************************************************************************
**
**  Function        GOEP_SetTraceLevel
**
**  Description     This function changes the trace level (debugging levels)
**
**  Returns         void
**
*****************************************************************************/
GOEP_API extern void GOEP_SetTraceLevel(UINT8 level);

/*******************************************************************************
**
** Function         GOEP_FreeBuf
**
** Description      free memory for specified packet
**
** Returns          void
**
*******************************************************************************/
GOEP_API extern void GOEP_FreeBuf(void **p_buf);

/*******************************************************************************
**
** Function         GOEP_SendMsg
**
** Description      send a message to BTU task
**
** Returns          void
**
*******************************************************************************/
GOEP_API extern void GOEP_SendMsg(void *p_msg);

/*******************************************************************************
**
** Function         GOEP_Init
**
** Description      This function is called once at stack startup to allocate
**                  (if using dynamic memory) and initialize the
**                  control block, and initializes the
**                  debug tracing level.
**
** Returns          void
**
*******************************************************************************/
GOEP_API extern void GOEP_Init(void);



/***    GOEP Service functions ***/


/*****************************************************************************
**
**  Function:    GOEP_Register()
**
**  Purpose:     Register an OBEX profile with SDP
**
**  Parameters:
**
**       char *p_name   service name;  optional (may be NULL)
**       UINT32 *phSDP  handle to the created record;  cannot be NULL
**       UINT8 scn      scn desired for the service;  must be > 0
**       UINT16 version version of the service;  optional (may be 0)
**
**  Returns:     (tGOEP_ERRORS) GOEP_SUCCESS if ok;  GOEP_ERROR on error
**
**  Notes:
**
**
**  Preconditions:
**    - phSDP must be set (to return the SDP record handle)
**    - scn must be set
**
**  Postconditions:
**    -
**
*****************************************************************************/
GOEP_API extern tGOEP_ERRORS GOEP_Register(char   *p_name,
        UINT32 *p_sdp_handle,
        UINT8  scn,
        UINT8  num_srv_class,
        UINT16 *p_service_class,
        UINT16 profile_id,
        UINT16 version);
/*****************************************************************************
**
**  Function:    GOEP_Register2()
**
**  Purpose:     Register an OBEX 1.5 profile with SDP
**
**  Parameters:
**
**       char *p_name   service name;  optional (may be NULL)
**       UINT32 *phSDP  handle to the created record;  cannot be NULL
**       UINT16 psm     psm desired for the service;  must be an legal L2CAP dynamic PSM
**       UINT16 version version of the service;  optional (may be 0)
**
**  Returns:     (tGOEP_ERRORS) GOEP_SUCCESS if ok;  GOEP_ERROR on error
**
**  Notes:
**
**
**  Preconditions:
**    - phSDP must be set (to return the SDP record handle)
**    - psm must be set
**
**  Postconditions:
**    -
**
*****************************************************************************/
GOEP_API extern tGOEP_ERRORS GOEP_Register2(char   *p_name,
        UINT32 *phSDP,
        UINT16 psm,
        UINT8  num_srv_class,
        UINT16 *p_service_class,
        UINT16 profile_id,
        UINT16 version);

/*****************************************************************************
**
**  Function:    GOEP_AddProtoLists()
**
**  Purpose:     Add the AdditionalProtocolDescriptorLists attribute
**               to a SDP record
**
**  Parameters:
**
**       UINT32 sdp_hdl         the SDP record handle
**       UINT8  scn             scn desired for the service;  must be > 0
**
**  Returns:     (tGOEP_ERRORS) GOEP_SUCCESS if ok;  GOEP_ERROR on error
**
**  Notes:
**
**
**  Preconditions:
**    - sdp_hdl must be set (to the SDP record handle from GOEP_Register())
**    - scn must be set
**
**  Postconditions:
**    -
**
*****************************************************************************/
GOEP_API extern tGOEP_ERRORS GOEP_AddProtoLists(UINT32 sdp_hdl, UINT8  scn);



/*** Debug Tracing support ***/
#if BT_USE_TRACES == TRUE

GOEP_API extern char *GOEP_ErrorName(tGOEP_ERRORS error);
GOEP_API extern void GOEP_TraceSupportedDataTypes(UINT8 data_types, UINT8 *p_data_types);
#else
#ifndef BTE_SIM_APP
/* define these trace functions out */
#define GOEP_CodeName(_x) ""
#define GOEP_ErrorName(_x) ""
#endif
#define GOEP_TraceSupportedDataTypes(x, y)
#endif /* end if BT_TRACE_PROTOCOL */


/*****************************************************************************
**
**  Design notes:
**
**  The responsibilities handle by this module include:
**
**  a)  Function signatures which assure the proper header fields are
**        set for the GOEP connection establishment, GOEP put, and GOEP get.
**        Required fields / headers are filled in through the API function
**        parameters.  Extensions beyond the required fields / headers
**        must be set through the tOBEX_HEADERS *p_headers parameter.
**
**  b)  Debug error checking for required fields and prohibited fields
**        for the GOEP connection establishment, GOEP put, and GOEP get.
**        With debugging on, non-optional headers set in the p_headers
**        parameter will generate a Widcomm assertion error.
**
**  c)  Does NOT include checks of any kind on the contents of the
**        authentication or authentication response headers.  In fact,
**        the logic for encoding, decoding, validating, and calculating
**        authentication data is outside the scope of GOEP and OBEX.
**        Refer to IrOBEX v1.2 sections 2.2.13, 2.2.14, and 3.5.
**
**  d)  Does NOT include checks of any kind on the contents of the
**        data (body headers) sent or received.
**
**
*****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* GOEP_UTIL_H */
