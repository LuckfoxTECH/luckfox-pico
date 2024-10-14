/*****************************************************************************/
/*                                                                           */
/*  Name:          hcid.c                                                    */
/*                                                                           */
/*  Description:   Internal definitions for HCID unit                        */
/*                                                                           */
/*  Date        Modification                                                 */
/*  ------------------------                                                 */
/*  07/20/99    Victor  Create                                               */
/*  08/03/99    Victor  Modifications made for BT specification V1.0a        */
/*                                                                           */
/*  Copyright (c) 1999-2004, WIDCOMM Inc., All Rights Reserved.              */
/*  WIDCOMM Bluetooth Core. Proprietary and confidential.                    */
/*                                                                           */
/*****************************************************************************/

#ifndef HCIDINT_H
#define HCIDINT_H


typedef struct
{
    UINT8    conn_id;           /* Connection id */
    UINT8    am_addr;           /* AM_ADDR */
    UINT16   handle;            /* Connection Handle */

#define HCID_STATE_IDLE         0
#define HCID_STATE_ACTIVE       1
#define HCID_STATE_SUSPENDING   2
#define HCID_STATE_SUSPENDED    3

    UINT8    state;

#define HCID_FLUSHING_STATE_NONE        0
#define HCID_FLUSHING_STATE_SENT        1
#define HCID_FLUSHING_STATE_NOT_SENT    2

    UINT8    flushing_state;

    UINT16   requested_packet_type; /* Bitmap of packet types requested by LM */
    UINT16   packet_type;           /* Bitmap of packet types adjusted by data rate */

    UINT8    num_pkts_no_ack;   /* Num packets sent to LC without tx_complete request */
    UINT8    pkts_to_send_dn;   /* Number of packet that can be sent to the LC */
    UINT16   lc_buffer_size;    /* Size of LC buffer */

    UINT8    loopback_mode;     /* Loopback mode as set by the host */

    BOOLEAN  data_rate_set;     /* TRUE if LM requested preferred rate */
    UINT8    data_rate;         /* Preferred rate requested by LM */

    UINT16   retransmit_timeout;   /* Timeout before auto flash for data in clk1 */
    UINT16   transmit_time_start;  /* Time stamp when first portion of L2CAP pkt went out */

    UINT16   num_pkts_not_acked_to_host;  /* Num ACL packets acked by LC but not acked to host */
    UINT16   num_pkts_not_acked_from_host;/* Num ACL packets sent to the host but not acked */

    UINT16   failed_contact_counter;     /* num consecutive failed contacts */

    UINT32   num_tx_bytes;      /* Number ACL bytes transmitted */
    UINT32   num_rx_bytes;      /* Number ACL bytes recieved */

    BUFFER_Q up_frames;         /* Frames waiting to be sent to the host */
    BUFFER_Q dn_frames;         /* Frames waiting to be sent to the LC */
} ACLDCB;

typedef struct
{
    BOOLEAN     is_flow_enabled;    /* If TRUE flow control towards Host is enabled */
    UINT16      max_up_acl_len;     /* Max len of ACL packet to be sent to the host */
    UINT16      max_up_sco_len;     /* Max len of SCO packet to be sent to the host */
    UINT16      max_up_acl_num;     /* Number of outstanding ACL packet host can accept */
    UINT16      max_up_sco_num;     /* Number of outstanding SCO packet host can accept */
    UINT16      pkts_to_send_up;    /* Number of packet that can be sent to the host */
    UINT16      num_acl_pkts_not_acked_to_host; /* Num ACL packets acked by LC but not acked to host */
    UINT16      num_sco_pkts_not_acked_to_host; /* Num SCO packets acked by LC but not acked to host */
#define HCID_MAX_ACL_PIPES      8

    ACLDCB      acld[HCID_MAX_ACL_PIPES]; /* Control block per pipe */
} HCIDCB;

extern HCIDCB hcidcb;

#endif
