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

/* Based on: */

/* tinycap.c
**
** Copyright 2011, The Android Open Source Project
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of The Android Open Source Project nor the names of
**       its contributors may be used to endorse or promote products derived
**       from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY The Android Open Source Project ``AS IS'' AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED. IN NO EVENT SHALL The Android Open Source Project BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
** CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
** LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
** OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
** DAMAGE.
*/

#include <tinyalsa/asoundlib.h>
#include <signal.h>
#include <libgen.h>
#include <limits.h>

#define OPTPARSE_IMPLEMENTATION
#include "optparse.h"

#include "rockaa_capt.h"

#define OUTNAME_PREFIX          "TALK_"

#define ID_RIFF 0x46464952
#define ID_WAVE 0x45564157
#define ID_FMT  0x20746d66
#define ID_DATA 0x61746164

#define FORMAT_PCM 1

static int capturing = 1;
static int prinfo = 1;

static void sigint_handler(int sig)
{
    if (sig == SIGINT)
    {
        capturing = 0;
    }
}

static
unsigned int capture_sample(FILE *file, unsigned int card, unsigned int device,
                            bool use_mmap, unsigned int channels, unsigned int rate,
                            enum pcm_format format, unsigned int period_size,
                            unsigned int period_count, unsigned int capture_time,
                            rockaa_c *handle)
{
    struct pcm_config config;
    unsigned int pcm_open_flags;
    struct pcm *pcm;
    char *buffer;
    unsigned int size;
    unsigned int frames_read;
    unsigned int total_frames_read;
    unsigned int bytes_per_frame;
    unsigned int read_len;

    memset(&config, 0, sizeof(config));
    config.channels = channels;
    config.rate = rate;
    config.period_size = period_size;
    config.period_count = period_count;
    config.format = format;
    config.start_threshold = 0;
    config.stop_threshold = 0;
    config.silence_threshold = 0;

    pcm_open_flags = PCM_IN;
    if (use_mmap)
        pcm_open_flags |= PCM_MMAP;

    pcm = pcm_open(card, device, pcm_open_flags, &config);
    if (!pcm || !pcm_is_ready(pcm))
    {
        fprintf(stderr, "Unable to open PCM device (%s)\n",
                pcm_get_error(pcm));
        return 0;
    }

    read_len = handle ? period_size : pcm_get_buffer_size(pcm);
    LOGI("handle is:%p and read_len:%d\n", handle, read_len);

    size = pcm_frames_to_bytes(pcm, read_len);
    buffer = malloc(size);
    if (!buffer)
    {
        fprintf(stderr, "Unable to allocate %u bytes\n", size);
        pcm_close(pcm);
        return 0;
    }

    if (prinfo)
    {
        printf("Capturing sample: %u ch, %u hz, %u bit\n", channels, rate,
               pcm_format_to_bits(format));
    }

    bytes_per_frame = pcm_frames_to_bytes(pcm, 1);
    total_frames_read = 0;
    frames_read = 0;

    if (handle)
    {
        rockaa_capt_effect_create(handle);
        /**
         * handle->ch_out is configured after rockaa_capt_effect_create()
         * if BF model is enabled, then ch_out == 1, otherwise ch_out == ch_near
         */
        handle->out_bytes = sizeof(short) * period_size * handle->ch_out;
        handle->out_buf = (char *)malloc(handle->out_bytes);
        if (!handle->out_buf)
        {
            fprintf(stderr, "Unable to allocate out_buf\n");
            goto exit_capt;
        }
    }

    while (capturing)
    {
        frames_read = pcm_readi(pcm, buffer, read_len);
        total_frames_read += frames_read;
        if ((total_frames_read / rate) >= capture_time)
        {
            capturing = 0;
        }
        if (fwrite(buffer, bytes_per_frame, frames_read, file) != frames_read)
        {
            fprintf(stderr, "Error capturing sample\n");
            break;
        }
        /* process and dump output file */
        if (handle)
        {
            rockaa_capt_effect_process(handle, buffer, handle->out_buf);
            if (handle->enable_doa &&
                    rockaa_capt_effect_results(handle) == STATUS_RES_SUCCESS)
            {
                LOGI("Got DOA Angles - horizon: %d pitch: %d\n",
                     __func__, __LINE__,
                     handle->results.angle_horiz,
                     handle->results.angle_pitch);
            }
            if (fwrite(handle->out_buf, 1, handle->out_bytes, handle->out_fp) != handle->out_bytes)
            {
                fprintf(stderr, "Error output rockaa samples\n");
                break;
            }
        }
    }
    if (handle)
        rockaa_capt_effect_destroy(handle);

exit_capt:
    free(buffer);
    pcm_close(pcm);
    return total_frames_read;
}

static int offline_file_process(rockaa_c *handle, char *in_file)
{
    short *pcm_in, *pcm_out;
    int read_len;

    LOGI("rockaa capture process via [offline] file start, in_file: %s\n", in_file);

    if (!handle->out_fp)
    {
        LOGE("out_fp is NULL\n");
        return -1;
    }

    handle->in_fp = fopen(in_file, "rb");
    if (!handle->in_fp)
    {
        LOGE("%s in_fp fopen failed\n", in_file);
        return -1;
    }

    /* Handle wav header and avoid to incorrect order of PCM channels */
    if (rockaa_common_handle_wav(handle->in_fp, in_file) < 0)
        return -1;

    pcm_in = (short *)malloc(handle->nb_samples * sizeof(short) * handle->channels);
    if (!pcm_in)
    {
        LOGE("alloc pcm_in failed\n");
        return -1;
    }

    rockaa_capt_effect_create(handle);
    /**
     * handle->ch_out is configured after rockaa_capt_effect_create()
     * if BF model is enabled, then ch_out == 1, otherwise ch_out == ch_near
     */
    pcm_out = (short *)malloc(handle->nb_samples * sizeof(short) * handle->ch_out);
    if (!pcm_out)
    {
        LOGE("alloc pcm_out failed\n");
        return -1;
    }
    while (capturing && !feof(handle->in_fp))
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
               handle->nb_samples * sizeof(short) * handle->ch_out,
               handle->out_fp);
    }

    rockaa_capt_effect_destroy(handle);

    if (pcm_out)
        free(pcm_out);
    if (pcm_in)
        free(pcm_in);

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

    LOGI("rockaa capture process via [offline] file end\n");
    return 0;
}

static void usage(char *command)
{
    printf(
        "Usage: %s [OPTION]...\n"
        "\n"
        "-h, help\n"
        "-v  print current version\n"
        "-b, bits of samples\n"
        "-r, the sample rate for stream (default: 16000)\n"
        "-c, the number of channels (default: 2)\n"
        "-C, the number of far-end channels (default: 1)\n"
        "-p, the size of per-period (default: 256)\n"
        "-n, the number of periods (default: 4)\n"
        "-t, capture time (default: UINT_MAX)\n"
        "-a, enable DOA and return result (default: disabled)\n"
        "-F, the path of json config file, without it capture raw pcm directly\n"
        "-I, use offline input file mode, without capture via sound card\n"
        "-Z, use debug mode, 0:disabled 1:summary 2:per-frame 3:summary+per-frame (default: 0)\n"
        "For example:\n"
        "    %s cap.wav -c 2 -C 1 -b 16 -r 16000 -t 10 -Z 1 -F config_aivqe.json\n"
        ,
        command, command);
}

int main(int argc, char **argv)
{
    FILE *file = NULL;
    struct wav_header header;
    unsigned int card = 0;
    unsigned int device = 0;
    unsigned int channels = 2, ch_far = 1;
    unsigned int rate = 16000;
    unsigned int bits = 16;
    unsigned int frames;
    unsigned int period_size = 256;
    unsigned int period_count = 4;
    unsigned int capture_time = UINT_MAX;
    unsigned int debug_mode = 0;
    bool use_mmap = false;
    enum pcm_format format;
    int no_header = 0, enable_doa = 0, c;
    struct optparse opts;
    char *in_name = NULL, *conf_name = NULL;
    char *command = argv[0], *in_file = NULL, *out_file = NULL;
    rockaa_c *handle = NULL;

    if (argc < 2)
    {
        usage(command);
        return 1;
    }

    if (strcmp(argv[1], "--") == 0)
    {
        file = stdout;
        prinfo = 0;
        no_header = 1;
    }
    else if (strcmp(argv[1], "-h") == 0)
    {
        usage(command);
        return 1;
    }
    else if (strcmp(argv[1], "-v") == 0)
    {
        rockaa_version("Talk Capture");
        return 1;
    }
    else if (strcmp(argv[1], "-I") == 0)
    {
        fprintf(stderr, "input file: %s\n", argv[2]);
        in_name = strdup(argv[2]);
        in_file = in_name;
    }
    else
    {
        file = fopen(argv[1], "wb");
        if (!file)
        {
            fprintf(stderr, "Unable to create file '%s'\n", argv[1]);
            return 1;
        }
        in_name = strdup(argv[1]);
    }

    /* parse command line arguments */
    optparse_init(&opts, argv + 1);
    while ((c = optparse(&opts, "aMD:d:c:C:r:b:p:n:t:O:F:Z:")) != -1)
    {
        switch (c)
        {
        case 'd':
            device = atoi(opts.optarg);
            break;
        case 'c':
            channels = atoi(opts.optarg);
            break;
        case 'C':
            ch_far = atoi(opts.optarg);
            break;
        case 'r':
            rate = atoi(opts.optarg);
            break;
        case 'b':
            bits = atoi(opts.optarg);
            break;
        case 'D':
            card = atoi(opts.optarg);
            break;
        case 'p':
            period_size = atoi(opts.optarg);
            break;
        case 'n':
            period_count = atoi(opts.optarg);
            break;
        case 't':
            capture_time = atoi(opts.optarg);
            break;
        case 'a':
            enable_doa = 1;
            break;
        case 'F':
            conf_name = strdup(opts.optarg);
            break;
        case 'O':
            out_file = strdup(opts.optarg);
            break;
        case 'M':
            use_mmap = true;
            break;
        case 'Z':
            debug_mode = atoi(opts.optarg);
            break;
        case '?':
            fprintf(stderr, "%s\n", opts.errmsg);
            return EXIT_FAILURE;
        }
    }

    header.riff_id = ID_RIFF;
    header.riff_sz = 0;
    header.riff_fmt = ID_WAVE;
    header.fmt_id = ID_FMT;
    header.fmt_sz = 16;
    header.audio_format = FORMAT_PCM;
    header.num_channels = channels;
    header.sample_rate = rate;

    switch (bits)
    {
    case 32:
        format = PCM_FORMAT_S32_LE;
        break;
    case 24:
        format = PCM_FORMAT_S24_LE;
        break;
    case 16:
        format = PCM_FORMAT_S16_LE;
        break;
    default:
        fprintf(stderr, "%u bits is not supported.\n", bits);
        if (file)
            fclose(file);
        return 1;
    }

    header.bits_per_sample = pcm_format_to_bits(format);
    header.byte_rate = (header.bits_per_sample / 8) * channels * rate;
    header.block_align = channels * (header.bits_per_sample / 8);
    header.data_id = ID_DATA;

    /* leave enough room for header */
    if (!no_header && file)
    {
        fseek(file, sizeof(struct wav_header), SEEK_SET);
    }

    /* install signal handler and begin capturing */
    signal(SIGINT, sigint_handler);

    if (conf_name)
    {
        handle = (rockaa_c *)malloc(sizeof(rockaa_c));
        if (!handle)
        {
            LOGE("alloc talk handle failed\n");
            return -1;
        }
        memset(handle, 0, sizeof(rockaa_c));

        handle->rate = rate;
        handle->channels = channels;
        handle->bits = bits;
        handle->ch_far = ch_far;
        handle->ch_near = channels - ch_far;
        handle->debug_mode = debug_mode;
        handle->nb_samples = period_size;
        handle->enable_doa = enable_doa;
        handle->conf_path = conf_name;

        if (out_file)
        {
            handle->out_fp = fopen(out_file, "wb");
            if (handle->out_fp == NULL)
            {
                LOGE("%s out_fp fopen failed\n", out_file);
                return -1;
            }
            free(out_file);
            out_file = NULL;
        }
        else
        {
            char *dirc, *basec, *bname, *dname;

            out_file = (char *)malloc(strlen(OUTNAME_PREFIX) + strlen(in_name) + 32);
            if (out_file == NULL)
            {
                LOGE("alloc out_file failed\n");
                return -1;
            }

            dirc = strdup(in_name);
            basec = strdup(in_name);
            dname = dirname(dirc);
            bname = basename(basec);

            /* AEC output is fixed mono channel */
            if (!strcmp(dname, "."))
                sprintf(out_file, "%s%dch_%db_%d_%s", OUTNAME_PREFIX,
                        1, handle->bits, handle->rate, bname);
            else
                sprintf(out_file, "%s/%s%dch_%db_%d_%s", dname, OUTNAME_PREFIX,
                        1, handle->bits, handle->rate, bname);

            if (dirc)
            {
                free(dirc);
                dirc = NULL;
            }
            if (basec)
            {
                free(basec);
                basec = NULL;
            }

            handle->out_fp = fopen(out_file, "wb");
            if (handle->out_fp == NULL)
            {
                LOGE("%s out_fp fopen failed\n", out_file);
                return -1;
            }

            LOGI("in_file:%s out_file:%s\n", in_file, out_file);
            if (out_file)
            {
                free(out_file);
                out_file = NULL;
            }
        }

        LOGI("rate=%d channels=%d ch_near=%d ch_far=%d nb_samples=%d debug_mode=%d conf_path:%s\n",
             handle->rate,
             handle->channels,
             handle->ch_near,
             handle->ch_far,
             handle->nb_samples,
             handle->debug_mode,
             handle->conf_path);

        if (in_file)
        {
            offline_file_process(handle, in_file);
            goto finish_talk;
        }
    }
    else
    {
        if (in_file)
        {
            LOGE("Can't support in_file: %s without conf file\n", in_file);
            goto finish_talk;
        }

        LOGI("Not specify conf_path and capture raw pcm directly\n");
    }

    LOGI("rockaa capture process via [online] run-time start\n");
    LOGI("in_file:%s, in_name:%s, conf_path:%s\n",
         in_file, in_name, handle ? handle->conf_path : "NULL");

    frames = capture_sample(file, card, device, use_mmap,
                            header.num_channels, header.sample_rate,
                            format, period_size, period_count, capture_time,
                            handle);
    LOGI("rockaa capture process via [online] run-time end\n");
    if (prinfo)
    {
        printf("Captured %u frames\n", frames);
    }

finish_talk:
    if (in_name)
    {
        free(in_name);
        in_name = NULL;
    }

    /* write header now all information is known */
    if (!no_header && file)
    {
        header.data_sz = frames * header.block_align;
        header.riff_sz = header.data_sz + sizeof(header) - 8;
        fseek(file, 0, SEEK_SET);
        fwrite(&header, sizeof(struct wav_header), 1, file);
    }

    if (file)
        fclose(file);
    if (handle)
    {
        if (handle->out_fp)
        {
            header.num_channels = 1;
            header.data_sz = frames * header.block_align / 2;
            header.riff_sz = header.data_sz + sizeof(header) - 8;
            fseek(handle->out_fp, 0, SEEK_SET);
            fwrite(&header, sizeof(struct wav_header), 1, handle->out_fp);
            fclose(handle->out_fp);
            if (handle->out_buf)
            {
                free(handle->out_buf);
                handle->out_buf = NULL;
            }
            handle->out_fp = NULL;
        }
        free(handle);
        handle = NULL;
    }

    LOGI("RockAA Capture Finish\n");
    return 0;
}
