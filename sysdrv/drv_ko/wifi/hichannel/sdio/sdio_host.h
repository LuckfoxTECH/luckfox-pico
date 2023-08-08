/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Header file for wal_net.c.
 * Author: Hisilicon
 * Create: 2020-09-02
 */

#ifndef __SDIO_HOST_H__
#define __SDIO_HOST_H__

#include "hi_types_base.h"
#include "oal_thread.h"
#include "hcc_comm.h"
#include <linux/semaphore.h>
#include <linux/mmc/sdio_func.h>
#include <linux/mmc/card.h>
#include <linux/mmc/host.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 宏定义 */
#define SDIO_CARD_NUM                               1
#define HISDIO_BLOCK_SIZE                           512
#define OAL_SDIO_TX                                 (1<<0)
#define OAL_SDIO_RX                                 (1<<1)
#define OAL_SDIO_ALL                                (OAL_SDIO_TX | OAL_SDIO_RX)
#define HISDIO_VENDOR_ID_HISI                       0x0296      /* VENDOR ID */
#define HISDIO_PRODUCT_ID_HISI                      0x5347      /* Product 1102 */

#define sdio_get_max_block_count(func)              ((func)->card->host->max_blk_count)
#define sdio_get_max_req_size(func)                 ((func)->card->host->max_req_size)
#define sdio_get_max_blk_size(func)                 ((func)->card->host->max_blk_size)
#define sdio_get_max_seg_size(func)                 ((func)->card->host->max_seg_size)
#define sdio_get_max_segs(func)                     ((func)->card->host->max_segs)
#define sdio_en_timeout(func)                       ((func)->enable_timeout)
#define sdio_func_num(func)                         ((func)->num)

#define HISDIO_WAKEUP_DEV_REG                       0xf0
#define ALLOW_TO_SLEEP_VALUE                        1
#define DISALLOW_TO_SLEEP_VALUE                     0

#define HISDIO_REG_FUNC1_FIFO                       0x00        /* Read Write FIFO */
#define HISDIO_REG_FUNC1_INT_STATUS                 0x08        /* interrupt mask and clear reg */
#define HISDIO_REG_FUNC1_INT_ENABLE                 0x09        /* interrupt */
#define HISDIO_REG_FUNC1_XFER_COUNT                 0x0c        /* notify number of bytes to be read */
#define HISDIO_REG_FUNC1_WRITE_MSG                  0x24        /* write msg to device */
#define HISDIO_REG_FUNC1_MSG_FROM_DEV               0x28        /* notify Host that device has got the msg */
#define HISDIO_REG_FUNC1_MSG_HIGH_FROM_DEV          0x2b        /* Host receive the msg ack */

#define HISDIO_FUNC1_INT_DREADY         (1 << 0)    /* data ready interrupt */
#define HISDIO_FUNC1_INT_RERROR         (1 << 1)    /* data read error interrupt */
#define HISDIO_FUNC1_INT_MFARM          (1 << 2)    /* ARM Msg interrupt */
#define HISDIO_FUNC1_INT_ACK            (1 << 3)    /* ACK interrupt */
#define HISDIO_FUNC1_INT_MASK           (HISDIO_FUNC1_INT_DREADY | HISDIO_FUNC1_INT_RERROR | HISDIO_FUNC1_INT_MFARM)

#define MCI_SLOT_NUM                                4
#define HISDIO_H2D_SCATT_BUFFLEN_ALIGN              32
#define SDIO_MAX_XFER_LEN                           (1024 * 128)
#define HISDIO_EXTEND_REG_COUNT                     64
#define HISDIO_DEV2HOST_SCATT_MAX                   64
#define HISDIO_HOST2DEV_SCATT_MAX                   64

/* WLAN_GPIO_INT */
extern int hi_rk_irq_gpio;

#if 0
#if(_PRE_OS_PLATFORM == _PRE_PLATFORM_JZ)
#define WLAN_GPIO_INT                               9
#elif (_PRE_OS_PLATFORM == _PRE_PLATFORM_HISILICON)
#define WLAN_GPIO_INT                               68
#endif
#else
//#define WLAN_GPIO_INT                               53 //将 GPIO1_C5 换算成 GPIO 编号为 32 * 1 + 8 * 2 + 5 = 53
#define WLAN_GPIO_INT                               40 //将 GPIO1_B0 换算成 GPIO 编号为 32 * 1 + 8 * 1 + 0 = 40
#endif
#define PROC_NAME_GPIO_WLAN_WAKEUP_HOST             "WLAN_INT"
#define HISDIO_D2H_SCATT_BUFFLEN_ALIGN_BITS         5

#define hisdio_align_4_or_blk(len)  ((len) < HISDIO_BLOCK_SIZE ? ALIGN((len), 4) : ALIGN((len), HISDIO_BLOCK_SIZE))
#define IRQF_DISABLED                               0x00000020
/* 0x30~0x38, 0x3c~7B */
#define HISDIO_EXTEND_BASE_ADDR     0x30
#define HISDIO_EXTEND_CREDIT_ADDR   0x3c
#define HISDIO_EXTEND_REG_COUNT     64

/* sdio flow control info, free cnt */
#define hisdio_short_pkt_set(reg, num)    do {(reg) = (((reg) & 0xFFFFFF00) | (((num) & 0xFF)));} while (0)
#define hisdio_large_pkt_set(reg, num)    do {(reg) = (((reg) & 0xFFFF00FF) | (((num) & 0xFF) << 8));} while (0)
#define hisdio_reserve_pkt_set(reg, num)  do {(reg) = (((reg) & 0xFF00FFFF) | (((num) & 0xFF) << 16));} while (0)
#define hisdio_comm_reg_seq_set(reg, num) do {(reg) = (((reg) & 0x00FFFFFF) | (((num) & 0xFF) << 24));} while (0)

typedef hi_s32(*sdio_msg_rx)(hi_void *data);

typedef enum _sdio_d2h_msg_type_ {
    D2H_MSG_WLAN_READY          = 0,
    D2H_MSG_FLOWCTRL_UPDATE     = 1,    /* For the credit flow ctrl */
    D2H_MSG_FLOWCTRL_OFF        = 2,    /* can't send data */
    D2H_MSG_FLOWCTRL_ON         = 3,    /* can send data */
    D2H_MSG_WAKEUP_SUCC         = 4,    /* Wakeup done */
    D2H_MSG_ALLOW_SLEEP         = 5,    /* ALLOW Sleep */
    D2H_MSG_DISALLOW_SLEEP      = 6,    /* DISALLOW Sleep */
    D2H_MSG_DEVICE_PANIC        = 7,    /* arm abort */
    D2H_MSG_POWEROFF_ACK        = 8,    /* Poweroff cmd ack */
    D2H_MSG_OPEN_BCPU_ACK       = 9,    /* OPEN BCPU cmd ack */
    D2H_MSG_CLOSE_BCPU_ACK      = 10,   /* CLOSE BCPU cmd ack */
    D2H_MSG_CREDIT_UPDATE       = 11,   /* update high priority buffer credit value */
    D2H_MSG_HIGH_PKT_LOSS       = 12,   /* high pri pkts loss count */
    D2H_MSG_HALT_BCPU           = 13,   /* halt bcpu ack */
    D2H_MSG_HEARTBEAT           = 14,   /* send heartbeat */
    D2H_MSG_HOST_SLEEP_ACK      = 16,   /* host sleep ack */
    D2H_MSG_BEFORE_DEV_SLEEP    = 17,   /* befor device sleep msg */
    D2H_MSG_DEV_WKUP            = 18,   /* device wkup msg */
    D2H_MSG_HEART_BEAT_OPEN_ACK = 19,   /* open heart beat ack */
    D2H_MSG_HEART_BEAT_CLOSE_ACK = 20,  /* close heart beat ack */
    D2H_MSG_TEST_WRITE_OVER     = 21,
    D2H_MSG_COUNT               = 32    /* max support msg count */
} hi_sdio_d2h_msg_type_e;

/* Host to device sdio message type */
typedef enum _sdio_h2d_msg_type_ {
    H2D_MSG_FLOWCTRL_ON         = 0, /* can send data, force to open */
    H2D_MSG_DEVICE_INFO_DUMP    = 1,
    H2D_MSG_DEVICE_MEM_DUMP     = 2,
    H2D_MSG_HEARTBEAT_ACK       = 3,
    H2D_MSG_PM_WLAN_OFF         = 4,
    H2D_MSG_SLEEP_REQ           = 5,
    H2D_MSG_PM_DEBUG            = 6,
    H2D_MSG_RESET_BCPU          = 7,
    H2D_MSG_QUERY_RF_TEMP       = 8,
    H2D_MSG_HCC_SLAVE_THRUPUT_BYPASS = 9,
    H2D_MSG_DEVICE_MEM_INFO          = 10,
    H2D_MSG_STOP_SDIO_TEST      = 11,
    H2D_MSG_PM_BCPU_OFF         = 12,
    H2D_MSG_HOST_SLEEP          = 16,
    H2D_MSG_HOST_DISSLEEP       = 17,
    H2D_MSG_HEART_BEAT_OPEN     = 18,
    H2D_MSG_HEART_BEAT_CLOSE    = 19,
    H2D_MSG_HICHANNEL_HDR_OPT   = 20,
    H2D_MSG_SDIO_READY          = 21,
    H2D_MSG_COUNT               = 32 /* max support msg count */
} hi_sdio_h2d_msg_type_e;

typedef enum sdio_exception_type {
    EXCEPTION_CMD_SEND_FAILED = 0,
    EXCEPTION_BUFF  = 32
} hi_sdio_exception_type;

enum {
    SDIO_READ = 0,
    SDIO_WRITE,
    SDIO_OPT_BUTT
};

typedef struct {
    hi_u32 int_stat;
    hi_u32 msg_stat;
    hi_u32 xfer_count;
    hi_u32 credit_info;
    hi_u8  comm_reg[HISDIO_EXTEND_REG_COUNT];
} hisdio_extend_func;

typedef struct {
    sdio_msg_rx msg_rx;
    void *data;
    hi_u32 count;
    hi_u64 cpu_time; /* the time of the last come! */
} sdio_msg_stru;

/* 结构体定义 */
typedef struct {
    hi_u8   short_free_cnt;
    hi_u8   large_free_cnt;
    oal_spin_lock_stru credit_lock;
} hsdio_credit_info;

typedef struct {
    hi_u32 max_scatt_num;
    struct scatterlist  *sglist;
} sdio_scatt_stru;

typedef struct {
    hi_u32                  state;
    struct sdio_func       *func;
    hi_void                *bus_data;
    struct hcc_bus_ops     *bus_ops;
    hi_u32                  func1_int_mask;
    unsigned long           ul_wlan_irq;
    hi_u8*                  rx_buf;
    hi_u8*                  tx_buf;
    oal_kthread_stru*       rx_handle;
    struct semaphore        rx_sema;
    sdio_scatt_stru         scatt_info[SDIO_OPT_BUTT];
    hi_u8                   *sdio_align_buff;
    sdio_msg_stru           msg[D2H_MSG_COUNT];
    hisdio_extend_func      *sdio_extend;
    hsdio_credit_info       sdio_credit_info;
} oal_channel_stru;

/* inline 函数定义 */
static inline hi_void oal_sdio_claim_host(struct sdio_func* func)
{
    sdio_claim_host(func);
}

static inline hi_void oal_sdio_release_host(struct sdio_func* func)
{
    sdio_release_host(func);
}

static inline void oal_sdio_writeb(struct sdio_func *func, hi_u8 b,
    hi_u32 addr, hi_s32 *err_ret)
{
    sdio_writeb(func, b, addr, err_ret);
}

static inline hi_void oal_sdio_writew(struct sdio_func *func, u16 b,
    unsigned int addr, hi_s32 *err_ret)
{
    sdio_writew(func, b, addr, err_ret);
}

static inline hi_void oal_sdio_writel(struct sdio_func *func, hi_u32 b,
    hi_u32 addr, hi_s32 *err_ret)
{
    sdio_writel(func, b, addr, err_ret);
}

static inline hi_s32 oal_sdio_writesb(struct sdio_func *func, hi_u32 addr,
    void *src, hi_s32 count)
{
    return sdio_writesb(func, addr, src, count);
}

static inline hi_u8 oal_sdio_readb(struct sdio_func *func, hi_u32 addr,
    hi_s32 *err_ret)
{
    return sdio_readb(func, addr, err_ret);
}

static inline hi_u16 oal_sdio_readw(struct sdio_func *func, hi_u32 addr,
    hi_s32 *err_ret)
{
    return sdio_readw(func, addr, err_ret);
}

static inline hi_u32 oal_sdio_readl(struct sdio_func *func, hi_u32 addr,
    hi_s32 *err_ret)
{
    return sdio_readl(func, addr, err_ret);
}

static inline hi_s32 oal_sdio_readsb(struct sdio_func *func, hi_u32 addr,
    void *src, hi_s32 count)
{
    return sdio_readsb(func, src, addr, count);
}

static inline hi_u8 oal_sdio_f0_readb(struct sdio_func *func, hi_u32 addr,
    hi_s32 *err_ret)
{
    return sdio_f0_readb(func, addr, err_ret);
}

static inline hi_void oal_sdio_f0_writeb(struct sdio_func *func, hi_u8 b,
    hi_u32 addr,  hi_s32 *err_ret)
{
    sdio_f0_writeb(func, b, addr, err_ret);
}

static inline hi_s32 oal_sdio_memcpy_fromio(struct sdio_func *func, hi_void *dst,
    hi_u32 addr, hi_s32 count)
{
    return sdio_memcpy_fromio(func, dst, addr, count);
}

/* 函数声明 */
hi_void hisi_sdio_rescan(hi_s32 slot);
hi_s32  oal_sdio_init(void *hcc_handler, hi_s32 sdio_dev_num, struct hcc_bus_ops *bus_ops);
hi_void oal_sdio_exit(oal_channel_stru *hi_sdio);
hi_s32  oal_sdio_transfer_list(const oal_channel_stru *hi_sdio, const hcc_data_queue *head, hi_s32 rw);
hi_s32  oal_sdio_build_rx_list(oal_channel_stru *hi_sdio, hcc_data_queue *head);
hi_s32  oal_sdio_transfer_register(oal_channel_stru *hi_sdio, struct hcc_bus_ops* bus_ops);
hi_void oal_sdio_transfer_unregister(oal_channel_stru *hi_sdio);
hi_s32  oal_sdio_get_credit(const oal_channel_stru *hi_sdio, hi_u32 *uc_hipriority_cnt);
hi_s32  oal_sdio_message_register(oal_channel_stru *hi_sdio, hi_u8 msg, sdio_msg_rx cb, hi_void *data);
hi_void oal_sdio_message_unregister(oal_channel_stru *hi_sdio, hi_u8 msg);
hi_s32  oal_sdio_send_msg(oal_channel_stru *hi_sdio, unsigned long val);
hi_void oal_disable_sdio_state(oal_channel_stru *hi_sdio, hi_u32 mask);
#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif

#endif
