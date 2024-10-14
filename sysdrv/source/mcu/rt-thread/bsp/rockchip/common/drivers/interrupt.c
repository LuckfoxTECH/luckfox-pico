/**
  * Copyright (c) 2018 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    interrupt.c
  * @version V0.1
  * @brief   interrupt interface for rt-thread
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-05-08     Cliff.Chen      first implementation
  *
  ******************************************************************************
  */

#include <rthw.h>
#include "hal_base.h"

#if (defined(ARCH_ARM_CORTEX_M0) || defined(ARCH_ARM_CORTEX_M3) || defined(ARCH_ARM_CORTEX_M4) || defined(ARCH_ARM_CORTEX_M7)) && !defined(HAS_CUSTOME_INTC)

#if EXT_INTERRUPT

struct rk_intc
{
    void *intc_base;
    rt_isr_handler_t irq_handler;
};

static rt_isr_handler_t ext_vector[NUM_EXT_INTERRUPTS];

static void intc_irq_dispatch(void *intc, uint32_t offset)
{
    uint32_t i, irq_no;

    for (i = 0; i < NUM_EXT_INTERRUPTS; i++)
    {
        if (HAL_INTC_GetFinalStatus(intc, i))
        {
            irq_no = i + offset * NUM_INT_PER_CONTROLLER;
            ext_vector[irq_no](NUM_INTERRUPTS + irq_no, NULL);
        }
    }
}

#define DEFINE_RK_INTC_IRQ(ID)                                 \
static void rk_intc##ID##_irq_dispatch(int irq, void *param);  \
static struct rk_intc rk_intc##ID =                            \
{                                                              \
    .intc_base = INTC##ID,                                     \
    .irq_handler = rk_intc##ID##_irq_dispatch,                 \
};                                                             \
static void rk_intc##ID##_irq_dispatch(int irq, void *param)   \
{                                                              \
    intc_irq_dispatch(INTC##ID, ID);                               \
}

#ifdef INTC0
DEFINE_RK_INTC_IRQ(0);
#endif
#ifdef INTC1
DEFINE_RK_INTC_IRQ(1);
#endif
#ifdef INTC2
DEFINE_RK_INTC_IRQ(2);
#endif
#ifdef INTC3
DEFINE_RK_INTC_IRQ(3);
#endif

static struct rk_intc *rk_intc_table[] =
{
#ifdef INTC0
    &rk_intc0,
#endif
#ifdef INTC1
    &rk_intc1,
#endif
#ifdef INTC2
    &rk_intc2,
#endif
#ifdef INTC3
    &rk_intc3,
#endif
};

static void rk_intc_init(void)
{
    uint32_t i;

    memset(ext_vector, 0, sizeof(ext_vector));
    for (i = 0; i < HAL_ARRAY_SIZE(rk_intc_table); i++)
    {
        HAL_NVIC_SetIRQHandler(INTC0_IRQn + i, (NVIC_IRQHandler)rk_intc_table[i]->irq_handler);
        HAL_NVIC_EnableIRQ(INTC0_IRQn + i);
        HAL_INTC_EnableAllRQ(rk_intc_table[i]->intc_base);
    }
}

static void rk_intc_mask(uint32_t vector)
{
    uint32_t intc, irq;

    if (vector >= TOTAL_INTERRUPTS)
        return;

    if (vector < NUM_INTERRUPTS)
        HAL_NVIC_DisableIRQ(vector);
    else
    {
        intc = (vector - NUM_INTERRUPTS) / NUM_INT_PER_CONTROLLER;
        irq = (vector - NUM_INTERRUPTS) % NUM_INT_PER_CONTROLLER;
        if (intc >= HAL_ARRAY_SIZE(rk_intc_table))
            return;
        HAL_INTC_MaskIRQ(rk_intc_table[intc]->intc_base, irq);
    }
}

static void rk_intc_unmask(uint32_t vector)
{
    uint32_t intc, irq;

    if (vector >= TOTAL_INTERRUPTS)
        return;

    if (vector < NUM_INTERRUPTS)
        HAL_NVIC_EnableIRQ(vector);
    else
    {
        intc = (vector - NUM_INTERRUPTS) / NUM_INT_PER_CONTROLLER;
        irq = (vector - NUM_INTERRUPTS) % NUM_INT_PER_CONTROLLER;
        if (intc >= HAL_ARRAY_SIZE(rk_intc_table))
            return;
        HAL_INTC_UnmaskIRQ(rk_intc_table[intc]->intc_base, irq);
    }
}


#endif /* end of EXT_INTERRUPT */

#ifdef RT_USING_PROF_IRQ

#ifndef TOTAL_INTERRUPTS
#define TOTAL_INTERRUPTS NUM_INTERRUPTS
#endif

#define IRQ_AVG_COUNT  200

struct irq_summry
{
    uint64_t count;
    uint64_t start;
    uint64_t time_total;
    uint64_t time_avg;
    uint64_t time_max;
};

static struct irq_summry g_irq_prof[TOTAL_INTERRUPTS];

static void irq_enter_hook(void)
{
    uint32_t irq;

    irq = __get_IPSR() - 16;
    g_irq_prof[irq].count++;
    g_irq_prof[irq].start = HAL_TIMER_GetCount(SYS_TIMER);
}

static void irq_leave_hook(void)
{
    uint64_t time_cur, end;
    uint32_t irq;

    irq = __get_IPSR() - 16;
    end = HAL_TIMER_GetCount(SYS_TIMER);
    if (end < g_irq_prof[irq].start)
        time_cur = end + (uint64_t) -1 - g_irq_prof[irq].start;
    else
        time_cur = end - g_irq_prof[irq].start;
    g_irq_prof[irq].time_total += time_cur;
    g_irq_prof[irq].time_max =
        g_irq_prof[irq].time_max > time_cur ? g_irq_prof[irq].time_max : time_cur;
    if (g_irq_prof[irq].count > 0 &&
            g_irq_prof[irq].count % IRQ_AVG_COUNT == 0)
    {
        g_irq_prof[irq].time_avg = g_irq_prof[irq].time_total / IRQ_AVG_COUNT;
        g_irq_prof[irq].time_total = 0;
    }
}

static void dump_irq_summary(int argc, char **argv)
{
    uint32_t i;

    rt_kprintf("---IRQ SUMMARY(the unit of time is us)---\n");
    rt_kprintf("IRQ    COUNT       AVG         MAX\n");
    for (i = 0; i < NUM_INTERRUPTS; i++)
    {
        rt_kprintf("%03d    %08d    %08d    %08d\n", i, g_irq_prof[i].count,
                   (uint32_t)(g_irq_prof[i].time_avg * 1000000 / PLL_INPUT_OSC_RATE),
                   (uint32_t)(g_irq_prof[i].time_max * 1000000 / PLL_INPUT_OSC_RATE));
    }
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(dump_irq_summary, dump irq summary);
#endif

#endif    /* end of RT_USING_PROF_IRQ */

void rt_hw_interrupt_init(void)
{
    uint32_t i;

    for (i = 0; i < NUM_INTERRUPTS; i++)
    {
        HAL_NVIC_SetPriority(i, NVIC_PERIPH_PRIO_DEFAULT, NVIC_PERIPH_SUB_PRIO_DEFAULT);
    }

#if EXT_INTERRUPT
    rk_intc_init();
#endif

#ifdef RT_USING_PROF_IRQ
    rt_interrupt_enter_sethook(irq_enter_hook);
    rt_interrupt_leave_sethook(irq_leave_hook);
#endif
}

void rt_hw_interrupt_mask(int vector)
{
#if EXT_INTERRUPT
    rk_intc_mask(vector);
#else
    HAL_NVIC_DisableIRQ(vector);
#endif
}

void rt_hw_interrupt_umask(int vector)
{
#if EXT_INTERRUPT
    rk_intc_unmask(vector);
#else
    HAL_NVIC_EnableIRQ(vector);
#endif
}

rt_isr_handler_t rt_hw_interrupt_install(int              vector,
        rt_isr_handler_t handler,
        void            *param,
        const char      *name)
{
#if EXT_INTERRUPT
    if (vector < NUM_INTERRUPTS)
        HAL_NVIC_SetIRQHandler(vector, (NVIC_IRQHandler)handler);
    else
        ext_vector[vector - NUM_INTERRUPTS] = handler;
#else
    HAL_NVIC_SetIRQHandler(vector, (NVIC_IRQHandler)handler);
#endif
    return handler;
}

#endif    /* end of defined(ARCH_ARM_CORTEX_M0) || defined(ARCH_ARM_CORTEX_M3) || defined(ARCH_ARM_CORTEX_M4) || defined(ARCH_ARM_CORTEX_M7) */
