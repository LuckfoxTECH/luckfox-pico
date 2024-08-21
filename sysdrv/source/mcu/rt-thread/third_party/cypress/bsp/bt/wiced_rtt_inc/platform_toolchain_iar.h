/*
 * $ Copyright Cypress Semiconductor $
 */
#ifndef INCLUDED_WWD_TOOLCHAIN_H
#define INCLUDED_WWD_TOOLCHAIN_H

#include <stddef.h>
#include "intrinsics.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
 *                      Macros
 ******************************************************/

#define WEAK __weak

#ifndef ALWAYS_INLINE
#define ALWAYS_INLINE_PRE     _Pragma( "inline=forced" )
#define ALWAYS_INLINE
#endif

#ifndef MAY_BE_UNUSED
#define MAY_BE_UNUSED_PRE    __root
#define MAY_BE_UNUSED
#endif

#ifndef NORETURN
#define NORETURN_PRE         __noreturn
#define NORETURN
#endif

#ifndef ALIGNED
#define PRAGMA_ALIGN_4096       _Pragma( "data_alignment=4096" )
#define PRAGMA_ALIGN_2048       _Pragma( "data_alignment=2048" )
#define PRAGMA_ALIGN_256        _Pragma( "data_alignment=256" )
#define PRAGMA_ALIGN_128        _Pragma( "data_alignment=128" )
#define PRAGMA_ALIGN_64         _Pragma( "data_alignment=64" )
#define PRAGMA_ALIGN_48         _Pragma( "data_alignment=48" )
#define PRAGMA_ALIGN_32         _Pragma( "data_alignment=32" )
#define PRAGMA_ALIGN_4          _Pragma( "data_alignment=4" )
#define ALIGNED_PRE(size)       PRAGMA_ALIGN_ ## size
#define ALIGNED(size)
#define IAR_AUTO_VAR_MAX_ALIGN  8
#endif

#ifndef STATIC_ALWAYS_INLINE
#define STATIC_ALWAYS_INLINE _Pragma( "inline=forced" ) static inline
#endif

#ifndef INLINE_ASM
#define INLINE_ASM  __asm
#endif

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/
void *memmem(const void *haystack, size_t haystacklen, const void *needle, size_t needlelen);
void *memrchr(const void *s, int c, size_t n);
size_t strlcpy(char *dest, const char *src, size_t size);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* #ifndef INCLUDED_WWD_TOOLCHAIN_H */
