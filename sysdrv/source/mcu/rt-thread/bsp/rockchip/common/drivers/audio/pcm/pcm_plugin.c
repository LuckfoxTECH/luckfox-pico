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
 * \file pcm/pcm_plugin.c
 * \ingroup PCM
 * \brief PCM Interface
 * \author Jaroslav Kysela <perex@perex.cz>
 * \author Abramo Bagnara <abramo@alsa-project.org>
 * \date 2000-2001
 */
/*
 *  PCM - Common plugin code
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

#ifdef RT_USING_DRIVER_AUDIO_PCM_PLUGIN

#include "pcm_os_interface.h"
#include "pcm.h"
#include "pcm_local.h"
#include "pcm_plugin.h"

static snd_pcm_sframes_t
snd_pcm_plugin_undo_read(snd_pcm_t *pcm ATTRIBUTE_UNUSED,
                         const snd_pcm_channel_area_t *res_areas ATTRIBUTE_UNUSED,
                         snd_pcm_uframes_t res_offset ATTRIBUTE_UNUSED,
                         snd_pcm_uframes_t res_size ATTRIBUTE_UNUSED,
                         snd_pcm_uframes_t slave_undo_size ATTRIBUTE_UNUSED)
{
    return -EIO;
}

static snd_pcm_sframes_t
snd_pcm_plugin_undo_write(snd_pcm_t *pcm ATTRIBUTE_UNUSED,
                          const snd_pcm_channel_area_t *res_areas ATTRIBUTE_UNUSED,
                          snd_pcm_uframes_t res_offset ATTRIBUTE_UNUSED,
                          snd_pcm_uframes_t res_size ATTRIBUTE_UNUSED,
                          snd_pcm_uframes_t slave_undo_size ATTRIBUTE_UNUSED)
{
    return -EIO;
}

static snd_pcm_sframes_t snd_pcm_plugin_write_areas(snd_pcm_t *pcm,
        const snd_pcm_channel_area_t *areas,
        snd_pcm_uframes_t offset,
        snd_pcm_uframes_t size)
{
    snd_pcm_plugin_t *plugin = pcm->private_data;
    snd_pcm_t *slave = plugin->gen.slave;
    snd_pcm_uframes_t xfer = 0;
    snd_pcm_sframes_t result;
    int err;

    while (size > 0)
    {
        snd_pcm_uframes_t frames = size;
        const snd_pcm_channel_area_t *slave_areas;
        snd_pcm_uframes_t slave_offset;
        snd_pcm_uframes_t slave_frames = ULONG_MAX;

        result = snd_pcm_mmap_begin(slave, &slave_areas, &slave_offset, &slave_frames);
        if (result < 0)
        {
            err = result;
            goto error;
        }
        if (slave_frames == 0)
            break;

        frames = plugin->write(pcm, areas, offset, frames,
                               slave_areas, slave_offset, &slave_frames);
        if (CHECK_SANITY(slave_frames > snd_pcm_mmap_playback_avail(slave)))
        {
            SND_ERR("write overflow %ld > %ld", slave_frames,
                    snd_pcm_mmap_playback_avail(slave));
            err = -EPIPE;
            goto error;
        }
        result = snd_pcm_mmap_commit(slave, slave_offset, slave_frames);

        if (result > 0 && (snd_pcm_uframes_t)result != slave_frames)
        {
            snd_pcm_sframes_t res;
            res = plugin->undo_write(pcm, slave_areas, slave_offset + result, slave_frames, slave_frames - result);
            if (res < 0)
            {
                err = res;
                goto error;
            }
            frames -= res;
        }
        if (result <= 0)
        {
            err = result;
            goto error;
        }
        snd_pcm_mmap_appl_forward(pcm, frames);
        offset += frames;
        xfer += frames;
        size -= frames;
    }
    return (snd_pcm_sframes_t)xfer;

error:
    return xfer > 0 ? (snd_pcm_sframes_t)xfer : err;
}

static snd_pcm_sframes_t snd_pcm_plugin_read_areas(snd_pcm_t *pcm,
        const snd_pcm_channel_area_t *areas,
        snd_pcm_uframes_t offset,
        snd_pcm_uframes_t size)
{
    snd_pcm_plugin_t *plugin = pcm->private_data;
    snd_pcm_t *slave = plugin->gen.slave;
    snd_pcm_uframes_t xfer = 0;
    snd_pcm_sframes_t result;
    int err;

    while (size > 0)
    {
        snd_pcm_uframes_t frames = size;
        const snd_pcm_channel_area_t *slave_areas;
        snd_pcm_uframes_t slave_offset;
        snd_pcm_uframes_t slave_frames = ULONG_MAX;

        result = snd_pcm_mmap_begin(slave, &slave_areas, &slave_offset, &slave_frames);
        if (result < 0)
        {
            err = result;
            goto error;
        }
        if (slave_frames == 0)
            break;
        frames = (plugin->read)(pcm, areas, offset, frames,
                                slave_areas, slave_offset, &slave_frames);
        if (CHECK_SANITY(slave_frames > snd_pcm_mmap_capture_avail(slave)))
        {
            SND_ERR("read overflow %ld > %ld", slave_frames,
                    snd_pcm_mmap_playback_avail(slave));
            err = -EPIPE;
            goto error;
        }
        result = snd_pcm_mmap_commit(slave, slave_offset, slave_frames);
        if (result > 0 && (snd_pcm_uframes_t)result != slave_frames)
        {
            snd_pcm_sframes_t res;

            res = plugin->undo_read(slave, areas, offset, frames, slave_frames - result);
            if (res < 0)
            {
                err = res;
                goto error;
            }
            frames -= res;
        }
        if (result <= 0)
        {
            err = result;
            goto error;
        }
        snd_pcm_mmap_appl_forward(pcm, frames);
        offset += frames;
        xfer += frames;
        size -= frames;
    }
    return (snd_pcm_sframes_t)xfer;

error:
    return xfer > 0 ? (snd_pcm_sframes_t)xfer : err;
}

static snd_pcm_sframes_t snd_pcm_plugin_avail_update(snd_pcm_t *pcm)
{
    snd_pcm_plugin_t *plugin = pcm->private_data;
    snd_pcm_t *slave = plugin->gen.slave;
    snd_pcm_sframes_t slave_size;

    slave_size = snd_pcm_avail_update(slave);
    return slave_size;
}

static snd_pcm_sframes_t
snd_pcm_plugin_writei(snd_pcm_t *pcm, const void *buffer, snd_pcm_uframes_t size)
{
    snd_pcm_channel_area_t areas[pcm->params.channels];
    snd_pcm_areas_from_buf(pcm, areas, (void *)buffer);
    return snd_pcm_write_areas(pcm, areas, 0, size,
                               snd_pcm_plugin_write_areas);
}

static snd_pcm_sframes_t
snd_pcm_plugin_readi(snd_pcm_t *pcm, void *buffer, snd_pcm_uframes_t size)
{
    snd_pcm_channel_area_t areas[pcm->params.channels];
    snd_pcm_areas_from_buf(pcm, areas, buffer);
    return snd_pcm_read_areas(pcm, areas, 0, size,
                              snd_pcm_plugin_read_areas);
}

const snd_pcm_fast_ops_t snd_pcm_plugin_fast_ops =
{
    .state = snd_pcm_generic_state,
    .hwsync = snd_pcm_generic_hwsync,
    .start = snd_pcm_generic_start,
    .drop = snd_pcm_generic_drop,
    .writei = snd_pcm_plugin_writei,
    .readi = snd_pcm_plugin_readi,
    .avail_update = snd_pcm_plugin_avail_update,
};

static const snd_pcm_plugin_manager_t *pcm_plugins[] =
{
    /* The first plugin should be a tail slave pcm. */
    &pcm_hw_plugin,
    &pcm_softvol_plugin,
};

void snd_pcm_plugin_init(snd_pcm_plugin_t *plugin)
{
    snd_memset(plugin, 0, sizeof(snd_pcm_plugin_t));
    plugin->undo_read = snd_pcm_plugin_undo_read;
    plugin->undo_write = snd_pcm_plugin_undo_write;
}

snd_size_t snd_pcm_plugin_read(snd_pcm_t *pcm, void *buffer, int size)
{
    if (!(pcm && pcm->fast_ops && pcm->fast_ops->writei))
    {
        SND_ERR("ERR: %s - something wrong! pcm: %s\n",
                __func__, pcm->name);
        return SND_ERROR;
    }

    return pcm->fast_ops->readi(pcm, buffer, size);
}

snd_size_t snd_pcm_plugin_write(snd_pcm_t *pcm, const void *buffer, int size)
{
    if (!(pcm && pcm->fast_ops && pcm->fast_ops->writei))
    {
        SND_ERR("ERR: %s - something wrong! pcm: %s\n",
                __func__, pcm->name);
        return SND_ERROR;
    }

    return pcm->fast_ops->writei(pcm, buffer, size);
}

int snd_pcm_plugin_loads(snd_pcm_t *pcm, void *args)
{
    snd_pcm_type_t type = (snd_pcm_type_t)(args);

    if (type == SND_PCM_TYPE_SOFTVOL)
    {
        snd_pcm_t *pcm_m = pcm, *pcm_s = NULL, *pcmout = NULL;
        int i;

        for (i = 0; i < ARRAY_SIZE(pcm_plugins); i++)
        {
            pcm_plugins[i]->load(pcm_m, pcm_s, &pcmout);
            if (i == ARRAY_SIZE(pcm_plugins) - 2)
                pcm_m = pcm;
            else
                pcm_m = pcmout;

            pcm_s = pcmout;
        }
    }

    return 0;
}

int snd_pcm_plugin_unloads(snd_pcm_t *pcm, void *args)
{
    snd_pcm_type_t type = (snd_pcm_type_t)(args);

    if (type == SND_PCM_TYPE_SOFTVOL)
    {
        snd_pcm_t *pcm_t = pcm, *pcm_out;
        int i;

        for (i = ARRAY_SIZE(pcm_plugins) - 1; i >= 0; i--)
        {
            pcm_plugins[i]->unload(pcm_t, &pcm_out);
            pcm_t = pcm_out;
        }
    }

    return 0;
}

int snd_pcm_plugin_controls(snd_pcm_t *pcm, uint32 cmd, void *args)
{
    switch (cmd)
    {
    case RK_AUDIO_CTL_PLUGIN_PREPARE:
        snd_pcm_plugin_loads(pcm, args);
        break;
    case RK_AUDIO_CTL_PLUGIN_RELEASE:
        snd_pcm_plugin_unloads(pcm, args);
        break;
    case RK_AUDIO_CTL_PLUGIN_SET_SOFTVOL:
        snd_pcm_plugin_softvol_set_volume(pcm, args);
        break;
    case RK_AUDIO_CTL_PLUGIN_GET_SOFTVOL:
        snd_pcm_plugin_softvol_get_volume(pcm, args);
        break;
    default:
        break;
    }

    return 0;
}

#endif /* CONFIG_DRIVER_AUDIO_PCM_PLUGIN */
