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

#ifndef _EXTCODEC_H_
#define _EXTCODEC_H_

#include <rthw.h>
#include <rtthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "audio_server.h"
#include "drv_dsp.h"

#define OPUS_ENCODE_RUN_ON_DSP          1
#define OPUS_DECODE_RUN_ON_DSP          0

#define DSP_CODEC_OPUS_ENCODE_OPEN      0x50000001
#define DSP_CODEC_OPUS_ENCODE_PROCESS   0x50000002
#define DSP_CODEC_OPUS_ENCODE_FINISH    0x50000003
#define DSP_CODEC_OPUS_ENCODE_CLOSE     0x50000004
#define DSP_CODEC_OPUS_DECODE_OPEN      0x50000005
#define DSP_CODEC_OPUS_DECODE_PROCESS   0x50000006
#define DSP_CODEC_OPUS_DECODE_FINISH    0x50000007
#define DSP_CODEC_OPUS_DECODE_CLOSE     0x50000008
#define DSP_CODEC_FLAC_DECODE_OPEN      0x50000009
#define DSP_CODEC_FLAC_DECODE_PROCESS   0x5000000A
#define DSP_CODEC_FLAC_DECODE_CLOSE     0x5000000B

#endif
