/**
  * Copyright (c) 2022 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#include "aupipe.h"

#ifdef RT_USING_AUPIPE_AUDIOSRC

#include "drv_heap.h"
#include "rk_audio.h"

#undef DBG_SECTION_NAME
#define DBG_SECTION_NAME    "AUDIOSRC"

#define AUDIOSRC(obj)               ((ApAudioSrc *)(obj))

typedef struct audiosrc_object
{
    ApObject base;
    ApPad *src_pad;

    int channels;
    char *card_name;

    rt_thread_t loop;
    int loop_running;
    struct rt_device *card;
    struct AUDIO_PARAMS param;
    struct audio_buf abuf;
    rt_sem_t sem;
} ApAudioSrc;

static int audiosrc_init(ApObject *obj, void *arg)
{
    ApAudioSrc *audsrc = AUDIOSRC(obj);

    int status = RT_EOK;
    char *parameters = (char *)arg;

    status |= aupipe_find_property(parameters, "card",
                                   VALUE_TYPE_STRING, &audsrc->card_name);
    status |= aupipe_find_property(parameters, "channels",
                                   VALUE_TYPE_INT, &audsrc->channels);
    if (status != RT_EOK)
    {
        LOG_E("%s plug-in parameter initialization failed", audsrc->base.name);
        return -RT_ERROR;
    }

    audsrc->src_pad = aupipe_pad_new(obj, audsrc->channels);
    RT_ASSERT(audsrc->src_pad != NULL);

    audsrc->sem = rt_sem_create("audsrc", 0, RT_IPC_FLAG_FIFO);
    RT_ASSERT(audsrc->sem != NULL);

    return RT_EOK;
}

static int audiosrc_deinit(ApObject *obj)
{
    ApAudioSrc *audsrc = AUDIOSRC(obj);

    rt_free(audsrc->src_pad);
    rt_free(audsrc->card_name);

    rt_sem_delete(audsrc->sem);

    return RT_EOK;
}

static void do_capture(void *arg)
{
    ApAudioSrc *obj = AUDIOSRC(arg);

    int bit_width;
    int period_byte;
    int total_period_byte;

    bit_width = obj->param.sampleBits >> 3;
    period_byte = obj->abuf.period_size * bit_width;
    total_period_byte = period_byte * obj->channels;

    char *read_buffer = (char *)rt_malloc(total_period_byte);

    while (obj->loop_running)
    {
        if (!rt_device_read(obj->card, 0, read_buffer, obj->abuf.period_size))
        {
            LOG_E("Error capturing sample\n");
            break;
        }

        for (int i = 0; i < obj->channels; i++)
        {
            if (obj->src_pad[i].peer == RT_NULL)
                continue;

            ApBuffer *buffer = (ApBuffer *)aupipe_buffer_new(DATA_TYPE_AUDIO, period_byte);
            if (!buffer)
            {
                LOG_E("%s malloc %d failed", period_byte);
                break;
            }

            for (int j = 0; j < obj->abuf.period_size; j++)
            {
                rt_memcpy(buffer->data + bit_width * j, read_buffer + bit_width * (obj->channels * j + i), bit_width);
            }

            buffer = aupipe_buffer_ref(buffer);
            aupipe_pad_push(&obj->src_pad[i], buffer);
        }
    }
    rt_free(read_buffer);
    rt_sem_release(obj->sem);

    obj->loop_running = 0;
}

static int audiosrc_open_card(ApAudioSrc *obj)
{
    int size;
    rt_err_t ret;
    int period_count = 4;

    Aupipe *aupipe = obj->base.parent;

    struct audio_buf *abuf = &obj->abuf;
    struct AUDIO_PARAMS *param = &obj->param;

    obj->card = rt_device_find(obj->card_name);
    if (!obj->card)
    {
        LOG_E("Can't find sound device: %s\n", obj->card_name);
        goto no_card;
    }

    ret = rt_device_open(obj->card, RT_DEVICE_OFLAG_RDONLY);
    if (ret < 0)
    {
        rt_kprintf("Failed to open %s, err: %d\n", obj->card->parent.name, ret);
        goto open_failed;
    }

    abuf->period_size = aupipe->period_size;
    abuf->buf_size = aupipe->period_size * period_count;
    size = abuf->buf_size * obj->channels * (aupipe->bits >> 3); /* frames to bytes */
    abuf->buf = rt_malloc_uncache(size);
    if (!abuf->buf)
    {
        rt_kprintf("Buffer alloc failed!\n");
        goto malloc_failed;
    }

    param->channels = obj->channels;
    param->sampleRate = aupipe->rate;
    param->sampleBits = aupipe->bits;

    ret = rt_device_control(obj->card, RK_AUDIO_CTL_PCM_PREPARE, abuf);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(obj->card, RK_AUDIO_CTL_HW_PARAMS, param);
    RT_ASSERT(ret == RT_EOK);

    return RT_EOK;

malloc_failed:
    rt_device_close(obj->card);
open_failed:
no_card:
    return -RT_ERROR;
}

static int audiosrc_close_card(ApAudioSrc *obj)
{
    struct audio_buf *abuf = &obj->abuf;
    rt_err_t ret;

    ret = rt_device_control(obj->card, RK_AUDIO_CTL_STOP, NULL);
    RT_ASSERT(ret == RT_EOK);
    ret = rt_device_control(obj->card, RK_AUDIO_CTL_PCM_RELEASE, NULL);
    RT_ASSERT(ret == RT_EOK);

    rt_free_uncache(abuf->buf);
    rt_device_close(obj->card);

    return RT_EOK;
}

static int audiosrc_set_state(ApObject *obj, int state)
{
    ApAudioSrc *audsrc = AUDIOSRC(obj);
    switch (state)
    {
    case STATE_NULL_TO_READY:
        LOG_I("STATE_NULL_TO_READY");
        if (audiosrc_open_card(audsrc) != RT_EOK)
            return -RT_ERROR;
        audsrc->loop = rt_thread_create("audiosrc", do_capture, audsrc,
                                        4096, RT_THREAD_PRIORITY_MAX / 2, 10);
        if (!audsrc->loop)
        {
            audiosrc_close_card(audsrc);
            return -RT_ERROR;
        }
        break;
    case STATE_PAUSED_TO_PLAYING:
        LOG_I("STATE_PAUSED_TO_PLAYING");
        audsrc->loop_running = 1;
        rt_thread_startup(audsrc->loop);
        break;
    case STATE_PLAYING_TO_PAUSED:
        LOG_I("STATE_PLAYING_TO_PAUSED");
        if (audsrc->loop_running == 1)
        {
            /* loop still running */
            audsrc->loop_running = 0;
            if (rt_sem_take(audsrc->sem, 3000))
            {
                /* Timeout, force delete */
                LOG_W("Timeout");
                rt_thread_delete(audsrc->loop);
            }
        }
        break;
    case STATE_READY_TO_NULL:
        LOG_I("STATE_READY_TO_NULL");
        audiosrc_close_card(audsrc);
        break;
    default:
        break;
    }

    return RT_EOK;
}

static int audiosrc_set_property(ApObject *obj, char *name, void *arg)
{
    return RT_EOK;
}

static int audiosrc_get_property(ApObject *obj, char *name, void *arg)
{
    return RT_EOK;
}

static ApPad *audiosrc_get_pad(ApObject *obj, int type, int id)
{
    ApAudioSrc *audsrc = AUDIOSRC(obj);

    if (type == PAD_TYPE_SINK)
    {
        LOG_E("%s plug-in has no sink pad", obj->name);
        return NULL;
    }

    return &audsrc->src_pad[id];
}

OBJECT_BASE_DEFINE(audiosrc, ApAudioSrc);

#endif

