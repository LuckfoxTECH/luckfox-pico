/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 */
#ifndef STDIO_H
#define STDIO_H

#include <stddef.h>
#include <stdarg.h>

#include <sys/reent.h>

_BEGIN_STD_C
#if !defined(__FILE_defined)
typedef __FILE FILE;
# define __FILE_defined
#endif

/*
 * The following three definitions are for ANSI C, which took them
 * from System V, which stupidly took internal interface macros and
 * made them official arguments to setvbuf(), without renaming them.
 * Hence, these ugly _IOxxx names are *supposed* to appear in user code.
 *
 * Although these happen to match their counterparts above, the
 * implementation does not rely on that (so these could be renumbered).
 */
#define	_IOFBF	0		/* setvbuf should set fully buffered */
#define	_IOLBF	1		/* setvbuf should set line buffered */
#define	_IONBF	2		/* setvbuf should set unbuffered */

#define	EOF	(-1)

#ifdef __BUFSIZ__
#define	BUFSIZ		__BUFSIZ__
#else
#define	BUFSIZ		1024
#endif

#ifdef __FOPEN_MAX__
#define FOPEN_MAX	__FOPEN_MAX__
#else
#define	FOPEN_MAX	20
#endif

#ifdef __FILENAME_MAX__
#define FILENAME_MAX    __FILENAME_MAX__
#else
#define	FILENAME_MAX	1024
#endif

#ifdef __L_tmpnam__
#define L_tmpnam	__L_tmpnam__
#else
#define	L_tmpnam	FILENAME_MAX
#endif


#ifndef SEEK_SET
#define	SEEK_SET	0	/* set file offset to offset */
#endif
#ifndef SEEK_CUR
#define	SEEK_CUR	1	/* set file offset to current plus offset */
#endif
#ifndef SEEK_END
#define	SEEK_END	2	/* set file offset to EOF plus offset */
#endif

#define	TMP_MAX		26

int printf(const char *fmt, ...)
                    __attribute__ ((__format__ (__printf__, 1, 2)));
/* sprintf() is unsafe and should not be used. Prefer snprintf(). */
int sprintf(char *str, const char *fmt, ...)
                    __attribute__ ((__format__ (__printf__, 2, 3)))
                    __attribute__ ((deprecated));
int snprintf(char *str, size_t size, const char *fmt, ...)
                    __attribute__ ((__format__ (__printf__, 3, 4)));
int vsnprintf (char *str, size_t size, const char *fmt, va_list ap)
                    __attribute__ ((__format__ (__printf__, 3, 0)));

int puts(const char *str);
int putchar(int c);

int sscanf(const char *str, const char *format, ...)
	__attribute__ ((__format__ (__scanf__, 2, 3)))
	__attribute__ ((__noreturn__));

#ifndef __KERNEL__

extern FILE *stdout;
extern FILE *stderr;

/*
 * The functions below send their output synchronously to the secure console.
 * They treat stdout and stderr the same, and will abort if stream is not one or
 * the other.
 */

int fputc(int c, FILE *stream);
int fputs(const char *s, FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
#endif

_END_STD_C

#endif /*STDIO_H*/
