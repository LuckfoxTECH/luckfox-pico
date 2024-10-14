/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#include "hal_bsp.h"
#include "hal_base.h"
#include "unity_runner.h"
#include <stdlib.h>

/********************* Private MACRO Definition ******************************/
//#define IRQ_LATENCY_TEST
//#define PERF_TEST
//#define SOFTIRQ_TEST
//#define WDT_TEST

/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

/************************************************/
/*                                              */
/*                 HW Borad config              */
/*                                              */
/************************************************/

/* TODO: Set Module IOMUX Function Here */

/************************************************/
/*                                              */
/*             IRQ_LATENCY_TEST                 */
/*                                              */
/************************************************/
#ifdef IRQ_LATENCY_TEST
#define IRQ_LATENCY_TEST_NUM   10
#define IRQ_LATENCY_TEST_LOOP  10000
#define IRQ_LATENCY_TEST_DELAY 500

static uint64_t time_start, time_end;
static double time_one, time_sum, time_max, time_min;

static void irq_rsvd_isr(void)
{
    time_end = HAL_GetSysTimerCount();
    time_one = ((time_end - time_start) * 1000000.0) / PLL_INPUT_OSC_RATE;
    time_sum += time_one;
    if (time_one > time_max) {
        time_max = time_one;
    }
    if (time_one < time_min) {
        time_min = time_one;
    }
//    printf("irq_rsvd: latency = %.2f\n", time_one);
}

static void irq_latency_test(void)
{
    int i, j;

    printf("irq_latency_test start:\n");

    printf("irq_rsvd:\n");
    HAL_NVIC_SetIRQHandler(RSVD0_MCU_IRQn, irq_rsvd_isr);
    HAL_NVIC_EnableIRQ(RSVD0_MCU_IRQn);
    HAL_DelayMs(2000);

    for (i = 0; i < IRQ_LATENCY_TEST_NUM; i++) {
        time_sum = 0;
        time_max = 0;
        time_min = 1000;
        for (j = 0; j < IRQ_LATENCY_TEST_LOOP; j++) {
            time_start = HAL_GetSysTimerCount();
            HAL_NVIC_SetPendingIRQ(RSVD0_MCU_IRQn);
            HAL_DelayUs(IRQ_LATENCY_TEST_DELAY);
        }
        printf("irq_rsvd latency: avg = %.2f, max = %.2f, min = %.2f\n",
               time_sum / IRQ_LATENCY_TEST_LOOP, time_max, time_min);
    }

    printf("irq_latency_test end.\n");
}
#endif

/************************************************/
/*                                              */
/*                  PERF_TEST                   */
/*                                              */
/************************************************/
#ifdef PERF_TEST
#include "benchmark.h"

uint32_t g_sum = 0;

static void perf_test(void)
{
    uint32_t loop = 1000, size = 64 * 1024;
    uint32_t *ptr;
    uint32_t time_start, time_end, time_ms;

    printf("Perftest Start:\n");
    benchmark_main();

    printf("memset test start:\n");
    ptr = (uint32_t *)malloc(size);
    if (ptr) {
        time_start = HAL_GetTick();
        for (int i = 0; i < loop; i++) {
            memset(ptr, i, size);
        }
        time_end = HAL_GetTick();
        time_ms = time_end - time_start;
        printf("memset bw=%ldKB/s, time_ms=%d\n",
               1000 * (size * loop / 1024) / time_ms, time_ms);

        /* prevent optimization */
        for (int i = 0; i < size / sizeof(uint32_t); i++) {
            g_sum += ptr[i];
        }
        printf("sum=%d\n", g_sum);
        free(ptr);
    }
    printf("memset test end\n");

    printf("Perftest End:\n");
}
#endif

/************************************************/
/*                                              */
/*                SOFTIRQ_TEST                  */
/*                                              */
/************************************************/
#ifdef SOFTIRQ_TEST
static void soft_isr(void)
{
    printf("softirq_test: enter isr\n");
}

static void softirq_test(void)
{
    printf("softirq_test start\n");
    HAL_NVIC_SetIRQHandler(RSVD0_MCU_IRQn, soft_isr);
    HAL_NVIC_EnableIRQ(RSVD0_MCU_IRQn);

    HAL_DelayMs(4000);
    HAL_NVIC_SetPendingIRQ(RSVD0_MCU_IRQn);
}
#endif

/************************************************/
/*                                              */
/*                  WDT_TEST                    */
/*                                              */
/************************************************/
#ifdef WDT_TEST

#define WDT_TEST_FREQ        PLL_INPUT_OSC_RATE
#define WDT_TEST_CLEAR_COUNT 3

static struct WDT_REG *pWdt = WDT;

static int wdt_int_count = 0;

static void wdt_isr(void)
{
    if (wdt_int_count < WDT_TEST_CLEAR_COUNT) {
        printf("wdt_test: isr eoi\n");
        HAL_WDT_ClearInterrupt();
    }
    wdt_int_count++;
}

static void wdt_test(void)
{
    int wdt_timeout = 4;
    uint32_t wdt_left_start, wdt_left_end;

    /* wdt globle reset */
    /* *(uint32_t*)0xff370004 = 0x10001000; */
    /* wdt mcu reset and counter reset */
    *(uint32_t *)0xff370004 = 0x20002000;
    pWdt->CRR = 0x76;

    printf("wdt_test start:\n");
    HAL_NVIC_SetIRQHandler(PMUWDT_IRQn, wdt_isr);
    HAL_NVIC_EnableIRQ(PMUWDT_IRQn);

    HAL_WDT_Init(WDT_TEST_FREQ, pWdt);
    HAL_WDT_SetTimeout(wdt_timeout);
    printf("wdt_test: timeout set-%ds, get-%ds, TORR-0x%lx\n",
           wdt_timeout, HAL_WDT_GetTimeout(), pWdt->TORR);
    HAL_WDT_Start(INDIRECT_SYSTEM_RESET);
    wdt_left_start = HAL_WDT_GetTimeLeft();
    HAL_DelayMs(1000);
    wdt_left_end = HAL_WDT_GetTimeLeft();
    printf("wdt_test: 1s(delay) = %ldus(wdt)\n",
           (wdt_left_start - wdt_left_end) / (WDT_TEST_FREQ / 1000000));
}
#endif

void test_demo(void)
{
#ifdef IRQ_LATENCY_TEST
    irq_latency_test();
#endif

#ifdef PERF_TEST
    perf_test();
#endif

#ifdef SOFTIRQ_TEST
    softirq_test();
#endif

#ifdef WDT_TEST
    wdt_test();
#endif
}
