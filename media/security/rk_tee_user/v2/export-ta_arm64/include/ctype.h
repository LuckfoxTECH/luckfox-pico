/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2018, Linaro Limited
 */
#ifndef __CTYPE_H
#define __CTYPE_H

#include <sys/_locale.h>

int isalnum(int c);
int isalpha(int c);
int iscntrl(int c);
int isdigit(int c);
int isgraph(int c);
int islower(int c);
int isprint(int c);
int ispunct(int c);
int isspace(int c);
int isupper(int c);
int isxdigit(int c);
int tolower(int c);
int toupper(int c);
int isblank (int c);

const char *__locale_ctype_ptr (void);
const char *__locale_ctype_ptr_l (locale_t);
int isspace_l (int c, struct __locale_t *locale);

#define _U	01
#define _L	02
#define _N	04
#define _S	010
#define _P	020
#define _C	040
#define _X	0100
#define _B	0200

extern const char	_ctype_[];

#endif /*__CTYPE_H*/
