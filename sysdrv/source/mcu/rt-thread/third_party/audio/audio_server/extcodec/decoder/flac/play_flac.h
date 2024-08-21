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

#ifndef PLAY_FLAC_H
#define PLAY_FLAC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "extcodec.h"

#ifdef RT_USING_EXT_FLAC_DECODER
#include "flac_decode.h"

#define FLAC_BUFFER_IN          4096

#define DEFAULT_FLAC_DECODER { \
        .type = "flac", \
        .support_seek = 0, \
        .check = play_flac_check_impl, \
        .init = play_flac_init_impl, \
        .process = play_flac_process_impl, \
        .get_post_state = play_flac_get_post_state_impl, \
        .destroy = play_flac_destroy_impl, \
    }

struct play_flac
{
    bool has_post;
    decode_input_t input;
    decode_output_t output;
    decode_post_t post;

    struct flac_dec *dec;

    void *userdata;
};

int play_flac_check_impl(char *buf, int len);
int play_flac_init_impl(struct play_decoder *self, play_decoder_cfg_t *cfg);
play_decoder_error_t play_flac_process_impl(struct play_decoder *self);
bool play_flac_get_post_state_impl(struct play_decoder *self);
void play_flac_destroy_impl(struct play_decoder *self);
#endif

#ifdef __cplusplus
}
#endif
#endif
