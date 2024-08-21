/**
 * rwnx_ipc_utils.h
 *
 * IPC utility function declarations
 *
 * Copyright (C) RivieraWaves 2012-2019
 */
#ifndef _RWNX_IPC_UTILS_H_
#define _RWNX_IPC_UTILS_H_

#include <linux/dma-mapping.h>
#include <linux/dmapool.h>
#include <linux/skbuff.h>

#include "lmac_msg.h"

#ifdef CONFIG_RWNX_DBG
/*  #define RWNX_DBG(format, arg...) pr_warn(format, ## arg) */
#define RWNX_DBG printk
#else
#define RWNX_DBG(a...) do {} while (0)
#endif

#define RWNX_FN_ENTRY_STR ">>> %s()\n", __func__

enum rwnx_dev_flag {
    RWNX_DEV_RESTARTING,
    RWNX_DEV_STACK_RESTARTING,
    RWNX_DEV_STARTED,
};

struct rwnx_hw;
struct rwnx_sta;

/**
 * struct rwnx_ipc_elem - Generic IPC buffer of fixed size
 *
 * @addr: Host address of the buffer.
 * @dma_addr: DMA address of the buffer.
 */
struct rwnx_ipc_elem {
    void *addr;
    dma_addr_t dma_addr;
};

/**
 * struct rwnx_ipc_elem_pool - Generic pool of IPC buffers of fixed size
 *
 * @nb: Number of buffers currenlty allocated in the pool
 * @buf: Array of buffers (size of array is @nb)
 * @pool: DMA pool in which buffers have been allocated
 */
struct rwnx_ipc_elem_pool {
    int nb;
    struct rwnx_ipc_elem *buf;
    struct dma_pool *pool;
};

/**
 * struct rwnx_ipc_elem - Generic IPC buffer of variable size
 *
 * @addr: Host address of the buffer.
 * @dma_addr: DMA address of the buffer.
 * @size: Size, in bytes, of the buffer
 */
struct rwnx_ipc_elem_var {
    void *addr;
    dma_addr_t dma_addr;
    size_t size;
};

/**
 * struct rwnx_ipc_dbgdump_elem - IPC buffer for debug dump
 *
 * @mutex: Mutex to protect access to debug dump
 * @buf: IPC buffer
 */
struct rwnx_ipc_dbgdump_elem {
    struct mutex mutex;
    struct rwnx_ipc_elem_var buf;
};

static const u32 rwnx_rxbuff_pattern = 0xCAFEFADE;

/*
 * Maximum Length of Radiotap header vendor specific data(in bytes)
 */
#define RADIOTAP_HDR_VEND_MAX_LEN   16

/*
 * Maximum Radiotap Header Length without vendor specific data (in bytes)
 */
#define RADIOTAP_HDR_MAX_LEN        80

/*
 * Unsupported HT Frame data length (in bytes)
 */
#define UNSUP_RX_VEC_DATA_LEN       2

/**
 * struct rwnx_ipc_skb_elem - IPC buffer for SKB element
 *
 * @skb: Pointer to the skb buffer allocated
 * @dma_addr: DMA address of the data buffer fo skb
 *
 */
struct rwnx_ipc_skb_elem {
    struct sk_buff *skb;
    dma_addr_t dma_addr;
};

#ifdef CONFIG_RWNX_FULLMAC

/* Maximum number of rx buffer the fw may use at the same time */
#define RWNX_RXBUFF_MAX (64 * NX_REMOTE_STA_MAX)

/**
 * struct rwnx_ipc_rxbuf_elems - IPC buffers for RX
 *
 * @skb: Array of buffer push to FW.
 * @idx: Index of the last pushed skb.(Use to find the next free entry quicker)
 *
 * Note: contrary to softmac version, dma_addr are stored inside skb->cb.
 * (cf &struct rwnx_skb_cb)
 */
struct rwnx_ipc_rxbuf_elems {
    struct sk_buff *skb[RWNX_RXBUFF_MAX];
    int idx;
};

/**
 * struct rwnx_skb_cb - Control Buffer structure for RX buffer
 *
 * @dma_addr: DMA address of the data buffer
 * @pattern: Known pattern (used to check pointer on skb)
 * @idx: Index in &struct rwnx_hw.rxbuff_table that contains address of this
 * buffer
 */
struct rwnx_skb_cb {
    dma_addr_t dma_addr;
    uint32_t pattern;
    uint32_t idx;
};

#define RWNX_RXBUFF_DMA_ADDR_SET(skbuff, addr)          \
    ((struct rwnx_skb_cb *)(skbuff->cb))->dma_addr = addr
#define RWNX_RXBUFF_DMA_ADDR_GET(skbuff)                \
    ((struct rwnx_skb_cb *)(skbuff->cb))->dma_addr

#define RWNX_RXBUFF_PATTERN_SET(skbuff, pat)                \
    ((struct rwnx_skb_cb *)(skbuff->cb))->pattern = pat
#define RWNX_RXBUFF_PATTERN_GET(skbuff)         \
    ((struct rwnx_skb_cb *)(skbuff->cb))->pattern

#define RWNX_RXBUFF_IDX_SET(skbuff, val)                \
    ((struct rwnx_skb_cb *)(skbuff->cb))->idx = val
#define RWNX_RXBUFF_IDX_GET(skbuff)             \
    ((struct rwnx_skb_cb *)(skbuff->cb))->idx

#define RWNX_RXBUFF_VALID_IDX(idx) ((idx) < RWNX_RXBUFF_MAX)

/* Used to ensure that hostid set to fw is never 0 */
#define RWNX_RXBUFF_IDX_TO_HOSTID(idx) ((idx) + 1)
#define RWNX_RXBUFF_HOSTID_TO_IDX(hostid) ((hostid) - 1)

#endif /* CONFIG_RWNX_FULLMAC */


#ifdef CONFIG_RWNX_FULLMAC
int rwnx_ipc_rxbuf_elem_allocs(struct rwnx_hw *rwnx_hw);
void rwnx_ipc_rxbuf_elem_pull(struct rwnx_hw *rwnx_hw, struct sk_buff *skb);
void rwnx_ipc_rxbuf_elem_sync(struct rwnx_hw *rwnx_hw, struct sk_buff *skb,
                              int len);
void rwnx_ipc_rxdesc_elem_repush(struct rwnx_hw *rwnx_hw,
                                 struct rwnx_ipc_elem *elem);
void rwnx_ipc_rxbuf_elem_repush(struct rwnx_hw *rwnx_hw,
                                struct sk_buff *skb);
#endif /* CONFIG_RWNX_FULLMAC */

void rwnx_ipc_msg_push(struct rwnx_hw *rwnx_hw, void *msg_buf, uint16_t len);
void rwnx_ipc_txdesc_push(struct rwnx_hw *rwnx_hw, void *tx_desc,
                          void *hostid, int hw_queue, int user);

void *rwnx_ipc_fw_trace_desc_get(struct rwnx_hw *rwnx_hw);

int rwnx_ipc_rxbuf_init(struct rwnx_hw *rwnx_hw, uint32_t rx_bufsz);
int rwnx_ipc_init(struct rwnx_hw *rwnx_hw, u8 *shared_ram);
void rwnx_ipc_deinit(struct rwnx_hw *rwnx_hw);
void rwnx_ipc_start(struct rwnx_hw *rwnx_hw);
void rwnx_ipc_stop(struct rwnx_hw *rwnx_hw);
void rwnx_ipc_tx_drain(struct rwnx_hw *rwnx_hw);
bool rwnx_ipc_tx_pending(struct rwnx_hw *rwnx_hw);

struct ipc_host_env_tag;
int rwnx_ipc_elem_var_allocs(struct rwnx_hw *rwnx_hw,
                             struct rwnx_ipc_elem_var *elem, size_t elem_size,
                             enum dma_data_direction dir,
                             void *buf, const void *init,
                             void (*push)(struct ipc_host_env_tag *, uint32_t));
void rwnx_ipc_elem_var_deallocs(struct rwnx_hw *rwnx_hw,
                                struct rwnx_ipc_elem_var *elem);
int rwnx_ipc_unsup_rx_vec_elem_allocs(struct rwnx_hw *rwnx_hw,
                                      struct rwnx_ipc_skb_elem *elem);

void rwnx_error_ind(struct rwnx_hw *rwnx_hw);
void rwnx_umh_done(struct rwnx_hw *rwnx_hw);

void rwnx_ipc_sta_buffer_init(struct rwnx_hw *rwnx_hw, int sta_idx);
void rwnx_ipc_sta_buffer(struct rwnx_hw *rwnx_hw, struct rwnx_sta *sta, int tid, int size);
#endif /* _RWNX_IPC_UTILS_H_ */
