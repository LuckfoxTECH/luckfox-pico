/*****************************************************************************
**
**  Name:       OPP_api.h
**
**  File:       OPP API Definitions
**
**  Copyright (c) 2000-2004, WIDCOMM Inc., All Rights Reserved.
**  WIDCOMM Bluetooth Core. Proprietary and confidential.
**
*****************************************************************************/
#ifndef OPP_API_H
#define OPP_API_H

#include "bt_target.h"
#include "btm_api.h"
#include "goep_fs.h"   /* object filesystem */

/*****************************************************************************
**  Common Constants and data types
*****************************************************************************/
/* Object format */
#define OPP_VCARD21_FMT          1       /* vCard 2.1 */
#define OPP_VCARD30_FMT          2       /* vCard 3.0 */
#define OPP_VCAL_FMT             3       /* vCal 1.0 */
#define OPP_ICAL_FMT             4       /* iCal 2.0 */
#define OPP_VNOTE_FMT            5       /* vNote */
#define OPP_VMSG_FMT             6       /* vMessage */
#define OPP_ANY_FMT              0xFF    /* any format */

typedef UINT8 tOPP_FMT;

/* Object format mask */
#define OPP_VCARD21_MASK         0x01    /* vCard 2.1 */
#define OPP_VCARD30_MASK         0x02    /* vCard 3.0 */
#define OPP_VCAL_MASK            0x04    /* vCal 1.0 */
#define OPP_ICAL_MASK            0x08    /* iCal 2.0 */
#define OPP_VNOTE_MASK           0x10    /* vNote */
#define OPP_VMSG_MASK            0x20    /* vMessage */
#define OPP_ANY_MASK             0x40    /* Any type of object. */

typedef UINT8 tOPP_FMT_MASK;

/* OPP Status Codes */
#define OPP_OK                  0
#define OPP_FAIL                1
#define OPP_NOT_FOUND           2
#define OPP_NO_PERMISSION       3
#define OPP_BUSY                4
#define OPP_ABORTED             5
#define OPP_UNSUPPORTED         6

/* Used in optional formatting functions */
#define OPP_MEM                 7       /* Not enough memory to complete operation. */
#define OPP_EOO                 8       /* Reached end of the object */
#define OPP_EOF                 9       /* Reached end of the file */

typedef UINT8 tOPP_STATUS;

/* Access event operation types */
#define OPP_OPER_PUSH           1   /* Request is a PUSH operation */
#define OPP_OPER_PULL           2   /* Request is a PULL operation */
typedef UINT8 tOPP_OPER;

/* Access response types */
#define OPP_ACCESS_ALLOW        0   /* Allow the requested operation */
#define OPP_ACCESS_FORBID       1   /* Disallow the requested operation */
#define OPP_ACCESS_NONSUP       2   /* Requested operation is not supported */
typedef UINT8 tOPP_ACCESS;


/* This structure describes an object property, or individual item, inside an object. */
typedef struct
{
    UINT8       *p_data;            /* Pointer to property data. */
    UINT32      parameters;         /* Property parameters. */
    UINT16      name;               /* Property name. */
    UINT16      len;                /* Length of data. */
} tOPP_PROP;


/* file system interfaces */
typedef struct
{
    tGOEP_OPEN_CBACK     *p_open;
    tGOEP_CLOSE_CBACK    *p_close;
    tGOEP_READ_CBACK     *p_read;
    tGOEP_WRITE_CBACK    *p_write;
    tGOEP_UNLINK_CBACK   *p_unlink;
} tOPP_GOEP_CBACKS;

/* Structure used to report object push status */
typedef struct
{
    UINT32              obj_size;       /* Total size of object (GOEP_LEN_UNKNOWN if unknown) */
    UINT16              bytes;          /* Number of bytes read or written since last progress event */
    tOPP_OPER           operation;      /* Is progress for Push or Pull */
} tOPP_PROGRESS_EVT;


/**************************
**  Server Definitions
***************************/
/* Extra Debug Code */
#ifndef OPP_SRVR_DEBUG
#define OPP_SRVR_DEBUG          TRUE
#endif

#if ((BT_USE_TRACES == FALSE) && (OPP_SRVR_DEBUG == TRUE))
#undef OPP_SRVR_DEBUG
#define OPP_SRVR_DEBUG          FALSE
#endif

#define OPP_IGNORE_HANDLE       0
typedef UINT8  tOPP_SRVR_HANDLE;  /* Handle used to reference current OPP instance */

/* Server callback function events */
#define OPP_SRVR_ENABLE_EVT      0   /* Object push server is enabled. */
#define OPP_SRVR_OPEN_EVT        1   /* Connection to peer is open. */
#define OPP_SRVR_PROGRESS_EVT    2   /* Object being sent or received. */
#define OPP_SRVR_OBJECT_EVT      3   /* Object has been received. */
#define OPP_SRVR_CLOSE_EVT       4   /* Connection to peer closed. */
#define OPP_SRVR_ACCESS_EVT      5   /* Request for access to push or pull object */
#define OPP_SRVR_TIMEOUT_EVT     6   /* OBEX timeout occurred (tOPP_OPER * returned) */
#define OPP_SRVR_SDP_EVT         7   /* Register SDP Record Event */
typedef UINT8 tOPP_SRVR_EVT;

/********************
** Event interfaces *
*********************/
/* Structure associated with OPP_SRVR_ENABLE_EVT */
typedef struct
{
    tOPP_STATUS         status;
    UINT8               scn;            /* RFCOMM server channel number */
    UINT8               num_handles;    /* Object format. */
    tOPP_SRVR_HANDLE    handle[OPP_MAX_SRVR_SESS]; /* Client instances */
} tOPP_SRVR_ENABLE_EVT;

/* Structure associated with OPP_SRVR_OBJECT_EVT */
typedef struct
{
    char                *p_name;        /* Object name. */
    tOPP_FMT            format;         /* Object format. */
} tOPP_SRVR_OBJECT_EVT;

typedef struct
{
    char                *p_name;        /* Object name */
    UINT8               *p_type;        /* Object Type - NULL if none */
    UINT32              size;           /* Object size */
    UINT16              typelen;        /* Type Header Length - 0 if none */
    tBTM_BD_NAME        dev_name;       /* Name of device, "" if unknown */
    tOPP_OPER           oper;           /* Operation (push or pull) */
    tOPP_FMT            format;         /* Object format */
} tOPP_SRVR_ACCESS_EVT;

/* Union of all server callback structures */
typedef union
{
    BD_ADDR                 rem_bdaddr; /* Passed in OPP_SRVR_OPEN_EVT */
    UINT32                  sdp_handle; /* Passed in OPP_SRVR_SDP_EVT event */
    tOPP_OPER               oper;       /* Passed in OPP_SRVR_TIMEOUT_EVT event */
    tOPP_SRVR_ENABLE_EVT    enable;
    tOPP_PROGRESS_EVT       prog;
    tOPP_SRVR_OBJECT_EVT    object;
    tOPP_SRVR_ACCESS_EVT    access;
} tOPP_SRVR;

/* Server callback function */
typedef void (tOPP_SRVR_CBACK)(tOPP_SRVR_HANDLE handle, tOPP_SRVR_EVT event,
                               tOPP_SRVR *p_data);

/******************
** api interfaces *
*******************/
/* Enable structure */
typedef struct
{
    tOPP_SRVR_CBACK  *p_cback;
    tOPP_FMT_MASK     formats;
    UINT8             sec_mask;
    UINT8             max_sessions;  /* Maximum number of client instances */
    UINT8             app_id;
} tOPP_SRVR_ENABLE;


/**************************
**  Client Definitions
***************************/
/* Extra Debug Code */
#ifndef OPP_CLNT_DEBUG
#define OPP_CLNT_DEBUG          TRUE
#endif

#if ((BT_USE_TRACES == FALSE) && (OPP_CLNT_DEBUG == TRUE))
#undef OPP_CLNT_DEBUG
#define OPP_CLNT_DEBUG          FALSE
#endif

/* Client callback function event */
#define OPP_CLNT_OPEN_EVT        0   /* Connection to peer is open. */
#define OPP_CLNT_PROGRESS_EVT    1   /* push/pull in progres */
#define OPP_CLNT_OBJECT_EVT      2   /* Object Pulled */
#define OPP_CLNT_CLOSE_EVT       3   /* Connection to peer closed. */
#define OPP_CLNT_SEARCH_EVT      4   /* SDP search completed. */
#define OPP_CLNT_TIMEOUT_EVT     5   /* OBEX timeout occurred (tOPP_OPER * retuned) */

typedef UINT8 tOPP_CLNT_EVT;

typedef struct
{
    tOPP_STATUS     status;     /* Fields are valid when status is OPP_CLNT_OK */
    tOPP_FMT_MASK   formats;    /* Mask of formats supported by peer OPP service */
    UINT8           *p_name;    /* Name of the service (NULL if not in service record) */
    UINT16          name_len;   /* Length in bytes of name */
    UINT16          lang_code;  /* Language encoding: ISO 639:1988 (E/F) */
    UINT16          char_code;  /* Character encoding: MIBEnum value */
    UINT16          base_id;    /* Base attribute ID */
    UINT16          profile_version; /* Profile Version (0 if not returned) */
} tOPP_CLNT_SEARCH_EVT;

typedef struct
{
    char            *p_name;        /* Object name. */
} tOPP_CLNT_OBJECT_EVT;

typedef union
{
    tOPP_STATUS          status;
    tOPP_OPER            oper;      /* Passed in OPP_CLNT_TIMEOUT_EVT event */
    tOPP_PROGRESS_EVT    prog;
    tOPP_CLNT_OBJECT_EVT object;
    tOPP_CLNT_SEARCH_EVT search;
} tOPP_CLNT;

/* Client callback function */
typedef void tOPP_CLNT_CBACK(tOPP_CLNT_EVT event, tOPP_CLNT *p_data);

/******************
** API Interfaces *
*******************/
/* Push Object Structure */
typedef struct
{
    tOPP_CLNT_CBACK *p_cback;
    char            *p_name; /* Fully qualified path containing name of object to push */
    char            *p_typehdr; /* OBEX Type Header String (Optional) NULL skips */
    BD_ADDR         bd_addr;
    UINT8           sec_mask;
    tOPP_FMT        format;
    UINT8           app_id;
} tOPP_CLNT_PUSH;

/* Pull Object Structure */
typedef struct
{
    tOPP_CLNT_CBACK *p_cback;
    char            *p_path;        /* Fully qualified path to receive vCard */
    BD_ADDR         bd_addr;
    UINT8           sec_mask;
    UINT8           app_id;
} tOPP_CLNT_PULL;

/* Exchange Object Structure */
typedef struct
{
    tOPP_CLNT_CBACK *p_cback;
    char            *p_send;     /* Fully qualified path containing name of card to push */
    char            *p_rcv_path; /* Fully qualified path to receive vCard */
    BD_ADDR         bd_addr;
    UINT8           sec_mask;
    UINT8           app_id;
} tOPP_CLNT_EXCH;


/*******************************************
**  Optional Formatting Constants and Types
********************************************/
/* vCal Object Type */
#define OPP_VCAL_EVENT           0       /* Object is an "Event" object. */
#define OPP_VCAL_TODO            1       /* Object is a "ToDo" object. */

typedef UINT8 tOPP_VCAL;

/* vCard Property Names */
#define OPP_VCARD_ADR            1       /* Address. */
#define OPP_VCARD_EMAIL          2       /* Email address. */
#define OPP_VCARD_FN             3       /* Formatted name. */
#define OPP_VCARD_NOTE           4       /* Note. */
#define OPP_VCARD_N              5       /* Name. */
#define OPP_VCARD_ORG            6       /* Organization. */
#define OPP_VCARD_TEL            7       /* Telephone number. */
#define OPP_VCARD_TITLE          8       /* Title. */
#define OPP_VCARD_URL            9       /* URL. */
#define OPP_VCARD_LUID           10      /* Locally Unique Identifier. */

/* vCal Property Names */
#define OPP_VCAL_CATEGORIES      1       /* Categories of event. */
#define OPP_VCAL_COMPLETED       2       /* Time event is completed. */
#define OPP_VCAL_DESCRIPTION     3       /* Description of event. */
#define OPP_VCAL_DTEND           4       /* End date and time of event. */
#define OPP_VCAL_DTSTART         5       /* Start date and time of event. */
#define OPP_VCAL_DUE             6       /* Due date and time of event. */
#define OPP_VCAL_LOCATION        7       /* Location of event. */
#define OPP_VCAL_PRIORITY        8       /* Priority of event. */
#define OPP_VCAL_STATUS          9       /* Status of event. */
#define OPP_VCAL_SUMMARY         10      /* Summary of event. */
#define OPP_VCAL_LUID            11      /* Locally Unique Identifier. */

/* vNote Property Names */
#define OPP_VNOTE_BODY           1       /* Message body text. */
#define OPP_VNOTE_LUID           2       /* Locally Unique Identifier. */

/* Structure of the 32-bit parameters mask:
**
**                  + property-specific
** +reserved        |        + character set
** |                |        |     + encoding
** |                |        |     |
** 0000000000000000 00000000 00000 000
*/

/* Encoding Parameter */
#define OPP_ENC_QUOTED_PRINTABLE (1<<0)  /* Quoted-Printable encoding. */
#define OPP_ENC_8BIT             (2<<0)  /* 8-bit encoding */

/* Character Set Parameter */
#define OPP_CHAR_BIG5            (1<<3)  /* Big5 character set. */
#define OPP_CHAR_EUC_JP          (2<<3)  /* EUC-JP character set. */
#define OPP_CHAR_EUC_KR          (3<<3)  /* EUC-KR character set. */
#define OPP_CHAR_GB2312          (4<<3)  /* GB2312 character set. */
#define OPP_CHAR_ISO_2022_JP     (5<<3)  /* ISO-2022-JP character set. */
#define OPP_CHAR_ISO_8859_1      (6<<3)  /* ISO-8859-1 character set. */
#define OPP_CHAR_ISO_8859_2      (7<<3)  /* ISO-8859-2 character set. */
#define OPP_CHAR_ISO_8859_3      (8<<3)  /* ISO-8859-3 character set. */
#define OPP_CHAR_ISO_8859_4      (9<<3)  /* ISO-8859-4 character set. */
#define OPP_CHAR_ISO_8859_5      (10<<3) /* ISO-8859-5 character set. */
#define OPP_CHAR_ISO_8859_6      (11<<3) /* ISO-8859-6 character set. */
#define OPP_CHAR_ISO_8859_7      (12<<3) /* ISO-8859-7 character set. */
#define OPP_CHAR_ISO_8859_8      (13<<3) /* ISO-8859-8 character set. */
#define OPP_CHAR_KOI8_R          (14<<3) /* KOI8-R character set. */
#define OPP_CHAR_SHIFT_JIS       (15<<3) /* Shift_JIS character set. */
#define OPP_CHAR_UTF_8           (16<<3) /* UTF-8 character set. */

/* Address Type Parameter */
#define OPP_ADR_DOM              (1<<8)  /* Domestic address. */
#define OPP_ADR_INTL             (1<<9)  /* International address. */
#define OPP_ADR_POSTAL           (1<<10) /* Postal address. */
#define OPP_ADR_PARCEL           (1<<11) /* Parcel post address. */
#define OPP_ADR_HOME             (1<<12) /* Home address. */
#define OPP_ADR_WORK             (1<<13) /* Work address. */

/* Telephone Number Type Parameter */
#define OPP_TEL_PREF             (1<<8)  /* Preferred number. */
#define OPP_TEL_WORK             (1<<9)  /* Work number. */
#define OPP_TEL_HOME             (1<<10) /* Home number. */
#define OPP_TEL_VOICE            (1<<11) /* Voice number. */
#define OPP_TEL_FAX              (1<<12) /* Fax number. */
#define OPP_TEL_MSG              (1<<13) /* Message number. */
#define OPP_TEL_CELL             (1<<14) /* Cell phone number. */
#define OPP_TEL_PAGER            (1<<15) /* Pager number. */


/*****************************************************************************
**  External Function Declarations
*****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/**************************
**  Common Functions
***************************/

/*******************************************************************************
**
** Function         OPP_SetFsCbacks
**
** Description      This function is called once before any OPP API function is
**                  used to set the file system access functions in OPP
**                  control block.
**
** Returns          void
**
*******************************************************************************/
OPP_API extern void OPP_SetFsCbacks(tOPP_GOEP_CBACKS *p_fs_cbacks);

/*******************************************************************************
**
** Function         OPP_Init
**
** Description      This function is called once at stack startup to allocate
**                  (if using dynamic memory) and initialize the
**                  control block, and initializes the
**                  debug tracing level.
**
** Returns          void
**
*******************************************************************************/
OPP_API extern void OPP_Init(void);

/*****************************************************************************
**
**  Function        OPP_SetTraceLevel
**
**  Description     This function changes the trace level (debugging levels)
**
**  Returns         void
**
*****************************************************************************/
OPP_API extern void OPP_SetTraceLevel(UINT8 level);


/**************************
**  Server Functions
***************************/

/*******************************************************************************
**
** Function         OPP_SrvrEnable
**
** Description      Enable the object push server.  This function must be
**                  called before any other functions in the OPP Server API are called.
**                  When the enable operation is complete the callback function
**                  will be called with an OPP_SRVR_ENABLE_EVT event.
**
**
** Returns          void
**
*******************************************************************************/
OPP_API extern void OPP_SrvrEnable(tOPP_SRVR_ENABLE *p_info);

/*******************************************************************************
**
** Function         OPP_SrvrDisable
**
** Description      Disable the object push server. If the server is currently
**                  connected to a peer device the connection will be closed.
**
** Returns          void
**
*******************************************************************************/
OPP_API extern void OPP_SrvrDisable(void);

/*******************************************************************************
**
** Function         OPP_SrvrClose
**
** Description      If the server is currently connected to a peer
**                  device the connection will be closed. The server remains
**                  in listen mode.
**
** Returns          void
**
*******************************************************************************/
OPP_API extern void OPP_SrvrClose(tOPP_SRVR_HANDLE handle);

/*******************************************************************************
**
** Function         OPP_SrvrAccessRsp
**
** Description      Sends a reply to an access request event (OPP_SRVR_ACCESS_EVT).
**                  This call MUST be made whenever the event occurs.
**
** Parameters       handle  - client instance passed in the event
**                  oper    - operation being accessed.
**                  access  - OPP_ACCESS_ALLOW, OPP_ACCESS_FORBID, or OPP_ACCESS_NONSUP
**                  p_name  - Full path of file to read from (pull) or write to
**                            (push).
**
** Returns          void
**
*******************************************************************************/
OPP_API extern void OPP_SrvrAccessRsp(tOPP_SRVR_HANDLE handle, tOPP_OPER oper,
                                      tOPP_ACCESS access, char *p_name);

/*******************************************************************************
**
** Function         OPP_SrvrRegisterSDP
**
** Description  Create a SDP record for the OPP Server.
**              If SDP handle is created successfully, Set "Object Transfer"
**              bit in the Class of Device.
**              The returned SDP handle can be freed with SDP_DeleteRecord().
**              This API can be called anytime AFTER OPP_SrvrEnable is called.
**
**              service_len must be <= BT_MAX_SERVICE_NAME_LEN (target.h) or
**              it is truncated.
**
** Event        OPP_SRVR_SDP_EVT is returned upon completion.  The operation
**                              was successful if return handle is NOT zero.
**
** Returns          void
**
*******************************************************************************/
OPP_API extern void OPP_SrvrRegisterSDP(UINT8 *p_service, UINT16 service_len);


/**************************
**  Client Functions
***************************/

/*******************************************************************************
**
** Function         OPP_ClntServiceSearch
**
** Description      Searches a remote device for an object push service.
**                  The result is returned in the OPP_CLNT_SEARCH_EVT.  If the
**                  service exists, the optional service name is returned with
**                  the event.
**
** Returns          void
**
*******************************************************************************/
OPP_API extern void OPP_ClntServiceSearch(BD_ADDR bd_addr, tOPP_CLNT_CBACK *p_cback);

/*******************************************************************************
**
** Function         OPP_ClntPush
**
** Description      Push an object to a peer device.  p_name must point to
**                  a fully qualified path and file name.
**
**
** Returns          void
**
*******************************************************************************/
OPP_API extern void OPP_ClntPush(tOPP_CLNT_PUSH *p_push);

/*******************************************************************************
**
** Function         OPP_ClntPullCard
**
** Description      Pull default card from peer. p_path must point to a fully
**                  qualified path specifying where to store the pulled card.
**
**
** Returns          void
**
*******************************************************************************/
OPP_API extern void OPP_ClntPullCard(tOPP_CLNT_PULL *p_pull);


/*******************************************************************************
**
** Function         OPP_ClntExchCard
**
** Description      Exchange business cards with a peer device. p_send points to
**                  a fully qualified path and file name of vcard to push.
**                  p_recv_path points to a fully qualified path specifying
**                  where to store the pulled card.
**
**
** Returns          void
**
*******************************************************************************/
OPP_API extern void OPP_ClntExchCard(tOPP_CLNT_EXCH *p_exch);


/*******************************************************************************
**
** Function         OPP_ClntAbort
**
** Description      Aborts the current operation. This function is called if
**                  the application wishes to close the connection before the object
**                  push/pull has completed. In a typical connection this function
**                  does not need to be called as the connection will be closed
**                  automatically when the object push is complete.
**
**
** Returns          void
**
*******************************************************************************/
OPP_API extern void OPP_ClntAbort(void);


/***********************************
**  Optional Formatting Functions
************************************/

/*******************************************************************************
**
** Function         OPP_BuildCard
**
** Description      Build a vCard 2.1 object.  The input to this function is
**                  an array of vCard properties and a pointer to memory to
**                  store the card.  The output is a formatted vCard.
**
**
** Returns          OPP_OK if operation successful.
**                  OPP_FAIL if invalid property data.
**                  OPP_MEM if not enough memory to complete build.
**
*******************************************************************************/
OPP_API extern tOPP_STATUS OPP_BuildCard(UINT8 *p_card, UINT16 *p_len,
        tOPP_PROP *p_prop,
        UINT8 num_prop);

/*******************************************************************************
**
** Function         OPP_BuildNote
**
** Description      Build a vNote object.  The input to this function is an
**                  array of vNote properties and a pointer to memory to store
**                  the card.  The output is a formatted vNote.
**
**
** Returns          OPP_OK if operation successful.
**                  OPP_FAIL if invalid property data.
**                  OPP_MEM if not enough memory to complete build.
**
*******************************************************************************/
OPP_API extern tOPP_STATUS OPP_BuildNote(UINT8 *p_note, UINT16 *p_len,
        tOPP_PROP *p_prop,
        UINT8 num_prop);

/*******************************************************************************
**
** Function         OPP_BuildCal
**
** Description      Build a vCal 1.0 object.  The input to this function is an
**                  array of vCaalproperties and a pointer to memory to store
**                  the card.  The output is a formatted vCal.
**
**
** Returns          OPP_OK if operation successful.
**                  OPP_FAIL if invalid property data.
**                  OPP_MEM if not enough memory to complete build.
**
*******************************************************************************/
OPP_API extern tOPP_STATUS OPP_BuildCal(UINT8 *p_cal, UINT16 *p_len,
                                        tOPP_PROP *p_prop, UINT8 num_prop,
                                        tOPP_VCAL vcal_type);

/*******************************************************************************
**
** Function         OPP_ParseCard
**
** Description      Parse a vCard 2.1 object.  The input to this function is
**                  a pointer to vCard data.  The output is an array of parsed
**                  vCard properties.
**
**
** Returns          OPP_OK if operation successful.
**                  OPP_FAIL if invalid property data.
**                  OPP_MEM if not enough memory to complete parsing.
**
*******************************************************************************/
OPP_API extern tOPP_STATUS OPP_ParseCard(tOPP_PROP *p_prop, UINT8 *p_num_prop,
        UINT8 *p_card, UINT16 len);

/*******************************************************************************
**
** Function         OPP_ParseNextCardProp
**
** Description      Parse a vCard 2.1 object.  The input to this function is
**                  a pointer to vCard data.  The output is a parsed
**                  vCard properties.
**
**
** Returns          OPP_OK if operation successful.
**                  OPP_FAIL if invalid property data.
**                  OPP_MEM if not enough memory to complete parsing.
**                  OPP_EOO if end of the vcard has been reached
**                  OPP_EOF if end of the file has been reached
**
*******************************************************************************/
OPP_API extern tOPP_STATUS OPP_ParseNextCardProp(tOPP_PROP *p_prop, UINT8 **p_card,
        UINT16 *p_len, BOOLEAN new_card);

/*******************************************************************************
**
** Function         OPP_ParseNote
**
** Description      Parse a vNote object.  The input to this function is a
**                  pointer to vNote data.  The output is an array of parsed
**                  vNote properties.
**
**
** Returns          OPP_OK if operation successful.
**                  OPP_FAIL if invalid property data.
**                  OPP_MEM if not enough memory to complete parsing.
**
*******************************************************************************/
OPP_API extern tOPP_STATUS OPP_ParseNote(tOPP_PROP *p_prop, UINT8 *p_num_prop,
        UINT8 *p_note, UINT16 len);

/*******************************************************************************
**
** Function         OPP_ParseCal
**
** Description      Parse a vCal object.  The input to this function is a
**                  pointer to vCal data.  The output is an array of parsed
**                  vCal properties.
**
**
** Returns          OPP_OK if operation successful.
**                  OPP_FAIL if invalid property data.
**                  OPP_MEM if not enough memory to complete parsing.
**
*******************************************************************************/
OPP_API extern tOPP_STATUS OPP_ParseCal(tOPP_PROP *p_prop, UINT8 *p_num_prop,
                                        UINT8 *p_cal, UINT16 len,
                                        tOPP_VCAL *p_vcal_type);

#ifdef __cplusplus
}
#endif

#endif /* OPP_API_H */
