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

#include "play_flac.h"

#ifdef RT_USING_EXT_FLAC_DECODER
#define DBG_SECTION_NAME    "FLAC"
#define DBG_LEVEL           DBG_INFO
#include "rtdbg.h"

int play_flac_check_impl(char *buf, int len)
{
    if (strncmp(buf, "fLaC", 4))
        return -RT_ERROR;

    return RT_EOK;
}

int play_flac_init_impl(struct play_decoder *self, play_decoder_cfg_t *cfg)
{
    struct play_flac *flac = (struct play_flac *)rt_calloc(1, sizeof(struct play_flac));
    if (!flac)
    {
        rt_kprintf("%s %d malloc %d failed\n", __func__, __LINE__, sizeof(struct play_flac));
        return -RT_ERROR;
    }
    flac->has_post = 0;
    flac->input = cfg->input;
    flac->output = cfg->output;
    flac->post = cfg->post;
    flac->userdata = cfg->userdata;
    self->userdata = (void *)flac;

    flac->dec = (struct flac_dec *)rkdsp_malloc(sizeof(struct flac_dec));
    if (!flac->dec)
    {
        rt_kprintf("%s %d malloc %d failed\n", __func__, __LINE__, sizeof(struct flac_dec));
        rt_free(flac);
        return -RT_ERROR;
    }
    memset(flac->dec, 0, sizeof(struct flac_dec));

    flac->dec->buf_in_size = FLAC_BUFFER_IN;
    flac->dec->buf_in = rkdsp_malloc(flac->dec->buf_in_size);
    flac->dec->file_size = player_get_file_length(flac->userdata);

    flac_dec_init(flac->dec);

    LOG_I("init success");

    return RT_EOK;
}

int play_flac_post(struct play_flac *flac)
{
    flac->dec->sample_rate = 16000;
    flac->dec->bits = 16;
    flac->dec->channels = 2;
    flac->post(flac->userdata, flac->dec->sample_rate, flac->dec->bits, flac->dec->channels);
    LOG_I("Post %d %d %d", flac->dec->sample_rate, flac->dec->bits, flac->dec->channels);
    flac->has_post = 1;

    return RT_EOK;
}

play_decoder_error_t play_flac_process_impl(struct play_decoder *self)
{
    struct play_flac *flac = (struct play_flac *) self->userdata;
    struct flac_dec *dec = flac->dec;
    short *buf;
    uint32_t buf_size;
    uint32_t read_bytes;
    uint32_t write_bytes;
    int err = PLAY_DECODER_SUCCESS;

    buf_size = 16 * 1024;
    buf = rt_malloc(buf_size);
    if (!buf)
    {
        rt_kprintf("%s %d malloc %d failed\n", __func__, __LINE__, buf_size);
        return PLAY_DECODER_DECODE_ERROR;
    }
    while (1)
    {
        if (dec->buf_in_left)
        {
            memcpy(dec->buf_in, dec->buf_in + (dec->buf_in_size - dec->buf_in_left), dec->buf_in_left);
            read_bytes = flac->input(flac->userdata, (char *)(dec->buf_in + dec->buf_in_left),
                                     FLAC_BUFFER_IN - dec->buf_in_left);
            if (read_bytes <= 0)
                read_bytes = dec->buf_in_left;
            else
                read_bytes += dec->buf_in_left;
        }
        else
        {
            read_bytes = flac->input(flac->userdata, (char *)dec->buf_in,
                                     FLAC_BUFFER_IN);
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
        }
        dec->buf_in_size = dec->buf_in_left = read_bytes;
        dec->out_of_data = dec->buf_out_size = 0;
        flac_dec_process(dec);
        if (dec->error_code >= FLAC__STREAM_DECODER_END_OF_STREAM)
        {
            err = PLAY_DECODER_DECODE_ERROR;
            if (dec->error_code == FLAC__STREAM_DECODER_END_OF_STREAM)
                err = PLAY_DECODER_SUCCESS;
            break;
        }

        if (dec->buf_out_size > 0)
        {
            if (buf_size < (dec->buf_out_size * sizeof(short) * dec->channels))
            {
                short *temp;
                temp = (short *)rt_realloc(buf, dec->buf_out_size * sizeof(short) * dec->channels);
                if (temp == NULL)
                {
                    rt_kprintf("%s %d malloc %d failed\n", __func__, __LINE__,
                               dec->buf_out_size * sizeof(short) * dec->channels);
                    err = PLAY_DECODER_DECODE_ERROR;
                    goto EXIT;
                }
                buf = temp;
                buf_size = dec->buf_out_size * sizeof(short) * dec->channels;
            }
            for (int i = 0; i < dec->buf_out_size * dec->channels; i += 2)
            {
                buf[i] = (short)dec->buf_out[0][i / 2];
                buf[i + 1] = (short)dec->buf_out[1][i / 2];
            }

            write_bytes = flac->output(flac->userdata,
                                       (char *)buf,
                                       buf_size);
            if (write_bytes < 0)
            {
                err = PLAY_DECODER_OUTPUT_ERROR;
                break;
            }
        }
        else if (flac->has_post == 0)
        {
            play_flac_post(flac);
        }
    }
EXIT:
    rt_free(buf);

    return err;
}

bool play_flac_get_post_state_impl(struct play_decoder *self)
{
    struct play_flac *flac = (struct play_flac *) self->userdata;
    return flac->has_post;
}

void play_flac_destroy_impl(struct play_decoder *self)
{
    struct play_flac *flac = (struct play_flac *) self->userdata;

    flac_dec_deinit(flac->dec);
    rkdsp_free(flac->dec->buf_in);
    rkdsp_free(flac->dec);
    rt_free(flac);
}

#endif  // RT_USING_EXT_flac_DECODER
