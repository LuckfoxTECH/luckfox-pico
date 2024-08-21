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
 *   Date: 2023.06.25
 */

#include "rockaa_capt.h"

int main(int argc, char **argv)
{
    rockaa_c *handle = NULL;
    int period_count = 4;
    short *pcm_in, *pcm_out;
    int read_len;

    handle = (rockaa_c *)malloc(sizeof(rockaa_c));
    if (!handle)
    {
        LOGE("alloc talk handle failed\n");
        return -1;
    }
    memset(handle, 0, sizeof(rockaa_c));

    handle->in_fp = fopen("sample_capt_in.pcm", "rb");
    if (!handle->in_fp)
    {
        LOGE("in_fp fopen 'sample_capt_in.pcm' failed\n");
        return -1;
    }
    handle->out_fp = fopen("sample_capt_out.pcm", "wb");
    if (!handle->out_fp)
    {
        LOGE("out_fp fopen 'sample_capt_out.pcm' failed\n");
        return -1;
    }

    handle->rate = 16000;
    handle->channels = 2;
    handle->bits = 16;
    handle->ch_far = 1;
    handle->ch_near = handle->channels - handle->ch_far;
    handle->debug_mode = 1;
    handle->nb_samples = 256;
    handle->enable_doa = 0;
    handle->conf_path = strdup("config_aivqe.json");
    handle->out_bytes = sizeof(short) * handle->nb_samples * 1; /* output mono */
    handle->out_buf = (char *)malloc(handle->out_bytes);
    if (!handle->out_buf)
    {
        LOGE("alloc out_buf failed\n");
        return -1;
    }

    pcm_in = (short *)malloc(handle->nb_samples * sizeof(short) * handle->channels);
    if (!pcm_in)
    {
        LOGE("alloc pcm_in failed\n");
        return -1;
    }
    pcm_out = (short *)malloc(handle->nb_samples * sizeof(short) * 1);
    if (!pcm_out)
    {
        LOGE("alloc pcm_out failed\n");
        return -1;
    }

    rockaa_capt_effect_create(handle);
    while (!feof(handle->in_fp))
    {
        read_len = fread(pcm_in, 1,
                         handle->nb_samples * sizeof(short) * handle->channels,
                         handle->in_fp);
        rockaa_capt_effect_process(handle, pcm_in, pcm_out);
        if (handle->enable_doa &&
                rockaa_capt_effect_results(handle) == STATUS_RES_SUCCESS)
        {
            LOGI("Got DOA Angles - horizon: %d pitch: %d\n",
                 __func__, __LINE__,
                 handle->results.angle_horiz,
                 handle->results.angle_pitch);
        }
        fwrite(pcm_out, 1,
               handle->nb_samples * sizeof(short) * 1,
               handle->out_fp);
    }
    rockaa_capt_effect_destroy(handle);

    if (pcm_out)
        free(pcm_out);
    if (pcm_in)
        free(pcm_in);
    if (handle->out_buf)
    {
        free(handle->out_buf);
        handle->out_buf = NULL;
    }
    if (handle->in_fp)
    {
        fclose(handle->in_fp);
        handle->in_fp = NULL;
    }
    if (handle->out_fp)
    {
        fclose(handle->out_fp);
        handle->out_fp = NULL;
    }
    if (handle)
    {
        free(handle);
        handle = NULL;
    }

    return 0;
}
