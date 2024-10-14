/*****************************************************************************
**
**  Name:           uipc_msg.h
**
**  Description:    This file contains sync message over UIPC
**
**  Copyright (c) 2002-2013, Broadcom Corp., All Rights Reserved.
**  Broadcom Bluetooth Core. Proprietary and confidential.
**
*****************************************************************************/
#ifndef UIPC_MSG_H
#define UIPC_MSG_H

#include "bt_types.h"

/****************************************************************************/
/*                            UIPC version number: 1.0                      */
/****************************************************************************/
#define UIPC_VERSION_MAJOR  0x0001
#define UIPC_VERSION_MINOR  0x0000


/********************************

    UIPC Management Messages

********************************/

/* tUIPC_STATUS codes*/
enum
{
    UIPC_STATUS_SUCCESS,
    UIPC_STATUS_FAIL
};
typedef UINT8 tUIPC_STATUS;

/* op_code */
#define UIPC_OPEN_REQ                   0x00
#define UIPC_OPEN_RSP                   0x01
#define UIPC_CLOSE_REQ                  0x02
#define UIPC_CLOSE_RSP                  0x03

/* Structure of UIPC_OPEN_REQ message */
typedef struct
{
    UINT8               opcode;         /* UIPC_OPEN_REQ */
} tUIPC_OPEN_REQ;
#define UIPC_OPEN_REQ_MSGLEN        (1)

/* Structure of UIPC_OPEN_RSP message */
typedef struct
{
    UINT8               opcode;         /* UIPC_OPEN_RESP */
    tUIPC_STATUS        status;         /* UIPC_STATUS */
    UINT16              version_major;  /* UIPC_VERSION_MAJOR */
    UINT16              version_minor;  /* UIPC_VERSION_MINOR */
    UINT8               num_streams;    /* Number of simultaneous streams supported by the light stack */
} tUIPC_OPEN_RSP;
#define UIPC_OPEN_RSP_MSGLEN        (7)

/* Structure of UIPC_CLOSE_REQ message */
typedef struct t_uipc_close_req
{
    UINT8               opcode;         /* UIPC_CLOSE_REQ */
} tUIPC_CLOSE_REQ;
#define UIPC_CLOSE_REQ_MSGLEN       (1)

/* Structure of UIPC_CLOSE_RSP message, only for BTC, full stack may ignore it */
typedef struct t_uipc_close_rsp
{
    UINT8               opcode;         /* UIPC_CLOSE_RSP */
} tUIPC_CLOSE_RSP;
#define UIPC_CLOSE_RSP_MSGLEN       (1)

/* UIPC management message structures */
typedef union
{
    UINT8               opcode;
    tUIPC_OPEN_REQ      open_req;
    tUIPC_OPEN_RSP      open_resp;
    tUIPC_CLOSE_REQ     close_req;
} tUIPC_MSG;

#define UIPC_MGMT_MSG_MAXLEN    (sizeof(tUIPC_MSG))

#define IPC_LOG_MSG_LEN  100
typedef struct t_uipc_log_msg
{
    UINT32              trace_set_mask;
    UINT8               msg[IPC_LOG_MSG_LEN];
} tUIPC_LOG_MSG;
#define UIPC_LOG_MSGLEN       (IPC_LOG_MSG_LEN + 4)

/********************************

    H5 Sync Message

********************************/

/* op_code */
#define SLIP_SYNC_TO_LITE_REQ        0
#define SLIP_SYNC_TO_LITE_RESP       1
#define SLIP_SYNC_TO_FULL_REQ        2
#define SLIP_SYNC_TO_FULL_RESP       3
#define SLIP_SYNC_NOTIFY             4

/* status */
#define SLIP_SYNC_SUCCESS            0
#define SLIP_SYNC_FAILURE            1

typedef struct
{
    UINT8       op_code;
    UINT8       status;
    UINT16      acl_pkt_size;
    UINT8       state;
    UINT8       lp_state;           /* Low Power state */
    UINT8       next_seqno;         /* next send seq */
    UINT8       ack;                /* next ack seq, expected seq from peer */
    UINT8       sent_ack;           /* last sent ack */
    UINT8       sliding_window_size;/* window size */
    BOOLEAN     oof_flow_control;   /* Out of Frame SW Flow Control */
    BOOLEAN     data_integrity_type;/* Level of Data Integrity Check */
    UINT8       rx_state;           /* rx state for incoming packet processing */
} tSLIP_SYNC_INFO;

/********************************

    L2CAP Sync Message

********************************/

/* op_code */
#define L2C_SYNC_TO_LITE_REQ        0
#define L2C_SYNC_TO_LITE_RESP       1
#define L2C_REMOVE_TO_LITE_REQ      2
#define L2C_REMOVE_TO_LITE_RESP     3
#define L2C_FLUSH_TO_FULL_IND       4

/* status */
#define L2C_SYNC_SUCCESS            0
#define L2C_SYNC_FAILURE            1

typedef struct t_l2c_stream_info
{
    UINT16  local_cid;          /* Local CID                        */
    UINT16  remote_cid;         /* Remote CID                       */
    UINT16  out_mtu;            /* Max MTU we will send             */
    UINT16  handle;             /* The handle used with LM          */
    UINT16  link_xmit_quota;    /* Num outstanding pkts allowed     */
    BOOLEAN is_flushable;       /* TRUE if flushable channel        */
} tL2C_STREAM_INFO;

typedef struct t_l2c_sync_to_lite_req
{
    UINT8   op_code;                       /* L2C_SYNC_TO_LITE_REQ */
    UINT16  light_xmit_quota;              /* Total quota for light stack    */
    UINT16  acl_data_size;                 /* Max ACL data size across HCI transport    */
    UINT16  non_flushable_pbf;             /* L2CAP_PKT_START_NON_FLUSHABLE if controller supports */
    /* Otherwise, L2CAP_PKT_START */
    UINT8   multi_av_data_cong_start;      /* Multi-AV queue size to start congestion */
    UINT8   multi_av_data_cong_end;        /* Multi-AV queue size to end congestion */
    UINT8   multi_av_data_cong_discard;    /* Multi-AV queue size to discard */
    UINT8   num_stream;
    tL2C_STREAM_INFO stream[BTM_SYNC_INFO_NUM_STR];
} tL2C_SYNC_TO_LITE_REQ;

typedef struct t_l2c_sync_to_lite_resp_stream
{
    UINT16  lcid;
    UINT8   status;
} tL2C_SYNC_TO_LITE_RESP_STREAM;

typedef struct t_l2c_sync_to_lite_resp
{
    UINT8   op_code;                       /* L2C_SYNC_TO_LITE_RESP */
    UINT16  light_xmit_unacked;            /* unacked packet more than quota in light stack    */
    UINT8   num_stream;
    tL2C_SYNC_TO_LITE_RESP_STREAM stream[BTM_SYNC_INFO_NUM_STR];
} tL2C_SYNC_TO_LITE_RESP;

typedef struct t_l2c_remove_to_lite_req
{
    UINT8   op_code;                       /* L2C_REMOVE_TO_LITE_REQ */
    UINT16  light_xmit_quota;              /* Total quota for light stack    */
    UINT8   num_stream;
    UINT16  lcid[BTM_SYNC_INFO_NUM_STR];
} tL2C_REMOVE_TO_LITE_REQ;

typedef tL2C_SYNC_TO_LITE_RESP  tL2C_REMOVE_TO_LITE_RESP;
typedef tL2C_REMOVE_TO_LITE_REQ tL2C_FLUSH_TO_FULL_IND;

typedef union t_l2c_sync_msg
{
    UINT8                       op_code;
    tL2C_SYNC_TO_LITE_REQ       sync_req;
    tL2C_SYNC_TO_LITE_RESP      sync_resp;
    tL2C_REMOVE_TO_LITE_REQ     remove_req;
    tL2C_REMOVE_TO_LITE_RESP    remove_resp;
    tL2C_FLUSH_TO_FULL_IND      flush_ind;
} tL2C_SYNC_MSG;

/********************************

    AVDTP Sync Message

********************************/

/* op_code */
#define AVDT_SYNC_TO_LITE_REQ        0
#define AVDT_SYNC_TO_LITE_RESP       1
#define AVDT_RESYNC_TO_LITE_REQ      2
#define AVDT_RESYNC_TO_LITE_RESP     3
#define AVDT_SYNC_TO_FULL_REQ        4
#define AVDT_SYNC_TO_FULL_RESP       5
#define AVDT_REMOVE_TO_LITE_REQ      6
#define AVDT_REMOVE_TO_LITE_RESP     7
#define AVDT_SYNC_TO_BTC_LITE_REQ    8
#define AVDT_SYNC_TO_BTC_LITE_RESP   9
#define AVDT_SYNC_CP_TO_BTC_LITE_REQ    0x0A
#define AVDT_SYNC_CP_TO_BTC_LITE_RESP   0x0B
#define AVDT_SYNC_CP_TO_LITE_REQ        0x0C
#define AVDT_SYNC_CP_TO_LITE_RESP       0x0D

/* status */
#define AVDT_SYNC_SUCCESS            0
#define AVDT_SYNC_FAILURE            1

#define AVDT_SYNC_CP_ID_NONE        0x0000
#define AVDT_SYNC_CP_ID_DTCP        0x0001  /* Not supported */
#define AVDT_SYNC_CP_ID_SCMS        0x0002


typedef struct
{
    UINT16  id;
    UINT8   scms_hdr;
} tAVDT_SYNC_CP;

typedef struct
{
    UINT16          lcid;
    tAVDT_SYNC_CP   cp;
} tAVDT_SYNC_CP_TO_BTC_LITE_REQ_STREAM;

typedef struct
{
    UINT8   opcode;                     /* AVDT_SYNC_CP_TO_BTC_LITE_REQ */
    UINT8   num_stream;
    tAVDT_SYNC_CP_TO_BTC_LITE_REQ_STREAM  stream[BTM_SYNC_INFO_NUM_STR];
} tAVDT_SYNC_CP_TO_BTC_LITE_REQ;

typedef struct
{
    UINT8   opcode;                     /* AVDT_SYNC_CP_TO_BTC_LITE_RESP */
    UINT8   status;
} tAVDT_SYNC_CP_TO_BTC_LITE_RESP;

typedef struct
{
    UINT16  lcid;
    UINT32  ssrc;
} tAVDT_SYNC_TO_BTC_LITE_REQ_STREAM;

typedef struct
{
    UINT8   opcode;                     /* AVDT_SYNC_TO_BTC_LITE_REQ */
    UINT8   num_stream;
    tAVDT_SYNC_TO_BTC_LITE_REQ_STREAM  stream[BTM_SYNC_INFO_NUM_STR];
} tAVDT_SYNC_TO_BTC_LITE_REQ;

typedef struct
{
    UINT8   opcode;                     /* AVDT_SYNC_TO_BTC_LITE_RESP */
    UINT8   status;
} tAVDT_SYNC_TO_BTC_LITE_RESP;

typedef struct t_avdt_scb_sync_info
{
    UINT8   handle;         /* SCB handle */
    BD_ADDR peer_addr;      /* BD address of peer */
    UINT16  local_cid;      /* Local CID                        */
    UINT16  peer_mtu;       /* L2CAP mtu of the peer device */
    UINT8   mux_tsid_media; /* TSID for media transport session */
    UINT16  media_seq;      /* media packet sequence number */
    tAVDT_SYNC_CP cp;       /* Content Protection */
} tAVDT_SCB_SYNC_INFO;

typedef struct t_avdt_sync_info
{
    UINT8   op_code;
    UINT8   status;

    tAVDT_SCB_SYNC_INFO scb_info[BTM_SYNC_INFO_NUM_STR];

} tAVDT_SYNC_INFO;

typedef union t_avdt_sync_msg
{
    UINT8                       op_code;
    tAVDT_SYNC_INFO             sync_info;
    tAVDT_SYNC_TO_BTC_LITE_REQ  btc_sync_req;
    tAVDT_SYNC_TO_BTC_LITE_RESP btc_sync_resp;
    tAVDT_SYNC_CP_TO_BTC_LITE_REQ   btc_sync_cp_req;
    tAVDT_SYNC_CP_TO_BTC_LITE_RESP  btc_sync_cp_resp;
} tAVDT_SYNC_MSG;

/********************************

    BTA AV Sync Message

********************************/

/* op_code for MM light stack */
#define BTA_AV_SYNC_TO_LITE_REQ             0
#define BTA_AV_SYNC_TO_LITE_RESP            1
#define BTA_AV_STR_START_TO_LITE_REQ        2
#define BTA_AV_STR_START_TO_LITE_RESP       3
#define BTA_AV_STR_STOP_TO_LITE_REQ         4
#define BTA_AV_STR_STOP_TO_LITE_RESP        5
#define BTA_AV_STR_CLEANUP_TO_LITE_REQ      6
#define BTA_AV_STR_CLEANUP_TO_LITE_RESP     7
#define BTA_AV_STR_SUSPEND_TO_LITE_REQ      8
#define BTA_AV_STR_SUSPEND_TO_LITE_RESP     9
#define BTA_AV_SYNC_ERROR_RESP              10

/* op_code for BTC light stack */
#define A2DP_START_REQ                      11
#define A2DP_START_RESP                     12
#define A2DP_STOP_REQ                       13
#define A2DP_STOP_RESP                      14
#define A2DP_CLEANUP_REQ                    15
#define A2DP_CLEANUP_RESP                   16
#define A2DP_SUSPEND_REQ                    17
#define A2DP_SUSPEND_RESP                   18

#define A2DP_JITTER_DONE_IND                41  /* For BTSNK */

#define AUDIO_CODEC_CONFIG_REQ              19
#define AUDIO_CODEC_CONFIG_RESP             20
#define AUDIO_CODEC_SET_BITRATE_REQ         21
#define AUDIO_CODEC_FLUSH_REQ               22
#define AUDIO_ROUTE_CONFIG_REQ              23
#define AUDIO_ROUTE_CONFIG_RESP             24
#define AUDIO_MIX_CONFIG_REQ                25
#define AUDIO_MIX_CONFIG_RESP               26
#define AUDIO_BURST_FRAMES_IND              27
#define AUDIO_BURST_END_IND                 28
#define AUDIO_EQ_MODE_CONFIG_REQ            29
#define AUDIO_SCALE_CONFIG_REQ              30

/* For TIVO, only applicable for I2S -> DAC */
#define AUDIO_SUB_ROUTE_REQ                 51
#define AUDIO_SUB_ROUTE_RESP                52

typedef struct
{
    UINT8   opcode;     /* A2DP_START_REQ */
    UINT16  lcid;
    UINT16  curr_mtu;
} tA2DP_START_REQ;

typedef struct
{
    UINT8   opcode;     /* A2DP_STOP_REQ */
    UINT16  lcid;
} tA2DP_STOP_REQ;

typedef struct
{
    UINT8   opcode;     /* A2DP_SUSPEND_REQ */
    UINT16  lcid;
} tA2DP_SUSPEND_REQ;

typedef struct
{
    UINT8   opcode;     /* A2DP_CLEANUP_REQ */
    UINT16  lcid;
    UINT16  curr_mtu;
} tA2DP_CLEANUP_REQ;

typedef struct
{
    UINT8   opcode;     /* A2DP_START_RESP, A2DP_STOP_RESP, A2DP_CLEANUP_RESP, A2DP_SUSPEND_RESP */
    UINT16  lcid;
} tA2DP_GENERIC_RESP;

#define AUDIO_CODEC_NONE            0x0000
#define AUDIO_CODEC_SBC_ENC         0x0001
#define AUDIO_CODEC_SBC_DEC         0x0002
#define AUDIO_CODEC_MP3_ENC         0x0004
#define AUDIO_CODEC_MP3_DEC         0x0008
#define AUDIO_CODEC_AAC_ENC         0x0010
#define AUDIO_CODEC_AAC_DEC         0x0020
#define AUDIO_CODEC_AAC_PLUS_ENC    0x0040
#define AUDIO_CODEC_AAC_PLUS_DEC    0x0080
#define AUDIO_CODEC_MP2_ENC         0x0100
#define AUDIO_CODEC_MP2_DEC         0x0200
#define AUDIO_CODEC_MP2_5_ENC       0x0400
#define AUDIO_CODEC_MP2_5_DEC       0x0800

typedef UINT16 tAUDIO_CODEC_TYPE;

/* SBC CODEC Parameters */

#define CODEC_INFO_SBC_SF_16K       0x00
#define CODEC_INFO_SBC_SF_32K       0x01
#define CODEC_INFO_SBC_SF_44K       0x02
#define CODEC_INFO_SBC_SF_48K       0x03

#define CODEC_INFO_SBC_BLOCK_4      0x00
#define CODEC_INFO_SBC_BLOCK_8      0x01
#define CODEC_INFO_SBC_BLOCK_12     0x02
#define CODEC_INFO_SBC_BLOCK_16     0x03

#define CODEC_INFO_SBC_CH_MONO      0x00
#define CODEC_INFO_SBC_CH_DUAL      0x01
#define CODEC_INFO_SBC_CH_STEREO    0x02
#define CODEC_INFO_SBC_CH_JS        0x03

#define CODEC_INFO_SBC_ALLOC_LOUDNESS   0x00
#define CODEC_INFO_SBC_ALLOC_SNR        0x01

#define CODEC_INFO_SBC_SUBBAND_4    0x00
#define CODEC_INFO_SBC_SUBBAND_8    0x01

/* MPEG audio version ID */
#define CODEC_INFO_MP25_ID              0x00
#define CODEC_INFO_RESERVE              0x01
#define CODEC_INFO_MP2_ID               0x02
#define CODEC_INFO_MP3_ID               0x03

#define CODEC_INFO_MP3_PROTECTION_ON    0x00
#define CODEC_INFO_MP3_PROTECTION_OFF   0x01

#define CODEC_INFO_MP3_BR_IDX_FREE      0x00
#define CODEC_INFO_MP3_BR_IDX_32K       0x01
#define CODEC_INFO_MP3_BR_IDX_40K       0x02
#define CODEC_INFO_MP3_BR_IDX_48K       0x03
#define CODEC_INFO_MP3_BR_IDX_56K       0x04
#define CODEC_INFO_MP3_BR_IDX_64K       0x05
#define CODEC_INFO_MP3_BR_IDX_80K       0x06
#define CODEC_INFO_MP3_BR_IDX_96K       0x07
#define CODEC_INFO_MP3_BR_IDX_112K      0x08
#define CODEC_INFO_MP3_BR_IDX_128K      0x09
#define CODEC_INFO_MP3_BR_IDX_160K      0x0A
#define CODEC_INFO_MP3_BR_IDX_192K      0x0B
#define CODEC_INFO_MP3_BR_IDX_224K      0x0C
#define CODEC_INFO_MP3_BR_IDX_256K      0x0D
#define CODEC_INFO_MP3_BR_IDX_320K      0x0E

#define CODEC_INFO_MP3_SF_44K           0x00
#define CODEC_INFO_MP3_SF_48K           0x01
#define CODEC_INFO_MP3_SF_32K           0x02

#define CODEC_INFO_MP3_MODE_STEREO      0x00
#define CODEC_INFO_MP3_MODE_JS          0x01
#define CODEC_INFO_MP3_MODE_DUAL        0x02
#define CODEC_INFO_MP3_MODE_SINGLE      0x03

/* layer 3, type of joint stereo coding method (intensity and ms) */
#define CODEC_INFO_MP3_MODE_EXT_OFF_OFF 0x00
#define CODEC_INFO_MP3_MODE_EXT_ON_OFF  0x01
#define CODEC_INFO_MP3_MODE_EXT_OFF_ON  0x02
#define CODEC_INFO_MP3_MODE_EXT_ON_ON   0x03


#define CODEC_INFO_MP2_PROTECTION_ON    0x00
#define CODEC_INFO_MP2_PROTECTION_OFF   0x01

#define CODEC_INFO_MP2_BR_IDX_FREE      0x00
#define CODEC_INFO_MP2_BR_IDX_8K        0x01
#define CODEC_INFO_MP2_BR_IDX_16K       0x02
#define CODEC_INFO_MP2_BR_IDX_24K       0x03
#define CODEC_INFO_MP2_BR_IDX_32K       0x04
#define CODEC_INFO_MP2_BR_IDX_40K       0x05
#define CODEC_INFO_MP2_BR_IDX_48K       0x06
#define CODEC_INFO_MP2_BR_IDX_56K       0x07
#define CODEC_INFO_MP2_BR_IDX_64K       0x08
#define CODEC_INFO_MP2_BR_IDX_80K       0x09
#define CODEC_INFO_MP2_BR_IDX_96K       0x0A
#define CODEC_INFO_MP2_BR_IDX_112K      0x0B
#define CODEC_INFO_MP2_BR_IDX_128K      0x0C
#define CODEC_INFO_MP2_BR_IDX_144K      0x0D
#define CODEC_INFO_MP2_BR_IDX_160K      0x0E

#define CODEC_INFO_MP2_SF_22K           0x00
#define CODEC_INFO_MP2_SF_24K           0x01
#define CODEC_INFO_MP2_SF_16K           0x02

#define CODEC_INFO_MP2_MODE_STEREO      0x00
#define CODEC_INFO_MP2_MODE_JS          0x01
#define CODEC_INFO_MP2_MODE_DUAL        0x02
#define CODEC_INFO_MP2_MODE_SINGLE      0x03

/* layer 3, type of joint stereo coding method (intensity and ms) */
#define CODEC_INFO_MP2_MODE_EXT_OFF_OFF 0x00
#define CODEC_INFO_MP2_MODE_EXT_ON_OFF  0x01
#define CODEC_INFO_MP2_MODE_EXT_OFF_ON  0x02
#define CODEC_INFO_MP2_MODE_EXT_ON_ON   0x03

#define CODEC_INFO_MP2_SAMPLE_PER_FRAME     576

/* mpeg 2.5 layer 3 decoder */

#define CODEC_INFO_MP25_PROTECTION_ON   0x00
#define CODEC_INFO_MP25_PROTECTION_OFF  0x01

#define CODEC_INFO_MP25_BR_IDX_FREE     0x00
#define CODEC_INFO_MP25_BR_IDX_8K       0x01
#define CODEC_INFO_MP25_BR_IDX_16K      0x02
#define CODEC_INFO_MP25_BR_IDX_24K      0x03
#define CODEC_INFO_MP25_BR_IDX_32K      0x04
#define CODEC_INFO_MP25_BR_IDX_40K      0x05
#define CODEC_INFO_MP25_BR_IDX_48K      0x06
#define CODEC_INFO_MP25_BR_IDX_56K      0x07
#define CODEC_INFO_MP25_BR_IDX_64K      0x08
#define CODEC_INFO_MP25_BR_IDX_80K      0x09
#define CODEC_INFO_MP25_BR_IDX_96K      0x0A
#define CODEC_INFO_MP25_BR_IDX_112K     0x0B
#define CODEC_INFO_MP25_BR_IDX_128K     0x0C
#define CODEC_INFO_MP25_BR_IDX_144K     0x0D
#define CODEC_INFO_MP25_BR_IDX_160K     0x0E

#define CODEC_INFO_MP25_SF_11K          0x00
#define CODEC_INFO_MP25_SF_12K          0x01
#define CODEC_INFO_MP25_SF_8K           0x02

#define CODEC_INFO_MP25_MODE_STEREO     0x00
#define CODEC_INFO_MP25_MODE_JS         0x01
#define CODEC_INFO_MP25_MODE_DUAL       0x02
#define CODEC_INFO_MP25_MODE_SINGLE     0x03

/* layer 3, type of joint stereo coding method (intensity and ms) */
#define CODEC_INFO_MP25_MODE_EXT_OFF_OFF 0x00
#define CODEC_INFO_MP25_MODE_EXT_ON_OFF  0x01
#define CODEC_INFO_MP25_MODE_EXT_OFF_ON  0x02
#define CODEC_INFO_MP25_MODE_EXT_ON_ON   0x03

#define CODEC_INFO_MP25_SAMPLE_PER_FRAME    576

/* AAC/AAC+ CODEC Parameters */
#define CODEC_INFO_AAC_SF_IDX_96K   0x0
#define CODEC_INFO_AAC_SF_IDX_88K   0x1
#define CODEC_INFO_AAC_SF_IDX_64K   0x2
#define CODEC_INFO_AAC_SF_IDX_48K   0x3
#define CODEC_INFO_AAC_SF_IDX_44K   0x4
#define CODEC_INFO_AAC_SF_IDX_32K   0x5
#define CODEC_INFO_AAC_SF_IDX_24K   0x6
#define CODEC_INFO_AAC_SF_IDX_22K   0x7
#define CODEC_INFO_AAC_SF_IDX_16K   0x8
#define CODEC_INFO_AAC_SF_IDX_12K   0x9
#define CODEC_INFO_AAC_SF_IDX_11K   0xA
#define CODEC_INFO_AAC_SF_IDX_08K   0xB
#define CODEC_INFO_AAC_SF_IDX_RESERVE   0xC

#define CODEC_INFO_AAC_BR_RATE_48K  288000
#define CODEC_INFO_AAC_BR_RATE_44K  264600
#define CODEC_INFO_AAC_BR_RATE_32K  192000


#define CODEC_INFO_AAC_1_CH           1         /*center front speaker */
#define CODEC_INFO_AAC_2_CH           2         /*left, right front speaker */
#define CODEC_INFO_AAC_3_CH           3         /*center front speaker, left right front speaker */
#define CODEC_INFO_AAC_4_CH           4         /*center/rear front speaker, left/right front speaker */
#define CODEC_INFO_AAC_5_CH           5         /*center, left, right front speaker, left/right surround */
#define CODEC_INFO_AAC_6_CH           6         /*center, left, right front speaker, left/right surround, LFE */
#define CODEC_INFO_AAC_7_CH           7         /*(left, right)center/left,right front speaker, left/right surround, LFE */


typedef struct
{
    UINT8   sampling_freq;
    UINT8   channel_mode;
    UINT8   block_length;
    UINT8   num_subbands;
    UINT8   alloc_method;
    UINT8   bitpool_size;   /* 2 - 250 */
} tCODEC_INFO_SBC;

typedef struct
{
    UINT8   ch_mode;
    UINT8   sampling_freq;
    UINT8   bitrate_index;  /* 0 - 14 */
} tCODEC_INFO_MP3;

typedef struct
{
    UINT8   ch_mode;
    UINT8   sampling_freq;
    UINT8   bitrate_index;  /* 0 - 14 */
} tCODEC_INFO_MP2;


typedef struct
{
    UINT8   ch_mode;
    UINT8   sampling_freq;
    UINT8   bitrate_index;  /* 0 - 14 */
} tCODEC_INFO_MP2_5;

typedef struct
{
    UINT16  sampling_freq;
    UINT8   channel_mode;   /* 0x02:mono, 0x01:dual */
    UINT32  bitrate;        /* 0 - 320K */
    UINT32  sbr_profile;        /* 1: ON, 0: OFF */
} tCODEC_INFO_AAC;

typedef union
{
    tCODEC_INFO_SBC     sbc;
    tCODEC_INFO_MP3     mp3;
    tCODEC_INFO_MP2     mp2;
    tCODEC_INFO_MP2_5   mp2_5;
    tCODEC_INFO_AAC     aac;
} tCODEC_INFO;

typedef struct
{
    UINT8               opcode;     /* AUDIO_CODEC_CONFIG_REQ */
    tAUDIO_CODEC_TYPE   codec_type;
    tCODEC_INFO         codec_info;
} tAUDIO_CODEC_CONFIG_REQ;

#define AUDIO_CONFIG_SUCCESS            0x00
#define AUDIO_CONFIG_NOT_SUPPORTED      0x01
#define AUDIO_CONFIG_FAIL_OUT_OF_MEMORY 0x02
#define AUDIO_CONFIG_FAIL_CODEC_USED    0x03
#define AUDIO_CONFIG_FAIL_ROUTE         0x04
typedef UINT8 tAUDIO_CONFIG_STATUS;

typedef struct
{
    UINT8                   opcode; /* AUDIO_CODEC_CONFIG_RESP */
    tAUDIO_CONFIG_STATUS    status;
} tAUDIO_CODEC_CONFIG_RESP;

typedef struct
{
    UINT8               opcode;     /* AUDIO_CODEC_SET_BITRATE_REQ */
    tAUDIO_CODEC_TYPE   codec_type;
    union
    {
        UINT8   sbc;
        UINT8   mp3;
        UINT32  aac;
    } codec_bitrate;
} tAUDIO_CODEC_SET_BITRATE_REQ;

#define AUDIO_ROUTE_SRC_FMRX        0x00
#define AUDIO_ROUTE_SRC_I2S         0x01
#define AUDIO_ROUTE_SRC_ADC         0x02
#define AUDIO_ROUTE_SRC_HOST        0x03
#define AUDIO_ROUTE_SRC_PTU         0x04
#define AUDIO_ROUTE_SRC_BTSNK       0x05
#define AUDIO_ROUTE_SRC_NONE        0x80
#define MAX_AUDIO_ROUTE_SRC         6
typedef UINT8 tAUDIO_ROUTE_SRC;

#define AUDIO_ROUTE_MIX_NONE        0x00
#define AUDIO_ROUTE_MIX_HOST        0x01
#define AUDIO_ROUTE_MIX_PCM         0x02
#define AUDIO_ROUTE_MIX_CHIRP       0x03
#define AUDIO_ROUTE_MIX_I2S         0x04
#define AUDIO_ROUTE_MIX_ADC         0x05
#define AUDIO_ROUTE_MIX_RESERVED    0x06
#define MAX_AUDIO_ROUTE_MIX         7
typedef UINT8 tAUDIO_ROUTE_MIX;

#define AUDIO_ROUTE_OUT_NONE        0x0000
#define AUDIO_ROUTE_OUT_BTA2DP      0x0001
#define AUDIO_ROUTE_OUT_FMTX        0x0002
#define AUDIO_ROUTE_OUT_BTSCO       0x0004
#define AUDIO_ROUTE_OUT_HOST        0x0008
#define AUDIO_ROUTE_OUT_DAC         0x0010
#define AUDIO_ROUTE_OUT_I2S         0x0020
#define AUDIO_ROUTE_OUT_BTA2DP_DAC  0x0040
#define AUDIO_ROUTE_OUT_BTA2DP_I2S  0x0080
#define AUDIO_ROUTE_OUT_BTSCO_DAC   0x0100
#define AUDIO_ROUTE_OUT_BTSCO_I2S   0x0200
#define AUDIO_ROUTE_OUT_HOST_BTA2DP 0x0400
#define AUDIO_ROUTE_OUT_HOST_BTSCO  0x0800
#define AUDIO_ROUTE_OUT_HOST_DAC    0x1000
#define AUDIO_ROUTE_OUT_HOST_I2S    0x2000
#define AUDIO_ROUTE_OUT_DAC_I2S     0x4000
#define AUDIO_ROUTE_OUT_RESERVED_2  0x8000

#define MAX_AUDIO_SINGLE_ROUTE_OUT  6
#define MAX_AUDIO_MULTI_ROUTE_OUT   16
typedef UINT16 tAUDIO_MULTI_ROUTE_OUT;
typedef UINT8  tAUDIO_ROUTE_OUT;

#define AUDIO_ROUTE_SF_8K           0x00
#define AUDIO_ROUTE_SF_16K          0x01
#define AUDIO_ROUTE_SF_32K          0x02
#define AUDIO_ROUTE_SF_44_1K        0x03
#define AUDIO_ROUTE_SF_48K          0x04
#define AUDIO_ROUTE_SF_11K          0x05
#define AUDIO_ROUTE_SF_12K          0x06
#define AUDIO_ROUTE_SF_22K          0x07
#define AUDIO_ROUTE_SF_24K          0x08
#define AUDIO_ROUTE_SF_NA           0xFF
typedef UINT8 tAUDIO_ROUTE_SF;

#define AUDIO_ROUTE_EQ_BASS_BOOST   0x00
#define AUDIO_ROUTE_EQ_CLASSIC      0x01
#define AUDIO_ROUTE_EQ_JAZZ         0x02
#define AUDIO_ROUTE_EQ_LIVE         0x03
#define AUDIO_ROUTE_EQ_NORMAL       0x04
#define AUDIO_ROUTE_EQ_ROCK         0x05
#define AUDIO_ROUTE_EQ_BYPASS       0x06

#define AUDIO_ROUTE_DIGITAL_VOLUME_CONTROL  0x07

#define AUDIO_ROUTE_EQ_CONFIG_GAIN  0xFF    /* Custion Gain Config */
typedef UINT8 tAUDIO_ROUTE_EQ;

typedef struct
{
    UINT8               opcode;     /* AUDIO_ROUTE_CONFIG_REQ */
    tAUDIO_ROUTE_SRC    src;
    tAUDIO_ROUTE_SF     src_sf;
    tAUDIO_ROUTE_OUT    out;
    tAUDIO_ROUTE_SF     out_codec_sf;
    tAUDIO_ROUTE_SF     out_i2s_sf;
    tAUDIO_ROUTE_EQ     eq_mode;
} tAUDIO_ROUTE_CONFIG_REQ;

typedef struct
{
    UINT8                   opcode; /* AUDIO_ROUTE_CONFIG_RESP */
    tAUDIO_CONFIG_STATUS    status;
} tAUDIO_ROUTE_CONFIG_RESP;

typedef struct
{
    UINT16  amp[2];                 /* left/right 15 bit amplitude value        */
    UINT16  tone[2];                /* left/right 12 bit frequency 0 - 4096Hz   */
    UINT16  mark[2];                /* left/right 16 bit mark time 0 - 65535ms  */
    UINT16  space[2];               /* left/right 16 bit space time 0 - 65535ms */
} tCHIRP_CONFIG;

typedef struct
{
    UINT8   pri_l;                  /* Primary Left scale : 0 ~ 255     */
    UINT8   mix_l;                  /* Mixing Left scale : 0 ~ 255      */
    UINT8   pri_r;                  /* Primary Right scale : 0 ~ 255    */
    UINT8   mix_r;                  /* Mixing Right scale : 0 ~ 255     */
} tMIX_SCALE_CONFIG;

/* For custon equalizer gain configuration */
typedef struct
{
    UINT32  audio_l_g0;         /* IIR biquad filter left ch gain 0 */
    UINT32  audio_l_g1;         /* IIR biquad filter left ch gain 1 */
    UINT32  audio_l_g2;         /* IIR biquad filter left ch gain 2 */
    UINT32  audio_l_g3;         /* IIR biquad filter left ch gain 3 */
    UINT32  audio_l_g4;         /* IIR biquad filter left ch gain 4 */
    UINT32  audio_l_gl;         /* IIR biquad filter left ch global gain  */
    UINT32  audio_r_g0;         /* IIR biquad filter left ch gain 0 */
    UINT32  audio_r_g1;         /* IIR biquad filter left ch gain 1 */
    UINT32  audio_r_g2;         /* IIR biquad filter left ch gain 2 */
    UINT32  audio_r_g3;         /* IIR biquad filter left ch gain 3 */
    UINT32  audio_r_g4;         /* IIR biquad filter left ch gain 4 */
    UINT32  audio_r_gl;         /* IIR biquad filter left ch global gain */
} tEQ_GAIN_CONFIG;

typedef struct
{
    UINT8               opcode;     /* AUDIO_MIX_CONFIG_REQ */
    tAUDIO_ROUTE_MIX    mix_src;
    tAUDIO_ROUTE_SF     mix_src_sf;
    tMIX_SCALE_CONFIG   mix_scale;
    tCHIRP_CONFIG       chirp_config;
} tAUDIO_MIX_CONFIG_REQ;

typedef struct
{
    UINT8                   opcode; /* AUDIO_MIX_CONFIG_RESP */
    tAUDIO_CONFIG_STATUS    status;
} tAUDIO_MIX_CONFIG_RESP;


typedef struct
{
    UINT8   opcode;                 /* AUDIO_BURST_FRAMES_IND */
    UINT32  burst_size;             /* in bytes */
} tAUDIO_BURST_FRAMES_IND;

typedef struct
{
    UINT8   opcode;                 /* AUDIO_BURST_END_IND */
} tAUDIO_BURST_END_IND;

typedef struct
{
    UINT8   opcode;                 /* AUDIO_CODEC_FLUSH_REQ */
} tAUDIO_CODEC_FLUSH_REQ;

typedef struct
{
    UINT8               opcode;     /* AUDIO_EQ_MODE_CONFIG_REQ */
    tAUDIO_ROUTE_EQ     eq_mode;
    tEQ_GAIN_CONFIG     filter_gain;    /* Valid only when eq_mode is 0xFF */
} tAUDIO_EQ_MODE_CONFIG_REQ;

typedef struct
{
    UINT8               opcode;     /* AUDIO_SCALE_CONFIG_REQ */
    tMIX_SCALE_CONFIG   mix_scale;
} tAUDIO_SCALE_CONFIG_REQ;

typedef UINT8 tBTA_AV_DUAL_STACK_EVT;

typedef struct
{
    UINT8               avdt_handle;    /* AVDTP handle */
    UINT8               chnl;           /* the channel: audio/video */
    UINT8               codec_type;     /* codec type */
    BOOLEAN             cong;           /* TRUE if AVDTP congested */
    UINT8               hdi;            /* the index to SCB[] */
    UINT8               hndl;           /* the handle: ((hdi + 1)|chnl) */
    UINT8               l2c_bufs;       /* the number of buffers queued to L2CAP */
    UINT16              l2c_cid;        /* L2CAP channel ID */
    BD_ADDR             peer_addr;      /* peer BD address */
} tBTA_AV_SYNC_INFO;

typedef struct
{
    tBTA_AV_DUAL_STACK_EVT  event;
    tBTA_AV_SYNC_INFO       sync_info;
    UINT16                  curr_mtu;                 /* common mtu shared by all active streams */
    UINT8                   multi_av_supported;       /* Whether multi-av is supported */
} tBTA_AV_SYNC_INFO_REQ; /* SYNC_TO_LITE_REQ */

/* Dual stack stream events */
typedef struct
{
    tBTA_AV_DUAL_STACK_EVT          event;
    UINT8                           scb_idx;
} tBTA_AV_SCB_EVT;

/* data type for the Audio Codec Information*/
typedef struct
{
    UINT16  bit_rate;                   /* SBC encoder bit rate in kbps */
    UINT16  bit_rate_busy;              /* SBC encoder bit rate in kbps */
    UINT16  bit_rate_swampd;            /* SBC encoder bit rate in kbps */
    UINT8   busy_level;                 /* Busy level indicating the bit-rate to be used */
    UINT8   codec_info[AVDT_CODEC_SIZE];
    UINT8   codec_type;                 /* Codec type */
} tBTA_AV_AUDIO_CODEC_SYNC_INFO;

/* Dual stack stream events */
typedef struct
{
    tBTA_AV_DUAL_STACK_EVT          event;
    UINT8                           scb_idx;
    UINT8                           audio_open_cnt;
    tBTA_AV_AUDIO_CODEC_SYNC_INFO   p_codec_cfg;
    UINT8                           start_stop_flag;
} tBTA_AV_SCB_REQ;

typedef struct
{
    tBTA_AV_DUAL_STACK_EVT          event;
    UINT8                           scb_idx;
    UINT8                           audio_open_cnt;
    UINT16                          curr_mtu;           /* common mtu shared by all active streams */
} tBTA_AV_SCB_CLEANUP_REQ;

/* Add request/response structures if needed ...
typedef struct
{
    event;
    data;
}tBTA_AV_SYNC_*_REQ/RESP;
*/

typedef union
{
    /* MM light stack */
    tBTA_AV_DUAL_STACK_EVT          event;
    tBTA_AV_SYNC_INFO_REQ           sync_info_req;
    tBTA_AV_SCB_EVT                 scb_evt;
    tBTA_AV_SCB_REQ                 scb_req;
    tBTA_AV_SCB_CLEANUP_REQ         scb_cleanup_req;

    /* BTC light stack */
    UINT8                           opcode;
    tA2DP_START_REQ                 btc_start_req;
    tA2DP_STOP_REQ                  btc_stop_req;
    tA2DP_CLEANUP_REQ               btc_cleanup_req;
    tA2DP_SUSPEND_REQ               btc_suspend_req;

    tAUDIO_CODEC_CONFIG_REQ         codec_config_req;
    tAUDIO_CODEC_SET_BITRATE_REQ    codec_bitrate_req;
    tAUDIO_CODEC_FLUSH_REQ          codec_flush_req;
    tAUDIO_ROUTE_CONFIG_REQ         route_config_req;
    tAUDIO_MIX_CONFIG_REQ           mix_config_req;
    tAUDIO_EQ_MODE_CONFIG_REQ       eq_mode_req;
    tAUDIO_SCALE_CONFIG_REQ         scale_config_req;
} tBTA_DUAL_STACK_MSG;

#endif /* UIPC_MSG_H */
