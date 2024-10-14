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

#ifndef __ROCKAA_COMMON_H__
#define __ROCKAA_COMMON_H__

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define rt_memset memset
#define rt_malloc(x) calloc(1, x)
#define rt_safe_free(x) do { if (x) free(x); } while (0)
#define RT_NULL     NULL
#define RT_NONE     "none"
#define RT_BOOL     char
#define RT_TRUE     1
#define RT_FALSE    0

#define RT_ARRAY_ELEMS(a)      (sizeof(a) / sizeof((a)[0]))

#define ROCKAA_VERSION     "V1.1.0"
#define DEFAULT_FRAME_GAP  16

#define ROCKAA_ALSA_BIT_FORMAT      0x8000000

typedef enum _alsa_snd_pcm_format_part {
    ALSA_SND_PCM_FORMAT_S16 = 2,
} alsa_snd_pcm_format_part;

typedef int8_t       INT8;
typedef int16_t      INT16;
typedef int32_t      INT32;
typedef int64_t      INT64;

typedef uint8_t      UINT8;
typedef uint8_t      UCHAR;
typedef uint16_t     UINT16;
typedef uint32_t     UINT32;
typedef uint64_t     UINT64;

typedef void        *RT_PTR;
typedef void         RT_VOID;
typedef float        RT_FLOAT;
typedef short        RT_SHORT;

typedef enum _ROCKAA_DEBUG
{
    DEBUG_SUMMARY    = 1 << 0,
    DEBUG_PER_FRAME  = 1 << 1,
} ROCKAA_DEBUG;

struct wav_header
{
    uint32_t riff_id;
    uint32_t riff_sz;
    uint32_t riff_fmt;
    uint32_t fmt_id;
    uint32_t fmt_sz;
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    uint32_t data_id;
    uint32_t data_sz;
};

#define LOGV(format, ...)                                     \
  do {                                                        \
  } while (0)

#define LOGD(format, ...)                                     \
  do {                                                        \
    fprintf(stderr, "[tagD] " format, ##__VA_ARGS__);         \
    if(format[strlen(format) - 1] != '\n')                    \
      fprintf(stderr, "\n");                                  \
  } while (0)

#define LOGI(format, ...)                                     \
  do {                                                        \
    fprintf(stderr, "[tagI] " format, ##__VA_ARGS__);         \
    if(format[strlen(format) - 1] != '\n')                    \
      fprintf(stderr, "\n");                                  \
  } while (0)

#define LOGE(format, ...)                                     \
  do {                                                        \
    fprintf(stderr, "[tagE] " format, ##__VA_ARGS__);         \
    if(format[strlen(format) - 1] != '\n')                    \
      fprintf(stderr, "\n");                                  \
  } while (0)

#define RT_ASSERT assert

inline static void rockaa_version(char *tag)
{
    printf("Test for Rock Audio Algorithm %s Version: %s\n",
           tag, ROCKAA_VERSION);
}

int rockaa_common_handle_wav(FILE *in_fp, char *in_file);

#endif /* __ROCKAA_COMMON_H__ */
