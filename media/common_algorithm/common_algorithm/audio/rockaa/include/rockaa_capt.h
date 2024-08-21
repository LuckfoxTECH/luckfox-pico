/*
 * Copyright 2023 Rockchip Electronics Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: xing.zheng@rock-chips.com
 *   Date: 2023.02.28
 */

#ifndef __ROCKAA_CAPT_H__
#define __ROCKAA_CAPT_H__

#include "rockaa_common.h"

enum
{
    STATUS_RES_SUCCESS = 0,
    STATUS_RES_NONE = -1,
    STATUS_RES_INVALID = -2,
};

typedef struct _result_capt
{
    int angle_horiz;
    int angle_pitch;
} result_capt;

typedef struct _rockaa_c
{
    /* base members */
    void *effect;
    FILE *in_fp;
    FILE *out_fp;
    char *in_buf;
    char *out_buf;
    char *conf_path;
    int out_bytes;
    int rate;
    int channels;
    int bits;
    int nb_samples;
    /* customized members */
    int debug_mode;
    bool enable_doa;
    int ch_near;
    int ch_far;
    int ch_out;  /* ==1: BF is enabled, ==ch_near: BF is disabled, configured by rockaa_capt_effect_create() */
    result_capt results;
} rockaa_c;

int rockaa_capt_effect_create(rockaa_c *handle);
int rockaa_capt_effect_destroy(rockaa_c *handle);
int rockaa_capt_effect_process(rockaa_c *handle, short *pcm_in, short *pcm_out);
int rockaa_capt_effect_results(rockaa_c *handle);

#endif /* __ROCKAA_CAPT_H__ */
