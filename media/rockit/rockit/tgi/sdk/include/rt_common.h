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

#ifndef INCLUDE_RT_BASE_RT_COMMON_H_
#define INCLUDE_RT_BASE_RT_COMMON_H_

#define RT_TAG_SIZE            32

#define RT_ABS(x)              ((x) < (0) ? -(x) : (x))

#define RT_MAX(a, b)           ((a) > (b) ? (a) : (b))
#define RT_MAX3(a, b, c)       RT_MAX(RT_MAX(a, b) , c)
#define RT_MAX4(a, b, c, d)    RT_MAX((a), RT_MAX3((b), (c), (d)))

#define RT_CLIP(a, l, h)       ((a) < (l) ? (l) : ((a) > (h) ? (h) : (a)))

#define RT_MIN(a, b)            ((a) > (b) ? (b) : (a))
#define RT_MIN3(a, b, c)       RT_MIN(MPP_MIN(a, b), c)
#define RT_MIN4(a, b, c, d)    RT_MIN((a), RT_MIN3((b), (c), (d)))

#define RT_SWAP(type, a, b) \
    do { \
        type SWAP_tmp = b; \
        b = a; \
        a = SWAP_tmp; \
    } while (0)

#define RT_ARRAY_ELEMS(a)      (sizeof(a) / sizeof((a)[0]))
#define RT_ALIGN(x, a)         (((x) + (a) - 1) & ~((a) - 1))
#define RT_ALIGN_ODD(x, a)     (RT_ALIGN(x, a) | a)
#define RT_ALIGN_16(x)         RT_ALIGN(x, 16)
#define RT_ALIGN_64(x)         RT_ALIGN(x, 64)
#define RT_ALIGN_256(x)        RT_ALIGN(x, 256)
#define RT_ALIGN_256_ODD(x)    (RT_ALIGN(x, 256) | 256)

#define RT_VSWAP(a, b)         { a ^= b; b ^= a; a ^= b; }

#define RT_MERGE_INT64(a, b)   (((INT64)a << 32) + b)

#ifndef RT_INT64_MIN
#define RT_INT64_MIN       (-0x7fffffffffffffffLL-1)
#define RT_NOPTS_VALUE     (-0x7fffffffffffffffLL-1)
#endif

#ifndef RT_INT64_MAX
#define RT_INT64_MAX        INT64_C(9223372036854775807)
#endif

#ifndef RT_INT32_MAX
#define RT_INT32_MAX        (2147483647)
#endif

#ifndef RT_SEEK_SIZE
#define RT_SEEK_SIZE        0x10000
#endif

#ifndef RT_ERROR_EOF
#define RT_ERROR_EOF        (-(int)MKTAG('E','O','F',' ')) // NOLINT  End of file
#endif

#ifndef RT_ERROR
#define RT_ERROR(e)         (-(e))   // NOLINT
#endif

#define RT_PTS_IS_VALID(pts)    (pts != RT_NOPTS_VALUE)

#define RT_FOURCC(f)                                                 \
    f & 0xFF, (f >> 8) & 0xFF, (f >> 16) & 0xFF, (f >> 24) & 0xFF

#endif  // INCLUDE_RT_BASE_RT_COMMON_H_
