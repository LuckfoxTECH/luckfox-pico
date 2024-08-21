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

#include "play_opus.h"

#ifdef RT_USING_EXT_OPUS_DECODER
#define DBG_SECTION_NAME    "OPUS"
#define DBG_LEVEL           DBG_INFO
#include "rtdbg.h"

#define GET_TOTAL_TIME      0
#define POST_INPUT_RATE     1

static struct audio_server_data_share g_share_data;

int play_opus_check_impl(char *buf, int len)
{
    if (strncmp(buf, "OggS", 4))
        return -RT_ERROR;

    /*
     * The 27th byte after OggS header is the start
     * of segments table[max 255], and the 26th byte
     * after OggS header is the number of segments
     * (also means the length of segments table),
     * OpusHead header follows the segments table
     */
    if (strncmp(buf + 27 + *(buf + 26), "OpusHead", 8))
        return -RT_ERROR;

    return RT_EOK;
}

int play_opus_prepare(struct play_opus *opus, uint32_t seek)
{
    char *buffer_in;
    uint32_t read_bytes;
    ogg_sync_state   oy;
    ogg_page         og;
    int page = 0;
    int ret = 0;

    opus->skip_page = 2;
    opus->skip_packet = 0;
#if !GET_TOTAL_TIME
    if (!seek)
        return RT_EOK;
#endif
    ogg_sync_init(&oy);
    while (seek)
    {
        uint32_t pos_a = 0, pos_b = 0;
        uint8_t segs_a = 0, segs_b = 0;
        /* s to ms */
        seek *= 1000;
SEEK_BUFFER_IN:
        buffer_in = ogg_sync_buffer(&oy, OGG_BUFFER_IN);
        read_bytes = opus->input(opus->userdata, buffer_in,
                                 OGG_BUFFER_IN);
        if (read_bytes <= 0)
        {
            if (page < 3)
            {
                LOG_E("No enough data");
                ogg_sync_clear(&oy);
                return -RT_ERROR;
            }
            else
            {
                break;
            }
        }
        ogg_sync_wrote(&oy, read_bytes);
SEEK_PAGEOUT:
        ret = ogg_sync_pageout(&oy, &og);
        if (ret != 1)
        {
            /* need more data for page */
            goto SEEK_BUFFER_IN;
        }
        page++;
        if (page == 3)
        {
            pos_a = *(uint32_t *)(og.header + 6);
            segs_a = *(uint8_t *)(og.header + 26);
        }
        if (page == 4)
        {
            pos_b = *(uint32_t *)(og.header + 6);
            segs_b = *(uint8_t *)(og.header + 26);
            int segs_t = (pos_b - pos_a) / 48 / segs_a;
            int s_page = (seek / (segs_b * segs_t));
            opus->skip_page = 2 + s_page;
            opus->skip_packet = seek % (segs_b * segs_t) / segs_t;
            LOG_I("pos_a %ld pos_b %ld segs_a %d segs_b %d segs_t %d s_page %d opus->s_page %d opus->s_packet %d",
                  pos_a, pos_b, segs_a, segs_b, segs_t, s_page, opus->skip_page, opus->skip_packet);
            break;
        }
        goto SEEK_PAGEOUT;
    }

#if GET_TOTAL_TIME
    uint32_t file_length = player_get_file_length(opus->userdata);
    uint32_t total_time = 0;
    if (file_length > 255 * 255 * 4)
    {
        /* A simple way to get total time of large file */
        LOG_D("Skip to %d", file_length - (255 * 255 * 2));
        player_preprocess_seek(opus->userdata, file_length - (255 * 255 * 2));
    }
    ogg_sync_clear(&oy);
    ogg_sync_init(&oy);
    while (1)
    {
        buffer_in = ogg_sync_buffer(&oy, OGG_BUFFER_IN);
        read_bytes = opus->input(opus->userdata, buffer_in,
                                 OGG_BUFFER_IN);
        if (read_bytes <= 0)
            break;
        ogg_sync_wrote(&oy, read_bytes);
        ret = ogg_sync_pageout(&oy, &og);
        if (ret != 1)
        {
            /* need more data for page */
            continue;
        }
        /* Actually it is 64 bit */
        total_time = *(uint32_t *)(og.header + 6);
    }
    ogg_sync_clear(&oy);

    total_time = total_time / 48;
    player_set_total_time(opus->userdata, total_time);
    LOG_D("Total time %ld.%lds. Start from %lds, skip %d pages %d packets",
          total_time / 1000, total_time % 1000,
          seek / 48000, opus->skip_page, opus->skip_packet);
#endif

    return player_preprocess_seek(opus->userdata, 0);
}

int play_opus_init_impl(struct play_decoder *self, play_decoder_cfg_t *cfg)
{
    struct play_opus *opus = (struct play_opus *)rt_calloc(1, sizeof(struct play_opus));
    if (!opus)
        return -RT_ERROR;
    memset((void *)&g_share_data, 0x0, sizeof(struct audio_server_data_share));
    memset((void *)opus, 0x0, sizeof(struct play_opus));
    opus->has_post = 0;
    opus->input = cfg->input;
    opus->output = cfg->output;
    opus->post = cfg->post;
    opus->userdata = cfg->userdata;
    self->userdata = (void *)opus;

    if (play_opus_prepare(opus, cfg->start_time) != RT_EOK)
        return -RT_ERROR;

    LOG_I("init success");

    return RT_EOK;
}

int play_opus_post(unsigned char *packet, struct play_opus *opus)
{
    struct audio_config cfg;
    int ret;

#if POST_INPUT_RATE
    cfg.sample_rate = *(uint32_t *)(packet + 12);
#else
    cfg.sample_rate = 48000;
#endif
    cfg.bits = 16;
    cfg.channels = *(packet + 9);
    if (cfg.channels != 1 && cfg.channels != 2)
    {
        LOG_E("not support channels %d", cfg.channels);
        return -RT_ERROR;
    }
    g_share_data.dat[0] = (uint32_t)&cfg;
    ret = AudioOpusDecodeOpen((uint32_t)&g_share_data);
    if (ret < 0)
    {
        LOG_E("AudioOpusDecodeOpen Failed");
        return -RT_ERROR;
    }
    opus->post(opus->userdata, cfg.sample_rate, cfg.bits, cfg.channels);
    LOG_I("Post %d %d %d", cfg.sample_rate, cfg.bits, cfg.channels);
    opus->has_post = 1;

    return RT_EOK;
}

play_decoder_error_t play_opus_process_impl(struct play_decoder *self)
{
    struct play_opus *opus = (struct play_opus *) self->userdata;
    opus_dec_t *opus_dec = NULL;
    ogg_sync_state   oy;
    ogg_page         og;
    ogg_stream_state os;
    ogg_packet       op;
    uint8_t first_page = 1;
    char *buffer_in;
    int read_bytes;
    int write_bytes;
    int result;
    int err = PLAY_DECODER_SUCCESS;
    int ret = 0;

    LOG_I("decode start");
    ogg_sync_init(&oy);
    while (1)
    {
BUFFER_IN:
        buffer_in = ogg_sync_buffer(&oy, OGG_BUFFER_IN);
        read_bytes = opus->input(opus->userdata, buffer_in,
                                 OGG_BUFFER_IN);
        if (read_bytes == -1)
        {
            err = PLAY_DECODER_INPUT_ERROR;
            break;
        }
        if (read_bytes == 0)
        {
            err = PLAY_DECODER_SUCCESS;
            break;
        }
        ogg_sync_wrote(&oy, read_bytes);
PAGEOUT_RETRY:
        result = ogg_sync_pageout(&oy, &og);
        if (result != 1)
        {
            goto BUFFER_IN;
        }
        if (!first_page)
        {
            /* Skip header info page */
            if (opus->skip_page)
            {
                opus->skip_page--;
                goto PAGEOUT_RETRY;
            }
            if (ogg_stream_pagein(&os, &og) < 0)
            {
                LOG_E("pagein failed");
                err = PLAY_DECODER_DECODE_ERROR;
                break;
            }
        }
        else
        {
            first_page = 0;
            opus->skip_page--;
            ogg_stream_init(&os, ogg_page_serialno(&og));;
            ogg_stream_pagein(&os, &og);
            if (ogg_stream_packetout(&os, &op) == 1)
            {
                play_opus_post(op.packet, opus);
                opus_dec = (opus_dec_t *)g_share_data.dat[0];
            }
            else
            {
                LOG_E("First packet out failed");
                err = PLAY_DECODER_DECODE_ERROR;
                break;
            }
        }
        while (ogg_stream_packetout(&os, &op) == 1)
        {
            if (opus->skip_packet)
            {
                opus->skip_packet--;
                continue;
            }
            opus_dec->buffer_in = (char *)op.packet;
            opus_dec->bytes_in = op.bytes;
            ret = AudioOpusDecode((uint32_t)&g_share_data);
            if (ret < 0)
            {
                LOG_E("Decoder fail:%d", ret);
                err = PLAY_DECODER_DECODE_ERROR;
                break;
            }
            LOG_D("decode %p %ld", opus_dec->buffer_out, opus_dec->bytes_out);
            write_bytes = opus->output(opus->userdata,
                                       (char *)opus_dec->buffer_out,
                                       opus_dec->bytes_out);
            if (write_bytes < 0)
            {
                err = PLAY_DECODER_OUTPUT_ERROR;
                break;
            }
        }
        goto PAGEOUT_RETRY;
    }
    if (first_page == 0)
        ogg_stream_clear(&os);
    ogg_sync_clear(&oy);

    return err;
}

bool play_opus_get_post_state_impl(struct play_decoder *self)
{
    struct play_opus *opus = (struct play_opus *) self->userdata;
    return opus->has_post;
}

void play_opus_destroy_impl(struct play_decoder *self)
{
    struct play_opus *opus = (struct play_opus *) self->userdata;

    if (opus->has_post)
        AudioOpusDecodeClose((uint32_t)&g_share_data);
    rt_free(opus);
}

#endif  // RT_USING_EXT_OPUS_DECODER
