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
 *   date: 20181126
 */

#ifndef SRC_RT_BASE_INCLUDE_RT_STRING_UTILS_H_
#define SRC_RT_BASE_INCLUDE_RT_STRING_UTILS_H_

#include "rt_header.h"  // NOLINT

#include <stdio.h>      // NOLINT
#include <stdarg.h>     // NOLINT
#include <string>       // NOLINT
#include <string.h>     // NOLINT   strerror/strstr/strcmp

#ifdef __cplusplus
extern "C" {
#endif

INT32       util_vsnprintf(char* buffer, size_t max_len, const char* format, va_list args);
INT32       util_snprintf(char* buffer, size_t max_len, const char* format, ...);
std::string util_vsprintf(const char* format, va_list args);
std::string util_sprintf(const char* format, ...);
std::string util_to_string(int num);
const char* util_to_char(const std::string buf);
char* util_stristr(const char* str1, const char* str2);
char* util_strsepstr(const char* str1, const char* str2, const char* delim);

INT32 util_strcasecmp(const char* str1, const char* str2);
INT32 util_strncasecmp(const char* str1, const char* str2, size_t n);
INT32 util_strtolower(char* dst, const char* src, INT32 length);
INT32 util_strtoupper(char* dst, const char* src, INT32 length);

INT32 util_strlcpy(char *dst, const char *src, INT32 size);
INT32 util_strlcat(char *dst, const char *src, INT32 size);

INT32 util_dump_str(INT32 fd, const char* args);

/**
 * Locale-independent conversion of ASCII characters to uppercase.
 */
static inline const int rt_toupper(int c) {
    if (c >= 'a' && c <= 'z')
        c ^= 0x20;
    return c;
}

/**
 * Locale-independent conversion of ASCII characters to lowercase.
 */
static inline const int rt_tolower(int c) {
    if (c >= 'A' && c <= 'Z')
        c ^= 0x20;
    return c;
}

#define RT_HASH_SEED   131

constexpr UINT64 func_hash_bkdr_constexpr(const char *str, UINT64 last_value = 0) {
    return *str ? func_hash_bkdr_constexpr(str + 1, last_value * RT_HASH_SEED + *str) : last_value;
}

// BKDR Hash Function
static inline UINT64 func_hash_bkdr(const char *str) {
    UINT32 seed = RT_HASH_SEED;  // 31 131 1313 13131 131313 etc..
    UINT64 hash = 0;

    while (*str) {
        hash = hash * RT_HASH_SEED + (*str);
        *str++;     // NOLINT
    }

    return hash;
}

#define RTSTRING_SWITCH(string)     switch (func_hash_bkdr_constexpr(string))
#define RTSTRING_CASE(string)       case func_hash_bkdr_constexpr(string)

#ifdef __cplusplus
}
#endif

#endif  // SRC_RT_BASE_INCLUDE_RT_STRING_UTILS_H_
