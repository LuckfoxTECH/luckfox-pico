/*
 * SPDX-License-Identifier: (Apache-2.0 OR LGPL-2.1-or-later)
 *
 * Copyright (c) 2020 Fuzhou Rockchip Electronics Co., Ltd
 *
 * Change Logs:
 * Date           Author          Notes
 * 2020-03-03     Xing Zheng      The first version
 */

#ifndef __DRIVER_AUDIO_PCM_PLUGIN_PROVIDER_H__
#define __DRIVER_AUDIO_PCM_PLUGIN_PROVIDER_H__

/**
 * struct snd_softvol - audio L/R soft volume.
 */
typedef struct snd_softvol
{
    uint16_t vol_l;
    uint16_t vol_r;
} snd_softvol_t;

#endif /* __DRIVER_AUDIO_PCM_PLUGIN_PROVIDER_H__ */
