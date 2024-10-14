/*
 * Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-04-17     Jair Wu      First version
 *
 */

#include "opus_encode.h"

#ifdef RT_USING_EXT_OPUS_ENCODER

#define OPUS_TIME_TEST     (0)

#if OPUS_ENCODE_RUN_ON_DSP && defined(RT_USING_DSP)

#include "drv_dsp.h"
static struct rt_device *g_dsp_handle;
static struct dsp_work *g_dsp_work;
static opus_enc_t *g_opus_enc;

int32_t AudioOpusEncodeOpen(uint32_t A2B_EncOpus)
{
    struct audio_server_data_share *pEncDat = (struct audio_server_data_share *)A2B_EncOpus;
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
    g_dsp_work->algo_type = DSP_CODEC_OPUS_ENCODE_OPEN;
    g_dsp_work->param = (uint32_t)A2B_EncOpus;
    g_dsp_work->param_size = sizeof(struct audio_server_data_share);
    ret = rt_device_control(g_dsp_handle, RKDSP_CTL_QUEUE_WORK, g_dsp_work);
    RT_ASSERT(ret == RT_EOK);
    ret = rt_device_control(g_dsp_handle, RKDSP_CTL_DEQUEUE_WORK, g_dsp_work);
    RT_ASSERT(ret == RT_EOK);
    /*
     *  @TODO
     *  Need to reduce the number of dcache operations
     * */
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, (void *)&pEncDat, sizeof(struct audio_server_data_share));
    g_opus_enc = (opus_enc_t *)pEncDat->dat[0];
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE,
                         (void *)g_opus_enc,
                         sizeof(opus_enc_t));
    if (g_opus_enc->header_size)
        rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE,
                             (void *)g_opus_enc->_buffer_header,
                             g_opus_enc->header_size);

    return g_dsp_work->result;
}

int32_t AudioOpusEncode(void)
{
    int ret;

    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)g_opus_enc->_buffer_in,
                         g_opus_enc->frame_size * g_opus_enc->channels * sizeof(short));
    g_dsp_work->id = 0;
    g_dsp_work->algo_type = DSP_CODEC_OPUS_ENCODE_PROCESS;
    ret = rt_device_control(g_dsp_handle, RKDSP_CTL_QUEUE_WORK, g_dsp_work);
    RT_ASSERT(ret == RT_EOK);
    ret = rt_device_control(g_dsp_handle, RKDSP_CTL_DEQUEUE_WORK, g_dsp_work);
    RT_ASSERT(ret == RT_EOK);
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE,
                         (void *)&g_opus_enc->ab_share_dat,
                         sizeof(struct audio_server_data_share));
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE,
                         (void *)g_opus_enc,
                         sizeof(opus_enc_t));
    if (g_opus_enc->ab_share_dat->dat[1])
        rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE,
                             (void *)g_opus_enc->_buffer_out,
                             g_opus_enc->ab_share_dat->dat[1]);

    return g_dsp_work->result;
}

int32_t AudioOpusEncodeFinish(void)
{
    int ret;

    g_dsp_work->id = 0;
    g_dsp_work->algo_type = DSP_CODEC_OPUS_ENCODE_FINISH;
    ret = rt_device_control(g_dsp_handle, RKDSP_CTL_QUEUE_WORK, g_dsp_work);
    RT_ASSERT(ret == RT_EOK);
    ret = rt_device_control(g_dsp_handle, RKDSP_CTL_DEQUEUE_WORK, g_dsp_work);
    RT_ASSERT(ret == RT_EOK);
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE,
                         (void *)&g_opus_enc->ab_share_dat,
                         sizeof(struct audio_server_data_share));
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE,
                         (void *)g_opus_enc,
                         sizeof(opus_enc_t));
    if (g_opus_enc->ab_share_dat->dat[1])
        rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE,
                             (void *)g_opus_enc->_buffer_out,
                             g_opus_enc->ab_share_dat->dat[1]);

    return g_dsp_work->result;
}

int32_t AudioOpusEncodeClose(void)
{
    int ret;

    g_dsp_work->id = 0;
    g_dsp_work->algo_type = DSP_CODEC_OPUS_ENCODE_CLOSE;
    ret = rt_device_control(g_dsp_handle, RKDSP_CTL_QUEUE_WORK, g_dsp_work);
    RT_ASSERT(ret == RT_EOK);
    ret = rt_device_control(g_dsp_handle, RKDSP_CTL_DEQUEUE_WORK, g_dsp_work);
    RT_ASSERT(ret == RT_EOK);
    rk_dsp_work_destroy(g_dsp_work);
    rt_device_close(g_dsp_handle);

    return g_dsp_work->result;
}

#else // OPUS_ENCODE_RUN_ON_DSP && defined(RT_USING_DSP)

static opus_enc_t g_opus_enc;

int32_t AudioOpusEncodeOpen(uint32_t A2B_EncOpus)
{
    struct audio_server_data_share *pEncDat = (struct audio_server_data_share *)A2B_EncOpus;
    struct audio_config *opus_cfg = (struct audio_config *)pEncDat->dat[0];
    struct opus_info *info = (struct opus_info *)pEncDat->dat[1];
    int err;

    memset(&g_opus_enc, 0, sizeof(opus_enc_t));
    g_opus_enc.comments = ope_comments_create();
    if (g_opus_enc.comments == NULL)
    {
        rt_kprintf("g_opus_enc.comments create failed\n");
        return -1;
    }
    rt_kprintf("g_opus_enc.comments %p\n", g_opus_enc.comments);
    g_opus_enc.max_packets = info->max_packets;
    if (info->comment_num)
    {
        char **comments = (char **)info->comments;
        int i;
        for (i = 0; i < info->comment_num; i++)
            ope_comments_add(g_opus_enc.comments, NULL, comments[i]);
    }
    else
    {
        ope_comments_add(g_opus_enc.comments, "ARTIST", "RK");
    }

    g_opus_enc.ab_share_dat = pEncDat;
    g_opus_enc.frame_size = opus_cfg->sample_rate / 50;    // 20ms
    g_opus_enc.sample_rate = opus_cfg->sample_rate;
    g_opus_enc.channels = opus_cfg->channels;
    g_opus_enc._buffer_in = rt_malloc(g_opus_enc.frame_size * g_opus_enc.channels * sizeof(short));
    if (g_opus_enc._buffer_in == NULL)
    {
        free(g_opus_enc._buffer_in);
        ope_comments_destroy(g_opus_enc.comments);
        rt_kprintf("Malloc buffer_in failed\n");
        return -1;
    }

    rt_kprintf("%s,opus_cfg.sample_rate = %d, opus_cfg.channels = %d\n",
               __func__, opus_cfg->sample_rate, opus_cfg->channels);
    g_opus_enc._encoder = ope_encoder_create_pull(g_opus_enc.comments,
                                                  opus_cfg->sample_rate, opus_cfg->channels,
                                                  opus_cfg->channels > 8 ? 255 : (opus_cfg->channels > 2),
                                                  &err);
    if (g_opus_enc._encoder == NULL || err != OPUS_OK)
    {
        rt_kprintf("ope_encoder_create_callbacks = %d\n", err);
        ope_comments_destroy(g_opus_enc.comments);
        rt_free(g_opus_enc._buffer_in);
        return -1;
    }
    opus_int32 nb_streams = 1, nb_coupled = 0;
    opus_int32 opus_frame_param = OPUS_FRAMESIZE_20_MS;
    opus_int32 vbr = 1;
    uint32_t bitrate;
    ope_encoder_ctl(g_opus_enc._encoder, OPUS_SET_EXPERT_FRAME_DURATION(opus_frame_param));
    ope_encoder_ctl(g_opus_enc._encoder, OPE_SET_MUXING_DELAY(0));
    ope_encoder_ctl(g_opus_enc._encoder, OPE_GET_NB_STREAMS(&nb_streams));
    ope_encoder_ctl(g_opus_enc._encoder, OPE_GET_NB_COUPLED_STREAMS(&nb_coupled));
    if (opus_cfg->bitrate)
    {
        bitrate = opus_cfg->bitrate;
        vbr = 0;
    }
    else
    {
        bitrate = ((64000 * nb_streams + 32000 * nb_coupled) *
                   (IMIN(48, IMAX(8, ((opus_cfg->sample_rate < 44100 ? opus_cfg->sample_rate : 48000) + 1000) / 1000)) + 16) + 32) >> 6;
    }
    bitrate = IMIN(opus_cfg->channels * 256000, bitrate);
    rt_kprintf("rate %d ch %d bitrate %d\n", opus_cfg->sample_rate, opus_cfg->channels, bitrate);
    ope_encoder_ctl(g_opus_enc._encoder, OPUS_SET_BITRATE(bitrate));
    ope_encoder_ctl(g_opus_enc._encoder, OPUS_SET_VBR(vbr));
    ope_encoder_ctl(g_opus_enc._encoder, OPUS_SET_SIGNAL(OPUS_SIGNAL_VOICE));
    ope_encoder_ctl(g_opus_enc._encoder, OPUS_SET_COMPLEXITY(0));
    ope_encoder_ctl(g_opus_enc._encoder, OPUS_SET_PACKET_LOSS_PERC(0));

    ope_encoder_flush_header(g_opus_enc._encoder);
    ope_encoder_get_page(g_opus_enc._encoder, (unsigned char **)&g_opus_enc._buffer_header, (opus_int32 *)&g_opus_enc.header_size, 0);
    pEncDat->dat[0] = (uint32_t)&g_opus_enc;
    pEncDat->dat[1] = 0;

    return 0;
}

int32_t AudioOpusEncode(void)
{
#if OPUS_TIME_TEST
    static int t11 = 0, t00 = 0;
    t11 = HAL_GetTick();
#endif
    g_opus_enc.enc_size = 0;
    g_opus_enc.packets++;
    ope_encoder_write(g_opus_enc._encoder, g_opus_enc._buffer_in, g_opus_enc.frame_size);
    ope_encoder_get_page(g_opus_enc._encoder, (unsigned char **)&g_opus_enc._buffer_out, (opus_int32 *)&g_opus_enc.enc_size,
                         (g_opus_enc.max_packets != 0) && (g_opus_enc.packets >= g_opus_enc.max_packets));
#if OPUS_TIME_TEST
    t00 = HAL_GetTick();
    rt_kprintf("timer_opus_enc = %d (ms)\n", t00 - t11);
#endif
    g_opus_enc.ab_share_dat->dat[1] = g_opus_enc.enc_size;
    return 0;
}

int32_t AudioOpusEncodeFinish(void)
{
    g_opus_enc.enc_size = 0;;
    if (g_opus_enc.drain_flag == 0)
    {
        ope_encoder_drain(g_opus_enc._encoder);
        g_opus_enc.drain_flag = 1;
    }
    ope_encoder_get_page(g_opus_enc._encoder, (unsigned char **)&g_opus_enc._buffer_out, (opus_int32 *)&g_opus_enc.enc_size, 0);
    g_opus_enc.ab_share_dat->dat[1] = g_opus_enc.enc_size;

    return 0;
}

int32_t AudioOpusEncodeClose(void)
{
    rt_kprintf("opus encode close\n");
    if (g_opus_enc._encoder)
    {
        rt_free(g_opus_enc._buffer_in);
        ope_encoder_destroy(g_opus_enc._encoder);
        ope_comments_destroy(g_opus_enc.comments);

        g_opus_enc._encoder = NULL;
    }

    return 0;
}

#endif  // OPUS_ENCODE_RUN_ON_DSP && defined(RT_USING_DSP)

#endif  // RT_USING_EXT_OPUS_ENCODER
