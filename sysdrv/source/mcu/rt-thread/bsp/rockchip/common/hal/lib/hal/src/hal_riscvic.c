/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_RISCVIC_MODULE_ENABLED

#include "riscv_csr_encoding.h"

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup RISCVIC
 *  @{
 */

/** @defgroup RISCVIC_How_To_Use How To Use
 *  @{

 The RISCVIC driver can be used as follows:

 - Invoke HAL_RISCVIC_Init() API to initialize RISCV Interrupt Controller.

 - Invoke HAL_IRQ_HANDLER_SetIRQHandler() API to register handler for a irq.

 - Invoke HAL_RISCVIC_DisableIRQ() to disable a irq.

 - Invoke HAL_RISCVIC_EnableIRQ() to enable a irq.

 @} */

/** @defgroup RISCVIC_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/
#ifdef HAL_RISCVIC_DEBUG
#define RISCVIC_Debug(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define RISCVIC_Debug(fmt, ...)
#endif

#if !defined(HAL_IRQ_HANDLER_MODULE_ENABLED) || !defined(HAL_RISCVIC_INTERRUPT_SEPERATE)
#error "Enable HAL_IRQ_HANDLER_MODULE_ENABLED and HAL_RISCVIC_INTERRUPT_SEPERATE"
#endif
/********************* Private Structure Definition **************************/
struct Exception_Stack_Frame {
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

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/
/**
 * @brief  RISCVIC Interrupt Controller Write CSR.
 * @param  addr: CSR Address.
 * @param  val: Value.
 * @return None.
 */
static void HAL_RISCVIC_WriteCSR(unsigned int addr, unsigned int val)
{
    switch (addr) {
    case IPIC_CISV:
        write_csr(0xbf0, val);
        break;
    case IPIC_CICSR:
        write_csr(0xbf1, val);
        break;
    case IPIC_IPR:
        write_csr(0xbf2, val);
        break;
    case IPIC_ISVR:
        write_csr(0xbf3, val);
        break;
    case IPIC_EOI:
        write_csr(0xbf4, val);
        break;
    case IPIC_SOI:
        write_csr(0xbf5, val);
        break;
    case IPIC_IDX:
        write_csr(0xbf6, val);
        break;
    case IPIC_ICSR:
        write_csr(0xbf7, val);
        break;
    default:
        break;
    }
}

static unsigned int HAL_RISCVIC_ReadCSR(unsigned int addr)
{
    unsigned int res;

    switch (addr) {
    case IPIC_CISV:
        res = read_csr(0xbf0);
        break;
    case IPIC_CICSR:
        res = read_csr(0xbf1);
        break;
    case IPIC_IPR:
        res = read_csr(0xbf2);
        break;
    case IPIC_ISVR:
        res = read_csr(0xbf3);
        break;
    case IPIC_EOI:
        res = read_csr(0xbf4);
        break;
    case IPIC_SOI:
        res = read_csr(0xbf5);
        break;
    case IPIC_IDX:
        res = read_csr(0xbf6);
        break;
    case IPIC_ICSR:
        res = read_csr(0xbf7);
        break;
    default:
        res = 0;
    }

    return res;
}

/**
 * @brief  Get Active Interrupt number.
 * @return interrupt number.
 */
static uint32_t HAL_RISCVIC_GetActiveIRQ(void)
{
    return HAL_RISCVIC_ReadCSR(IPIC_CISV);
}

static void HAL_RISCVIC_ExtIRQInit(void)
{
    set_csr(mie, MIP_MEIP);
}

static void HAL_RISCVIC_ExtIRQHandle(void)
{
    /* Any value can be written in IPIC_SOI to start process */
    HAL_RISCVIC_WriteCSR(IPIC_SOI, 0x0);
#ifdef HAL_INTMUX_MODULE_ENABLED
    HAL_INTMUX_DirectDispatch(HAL_RISCVIC_GetActiveIRQ());
#else
    printf("Irq is %ld, please open HAL_INTMUX_MODULE_ENABLED!\n", HAL_RISCVIC_GetActiveIRQ());
#endif
    /* Any value can be written in IPIC_EOI to end process */
    HAL_RISCVIC_WriteCSR(IPIC_EOI, 0x0);
}

static int Print_Stack_Frame(unsigned int sp)
{
#ifdef HAL_RISCVIC_DEBUG
    struct Exception_Stack_Frame *esf = (struct Exception_Stack_Frame *)(sp + 4);

    RISCVIC_Debug("\n=================================================================\n");
    RISCVIC_Debug("x1 (ra   : Return address		    ) ==> 0x%08x\n", esf->x1);
    RISCVIC_Debug("x2 (sp   : Stack pointer		    ) ==> 0x%08x\n", esf->x2);
    RISCVIC_Debug("x3 (gp   : Global pointer		    ) ==> 0x%08x\n", esf->x3);
    RISCVIC_Debug("x4 (tp   : Thread pointer		    ) ==> 0x%08x\n", esf->x4);
    RISCVIC_Debug("x5 (t0   : Temporary             ) ==> 0x%08x\n", esf->x5);
    RISCVIC_Debug("x6 (t1   : Temporary             ) ==> 0x%08x\n", esf->x6);
    RISCVIC_Debug("x7 (t2   : Temporary             ) ==> 0x%08x\n", esf->x7);
    RISCVIC_Debug("x8 (s0/fp: Save register,frame pointer   ) ==> 0x%08x\n", esf->x8);
    RISCVIC_Debug("x9 (s1   : Save register		    ) ==> 0x%08x\n", esf->x9);
    RISCVIC_Debug("x10(a0   : Function argument,return value) ==> 0x%08x\n", esf->x10);
    RISCVIC_Debug("x11(a1   : Function argument,return value) ==> 0x%08x\n", esf->x11);
    RISCVIC_Debug("x12(a2   : Function argument         ) ==> 0x%08x\n", esf->x12);
    RISCVIC_Debug("x13(a3   : Function argument         ) ==> 0x%08x\n", esf->x13);
    RISCVIC_Debug("x14(a4   : Function argument         ) ==> 0x%08x\n", esf->x14);
    RISCVIC_Debug("x15(a5   : Function argument         ) ==> 0x%08x\n", esf->x15);
    RISCVIC_Debug("x16(a6   : Function argument         ) ==> 0x%08x\n", esf->x16);
    RISCVIC_Debug("x17(a7   : Function argument         ) ==> 0x%08x\n", esf->x17);
    RISCVIC_Debug("x18(s2   : Save register		    ) ==> 0x%08x\n", esf->x18);
    RISCVIC_Debug("x19(s3   : Save register		    ) ==> 0x%08x\n", esf->x19);
    RISCVIC_Debug("x20(s4   : Save register		    ) ==> 0x%08x\n", esf->x20);
    RISCVIC_Debug("x21(s5   : Save register		    ) ==> 0x%08x\n", esf->x21);
    RISCVIC_Debug("x22(s6   : Save register		    ) ==> 0x%08x\n", esf->x22);
    RISCVIC_Debug("x23(s7   : Save register		    ) ==> 0x%08x\n", esf->x23);
    RISCVIC_Debug("x24(s8   : Save register		    ) ==> 0x%08x\n", esf->x24);
    RISCVIC_Debug("x25(s9   : Save register		    ) ==> 0x%08x\n", esf->x25);
    RISCVIC_Debug("x26(s10  : Save register		    ) ==> 0x%08x\n", esf->x26);
    RISCVIC_Debug("x27(s11  : Save register		    ) ==> 0x%08x\n", esf->x27);
    RISCVIC_Debug("x28(t3   : Temporary             ) ==> 0x%08x\n", esf->x28);
    RISCVIC_Debug("x29(t4   : Temporary             ) ==> 0x%08x\n", esf->x29);
    RISCVIC_Debug("x30(t5   : Temporary             ) ==> 0x%08x\n", esf->x30);
    RISCVIC_Debug("x31(t6   : Temporary             ) ==> 0x%08x\n", esf->x31);
    RISCVIC_Debug("=================================================================\n");
#endif

    return 0;
}

/** @} */

/********************* Public Function Definition ****************************/

/** @defgroup RISCVIC_Exported_Functions_Group5 Other Functions
 *  @{
 */

extern void scr1_trap_entry(void);

/**
 * @brief  Init RISCVIC Interrupt Controller.
 * @return HAL_OK.
 */
HAL_Status HAL_RISCVIC_Init(void)
{
    int i;

    write_csr(mstatus, 0x00001888);
    /* Set the vector table */
    write_csr(mtvec, scr1_trap_entry);

    for (i = 0; i < 8; i++) {
        HAL_RISCVIC_WriteCSR(IPIC_IDX, i);
        /* enable, level trigger */
        HAL_RISCVIC_WriteCSR(IPIC_ICSR, IPIC_ICSR_IE);
    }
    /* Enable the external interrupt */
    HAL_RISCVIC_ExtIRQInit();

    return HAL_OK;
}

/**
 * @brief  Dispatch handler for a irq.
 * @param  iMcause: MCAUSE register value which hold the interrupt's status.
 * @param  iMepc: MEPC register value.
 * @param  iSp: stack pointer.
 * @return HAL_OK.
 */
HAL_Status HAL_RISCVIC_IRQHandler(unsigned int iMcause, unsigned int iMepc, unsigned int iSp)
{
    unsigned int mcause = iMcause & 0x7fffffff;

    if ((iMcause & 0x80000000) == 0x80000000) {
        /* environment interrupt */
        switch (mcause) {
        case IRQ_S_SOFT:
            RISCVIC_Debug("s_soft irq enter\n");
            break;
        case IRQ_H_SOFT:
            RISCVIC_Debug("h_soft irq enter\n");
            break;
        case IRQ_M_SOFT:
            RISCVIC_Debug("m_soft irq enter\n");
            break;
        case IRQ_S_TIMER:
            RISCVIC_Debug("s_timer irq enter\n");
            break;
        case IRQ_H_TIMER:
            RISCVIC_Debug("h_timer irq enter\n");
            break;
        case IRQ_M_TIMER:
            RISCVIC_Debug("sys tick irq enter\n");
            break;
        case IRQ_S_EXT:
            RISCVIC_Debug("s_ext irq enter\n");
            break;
        case IRQ_H_EXT:
            RISCVIC_Debug("h_ext irq enter\n");
            break;
        case IRQ_M_EXT:
            HAL_RISCVIC_ExtIRQHandle();
            break;
        case IRQ_COP:
            RISCVIC_Debug("cop irq enter\n");
            break;
        case IRQ_HOST:
            RISCVIC_Debug("host irq enter\n");
            break;
        default:
            break;
        }
    } else {
        /* system interrupt */
        switch (mcause) {
        case EXC_IAM:
            RISCVIC_Debug("Instruction address misaligned\n");
            break;
        case EXC_IAF:
            RISCVIC_Debug("Instruction access fault\n");
            break;
        case EXC_II:
            RISCVIC_Debug("Illegal instruction\n");
            break;
        case EXC_BP:
            /* Since we use the imc instruction set,
             * ebreak is 2-byte instruction. So add 2 to
             * mepc.
             */
            write_csr(mepc, iMepc + 2);
            RISCVIC_Debug("Breakpoint\n");
            break;
        case EXC_LAM:
            RISCVIC_Debug("Load address misaligned\n");
            break;
        case EXC_LAF:
            RISCVIC_Debug("Load access fault\n");
            break;
        case EXC_SAM:
            RISCVIC_Debug("Store/AMO address misaligned\n");
            break;
        case EXC_SAF:
            RISCVIC_Debug("Store/AMO access fault\n");
            break;
        case EXC_ECFU:
            RISCVIC_Debug("Environment call from U-mode\n");
            break;
        case EXC_ECFS:
            RISCVIC_Debug("Environment call from S-mode\n");
            break;
        case EXC_ECFM:
            write_csr(mepc, iMepc + 4);
            RISCVIC_Debug("Environment call from M-mode\n");
            break;
        case EXC_IPF:
            RISCVIC_Debug("Instruction page fault\n");
            break;
        case EXC_LPF:
            RISCVIC_Debug("Load page fault\n");
            break;
        case EXC_SPF:
            RISCVIC_Debug("Store/AMO page fault\n");
            break;
        default:
            break;
        }

        Print_Stack_Frame(iSp);
        RISCVIC_Debug("mepc => 0x%08x\n", read_csr(mepc));
        RISCVIC_Debug("exception pc => 0x%08x\n", iMepc);
        RISCVIC_Debug("stack pointer => 0x%08x\n", iSp);
        while (1) {
            ;
        }
    }

    return HAL_OK;
}
/** @} */

/** @} */

/** @} */
#endif
