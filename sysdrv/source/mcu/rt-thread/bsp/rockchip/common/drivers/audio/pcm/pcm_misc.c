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
 *  PCM Interface - misc routines
 *  Copyright (c) 1998 by Jaroslav Kysela <perex@perex.cz>
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

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_DRIVER_AUDIO_PCM_PLUGIN

#include "pcm_os_interface.h"
#include "pcm_local.h"

/**
 * \brief Return bits needed to store a PCM sample
 * \param format Sample format
 * \return bits per sample, a negative error code if not applicable
 */
int snd_pcm_format_physical_width(snd_pcm_format_t format)
{
    switch (format)
    {
    case SND_PCM_FORMAT_S8:
        return 8;
    case SND_PCM_FORMAT_S16_LE:
        return 16;
    case SND_PCM_FORMAT_S18_3LE:
    case SND_PCM_FORMAT_S20_3LE:
    case SND_PCM_FORMAT_S24_3LE:
        return 24;
    case SND_PCM_FORMAT_S24_LE:
    case SND_PCM_FORMAT_S32_LE:
        return 32;
    default:
        return -EINVAL;
    }
}

/**
 * \brief Return 64 bit expressing silence for a PCM sample format
 * \param format Sample format
 * \return silence 64 bit word
 */
uint64_t snd_pcm_format_silence_64(snd_pcm_format_t format)
{
    switch (format)
    {
    case SND_PCM_FORMAT_S8:
    case SND_PCM_FORMAT_S16_LE:
    case SND_PCM_FORMAT_S24_LE:
    case SND_PCM_FORMAT_S32_LE:
    case SND_PCM_FORMAT_S24_3LE:
    case SND_PCM_FORMAT_S20_3LE:
    case SND_PCM_FORMAT_S18_3LE:
        return 0;
    default:
        snd_assert(0);
        return 0;
    }
    return 0;
}

/**
 * \brief Return 32 bit expressing silence for a PCM sample format
 * \param format Sample format
 * \return silence 32 bit word
 */
uint32_t snd_pcm_format_silence_32(snd_pcm_format_t format)
{
    snd_assert(snd_pcm_format_physical_width(format) <= 32);
    return (uint32_t)snd_pcm_format_silence_64(format);
}

/**
 * \brief Return 16 bit expressing silence for a PCM sample format
 * \param format Sample format
 * \return silence 16 bit word
 */
uint16_t snd_pcm_format_silence_16(snd_pcm_format_t format)
{
    snd_assert(snd_pcm_format_physical_width(format) <= 16);
    return (uint16_t)snd_pcm_format_silence_64(format);
}


/**
 * \brief Return endian info for a PCM sample format
 * \param format Format
 * \return 0 big endian, 1 little endian, a negative error code if endian independent
 */
int snd_pcm_format_little_endian(snd_pcm_format_t format)
{
    switch (format)
    {
    case SND_PCM_FORMAT_S16_LE:
    case SND_PCM_FORMAT_S24_LE:
    case SND_PCM_FORMAT_S32_LE:
    case SND_PCM_FORMAT_S20_3LE:
    case SND_PCM_FORMAT_S18_3LE:
        return 1;
    default:
        return -EINVAL;
    }
}

/**
 * \brief Return endian info for a PCM sample format
 * \param format Format
 * \return 0 little endian, 1 big endian, a negative error code if endian independent
 */
int snd_pcm_format_big_endian(snd_pcm_format_t format)
{
    int val;

    val = snd_pcm_format_little_endian(format);
    if (val < 0)
        return val;
    return !val;
}

/**
 * \brief Return endian info for a PCM sample format
 * \param format Format
 * \return 0 swapped, 1 CPU endian, a negative error code if endian independent
 */
int snd_pcm_format_cpu_endian(snd_pcm_format_t format)
{
#ifdef __BIG_ENDIAN
    return snd_pcm_format_big_endian(format);
#else
    return snd_pcm_format_little_endian(format);
#endif
}

#endif /* CONFIG_DRIVER_AUDIO_PCM_MISC */
