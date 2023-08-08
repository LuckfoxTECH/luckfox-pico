/*
 * Copyright 2020 Rockchip Electronics Co. LTD
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
 */

#ifndef INCLUDE_RT_MPI_RK_TYPE_H_
#define INCLUDE_RT_MPI_RK_TYPE_H_

#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef unsigned char           RK_UCHAR;
typedef uint8_t                 RK_U8;
typedef uint16_t                RK_U16;
typedef uint32_t                RK_U32;
typedef uint32_t                RK_UL;
typedef uintptr_t               RK_UINTPTR_T;

typedef char                    RK_CHAR;
typedef int8_t                  RK_S8;
typedef int16_t                 RK_S16;
typedef int32_t                 RK_S32;
typedef int32_t                 RK_SL;

typedef float                   RK_FLOAT;
typedef double                  RK_DOUBLE;

typedef uint64_t                RK_U64;
typedef int64_t                 RK_S64;

typedef uint32_t                RK_SIZE_T;
typedef uint32_t                RK_LENGTH_T;

typedef unsigned int            RK_HANDLE;

typedef enum {
    RK_FALSE = 0,
    RK_TRUE  = 1,
} RK_BOOL;

#ifndef NULL
    #define NULL                0L
#endif

#define RK_NULL                 0L
#define RK_SUCCESS              0
#define RK_FAILURE              (-1)

#define RK_VOID                 void

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  // INCLUDE_RT_MPI_RK_TYPE_H_
