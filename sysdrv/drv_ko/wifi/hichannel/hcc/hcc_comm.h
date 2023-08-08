/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Header file for hcc_comm.c
 * Author: Hisilicon
 * Create: 2019-08-04
 */

#ifndef __HCC_COMM_H
#define __HCC_COMM_H

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "hi_types.h"
#include "osal_list.h"
#include "oal_spinlock.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  2 宏定义
*****************************************************************************/
#define hcc_get_event_hdr(pst_event)        ((pst_event)->event_hdr)
#define hcc_get_event_payload(pst_event)    ((hi_u8*)((pst_event)->auc_event_data))
#define hcc_get_event_len(pst_event)        ((pst_event)->event_hdr.us_length)

#define hcc_field_setup(_p, _m, _v)         ((_p)->_m = (_v))

/* 事件头初始化宏 */
#define hcc_mac_event_hdr_init(_pst_event_hdr, _en_type, _uc_sub_type, _uc_chip_id, \
                                _uc_device_id, _uc_vap_id, _us_length) \
    do { \
        hcc_field_setup((_pst_event_hdr), en_type, (_en_type)); \
        hcc_field_setup((_pst_event_hdr), uc_sub_type, (_uc_sub_type)); \
        hcc_field_setup((_pst_event_hdr), uc_chip_id, (_uc_chip_id)); \
        hcc_field_setup((_pst_event_hdr), uc_device_id, (_uc_device_id)); \
        hcc_field_setup((_pst_event_hdr), uc_vap_id, (_uc_vap_id)); \
        hcc_field_setup((_pst_event_hdr), us_length, (_us_length)); \
    }while (0)

#define HCC_TEST_CMD_START_TX          0
#define HCC_TEST_CMD_START_RX          1
#define HCC_TEST_CMD_START_LOOP        2
#define HCC_TEST_CMD_STOP_TEST         3

/* hcc tx transfer flow control */
#define HCC_FC_NONE         0x0   /* 无流控 */
#define HCC_FC_WAIT         0x1   /* 阻塞等待，如果是在中断上下文调用，该标记被自动清除,非中断上下文生效 */
#define HCC_FC_NET          0x2   /* 对于网络层的流控 */
#define HCC_FC_DROP         0x4   /* 流控采用丢包方式,流控时返回成功 */
#define HCC_FC_ALL          (HCC_FC_WAIT | HCC_FC_NET | HCC_FC_DROP)

#define HCC_CONFIG_FRAME                0x80

#undef CONFIG_HCC_DEBUG
#undef CONFIG_HCC_TX_MULTI_BUF_CHECK
#define CONFIG_HCC_HEADER_CHECK_SUM

/*****************************************************************************
  2 结构体定义
*****************************************************************************/
typedef enum {
    HCC_NORMAL_QUEUE = 0,    /* netbuf is shared with others */
    HCC_HIGH_QUEUE = 1,      /* netbuf is special for high pri */
    HCC_QUEUE_BUTT
} hcc_netbuf_queue_type_enum;

/*****************************************************************************
  结构名  : frw_event_hdr_stru
  结构说明: 事件头结构体,
  备注    : uc_length的值为(payload长度 + 事件头长度 - 2)
*****************************************************************************/
typedef struct {
    hi_u8           type;         /* 事件类型 */
    hi_u8           sub_type;     /* 事件子类型 */
    hi_u8           chip_id;      /* 芯片ID */
    hi_u8           device_id;    /* 设备ID */
    hi_u8           vap_id;       /* VAP ID */
    hi_u8           resever;      /* 保留 */
    hi_u16          us_length;    /* payload length */
} hcc_mac_event_hdr_stru;

typedef enum {
    HCC_ACTION_TYPE_WIFI = 0, /* data from wifi */
    HCC_ACTION_TYPE_OAM = 1,  /* data from oam,sdt etc. */
    HCC_ACTION_TYPE_TEST = 2, /* used for hcc transfer test */
    HCC_ACTION_TYPE_BUTT
} hcc_action_type_enum;

typedef enum _wifi_sub_type_ {
    WIFI_CONTROL_TYPE       = 0,
    WIFI_DATA_TYPE          = 1,
    WIFI_SUB_TYPE_BUTT,
} wifi_sub_type;

typedef enum _oam_sub_type_ {
    DEAULT_VALUE        = 0,
    DUMP_REG            = 1,
    DUMP_MEM            = 2,
} oam_sub_type;

typedef enum {
    HCC_WIFI_SUB_TYPE_CONTROL  = 0,
    HCC_WIFI_SUB_TYPE_DATA     = 1,
    HCC_WIFI_SUB_TYPE_BUTT,
} hcc_event_wifi_sub_type;

typedef struct {
    hi_u32 main_type;
    hi_u32 sub_type;
    hi_u8  extend_len;
    hi_u32 fc_flag;
    hi_u32 queue_id;
    hi_u8  resv[3];    /* reserved bytes: 3. */
} hcc_transfer_param;

/* hcc protocol header
|-------hcc total(64B)-----|-----------package mem--------------|
|hcc hdr|pad hdr|hcc extend|pad_payload|--------payload---------|
*/
#pragma pack(push, 1)
/* 4bytes */
struct hcc_header {
    hi_u16      sub_type : 4;        /* sub type to hcc type,refer to hcc_action_type */
    hi_u16      main_type : 3;       /* main type to hcc type,refer to hcc_action_type */
    hi_u16      pad_hdr : 6;         /* pad_hdr only need 6 bits, pad_hdr used to algin hcc tcm hdr(64B) */
    hi_u16      pad_align : 2;       /* the pad to align the payload addr */
    hi_u16      more : 1;            /* for hcc aggregation */
    hi_u16      seq : 4;             /* seq num for debug */
    hi_u16      pay_len : 12;        /* the payload length, did't contain the extend hcc hdr area */
};
#pragma pack(pop)

typedef struct hcc_header hcc_header_stru;
#define FRW_EVENT_TYPE_ENUM_UINT8 hi_u8

struct hcc_extend_hdr {
    FRW_EVENT_TYPE_ENUM_UINT8  nest_type;
    hi_u8                  nest_sub_type;
    hi_u8                  chip_id : 2;
    hi_u8                  device_id : 2;
    hi_u8                  vap_id : 4;
    hi_u8                  config_frame;
};

typedef struct hcc_extend_hdr frw_hcc_extend_hdr_stru;

#ifdef _PRE_HICHANNEL_HDR_OPT
#define HCC_HDR_TOTAL_LEN   36
#else
#define HCC_HDR_TOTAL_LEN   64
#endif
#define HCC_HDR_LEN         (sizeof(hcc_header_stru))
#define HCC_HDR_RESERVED_MAX_LEN    (HCC_HDR_TOTAL_LEN - HCC_HDR_LEN)
#define HCC_EXTEND_TOTAL_SIZE  (WLAN_MAX_MAC_HDR_LEN + HI_MAX_DEV_CB_LEN + (hi_u32)sizeof(frw_hcc_extend_hdr_stru))

typedef enum _hcc_test_case_ {
    HCC_TEST_CASE_TX = 0,
    HCC_TEST_CASE_RX = 1,
    HCC_TEST_CASE_LOOP = 2,
    HCC_TEST_CASE_COUNT
} hcc_test_case;

/* less than 16,4bits for sub_type */
typedef enum _hcc_test_subtype_ {
    HCC_TEST_SUBTYPE_CMD = 0,   /* command mode */
    HCC_TEST_SUBTYPE_DATA = 1,
    HCC_TEST_SUBTYPE_BUTT
} hcc_test_subtype;

typedef struct _hcc_test_cmd_stru_ {
    hi_u16 cmd_type;
    hi_u16 cmd_len;
} hcc_test_cmd_stru;
#define hcc_get_test_cmd_data(base) (((hi_u8*)(base)) + sizeof(hcc_test_cmd_stru))

typedef struct _hsdio_trans_test_info_ {
    hi_u32 actual_tx_pkts;
    hi_u8  resv[4];                 /* 4 byte保留字段 */
    hi_u64 total_h2d_trans_bytes;   /* total bytes trans by from host to device */
    hi_u64 total_d2h_trans_bytes;   /* total bytes trans by from device to host */
} hsdio_trans_test_info;

typedef struct _hsdio_trans_test_rx_info_ {
    hi_u32 total_trans_pkts;
    hi_u32 pkt_len;
    hi_u8  pkt_value;
    hi_u8  resv[3]; /* 3 byte保留字段，word对齐 */
} hsdio_trans_test_rx_info;

enum _hcc_descr_type_ {
    HCC_DESCR_ASSEM_RESET = 0,
    HCC_QUEUE_SWITCH = 1,
    HCC_DESCR_TYPE_BUTT
};

typedef hi_u32 hcc_descr_type;
/* This descr buff is reversed in device,
   the callback function can't cost much time,
   just for transfer sdio buff message */
struct hcc_descr_header {
    hcc_descr_type    descr_type;
};

typedef struct exception_bcpu_dump_header {
    hi_u32  align_type;
    hi_u32  start_addr;
    hi_u32  men_len;
} exception_bcpu_dump_header;

enum dump_h2d_cmd {
    READ_MEM          = 0,
    READ_REG          = 1,

    DUMP_H2D_BUTT,
};

enum dump_d2h_cmd {
    DATA              = 0,

    DUMP_D2H_BUTT,
};
typedef hi_u8 hcc_netbuf_queue_type;

typedef enum {
    DATA_HI_QUEUE = 0,
    DATA_LO_QUEUE = 1,
    HCC_QUEUE_COUNT
} hcc_queue_type_enum;

/* 枚举定义 */
typedef enum {
    HCC_TYPE_DATA,
    HCC_TYPE_MSG,
    HCC_TYPE_TEST_XFER,
    HCC_TYPE_BUFF
} hcc_type_enum;

typedef enum {
    HCC_TEST_WRITE_START,
    HCC_TEST_WRITE_TRANFER,
    HCC_TEST_WRITE_OVER,
    HCC_TEST_READ_START,
    HCC_TEST_READ_TRANFER,
    HCC_TEST_READ_OVER,
}hcc_test_xfer_enum;

typedef enum {
    NETBUF_STRU_TYPE,
    NORMAL_STRU_TYPE,
    UNKNOWN_STRU_TYPE,
}hcc_stru_type;

/*****************************************************************************
  3 函数声明
*****************************************************************************/
typedef struct {
    hi_u8                  nest_type;
    hi_u8                  nest_sub_type;
    hi_u16                 buf_len;
    hi_void               *netbuf;
} hcc_event_stru;

typedef hi_void (*hcc_free_uc_buf)(hi_void* data);

typedef struct {
    hi_char *buf;
    hi_s32  length;
    hi_void *priv;
    hi_u32  priv_type;
    hi_u32  msg_num;
    hcc_free_uc_buf free;
    struct osal_list_head list;
}hcc_unc_struc;

typedef struct {
    char* buf;
    hi_s32 length;
}hcc_normal_struc;

typedef hcc_unc_struc* (*hcc_alloc_uc_buf)(hi_s32 len, hcc_stru_type type);
typedef hi_s32 (*hcc_rx)(hi_void *data);
typedef hi_u32 (*wlan_pm_wakeup_dev)(hi_void *data);

struct hcc_bus_ops {
    hcc_rx rx;
    hcc_alloc_uc_buf alloc_unc_buf;
    wlan_pm_wakeup_dev pm_wakeup_dev;
};

typedef struct {
    struct osal_list_head   data_queue;
    hi_u32                  qlen;
    oal_spin_lock_stru      data_queue_lock;
    hi_u8                   flow_flag;          /* 协议栈流程标记位 */
    hi_u8                   resv[3];            /* 3: 四字节对齐 */
}hcc_data_queue;

/*****************************************************************************
  inline
*****************************************************************************/
/* the macro to set hcc hdr */
static inline hi_void hcc_hdr_param_init(hcc_transfer_param *param, hi_u32 main_type, hi_u32 sub_type,
    hi_u8 extend_len, hi_u32 fc_type, hi_u32 queue_id)
{
    param->main_type = main_type;
    param->sub_type = sub_type;
    param->extend_len = extend_len;
    param->fc_flag = fc_type;
    param->queue_id = queue_id;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
