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

#ifndef PLAY_OPUS_H
#define PLAY_OPUS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "extcodec.h"

#ifdef RT_USING_EXT_OPUS_DECODER
#include "opus_decode.h"

#define OGG_BUFFER_IN       2048

#define DEFAULT_OPUS_DECODER { \
        .type = "opus", \
        .support_seek = 1, \
        .check = play_opus_check_impl, \
        .init = play_opus_init_impl, \
        .process = play_opus_process_impl, \
        .get_post_state = play_opus_get_post_state_impl, \
        .destroy = play_opus_destroy_impl, \
    }

struct play_opus
{
    bool has_post;
    decode_input_t input;
    decode_output_t output;
    decode_post_t post;

    uint16_t skip_page;  /* Number of page need to skip when seek */
    uint8_t skip_packet; /* Number of packet need to skip after page skip */

    void *userdata;
};

int play_opus_check_impl(char *buf, int len);
int play_opus_init_impl(struct play_decoder *self, play_decoder_cfg_t *cfg);
play_decoder_error_t play_opus_process_impl(struct play_decoder *self);
bool play_opus_get_post_state_impl(struct play_decoder *self);
void play_opus_destroy_impl(struct play_decoder *self);
#endif

#ifdef __cplusplus
}
#endif
#endif
