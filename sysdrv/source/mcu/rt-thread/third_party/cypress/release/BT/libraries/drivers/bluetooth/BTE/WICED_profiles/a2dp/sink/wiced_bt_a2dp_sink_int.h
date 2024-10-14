/*
 * $ Copyright Broadcom Corporation $
 */

/** @file
 *
 * This is the private interface file for the audio sink profile.
 */

#pragma once

#include "wiced_bt_avdt.h"
#include "wiced_bt_a2dp_int.h"
#include "wiced_bt_a2dp_sink.h"
#include "btu.h"

/*****************************************************************************
**  Constants
*****************************************************************************/

/* Set it to TRUE for adding debugging traces */
#ifndef WICED_BT_A2DP_SINK_DEBUG
#define WICED_BT_A2DP_SINK_DEBUG FALSE
#endif

/* Maximun number of AVDTP signaling connections */
#define WICED_BT_A2DP_SINK_MAX_NUM_CONN 2
#define WICED_BT_A2DP_SINK_MAX_NUM_CODECS 2
/* Max stream end-points */
#define WICED_BT_A2DP_SINK_MAX_SEPS     WICED_BT_A2DP_SINK_MAX_NUM_CONN*WICED_BT_A2DP_SINK_MAX_NUM_CODECS
#define WICED_BT_A2DP_SINK_SEP_MEDIA_TYPE_INDEX 2

/* The timer in milliseconds to guard against link busy and AVDT_CloseReq failed to be sent */
#define WICED_A2DP_SINK_CLOSE_REQ_TIME_VAL  4000

#define WICED_BT_A2DP_SINK_TASK_ID          BTU_TASK

/* Define the task message box for A2DP Sink */
#define WICED_BT_A2DP_SINK_TASK_MBOX        TASK_MBOX_2

/* Maximum number of SEPS in stream discovery results */
#define WICED_BT_A2DP_SINK_NUM_SEPS         8

/* Size of database for service discovery */
#define WICED_BT_A2DP_SINK_DISC_BUF_SIZE    1000

/* Maximum length of AVDTP security data */
#define WICED_BT_A2DP_SINK_SECURITY_MAX_LEN 400

/**************************/
/* Audio channel configuration */
/**************************/
/* AVDTP signaling channel MTU at L2CAP */
#define WICED_BT_A2DP_SINK_AVDT_SIG_CH_MTU              644

/* AVDTP audio transport channel MTU at L2CAP */
#define WICED_BT_A2DP_SINK_AVDT_AUDIO_MAX_MTU           1008

/* AVDTP audio transport channel flush timeout */
#define WICED_BT_A2DP_SINK_AVDT_AUDIO_FLUSH_TOUT        200

/* AVDTP audio channel max data queue size */
#define WICED_BT_A2DP_SINK_AVDT_AUDIO_MAX_QUEUE_SIZE    6


/* AVDTP protocol timeout values */
#define WICED_BT_A2DP_SINK_RET_TOUT         4
#define WICED_BT_A2DP_SINK_SIG_TOUT         4
#define WICED_BT_A2DP_SINK_IDLE_TOUT        10

/* Initiator/acceptor role for adaption */
#define WICED_BT_A2DP_SINK_ROLE_OPEN_ACP       0x00     /**< Connection Open Acceptor */
#define WICED_BT_A2DP_SINK_ROLE_OPEN_INT       0x01     /**< Connection Open Initiator */

/* Initiator/acceptor signaling roles */
#define WICED_BT_A2DP_SINK_ROLE_START_ACP      0x00
#define WICED_BT_A2DP_SINK_ROLE_START_INT      0x10     /**< Do not change this value */

/*****************************************************************************
**  Enums
*****************************************************************************/

/* These events are global to A2DP sink and not handled by state machine */
typedef enum
{
    WICED_BT_A2DP_SINK_INVALID_EVT,             /**< Invalid event, used to signify an event from AVDT which is not handled */
    WICED_BT_A2DP_SINK_API_DEINIT_EVT,          /**< De-init API event */
    WICED_BT_A2DP_SINK_SIG_CHG_EVT,             /**< AVDT Signalling change event */
    WICED_BT_A2DP_SINK_AVDT_REPOPT_CONN_EVT,    /**< AVDT Reporting channel connected/disconnected */
    WICED_BT_A2DP_SINK_GLOBAL_EVT_END           /**< IMPORTANT: This should always be the last event in this enum */
} wiced_bt_a2dp_sink_global_evt_t;

/* These events are handled by the A2DP sink state machine for each connection control block */
typedef enum
{
    WICED_BT_A2DP_SINK_API_CONNECT_EVT = WICED_BT_A2DP_SINK_GLOBAL_EVT_END,
    WICED_BT_A2DP_SINK_API_DISCONNECT_EVT,
    WICED_BT_A2DP_SINK_API_START_EVT,
    WICED_BT_A2DP_SINK_API_SUSPEND_EVT,
    WICED_BT_A2DP_SINK_SDP_DISC_OK_EVT,
    WICED_BT_A2DP_SINK_SDP_DISC_FAIL_EVT,
    WICED_BT_A2DP_SINK_STR_OPEN_OK_EVT,
    WICED_BT_A2DP_SINK_STR_OPEN_FAIL_EVT,
    WICED_BT_A2DP_SINK_STR_START_OK_EVT,
    WICED_BT_A2DP_SINK_STR_START_FAIL_EVT,
    WICED_BT_A2DP_SINK_STR_CLOSE_OK_EVT,
    WICED_BT_A2DP_SINK_STR_CONFIG_IND_EVT,
    WICED_BT_A2DP_SINK_STR_CONFIG_RSP_OK_EVT,
    WICED_BT_A2DP_SINK_STR_CONFIG_RSP_FAIL_EVT,
    WICED_BT_A2DP_SINK_STR_SUSPEND_CFM_EVT,
    WICED_BT_A2DP_SINK_STR_RECONFIG_IND_EVT,
    WICED_BT_A2DP_SINK_AVDT_CONNECT_EVT,
    WICED_BT_A2DP_SINK_AVDT_DISCONNECT_EVT,
    WICED_BT_A2DP_SINK_LAST_EVENT                   /**< IMPORTANT: This should always be the last event in this enum */
} wiced_bt_a2dp_sink_state_evt_t;

/* State machine states - Applicable for each connection control block */
typedef enum
{
    WICED_BT_A2DP_SINK_INIT_SST,
    WICED_BT_A2DP_SINK_SIG_OPENING_SST,
    WICED_BT_A2DP_SINK_SIG_OPEN_SST,
    WICED_BT_A2DP_SINK_INCOMING_SST,
    WICED_BT_A2DP_SINK_OPEN_SST,
    WICED_BT_A2DP_SINK_CLOSING_SST
} wiced_bt_a2dp_sink_state_t;

/*****************************************************************************
**  Data types
*****************************************************************************/

/* Data type for
 * WICED_BT_A2DP_SINK_API_CONNECT_EVT
 * WICED_BT_A2DP_SINK_API_DISCONNECT_EVT
 * WICED_BT_A2DP_SINK_API_START_EVT
 * WICED_BT_A2DP_SINK_API_SUSPEND_EVT
 */
typedef struct
{
    BT_HDR                      hdr;
    wiced_bt_device_address_t   bd_address;
} wiced_bt_a2dp_sink_api_data_t;

/* Data type for
 * WICED_BT_A2DP_SINK_STR_CONFIG_RSP_OK_EVT and
 * WICED_BT_A2DP_SINK_STR_CONFIG_RSP_FAIL_EVT
 */
typedef struct
{
    BT_HDR                      hdr;
    uint8_t                     err_code;
    uint8_t                     category;
    wiced_bt_device_address_t   peer_addr;      /* peer BD address */
} wiced_bt_a2dp_sink_setconfig_rsp_t;

/* Data type for all stream events from AVDTP */
typedef struct
{
    BT_HDR                      hdr;
    wiced_bt_avdt_cfg_t         cfg;        /* configuration/capabilities parameters */
    wiced_bt_avdt_ctrl_t        msg;        /* AVDTP callback message parameters */
    wiced_bt_device_address_t   bd_addr;    /* bd address */
    uint8_t                     handle;
} wiced_bt_a2dp_sink_str_msg_t;

/* Data type for WICED_BT_A2DP_SINK_STR_DISC_OK_EVT */
typedef struct
{
    BT_HDR                      hdr;
    uint16_t                    avdt_version;   /* AVDTP protocol version */
} wiced_bt_a2dp_sink_sdp_res_t;

/* Union of all event datatypes */
typedef union
{
    BT_HDR                             hdr;
    wiced_bt_a2dp_sink_api_data_t      api_data;
    wiced_bt_a2dp_sink_setconfig_rsp_t setconfig_rsp;
    wiced_bt_a2dp_sink_str_msg_t       str_msg;
    wiced_bt_a2dp_sink_sdp_res_t       sdp_res;
} wiced_bt_a2dp_sink_data_t;

/* type for SEP control block */
typedef struct
{
    uint8_t                     av_handle;      /* AVDTP handle */
    wiced_bt_a2dp_sink_codec_t  codec_type;     /* codec type */
    uint16_t                    codec_id;       /* codec id for codec_type=WICED_BT_A2DP_SINK_CODEC_VENDOR_SPECIFIC */
    wiced_bt_avdt_sep_info_t    sep_info;
} wiced_bt_a2dp_sink_sep_t;

/* Type for A2DP sink stream control block */
typedef struct
{
    wiced_bt_sdp_discovery_db_t     *p_sdp_db;       /* pointer to SDP database */
    wiced_bt_a2dp_sink_sep_t         seps[WICED_BT_A2DP_SINK_MAX_SEPS];
    wiced_bt_a2dp_sink_sep_t         sep;
    wiced_bt_avdt_cfg_t             *p_cap;          /* buffer used for get capabilities */
    wiced_bt_avdt_sep_info_t         sep_info[WICED_BT_A2DP_SINK_NUM_SEPS]; /* stream discovery results */
    wiced_bt_avdt_cfg_t              cfg;            /* local SEP configuration */
    wiced_bt_device_address_t        peer_addr;      /* peer BD address */
    uint16_t                         l2c_cid;        /* L2CAP channel ID */
    uint16_t                         stream_mtu;     /* MTU of stream */
    uint16_t                         avdt_version;   /* the avdt version of peer device */
    wiced_bt_a2dp_sink_codec_t       codec_type;     /* codec type */
    uint16_t                         codec_id;       /* codec id for codec_type=WICED_BT_A2DP_SINK_CODEC_VENDOR_SPECIFIC */
    uint8_t                          media_type;     /* Media type */
    wiced_result_t                   open_status;    /* open failure status */
    uint16_t                         cur_psc_mask;   /* Protocol service capabilities mask for current connection */
    uint8_t                          avdt_handle;    /* AVDTP handle */
    uint8_t                          num_seps;       /* number of seps returned by stream discovery */
    uint8_t                          sep_info_idx;   /* current index into sep_info */
    uint8_t                          sep_idx;        /* current index into local seps[] */
    uint8_t                          rcfg_idx;       /* reconfig requested index into sep_info */
    uint8_t                          state;          /* state machine state */
    uint8_t                          avdt_label;     /* AVDTP label */
    uint8_t                          app_id;         /* application id */
    uint8_t                          num_recfg;      /* number of reconfigure sent */
    uint8_t                          role;
    uint8_t                          l2c_bufs;       /* the number of buffers queued to L2CAP */
    wiced_bool_t                     started;        /* TRUE if stream started */
    wiced_bool_t                     co_started;     /* TRUE if stream started from call-out perspective */
    wiced_bool_t                     co_delay;       /* TRUE if co_delay is called */
    wiced_bool_t                     recfg_sup;      /* TRUE if the first attempt to reconfigure the stream was successfull, else False if command fails */
    wiced_bool_t                     suspend_sup;    /* TRUE if Suspend stream is supported, else FALSE if suspend command fails */
    wiced_bool_t                     deregistring;   /* TRUE if deregistering */
    wiced_bool_t                     recfg_ind;      /* TRUE if reconfigure attempt happens in open state */
    wiced_bool_t                     is_api_close;   /* Whether the close is called by local device through API or not */
} wiced_bt_a2dp_sink_scb_t;

/* Type for A2DP sink connection control block */
typedef struct
{
    wiced_bool_t                     in_use;         /* whether ccb is in use or not */
    wiced_bt_device_address_t        peer_addr;      /* peer BD address */
    uint8_t                          state;          /* state machine state */
    uint8_t                          ccb_handle;     /* ccb handle to layer above */
    uint8_t                          avdt_handle;    /* avdt handle from layer below */
    uint8_t                          scb_hdl;        /* stream control block associated with this connection */
    wiced_bt_avdt_ctrl_cback_t      *p_dt_cback;     /* the callback function to receive events from AVDT control channel */
    wiced_bool_t                     is_str_active;  /* whether stream channel is active */
    wiced_bt_a2dp_sink_scb_t        *p_scb;          /* pointer to the stream control block */
} wiced_bt_a2dp_sink_ccb_t;

/* Type for A2DP sink control block */
typedef struct
{
    wiced_bt_a2dp_sink_sep_t         seps[WICED_BT_A2DP_SINK_MAX_SEPS];
    wiced_bt_a2dp_config_data_t     *p_config_data;  /* Configuration data from the application */
    wiced_bt_a2dp_sink_control_cb_t  control_cb;     /* Application registered control callback function */
    wiced_bt_a2dp_sink_data_cb_t     data_cb;        /* Application registered data callback function */
    wiced_bool_t                     is_init;        /* A2DP Sink is initialized or not */
    wiced_bt_device_address_t        sdp_bd_addr;    /* peer BD address */
    wiced_bt_a2dp_sink_ccb_t         ccb[WICED_BT_A2DP_SINK_MAX_NUM_CONN]; /* AVDTP connection control block */
    wiced_bt_a2dp_sink_scb_t         p_scb[WICED_BT_A2DP_SINK_MAX_SEPS];          /* Pointer to a stream control block */
    TIMER_LIST_ENT                   timer;          /* Timer list entry */
} wiced_bt_a2dp_sink_cb_t;

/* Type for non state machine action functions */
typedef void (*wiced_bt_a2sp_sink_nsm_act_t)(wiced_bt_a2dp_sink_data_t *p_data);

/* Type for state machine action functions */
typedef void (*wiced_bt_a2dp_sink_sm_act_t)(wiced_bt_a2dp_sink_ccb_t *p_ccb,
        wiced_bt_a2dp_sink_data_t *p_data);

typedef struct
{
    uint8_t                     event;      /**< One of wiced_bt_a2dp_sink_state_evt_t events */
    uint8_t                     next_state; /**< One of wiced_bt_a2dp_sink_state_t states */
    wiced_bt_a2dp_sink_sm_act_t pfhandler;  /**< Event Handler */
} wiced_bt_a2dp_sink_event_map_t;

typedef struct
{
    const wiced_bt_a2dp_sink_event_map_t *state_table;
    uint8_t                               size_of_table;
} wiced_bt_a2dp_sink_sst_tbl_entry_t;

/*****************************************************************************
**  Global data
*****************************************************************************/

/* Control block declaration */
extern wiced_bt_a2dp_sink_cb_t wiced_bt_a2dp_sink_cb;

/*****************************************************************************
**  Function prototypes
*****************************************************************************/

/* Main functions */
wiced_result_t wiced_bt_a2dp_sink_register(void);
extern wiced_bool_t wiced_bt_a2dp_sink_hdl_event(BT_HDR *p_msg);
extern void wiced_bt_a2dp_sink_dereg_comp(void);
extern void wiced_bt_a2dp_sink_dealloc_ccb(wiced_bt_a2dp_sink_ccb_t *p_ccb);
extern void wiced_bt_a2dp_sink_dealloc_scb(wiced_bt_a2dp_sink_scb_t *p_scb);
extern void wiced_bt_a2dp_sink_conn_cback(uint8_t handle,
        wiced_bt_device_address_t bd_addr, uint8_t event, wiced_bt_avdt_ctrl_t *p_data);

/* State machine related function */
extern void wiced_bt_a2dp_sink_ssm_execute(wiced_bt_a2dp_sink_ccb_t *p_ccb,
        wiced_bt_a2dp_sink_data_t *p_data, uint8_t event);
extern wiced_result_t wiced_bt_a2dp_sink_init_state_machine(void);


/* State machine action functions */
extern void wiced_bt_a2dp_sink_do_sdp(wiced_bt_a2dp_sink_ccb_t *p_ccb,
                                      wiced_bt_a2dp_sink_data_t *p_data);
extern void wiced_bt_a2dp_sink_cleanup(wiced_bt_a2dp_sink_ccb_t *p_ccb,
                                       wiced_bt_a2dp_sink_data_t *p_data);
extern void wiced_bt_a2dp_sink_free_sdb(wiced_bt_a2dp_sink_ccb_t *p_cb,
                                        wiced_bt_a2dp_sink_data_t *p_data);
extern void wiced_bt_a2dp_sink_config_ind(wiced_bt_a2dp_sink_ccb_t *p_ccb,
        wiced_bt_a2dp_sink_data_t *p_data);
extern void wiced_bt_a2dp_sink_disconnect_req(wiced_bt_a2dp_sink_ccb_t *p_ccb,
        wiced_bt_a2dp_sink_data_t *p_data);
extern void wiced_bt_a2dp_sink_setconfig_rsp(wiced_bt_a2dp_sink_ccb_t *p_ccb,
        wiced_bt_a2dp_sink_data_t *p_data);
extern void wiced_bt_a2dp_sink_reconfig_rsp(wiced_bt_a2dp_sink_ccb_t *p_ccb,
        wiced_bt_a2dp_sink_data_t *p_data);
extern void wiced_bt_a2dp_sink_str_opened(wiced_bt_a2dp_sink_ccb_t *p_ccb,
        wiced_bt_a2dp_sink_data_t *p_data);
extern void wiced_bt_a2dp_sink_str_open_fail(wiced_bt_a2dp_sink_ccb_t *p_ccb,
        wiced_bt_a2dp_sink_data_t *p_data);
extern void wiced_bt_a2dp_sink_do_close(wiced_bt_a2dp_sink_ccb_t *p_ccb,
                                        wiced_bt_a2dp_sink_data_t *p_data);
extern void wiced_bt_a2dp_sink_close_str(wiced_bt_a2dp_sink_ccb_t *p_ccb,
        wiced_bt_a2dp_sink_data_t *p_data);
extern void wiced_bt_a2dp_sink_connect_req(wiced_bt_a2dp_sink_ccb_t *p_ccb,
        wiced_bt_a2dp_sink_data_t *p_data);
extern void wiced_bt_a2dp_sink_sdp_failed(wiced_bt_a2dp_sink_ccb_t *p_ccb,
        wiced_bt_a2dp_sink_data_t *p_data);
extern void wiced_bt_a2dp_sink_sig_hdl_ap_close(wiced_bt_a2dp_sink_ccb_t *p_ccb,
        wiced_bt_a2dp_sink_data_t *p_data);
extern void wiced_bt_a2dp_sink_do_start(wiced_bt_a2dp_sink_ccb_t *p_ccb,
                                        wiced_bt_a2dp_sink_data_t *p_data);
extern void wiced_bt_a2dp_sink_str_stopped(wiced_bt_a2dp_sink_ccb_t *p_ccb,
        wiced_bt_a2dp_sink_data_t *p_data);
extern void wiced_bt_a2dp_sink_start_ok(wiced_bt_a2dp_sink_ccb_t *p_ccb,
                                        wiced_bt_a2dp_sink_data_t *p_data);
extern void wiced_bt_a2dp_sink_start_failed(wiced_bt_a2dp_sink_ccb_t *p_ccb,
        wiced_bt_a2dp_sink_data_t *p_data);
extern void wiced_bt_a2dp_sink_suspend_cfm(wiced_bt_a2dp_sink_ccb_t *p_ccb,
        wiced_bt_a2dp_sink_data_t *p_data);
extern void wiced_bt_a2dp_sink_hdl_str_close(wiced_bt_a2dp_sink_ccb_t *p_ccb,
        wiced_bt_a2dp_sink_data_t *p_data);
extern void wiced_bt_a2dp_sink_rej_conn(wiced_bt_a2dp_sink_ccb_t *p_ccb,
                                        wiced_bt_a2dp_sink_data_t *p_data);
extern void wiced_bt_a2dp_sink_sig_opened(wiced_bt_a2dp_sink_ccb_t *p_ccb,
        wiced_bt_a2dp_sink_data_t *p_data);
extern void wiced_bt_a2dp_sink_sig_closed(wiced_bt_a2dp_sink_ccb_t *p_ccb,
        wiced_bt_a2dp_sink_data_t *p_data);
extern void wiced_bt_a2dp_sink_sig_open_fail(wiced_bt_a2dp_sink_ccb_t *p_ccb,
        wiced_bt_a2dp_sink_data_t *p_data);
extern void wiced_bt_a2dp_sink_sig_hdl_ap_close_disconnect_req(wiced_bt_a2dp_sink_ccb_t *p_ccb,
        wiced_bt_a2dp_sink_data_t *p_data);
extern void wiced_bt_a2dp_sink_sig_closed_cleanup(wiced_bt_a2dp_sink_ccb_t *p_ccb,
        wiced_bt_a2dp_sink_data_t *p_data);
extern void wiced_bt_a2dp_sink_stream_chg(wiced_bt_a2dp_sink_scb_t *p_scb,
        wiced_bool_t started);

/* Data path and avdt control callback functions */
extern void wiced_bt_a2dp_sink_ctrl_cback(uint8_t handle, wiced_bt_device_address_t bd_addr,
        uint8_t event, wiced_bt_avdt_ctrl_t *p_data);
extern void wiced_bt_a2dp_sink_data_cback(uint8_t handle, BT_HDR *p_pkt,
        uint32_t time_stamp, uint8_t m_pt);

/* Codec configuration related functions */
extern wiced_bool_t wiced_bt_a2dp_sink_cfg_init(wiced_bt_a2dp_codec_info_t *p_codec_info,
        uint8_t *p_built_codec_info, uint8_t *p_num_protect, uint8_t *p_protect_info);

extern void wiced_bt_a2dp_sink_cfg_setcfg_ind_handler(
    wiced_bt_a2dp_codec_info_list_t *p_codec_capabilities,
    wiced_bt_a2dp_sink_str_msg_t *str_msg);

/* Utility functions */
extern void wiced_bt_a2dp_sink_utils_bdcpy(wiced_bt_device_address_t a,
        const wiced_bt_device_address_t b);

extern int wiced_bt_a2dp_sink_utils_bdcmp(const wiced_bt_device_address_t a,
        const wiced_bt_device_address_t b);

/* Debug functions */
#if (defined(WICED_BT_A2DP_SINK_DEBUG) && WICED_BT_A2DP_SINK_DEBUG == TRUE)
extern char *wiced_bt_a2dp_sink_evt_code(uint8_t evt_code);
extern char *wiced_bt_a2dp_sink_st_code(uint8_t state);
#endif

