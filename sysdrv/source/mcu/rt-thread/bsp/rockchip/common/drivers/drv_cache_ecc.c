/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-05-15     Tony Xie     first implementation
 */

#include <rthw.h>
#include <rtthread.h>
#include "drv_cache_ecc.h"

#ifdef RT_USING_CACHE_ECC
static void l1_l2_ecc_print_status(void)
{
    uint32_t status, misc0, misc1;

    status = HAL_CACHE_ECC_GetErxstatus(CACHE_ECC_ERR0);
    misc0 = HAL_CACHE_ECC_GetErxmisc0(CACHE_ECC_ERR0);
    misc1 = HAL_CACHE_ECC_GetErxmisc1(CACHE_ECC_ERR0);
    rt_kprintf("l1_l2_ecc status-%x misc:%x-%x\n", status, misc1, misc0);
}

static void dsu_l3_ecc_print_status(void)
{
    uint32_t status, misc0, misc1;

    status = HAL_CACHE_ECC_GetErxstatus(CACHE_ECC_ERR1);
    misc0 = HAL_CACHE_ECC_GetErxmisc0(CACHE_ECC_ERR1);
    misc1 = HAL_CACHE_ECC_GetErxmisc1(CACHE_ECC_ERR1);
    rt_kprintf("dsu_l3_ecc status-%x misc:%x-%x\n", status, misc1, misc0);
}

static void l1_l2_ecc_fault_irq_handler(int irq, void *param)
{
    int cpu;

    cpu = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    rt_kprintf("l1_l2_ecc_fault: cpu-%d irq-%d\n", cpu, irq);
    l1_l2_ecc_print_status();
    while (1);
}

static void dsu_l3_ecc_fault_irq_handler(int irq, void *param)
{
    int cpu;

    cpu = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    rt_kprintf("dsu_l3_ecc_fault: cpu-%d irq-%d\n", cpu, irq);
    dsu_l3_ecc_print_status();
    while (1);
}

static void l1_l2_ecc_error_irq_handler(int irq, void *param)
{
    int cpu;

    cpu = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    rt_kprintf("l1_l2_ecc_error: cpu-%d irq-%d\n", cpu, irq);
    l1_l2_ecc_print_status();
    dsu_l3_ecc_print_status();
    while (1);
}

static void dsu_l3_ecc_error_irq_handler(int irq, void *param)
{
    int cpu;

    cpu = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    rt_kprintf("dsu_l3_ecc_error: cpu-%d irq-%d\n", cpu, irq);
    l1_l2_ecc_print_status();
    dsu_l3_ecc_print_status();
    while (1);
}

/**
 * @brief  cache ecc init.
 * @param  err1_irq_cpu: the cpu specified to handle err1 irq.
 */
int rk_cache_ecc_init(uint32_t err1_irq_cpu)
{
    int cpu_irq;
    uint32_t cpu = HAL_CPU_TOPOLOGY_GetCurrentCpuId();

    cpu_irq = NFAULT1_IRQn + cpu;
    rt_hw_interrupt_install(cpu_irq, l1_l2_ecc_fault_irq_handler,
                            NULL, "cpu_ecc_fault");
    HAL_GIC_Enable(cpu_irq);

    cpu_irq = NERR1_IRQn + cpu;
    rt_hw_interrupt_install(cpu_irq, l1_l2_ecc_error_irq_handler,
                            NULL, "cpu_ecc_error");
    HAL_GIC_Enable(cpu_irq);

    if (cpu == err1_irq_cpu)
    {
        rt_hw_interrupt_install(NFAULT0_IRQn, dsu_l3_ecc_fault_irq_handler,
                                NULL, "dsu_l3_ecc_fault");
        HAL_GIC_Enable(NFAULT0_IRQn);
        rt_hw_interrupt_install(NERR0_IRQn, dsu_l3_ecc_error_irq_handler,
                                NULL, "dsu_l3_ecc_error");
        HAL_GIC_Enable(NERR0_IRQn);
    }

    return RT_EOK;
}

#endif
