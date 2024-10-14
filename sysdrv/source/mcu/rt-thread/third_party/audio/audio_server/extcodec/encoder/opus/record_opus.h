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

#ifndef RECORD_OPUS_H
#define RECORD_OPUS_H
#ifdef __cplusplus
extern "C" {
#endif

#include "extcodec.h"

#ifdef RT_USING_EXT_OPUS_ENCODER
#include "opus_encode.h"
int record_opus_init(struct record_encoder *self, record_encoder_cfg_t *cfg);
record_encoder_error_t record_opus_process(struct record_encoder *self);
bool record_opus_get_post_state(struct record_encoder *self);
void record_opus_destroy(struct record_encoder *self);

#define DEFAULT_OPUS_ENCODER { \
        .type = "opus", \
        .init = record_opus_init, \
        .process = record_opus_process, \
        .get_post_state = record_opus_get_post_state, \
        .destroy = record_opus_destroy, \
    }
#endif

#ifdef __cplusplus
}
#endif
#endif

