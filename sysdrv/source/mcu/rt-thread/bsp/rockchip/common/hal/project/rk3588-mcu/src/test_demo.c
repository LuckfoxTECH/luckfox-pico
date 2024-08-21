/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2023 Rockchip Electronics Co., Ltd.
 */

#include "hal_bsp.h"
#include "hal_base.h"
#include "unity_runner.h"
#include <stdlib.h>

/********************* Private MACRO Definition ******************************/
//#define GPIO_TEST
//#define GPIO_DIRECT_TEST
//#define GPIO_VIRTUAL_MODEL_TEST
//#define IRQ_LATENCY_TEST
//#define PERF_TEST
//#define SOFTIRQ_TEST
//#define TIMER_TEST

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
/*                  GPIO_TEST                   */
/*                                              */
/************************************************/
#ifdef GPIO_TEST
static void gpio3_isr(uint32_t irq, void *args)
{
    HAL_GPIO_IRQHandler(GPIO3, GPIO_BANK3);
}

static HAL_Status c0_call_back(eGPIO_bankId bank, uint32_t pin, void *args)
{
    printf("GPIO3C0 callback!\n");

    return HAL_OK;
}

static void gpio_test(void)
{
    uint32_t level;

    /* set pinctrl function */
    HAL_PINCTRL_SetParam(GPIO_BANK3,
                         GPIO_PIN_C0,
                         PIN_CONFIG_MUX_FUNC0);
    /* Test pinctrl pull */
    printf("test_gpio pull UP\n");
    HAL_PINCTRL_SetParam(GPIO_BANK3,
                         GPIO_PIN_C0,
                         PIN_CONFIG_PUL_UP);
    printf("GPIO3C_P: %p = 0x%lx\n", &VCCIO3_5_IOC->GPIO3C_P, VCCIO3_5_IOC->GPIO3C_P);
    HAL_DelayMs(3000);
    printf("test_gpio pull DOWN\n");
    HAL_PINCTRL_SetParam(GPIO_BANK3,
                         GPIO_PIN_C0,
                         PIN_CONFIG_PUL_DOWN);
    HAL_DelayMs(3000);
    printf("GPIO3C_P: %p = 0x%lx\n", &VCCIO3_5_IOC->GPIO3C_P, VCCIO3_5_IOC->GPIO3C_P);

    /* Test GPIO output */
    HAL_GPIO_SetPinDirection(GPIO3, GPIO_PIN_C0, GPIO_OUT);
    level = HAL_GPIO_GetPinLevel(GPIO3, GPIO_PIN_C0);
    printf("test_gpio 3c0 level = %ld\n", level);
    HAL_DelayMs(3000);
    HAL_GPIO_SetPinLevel(GPIO3, GPIO_PIN_C0, GPIO_HIGH);
    level = HAL_GPIO_GetPinLevel(GPIO3, GPIO_PIN_C0);
    printf("test_gpio 3c0 output high level = %ld\n", level);
    HAL_DelayMs(3000);
    HAL_GPIO_SetPinLevel(GPIO3, GPIO_PIN_C0, GPIO_LOW);
    level = HAL_GPIO_GetPinLevel(GPIO3, GPIO_PIN_C0);
    printf("test_gpio 3c0 output low level = %ld\n", level);
    HAL_DelayMs(3000);

    /* Test GPIO interrupt */
    HAL_GPIO_SetPinDirection(GPIO3, GPIO_PIN_C0, GPIO_IN);
    HAL_INTMUX_SetIRQHandler(GPIO3_IRQn, gpio3_isr, NULL);
    HAL_IRQ_HANDLER_SetGpioIRQHandler(GPIO_BANK3, GPIO_PIN_C0, c0_call_back, NULL);
    HAL_INTMUX_EnableIRQ(GPIO3_IRQn);
    HAL_GPIO_SetIntType(GPIO3, GPIO_PIN_C0, GPIO_INT_TYPE_EDGE_BOTH);
    HAL_GPIO_EnableIRQ(GPIO3, GPIO_PIN_C0);
    printf("test_gpio interrupt ready\n");
}
#endif

/************************************************/
/*                                              */
/*              GPIO_DIRECT_TEST                */
/*                                              */
/************************************************/
#ifdef GPIO_DIRECT_TEST
static void gpio0_isr(uint32_t irq, void *args)
{
    HAL_GPIO_IRQHandler(GPIO0, GPIO_BANK0);
}

static HAL_Status a7_call_back(eGPIO_bankId bank, uint32_t pin, void *args)
{
    printf("GPIO0A7 callback!\n");

    return HAL_OK;
}

static void gpio_direct_test(void)
{
    /* set pinctrl function */
    HAL_PINCTRL_SetParam(GPIO_BANK0,
                         GPIO_PIN_A7,
                         PIN_CONFIG_MUX_FUNC0);

    /* Test GPIO interrupt */
    HAL_GPIO_SetPinDirection(GPIO0, GPIO_PIN_A7, GPIO_IN);
    HAL_NVIC_SetIRQHandler(GPIO0_IRQn, gpio0_isr);
    HAL_IRQ_HANDLER_SetGpioIRQHandler(GPIO_BANK0, GPIO_PIN_A7, a7_call_back, NULL);
    HAL_NVIC_EnableIRQ(GPIO0_IRQn);
    HAL_GPIO_SetIntType(GPIO0, GPIO_PIN_A7, GPIO_INT_TYPE_EDGE_BOTH);
    HAL_GPIO_EnableIRQ(GPIO0, GPIO_PIN_A7);
    printf("test_gpio interrupt ready\n");
}
#endif

/************************************************/
/*                                              */
/*          GPIO_VIRTUAL_MODEL_TEST             */
/*                                              */
/************************************************/
#ifdef GPIO_VIRTUAL_MODEL_TEST
static void gpio3_exp_isr(uint32_t irq, void *args)
{
    HAL_GPIO_IRQHandler(GPIO3_EXP, GPIO_BANK3_EXP);
}

static HAL_Status c0_call_back(eGPIO_bankId bank, uint32_t pin, void *args)
{
    printf("GPIO3C0 exp callback!\n");

    return HAL_OK;
}

static void gpio_virtual_model_test(void)
{
    uint32_t level;

    /* set pinctrl function */
    HAL_PINCTRL_SetParam(GPIO_BANK3,
                         GPIO_PIN_C0,
                         PIN_CONFIG_MUX_FUNC0);

    /* set pin group configuration */
    HAL_GPIO_EnableVirtualModel(GPIO3);
    HAL_GPIO_SetVirtualModel(GPIO3,
                             GPIO_PIN_A0 | GPIO_PIN_A1 | GPIO_PIN_A2 | GPIO_PIN_A3 |
                             GPIO_PIN_A4 | GPIO_PIN_A5 | GPIO_PIN_A6 | GPIO_PIN_A7 |
                             GPIO_PIN_B0 | GPIO_PIN_B1 | GPIO_PIN_B2 | GPIO_PIN_B3 |
                             GPIO_PIN_B4 | GPIO_PIN_B5 | GPIO_PIN_B6 | GPIO_PIN_B7 |
                             GPIO_PIN_C1 | GPIO_PIN_C2 | GPIO_PIN_C3 |
                             GPIO_PIN_C4 | GPIO_PIN_C5 | GPIO_PIN_C6 | GPIO_PIN_C7 |
                             GPIO_PIN_D0 | GPIO_PIN_D1 | GPIO_PIN_D2 | GPIO_PIN_D3 |
                             GPIO_PIN_D4 | GPIO_PIN_D5 | GPIO_PIN_D6 | GPIO_PIN_D7,
                             GPIO_VIRTUAL_MODEL_OS_A);
    HAL_GPIO_SetVirtualModel(GPIO3,
                             GPIO_PIN_C0,
                             GPIO_VIRTUAL_MODEL_OS_B);

    /* Test GPIO output */
    HAL_GPIO_SetPinDirection(GPIO3_EXP, GPIO_PIN_C0, GPIO_OUT);
    level = HAL_GPIO_GetPinLevel(GPIO3_EXP, GPIO_PIN_C0);
    printf("test gpio 3c0 level = %ld\n", level);
    HAL_DelayMs(3000);
    HAL_GPIO_SetPinLevel(GPIO3_EXP, GPIO_PIN_C0, GPIO_HIGH);
    level = HAL_GPIO_GetPinLevel(GPIO3_EXP, GPIO_PIN_C0);
    printf("test_gpio 3c0 output high level = %ld\n", level);
    HAL_DelayMs(3000);
    HAL_GPIO_SetPinLevel(GPIO3_EXP, GPIO_PIN_C0, GPIO_LOW);
    level = HAL_GPIO_GetPinLevel(GPIO3_EXP, GPIO_PIN_C0);
    printf("test_gpio 3c0 output low level = %ld\n", level);
    HAL_DelayMs(3000);

    /* Test GPIO interrupt */
    HAL_GPIO_SetPinDirection(GPIO3_EXP, GPIO_PIN_C0, GPIO_IN);
    HAL_INTMUX_SetIRQHandler(GPIO3_EXP_IRQn, gpio3_exp_isr, NULL);
    HAL_IRQ_HANDLER_SetGpioIRQHandler(GPIO_BANK3_EXP, GPIO_PIN_C0, c0_call_back, NULL);
    HAL_INTMUX_EnableIRQ(GPIO3_EXP_IRQn);
    HAL_GPIO_SetIntType(GPIO3_EXP, GPIO_PIN_C0, GPIO_INT_TYPE_EDGE_BOTH);
    HAL_GPIO_EnableIRQ(GPIO3_EXP, GPIO_PIN_C0);
    printf("test gpio exp interrupt ready\n");
}
#endif

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
/*                  TIMER_TEST                  */
/*                                              */
/************************************************/
#ifdef TIMER_TEST
static int timer_int_count = 0;
static uint32_t latency_sum = 0;
static uint32_t latency_max = 0;
struct TIMER_REG *test_timer = TIMER10;
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
    /* test_timer: TIMER10 is a decrement count TIMER */
    desc_timer = true;
    count = (uint32_t)(start - end);
    printf("test_timer 1s count: %ld(%lld, %lld)\n", count, start, end);
    HAL_TIMER_Stop(test_timer);

    HAL_INTMUX_SetIRQHandler(TIMER10_IRQn, timer_isr, NULL);
    HAL_INTMUX_EnableIRQ(TIMER10_IRQn);
    printf("timer_test: 1s interrupt start\n");
    HAL_TIMER_Init(test_timer, TIMER_FREE_RUNNING);
    HAL_TIMER_SetCount(test_timer, 24000000);
    HAL_TIMER_Start_IT(test_timer);
}
#endif

void test_demo(void)
{
#ifdef GPIO_TEST
    gpio_test();
#endif

#ifdef GPIO_DIRECT_TEST
    gpio_direct_test();
#endif

#ifdef GPIO_VIRTUAL_MODEL_TEST
    gpio_virtual_model_test();
#endif

#ifdef IRQ_LATENCY_TEST
    irq_latency_test();
#endif

#ifdef PERF_TEST
    perf_test();
#endif

#ifdef SOFTIRQ_TEST
    softirq_test();
#endif

#ifdef TIMER_TEST
    timer_test();
#endif
}
