/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 */

/*
 * This file provides what C99 standard requires for <stdlib.h> in
 * 7.20.3 Memory management functions
 */

#ifndef STDLIB_H
#define STDLIB_H

#include <compiler.h>
#include <stddef.h>
#include <malloc.h>

#include <sys/reent.h>
#include <sys/_locale.h>

_BEGIN_STD_C

void
qsort(void *aa, size_t n, size_t es, int (*cmp)(const void *, const void *));
int atoi(const char *nptr);
void *
bsearch(const void *key, const void *base0, size_t nmemb, size_t size,
    int (*compar)(const void *, const void *));

void abort(void) __noreturn;

int abs(int i);

/* The largest number returned by rand() */
#define	RAND_MAX	__INT_MAX__
int rand(void);

unsigned long _strtoul (const char *nptr, char **endptr, int base);
unsigned long strtoul (const char *s, char **ptr, int base);

int	__locale_mb_cur_max (void);

double	_strtod_r (struct _reent *,const char *__restrict __n,
		   char **__restrict __end_PTR);
double	strtod_l (const char *__restrict, char **__restrict, locale_t);
float	strtof_l (const char *__restrict, char **__restrict, locale_t);
double	strtod (const char *__restrict __n, char **__restrict __end_PTR);
long	strtol_l (const char *__restrict, char **__restrict, int, locale_t);
long	strtol (const char *__restrict __n, char **__restrict __end_PTR,
		 int __base);
long double strtold_l (const char *__restrict, char **__restrict, locale_t);
float	strtof (const char *__restrict __n, char **__restrict __end_PTR);
long	_strtol_r (struct _reent *,const char *__restrict __n,
		   char **__restrict __end_PTR, int __base);
long long strtoll (const char *__restrict __n, char **__restrict __end_PTR,
		   int __base);
long long _strtoll_r (struct _reent *, const char *__restrict __n,
		      char **__restrict __end_PTR, int __base);
long long strtoll_l (const char *__restrict, char **__restrict, int, locale_t);
unsigned long long _strtoull_r (struct _reent *, const char *__restrict __n,
				char **__restrict __end_PTR, int __base);
unsigned long long strtoull_l (const char *__restrict, char **__restrict, int,
			       locale_t __loc);
unsigned long long strtoull (const char *__restrict __n,
			     char **__restrict __end_PTR, int __base);
long double strtold (const char *__restrict, char **__restrict);
long double _strtold_r (struct _reent *, const char *__restrict,
			char **__restrict);

int	_mbtowc_r (struct _reent *, wchar_t *__restrict,
		   const char *__restrict, size_t, _mbstate_t *);
int	_wctomb_r (struct _reent *, char *, wchar_t, _mbstate_t *);

_END_STD_C

#endif /* STDLIB_H */
