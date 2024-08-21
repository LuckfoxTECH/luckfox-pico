/*
 * SPDX-License-Identifier: (Apache-2.0 OR LGPL-2.1-or-later)
 *
 * Copyright (c) 2020 Fuzhou Rockchip Electronics Co., Ltd
 *
 * Change Logs:
 * Date           Author          Notes
 * 2020-03-03     Xing Zheng      The first version
 *
 * Based on
 */
/*
 *  PCM Interface - local header file
 *  Copyright (c) 2000 by Jaroslav Kysela <perex@perex.cz>
 *                        Abramo Bagnara <abramo@alsa-project.org>
 *
 *
 *   This library is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as
 *   published by the Free Software Foundation; either version 2.1 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */

#ifndef __DRIVER_AUDIO_PCM_TYPES_H__
#define __DRIVER_AUDIO_PCM_TYPES_H__

#define SND_NAME_MAX    16

#define min(x, y)   (x < y ? x : y)
#define max(x, y)   (x < y ? y : x)

typedef unsigned long uint32;

/** PCM handle */
typedef struct audio_pcm snd_pcm_t;
typedef struct audio_pcm_ops snd_pcm_ops_t;
typedef struct AUDIO_PARAMS snd_pcm_hw_params_t;

/** Unsigned frames quantity */
typedef unsigned long snd_pcm_uframes_t;
/** Signed frames quantity */
typedef signed long snd_pcm_sframes_t;

/** PCM stream (direction) */
typedef enum _snd_pcm_stream
{
    /** Playback stream */
    SND_PCM_STREAM_PLAYBACK = 0, // AUDIO_STREAM_PLAYBACK
    /** Capture stream */
    SND_PCM_STREAM_CAPTURE, // AUDIO_STREAM_CAPTURE
    /** Last stream */
    SND_PCM_STREAM_LAST = SND_PCM_STREAM_CAPTURE
} snd_pcm_stream_t;

/** PCM state */
typedef enum _snd_pcm_state
{
    /** Open */
    SND_PCM_STATE_OPEN, // = AUDIO_STREAM_STATE_OPEN,
    /** Setup installed */
    SND_PCM_STATE_SETUP, // = AUDIO_STREAM_STATE_SETUP,
    /** Ready to start */
    SND_PCM_STATE_PREPARED, // = AUDIO_STREAM_STATE_PREPARED,
    /** Running */
    SND_PCM_STATE_RUNNING, // = AUDIO_STREAM_STATE_RUNNING,
    /** Stopped: underrun (playback) or overrun (capture) detected */
    SND_PCM_STATE_XRUN, // = AUDIO_STREAM_STATE_XRUN,
    /** Draining: running (playback) or stopped (capture) */
    SND_PCM_STATE_DRAINING, // = AUDIO_STREAM_STATE_STOP,
    /** Last state */
    SND_PCM_STATE_LAST = SND_PCM_STATE_DRAINING,
} snd_pcm_state_t;

/** PCM sample format */
typedef enum _snd_pcm_format
{
    /** Unknown */
    SND_PCM_FORMAT_UNKNOWN = -1,
    /** Signed 8 bit */
    SND_PCM_FORMAT_S8 = 0,
    /** Signed 16 bit Little Endian */
    SND_PCM_FORMAT_S16_LE,
    /** Signed 24 bit Little Endian using low three bytes in 32-bit word */
    SND_PCM_FORMAT_S24_LE,
    /** Signed 32 bit Little Endian */
    SND_PCM_FORMAT_S32_LE,
    /** Signed 24bit Little Endian in 3bytes format */
    SND_PCM_FORMAT_S24_3LE,
    /** Signed 20bit Little Endian in 3bytes format */
    SND_PCM_FORMAT_S20_3LE,
    /** Signed 18bit Little Endian in 3bytes format */
    SND_PCM_FORMAT_S18_3LE,
    /** Last format */
    SND_PCM_FORMAT_LAST = SND_PCM_FORMAT_S18_3LE,
} snd_pcm_format_t;

/** PCM area specification */
typedef struct _snd_pcm_channel_area
{
    /** base address of channel samples */
    void *addr;
    /** offset to first sample in bits */
    unsigned int first;
    /** samples distance in bits */
    unsigned int step;
} snd_pcm_channel_area_t;

typedef struct
{
    int (*hw_params)(snd_pcm_t *pcm, snd_pcm_stream_t stream, snd_pcm_hw_params_t *params);
} snd_pcm_plugin_ops_t;

typedef struct
{
    int (*prepare)(snd_pcm_t *pcm); /* locked */
    int (*start)(snd_pcm_t *pcm); /* locked */
    int (*drop)(snd_pcm_t *pcm); /* locked */
    snd_pcm_state_t (*state)(snd_pcm_t *pcm); /* locked */
    int (*hwsync)(snd_pcm_t *pcm); /* locked */
    snd_pcm_sframes_t (*writei)(snd_pcm_t *pcm, const void *buffer, snd_pcm_uframes_t size); /* need own locking */
    snd_pcm_sframes_t (*readi)(snd_pcm_t *pcm, void *buffer, snd_pcm_uframes_t size); /* need own locking */
    snd_pcm_sframes_t (*avail_update)(snd_pcm_t *pcm); /* locked */
    snd_pcm_sframes_t (*mmap_commit)(snd_pcm_t *pcm, snd_pcm_uframes_t offset, snd_pcm_uframes_t size); /* locked */
} snd_pcm_fast_ops_t;

int snd_pcm_plugin_controls(snd_pcm_t *pcm, uint32 cmd, void *args);

rt_size_t snd_pcm_plugin_write(snd_pcm_t *pcm, const void *buffer, int size);
rt_size_t snd_pcm_plugin_read(snd_pcm_t *pcm, void *buffer, int size);

extern const snd_pcm_fast_ops_t snd_pcm_plugin_fast_ops;

#endif /* __DRIVER_AUDIO_PCM_TYPES_H__ */
