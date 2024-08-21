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

#include "opus_decode.h"

#ifdef RT_USING_EXT_OPUS_DECODER

int32_t AudioOpusDecodeOpen(uint32_t A2B_DecOpus)
{
    struct audio_server_data_share *share_data = (struct audio_server_data_share *)A2B_DecOpus;
    struct audio_config *cfg = (struct audio_config *)share_data->dat[0];
    opus_dec_t *opus;
    int err;
    opus = rt_malloc(sizeof(opus_dec_t));
    RT_ASSERT(opus != NULL);

    opus->decoder = opus_decoder_create(cfg->sample_rate, cfg->channels, &err);
    opus->sample_rate = cfg->sample_rate;
    opus->channels = cfg->channels;
    opus->buffer_out = (uint16_t *)rt_malloc(OPUS_FRAME_SIZE * cfg->channels * sizeof(uint16_t));
    RT_ASSERT(opus->buffer_out != NULL);
    opus->ab_share_dat = share_data;
    share_data->dat[0] = (uint32_t)opus;

    return 0;
}

int32_t AudioOpusDecodeClose(uint32_t A2B_DecOpus)
{
    struct audio_server_data_share *share_data = (struct audio_server_data_share *)A2B_DecOpus;
    opus_dec_t *opus = (opus_dec_t *)share_data->dat[0];
    rt_free(opus->buffer_out);
    opus_decoder_destroy(opus->decoder);
    rt_free(opus);

    return 0;
}

int32_t AudioOpusDecode(uint32_t A2B_DecOpus)
{
    struct audio_server_data_share *share_data = (struct audio_server_data_share *)A2B_DecOpus;
    opus_dec_t *opus = (opus_dec_t *)share_data->dat[0];
    int len;

    len = opus_decode(opus->decoder,
                      (unsigned char *)opus->buffer_in,
                      opus->bytes_in,
                      (opus_int16 *)opus->buffer_out,
                      OPUS_FRAME_SIZE,
                      0);
    if (len <= 0)
    {
        rt_kprintf("decode error %d\n", len);
        return -1;
    }
    opus->bytes_out = len * sizeof(uint16_t) * opus->channels;

    return 0;
}
#endif
