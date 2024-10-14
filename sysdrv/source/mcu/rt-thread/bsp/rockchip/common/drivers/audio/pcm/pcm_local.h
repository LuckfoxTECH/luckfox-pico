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

#ifndef __DRIVER_AUDIO_PCM_LOCAL_H__
#define __DRIVER_AUDIO_PCM_LOCAL_H__

#include "pcm.h"
#include "pcm_os_interface.h"
#include "pcm_errno.h"

#define CHECK_SANITY(x) x

snd_pcm_t *snd_pcm_plugin_hw_get(snd_pcm_t *pcm);

typedef snd_pcm_sframes_t (*snd_pcm_xfer_areas_func_t)(snd_pcm_t *pcm,
        const snd_pcm_channel_area_t *areas,
        snd_pcm_uframes_t offset,
        snd_pcm_uframes_t size);

static inline snd_pcm_sframes_t __snd_pcm_avail_update(snd_pcm_t *pcm)
{
    return pcm->fast_ops->avail_update(pcm);
}

static inline int __snd_pcm_start(snd_pcm_t *pcm)
{
    return pcm->fast_ops->start(pcm);
}

static inline snd_pcm_state_t __snd_pcm_state(snd_pcm_t *pcm)
{
    return pcm->fast_ops->state(pcm);
}

static inline int __snd_pcm_hwsync(snd_pcm_t *pcm)
{
    return pcm->fast_ops->hwsync(pcm);
}

/* handle special error cases */
static inline int snd_pcm_check_error(snd_pcm_t *pcm, int err)
{
    if (err == -EINTR)
    {
        switch (__snd_pcm_state(pcm))
        {
        default:
            break;
        }
    }
    return err;
}

static inline snd_pcm_uframes_t snd_pcm_mmap_playback_avail(snd_pcm_t *pcm)
{
    return audio_pcm_playback_avail(pcm);
}

static inline snd_pcm_uframes_t snd_pcm_mmap_capture_avail(snd_pcm_t *pcm)
{
    return audio_pcm_capture_avail(pcm);
}

static inline snd_pcm_uframes_t snd_pcm_mmap_avail(snd_pcm_t *pcm)
{
    if (pcm->as->stream == SND_PCM_STREAM_PLAYBACK)
        return snd_pcm_mmap_playback_avail(pcm);
    else
        return snd_pcm_mmap_capture_avail(pcm);
}

static inline void snd_pcm_mmap_appl_backward(snd_pcm_t *pcm, snd_pcm_uframes_t frames)
{
    audio_pcm_appl_backward(pcm, frames);
}

static inline void snd_pcm_mmap_appl_forward(snd_pcm_t *pcm, snd_pcm_uframes_t frames)
{
    audio_pcm_appl_forward(pcm, frames);
}

static inline void snd_pcm_mmap_hw_backward(snd_pcm_t *pcm, snd_pcm_uframes_t frames)
{
    audio_pcm_hw_backward(pcm, frames);
}

static inline void snd_pcm_mmap_hw_forward(snd_pcm_t *pcm, snd_pcm_uframes_t frames)
{
    audio_pcm_hw_forward(pcm, frames);
}

static inline void *snd_pcm_channel_area_addr(const snd_pcm_channel_area_t *area, snd_pcm_uframes_t offset)
{
    unsigned int bitofs = area->first + area->step * offset;

    snd_assert(bitofs % 8 == 0);
    return (char *) area->addr + bitofs / 8;
}

static inline unsigned int snd_pcm_channel_area_step(const snd_pcm_channel_area_t *area)
{
    snd_assert(area->step % 8 == 0);
    return area->step / 8;
}

static inline const snd_pcm_channel_area_t *snd_pcm_mmap_areas(snd_pcm_t *pcm)
{
    return pcm->running_areas;
}

/* TODO: locks for pcm plugins */
#define __snd_pcm_lock(pcm)     do {} while (0)
#define __snd_pcm_unlock(pcm)       do {} while (0)
#define snd_pcm_lock(pcm)       do {} while (0)
#define snd_pcm_unlock(pcm)     do {} while (0)

int snd_pcm_new(snd_pcm_t **pcmp);
int snd_pcm_free(snd_pcm_t *pcm);

void snd_pcm_areas_from_buf(snd_pcm_t *pcm, snd_pcm_channel_area_t *areas, void *buf);

void snd_pcm_mmap_appl_backward(snd_pcm_t *pcm, snd_pcm_uframes_t frames);
void snd_pcm_mmap_appl_forward(snd_pcm_t *pcm, snd_pcm_uframes_t frames);
void snd_pcm_mmap_hw_backward(snd_pcm_t *pcm, snd_pcm_uframes_t frames);
void snd_pcm_mmap_hw_forward(snd_pcm_t *pcm, snd_pcm_uframes_t frames);

snd_pcm_sframes_t snd_pcm_read_areas(snd_pcm_t *pcm, const snd_pcm_channel_area_t *areas,
                                     snd_pcm_uframes_t offset, snd_pcm_uframes_t size,
                                     snd_pcm_xfer_areas_func_t func);
snd_pcm_sframes_t snd_pcm_write_areas(snd_pcm_t *pcm, const snd_pcm_channel_area_t *areas,
                                      snd_pcm_uframes_t offset, snd_pcm_uframes_t size,
                                      snd_pcm_xfer_areas_func_t func);

#endif /* __DRIVER_AUDIO_PCM_LOCAL_H__ */
