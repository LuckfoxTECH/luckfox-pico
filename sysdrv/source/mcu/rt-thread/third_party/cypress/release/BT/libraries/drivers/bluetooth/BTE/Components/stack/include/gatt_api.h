/****************************************************************************
**
**  Name:       gatt_api.h
**  Function    this file contains the definitions for the GATT API
**
**
**  Copyright (c) 1999-2014, Broadcom Corp., All Rights Reserved.
**  Broadcom Bluetooth Core. Proprietary and confidential.
**
******************************************************************************/
#ifndef GATT_API_H
#define GATT_API_H

#include "bt_target.h"
#include "gattdefs.h"
#include "sdpdefs.h"
#include "btm_api.h"

/*****************************************************************************
**  Constants
*****************************************************************************/
/* Success code and error codes */
#define  GATT_SUCCESS                        0x00
#define  GATT_INVALID_HANDLE                 0x01
#define  GATT_READ_NOT_PERMIT                0x02
#define  GATT_WRITE_NOT_PERMIT               0x03
#define  GATT_INVALID_PDU                    0x04
#define  GATT_INSUF_AUTHENTICATION           0x05
#define  GATT_REQ_NOT_SUPPORTED              0x06
#define  GATT_INVALID_OFFSET                 0x07
#define  GATT_INSUF_AUTHORIZATION            0x08
#define  GATT_PREPARE_Q_FULL                 0x09
#define  GATT_NOT_FOUND                      0x0a
#define  GATT_NOT_LONG                       0x0b
#define  GATT_INSUF_KEY_SIZE                 0x0c
#define  GATT_INVALID_ATTR_LEN               0x0d
#define  GATT_ERR_UNLIKELY                   0x0e
#define  GATT_INSUF_ENCRYPTION               0x0f
#define  GATT_UNSUPPORT_GRP_TYPE             0x10
#define  GATT_INSUF_RESOURCE                 0x11


#define  GATT_NO_RESOURCES                   0x80
#define  GATT_INTERNAL_ERROR                 0x81
#define  GATT_WRONG_STATE                    0x82
#define  GATT_DB_FULL                        0x83
#define  GATT_BUSY                           0x84
#define  GATT_ERROR                          0x85
#define  GATT_CMD_STARTED                    0x86
#define  GATT_ILLEGAL_PARAMETER              0x87
#define  GATT_PENDING                        0x88
#define  GATT_AUTH_FAIL                      0x89
#define  GATT_MORE                           0x8a
#define  GATT_INVALID_CFG                    0x8b
#define  GATT_SERVICE_STARTED                0x8c
#define  GATT_ENCRYPED_MITM                  GATT_SUCCESS
#define  GATT_ENCRYPED_NO_MITM               0x8d
#define  GATT_NOT_ENCRYPTED                  0x8e
#define  GATT_CONGESTED                      0x8f

/* 0xE0 ~ 0xFB reserved for future use */
#define  GATT_WRITE_REQ_REJECTED             0xFC /* Write operation rejected */
#define  GATT_CCC_CFG_ERR                    0xFD /* Client Characteristic Configuration
                                                     Descriptor Improperly Configured */
#define  GATT_PRC_IN_PROGRESS                0xFE /* Procedure Already in progress */
#define  GATT_OUT_OF_RANGE                   0xFF /* Attribute value out of range */
typedef UINT8 tGATT_STATUS;


#define  GATT_RSP_ERROR                      0x01
#define  GATT_REQ_MTU                        0x02
#define  GATT_RSP_MTU                        0x03
#define  GATT_REQ_FIND_INFO                  0x04
#define  GATT_RSP_FIND_INFO                  0x05
#define  GATT_REQ_FIND_TYPE_VALUE            0x06
#define  GATT_RSP_FIND_TYPE_VALUE            0x07
#define  GATT_REQ_READ_BY_TYPE               0x08
#define  GATT_RSP_READ_BY_TYPE               0x09
#define  GATT_REQ_READ                       0x0A
#define  GATT_RSP_READ                       0x0B
#define  GATT_REQ_READ_BLOB                  0x0C
#define  GATT_RSP_READ_BLOB                  0x0D
#define  GATT_REQ_READ_MULTI                 0x0E
#define  GATT_RSP_READ_MULTI                 0x0F
#define  GATT_REQ_READ_BY_GRP_TYPE           0x10
#define  GATT_RSP_READ_BY_GRP_TYPE           0x11
#define  GATT_REQ_WRITE                      0x12 /*                 0001-0010 (write)*/
#define  GATT_RSP_WRITE                      0x13
#define  GATT_CMD_WRITE                      0x52 /* changed in V4.0 01001-0010(write cmd)*/
#define  GATT_REQ_PREPARE_WRITE              0x16
#define  GATT_RSP_PREPARE_WRITE              0x17
#define  GATT_REQ_EXEC_WRITE                 0x18
#define  GATT_RSP_EXEC_WRITE                 0x19
#define  GATT_HANDLE_VALUE_NOTIF             0x1B
#define  GATT_HANDLE_VALUE_IND               0x1D
#define  GATT_HANDLE_VALUE_CONF              0x1E
#define  GATT_OP_CODE_MAX                    (GATT_HANDLE_VALUE_CONF + 1) /* 0x1F = GATT_HANDLE_VALUE_CONF + 1 = 31*/

#define  GATT_SIGN_CMD_WRITE                 0xD2 /* changed in V4.0 1101-0010 (signed write)  see write cmd above*/

#define GATT_OPCODE_COMMAND_FLAG        0x40

#define  GATT_HANDLE_IS_VALID(x) ((x) != 0)

#define GATT_CONN_UNKNOWN                   0
#define GATT_CONN_L2C_FAILURE               1                               /* general L2cap failure  */
#define GATT_CONN_TIMEOUT                   HCI_ERR_CONNECTION_TOUT         /* 0x08 connection timeout  */
#define GATT_CONN_TERMINATE_PEER_USER       HCI_ERR_PEER_USER               /* 0x13 connection terminate by peer user  */
#define GATT_CONN_TERMINATE_LOCAL_HOST      HCI_ERR_CONN_CAUSE_LOCAL_HOST   /* 0x16 connectionterminated by local host  */
#define GATT_CONN_FAIL_ESTABLISH            HCI_ERR_CONN_FAILED_ESTABLISHMENT/* 0x03E connection fail to establish  */
#define GATT_CONN_LMP_TIMEOUT               HCI_ERR_LMP_RESPONSE_TIMEOUT     /* 0x22 connection fail for LMP response tout */
#define GATT_CONN_CANCEL                    L2CAP_CONN_CANCEL                /* 0x0100 L2CAP connection cancelled  */
typedef UINT16 tGATT_DISCONN_REASON;

/* MAX GATT MTU size
*/
#ifndef GATT_MAX_MTU_SIZE
#define GATT_MAX_MTU_SIZE     23
#endif

/* max legth of an attribute value
*/
#ifndef GATT_MAX_ATTR_LEN
#define GATT_MAX_ATTR_LEN     600
#endif

/* default GATT MTU size over LE link
*/
#define GATT_DEF_BLE_MTU_SIZE       23

/* invalid connection ID
*/
#define GATT_INVALID_CONN_ID        0xFFFF

#ifndef GATT_CL_MAX_LCB
#define GATT_CL_MAX_LCB     22
#endif

#ifndef GATT_MAX_SCCB
#define GATT_MAX_SCCB       10
#endif

#ifndef GATTP_TRANSPORT_SUPPORTED
#define GATTP_TRANSPORT_SUPPORTED        GATT_TRANSPORT_LE_BR_EDR
#endif


/* GATT notification caching timer, default to be three seconds
*/
#ifndef GATTC_NOTIF_TIMEOUT
#define GATTC_NOTIF_TIMEOUT   3
#endif

/*****************************************************************************
** GATT Structure Definition
*****************************************************************************/

/* Attribute permissions
*/
#define GATT_PERM_READ              (1 << 0) /* bit 0 */
#define GATT_PERM_READ_ENCRYPTED    (1 << 1) /* bit 1 */
#define GATT_PERM_READ_ENC_MITM     (1 << 2) /* bit 2 */
#define GATT_PERM_WRITE             (1 << 4) /* bit 4 */
#define GATT_PERM_WRITE_ENCRYPTED   (1 << 5) /* bit 5 */
#define GATT_PERM_WRITE_ENC_MITM    (1 << 6) /* bit 6 */
#define GATT_PERM_WRITE_SIGNED      (1 << 7) /* bit 7 */
#define GATT_PERM_WRITE_SIGNED_MITM (1 << 8) /* bit 8 */
typedef UINT16 tGATT_PERM;

/* Characteristic properties
*/
#define GATT_CHAR_PROP_BIT_BROADCAST    (1 << 0)
#define GATT_CHAR_PROP_BIT_READ         (1 << 1)
#define GATT_CHAR_PROP_BIT_WRITE_NR     (1 << 2)
#define GATT_CHAR_PROP_BIT_WRITE        (1 << 3)
#define GATT_CHAR_PROP_BIT_NOTIFY       (1 << 4)
#define GATT_CHAR_PROP_BIT_INDICATE     (1 << 5)
#define GATT_CHAR_PROP_BIT_AUTH         (1 << 6)
#define GATT_CHAR_PROP_BIT_EXT_PROP     (1 << 7)
typedef UINT8 tGATT_CHAR_PROP;


/* Format of the value of a characteristic. enumeration type
*/
enum
{
    GATT_FORMAT_RES,            /* rfu */
    GATT_FORMAT_BOOL,           /* 0x01 boolean */
    GATT_FORMAT_2BITS,           /* 0x02 2 bit */
    GATT_FORMAT_NIBBLE,         /* 0x03 nibble */
    GATT_FORMAT_UINT8,          /* 0x04 uint8 */
    GATT_FORMAT_UINT12,         /* 0x05 uint12 */
    GATT_FORMAT_UINT16,         /* 0x06 uint16 */
    GATT_FORMAT_UINT24,         /* 0x07 uint24 */
    GATT_FORMAT_UINT32,         /* 0x08 uint32 */
    GATT_FORMAT_UINT48,         /* 0x09 uint48 */
    GATT_FORMAT_UINT64,         /* 0x0a uint64 */
    GATT_FORMAT_UINT128,        /* 0x0B uint128 */
    GATT_FORMAT_SINT8,          /* 0x0C signed 8 bit integer */
    GATT_FORMAT_SINT12,         /* 0x0D signed 12 bit integer */
    GATT_FORMAT_SINT16,         /* 0x0E signed 16 bit integer */
    GATT_FORMAT_SINT24,         /* 0x0F signed 24 bit integer */
    GATT_FORMAT_SINT32,         /* 0x10 signed 32 bit integer */
    GATT_FORMAT_SINT48,         /* 0x11 signed 48 bit integer */
    GATT_FORMAT_SINT64,         /* 0x12 signed 64 bit integer */
    GATT_FORMAT_SINT128,        /* 0x13 signed 128 bit integer */
    GATT_FORMAT_FLOAT32,        /* 0x14 float 32 */
    GATT_FORMAT_FLOAT64,        /* 0x15 float 64*/
    GATT_FORMAT_SFLOAT,         /* 0x16 IEEE-11073 16 bit SFLOAT */
    GATT_FORMAT_FLOAT,          /* 0x17 IEEE-11073 32 bit SFLOAT */
    GATT_FORMAT_DUINT16,        /* 0x18 IEEE-20601 format */
    GATT_FORMAT_UTF8S,          /* 0x19 UTF-8 string */
    GATT_FORMAT_UTF16S,         /* 0x1a UTF-16 string */
    GATT_FORMAT_STRUCT,         /* 0x1b Opaque structure*/
    GATT_FORMAT_MAX             /* 0x1c or above reserved */
};
typedef UINT8 tGATT_FORMAT;

/* Characteristic Presentation Format Descriptor value
*/
typedef struct
{
    UINT16              unit;       /* as UUIUD defined by SIG */
    UINT16              descr;       /* as UUID as defined by SIG */
    tGATT_FORMAT        format;
    INT8                exp;
    UINT8               name_spc;   /* The name space of the description */
} tGATT_CHAR_PRES;

/* Characteristic Report reference Descriptor format
*/
typedef struct
{
    UINT8              rpt_id;       /* report ID */
    UINT8              rpt_type;       /* report type */
} tGATT_CHAR_RPT_REF;


#define GATT_VALID_RANGE_MAX_SIZE       16
typedef struct
{
    UINT8                   format;
    UINT16                  len;
    /* in little endian format */
    UINT8                   lower_range[GATT_VALID_RANGE_MAX_SIZE];
    UINT8                   upper_range[GATT_VALID_RANGE_MAX_SIZE];
} tGATT_VALID_RANGE;

/* Characteristic Aggregate Format attribute value
*/
#define GATT_AGGR_HANDLE_NUM_MAX        10
typedef struct
{
    UINT8                   num_handle;
    UINT16                  handle_list[GATT_AGGR_HANDLE_NUM_MAX];
} tGATT_CHAR_AGGRE;

/* Characteristic descriptor: Extended Properties value
*/
#define GATT_CHAR_BIT_REL_WRITE    0x0001       /* permits reliable writes of the
                                                   Characteristic Value */
#define GATT_CHAR_BIT_WRITE_AUX    0x0002       /* permits writes to the
                                                   characteristic descriptor */


/* characteristic descriptor: client configuration value
*/
#define GATT_CLT_CONFIG_NONE               0x0000
#define GATT_CLT_CONFIG_NOTIFICATION       0x0001
#define GATT_CLT_CONFIG_INDICATION         0x0002
typedef UINT16 tGATT_CLT_CHAR_CONFIG;


/* characteristic descriptor: server configuration value
*/
#define GATT_SVR_CONFIG_NONE                     0x0000
#define GATT_SVR_CONFIG_BROADCAST                0x0001
typedef UINT16 tGATT_SVR_CHAR_CONFIG;

/* Characteristic descriptor: Extended Properties value
*/
#define GATT_CHAR_BIT_REL_WRITE    0x0001       /* permits reliable writes of the
                                                   Characteristic Value */
#define GATT_CHAR_BIT_WRITE_AUX    0x0002       /* permits writes to the
                                                   characteristic descriptor */

/* authentication requirement
*/
#define GATT_AUTH_REQ_NONE              0
#define GATT_AUTH_REQ_NO_MITM           1   /* unauthenticated encryption */
#define GATT_AUTH_REQ_MITM              2   /* authenticated encryption */
#define GATT_AUTH_REQ_SIGNED_NO_MITM    3
#define GATT_AUTH_REQ_SIGNED_MITM       4
typedef UINT8 tGATT_AUTH_REQ;

/* Attribute Value structure
*/
typedef struct
{
    UINT16          handle;     /* attribute handle */
    UINT16          offset;     /* attribute value offset, if no offfset is needed for
                                   the command, ignore it */
    UINT16          len;        /* length of attribute value */
    tGATT_AUTH_REQ  auth_req;   /*  authentication request */
    UINT8           value[1];  /* the actual attribute value, more bytes can follow right after the structure */
} tGATT_VALUE;

/* Union of the event data which is used in the server respond API to
** carry the server response information
*/
typedef union
{
    /* data type            member          event   */
    tGATT_VALUE             attr_value;     /* READ, HANDLE_VALUE_IND, PREPARE_WRITE */
    /* READ_BLOB, READ_BY_TYPE */
    UINT16                  handle;         /* WRITE, WRITE_BLOB */

} tGATTS_RSP;

/* Transports for the primary service  */
#define GATT_TRANSPORT_LE           BT_TRANSPORT_LE
#define GATT_TRANSPORT_BR_EDR       BT_TRANSPORT_BR_EDR
#define GATT_TRANSPORT_LE_BR_EDR    (BT_TRANSPORT_LE|BT_TRANSPORT_BR_EDR)
typedef UINT8 tGATT_TRANSPORT;

#define GATT_PREP_WRITE_CANCEL   0x00
#define GATT_PREP_WRITE_EXEC     0x01
typedef UINT8   tGATT_EXEC_FLAG;

/* read request always based on UUID */
typedef struct
{
    UINT16        handle;
    UINT16        offset;
    BOOLEAN       is_long;
    UINT16        *p_val_len; /**< input and output parameter for value length */
    UINT8         *p_val;     /**< value pointer */
} tGATT_READ_REQ;

/* write request data */
typedef struct
{
    UINT16          handle;     /* attribute handle */
    BOOLEAN         is_prep;    /* is prepare write */
    UINT16          offset;     /**< Offset to write */
    UINT16          val_len;    /**< output parameter for value length */
    UINT8           *p_val;     /**< value pointer */
} tGATT_WRITE_REQ;

/* callback data for server access request from client */
typedef union
{
    tGATT_READ_REQ         read_req;       /* read request, read by Type, read blob */

    tGATT_WRITE_REQ        write_req;    /* write */
    /* prepare write */
    /* write blob */
    UINT16                 handle;       /* handle value confirmation */
    UINT16                 mtu;          /* MTU exchange request */
    tGATT_EXEC_FLAG        exec_write;    /* execute write */
} tGATTS_DATA;

typedef UINT8 tGATT_SERV_IF;               /* GATT Service Interface */

enum
{
    GATTS_REQ_TYPE_READ = 1,        /* Attribute read request */
    GATTS_REQ_TYPE_WRITE,           /* Attribute write request */
    GATTS_REQ_TYPE_PREP_WRITE,      /* Attribute Prepare Write Notification */
    GATTS_REQ_TYPE_WRITE_EXEC,      /* Execute write */
    GATTS_REQ_TYPE_MTU,             /* MTU exchange information */
    GATTS_REQ_TYPE_CONF             /* handle value confirmation */
};
typedef UINT8   tGATTS_REQ_TYPE;



/* Client Used Data Structure
*/
/* definition of different discovery types */
enum
{
    GATT_DISC_SRVC_ALL = 1,     /* discover all services */
    GATT_DISC_SRVC_BY_UUID,     /* discover service of a special type */
    GATT_DISC_INC_SRVC,         /* discover the included service within a service */
    GATT_DISC_CHAR,             /* discover characteristics of a service
                                   with/without type requirement */
    GATT_DISC_CHAR_DSCPT,       /* discover characteristic descriptors of a character */
    GATT_DISC_MAX               /* maximnun discover type */
};
typedef UINT8   tGATT_DISC_TYPE;

/* Discover parameters of different discovery types
*/
typedef struct
{
    tBT_UUID    uuid;           /* Service UUID or Characteristic UUID */
    UINT16      s_handle;
    UINT16      e_handle;
} tGATT_DISC_PARAM;

/* GATT read type enumeration
*/
enum
{
    GATT_READ_BY_TYPE =        1,
    GATT_READ_BY_HANDLE,
    GATT_READ_MULTIPLE,
    GATT_READ_CHAR_VALUE,
    GATT_READ_PARTIAL,
    GATT_READ_MAX
};
typedef UINT8 tGATT_READ_TYPE;

/* Read By Type Request (GATT_READ_BY_TYPE) Data
*/
typedef struct
{
    tGATT_AUTH_REQ      auth_req;
    UINT16              s_handle;
    UINT16              e_handle;
    tBT_UUID            uuid;
} tGATT_READ_BY_TYPE;

/*   GATT_READ_MULTIPLE request data
*/
#define GATT_MAX_READ_MULTI_HANDLES      10           /* Max attributes to read in one request */
typedef struct
{
    tGATT_AUTH_REQ          auth_req;
    UINT16                  num_handles;                            /* number of handles to read */
    UINT16                  handles[GATT_MAX_READ_MULTI_HANDLES];   /* handles list to be read */
} tGATT_READ_MULTI;

/*   Read By Handle Request (GATT_READ_BY_HANDLE) data */
typedef struct
{
    tGATT_AUTH_REQ         auth_req;
    UINT16                 handle;
} tGATT_READ_BY_HANDLE;

/*   READ_BT_HANDLE_Request data */
typedef struct
{
    tGATT_AUTH_REQ         auth_req;
    UINT16                 handle;
    UINT16                 offset;
} tGATT_READ_PARTIAL;

/* Read Request Data
*/
typedef union
{
    tGATT_READ_BY_TYPE   service;
    tGATT_READ_BY_TYPE   char_type;        /* characteristic type */
    tGATT_READ_MULTI     read_multiple;
    tGATT_READ_BY_HANDLE by_handle;
    tGATT_READ_PARTIAL   partial;
} tGATT_READ_PARAM;

/* GATT write type enumeration */
enum
{
    GATT_WRITE_NO_RSP = 1,
    GATT_WRITE,
    GATT_WRITE_PREPARE
};
typedef UINT8 tGATT_WRITE_TYPE;

typedef struct
{
    UINT16               handle;             /* valid in GATTC_OPTYPE_READ/GATTC_OPTYPE_NOTIFICATION/GATTC_OPTYPE_INDICATION */
    UINT16               len;                /* valid in GATTC_OPTYPE_READ/GATTC_OPTYPE_NOTIFICATION/GATTC_OPTYPE_INDICATION */
    UINT16               offset;
    UINT8                *p_data;            /* valid GATTC_OPTYPE_READ/GATTC_OPTYPE_NOTIFICATION/GATTC_OPTYPE_INDICATION */
} tGATT_ATTR_DATA;

/* Client Operation Complete Callback Data
*/
typedef union
{
    tGATT_ATTR_DATA      att_value;
    UINT16               mtu;
    UINT16               handle;
} tGATT_CL_COMPLETE;

/* GATT client operation type, used in client callback function
*/
#define GATTC_OPTYPE_NONE                 0
#define GATTC_OPTYPE_DISCOVERY            1
#define GATTC_OPTYPE_READ                 2
#define GATTC_OPTYPE_WRITE                3
#define GATTC_OPTYPE_EXE_WRITE            4
#define GATTC_OPTYPE_CONFIG               5
#define GATTC_OPTYPE_NOTIFICATION         6
#define GATTC_OPTYPE_INDICATION           7
typedef UINT8 tGATTC_OPTYPE;

/* characteristic declaration
*/
typedef struct
{
    tGATT_CHAR_PROP       char_prop;   /* characterisitc properties */
    UINT16                val_handle;  /* characteristic value attribute handle */
    UINT16                handle;      /* characteristic declaration handle */
    tBT_UUID              char_uuid;   /* characteristic UUID type */
} tGATT_CHAR_DCLR_VAL;

/* primary service group data
*/
typedef struct
{
    tBT_UUID        service_type;   /* group type */
    UINT16          s_handle;       /* starting handle of the group */
    UINT16          e_handle;       /* ending handle of the group */
} tGATT_GROUP_VALUE;


/* included service attribute value
*/
typedef struct
{
    tBT_UUID    service_type;       /* included service UUID */
    UINT16      handle;             /* include service value handle */
    UINT16      s_handle;           /* starting handle */
    UINT16      e_handle;           /* ending handle */
} tGATT_INCL_SRVC;

/* discover characteristic descriptor information
*/
typedef struct
{
    tBT_UUID            type;      /* descriptor UUID type */
    UINT16              handle;    /* descriptor attribute handle */
} tGATT_CHAR_DESCR_INFO;

typedef union
{
    tGATT_INCL_SRVC     incl_service;  /*  When used with GATT_DISC_INC_SRVC type
                                          discovery result,include service value */

    tGATT_GROUP_VALUE   group_value;   /* Service UUID type.
                                          This field is used with GATT_DISC_SRVC_ALL
                                          or GATT_DISC_SRVC_BY_UUID
                                          type of discovery result callback. */

    tGATT_CHAR_DCLR_VAL dclr_value;    /* Characteristic declaration value.
                                          This field is used with GATT_DISC_CHAR
                                          type discovery.*/

    tGATT_CHAR_DESCR_INFO   char_descr_info; /* characteristic descriptor information */

} tGATT_DISC_RES;

typedef UINT8 tGATT_IF;

#define GATT_LINK_IDLE_TIMEOUT_WHEN_NO_APP    0 /* start a idle timer for this duration
                                                   when no application need to use the
                                                   link */

#define GATT_LINK_NO_IDLE_TIMEOUT            0xFFFF
#define GATT_INVALID_ACL_HANDLE              0xFFFF

/* default connection ID if skip external value fetching */
#define GATT_FIXED_DB_EXT_VAL_REQ_CONN_LOCAL  0

typedef UINT16 tGATT_APPEARANCE;        /**< GATT appearance (see #gatt_appearance_e in gattdefs.h) */

/*****************************************************************************
 *  GATT Database Defintions
 *****************************************************************************/
/*  The permission bits (see Vol 3, Part F, 3.3.1.1) */
#define LEGATTDB_PERM_NONE                             (0x00)
#define LEGATTDB_PERM_VARIABLE_LENGTH                  (0x1 << 0)
#define LEGATTDB_PERM_READABLE                         (0x1 << 1)
#define LEGATTDB_PERM_WRITE_CMD                        (0x1 << 2)
#define LEGATTDB_PERM_WRITE_REQ                        (0x1 << 3)
#define LEGATTDB_PERM_AUTH_READABLE                    (0x1 << 4)
#define LEGATTDB_PERM_RELIABLE_WRITE                   (0x1 << 5)
#define LEGATTDB_PERM_AUTH_WRITABLE                    (0x1 << 6)

#define LEGATTDB_PERM_WRITABLE  (LEGATTDB_PERM_WRITE_CMD | LEGATTDB_PERM_WRITE_REQ| LEGATTDB_PERM_AUTH_WRITABLE)
#define LEGATTDB_PERM_MASK                             (0x7f)   /* All the permission bits. */
#define LEGATTDB_PERM_SERVICE_UUID_128                 (0x1 << 7)


/*  GATT Characteristic Properties */
#define LEGATTDB_CHAR_PROP_BROADCAST                  (0x1 << 0)
#define LEGATTDB_CHAR_PROP_READ                       (0x1 << 1)
#define LEGATTDB_CHAR_PROP_WRITE_NO_RESPONSE          (0x1 << 2)
#define LEGATTDB_CHAR_PROP_WRITE                      (0x1 << 3)
#define LEGATTDB_CHAR_PROP_NOTIFY                     (0x1 << 4)
#define LEGATTDB_CHAR_PROP_INDICATE                   (0x1 << 5)
#define LEGATTDB_CHAR_PROP_AUTHD_WRITES               (0x1 << 6)
#define LEGATTDB_CHAR_PROP_EXTENDED                   (0x1 << 7)

/* Conversion macros */
#define BIT16_TO_8( val ) \
    (UINT8)(  (val)        & 0xff),/* LSB */ \
    (UINT8)(( (val) >> 8 ) & 0xff) /* MSB */

/* UUID lengths */
#define LEGATTDB_UUID16_SIZE                 2
#define LEGATTDB_UUID128_SIZE                16


/* Service and Characteristic macros  */
#define ATTRIBUTE16(  handle, permission, datalen, uuid ) \
    BIT16_TO_8(handle), \
    (UINT8)(permission), \
    (UINT8)(datalen + 2), \
    BIT16_TO_8(uuid)

#define PRIMARY_SERVICE_UUID16(handle, service)  \
    BIT16_TO_8((UINT16)(handle)), \
    LEGATTDB_PERM_READABLE, \
    4, \
    BIT16_TO_8((GATT_UUID_PRI_SERVICE)), \
    BIT16_TO_8((service))

#define PRIMARY_SERVICE_UUID128(handle, service)  \
    BIT16_TO_8((UINT16)(handle)), \
    LEGATTDB_PERM_READABLE, \
    18, \
    BIT16_TO_8(GATT_UUID_PRI_SERVICE), \
    service

#define SECONDARY_SERVICE_UUID16(handle, service)  \
    BIT16_TO_8((UINT16)(handle)), \
    LEGATTDB_PERM_READABLE, \
    4, \
    BIT16_TO_8((GATT_UUID_SEC_SERVICE)), \
    BIT16_TO_8((service))

#define SECONDARY_SERVICE_UUID128(handle, service)  \
    BIT16_TO_8((UINT16)(handle)), \
    LEGATTDB_PERM_READABLE, \
    18, \
    BIT16_TO_8(GATT_UUID_SEC_SERVICE), \
    service

#define INCLUDE_SERVICE_UUID16(handle, service_handle, end_group_handle, service)  \
    BIT16_TO_8((UINT16)(handle)), \
    LEGATTDB_PERM_READABLE, \
    8, \
    BIT16_TO_8(GATT_UUID_INCLUDE_SERVICE), \
    BIT16_TO_8(service_handle), \
    BIT16_TO_8(end_group_handle), \
    BIT16_TO_8(service)

#define CHARACTERISTIC_UUID16(handle, handle_value, uuid, properties, permission) \
    BIT16_TO_8((UINT16)(handle)), \
    LEGATTDB_PERM_READABLE, \
    0x07, \
    BIT16_TO_8(GATT_UUID_CHAR_DECLARE), \
    (UINT8)(properties), \
    BIT16_TO_8((UINT16)(handle_value)), \
    BIT16_TO_8(uuid), \
    BIT16_TO_8((UINT16)(handle_value)), \
    (UINT8)(permission), \
    (UINT8)(LEGATTDB_UUID16_SIZE), \
    BIT16_TO_8(uuid)

#define CHARACTERISTIC_UUID128(handle, handle_value, uuid, properties, permission) \
    BIT16_TO_8((UINT16)(handle)), \
    LEGATTDB_PERM_READABLE, \
    21, \
    BIT16_TO_8(GATT_UUID_CHAR_DECLARE), \
    (UINT8)(properties), \
    BIT16_TO_8((UINT16)(handle_value)), \
    uuid, \
    BIT16_TO_8((UINT16)(handle_value)), \
    (UINT8)(permission | LEGATTDB_PERM_SERVICE_UUID_128), \
    (UINT8)(LEGATTDB_UUID128_SIZE), \
    uuid

#define CHARACTERISTIC_UUID16_WRITABLE(handle, handle_value, uuid, properties, permission) \
    BIT16_TO_8((UINT16)(handle)), \
    LEGATTDB_PERM_READABLE, \
    0x07, \
    BIT16_TO_8(GATT_UUID_CHAR_DECLARE), \
    (UINT8)(properties), \
    BIT16_TO_8((UINT16)(handle_value)), \
    BIT16_TO_8(uuid), \
    BIT16_TO_8((UINT16)(handle_value)), \
    (UINT8)(permission), \
    (UINT8)(LEGATTDB_UUID16_SIZE), \
    (UINT8)(0), \
    BIT16_TO_8(uuid)

#define CHARACTERISTIC_UUID128_WRITABLE(handle, handle_value, uuid, properties, permission) \
    BIT16_TO_8((UINT16)(handle)), \
    LEGATTDB_PERM_READABLE, \
    21, \
    BIT16_TO_8(GATT_UUID_CHAR_DECLARE), \
    (UINT8)(properties), \
    BIT16_TO_8((UINT16)(handle_value)), \
    uuid, \
    BIT16_TO_8((UINT16)(handle_value)), \
    (UINT8)(permission | LEGATTDB_PERM_SERVICE_UUID_128), \
    (UINT8)(LEGATTDB_UUID128_SIZE), \
    (UINT8)(0), \
    uuid

#define CHAR_DESCRIPTOR_UUID16_WRITABLE(handle, uuid, permission) \
    BIT16_TO_8((UINT16)(handle)), \
    (UINT8)(permission), \
    (UINT8)(LEGATTDB_UUID16_SIZE), \
    (UINT8)(0), \
    BIT16_TO_8(uuid)

#define CHAR_DESCRIPTOR_UUID16(handle, uuid, permission) \
    BIT16_TO_8((UINT16)(handle)), \
    (UINT8)(permission), \
    (UINT8)(LEGATTDB_UUID16_SIZE), \
    BIT16_TO_8(uuid)

#define CHAR_DESCRIPTOR_UUID128_WRITABLE(handle, uuid, permission) \
    BIT16_TO_8((uint16_t)(handle)), \
    (uint8_t)(permission | LEGATTDB_PERM_SERVICE_UUID_128), \
    (uint8_t)(LEGATTDB_UUID128_SIZE), \
    (uint8_t)(0), \
    uuid

#define CHAR_DESCRIPTOR_UUID128(handle, uuid, permission) \
    BIT16_TO_8((uint16_t)(handle)), \
    (uint8_t)(permission | LEGATTDB_PERM_SERVICE_UUID_128), \
    (uint8_t)(LEGATTDB_UUID128_SIZE), \
    uuid


/***********************************************************************************//**
 *  @addtogroup  gatt_events        Events
 *  @ingroup     gatt_call_back
 *
 *  <b> GATT Events </b>
 *
 *  @{
 ***************************************************************************************/
/** GATT events */
typedef enum
{
    GATT_CONNECTION_STATUS_EVT,                         /**< GATT connection status change. Event data: tGATT_CONNECTION_STATUS_EVT */
    GATT_OPERATION_CPLT_EVT,                            /**< GATT operation complete. Event data: tGATT_OPERATION_CPLT_EVT */
    GATT_DISCOVERY_RESULT_EVT,                          /**< GATT attribute discovery result. Event data: tGATT_DISCOVERY_RESULT_EVT */
    GATT_DISCOVERY_CPLT_EVT,                            /**< GATT attribute discovery complete. Event data: tGATT_DISCOVERY_CPLT_EVT */
    GATT_ATTRIBUTE_REQUEST_EVT,                         /**< GATT attribute request (from remote client). Event data: tGATT_ATTRIBUTE_REQUEST_EVT */
    GATT_CONGESTION_EVT                                 /**< GATT congestion (running low in tx buffers). Event data: tGATT_CONGESTION_EVT */
} tGATT_EVENT;
/**@}  gatt_events */


/***********************************************************************************//**
 *  @addtogroup  gatt_event_data        Event Data Structures
 *  @ingroup     gatt_call_back
 *
 *  <b> GATT Event Data Structures </b>
 *
 *  @{
 ***************************************************************************************/

/** Discovery result (used by GATT_DISCOVERY_RESULT_EVT notification) */
typedef struct
{
    UINT16      conn_id;                                /**< ID of the connection */
    tGATT_DISC_TYPE                discovery_type;      /**< Discovery type - @link GATT_DISC_SRVC_ALL  tGATT_DISC_TYPE                @endlink */
    tGATT_DISC_RES                 discovery_data;      /**< Discovery data  */
} tGATT_DISCOVERY_RESULT_EVT;

/** Discovery Complete (used by GATT_DISCOVERY_CPLT_EVT notification) */
typedef struct
{
    UINT16                          conn_id;    /**< ID of the connection */
    tGATT_DISC_TYPE                 disc_type;  /**< Discovery type - @link GATT_DISC_SRVC_ALL  tGATT_DISC_TYPE                @endlink */
    tGATT_STATUS                    status;     /**< Status of operation  @link  tGATT_STATUS           @endlink */
} tGATT_DISCOVERY_CPLT_EVT;

/** Response to read/write/disc/config operations (used by GATT_OPERATION_CPLT_EVT notification) */
typedef struct
{
    UINT16                                  conn_id;            /**< ID of the connection */
    tGATTC_OPTYPE                           op;                 /**< Type of operation completed @link GATTC_OPTYPE_NONE  tGATTC_OPTYPE @endlink */
    tGATT_STATUS                            status;             /**< Status of operation  @link  tGATT_STATUS           @endlink */
    tGATT_CL_COMPLETE                       response_data;      /**< Response data (dependent on optype) */
} tGATT_OPERATION_CPLT_EVT;

/** GATT connection status (used by GATT_CONNECTION_STATUS_EVT notification) */
typedef struct
{
    UINT8           *bd_addr;               /**< Remote device address */
    tBLE_ADDR_TYPE   addr_type;             /**< Remmote device address type */
    UINT16          conn_id;                /**< ID of the connection */
    BOOLEAN         connected;              /**< TRUE if connected, FALSE if disconnected */
    tGATT_DISCONN_REASON reason;            /**< Reason code (see @link tGATT_DISCONN_REASON @linkend) */
    tBT_TRANSPORT   transport;              /**< Transport type of the connection */
    UINT8           link_role;              /**< Link role on this connection */
} tGATT_CONNECTION_STATUS_EVT;

/** GATT attribute request (used by GATT_ATTRIBUTE_REQUEST_EVT notification) */
typedef struct
{
    UINT16          conn_id;                /**< ID of the connection */
    tGATTS_REQ_TYPE request_type;           /**< Request type */
    tGATTS_DATA     request_info;           /**< Information about attribute being request (dependent on request type) */
} tGATT_ATTRIBUTE_REQUEST_EVT;

/** channel congestion/uncongestion (used by GATT_CONGESTION_EVT notification) */
typedef struct
{
    UINT16          conn_id;                /**< ID of the connection */
    BOOLEAN         congested;              /**< congestion state */
} tGATT_CONGESTION_EVT;

/** Stuctures for GATT event notifications */
typedef union
{
    tGATT_DISCOVERY_RESULT_EVT          discovery_result;   /**< Data for GATT_DISCOVERY_RESULT_EVT */
    tGATT_DISCOVERY_CPLT_EVT            discovery_cplt;     /**< Data for GATT_DISCOVERY_CPLT_EVT */
    tGATT_OPERATION_CPLT_EVT            operation_cplt;     /**< Data for GATT_OPERATION_CPLT_EVT */
    tGATT_CONNECTION_STATUS_EVT         connection_status;  /**< Data for GATT_CONNECTION_STATUS_EVT */
    tGATT_ATTRIBUTE_REQUEST_EVT         attribute_request;  /**< Data for GATT_ATTRIBUTE_REQUEST_EVT */
    tGATT_CONGESTION_EVT                congestion;         /**< Data for GATT_CONGESTION_EVT */
} tGATT_EVENT_DATA;

/**@}  gatt_event_data */

/****************************************************************************/
/**
 *  @addtogroup  gatt_callback_type        Callback Function
 *  @ingroup     gatt_call_back
 *
 *  <b> GATT Callback Function </b>
 *
 *  @{
 */
/****************************************************************************/

/**
 * GATT event notification callback
 *
 * Callback for GATT event notifications
 * Registered using GATT_Register()
 *
 * @param event             : Event ID
 * @param p_event_data      : Event data
 *
 * @return Status of event handling
*/
typedef tGATT_STATUS tGATT_CBACK(tGATT_EVENT event, tGATT_EVENT_DATA *p_event_data);
/**@} gatt_callback_type */
/**@} gatt_call_back */

/*****************************************************************************
**  External Function Declarations
*****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif


/*******************************************************************************/
/* GATT Profile API Functions */
/*******************************************************************************/
/* GATT Profile Server Functions */
/*******************************************************************************/


/*******************************************************************************
**
** Function         GATTs_HandleValueIndication
**
** Description      This function sends a handle value indication to a client.
**
** Parameter        conn_id: connection identifier.
**                  attr_handle: Attribute handle of this handle value indication.
**                  val_len: Length of the indicated attribute value.
**                  p_val: Pointer to the indicated attribute value data.
**
** Returns          GATT_SUCCESS if sucessfully sent or queued; otherwise error code.
**
*******************************************************************************/
GATT_API extern  tGATT_STATUS GATTS_HandleValueIndication(UINT16 conn_id,
        UINT16 attr_handle,
        UINT16 val_len,
        UINT8 *p_val);

/*******************************************************************************
**
** Function         GATTS_HandleValueNotification
**
** Description      This function sends a handle value notification to a client.
**
** Parameter       conn_id: connection identifier.
**                  attr_handle: Attribute handle of this handle value indication.
**                  val_len: Length of the indicated attribute value.
**                  p_val: Pointer to the indicated attribute value data.
**
** Returns          GATT_SUCCESS if sucessfully sent; otherwise error code.
**
*******************************************************************************/
GATT_API extern  tGATT_STATUS GATTS_HandleValueNotification(UINT16 conn_id,
        UINT16 attr_handle,
        UINT16 val_len,
        UINT8 *p_val);


/*******************************************************************************
**
** Function         GATTS_SendRsp
**
** Description      This function sends the server response to client.
**
** Parameter        conn_id: connection identifier.
**                  status: response status
**                  attr_handle: attribute handle.
**                  attr_len: attribute length
**                  offset: attribute offset
**                  p_attr: attribute value pointer
**
** Returns          GATT_SUCCESS if sucessfully sent; otherwise error code.
**
*******************************************************************************/
GATT_API extern tGATT_STATUS GATTS_SendRsp(UINT16 conn_id, tGATT_STATUS status,
        UINT16 attr_handle, UINT16 attr_len,
        UINT16 offset, UINT8 *p_attr);

/*****************************************************************************
**  Prebuilt database definitions and APIs
*****************************************************************************/
/***************************************************************************//**
**
** Function         GATTS_DbInit
**
** Description      Inialize GATT database
**
** @param[in]       p_gatt_db:      First element in GATT database array
** @param[in]       size:           size (in bytes) of GATT database
** @param[in]       p_ext_attr:     Table of external attributes (external to GATT database)
** @param[in]       ext_attr_num:   Number of external attributes
**
**  @return @link tGATT_STATUS tGATT_STATUS @endlink
**          - GATT_SUCCESS    : If operation started successfully
**          - gatt error code : Otherwise
**
*******************************************************************************/
tGATT_STATUS GATTS_DbInit(const UINT8 *p_gatt_db, UINT16 size);

/*******************************************************************************/
/* GATT Profile Client Functions */
/*******************************************************************************/

/*******************************************************************************
**
** Function         GATTC_ConfigureMTU
**
** Description      This function is called to configure the ATT MTU size for
**                  a connection on an LE transport.
**
** Parameters       conn_id: connection identifier.
**                  mtu    - attribute MTU size..
**
** Returns          GATT_SUCCESS if command started successfully.
**
*******************************************************************************/
GATT_API extern tGATT_STATUS GATTC_ConfigureMTU(UINT16 conn_id, UINT16  mtu);

/*******************************************************************************
**
** Function         GATTC_Discover
**
** Description      This function is called to do a discovery procedure on ATT server.
**
** Parameters       conn_id: connection identifier.
**                  disc_type:discovery type.
**                  p_param: parameters of discovery requirement.
**
** Returns          GATT_SUCCESS if command received/sent successfully.
**
*******************************************************************************/
GATT_API extern tGATT_STATUS GATTC_Discover(UINT16 conn_id, tGATT_DISC_TYPE disc_type,
        tGATT_DISC_PARAM *p_param);

/*******************************************************************************
**
** Function         GATTC_Read
**
** Description      This function is called to read the value of an attribute from
**                  the server.
**
** Parameters       conn_id: connection identifier.
**                  type    - attribute read type.
**                  p_read  - read operation parameters.
**
** Returns          GATT_SUCCESS if command started successfully.
**
*******************************************************************************/
GATT_API extern tGATT_STATUS GATTC_Read(UINT16 conn_id, tGATT_READ_TYPE type,
                                        tGATT_READ_PARAM *p_read);

/*******************************************************************************
**
** Function         GATTC_Write
**
** Description      This function is called to read the value of an attribute from
**                  the server.
**
** Parameters       conn_id: connection identifier.
**                  type    - attribute write type.
**                  p_write  - write operation parameters.
**
** Returns          GATT_SUCCESS if command started successfully.
**
*******************************************************************************/
GATT_API extern tGATT_STATUS GATTC_Write(UINT16 conn_id, tGATT_WRITE_TYPE type,
        tGATT_VALUE *p_write);


/*******************************************************************************
**
** Function         GATTC_ExecuteWrite
**
** Description      This function is called to send an Execute write request to
**                  the server.
**
** Parameters       conn_id: connection identifier.
**                  is_execute - to execute or cancel the prepare write requet(s)
**
** Returns          GATT_SUCCESS if command started successfully.
**
*******************************************************************************/
GATT_API extern tGATT_STATUS GATTC_ExecuteWrite(UINT16 conn_id, BOOLEAN is_execute);

/*******************************************************************************
**
** Function         GATTC_SendHandleValueConfirm
**
** Description      This function is called to send a handle value confirmation
**                  as response to a handle value notification from server.
**
** Parameters       conn_id: connection identifier.
**                  handle: the handle of the attribute confirmation.
**
** Returns          GATT_SUCCESS if command started successfully.
**
*******************************************************************************/
GATT_API extern tGATT_STATUS GATTC_SendHandleValueConfirm(UINT16 conn_id,
        UINT16 handle);

#ifndef BTE_SMALL_STACK
/***************************************************************************//**
**
** Function         GATT_SetTraceLevel
**
** Description      This function sets the trace level.If called with
**                  a value of 0xFF, it simply returns the current trace level.
**
**  @param[in]  new_level    : new level
**
**  @return     <b> The new or current trace level </b>
**
*******************************************************************************/
GATT_API extern UINT8 GATT_SetTraceLevel(UINT8 new_level);
#endif /* !BTE_SMALL_STACK */

/*******************************************************************************
**
** Function         GATT_SetIdleTimeout
**
** Description      This function (common to both client and server) sets the idle
**                  timeout for a tansport connection
**
** Parameter        bd_addr:   target device bd address.
**                  idle_tout: timeout value in seconds.
**                  transport: trasnport option.
**
** Returns          void
**
*******************************************************************************/
GATT_API extern void GATT_SetIdleTimeout(BD_ADDR bd_addr, UINT16 idle_tout,
        tGATT_TRANSPORT transport);


/*******************************************************************************
**
** Function         GATT_Register
**
** Description      Register GATT application callback
**
** Returns          GATT_SUCCESS
**
*******************************************************************************/
GATT_API extern tGATT_STATUS GATT_Register(tGATT_CBACK *p_gatt_cback);

/*******************************************************************************
**
** Function         GATT_Deregister
**
** Description      This function deregistered the application from GATT.
**
** Parameters       gatt_if: application interface.
**
** Returns          None.
**
*******************************************************************************/
GATT_API extern void GATT_Deregister(tGATT_IF gatt_if);

/*******************************************************************************
**
** Function         GATT_BLE_Connect
**
** Description      This function initiate a connecttion to a remote device on GATT
**                  channel over BLE bearer.
**
** Parameters       gatt_if: applicaiton interface
**                  bd_addr: peer device address.
**                  conn_mode: connection scan mode used for connection initaition over LE
**                  is_direct: is a direct conenection or a background auto connection
**
** Returns          TRUE if connection started; FALSE if connection start failure.
**
*******************************************************************************/
GATT_API extern  BOOLEAN GATT_BLE_Connect(tGATT_IF gatt_if, BD_ADDR bd_addr,
        tBLE_ADDR_TYPE bd_addr_type,
        tBLE_CONN_MODE conn_mode,
        BOOLEAN is_direct);

/*******************************************************************************
**
** Function         GATT_BR_Connect
**
** Description      This function initiate a connecttion to a remote device on GATT
**                  channel over BR/EDR bearer.
**
** Parameters       gatt_if: applicaiton interface
**                  bd_addr: peer device address.
**
** Returns          TRUE if connection started; FALSE if connection start failure.
**
*******************************************************************************/
GATT_API extern BOOLEAN GATT_BR_Connect(tGATT_IF gatt_if, BD_ADDR bd_addr);

/*******************************************************************************
**
** Function         GATT_CancelConnect
**
** Description      This function terminate the connection initaition to a remote
**                  device on GATT channel.
**
** Parameters       gatt_if: client interface. If 0 used as unconditionally disconnect,
**                          typically used for direct connection cancellation.
**                  bd_addr: peer device address.
**                  is_direct: is a direct conenection or a background auto connection
**
** Returns          TRUE if connection started; FALSE if connection start failure.
**
*******************************************************************************/
GATT_API extern BOOLEAN GATT_CancelConnect(tGATT_IF gatt_if, BD_ADDR bd_addr,
        BOOLEAN is_direct);

/*******************************************************************************
**
** Function         GATT_Disconnect
**
** Description      This function disconnect the GATT channel for this registered
**                  application.
**
** Parameters       conn_id: connection identifier.
**
** Returns          GATT_SUCCESS if disconnected.
**
*******************************************************************************/
GATT_API extern tGATT_STATUS GATT_Disconnect(UINT16 conn_id);


/*******************************************************************************
**
** Function         GATT_GetConnectionInfor
**
** Description      This function use conn_id to find its associated BD address
**                  and application interface
**
** Parameters        conn_id: connection id  (input)
**                   p_gatt_if: application interface (output)
**                   bd_addr: peer device address. (output)
**
** Returns          TRUE the ligical link information is found for conn_id
**
*******************************************************************************/
GATT_API extern BOOLEAN GATT_GetConnectionInfor(UINT16 conn_id, tGATT_IF *p_gatt_if,
        BD_ADDR bd_addr,
        tBT_TRANSPORT *p_transport);


/*******************************************************************************
**
** Function         GATT_GetConnIdIfConnected
**
** Description      This function find the conn_id if the logical link for BD address
**                  and application interface is connected
**
** Parameters        gatt_if: application interface (input)
**                   bd_addr: peer device address. (input)
**                   p_conn_id: connection id  (output)
**
** Returns          TRUE the ligical link is connected
**
*******************************************************************************/
GATT_API extern BOOLEAN GATT_GetConnIdIfConnected(tGATT_IF gatt_if, BD_ADDR bd_addr,
        UINT16 *p_conn_id,
        tBT_TRANSPORT transport);

/*******************************************************************************
**
** Function         GATT_Listen
**
** Description      This function start or stop LE advertisement and listen for
**                  connection.
**
** Parameters       gatt_if: application interface
**                  p_bd_addr: listen for specific address connection, or NULL for
**                             listen to all device connection.
**                  start: is a direct conenection or a background auto connection
**
** Returns          TRUE if advertisement is started; FALSE if adv start failure.
**
*******************************************************************************/
GATT_API extern BOOLEAN GATT_Listen(tGATT_IF gatt_if, BOOLEAN start,
                                    BD_ADDR_PTR bd_addr);

/*******************************************************************************
**
** Function         GATT_ConfigServiceChangeCCC
**
** Description      Configure service change indication on remote device
**
** Returns          None.
**
*******************************************************************************/
GATT_API extern void GATT_ConfigServiceChangeCCC(BD_ADDR remote_bda, BOOLEAN enable,
        tBT_TRANSPORT transport);

#ifdef __cplusplus

}
#endif

#endif  /* GATT_API_H */
