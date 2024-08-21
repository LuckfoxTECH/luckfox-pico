/**
  * Copyright (c) 2020 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_spinand.c
  * @version V1.0
  * @brief   spi nand interface
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-06-16     Dingqiang Lin   the first version
  *
  ******************************************************************************
  */

/** @addtogroup RKBSP_Driver_Reference
 *  @{
 */

/** @addtogroup SPINAND
 *  @{
 */

/** @defgroup SPINAND_How_To_Use How To Use
 *  @{

    SPI_NAND is a framework protocol layer based on SPI Nand flash. It needs to be combined
 with the corresponding driver layer to complete the transmission of the protocol.

 @} */

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <drivers/mtd.h>

#include "board.h"
#include "hal_bsp.h"
#include "drv_clock.h"
#include "hal_base.h"
#include "mini_ftl.h"

#ifdef RT_USING_SPINAND

/********************* Private MACRO Definition ******************************/
/** @defgroup SPINAND_Private_Macro Private Macro
 *  @{
 */
#ifdef SPINAND_DEBUG
#define spinand_dbg(...)     rt_kprintf(__VA_ARGS__)
#else
#define spinand_dbg(...)
#endif

#define MTD_TO_SPINAND(mtd) ((struct SPI_NAND *)mtd->priv)

/** @} */  // SPINAND_Private_Macro

/********************* Private Structure Definition **************************/
/** @defgroup SPINAND_Private_Structure Private Structure
 *  @{
 */

static struct rt_mutex spinand_lock;

int spinand_read(rt_mtd_t *mtd, loff_t from, struct mtd_io_desc *ops)
{
    int ret;

    spinand_dbg("%s addr= %lx len= %x\n", __func__, (uint32_t)from, (uint32_t)ops->datlen);

    rt_mutex_take(&spinand_lock, RT_WAITING_FOREVER);
    ret = mftl_mtd_read(mtd, from, ops);
    rt_mutex_release(&spinand_lock);

    return ret;
}

int spinand_write(rt_mtd_t *mtd, loff_t to, struct mtd_io_desc *ops)
{
    int ret;

    spinand_dbg("%s addr= %lx len= %x\n", __func__, (uint32_t)to, (uint32_t)ops->datlen);
    rt_mutex_take(&spinand_lock, RT_WAITING_FOREVER);
    ret = mftl_mtd_write(mtd, to, ops);
    rt_mutex_release(&spinand_lock);

    return ret;
}

int spinand_erase(rt_mtd_t *mtd, loff_t addr, size_t len)
{
    int ret;

    spinand_dbg("%s addr= %lx len= %lx\n", __func__, (uint32_t)addr, (uint32_t)len);
    rt_mutex_take(&spinand_lock, RT_WAITING_FOREVER);
    ret = mftl_mtd_erase(mtd, addr, len);
    rt_mutex_release(&spinand_lock);

    return ret;
}

int spinand_isbad(rt_mtd_t *mtd, uint32_t block)
{
    struct SPI_NAND *spinand = MTD_TO_SPINAND(mtd);
    int32_t ret = RT_EOK;

    spinand_dbg("%s blk= %lx\n", __func__, block);
    rt_mutex_take(&spinand_lock, RT_WAITING_FOREVER);
    ret = HAL_SPINAND_IsBad(spinand, block * spinand->pagePerBlk);
    rt_mutex_release(&spinand_lock);

    return ret;
}

int spinand_markbad(rt_mtd_t *mtd, uint32_t block)
{
    struct SPI_NAND *spinand = MTD_TO_SPINAND(mtd);
    int32_t ret = RT_EOK;

    spinand_dbg("%s blk= %lx\n", __func__, block);
    rt_mutex_take(&spinand_lock, RT_WAITING_FOREVER);
    ret = HAL_SPINAND_MarkBad(spinand, block * spinand->pagePerBlk);
    rt_mutex_release(&spinand_lock);

    return ret;
}

static const struct mtd_ops ops =
{
    spinand_erase,
    spinand_read,
    spinand_write,
    spinand_isbad,
    spinand_markbad,
};

#if defined(RT_USING_SPINAND_FSPI_HOST)
static HAL_Status SPI_Xfer(struct SPI_NAND_HOST *spi, struct HAL_SPI_MEM_OP *op)
{
    struct HAL_FSPI_HOST *host = (struct HAL_FSPI_HOST *)spi->userdata;

    host->mode = spi->mode;
    host->cs = 0;
    return HAL_FSPI_SpiXfer(host, op);
}

static uint32_t spinand_adapt(struct SPI_NAND *spinand)
{
    struct HAL_FSPI_HOST *host = &g_fspi1Dev;
    uint32_t ret;

    RT_ASSERT(host);

    /* Designated host to SPI_NAND */
    if (RT_SPINAND_SPEED > 0 && RT_SPINAND_SPEED <= SPINAND_SPEED_MAX)
    {
        spinand->spi->speed = RT_SPINAND_SPEED;
    }
    else
    {
        spinand->spi->speed = SPINAND_SPEED_DEFAULT;
    }
    clk_enable_by_id(host->sclkGate);
    clk_enable_by_id(host->hclkGate);
    clk_set_rate(host->sclkID, spinand->spi->speed);

    HAL_FSPI_Init(host);
    spinand->spi->userdata = (void *)host;
    spinand->spi->mode = HAL_SPI_MODE_3 | HAL_SPI_TX_QUAD | HAL_SPI_RX_QUAD;
    spinand->spi->xfer = SPI_Xfer;

    /* Init SPI_NAND abstract */
    ret = HAL_SPINAND_Init(spinand);
    if (ret)
    {
        clk_disable_by_id(host->sclkGate);
        clk_disable_by_id(host->hclkGate);
    }

    return ret;
}
#elif defined(RT_USING_SPINAND_SPI_HOST)
static HAL_Status SPI_Xfer(struct SPI_NAND_HOST *spi, struct HAL_SPI_MEM_OP *op)
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

//    rt_kprintf("%s %x %lx\n", __func__, op->cmd.opcode, op->data.nbytes);
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

//    rt_kprintf("%s finished %d\n", __func__, ret);

    return ret;
}

static uint32_t spinand_adapt(struct SPI_NAND *spinand)
{
    struct rt_spi_device *spi_device = NULL;

#if defined(RT_SPINAND_SPI_DEVICE_NAME)
    spi_device = (struct rt_spi_device *)rt_device_find(RT_SPINAND_SPI_DEVICE_NAME);
#endif
    if (!spi_device)
    {
        rt_kprintf("%s can not find %s\n", __func__, RT_SPINAND_SPI_DEVICE_NAME);

        return RT_EINVAL;
    }

    /* Designated host to SPI Nand */
    spinand->spi->userdata = (void *)spi_device;
    spinand->spi->mode = HAL_SPI_MODE_3;
    spinand->spi->xfer = SPI_Xfer;
    if (RT_SPINAND_SPEED > 0 && RT_SPINAND_SPEED <= HAL_SPI_MASTER_MAX_SCLK_OUT)
    {
        spinand->spi->speed = RT_SPINAND_SPEED;
    }
    else
    {
        spinand->spi->speed = HAL_SPI_MASTER_MAX_SCLK_OUT;
    }

    /* Init SPI Nand abstract */
    return HAL_SPINAND_Init(spinand);
}
#else
static uint32_t spinand_adapt(struct SPI_NAND *spinandF)
{
    return RT_EINVAL;
}
#endif

/** @} */  // SPINAND_Private_Function

/********************* Public Function Definition ****************************/

/** @defgroup SPINAND_Public_Functions Public Functions
 *  @{
 */

/* define partitions to it, mtd_spinand reserved for spi nand dev */
struct mtd_part spinand_parts[1] =
{
    { "spinand0", 0, 0, },
};

/**
 * @brief  Init SPI_NAND framwork and apply to use.
 * @attention The SPI_NAND will be enabled when board initialization, do not
 *      dynamically switch SPINAND unless specifically required.
 */
int rt_hw_spinand_init(void)
{
    struct mtd_info *mtd_dev;
    struct SPI_NAND *spinand;
    struct SPI_NAND_HOST *spi;
    int32_t ret;

    mtd_dev = (struct mtd_info *)rt_calloc(1, sizeof(*mtd_dev));
    RT_ASSERT(mtd_dev);
    spinand = (struct SPI_NAND *)rt_calloc(1, sizeof(*spinand));
    RT_ASSERT(spinand);
    spi = (struct SPI_NAND_HOST *)rt_calloc(1, sizeof(*spi));
    RT_ASSERT(spi);
    spinand->spi = spi;

    ret = spinand_adapt(spinand);
    if (ret)
    {
        rt_kprintf("SPI Nand init adapt error, ret= %ld\n", ret);
        goto exit;
    }

    /* flash feature setting */
    if (rt_mutex_init(&(spinand_lock), "spinandLock", RT_IPC_FLAG_FIFO) != RT_EOK)
    {
        rt_kprintf("Init mutex error\n");
        RT_ASSERT(0);
    }

    /* register mtd spinand */
    mtd_dev->sector_size     = spinand->secPerPage * SPINAND_SECTOR_SIZE;
    mtd_dev->writesize_shift = __rt_ffs(mtd_dev->sector_size) - 1;
    mtd_dev->writesize_mask  = mtd_dev->sector_size - 1;
    mtd_dev->block_size      = mtd_dev->sector_size  * spinand->pagePerBlk;
    mtd_dev->erasesize_shift = __rt_ffs(mtd_dev->block_size) - 1;
    mtd_dev->erasesize_mask  = mtd_dev->block_size - 1;
    mtd_dev->oob_size        = spinand->secPerPage * 16;
    mtd_dev->oob_avail        = spinand->secPerPage * 2;
    mtd_dev->offset          = 0;
    mtd_dev->size            = spinand->size;
    mtd_dev->priv            = spinand;
    mtd_dev->ops             = &ops;

    spinand_dbg("sector_size %lx\n", mtd_dev->sector_size);
    spinand_dbg("writesize_shift %lx\n", mtd_dev->writesize_shift);
    spinand_dbg("writesize_mask %lx\n", mtd_dev->writesize_mask);
    spinand_dbg("block_size %lx\n", mtd_dev->block_size);
    spinand_dbg("erasesize_shift %lx\n", mtd_dev->erasesize_shift);
    spinand_dbg("erasesize_mask %lx\n", mtd_dev->erasesize_mask);
    spinand_dbg("oob_size %lx\n", mtd_dev->oob_size);
    spinand_dbg("oob_avail %lx\n", mtd_dev->oob_avail);
    spinand_dbg("size %lx\n", mtd_dev->size);

    spinand_parts[0].size   = (uint32_t)mtd_dev->size;


    ret = rt_mtd_register(mtd_dev, (const struct mtd_part *)spinand_parts, HAL_ARRAY_SIZE(spinand_parts));
    ret = mini_ftl_register(mtd_dev);
    if (ret < 0)
    {
        rt_kprintf("mini_ftl_register register fail %d\n", ret);
        goto exit;
    }
exit:
    if (ret < 0)
    {
        rt_free(spinand->spi);
        rt_free(spinand);
        rt_free(mtd_dev);
    }

    return ret;
}
INIT_DEVICE_EXPORT(rt_hw_spinand_init);

/** @} */  // SPINAND_Public_Function

#endif

/** @} */  // SPINAND

/** @} */  // RKBSP_Common_Driver
