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

#ifndef __DRIVER_AUDIO_PCM_GENERIC_H__
#define __DRIVER_AUDIO_PCM_GENERIC_H__

typedef struct
{
    snd_pcm_t *slave;
    int close_slave;
} snd_pcm_generic_t;

int snd_pcm_generic_hw_params(snd_pcm_t *pcm, snd_pcm_hw_params_t *params);
snd_pcm_state_t snd_pcm_generic_state(snd_pcm_t *pcm);
int snd_pcm_generic_hwsync(snd_pcm_t *pcm);
int snd_pcm_generic_start(snd_pcm_t *pcm);
int snd_pcm_generic_drop(snd_pcm_t *pcm);

#endif /* __DRIVER_AUDIO_PCM_GENERIC_H__ */
