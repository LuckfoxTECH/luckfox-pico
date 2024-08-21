/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2023 Rockchip Electronics Co., Ltd.
 */

#include "hal_bsp.h"
#include "hal_base.h"
#include "task_ipc.h"
#include "test_conf.h"
#include "unity_runner.h"
#include <stdlib.h>

/********************* Private MACRO Definition ******************************/
//#define IRQ_LATENCY_TEST
//#define PERF_TEST
//#define SOFTIRQ_TEST
//#define TIMER_TEST
//#define UNITY_TEST

/********************* Private Structure Definition **************************/

static struct GIC_AMP_IRQ_INIT_CFG irqsConfig[] = {
/* TODO: Config the irqs here.
 * GIC version: GICv2
 * The priority higher than 0x80 is non-secure interrupt.
 */
    GIC_AMP_IRQ_CFG_ROUTE(UART0_IRQn, 0xd0, CPU_GET_AFFINITY(1, 0)),

#ifdef IRQ_LATENCY_TEST
    GIC_AMP_IRQ_CFG_ROUTE(RSVD0_IRQn, 0xd0, CPU_GET_AFFINITY(0, 0)),
#endif

#ifdef SOFTIRQ_TEST
    GIC_AMP_IRQ_CFG_ROUTE(RSVD0_IRQn, 0xd0, CPU_GET_AFFINITY(0, 0)),
#endif

#ifdef TIMER_TEST
    GIC_AMP_IRQ_CFG_ROUTE(TIMER4_IRQn, 0xd0, CPU_GET_AFFINITY(0, 0)),
#endif

    GIC_AMP_IRQ_CFG_ROUTE(0, 0, CPU_GET_AFFINITY(1, 0)),   /* sentinel */
};

static struct GIC_IRQ_AMP_CTRL irqConfig = {
    .cpuAff = CPU_GET_AFFINITY(1, 0),
    .defPrio = 0xd0,
    .defRouteAff = CPU_GET_AFFINITY(1, 0),
    .irqsCfg = &irqsConfig[0],
};

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
//    printf("irq_rsvd: latency = %.2f us\n", time_one);
}

static void irq_latency_test(void)
{
    int i, j;

    printf("irq_latency_test start:\n");

    printf("irq_rsvd:\n");
    HAL_IRQ_HANDLER_SetIRQHandler(RSVD0_IRQn, irq_rsvd_isr, NULL);
    HAL_GIC_Enable(RSVD0_IRQn);
    HAL_DelayMs(2000);

    for (i = 0; i < IRQ_LATENCY_TEST_NUM; i++) {
        time_sum = 0;
        time_max = 0;
        time_min = 1000;
        for (j = 0; j < IRQ_LATENCY_TEST_LOOP; j++) {
            time_start = HAL_GetSysTimerCount();
            HAL_GIC_SetPending(RSVD0_IRQn);
            HAL_DelayUs(IRQ_LATENCY_TEST_DELAY);
        }
        printf("irq_rsvd latency: avg = %.2f us, max = %.2f us, min = %.2f us\n",
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
    uint32_t cpu_id, loop = 1000, size = 4 * 1024 * 1024;
    uint32_t *ptr;
    uint64_t start, end;
    double time_s;

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    printf("perftest: cpu-%ld\n", cpu_id);

    benchmark_main();

    printf("test memset start!\n");
    ptr = (uint32_t *)malloc(size);
    if (ptr) {
        start = HAL_GetSysTimerCount();
        for (int i = 0; i < loop; i++) {
            memset(ptr, i, size);
        }
        end = HAL_GetSysTimerCount();
        time_s = ((end - start) * 1.0) / PLL_INPUT_OSC_RATE;
        printf("\nmemset bw=%.2fMB/s, time_s=%.2f\n", (size * loop) / time_s / 1000000, time_s);

        /* prevent optimization */
        for (int i = 0; i < size / sizeof(uint32_t); i++) {
            g_sum += ptr[i];
        }
        printf("sum=%d\n", g_sum);
        free(ptr);
    }
    printf("test memset end!\n");
}
#endif

/************************************************/
/*                                              */
/*                SOFTIRQ_TEST                  */
/*                                              */
/************************************************/
#ifdef SOFTIRQ_TEST
static void soft_isr(int vector, void *param)
{
    printf("soft_isr, vector = %d\n", vector);
    HAL_GIC_EndOfInterrupt(vector);
}

static void softirq_test(void)
{
    HAL_IRQ_HANDLER_SetIRQHandler(RSVD0_IRQn, soft_isr, NULL);
    HAL_GIC_Enable(RSVD0_IRQn);

    HAL_GIC_SetPending(RSVD0_IRQn);
}
#endif

/************************************************/
/*                                              */
/*                  TIMER_TEST                  */
/*                                              */
/************************************************/
#ifdef TIMER_TEST
static int timer_int_count = 0;
static uint32_t latency_sum = 0;
static uint32_t latency_max = 0;
struct TIMER_REG *test_timer = TIMER4;
static bool desc_timer = true;

static void timer_isr(uint32_t irq, void *args)
{
    uint32_t count;
    uint32_t latency;

    count = (uint32_t)HAL_TIMER_GetCount(test_timer);
    if (desc_timer) {
        count = 24000000 - count;
    }
    /* 24M timer: 41.67ns per count */
    latency = count * 41;
    printf("timer_test: latency=%ldns(count=%ld)\n", latency, count);
    timer_int_count++;
    latency_sum += latency;
    latency_max = latency_max > latency ? latency_max : latency;
    if (timer_int_count == 100) {
        printf("timer_test: latency avg=%dns,max=%dns\n", latency_sum / timer_int_count, latency_max);
        timer_int_count = 0;
        latency_sum = 0;
        latency_max = 0;
        HAL_TIMER_ClrInt(test_timer);
        HAL_TIMER_Stop_IT(test_timer);
    }

    HAL_TIMER_ClrInt(test_timer);
}

static void timer_test(void)
{
    uint64_t start, end;
    uint32_t count;

    printf("timer_test start\n");
    start = HAL_GetSysTimerCount();
    HAL_DelayUs(1000000);
    end = HAL_GetSysTimerCount();
    /* sys_timer: TIMER5 is a increment count TIMER */
    count = (uint32_t)(end - start);
    printf("sys_timer 1s count: %ld(%lld, %lld)\n", count, start, end);

    HAL_TIMER_Init(test_timer, TIMER_FREE_RUNNING);
    HAL_TIMER_SetCount(test_timer, 2000000000);
    HAL_TIMER_Start(test_timer);
    start = HAL_TIMER_GetCount(test_timer);
    HAL_DelayUs(1000000);
    end = HAL_TIMER_GetCount(test_timer);
    /* Pay attention to the timer type */
    /* test_timer: TIMER4 is a decrement count TIMER */
    desc_timer = true;
    count = (uint32_t)(start - end);
    printf("test_timer 1s count: %ld(%lld, %lld)\n", count, start, end);
    HAL_TIMER_Stop(test_timer);

    HAL_IRQ_HANDLER_SetIRQHandler(TIMER4_IRQn, timer_isr, NULL);
    HAL_GIC_Enable(TIMER4_IRQn);
    printf("timer_test: 1s interrupt start\n");
    HAL_TIMER_Init(test_timer, TIMER_FREE_RUNNING);
    HAL_TIMER_SetCount(test_timer, 24000000);
    HAL_TIMER_Start_IT(test_timer);
}
#endif

/********************* Public Function Definition ****************************/

void TEST_DEMO_GIC_Init(void)
{
    HAL_GIC_Init(&irqConfig);
}

void test_demo(void)
{
#if defined(IRQ_LATENCY_TEST) && defined(CPU0)
    irq_latency_test();
#endif

#if defined(PERF_TEST) && defined(CPU0)
    perf_test();
#endif

#if defined(SOFTIRQ_TEST) && defined(CPU0)
    softirq_test();
#endif

#if defined(TIMER_TEST) && defined(CPU0)
    timer_test();
#endif

#if defined(UNITY_TEST) && defined(CPU0)
    /* Unity Test */
    test_main();
#endif
}
