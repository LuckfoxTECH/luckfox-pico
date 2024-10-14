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
 *   Date: 2023.03.30
 */

#ifndef __ROCKAA_PLAY_H__
#define __ROCKAA_PLAY_H__

#include "rockaa_common.h"

typedef struct _rockaa_p
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
} rockaa_p;

int rockaa_play_effect_create(rockaa_p *handle);
int rockaa_play_effect_destroy(rockaa_p *handle);
int rockaa_play_effect_process(rockaa_p *handle, short *pcm_in, short *pcm_out);

#endif /* __ROCKAA_PLAY_H__ */
