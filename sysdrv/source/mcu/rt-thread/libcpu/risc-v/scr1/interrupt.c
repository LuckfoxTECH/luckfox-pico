/**
  * Copyright (c) 2020 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    interrupt.c
  * @author  Jason Zhu
  * @version V0.1
  * @date    23-Feb-2020
  * @brief
  *
  ******************************************************************************
  */

#include <rtthread.h>
#include <rthw.h>
#include "board.h"

#ifdef DEBUG
#define DEBUG_PRINTF(...)   rt_kprintf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif

static struct rt_irq_desc irq_desc[MAX_HANDLERS];
static struct rt_irq_desc soft_irq_desc;

extern void trap_entry();

static void write_csr_ipic(unsigned int addr, unsigned int val)
{
    switch (addr)
    {
    case IPIC_CISV :
        write_csr(0xbf0, val);
        break;
    case IPIC_CICSR :
        write_csr(0xbf1, val);
        break;
    case IPIC_IPR :
        write_csr(0xbf2, val);
        break;
    case IPIC_ISVR :
        write_csr(0xbf3, val);
        break;
    case IPIC_EOI :
        write_csr(0xbf4, val);
        break;
    case IPIC_SOI :
        write_csr(0xbf5, val);
        break;
    case IPIC_IDX :
        write_csr(0xbf6, val);
        break;
    case IPIC_ICSR :
        write_csr(0xbf7, val);
        break;
    default:
        break;
    }

    return ;
}

static unsigned int read_csr_ipic(unsigned int addr)
{
    unsigned int res;

    switch (addr)
    {
    case IPIC_CISV :
        res = read_csr(0xbf0);
        break;
    case IPIC_CICSR :
        res = read_csr(0xbf1);
        break;
    case IPIC_IPR :
        res = read_csr(0xbf2);
        break;
    case IPIC_ISVR :
        res = read_csr(0xbf3);
        break;
    case IPIC_EOI :
        res = read_csr(0xbf4);
        break;
    case IPIC_SOI :
        res = read_csr(0xbf5);
        break;
    case IPIC_IDX :
        res = read_csr(0xbf6);
        break;
    case IPIC_ICSR :
        res = read_csr(0xbf7);
        break;
    default:
        res = 0;
    }

    return res;
}

static void m_ext_irq_init()
{
    set_csr(mie, MIP_MEIP);
}

static void m_soft_irq_init()
{
    set_csr(mie, MIP_MSIP);
}

static void m_ext_irq_handle(void)
{
    rt_isr_handler_t isr_func;
    rt_uint32_t irq;
    void *param;

    /* Any value can be written in IPIC_SOI to start process */
    write_csr_ipic(IPIC_SOI, 0x0);
    irq = rt_hw_interrupt_get_active();
    rt_hw_interrupt_mask(irq);
    /* get interrupt service routine */
    isr_func = irq_desc[irq].handler;
    param = irq_desc[irq].param;

    /* turn to interrupt service routine */
    isr_func(irq, param);
    /* Any value can be written in IPIC_EOI to end process */
    write_csr_ipic(IPIC_EOI, 0x0);
    rt_hw_interrupt_umask(irq);
#ifdef RT_USING_INTERRUPT_INFO
    irq_desc[irq].counter ++;
#endif
}

static void m_soft_irq_handle(void)
{
    rt_isr_handler_t isr_func;
    void *param;

    clear_csr(mie, MIP_MSIP);
    /* get interrupt service routine */
    isr_func = soft_irq_desc.handler;
    param = soft_irq_desc.param;

    /* turn to interrupt service routine */
    isr_func(0, param);

#ifdef RT_USING_INTERRUPT_INFO
    soft_irq_desc.counter ++;
#endif
    set_csr(mie, MIP_MSIP);
}

static void SysTick_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    _update_timer();
    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}

struct exception_stack_frame
{
    uint32_t x1;
    uint32_t x2;
    uint32_t x3;
    uint32_t x4;
    uint32_t x5;
    uint32_t x6;
    uint32_t x7;
    uint32_t x8;
    uint32_t x9;
    uint32_t x10;
    uint32_t x11;
    uint32_t x12;
    uint32_t x13;
    uint32_t x14;
    uint32_t x15;
    uint32_t x16;
    uint32_t x17;
    uint32_t x18;
    uint32_t x19;
    uint32_t x20;
    uint32_t x21;
    uint32_t x22;
    uint32_t x23;
    uint32_t x24;
    uint32_t x25;
    uint32_t x26;
    uint32_t x27;
    uint32_t x28;
    uint32_t x29;
    uint32_t x30;
    uint32_t x31;
};

void print_stack_frame(unsigned int sp)
{
    struct exception_stack_frame *esf = (struct exception_stack_frame *)(sp + 4);

    rt_kprintf("\n=================================================================\n");
    rt_kprintf("x1 (ra   : Return address                ) ==> 0x%08x\n", esf->x1);
    rt_kprintf("x2 (sp   : Stack pointer                 ) ==> 0x%08x\n", esf->x2);
    rt_kprintf("x3 (gp   : Global pointer                ) ==> 0x%08x\n", esf->x3);
    rt_kprintf("x4 (tp   : Thread pointer                ) ==> 0x%08x\n", esf->x4);
    rt_kprintf("x5 (t0   : Temporary                     ) ==> 0x%08x\n", esf->x5);
    rt_kprintf("x6 (t1   : Temporary                     ) ==> 0x%08x\n", esf->x6);
    rt_kprintf("x7 (t2   : Temporary                     ) ==> 0x%08x\n", esf->x7);
    rt_kprintf("x8 (s0/fp: Save register,frame pointer   ) ==> 0x%08x\n", esf->x8);
    rt_kprintf("x9 (s1   : Save register                 ) ==> 0x%08x\n", esf->x9);
    rt_kprintf("x10(a0   : Function argument,return value) ==> 0x%08x\n", esf->x10);
    rt_kprintf("x11(a1   : Function argument,return value) ==> 0x%08x\n", esf->x11);
    rt_kprintf("x12(a2   : Function argument             ) ==> 0x%08x\n", esf->x12);
    rt_kprintf("x13(a3   : Function argument             ) ==> 0x%08x\n", esf->x13);
    rt_kprintf("x14(a4   : Function argument             ) ==> 0x%08x\n", esf->x14);
    rt_kprintf("x15(a5   : Function argument             ) ==> 0x%08x\n", esf->x15);
    rt_kprintf("x16(a6   : Function argument             ) ==> 0x%08x\n", esf->x16);
    rt_kprintf("x17(a7   : Function argument             ) ==> 0x%08x\n", esf->x17);
    rt_kprintf("x18(s2   : Save register                 ) ==> 0x%08x\n", esf->x18);
    rt_kprintf("x19(s3   : Save register                 ) ==> 0x%08x\n", esf->x19);
    rt_kprintf("x20(s4   : Save register                 ) ==> 0x%08x\n", esf->x20);
    rt_kprintf("x21(s5   : Save register                 ) ==> 0x%08x\n", esf->x21);
    rt_kprintf("x22(s6   : Save register                 ) ==> 0x%08x\n", esf->x22);
    rt_kprintf("x23(s7   : Save register                 ) ==> 0x%08x\n", esf->x23);
    rt_kprintf("x24(s8   : Save register                 ) ==> 0x%08x\n", esf->x24);
    rt_kprintf("x25(s9   : Save register                 ) ==> 0x%08x\n", esf->x25);
    rt_kprintf("x26(s10  : Save register                 ) ==> 0x%08x\n", esf->x26);
    rt_kprintf("x27(s11  : Save register                 ) ==> 0x%08x\n", esf->x27);
    rt_kprintf("x28(t3   : Temporary                     ) ==> 0x%08x\n", esf->x28);
    rt_kprintf("x29(t4   : Temporary                     ) ==> 0x%08x\n", esf->x29);
    rt_kprintf("x30(t5   : Temporary                     ) ==> 0x%08x\n", esf->x30);
    rt_kprintf("x31(t6   : Temporary                     ) ==> 0x%08x\n", esf->x31);
    rt_kprintf("=================================================================\n");
}

void handle_trap(unsigned int i_mcause, unsigned int i_mepc, unsigned int i_sp)
{
    DEBUG_PRINTF("mcasue 0x%x, i_mepc is 0x%x, i_sp is 0x%x\n", i_mcause, i_mepc, i_sp);
    unsigned int mcause = i_mcause & 0x7fffffff;

    if ((i_mcause & 0x80000000) == 0x80000000)
    {
        /* environment interrupt */
        switch (mcause)
        {
        case IRQ_S_SOFT:
            rt_kprintf("s_soft irq enter\n");
            break;
        case IRQ_H_SOFT:
            rt_kprintf("h_soft irq enter\n");
            break;
        case IRQ_M_SOFT:
            m_soft_irq_handle();
            break;
        case IRQ_S_TIMER:
            rt_kprintf("s_timer irq enter\n");
            break;
        case IRQ_H_TIMER:
            rt_kprintf("h_timer irq enter\n");
            break;
        case IRQ_M_TIMER:
            SysTick_Handler();
            break;
        case IRQ_S_EXT:
            rt_kprintf("s_ext irq enter\n");
            break;
        case IRQ_H_EXT:
            rt_kprintf("h_ext irq enter\n");
            break;
        case IRQ_M_EXT:
            m_ext_irq_handle();
            break;
        case IRQ_COP:
            rt_kprintf("cop irq enter\n");
            break;
        case IRQ_HOST:
            rt_kprintf("host irq enter\n");
            break;
        default:
            rt_kprintf("Environment interrupt not implecation, mcause is 0x%x\n", i_mcause);
        }
    }
    else
    {
        /* system interrupt */
        switch (mcause)
        {
        case EXC_IAM:
            rt_kprintf("Instruction address misaligned\n");
            break;
        case EXC_IAF:
            rt_kprintf("Instruction access fault\n");
            break;
        case EXC_II:
            rt_kprintf("Illegal instruction\n");
            break;
        case EXC_BP:
            write_csr(mepc, i_mepc + 4);
            rt_kprintf("Breakpoint\n");
            break;
        case EXC_LAM:
            rt_kprintf("Load address misaligned\n");
            break;
        case EXC_LAF:
            rt_kprintf("Load access fault\n");
            break;
        case EXC_SAM:
            rt_kprintf("Store/AMO address misaligned\n");
            break;
        case EXC_SAF:
            rt_kprintf("Store/AMO access fault\n");
            break;
        case EXC_ECFU:
            rt_kprintf("Environment call from U-mode\n");
            break;
        case EXC_ECFS:
            rt_kprintf("Environment call from S-mode\n");
            break;
        case EXC_ECFM:
            write_csr(mepc, i_mepc + 4);
            rt_kprintf("Environment call from M-mode\n");
            break;
        case EXC_IPF:
            rt_kprintf("Instruction page fault\n");
            break;
        case EXC_LPF:
            rt_kprintf("Load page fault\n");
            break;
        case EXC_SPF:
            rt_kprintf("Store/AMO page fault\n");
            break;
        default:
            rt_kprintf("System interrupt not implecation, mcause is 0x%x\n", i_mcause);
        }
        print_stack_frame(i_sp);
        rt_kprintf("exception pc => 0x%08x\n", i_mepc);
        rt_kprintf("stack pointer => 0x%08x\n", i_sp);
        while (1);
    }
}

RT_WEAK void rt_hw_interrupt_active(int vector) {}

void rt_hw_interrupt_init(void)
{
    write_csr(mtvec, &trap_entry);
    m_ext_irq_init();
    m_soft_irq_init();
}

void rt_hw_interrupt_mask(int vector)
{
    /* only mask int_mux */
    rt_hw_interrupt_deactive(vector);
}

void rt_hw_interrupt_umask(int vector)
{
    rt_hw_interrupt_active(vector);
}

rt_isr_handler_t rt_hw_interrupt_install(int vector, rt_isr_handler_t handler,
        void *param, const char *name)
{
    rt_isr_handler_t old_handler = RT_NULL;

    if (vector < MAX_HANDLERS)
    {
        old_handler = irq_desc[vector].handler;
        if (handler != RT_NULL)
        {
            irq_desc[vector].handler = (rt_isr_handler_t)handler;
            irq_desc[vector].param = param;
            /* enable interrupt vector as default */
            write_csr_ipic(IPIC_IDX, vector / INTERRUPT_SEPERATE);
            write_csr_ipic(IPIC_ICSR, IPIC_ICSR_IE);
#ifdef RT_USING_INTERRUPT_INFO
            rt_snprintf(irq_desc[vector].name, RT_NAME_MAX - 1, "%s", name);
            irq_desc[vector].counter = 0;
#endif
        }
    }

    return old_handler;
}

rt_isr_handler_t rt_soft_interrupt_install(rt_isr_handler_t handler,
        void *param, const char *name)
{
    rt_isr_handler_t old_handler = RT_NULL;

    old_handler = soft_irq_desc.handler;
    if (handler != RT_NULL)
    {
        soft_irq_desc.handler = (rt_isr_handler_t)handler;
        soft_irq_desc.param = param;
#ifdef RT_USING_INTERRUPT_INFO
        rt_snprintf(soft_irq_desc.name, RT_NAME_MAX - 1, "%s", name);
        soft_irq_desc.counter = 0;
#endif
    }

    return old_handler;
}
