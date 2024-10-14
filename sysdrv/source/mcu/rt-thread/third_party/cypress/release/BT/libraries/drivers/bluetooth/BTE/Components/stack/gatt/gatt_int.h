
/****************************************************************************
**
**  Name:       gatt_int.h
**
**  Function    this file contains internally used GATT profile
**
**  Copyright (c) 1999-2014, Broadcom Corp., All Rights Reserved.
**  Broadcom Bluetooth Core. Proprietary and confidential.
**
******************************************************************************/

#ifndef  GATT_INT_H
#define  GATT_INT_H

#include "bt_target.h"

#if BLE_INCLUDED == TRUE

#if (BT_TRACE_PROTOCOL == TRUE)
#include "trace_api.h"
#endif

#include "bt_trace.h"
#include "gatt_api.h"
#include "btm_ble_api.h"
#include "btu.h"

#include <string.h>

/* Define GATT debug trace macros */
#if (defined(BT_USE_TRACES) && (BT_USE_TRACES == TRUE))
#define GATT_TRACE_UUID(uuid) (gatt_dbg_display_uuid(uuid))
#else
#define GATT_TRACE_UUID(uuid)
#endif

#define GATT_CREATE_CONN_ID(tcb_idx, gatt_if)  ((UINT16) ((((UINT8)(tcb_idx) ) << 8) \
                                             | ((UINT8) (gatt_if))))

#define GATT_GET_TCB_IDX(conn_id)  ((UINT8) (((UINT16) (conn_id)) >> 8))
#define GATT_GET_GATT_IF(conn_id)  ((tGATT_IF)((UINT8) (conn_id)))
#define GATT_TRANS_ID_MAX                   0x0fffffff      /* 4 MSB is reserved */

/* security action for GATT write and read request */
#define GATT_SEC_NONE              0
#define GATT_SEC_OK                1
#define GATT_SEC_SIGN_DATA         2   /* compute the signature for the write cmd */
#define GATT_SEC_ENCRYPT           3    /* encrypt the link with current key */
#define GATT_SEC_ENCRYPT_NO_MITM   4    /* unauthenticated encryption or better */
#define GATT_SEC_ENCRYPT_MITM      5    /* authenticated encryption */
#define GATT_SEC_ENC_PENDING       6   /* wait for link encryption pending */
typedef UINT8 tGATT_SEC_ACTION;


#define GATT_ATTR_OP_SPT_MTU               (0x00000001 << 0)
#define GATT_ATTR_OP_SPT_FIND_INFO         (0x00000001 << 1)
#define GATT_ATTR_OP_SPT_FIND_BY_TYPE      (0x00000001 << 2)
#define GATT_ATTR_OP_SPT_READ_BY_TYPE      (0x00000001 << 3)
#define GATT_ATTR_OP_SPT_READ              (0x00000001 << 4)
#define GATT_ATTR_OP_SPT_MULT_READ         (0x00000001 << 5)
#define GATT_ATTR_OP_SPT_READ_BLOB         (0x00000001 << 6)
#define GATT_ATTR_OP_SPT_READ_BY_GRP_TYPE  (0x00000001 << 7)
#define GATT_ATTR_OP_SPT_WRITE             (0x00000001 << 8)
#define GATT_ATTR_OP_SPT_WRITE_CMD         (0x00000001 << 9)
#define GATT_ATTR_OP_SPT_PREP_WRITE        (0x00000001 << 10)
#define GATT_ATTR_OP_SPT_EXE_WRITE         (0x00000001 << 11)
#define GATT_ATTR_OP_SPT_HDL_VALUE_CONF    (0x00000001 << 12)
#define GATT_ATTR_OP_SP_SIGN_WRITE        (0x00000001 << 13)

#define GATT_INDEX_INVALID  0xff

#define GATT_PENDING_REQ_NONE    0


#define GATT_WRITE_CMD_MASK  0xc0  /*0x1100-0000*/
#define GATT_AUTH_SIGN_MASK  0x80  /*0x1000-0000*/
#define GATT_AUTH_SIGN_LEN   12

#define GATT_HDR_SIZE           3 /* 1B opcode + 2B handle */

/* wait for ATT cmd response timeout value */
#define GATT_WAIT_FOR_RSP_TOUT       30

/* characteristic descriptor type */
#define GATT_DESCR_EXT_DSCPTOR   1    /* Characteristic Extended Properties */
#define GATT_DESCR_USER_DSCPTOR  2    /* Characteristic User Description    */
#define GATT_DESCR_CLT_CONFIG    3    /* Client Characteristic Configuration */
#define GATT_DESCR_SVR_CONFIG    4    /* Server Characteristic Configuration */
#define GATT_DESCR_PRES_FORMAT   5    /* Characteristic Presentation Format */
#define GATT_DESCR_AGGR_FORMAT   6    /* Characteristic Aggregate Format */
#define GATT_DESCR_VALID_RANGE   7    /* Characteristic Valid Range */
#define GATT_DESCR_UNKNOWN       0xff

#define GATT_SEC_FLAG_LKEY_UNAUTHED     BTM_SEC_FLAG_LKEY_KNOWN
#define GATT_SEC_FLAG_LKEY_AUTHED       BTM_SEC_FLAG_LKEY_AUTHED
#define GATT_SEC_FLAG_ENCRYPTED         BTM_SEC_FLAG_ENCRYPTED
typedef UINT8 tGATT_SEC_FLAG;

#if (defined(GATT_FIXED_DB) && (GATT_FIXED_DB == TRUE))
/* Fixed GATT DB IF indices */
#define GATT_FIXED_DB_IF_GAP        1
#define GATT_FIXED_DB_IF_APP        2
#define GATT_FIXED_DB_IF_HOGP       3
#endif /* (!defined(GATT_FIXED_DB) || (GATT_FIXED_DB == FALSE)) */

/* Find Information Response Type
*/
#define GATT_INFO_TYPE_PAIR_16      0x01
#define GATT_INFO_TYPE_PAIR_128     0x02

/*  GATT client FIND_TYPE_VALUE_Request data (currently used only for find service/characteristic by UUID) */
typedef struct
{
    tBT_UUID        uuid;           /* type of attribute to be found */
    UINT16          s_handle;       /* starting handle */
    UINT16          e_handle;       /* ending handle */
    UINT16          value_len;      /* length of the attribute value */
    UINT8           value[MAX_UUID_SIZE];     /* pointer to the attribute value to be found */
} tGATT_FIND_TYPE_VALUE;

/* client request message to ATT protocol
*/
typedef union
{
    tGATT_READ_BY_TYPE      browse;     /* read by type request */
    tGATT_FIND_TYPE_VALUE   find_type_value;/* find by type value */
    tGATT_READ_MULTI        read_multi;   /* read multiple request */
    tGATT_READ_PARTIAL      read_blob;    /* read blob */
    tGATT_ATTR_DATA         write_value;   /* write request */
    /* prepare write */
    UINT16                  handle;        /* read,  handle value confirmation */
    UINT16                  mtu;
    tGATT_EXEC_FLAG         exec_write;    /* execute write */
} tGATT_CL_MSG;

/* error response strucutre */
typedef struct
{
    UINT16  handle;
    UINT8   cmd_code;
    UINT8   reason;
} tGATT_ERROR;

/* server response message to ATT protocol
*/
typedef union
{
    /* data type            member          event   */
    tGATT_VALUE             attr_value;     /* READ, HANDLE_VALUE_IND, PREPARE_WRITE */
    /* READ_BLOB, READ_BY_TYPE */
    tGATT_ERROR             error;          /* ERROR_RSP */
    UINT16                  handle;         /* WRITE, WRITE_BLOB */
    UINT16                  mtu;            /* exchange MTU request */
} tGATT_SR_MSG;

/* Characteristic declaration attribute value
*/
typedef struct
{
    tGATT_CHAR_PROP             property;
    UINT16                      char_val_handle;
} tGATT_CHAR_DECL;

/* attribute value maintained in the server database
*/
typedef union
{
    tBT_UUID                uuid;               /* service declaration */
    tGATT_CHAR_DECL         char_decl;          /* characteristic declaration */
    tGATT_INCL_SRVC         incl_handle;        /* included service */

} tGATT_ATTR_VALUE;

/* Attribute UUID type
*/
#define GATT_ATTR_UUID_TYPE_16      0
#define GATT_ATTR_UUID_TYPE_128     1
#define GATT_ATTR_UUID_TYPE_32      2
typedef UINT8   tGATT_ATTR_UUID_TYPE;

#define GATT_LISTEN_TO_ALL  0xff
#define GATT_LISTEN_TO_NONE 0

/* Data Structure used for GATT server */
/* An GATT registration record consists of a handle, and 1 or more attributes */
/* A service registration information record consists of beginning and ending */
/* attribute handle, service UUID and a set of GATT server callback.          */

typedef struct
{
    tGATT_CBACK  *app_cb;
    tGATT_IF     gatt_if; /* one based */
    BOOLEAN      in_use;
    UINT8        listening; /* if adv for all has been enabled */
} tGATT_REG;


/* command queue for each connection */
typedef struct
{
    BT_HDR      *p_cmd;
    UINT16      clcb_idx;
    UINT8       op_code;
    BOOLEAN     to_send;
} tGATT_CMD_Q;


#if GATT_MAX_SR_PROFILES <= 8
typedef UINT8 tGATT_APP_MASK;
#elif GATT_MAX_SR_PROFILES <= 16
typedef UINT16 tGATT_APP_MASK;
#elif GATT_MAX_SR_PROFILES <= 32
typedef UINT32 tGATT_APP_MASK;
#endif

/* command details for each connection */
typedef struct
{
    BT_HDR          *p_rsp_msg;
    tGATT_READ_MULTI multi_req;
    BUFFER_Q         multi_rsp_q;
    UINT16           handle;
    UINT8            op_code;
    UINT8            status;
} tGATT_SR_CMD;


#define     GATT_CH_CLOSE               0
#define     GATT_CH_CLOSING             1
#define     GATT_CH_CONN                2
#define     GATT_CH_OPEN                3
#define     GATT_CH_W4_SEC_COMP         4
#define     GATT_CH_W4_DATA_SIGN_COMP   5

typedef UINT8 tGATT_CH_STATE;

typedef struct
{
    BUFFER_Q        pending_enc_clcb;   /* pending encryption channel q */
    tGATT_SEC_ACTION sec_act;
    BD_ADDR         peer_bda;
    tBLE_ADDR_TYPE  peer_bda_type;
    tBT_TRANSPORT   transport;
    UINT8           link_role;
    UINT32          trans_id;

    UINT16          att_lcid;           /* L2CAP channel ID for ATT */
    UINT16          payload_size;

    tGATT_CH_STATE  ch_state;
    UINT8           ch_flags;

    tGATT_IF         app_hold_link[GATT_MAX_APPS];

    /* server needs */
    /* server response data */
    tGATT_SR_CMD    sr_cmd;
    UINT16          indicate_handle;
    BUFFER_Q        pending_ind_q;

    TIMER_LIST_ENT  conf_timer_ent;     /* peer confirm to indication timer */

    UINT8            ind_count;
#if (defined(BTU_DYNAMIC_CB_INCLUDED)  && (BTU_DYNAMIC_CB_INCLUDED == TRUE))
    tGATT_CMD_Q       *cl_cmd_q;
#else
    tGATT_CMD_Q       cl_cmd_q[GATT_CL_MAX_LCB];
#endif
    TIMER_LIST_ENT    rsp_timer_ent;        /* peer response timer */
    TIMER_LIST_ENT    ind_ack_timer_ent;    /* local app confirm to indication timer */
    UINT8             pending_cl_req;
    UINT8             next_slot_inq;    /* index of next available slot in queue */
    BOOLEAN           in_use;
    UINT8             tcb_idx;
} tGATT_TCB;


/* logic channel */
typedef struct
{
    UINT16                  next_disc_start_hdl;   /* starting handle for the
                                                      next inc service discovery */
    tGATT_DISC_RES          result;
    BOOLEAN                 wait_for_read_rsp;
} tGATT_READ_INC_UUID128;
typedef struct
{
    tGATT_TCB               *p_tcb;         /* associated TCB of this CLCB */
    tGATT_REG               *p_reg;        /* owner of this CLCB */
    UINT8                   sccb_idx;
    UINT8                   *p_attr_buf;    /* attr buf for read mult, prepare write */
    tBT_UUID                uuid;
    UINT16                  conn_id;        /* connection handle */
    UINT16                  clcb_idx;
    UINT16                  s_handle;       /* starting handle of the active request */
    UINT16                  e_handle;       /* ending handle of the active request */
    UINT16                  counter;        /* used as offset, attribute length,
                                               num of prepare write */
    UINT16                  start_offset;
    tGATT_AUTH_REQ          auth_req;       /* authentication requirement */
    UINT8                   operation;      /* 1 logic chan can have 1 oper active */
    UINT8                   op_subtype;     /* operation subtype */
    UINT8                   status;         /* operation status */
    BOOLEAN                 first_read_blob_after_read;
    tGATT_READ_INC_UUID128  read_uuid128;
    BOOLEAN                 in_use;
    UINT16                  requested_mtu;  /* application requested MTU */
} tGATT_CLCB;

typedef struct
{
    tGATT_CLCB  *p_clcb;
} tGATT_PENDING_ENC_CLCB;


#define GATT_SIGN_WRITE             1
#define GATT_VERIFY_SIGN_DATA       2

typedef struct
{
    BT_HDR      hdr;
    tGATT_CLCB  *p_clcb;
} tGATT_SIGN_WRITE_OP;

typedef struct
{
    BT_HDR      hdr;
    tGATT_TCB   *p_tcb;
    BT_HDR      *p_data;

} tGATT_VERIFY_SIGN_OP;


typedef struct
{
    UINT16                  clcb_idx;
    BOOLEAN                 in_use;
} tGATT_SCCB;

typedef struct
{
    tGATT_IF        gatt_if[GATT_MAX_APPS];
    tGATT_IF        listen_gif[GATT_MAX_APPS];
    BD_ADDR         remote_bda;
    BOOLEAN         in_use;
} tGATT_BG_CONN_DEV;

/* GATT service change Client Congfiguration related variable */
#define GATT_SERVICE_CHG_CCC_STAGE_1        1   /* wait for connection */
#define GATT_SERVICE_CHG_CCC_STAGE_2        2   /* GATT service discovery */
#define GATT_SERVICE_CHG_CCC_STAGE_3        3   /* service change char discovery */
#define GATT_SERVICE_CHG_CCC_STAGE_4        4   /* service change CCC discovery */
#define GATT_SERVICE_CHG_CCC_STAGE_5        5   /* config CCC */

typedef struct
{
    UINT16  conn_id;
    BOOLEAN in_use;
    BOOLEAN connected;
    BD_ADDR bda;
    tBT_TRANSPORT   transport;

    UINT16      s_handle;
    UINT16      e_handle;
    UINT8       ccc_stage;
    UINT8       ccc_result;

} tGATT_PROFILE_CLCB;

typedef struct
{
#if (defined(BTU_DYNAMIC_CB_INCLUDED)  && (BTU_DYNAMIC_CB_INCLUDED == TRUE))
    tGATT_TCB           *tcb;
#else
    tGATT_TCB           tcb[GATT_MAX_PHY_CHANNEL];
#endif
    BUFFER_Q            sign_op_queue;

    tGATT_REG           cl_rcb[GATT_MAX_APPS];
#if (defined(BTU_DYNAMIC_CB_INCLUDED)  && (BTU_DYNAMIC_CB_INCLUDED == TRUE))
    tGATT_CLCB          *clcb;                  /* connection link control block*/
#else
    tGATT_CLCB          clcb[GATT_CL_MAX_LCB];  /* connection link control block*/
#endif
    UINT8               trace_level;
    UINT16              def_mtu_size;

    tGATT_PROFILE_CLCB  profile_clcb[GATT_MAX_APPS];
    tGATT_BG_CONN_DEV   bgconn_dev[GATT_MAX_BG_CONN_DEV];

} tGATT_CB;


#ifdef __cplusplus
extern "C" {
#endif

/* Global GATT data */
#if GATT_DYNAMIC_MEMORY == FALSE
GATT_API extern tGATT_CB  gatt_cb;
#else
GATT_API extern tGATT_CB *gatt_cb_ptr;
#define gatt_cb (*gatt_cb_ptr)
#endif

#ifdef __cplusplus
}
#endif

/* internal functions */
extern void gatt_init(void);
extern void gatt_deinit(void);

/* from gatt_main.c */
extern BOOLEAN gatt_disconnect(tGATT_TCB *p_tcb);

extern BOOLEAN gatt_act_connect(tGATT_REG *p_reg, BD_ADDR bd_addr,
                                tBT_TRANSPORT transport);

extern BOOLEAN gatt_connect(BD_ADDR rem_bda,  tGATT_TCB *p_tcb,
                            tBT_TRANSPORT transport);

extern void gatt_data_process(tGATT_TCB *p_tcb, BT_HDR *p_buf);

extern void gatt_update_app_use_link_flag(tGATT_IF gatt_if, tGATT_TCB *p_tcb,
        BOOLEAN is_add, BOOLEAN check_acl_link);

extern void gatt_set_ch_state(tGATT_TCB *p_tcb, tGATT_CH_STATE ch_state);
extern tGATT_CH_STATE gatt_get_ch_state(tGATT_TCB *p_tcb);
extern BOOLEAN gatt_ble_send_brcm_notify(BD_ADDR remote_bda, UINT16 handle,
        UINT16 len, UINT8 notif_msg);

/* Functions provided by att_protocol.c */
extern tGATT_STATUS attp_send_cl_msg(tGATT_TCB *p_tcb, UINT16 clcb_idx, UINT8 op_code,
                                     tGATT_CL_MSG *p_msg);

extern tGATT_STATUS attp_send_msg_to_l2cap(tGATT_TCB *p_tcb, BT_HDR *p_toL2CAP);
extern BT_HDR *attp_build_err_cmd(UINT8 cmd_code, UINT16 err_handle, UINT8 reason);
extern BT_HDR *attp_build_mtu_cmd(UINT8 op_code, UINT16 rx_mtu);
extern BT_HDR *attp_build_exec_write_cmd(UINT8 op_code, UINT8 flag);
extern BT_HDR *attp_build_value_cmd(UINT8 tcb_idx, UINT16 payload_size, UINT8 op_code, UINT16 handle,
                                    UINT16 offset, UINT16 len, UINT8 *p_data);
extern BT_HDR *attp_build_opcode_cmd(UINT8 op_code);

/* utility functions */
extern UINT8 *gatt_dbg_op_name(UINT8 op_code);
extern UINT32 gatt_add_sdp_record(tBT_UUID *p_uuid, UINT16 start_hdl, UINT16 end_hdl);
extern BOOLEAN gatt_parse_uuid_from_cmd(tBT_UUID *p_uuid, UINT16 len, UINT8 **p_data);
extern UINT8 gatt_build_uuid_to_stream(UINT8 **p_dst, tBT_UUID uuid);
extern BOOLEAN gatt_uuid_compare(tBT_UUID src, tBT_UUID tar);
extern void gatt_convert_uuid32_to_uuid128(UINT8 uuid_128[LEN_UUID_128], UINT32 uuid_32);

extern void gatt_start_rsp_timer(tGATT_TCB    *p_tcb);
extern void gatt_start_conf_timer(tGATT_TCB    *p_tcb);
extern void gatt_rsp_timeout(TIMER_LIST_ENT *p_tle);
extern void gatt_ind_ack_timeout(TIMER_LIST_ENT *p_tle);
extern void gatt_start_ind_ack_timer(tGATT_TCB *p_tcb);
extern void gatt_dbg_display_uuid(tBT_UUID bt_uuid);
extern BOOLEAN gatt_cl_send_next_cmd_inq(tGATT_TCB *p_tcb);
extern void gatt_convert_uuid16_to_uuid128(UINT8 uuid_128[LEN_UUID_128], UINT16 uuid_16);


extern tGATT_STATUS gatt_send_error_rsp(tGATT_TCB *p_tcb, UINT8 err_code, UINT8 op_code,
                                        UINT16 handle);

extern tGATT_PENDING_ENC_CLCB *gatt_add_pending_enc_channel_clcb(tGATT_TCB *p_tcb,
        tGATT_CLCB *p_clcb);

extern BOOLEAN gatt_find_the_connected_bda(UINT8 start_idx, BD_ADDR bda,
        UINT8 *p_found_idx,
        tBT_TRANSPORT *p_transport);

extern BOOLEAN gatt_update_listen_mode(void);
extern BOOLEAN gatt_cl_send_next_cmd_inq(tGATT_TCB *p_tcb);

/* for background connection */
extern BOOLEAN gatt_update_auto_connect_dev(tGATT_IF gatt_if, BOOLEAN add,
        BD_ADDR bd_addr, BOOLEAN is_initiator);

extern BOOLEAN gatt_is_bg_dev_for_app(tGATT_BG_CONN_DEV *p_dev, tGATT_IF gatt_if);
extern BOOLEAN gatt_remove_bg_dev_for_app(tGATT_IF gatt_if, BD_ADDR bd_addr);
extern UINT8 gatt_get_num_apps_for_bg_dev(BD_ADDR bd_addr);
extern BOOLEAN gatt_find_app_for_bg_dev(BD_ADDR bd_addr, tGATT_IF *p_gatt_if);
extern tGATT_BG_CONN_DEV *gatt_find_bg_dev(BD_ADDR remote_bda);
extern void gatt_deregister_bgdev_list(tGATT_IF gatt_if);
extern void gatt_reset_bgdev_list(void);

/* server function */
extern BOOLEAN gatt_server_handle_client_req(tGATT_TCB *p_tcb, UINT8 op_code, UINT16 len, UINT8 *p_data, BT_HDR *p_buf);
extern void gatt_sr_send_req_callback(UINT16 conn_id,  UINT8 op_code, tGATTS_DATA *p_req_data);
extern BOOLEAN gatt_cancel_open(tGATT_IF gatt_if, BD_ADDR bda);
extern void *gatt_sr_get_attr_ptr(UINT16 attr_handle);
extern tGATT_STATUS gatt_sr_dispatchExtValueCb(UINT8 conn_idx, UINT8 req_type, UINT16 handle,
        UINT8 *p_val, UINT16 *p_val_len, UINT16 offset);
extern UINT8 gatt_sr_get_attr_value(UINT8 tcb_idx, UINT16 attr_handle, UINT16 *p_attr_val_len, UINT8 *p_buf);
extern tGATT_STATUS gatts_attibute_request(tGATT_CBACK *gatt_cback, UINT16 conn_id, tGATTS_REQ_TYPE type, tGATTS_DATA *p_data);
extern tGATT_STATUS gatt_sr_cache_multiple_read_rsp(UINT8 tcb_idx, UINT16 attr_handle, tGATT_STATUS status, UINT16 attr_len, UINT8 *p_data);
extern tGATT_STATUS gatt_sr_process_app_rsp(tGATT_TCB *p_tcb, UINT8 op_code, tGATT_STATUS status, UINT16 attr_handle, UINT16 attr_len,  UINT16 attr_offset, UINT8 *p_att);
extern BT_HDR *attp_build_sr_msg(tGATT_TCB *p_tcb, UINT8 op_code, UINT16 attr_handle, UINT16 attr_len,  UINT16 attr_offset, UINT8 *p_att);
extern void gatt_dequeue_sr_cmd(tGATT_TCB *p_tcb);
/*   */

extern tGATT_REG *gatt_get_regcb(tGATT_IF gatt_if);
extern BOOLEAN gatt_is_clcb_allocated(UINT16 conn_id);
extern tGATT_CLCB *gatt_clcb_alloc(UINT16 conn_id);
extern void gatt_clcb_dealloc(tGATT_CLCB *p_clcb);

extern BOOLEAN gatt_find_app_hold_link(tGATT_TCB *p_tcb, UINT8 start_idx,
                                       UINT8 *p_found_idx, tGATT_IF *p_gatt_if);

extern UINT8 gatt_num_apps_hold_link(tGATT_TCB *p_tcb);
extern UINT8 gatt_num_clcb_by_bd_addr(BD_ADDR bda);
extern tGATT_TCB *gatt_find_tcb_by_cid(UINT16 lcid);
extern tGATT_TCB *gatt_allocate_tcb_by_bdaddr(BD_ADDR bda, tBT_TRANSPORT transport);
extern tGATT_TCB *gatt_get_tcb_by_idx(UINT8 tcb_idx);
extern tGATT_TCB *gatt_find_tcb_by_addr(BD_ADDR bda, tBT_TRANSPORT transport);
extern void gatt_reset_tcb(tGATT_TCB *p_tcb);

/* GATT client functions */
extern UINT8 gatt_send_write_msg(tGATT_TCB *p_tcb, UINT16 clcb_idx, UINT8 op_code,
                                 UINT16 handle, UINT16 len, UINT16
                                 offset, UINT8 *p_data);

extern void gatt_cleanup_upon_disc(BD_ADDR bda, UINT16 reason, tBT_TRANSPORT transport);
extern void gatt_end_operation(tGATT_CLCB *p_clcb, tGATT_STATUS status, void *p_data);

extern void gatt_act_discovery(tGATT_CLCB *p_clcb);
extern void gatt_act_read(tGATT_CLCB *p_clcb, UINT16 offset);
extern void gatt_act_write(tGATT_CLCB *p_clcb, UINT8 sec_act);

extern UINT8 gatt_act_send_browse(tGATT_TCB *p_tcb, UINT16 index, UINT8 op,
                                  UINT16 s_handle, UINT16 e_handle, tBT_UUID uuid);

extern tGATT_CLCB *gatt_cmd_dequeue(tGATT_TCB *p_tcb, UINT8 *p_opcode);

extern void gatt_cmd_enq(tGATT_TCB *p_tcb, UINT16 clcb_idx, BOOLEAN to_send,
                         UINT8 op_code, BT_HDR *p_buf);

extern void gatt_client_handle_server_rsp(tGATT_TCB *p_tcb, UINT8 op_code,
        UINT16 len, UINT8 *p_data);

extern void gatt_send_queue_write_cancel(tGATT_TCB *p_tcb, tGATT_CLCB *p_clcb,
        tGATT_EXEC_FLAG flag);

/* gatt_auth.c */
extern BOOLEAN gatt_security_check_start(tGATT_CLCB *p_clcb);
extern void gatt_verify_signature(tGATT_TCB *p_tcb, BT_HDR *p_buf);
extern tGATT_SEC_ACTION gatt_determine_sec_act(tGATT_CLCB *p_clcb);
extern tGATT_STATUS gatt_get_link_encrypt_status(tGATT_TCB *p_tcb);
extern tGATT_SEC_ACTION gatt_get_sec_act(tGATT_TCB *p_tcb);
extern void gatt_set_sec_act(tGATT_TCB *p_tcb, tGATT_SEC_ACTION sec_act);

extern void gatt_reset_bgdev_list(void);

#endif

#endif /* BLE_INCLUDED */
