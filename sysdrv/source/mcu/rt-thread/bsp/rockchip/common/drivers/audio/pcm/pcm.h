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
/**
 * \file include/pcm.h
 * \brief Application interface library for the ALSA driver
 * \author Jaroslav Kysela <perex@perex.cz>
 * \author Abramo Bagnara <abramo@alsa-project.org>
 * \author Takashi Iwai <tiwai@suse.de>
 * \date 1998-2001
 *
 * Application interface library for the ALSA driver.
 * See the \ref pcm page for more details.
 */
/*
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

#ifndef __DRIVER_AUDIO_PCM_H__
#define __DRIVER_AUDIO_PCM_H__

#include "pcm_types.h"
#include "pcm_plugin.h"

int snd_pcm_start(snd_pcm_t *pcm);
int snd_pcm_drop(snd_pcm_t *pcm);
int snd_pcm_hwsync(snd_pcm_t *pcm);
int snd_pcm_close(snd_pcm_t *pcm);
snd_pcm_state_t snd_pcm_state(snd_pcm_t *pcm);
snd_pcm_sframes_t snd_pcm_avail_update(snd_pcm_t *pcm);

/**
 * \defgroup PCM_Direct Direct Access (MMAP) Functions
 * \ingroup PCM
 * See the \ref pcm page for more details.
 * \{
 */
int snd_pcm_mmap_begin(snd_pcm_t *pcm,
                       const snd_pcm_channel_area_t **areas,
                       snd_pcm_uframes_t *offset,
                       snd_pcm_uframes_t *frames);
snd_pcm_sframes_t snd_pcm_mmap_commit(snd_pcm_t *pcm,
                                      snd_pcm_uframes_t offset,
                                      snd_pcm_uframes_t frames);

int snd_pcm_format_little_endian(snd_pcm_format_t format);
int snd_pcm_format_big_endian(snd_pcm_format_t format);
int snd_pcm_format_cpu_endian(snd_pcm_format_t format);
int snd_pcm_format_physical_width(snd_pcm_format_t format);     /* in bits */
uint16_t snd_pcm_format_silence_16(snd_pcm_format_t format);
uint32_t snd_pcm_format_silence_32(snd_pcm_format_t format);
uint64_t snd_pcm_format_silence_64(snd_pcm_format_t format);

int snd_pcm_area_silence(const snd_pcm_channel_area_t *dst_channel, snd_pcm_uframes_t dst_offset,
                         unsigned int samples, snd_pcm_format_t format);
int snd_pcm_areas_silence(const snd_pcm_channel_area_t *dst_channels, snd_pcm_uframes_t dst_offset,
                          unsigned int channels, snd_pcm_uframes_t frames, snd_pcm_format_t format);
int snd_pcm_area_copy(const snd_pcm_channel_area_t *dst_channel, snd_pcm_uframes_t dst_offset,
                      const snd_pcm_channel_area_t *src_channel, snd_pcm_uframes_t src_offset,
                      unsigned int samples, snd_pcm_format_t format);
int snd_pcm_areas_copy(const snd_pcm_channel_area_t *dst_channels, snd_pcm_uframes_t dst_offset,
                       const snd_pcm_channel_area_t *src_channels, snd_pcm_uframes_t src_offset,
                       unsigned int channels, snd_pcm_uframes_t frames, snd_pcm_format_t format);

static inline snd_pcm_format_t to_snd_pcm_format(eAUDIO_sampleBits bits)
{
    snd_pcm_format_t format;

    switch (bits)
    {
    case AUDIO_SAMPLEBITS_8:
        format = SND_PCM_FORMAT_S8;
        break;
    case AUDIO_SAMPLEBITS_16:
        format = SND_PCM_FORMAT_S16_LE;
        break;
    case AUDIO_SAMPLEBITS_18:
        format = SND_PCM_FORMAT_S18_3LE;
        break;
    case AUDIO_SAMPLEBITS_20:
        format = SND_PCM_FORMAT_S16_LE;
        break;
    case AUDIO_SAMPLEBITS_24:
        format = SND_PCM_FORMAT_S24_LE;
        break;
    case AUDIO_SAMPLEBITS_32:
        format = SND_PCM_FORMAT_S32_LE;
        break;
    default:
        format = SND_PCM_FORMAT_UNKNOWN;
        break;
    }

    return format;
}

#endif /* __DRIVER_AUDIO_PCM_H__*/
