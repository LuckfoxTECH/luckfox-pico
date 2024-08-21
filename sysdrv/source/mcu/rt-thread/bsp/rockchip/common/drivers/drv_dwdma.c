/**
  * Copyright (c) 2020 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_dwdma.c
  * @author  sugar zhang
  * @version V0.1
  * @date    29-Nov-2020
  * @brief   dw driver
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>
#include <rthw.h>

#ifdef RT_USING_DMA_DWDMA

#include "dma.h"
#include "drv_clock.h"

#include "hal_base.h"
#include "hal_bsp.h"

#define WORK_QUEUE_STACK_SIZE       512
#define WORK_QUEUE_PRIORITY         0

static uint8_t s_dwdma_index = 0;

typedef enum
{
    DMA_STATE_RUNNING,
    DMA_STATE_STOP,
} dma_state_t;

struct rt_dma_dwdma
{
    struct rt_dma_device dma;
    struct rt_mutex chan_lock;
    struct rt_workqueue *isr_workqueue;
    struct rt_work *isr_work;

    struct HAL_DWDMA_DEV *dw; /* hal layer */
};

struct rt_dwdma_chan
{
    struct rt_dma_chan chan;
    dma_state_t state;

    struct DWDMA_CHAN *dwc; /* hal layer */
};

static inline struct rt_dma_dwdma *to_dwdma(struct rt_dma_device *dma)
{
    return rt_container_of(dma, struct rt_dma_dwdma, dma);
}

static inline struct rt_dwdma_chan *to_dwc(struct rt_dma_chan *chan)
{
    return rt_container_of(chan, struct rt_dwdma_chan, chan);
}

static int dwdma_start(struct rt_dma_chan *chan)
{
    struct rt_dwdma_chan *rt_dwc = to_dwc(chan);
    int ret;

    if (rt_dwc->state == DMA_STATE_RUNNING)
        return RT_EOK;

    ret = HAL_DWDMA_Start(rt_dwc->dwc);
    if (!ret)
        rt_dwc->state = DMA_STATE_RUNNING;

    return ret;
}

static int dwdma_stop(struct rt_dma_chan *chan)
{
    struct rt_dwdma_chan *rt_dwc = to_dwc(chan);
    int ret;

    if (rt_dwc->state == DMA_STATE_STOP)
        return RT_EOK;

    ret = HAL_DWDMA_Stop(rt_dwc->dwc);

    if (!ret)
        rt_dwc->state = DMA_STATE_STOP;

    return ret;
}

static struct rt_dma_chan *dwdma_request_channel(struct rt_dma_device *dma, uint16_t id)
{
    struct rt_dma_dwdma *rt_dwdma = to_dwdma(dma);
    struct rt_dwdma_chan *rt_dwc;
    struct DWDMA_CHAN *dwc = NULL;

    rt_mutex_take(&rt_dwdma->chan_lock, RT_WAITING_FOREVER);
    dwc = HAL_DWDMA_RequestChannel(rt_dwdma->dw, id);
    rt_mutex_release(&rt_dwdma->chan_lock);
    if (!dwc)
        return RT_NULL;

    dwc->desc = rt_dma_malloc(NR_DESCS_PER_CHANNEL * sizeof(*(dwc->desc)));
    if (!dwc->desc)
        goto err;

    rt_dwc = rt_calloc(1, sizeof(*rt_dwc));
    if (!rt_dwc)
        goto err_desc;

    rt_dwc->dwc = dwc;
    rt_dwc->chan.device = dma;
    rt_dwc->state = DMA_STATE_STOP;

    return &rt_dwc->chan;

err_desc:
    rt_dma_free(dwc->desc);
err:
    HAL_DWDMA_ReleaseChannel(dwc);

    return RT_NULL;
}

static int dwdma_release_channel(struct rt_dma_chan *chan)
{
    struct rt_dwdma_chan *rt_dwc = to_dwc(chan);
    struct rt_dma_dwdma *rt_dwdma = to_dwdma(chan->device);
    HAL_Status hRet;

    rt_mutex_take(&rt_dwdma->chan_lock, RT_WAITING_FOREVER);
    hRet = HAL_DWDMA_ReleaseChannel(rt_dwc->dwc);
    rt_mutex_release(&rt_dwdma->chan_lock);
    HAL_ASSERT(hRet);

    rt_dma_free(rt_dwc->dwc->desc);
    rt_free(rt_dwc);

    return hRet;
}

static int dwdma_config(struct rt_dma_chan *chan, struct dma_slave_config *config)
{
    struct rt_dwdma_chan *rt_dwc = to_dwc(chan);
    struct DMA_SLAVE_CONFIG sConfig;

    sConfig.direction = config->direction;
    sConfig.srcAddr = config->src_addr;
    sConfig.dstAddr = config->dst_addr;
    sConfig.srcAddrWidth = config->src_addr_width;
    sConfig.dstAddrWidth = config->dst_addr_width;
    sConfig.srcMaxBurst = config->src_maxburst;
    sConfig.dstMaxBurst = config->dst_maxburst;
    sConfig.srcInterlaceSize = config->src_interlace_size;
    sConfig.dstInterlaceSize = config->dst_interlace_size;

    return HAL_DWDMA_Config(rt_dwc->dwc, &sConfig);
}

static int dwdma_prep_dma_cyclic(struct rt_dma_chan *chan, uint32_t dmaAddr,
                                 uint32_t len, uint32_t periodLen,
                                 dma_direction_t direction,
                                 dma_callback callback, void *cparam)
{
    struct rt_dwdma_chan *rt_dwc = to_dwc(chan);

    return HAL_DWDMA_PrepDmaCyclic(rt_dwc->dwc, dmaAddr, len, periodLen,
                                   direction, callback, cparam);
}

static int dwdma_prep_dma_single(struct rt_dma_chan *chan, uint32_t dmaAddr,
                                 uint32_t len,
                                 dma_direction_t direction,
                                 dma_callback callback, void *cparam)
{
    struct rt_dwdma_chan *rt_dwc = to_dwc(chan);

    return HAL_DWDMA_PrepDmaSingle(rt_dwc->dwc, dmaAddr, len, direction,
                                   callback, cparam);
}

static int dwdma_prep_dma_memcpy(struct rt_dma_chan *chan, uint32_t dst,
                                 uint32_t src, uint32_t len,
                                 dma_callback callback, void *cparam)
{
    struct rt_dwdma_chan *rt_dwc = to_dwc(chan);

    return HAL_DWDMA_PrepDmaMemcpy(rt_dwc->dwc, dst, src, len,
                                   callback, cparam);
}

static void dwdma_work(struct rt_work *work, void *work_data)
{
    const struct DWDMA_CHAN *dwc = (const struct DWDMA_CHAN *)work_data;

    RT_ASSERT(dwc);

    if (dwc->callback)
        dwc->callback(dwc->cparam);
}

static void dwdma_dwc_isr(struct HAL_DWDMA_DEV *dw, uint32_t id)
{
    struct rt_dma_dwdma *rt_dwdma = dw->priv;
    rt_uint32_t status;

    status = HAL_DWDMA_IrqHandler(dw, id);
    if (status)
        rt_workqueue_dowork(rt_dwdma->isr_workqueue, &rt_dwdma->isr_work[id]);
}

/**
 * IRQ_SHARED depends on implementation of SoC dwdma irq. please
 * enable this macro if SoC using one shared irq for all dma channel.
 * such as rk2106. otherwise, say NO.
 */
#ifdef RT_USING_DMA_DWDMA_IRQ_SHARED
static void dwdma_common_isr(struct HAL_DWDMA_DEV *dw)
{
    rt_uint32_t i;

    rt_interrupt_enter();
    for (i = 0; i < dw->maxChans; i++)
        dwdma_dwc_isr(dw, i);
    rt_interrupt_leave();
}

#define DEFINE_DWDMA_ISR(ID)                         \
static void dwdma##ID##_isr(int vector, void *param) \
{                                                    \
    dwdma_common_isr(&g_dwDma##ID##Dev);             \
}                                                    \
                                                     \
static const rt_isr_handler_t dma##ID##_isr[] =      \
{                                                    \
    dwdma##ID##_isr,                                 \
};

#ifdef DMA
DEFINE_DWDMA_ISR()
#endif
#ifdef DMA0
DEFINE_DWDMA_ISR(0)
#endif
#ifdef DMA1
DEFINE_DWDMA_ISR(1)
#endif
#ifdef DMA2
DEFINE_DWDMA_ISR(2)
#endif

#else

#define DEFINE_DWDMA_ISR(VID, CID)                                    \
static RT_UNUSED void dwdma##VID##_isr_##CID(int vector, void *param) \
{                                                                     \
    rt_interrupt_enter();                                             \
    dwdma_dwc_isr(&g_dwDma##VID##Dev, CID);                           \
    rt_interrupt_leave();                                             \
}

#define DEFINE_DWDMA_ISR_GROUP(ID)                    \
DEFINE_DWDMA_ISR(ID, 0)                               \
DEFINE_DWDMA_ISR(ID, 1)                               \
DEFINE_DWDMA_ISR(ID, 2)                               \
DEFINE_DWDMA_ISR(ID, 3)                               \
DEFINE_DWDMA_ISR(ID, 4)                               \
DEFINE_DWDMA_ISR(ID, 5)                               \
DEFINE_DWDMA_ISR(ID, 6)                               \
DEFINE_DWDMA_ISR(ID, 7)                               \
                                                      \
static const rt_isr_handler_t dma##ID##_isr[8] =      \
{                                                     \
    dwdma##ID##_isr_0,                                \
    dwdma##ID##_isr_1,                                \
    dwdma##ID##_isr_2,                                \
    dwdma##ID##_isr_3,                                \
    dwdma##ID##_isr_4,                                \
    dwdma##ID##_isr_5,                                \
    dwdma##ID##_isr_6,                                \
    dwdma##ID##_isr_7,                                \
};

#if defined(DMA) && defined(DMA_NUM_CHANNELS)
DEFINE_DWDMA_ISR_GROUP()
#endif

#if defined(DMA0) && defined(DMA0_NUM_CHANNELS)
DEFINE_DWDMA_ISR_GROUP(0)
#endif

#if defined(DMA1) && defined(DMA1_NUM_CHANNELS)
DEFINE_DWDMA_ISR_GROUP(1)
#endif

#if defined(DMA2) && defined(DMA2_NUM_CHANNELS)
DEFINE_DWDMA_ISR_GROUP(2)
#endif

#endif /* RT_USING_DMA_DWDMA_IRQ_SHARED */

static rt_err_t dwdma_init(struct HAL_DWDMA_DEV *dw,
                           const rt_isr_handler_t *isr)
{
    struct rt_dma_dwdma *rt_dwdma;
    struct rt_dma_device *dma;
    HAL_Status hal_ret;
    char name[RT_NAME_MAX];
    int i, num_irqs;

    rt_dwdma = rt_calloc(1, sizeof(*rt_dwdma));
    RT_ASSERT(rt_dwdma);

    rt_dwdma->dw = dw;
    dw->priv = rt_dwdma;

    hal_ret = HAL_DWDMA_Init(dw);
    RT_ASSERT(hal_ret == HAL_OK);

    rt_snprintf(name, RT_NAME_MAX, "dwdma-%d", s_dwdma_index++);

    rt_mutex_init(&rt_dwdma->chan_lock, name, RT_IPC_FLAG_FIFO);

    dma = &rt_dwdma->dma;

    dma->base = (uint32_t)dw->pReg;
    dma->start = dwdma_start;
    dma->stop = dwdma_stop;
    dma->request_channel = dwdma_request_channel;
    dma->release_channel = dwdma_release_channel;
    dma->config = dwdma_config;
    dma->prep_dma_memcpy = dwdma_prep_dma_memcpy;
    dma->prep_dma_cyclic = dwdma_prep_dma_cyclic;
    dma->prep_dma_single = dwdma_prep_dma_single;

    rt_dwdma->isr_workqueue = rt_workqueue_create(name, WORK_QUEUE_STACK_SIZE,
                              WORK_QUEUE_PRIORITY);
    RT_ASSERT(rt_dwdma->isr_workqueue);

    rt_dwdma->isr_work = rt_calloc(dw->maxChans, sizeof(*rt_dwdma->isr_work));
    RT_ASSERT(rt_dwdma->isr_work);

    for (i = 0; i < dw->maxChans; i++)
        rt_work_init(&rt_dwdma->isr_work[i], dwdma_work,
                     (void *)&dw->chan[i]);

#ifdef RT_USING_DMA_DWDMA_IRQ_SHARED
    num_irqs = 1;
#else
    num_irqs = dw->maxChans;
#endif

    for (i = 0; i < num_irqs; i++)
    {
        rt_hw_interrupt_install(dw->irq[i], isr[i], RT_NULL, RT_NULL);
        rt_hw_interrupt_umask(dw->irq[i]);
    }

    return rt_hw_dma_register(&rt_dwdma->dma);
}

#define DWDMA_INIT(ID)  dwdma_init(&g_dwDma##ID##Dev, dma##ID##_isr);

int rt_hw_dwdma_init(void)
{
    rt_err_t ret;

#ifdef DMA
    DWDMA_INIT();
#endif
#ifdef DMA0
    DWDMA_INIT(0);
#endif
#ifdef DMA1
    DWDMA_INIT(1);
#endif
#ifdef DMA2
    DWDMA_INIT(2);
#endif

    return 0;
}

INIT_PREV_EXPORT(rt_hw_dwdma_init);
#endif
