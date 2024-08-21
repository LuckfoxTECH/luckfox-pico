/*
 * SPDX-License-Identifier: (Apache-2.0 OR LGPL-2.1-or-later)
 *
 * Copyright (c) 2020 Fuzhou Rockchip Electronics Co., Ltd
 *
 * Change Logs:
 * Date           Author          Notes
 * 2020-03-03     Xing Zheng      The first version
 */

#ifndef __DRIVER_AUDIO_PCM_OS_INTERFACE_H__
#define __DRIVER_AUDIO_PCM_OS_INTERFACE_H__

#include "drv_heap.h"
#include "../audio/rk_audio.h"

#define LONG_MAX    ((long)(~0UL>>1))
#define ULONG_MAX   (~0UL)
#define UINT_MAX    (~0U)

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#endif

#ifndef ATTRIBUTE_UNUSED
/** do not print warning (gcc) when function parameter is not used */
#define ATTRIBUTE_UNUSED __attribute__ ((__unused__))
#endif

/* Here are used for pcm plugin. */
#ifndef isprint
#define in_range(c, lo, up)  ((unsigned char)c >= lo && (unsigned char)c <= up)
#define isprint(c)           in_range(c, 0x20, 0x7f)
#define isdigit(c)           in_range(c, '0', '9')
#define isxdigit(c)          (isdigit(c) || in_range(c, 'a', 'f') || in_range(c, 'A', 'F'))
#define islower(c)           in_range(c, 'a', 'z')
#define isspace(c)           (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v')
#endif

#define __bswap_constant_16(x)                  \
  ((__uint16_t) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8)))

static __inline unsigned short int
bswap_16(unsigned short int __bsx)
{
    return __bswap_constant_16(__bsx);
}

static __inline unsigned int
bswap_32(unsigned int __bsx)
{
    return __builtin_bswap32(__bsx);
}

#if defined(__RK_OS__)

#define SND_SUCCESS    RK_SUCCESS
#define SND_ERROR      RK_ERROR
#define SND_PARA_ERR   RK_PARA_ERR
#define SND_TIMEOUT    RK_TIMEOUT
#define SND_EXIST      RK_EXIST
#define SND_EMPTY      RK_EMPTY

#define SND_VERBOSE(fmt, args...)
#define SND_INFO       rk_printf
#define SND_ERR        rk_printf

#define snd_zmalloc    rkos_memory_zalloc
#define snd_free       rkos_memory_free
#define snd_memset     rkos_memset

#define snd_assert     RK_ASSERT

typedef rk_err_t       snd_err_t;
typedef rk_size_t      snd_size_t;
typedef rk_off_t       snd_off_t;

#elif defined(RT_USING_CORE_RTTHREAD)

#define SND_SUCCESS    RT_EOK
#define SND_ERROR      RT_ERROR
#define SND_PARA_ERR   RT_EINVAL
#define SND_TIMEOUT    RT_ETIMEOUT
#define SND_EXIST      RT_EINVAL
#define SND_EMPTY      RT_EEMPTY

#define SND_VERBOSE(fmt, args...)
#define SND_INFO(...)  rt_kprintf("SND_I: ");rt_kprintf(__VA_ARGS__)
#define SND_ERR(...)   rt_kprintf("SND_E: ");rt_kprintf(__VA_ARGS__)

#define snd_zmalloc    rt_malloc
#define snd_free       rt_free
#define snd_memset     rt_memset

#define snd_assert     RT_ASSERT

#define CONFIG_SOFTVOL_MIN_DB SOFTVOL_MIN_DB
#define CONFIG_SOFTVOL_MAX_DB SOFTVOL_MAX_DB
#define CONFIG_SOFTVOL_FORMAT SOFTVOL_FORMAT
#define CONFIG_SOFTVOL_CHANNELS SOFTVOL_CHANNELS
#define CONFIG_SOFTVOL_RESOLUTION SOFTVOL_RESOLUTION

typedef rt_err_t       snd_err_t;
typedef rt_size_t      snd_size_t;
typedef rt_off_t       snd_off_t;

#endif

#endif /* __DRIVER_AUDIO_PCM_OS_INTERFACE_H__ */
