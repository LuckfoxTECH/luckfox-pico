/*****************************************************************************
**
**  Name:       bip_api.h
**
**  File:       Common Interface to the Basic Imaging Profile
**
**  Copyright (c) 2000-2004, WIDCOMM Inc., All Rights Reserved.
**  WIDCOMM Bluetooth Core. Proprietary and confidential.
**
*****************************************************************************/
#ifndef BIP_API_H
#define BIP_API_H

#include "bt_target.h"
#include "xml_pars_api.h"
#include "obx_api.h"
#include "gap_api.h"
#include "goep_fs.h"



/*****************************************************************************
**  Constants and Type Definitions
*****************************************************************************/
#define BIP_VERSION         0x0100
#define BIP_MAX_TARGETS         6

/* !28-bit UUIDs used in SOAP messages */
#define BIP_UUID_LENGTH                 (16)
#define BIP_PUSH_UUID       "\xE3\x3D\x95\x45\x83\x74\x4A\xD7\x9E\xC5\xC1\x6B\xE3\x1E\xDE\x8E"
#define BIP_PULL_UUID       "\x8E\xE9\xB3\xD0\x46\x08\x11\xD5\x84\x1A\x00\x02\xA5\x32\x5B\x4E"
#define BIP_PRINT_UUID      "\x92\x35\x33\x50\x46\x08\x11\xD5\x84\x1A\x00\x02\xA5\x32\x5B\x4E"
#define BIP_ARCHIVE_UUID    "\x94\x01\x26\xC0\x46\x08\x11\xD5\x84\x1A\x00\x02\xA5\x32\x5B\x4E"
#define BIP_CAMERA_UUID     "\x94\x7E\x74\x20\x46\x08\x11\xD5\x84\x1A\x00\x02\xA5\x32\x5B\x4E"
#define BIP_DISPLAY_UUID    "\x94\xC7\xCD\x20\x46\x08\x11\xD5\x84\x1A\x00\x02\xA5\x32\x5B\x4E"

#define BIP_REF_OBJ_UUID    "\x8E\x61\xF9\x5D\x1A\x79\x11\xD4\x8E\xA4\x00\x80\x5F\x9B\x98\x34"
#define BIP_ARCH_OBJ_UUID   "\x8E\x61\xF9\x5E\x1A\x79\x11\xD4\x8E\xA4\x00\x80\x5F\x9B\x98\x34"

/* mandatory supported functions of the feature */
#define BIP_MIN_IR_PUSH_FUNCS       0x00000A
#define BIP_MIN_RR_PUSH_FUNCS       0x000003
#define BIP_MIN_IR_PULL_FUNCS       0x0000A0
#define BIP_AMN_IR_PULL_FUNCS       0x000120 /* alternative mandatory functions */
#define BIP_MIN_RR_PULL_FUNCS       0x0001E1
#define BIP_MIN_IR_PRINT_FUNCS      0x000800
#define BIP_MIN_RR_PRINT_FUNCS      0x010801
#define BIP_MIN_IR_REFOBJ_FUNCS     0x001000
#define BIP_MIN_RR_REFOBJ_FUNCS     0x001000
#define BIP_MIN_IR_ARCHIVE_FUNCS    0x000200
#define BIP_MIN_RR_ARCHIVE_FUNCS    0x012000
#define BIP_MIN_IR_ARCHOBJ_FUNCS    0x0001E1
#define BIP_MIN_RR_ARCHOBJ_FUNCS    0x000120
#define BIP_AMN_RR_ARCHOBJ_FUNCS    0x0000A0
#define BIP_MIN_IR_CAMERA_FUNCS     0x004000
#define BIP_MIN_RR_CAMERA_FUNCS     0x0041C0
#define BIP_MIN_IR_DISPLAY_FUNCS    0x00001A
#define BIP_MIN_RR_DISPLAY_FUNCS    0x000033

#define BIP_MAX_IR_PUSH_FUNCS       0x00000F
#define BIP_MAX_RR_PUSH_FUNCS       0x00000F
#define BIP_MAX_IR_PULL_FUNCS       0x0007E1
#define BIP_MAX_RR_PULL_FUNCS       0x0007E1
#define BIP_MAX_IR_PRINT_FUNCS      0x010801
#define BIP_MAX_RR_PRINT_FUNCS      0x010801
#define BIP_MAX_IR_REFOBJ_FUNCS     0x001000
#define BIP_MAX_RR_REFOBJ_FUNCS     0x001000
#define BIP_MAX_IR_ARCHIVE_FUNCS    0x012000
#define BIP_MAX_RR_ARCHIVE_FUNCS    0x012000
#define BIP_MAX_IR_ARCHOBJ_FUNCS    0x0007E1
#define BIP_MAX_RR_ARCHOBJ_FUNCS    0x0007E1
#define BIP_MAX_IR_CAMERA_FUNCS     0x0041C0
#define BIP_MAX_RR_CAMERA_FUNCS     0x0041C0
#define BIP_MAX_IR_DISPLAY_FUNCS    0x00003B
#define BIP_MAX_RR_DISPLAY_FUNCS    0x00003B

/* user defined header */
#define BIP_UDH_IMG_HDL     0x30    /* null terminated, UTF-16 encoded Unicode text length prefixed with a two-byte unsigned integer */
#define BIP_UDH_IMG_DESC    0x71    /* byte sequence, length prefixed with a two-byte unsigned integer */

/* Application Parameters Header
Tag IDs used in the Application Parameters header:
*/
/*  Tag ID                 Length  Possible Values */
#define BIP_APH_NUM_RET_HNDL    0x01    /* NbReturnedHandles       2 bytes 0x0000 to 0xFFFF */
#define BIP_APH_LIST_STOFF      0x02    /* ListStartOffset         2 bytes 0x0000 to 0xFFFF */
#define BIP_APH_LAST_CAPTURED   0x03    /* LatestCapturedImages    1 byte  0x00(False), 0x01(True) */
#define BIP_APH_PART_FILE_LEN   0x04    /* PartialFileLength       4 bytes 0x00000000 to 0xFFFFFFFF */
#define BIP_APH_PART_FILE_STOFF 0x05    /* PartialFileStartOffset  4 bytes 0x00000000 to 0xFFFFFFFF */
#define BIP_APH_TOTAL_FILE_SZ   0x06    /* TotalFileSize           4 bytes 0x00000000 to 0xFFFFFFFF */
#define BIP_APH_END_FLAG        0x07    /* EndFlag                 1 byte  0x00(False), 0x01(True) */
#define BIP_APH_REMOTE_DISP     0x08    /* RemoteDisplay           1 byte  tBIP_DSP_OP */
#define BIP_APH_SERVICE_ID      0x09    /* ServiceID               16 bytes UUID */
#define BIP_APH_STORE_FLAG      0x0A    /* StoreFlag               1 byte  0x00(False), 0x01(True) */
#define BIP_APH_MAX_TAG         BIP_APH_STORE_FLAG

/* Operation codes for remote display put operation */
enum
{
    BIP_DSP_NEXT_IMG    = 0x01, /* NextImage */
    BIP_DSP_PREV_IMG    = 0x02, /* PreviousImage */
    BIP_DSP_SELT_IMG    = 0x03, /* SelectImage */
    BIP_DSP_CURR_IMG    = 0x04  /* CurrentImage */
};
#define BIP_DSP_MIN_OP      BIP_DSP_NEXT_IMG
#define BIP_DSP_MAX_OP      BIP_DSP_CURR_IMG
typedef UINT8 tBIP_DSP_OP;

/* Initiator (primary channel) callback events. */
enum
{
    BIP_IP_START_EVT,       /* 0 Reports the result of BIP_StartInitiator() */
    BIP_IP_CON_EVT,         /* 1 Notifies of a CONNECT confirm */
    BIP_IP_AUTH_EVT,        /* 2 Notifies of authentication challenge from OBEX server */
    BIP_IP_API_ERR_EVT,     /* 3 Reports that BIP can not process the API function */
    BIP_IP_CLOSE_EVT,       /* 4 Notifies the RFCOMM port is closed */
    BIP_IP_TIMEOUT_EVT,     /* 5 Notifies timeout for OBEX session */
    BIP_IP_SDP_RESULT_EVT,  /* 6 Notifies the result for SDP search */
    BIP_IP_ABORT_RSP_EVT,   /* 7 Notifies of a ABORT response */
    BIP_IP_PUT_RSP_EVT,     /* 8 Notifies of a Put response for PutImage, PutLinkedThumbnail, PutLinkedAttachment,RemoteDisplay, DeleteImage, StartPrint, or StartArchive */
    BIP_IP_GET_FINAL_EVT,   /* 9 Notifies of the final packet of a GET request */
    BIP_IP_GET_CAP_EVT,     /* 10Notifies of a GetCapabilities response */
    BIP_IP_GET_LIST_EVT,    /* 11Notifies of a GetImagesList response */
    BIP_IP_GET_PROP_EVT,    /* 12Notifies of a GetImageProperties response */
    BIP_IP_GET_IMG_EVT,     /* 13Notifies of a GetImage response */
    BIP_IP_GET_THUMB_EVT,   /* 14Notifies of a GetLinkedThumbnail response */
    BIP_IP_GET_ATTACH_EVT,  /* 15Notifies of a GetLinkedAttachment response */
    BIP_IP_GET_MONITOR_EVT, /* 16Notifies of a GetMonitoringImage response */
    BIP_IP_GET_STATUS_EVT,  /* 17Notifies of a GetStatus response */
    BIP_IP_MAX_EVT          /* 18place holder */
};
typedef UINT8 tBIP_IP_EVENT;


/* Initiator (secondary channel) callback events. */
enum
{
    BIP_IS_START_EVT,       /* 0 Reports the result of BIP_StartRefService()  */
    BIP_IS_CON_EVT,         /* 1 Notifies of a CONNECT request */
    BIP_IS_PASSWORD_EVT,    /* 2 received the authentication response from the OBEX client. */
    BIP_IS_API_ERR_EVT,     /* 3 Reports that BIP can not process the API function */
    BIP_IS_CLOSE_EVT,       /* 4 Notifies the RFCOMM port is closed */
    BIP_IS_TIMEOUT_EVT,     /* 5 Notifies timeout for OBEX session */
    BIP_IS_ABORT_EVT,       /* 6 Notifies of a ABORT request */
    BIP_IS_GET_FINAL_EVT,   /* 7 Notifies of the final packet of a PUT/GET request */
    BIP_IS_GET_CAP_EVT,     /* 8 Notifies of a GetCapabilities request */
    BIP_IS_GET_LIST_EVT,    /* 9 Notifies of a GetImagesList request */
    BIP_IS_GET_PROP_EVT,    /* 10Notifies of a GetImageProperties request */
    BIP_IS_GET_IMG_EVT,     /* 11Notifies of a GetImage request */
    BIP_IS_GET_THUMB_EVT,   /* 12Notifies of a GetLinkedThumbnail request */
    BIP_IS_GET_ATTACH_EVT,  /* 13Notifies of a GetLinkedAttachment request */
    BIP_IS_GET_PARTIAL_EVT, /* 14Notifies of a GetPartialImage request */
    BIP_IS_DEL_IMG_EVT,     /* 15Notifies of a DeleteImage request */
    BIP_IS_MAX_EVT          /* place holder */
};
typedef UINT8 tBIP_IS_EVENT;

/* Responder (primary channel) callback events. */
enum
{
    BIP_RP_START_EVT        = BIP_IS_START_EVT,       /* 0 Reports the result of BIP_StartResponder() */
    BIP_RP_CON_EVT          = BIP_IS_CON_EVT,         /* 1 Notifies of a CONNECT request */
    BIP_RP_PASSWORD_EVT     = BIP_IS_PASSWORD_EVT,    /* 2 received the authentication response from the OBEX client. */
    BIP_RP_API_ERR_EVT      = BIP_IS_API_ERR_EVT,     /* 3 Reports that BIP can not process the API function */
    BIP_RP_CLOSE_EVT        = BIP_IS_CLOSE_EVT,       /* 4 Notifies the RFCOMM port is closed */
    BIP_RP_TIMEOUT_EVT      = BIP_IS_TIMEOUT_EVT,     /* 5 Notifies timeout for OBEX session */
    BIP_RP_ABORT_EVT        = BIP_IS_ABORT_EVT,       /* 6 Notifies of a ABORT request */
    BIP_RP_GET_FINAL_EVT    = BIP_IS_GET_FINAL_EVT,   /* 7 Notifies of the final packet of a GET request */
    BIP_RP_GET_CAP_EVT      = BIP_IS_GET_CAP_EVT,     /* 8 Notifies of a GetCapabilities request */
    BIP_RP_GET_LIST_EVT     = BIP_IS_GET_LIST_EVT,    /* 9 Notifies of a GetImagesList request */
    BIP_RP_GET_PROP_EVT     = BIP_IS_GET_PROP_EVT,    /* 10Notifies of a GetImageProperties request */
    BIP_RP_GET_IMG_EVT      = BIP_IS_GET_IMG_EVT,     /* 11Notifies of a GetImage request */
    BIP_RP_GET_THUMB_EVT    = BIP_IS_GET_THUMB_EVT,   /* 12Notifies of a GetLinkedThumbnail request */
    BIP_RP_GET_ATTACH_EVT   = BIP_IS_GET_ATTACH_EVT,  /* 13Notifies of a GetLinkedAttachment request */
    BIP_RP_PUT_FINAL_EVT,                             /* 14Notifies of the final packet of a PUT/GET request */
    BIP_RP_DEL_IMG_EVT      = BIP_IS_DEL_IMG_EVT,     /* 15Notifies of a DeleteImage request */
    BIP_RP_GET_MONITOR_EVT, /* 16Notifies of a GetMonitoringImage request */
    BIP_RP_GET_STATUS_EVT,  /* 17Notifies of a GetStatus request */
    BIP_RP_PUT_IMG_EVT,     /* 18Notifies of a PutImage request */
    BIP_RP_PUT_THUMB_EVT,   /* 19Notifies of a PutLinkedThumbnail request */
    BIP_RP_PUT_ATTACH_EVT,  /* 20Notifies of a PutLinkedAttachment request */
    BIP_RP_REMOTE_DISP_EVT, /* 21Notifies of a RemoteDisplay request */
    BIP_RP_PRINT_EVT,       /* 22Notifies of a StartPrint request */
    BIP_RP_ARCHIVE_EVT,     /* 23Notifies of a StartArchive request */
    BIP_RP_MAX_EVT          /* 24place holder */
};
typedef UINT8 tBIP_RP_EVENT;

/* Responder (secondary channel) callback events. */
enum
{
    BIP_RS_START_EVT        = BIP_IP_START_EVT,         /* 0 Reports the result of BIP_ConRefService() */
    BIP_RS_CON_EVT          = BIP_IP_CON_EVT,           /* 1 Notifies of a CONNECT confirm */
    BIP_RS_AUTH_EVT         = BIP_IP_AUTH_EVT,          /* 2 Notifies of authentication challenge from OBEX server */
    BIP_RS_API_ERR_EVT      = BIP_IP_API_ERR_EVT,       /* 3 Reports that BIP can not process the API function */
    BIP_RS_CLOSE_EVT        = BIP_IP_CLOSE_EVT,         /* 4 Notifies the RFCOMM port is closed */
    BIP_RS_TIMEOUT_EVT      = BIP_IP_TIMEOUT_EVT,       /* 5 Notifies timeout for OBEX session */
    BIP_RS_SDP_RESULT_EVT   = BIP_IP_SDP_RESULT_EVT,    /* 6 Notifies the result for SDP search */
    BIP_RS_ABORT_RSP_EVT    = BIP_IP_ABORT_RSP_EVT,     /* 7 Notifies of a ABORT response */
    BIP_RS_PUT_RSP_EVT      = BIP_IP_PUT_RSP_EVT,       /* 8 Notifies of a Put response for DeleteImage */
    BIP_RS_GET_FINAL_EVT    = BIP_IP_GET_FINAL_EVT,     /* 9 Notifies of the final packet of a PUT/GET request */
    BIP_RS_GET_CAP_EVT      = BIP_IP_GET_CAP_EVT,       /* 10Notifies of a GetCapabilities response */
    BIP_RS_GET_LIST_EVT     = BIP_IP_GET_LIST_EVT,      /* 11Notifies of a GetImagesList response */
    BIP_RS_GET_PROP_EVT     = BIP_IP_GET_PROP_EVT,      /* 12Notifies of a GetImageProperties response */
    BIP_RS_GET_IMG_EVT      = BIP_IP_GET_IMG_EVT,       /* 13Notifies of a GetImage response */
    BIP_RS_GET_THUMB_EVT    = BIP_IP_GET_THUMB_EVT,     /* 14Notifies of a GetLinkedThumbnail response */
    BIP_RS_GET_ATTACH_EVT   = BIP_IP_GET_ATTACH_EVT,    /*15 Notifies of a GetLinkedAttachment response */
    BIP_RS_GET_PARTIAL_EVT,     /* 16Notifies of a GetPartialImage response */
    BIP_RS_MAX_EVT              /* 17place holder */
};
typedef UINT8 tBIP_RS_EVENT;
#define BIP_INVALID_EVT     0xFF

/* These macros are used by BTA BI */
#define BIP_INT_IP_EVT          0x0000
#define BIP_INT_IS_EVT          0x0040
#define BIP_INT_RS_EVT          0x0080
#define BIP_INT_RP_EVT          0x00C0
#define BIP_INT_ROLE_MASK       0x00C0  /* IP, IS, RP or RS */
#define BIP_INT_EVT_2_ROLE_IND(int_evt)     (((int_evt)&BIP_INT_ROLE_MASK) >> 6)

/* Operation codes for BIP_GetRequest() and BIP_GetResponse */
enum
{
    BIP_OP_INVALID      = 0x0,  /* GetCapabilities request/response */
    BIP_OP_GET_CAP,             /* 0x1 GetCapabilities request/response */
    BIP_OP_GET_IMG_LIST,        /* 0x2 GetImagesList request/response */
    BIP_OP_GET_IMG_PROP,        /* 0x3 GetImageProperties request/response */
    BIP_OP_GET_IMG,             /* 0x4 GetImage request/response */
    BIP_OP_GET_THUMB,           /* 0x5 GetLinkedThumbnail request/response */
    BIP_OP_GET_ATTACH,          /* 0x6 GetLinkedAttachment request/response */
    BIP_OP_GET_PARTIAL,         /* 0x7 GetPartialImage request/response */
    BIP_OP_GET_MONITORING,      /* 0x8 GetMonitoringImage request/response */
    BIP_OP_GET_STATUS,          /* 0x9 GetStatus request/response */
    BIP_MAX_GET_OP              /* 0xA */
};
typedef UINT8  tBIP_GET_OP;
#define BIP_OP_FIRST_GET        BIP_OP_GET_CAP
#define BIP_OP_TYPE_BIT         0x80
#define BIP_OP_TYPE_PUT         0x80
#define BIP_OP_TYPE_GET         0x00

/* Operation codes for BIP_PutRequest() and BIP_PutResponse */
enum
{
    BIP_OP_PUT_IMAGE    = BIP_OP_TYPE_PUT,  /* PutImage request/response */
    BIP_OP_PUT_THUMB,           /* 0x81 PutLinkedThumbnail request/response */
    BIP_OP_PUT_ATTACH,          /* 0x82 PutLinkedAttachment request/response */
    BIP_OP_REMOTE_DISP,         /* 0x83 RemoteDisplay request/response */
    BIP_OP_DEL_IMAGE,           /* 0x84 DeleteImage request/response */
    BIP_OP_START_PRINT,         /* 0x85 StartPrint request/response */
    BIP_OP_START_ARCHIVE,       /* 0x86 StartArchive request/response */
    BIP_OP_CONTINUE,            /* 0x87 Continue response in respond to PUT_XXX_EVT */
    BIP_OP_MAX_PUT              /* 0x88 */
};
typedef UINT8  tBIP_PUT_OP;
#define BIP_OP_FIRST_PUT        BIP_OP_PUT_IMAGE
#define BIP_OP_MAX              BIP_OP_MAX_PUT

enum
{
    BIP_XML_ATTACH_DESC      = 0x0, /* attachemnt descriptor - tBIP_ATTACH_DESC */
    BIP_XML_IMG_CAPS         = 0x1, /* imaging-capabilities object - tBIP_IMAGING_CAPS(Body), tBIP_IMG_CAPS_RSP */
    BIP_XML_IMG_DESC         = 0x2, /* image descriptor - tBIP_IMAGE_DESC */
    BIP_XML_IMG_HDL_DESC     = 0x3, /* image handle descriptor - tBIP_IMAGE_HDL_DESC */
    BIP_XML_IMG_PROPS        = 0x4, /* image properties object - tBIP_IMAGE_PROPERTIES(B), tBIP_IMG_PROPS_RSP */
    BIP_XML_IMG_LIST         = 0x5, /* images-listing object - tBIP_IMAGE_LIST(B), tBIP_IMAGE_LIST_ELM */
    BIP_XML_MAX_OBJ          = 0x6
};
typedef UINT8  tBIP_XML_OBJ;

/* Service codes used in various APIs */
enum
{
    BIP_SERVICE_NONE,       /* 0 */
    BIP_SERVICE_PUSH,       /* 1 */
    BIP_SERVICE_PULL,       /* 2 */
    BIP_SERVICE_PRINT,      /* 3 */
    BIP_SERVICE_ARCHIVE,    /* 4 */
    BIP_SERVICE_CAMERA,     /* 5 */
    BIP_SERVICE_DISPLAY,    /* 6 */
    BIP_SERVICE_REF_OBJ,    /* 7 */
    BIP_SERVICE_ARCH_OBJ,   /* 8 */
    BIP_MAX_SERVICE         /* 9 */
};
typedef UINT8 tBIP_SERVICE;

/* Flags used to indicate features offerred/desired (16-bit mask) */
enum
{
    BIP_FT_IMG_PUSH         = 0x0001,
    BIP_FT_IMG_PUSH_STORE   = 0x0002,
    BIP_FT_IMG_PUSH_PRINT   = 0x0004,
    BIP_FT_IMG_PUSH_DISPLAY = 0x0008,
    BIP_FT_IMG_PULL         = 0x0010,
    BIP_FT_ADV_IMG_PRINT    = 0x0020,
    BIP_FT_AUTO_ARCHIVE     = 0x0040,
    BIP_FT_REMOTE_CAMERA    = 0x0080,
    BIP_FT_REMOTE_DISPLAY   = 0x0100
};
typedef UINT16 tBIP_FEATURE_FLAGS;

#define BIP_FT_IMG_PUSH_FLAGS   0x000F
#define BIP_NUM_IMG_PUSH_SET(xn,xf)   {(xn) = 0; if((xf)&1) (xn)++; if((xf)&2) (xn)++; if((xf)&4) (xn)++; if((xf)&8) (xn)++;}
#define BIP_FT_INVALID_MASK     0xFE00

/* Flags used to indicate capabilities offerred/desired (8-bit mask) */
enum
{
    BIP_CP_GEN_IMAGING      = 0x01,
    BIP_CP_CAPTURING        = 0x02,
    BIP_CP_PRINTING         = 0x04,
    BIP_CP_DISPLAY          = 0x08
};
typedef UINT8 tBIP_CAPABILITY_FLAGS;

/* Flags used to indicate functions offerred/desired (32-bit mask) */
enum
{
    BIP_FN_GET_CAPABILITIES = 0x00000001,
    BIP_FN_PUT_IMAGE        = 0x00000002,
    BIP_FN_PUT_ATTACHMENT   = 0x00000004,
    BIP_FN_PUT_THUMBNAIL    = 0x00000008,
    BIP_FN_REMOTE_DISPLAY   = 0x00000010,
    BIP_FN_GET_IMG_LIST     = 0x00000020,
    BIP_FN_GET_IMG_PROPTIES = 0x00000040,
    BIP_FN_GET_IMG          = 0x00000080,
    BIP_FN_GET_THUMBNAIL    = 0x00000100,
    BIP_FN_GET_ATTACHMENT   = 0x00000200,
    BIP_FN_DELETE_IMAGE     = 0x00000400,
    BIP_FN_START_PRINT      = 0x00000800,
    BIP_FN_GET_PARTIAL_IMG  = 0x00001000,
    BIP_FN_START_ARCHIVE    = 0x00002000,
    BIP_FN_GET_MONITOR_IMG  = 0x00004000,
    BIP_FN_GET_STATUS       = 0x00010000,
    BIP_FN_ALL              = 0x00017FFF
};
typedef UINT32 tBIP_FUNCTION_FLAGS;

/* Transformation options used in put/get image operations */
enum
{
    BIP_TRANS_NONE,
    BIP_TRANS_STRETCH,
    BIP_TRANS_FILL,
    BIP_TRANS_CROP
};
typedef UINT8 tBIP_OPT_TRANS;

enum
{
    BIP_PRES_DE_CAP_PREFER   = 0x01, /* prefer element is present in tBIP_IMAGING_CAPABILITIES */
    BIP_PRES_DE_FILTER_PARMS = 0x02, /* filtering-parameters element is present in tBIP_IMAGING_CAPABILITIES or image-handles-descriptor */
    BIP_PRES_DE_CAP_DPOF     = 0x04, /* DPOF-options element is present in tBIP_IMAGING_CAPABILITIES */
    BIP_PRES_DE_IMAGE        = 0x08, /* image element is present in image-descriptor */
    BIP_PRES_DE_ATTACHMENT   = 0x10  /* attachment element is present in attachment-descriptor */
};
typedef UINT8 tBIP_PRES_DATA_ELEM;

enum
{
    BIP_PRES_DM_HANDLE        = 0x01, /* required field. does not need this flag */
    BIP_PRES_DM_CREATED       = 0x02,
    BIP_PRES_DM_MODIFIED      = 0x04,
    BIP_PRES_DM_SIZE          = 0x08, /* size or maxsize */
    BIP_PRES_DM_ENCODING      = 0x10, /* encoding - for image */
    BIP_PRES_DM_CONT_TYPE     = 0x10, /* content-type for attachment */
    BIP_PRES_DM_PIXEL         = 0x20, /* pixel (range) - for image */
    BIP_PRES_DM_CHARSET       = 0x20, /* charset for attachment */
    BIP_PRES_DM_TRANSFORM     = 0x40, /* transformation - for image */
    BIP_PRES_DM_NAME          = 0x40  /* name - for attachment*/
};
typedef UINT8 tBIP_PRES_DATA_MEMBER;


/* API function return value result codes. */
enum
{
    BIP_SUCCESS,              /* success                          */
    BIP_ERROR,                /* general failure                  */
    BIP_ALREADY_STARTED,      /* server/service already started   */
    BIP_RESOURCES,            /* resources not available          */
    BIP_INVALID_PARAM,        /* bad parameter(s)                 */
    BIP_OBEX_FAILURE,         /* OBEX failure                     */
    BIP_BAD_REQUEST,          /* bad request                      */
    BIP_XML_ERROR
};
typedef UINT8 tBIP_STATUS;

#define BIP_IMG_HANDLE_SIZE         7  /* fixed size per BIP specification */
#define BIP_SERVICE_ID_LEN          16 /* The BIP specification OBEX session use this size */
#define BIP_TIME_STR_SIZE           16 /* for the time stamps in XML objects */

typedef UINT8 tBIP_IMG_HDL_STR[BIP_IMG_HANDLE_SIZE + 1]; /* image handle */
typedef UINT8 tBIP_TIME_STR[BIP_TIME_STR_SIZE + 1];     /* time stamp */
typedef UINT8 tBIP_IMG_ENC_STR[BIP_IMG_ENCODE_SIZE + 1]; /* image encoding */
typedef UINT8 tBIP_TYPE_STR[BIP_TYPE_SIZE + 1];         /* content type */
typedef UINT8 tBIP_CHARSET_STR[BIP_CHARSET_SIZE + 1];
typedef UINT8 tBIP_ATT_NAME_STR[BIP_ATT_NAME_SIZE + 1]; /* attachment name */
typedef UINT8 tBIP_FNAME_STR[BIP_FNAME_SIZE + 1];       /* friendly name */
typedef UINT8 tBIP_TEMP_NAME_STR[BIP_TEMP_NAME_SIZE + 1]; /* temporary storage file name */
typedef UINT8 tBIP_IMG_NAME_STR[BIP_IMG_NAME_SIZE + 1]; /* image file name (including path)*/
typedef UINT8 tBIP_OBJ_NAME_STR[BIP_OBJ_NAME_SIZE + 1];   /* object (image, attachment, thumbnail)
                                                           file name */
typedef UINT8 tBIP_SERVICE_ID[BIP_SERVICE_ID_LEN];

typedef UINT8 *tBIP_STR_PTR;
typedef UINT8  tBIP_HANDLE;     /* Handle used to reference current BIP instance */


typedef struct
{
    tBIP_IMG_HDL_STR handle; /* The image’s handle.                      */
    tBIP_TIME_STR    created;/* The image’s creation time. The format is
                              * YYYYMMDDTHHMMSS, where the capital letter ‘T’ is explicitly
                              * inserted between the day and hour fields. It is recommended that
                              * whenever possible UTC time be used; when UTC time is used,
                              * the letter ‘Z’ is appended to the end of the string.            17 */
    tBIP_TIME_STR    modified;/* The image’s modification time. It uses the same format as the
                              * created attribute.                                              17 */
} tBIP_IMAGE_LIST_ELM;

typedef struct
{
    /* (w*h - w2*h2) if range and not bound by aspect ratio
    or (w** - w2*h2) if range and bound by aspect ratio (h is 0)
    or (w*h), if not a range (w2 or h2 is 0) */
    UINT16  w;   /* width  */
    UINT16  h;   /* height - 0 if use range and ratio */
    UINT16  w2;  /* width  - 0 if not range */
    UINT16  h2;  /* height - 0 if not range */
} tBIP_PIXEL;


/* the image descriptor object
   - used in GetImage, PutImage request (x-bt/img-img) */
typedef struct
{
    UINT32              size;
    tBIP_PIXEL          pixel;    /* r */
    tBIP_IMG_ENC_STR    encoding; /* r */
    tBIP_OPT_TRANS      transform;
} tBIP_IMAGE_DESC;

typedef struct
{
    UINT32           size;
    tBIP_PIXEL       pixel;    /* r */
    tBIP_IMG_ENC_STR encoding; /* r */
} tBIP_IMAGE_FORMAT;

typedef struct
{
    tBIP_TYPE_STR    type; /* r  content type (MIME)*/
    tBIP_CHARSET_STR charset;
} tBIP_ATTACH_FORMAT;

typedef struct
{
    UINT32              size;      /* in bytes */
    tBIP_TYPE_STR       type;      /* r  content type (MIME)*/
    tBIP_CHARSET_STR    charset;
    tBIP_ATT_NAME_STR   name;      /* r */
    tBIP_TIME_STR       created;
    tBIP_TIME_STR       modified;
} tBIP_IMAGE_ATTACH;

typedef struct
{
    BOOLEAN     created; /* 1, if filter on time created. 0(default), otherwise. */
    BOOLEAN     modified;
    BOOLEAN     encoding;
    BOOLEAN     pixel;
} tBIP_IMG_FILTER_CAP;

typedef struct
{
    BOOLEAN std;    /* 1, if standard print is supported. 0, otherwise. */
    BOOLEAN idx;    /* 1, if index print is supported. 0, otherwise. */
    BOOLEAN mul;    /* 1, if multiple image print is supported. 0, otherwise. */
    BOOLEAN siz;    /* 1, if specific size print is supported. 0, otherwise. */
    UINT8   qty;    /* the number of sets supported. 0, otherwise. */
    BOOLEAN dsc;    /* 1, if character stamp is supported. 0, otherwise. */
    BOOLEAN trm;    /* 1, if trimming is supported. 0, otherwise. */
} tBIP_OPT_DPOF;

/* the attachment descriptor object
   - used in PutLinkedAttachment request (x-bt/img-attachment) */
typedef struct
{
    UINT32              size;      /* in bytes */
    UINT8               pres;      /* bitwise OR of tBIP_PRES_DATA_MEMBER */
    tBIP_TYPE_STR       type;      /* r content type (MIME) */
    tBIP_CHARSET_STR    charset;
    tBIP_ATT_NAME_STR   name;      /* r */
    tBIP_TIME_STR       created;
} tBIP_ATTACH_DESC;

/* the imaging capabilities object
   - response for GetCapabilities function (x-bt/img-capabilities) */
typedef struct
{
    tBIP_IMAGE_DESC         prefer; /* image format prefer to receive */
    tBIP_IMAGE_FORMAT       *fmts;  /* image format can be retrieved by other devices */
    tBIP_ATTACH_FORMAT      *attfmt;/* supported attachment formats */
    tBIP_IMG_FILTER_CAP     filter; /* the ability to filter iamges for image-list */
    UINT8       pres;               /* bitwise OR of tBIP_PRES_DATA_ELEM */
    UINT8       num_fmts;           /* the actual number of items in fmts */
    UINT8       num_attfmt;         /* the actual number of items in attfmt */
    tBIP_OPT_DPOF           dpof;   /* supported DPOF options */
} tBIP_IMAGING_CAPS;


/* used in the image handles descriptor object (filtering parameters for image list) */
typedef struct
{
    tBIP_PIXEL       pixel;
    UINT8            pres;          /* bitwise OR  of tBIP_PRES_DATA_MEMBER */
    tBIP_TIME_STR    created1;
    tBIP_TIME_STR    created2;
    tBIP_TIME_STR    modified1;
    tBIP_TIME_STR    modified2;
    tBIP_IMG_ENC_STR encoding;
} tBIP_IMAGE_HDL_DESC;

/* the image listing object
   - response for GetImagesList function (x-bt/img-listing) */
typedef struct
{
    UINT8               num_list;   /* max number allowed in list */
    tBIP_IMAGE_LIST_ELM *list;
} tBIP_IMAGE_LIST;

/* the image properties object
   - response for GetImageProperties function (x-bt/img-properties) */
typedef struct
{
    tBIP_IMAGE_FORMAT   native;     /* if native is not present (in other chunk of object): encoding is 0 str */
    tBIP_IMAGE_DESC     *variant;
    tBIP_IMAGE_ATTACH   *attach;
    tBIP_IMG_HDL_STR    handle;     /* the image handle */
    tBIP_FNAME_STR      f_name;     /* the friendly name */
    UINT8               num_variant;/* the actual number of items in variant */
    UINT8               num_attach; /* the actual number of items in attach */
} tBIP_IMAGE_PROPERTIES;

typedef struct
{
    tBIP_IMAGE_PROPERTIES   props;
    tOBX_RSP_CODE           rsp;        /* the OBEX response code */
} tBIP_IMG_PROPS_RSP;

/* Service Discovery Structure for referenced objects (used in BIP_StartObjService) */
typedef struct
{
    tBIP_FUNCTION_FLAGS functions;      /* supported functions */
    UINT8               scn;            /* scn */
    tBIP_SERVICE_ID     service_id;     /* service id */
    tBIP_FNAME_STR      service_name;   /* service name */
} tBIP_SDP_OBJ_PARAMS;

/* Service Discovery Structure  */
#define BIP_UINT64_SIZE     8
typedef UINT8 tBIP_UINT64[BIP_UINT64_SIZE];

typedef struct
{
    tBIP_FUNCTION_FLAGS     functions;      /* supported functions */
    UINT8                   scn;            /* scn */
    tBIP_CAPABILITY_FLAGS   capability;     /* supported capability */
    tBIP_FEATURE_FLAGS      features;       /* supported features */
    tBIP_UINT64             capacity;       /* total image capacity(in bytes) */
    UINT16                  version;        /* profile version */
    tBIP_FNAME_STR          service_name;   /* service name */
} tBIP_SDP_PARAMS;


#define BIP_NB_RET_HDLS_NO_LIMIT    65535
#define BIP_NB_RET_HDLS_NUM_ONLY    0
typedef struct
{
    tBIP_IMAGE_HDL_DESC hdl_desc;   /* the filtering mask to apply to the image-list */
    UINT16              nb_ret_hdl; /* the maximum number of image handles to be returned; 0 to get the number only. 65535 to set no limit */
    UINT16              start_off;  /* 0-based offset from the beginning of the list */
    BOOLEAN             last_cap;   /* 0, to get unrestricted, unordered images-list. 1 to get only locally captured images sorted by descending capture time */
} tBIP_IMG_LIST_REQ;

typedef struct
{
    UINT16              nb_ret_hdl; /* the maximum number of image handles to be returned; 0 to get the number only. 65535 to set no limit */
    UINT16              start_off;  /* 0-based offset from the beginning of the list */
    BOOLEAN             last_cap;   /* 0, to get unrestricted, unordered images-list. 1 to get only locally captured images sorted by descending capture time */
    tBIP_IMAGE_HDL_DESC hdl_desc;   /* the image handles descriptor XML object */
} tBIP_LIST_REQ_EVT;

typedef struct
{
    UINT16              nb_ret_hdl; /* The number of image handles returned in the list */
    tBIP_IMAGE_HDL_DESC hdl_desc;   /* info to compose image handles descriptor XML object. */
    tBIP_IMAGE_LIST_ELM *list;
} tBIP_IMG_LIST_RSP;

typedef struct
{
    UINT16              nb_ret_hdl; /* The number of image handles returned in the list */
    tBIP_IMAGE_HDL_DESC hdl_desc;   /* The temporary file name that holds that image handles descriptor XML object. */
    tBIP_IMAGE_LIST     list;       /* The temporary file name that holds that images list XML object. */
} tBIP_LIST_RSP_EVT;

typedef struct
{
    tBIP_IMAGE_DESC     *p_desc;/* if NULL, get the native version of the image */
    tBIP_IMG_HDL_STR    hdl;    /* the image handle */
    tBIP_TEMP_NAME_STR  temp;   /* The file name to store the received image */
} tBIP_GET_IMG_REQ;

typedef struct
{
    tBIP_IMG_HDL_STR    hdl;    /* the image handle */
    tBIP_IMAGE_DESC     *p_desc;/* if NULL, get the native version of the image */
} tBIP_GET_IMG_REQ_EVT;


typedef struct
{
    tBIP_OBJ_NAME_STR  obj;    /* The file name of the object (image, attachment, thumbnail) */
    tOBX_RSP_CODE      rsp;    /* the OBEX response code */
} tBIP_GET_OBJ_RSP;

typedef struct
{
    tBIP_IMG_HDL_STR    hdl;    /* the image handle */
    tBIP_TEMP_NAME_STR  temp;   /* The file name to store the received thumbnail image */
} tBIP_GET_THUMB_REQ;


typedef struct
{
    tBIP_IMG_HDL_STR    hdl;    /* the image handle */
    tBIP_ATT_NAME_STR   name;   /* The (file) name of the attachment (as in the image properties) */
    tBIP_TEMP_NAME_STR  temp;   /* The file name to store the received attachment */
} tBIP_GET_ATTACH_REQ;

typedef struct
{
    tBIP_ATT_NAME_STR   name;   /* The (file) name of the attachment (as in the image properties) */
    tBIP_IMG_HDL_STR    hdl;    /* the image handle */
} tBIP_GET_ATTACH_EVT;

typedef struct
{
    UINT32              len;    /* Partial file length */
    UINT32              offset; /* Partial file start offset */
    tBIP_IMG_NAME_STR   iname;  /* the image file name (as in DPOF file for name header) */
    tBIP_TEMP_NAME_STR  temp;   /* The file name to store the received partial image */
} tBIP_GET_PART_REQ;

typedef struct
{
    tBIP_IMG_NAME_STR   iname;  /* the image file name (as in DPOF file from name header) */
    UINT32              len;    /* Partial file length */
    UINT32              offset; /* Partial file start offset */
} tBIP_GET_PART_REQ_EVT;

typedef struct
{
    UINT32  len;    /* Partial file length */
    UINT32  total;  /* Total file length */
    BOOLEAN end;    /* TRUE, if the final part of the image. */
} tBIP_GET_PART_RSP_EVT;

typedef struct
{
    BOOLEAN             store;  /* TRUE, to store the full size image */
    tBIP_TEMP_NAME_STR  temp;   /* The file name to store the received monitoring image */
} tBIP_GET_MONITOR_REQ;

typedef struct
{
    tBIP_STR_PTR        p_img_hdl;
    tBIP_TEMP_NAME_STR  temp;   /* The name of the monitoring image */
} tBIP_GET_MONITOR_RSP;

typedef struct
{
    tBIP_IMAGE_DESC     desc;   /* the image descriptor */
    tBIP_IMG_NAME_STR   iname;  /* the image file name (for Name header) */
    tBIP_OBJ_NAME_STR   obj;   /* the image file name (for file system) */
} tBIP_PUT_IMAGE_REQ;

typedef struct
{
    tBIP_IMG_NAME_STR   img_name;   /* the image name (Name header)   */
    tBIP_IMAGE_DESC     img_desc;   /* The temporary file name that holds the image descriptor XML object. */
} tBIP_PUT_IMAGE_EVT;

typedef struct
{
    tBIP_IMG_HDL_STR    img_hdl;
    tOBX_RSP_CODE       rsp_code;
} tBIP_PUT_IMAGE_RSP;

typedef struct
{
    tBIP_STR_PTR        p_img_hdl;
    tOBX_RSP_CODE       rsp_code;
} tBIP_PUT_RSP_EVT;

typedef struct
{
    tBIP_OBJ_NAME_STR   obj;    /* the thumbnail file name (to access file system) */
    tBIP_IMG_HDL_STR    handle; /* the image handle */
} tBIP_PUT_THUMB_REQ;

typedef struct
{
    tBIP_OBJ_NAME_STR   obj;    /* the attachment file name (to access file system) */
    tBIP_ATTACH_DESC    desc;   /* the linked attachment descriptor */
    tBIP_IMG_HDL_STR    handle; /* the image handle */
} tBIP_PUT_ATTACH_REQ;

typedef struct
{
    tBIP_IMG_HDL_STR    handle; /* the image handle */
    tBIP_ATTACH_DESC    desc;   /* The linked attachment descriptor. */
} tBIP_PUT_ATTACH_EVT;

typedef struct
{
    BT_HDR                  hdr;
    UINT16                  sdp_uuid;   /* The UUID in the SDP search */
    union
    {
        tBIP_SDP_PARAMS     primary;    /* if uuid is UUID_SERVCLASS_IMAGING_RESPONDER  */
        tBIP_SDP_OBJ_PARAMS secondary;  /* if uuid is UUID_SERVCLASS_IMAGING_REF_OBJECTS or
                                         * UUID_SERVCLASS_IMAGING_AUTO_ARCHIVE*/
        UINT16              sdp_result; /* if uuid is 0, the SDP result code */
    } v;
} tBIP_SDP_RESULT_EVT;

typedef struct
{
    tBIP_SERVICE_ID     service_id; /* The service of the Referenced Object service record. */
    tBIP_OBJ_NAME_STR   dpof;       /* The file name that holds the DPOF object */
} tBIP_START_PR_REQ;

typedef struct
{
    tBIP_STR_PTR        p_handle;    /* the image handle. NULL, if empty. */
    tBIP_DSP_OP         param;       /* The remote display operation. See Table 5. */
} tBIP_RMT_DISP_REQ;


/*****************************************************************************
**  Callback Function Definitions
**          See BIP API documentation for specification of p
*****************************************************************************/

typedef void (tBIP_APP_CBACK)(UINT8        evt,
                              void         *p,          /* in/out */
                              tBIP_STATUS  status);     /* out */
/* Application implements this callback routine to receive events from the primary channel of BIP initiator. */
typedef void (tBIP_IP_CBACK)(tBIP_IP_EVENT evt,
                             void         *p,          /* in/out */
                             tBIP_STATUS  status);     /* out */
/* Application implements this callback routine to receive events from the secondary channel of BIP initiator. */
typedef void (tBIP_IS_CBACK)(tBIP_IS_EVENT evt,
                             void         *p,          /* in/out */
                             tBIP_STATUS  status);     /* out */
/* Application implements this callback routine to receive events from the primary channel of BIP responder. */
typedef void (tBIP_RP_CBACK)(tBIP_RP_EVENT evt,
                             void         *p,          /* in/out */
                             tBIP_STATUS  status);     /* out */
/* Application implements this callback routine to receive events from the secondary channel of BIP responder. */
typedef void (tBIP_RS_CBACK)(tBIP_RS_EVENT evt,
                             void         *p,          /* in/out */
                             tBIP_STATUS  status);     /* out */

typedef struct
{
    UINT8   *p_realm;    /* The realm that represent the server */
    UINT8   realm_len;   /* The length of p_realm. */
    tOBX_CHARSET    charset;    /* the charset for p_realm */
    BOOLEAN         userid_required; /* TRUE, if the peer requests for user ID */
} tBIP_AUTH_EVT;

typedef struct
{
    UINT8   *p_pswd;    /* The password. */
    UINT8   *p_userid;  /* The user id. */
    UINT8   pswd_len;   /* The length of p_password. */
    UINT8   userid_len; /* The length of p_userid. */
} tBIP_AUTH;

typedef struct
{
    tBIP_SERVICE    service;    /* The BIP feature to connect to. */
    tBIP_IP_CBACK   *p_cback;   /* The callback routine for the primary channel */
    BD_ADDR         bd_addr;    /* The Bluetooth address of the responder */
    UINT8           sec_level;  /* The security level to set on the primary channel */
    UINT8           scn;        /* the SCN for this channel */
    UINT8           app_id;         /* application ID for file system */
} tBIP_START_IR_MSG;

enum
{
    BIP_SET_COD_IGNORE          = 0,                          /* 0x00 - do not set */
    BIP_SET_COD_MAJOR_MINOR     = GAP_SET_COD_MAJOR_MINOR,    /* 0x01 */
    BIP_SET_COD_SERVICE_CLASS   = GAP_SET_COD_SERVICE_CLASS,  /* 0x02 only set the bits in the input */
    BIP_SET_COD_ALL             = GAP_SET_COD_ALL             /* 0x08 take service class as the input (may clear some set bits!!) */
};
typedef UINT8   tBIP_SET_COD;

/* parameters passed to BIP_StartResponder */
typedef struct
{
    tBIP_RP_CBACK   *p_cback;   /* The callback routine for the primary channel */
    tGAP_COD        cod;        /* The class of device to set */
    tBIP_SET_COD    set_cod;    /* See the following table for the options to set CoD. */
    tBIP_SDP_PARAMS sdp;        /* The information to register the BIP responder service to SDP. */
    UINT8           sec_level;  /* The security level to set on the primary channel */
    BOOLEAN         use_auth;   /* Use OBEX authentication for incoming OBEX connections */
    UINT8           app_id;         /* application ID for file system */
} tBIP_START_RP_MSG;

/* parameters passed to BIP_StartRefService */
typedef struct
{
    tBIP_SDP_OBJ_PARAMS sdp_params; /* The SDP parameters for referenced object service SDP record */
    tBIP_IS_CBACK       *p_cback;   /* The callback routine for the application to receive events from initiator. */
    tBIP_SERVICE        service;    /* The referenced object service. */
    UINT8               sec_level;  /* The security level to set on the secondary channel */
    UINT8           app_id;         /* application ID for file system */
} tBIP_START_REF_MSG;

/* parameters passed to BIP_ConRefService */
typedef struct
{
    tBIP_RS_CBACK   *p_cback;       /* The callback routine for the application to receive events from responder. */
    tBIP_SERVICE    service;        /* The referenced object service. */
    tBIP_SERVICE_ID service_id;     /* The service ID of the reference object SDP record. */
    UINT8           sec_level;      /* The security level to set on the secondary channel */
    UINT8           app_id;         /* application ID for file system */
} tBIP_CON_REF_MSG;


/*****************************************************************************
**  External Function Declarations
*****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/* Common functions (as initiator or responder) */
/*****************************************************************************
**
** Function         BIP_Close
**
** Description      Called by an application to close the OBEX session.
**
** Returns          BIP_SUCCESS if success
**                  BIP_INVALID_PARAM if bad handle
**                  BIP_RESOURCES if no GKI buffer to send the request
**
*****************************************************************************/
BIP_API extern tBIP_STATUS BIP_Close(tBIP_HANDLE handle);

/*****************************************************************************
**
** Function         BIP_GetRequest
**
** Description      Send a GET Request to the connected OBEX server
**
** Returns          BIP_SUCCESS if success
**                  BIP_INVALID_PARAM if any of the parameters is bad
**                  BIP_RESOURCES if no GKI buffer to send the request
**
*****************************************************************************/
BIP_API extern tBIP_STATUS BIP_GetRequest(tBIP_HANDLE handle, tBIP_GET_OP op,
        void *p_data);


/*****************************************************************************
**
** Function         BIP_GetResponse
**
** Description      Send a GET response to the connected OBEX client
**
** Returns          BIP_SUCCESS if success
**                  BIP_INVALID_PARAM if any of the parameters is bad
**                  BIP_RESOURCES if no GKI buffer to send the request
**
*****************************************************************************/
BIP_API extern tBIP_STATUS BIP_GetResponse(tBIP_HANDLE handle, tBIP_GET_OP op,
        void *p_data);


/*****************************************************************************
**
** Function         BIP_PutRequest
**
** Description      Send a PUT request to the connected OBEX server
**
** Returns          BIP_SUCCESS if success
**                  BIP_INVALID_PARAM if any of the parameters is bad
**                  BIP_RESOURCES if no GKI buffer to send the request
**
*****************************************************************************/
BIP_API extern tBIP_STATUS BIP_PutRequest(tBIP_HANDLE handle, tBIP_PUT_OP op,
        void *p_data);


/*****************************************************************************
**
** Function         BIP_PutResponse
**
** Description      Send a PUT response to the connected OBEX client
**
** Returns          BIP_SUCCESS if success
**                  BIP_INVALID_PARAM if any of the parameters is bad
**                  BIP_RESOURCES if no GKI buffer to send the request
**
*****************************************************************************/
BIP_API extern tBIP_STATUS BIP_PutResponse(tBIP_HANDLE handle, tBIP_PUT_OP op,
        void *p_data);


/*****************************************************************************
**
** Function         BIP_AbortRequest
**
** Description      Send ABORT request to the connected OBEX server
**
** Returns          BIP_SUCCESS if success
**                  BIP_BAD_REQUEST if the given BIP handle can not issue this request
**                  BIP_RESOURCES if no GKI buffer to send the request
**
*****************************************************************************/
BIP_API extern tBIP_STATUS BIP_AbortRequest(tBIP_HANDLE handle);


/*****************************************************************************
**
** Function         BIP_Auth
**
** Description      Called by OBEX server to verify the authentication response
**                  from the client
**                  Or Called by OBEX client to respond to the authentication
**                  challenge from the server
**
** Returns          BIP_SUCCESS if success, otherwise error
**                  BIP_RESOURCES if no GKI buffer to send the request
**
*****************************************************************************/
BIP_API extern tBIP_STATUS BIP_Auth(tBIP_HANDLE handle, tBIP_AUTH *p_auth);



/*****************************************************************************
**
** Function         BIP_CheckService
**
** Description      This call returns the associated BIP feature for the
**                  specified handle.
**
** Returns          BIP_SERV_NONE if the handle is not valid or not found.
**                  Otherwise, the associated BIP feature is returned (tBIP_SERVICE)
**
*****************************************************************************/
BIP_API extern tBIP_SERVICE BIP_CheckService(tBIP_HANDLE handle);


/*****************************************************************************
**
**  Function        BIP_SetTraceLevel
**
**  Description     This function changes the trace level (debugging levels)
**
**  Returns         void
**
*****************************************************************************/
BIP_API extern void BIP_SetTraceLevel(UINT8 level);


/*******************************************************************************
**
** Function         BIP_Init
**
** Description      This function is called once at stack startup to allocate
**                  (if using dynamic memory) and initialize the
**                  control block, and initializes the
**                  debug tracing level.
**
** Returns          void
**
*******************************************************************************/
BIP_API extern void BIP_Init(void);


/*******************************************************************************
**
** Function         BIP_SetFsCbacks
**
** Description      This function is called once before any BIP API function is
**                  used to set the file system access function in BIP
**                  control block.
**
** Returns          void
**
*******************************************************************************/
BIP_API extern void BIP_SetFsCbacks(tGOEP_OPEN_CBACK *p_open, tGOEP_CLOSE_CBACK *p_close,
                                    tGOEP_READ_CBACK *p_read, tGOEP_WRITE_CBACK *p_write,
                                    tGOEP_SEEK_CBACK *p_seek);

/* Initiator functions (BIP_INITR_INCLUDED == TRUE) */


/*****************************************************************************
**
** Function         BIP_StartInitiator
**
** Description      Start BIP initiator by creating an OBEX session with the
**                  responder.
**
** Returns          BIP_SUCCESS if success
**                  BIP_RESOURCES if no GKI buffer to send the request
**
*****************************************************************************/
BIP_API extern tBIP_STATUS BIP_StartInitiator(tBIP_START_IR_MSG *p_msg);


/*****************************************************************************
**
** Function         BIP_StopInitiator
**
** Description      Called by an application to close the OBEX session with
**                  the responder.
**
** Returns          BIP_SUCCESS if success
**                  BIP_RESOURCES if no GKI buffer to send the request
**
*****************************************************************************/
BIP_API extern tBIP_STATUS BIP_StopInitiator(void);


/*****************************************************************************
**
** Function         BIP_StartRefService
**
** Description      Called by an application to start the secondary service for
**                  initiator. Available when BIP_ARCHIVE_INCLUDED == TRUE or
**                  BIP_PRINTING_INCLUDED == TRUE.
**
** Returns          BIP_SUCCESS if success
**                  BIP_RESOURCES if no GKI buffer to send the request
**
*****************************************************************************/
BIP_API extern tBIP_STATUS BIP_StartRefService(tBIP_START_REF_MSG *p_msg);


/*****************************************************************************
**
** Function         BIP_StopRefService
**
** Description      Called by an application to Stop the secondary channel service
**                  of the initiator. Available when BIP_ARCHIVE_INCLUDED == TRUE or
**                  BIP_PRINTING_INCLUDED == TRUE.
**
** Returns          BIP_SUCCESS if success
**                  BIP_RESOURCES if no GKI buffer to send the request
**
*****************************************************************************/
BIP_API extern tBIP_STATUS BIP_StopRefService(void);



/* End of Initiator functions (BIP_INITR_INCLUDED == TRUE) */

/* Responder functions (BIP_RSPDR_INCLUDED == TRUE) */

/*****************************************************************************
**
** Function         BIP_StartResponder
**
** Description      Start BIP responder by initializing an OBEX session and
**                  then listens for connections with initiator applications.
**
** Returns          BIP_SUCCESS if success
**                  BIP_RESOURCES if no GKI buffer to send the request
**
*****************************************************************************/
BIP_API extern tBIP_STATUS BIP_StartResponder(tBIP_START_RP_MSG *p_msg);


/*****************************************************************************
**
** Function         BIP_StopResponder
**
** Description      Called by an application to take the responder out of
**                  listen mode and close the OBEX session.
**
** Returns          BIP_SUCCESS if success
**                  BIP_RESOURCES if no GKI buffer to send the request
**
*****************************************************************************/
BIP_API extern tBIP_STATUS BIP_StopResponder(void);



/*****************************************************************************
**
** Function         BIP_ConRefService
**
** Description      Called by an application to connect to the requested
**                  secondary channel. Available when BIP_ARCHIVE_INCLUDED == TRUE
**                  or BIP_PRINTING_INCLUDED == TRUE.
**
** Returns          BIP_SUCCESS if success
**                  BIP_RESOURCES if no GKI buffer to send the request
**                                or initiator is not connected
**
*****************************************************************************/
BIP_API extern tBIP_STATUS BIP_ConRefService(tBIP_CON_REF_MSG *p_msg);


/*****************************************************************************
**
** Function         BIP_DisconRefService
**
** Description      Called by an application to disconnect the secondary channel
**                  from the initiator. Available when BIP_ARCHIVE_INCLUDED == TRUE
**                  or BIP_PRINTING_INCLUDED == TRUE.
**
** Returns          BIP_SUCCESS if success
**                  BIP_RESOURCES if no GKI buffer to send the request
**
*****************************************************************************/
BIP_API extern tBIP_STATUS BIP_DisconRefService(void);


/* End of Responder functions (BIP_RSPDR_INCLUDED == TRUE) */

#ifdef __cplusplus
}
#endif

#endif /* BIP_API_H */
