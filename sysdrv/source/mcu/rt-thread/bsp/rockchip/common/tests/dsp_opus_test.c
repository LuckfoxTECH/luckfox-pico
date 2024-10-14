/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    dsp_opus_test.c
  * @author  jair wu
  * @version V0.1
  * @date    08-Apr-2020
  * @brief   dsp opus codec test
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rthw.h>
#include <rtthread.h>

#ifdef RT_USING_DSP_OPUS_CODEC_TEST

#include <stdint.h>
#include "drv_cache.h"
#include "drv_dsp.h"
#include "drv_heap.h"
#include "hal_base.h"
#include "rk_audio.h"

#define AUDIO_SERVER_DATA_SHARE_MAX     2
#define OPUS_SAMPLE_RATE                48000
#define OPUS_BITS                       16
#define OPUS_CHANNELS                   4
#define OPUS_BITRATE                    128000

#define DSP_CODEC_OPUS_ENCODE_OPEN      0x50000001
#define DSP_CODEC_OPUS_ENCODE_PROCESS   0x50000002
#define DSP_CODEC_OPUS_ENCODE_FINISH    0x50000003
#define DSP_CODEC_OPUS_ENCODE_CLOSE     0x50000004
#define DSP_CODEC_OPUS_DECODE_OPEN      0x50000005
#define DSP_CODEC_OPUS_DECODE_PROCESS   0x50000006
#define DSP_CODEC_OPUS_DECODE_FINISH    0x50000007
#define DSP_CODEC_OPUS_DECODE_CLOSE     0x50000008

struct share_data
{
    size_t type;
    size_t dat[AUDIO_SERVER_DATA_SHARE_MAX];
} g_ABCoreDat;

struct audio_config
{
    int sample_rate;
    int bits;
    int channels;
    int32_t bitrate;
};

struct opus_info
{
    int max_packets;
    int comment_num;
    uint32_t comments;
};

typedef struct _opus_enc_s
{
    void    *_encoder;
    void    *comments;
    int     sample_rate;
    int     channels;
    int     frame_size;
    short   *_buffer_in;
    short   *_buffer_header;
    short   *_buffer_out;
    int     header_size;
    int     enc_size;
    int     drain_flag;
    int     packets;
    int     max_packets;

    struct share_data *share_dat;
} opus_enc_t;

struct _opus_reader
{
    void * or;
    char name[128];
    void *(*open)(char *name);
    uint32_t (*read)(void * or, char *buf, size_t size, size_t count);
    void (*close)(void * or);
};

static struct _opus_reader opus_reader;
static opus_enc_t *g_opus;
static struct rt_device *g_dsp_handle;
static struct dsp_work *g_dsp_work;
static char file_path[64];
static uint32_t duration;
static int show_cycles;
static struct audio_buf abuf;
static int g_rate;
static int g_ch;
static int g_packet;

static void *file_open(char *path)
{
    FILE *fd;
    fd = fopen(path, "rb");
    return fd;
}

static uint32_t file_read(void *fd, char *buf, size_t size, size_t count)
{
    return fread(buf, size, count, (FILE *)fd);
}

static void file_close(void *fd)
{
    fclose((FILE *)fd);
}

static void *card_open(char *name)
{
    struct rt_device *card;
    struct AUDIO_PARAMS param;
    size_t size;
    int ret;

    card = rt_device_find(name);
    if (!card)
    {
        rt_kprintf("Cannot find %s\n", name);
        return NULL;
    }

    rt_device_open(card, RT_DEVICE_OFLAG_RDONLY);
    abuf.period_size = g_opus->frame_size;
    abuf.buf_size = g_opus->frame_size * 4;
    size = abuf.buf_size * OPUS_CHANNELS * (OPUS_BITS >> 3); /* frames to bytes */
    abuf.buf = rt_malloc_uncache(size);

    param.channels = g_ch;
    param.sampleRate = g_rate;
    param.sampleBits = OPUS_BITS;

    ret = rt_device_control(card, RK_AUDIO_CTL_PCM_PREPARE, &abuf);
    RT_ASSERT(ret == RT_EOK);
    ret = rt_device_control(card, RK_AUDIO_CTL_HW_PARAMS, &param);
    RT_ASSERT(ret == RT_EOK);

    return card;
}

static uint32_t card_read(void *card, char *buf, size_t size, size_t count)
{
    return rt_device_read(card, 0, buf, count);
}

static void card_close(void *card)
{
    int ret;

    ret = rt_device_control(card, RK_AUDIO_CTL_STOP, NULL);
    RT_ASSERT(ret == RT_EOK);
    ret = rt_device_control(card, RK_AUDIO_CTL_PCM_RELEASE, NULL);
    RT_ASSERT(ret == RT_EOK);
    rt_device_close(card);
    rt_free_uncache(abuf.buf);
}

static void do_opus_encode(void *arg)
{
    struct audio_config opus_cfg;
    struct opus_info *info;
    char **comments;
    uint32_t size;
    uint32_t count;
    uint32_t input_bytes = 0;
    uint32_t encode_bytes;
    uint32_t freq;
    uint32_t cycles = 0, finish_cycles = 0;
    uint32_t total_cycles = 0;
    uint32_t total_enc = 0;
    FILE *fd;
    rt_err_t ret;

    opus_cfg.sample_rate = g_rate;
    opus_cfg.bits = OPUS_BITS;
    opus_cfg.channels = g_ch;
    opus_cfg.bitrate = OPUS_BITRATE;
    info = rkdsp_malloc(sizeof(struct opus_info));
    info->max_packets = g_packet;
    info->comment_num = 2;

    comments = rkdsp_malloc((sizeof(char *) * info->comment_num));
    comments[0] = rkdsp_malloc(128);
    memset(comments[0], 0x0, 128);
    snprintf(comments[0], 128, "ARTIST=RK");

    comments[1] = rkdsp_malloc(128);
    memset(comments[1], 0x0, 128);
    snprintf(comments[1], 128, "SN=test2108");

    info->comments = (uint32_t)comments;
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)comments, sizeof(char *) * info->comment_num);
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)info, sizeof(struct opus_info));

    g_ABCoreDat.dat[0] = (uint32_t)&opus_cfg;
    g_ABCoreDat.dat[1] = (uint32_t)info;
    g_dsp_handle = rt_device_find("dsp0");
    ret = rt_device_open(g_dsp_handle, RT_DEVICE_OFLAG_RDWR);
    RT_ASSERT(ret == RT_EOK);
    ret = rt_device_control(g_dsp_handle, RKDSP_CTL_SET_FREQ, (void *)297000000);
    RT_ASSERT(ret == RT_EOK);
    ret = rt_device_control(g_dsp_handle, RKDSP_CTL_GET_FREQ, (void *)&freq);
    RT_ASSERT(ret == RT_EOK);
    rt_kprintf("DSP set freq %ld\n", freq);
    g_dsp_work = rk_dsp_work_create(RKDSP_CODEC_WORK);
    g_dsp_work->id = 0;
    g_dsp_work->algo_type = DSP_CODEC_OPUS_ENCODE_OPEN;
    g_dsp_work->param = (uint32_t)&g_ABCoreDat;
    g_dsp_work->param_size = sizeof(struct share_data);
    ret = rt_device_control(g_dsp_handle, RKDSP_CTL_QUEUE_WORK, g_dsp_work);
    RT_ASSERT(ret == RT_EOK);
    ret = rt_device_control(g_dsp_handle, RKDSP_CTL_DEQUEUE_WORK, g_dsp_work);
    RT_ASSERT(ret == RT_EOK);
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, (void *)&g_ABCoreDat, sizeof(struct share_data));
    g_opus = (opus_enc_t *)g_ABCoreDat.dat[0];
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, (void *)g_opus, sizeof(opus_enc_t));
    rkdsp_free(info);
    for (int i = 0; i < info->comment_num; i++)
        rkdsp_free(comments[i]);
    rkdsp_free(comments);

    opus_reader.or = opus_reader.open(opus_reader.name);
    if (opus_reader.or == NULL)
    {
        rt_kprintf("Open %s failed.\n", opus_reader.name);
        goto EXIT;
    }

    count = duration * opus_cfg.sample_rate / g_opus->frame_size;
    size = g_opus->frame_size * opus_cfg.channels * (opus_cfg.bits >> 3);
    rt_kprintf("Save to %s. Duration %ds, r %d b %d c %d p %d\n",
               file_path,
               duration,
               opus_cfg.sample_rate,
               opus_cfg.bits,
               opus_cfg.channels,
               g_opus->frame_size);
    fd = fopen(file_path, "wb+");
    if (fd == NULL)
    {
        rt_kprintf("Failed to open %s\n", file_path);
        opus_reader.close(opus_reader.or);
        goto EXIT;
    }
    if (g_opus->header_size)
    {
        rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, (void *)g_opus->_buffer_header, g_opus->header_size);
        fwrite((char *)g_opus->_buffer_header, 1, g_opus->header_size, fd);
        total_enc += g_opus->header_size;
        if (show_cycles)
            rt_kprintf("[0x%08lx][%8ld][cycles:%8ld]%p %d[Header]\n",
                       HAL_GetTick(),
                       input_bytes,
                       cycles,
                       (void *)g_opus->_buffer_header,
                       g_opus->header_size);
        g_opus->header_size = 0;
        rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)g_opus, sizeof(opus_enc_t));
    }
    while (count)
    {
        size_t in_bytes = opus_reader.read(opus_reader.or, (void *)g_opus->_buffer_in,
                                           opus_cfg.channels * (opus_cfg.bits >> 3),
                                           g_opus->frame_size);
        if (in_bytes <= 0)
            break;
        rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)g_opus->_buffer_in, size);
        input_bytes += size;
        g_dsp_work->id = 0;
        g_dsp_work->algo_type = DSP_CODEC_OPUS_ENCODE_PROCESS;
        ret = rt_device_control(g_dsp_handle, RKDSP_CTL_QUEUE_WORK, g_dsp_work);
        RT_ASSERT(ret == RT_EOK);
        ret = rt_device_control(g_dsp_handle, RKDSP_CTL_DEQUEUE_WORK, g_dsp_work);
        RT_ASSERT(ret == RT_EOK);
        rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, (void *)&g_ABCoreDat, sizeof(struct share_data));
        encode_bytes = g_ABCoreDat.dat[1];
        cycles += g_dsp_work->cycles;
        total_cycles += g_dsp_work->cycles;
        count--;
        if (encode_bytes == 0)
            continue;
        if (encode_bytes)
        {
            rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, (void *)g_opus, sizeof(opus_enc_t));
            rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, (void *)g_opus->_buffer_out, encode_bytes);
            fwrite((char *)g_opus->_buffer_out, 1, encode_bytes, fd);
            total_enc += encode_bytes;
            if (show_cycles)
                rt_kprintf("[0x%08lx][%8ld][cycles:%8ld]%p %d\n",
                           HAL_GetTick(),
                           input_bytes,
                           cycles,
                           (void *)g_opus->_buffer_out,
                           encode_bytes);
            cycles = 0;
        }
    }
    opus_reader.close(opus_reader.or);
FINISH_RETRY:
    g_dsp_work->id = 0;
    g_dsp_work->algo_type = DSP_CODEC_OPUS_ENCODE_FINISH;
    ret = rt_device_control(g_dsp_handle, RKDSP_CTL_QUEUE_WORK, g_dsp_work);
    ret = rt_device_control(g_dsp_handle, RKDSP_CTL_DEQUEUE_WORK, g_dsp_work);
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, (void *)&g_ABCoreDat, sizeof(struct share_data));
    cycles += g_dsp_work->cycles;
    finish_cycles += g_dsp_work->cycles;
    total_cycles += g_dsp_work->cycles;
    encode_bytes = g_ABCoreDat.dat[1];
    if (encode_bytes)
    {
        rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, (void *)g_opus, sizeof(opus_enc_t));
        rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, (void *)g_opus->_buffer_out, encode_bytes);
        fwrite((char *)g_opus->_buffer_out, 1, encode_bytes, fd);
        total_enc += encode_bytes;
        if (show_cycles)
            rt_kprintf("[0x%08lx][%8ld][cycles:%8ld]%p %d[Finish]\n",
                       HAL_GetTick(),
                       input_bytes,
                       cycles,
                       (void *)g_opus->_buffer_out,
                       encode_bytes);
        cycles = 0;
        goto FINISH_RETRY;
    }
    rt_kprintf("Total bytes %ld -> %ld\n", input_bytes, total_enc);
    if (show_cycles)
    {
        uint32_t p_cycle = (total_cycles - finish_cycles) / duration;
        uint32_t t_cycle = total_cycles / duration;
        rt_kprintf("Avg cycles %d.%06d MHZ(%d.%06d MHZ)\n",
                   p_cycle / 1000000, p_cycle % 1000000,
                   t_cycle / 1000000, t_cycle % 1000000);
    }
    fclose(fd);
EXIT:
    g_dsp_work->id = 0;
    g_dsp_work->algo_type = DSP_CODEC_OPUS_ENCODE_CLOSE;
    ret = rt_device_control(g_dsp_handle, RKDSP_CTL_QUEUE_WORK, g_dsp_work);
    ret = rt_device_control(g_dsp_handle, RKDSP_CTL_DEQUEUE_WORK, g_dsp_work);
    ret = rk_dsp_work_destroy(g_dsp_work);
    rt_device_close(g_dsp_handle);
}

static void do_opus_decode(void *arg)
{
    rt_kprintf("WIP\n");
}

void dsp_opus(int argc, char *argv[])
{
    rt_thread_t tid;
    int index = 1;
    int encode = 1;
    int card_in = 1;

    show_cycles = 0;
    duration = 5;
    g_rate = OPUS_SAMPLE_RATE;
    g_ch = OPUS_CHANNELS;
    g_packet = 20;
    while (index < argc)
    {
        if (strcmp(argv[index], "-D") == 0)
        {
            index++;
            memset(opus_reader.name, 0x0, sizeof(opus_reader.name));
            memcpy(opus_reader.name, argv[index], strlen(argv[index]));
            card_in = 1;
        }
        else if (strcmp(argv[index], "-i") == 0)
        {
            index++;
            memset(opus_reader.name, 0x0, sizeof(opus_reader.name));
            memcpy(opus_reader.name, argv[index], strlen(argv[index]));
            card_in = 0;
        }
        else if (strcmp(argv[index], "-t") == 0)
        {
            index++;
            duration = atoi(argv[index]);
        }
        else if (strcmp(argv[index], "-f") == 0)
        {
            index++;
            memcpy(file_path, argv[index], strlen(argv[index]));
        }
        else if (strcmp(argv[index], "-r") == 0)
        {
            index++;
            g_rate = atoi(argv[index]);
        }
        else if (strcmp(argv[index], "-c") == 0)
        {
            index++;
            g_ch = atoi(argv[index]);
        }
        else if (strcmp(argv[index], "-e") == 0)
        {
            encode = 1;
        }
        else if (strcmp(argv[index], "-d") == 0)
        {
            encode = 0;
        }
        else if (strcmp(argv[index], "-v") == 0)
        {
            show_cycles = 1;
        }
        else if (strcmp(argv[index], "-p") == 0)
        {
            index++;
            g_packet = atoi(argv[index]);
        }
        index++;
    }
    if (card_in)
    {
        opus_reader.open = card_open;
        opus_reader.read = card_read;
        opus_reader.close = card_close;
    }
    else
    {
        opus_reader.open = file_open;
        opus_reader.read = file_read;
        opus_reader.close = file_close;
    }
    if (encode)
    {
        tid = rt_thread_create("do_opusenc", do_opus_encode, NULL, 2048, 10, 10);
    }
    else
    {
        tid = rt_thread_create("do_opusdec", do_opus_decode, NULL, 2048, 10, 10);
    }
    RT_ASSERT(tid);
    rt_thread_startup(tid);
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(dsp_opus, dsp opus test);
#endif // RT_USING_FINSH

#endif // RT_USING_DSP_AUDIO_CODEC_TEST
