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

#ifndef _OPUS_ENCODE_H_
#define _OPUS_ENCODE_H_

#include "extcodec.h"

#ifdef RT_USING_EXT_OPUS_ENCODER
#include "arch.h"
#include "ogg_packer.h"
#include "opusenc.h"

struct opus_info
{
    int max_packets;
    int comment_num;
    uint32_t comments;
};

typedef struct _opus_enc_s
{
    OggOpusEnc          *_encoder;
    OggOpusComments     *comments;
    int                 sample_rate;
    int                 channels;
    int                 frame_size;
    short               *_buffer_in;
    short               *_buffer_header;
    short               *_buffer_out;
    int                 header_size;
    int                 enc_size;
    int                 drain_flag;
    int                 packets;
    int                 max_packets;

    struct audio_server_data_share *ab_share_dat;
} opus_enc_t;

int32_t AudioOpusEncodeOpen(uint32_t A2B_EncOpus);
int32_t AudioOpusEncodeClose(void);
int32_t AudioOpusEncode(void);
int32_t AudioOpusEncodeFinish(void);
#endif  // RT_USING_EXT_OPUS_ENCODER

#endif  // _OPUS_ENCODE_H_
