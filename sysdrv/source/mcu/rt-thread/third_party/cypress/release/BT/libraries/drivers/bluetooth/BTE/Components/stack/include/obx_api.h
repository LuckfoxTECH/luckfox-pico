/*****************************************************************************
**
**  Name:         obx_api.h
**
**  File:         Object Exchange Protocol Application Programming Interface
**
**  Copyright (c) 2003-2014, Broadcom Corp., All Rights Reserved.
**  WIDCOMM Bluetooth Core. Proprietary and confidential.
**
*****************************************************************************/
#ifndef OBX_API_H
#define OBX_API_H
#include "bt_target.h"
#include "bt_types.h"
#include "l2c_api.h"

/* 18 is 7/OBX_CONN_HDRS_OFFSET + 5/conn id, 2/ssn, 2/srm 2/srm_param */
#define OBX_HDR_OFFSET              (18 + L2CAP_MIN_OFFSET)
#define OBX_MAX_TRIPLET             3

#define OBX_MIN_MTU                 255 /* per IrOBEX spec */
#define OBX_MAX_MTU                 (OBX_LRG_DATA_POOL_SIZE - BT_HDR_SIZE - OBX_HDR_OFFSET)

#define OBX_CONN_ID_SIZE            4
#define OBX_PKT_LEN_SIZE            2

#define OBX_CONN_HDRS_OFFSET        7
#define OBX_SESS_HDRS_OFFSET        3
#define OBX_DISCON_HDRS_OFFSET      3
#define OBX_PUT_HDRS_OFFSET         3
#define OBX_GET_HDRS_OFFSET         3
#define OBX_SETPATH_REQ_HDRS_OFFSET 5
#define OBX_ABORT_HDRS_OFFSET       3
#define OBX_ACTION_HDRS_OFFSET      3
#define OBX_RESPONSE_HDRS_OFFSET    3

/* this is not needed if OBX_HDR_OFFSET is 18+ */
#define OBX_MAX_CONN_HDR_EXTRA      8 /* 5/conn id, 2/ssn, 2/srm 2/srm_param - (5/setpath + 5/conn_id - 7/conn) */

/* offset for header functions to access fields */
#define OBX_CONNECT_MTU_OFFSET      5
#define OBX_SETPATH_FLAG_OFFSET     3

#define OBX_MAX_NUM_AUTH_TRIPLET    3
#define OBX_UNICODE_SIZE            2 /* sizeof(UINT16) */

#define OBX_INVALID_HDR_LEN         0xFFFF

#define OBX_MIN_NONCE_SIZE          4   /* fixed size per IrOBEX spec */
#define OBX_NONCE_SIZE              16  /* fixed size per IrOBEX spec */
#define OBX_DIGEST_SIZE             16  /* fixed size per IrOBEX spec */
#define OBX_MAX_AUTH_KEY_SIZE       16
#define OBX_MAX_AUTH_USER_SIZE      20
#define OBX_SESSION_ID_SIZE         16  /* fixed size per IrOBEX spec */
#define OBX_SESSION_INFO_SIZE       32  /* OBX_SESSION_ID_SIZE + 4(local nonce) + 4 (connection id) + 4 (timeout) + 2(mtu) + 1(state) + 1(srm)  */
#define OBX_SESSION_INFO_NONCE_IDX  16  /* The index to the (local nonce) in session info */
#define OBX_SESSION_INFO_ID_IDX     20  /* The index to the (connection id) in session info */
#define OBX_SESSION_INFO_TO_IDX     24  /* The index to the (timeout) in session info */
#define OBX_SESSION_INFO_MTU_IDX    28  /* The index to peer MTU in session info */
#define OBX_SESSION_INFO_ST_IDX     30  /* The index to sr/cl state in session info */
#define OBX_SESSION_INFO_SRM_IDX    31  /* The index to srm in session info */
#define OBX_TIMEOUT_SIZE            4

/* handle related definitions */
#define OBX_SESS_SHIFT              8
#define OBX_ENC_SESS_HANDLE(oh, os) (((os)<<OBX_SESS_SHIFT)|(oh))
#define OBX_HANDLE_MASK             0xFF
#define OBX_SESS_MASK               0x7F00
#define OBX_DEC_HANDLE(os)          ((os) & OBX_HANDLE_MASK)
#define OBX_DEC_SESS_IND(os)        ((os & OBX_SESS_MASK)>>OBX_SESS_SHIFT)

/* Return values for API functions */
enum
{
    OBX_SUCCESS,        /* Status is successful. */
    OBX_BAD_PARAMS,     /* Bad parameter(s). */
    OBX_NO_RESOURCES,   /* No resources (GKI buffers, control block) */
    OBX_BAD_HANDLE      /* The OBEX handle is not valid. */
};
typedef UINT8 tOBX_STATUS;


typedef UINT16 tOBX_HANDLE;

#define OBX_HANDLE_NULL             0

enum
{
    OBX_PT_PUT,     /* Regular Put request */
    OBX_PT_DELETE,  /* Delete request - a Put request with NO Body or End-of-Body header. */
    OBX_PT_CREATE   /* Create-Empty request - a Put request with an empty End-of-Body header. */
};
typedef UINT8   tOBX_PUT_TYPE;

/* SetPath Request Flags - the following definitions can be ORed if both flags are wanted */
#define OBX_SPF_BACKUP      0x01    /* Backup a level before applying name(equivalent to ../) */
#define OBX_SPF_NO_CREATE   0x02    /* Don't create directory if it does not exist, return an error instead. */
typedef UINT8   tOBX_SETPATH_FLAG;

/* Authentication Challenge Options */
#define OBX_AO_NONE         0x00    /* If this is used in OBX_StartServer and the authenticate
                                     * flag is TRUE, the optional Challenge Information (tag 0x01)
                                     * will not be sent. */
#define OBX_AO_USR_ID       0x01    /* Set this bit to make the client respond with the user ID. */
typedef UINT8 tOBX_AUTH_OPT;

/* CHARSET definition for Authentication Challenge Realm */
#define OBX_RCS_ASCII       0x00    /* ASCII */
#define OBX_RCS_8859_1      0x01    /* ISO-8859-1 */
#define OBX_RCS_8859_2      0x02    /* ISO-8859-2 */
#define OBX_RCS_8859_3      0x03    /* ISO-8859-3 */
#define OBX_RCS_8859_4      0x04    /* ISO-8859-4 */
#define OBX_RCS_8859_5      0x05    /* ISO-8859-5 */
#define OBX_RCS_8859_6      0x06    /* ISO-8859-6 */
#define OBX_RCS_8859_7      0x07    /* ISO-8859-7 */
#define OBX_RCS_8859_8      0x08    /* ISO-8859-8 */
#define OBX_RCS_8859_9      0x09    /* ISO-8859-9 */
#define OBX_RCS_UNICODE     0xFF    /* Unicode */
typedef UINT8 tOBX_CHARSET;

typedef struct
{
    UINT8               ssn;        /* session sequence number */
    BOOLEAN             final;      /* TRUE, if this is the final packet of this PUT transaction. */
    tOBX_PUT_TYPE       type;       /* The type of PUT request. */
} tOBX_PUT_EVT;

typedef struct
{
    UINT8               ssn;        /* session sequence number */
    BOOLEAN             final;      /* TRUE, if this is the final packet of this GET transaction. */
} tOBX_GET_EVT;

typedef struct
{
    UINT8               ssn;        /* session sequence number */
    BD_ADDR             peer_addr;  /* The peer Bluetooth Address. */
    UINT16              mtu;        /* The peer MTU. This element is associated with OBX_CONNECT_REQ_EVT and OBX_CONNECT_RSP_EVT. */
    tOBX_HANDLE         handle;     /* the OBX handle returned by OBX_StartServer(), OBX_CreateSession() and OBX_ConnectReq() */
    BOOLEAN             no_rsp;     /* TRUE, when the event is generated as a part of RESUME SESSION */
} tOBX_CONN_EVT;

/* Session Opcode Definitions: */
#define OBX_SESS_OP_CREATE              0x00 /* Create Session */
#define OBX_SESS_OP_CLOSE               0x01 /* Close Session */
#define OBX_SESS_OP_SUSPEND             0x02 /* Suspend Session */
#define OBX_SESS_OP_RESUME              0x03 /* Resume Session */
#define OBX_SESS_OP_SET_TIME            0x04 /* Set Timeout */
#define OBX_SESS_OP_TRANSPORT           0xFF /* transport dropped */
typedef UINT8   tOBX_SESS_OP;

/* Session States Definitions for external use: */
enum
{
    OBX_SESS_NONE,       /* 0x00    session is not engaged/closed */
    OBX_SESS_ACTIVE,     /* 0x01    session is active. */
    OBX_SESS_SUSPENDED,  /* 0x02    session is suspended. */
    OBX_SESS_EXT_MAX
};
typedef UINT8 tOBX_SESS_ST;


typedef struct
{
    UINT8               ssn;            /* session sequence number */
    tOBX_SESS_OP        sess_op;        /* the session op code */
    tOBX_SESS_ST        sess_st;        /* the session state */
    BD_ADDR             peer_addr;      /* The peer Bluetooth Address. */
    UINT8               *p_sess_info;   /* The session ID and the local nonce for a reliable session, a reference to the location in OBEX control block or NULL */
    UINT32              timeout;        /* The number of seconds remaining in suspend. 0xffff if infinite. */
    UINT32              obj_offset;     /* The object offset for resume session. */
    UINT8               nssn;           /* tne next session sequence number the server expects */
} tOBX_SESS_EVT;

#define OBX_ACT_COPY        0x00    /* Copy object */
#define OBX_ACT_MOVE        0x01    /* Move/rename object */
#define OBX_ACT_PERMISSION  0x02    /* Set object permission */
typedef UINT8 tOBX_ACTION;

typedef struct
{
    UINT8               ssn;        /* session sequence number */
    tOBX_ACTION         action;     /* The action opcode. */
} tOBX_ACT_EVT;

typedef struct
{
    UINT8               ssn;        /* session sequence number */
    tOBX_SETPATH_FLAG   flag;       /* The set path flags. */
} tOBX_SETPATH_EVT;

/* permission flags */
#define OBX_PERMISSION_READ         0x01
#define OBX_PERMISSION_WRITE        0x02
#define OBX_PERMISSION_DELETE       0x04
#define OBX_PERMISSION_MODIFY       0x80

typedef union
{
    UINT8               ssn;        /* session sequence number */
    tOBX_CONN_EVT       conn;       /* This element is associated with OBX_CONNECT_REQ_EVT and OBX_CONNECT_RSP_EVT. */
    tOBX_SESS_EVT       sess;       /* This element is associated with OBX_SESSION_RSP_EVT and OBX_SESSION_REQ_EVT. */
    tOBX_PUT_EVT        put;        /* This element is associated with OBX_PUT_REQ_EVT. */
    tOBX_SETPATH_EVT    sp;         /* This element is associated with OBX_SETPATH_REQ_EVT. */
    tOBX_ACT_EVT        action;     /* This element is associated with OBX_ACTION_REQ_EVT */
    tOBX_GET_EVT        get;        /* This element is associated with OBX_GET_REQ_EVT. TRUE, if this is the final packet that contains the OBEX headers for this GET request. */
} tOBX_EVT_PARAM;


enum
{
    OBX_NULL_EVT,
    /* server events */
    OBX_CONNECT_REQ_EVT,    /* param = packet MTU */
    OBX_SESSION_REQ_EVT,    /* A Crease Session or Resume Session request is received by the server. Call OBX_SessionRsp(). */
    OBX_DISCONNECT_REQ_EVT,
    OBX_PUT_REQ_EVT,        /* could have param indicate delete or create? */
    OBX_GET_REQ_EVT,
    OBX_SETPATH_REQ_EVT,    /* param = flags */
    OBX_ABORT_REQ_EVT,
    OBX_ACTION_REQ_EVT,     /* An Action request is received by the server. Call OBX_ActionRsp(). */

    /* client events */
    OBX_CONNECT_RSP_EVT,    /* param = packet MTU */
    OBX_SESSION_RSP_EVT,    /* A response for Create Session or Resume Session is received by the client. The client needs to remember the session id. The session id is to be used in calling OBX_ResumeSession, if the current session is terminated prematurely. */
    OBX_DISCONNECT_RSP_EVT,
    OBX_PUT_RSP_EVT,
    OBX_GET_RSP_EVT,
    OBX_SETPATH_RSP_EVT,
    OBX_ABORT_RSP_EVT,
    OBX_ACTION_RSP_EVT,     /* An Action response is received by the client. */

    /* common events */
    OBX_SESSION_INFO_EVT,   /* the session information event to resume the session. */
    OBX_CLOSE_IND_EVT,      /* when transport goes down; p_pkt = NULL; no response needed */
    OBX_TIMEOUT_EVT,        /* param = tOBX_EVENT */
    OBX_PASSWORD_EVT
};
typedef UINT8 tOBX_EVENT;

/* this is used by the protocol display function only*/
enum
{
    OBX_NULL_DISP,
    /* server events */
    OBX_CONNECT_REQ_DISP,
    OBX_SESSION_REQ_DISP,
    OBX_DISCONNECT_REQ_DISP,
    OBX_PUT_REQ_DISP,
    OBX_GET_REQ_DISP,
    OBX_SETPATH_REQ_DISP,
    OBX_ABORT_REQ_DISP,
    OBX_ACTION_REQ_DISP,
    /* client events */
    OBX_CONNECT_RSP_DISP,
    OBX_SESSION_RSP_DISP,
    OBX_DISCONNECT_RSP_DISP,
    OBX_PUT_RSP_DISP,
    OBX_GET_RSP_DISP,
    OBX_SETPATH_RSP_DISP,
    OBX_ABORT_RSP_DISP,
    OBX_ACTION_RSP_DISP,
    /* common events */
    OBX_CLOSE_IND_DISP,
    OBX_TIMEOUT_DISP,
    OBX_PASSWORD_DISP
};
#define OBX_DISP_IS_RECV    0x80
#define OBX_DISP_TYPE_MASK  0x7F

#define OBX_MAX_EVT         OBX_PASSWORD_EVT
#define OBX_MAX_OFFSET_IND  OBX_ABORT_RSP_EVT /* This is used to access obx_hdr_start_offset */

/*
** Define Miscellaneous Constants
*/
#define OBX_VERSION                     0x10    /* Version 1.0 */
#define OBX_CONN_FLAGS                  0       /* Connect flags per IrOBEX spec */
#define OBX_SETPATH_CONST               0       /* SetPath Request constants per IrOBEX spec */
#define OBX_INVALID_CONN_ID             0xFFFFFFFF  /* invalid connection ID per IrOBEX spec */
#define OBX_INFINITE_TIMEOUT            0xFFFFFFFF

/* Header Identifier Data Type Constants */
#define OBX_HI_TYPE_MASK                0xC0    /* This mask get the encoding (data type) of the header ID. */
#define OBX_HI_ID_MASK                  0x3F    /* This mask gets the meaning of the header ID. */
#define OBX_HI_TYPE_UNIC                0x00    /* Null terminated Unicode text */
#define OBX_HI_TYPE_ARRAY               0x40    /* Unstructured octet array (byte sequence) */
#define OBX_HI_TYPE_BYTE                0x80    /* 8-bit integer */
#define OBX_HI_TYPE_INT                 0xC0    /* 32-bit integer */

/*
** Define OBEX Header Identifiers
*/
#define OBX_HI_NULL                     0
#define OBX_HI_COUNT                    0xC0
#define OBX_HI_NAME                     0x01
#define OBX_HI_TYPE                     0x42
#define OBX_HI_LENGTH                   0xC3
#define OBX_HI_TIME                     0x44
#define OBX_HI_DESCRIPTION              0x05
#define OBX_HI_TARGET                   0x46
#define OBX_HI_HTTP                     0x47
#define OBX_HI_BODY                     0x48
#define OBX_HI_BODY_END                 0x49
#define OBX_HI_WHO                      0x4A
#define OBX_HI_CONN_ID                  0xCB
#define OBX_HI_APP_PARMS                0x4C
#define OBX_HI_CHALLENGE                0x4D
#define OBX_HI_AUTH_RSP                 0x4E
#define OBX_HI_CREATOR_ID               0xCF
#define OBX_HI_WAN_UUID                 0x50
#define OBX_HI_OBJ_CLASS                0x51
#define OBX_HI_SESSION_PARAM            0x52
#define OBX_HI_SESSION_SN               0x93
#define OBX_HI_ACTION_ID                0x94
#define OBX_HI_DEST_NAME                0x15
#define OBX_HI_PERMISSION               0xD6
#define OBX_HI_SRM                      0x97
#define OBX_HI_SRM_PARAM                0x98
#define OBX_HI_LO_USER                  0x30
#define OBX_HI_HI_USER                  0x3F

/* Obex Header Values for the SRM header */
#define OBX_HV_SRM_DISABLE  0x00    /* SRM header value - disable */
#define OBX_HV_SRM_ENABLE   0x01    /* SRM header value - enable */
#define OBX_HV_SRM_IND      0x02    /* SRM header value - indicate support */

/* Obex Header Values for the SRM Parameter header */
#define OBX_HV_SRM_PARAM_MORE   0x00    /* SRM Param header value - request additional packet */
#define OBX_HV_SRM_PARAM_WAIT   0x01    /* SRM Param header value - wait for next req/rsp */
#define OBX_HV_SRM_PARAM_COMBO  0x02    /* SRM Param header value - next and wait */

#define OBX_TAG_SESS_PARAM_ADDR         0x00
#define OBX_TAG_SESS_PARAM_NONCE        0x01
#define OBX_TAG_SESS_PARAM_SESS_ID      0x02
#define OBX_TAG_SESS_PARAM_NSEQNUM      0x03
#define OBX_TAG_SESS_PARAM_TOUT         0x04
#define OBX_TAG_SESS_PARAM_SESS_OP      0x05
#define OBX_TAG_SESS_PARAM_OBJ_OFF      0x06
#define OBX_MAX_SESS_PARAM_TRIP         7   /* max number of TLV for session operations */

#define OBX_LEN_SESS_PARAM_SESS_OP      1
#define OBX_LEN_SESS_PARAM_OBJ_OFF      4   /* this value varies, so it needs to be verified on the receiving side */

/*
** Define OBEX Request Codes
*/
#define OBX_REQ_CONNECT                         0x00    /* need to set final bit */
#define OBX_REQ_DISCONNECT                      0x01    /* need to set final bit */
#define OBX_REQ_PUT                             0x02
#define OBX_REQ_GET                             0x03
#define OBX_REQ_SETPATH                         0x05    /* need to set final bit */
#define OBX_REQ_ACTION                          0x06
#define OBX_REQ_SESSION                         0x07    /* need to set final bit */
#define OBX_REQ_ABORT                           0x7F    /* need to set final bit */
#define OBX_FINAL                               0x80

/* OBEX response code as defined in IrOBEX spec. version 1.2 */
#define OBX_RSP_DEFAULT                         0x00
#define OBX_RSP_FAILED                          0x08    /* OBEX failed - not from spec */
#define OBX_RSP_CONTINUE                        0x10    /* Continue */
#define OBX_RSP_OK                              0x20    /* OK, Success */
#define OBX_RSP_CREATED                         0x21    /* Created */
#define OBX_RSP_ACCEPTED                        0x22    /* Accepted */
#define OBX_RSP_NON_AUTH_INFO                   0x23    /* Non-Authoritative Information */
#define OBX_RSP_NO_CONTENT                      0x24    /* No Content */
#define OBX_RSP_RESET_CONTENT                   0x25    /* Reset Content */
#define OBX_RSP_PART_CONTENT                    0x26    /* Partial Content */
#define OBX_RSP_MULTI_CHOICES                   0x30    /* Multiple Choices */
#define OBX_RSP_MVD_PERM                        0x31    /* Moved Permanently */
#define OBX_RSP_MVD_TEMP                        0x32    /* Moved temporarily */
#define OBX_RSP_SEE_OTHER                       0x33    /* See Other */
#define OBX_RSP_NOT_MODIFIED                    0x34    /* Not modified */
#define OBX_RSP_USE_PROXY                       0x35    /* Use Proxy */
#define OBX_RSP_BAD_REQUEST                     0x40    /* Bad Request - server couldn't understand request */
#define OBX_RSP_UNAUTHORIZED                    0x41    /* Unauthorized */
#define OBX_RSP_PAYMENT_REQD                    0x42    /* Payment required */
#define OBX_RSP_FORBIDDEN                       0x43    /* Forbidden - operation is understood but refused */
#define OBX_RSP_NOT_FOUND                       0x44    /* Not Found */
#define OBX_RSP_NOT_ALLOWED                     0x45    /* Method not allowed */
#define OBX_RSP_NOT_ACCEPTABLE                  0x46    /* Not Acceptable */
#define OBX_RSP_PROXY_AUTH_REQD                 0x47    /* Proxy Authentication required */
#define OBX_RSP_REQUEST_TIMEOUT                 0x48    /* Request Time Out */
#define OBX_RSP_CONFLICT                        0x49    /* Conflict */
#define OBX_RSP_GONE                            0x4A    /* Gone */
#define OBX_RSP_LENGTH_REQD                     0x4B    /* Length Required */
#define OBX_RSP_PRECONDTN_FAILED                0x4C    /* Precondition failed */
#define OBX_RSP_REQ_ENT_2_LARGE                 0x4D    /* Requested entity too large */
#define OBX_RSP_REQ_URL_2_LARGE                 0x4E    /* Request URL too large */
#define OBX_RSP_UNSUPTD_TYPE                    0x4F    /* Unsupported media type */
#define OBX_RSP_INTRNL_SRVR_ERR                 0x50    /* Internal Server Error */
#define OBX_RSP_NOT_IMPLEMENTED                 0x51    /* Not Implemented */
#define OBX_RSP_BAD_GATEWAY                     0x52    /* Bad Gateway */
#define OBX_RSP_SERVICE_UNAVL                   0x53    /* Service Unavailable */
#define OBX_RSP_GATEWAY_TIMEOUT                 0x54    /* Gateway Timeout */
#define OBX_RSP_HTTP_VER_NOT_SUPTD              0x55    /* HTTP version not supported */
#define OBX_RSP_DATABASE_FULL                   0x60    /* Database Full */
#define OBX_RSP_DATABASE_LOCKED                 0x61    /* Database Locked */

#define OBX_MAX_OK_RSP      OBX_RSP_PART_CONTENT

typedef UINT8   tOBX_RSP_CODE;

/* tags for authentication triplet */
#define OBX_NONCE_CHLNG_TAG         0
#define OBX_OPTIONS_CHLNG_TAG       1
#define OBX_REALM_CHLNG_TAG         2

#define OBX_DIGEST_RSP_TAG          0
#define OBX_USERID_RSP_TAG          1
#define OBX_NONCE_RSP_TAG           2

typedef struct
{
    UINT8   tag;
    UINT8   len;
    UINT8   *p_array;
} tOBX_TRIPLET;

/* Server Callback type: */
typedef void (tOBX_SR_CBACK)(tOBX_HANDLE shandle, tOBX_EVENT event, tOBX_EVT_PARAM param, BT_HDR *p_pkt);
/* Client Callback type: */
typedef void (tOBX_CL_CBACK)(tOBX_HANDLE handle, tOBX_EVENT event, UINT8 rsp_code, tOBX_EVT_PARAM param, BT_HDR *p_pkt);


typedef struct
{
    UINT16  len;                        /* Length of target header. */
    UINT8   target[OBX_MAX_TARGET_LEN]; /* The byte sequence that describes the target header. */
} tOBX_TARGET;


typedef struct
{
    tOBX_TARGET     *p_target;
    tOBX_SR_CBACK   *p_cback;
    UINT16          mtu;
    UINT8           scn;            /* The RFCOMM SCN number that this server listens for incoming requests. 0, if do not wish to listen to connection from RFCOMM. */
    BOOLEAN         authenticate;
    UINT8           auth_option;
    UINT8           realm_charset;
    UINT8           *p_realm;
    UINT8           realm_len;
    UINT8           max_sessions;
    BOOLEAN         get_nonf;       /* report GET non-final request event. If FALSE, GET response is sent automatically */
    UINT16          psm;            /* The L2CAP PSM number that this server listens for incoming requests. 0, if do not wish to listen to connection from L2CAP. */
    UINT32          nonce;          /* This is converted to UINT8[16] internally before adding to the OBEX header. This value is copied to the server control block and is increased after each use. 0, if only legacy OBEX (unreliable) session is desired. */
    BOOLEAN         srm;            /* TRUE, to support single response mode. */
    UINT8           max_suspend;    /* Max number of suspended session. must be less than OBX_MAX_SUSPEND_SESSIONS. ignored, if nonce is 0 */
} tOBX_StartParams;





/*****************************************************************************
**  External Function Declarations
*****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
**
** Function     OBX_Init
**
** Description  This function is called to initialize the control block for this
**              layer. It must be called before accessing any other of its API
**              functions.  It is typically called once during the start up of
**              the stack.
**
** Returns      void.
**
*******************************************************************************/
OBX_API extern void OBX_Init(void);

/*******************************************************************************
**
** Function         OBX_SetTraceLevel
**
** Description      This function sets the debug trace level for OBX.
**                  If 0xff is passed, the current trace level is returned.
**
**                  Input Parameters:
**                      level:  The level to set the OBX tracing to:
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
*******************************************************************************/
OBX_API extern UINT8 OBX_SetTraceLevel(UINT8 level);

/*******************************************************************************
** Function     OBX_HandleToMtu
**
** Description  Given an OBEX handle, return the associated peer MTU.
**
** Returns      MTU.
**
*******************************************************************************/
OBX_API extern UINT16 OBX_HandleToMtu(tOBX_HANDLE handle);

/* Server API's: */
/*******************************************************************************
**
** Function     OBX_StartServer
**
** Description  This function is to register a server entity to OBEX.
**
** Returns      OBX_SUCCESS, if successful.
**              OBX_NO_RESOURCES, if OBX does not have resources.
**
*******************************************************************************/
OBX_API extern tOBX_STATUS OBX_StartServer(tOBX_StartParams *p_params, tOBX_HANDLE *p_handle);

/*******************************************************************************
**
** Function     OBX_StopServer
**
** Description  This function is to stop this OBEX server from receiving any
**              more incoming requests.
**
** Returns      OBX_SUCCESS, if successful.
**              OBX_BAD_HANDLE, if the handle is not valid.
**
*******************************************************************************/
OBX_API extern tOBX_STATUS OBX_StopServer(tOBX_HANDLE handle);

/*******************************************************************************
**
** Function     OBX_AddSuspendedSession
**
** Description  This function is to add the session information for a previously
**              suspended reliable session to the server control block
**
** Returns      OBX_SUCCESS, if successful.
**              OBX_BAD_HANDLE, if the handle is not valid.
**
*******************************************************************************/
OBX_API extern tOBX_STATUS OBX_AddSuspendedSession(tOBX_HANDLE shandle, BD_ADDR peer_addr, UINT8 *p_sess_info,
        UINT32 timeout, UINT8 ssn, UINT32 offset);

/*******************************************************************************
**
** Function     OBX_ConnectRsp
**
** Description  This function is called to send the response to a Connect
**              Request from an OBEX client.
**
** Returns      OBX_SUCCESS, if successful.
**              OBX_BAD_HANDLE, if the handle is not valid.
**
*******************************************************************************/
OBX_API extern tOBX_STATUS OBX_ConnectRsp(tOBX_HANDLE shandle, UINT8 rsp_code, BT_HDR *p_pkt);

/*******************************************************************************
**
** Function     OBX_SessionRsp
**
** Description  This function is called to respond to a request to create a reliable session.
**
**
** Returns      OBX_SUCCESS, if successful.
**              OBX_BAD_HANDLE, if the handle is not valid.
**
*******************************************************************************/
OBX_API extern tOBX_STATUS OBX_SessionRsp(tOBX_HANDLE shandle, UINT8 rsp_code,
        UINT8 ssn, UINT32 offset, BT_HDR *p_pkt);

/*******************************************************************************
**
** Function     OBX_SetPathRsp
**
** Description  This function is called to send the response to a Set Path
**              Request from an OBEX client.
**
** Returns      OBX_SUCCESS, if successful.
**              OBX_BAD_HANDLE, if the handle is not valid.
**
*******************************************************************************/
OBX_API extern tOBX_STATUS OBX_SetPathRsp(tOBX_HANDLE shandle, UINT8 rsp_code, BT_HDR *p_pkt);

/*******************************************************************************
**
** Function     OBX_PutRsp
**
** Description  This function is called to send the response to a Put
**              Request from an OBEX client.
**
** Returns      OBX_SUCCESS, if successful.
**              OBX_BAD_HANDLE, if the handle is not valid.
**
*******************************************************************************/
OBX_API extern tOBX_STATUS OBX_PutRsp(tOBX_HANDLE shandle, UINT8 rsp_code, BT_HDR *p_pkt);

/*******************************************************************************
**
** Function     OBX_GetRsp
**
** Description  This function is called to send the response to a Get
**              Request from an OBEX client.
**
** Returns      OBX_SUCCESS, if successful.
**              OBX_BAD_HANDLE, if the handle is not valid.
**
*******************************************************************************/
OBX_API extern tOBX_STATUS OBX_GetRsp(tOBX_HANDLE shandle, UINT8 rsp_code, BT_HDR *p_pkt);

/*******************************************************************************
**
** Function     OBX_AbortRsp
**
** Description  This function is called to send the response to an Abort
**              Request from an OBEX client.
**
** Returns      OBX_SUCCESS, if successful.
**              OBX_BAD_HANDLE, if the handle is not valid.
**
*******************************************************************************/
OBX_API extern tOBX_STATUS OBX_AbortRsp(tOBX_HANDLE shandle, UINT8 rsp_code, BT_HDR *p_pkt);

/*******************************************************************************
**
** Function     OBX_ActionRsp
**
** Description  This function is called to respond to an Action command Request
**              from an OBEX client.
**
** Returns      OBX_SUCCESS, if successful.
**              OBX_BAD_HANDLE, if the handle is not valid.
**
*******************************************************************************/
OBX_API extern tOBX_STATUS OBX_ActionRsp(tOBX_HANDLE shandle, UINT8 rsp_code, BT_HDR *p_pkt);

/*******************************************************************************
**
** Function     OBX_DisconnectRsp
**
** Description  This function is called to send the response to a Disconnect
**              Request from an OBEX client.
**              This function can also be used to force close the transport
**              to a connected client.
**
** Returns      OBX_SUCCESS, if successful.
**              OBX_BAD_HANDLE, if the handle is not valid.
**
*******************************************************************************/
OBX_API extern tOBX_STATUS OBX_DisconnectRsp(tOBX_HANDLE shandle, UINT8 rsp_code, BT_HDR *p_pkt);

/*******************************************************************************
**
** Function     OBX_Password
**
** Description  This function is called to respond to an OBX_PASSWORD_EVT event.
**
** Returns      OBX_SUCCESS, if successful.
**              OBX_NO_RESOURCES, if OBX does not resources
**
*******************************************************************************/
OBX_API extern tOBX_STATUS OBX_Password(tOBX_HANDLE shandle, UINT8 *p_password, UINT8 password_len,
                                        UINT8 *p_userid, UINT8 userid_len);

/*******************************************************************************
**
** Function     OBX_GetPeerAddr
**
** Description  This function is called to learn the Bluetooth address of the
**              connected device.
**
** Returns      L2CAP channel ID.
**
*******************************************************************************/
OBX_API extern UINT16 OBX_GetPeerAddr(tOBX_HANDLE shandle, BD_ADDR bd_addr);

/*******************************************************************************
**
** Function     OBX_GetPortHandle
**
** Description  This function is called to get the RFCOMM port handle for the obex connection.
**
**
** Returns      OBX_SUCCESS, if successful.
**              OBX_NO_RESOURCES, if no existing connection.
**              OBX_BAD_HANDLE, if the handle is not valid.
**
*******************************************************************************/
OBX_API extern tOBX_STATUS OBX_GetPortHandle(tOBX_HANDLE handle, UINT16 *port_handle);

/* Client API's: */
/*******************************************************************************
**
** Function     OBX_ConnectReq
**
** Description  This function registers a client entity to OBEX and sends a
**              CONNECT request to the server specified by the API parameters.
**
** Returns      OBX_SUCCESS, if successful.
**              OBX_NO_RESOURCES, if OBX does not resources
**
*******************************************************************************/
OBX_API extern tOBX_STATUS OBX_ConnectReq(BD_ADDR bd_addr, UINT8 scn, UINT16 mtu,
        tOBX_CL_CBACK *p_cback, tOBX_HANDLE *p_handle, BT_HDR *p_pkt);

/*******************************************************************************
**
** Function     OBX_AllocSession
**
** Description  This function registers a client entity to OBEX.
**              If p_session_id is not NULL, it tries to find an suspended session
**                  with matching session_id.
**              If scn is not 0, it allocates a control block for this new session.
**              Otherwise, it allocates a control block for the given PSM.
**              The associated virtual PSM assigned by L2CAP is returned in p_psm
**              The allocated OBEX handle is returned in p_handle.
**
** Returns      OBX_SUCCESS, if successful.
**              OBX_NO_RESOURCES, if OBX does not resources
**
*******************************************************************************/
OBX_API extern tOBX_STATUS OBX_AllocSession(UINT8 *p_session_info, UINT8 scn, UINT16 *p_psm,
        tOBX_CL_CBACK *p_cback, tOBX_HANDLE *p_handle);

/*******************************************************************************
**
** Function     OBX_CreateSession
**
** Description  This function registers a client entity to OBEX.
**              It may send a CreateSession request and wait for CreateSession response.
**              It sends a CONNECT request to the server specified by the API parameters.
**
** Returns      OBX_SUCCESS, if successful.
**              OBX_NO_RESOURCES, if OBX does not resources
**
*******************************************************************************/
OBX_API extern tOBX_STATUS OBX_CreateSession(BD_ADDR bd_addr, UINT16 mtu, BOOLEAN srm, UINT32 nonce,
        tOBX_HANDLE handle, BT_HDR *p_pkt);

/*******************************************************************************
**
** Function     OBX_ResumeSession
**
** Description  This function registers a client entity to OBEX and resumes
**              a previously interrupted reliable session.
**
** Returns      OBX_SUCCESS, if successful.
**              OBX_NO_RESOURCES, if OBX does not resources
**
*******************************************************************************/
OBX_API extern tOBX_STATUS OBX_ResumeSession(BD_ADDR bd_addr, UINT8 ssn, UINT32 offset, tOBX_HANDLE handle);

/*******************************************************************************
**
** Function     OBX_SessionReq
**
** Description  This function is used to Suspend, Resume, or Close a session.
**              For Resume: this function registers a client entity to OBEX and resumes
**              a previously interrupted reliable session.
**
** Returns      OBX_SUCCESS, if successful.
**              OBX_NO_RESOURCES, if OBX does not resources
**
*******************************************************************************/
OBX_API extern tOBX_STATUS OBX_SessionReq(tOBX_HANDLE handle, tOBX_SESS_OP opcode, UINT32 timeout);

/*******************************************************************************
**
** Function     OBX_SetPathReq
**
** Description  This function sends a Set Path request to the connected server.
**
** Returns      OBX_SUCCESS, if successful.
**              OBX_BAD_HANDLE, if the handle is not valid.
**
*******************************************************************************/
OBX_API extern tOBX_STATUS OBX_SetPathReq(tOBX_HANDLE handle, UINT8 flags, BT_HDR *p_pkt);

/*******************************************************************************
**
** Function     OBX_PutReq
**
** Description  This function sends a Put request to the connected server.
**
** Returns      OBX_SUCCESS, if successful.
**              OBX_BAD_HANDLE, if the handle is not valid.
**
*******************************************************************************/
OBX_API extern tOBX_STATUS OBX_PutReq(tOBX_HANDLE handle, BOOLEAN final, BT_HDR *p_pkt);

/*******************************************************************************
**
** Function     OBX_GetReq
**
** Description  This function sends a Get request to the connected server.
**
** Returns      OBX_SUCCESS, if successful.
**              OBX_BAD_HANDLE, if the handle is not valid.
**
*******************************************************************************/
OBX_API extern tOBX_STATUS OBX_GetReq(tOBX_HANDLE handle, BOOLEAN final, BT_HDR *p_pkt);

/*******************************************************************************
**
** Function     OBX_AbortReq
**
** Description  This function sends an Abort request to the connected server.
**
** Returns      OBX_SUCCESS, if successful.
**              OBX_BAD_HANDLE, if the handle is not valid.
**
*******************************************************************************/
OBX_API extern tOBX_STATUS OBX_AbortReq(tOBX_HANDLE handle, BT_HDR *p_pkt);

/*******************************************************************************
**
** Function     OBX_DisconnectReq
**
** Description  This function sends a Disconnect request to the connected server.
**
** Returns      OBX_SUCCESS, if successful.
**              OBX_BAD_HANDLE, if the handle is not valid.
**
*******************************************************************************/
OBX_API extern tOBX_STATUS OBX_DisconnectReq(tOBX_HANDLE handle, BT_HDR *p_pkt);

/*******************************************************************************
**
** Function     OBX_ActionReq
**
** Description  This function sends a Action request to the connected server.
**              The Name Header and DestName Header must be in p_pkt for
**                  the Copy and Move Object action.
**              The Name header and Permission Header must be in p_pkt for
**                  the Set Object Permission action.
**
** Returns      OBX_SUCCESS, if successful.
**              OBX_BAD_HANDLE, if the handle is not valid.
**
*******************************************************************************/
OBX_API extern tOBX_STATUS OBX_ActionReq(tOBX_HANDLE handle, tOBX_ACTION action_id, BT_HDR *p_pkt);

/*******************************************************************************
**
** Function     OBX_AuthResponse
**
** Description  This function is called to respond to an OBX_PASSWORD_EVT event.
**
** Returns      OBX_SUCCESS, if successful.
**              OBX_NO_RESOURCES, if OBX does not resources
**
*******************************************************************************/
OBX_API extern tOBX_STATUS OBX_AuthResponse(tOBX_HANDLE handle,
        UINT8 *p_password, UINT8 password_len,
        UINT8 *p_userid, UINT8 userid_len,
        BOOLEAN authenticate);


/*******************************************************************************
**
** Function     OBX_HdrInit
**
** Description  This function is called to initialize an OBEX packet. This
**              function takes a GKI buffer and sets the offset in BT_HDR as
**              OBEX_HDR_OFFSET, the len as 0. The layer_specific is set to the
**              length still available. This function compares the given
**              (pkt_size - sizeof(BT_HDR)) with the peer MTU to get the lesser
**              of the two and set the layer_specific to
**              (lesser_size - OBEX_HDR_OFFSET).
**              If composing a header for the CONNECT request (there is no
**              client handle yet), use OBX_HANDLE_NULL as the handle.
**
** Returns      BT_HDR *.
**
*******************************************************************************/
OBX_API extern BT_HDR *OBX_HdrInit(tOBX_HANDLE handle, UINT16 pkt_size);

/*******************************************************************************
**
** Function     OBX_AddNameHdr
**
** Description  This function is called to add an OBEX Name header
**              to an OBEX packet.
**
** Returns      TRUE, if the header is added successfully.
**              FALSE, if the operation failed. p_pkt is not altered.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_AddNameHdr(BT_HDR *p_pkt, UINT16 *p_name, UINT16 len);

/*******************************************************************************
**
** Function     OBX_CharToWchar
**
** Description  This function is called to convert ASCII to Unicode (UINT16).
**
** Returns      the length.
**
*******************************************************************************/
OBX_API extern UINT16 OBX_CharToWchar(UINT16 *w_str, char *a_str, UINT16 w_size);

/*******************************************************************************
**
** Function     OBX_AddAsciiNameHdr
**
** Description  This function is called to add an OBEX Name header
**              to an OBEX packet.
**
** Returns      TRUE, if the header is added successfully.
**              FALSE, if the operation failed. p_pkt is not altered.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_AddAsciiNameHdr(BT_HDR *p_pkt, char *p_name);

/*******************************************************************************
**
** Function     OBX_AddTypeHdr
**
** Description  This function is called to add an OBEX Type header
**              to an OBEX packet.
**
** Returns      TRUE, if the header is added successfully.
**              FALSE, if the operation failed. p_pkt is not altered.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_AddTypeHdr(BT_HDR *p_pkt, char *p_type);

/*******************************************************************************
**
** Function     OBX_AddLengthHdr
**
** Description  This function is called to add an OBEX Length header to an OBEX
**              packet. The Length header describes the total length in bytes of
**              the object.
**
** Returns      TRUE, if the header is added successfully.
**              FALSE, if the operation failed. p_pkt is not altered.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_AddLengthHdr(BT_HDR *p_pkt, UINT32 len);

/*******************************************************************************
**
** Function     OBX_AddTimeHdr
**
** Description  This function is called to add an OBEX Time header
**              to an OBEX packet.
**
** Returns      TRUE, if the header is added successfully.
**              FALSE, if the operation failed. p_pkt is not altered.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_AddTimeHdr(BT_HDR *p_pkt, char *p_time);

/*******************************************************************************
**
** Function     OBX_AddDescrHdr
**
** Description  This function is called to add an OBEX Description header
**              to an OBEX packet.
**
** Returns      TRUE, if the header is added successfully.
**              FALSE, if the operation failed. p_pkt is not altered.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_AddDescrHdr(BT_HDR *p_pkt, UINT16 *p_descr, UINT16 len);

/*******************************************************************************
**
** Function     OBX_AddAsciiDescrHdr
**
** Description  This function is called to add an OBEX Description header
**              to an OBEX packet.
**
** Returns      TRUE, if the header is added successfully.
**              FALSE, if the operation failed. p_pkt is not altered.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_AddAsciiDescrHdr(BT_HDR *p_pkt, char *p_descr);

/*******************************************************************************
**
** Function     OBX_AddTargetHdr
**
** Description  This function is called to add an OBEX Target header to an OBEX
**              packet. This header is most commonly used in Connect packets.
**
**              NOTE: The target header must be the first header in an OBEX message.
**
** Returns      TRUE, if the header is added successfully.
**              FALSE, if the operation failed. p_pkt is not altered.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_AddTargetHdr(BT_HDR *p_pkt, UINT8 *p_target, UINT16 len);

/*******************************************************************************
**
** Function     OBX_AddHttpHdr
**
** Description  This function is called to add an OBEX HTTP header
**              to an OBEX packet.
**
** Returns      TRUE, if the header is added successfully.
**              FALSE, if the operation failed. p_pkt is not altered.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_AddHttpHdr(BT_HDR *p_pkt, UINT8 *p_http, UINT16 len);

/*******************************************************************************
**
** Function     OBX_AddBodyHdr
**
** Description  This function is called to add an OBEX body header
**              to an OBEX packet.
**
**              NOTE: The body header must be the last header in an OBEX message.
**
** Returns      TRUE, if the header is added successfully.
**              FALSE, if the operation failed. p_pkt is not altered.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_AddBodyHdr(BT_HDR *p_pkt, UINT8 *p_body, UINT16 len, BOOLEAN end);

/* Alternate Body header functions: for non-blocking scenario */
/*******************************************************************************
**
** Function     OBX_AddBodyStart
**
** Description  This function is called to get the address to the beginning of
**              the byte sequence for an OBEX body header in an OBEX packet.
**
** Returns      The address to add body content.
**
*******************************************************************************/
OBX_API extern UINT8 *OBX_AddBodyStart(BT_HDR *p_pkt, UINT16 *p_len);

/*******************************************************************************
**
** Function     OBX_AddBodyEnd
**
** Description  This function is called to add the HI and the length of HV of an
**              OBEX body header to an OBEX packet. If end is TRUE, HI is
**              OBX_HI_BODY_END. If FALSE, HI is OBX_HI_BODY. It is assumed that
**              the actual value of the body has been copied into the OBEX packet.
**
** Returns      void
**
*******************************************************************************/
OBX_API extern void OBX_AddBodyEnd(BT_HDR *p_pkt, UINT8 *p, UINT16 len, BOOLEAN end);

/*******************************************************************************
**
** Function     OBX_AddWhoHdr
**
** Description  This function is called to add an OBEX Who header to an OBEX
**              packet.
**
** Note:        Who header is typically used in an OBEX CONNECT response packet
**              to indicate the UUID of the service that has accepted the
**              directed connection. If the server calls OBX_StartServer() with
**              specified target header, this OBEX implementation automatically
**              adds this WHO header to the CONNECT response packet. If
**              OBX_StartServer() is called with target header length as 0, the
**              OBEX API user is responsible to add the WHO header.
**
** Returns      TRUE, if the header is added successfully.
**              FALSE, if the operation failed. p_pkt is not altered.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_AddWhoHdr(BT_HDR *p_pkt, UINT8 *p_who, UINT16 len);

/*******************************************************************************
**
** Function     OBX_AddAppParamHdr
**
** Description  This function is called to add an OBEX Application Parameter
**              header to an OBEX packet. This header is used by the application
**              layer above OBEX to convey application specific information.
**
** Returns      TRUE, if the header is added successfully.
**              FALSE, if the operation failed. p_pkt is not altered.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_AddAppParamHdr(BT_HDR *p_pkt, tOBX_TRIPLET *p_triplet, UINT8 num);

/*******************************************************************************
**
** Function     OBX_AddDestNameHdr
**
** Description  This function is called to add an OBEX DestName header
**              to an OBEX packet.
**
** Returns      TRUE, if the header is added successfully.
**              FALSE, if the operation failed. p_pkt is not altered.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_AddDestNameHdr(BT_HDR *p_pkt, UINT16 *p_dest, UINT16 len);

/*******************************************************************************
**
** Function     OBX_AddAsciiDestNameHdr
**
** Description  This function is called to add an OBEX DestName header
**              to an OBEX packet.
**
** Returns      TRUE, if the header is added successfully.
**              FALSE, if the operation failed. p_pkt is not altered.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_AddAsciiDestNameHdr(BT_HDR *p_pkt, char *p_descr);

/*******************************************************************************
**
** Function     OBX_AddPermissionHdr
**
** Description  This function is called to add an OBEX Permission header to an OBEX
**              packet.
**              bit 0 is set for read permission
**              bit 1 is set for write permission
**              bit 2 is set for delete permission
**              bit 7 is set for modify permission
**
** Returns      TRUE, if the header is added successfully.
**              FALSE, if the operation failed. p_pkt is not altered.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_AddPermissionHdr(BT_HDR *p_pkt, UINT8 user, UINT8 group, UINT8 other);

/*******************************************************************************
**
** Function     OBX_Add1ByteHdr
**
** Description  This function is called to add a header with type as UINT8
**              to an OBEX packet.
**
** Returns      TRUE, if the header is added successfully.
**              FALSE, if the operation failed. p_pkt is not altered.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_Add1ByteHdr(BT_HDR *p_pkt, UINT8 id, UINT8 data);

/*******************************************************************************
**
** Function     OBX_Add4ByteHdr
**
** Description  This function is called to add a header with type as UINT32
**              to an OBEX packet.
**
** Returns      TRUE, if the header is added successfully.
**              FALSE, if the operation failed. p_pkt is not altered.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_Add4ByteHdr(BT_HDR *p_pkt, UINT8 id, UINT32 data);

/*******************************************************************************
**
** Function     OBX_AddByteStrStart
**
** Description  This function is called to get the address to the beginning of
**              the byte sequence for an OBEX header in an OBEX packet.
**
** Returns      The address to add the byte sequence.
**
*******************************************************************************/
OBX_API extern UINT8 *OBX_AddByteStrStart(BT_HDR *p_pkt, UINT16 *p_len);

/*******************************************************************************
**
** Function     OBX_AddByteStrHdr
**
** Description  This function is called to add a header with type as byte sequence
**              to an OBEX packet.
**
** Returns      TRUE, if the header is added successfully.
**              FALSE, if the operation failed. p_pkt is not altered.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_AddByteStrHdr(BT_HDR *p_pkt, UINT8 id, UINT8 *p_data, UINT16 len);

/*******************************************************************************
**
** Function     OBX_AddUnicodeHdr
**
** Description  This function is called to add a header with type as Unicode string
**              to an OBEX packet.
**
** Returns      TRUE, if the header is added successfully.
**              FALSE, if the operation failed. p_pkt is not altered.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_AddUnicodeHdr(BT_HDR *p_pkt, UINT8 id, UINT16 *p_data, UINT16 len);

/*******************************************************************************
**
** Function     OBX_AddTriplet
**
** Description  This function is called to add a header with type as byte sequence
**              to an OBEX packet.
**
** Note:        The byte sequence uses a Tag-Length-Value encoding scheme
**              These headers include: Application Parameters header
**                                     Authenticate Challenge header
**                                     Authenticate Response header
**
** Returns      TRUE, if the header is added successfully.
**              FALSE, if the operation failed. p_pkt is not altered.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_AddTriplet(BT_HDR *p_pkt, UINT8 id, tOBX_TRIPLET *p_triplet, UINT8 num);

/*******************************************************************************
**
** Function     OBX_CheckHdr
**
** Description  This function is called to check if the given OBEX packet
**              contains the specified header.
**
** Returns      NULL, if the header is not in the OBEX packet.
**              The pointer to the specified header beginning from HI.
**
*******************************************************************************/
OBX_API extern UINT8 *OBX_CheckHdr(BT_HDR *p_pkt, UINT8 id);

/*******************************************************************************
**
** Function     OBX_ReadNumHdrs
**
** Description  This function is called to check the number of headers in the
**              given OBEX packet
**
** Returns      number of headers.
**
*******************************************************************************/
OBX_API extern UINT8 OBX_ReadNumHdrs(BT_HDR *p_pkt, UINT8 *p_num_body);

/*******************************************************************************
**
** Function     OBX_ReadHdrLen
**
** Description  This function is called to check the length of the specified
**              header in the given OBEX packet.
**
** Returns      OBX_INVALID_HDR_LEN, if the header is not in the OBEX packet.
**              Otherwise the actual length of the header.
**
*******************************************************************************/
OBX_API extern UINT16 OBX_ReadHdrLen(BT_HDR *p_pkt, UINT8 id);

/*******************************************************************************
**
** Function     OBX_ReadNameHdr
**
** Description  This function is called to get the Name Header in the given
**              OBEX packet.
**
** Returns      TRUE, if the header is in the OBEX packet.
**              FALSE, otherwise.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_ReadNameHdr(BT_HDR *p_pkt, UINT16 *p_name, UINT16 *p_len);

/*******************************************************************************
**
** Function     OBX_WcharToChar
**
** Description  This function is called to convert Unicode (UINT16) to ASCII.
**
** Returns      void.
**
*******************************************************************************/
OBX_API extern void OBX_WcharToChar(char *a_str, UINT16 *w_str, UINT16 a_size) ;

/*******************************************************************************
**
** Function     OBX_ReadAsciiNameHdr
**
** Description  This function is called to get the Name Header in the given
**              OBEX packet. If Name header exists in the given OBEX packet,
**              it is converted to ASCII format and copied into p_name.
**
** Returns      TRUE, if the header is in the OBEX packet.
**              FALSE, otherwise.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_ReadAsciiNameHdr(BT_HDR *p_pkt, char *p_name, UINT16 max_len);

/*******************************************************************************
**
** Function     OBX_ReadTypeHdr
**
** Description  This function is called to get the Type Header in the given
**              OBEX packet.
**
** Returns      TRUE, if the header is in the OBEX packet.
**              FALSE, otherwise.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_ReadTypeHdr(BT_HDR *p_pkt, UINT8 **p_type, UINT16 *p_len);

/*******************************************************************************
**
** Function     OBX_ReadLengthHdr
**
** Description  This function is called to get the Length Header in the given
**              OBEX packet.
**
** Returns      TRUE, if the header is in the OBEX packet.
**              FALSE, otherwise.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_ReadLengthHdr(BT_HDR *p_pkt, UINT32 *p_len);

/*******************************************************************************
**
** Function     OBX_ReadTimeHdr
**
** Description  This function is called to get the Time Header in the given
**              OBEX packet.
**
** Returns      TRUE, if the header is in the OBEX packet.
**              FALSE, otherwise.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_ReadTimeHdr(BT_HDR *p_pkt, UINT8 **p_time, UINT16 *p_len);

/*******************************************************************************
**
** Function     OBX_ReadDescrHdr
**
** Description  This function is called to get the Description Header in the
**              given OBEX packet.
**
** Returns      TRUE, if the header is in the OBEX packet.
**              FALSE, otherwise.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_ReadDescrHdr(BT_HDR *p_pkt, UINT16 *p_descr, UINT16 *p_len);

/*******************************************************************************
**
** Function     OBX_ReadDestNameHdr
**
** Description  This function is called to get the DestName Header in the
**              given OBEX packet.
**
** Returns      TRUE, if the header is in the OBEX packet.
**              FALSE, otherwise.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_ReadDestNameHdr(BT_HDR *p_pkt, UINT16 *p_dest, UINT16 *p_len);

/*******************************************************************************
**
** Function     OBX_ReadAsciiDescrHdr
**
** Description  This function is called to get the Description Header in the
**              given OBEX packet. If Description header exists in the given
**              OBEX packet, it is converted to ASCII format and copied into
**              p_descr.
**
** Returns      TRUE, if the header is in the OBEX packet.
**              FALSE, otherwise.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_ReadAsciiDescrHdr(BT_HDR *p_pkt, char *p_descr, UINT16 max_len);

/*******************************************************************************
**
** Function     OBX_ReadAsciiDestNameHdr
**
** Description  This function is called to get the DestName Header in the
**              given OBEX packet. If DestName header exists in the given
**              OBEX packet, it is converted to ASCII format and copied into
**              p_descr.
**
** Returns      TRUE, if the header is in the OBEX packet.
**              FALSE, otherwise.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_ReadAsciiDestNameHdr(BT_HDR *p_pkt, char *p_dest, UINT16 max_len);

/*******************************************************************************
**
** Function     OBX_ReadTargetHdr
**
** Description  This function is called to get the Target Header in the
**              given OBEX packet.
**
** Returns      TRUE, if the header is in the OBEX packet.
**              FALSE, otherwise.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_ReadTargetHdr(BT_HDR *p_pkt, UINT8 **p_target, UINT16 *p_len, UINT8 next);

/*******************************************************************************
**
** Function     OBX_ReadHttpHdr
**
** Description  This function is called to get the HTTP Header in the
**              given OBEX packet.
**
** Returns      TRUE, if the header is in the OBEX packet.
**              FALSE, otherwise.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_ReadHttpHdr(BT_HDR *p_pkt, UINT8 **p_http, UINT16 *p_len, UINT8 next);

/*******************************************************************************
**
** Function     OBX_ReadBodyHdr
**
** Description  This function is called to get the Body Header in the
**              given OBEX packet.
**
** Returns      1, if a single header is in the OBEX packet.
**              2, if a end of body header is in the OBEX packet.
**              0, (FALSE) otherwise.
**
*******************************************************************************/
OBX_API extern UINT8 OBX_ReadBodyHdr(BT_HDR *p_pkt, UINT8 **p_body, UINT16 *p_len, BOOLEAN *p_end);

/*******************************************************************************
**
** Function     OBX_ReadWhoHdr
**
** Description  This function is called to get the Who Header in the
**              given OBEX packet.
**
** Returns      TRUE, if the header is in the OBEX packet.
**              FALSE, otherwise.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_ReadWhoHdr(BT_HDR *p_pkt, UINT8 **p_who, UINT16 *p_len);

/*******************************************************************************
**
** Function     OBX_ReadAppParamHdr
**
** Description  This function is called to get the Application Parameter Header
**              in the given OBEX packet.
**
** Returns      TRUE, if the header is in the OBEX packet.
**              FALSE, otherwise.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_ReadAppParamHdr(BT_HDR *p_pkt, UINT8 *p_tag, UINT8 **p_app_param, UINT8 *p_len, UINT8 next);

/*******************************************************************************
**
** Function     OBX_ReadPermissionHdr
**
** Description  This function is called to get the Application Parameter Header
**              in the given OBEX packet.
**
** Returns      TRUE, if the header is in the OBEX packet.
**              FALSE, otherwise.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_ReadPermissionHdr(BT_HDR *p_pkt, UINT8 *p_user, UINT8 *p_group, UINT8 *p_other);

/*******************************************************************************
**
** Function     OBX_Read1ByteHdr
**
** Description  This function is called to get the UINT8 HV of the given HI
**              in the given OBEX packet.
**
** Returns      TRUE, if the header is in the OBEX packet.
**              FALSE, otherwise.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_Read1ByteHdr(BT_HDR *p_pkt, UINT8 id, UINT8 *p_data);

/*******************************************************************************
**
** Function     OBX_Read4ByteHdr
**
** Description  This function is called to get the UINT32 HV of the given HI
**              in the given OBEX packet.
**
** Returns      TRUE, if the header is in the OBEX packet.
**              FALSE, otherwise.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_Read4ByteHdr(BT_HDR *p_pkt, UINT8 id, UINT32 *p_data);

/*******************************************************************************
**
** Function     OBX_ReadByteStrHdr
**
** Description  This function is called to get the byte sequence HV of the given
**              HI in the given OBEX packet.
**
** Returns      TRUE, if the header is in the OBEX packet.
**              FALSE, otherwise.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_ReadByteStrHdr(BT_HDR *p_pkt, UINT8 id, UINT8 **p_data, UINT16 *p_len, UINT8 next);

/*******************************************************************************
**
** Function     OBX_ReadUnicodeHdr
**
** Description  This function is called to get the Unicode HV of the given
**              HI in the given OBEX packet.
**
** Returns      TRUE, if the header is in the OBEX packet.
**              FALSE, otherwise.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_ReadUnicodeHdr(BT_HDR *p_pkt, UINT8 id, UINT16 *p_data, UINT16 *p_len);

/*******************************************************************************
**
** Function     OBX_ReadTriplet
**
** Description  This function is called to get the Triplet HV of the given
**              HI in the given OBEX packet.
**
** Returns      TRUE, if the header is in the OBEX packet.
**              FALSE, otherwise.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_ReadTriplet(BT_HDR *p_pkt, UINT8 id, tOBX_TRIPLET *p_triplet, UINT8 *p_num);

/*******************************************************************************
**
** Function     OBX_ReadActionIdHdr
**
** Description  This function is called to get the HV of the Action ID header
**              in the given OBEX packet.
**
** Returns      TRUE, if the header is in the OBEX packet.
**              FALSE, otherwise.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_ReadActionIdHdr(BT_HDR *p_pkt, UINT8 *p_data);

/*******************************************************************************
**
** Function     OBX_ReadChallenge
**
** Description  This function is called to read the Realm and options of the
**              Authentication Challenge Header in the given OBEX packet.
**
** Returns      TRUE, if the header is in the OBEX packet.
**              FALSE, otherwise.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_ReadChallenge(BT_HDR *p_pkt, tOBX_CHARSET *p_charset,
        UINT8 **p_realm, UINT8 *p_len,
        tOBX_AUTH_OPT *p_option);

/*******************************************************************************
**
** Function     OBX_ReadAuthParams
**
** Description  This function is called to read the User ID of the
**              Authentication Response Header in the given OBEX packet.
**
** Returns      TRUE, if the header is in the OBEX packet.
**              FALSE, otherwise.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_ReadAuthParams(BT_HDR *p_pkt, UINT8 **p_userid, UINT8 *p_len,
        BOOLEAN *is_challenged, tOBX_AUTH_OPT *p_option);


/*******************************************************************************
**
** Function         utfc_16_to_8
**
** Description      Convert a UTF-16 array to a null-terminated UTF-8 string.
**                  Illegal characters are skipped.
**
** Returns          Length of UTF-8 string in bytes.
**
*******************************************************************************/
OBX_API extern UINT16 utfc_16_to_8(UINT8 *p_utf8, UINT16 utf8_len, UINT16 *p_utf16, UINT16 utf16_len);

/*******************************************************************************
**
** Function         utfc_8_to_16
**
** Description      Convert a null-terminated UTF-8 string to a UTF-16 array.
**                  Illegal characters are skipped.
**
** Returns          Length of UTF-16 array.
**
*******************************************************************************/
OBX_API extern UINT16 utfc_8_to_16(UINT16 *p_utf16, UINT16 utf16_len, UINT8 *p_utf8);

/*******************************************************************************
**
** Function     OBX_AddUtf8NameHdr
**
** Description  This function is called to add an OBEX Name header
**              to an OBEX packet.
**
** Returns      TRUE, if the header is added successfully.
**              FALSE, if the operation failed. p_pkt is not altered.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_AddUtf8NameHdr(BT_HDR *p_pkt, UINT8 *p_name);

/*******************************************************************************
**
** Function     OBX_AddUtf8DescrHdr
**
** Description  This function is called to add an OBEX Description header
**              to an OBEX packet.
**
** Returns      TRUE, if the header is added successfully.
**              FALSE, if the operation failed. p_pkt is not altered.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_AddUtf8DescrHdr(BT_HDR *p_pkt, UINT8 *p_descr);

/*******************************************************************************
**
** Function     OBX_AddUtf8DestNameHdr
**
** Description  This function is called to add an OBEX DestName header
**              to an OBEX packet.
**
** Returns      TRUE, if the header is added successfully.
**              FALSE, if the operation failed. p_pkt is not altered.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_AddUtf8DestNameHdr(BT_HDR *p_pkt, UINT8 *p_dest);

/*******************************************************************************
**
** Function     OBX_ReadUtf8NameHdr
**
** Description  This function is called to get the Name Header in the given
**              OBEX packet. If Name header exists in the given OBEX packet,
**              it is converted to UTF8 format and copied into p_name.
**
** Returns      TRUE, if the header is in the OBEX packet.
**              FALSE, otherwise.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_ReadUtf8NameHdr(BT_HDR *p_pkt, UINT8 *p_name, UINT16 max_len);

/*******************************************************************************
**
** Function     OBX_ReadUtf8DescrHdr
**
** Description  This function is called to get the Description Header in the
**              given OBEX packet. If Description header exists in the given
**              OBEX packet, it is converted to UTF8 format and copied into
**              p_descr.
**
** Returns      TRUE, if the header is in the OBEX packet.
**              FALSE, otherwise.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_ReadUtf8DescrHdr(BT_HDR *p_pkt, UINT8 *p_descr, UINT16 max_len);

/*******************************************************************************
**
** Function     OBX_ReadUtf8DestNameHdr
**
** Description  This function is called to get the DestName Header in the
**              given OBEX packet.
**
** Returns      TRUE, if the header is in the OBEX packet.
**              FALSE, otherwise.
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_ReadUtf8DestNameHdr(BT_HDR *p_pkt, UINT8 *p_dest, UINT16 max_len);

/*******************************************************************************
**
** Function     OBX_VerifyResponse
**
** Description  This function is called by the client to verify the challenge
**              response.
**
** Returns      TRUE, if successful.
**              FALSE, if authentication failed
**
*******************************************************************************/
OBX_API extern BOOLEAN OBX_VerifyResponse(UINT32 nonce_u32, UINT8 *p_password, UINT8 password_len, UINT8 *p_response);

#ifdef __cplusplus
}
#endif

#endif /* OBX_API_H */
