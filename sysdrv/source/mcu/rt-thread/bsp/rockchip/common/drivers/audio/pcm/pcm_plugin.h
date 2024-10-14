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

#ifndef __DRIVER_AUDIO_PCM_PLUGIN_H__
#define __DRIVER_AUDIO_PCM_PLUGIN_H__

#include "pcm_os_interface.h"
#include "pcm_generic.h"
#include "pcm_plugin_provider.h"

typedef snd_pcm_uframes_t (*snd_pcm_slave_xfer_areas_func_t)
(snd_pcm_t *pcm,
 const snd_pcm_channel_area_t *areas,
 snd_pcm_uframes_t offset,
 snd_pcm_uframes_t size,
 const snd_pcm_channel_area_t *slave_areas,
 snd_pcm_uframes_t slave_offset,
 snd_pcm_uframes_t *slave_sizep);

typedef snd_pcm_sframes_t (*snd_pcm_slave_xfer_areas_undo_func_t)
(snd_pcm_t *pcm,
 const snd_pcm_channel_area_t *res_areas,  /* result areas */
 snd_pcm_uframes_t res_offset,     /* offset of result areas */
 snd_pcm_uframes_t res_size,       /* size of result areas */
 snd_pcm_uframes_t slave_undo_size);

typedef struct
{
    int (*load)(snd_pcm_t *pcm, snd_pcm_t *slave, snd_pcm_t **pcmout);
    int (*unload)(snd_pcm_t *pcm, snd_pcm_t **pcmout);
} snd_pcm_plugin_manager_t;

typedef struct
{
    snd_pcm_generic_t gen;
    snd_pcm_slave_xfer_areas_func_t read;
    snd_pcm_slave_xfer_areas_func_t write;
    snd_pcm_slave_xfer_areas_undo_func_t undo_read;
    snd_pcm_slave_xfer_areas_undo_func_t undo_write;
    int (*init)(snd_pcm_t *pcm);
} snd_pcm_plugin_t;

void snd_pcm_plugin_init(snd_pcm_plugin_t *plugin);

typedef struct _snd_pcm_channel_info
{
    unsigned int channel;
    void *addr;         /* base address of channel samples */
    unsigned int first;     /* offset to first sample in bits */
    unsigned int step;      /* samples distance in bits */
} snd_pcm_channel_info_t;

snd_pcm_sframes_t snd_pcm_plugin_undo_read_generic
(snd_pcm_t *pcm,
 const snd_pcm_channel_area_t *res_areas,  /* result areas */
 snd_pcm_uframes_t res_offset,     /* offset of result areas */
 snd_pcm_uframes_t res_size,       /* size of result areas */
 snd_pcm_uframes_t slave_undo_size);

snd_pcm_sframes_t snd_pcm_plugin_undo_write_generic
(snd_pcm_t *pcm,
 const snd_pcm_channel_area_t *res_areas,  /* result areas */
 snd_pcm_uframes_t res_offset,     /* offset of result areas */
 snd_pcm_uframes_t res_size,       /* size of result areas */
 snd_pcm_uframes_t slave_undo_size);

int snd_pcm_plugin_loads(snd_pcm_t *pcm, void *args);
int snd_pcm_plugin_unloads(snd_pcm_t *pcm, void *args);

snd_err_t snd_pcm_plugin_softvol_set_volume(snd_pcm_t *pcm, snd_softvol_t *softvol);
snd_err_t snd_pcm_plugin_softvol_get_volume(snd_pcm_t *pcm, snd_softvol_t *softvol);

extern const snd_pcm_plugin_manager_t pcm_softvol_plugin;
extern const snd_pcm_plugin_manager_t pcm_hw_plugin;

#endif /* __DRIVER_AUDIO_PCM_PLUGIN_H__ */
