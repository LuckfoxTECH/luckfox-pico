/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_sdio.c
  * @version V0.1
  * @brief   sdio driver interface
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-07-15     Shawn.lin       first implementation
  *
  ******************************************************************************
  */

#include <rtdef.h>
#include <rtdevice.h>
#include <rthw.h>

#ifdef RT_USING_SDIO
#include <drivers/mmcsd_core.h>
#include "drv_pm.h"
#include "hal_base.h"
#include "drv_clock.h"
#include "drv_sdio.h"
#include "dma.h"

#define RK_MMC_DBG 0
#define MMC_INTERNAL_DMA_BUF_SIZE (32 * 1024)

extern struct rk_mmc_platform_data rk_mmc_table[];

#if RK_MMC_DBG
#define PRINT_MMC_DBG(fmt, args...)     \
    do                                  \
    {                                   \
        rt_kprintf("RK_MMC_DBG: tick-%d, ", rt_tick_get());   \
        rt_kprintf(fmt, ## args);       \
    }                                   \
    while(0)
#else
#define PRINT_MMC_DBG(fmt, args...)  do { } while (0)
#endif

#define PRINT_MMC_REGS(base)                                        \
    do                                                              \
    {                                                               \
        int i_for_marco;                                            \
        rt_uint32_t addr;                                           \
        for(i_for_marco=0; i_for_marco<20; i_for_marco++)           \
        {                                                           \
            addr = base + i_for_marco*4*4;                          \
            rt_kprintf("0x%x: 0x%x, 0x%x, 0x%x, 0x%x\n", addr,      \
                    GET_REG(addr+0x0),                              \
                    GET_REG(addr+0x4),                              \
                    GET_REG(addr+0x8),                              \
                    GET_REG(addr+0xc));                             \
        }                                                           \
    }                                                               \
    while(0)

static void rk_mmc_interrupt(rt_uint32_t ID);

#define DEFINE_ROCKCHIP_MMC(ID)                            \
static void rk_mmc##ID##_irq(int irq, void *param)   \
{                                                           \
    rk_mmc_interrupt(ID);                         \
}

#define MAX_ID_NUM 2

#if defined(RT_USING_SDIO0)
DEFINE_ROCKCHIP_MMC(0);
#endif /* RT_USING_SDIO0 */

#if defined(RT_USING_SDIO1)
DEFINE_ROCKCHIP_MMC(1);
#endif /* RT_USING_SDIO1 */

#if defined(RT_USING_SDIO2)
DEFINE_ROCKCHIP_MMC(2);
#endif /* RT_USING_SDIO2 */

static void rk_mmc_register_irq(rt_uint32_t irq, rt_uint32_t ID)
{
    /* enable interrupt */
#ifdef RT_USING_SDIO0
    if (ID == 0)
        rt_hw_interrupt_install(irq, rk_mmc0_irq, NULL, "mmc_isr_sdio0");
#endif

#ifdef RT_USING_SDIO1
    if (ID == 1)
        rt_hw_interrupt_install(irq, rk_mmc1_irq, NULL, "mmc_isr_sdio1");
#endif

#ifdef RT_USING_SDIO2
    if (ID == 2)
        rt_hw_interrupt_install(irq, rk_mmc2_irq, NULL, "mmc_isr_sdio2");
#endif

    rt_hw_interrupt_umask(irq);
}

static int rk_mmc_write_pio(struct mmc_driver *mmc_drv)
{
    struct HAL_MMC_HOST *hal_host = (struct HAL_MMC_HOST *)mmc_drv->priv;
    struct rt_mmcsd_cmd *cmd = mmc_drv->cmd;
    struct rt_mmcsd_data *data = RT_NULL;
    rt_uint32_t size;

    if (cmd)
        data = cmd->data;

    if (!data)
    {
        rt_kprintf("ERROR: %s, data is NULL\n", __func__);
        return -RT_EIO;
    }

    size = data->blks * data->blksize;
    PRINT_MMC_DBG("%s, Send %d bytes\n", __func__, size);
    HAL_MMC_WriteData(hal_host, data->buf, size);
    HAL_MMC_ResetFifo(hal_host);

    return 0;
}

static int rk_mmc_read_pio(struct mmc_driver *mmc_drv)
{
    struct HAL_MMC_HOST *hal_host = (struct HAL_MMC_HOST *)mmc_drv->priv;
    struct rt_mmcsd_cmd *cmd = mmc_drv->cmd;
    struct rt_mmcsd_data *data = RT_NULL;
    rt_uint32_t size;
    int ret;

    if (cmd)
        data = cmd->data;

    if (!data)
    {
        rt_kprintf("ERROR: %s, data is NULL\n", __func__);
        return -RT_EIO;
    }

    size = data->blks * data->blksize;
    PRINT_MMC_DBG("%s, read %d bytes\n", __func__, size);
    ret = HAL_MMC_ReadData(hal_host, data->buf, size);
    if (ret)
    {
        rt_kprintf("ERROR: %s, fifo IO error, ret: %d\n", __func__, ret);
        return -RT_EIO;
    }

    HAL_MMC_ResetFifo(hal_host);

    return 0;
}

static void rk_mmc_set_iocfg(struct rt_mmcsd_host *host, struct rt_mmcsd_io_cfg *io_cfg)
{
    rt_uint32_t clkdiv = 0;
    struct mmc_driver *mmc_drv = host->private_data;
    struct HAL_MMC_HOST *hal_host = (struct HAL_MMC_HOST *)mmc_drv->priv;

    PRINT_MMC_DBG("%s start\n", __func__);

    /*
     * TBD:
     * (1) support reading ciu from CRU
     * (2) support dynamic clk enable and disable
     */
    if (io_cfg->clock)
    {
        clkdiv = mmc_drv->host->freq_max / io_cfg->clock / 2;
        HAL_MMC_UpdateClockRegister(hal_host, clkdiv);
        hal_host->cachedDiv = clkdiv;
        PRINT_MMC_DBG("io_cfg->clock: %lu, clock in: %lu, clkdiv: %d\n",
                      io_cfg->clock, MMC_CLOCK_IN, clkdiv);
    }

    if (io_cfg->bus_width == MMCSD_BUS_WIDTH_4)
    {
        HAL_MMC_SetCardWidth(hal_host, MMC_CARD_WIDTH_4BIT);
        hal_host->cachedBw = MMC_CARD_WIDTH_4BIT;
        PRINT_MMC_DBG("set to 4-bit mode\n", mmc_drv->host->freq_max, clkdiv);
    }
    else if (io_cfg->bus_width == MMCSD_BUS_WIDTH_8)
    {
        HAL_MMC_SetCardWidth(hal_host, MMC_CARD_WIDTH_8BIT);
        hal_host->cachedBw = MMC_CARD_WIDTH_8BIT;
        PRINT_MMC_DBG("set to 8-bit mode\n", mmc_drv->host->freq_max, clkdiv);
    }
    else
    {
        HAL_MMC_SetCardWidth(hal_host, MMC_CARD_WIDTH_1BIT);
        hal_host->cachedBw = MMC_CARD_WIDTH_1BIT;
        PRINT_MMC_DBG("set to 1-bit mode\n", mmc_drv->host->freq_max, clkdiv);
    }

    /*
     * maybe switch power to the card
     * FIXME: Rockchip boards are most likely to play with inverse
     * logic for using PWERN, so don't be astonished if seeing we
     * set power state to FALSE for MMCSD_POWER_UP....
     */
    switch (io_cfg->power_mode)
    {
    case MMCSD_POWER_OFF:
        HAL_MMC_PowerCtrl(hal_host, false);
        break;
    case MMCSD_POWER_UP:
        HAL_MMC_PowerCtrl(hal_host, true);
        break;
    case MMCSD_POWER_ON:
        break;
    default:
        rt_kprintf("ERROR: %s, unknown power_mode %d\n", __func__, io_cfg->power_mode);
        break;
    }
    PRINT_MMC_DBG("%s end\n", __func__);
}

static void rk_mmc_enable_sdio_irq(struct rt_mmcsd_host *host, rt_int32_t enable)
{
    struct mmc_driver *mmc_drv = host->private_data;
    struct HAL_MMC_HOST *hal_host = (struct HAL_MMC_HOST *)mmc_drv->priv;
    rt_uint32_t reg;

    PRINT_MMC_DBG("%s start\n", __func__);

    if (enable)
    {
        HAL_MMC_ClearRawInterrupt(hal_host, MMC_INT_STATUS_SDIO);
        reg = HAL_MMC_GetInterruptMask(hal_host);
        reg |= MMC_INT_STATUS_SDIO;
        HAL_MMC_SetInterruptMask(hal_host, reg);
    }
    else
    {
        reg = HAL_MMC_GetInterruptMask(hal_host);
        reg &= ~MMC_INT_STATUS_SDIO;
        HAL_MMC_SetInterruptMask(hal_host, reg);
    }
}

static rt_int32_t rk_mmc_get_card_status(struct rt_mmcsd_host *host)
{
    PRINT_MMC_DBG("%s, start\n", __func__);
    PRINT_MMC_DBG("%s, end\n", __func__);
    return 0;
}

static void rk_mmc_send_command(struct mmc_driver *mmc_drv, struct rt_mmcsd_cmd *cmd)
{
    struct HAL_MMC_HOST *hal_host = (struct HAL_MMC_HOST *)mmc_drv->priv;
    struct rt_mmcsd_req *req = mmc_drv->req;
    struct rt_mmcsd_data *data = cmd->data;
    int ret;

    rt_uint32_t cmd_flags = 0;

    if (!cmd)
    {
        /* fixme: stop dma */
        rt_kprintf("ERROR: %s, cmd is NULL\n", __func__);
        return;
    }

    if (data)
    {
        cmd_flags |= MMC_CMD_FLAG_DATA_EXPECTED;
        /* always set data start - also set direction flag for read */
        if (data->flags & DATA_DIR_WRITE)
            cmd_flags |= MMC_CMD_FLAG_WRITE_TO_CARD;

        if (data->flags & DATA_STREAM)
            cmd_flags |= MMC_CMD_FLAG_DATA_STREAM;
    }

    if (cmd == req->stop)
        cmd_flags |= MMC_CMD_FLAG_STOP_TRANSFER;
    else
        cmd_flags |= MMC_CMD_FLAG_WAIT_PREV_DATA;

    switch (resp_type(cmd))
    {
    case RESP_NONE:
        break;
    case RESP_R1:
    case RESP_R5:
    case RESP_R6:
    case RESP_R7:
    case RESP_R1B:
        cmd_flags |= MMC_CMD_FLAG_RESPONSE_EXPECTED;
        cmd_flags |= MMC_CMD_FLAG_CHECK_RESP_CRC;
        break;
    case RESP_R2:
        cmd_flags |= MMC_CMD_FLAG_RESPONSE_EXPECTED;
        cmd_flags |= MMC_CMD_FLAG_CHECK_RESP_CRC;
        cmd_flags |= MMC_CMD_FLAG_LONG_RESPONSE;
        break;
    case RESP_R3:
    case RESP_R4:
        cmd_flags |= MMC_CMD_FLAG_RESPONSE_EXPECTED;
        break;
    default:
        rt_kprintf("ERROR: %s, unknown cmd type %x\n", __func__, resp_type(cmd));
        return;
    }

    if (cmd->cmd_code == GO_IDLE_STATE)
        cmd_flags |= MMC_CMD_FLAG_SEND_INIT;

    /* CMD 11 check switch voltage */
    if (cmd->cmd_code == READ_DAT_UNTIL_STOP)
        cmd_flags |= MMC_CMD_FLAG_SWITCH_VOLTAGE;

    PRINT_MMC_DBG("cmd code: %d, args: 0x%x, resp type: 0x%x, flag: 0x%x\n",
                  cmd->cmd_code, cmd->arg, resp_type(cmd), cmd_flags);
    ret = HAL_MMC_SendCommand(hal_host, cmd->cmd_code, cmd->arg, cmd_flags);

    if (ret)
    {
        rt_kprintf("ERROR: %s, Send command timeout, cmd: %d, status: 0x%x\n", __func__,
                   cmd->cmd_code, HAL_MMC_GetStatus(hal_host));
    }
}
static void rk_mmc_prepare_data(struct mmc_driver *mmc_drv)
{
    struct rt_mmcsd_cmd *cmd = mmc_drv->cmd;
    struct rt_mmcsd_data *data = cmd->data;
    struct HAL_MMC_HOST *hal_host = (struct HAL_MMC_HOST *)mmc_drv->priv;
    rt_uint32_t data_size;
    bool use_internal_buffer = false;

    if (!data)
    {
        HAL_MMC_SetBlockSize(hal_host, 0);
        HAL_MMC_SetByteCount(hal_host, 0);
        return;
    }

    data_size = data->blks * data->blksize;

#ifdef MMC_FORCE_INTERNAL_BUF
    use_internal_buffer = true;
#endif

    if (!HAL_IS_CACHELINE_ALIGNED((rt_uint32_t)(data->buf)) ||
            !HAL_IS_CACHELINE_ALIGNED(data_size))
        use_internal_buffer = true;

    HAL_MMC_SetBlockSize(hal_host, data->blksize);

    /*
    if (data_size % 4)
    {
        rt_kprintf("warnning: data_size should better be a multiple of 4, but now is %d\n", data_size);
    }
    */

    HAL_MMC_SetByteCount(hal_host, data_size);

    PRINT_MMC_DBG("%s, set blk size: 0x%x, byte count: 0x%x\n", __func__, data->blksize, data_size);

    if (data_size > MMC_DMA_DESC_BUFF_SIZE * mmc_drv->max_desc)
    {
        rt_kprintf("ERROR: %s, given buffer is too big, size: 0x%x, max: 0x%x\n", __func__,
                   data_size, MMC_DMA_DESC_BUFF_SIZE * mmc_drv->max_desc);
        return;
    }

    if (data_size > MMC_INTERNAL_DMA_BUF_SIZE)
    {
        rt_kprintf("ERROR: please increase MMC_INTERNAL_DMA_BUF_SIZE.\n");
        return;
    }

#ifdef MMC_USE_DMA
    if (data->flags & DATA_DIR_WRITE)
    {
        if (use_internal_buffer)
        {
            rt_memcpy(mmc_drv->mmc_dma_buf, data->buf, data_size);
            rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, mmc_drv->mmc_dma_buf, data_size);
        }
        else
        {
            rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, data->buf, data_size);
        }
    }

    if (use_internal_buffer)
        HAL_MMC_InitDescriptors(hal_host, (rt_uint32_t *)mmc_drv->mmc_dma_buf, data_size);
    else
        HAL_MMC_InitDescriptors(hal_host, data->buf, data_size);

    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, hal_host->descriptors,
                         sizeof(MMC_DMA_DESCRIPTORS) * mmc_drv->max_desc);

    HAL_MMC_StartDma(hal_host);
#endif
}

int rk_mmc_wait_card_idle(struct HAL_MMC_HOST *hal_host)
{
    rt_uint32_t tick, timeout;

    tick = rt_tick_get();
    timeout = tick + RT_TICK_PER_SECOND * 5; //5000ms

    while (HAL_MMC_IsDataStateBusy(hal_host))
    {
        tick = rt_tick_get();
        if (tick > timeout)
        {
            return -RT_ETIMEOUT;
        }
    }

    return 0;
}

static void delay_us(uint32_t us)
{
    volatile uint32_t len;

    for (; us > 0; us--)
        for (len = 0; len < 20; len++)
            ;
}

static int rk_mmc_check_cmd(struct HAL_MMC_HOST *hal_host, rt_uint32_t cmd_code)
{
    rt_uint32_t tick, timeout, status;
    tick = rt_tick_get();
    timeout = tick + RT_TICK_PER_SECOND; //500ms

    do
    {
        status = HAL_MMC_GetRawInterrupt(hal_host);
        tick = rt_tick_get();
        if (tick > timeout)
        {
            PRINT_MMC_DBG("ERROR: %s, get response timeout(cmd is not received by card), RINTSTS: 0x%x, cmd: %d\n",
                          __func__, status, cmd_code);
            return -RT_ETIMEOUT;
        }
    }
    while (!(status & MMC_INT_STATUS_CMD_DONE));

    HAL_MMC_ClearRawInterrupt(hal_host, MMC_INT_STATUS_CMD_DONE);

    status = HAL_MMC_GetRawInterrupt(hal_host);
    if (status & MMC_INT_STATUS_RESPONSE_TIMEOUT)
    {
        HAL_MMC_ClearRawInterrupt(hal_host, MMC_INT_STATUS_RESPONSE_TIMEOUT);
        PRINT_MMC_DBG("ERROR: %s, get response timeout, RINTSTS: 0x%x\n", __func__, status);
        return -RT_ETIMEOUT;
    }

    else if (status & (MMC_INT_STATUS_RESP_CRC_ERROR | MMC_INT_STATUS_RESPONSE_ERROR))
    {
        HAL_MMC_ClearRawInterrupt(hal_host, MMC_INT_STATUS_RESP_CRC_ERROR | MMC_INT_STATUS_RESPONSE_ERROR);
        rt_kprintf("ERROR: %s, response error or response crc error, RINTSTS: 0x%x\n", __func__, status);
        return -RT_ERROR;
    }

    return RT_EOK;
}

static int rk_mmc_get_response(struct mmc_driver *mmc_drv, struct rt_mmcsd_cmd *cmd)
{
    int i;
    struct HAL_MMC_HOST *hal_host = (struct HAL_MMC_HOST *)mmc_drv->priv;

    cmd->resp[0] = 0;
    cmd->resp[1] = 0;
    cmd->resp[2] = 0;
    cmd->resp[3] = 0;

    i = rk_mmc_check_cmd(hal_host, cmd->cmd_code);
    if (i)
        return i;

    for (i = 0; i < 4; i++)
    {
        if (resp_type(cmd) == RESP_R2)
        {
            cmd->resp[i] = HAL_MMC_GetResponse(hal_host, 3 - i);
            //fixme : R2 must delay some time 1ms here ,when use UHS-I card.
            //rt_thread_sleep(RT_TICK_PER_SECOND / 100);
            delay_us(5000);
        }
        else
        {
            cmd->resp[i] = HAL_MMC_GetResponse(hal_host, i);
        }
    }

    PRINT_MMC_DBG("resp: 0x%x, 0x%x, 0x%x, 0x%x\n", cmd->resp[0], cmd->resp[1], cmd->resp[2], cmd->resp[3]);

    return 0;
}

static int rk_mmc_start_transfer(struct mmc_driver *mmc_drv)
{
    struct HAL_MMC_HOST *hal_host = (struct HAL_MMC_HOST *)mmc_drv->priv;
    struct rt_mmcsd_cmd *cmd = mmc_drv->cmd;
    struct rt_mmcsd_data *data = RT_NULL;
    int ret;
    rt_uint32_t interrupt, status, reg;

    if (cmd)
        data = cmd->data;

    if (!data)
    {
        return 0;
    }

    PRINT_MMC_DBG("%s, start\n", __func__);

    //open data interrupts
    reg = HAL_MMC_GetInterruptMask(hal_host);
    reg |= MMC_INT_STATUS_DATA;
    HAL_MMC_SetInterruptMask(hal_host, reg);

    //fixme: spin_unlock_irqrestore(&host->lock, flags);
    ret = rt_completion_wait(&mmc_drv->transfer_completion, RT_TICK_PER_SECOND * 5);

    reg = HAL_MMC_GetInterruptMask(hal_host);
    reg &= ~MMC_INT_STATUS_DATA;
    HAL_MMC_SetInterruptMask(hal_host, reg);

    if (ret)
    {
        cmd->err = ret;
        interrupt = HAL_MMC_GetRawInterrupt(hal_host);
        status = HAL_MMC_GetStatus(hal_host);
        HAL_MMC_ClearRawInterrupt(hal_host, interrupt);
        rt_kprintf("ERROR: %s, transfer timeout, ret: %d, RINTSTS: 0x%x, STATUS: 0x%x\n", __func__, ret, interrupt, status);
        return -RT_ETIMEOUT;
    }

    if (mmc_drv->transfer_state & MMC_INIT_STATUS_DATA_ERROR)
    {
        mmc_drv->transfer_state = 0;
        return -RT_ERROR;
    }

    data->bytes_xfered = data->blks * data->blksize;
#ifdef MMC_USE_DMA
    if (!(data->flags & DATA_DIR_WRITE))
    {
#ifdef MMC_FORCE_INTERNAL_BUF
        rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, mmc_drv->mmc_dma_buf, data->bytes_xfered);
        rt_memcpy(data->buf, mmc_drv->mmc_dma_buf, data->bytes_xfered);
#else
        if (!HAL_IS_CACHELINE_ALIGNED((rt_uint32_t)(data->buf)) ||
                !HAL_IS_CACHELINE_ALIGNED(data->bytes_xfered))
        {
            rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, mmc_drv->mmc_dma_buf, data->bytes_xfered);
            rt_memcpy(data->buf, mmc_drv->mmc_dma_buf, data->bytes_xfered);
        }
        else
        {
            rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, data->buf, data->bytes_xfered);
        }
#endif
    }
#endif

    return 0;
}

static void rk_mmc_complete_request(struct mmc_driver *mmc_drv)
{
    struct HAL_MMC_HOST *hal_host = (struct HAL_MMC_HOST *)mmc_drv->priv;

#ifdef MMC_USE_DMA
    HAL_MMC_StopDma(hal_host);
#endif
    mmc_drv->cmd = RT_NULL;
    mmc_drv->req = RT_NULL;
    mmc_drv->data = RT_NULL;

    rt_memset(hal_host->descriptors, 0, 4096);

    HAL_MMC_SetBlockSize(hal_host, 0);
    HAL_MMC_SetByteCount(hal_host, 0);

    mmcsd_req_complete(mmc_drv->host);
}

static void rk_mmc_request(struct rt_mmcsd_host *host, struct rt_mmcsd_req *req)
{
    int ret;
    struct mmc_driver *mmc_drv = host->private_data;
    struct rt_mmcsd_cmd *cmd = req->cmd;
    struct HAL_MMC_HOST *hal_host = (struct HAL_MMC_HOST *)mmc_drv->priv;

    mmc_drv->transfer_state = 0;
    mmc_drv->req = req;
    mmc_drv->cmd = cmd;

    rt_completion_init(&mmc_drv->transfer_completion);

    pm_runtime_request(PM_RUNTIME_ID_SDIO);

    ret = rk_mmc_wait_card_idle(hal_host);
    if (ret)
    {
        rt_kprintf("ERROR: %s, data transfer timeout, status: 0x%x\n", __func__, HAL_MMC_GetStatus(hal_host));
        goto out;
    }

    /*  Should we need reset it all the time? */
    ret = HAL_MMC_ResetFifo(hal_host);
    if (ret)
    {
        rt_kprintf("ERROR: %s fail to reset FIFO", __func__);
        cmd->err = -RT_ERROR;
        goto out;
    }

    rk_mmc_prepare_data(mmc_drv);
    rk_mmc_send_command(mmc_drv, cmd);
    ret = rk_mmc_get_response(mmc_drv, cmd);
    if (ret)
    {
        cmd->err = ret;
        rt_kprintf("%s,get response returns %d, cmd: %d\n", __func__, ret, cmd->cmd_code);
        goto out;
    }
    rk_mmc_start_transfer(mmc_drv);

    if (req->stop)
    {
        /* send stop command */
        PRINT_MMC_DBG("%s send stop\n", __func__);
        rk_mmc_send_command(mmc_drv, req->stop);
    }

out:
    rk_mmc_complete_request(mmc_drv);
    pm_runtime_release(PM_RUNTIME_ID_SDIO);
}

#if defined(RT_USING_PM)
static int rt_mmc_pm_suspend(const struct rt_device *device)
{
    struct HAL_MMC_HOST *hal_host = device->user_data;
    rt_uint32_t cmd_flags = MMC_CMD_FLAG_WAIT_PREV_DATA;
    int ret = RT_EOK;

    if (!hal_host->keepPowerInSuspend)
        return ret;

    pm_runtime_request(PM_RUNTIME_ID_SDIO);

    ret = rk_mmc_wait_card_idle(hal_host);
    if (ret)
    {
        rt_kprintf("ERROR: %s, wiat prev idle timeout, status: 0x%x\n",
                   __func__, HAL_MMC_GetStatus(hal_host));
        goto out;
    }

    HAL_MMC_SetBlockSize(hal_host, 0);
    HAL_MMC_SetByteCount(hal_host, 0);

    /* Deselect card */
    ret = HAL_MMC_SendCommand(hal_host, SELECT_CARD, 0, cmd_flags);
    if (ret)
    {
        rt_kprintf("ERROR: %s, Send sleep command timeout, status: 0x%x\n",
                   __func__, HAL_MMC_GetStatus(hal_host));
        goto out;;
    }

    ret = rk_mmc_check_cmd(hal_host, SELECT_CARD);
    if (ret)
        goto out;

    cmd_flags |= MMC_CMD_FLAG_RESPONSE_EXPECTED;
    cmd_flags |= MMC_CMD_FLAG_CHECK_RESP_CRC;
    /* RCA is 1 for mmc card */
    ret = HAL_MMC_SendCommand(hal_host, SLEEP_AWAKE, 1 << 16 | 1 << 15, cmd_flags);
    if (ret)
    {
        rt_kprintf("ERROR: %s, Send sleep command timeout, status: 0x%x\n",
                   __func__, HAL_MMC_GetStatus(hal_host));
        goto out;;
    }

    while (HAL_MMC_IsDataStateBusy(hal_host));

out:

    if (ret)
        rt_kprintf("ERROR: %s, idle timeout, status: 0x%x\n",
                   __func__, HAL_MMC_GetStatus(hal_host));

    pm_runtime_release(PM_RUNTIME_ID_SDIO);
    return ret;
}

static void rt_mmc_pm_resume(const struct rt_device *device)
{
    struct HAL_MMC_HOST *hal_host = device->user_data;
    rt_uint32_t cmd_flags = MMC_CMD_FLAG_WAIT_PREV_DATA;
    int ret = RT_EOK;

    HAL_MMC_Init(hal_host);
    HAL_MMC_PowerCtrl(hal_host, true);
    HAL_MMC_UpdateClockRegister(hal_host, hal_host->cachedDiv);
    HAL_MMC_SetCardWidth(hal_host, hal_host->cachedBw);

    if (!hal_host->keepPowerInSuspend)
        return;

    HAL_MMC_SetBlockSize(hal_host, 0);
    HAL_MMC_SetByteCount(hal_host, 0);

    cmd_flags |= MMC_CMD_FLAG_RESPONSE_EXPECTED;
    cmd_flags |= MMC_CMD_FLAG_CHECK_RESP_CRC;
    /* RCA is 1 for mmc card */
    ret = HAL_MMC_SendCommand(hal_host, SLEEP_AWAKE, 1 << 16 | 0 << 15, cmd_flags);
    if (ret)
    {
        rt_kprintf("ERROR: %s, Send awake command timeout, status: 0x%x\n",
                   __func__, HAL_MMC_GetStatus(hal_host));
        return;
    }
    while (HAL_MMC_IsDataStateBusy(hal_host));

    /* Select card */
    cmd_flags = MMC_CMD_FLAG_WAIT_PREV_DATA;
    ret = HAL_MMC_SendCommand(hal_host, SELECT_CARD, 1 << 16, cmd_flags);
    if (ret)
    {
        rt_kprintf("ERROR: %s, Send select command timeout, status: 0x%x\n",
                   __func__, HAL_MMC_GetStatus(hal_host));
    }

    ret = rk_mmc_check_cmd(hal_host, SELECT_CARD);
}

static struct rt_device_pm_ops rk_mmc_pm_ops =
{
    .suspend = rt_mmc_pm_suspend,
    .resume = rt_mmc_pm_resume,
};
#endif


static const struct rt_mmcsd_host_ops rk_mmc_ops =
{
    .request            = rk_mmc_request,
    .set_iocfg          = rk_mmc_set_iocfg,
    .enable_sdio_irq    = rk_mmc_enable_sdio_irq,
    .get_card_status    = rk_mmc_get_card_status,
};

static void rk_mmc_interrupt(rt_uint32_t ID)
{
    rt_uint32_t status;
    struct mmc_driver *mmc_drv = NULL;
    struct HAL_MMC_HOST *hal_host = NULL;
    struct rk_mmc_platform_data *pdata = NULL;
    rt_uint32_t i;

    for (i = 0; i < MAX_ID_NUM; i++)
    {
        pdata = &rk_mmc_table[i];
        if (pdata == NULL || pdata->base == 0)
            break;

        if (pdata->control_id == ID)
        {
            mmc_drv = pdata->drv;
            hal_host = (struct HAL_MMC_HOST *)mmc_drv->priv;
            break;
        }
    }

    HAL_ASSERT(hal_host != NULL && mmc_drv != NULL);

    /* enter interrupt */
    rt_interrupt_enter();
    status = HAL_MMC_GetUnmaskedInterrupt(hal_host);
    PRINT_MMC_DBG("unmasked interrupts: 0x%x\n", status);

    if (status & MMC_INT_STATUS_CARD_DETECT)
    {
        rt_uint32_t card_status = HAL_MMC_GetCardStatus(hal_host);

        if (card_status == CARD_UNPLUGED)
        {
            rt_kprintf("card disconnected\n");
        }
        else
        {
            rt_kprintf("card connected\n");
        }
        mmcsd_change(mmc_drv->host);
    }

    if (status & MMC_INT_STATUS_SDIO)
    {
        sdio_irq_wakeup(mmc_drv->host);
    }

    if (status & MMC_INIT_STATUS_DATA_ERROR)
    {
        rt_kprintf("ERROR: %s, data error, status: 0x%x\n", __func__, status);
        mmc_drv->transfer_state = MMC_INIT_STATUS_DATA_ERROR;
    }

    if (status & MMC_INT_STATUS_TRANSFER_OVER)
    {
    }

    if (status & MMC_INT_STATUS_TX_REQUEST)
    {
        rk_mmc_write_pio(mmc_drv);
    }

    if (status & MMC_INT_STATUS_RX_REQUEST)
    {
        rk_mmc_read_pio(mmc_drv);
    }

    HAL_MMC_ClearRawInterrupt(hal_host, MMC_INT_STATUS_ALL);
    rt_completion_done(&mmc_drv->transfer_completion);
    /* leave interrupt */
    rt_interrupt_leave();
}

int rk_mmc_probe(struct rk_mmc_platform_data *pdata)
{
    struct mmc_driver *mmc_drv;
    struct rt_mmcsd_host *host;
    struct HAL_MMC_HOST *hal_host;
    struct rt_device *dev = RT_NULL;

    PRINT_MMC_DBG("%s start\n", __func__);

    hal_host = (struct HAL_MMC_HOST *)rt_malloc(sizeof(struct HAL_MMC_HOST));
    rt_memset(hal_host, 0, sizeof(struct HAL_MMC_HOST));

    mmc_drv = (struct mmc_driver *)rt_malloc(sizeof(struct mmc_driver));
    rt_memset(mmc_drv, 0, sizeof(struct mmc_driver));

    mmc_drv->priv = hal_host;

    host = mmcsd_alloc_host();
    if (!host)
    {
        rt_kprintf("ERROR: %s, failed to malloc host\n", __func__);
        return -RT_ENOMEM;
    }

    /* cache-line aligned */
    hal_host->descriptors = (MMC_DMA_DESCRIPTORS *)rt_dma_malloc(4096 + 64);

    mmc_drv->mmc_dma_buf = rt_dma_malloc(MMC_INTERNAL_DMA_BUF_SIZE + 64);

    if (!hal_host->descriptors)
    {
        rt_kprintf("ERROR: %s, failed to malloc dma descriptors\n", __func__);
        return -RT_ENOMEM;
    }

    rt_memset(hal_host->descriptors, 0, 4096);
    mmc_drv->max_desc = 4096 / (sizeof(MMC_DMA_DESCRIPTORS));

    host->ops = &rk_mmc_ops;
    host->freq_min = pdata->freq_min;
    host->freq_max = pdata->freq_max;
    host->valid_ocr = VDD_32_33 | VDD_33_34;
    host->flags = pdata->flags;

    host->max_seg_size = MMC_DMA_DESC_BUFF_SIZE;
    host->max_dma_segs = mmc_drv->max_desc;
    host->max_blk_size = 512;
    host->max_blk_count = 2048;
    host->private_data = mmc_drv;

    mmc_drv->host = host;

    hal_host->pReg = (struct MMC_REG *)pdata->base;
    hal_host->irq = pdata->irq;
    hal_host->keepPowerInSuspend = pdata->keep_power_in_suspend;
    pdata->drv = mmc_drv;
    HAL_MMC_Init(hal_host);

    rk_mmc_register_irq(hal_host->irq, pdata->control_id);
    mmcsd_change(host);

    HAL_MMC_SetInterruptMask(hal_host, MMC_INT_STATUS_CARD_DETECT);
#if defined(RT_USING_PM)
    dev = rt_calloc(1, sizeof(struct rt_device));
    if (!dev)
        return -RT_ENOMEM;
    dev->user_data = hal_host;
    rt_pm_register_device(dev, &rk_mmc_pm_ops);
#endif

    PRINT_MMC_DBG("%s end\n", __func__);

    return 0;
}

int rk_mmc_init(void)
{
    int i;
    struct rk_mmc_platform_data *pdata;
    struct clk_gate *clkgate;

    for (i = 0; i < MAX_ID_NUM; i++)
    {
        pdata = &rk_mmc_table[i];
        if (pdata == NULL || pdata->base == 0)
            break;

        rt_kprintf("base = 0x%x, irq = %d\n", pdata->base, pdata->irq);
        /* Adjust input card clock, and we have a hardware divider 2 */
        clk_set_rate(pdata->clk_id, pdata->freq_max * 2);
        clkgate = get_clk_gate_from_id(HCLK_SDIO_GATE);
        clk_enable(clkgate);
        clkgate = get_clk_gate_from_id(CLK_SDIO_GATE);
        clk_enable(clkgate);
        rk_mmc_probe(pdata);
    }
    return 0;
}

INIT_DEVICE_EXPORT(rk_mmc_init);
#endif
