/*****************************************************************************
**
**  Name:          btm_int.h
**
**  Description:   this file contains the main Bluetooth Manager (BTM)
**                 internal definitions.
**
**  Copyright (c) 1999-2015, Broadcom Corp., All Rights Reserved
**  Broadcom Bluetooth Core. Proprietary and confidential.
******************************************************************************/
#ifndef BTM_INT_H
#define BTM_INT_H

#include "bt_target.h"
#include "gki.h"
#include "hcidefs.h"
#include "btm_cfg.h"

#if RFCOMM_INCLUDED == TRUE
#include "rfcdefs.h"
#endif

#if (defined(BTM_N2BT_INCLUDED) && (BTM_N2BT_INCLUDED == TRUE))
#include "btm_n2bt.h"
#endif

#include "btm_api.h"
#include "btm_coex_api.h"
#include "btu_vs_api.h"

#if (BTU_DUAL_STACK_MM_INCLUDED == TRUE) || (BTU_DUAL_STACK_BTC_INCLUDED == TRUE)
#include "uipc.h"
#include "uipc_msg.h"
#endif

#if (BLE_INCLUDED == TRUE)
#include "btm_ble_int.h"
#if (SMP_INCLUDED == TRUE)
#include "smp_api.h"
#endif
#endif

#if (BTM_CLB_INCLUDED == TRUE)
#include "btm_clb_int.h"
#endif

#if BTM_MAX_LOC_BD_NAME_LEN > 0
typedef char tBTM_LOC_BD_NAME[BTM_MAX_LOC_BD_NAME_LEN + 1];
#endif

#define  BTM_ACL_IS_CONNECTED(bda)   (btm_bda_to_acl (bda, BT_TRANSPORT_BR_EDR) != NULL)

/* Definitions for Server Channel Number (SCN) management
*/
#define BTM_MAX_SCN      PORT_MAX_RFC_PORTS

/* Define masks for supported and exception 2.0 ACL packet types
*/
#define BTM_ACL_SUPPORTED_PKTS_MASK      (HCI_PKT_TYPES_MASK_DM1        | \
                                          HCI_PKT_TYPES_MASK_DH1        | \
                                          HCI_PKT_TYPES_MASK_DM3        | \
                                          HCI_PKT_TYPES_MASK_DH3        | \
                                          HCI_PKT_TYPES_MASK_DM5        | \
                                          HCI_PKT_TYPES_MASK_DH5)

#define BTM_ACL_EXCEPTION_PKTS_MASK      (HCI_PKT_TYPES_MASK_NO_2_DH1   | \
                                          HCI_PKT_TYPES_MASK_NO_3_DH1   | \
                                          HCI_PKT_TYPES_MASK_NO_2_DH3   | \
                                          HCI_PKT_TYPES_MASK_NO_3_DH3   | \
                                          HCI_PKT_TYPES_MASK_NO_2_DH5   | \
                                          HCI_PKT_TYPES_MASK_NO_3_DH5)

/* Used to pass to btm_get_packet_types to ignore the parameter */
#define BTM_ACL_PKT_TYPES_IGNORE          0xFFFF

#define BTM_EPR_AVAILABLE(p) ((HCI_ATOMIC_ENCRYPT_SUPPORTED((p)->peer_lmp_features[HCI_EXT_FEATURES_PAGE_0]) && \
                               HCI_ATOMIC_ENCRYPT_SUPPORTED(btm_cb.devcb.local_lmp_features[HCI_EXT_FEATURES_PAGE_0])) \
                               ? TRUE : FALSE)

#define BTM_IS_BRCM_CONTROLLER() (btm_cb.devcb.local_version.manufacturer == LMP_COMPID_BROADCOM)

#define BTM_IS_CY_CONTROLLER() (btm_cb.devcb.local_version.manufacturer == LMP_COMPID_CYPRESS)


/* Define the ACL Management control structure
*/
typedef struct
{
    UINT16          hci_handle;
    BD_ADDR         remote_addr;
    tBTM_BD_NAME    remote_name;

#if BR_INCLUDED == TRUE
    UINT16          pkt_types_mask;
    UINT16          clock_offset;
    DEV_CLASS       remote_dc;
    UINT16          manufacturer;
    UINT16          lmp_subversion;
    UINT16          link_super_tout;
    BD_FEATURES     peer_lmp_features[HCI_EXT_FEATURES_PAGE_MAX + 1];    /* Peer LMP Extended features mask table for the device */
    UINT8           num_read_pages;
    UINT8           lmp_version;

#define BTM_ACL_SWKEY_STATE_IDLE                0
#define BTM_ACL_SWKEY_STATE_MODE_CHANGE         1
#define BTM_ACL_SWKEY_STATE_ENCRYPTION_OFF      2
#define BTM_ACL_SWKEY_STATE_SWITCHING           3
#define BTM_ACL_SWKEY_STATE_ENCRYPTION_ON       4
#define BTM_ACL_SWKEY_STATE_IN_PROGRESS         5
    UINT8           switch_role_state;
    UINT8           change_key_state;
#endif

    BOOLEAN         in_use;
    UINT8           link_role;
    BOOLEAN         link_up_issued;     /* True if busy_level link up has been issued */

#define BTM_ACL_ENCRYPT_STATE_IDLE              0
#define BTM_ACL_ENCRYPT_STATE_ENCRYPT_OFF       1   /* encryption turning off */
#define BTM_ACL_ENCRYPT_STATE_TEMP_FUNC         2   /* temporarily off for change link key or role switch */
#define BTM_ACL_ENCRYPT_STATE_ENCRYPT_ON        3   /* encryption turning on */
    UINT8           encrypt_state;                  /* overall BTM encryption state */

#if BTM_PWR_MGR_INCLUDED == FALSE
    UINT8           mode;
#endif /* BTM_PWR_MGR_INCLUDED */


#if BLE_INCLUDED == TRUE
    tBT_TRANSPORT   transport;
    BD_ADDR         conn_addr;              /* local device address used for this connection */
    UINT8           conn_addr_type;         /* local device address type for this connection */
    BD_ADDR         active_remote_addr;     /* remote address used on this connection */
    UINT8           active_remote_addr_type;         /* local device address type for this connection */
    BD_FEATURES     peer_le_features;       /* Peer LE Used features mask for the device */

#endif
#if BT_BRCM_VS_INCLUDED == TRUE
#if BLE_BRCM_INCLUDED == TRUE
    void    *p_rssi_cback;
#endif
#if (defined BTM_TBFC_INCLUDED && BTM_TBFC_INCLUDED == TRUE)
    TIMER_LIST_ENT  tbfc_timer_entry;   /* TBFC resume timer */
    UINT8           tbfc_resume_attempt;
#endif
    UINT32          remote_bpcs_feat;
#endif
#if defined(UCD_HID_INCLUDED) && (UCD_HID_INCLUDED == TRUE)
    TIMER_LIST_ENT  ucd_tle;   /* Timer for UCD process    */
#endif
} tACL_CONN;

/*****************************************************
** TIMER Definitions
******************************************************/
#define TT_DEV_RESET     1
#define TT_DEV_RLN       2
#define TT_DEV_RLNKP     4              /* Read Link Policy Settings */

#if (BTM_BR_SC_INCLUDED == TRUE)
#define BTM_SC_TRUE    0x01             /* set this bit if device supports SC */
#define BTM_PING_TRUE  0x02             /* set this bit if device supports ping */
#endif

/* Define the Device Management control structure
*/
typedef struct
{
    tBTM_DEV_STATUS_CB  *p_dev_status_cb;   /* Device status change callback        */
    tBTM_VS_EVT_CB      *p_vend_spec_cb[BTM_MAX_VSE_CALLBACKS];     /* Register for vendor specific events  */

    TIMER_LIST_ENT       reset_timer;
    tBTM_CMPL_CB        *p_reset_cmpl_cb;   /* Callback function to be called       */
    /* when startup of the device is done   */
    TIMER_LIST_ENT       rln_timer;
    tBTM_CMPL_CB        *p_rln_cmpl_cb;     /* Callback function to be called when  */
    /* read local name function complete    */

    TIMER_LIST_ENT       rssi_timer;
    tBTM_CMPL_CB        *p_rssi_cmpl_cb;    /* Callback function to be called when  */
    /* read rssi function completes         */
    TIMER_LIST_ENT       txpwer_timer;
    tBTM_CMPL_CB        *p_txpwer_cmpl_cb;   /* Callback function to be called when  */
    /* read inq tx power function completes  */
#if BR_INCLUDED == TRUE
    tBTM_CMPL_CB        *p_stored_link_key_cmpl_cb;   /* Read/Write/Delete stored link key    */

    TIMER_LIST_ENT       rlinkp_timer;
    tBTM_CMPL_CB        *p_rlinkp_cmpl_cb;  /* Callback function to be called when  */
    /* read link policy function completes  */
    TIMER_LIST_ENT       lnk_quality_timer;
    tBTM_CMPL_CB        *p_lnk_qual_cmpl_cb;/* Callback function to be called when  */
    /* read link quality function completes */

    TIMER_LIST_ENT       qossu_timer;
    tBTM_CMPL_CB        *p_qossu_cmpl_cb;   /* Callback function to be called when  */
    /* qos setup function completes         */

    tBTM_ROLE_SWITCH_CMPL switch_role_ref_data;
    tBTM_CMPL_CB        *p_switch_role_cb;  /* Callback function to be called when  */
    /* requested switch role is completed   */

    tBTM_CHANGE_KEY_CMPL chg_link_key_ref_data;
    tBTM_CMPL_CB        *p_chg_link_key_cb; /* Callback function to be called when  */
    /* change of link key is completed      */
#endif

    TIMER_LIST_ENT       tx_power_timer;
    tBTM_CMPL_CB        *p_tx_power_cmpl_cb;/* Callback function to be called       */

    tBTM_COEX_CBACK     *p_coex_cback;      /* Callback function to be called on coexistence command complete / status event */

    BD_ADDR              local_addr;        /* BD_ADDR of the local device          */
    tBTM_VERSION_INFO    local_version;     /* Local Version Information            */

    TIMER_LIST_ENT       lq_stat_timer;
    tBTM_CMPL_CB        *p_lq_stat_cmpl_cb;    /* Callback function to be called when  */
    /* read link quality event complete received */

    /* Local LMP Extended features mask table for the device */
    BD_FEATURES          local_lmp_features[HCI_EXT_FEATURES_PAGE_MAX + 1];

#if BLE_INCLUDED == TRUE
    BD_ADDR                 read_tx_pwr_addr;   /* read TX power target address     */
    BD_FEATURES             local_le_features;  /* Local LE Supported features mask for the device */

#if (BLE_BRCM_INCLUDED == TRUE)
#define BTM_LE_SUPPORT_STATE_SIZE   8
    UINT8                   le_supported_states[BTM_LE_SUPPORT_STATE_SIZE];
#endif
    UINT8                   local_keys_available;
    tBTM_BLE_LOCAL_ID_KEYS  id_keys;        /* local BLE ID keys                    */
    BT_OCTET16              er;             /* BLE encryption key                   */

#if BTM_BLE_CONFORMANCE_TESTING == TRUE
    BOOLEAN                 no_disc_if_pair_fail;
    BOOLEAN                 enable_test_mac_val;
    BT_OCTET8               test_mac;
    BOOLEAN                 enable_test_local_sign_cntr;
    UINT32                  test_local_sign_cntr;
#define BTM_LE_AUTH_REQ_MASK_KEEP_RFU       0xFD    /* set to 0 only RFU bit */
    /* in bonding flags */
    BOOLEAN                 keep_rfu_in_auth_req;
#endif


#endif

#define BTM_DEV_STATE_WAIT_RESET_CMPLT  0
#define BTM_DEV_STATE_WAIT_AFTER_RESET  1
#define BTM_DEV_STATE_READY             2
#define BTM_DEV_STATE_DISABLING         3

    UINT8                state;


#if (BTM_BR_SC_INCLUDED == TRUE)
    DEV_CLASS            dev_class;         /* Local device class                   */
    UINT8                loc_sec_caps;
    BOOLEAN              level0_treatm_disabled;    /* == TRUE means that Secure Connections Only mode device */
    /* rejects service level 0 connections if itself or peer  */
    /* device doesn't support secure connections              */
#endif

#define BTM_RE_READ_1ST_PAGE            0x01            /* Set it if you set at least one of "..._HOST_MAY_SUPP_..." bits */
#define BTM_HOST_MAY_SUPP_SSP           0x02
#define BTM_HOST_MAY_SUPP_LE            0x04
#define BTM_HOST_MAY_SUPP_SIMULT_BR_LE  0x08
#define BTM_RELOAD_LE_HOST_FEATURE      0x10

#if (BTM_BR_SC_INCLUDED == TRUE)
#define BTM_HOST_MAY_SUPP_SC            0x20
#endif
    UINT8               lmp_features_host_may_support;  /* The flags of LMP features host may support via BR/EDR ctrlr + BTM_RE_READ_1ST_PAGE */

#if BR_INCLUDED == TRUE
    tBTM_IO_CAP          loc_io_caps;       /* IO capability of the local device */
    BOOLEAN              loc_auth_req;      /* the auth_req flag  */

    UINT8               supported_cmds[HCI_NUM_SUPP_COMMANDS_BYTES]; /* Supported Commands bit field */
    BD_FEATURES          brcm_features;     /* Broadcom specific features bit mask  */
#endif

#if (defined BTM_TBFC_INCLUDED && BTM_TBFC_INCLUDED == TRUE)
    UINT8                tbfc_enabled;
    BOOLEAN              tbfc_scan_auto_enable;
#endif

} tBTM_DEVCB;

/* Define the structures and constants used for inquiry
*/

/* Definitions of limits for inquiries */
#define BTM_PER_INQ_MIN_MAX_PERIOD      HCI_PER_INQ_MIN_MAX_PERIOD
#define BTM_PER_INQ_MAX_MAX_PERIOD      HCI_PER_INQ_MAX_MAX_PERIOD
#define BTM_PER_INQ_MIN_MIN_PERIOD      HCI_PER_INQ_MIN_MIN_PERIOD
#define BTM_PER_INQ_MAX_MIN_PERIOD      HCI_PER_INQ_MAX_MIN_PERIOD
#define BTM_MAX_INQUIRY_LENGTH          HCI_MAX_INQUIRY_LENGTH
#define BTM_MIN_INQUIRY_LEN             0x01

#define BTM_MIN_INQ_TX_POWER    -70
#define BTM_MAX_INQ_TX_POWER    20

#if BTM_USE_INQ_RESULTS_FILTER == TRUE
typedef struct
{
    UINT32          inq_count;          /* Used for determining if a response has already been      */
    /* received for the current inquiry operation. (We do not   */
    /* want to flood the caller with multiple responses from    */
    /* the same device.                                         */
    BD_ADDR         bd_addr;
} tINQ_BDADDR;
#endif

typedef struct
{
    UINT32          time_of_resp;
    UINT32          inq_count;          /* "timestamps" the entry with a particular inquiry count   */
    /* Used for determining if a response has already been      */
    /* received for the current inquiry operation. (We do not   */
    /* want to flood the caller with multiple responses from    */
    /* the same device.                                         */
    tBTM_INQ_INFO   inq_info;
    BOOLEAN         in_use;

#if (BLE_INCLUDED == TRUE)
    BOOLEAN         scan_rsp;
#endif
} tINQ_DB_ENT;


typedef struct
{
    tBTM_CMPL_CB *p_remname_cmpl_cb;

#define BTM_EXT_RMT_NAME_TIMEOUT    40
    TIMER_LIST_ENT  rmt_name_timer_ent;

    UINT8            discoverable_mode;
    UINT8            connectable_mode;

    BD_ADDR          remname_bda;           /* Name of bd addr for active remote name request */
#define BTM_RMT_NAME_INACTIVE       0
#define BTM_RMT_NAME_EXT            0x1     /* Initiated through API */
#define BTM_RMT_NAME_SEC            0x2     /* Initiated internally by security manager */
#define BTM_RMT_NAME_INQ            0x4     /* Remote name initiated internally by inquiry */
    BOOLEAN          remname_active;        /* State of a remote name request by external API */

    tBTM_INQ_RESULTS_CB *p_inq_results_cb;

    TIMER_LIST_ENT   inq_timer_ent;
#if BTM_USE_INQ_RESULTS_FILTER == TRUE
#if (BTM_INQ_DB_INCLUDED == TRUE)
    tINQ_BDADDR     *p_bd_db;               /* Pointer to memory that holds bdaddrs */
    UINT16           num_bd_entries;        /* Number of entries in database */
    UINT16           max_bd_entries;        /* Maximum number of entries that can be stored */
#endif
#endif
#if (BTM_INQ_DB_INCLUDED == TRUE)
    tINQ_DB_ENT      inq_db[BTM_INQ_DB_SIZE];
#else
    tBTM_INQ_RESULTS inq_result_cache;
#endif
    tBTM_INQ_PARMS   inqparms;              /* Contains the parameters for the current inquiry */

#if BR_INCLUDED == TRUE
    UINT16           page_scan_window;
    UINT16           page_scan_period;
    UINT16           inq_scan_window;
    UINT16           inq_scan_period;
    UINT16           inq_scan_type;
    UINT16           page_scan_type;        /* current page scan type */

    tBTM_CMPL_CB    *p_inqfilter_cmpl_cb;   /* Called (if not NULL) after inquiry filter completed */
    UINT32           inq_counter;           /* Counter incremented each time an inquiry completes */
    /* Used for determining whether or not duplicate devices */
    /* have responded to the same inquiry */
    tBTM_INQUIRY_CMPL inq_cmpl_info;        /* Status and number of responses from the last inquiry */

    UINT16           per_min_delay;         /* Current periodic minimum delay */
    UINT16           per_max_delay;         /* Current periodic maximum delay */
    BOOLEAN          inqfilt_active;
    UINT8            pending_filt_complete_event; /* to take care of btm_event_filter_complete corresponding to */
    /* inquiry that has been cancelled*/
    UINT8            inqfilt_type;          /* Contains the inquiry filter type (BD ADDR, COD, or Clear) */

    BOOLEAN          no_inc_ssp;        /* TRUE, to stop inquiry on incoming SSP */
#endif
#define BTM_INQ_INACTIVE_STATE      0
#define BTM_INQ_CLR_FILT_STATE      1   /* Currently clearing the inquiry filter preceeding the inquiry request */
    /* (bypassed if filtering is not used)                                  */
#define BTM_INQ_SET_FILT_STATE      2   /* Sets the new filter (or turns off filtering) in this state */
#define BTM_INQ_ACTIVE_STATE        3   /* Actual inquiry or periodic inquiry is in progress */
#define BTM_INQ_REMNAME_STATE       4   /* Remote name requests are active  */

    UINT8            state;             /* Current state that the inquiry process is in */
    UINT8            inq_active;        /* Bit Mask indicating type of inquiry is active */


} tBTM_INQUIRY_VAR_ST;

/* The MSB of the clock offset field indicates that the offset is valid if TRUE */
#define BTM_CLOCK_OFFSET_VALID      0x8000

/* Define the structures needed by security management
*/

#define BTM_SEC_INVALID_HANDLE  0xFFFF

typedef UINT8 *BTM_BD_NAME_PTR;                        /* Pointer to Device name */

typedef void (tBTM_SCO_IND_CBACK)(UINT16 sco_inx) ;

/* MACROs to convert from SCO packet types mask to ESCO and back */
#define BTM_SCO_PKT_TYPE_MASK   (   HCI_PKT_TYPES_MASK_HV1      \
                                 |  HCI_PKT_TYPES_MASK_HV2      \
                                 |  HCI_PKT_TYPES_MASK_HV3)

/* Mask defining only the SCO types of an esco packet type */
#define BTM_ESCO_PKT_TYPE_MASK  (   HCI_ESCO_PKT_TYPES_MASK_HV1 \
                                 |  HCI_ESCO_PKT_TYPES_MASK_HV2 \
                                 |  HCI_ESCO_PKT_TYPES_MASK_HV3)

#define BTM_SCO_2_ESCO(scotype)  ((UINT16)(((scotype) & BTM_SCO_PKT_TYPE_MASK) >> 5))
#define BTM_ESCO_2_SCO(escotype) ((UINT16)(((escotype) & BTM_ESCO_PKT_TYPE_MASK) << 5))

/* Define masks for supported and exception 2.0 SCO packet types
*/
#define BTM_SCO_SUPPORTED_PKTS_MASK      (HCI_ESCO_PKT_TYPES_MASK_HV1       | \
                                          HCI_ESCO_PKT_TYPES_MASK_HV2       | \
                                          HCI_ESCO_PKT_TYPES_MASK_HV3       | \
                                          HCI_ESCO_PKT_TYPES_MASK_EV3       | \
                                          HCI_ESCO_PKT_TYPES_MASK_EV4       | \
                                          HCI_ESCO_PKT_TYPES_MASK_EV5)

#define BTM_SCO_EXCEPTION_PKTS_MASK      (HCI_ESCO_PKT_TYPES_MASK_NO_2_EV3  | \
                                          HCI_ESCO_PKT_TYPES_MASK_NO_3_EV3  | \
                                          HCI_ESCO_PKT_TYPES_MASK_NO_2_EV5  | \
                                          HCI_ESCO_PKT_TYPES_MASK_NO_3_EV5)


#define BTM_SCO_ROUTE_UNKNOWN       0xff

/* Define the structure that contains (e)SCO data */
typedef struct
{
    tBTM_ENH_ESCO_PARAMS    setup;
    tBTM_ESCO_DATA          data;           /* Connection complete information */
    UINT8                   hci_status;
} tBTM_ESCO_INFO;

/* Define the structure used for SCO Management
*/
typedef struct
{
    tBTM_ESCO_INFO   esco;              /* Current settings             */
#if BTM_SCO_HCI_INCLUDED == TRUE
    BUFFER_Q         xmit_data_q;       /* SCO data transmitting queue  */
#endif
    UINT16           state;             /* The state of the SCO link    */
    UINT16           hci_handle;        /* HCI Handle                   */
    BOOLEAN          is_orig;           /* TRUE if the originator       */
    BOOLEAN          rem_bd_known;      /* TRUE if remote BD addr known */

} tSCO_CONN;

/* SCO Management control block */
typedef struct
{
#if BTM_SCO_HCI_INCLUDED == TRUE
    tBTM_SCO_DATA_CB     *p_data_cb;        /* Callback for SCO data over HCI */
    UINT32               xmit_window_size; /* Total SCO window in bytes  */
#endif
    tSCO_CONN            sco_db[BTM_MAX_SCO_LINKS];
    tBTM_ENH_ESCO_PARAMS def_esco_parms;
    BD_ADDR              xfer_addr;
    UINT16               sco_disc_reason;
    BOOLEAN              esco_supported;    /* TRUE if 1.2 cntlr AND supports eSCO links */
    tBTM_SCO_TYPE        xfer_sco_type;
    tBTM_SCO_PCM_PARAM   sco_pcm_param;
    tBTM_SCO_CODEC_TYPE  codec_in_use;      /* None, CVSD, MSBC, etc. */
    tBTM_SCO_ROUTE_TYPE  sco_route;         /* HCI, PCM, or TEST */
} tSCO_CB;


#if BTM_SCO_INCLUDED == TRUE
extern void btm_sco_chk_pend_rolechange(UINT16 hci_handle);
#else
#define btm_sco_chk_pend_rolechange(hci_handle)
#endif


typedef struct
{
    BD_ADDR             pseudo_addr;    /* LE pseudo address of the device if
                                            different from device address  */
    tBLE_ADDR_TYPE      ble_addr_type;  /* LE device type: public or random address */
    tBLE_ADDR_TYPE      static_addr_type;   /* static address type */
    BD_ADDR             static_addr;    /* static address */

#define BTM_WHITE_LIST_BIT          0x01
#define BTM_RESOLVING_LIST_BIT      0x02
    UINT8               in_controller_list;   /* in controller resolving list or not */
    UINT8               resolving_list_index;
#if BTM_BLE_PRIVACY_SPT == TRUE
    BD_ADDR             cur_rand_addr;  /* current random address */

#define BTM_BLE_ADDR_PSEUDO         0   /* address index device record */
#define BTM_BLE_ADDR_RRA            1   /* cur_rand_addr */
#define BTM_BLE_ADDR_STATIC         2   /* static_addr  */
    UINT8               active_addr_type;
#endif

#if SMP_INCLUDED == TRUE
    tBTM_LE_KEY_TYPE    key_type;       /* bit mask of valid key types in record */
    tBTM_SEC_BLE_KEYS   keys;           /* LE device security info in slave rode */
#endif
} tBTM_SEC_BLE;

/*
** Define structure for Security Device Record with PID for address resolving.
*
*/
typedef struct
{
    BD_ADDR              bd_addr;           /* BD_ADDR of the device              */
    tBLE_ADDR_TYPE       ble_addr_type; /* LE address type: public or random address*/
    BOOLEAN              in_use;            /*  PID in use*/
    tBLE_ADDR_TYPE      addr_type;
    BD_ADDR             static_addr;
#if (defined BTM_BLE_HOST_ADDR_RESOLUTION && BTM_BLE_HOST_ADDR_RESOLUTION == TRUE)
    BT_OCTET16          irk;
#endif
} tBTM_SEC_PID_REC;

/*
** Define structure for Security Device Record.
** A record exists for each device authenticated with this device
*/
typedef struct
{
    void                *p_ref_data;
    tBTM_EVENT_CBACK    *p_callback;
    UINT32               timestamp;         /* Timestamp of the last connection   */
    UINT16               hci_handle;        /* Handle to connection when exists   */

    BD_ADDR              bd_addr;           /* BD_ADDR of the device              */
    tBTM_BD_NAME    sec_bd_name;        /* User friendly name of the device. (may be truncated to save space in dev_rec table) */
    DEV_CLASS            dev_class;         /* DEV_CLASS of the device            */

#if BR_INCLUDED == TRUE
    UINT16               clock_offset;      /* Latest known clock offset          */

    LINK_KEY             link_key;          /* Device link key                    */
    BD_FEATURES     features[HCI_EXT_FEATURES_PAGE_MAX + 1];           /* Features supported by the device */
    UINT8           num_read_pages;
    BOOLEAN     link_key_not_sent;      /* link key notification has not been sent waiting for name */
    UINT8       link_key_type;          /* Type of key used in pairing   */
    BOOLEAN     link_key_changed;       /* Changed link key during current connection */


#define BTM_MAX_PRE_SM4_LKEY_TYPE   BTM_LKEY_TYPE_REMOTE_UNIT /* the link key type used by legacy pairing */

#define BTM_SM4_UNKNOWN     0x00
#define BTM_SM4_KNOWN       0x10
#define BTM_SM4_TRUE        0x11
#define BTM_SM4_REQ_PEND    0x08        /* set this bit when getting remote features */
#define BTM_SM4_UPGRADE     0x04        /* set this bit when upgrading link key */
#define BTM_SM4_RETRY       0x02        /* set this bit to retry on HCI_ERR_KEY_MISSING or HCI_ERR_LMP_ERR_TRANS_COLLISION */
#define BTM_SM4_DD_ACP      0x20        /* set this bit to indicate peer initiated dedicated bonding */
#define BTM_SM4_CONN_PEND   0x40        /* set this bit to indicate accepting acl conn; to be cleared on btm_acl_created */
    UINT8       sm4;                    /* BTM_SM4_TRUE, if the peer supports SM4 */
    tBTM_IO_CAP rmt_io_caps;            /* IO capability of the peer device */
    BOOLEAN     rmt_auth_req;           /* the auth_req flag as in the IO caps rsp evt */
#if (BTM_BR_SC_INCLUDED == TRUE)
    UINT8       rmt_sec_caps;
    BOOLEAN     rem_features_needed;    /* set to true if the local device is in "Secure Connections Only" mode and it receives from the peer */
    /* HCI_IO_CAPABILITY_REQUEST_EVT before it knows peer features (needed to determine if the peer supports Secure Connections) */
#endif

#if defined(UCD_HID_INCLUDED) && (UCD_HID_INCLUDED == TRUE)
    BOOLEAN     ucd_allowed;            /* TRUE if the device supports UCD     */
    BOOLEAN     ucd_data_received;      /* TRUE if UCD data had been received     */
#endif

#define BTM_SEC_RS_NOT_PENDING          0           /* Role Switch not in progress */
#define BTM_SEC_RS_PENDING              1           /* Role Switch in progress */
#define BTM_SEC_DISC_PENDING            2           /* Disconnect is pending */
    UINT8           rs_disc_pending;

#if defined (BTM_TBFC_INCLUDED) && (BTM_TBFC_INCLUDED == TRUE)

    UINT8       scan_mode;
#endif
#endif

#define BTM_SEC_AUTHENTICATED   BTM_SEC_FLAG_AUTHENTICATED  /* 0x02 */
#define BTM_SEC_ENCRYPTED       BTM_SEC_FLAG_ENCRYPTED      /* 0x04 */
#define BTM_SEC_NAME_KNOWN      0x08
#define BTM_SEC_LINK_KEY_KNOWN  BTM_SEC_FLAG_LKEY_KNOWN /* 0x10 */
#define BTM_SEC_LINK_KEY_AUTHED BTM_SEC_FLAG_LKEY_AUTHED    /* 0x20 */
#define BTM_SEC_IN_USE          0x80
    /* LE link security flag */
#define BTM_SEC_LE_AUTHENTICATED   0x0200   /* LE link is encrypted after pairing with MITM */
#define BTM_SEC_LE_ENCRYPTED       0x0400   /* LE link is encrypted */
#define BTM_SEC_LE_NAME_KNOWN      0x0800   /* not used */
#define BTM_SEC_LE_LINK_KEY_KNOWN  0x1000   /* bonded with peer (peer LTK and/or SRK is saved) */
#define BTM_SEC_LE_LINK_KEY_AUTHED 0x2000   /* pairing is done with MITM */

    UINT16           sec_flags;          /* Current device security state      */

#define BTM_SEC_STATE_IDLE                  0
#define BTM_SEC_STATE_AUTHENTICATING        1
#define BTM_SEC_STATE_ENCRYPTING            2
#define BTM_SEC_STATE_GETTING_NAME          3
#define BTM_SEC_STATE_AUTHORIZING           4
#define BTM_SEC_STATE_SWITCHING_ROLE        5
#define BTM_SEC_STATE_DISCONNECTING         6   /* disconnecting BR/EDR */
#define BTM_SEC_STATE_DELAY_FOR_ENC         7   /* delay to check for encryption to work around controller problems */
#define BTM_SEC_STATE_DISCONNECTING_BLE     8   /* disconnecting BLE */
#define BTM_SEC_STATE_DISCONNECTING_BOTH    9   /* disconnecting BR/EDR and BLE */

    UINT8       sec_state;              /* Operating state                    */
    BOOLEAN     is_originator;          /* TRUE if device is originating connection */
#if (L2CAP_UCD_INCLUDED == TRUE)
    BOOLEAN     is_ucd;                 /* TRUE if device is sending or receiving UCD */
    /* if incoming security failed, received UCD will be discarded */
#endif
    BOOLEAN     role_master;            /* TRUE if current mode is master     */
    UINT16      security_required;      /* Security required for connection   */

#if (BLE_INCLUDED == TRUE)
    UINT16              ble_hci_handle;         /* use in DUMO connection */
    UINT8               enc_key_size_br;        /* current LK size */
    UINT8               enc_key_size_le;        /* current LTK (or STK) size */
    tBTM_SEC_BLE        ble;
    tBT_DEVICE_TYPE     device_type;
    tBTM_LE_CONN_PRAMS  conn_params;
#if SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE && BTM_CROSS_TRANSP_KEY_DERIVATION == TRUE
    BOOLEAN             new_encr_key_256;   /* Set to TRUE when the newly generated LK
                                            ** is generated from P-256.
                                            ** Link encrypted with such LK can be used
                                            ** for SM over BR/EDR.
                                            */
    BOOLEAN             no_smp_on_br;       /* if set to TRUE then SMP on BR/EDR doesn't
                                            ** work, i.e. link keys crosspairing
                                            ** SC BR/EDR->SC LE doesn't happen */
#endif
#endif


} tBTM_SEC_DEV_REC;

#define BTM_SEC_IS_SM4(sm) ((BOOLEAN)(BTM_SM4_TRUE == ((sm)&BTM_SM4_TRUE)))
#define BTM_SEC_IS_SM4_LEGACY(sm) ((BOOLEAN)(BTM_SM4_KNOWN == ((sm)&BTM_SM4_TRUE)))
#define BTM_SEC_IS_SM4_UNKNOWN(sm) ((BOOLEAN)(BTM_SM4_UNKNOWN == ((sm)&BTM_SM4_TRUE)))

#define BTM_SEC_LE_MASK    (BTM_SEC_LE_AUTHENTICATED|BTM_SEC_LE_ENCRYPTED|BTM_SEC_LE_LINK_KEY_KNOWN|BTM_SEC_LE_LINK_KEY_AUTHED)

/*
** Define device configuration structure
*/
typedef struct
{
#if BTM_MAX_LOC_BD_NAME_LEN > 0
    tBTM_LOC_BD_NAME bd_name;                    /* local Bluetooth device name */
#endif
    BOOLEAN          pin_type;                   /* TRUE if PIN type is fixed */
    UINT8            pin_code_len;               /* Bonding information */
    PIN_CODE         pin_code;                   /* PIN CODE if pin type is fixed */
    BOOLEAN          connectable;                /* If TRUE page scan should be enabled */
    UINT8            def_inq_scan_mode;          /* ??? limited/general/none */
} tBTM_CFG;

#if BTM_PWR_MGR_INCLUDED == TRUE
enum
{
    BTM_PM_ST_ACTIVE  = BTM_PM_STS_ACTIVE,
    BTM_PM_ST_HOLD    = BTM_PM_STS_HOLD,
    BTM_PM_ST_SNIFF   = BTM_PM_STS_SNIFF,
    BTM_PM_ST_PARK    = BTM_PM_STS_PARK,
    BTM_PM_ST_PENDING = BTM_PM_STS_PENDING
#if BTM_TBFC_INCLUDED == TRUE
    , BTM_PM_ST_SUSPENDED    = BTM_PM_STS_SUSPENDED
#endif
};
typedef UINT8 tBTM_PM_STATE;

enum
{
    BTM_PM_SET_MODE_EVT,    /* Set power mode API is called. */
    BTM_PM_UPDATE_EVT,
    BTM_PM_RD_MODE_EVT     /* Read power mode API is called. */
};
typedef UINT8 tBTM_PM_EVENT;

typedef struct
{
    UINT16          event;
    UINT16          len;
    UINT8           link_ind;
} tBTM_PM_MSG_DATA;

typedef struct
{
    UINT8 hci_status;
    UINT8 mode;
    UINT16 interval;
} tBTM_PM_MD_CHG_DATA;

typedef struct
{
    UINT8          pm_id;      /* the entity that calls SetPowerMode API */
    tBTM_PM_PWR_MD *p_pmd;
} tBTM_PM_SET_MD_DATA;

typedef struct
{
    void        *p_data;
    UINT8        link_ind;
} tBTM_PM_SM_DATA;

typedef struct
{
    tBTM_PM_PWR_MD req_mode[BTM_MAX_PM_RECORDS + 1]; /* the desired mode and parameters of the connection*/
    tBTM_PM_PWR_MD set_mode;  /* the mode and parameters sent down to the host controller. */
    UINT16         interval;  /* the interval from last mode change event. */
#if (BTM_SSR_INCLUDED == TRUE)
    UINT16         max_lat;   /* stored SSR maximum latency */
    UINT16         min_rmt_to;/* stored SSR minimum remote timeout */
    UINT16         min_loc_to;/* stored SSR minimum local timeout */
#endif
    tBTM_PM_STATE  state;     /* contains the current mode of the connection */
    tBTM_PM_STATE  prev_state;     /* contains the previous mode of the connection when a mode change is started */
    BOOLEAN        chg_ind;   /* a request change indication */
} tBTM_PM_MCB;

#define BTM_PM_REC_NOT_USED 0
typedef struct
{
    tBTM_PM_STATUS_CBACK *cback;/* to notify the registered party of mode change event */
    UINT8                 mask; /* registered request mask. 0, if this entry is not used */
} tBTM_PM_RCB;
#endif

enum
{
    BTM_BLI_ACL_UP_EVT,
    BTM_BLI_ACL_DOWN_EVT,
    BTM_BLI_PAGE_EVT,
    BTM_BLI_PAGE_DONE_EVT,
    BTM_BLI_INQ_EVT,
    BTM_BLI_INQ_DONE_EVT
};
typedef UINT8 tBTM_BLI_EVENT;

/* Pairing State */
enum
{
    BTM_PAIR_STATE_IDLE,                        /* Idle                                         */
    BTM_PAIR_STATE_GET_REM_NAME,                /* Getting the remote name (to check for SM4)   */
    BTM_PAIR_STATE_WAIT_PIN_REQ,                /* Started authentication, waiting for PIN req (PIN is pre-fetched) */
    BTM_PAIR_STATE_WAIT_LOCAL_PIN,              /* Waiting for local PIN code                   */
    BTM_PAIR_STATE_WAIT_NUMERIC_CONFIRM,        /* Waiting user 'yes' to numeric confirmation   */
    BTM_PAIR_STATE_KEY_ENTRY,                   /* Key entry state (we are a keyboard)          */
    BTM_PAIR_STATE_WAIT_LOCAL_OOB_RSP,          /* Waiting for local response to peer OOB data  */
    BTM_PAIR_STATE_WAIT_LOCAL_IOCAPS,           /* Waiting for local IO capabilities and OOB data */
    BTM_PAIR_STATE_INCOMING_SSP,                /* Incoming SSP (got peer IO caps when idle)    */
    BTM_PAIR_STATE_WAIT_AUTH_COMPLETE,          /* All done, waiting authentication cpmplete    */
    BTM_PAIR_STATE_WAIT_DISCONNECT              /* Waiting to disconnect the ACL                */
};
typedef UINT8 tBTM_PAIRING_STATE;

#define BTM_PAIR_FLAGS_WE_STARTED_DD    0x01    /* We want to do dedicated bonding              */
#define BTM_PAIR_FLAGS_PEER_STARTED_DD  0x02    /* Peer initiated dedicated bonding             */
#define BTM_PAIR_FLAGS_DISC_WHEN_DONE   0x04    /* Disconnect when done     */
#define BTM_PAIR_FLAGS_PIN_REQD         0x08    /* set this bit when pin_callback is called     */
#define BTM_PAIR_FLAGS_PRE_FETCH_PIN    0x10    /* set this bit when pre-fetch pin     */
#define BTM_PAIR_FLAGS_REJECTED_CONNECT 0x20    /* set this bit when rejected incoming connection  */
#define BTM_PAIR_FLAGS_WE_CANCEL_DD     0x40
#define BTM_PAIR_FLAGS_LE_ACTIVE        0x80    /* use this bit when SMP pairing is active */

#if (BTA_MIP_INCLUDED == TRUE)
/* MIP management control block */
typedef struct
{
    UINT16              mip_hndl_a2dp;              /* Connection handle for MIP A2DP   */
    UINT16              mip_hndl_3dg;               /* Connection handle for MIP 3DG    */
    tBTM_MIP_EVENTS_CB  *p_cback;                   /* Callback function */
    tBTM_MIP_QUERY_CB   *p_query_cb;                /* Device Query callback */
} tBTM_MIP_CB;
#endif

#if (BTU_DUAL_STACK_MM_INCLUDED == TRUE) || (BTU_DUAL_STACK_BTC_INCLUDED == TRUE)
/*****************************************************
** Dual Stack Definitions
******************************************************/

/* Light stack co-processor type (Multimedia or BT controller) */
enum
{
    BTM_SYNC_PEER_TYPE_NONE,
    BTM_SYNC_PEER_TYPE_MM,
    BTM_SYNC_PEER_TYPE_BTC
};
typedef UINT8 tBTM_SYNC_PEER_TYPE;

/* Control block for BTM_SYNC state machine */
typedef struct
{
    tBTM_SYNC_STACK_CBACK p_sync_stack_cback;   /* Switch callback function */

    UINT16  peer_uipc_version_major;            /* UIPC version of peer processor */
    UINT16  peer_uipc_version_minor;
    tBTM_SYNC_PEER_TYPE peer_type;              /* Co-processor type (Multimedia or BT controller) */

    UINT8   status;
    UINT8   state;
    UINT8   sync_substate;          /* substates while in SYNCHRONIZING state */
    tBTM_SW_DIR sw_dir;             /* direction of current switch */
    UINT8   cur_layer_sync;         /* layer currently being sync'd */
    tBTM_SYNC_INFO sync_info;
    UINT32  full_baudrate;          /* BB UART baudrate */
    UINT32  lite_baudrate;          /* MM UART baudrate */
#if (defined(HCILP_INCLUDED) && HCILP_INCLUDED == TRUE)
    BOOLEAN lpm_enabled;
#endif
    /* Timer */
    UINT8   timer_start_state;      /* btm_sync_sm state when the timer was started */
    TIMER_LIST_ENT timer_tle;       /* btu timer list entry for btm_sync_sm */

    tUIPC_RCV_CBACK *p_uipc_rcv_cback;
#if (BTU_DUAL_STACK_BTC_CP_INCLUDED == TRUE)
    BOOLEAN     is_cp_sent;         /* whether upper layer sent AVDT_SYNC_CP_TO_BTC_LITE_REQ 0xa0 */
#endif
} tBTM_SYNC_CB;
#endif

#if (L2CAP_UCD_INCLUDED == TRUE)

#define CONN_ORIENT_TERM                0x00    /* incoming connection oriented */
#define CONN_ORIENT_ORIG                0x01    /* outgoing connection oriented */
#define CONNLESS_TERM                   0x02    /* incoming connectionless      */
#define CONNLESS_ORIG                   0x03    /* outgoing connectionless      */
#define CONNECTION_TYPE_ORIG_MASK       0x01    /* mask for direction           */
#define CONNECTION_TYPE_CONNLESS_MASK   0x02    /* mask for connectionless or not */
typedef UINT8 CONNECTION_TYPE;

#else

#define CONN_ORIENT_TERM                FALSE
#define CONN_ORIENT_ORIG                TRUE
typedef BOOLEAN CONNECTION_TYPE;

#endif /* (L2CAP_UCD_INCLUDED == TRUE) */

/* Define a structure to hold all the BTM data
*/

#define BTM_STATE_BUFFER_SIZE  5                  /* size of state buffer */

typedef struct
{
#if (BTU_STACK_LITE_ENABLED == FALSE)
    tBTM_CFG    cfg;                        /* Device configuration */

    /****************************************************
    **      ACL Management
    ****************************************************/
#if (defined(BTU_DYNAMIC_CB_INCLUDED)  && (BTU_DYNAMIC_CB_INCLUDED == TRUE))
    tACL_CONN   *acl_db;
#else
    tACL_CONN   acl_db[MAX_L2CAP_LINKS];
#endif /* BTU_DYNAMIC_CB_INCLUDED */


#if (defined(BTM_BUSY_LEVEL_CHANGE_INCLUDED) && BTM_BUSY_LEVEL_CHANGE_INCLUDED == TRUE)
    tBTM_BL_EVENT_MASK     bl_evt_mask;
    tBTM_BL_CHANGE_CB     *p_bl_changed_cb;    /* Callback for when Busy Level changed */
#else
    tBTM_ACL_DB_CHANGE_CB *p_acl_changed_cb;    /* Callback for when ACL DB changed */
#endif

#if (BR_INCLUDED == TRUE)

#if(( RFCOMM_INCLUDED==TRUE) && ((!defined(RFCOMM_USE_EXTERNAL_SCN) || (RFCOMM_USE_EXTERNAL_SCN==FALSE))))
    UINT8       btm_scn[BTM_MAX_SCN];        /* current SCNs: TRUE if SCN is in use */
#endif

    UINT16      btm_def_link_policy;
    UINT16      btm_def_link_super_tout;

    tBTM_LSTO_CBACK       *p_lsto_cback;  /* for link supervision timeout change event */

    /* Packet types supported by the local device */
    UINT16      btm_acl_pkt_types_supported;
    UINT16      btm_sco_pkt_types_supported;

    /****************************************************
    **      Power Management
    ****************************************************/
#if BTM_PWR_MGR_INCLUDED == TRUE
    tBTM_PM_MCB pm_mode_db[MAX_L2CAP_LINKS];   /* per ACL link */
    tBTM_PM_RCB pm_reg_db[BTM_MAX_PM_RECORDS + 1]; /* per application/module */
    UINT8       pm_pend_link;  /* the index of acl_db, which has a pending PM cmd */
    UINT8       pm_pend_id;    /* the id pf the module, which has a pending PM cmd */
#endif /* BTM_PWR_MGR_INCLUDED == TRUE */

    /*****************************************************
    **      SCO Management
    *****************************************************/
#if (BTM_SCO_INCLUDED == TRUE)
    tSCO_CB             sco_cb;
#endif
#endif

    /*****************************************************
    **      Device control
    *****************************************************/
    tBTM_DEVCB  devcb;

    /*****************************************************
    **      BLE Device controllers
    *****************************************************/
#if (BLE_INCLUDED == TRUE)
    tBTM_BLE_CB             ble_ctr_cb;

    UINT16                  enc_handle;
    BT_OCTET8               enc_rand;   /* received rand value from LTK request*/
    UINT16                  ediv;       /* received ediv value from LTK request */
    UINT8                   key_size;
    UINT16                  default_tx_data_length; /* Suggested Max Tx Octets */
#endif

    /*****************************************************
    **      Inquiry
    *****************************************************/
    tBTM_INQUIRY_VAR_ST     btm_inq_vars;

    tBTM_FILTER_CB          *p_inq_filter_cb;   /* Callback that can be set if host */
    /* wants to verify inquiry filters */

    /*****************************************************
    **      Security Management
    *****************************************************/
    tBTM_EVENT_CBACK         *p_management_callback;

#define BTM_SEC_MAX_RMT_NAME_CALLBACKS  2
    tBTM_RMT_NAME_CALLBACK  *p_rmt_name_callback[BTM_SEC_MAX_RMT_NAME_CALLBACKS];

    tBTM_FILTER_CB          *p_conn_filter_cb;  /* Callback that can be set if host */
    /* wants to verify connectability filters*/
#if BR_INCLUDED == TRUE
    tBTM_SEC_DEV_REC        *p_collided_dev_rec;
    TIMER_LIST_ENT           sec_collision_tle;
    UINT32                   collision_start_time;
    UINT32                   max_collision_delay;
    BOOLEAN                  pin_type_changed;       /* pin type changed during bonding */
    UINT16                   disc_handle;   /* for legacy devices */
    UINT8                    disc_reason;   /* for legacy devices */
    UINT8                    pin_code_len;  /* for legacy devices */
    PIN_CODE                 pin_code;      /* for legacy devices */
    DEV_CLASS                connecting_dc;
    UINT8                   first_disabled_channel;
    UINT8                   last_disabled_channel;
    BUFFER_Q                page_queue;
    BOOLEAN                 paging;
#endif

    UINT8                    security_mode;
    BOOLEAN                  pairing_disabled;
    BOOLEAN                  bonding_disabled;
    BOOLEAN                  connect_only_paired;
    BOOLEAN                  security_mode_changed;  /* mode changed during bonding */
    BOOLEAN                  sec_req_pending;       /*   TRUE if a request is pending */

    tBTM_PAIRING_STATE       pairing_state; /* The current pairing state    */
    UINT8                    pairing_flags; /* The current pairing flags    */
    BD_ADDR                  pairing_bda;   /* The device currently pairing */
    TIMER_LIST_ENT           pairing_tle;   /* Timer for pairing process    */

#if (defined(BTU_DYNAMIC_CB_INCLUDED)  && (BTU_DYNAMIC_CB_INCLUDED == TRUE))
    tBTM_SEC_DEV_REC         *sec_dev_rec;
#if BTM_BLE_PRIVACY_SPT == TRUE
    tBTM_SEC_PID_REC         *sec_pid_rec;
#endif
#else
    tBTM_SEC_DEV_REC         sec_dev_rec[BTM_SEC_MAX_DEVICE_RECORDS];
#if BTM_BLE_PRIVACY_SPT == TRUE
    tBTM_SEC_PID_REC         sec_pid_rec[BTM_SEC_HOST_PRIVACY_ADDR_RESOLUTION_TABLE_SIZE];
#endif
#endif

    tBTM_MKEY_CALLBACK      *mkey_cback;

    UINT32                   dev_rec_count;      /* Counter used for device record timestamp */
    BD_ADDR                  connecting_bda;
    UINT8                   acl_disc_reason;
    UINT8                   trace_level;
#if (defined(BTM_BUSY_LEVEL_CHANGE_INCLUDED) && BTM_BUSY_LEVEL_CHANGE_INCLUDED == TRUE)
    UINT8                   num_acl;    /* num of active ACL links */
    UINT8                   busy_level; /* the current busy level */
    BOOLEAN                 is_paging;  /* TRUE, if paging is in progess */
    BOOLEAN                 is_inquiry; /* TRUE, if inquiry is in progess */
#endif

    BOOLEAN                 discing;

#endif /* BTU_STACK_LITE_ENABLED == FALSE */

#if BR_INCLUDED == TRUE
#if (BTU_DUAL_STACK_MM_INCLUDED == TRUE) || (BTU_DUAL_STACK_BTC_INCLUDED == TRUE) || (BTU_STACK_LITE_ENABLED == TRUE)
    tBTM_IPC_EVT_CBACK      *p_ipc_cback; /* Callback to recieve bta ipc events */
#endif
#if (BTU_DUAL_STACK_MM_INCLUDED == TRUE) || (BTU_DUAL_STACK_BTC_INCLUDED == TRUE)
    tBTM_SYNC_CB            sync_cb;  /* BTM swiching/syncing control block */
#endif
#if (BTU_DUAL_STACK_BTC_INCLUDED == TRUE)
    tBTM_IPC_EVT_CBACK      *p_rtipc_cback; /* Callback to recieve RT(audio routing) related ipc events */
#endif

#if (defined(BTM_N2BT_INCLUDED) && (BTM_N2BT_INCLUDED == TRUE))
    tBTM_N2BT_CB            n2bt_cb;
#endif

#if (BTA_MIP_INCLUDED == TRUE)
    tBTM_MIP_CB             mip_cb;
#endif
#endif

#if  (!defined(BT_TRACE_VERBOSE) || (BT_TRACE_VERBOSE == FALSE))
    char state_temp_buffer[BTM_STATE_BUFFER_SIZE];
#endif



    /*****************************************************
    **      Connectionless broadcast Management
    *****************************************************/
#if (BTM_CLB_INCLUDED == TRUE)
    tBTM_CLB_CB             clb_cb;
#endif

#if (defined BTM_SECURE_CONNECTIONS_CONFORMANCE_TEST && BTM_SECURE_CONNECTIONS_CONFORMANCE_TEST == TRUE)
    BOOLEAN no_sec_conns_supp_ctrlr;
    BOOLEAN no_sec_conns_supp_host;
#if SMP_LE_SC_INCLUDED == TRUE
    BOOLEAN send_smp_pair_req_for_p192;
#endif
#endif
} tBTM_CB;


#ifdef __cplusplus
extern "C"
{
#endif

#if BTM_DYNAMIC_MEMORY == FALSE
BTM_API extern tBTM_CB  btm_cb;
#else
BTM_API extern tBTM_CB *btm_cb_ptr;
#define btm_cb (*btm_cb_ptr)
#endif

/* Internal functions provided by btm_main.c
********************************************
*/
extern void         btm_init(void);

/* Internal functions provided by btm_inq.c
*******************************************
*/
extern tBTM_STATUS  btm_initiate_rem_name(BD_ADDR remote_bda,
        tBTM_INQ_RESULTS *p_cur,
        UINT8 origin, UINT32 timeout,
        tBTM_CMPL_CB *p_cb);

extern void         btm_process_remote_name(BD_ADDR bda, BD_NAME name, UINT16 evt_len,
        UINT8 hci_status);
extern void         btm_inq_rmt_name_failed(void);

/* Inquiry related functions */
extern void         btm_clr_inq_db(BD_ADDR p_bda);
extern void         btm_inq_db_init(void);
extern void         btm_process_inq_results(UINT8 *p, UINT8 inq_res_mode);
extern void         btm_process_inq_complete(UINT8 hci_status, UINT8 mode);
extern void         btm_event_filter_complete(UINT8 *p);
extern void         btm_inq_stop_on_ssp(void);
extern void         btm_inq_clear_ssp(void);
extern tINQ_DB_ENT *btm_inq_db_find(BD_ADDR p_bda);
extern BOOLEAN      btm_inq_find_bdaddr(BD_ADDR p_bda);

#if (BTM_EIR_CLIENT_INCLUDED == TRUE)
extern BOOLEAN btm_lookup_eir(BD_ADDR_PTR p_rem_addr);
#endif

/* Internal functions provided by btm_acl.c
********************************************
*/
extern void         btm_acl_init(void);
extern void         btm_acl_timeout(TIMER_LIST_ENT  *p_tle);
extern void         btm_acl_created(BD_ADDR bda, DEV_CLASS dc, BD_NAME bdn,
                                    UINT16 hci_handle, UINT8 link_role, tBT_TRANSPORT transport);
extern void         btm_acl_removed(BD_ADDR bda, tBT_TRANSPORT transport, UINT8 reason);
extern void         btm_acl_device_down(void);
extern void         btm_acl_update_busy_level(tBTM_BLI_EVENT event);
extern void         btm_acl_link_key_change(UINT16 handle, UINT8 status);

extern void         btm_cont_rswitch_or_chglinkkey(tACL_CONN *p,
        tBTM_SEC_DEV_REC *p_dev_rec,
        UINT8 hci_status);

extern UINT8        btm_handle_to_acl_index(UINT16 hci_handle);
extern void         btm_read_link_policy_complete(UINT8 *p);
extern void         btm_read_rssi_complete(UINT8 *p);
extern void         btm_read_tx_power_complete(UINT8 *p, BOOLEAN is_ble);
extern void         btm_read_link_quality_complete(UINT8 *p);
extern UINT16       btm_get_acl_packet_types(UINT16 pkt_types);
extern void         btm_process_clk_off_comp_evt(UINT16 hci_handle, UINT16 clock_offset);
extern void         btm_acl_role_changed(UINT8 hci_status, BD_ADDR bd_addr, UINT8 new_role);
extern void         btm_acl_encrypt_change(UINT16 handle, UINT8 status, UINT8 encr_enable);
BTM_API extern UINT16       btm_get_acl_disc_reason_code(void);
BTM_API extern tBTM_STATUS  btm_remove_acl(BD_ADDR bd_addr, tBT_TRANSPORT transport);
extern void         btm_read_remote_features_complete(UINT8 *p);
extern void         btm_read_remote_ext_features_complete(UINT8 *p);
extern void         btm_read_remote_ext_features_failed(UINT8 status, UINT16 handle);
extern void         btm_read_remote_version_complete(UINT8 *p);
extern void         btm_establish_continue(tACL_CONN *p_acl_cb);
extern void         btm_read_lq_stat_complete(tBTM_VSC_CMPL *p_vcs_pcm_params);

/* Read maximum data packet that can be sent over current connection */
extern UINT16 btm_get_max_packet_size(BD_ADDR addr);
extern tACL_CONN *btm_bda_to_acl(BD_ADDR bda, tBT_TRANSPORT transport);
extern BOOLEAN    btm_acl_notif_conn_collision(BD_ADDR bda);

#if defined(UCD_HID_INCLUDED) && (UCD_HID_INCLUDED == TRUE)
extern void btm_deferred_ucd_timeout(TIMER_LIST_ENT *p_tle);
#endif

#if BTM_PWR_MGR_INCLUDED == FALSE
extern void         btm_process_mode_change(UINT8 hci_status, UINT16 hci_handle, UINT8 mode,
        UINT16 interval);

/* Internal functions provided by btm_pm.c
********************************************
*/
#else
extern void btm_pm_reset(void);
extern void btm_pm_sm_alloc(UINT8 ind);
extern void btm_pm_proc_cmd_status(UINT8 status);
extern void btm_pm_proc_mode_change(UINT8 hci_status, UINT16 hci_handle, UINT8 mode,
                                    UINT16 interval);
extern void btm_pm_proc_ssr_evt(UINT8 *p, UINT16 evt_len);
#if BTM_SCO_INCLUDED == TRUE
extern void btm_sco_chk_pend_unpark(UINT8 hci_status, UINT16 hci_handle);
#else
#define btm_sco_chk_pend_unpark(hci_status, hci_handle)
#endif /* BTM_SCO_INCLUDED */
#endif /* BTM_PWR_MGR_INCLUDED == FALSE */


/* Internal functions provided by btm_sco.c
********************************************
*/
extern void btm_sco_init(void);
extern void btm_sco_connected(UINT8 hci_status, BD_ADDR bda, UINT16 hci_handle,
                              tBTM_ESCO_DATA *p_esco_data);
extern void btm_esco_proc_conn_chg(UINT8 status, UINT16 handle, UINT8 tx_interval,
                                   UINT8 retrans_window, UINT16 rx_pkt_len,
                                   UINT16 tx_pkt_len);
extern void btm_sco_conn_req(BD_ADDR bda,  DEV_CLASS dev_class, UINT8 link_type);
extern void btm_sco_removed(UINT16 hci_handle, UINT8 reason);
extern void btm_sco_acl_removed(BD_ADDR bda);
extern BOOLEAN btm_sco_is_active(BD_ADDR bda);

extern void btm_route_sco_data(BT_HDR *p_msg);
extern BOOLEAN btm_is_sco_active(UINT16 handle);
extern void btm_remove_sco_links(BD_ADDR bda);
extern BOOLEAN btm_is_sco_active_by_bdaddr(BD_ADDR remote_bda);

extern void    btm_read_def_esco_mode(tBTM_ENH_ESCO_PARAMS *p_parms);
extern UINT16  btm_find_scb_by_handle(UINT16 handle);
extern void btm_sco_flush_sco_data(UINT16 sco_inx);


/* Internal functions provided by btm_devctl.c
**********************************************
*/
extern void btm_dev_init(void);
extern void btm_dev_absent(void);
extern void btm_dev_timeout(TIMER_LIST_ENT *p_tle);
extern void btm_reset_complete(void);
extern void btm_read_local_version_complete(UINT8 *p, UINT16 evt_len);
extern void btm_read_hci_buf_size_complete(UINT8 *p, UINT16 evt_len);
extern void btm_read_local_supported_cmds_complete(UINT8 *p);
extern void btm_read_local_features_complete(UINT8 *p, UINT16 evt_len);
extern void btm_read_local_ext_features_complete(UINT8 *p, UINT16 evt_len);
extern void btm_read_local_name_complete(UINT8 *p, UINT16 evt_len);
extern void btm_read_local_addr_complete(UINT8 *p, UINT16 evt_len);
extern  void btm_reset_ctrlr_complete(void);
extern void btm_write_simple_paring_mode_complete(UINT8 *p);
extern void btm_write_le_host_supported_complete(UINT8 *p);
extern void btm_get_local_ext_features(UINT8 page_number);
#if (BTM_BR_SC_INCLUDED == TRUE)
extern void btm_write_secure_conns_host_support_complete(UINT8 *p);
#if (defined BTM_SECURE_CONNECTIONS_CONFORMANCE_TEST && BTM_SECURE_CONNECTIONS_CONFORMANCE_TEST == TRUE)
BTM_API extern void btm_no_sec_conns_support_by_host(BOOLEAN set_no_support_for_sec_conns);
#if SMP_LE_SC_INCLUDED == TRUE
BTM_API extern void btm_set_smp_pair_req_for_p192(BOOLEAN send_smp_pair_req_for_p192);
#endif /* SMP_LE_SC_INCLUDED == TRUE */
#endif
#endif /* BTM_BR_SC_INCLUDED == TRUE */

#if BR_INCLUDED == TRUE
extern void btm_sec_bond_cancel_complete(void);
extern void btm_acl_report_role_change(UINT8 hci_status, BD_ADDR bda);
extern void btm_sec_connect_after_reject_timeout(TIMER_LIST_ENT *p_tle);
extern void btm_sec_br_pairing_timeout(tBTM_SEC_DEV_REC *p_dev_rec, tBTM_PAIRING_STATE pairing_state);
extern void btm_sec_pairing_complete_notification(BD_ADDR bd_addr, UINT8 *dev_class, UINT8 *bd_name, UINT8 hci_status);
extern tBTM_STATUS btm_sec_send_hci_disconnect(tBTM_SEC_DEV_REC *p_dev_rec, UINT8 reason, UINT16 conn_handle);
extern void btm_restore_mode(void);
extern tBTM_STATUS btm_br_sec_bond(tBTM_SEC_DEV_REC *p_dev_rec, BD_ADDR bd_addr, UINT8 pin_len, UINT8 *p_pin);
extern void btm_sec_auth_collision(UINT16 handle);
#endif

#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE && BTM_CROSS_TRANSP_KEY_DERIVATION == TRUE
extern BOOLEAN btm_sec_use_smp_br_chnl(tBTM_SEC_DEV_REC *p_dev_rec);
extern BOOLEAN btm_sec_is_master(tBTM_SEC_DEV_REC *p_dev_rec);
#endif


#if (BLE_INCLUDED == TRUE)
extern void btm_read_ble_buf_size_complete(UINT8 *p, UINT16 evt_len);
extern void btm_read_ble_local_supported_features_complete(UINT8 *p, UINT16 evt_len);
extern void btm_read_white_list_size_complete(UINT8 *p, UINT16 evt_len);
extern void btm_ble_add_2_white_list_complete(UINT8 status);
extern void btm_ble_remove_from_white_list_complete(UINT8 *p, UINT16 evt_len);
extern void btm_ble_clear_white_list_complete(UINT8 *p, UINT16 evt_len);
extern void btm_set_ble_host_chnl_class_cmplt(UINT8 *p, UINT8 ctr_id);
extern tBTM_SEC_PID_REC *btm_ble_find_pid_db_by_identity_addr(BD_ADDR id_addr);
#if (defined BLE_DATA_LEN_EXT_INCLUDED) && (BLE_DATA_LEN_EXT_INCLUDED == TRUE)
extern void btm_read_ble_default_data_length_complete(UINT8 *p, UINT16 evt_len);
#endif

#if (defined BTM_BLE_HOST_ADDR_RESOLUTION && BTM_BLE_HOST_ADDR_RESOLUTION == TRUE)
extern tBTM_SEC_PID_REC *btm_ble_find_pid_db(BD_ADDR bd_addr);
#endif
extern void btm_ble_update_resolving_list(BD_ADDR pseudo_bda, BOOLEAN add);

/* LE security function from btm_sec.c */

#if SMP_INCLUDED == TRUE
extern void btm_ble_link_sec_check(BD_ADDR bd_addr, tBTM_LE_AUTH_REQ auth_req, tBTM_BLE_SEC_REQ_ACT *p_sec_req_act);
extern void btm_ble_ltk_request_reply(BD_ADDR bda,  BOOLEAN use_stk, BT_OCTET16 stk);
extern UINT8 btm_proc_smp_cback(tSMP_EVT event, BD_ADDR bd_addr, tSMP_EVT_DATA *p_data);
extern tBTM_STATUS btm_ble_set_encryption(tACL_CONN *p_acl, void *p_ref_data);
extern void btm_ble_ltk_request(UINT16 handle, UINT8 rand[8], UINT16 ediv);
extern tBTM_STATUS btm_ble_start_encrypt(BD_ADDR bda, BOOLEAN use_stk, BT_OCTET16 stk);
extern void btm_ble_link_encrypted(BD_ADDR bd_addr, UINT8 encr_enable, tBTM_STATUS res);
#endif /* SMP_INCLUDED */

#endif

extern void btm_set_afh_host_chnl_class_cmplt(UINT8 *p, UINT8 ctr_id);
extern void btm_set_mws_chan_params_cmplt(UINT8 *p, UINT8 ctr_id);
extern void btm_set_extrn_frame_cfg_cmplt(UINT8 *p, UINT8 ctr_id);
extern void btm_set_mws_signaling_cmplt(UINT8 *p, UINT8 ctr_id);
extern void btm_set_mws_trans_layer_cmplt(UINT8 *p, UINT8 ctr_id);
extern void btm_set_mws_scan_freq_table_cmplt(UINT8 *p, UINT8 ctr_id);
extern void btm_set_mws_pattern_cfg_cmplt(UINT8 *p, UINT8 ctr_id);
extern void btm_get_mws_trans_layer_cmplt(UINT8 *p, UINT8 ctr_id);
extern void btm_coexist_status_fail(UINT16 opcode, UINT8 status, UINT8 ctr_id);

/* Vendor Specific Command complete evt handler */
extern void btm_vsc_complete(UINT8 *p, UINT16 cc_opcode, UINT16 evt_len,
                             tBTM_CMPL_CB *p_vsc_cplt_cback);
extern void btm_inq_db_reset(void);
extern void btm_vendor_specific_evt(UINT8 *p, UINT8 evt_len);
extern UINT8 btm_get_hci_version(void);
extern void btm_read_stored_link_key_complete(UINT8 *p);
extern void btm_write_stored_link_key_complete(UINT8 *p);
extern void btm_delete_stored_link_key_complete(UINT8 *p);
extern void btm_return_link_keys_evt(tBTM_RETURN_LINK_KEYS_EVT *result);
extern void btm_report_device_status(tBTM_DEV_STATUS status);


/* Internal functions provided by btm_dev.c
**********************************************
*/
extern BOOLEAN btm_dev_support_switch(BD_ADDR bd_addr);
extern UINT8 btm_get_voice_coding_support(void);

extern tBTM_SEC_DEV_REC  *btm_sec_alloc_dev(BD_ADDR bd_addr);
extern void               btm_sec_free_dev(tBTM_SEC_DEV_REC *p_dev_rec);
extern tBTM_SEC_DEV_REC  *btm_find_dev(BD_ADDR bd_addr);
extern tBTM_SEC_DEV_REC  *btm_find_or_alloc_dev(BD_ADDR bd_addr);
extern tBTM_SEC_DEV_REC  *btm_find_dev_by_handle(UINT16 handle);

/* Internal functions provided by btm_sec.c
**********************************************
*/
extern BOOLEAN btm_dev_support_switch(BD_ADDR bd_addr);
extern tBTM_STATUS  btm_sec_l2cap_access_req(BD_ADDR bd_addr, UINT16 psm,
        UINT16 handle, CONNECTION_TYPE conn_type,
        tBTM_EVENT_CBACK *p_callback, void *p_ref_data);
extern void btm_sec_conn_req(UINT8 *bda, UINT8 *dc);
extern void btm_create_conn_cancel_complete(UINT8 *p);
extern void btm_proc_lsto_evt(UINT16 handle, UINT16 timeout);
extern void btm_read_inq_tx_power_complete(UINT8 *p);

extern void  btm_sec_init(UINT8 sec_mode);
extern void  btm_sec_dev_reset(void);
extern void  btm_sec_abort_access_req(BD_ADDR bd_addr);
extern void  btm_sec_change_link_key(UINT16 handle);
extern void  btm_sec_auth_complete(UINT16 handle, UINT8 hci_status);
extern void  btm_sec_mkey_comp_event(UINT16 handle, UINT8 status, UINT8 key_flg);
extern void  btm_sec_encrypt_change(UINT16 handle, UINT8 status, UINT8 encr_enable);
extern void  btm_sec_connected(UINT8 *bda, UINT16 handle, UINT8 status, UINT8 enc_mode);
extern tBTM_STATUS btm_sec_disconnect(UINT16 handle, UINT8 reason);
extern void  btm_sec_disconnected(UINT16 handle, UINT8 reason);
extern void  btm_sec_rmt_name_request_complete(UINT8 *bd_addr, UINT8 *bd_name, UINT8 status);
extern void  btm_sec_rmt_host_support_feat_evt(UINT8 *p);
extern void  btm_io_capabilities_req(UINT8 *p);
extern void  btm_io_capabilities_rsp(UINT8 *p);
extern void  btm_proc_sp_req_evt(tBTM_EVENT event, UINT8 *p);
extern void  btm_keypress_notif_evt(UINT8 *p);
extern void  btm_simple_pair_complete(UINT8 *p);
extern void  btm_sec_link_key_notification(UINT8 *p_bda, UINT8 *p_link_key, UINT8 key_type);
extern void  btm_sec_link_key_request(UINT8 *p_bda);
extern void  btm_sec_pin_code_request(UINT8 *p_bda);
extern void  btm_sec_update_clock_offset(UINT16 handle, UINT16 clock_offset);
extern void  btm_sec_dev_rec_cback_event(tBTM_SEC_DEV_REC *p_dev_rec, tBTM_STATUS res, BOOLEAN is_le_trasnport);
extern void  btm_send_link_key_notif(tBTM_SEC_DEV_REC *p_dev_rec, BOOLEAN is_le_trasnport);
extern tBTM_STATUS btm_set_encryption(UINT8 *p_bda, tBT_TRANSPORT transport, tBTM_EVENT_CBACK *p_callback, void *p_ref_data);
extern tBTM_STATUS btm_sec_execute_procedure(tBTM_SEC_DEV_REC *p_dev_rec);
extern void  btm_sec_auth_payload_tout(UINT8 *p, UINT16 hci_evt_len);
extern void     btm_sec_change_pairing_state(tBTM_PAIRING_STATE new_state);
#if (BT_USE_TRACES == TRUE)
char     *btm_pair_state_descr(tBTM_PAIRING_STATE state);
#endif
#if (BTM_BR_SC_INCLUDED == TRUE)
extern void btm_sec_set_peer_sec_caps(tACL_CONN *p_acl_cb, tBTM_SEC_DEV_REC *p_dev_rec);
extern void btm_sec_set_local_sec_caps(void);
#endif

#if BLE_INCLUDED == TRUE
extern tBTM_STATUS btm_ble_sec_bond(tBTM_SEC_DEV_REC *p_dev_rec, BD_ADDR bd_addr);
extern void  btm_sec_clear_ble_keys(tBTM_SEC_DEV_REC  *p_dev_rec);
extern BOOLEAN btm_sec_is_a_bonded_dev(BD_ADDR bda);
extern void btm_consolidate_dev(tBTM_SEC_DEV_REC *p_target_rec);
#if SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE && SMP_CONFORMANCE_TESTING == TRUE
BTM_API extern BOOLEAN btm_set_peer_sc_notif(BD_ADDR bd_addr, BOOLEAN enable);
#endif
#if BTM_BLE_PRIVACY_SPT == TRUE
extern BOOLEAN btm_ble_addr_resolvable(BD_ADDR rpa, tBTM_SEC_DEV_REC *p_dev_rec);
extern tBTM_STATUS btm_ble_read_resolving_list_entry(tBTM_SEC_DEV_REC *p_dev_rec);
extern BOOLEAN btm_ble_resolving_list_load_dev(tBTM_SEC_DEV_REC *p_dev_rec);
extern void btm_ble_resolving_list_remove_dev(tBTM_SEC_DEV_REC *p_dev_rec);
extern tBTM_STATUS btm_ble_clear_resolving_list(void);

#endif  /* end of BTM_BLE_PRIVACY_SPT*/
extern BOOLEAN btm_ble_init_pseudo_addr(tBTM_SEC_DEV_REC *p_dev_rec, BD_ADDR new_pseudo_addr);
#endif /* BLE_INCLUDED */

extern tINQ_DB_ENT *btm_inq_db_new(BD_ADDR p_bda);

#if BTM_OOB_INCLUDED == TRUE
extern void  btm_rem_oob_req(UINT8 *p);
extern void  btm_read_local_oob_complete(UINT8 *p);
#if (BTM_BR_SC_INCLUDED == TRUE)
extern void  btm_read_local_oob_ext_data_complete(UINT8 *p);
#endif
#else
#define btm_rem_oob_req(p)
#define btm_read_local_oob_complete(p)
#if (BTM_BR_SC_INCLUDED == TRUE)
#define btm_read_local_oob_ext_data_complete(p)
#endif
#endif
extern void  btm_acl_resubmit_page(void);
extern void  btm_acl_reset_paging(void);
extern void  btm_acl_paging(BT_HDR *p, BD_ADDR dest);
extern void  btm_acl_set_discing(BOOLEAN discing);
extern UINT8 btm_sec_clr_service_by_psm(UINT16 psm);

/* Internal functions provided by btm_sync.c (for Dual Stack support)
**********************************************
*/
#if (BTU_DUAL_STACK_MM_INCLUDED == TRUE) || (BTU_DUAL_STACK_BTC_INCLUDED == TRUE)
extern void btm_sync_sm_execute(UINT8 event, void *p_data);

extern void btm_sync_transport_error_evt(void);
extern void btm_sync_init(void);
extern void btm_sync_timeout(TIMER_LIST_ENT *p_tle);
extern void btm_sync_handle_open_rsp(tUIPC_MSG *p_mgmt_msg);
extern void btm_sync_handle_close_rsp(void);

/* events for tBTM_SYNC_NOTIFY_FCN */
#define BTM_SYNC_NOTIFY_BB2MM_SWITCH_EVT    0
#define BTM_SYNC_NOTIFY_MM2BB_SWITCH_EVT    1
#define BTM_SYNC_NOTIFY_RESYNC_EVT          2
#define BTM_SYNC_NOTIFY_SYNC_CPLT_EVT       3
#define BTM_SYNC_NOTIFY_RESYNC_CPLT_EVT     4
#define BTM_SYNC_NOTIFY_BB2BTC_SWITCH_EVT   5
#define BTM_SYNC_NOTIFY_BTC2BB_SWITCH_EVT   6

typedef BOOLEAN(*tBTM_SYNC_NOTIFY_FCN)(UINT8 event);
typedef void (*tBTM_SYNC_REQ_FCN)(tBTM_SYNC_INFO *p_btm_sync_info, tBTM_SYNC_CPLT_CBACK p_sync_cplt_cback);
typedef void (*tBTM_SYNC_RESET_FCN)(void);

#if (BTU_DUAL_STACK_BTC_INCLUDED == TRUE)
extern  tBTM_SYNC_PEER_TYPE btm_get_peer_type(void);
extern void btm_ipc_process_vse_cback(tBTM_VSC_CMPL *p_params);
#endif

#endif

/* Internal functions provided by btm_ipc.c (for Dual Stack support)
**********************************************
*/
#if (BTU_DUAL_STACK_MM_INCLUDED == TRUE) || (BTU_DUAL_STACK_BTC_INCLUDED == TRUE) || (BTU_STACK_LITE_ENABLED == TRUE)

extern void btm_ipc_msg_handler(BT_HDR *p_msg);
extern void btm_ipc_process_vse(UINT8 evt_len, UINT8 *p);

#ifndef BTM_UIPC_DUAL_STACK_CHNB
#define BTM_UIPC_DUAL_STACK_CHNB UIPC_CH_ID_0
#endif

/* If host only supports MM light stacks, then map UIPC_XXX functions directly to the UIPC wrapper functions */
#if (!defined(BTU_DUAL_STACK_BTC_INCLUDED) || (BTU_DUAL_STACK_BTC_INCLUDED==FALSE))

#define UIPC_INIT( p )                UIPC_Init((void *)p)
#define UIPC_OPEN( dest, p_cback )    UIPC_Open(BTM_UIPC_DUAL_STACK_CHNB, (tUIPC_RCV_CBACK *)p_cback)
#define UIPC_CLOSE()                  UIPC_Close(BTM_UIPC_DUAL_STACK_CHNB)
#define UIPC_SENDBUF(p_msg)           UIPC_SendBuf(BTM_UIPC_DUAL_STACK_CHNB, (BT_HDR *)p_msg)
#define UIPC_SEND(evt, p_buf, length) UIPC_Send(BTM_UIPC_DUAL_STACK_CHNB, (UINT16)evt, (UINT8*)p_buf, (UINT16)length)

/* If host supports both MM and BTC light stacks, then map UIPC_XXX functions to btm_uipc_XXX so */
/* that the UIPC messages can be sent to the correct light stack */
#else
BT_API extern void btm_uipc_init(void *p);
BT_API extern BOOLEAN btm_uipc_open(tBTM_SW_DIR dir, tUIPC_RCV_CBACK *p_cback);
BT_API extern void btm_uipc_close(void);
BT_API extern BOOLEAN btm_uipc_send_buf(BT_HDR *p_msg);
BT_API extern BOOLEAN btm_uipc_send(UINT16 msg_evt, UINT8 *p_buf, UINT16 msglen);

#define UIPC_INIT( p )                btm_uipc_init((void *)p)
#define UIPC_OPEN( dest, p_cback )    btm_uipc_open((UINT8)dest, (tUIPC_RCV_CBACK *)p_cback)
#define UIPC_CLOSE()                  btm_uipc_close()
#define UIPC_SENDBUF(p_msg)           btm_uipc_send_buf((BT_HDR *)p_msg)
#define UIPC_SEND(evt, p_buf, length) btm_uipc_send((UINT16)evt, (UINT8*)p_buf, (UINT16)length)
#endif /* (BTU_DUAL_STACK_BTC_INCLUDED == TRUE) */

#endif

#if (BTA_MIP_INCLUDED == TRUE)
extern BOOLEAN btm_process_mip_vse(UINT8 *p, UINT8 evt_len);
extern UINT16 btm_get_mip_handle(UINT8 mip_mode);
#endif

#if (defined BLE_INCLUDED) && (BLE_INCLUDED == TRUE)
extern void btm_ble_notify_conn_param_update(UINT8 *p);
extern void btm_ble_notify_conn_param_response(UINT8 status);
extern UINT8 get_bd_addr_by_hci_handle(UINT16 conn_handle, BD_ADDR bda);
extern void btm_ble_phy_update_evt(tBTM_EVENT_DATA *p_event_data);
#endif

#ifdef __cplusplus
}
#endif

#endif

