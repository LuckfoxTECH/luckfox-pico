/**
  * Copyright (c) 2023 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    filesrc.c
  * @author  Jun Zeng
  * @version V0.1
  * @date    10-03-2023
  * @brief   filesrc for rksoc
  *
  ******************************************************************************
  */

#include "aupipe.h"

#ifdef RT_USING_AUPIPE_FILESRC

#include "drv_heap.h"
#include "rk_audio.h"

#undef DBG_SECTION_NAME
#define DBG_SECTION_NAME    "FILESRC"

#define ID_RIFF                    0x46464952
#define ID_WAVE                    0x45564157
#define ID_FMT                     0x20746d66
#define ID_DATA                    0x61746164

#define FILESRC(obj)               ((ApFileSrc *)(obj))

typedef struct riff_chunk
{
    uint32_t chunk_id;
    uint32_t chunk_size;
    uint32_t format;
} RiffChunk;

typedef struct sub_chunk
{
    uint32_t chunk_id;
    uint32_t chunk_size;
} SubChunk;

typedef struct fmt_chunk
{
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
} FmtChunk;

typedef struct filesrc_object
{
    ApObject base;
    ApPad *src_pad;

    int offset;
    int channels;
    int samplebits;
    int samplerate;

    rt_sem_t sem;
    rt_thread_t loop;
    int loop_running;

    FILE *fp;
    SubChunk sub_chunk;
    FmtChunk fmt_chunk;
    RiffChunk riff_chunk;
} ApFileSrc;

static int filesrc_init(ApObject *obj, void *arg)
{
    ApFileSrc *object = FILESRC(obj);

    int status = 0;
    int more_chunks = 1;

    char *filename = NULL;
    char *parameters = (char *)arg;

    Aupipe *aupipe = obj->parent;

    status |= aupipe_find_property(parameters, "filename",
                                   VALUE_TYPE_STRING, &filename);
    status |= aupipe_find_property(parameters, "channels",
                                   VALUE_TYPE_INT, &object->channels);
    if (status != RT_EOK)
    {
        LOG_E("%s plug-in parameter initialization failed", object->base.name);
        return -RT_ERROR;
    }

    object->fp = fopen(filename, "rb");
    if (!object->fp)
    {
        rt_kprintf("Unable to open file %s\n", filename);
        return -RT_ERROR;
    }

    fread(&object->riff_chunk, sizeof(RiffChunk), 1, object->fp);
    if ((object->riff_chunk.chunk_id != ID_RIFF) ||
            (object->riff_chunk.format != ID_WAVE))
    {
        object->offset = 0;

        fseek(object->fp, 0, SEEK_SET);

        object->samplebits = aupipe->bits;
        object->samplerate = aupipe->rate;
    }
    else
    {
        do
        {
            fread(&object->sub_chunk, sizeof(SubChunk), 1, object->fp);

            switch (object->sub_chunk.chunk_id)
            {
            case ID_FMT:
                fread(&object->fmt_chunk, sizeof(FmtChunk), 1, object->fp);
                /* If the format header is larger, skip the rest */
                if (object->sub_chunk.chunk_size > sizeof(FmtChunk))
                    fseek(object->fp, object->sub_chunk.chunk_size - sizeof(FmtChunk), SEEK_CUR);
                break;
            case ID_DATA:
                /* Stop looking for chunks */
                more_chunks = 0;
                break;
            default:
                /* Unknown chunk, skip bytes */
                fseek(object->fp, object->sub_chunk.chunk_size, SEEK_CUR);
            }
        }
        while (more_chunks);

        object->offset = ftell(object->fp);

        object->channels = object->fmt_chunk.num_channels;
        object->samplerate = object->fmt_chunk.sample_rate;
        if (object->samplerate != aupipe->rate)
        {
            LOG_E("Music file samplerate does not match the aupipe samplerate");
            return -RT_ERROR;
        }
        object->samplebits = object->fmt_chunk.bits_per_sample;
        if (object->samplebits != aupipe->bits)
        {
            LOG_E("Music file samplebits does not match the aupipe samplebits");
            return -RT_ERROR;
        }
    }

    object->src_pad = aupipe_pad_new(obj, object->channels);
    RT_ASSERT(object->src_pad != NULL);

    object->sem = rt_sem_create("filesrc", 0, RT_IPC_FLAG_FIFO);
    RT_ASSERT(object->sem != NULL);

    rt_free(filename);

    return RT_EOK;
}

static int filesrc_deinit(ApObject *obj)
{
    ApFileSrc *object = FILESRC(obj);

    fclose(object->fp);

    rt_free(object->src_pad);

    rt_sem_delete(object->sem);

    return RT_EOK;
}

static void do_playfile(void *arg)
{
    ApFileSrc *obj = FILESRC(arg);

    Aupipe *aupipe = obj->base.parent;

    int read_byte = 0;
    int bit_width = obj->samplebits >> 3;
    int period_byte = aupipe->period_size * bit_width;
    int total_period_byte = period_byte * obj->channels;

    char *file_buffer = rt_malloc(total_period_byte);

    while (obj->loop_running)
    {
        read_byte = fread(file_buffer, 1, total_period_byte, obj->fp);
        if (read_byte < total_period_byte)
        {
            memset(file_buffer + read_byte, 0x00, (total_period_byte - read_byte));

            fseek(obj->fp, obj->offset, SEEK_SET);
        }

        for (int i = 0; i < obj->channels; i++)
        {
            if (obj->src_pad[i].peer == RT_NULL)
                continue;

            ApBuffer *buffer = aupipe_buffer_new(DATA_TYPE_AUDIO, period_byte);
            if (!buffer)
            {
                LOG_E("%s malloc %d failed", obj->base.name, period_byte);
                break;
            }

            for (int j = 0; j < aupipe->period_size; j++)
            {
                rt_memcpy(buffer->data + bit_width * j, file_buffer +
                          bit_width * (obj->channels * j + i), bit_width);
            }

            buffer = aupipe_buffer_ref(buffer);
            aupipe_pad_push(&obj->src_pad[i], buffer);
        }
    }
    rt_free(file_buffer);

    rt_sem_release(obj->sem);
    obj->loop_running = 0;
}

static int filesrc_set_state(ApObject *obj, int state)
{
    ApFileSrc *object = FILESRC(obj);

    switch (state)
    {
    case STATE_NULL_TO_READY:
        LOG_I("STATE_NULL_TO_READY");
        object->loop = rt_thread_create("filesrc", do_playfile, object,
                                        4096, RT_THREAD_PRIORITY_MAX / 2, 10);
        if (!object->loop)
        {
            LOG_E("do_playfile thread create failed");
            return -RT_ERROR;
        }
        break;
    case STATE_PAUSED_TO_PLAYING:
        LOG_I("STATE_PAUSED_TO_PLAYING");
        object->loop_running = 1;
        rt_thread_startup(object->loop);
        break;
    case STATE_PLAYING_TO_PAUSED:
        LOG_I("STATE_PLAYING_TO_PAUSED");
        if (object->loop_running == 1)
        {
            /* loop still running */
            object->loop_running = 0;
            if (rt_sem_take(object->sem, 3000))
            {
                /* Timeout, force delete */
                LOG_W("Timeout");
                rt_thread_delete(object->loop);
            }
        }
        break;
    case STATE_READY_TO_NULL:
        LOG_I("STATE_READY_TO_NULL");
        break;
    default:
        break;
    }

    return RT_EOK;
}

static int filesrc_set_property(ApObject *obj, char *name, void *arg)
{
    return RT_EOK;
}

static int filesrc_get_property(ApObject *obj, char *name, void *arg)
{
    return RT_EOK;
}

static ApPad *filesrc_get_pad(ApObject *obj, int type, int id)
{
    ApFileSrc *object = FILESRC(obj);

    if (type == PAD_TYPE_SINK)
    {
        LOG_E("%s plug-in has no sink pad", obj->name);
        return NULL;
    }

    return &object->src_pad[id];
}

OBJECT_BASE_DEFINE(filesrc, ApFileSrc);

#endif

