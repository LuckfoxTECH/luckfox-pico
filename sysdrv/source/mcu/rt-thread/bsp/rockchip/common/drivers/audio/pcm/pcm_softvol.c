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
 * \file pcm/pcm_softvol.c
 * \ingroup PCM_Plugins
 * \brief PCM Soft Volume Plugin Interface
 * \author Takashi Iwai <tiwai@suse.de>
 * \date 2004
 */
/*
 *  PCM - Soft Volume Plugin
 *  Copyright (c) 2004 by Takashi Iwai <tiwai@suse.de>
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

#ifdef RT_USING_DRIVER_AUDIO_PCM_PLUGIN_SOFTVOL

#include <math.h>

#include "pcm_os_interface.h"
#include "pcm_local.h"
#include "pcm_plugin.h"
#include "pcm_plugin_provider.h"

typedef struct
{
    /* This field need to be the first */
    snd_pcm_plugin_t plug;
    snd_pcm_format_t sformat;
    unsigned int cchannels;
    unsigned int cur_vol[2];
    unsigned int max_val;     /* max index */
    unsigned int zero_dB_val; /* index at 0 dB */
    double min_dB;
    double max_dB;
    unsigned int *dB_value;
} snd_pcm_softvol_t;

typedef struct
{
    double min_dB;
    double max_dB;
    int resolution;
    int channels;
    snd_pcm_format_t format;
} snd_pcm_plugin_softvol_config_t;

#define VOL_SCALE_SHIFT         16
#define VOL_SCALE_MASK          ((1 << VOL_SCALE_SHIFT) - 1)

#define PRESET_RESOLUTION       (256)
#define PRESET_MIN_DB           (-51.0)
#define ZERO_DB                 (0.0)
#define MAX_DB_UPPER_LIMIT      (50)
#define MIN_DB_DOWNER_LIMIT     (-100)

static const unsigned int preset_dB_value[PRESET_RESOLUTION] =
{
    0x00b8, 0x00bd, 0x00c1, 0x00c5, 0x00ca, 0x00cf, 0x00d4, 0x00d9,
    0x00de, 0x00e3, 0x00e8, 0x00ed, 0x00f3, 0x00f9, 0x00fe, 0x0104,
    0x010a, 0x0111, 0x0117, 0x011e, 0x0124, 0x012b, 0x0132, 0x0139,
    0x0140, 0x0148, 0x0150, 0x0157, 0x015f, 0x0168, 0x0170, 0x0179,
    0x0181, 0x018a, 0x0194, 0x019d, 0x01a7, 0x01b0, 0x01bb, 0x01c5,
    0x01cf, 0x01da, 0x01e5, 0x01f1, 0x01fc, 0x0208, 0x0214, 0x0221,
    0x022d, 0x023a, 0x0248, 0x0255, 0x0263, 0x0271, 0x0280, 0x028f,
    0x029e, 0x02ae, 0x02be, 0x02ce, 0x02df, 0x02f0, 0x0301, 0x0313,
    0x0326, 0x0339, 0x034c, 0x035f, 0x0374, 0x0388, 0x039d, 0x03b3,
    0x03c9, 0x03df, 0x03f7, 0x040e, 0x0426, 0x043f, 0x0458, 0x0472,
    0x048d, 0x04a8, 0x04c4, 0x04e0, 0x04fd, 0x051b, 0x053a, 0x0559,
    0x0579, 0x0599, 0x05bb, 0x05dd, 0x0600, 0x0624, 0x0648, 0x066e,
    0x0694, 0x06bb, 0x06e3, 0x070c, 0x0737, 0x0762, 0x078e, 0x07bb,
    0x07e9, 0x0818, 0x0848, 0x087a, 0x08ac, 0x08e0, 0x0915, 0x094b,
    0x0982, 0x09bb, 0x09f5, 0x0a30, 0x0a6d, 0x0aab, 0x0aeb, 0x0b2c,
    0x0b6f, 0x0bb3, 0x0bf9, 0x0c40, 0x0c89, 0x0cd4, 0x0d21, 0x0d6f,
    0x0dbf, 0x0e11, 0x0e65, 0x0ebb, 0x0f12, 0x0f6c, 0x0fc8, 0x1026,
    0x1087, 0x10e9, 0x114e, 0x11b5, 0x121f, 0x128b, 0x12fa, 0x136b,
    0x13df, 0x1455, 0x14ce, 0x154a, 0x15c9, 0x164b, 0x16d0, 0x1758,
    0x17e4, 0x1872, 0x1904, 0x1999, 0x1a32, 0x1ace, 0x1b6e, 0x1c11,
    0x1cb9, 0x1d64, 0x1e13, 0x1ec7, 0x1f7e, 0x203a, 0x20fa, 0x21bf,
    0x2288, 0x2356, 0x2429, 0x2500, 0x25dd, 0x26bf, 0x27a6, 0x2892,
    0x2984, 0x2a7c, 0x2b79, 0x2c7c, 0x2d85, 0x2e95, 0x2fab, 0x30c7,
    0x31ea, 0x3313, 0x3444, 0x357c, 0x36bb, 0x3801, 0x394f, 0x3aa5,
    0x3c02, 0x3d68, 0x3ed6, 0x404d, 0x41cd, 0x4355, 0x44e6, 0x4681,
    0x4826, 0x49d4, 0x4b8c, 0x4d4f, 0x4f1c, 0x50f3, 0x52d6, 0x54c4,
    0x56be, 0x58c3, 0x5ad4, 0x5cf2, 0x5f1c, 0x6153, 0x6398, 0x65e9,
    0x6849, 0x6ab7, 0x6d33, 0x6fbf, 0x7259, 0x7503, 0x77bd, 0x7a87,
    0x7d61, 0x804d, 0x834a, 0x8659, 0x897a, 0x8cae, 0x8ff5, 0x934f,
    0x96bd, 0x9a40, 0x9dd8, 0xa185, 0xa548, 0xa922, 0xad13, 0xb11b,
    0xb53b, 0xb973, 0xbdc5, 0xc231, 0xc6b7, 0xcb58, 0xd014, 0xd4ed,
    0xd9e3, 0xdef6, 0xe428, 0xe978, 0xeee8, 0xf479, 0xfa2b, 0xffff,
};

/* (32bit x 16bit) >> 16 */
typedef union
{
    int i;
    short s[2];
} val_t;

static inline int MULTI_DIV_32x16(int a, unsigned short b)
{
    val_t v, x, y;
    v.i = a;
    y.i = 0;
#ifdef __BIG_ENDIAN
    x.i = (unsigned int)v.s[1] * b;
    y.s[1] = x.s[0];
    y.i += (int)v.s[0] * b;
#else
    x.i = (unsigned short)v.s[0];
    x.i *= b;
    y.s[0] = x.s[1];
    y.i += (int)v.s[1] * b;
#endif
    return y.i;
}

static inline int MULTI_DIV_int(int a, unsigned int b, int swap)
{
    unsigned int gain = (b >> VOL_SCALE_SHIFT);
    int fraction;
    a = swap ? (int)bswap_32(a) : a;
    fraction = MULTI_DIV_32x16(a, b & VOL_SCALE_MASK);
    if (gain)
    {
        long long amp = (long long)a * gain + fraction;
        if (amp > (int)0x7fffffff)
            amp = (int)0x7fffffff;
        else if (amp < (int)0x80000000)
            amp = (int)0x80000000;
        return swap ? (int)bswap_32((int)amp) : (int)amp;
    }
    return swap ? (int)bswap_32(fraction) : fraction;
}

/* always little endian */
static inline int MULTI_DIV_24(int a, unsigned int b)
{
    unsigned int gain = b >> VOL_SCALE_SHIFT;
    int fraction;
    fraction = MULTI_DIV_32x16(a, b & VOL_SCALE_MASK);
    if (gain)
    {
        long long amp = (long long)a * gain + fraction;
        if (amp > (int)0x7fffff)
            amp = (int)0x7fffff;
        else if (amp < (int)0x800000)
            amp = (int)0x800000;
        return (int)amp;
    }
    return fraction;
}

static inline short MULTI_DIV_short(short a, unsigned int b, int swap)
{
    unsigned int gain = b >> VOL_SCALE_SHIFT;
    int fraction;
    a = swap ? (short)bswap_16(a) : a;
    fraction = (int)(a * (b & VOL_SCALE_MASK)) >> VOL_SCALE_SHIFT;
    if (gain)
    {
        int amp = a * gain + fraction;
        if (abs(amp) > 0x7fff)
            amp = (a < 0) ? (short)0x8000 : (short)0x7fff;
        return swap ? (short)bswap_16((short)amp) : (short)amp;
    }
    return swap ? (short)bswap_16((short)fraction) : (short)fraction;
}

/*
 * apply volumue attenuation
 */
#define CONVERT_AREA(TYPE, swap) do {   \
    unsigned int ch, fr; \
    TYPE *src, *dst; \
    for (ch = 0; ch < channels; ch++) { \
        src_area = &src_areas[ch]; \
        dst_area = &dst_areas[ch]; \
        src = snd_pcm_channel_area_addr(src_area, src_offset); \
        dst = snd_pcm_channel_area_addr(dst_area, dst_offset); \
        src_step = snd_pcm_channel_area_step(src_area) / sizeof(TYPE); \
        dst_step = snd_pcm_channel_area_step(dst_area) / sizeof(TYPE); \
        GET_VOL_SCALE; \
        fr = frames; \
        if (! vol_scale) { \
            while (fr--) { \
                *dst = 0; \
                dst += dst_step; \
            } \
        } else if (vol_scale == 0xffff) { \
            while (fr--) { \
                *dst = *src; \
                src += src_step; \
                dst += dst_step; \
            } \
        } else { \
            while (fr--) { \
                *dst = (TYPE) MULTI_DIV_##TYPE(*src, vol_scale, swap); \
                src += src_step; \
                dst += dst_step; \
            } \
        } \
    } \
} while (0)

#define CONVERT_AREA_S24_3LE() do {                 \
    unsigned int ch, fr;                        \
    unsigned char *src, *dst;                   \
    int tmp;                            \
    for (ch = 0; ch < channels; ch++) {             \
        src_area = &src_areas[ch];              \
        dst_area = &dst_areas[ch];              \
        src = snd_pcm_channel_area_addr(src_area, src_offset);  \
        dst = snd_pcm_channel_area_addr(dst_area, dst_offset);  \
        src_step = snd_pcm_channel_area_step(src_area);     \
        dst_step = snd_pcm_channel_area_step(dst_area);     \
        GET_VOL_SCALE;                      \
        fr = frames;                        \
        if (! vol_scale) {                  \
            while (fr--) {                  \
                dst[0] = dst[1] = dst[2] = 0;       \
                dst += dst_step;            \
            }                       \
        } else if (vol_scale == 0xffff) {           \
            while (fr--) {                  \
                dst[0] = src[0];            \
                dst[1] = src[1];            \
                dst[2] = src[2];            \
                src += dst_step;            \
                dst += src_step;            \
            }                       \
        } else {                        \
            while (fr--) {                  \
                tmp = src[0] |              \
                      (src[1] << 8) |           \
                      (((signed char *) src)[2] << 16); \
                tmp = MULTI_DIV_24(tmp, vol_scale); \
                dst[0] = tmp;               \
                dst[1] = tmp >> 8;          \
                dst[2] = tmp >> 16;         \
                src += dst_step;            \
                dst += src_step;            \
            }                       \
        }                           \
    }                               \
} while (0)

#define CONVERT_AREA_S24_LE() do {                  \
    unsigned int ch, fr;                        \
    int *src, *dst;                         \
    int tmp;                            \
    for (ch = 0; ch < channels; ch++) {             \
        src_area = &src_areas[ch];              \
        dst_area = &dst_areas[ch];              \
        src = snd_pcm_channel_area_addr(src_area, src_offset);  \
        dst = snd_pcm_channel_area_addr(dst_area, dst_offset);  \
        src_step = snd_pcm_channel_area_step(src_area)      \
                / sizeof(int);              \
        dst_step = snd_pcm_channel_area_step(dst_area)      \
                / sizeof(int);              \
        GET_VOL_SCALE;                      \
        fr = frames;                        \
        if (! vol_scale) {                  \
            while (fr--) {                  \
                *dst = 0;               \
                dst += dst_step;            \
            }                       \
        } else if (vol_scale == 0xffff) {           \
            while (fr--) {                  \
                *dst = *src;                \
                src += dst_step;            \
                dst += src_step;            \
            }                       \
        } else {                        \
            while (fr--) {                  \
                tmp = *src << 8;            \
                tmp = (signed int) tmp >> 8;        \
                *dst = MULTI_DIV_24(tmp, vol_scale);    \
                src += dst_step;            \
                dst += src_step;            \
            }                       \
        }                           \
    }                               \
} while (0)

#define GET_VOL_SCALE \
    switch (ch) { \
    case 0: \
    case 2: \
        vol_scale = (channels == ch + 1) ? vol_c : vol[0]; \
        break; \
    case 4: \
    case 5: \
        vol_scale = vol_c; \
        break; \
    default: \
        vol_scale = vol[ch & 1]; \
        break; \
    }

/* 2-channel stereo control */
static void softvol_convert_stereo_vol(snd_pcm_softvol_t *svol,
                                       const snd_pcm_channel_area_t *dst_areas,
                                       snd_pcm_uframes_t dst_offset,
                                       const snd_pcm_channel_area_t *src_areas,
                                       snd_pcm_uframes_t src_offset,
                                       unsigned int channels,
                                       snd_pcm_uframes_t frames)
{
    const snd_pcm_channel_area_t *dst_area, *src_area;
    unsigned int src_step, dst_step;
    unsigned int vol_scale, vol[2], vol_c;

    if (svol->cur_vol[0] == 0 && svol->cur_vol[1] == 0)
    {
        snd_pcm_areas_silence(dst_areas, dst_offset, channels, frames,
                              svol->sformat);
        return;
    }
    else if (svol->zero_dB_val && svol->cur_vol[0] == svol->zero_dB_val &&
             svol->cur_vol[1] == svol->zero_dB_val)
    {
        snd_pcm_areas_copy(dst_areas, dst_offset, src_areas, src_offset,
                           channels, frames, svol->sformat);
        return;
    }

    if (svol->max_val == 1)
    {
        vol[0] = svol->cur_vol[0] ? 0xffff : 0;
        vol[1] = svol->cur_vol[1] ? 0xffff : 0;
        vol_c = vol[0] | vol[1];
    }
    else
    {
        vol[0] = svol->dB_value[svol->cur_vol[0]];
        vol[1] = svol->dB_value[svol->cur_vol[1]];
        vol_c = svol->dB_value[(svol->cur_vol[0] + svol->cur_vol[1]) / 2];
    }
    switch (svol->sformat)
    {
    case SND_PCM_FORMAT_S16_LE:
        /* 16bit samples */
        CONVERT_AREA(short,
                     !snd_pcm_format_cpu_endian(svol->sformat));
        break;
    case SND_PCM_FORMAT_S32_LE:
        /* 32bit samples */
        CONVERT_AREA(int,
                     !snd_pcm_format_cpu_endian(svol->sformat));
        break;
    case SND_PCM_FORMAT_S24_LE:
        /* 24bit samples */
        CONVERT_AREA_S24_LE();
        break;
    case SND_PCM_FORMAT_S24_3LE:
        CONVERT_AREA_S24_3LE();
        break;
    default:
        break;
    }
}

/* after stereo control and before mono control */
#undef GET_VOL_SCALE
#define GET_VOL_SCALE

/* mono control */
static void softvol_convert_mono_vol(snd_pcm_softvol_t *svol,
                                     const snd_pcm_channel_area_t *dst_areas,
                                     snd_pcm_uframes_t dst_offset,
                                     const snd_pcm_channel_area_t *src_areas,
                                     snd_pcm_uframes_t src_offset,
                                     unsigned int channels,
                                     snd_pcm_uframes_t frames)
{
    const snd_pcm_channel_area_t *dst_area, *src_area;
    unsigned int src_step, dst_step;
    unsigned int vol_scale;

    if (svol->cur_vol[0] == 0)
    {
        snd_pcm_areas_silence(dst_areas, dst_offset, channels, frames,
                              svol->sformat);
        return;
    }
    else if (svol->zero_dB_val && svol->cur_vol[0] == svol->zero_dB_val)
    {
        snd_pcm_areas_copy(dst_areas, dst_offset, src_areas, src_offset,
                           channels, frames, svol->sformat);
        return;
    }

    if (svol->max_val == 1)
        vol_scale = svol->cur_vol[0] ? 0xffff : 0;
    else
        vol_scale = svol->dB_value[svol->cur_vol[0]];
    switch (svol->sformat)
    {
    case SND_PCM_FORMAT_S16_LE:
        /* 16bit samples */
        CONVERT_AREA(short,
                     !snd_pcm_format_cpu_endian(svol->sformat));
        break;
    case SND_PCM_FORMAT_S32_LE:
        /* 32bit samples */
        CONVERT_AREA(int,
                     !snd_pcm_format_cpu_endian(svol->sformat));
        break;
    case SND_PCM_FORMAT_S24_LE:
        /* 24bit samples */
        CONVERT_AREA_S24_LE();
        break;
    case SND_PCM_FORMAT_S24_3LE:
        CONVERT_AREA_S24_3LE();
        break;
    default:
        break;
    }
}

static void dump_current_volume(snd_pcm_softvol_t *svol)
{
    SND_VERBOSE("current vol: %d / %d\n",
                svol->cur_vol[0], svol->cur_vol[1]);
    return;
}

static void softvol_free(snd_pcm_softvol_t *svol)
{
    if (svol->dB_value && svol->dB_value != preset_dB_value)
        free(svol->dB_value);
    free(svol);
}

static int snd_pcm_softvol_close(snd_pcm_t *pcm)
{
    snd_pcm_softvol_t *svol = pcm->private_data;
    softvol_free(svol);
    return 0;
}

static int snd_pcm_softvol_hw_params(snd_pcm_t *pcm, snd_pcm_stream_t stream,
                                     snd_pcm_hw_params_t *params)
{
    snd_pcm_softvol_t *svol = pcm->private_data;
    snd_pcm_t *slave = svol->plug.gen.slave;

    pcm->format = to_snd_pcm_format(params->sampleBits);
    slave->plugin_ops->hw_params(slave, stream, params);

    return 0;
}

static snd_pcm_uframes_t
snd_pcm_softvol_write_areas(snd_pcm_t *pcm,
                            const snd_pcm_channel_area_t *areas,
                            snd_pcm_uframes_t offset,
                            snd_pcm_uframes_t size,
                            const snd_pcm_channel_area_t *slave_areas,
                            snd_pcm_uframes_t slave_offset,
                            snd_pcm_uframes_t *slave_sizep)
{
    snd_pcm_softvol_t *svol = pcm->private_data;
    if (size > *slave_sizep)
        size = *slave_sizep;

    dump_current_volume(svol);
    if (svol->cchannels == 1)
        softvol_convert_mono_vol(svol, slave_areas, slave_offset,
                                 areas, offset, pcm->params.channels, size);
    else
        softvol_convert_stereo_vol(svol, slave_areas, slave_offset,
                                   areas, offset, pcm->params.channels, size);
    *slave_sizep = size;
    return size;
}

static snd_pcm_uframes_t
snd_pcm_softvol_read_areas(snd_pcm_t *pcm,
                           const snd_pcm_channel_area_t *areas,
                           snd_pcm_uframes_t offset,
                           snd_pcm_uframes_t size,
                           const snd_pcm_channel_area_t *slave_areas,
                           snd_pcm_uframes_t slave_offset,
                           snd_pcm_uframes_t *slave_sizep)
{
    snd_pcm_softvol_t *svol = pcm->private_data;
    if (size > *slave_sizep)
        size = *slave_sizep;
    dump_current_volume(svol);
    if (svol->cchannels == 1)
        softvol_convert_mono_vol(svol, areas, offset, slave_areas,
                                 slave_offset, pcm->params.channels, size);
    else
        softvol_convert_stereo_vol(svol, areas, offset, slave_areas,
                                   slave_offset, pcm->params.channels, size);
    *slave_sizep = size;
    return size;
}

static void snd_pcm_softvol_dump(snd_pcm_t *pcm)
{
    snd_pcm_softvol_t *svol = pcm->private_data;

    SND_INFO("==== Soft Volume PCM Plugin ====\n");
    SND_INFO("pcm name: %s\n", pcm->name);
    if (svol->max_val == 1)
    {
        SND_INFO("boolean\n");
    }
    else
    {
        char fbuf[16] = { 0 };

        sprintf(fbuf, "%.6f", svol->min_dB);
        SND_INFO("min_dB: %s\n", fbuf);
        sprintf(fbuf, "%.6f", svol->max_dB);
        SND_INFO("max_dB: %s\n", fbuf);
        SND_INFO("resolution: %d\n", svol->max_val + 1);
        SND_INFO("cur_vol: %d/%d\n", svol->cur_vol[0], svol->cur_vol[1]);
    }
}

/*
 * load and set up user-control
 * returns 0 if the user-control is found or created,
 * returns 1 if the control is a hw control,
 * or a negative error code
 */
static int softvol_load_control(snd_pcm_softvol_t *svol,
                                int cchannels, double min_dB, double max_dB,
                                int resolution)
{
    unsigned int i;

    svol->max_val = resolution - 1;
    svol->min_dB = min_dB;
    svol->max_dB = max_dB;
    /* Half resolution volumes by default. */
    svol->cur_vol[0] = PRESET_RESOLUTION >> 1;
    svol->cur_vol[1] = PRESET_RESOLUTION >> 1;
    if (svol->max_val == 1 || svol->max_dB == ZERO_DB)
        svol->zero_dB_val = svol->max_val;
    else if (svol->max_dB < 0)
        svol->zero_dB_val = 0; /* There is no 0 dB setting */
    else
        svol->zero_dB_val = (min_dB / (min_dB - max_dB)) *
                            svol->max_val;
    if (svol->max_val == 1)
        return 0;

    /* Setup dB table */
    if (min_dB == PRESET_MIN_DB &&
            max_dB == ZERO_DB &&
            resolution == PRESET_RESOLUTION)
    {
        svol->dB_value = (unsigned int *)preset_dB_value;
    }
    else
    {
        /* Have soft float */
        svol->dB_value = snd_zmalloc(resolution * sizeof(unsigned int));
        if (! svol->dB_value)
        {
            SND_ERR("cannot allocate dB table");
            return -ENOMEM;
        }
        svol->min_dB = min_dB;
        svol->max_dB = max_dB;
        for (i = 0; i <= svol->max_val; i++)
        {
            double db = svol->min_dB +
                        (i * (svol->max_dB - svol->min_dB)) /
                        svol->max_val;
            double v = (pow(10.0, db / 20.0) *
                        (double)(1 << VOL_SCALE_SHIFT));

            svol->dB_value[i] = (unsigned int)v;
        }
        if (svol->zero_dB_val)
            svol->dB_value[svol->zero_dB_val] = 65535;
    }
    return 0;
}

static const snd_pcm_plugin_ops_t snd_pcm_softvol_plugin_ops =
{
    .hw_params = snd_pcm_softvol_hw_params,
};

static int snd_pcm_softvol_open(snd_pcm_t *pcm,
                                snd_pcm_t **pcmout,
                                snd_pcm_format_t sformat,
                                int cchannels,
                                double min_dB, double max_dB, int resolution,
                                snd_pcm_t *slave)
{
    snd_pcm_softvol_t *svol;
    int err;

    if (sformat != SND_PCM_FORMAT_UNKNOWN &&
            sformat != SND_PCM_FORMAT_S16_LE &&
            sformat != SND_PCM_FORMAT_S24_LE &&
            sformat != SND_PCM_FORMAT_S32_LE)
        return -EINVAL;
    svol = snd_zmalloc(sizeof(*svol));
    if (! svol)
        return -ENOMEM;

    err = softvol_load_control(svol, cchannels,
                               min_dB, max_dB, resolution);
    if (err < 0)
    {
        softvol_free(svol);
        return err;
    }

    /* do softvol */
    snd_pcm_plugin_init(&svol->plug);
    svol->sformat = sformat;
    svol->cchannels = cchannels;
    svol->plug.read = snd_pcm_softvol_read_areas;
    svol->plug.write = snd_pcm_softvol_write_areas;
    svol->plug.undo_read = snd_pcm_plugin_undo_read_generic;
    svol->plug.undo_write = snd_pcm_plugin_undo_write_generic;
    svol->plug.gen.slave = slave;

    slave->fast_op_arg = pcm;

    pcm->type = SND_PCM_TYPE_SOFTVOL;
    pcm->plugin_ops = &snd_pcm_softvol_plugin_ops;
    pcm->fast_ops = &snd_pcm_plugin_fast_ops;
    pcm->private_data = svol;

    snd_memset(pcm->name, 0, sizeof(pcm->name));
    strcpy(pcm->name, "softvol");

    /* output pcm self. */
    *pcmout = pcm;

    return 0;
}

static bool snd_pcm_softvol_check_db(char *str)
{
    int i;

    if (!str || !strlen(str))
        return false;

    for (i = 0; i < strlen(str); i++)
    {
        if ((i == 0) && str[i] == '-')
            continue;
        if ((i != (strlen(str) - 1)) && str[i] == '.')
            continue;
        if (!isdigit(str[i]))
            return false;
    }

    return true;
}

static bool snd_pcm_softvol_strtoformat(const char *str, snd_pcm_format_t *format)
{
    struct str_format
    {
        const char name[16];
        snd_pcm_format_t format;
    };
    struct str_format supports[SND_PCM_FORMAT_LAST + 1] =
    {
        {
            .name = "S8",
            .format = SND_PCM_FORMAT_S8,
        },
        {
            .name = "S16_LE",
            .format = SND_PCM_FORMAT_S16_LE,
        },
        {
            .name = "S24_LE",
            .format = SND_PCM_FORMAT_S24_LE,
        },
        {
            .name = "S32_LE",
            .format = SND_PCM_FORMAT_S32_LE,
        },
        {
            .name = "S24_3LE",
            .format = SND_PCM_FORMAT_S24_3LE,
        },
        {
            .name = "S20_3LE",
            .format = SND_PCM_FORMAT_S20_3LE,
        },
        {
            .name = "S18_3LE",
            .format = SND_PCM_FORMAT_S18_3LE,
        },
    };
    int i;

    if (!str || !strlen(str))
        return false;

    for (i = 0; i < SND_PCM_FORMAT_LAST + 1; i++)
    {
        if (!strncmp(str, supports[i].name, SND_NAME_MAX))
        {
            *format = supports[i].format;
            return true;
        }
    }

    *format = SND_PCM_FORMAT_UNKNOWN;
    return false;
}

static int snd_pcm_softvol_strtodb(const char *str, double *dB)
{
    char *str_end = NULL;

    if (!str)
        return -EINVAL;

    *dB = strtod((const char *)str, (char **)&str_end);
    return 0;
}

static int snd_pcm_softvol_config(snd_pcm_plugin_softvol_config_t *config)
{
    double dB;
    snd_pcm_format_t format;
    int resolution = CONFIG_SOFTVOL_RESOLUTION;
    int channels = CONFIG_SOFTVOL_CHANNELS;
    int ret = 0;

    if (resolution <= 1 || resolution > 1024)
    {
        SND_ERR("ERR: Invalid resolution value %d\n", resolution);
        ret = -EINVAL;
        goto out;
    }
    config->resolution = resolution;

    if (channels <= 0)
    {
        SND_ERR("ERR: Invalid channels value %d\n", channels);
        ret = -EINVAL;
        goto out;
    }
    config->channels = channels;

    if (!snd_pcm_softvol_check_db(CONFIG_SOFTVOL_MIN_DB))
    {
        SND_ERR("ERR: CONFIG_SOFTVOL_MIN_DB: %s\n",
                CONFIG_SOFTVOL_MIN_DB);
        ret = -EINVAL;
        goto out;
    }
    if (!snd_pcm_softvol_check_db(CONFIG_SOFTVOL_MAX_DB))
    {
        SND_ERR("ERR: CONFIG_SOFTVOL_MAX_DB: %s\n",
                CONFIG_SOFTVOL_MAX_DB);
        ret = -EINVAL;
        goto out;
    }

    if (!snd_pcm_softvol_strtoformat(CONFIG_SOFTVOL_FORMAT, &format))
    {
        SND_ERR("ERR: CONFIG_SOFTVOL_FORMAT: %s\n",
                CONFIG_SOFTVOL_FORMAT);
        ret = -EINVAL;
        goto out;
    }
    config->format = format;

    ret = snd_pcm_softvol_strtodb(CONFIG_SOFTVOL_MIN_DB, &dB);
    if (ret < 0)
    {
        SND_ERR("ERR: invalid min_dB: %d\n", ret);
        goto out;
    }
    if (dB >= 0)
    {
        SND_ERR("ERR: min_dB(%lf) must be a negative value\n", dB);
        ret = -EINVAL;
        goto out;
    }
    if (dB < MIN_DB_DOWNER_LIMIT)
        dB = MIN_DB_DOWNER_LIMIT;
    config->min_dB = dB;

    ret = snd_pcm_softvol_strtodb(CONFIG_SOFTVOL_MAX_DB, &dB);
    if (ret < 0)
    {
        SND_ERR("ERR: invalid max_dB: %d\n", ret);
        goto out;
    }
    config->max_dB = dB;

    if (config->max_dB <= config->min_dB ||
            config->max_dB > MAX_DB_UPPER_LIMIT)
    {
        SND_ERR("max_dB must be larger than min_dB and less than %d dB\n",
                MAX_DB_UPPER_LIMIT);
        return -EINVAL;
    }

out:
    return ret;
}

static int snd_pcm_plugin_softvol_load(snd_pcm_t *pcm, snd_pcm_t *slave,
                                       snd_pcm_t **pcmout)
{
    snd_pcm_plugin_softvol_config_t config;
    int err;

    if (!pcm)
    {
        err = -EINVAL;
        SND_ERR("ERR: NULL pcm\n");
        goto out;
    }

    err = snd_pcm_softvol_config(&config);
    if (err < 0)
    {
        SND_ERR("ERR: softvol prepare failed: %d\n", err);
        goto out;
    }

    err = snd_pcm_softvol_open(pcm, pcmout,
                               config.format,
                               config.channels,
                               config.min_dB,
                               config.max_dB,
                               config.resolution,
                               slave);
    if (err < 0)
    {
        SND_ERR("ERR: softvol open failed: %d\n", err);
        goto out;
    }

    snd_pcm_softvol_dump(pcm);

out:
    return err;
}

static int snd_pcm_plugin_softvol_unload(snd_pcm_t *pcm, snd_pcm_t **pcmout)
{
    snd_pcm_softvol_t *svol = pcm->private_data;

    *pcmout = svol->plug.gen.slave;
    snd_pcm_softvol_close(pcm);

    return SND_SUCCESS;
}

const snd_pcm_plugin_manager_t pcm_softvol_plugin =
{
    .load = snd_pcm_plugin_softvol_load,
    .unload = snd_pcm_plugin_softvol_unload,
};

snd_err_t snd_pcm_plugin_softvol_set_volume(snd_pcm_t *pcm, snd_softvol_t *softvol)
{
    snd_pcm_softvol_t *svol = pcm->private_data;

    softvol->vol_l = min(softvol->vol_l, svol->max_val);
    softvol->vol_r = min(softvol->vol_r, svol->max_val);
    svol->cur_vol[0] = softvol->vol_l;
    svol->cur_vol[1] = softvol->vol_r;

    snd_pcm_softvol_dump(pcm);
    return 0;
}

snd_err_t snd_pcm_plugin_softvol_get_volume(snd_pcm_t *pcm, snd_softvol_t *softvol)
{
    snd_pcm_softvol_t *svol = pcm->private_data;

    softvol->vol_l = svol->cur_vol[0];
    softvol->vol_r = svol->cur_vol[1];
    return 0;
}

#endif /* CONFIG_DRIVER_AUDIO_PCM_PLUGIN_SOFTVOL */
