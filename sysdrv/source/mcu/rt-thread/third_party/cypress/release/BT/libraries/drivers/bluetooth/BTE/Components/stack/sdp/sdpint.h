/****************************************************************************/
/*                                                                          */
/*  Name:       sdp_int.h                                                   */
/*                                                                          */
/*  Function    this file contains internally used SDP definitions          */
/*                                                                          */
/*  Copyright (c) 1999-2009, Broadcom Corp., All Rights Reserved.           */
/*  WIDCOMM Bluetooth Core. Proprietary and confidential.                   */
/*                                                                          */
/*****************************************************************************/

#ifndef  SDP_INT_H
#define  SDP_INT_H

#include "bt_target.h"
#include "sdp_api.h"
#include "l2c_api.h"


/* Continuation length - we use a 2-byte offset */
#define SDP_CONTINUATION_LEN        2
#define SDP_MAX_CONTINUATION_LEN    16          /* As per the spec */

/* Timeout definitions. */
#define SDP_INACT_TIMEOUT       30              /* Inactivity timeout         */


/* Define the Out-Flow default values. */
#define  SDP_OFLOW_QOS_FLAG                 0
#define  SDP_OFLOW_SERV_TYPE                0
#define  SDP_OFLOW_TOKEN_RATE               0
#define  SDP_OFLOW_TOKEN_BUCKET_SIZE        0
#define  SDP_OFLOW_PEAK_BANDWIDTH           0
#define  SDP_OFLOW_LATENCY                  0
#define  SDP_OFLOW_DELAY_VARIATION          0

/* Define the In-Flow default values. */
#define  SDP_IFLOW_QOS_FLAG                 0
#define  SDP_IFLOW_SERV_TYPE                0
#define  SDP_IFLOW_TOKEN_RATE               0
#define  SDP_IFLOW_TOKEN_BUCKET_SIZE        0
#define  SDP_IFLOW_PEAK_BANDWIDTH           0
#define  SDP_IFLOW_LATENCY                  0
#define  SDP_IFLOW_DELAY_VARIATION          0

#define  SDP_LINK_TO                        0

/* Define the type of device notification. */
/* (Inquiry Scan and Page Scan)            */
#define  SDP_DEVICE_NOTI_LEN                sizeof (BT_HDR) +           \
                                            HCIC_PREAMBLE_SIZE +        \
                                            HCIC_PARAM_SIZE_WRITE_PARAM1

#define  SDP_DEVICE_NOTI_FLAG               0x03

/* Define the Protocol Data Unit (PDU) types.
*/
#define  SDP_PDU_ERROR_RESPONSE                 0x01
#define  SDP_PDU_SERVICE_SEARCH_REQ             0x02
#define  SDP_PDU_SERVICE_SEARCH_RSP             0x03
#define  SDP_PDU_SERVICE_ATTR_REQ               0x04
#define  SDP_PDU_SERVICE_ATTR_RSP               0x05
#define  SDP_PDU_SERVICE_SEARCH_ATTR_REQ        0x06
#define  SDP_PDU_SERVICE_SEARCH_ATTR_RSP        0x07

/* Max UUIDs and attributes we support per sequence */
#define     MAX_UUIDS_PER_SEQ       16
#define     MAX_ATTR_PER_SEQ        16

/* Internal UUID sequence representation */
typedef struct
{
    UINT16     len;
    UINT8      value[MAX_UUID_SIZE];
} tUID_ENT;

typedef struct
{
    UINT16      num_uids;
    tUID_ENT    uuid_entry[MAX_UUIDS_PER_SEQ];
} tSDP_UUID_SEQ;


/* Internal attribute sequence definitions */
typedef struct
{
    UINT16      start;
    UINT16      end;
} tATT_ENT;

typedef struct
{
    UINT16      num_attr;
    tATT_ENT    attr_entry[MAX_ATTR_PER_SEQ];
} tSDP_ATTR_SEQ;

#ifdef SDP_RAW_DATA_SERVER

#define SDP_UINT1(value)                (value)
#define SDP_UINT2(value)                (value) >> 8, (value) & 0xff
#define SDP_UINT4(value)                (value) >> 24, ((value) >> 16) & 0xff, ((value) >> 8) & 0xff, (value) & 0xff
#define SDP_UINT8(value)                (value) >> 56, ((value) >> 48) & 0xff, ((value >> 40) >> 8) & 0xff, (value >> 32) & 0xff, ((value) >> 24) & 0xff, ((value) >> 16) & 0xff, ((value) >> 8) & 0xff, (value) & 0xff
#define SDP_BOOLEAN                     SDP_UINT1

#define SDP_ATTR_VALUE_UINT1(value)     (UINT_DESC_TYPE << 3)    | SIZE_ONE_BYTE,    SDP_UINT1(value)
#define SDP_ATTR_VALUE_UINT2(value)     (UINT_DESC_TYPE << 3)    | SIZE_TWO_BYTES,   SDP_UINT2(value)
#define SDP_ATTR_VALUE_UINT4(value)     (UINT_DESC_TYPE << 3)    | SIZE_FOUR_BYTES,  SDP_UINT4(value)
#define SDP_ATTR_VALUE_UINT8(value)     (UINT_DESC_TYPE << 3)    | SIZE_EIGHT_BYTES, SDP_UINT8(value)
#define SDP_ATTR_VALUE_BOOLEAN(value)   (BOOLEAN_DESC_TYPE << 3),                    SDP_UINT1(value)

#define SDP_ATTR_VALUE_TEXT             (TEXT_STR_DESC_TYPE << 3) | SIZE_IN_NEXT_BYTE
#define SDP_ATTR_VALUE_TEXT_1(len)      (TEXT_STR_DESC_TYPE << 3) | SIZE_IN_NEXT_BYTE, SDP_UINT1(len)
#define SDP_ATTR_VALUE_TEXT_2(len)      (TEXT_STR_DESC_TYPE << 3) | SIZE_IN_NEXT_WORD, SDP_UINT2(len)
#define SDP_ATTR_VALUE_TEXT_4(len)      (TEXT_STR_DESC_TYPE << 3) | SIZE_IN_NEXT_LONG, SDP_UINT4(len)

#define SDP_ATTR_UINT1(id, value)       SDP_ATTR_ID(id), SDP_ATTR_VALUE_UINT1(value)
#define SDP_ATTR_UINT2(id, value)       SDP_ATTR_ID(id), SDP_ATTR_VALUE_UINT2(value)
#define SDP_ATTR_UINT4(id, value)       SDP_ATTR_ID(id), SDP_ATTR_VALUE_UINT4(value)
#define SDP_ATTR_UINT8(id, value)       SDP_ATTR_ID(id), SDP_ATTR_VALUE_UINT8(value)
#define SDP_ATTR_BOOLEAN(id, value)     SDP_ATTR_ID(id), SDP_ATTR_VALUE_BOOLEAN(value)

#define SDP_ATTR_ID                     SDP_ATTR_VALUE_UINT2

#define SDP_ATTR_UUID16(uuid)           ((UUID_DESC_TYPE << 3) | SIZE_TWO_BYTES), SDP_UINT2(uuid)

#define SDP_ATTR_TEXT(id, len)          SDP_ATTR_ID(id), SDP_ATTR_VALUE_TEXT, (len)
#define SDP_ATTR_TEXT_1(id, len)        SDP_ATTR_ID(id), SDP_ATTR_VALUE_TEXT_1(len)
#define SDP_ATTR_TEXT_2(id, len)        SDP_ATTR_ID(id), SDP_ATTR_VALUE_TEXT_2(len)
#define SDP_ATTR_TEXT_4(id, len)        SDP_ATTR_ID(id), SDP_ATTR_VALUE_TEXT_4(len)

#define SDP_ATTR_SEQUENCE_1(length)     ((DATA_ELE_SEQ_DESC_TYPE << 3) | SIZE_IN_NEXT_BYTE), (length)

#define SDP_ATTR_RECORD_HANDLE(handle)  SDP_ATTR_UINT4(ATTR_ID_SERVICE_RECORD_HDL, handle)

#define SDP_ATTR_CLASS_ID(uuid)                                         \
    SDP_ATTR_ID(ATTR_ID_SERVICE_CLASS_ID_LIST), SDP_ATTR_SEQUENCE_1(3), \
        SDP_ATTR_UUID16(uuid)

#define SDP_ATTR_PROTOCOL_DESC_LIST(l2cap_chan)                         \
    SDP_ATTR_ID(ATTR_ID_PROTOCOL_DESC_LIST), SDP_ATTR_SEQUENCE_1(13),   \
        SDP_ATTR_SEQUENCE_1(6),                                         \
        SDP_ATTR_UUID16(UUID_PROTOCOL_L2CAP),                           \
            SDP_ATTR_VALUE_UINT2(1),                                    \
        SDP_ATTR_SEQUENCE_1(3),                                         \
            SDP_ATTR_UUID16(l2cap_chan)

#define SDP_ATTR_RFCOMM_PROTOCOL_DESC_LIST(scn)                         \
    SDP_ATTR_ID(ATTR_ID_PROTOCOL_DESC_LIST), SDP_ATTR_SEQUENCE_1(12),   \
        SDP_ATTR_SEQUENCE_1(3),                                         \
            SDP_ATTR_UUID16(UUID_PROTOCOL_L2CAP),                       \
        SDP_ATTR_SEQUENCE_1(5),                                         \
            SDP_ATTR_UUID16(UUID_PROTOCOL_RFCOMM),                      \
            SDP_ATTR_VALUE_UINT1(scn)

#define SDP_ATTR_BROWSE_LIST                                            \
    SDP_ATTR_ID(ATTR_ID_BROWSE_GROUP_LIST), SDP_ATTR_SEQUENCE_1(3),     \
        SDP_ATTR_UUID16(UUID_SERVCLASS_PUBLIC_BROWSE_GROUP)

#define SDP_ATTR_LANGUAGE_BASE_ATTR_ID_LIST                                     \
    SDP_ATTR_ID(ATTR_ID_LANGUAGE_BASE_ATTR_ID_LIST), SDP_ATTR_SEQUENCE_1(9),    \
       SDP_ATTR_VALUE_UINT2(LANG_ID_CODE_ENGLISH),                              \
       SDP_ATTR_VALUE_UINT2(LANG_ID_CHAR_ENCODE_UTF8),                          \
       SDP_ATTR_VALUE_UINT2(LANGUAGE_BASE_ID)

#define SDP_ATTR_PROFILE_DESC_LIST(uuid, version)                       \
    SDP_ATTR_ID(ATTR_ID_BT_PROFILE_DESC_LIST), SDP_ATTR_SEQUENCE_1(8),  \
        SDP_ATTR_SEQUENCE_1(6),                                         \
            SDP_ATTR_UUID16(uuid),                                      \
            SDP_ATTR_VALUE_UINT2(version)

#define SDP_ATTR_SERVICE_NAME(len)                                  \
    SDP_ATTR_TEXT(ATTR_ID_SERVICE_NAME, len)

#define SDP_ATTR_SERVICE_DESCRIPTION(len)                           \
    SDP_ATTR_TEXT(ATTR_ID_SERVICE_DESCRIPTION, len)

#define tSDP_ATTRIBUTE  UINT8
#define tSDP_RECORD     UINT8
#define tSDP_DB         UINT8

#else
/* Define the attribute element of the SDP database record */
typedef struct
{
    UINT32  len;           /* Number of bytes in the entry */
    UINT8   *value_ptr;    /* Points to attr_pad */
    UINT16  id;
    UINT8   type;
} tSDP_ATTRIBUTE;

/* An SDP record consists of a handle, and 1 or more attributes */
typedef struct
{
    UINT32              record_handle;
    UINT32              free_pad_ptr;
    UINT16              num_attributes;
    tSDP_ATTRIBUTE      attribute[SDP_MAX_REC_ATTR];
    UINT8               attr_pad[SDP_MAX_PAD_LEN];
} tSDP_RECORD;


/* Define the SDP database */
typedef struct
{
    UINT32         di_primary_handle;       /* Device ID Primary record or NULL if nonexistent */
    BOOLEAN        brcm_di_registered;
    UINT16         num_records;
    tSDP_RECORD    record[SDP_MAX_RECORDS];
} tSDP_DB;
#endif /* !SDP_RAW_DATA_SERVER */

enum
{
    SDP_IS_SEARCH,
    SDP_IS_ATTR_SEARCH,
    SDP_IS_PASS_THRU    /* only when SDP_FOR_JV_INCLUDED == TRUE */
};

#if SDP_SERVER_ENABLED == TRUE
/* Continuation information for the SDP server response */
typedef struct
{
    UINT16            next_attr_index; /* attr index for next continuation response */
    UINT16            next_attr_start_id;  /* attr id to start with for the attr index in next cont. response */
    tSDP_RECORD       *prev_sdp_rec; /* last sdp record that was completely sent in the response */
    BOOLEAN           last_attr_seq_desc_sent; /* whether attr seq length has been sent previously */
    UINT16            attr_offset; /* offset within the attr to keep trak of partial attributes in the responses */
} tSDP_CONT_INFO;
#endif  /* SDP_SERVER_ENABLED == TRUE */

/* Define the SDP Connection Control Block */
typedef struct
{
#define SDP_STATE_IDLE              0
#define SDP_STATE_CONN_SETUP        1
#define SDP_STATE_CONNECTED         3
    UINT8             con_state;

    BD_ADDR           device_address;
    TIMER_LIST_ENT    timer_entry;
    UINT16            rem_mtu_size;
    UINT16            connection_id;
    UINT16            list_len;                 /* length of the response in the GKI buffer */
    UINT8             *rsp_list;                /* pointer to GKI buffer holding response */

#if SDP_CLIENT_ENABLED == TRUE
    tSDP_DISCOVERY_DB *p_db;                    /* Database to save info into   */
    tSDP_DISC_CMPL_CB *p_cb;                    /* Callback for discovery done  */
    UINT16            num_handles;              /* Number of server handles     */
    UINT16            cur_handle;               /* Current handle being processed */
    UINT16            transaction_id;
    UINT16            disconnect_reason;        /* Disconnect reason            */
#if (defined(SDP_BROWSE_PLUS) && SDP_BROWSE_PLUS == TRUE)
    UINT16            cur_uuid_idx;
#endif

#define SDP_DISC_WAIT_CONN          0
#define SDP_DISC_WAIT_HANDLES       1
#define SDP_DISC_WAIT_ATTR          2
#define SDP_DISC_WAIT_SEARCH_ATTR   3
#define SDP_DISC_WAIT_PASS_THRU     4    /* only when SDP_FOR_JV_INCLUDED == TRUE */
#define SDP_DISC_WAIT_CANCEL        5

    UINT8             disc_state;
#endif  /* SDP_CLIENT_ENABLED == TRUE */

#if SDP_SERVER_ENABLED == TRUE
    UINT16            cont_offset;              /* Continuation state data in the server response */
    tSDP_CONT_INFO    cont_info;                /* structure to hold continuation information for the server response */
#endif  /* SDP_SERVER_ENABLED == TRUE */
} tCONN_CB;


/*  The main SDP control block */
typedef struct
{
#if SDP_CLIENT_ENABLED == TRUE
    tCONN_CB          client_ccb;
#endif

#if SDP_SERVER_ENABLED == TRUE
    tCONN_CB          server_ccb;

#ifndef SDP_RAW_DATA_SERVER
    tSDP_DB           server_db;
#else
    UINT8             *p_server_db;
    UINT16            server_db_len;
#endif /* SDP_RAW_DATA_SERVER */

#endif /* SDP_SERVER_ENABLED */
    tL2CAP_APPL_INFO  reg_info;                 /* L2CAP Registration info */
    UINT16            max_attr_list_size;       /* Max attribute list size to use   */
    UINT8             trace_level;
} tSDP_CB;

#ifdef __cplusplus
extern "C" {
#endif
/* Global SDP data */
#if SDP_DYNAMIC_MEMORY == FALSE
SDP_API extern tSDP_CB  sdp_cb;
#else
SDP_API extern tSDP_CB *sdp_cb_ptr;
#define sdp_cb (*sdp_cb_ptr)
#endif

#ifdef __cplusplus
}
#endif

/* Functions provided by sdp_main.c */
SDP_API extern void     sdp_init(void);
SDP_API extern tCONN_CB *sdp_conn_originate(UINT8 *p_bd_addr);
SDP_API extern void     sdp_disconnect(tCONN_CB *p_ccb, UINT16 reason);

#if (defined(SDP_DEBUG) && SDP_DEBUG == TRUE)
SDP_API extern UINT16 sdp_set_max_attr_list_size(UINT16 max_size);
#endif

/* Functions provided by sdp_conn.c
*/
extern void sdp_conn_timeout(tCONN_CB *p_ccb);

/* Functions provided by sdp_utils.c
*/
extern tCONN_CB *sdpu_find_ccb_by_cid(UINT16 cid);
extern tCONN_CB *sdpu_find_ccb_by_db(tSDP_DISCOVERY_DB *p_db);
extern tCONN_CB *sdpu_allocate_ccb(void);
extern void      sdpu_release_ccb(tCONN_CB *p_ccb);

extern UINT8    *sdpu_build_attrib_seq(UINT8 *p_out, UINT16 *p_attr, UINT16 num_attrs);
extern UINT8    *sdpu_build_attrib_entry(UINT8 *p_out, tSDP_ATTRIBUTE *p_attr);
extern void      sdpu_build_n_send_error(tCONN_CB *p_ccb, UINT16 trans_num, UINT16 error_code, char *p_error_text);

extern UINT8    *sdpu_extract_attr_seq(UINT8 *p, UINT16 param_len, tSDP_ATTR_SEQ *p_seq);
extern UINT8    *sdpu_extract_uid_seq(UINT8 *p, UINT16 param_len, tSDP_UUID_SEQ *p_seq);

#ifdef SDP_RAW_DATA_SERVER
extern UINT8    *sdpu_get_first_rec(void);
extern UINT8    *sdpu_get_next_rec(UINT8 *p);
extern UINT8    *sdpu_get_first_attr(UINT8 *p_rec);
extern UINT8    *sdpu_get_next_attr(UINT8 *p_rec, UINT8 *p_attr);
extern UINT8    *sdpu_get_attr_type(UINT8 *p_attr);
extern UINT8    *sdpu_build_partial_attrib_entry(UINT8 *p_out, UINT8 *p_attr, UINT16 len, UINT16 *offset);
extern UINT32   sdpu_get_rec_handle(UINT8 *p_rec);
extern UINT16   sdpu_get_attr_id(UINT8 *p);
extern UINT32   sdpu_get_attr_len(UINT8 *p_attr);
#endif
SDP_API extern UINT8    *sdpu_get_len_from_type(UINT8 *p, UINT8 type, UINT32 *p_len);
extern BOOLEAN  sdpu_is_base_uuid(UINT8 *p_uuid);
extern BOOLEAN  sdpu_compare_uuid_arrays(UINT8 *p_uuid1, UINT32 len1, UINT8 *p_uuid2, UINT16 len2);
SDP_API extern BOOLEAN  sdpu_compare_bt_uuids(tBT_UUID *p_uuid1, tBT_UUID *p_uuid2);
extern BOOLEAN  sdpu_compare_uuid_with_attr(tBT_UUID *p_btuuid, tSDP_DISC_ATTR *p_attr);

extern void     sdpu_sort_attr_list(UINT16 num_attr, tSDP_DISCOVERY_DB *p_db);
extern UINT16 sdpu_get_list_len(tSDP_UUID_SEQ   *uid_seq, tSDP_ATTR_SEQ   *attr_seq);
extern UINT16 sdpu_get_attrib_seq_len(tSDP_RECORD *p_rec, tSDP_ATTR_SEQ *attr_seq);
extern UINT16 sdpu_get_attrib_entry_len(tSDP_ATTRIBUTE *p_attr);
extern UINT8 *sdpu_build_partial_attrib_entry(UINT8 *p_out, tSDP_ATTRIBUTE *p_attr, UINT16 len, UINT16 *offset);


/* Functions provided by sdp_db.c
*/
extern tSDP_RECORD    *sdp_db_service_search(tSDP_RECORD *p_rec, tSDP_UUID_SEQ *p_seq);
extern tSDP_RECORD    *sdp_db_find_record(UINT32 handle);
extern tSDP_ATTRIBUTE *sdp_db_find_attr_in_rec(tSDP_RECORD *p_rec, UINT16 start_attr, UINT16 end_attr);


/* Functions provided by sdp_server.c
*/
#if SDP_SERVER_ENABLED == TRUE
extern void     sdp_server_handle_client_req(tCONN_CB *p_ccb, BT_HDR *p_msg);
#else
#define sdp_server_handle_client_req(p_ccb, p_msg)
#endif

/* Functions provided by sdp_discovery.c
*/
#if SDP_CLIENT_ENABLED == TRUE
extern void sdp_disc_connected(tCONN_CB *p_ccb);
extern void sdp_disc_server_rsp(tCONN_CB *p_ccb, BT_HDR *p_msg);
#else
#define sdp_disc_connected(p_ccb)
#define sdp_disc_server_rsp(p_ccb, p_msg)
#endif

#if defined(BT_TRACE_VERBOSE) && (BT_TRACE_VERBOSE == TRUE)
#include "trace_api.h"
#endif

#endif

