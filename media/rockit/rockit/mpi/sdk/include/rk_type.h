/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_RK_TYPE_H_
#define INCLUDE_RT_MPI_RK_TYPE_H_
#ifndef __BUILD_KO__
#include <stdint.h>
#endif
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef unsigned char RK_UCHAR;
typedef uint8_t RK_U8;
typedef uint16_t RK_U16;
typedef uint32_t RK_U32;
typedef uint32_t RK_UL;
typedef uintptr_t RK_UINTPTR_T;

typedef char RK_CHAR;
typedef int8_t RK_S8;
typedef int16_t RK_S16;
typedef int32_t RK_S32;
typedef int32_t RK_SL;

typedef float RK_FLOAT;
typedef double RK_DOUBLE;

typedef uint64_t RK_U64;
typedef int64_t RK_S64;

typedef uint32_t RK_SIZE_T;
typedef uint32_t RK_LENGTH_T;

typedef unsigned int RK_HANDLE;

typedef enum {
  RK_FALSE = 0,
  RK_TRUE = 1,
} RK_BOOL;

#ifndef NULL
#define NULL 0L
#endif

#define RK_NULL 0L
#define RK_SUCCESS 0
#define RK_FAILURE (-1)

#define RK_VOID void

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif // INCLUDE_RT_MPI_RK_TYPE_H_
