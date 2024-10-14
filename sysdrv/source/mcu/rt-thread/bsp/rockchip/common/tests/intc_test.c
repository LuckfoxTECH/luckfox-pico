/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 */
#include "rtthread.h"
#include "rthw.h"
#include "hal_base.h"

#ifdef RT_USING_COMMON_TEST_INTC

static uint8_t trigger_irq;

void irq_final_handler()
{
    if (!HAL_INTC_GetFinalStatus(INTC0, trigger_irq))
    {
        rt_kprintf("irq final test failed: %d, raw: 0x%x, status: 0x%x, mask: 0x%x, final: 0x%x\n",
                   trigger_irq, INTC0->IRQ_RAWSTATUS_L, INTC0->IRQ_STATUS_L,
                   INTC0->IRQ_MASKSTATUS_L, INTC0->IRQ_FINALSTATUS_L);
    }
    else
    {
        /* code */
        rt_kprintf("irq final test success: %d\n", trigger_irq);
    }
    HAL_INTC_ClearSoftwareIRQ(INTC0, trigger_irq);
}

void irq_mask_handler()
{
    rt_kprintf("irq mask test failed: %d\n", trigger_irq);
    HAL_INTC_ClearSoftwareIRQ(INTC0, trigger_irq);
}

void irq_unmask_handler()
{
    if (!HAL_INTC_GetFinalStatus(INTC0, trigger_irq))
    {
        rt_kprintf("irq unmask test failed: %d, raw: 0x%x, status: 0x%x, mask: 0x%x, final: 0x%x\n",
                   trigger_irq, INTC0->IRQ_RAWSTATUS_L, INTC0->IRQ_STATUS_L,
                   INTC0->IRQ_MASKSTATUS_L, INTC0->IRQ_FINALSTATUS_L);
    }
    else
    {
        /* code */
        rt_kprintf("irq unmask test success: %d\n", trigger_irq);
    }
    HAL_INTC_ClearSoftwareIRQ(INTC0, trigger_irq);
}

void irq_disable_handler()
{
    rt_kprintf("irq disable test failed: %d\n", trigger_irq);
    HAL_INTC_ClearSoftwareIRQ(INTC0, trigger_irq);
}

void irq_enable_handler()
{
    if (!HAL_INTC_GetFinalStatus(INTC0, trigger_irq))
    {
        rt_kprintf("irq enable test failed: %d\n", trigger_irq);
    }
    else
    {
        /* code */
        rt_kprintf("irq enable test success: %d\n", trigger_irq);
    }
    HAL_INTC_ClearSoftwareIRQ(INTC0, trigger_irq);
}

void irq_priority_low_handler()
{
    rt_kprintf("irq priority low test failed: %d\n", trigger_irq);
    HAL_INTC_ClearSoftwareIRQ(INTC0, trigger_irq);
}

void irq_priority_high_handler()
{
    if (!HAL_INTC_GetFinalStatus(INTC0, trigger_irq))
    {
        rt_kprintf("irq priority high test failed: %d\n", trigger_irq);
    }
    else
    {
        /* code */
        rt_kprintf("irq priority high test success: %d\n", trigger_irq);
    }
    HAL_INTC_ClearSoftwareIRQ(INTC0, trigger_irq);
    HAL_INTC_SetPriorityLevel(INTC0, 0);
}

static void test_irq(struct INTC_REG *INTC, uint8_t IRQn)
{
    /* setup */
    rt_hw_interrupt_install(INTC0_IRQn, irq_final_handler, NULL, "irq_handle");
    rt_hw_interrupt_umask(IRQn);

    /* test irq trigger, the final statue must matched */
    trigger_irq = IRQn - NUM_INTERRUPTS;
    HAL_INTC_SetSoftwareIRQ(INTC, trigger_irq);

    /* test irq mask*/
    rt_hw_interrupt_install(INTC0_IRQn, irq_mask_handler, NULL, "irq_handle");
    HAL_INTC_MaskIRQ(INTC, trigger_irq);
    HAL_INTC_SetSoftwareIRQ(INTC, trigger_irq);

    /* test irq unmask */
    rt_hw_interrupt_install(INTC0_IRQn, irq_unmask_handler, NULL, "irq_handle");
    HAL_INTC_ClearSoftwareIRQ(INTC, trigger_irq);
    HAL_INTC_UnmaskIRQ(INTC, trigger_irq);
    HAL_INTC_SetSoftwareIRQ(INTC, trigger_irq);

    /* test irq disable */
    rt_hw_interrupt_install(INTC0_IRQn, irq_disable_handler, NULL, "irq_handle");
    HAL_INTC_DisableIRQ(INTC, trigger_irq);
    HAL_INTC_SetSoftwareIRQ(INTC, trigger_irq);

    /* test irq enable*/
    rt_hw_interrupt_install(INTC0_IRQn, irq_enable_handler, NULL, "irq_handle");
    HAL_INTC_ClearSoftwareIRQ(INTC, trigger_irq);
    HAL_INTC_EnableIRQ(INTC, trigger_irq);
    HAL_INTC_SetSoftwareIRQ(INTC, trigger_irq);

    /* test low priority */
    rt_hw_interrupt_install(INTC0_IRQn, irq_priority_low_handler, NULL, "irq_handle");
    HAL_INTC_SetPriority(INTC, trigger_irq, 0x07);
    HAL_INTC_SetPriorityLevel(INTC, 0x08);
    HAL_INTC_SetSoftwareIRQ(INTC, trigger_irq);

    /* test high priority */
    rt_hw_interrupt_install(INTC0_IRQn, irq_priority_high_handler, NULL, "irq_handle");
    HAL_INTC_ClearSoftwareIRQ(INTC, trigger_irq);
    HAL_INTC_SetPriority(INTC, trigger_irq, 0x09);
    HAL_INTC_SetSoftwareIRQ(INTC, trigger_irq);
}

struct INTC_REG *g_intc[] =
{
#ifdef INTC0
    INTC0,
#endif
#ifdef INTC1
    INTC1,
#endif
#ifdef INTC2
    INTC2,
#endif
#ifdef INTC3
    INTC3,
#endif
};

static void test_intc(int argc, char **argv)
{
    uint8_t controller_index, irq_index, irqno;
    struct INTC_REG *INTC;

    for (irqno = NUM_INTERRUPTS; irqno < TOTAL_INTERRUPTS; irqno++)
    {
        controller_index = (irqno - NUM_INTERRUPTS) / 64;
        if (controller_index >= HAL_ARRAY_SIZE(g_intc))
        {
            rt_kprintf("irq number overflow\n");
            return;
        }
        INTC = g_intc[controller_index];
        irq_index = irqno % 64;
        test_irq(INTC, irq_index);
    }

    rt_kprintf("test intc finish\n");
}

static uint64_t irq_set_time, irq_handle_time;
static uint8_t irq_index, irq_offset;

void irq_perf_handler()
{
    uint32_t cycles;

    irq_handle_time = HAL_GetSysTimerCount();
    cycles = irq_handle_time - irq_set_time;
    rt_kprintf("irq responsed time: %d(cycle count of OSC)\n", cycles);
    HAL_INTC_ClearSoftwareIRQ(g_intc[irq_index], irq_offset);
}

static void test_irq_perf(int argc, char **argv)
{
    uint32_t irq, loop;

    if (argc != 3)
    {
        rt_kprintf("usage: %s irq_no loop_count\n", argv[0]);
        return;
    }

    irq = atoi(argv[1]);
    if (irq >= TOTAL_INTERRUPTS)
    {
        rt_kprintf("inval irq no\n", argv[0]);
        return;
    }

    loop = atoi(argv[2]);

    rt_hw_interrupt_install(irq, irq_perf_handler, NULL, "irq_handle");
    rt_hw_interrupt_umask(irq);

    for (int i = 0; i < loop; i++)
    {
        if (irq < NUM_INTERRUPTS)
        {
            irq_set_time = HAL_GetSysTimerCount();
            HAL_NVIC_SetPendingIRQ(irq);
        }
        else
        {
            /* code */
            irq_index = (irq - NUM_INTERRUPTS) / 64;
            irq_offset = (irq - NUM_INTERRUPTS) % 64;
            irq_set_time = HAL_GetSysTimerCount();
            HAL_INTC_SetSoftwareIRQ(g_intc[irq_index], irq_offset);
        }
    }

    rt_kprintf("%s finish\n", __FUNCTION__);
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(test_intc, intc test);
MSH_CMD_EXPORT(test_irq_perf, irq perf test);
#endif

#endif
