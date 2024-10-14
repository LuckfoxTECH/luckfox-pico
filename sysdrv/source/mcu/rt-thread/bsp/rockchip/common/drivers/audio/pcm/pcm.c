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
 * \file pcm/pcm.c
 * \ingroup PCM
 * \brief PCM Interface
 * \author Jaroslav Kysela <perex@perex.cz>
 * \author Abramo Bagnara <abramo@alsa-project.org>
 * \date 2000-2001
 *
 * PCM Interface is designed to write or read digital audio frames. A
 * frame is the data unit converted into/from sound in one time unit
 * (1/rate seconds), by example if you set your playback PCM rate to
 * 44100 you'll hear 44100 frames per second. The size in bytes of a
 * frame may be obtained from bits needed to store a sample and
 * channels count.
 *
 * See the \ref pcm page for more details.
 */
/*
 *  PCM Interface - main file
 *  Copyright (c) 1998 by Jaroslav Kysela <perex@perex.cz>
 *  Copyright (c) 2000 by Abramo Bagnara <abramo@alsa-project.org>
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
#include "pcm.h"

/**
 * \brief Return PCM state
 * \param pcm PCM handle
 * \return PCM state #snd_pcm_state_t of given PCM handle
 *
 * This is a faster way to obtain only the PCM state without calling
 * \link ::snd_pcm_status() \endlink.
 *
 * The function is thread-safe when built with the proper option.
 */
snd_pcm_state_t snd_pcm_state(snd_pcm_t *pcm)
{
    snd_pcm_state_t state;

    snd_assert(pcm);
    snd_pcm_lock(pcm);
    state = __snd_pcm_state(pcm);
    snd_pcm_unlock(pcm);
    return state;
}

/**
 * \brief (DEPRECATED) Synchronize stream position with hardware
 * \param pcm PCM handle
 * \return 0 on success otherwise a negative error code
 *
 * Note this function does not update the actual r/w pointer
 * for applications. The function #snd_pcm_avail_update()
 * have to be called before any mmap begin+commit operation.
 *
 * The function is thread-safe when built with the proper option.
 */
int snd_pcm_hwsync(snd_pcm_t *pcm)
{
    int err;

    snd_assert(pcm);
    snd_pcm_lock(pcm);
    err = __snd_pcm_hwsync(pcm);
    snd_pcm_unlock(pcm);
    return err;
}

int snd_pcm_drop(snd_pcm_t *pcm)
{
    int err;

    snd_assert(pcm);
    snd_pcm_lock(pcm);
    err = pcm->fast_ops->drop(pcm);
    snd_pcm_unlock(pcm);
    return err;
}

/**
 * \brief Silence an area
 * \param dst_area area specification
 * \param dst_offset offset in frames inside area
 * \param samples samples to silence
 * \param format PCM sample format
 * \return 0 on success otherwise a negative error code
 */
int snd_pcm_area_silence(const snd_pcm_channel_area_t *dst_area, snd_pcm_uframes_t dst_offset,
                         unsigned int samples, snd_pcm_format_t format)
{
    /* FIXME: sub byte resolution and odd dst_offset */
    char *dst;
    unsigned int dst_step;
    int width;
    uint64_t silence;
    if (!dst_area->addr)
        return 0;
    dst = snd_pcm_channel_area_addr(dst_area, dst_offset);
    width = snd_pcm_format_physical_width(format);
    silence = snd_pcm_format_silence_64(format);
    if (dst_area->step == (unsigned int) width)
    {
        unsigned int dwords = samples * width / 64;
        uint64_t *dstp = (uint64_t *)dst;
        samples -= dwords * 64 / width;
        while (dwords-- > 0)
            *dstp++ = silence;
        if (samples == 0)
            return 0;
    }
    dst_step = dst_area->step / 8;
    switch (width)
    {
    case 4:
    {
        uint8_t s0 = silence & 0xf0;
        uint8_t s1 = silence & 0x0f;
        int dstbit = dst_area->first % 8;
        int dstbit_step = dst_area->step % 8;
        while (samples-- > 0)
        {
            if (dstbit)
            {
                *dst &= 0xf0;
                *dst |= s1;
            }
            else
            {
                *dst &= 0x0f;
                *dst |= s0;
            }
            dst += dst_step;
            dstbit += dstbit_step;
            if (dstbit == 8)
            {
                dst++;
                dstbit = 0;
            }
        }
        break;
    }
    case 8:
    {
        uint8_t sil = silence;
        while (samples-- > 0)
        {
            *dst = sil;
            dst += dst_step;
        }
        break;
    }
    case 16:
    {
        uint16_t sil = silence;
        while (samples-- > 0)
        {
            *(uint16_t *)dst = sil;
            dst += dst_step;
        }
        break;
    }
    case 24:
#ifdef SND_LITTLE_ENDIAN
        *(dst + 0) = silence >> 0;
        *(dst + 1) = silence >> 8;
        *(dst + 2) = silence >> 16;
#else
        *(dst + 2) = silence >> 0;
        *(dst + 1) = silence >> 8;
        *(dst + 0) = silence >> 16;
#endif
        break;
    case 32:
    {
        uint32_t sil = silence;
        while (samples-- > 0)
        {
            *(uint32_t *)dst = sil;
            dst += dst_step;
        }
        break;
    }
    case 64:
    {
        while (samples-- > 0)
        {
            *(uint64_t *)dst = silence;
            dst += dst_step;
        }
        break;
    }
    default:
        SND_ERR("invalid format width %d", width);
        return -EINVAL;
    }
    return 0;
}

/**
 * \brief Silence one or more areas
 * \param dst_areas areas specification (one for each channel)
 * \param dst_offset offset in frames inside area
 * \param channels channels count
 * \param frames frames to silence
 * \param format PCM sample format
 * \return 0 on success otherwise a negative error code
 */
int snd_pcm_areas_silence(const snd_pcm_channel_area_t *dst_areas, snd_pcm_uframes_t dst_offset,
                          unsigned int channels, snd_pcm_uframes_t frames, snd_pcm_format_t format)
{
    int width = snd_pcm_format_physical_width(format);
    while (channels > 0)
    {
        void *addr = dst_areas->addr;
        unsigned int step = dst_areas->step;
        const snd_pcm_channel_area_t *begin = dst_areas;
        int channels1 = channels;
        unsigned int chns = 0;
        int err;
        while (1)
        {
            channels1--;
            chns++;
            dst_areas++;
            if (channels1 == 0 ||
                    dst_areas->addr != addr ||
                    dst_areas->step != step ||
                    dst_areas->first != dst_areas[-1].first + width)
                break;
        }
        if (chns > 1 && chns * width == step)
        {
            /* Collapse the areas */
            snd_pcm_channel_area_t d;
            d.addr = begin->addr;
            d.first = begin->first;
            d.step = width;
            err = snd_pcm_area_silence(&d, dst_offset * chns, frames * chns, format);
            channels -= chns;
        }
        else
        {
            err = snd_pcm_area_silence(begin, dst_offset, frames, format);
            dst_areas = begin + 1;
            channels--;
        }
        if (err < 0)
            return err;
    }
    return 0;
}

int snd_pcm_new(snd_pcm_t **pcmp)
{
    snd_pcm_t *pcm;

    pcm = snd_zmalloc(sizeof(*pcm));
    if (!pcm)
        return -ENOMEM;

    *pcmp = pcm;

    return 0;
}

int snd_pcm_free(snd_pcm_t *pcm)
{
    snd_assert(pcm);

    snd_free(pcm);
    return 0;
}

/**
 * \brief close PCM handle
 * \param pcm PCM handle
 * \return 0 on success otherwise a negative error code
 *
 * Closes the specified PCM handle and frees all associated
 * resources.
 */
int snd_pcm_close(snd_pcm_t *pcm)
{
    int res = 0, err;

    snd_assert(pcm);
    err = snd_pcm_free(pcm);
    if (err < 0)
        res = err;
    return res;
}

/**
 * \brief Start a PCM
 * \param pcm PCM handle
 * \return 0 on success otherwise a negative error code
 *
 * The function is thread-safe when built with the proper option.
 */
int snd_pcm_start(snd_pcm_t *pcm)
{
    int err;

    snd_assert(pcm);
    snd_pcm_lock(pcm);
    err = __snd_pcm_start(pcm);
    snd_pcm_unlock(pcm);
    return err;
}

/**
 * \brief Copy an area
 * \param dst_area destination area specification
 * \param dst_offset offset in frames inside destination area
 * \param src_area source area specification
 * \param src_offset offset in frames inside source area
 * \param samples samples to copy
 * \param format PCM sample format
 * \return 0 on success otherwise a negative error code
 */
int snd_pcm_area_copy(const snd_pcm_channel_area_t *dst_area, snd_pcm_uframes_t dst_offset,
                      const snd_pcm_channel_area_t *src_area, snd_pcm_uframes_t src_offset,
                      unsigned int samples, snd_pcm_format_t format)
{
    /* FIXME: sub byte resolution and odd dst_offset */
    const char *src;
    char *dst;
    int width;
    int src_step, dst_step;
    if (dst_area == src_area && dst_offset == src_offset)
        return 0;
    if (!src_area->addr)
        return snd_pcm_area_silence(dst_area, dst_offset, samples, format);
    src = snd_pcm_channel_area_addr(src_area, src_offset);
    if (!dst_area->addr)
        return 0;
    dst = snd_pcm_channel_area_addr(dst_area, dst_offset);
    width = snd_pcm_format_physical_width(format);
    if (src_area->step == (unsigned int) width &&
            dst_area->step == (unsigned int) width)
    {
        size_t bytes = samples * width / 8;
        samples -= bytes * 8 / width;
        snd_assert(src < dst || src >= dst + bytes);
        snd_assert(dst < src || dst >= src + bytes);
        memcpy(dst, src, bytes);
        if (samples == 0)
            return 0;
    }
    src_step = src_area->step / 8;
    dst_step = dst_area->step / 8;
    switch (width)
    {
    case 4:
    {
        int srcbit = src_area->first % 8;
        int srcbit_step = src_area->step % 8;
        int dstbit = dst_area->first % 8;
        int dstbit_step = dst_area->step % 8;
        while (samples-- > 0)
        {
            unsigned char srcval;
            if (srcbit)
                srcval = *src & 0x0f;
            else
                srcval = *src & 0xf0;
            if (dstbit)
                *dst &= 0xf0;
            else
                *dst &= 0x0f;
            *dst |= srcval;
            src += src_step;
            srcbit += srcbit_step;
            if (srcbit == 8)
            {
                src++;
                srcbit = 0;
            }
            dst += dst_step;
            dstbit += dstbit_step;
            if (dstbit == 8)
            {
                dst++;
                dstbit = 0;
            }
        }
        break;
    }
    case 8:
    {
        while (samples-- > 0)
        {
            *dst = *src;
            src += src_step;
            dst += dst_step;
        }
        break;
    }
    case 16:
    {
        while (samples-- > 0)
        {
            *(uint16_t *)dst = *(const uint16_t *)src;
            src += src_step;
            dst += dst_step;
        }
        break;
    }
    case 24:
        while (samples-- > 0)
        {
            *(dst + 0) = *(src + 0);
            *(dst + 1) = *(src + 1);
            *(dst + 2) = *(src + 2);
            src += src_step;
            dst += dst_step;
        }
        break;
    case 32:
    {
        while (samples-- > 0)
        {
            *(uint32_t *)dst = *(const uint32_t *)src;
            src += src_step;
            dst += dst_step;
        }
        break;
    }
    case 64:
    {
        while (samples-- > 0)
        {
            *(uint64_t *)dst = *(const uint64_t *)src;
            src += src_step;
            dst += dst_step;
        }
        break;
    }
    default:
        SND_ERR("invalid format width %d", width);
        return -EINVAL;
    }
    return 0;
}

/**
 * \brief Copy one or more areas
 * \param dst_areas destination areas specification (one for each channel)
 * \param dst_offset offset in frames inside destination area
 * \param src_areas source areas specification (one for each channel)
 * \param src_offset offset in frames inside source area
 * \param channels channels count
 * \param frames frames to copy
 * \param format PCM sample format
 * \return 0 on success otherwise a negative error code
 */
int snd_pcm_areas_copy(const snd_pcm_channel_area_t *dst_areas, snd_pcm_uframes_t dst_offset,
                       const snd_pcm_channel_area_t *src_areas, snd_pcm_uframes_t src_offset,
                       unsigned int channels, snd_pcm_uframes_t frames, snd_pcm_format_t format)
{
    int width = snd_pcm_format_physical_width(format);
    snd_assert(dst_areas);
    snd_assert(src_areas);
    if (! channels)
    {
        SND_ERR("invalid channels %d", channels);
        return -EINVAL;
    }
    if (! frames)
    {
        SND_ERR("invalid frames %ld", frames);
        return -EINVAL;
    }
    while (channels > 0)
    {
        unsigned int step = src_areas->step;
        void *src_addr = src_areas->addr;
        const snd_pcm_channel_area_t *src_start = src_areas;
        void *dst_addr = dst_areas->addr;
        const snd_pcm_channel_area_t *dst_start = dst_areas;
        int channels1 = channels;
        unsigned int chns = 0;
        while (dst_areas->step == step)
        {
            channels1--;
            chns++;
            src_areas++;
            dst_areas++;
            if (channels1 == 0 ||
                    src_areas->step != step ||
                    src_areas->addr != src_addr ||
                    dst_areas->addr != dst_addr ||
                    src_areas->first != src_areas[-1].first + width ||
                    dst_areas->first != dst_areas[-1].first + width)
                break;
        }
        if (chns > 1 && chns * width == step)
        {
            if (src_offset != dst_offset ||
                    src_start->addr != dst_start->addr ||
                    src_start->first != dst_start->first)
            {
                /* Collapse the areas */
                snd_pcm_channel_area_t s, d;
                s.addr = src_start->addr;
                s.first = src_start->first;
                s.step = width;
                d.addr = dst_start->addr;
                d.first = dst_start->first;
                d.step = width;
                snd_pcm_area_copy(&d, dst_offset * chns,
                                  &s, src_offset * chns,
                                  frames * chns, format);
            }
            channels -= chns;
        }
        else
        {
            snd_pcm_area_copy(dst_start, dst_offset,
                              src_start, src_offset,
                              frames, format);
            src_areas = src_start + 1;
            dst_areas = dst_start + 1;
            channels--;
        }
    }
    return 0;
}

/* locked version */
int __snd_pcm_mmap_begin(snd_pcm_t *pcm, const snd_pcm_channel_area_t **areas,
                         snd_pcm_uframes_t *offset, snd_pcm_uframes_t *frames)
{
    snd_pcm_uframes_t cont;
    snd_pcm_uframes_t f;
    snd_pcm_uframes_t avail;
    const snd_pcm_channel_area_t *xareas;
    snd_assert(pcm && areas && offset && frames);
    xareas = snd_pcm_mmap_areas(pcm);

    if (xareas == NULL)
        return -EBADFD;
    *areas = xareas;
    *offset = pcm->status.appl_ptr % pcm->abuf.buf_size;
    avail = snd_pcm_mmap_avail(pcm);
    if (avail > pcm->abuf.buf_size)
        avail = pcm->abuf.buf_size;
    cont = pcm->abuf.buf_size - *offset;
    f = *frames;
    if (f > avail)
        f = avail;
    if (f > cont)
        f = cont;
    *frames = f;
    return 0;
}

/* locked version*/
snd_pcm_sframes_t __snd_pcm_mmap_commit(snd_pcm_t *pcm,
                                        snd_pcm_uframes_t offset,
                                        snd_pcm_uframes_t frames)
{
    snd_assert(pcm);
    if (CHECK_SANITY(offset != pcm->status.appl_ptr % pcm->abuf.buf_size))
    {
        SND_ERR("commit offset (%ld) doesn't match with appl_ptr (%ld) %% buf_size (%ld)",
                offset, pcm->status.appl_ptr, pcm->abuf.buf_size);
        return -EPIPE;
    }
    if (CHECK_SANITY(frames > snd_pcm_mmap_avail(pcm)))
    {
        SND_ERR("commit frames (%ld) overflow (avail = %ld)", frames,
                snd_pcm_mmap_avail(pcm));
        return -EPIPE;
    }
    return pcm->fast_ops->mmap_commit(pcm, offset, frames);
}

snd_pcm_sframes_t snd_pcm_mmap_commit(snd_pcm_t *pcm,
                                      snd_pcm_uframes_t offset,
                                      snd_pcm_uframes_t frames)
{
    snd_pcm_sframes_t result;

    snd_pcm_lock(pcm);
    result = __snd_pcm_mmap_commit(pcm, offset, frames);
    snd_pcm_unlock(pcm);
    return result;
}

/**
 * \brief Application request to access a portion of direct (mmap) area
 * \param pcm PCM handle
 * \param areas Returned mmap channel areas
 * \param offset Returned mmap area offset in area steps (== frames)
 * \param frames mmap area portion size in frames (wanted on entry, contiguous available on exit)
 * \return 0 on success otherwise a negative error code
 *
 * It is necessary to call the snd_pcm_avail_update() function directly before
 * this call. Otherwise, this function can return a wrong count of available frames.
 *
 * The function should be called before a sample-direct area can be accessed.
 * The resulting size parameter is always less or equal to the input count of frames
 * and can be zero, if no frames can be processed (the ring buffer is full).
 *
 * See the snd_pcm_mmap_commit() function to finish the frame processing in
 * the direct areas.
 *
 * The function is thread-safe when built with the proper option.
 */
int snd_pcm_mmap_begin(snd_pcm_t *pcm,
                       const snd_pcm_channel_area_t **areas,
                       snd_pcm_uframes_t *offset,
                       snd_pcm_uframes_t *frames)
{
    int err;

    snd_pcm_lock(pcm);
    err = __snd_pcm_mmap_begin(pcm, areas, offset, frames);
    snd_pcm_unlock(pcm);
    return err;
}

void snd_pcm_areas_from_buf(snd_pcm_t *pcm, snd_pcm_channel_area_t *areas,
                            void *buf)
{
    unsigned int channel;
    unsigned int channels;

    snd_pcm_lock(pcm);
    channels = pcm->params.channels;
    for (channel = 0; channel < channels; ++channel, ++areas)
    {
        areas->addr = buf;
        areas->first = channel * pcm->params.sampleBits;
        areas->step = frame_bits(pcm);
    }
    snd_pcm_unlock(pcm);
}

/**
 * \brief Return number of frames ready to be read (capture) / written (playback)
 * \param pcm PCM handle
 * \return a positive number of frames ready otherwise a negative
 * error code
 *
 * On capture does all the actions needed to transport to application
 * level all the ready frames across underlying layers.
 *
 * The position is not synced with hardware (driver) position in the sound
 * ring buffer in this function. This function is a light version of
 * #snd_pcm_avail() .
 *
 * Using this function is ideal after poll() or select() when audio
 * file descriptor made the event and when application expects just period
 * timing.
 *
 * Also this function might be called after #snd_pcm_delay() or
 * #snd_pcm_hwsync() functions to move private ring buffer pointers
 * in alsa-lib (the internal plugin chain).
 *
 * The function is thread-safe when built with the proper option.
 */
snd_pcm_sframes_t snd_pcm_avail_update(snd_pcm_t *pcm)
{
    snd_pcm_sframes_t result;

    snd_pcm_lock(pcm);
    result = __snd_pcm_avail_update(pcm);
    snd_pcm_unlock(pcm);
    return result;
}

snd_pcm_sframes_t snd_pcm_read_areas(snd_pcm_t *pcm, const snd_pcm_channel_area_t *areas,
                                     snd_pcm_uframes_t offset, snd_pcm_uframes_t size,
                                     snd_pcm_xfer_areas_func_t func)
{
    snd_pcm_uframes_t xfer = 0;
    snd_pcm_sframes_t err = 0;
    snd_pcm_state_t state;

    if (size == 0)
        return 0;

    __snd_pcm_lock(pcm); /* forced lock */
    while (size > 0)
    {
        snd_pcm_uframes_t frames;
        snd_pcm_sframes_t avail;

        state = __snd_pcm_state(pcm);
        switch (state)
        {
        case SND_PCM_STATE_XRUN:
        case SND_PCM_STATE_PREPARED:
            err = __snd_pcm_start(pcm);
            if (err < 0)
                goto _end;
            break;
        case SND_PCM_STATE_RUNNING:
            err = __snd_pcm_hwsync(pcm);
            if (err < 0)
                goto _end;
            break;
        case SND_PCM_STATE_DRAINING:
            break;
        default:
            err = -EBADFD;
            goto _end;
        }
        avail = __snd_pcm_avail_update(pcm);
        if (avail < 0)
        {
            err = avail;
            goto _end;
        }
        if (avail == 0)
        {
            if (state == SND_PCM_STATE_DRAINING)
                goto _end;
        }
        frames = size;
        if (frames > (snd_pcm_uframes_t) avail)
            frames = avail;
        if (! frames)
            break;
        err = func(pcm, areas, offset, frames);
        if (err < 0)
            break;
        frames = err;
        offset += frames;
        size -= frames;
        xfer += frames;
    }
_end:
    __snd_pcm_unlock(pcm);
    return xfer > 0 ? (snd_pcm_sframes_t) xfer : snd_pcm_check_error(pcm, err);
}

snd_pcm_sframes_t snd_pcm_write_areas(snd_pcm_t *pcm, const snd_pcm_channel_area_t *areas,
                                      snd_pcm_uframes_t offset, snd_pcm_uframes_t size,
                                      snd_pcm_xfer_areas_func_t func)
{
    snd_pcm_uframes_t xfer = 0;
    snd_pcm_sframes_t err = 0;
    snd_pcm_state_t state;

    if (size == 0)
        return 0;

    __snd_pcm_lock(pcm); /* forced lock */
    while (size > 0)
    {
        snd_pcm_uframes_t frames;
        snd_pcm_sframes_t avail;

        state = __snd_pcm_state(pcm);
        switch (state)
        {
        case SND_PCM_STATE_PREPARED:
            break;
        case SND_PCM_STATE_RUNNING:
            err = __snd_pcm_hwsync(pcm);
            if (err < 0)
                goto _end;
            break;
        case SND_PCM_STATE_XRUN:
            break;
        default:
            err = -EBADFD;
            goto _end;
        }
        avail = __snd_pcm_avail_update(pcm);
        if (avail < 0)
        {
            err = avail;
            goto _end;
        }

        frames = size;
        if (frames > (snd_pcm_uframes_t) avail)
            frames = avail;
        if (! frames)
            break;
        err = func(pcm, areas, offset, frames);
        if (err < 0)
            break;
        frames = err;
        if ((state == SND_PCM_STATE_PREPARED) || (state == SND_PCM_STATE_XRUN))
        {
            snd_pcm_sframes_t hw_avail = pcm->abuf.buf_size - avail;
            hw_avail += frames;
            /* some plugins might automatically start the stream */
            state = __snd_pcm_state(pcm);
            if (((state == SND_PCM_STATE_PREPARED) || (state == SND_PCM_STATE_XRUN)) &&
                    hw_avail >= (snd_pcm_sframes_t) pcm->start_threshold)
            {
                err = __snd_pcm_start(pcm);
                if (err < 0)
                    goto _end;
            }
        }

        offset += frames;
        size -= frames;
        xfer += frames;
    }
_end:
    __snd_pcm_unlock(pcm);
    return xfer > 0 ? (snd_pcm_sframes_t) xfer : snd_pcm_check_error(pcm, err);
}

#endif /* CONFIG_DRIVER_AUDIO_PCM */
