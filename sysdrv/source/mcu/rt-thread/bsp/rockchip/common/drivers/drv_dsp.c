/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_dsp.c
  * @author  Huaping Liao
  * @version V0.1
  * @date    24-Jan-2019
  * @brief   dsp driver
  *
  ******************************************************************************
  */

/** @addtogroup RKBSP_Driver_Reference
 *  @{
 */

/** @addtogroup DSP
 *  @{
 */

/** @defgroup DSP_How_To_Use How To Use
 *  @{

 The DSP driver use to boot dsp and perform algorithm task, it can be used in
 the following three scenarios:

- (A) DSP driver boot and manage the dsp core:
    - 1) download dsp firmware;
    - 2) boot dsp core;
    - 3) suspend and resume;
- (B) DSP driver send algotithm task to dsp core and retrieval the result

 @} */
#include <rtdevice.h>
#include <rtthread.h>
#include <rthw.h>

#ifdef RT_USING_DSP
#include "board.h"
#include "drv_dsp.h"
#include "drv_clock.h"
#include "drv_dvfs.h"
#include "drv_heap.h"
#include "drv_snor.h"
#include "dma.h"
#include "dsp.h"
#include "hal_base.h"

#ifdef RT_USING_FWANALYSIS
#include "rkpart.h"
#endif

#if defined(RT_USING_DSPFW_LOADER)
#if defined(RT_USING_DSPFW_BUILTIN)
extern const uint8_t dsp_fw[];
#else
#include <dfs_posix.h>
#endif
#endif

/********************* Private MACRO Definition ******************************/

#define DSP_DEBUG     0
#define DSP_TAG       "rkdsp"
#if DSP_DEBUG

#define dsp_dbg(...) \
    do { \
        rt_kprintf("%s:%s: ", DSP_TAG, __func__);    \
        rt_kprintf(__VA_ARGS__);                     \
    } while(0)
#define rk_dsp_func_enter(...) dsp_dbg("enter\n");
#define rk_dsp_func_exit(...)  dsp_dbg("exit\n");
#else
#define dsp_dbg(...)
#define rk_dsp_func_enter(...)
#define rk_dsp_func_exit(...)
#endif // DSP_DEBUG

#define dsp_err(...) \
     do { \
        rt_kprintf("%s:%s: ", DSP_TAG, __func__);    \
        rt_kprintf(__VA_ARGS__);                     \
    } while(0)

#define MAX_ASSERT_LOG_SIZE           128

#define WORK_QUEUE_CNT                4
#define WORK_QUEUE_STACK_SIZE         1024
#define WORK_QUEUE_PRIORITY           10

/* RKDSP work os-mailbox config */
#define WORK_MB_SIZE                  10
#define WORK_MB_READY_VALUE           0xffffaaaa

#ifdef RT_USING_DSP_TRACE
/* RKDSP trace */
#define DSP_TRACE_BUFFER_SIZE         (1 * 1024)
#define DSP_TRACE_SLOT_SIZE           (128)
#define DSP_TRACE_SLOT_COUNT          (DSP_TRACE_BUFFER_SIZE / DSP_TRACE_SLOT_SIZE)
#endif

/********************* Private Structure Definition **************************/

enum dsp_status
{
    DSP_OFF      = 0,
    DSP_ON,
    DSP_SLEEP,
    DSP_SHUTDOWN,
    DSP_WAITING,
    DSP_READY,
};

struct dsp_cfg_param
{
    uint32_t type;
    uint32_t trace_buffer;
    uint32_t trace_buffer_size;
    uint32_t trace_slot_size;
    uint32_t heap;
    uint32_t heap_size;
    int32_t  uart;
    uint32_t reserve[5];
};

struct mbox_args
{
    eMBOX_CH chan;
    uint32_t cmd;
    uint32_t data;
    uint32_t userdata;
};

#ifdef RT_USING_DSPFW_LOADER

#define DSP_FIRMWARE_NAME             "rkdsp.bin"
#define DSP_IMAGE_NAME_SIZE           32
#define DSP_IMAGE_MAX_SECTION         32
#define FIRMWARE_MAGIC_SIZE           16
#define FIRMWARE_VERSION_SIZE         16
#define FIRMWARE_RESERVE_SIZE         60
#define FIRMWARE_MAX_IMAGES           16
#define FIRMWARE_READ_LEN             (1024 * 8)

#define ITCM_SIZE                     (DSP_ITCM_END - DSP_ITCM_BASE + 1)
#define DTCM_SIZE                     (DSP_DTCM_END - DSP_DTCM_BASE + 1)

struct dsp_sec_hdr
{
    uint32_t id;
    uint32_t size;
    uint32_t load_addr;
};

struct dsp_loader
{
#if defined(RT_USING_DSPFW_BUILTIN)
    void *fw_data;
    void *fw_off;
#else
    int fw_fd;
#endif

    /* Reserved dsp external text memory */
    uint8_t *ext_text;
};

struct dsp_img_hdr
{
    uint32_t id;
    char name[DSP_IMAGE_NAME_SIZE];
    uint32_t section_count;
};

struct dsp_fw_hdr
{
    char magic[FIRMWARE_MAGIC_SIZE];
    char version[FIRMWARE_VERSION_SIZE];
    uint32_t image_count;
    uint32_t image_size[FIRMWARE_MAX_IMAGES];
    uint8_t reserve[FIRMWARE_RESERVE_SIZE];
};
#endif // #ifdef RT_USING_DSPFW_LOADER

struct rk_dsp
{
    struct DSP_DEV dsp_dev;
    struct rt_dsp_device parent;
    volatile enum dsp_status status;
    volatile enum dsp_status last_status;

    /* Interrupt workqueue */
    struct rt_workqueue *isr_workqueue;
    /* Work mailbox */
    rt_mailbox_t work_mb;

    struct rt_work isr_work[WORK_QUEUE_CNT];
    struct mbox_args isr_mbox[WORK_QUEUE_CNT];

    volatile uint8_t work_cnt;
    uint8_t work_index;
    rt_isr_handler_t error_isr;

#ifdef RT_USING_DSP_TRACE
    /* Trace */
    char *trace_buffer;
    uint32_t trace_index;
#endif
#ifdef RT_USING_DSPFW_LOADER
    struct dsp_loader loader;
#endif
};

/********************* Private Variable Definition ***************************/

static struct rk_dsp rk_dsp_dev;

static void rk_dsp_mbox_callback(struct MBOX_CMD_DAT *msg, void *arg);
static void dsp_mbox_isr(int irq, void *param);
static struct MBOX_CLIENT mbox_client[MBOX_CHAN_CNT] =
{
    {"mbox0_chan0", NUM_INTERRUPTS, rk_dsp_mbox_callback, (void *)MBOX_CH_0},
    {"mbox0_chan1", NUM_INTERRUPTS, rk_dsp_mbox_callback, (void *)MBOX_CH_1},
    {"mbox0_chan2", NUM_INTERRUPTS, rk_dsp_mbox_callback, (void *)MBOX_CH_2},
    {"mbox0_chan3", NUM_INTERRUPTS, rk_dsp_mbox_callback, (void *)MBOX_CH_3},
};

#define DSP_MBOX_ISR_FUNC(ID) \
static void dsp_mbox_isr##ID(int irq, void *param) \
{ \
    dsp_mbox_isr(mbox_client[(int)ID].irq,param);      \
}

DSP_MBOX_ISR_FUNC(0)
DSP_MBOX_ISR_FUNC(1)
DSP_MBOX_ISR_FUNC(2)
DSP_MBOX_ISR_FUNC(3)

static const rt_isr_handler_t dsp_mbox_isr_func[MBOX_CHAN_CNT] =
{
    dsp_mbox_isr0,
    dsp_mbox_isr1,
    dsp_mbox_isr2,
    dsp_mbox_isr3
};

static void dsp_mbox_int_install(int id)
{
    rt_hw_interrupt_install(mbox_client[id].irq, dsp_mbox_isr_func[id], RT_NULL, RT_NULL);
    rt_hw_interrupt_umask(mbox_client[id].irq);
}


/********************* Private Function Definition ***************************/
/** @defgroup DSP_Private_Function Private Function
 *  @{
 */

static rt_err_t dsp_mbox_send_msg(struct MBOX_REG *mbox, eMBOX_CH chan,
                                  uint32_t cmd, struct dsp_work *work);
static rt_err_t dsp_mbox_send_data(struct MBOX_REG *mbox, eMBOX_CH chan,
                                   uint32_t cmd, uint32_t data);

#ifdef RT_USING_DSPFW_LOADER

/**
 * @brief  Access itcm must by word. If have't cache, must use use this function.
 * @param  src: Source data addr.
 * @param  dst: Destination tcm addr.
 * @param  len: Data word len.
 */
static void rk_dsp_tcm_memcpy(uint32_t *dst, uint32_t *src, int len)
{
    while (len--)
        *dst++ = *src++;
}

/**
 * @brief  create fw/img/sec head by len, if define xip, will use fw_data,
 *      if not, malloc a buffer and read head from file.
 * @param  loader: loader point.
 * @param  len: head lenght.
 * @return head point
 */
static void *rk_dsp_head_create(struct dsp_loader *loader, int len)
{
    void *head;
#if defined(RT_USING_DSPFW_BUILTIN)
    head = loader->fw_off;
    loader->fw_off += len;
#else
    head = rt_dma_malloc(len);
    RT_ASSERT(head);
    RT_ASSERT(read(loader->fw_fd, head, len) == len);
#endif
    return head;
}

/**
 * @brief  destory head if not define xip.
 * @param  head: head point.
 */
static void rk_dsp_head_destory(void *head)
{
#if !defined(RT_USING_DSPFW_BUILTIN)
    if (head)
        rt_dma_free(head);
#endif
}

/**
 * @brief  loader section data to tcm.
 * @param  loader: loader point.
 * @param  sec: section point.
 * @return rt_err_t
 */
static rt_err_t rk_dsp_sec_loader(struct dsp_loader *loader, struct dsp_sec_hdr *sec)
{
#if defined(RT_USING_DSPFW_BUILTIN)
    rk_dsp_tcm_memcpy((uint32_t *)sec->load_addr, (uint32_t *)loader->fw_off, sec->size / 4);
    dsp_dbg("Copy now src = 0x%08x, dst=0x%08x.\n", loader->fw_off, sec->load_addr);
    loader->fw_off += sec->size;
#else
    void *rd_buf;
    int len, off;

    len = sec->size > FIRMWARE_READ_LEN ? FIRMWARE_READ_LEN : sec->size;
    rd_buf = rt_malloc(len);
    RT_ASSERT(rd_buf);

    for (off = 0; off < sec->size; off += FIRMWARE_READ_LEN)
    {
        len = sec->size - off;
        if (len > FIRMWARE_READ_LEN)     /* Limit read size to avoid unmemory */
            len = FIRMWARE_READ_LEN;
        dsp_dbg("Read file, len = %d, off = %d, off = %d\n",
                len, off, lseek(loader->fw_fd, 0, SEEK_CUR));
        if (read(loader->fw_fd, rd_buf, len) != len)
        {
            dsp_err("Read fail, fd = %d, len = %d", loader->fw_fd, len);
            break;
        }
        rk_dsp_tcm_memcpy((uint32_t *)(sec->load_addr + off), (uint32_t *)rd_buf, len / 4);
    }
    rt_free(rd_buf);
#endif
    return RT_EOK;
}

/**
 * @brief  Parse section and download to dsp tcm.
 * @param  loader: loader point.
 * @param  image_size: image data length.
 * @return rt_err_t
 */

/**
 * @xSramDataBase: sram start address mapped dcache address space
 * @xSramSize: sram size
 * These two variables are defined in linker script,
 * dsp loader need these info to check if address
 * valid when dsp firmware need to use sram
 */
extern uint8_t xSramDataBase[];
extern uint8_t xSramSize[];
static struct dsp_sec_hdr sram_first_sec;
static struct dsp_sec_hdr sram_final_sec;
static rt_err_t rk_dsp_image_parse(struct dsp_loader *loader, uint32_t image_size)
{
    int idx;
    rt_err_t ret = RT_EOK;
    uint32_t count = 0;
    struct dsp_img_hdr *img_hdr;
    struct dsp_sec_hdr *sec;
    uint32_t sram_date_end = (uint32_t)xSramDataBase + (uint32_t)xSramSize;

    img_hdr = rk_dsp_head_create(loader, sizeof(struct dsp_img_hdr));
    dsp_dbg("parse image, id=%d, name=%s, count=%d\n",
            img_hdr->id, img_hdr->name, img_hdr->section_count);

    sram_first_sec.id = 0;
    sram_first_sec.size = 0;
    sram_first_sec.load_addr = sram_date_end;
    sram_final_sec.id = 0;
    sram_final_sec.size = 0;
    sram_final_sec.load_addr = 0;

    count = sizeof(struct dsp_img_hdr);
    for (idx = 0; idx < img_hdr->section_count; idx++)
    {
        sec = rk_dsp_head_create(loader, sizeof(struct dsp_sec_hdr));
        count += sizeof(struct dsp_sec_hdr);
        if (!sec->size)
            continue;
        dsp_dbg("Flash section, id=%d, size=%d, dst=0x%08x.\n",
                sec->id, sec->size, sec->load_addr);
        if (sec->load_addr >= RK_SRAM_END && sec->load_addr < sram_date_end)
        {
            if (sec->load_addr < sram_first_sec.load_addr)
                sram_first_sec.load_addr = sec->load_addr;
            if (sec->load_addr > sram_final_sec.load_addr)
            {
                sram_final_sec.load_addr = sec->load_addr;
                sram_final_sec.size = sec->size;
            }
        }
        rk_dsp_sec_loader(loader, sec);
        count += sec->size;
        rk_dsp_head_destory(sec);
    }

    if (count != image_size)
    {
        dsp_err("parse image failed, name=%s, offset=%d\n", img_hdr->name, count);
        ret = -RT_ERROR;
        goto out;
    }

out:
    rk_dsp_head_destory(img_hdr);
    return ret;
}

/**
 * @brief  Parse firmware data and get image.
 * @param  loader: loader point.
 * @return rt_err_t
 */
static rt_err_t rk_dsp_prepare_image(struct dsp_loader *loader)
{
    rt_err_t ret = RT_EOK;
    uint32_t image_size;
    struct dsp_fw_hdr *fw_hdr = NULL;
    const char *magic = "#RKCPDSPFW#";

    fw_hdr = rk_dsp_head_create(loader, sizeof(struct dsp_fw_hdr));
    /* Check firmware magic, must be #RKCPDSPFW# */
    if (rt_memcmp(fw_hdr, magic, strlen(magic)))
    {
        dsp_err("Invalid DSP firmware\n");
        ret = -RT_ERROR;
        goto out;
    }
    dsp_dbg("firmware: magic:%s, version:%s, count:%d\n",
            fw_hdr->magic, fw_hdr->version,
            fw_hdr->image_count);

    for (int i = 0; i < fw_hdr->image_count; i++)
    {
        image_size = fw_hdr->image_size[i];
        ret = rk_dsp_image_parse(loader, image_size);
    }

out:
    rk_dsp_head_destory(fw_hdr);
    return ret;
}

/**
 * @brief  Init firmware data if define xip, get data from dsp_fw,
 *      if not, open firmware file.
 * @param  loader: loader point.
 * @return rt_err_t
 */
static rt_err_t rk_dsp_fw_init(struct dsp_loader *loader)
{
    rt_err_t ret = RT_EOK;
#if defined(RT_USING_DSPFW_BUILTIN)
    loader->fw_data = (void *)dsp_fw;
    loader->fw_off = loader->fw_data;
#elif defined(RT_PATH_DSPFW)
    loader->fw_fd = open(RT_PATH_DSPFW, O_RDONLY, 0);
    if (loader->fw_fd < 0)
        ret = -RT_EFULL;
    dsp_dbg("open fw:%s, fd = %d\n", RT_PATH_DSPFW, loader->fw_fd);
#else
    ret = -RT_ERROR;
#endif
    return ret;
}

/**
 * @brief  Deinit firmware data if define xip, set fw_data to 0,
 *      if not, close firmware file.
 * @param  loader: loader point.
 * @return rt_err_t
 */
static rt_err_t rk_dsp_fw_deinit(struct dsp_loader *loader)
{
    rt_err_t ret = RT_EOK;
#if defined(RT_USING_DSPFW_BUILTIN)
    loader->fw_data = 0;
    loader->fw_off = 0;
#elif defined(RT_PATH_DSPFW)
    close(loader->fw_fd);
    loader->fw_fd = -1;
    dsp_dbg("open fw:%s, fd = %d\n", RT_PATH_DSPFW, loader->fw_fd);
#else
    ret = -RT_ERROR;
#endif
    return ret;
}

/**
 * @brief  Get dsp firmware and download to tcm.
 * @param  loader: loader point.
 * @return rt_err_t
 */
static rt_err_t rk_dsp_loader_image(struct dsp_loader *loader)
{
    rt_err_t ret = RT_EOK;

    ret = rk_dsp_fw_init(loader);
    RT_ASSERT(!ret);

    ret = rk_dsp_prepare_image(loader);
    RT_ASSERT(!ret);

    /* Flush itcm and dtcm data */
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)DSP_ITCM_BASE, ITCM_SIZE);
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)DSP_DTCM_BASE, DTCM_SIZE);
    if (sram_final_sec.size)
    {
        dsp_dbg("SRAM section start 0x%x final 0x%x final size %ld\n",
                sram_first_sec.load_addr,
                sram_final_sec.load_addr,
                sram_final_sec.size);
        uint32_t total_size =
            RT_ALIGN(sram_final_sec.load_addr - sram_first_sec.load_addr + sram_final_sec.size,
                     CACHE_LINE_SIZE);
        rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)sram_first_sec.load_addr, total_size);
    }
    ret = rk_dsp_fw_deinit(loader);
    RT_ASSERT(!ret);

    return ret;
}
#endif // #ifdef RT_USING_DSPFW_LOADER

/**
 * @brief  Clean or invalidate work and param buffer.
 * @param  work: dsp_work buffer point.
 * @param  ops: RT_HW_CACHE_FLUSH, RT_HW_CACHE_INVALIDATE.
 */
static void rk_dsp_sync_cache(struct dsp_work *work, enum RT_HW_CACHE_OPS ops)
{
    if (work->param_size && work->param)
        rt_hw_cpu_dcache_ops(ops, (void *)work->param, work->param_size);
    rt_hw_cpu_dcache_ops(ops, work, sizeof(struct dsp_work));
}

#ifdef RT_USING_DSP_TRACE
/**
 * @brief  Print dsp trace buffer what dsp print in trace buffer
 * @param  rkdsp: dsp device point.
 * @param  index: trace index, transfer from dsp.
 * @return int
 */
static int rk_dsp_trace(struct rk_dsp *rkdsp, uint32_t index)
{
    int ret = 0;
    uint32_t trace_end = index;
    uint32_t trace_start = rkdsp->trace_index;

    if (trace_end < trace_start ||
            trace_end - trace_start > DSP_TRACE_SLOT_COUNT)
    {
        dsp_err("trace slot overflow, start=%d, end=%d\n",
                trace_start, trace_end);
        ret = -EFAULT;
        goto out;
    }

    while (trace_start <= trace_end)
    {
        int slot_index = trace_start % DSP_TRACE_SLOT_COUNT;
        char *trace_slot = rkdsp->trace_buffer +
                           DSP_TRACE_SLOT_SIZE * slot_index;
        rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, trace_slot,
                             DSP_TRACE_SLOT_SIZE);
        if (*trace_slot)
        {
            *(trace_slot + DSP_TRACE_SLOT_SIZE - 1) = 0;
            dsp_err("##dsp##: %s", trace_slot);
        }

        trace_start++;
    }
    rkdsp->trace_index = trace_start;

out:
    return ret;
}
#endif

/**
 * @brief  Manage dsp configdone cmd and destory config buffer.
 * @param  rkdsp: dsp device point.
 * @param  data: work point.
 * @return rt_err_t
 */
static rt_err_t rk_dsp_workdone(struct rk_dsp *rkdsp, uint32_t data)
{
    rk_dsp_sync_cache((struct dsp_work *)data, RT_HW_CACHE_INVALIDATE);
    return rt_mb_send(rkdsp->work_mb, data);
}

/**
 * @brief  Manage dsp configdone cmd and destory config buffer.
 * @param  rkdsp: dsp device point.
 * @param  data: work point.
 * @return rt_err_t
 */
static rt_err_t rk_dsp_configdone(struct rk_dsp *rkdsp, uint32_t data)
{
    struct dsp_work *work = (struct dsp_work *)data;

    rk_dsp_sync_cache(work, RT_HW_CACHE_INVALIDATE);
    dsp_dbg("configdone: result = 0x%08x, rate = %d\n", work->result, work->rate);
    rkdsp_free((void *)work->param);
    rk_dsp_work_destroy(work);
    return RT_EOK;
}

/**
 * @brief  Config dsp trace and other param.
 * @param  rkdsp: dsp device point.
 * @param  data: dsp transfer when send ready cmd.
 * @return rt_err_t
 */
static rt_err_t rk_dsp_config(struct rk_dsp *rkdsp, uint32_t data)
{
    rt_err_t ret = -RT_ERROR;
    struct dsp_work *work = RT_NULL;
    struct dsp_cfg_param *cfg = RT_NULL;
    uint32_t params_size = sizeof(struct dsp_cfg_param);

    dsp_dbg("dsp ready, now to config. data = %x\n", data);
    work = rk_dsp_work_create(RKDSP_CFG_WORK);
    if (!work)
        goto err;
    cfg = rkdsp_malloc(params_size);
    if (!cfg)
    {
        ret = -RT_ENOMEM;
        goto err;
    }
    rt_memset(cfg, 0, params_size);
#ifdef RT_USING_DSP_TRACE
    cfg->trace_slot_size = DSP_TRACE_SLOT_SIZE;
    cfg->trace_buffer = (uint32_t)rkdsp_malloc(DSP_TRACE_BUFFER_SIZE);
    if (cfg->trace_buffer)
    {
        rt_memset((void *)cfg->trace_buffer, 0, DSP_TRACE_BUFFER_SIZE);
        cfg->trace_buffer_size = DSP_TRACE_BUFFER_SIZE;
        rkdsp->trace_index = 0;
        rkdsp->trace_buffer = (char *)cfg->trace_buffer;

        rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)cfg->trace_buffer,
                             DSP_TRACE_BUFFER_SIZE);
    }
    else
    {
        cfg->trace_buffer_size = 0;
    }
#endif

#if defined(RT_DSP_UART_PORT)
    cfg->uart = RT_DSP_UART_PORT;
#else
    cfg->uart = -1;     /* Don't need set, use default port */
#endif
    work->param = (uint32_t)cfg;
    work->param_size = params_size;
    return dsp_mbox_send_msg(rkdsp->dsp_dev.mboxReg, MBOX_CH_1,
                             DSP_CMD_CONFIG, work);
err:
    if (cfg)
        rkdsp_free(cfg);
    rk_dsp_work_destroy(work);
    dsp_err("dsp config fail, error = %d\n", ret);
    return ret;
}

/**
 * @brief  Call by rk_dsp_mbox_callback to manage dsp msg.
 * @param  work: work point.
 * @param  work_data: work info.
 */
static void rk_dsp_work(struct rt_work *work, void *work_data)
{
    rk_dsp_func_enter();

    rt_base_t level;
    struct rk_dsp *rkdsp;
    struct mbox_args mbox_data;

    RT_ASSERT(work_data);
    level = rt_hw_interrupt_disable();
    memcpy(&mbox_data, work_data, sizeof(struct mbox_args));
    rkdsp = (struct rk_dsp *)mbox_data.userdata;
    rkdsp->work_cnt--;
    rt_hw_interrupt_enable(level);

    switch (mbox_data.chan)
    {
    case MBOX_CH_0:
        if (mbox_data.cmd == DSP_CMD_WORK_DONE)
            rk_dsp_workdone(rkdsp, mbox_data.data);
        break;
    case MBOX_CH_1:
        if (mbox_data.cmd == DSP_CMD_READY)
        {
            rk_dsp_config(rkdsp, mbox_data.data);
        }
        else if (mbox_data.cmd == DSP_CMD_CONFIG_DONE)
        {
            rk_dsp_configdone(rkdsp, mbox_data.data);
            /* User waiting dsp ready, need notify */
            if (rkdsp->status == DSP_WAITING)
                rt_mb_send(rkdsp->work_mb, WORK_MB_READY_VALUE);
            rkdsp->status = DSP_READY;
        }
        else if (mbox_data.cmd == DSP_CMD_ASSERT)
        {
            dsp_err("HiFi3 assertion failed\n");
            if (mbox_data.data)
            {
                rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE,
                                     (void *)mbox_data.data,
                                     MAX_ASSERT_LOG_SIZE);
                dsp_err((char *)mbox_data.data);
            }
        }
        break;
    case MBOX_CH_2:
        if (mbox_data.cmd == DSP_CMD_STARTUP)
        {
            // todo
        }
        dsp_mbox_send_data(rkdsp->dsp_dev.mboxReg, mbox_data.chan,
                           mbox_data.cmd, mbox_data.data);
        break;
    case MBOX_CH_3:
#ifdef RT_USING_DSP_TRACE
        if (mbox_data.cmd == DSP_CMD_TRACE)
        {
            rk_dsp_trace(rkdsp, mbox_data.data);
            dsp_mbox_send_data(rkdsp->dsp_dev.mboxReg, mbox_data.chan,
                               DSP_CMD_TRACE_DONE, rkdsp->trace_index);
        }
#endif
        break;
    default:
        break;
    }

    rk_dsp_func_exit();
}

/**
 * @brief  Send cmd and data to dsp by mbox.
 * @param  mbox: Mbox register base.
 * @param  chan: mbox chan.
 * @param  cmd: msg command.
 * @param  data: msg data.
 * @return rt_err_t
 */
static rt_err_t __unused dsp_mbox_send_data(struct MBOX_REG *mbox,
        eMBOX_CH chan,
        uint32_t cmd,
        uint32_t data)
{
    struct MBOX_CMD_DAT cmdData;

    cmdData.CMD  = cmd;
    cmdData.DATA = data;
    return HAL_MBOX_SendMsg(mbox, chan, &cmdData);
}

/**
 * @brief  Send cmd and work to dsp by mbox.
 * @param  mbox: Mbox register base.
 * @param  chan: mbox chan.
 * @param  cmd: msg command.
 * @param  work: dsp work point.
 * @return rt_err_t
 */
static rt_err_t dsp_mbox_send_msg(struct MBOX_REG *mbox, eMBOX_CH chan,
                                  uint32_t cmd, struct dsp_work *work)
{
    struct MBOX_CMD_DAT cmdData;

    cmdData.CMD  = cmd;
    cmdData.DATA = (uint32_t)work;
    rk_dsp_sync_cache(work, RT_HW_CACHE_FLUSH);
    return HAL_MBOX_SendMsg(mbox, chan, &cmdData);
}

/**
 * @brief  Mbox interrupt function callback.
 * @param  msg: mbox msg, include cmd, chan and data.
 * @param  arg: callback function argument, register in mbox_client.
 */
static void rk_dsp_mbox_callback(struct MBOX_CMD_DAT *msg, void *arg)
{
    rt_base_t level;
    struct rt_work *work = NULL;
    struct mbox_args *mbox_data;
    struct rk_dsp *rkdsp = &rk_dsp_dev;

    level = rt_hw_interrupt_disable();
    if (rkdsp->work_cnt >= WORK_QUEUE_CNT)
    {
        rt_hw_interrupt_enable(level);
        dsp_err("work use is full\n");
        return;
    }
    rkdsp->work_cnt++;
    rkdsp->work_index++;
    if (rkdsp->work_index >= WORK_QUEUE_CNT)
        rkdsp->work_index = 0;
    work = &rkdsp->isr_work[rkdsp->work_index];
    mbox_data = &rkdsp->isr_mbox[rkdsp->work_index];

    mbox_data->cmd  = msg->CMD;
    mbox_data->data = msg->DATA;
    mbox_data->chan = (eMBOX_CH)arg;
    mbox_data->userdata = (uint32_t)rkdsp;
    rt_hw_interrupt_enable(level);

    dsp_dbg("received mbox data, chan=%d, cmd=0x%08x, data=0x%08x\n",
            mbox_data->chan, mbox_data->cmd, mbox_data->data);

    rt_work_init(work, rk_dsp_work, (void *)mbox_data);
    if (rt_workqueue_dowork(rkdsp->isr_workqueue, work) == -RT_EBUSY)
        dsp_err("work is busy, current index = %d\n", rkdsp->work_index);
}

/**
 * @brief  Dsp pfatalerror interrupt, will be call when dsp error.
 * @param  irq: no work.
 * @param  param: no work.
 */
static void dsp_error_isr(int irq, void *param)
{
    struct rk_dsp *rkdsp = &rk_dsp_dev;

    /* enter interrupt */
    rt_interrupt_enter();

    rt_hw_interrupt_mask(rkdsp->dsp_dev.errorIrq);
    dsp_err("pfatalerror, dsp_stat0:0x%08x, dsp_stat1:0x%08x\n",
            rkdsp->dsp_dev.grfReg->DSP_STAT0,
            rkdsp->dsp_dev.grfReg->DSP_STAT1);

    /* leave interrupt */
    rt_interrupt_leave();
}

/**
 * @brief  Mbox interrupt function.
 * @param  irq: no work.
 * @param  param: no work.
 */
static void dsp_mbox_isr(int irq, void *param)
{
    struct rk_dsp *rkdsp = &rk_dsp_dev;

    /* enter interrupt */
    rt_interrupt_enter();

    HAL_MBOX_IrqHandler(irq, rkdsp->dsp_dev.mboxReg);

    /* leave interrupt */
    rt_interrupt_leave();
}

/**
 * @brief  Init mbox and register client.
 * @param  rkdsp: dsp device point.
 */
static void rk_dsp_mbox_init(struct rk_dsp *rkdsp)
{
    rt_err_t ret;
    uint32_t chan;

    ret = HAL_MBOX_Init(rkdsp->dsp_dev.mboxReg, rkdsp->dsp_dev.mboxIsA2B);
    RT_ASSERT(!ret);
    for (chan = 0; chan < MBOX_CHAN_CNT; chan++)
    {
        mbox_client[chan].irq = rkdsp->dsp_dev.mboxIrq[chan];
        ret = HAL_MBOX_RegisterClient(rkdsp->dsp_dev.mboxReg, chan, &mbox_client[chan]);
        RT_ASSERT(ret == RT_EOK);
        dsp_mbox_int_install(chan);
    }
}

/**
 * @brief  Deinit mbox and unregister client.
 * @param  rkdsp: dsp device handle.
 */
static void rk_dsp_mbox_deinit(struct rk_dsp *rkdsp)
{
    uint32_t chan;

    HAL_MBOX_DeInit(rkdsp->dsp_dev.mboxReg);
    for (chan = 0; chan < MBOX_CHAN_CNT; chan++)
    {
        mbox_client[chan].irq = rkdsp->dsp_dev.mboxIrq[chan];
        HAL_MBOX_UnregisterClient(rkdsp->dsp_dev.mboxReg, chan, &mbox_client[chan]);
    }
}

/**
 * @brief  Download firmware, power up and boot dsp device.
 * @param  dsp: dsp device point.
 * @return rt_err_t
 */
static rt_err_t rk_dsp_open(struct rt_dsp_device *dsp, rt_uint16_t oflag)
{
    rk_dsp_func_enter();

    rt_err_t ret = RT_EOK;
    struct rk_dsp *rkdsp = (struct rk_dsp *)dsp->parent.user_data;

    rk_dsp_mbox_init(rkdsp);
    HAL_DSP_PowerOn(&rkdsp->dsp_dev);
    HAL_DSP_Enable(&rkdsp->dsp_dev, 0);
    rkdsp->status = DSP_ON;

#ifdef RT_USING_DSPFW_LOADER
    ret = rk_dsp_loader_image(&rkdsp->loader);
    /* Set dsp run */
    HAL_DSP_Start(&rkdsp->dsp_dev);
#endif
    rkdsp->dsp_dev.ops->ioctl(&rkdsp->dsp_dev, DSP_IOCTL_SET_MEM_GATING, (void *)1);
    rt_hw_interrupt_umask(rkdsp->dsp_dev.errorIrq);

    rk_dsp_func_exit();
    return ret;
}

/**
 * @brief  power down and stop dsp device.
 * @param  dsp: dsp device point.
 * @return rt_err_t
 */
static rt_err_t rk_dsp_close(struct rt_dsp_device *dsp)
{
    rk_dsp_func_enter();

    struct rk_dsp *rkdsp = (struct rk_dsp *)dsp->parent.user_data;

#ifdef RT_USING_DSPFW_LOADER
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)DSP_ITCM_BASE, ITCM_SIZE);
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)DSP_DTCM_BASE, DTCM_SIZE);
#endif

#ifdef RT_USING_DSP_TRACE
    rk_dsp_trace(rkdsp, rkdsp->trace_index + DSP_TRACE_SLOT_COUNT);
    if (rkdsp->trace_buffer)
        rkdsp_free(rkdsp->trace_buffer);
#endif

    HAL_DSP_Stop(&rkdsp->dsp_dev);
    HAL_DSP_PowerOff(&rkdsp->dsp_dev);
    rk_dsp_mbox_deinit(rkdsp);
    rkdsp->status = DSP_OFF;

    rk_dsp_func_exit();
    return RT_EOK;
}

/**
 * @brief  dsp device suspend.
 * @param  dsp: dsp device point.
 * @return rt_err_t
 */
static rt_err_t rk_dsp_suspend(struct rt_dsp_device *dsp)
{
    rk_dsp_func_enter();
    struct rk_dsp *rkdsp = (struct rk_dsp *)dsp->parent.user_data;

    rkdsp->last_status = rkdsp->status;
    rkdsp->status = DSP_SLEEP;
    HAL_DSP_Stop(&rkdsp->dsp_dev);
    HAL_DSP_SetTcmMode(DSP_TCM, RET2N_MODE);

    rk_dsp_func_exit();
    return RT_EOK;
}

/**
 * @brief  dsp device resume.
 * @param  dsp: dsp device point.
 * @return rt_err_t
 */
static rt_err_t rk_dsp_resume(struct rt_dsp_device *dsp)
{
    rk_dsp_func_enter();
    struct rk_dsp *rkdsp = (struct rk_dsp *)dsp->parent.user_data;

    HAL_DSP_SetTcmMode(DSP_TCM, NOR_MODE);
    HAL_DSP_Start(&rkdsp->dsp_dev);
    rkdsp->status = rkdsp->last_status;

    rk_dsp_func_exit();
    return RT_EOK;
}

/**
 * @brief  Notify dsp enter power down.
 * @param  dsp: dsp device point.
 * @return rt_err_t
 */
static rt_err_t rk_dsp_shutdown(struct rt_dsp_device *dsp)
{
    rk_dsp_func_enter();
    rt_err_t ret = RT_EOK;
    struct rk_dsp *rkdsp = (struct rk_dsp *)dsp->parent.user_data;

    dsp_mbox_send_data(rkdsp->dsp_dev.mboxReg, MBOX_CH_1, DSP_CMD_SHUTDOWN, 0);
    rkdsp->status = DSP_SHUTDOWN;
    if (HAL_DSP_WaitForPowerSt(ST_DSPAPM_WAIT, 50))
        ret = -RT_ETIMEOUT;

    rk_dsp_func_exit();
    return ret;
}

/**
 * @brief  Wakeup dsp from power state machine.
 * @param  dsp: dsp device point.
 * @return rt_err_t
 */
static rt_err_t rk_dsp_wakeup(struct rt_dsp_device *dsp)
{
    rk_dsp_func_enter();
    rt_err_t ret = RT_EOK;
    struct rk_dsp *rkdsp = (struct rk_dsp *)dsp->parent.user_data;

    HAL_DSP_SoftWakeup();
    rkdsp->status = DSP_ON;
    if (HAL_DSP_WaitForPowerSt(ST_DSPAPM_NORMAL, 50))
        ret = -RT_ETIMEOUT;

    rk_dsp_func_exit();
    return ret;
}

#ifdef RT_USING_FWANALYSIS
/**
 * @brief  Send vendor key to dsp.
 * @param  dsp: dsp device pointer.
 * @return rt_err_t
 */
static rt_err_t rk_dsp_send_vendor_key(struct rk_dsp *rkdsp, int *result)
{
    struct dsp_auth *vendor_auth;
    char *key;
    char *uncache_buf;
    int size;
    int ret, len = 1024;

    size = RT_ALIGN(sizeof(struct dsp_auth), RKDSP_CACHE_LINE_SIZE) + RKDSP_CACHE_LINE_SIZE;
    uncache_buf = rt_malloc_uncache(size);
    RT_ASSERT(uncache_buf != RT_NULL);
    vendor_auth = (struct dsp_auth *)RT_ALIGN((uint32_t)uncache_buf, RKDSP_CACHE_LINE_SIZE);
    RT_ASSERT(vendor_auth != RT_NULL);
    key = (char *)rkdsp_malloc(len);
    RT_ASSERT(key != RT_NULL);
    ret = get_device_sn(DEV_VENDOR_KEY, key, len);
    if (ret <= 0)
    {
        rt_kprintf("read vendor key fail\n");
        rt_free_uncache(uncache_buf);
        rkdsp_free(key);
        return RT_ERROR;
    }

    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)key, len);
    vendor_auth->flag = DSP_AUTH_WAIT_FLAG;
    vendor_auth->key = (uint32_t)key;
    vendor_auth->len = (uint32_t)len;
    vendor_auth->result = 0;
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)vendor_auth, sizeof(struct dsp_auth));
    dsp_mbox_send_data(rkdsp->dsp_dev.mboxReg, MBOX_CH_1, DSP_CMD_SEND_VENDOR_KEY,
                       (uint32_t)vendor_auth);
#ifdef RT_USING_SNOR
    rk_snor_xip_suspend();
#endif
    do
    {
        HAL_CPUDelayUs(10);
    }
    while (vendor_auth->flag != DSP_AUTH_RESP_FLAG);
#ifdef RT_USING_SNOR
    rk_snor_xip_resume();
#endif
    if (result)
        *result = vendor_auth->result;

    rt_free_uncache(uncache_buf);
    rkdsp_free(key);
    return RT_EOK;
}
#endif

/**
 * @brief  dsp control function, to queue and dequeue work.
 * @param  dsp: dsp device point.
 * @param  cmd: control command.
 * @param  arg: control argument.
 * @return rt_err_t
 */
static rt_err_t rk_dsp_control(struct rt_dsp_device *dsp, int cmd, void *arg)
{
    rk_dsp_func_enter();

    uint32_t value;
    rt_err_t ret = -RT_ERROR;
    struct rk_dsp *rkdsp;
    struct DSP_DEV *dsp_dev;
    struct dsp_work *work, *recv_work;
    struct rk_dvfs_desc *desc_dsp;

    rkdsp = (struct rk_dsp *)dsp->parent.user_data;
    if (!arg)
        return -RT_EINVAL;
    dsp_dev = &rkdsp->dsp_dev;
    work = (struct dsp_work *)arg;

    switch (cmd)
    {
    case RKDSP_CTL_QUEUE_WORK:
        if (rkdsp->status < DSP_WAITING)
        {
            rkdsp->status = DSP_WAITING;
            dsp_dbg("rk_dsp_control wait dsp ready\n");
            ret = rt_mb_recv(rkdsp->work_mb, &value, RT_WAITING_FOREVER);
            if (ret)
                break;
            dsp_dbg("rk_dsp_control dsp ready now\n");
        }

        dsp_dbg("queue work:0x%08x,id:0x%08x,type:0x%08x,"
                "size:0x%08x,param:0x%08x\n", work, work->id,
                work->algo_type, work->param_size, work->param);
        ret = dsp_mbox_send_msg(dsp_dev->mboxReg, MBOX_CH_0,
                                DSP_CMD_WORK, work);
        break;
    case RKDSP_CTL_DEQUEUE_WORK:
        ret = rt_mb_recv(rkdsp->work_mb, &value, RT_WAITING_FOREVER);
        if (ret)
            break;

        recv_work = (struct dsp_work *)value;
        if (recv_work != work)
        {
            dsp_err("work = 0x%08x, but recv_work = 0x%08x\n", work, recv_work);
            ret = -RT_ERROR;
        }
        break;
    case RKDSP_CTL_GET_FREQ:
        desc_dsp = dvfs_get_by_clk(dsp_dev->clkId, NULL);
        *(uint32_t *)arg = dvfs_req_get_rate(desc_dsp);
        ret = RT_EOK;
        break;
    case RKDSP_CTL_SET_FREQ:
        dsp_dev->ops->ioctl(dsp_dev, DSP_IOCTL_SET_DVFS_ST, (void *)true);
        desc_dsp = dvfs_get_by_clk(dsp_dev->clkId, NULL);
        ret = dvfs_set_rate(desc_dsp, 0, (uint32_t)arg);
        dsp_dev->ops->ioctl(dsp_dev, DSP_IOCTL_SET_DVFS_ST, (void *)false);

        dsp_dbg("dsp freq set to %d Hz\n", dvfs_req_get_rate(desc_dsp));
        break;
#ifdef RT_USING_FWANALYSIS
    case RKDSP_CTL_SEND_VENDOR_KEY:
        if (rkdsp->status < DSP_WAITING)
        {
            rkdsp->status = DSP_WAITING;
            dsp_dbg("rk_dsp_control wait dsp ready\n");
            ret = rt_mb_recv(rkdsp->work_mb, &value, RT_WAITING_FOREVER);
            if (ret)
                break;
            dsp_dbg("rk_dsp_control dsp ready now\n");
        }
        ret = rk_dsp_send_vendor_key(rkdsp, (int *)arg);
        break;
#endif
    default:
        ret = -RT_EIO;
        break;
    }

    rk_dsp_func_exit();
    return ret;
}

static const struct rt_dsp_ops rk_dsp_ops =
{
    .open = rk_dsp_open,
    .close = rk_dsp_close,
    .suspend = rk_dsp_suspend,
    .resume  = rk_dsp_resume,
    .shutdown = rk_dsp_shutdown,
    .wakeup = rk_dsp_wakeup,
    .control = rk_dsp_control,
};

/**
 * @brief  dsp device init and driver register.
 * @param  rkdsp: rkdsp point.
 * @param  dsp_name: dsp driver name.
 * @return rt_err_t
 */
static rt_err_t rk_dsp_register(struct rk_dsp *rkdsp, char *dsp_name)
{
    struct rt_dsp_device *rt_dsp;

    RT_ASSERT(rkdsp);
    RT_ASSERT(dsp_name);

    rt_dsp = &rkdsp->parent;
    rt_dsp->ops = &rk_dsp_ops;

    HAL_DSP_Init(&rkdsp->dsp_dev);

    rkdsp->isr_workqueue = rt_workqueue_create("dsp_isr", WORK_QUEUE_STACK_SIZE,
                           WORK_QUEUE_PRIORITY);
    RT_ASSERT(rkdsp->isr_workqueue);

    rkdsp->work_mb = rt_mb_create("dsp_mb", WORK_MB_SIZE,  RT_IPC_FLAG_FIFO);
    RT_ASSERT(rkdsp->work_mb);

    rt_hw_interrupt_install(rkdsp->dsp_dev.errorIrq, rkdsp->error_isr, RT_NULL, RT_NULL);

    HAL_DSP_PowerOff(&rkdsp->dsp_dev);
    return rt_hw_dsp_register(rt_dsp, dsp_name, RT_DEVICE_FLAG_RDWR, rkdsp);
}
/** @} */

/** @defgroup DSP_Public_Functions Public Functions
 *  @{
 */

/**
 * @brief  Create dsp work.
 * @param  work_type: work type: DSP_ALGO_WORK, DSP_CFG_WORK.
 * @return struct dsp_work
 */
struct dsp_work *rk_dsp_work_create(enum rkdsp_work_type work_type)
{
    struct dsp_work *work;

    work = rkdsp_malloc(sizeof(struct dsp_work));
    if (!work)
        return work;
    rt_memset(work, 0, sizeof(struct dsp_work));

    work->work_type = work_type;
    return work;
}

/**
 * @brief  Free work buffer.
 * @param  work: work point
 * @return rt_err_t
 */
rt_err_t rk_dsp_work_destroy(struct dsp_work *work)
{
    if (work)
        rkdsp_free((void *)work);
    return RT_EOK;
}

/**
 * @brief  Init dsp param and register dsp driver.
 * @return 0
 */
int rt_hw_dsp_init(void)
{
    rk_dsp_dev.error_isr = &dsp_error_isr;
    rk_dsp_dev.work_cnt = 0;
    rk_dsp_dev.work_index = 0;

    rk_dsp_register(&rk_dsp_dev, "dsp0");

    return 0;
}

/** @} */

INIT_DEVICE_EXPORT(rt_hw_dsp_init);

#endif // RT_USING_DSP

/** @} */

/** @} */
