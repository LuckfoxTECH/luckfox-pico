/*
 * Copyright (c) 2015 iComm-semi Ltd.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


#ifndef __HWIF_H__
#define __HWIF_H__


#include <linux/skbuff.h>

// general adress for chip id
#define SYS_REG_BASE                           0xc0000000
#define ADR_CHIP_ID_0                          (SYS_REG_BASE+0x00000008)          
#define ADR_CHIP_ID_1                          (SYS_REG_BASE+0x0000000c)          
#define ADR_CHIP_ID_2                          (SYS_REG_BASE+0x00000010)          
#define ADR_CHIP_ID_3                          (SYS_REG_BASE+0x00000014) 

#define MAX_FRAME_SIZE                      2432
/*  802.11ad extends maximum MSDU size for DMG (freq > 40Ghz) networks
 *  to 3839 or 7920 bytes, see 8.2.3 General frame format */
#define MAX_FRAME_SIZE_DMG                  4096
#define HCI_RX_AGGR_SIZE                   0x2710
//#define MAX_HCI_RX_AGGR_SIZE                (HCI_RX_AGGR_SIZE+MAX_FRAME_SIZE)  //AGGR_SIZE+MPDU
#define MAX_HCI_RX_AGGR_SIZE                    (0xFFFF) //HCI RX maximum data length 16bits

#define PLATFORM_DMA_ALIGNED __attribute__ ((aligned(PLATFORM_DEF_DMA_ALIGN_SIZE)))

struct ssv6xxx_hwif_info {
    unsigned long tx_run_data_time;
};

/**
* Hardware Interface APIs on Linux platform.
*/
struct ssv6xxx_hwif_ops {
    int (*read)(struct device *child, void *buf,size_t *size);
    int (*write)(struct device *child, void *buf, size_t len,u8 queue_num);
    int (*readreg)(struct device *child, u32 addr, u32 *buf);
    int (*writereg)(struct device *child, u32 addr, u32 buf);
    void (*hwif_rx_task)(struct device *child, int (*rx_cb)(struct sk_buff *rx_skb, void *args), 
            int (*is_rx_q_full)(void *args), void *args, u32 recv_cnt);
    void (*get_tx_req_cnt)(struct device *child, int *tx_req_cnt);

#if (HWIF_SUPPORT == 2)
    int (*cmd52_read)(struct device *child, u32 addr, u32 *value);
    int (*cmd52_write)(struct device *child, u32 addr, u32 value);
#endif

    /* for debug operation */
    void (*tx_st)(struct device *child, u32 *pkt_cnt);
    void (*rx_st)(struct device *child, u32 *pkt_cnt);

    /* fw reset operation */
    void (*fw_reset)(struct device *child);
#ifdef SSV_PERFORMANCE_WATCH
    void (*get_info)(struct device *child, struct ssv6xxx_hwif_info *info);
    void (*clr_info)(struct device *child);
#endif
};

int ssv_configure_ipc_mem(struct device *child, struct ssv6xxx_hwif_ops *hwif_ops);

#endif /* __HWIF_H__ */
