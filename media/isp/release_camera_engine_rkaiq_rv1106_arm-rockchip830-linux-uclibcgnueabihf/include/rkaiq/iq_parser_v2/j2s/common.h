/*
 *  Copyright (c) 2020, Rockchip Electronics Co., Ltd
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#ifndef J2S_COMMON_H
#define J2S_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define J2S_VERSION "1.0.0~20201229"

//#define DEBUG

#ifndef unlikely
#define unlikely(x) __builtin_expect((x), 0)
#endif

#ifndef MSG
#define MSG(level, fmt, args...)                                      \
    fprintf(stderr, #level ": %s(%d) [%s]: " fmt, __FILE__, __LINE__, \
        __func__, ##args)
#endif

#ifndef DBG
#if defined(NDEBUG)
#define DBG(fmt, args...)
#elif defined(DEBUG)
#define DBG(fmt, args...) MSG(D, fmt, ##args)
#else
#define DBG(fmt, args...)    \
    if (getenv("J2S_DEBUG")) \
    MSG(D, fmt, ##args)
#endif
#endif

#ifndef INFO
#define INFO(fmt, args...) MSG(I, fmt, ##args)
#endif

#ifndef WARN
#define WARN(fmt, args...) MSG(W, fmt, ##args)
#endif

#ifndef ERR
#define ERR(fmt, args...) MSG(E, fmt, ##args)
#endif

#ifndef DASSERT
#define DASSERT(b, action)                             \
    do {                                               \
        if (unlikely(!(b))) {                          \
            ERR("debug assertion failure (%s)\n", #b); \
            action;                                    \
        }                                              \
    } while (0)
#endif

#ifndef DASSERT_MSG
#define DASSERT_MSG(b, action, fmt, args...) \
    do {                                     \
        if (unlikely(!(b))) {                \
            ERR(fmt, ##args);                \
            action;                          \
        }                                    \
    } while (0)
#endif

#define MAX_LINE 8192
#define MAX_NAME 32 

#define J2S_DESC_MAGIC "@desc:"

typedef enum {
    J2S_TYPE_INT_8 = 1,
    J2S_TYPE_UINT_8,
    J2S_TYPE_INT_16,
    J2S_TYPE_UINT_16,
    J2S_TYPE_INT_32,
    J2S_TYPE_UINT_32,
    J2S_TYPE_INT_64,
    J2S_TYPE_UINT_64,
    J2S_TYPE_FLOAT,
    J2S_TYPE_DOUBLE,
    J2S_TYPE_STRING, /* For char *|char [] */
    J2S_TYPE_STRUCT,
} j2s_type;

typedef enum {
    J2S_FLAG_ARRAY = 1 << 0,
    J2S_FLAG_POINTER = 1 << 1,
    J2S_FLAG_DEP_ARRAY = 1 << 2, /* int array[1][2] */
    J2S_FLAG_DEP_POINTER = 1 << 3, /* char ** */
    J2S_FLAG_ARRAY_POINTER = 1 << 4, /* typedef int (*array_ptr_int)[4] */
} j2s_flag;

#define J2S_IS_SIMPLE_STRING(obj) \
    ((obj)->type == J2S_TYPE_STRING && ((obj)->flags == J2S_FLAG_ARRAY || (obj)->flags == J2S_FLAG_POINTER))

#define J2S_IS_ARRAY(obj) \
    ((obj)->flags & J2S_FLAG_ARRAY && !((obj)->flags & J2S_FLAG_ARRAY_POINTER))

#define J2S_IS_POINTER(obj) \
    ((obj)->flags & J2S_FLAG_POINTER && !J2S_IS_ARRAY(obj))

#ifdef __cplusplus
}
#endif

#endif // J2S_COMMON_H
