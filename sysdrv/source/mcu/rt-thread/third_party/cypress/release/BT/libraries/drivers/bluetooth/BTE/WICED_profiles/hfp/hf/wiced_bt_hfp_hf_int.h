/*
 * $ Copyright Broadcom Corporation $
 */

/** @file
 *
 * This is the private interface file for the Handsfree profile.
 */

#pragma once

#include "wiced_bt_hfp_hf.h"
#include "wiced_bt_sdp.h"
#include "wiced_bt_rfcomm.h"
#include "btu.h"

/*****************************************************************************
**  Constants
*****************************************************************************/
/* Set it to TRUE for adding debugging traces */
#ifndef WICED_BT_HFP_HF_DEBUG
#define WICED_BT_HFP_HF_DEBUG FALSE
#endif

#define WICED_BT_HFP_HF_VERSION_1_5 0x0105
#define WICED_BT_HFP_HF_VERSION_1_6 0x0106
#define WICED_BT_HFP_HF_VERSION_1_7 0x0107

/* Note, if you change the default version here, please also change the one in
 * bta_ag_api.h, they are meant to be the same.
 */
#ifndef WICED_BT_HFP_HF_VERSION
#define WICED_BT_HFP_HF_VERSION WICED_BT_HFP_HF_VERSION_1_7
#endif

#ifndef WICED_BT_HFP_HF_IND_SUPPORTED
#define WICED_BT_HFP_HF_IND_SUPPORTED TRUE
#endif

#ifdef WICED_BT_HFP_HF_WBS_INCLUDED
#define WICED_BT_HFP_HF_WBS_INCLUDED FALSE
#endif

/* RFCOMM MTU SIZE */
#define WICED_BT_HFP_HF_MTU              255

#if (WICED_BT_HFP_HF_VERSION >= WICED_BT_HFP_HF_VERSION_1_7 \
  && WICED_BT_HFP_HF_IND_SUPPORTED == TRUE)
/* Max number of peer HF indicator */
#define WICED_BT_HFP_HF_MAX_NUM_PEER_IND 20
#endif

#define WICED_BT_HFP_HF_CMD_TIMEOUT_VALUE 5000 /* 5 seconds */

#define WICED_BT_HFP_HF_PORT_EV_MASK PORT_EV_RXCHAR

#define WICED_BT_HFP_HF_TASK_ID          BTU_TASK

/* Define the task message box for hfp hf */
#define WICED_BT_HFP_HF_TASK_MBOX        TASK_MBOX_3

/* Size of database for service discovery */
#define WICED_BT_HFP_HF_DISC_BUF_SIZE    1000

#if (WICED_BT_HFP_HF_VERSION >= WICED_BT_HFP_HF_VERSION_1_7 \
  && WICED_BT_HFP_HF_IND_SUPPORTED == TRUE)
/* List of supported HF indicator */
/* 1 for Enhanced Safety Status */
/* 2 for Battery Level Status */
#ifndef WICED_BT_HFP_HF_IND_INFO
#define WICED_BT_HFP_HF_IND_INFO "1,2"
#endif
#endif

#define WICED_BT_HFP_HF_MAX_PHONE_NUMBER_LEN 20

#define WICED_BT_HFP_HF_MAX_AT_CMD_LEN WICED_BT_HFP_HF_MTU

/* AT command argument capabilities */
#define WICED_BT_HFP_HF_AT_NONE 0x01 /* No argument */
#define WICED_BT_HFP_HF_AT_SET  0x02 /* Set value */
#define WICED_BT_HFP_HF_AT_READ 0x04 /* Read value */
#define WICED_BT_HFP_HF_AT_TEST 0x08 /* Test value range */
#define WICED_BT_HFP_HF_AT_FREE 0x10 /* Freeform argument */

/* AT argument format */
#define WICED_BT_HFP_HF_AT_FMT_NONE 0 /* No arguments */
#define WICED_BT_HFP_HF_AT_FMT_STR  1 /* String */
#define WICED_BT_HFP_HF_AT_FMT_INT  2 /* Integer */

/* No event to application */
#define WICED_BT_HFP_HF_NO_EVT WICED_BT_HFP_HF_CONNECTION_STATE_EVT

/* ASCII charcter string of arguments to the AT command or result */
#define WICED_BT_HFP_HF_AT_MAX_LEN WICED_BT_HFP_HF_MTU

/* These events are global to Handsfree and not handled by state machine */
typedef enum
{
    WICED_BT_HFP_HF_INVALID_EVT,    /**< Invalid event, used to signify an event from AVDT which is not handled */
    WICED_BT_HFP_HF_API_DEINIT_EVT, /**< De-init API event */
    WICED_BT_HFP_HF_GLOBAL_EVT_END  /**< IMPORTANT: This should always be the last event in this enum */
} wiced_bt_hfp_hf_global_evt_t;

/* These events are handled by the Handsfree state machine for each connection control block */
typedef enum
{
    WICED_BT_HFP_HF_API_CONNECT_EVT = WICED_BT_HFP_HF_GLOBAL_EVT_END,
    WICED_BT_HFP_HF_API_DISCONNECT_EVT,
    WICED_BT_HFP_HF_API_CALL_ACTION_EVT,
    WICED_BT_HFP_HF_API_NOTIFY_VOLUME_EVT,
    WICED_BT_HFP_HF_API_SEND_AT_CMD_EVT,
    WICED_BT_HFP_HF_RFC_CONNECT_EVT,
    WICED_BT_HFP_HF_RFC_CONNECT_FAIL_EVT,
    WICED_BT_HFP_HF_RFC_DISCONNECT_EVT,
    WICED_BT_HFP_HF_RFC_DATA_EVT,
    WICED_BT_HFP_HF_SDP_DISC_OK_EVT,
    WICED_BT_HFP_HF_SDP_DISC_FAIL_EVT,
    WICED_BT_HFP_HF_CMD_TIMEOUT_EVT,
    WICED_BT_HFP_HF_LAST_EVENT /**< IMPORTANT:This should always be the last event in this enum */
} wiced_bt_hfp_hf_state_evt_t;

/* State machine states - Applicable for each scb */
typedef enum
{
    WICED_BT_HFP_HF_INIT_SST,
    WICED_BT_HFP_HF_OPENING_SST,
    WICED_BT_HFP_HF_OPEN_SST,
    WICED_BT_HFP_HF_CLOSING_SST
} wiced_bt_hfp_hf_state_t;

/* AT commands */
enum
{
    WICED_BT_HFP_HF_CMD_VGS,
    WICED_BT_HFP_HF_CMD_VGM,
    WICED_BT_HFP_HF_CMD_CKPD,
    WICED_BT_HFP_HF_CMD_A,
    WICED_BT_HFP_HF_CMD_D,
    WICED_BT_HFP_HF_CMD_CCWA,
    WICED_BT_HFP_HF_CMD_CHLD,
    WICED_BT_HFP_HF_CMD_CHUP,
    WICED_BT_HFP_HF_CMD_CIND,
    WICED_BT_HFP_HF_CMD_CLIP,
    WICED_BT_HFP_HF_CMD_CMER,
    WICED_BT_HFP_HF_CMD_VTS,
    WICED_BT_HFP_HF_CMD_BINP,
    WICED_BT_HFP_HF_CMD_BLDN,
    WICED_BT_HFP_HF_CMD_BVRA,
    WICED_BT_HFP_HF_CMD_BRSF,
    WICED_BT_HFP_HF_CMD_NREC,
    WICED_BT_HFP_HF_CMD_CNUM,
    WICED_BT_HFP_HF_CMD_BTRH,
    WICED_BT_HFP_HF_CMD_COPS,
    WICED_BT_HFP_HF_CMD_CMEE,
    WICED_BT_HFP_HF_CMD_CLCC,
    WICED_BT_HFP_HF_CMD_BCC,
    WICED_BT_HFP_HF_CMD_BCS,
    WICED_BT_HFP_HF_CMD_BAC,
    WICED_BT_HFP_HF_CMD_BIA,
    WICED_BT_HFP_HF_CMD_BIND,
    WICED_BT_HFP_HF_CMD_BIEV,
    WICED_BT_HFP_HF_CMD_UNAT
};


/*****************************************************************************
**  Data types
*****************************************************************************/

typedef struct
{
    const char              *p_res;    /* AT command string */
    uint8_t                  arg_type;
    wiced_bt_hfp_hf_event_t  app_evt;
} wiced_bt_hfp_hf_at_res_t;

/* Callback function executed when command is parsed */
typedef void (wiced_bt_hfp_hf_at_res_cback_t)(void *p_scb, uint16_t res, char *p_arg);

/* Callback function executed to send "ERROR" result code */
typedef void (wiced_bt_hfp_hf_at_err_cback_t)(void *p_scb, wiced_bool_t unknown, char *p_arg);

/* AT command parsing control block */
typedef struct
{
    void                           *p_user;       /* User-defined data */
    char                           *p_res_buf;    /* Temp parsing buffer */
    uint16_t                        res_pos;      /* Position in temp buffer */
    uint16_t                        res_max_len;  /* Length of temp buffer to allocate */
    uint8_t                         state;        /* Parsing state */
} wiced_bt_hfp_hf_at_cb_t;

/* Data type for
 * WICED_BT_HFP_HF_API_CONNECT_EVT
 * WICED_BT_HFP_HF_API_DISCONNECT_EVT
 */
typedef struct
{
    BT_HDR                    hdr;
    wiced_bt_device_address_t bd_address;
} wiced_bt_hfp_hf_api_data_t;

/* Data type for
 * WICED_BT_HFP_HF_API_CALL_ACTION_EVT
 */
typedef struct
{
    BT_HDR                         hdr;
    wiced_bt_device_address_t      bd_address;
    wiced_bt_hfp_hf_call_action_t  action;
    char                           number[WICED_BT_HFP_HF_MAX_PHONE_NUMBER_LEN];
} wiced_bt_hfp_hf_api_call_action_t;

/* Data type for
 * WICED_BT_HFP_HF_API_NOTIFY_VOLUME_EVT
 */
typedef struct
{
    BT_HDR                        hdr;
    wiced_bt_device_address_t     bd_address;
    wiced_bt_hfp_hf_volume_type_t volume_type;
    uint8_t                       volume_level;
} wiced_bt_hfp_hf_api_notify_vol_t;

/* Data type for
 * WICED_BT_HFP_HF_API_SEND_AT_CMD_EVT
 */
typedef struct
{
    BT_HDR                    hdr;
    wiced_bt_device_address_t bd_address;
    char                      at_cmd[WICED_BT_HFP_HF_MAX_AT_CMD_LEN];
} wiced_bt_hfp_hf_api_send_at_cmd_t;

/* Data type for WICED_BT_HFP_HF_SDP_DISC_OK_EVT */
typedef struct
{
    BT_HDR   hdr;
    uint8_t  peer_scn;          /* peer scn to connect to */
    uint16_t peer_feature_mask; /* peer HF feature mask */
    uint16_t peer_version;      /* peer HF version */
} wiced_bt_hfp_hf_sdp_res_t;

/* Data type for WICED_BT_HFP_HF_RFC_DATA_EVT */
typedef struct
{
    BT_HDR   hdr;
    void    *p_data;
    uint16_t len;
} wiced_bt_hfp_rfc_data_t;

/* Data type for
 * WICED_BT_HFP_HF_RFC_CONNECT_FAIL_EVT
 */
typedef struct
{
    BT_HDR                    hdr;
    wiced_bt_device_address_t bd_address;
} wiced_bt_hfp_hf_connect_fail_t;


/* Union of all event datatypes */
typedef union
{
    BT_HDR                            hdr;
    wiced_bt_hfp_hf_api_data_t        api_data;
    wiced_bt_hfp_hf_api_call_action_t api_call_action;
    wiced_bt_hfp_hf_api_notify_vol_t  api_notify_vol;
    wiced_bt_hfp_hf_api_send_at_cmd_t api_send_at_cmd;
    wiced_bt_hfp_hf_sdp_res_t         sdp_res;
    wiced_bt_hfp_rfc_data_t           rfc_data;
    wiced_bt_hfp_hf_connect_fail_t    connect_fail;
} wiced_bt_hfp_hf_data_t;

#if (WICED_BT_HFP_HF_VERSION >= WICED_BT_HFP_HF_VERSION_1_7 \
  && WICED_BT_HFP_HF_IND_SUPPORTED == TRUE)
/* Type for HF indicator */
typedef struct
{
    uint32_t     ind_id;
    wiced_bool_t is_enable;
    uint32_t     ind_val;
} wiced_bt_hfp_hf_peer_indicator_t;
#endif

/* Type for each service control block */
typedef struct
{
    wiced_bool_t                 in_use;                 /* scb in use */
    uint16_t                     rfcomm_handle;          /* RFCOMM handle */
    wiced_bt_device_address_t    peer_addr;              /* For a server, it will be NULL till a connection is established */
    wiced_bt_hfp_hf_at_cb_t      at_cb;                  /* AT command interpreter */
    wiced_bt_sdp_discovery_db_t *p_sdp_db;               /* Pointer to SDP database */
    wiced_bool_t                 dealloc;                /* TRUE if service shutting down */
    wiced_bool_t                 slc_open;               /* Set to TRUE when service level connection up */
    wiced_bool_t                 is_server;              /* Is the SCB a server or client */
    uint8_t                      state;                  /* State machine state */
    uint8_t                      conn_service;           /* Connected service */
    uint16_t                     peer_feature_mask;   /* Peer device features */
    uint16_t                     peer_version;        /* Handsfree profile version of te peer */
    uint8_t                      peer_scn;               /* Peer scn */
    uint8_t                      slc_at_init_state;      /* At-cmd state during slc establishment */
    BUFFER_Q                     wiced_bt_hfp_hf_at_cmd_queue; /* Q for sending AT cmds serially*/
    TIMER_LIST_ENT               wiced_bt_hfp_hf_at_cmd_queue_timer; /* Timer to recover if peer do not respond to AT command */
    uint8_t                      wiced_bt_hfp_hf_at_cmd_queue_depth; /* Depth of AT command Q*/
    uint32_t                     feature_mask;
    uint8_t                      mic_volume;
    uint8_t                      speaker_volume;
    wiced_bool_t                 ind_string_received;
    uint8_t                      call_ind_id;
    uint8_t                      call_setup_ind_id;
    uint8_t                      callheld_ind_id;
    uint8_t                      service_state_ind_id;
    uint8_t                      service_type_ind_id;
    uint8_t                      battery_level_ind_id;
    uint8_t                      rssi_ind_id;
    uint8_t                      call_ind_val;
    uint8_t                      call_setup_ind_val;
    uint8_t                      callheld_ind_val;
#if (WICED_BT_HFP_HF_VERSION >= WICED_BT_HFP_HF_VERSION_1_7 \
  && WICED_BT_HFP_HF_IND_SUPPORTED == TRUE)
    wiced_bt_hfp_hf_peer_indicator_t peer_ind[WICED_BT_HFP_HF_MAX_NUM_PEER_IND]; /* Peer HF indicator status */
#endif
} wiced_bt_hfp_hf_scb_t;

/* Type for Handsfree control block */
typedef struct
{
    wiced_bool_t                   is_init;                       /* Handsfree is initialized or not */
    wiced_bt_hfp_hf_scb_t          scb[WICED_BT_HFP_HF_MAX_CONN]; /* Service control blocks */
    wiced_bt_hfp_hf_config_data_t  config_data;                   /* Configuration data from the application */
    wiced_bt_hfp_hf_event_cb_t     p_event_cback;                 /* Application callback */
    wiced_bt_device_address_t      sdp_bd_addr;
} wiced_bt_hfp_hf_cb_t;

/* Type for non state machine action functions */
typedef void (*wiced_bt_hfp_hf_nsm_act_t)(wiced_bt_hfp_hf_data_t *p_data);

/* Type for state machine action functions */
typedef void (*wiced_bt_hfp_hf_sm_act_t)(wiced_bt_hfp_hf_scb_t *p_ccb,
        wiced_bt_hfp_hf_data_t *p_data);

typedef struct
{
    uint8_t                  event;      /**< One of wiced_bt_hfp_hf_state_evt_t events */
    uint8_t                  next_state; /**< One of wiced_bt_hfp_hf_state_t states */
    wiced_bt_hfp_hf_sm_act_t pfhandler;  /**< Event Handler */
} wiced_bt_hfp_hf_event_map_t;

typedef struct
{
    const wiced_bt_hfp_hf_event_map_t *state_table;
    uint8_t                            size_of_table;
} wiced_bt_hfp_hf_sst_tbl_entry_t;


/*****************************************************************************
**  Global data
*****************************************************************************/

/* Control block declaration */
extern wiced_bt_hfp_hf_cb_t wiced_bt_hfp_hf_cb;

extern wiced_bt_hfp_hf_at_res_t wiced_bt_hfp_hf_res[];

/*****************************************************************************
**  Function prototypes
*****************************************************************************/

/* Main functions */
extern wiced_bt_hfp_hf_scb_t *wiced_bt_hfp_hf_get_scb_by_bd_addr(
    wiced_bt_device_address_t bd_addr);
extern wiced_bt_hfp_hf_scb_t *wiced_bt_hfp_hf_get_scb_by_handle(uint16_t handle);
extern wiced_result_t wiced_bt_hfp_hf_register(uint8_t scn);
extern void wiced_bt_hfp_hf_scb_dealloc(wiced_bt_hfp_hf_scb_t *p_scb);
extern wiced_bt_hfp_hf_scb_t *wiced_bt_hfp_hf_scb_alloc(void);


/* State machine related */
extern wiced_bool_t wiced_bt_hfp_hf_hdl_event(BT_HDR *p_msg);
extern wiced_result_t wiced_bt_hfp_hf_init_state_machine(void);
extern void wiced_bt_hfp_hf_ssm_execute(wiced_bt_hfp_hf_scb_t *p_scb,
                                        wiced_bt_hfp_hf_data_t *p_data, uint8_t event);

/* Utility functions */
extern void wiced_bt_hfp_hf_utils_bdcpy(wiced_bt_device_address_t a,
                                        const wiced_bt_device_address_t b);
extern int wiced_bt_hfp_hf_utils_bdcmp(const wiced_bt_device_address_t a,
                                       const wiced_bt_device_address_t b);

/* Action functions */
extern void wiced_bt_hfp_hf_do_sdp(wiced_bt_hfp_hf_scb_t *p_scb,
                                   wiced_bt_hfp_hf_data_t *p_data);
extern void wiced_bt_hfp_hf_sdp_failed(wiced_bt_hfp_hf_scb_t *p_scb,
                                       wiced_bt_hfp_hf_data_t *p_data);
extern void wiced_bt_hfp_hf_rfc_connect_req(wiced_bt_hfp_hf_scb_t *p_scb,
        wiced_bt_hfp_hf_data_t *p_data);
extern void wiced_bt_hfp_hf_rfc_connection_fail(wiced_bt_hfp_hf_scb_t *p_scb,
        wiced_bt_hfp_hf_data_t *p_data);
extern void wiced_bt_hfp_hf_rfc_connected(wiced_bt_hfp_hf_scb_t *p_scb,
        wiced_bt_hfp_hf_data_t *p_data);
extern void wiced_bt_hfp_hf_rfc_disconnect_req(wiced_bt_hfp_hf_scb_t *p_scb,
        wiced_bt_hfp_hf_data_t *p_data);
extern void wiced_bt_hfp_hf_rfc_disconnected(wiced_bt_hfp_hf_scb_t *p_scb,
        wiced_bt_hfp_hf_data_t *p_data);
extern void wiced_bt_hfp_hf_rfc_data(wiced_bt_hfp_hf_scb_t *p_scb,
                                     wiced_bt_hfp_hf_data_t *p_data);
extern void wiced_bt_hfp_hf_slc_open(wiced_bt_hfp_hf_scb_t *p_scb,
                                     wiced_bt_hfp_hf_data_t *p_data);
extern void wiced_bt_hfp_hf_do_call_action(wiced_bt_hfp_hf_scb_t *p_scb,
        wiced_bt_hfp_hf_data_t *p_data);
extern void wiced_bt_hfp_hf_do_notify_volume(wiced_bt_hfp_hf_scb_t *p_scb,
        wiced_bt_hfp_hf_data_t *p_data);
extern void wiced_bt_hfp_hf_do_send_at_cmd(wiced_bt_hfp_hf_scb_t *p_scb,
        wiced_bt_hfp_hf_data_t *p_data);
extern void wiced_bt_hfp_hf_cmd_timeout(wiced_bt_hfp_hf_scb_t *p_scb,
                                        wiced_bt_hfp_hf_data_t *p_data);

/* RFCOMM callback related functions */
extern void wiced_bt_hfp_hf_rfcomm_mgmt_cback(wiced_bt_rfcomm_result_t code, uint16_t handle);
extern int wiced_bt_hfp_hf_rfcomm_data_cback(uint16_t handle, void *p_data, uint16_t len);

/* AT command related */
extern void wiced_bt_hfp_hf_at_init(wiced_bt_hfp_hf_at_cb_t *p_cb);
extern void wiced_bt_hfp_hf_at_reinit(wiced_bt_hfp_hf_at_cb_t *p_cb);
extern wiced_bool_t wiced_bt_hfp_hf_at_parse(wiced_bt_hfp_hf_at_cb_t *p_cb,
        char *p_buf, uint16_t len);
extern void wiced_bt_hfp_hf_at_cmd_queue_init(wiced_bt_hfp_hf_scb_t *p_scb);
extern void wiced_bt_hfp_hf_at_cmd_queue_free(wiced_bt_hfp_hf_scb_t *p_scb);
extern void wiced_bt_hfp_hf_at_cmd_queue_send(wiced_bt_hfp_hf_scb_t *p_scb);
extern void wiced_bt_hfp_hf_at_cmd_queue_enqueue(wiced_bt_hfp_hf_scb_t *p_scb,
        uint8_t cmd, char *buf, uint16_t len);
extern void wiced_bt_hfp_hf_at_cmd_queue_flush(wiced_bt_hfp_hf_scb_t *p_scb);
extern void wiced_bt_hfp_hf_at_cmd_queue_timeout(wiced_bt_hfp_hf_scb_t *p_scb);
extern uint8_t wiced_bt_hfp_hf_at_cmd_queue_handle_res(wiced_bt_hfp_hf_scb_t *p_scb);
extern void wiced_bt_hfp_hf_at_cmd_flush_cmd_in_queue(wiced_bt_hfp_hf_scb_t *p_scb, uint8_t cmd);
extern void wiced_bt_hfp_hf_at_cback(void *user_data, uint16_t res, char *p_arg);
extern void wiced_bt_hfp_hf_at_err_cback(void *user_data, wiced_bool_t unknown, char *p_arg);
extern void wiced_bt_hfp_hf_at_send_cmd(wiced_bt_hfp_hf_scb_t *p_scb, uint8_t cmd,
                                        uint8_t arg_type, uint8_t arg_format, const char *p_arg, int16_t int_arg);


/* SLC related */
extern void wiced_bt_hfp_hf_at_slc(wiced_bt_hfp_hf_scb_t *p_scb);

/* Debug functions */
#if (defined(WICED_BT_HFP_HF_DEBUG) && WICED_BT_HFP_HF_DEBUG == TRUE)
extern char *wiced_bt_hfp_hf_st_code(uint8_t state);
extern char *wiced_bt_hfp_hf_evt_code(uint8_t evt_code);
#endif

