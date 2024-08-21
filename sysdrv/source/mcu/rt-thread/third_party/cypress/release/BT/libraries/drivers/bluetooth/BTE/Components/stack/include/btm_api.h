/*****************************************************************************
**
**  Name:          btm_api.h
**
**  Description:   This file contains the Bluetooth Manager (BTM) API function
**                 external definitions.
**
**                 The BTM consists of several management entities:
**                      1. Device Control - controls the local device
**                      2. Device Discovery - manages inquiries, discover database
**                      3. ACL Channels - manages ACL connections
**                      4. SCO Channels - manages SCO connections
**                      5. Security - manages all security functionality
**                      6. Power Management - manages park, sniff, hold, etc. (optional)
**
**  Copyright (c) 1999-2015, Broadcom Corp., All Rights Reserved.
**  Broadcom Bluetooth Core. Proprietary and confidential.
******************************************************************************/
#ifndef BTM_API_H
#define BTM_API_H

#include "bt_target.h"
#include "sdp_api.h"
#include "hcimsgs.h"

#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE
//#include "smp_api.h"
#endif
/*************************************
**  Power Manager Callback Functions
**************************************/
typedef UINT8 tBTM_PM_STATUS;
typedef void (tBTM_PM_STATUS_CBACK)(BD_ADDR p_bda, tBTM_PM_STATUS status,
                                    UINT16 value, UINT8 hci_status);
/*****************************************************************************
**  DEVICE CONTROL and COMMON
*****************************************************************************/
/*****************************
**  Device Control Constants
******************************/
/* Maximum number of bytes allowed for vendor specific command parameters */
#define BTM_MAX_VENDOR_SPECIFIC_LEN  HCI_COMMAND_SIZE

#ifndef BTEWICED

/* BTM application return status codes */
enum
{
    BTM_SUCCESS = 0,                    /* 0  Command succeeded                 */
    BTM_CMD_STARTED,                    /* 1  Command started OK.               */
    BTM_BUSY,                           /* 2  Device busy with another command  */
    BTM_NO_RESOURCES,                   /* 3  No resources to issue command     */
    BTM_MODE_UNSUPPORTED,               /* 4  Request for 1 or more unsupported modes */
    BTM_ILLEGAL_VALUE,                  /* 5  Illegal parameter value           */
    BTM_WRONG_MODE,                     /* 6  Device in wrong mode for request  */
    BTM_UNKNOWN_ADDR,                   /* 7  Unknown remote BD address         */
    BTM_DEVICE_TIMEOUT,                 /* 8  Device timeout                    */
    BTM_BAD_VALUE_RET,                  /* 9  A bad value was received from HCI */
    BTM_ERR_PROCESSING,                 /* 10 Generic error                     */
    BTM_NOT_AUTHORIZED,                 /* 11 Authorization failed              */
    BTM_DEV_RESET,                      /* 12 Device has been reset             */
    BTM_CMD_STORED,                     /* 13 request is stored in control block */
    BTM_ILLEGAL_ACTION,                 /* 14 state machine gets illegal command */
    BTM_DELAY_CHECK,                    /* 15 delay the check on encryption */
    BTM_SCO_BAD_LENGTH,                 /* 16 Bad SCO over HCI data length */
    BTM_SUCCESS_NO_SECURITY,            /* 17 security passed, no security set  */
    BTM_FAILED_ON_SECURITY,             /* 18 security failed                   */
    BTM_REPEATED_ATTEMPTS               /* 19 repeated attempts for LE security requests */
#if (BTM_BR_SC_INCLUDED == TRUE)
    , BTM_MODE4_LEVEL4_NOT_SUPPORTED    /* 20 Secure Connections Only Mode can't be supported */
#endif
};
typedef UINT8 tBTM_STATUS;
#else
#include "wiced_result.h"
typedef enum
{
    BTM_SUCCESS                     = WICED_BT_SUCCESS,                     /* Command succeeded                 */
    BTM_CMD_STARTED                 = WICED_BT_PENDING,                     /* Command started OK.               */
    BTM_BUSY                        = WICED_BT_BUSY,                        /* Device busy with another command  */
    BTM_NO_RESOURCES                = WICED_BT_NO_RESOURCES,                /* No resources to issue command     */
    BTM_MODE_UNSUPPORTED            = WICED_BT_UNSUPPORTED,                 /* Request for 1 or more unsupported modes */
    BTM_ILLEGAL_VALUE               = WICED_BT_ILLEGAL_VALUE,               /* Illegal parameter value           */
    BTM_WRONG_MODE                  = WICED_BT_WRONG_MODE,                  /* Device in wrong mode for request  */
    BTM_UNKNOWN_ADDR                = WICED_BT_UNKNOWN_ADDR,                /* Unknown remote BD address         */
    BTM_DEVICE_TIMEOUT              = WICED_BT_TIMEOUT,                     /* Device timeout                    */
    BTM_BAD_VALUE_RET               = WICED_BT_BAD_VALUE_RET,               /* A bad value was received from HCI */
    BTM_ERR_PROCESSING              = WICED_BT_ERROR,                       /* Generic error                     */
    BTM_NOT_AUTHORIZED              = WICED_BT_NOT_AUTHORIZED,              /* Authorization failed              */
    BTM_DEV_RESET                   = WICED_BT_DEV_RESET,                   /* Device has been reset             */
    BTM_CMD_STORED                  = WICED_BT_CMD_STORED,                  /* request is stored in control block */
    BTM_ILLEGAL_ACTION              = WICED_BT_ILLEGAL_ACTION,              /* state machine gets illegal command */
    BTM_DELAY_CHECK                 = WICED_BT_DELAY_CHECK,                 /* delay the check on encryption */
    BTM_SCO_BAD_LENGTH              = WICED_BT_SCO_BAD_LENGTH,              /* Bad SCO over HCI data length */
    BTM_SUCCESS_NO_SECURITY         = WICED_BT_SUCCESS_NO_SECURITY,         /* security passed, no security set  */
    BTM_FAILED_ON_SECURITY          = WICED_BT_FAILED_ON_SECURITY,          /* security failed                   */
    BTM_REPEATED_ATTEMPTS           = WICED_BT_REPEATED_ATTEMPTS,           /* repeated attempts for LE security requests */
    BTM_MODE4_LEVEL4_NOT_SUPPORTED  = WICED_BT_MODE4_LEVEL4_NOT_SUPPORTED,  /* Secure Connections Only Mode can't be supported */
    BTM_USE_DEFAULT_SECURITY        = WICED_BT_USE_DEFAULT_SECURITY,        /* Use default security */
    BTM_KEY_MISSING                 = WICED_BT_KEY_MISSING,                 /* Key Missing */
    BTM_ENCRYPT_DISABLED            = WICED_BT_ENCRYPT_DISABLED              /* Encryption is disabled */
} tBTM_STATUS;
#endif
/*************************
**  Device Control Types
**************************/
#define BTM_DEVICE_ROLE_BR      0x01
#define BTM_DEVICE_ROLE_DUAL    0x02
#define BTM_MAX_DEVICE_ROLE     BTM_DEVICE_ROLE_DUAL
typedef UINT8 tBTM_DEVICE_ROLE;

/* Device name of peer (may be truncated to save space in BTM database) */
typedef UINT8 tBTM_BD_NAME[BTM_MAX_REM_BD_NAME_LEN + 1];

/* Structure returned with local version information */
typedef struct
{
    UINT8       hci_version;
    UINT16      hci_revision;
    UINT8       lmp_version;
    UINT16      manufacturer;
    UINT16      lmp_subversion;
} tBTM_VERSION_INFO;

/* Structure returned with Vendor Specific Command complete callback */
typedef struct
{
    UINT16  opcode;
    UINT16  param_len;
    UINT8   *p_param_buf;
} tBTM_VSC_CMPL;

#define  BTM_VSC_CMPL_DATA_SIZE  (BTM_MAX_VENDOR_SPECIFIC_LEN + sizeof(tBTM_VSC_CMPL))
/**************************************************
**  Device Control and General Callback Functions
***************************************************/
/* Callback function for when device status changes. Appl must poll for
** what the new state is (BTM_IsDeviceUp). The event occurs whenever the stack
** has detected that the controller status has changed. This asynchronous event
** is enabled/disabled by calling BTM_RegisterForDeviceStatusNotif().
*/
enum
{
    BTM_DEV_STATUS_UP,
    BTM_DEV_STATUS_DOWN,
    BTM_DEV_STATUS_CMD_TOUT
};

typedef UINT8 tBTM_DEV_STATUS;


typedef void (tBTM_DEV_STATUS_CB)(tBTM_DEV_STATUS status);


/* Callback function for when a vendor specific event occurs. The length and
** array of returned parameter bytes are included. This asynchronous event
** is enabled/disabled by calling BTM_RegisterForVSEvents().
*/
typedef void (tBTM_VS_EVT_CB)(UINT8 len, UINT8 *p);

/* General callback function for notifying an application that a synchronous
** BTM function is complete. The pointer contains the address of any returned data.
*/
typedef void (tBTM_CMPL_CB)(void *p1);

/* VSC callback function for notifying an application that a synchronous
** BTM function is complete. The pointer contains the address of any returned data.
*/
typedef void (tBTM_VSC_CMPL_CB)(tBTM_VSC_CMPL *p1);

/* Callback for apps to check connection and inquiry filters.
** Parameters are the BD Address of remote and the Dev Class of remote.
** If the app returns none zero, the connection or inquiry result will be dropped.
*/
typedef UINT8(tBTM_FILTER_CB)(BD_ADDR bd_addr, DEV_CLASS dc);

/*****************************************************************************
**  DEVICE DISCOVERY - Inquiry, Remote Name, Discovery, Class of Device
*****************************************************************************/
/*******************************
**  Device Discovery Constants
********************************/
/* Discoverable modes */
#ifndef BTM_DISCOVERABILITY_MODE    /* To avoid redefintions when including wiced_bt_dev.h */
#define BTM_DISCOVERABILITY_MODE
#define BTM_NON_DISCOVERABLE        0
#define BTM_LIMITED_DISCOVERABLE    1
#define BTM_GENERAL_DISCOVERABLE    2
#define BTM_DISCOVERABLE_MASK       (BTM_LIMITED_DISCOVERABLE|BTM_GENERAL_DISCOVERABLE)
#define BTM_MAX_DISCOVERABLE        BTM_GENERAL_DISCOVERABLE
#endif /* BTM_DISCOVERABILITY_MODE */

/* Connectable modes */
#ifndef BTM_CONNECTABILITY_MODE     /* To avoid redefintions when including wiced_bt_dev.h */
#define BTM_CONNECTABILITY_MODE
#define BTM_NON_CONNECTABLE         0
#define BTM_CONNECTABLE             1
#define BTM_CONNECTABLE_MASK        (BTM_NON_CONNECTABLE | BTM_CONNECTABLE)
#endif /* BTM_CONNECTABILITY_MODE */

/* Inquiry modes
 * Note: These modes are associated with the inquiry active values (BTM_*ACTIVE) */
#ifndef BTM_INQUIRY_MODE            /* To avoid redefintions when including wiced_bt_dev.h */
#define BTM_INQUIRY_MODE
#define BTM_INQUIRY_NONE            0
#define BTM_GENERAL_INQUIRY         0x01
#define BTM_LIMITED_INQUIRY         0x02
#define BTM_BR_INQUIRY_MASK         (BTM_GENERAL_INQUIRY | BTM_LIMITED_INQUIRY)
#endif /* BTM_INQUIRY_MODE */

/* BTM_IsInquiryActive return values (Bit Mask)
 * Note: These bit masks are associated with the inquiry modes (BTM_*_INQUIRY) */
#define BTM_INQUIRY_INACTIVE        0x0     /* no inquiry in progress */
#define BTM_GENERAL_INQUIRY_ACTIVE  BTM_GENERAL_INQUIRY     /* a general inquiry is in progress */
#define BTM_LIMITED_INQUIRY_ACTIVE  BTM_LIMITED_INQUIRY     /* a limited inquiry is in progress */
#define BTM_PERIODIC_INQUIRY_ACTIVE 0x8     /* a periodic inquiry is active */
#define BTM_SSP_INQUIRY_ACTIVE      0x4     /* SSP is active, so inquiry is disallowed (work around for FW bug) */
#define BTM_LE_GENERAL_INQUIRY_ACTIVE  BTM_BLE_GENERAL_INQUIRY     /* a general inquiry is in progress */
#define BTM_LE_LIMITED_INQUIRY_ACTIVE  BTM_BLE_LIMITED_INQUIRY     /* a limited inquiry is in progress */

/* inquiry activity mask */
#define BTM_BR_INQ_ACTIVE_MASK         (BTM_GENERAL_INQUIRY_ACTIVE|BTM_LIMITED_INQUIRY_ACTIVE|BTM_PERIODIC_INQUIRY_ACTIVE) /* BR/EDR inquiry activity mask */
#define BTM_BLE_SCAN_ACTIVE_MASK       0xF0     /* LE scan activity mask */
#define BTM_INQUIRY_ACTIVE_MASK        (BTM_BR_INQ_ACTIVE_MASK | BTM_BLE_INQUIRY_MASK) /* inquiry activity mask */

/* Define scan types */
#define BTM_SCAN_TYPE_STANDARD      0
#define BTM_SCAN_TYPE_INTERLACED    1       /* 1.2 devices only */

/* Define inquiry results mode */
#define BTM_INQ_RESULT_STANDARD     0
#define BTM_INQ_RESULT_WITH_RSSI    1
#define BTM_INQ_RESULT_EXTENDED     2

#define BTM_INQ_RES_IGNORE_RSSI     0x7f    /* RSSI value not supplied (ignore it) */
#define BTM_SCAN_PARAM_IGNORE       0       /* Passed to BTM_SetScanConfig() to ignore */

/* Inquiry Filter Condition types (see tBTM_INQ_PARMS) */
#ifndef BTM_INQUIRY_FILTER               /* To avoid redefintions when including wiced_bt_dev.h */
#define BTM_INQUIRY_FILTER
#define BTM_CLR_INQUIRY_FILTER          0                   /* Inquiry Filtering is turned off */
#define BTM_FILTER_COND_DEVICE_CLASS    HCI_FILTER_COND_DEVICE_CLASS /* Filter on device class */
#define BTM_FILTER_COND_BD_ADDR         HCI_FILTER_COND_BD_ADDR /* Filter on device addr */
#endif /* BTM_INQUIRY_FILTER */

/* State of the remote name retrieval during inquiry operations.
** Used in the tBTM_INQ_INFO structure, and returned in the
** BTM_InqDbRead, BTM_InqDbFirst, and BTM_InqDbNext functions.
** The name field is valid when the state returned is
** BTM_INQ_RMT_NAME_DONE */
#define BTM_INQ_RMT_NAME_EMPTY      0
#define BTM_INQ_RMT_NAME_PENDING    1
#define BTM_INQ_RMT_NAME_DONE       2
#define BTM_INQ_RMT_NAME_FAILED     3

/*********************************
 *** Class of Device constants ***
 *********************************/
#define BTM_FORMAT_TYPE_1   0x00

/****************************
** minor device class field
*****************************/

/* 0x00 is used as unclassified for all minor device classes */
#define BTM_COD_MINOR_UNCLASSIFIED          0x00

/* minor device class field for Computer Major Class */
/* #define BTM_COD_MINOR_UNCLASSIFIED       0x00    */
#define BTM_COD_MINOR_DESKTOP_WORKSTATION   0x04
#define BTM_COD_MINOR_SERVER_COMPUTER       0x08
#define BTM_COD_MINOR_LAPTOP                0x0C
#define BTM_COD_MINOR_HANDHELD_PC_PDA       0x10    /* clam shell */
#define BTM_COD_MINOR_PALM_SIZE_PC_PDA      0x14
#define BTM_COD_MINOR_WEARABLE_COMPUTER     0x18    /* watch sized */

/* minor device class field for Phone Major Class */
/* #define BTM_COD_MINOR_UNCLASSIFIED       0x00    */
#define BTM_COD_MINOR_CELLULAR              0x04
#define BTM_COD_MINOR_CORDLESS              0x08
#define BTM_COD_MINOR_SMART_PHONE           0x0C
#define BTM_COD_MINOR_WIRED_MDM_V_GTWY      0x10 /* wired modem or voice gatway */
#define BTM_COD_MINOR_ISDN_ACCESS           0x14

/* minor device class field for LAN Access Point Major Class */
/* Load Factor Field bit 5-7 */
#define BTM_COD_MINOR_FULLY_AVAILABLE       0x00
#define BTM_COD_MINOR_1_17_UTILIZED         0x20
#define BTM_COD_MINOR_17_33_UTILIZED        0x40
#define BTM_COD_MINOR_33_50_UTILIZED        0x60
#define BTM_COD_MINOR_50_67_UTILIZED        0x80
#define BTM_COD_MINOR_67_83_UTILIZED        0xA0
#define BTM_COD_MINOR_83_99_UTILIZED        0xC0
#define BTM_COD_MINOR_NO_SERVICE_AVAILABLE  0xE0
/* sub-Field bit 2-4 */
/* #define BTM_COD_MINOR_UNCLASSIFIED       0x00    */

/* minor device class field for Audio/Video Major Class */
/* #define BTM_COD_MINOR_UNCLASSIFIED       0x00    */
#define BTM_COD_MINOR_CONFM_HEADSET         0x04
#define BTM_COD_MINOR_CONFM_HANDSFREE       0x08
#define BTM_COD_MINOR_MICROPHONE            0x10
#define BTM_COD_MINOR_LOUDSPEAKER           0x14
#define BTM_COD_MINOR_HEADPHONES            0x18
#define BTM_COD_MINOR_PORTABLE_AUDIO        0x1C
#define BTM_COD_MINOR_CAR_AUDIO             0x20
#define BTM_COD_MINOR_SET_TOP_BOX           0x24
#define BTM_COD_MINOR_HIFI_AUDIO            0x28
#define BTM_COD_MINOR_VCR                   0x2C
#define BTM_COD_MINOR_VIDEO_CAMERA          0x30
#define BTM_COD_MINOR_CAMCORDER             0x34
#define BTM_COD_MINOR_VIDEO_MONITOR         0x38
#define BTM_COD_MINOR_VIDDISP_LDSPKR        0x3C
#define BTM_COD_MINOR_VIDEO_CONFERENCING    0x40
#define BTM_COD_MINOR_GAMING_TOY            0x48

/* minor device class field for Peripheral Major Class */
/* Bits 6-7 independently specify mouse, keyboard, or combo mouse/keyboard */
#define BTM_COD_MINOR_KEYBOARD              0x40
#define BTM_COD_MINOR_POINTING              0x80
#define BTM_COD_MINOR_COMBO                 0xC0
/* Bits 2-5 OR'd with selection from bits 6-7 */
/* #define BTM_COD_MINOR_UNCLASSIFIED       0x00    */
#define BTM_COD_MINOR_JOYSTICK              0x04
#define BTM_COD_MINOR_GAMEPAD               0x08
#define BTM_COD_MINOR_REMOTE_CONTROL        0x0C
#define BTM_COD_MINOR_SENSING_DEVICE        0x10
#define BTM_COD_MINOR_DIGITIZING_TABLET     0x14
#define BTM_COD_MINOR_CARD_READER           0x18    /* e.g. SIM card reader */
#define BTM_COD_MINOR_DIGITAL_PAN           0x1C
#define BTM_COD_MINOR_HAND_SCANNER          0x20
#define BTM_COD_MINOR_HAND_GESTURAL_INPUT   0x24

/* minor device class field for Imaging Major Class */
/* Bits 5-7 independently specify display, camera, scanner, or printer */
#define BTM_COD_MINOR_DISPLAY               0x10
#define BTM_COD_MINOR_CAMERA                0x20
#define BTM_COD_MINOR_SCANNER               0x40
#define BTM_COD_MINOR_PRINTER               0x80
/* Bits 2-3 Reserved */
/* #define BTM_COD_MINOR_UNCLASSIFIED       0x00    */

/* minor device class field for Wearable Major Class */
/* Bits 2-7 meaningful    */
#define BTM_COD_MINOR_WRIST_WATCH           0x04
#define BTM_COD_MINOR_PAGER                 0x08
#define BTM_COD_MINOR_JACKET                0x0C
#define BTM_COD_MINOR_HELMET                0x10
#define BTM_COD_MINOR_GLASSES               0x14

/* minor device class field for Toy Major Class */
/* Bits 2-7 meaningful    */
#define BTM_COD_MINOR_ROBOT                 0x04
#define BTM_COD_MINOR_VEHICLE               0x08
#define BTM_COD_MINOR_DOLL_ACTION_FIGURE    0x0C
#define BTM_COD_MINOR_CONTROLLER            0x10
#define BTM_COD_MINOR_GAME                  0x14

/* minor device class field for Health Major Class */
/* Bits 2-7 meaningful    */
#define BTM_COD_MINOR_BLOOD_MONITOR         0x04
#define BTM_COD_MINOR_THERMOMETER           0x08
#define BTM_COD_MINOR_WEIGHING_SCALE        0x0C
#define BTM_COD_MINOR_GLUCOSE_METER         0x10
#define BTM_COD_MINOR_PULSE_OXIMETER        0x14
#define BTM_COD_MINOR_HEART_PULSE_MONITOR   0x18
#define BTM_COD_MINOR_HEALTH_DATA_DISPLAY   0x1C
#define BTM_COD_MINOR_STEP_COUNTER          0x20
#define BTM_COD_MINOR_BODY_COM_ANALYZER     0x24
#define BTM_COD_MINOR_PEAK_FLOW_MONITOR     0x28
#define BTM_COD_MINOR_MEDICATION_MONITOR    0x2C
#define BTM_COD_MINOR_KNEE_PROSTHESIS       0x30
#define BTM_COD_MINOR_ANKLE_PROSTHESIS      0x34
#define BTM_COD_MINOR_GEN_HEALTH_MGR        0x38
#define BTM_COD_MINOR_PERS_HEALTH_MGR       0x3C


/***************************
** major device class field
****************************/
#define BTM_COD_MAJOR_MISCELLANEOUS         0x00
#define BTM_COD_MAJOR_COMPUTER              0x01
#define BTM_COD_MAJOR_PHONE                 0x02
#define BTM_COD_MAJOR_LAN_ACCESS_PT         0x03
#define BTM_COD_MAJOR_AUDIO                 0x04
#define BTM_COD_MAJOR_PERIPHERAL            0x05
#define BTM_COD_MAJOR_IMAGING               0x06
#define BTM_COD_MAJOR_WEARABLE              0x07
#define BTM_COD_MAJOR_TOY                   0x08
#define BTM_COD_MAJOR_HEALTH                0x09
#define BTM_COD_MAJOR_UNCLASSIFIED          0x1F

/***************************
** service class fields
****************************/
#define BTM_COD_SERVICE_LMTD_DISCOVER       0x0020
#define BTM_COD_SERVICE_POSITIONING         0x0100
#define BTM_COD_SERVICE_NETWORKING          0x0200
#define BTM_COD_SERVICE_RENDERING           0x0400
#define BTM_COD_SERVICE_CAPTURING           0x0800
#define BTM_COD_SERVICE_OBJ_TRANSFER        0x1000
#define BTM_COD_SERVICE_AUDIO               0x2000
#define BTM_COD_SERVICE_TELEPHONY           0x4000
#define BTM_COD_SERVICE_INFORMATION         0x8000

/* class of device field macros */
#define BTM_COD_FORMAT_TYPE(u8, pd)         {u8  = pd[2]&0x03;}
#define BTM_COD_MINOR_CLASS(u8, pd)         {u8  = pd[2]&0xFC;}
#define BTM_COD_MAJOR_CLASS(u8, pd)         {u8  = pd[1]&0x1F;}
#define BTM_COD_SERVICE_CLASS(u16, pd)      {u16 = pd[0]; u16<<=8; u16 += pd[1]&0xE0;}

/* to set the fields (assumes that format type is always 0) */
#define FIELDS_TO_COD(pd, mn, mj, sv) {pd[2] = mn; pd[1] =              \
                                       mj+ ((sv)&BTM_COD_SERVICE_CLASS_LO_B); \
                                       pd[0] = (sv) >> 8;}

/* the COD masks */
#define BTM_COD_FORMAT_TYPE_MASK      0x03
#define BTM_COD_MINOR_CLASS_MASK      0xFC
#define BTM_COD_MAJOR_CLASS_MASK      0x1F
#define BTM_COD_SERVICE_CLASS_LO_B    0x00E0
#define BTM_COD_SERVICE_CLASS_MASK    0xFFE0

#ifdef MPAF_CUSTOM_STACK
#define BTM_BLE_KB_APPEARANCE_VAL          0x03c1
#define BTM_BLE_MS_APPEARANCE_VAL          0x03c2
#endif

/* BTM service definitions
** Used for storing EIR data to bit mask
*/
#ifndef BTM_EIR_UUID_LKUP_TBL
#ifndef BTM_EIR_UUID_ENUM           /* To avoid redefintions when including wiced_bt_dev.h */
#define BTM_EIR_UUID_ENUM
enum
{
    BTM_EIR_UUID_SERVCLASS_SERVICE_DISCOVERY_SERVER,
    /*    BTM_EIR_UUID_SERVCLASS_BROWSE_GROUP_DESCRIPTOR,   */
    /*    BTM_EIR_UUID_SERVCLASS_PUBLIC_BROWSE_GROUP,       */
    BTM_EIR_UUID_SERVCLASS_SERIAL_PORT,
    BTM_EIR_UUID_SERVCLASS_LAN_ACCESS_USING_PPP,
    BTM_EIR_UUID_SERVCLASS_DIALUP_NETWORKING,
    BTM_EIR_UUID_SERVCLASS_IRMC_SYNC,
    BTM_EIR_UUID_SERVCLASS_OBEX_OBJECT_PUSH,
    BTM_EIR_UUID_SERVCLASS_OBEX_FILE_TRANSFER,
    BTM_EIR_UUID_SERVCLASS_IRMC_SYNC_COMMAND,
    BTM_EIR_UUID_SERVCLASS_HEADSET,
    /*    BTM_EIR_UUID_SERVCLASS_CORDLESS_TELEPHONY,        */
    BTM_EIR_UUID_SERVCLASS_AUDIO_SOURCE,
    BTM_EIR_UUID_SERVCLASS_AUDIO_SINK,
    BTM_EIR_UUID_SERVCLASS_AV_REM_CTRL_TARGET,
    /*    BTM_EIR_UUID_SERVCLASS_ADV_AUDIO_DISTRIBUTION,    */
    BTM_EIR_UUID_SERVCLASS_AV_REMOTE_CONTROL,
    /*    BTM_EIR_UUID_SERVCLASS_VIDEO_CONFERENCING,        */
    /*    BTM_EIR_UUID_SERVCLASS_INTERCOM,                  */
    BTM_EIR_UUID_SERVCLASS_FAX,
    BTM_EIR_UUID_SERVCLASS_HEADSET_AUDIO_GATEWAY,
    /*    BTM_EIR_UUID_SERVCLASS_WAP,                       */
    /*    BTM_EIR_UUID_SERVCLASS_WAP_CLIENT,                */
    BTM_EIR_UUID_SERVCLASS_PANU,
    BTM_EIR_UUID_SERVCLASS_NAP,
    BTM_EIR_UUID_SERVCLASS_GN,
    BTM_EIR_UUID_SERVCLASS_DIRECT_PRINTING,
    /*    BTM_EIR_UUID_SERVCLASS_REFERENCE_PRINTING,        */
    BTM_EIR_UUID_SERVCLASS_IMAGING,
    BTM_EIR_UUID_SERVCLASS_IMAGING_RESPONDER,
    BTM_EIR_UUID_SERVCLASS_IMAGING_AUTO_ARCHIVE,
    BTM_EIR_UUID_SERVCLASS_IMAGING_REF_OBJECTS,
    BTM_EIR_UUID_SERVCLASS_HF_HANDSFREE,
    BTM_EIR_UUID_SERVCLASS_AG_HANDSFREE,
    BTM_EIR_UUID_SERVCLASS_DIR_PRT_REF_OBJ_SERVICE,
    /*    BTM_EIR_UUID_SERVCLASS_REFLECTED_UI,              */
    BTM_EIR_UUID_SERVCLASS_BASIC_PRINTING,
    BTM_EIR_UUID_SERVCLASS_PRINTING_STATUS,
    BTM_EIR_UUID_SERVCLASS_HUMAN_INTERFACE,
    /*    BTM_EIR_UUID_SERVCLASS_CABLE_REPLACEMENT,         */
    /*    BTM_EIR_UUID_SERVCLASS_HCRP_PRINT,                */
    /*    BTM_EIR_UUID_SERVCLASS_HCRP_SCAN,                 */
    /*    BTM_EIR_UUID_SERVCLASS_COMMON_ISDN_ACCESS,        */
    /*    BTM_EIR_UUID_SERVCLASS_VIDEO_CONFERENCING_GW,     */
    /*    BTM_EIR_UUID_SERVCLASS_UDI_MT,                    */
    /*    BTM_EIR_UUID_SERVCLASS_UDI_TA,                    */
    /*    BTM_EIR_UUID_SERVCLASS_VCP,                       */
    BTM_EIR_UUID_SERVCLASS_SAP,
    BTM_EIR_UUID_SERVCLASS_PBAP_PCE,
    BTM_EIR_UUID_SERVCLASS_PBAP_PSE,
    /*    BTM_EIR_UUID_SERVCLASS_TE_PHONE_ACCESS,           */
    /*    BTM_EIR_UUID_SERVCLASS_ME_PHONE_ACCESS,           */
    BTM_EIR_UUID_SERVCLASS_PHONE_ACCESS,
    BTM_EIR_UUID_SERVCLASS_HEADSET_HS,
    BTM_EIR_UUID_SERVCLASS_PNP_INFORMATION,
    /*    BTM_EIR_UUID_SERVCLASS_GENERIC_NETWORKING,        */
    /*    BTM_EIR_UUID_SERVCLASS_GENERIC_FILETRANSFER,      */
    /*    BTM_EIR_UUID_SERVCLASS_GENERIC_AUDIO,             */
    /*    BTM_EIR_UUID_SERVCLASS_GENERIC_TELEPHONY,         */
    /*    BTM_EIR_UUID_SERVCLASS_UPNP_SERVICE,              */
    /*    BTM_EIR_UUID_SERVCLASS_UPNP_IP_SERVICE,           */
    /*    BTM_EIR_UUID_SERVCLASS_ESDP_UPNP_IP_PAN,          */
    /*    BTM_EIR_UUID_SERVCLASS_ESDP_UPNP_IP_LAP,          */
    /*    BTM_EIR_UUID_SERVCLASS_ESDP_UPNP_IP_L2CAP,        */
    BTM_EIR_UUID_SERVCLASS_VIDEO_SOURCE,
    BTM_EIR_UUID_SERVCLASS_VIDEO_SINK,
    /*    BTM_EIR_UUID_SERVCLASS_VIDEO_DISTRIBUTION         */
    /*    BTM_EIR_UUID_SERVCLASS_HDP_PROFILE                */
    BTM_EIR_UUID_SERVCLASS_MESSAGE_ACCESS,
    BTM_EIR_UUID_SERVCLASS_MESSAGE_NOTIFICATION,
    BTM_EIR_UUID_SERVCLASS_HDP_SOURCE,
    BTM_EIR_UUID_SERVCLASS_HDP_SINK,
    BTM_EIR_UUID_SERVCLASS_MPS,
    BTM_EIR_MAX_SERVICES
};
#endif  /* BTM_EIR_UUID_ENUM */
#endif /* BTM_EIR_UUID_LKUP_TBL */

/* search result in EIR of inquiry database */
#define BTM_EIR_FOUND           0
#define BTM_EIR_NOT_FOUND       1
#define BTM_EIR_UNKNOWN         2

typedef UINT8 tBTM_EIR_SEARCH_RESULT;

#define BTM_EIR_FLAGS_TYPE                  HCI_EIR_FLAGS_TYPE                  /* 0x01 */
#define BTM_EIR_MORE_16BITS_UUID_TYPE       HCI_EIR_MORE_16BITS_UUID_TYPE       /* 0x02 */
#define BTM_EIR_COMPLETE_16BITS_UUID_TYPE   HCI_EIR_COMPLETE_16BITS_UUID_TYPE   /* 0x03 */
#define BTM_EIR_MORE_32BITS_UUID_TYPE       HCI_EIR_MORE_32BITS_UUID_TYPE       /* 0x04 */
#define BTM_EIR_COMPLETE_32BITS_UUID_TYPE   HCI_EIR_COMPLETE_32BITS_UUID_TYPE   /* 0x05 */
#define BTM_EIR_MORE_128BITS_UUID_TYPE      HCI_EIR_MORE_128BITS_UUID_TYPE      /* 0x06 */
#define BTM_EIR_COMPLETE_128BITS_UUID_TYPE  HCI_EIR_COMPLETE_128BITS_UUID_TYPE  /* 0x07 */
#define BTM_EIR_SHORTENED_LOCAL_NAME_TYPE   HCI_EIR_SHORTENED_LOCAL_NAME_TYPE   /* 0x08 */
#define BTM_EIR_COMPLETE_LOCAL_NAME_TYPE    HCI_EIR_COMPLETE_LOCAL_NAME_TYPE    /* 0x09 */
#define BTM_EIR_TX_POWER_LEVEL_TYPE         HCI_EIR_TX_POWER_LEVEL_TYPE         /* 0x0A */
#define BTM_EIR_3D_SYNC_TYPE                HCI_EIR_3D_SYNC_TYPE                /* 0x3D */
#define BTM_EIR_MANUFACTURER_SPECIFIC_TYPE  HCI_EIR_MANUFACTURER_SPECIFIC_TYPE  /* 0xFF */

/* the following EIR tags are defined to OOB, not regular EIR data */
#define BTM_EIR_OOB_BD_ADDR_TYPE            HCI_EIR_OOB_BD_ADDR_TYPE    /* 6 bytes */
#define BTM_EIR_OOB_COD_TYPE                HCI_EIR_OOB_COD_TYPE        /* 3 bytes */
#define BTM_EIR_OOB_SSP_HASH_C_TYPE         HCI_EIR_OOB_SSP_HASH_C_TYPE /* 16 bytes */
#define BTM_EIR_OOB_SSP_RAND_R_TYPE         HCI_EIR_OOB_SSP_RAND_R_TYPE /* 16 bytes */
#if (BTM_BR_SC_INCLUDED == TRUE)
#define BTM_EIR_OOB_SSP_HASH_C_256_TYPE     HCI_EIR_OOB_SSP_HASH_C_256_TYPE /* 16 bytes */
#define BTM_EIR_OOB_SSP_RAND_R_256_TYPE     HCI_EIR_OOB_SSP_RAND_R_256_TYPE /* 16 bytes */
#endif

#define BTM_OOB_DATA_LEN_SIZE       2
#define BTM_OOB_BD_ADDR_SIZE        6
#define BTM_OOB_MANDATORY_SIZE          (BTM_OOB_DATA_LEN_SIZE + BTM_OOB_BD_ADDR_SIZE)  /* i.e. 8 = 2 + 6 */
#if (BTM_BR_SC_INCLUDED == TRUE)
#define BTM_EIR_ELEM_LT_OVERHEAD        2 /* EIR element overhead: 1 byte for EIR element length, 1 byte for EIR element type */
#endif
#define BTM_OOB_COD_SIZE            BT_OOB_COD_SIZE
#define BTM_OOB_HASH_C_SIZE         BT_OOB_HASH_C_SIZE
#define BTM_OOB_RAND_R_SIZE         BT_OOB_RAND_R_SIZE
#if (BTM_BR_SC_INCLUDED == TRUE)
#define BTM_OOB_HASH_C_256_SIZE         BT_OOB_HASH_C_256_SIZE
#define BTM_OOB_RAND_R_256_SIZE         BT_OOB_RAND_R_256_SIZE
#define BTM_OOB_OOB_EXT_DATA_BLOCK_SIZE BTM_OOB_MANDATORY_SIZE \
                                        + (BTM_EIR_ELEM_LT_OVERHEAD + BTM_OOB_HASH_C_SIZE) \
                                        + (BTM_EIR_ELEM_LT_OVERHEAD + BTM_OOB_RAND_R_SIZE) \
                                        + (BTM_EIR_ELEM_LT_OVERHEAD + BTM_OOB_HASH_C_256_SIZE) \
                                        + (BTM_EIR_ELEM_LT_OVERHEAD + BTM_OOB_RAND_R_256_SIZE)
#endif

#if BLE_INCLUDED == TRUE
#define BTM_BLE_SEC_NONE                0
#define BTM_BLE_SEC_ENCRYPT             1 /* encrypt the link using current key */
#define BTM_BLE_SEC_ENCRYPT_NO_MITM     2
#define BTM_BLE_SEC_ENCRYPT_MITM        3
typedef UINT8   tBTM_BLE_SEC_ACT;
#endif
/************************************************************************************************
** BTM Services MACROS handle array of UINT32 bits for more than 32 services
*************************************************************************************************/
/* Determine the number of UINT32's necessary for services */
#define BTM_EIR_ARRAY_BITS          32          /* Number of bits in each array element */
#ifndef BTM_EIR_SERVICE_ARRAY_SIZE
#define BTM_EIR_SERVICE_ARRAY_SIZE  (((UINT32)BTM_EIR_MAX_SERVICES / BTM_EIR_ARRAY_BITS) + \
                                    (((UINT32)BTM_EIR_MAX_SERVICES % BTM_EIR_ARRAY_BITS) ? 1 : 0))
#endif

/* MACRO to set the service bit mask in a bit stream */
#define BTM_EIR_SET_SERVICE(p, service)  (((UINT32 *)(p))[(((UINT32)(service)) / BTM_EIR_ARRAY_BITS)] |=  \
                                    ((UINT32)1 << (((UINT32)(service)) % BTM_EIR_ARRAY_BITS)))


/* MACRO to clear the service bit mask in a bit stream */
#define BTM_EIR_CLR_SERVICE(p, service)  (((UINT32 *)(p))[(((UINT32)(service)) / BTM_EIR_ARRAY_BITS)] &=  \
                                    ~((UINT32)1 << (((UINT32)(service)) % BTM_EIR_ARRAY_BITS)))

/* MACRO to check the service bit mask in a bit stream */
#define BTM_EIR_HAS_SERVICE(p, service)  ((((UINT32 *)(p))[(((UINT32)(service)) / BTM_EIR_ARRAY_BITS)] &  \
                                    ((UINT32)1 << (((UINT32)(service)) % BTM_EIR_ARRAY_BITS))) >> (((UINT32)(service)) % BTM_EIR_ARRAY_BITS))

/* start of EIR in HCI buffer, 4 bytes = HCI Command(2) + Length(1) + FEC_Req(1) */
#define BTM_HCI_EIR_OFFSET          (BT_HDR_SIZE + 4)

/* start of EIR data in HCI buffer, 4 bytes = HCI Command(2) + Length(1) + FEC_Req(1) */
#define BTM_EIR_OFFSET          (4)

/***************************
**  Device Discovery Types
****************************/
/* Definitions of the parameters passed to BTM_StartInquiry and
** BTM_SetPeriodicInquiryMode.
*/
typedef struct              /* contains the two device class condition fields */
{
    DEV_CLASS       dev_class;
    DEV_CLASS       dev_class_mask;
} tBTM_COD_COND;


typedef union               /* contains the inquiry filter condition */
{
    BD_ADDR         bdaddr_cond;
    tBTM_COD_COND   cod_cond;
} tBTM_INQ_FILT_COND;


typedef struct              /* contains the parameters passed to the inquiry functions */
{
    UINT8   mode;                       /* general or limited */
    UINT8   duration;                   /* duration of the inquiry (1.28 sec increments) */
#if (BTM_INQ_DB_INCLUDED == TRUE)
    UINT8   max_resps;                  /* maximum number of responses to return */
    BOOLEAN report_dup;                 /* report duplicated inquiry response with higher RSSI value */
#endif
    UINT8   filter_cond_type;           /* new devices, BD ADDR, COD, or No filtering */
    tBTM_INQ_FILT_COND  filter_cond;    /* filter value based on filter cond type */
} tBTM_INQ_PARMS;

#define BTM_INQ_RESULT_BR       0x01
#define BTM_INQ_RESULT_BLE      0x02

#if (BLE_INCLUDED == TRUE)
#define BTM_BLE_EVT_CONN_ADV        0x00
#define BTM_BLE_EVT_CONN_DIR_ADV    0x01
#define BTM_BLE_EVT_DISC_ADV        0x02
#define BTM_BLE_EVT_NON_CONN_ADV    0x03
#define BTM_BLE_EVT_SCAN_RSP        0x04
typedef UINT8 tBTM_BLE_EVT_TYPE;
#endif

/* These are the fields returned in each device's response to the inquiry.  It
** is returned in the results callback if registered.
*/
typedef struct
{
    UINT16      clock_offset;
    BD_ADDR     remote_bd_addr;
    DEV_CLASS   dev_class;
    UINT8       page_scan_rep_mode;
    UINT8       page_scan_per_mode;
    UINT8       page_scan_mode;
    INT8        rssi;       /* Set to BTM_INQ_RES_IGNORE_RSSI if  not valid */
#if (BTM_EIR_CLIENT_INCLUDED == TRUE)
    UINT32      eir_uuid[BTM_EIR_SERVICE_ARRAY_SIZE];
    BOOLEAN     eir_complete_list;
#endif
} tBTM_INQ_RESULTS;

/* This is the inquiry response information held in its database by BTM, and available
** to applications via BTM_InqDbRead, BTM_InqDbFirst, and BTM_InqDbNext.
*/
typedef struct
{
    tBTM_INQ_RESULTS    results;

    BOOLEAN             appl_knows_rem_name;    /* set by application if it knows the remote name of the peer device.
                                                   This is later used by application to determine if remote name request is
                                                   required to be done. Having the flag here avoid duplicate store of inquiry results */


#if (BTM_INQ_GET_REMOTE_NAME == TRUE || BLE_INCLUDED == TRUE)
    UINT16          remote_name_len;
    tBTM_BD_NAME    remote_name;
    UINT8           remote_name_state;
    UINT8           remote_name_type;
#endif

} tBTM_INQ_INFO;


/* Structure returned with inquiry complete callback */
typedef struct
{
    tBTM_STATUS status;
    UINT8       num_resp;       /* Number of results from the current inquiry */
} tBTM_INQUIRY_CMPL;


/* Structure returned with remote name  request */
typedef struct
{
    UINT16      status;
    BD_ADDR     bd_addr;
    UINT16      length;
    BD_NAME     remote_bd_name;
} tBTM_REMOTE_DEV_NAME;

typedef struct
{
    UINT8   pcm_intf_rate;  /* PCM interface rate: 0: 128kbps, 1: 256 kbps;
                                2:512 bps; 3: 1024kbps; 4: 2048kbps */
    UINT8   frame_type;     /* frame type: 0: short; 1: long */
    UINT8   sync_mode;      /* sync mode: 0: slave; 1: master */
    UINT8   clock_mode;     /* clock mode: 0: slave; 1: master */

} tBTM_SCO_PCM_PARAM;

/****************************************
**  Device Discovery Callback Functions
*****************************************/
/* Callback function for notifications when the BTM gets inquiry response.
** First param is inquiry results database, second is pointer of EIR.
*/
typedef void (tBTM_INQ_RESULTS_CB)(tBTM_INQ_RESULTS *p_inq_results, UINT8 *p_eir);

/*****************************************************************************
**  ACL CHANNEL MANAGEMENT
*****************************************************************************/
/******************
**  ACL Constants
*******************/

/* ACL modes */
#define BTM_ACL_MODE_NORMAL     HCI_MODE_ACTIVE
#define BTM_ACL_MODE_HOLD       HCI_MODE_HOLD
#define BTM_ACL_MODE_SNIFF      HCI_MODE_SNIFF
#define BTM_ACL_MODE_PARK       HCI_MODE_PARK

/* Returned with structure in role switch callback (tBTM_ROLE_SWITCH_CMPL) */
#define BTM_ROLE_MASTER         HCI_ROLE_MASTER
#define BTM_ROLE_SLAVE          HCI_ROLE_SLAVE
#define BTM_ROLE_UNDEFINED      0xff     /* undefined value (error status) */

/* ACL Packet Types */
#define BTM_ACL_PKT_TYPES_MASK_DM1      HCI_PKT_TYPES_MASK_DM1
#define BTM_ACL_PKT_TYPES_MASK_DH1      HCI_PKT_TYPES_MASK_DH1
#define BTM_ACL_PKT_TYPES_MASK_DM3      HCI_PKT_TYPES_MASK_DM3
#define BTM_ACL_PKT_TYPES_MASK_DH3      HCI_PKT_TYPES_MASK_DH3
#define BTM_ACL_PKT_TYPES_MASK_DM5      HCI_PKT_TYPES_MASK_DM5
#define BTM_ACL_PKT_TYPES_MASK_DH5      HCI_PKT_TYPES_MASK_DH5
#define BTM_ACL_PKT_TYPES_MASK_NO_2_DH1 HCI_PKT_TYPES_MASK_NO_2_DH1
#define BTM_ACL_PKT_TYPES_MASK_NO_3_DH1 HCI_PKT_TYPES_MASK_NO_3_DH1
#define BTM_ACL_PKT_TYPES_MASK_NO_2_DH3 HCI_PKT_TYPES_MASK_NO_2_DH3
#define BTM_ACL_PKT_TYPES_MASK_NO_3_DH3 HCI_PKT_TYPES_MASK_NO_3_DH3
#define BTM_ACL_PKT_TYPES_MASK_NO_2_DH5 HCI_PKT_TYPES_MASK_NO_2_DH5
#define BTM_ACL_PKT_TYPES_MASK_NO_3_DH5 HCI_PKT_TYPES_MASK_NO_3_DH5

/***************
**  ACL Types
****************/

/* Structure returned with link policy information (in tBTM_CMPL_CB callback function)
** in response to BTM_ReadLinkPolicy call.
*/
typedef struct
{
    tBTM_STATUS status;
    UINT8       hci_status;
    BD_ADDR     rem_bda;
    UINT16      settings;
} tBTM_LNK_POLICY_RESULTS;

/* Structure returned with Role Switch information (in tBTM_CMPL_CB callback function)
** in response to BTM_SwitchRole call.
*/
typedef struct
{
    UINT8   hci_status;     /* HCI status returned with the event */
    UINT8   role;           /* BTM_ROLE_MASTER or BTM_ROLE_SLAVE */
    BD_ADDR remote_bd_addr; /* Remote BD addr involved with the switch */
} tBTM_ROLE_SWITCH_CMPL;


/* Structure returned with Change Link Key information (in tBTM_CMPL_CB callback function)
** in response to BTM_ChangeLinkKey call.
*/
typedef struct
{
    UINT8   hci_status;     /* HCI status returned with the event */
    BD_ADDR remote_bd_addr; /* Remote BD addr involved with the change link key */
} tBTM_CHANGE_KEY_CMPL;


/* Structure returned with QoS information (in tBTM_CMPL_CB callback function)
** in response to BTM_SetQoS call.
*/
typedef struct
{
    FLOW_SPEC flow;
    UINT16 handle;
    UINT8 status;
} tBTM_QOS_SETUP_CMPL;


/* Structure returned with read RSSI event (in tBTM_CMPL_CB callback function)
** in response to BTM_ReadRSSI call.
*/
typedef struct
{
    tBTM_STATUS status;
    UINT8       hci_status;
    INT8        rssi;
    BD_ADDR     rem_bda;
} tBTM_RSSI_RESULTS;

/*
**BR/EDR packet types detail statistics
*/
typedef PACKED union
{
    UINT16  array[10];
    PACKED struct
    {
        UINT16    null_count;        /**< No.of NULL packets received/transmitted */
        UINT16    pool_count;        /**< No.of POLL packets received/transmitted */
        UINT16    dm1_count;         /**< No.of DM1 packets received/transmitted  */
        UINT16    dh1_count;         /**< No.of DH1 packets received/transmitted  */
        UINT16    dv_count;          /**< No.of DV packets received/transmitted   */
        UINT16    aux1_count;        /**< No.of AUX1 packets received/transmitted */
        UINT16    dm3_count;         /**< No.of DM3 packets received/transmitted  */
        UINT16    dh3_count;         /**< No.of DH3 packets received/transmitted  */
        UINT16    dm5_count;         /**< No.of DM5 packets received/transmitted  */
        UINT16    dh5_count;         /**< No.of DH5 packets received/transmitted  */
    } br_packet_types;
    PACKED struct
    {
        UINT16    null_count;        /**< No.of NULL packets received/transmitted  */
        UINT16    pool_count;        /**< No.of POLL packets received/transmitted  */
        UINT16    dm1_count;         /**< No.of DM1 packets received/transmitted   */
        UINT16    _2_dh1_count;      /**< No.of 2DH1 packets received/transmitted  */
        UINT16    _3_dh1_count;      /**< No.of 3DH1 packets received/transmitted  */
        UINT16    _2_dh3_count;      /**< No.of 2DH3 packets received/transmitted  */
        UINT16    _3_dh3_count;      /**< No.of 3DH3 packets received/transmitted  */
        UINT16    _2_dh5_count;      /**< No.of 2DH5 packets received/transmitted  */
        UINT16    _3_dh5_count;      /**< No.of 3DH5 packets received/transmitted  */
        UINT16    not_used;          /**< Not Used */
    } edr_packet_types;
} tBTM_BR_EDR_PKT_TYPE_STATS;

/*
**BR/EDR link statistics
*/
typedef PACKED struct
{
    tBTM_BR_EDR_PKT_TYPE_STATS rxPkts;  /**< Received packets details */
    tBTM_BR_EDR_PKT_TYPE_STATS txPkts;  /**< Transmitted packets details */
    UINT32 rxAclBytes;        /**< Total Received ACL bytes */
    UINT32 txAclBytes;        /**< Total Transmitted ACL bytes */
    UINT16 hecErrs;           /**< hecErrs packet count */
    UINT16 crcErrs;           /**< crcErrs packet count */
    UINT16 seqnRepeat;        /**< seqnRepeat packet count */
} tBTM_LQ_BR_EDR_STATS;

/*
**Link Quality statistics action type
*/
enum
{
    BTM_CLEAR_LINK_QUALITY_STATS            = 0,                  /**< clear link quality stats */
    BTM_READ_LINK_QUALITY_STATS             = 1,                  /**< read link quality stats */
    BTM_READ_THEN_CLEAR_LINK_QUALITY_STATS  = 2,                  /**< read then clear link quality stats */
};
typedef UINT8 tBTM_LQ_STATS_ACTION;   /**< Link Quality Statistic Action */

/*
**LE link statistics
*/
typedef PACKED struct
{
    UINT32  tx_pkt_cnt;           /**< transmit packet count */
    UINT32  tx_acked_cnt;         /**< transmit packet acknowledged count */
    UINT32  rx_good_pkt_cnt;      /**< received good packet count */
    UINT32  rx_good_bytes;        /**< received good byte count */
    UINT32  rx_all_pkt_sync_to;   /**< all received packet sync timeout count */
    UINT32  rx_all_pkt_crc_err;   /**< all received packet crc error count */
} tBTM_LQ_LE_STATS;

/* Structure returned with read LQ Stats event (in tBTM_CMPL_CB callback function)
** in response to BTM_ReadLQStats call.
*/
typedef PACKED struct
{
    UINT8   status;             /**< event status */
    UINT16  conn_handle;        /**< connection handle of link quality stats */
    tBTM_LQ_STATS_ACTION   action;             /**< Action to be performed*/
    PACKED union
    {
        tBTM_LQ_BR_EDR_STATS br_edr_stats; /**< br edr statistics */
        tBTM_LQ_LE_STATS     le_stats;     /**< le statistics */
    } tBTM_LQ_STATS;
} tBTM_LQ_STATS_RESULTS;

/* Structure returned with read current TX power event (in tBTM_CMPL_CB callback function)
** in response to BTM_ReadTxPower call.
*/
typedef struct
{
    tBTM_STATUS status;
    UINT8       hci_status;
    INT8        tx_power;
    BD_ADDR     rem_bda;
} tBTM_TX_POWER_RESULTS;

/* Structure returned with read link quality event (in tBTM_CMPL_CB callback function)
** in response to BTM_ReadLinkQuality call.
*/
typedef struct
{
    tBTM_STATUS status;
    UINT8       hci_status;
    UINT8       link_quality;
    BD_ADDR     rem_bda;
} tBTM_LINK_QUALITY_RESULTS;

/* Structure returned with read inq tx power quality event (in tBTM_CMPL_CB callback function)
** in response to BTM_ReadInquiryRspTxPower call.
*/
typedef struct
{
    tBTM_STATUS status;
    UINT8       hci_status;
    INT8        tx_power;
} tBTM_INQ_TXPWR_RESULTS;

enum
{
    BTM_BL_CONN_EVT,
    BTM_BL_DISCN_EVT,
    BTM_BL_UPDATE_EVT,
    BTM_BL_ROLE_CHG_EVT,
    BTM_BL_COLLISION_EVT
};
typedef UINT8 tBTM_BL_EVENT;
typedef UINT16 tBTM_BL_EVENT_MASK;

#define BTM_TBFC_SCAN_DISABLE                   0
#define BTM_TBFC_SCAN_ENABLE                    1
typedef UINT8 tBTM_SCAN_MODE;

#define BTM_BL_CONN_MASK        0x0001
#define BTM_BL_DISCN_MASK       0x0002
#define BTM_BL_UPDATE_MASK      0x0004
#define BTM_BL_ROLE_CHG_MASK    0x0008

/* Device features mask definitions */
#define BTM_FEATURE_BYTES_PER_PAGE  HCI_FEATURE_BYTES_PER_PAGE
#define BTM_EXT_FEATURES_PAGE_MAX   HCI_EXT_FEATURES_PAGE_MAX

/* the data type associated with BTM_BL_CONN_EVT */
typedef struct
{
    tBTM_BL_EVENT   event;      /* The event reported. */
    BD_ADDR_PTR     p_bda;      /* The address of the newly connected device */
    DEV_CLASS_PTR   p_dc;       /* The device class */
    BD_NAME_PTR     p_bdn;      /* The device name */
    UINT8          *p_features; /* pointer to the remote device's features page[0] (supported features page) */
#if BLE_INCLUDED == TRUE
    UINT16          handle;     /* connection handle */
    tBT_TRANSPORT   transport; /* link is LE or not */
#endif
} tBTM_BL_CONN_DATA;

/* the data type associated with BTM_BL_DISCN_EVT */
typedef struct
{
    tBTM_BL_EVENT   event;  /* The event reported. */
    BD_ADDR_PTR     p_bda;  /* The address of the disconnected device */
#if BLE_INCLUDED == TRUE
    UINT16          handle; /* disconnected connection handle */
    tBT_TRANSPORT   transport; /* link is LE link or not */
#endif
} tBTM_BL_DISCN_DATA;

/* the data type associated with BTM_BL_UPDATE_EVT */
typedef struct
{
    tBTM_BL_EVENT   event;  /* The event reported. */
    UINT8           busy_level;/* when paging or inquiring, level is 10.
                                * Otherwise, the number of ACL links. */
} tBTM_BL_UPDATE_DATA;

/* the data type associated with BTM_BL_ROLE_CHG_EVT */
typedef struct
{
    tBTM_BL_EVENT   event;      /* The event reported. */
    BD_ADDR_PTR     p_bda;      /* The address of the peer connected device */
    UINT8           new_role;
    UINT8           hci_status; /* HCI status returned with the event */
} tBTM_BL_ROLE_CHG_DATA;

#if (defined BTM_TBFC_INCLUDED && BTM_TBFC_INCLUDED == TRUE)
typedef struct
{
    tBTM_BL_EVENT   event;      /* The event reported. */
    BD_ADDR_PTR     p_bda;      /* The address of the newly connected device */
} tBTM_BL_SUSPEND_DATA;

typedef struct
{
    tBTM_BL_EVENT   event;      /* The event reported. */
    BD_ADDR_PTR     p_bda;      /* The address of the newly connected device */
} tBTM_BL_RESUME_DATA;
#endif

typedef union
{
    tBTM_BL_EVENT           event;  /* The event reported. */
    tBTM_BL_CONN_DATA       conn;   /* The data associated with BTM_BL_CONN_EVT */
    tBTM_BL_DISCN_DATA      discn;  /* The data associated with BTM_BL_DISCN_EVT */
    tBTM_BL_UPDATE_DATA     update; /* The data associated with BTM_BL_UPDATE_EVT */
    tBTM_BL_ROLE_CHG_DATA   role_chg;/*The data associated with BTM_BL_ROLE_CHG_EVT */

} tBTM_BL_EVENT_DATA;

/* Callback function for notifications when the BTM busy level
** changes.
*/
typedef void (tBTM_BL_CHANGE_CB)(tBTM_BL_EVENT_DATA *p_data);

/***************************
**  ACL Callback Functions
****************************/
/* Callback function for notifications when the BTM ACL connection DB
** changes. First param is BD address, second is if added or removed.
** Registered through BTM_AclRegisterForChanges call.
*/
#if BLE_INCLUDED == TRUE
typedef void (tBTM_ACL_DB_CHANGE_CB)(BD_ADDR p_bda, UINT8 *features,
                                     BOOLEAN is_new, UINT16 handle,
                                     tBT_TRANSPORT transport, UINT8 reason);
#else
typedef void (tBTM_ACL_DB_CHANGE_CB)(BD_ADDR p_bda,
                                     BD_NAME p_bdn, UINT8 *features,
                                     BOOLEAN is_new, UINT8 reason);
#endif

/*****************************************************************************
**  SCO CHANNEL MANAGEMENT
*****************************************************************************/
/******************
**  SCO Constants
*******************/

/* Define an invalid SCO index and an invalid HCI handle */
#define BTM_INVALID_SCO_INDEX       0xFFFF
#define BTM_INVALID_HCI_HANDLE      0xFFFF

/* Define an invalid SCO disconnect reason */
#define BTM_INVALID_SCO_DISC_REASON 0xFFFF

/* Define first active SCO index */
#define BTM_FIRST_ACTIVE_SCO_INDEX  BTM_MAX_SCO_LINKS

/* Define SCO packet types used in APIs */
#define BTM_SCO_PKT_TYPES_MASK_HV1  HCI_ESCO_PKT_TYPES_MASK_HV1
#define BTM_SCO_PKT_TYPES_MASK_HV2  HCI_ESCO_PKT_TYPES_MASK_HV2
#define BTM_SCO_PKT_TYPES_MASK_HV3  HCI_ESCO_PKT_TYPES_MASK_HV3
#define BTM_SCO_PKT_TYPES_MASK_EV3  HCI_ESCO_PKT_TYPES_MASK_EV3
#define BTM_SCO_PKT_TYPES_MASK_EV4  HCI_ESCO_PKT_TYPES_MASK_EV4
#define BTM_SCO_PKT_TYPES_MASK_EV5  HCI_ESCO_PKT_TYPES_MASK_EV5
#define BTM_SCO_PKT_TYPES_MASK_NO_2_EV3  HCI_ESCO_PKT_TYPES_MASK_NO_2_EV3
#define BTM_SCO_PKT_TYPES_MASK_NO_3_EV3  HCI_ESCO_PKT_TYPES_MASK_NO_3_EV3
#define BTM_SCO_PKT_TYPES_MASK_NO_2_EV5  HCI_ESCO_PKT_TYPES_MASK_NO_2_EV5
#define BTM_SCO_PKT_TYPES_MASK_NO_3_EV5  HCI_ESCO_PKT_TYPES_MASK_NO_3_EV5

#define BTM_SCO_LINK_ONLY_MASK  (BTM_SCO_PKT_TYPES_MASK_HV1 | \
                                 BTM_SCO_PKT_TYPES_MASK_HV2 | \
                                 BTM_SCO_PKT_TYPES_MASK_HV3)

#define BTM_ESCO_LINK_ONLY_MASK (BTM_SCO_PKT_TYPES_MASK_EV3 | \
                                 BTM_SCO_PKT_TYPES_MASK_EV4 | \
                                 BTM_SCO_PKT_TYPES_MASK_EV5)

#define BTM_SCO_LINK_ALL_PKT_MASK   (BTM_SCO_LINK_ONLY_MASK     | \
                                     BTM_ESCO_LINK_ONLY_MASK)

#define BTM_VALID_SCO_ALL_PKT_TYPE HCI_VALID_SCO_ALL_PKT_TYPE

/***************
**  SCO Types
****************/
#define BTM_LINK_TYPE_SCO           HCI_LINK_TYPE_SCO
#define BTM_LINK_TYPE_ESCO          HCI_LINK_TYPE_ESCO
typedef UINT8 tBTM_SCO_TYPE;


/*************************************************
** BT 4.0 and earlier Synchronous Settings
**************************************************/

/*******************
** SCO Codec Types
********************/
#define BTM_SCO_CODEC_NONE          0x0000
#define BTM_SCO_CODEC_CVSD          0x0001
#define BTM_SCO_CODEC_MSBC          0x0002
typedef UINT16 tBTM_SCO_CODEC_TYPE;

/*******************
** SCO Voice Settings
********************/
#define BTM_VOICE_SETTING_CVSD  ((UINT16)  (HCI_INP_CODING_LINEAR          |   \
                                            HCI_INP_DATA_FMT_2S_COMPLEMENT |   \
                                            HCI_INP_SAMPLE_SIZE_16BIT      |   \
                                            HCI_AIR_CODING_FORMAT_CVSD))

#define BTM_VOICE_SETTING_TRANS ((UINT16)  (HCI_INP_CODING_LINEAR          |   \
                                            HCI_INP_DATA_FMT_2S_COMPLEMENT |   \
                                            HCI_INP_SAMPLE_SIZE_16BIT      |   \
                                            HCI_AIR_CODING_FORMAT_TRANSPNT))

/*************************************************
** BT 4.1 and later Enhanced Synchronous Settings
**************************************************/

/* SCO Data Path */
#define BTM_DATA_PATH_PCM           1                  /* 0x01-0xFE (PCM Chan) */
#define BTM_DATA_PATH_HCI           HCI_DATA_PATH_HCI  /* HCI-0, 0x01-0xFE (PCM Chan) */
#define BTM_DATA_PATH_TEST          HCI_DATA_PATH_TEST /* 0xFF-Audio Test Mode */
typedef UINT8 tBTM_SCO_ROUTE_TYPE;

/* Coding Formats */
#define BTM_CODING_FORMAT_ULAW      HCI_CODING_FORMAT_ULAW   /*  (0x00)  u-Law log    */
#define BTM_CODING_FORMAT_ALAW      HCI_CODING_FORMAT_ALAW   /*  (0x01)  A-Law log    */
#define BTM_CODING_FORMAT_CVSD      HCI_CODING_FORMAT_CVSD   /*  (0x02)  CVSD         */
#define BTM_CODING_FORMAT_TRANSPNT  HCI_CODING_FORMAT_TRANSPNT/* (0x03)  Transparent  */
#define BTM_CODING_FORMAT_LINEAR    HCI_CODING_FORMAT_LINEAR /*  (0x04)  Linear PCM   */
#define BTM_CODING_FORMAT_MSBC      HCI_CODING_FORMAT_MSBC   /*  (0x05)  MSBC PCM     */
#define BTM_CODING_FORMAT_VS        HCI_CODING_FORMAT_VS     /*  (0xFF)  VSC used     */

/* PCM Data Formats */
#define BTM_PCM_DATA_FORMAT_NA      HCI_PCM_DATA_FORMAT_NA  /* (0x0) N/A to coding fmt */
#define BTM_PCM_DATA_FORMAT_1_COMP  HCI_PCM_DATA_FORMAT_1_COMP /* (0x1) 1's complement */
#define BTM_PCM_DATA_FORMAT_2_COMP  HCI_PCM_DATA_FORMAT_2_COMP /* (0x2) 2's complement */
#define BTM_PCM_DATA_FORMAT_SIGN    HCI_PCM_DATA_FORMAT_SIGN   /* (0x3) Sign-magnitude */
#define BTM_PCM_DATA_FORMAT_UNSIGN  HCI_PCM_DATA_FORMAT_UNSIGN /* (0x4) Unsigned       */

/*******************
** SCO Data Status
********************/
enum
{
    BTM_SCO_DATA_CORRECT,
    BTM_SCO_DATA_PAR_ERR,
    BTM_SCO_DATA_NONE,
    BTM_SCO_DATA_PAR_LOST
};
typedef UINT8 tBTM_SCO_DATA_FLAG;

/***************************
**  SCO Callback Functions
****************************/
typedef void (tBTM_SCO_CB)(UINT16 sco_inx);
typedef void (tBTM_SCO_DATA_CB)(UINT16 sco_inx, BT_HDR *p_data,
                                tBTM_SCO_DATA_FLAG status);

/******************
**  eSCO Constants
*******************/
#define BTM_TXRX_64KBITS_RATE       0x00001f40  /* 64 kbits/sec data rate */
#define BTM_TXRX_128KBITS_RATE      0x00003E80  /* 128 kbits/sec data rate */
#define BTM_64KBITS_RATE            BTM_TXRX_64KBITS_RATE   /* 64 kbits/sec data rate */
#define BTM_128KBITS_RATE           BTM_TXRX_128KBITS_RATE  /* 128 kbits/sec data rate */

#ifndef BTM_INPUT_OUTPUT_64K_RATE
#define BTM_INPUT_OUTPUT_64K_RATE   0x00003E80  /* 16000 Bytes/sec over transport */
#endif
#ifndef BTM_INPUT_OUTPUT_128K_RATE
#define BTM_INPUT_OUTPUT_128K_RATE  0x00007D00  /* 32000 Bytes/sec over transport */
#endif


/* Retransmission effort */
#define BTM_ESCO_RETRANS_OFF        0
#define BTM_ESCO_RETRANS_POWER      1
#define BTM_ESCO_RETRANS_QUALITY    2
#define BTM_ESCO_RETRANS_DONTCARE   0xff

/* Max Latency Don't Care */
#define BTM_ESCO_MAX_LAT_DONTCARE   0xffff

/* Enhanced Setup/Accept Synchronous Connection (See hcimsgs.h)
** passed in BTM_SetEnhEScoMode()
*/
typedef tHCI_CODING_FORMAT      tBTM_CODING_FORMAT;
typedef tHCI_ENH_ESCO_PARAMS    tBTM_ENH_ESCO_PARAMS;

/* Structure passed with SCO change command and events.
** Used by both Sync and Enhanced sync messaging
*/
typedef struct
{
    UINT16 max_latency;
    UINT16 packet_types;
    UINT8  retrans_effort;
} tBTM_CHG_ESCO_PARAMS;

/* Returned by BTM_ReadEScoLinkParms() */
typedef struct
{
    UINT16  rx_pkt_len;
    UINT16  tx_pkt_len;
    BD_ADDR bd_addr;
    UINT8   link_type;  /* BTM_LINK_TYPE_SCO or BTM_LINK_TYPE_ESCO */
    UINT8   tx_interval;
    UINT8   retrans_window;
    UINT8   air_mode;
} tBTM_ESCO_DATA;

/*****************************************************************************
**  SECURITY MANAGEMENT
*****************************************************************************/
/*******************************
**  Security Manager Constants
********************************/

/* Security Mode (BTM_SetSecurityMode) */
#define BTM_SEC_MODE_UNDEFINED      0
#define BTM_SEC_MODE_NONE           1
#define BTM_SEC_MODE_SERVICE        2
#define BTM_SEC_MODE_LINK           3
#define BTM_SEC_MODE_SP             4
#define BTM_SEC_MODE_SP_DEBUG       5
#define BTM_SEC_MODE_SC             6


/* Security Service Levels [bit mask] (BTM_SetSecurityLevel)
** Encryption should not be used without authentication
*/
#ifndef BTM_SEC_LEVEL              /* To avoid redefintions when including wiced_bt_dev.h */
#define BTM_SEC_LEVEL
#define BTM_SEC_NONE               0x0000 /* Nothing required */
#define BTM_SEC_IN_AUTHENTICATE    0x0002 /* Inbound call requires authentication */
#define BTM_SEC_IN_ENCRYPT         0x0004 /* Inbound call requires encryption */
#define BTM_SEC_OUT_AUTHENTICATE   0x0010 /* Outbound call requires authentication */
#define BTM_SEC_OUT_ENCRYPT        0x0020 /* Outbound call requires encryption */
#define BTM_SEC_ENCRYPT            0x0024 /* Inbound/outbound requires encryption */
#if (BTM_BR_SC_INCLUDED == TRUE)
#define BTM_SEC_MODE4_LEVEL4       0x0040 /* Secure Connections Only Mode */
#endif
#define BTM_SEC_IN_MITM            0x1000 /* inbound Do man in the middle protection */
#define BTM_SEC_OUT_MITM           0x2000 /* outbound Do man in the middle protection */

#define BTM_SEC_SECURE_CONNECTION  BTM_SEC_MODE4_LEVEL4
#endif /* BTM_SEC_LEVEL */

/* Security Flags [bit mask] (BTM_GetSecurityFlags)
*/
#define BTM_SEC_FLAG_AUTHORIZED     0x01
#define BTM_SEC_FLAG_AUTHENTICATED  0x02
#define BTM_SEC_FLAG_ENCRYPTED      0x04
#define BTM_SEC_FLAG_LKEY_KNOWN     0x10
#define BTM_SEC_FLAG_LKEY_AUTHED    0x20

/* These definitions need to be kept the same values ad the BTM_SEC_LE_LINK_* ones */
#ifndef BTM_SEC_LINK_STATE              /* To avoid redefintions when including wiced_bt_dev.h */
#define BTM_SEC_LINK_STATE
#define BTM_SEC_LINK_ENCRYPTED           0x01
#define BTM_SEC_LINK_PAIRED_WITHOUT_MITM 0x02
#define BTM_SEC_LINK_PAIRED_WITH_MITM    0x04
#endif

/* PIN types */
#define BTM_PIN_TYPE_VARIABLE       HCI_PIN_TYPE_VARIABLE
#define BTM_PIN_TYPE_FIXED          HCI_PIN_TYPE_FIXED

/* Link Key types used to generate the new link key.
** returned in link key notification callback function
*/
#define BTM_LKEY_TYPE_COMBINATION   HCI_LKEY_TYPE_COMBINATION
#define BTM_LKEY_TYPE_LOCAL_UNIT    HCI_LKEY_TYPE_LOCAL_UNIT
#define BTM_LKEY_TYPE_REMOTE_UNIT   HCI_LKEY_TYPE_REMOTE_UNIT
#define BTM_LKEY_TYPE_DEBUG_COMB    HCI_LKEY_TYPE_DEBUG_COMB
#define BTM_LKEY_TYPE_UNAUTH_COMB   HCI_LKEY_TYPE_UNAUTH_COMB
#define BTM_LKEY_TYPE_AUTH_COMB     HCI_LKEY_TYPE_AUTH_COMB
#define BTM_LKEY_TYPE_CHANGED_COMB  HCI_LKEY_TYPE_CHANGED_COMB
#if BTM_BR_SC_INCLUDED == TRUE
#define BTM_LKEY_TYPE_UNAUTH_COMB_P_256 HCI_LKEY_TYPE_UNAUTH_COMB_P_256
#define BTM_LKEY_TYPE_AUTH_COMB_P_256   HCI_LKEY_TYPE_AUTH_COMB_P_256
#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE && BTM_CROSS_TRANSP_KEY_DERIVATION == TRUE
#define BTM_LTK_DERIVED_LKEY_OFFSET 0x20    /* "easy" requirements for LK derived from LTK */
#endif /* BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE && BTM_CROSS_TRANSP_KEY_DERIVATION == TRUE */
#endif /* BTM_BR_SC_INCLUDED == TRUE */
#define BTM_LKEY_TYPE_IGNORE        0xff    /* used when event is response from
                                               hci return link keys request */

/* Protocol level security (BTM_SetSecurityLevel) */
#define BTM_SEC_PROTO_L2CAP         0
#define BTM_SEC_PROTO_SDP           1
#define BTM_SEC_PROTO_TCS           2
#define BTM_SEC_PROTO_RFCOMM        3
#define BTM_SEC_PROTO_OBEX          4
#define BTM_SEC_PROTO_BNEP          5
#define BTM_SEC_PROTO_HID           6   /* HID      */
#define BTM_SEC_PROTO_AVDT          7
#define BTM_SEC_PROTO_MCA           8

/* Determine the number of UINT32's necessary for security services */
#define BTM_SEC_ARRAY_BITS          32          /* Number of bits in each array element */
#define BTM_SEC_SERVICE_ARRAY_SIZE  (((UINT32)BTM_SEC_MAX_SERVICES / BTM_SEC_ARRAY_BITS) + \
                                    (((UINT32)BTM_SEC_MAX_SERVICES % BTM_SEC_ARRAY_BITS) ? 1 : 0))

/* Security service definitions (BTM_SetSecurityLevel)
** Used for Authorization APIs
*/
#define BTM_SEC_SERVICE_SDP_SERVER      0
#define BTM_SEC_SERVICE_SERIAL_PORT     1
#define BTM_SEC_SERVICE_LAN_ACCESS      2
#define BTM_SEC_SERVICE_DUN             3
#define BTM_SEC_SERVICE_IRMC_SYNC       4
#define BTM_SEC_SERVICE_IRMC_SYNC_CMD   5
#define BTM_SEC_SERVICE_OBEX            6
#define BTM_SEC_SERVICE_OBEX_FTP        7
#define BTM_SEC_SERVICE_HEADSET         8
#define BTM_SEC_SERVICE_FAX             9
#define BTM_SEC_SERVICE_HEADSET_AG      10
#define BTM_SEC_SERVICE_PNP_INFO        11
#define BTM_SEC_SERVICE_BPP_JOB         12
#define BTM_SEC_SERVICE_BPP_STATUS      13
#define BTM_SEC_SERVICE_BPP_REF         14
#define BTM_SEC_SERVICE_BNEP_PANU       15
#define BTM_SEC_SERVICE_BNEP_GN         16
#define BTM_SEC_SERVICE_BNEP_NAP        17
#define BTM_SEC_SERVICE_HF_HANDSFREE    18
#define BTM_SEC_SERVICE_AG_HANDSFREE    19
#define BTM_SEC_SERVICE_TE_PHONE_ACCESS 20
#define BTM_SEC_SERVICE_ME_PHONE_ACCESS 21
#define BTM_SEC_SERVICE_HID_SEC_CTRL    22
#define BTM_SEC_SERVICE_HID_NOSEC_CTRL  23
#define BTM_SEC_SERVICE_HID_INTR        24
#define BTM_SEC_SERVICE_BIP             25
#define BTM_SEC_SERVICE_BIP_REF         26
#define BTM_SEC_SERVICE_AVDTP           27
#define BTM_SEC_SERVICE_AVDTP_NOSEC     28
#define BTM_SEC_SERVICE_AVCTP           29
#define BTM_SEC_SERVICE_SAP             30
#define BTM_SEC_SERVICE_PBAP            31
#define BTM_SEC_SERVICE_RFC_MUX         32
#define BTM_SEC_SERVICE_AVCTP_BROWSE    33
#define BTM_SEC_SERVICE_MAP             34
#define BTM_SEC_SERVICE_MAP_NOTIF       35
#define BTM_SEC_SERVICE_MCAP_CTRL       36
#define BTM_SEC_SERVICE_MCAP_DATA       37
#define BTM_SEC_SERVICE_HDP_SNK         38
#define BTM_SEC_SERVICE_HDP_SRC         39
#define BTM_SEC_SERVICE_ATT             40
#define BTM_SEC_SERVICE_3DS             41
#define BTM_SEC_SERVICE_CTN             42
#define BTM_SEC_SERVICE_CTN_NOTIF       43

/* Update these as services are added */
#define BTM_SEC_SERVICE_FIRST_EMPTY     44

#ifndef BTM_SEC_MAX_SERVICES
#define BTM_SEC_MAX_SERVICES            55
#endif

/************************************************************************************************
** Security Services MACROS handle array of UINT32 bits for more than 32 trusted services
*************************************************************************************************/
/* MACRO to set the security service bit mask in a bit stream */
#define BTM_SEC_SET_SERVICE(p, service)  (((UINT32 *)(p))[(((UINT32)(service)) / BTM_SEC_ARRAY_BITS)] |=  \
                                    ((UINT32)1 << (((UINT32)(service)) % BTM_SEC_ARRAY_BITS)))


/* MACRO to clear the security service bit mask in a bit stream */
#define BTM_SEC_CLR_SERVICE(p, service)  (((UINT32 *)(p))[(((UINT32)(service)) / BTM_SEC_ARRAY_BITS)] &=  \
                                    ~((UINT32)1 << (((UINT32)(service)) % BTM_SEC_ARRAY_BITS)))

/* MACRO to check the security service bit mask in a bit stream (Returns TRUE or FALSE) */
#define BTM_SEC_IS_SERVICE_TRUSTED(p, service)    (((((UINT32 *)(p))[(((UINT32)(service)) / BTM_SEC_ARRAY_BITS)]) &   \
                                        (UINT32)(((UINT32)1 << (((UINT32)(service)) % BTM_SEC_ARRAY_BITS)))) ? TRUE : FALSE)

/* MACRO to copy two trusted device bitmask */
#define BTM_SEC_COPY_TRUSTED_DEVICE(p_src, p_dst)   {UINT32 trst; for (trst = 0; trst < BTM_SEC_SERVICE_ARRAY_SIZE; trst++) \
                                                        ((UINT32 *)(p_dst))[trst] = ((UINT32 *)(p_src))[trst];}

/* MACRO to clear two trusted device bitmask */
#define BTM_SEC_CLR_TRUSTED_DEVICE(p_dst)   {UINT32 trst; for (trst = 0; trst < BTM_SEC_SERVICE_ARRAY_SIZE; trst++) \
                                                        ((UINT32 *)(p_dst))[trst] = 0;}

/* Following bits can be provided by host in the trusted_mask array */
/* 0..31 bits of mask[0] (Least Significant Word) */
#define BTM_SEC_TRUST_SDP_SERVER        (1 << BTM_SEC_SERVICE_SDP_SERVER)
#define BTM_SEC_TRUST_SERIAL_PORT       (1 << BTM_SEC_SERVICE_SERIAL_PORT)
#define BTM_SEC_TRUST_LAN_ACCESS        (1 << BTM_SEC_SERVICE_LAN_ACCESS)
#define BTM_SEC_TRUST_DUN               (1 << BTM_SEC_SERVICE_DUN)
#define BTM_SEC_TRUST_IRMC_SYNC         (1 << BTM_SEC_SERVICE_IRMC_SYNC)
#define BTM_SEC_TRUST_IRMC_SYNC_CMD     (1 << BTM_SEC_SERVICE_IRMC_SYNC_CMD)
#define BTM_SEC_TRUST_OBEX              (1 << BTM_SEC_SERVICE_OBEX)
#define BTM_SEC_TRUST_OBEX_FTP          (1 << BTM_SEC_SERVICE_OBEX_FTP)
#define BTM_SEC_TRUST_HEADSET           (1 << BTM_SEC_SERVICE_HEADSET)
#define BTM_SEC_TRUST_FAX               (1 << BTM_SEC_SERVICE_FAX)
#define BTM_SEC_TRUST_HEADSET_AG        (1 << BTM_SEC_SERVICE_HEADSET_AG)
#define BTM_SEC_TRUST_PNP_INFO          (1 << BTM_SEC_SERVICE_PNP_INFO)
#define BTM_SEC_TRUST_BPP_JOB           (1 << BTM_SEC_SERVICE_JOB)
#define BTM_SEC_TRUST_BPP_STATUS        (1 << BTM_SEC_SERVICE_STATUS)
#define BTM_SEC_TRUST_BPP_REF           (1 << BTM_SEC_SERVICE_REF)
#define BTM_SEC_TRUST_BNEP_PANU         (1 << BTM_SEC_SERVICE_BNEP_PANU)
#define BTM_SEC_TRUST_BNEP_GN           (1 << BTM_SEC_SERVICE_BNEP_GN)
#define BTM_SEC_TRUST_BNEP_NAP          (1 << BTM_SEC_SERVICE_BNEP_NAP)
#define BTM_SEC_TRUST_HFP_HF            (1 << BTM_SEC_SERVICE_HF_HANDSFREE)
#define BTM_SEC_TRUST_HFP_AG            (1 << BTM_SEC_SERVICE_AG_HANDSFREE)
#define BTM_SEC_TRUST_TE_PHONE_ACCESS   (1 << BTM_SEC_SERVICE_TE_PHONE_ACCESS)
#define BTM_SEC_TRUST_ME_PHONE_ACCESS   (1 << BTM_SEC_SERVICE_ME_PHONE_ACCESS)
#define BTM_SEC_TRUST_HID_CTRL          (1 << BTM_SEC_SERVICE_HID_SEC_CTRL)
#define BTM_SEC_TRUST_HID_NOSEC_CTRL    (1 << BTM_SEC_SERVICE_HID_NOSEC_CTRL)
#define BTM_SEC_TRUST_HID_INTR          (1 << BTM_SEC_SERVICE_HID_INTR)
#define BTM_SEC_TRUST_BIP               (1 << BTM_SEC_SERVICE_BIP)
#define BTM_SEC_TRUST_BIP_REF           (1 << BTM_SEC_SERVICE_BIP_REF))
#define BTM_SEC_TRUST_AVDTP             (1 << BTM_SEC_SERVICE_AVDTP
#define BTM_SEC_TRUST_AVDTP_NOSEC       (1 << BTM_SEC_SERVICE_AVDTP_NOSEC)
#define BTM_SEC_TRUST_AVCTP             (1 << BTM_SEC_SERVICE_AVCTP)
#define BTM_SEC_TRUST_SAP               (1 << BTM_SEC_SERVICE_SAP)
#define BTM_SEC_TRUST_PBAP              (1 << BTM_SEC_SERVICE_PBAP)

/* 0..31 bits of mask[1] (Most Significant Word) */
#define BTM_SEC_TRUST_RFC_MUX           (1 << (BTM_SEC_SERVICE_RFC_MUX - 32))
#define BTM_SEC_TRUST_AVCTP_BROWSE      (1 << (BTM_SEC_SERVICE_AVCTP_BROWSE - 32))
#define BTM_SEC_TRUST_MAP               (1 << (BTM_SEC_SERVICE_MAP - 32))
#define BTM_SEC_TRUST_MAP_NOTIF         (1 << (BTM_SEC_SERVICE_MAP_NOTIF - 32))
#define BTM_SEC_TRUST_MCAP_CTRL         (1 << (BTM_SEC_SERVICE_MCAP_CTRL - 32))
#define BTM_SEC_TRUST_MCAP_DATA         (1 << (BTM_SEC_SERVICE_MCAP_DATA - 32))
#define BTM_SEC_TRUST_HDP_SNK           (1 << (BTM_SEC_SERVICE_HDP_SNK - 32))
#define BTM_SEC_TRUST_HDP_SRC           (1 << (BTM_SEC_SERVICE_HDP_SRC - 32))
#define BTM_SEC_TRUST_ATT               (1 << (BTM_SEC_SERVICE_ATT - 32))
#define BTM_SEC_TRUST_3DS               (1 << (BTM_SEC_SERVICE_3DS - 32))

#define BTM_SEC_TRUST_ALL               0xFFFFFFFF  /* for each array element */

/* Remote Name Resolved.  Parameters are
**              BD Address of remote
**              BD Name of remote
*/
typedef void (tBTM_RMT_NAME_CALLBACK)(BD_ADDR bd_addr, DEV_CLASS dc,
                                      tBTM_BD_NAME bd_name);


#define BTM_IO_CAP_OUT      0   /* DisplayOnly */
#define BTM_IO_CAP_IO       1   /* DisplayYesNo */
#define BTM_IO_CAP_IN       2   /* KeyboardOnly */
#define BTM_IO_CAP_NONE     3   /* NoInputNoOutput */
#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE
#define BTM_IO_CAP_KBDISP   4   /* Keyboard display */
#define BTM_IO_CAP_MAX      5
#else
#define BTM_IO_CAP_MAX      4
#endif

typedef UINT8 tBTM_IO_CAP;

#define BTM_MAX_PASSKEY_VAL (999999)
#define BTM_MIN_PASSKEY_VAL (0)

#define BTM_AUTH_SP_NO      0   /* MITM Protection Not Required - Single Profile/non-bonding
                                Numeric comparison with automatic accept allowed */
#define BTM_AUTH_SP_YES     1   /* MITM Protection Required - Single Profile/non-bonding
                                Use IO Capabilities to determine authentication procedure */
#define BTM_AUTH_AP_NO      2   /* MITM Protection Not Required - All Profiles/dedicated bonding
                                Numeric comparison with automatic accept allowed */
#define BTM_AUTH_AP_YES     3   /* MITM Protection Required - All Profiles/dedicated bonding
                                Use IO Capabilities to determine authentication procedure */
#define BTM_AUTH_SPGB_NO    4   /* MITM Protection Not Required - Single Profiles/general bonding
                                Numeric comparison with automatic accept allowed */
#define BTM_AUTH_SPGB_YES   5   /* MITM Protection Required - Single Profiles/general bonding
                                Use IO Capabilities to determine authentication procedure */
#define BTM_AUTH_DD_BOND    2   /* this bit is ORed to the BTM_AUTH_SP_* when IO exchange for dedicated bonding */
#define BTM_AUTH_GB_BIT     4   /* the genernal bonding bit */
#define BTM_AUTH_BONDS      6   /* the general/dedicated bonding bits  */
#define BTM_AUTH_YN_BIT     1   /* this is the Yes or No bit  */

typedef UINT8 tBTM_AUTH_REQ;

#ifndef BTM_OOB_STATE
#define BTM_OOB_STATE
enum
{
    BTM_OOB_NONE,
    BTM_OOB_PRESENT,
    BTM_OOB_PRESENT_192 = BTM_OOB_PRESENT,
    BTM_OOB_PRESENT_256,
    BTM_OOB_PRESENT_192_256,
    BTM_OOB_UNKNOWN
};
#endif
typedef UINT8 tBTM_OOB_DATA;

/* data type for BTM_SCO_CONNECTED_EVT */
typedef struct
{
    UINT16 sco_inx;
} tBTM_SCO_CONNECTED;

/* data type for BTM_SCO_DISCONNECTED_EVT */
typedef struct
{
    UINT16 sco_inx;
} tBTM_SCO_DISCONNECTED;

/* data type for BTM_SCO_CONNECTION_REQUEST_EVT */
typedef struct
{
    UINT16        sco_inx;
    BD_ADDR       bd_addr;
    DEV_CLASS     dev_class;
    tBTM_SCO_TYPE link_type;
} tBTM_SCO_CONN_REQ_EVT_DATA;

/* data type for BTM_SCO_CONNECTION_CHANGE_EVT */
typedef struct
{
    UINT16  sco_inx;
    UINT16  rx_pkt_len;
    UINT16  tx_pkt_len;
    BD_ADDR bd_addr;
    UINT8   hci_status;
    UINT8   tx_interval;
    UINT8   retrans_window;
} tBTM_SCO_CONN_CHG_EVT_DATA;

/* data type for BTM_BLE_CONNECTION_PARAM_UPDATE */
typedef struct
{
    UINT8   status;
    BD_ADDR bd_addr;
    UINT16  conn_interval;
    UINT16  conn_latency;
    UINT16  supervision_timeout;
} tBTM_BLE_CONN_PARAM_UPDATE;

/** BLE Physical link update event related data */
typedef struct
{
    UINT8     status;                 /**< LE Phy update status */
    BD_ADDR   bd_address;             /**< peer BD address*/
    UINT8     tx_phy;                 /**< Transmitter PHY, values: 1=1M, 2=2M, 3=LE coded */
    UINT8     rx_phy;                 /**< Receiver PHY, values: 1=1M, 2=2M, 3=LE coded */
} tBTM_BLE_PHY_UPDATE;

#if SMP_CONFORMANCE_TESTING == TRUE
/* data type for BTM_SMP_SC_PEER_INFO_EVT */
typedef struct
{
    BOOLEAN             peer_sc_is_set;     /* Secure Connection    */
    BOOLEAN             peer_kp_n_is_set;   /* KeyPress Notification */
} tBTM_SMP_SC_PEER_INFO;
#endif

/* data type for BTM_SP_IO_RSP_EVT (replaced with ) */
typedef struct
{
    BD_ADDR         bd_addr;        /* peer address */
    tBTM_IO_CAP     io_cap;         /* peer IO capabilities */
    tBTM_OOB_DATA   oob_data;       /* OOB data present at peer device for the local device */
    tBTM_AUTH_REQ   auth_req;       /* Authentication required for peer device */
} tBTM_SP_IO_RSP;

/* data type for BTM_USER_CONFIRMATION_REQUEST_EVT */
typedef struct
{
    BD_ADDR         bd_addr;        /* peer address */
    UINT32          num_val;        /* the numeric value for comparison. If just_works, do not show this number to UI */
    BOOLEAN         just_works;     /* TRUE, if "Just Works" association model */
    tBTM_AUTH_REQ   loc_auth_req;   /* Authentication required for local device */
    tBTM_AUTH_REQ   rmt_auth_req;   /* Authentication required for peer device */
} tBTM_SP_CFM_REQ;

/* data type for BTM_USER_PASSKEY_REQUEST_EVT */
typedef struct
{
    BD_ADDR         bd_addr;        /* peer address */
} tBTM_SP_KEY_REQ;

/* data type for BTM_PASSKEY_NOTIFICATION_EVT */
typedef struct
{
    BD_ADDR         bd_addr;        /* peer address */
    UINT32          passkey;        /* passkey */
} tBTM_SP_KEY_NOTIF;

enum
{
    BTM_SP_KEY_STARTED,         /* 0 - passkey entry started */
    BTM_SP_KEY_ENTERED,         /* 1 - passkey digit entered */
    BTM_SP_KEY_ERASED,          /* 2 - passkey digit erased */
    BTM_SP_KEY_CLEARED,         /* 3 - passkey cleared */
    BTM_SP_KEY_COMPLT,          /* 4 - passkey entry completed */
    BTM_SP_KEY_OUT_OF_RANGE     /* 5 - out of range */
};
typedef UINT8   tBTM_SP_KEY_TYPE;

/* data type for BTM_USER_KEYPRESS_NOTIFICATION_EVT */
typedef struct
{
    BD_ADDR             bd_addr;        /* peer address */
    tBTM_SP_KEY_TYPE   notif_type;
} tBTM_SP_KEYPRESS;

/** Security/authentication failure status  (used by BTM_SECURITY_FAILED_EVT notication) */
typedef struct
{
    BD_ADDR                             bd_addr;        /**< BD Address of remote */
    tBTM_STATUS                         status;         /**< BTM status */
    UINT8                               hci_status;     /**< HCI status */
} tBTM_SECURITY_FAILED_EVT;

/** BR/EDR pairing complete infomation */
typedef struct
{
    UINT8           status;         /**< status of the simple pairing process (HCI status code)  */
} tBTM_PAIRING_INFO_BR_EDR;

/** BLE pairing complete infomation */
typedef struct
{
    tBTM_STATUS     status;                 /**< status of the simple pairing process   */
    UINT8           reason;                 /**< Reason tSMP_STATUS type (UINT8  )    */
    UINT8           sec_level;              /**< 0 - None, 1- Unauthenticated Key, 4-Authenticated Key  */
    BOOLEAN         is_pair_cancel;         /**< True if canceled, else False   */
    BD_ADDR         resolved_bd_addr;       /**< Resolved address (if remote device using private address) */
    tBLE_ADDR_TYPE  resolved_bd_addr_type;  /**< Resolved addr type of bonded device  */
} tBTM_PAIRING_INFO_BLE;

/** Transport dependent pairing complete infomation */
typedef union
{
    tBTM_PAIRING_INFO_BR_EDR  br_edr;         /**< BR/EDR pairing complete infomation */
    tBTM_PAIRING_INFO_BLE     ble;            /**< BLE pairing complete infomation */
} tBTM_PAIRING_INFO;

/** Pairing complete notification (BTM_PAIRING_COMPLETE_EVT event data type) */
typedef struct
{
    UINT8                       *bd_addr;               /**< peer address           */
    tBT_TRANSPORT               transport;              /**< BT_TRANSPORT_BR_EDR or BT_TRANSPORT_LE */
    tBTM_PAIRING_INFO           pairing_complete_info;  /**< Transport dependent pairing complete infomation */
    tBTM_STATUS                 bonding_status;         /**< current status of bonding process to notify app of completion status of storing keys */
} tBTM_PAIRING_COMPLETE_EVT;


/** Security request (BTM_SECURITY_REQUEST_EVT event data type) */
typedef struct
{
    BD_ADDR                     bd_addr;                /**< peer address           */
} tBTM_SECURITY_REQUEST_EVT;

typedef void (tBTM_MKEY_CALLBACK)(BD_ADDR bd_addr, UINT8 status, UINT8 key_flag) ;

/** Scan duty cycle */
#ifndef BTM_BLE_SCAN_TYPE
#define BTM_BLE_SCAN_TYPE
enum
{
    BTM_BLE_SCAN_TYPE_NONE,         /**< Stop scanning */
    BTM_BLE_SCAN_TYPE_HIGH_DUTY,    /**< High duty cycle scan */
    BTM_BLE_SCAN_TYPE_LOW_DUTY      /**< Low duty cycle scan */
};
#endif
typedef UINT8 tBTM_BLE_SCAN_TYPE;   /**< scan type (see #wiced_bt_ble_scan_type_e) */

#ifndef BTM_LE_KEY_TYPES            /* To avoid redefintions when including wiced_bt_dev.h */
#define BTM_LE_KEY_TYPES
#define BTM_LE_KEY_PENC             SMP_SEC_KEY_TYPE_ENC        /* encryption information of peer device */
#define BTM_LE_KEY_PID              SMP_SEC_KEY_TYPE_ID         /* identity key of the peer device */
#define BTM_LE_KEY_PCSRK            SMP_SEC_KEY_TYPE_CSRK       /* peer SRK */
#define BTM_LE_SEC_DEFAULT_KEY      SMP_SEC_DEFAULT_KEY
#if SMP_LE_SC_INCLUDED == TRUE
#define BTM_LE_KEY_PLK              SMP_SEC_KEY_TYPE_LK
#define BTM_LE_KEY_LENC             (SMP_SEC_KEY_TYPE_ENC << 4)       /* master role security information:div */
#define BTM_LE_KEY_LID              (SMP_SEC_KEY_TYPE_ID << 4)        /* master device ID key */
#define BTM_LE_KEY_LCSRK            (SMP_SEC_KEY_TYPE_CSRK << 4)      /* local CSRK has been deliver to peer */
#define BTM_LE_KEY_LLK              (SMP_SEC_KEY_TYPE_LK << 4)
#define BTM_LE_BR_SEC_DEFAULT_KEY   SMP_BR_SEC_DEFAULT_KEY
#else
#define BTM_LE_KEY_LENC             (SMP_SEC_KEY_TYPE_ENC << 3)       /* master role security information:div */
#define BTM_LE_KEY_LID              (SMP_SEC_KEY_TYPE_ID << 3)        /* master device ID key */
#define BTM_LE_KEY_LCSRK            (SMP_SEC_KEY_TYPE_CSRK << 3)      /* local CSRK has been deliver to peer */
#endif
#endif  /* BTM_LE_KEY_TYPES */
typedef UINT8 tBTM_LE_KEY_TYPE;

#define BTM_LE_AUTH_REQ_NO_BOND SMP_AUTH_NO_BOND   /* 0 */
#define BTM_LE_AUTH_REQ_BOND    SMP_AUTH_GEN_BOND  /* 1 << 0 */
#define BTM_LE_AUTH_REQ_MITM    SMP_AUTH_YN_BIT    /* 1 << 2 */
typedef UINT8 tBTM_LE_AUTH_REQ;

#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE
#define BTM_LE_SC_SUPPORT_BIT           SMP_SC_SUPPORT_BIT     /* (1 << 3) */
#define BTM_LE_KP_SUPPORT_BIT           SMP_KP_SUPPORT_BIT     /* (1 << 4) */

#define BTM_LE_AUTH_REQ_SC_ONLY         SMP_AUTH_SC_ENC_ONLY    /* 1 << 3 */
#define BTM_LE_AUTH_REQ_SC_BOND         SMP_AUTH_SC_GB          /* 1001 */
#define BTM_LE_AUTH_REQ_SC_MITM         SMP_AUTH_SC_MITM_NB     /* 1100 */
#define BTM_LE_AUTH_REQ_SC_MITM_BOND    SMP_AUTH_SC_MITM_GB     /* 1101 */
#define BTM_LE_AUTH_REQ_MASK            SMP_AUTH_MASK           /* 0x1D */
#else
#define BTM_LE_AUTH_REQ_MASK    SMP_AUTH_MASK  /* 0x05*/
#endif

/* LE security level */
#define BTM_LE_SEC_NONE             SMP_SEC_NONE
#define BTM_LE_SEC_UNAUTHENTICATE   SMP_SEC_UNAUTHENTICATE      /* 1 */
#define BTM_LE_SEC_AUTHENTICATED    SMP_SEC_AUTHENTICATED       /* 4 */
typedef UINT8 tBTM_LE_SEC;

/* BLE encryption keys */
typedef struct
{
    BT_OCTET16  ltk;
    BT_OCTET8   rand;
    UINT16      ediv;
    UINT8       sec_level;
    UINT8       key_size;
} tBTM_LE_PENC_KEYS;

/* BLE CSRK keys */
typedef struct
{
    UINT32          counter;
    BT_OCTET16      csrk;
    UINT8           sec_level;
} tBTM_LE_PCSRK_KEYS;

/* BLE Encryption reproduction keys */
typedef struct
{
#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE
    BT_OCTET16  ltk;
#endif
    UINT16      div;
    UINT8       key_size;
    UINT8       sec_level;
} tBTM_LE_LENC_KEYS;

/* BLE SRK keys */
typedef struct
{
    UINT32          counter;
    UINT16          div;
    UINT8           sec_level;
#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE
    BT_OCTET16      csrk;
#endif
} tBTM_LE_LCSRK_KEYS;

typedef struct
{
    BT_OCTET16          irk;
    tBLE_ADDR_TYPE      addr_type;
    BD_ADDR             static_addr;
} tBTM_LE_PID_KEYS;

typedef union
{
    tBTM_LE_PENC_KEYS   penc_key;       /* received peer encryption key */
    tBTM_LE_PCSRK_KEYS  pcsrk_key;       /* received peer device SRK */
    tBTM_LE_PID_KEYS    pid_key;        /* peer device ID key */
    tBTM_LE_LENC_KEYS   lenc_key;       /* local encryption reproduction keys LTK = = d1(ER,DIV,0)*/
    tBTM_LE_LCSRK_KEYS   lcsrk_key;      /* local device CSRK = d1(ER,DIV,1)*/
} tBTM_LE_KEY_VALUE;

#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE
/* keypress notifications received via LE SC SM */
#define BTM_LE_SC_KEY_STARTED       SMP_SC_KEY_STARTED          /* 0 - passkey entry started */
#define BTM_LE_SC_KEY_ENTERED       SMP_SC_KEY_ENTERED          /* 1 - passkey digit entered */
#define BTM_LE_SC_KEY_ERASED        SMP_SC_KEY_ERASED           /* 2 - passkey digit erased */
#define BTM_LE_SC_KEY_CLEARED       SMP_SC_KEY_CLEARED          /* 3 - passkey cleared */
#define BTM_LE_SC_KEY_COMPLT        SMP_SC_KEY_COMPLT           /* 4 - passkey entry completed */
#define BTM_LE_SC_KEY_OUT_OF_RANGE  SMP_SC_KEY_OUT_OF_RANGE    /* 5 - out of range */

typedef UINT8    tBTM_LE_SC_KEY_TYPE;
#endif

typedef struct
{
    /* BR/EDR key */
    UINT8                           br_edr_key_type;        /* BR/EDR Link Key type */
    LINK_KEY                        br_edr_key;             /* BR/EDR Link Key */
} tBTM_BR_EDR_KEY;


typedef union
{
    tBTM_LE_KEY_VALUE le_keys;
    tBTM_BR_EDR_KEY  br_edr_key;
} tBTM_SEC_KEY_VALUE;

#define BTM_BLE_KEY_TYPE_ID         1
#define BTM_BLE_KEY_TYPE_ER         2
#define BTM_BLE_KEY_TYPE_COUNTER    3  //tobe obsolete

/** LE Security information of device in Slave Role (used for BTM_PAIRED_DEVICE_LINK_KEYS_UPDATE_EVT) */
typedef struct
{
    BT_OCTET16          irk;            /**< peer diverified identity root */
#if SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE
    BT_OCTET16          pltk;           /**< peer long term key */
    BT_OCTET16          pcsrk;          /**< peer SRK peer device used to secured sign local data  */

    BT_OCTET16          lltk;           /**< local long term key */
    BT_OCTET16          lcsrk;          /**< local SRK peer device used to secured sign local data  */
#else
    BT_OCTET16          ltk;            /**< peer long term key */
    BT_OCTET16          csrk;           /**< peer SRK peer device used to secured sign local data  */
#endif

    BT_OCTET8           rand;           /**< random vector for LTK generation */
    UINT16              ediv;           /**< LTK diversifier of this slave device */
    UINT16              div;            /**< local DIV  to generate local LTK=d1(ER,DIV,0) and CSRK=d1(ER,DIV,1)  */
    UINT8               sec_level;      /**< local pairing security level */
    UINT8               key_size;       /**< key size of the LTK delivered to peer device */
    UINT8               srk_sec_level;  /**< security property of peer SRK for this device */
    UINT8               local_csrk_sec_level;  /**< security property of local CSRK for this device */

    UINT32              counter;        /**< peer sign counter for verifying rcv signed cmd */
    UINT32              local_counter;  /**< local sign counter for sending signed write cmd*/
} tBTM_SEC_BLE_KEYS;

/* Combined BR/EDR and BLE security keys */
typedef struct
{
    /* BR/EDR key */
    UINT8                           br_edr_key_type;        /* BR/EDR Link Key type */
    LINK_KEY                        br_edr_key;             /* BR/EDR Link Key */

    /* LE Keys */
    tBTM_LE_KEY_TYPE                le_keys_available_mask; /*  Mask of available BLE keys */
    tBLE_ADDR_TYPE                  ble_addr_type;          /*  LE device type: public or random address */
    tBLE_ADDR_TYPE                  static_addr_type;       /*  static address type */
    BD_ADDR                         static_addr;            /*  static address */
    tBTM_SEC_BLE_KEYS               le_keys;                /*  LE keys */
} tBTM_SEC_KEYS;

/** Size of key data in tBTM_PAIRED_DEVICE_LINK_KEYS */
#ifndef BTM_SECURITY_KEY_DATA_LEN  /* also defined in wiced_bt_dev.h */
#define BTM_SECURITY_KEY_DATA_LEN       sizeof(tBTM_SEC_KEYS)   /**< Security key data length (used by wiced_bt_device_link_keys_t structure): sizeof(tBTM_SEC_KEYS) */
#endif /* BTM_SECURITY_KEY_DATA_LEN */

typedef struct
{
    BT_OCTET16       ir;
    BT_OCTET16       irk;
    BT_OCTET16       dhk;

} tBTM_BLE_LOCAL_ID_KEYS;

/** New LE identity key for local device (used by BTM_LE_LOCAL_IDENTITY_KEYS_UPDATE_EVT and BTM_LE_LOCAL_KEYS_REQUEST_EVT notification) */
typedef struct
{
    UINT8                           key_type_mask;  /**< The type of the key (BTM_BLE_KEY_TYPE_ID or BTM_BLE_KEY_TYPE_ER) */
    tBTM_BLE_LOCAL_ID_KEYS          id_keys;        /**< TBD    */
    BT_OCTET16                      er;             /**< TBD    */
} tBTM_LOCAL_IDENTITY_KEY_DATA;

#ifndef BTM_SECURITY_LOCAL_KEY_DATA_LEN  /* also defined in wiced_bt_dev.h */
#define BTM_SECURITY_LOCAL_KEY_DATA_LEN sizeof(tBTM_LOCAL_IDENTITY_KEY_DATA)          /**< Local security key data length (used by wiced_bt_local_identity_keys_t structure): sizeof(tBTM_LOCAL_IDENTITY_KEY_DATA) */
#endif /* BTM_SECURITY_LOCAL_KEY_DATA_LEN */

/** LE identity key for local device (used by BTM_LE_LOCAL_IDENTITY_KEYS_UPDATE_EVT and BTM_LE_LOCAL_KEYS_REQUEST_EVT notification) */
typedef struct
{
    UINT8           local_key_data[BTM_SECURITY_LOCAL_KEY_DATA_LEN];
} tBTM_LOCAL_IDENTITY_KEYS;


/* LE Secure connection event data */
/* type of OOB data required from application */
#ifndef BTM_OOB_REQ_TYPE
#define BTM_OOB_REQ_TYPE
enum
{
    BTM_OOB_INVALID_TYPE,
    BTM_OOB_PEER,
    BTM_OOB_LOCAL,
    BTM_OOB_BOTH
};
#endif
typedef UINT8 tBTM_OOB_DATA_TYPE;

/* data type for BTM_SMP_REMOTE_OOB_DATA_REQUEST_EVT */
typedef struct
{
    BD_ADDR             bd_addr;        /* peer address */
} tBTM_SMP_REMOTE_OOB_REQ;

/* data type for BTM_SMP_SC_REMOTE_OOB_DATA_REQUEST_EVT */
typedef struct
{
    BD_ADDR             bd_addr;        /* peer address */
    tBTM_OOB_DATA_TYPE  oob_type;       /* requested oob data type (BTM_OOB_PEER, BTM_OOB_LOCAL, or BTM_OOB_BOTH) */
} tBTM_SMP_SC_REMOTE_OOB_REQ;

typedef struct
{
    BT_OCTET32  x;
    BT_OCTET32  y;
} tBTM_PUBLIC_KEY;

/**< Data for BTM_SMP_SC_LOCAL_OOB_DATA_NOTIFICATION_EVT */
typedef struct
{
    BOOLEAN         present;
    BT_OCTET16      randomizer;
    BT_OCTET16      commitment;

    tBLE_BD_ADDR    addr_sent_to;
    BT_OCTET32      private_key_used;   /* is used to calculate:
                                           publ_key_used = P-256(private_key_used, curve_p256.G) - send it to the other side
                                           dhkey = P-256(private_key_used, publ key rcvd from the other side)
                                           */
    tBTM_PUBLIC_KEY publ_key_used;      /* P-256(private_key_used, curve_p256.G) */
} tBTM_SMP_SC_LOCAL_OOB_DATA;

/** bonding device information from wiced_bt_dev_get_bonded_devices */
typedef PACKED struct
{
    BD_ADDR           bd_addr;                 /**< peer address           */
    tBLE_ADDR_TYPE    addr_type;               /**< peer address type : BLE_ADDR_PUBLIC/BLE_ADDR_RANDOM */
    tBT_DEVICE_TYPE   device_type;             /**< peer device type : BT_DEVICE_TYPE_BREDR/BT_DEVICE_TYPE_BLE/BT_DEVICE_TYPE_BREDR_BLE  */
} tBT_BONDED_DEVICE_INFO_TYPE;




/* Callback function for when a link supervision timeout event occurs.
** This asynchronous event is enabled/disabled by calling BTM_RegForLstoEvt().
*/
typedef void (tBTM_LSTO_CBACK)(BD_ADDR remote_bda, UINT16 timeout);

/*****************************************************************************
**  POWER MANAGEMENT
*****************************************************************************/
/****************************
**  Power Manager Constants
*****************************/
/* BTM Power manager status codes */
#ifndef BTM_PM_STATUS_CODES
#define BTM_PM_STATUS_CODES
enum
{
    BTM_PM_STS_ACTIVE = HCI_MODE_ACTIVE,
    BTM_PM_STS_HOLD   = HCI_MODE_HOLD,
    BTM_PM_STS_SNIFF  = HCI_MODE_SNIFF,
    BTM_PM_STS_PARK   = HCI_MODE_PARK,
    BTM_PM_STS_SSR,     /* report the SSR parameters in HCI_SNIFF_SUB_RATE_EVT */
    BTM_PM_STS_PENDING,   /* when waiting for status from controller */
    BTM_PM_STS_ERROR   /* when HCI command status returns error */
#if BTM_TBFC_INCLUDED == TRUE
    , BTM_PM_STS_SUSPENDED
#endif
};
#endif
//typedef UINT8 tBTM_PM_STATUS;

/* BTM Power manager modes */
enum
{
    BTM_PM_MD_ACTIVE = BTM_PM_STS_ACTIVE,
    BTM_PM_MD_HOLD   = BTM_PM_STS_HOLD,
    BTM_PM_MD_SNIFF  = BTM_PM_STS_SNIFF,
    BTM_PM_MD_PARK   = BTM_PM_STS_PARK,
    BTM_PM_MD_FORCE  = 0x10 /* OR this to force ACL link to a certain mode */
#if BTM_TBFC_INCLUDED == TRUE
    , BTM_PM_MD_SUSPEND = BTM_PM_STS_SUSPENDED
#endif
};
typedef UINT8 tBTM_PM_MODE;

#define BTM_PM_SET_ONLY_ID  0x80

/* Operation codes */
#define BTM_PM_REG_SET      1 /* The module wants to set the desired power mode */
#define BTM_PM_REG_NOTIF    2 /* The module wants to receive mode change event */
#define BTM_PM_DEREG        4 /* The module does not want to involve with PM anymore */

/************************
**  Power Manager Types
*************************/
typedef struct
{
    UINT16          max;
    UINT16          min;
    UINT16          attempt;
    UINT16          timeout;
    tBTM_PM_MODE    mode;
} tBTM_PM_PWR_MD;



/************************
**  Stored Linkkey Types
*************************/
#define BTM_CB_EVT_RETURN_LINK_KEYS         1
#define BTM_CB_EVT_READ_STORED_LINK_KEYS    2
#define BTM_CB_EVT_WRITE_STORED_LINK_KEYS   3
#define BTM_CB_EVT_DELETE_STORED_LINK_KEYS  4

typedef struct
{
    UINT8          event;

} tBTM_STORED_LINK_KEYS_EVT;


typedef struct
{
    UINT8          event;
    UINT8          num_keys;

} tBTM_RETURN_LINK_KEYS_EVT;


typedef struct
{
    BD_ADDR         bd_addr;
    LINK_KEY        link_key;

} tBTM_BD_ADDR_LINK_KEY_PAIR;


typedef struct
{
    UINT8          event;
    UINT8          status;
    UINT16         max_keys;
    UINT16         read_keys;

} tBTM_READ_STORED_LINK_KEY_COMPLETE;


typedef struct
{
    UINT8          event;
    UINT8          status;
    UINT8          num_keys;

} tBTM_WRITE_STORED_LINK_KEY_COMPLETE;


typedef struct
{
    UINT8          event;
    UINT8          status;
    UINT16         num_keys;

} tBTM_DELETE_STORED_LINK_KEY_COMPLETE;


/* These macros are defined to check the Broadcom features supported in controller
 * (the return value for BTM_ReadBrcmFeatures() */
/* multi-av */
#define BTM_FEATURE_MULTI_AV_MASK 0x01
#define BTM_FEATURE_MULTI_AV_OFF  0
#define BTM_VSC_MULTI_AV_SUPPORTED(x) ((x)[BTM_FEATURE_MULTI_AV_OFF] & BTM_FEATURE_MULTI_AV_MASK)

/* WBS SBC codec */
#define BTM_FEATURE_WBS_SBC_MASK 0x02
#define BTM_FEATURE_WBS_SBC_OFF  0
#define BTM_VSC_WBS_SBC_SUPPORTED(x) ((x)[BTM_FEATURE_WBS_SBC_OFF] & BTM_FEATURE_WBS_SBC_MASK)

/* Advanced Audio LC-PLC */
#define BTM_FEATURE_AUDIO_LC_PLC_MASK 0x04
#define BTM_FEATURE_AUDIO_LC_PLC_OFF  0
#define BTM_VSC_AUDIO_LC_PLC_SUPPORTED(x) ((x)[BTM_FEATURE_AUDIO_LC_PLC_OFF] & BTM_FEATURE_AUDIO_LC_PLC_MASK)

/* Light stack for audio routing in Controller */
#define BTM_FEATURE_LIGHT_STACK_MASK 0x08
#define BTM_FEATURE_LIGHT_STACK_OFF  0
#define BTM_VSC_LIGHT_STACK_SUPPORTED(x) ((x)[BTM_FEATURE_LIGHT_STACK_OFF] & BTM_FEATURE_LIGHT_STACK_MASK)

/* NFC support */
#define BTM_FEATURE_NFC_MASK        (HCI_BRCM_FEATURE_NFC_MASK) /* 0x10 */
#define BTM_FEATURE_NFC_OFF         (HCI_BRCM_FEATURE_NFC_OFF)  /* 0    */
#define BTM_VSC_NFC_SUPPORTED(x) ((x)[BTM_FEATURE_NFC_OFF] & BTM_FEATURE_NFC_MASK)


/************************
**  Dual-Stack support
*************************/
/* BTM_SYNC_FAIL_EVT reason codes */
#define BTM_SYNC_SUCCESS                    0
#define BTM_SYNC_FAIL_BTE_SWITCH_REJECTED   1
#define BTM_SYNC_FAIL_TRANS_PAUSE           2
#define BTM_SYNC_FAIL_CORE_SYNC             3
#define BTM_SYNC_FAIL_BTA_SYNC              4
#define BTM_SYNC_FAIL_TRANS_RESUME          5
#define BTM_SYNC_FAIL_RESYNC                6
#define BTM_SYNC_FAIL_ERROR                 7
#define BTM_SYNC_FAIL_UIPC_OPEN             8
typedef UINT8 tBTM_SYNC_STATUS;

/* Direction of sync (used by BTM_SyncStack) */
#define BTM_SW_BB_TO_MM     0
#define BTM_SW_TO_BB        1   /* Switch back to baseband stack (from either MM or BTC host) */
#define BTM_SW_RESYNC       2
#define BTM_SW_BB_TO_BTC    3   /* Switch from baseband stack to Bluetooth Controller Host stack */
#define BTM_SW_MM_TO_BB     4
#define BTM_SW_BTC_TO_BB    5
typedef UINT8 tBTM_SW_DIR;

/* Stack synchronization events (returned by tBTM_SYNC_STACK_CBACK callback) */
#define BTM_SYNC_CPLT_EVT   0
#define BTM_SYNC_BTA_EVT    1
#define BTM_RESYNC_CPLT_EVT 2
#define BTM_UIPC_OPENED_EVT 3
#define BTM_UIPC_CLOSED_EVT 4
typedef UINT8 tBTM_SYNC_STACK_EVT;

/* Synchronization info from BTA/application that will be sent when calling BTE sync request functions */
typedef struct
{
    tBTM_SW_DIR dir;
    UINT16      lcid[BTM_SYNC_INFO_NUM_STR];
    UINT8       avdt_handle[BTM_SYNC_INFO_NUM_STR];
} tBTM_SYNC_INFO;

/* Stack synchonization callback function
** Parameters are
**              event:  stack synchronization event
**              status: BTM_SUCCESS if event was successful
*/
typedef void (*tBTM_SYNC_STACK_CBACK)(tBTM_SYNC_STACK_EVT event, tBTM_SYNC_STATUS status);


/* Sync complete callback function. Called by bte layers after synchronization is complete
** so that BTM_SYNC can procede with the next step for switching stack to MM
**
** Parameters are
**              status: BTM_SUCCESS if synchronization was successful
*/
typedef void (*tBTM_SYNC_CPLT_CBACK)(tBTM_STATUS status);



/* IPC event callback function. Called by BTM when an IPC event is received.
** These events are currently sent to DM through the callback function.
**
** Parameters are
**              status: BTM_SUCCESS if synchronization was successful
**              p_data: Actual message in the IPC
*/
typedef void (tBTM_IPC_EVT_CBACK)(tBTM_STATUS status, BT_HDR *p_data);

/* MIP evnets, callbacks    */
enum
{
    BTM_MIP_MODE_CHG_EVT,
    BTM_MIP_DISCONNECT_EVT,
    BTM_MIP_PKTS_COMPL_EVT,
    BTM_MIP_RXDATA_EVT
};
typedef UINT8 tBTM_MIP_EVT;

typedef struct
{
    tBTM_MIP_EVT    event;
    BD_ADDR         bd_addr;
    UINT16          mip_id;
} tBTM_MIP_MODE_CHANGE;

typedef struct
{
    tBTM_MIP_EVT    event;
    UINT16          mip_id;
    UINT8           disc_reason;
} tBTM_MIP_CONN_TIMEOUT;

#define BTM_MIP_MAX_RX_LEN  17

typedef struct
{
    tBTM_MIP_EVT    event;
    UINT16          mip_id;
    UINT8           rx_len;
    UINT8           rx_data[BTM_MIP_MAX_RX_LEN];
} tBTM_MIP_RXDATA;

typedef struct
{
    tBTM_MIP_EVT    event;
    BD_ADDR         bd_addr;
    UINT8           data[11];       /* data[0] shows Vender-specific device type */
} tBTM_MIP_EIR_HANDSHAKE;

typedef struct
{
    tBTM_MIP_EVT    event;
    UINT16          num_sent;       /* Number of packets completed at the controller */
} tBTM_MIP_PKTS_COMPL;

typedef union
{
    tBTM_MIP_EVT            event;
    tBTM_MIP_MODE_CHANGE    mod_chg;
    tBTM_MIP_CONN_TIMEOUT   conn_tmo;
    tBTM_MIP_EIR_HANDSHAKE  eir;
    tBTM_MIP_PKTS_COMPL     completed;
    tBTM_MIP_RXDATA         rxdata;
} tBTM_MIP_EVENT_DATA;

/* MIP event callback function  */
typedef void (tBTM_MIP_EVENTS_CB)(tBTM_MIP_EVT event, tBTM_MIP_EVENT_DATA data);

/* MIP Device query callback function  */
typedef BOOLEAN(tBTM_MIP_QUERY_CB)(BD_ADDR dev_addr, UINT8 *p_mode, LINK_KEY link_key);



/** Bluetooth Management events */
#ifndef BTM_MANAGEMENT_EVT
#define BTM_MANAGEMENT_EVT
enum
{
    /* Bluetooth status events */
    BTM_ENABLED_EVT,                        /**< Bluetooth controller and host stack enabled. Event data: tBTM_ENABLED_EVT */
    BTM_DISABLED_EVT,                       /**< Bluetooth controller and host stack disabled. Event data: NULL */
    BTM_POWER_MANAGEMENT_STATUS_EVT,        /**< Power management status change. Event data: tBTM_POWER_MANAGEMENT_STATUS_EVT */

    /* Security events */
    BTM_PIN_REQUEST_EVT,                    /**< PIN request (used only with legacy devices). Event data: tBTM_BD_NAME_AND_CLASS */
    BTM_USER_CONFIRMATION_REQUEST_EVT,      /**< received USER_CONFIRMATION_REQUEST event. Respond using wiced_bt_dev_confirm_req_reply. Event data: tBTM_SP_CFM_REQ */
    BTM_PASSKEY_NOTIFICATION_EVT,           /**< received USER_PASSKEY_NOTIFY event. Event data: tBTM_SP_KEY_NOTIF */
    BTM_PASSKEY_REQUEST_EVT,                /**< received USER_PASSKEY_REQUEST event. Respond using wiced_bt_dev_pass_key_req_reply. Event data: tBTM_SP_KEY_REQ */
    BTM_KEYPRESS_NOTIFICATION_EVT,          /**< received KEYPRESS_NOTIFY event. Event data: tBTM_SP_KEYPRESS */
    BTM_PAIRING_IO_CAPABILITIES_BR_EDR_REQUEST_EVT, /**< Requesting IO capabilities for BR/EDR pairing. Event data: tBTM_IO_CAPS_REQ_BR_EDR */
    BTM_PAIRING_IO_CAPABILITIES_BR_EDR_RESPONSE_EVT,/**< Received IO capabilities response for BR/EDR pairing. Event data: tBTM_SP_IO_RSP */
    BTM_PAIRING_IO_CAPABILITIES_BLE_REQUEST_EVT,    /**< Requesting IO capabilities for BLE pairing. Event data: tBTM_IO_CAPS_REQ_BLE */
    BTM_PAIRING_COMPLETE_EVT,               /**< received SIMPLE_PAIRING_COMPLETE event. Event data: tBTM_PAIRING_COMPLETE_EVT */
    BTM_ENCRYPTION_STATUS_EVT,              /**< Encryption enable/disabled (result of calling wiced_bt_dev_set_encryption). Event data: tBTM_ENCRYPTION_STATUS_EVT */
    BTM_SECURITY_REQUEST_EVT,               /**< Security request (LE connections). Respond using wiced_bt_ble_security_grant. Event data: tBTM_SECURITY_REQUEST_EVT */
    BTM_SECURITY_FAILED_EVT,                /**< Security procedure/authentication failed. Event data: tBTM_SECURITY_FAILED_EVT */
    BTM_SECURITY_ABORTED_EVT,               /**< Security procedure aborted locally, or unexpected link drop. Event data: tBTM_BD_NAME_AND_CLASS */

    BTM_READ_LOCAL_OOB_DATA_COMPLETE_EVT,   /**< Result of reading local OOB data (wiced_bt_dev_read_local_oob_data). Event data: tBTM_READ_LOCAL_OOB_DATA_COMPLETE_EVT */
    BTM_REMOTE_OOB_DATA_REQUEST_EVT,        /**< OOB data from remote device (reply using wiced_bt_dev_remote_oob_data_reply). Event data: tBTM_REMOTE_OOB_DATA_REQUEST_EVT */

    BTM_PAIRED_DEVICE_LINK_KEYS_UPDATE_EVT, /**< Valid when (BTM_INTERNAL_LINKKEY_STORAGE_INCLUDED == FALSE) Updated remote device link keys (store device_link_keys to  NV memory). Event data: tBTM_PAIRED_DEVICE_LINK_KEYS */
    BTM_PAIRED_DEVICE_LINK_KEYS_REQUEST_EVT,/**< Valid when (BTM_INTERNAL_LINKKEY_STORAGE_INCLUDED == FALSE) Request for stored remote device link keys (restore device_link_keys from NV memory). If successful, return BTM_SUCCESS. Event data: tBTM_PAIRED_DEVICE_LINK_KEYS */

    BTM_LOCAL_IDENTITY_KEYS_UPDATE_EVT,     /**< Valid when (BTM_INTERNAL_LINKKEY_STORAGE_INCLUDED == FALSE) Update local identity key (stored local_identity_keys NV memory). Event data: tBTM_LOCAL_IDENTITY_KEYS */
    BTM_LOCAL_IDENTITY_KEYS_REQUEST_EVT,    /**< Valid when (BTM_INTERNAL_LINKKEY_STORAGE_INCLUDED == FALSE) Request local identity key (get local_identity_keys from NV memory). If successful, return BTM_SUCCESS. Event data: tBTM_LOCAL_IDENTITY_KEYS */


    BTM_BLE_SCAN_STATE_CHANGED_EVT,         /**< BLE scan state change. Event data: wiced_bt_ble_scan_type_t */
    BTM_BLE_ADVERT_STATE_CHANGED_EVT,       /**< BLE advertisement state change. Event data: wiced_bt_ble_advert_mode_t */

    /* SMP security events */
    BTM_SMP_REMOTE_OOB_DATA_REQUEST_EVT,        /**< SMP remote oob data request. Reply using wiced_bt_smp_oob_data_reply. Event data: tBTM_SMP_REMOTE_OOB_REQ  */
    BTM_SMP_SC_REMOTE_OOB_DATA_REQUEST_EVT,     /**< SC remote oob data request. Reply using SMP_SCOobDataReply. Event data: tBTM_SMP_SC_REMOTE_OOB_REQ */
    BTM_SMP_SC_LOCAL_OOB_DATA_NOTIFICATION_EVT, /**< Result from reading local SMP OOB data (SMP_CrLocScOobData). Event data: */

    /* SCO / eSCO events */
    BTM_SCO_CONNECTED_EVT,                  /**< SCO connected. Event data: tBTM_SCO_CONNECTED */
    BTM_SCO_DISCONNECTED_EVT,               /**< SCO disconnected. Event data: tBTM_SCO_DISCONNECTED */
    BTM_SCO_CONNECTION_REQUEST_EVT,         /**< SCO connection request. Event data: tBTM_SCO_CONN_REQ_EVT_DATA */
    BTM_SCO_CONNECTION_CHANGE_EVT,           /**< SCO connection request. Event data: tBTM_SCO_CONN_CHG_EVT_DATA */
    BTM_BLE_CONNECTION_PARAM_UPDATE,        /**< BLE connection parameter update. Event data: tBTM_BLE_CONN_PARAM_UPDATE */
    BTM_BLE_PHY_UPDATE_EVT,                  /**< BLE Physical link update. Event data: tBTM_BLE_PHY_UPDATE */
    BTM_LPM_STATE_LOW_POWER,                /**< BT device wake has been deasserted */
#if SMP_CONFORMANCE_TESTING == TRUE
    BTM_SMP_SC_PEER_INFO_EVT,                /** The Secure Connections support information of the peer device */
#endif
};
#endif
typedef UINT8 tBTM_EVENT;
/**@} btm_call_back_events */

/****************************************************************************/
/**
 * @addtogroup  btm_call_back_event_data_structs     Event Data Structures
 * @ingroup     btm_call_back
 * @{
 */

/** Device enabled (used by BTM_ENABLED_EVT) */
typedef struct
{
    tBTM_STATUS             status;                     /**< Status */
} tBTM_ENABLED_EVT;

/** Device disabled (used by BTM_DISABLED_EVT) */
typedef struct
{
    UINT8          reason;                     /** < Reason for BTM Disable */
} tBTM_DISABLED_EVT;

/** Remote device information (used by BTM_PIN_REQUEST_EVT, BTM_SECURITY_ABORTED_EVT) */
typedef struct
{
    UINT8                   *bd_addr;                   /**< BD Address of remote */
    UINT8                   *dev_class;                  /**< peer class of device   */
    UINT8                   *bd_name;                    /**< BD Name of remote */
} tBTM_BD_NAME_AND_CLASS;

/** Change in power management status  (used by BTM_POWER_MANAGEMENT_STATUS_EVT notication) */
typedef struct
{
    BD_ADDR_PTR                         bd_addr;        /**< BD Address of remote */
    tBTM_PM_STATUS                      status;         /**< PM status */
    UINT16                              value;          /**< Additional mode data */
    UINT8                               hci_status;     /**< HCI status */
} tBTM_POWER_MANAGEMENT_STATUS_EVT;

/** ACL connection status change (used by BTM_CONNECTION_STATUS_EVT) */
typedef struct
{
    UINT8                   *bd_addr;                   /**< BD Address of remote */
    UINT8                   *dev_class;                 /**< peer class of device   */
    UINT8                   *bd_name;                   /**< BD Name of remote */
    UINT8                   *features;
    BOOLEAN                 is_connected;               /**< TRUE if connected */
    UINT16                  handle;                     /**< HCI handle */
    tBT_TRANSPORT           transport;                  /**< BT_TRANSPORT_BR_EDR or BT_TRANSPORT_LE */
} tBTM_CONNECTION_STATUS_EVT;

/** SCO connection status change (used by BTM_SCO_CONNECTION_STATUS_EVT) */
typedef struct
{
    UINT16                  handle;                     /**< SCO handle */
    BOOLEAN                 is_connected;               /**< TRUE if connected */
} tBTM_SCO_CONNECTION_STATUS_EVT;

/** Encryption status change (used by BTM_ENCRYPTION_STATUS_EVT) */
typedef struct
{
    UINT8                   *bd_addr;                   /**< BD Address of remote */
    tBT_TRANSPORT           transport;                  /**< BT_TRANSPORT_BR_EDR or BT_TRANSPORT_LE */
    void                    *p_ref_data;                /**< Optional data passed in by wiced_bt_dev_set_encryption */
    tBTM_STATUS             result;                     /**< Result of the operation */
} tBTM_ENCRYPTION_STATUS_EVT;

/** Local OOB data BTM_READ_LOCAL_OOB_DATA_COMPLETE_EVT */
typedef struct
{
    tBTM_STATUS             status;                     /**< Status */
    BOOLEAN                 is_extended_oob_data;       /**< TRUE if extended OOB data */

    BT_OCTET16              c_192;                      /**< Simple Pairing Hash C derived from the P-192 public key */
    BT_OCTET16              r_192;                      /**< Simple Pairing Randomnizer R associated with the P-192 public key */
    BT_OCTET16              c_256;                      /**< Simple Pairing Hash C derived from the P-256 public key (valid only if is_extended_oob_data=TRUE) */
    BT_OCTET16              r_256;                      /**< Simple Pairing Randomnizer R associated with the P-256 public key (valid only if is_extended_oob_data=TRUE) */
} tBTM_READ_LOCAL_OOB_DATA_COMPLETE_EVT;

/** BTM_REMOTE_OOB_DATA_REQUEST_EVT */
typedef struct
{
    BD_ADDR                 bd_addr;                    /**< BD Address of remote */
    BOOLEAN                 extended_oob_data;               /**< TRUE if requesting extended OOB (P-256) */
} tBTM_REMOTE_OOB_DATA_REQUEST_EVT;

/** BR/EDR Pairing IO Capabilities */
typedef struct
{
    BD_ADDR                     bd_addr;                /**< [in] BD Address of remote   */
    tBTM_IO_CAP                 local_io_cap;           /**< local IO capabilities   */
    tBTM_OOB_DATA               oob_data;               /**< OOB data present at peer device for the local device   */
    tBTM_AUTH_REQ               auth_req;               /**< Authentication required for peer device                */
    BOOLEAN                     is_orig;                /**< TRUE, if local device initiated the pairing process    */
} tBTM_IO_CAPS_REQ_BR_EDR;

/** BLE Pairing IO Capabilities */
typedef struct
{
    BD_ADDR                     bd_addr;                /**< [in] BD Address of remote   */
    tBTM_IO_CAP                 local_io_cap;           /**< local IO capabilities   */
    UINT8                       oob_data;               /**< OOB data present (locally) for the peer device                             */
    tBTM_LE_AUTH_REQ            auth_req;               /**< Authentication request (for local device) contain bonding and MITM info    */
    UINT8                       max_key_size;           /**< Max encryption key size                                                    */
    tBTM_LE_KEY_TYPE            init_keys;              /**< Keys to be distributed, bit mask                                           */
    tBTM_LE_KEY_TYPE            resp_keys;              /**< keys to be distributed, bit mask                                           */
} tBTM_IO_CAPS_REQ_BLE;

/** Paired device link key notification (used by BTM_PAIRED_DEVICE_LINK_KEYS_UPDATE_EVT notication) */
typedef struct
{
    BD_ADDR                         bd_addr;                                    /**< BD Address of remote */
    tBTM_SEC_KEYS                   key_data;
} tBTM_PAIRED_DEVICE_LINK_KEYS;

/** BTM_SCO_DATA_EVT */
typedef struct
{
    UINT8   handle;                         /**< SCO handle */
    UINT8   *p_data;                        /**< SCO data */
} tBTM_SCO_DATA_EVT;

/** avertisement mode (used when calling BTM_BleStartAdvertisements()) */
#ifndef BTM_BLE_ADVERT_MODE
#define BTM_BLE_ADVERT_MODE
enum
{
    BTM_BLE_ADVERT_OFF,                 /**< Stop advertising */
    BTM_BLE_ADVERT_DIRECTED_HIGH,       /**< Directed avertisement (high duty cycle) */
    BTM_BLE_ADVERT_DIRECTED_LOW,        /**< Directed avertisement (low duty cycle) */
    BTM_BLE_ADVERT_UNDIRECTED_HIGH,     /**< Undirected avertisement (high duty cycle) */
    BTM_BLE_ADVERT_UNDIRECTED_LOW,      /**< Undirected avertisement (low duty cycle) */
    BTM_BLE_ADVERT_NONCONN_HIGH,        /**< Non-connectable avertisement (high duty cycle) */
    BTM_BLE_ADVERT_NONCONN_LOW,         /**< Non-connectable avertisement (low duty cycle) */
    BTM_BLE_ADVERT_DISCOVERABLE_HIGH,   /**< discoverable avertisement (high duty cycle) */
    BTM_BLE_ADVERT_DISCOVERABLE_LOW     /**< discoverable avertisement (low duty cycle) */
};
#endif
typedef UINT8 tBTM_BLE_AVERT_MODE;       /**< Avertisement mode */

/** scan mode used in initiating */
#ifndef BTM_BLE_CONN_MODE
#define BTM_BLE_CONN_MODE
enum
{
    BLE_CONN_MODE_OFF,              /**< Stop initiating */
    BLE_CONN_MODE_LOW_DUTY,         /**< slow connection scan parameter */
    BLE_CONN_MODE_HIGH_DUTY         /**< fast connection scan parameter */
};
#endif
typedef UINT8 tBLE_CONN_MODE;       /**< Conn mode */

/** Structure definitions for Bluetooth Management (wiced_bt_management_cback_t) event notifications */
typedef union
{
    /* Bluetooth status event data types*/
    tBTM_ENABLED_EVT                    enabled;                            /**< Data for BTM_ENABLED_EVT */
    tBTM_DISABLED_EVT                   disabled;                           /**< Data for BTM_DISABLED_EVT */
    tBTM_CONNECTION_STATUS_EVT          connection_status;                  /**< Data for BTM_CONNECTION_STATUS_EVT */
    tBTM_POWER_MANAGEMENT_STATUS_EVT    power_mgmt_notification;            /**< Data for BTM_POWER_MANAGEMENT_STATUS_EVT */

    /* Security event data types */
    tBTM_BD_NAME_AND_CLASS              pin_request;                        /**< Data for BTM_PIN_REQUEST_EVT */
    tBTM_SP_CFM_REQ                     user_confirmation_request;          /**< Data for BTM_USER_CONFIRMATION_REQUEST_EVT */
    tBTM_SP_KEY_NOTIF                   user_passkey_notification;          /**< Data for BTM_USER_PASSKEY_NOTIFICATION_EVT */
    tBTM_SP_KEY_REQ                     user_passkey_request;               /**< Data for BTM_USER_PASSKEY_REQUEST_EVT */
    tBTM_SP_KEYPRESS                    user_keypress_notification;         /**< Data for BTM_USER_KEYPRESS_NOTIFICATION_EVT */
    tBTM_IO_CAPS_REQ_BR_EDR             pairing_io_capabilities_bredr_request;  /**< Data for BTM_PAIRING_IO_CAPABILITIES_BR_EDR_REQUEST_EVT */
    tBTM_SP_IO_RSP                      pairing_io_capabilities_bredr_response; /**< Data for BTM_PAIRING_IO_CAPABILITIES_BR_EDR_RESPONSE_EVT */
    tBTM_IO_CAPS_REQ_BLE                pairing_io_capabilities_ble_request;    /**< Data for BTM_PAIRING_IO_CAPABILITIES_BLE_REQUEST_EVT */
    tBTM_PAIRING_COMPLETE_EVT           pairing_complete;                   /**< Data for BTM_PAIRING_COMPLETE_EVT */
    tBTM_ENCRYPTION_STATUS_EVT          encryption_status;                  /**< Data for BTM_ENCRYPTION_STATUS_EVT */
    tBTM_SECURITY_REQUEST_EVT           security_request;                   /**< Data for BTM_SECURITY_REQUEST_EVT */
    tBTM_SECURITY_FAILED_EVT            security_failed;                    /**< Data for BTM_SECURITY_FAILED_EVT */
    tBTM_BD_NAME_AND_CLASS              security_aborted;                   /**< Data for BTM_SECURITY_ABORTED_EVT */

    tBTM_READ_LOCAL_OOB_DATA_COMPLETE_EVT read_local_oob_data_complete;     /**< Data for BTM_READ_LOCAL_OOB_DATA_COMPLETE_EVT */
    tBTM_REMOTE_OOB_DATA_REQUEST_EVT    remote_oob_data_request;            /**< Data for BTM_REMOTE_OOB_DATA_REQUEST_EVT */

    tBTM_PAIRED_DEVICE_LINK_KEYS        paired_device_link_keys_update;     /**< Data for BTM_PAIRED_DEVICE_LINK_KEYS_UPDATE_EVT */
    tBTM_PAIRED_DEVICE_LINK_KEYS        paired_device_link_keys_request;    /**< Data for BTM_PAIRED_DEVICE_LINK_KEYS_REQUEST_EVT */
    tBTM_LOCAL_IDENTITY_KEYS            local_identity_keys_update;         /**< Data for BTM_LOCAL_IDENTITY_KEYS_UPDATE_EVT */
    tBTM_LOCAL_IDENTITY_KEYS            local_identity_keys_request;        /**< Data for BTM_LOCAL_IDENTITY_KEYS_REQUEST_EVT */

    tBTM_BLE_SCAN_TYPE                  ble_scan_state_changed;             /**< Data for BTM_BLE_SCAN_STATE_CHANGED_EVT */
    tBTM_BLE_AVERT_MODE                 ble_advert_state_changed;           /**< Data for BTM_BLE_ADVERT_STATE_CHANGED_EVT */
    tBTM_OOB_DATA                       oob_data;                           /**< OOB data present at peer device for the local device   */

    tBTM_SMP_REMOTE_OOB_REQ             smp_remote_oob_data_request;        /**< Data for BTM_SMP_REMOTE_OOB_DATA_REQUEST_EVT */
    tBTM_SMP_SC_REMOTE_OOB_REQ          smp_sc_remote_oob_data_request;     /**< Data for BTM_SMP_SC_REMOTE_OOB_DATA_REQUEST_EVT */
    tBTM_SMP_SC_LOCAL_OOB_DATA          *p_smp_sc_local_oob_data;           /**< Data for BTM_SMP_SC_LOCAL_OOB_DATA_NOTIFICATION_EVT */

    tBTM_SCO_CONNECTED                  sco_connected;                      /**< Data for BTM_SCO_CONNECTED_EVT */
    tBTM_SCO_DISCONNECTED               sco_disconnected;                   /**< Data for BTM_SCO_DISCONNECTED_EVT */
    tBTM_SCO_CONN_REQ_EVT_DATA          sco_conn_req;                       /**< Data for BTM_SCO_CONNECTION_REQUEST_EVT */
    tBTM_SCO_CONN_CHG_EVT_DATA          sco_conn_chg;                       /**< Data for BTM_SCO_CONNECTION_CHANGE_EVT */
    tBTM_BLE_CONN_PARAM_UPDATE          ble_connection_param_update;        /**< Data for BTM_BLE_CONNECTION_PARAM_UPDATE */
    tBTM_BLE_PHY_UPDATE                 ble_phy_update_event;               /**< Data for BTM_BLE_PHY_UPDATE_EVT */

#if SMP_CONFORMANCE_TESTING == TRUE
    tBTM_SMP_SC_PEER_INFO               smp_sc_peer_info;                   /* Data for BTM_SMP_SC_PEER_INFO_EVT */
#endif
} tBTM_EVENT_DATA;
/**@} btm_call_back_event_data_structs */

/****************************************************************************/
/**
 * @addtogroup  btm_call_back_fcn    Callback Functions
 * @ingroup     btm_call_back
 * @{
 */

/**
 * Bluetooth Management callback
 *
 * Callback for Bluetooth Management event notifications.
 * Registered using wiced_bt_stack_init()
 *
 * @param event             : Event ID
 * @param p_event_data      : Event data
 *
 * @return Status of event handling
 */
typedef tBTM_STATUS(tBTM_EVENT_CBACK)(tBTM_EVENT event, tBTM_EVENT_DATA *p_event_data);



/*****************************************************************************
**  EXTERNAL FUNCTION DECLARATIONS
*****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
**  DEVICE CONTROL and COMMON FUNCTIONS
*****************************************************************************/

/*******************************************************************************
**
** Function         BTM_SetAfhChannels
**
** Description      This function is called to disable channels
**
** Returns          status
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetAfhChannels(UINT8 first, UINT8 last);

/*******************************************************************************
**
** Function         BTM_SetAfhChannelAssessment
**
** Description      This function is called to set the channel assessment mode on or off
**
** Returns          status
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetAfhChannelAssessment(BOOLEAN enable_or_disable);

/*******************************************************************************
**
** Function         BTM_DeviceReset
**
** Description      This function is called to reset the controller.The Callback function
**                  if provided is called when startup of the device has
**                  completed.
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_DeviceReset(tBTM_CMPL_CB *p_cb);


/*******************************************************************************
**
** Function         BTM_IsDeviceUp
**
** Description      This function is called to check if the device is up.
**
** Returns          TRUE if device is up, else FALSE
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_IsDeviceUp(void);


/*******************************************************************************
**
** Function         BTM_SetLocalDeviceName
**
** Description      This function is called to set the local device name.
**
** Returns          BTM_CMD_STARTED if successful, otherwise an error
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetLocalDeviceName(char *p_name);

/*******************************************************************************
**
** Function         BTM_SetDeviceClass
**
** Description      This function is called to set the local device class
**
** Returns          BTM_SUCCESS if successful, otherwise an error
**
*******************************************************************************/
BTM_API extern tBTM_STATUS  BTM_SetDeviceClass(DEV_CLASS dev_class);


/*******************************************************************************
**
** Function         BTM_ReadLocalDeviceName
**
** Description      This function is called to read the local device name.
**
** Returns          status of the operation
**                  If success, BTM_SUCCESS is returned and p_name points stored
**                              local device name
**                  If BTM doesn't store local device name, BTM_NO_RESOURCES is
**                              is returned and p_name is set to NULL
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_ReadLocalDeviceName(char **p_name);

/*******************************************************************************
**
** Function         BTM_ReadLocalDeviceNameFromController
**
** Description      Get local device name from controller. Do not use cached
**                  name (used to get chip-id prior to btm reset complete).
**
** Returns          BTM_CMD_STARTED if successful, otherwise an error
**
*******************************************************************************/
BTM_API extern
tBTM_STATUS BTM_ReadLocalDeviceNameFromController(tBTM_CMPL_CB *p_rln_cmpl_cback);

/*******************************************************************************
**
** Function         BTM_ReadLocalVersion
**
** Description      This function is called to read the local device version
**
** Returns          BTM_SUCCESS if successful, otherwise an error
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_ReadLocalVersion(tBTM_VERSION_INFO *p_vers);


/*******************************************************************************
**
** Function         BTM_ReadLocalDeviceAddr
**
** Description      This function is called to read the local device address
**
** Returns          BTM_SUCCESS
**                  Callback returns the local device address
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_ReadLocalDeviceAddr(tBTM_CMPL_CB *p_cb);


/*******************************************************************************
**
** Function         BTM_GetLocalDeviceAddr
**
** Description      This function is called to read the local device address
**
** Returns          void
**                  the local device address is copied into bd_addr
**
*******************************************************************************/
BTM_API extern void BTM_GetLocalDeviceAddr(BD_ADDR bd_addr);


/*******************************************************************************
**
** Function         BTM_SetLocalDeviceAddr
**
** Description      This function is called to set the local device address
**
** Returns          BTM_CMD_STARTED: device address set procedure started
**                  BTM_NO_RESOURCES: command start failure due to no resources
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetLocalDeviceAddr(BD_ADDR bd_addr, tBTM_CMPL_CB *p_cb);

/*******************************************************************************
**
** Function         BTM_SetTXPower
**
** Description      This function is called to set the BT tx power.
**                      To set the advertisement TX power, the first parameter should be set to NULL.
**
** Returns          BTM_SUCCESS if successful, otherwise an error
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetTXPower(BD_ADDR bd_addr, INT8 power);

/*******************************************************************************
**
** Function         BTM_ReadDeviceClass
**
** Description      This function is called to read the local device class
**
** Returns          pointer to the device class
**
*******************************************************************************/
BTM_API extern UINT8 *BTM_ReadDeviceClass(void);


/*******************************************************************************
**
** Function         BTM_ReadLocalFeatures
**
** Description      This function is called to read the local features
**
** Returns          pointer to the local features string
**
*******************************************************************************/
BTM_API extern UINT8 *BTM_ReadLocalFeatures(void);

/*******************************************************************************
**
** Function         BTM_ReadLocalExtendedFeatures
**
** Description      This function is called to read the local extended features
**
** Returns          pointer to the local extended features mask or NULL if bad
**                  page
**
*******************************************************************************/
BTM_API extern UINT8 *BTM_ReadLocalExtendedFeatures(UINT8 page_number);

/*******************************************************************************
**
** Function         BTM_ReadLocalSupportedCommands
**
** Description      This function is called to read the supported commands
**                  of the local controller.
**
** Returns          pointer to the array of locally supported commands
**
*******************************************************************************/
BTM_API extern UINT8 *BTM_ReadLocalSupportedCommands(void);

/*******************************************************************************
**
** Function         BTM_ReadBrcmFeatures
**
** Description      This function is called to read the Broadcom specific features
**
** Returns          pointer to the Broadcom features string
**
*******************************************************************************/
BTM_API extern UINT8 *BTM_ReadBrcmFeatures(void);

/*******************************************************************************
**
** Function         BTM_RegisterForDeviceStatusNotif
**
** Description      This function is called to register for device status
**                  change notifications.
**
** Returns          pointer to previous caller's callback function or NULL if first
**                  registration.
**
*******************************************************************************/
BTM_API extern
tBTM_DEV_STATUS_CB *BTM_RegisterForDeviceStatusNotif(tBTM_DEV_STATUS_CB *p_cb);


/*******************************************************************************
**
** Function         BTM_RegisterForVSEvents
**
** Description      This function is called to register/deregister for vendor
**                  specific HCI events.
**
**                  If is_register=TRUE, then the function will be registered;
**                  if is_register=FALSE, then the function will be deregistered.
**
** Returns          BTM_SUCCESS if successful,
**                  BTM_BUSY if maximum number of callbacks have already been
**                           registered.
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_RegisterForVSEvents(tBTM_VS_EVT_CB *p_cb,
        BOOLEAN is_register);

/*******************************************************************************
**
** Function         BTM_ContinueReset
**
** Description      Instructs stack to continue its stack initialization after
**                  an application has completed any vender specific commands
**                  sent to the controller.
**
**                  Note: This function is only called if an application
**                      initialization function has been inserted in the reset
**                      sequence.  (BTM_APP_DEV_INIT is defined with a function).
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_ContinueReset(void);

/*******************************************************************************
**
** Function         BTM_VendorSpecificCommand
**
** Description      Send a vendor specific HCI command to the controller.
**
** Returns
**      BTM_SUCCESS         Command sent. Does not expect command complete
**                              event. (command cmpl callback param is NULL)
**      BTM_CMD_STARTED     Command sent. Waiting for command cmpl event.
**      BTM_BUSY            Command not sent. Waiting for cmd cmpl event for
**                              prior command.
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_VendorSpecificCommand(UINT16 opcode,
        UINT8 param_len,
        UINT8 *p_param_buf,
        tBTM_VSC_CMPL_CB *p_cb);

/*******************************************************************************
**
** Function         BTM_AllocateSCN
**
** Description      Look through the Server Channel Numbers for a free one to be
**                  used with an RFCOMM connection.
**
** Returns          Allocated SCN number or 0 if none.
**
*******************************************************************************/
BTM_API extern UINT8 BTM_AllocateSCN(void);

/*******************************************************************************
**
** Function         BTM_FreeSCN
**
** Description      Free the specified SCN.
**
** Returns          TRUE if successful, FALSE if SCN is not in use or invalid
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_FreeSCN(UINT8 scn);


/*******************************************************************************
**
** Function         BTM_SetTraceLevel
**
** Description      This function sets the trace level for BTM.  If called with
**                  a value of 0xFF, it simply returns the current trace level.
**
** Returns          The new or current trace level
**
*******************************************************************************/
BTM_API extern UINT8 BTM_SetTraceLevel(UINT8 new_level);

/*******************************************************************************
**
** Function         BTM_WritePageTimeout
**
** Description      Send HCI Wite Page Timeout.
**
** Returns
**      BTM_SUCCESS         Command sent.
**      BTM_NO_RESOURCES    If out of resources to send the command.
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_WritePageTimeout(UINT16 timeout);

/*******************************************************************************
**
** Function         BTM_WriteVoiceSettings
**
** Description      Send HCI Write Voice Settings command.
**                  See hcidefs.h for settings bitmask values.
**
** Returns
**      BTM_SUCCESS         Command sent.
**      BTM_NO_RESOURCES     If out of resources to send the command.
**
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_WriteVoiceSettings(UINT16 settings);

/*******************************************************************************
**
** Function         BTM_EnableTestMode
**
** Description      Send HCI the enable device under test command.
**
**                  Note: Controller can only be taken out of this mode by
**                      resetting the controller.
**
** Returns
**      BTM_SUCCESS         Command sent.
**      BTM_NO_RESOURCES    If out of resources to send the command.
**
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_EnableTestMode(void);


/*****************************************************************************
**  DEVICE DISCOVERY FUNCTIONS - Inquiry, Remote Name, Discovery, Class of Device
*****************************************************************************/
/*******************************************************************************
**
** Function         BTM_SetScanConfig
**
** Description      This function is called to configure the page and inquiry scan
**                  activities without having to changing the visiblity.
**
**                  Note: BTM_SCAN_PARAM_IGNORE is passed to the function to ignore
**                        the setting of inquiry or page scan activity.
**
** Returns          BTM_SUCCESS if successful
**                  BTM_NO_RESOURCES if couldn't get a memory pool buffer
**                  BTM_ILLEGAL_VALUE if a bad parameter was detected
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetScanConfig(UINT16 inquiry_scan_interval,
        UINT16 inquiry_scan_window,
        UINT16 page_scan_interval,
        UINT16 page_scan_window);

/*******************************************************************************
**
** Function         BTM_SetDiscoverability
**
** Description      This function is called to set the device into or out of
**                  discoverable mode. Discoverable mode means inquiry
**                  scans are enabled.  If a value of '0' is entered for window or
**                  interval, the default values are used.
**
** Returns          BTM_SUCCESS if successful
**                  BTM_BUSY if a setting of the filter is already in progress
**                  BTM_NO_RESOURCES if couldn't get a memory pool buffer
**                  BTM_ILLEGAL_VALUE if a bad parameter was detected
**                  BTM_WRONG_MODE if the device is not up.
**
*******************************************************************************/
BTM_API extern tBTM_STATUS  BTM_SetDiscoverability(UINT8 inq_mode, UINT16 window,
        UINT16 interval);


/*******************************************************************************
**
** Function         BTM_ReadDiscoverability
**
** Description      This function is called to read the current discoverability
**                  mode of the device.
**
** Output Params:   p_window - current inquiry scan duration
**                  p_interval - current inquiry scan interval
**
** Returns          BTM_NON_DISCOVERABLE, BTM_LIMITED_DISCOVERABLE, or
**                  BTM_GENERAL_DISCOVERABLE
**
*******************************************************************************/
BTM_API extern UINT16       BTM_ReadDiscoverability(UINT16 *p_window,
        UINT16 *p_interval);

#ifndef BTEWICED
/*******************************************************************************
**
** Function         BTM_SetPeriodicInquiryMode
**
** Description      This function is called to set the device periodic inquiry mode.
**                  If the duration is zero, the periodic inquiry mode is cancelled.
**
** Parameters:      p_inqparms - pointer to the inquiry information
**                      mode - GENERAL or LIMITED inquiry
**                      duration - length in 1.28 sec intervals (If '0', the inquiry is CANCELLED)
**                      max_resps - maximum amount of devices to search for before ending the inquiry
**                      filter_cond_type - BTM_CLR_INQUIRY_FILTER, BTM_FILTER_COND_DEVICE_CLASS, or
**                                         BTM_FILTER_COND_BD_ADDR
**                      filter_cond - value for the filter (based on filter_cond_type)
**
**                  max_delay - maximum amount of time between successive inquiries
**                  min_delay - minimum amount of time between successive inquiries
**                  p_results_cb - callback returning pointer to results (tBTM_INQ_RESULTS)
**
** Returns          BTM_CMD_STARTED if successfully started
**                  BTM_ILLEGAL_VALUE if a bad parameter is detected
**                  BTM_NO_RESOURCES if could not allocate a message buffer
**                  BTM_SUCCESS - if cancelling the periodic inquiry
**                  BTM_BUSY - if an inquiry is already active
**                  BTM_WRONG_MODE if the device is not up.
**
*******************************************************************************/
BTM_API extern tBTM_STATUS  BTM_SetPeriodicInquiryMode(tBTM_INQ_PARMS *p_inqparms,
        UINT16 max_delay,
        UINT16 min_delay,
        tBTM_INQ_RESULTS_CB *p_results_cb);
#endif

/*******************************************************************************
**
** Function         BTM_StartInquiry
**
** Description      This function is called to start an inquiry.
**
** Parameters:      p_inqparms - pointer to the inquiry information
**                      mode - GENERAL or LIMITED inquiry
**                      duration - length in 1.28 sec intervals (If '0', the inquiry is CANCELLED)
**                      max_resps - maximum amount of devices to search for before ending the inquiry
**                      filter_cond_type - BTM_CLR_INQUIRY_FILTER, BTM_FILTER_COND_DEVICE_CLASS, or
**                                         BTM_FILTER_COND_BD_ADDR
**                      filter_cond - value for the filter (based on filter_cond_type)
**
**                  p_results_cb   - Pointer to the callback routine which gets called
**                                upon receipt of an inquiry result. If this field is
**                                NULL, the application is not notified.
**
** Returns          tBTM_STATUS
**                  BTM_CMD_STARTED if successfully initiated
**                  BTM_BUSY if already in progress
**                  BTM_ILLEGAL_VALUE if parameter(s) are out of range
**                  BTM_NO_RESOURCES if could not allocate resources to start the command
**                  BTM_WRONG_MODE if the device is not up.
**
*******************************************************************************/
BTM_API extern tBTM_STATUS  BTM_StartInquiry(tBTM_INQ_PARMS *p_inqparms, tBTM_INQ_RESULTS_CB *p_results_cb);


/*******************************************************************************
**
** Function         BTM_IsInquiryActive
**
** Description      This function returns a bit mask of the current inquiry state
**
** Returns          BTM_INQUIRY_INACTIVE if inactive (0)
**                  BTM_LIMITED_INQUIRY_ACTIVE if a limted inquiry is active
**                  BTM_GENERAL_INQUIRY_ACTIVE if a general inquiry is active
**                  BTM_PERIODIC_INQUIRY_ACTIVE if a periodic inquiry is active
**
*******************************************************************************/
BTM_API extern UINT16 BTM_IsInquiryActive(void);


/*******************************************************************************
**
** Function         BTM_CancelInquiry
**
** Description      This function cancels an inquiry if active
**
** Returns          BTM_SUCCESS if successful
**                  BTM_NO_RESOURCES if could not allocate a message buffer
**                  BTM_WRONG_MODE if the device is not up.
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_CancelInquiry(void);

/*******************************************************************************
 * Function         BTM_SetAdvancedConnectionParams
 *
 *                  Set advanced connection parameters (using remote clock offset, page scan mode
 *                  and other information obtained during inquiry scan in BTM_SCAN_RESULT_EVT)
 *
 *                  If not called, then default connection parameters will be used.
 *
 * @param[in]       p_inquiry_scan_result : Inquiry scan result (from BTM_SCAN_RESULT_EVT)
 *
 * @return          WICED_BT_SUCCESS : on success;
 *                  WICED_BT_FAILED  : if an error occurred
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetAdvancedConnectionParams(tBTM_INQ_RESULTS *p_inquiry_scan_result);


/*******************************************************************************
**
** Function         BTM_CancelPeriodicInquiry
**
** Description      This function cancels a periodic inquiry
**
** Returns
**                  BTM_NO_RESOURCES if could not allocate a message buffer
**                  BTM_SUCCESS - if cancelling the periodic inquiry
**                  BTM_WRONG_MODE if the device is not up.
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_CancelPeriodicInquiry(void);


/*******************************************************************************
**
** Function         BTM_SetInquiryFilterCallback
**
** Description      Host can register to be asked whenever an inquiry result
**                  is received.  If host does not like the device no name
**                  request is issued for the device
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_SetInquiryFilterCallback(tBTM_FILTER_CB *p_callback);


/*******************************************************************************
**
** Function         BTM_SetConnectability
**
** Description      This function is called to set the device into or out of
**                  connectable mode. Discoverable mode means page scans enabled.
**
** Returns          BTM_SUCCESS if successful
**                  BTM_ILLEGAL_VALUE if a bad parameter is detected
**                  BTM_NO_RESOURCES if could not allocate a message buffer
**                  BTM_WRONG_MODE if the device is not up.
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetConnectability(UINT8 page_mode, UINT16 window,
        UINT16 interval);


/*******************************************************************************
**
** Function         BTM_ReadConnectability
**
** Description      This function is called to read the current discoverability
**                  mode of the device.
** Output Params    p_window - current page scan duration
**                  p_interval - current time between page scans
**
** Returns          BTM_NON_CONNECTABLE or BTM_CONNECTABLE
**
*******************************************************************************/
BTM_API extern UINT16 BTM_ReadConnectability(UINT16 *p_window, UINT16 *p_interval);


/*******************************************************************************
**
** Function         BTM_SetInquiryMode
**
** Description      This function is called to set standard, with RSSI
**                  mode or extended of the inquiry for local device.
**
** Input Params:    BTM_INQ_RESULT_STANDARD, BTM_INQ_RESULT_WITH_RSSI or
**                  BTM_INQ_RESULT_EXTENDED
**
** Returns          BTM_SUCCESS if successful
**                  BTM_NO_RESOURCES if couldn't get a memory pool buffer
**                  BTM_ILLEGAL_VALUE if a bad parameter was detected
**                  BTM_WRONG_MODE if the device is not up.
**
*******************************************************************************/
BT_API extern tBTM_STATUS  BTM_SetInquiryMode(UINT8 mode);

/*******************************************************************************
**
** Function         BTM_SetInquiryScanType
**
** Description      This function is called to set the iquiry scan-type to
**                  standard or interlaced.
**
** Input Params:    BTM_SCAN_TYPE_STANDARD or BTM_SCAN_TYPE_INTERLACED
**
** Returns          BTM_SUCCESS if successful
**                  BTM_MODE_UNSUPPORTED if not a 1.2 device
**                  BTM_WRONG_MODE if the device is not up.
**
*******************************************************************************/
BT_API extern tBTM_STATUS BTM_SetInquiryScanType(UINT16 scan_type);

/*******************************************************************************
**
** Function         BTM_SetPageScanType
**
** Description      This function is called to set the page scan-type to
**                  standard or interlaced.
**
** Input Params:    BTM_SCAN_TYPE_STANDARD or BTM_SCAN_TYPE_INTERLACED
**
** Returns          BTM_SUCCESS if successful
**                  BTM_MODE_UNSUPPORTED if not a 1.2 device
**                  BTM_WRONG_MODE if the device is not up.
**
*******************************************************************************/
BT_API extern tBTM_STATUS BTM_SetPageScanType(UINT16 scan_type);

/*******************************************************************************
**
** Function         BTM_ReadRemoteDeviceName
**
** Description      This function initiates a remote device HCI command to the
**                  controller and calls the callback when the process has completed.
**
** Input Params:    remote_bda      - device address of name to retrieve
**                  p_cb            - callback function called when BTM_CMD_STARTED
**                                    is returned.
**                                    A pointer to tBTM_REMOTE_DEV_NAME is passed to the
**                                    callback.
**
** Returns
**                  BTM_CMD_STARTED is returned if the request was successfully sent
**                                  to HCI.
**                  BTM_BUSY if already in progress
**                  BTM_UNKNOWN_ADDR if device address is bad
**                  BTM_NO_RESOURCES if could not allocate resources to start the command
**                  BTM_WRONG_MODE if the device is not up.
**
*******************************************************************************/
BTM_API extern tBTM_STATUS  BTM_ReadRemoteDeviceName(BD_ADDR remote_bda,
        tBTM_CMPL_CB *p_cb);

/*******************************************************************************
**
** Function         BTM_CancelRemoteDeviceName
**
** Description      This function initiates the cancel request for the specified
**                  remote device.
**
** Input Params:    None
**
** Returns
**                  BTM_CMD_STARTED is returned if the request was successfully sent
**                                  to HCI.
**                  BTM_NO_RESOURCES if could not allocate resources to start the command
**                  BTM_WRONG_MODE if there is not an active remote name request.
**
*******************************************************************************/
BTM_API extern tBTM_STATUS  BTM_CancelRemoteDeviceName(void);

/*******************************************************************************
**
** Function         BTM_ReadRemoteVersion
**
** Description      This function is called to read a remote device's version
**
** Returns          BTM_SUCCESS if successful, otherwise an error
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_ReadRemoteVersion(BD_ADDR addr,
        UINT8 *lmp_version,
        UINT16 *manufacturer,
        UINT16 *lmp_sub_version);

/*******************************************************************************
**
** Function         BTM_ReadRemoteFeatures
**
** Description      This function is called to read a remote device's
**                  supported features mask (features mask located at page 0)
**
**                  Note: The size of device features mask page is
**                  BTM_FEATURE_BYTES_PER_PAGE bytes.
**
** Returns          pointer to the remote supported features mask
**
*******************************************************************************/
BTM_API extern UINT8 *BTM_ReadRemoteFeatures(BD_ADDR addr);

/*******************************************************************************
**
** Function         BTM_ReadRemoteExtendedFeatures
**
** Description      This function is called to read a specific extended features
**                  page of the remote device
**
**                  Note1: The size of device features mask page is
**                  BTM_FEATURE_BYTES_PER_PAGE bytes.
**                  Note2: The valid device features mask page number depends on
**                  the remote device capabilities. It is expected to be in the
**                  range [0 - BTM_EXT_FEATURES_PAGE_MAX].

** Returns          pointer to the remote extended features mask
**                  or NULL if page_number is not valid
**
*******************************************************************************/
BTM_API extern UINT8 *BTM_ReadRemoteExtendedFeatures(BD_ADDR addr, UINT8 page_number);

/*******************************************************************************
**
** Function         BTM_ReadNumberRemoteFeaturesPages
**
** Description      This function is called to retrieve the number of feature pages
**                  read from the remote device
**
** Returns          number of features pages read from the remote device
**
*******************************************************************************/
BTM_API extern UINT8 BTM_ReadNumberRemoteFeaturesPages(BD_ADDR addr);

/*******************************************************************************
**
** Function         BTM_ReadAllRemoteFeatures
**
** Description      This function is called to read all features of the remote device
**
** Returns          pointer to the byte[0] of the page[0] of the remote device
**                  feature mask.
**
** Note:            the function returns the pointer to the array of the size
**                  BTM_FEATURE_BYTES_PER_PAGE * (BTM_EXT_FEATURES_PAGE_MAX + 1).
**
*******************************************************************************/
BTM_API extern UINT8 *BTM_ReadAllRemoteFeatures(BD_ADDR addr);

/*******************************************************************************
**
** Function         BTM_InqFirstResult
**
** Description      This function looks through the inquiry database for the first
**                  used entrysince the LAST inquiry. This is used in conjunction
**                  with BTM_InqNext by applications as a way to walk through the
**                  inquiry results database.
**
** Returns          pointer to first in-use entry, or NULL if DB is empty
**
*******************************************************************************/
BTM_API extern tBTM_INQ_INFO *BTM_InqFirstResult(void);


/*******************************************************************************
**
** Function         BTM_InqNextResult
**
** Description      This function looks through the inquiry database for the next
**                  used entrysince the LAST inquiry. If the input parameter is NULL,
**                  the first entry is returned.
**
** Returns          pointer to next in-use entry, or NULL if no more found.
**
*******************************************************************************/
BTM_API extern tBTM_INQ_INFO *BTM_InqNextResult(tBTM_INQ_INFO *p_cur);


/*******************************************************************************
**
** Function         BTM_InqDbRead
**
** Description      This function looks through the inquiry database for a match
**                  based on Bluetooth Device Address. This is the application's
**                  interface to get the inquiry details of a specific BD address.
**
** Returns          pointer to entry, or NULL if not found
**
*******************************************************************************/
BTM_API extern tBTM_INQ_INFO *BTM_InqDbRead(BD_ADDR p_bda);


/*******************************************************************************
**
** Function         BTM_InqDbFirst
**
** Description      This function looks through the inquiry database for the first
**                  used entry, and returns that. This is used in conjunction with
**                  BTM_InqDbNext by applications as a way to walk through the
**                  inquiry database.
**
** Returns          pointer to first in-use entry, or NULL if DB is empty
**
*******************************************************************************/
BTM_API extern tBTM_INQ_INFO *BTM_InqDbFirst(void);


/*******************************************************************************
**
** Function         BTM_InqDbNext
**
** Description      This function looks through the inquiry database for the next
**                  used entry, and returns that.  If the input parameter is NULL,
**                  the first entry is returned.
**
** Returns          pointer to next in-use entry, or NULL if no more found.
**
*******************************************************************************/
BTM_API extern tBTM_INQ_INFO *BTM_InqDbNext(tBTM_INQ_INFO *p_cur);


/*******************************************************************************
**
** Function         BTM_ClearInqDb
**
** Description      This function is called to clear out a device or all devices
**                  from the inquiry database.
**
** Parameter        p_bda - (input) BD_ADDR ->  Address of device to clear
**                                              (NULL clears all entries)
**
** Returns          BTM_BUSY if an inquiry, get remote name, or event filter
**                          is active, otherwise BTM_SUCCESS
**
*******************************************************************************/
BTM_API extern tBTM_STATUS  BTM_ClearInqDb(BD_ADDR p_bda);


/*******************************************************************************
**
** Function         BTM_ReadNumInqDbEntries
**
** Returns          This function returns the number of entries in the inquiry database.
**
*******************************************************************************/
BTM_API extern UINT8 BTM_ReadNumInqDbEntries(void);

#ifndef BTEWICED
/*******************************************************************************
**
** Function         BTM_InquiryRegisterForChanges
**
** Description      This function is called to register a callback for when the
**                  inquiry database changes, i.e. new entry or entry deleted.
**
** Returns          BTM_SUCCESS if successful, otherwise error code
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_InquiryRegisterForChanges(tBTM_INQ_DB_CHANGE_CB *p_cb);
#endif  /* !BTEWICED */

/*******************************************************************************
**
** Function         BTM_ReadInquiryRspTxPower
**
** Description      This command will read the inquiry Transmit Power level used
**                  to transmit the FHS and EIR data packets.
**                  This can be used directly in the Tx Power Level EIR data type.
**
** Returns          BTM_SUCCESS if successful
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_ReadInquiryRspTxPower(tBTM_CMPL_CB *p_cb);

/*******************************************************************************
**
** Function         BTM_WriteInquiryTxPower
**
** Description      This command is used to write the inquiry transmit power level
**                  used to transmit the inquiry (ID) data packets. The Controller
**                  should use the supported TX power level closest to the Tx_Power
**                  parameter.
**
** Returns          BTM_SUCCESS if successful
**
*******************************************************************************/
BTM_API extern tBTM_STATUS  BTM_WriteInquiryTxPower(INT8 tx_power);

/*******************************************************************************
**
** Function         BTM_StartDiscovery
**
** Description      This function is called by an application (or profile)
**                  when it wants to trigger an service discovery using the
**                  BTM's discovery database.
**
** Returns          tBTM_STATUS
**                      BTM_CMD_STARTED if the discovery was initiated
**                      BTM_BUSY if one is already in progress
**                      BTM_UNKNOWN_ADDR if no addresses are in the INQ DB
**                      BTM_ERR_PROCESSING if err initiating the command
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_StartDiscovery(tBTM_CMPL_CB *p_cmpl_cb,
        BD_ADDR_PTR p_rem_addr);


/*******************************************************************************
**
** Function         BTM_FindAttribute
**
** Description      This function is called by an application (or profile)
**                  when it wants to see if an attribute exists in the BTM
**                  discovery database.
**
** Returns          Pointer to matching record, or NULL
**
*******************************************************************************/
BTM_API extern tSDP_DISC_REC *BTM_FindAttribute(UINT16 attr_id,
        tSDP_DISC_REC *p_start_rec);


/*******************************************************************************
**
** Function         BTM_FindService
**
** Description      This function is called by an application (or profile)
**                  when it wants to see if a service exists in the BTM
**                  discovery database.
**
** Returns          Pointer to matching record, or NULL
**
*******************************************************************************/
BTM_API extern tSDP_DISC_REC *BTM_FindService(UINT16 service_uuid,
        tSDP_DISC_REC *p_start_rec);


/*******************************************************************************
**
** Function         BTM_SetDiscoveryParams
**
** Description      This function is called to set the BTM default discovery parameters.
**                  These UUID and attribute filters are used during the call to
**                  BTM_StartDiscovery.
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_SetDiscoveryParams(UINT16 num_uuid, tSDP_UUID *p_uuid_list,
        UINT16 num_attr, UINT16 *p_attr_list);


/*****************************************************************************
**  ACL CHANNEL MANAGEMENT FUNCTIONS
*****************************************************************************/
/*******************************************************************************
**
** Function         BTM_SetLinkPolicy
**
** Description      Create and send HCI "Write Policy Set" command
**
** Returns          BTM_CMD_STARTED if successfully initiated, otherwise error
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetLinkPolicy(BD_ADDR remote_bda,
        UINT16 *settings);


/*******************************************************************************
**
** Function         BTM_ReadLinkPolicy
**
** Description      This function is called to read the link policy settings.
**                  The address of link policy results are returned in the callback.
**                  (tBTM_LNK_POLICY_RESULTS)
**
** Returns          BTM_CMD_STARTED if successfully initiated, otherwise error
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_ReadLinkPolicy(BD_ADDR remote_bda,
        tBTM_CMPL_CB *p_cb);

/*******************************************************************************
**
** Function         BTM_SetDefaultLinkPolicy
**
** Description      Set the default value for HCI "Write Policy Set" command
**                  to use when an ACL link is created.
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_SetDefaultLinkPolicy(UINT16 settings);

/*******************************************************************************
**
** Function         BTM_SetDefaultLinkSuperTout
**
** Description      Set the default value for HCI "Write Link Supervision Timeout"
**                  command to use when an ACL link is created.
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_SetDefaultLinkSuperTout(UINT16 timeout);

/*******************************************************************************
**
** Function         BTM_SetLinkSuperTout
**
** Description      Create and send HCI "Write Link Supervision Timeout" command
**
** Returns          BTM_CMD_STARTED if successfully initiated, otherwise error
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetLinkSuperTout(BD_ADDR remote_bda,
        UINT16 timeout);

/*******************************************************************************
**
** Function         BTM_GetLinkSuperTout
**
** Description      Read the link supervision timeout value of the connection
**
** Returns          status of the operation
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_GetLinkSuperTout(BD_ADDR remote_bda,
        UINT16 *p_timeout);

/*******************************************************************************
**
** Function         BTM_RegForLstoEvt
**
** Description      register for the HCI "Link Supervision Timeout Change" event
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_RegForLstoEvt(tBTM_LSTO_CBACK *p_cback);


/* These next APIs are available if the power manager is not compiled in */
#if BTM_PWR_MGR_INCLUDED == FALSE
/*******************************************************************************
**
** Function         BTM_SetHoldMode
**
** Description      This function is called to set a connection into hold mode.
**                  A check is made if the connection is in sniff or park mode,
**                  and if yes, the hold mode is ignored.
**
** Returns          BTM_CMD_STARTED if successfully initiated, otherwise error
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetHoldMode(BD_ADDR remote_bda, UINT16 min_interval,
        UINT16 max_interval);


/*******************************************************************************
**
** Function         BTM_SetSniffMode
**
** Description      This function is called to set a connection into sniff mode.
**                  A check is made if the connection is already in sniff or park
**                  mode, and if yes, the sniff mode is ignored.
**
** Returns          BTM_CMD_STARTED if successfully initiated, otherwise error
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetSniffMode(BD_ADDR remote_bda, UINT16 min_period,
        UINT16 max_period, UINT16 attempt,
        UINT16 timeout);


/*******************************************************************************
**
** Function         BTM_CancelSniffMode
**
** Description      This function is called to put a connection out of sniff mode.
**                  A check is made if the connection is already in sniff mode,
**                  and if not, the cancel sniff mode is ignored.
**
** Returns          BTM_CMD_STARTED if successfully initiated, otherwise error
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_CancelSniffMode(BD_ADDR remote_bda);

/*******************************************************************************
**
** Function         BTM_SetParkMode
**
** Description      This function is called to set a connection into park mode.
**                  A check is made if the connection is already in sniff or park
**                  mode, and if yes, the park mode is ignored.
**
** Returns          BTM_CMD_STARTED if successfully initiated, otherwise error
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetParkMode(BD_ADDR remote_bda,
        UINT16 beacon_min_period,
        UINT16 beacon_max_period);


/*******************************************************************************
**
** Function         BTM_CancelParkMode
**
** Description      This function is called to put a connection out of park mode.
**                  A check is made if the connection is already in park mode,
**                  and if not, the cancel sniff mode is ignored.
**
** Returns          BTM_CMD_STARTED if successfully initiated, otherwise error
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_CancelParkMode(BD_ADDR remote_bda);


/*******************************************************************************
**
** Function         BTM_ReadAclMode
**
** Description      This returns the current mode for a specific
**                  ACL connection.
**
** Input Param      remote_bda - device address of desired ACL connection
**
** Output Param     p_mode - address where the current mode is copied into.
**                          BTM_ACL_MODE_NORMAL
**                          BTM_ACL_MODE_HOLD
**                          BTM_ACL_MODE_SNIFF
**                          BTM_ACL_MODE_PARK
**                          (valid only if return code is BTM_SUCCESS)
**
** Returns          BTM_SUCCESS if successful,
**                  BTM_UNKNOWN_ADDR if bd addr is not active or bad
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_ReadAclMode(BD_ADDR remote_bda, UINT8 *p_mode);

#endif /* if BTM_PWR_MGR_INCLUDED == FALSE */


/*******************************************************************************
**
** Function         BTM_SetPacketTypes
**
** Description      This function is set the packet types used for a specific
**                  ACL connection,
**
** Returns          BTM_CMD_STARTED if successfully initiated, otherwise error
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetPacketTypes(BD_ADDR remote_bda, UINT16 pkt_types);


/*******************************************************************************
**
** Function         BTM_ReadPacketTypes
**
** Description      This function is set the packet types used for the specified
**                  ACL connection,
**
** Returns          packet types supported for the connection, or 0 if no BD address
**
*******************************************************************************/
BTM_API extern UINT16 BTM_ReadPacketTypes(BD_ADDR remote_bda);


/*******************************************************************************
**
** Function         BTM_IsAclConnectionUp
**
** Description      This function is called to check if an ACL connection exists
**                  to a specific remote BD Address.
**
** Returns          TRUE if connection is up, else FALSE.
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_IsAclConnectionUp(BD_ADDR remote_bda,
        tBT_TRANSPORT transport);


/*******************************************************************************
**
** Function         BTM_GetRole
**
** Description      This function is called to get the role of the local device
**                  for the ACL connection with the specified remote device
**
** Returns          BTM_SUCCESS if connection exists.
**                  BTM_UNKNOWN_ADDR if no active link with bd addr specified
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_GetRole(BD_ADDR remote_bd_addr, UINT8 *p_role, tBT_TRANSPORT transport);



/*******************************************************************************
**
** Function         BTM_SwitchRole
**
** Description      This function is called to switch role between master and
**                  slave.  If role is already set it will do nothing.  If the
**                  command was initiated, the callback function is called upon
**                  completion.
**
** Returns          BTM_SUCCESS if already in specified role.
**                  BTM_CMD_STARTED if command issued to controller.
**                  BTM_NO_RESOURCES if couldn't allocate memory to issue command
**                  BTM_UNKNOWN_ADDR if no active link with bd addr specified
**                  BTM_MODE_UNSUPPORTED if local device doesn't support role switching
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SwitchRole(BD_ADDR remote_bd_addr,
        UINT8 new_role,
        tBTM_CMPL_CB *p_cb);

/*******************************************************************************
**
** Function         BTM_ChangeLinkKey
**
** Description      This function is called to change the link key of the
**                  connection.
**
** Returns          BTM_CMD_STARTED if command issued to controller.
**                  BTM_NO_RESOURCES if couldn't allocate memory to issue command
**                  BTM_UNKNOWN_ADDR if no active link with bd addr specified
**                  BTM_BUSY if the previous command is not completed
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_ChangeLinkKey(BD_ADDR remote_bd_addr,
        tBTM_CMPL_CB *p_cb);

/*******************************************************************************
**
** Function         BTM_ReadRSSI
**
** Description      This function is called to read the link RSSI.
**                  The address of link RSSI results are returned in the callback.
**                  (tBTM_RSSI_RESULTS)
**
** Returns          BTM_CMD_STARTED if command issued to controller.
**                  BTM_NO_RESOURCES if couldn't allocate memory to issue command
**                  BTM_UNKNOWN_ADDR if no active link with bd addr specified
**                  BTM_BUSY if command is already in progress
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_ReadRSSI(BD_ADDR remote_bda,  tBT_TRANSPORT transport,
                                        tBTM_CMPL_CB *p_cb);


/*******************************************************************************
**
** Function         BTM_ReadTxPower
**
** Description      This function is called to read the current connection
**                  TX power of the connection. The TX power level results
**                  are returned in the callback.
**                  (tBTM_RSSI_RESULTS)
**
** Returns          BTM_CMD_STARTED if command issued to controller.
**                  BTM_NO_RESOURCES if couldn't allocate memory to issue command
**                  BTM_UNKNOWN_ADDR if no active link with bd addr specified
**                  BTM_BUSY if command is already in progress
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_ReadTxPower(BD_ADDR remote_bda,
        tBT_TRANSPORT transport,
        tBTM_CMPL_CB *p_cb);

/*******************************************************************************
**
** Function         BTM_ReadAdvTxPower
**
** Description      This function is called to read the advertisements
**                  TX power. The TX power level results
**                  are returned in the callback.
**                  (tBTM_RSSI_RESULTS)
**
** Returns          BTM_CMD_STARTED if command issued to controller.
**                  BTM_NO_RESOURCES if couldn't allocate memory to issue command
**                  BTM_UNKNOWN_ADDR if no active link with bd addr specified
**                  BTM_BUSY if command is already in progress
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_ReadAdvTxPower(tBTM_CMPL_CB *p_cb);


/*******************************************************************************
**
** Function         BTM_ReadLinkQuality
**
** Description      This function is called to read the link quality.
**                  The value of the link quality is returned in the callback.
**                  (tBTM_LINK_QUALITY_RESULTS)
**
** Returns          BTM_CMD_STARTED if command issued to controller.
**                  BTM_NO_RESOURCES if couldn't allocate memory to issue command
**                  BTM_UNKNOWN_ADDR if no active link with bd addr specified
**                  BTM_BUSY if command is already in progress
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_ReadLinkQuality(BD_ADDR remote_bda, tBTM_CMPL_CB *p_cb);

/*******************************************************************************
**
** Function         BTM_RegBusyLevelNotif
**
** Description      This function is called to register a callback to receive
**                  busy level change events.
**
** Returns          BTM_SUCCESS if successfully registered, otherwise error
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_RegBusyLevelNotif(tBTM_BL_CHANGE_CB *p_cb,
        UINT8 *p_level,
        tBTM_BL_EVENT_MASK evt_mask);

/*******************************************************************************
**
** Function         BTM_AclRegisterForChanges
**
** Description      This function is called to register a callback to receive
**                  ACL database change events, i.e. new connection or removed.
**
** Returns          BTM_SUCCESS if successfully initiated, otherwise error
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_AclRegisterForChanges(tBTM_ACL_DB_CHANGE_CB *p_cb);

/*******************************************************************************
**
** Function         BTM_GetNumAclLinks
**
** Description      This function is called to count the number of
**                  ACL links that are active.
**
** Returns          UINT16  Number of active ACL links
**
*******************************************************************************/
BTM_API extern UINT16 BTM_GetNumAclLinks(void);


/*******************************************************************************
**
** Function         BTM_ReadClockOffset
**
** Description      This returns the clock offset for a specific
**                  ACL connection.
**
** Returns          clock-offset or 0 if unknown
**
*******************************************************************************/
BTM_API extern UINT16 BTM_ReadClockOffset(BD_ADDR remote_bda);


/*******************************************************************************
**
** Function         BTM_SetQoS
**
** Description      This function is called to setup QoS
**
** Returns          BTM_CMD_STARTED if successfully initiated, otherwise error
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetQoS(BD_ADDR bd, FLOW_SPEC *p_flow,
                                      tBTM_CMPL_CB *p_cb);


/*****************************************************************************
**  (e)SCO CHANNEL MANAGEMENT FUNCTIONS
*****************************************************************************/
/*******************************************************************************
**
** Function         BTM_CreateSco
**
** Description      This function is called to create an SCO connection. If the
**                  "is_orig" flag is TRUE, the connection will be originated,
**                  otherwise BTM will wait for the other side to connect.
**
** Returns          BTM_UNKNOWN_ADDR if the ACL connection is not up
**                  BTM_BUSY         if another SCO being set up to
**                                   the same BD address
**                  BTM_NO_RESOURCES if the max SCO limit has been reached
**                  BTM_CMD_STARTED  if the connection establishment is started.
**                                   In this case, "*p_sco_inx" is filled in
**                                   with the sco index used for the connection.
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_CreateSco(BD_ADDR remote_bda, BOOLEAN is_orig,
        UINT16 pkt_types, UINT16 *p_sco_inx);

/*******************************************************************************
**
** Function         BTM_RemoveSco
**
** Description      This function is called to remove a specific SCO connection.
**
** Returns          BTM_CMD_STARTED if successfully initiated, otherwise error
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_RemoveSco(UINT16 sco_inx);


/*******************************************************************************
**
** Function         BTM_SetScoPacketTypes
**
** Description      This function is called to set the packet types used for
**                  a specific SCO connection,
**
** Parameters       pkt_types - One or more of the following
**                  BTM_SCO_PKT_TYPES_MASK_HV1
**                  BTM_SCO_PKT_TYPES_MASK_HV2
**                  BTM_SCO_PKT_TYPES_MASK_HV3
**                  BTM_SCO_PKT_TYPES_MASK_EV3
**                  BTM_SCO_PKT_TYPES_MASK_EV4
**                  BTM_SCO_PKT_TYPES_MASK_EV5
**
**                  BTM_SCO_LINK_ALL_MASK   - enables all supported types
**
** Returns          BTM_CMD_STARTED if successfully initiated, otherwise error
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetScoPacketTypes(UINT16 sco_inx, UINT16 pkt_types);


/*******************************************************************************
**
** Function         BTM_ReadScoPacketTypes
**
** Description      This function is read the packet types used for a specific
**                  SCO connection.
**
** Returns       One or more of the following (bitmask)
**                  BTM_SCO_PKT_TYPES_MASK_HV1
**                  BTM_SCO_PKT_TYPES_MASK_HV2
**                  BTM_SCO_PKT_TYPES_MASK_HV3
**                  BTM_SCO_PKT_TYPES_MASK_EV3
**                  BTM_SCO_PKT_TYPES_MASK_EV4
**                  BTM_SCO_PKT_TYPES_MASK_EV5
**
** Returns          packet types supported for the connection
**
*******************************************************************************/
BTM_API extern UINT16 BTM_ReadScoPacketTypes(UINT16 sco_inx);


/*******************************************************************************
**
** Function         BTM_ReadDeviceScoPacketTypes
**
** Description      This function is read the SCO packet types that
**                  the device supports.
**
** Returns          packet types supported by the device.
**
*******************************************************************************/
BTM_API extern UINT16 BTM_ReadDeviceScoPacketTypes(void);


/*******************************************************************************
**
** Function         BTM_ReadScoHandle
**
** Description      This function is used to read the HCI handle used for a specific
**                  SCO connection,
**
** Returns          handle for the connection, or 0xFFFF if invalid SCO index.
**
*******************************************************************************/
BTM_API extern UINT16 BTM_ReadScoHandle(UINT16 sco_inx);


/*******************************************************************************
**
** Function         BTM_ReadScoBdAddr
**
** Description      This function is read the remote BD Address for a specific
**                  SCO connection,
**
** Returns          pointer to BD address or NULL if not known
**
*******************************************************************************/
BTM_API extern UINT8 *BTM_ReadScoBdAddr(UINT16 sco_inx);


/*******************************************************************************
**
** Function         BTM_ReadScoDiscReason
**
** Description      This function is returns the reason why an (e)SCO connection
**                  has been removed. It contains the value until read, or until
**                  another (e)SCO connection has disconnected.
**
** Returns          HCI reason or BTM_INVALID_SCO_DISC_REASON if not set.
**
*******************************************************************************/
BTM_API extern UINT16 BTM_ReadScoDiscReason(void);


/*******************************************************************************
**
** Function         BTM_SetEScoMode
**
** Description      This function sets up the negotiated parameters for SCO and/or
**                  eSCO, and sets as the default mode used for calls to
**                  BTM_CreateSco.  It can be called only when there are no
**                  active (e)SCO links.
**
** Returns          BTM_SUCCESS if the successful.
**                  BTM_BUSY if there are one or more active (e)SCO links.
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetEScoMode(tBTM_ENH_ESCO_PARAMS *p_parms);

/*******************************************************************************
**
** Function         BTM_SetWBSCodec
**
** Description      This function sends command to the controller to setup
**                  WBS codec for the upcoming eSCO connection.
**
** Returns          BTM_SUCCESS.
**
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetWBSCodec(tBTM_SCO_CODEC_TYPE codec_type);

/*******************************************************************************
**
** Function         BTM_SetWBSCodec_Modified
**
** Description      This function sends command to the controller to setup
**                  WBS codec for the upcoming eSCO connection.
**
** Returns          BTM_SUCCESS.
**
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetWBSCodec_Modified(tBTM_SCO_CODEC_TYPE codec_type,
        UINT16 handle);

/*******************************************************************************
**
** Function         BTM_ReadEScoLinkParms
**
** Description      This function returns the current eSCO link parameters for
**                  the specified handle.  This can be called anytime a connection
**                  is active, but is typically called after receiving the SCO
**                  opened callback.
**
**                  Note: If called over a 1.1 controller, only the packet types
**                        field has meaning.
**                  Note: If the upper layer doesn't know the current sco index,
**                  BTM_FIRST_ACTIVE_SCO_INDEX can be used as the first parameter to
**                  find the first active SCO index
**
** Returns          BTM_SUCCESS if returned data is valid connection.
**                  BTM_ILLEGAL_VALUE if no connection for specified sco_inx.
**                  BTM_MODE_UNSUPPORTED if local controller does not support
**                      1.2 specification.
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_ReadEScoLinkParms(UINT16 sco_inx,
        tBTM_ESCO_DATA *p_parms);

/*******************************************************************************
**
** Function         BTM_ChangeEScoLinkParms
**
** Description      This function requests renegotiation of the parameters on
**                  the current eSCO Link.  If any of the changes are accepted
**                  by the controllers, the BTM_ESCO_CHG_EVT event is sent in
**                  the tBTM_ESCO_CBACK function with the current settings of
**                  the link. The callback is registered through the call to
**                  BTM_SetEScoMode.
**
**
** Returns          BTM_CMD_STARTED if command is successfully initiated.
**                  BTM_ILLEGAL_VALUE if no connection for specified sco_inx.
**                  BTM_NO_RESOURCES - not enough resources to initiate command.
**                  BTM_MODE_UNSUPPORTED if local controller does not support
**                      1.2 specification.
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_ChangeEScoLinkParms(UINT16 sco_inx,
        tBTM_CHG_ESCO_PARAMS *p_parms);

/*******************************************************************************
**
** Function         BTM_EScoConnRsp
**
** Description      This function is called upon receipt of an (e)SCO connection
**                  request event (BTM_ESCO_CONN_REQ_EVT) to accept or reject
**                  the request. Parameters used to negotiate eSCO links.
**                  If p_parms is NULL, then values set through BTM_SetEScoMode
**                  are used.
**                  If the link type of the incoming request is SCO, then only
**                  the tx_bw, max_latency, content format, and packet_types are
**                  valid.  The hci_status parameter should be
**                  ([0x0] to accept, [0x0d..0x0f] to reject)
**
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_EScoConnRsp(UINT16 sco_inx, UINT8 hci_status,
                                    tBTM_ENH_ESCO_PARAMS *p_parms);

/*******************************************************************************
**
** Function         BTM_GetNumScoLinks
**
** Description      This function returns the number of active SCO links.
**
** Returns          UINT8
**
*******************************************************************************/
BTM_API extern UINT8 BTM_GetNumScoLinks(void);

/*****************************************************************************
**  SECURITY MANAGEMENT FUNCTIONS
*****************************************************************************/
#ifndef BTEWICED
/*******************************************************************************
**
** Function         BTM_SecRegister
**
** Description      Application manager calls this function to register for
**                  security services.  There can be one and only one application
**                  saving link keys.  BTM allows only first registration.
**
** Returns          TRUE if registered OK, else FALSE
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_SecRegister(tBTM_APPL_INFO *p_cb_info);


/*******************************************************************************
**
** Function         BTM_SecRegisterLinkKeyNotificationCallback
**
** Description      Profiles can register to be notified when a new Link Key
**                  is generated per connection.
**
** Returns          TRUE if registered OK, else FALSE
**
*******************************************************************************/
BTM_API extern BOOLEAN
BTM_SecRegisterLinkKeyNotificationCallback(tBTM_LINK_KEY_CALLBACK *p_callback);
#else
/*******************************************************************************
**
** Function         BTM_Register
**
** Description      Register BTM event callback
**
** Returns          TRUE if registered OK, else FALSE
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_Register(tBTM_EVENT_CBACK *p_callback);

/*******************************************************************************
**
** Function         BTM_Disable
**
** Description      Send HCI RESET to controller and free up resources.
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_Disable(tBTM_CMPL_CB *p_cb);
#endif


/*******************************************************************************
**
** Function         BTM_SecAddRmtNameNotifyCallback
**
** Description      Profiles can register to be notified when name of the
**                  remote device is resolved (up to BTM_SEC_MAX_RMT_NAME_CALLBACKS).
**
** Returns          TRUE if registered OK, else FALSE
**
*******************************************************************************/
BTM_API extern BOOLEAN
BTM_SecAddRmtNameNotifyCallback(tBTM_RMT_NAME_CALLBACK *p_callback);


/*******************************************************************************
**
** Function         BTM_SecDeleteRmtNameNotifyCallback
**
** Description      A profile can deregister notification when a new Link Key
**                  is generated per connection.
**
** Returns          TRUE if OK, else FALSE
**
*******************************************************************************/
BTM_API extern BOOLEAN
BTM_SecDeleteRmtNameNotifyCallback(tBTM_RMT_NAME_CALLBACK *p_callback);


/*******************************************************************************
**
** Function         BTM_SecSetConnectFilterCallback
**
** Description      Host can register to be asked whenever an HCI connection
**                  request is received.  In the registered function host
**                  suppose to check connectibility filters.  Yes/No result
**                  should be returned synchronously.
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_SecSetConnectFilterCallback(tBTM_FILTER_CB *p_callback);


/*******************************************************************************
**
** Function         BTM_GetSecurityMode
**
** Description      Get security mode for the device
**
** Returns          void
**
*******************************************************************************/
BTM_API extern UINT8 BTM_GetSecurityMode(void);


/*******************************************************************************
**
** Function         BTM_GetSecurityFlags
**
** Description      Get security flags for the device
**
** Returns          BOOLEAN TRUE or FALSE is device found
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_GetSecurityFlags(BD_ADDR bd_addr, UINT8 *p_sec_flags);

/*******************************************************************************
**
** Function         BTM_GetSecurityFlagsByTransport
**
** Description      Get security flags for the device on a particular transport
**
** Returns          BOOLEAN TRUE or FALSE is device found
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_GetSecurityFlagsByTransport(BD_ADDR bd_addr,
        UINT8 *p_sec_flags, tBT_TRANSPORT transport);

/*******************************************************************************
**
** Function         BTM_ReadTrustedMask
**
** Description      Get trusted mask for the device
**
** Returns          NULL, if the device record is not found.
**                  otherwise, the trusted mask
**
*******************************************************************************/
BTM_API extern UINT32 *BTM_ReadTrustedMask(BD_ADDR bd_addr);


/*******************************************************************************
**
** Function         BTM_SetSecurityMode
**
** Description      Set security mode for the device
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_SetSecurityMode(UINT8 sec_mode);


/*******************************************************************************
**
** Function         BTM_SetPinType
**
** Description      Set PIN type for the device.
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_SetPinType(UINT8 pin_type, PIN_CODE pin_code,
                                   UINT8 pin_code_len);


/*******************************************************************************
**
** Function         BTM_SetPairableMode
**
** Description      Enable or disable pairing
**
** Parameters       allow_pairing - (TRUE or FALSE) whether or not the device
**                      allows pairing.
**                  connect_only_paired - (TRUE or FALSE) whether or not to
**                      only allow paired devices to connect.
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_SetPairableMode(BOOLEAN allow_pairing,
                                        BOOLEAN connect_only_paired);


/*******************************************************************************
**
** Function         BTM_SetBondableMode
**
** Description      Enable or disable bonding
**
** Parameters       allow_bonding - (TRUE or FALSE) whether or not the device
**                      allows bonding.
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_SetBondableMode(BOOLEAN allow_bonding);


#if (BTM_BR_SC_INCLUDED == TRUE)
/*******************************************************************************
**
** Function         BTM_DisableLevel0Treatment
**
** Description      Enable or disable default treatment for Mode 4 Level 0 services
**
** Parameter        disable_level0_treatment - (TRUE or FALSE) whether or not the device
**                  TRUE means that the device should treat Mode 4 Level 0 services as
**                  services of other levels.
**                  FALSE means that the device should provide default treatment for
**                  Mode 4 Level 0 services.
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_DisableLevel0Treatment(BOOLEAN disable_level0_treatment);
#endif

/*******************************************************************************
**
** Function         BTM_SecSetAuthPayloadTimeout
**
** Description      This command writes the Authenticated_Payload_Timeout to the
**                  controller for the specified connection handle.
**
**                  When the Connection_Handle identifies a BR/EDR ACL connection:
**                      If the connection is in sniff mode, the timeout_value shall
**                          be equal to or greater than Tsniff.
**                      If the connection is in sniff subrating mode, the timeout_value
**                          shall be equal to or greater than (max subrate)xTsniff.
**
**                  When the Connection_Handle identifies an LE connection, the
**                  Authenticated_Payload_Timeout shall be equal to or greater than
**                  connInterval * (1 + connSlaveLatency).
**
** Parameters       handle - (UINT16) Connection Handle
**                  timeout_value - 10 msec increments (Range: 1 - 0xffff)
**
** Returns          BOOLEAN - TRUE if sent to controller, FALSE if error or
**                             controller does not support command
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_SecSetAuthPayloadTimeout(UINT16 handle, UINT16 timeout_value);

/*******************************************************************************
**
** Function         BTM_SecAddDevice
**
** Description      Add/modify device.  This function will be normally called
**                  during host startup to restore all required information
**                  stored in the NVRAM.
**                  dev_class, bd_name, link_key, and features are NULL if unknown
**
** Returns          TRUE if added OK, else FALSE
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_SecAddDevice(BD_ADDR bd_addr, DEV_CLASS dev_class,
                                        BD_NAME bd_name, UINT8 *features,
                                        LINK_KEY link_key,
                                        UINT8 key_type, tBTM_IO_CAP io_cap);


/*******************************************************************************
**
** Function         BTM_SecDeleteDevice
**
** Description      Free resources associated with the device.
**
** Returns          TRUE if rmoved OK, FALSE if not found
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_SecDeleteDevice(BD_ADDR bd_addr);


/*******************************************************************************
**
** Function         BTM_SecUseMasterLinkKey
**
** Description      This function is called to tell master of the piconet to
**                  switch to master link key
**
** Returns          BTM_SUCCESS if command is successully initiated
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SecUseMasterLinkKey(BOOLEAN use_master_key);


/*******************************************************************************
**
** Function         BTM_SetMasterKeyCompCback
**
** Description      This function is called to register for the master key complete
**                  status event.
**
** Parameters:      mkey_cback - callback registered with the security manager
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_SetMasterKeyCompCback(tBTM_MKEY_CALLBACK *mkey_cback);


/*******************************************************************************
**
** Function         BTM_SecGetDeviceLinkKey
**
** Description      This function is called to obtain link key for the device
**                  it returns BTM_SUCCESS if link key is available, or
**                  BTM_UNKNOWN_ADDR if Security Manager does not know about
**                  the device or device record does not contain link key info
**
** Returns          BTM_SUCCESS if successful, otherwise error code
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SecGetDeviceLinkKey(BD_ADDR bd_addr,
        LINK_KEY link_key);


/*******************************************************************************
**
** Function         BTM_SecGetDeviceLinkKeyType
**
** Description      This function is called to obtain link key type for the
**                  device.
**                  it returns BTM_SUCCESS if link key is available, or
**                  BTM_UNKNOWN_ADDR if Security Manager does not know about
**                  the device or device record does not contain link key info
**
** Returns          BTM_LKEY_TYPE_IGNORE if link key is unknown, link type
**                  otherwise.
**
*******************************************************************************/
BTM_API extern UINT8 BTM_SecGetDeviceLinkKeyType(BD_ADDR bd_addr);


/*******************************************************************************
**
** Function         BTM_PINCodeReply
**
** Description      This function is called after Security Manager submitted
**                  PIN code request to the UI.
**
** Parameters:      bd_addr      - Address of the device for which PIN was requested
**                  res          - result of the operation BTM_SUCCESS if success
**                  pin_len      - length in bytes of the PIN Code
**                  p_pin        - pointer to array with the PIN Code
**                  trusted_mask - bitwise OR of trusted services (array of UINT32)
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_PINCodeReply(BD_ADDR bd_addr, tBTM_STATUS res, UINT8 pin_len,
                                     UINT8 *p_pin);

/*******************************************************************************
**
** Function         BTM_SecBond
**
** Description      This function is called to perform bonding with peer device.
**
** Parameters:      bd_addr      - Address of the device to bond
**                  pin_len      - length in bytes of the PIN Code
**                  p_pin        - pointer to array with the PIN Code
**                  trusted_mask - bitwise OR of trusted services (array of UINT32)

** Returns          BTM_CMD_STARTED if successfully initiated, otherwise error
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SecBond(BD_ADDR bd_addr,
                                       UINT8 pin_len, UINT8 *p_pin);

/*******************************************************************************
**
** Function         BTM_SecBondByTransport
**
** Description      This function is called to perform bonding by designated transport
**
** Parameters:      bd_addr      - Address of the device to bond
**                  pin_len      - length in bytes of the PIN Code
**                  p_pin        - pointer to array with the PIN Code
**                  trusted_mask - bitwise OR of trusted services (array of UINT32)
**                  transport :  Physical transport to use for bonding (BR/EDR or LE)
**
** Returns          BTM_CMD_STARTED if successfully initiated, otherwise error
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SecBondByTransport(BD_ADDR bd_addr,
        tBLE_ADDR_TYPE bd_addr_type,
        tBT_TRANSPORT transport,
        UINT8 pin_len, UINT8 *p_pin);

/*******************************************************************************
**
** Function         BTM_SecBondCancel
**
** Description      This function is called to cancel ongoing bonding process
**                  with peer device.
**
** Returns          BTM_CMD_STARTED if successfully initiated, otherwise error
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SecBondCancel(BD_ADDR bd_addr);

/*******************************************************************************
**
** Function         BTM_SetEncryption
**
** Description      This function is called to ensure that connection is
**                  encrypted.  Should be called only on an open connection.
**                  Typically only needed for connections that first want to
**                  bring up unencrypted links, then later encrypt them.
**
** Parameters:      bd_addr       - Address of the peer device
**                  transport     - over which trasnport the link to be encrypted;
**                                  TRUE for LE, FALSE for BR/EDR
**                  p_callback    - Pointer to callback function called if
**                                  this function returns PENDING after required
**                                  procedures are completed.  Can be set to NULL
**                                  if status is not desired.
**                  p_ref_data    - pointer to any data the caller wishes to receive
**                                  in the callback function upon completion.
*                                   can be set to NULL if not used.
**
** Returns          BTM_SUCCESS   - already encrypted
**                  BTM_PENDING   - command will be returned in the callback
**                  BTM_WRONG_MODE- connection not up.
**                  BTM_BUSY      - security procedures are currently active
**                  BTM_MODE_UNSUPPORTED - if security manager not linked in.
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetEncryption(BD_ADDR bd_addr, tBT_TRANSPORT transport,
        void *p_ref_data);

/*******************************************************************************
**
** Function         BTM_ConfirmReqReply
**
** Description      This function is called to confirm the numeric value for
**                  Simple Pairing in response to BTM_SP_CFM_REQ_EVT
**
** Parameters:      res           - result of the operation BTM_SUCCESS if success
**                  bd_addr       - Address of the peer device
**
*******************************************************************************/
BTM_API extern void BTM_ConfirmReqReply(tBTM_STATUS res, BD_ADDR bd_addr);

/*******************************************************************************
**
** Function         BTM_PasskeyReqReply
**
** Description      This function is called to provide the passkey for
**                  Simple Pairing in response to BTM_SP_KEY_REQ_EVT
**
** Parameters:      res           - result of the operation BTM_SUCCESS if success
**                  bd_addr       - Address of the peer device
**                  passkey       - numeric value in the range of 0 - 999999(0xF423F).
**
*******************************************************************************/
BTM_API extern void BTM_PasskeyReqReply(tBTM_STATUS res, BD_ADDR bd_addr,
                                        UINT32 passkey);

/*******************************************************************************
**
** Function         BTM_SendKeypressNotif
**
** Description      This function is used during the passkey entry model
**                  by a device with KeyboardOnly IO capabilities
**                  (very likely to be a HID Device).
**                  It is called by a HID Device to inform the remote device when
**                  a key has been entered or erased.
**
** Parameters:      bd_addr - Address of the peer device
**                  type - notification type
**
*******************************************************************************/
BTM_API extern void BTM_SendKeypressNotif(BD_ADDR bd_addr, tBTM_SP_KEY_TYPE type);

/*******************************************************************************
**
** Function         BTM_IoCapRsp
**
** Description      This function is called in response to BTM_SP_IO_REQ_EVT
**                  When the event data io_req.oob_data is set to BTM_OOB_UNKNOWN
**                  by the tBTM_SP_CALLBACK implementation, this function is
**                  called to provide the actual response
**
** Parameters:      bd_addr - Address of the peer device
**                  io_cap  - The IO capability of local device.
**                  oob     - The type of remote OOB data present on local device.
**                  auth_req- MITM protection required or not.
**
*******************************************************************************/
BTM_API extern void BTM_IoCapRsp(BD_ADDR bd_addr, tBTM_IO_CAP io_cap,
                                 tBTM_OOB_DATA oob, tBTM_AUTH_REQ auth_req);

/*******************************************************************************
**
** Function         BTM_ReadLocalOobData
**
** Description      This function is called to read the local OOB data from
**                  LM
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_ReadLocalOobData(void);

/*******************************************************************************
**
** Function         BTM_RemoteOobDataReply
**
** Description      This function is called to provide the remote OOB data for
**                  Simple Pairing in response to BTM_SP_RMT_OOB_EVT
**
** Parameters:      bd_addr     - Address of the peer device
**                  c           - simple pairing Hash C.
**                  r           - simple pairing Randomizer R.
**
*******************************************************************************/
BTM_API extern void BTM_RemoteOobDataReply(tBTM_STATUS res, BD_ADDR bd_addr,
        BT_OCTET16 c, BT_OCTET16 r);

#if (BTM_BR_SC_INCLUDED == TRUE)
/*******************************************************************************
**
** Function         BTM_RemoteOobExtDataReply
**
** Description      This function is called to provide the remote OOB extended data
**                  for Simple Pairing in response to BTM_SP_RMT_OOB_EXT_EVT when
**                  both local and peer devices support SC.
**
**
** Parameters:      bd_addr     - Address of the peer device
**                  c_192       - simple pairing Hash C derived from the P-192 public key.
**                  r_192       - simple pairing Randomizer R associated with the P-192 public key.
**                  c_256       - simple pairing Hash C derived from the P-256 public key.
**                  r_256       - simple pairing Randomizer R associated with the P-256 public key.
**
*******************************************************************************/
BTM_API extern void BTM_RemoteOobExtDataReply(tBTM_STATUS res, BD_ADDR bd_addr,
        BT_OCTET16 c_192, BT_OCTET16 r_192,
        BT_OCTET16 c_256, BT_OCTET16 r_256);
#endif

/*******************************************************************************
**
** Function         BTM_BuildOobData
**
** Description      This function is called to build the OOB data payload to
**                  be sent over OOB (non-Bluetooth) link
**
** Parameters:      p_data  - the location for OOB data
**                  max_len - p_data size.
**                  c       - simple pairing Hash C.
**                  r       - simple pairing Randomizer  C.
**                  name_len- 0, local device name would not be included.
**                            otherwise, the local device name is included for
**                            up to this specified length
**
** Returns          Number of bytes in p_data.
**
*******************************************************************************/
BTM_API extern UINT16 BTM_BuildOobData(UINT8 *p_data, UINT16 max_len, BT_OCTET16 c,
                                       BT_OCTET16 r, UINT8 name_len);

#if (BTM_BR_SC_INCLUDED == TRUE)
/*******************************************************************************
**
** Function         BTM_BuildOobExtData
**
** Description      This function is called to build the OOB data block to be used
**                  to send OOB extended data over OOB (non-Bluetooth) link.
**
** Parameters:      p_data  - OOB data block location
**                  max_len - OOB data block size
**                  c_192   - simple pairing Hash C derived from the P-192 public key.
**                  r_192   - simple pairing Randomizer R associated with the P-192 public key.
**                  c_256   - simple pairing Hash C derived from the P-256 public key.
**                  r_256   - simple pairing Randomizer R associated with the P-256 public key.
*
** Returns          Number of bytes put into OOB data block.
**
*******************************************************************************/
BTM_API extern UINT16 BTM_BuildOobExtData(UINT8 *p_data, UINT16 max_len,
        BT_OCTET16 c_192, BT_OCTET16 r_192,
        BT_OCTET16 c_256, BT_OCTET16 r_256);

/*******************************************************************************
**
** Function         BTM_AreBrScSuppLocally
**
** Description      This function is called to check if the local device supports
**                  BR/EDR Secure Connections.
**
** Returns          TRUE if BR/EDR Secure Connections are supported locally,
**                  else FALSE.
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_AreBrScSuppLocally(void);

/*******************************************************************************
**
** Function         BTM_AreBrScSuppByPeer
**
** Description      This function is called to check if the peer supports
**                  BR/EDR Secure Connections.
**
** Parameters:      bd_addr - address of the peer
**
** Returns          TRUE if BR/EDR Secure Connections are supported by the peer,
**                  else FALSE.
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_AreBrScSuppByPeer(BD_ADDR bd_addr);

#endif  /* BTM_BR_SC_INCLUDED == TRUE */

/*******************************************************************************
**
** Function         BTM_IsLeScSuppLocally
**
** Description      This function is called to check if LE SC is supported.
**
** Parameters:      None.
**
**  Returns         Boolean - TRUE if LE SC is supported.
*******************************************************************************/
BTM_API extern BOOLEAN BTM_IsLeScSuppLocally(void);

/*******************************************************************************
**
** Function         BTM_ReadOobData
**
** Description      This function is called to parse the OOB data payload
**                  received over OOB (non-Bluetooth) link
**
** Parameters:      p_data  - the location for OOB data
**                  eir_tag - The associated EIR tag to read the data.
**                  *p_len(output) - the length of the data with the given tag.
**
** Returns          the beginning of the data with the given tag.
**                  NULL, if the tag is not found.
**
*******************************************************************************/
BTM_API extern UINT8 *BTM_ReadOobData(UINT8 *p_data, UINT8 eir_tag, UINT8 *p_len);

/*****************************************************************************
**  POWER MANAGEMENT FUNCTIONS
*****************************************************************************/
#ifndef BTEWICED
/*******************************************************************************
**
** Function         BTM_PmRegister
**
** Description      register or deregister with power manager
**
** Returns          BTM_SUCCESS if successful,
**                  BTM_NO_RESOURCES if no room to hold registration
**                  BTM_ILLEGAL_VALUE
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_PmRegister(UINT8 mask, UINT8 *p_pm_id,
        tBTM_PM_STATUS_CBACK *p_cb);
#endif

/*******************************************************************************
**
** Function         BTM_SetPowerMode
**
** Description      store the mode in control block or
**                  alter ACL connection behavior.
**
** Returns          BTM_SUCCESS if successful,
**                  BTM_UNKNOWN_ADDR if bd addr is not active or bad
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetPowerMode(UINT8 pm_id, BD_ADDR remote_bda,
        tBTM_PM_PWR_MD *p_mode);


/*******************************************************************************
**
** Function         BTM_ReadPowerMode
**
** Description      This returns the current mode for a specific
**                  ACL connection.
**
** Input Param      remote_bda - device address of desired ACL connection
**
** Output Param     p_mode - address where the current mode is copied into.
**                          BTM_ACL_MODE_NORMAL
**                          BTM_ACL_MODE_HOLD
**                          BTM_ACL_MODE_SNIFF
**                          BTM_ACL_MODE_PARK
**                          (valid only if return code is BTM_SUCCESS)
**
** Returns          BTM_SUCCESS if successful,
**                  BTM_UNKNOWN_ADDR if bd addr is not active or bad
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_ReadPowerMode(BD_ADDR remote_bda,
        tBTM_PM_MODE *p_mode);

/*******************************************************************************
**
** Function         BTM_SetSsrParams
**
** Description      This sends the given SSR parameters for the given ACL
**                  connection if it is in ACTIVE mode.
**
** Input Param      remote_bda - device address of desired ACL connection
**                  max_lat    - maximum latency (in 0.625ms)(0-0xFFFE)
**                  min_rmt_to - minimum remote timeout
**                  min_loc_to - minimum local timeout
**
**
** Returns          BTM_SUCCESS if the HCI command is issued successful,
**                  BTM_UNKNOWN_ADDR if bd addr is not active or bad
**                  BTM_CMD_STORED if the command is stored
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetSsrParams(BD_ADDR remote_bda, UINT16 max_lat,
        UINT16 min_rmt_to, UINT16 min_loc_to);



/*******************************************************************************
**
** Function         BTM_IsPowerManagerOn
**
** Description      This function is called to check if power manager is included.
**                  in the BTE version.
**
** Returns          TRUE if power manager is compiled in, otherwise FALSE.
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_IsPowerManagerOn(void);

/*******************************************************************************
**
** Function         BTM_GetHCIConnHandle
**
** Description      This function is called to get the handle for an ACL connection
**                  to a specific remote BD Address.
**
** Returns          the handle of the connection, or 0xFFFF if none.
**
*******************************************************************************/
BTM_API extern UINT16 BTM_GetHCIConnHandle(BD_ADDR remote_bda,
        tBT_TRANSPORT transport);

/*******************************************************************************
**
** Function         BTM_SetUCDAllowed
**
** Description      This function finds device and set it as ucd allowed.
**
** Returns          TRUE if device is found, FALSE otherwise
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_SetUCDAllowed(BD_ADDR bd_addr, BOOLEAN ucd_allowed);

/*******************************************************************************
**
** Function         BTM_ReadStoredLinkKey
**
** Description      This function is called to obtain link key for the specified
**                  device from the NVRAM storage attached to the Bluetooth
**                  controller.
**
** Parameters:      bd_addr      - Address of the device
**                  p_cb         - Call back function to be called to return
**                                 the results
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_ReadStoredLinkKey(BD_ADDR bd_addr,  tBTM_CMPL_CB *p_cb);


/*******************************************************************************
**
** Function         BTM_WriteStoredLinkKey
**
** Description      This function is called to write link keys for the specified
**                  device addresses to the NVRAM storage attached to the Bluetooth
**                  controller.
**
** Parameters:      num_keys     - Number of link keys
**                  bd_addr      - Addresses of the devices
**                  link_key     - Link Keys to be stored
**                  p_cb         - Call back function to be called to return
**                                 the results
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_WriteStoredLinkKey(UINT8 num_keys,
        BD_ADDR *bd_addr,
        LINK_KEY *link_key,
        tBTM_CMPL_CB *p_cb);


/*******************************************************************************
**
** Function         BTM_DeleteStoredLinkKey
**
** Description      This function is called to delete link key for the specified
**                  device addresses from the NVRAM storage attached to the Bluetooth
**                  controller.
**
** Parameters:      bd_addr      - Addresses of the devices
**                  p_cb         - Call back function to be called to return
**                                 the results
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_DeleteStoredLinkKey(BD_ADDR bd_addr, tBTM_CMPL_CB *p_cb);

/*******************************************************************************
**
** Function         BTM_SetTriggeredClockCapture
**
** Description      This function is called to configure the BR/EDR controller for
**                  triggered clock capture.
**
** Parameters:      conn_handle     - Connection handle
**                  enable          - Enable or disable triggered clock capture
**                  which_clock     - 0 conn_handle is ignored, 1 conn_handle shall be valid
**                  lpo_allowed     - whether low power oscillator is allowed to be used or not
**                  num_clock_capture_to_filter - Filter triggered clock captures between clock capture events
**
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetTriggeredClockCapture(UINT16 conn_handle,
        UINT8 enable,
        UINT8 which_clock,
        UINT8 lpo_allowed,
        UINT8 num_clock_captures_to_filter);

/*******************************************************************************
**
** Function         BTM_WriteEIR
**
** Description      This function is called to write EIR data to controller.
**
** Parameters       p_buff - allocated HCI command buffer including extended
**                           inquriry response
**
** Returns          BTM_SUCCESS  - if successful
**                  BTM_MODE_UNSUPPORTED - if local device cannot support it
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_WriteEIR(BT_HDR *p_buff);

/*******************************************************************************
**
** Function         BTM_CheckEirData
**
** Description      This function is called to get EIR data from significant part.
**
** Parameters       p_eir - pointer of EIR significant part
**                  type   - finding EIR data type
**                  p_length - return the length of EIR data
**
** Returns          pointer of EIR data
**
*******************************************************************************/
BTM_API extern UINT8 *BTM_CheckEirData(UINT8 *p_eir, UINT8 type, UINT8 *p_length);

/*******************************************************************************
**
** Function         BTM_HasEirService
**
** Description      This function is called to know if UUID in bit map of UUID.
**
** Parameters       p_eir_uuid - bit map of UUID list
**                  uuid16 - UUID 16-bit
**
** Returns          TRUE - if found
**                  FALSE - if not found
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_HasEirService(UINT32 *p_eir_uuid, UINT16 uuid16);

/*******************************************************************************
**
** Function         BTM_HasInquiryEirService
**
** Description      This function is called to know if UUID in bit map of UUID list.
**
** Parameters       p_results - inquiry results
**                  uuid16 - UUID 16-bit
**
** Returns          BTM_EIR_FOUND - if found
**                  BTM_EIR_NOT_FOUND - if not found and it is complete list
**                  BTM_EIR_UNKNOWN - if not found and it is not complete list
**
*******************************************************************************/
BTM_API extern tBTM_EIR_SEARCH_RESULT
BTM_HasInquiryEirService(tBTM_INQ_RESULTS *p_results, UINT16 uuid16);

/*******************************************************************************
**
** Function         BTM_AddEirService
**
** Description      This function is called to add a service in bit map of UUID list.
**
** Parameters       p_eir_uuid - bit mask of UUID list for EIR
**                  uuid16 - UUID 16-bit
**
** Returns          None
**
*******************************************************************************/
BTM_API extern void BTM_AddEirService(UINT32 *p_eir_uuid, UINT16 uuid16);

/*******************************************************************************
**
** Function         BTM_RemoveEirService
**
** Description      This function is called to remove a service in bit map of UUID list.
**
** Parameters       p_eir_uuid - bit mask of UUID list for EIR
**                  uuid16 - UUID 16-bit
**
** Returns          None
**
*******************************************************************************/
BTM_API extern void BTM_RemoveEirService(UINT32 *p_eir_uuid, UINT16 uuid16);

/*******************************************************************************
**
** Function         BTM_GetEirSupportedServices
**
** Description      This function is called to get UUID list from bit map of UUID list.
**
** Parameters       p_eir_uuid - bit mask of UUID list for EIR
**                  p - reference of current pointer of EIR
**                  max_num_uuid16 - max number of UUID can be written in EIR
**                  num_uuid16 - number of UUID have been written in EIR
**
** Returns          BTM_EIR_MORE_16BITS_UUID_TYPE, if it has more than max
**                  BTM_EIR_COMPLETE_16BITS_UUID_TYPE, otherwise
**
*******************************************************************************/
BTM_API extern UINT8 BTM_GetEirSupportedServices(UINT32 *p_eir_uuid,    UINT8 **p,
        UINT8  max_num_uuid16,
        UINT8 *p_num_uuid16);

/*******************************************************************************
**
** Function         BTM_GetEirUuidList
**
** Description      This function parses EIR and returns UUID list.
**
** Parameters       p_eir - EIR
**                  uuid_size - LEN_UUID_16, LEN_UUID_32, LEN_UUID_128
**                  p_num_uuid - return number of UUID in found list
**                  p_uuid_list - return UUID 16-bit list
**                  max_num_uuid - maximum number of UUID to be returned
**
** Returns          0 - if not found
**                  BTM_EIR_COMPLETE_16BITS_UUID_TYPE
**                  BTM_EIR_MORE_16BITS_UUID_TYPE
**                  BTM_EIR_COMPLETE_32BITS_UUID_TYPE
**                  BTM_EIR_MORE_32BITS_UUID_TYPE
**                  BTM_EIR_COMPLETE_128BITS_UUID_TYPE
**                  BTM_EIR_MORE_128BITS_UUID_TYPE
**
*******************************************************************************/
BTM_API extern UINT8 BTM_GetEirUuidList(UINT8 *p_eir, UINT8 uuid_size,
                                        UINT8 *p_num_uuid,
                                        UINT8 *p_uuid_list, UINT8 max_num_uuid);

/*******************************************************************************
**
** Function         BTM_SyncStack
**
** Description      For Dual-Stack support. Called to initiate switching to/from
**                  main stack (running on phone baseband) to mm stack (light
**                  stack running on multi-media chip)
**
** Parameters       sync_dir: BTM_SW_BB_TO_MM: switch from BB to MM stack
**                            BTM_SW_MM_TO_BB: switch from MM to BB stack
**                            BTM_SW_RESYNC: resync MM and BB stacks
**
**                  p_sync_cback: callback function for event notification
** Returns
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SyncStack(tBTM_SW_DIR sync_dir,
        tBTM_SYNC_STACK_CBACK p_sync_cback);

/*******************************************************************************
**
** Function         BTM_SyncBtaRsp
**
** Description      For Dual-Stack support. Called to indicate that upper layers
**                  (e.g. BTA or application) have completed synchronizing bta/app
**                  specific layers for switching.
**
**                  Called in response to 'BTM_SYNC_BTA_EVT'
**
** Parameters       status: BTM_SUCESS: bta/app successfully synchronized
**                          otherwise:  sync was unsuccessfule. Abort switch.
**
**                  p_btm_sync_info: information from bta/app that will be needed
**                                  by BTE (avdt and l2cap) for switching.
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_SyncBtaRsp(tBTM_STATUS status, tBTM_SYNC_INFO *p_btm_sync_info);

/*******************************************************************************
**
** Function         BTM_OpenUIPC
**
** Description      For Dual-Stack support. Called to open UIPC between
**                  main stack (running on phone baseband) to embedded light stack
**                  (running on Multimedia or Bluetooth Controller chip)
**
** Parameters       sync_dir: BTM_SW_BB_TO_MM: switch from BB to MM stack
**                            BTM_SW_BB_TO_BTC:switch from BB to BTC stack
**
**                  p_sync_callback: callback function for event notification
** Returns
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_OpenUIPC(tBTM_SW_DIR sync_dir,
                                        tBTM_SYNC_STACK_CBACK p_sync_callback);

/*******************************************************************************
**
** Function         BTM_CloseUIPC
**
** Description      For Dual-Stack support. Called to close UIPC between
**                  main stack (running on phone baseband) to embedded light stack
**                  (running on Multimedia or Bluetooth Controller chip)
**
** Parameters
**                  p_sync_callback: callback function for event notification
** Returns
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_CloseUIPC(tBTM_SYNC_STACK_CBACK p_sync_callback);

/*******************************************************************************
**
** Function         BTM_IpcSend
**
** Description      For Dual-Stack support. Called to send ipc messages from
**                  full stack to lite stack and vice-versa. This API is
**                  typically called by bta layers e.g. bta_av.
**
**
** Parameters       len:    Length of the buffer in the ipc message
**
**                  buffer: Pointer to the buffer to be passed in the IPC message
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_IpcSend(UINT16 len, UINT8 *buffer);

/*******************************************************************************
**
** Function         BTM_IpcSendBuf
**
** Description      For Dual-Stack support. Called to send ipc messages from
**                  full stack to lite stack and vice-versa. This API is
**                  typically called by bta layers e.g. bta_av_sync.
**
**
** Parameters       p_buf: Pointer to the buffer to be passed in the IPC message
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_IpcSendBuf(BT_HDR *p_buf);

/*******************************************************************************
**
** Function         BTM_RegIpcEvtHandler
**
** Description      registers the DM provided handler for IPC events
**
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_RegIpcEvtHandler(tBTM_IPC_EVT_CBACK *p_cback);

/*******************************************************************************
**
** Function         BTM_RegRTIpcEvtHandler
**
** Description      registers the RT(Audio Routing) provided handler for IPC events
**
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_RegRTIpcEvtHandler(tBTM_IPC_EVT_CBACK *p_cback);

/*****************************************************************************
**  N2BT
*****************************************************************************/

/* Data callback for N2BT */
typedef void (tBTM_N2BT_DATA_CB)(BD_ADDR bd_addr, UINT16 handle, UINT8 *p_data,
                                 UINT16 datalen);

/*******************************************************************************
**
** Function         BTM_N2BtAcquire
**
** Description      Put controller into acquisition mode
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_N2BtAcquire(BD_ADDR bd_addr, UINT16 timeout,
                                    UINT8 freq, UINT8 src_addrlen, UINT8 sensor_flags,
                                    UINT8 sensor_type, UINT8 sensor_clk_accuracy,
                                    UINT16 add_rx_window, UINT16 init_crc,
                                    UINT32 ac_low, UINT32 ac_high, UINT16 pkt_hdr,
                                    UINT16 list_dur, UINT16 list_int,
                                    UINT8 oor_missed_pkts, tBTM_VSC_CMPL_CB *p_cb,
                                    tBTM_N2BT_DATA_CB *p_data_cback);

/*******************************************************************************
**
** Function         BTM_N2BtDisconnect
**
** Description      Disconnects all N2BT devices
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_N2BtDisconnect(void);


/*******************************************************************************
**
** Function         BTM_ConfigI2SPCM
**
** Description      This function sends VSC Write_I2SPCM_Interface_Param
**                  as to the specified codec_type.
**
**
** Parameter        codec_type: codec_type to be used for sco connection.
**                  role: master or slave role
**                  sample_rate: sampling rate
**                  clock_rate:clock rate 128K to 2048K
**
**
** Returns          BTM_SUCCESS if the successful.
**                  BTM_ILLEGAL_VALUE: wrong codec type
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_ConfigI2SPCM(tBTM_SCO_CODEC_TYPE codec_type,
        UINT8 role, UINT8 sample_rate,
        UINT8 clock_rate);

/*******************************************************************************
**
** Function         BTM_ConfigScoPath
**
** Description      This function enables/disables SCO over HCI and registers SCO
**                  data callback if SCO over HCI is enabled.
**
** Parameter        path: PCM or HCI or test
**                  p_sco_data_cb: callback function or SCO data if path is set
**                                 to transport.
**                  p_pcm_param: pointer to the PCM interface parameter. If a NULL
**                               pointer is used, PCM parameter maintained in
**                               the control block will be used; otherwise update
**                               control block value.
**                  err_data_rpt: Lisbon feature to enable the erronous data report
**                                or not.
**
** Returns          BTM_SUCCESS if the successful (PCM or Test mode).
**                  BTM_NO_RESOURCES: no resources to start the command.
**                  BTM_ILLEGAL_VALUE: invalid callback function pointer.
**                  BTM_CMD_STARTED :Command sent. Waiting for command cmpl event.
**
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_ConfigScoPath(tBTM_SCO_ROUTE_TYPE path,
        tBTM_SCO_DATA_CB *p_sco_data_cb,
        tBTM_SCO_PCM_PARAM *p_pcm_param,
        BOOLEAN err_data_rpt);


/*****************************************************************************
**  SCO OVER HCI
*****************************************************************************/

/*******************************************************************************
**
** Function         BTM_WriteScoData
**
** Description      This function write SCO data to a specified instance. The data
**                  to be written p_buf needs to carry an offset of
**                  HCI_SCO_PREAMBLE_SIZE bytes, and the data length can not
**                  exceed BTM_SCO_DATA_SIZE_MAX bytes, whose default value is set
**                  to 60 and is configurable. Data longer than the maximum bytes
**                  will be truncated.
**
** Returns          BTM_SUCCESS: data write is successful
**                  BTM_ILLEGAL_VALUE: SCO data contains illegal offset value.
**                  BTM_SCO_BAD_LENGTH: SCO data length exceeds the max SCO packet
**                                      size.
**                  BTM_NO_RESOURCES: no resources.
**                  BTM_UNKNOWN_ADDR: unknown SCO connection handle, or SCO is not
**                                    routed via HCI.
**
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_WriteScoData(UINT16 sco_inx, BT_HDR *p_buf);

/*******************************************************************************
**
** Function         BTM_RegMIPEvents
**
** Description      This function is called to register a callback for MIP
**                  related events.
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_RegMIPEvents(tBTM_MIP_EVENTS_CB *p_callback,
        tBTM_MIP_QUERY_CB *p_query_cb);

/*******************************************************************************
**
** Function         BTM_MIP_SetMode
**
** Description      This function set the remote device into MIP mode.
**
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_MIP_SetMode(BD_ADDR rem_addr, UINT8 mip_role,
        UINT8 mip_mode);

#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE && BTM_CROSS_TRANSP_KEY_DERIVATION == TRUE
/*******************************************************************************
**
** Function         BTM_SetNoSmpOnBr
**
** Description      This function turns OFF/ON SMP over BR/EDR (i.e. link keys
**                  crosspairing SC BR/EDR->SC LE) for the remote device.
**                  If mode is set to TRUE then the crosspairing will not happen.
**
** Returns          FALSE if the remote device with bd_addr is unknown,
**                  TRUE otherwise.
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_SetNoSmpOnBr(BD_ADDR bd_addr, BOOLEAN mode);

/*******************************************************************************
 **
 **  Function         BTM_ReadLQStats
 **
 **  Description      This function is called to read the link Quality stats.
 **                   The results are returned in the callback.
 **                   (tBTM_LQ_STATS_RESULTS)
 **
 **  Returns          BTM_CMD_STARTED if command issued to controller.
 **                   BTM_NO_RESOURCES if couldn't allocate memory to issue command
 **                   BTM_UNKNOWN_ADDR if no active link with bd addr specified
 **                   BTM_BUSY if command is already in progress
 **
 ********************************************************************************/
BTM_API extern tBTM_STATUS BTM_ReadLQStats(BD_ADDR remote_bda, tBT_TRANSPORT transport, tBTM_LQ_STATS_ACTION action,
        tBTM_CMPL_CB *p_cb);
#endif

#ifdef __cplusplus
}
#endif

#endif /* BTM_API_H */
