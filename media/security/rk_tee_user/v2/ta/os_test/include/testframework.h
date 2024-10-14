/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 */

#ifndef GUARD_testframework_H
#define GUARD_testframework_H

#include "tee_internal_api.h"

#include "tee_arith_internal.h"

#include "tb_macros.h"
#include "tb_asserts.h"

/* define the max size of generated numbers */
/* this is number of hex chars in the number */
#define MAX_RAND_DIGITS         256
#define MAX_RAND_STRING_SIZE    (MAX_RAND_DIGITS + 2)

size_t my_strlen(const char *string);
int my_strcmp(const char *s1, const char *s2);
size_t my_strlcpy(char *dst, const char *src, size_t siz);

void tb_set_random_value(TEE_BigInt *a, char *str, int allow_neg);
void tb_get_random_str(char *str, int allow_neg);

void tb_main(void);

void tb_var(void);
void tb_conv(void);
void tb_cmp(void);
void tb_addsub(void);
void tb_mul(void);
void tb_shift(void);
void tb_div(void);
void tb_gcd(void);
void tb_modulus(void);
void tb_fmm(void);
void tb_prime(void);

int TEE_BigIntConvertFromString(TEE_BigInt *dest, const char *src);
char *TEE_BigIntConvertToString(char *dest, int mode, const TEE_BigInt *src);

#endif /* include guard */
