/*
 *  Copyright (C) 2019, Fuzhou Rockchip Electronics Co., Ltd.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of Fuzhou Rockchip Electronics Co., Ltd. nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <rtthread.h>
#ifdef RT_USING_DSP
#include <rtdevice.h>
#include <rthw.h>
#include <stdio.h>
#include <string.h>
#include "asr.h"
#include "drv_cache.h"
#include "drv_dsp.h"
#include "drv_heap.h"
#include "globe_player.h"
#include "audio_server.h"
#include "rk_audio.h"
#include "sys_ctrl.h"

#define DBG_LEVEL         DBG_INFO
#define DBG_SECTION_NAME    "ASR"
#include "rtdbg.h"

#ifdef APP_USING_VAD
static struct rt_device *audio_dev;
static struct rt_device *vad_dev;
#else
static struct audio_player_stream *asr_stream;
static int stream_start;
#endif
static struct wake_cfg_param *g_cfg_param;
enum asr_state g_asr_state;
static event_queue *g_asr_queue = NULL;
static rt_thread_t asr_task_handle = NULL;
static int asr_run_exit = 1;

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
    rkdsp_free(work);
}

#ifdef APP_USING_VAD
static void audio_init(void)
{
    struct audio_buf vadbuf;
    struct audio_buf abuf;
    struct AUDIO_PARAMS aparams;
    uint32_t periodsz = 0;
    rt_err_t ret = RT_EOK;

    LOG_D("VAD init");
    audio_dev = rt_device_find(PRJCONF_ASR_SOUND_CARD);
    RT_ASSERT(audio_dev);
    vad_dev = rt_device_find("vad");
    RT_ASSERT(vad_dev);

    ret = rt_device_open(audio_dev, RT_DEVICE_OFLAG_RDONLY);
    RT_ASSERT(ret == RT_EOK);
    ret = rt_device_open(vad_dev, RT_DEVICE_OFLAG_RDONLY);
    RT_ASSERT(ret == RT_EOK);

    vadbuf.buf_size = AUDIO_VAD_BUFFER_SIZE;
    vadbuf.buf = (uint8_t *)g_cfg_param->ringbuf_addr;
    periodsz = g_cfg_param->period_size;
    RT_ASSERT(vadbuf.buf != RT_NULL);

    LOG_I("VAD buf: 0x%x, size: 0x%x bytes", vadbuf.buf, vadbuf.buf_size);
    LOG_I("VAD periodsize: 0x%08x bytes", periodsz);

    /* config vad */
    ret = rt_device_control(vad_dev, RK_AUDIO_CTL_VAD_SET_BUFFER, &vadbuf);
    RT_ASSERT(ret == RT_EOK);
    ret = rt_device_control(vad_dev, RK_AUDIO_CTL_VAD_SET_PERIOD, &periodsz);
    RT_ASSERT(ret == RT_EOK);
    ret = rt_device_control(vad_dev, RK_AUDIO_CTL_VAD_EN_BUSMODE, NULL);
    RT_ASSERT(ret == RT_EOK);

    /* TODO: temp */
    abuf.buf = vadbuf.buf;
    abuf.buf_size = AUDIO_VAD_BUFFER_SIZE / 4;
    abuf.period_size = abuf.buf_size / 16;
    ret = rt_device_control(audio_dev, RK_AUDIO_CTL_PCM_PREPARE, &abuf);
    RT_ASSERT(ret == RT_EOK);

    /* config stream */
    rt_memset(&aparams, 0x0, sizeof(aparams));
    aparams.channels = ASR_RECORD_CHANNELS;
    aparams.sampleRate = ASR_RECORD_SAMPLERATE;
    aparams.sampleBits = ASR_RECORD_BITS;

    ret = rt_device_control(audio_dev, RK_AUDIO_CTL_HW_PARAMS, &aparams);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(audio_dev, RK_AUDIO_CTL_START, NULL);
    RT_ASSERT(ret == RT_EOK);
}

static void audio_stop(void)
{
    rt_device_control(audio_dev, RK_AUDIO_CTL_STOP, NULL);
    rt_device_close(audio_dev);
    rt_device_close(vad_dev);
}
#endif

static void asr_run(void *dev)
{
    struct rt_device *dsp_dev;
    struct dsp_work *work;
    int dsp_freq;
#ifndef APP_USING_VAD
    struct wake_work_param *asr_buf;
    char *audio_buf = NULL;
    int i;
#endif
    int ret;

    asr_run_exit = 0;
    dsp_dev = rt_device_find("dsp0");
    if (dsp_dev == RT_NULL)
    {
        LOG_E("Can not find dsp");
        asr_run_exit = 1;
        return;
    }
    ret = rt_device_open(dsp_dev, RT_DEVICE_OFLAG_RDWR);
    if (ret != RT_NULL)
    {
        LOG_E("Can not open dsp");
        asr_run_exit = 1;
        return;
    }

    /* Config for voice wakeup */
    g_cfg_param = rkdsp_malloc(sizeof(struct wake_cfg_param));
    g_cfg_param->ringbuf_size = 128 * 1024;
#ifdef APP_USING_VAD
    work = dsp_create_work(WAKEUP_ID, WAKEUP_CONFIG,
                           (uint32_t)g_cfg_param, sizeof(struct wake_cfg_param));
#else
    work = dsp_create_work(ASR_WAKE_ID, ASR_WAKEUP_CONFIG,
                           (uint32_t)g_cfg_param, sizeof(struct wake_cfg_param));
#endif
    if (!work)
    {
        LOG_E("DSP config work create fail");
        asr_run_exit = 1;
        return;
    }
#ifdef PRJCONF_AI_CUSTOM
    ret = rt_device_control(dsp_dev, RKDSP_CTL_SET_FREQ, (void *)(594000000));
#else
    ret = rt_device_control(dsp_dev, RKDSP_CTL_SET_FREQ, (void *)(396000000));
#endif
    RT_ASSERT(!ret);
    ret = rt_device_control(dsp_dev, RKDSP_CTL_GET_FREQ, (void *)&dsp_freq);
    RT_ASSERT(!ret);
    LOG_I("Set DSP freq %d", dsp_freq);
    ret = rt_device_control(dsp_dev, RKDSP_CTL_QUEUE_WORK, work);
    RT_ASSERT(!ret);
    ret = rt_device_control(dsp_dev, RKDSP_CTL_DEQUEUE_WORK, work);
    RT_ASSERT(!ret);
    LOG_I("Ringbuf_addr:0x%08x, period_size:0x%08x",
          g_cfg_param->ringbuf_addr, g_cfg_param->period_size);
    dsp_destory_work(work);
    work = NULL;
#ifdef APP_USING_VAD
    audio_init();
#endif
    if (g_cfg_param)
        rkdsp_free(g_cfg_param);
    g_cfg_param = NULL;

#ifdef APP_USING_VAD
    work = dsp_create_work(WAKEUP_ID, WAKEUP_START, 0, 0);
#else
    audio_buf = rt_malloc(ASR_BUF_LEN * ASR_RECORD_CHANNELS);
    RT_ASSERT(audio_buf != NULL);
    memset(audio_buf, 0x0, ASR_BUF_LEN * ASR_RECORD_CHANNELS);
    asr_buf = (struct wake_work_param *)rkdsp_malloc(sizeof(struct wake_work_param));
#if defined(PRJCONF_AI_CUSTOM)
    asr_buf->in_buf = (uint32_t)rkdsp_malloc(ASR_BUF_LEN);
    asr_buf->out_buf = (uint32_t)rkdsp_malloc(ASR_BUF_LEN / 2);
    asr_buf->aec_buf = (uint32_t)rkdsp_malloc(ASR_BUF_LEN / 2);
#elif defined(PRJCONF_AI_LWAKEUP)
    asr_buf->buf = (uint32_t)rkdsp_malloc(ASR_BUF_LEN);
    asr_buf->output = (uint32_t)rkdsp_malloc(ASR_BUF_LEN / 2);
#else
    asr_buf->buf = (uint32_t)rkdsp_malloc(ASR_BUF_LEN);
#endif
    asr_buf->buf_size = ASR_BUF_LEN;
    asr_buf->result = 0;
    work = dsp_create_work(ASR_WAKE_ID, ASR_WAKEUP_START, (uint32_t)asr_buf,
                           sizeof(struct wake_work_param));
#endif
    if (!work)
    {
        LOG_E("DSP work create fail");
        g_asr_state = ASR_ERROR;
    }
#ifndef APP_USING_VAD
    /* Wait read task ready */
    while (!stream_start)
        rt_thread_mdelay(10);
#endif
    while (g_asr_state == ASR_RUNNING)
    {
#ifndef APP_USING_VAD
#if defined(PRJCONF_AI_CUSTOM)
        audio_stream_read(asr_stream, (char *)audio_buf, ASR_BUF_LEN * 2);
        for (i = 0; i < ASR_BUF_LEN / 4; i++)
        {
            *((int *)asr_buf->in_buf + i) = *((int *)audio_buf + i * 2);
            *((short *)asr_buf->aec_buf + i) = *((short *)audio_buf + i * 4 + 2);
        }
        rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)asr_buf->in_buf, ASR_BUF_LEN);
        rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)asr_buf->aec_buf, ASR_BUF_LEN / 2);
#elif defined(PRJCONF_AI_LWAKEUP)
        audio_stream_read(asr_stream, (char *)audio_buf, ASR_BUF_LEN * 2);
        for (i = 0; i < ASR_BUF_LEN / 2; i++)
        {
            *((short *)asr_buf->buf + i) = *((short *)audio_buf + i * 2);
        }
        rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)asr_buf->buf, ASR_BUF_LEN);
#else
        audio_stream_read(asr_stream, audio_buf, ASR_BUF_LEN * ASR_RECORD_CHANNELS);
        for (i = 0; i < ASR_BUF_LEN / 2; i++)
        {
            *((short *)asr_buf->buf + i) = *((short *)audio_buf + i * ASR_RECORD_CHANNELS);
        }
        rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)asr_buf->buf, ASR_BUF_LEN);
#endif
#endif
        ret = rt_device_control(dsp_dev, RKDSP_CTL_QUEUE_WORK, work);
        RT_ASSERT(!ret);
        /* Wait for wake up */
        ret = rt_device_control(dsp_dev, RKDSP_CTL_DEQUEUE_WORK, work);
        RT_ASSERT(!ret);
#if defined(PRJCONF_AI_CUSTOM)
        rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, (void *)asr_buf->out_buf, ASR_BUF_LEN / 2);
#endif
        if (work->result == 1 && g_asr_state == ASR_RUNNING)
        {
            /* play notice */
#ifdef APP_USING_VAD
            LOG_I("wakeup:%d %d", work->result, work->param);
            sys_event_send(CTRL_MSG_TYPE_VKEY, CTRL_MSG_SUB_TYPE_ASR, (uint32_t)work->param, 0);
#else
            LOG_I("wakeup:%d %d", work->result, ((struct wake_work_param *)work->param)->result);
            sys_event_send(CTRL_MSG_TYPE_VKEY, CTRL_MSG_SUB_TYPE_ASR, ((struct wake_work_param *)work->param)->result, 0);
#endif
        }
    }
    asr_run_exit = 1;
    dsp_destory_work(work);
#ifdef APP_USING_VAD
    audio_stop();
#else
#if defined(PRJCONF_AI_CUSTOM)
    rkdsp_free((void *)asr_buf->in_buf);
    rkdsp_free((void *)asr_buf->out_buf);
    rkdsp_free((void *)asr_buf->aec_buf);
#else
    rkdsp_free((void *)asr_buf->buf);
#endif
    rkdsp_free((void *)asr_buf);
    rt_free(audio_buf);
#endif
    LOG_W("ASR stop");
    if (g_asr_state != ASR_ERROR)
        g_asr_state = ASR_IDLE;
    rt_device_close(dsp_dev);
    dsp_dev = NULL;
}

#ifndef APP_USING_VAD
#if defined(PRJCONF_AI_CUSTOM) || defined(PRJCONF_AI_LWAKEUP)
static void asr_read(void *dev)
{
    capture_device_t record_device = (capture_device_t)DEFAULT_CAPTURE_DEVICE;
    capture_device_cfg_t record_device_cfg;
    char *audio_buf = NULL;
    short *aud_buf;
    SRCState_4ch *src_handle;
    uint32_t input_rate = 48000, output_rate = 16000;
    uint32_t rate_ratio = input_rate / output_rate;
    short *src_out;
    long out_len;
    int period_size;
    int size = 0;

    record_device_cfg.device_name = PRJCONF_ASR_SOUND_CARD;
    record_device_cfg.samplerate = ASR_RECORD_SAMPLERATE;
    record_device_cfg.frame_size = ASR_RECORD_BUFFER_SIZE;
    record_device_cfg.channels = ASR_RECORD_CHANNELS;
    record_device_cfg.bits = ASR_RECORD_BITS;
    period_size = ASR_RECORD_BUFFER_SIZE / ASR_RECORD_CHANNELS / (ASR_RECORD_BITS >> 3);

    if (record_device.open(&record_device, &record_device_cfg))
    {
        LOG_E("Can not open %s", PRJCONF_ASR_SOUND_CARD);
        g_asr_state = ASR_ERROR;
        goto ASR_END;
    }
    if (record_device.start(&record_device) != 0)
    {
        LOG_E("Can not start %s", PRJCONF_ASR_SOUND_CARD);
        record_device.close(&record_device);
        g_asr_state = ASR_ERROR;
        goto ASR_END;
    }

    size = ASR_RECORD_BUFFER_SIZE + NUMTAPS_4CH * 4 * 2;
    audio_buf = rt_malloc(size);
    RT_ASSERT(audio_buf != NULL);
    asr_stream = audio_stream_create(ASR_RECORD_BUFFER_SIZE * 4);
    RT_ASSERT(asr_stream != NULL);
    audio_stream_start(asr_stream);
    LOG_D("Record start %p %d", audio_buf, ASR_RECORD_BUFFER_SIZE);
    src_handle = (SRCState_4ch *)rt_malloc(sizeof(SRCState_4ch));
    resample_48to16_4ch_init(src_handle, input_rate, output_rate);
    src_out = (ssrc_type *)rt_malloc(ASR_RECORD_BUFFER_SIZE / rate_ratio);
    if (!src_out)
    {
        LOG_E("src_out malloc failed");
        rt_free(audio_buf);
        record_device.close(&record_device);
        g_asr_state = ASR_ERROR;
        goto ASR_END;
    }
    memset(src_out, 0x0, ASR_RECORD_BUFFER_SIZE / rate_ratio);
    aud_buf = (short *)(audio_buf + NUMTAPS_4CH * 4 * 2);
    while (g_asr_state == ASR_RUNNING)
    {
        record_device.read(&record_device, aud_buf, ASR_RECORD_BUFFER_SIZE);
        resample_48to16_4ch_process(src_handle, aud_buf, src_out,
                                    (long)period_size, &out_len);
        audio_stream_write(asr_stream, (char *)src_out, out_len * 8);
        stream_start = 1;
    }
    record_device.stop(&record_device);
    record_device.close(&record_device);
    rt_free(src_handle);
    rt_free(audio_buf);
    rt_free(src_out);
ASR_END:
    audio_stream_stop(asr_stream);
    while (asr_run_exit == 0)
        rt_thread_mdelay(10);
    audio_stream_destroy(asr_stream);
    asr_stream = NULL;
}
#else
static void asr_read(void *dev)
{
    capture_device_t record_device = DEFAULT_CAPTURE_DEVICE;
    capture_device_cfg_t record_device_cfg;
    char *audio_buf;

    record_device_cfg.device_name = PRJCONF_ASR_SOUND_CARD;
    record_device_cfg.samplerate = ASR_RECORD_SAMPLERATE;
    record_device_cfg.frame_size = ASR_RECORD_BUFFER_SIZE;
    record_device_cfg.channels = ASR_RECORD_CHANNELS;
    record_device_cfg.bits = ASR_RECORD_BITS;

    if (record_device.open(&record_device, &record_device_cfg))
    {
        LOG_E("Can not open %s", PRJCONF_ASR_SOUND_CARD);
        g_asr_state = ASR_ERROR;
        goto ASR_END;
    }
    if (record_device.start(&record_device) != 0)
    {
        LOG_E("Can not start %s", PRJCONF_ASR_SOUND_CARD);
        record_device.close(&record_device);
        g_asr_state = ASR_ERROR;
        goto ASR_END;
    }

    audio_buf = rt_malloc(ASR_RECORD_BUFFER_SIZE);
    RT_ASSERT(audio_buf != NULL);
    asr_stream = audio_stream_create(ASR_RECORD_BUFFER_SIZE * 4);
    RT_ASSERT(asr_stream != NULL);
    audio_stream_start(asr_stream);
    LOG_D("Record start %p %d", audio_buf, ASR_RECORD_BUFFER_SIZE);
    while (g_asr_state == ASR_RUNNING)
    {
        record_device.read(&record_device, audio_buf, ASR_RECORD_BUFFER_SIZE);
        audio_stream_write(asr_stream, audio_buf, ASR_RECORD_BUFFER_SIZE);
        stream_start = 1;
    }
    record_device.stop(&record_device);
    record_device.close(&record_device);
    rt_free(audio_buf);
ASR_END:
    audio_stream_stop(asr_stream);
    while (asr_run_exit == 0)
        rt_thread_mdelay(10);
    audio_stream_destroy(asr_stream);
    asr_stream = NULL;
}
#endif
#endif

static int asr_init(void)
{
#ifdef APP_USING_VAD
    LOG_W("ASR init VAD path");
#else
    LOG_W("ASR init CM4 path");
#endif

    g_asr_state = ASR_RUNNING;
    rt_thread_t tid;
#ifndef APP_USING_VAD
    stream_start = 0;
    tid = rt_thread_create("asr read", asr_read, NULL, 1024, 15, 10);
    if (tid)
        rt_thread_startup(tid);
#endif
    tid = rt_thread_create("asr run", asr_run, NULL, 512, 15, 10);
    if (tid)
        rt_thread_startup(tid);
    return RT_EOK;
}

void asr_task(void *arg)
{
    struct event_msg msg;

    g_asr_queue = normal_event_queue_create(1, sizeof(struct event_msg));
    while (1)
    {
        g_asr_queue->recv(g_asr_queue, &msg, RT_WAITING_FOREVER);
        switch (msg.event)
        {
        case ASR_QUEUE_CMD_LOAD:
            if (g_asr_state == ASR_IDLE || g_asr_state == ASR_ERROR)
            {
                if (asr_init() == RT_EOK)
                {
                    LOG_W("ASR start");
                }
            }
            break;
        case ASR_QUEUE_CMD_STOP:
            g_asr_state = ASR_STOP;
#ifndef APP_USING_VAD
            audio_stream_stop(asr_stream);
#endif
            break;
        default:
            break;
        }
    }
}

int asr_load(void)
{
    struct event_msg msg;

    if (g_asr_queue)
    {
        msg.event = ASR_QUEUE_CMD_LOAD;
        msg.data = 0;
        g_asr_queue->send(g_asr_queue, &msg, RT_WAITING_FOREVER);
        while (g_asr_state != ASR_RUNNING && g_asr_state != ASR_ERROR)
            rt_thread_mdelay(10);
        LOG_I("ASR load ret %d", g_asr_state);

        return g_asr_state;
    }

    return -RT_ERROR;
}

int asr_stop(void)
{
    struct event_msg msg;

    if (g_asr_queue)
    {
        msg.event = ASR_QUEUE_CMD_STOP;
        msg.data = 0;
        g_asr_queue->send(g_asr_queue, &msg, RT_WAITING_FOREVER);
        while (g_asr_state != ASR_IDLE && g_asr_state != ASR_ERROR)
            rt_thread_mdelay(10);
        LOG_I("ASR stop ret %d", g_asr_state);

        return g_asr_state;
    }

    return -RT_ERROR;
}

void asr_start_app(void)
{
    if (asr_task_handle == NULL)
    {
        asr_task_handle = rt_thread_create("asr task", asr_task, NULL, 256, 15, 10);
        if (asr_task_handle)
            rt_thread_startup(asr_task_handle);
        else
            LOG_E("Create ASR task failed");
    }
}

MSH_CMD_EXPORT(asr_start_app, Create ASR task);
MSH_CMD_EXPORT(asr_load, Load ASR);
MSH_CMD_EXPORT(asr_stop, Stop ASR);
#endif
