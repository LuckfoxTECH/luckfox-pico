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
 * \file pcm/pcm_hw.c
 * \ingroup PCM_Plugins
 * \brief PCM HW Plugin Interface
 * \author Abramo Bagnara <abramo@alsa-project.org>
 * \author Jaroslav Kysela <perex@perex.cz>
 * \date 2000-2001
 */
/*
 *  PCM - Hardware
 *  Copyright (c) 2000 by Abramo Bagnara <abramo@alsa-project.org>
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

#ifdef RT_USING_DRIVER_AUDIO_PCM_PLUGIN_HW

#include "pcm_os_interface.h"
#include "pcm_local.h"
#include "pcm_plugin.h"

/**
 * Because the master pcm manage the state and hw/appl ptrs, so we need to
 * change to master pcm and synchronize hw/appl ptrs to hardware pcm's.
 */
static snd_pcm_state_t snd_pcm_hw_state(snd_pcm_t *pcmhw)
{
    snd_pcm_t *pcm = pcmhw->fast_op_arg;
    return (snd_pcm_state_t)audio_stream_get_state(pcm->as);
}

static int snd_pcm_hw_hwsync(snd_pcm_t *pcmhw)
{
    snd_pcm_t *pcm = pcmhw->fast_op_arg;

    /* Sync the hw / appl ptr from softvol to hw plugin */
    pcmhw->status.hw_ptr = pcm->status.hw_ptr;
    pcmhw->status.appl_ptr = pcm->status.appl_ptr;

    return 0;
}

static int snd_pcm_hw_prepare(snd_pcm_t *pcmhw)
{
    /* DO NOTHING */
    return 0;
}

static int snd_pcm_hw_start(snd_pcm_t *pcmhw)
{
    snd_pcm_t *pcm = pcmhw->fast_op_arg;
    return rk_audio_hw_start(pcm->as);
}

static int snd_pcm_hw_drop(snd_pcm_t *pcmhw)
{
    snd_pcm_t *pcm = pcmhw->fast_op_arg;
    return rk_audio_hw_stop(pcm->as);
}

static snd_pcm_sframes_t snd_pcm_hw_mmap_commit(snd_pcm_t *pcmhw,
        snd_pcm_uframes_t offset ATTRIBUTE_UNUSED,
        snd_pcm_uframes_t size)
{
    /* We don't need to forward appl ptr here, just reture size directly. */
    return size;
}

static snd_pcm_sframes_t snd_pcm_hw_avail_update(snd_pcm_t *pcmhw)
{
    snd_pcm_t *pcm = pcmhw->fast_op_arg;
    struct audio_stream *as = pcm->as;
    snd_pcm_uframes_t avail;
    int ret;

    do
    {
        if (as->stream == AUDIO_STREAM_PLAYBACK)
            avail = audio_pcm_playback_avail(pcm);
        else
            avail = audio_pcm_capture_avail(pcm);

        if (avail < pcm->abuf.period_size)
        {
            ret = rk_audio_hw_wait_for_avail(as);
            if (ret != SND_SUCCESS)
            {
                SND_ERR("ERR: %s pcm: %s, wait avail: %d, err: %d\n",
                        __func__, pcm->name, avail, ret);
                goto err;
            }
        }
    }
    while (avail < pcm->abuf.period_size);

    switch (audio_stream_get_state(as))
    {
    case SND_PCM_STATE_RUNNING:
        if (avail >= pcm->stop_threshold)
        {
            /* Do nothing */
        }
        break;
    case SND_PCM_STATE_XRUN:
        /* Do nothing */
        break;
    default:
        break;
    }

err:
    return avail;
}

static const snd_pcm_fast_ops_t snd_pcm_hw_fast_ops =
{
    .state = snd_pcm_hw_state,
    .hwsync = snd_pcm_hw_hwsync,
    .prepare = snd_pcm_hw_prepare,
    .start = snd_pcm_hw_start,
    .drop = snd_pcm_hw_drop,
    .avail_update = snd_pcm_hw_avail_update,
    .mmap_commit = snd_pcm_hw_mmap_commit,
};

static int snd_pcm_hw_hw_params(snd_pcm_t *pcm, snd_pcm_stream_t stream,
                                snd_pcm_hw_params_t *params)
{
    int ch;

    pcm->format = to_snd_pcm_format(params->sampleBits);

    /* malloc for running_areas */
    pcm->running_areas = snd_zmalloc(params->channels * sizeof(pcm->running_areas[0]));
    if (pcm->running_areas == NULL)
    {
        SND_ERR("ERR - alloc running_areas failed!\n");
        return SND_ERROR;
    }

    /* Fill running_areas from channel info. */
    for (ch = 0; ch < params->channels; ch++)
    {
        snd_pcm_channel_area_t *running_areas = &(pcm->running_areas[ch]);
        int width = snd_pcm_format_physical_width(pcm->format);

        running_areas->step = width * params->channels;
        running_areas->first = ch * width;
        running_areas->addr = pcm->abuf.buf;
    }

    return 0;
}

static const snd_pcm_plugin_ops_t snd_pcm_hw_plugin_ops =
{
    .hw_params = snd_pcm_hw_hw_params,
};

static int snd_pcm_hw_open(snd_pcm_t *pcm, snd_pcm_t **pcmout)
{
    int err;

    err = snd_pcm_new(pcmout);
    if (err < 0)
    {
        SND_ERR("ERR - snd_pcm_new failed: %d\n", err);
        return err;
    }

    /* Copied some common configuarations from master pcm. */
    if (pcm)
        **pcmout = *pcm;

    (*pcmout)->type = SND_PCM_TYPE_HW;
    (*pcmout)->plugin_ops = &snd_pcm_hw_plugin_ops;
    (*pcmout)->fast_ops = &snd_pcm_hw_fast_ops;
    snd_memset((*pcmout)->name, 0, sizeof((*pcmout)->name));
    strcpy((*pcmout)->name, "hw");

    return 0;
}

static int snd_pcm_hw_close(snd_pcm_t *pcm)
{
    /* free for running_areas */
    if (pcm->running_areas)
        snd_free(pcm->running_areas);

    snd_free(pcm);

    return 0;
}

static void snd_pcm_hw_dump(snd_pcm_t *pcm)
{
    SND_INFO("==== Hardware PCM ====\n");
    SND_INFO("pcm name: %s\n", pcm->name);
}

static int snd_pcm_plugin_hw_load(snd_pcm_t *pcm, snd_pcm_t *slave,
                                  snd_pcm_t **pcmout)
{
    int ret;

    ret = snd_pcm_hw_open(pcm, pcmout);
    if (ret < 0)
    {
        SND_ERR("ERR: pcm hw open something wrong: %d\n", ret);
        goto err;
    }

    snd_pcm_hw_dump(*pcmout);
err:
    return ret;
}

static int snd_pcm_plugin_hw_unload(snd_pcm_t *pcm, snd_pcm_t **pcmout)
{
    /* pcm hardware is the tail plugin. */
    *pcmout = NULL;
    return snd_pcm_hw_close(pcm);
}

const snd_pcm_plugin_manager_t pcm_hw_plugin =
{
    .load = snd_pcm_plugin_hw_load,
    .unload = snd_pcm_plugin_hw_unload,
};

#endif /* CONFIG_DRIVER_AUDIO_PCM_PLUGIN_HW */
