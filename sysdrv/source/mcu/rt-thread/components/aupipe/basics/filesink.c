/**
  * Copyright (c) 2023 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    filesink.c
  * @author  Jun Zeng
  * @version V0.1
  * @date    10-03-2023
  * @brief   filesink for rksoc
  *
  ******************************************************************************
  */

#include "aupipe.h"

#ifdef RT_USING_AUPIPE_FILESINK

#include "drv_heap.h"
#include "rk_audio.h"

#undef DBG_SECTION_NAME
#define DBG_SECTION_NAME    "FILESINK"

#define FILESINK(obj)               ((ApFileSink *)(obj))

typedef struct filesink_object
{
    ApObject base;
    ApPad *sink_pad;

    int channels;
    int bit_width;
    int period_byte;
    int sinkpad_map;

    FILE *fp;
    char *buffer;
    rt_thread_t loop;
} ApFileSink;

static int filesink_process(ApPad *pad, ApBuffer *buffer);

static int filesink_init(ApObject *obj, void *arg)
{
    ApFileSink *object = FILESINK(obj);

    int status = 0;

    char *filename = NULL;
    char *parameters = (char *)arg;

    Aupipe *aupipe = object->base.parent;

    status |= aupipe_find_property(parameters, "filename",
                                   VALUE_TYPE_STRING, &filename);
    status |= aupipe_find_property(parameters, "channels",
                                   VALUE_TYPE_INT, &object->channels);
    if (status != RT_EOK)
    {
        LOG_E("%s plug-in parameter initialization failed", object->base.name);
        return -RT_ERROR;
    }

    object->bit_width = aupipe->bits >> 3;
    object->sinkpad_map = (1 << object->channels) - 1;

    object->fp = fopen(filename, "wb");
    if (!object->fp)
    {
        rt_kprintf("Unable to open file %s\n", filename);
        return RT_EOK;
    }

    object->period_byte = aupipe->period_size * object->channels * object->bit_width;
    object->buffer = rt_malloc(object->period_byte);
    if (object->buffer == NULL)
    {
        LOG_E("%s malloc %d failed", object->base.name, object->period_byte);
        return -RT_ERROR;
    }

    object->sink_pad = aupipe_pad_new(obj, object->channels);
    RT_ASSERT(object->sink_pad != NULL);
    for (int i = 0; i < object->channels; i++)
    {
        object->sink_pad[i].process = filesink_process;
    }

    return RT_EOK;
}

static int filesink_deinit(ApObject *obj)
{
    ApFileSink *object = FILESINK(obj);

    fclose(object->fp);

    rt_free(object->buffer);
    rt_free(object->sink_pad);

    return RT_EOK;
}

static int filesink_process(ApPad *pad, ApBuffer *buffer)
{
    ApFileSink *obj = FILESINK(pad->parent);

    int status = RT_EOK;
    short *out = (short *)obj->buffer;
    Aupipe *aupipe = obj->base.parent;

    if (buffer->type != DATA_TYPE_AUDIO)
    {
        aupipe_buffer_unref(buffer);
        return -RT_ERROR;
    }

    for (int i = 0; i < aupipe->period_size; i++)
    {
        rt_memcpy(obj->buffer + (obj->channels * i + pad->id) * obj->bit_width,
                  buffer->data + obj->bit_width * i, obj->bit_width);
    }
    aupipe_buffer_unref(buffer);

    obj->sinkpad_map &= ~(1 << pad->id);

    if (obj->sinkpad_map == 0)
    {
        obj->sinkpad_map = (1 << obj->channels) - 1;
    }
    else
    {
        return status;
    }

    fwrite(obj->buffer, obj->period_byte, 1, obj->fp);

    return status;
}

static int filesink_set_state(ApObject *obj, int state)
{
    ApFileSink *object = FILESINK(obj);

    switch (state)
    {
    case STATE_NULL_TO_READY:
        LOG_I("STATE_NULL_TO_READY");
        break;
    case STATE_READY_TO_NULL:
        LOG_I("STATE_READY_TO_NULL");
        break;
    default:
        break;
    }

    return RT_EOK;
}

static int filesink_set_property(ApObject *obj, char *name, void *arg)
{
    return RT_EOK;
}

static int filesink_get_property(ApObject *obj, char *name, void *arg)
{
    return RT_EOK;
}

static ApPad *filesink_get_pad(ApObject *obj, int type, int id)
{
    ApFileSink *object = FILESINK(obj);

    if (type == PAD_TYPE_SRC)
        return NULL;

    return &object->sink_pad[id];
}

OBJECT_BASE_DEFINE(filesink, ApFileSink);

#endif

