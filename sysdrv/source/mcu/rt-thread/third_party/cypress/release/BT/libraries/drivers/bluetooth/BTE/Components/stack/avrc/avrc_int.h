/*****************************************************************************
**
**  Name:       avrc_int.h
**
**  Description:AVRCP internal header file.
**
**  Copyright (c) 2003-2006, Broadcom Corp., All Rights Reserved.
**  Broadcom Bluetooth Core. Proprietary and confidential.
**
*****************************************************************************/


#ifndef AVRC_INT_H
#define AVRC_INT_H

#include "avct_defs.h"
#include "avrc_api.h"
#include "btm_cfg.h"

/*      DEBUG FLAGS
 *
 * #define META_DEBUG_ENABLED
 */
/*****************************************************************************
**  Constants
*****************************************************************************/

/* Number of attributes in AVRC SDP record. */
#define AVRC_NUM_ATTR            6

/* Number of protocol elements in protocol element list. */
#define AVRC_NUM_PROTO_ELEMS     2

#ifndef AVRC_MIN_CMD_LEN
#define AVRC_MIN_CMD_LEN    20
#endif

#define AVRC_UNIT_OPRND_BYTES   5
#define AVRC_SUB_OPRND_BYTES    4
#define AVRC_SUBRSP_OPRND_BYTES 3
#define AVRC_SUB_PAGE_MASK      7
#define AVRC_SUB_PAGE_SHIFT     4
#define AVRC_SUB_EXT_CODE       7
#define AVRC_PASS_OP_ID_MASK    0x7F
#define AVRC_PASS_STATE_MASK    0x80
#define AVRC_CMD_OPRND_PAD      0xFF

#define AVRC_CTYPE_MASK         0x0F
#define AVRC_SUBTYPE_MASK       0xF8
#define AVRC_SUBTYPE_SHIFT      3
#define AVRC_SUBID_MASK         0x07
#define AVRC_SUBID_IGNORE       0x07

#define AVRC_SINGLE_PARAM_SIZE      1
#define AVRC_METADATA_PKT_TYPE_MASK 0x03
#define AVRC_PASS_THOUGH_MSG_MASK   0x80           /* MSB of msg_type indicates the PAS THROUGH msg */
#define AVRC_VENDOR_UNIQUE_MASK     0x70           /* vendor unique id */


/* Company ID is 24-bit integer We can not use the macros in bt_types.h */
#define AVRC_CO_ID_TO_BE_STREAM(p, u32) {*(p)++ = (UINT8)((u32) >> 16); *(p)++ = (UINT8)((u32) >> 8); *(p)++ = (UINT8)(u32); }
#define AVRC_BE_STREAM_TO_CO_ID(u32, p) {u32 = (((UINT32)(*((p) + 2))) + (((UINT32)(*((p) + 1))) << 8) + (((UINT32)(*(p))) << 16)); (p) += 3;}

#define AVRC_AVC_HDR_SIZE           3   /* ctype, subunit*, opcode */

#define AVRC_MIN_META_HDR_SIZE      4   /* pdu id(1), packet type(1), param len(2) */
#define AVRC_MIN_BROWSE_HDR_SIZE    3   /* pdu id(1), param len(2) */

#define AVRC_VENDOR_HDR_SIZE        6   /* ctype, subunit*, opcode, CO_ID */
#define AVRC_MSG_VENDOR_OFFSET      23
#define AVRC_MIN_VENDOR_SIZE        (AVRC_MSG_VENDOR_OFFSET + BT_HDR_SIZE + AVRC_MIN_META_HDR_SIZE)

#define AVRC_PASS_THRU_SIZE         8
#define AVRC_MSG_PASS_THRU_OFFSET   25
#define AVRC_MIN_PASS_THRU_SIZE     (AVRC_MSG_PASS_THRU_OFFSET + BT_HDR_SIZE + 4)

#define AVRC_MIN_BROWSE_SIZE        (AVCT_BROWSE_OFFSET + BT_HDR_SIZE + AVRC_MIN_BROWSE_HDR_SIZE)

#define AVRC_CTRL_PKT_LEN(pf, pk)   {pf = (UINT8 *)((pk) + 1) + (pk)->offset + 2;}

#define AVRC_MAX_CTRL_DATA_LEN      (AVRC_PACKET_LEN)

/* Timeout for waiting for avrc command responses (in seconds) */
#ifndef AVRC_CMD_TOUT
#define AVRC_CMD_TOUT               2
#endif

/* Flags for avrc_cb.ccb_int[].flags */
#define AVRC_CB_FLAGS_RSP_PENDING   0x01        /* Waiting for AVRC response */

/*****************************************************************************
**  Type definitions
*****************************************************************************/

#if (AVRC_METADATA_INCLUDED == TRUE)
/* type for Metadata fragmentation control block */
typedef struct
{
    BT_HDR              *p_fmsg;        /* the fragmented message */
    UINT8               frag_pdu;       /* the PDU ID for fragmentation */
    BOOLEAN             frag_enabled;   /* fragmentation flag */
} tAVRC_FRAG_CB;

/* type for Metadata re-assembly control block */
typedef struct
{
    BT_HDR              *p_rmsg;        /* the received message */
    UINT16              rasm_offset;    /* re-assembly flag, the offset of the start fragment */
    UINT8               rasm_pdu;       /* the PDU ID for re-assembly */
} tAVRC_RASM_CB;
#endif

/* AVRC internal connection control block */
typedef struct
{
    BUFFER_Q            cmd_q;          /* Command queue for serializing vendor specific commands */
    UINT8               flags;          /* See AVRC_CB_FLAGS_* definitions */
    TIMER_LIST_ENT      tle;            /* Command timeout timer */
} tAVRC_CONN_INT_CB;

typedef struct
{
#if (defined(BTU_DYNAMIC_CB_INCLUDED)  && (BTU_DYNAMIC_CB_INCLUDED == TRUE))
    tAVRC_CONN_CB       *ccb;                   /* Connection control block from AVRC_Open API */
    tAVRC_CONN_INT_CB   *ccb_int;               /* Internal connection control block  */
#if (AVRC_METADATA_INCLUDED == TRUE)
    tAVRC_FRAG_CB       *fcb;
    tAVRC_RASM_CB       *rcb;
#endif
#else   /* BTU_DYNAMIC_CB_INCLUDED */
    tAVRC_CONN_CB       ccb[AVCT_NUM_CONN];     /* Connection control block from AVRC_Open API */
    tAVRC_CONN_INT_CB   ccb_int[AVCT_NUM_CONN]; /* Internal connection control block  */
#if (AVRC_METADATA_INCLUDED == TRUE)
    tAVRC_FRAG_CB       fcb[AVCT_NUM_CONN];
    tAVRC_RASM_CB       rcb[AVCT_NUM_CONN];
#endif
#endif /* BTU_DYNAMIC_CB_INCLUDED */
    tAVRC_FIND_CBACK    *p_cback;       /* pointer to application callback */
    tSDP_DISCOVERY_DB   *p_db;          /* pointer to discovery database */
    UINT16              service_uuid;   /* service UUID to search */
    UINT8               trace_level;
} tAVRC_CB;



#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
** Main Control Block
*******************************************************************************/
#if AVRC_DYNAMIC_MEMORY == FALSE
AVRC_API extern tAVRC_CB  avrc_cb;
#else
AVRC_API extern tAVRC_CB *avrc_cb_ptr;
#define avrc_cb (*avrc_cb_ptr)
#endif

extern BOOLEAN avrc_is_valid_pdu_id(UINT8 pdu_id);
extern BOOLEAN avrc_is_valid_player_attrib_value(UINT8 attrib, UINT8 value);
extern BT_HDR *avrc_alloc_ctrl_pkt(UINT8 pdu);
extern tAVRC_STS avrc_pars_pass_thru(tAVRC_MSG_PASS *p_msg, UINT16 *p_vendor_unique_id);
extern UINT8 avrc_opcode_from_pdu(UINT8 pdu);
extern BOOLEAN avrc_is_valid_opcode(UINT8 opcode);
extern void avrc_flush_cmd_q(UINT8 handle);
void avrc_start_cmd_timer(UINT8 handle, UINT8 label, UINT8 msg_mask);

#ifdef __cplusplus
}
#endif

#endif /* AVRC_INT_H */

