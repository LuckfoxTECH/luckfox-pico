/*
 * Copyright (c) 2019 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-12-13     Jair Wu      First version
 *
 */

#include <rtthread.h>

#if defined(ENABLE_DUAL_TRACKING) && defined(RT_USING_AUDIO_SERVER)
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "audio_server.h"
#include "dual_track.h"
#include "rk_audio.h"
#include "drv_heap.h"
#include "rthw.h"

#define DBG_LEVEL         DBG_INFO
#define DBG_SECTION_NAME    "DTRACK"
#include "rtdbg.h"

struct dt_recorder
{
    struct audio_player_queue *write_queue;
    struct audio_player_queue *encode_queue;
    struct audio_player_queue *record_queue;

    struct audio_player_stream *encode_stream_a;
    struct audio_player_stream *encode_stream_d;
    struct audio_player_stream *record_stream_a;
    struct audio_player_stream *record_stream_d;

    audio_player_mutex_handle state_lock;
    audio_player_semaphore_handle pause_sem;

    audio_player_thread_handle write_task;
    audio_player_thread_handle encode_task;
    audio_player_thread_handle record_task;

    record_writer_t writer;

    const char *target;
    const char *tag;

    void *userdata;

    const char *device_name;
    capture_device_t device;

    int samplerate;
    int bits;
    int channels;
};

void dt_writer_run(void *arg)
{
    dt_recorder_handle_t dt_recorder = (dt_recorder_handle_t)arg;
    struct wav_header m_wav_header;
    media_sdk_msg_t msg;
    record_writer_t writer_a;
    record_writer_t writer_d;
    record_writer_cfg_t processor_cfg;
    char *read_buf;
    size_t read_size = 0;
    size_t frame_size = 0;
    size_t total_byte;
    char target_a[128];
    char target_d[128];
    int is_wav;
    int res;
    while (1)
    {
        LOG_I("%s wait for msg", __func__);
        if (audio_queue_receive(dt_recorder->write_queue, &msg) == -1)
        {
            LOG_I("write_run exit");
            return;
        }
        is_wav = 0;
        total_byte = 0;
        writer_a = writer_d = msg.recorder.writer;
        memset(target_a, 0x0, 128);
        memset(target_d, 0x0, 128);
        int pos = strrchr(msg.recorder.target, '.') - msg.recorder.target;
        strcat(target_a, msg.recorder.target);
        strcat(target_d, msg.recorder.target);
        target_a[pos] = '\0';
        target_d[pos] = '\0';
        strcat(target_a, "_a.");
        strcat(target_d, "_d.");
        strcat(target_a, msg.recorder.type);
        strcat(target_d, msg.recorder.type);
        processor_cfg.target = target_a;
        processor_cfg.tag = dt_recorder->tag;
        res = writer_a.init(&writer_a, &processor_cfg);
        if (res)
        {
            LOG_E("writer_a init failed %d\n", res);
            audio_stream_finish(dt_recorder->encode_stream_a);
            audio_stream_finish(dt_recorder->encode_stream_d);
            audio_stream_finish(dt_recorder->record_stream_a);
            audio_stream_finish(dt_recorder->record_stream_d);
            goto WRITE_OUT;
        }
        processor_cfg.target = target_d;
        res = writer_d.init(&writer_d, &processor_cfg);
        if (res)
        {
            LOG_E("writer_d init failed %d\n", res);
            audio_stream_finish(dt_recorder->encode_stream_a);
            audio_stream_finish(dt_recorder->encode_stream_d);
            audio_stream_finish(dt_recorder->record_stream_a);
            audio_stream_finish(dt_recorder->record_stream_d);
            writer_a.destroy(&writer_a);
            goto WRITE_OUT;
        }
        if (!strcmp(msg.recorder.type, "wav"))
        {
            is_wav = 1;
            wav_header_init(&m_wav_header, dt_recorder->samplerate, dt_recorder->bits, dt_recorder->channels);
            writer_a.write(&writer_a, (char *)&m_wav_header, sizeof(m_wav_header));
            writer_d.write(&writer_d, (char *)&m_wav_header, sizeof(m_wav_header));
        }
        processor_cfg.frame_size = 64 * 1024;
        frame_size = processor_cfg.frame_size;
        read_buf = rt_malloc(frame_size);
        if (!read_buf)
        {
            LOG_E("read_buf mallic failed %d\n", frame_size);
            audio_stream_finish(dt_recorder->encode_stream_a);
            audio_stream_finish(dt_recorder->encode_stream_d);
            audio_stream_finish(dt_recorder->record_stream_a);
            audio_stream_finish(dt_recorder->record_stream_d);
            goto WRITE_OUT1;
        }
        while (1)
        {
            read_size = audio_stream_read(dt_recorder->encode_stream_a, read_buf, frame_size);
            if (read_size != frame_size)
            {
                LOG_I("encode_stream_a was stopped");
                goto WRITE_OUT2;
            }
            if (writer_a.write(&writer_a, read_buf, read_size) != read_size)
            {
                LOG_E("writer_a error");
                goto ERROR_OUT;
            }
            read_size = audio_stream_read(dt_recorder->encode_stream_d, read_buf, frame_size);
            if (read_size != frame_size)
            {
                LOG_I("encode_stream_d was stopped");
                goto WRITE_OUT2;
            }
            if (writer_d.write(&writer_d, read_buf, read_size) != read_size)
            {
                LOG_E("writer_d error");
                goto ERROR_OUT;
            }
            total_byte += read_size;
            rt_thread_mdelay(1);
        }
ERROR_OUT:
        audio_stream_finish(dt_recorder->encode_stream_a);
        audio_stream_finish(dt_recorder->encode_stream_d);
        audio_stream_finish(dt_recorder->record_stream_a);
        audio_stream_finish(dt_recorder->record_stream_d);
WRITE_OUT2:
        rt_free(read_buf);
WRITE_OUT1:
        if (is_wav)
        {
            wav_header_complete(&m_wav_header, total_byte);
            writer_a.userdata = (void *)&m_wav_header;
            writer_d.userdata = (void *)&m_wav_header;
            writer_a.write(&writer_a, (char *)&m_wav_header, sizeof(m_wav_header));
            writer_d.write(&writer_d, (char *)&m_wav_header, sizeof(m_wav_header));
        }
        writer_d.destroy(&writer_d);
        writer_a.destroy(&writer_a);
WRITE_OUT:
        LOG_I("writer out");
        continue;
    }
}

void dt_encoder_run(void *arg)
{
    dt_recorder_handle_t dt_recorder = (dt_recorder_handle_t)arg;
    media_sdk_msg_t msg;
    int32_t read_bytes;
    int32_t write_bytes;
    int32_t frame_size;
    char *buffer;
    int is_wav;

    while (1)
    {
        LOG_I("%s wait for msg", __func__);
        if (audio_queue_receive(dt_recorder->encode_queue, &msg) == -1)
        {
            LOG_I("write_run exit");
            return;
        }
        if (strcmp(msg.recorder.type, "wav"))
        {
            LOG_E("not support this type %s.\n", msg.recorder.type);
            is_wav = 0;
            continue;
        }
        else
        {
            frame_size = 4096;
            is_wav = 1;
        }
        buffer = rt_malloc(frame_size);
        audio_stream_start(dt_recorder->encode_stream_a);
        audio_stream_start(dt_recorder->encode_stream_d);
        audio_queue_send(dt_recorder->write_queue, &msg);
        while (1)
        {
            read_bytes = audio_stream_read(dt_recorder->record_stream_a,
                                        buffer,
                                        frame_size);
            if (read_bytes != frame_size)
            {
                LOG_I("encoder input error");
                audio_stream_stop(dt_recorder->encode_stream_a);
                audio_stream_stop(dt_recorder->encode_stream_d);
                break;
            }
            if (!is_wav)
            {
                // encode here
            }
            else
            {
                write_bytes = audio_stream_write(dt_recorder->encode_stream_a,
                                              buffer,
                                              frame_size);
            }
            if (write_bytes == -1)
            {
                LOG_E("encode output failed");
                audio_stream_stop(dt_recorder->record_stream_a);
                audio_stream_stop(dt_recorder->record_stream_d);
                break;
            }
            read_bytes = audio_stream_read(dt_recorder->record_stream_d,
                                        buffer,
                                        frame_size);
            if (read_bytes != frame_size)
            {
                LOG_I("encoder input error");
                audio_stream_stop(dt_recorder->encode_stream_a);
                audio_stream_stop(dt_recorder->encode_stream_d);
                break;
            }
            if (!is_wav)
            {
                // encode here
            }
            else
            {
                write_bytes = audio_stream_write(dt_recorder->encode_stream_d,
                                              buffer,
                                              frame_size);
            }
            if (write_bytes == -1)
            {
                LOG_E("encode output failed");
                audio_stream_stop(dt_recorder->record_stream_a);
                audio_stream_stop(dt_recorder->record_stream_d);
                break;
            }
        }
    }
}

void dt_capture_run(void *arg)
{
    dt_recorder_handle_t dt_recorder = (dt_recorder_handle_t)arg;
    rt_device_t audio_dev_a;
    rt_device_t audio_dev_d;
    char *audio_buf_a;
    char *audio_buf_d;
    size_t frame_size;
    int write_size1, write_size2;
    media_sdk_msg_t msg;
    struct AUDIO_PARAMS param;
    struct audio_buf abuf_a;
    struct audio_buf abuf_d;
    uint32_t size;
    int ret;
    while (1)
    {
        LOG_I("%s wait for msg", __func__);
        if (audio_queue_receive(dt_recorder->record_queue, &msg) == -1)
        {
            LOG_E("capture_run receive data failed");
            return;
        }
        audio_dev_a = rt_device_find("adcc");
        if (!audio_dev_a)
        {
            LOG_E("cannot find card 1\n");
            continue;
        }
        audio_dev_d = rt_device_find("pdmc");
        if (!audio_dev_d)
        {
            LOG_E("cannot find card 2\n");
            continue;
        }
        ret = rt_device_open(audio_dev_a, RT_DEVICE_OFLAG_RDONLY);
        RT_ASSERT(ret == RT_EOK);
        ret = rt_device_open(audio_dev_d, RT_DEVICE_OFLAG_RDONLY);
        RT_ASSERT(ret == RT_EOK);

        abuf_d.period_size = abuf_a.period_size = 1024;
        abuf_d.buf_size = abuf_a.buf_size = 1024 * 4;
        size = abuf_a.buf_size * dt_recorder->channels * (dt_recorder->bits >> 3); /* frames to bytes */
        frame_size = abuf_a.period_size * dt_recorder->channels * (dt_recorder->bits >> 3);
        abuf_a.buf = rt_malloc_uncache(size);
        if (abuf_a.buf == NULL)
        {
            LOG_E("malloc uncache %d failed, increase RT_UNCACHE_HEAP_ORDER");
            rt_device_close(audio_dev_a);
            rt_device_close(audio_dev_d);
            continue;
        }
        abuf_d.buf = rt_malloc_uncache(size);
        if (abuf_d.buf == NULL)
        {
            LOG_E("malloc uncache %d failed, increase RT_UNCACHE_HEAP_ORDER");
            rt_free_uncache(abuf_a.buf);
            rt_device_close(audio_dev_a);
            rt_device_close(audio_dev_d);
            continue;
        }

        param.channels = dt_recorder->channels;
        param.sampleRate = dt_recorder->samplerate;
        param.sampleBits = dt_recorder->bits;

        ret = rt_device_control(audio_dev_a, RK_AUDIO_CTL_PCM_PREPARE, &abuf_a);
        RT_ASSERT(ret == RT_EOK);
        ret = rt_device_control(audio_dev_d, RK_AUDIO_CTL_PCM_PREPARE, &abuf_d);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(audio_dev_a, RK_AUDIO_CTL_HW_PARAMS, &param);
        RT_ASSERT(ret == RT_EOK);
        ret = rt_device_control(audio_dev_d, RK_AUDIO_CTL_HW_PARAMS, &param);
        RT_ASSERT(ret == RT_EOK);

        audio_buf_a = rt_malloc(size / 4);
        if (!audio_buf_a)
        {
            LOG_E("audio_buf_a malloc failed");
            rt_free_uncache(abuf_a.buf);
            rt_free_uncache(abuf_d.buf);
            rt_device_close(audio_dev_a);
            rt_device_close(audio_dev_d);
            continue;
        }
        audio_buf_d = rt_malloc(size / 4);
        if (!audio_buf_d)
        {
            LOG_E("audio_buf_a malloc failed");
            rt_free(audio_buf_a);
            rt_free_uncache(abuf_a.buf);
            rt_free_uncache(abuf_d.buf);
            rt_device_close(audio_dev_a);
            rt_device_close(audio_dev_d);
            rt_free(audio_buf_a);
            continue;
        }
        audio_stream_start(dt_recorder->record_stream_a);
        audio_stream_start(dt_recorder->record_stream_d);
        audio_queue_send(dt_recorder->encode_queue, &msg);
        while (1)
        {
            rt_device_read(audio_dev_a, 0, audio_buf_a, abuf_a.period_size);
            rt_device_read(audio_dev_d, 0, audio_buf_d, abuf_d.period_size);
            write_size1 = audio_stream_write(dt_recorder->record_stream_a,
                                          audio_buf_a, frame_size);
            write_size2 = audio_stream_write(dt_recorder->record_stream_d,
                                          audio_buf_d, frame_size);
            if (write_size1 != frame_size || write_size2 != frame_size)
                break;
        }
        LOG_I("capture end");
        rt_free(audio_buf_a);
        rt_free(audio_buf_d);

        ret = rt_device_control(audio_dev_a, RK_AUDIO_CTL_STOP, NULL);
        RT_ASSERT(ret == RT_EOK);
        ret = rt_device_control(audio_dev_d, RK_AUDIO_CTL_STOP, NULL);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(audio_dev_a, RK_AUDIO_CTL_PCM_RELEASE, NULL);
        RT_ASSERT(ret == RT_EOK);
        ret = rt_device_control(audio_dev_d, RK_AUDIO_CTL_PCM_RELEASE, NULL);
        RT_ASSERT(ret == RT_EOK);

        rt_free_uncache(abuf_a.buf);
        rt_free_uncache(abuf_d.buf);
        rt_device_close(audio_dev_a);
        rt_device_close(audio_dev_d);
    }
    LOG_I("capture out");
}

dt_recorder_handle_t dt_capture_create(recorder_cfg_t *cfg)
{
    dt_recorder_handle_t dt_recorder = (dt_recorder_handle_t) rt_malloc(sizeof(*dt_recorder));
    LOG_I("dt_capture_create in");
    if (dt_recorder)
    {
        dt_recorder->write_queue = audio_queue_create(1, sizeof(media_sdk_msg_t));
        dt_recorder->encode_queue = audio_queue_create(1, sizeof(media_sdk_msg_t));
        dt_recorder->record_queue = audio_queue_create(1, sizeof(media_sdk_msg_t));
        dt_recorder->record_stream_a = audio_stream_create(cfg->record_buf_size);
        dt_recorder->encode_stream_a = audio_stream_create(cfg->encode_buf_size);
        dt_recorder->record_stream_d = audio_stream_create(cfg->record_buf_size);
        dt_recorder->encode_stream_d = audio_stream_create(cfg->encode_buf_size);
        dt_recorder->state_lock = rt_mutex_create("dt_state", RT_IPC_FLAG_FIFO);
        dt_recorder->pause_sem = rt_sem_create("dt_pause", 1, RT_IPC_FLAG_FIFO);
        dt_recorder->tag = cfg->tag;
        dt_recorder->userdata = cfg->userdata;

        dt_recorder->write_task = rt_thread_create("dt_writer", dt_writer_run, dt_recorder, 2048, 14, 10);
        if (dt_recorder->write_task)
            rt_thread_startup(dt_recorder->write_task);
        dt_recorder->encode_task = rt_thread_create("dt_encoder", dt_encoder_run, dt_recorder, 8192, 14, 10);
        if (dt_recorder->encode_task)
            rt_thread_startup(dt_recorder->encode_task);
        dt_recorder->record_task = rt_thread_create("dt_recorder", dt_capture_run, dt_recorder, 2048, 14, 10);
        if (dt_recorder->record_task)
            rt_thread_startup(dt_recorder->record_task);
    }
    LOG_I("dt_capture_create out");
    return dt_recorder;
}

void dt_capture_destroy(dt_recorder_handle_t self)
{
    dt_recorder_handle_t dt_recorder = self;
    LOG_I("dt_capture_destory in.");
    if (dt_recorder)
    {
        audio_queue_destroy(dt_recorder->write_queue);
        audio_queue_destroy(dt_recorder->encode_queue);
        audio_queue_destroy(dt_recorder->record_queue);
        audio_stream_destroy(dt_recorder->record_stream_a);
        audio_stream_destroy(dt_recorder->encode_stream_a);
        audio_stream_destroy(dt_recorder->record_stream_d);
        audio_stream_destroy(dt_recorder->encode_stream_d);
        rt_mutex_delete(dt_recorder->state_lock);
        rt_sem_delete(dt_recorder->pause_sem);
        rt_thread_delete(dt_recorder->write_task);
        rt_thread_delete(dt_recorder->encode_task);
        rt_thread_delete(dt_recorder->record_task);
        rt_free(dt_recorder);
        dt_recorder = NULL;
    }
}

void dt_capture_start(dt_recorder_handle_t self, record_cfg_t *cfg)
{
    LOG_I("dt_capture_start start type %s", cfg->type);

    dt_recorder_handle_t dt_recorder = self;
    media_sdk_msg_t msg;

    dt_recorder->samplerate = cfg->samplerate;
    dt_recorder->bits = cfg->bits;
    dt_recorder->channels = cfg->channels;
    msg.type = CMD_RECORDER_START;
    msg.recorder.mode = RECORD_MODE_PROMPT;
    msg.recorder.target = cfg->target;
    msg.recorder.type = cfg->type;
    msg.recorder.writer = cfg->writer;
    msg.recorder.need_free = cfg->need_free;
    msg.recorder.end_session = RT_FALSE;
    audio_queue_send(dt_recorder->record_queue, &msg);
}

void dt_capture_stop(dt_recorder_handle_t self)
{
    audio_stream_stop(self->record_stream_a);
    audio_stream_stop(self->record_stream_d);
}
#endif
