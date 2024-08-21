/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_spi.c
  * @author  David Wu
  * @version V0.1
  * @date    26-Feb-2019
  * @brief   SPI Driver
  *
  ******************************************************************************
  */

/** @addtogroup RKBSP_Driver_Reference
 *  @{
 */

/** @addtogroup SPI
 *  @{
 */

/** @defgroup SPI_How_To_Use How To Use
 *  @{

 See more information, click [here](https://www.rt-thread.org/document/site/programming-manual/device/spi/spi/)

 @} */

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#if defined(RT_USING_SPI) && (defined(RT_USING_SPI0) || defined(RT_USING_SPI1) || defined(RT_USING_SPI2))

#include "hal_base.h"
#include "drv_clock.h"
#include "drv_pm.h"
#include "dma.h"
#include "hal_bsp.h"

#define SPI_DEBUG 0

#if SPI_DEBUG
#define spi_dbg(dev, fmt, ...) \
do { \
    rt_kprintf("%s: " fmt, ((struct rt_device *)dev)->parent.name, ##__VA_ARGS__); \
} while(0)
#else
#define spi_dbg(dev, fmt, ...) \
do { \
} while(0)
#endif

#define ROCKCHIP_SPI_CS_0 (0)
#define ROCKCHIP_SPI_CS_1 (1)

#define RXBUSY (1 << 0)
#define TXBUSY (1 << 1)

#define ROCKCHIP_SPI_TX_IDLE_TIMEOUT (20)

struct rockchip_spi_cs
{
    rt_int8_t pin;
};

/* SPI Controller driver's private data. */
struct rockchip_spi
{
    /* SPI bus */
    struct rt_spi_bus bus;
    struct rt_spi_device rt_spi_dev[2]; /* each cs one dev */
    struct rockchip_spi_cs rt_spi_cs[2];

    /* hardware related */
    const struct HAL_SPI_DEV *hal_dev;
    void (*isr)(void);
    rt_uint16_t dma_burst_size;
    struct rt_mutex spi_lock;
    struct clk_gate *pclk_gate;
    struct clk_gate *sclk_gate;

    /* status */
    rt_int32_t error;
    struct rt_completion done;
    struct rt_semaphore sem_lock;

    /* Hal */
    struct SPI_HANDLE instance;

    /* DMA */
    rt_uint32_t state;
    struct rt_device *dma;
    struct rt_dma_transfer tx_dma_xfer;
    struct rt_dma_transfer rx_dma_xfer;
};

/**
 * rockchip_spi_irq - spi irq handler
 * @spi: SPI private structure which contains SPI specific data
 */
static void rockchip_spi_irq(struct rockchip_spi *spi)
{
    struct SPI_HANDLE *pSPI = &spi->instance;
    int status;

    /* enter interrupt */
    rt_interrupt_enter();

    status = HAL_SPI_IrqHandler(pSPI);
    if (status != HAL_BUSY)
    {
        spi->error = status;
        if (spi->error)
            spi_dbg(&spi->bus.parent, "irq handle error: %d.\n", spi->error);
        rt_completion_done(&spi->done);
    }

    /* leave interrupt */
    rt_interrupt_leave();
}

/**
 * rockchip_spi_configure - probe function for SPI Controller
 * @device: SPI device structure
 * @configuration: SPI configuration structure
 * @return: return error code
 */
static rt_err_t rockchip_spi_configure(struct rt_spi_device *device,
                                       struct rt_spi_configuration *configuration)
{
    struct rockchip_spi *spi = device->bus->parent.user_data;
    struct SPI_HANDLE *pSPI = &spi->instance;
    struct SPI_CONFIG *pSPIConfig = &pSPI->config;

    if (!configuration->max_hz)
        return RT_EINVAL;

    rt_mutex_take(&spi->spi_lock, RT_WAITING_FOREVER);

    /* Data width */
    if (configuration->data_width <= 8)
        pSPIConfig->nBytes = CR0_DATA_FRAME_SIZE_8BIT;
    else if (configuration->data_width <= 16)
        pSPIConfig->nBytes = CR0_DATA_FRAME_SIZE_16BIT;
    else
    {
        rt_mutex_release(&spi->spi_lock);
        return RT_EINVAL;
    }
    /* CPOL */
    if (configuration->mode & RT_SPI_CPOL)
        pSPIConfig->clkPolarity = CR0_POLARITY_HIGH;
    else
        pSPIConfig->clkPolarity = CR0_POLARITY_LOW;

    /* CPHA */
    if (configuration->mode & RT_SPI_CPHA)
        pSPIConfig->clkPhase = CR0_PHASE_2EDGE;
    else
        pSPIConfig->clkPhase = CR0_PHASE_1EDGE;

    /* MSB or LSB */
    if (configuration->mode & RT_SPI_MSB)
        pSPIConfig->firstBit = CR0_FIRSTBIT_MSB;
    else
        pSPIConfig->firstBit = CR0_FIRSTBIT_LSB;

    if (configuration->mode & RT_SPI_SLAVE)
        pSPIConfig->opMode = CR0_OPM_SLAVE;
    else
        pSPIConfig->opMode = CR0_OPM_MASTER;

    if (pSPIConfig->speed == configuration->max_hz)
    {
        rt_mutex_release(&spi->spi_lock);
        return RT_EOK;
    }

    pSPIConfig->speed = configuration->max_hz;
    if (pSPIConfig->opMode == CR0_OPM_MASTER)
    {
        if (pSPIConfig->speed > HAL_SPI_MASTER_MAX_SCLK_OUT)
            pSPIConfig->speed = HAL_SPI_MASTER_MAX_SCLK_OUT;

        if (pSPIConfig->speed > (HAL_SPI_MASTER_MAX_SCLK_OUT >> 1))
            pSPI->maxFreq = 2 * pSPIConfig->speed;
        else
            pSPI->maxFreq = 3 * HAL_SPI_MASTER_MAX_SCLK_OUT;

        clk_set_rate(spi->hal_dev->clkId,  pSPI->maxFreq);
        pSPI->maxFreq = clk_get_rate(spi->hal_dev->clkId);
    }
    else
    {
        if (pSPIConfig->speed > HAL_SPI_SLAVE_MAX_SCLK_OUT)
            pSPIConfig->speed = HAL_SPI_SLAVE_MAX_SCLK_OUT;

        if (pSPIConfig->speed > (HAL_SPI_SLAVE_MAX_SCLK_OUT >> 1))
            pSPI->maxFreq = 6 * pSPIConfig->speed;
        else
            pSPI->maxFreq = 6 * HAL_SPI_SLAVE_MAX_SCLK_OUT;

        clk_set_rate(spi->hal_dev->clkId, pSPI->maxFreq);
        pSPI->maxFreq = clk_get_rate(spi->hal_dev->clkId);
    }

    spi_dbg(pSPI, "SPI SCLK %dHz, speed %dHz\n", pSPI->maxFreq, pSPIConfig->speed);
    rt_mutex_release(&spi->spi_lock);

    return RT_EOK;
}

static void rockchip_spi_dma_handle_err(struct rockchip_spi *spi)
{
    struct SPI_HANDLE *pSPI = &spi->instance;

    rt_sem_take(&spi->sem_lock, RT_WAITING_FOREVER);
    /*
     * For DMA mode, we need terminate DMA channel and flush
     * fifo for the next transfer if DMA thansfer timeout.
     * handle_err() was called by core if transfer failed.
     * Maybe it is reasonable for error handling here.
     */
    if (HAL_SPI_IsDmaXfer(pSPI))
    {
        if (spi->state & RXBUSY)
        {
            rt_device_control(spi->dma, RT_DEVICE_CTRL_DMA_STOP,
                              &spi->rx_dma_xfer);
            rt_device_control(spi->dma, RT_DEVICE_CTRL_DMA_RELEASE_CHANNEL,
                              &spi->rx_dma_xfer);
            HAL_SPI_FlushFifo(pSPI);
        }

        if (spi->state & TXBUSY)
        {
            rt_device_control(spi->dma, RT_DEVICE_CTRL_DMA_STOP,
                              &spi->tx_dma_xfer);
            rt_device_control(spi->dma, RT_DEVICE_CTRL_DMA_RELEASE_CHANNEL,
                              &spi->tx_dma_xfer);
        }
    }
    rt_sem_release(&spi->sem_lock);
}

static void rockchip_spi_dma_rxcb(void *data)
{
    struct rockchip_spi *spi = data;

    rt_sem_take(&spi->sem_lock, RT_WAITING_FOREVER);

    spi->state &= ~RXBUSY;
    if (!(spi->state & TXBUSY))
        rt_completion_done(&spi->done);
    rt_sem_release(&spi->sem_lock);
}

static void rockchip_spi_dma_txcb(void *data)
{
    struct rockchip_spi *spi = data;

    rt_sem_take(&spi->sem_lock, RT_WAITING_FOREVER);

    spi->state &= ~TXBUSY;
    if (!(spi->state & RXBUSY))
        rt_completion_done(&spi->done);
    rt_sem_release(&spi->sem_lock);
}

static rt_err_t rockchip_spi_wait_idle(struct rockchip_spi *spi)
{
    struct SPI_HANDLE *pSPI = &spi->instance;
    rt_err_t ret = RT_EBUSY;
    uint32_t timeout;

    timeout = rt_tick_get() + ROCKCHIP_SPI_TX_IDLE_TIMEOUT; /* some tolerance */
    do
    {
        ret = HAL_SPI_QueryBusState(pSPI);
        if (ret == HAL_OK)
            break;
    }
    while (timeout > rt_tick_get());

    return ret;
}

static rt_uint32_t rockchip_spi_calc_burst_size(rt_uint32_t data_len)
{
    rt_uint32_t i;

    /* burst size: 1, 2, 4, 8 */
    for (i = 1; i < 8; i <<= 1)
    {
        if (data_len & i)
            break;
    }

    /* DW_DMA is not support burst 2 */
    if (i == 2)
        i = 1;

    return i;
}

/**
 * rockchip_spi_dma_prepare - spi prepare dma tranfer config
 * @spi: SPI private structure which contains SPI specific data
 * @message: SPI message structure
 * @return: return error code
 */
static rt_err_t rockchip_spi_dma_prepare(struct rockchip_spi *spi, struct rt_spi_message *message)
{
    struct SPI_HANDLE *pSPI = &spi->instance;
    rt_err_t ret;

    rt_memset(&spi->rx_dma_xfer, 0, sizeof(struct rt_dma_transfer));
    rt_memset(&spi->tx_dma_xfer, 0, sizeof(struct rt_dma_transfer));

    spi->state &= ~RXBUSY;
    spi->state &= ~TXBUSY;

    /* Configure rx firstly. */
    if (message->recv_buf)
    {
        spi->rx_dma_xfer.direction = spi->hal_dev->rxDma.direction;
        spi->rx_dma_xfer.dma_req_num = spi->hal_dev->rxDma.channel;
        spi->rx_dma_xfer.src_addr = spi->hal_dev->rxDma.addr;
        spi->rx_dma_xfer.src_addr_width = pSPI->config.nBytes;
        spi->rx_dma_xfer.src_maxburst = spi->dma_burst_size;
        spi->rx_dma_xfer.dst_addr = (rt_uint32_t)message->recv_buf;
        spi->rx_dma_xfer.dst_addr_width = pSPI->config.nBytes;
        spi->rx_dma_xfer.dst_maxburst = spi->dma_burst_size;

        spi->rx_dma_xfer.len = message->length;
        spi->rx_dma_xfer.callback = rockchip_spi_dma_rxcb;
        spi->rx_dma_xfer.cparam = spi;

        ret = rt_device_control(spi->dma, RT_DEVICE_CTRL_DMA_REQUEST_CHANNEL,
                                &spi->rx_dma_xfer);
        if (ret)
            return ret;

        ret = rt_device_control(spi->dma, RT_DEVICE_CTRL_DMA_SINGLE_PREPARE,
                                &spi->rx_dma_xfer);
        if (ret)
            goto rx_release;
    }

    if (message->send_buf)
    {
        spi->tx_dma_xfer.direction = spi->hal_dev->txDma.direction;
        spi->tx_dma_xfer.dma_req_num = spi->hal_dev->txDma.channel;
        spi->tx_dma_xfer.dst_addr = spi->hal_dev->txDma.addr;
        spi->tx_dma_xfer.dst_addr_width = pSPI->config.nBytes;
        spi->tx_dma_xfer.dst_maxburst = 8;
        spi->tx_dma_xfer.src_addr = (rt_uint32_t)message->send_buf;
        spi->tx_dma_xfer.src_addr_width = pSPI->config.nBytes;
        spi->tx_dma_xfer.src_maxburst = 8;

        spi->tx_dma_xfer.len = message->length;
        spi->tx_dma_xfer.callback = rockchip_spi_dma_txcb;
        spi->tx_dma_xfer.cparam = spi;

        ret = rt_device_control(spi->dma, RT_DEVICE_CTRL_DMA_REQUEST_CHANNEL,
                                &spi->tx_dma_xfer);
        if (ret)
            goto rx_release;

        ret = rt_device_control(spi->dma, RT_DEVICE_CTRL_DMA_SINGLE_PREPARE,
                                &spi->tx_dma_xfer);
        if (ret)
            goto tx_release;
#ifdef RT_USING_CACHE
        rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)message->send_buf, message->length);
#endif
    }

    if (message->recv_buf)
    {
        spi->state |= RXBUSY;

        /* Start dma transfer. */
        ret = rt_device_control(spi->dma, RT_DEVICE_CTRL_DMA_START,
                                &spi->rx_dma_xfer);
        if (ret)
            goto tx_release;
    }

    if (message->send_buf)
    {
        spi->state |= TXBUSY;

        /* Start dma transfer. */
        ret = rt_device_control(spi->dma, RT_DEVICE_CTRL_DMA_START,
                                &spi->tx_dma_xfer);
        if (ret)
            goto rx_stop;
    }

    return RT_EOK;

rx_stop:
    if (message->recv_buf)
        rt_device_control(spi->dma, RT_DEVICE_CTRL_DMA_STOP,
                          &spi->rx_dma_xfer);

tx_release:
    if (message->send_buf)
        rt_device_control(spi->dma, RT_DEVICE_CTRL_DMA_RELEASE_CHANNEL,
                          &spi->tx_dma_xfer);

rx_release:
    if (message->recv_buf)
        rt_device_control(spi->dma, RT_DEVICE_CTRL_DMA_RELEASE_CHANNEL,
                          &spi->rx_dma_xfer);

    return ret;
}

/**
 * rockchip_spi_dma_complete - spi compete dma tranfer
 * @spi: SPI private structure which contains SPI specific data
 * @message: SPI message structure
 * @return: return error code
 */
static void rockchip_spi_dma_complete(struct rockchip_spi *spi, struct rt_spi_message *message)
{
    if (message->recv_buf)
    {
        rt_device_control(spi->dma, RT_DEVICE_CTRL_DMA_STOP,
                          &spi->rx_dma_xfer);
        rt_device_control(spi->dma, RT_DEVICE_CTRL_DMA_RELEASE_CHANNEL,
                          &spi->rx_dma_xfer);
#ifdef RT_USING_CACHE
        rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, (void *)message->recv_buf, message->length);
#endif
    }

    if (message->send_buf)
    {
        rt_device_control(spi->dma, RT_DEVICE_CTRL_DMA_STOP,
                          &spi->tx_dma_xfer);
        rt_device_control(spi->dma, RT_DEVICE_CTRL_DMA_RELEASE_CHANNEL,
                          &spi->tx_dma_xfer);
    }
}

/**
 * rockchip_spi_xfer - spi tranfer api call by spi ops
 * @device: SPI device structure
 * @message: SPI message structure
 * @return: return error code
 */
static rt_uint32_t rockchip_spi_xfer(struct rt_spi_device *device, struct rt_spi_message *message)
{
    struct rockchip_spi *spi = device->bus->parent.user_data;
    struct SPI_HANDLE *pSPI = &spi->instance;
    struct rockchip_spi_cs *cs = (struct rockchip_spi_cs *)device->parent.user_data;
    struct clk_gate *pclk_gate, *sclk_gate;
    rt_uint32_t timeout; /* ms */
    rt_err_t ret = RT_EOK;

    rt_mutex_take(&spi->spi_lock, RT_WAITING_FOREVER);

    pclk_gate = spi->pclk_gate;
    sclk_gate = spi->sclk_gate;

    pm_runtime_request(PM_RUNTIME_ID_SPI);

    clk_enable(pclk_gate);
    clk_enable(sclk_gate);

    if (message->send_buf == NULL && message->recv_buf == NULL)
    {
        if (message->cs_take)
        {
            HAL_SPI_SetCS(pSPI, cs->pin, true);
        }
        if (message->cs_release)
        {
            HAL_SPI_SetCS(pSPI, cs->pin, false);
        }

        clk_disable(sclk_gate);
        clk_disable(pclk_gate);
        pm_runtime_release(PM_RUNTIME_ID_SPI);
        rt_mutex_release(&spi->spi_lock);

        return RT_EOK;
    }

    spi->dma_burst_size = rockchip_spi_calc_burst_size(message->length);
    pSPI->dmaBurstSize = spi->dma_burst_size;
    /* Configure spi mode here. */
    HAL_SPI_Configure(pSPI, message->send_buf, message->recv_buf, message->length);

    spi->error = 0;
    if (!HAL_SPI_IsSlave(pSPI))
    {
        /* cs only for master */
        if (message->cs_take)
            HAL_SPI_SetCS(pSPI, cs->pin, true);

        /* Use poll mode for master while less fifo length. */
        if (spi->dma && HAL_SPI_CanDma(pSPI))
        {
            HAL_SPI_DmaTransfer(pSPI);
            ret = rockchip_spi_dma_prepare(spi, message);
            if (RT_EOK != ret)
                goto complete;

            timeout = HAL_SPI_CalculateTimeout(pSPI);
            ret = rt_completion_wait(&spi->done, rt_tick_from_millisecond(timeout));
            if (RT_EOK != ret)
            {
                rt_kprintf("%s timer out \n", __func__);
                rockchip_spi_dma_handle_err(spi);
            }
            else
            {
                rockchip_spi_dma_complete(spi, message);
            }
            if (message->send_buf && ret == RT_EOK)
            {
                rockchip_spi_wait_idle(spi);
            }
        }
        else
        {
            HAL_SPI_PioTransfer(pSPI);
            if (message->send_buf)
            {
                rockchip_spi_wait_idle(spi);
            }
        }
    }
    else
    {
        /* Use IT mode for slave while less fifo length. */
        if (spi->dma && HAL_SPI_CanDma(pSPI))
        {
            HAL_SPI_DmaTransfer(pSPI);
            ret = rockchip_spi_dma_prepare(spi, message);
            if (RT_EOK != ret)
                goto complete;

            /* Timeout is forever for slave. */
            rt_completion_wait(&spi->done, RT_WAITING_FOREVER);
            rockchip_spi_dma_complete(spi, message);
            if (message->send_buf && ret == RT_EOK)
            {
                rockchip_spi_wait_idle(spi);
            }
        }
        else
        {
            HAL_SPI_ItTransfer(pSPI);
            rt_hw_interrupt_umask(spi->hal_dev->irqNum);

            /* Timeout is forever for slave. */
            rt_completion_wait(&spi->done, RT_WAITING_FOREVER);

            rt_hw_interrupt_mask(spi->hal_dev->irqNum);
            if (message->send_buf)
            {
                rockchip_spi_wait_idle(spi);
            }
        }
    }

complete:
    if (RT_EOK != ret)
    {
        spi->error = ret;
        spi_dbg(&spi->bus.parent, "%s error: %d\n", __func__, spi->error);
    }

    /* Disable SPI when finished. */
    HAL_SPI_Stop(pSPI);

    /* cs only for master */
    if (!HAL_SPI_IsSlave(pSPI))
    {
        if (message->cs_release)
            HAL_SPI_SetCS(pSPI, cs->pin, false);
    }

    clk_disable(sclk_gate);
    clk_disable(pclk_gate);

    pm_runtime_release(PM_RUNTIME_ID_SPI);

    rt_mutex_release(&spi->spi_lock);

    /* Successful to return message length and fail to return 0. */
    return spi->error ? 0 : message->length;
}

static struct rt_spi_ops rockchip_spi_ops =
{
    rockchip_spi_configure,
    rockchip_spi_xfer,
};

/**
 * rockchip_spi_probe - probe function for SPI Controller
 * @spi: SPI private structure which contains SPI specific data
 * @bus_name: SPI bus name
 * @slave: slave or master mode for SPI
 */
static rt_err_t rockchip_spi_probe(struct rockchip_spi *spi, char *bus_name)
{
    char *dev_name;
    rt_err_t ret;

#ifdef RT_USING_DMA
    spi->dma = rt_dma_get(spi->hal_dev->txDma.dmac);
#endif
    if (spi->dma == NULL)
        rt_kprintf("%s only support CPU transfer\n", __func__);

    /* register irq */
    rt_hw_interrupt_install(spi->hal_dev->irqNum, (void *)spi->isr, spi, RT_NULL);
    rt_hw_interrupt_umask(spi->hal_dev->irqNum);

    HAL_SPI_Init(&spi->instance, spi->hal_dev->base, spi->hal_dev->isSlave);
    spi->pclk_gate = get_clk_gate_from_id(spi->hal_dev->pclkGateID);
    spi->sclk_gate = get_clk_gate_from_id(spi->hal_dev->clkGateID);

    rt_completion_init(&spi->done);
    rt_sem_init(&spi->sem_lock, "sem_lock", 1, RT_IPC_FLAG_FIFO);

    if (rt_mutex_init(&spi->spi_lock, "spi_lock", RT_IPC_FLAG_FIFO) != RT_EOK)
        RT_ASSERT(0);

    spi->bus.parent.user_data = spi;

    ret = rt_spi_bus_register(&spi->bus, bus_name, &rockchip_spi_ops);
    if (!ret)
    {
        dev_name = rt_malloc(20);
        rt_memset(dev_name, 0, 20);
        strcpy(dev_name, bus_name);
        strcat(dev_name, "_0");
        rt_spi_bus_attach_device(&spi->rt_spi_dev[0], dev_name, bus_name, (void *)&spi->rt_spi_cs[0]);
        rt_memset(dev_name, 0, 20);
        strcpy(dev_name, bus_name);
        strcat(dev_name, "_1");
        rt_spi_bus_attach_device(&spi->rt_spi_dev[1], dev_name, bus_name, (void *)&spi->rt_spi_cs[1]);
        rt_free(dev_name);
    }

    return ret;
}

#define DEFINE_ROCKCHIP_SPI(ID)                                                                       \
                                                                                                      \
static void rockchip_spi##ID##_irq(int irq, void *param);                                             \
                                                                                                      \
static struct rockchip_spi s_spi##ID =                                                                \
{                                                                                                     \
    .hal_dev = &g_spi##ID##Dev,                                                                         \
    .isr = (void *)&rockchip_spi##ID##_irq,                                                           \
    .rt_spi_cs[0].pin = ROCKCHIP_SPI_CS_0,                                                            \
    .rt_spi_cs[1].pin = ROCKCHIP_SPI_CS_1,                                                            \
};                                                                                                    \
                                                                                                      \
static void rockchip_spi##ID##_irq(int irq, void *param)                                              \
{                                                                                                     \
    rockchip_spi_irq(&s_spi##ID);                                                                     \
}                                                                                                     \

#ifdef RT_USING_SPI0
DEFINE_ROCKCHIP_SPI(0)
#endif

#ifdef RT_USING_SPI1
DEFINE_ROCKCHIP_SPI(1)
#endif

#ifdef RT_USING_SPI2
DEFINE_ROCKCHIP_SPI(2)
#endif

int rockchip_rt_hw_spi_init(void)
{
#ifdef RT_USING_SPI0
    rockchip_spi_probe(&s_spi0, "spi0");
#endif

#ifdef RT_USING_SPI1
    rockchip_spi_probe(&s_spi1, "spi1");
#endif

#ifdef RT_USING_SPI2
    rockchip_spi_probe(&s_spi2, "spi2");
#endif

    return 0;
}

INIT_PREV_EXPORT(rockchip_rt_hw_spi_init);
/** @} */  // SPI_Public_Functions

#endif
/** @} */

/** @} */
