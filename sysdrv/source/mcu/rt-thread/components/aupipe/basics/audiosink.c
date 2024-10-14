/**
  * Copyright (c) 2022 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#include "aupipe.h"

#ifdef RT_USING_AUPIPE_AUDIOSINK

#include "drv_heap.h"
#include "rk_audio.h"

#undef DBG_SECTION_NAME
#define DBG_SECTION_NAME    "AUDIOSINK"

#define AUDIOSINK(obj)               ((ApAudioSink *)(obj))

typedef struct audiosink_object
{
    ApObject base;
    ApPad *sink_pad;

    int channels;
    char *card_name;

    char *buffer;
    rt_thread_t loop;
    struct rt_device *card;
    struct AUDIO_PARAMS param;
    struct audio_buf abuf;
    uint32_t bit_width;
    uint32_t sink_map;
    uint32_t default_map;
} ApAudioSink;

static int audiosink_process(ApPad *pad, ApBuffer *buffer);

static int audiosink_init(ApObject *obj, void *arg)
{
    ApAudioSink *audsink = AUDIOSINK(obj);

    int status = RT_EOK;
    Aupipe *aupipe = obj->parent;
    char *parameters = (char *)arg;

    status |= aupipe_find_property(parameters, "card",
                                   VALUE_TYPE_STRING, &audsink->card_name);
    status |= aupipe_find_property(parameters, "channels",
                                   VALUE_TYPE_INT, &audsink->channels);
    if (status != RT_EOK)
    {
        LOG_E("%s plug-in parameter initialization failed", audsink->base.name);
        return -RT_ERROR;
    }

    audsink->bit_width = aupipe->bits >> 3;
    audsink->sink_map = (1 << audsink->channels) - 1;
    audsink->buffer = rt_malloc(aupipe->period_size * audsink->channels * audsink->bit_width);

    audsink->sink_pad = aupipe_pad_new(obj, audsink->channels);
    RT_ASSERT(audsink->sink_pad != NULL);
    for (int i = 0; i < audsink->channels; i++)
    {
        audsink->sink_pad[i].process = audiosink_process;
    }

    return RT_EOK;
}

static int audiosink_deinit(ApObject *obj)
{
    ApAudioSink *audsink = AUDIOSINK(obj);

    rt_free(audsink->buffer);
    rt_free(audsink->sink_pad);
    rt_free(audsink->card_name);

    return RT_EOK;
}

static int audiosink_process(ApPad *pad, ApBuffer *buffer)
{
    ApAudioSink *obj = AUDIOSINK(pad->parent);

    int ret = RT_EOK;
    short *out = (short *)obj->buffer;

    if (buffer->type != DATA_TYPE_AUDIO)
    {
        aupipe_buffer_unref(buffer);
        return -RT_ERROR;
    }

    for (int i = 0; i < obj->abuf.period_size; i++)
    {
        rt_memcpy(obj->buffer + (obj->param.channels * i + pad->id) * obj->bit_width,
                  buffer->data + obj->bit_width * i, obj->bit_width);
    }
    aupipe_buffer_unref(buffer);

    obj->sink_map = obj->sink_map & (~(1 << pad->id));
    if (obj->sink_map != 0)
    {
        return ret;
    }
    obj->sink_map = (1 << obj->channels) - 1;

    if (!rt_device_write(obj->card, 0, out, obj->abuf.period_size))
    {
        rt_kprintf("Error playing sample\n");
        ret = -RT_ERROR;
    }

    return ret;
}

static int audiosink_open_card(ApAudioSink *obj)
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

    ret = rt_device_open(obj->card, RT_DEVICE_OFLAG_WRONLY);
    if (ret < 0)
    {
        LOG_E("Failed to open %s, err: %d\n", obj->card->parent.name, ret);
        goto open_failed;
    }

    abuf->period_size = aupipe->period_size;
    abuf->buf_size = aupipe->period_size * period_count;
    size = abuf->buf_size * obj->channels * (aupipe->bits >> 3); /* frames to bytes */
    abuf->buf = rt_malloc_uncache(size);
    if (!abuf->buf)
    {
        LOG_E("Buffer alloc failed!\n");
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

static int audiosink_close_card(ApAudioSink *obj)
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

static int audiosink_set_state(ApObject *obj, int state)
{
    ApAudioSink *audsink = AUDIOSINK(obj);
    switch (state)
    {
        case STATE_NULL_TO_READY:
            LOG_I("STATE_NULL_TO_READY");
            if (audiosink_open_card(audsink) != RT_EOK)
                return -RT_ERROR;
            break;
        case STATE_READY_TO_NULL:
            LOG_I("STATE_READY_TO_NULL");
            audiosink_close_card(audsink);
            break;
        default:
            break;
    }

    return RT_EOK;
}

static int audiosink_set_property(ApObject *obj, char *name, void *arg)
{
    return RT_EOK;
}

static int audiosink_get_property(ApObject *obj, char *name, void *arg)
{
    return RT_EOK;
}

static ApPad *audiosink_get_pad(ApObject *obj, int type, int id)
{
    ApAudioSink *audsink = AUDIOSINK(obj);

    if (type == PAD_TYPE_SRC)
        return NULL;

    return &audsink->sink_pad[id];
}

OBJECT_BASE_DEFINE(audiosink, ApAudioSink);

#endif

