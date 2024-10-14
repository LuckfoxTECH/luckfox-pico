/*
 * Copyright (c) 2021 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-09     Jair Wu      First version
 *
 */

#include "flac_decode.h"

#ifdef RT_USING_EXT_FLAC_DECODER

#include "drv_dsp.h"
static struct rt_device *g_dsp_handle;
static struct dsp_work *g_dsp_work;

int32_t flac_dec_init(struct flac_dec *dec)
{
    int ret;

    g_dsp_handle = rt_device_find("dsp0");
    ret = rt_device_open(g_dsp_handle, RT_DEVICE_OFLAG_RDWR);
    RT_ASSERT(ret == RT_EOK);

#ifdef RT_USING_PM_DVFS
    uint32_t freq;
    ret = rt_device_control(g_dsp_handle, RKDSP_CTL_SET_FREQ, (void *)297000000);
    RT_ASSERT(ret == RT_EOK);
    ret = rt_device_control(g_dsp_handle, RKDSP_CTL_GET_FREQ, (void *)&freq);
    RT_ASSERT(ret == RT_EOK);
    rt_kprintf("DSP set freq %ld\n", freq);
#endif

    g_dsp_work = rk_dsp_work_create(RKDSP_CODEC_WORK);
    g_dsp_work->id = 0;
    g_dsp_work->algo_type = DSP_CODEC_FLAC_DECODE_OPEN;
    g_dsp_work->param = (uint32_t)dec;
    g_dsp_work->param_size = sizeof(struct flac_dec);
    ret = rt_device_control(g_dsp_handle, RKDSP_CTL_QUEUE_WORK, g_dsp_work);
    RT_ASSERT(ret == RT_EOK);
    ret = rt_device_control(g_dsp_handle, RKDSP_CTL_DEQUEUE_WORK, g_dsp_work);
    RT_ASSERT(ret == RT_EOK);

    return g_dsp_work->result;
}

int32_t flac_dec_process(struct flac_dec *dec)
{
    int ret;

    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, dec->buf_in, dec->buf_in_size);

    g_dsp_work->id = 0;
    g_dsp_work->algo_type = DSP_CODEC_FLAC_DECODE_PROCESS;
    ret = rt_device_control(g_dsp_handle, RKDSP_CTL_QUEUE_WORK, g_dsp_work);
    RT_ASSERT(ret == RT_EOK);
    ret = rt_device_control(g_dsp_handle, RKDSP_CTL_DEQUEUE_WORK, g_dsp_work);
    RT_ASSERT(ret == RT_EOK);

    if (dec->buf_out_size)
    {
        rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, dec->buf_out[0], dec->buf_out_size * sizeof(uint32_t));
        rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, dec->buf_out[1], dec->buf_out_size * sizeof(uint32_t));
    }

    return g_dsp_work->result;
}

int32_t flac_dec_deinit(struct flac_dec *dec)
{
    int ret;

    g_dsp_work->id = 0;
    g_dsp_work->algo_type = DSP_CODEC_FLAC_DECODE_CLOSE;
    ret = rt_device_control(g_dsp_handle, RKDSP_CTL_QUEUE_WORK, g_dsp_work);
    RT_ASSERT(ret == RT_EOK);
    ret = rt_device_control(g_dsp_handle, RKDSP_CTL_DEQUEUE_WORK, g_dsp_work);
    RT_ASSERT(ret == RT_EOK);
    rk_dsp_work_destroy(g_dsp_work);
    rt_device_close(g_dsp_handle);

    return g_dsp_work->result;
}

#endif  // RT_USING_EXT_FLAC_DECODER
