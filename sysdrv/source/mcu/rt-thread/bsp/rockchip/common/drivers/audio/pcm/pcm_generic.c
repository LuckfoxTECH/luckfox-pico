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
 * \file pcm/pcm_generic.c
 * \ingroup PCM
 * \brief PCM Interface
 * \author Jaroslav Kysela <perex@perex.cz>
 * \date 2004
 */
/*
 *  PCM - Common generic plugin code
 *  Copyright (c) 2004 by Jaroslav Kysela <perex@perex.cz>
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

#include "pcm.h"
#include "pcm_generic.h"

int snd_pcm_generic_hwsync(snd_pcm_t *pcm)
{
    snd_pcm_generic_t *generic = pcm->private_data;
    return snd_pcm_hwsync(generic->slave);
}

int snd_pcm_generic_start(snd_pcm_t *pcm)
{
    snd_pcm_generic_t *generic = pcm->private_data;
    return snd_pcm_start(generic->slave);
}

int snd_pcm_generic_drop(snd_pcm_t *pcm)
{
    snd_pcm_generic_t *generic = pcm->private_data;
    return snd_pcm_drop(generic->slave);
}

snd_pcm_state_t snd_pcm_generic_state(snd_pcm_t *pcm)
{
    snd_pcm_generic_t *generic = pcm->private_data;
    return snd_pcm_state(generic->slave);
}

snd_pcm_sframes_t
snd_pcm_plugin_undo_read_generic(snd_pcm_t *pcm ATTRIBUTE_UNUSED,
                                 const snd_pcm_channel_area_t *res_areas ATTRIBUTE_UNUSED,
                                 snd_pcm_uframes_t res_offset ATTRIBUTE_UNUSED,
                                 snd_pcm_uframes_t res_size ATTRIBUTE_UNUSED,
                                 snd_pcm_uframes_t slave_undo_size)
{
    return slave_undo_size;
}

snd_pcm_sframes_t
snd_pcm_plugin_undo_write_generic(snd_pcm_t *pcm ATTRIBUTE_UNUSED,
                                  const snd_pcm_channel_area_t *res_areas ATTRIBUTE_UNUSED,
                                  snd_pcm_uframes_t res_offset ATTRIBUTE_UNUSED,
                                  snd_pcm_uframes_t res_size ATTRIBUTE_UNUSED,
                                  snd_pcm_uframes_t slave_undo_size)
{
    return slave_undo_size;
}

#endif /* CONFIG_DRIVER_AUDIO_PCM_GENERIC */