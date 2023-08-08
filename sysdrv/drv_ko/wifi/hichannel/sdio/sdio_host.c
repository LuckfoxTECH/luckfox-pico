/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: hcc layer frw task.
 * Author: dujinxin
 * Create: 2020-09-28
 */
#include "hcc_host.h"
#include "sdio_host.h"
#include "hcc_list.h"
#include "oam_ext_if.h"
#include "oal_mm.h"
#include "securec.h"
#include "oal_completion.h"
#include "hi_types.h"
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/pm_runtime.h>
#include <linux/mmc/sdio.h>
#include <linux/scatterlist.h>
#include <linux/sched/clock.h>
#include <linux/rfkill-wlan.h>

#if defined(RFKILL_RK)
#else
int rockchip_wifi_get_oob_irq(void)
{
	pr_info("rockchip_wifi_get_oob_irq number: %d\n", hi_rk_irq_gpio);

	return gpio_to_irq(hi_rk_irq_gpio);
}
#endif

#if 0
#if (_PRE_OS_PLATFORM == _PRE_PLATFORM_JZ)
#include <mach/jzmmc.h>
#include <soc/gpio.h>
#endif
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 宏定义 */
#define FUNC_NUM_SHIFT_BIT          28
#define REGISTER_ADDR_SHIFT_BIT     9

#define SDIO_PROBLE_TIMES           3
#define DELAY_10_US                 10
#define TIMEOUT_MUTIPLE_10          10
#define TIMEOUT_MUTIPLE_5           5
#define HCC_TASK_RX_ISR_NAME        "rx_isr"
#define HCC_TASK_RX_ISR_PRIO        3
#define HCC_TASK_RX_ISR_SIZE        0x400

#define gpio_reg_writel(addr, val)            *((volatile unsigned int *)(addr)) = val;

/* 支持离散内存收发 */
#define _SCATTER_LIST_MEMORY

/* 全局变量定义 */
static oal_completion g_sdio_driver_complete;
static oal_channel_stru *g_hi_sdio_;

static struct sdio_device_id const g_oal_sdio_ids[] = {
    { SDIO_DEVICE(HISDIO_VENDOR_ID_HISI, HISDIO_PRODUCT_ID_HISI) },
    {},
};

static hi_void oal_sdio_print_state(hi_u32 old_state, hi_u32 new_state)
{
    if (old_state != new_state) {
        oam_info_log4("sdio state changed, tx[%d=>%d],rx[%d=>%d] (1:on, 0:off)\n",
            (old_state & OAL_SDIO_TX) ? 1 : 0, (new_state & OAL_SDIO_TX) ? 1 : 0,
            (old_state & OAL_SDIO_RX) ? 1 : 0, (new_state & OAL_SDIO_RX) ? 1 : 0);
    }
}

hi_void oal_free_sdio_stru(oal_channel_stru *hi_sdio)
{
    hi_unref_param(hi_sdio);
    oam_error_log0("oal_free_sdio_stru\n");
}

oal_channel_stru *oal_get_sdio_default_handler(hi_void)
{
    return g_hi_sdio_;
}

oal_channel_stru *oal_sdio_alloc(struct sdio_func *func)
{
    oal_channel_stru* hi_sdio = HI_NULL;
    if (func == HI_NULL) {
        oam_error_log0("oal_sdio_alloc: func null!\n");
        return HI_NULL;
    }

    hi_sdio = oal_get_sdio_default_handler();
    if (hi_sdio == HI_NULL) {
        oam_error_log0("Failed to alloc hi_sdio!\n");
        return HI_NULL;
    }

    hi_sdio->func           = func;
    sdio_set_drvdata(func, hi_sdio);

    return hi_sdio;
}

hi_void oal_disable_sdio_state(oal_channel_stru *hi_sdio, hi_u32 mask)
{
    hi_u32 old_state;
    if (hi_sdio == HI_NULL || hi_sdio->func == HI_NULL) {
        oam_error_log0("oal_enable_sdio_state: hi_sdio null!\n");
        return;
    }

    oal_sdio_claim_host(hi_sdio->func);
    old_state = hi_sdio->state;
    hi_sdio->state &= ~mask;
    oal_sdio_print_state(old_state, hi_sdio->state);
    oal_sdio_release_host(hi_sdio->func);
}

static hi_s32 oal_sdio_sleep_dev_internal(oal_channel_stru *hi_sdio)
{
    int    ret;
    if (hi_sdio == HI_NULL || hi_sdio->func == HI_NULL) {
        return -OAL_EFAIL;
    }

    oal_sdio_claim_host(hi_sdio->func);
    sdio_f0_writeb(hi_sdio->func, ALLOW_TO_SLEEP_VALUE, HISDIO_WAKEUP_DEV_REG, &ret);
    oal_sdio_release_host(hi_sdio->func);
    return ret;
}

hi_s32 oal_sdio_send_msg(oal_channel_stru *hi_sdio, unsigned long val)
{
    hi_s32       ret  = HI_SUCCESS;
    struct sdio_func *func = HI_NULL;
    if (hi_sdio == HI_NULL || hi_sdio->func == HI_NULL) {
        oam_error_log0("{oal_sdio_send_msg::sdio is not initialized,can't send sdio msg!}");
        return -OAL_EINVAL;
    }

    func = hi_sdio->func;

    if (hi_sdio->bus_ops &&
        hi_sdio->bus_ops->pm_wakeup_dev) {
        if (hi_sdio->bus_ops->pm_wakeup_dev(hi_sdio->bus_data) != HI_SUCCESS) {
            oam_error_log0("{oal_sdio_send_msg::host wakeup device failed}");
            return -OAL_EBUSY;
        }
    }

    oal_sdio_claim_host(func);
    oal_sdio_writel(func, (1 << val), HISDIO_REG_FUNC1_WRITE_MSG, &ret);
    if (ret) {
        oam_error_log2("{oal_sdio_send_msg::failed to send sdio msg[%lu]!ret=%d}", val, ret);
    }
    oal_sdio_release_host(func);
    return ret;
}

hi_s32 oal_sdio_get_state(const oal_channel_stru *hi_sdio, hi_u32 mask)
{
    if (hi_sdio == HI_NULL) {
        return HI_FALSE;
    }

    if ((hi_sdio->state & mask) == mask) {
        return HI_TRUE;
    } else {
        return HI_FALSE;
    }
}

hi_void oal_enable_sdio_state(oal_channel_stru *hi_sdio, hi_u32 mask)
{
    hi_u32 old_state;
    if (hi_sdio == HI_NULL || hi_sdio->func == HI_NULL) {
        oam_error_log0("oal_enable_sdio_state: hi_sdio null!\n");
        return;
    }

    oal_sdio_claim_host(hi_sdio->func);
    old_state = hi_sdio->state;
    hi_sdio->state |= mask;
    oal_sdio_print_state(old_state, hi_sdio->state);
    oal_sdio_release_host(hi_sdio->func);
}

hi_void oal_sdio_dev_shutdown(struct device *dev)
{
    hi_s32   ret;
    hi_unref_param(dev);
    oal_channel_stru *hi_sdio = oal_get_sdio_default_handler();
    if ((hi_sdio == HI_NULL) || (hi_sdio->func == NULL)) {
        return;
    }

    if (oal_sdio_sleep_dev_internal(hi_sdio) != HI_SUCCESS) {
        return;
    }

    if (oal_sdio_send_msg(hi_sdio, H2D_MSG_PM_WLAN_OFF) != HI_SUCCESS) {
        return;
    }

    if (HI_TRUE != oal_sdio_get_state(hi_sdio, OAL_SDIO_ALL)) {
        return;
    }

    oal_sdio_claim_host(hi_sdio->func);
    ret = sdio_disable_func(hi_sdio->func);
    oal_sdio_release_host(hi_sdio->func);
    if (ret) {
        oam_error_log0("sdio_disable_func fail!\n");
    }
}

static hi_s32 oal_sdio_suspend(struct device *dev)
{
    struct sdio_func *func = HI_NULL;
    oal_channel_stru *hi_sdio = HI_NULL;

    oam_error_log0("+++++++sdio suspend+++++++++++++\n");
    if (dev == HI_NULL) {
         oam_error_log0("[WARN]dev is null\n");
        return HI_SUCCESS;
    }
    func = dev_to_sdio_func(dev);
    hi_sdio = sdio_get_drvdata(func);
    if (hi_sdio == HI_NULL) {
         oam_error_log0("hi_sdio is null\n");
        return HI_SUCCESS;
    }
    return HI_SUCCESS;
}

static hi_s32 oal_sdio_resume(struct device *dev)
{
    struct sdio_func *func = HI_NULL;
    oal_channel_stru *hi_sdio = HI_NULL;

    oam_error_log0("+++++++sdio resume+++++++++++++\n");
    if (dev == HI_NULL) {
        oam_error_log0("[WARN]dev is null\n");
        return HI_SUCCESS;
    }
    func = dev_to_sdio_func(dev);
    hi_sdio = sdio_get_drvdata(func);
    if (hi_sdio == HI_NULL) {
        oam_error_log0("hi_sdio is null\n");
        return HI_SUCCESS;
    }
    return HI_SUCCESS;
}

hi_s32 oal_sdio_transfer_rx_reserved_buff(const oal_channel_stru *hi_sdio)
{
    hi_s32 ret;
    if (hi_sdio->sdio_extend == HI_NULL) {
        oam_error_log0("{hi_sdio->sdio_extend NULL!}");
        return -OAL_EINVAL;
    }

    hi_u32 ul_extend_len = hi_sdio->sdio_extend->xfer_count;

    if (ul_extend_len == 0) {
        oam_error_log0("{extend_len is zero!}");
        return -OAL_EINVAL;
    }

    if (ul_extend_len > SDIO_MAX_XFER_LEN) {
        return -OAL_EINVAL;
    }

    ret = oal_sdio_readsb(hi_sdio->func, 0, hi_sdio->rx_buf, ul_extend_len);

    return ret;
}

hi_void oal_copy_data_to_uncbuf(const oal_channel_stru *hi_sdio,
    const hcc_data_queue *head,
    hi_u8* buf,
    hi_u32 len)
{
    hcc_unc_struc *unc_buf = HI_NULL;
    hcc_unc_struc *tmp = HI_NULL;
    hi_u32 offset = 0;
    hi_s32 index = 0;
    osal_list_for_each_entry_safe(unc_buf, tmp, (&head->data_queue), list) {
        memcpy_s(oal_unc_data(unc_buf), oal_unc_len(unc_buf), buf + offset, oal_unc_len(unc_buf));
        offset += (hi_sdio->sdio_extend->comm_reg[index] << HISDIO_D2H_SCATT_BUFFLEN_ALIGN_BITS);
        len -= (hi_sdio->sdio_extend->comm_reg[index] << HISDIO_D2H_SCATT_BUFFLEN_ALIGN_BITS);
        if (len  <= 0) {
            break;
        }
        index++;
    }
}

hi_s32 oal_sdio_tranfer_commit_memory(const oal_channel_stru *hi_sdio,
    const hcc_data_queue *head,
    hi_s32 rw)
{
    hi_s32 ret;
    hi_u32 queue_len;
    hi_u32 sum_len = 0;
    hi_u32 idx = 0;
    hcc_unc_struc *unc_buf = HI_NULL;
    hcc_unc_struc *tmp = HI_NULL;

    queue_len = hcc_list_len(head);

    osal_list_for_each_entry_safe(unc_buf, tmp, (&head->data_queue), list) {
        if (!oal_is_aligned((uintptr_t)oal_unc_data(unc_buf), 4)) { /* 4: 4字节对齐 */
            oam_error_log3("oal_sdio_transfer_netbuf_list netbuf 4 aligned fail!:: unc_buf[%p], len[%d], rw[%d]",
                oal_unc_data(unc_buf), oal_unc_len(unc_buf), rw);
            return -OAL_EINVAL;
        }

        if (WARN_ON(!oal_is_aligned(oal_unc_len(unc_buf), HIBUS_H2D_SCATT_BUFFLEN_ALIGN))) {
            return -OAL_EINVAL;
        }

        if (rw == SDIO_WRITE) {
            memcpy_s(hi_sdio->tx_buf + sum_len, SDIO_MAX_XFER_LEN, oal_unc_data(unc_buf), oal_unc_len(unc_buf));
        }

        sum_len += oal_unc_len(unc_buf);
        idx++;
    }

    if (oal_unlikely(idx > queue_len)) {
        return -OAL_EINVAL;
    }

    if (WARN_ON(sum_len > SDIO_MAX_XFER_LEN)) {
        return -OAL_EINVAL;
    }

    if (sum_len < HISDIO_BLOCK_SIZE) {
        sum_len = hi_byte_align(sum_len, 4); /* 4: 4字节对齐 */
    } else {
        sum_len = hi_byte_align(sum_len, HISDIO_BLOCK_SIZE);
    }

    oal_sdio_claim_host(hi_sdio->func);
    if (rw == SDIO_WRITE) {
        ret = oal_sdio_writesb(hi_sdio->func, 0, hi_sdio->tx_buf, sum_len);
    } else {
        ret = oal_sdio_readsb(hi_sdio->func, 0, hi_sdio->rx_buf, sum_len);
    }
    oal_sdio_release_host(hi_sdio->func);

    /* 将从device获取的数据拷贝到netbuf */
    if (rw == SDIO_READ) {
        oal_copy_data_to_uncbuf(hi_sdio, head, hi_sdio->rx_buf, sum_len);
    }
    return ret;
}

hi_void check_sg_format(struct scatterlist *sg, hi_u32 sg_len)
{
    hi_u32 i = 0;
    struct scatterlist *sg_t = HI_NULL;
    for_each_sg(sg, sg_t, sg_len, i) {
        if (oal_unlikely(HI_NULL == sg_t)) {
            return;
        }
        if (oal_warn_on(((uintptr_t)sg_virt(sg_t) & 0x03) || (sg_t->length & 0x03))) {
            oam_error_log3("check_sg_format:[i:%d][addr:%p][len:%u]\n", i, sg_virt(sg_t), sg_t->length);
        }
    }
}

hi_s32 oal_mmc_io_rw_scat_extended(const oal_channel_stru *hi_sdio,
    hi_s32 write, hi_u32 fn, hi_u32 addr,
    hi_s32 incr_addr, struct scatterlist *sg,
    hi_u32 sg_len, hi_u32 blocks, hi_u32 blksz)
{
    struct mmc_request mrq = {0};
    struct mmc_command cmd = {0};
    struct mmc_data data = {0};

    if ((blksz == 0) || (addr & ~0x1FFFF)) {
        return -EINVAL;
    }

    struct mmc_card *card = hi_sdio->func->card;
    check_sg_format(sg, sg_len);

    mrq.cmd = &cmd;
    mrq.data = &data;

    cmd.opcode = SD_IO_RW_EXTENDED;
    cmd.arg = write ? 0x80000000 : 0x00000000;
    cmd.arg |= fn << FUNC_NUM_SHIFT_BIT;
    cmd.arg |= incr_addr ? 0x04000000 : 0x00000000;
    cmd.arg |= addr << REGISTER_ADDR_SHIFT_BIT;
    if (blocks == 1 && blksz <= HISDIO_BLOCK_SIZE) {
        cmd.arg |= (blksz == HISDIO_BLOCK_SIZE) ? 0 : blksz;
    } else {
        cmd.arg |= 0x08000000 | blocks;
    }
    cmd.flags = MMC_RSP_SPI_R5 | MMC_RSP_R5 | MMC_CMD_ADTC;

    data.blksz = blksz;
    data.blocks = blocks;
    data.flags = write ? MMC_DATA_WRITE : MMC_DATA_READ;
    data.sg = sg;
    data.sg_len = sg_len;

    mmc_set_data_timeout(&data, card);
    mmc_wait_for_req(card->host, &mrq);

    if (cmd.error) {
        return cmd.error;
    }
    if (data.error) {
        return data.error;
    }

    if (oal_warn_on(mmc_host_is_spi(card->host))) {
        oam_error_log0("HiSi WiFi  driver do not support spi sg transfer!\n");
        return -EIO;
    }

    if (cmd.resp[0] & R5_ERROR) {
        return -EIO;
    }

    if (cmd.resp[0] & R5_FUNCTION_NUMBER) {
        return -EINVAL;
    }

    if (cmd.resp[0] & R5_OUT_OF_RANGE) {
        return -ERANGE;
    }

    return 0;
}

static hi_s32 _oal_sdio_transfer_scatt(const oal_channel_stru *hi_sdio,
    hi_s32 rw, hi_u32 addr, struct scatterlist *sg,
    hi_u32 sg_len, hi_u32 rw_sz)
{
    hi_s32 ret;
    hi_s32 write = (rw == SDIO_READ) ? 0 : 1;
    struct sdio_func *func = hi_sdio->func;
    oal_sdio_claim_host(func);

    if (oal_unlikely(oal_sdio_get_state(hi_sdio, OAL_SDIO_ALL) != HI_TRUE)) {
        oal_sdio_release_host(func);
        return -OAL_EFAIL;
    }
    ret = oal_mmc_io_rw_scat_extended(hi_sdio, write, sdio_func_num(hi_sdio->func), addr, 0, sg, sg_len,
        (rw_sz / HISDIO_BLOCK_SIZE) ? : 1, min(rw_sz, (hi_u32)HISDIO_BLOCK_SIZE));
    if (oal_unlikely(ret)) {
        if (write) {
            oam_error_log1("{oal_sdio_transfer_scatt::write failed=%d}", ret);
        } else {
            oam_error_log1("{oal_sdio_transfer_scatt::read failed=%d}", ret);
        }
    }
    oal_sdio_release_host(func);

    return ret;
}

hi_s32 oal_sdio_transfer_scatt(const oal_channel_stru *hi_sdio, hi_s32 rw, hi_u32 addr, struct scatterlist *sg,
    hi_u32 sg_len, hi_u32 sg_max_len, hi_u32 rw_sz)
{
    hi_u32 align_len;
    hi_u32 align_t;

    if ((hi_sdio == HI_NULL) || (rw_sz == 0) || (sg_max_len < sg_len)) {
        return -OAL_EINVAL;
    }

    if ((!hi_sdio) || (!rw_sz) || (sg_max_len < sg_len) || (sg == HI_NULL)) {
        oam_error_log3("oal_sdio_transfer_scatt: hi_sdio:%p,/rw_sz:%d,/sg_max_len<sg_len?:%d,/sg null}",
            (uintptr_t)hi_sdio, rw_sz, sg_max_len < sg_len);
        return -OAL_EINVAL;
    }

    if (oal_warn_on(!sg_len)) {
        oam_error_log2("Sdio %d(1:read,2:write) Scatter list num should never be zero, total request len: %u}",
            rw == SDIO_READ ? 1 : 2, rw_sz); /* 1:read,2:write */
        return -OAL_EINVAL;
    }

    align_t = hisdio_align_4_or_blk(rw_sz);
    align_len = align_t - rw_sz;

    if (oal_likely(align_len)) {
        if (oal_unlikely(sg_len + 1 > sg_max_len)) {
            oam_error_log2("{sg list over,sg_len:%u, sg_max_len:%u\n}", sg_len, sg_max_len);
            return -OAL_ENOMEM;
        }
        sg_set_buf(&sg[sg_len], hi_sdio->sdio_align_buff, align_len);
        sg_len++;
    }

    sg_mark_end(&sg[sg_len - 1]);
    rw_sz = align_t;

    oal_warn_on((rw_sz >= HISDIO_BLOCK_SIZE) && (rw_sz & (HISDIO_BLOCK_SIZE - 1)));
    oal_warn_on((rw_sz < HISDIO_BLOCK_SIZE)  && (rw_sz & (4 - 1))); /* 4: 4字节对齐 */

    if (oal_warn_on(align_len & 0x3)) {
        oam_warning_log1("{not 4 bytes align:%u\n}", align_len);
    }

    return _oal_sdio_transfer_scatt(hi_sdio, rw, addr, sg, sg_len, rw_sz);
}

hi_s32 oal_sdio_tranfer_sglist_memory(const oal_channel_stru *hi_sdio,
    const hcc_data_queue *head,
    hi_s32 rw)
{
    hi_s32 ret;
    hi_u32 queue_len;
    hi_u32 sum_len = 0;
    hi_u32 idx = 0;
    hcc_unc_struc *unc_buf = HI_NULL;
    hcc_unc_struc *tmp = HI_NULL;
    hi_u32 request_sg_len;
    struct scatterlist *sg = HI_NULL;
    struct sg_table sgtable = {0};
    hi_u8 sg_realloc = 0;

    queue_len = hcc_list_len(head);

    request_sg_len = queue_len + 1;
    if (oal_unlikely(request_sg_len > hi_sdio->scatt_info[rw].max_scatt_num)) {
        if (sg_alloc_table(&sgtable, request_sg_len, GFP_KERNEL)) {
            return -OAL_ENOMEM;
        }
        sg_realloc = 1;
        sg = sgtable.sgl;
    } else {
        sg = hi_sdio->scatt_info[rw].sglist;
    }

    memset_s(sg, sizeof(struct scatterlist) * request_sg_len, 0, sizeof(struct scatterlist) * request_sg_len);

    osal_list_for_each_entry_safe(unc_buf, tmp, (&head->data_queue), list) {
        if (!oal_is_aligned((uintptr_t)oal_unc_data(unc_buf), 4)) { /* 4: 4字节对齐 */
            return -OAL_EINVAL;
        }

        if (!oal_is_aligned(oal_unc_len(unc_buf), HIBUS_H2D_SCATT_BUFFLEN_ALIGN)) {
            return -OAL_EINVAL;
        }

        sg_set_buf(&sg[idx], oal_unc_data(unc_buf), oal_unc_len(unc_buf));
        sum_len += oal_unc_len(unc_buf);
        idx++;
    }

    if (oal_unlikely(idx > queue_len)) {
        return -OAL_EINVAL;
    }

    ret = oal_sdio_transfer_scatt(hi_sdio, rw, HISDIO_REG_FUNC1_FIFO, sg, idx, request_sg_len, sum_len);
    if (sg_realloc) {
        sg_free_table(&sgtable);
    }
    return ret;
}

hi_s32 oal_sdio_transfer_list(const oal_channel_stru *hi_sdio,
    const hcc_data_queue *head,
    hi_s32 rw)
{
    hi_s32 ret;

    if ((!hi_sdio) || (!head)) {
        oam_error_log0("hi_sdio head null\n");
        return -OAL_EINVAL;
    }

    if (WARN_ON(rw >= SDIO_OPT_BUTT)) {
        return -OAL_EINVAL;
    }

    if (WARN_ON(hcc_is_list_empty(head))) {
        return -OAL_EINVAL;
    }

    if (rw == SDIO_WRITE) {
        if (hi_sdio->bus_ops &&
            hi_sdio->bus_ops->pm_wakeup_dev) {
            if (hi_sdio->bus_ops->pm_wakeup_dev(hi_sdio->bus_data) != HI_SUCCESS) {
                oam_error_log0("{oal_sdio_transfer_netbuf_list::host wakeup device failed}");
                return -OAL_EBUSY;
            }
        }
    }

#ifdef _SCATTER_LIST_MEMORY
    ret = oal_sdio_tranfer_sglist_memory(hi_sdio, head, rw);
#else
    ret = oal_sdio_tranfer_commit_memory(hi_sdio, head, rw);
#endif
    return ret;
}

hi_s32 oal_sdio_build_rx_list(oal_channel_stru *hi_sdio, hcc_data_queue *head)
{
    hi_s32 i;
    hi_u8  buff_len;
    hi_u16 buff_len_t;
    hi_s32 ret = HI_SUCCESS;
    hi_u32 sum_len = 0;
    hcc_unc_struc *unc_buf = HI_NULL;

    if (!hcc_is_list_empty(head)) {
        oam_error_log0("oal_sdio_build_rx_list: oal netbuf list empty");
        return -OAL_EINVAL;
    }

    if ((hi_sdio == HI_NULL) || (hi_sdio->bus_ops == HI_NULL) ||
        (hi_sdio->bus_ops->alloc_unc_buf == HI_NULL)) {
        oam_error_log0("oal_sdio_build_rx_list: sdio initialization incomplete");
        return -OAL_EINVAL;
    }

    for (i = 0; i < HISDIO_EXTEND_REG_COUNT; i++) {
        buff_len = hi_sdio->sdio_extend->comm_reg[i];
        if (buff_len == 0) {
            break;
        }

        buff_len_t = buff_len << HISDIO_D2H_SCATT_BUFFLEN_ALIGN_BITS;

        unc_buf = hi_sdio->bus_ops->alloc_unc_buf(buff_len_t, NETBUF_STRU_TYPE);
        if (unc_buf ==  HI_NULL) {
            oam_error_log0("oal_sdio_build_rx_list: alloc_unc_buf fail");
            goto failed_unc_buf_alloc;
        }
        sum_len += buff_len_t;

        hcc_list_add_tail(head, unc_buf);
    }

    if (oal_warn_on(hisdio_align_4_or_blk(sum_len) != hi_sdio->sdio_extend->xfer_count)) {
        oam_warning_log3("{[WIFI][E]scatt total len[0x%x] should = xfercount[0x%x],after pad len:0x%x}",
            sum_len, hi_sdio->sdio_extend->xfer_count, hisdio_align_4_or_blk(sum_len));
        goto failed_unc_buf_alloc;
    }

    if (oal_unlikely(hcc_is_list_empty(head))) {
        return -OAL_EINVAL;
    }

    return ret;
failed_unc_buf_alloc:
    hcc_list_purge(head);
    ret = oal_sdio_transfer_rx_reserved_buff(hi_sdio);
    if (ret != HI_SUCCESS) {
        oam_error_log0("oal_sdio_transfer_rx_reserved_buff fail\n");
    }
    return -OAL_ENOMEM;
}

hi_s32 oal_sdio_dev_init(oal_channel_stru *hi_sdio)
{
    struct sdio_func   *func = HI_NULL;
    hi_s32               ret;

    if (hi_sdio == HI_NULL || hi_sdio->func == HI_NULL) {
        return -OAL_EFAIL;
    }

    func = hi_sdio->func;
    oal_sdio_claim_host(hi_sdio->func);
     /* 超时时间为1000  */
    sdio_en_timeout(func)  = 1000;

    ret = sdio_enable_func(func);
    if (ret < 0) {
        oam_error_log1("failed to enable sdio function! ret=%d\n", ret);
        goto failed_enabe_func;
    }

    ret = sdio_set_block_size(func, HISDIO_BLOCK_SIZE);
    if (ret) {
        oam_error_log1("failed to set sdio blk size! ret=%d\n", ret);
        goto failed_set_block_size;
    }

    oal_sdio_writeb(func, HISDIO_FUNC1_INT_MASK, HISDIO_REG_FUNC1_INT_STATUS, &ret);
    if (ret) {
        oam_error_log1("failed to clear sdio interrupt! ret=%d\n", ret);
        goto failed_clear_func1_int;
    }

    oal_sdio_writeb(func, HISDIO_FUNC1_INT_MASK, HISDIO_REG_FUNC1_INT_ENABLE, &ret);
    if (ret < 0) {
        oam_error_log1("failed to enable sdio interrupt! ret=%d\n", ret);
        goto failed_enable_func1;
    }

    oal_enable_sdio_state(hi_sdio, OAL_SDIO_ALL);
    oal_sdio_release_host(hi_sdio->func);

    return HI_SUCCESS;
failed_enable_func1:
failed_clear_func1_int:
failed_set_block_size:
    sdio_disable_func(func);
failed_enabe_func:
    oal_sdio_release_host(hi_sdio->func);
    return ret;
}

static hi_void oal_sdio_dev_deinit(oal_channel_stru *hi_sdio)
{
    struct sdio_func   *func = HI_NULL;
    hi_s32           ret = 0;
    func  = hi_sdio->func;
    sdio_claim_host(func);
    oal_sdio_writeb(func, 0, HISDIO_REG_FUNC1_INT_ENABLE, &ret);
    sdio_disable_func(func);
    oal_disable_sdio_state(hi_sdio, OAL_SDIO_ALL);
    sdio_release_host(func);
}

hi_void sdio_card_detect_change(hi_s32 sdio_dev_num)
{
#if 0
#if (_PRE_OS_PLATFORM == _PRE_PLATFORM_HISILICON)
    hisi_sdio_rescan(sdio_dev_num);
#elif (_PRE_OS_PLATFORM == _PRE_PLATFORM_JZ)
    jzmmc_manual_detect(sdio_dev_num, 1);
#endif
#else
	printk("call rockchip carddetect api\n");
	//rockchip_wifi_set_carddetect(0);
	//rockchip_wifi_power(0);
	//msleep(50);
	//rockchip_wifi_power(1);
	msleep(100);
	//rockchip_wifi_set_carddetect(1);
#endif
}

hi_s32 oal_sdio_detectcard_to_core(hi_s32 sdio_dev_num)
{
    sdio_card_detect_change(sdio_dev_num);
    return HI_SUCCESS;
}
static hi_void oal_sdio_remove(struct sdio_func *func)
{
    oal_channel_stru *hi_sdio = HI_NULL;
    if (func == HI_NULL) {
        oam_error_log0("[Error]oal_sdio_remove: Invalid NULL func!\n");
        return;
    }

    hi_sdio = (oal_channel_stru *)sdio_get_drvdata(func);
    if (hi_sdio == HI_NULL) {
        oam_error_log0("[Error]Invalid NULL hi_sdio!\n");
        return;
    }

    oal_sdio_dev_deinit(hi_sdio);
    sdio_set_drvdata(func, NULL);
    oam_error_log0("Hisilicon connectivity sdio driver has been removed.");
}

static hi_s32 oal_sdio_extend_buf_get(const oal_channel_stru *hi_sdio)
{
    hi_s32 ret;

    ret = oal_sdio_memcpy_fromio(hi_sdio->func, (hi_void *)hi_sdio->sdio_extend,
        HISDIO_EXTEND_BASE_ADDR, sizeof(hisdio_extend_func));
    if (ret != HI_SUCCESS) {
        oam_info_log0("{[SDIO][Err]sdio read extend_buf fail!}");
    }
    return ret;
}

static hi_s32 oal_sdio_get_func1_int_status(const oal_channel_stru *hi_sdio, hi_u8 *int_stat)
{
    hi_sdio->sdio_extend->int_stat &= hi_sdio->func1_int_mask;
    *int_stat = (hi_sdio->sdio_extend->int_stat & 0xF);
    return HI_SUCCESS;
}

static hi_s32 oal_sdio_msg_stat(const oal_channel_stru *hi_sdio, hi_u32 *msg)
{
    hi_s32 ret = 0;

    *msg = oal_sdio_readl(hi_sdio->func, HISDIO_REG_FUNC1_MSG_FROM_DEV, &ret);

    if (ret) {
        oam_info_log1("sdio readb error![ret=%d]\n", ret);
        return ret;
    }
    hi_sdio->sdio_extend->msg_stat = *msg;

    return HI_SUCCESS;
}

hi_s32 oal_sdio_msg_irq(oal_channel_stru *hi_sdio)
{
    hi_u32 bit;
    struct sdio_func    *func;
    hi_u32               msg = 0;
    hi_s32               ret;
    unsigned long        msg_tmp;

    func       = hi_sdio->func;
    /* reading interrupt form ARM Gerneral Purpose Register(0x28)  */
    ret = oal_sdio_msg_stat(hi_sdio, &msg);
    if (ret) {
        oam_info_log1("[SDIO][Err]oal_sdio_msg_stat error![ret=%d]\n", ret);
        return ret;
    }
    msg_tmp = (unsigned long)msg;

    if (!msg) {
        return HI_SUCCESS;
    }
    if (test_bit(D2H_MSG_DEVICE_PANIC, &msg_tmp)) {
        oal_disable_sdio_state(hi_sdio, OAL_SDIO_ALL);
    }
    oal_sdio_release_host(hi_sdio->func);
    if (test_and_clear_bit(D2H_MSG_DEVICE_PANIC, &msg_tmp)) {
        bit = D2H_MSG_DEVICE_PANIC;
        hi_sdio->msg[bit].count++;
        hi_sdio->msg[bit].cpu_time = cpu_clock(UINT_MAX);
        if (hi_sdio->msg[bit].msg_rx) {
            oam_info_log1("device panic msg come, 0x%8x\n", msg);
            hi_sdio->msg[bit].msg_rx(hi_sdio->msg[bit].data);
        }
    }
    bit = 0;
    for_each_set_bit(bit, (const unsigned long *)&msg_tmp, D2H_MSG_COUNT) {
        if (bit >= D2H_MSG_COUNT) {
            oam_info_log0("oal_sdio_msg_irq, bit >= D2H_MSG_COUNT\n");
            return -OAL_EFAIL;
        }
        hi_sdio->msg[bit].count++;
        hi_sdio->msg[bit].cpu_time = cpu_clock(UINT_MAX);
        if (hi_sdio->msg[bit].msg_rx) {
            hi_sdio->msg[bit].msg_rx(hi_sdio->msg[bit].data);
        }
    }
    oal_sdio_claim_host(hi_sdio->func);

    return HI_SUCCESS;
}

hi_s32 oal_sdio_get_credit(const oal_channel_stru *hi_sdio, hi_u32 *uc_hipriority_cnt)
{
    hi_s32 ret;
    sdio_claim_host(hi_sdio->func);
    ret = oal_sdio_memcpy_fromio(hi_sdio->func, (hi_u8 *)uc_hipriority_cnt,
                                 HISDIO_EXTEND_CREDIT_ADDR, sizeof(*uc_hipriority_cnt));
    sdio_release_host(hi_sdio->func);
    /* 此处要让出CPU */
    schedule();
    return ret;
}

static hi_s32 oal_sdio_xfercount_get(const oal_channel_stru *hi_sdio, hi_u32 *xfercount)
{
    hi_s32 ret = 0;
    /* read from 0x0c */
    *xfercount = oal_sdio_readl(hi_sdio->func, HISDIO_REG_FUNC1_XFER_COUNT, &ret);
    if (oal_unlikely(ret)) {
        oam_error_log1("[E]sdio read xercount failed ret=%d\n", ret);
        return ret;
    }
    hi_sdio->sdio_extend->xfer_count = *xfercount;
    return HI_SUCCESS;
}

hi_s32 oal_sdio_transfer_register(oal_channel_stru *hi_sdio, struct hcc_bus_ops *bus_ops)
{
    if (hi_sdio == HI_NULL) {
        return -OAL_EINVAL;
    }
    hi_sdio->bus_ops  = bus_ops;
    return HI_SUCCESS;
}

hi_void oal_sdio_transfer_unregister(oal_channel_stru *hi_sdio)
{
    hi_sdio->bus_ops = HI_NULL;
}

hi_s32 oal_sdio_message_register(oal_channel_stru *hi_sdio, hi_u8 msg, sdio_msg_rx cb, hi_void *data)
{
    if (hi_sdio == HI_NULL || msg >= D2H_MSG_COUNT) {
        return -OAL_EFAIL;
    }
    hi_sdio->msg[msg].msg_rx = cb;
    hi_sdio->msg[msg].data = data;
    return HI_SUCCESS;
}

hi_void oal_sdio_message_unregister(oal_channel_stru *hi_sdio, hi_u8 msg)
{
    if (hi_sdio == HI_NULL || msg >= D2H_MSG_COUNT) {
        return;
    }
    hi_sdio->msg[msg].msg_rx = HI_NULL;
    hi_sdio->msg[msg].data = HI_NULL;
}

hi_s32 oal_sdio_data_sg_irq(oal_channel_stru *hi_sdio)
{
    struct sdio_func   *func = HI_NULL;
    hi_s32 ret;
    hi_u32              xfer_count;

    if (hi_sdio == HI_NULL || hi_sdio->func == HI_NULL || hi_sdio->bus_data == HI_NULL) {
        return -OAL_EINVAL;
    }

    func = hi_sdio->func;
    ret = oal_sdio_xfercount_get(hi_sdio, &xfer_count);
    if (oal_unlikely(ret)) {
        return -OAL_EFAIL;
    }
    /* beacuse get buf may cost lot of time, so release bus first */
    if (hi_sdio->bus_ops == HI_NULL ||
        hi_sdio->bus_ops->rx == HI_NULL) {
        return -OAL_EINVAL;
    }

    oal_sdio_release_host(func);
    hi_sdio->bus_ops->rx(hi_sdio->bus_data);
    oal_sdio_claim_host(func);

    return HI_SUCCESS;
}

hi_s32 oal_sdio_do_isr(oal_channel_stru *hi_sdio)
{
    hi_u8                   int_mask;
    hi_s32                   ret;

    if (oal_unlikely(HI_TRUE != oal_sdio_get_state(hi_sdio, OAL_SDIO_RX))) {
        oam_error_log0("oal_sdio_do_isr rx disable");
        return HI_SUCCESS;
    }

    ret = oal_sdio_extend_buf_get(hi_sdio);
    if (oal_unlikely(ret)) {
        return -OAL_EFAIL;
    }

    ret = oal_sdio_get_func1_int_status(hi_sdio, &int_mask);
    if (oal_unlikely(ret)) {
        return ret;
    }

    if (oal_unlikely((int_mask & HISDIO_FUNC1_INT_MASK) == 0)) {
        oam_info_log0("no sdio isr");
        return HI_SUCCESS;
    }

    /* message interrupt, flow control */
    if (int_mask & HISDIO_FUNC1_INT_MFARM) {
        if (oal_sdio_msg_irq(hi_sdio) != HI_SUCCESS) {
            return -OAL_EFAIL;
        }
    }

    if (int_mask & HISDIO_FUNC1_INT_DREADY) {
        return oal_sdio_data_sg_irq(hi_sdio);
    }
    return HI_SUCCESS;
}

hi_void oal_sdio_isr(struct sdio_func *func)
{
    oal_channel_stru     *hi_sdio = HI_NULL;
    hi_s32                     ret;
    if (func == HI_NULL) {
        oam_error_log0("oal_sdio_isr func null\n");
        return;
    }

    hi_sdio = sdio_get_drvdata(func);
    if (hi_sdio == HI_NULL || hi_sdio->func == HI_NULL) {
        oam_error_log1("hi_sdio/hi_sdio->func is NULL :%p\n", (uintptr_t)hi_sdio);
        return;
    }

    oal_sdio_claim_host(hi_sdio->func);
    ret = oal_sdio_do_isr(hi_sdio);
    if (oal_unlikely(ret)) {
        oam_error_log0("oal_sdio_do_isr fail\n");
    }
    oal_sdio_release_host(hi_sdio->func);
}

static hi_s32 hcc_task_rx_gpio_thread(hi_void *data)
{
    oal_channel_stru *hi_sdio;
    hi_sdio = (oal_channel_stru *)data;
    oam_warning_log0("hcc_task_rx_gpio_thread start");
    allow_signal(SIGTERM);
    while (!down_interruptible(&hi_sdio->rx_sema)) {
        if (kthread_should_stop()) {
             oam_error_log0("exit gpio rx thread");
            break;
        }
        /* start to read GPIO interrupt */
        oal_sdio_isr(hi_sdio->func);
    }
    oam_info_log0("hcc_task_rx_gpio_thread is terminated");
    return HI_SUCCESS;
}

static hi_s32 sdio_start_gpio_thread(oal_channel_stru *hi_sdio)
{
    oal_kthread_param_stru thread_param = {0};
    memset_s(&thread_param, sizeof(oal_kthread_param_stru), 0, sizeof(oal_kthread_param_stru));
    thread_param.l_cpuid = 0;
    thread_param.l_policy = OAL_SCHED_FIFO;
    thread_param.l_prio = HCC_TASK_RX_ISR_PRIO;
    thread_param.ul_stacksize = HCC_TASK_RX_ISR_SIZE;

    hi_sdio->rx_handle = oal_kthread_create(HCC_TASK_RX_ISR_NAME,
        hcc_task_rx_gpio_thread, hi_sdio, &thread_param);
    if (hi_sdio->rx_handle == HI_NULL) {
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

static irqreturn_t oal_gpio_isr(int irq, void *data)
{
    oal_channel_stru* hi_sdio = (oal_channel_stru*)data;
    if (hi_sdio == NULL) {
        return IRQ_HANDLED;
    }
    /* 唤醒RX线程 */
    up(&hi_sdio->rx_sema);

    return IRQ_HANDLED;
}

int oal_sdio_interrupt_register(oal_channel_stru *hi_sdio)
{
    int ret;
    int irq;
#if 0
#if 0
#if(_PRE_OS_PLATFORM == _PRE_PLATFORM_JZ)
    jz_gpio_set_func(WLAN_GPIO_INT, GPIO_INT_RE);
#elif (_PRE_OS_PLATFORM == _PRE_PLATFORM_HISILICON)
    void __iomem *io_mux_base;
    io_mux_base = ioremap(0x112C0054, 0x04);
    gpio_reg_writel(io_mux_base, 0x1A00);
    iounmap(io_mux_base);
#endif
#else
	if(!gpio_is_valid(WLAN_GPIO_INT))
		printk("gpio%d is invalid! \n",WLAN_GPIO_INT);
	else
		printk("===> gpio%d is available! \n",WLAN_GPIO_INT);
#endif
    ret = gpio_request_one(WLAN_GPIO_INT, GPIOF_IN, PROC_NAME_GPIO_WLAN_WAKEUP_HOST);
    if (ret) {
        goto gpio_request_one_fail;
    }

    irq = gpio_to_irq(WLAN_GPIO_INT);
#else
    irq = rockchip_wifi_get_oob_irq();
#endif

    hi_sdio->ul_wlan_irq = irq;

    sema_init(&hi_sdio->rx_sema, 0);

    ret = request_irq(irq, oal_gpio_isr, IRQF_NO_SUSPEND |
        IRQF_TRIGGER_RISING, "wifi_gpio_intr", hi_sdio); /* | IRQF_DISABLED */
    if (ret < 0) {
        return ret;
    }

    disable_irq(irq);
    enable_irq(irq);
    return HI_SUCCESS;
#if 0
gpio_request_one_fail:
    return HI_FAILURE;
#endif
}

static void oal_sdio_interrupt_unregister(oal_channel_stru *hi_sdio)
{
    disable_irq_nosync(hi_sdio->ul_wlan_irq);
    free_irq(hi_sdio->ul_wlan_irq, hi_sdio);
#if 0
    gpio_free(WLAN_GPIO_INT);
#endif
    return;
}

static hi_s32 oal_sdio_probe(struct sdio_func *func, const struct sdio_device_id *ids)
{
    oal_channel_stru *hi_sdio = HI_NULL;
    int ret;

    if (func == HI_NULL || func->card == HI_NULL || func->card->host == HI_NULL || (!ids)) {
        oam_error_log0("oal_sdio_probe:func func->card->host ids null\n");
        return -OAL_EFAIL;
    }

    hi_sdio = oal_sdio_alloc(func);
    if (hi_sdio == HI_NULL) {
        oam_error_log0("failed to alloc hi_sdio!\n");
        goto failed_sdio_alloc;
    }

    /* 注册GPIO中断 */
    ret = oal_sdio_interrupt_register(hi_sdio);
    if (ret < 0) {
        goto failed_sdio_dev_init;
    }

    ret = sdio_start_gpio_thread(hi_sdio);
    if (ret < 0) {
        goto failed_sdio_start_thread_fail;
    }

    oal_disable_sdio_state(hi_sdio, OAL_SDIO_ALL);

    if (oal_sdio_dev_init(hi_sdio) != HI_SUCCESS) {
        oam_error_log0("sdio dev init failed!\n");
        goto failed_sdio_start_thread_fail;
    }

    oal_complete(&g_sdio_driver_complete);
    return HI_SUCCESS;
failed_sdio_start_thread_fail:
    oal_sdio_interrupt_unregister(hi_sdio);
failed_sdio_dev_init:
    oal_free(hi_sdio);
failed_sdio_alloc:
    return -OAL_EFAIL;
}

static const struct dev_pm_ops oal_sdio_pm_ops = {
    .suspend = oal_sdio_suspend,
    .resume = oal_sdio_resume,
};

static  struct sdio_driver oal_sdio_driver = {
    .name       = "oal_sdio",
    .id_table   = g_oal_sdio_ids,
    .probe      = oal_sdio_probe,
    .remove     = oal_sdio_remove,
    .drv        = {
        .owner      = THIS_MODULE,
        .pm         = &oal_sdio_pm_ops,
        .shutdown   = oal_sdio_dev_shutdown,
    }
};

hi_s32 oal_sdio_func_probe(oal_channel_stru *hi_sdio, hi_s32 sdio_dev_num)
{
    hi_s32 ret;
    hi_s32 times = SDIO_PROBLE_TIMES;
    if (hi_sdio == HI_NULL) {
        return -OAL_EFAIL;
    }
    oal_init_completion(&g_sdio_driver_complete);
    ret = oal_sdio_detectcard_to_core(sdio_dev_num);
    if (ret) {
        oam_error_log1("fail to detect sdio card, ret=%d\n", ret);
        goto failed_sdio_reg;
    }
    ret = sdio_register_driver(&oal_sdio_driver);
    if (ret) {
        oam_error_log1("register sdio driver Failed ret=%d\n", ret);
        goto failed_sdio_reg;
    }

    do {
        if (oal_wait_for_completion_timeout(&g_sdio_driver_complete, TIMEOUT_MUTIPLE_10 * HZ) != 0) {
            break;
        }

        ret = oal_sdio_detectcard_to_core(sdio_dev_num);
        if (ret) {
            oam_error_log1("fail to detect sdio card, ret=%d\n", ret);
            break;
        }
    } while (--times > 0);

    if (times <= 0) {
        oam_error_log0("hisi sdio load sucuess, sdio enum done.\n");
        goto failed_sdio_enum;
    }

    oal_sdio_claim_host(hi_sdio->func);
    oal_disable_sdio_state(hi_sdio, OAL_SDIO_ALL);
    oal_sdio_release_host(hi_sdio->func);
    return HI_SUCCESS;
failed_sdio_enum:
    sdio_unregister_driver(&oal_sdio_driver);
failed_sdio_reg:
    return -OAL_EFAIL;
}

hi_void oal_sdio_credit_info_init(oal_channel_stru *hi_sdio)
{
    hi_sdio->sdio_credit_info.large_free_cnt = 0;
    hi_sdio->sdio_credit_info.short_free_cnt = 0;
    oal_spin_lock_init(&hi_sdio->sdio_credit_info.credit_lock);
}

oal_channel_stru *oal_sdio_init_module(hcc_handler_stru *hcc_handler, struct hcc_bus_ops *bus_ops)
{
    oal_channel_stru *hi_sdio = (oal_channel_stru *)oal_memalloc(sizeof(oal_channel_stru));
    if (hi_sdio == HI_NULL) {
        return HI_NULL;
    }
    memset_s(hi_sdio, sizeof(oal_channel_stru), 0, sizeof(oal_channel_stru));

    hi_sdio->rx_buf = (hi_u8 *)oal_memalloc(SDIO_MAX_XFER_LEN);
    if (hi_sdio->rx_buf == HI_NULL) {
        goto alloc_sdio_struct_fail;
    }

    hi_sdio->tx_buf = (hi_u8 *)oal_memalloc(SDIO_MAX_XFER_LEN);
    if (hi_sdio->tx_buf == HI_NULL) {
        goto alloc_rx_buf_fail;
    }

    oal_sdio_credit_info_init(hi_sdio);

    hi_sdio->sdio_extend = (hisdio_extend_func *)oal_memalloc(sizeof(hisdio_extend_func));
    if (hi_sdio->sdio_extend == HI_NULL) {
        goto failed_sdio_extend_alloc;
    }
    memset_s(hi_sdio->sdio_extend, sizeof(hisdio_extend_func), 0, sizeof(hisdio_extend_func));

    hi_sdio->scatt_info[SDIO_READ].max_scatt_num = HISDIO_DEV2HOST_SCATT_MAX + 1;
    hi_sdio->scatt_info[SDIO_READ].sglist = oal_kzalloc(
        sizeof(struct scatterlist) * (HISDIO_DEV2HOST_SCATT_MAX + 1), OAL_GFP_KERNEL);
    if (hi_sdio->scatt_info[SDIO_READ].sglist == HI_NULL) {
        goto failed_sdio_read_sg_alloc;
    }

    hi_sdio->scatt_info[SDIO_WRITE].max_scatt_num = HISDIO_HOST2DEV_SCATT_MAX + 2; /* 2: 多预留2个 */
    hi_sdio->scatt_info[SDIO_WRITE].sglist = oal_kzalloc(
        sizeof(struct scatterlist) * (hi_sdio->scatt_info[SDIO_WRITE].max_scatt_num), OAL_GFP_KERNEL);
    if (hi_sdio->scatt_info[SDIO_WRITE].sglist == HI_NULL) {
        goto failed_sdio_write_sg_alloc;
    }

    hi_sdio->sdio_align_buff = oal_kzalloc(HISDIO_BLOCK_SIZE, OAL_GFP_KERNEL);
    if (hi_sdio->sdio_align_buff == HI_NULL) {
        goto failed_sdio_align_buff_fail;
    }

    hi_sdio->func1_int_mask = HISDIO_FUNC1_INT_MASK;
    hi_sdio->bus_data = (hi_void*)hcc_handler;

    if (oal_sdio_transfer_register(hi_sdio, bus_ops) != HI_SUCCESS) {
        goto failed_sdio_align_buff_fail;
    }

    g_hi_sdio_ = hi_sdio;

    return hi_sdio;
failed_sdio_align_buff_fail:
    oal_free(hi_sdio->scatt_info[SDIO_WRITE].sglist);
    hi_sdio->scatt_info[SDIO_WRITE].sglist = HI_NULL;
    hi_sdio->scatt_info[SDIO_WRITE].max_scatt_num = 0;
failed_sdio_write_sg_alloc:
    oal_free(hi_sdio->scatt_info[SDIO_READ].sglist);
    hi_sdio->scatt_info[SDIO_READ].sglist = HI_NULL;
    hi_sdio->scatt_info[SDIO_READ].max_scatt_num = 0;
failed_sdio_read_sg_alloc:
    oal_free(hi_sdio->sdio_extend);
    hi_sdio->sdio_extend = HI_NULL;
failed_sdio_extend_alloc:
    oal_free(hi_sdio->tx_buf);
    hi_sdio->tx_buf = HI_NULL;
alloc_rx_buf_fail:
    oal_free(hi_sdio->rx_buf);
    hi_sdio->rx_buf = HI_NULL;
alloc_sdio_struct_fail:
    oal_free(hi_sdio);
    return HI_NULL;
}


hi_void  oal_sdio_exit_module(oal_channel_stru *hi_sdio)
{
    oal_free(hi_sdio->tx_buf);
    oal_free(hi_sdio->rx_buf);
    oal_free(hi_sdio->sdio_extend);
    oal_sdio_transfer_unregister(hi_sdio);
    oal_free(hi_sdio);
    g_hi_sdio_ = HI_NULL;
}

/* hi_s32 val卡选择 */
hi_s32 oal_sdio_init(void *data, hi_s32 sdio_dev_num, struct hcc_bus_ops *bus_ops)
{
    oal_channel_stru *hi_sdio;
    hi_s32 ret;
    hcc_handler_stru *hcc_handler = (hcc_handler_stru *)data;

    hi_sdio = oal_sdio_init_module(hcc_handler, bus_ops);
    if (hi_sdio == NULL) {
        oam_error_log0("oal_sdio_init_module: failed");
        return HI_FAILURE;
    }

    ret = oal_sdio_func_probe(hi_sdio, sdio_dev_num);
    if (ret != HI_SUCCESS) {
        oam_error_log1("regeste_sdio_drv:: oal_sdio_func_probe failed", ret);
        goto oal_sdio_func_probe_fail;
    }

    oal_enable_sdio_state(hi_sdio, OAL_SDIO_ALL);

    hcc_handler->hi_channel = (hi_void*)hi_sdio;

    return HI_SUCCESS;
oal_sdio_func_probe_fail:
    oal_free(hi_sdio);
    return HI_FAILURE;
}

hi_void oal_sdio_exit(oal_channel_stru *hi_sdio)
{
    oal_disable_sdio_state(hi_sdio, OAL_SDIO_ALL);
    oal_sdio_interrupt_unregister(hi_sdio);
    oal_kthread_stop(hi_sdio->rx_handle);
    up(&hi_sdio->rx_sema);
    sdio_unregister_driver(&oal_sdio_driver);
    oal_sdio_exit_module(hi_sdio);
}

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif

