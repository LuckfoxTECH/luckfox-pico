/*****************************************************************************
**
**  Name:       bpp_api.h
**
**  File:       Application Programming Interface to the Basic Printing Profile
**
**  Copyright (c) 2000-2005, WIDCOMM Inc., All Rights Reserved.
**  WIDCOMM Bluetooth Core. Proprietary and confidential.
**
*****************************************************************************/
#ifndef BPP_API_H
#define BPP_API_H

#include "bt_target.h"
#include "obx_api.h"
#include "goep_fs.h"

/*****************************************************************************
**   Macros and Enumeration Definitions
*****************************************************************************/

#define BPP_PR_MAX_AUTH_KEY_SIZE    16

#define BPP_CHAR_REPERTOIRES_SIZE   16    /* 128 bit */
#define BPP_VERSION                 0x0100

#ifndef BPP_MAX_NAME_LEN
#define BPP_MAX_NAME_LEN            63
#endif

#ifndef BPP_PATH_SEPARATOR
#define BPP_PATH_SEPARATOR   ((char) 0x2f)   /* 0x2f ('/'), or 0x5c ('\') */
#endif

#ifndef BPP_SND_OBJ_IDLE_TOUT
#define BPP_SND_OBJ_IDLE_TOUT       60
#endif

enum
{
    BPP_SND_START_EVT,          /* 0 */
    BPP_SND_CON_EVT,            /* 1 */
    BPP_SND_CLOSE_EVT,          /* 2 */
    BPP_SND_AUTH_EVT,           /* 3 */

    BPP_SND_PROGRESS_EVT,       /* 4 */
    BPP_SND_DOC_CMPL_EVT,       /* 5 */
    BPP_SND_ABORT_EVT,          /* 6 */

    /* detailed event for SOAP request , mapping to PR event index */
    BPP_SND_GET_PR_ATTR_EVT,    /* 7  Get Printer Attributes response received    */
    BPP_SND_CREATE_JOB_EVT,     /* 8  Create Job response received                */
    BPP_SND_GET_JOB_ATTR_EVT,   /* 9  Get Job Attributes response received        */
    BPP_SND_CANCEL_JOB_EVT,     /* 10 Cancel Job response received                */
    BPP_SND_GET_EVENT_EVT,      /* 11 Get Event response received                 */
    BPP_SND_CREATE_PREC_JOB_EVT,/* 12 Create Precise Job response received        */
    BPP_SND_GET_MARGIN_EVT,     /* 13 Get Margins response received               */
    BPP_SND_GET_REF_OBJ_EVT,    /* 14 Get Reference object request received in object channel */

    BPP_SND_TIMEOUT_EVT,        /* 15 */
    BPP_SND_API_ERR_EVT         /* 16 Is this needed? Or returns in status of other events */
};

typedef UINT16  tBPP_SND_EVT;
#define BPP_SND_INVALID_EVENT    0xFF

/* Printer events */
enum
{
    BPP_PR_START_EVT,           /* 0  result of BPP_StartPrinter                 */
    BPP_PR_CON_EVT,             /* 1  a sender has connected on specified channel      */
    BPP_PR_CLOSE_EVT,           /* 2  a sender has disconnected the specified channel  */
    BPP_PR_PASSWORD_EVT,        /* 3  OBEX authentication response is received   */

    BPP_PR_FILE_NAME_EVT,       /* 4  File Push request received                 */
    BPP_PR_PUT_END_EVT,         /* 5  the last packet of FilePush or SendDocument*/
    BPP_PR_ABORT_EVT,           /* 6  an ongoing operation is aborted            */

    BPP_PR_GET_PR_ATTRS_EVT,    /* 7  Get Printer Attributes request received    */
    BPP_PR_CREATE_JOB_EVT,      /* 8  Create Job request received                */
    BPP_PR_GET_JOB_ATTRS_EVT,   /* 9  Get Job Attributes request received        */
    BPP_PR_CANCEL_JOB_EVT,      /* 10 Cancel Job request received                */
    BPP_PR_GET_EVENT_EVT,       /* 11 Get Event request received                 */
    BPP_PR_CREATE_PREC_JOB_EVT, /* 12 Create Precise Job request received        */
    BPP_PR_GET_MARGINS_EVT,     /* 13 Get Margins request received               */

    BPP_PR_CANCEL_GET_EVENT_EVT,/* 14 Cancel Get Event request received          */
    BPP_PR_TIMEOUT_EVT,         /* 15 timeout event reported from OBX */
    BPP_PR_SEND_DOC_EVT,        /* 16 Send Document request received             */
    BPP_PR_GET_END_EVT,         /* 17 the last packet of BPP_GetRefObj is received*/
    BPP_PR_SDP_EVT,             /* 18 report the SDP result of BPP_ConRefObjService() */
    BPP_PR_API_ERR_EVT,
    BPP_REF_OBJ_RSP_EVT
};
typedef UINT16 tBPP_PR_EVT;
#define BPP_PR_INVALID_EVENT    0xFF

/* Operation Status Codes -- BPP spec 16.2 */
#define BPP_OK                      0x0000  /* successful-ok */
#define BPP_OK_IGN_OR_SUBST_ATTRS   0x0001  /* successful-ok-ignored-or-substituted-attributes */
#define BPP_OK_CONFL_ATTRS          0x0002  /* successful-ok-conflicting-attributes */
#define BPP_MAX_OK_OPER_STATUS      BPP_OK_CONFL_ATTRS
#define BPP_CERR_BAD_REQ            0x0400  /* client-error-bad-request */
#define BPP_CERR_FORBIDDEN          0x0401  /* client-error-forbidden */
#define BPP_CERR_NOT_AUTHENTICATED  0x0402  /* client-error-not-authenticated */
#define BPP_CERR_NOT_AUTHORIZED     0x0403  /* client-error-not-authorized */
#define BPP_CERR_NOT_POSSIBLE       0x0404  /* client-error-not-possible */
#define BPP_CERR_TIMEOUT            0x0405  /* client-error-timeout */
#define BPP_CERR_NOT_FOUND          0x0406  /* client-error-not-found */
#define BPP_CERR_GONE               0x0407  /* client-error-gone */
#define BPP_CERR_REQ_ENT_TOO_LONG   0x0408  /* client-error-request-entity-too-large */
#define BPP_CERR_REQ_VAL_TOO_LONG   0x0409  /* client-error-request-value-too-long */
#define BPP_CERR_DOC_FORM_NOT_SUPP  0x040a  /* client-error-document-format-not-supported */
#define BPP_CERR_ATS_O_VLS_NOT_SUPP 0x040b  /* client-error-attributes-or-values-not-supported */
#define BPP_CERR_URI_SCH_NOT_SUPP   0x040c  /* client-error-uri-scheme-not-supported */
#define BPP_CERR_CH_SET_NOT_SUPP    0x040d  /* client-error-charset-not-supported */
#define BPP_CERR_CONFL_ATTRS        0x040e  /* client-error-conflicting-attributes */
#define BPP_CERR_COMPR_NOT_SUPP     0x040f  /* client-error-compression-not-supported */
#define BPP_CERR_COMPR_ERR          0x0410  /* client-error-compression-error */
#define BPP_CERR_DOC_FORM_ERR       0x0411  /* client-error-document-format-error */
#define BPP_CERR_DOC_ACC_ERR        0x0412  /* client-error-document-access-error */
#define BPP_CERR_MD_NOT_LOADED      0x0418  /* client-error-media-not-loaded */
#define BPP_SERR_INT_ERR            0x0500  /* server-error-internal-error */
#define BPP_SERR_OP_NOT_SUPP        0x0501  /* server-error-operation-not-supported */
#define BPP_SERR_SRV_UNAVAIL        0x0502  /* server-error-service-unavailable */
#define BPP_SERR_VER_NOT_SUPP       0x0503  /* server-error-version-not-supported */
#define BPP_SERR_DEV_ERR            0x0504  /* server-error-device-error */
#define BPP_SERR_TMP_ERR            0x0505  /* server-error-temporary-error */
#define BPP_SERR_NOT_ACC_JOBS       0x0506  /* server-error-not-accepting-jobs */
#define BPP_SERR_BUSY               0x0507  /* server-error-busy */
#define BPP_SERR_JOB_CANC           0x0508  /* server-error-job-canceled */
#define BPP_SERR_MPLE_JOBS_NOT_SUPP 0x0509  /* server-error-multiple-document-jobs-not-supported */

typedef UINT16 tBPP_OPER_STATUS;


/* Printer State */
enum
{
    BPP_PR_STATE_UNDEF,          /* undefined */
    BPP_PR_IDLE,
    BPP_PR_PROCESSING,
    BPP_PR_STOPPED,
    BPP_MAX_PR_STATE
};
typedef UINT16 tBPP_PR_STATE;

/* Printer State Reason Undefined*/
enum
{
    BPP_ST_RS_UNDEF,          /* undefined */
    BPP_ST_RS_NONE,
    BPP_ST_RS_ATT_REQ,
    BPP_ST_RS_MED_JAM,
    BPP_ST_RS_PAUSED,
    BPP_ST_RS_DOOR_OPEN,
    BPP_ST_RS_MED_LOW,
    BPP_ST_RS_MED_EMP,
    BPP_ST_RS_OUT_AFUL,
    BPP_ST_RS_OUT_FUL,
    BPP_ST_RS_MARKER_L,
    BPP_ST_RS_MARKER_E,
    BPP_ST_RS_MARKER_F
};
typedef UINT16 tBPP_PR_ST_REASON;

enum
{
    BPP_JOB_STATE_UNDEF,
    BPP_PRINTING,   /* 1 */
    BPP_WAITING,    /* 2 */
    BPP_STOPPED,    /* 3 */
    BPP_COMPLETED,  /* 4 */
    BPP_ABORTED,    /* 5 */
    BPP_CANCELLED,  /* 6 */
    BPP_JS_UNKNOWN, /* 7 */
    BPP_MAX_JOB_STATE
};
typedef UINT16 tBPP_JOB_STATE;

/* Operation codes for BPP_GetRequest() */
enum
{
    BPP_OP_INVALID      = 0x0,  /* GetCapabilities request/response */
    BPP_OP_GET_PR_ATTR,         /* 0x1 GetPrinterAttributes request/response */
    BPP_OP_GET_JOB_ATTR,        /* 0x2 GetJobAttributes request/response */
    BPP_OP_GET_CREATE_JOB,      /* 0x3 CreateJob GET request/response */
    BPP_OP_GET_CANCEL_JOB,      /* 0x4 CancelJob request/response */
    BPP_OP_GET_EVENT,           /* 0x5 GetEvent request/response */
    BPP_OP_GET_MARGINS,         /* 0x6 GetMargin request/response */
    BPP_OP_GET_REF_OBJ,         /* 0x7 GetReferencedObjects request/response */
    BPP_MAX_GET_OP              /* 0x8   */
};
typedef UINT8  tBPP_GET_OP;
#define BPP_OP_FIRST_GET        BPP_OP_GET_PR_ATTR
#define BPP_OP_TYPE_BIT         0x80
#define BPP_OP_TYPE_GET         0x00
#define BPP_OP_TYPE_PUT         0x80

#define BPP_INVALID_BIT_MASK            0x000F

/* Sides/SidesSupported */
enum
{
    BPP_SIDES_UNDEF,             /* undefined */
    BPP_ONE_SIDED,
    BPP_TWO_SIDED_LONG_EDGE,
    BPP_TWO_SIDED_SHORT_EDGE,
    BPP_MAX_SIDES
};
typedef UINT16 tBPP_SIDES;

#define BPP_BIT_ONE_SIDED               0x0010
#define BPP_BIT_TWO_SIDED_LONG_EDGE     0x0020
#define BPP_BIT_TWO_SIDED_SHORT_EDGE    0x0040
typedef UINT16 tBPP_SIDES_B;

/* Orientation/OrientationsSupported */
enum
{
    BPP_ORIENTATION_UNDEF,
    BPP_PORTRAIT,
    BPP_LANDSCAPE,
    BPP_REVERSE_LANDSCAPE,
    BPP_REVERSE_PORTRAIT,
    BPP_MAX_ORIENT
};
typedef UINT16 tBPP_ORIENTATION;

#define BPP_BIT_PORTRAIT                0x0010
#define BPP_BIT_LANDSCAPE               0x0020
#define BPP_BIT_REVERSE_LANDSCAPE       0x0040
#define BPP_BIT_REVERSE_PORTRAIT        0x0080
typedef UINT16 tBPP_ORIENTATION_B;

/* PrinterQuality/PrinterQualitySupported */
enum
{
    BPP_PR_QUALITY_UNDEF,
    BPP_DRAFT,
    BPP_NORMAL,
    BPP_HIGH,
    BPP_MAX_PR_QUAL
};
typedef UINT16 tBPP_PR_QUALITY;

#define BPP_BIT_DRAFT                   0x0010
#define BPP_BIT_NORMAL                  0x0020
#define BPP_BIT_HIGH                    0x0040
typedef UINT16 tBPP_PR_QUALITY_B;

/* MediaType */
enum
{
    BPP_MEDIA_TYPE_UNDEF,
    BPP_STATIONERY,             /* 1 */
    BPP_STATIONERY_COATED,      /* 2 */
    BPP_STATIONERY_INKJET,      /* 3 */
    BPP_STATIONERY_PREPRINTED,  /* 4 */
    BPP_STATIONERY_LETTERHEAD,  /* 5 */
    BPP_STATIONERY_PREPUNCHED,  /* 6 */
    BPP_STATIONERY_FINE,        /* 7 */
    BPP_STATIONERY_HEAVYWEIGHT, /* 8 */
    BPP_STATIONERY_LIGHTWEIGHT, /* 9 */
    BPP_TRANSPARENCY,           /* 10 */
    BPP_ENVELOPE,               /* 11 */
    BPP_ENVELOPE_PLAIN,         /* 12 */
    BPP_ENVELOPE_WINDOW,        /* 13 */
    BPP_CONTINUOUS,             /* 14 */
    BPP_CONTINUOUS_LONG,        /* 15 */
    BPP_CONTINUOUS_SHORT,       /* 16 */
    BPP_TAB_STOCK,              /* 17 */
    BPP_PRE_CUT_TABS,           /* 18 */
    BPP_FULL_CUT_TABS,          /* 19 */
    BPP_MULTI_PART_FORM,        /* 20 */
    BPP_LABELS,                 /* 21 */
    BPP_MULTI_LAYER,            /* 22 */
    BPP_SCREEN,                 /* 23 */
    BPP_SCREEN_PAGED,           /* 24 */
    BPP_PHOTOGRAPHIC,           /* 25 */
    BPP_PHOTOGRAPHIC_GLOSSY,    /* 26 */
    BPP_PHOTOGRAPHIC_HIGH_GLOSS,/* 27 */
    BPP_PHOTOGRAPHIC_SEMI_GLOSS,/* 28 */
    BPP_PHOTOGRAPHIC_SATIN,     /* 29 */
    BPP_PHOTOGRAPHIC_MATTE,     /* 30 */
    BPP_PHOTOGRAPHIC_FILM,      /* 31 */
    BPP_BACK_PRINT_FILM,        /* 32 */
    BPP_CARDSTOCK,              /* 33 */
    BPP_ROLL,                   /* 34 */
    BPP_MAX_MEDIA_TYPE
};
typedef UINT16 tBPP_MEDIA_TYPE;

#define BPP_BIT_STATIONERY          0x0010


/* Determine the number of UINT32's necessary for media types */
#define BPP_MTYPE_ARRAY_BITS    32          /* Number of bits in each array element */
#define BPP_MTYPE_ARRAY_INDEX (((UINT32)BPP_MAX_MEDIA_TYPE / BPP_MTYPE_ARRAY_BITS) + \
                                    (((UINT32)BPP_MAX_MEDIA_TYPE % BPP_MTYPE_ARRAY_BITS) ? 1 : 0))

#define BPP_PR_MAX_PR_ATTRS     19
#define BPP_PR_MAX_JOB_ATTRS    6
#define BPP_PR_MAX_CRT_ATTR     12
/* enumerations used in printer*/
enum
{
    BPP_ATTR_UNDEF,             /* 0  undefined */
    /* the following are used for printer attribute */
    BPP_ATTR_PR_NAME,           /* 1  string data       PrinterName */
    BPP_ATTR_PR_LOCATION,       /* 2  string data       PrinterLocation */
    BPP_ATTR_PR_STATE,          /* 3  tBPP_PR_STATE     PrinterState */
    BPP_ATTR_PR_STATE_REASONS,  /* 4  string data       PrinterStateReasons */
    BPP_ATTR_PR_DOC_FORMAT_SUPP,/* 5  string data (,)   DocumentFormatsSupported */
    BPP_ATTR_PR_COLOR_SUPP,     /* 6  boolean           ColorSupported */
    BPP_ATTR_PR_MAX_COPIES_SUPP,/* 7  UINT32            MaxCopiesSupported */
    BPP_ATTR_PR_SIDES_SUPP,     /* 8  tBPP_SIDES_B      SidesSupported */
    BPP_ATTR_PR_NBR_UP_SUPP,    /* 9  UINT32            NumberUpSupported */
    BPP_ATTR_PR_ORIENT_SUPP,    /* 10 tBPP_ORIENTATION_B OrientationsSupported */
    BPP_ATTR_PR_MEDIA_SIZE_SUPP,/* 11 string data (,)   MediaSizesSupported */
    BPP_ATTR_PR_MEDIA_TYPE_SUPP,/* 12 string data (,)   MediaTypesSupported */
    BPP_ATTR_PR_MEDIA_LOADED,   /* 13 tBPP_MEDIUM       MediaLoaded */
    BPP_ATTR_PR_QUALITY_SUPP,   /* 14 tBPP_PR_QUALITY_B PrintQualitySupported */
    BPP_ATTR_PR_QUEUED_JOB_CNT, /* 15 UINT32            QueuedJobCount */
    BPP_ATTR_PR_IMG_FORMAT_SUPP,/* 16 string data (,)   ImageFormatsSupported */
    BPP_ATTR_PR_BASIC_TXT_WIDTH,/* 17 UINT32            BasicTextPageWidth */
    BPP_ATTR_PR_BASIC_TXT_HIGHT,/* 18 UINT32            BasicTextPageHeight */
    BPP_ATTR_PR_GEN_CURR_OP,    /* 19 string data       PrinterGeneralCurrentOperator */
    /* the following are used for get job attribute */
    BPP_ATTR_JOB_ID,            /* 20 UINT32            JobId */
    BPP_ATTR_JOB_STATE,         /* 21 tBPP_JOB_STATE    JobState */
    BPP_ATTR_JOB_SHEETS_COMPL,  /* 22 INT32             JobMediaSheetsCompleted */
    BPP_ATTR_JOB_NR_INTERV_JOBS,/* 23 INT32             NumberOfInterveningJobs */
    BPP_ATTR_JOB_NAME,          /* 24 string data       JobName */
    BPP_ATTR_JOB_ORIG_USER,     /* 25 string data       JobOriginatingUserName */
    /* the following are used for create job */
    /*  also BPP_ATTR_JOB_ID, BPP_ATTR_JOB_NAME, BPP_ATTR_JOB_ORIG_USER) */
    BPP_ATTR_DOC_FORMAT,        /* 26 string data       DocumentFormat */
    BPP_ATTR_COPIES,            /* 27 UINT32            Copies */
    BPP_ATTR_SIDES,             /* 28 tBPP_SIDES        Sides */
    BPP_ATTR_NUMBER_UP,         /* 29 UINT32            NumberUp */
    BPP_ATTR_ORIENT_REQ,        /* 30 tBPP_ORIENTATION  OrientationRequested */
    BPP_ATTR_MEDIA_SIZE,        /* 31 string data       MediaSize */
    BPP_ATTR_PRINT_QUALITY,     /* 32 tBPP_PR_QUALITY   PrintQuality */
    BPP_ATTR_MEDIA_TYPE,        /* 33 tBPP_MEDIA_TYPE   MediaType */
    BPP_ATTR_CANCEL_LOST_LINK,  /* 34 boolean           CancelOnLostLink */
    /* used in GetMarginRsp */
    BPP_ATTR_MARGINS,            /* 35 string data       Margins */
    BPP_ATTR_ALL                /* 36 if BPP_PR_GET_PR_ATTRS_EVT does not specify the requested attributes */
};
typedef UINT8 tBPP_ATTR;

#define BPP_PATR_BIT_NAME               0x00000010          /* 1   PrinterName */
#define BPP_PATR_BIT_LOCATION           0x00000020          /* 2   PrinterLocation */
#define BPP_PATR_BIT_STATE              0x00000040          /* 3   PrinterState */
#define BPP_PATR_BIT_STATE_REASONS      0x00000080          /* 4   PrinterStateReasons */
#define BPP_PATR_BIT_DOC_FORMAT_SUPP    0x00000100          /* 5   DocumentFormatsSupported */
#define BPP_PATR_BIT_COLOR_SUPP         0x00000200          /* 6   ColorSupported */
#define BPP_PATR_BIT_MAX_COPIES_SUPP    0x00000400          /* 7   MaxCopiesSupported */
#define BPP_PATR_BIT_SIDES_SUPP         0x00000800          /* 8   SidesSupported */
#define BPP_PATR_BIT_NBR_UP_SUPP        0x00001000          /* 9   NumberUpSupported */
#define BPP_PATR_BIT_ORIENT_SUPP        0x00002000          /* 10  OrientationsSupported */
#define BPP_PATR_BIT_MEDIA_SIZE_SUPP    0x00004000          /* 11  MediaSizesSupported */
#define BPP_PATR_BIT_MEDIA_TYPE_SUPP    0x00008000          /* 12  MediaTypesSupported */
#define BPP_PATR_BIT_MEDIA_LOADED       0x00010000          /* 13  MediaLoaded */
#define BPP_PATR_BIT_QUALITY_SUPP       0x00020000          /* 14  PrintQualitySupported */
#define BPP_PATR_BIT_QUEUED_JOB_CNT     0x00040000          /* 15  QueuedJobCount */
#define BPP_PATR_BIT_IMG_FORMAT_SUPP    0x00080000          /* 16  ImageFormatsSupported */
#define BPP_PATR_BIT_BASIC_TXT_WIDTH    0x00100000          /* 17  BasicTextPageWidth */
#define BPP_PATR_BIT_BASIC_TXT_HIGHT    0x00200000          /* 18  BasicTextPageHeight */
#define BPP_PATR_BIT_GEN_CURR_OP        0x00400000          /* 19  PrinterGeneralCurrentOperator */

typedef UINT32 tBPP_PR_ATTR_B;
#define BPP_PR_ATTR_VALID_MASK          0x007ffff0

#define BPP_JATR_BIT_ID                 0x00000010          /* 20   JobId */
#define BPP_JATR_BIT_STATE              0x00000020          /* 21   JobState */
#define BPP_JATR_BIT_SHEETS_COMPL       0x00000040          /* 22   JobMediaSheetsCompleted */
#define BPP_JATR_BIT_NR_INTERV_JOBS     0x00000080          /* 23   NumberOfInterveningJobs */
#define BPP_JATR_BIT_NAME               0x00000100          /* 24   JobName */
#define BPP_JATR_BIT_ORIG_USER          0x00000200          /* 25   JobOriginatingUserName */

typedef UINT32 tBPP_JOB_ATTR_B;
#define BPP_JOB_ATTR_VALID_MASK         0x000003f0

#define BPP_MAX_NUM_ATTRS   19
#define BPP_FIRST_JOB_ATTR  BPP_ATTR_JOB_ID
#define BPP_TID_JOB_NAME    0x0D


enum
{
    BPP_SUCCESS,
    BPP_ERROR,
    BPP_BAD_REQUEST,
    BPP_BAD_PARAMS,
    BPP_RESOURCE,
    BPP_NOT_FOUND,
    BPP_SDP_ERROR,
    BPP_SOAP_ERROR,
    BPP_UNSUPTD_TYPE
};
typedef UINT8 tBPP_STATUS;

enum
{
    BPP_OFFLINE        = 0, /* Non discoverable. non Connectable.  */
    BPP_BONDING,            /* Pairable mode. Discoverable.   */
    BPP_PRIVATE_ONLINE,     /* Non discoverable. Connectable. */
    BPP_PUBLIC_ONLINE       /* Discoverable. Connectable.      */
};
#define BPP_MAX_PR_MODE         BPP_PUBLIC_ONLINE
typedef UINT8 tBPP_PR_MODE;

#define BPP_PR_MODE_OP_CONNECT      0x01 /* set connectable mode */
#define BPP_PR_MODE_OP_DISCOV_GEN   0x02 /* set discoverable (general) mode */
#define BPP_PR_MODE_OP_DISCOV_LMTD  0x04 /* set discoverable (limited) mode */
#define BPP_PR_MODE_OP_PAIRING      0x08 /* set allow/disallow pairing mode */

enum
{
    BPP_JOB_CHANNEL = 1,
    BPP_STATUS_CHANNEL,
    BPP_OBJECT_CHANNEL,
    BPP_INVALID_CHANNEL
};

typedef UINT8 tBPP_CHANNEL;

/*****************************************************************************
**   Type Definitions
*****************************************************************************/
typedef UINT8 tBPP_UINT128[BPP_CHAR_REPERTOIRES_SIZE];

typedef char tBPP_MS_STR[BPP_MEDIA_SIZE_LEN];

typedef UINT32 tBPP_JOB_ID;

typedef UINT8   tBPP_HANDLE;

typedef struct
{
    UINT8   *p_realm;    /* The realm that represent the server */
    UINT8   realm_len;   /* The length of p_realm. */
    tOBX_CHARSET    charset;    /* the charset for p_realm */
    BOOLEAN         userid_required; /* TRUE, if the peer requests for user ID */
} tBPP_AUTH_EVT;

typedef struct
{
    UINT8   *p_pswd;    /* The password. */
    UINT8   *p_userid;  /* The user id. */
    UINT8   pswd_len;   /* The length of p_password. */
    UINT8   userid_len; /* The length of p_userid. */
} tBPP_AUTH;

typedef struct
{
    tBPP_MS_STR         msize;   /* loaded medium size */
    tBPP_MEDIA_TYPE     mtype;   /* loaded medium type */
} tBPP_MEDIUM;

enum
{
    BPP_TYPE_UNKNOWN,
    BPP_TYPE_UINT,          /* 1  use tBPP_ATTRIBUTE.u.value as UINT32 */
    BPP_TYPE_BOOLEAN,       /* 2  use tBPP_ATTRIBUTE.u.value as BOOLEAN */
    BPP_TYPE_BYTESEQ,       /* 3  use tBPP_ATTRIBUTE.u.p_data */
    BPP_TYPE_SIDES_B,       /* 4  use tBPP_ATTRIBUTE.u.value as tBPP_SIDES_B (multiple bits maybe set) */
    BPP_TYPE_ORIENTATION_B, /* 5  use tBPP_ATTRIBUTE.u.value as tBPP_ORIENTATION_B (multiple bits maybe set) */
    BPP_TYPE_PR_QUALITY_B,  /* 6  use tBPP_ATTRIBUTE.u.value as tBPP_PR_QUALITY_B (multiple bits maybe set) */
    BPP_TYPE_PR_STATE,      /* 7  use tBPP_ATTRIBUTE.u.value as tBPP_PR_STATE */
    BPP_TYPE_SIDES,         /* 8  use tBPP_ATTRIBUTE.u.value as tBPP_SIDES */
    BPP_TYPE_ORIENTATION,   /* 9  use tBPP_ATTRIBUTE.u.value as tBPP_ORIENTATION */
    BPP_TYPE_PR_QUALITY,    /* 10 use tBPP_ATTRIBUTE.u.value as tBPP_PR_QUALITY */
    BPP_TYPE_MEDIA_TYPE,    /* 11 use tBPP_ATTRIBUTE.u.value as tBPP_MEDIA_TYPE */
    BPP_TYPE_JOB_STATE,     /* 12 use tBPP_ATTRIBUTE.u.value as tBPP_JOB_STATE */
    BPP_TYPE_MEDIA_LOADED,  /* 13 use tBPP_ATTRIBUTE.u.p_medium as tBPP_MEDIUM,
                             *    use tBPP_ATTRIBUTE.len is the number of tBPP_MEDIUM */
    BPP_TYPE_PR_REQ,        /* 14 internal */
    BPP_TYPE_PR_RSP,        /* 15 internal */
    BPP_TYPE_PR_ATTRS,      /* 16 internal */
    BPP_TYPE_JOB_ATTR,      /* 17 internal */
    BPP_TYPE_PR_ST_REASON   /* 18 use tBPP_ATTRIBUTE.u value as tBPP_PR_ST_REASON */
};
typedef UINT8   tBPP_ATTR_TYPE;

enum
{
    BPP_SERVICE_DIR_PRINT,
    BPP_SERVICE_PRINT_STATUS,
    BPP_SERVICE_REF_OBJ,
    BPP_SERVICE_REF_PRINT
};
typedef UINT8 tBPP_SERVICE;
typedef union
{
    tBPP_MEDIUM         *p_medium;
    UINT8               *p_data;    /* Pointer to Attribute data. */
    UINT32              value;      /* Attribute value (integer) */
    BOOLEAN             boln;       /* Attribute value (boolean) */
    UINT16              enumu;      /* em=numeration type */
} tBPP_ATTR_U;

typedef struct
{
    tBPP_ATTR_U     u;
    UINT16          len;        /* Length of data. */
    tBPP_ATTR_TYPE  type;
    tBPP_ATTR       attr;       /* Attribute name. */
} tBPP_ATTRIBUTE;

/* data type used by BPP_GetPrAttrsRsp(), BPP_GetJobAttrsRsp(), BPP_GetEventRsp() */
typedef struct
{
    BT_HDR          *p_abuf;    /* if not NULL, contains p_attrs */
    BT_HDR          *p_dbuf;    /* if not NULL, contains p_attrs[xx].p_data */
    tBPP_ATTRIBUTE  *p_attrs;   /* the array of attributes */
    UINT8           num_attrs;  /* the number of attributes in the response */
} tBPP_SOAP_RSP;

typedef struct
{
    UINT8  *p;          /* NULL if not used, else MUST be NULL terminated */
    UINT16 len;         /* string len */
} tBPP_STR;

/* data type associated with BPP_PR_START_EVT */
typedef struct
{
    tBPP_HANDLE     *p_handle;
    UINT8           num_handles; /* the number of BPP handle returned.
                                  * this number is less than or equal to
                                  * the given tBPP_START_INFO.max_con */
} tBPP_PR_START;

/* data type associated with BPP_PR_PASSWORD_EVT */
typedef struct
{
    UINT8           *p_userid;
    UINT8           userid_len;
    BOOLEAN         userid_required;    /* TRUE if user ID is required in response (rechallanged)  */
} tBPP_PR_PASSWORD;

/* data type associated with BPP_PR_GET_PR_ATTRS_EVT */
typedef struct
{
    tBPP_ATTR       *p_attrs;   /* the array of requested attributes */
    UINT8           num_attrs;  /* the number of attributes in the request */
    BOOLEAN         final;      /* last packet of the SOAP request */
} tBPP_PR_ATTR_REQ;

/* data type associated with BPP_PR_GET_JOB_ATTRS_EVT */
typedef struct
{
    tBPP_PR_ATTR_REQ    attr;
    tBPP_JOB_ID         id;     /* the JOB ID.  */
} tBPP_PR_JOB_ATR_REQ;

/* data type associated with BPP_PR_CREATE_JOB_EVT, BPP_PR_CREATE_PREC_JOB_EVT */
typedef struct
{
    BT_HDR          *p_buf;     /* if not NULL, contains p_attrs[xx].p_data */
    tBPP_ATTRIBUTE  *p_attrs;   /* the array of attributes */
    UINT8           num_attrs;  /* the number of attributes in the request */
    BOOLEAN         final;      /* last packet of the create (precise) job request */
} tBPP_ATTR_STRUCT;

typedef tBPP_ATTR_STRUCT tBPP_PR_CREATE_REQ;

/* data type associated with BPP_PR_GET_MARGINS_EVT */
typedef tBPP_PR_CREATE_REQ tBPP_PR_MARGINS_REQ;

/* data type associated with BPP_PR_FILE_NAME_EVT, BPP_PR_SEND_DOC_EVT */
typedef struct
{
    UINT8           *p_buf;     /* the GKI buffer that holds type, name and desc */
    tBPP_JOB_ID     id;         /* the JOB ID. 0 is invalid and is used with BPP_PR_FILE_NAME_EVT */
    tBPP_STR        type;       /* the Type header (MIME media type of the document) */
    tBPP_STR        name;       /* the optional Name header */
    tBPP_STR        desc;       /* the optional description header */
} tBPP_PR_NAME_REQ;

/* data type associated with BPP_PR_SDP_EVT */
typedef struct
{
    UINT16              sdp_status;
    UINT8               scn;
} tBPP_SDP_DATA;

typedef struct
{
    tBPP_JOB_ID     jid;
    BOOLEAN         final;
} tBPP_JOB;

/* union of the event date */
typedef union
{
    /* data type        member      event   */
    tBPP_PR_START       start;  /* BPP_PR_START_EVT */
    BD_ADDR             con;    /* BPP_PR_CON_EVT */
    tBPP_PR_PASSWORD    pswd;   /* BPP_PR_PASSWORD_EVT */
    tBPP_PR_NAME_REQ    name;   /* BPP_PR_FILE_NAME_EVT,
                                 * BPP_PR_SEND_DOC_EVT */
    tBPP_PR_ATTR_REQ    pr_atr; /* BPP_PR_GET_PR_ATTRS_EVT */
    tBPP_PR_JOB_ATR_REQ job_atr;/* BPP_PR_GET_JOB_ATTRS_EVT */
    tBPP_PR_CREATE_REQ  job;    /* BPP_PR_CREATE_JOB_EVT,
                                 * BPP_PR_CREATE_PREC_JOB_EVT */
    tBPP_PR_MARGINS_REQ margin; /* BPP_PR_GET_MARGINS_EVT */
    tBPP_JOB            jid;    /* BPP_PR_CANCEL_JOB_EVT,
                                 * BPP_PR_GET_EVENT_EVT*/
    char                *p_name;/* BPP_PR_ABORT_EVT */
    tOBX_RSP_CODE       rsp;    /* BPP_PR_GET_END_EVT */
    tBPP_SDP_DATA       sdp;    /* BPP_PR_SDP_EVT. sdp.scn=0 means SDP failed */
} tBPP_PR;

/* Data type used in BPP Sender side */
/* data type used by BPP_CreateJob() */
typedef struct
{
    char                *p_job_name;    /* if not NULL, contains JobName */
    char                *p_origin_usr;  /* if not NULL, contain the JobOriginatingUserName*/
    char                *p_mime_type;   /* mime type of the file to be print */
    char                *p_msize;       /* media size requested */
    UINT32              num_copies;     /* number of copy requested */
    UINT32              num_up;
    tBPP_SIDES          sides;
    tBPP_ORIENTATION    orientation;
    tBPP_PR_QUALITY     quality;        /* print quality */
    tBPP_MEDIA_TYPE     mtype;
    UINT8               cancel_lk;      /* cancel on Lost link : */
    /* 0 :do not cancel job when lk happen; 1: cancel job when lk ; 2: not specified;*/
} tBPP_CREATE_REQ;

/* BPP Sender call back event associated data */
typedef struct
{
    tBPP_MEDIA_TYPE         mtype;          /* media type */
    char                    *p_msize;       /* media size */
} tBPP_MEDIA_LOADED;

#ifndef BPP_MAX_MEDIA_LOADED
#define BPP_MAX_MEDIA_LOADED    8
#endif

typedef struct tbpp_str_list
{
    struct tbpp_str_list    *p_next;        /* Address of next entry. Null if end of list */
    char                    *p_str;         /* Null terminated string */
} tBPP_ATTR_STR_LIST;

typedef struct
{
    UINT32          max_copies;              /* Maximum number of copies */
    UINT32          max_number_up;           /* Maximum number of pages on a side */
    UINT32          queued_jobs;             /* Number of jobs in printer queue */
    UINT32          btp_width;               /* Basic text page width */
    UINT32          btp_height;              /* Basic text page height */
    UINT32          num_mloaded;             /* number of media loaded entries */
    UINT32          mtypes_mask[BPP_MTYPE_ARRAY_INDEX]; /* List of media types supported (bitmask). */
    tBPP_OPER_STATUS    op_sts;

    char            name[BPP_MAX_NAME_LEN + 1]; /* Name - may be truncated */
    char            loc[BPP_MAX_NAME_LEN + 1]; /* Location - may be truncated */
    char            gen_info[BPP_MAX_NAME_LEN + 1]; /* PrinterGeneralCurrentOperator */
    tBPP_MEDIA_LOADED mloaded[BPP_MAX_MEDIA_LOADED]; /* Array of loaded media */
    tBPP_PR_STATE   state;                   /* State - idle, processing, or stopped */
    tBPP_PR_ST_REASON  st_rsn;

    tBPP_SIDES_B    sides_mask;              /* BPP_BP_ONE_SIDED, BPP_BP_TWO_SIDED_LONG,
                                                and/or BPP_BP_TWO_SIDED_SHORT */

    tBPP_PR_QUALITY_B quality_mask;            /* BPP_BP_QUALITY_NORMAL, BPP_BP_QUALITY_DRAFT,
                                                and/or BPP_BP_QUALITY_HIGH */

    tBPP_ORIENTATION_B  orient_mask;             /* BPP_BP_PORTRAIT, BPP_BP_LANDSCAPE,
                                                BPP_BP_REVERSE_LANDSCAPE, and/or
                                                BPP_BP_REVERSE_PORTRAIT */

    BOOLEAN         color_supported;         /* Printer supports color? */
    tBPP_ATTR_STR_LIST  *p_doc_fmt_list;     /* List of document formats supported */
    tBPP_ATTR_STR_LIST  *p_media_sizes_list; /* List of media sizes supported. NULL if none */
    tBPP_ATTR_STR_LIST  *p_img_fmt_list;     /* List of image formats supported. NULL if none */
} tBPP_PR_ATR_RSP;

typedef struct
{
    tBPP_JOB_ID         jid;
    tBPP_JOB_STATE      job_state;
    char                j_name[BPP_MAX_NAME_LEN + 1];
    char                j_usr_name[BPP_MAX_NAME_LEN + 1];
    UINT32              sheet_cmpl;             /* JobMediaSheetsCompleted */
    UINT32              num_intv;               /* NumberOfIntervening Jobs */
    tBPP_OPER_STATUS    op_sts;

} tBPP_JOB_ATR_RSP;

/* CreateJobRsp event associated data */
typedef struct
{
    tBPP_JOB_ID         jid;
    tBPP_OPER_STATUS    op_sts;
} tBPP_CREATE_JOB_RSP;

typedef struct
{
    tBPP_JOB_ID         jid;
    tBPP_JOB_STATE      job_state;
    tBPP_PR_STATE       pr_state;
    UINT16              st_rsn;     /* Printer state reason */
    tBPP_OPER_STATUS    op_sts;

} tBPP_GET_EVT_RSP;

typedef struct
{
    char                margin[BPP_MAX_NAME_LEN + 1]; /* tBPP_MARGIN  margin; */
    tBPP_OPER_STATUS    op_sts;
} tBPP_GET_MARGIN_RSP;

typedef struct
{
    UINT32              read;
    UINT32              total;
} tBPP_PROGRESS;

typedef struct
{
    UINT8               *name; /* object name to get */
    UINT32              len;    /* object length requested */
    UINT32              offset; /* offset position requested */
} tBPP_GET_ROBJ_REQ;

typedef union
{
    tBPP_PR_ATR_RSP     get_pattr;   /* BPP_SND_GET_PR_ATTR_EVT */
    tBPP_JOB_ATR_RSP    get_jattr;   /* BPP_SND_GET_JOB_ATTR_EVT */
    tBPP_CREATE_JOB_RSP job_rsp;     /* BPP_SND_CREATE_JOB_EVT  & BPP_SND_CANCEL_JOB_EVT */
    tBPP_GET_EVT_RSP    get_evt;     /* BPP_SND_GET_EVENT_EVT */
    tBPP_GET_MARGIN_RSP get_marg;    /* BPP_SND_GET_MARGIN_EVT   */
    tBPP_PROGRESS       progress;    /* BPP_SND_PROGRESS_EVT */
    tBPP_GET_ROBJ_REQ   ref_obj_req;

    tOBX_RSP_CODE       rsp;         /* BPP_SND_GET_RSP_EVT */
    tBPP_SDP_DATA       sdp;         /* BPP_SND_SDP_EVT. sdp.scn=0 means SDP failed */
    BD_ADDR             con;         /* BPP_SND_CON_EVT : remote bd addr */
    tBPP_STATUS         status;      /* BPP_SND_API_ERR_EVT: error reason */
    /* BPP_SND_DOC_CMPL_EVT: status code */
    tBPP_AUTH_EVT       auth;        /* BPP_SND_AUTH_EVT */
    char                *p_name;     /* BPP_SND_ABORT_EVT */
} tBPP_SND;

/* Application implements this callback routine to receive events from the BPP printer job channel */
typedef void (tBPP_CBACK)(tBPP_HANDLE handle, tBPP_CHANNEL channel, UINT16 event, void *p_data);

/* Application implements this callback routine to receive events from the BPP printer job channel */
typedef void (tBPP_PR_CBACK)(tBPP_HANDLE handle, tBPP_CHANNEL channel, tBPP_PR_EVT event, tBPP_PR *p_data);

/* Application implements this callback routine to receive events from the BPP sender job channel */
typedef void (tBPP_SND_CBACK)(tBPP_HANDLE handle, tBPP_CHANNEL channel, tBPP_SND_EVT event, tBPP_SND *p_data);

/* file system interfaces */
typedef struct
{
    tGOEP_OPEN_CBACK     *p_open;
    tGOEP_CLOSE_CBACK    *p_close;
    tGOEP_WRITE_CBACK    *p_write;
    tGOEP_READ_CBACK     *p_read;
    tGOEP_SEEK_CBACK     *p_seek;
} tBPP_FS_CBACKS;

/* data type used by BPP_StartPrinter() API */
typedef struct
{
    tBPP_FS_CBACKS      fs_cbacks;
    tBPP_PR_CBACK       *p_cback;   /* Application function to call back */
    UINT8               scn_print;  /* Job channel SCN (Server Channel Number). */
    UINT8               scn_status; /* Status channel SCN. */
    UINT8               max_con;    /* Max connections allowed */
    BOOLEAN             use_auth;   /* TRUE to use OBEX authentication */
    UINT8               app_id;     /* for file system */
} tBPP_START_INFO;

/* data type used by BPP_StartSender() API */
typedef struct
{
    tBPP_SERVICE        service;    /* The BPP service(direct printing/reference prinitng) to connect to. */
    tBPP_FS_CBACKS      fs_cbacks;
    tBPP_SND_CBACK      *p_cback;   /* Application function to call back */
    BD_ADDR             bd_addr;    /* The Bluetooth address of the printer */
    UINT8               scn;        /* Job/Status/Referenced Object channel SCN (Server Channel Number). */
    UINT8               sec_level;
    UINT8               app_id;     /* for file system */
} tBPP_START_SND_INFO;


typedef struct
{
    UINT8               scn;            /* scn */
    UINT8               app_id;     /* for file system */
} tBPP_START_REF_INFO;

/* data type used by BPP_RegisterSDP() and BPP_UpdateSdpParams() */
typedef struct
{
    tBPP_STR    doc_format;     /* Document Format Supported. See page BPP spec. section 12.2.2. */
    tBPP_STR    image;          /* XHTML-Print formats suported. See BPP spec. section 12.2.4. */
    tBPP_STR    pr_1284_id;     /* ID string. See BPP spec. */
    tBPP_STR    service;        /* Service Name. */
    tBPP_STR    name;           /* Printer Name. */
    tBPP_STR    location;       /* Printer Location */
    tBPP_STR    media_types;    /* Media types supported. Se BPP spec. 12.2.6 */
    tBPP_UINT128 char_rep;      /* Character repertoires supported. See BPP spec. section 12.2.3 */
    UINT16      media_width;    /* Max media width in mm. See BPP spec. 12.2.7 */
    UINT16      media_length;   /* Max media height in mm. See BPP spec. 12.2.7 */
    BOOLEAN     color;          /* Color supported. */
    BOOLEAN     duplex;         /* Duplex supported */
} tBPP_SDP_PARAMS;


/*****************************************************************************
**  External Function Declarations
*****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************
**  Utility Functions
*****************************************************************************/

/*******************************************************************************
**
** Function         BPP_Init
**
** Description      This function is called at stack startup to initialize the
**                  control block and tracing level.
**
** Returns          void
**
*******************************************************************************/
BPP_API extern void BPP_Init(void);

/*****************************************************************************
**
**  Function        BPP_SetTraceLevel
**
**  Description     This function changes the trace debug level
**
**  Returns         void
**
*****************************************************************************/
BPP_API extern void BPP_SetTraceLevel(UINT8 level);


/*****************************************************************************
**
** Function         BPP_RegisterSDP
**
** Description      Create a SDP record for the printer
**                  The function can be called after BPP_StartPrinter() is
**                  called and BPP_PR_START_EVT reports num_handles > 0..
**                  The SDP handle can be freed with SDP_DeleteRecord().
**
** Returns          0, if failed.
**                  Otherwise, the created SDP handle
**
*****************************************************************************/
BPP_API extern UINT32 BPP_RegisterSDP(tBPP_SDP_PARAMS *p_sdp_params);

/*****************************************************************************
**
** Function         BPP_UpdateSdpParams
**
** Description      Updates the SDP parameters for the printer
**                  sdp_handle must be a valid SDP handle
**
** Returns          TRUE, if successfully processed
**
*****************************************************************************/
BPP_API extern BOOLEAN BPP_UpdateSdpParams(UINT32 sdp_handle, tBPP_SDP_PARAMS *p_sdp);

/*****************************************************************************
**
** Function         BPP_SetPrMode
**
** Description      Sets active mode for the printer.
**
**                  If BPP_PR_MODE_OP_CONNECT bit is set in op,
**                     the connectable mode is set according to printer_mode.
**                  If BPP_PR_MODE_OP_DISCOV_GEN or BPP_PR_MODE_OP_DISCOV_LMTD
**                     bit is set in op,
**                     the discoverable mode is set according to printer_mode.
**                  If both BPP_PR_MODE_OP_DISCOV_GEN and BPP_PR_MODE_OP_DISCOV_LMTD
**                     bits are set in op, BPP_PR_MODE_OP_DISCOV_GEN is used.
**                  If BPP_PR_MODE_OP_PAIRING bit is set in op, this function
**                     does the following:
**                     If printer_mode is BPP_BONDING, pairing is allowed
**                     If printer_mode is not BPP_BONDING, pairing is disallowed
**
** Returns          void
**
*****************************************************************************/
BPP_API extern void BPP_SetPrMode(tBPP_PR_MODE printer_mode, UINT8 op);

/*****************************************************************************
**  Printer Functions
*****************************************************************************/

/*****************************************************************************
**
** Function         BPP_StartPrinter
**
** Description      Registers callbacks for basic functionallity as a printer
**
** Returns          void
**
*****************************************************************************/
BPP_API extern void BPP_StartPrinter(tBPP_START_INFO   *p_msg);

/*****************************************************************************
**
** Function         BPP_StopPrinter
**
** Description      Deregisters the printer from the stack.
**
** Returns          void
**
*****************************************************************************/
BPP_API extern void BPP_StopPrinter(void);

/*****************************************************************************
**
** Function         BPP_PrClose
**
** Description      Close the connection to the sender on this handle.
**                  If the status or object channel of this handle is connected,
**                  it will also be disconnected.
**
** Returns          void
**
*****************************************************************************/
BPP_API extern void BPP_PrClose(tBPP_HANDLE handle);

/*******************************************************************************
**
** Function         BPP_Password
**
** Description      Verify the authentication response from the connected client.
**                  Called in response to a BPP_PR_PASSWORD_EVT event.
**                  Used when "enable_authen" is set to TRUE in BPP_StartPrinter().
**
**                  Note: If the "userid_required" is TRUE in the BPP_PR_PASSWORD_EVT
**                        event, then p_userid is required, otherwise it is optional.
**
**                  p_password  must be less than BPP_PR_MAX_AUTH_KEY_SIZE (16 bytes)
**                  p_userid    must be less than OBX_MAX_REALM_LEN (defined in target.h)
**
** Returns          void
**
*******************************************************************************/
BPP_API extern void BPP_Password(tBPP_HANDLE handle, char *p_password, char *p_userid);

/*******************************************************************************
**
** Function         BPP_NameRsp
**
** Description      Called in response to an BPP_PR_FILE_NAME_EVT or
**                  BPP_PR_SEND_DOC_EVT event.
**
**                  p_name must be fully qualified path/name to receive the
**                  object being pushed.
**
** Returns          void
**
*******************************************************************************/
BPP_API extern void BPP_NameRsp(tBPP_HANDLE    handle,
                                char           *p_name,
                                tOBX_RSP_CODE  obx_rsp);

/*****************************************************************************
**
** Function         BPP_Abort
**
** Description      Aborts a 'get' operation on BPP_OBJECT_CHANNEL
**
** Returns          void
**
*****************************************************************************/
BPP_API extern void BPP_Abort(tBPP_HANDLE       handle);


/*****************************************************************************
** Job Based Response Functions
*****************************************************************************/

/*****************************************************************************
**
** Function         BPP_GetPrAttrsRsp
**
** Description      Responds to a Get Printer Attributes request
**                  (BPP_PR_GET_PR_ATTRS_EVT).
**                  The tBPP_SOAP_RSP must include all of the attributes
**                  in the event data:
**
** Returns          void
**
*****************************************************************************/
BPP_API extern void BPP_GetPrAttrsRsp(tBPP_HANDLE      handle,
                                      tBPP_CHANNEL     channel,
                                      tBPP_SOAP_RSP    *p_attrs,
                                      tBPP_OPER_STATUS  op_stat);

/*****************************************************************************
**
** Function         BPP_CreateJobRsp
**
** Description      Responds to a Create Job request (BPP_PR_CREATE_JOB_EVT).
**
** Returns          void
**
*****************************************************************************/
BPP_API extern void BPP_CreateJobRsp(tBPP_HANDLE       handle,
                                     tBPP_JOB_ID       job_id,
                                     tBPP_OPER_STATUS  op_stat);

/*****************************************************************************
**
** Function         BPP_GetJobAttrsRsp
**
** Description      Responds to a Get Job Attributes request.
**                  (BPP_PR_GET_JOB_ATTRS_EVT).
**                  The tBPP_SOAP_RSP must include all of the attributes
**                  in the event data:
**
** Returns          void
**
*****************************************************************************/
BPP_API extern void BPP_GetJobAttrsRsp(tBPP_HANDLE         handle,
                                       tBPP_CHANNEL        channel,
                                       tBPP_JOB_ID         job_id,
                                       tBPP_SOAP_RSP       *p_attrs,
                                       tBPP_OPER_STATUS    op_stat);

/*****************************************************************************
**
** Function         BPP_CancelJobRsp
**
** Description      Responds to a Cancel Job request (BPP_PR_CANCEL_JOB_EVT).
**
** Returns          void
**
*****************************************************************************/
BPP_API extern void BPP_CancelJobRsp(tBPP_HANDLE       handle,
                                     tBPP_CHANNEL      channel,
                                     tBPP_JOB_ID       job_id,
                                     tBPP_OPER_STATUS  op_stat);

/*****************************************************************************
**
** Function         BPP_GetEventRsp
**
** Description      Responds to a Get Event request (BPP_PR_GET_EVENT_EVT).
**                  The tBPP_SOAP_RSP must include the following attributes:
**                  BPP_ATTR_JOB_ID,
**                  BPP_ATTR_JOB_STATE,
**                  BPP_ATTR_PR_STATE,
**                  BPP_ATTR_PR_STATE_REASONS
**
** Returns          void
**
*****************************************************************************/
BPP_API extern void BPP_GetEventRsp(tBPP_HANDLE        handle,
                                    tBPP_SOAP_RSP      *p_rsp,
                                    tBPP_OPER_STATUS   op_stat);

/*****************************************************************************
**
** Function         BPP_CreatePreciseJobRsp
**
** Description      Responds to a Create Precise Job request.
**                  (BPP_PR_CREATE_PREC_JOB_EVT)
**
** Returns          void
**
*****************************************************************************/
BPP_API extern void BPP_CreatePreciseJobRsp(tBPP_HANDLE        handle,
        tBPP_JOB_ID        job_id,
        tBPP_OPER_STATUS   op_stat);

/*****************************************************************************
**
** Function         BPP_GetMarginsRsp
**
** Description      Responds to a Get Margins request (BPP_PR_GET_MARGINS_EVT).
**
** Returns          void
**
*****************************************************************************/
BPP_API extern void BPP_GetMarginsRsp(tBPP_HANDLE      handle,
                                      const char      *p_margins,
                                      tBPP_OPER_STATUS op_stat);

/*****************************************************************************
** Referenced Object Functions
*****************************************************************************/

/*****************************************************************************
**
** Function         BPP_ConRefObjService
**
** Description      Connect to a ReferencedObject Service on the sender.
**
** Returns          void
**
*****************************************************************************/
BPP_API extern void BPP_ConRefObjService(tBPP_HANDLE      handle);

/*****************************************************************************
**
** Function         BPP_DisconRefObjService
**
** Description      Disconnect from a ReferencedObject Service on the sender.
**
** Returns          void
**
*****************************************************************************/
BPP_API extern void BPP_DisconRefObjService(tBPP_HANDLE      handle);

/*****************************************************************************
**
** Function         BPP_GetRefObj
**
** Description      Sends a Get Referenced Object request to the sender.
**
**                  p_name  must be fully qualified path/name to receive the
**                          object being pulled.
**                  p_obj_id (UTF8 encoding) is the object reference in the
**                          document received from the sender.
**                  offset  the byte offset into the image or print object (>=0)
**                  count   the number of bytes to send (>= -1).
**                          -1 indicates the remainder of the object.
**                  size    TRUE to request the file size from sender.
**
** Returns          void
**
*****************************************************************************/
BPP_API extern void BPP_GetRefObj(tBPP_HANDLE  handle,
                                  char         *p_name,
                                  char         *p_obj_id,
                                  UINT32       offset,
                                  INT32        count,
                                  BOOLEAN      size);

/*****************************************************************************
**
** Function         BPP_StartSender
**
** Description      Start BPP sender by creating an OBEX session with the
**                  printer.
**
** Returns
**
*****************************************************************************/
BPP_API extern void BPP_StartSender(tBPP_START_SND_INFO *p_msg);

/*****************************************************************************
**
** Function         BPP_StopSender
**
** Description      Close OBEX sessions, and deregisters the BPP sender from the stack.
**
** Returns          void
**
*****************************************************************************/
BPP_API extern void BPP_StopSender(void);

/*****************************************************************************
**
** Function         BPP_AuthRsp
**
** Description      Called by OBEX client to respond to the authentication challenge
**                  from the server.
**
** Returns          void
**
*****************************************************************************/
BPP_API extern tBPP_STATUS BPP_AuthRsp(tBPP_AUTH *p_auth_rsp);

/*****************************************************************************
**
** Function         BPP_SendDoc
**
** Description      Transfer the actual print content of a print job to printer.
**
** Returns          void
**
*****************************************************************************/
BPP_API extern tBPP_STATUS BPP_SendDoc(UINT32 job_id, UINT8 *p_file_name, UINT8 *p_mime_type, BOOLEAN req_ref_chnl);

/*****************************************************************************
**
** Function         BPP_AbortDoc
**
** Description      cancel job using job id
**
** Returns          void
**
*****************************************************************************/
BPP_API extern tBPP_STATUS BPP_AbortDoc(UINT32 job_id, tBPP_CHANNEL chnl);

/*****************************************************************************
**
** Function         BPP_GetPrAttrs
**
** Description      Send GetPrinterAttrbutes request to printer
**
** Returns          void
**
*****************************************************************************/
BPP_API extern tBPP_STATUS BPP_GetPrAttrs(tBPP_PR_ATTR_B  attr_mask,
        tBPP_CHANNEL channel);

/*****************************************************************************
**
** Function         BPP_FreePrAttrMem
**
** Description      Frees the memory returned with the printer attributes.
**                  Note: This memory will be released automatically the
**                  next time BPP_GetPrAttrs is called if not already freed.
**
** Returns          void
**
*****************************************************************************/
BPP_API extern tBPP_STATUS BPP_FreePrAttrMem(tBPP_PR_ATR_RSP  *p_attrmem, tBPP_CHANNEL channel);

/*****************************************************************************
**
** Function         BPP_CreateJob
**
** Description      Send a CreateJob request to printer.
**
** Returns          void
**
*****************************************************************************/
BPP_API extern tBPP_STATUS BPP_CreateJob(tBPP_CREATE_REQ  *p_req_msg, BOOLEAN is_precise);
/*****************************************************************************
**
** Function         BPP_ConnStatusService
**
** Description
**
** Returns          void
**
*****************************************************************************/
BPP_API extern void BPP_ConnStatusService(void);

/*****************************************************************************
**
** Function         BPP_DisconnStatusService
**
** Description
**
** Returns          void
**
*****************************************************************************/
BPP_API extern void BPP_DisconnStatusService(void);

/*****************************************************************************
**
** Function         BPP_GetJobAttrs
**
** Description
**
** Returns          tBPP_STATUS
**
*****************************************************************************/
BPP_API extern tBPP_STATUS BPP_GetJobAttrs(tBPP_JOB_ATTR_B  attr_mask, tBPP_JOB_ID job_id, tBPP_CHANNEL channel);

/*****************************************************************************
**
** Function         BPP_GetEvent
**
** Description
**
** Returns          tBPP_STATUS
**
*****************************************************************************/
BPP_API extern tBPP_STATUS BPP_GetEvent(tBPP_JOB_ID job_id);

/*****************************************************************************
**
** Function         BPP_GetMargin
**
** Description      Send BPP_GetMargin request to learn the exact dimension of
**                  the printable area for a particular printer on a particular
**                  medium size and type.
**
** Returns          void
**
*****************************************************************************/
BPP_API extern tBPP_STATUS BPP_GetMargin(UINT8 *p_media_size, tBPP_MEDIA_TYPE media_type);
/*****************************************************************************
**
** Function         BPP_GetRefObjRsp
**
** Description      Sends a Get Referenced Object response with fully quarlified object path.
**
**                  p_name  must be fully qualified path/name to receive the
**                          object being pulled.
**
** Returns          void
*****************************************************************************/
BPP_API extern void BPP_GetRefObjRsp(char *p_name);

#ifdef __cplusplus
}
#endif

#endif  /* BPP_API_H */

