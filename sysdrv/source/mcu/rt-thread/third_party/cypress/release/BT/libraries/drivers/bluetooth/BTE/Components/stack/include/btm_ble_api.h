/*****************************************************************************
**
**  Name:          btm_ble_api.h
**
**  Description:   This file contains the Bluetooth Manager (BTM) API function
**                 external definitions.
**
**
**  Copyright (c) 1999-2016, Broadcom Corp., All Rights Reserved.
**  Broadcom Bluetooth Core. Proprietary and confidential.
******************************************************************************/
#ifndef BTM_BLE_API_H
#define BTM_BLE_API_H

#include "btm_api.h"

#define CHNL_MAP_LEN    5
typedef UINT8 tBTM_BLE_CHNL_MAP[CHNL_MAP_LEN];

/* 0x00-0x04 only used for set advertising parameter command */
#define BTM_BLE_CONNECT_EVT     0x00        /* Connectable undirected advertising */
#define BTM_BLE_CONNECT_DIR_EVT 0x01        /* Connectable directed advertising */
#define BTM_BLE_DISCOVER_EVT    0x02        /* Scannable undirected advertising */
#define BTM_BLE_NON_CONNECT_EVT 0x03        /* Non connectable undirected advertising */
#define BTM_BLE_CONNECT_LO_DUTY_DIR_EVT 0x04        /* Connectable low duty cycle directed advertising  */
/* 0x00 - 0x05 can be received on adv event type */
#define BTM_BLE_SCAN_RSP_EVT    0x04
#define BTM_BLE_SCAN_REQ_EVT    0x05
#define BTM_BLE_UNKNOWN_EVT     0xff

#define BTM_BLE_UNKNOWN_EVT     0xff

#define BTM_BLE_SCAN_MODE_PASS      0
#define BTM_BLE_SCAN_MODE_ACTI      1
#define BTM_BLE_SCAN_MODE_NONE      0xff
typedef UINT8 tBTM_BLE_SCAN_MODE;

/* advertising channel map */
#define BTM_BLE_ADV_CHNL_37    (0x01 << 0)
#define BTM_BLE_ADV_CHNL_38    (0x01 << 1)
#define BTM_BLE_ADV_CHNL_39    (0x01 << 2)
typedef UINT8 tBTM_BLE_ADV_CHNL_MAP;

/*default advertising channel map */
#ifndef BTM_BLE_DEFAULT_ADV_CHNL_MAP
#define BTM_BLE_DEFAULT_ADV_CHNL_MAP   (BTM_BLE_ADV_CHNL_37| BTM_BLE_ADV_CHNL_38| BTM_BLE_ADV_CHNL_39)
#endif

/* advertising filter policy */
#define AP_SCAN_CONN_ALL           0x00        /* default */
#define AP_SCAN_WL_CONN_ALL        0x01
#define AP_SCAN_ALL_CONN_WL        0x02
#define AP_SCAN_CONN_WL            0x03
#define AP_SCAN_CONN_POLICY_MAX    0x04
typedef UINT8   tBTM_BLE_AFP;

/* default advertising filter policy */
#ifndef BTM_BLE_DEFAULT_AFP
#define BTM_BLE_DEFAULT_AFP   AP_SCAN_CONN_ALL
#endif

/* scanning filter policy */
#define SP_ADV_ALL     0x00     /* 0: accept adv packet from all, directed adv pkt not directed
                                      to local device is ignored */
#define SP_ADV_WL      0x01     /* 1: accept adv packet from device in white list, directed adv
                                      packet not directed to local device is ignored */
#define SP_ADV_ALL_RPA_DIR_ADV     0x02     /* 2: accept adv packet from all, directed adv pkt
                                      not directed to me is ignored except direct adv with RPA */
#define SP_ADV_WL_RPA_DIR_ADV      0x03     /* 3: accept adv packet from device in white list,
                        directed adv pkt not directed to me is ignored except direct adv with RPA */
typedef UINT8   tBTM_BLE_SFP;

#ifndef BTM_BLE_DEFAULT_SFP
#define BTM_BLE_DEFAULT_SFP   SP_ADV_ALL
#endif

/* adv parameter boundary values */
#define BTM_BLE_ADV_INT_MIN            0x0020
#define BTM_BLE_ADV_INT_MAX            0x4000

/* connection parameter boundary values */
#define BTM_BLE_SCAN_INT_MIN            0x0004
#define BTM_BLE_SCAN_INT_MAX            0x4000
#define BTM_BLE_SCAN_WIN_MIN            0x0004
#define BTM_BLE_SCAN_WIN_MAX            0x4000
#define BTM_BLE_CONN_INT_MIN            0x0006
#define BTM_BLE_CONN_INT_MAX            0x0C80
#define BTM_BLE_CONN_LATENCY_MAX        500
#define BTM_BLE_CONN_SUP_TOUT_MIN       0x000A
#define BTM_BLE_CONN_SUP_TOUT_MAX       0x0C80
#define BTM_BLE_CONN_PARAM_UNDEF        0xffff      /* use this value when a specific value not to be overwritten */
#define BTM_BLE_CONN_SUP_TOUT_DEF       700

/* default connection parameters if not configured, use GAP recommend value for auto/selective connection */
/* default scan interval */
#ifndef BTM_BLE_SCAN_FAST_INT
#define BTM_BLE_SCAN_FAST_INT    96    /* 30 ~ 60 ms (use 60)  = 96 *0.625 */
#endif
/* default scan window for background connection, applicable for auto connection or selective conenction */
#ifndef BTM_BLE_SCAN_FAST_WIN
#define BTM_BLE_SCAN_FAST_WIN   48      /* 30 ms = 48 *0.625 */
#endif

/* default scan paramter used in reduced power cycle (background scanning) */
#ifndef BTM_BLE_SCAN_SLOW_INT_1
#define BTM_BLE_SCAN_SLOW_INT_1    2048    /* 1.28 s   = 2048 *0.625 */
#endif
#ifndef BTM_BLE_SCAN_SLOW_WIN_1
#define BTM_BLE_SCAN_SLOW_WIN_1   18      /* 11.25 ms = 18 *0.625 */
#endif

/* default scan paramter used in reduced power cycle (background scanning) */
#ifndef BTM_BLE_SCAN_SLOW_INT_2
#define BTM_BLE_SCAN_SLOW_INT_2    4096    /* 2.56 s   = 4096 *0.625 */
#endif
#ifndef BTM_BLE_SCAN_SLOW_WIN_2
#define BTM_BLE_SCAN_SLOW_WIN_2   36      /* 22.5 ms = 36 *0.625 */
#endif

/* default connection interval min */
#ifndef BTM_BLE_CONN_INT_MIN_DEF
#define BTM_BLE_CONN_INT_MIN_DEF     24      /* recommended min: 30ms  = 24 * 1.25 */
#endif
/* default connectino interval max */
#ifndef BTM_BLE_CONN_INT_MAX_DEF
#define BTM_BLE_CONN_INT_MAX_DEF     40      /* recommended max: 50 ms = 56 * 1.25 */
#endif
/* default slave latency */
#ifndef BTM_BLE_CONN_SLAVE_LATENCY_DEF
#define BTM_BLE_CONN_SLAVE_LATENCY_DEF  0      /* 0 */
#endif
/* default supervision timeout */
#ifndef BTM_BLE_CONN_TIMEOUT_DEF
#define BTM_BLE_CONN_TIMEOUT_DEF    2000
#endif

#define BTM_BLE_DIR_CONN_FALLBACK_UNDIR         1
#define BTM_BLE_DIR_CONN_FALLBACK_NO_ADV        2

#ifndef BTM_BLE_DIR_CONN_FALLBACK
#define BTM_BLE_DIR_CONN_FALLBACK   BTM_BLE_DIR_CONN_FALLBACK_UNDIR
#endif

#define BTM_CMAC_TLEN_SIZE          8                   /* 64 bits */
#define BTM_BLE_AUTH_SIGN_LEN       12                   /* BLE data signature length 8 Bytes + 4 bytes counter*/
typedef UINT8 BLE_SIGNATURE[BTM_BLE_AUTH_SIGN_LEN];         /* Device address */

#ifndef BTM_BLE_HOST_SUPPORT
#define BTM_BLE_HOST_SUPPORT        0x01
#endif

#ifndef BTM_BLE_SIMULTANEOUS_HOST
#define BTM_BLE_SIMULTANEOUS_HOST    0x01
#endif

#ifndef BTM_BLE_PRIVATE_ADDR_INT
#define BTM_BLE_PRIVATE_ADDR_INT    900           /* 15 minutes minimum for random address refreshing */
#endif

#define BTM_BLE_AD_DATA_LEN         31
/* Structure returned with Rand/Encrypt complete callback */
typedef struct
{
    UINT8   status;
    UINT8   param_len;
    UINT16  opcode;
    UINT8   param_buf[BT_OCTET16_LEN];
} tBTM_RAND_ENC;

/* General callback function for notifying an application that a synchronous
** BTM function is complete. The pointer contains the address of any returned data.
*/
typedef void (tBTM_RAND_ENC_CB)(tBTM_RAND_ENC *p1);

#define BTM_BLE_FILTER_TARGET_SCANNER       0x01
#define BTM_BLE_FILTER_TARGET_ADVR          0x00

#define BTM_BLE_POLICY_BLACK_ALL            0x00    /* relevant to both */
#define BTM_BLE_POLICY_ALLOW_SCAN           0x01    /* relevant to advertiser */
#define BTM_BLE_POLICY_ALLOW_CONN           0x02    /* relevant to advertiser */
#define BTM_BLE_POLICY_WHITE_ALL            0x03    /* relevant to both */

/* ADV data flag bit definition used for BTM_BLE_AD_TYPE_FLAG */
#define BTM_BLE_LIMIT_DISC_FLAG         (0x01 << 0)             /* bit 0 */
#define BTM_BLE_GEN_DISC_FLAG           (0x01 << 1)             /* bit 1 */
#define BTM_BLE_BREDR_NOT_SPT           (0x01 << 2)             /* bit 2*/
/* 4.1 spec adv flag for simultaneous BR/EDR+LE connection support */
#define BTM_BLE_DMT_CONTROLLER_SPT      (0x01 << 3)             /* bit 3 */
#define BTM_BLE_DMT_HOST_SPT            (0x01 << 4)             /* bit 4 */
#define BTM_BLE_NON_LIMIT_DISC_FLAG     (0x00 )         /* lowest bit unset */
#define BTM_BLE_ADV_FLAG_MASK           (BTM_BLE_LIMIT_DISC_FLAG | BTM_BLE_BREDR_NOT_SPT | BTM_BLE_GEN_DISC_FLAG)
#define BTM_BLE_LIMIT_DISC_MASK         (BTM_BLE_LIMIT_DISC_FLAG )

#define BTM_BLE_AD_BIT_DEV_NAME        (0x00000001 << 0)
#define BTM_BLE_AD_BIT_FLAGS           (0x00000001 << 1)
#define BTM_BLE_AD_BIT_MANU            (0x00000001 << 2)
#define BTM_BLE_AD_BIT_TX_PWR          (0x00000001 << 3)
#define BTM_BLE_AD_BIT_INT_RANGE       (0x00000001 << 5)
#define BTM_BLE_AD_BIT_SERVICE         (0x00000001 << 6)
#define BTM_BLE_AD_BIT_SERVICE_SOL     (0x00000001 << 7)
#define BTM_BLE_AD_BIT_SERVICE_DATA    (0x00000001 << 8)
#define BTM_BLE_AD_BIT_SIGN_DATA       (0x00000001 << 9)
#define BTM_BLE_AD_BIT_SERVICE_128SOL  (0x00000001 << 10)
#define BTM_BLE_AD_BIT_APPEARANCE      (0x00000001 << 11)
#define BTM_BLE_AD_BIT_PUBLIC_ADDR      (0x00000001 << 12)
#define BTM_BLE_AD_BIT_RANDOM_ADDR      (0x00000001 << 13)
#define BTM_BLE_AD_BIT_SERVICE_32       (0x00000001 << 4)
#define BTM_BLE_AD_BIT_SERVICE_32SOL    (0x00000001 << 14)
#define BTM_BLE_AD_BIT_PROPRIETARY      (0x00000001 << 15)
#define BTM_BLE_AD_BIT_SERVICE_128      (0x00000001 << 16)      /*128-bit Service UUIDs*/

typedef  UINT32  tBTM_BLE_AD_MASK;

#define BTM_BLE_AD_TYPE_FLAG            HCI_EIR_FLAGS_TYPE                  /* 0x01 */
#define BTM_BLE_AD_TYPE_16SRV_PART      HCI_EIR_MORE_16BITS_UUID_TYPE       /* 0x02 */
#define BTM_BLE_AD_TYPE_16SRV_CMPL      HCI_EIR_COMPLETE_16BITS_UUID_TYPE   /* 0x03 */
#define BTM_BLE_AD_TYPE_32SRV_PART      HCI_EIR_MORE_32BITS_UUID_TYPE       /* 0x04 */
#define BTM_BLE_AD_TYPE_32SRV_CMPL      HCI_EIR_COMPLETE_32BITS_UUID_TYPE   /* 0x05 */
#define BTM_BLE_AD_TYPE_128SRV_PART     HCI_EIR_MORE_128BITS_UUID_TYPE       /* 0x06 */
#define BTM_BLE_AD_TYPE_128SRV_CMPL     HCI_EIR_COMPLETE_128BITS_UUID_TYPE   /* 0x07 */
#define BTM_BLE_AD_TYPE_NAME_SHORT      HCI_EIR_SHORTENED_LOCAL_NAME_TYPE       /* 0x08 */
#define BTM_BLE_AD_TYPE_NAME_CMPL       HCI_EIR_COMPLETE_LOCAL_NAME_TYPE        /* 0x09 */
#define BTM_BLE_AD_TYPE_TX_PWR          HCI_EIR_TX_POWER_LEVEL_TYPE             /* 0x0A */
#define BTM_BLE_AD_TYPE_DEV_CLASS       0x0D
#define BTM_BLE_AD_TYPE_SM_TK           0x10
#define BTM_BLE_AD_TYPE_SM_OOB_FLAG     0x11
#define BTM_BLE_AD_TYPE_INT_RANGE       0x12
#define BTM_BLE_AD_TYPE_SOL_SRV_UUID    0x14
#define BTM_BLE_AD_TYPE_128SOL_SRV_UUID 0x15
#define BTM_BLE_AD_TYPE_SERVICE_DATA    0x16
#define BTM_BLE_AD_TYPE_PUBLIC_TARGET   0x17
#define BTM_BLE_AD_TYPE_RANDOM_TARGET   0x18
#define BTM_BLE_AD_TYPE_APPEARANCE      0x19
#define BTM_BLE_AD_TYPE_ADV_INT         0x1a
#define BTM_BLE_AD_TYPE_32SOL_SRV_UUID    0x1b  /* TBD */
#define BTM_BLE_AD_TYPE_32SERVICE_DATA    0x1c  /* TBD */
#define BTM_BLE_AD_TYPE_128SERVICE_DATA    0x1d /* TBD */

#define BTM_BLE_AD_TYPE_MANU            HCI_EIR_MANUFACTURER_SPECIFIC_TYPE      /* 0xff */
typedef UINT8   tBTM_BLE_AD_TYPE;

/* security settings used with L2CAP LE COC */
#define BTM_SEC_LE_LINK_ENCRYPTED           0x01
#define BTM_SEC_LE_LINK_PAIRED_WITHOUT_MITM 0x02
#define BTM_SEC_LE_LINK_PAIRED_WITH_MITM    0x04

/*  Min/max Preferred  number of payload octets that the local Controller
    should include in a single Link Layer Data Channel PDU. */
#define BTM_BLE_LL_DATA_PDU_MAX     0x00fb
#define BTM_BLE_LL_DATA_PDU_MIN     0x001b

/*  Preferred maximum number of microseconds that the local Controller
    should use to transmit a single Link Layer Data Channel PDU. */
#define BTM_BLE_LL_DATA_TX_TIME_MIN     0x0148
#define BTM_BLE_LL_DATA_TX_TIME_MAX     0x0848


/*  Host preferences on PHY
    */

/*  bit field that indicates the transmitter PHYs that
    the Host prefers the Controller to use.Bit number 3 -7 reserved for future.*/
#define BTM_BLE_PREFER_1M_PHY           0x01
#define BTM_BLE_PREFER_2M_PHY           0x02
#define BTM_BLE_PREFER_LELR_PHY         0x04
typedef UINT8   tBTM_BLE_HOST_PHY_PREFERENCES;

/*  The PHY_options parameter is a bit field that allows the Host to specify options
    for LE long range PHY. Default connection is with no LELR.The Controller may override any
    preferred coding (S2 coded phy for 512k speed LELR and s8 coded phy for 128K LELR) for
    transmitting on the LE Coded PHY.
    The Host may specify a preferred coding even if it prefers not to use the LE
    Coded transmitter PHY since the Controller may override the PHY preference.
    Bit 2-15 reserved for future use.
Note:-  These preferences applicable only when BTM_BLE_PREFER_LELR_PHY flag gest set */
#define BTM_BLE_PREFER_NO_LELER                0x0000
#define BTM_BLE_PREFER_LELR_125K               0x0001
#define BTM_BLE_PREFER_LELR_512K               0x0002
typedef UINT16  tBTM_BLE_HOST_LELR_PHY_PREFERENCES;

/* slave preferred connection interval range */
typedef struct
{
    UINT16  low;
    UINT16  hi;

} tBTM_BLE_INT_RANGE;

/* Service tag supported in the device */
typedef struct
{
    UINT8       num_service;
    BOOLEAN     list_cmpl;
    UINT16      *p_uuid;
} tBTM_BLE_SERVICE;

/* 32 bits Service supported in the device */
typedef struct
{
    UINT8       num_service;
    BOOLEAN     list_cmpl;
    UINT32      *p_uuid;
} tBTM_BLE_32SERVICE;

/* 128 bits Service supported in the device */
typedef struct
{
    BOOLEAN     list_cmpl;
    UINT8       uuid128[MAX_UUID_SIZE];
} tBTM_BLE_128SERVICE;

typedef struct
{
    UINT8       len;
    UINT8      *p_val;
} tBTM_BLE_MANU;


typedef struct
{
    tBT_UUID    service_uuid;
    UINT8       len;
    UINT8      *p_val;
} tBTM_BLE_SERVICE_DATA;

typedef struct
{
    UINT8       adv_type;
    UINT8       len;
    UINT8       *p_val;     /* number of len byte */
} tBTM_BLE_PROP_ELEM;

typedef struct
{
    UINT8                   num_elem;
    tBTM_BLE_PROP_ELEM      *p_elem;
} tBTM_BLE_PROPRIETARY;

typedef struct
{
    tBTM_BLE_INT_RANGE      int_range;         /* slave prefered conn interval range */
    tBTM_BLE_MANU           *p_manu;           /* manufactuer data */
    tBTM_BLE_SERVICE        *p_services;       /* services */
    tBTM_BLE_128SERVICE     *p_services_128b;  /* 128 bits service */
    tBTM_BLE_32SERVICE      *p_service_32b;     /* 32 bits Service UUID */
    tBTM_BLE_SERVICE        *p_sol_services;    /* 16 bits services Solicitation UUIDs */
    tBTM_BLE_32SERVICE      *p_sol_service_32b;    /* List of 32 bit Service Solicitation UUIDs */
    tBTM_BLE_128SERVICE     *p_sol_service_128b;    /* List of 128 bit Service Solicitation UUIDs */
    tBTM_BLE_PROPRIETARY    *p_proprietary;
    tBTM_BLE_SERVICE_DATA   *p_service_data;    /* service data */
    UINT16                  appearance;
    UINT8                   flag;
    UINT8                   tx_power;
} tBTM_BLE_ADV_DATA;

typedef struct
{
    UINT8       *p_ad_data;
    UINT16      len;
    UINT8       ad_type;
} tBTM_BLE_ADV_ELEM;

/* These are the fields returned in each device adv packet.  It
** is returned in the results callback if registered.
*/
typedef struct
{
    UINT8               conn_mode;
    tBTM_BLE_AD_MASK    ad_mask;        /* mask of the valid adv data field */
    UINT8               flag;
    UINT8               tx_power_level;
    UINT8               remote_name_len;
    UINT8               *p_remote_name;
    tBTM_BLE_SERVICE    service;
} tBTM_BLE_INQ_DATA;

enum
{
    BTM_BLE_CONN_NONE,
    BTM_BLE_CONN_AUTO,
    BTM_BLE_CONN_SELECTIVE
};
typedef UINT8   tBTM_BLE_CONN_TYPE;

/** LE inquiry result type */
typedef struct
{
    BD_ADDR                         remote_bd_addr;                         /**< Device address */
    UINT8                           ble_addr_type;                          /**< LE Address type */
    tBTM_BLE_EVT_TYPE               ble_evt_type;                           /**< Scan result event type */
    INT8                            rssi;                                   /* Set to BTM_INQ_RES_IGNORE_RSSI if  not valid */
    UINT8                           flag;
} tBTM_BLE_SCAN_RESULT;

#define BTM_BLE_MULTI_ADV_TX_POWER_MIN                     -21
#define BTM_BLE_MULTI_ADV_TX_POWER_MAX                      9
typedef INT8 tBTM_BLE_MULTI_ADV_TX_POWER;

/** Multi-advertisement type */
enum
{
    BTM_BLE_MULTI_ADVERT_CONNECTABLE_UNDIRECT_EVENT         = 0x00,
    BTM_BLE_MULTI_ADVERT_CONNECTABLE_DIRECT_EVENT           = 0x01,
    BTM_BLE_MULTI_ADVERT_DISCOVERABLE_EVENT                 = 0x02,
    BTM_BLE_MULTI_ADVERT_NONCONNECTABLE_EVENT               = 0x03,
    BTM_BLE_MULTI_ADVERT_LOW_DUTY_CYCLE_DIRECT_EVENT        = 0x04
};
typedef UINT8 tBTM_BLE_MULTI_ADV_TYPE;

/** LE Multi advertising parameter */
typedef struct
{
    UINT16                       adv_int_min;            /**< Minimum adv interval */
    UINT16                       adv_int_max;            /**< Maximum adv interval */
    tBTM_BLE_MULTI_ADV_TYPE      adv_type;               /**< Adv event type */
    tBTM_BLE_ADV_CHNL_MAP        channel_map;            /**< Adv channel map */
    tBTM_BLE_AFP                 adv_filter_policy;      /**< Advertising filter policy */
    tBTM_BLE_MULTI_ADV_TX_POWER  adv_tx_power;           /**< Adv tx power */
    BD_ADDR                      peer_bd_addr;           /**< Peer Device address */
    tBLE_ADDR_TYPE               peer_addr_type;         /**< Peer LE Address type */
    BD_ADDR                      own_bd_addr;            /**< Own LE address */
    tBLE_ADDR_TYPE               own_addr_type;          /**< Own LE Address type */
} tBTM_BLE_MULTI_ADV_PARAMS;

typedef BOOLEAN(tBTM_BLE_SEL_CBACK)(BD_ADDR random_bda,     UINT8 *p_remote_name);

/* callback function for SMP signing algorithm, signed data in little endian order with tlen bits long */
typedef void (tBTM_BLE_SIGN_CBACK)(void *p_ref_data, UINT8 *p_signing_data);
typedef void (tBTM_BLE_VERIFY_CBACK)(void *p_ref_data, BOOLEAN match);
/* random address set complete callback */
typedef void (tBTM_BLE_RANDOM_SET_CBACK)(BD_ADDR random_bda);

/**
 * Inquiry result callback.
 *
 * @param p_scan_result             : scan result data (NULL if inquiry is complete)
 * @param p_adv_data                : Advertisement data (parse using wiced_bt_ble_check_advertising_data)
 *
 * @return Nothing
 */
typedef void (tBTM_BLE_SCAN_RESULT_CBACK)(tBTM_BLE_SCAN_RESULT *p_scan_result, UINT8 *p_adv_data);


/** Host PHY preferences */
typedef struct
{
    BD_ADDR                                 remote_bd_addr;     /**< Peer Device address */
    tBTM_BLE_HOST_PHY_PREFERENCES           tx_phys;            /**< Host preference among the TX PHYs */
    tBTM_BLE_HOST_PHY_PREFERENCES           rx_phys;            /**< Host preference among the RX PHYs */
    tBTM_BLE_HOST_LELR_PHY_PREFERENCES      phy_opts;           /**< Host preference on LE coded PHY */
    UINT8                                   all_phys;           /**< Reserved for future use */
} tBTM_BLE_PHY_PREFERENCES;

/*****************************************************************************
**  EXTERNAL FUNCTION DECLARATIONS
*****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/*******************************************************************************
**
** Function         BTM_SecAddBleDevice
**
** Description      Add/modify device.  This function will be normally called
**                  during host startup to restore all required information
**                  for a LE device stored in the NVRAM.
**
** Parameters:      bd_addr          - BD address of the peer
**                  bd_name          - Name of the peer device.  NULL if unknown.
**                  dev_type         - Remote device's device type.
**                  addr_type        - LE device address type.
**
** Returns          TRUE if added OK, else FALSE
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_SecAddBleDevice(BD_ADDR bd_addr, BD_NAME bd_name,
        tBT_DEVICE_TYPE dev_type, tBLE_ADDR_TYPE addr_type);

#ifndef BTEWICED
/*******************************************************************************
**
** Function         BTM_SecAddBleKey
**
** Description      Add/modify LE device information.  This function will be
**                  normally called during host startup to restore all required
**                  information stored in the NVRAM.
**
** Parameters:      bd_addr          - BD address of the peer
**                  p_le_key         - LE key values.
**                  key_type         - LE SMP key type.
*
** Returns          TRUE if added OK, else FALSE
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_SecAddBleKey(BD_ADDR bd_addr, tBTM_LE_KEY_VALUE *p_le_key,
                                        tBTM_LE_KEY_TYPE key_type);
#endif


/*******************************************************************************
**
** Function         BTM_BleStartAdvertisements
**
** Description      This function start/stop LE advertisement with required adv mode.
**
** Parameters       advert_mode : BTM_BLE_ADVERT_OFF,
**                                BTM_BLE_ADVERT_DIRECTED_HIGH, BTM_BLE_ADVERT_DIRECTED_LOW,
**                                 BTM_BLE_ADVERT_UNDIRECTED_HIGH, BTM_BLE_ADVERT_UNDIRECTED_LOW,
**                                 BTM_BLE_ADVERT_NONCONN_HIGH, BTM_BLE_ADVERT_NONCONN_LOW
**                                 BTM_BLE_ADVERT_DISCOVERABLE_HIGH, BTM_BLE_ADVERT_DISCOVERABLE_LOW
**
**                  direct_addr_type: directed advertising peer address type. BLE_ADDR_PUBLIC or BLE_ADDR_RANDOM
**                  p_addr_ptr: directed advertising peer address. NULL if notusing directed adv
**
** Returns          status
**
*******************************************************************************/
tBTM_STATUS BTM_BleStartAdvertisements(tBTM_BLE_AVERT_MODE advert_mode, tBLE_ADDR_TYPE direct_addr_type,  BD_ADDR_PTR p_addr_ptr);

/*******************************************************************************
 *
 * Function         BTM_BleGetAdvertMode
 *
 * Description      This function get current advertising mode
 *
 * @return          Current advertising state
**
*******************************************************************************/
tBTM_BLE_AVERT_MODE BTM_BleGetAdvertMode(void);


/*******************************************************************************
**
** Function         BTM_BleSetAdvParams
**
** Description      This function is called to set advertising parameters.
**
** Parameters:       None.
**
** Returns          void
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_BleSetAdvParams(UINT16 adv_int_min, UINT16 adv_int_max,
        tBLE_BD_ADDR *p_dir_bda, tBTM_BLE_ADV_CHNL_MAP chnl_map);

/*******************************************************************************
**
** Function         BTM_BleWriteAdvRawData
**
** Description      This function is called to write advertising data.
**
** Parameters:       None.
**
** Returns          void
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_BleWriteAdvRawData(UINT8 num_elem, tBTM_BLE_ADV_ELEM *p_elem);

/*******************************************************************************
**
** Function         BTM_BleSetScanParams
**
** Description      This function is called to set Scan parameters.
**
** Parameters       adv_int_min: minimum advertising interval
**                  adv_int_max: maximum advertising interval
**                  scan_type: scan mode.
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_BleSetScanParams(UINT16 scan_interval, UINT16 scan_window,
        tBTM_BLE_SCAN_MODE scan_type);

/*******************************************************************************
**
** Function         BTM_BleWriteRawScanRsp
**
** Description      This function is called to write LE scan response.
**
** Parameters:      p_scan_rsp: scan response.
**
** Returns          status
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_BleWriteRawScanRsp(UINT8 num_elem, tBTM_BLE_ADV_ELEM *p_elem);

/*******************************************************************************
**
** Function         BTM_BleObserve
**
** Description      This procedure keep the device listening for advertising
**                  events from a broadcast device.
**
** Parameters       start: start or stop observe.
**
** Returns          void
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_BleObserve(BOOLEAN start, UINT8 duration, tBTM_BLE_SCAN_RESULT_CBACK *p_results_cb);

/*******************************************************************************
 * Function         BTM_BleScan
 *
 *                  Start LE scanning
 *
 * @param[in]       scan_type : BTM_BLE_SCAN_TYPE_NONE, BTM_BLE_SCAN_TYPE_HIGH_DUTY,  BTM_BLE_SCAN_TYPE_LOW_DUTY
 * @param[in]       duplicate_enable : TRUE or FALSE to enable or disable scan duplicate filtering
 * @param[in]       p_scan_result_cback : inquiry result callback
 *
 * @return          wiced_result_t
 *                  WICED_BT_PENDING if successfully initiated
 *                  WICED_BT_BUSY if already in progress
 *                  WICED_BT_ILLEGAL_VALUE if parameter(s) are out of range
 *                  WICED_BT_NO_RESOURCES if could not allocate resources to start the command
 *                  WICED_BT_WRONG_MODE if the device is not up.
*******************************************************************************/
BTM_API extern tBTM_STATUS  BTM_BleScan(tBTM_BLE_SCAN_TYPE scan_type, BOOLEAN duplicate_enable,
                                        tBTM_BLE_SCAN_RESULT_CBACK *p_scan_result_cback);

/*******************************************************************************
 *
 * Function         BTM_BleGetCurrentScanState
 *
 *                  Get current scan state
 *
 * @return          tBTM_BLE_SCAN_TYPE
 *                      BTM_BLE_SCAN_TYPE_NONE:         not scanning
 *                      BTM_BLE_SCAN_TYPE_HIGH_DUTY     High duty cycle scan
 *                      BTM_BLE_SCAN_TYPE_LOW_DUTY      Low duty cycle scan
*******************************************************************************/
BTM_API extern tBTM_BLE_SCAN_TYPE BTM_BleGetCurrentScanState(void);

/*******************************************************************************
**
** Function         BTM_GetDeviceIDRoot
**
** Description      This function is called to read the local device identity
**                  root.
**
** Returns          void
**                  the local device ER is copied into er
**
*******************************************************************************/
BTM_API extern void BTM_GetDeviceIDRoot(BT_OCTET16 ir);

/*******************************************************************************
**
** Function         BTM_GetDeviceEncRoot
**
** Description      This function is called to read the local device encryption
**                  root.
**
** Returns          void
**                  the local device ER is copied into er
**
*******************************************************************************/
BTM_API extern void BTM_GetDeviceEncRoot(BT_OCTET16 er);

/*******************************************************************************
**
** Function         BTM_GetDeviceDHK
**
** Description      This function is called to read the local device DHK.
**
** Returns          void
**                  the local device DHK is copied into dhk
**
*******************************************************************************/
BTM_API extern void BTM_GetDeviceDHK(BT_OCTET16 dhk);

/*******************************************************************************
**
** Function         BTM_SecurityGrant
**
** Description      This function is called to grant security process.
**
** Parameters       bd_addr - peer device bd address.
**                  res     - result of the operation BTM_SUCCESS if success,
                              BTM_MODE_UNSUPPORTED, if local device does not allow pairing;
**                            Otherwise, BTM_REPEATED_ATTEMPTS is too many attempts.
**
** Returns          None
**
*******************************************************************************/
BTM_API extern void BTM_SecurityGrant(BD_ADDR bd_addr, UINT8 res);

/*******************************************************************************
**
** Function         BTM_BlePasskeyReply
**
** Description      This function is called after Security Manager submitted
**                  passkey request to the application.
**
** Parameters:      bd_addr      - Address of the device for which passkey was requested
**                  res          - result of the operation SMP_SUCCESS if success
**                  passkey - numeric value in the range of
**                  BTM_MIN_PASSKEY_VAL(0) - BTM_MAX_PASSKEY_VAL(999999(0xF423F)).
**
*******************************************************************************/
BTM_API extern void BTM_BlePasskeyReply(BD_ADDR bd_addr, UINT8 res, UINT32 passkey);

/*******************************************************************************
**
** Function         BTM_BleConfirmReply
**
** Description      This function is called after Security Manager submitted
**                  numeric comparison request to the application.
**
** Parameters:      bd_addr      - Address of the device with which numeric
**                                 comparison was requested
**                  res          - comparison result BTM_SUCCESS if success
**
*******************************************************************************/
BTM_API extern void BTM_BleConfirmReply(BD_ADDR bd_addr, UINT8 kp_val);

/*******************************************************************************
**
** Function         BTM_BleKeyprNotif
**
** Description      This function passes keypress notificatin value to SMP by
**                  calling SMP function.
**
** Parameters:      bd_addr      - Address of the device to which keypress
**                                 notificatin has to be sent.
**                  kp_val       - keypress notification value to send.
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_BleKeyprNotif(BD_ADDR bd_addr, UINT8 kp_val);

/*******************************************************************************
**
** Function         BTM_LeOobDataReply
**
** Description      This function is called to provide the OOB data for
**                  SMP in response to BTM_LE_OOB_REQ_EVT
**
** Parameters:      bd_addr     - Address of the peer device
**                  res         - result of the operation SMP_SUCCESS if success
**                  p_data      - simple pairing Randomizer  C.
**
*******************************************************************************/
BTM_API extern void BTM_BleOobDataReply(BD_ADDR bd_addr, UINT8 res, UINT8 len, UINT8 *p_data);


/*******************************************************************************
**
** Function         BTM_BleDataSignature
**
** Description      This function is called to sign the data using AES128 CMAC
**                  algorith.
**
** Parameter        bd_addr: target device the data to be signed for.
**                  p_text: singing data
**                  len: length of the signing data
**                  signature: output parameter where data signature is going to
**                             be stored.
**
** Returns          TRUE if signing sucessul, otherwise FALSE.
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_BleDataSignature(BD_ADDR bd_addr, UINT8 *p_text, UINT16 len,
        BLE_SIGNATURE signature);

/*******************************************************************************
**
** Function         BTM_BleVerifySignature
**
** Description      This function is called to verify the data signature
**
** Parameter        bd_addr: target device the data to be signed for.
**                  p_orig:  original data before signature.
**                  len: length of the signing data
**                  counter: counter used when doing data signing
**                  p_comp: signature to be compared against.

** Returns          TRUE if signature verified correctly; otherwise FALSE.
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_BleVerifySignature(BD_ADDR bd_addr, UINT8 *p_orig,
        UINT16 len, UINT32 counter,
        UINT8 *p_comp);

/*******************************************************************************
**
** Function         BTM_ReadConnectionAddr
**
** Description      This function is called to set the local device random address
**                  .
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_ReadConnectionAddr(BD_ADDR remote_bda, BD_ADDR local_conn_addr,
        tBLE_ADDR_TYPE *p_addr_type);



/*******************************************************************************
**
** Function         BTM_ReadRemoteConnectionAddr
**
** Description      This function is read the remote device address currently used
**                  .
**
** Returns          void
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_ReadRemoteConnectionAddr(BD_ADDR pseudo_addr,
        BD_ADDR conn_addr,
        tBLE_ADDR_TYPE *p_addr_type);

/*******************************************************************************
**
** Function         BTM_BleLoadLocalKeys
**
** Description      Local local identity key, encryption root or sign counter.
**
** Parameters:      key_type: type of key, can be BTM_BLE_KEY_TYPE_ID, BTM_BLE_KEY_TYPE_ER
**                            or BTM_BLE_KEY_TYPE_COUNTER.
**                  p_key: pointer to the key.
*
** Returns          non2.
**
*******************************************************************************/
BTM_API extern void BTM_BleLoadLocalKeys(UINT8 key_type, tBTM_LOCAL_IDENTITY_KEY_DATA *p_key);

/*******************************************************************************
**
** Function         BTM_BleSetBgConnType
**
** Description      This function is called to set BLE background connection
**                  procedure type. It can be auto connection, or selective connection.
**
** Parameters       conn_type: it can be auto connection, or selective connection.
**                  p_select_cback: callback function when selective connection procedure
**                              is being used.
**
** Returns          void
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_BleSetBgConnType(tBTM_BLE_CONN_TYPE   conn_type,
        tBTM_BLE_SEL_CBACK   *p_select_cback);

/*******************************************************************************
**
** Function         BTM_BleUpdateBgConnDev
**
** Description      This function is called to add or remove a device into/from
**                  background connection procedure. The background connection
*                   procedure is decided by the background connection type, it can be
*                   auto connection, or selective connection.
**
** Parameters       add_remove: TRUE to add; FALSE to remove.
**                  remote_bda: device address to add/remove.
**
** Returns          void
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_BleUpdateBgConnDev(BOOLEAN add_remove, BD_ADDR   remote_bda);


/********************************************************
**
** Function         BTM_BleSetPrefConnParams
**
** Description      Set a peripheral's preferred connection parameters. When
**                  any of the value does not want to be updated while others
**                  do, use BTM_BLE_CONN_PARAM_UNDEF for the ones want to
**                  leave untouched.
**
** Parameters:      bd_addr          - BD address of the peripheral
**                  min_conn_int     - minimum preferred connection interval
**                  max_conn_int     - maximum preferred connection interval
**                  slave_latency    - preferred slave latency
**                  supervision_tout - preferred supervision timeout
**
** Returns          void
**
*******************************************************************************/
BTM_API extern  void BTM_BleSetPrefConnParams(BD_ADDR bd_addr,
        UINT16 min_conn_int,  UINT16 max_conn_int,
        UINT16 slave_latency, UINT16 supervision_tout);

/******************************************************************************
**
** Function         BTM_BleSetConnScanParams
**
** Description      Set scan parameters used in BLE connection request
**
** Parameters:      scan_interval    - scan interval
**                  scan_window      - scan window
**
** Returns          void
**
*******************************************************************************/
BTM_API extern  void BTM_BleSetConnScanParams(UINT16 scan_interval, UINT16 scan_window);

/*******************************************************************************
**
** Function         BTM_CheckAdvData
**
** Description      This function is called to get ADV data for a specific type.
**
** Parameters       p_adv - pointer of ADV data
**                  type   - finding ADV data type
**                  p_length - return the length of ADV data not including type
**
** Returns          pointer of ADV data
**
*******************************************************************************/
BTM_API extern  UINT8 *BTM_CheckAdvData(UINT8 *p_adv, UINT8 type, UINT8 *p_length);

/*******************************************************************************
**
** Function         BTM_ReadDevInfo
**
** Description      This function is called to read the device/address type
**                  of BD address.
**
** Parameter        remote_bda: remote device address
**                  p_dev_type: output parameter to read the device type.
**                  p_addr_type: output parameter to read the address type.
**
*******************************************************************************/
BTM_API extern void BTM_ReadDevInfo(BD_ADDR remote_bda, tBT_DEVICE_TYPE *p_dev_type,
                                    tBLE_ADDR_TYPE *p_addr_type);

/*******************************************************************************
**
** Function         BTM_BleBroadcast
**
** Description      This function is to start or stop broadcasting.
**
** Parameters       start: start or stop broadcasting.
**
** Returns          status.
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_BleBroadcast(BOOLEAN start);

/*******************************************************************************
**
** Function         BTM_BleConfigPrivacy
**
** Description      This function is called to enable or disable the privacy in
**                  the local device.
**
** Parameters       enable: TRUE to enable it; FALSE to disable it.
**
** Returns          BTM_SUCCESS privacy mode set success; otherwise failed.
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_BleConfigPrivacy(BOOLEAN enable);

/*******************************************************************************
**
** Function         BTM_BleSetConnMode
**
** Description      This function is called to set BLE connectable mode for a
**                  peripheral device.
**
** Parameters       conn_mode:  directed connectable mode, or non-directed.It can
**                              be BTM_BLE_CONNECT_EVT, BTM_BLE_CONNECT_DIR_EVT or
**                              BTM_BLE_CONNECT_LO_DUTY_DIR_EVT
**
** Returns          BTM_ILLEGAL_VALUE if controller does not support BLE.
**                  BTM_SUCCESS is status set successfully; otherwise failure.
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_BleSetConnMode(UINT8 conn_mode);



/*******************************************************************************
**
** Function         BTM_BleUpdateAdvWhitelist
**
** Description      Add or remove device from advertising white list
**
** Returns          void
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_BleUpdateAdvWhitelist(BOOLEAN add_remove, BD_ADDR emote_bda);

/*******************************************************************************
**
** Function         BTM_BleClearWhiteList
**
** Description      clear all advertising white list
**
** Returns          BOOLEAN
**
*******************************************************************************/
BOOLEAN BTM_BleClearWhiteList(void);


/*******************************************************************************
**
** Function         BTM_BleReadWhiteListSize
**
** Description      return size of white list of controller
**
** Returns          size of whitelist
**
*******************************************************************************/
UINT8 BTM_BleReadWhiteListSize(void);



/*******************************************************************************
**
** Function         BTM_BleUpdateAdvFilterPolicy
**
** Description      This function update the filter policy of advertiser.
**
** Parameter        adv_policy: advertising filter policy
**
** Return           void
*******************************************************************************/
BTM_API extern BOOLEAN BTM_BleUpdateAdvFilterPolicy(tBTM_BLE_AFP adv_policy);

/*******************************************************************************
**
** Function         BTM_UseLeLink
**
** Description      This function is to select the underneath physical link to use.
**
** Returns          TRUE to use LE, FALSE use BR/EDR.
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_UseLeLink(BD_ADDR bd_addr);

/*******************************************************************************
**
** Function         BTM_BleStackEnable
**
** Description      Enable/Disable BLE functionality on stack regarless controller
**                  capability.
**
** Parameters:      enable: TRUE to enable, FALSE to disable.
**
** Returns          TRUE if added OK, else FALSE
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_BleStackEnable(BOOLEAN enable);

/*******************************************************************************
**
** Function         BTM_GetLeSecurityState
**
** Description      This function is called to get security mode 1 flags and
**                  encryption key size for LE peer.
**
** Returns          BOOLEAN TRUE if LE device is found, FALSE otherwise.
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_GetLeSecurityState(BD_ADDR bd_addr,
        UINT8 *p_le_dev_sec_flags,
        UINT8 *p_le_key_size);

/*******************************************************************************
**
** Function         BTM_BleSecurityProcIsRunning
**
** Description      This function indicates if LE security procedure is
**                  currently running with the peer.
**
** Returns          BOOLEAN TRUE if security procedure is running, FALSE otherwise.
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_BleSecurityProcIsRunning(BD_ADDR bd_addr);

/*******************************************************************************
**
** Function         BTM_BleGetSupportedKeySize
**
** Description      This function gets the maximum encryption key size in bytes
**                  the local device can suport.
**                  record.
**
** Returns          the key size or 0 if the size can't be retrieved.
**
*******************************************************************************/
BTM_API extern UINT8 BTM_BleGetSupportedKeySize(BD_ADDR bd_addr);

/*******************************************************************************
**
** Function         BTM_SetDataChannelPDULength
**
** Description      This function is called to set maximum LL transmission packet size
**
** Returns          BTM_SUCCESS if success; otherwise failed.
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetDataChannelPDULength(BD_ADDR bd_addr, UINT16 tx_pdu_length);

/*******************************************************************************
**
** Function         BTM_BleAddAddressResolutionDB
**
** Description      add link keys to address resolution db
**
** Returns          tBTM_STATUS
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_BleAddAddressResolutionDB(tBTM_PAIRED_DEVICE_LINK_KEYS *p_linkkeys);

/*******************************************************************************
**
** Function         BTM_BleRemoveAddressResolutionDB
**
** Description      remove link keys from address resolution db
**
** Returns          tBTM_STATUS
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_BleRemoveAddressResolutionDB(tBTM_PAIRED_DEVICE_LINK_KEYS *p_linkkeys);

/*******************************************************************************
**
** Function         BTM_BleUpdateLtkSize
**
** Description      This function updates LTK size
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_BleUpdateLtkSize(BD_ADDR bd_addr, UINT8 enc_key_size);


/*******************************************************************************
**
** Function         BTM_BleReadRemoteDeviceName
**
** Description      This function initiates a read remote device name procedure
**                  and calls the callback when the process has completed.
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
BTM_API extern tBTM_STATUS  BTM_BleReadRemoteDeviceName(BD_ADDR remote_bda, tBTM_CMPL_CB *p_cb) ;
/*******************************************************************************
**
** Function         BTM_BleSetRandomStaticAddr
**
** Description      This function set random static address as local address for le connection
**
** Parameters       rs_addr, to disable use of static random address, rs_addr should be dummy address as 0's
**
** Returns          BTM_SUCCESS privacy mode set success; otherwise failed.
**
*******************************************************************************/
tBTM_STATUS BTM_BleSetStaticRandomAddr(BD_ADDR rs_addr);

/*******************************************************************************
**
** Function         BTM_BleSetPhy
**
** Description      This function allows host to configure the LE link to 1M or 2M and LE coding to be used
**
** Input Params:    tBTM_BLE_PHY_PREFERENCES      - Phy preferences
**
** Returns
**                  BTM_SUCCESS is returned if the request was successfully sent to HCI.

**                  BTM_ILLEGAL_VALUE if phy_preferences is NULL
**                  BTM_UNKNOWN_ADDR if device address is bad
**                  BTM_NO_RESOURCES if could not allocate resources to start the command
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_BleSetPhy(tBTM_BLE_PHY_PREFERENCES *phy_preferences);

#ifdef __cplusplus
}
#endif

#endif

