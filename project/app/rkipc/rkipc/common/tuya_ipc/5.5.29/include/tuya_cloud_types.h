/*
tuya_cloud_types.h
Copyright(C),2018-2020, 涂鸦科技 www.tuya.comm
*/

#ifndef TUYA_CLOUD_TYPES_H
#define TUYA_CLOUD_TYPES_H

#include "tuya_iot_config.h"
#include <stdarg.h>

#if OPERATING_SYSTEM == SYSTEM_LINUX
#include <ctype.h>
#endif

#include "tuya_cloud_error_code.h"
#include "tuya_error_code.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef long long DLONG_T;
typedef DLONG_T *PDLONG_T;
typedef unsigned long long DDWORD_T;
typedef DDWORD_T *PDDWORD_T;
typedef float FLOAT_T;
typedef FLOAT_T *PFLOAT_T;
typedef signed int INT_T;
typedef int *PINT_T;
typedef void *PVOID_T;
typedef char CHAR_T;
typedef char *PCHAR_T;
typedef signed char SCHAR_T;
typedef unsigned char UCHAR_T;
typedef short SHORT_T;
typedef unsigned short USHORT_T;
typedef short *PSHORT_T;
typedef long LONG_T;
typedef unsigned long ULONG_T;
typedef long *PLONG_T;
typedef unsigned char BYTE_T;
typedef BYTE_T *PBYTE_T;
typedef unsigned short WORD_T;
typedef WORD_T *PWORD_T;
typedef unsigned int DWORD_T;
typedef DWORD_T *PDWORD_T;
typedef unsigned int UINT_T;
typedef unsigned int *PUINT_T;
typedef int BOOL_T;
typedef BOOL_T *PBOOL_T;
typedef long long int INT64_T;
typedef INT64_T *PINT64_T;
typedef unsigned long long int UINT64_T;
typedef UINT64_T *PUINT64_T;
typedef short INT16_T;
typedef INT16_T *PINT16_T;
typedef unsigned short UINT16_T;
typedef UINT16_T *PUINT16_T;
typedef char INT8_T;
typedef INT8_T *PINT8_T;
typedef unsigned char UINT8_T;
typedef UINT8_T *PUINT8_T;

typedef ULONG_T TIME_MS;
typedef ULONG_T TIME_S;
typedef unsigned int TIME_T;

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef VOID
#define VOID void
#endif

#ifndef VOID_T
#define VOID_T void
#endif

#ifndef CONST
#define CONST const
#endif

#ifndef STATIC
#define STATIC static
#endif

#ifndef SIZEOF
#define SIZEOF sizeof
#endif

#ifndef INLINE
#define INLINE inline
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

#ifndef bool_t
typedef int bool_t;
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

typedef size_t SIZE_T;

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef BIT
#define BIT(nr) (1UL << (nr))
#endif
#define MAKEWORD(a, b) ((WORD_T)(((BYTE_T)(a)) | ((WORD_T)((BYTE_T)(b))) << 8))
#define MAKELONG(a, b) ((LONG_T)(((WORD_T)(a)) | ((DWORD_T)((WORD_T)(b))) << 16))
#define LOWORD(l) ((WORD_T)(l))
#define HIWORD(l) ((WORD_T)(((DWORD_T)(l) >> 16) & 0xFFFF))
#define LOBYTE(w) ((BYTE_T)(w))
#define HIBYTE(w) ((BYTE_T)(((WORD_T)(w) >> 8) & 0xFF))

#define WORD_SWAP(X) (((X << 8) | (X >> 8)) & 0xFFFF)
#define DWORD_SWAP(X)                                                                              \
	((((X)&0xff) << 24) + (((X)&0xff00) << 8) + (((X)&0xff0000) >> 8) + (((X)&0xff000000) >> 24))
#define DDWORD_SWAP(X)                                                                             \
	{                                                                                              \
		unsigned long temp_low, temp_high;                                                         \
		temp_low = DWORD_SWAP((unsigned long)((X)&0xffffffff));                                    \
		temp_high = DWORD_SWAP((unsigned long)((X) >> 32));                                        \
		X = 0;                                                                                     \
		X |= temp_low;                                                                             \
		X <<= 32;                                                                                  \
		X |= temp_high;                                                                            \
	}

#if defined(LITTLE_END) && (LITTLE_END == 1)
#define UNI_NTOHS(X) WORD_SWAP(X)
#define UNI_HTONS(X) WORD_SWAP(X)
#define UNI_NTOHL(X) DWORD_SWAP(X)
#define UNI_HTONL(X) DWORD_SWAP(X)
#define UNI_NTOHLL(X) DDWORD_SWAP(X)
#define UNI_HTONLL(X) DDWORD_SWAP(X)

#else

#define UNI_NTOHS(X) X
#define UNI_HTONS(X) X
#define UNI_NTOHL(X) X
#define UNI_HTONL(X) X
#define UNI_NTOHLL(X) X
#define UNI_HTONLL(X) X

#endif

#define OFFSOF(s, m) ((SIZE_T)(&(((s *)0)->m)))
#define CNTSOF(a) (sizeof(a) / sizeof(a[0]))
#define CNTR_OF(ptr, type, member)                                                                 \
	({ (type *)((char *)ptr - OFFSOF(type, member)); }) // continer of

#ifdef __cplusplus
}
#endif

#endif // TUYA_CLOUD_TYPES_H
