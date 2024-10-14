/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    dsp_test.c
  * @author  Huaping Liao
  * @version V0.1
  * @date    20-Mar-2019
  * @brief   dsp test for pisces to receive voice wakeup
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#if defined(RT_USING_COMMON_TEST_DSP)

#include "dsp_test.h"
#include "drv_dsp.h"
#include "dma.h"
#include "drv_dvfs.h"

#define WAKEUP_CONFIG                0x40000001
#define WAKEUP_START                 0x40000002
#define ASR_WAKEUP_CONFIG            0x40000003
#define ASR_WAKEUP_START             0x40000004

#define WAKEUP_WORD_LEN              32
#define WAKEUP_ID                    0x50000001
#define ASR_WAKE_ID                  0x50000002

#define WAKE_WORD_LEN                32
#define ASR_BUF_LEN                  320
/*
 * wake_cfg_param - parameter used to config DSP core
 *
 * @wake_word: a string to store wake up word
 * @advan_size: after wake up, lead size to get voice, unit byte
 * @unit_size: the length of each get voice, uint byte
 * @result: result to return
 */
struct wake_cfg_param
{
    uint32_t advan_size;
    uint32_t period_size;
    uint32_t ringbuf_size;
    uint32_t ringbuf_addr;
    uint32_t mcu_hw_ptr;
    uint32_t dsp_hw_ptr;
    uint32_t result;
    uint32_t reserve[0];
};

struct wake_work_param
{
    uint32_t buf;
    uint32_t buf_size;
    uint32_t result;
};

static dsp_wakeup_call g_wakeup_callback;
static struct wake_cfg_param *cfg_param;

void dsp_wakeup_register_call(dsp_wakeup_call callback)
{
    g_wakeup_callback = callback;
}

uint32_t dsp_malloc(uint32_t size)
{
    return cfg_param->ringbuf_addr;
}

uint32_t dsp_get_period_size(void)
{
    return cfg_param->period_size;
}

static struct dsp_work *dsp_create_work(uint32_t id,
                                        uint32_t algo_type,
                                        uint32_t param,
                                        uint32_t param_size)
{
    struct dsp_work *work;

    work = rk_dsp_work_create(RKDSP_ALGO_WORK);
    if (work)
    {
        work->id = id;
        work->algo_type = algo_type;
        work->param = param;
        work->param_size = param_size;
    }
    return work;
}

static void dsp_destory_work(struct dsp_work *work)
{
    rk_dsp_work_destroy(work);
}

static void dsp_vad_wakeup(void *args)
{
    rt_err_t ret;
    uint32_t rate;
    struct rt_device *dsp_dev;
    struct dsp_work *work;

    rt_kprintf("dsp wakeup_test\n");

    dsp_dev = rt_device_find("dsp0");
    RT_ASSERT(dsp_dev != RT_NULL);
    rt_device_open(dsp_dev, RT_DEVICE_OFLAG_RDWR);

    rt_device_control(dsp_dev, RKDSP_CTL_SET_FREQ, (void *)(396 * MHZ));
    rt_device_control(dsp_dev, RKDSP_CTL_GET_FREQ, (void *)&rate);
    rt_kprintf("current dsp freq: %d MHz\n", rate / MHZ);

    cfg_param = rkdsp_malloc(sizeof(struct wake_cfg_param));
    /* Config for voice wakeup */
    cfg_param->ringbuf_size = 128 * 1024;
    work = dsp_create_work(WAKEUP_ID, WAKEUP_CONFIG,
                           (uint32_t)cfg_param, sizeof(struct wake_cfg_param));
    if (!work)
        rt_kprintf("dsp create config work fail\n");

    ret = rt_device_control(dsp_dev, RKDSP_CTL_QUEUE_WORK, work);
    RT_ASSERT(!ret);
    ret = rt_device_control(dsp_dev, RKDSP_CTL_DEQUEUE_WORK, work);
    RT_ASSERT(!ret);
    rt_kprintf("ringbuf_addr:0x%08x, period_size:0x%08x\n",
               cfg_param->ringbuf_addr, cfg_param->period_size);
    dsp_destory_work(work);

    /* Start to work */
    work = dsp_create_work(WAKEUP_ID, WAKEUP_START, 0, 0);
    if (!work)
        rt_kprintf("dsp create config work fail\n");

    while (1)
    {
        ret = rt_device_control(dsp_dev, RKDSP_CTL_QUEUE_WORK, work);
        RT_ASSERT(!ret);
        ret = rt_device_control(dsp_dev, RKDSP_CTL_DEQUEUE_WORK, work);
        RT_ASSERT(!ret);
        rt_kprintf("work result:0x%08x\n", work->result);
    }
    dsp_destory_work(work);
    rkdsp_free(cfg_param);
    rt_device_close(dsp_dev);
}

static struct wake_work_param *dsp_asr_param_prepare()
{
    struct wake_work_param *param = rkdsp_malloc(sizeof(struct wake_work_param));
    /* Can change to use voice buffer */
    param->buf = (uint32_t)rkdsp_malloc(ASR_BUF_LEN);
    param->buf_size = ASR_BUF_LEN;
    param->result = 0;
    return param;
}

static void dsp_asr_wakeup(void *args)
{
    rt_err_t ret;
    uint32_t rate;
    struct rt_device *dsp_dev;
    struct dsp_work *work;
    struct wake_work_param *param;

    rt_kprintf("dsp wakeup_test\n");

    dsp_dev = rt_device_find("dsp0");
    RT_ASSERT(dsp_dev != RT_NULL);
    rt_device_open(dsp_dev, RT_DEVICE_OFLAG_RDWR);

    rt_device_control(dsp_dev, RKDSP_CTL_SET_FREQ, (void *)(396 * MHZ));
    rt_device_control(dsp_dev, RKDSP_CTL_GET_FREQ, (void *)&rate);
    rt_kprintf("current dsp freq: %d MHz\n", rate / MHZ);

    cfg_param = rkdsp_malloc(sizeof(struct wake_cfg_param));
    /* Config for voice wakeup */
    cfg_param->ringbuf_size = 128 * 1024;
    work = dsp_create_work(ASR_WAKE_ID, ASR_WAKEUP_CONFIG,
                           (uint32_t)cfg_param, sizeof(struct wake_cfg_param));
    if (!work)
        rt_kprintf("dsp create config work fail\n");

    ret = rt_device_control(dsp_dev, RKDSP_CTL_QUEUE_WORK, work);
    RT_ASSERT(!ret);
    ret = rt_device_control(dsp_dev, RKDSP_CTL_DEQUEUE_WORK, work);
    RT_ASSERT(!ret);
    rt_kprintf("ringbuf_addr:0x%08x, period_size:0x%08x\n",
               cfg_param->ringbuf_addr, cfg_param->period_size);
    dsp_destory_work(work);

    /* Start to work */
    param = dsp_asr_param_prepare();
    work = dsp_create_work(ASR_WAKE_ID, ASR_WAKEUP_START, (uint32_t)param,
                           sizeof(struct wake_work_param));
    if (!work)
        rt_kprintf("dsp create config work fail\n");

    while (1)
    {
        ret = rt_device_control(dsp_dev, RKDSP_CTL_QUEUE_WORK, work);
        RT_ASSERT(!ret);
        ret = rt_device_control(dsp_dev, RKDSP_CTL_DEQUEUE_WORK, work);
        RT_ASSERT(!ret);
        rt_kprintf("work result:0x%08x\n", work->result);
    }
    dsp_destory_work(work);
    rkdsp_free(cfg_param);
    rt_device_close(dsp_dev);
}

static void dsp_send_vendor_key(void *args)
{
    struct rt_device *dsp_dev;
    int result;
    rt_err_t ret;

    rt_kprintf("dsp send vendor key\n");

    dsp_dev = rt_device_find("dsp0");
    RT_ASSERT(dsp_dev != RT_NULL);
    rt_device_open(dsp_dev, RT_DEVICE_OFLAG_RDWR);

    ret = rt_device_control(dsp_dev, RKDSP_CTL_SEND_VENDOR_KEY, (void *)&result);
    if (ret == RT_EOK)
        rt_kprintf("result = %d\n", result);
    else
        rt_kprintf("send vendor key failed\n");

    rt_device_close(dsp_dev);
}

static void dsp_vad_test(void)
{
    rt_thread_t thread;

    thread = rt_thread_create("dsp_vad_wakeup",
                              dsp_vad_wakeup, RT_NULL,
                              1024, 10, 20);
    if (thread != RT_NULL)
        rt_thread_startup(thread);
}

static void dsp_asr_test(void)
{
    rt_thread_t thread;

    thread = rt_thread_create("dsp_asr_wakeup",
                              dsp_asr_wakeup, RT_NULL,
                              1024, 10, 20);
    if (thread != RT_NULL)
        rt_thread_startup(thread);
}

static void dsp_vendor_test(void)
{
    rt_thread_t thread;

    thread = rt_thread_create("dsp_vendor",
                              dsp_send_vendor_key, RT_NULL,
                              1024, 10, 10);
    if (thread != RT_NULL)
        rt_thread_startup(thread);
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(dsp_vad_test, dsp_vad_test test. e.g: dsp_vad_test);
MSH_CMD_EXPORT(dsp_asr_test, dsp_asr_test test. e.g: dsp_asr_test);
MSH_CMD_EXPORT(dsp_vendor_test, dsp_vendor_test test. e.g: dsp_vendor_test);
#endif

#endif
