/*****************************************************************************
**
**  Name:          btm_ble_int.h
**
**  Description:   this file contains the main Bluetooth Manager (BTM)
**                 internal definitions.
**
**  Copyright (c) 1999-2015, Broadcom Corp., All Rights Reserved
**  Broadcom Bluetooth Core. Proprietary and confidential.
******************************************************************************/

#ifndef BTM_BLE_INT_H
#define BTM_BLE_INT_H

#include "bt_target.h"
#include "gki.h"
#include "hcidefs.h"
#include "btm_ble_api.h"
#include "btm_int.h"

#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE
#include "smp_api.h"
#endif

/* scanning enable status */
#define BTM_BLE_SCAN_ENABLE      0x01
#define BTM_BLE_SCAN_DISABLE     0x00

/* advertising enable status */
#define BTM_BLE_ADV_ENABLE     0x01
#define BTM_BLE_ADV_DISABLE    0x00

/* use the high 4 bits unused by inquiry mode */
#define BTM_BLE_SELECT_SCAN     0x20
#define BTM_BLE_NAME_REQUEST    0x40
#define BTM_BLE_OBSERVE         0x80

#define BTM_BLE_MAX_WL_ENTRY        1
#define BTM_BLE_AD_DATA_LEN         31

#define BTM_BLE_ENC_MASK    0x03

#define BTM_BLE_DUPLICATE_ENABLE        1
#define BTM_BLE_DUPLICATE_DISABLE       0

#define BTM_BLE_GAP_DISC_SCAN_INT   18         /* Interval(scan_int) = 11.25 ms= 0x0010 * 0.625 ms */
#define BTM_BLE_GAP_DISC_SCAN_WIN   18         /* scan_window = 11.25 ms= 0x0010 * 0.625 ms */
#define BTM_BLE_GAP_LIM_TOUT        180          /* Tgap(lim_timeout) = 180s max */

#define BTM_BLE_GAP_ADV_FAST_INT_1         48         /* TGAP(adv_fast_interval1) = 30(used) ~ 60 ms  = 48 *0.625 */
#define BTM_BLE_GAP_ADV_FAST_INT_2         160         /* TGAP(adv_fast_interval2) = 100(used) ~ 150 ms = 160 * 0.625 ms */
#define BTM_BLE_GAP_ADV_SLOW_INT           2048         /* Tgap(adv_slow_interval) = 1.28 s= 512 * 0.625 ms */
#define BTM_BLE_GAP_ADV_DIR_MAX_INT        800         /* Tgap(dir_conn_adv_int_max) = 500 ms = 800 * 0.625 ms */
#define BTM_BLE_GAP_ADV_DIR_MIN_INT        400         /* Tgap(dir_conn_adv_int_min) = 250 ms = 400 * 0.625 ms */

#define BTM_BLE_GAP_FAST_ADV_TOUT          30

#define BTM_BLE_SEC_REQ_ACT_NONE           0
#define BTM_BLE_SEC_REQ_ACT_ENCRYPT        1 /* encrypt the link using current key or key refresh */
#define BTM_BLE_SEC_REQ_ACT_PAIR           2
#define BTM_BLE_SEC_REQ_ACT_DISCARD        3 /* discard the sec request while encryption is started but not completed */
typedef UINT8   tBTM_BLE_SEC_REQ_ACT;

#define BLE_STATIC_PRIVATE_MSB_MASK          0x3f
#define BLE_RESOLVE_ADDR_MSB                 0x40   /*  most significant bit, bit7, bit6 is 01 to be resolvable random */
#define BLE_RESOLVE_ADDR_MASK                0xc0   /* bit 6, and bit7 */
#define BTM_BLE_IS_RESOLVE_BDA(x)           ((x[0] & BLE_RESOLVE_ADDR_MASK) == BLE_RESOLVE_ADDR_MSB)

/* LE scan activity bit mask */
#define BTM_BLE_INQUIRY_NONE            0x00
#define BTM_BLE_SCAN_HIGH_DUTY          0x01
#define BTM_BLE_GENERAL_INQUIRY         0x10
#define BTM_BLE_LIMITED_INQUIRY         0x20
#define BTM_BLE_INQUIRY_MASK            (BTM_BLE_GENERAL_INQUIRY|BTM_BLE_LIMITED_INQUIRY)
#define BTM_BLE_SELECT_CONN_ACTIVE        0x40     /* selection connection is in progress */
#define BTM_BLE_OBSERVE_ACTIVE            0x80     /* observe is in progress */

/* BLE scan activity mask checking */
#define BTM_BLE_IS_SCAN_ACTIVE(x)           ((x) & BTM_BLE_SCAN_ACTIVE_MASK)
#define BTM_BLE_IS_INQ_ACTIVE(x)            ((x) & BTM_BLE_INQUIRY_MASK)
#define BTM_BLE_IS_OBS_ACTIVE(x)            ((x) & BTM_BLE_OBSERVE_ACTIVE)
#define BTM_BLE_IS_SEL_CONN_ACTIVE(x)       ((x) & BTM_BLE_SELECT_CONN_ACTIVE)
#define BTM_BLE_IS_SCAN_HIGH_DUTY_ACTIVE(x) ((x) & BTM_BLE_SCAN_HIGH_DUTY)

/* BLE ADDR type ID bit */
#define BLE_ADDR_TYPE_ID_BIT           0x02

typedef struct
{
    UINT32          inq_count;          /* Used for determining if a response has already been      */
    /* received for the current inquiry operation. (We do not   */
    /* want to flood the caller with multiple responses from    */
    /* the same device.                                         */
    BOOLEAN         scan_rsp;
    tBLE_BD_ADDR    le_bda;
} tINQ_LE_BDADDR;

#define BTM_BLE_ADV_DATA_LEN_MAX        31
#define BTM_BLE_CACHE_ADV_DATA_MAX      62

#define BTM_BLE_VALID_PRAM(x, min, max)  (((x) >= (min) && (x) <= (max)) || ((x) == BTM_BLE_CONN_PARAM_UNDEF))

typedef struct
{

    UINT16           scan_window;
    UINT16           scan_interval;
    UINT8            duplicate_enable;
    UINT8            scan_type;        /* current scan type: active or passive */
    UINT8            scan_duplicate_filter; /* duplicate filter enabled for scan */
    UINT16           adv_interval_min;
    UINT16           adv_interval_max;
    tBTM_BLE_AFP     afp;               /* advertising filter policy */
    tBTM_BLE_SFP     sfp;               /* scanning filter policy */

    tBLE_ADDR_TYPE   adv_addr_type;
    UINT8            evt_type;
    UINT8            adv_mode;
    tBLE_BD_ADDR     direct_bda;
    tBTM_BLE_AVERT_MODE advert_mode;   /* current advertisement mode */

    TIMER_LIST_ENT   adv_mode_timer;

    UINT8            adv_len;
    UINT8            adv_data_cache[BTM_BLE_CACHE_ADV_DATA_MAX];

    /* inquiry BD addr database */
    UINT8               num_bd_entries;
    UINT8               max_bd_entries;

    tBTM_BLE_ADV_CHNL_MAP  adv_chnl_map;

    TIMER_LIST_ENT   inq_timer_ent;
    BOOLEAN          scan_rsp;
    UINT8            state;             /* Current state that the inquiry process is in */
} tBTM_BLE_INQ_CB;


/* random address resolving complete callback */
#if (defined BTM_BLE_HOST_ADDR_RESOLUTION && BTM_BLE_HOST_ADDR_RESOLUTION == TRUE)
typedef void (tBTM_BLE_RESOLVE_CBACK)(void *match_rec, void *pid_rec, void *p);
#endif

typedef void (tBTM_BLE_ADDR_CBACK)(BD_ADDR_PTR static_random, void *p);

/* random address management control block */
typedef struct
{
    tBLE_ADDR_TYPE              own_addr_type;         /* local device LE address type */
    BD_ADDR                     private_addr;
    BD_ADDR                     random_bda;
    BOOLEAN                     busy;
    UINT16                      index;
#if (defined BTM_BLE_HOST_ADDR_RESOLUTION && BTM_BLE_HOST_ADDR_RESOLUTION == TRUE)
    tBTM_BLE_RESOLVE_CBACK      *p_resolve_cback;
#endif
    tBTM_BLE_ADDR_CBACK         *p_generate_cback;
    void                        *p;
    TIMER_LIST_ENT              raddr_timer_ent;
    BOOLEAN                     enhanced_conn_evt;
} tBTM_LE_RANDOM_CB;

#ifndef BTM_BLE_MAX_BG_CONN_DEV_NUM
#define BTM_BLE_MAX_BG_CONN_DEV_NUM    10
#endif

typedef struct
{
    UINT16              min_conn_int;
    UINT16              max_conn_int;
    UINT16              slave_latency;
    UINT16              supervision_tout;

} tBTM_LE_CONN_PRAMS;


typedef struct
{
    BD_ADDR     bd_addr;
    UINT8       attr;
    BOOLEAN     is_connected;
    BOOLEAN     in_use;
} tBTM_LE_BG_CONN_DEV;

/* white list using state as a bit mask */
#define BTM_BLE_WL_IDLE         0
#define BTM_BLE_WL_INIT         1
#define BTM_BLE_WL_SCAN         2
#define BTM_BLE_WL_ADV          4
typedef UINT8 tBTM_BLE_WL_STATE;

/* resolving list using state as a bit mask */
#define BTM_BLE_RL_IDLE         0
#define BTM_BLE_RL_INIT         1
#define BTM_BLE_RL_SCAN         2
#define BTM_BLE_RL_ADV          4
typedef UINT8 tBTM_BLE_RL_STATE;

/* BLE connection state */
#define BLE_CONN_IDLE    0
#define BLE_DIR_CONN     1
#define BLE_BG_CONN      2
#define BLE_CONN_CANCEL  3
typedef UINT8 tBTM_BLE_CONN_ST;

typedef struct
{
    void    *p_param;
} tBTM_BLE_CONN_REQ;

/* LE state request */
#define BTM_BLE_STATE_INVALID               0
#define BTM_BLE_STATE_CONN_ADV              1
#define BTM_BLE_STATE_INIT                  2
#define BTM_BLE_STATE_MASTER                3
#define BTM_BLE_STATE_SLAVE                 4
#define BTM_BLE_STATE_LO_DUTY_DIR_ADV       5
#define BTM_BLE_STATE_HI_DUTY_DIR_ADV       6
#define BTM_BLE_STATE_NON_CONN_ADV          7
#define BTM_BLE_STATE_PASSIVE_SCAN          8
#define BTM_BLE_STATE_ACTIVE_SCAN           9
#define BTM_BLE_STATE_SCAN_ADV              10
#define BTM_BLE_STATE_MAX                   11
typedef UINT8 tBTM_BLE_STATE;

#define BTM_BLE_STATE_CONN_ADV_BIT          0x0001
#define BTM_BLE_STATE_INIT_BIT              0x0002
#define BTM_BLE_STATE_MASTER_BIT            0x0004
#define BTM_BLE_STATE_SLAVE_BIT             0x0008
#define BTM_BLE_STATE_LO_DUTY_DIR_ADV_BIT   0x0010
#define BTM_BLE_STATE_HI_DUTY_DIR_ADV_BIT   0x0020
#define BTM_BLE_STATE_NON_CONN_ADV_BIT      0x0040
#define BTM_BLE_STATE_PASSIVE_SCAN_BIT      0x0080
#define BTM_BLE_STATE_ACTIVE_SCAN_BIT       0x0100
#define BTM_BLE_STATE_SCAN_ADV_BIT          0x0200
typedef UINT16 tBTM_BLE_STATE_MASK;

#define BTM_BLE_STATE_ALL_MASK              0x03ff
#define BTM_BLE_STATE_ALL_ADV_MASK          (BTM_BLE_STATE_CONN_ADV_BIT|BTM_BLE_STATE_LO_DUTY_DIR_ADV_BIT|BTM_BLE_STATE_HI_DUTY_DIR_ADV_BIT|BTM_BLE_STATE_SCAN_ADV_BIT)
#define BTM_BLE_STATE_ALL_SCAN_MASK         (BTM_BLE_STATE_PASSIVE_SCAN_BIT|BTM_BLE_STATE_ACTIVE_SCAN_BIT)
#define BTM_BLE_STATE_ALL_CONN_MASK         (BTM_BLE_STATE_MASTER_BIT|BTM_BLE_STATE_SLAVE_BIT)


typedef struct
{
    BD_ADDR         *resolve_q_random_pseudo;
    UINT8           *resolve_q_action;
    UINT8           q_next;
    UINT8           q_pending;
} tBTM_BLE_RESOLVE_Q;

typedef struct
{
    BOOLEAN     in_use;
    BOOLEAN     to_add;
    BD_ADDR     bd_addr;
    UINT8       attr;
} tBTM_BLE_WL_OP;

/* BLE privacy mode */
#define BTM_PRIVACY_NONE    0              /* BLE no privacy */
#define BTM_PRIVACY_1_1     1              /* BLE privacy 1.1, do not support privacy 1.0 */
#define BTM_PRIVACY_1_2     2              /* BLE privacy 1.2 */
typedef UINT8 tBTM_PRIVACY_MODE;

/* data length change event callback */
typedef void (tBTM_DATA_LENGTH_CHANGE_CBACK)(UINT16 max_tx_length, UINT16 max_rx_length);

/* Define BLE Device Management control structure
*/
typedef struct
{
    UINT8            scan_activity;         /* LE scan activity mask */

    /*****************************************************
    **      BLE Inquiry
    *****************************************************/
    tBTM_BLE_INQ_CB     inq_var;

    /* observer callback and timer */
    tBTM_BLE_SCAN_RESULT_CBACK *p_obs_results_cb;
    TIMER_LIST_ENT   obs_timer_ent;

    /* background connection procedure cb value */
    tBTM_BLE_CONN_TYPE  bg_conn_type;
    UINT16              scan_int;
    UINT16              scan_win;
    tBTM_BLE_SEL_CBACK  *p_select_cback;
    tBTM_BLE_SEL_CBACK  *p_suspend_cback;/* suspended selective callback */

    tBLE_CONN_MODE      bg_conn_mode;       /* bg conn mode in initiating state */
    TIMER_LIST_ENT      bg_conn_timer_entry;

    /* white list information */
    UINT8            num_empty_filter;      /* Number of entries in white list */
    UINT8            max_filter_entries;    /* Maximum number of entries that can be stored */
    tBTM_BLE_WL_STATE       wl_state;
    UINT8                   bg_dev_num;
#if (defined(BTU_DYNAMIC_CB_INCLUDED)  && (BTU_DYNAMIC_CB_INCLUDED == TRUE))
    tBTM_LE_BG_CONN_DEV     *bg_dev_list;
#else
    tBTM_LE_BG_CONN_DEV     bg_dev_list[BTM_BLE_MAX_BG_CONN_DEV_NUM];
#endif
    BUFFER_Q                conn_pending_q;
    tBTM_BLE_CONN_ST        conn_state;

    /* random address management control block */
    tBTM_LE_RANDOM_CB   addr_mgnt_cb;

    BOOLEAN            st_random_addr_used;  /* static random address used */

#if (BLE_BRCM_INCLUDED == TRUE)
    BOOLEAN         scatternet_enable;
#endif

    tBLE_CONN_MODE      conn_mode;       /* direct conn mode in initiating state */
#if BTM_BLE_PRIVACY_SPT == TRUE
    tBTM_PRIVACY_MODE   privacy_mode;               /* privacy mode */
    UINT8               resolving_list_max_size;   /* resolving list max size */
    UINT8               resolving_list_avail_size;     /* resolving list available size */
    UINT8               suspended_rl_st;
    UINT8               *irk_list_mask;

    tBTM_BLE_RESOLVE_Q   resolving_list_pend_q;
    tBTM_BLE_RL_STATE   rl_state;
#endif
#if (defined(BTU_DYNAMIC_CB_INCLUDED)  && (BTU_DYNAMIC_CB_INCLUDED == TRUE))
    tBTM_BLE_WL_OP  *wl_op_q;
#else
    tBTM_BLE_WL_OP  wl_op_q[BTM_BLE_MAX_BG_CONN_DEV_NUM];
#endif
    /* current BLE link state */
    tBTM_BLE_STATE_MASK         cur_states;     /* bit mask of tBTM_BLE_STATE */
    UINT8                       link_count[2];  /* total link count master and slave*/
#if (BTM_BLE_CONFORMANCE_TESTING == TRUE)
    UINT16                      rpa_tout;
#if BLE_DATA_LEN_EXT_INCLUDED == TRUE
    tBTM_DATA_LENGTH_CHANGE_CBACK    *p_data_length_change_cback;
#endif
#endif

    /* Scan cback */
    tBTM_BLE_SCAN_RESULT_CBACK  *p_scan_result_cback;

} tBTM_BLE_CB;

#ifdef __cplusplus
extern "C" {
#endif

extern void btm_ble_timeout(TIMER_LIST_ENT *p_tle);
extern void btm_ble_process_adv_pkt(UINT8 *p);
extern void btm_ble_proc_scan_rsp_rpt(UINT8 *p);
extern tBTM_STATUS btm_ble_read_remote_name(BD_ADDR remote_bda, tBTM_BLE_SCAN_RESULT *p_cur, tBTM_CMPL_CB *p_cb);
extern BOOLEAN btm_ble_cancel_remote_name(BD_ADDR remote_bda);

extern tBTM_STATUS btm_ble_start_inquiry(UINT8 mode, UINT16 duration, UINT8 filter_cond_type, tBTM_INQ_FILT_COND *p_filter_cond, UINT8 duplicate_enable);
extern void btm_ble_stop_scan(void);
extern void btm_clear_all_pending_le_entry(void);

extern void btm_ble_stop_inquiry(void);
extern void btm_ble_init(void);
extern void *btm_ble_connected(UINT8 *bda, UINT16 handle, UINT8 enc_mode, UINT8 role, tBLE_ADDR_TYPE addr_type, BOOLEAN addr_matched);
extern void btm_ble_read_remote_features_complete(UINT8 *p);
extern void btm_ble_write_adv_enable_complete(UINT8 *p);
extern void btm_ble_conn_complete(UINT8 *p, UINT16 evt_len, BOOLEAN enhanced);
extern void btm_read_ble_local_supported_states_complete(UINT8 *p, UINT16 evt_len);
extern tBTM_BLE_CONN_ST btm_ble_get_conn_st(void);
extern void btm_ble_set_conn_st(tBTM_BLE_CONN_ST new_st);
extern tBTM_STATUS btm_ble_start_adv(void);
extern tBTM_STATUS btm_ble_stop_adv(void);
extern tBTM_STATUS btm_ble_start_scan(void);
extern void btm_ble_create_ll_conn_complete(UINT8 status);
extern BOOLEAN btm_ble_sec_add_ble_device_and_keys(BD_ADDR remote_bda, tBLE_ADDR_TYPE bd_addr_type);

/* LE device management functions */
extern void btm_ble_reset_id(void);

/* security related functions */
extern void btm_ble_increment_sign_ctr(BD_ADDR bd_addr, BOOLEAN is_local);
extern BOOLEAN btm_get_local_div(BD_ADDR bd_addr, UINT16 *p_div);
extern BOOLEAN btm_ble_get_enc_key_type(BD_ADDR bd_addr, UINT8 *p_key_types);

extern void btm_ble_rand_enc_complete(UINT8 *p, UINT16 op_code, tBTM_RAND_ENC_CB *p_enc_cplt_cback);
extern void btm_sec_save_le_key(BD_ADDR bd_addr, tBTM_LE_KEY_TYPE key_type, tBTM_LE_KEY_VALUE *p_keys, BOOLEAN pass_to_application);

/* white list function */
extern BOOLEAN btm_update_dev_to_white_list(BOOLEAN to_add, BD_ADDR bd_addr, UINT8 wl_type);
extern BOOLEAN btm_update_bg_conn_list(BOOLEAN to_add, BD_ADDR bd_addr, UINT8 *p_attr_tag);
extern void btm_update_scanner_filter_policy(tBTM_BLE_SFP scan_policy);
extern void btm_update_adv_filter_policy(tBTM_BLE_AFP adv_policy);
extern void btm_ble_clear_white_list(void);
extern void btm_read_white_list_size_complete(UINT8 *p, UINT16 evt_len);
extern void btm_ble_add_2_white_list_complete(UINT8 status);
extern void btm_ble_remove_from_white_list_complete(UINT8 *p, UINT16 evt_len);
extern void btm_ble_clear_white_list_complete(UINT8 *p, UINT16 evt_len);

/* background connection function */
extern BOOLEAN btm_ble_suspend_bg_conn(void);
extern BOOLEAN btm_ble_resume_bg_conn(void);
extern void btm_ble_initiate_select_conn(BD_ADDR bda);
extern BOOLEAN btm_ble_start_auto_conn(BOOLEAN start);
extern BOOLEAN btm_ble_start_select_conn(BOOLEAN start, tBTM_BLE_SEL_CBACK   *p_select_cback);
extern UINT8 btm_ble_count_unconn_dev_in_whitelist(void);
extern void btm_ble_update_mode_operation(UINT8 link_role, BD_ADDR bda, UINT8 status);
extern BOOLEAN btm_execute_wl_dev_operation(void);
extern void btm_ble_update_link_topology_mask(UINT8 role, BOOLEAN increase);
extern void btm_ble_get_conn_mode_scan_param(tBLE_CONN_MODE conn_mode, UINT16 *p_scan_int, UINT16 *p_scan_win,
        UINT16 *p_conn_int_min, UINT16 *p_conn_int_max,
        UINT16 *p_conn_latency, UINT16 *p_conn_supervision_tout,
        UINT32 *p_conn_duration);

/* direct connection utility */
extern BOOLEAN btm_send_pending_direct_conn(void);
extern void btm_ble_enqueue_direct_conn_req(void *p_param);
extern tBLE_CONN_MODE btm_ble_get_next_conn_scan_mode(void);
extern void btm_ble_set_conn_scan_mode(tBLE_CONN_MODE scan_mode);
extern void btm_ble_set_bg_conn_scan_mode(tBLE_CONN_MODE scan_mode);
extern tBLE_CONN_MODE btm_ble_get_next_bg_conn_scan_mode(void);

/* BLE address management */
extern void btm_gen_resolvable_private_addr(void *p_cmd_cplt_cback);
extern void btm_gen_non_resolvable_private_addr(tBTM_BLE_ADDR_CBACK *p_cback, void *p);
#if (defined BTM_BLE_HOST_ADDR_RESOLUTION && BTM_BLE_HOST_ADDR_RESOLUTION == TRUE)
extern void btm_ble_resolve_random_addr(BD_ADDR random_bda, tBTM_BLE_RESOLVE_CBACK *p_cback, void *p, BOOLEAN enhanced_conn_evt);
#endif
extern void btm_gen_resolve_paddr_low(tBTM_RAND_ENC *p);


/* BLE address mapping with CS feature */
extern BOOLEAN btm_identity_addr_to_random_pseudo(BD_ADDR bd_addr, UINT8 *p_addr_type, BOOLEAN refresh);
extern BOOLEAN btm_random_pseudo_to_identity_addr(BD_ADDR random_pseudo, UINT8 *p_static_addr_type);
extern void btm_ble_refresh_peer_rpa(BD_ADDR pseudo_bda, BD_ADDR rra, UINT8 rra_type);
extern void btm_ble_refresh_local_rpa(BD_ADDR pesudo_addr, BD_ADDR local_rpa);

/* device control function */
extern void btm_set_ble_host_chnl_class_cmplt(UINT8 *p, UINT8 ctr_id);
extern void btm_read_ble_buf_size_complete(UINT8 *p, UINT16 evt_len);
extern void btm_read_ble_local_supported_features_complete(UINT8 *p, UINT16 evt_len);

/*  privacy function */
extern void btm_ble_read_resolving_list_entry_complete(UINT8 *p, UINT16 evt_len) ;
extern void btm_ble_remove_resolving_list_entry_complete(UINT8 *p, UINT16 evt_len);
extern void btm_ble_add_resolving_list_entry_complete(UINT8 *p, UINT16 evt_len);
extern void btm_ble_clear_resolving_list_complete(UINT8 *p, UINT16 evt_len);
extern void btm_read_ble_resolving_list_size_complete(UINT8 *p, UINT16 evt_len);
extern void btm_ble_enable_resolving_list(UINT8);
extern BOOLEAN btm_ble_disable_resolving_list(UINT8, BOOLEAN);
extern void btm_ble_enable_resolving_list_for_platform(UINT8 rl_mask);
extern BOOLEAN btm_ble_resolving_list_load_dev_act(BD_ADDR bd_addr, tBLE_ADDR_TYPE addr_type, BD_ADDR stat_addr,
        BT_OCTET16 irk_value);
extern tBTM_STATUS btm_ble_resolving_list_remove_dev_act(BD_ADDR bd_addr, tBLE_ADDR_TYPE addr_type, BD_ADDR stat_addr);

extern BOOLEAN btm_ble_topology_check(tBTM_BLE_STATE_MASK request);
extern BOOLEAN btm_ble_clear_topology_mask(tBTM_BLE_STATE_MASK request_state);
extern BOOLEAN btm_ble_set_topology_mask(tBTM_BLE_STATE_MASK request_state);
extern void btm_ble_resolving_list_init(UINT8 max_irk_list_sz, BOOLEAN to_clear);
extern void btm_ble_resolving_list_cleanup(void);
BT_API extern void btm_set_random_address(BD_ADDR random_bda);

#if BTM_BLE_CONFORMANCE_TESTING == TRUE
BT_API extern void btm_ble_set_no_disc_if_pair_fail(BOOLEAN disble_disc);
BT_API extern void btm_ble_set_test_mac_value(BOOLEAN enable, UINT8 *p_test_mac_val);
BT_API extern void btm_ble_set_test_local_sign_cntr_value(BOOLEAN enable, UINT32 test_local_sign_cntr);
BT_API extern BOOLEAN btm_is_local_llt_enabled(void);
BT_API extern BOOLEAN btm_is_remote_llt_enabled(BD_ADDR remote_bda);
BT_API extern void btm_ble_set_rpa_tout(UINT32 timeout);
BT_API extern void btm_ble_set_keep_rfu_in_auth_req(BOOLEAN keep_rfu);
#if BLE_DATA_LEN_EXT_INCLUDED == TRUE
BT_API extern void btm_ble_register_data_length_change_cback(void *p_cback);
#endif
#endif

#if !defined(BTM_INTERNAL_LINKKEY_STORAGE_INCLUDED) || (BTM_INTERNAL_LINKKEY_STORAGE_INCLUDED == FALSE)
extern tBTM_STATUS btm_ble_request_stored_key(BD_ADDR bd_addr, tBTM_LE_KEY_TYPE key_type, tBTM_LE_KEY_VALUE *p_le_keys);
#endif

#ifdef __cplusplus
}
#endif

#endif
