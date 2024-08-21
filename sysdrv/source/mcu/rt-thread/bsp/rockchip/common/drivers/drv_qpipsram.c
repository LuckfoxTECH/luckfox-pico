/**
  * Copyright (c) 2020 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_qpipsram.c
  * @version V1.0
  * @brief   qpi psram interface
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-04-30     Dingqiang Lin   the first version
  * 2020-09-18     Dingqiang Lin   Support PM
  *
  ******************************************************************************
  */

/** @addtogroup RKBSP_Driver_Reference
 *  @{
 */

/** @addtogroup QPIPSRAM
 *  @{
 */

/** @defgroup QPIPSRAM_How_To_Use How To Use
 *  @{

    QPIPSRAM is a framework protocol layer based on QPI psram. It needs to be combined
 with the corresponding driver layer to complete the transmission of the protocol.

 - PSRAM Layer(drv_psram.h only)
 - QPIPSRAM Protocol Layer
 - Controller Host Layer (FSPI, SFC, SPI)
 - QPI Psram

 How to read/write QPI Psram data in SPI mode:
 - consult to psram_test.c

 @} */

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#include "board.h"
#include "hal_bsp.h"
#include "hal_base.h"

#include "drv_psram.h"
#include "drv_clock.h"

#ifdef RT_USING_QPIPSRAM

/********************* Private MACRO Definition ******************************/
/** @defgroup QPIPSRAM_Private_Macro Private Macro
 *  @{
 */

// #define QPIPSRAM_DEBUG
#ifdef QPIPSRAM_DEBUG
#define QPIPSRAM_DBG(...)     rt_kprintf(__VA_ARGS__)
#else
#define QPIPSRAM_DBG(...)
#endif

/** @} */  // QPIPSRAM_Private_Macro

/********************* Private Structure Definition **************************/
/** @defgroup QPIPSRAM_Private_Structure Private Structure
 *  @{
 */

/** @} */  // QPIPSRAM_Private_Structure

/********************* Private Variable Definition ***************************/
/** @defgroup QPIPSRAM_Private_Macro Variable Macro
 *  @{
 */

#define DEFINE_ROCKCHIP_PSRAM_DEV(ID)                                           \
                                                                                \
static struct QPI_PSRAM s_qpipsram_dev##ID;                                     \
static struct QPIPSRAM_HOST s_spi_port##ID;                                     \
extern RT_UNUSED void rt_hw_qpipsram_cs##ID##_gpio_take(void);                  \
extern RT_UNUSED void rt_hw_qpipsram_cs##ID##_gpio_release(void);               \

DEFINE_ROCKCHIP_PSRAM_DEV(0)

#ifdef RT_USING_QPIPSRAM_FSPI_HOST_CS1
DEFINE_ROCKCHIP_PSRAM_DEV(1)
#endif

#ifdef RT_USING_QPIPSRAM_FSPI_HOST_CS2
DEFINE_ROCKCHIP_PSRAM_DEV(2)
#endif

#ifdef RT_USING_QPIPSRAM_FSPI_HOST_CS3
DEFINE_ROCKCHIP_PSRAM_DEV(3)
#endif

struct rt_mutex host_lock;

/** @} */  // QPIPSRAM_Variable_Macro

/********************* Private Function Definition ***************************/
/** @defgroup QPIPSRAM_Private_Function Private Function
 *  @{
 */

rt_err_t qpipsram_suspend(struct rk_psram_device *dev)
{
    struct QPI_PSRAM *psram = (struct QPI_PSRAM *)dev->priv;

    QPIPSRAM_DBG("%s enter ... \n", __func__);

#ifdef RT_USING_QPIPSRAM_FSPI_HOST
    if (s_qpipsram_dev0.spi->mode & HAL_SPI_XIP)
    {
        HAL_QPIPSRAM_XIPDisable(&s_qpipsram_dev0);
    }
#endif

    return HAL_QPIPSRAM_Supsend(psram);
}

rt_err_t qpipsram_resume(struct rk_psram_device *dev)
{
    rt_err_t ret;
    struct QPI_PSRAM *psram = (struct QPI_PSRAM *)dev->priv;

#ifdef RT_USING_QPIPSRAM_FSPI_HOST
    struct HAL_FSPI_HOST *host = (struct HAL_FSPI_HOST *)psram->spi->userdata;

    HAL_FSPI_Init(host);
    if (psram->spi->speed > HAL_FSPI_SPEED_THRESHOLD)
    {
        HAL_FSPI_SetDelayLines(host, host->cell);
    }

#endif

    ret = HAL_QPIPSRAM_Resume(psram);

#ifdef RT_USING_QPIPSRAM_FSPI_HOST
    if (s_qpipsram_dev0.spi->mode & HAL_SPI_XIP)
    {
        HAL_QPIPSRAM_XIPEnable(&s_qpipsram_dev0);
    }
#endif

    QPIPSRAM_DBG("%s exit ... \n", __func__);

    return ret;
}

rt_size_t qpipsram_read(struct rk_psram_device *dev, rt_off_t pos, rt_uint8_t *data, rt_size_t size)
{
    struct QPI_PSRAM *psram = (struct QPI_PSRAM *)dev->priv;
    int ret;

    QPIPSRAM_DBG("%s pos = %08x,size = %08x\n", __func__, pos, size);

    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(size != 0);

    rt_mutex_take(&host_lock, RT_WAITING_FOREVER);
    qpipsram_suspend(dev);
    ret = HAL_QPIPSRAM_ReadData(psram, pos, (void *)data, size);
    qpipsram_resume(dev);
    rt_mutex_release(&host_lock);
    if (ret != size)
    {
        rt_kprintf("%s fail, ret= %d\n", __func__, ret);

        return -RT_ERROR;
    }
    return size;
}

rt_size_t qpipsram_write(struct rk_psram_device *dev, rt_off_t pos, const rt_uint8_t *data, rt_size_t size)
{
    struct QPI_PSRAM *psram = (struct QPI_PSRAM *)dev->priv;
    int ret;

    QPIPSRAM_DBG("%s pos = %08x,size = %08x\n", __func__, pos, size);

    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(size != 0);

    rt_mutex_take(&host_lock, RT_WAITING_FOREVER);
    qpipsram_suspend(dev);
    ret = HAL_QPIPSRAM_ProgData(psram, pos, (void *)data, size);
    qpipsram_resume(dev);
    rt_mutex_release(&host_lock);
    if (ret != size)
    {
        return -RT_ERROR;
    }
    return size;
}

struct rk_psram_driver_ops qpipsram_ops =
{
    qpipsram_read,
    qpipsram_write,
    qpipsram_suspend,
    qpipsram_resume,
};

#if defined(RT_USING_QPIPSRAM_SPI_HOST)
static HAL_Status SPI_Xfer(struct QPIPSRAM_HOST *spi, struct HAL_SPI_MEM_OP *op)
{
    struct rt_spi_device *spi_device = (struct rt_spi_device *)spi->userdata;
    struct rt_spi_configuration cfg;
    uint32_t pos = 0;
    const uint8_t *tx_buf = NULL;
    uint8_t *rx_buf = NULL;
    uint8_t op_buf[HAL_SPI_OP_LEN_MAX];
    int32_t op_len;
    int32_t i, ret;

    if (op->data.nbytes)
    {
        if (op->data.dir == HAL_SPI_MEM_DATA_IN)
            rx_buf = op->data.buf.in;
        else
            tx_buf = op->data.buf.out;
    }

    /* rt_kprintf("%s %x %lx\n", __func__, op->cmd.opcode, op->data.nbytes); */
    op_len = sizeof(op->cmd.opcode) + op->addr.nbytes + op->dummy.nbytes;
    if (op_len > HAL_SPI_OP_LEN_MAX)
        return -RT_EINVAL;
    op_buf[pos++] = op->cmd.opcode;

    if (op->addr.nbytes)
    {
        for (i = 0; i < op->addr.nbytes; i++)
            op_buf[pos + i] = op->addr.val >> (8 * (op->addr.nbytes - i - 1));
        pos += op->addr.nbytes;
    }

    if (op->dummy.nbytes)
        memset(&op_buf[pos], 0xff, op->dummy.nbytes);

    cfg.data_width = 8;
    cfg.mode = spi->mode | RT_SPI_MSB;
    cfg.max_hz = spi->speed;
    rt_spi_configure(spi_device, &cfg);

    if (tx_buf)
    {
        ret = rt_spi_send_then_send(spi_device, op_buf, op_len, tx_buf, op->data.nbytes);
        if (ret)
            ret = HAL_ERROR;
    }
    else if (rx_buf)
    {
        ret = rt_spi_send_then_recv(spi_device, op_buf, op_len, rx_buf, op->data.nbytes);
        if (ret)
            ret = HAL_ERROR;
    }
    else
    {
        ret = rt_spi_send(spi_device, op_buf, op_len);
        if (ret != op_len)
            ret = HAL_ERROR;
        else
            ret = HAL_OK;
    }

    /* rt_kprintf("%s finished %d\n", __func__, ret); */

    return ret;
}

static uint32_t qpipsram_adapt(struct QPI_PSRAM *psram)
{
    struct rt_spi_device *spi_device = NULL;

#if defined(RT_QPIPSRAM_SPI_DEVICE_NAME)
    spi_device = (struct rt_spi_device *)rt_device_find(RT_QPIPSRAM_SPI_DEVICE_NAME);
#endif
    if (!spi_device)
    {
        rt_kprintf("%s can not find %s\n", __func__, RT_QPIPSRAM_SPI_DEVICE_NAME);

        return RT_EINVAL;
    }

    /* Designated host to QPIPSRAM */
    psram->spi->userdata = (void *)spi_device;
    psram->spi->mode = HAL_SPI_MODE_3;
    psram->spi->xfer = SPI_Xfer;
    if (RT_QPIPSRAM_SPEED > 0 && RT_QPIPSRAM_SPEED <= HAL_SPI_MASTER_MAX_SCLK_OUT)
    {
        psram->spi->speed = RT_QPIPSRAM_SPEED;
    }
    else
    {
        psram->spi->speed = HAL_SPI_MASTER_MAX_SCLK_OUT;
    }

    QPIPSRAM_DBG("%s find %s\n", __func__, RT_QPIPSRAM_SPI_DEVICE_NAME, psram->spi->speed);

    /* Init QPI Psram abstract */
    return HAL_QPIPSRAM_Init(psram);
}
#elif defined(RT_USING_QPIPSRAM_FSPI_HOST)
static HAL_Status SPI_Xfer(struct QPIPSRAM_HOST *spi, struct HAL_SPI_MEM_OP *op)
{
    struct HAL_FSPI_HOST *host = (struct HAL_FSPI_HOST *)spi->userdata;
    HAL_Status ret;

    host->cs = spi->cs >= 1 ? 1 : 0;
    host->mode = spi->mode;

    if (spi->cs_gpio_take)
        spi->cs_gpio_take();
    ret = HAL_FSPI_SpiXfer(host, op);
    if (spi->cs_gpio_release)
        spi->cs_gpio_release();

    if (ret)
    {
        rt_kprintf("%s fail, ret= %d\n", __func__, ret);
    }

    return ret;
}

#ifdef HAL_FSPI_XIP_ENABLE
static HAL_Status SPI_XipConfig(struct QPIPSRAM_HOST *spi, struct HAL_SPI_MEM_OP *op, uint32_t on)
{
    struct HAL_FSPI_HOST *host = (struct HAL_FSPI_HOST *)spi->userdata;

    host->cs = spi->cs;
    if (op)
    {
        host->xmmcDev[0].type = DEV_PSRAM;
        HAL_FSPI_XmmcSetting(host, op);
    }

    return HAL_FSPI_XmmcRequest(host, on);
}
#endif

static HAL_Status qpipsram_adapt(struct QPI_PSRAM *psram)
{
    struct HAL_FSPI_HOST *host = NULL;
    uint32_t ret;

    /* Designated host to SPI PSRAM */
#ifdef FSPI1
    host = &g_fspi1Dev;
#endif

    if (host == NULL)
        return HAL_ERROR;

    if (psram->spi->cs == 0)
    {
        if (RT_QPIPSRAM_SPEED > 0 && RT_QPIPSRAM_SPEED <= QPIPSRAM_SPEED_MAX)
        {
            psram->spi->speed = RT_QPIPSRAM_SPEED;
        }
        else
        {
            psram->spi->speed = QPIPSRAM_SPEED_DEFAULT;
        }
        clk_enable_by_id(host->sclkGate);
        clk_enable_by_id(host->hclkGate);
        clk_enable_by_id(host->xipClkGate);
        HAL_CRU_ClkSetFreq(host->sclkID, psram->spi->speed);
    }

    host->xmmcDev[psram->spi->cs].type = DEV_PSRAM;
    HAL_FSPI_Init(host);
    psram->spi->userdata = (void *)host;
    psram->spi->mode = HAL_SPI_MODE_3;
    psram->spi->xfer = SPI_Xfer;

    if (psram->spi->speed > HAL_FSPI_SPEED_THRESHOLD && psram->spi->cs == 0)
    {
        uint8_t idByte[2], id[2];
        int32_t right, left;

        HAL_QPIPSRAM_Init(psram);
        HAL_QPIPSRAM_ReadID(psram, id, 3);
        HAL_CRU_ClkSetFreq(host->sclkID, psram->spi->speed);
        left = -1;
        for (right = 0; right <= HAL_FSPI_MAX_DELAY_LINE_CELLS; right++)
        {
            HAL_FSPI_SetDelayLines(host, (uint8_t)right);
            HAL_QPIPSRAM_ReadID(psram, idByte, 2);
            QPIPSRAM_DBG("%s %d %x\n", __func__, right, *(uint16_t *)idByte);
            if ((idByte[0] == id[0] && idByte[1] == id[1]) && left == -1)
            {
                left = right;
            }
            else if ((idByte[0] != id[0] || idByte[1] != id[1]) && left >= 0)
            {
                break;
            }
        }

        if (left >= 0 && (right - left > 10))
        {
            host->cell = (uint8_t)((right + left) / 2);
            HAL_FSPI_SetDelayLines(host, host->cell);
        }
        else
        {
            HAL_CRU_ClkSetFreq(host->sclkID, HAL_FSPI_SPEED_THRESHOLD);
            HAL_FSPI_DLLDisable(host);
        }
    }
    else if (psram->spi->cs == 0)
    {
        HAL_CRU_ClkSetFreq(host->sclkID, psram->spi->speed);
    }

    psram->spi->mode |= HAL_SPI_TX_QUAD | HAL_SPI_RX_QUAD;

#ifdef HAL_FSPI_XIP_ENABLE
    if (!psram->spi->cs_gpio_take)
    {
        psram->spi->mode |= HAL_SPI_XIP;
        psram->spi->xipMem = host->xipMemData;
        psram->spi->xipConfig = SPI_XipConfig;
    }
#endif
    /* Init SPI psram abstract */

    ret = HAL_QPIPSRAM_Init(psram);
    if (ret && psram->spi->cs == 0)
    {
        clk_disable_by_id(host->sclkGate);
        clk_disable_by_id(host->hclkGate);
        clk_disable_by_id(host->xipClkGate);
    }

    return ret;
}
#else
static uint32_t qpipsram_adapt(struct QPI_PSRAM *psram)
{
    return RT_EINVAL;
}
#endif

rt_err_t qpipsram_init(struct QPI_PSRAM *psram, char *name)
{
    rt_err_t ret;

    ret = qpipsram_adapt(psram);
    if (ret)
    {
        rt_kprintf("QPIPSRAM %s Init adapter extend error ret= %ld\n", name, ret);
        return ret;
    }

    /* register qpipsram device */
    ret = rk_psram_register(&qpipsram_ops, HAL_QPIPSRAM_GetCapacity(psram), name, psram);
    if (ret)
    {
        rt_kprintf("QPIPSRAM %s register psram error ret= %ld\n", name, ret);
    }

    return ret;
}

/** @} */  // QPIPSRAM_Private_Function

/********************* Public Function Definition ****************************/

/** @defgroup QPIPSRAM_Public_Functions Public Functions
 *  @{
 */

/**
 * @brief  Init QPIPSRAM framwork and apply to use.
 * @attention The QPIPSRAM will be enabled when board initialization, do not dynamically switch QPIPSRAM
 *      unless specifically required.
 */
int rt_hw_qpipsram_init(void)
{
    int ret = -RT_ERROR;
    struct QPI_PSRAM *psram;

    if (rt_mutex_init(&host_lock, "host_lock", RT_IPC_FLAG_FIFO) != RT_EOK)
    {
        rt_kprintf("Init mutex error\n");
        RT_ASSERT(0);
        goto exit;
    }

    psram = &s_qpipsram_dev0;
    memset(psram, 0, sizeof(struct QPI_PSRAM));
    psram->spi = &s_spi_port0;
    memset(psram->spi, 0, sizeof(struct QPIPSRAM_HOST));
    psram->spi->cs = 0;

    if (qpipsram_init(psram, "psram0"))
    {
        goto exit;
    }

#ifdef RT_USING_QPIPSRAM_FSPI_HOST_CS1
    psram = &s_qpipsram_dev1;
    memset(psram, 0, sizeof(struct QPI_PSRAM));
    psram->spi = &s_spi_port1;
    memset(psram->spi, 0, sizeof(struct QPIPSRAM_HOST));
    psram->spi->cs = 1;

#ifdef RT_USING_QPIPSRAM_FSPI_HOST_CS1_GPIO
    psram->spi->cs_gpio_take = rt_hw_qpipsram_cs1_gpio_take;
    psram->spi->cs_gpio_release = rt_hw_qpipsram_cs1_gpio_release;
#endif
    if (qpipsram_init(psram, "psram1"))
    {
        goto exit;
    }
#endif

#ifdef RT_USING_QPIPSRAM_FSPI_HOST_CS2
    psram = &s_qpipsram_dev2;
    memset(psram, 0, sizeof(struct QPI_PSRAM));
    psram->spi = &s_spi_port2;
    memset(psram->spi, 0, sizeof(struct QPIPSRAM_HOST));
    psram->spi->cs = 2;

#ifdef RT_USING_QPIPSRAM_FSPI_HOST_CS2_GPIO
    psram->spi->cs_gpio_take = rt_hw_qpipsram_cs2_gpio_take;
    psram->spi->cs_gpio_release = rt_hw_qpipsram_cs2_gpio_release;
#endif
    if (qpipsram_init(psram, "psram2"))
    {
        goto exit;
    }
#endif

#ifdef RT_USING_QPIPSRAM_FSPI_HOST_CS3
    psram = &s_qpipsram_dev3;
    memset(psram, 0, sizeof(struct QPI_PSRAM));
    psram->spi = &s_spi_port3;
    memset(psram->spi, 0, sizeof(struct QPIPSRAM_HOST));
    psram->spi->cs = 3;

#ifdef RT_USING_QPIPSRAM_FSPI_HOST_CS3_GPIO
    psram->spi->cs_gpio_take = rt_hw_qpipsram_cs3_gpio_take;
    psram->spi->cs_gpio_release = rt_hw_qpipsram_cs3_gpio_release;
#endif
    if (qpipsram_init(psram, "psram3"))
    {
        goto exit;
    }
#endif

#ifdef RT_USING_QPIPSRAM_FSPI_HOST
    if (s_qpipsram_dev0.spi->mode & HAL_SPI_XIP)
    {
        HAL_QPIPSRAM_XIPEnable(&s_qpipsram_dev0);
    }
#endif
    ret = RT_EOK;

exit:
    return ret;
}
#ifdef RT_USING_QPIPSRAM_FSPI_HOST
INIT_BOARD_EXPORT(rt_hw_qpipsram_init);
#else
INIT_DEVICE_EXPORT(rt_hw_qpipsram_init);
#endif

/** @} */  // QPIPSRAM_Public_Function

#endif

/** @} */  // QPIPSRAM

/** @} */  // RKBSP_Common_Driver
