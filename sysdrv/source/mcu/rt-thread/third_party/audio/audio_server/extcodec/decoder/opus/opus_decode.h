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

#ifndef _OPUS_DECODE_H
#define _OPUS_DECODE_H

#include "extcodec.h"

#ifdef RT_USING_EXT_OPUS_DECODER
#include "celt.h"
#include "ogg.h"
#include "opus.h"

#define OPUS_FRAME_SIZE     960

typedef struct _opus_dec_s
{
    OpusDecoder *decoder;
    int         sample_rate;
    int         channels;
    char        *buffer_in;
    uint32_t    bytes_in;
    uint16_t    *buffer_out;
    uint32_t    bytes_out;

    struct audio_server_data_share *ab_share_dat;
} opus_dec_t;

int32_t AudioOpusDecodeOpen(uint32_t A2B_EncOpus);
int32_t AudioOpusDecodeClose(uint32_t A2B_EncOpus);
int32_t AudioOpusDecode(uint32_t A2B_EncOpus);
#endif  // RT_USING_EXT_OPUS_DECODER

#endif  // _OPUS_DECODE_H
