/**
  * Copyright (c) 2018 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    dma.h
  * @author  Sugar Zhang
  * @version V0.1
  * @date    8-Feb-2019
  * @brief   general dma framework driver for pisces
  *
  ******************************************************************************
  */

#ifndef __DMA_H__
#define __DMA_H__

/* Includes ------------------------------------------------------------------*/

#include <rtthread.h>
#include <hal_base.h>

/* Exported macro ------------------------------------------------------------*/

#define RT_DEVICE_CTRL_DMA_OPEN                     (1)
#define RT_DEVICE_CTRL_DMA_CLOSE                    (2)
#define RT_DEVICE_CTRL_DMA_REQUEST_CHANNEL          (3)
#define RT_DEVICE_CTRL_DMA_RELEASE_CHANNEL          (4)
#define RT_DEVICE_CTRL_DMA_SINGLE_PREPARE           (5)
#define RT_DEVICE_CTRL_DMA_CYCLIC_PREPARE           (6)
#define RT_DEVICE_CTRL_DMA_START                    (7)
#define RT_DEVICE_CTRL_DMA_STOP                     (8)
#define RT_DEVICE_CTRL_DMA_GET_POSITION             (9)

/* Exported types ------------------------------------------------------------*/

typedef enum
{
    RT_DMA_MEM_TO_MEM = 0,
    RT_DMA_MEM_TO_DEV = 1,
    RT_DMA_DEV_TO_MEM = 2,
    RT_DMA_DEV_TO_DEV = 3,
    RT_DMA_TRANS_NONE = 4,
} dma_direction_t;

typedef enum
{
    RT_DMA_SLAVE_BUSWIDTH_UNDEFINED = 0,
    RT_DMA_SLAVE_BUSWIDTH_1_BYTE = 1,
    RT_DMA_SLAVE_BUSWIDTH_2_BYTES = 2,
    RT_DMA_SLAVE_BUSWIDTH_3_BYTES = 3,
    RT_DMA_SLAVE_BUSWIDTH_4_BYTES = 4,
    RT_DMA_SLAVE_BUSWIDTH_8_BYTES = 8,
    RT_DMA_SLAVE_BUSWIDTH_16_BYTES = 16,
    RT_DMA_SLAVE_BUSWIDTH_32_BYTES = 32,
    RT_DMA_SLAVE_BUSWIDTH_64_BYTES = 64,
} dma_slave_buswidth_t;

struct dma_slave_config
{
    dma_direction_t direction;
    dma_slave_buswidth_t src_addr_width;
    dma_slave_buswidth_t dst_addr_width;
    uint32_t src_addr;
    uint32_t dst_addr;
    uint16_t src_maxburst;
    uint16_t dst_maxburst;
    uint16_t src_interlace_size;
    uint16_t dst_interlace_size;
};

typedef void (*dma_callback)(void *cparam);

struct rt_dma_chan;

struct rt_dma_device
{
    struct rt_device dev;
    uint32_t base;
    int (*start)(struct rt_dma_chan *chan);
    int (*stop)(struct rt_dma_chan *chan);

    /* id: DMA_REQ_Type */
    struct rt_dma_chan *(*request_channel)(struct rt_dma_device *dma, uint16_t id);
    int (*release_channel)(struct rt_dma_chan *chan);

    int (*config)(struct rt_dma_chan *chan, struct dma_slave_config *config);
    int (*prep_dma_memcpy)(struct rt_dma_chan *chan, uint32_t dst,
                           uint32_t src, uint32_t len,
                           dma_callback callback, void *cparam);
    int (*prep_dma_cyclic)(struct rt_dma_chan *chan, uint32_t dmaAddr,
                           uint32_t len, uint32_t periodLen,
                           dma_direction_t direction,
                           dma_callback callback, void *cparam);
    int (*prep_dma_single)(struct rt_dma_chan *chan, uint32_t dmaAddr,
                           uint32_t len,
                           dma_direction_t direction,
                           dma_callback callback, void *cparam);
    int (*get_position)(struct rt_dma_chan *chan);
};

struct rt_dma_chan
{
    struct rt_dma_device *device;
    int chanId;
};

struct rt_dma_transfer
{
    rt_uint32_t position;
    /* user's params */
    rt_uint32_t dma_req_num; /* peri dma request num */
    rt_uint32_t direction; /* direction */
    rt_uint32_t src_addr; /* src addr */
    rt_uint32_t dst_addr; /* dst addr */
    rt_uint32_t src_addr_width; /* addr bus width */
    rt_uint32_t dst_addr_width; /* addr bus width */
    rt_uint32_t src_maxburst; /* burst len */
    rt_uint32_t dst_maxburst; /* burst len */
    rt_uint32_t src_interlace_size; /* interlace size */
    rt_uint32_t dst_interlace_size; /* interlace size */

    rt_uint32_t len; /* in bytes */
    rt_uint32_t period_len; /* in bytes */
    rt_bool_t cyclic; /* indicate whether the xfer is cyclic or single */
    dma_callback callback; /* dma xfer complete callback func */
    void *cparam; /* callback param */

    struct rt_dma_chan *chan; /* don't touch this. */
};

/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

rt_device_t rt_dma_get(uint32_t base);
rt_err_t rt_hw_dma_register(struct rt_dma_device *dma);
void *rt_dma_malloc(uint32_t size);
__STATIC_FORCEINLINE void rt_dma_free(void *ptr)
{
    rt_free_align(ptr);
}
#endif
