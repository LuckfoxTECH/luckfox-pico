/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_snor.c
  * @version V1.1
  * @brief   spi nor interface
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-02-20     Dingqiang Lin   the first version
  * 2019-06-27     Dingqiang Lin   support FSPI
  * 2019-08-02     Dingqiang Lin   support SPI
  * 2020-09-23     Dingqiang Lin   Support scan and attach two independent spi nor
  *
  ******************************************************************************
  */

/** @addtogroup RKBSP_Driver_Reference
 *  @{
 */

/** @addtogroup SNOR
 *  @{
 */

/** @defgroup SNOR_How_To_Use How To Use
 *  @{

    SNOR is a framework protocol layer based on nor flash. It needs to be combined
 with the corresponding driver layer to complete the transmission of the protocol.

 - Littler fs      Elm fat fs
 - MTD Layer       Block Layer
 - SNOR Protocol Layer
 - Controller Host Layer (FSPI, SFC, SPI)
 - Nor flash

 reference to <Rockchip_Developer_Guide_RT-Thread_SPIFLASH_CN.md> to get more informations

 @} */

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#include "board.h"
#include "hal_bsp.h"
#include "drv_clock.h"
#include "drv_snor.h"
#include "drv_flash_partition.h"
#include "hal_base.h"

#ifdef RT_USING_SNOR

/********************* Private MACRO Definition ******************************/
/** @defgroup SNOR_Private_Macro Private Macro
 *  @{
 */

#ifdef SNOR_DEBUG
#define SNOR_DBG(...)     rt_kprintf(__VA_ARGS__)
#else
#define SNOR_DBG(...)
#endif

#define SPIFLASH_MTD_DEV_NAME_MAX 8
#define SPINOR_ID_MAX_LENGTH      5    /**< SPI Nor id data max length */

#define MTD_TO_SPIFLASH(m) (struct spiflash_device *)(m)

/** @} */  // SNOR_Private_Macro

/********************* Private Structure Definition **************************/
/** @defgroup SNOR_Private_Structure Private Structure
 *  @{
 */

struct spiflash_device s_spiflash[SPIFLASH_DEV_NUM];
static const struct rt_mtd_nor_driver_ops snor_mtd_ops;

/** @} */  // SNOR_Private_Structure

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/
/** @defgroup SNOR_Private_Function Private Function
 *  @{
 */

#ifdef RT_USING_SNOR_FSPI_HOST
static HAL_Status fspi_xfer(struct SNOR_HOST *spi, struct HAL_SPI_MEM_OP *op)
{
    struct HAL_FSPI_HOST *host = (struct HAL_FSPI_HOST *)spi->userdata;

    host->mode = spi->mode;
    host->cs = 0;
    return HAL_FSPI_SpiXfer(host, op);
}

static HAL_Status fspi_xip_config(struct SNOR_HOST *spi, struct HAL_SPI_MEM_OP *op, uint32_t on)
{
    struct HAL_FSPI_HOST *host = (struct HAL_FSPI_HOST *)spi->userdata;

    host->cs = 0;
    if (op)
    {
        HAL_FSPI_XmmcSetting(host, op);
    }

    return HAL_FSPI_XmmcRequest(host, on);
}

static int rockchip_sfc_delay_lines_tuning(struct SPI_NOR *nor, struct HAL_FSPI_HOST *host)
{
    uint8_t id_temp[SPINOR_ID_MAX_LENGTH];
    uint16_t cell_max = (uint16_t)HAL_FSPI_GetMaxDllCells(host);
    uint16_t right, left = 0;
    uint16_t step = HAL_FSPI_DLL_TRANING_STEP;
    bool dll_valid = false;

    HAL_SNOR_Init(nor);
    for (right = 0; right <= cell_max; right += step)
    {
        int ret;

        ret = HAL_FSPI_SetDelayLines(host, right);
        if (ret)
        {
            dll_valid = false;
            break;
        }
        ret = HAL_SNOR_ReadID(nor, id_temp);
        if (ret)
        {
            dll_valid = false;
            break;
        }

        SNOR_DBG("dll read flash id:%x %x %x\n",
                 id_temp[0], id_temp[1], id_temp[2]);

        ret = HAL_SNOR_IsFlashSupported(id_temp);
        if (dll_valid && !ret)
        {
            right -= step;

            break;
        }
        if (!dll_valid && ret)
            left = right;

        if (ret)
            dll_valid = true;

        /* Add cell_max to loop */
        if (right == cell_max)
            break;
        if (right + step > cell_max)
            right = cell_max - step;
    }

    if (dll_valid && (right - left) >= HAL_FSPI_DLL_TRANING_VALID_WINDOW)
    {
        if (left == 0 && right < cell_max)
            host->cell = left + (right - left) * 2 / 5;
        else
            host->cell = left + (right - left) / 2;
    }
    else
    {
        host->cell = 0;
    }

    if (host->cell)
    {
        SNOR_DBG("%d %d %d dll training success in %dMHz max_cells=%u\n",
                 left, right, host->cell, nor->spi->speed, cell_max);
        HAL_FSPI_SetDelayLines(host, host->cell);
        return 0;
    }
    else
    {
        SNOR_DBG("%d %d dll training failed in %dMHz, reduce the frequency\n",
                 left, right, nor->spi->speed);
        HAL_FSPI_DLLDisable(host);
        return -1;
    }
}

static uint32_t fspi_snor_adapt(struct SPI_NOR *nor)
{
    struct HAL_FSPI_HOST *host = &g_fspi0Dev;
    uint32_t ret;
    rt_base_t level;
    int dll_result = 0;

    RT_ASSERT(host);

    /* Designated host to SNOR */
    level = rt_hw_interrupt_disable();
    if (RT_SNOR_SPEED > 0 && RT_SNOR_SPEED <= SNOR_SPEED_MAX)
    {
        nor->spi->speed = RT_SNOR_SPEED;
    }
    else
    {
        nor->spi->speed = SNOR_SPEED_DEFAULT;
    }
    HAL_CRU_ClkSetFreq(host->sclkID, nor->spi->speed);
    nor->spi->speed = HAL_CRU_ClkGetFreq(host->sclkID);

    host->xmmcDev[0].type = DEV_NOR;
    /* XIP disabeld */
    HAL_FSPI_Init(host);
    nor->spi->userdata = (void *)host;
    nor->spi->mode = HAL_SPI_MODE_3;
    nor->spi->xfer = fspi_xfer;

    if (nor->spi->speed > HAL_FSPI_SPEED_THRESHOLD)
    {
        dll_result = rockchip_sfc_delay_lines_tuning(nor, host);
    }
    else
    {
        HAL_FSPI_DLLDisable(host);
    }

#ifndef RT_SNOR_DUAL_IO
    nor->spi->mode |= (HAL_SPI_TX_QUAD | HAL_SPI_RX_QUAD);
#else
    nor->spi->mode |= HAL_SPI_RX_DUAL;
#endif
#ifdef HAL_FSPI_XIP_ENABLE
    nor->spi->mode |= HAL_SPI_XIP;
    nor->spi->xipConfig = fspi_xip_config;
    nor->spi->xipMem = host->xipMemData;
    nor->spi->xipMemCode = host->xipMemCode;
#endif

    /* Init spi nor abstract */
    ret = HAL_SNOR_Init(nor);
    if ((ret == HAL_OK) && (nor->spi->mode & HAL_SPI_XIP))
    {
        HAL_SNOR_XIPEnable(nor);
    }

    if (dll_result)
    {
        HAL_CRU_ClkSetFreq(host->sclkID, HAL_FSPI_SPEED_THRESHOLD);
        nor->spi->speed = HAL_CRU_ClkGetFreq(host->sclkID);
        rt_kprintf("%s dll turning failed %d\n", __func__, dll_result);
    }

    rt_hw_interrupt_enable(level);

    return ret;
}
#endif

#ifdef RT_USING_SNOR_SFC_HOST
static uint32_t sfc_snor_adapt(struct SPI_NOR *nor)
{
    struct HAL_SFC_HOST *host = (struct HAL_SFC_HOST *)rt_calloc(1, sizeof(*host));

    RT_ASSERT(host);

    /* Designated host to SNOR */
    host->instance = SFC;
    HAL_SFC_Init(host);
    nor->spi->userdata = (void *)host;
    nor->spi->mode = HAL_SPI_MODE_3;
    nor->spi->mode |= (HAL_SPI_TX_QUAD | HAL_SPI_RX_QUAD);
    nor->spi->xfer = HAL_SFC_SpiXfer;

    /* Init spi nor abstract */
    if (HAL_SNOR_Init(nor))
    {
        rt_free(host);
        return RT_ERROR;
    }
    else
    {
        return RT_EOK;
    }
}
#endif

#ifdef RT_USING_SNOR_SPI_HOST
HAL_Status spi_xfer(struct SNOR_HOST *spi, struct HAL_SPI_MEM_OP *op)
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

    SNOR_DBG("%s %x %lx\n", __func__, op->cmd.opcode, op->data.nbytes);
    op_len = sizeof(op->cmd.opcode) + op->addr.nbytes + op->dummy.nbytes;
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

    SNOR_DBG("%s finished %d\n", __func__, ret);

    return ret;
}

static uint32_t spi_snor_adapt(struct SPI_NOR *nor)
{
    struct rt_spi_device *spi_device = NULL;

#if defined(RT_SNOR_SPI_DEVICE_NAME)
    spi_device = (struct rt_spi_device *)rt_device_find(RT_SNOR_SPI_DEVICE_NAME);
#endif
    if (!spi_device)
    {
        rt_kprintf("%s can not find %s\n", __func__, RT_SNOR_SPI_DEVICE_NAME);

        return -RT_EINVAL;
    }

    /* Designated host to SNOR */
    nor->spi->userdata = (void *)spi_device;
    nor->spi->mode = HAL_SPI_MODE_3;
    nor->spi->xfer = spi_xfer;
#if (RT_SNOR_SPEED > 0 && RT_SNOR_SPEED <= HAL_SPI_MASTER_MAX_SCLK_OUT)
    nor->spi->speed = RT_SNOR_SPEED;
#else
    nor->spi->speed = HAL_SPI_MASTER_MAX_SCLK_OUT;
#endif

    /* Init spi nor abstract */
    return HAL_SNOR_Init(nor);
}
#endif

static int snor_init(uint8_t dev_id, char *name, enum spiflash_host type)
{
    int ret = -RT_ERROR;
    struct spiflash_device *spiflash;
    struct rt_mtd_nor_device *mtd_dev;
    struct rt_mutex *lock;
    struct SPI_NOR *nor;

    if (dev_id >= SPIFLASH_DEV_NUM)
    {
        return -RT_EINVAL;
    }

    spiflash = &s_spiflash[dev_id];
    rt_memset(spiflash, 0, sizeof(struct spiflash_device));

    mtd_dev = &spiflash->dev;
    lock = &spiflash->lock;
    nor = &spiflash->nor;
    nor->spi = &spiflash->host;

    switch (type)
    {
#ifdef RT_USING_SNOR_FSPI_HOST
    case SPIFLASH_FSPI_HOST:
        ret = fspi_snor_adapt(nor);
        break;
#endif
#ifdef RT_USING_SNOR_SPI_HOST
    case SPIFLASH_SPI_HOST:
        ret = spi_snor_adapt(nor);
        break;
#endif
#ifdef RT_USING_SNOR_SFC_HOST
    case SPIFLASH_SFC_HOST:
        ret = sfc_snor_adapt(nor);
        break;
#endif
    default:
        rt_kprintf("%s dev type %d not support\n", __func__, type);
    }

    if (ret)
    {
        rt_kprintf("%s dev type %d fail ret= %d\n", __func__, type, ret);
        goto exit;
    }

    if (rt_mutex_init(lock, "snor_lock", RT_IPC_FLAG_FIFO) != RT_EOK)
    {
        rt_kprintf("Init mutex error\n");
        RT_ASSERT(0);
    }

    /* dev setting */
    mtd_dev->ops = &snor_mtd_ops;
    mtd_dev->block_size   = nor->sectorSize;
    mtd_dev->block_start  = 0;
    mtd_dev->block_end    = HAL_SNOR_GetCapacity(nor) / mtd_dev->block_size;
    rt_mtd_nor_register_device(name, mtd_dev);
    rk_partition_init(mtd_dev);
    spiflash->type = type;

exit:

    return ret;
}

/** @} */  // SNOR_Private_Function

/********************* Public Function Definition ****************************/

/** @defgroup SNOR_Public_Functions Public Functions
 *  @{
 */

/**
 * @brief  Suspend SNOR XIP function and disable interrupt
 */
rt_err_t rk_snor_xip_suspend(void)
{
#ifdef RT_USING_SNOR_FSPI_HOST
    struct spiflash_device *spiflash = &s_spiflash[0];

    if (spiflash->nor.spi->mode & HAL_SPI_XIP)
    {
        spiflash->level = rt_hw_interrupt_disable();
        HAL_SNOR_XIPDisable(&spiflash->nor);
    }
#endif

    return RT_EOK;
}

/**
 * @brief  Resume SNOR XIP function and enable interrupt
 */
rt_err_t rk_snor_xip_resume(void)
{
#ifdef RT_USING_SNOR_FSPI_HOST
    struct spiflash_device *spiflash = &s_spiflash[0];

    if (spiflash->nor.spi->mode & HAL_SPI_XIP)
    {
        HAL_SNOR_XIPEnable(&spiflash->nor);
        rt_hw_interrupt_enable(spiflash->level);
    }
#endif

    return RT_EOK;
}

/**
 * @brief  Suspend SNOR controller including XIP function.
 */
rt_err_t rk_snor_suspend(void)
{
    return rk_snor_xip_suspend();
}

/**
 * @brief  Resume SNOR controller including XIP function.
 */
rt_err_t rk_snor_resume(void)
{
#ifdef RT_USING_SNOR_FSPI_HOST
    struct spiflash_device *spiflash = &s_spiflash[0];

    if (spiflash->type == SPIFLASH_FSPI_HOST)
    {
        struct HAL_FSPI_HOST *host = (struct HAL_FSPI_HOST *)spiflash->host.userdata;

        HAL_FSPI_Init(host);
        if (spiflash->nor.spi->speed > HAL_FSPI_SPEED_THRESHOLD)
        {
            HAL_FSPI_SetDelayLines(host, host->cell);
        }
    }
#endif

    return rk_snor_xip_resume();
}

/*
 * The length of buffer is at least 8 bytes.
 */
rt_err_t snor_read_uuid(struct rt_mtd_nor_device *dev, uint8_t *buf)
{
    struct spiflash_device *spiflash = MTD_TO_SPIFLASH(dev);
    int32_t ret;

    rt_mutex_take(&spiflash->lock, RT_WAITING_FOREVER);
    rk_snor_xip_suspend();
    ret = HAL_SNOR_ReadUUID(&spiflash->nor, buf);
    rk_snor_xip_resume();
    rt_mutex_release(&spiflash->lock);

    return ret ? RT_ERROR : RT_EOK;
}

static rt_err_t snor_mtd_read_id(struct rt_mtd_nor_device *dev)
{
    struct spiflash_device *spiflash = MTD_TO_SPIFLASH(dev);
    rt_uint8_t id[5];

    rt_mutex_take(&spiflash->lock, RT_WAITING_FOREVER);
    rk_snor_xip_suspend();
    HAL_SNOR_ReadID(&spiflash->nor, id);
    rk_snor_xip_resume();
    rt_mutex_release(&spiflash->lock);

    return *(rt_uint32_t *)(id);
}


static rt_size_t snor_mtd_write(struct rt_mtd_nor_device *dev, rt_off_t pos, const rt_uint8_t *data, rt_size_t size)
{
    struct spiflash_device *spiflash = MTD_TO_SPIFLASH(dev);
    struct SPI_NOR *nor = &spiflash->nor;
    int ret;

    SNOR_DBG("%s pos = %08x,size = %08x\n", __func__, pos, size);

    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(size != 0);
    /* Buffer should not point to XIP memory */
    if (nor->spi->mode & HAL_SPI_XIP)
    {
        RT_ASSERT((((uint32_t)data + size) < nor->spi->xipMemCode) ||
                  ((uint32_t)data >= (nor->spi->xipMemCode + nor->size)));
        RT_ASSERT((((uint32_t)data + size) < nor->spi->xipMem) ||
                  ((uint32_t)data >= (nor->spi->xipMem + nor->size)));
    }

    rt_mutex_take(&spiflash->lock, RT_WAITING_FOREVER);
    rk_snor_xip_suspend();
    ret = HAL_SNOR_ProgData(nor, pos, (void *)data, size);
    rk_snor_xip_resume();
    rt_mutex_release(&spiflash->lock);
    if (ret != size)
    {
        return -RT_ERROR;
    }
    return size;
}

static rt_size_t snor_mtd_read(struct rt_mtd_nor_device *dev, rt_off_t pos, rt_uint8_t *data, rt_size_t size)
{
    struct spiflash_device *spiflash = MTD_TO_SPIFLASH(dev);
    struct SPI_NOR *nor = &spiflash->nor;
    int ret;

    SNOR_DBG("%s pos = %08x,size = %08x\n", __func__, pos, size);

    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(size != 0);

    rt_mutex_take(&spiflash->lock, RT_WAITING_FOREVER);
    if (nor->spi->mode & HAL_SPI_XIP)
    {
        HAL_DCACHE_InvalidateByRange((uint32_t)(pos + nor->spi->xipMem), size);
        rt_memcpy(data, (uint32_t *)(pos + nor->spi->xipMem), size);
        ret = size;
    }
    else
    {
        rk_snor_xip_suspend();
        ret = HAL_SNOR_ReadData(nor, pos, (void *)data, size);
        rk_snor_xip_resume();
    }
    rt_mutex_release(&spiflash->lock);
    if (ret != size)
    {
        return -RT_ERROR;
    }
    return size;
}

static rt_err_t snor_mtd_erase_sector(struct rt_mtd_nor_device *dev, rt_off_t pos, rt_uint32_t size)
{
    struct spiflash_device *spiflash = MTD_TO_SPIFLASH(dev);
    struct SPI_NOR *nor = &spiflash->nor;
    int ret = RT_EOK;
    uint32_t nsec = size / nor->sectorSize;

    SNOR_DBG("%s pos = %08x,size = %08x\n", __func__, pos, size);

    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(size != 0);

    rt_mutex_take(&spiflash->lock, RT_WAITING_FOREVER);
    rk_snor_xip_suspend();
    if (pos == 0 && size == nor->size)
    {
        ret = HAL_SNOR_Erase(nor, pos, ERASE_CHIP);
        if (ret)
        {
            ret = -RT_ERROR;
        }
        nsec = 0;
    }
    while (nsec)
    {
        ret = HAL_SNOR_Erase(nor, pos, ERASE_SECTOR);
        if (ret)
        {
            ret = -RT_ERROR;

            break;
        }
        nsec --;
        pos += dev->block_size;
    }
    rk_snor_xip_resume();
    rt_mutex_release(&spiflash->lock);
    if (ret != RT_EOK)
    {
        return -RT_ERROR;
    }
    return ret;
}

static const struct rt_mtd_nor_driver_ops snor_mtd_ops =
{
    snor_mtd_read_id,
    snor_mtd_read,
    snor_mtd_write,
    snor_mtd_erase_sector,
};

/**
 * @brief  Init SNOR framwork and apply to use.
 * @attention The SNOR will be enabled when board initialization, do not dynamically switch SNOR
 *      unless specifically required.
 */
int rt_hw_snor_init(void)
{
    int ret;

#if defined(RT_USING_SNOR_FSPI_HOST)
    ret = snor_init(0, "snor", SPIFLASH_FSPI_HOST);
#endif

#if defined(RT_USING_SNOR_SFC_HOST)
    ret = snor_init(0, "snor", SPIFLASH_SFC_HOST);
#endif

#if !defined(RT_USING_SNOR_FSPI_HOST) && defined(RT_USING_SNOR_SPI_HOST)
    ret = snor_init(0, "snor", SPIFLASH_SPI_HOST);
#elif defined(RT_USING_SNOR_FSPI_HOST) && defined(RT_USING_SNOR_SPI_HOST)
    ret = snor_init(1, "snor1", SPIFLASH_SPI_HOST);
#endif

    return ret;
}
INIT_DEVICE_EXPORT(rt_hw_snor_init);

/** @} */  // SNOR_Public_Function

#endif

/** @} */  // SNOR

/** @} */  // RKBSP_Common_Driver
