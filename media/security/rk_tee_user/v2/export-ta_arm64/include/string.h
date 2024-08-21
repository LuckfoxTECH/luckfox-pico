/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 */

/*
 * This file provides what C99 standard requires for <string.h>
 * for some functions
 */

#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <sys/cdefs.h>

#include <sys/_locale.h>
#include <sys/reent.h>

_BEGIN_STD_C

void *memcpy(void *__restrict s1, const void *__restrict s2, size_t n);
void *memmove(void *s1, const void *s2, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
void *memset(void *s, int c, size_t n);

int strcmp(const char *s1, const char *s2);
int strcoll (const char *a, const char *b);
int strncmp(const char *s1, const char *s2, size_t n);
size_t strlen(const char *s);
size_t strnlen(const char *s, size_t n);
char *strdup(const char *s);
char *strndup(const char *s, size_t n);
char *strchr(const char *s, int c);
char *strstr(const char *big, const char *little);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);
char *strrchr(const char *s, int i);
char *strcat(char *dest, const char *src);
char *strncat (char *s1, const char *s2, size_t n);

void *memchr(const void *buf, int c, size_t length);
size_t strxfrm(char *s1, const char *s2, size_t n);
size_t strspn(const char *s1, const char *s2);
size_t strcspn(const char *s1, const char *s2);
char *strpbrk(const char *s1, const char *s2);
char *strtok_r(char *__restrict, const char *__restrict, char **__restrict);
char *strtok (char *__restrict, const char *__restrict);
char *strerror (int);

_END_STD_C

#endif /* STRING_H */
