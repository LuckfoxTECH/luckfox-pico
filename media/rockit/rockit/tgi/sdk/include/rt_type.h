/*
 * Copyright 2018 Rockchip Electronics Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * author: martin.cheng@rock-chips.com
 *   date: 20180704
 */

#ifndef INCLUDE_RT_BASE_RT_TYPE_H_
#define INCLUDE_RT_BASE_RT_TYPE_H_

#include <stdint.h>

#define RT_NULL     0
#define RT_NONE     "none"
#define RT_BOOL     char
#define RT_TRUE     1
#define RT_FALSE    0

#define TODO_FLAG   0

#ifndef uid_t
#define uid_t        uint32_t
#endif
typedef int32_t      rt_status;

typedef int8_t       INT8;
typedef int16_t      INT16;
typedef int32_t      INT32;
typedef int64_t      INT64;

typedef uint8_t      UINT8;
typedef uint8_t      UCHAR;
typedef uint16_t     UINT16;
typedef uint32_t     UINT32;
typedef uint64_t     UINT64;

typedef void*        RT_PTR;
typedef void         RT_VOID;
typedef float        RT_FLOAT;
typedef short        RT_SHORT;   // NOLINT

#define RtToBool(cond)  ((cond) != 0)
#define RtToS8(x)       ((INT8)(x))
#define RtToU8(x)       ((UINT8)(x))
#define RtToS16(x)      ((INT16)(x))
#define RtToU16(x)      ((UINT16)(x))
#define RtToS32(x)      ((INT32)(x))
#define RtToU32(x)      ((UINT32)(x))

#define RT_MaxS16   32767
#define RT_MinS16   -32767
#define RT_MaxU16   0xFFFF
#define RT_MinU16   0
#define RT_MaxS32   0x7FFFFFFF
#define RT_MinS32   -SK_MaxS32
#define RT_MaxU32   0xFFFFFFFF
#define RT_MinU32   0
#define RT_NaN32    (1 << 31)

#define RtAlign2(x)     (((x) + 1) >> 1 << 1)
#define RtIsAlign2(x)   (0 == ((x) & 1))

#define RtAlign4(x)     (((x) + 3) >> 2 << 2)
#define RtIsAlign4(x)   (0 == ((x) & 3))

#define RtAlign8(x)     (((x) + 7) >> 3 << 3)
#define RtIsAlign8(x)   (0 == ((x) & 7))

#define MKTAG(a, b, c, d) ((a) | ((b) << 8) | ((c) << 16) | ((unsigned)(d) << 24))

#define RT_RB(type, x)     (((const type*)(x))[0])
#define RT_WB(type, p, d)  do { ((type*)(p))[0] = (d); } while (0)   // NOLINT

typedef struct _RTRect {
    INT32 x;
    INT32 y;
    INT32 w;
    INT32 h;
} RTRect;

typedef struct _RTCropInfo {
    RTRect region;
    UINT32 virWidth;
    UINT32 virHeight;
} RTCropInfo;

#endif  // INCLUDE_RT_BASE_RT_TYPE_H_
