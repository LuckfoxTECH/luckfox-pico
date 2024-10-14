/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    cpu_mem_test.c
  * @version V0.1
  * @brief   cpu and mem test
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-07-01     Tony.Xie        first implementation
  *
  ******************************************************************************
  */

#include <rthw.h>
#include <rtthread.h>
#include "hal_base.h"

#ifdef RT_USING_COMMON_TEST_PM
#if 0
#define PMTST_INFO(fmt, ...) rt_kprintf(fmt, ##__VA_ARGS__)
#else
#define PMTST_INFO(fmt, ...)
#endif

#if 0
#define PMTST_ERR(fmt, ...) rt_kprintf(fmt, ##__VA_ARGS__)
#else
#define PMTST_ERR(fmt, ...)
#endif

static const char pi_result[] = "3141592653589793238462643383279528841971693993751058209749445923078164062862089986280348253421170679821480865132823664709384469555822317253594081284811174502841270193852115559644622948954930381964428810975665933446128475648233786783165271201991456485669234634861045432664821339360726024914127372458706606315588174881520920962829254917153643678925903611330530548820466521384146951941511609433057273657595919530921861173819326117931051185480744623799627495673518857527248912279381830119491298336733624406566438602139494639522473719070217986943702770539217176293176752384674818467669451320005681271452635608277857713427577896091736371787214684409012249534301465495853710579227968925892354201995611212902196864344181598136297747713099605187072113499999983729780499510597317328160963185";

static __inline int calc_pi(int *pi_mem_result, char *pi_mem_pi_calc,
                            long *pi_mem_f)
{
    int bit = 0, i = 0;
    long a = 10000, b = 0, c = 2800, d = 0, e = 0, g = 0;
    int *result;
    long *f;
    int len = 0;
    char *pi_calc, *pi_tmp;
    char *pi_just = (char *)&pi_result[0];
    size_t pi_just_size = sizeof(pi_result);

    result = pi_mem_result;
    if (result == NULL)
        return -ENOMEM;

    f = pi_mem_f;
    if (f == NULL)
        return -ENOMEM;

    pi_calc = pi_mem_pi_calc;
    if (pi_calc == NULL)
        return -ENOMEM;

    for (; b - c;)
        f[b++] = a / 5;
    for (; d = 0, g = c * 2; c -= 14, result[bit++] = e + d / a, e = d % a)
        for (b = c; d += f[b] * a, f[b] = d % --g, d /= g--, --b; d *= b);

    pi_tmp = pi_calc;
    for (i = 0; i < bit; i++)
    {
        len += rt_snprintf(pi_tmp + len, 1000 - len, "%d", result[i]);
        /* PMTST_INFO("%c", pi_calc[i]); */
    }

#if 0
    PMTST_INFO("%d, %d\n", bit, pi_just_size);
    for (i = 0; i < pi_just_size; i++)
    {
        PMTST_INFO("%c", pi_calc[i]);
    }
    PMTST_INFO("**\n");

    for (i = 0; i < pi_just_size; i++)
    {
        PMTST_INFO("%c", pi_just[i]);
    }
    PMTST_INFO("**\n");
#endif

    if (strncmp(pi_just, pi_calc, pi_just_size) == 0)
    {
        PMTST_INFO("** %s ok\n", __func__);
        return 0;
    }
    else
    {
        PMTST_INFO("** %s error!\n", __func__);
        return -1;
    }
}

void test_cpu_func_calc_pi(int *mem_result, char *mem_pi_calc, long *pi_mem_f)
{
    int i;
    for (i = 0; i < 1; i++)
    {
        calc_pi(mem_result, mem_pi_calc, pi_mem_f);
        calc_pi(mem_result, mem_pi_calc, pi_mem_f);
        calc_pi(mem_result, mem_pi_calc, pi_mem_f);
        calc_pi(mem_result, mem_pi_calc, pi_mem_f);
        calc_pi(mem_result, mem_pi_calc, pi_mem_f);
    }
}

#define TEST_VALUE 0x55555555
#define UL_ONEBITS 0xffffffff
#define UL_BYTE(x) ((x | x << 8 | x << 16 | x << 24))
#define BUF_SIZE     (256 * 1024)
#define FT_FAILED -1
typedef unsigned long ul;
typedef unsigned long volatile ulv;

static int compare_regions(ulv *bufa, ulv *bufb, ul count)
{
    int r = 0;
    ul i;
    ulv *p1 = bufa;
    ulv *p2 = bufb;

    for (i = 0; i < count; i++, p1++, p2++)
    {
        if (*p1 != *p2)
        {
            r = -1;
            PMTST_ERR("rktest_mem_error (%p, %p): %lx, %lx\n", p1, p2, *p1, *p2);
            break;
        }
    }
    return r;
}

static int compare_regions_reverse(ulv *bufa, ulv *bufb, ul count)
{
    int r = 0;
    ul i;
    ulv *p1 = bufa;
    ulv *p2 = bufb;

    for (i = 0; i < count; i++, p1++, p2++)
    {
        if (*p1 != ~(*p2))
        {
            r = -1;
            PMTST_ERR("rktest_mem_error (%p, %p): %lx, %lx\n", p1, p2, *p1, ~(*p2));
            break;
        }
    }
    return r;
}

static int test_stuck_address(ulv *bufa, ul count)
{
    ulv *p1 = bufa;
    ul j;
    ul i;

    for (j = 0; j < 16; j++)
    {
        p1 = (ulv *) bufa;
        for (i = 0; i < count; i++)
        {
            *p1 = (((j + i) % 2) == 0 ? (ul) p1 : ~((ul) p1));
            *p1++;
        }
        p1 = (ulv *) bufa;
        for (i = 0; i < count; i++, p1++)
        {
            if (*p1 != (((j + i) % 2) == 0 ? (ul) p1 : ~((ul) p1)))
            {
                return -1;
            }
        }
    }
    return 0;
}

static int test_random_value(ulv *bufa, ulv *bufb, ul count)
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    ul i;

    for (i = 0; i < count; i++)
    {
        *p1++ = *p2++ = TEST_VALUE;
    }
    return compare_regions(bufa, bufb, count);
}

static int test_xor_comparison(ulv *bufa, ulv *bufb, ul count)
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    ul i;
    ul q = TEST_VALUE;

    for (i = 0; i < count; i++)
    {
        *p1++ ^= q;
        *p2++ ^= q;
    }
    return compare_regions(bufa, bufb, count);
}

static int test_sub_comparison(ulv *bufa, ulv *bufb, ul count)
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    ul i;
    ul q = TEST_VALUE;

    for (i = 0; i < count; i++)
    {
        *p1++ -= q;
        *p2++ -= q;
    }
    return compare_regions(bufa, bufb, count);
}

static int test_mul_comparison(ulv *bufa, ulv *bufb, ul count)
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    ul i;
    ul q = TEST_VALUE;

    for (i = 0; i < count; i++)
    {
        *p1++ *= q;
        *p2++ *= q;
    }
    return compare_regions(bufa, bufb, count);
}

static int test_div_comparison(ulv *bufa, ulv *bufb, ul count)
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    ul i;
    ul q = TEST_VALUE;

    for (i = 0; i < count; i++)
    {
        if (!q)
        {
            q++;
        }
        *p1++ /= q;
        *p2++ /= q;
    }
    return compare_regions(bufa, bufb, count);
}

static int test_or_comparison(ulv *bufa, ulv *bufb, ul count)
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    ul i;
    ul q = TEST_VALUE;

    for (i = 0; i < count; i++)
    {
        *p1++ |= q;
        *p2++ |= q;
    }
    return compare_regions(bufa, bufb, count);
}

static int test_and_comparison(ulv *bufa, ulv *bufb, ul count)
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    ul i;
    ul q = TEST_VALUE;

    for (i = 0; i < count; i++)
    {
        *p1++ &= q;
        *p2++ &= q;
    }
    return compare_regions(bufa, bufb, count);
}

static int test_seqinc_comparison(ulv *bufa, ulv *bufb, ul count)
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    ul i;
    ul q = TEST_VALUE;
    ul value;

    for (i = 0; i < count; i++)
    {
        value = (i + q);
        *p1++ = value;
        *p2++ = ~value;
    }
    return compare_regions_reverse(bufa, bufb, count);
}

static int test_solidbits_comparison(ulv *bufa, ulv *bufb, ul count)
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    ul j;
    ul q;
    ul i;
    ul value;

    for (j = 0; j < 64; j++)
    {
        q = ((j % 2) == 0 ? UL_ONEBITS : 0);
        p1 = (ulv *) bufa;
        p2 = (ulv *) bufb;
        for (i = 0; i < count; i++)
        {
            value = ((i % 2) == 0 ? q : ~q);
            *p1++ = value;
            *p2++ = ~value;
        }
        if (compare_regions_reverse(bufa, bufb, count))
        {
            return -1;
        }
    }
    return 0;
}

static int test_blockseq_comparison(ulv *bufa, ulv *bufb, ul count)
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    ul j;
    ul i;
    ul value;

    for (j = 0; j < 256; j++)
    {
        p1 = (ulv *) bufa;
        p2 = (ulv *) bufb;
        for (i = 0; i < count; i++)
        {
            value = (ul) UL_BYTE(j);
            *p1++ = value;
            *p2++ = ~value;
        }
        if (compare_regions_reverse(bufa, bufb, count))
        {
            return -1;
        }
    }
    return 0;
}
static int test_mem(void *aligned, unsigned long b_size)
{
    ulv *bufa, *bufb;
    ul halflen, count;

    halflen = b_size / 2;
    count = halflen / sizeof(ul);
    bufa = (ulv *)aligned;
    bufb = (ulv *)((ul)aligned + halflen);
    if (test_stuck_address(aligned, (b_size) / sizeof(ul)))
    {
        PMTST_INFO("test_stuck_address\n");
        return FT_FAILED;
    }

    if (test_random_value(bufa, bufb, count))
    {
        PMTST_INFO("test_random_value_ error\n");
        return FT_FAILED;
    }

    if (test_xor_comparison(bufa, bufb, count))
    {
        PMTST_INFO("test_xor_comparison_ error\n");
        return FT_FAILED;
    }

    if (test_sub_comparison(bufa, bufb, count))
    {
        PMTST_INFO("test_sub_comparison_ error\n");
        return FT_FAILED;
    }

    if (test_mul_comparison(bufa, bufb, count))
    {
        PMTST_INFO("test_mul_comparison_ error\n");
        return FT_FAILED;
    }

    if (test_div_comparison(bufa, bufb, count))
    {
        PMTST_INFO("test_div_comparison_ error\n");
        return FT_FAILED;
    }

    if (test_or_comparison(bufa, bufb, count))
    {
        PMTST_INFO("test_or_comparison_ error\n");
        return FT_FAILED;
    }

    if (test_and_comparison(bufa, bufb, count))
    {
        PMTST_INFO("test_and_comparison_ error\n");
        return FT_FAILED;
    }

    if (test_seqinc_comparison(bufa, bufb, count))
    {
        PMTST_INFO("test_seqinc_comparison_ error\n");
        return FT_FAILED;
    }

    if (test_solidbits_comparison(bufa, bufb, count))
    {
        PMTST_INFO("test_solidbits_comparison_ error\n");
        return FT_FAILED;
    }

    if (test_blockseq_comparison(bufa, bufb, count))
    {
        PMTST_INFO("test_blockseq_comparison_ error\n");
        return FT_FAILED;
    }

    return 0;
}

void rk_test_mem(void *addr, unsigned long b_size)
{
    test_mem(addr, b_size);
}
#endif
