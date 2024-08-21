/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_AP_CORE

/*----------------------------------------------------------------------------
  Internal References
 *----------------------------------------------------------------------------*/
void Vectors                   (void) __attribute__ ((section("RESET")));
void Reset_Handler             (void);
void Default_Handler           (void);
void IRQ_Handler               (void);
void IRQ_HardIrqHandler        (void);
void IRQ_HardIrqPreemptHandler (uint32_t irqn);

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/
void Undef_Handler (void) __attribute__ ((weak, alias("Default_Handler")));
void SVC_Handler   (void) __attribute__ ((weak, alias("Default_Handler")));
void PAbt_Handler  (void) __attribute__ ((weak, alias("Default_Handler")));
void DAbt_Handler  (void) __attribute__ ((weak, alias("Default_Handler")));
void FIQ_Handler   (void) __attribute__ ((weak, alias("Default_Handler")));

#ifndef HAL_GIC_MODULE_ENABLED
void IRQ_Handler   (void) __attribute__ ((weak, alias("Default_Handler")));
#else
#ifdef HAL_GIC_PREEMPT_FEATURE_ENABLED
void IRQ_HardIrqPreemptHandler(uint32_t irqn)
{
#ifdef HAL_GPIO_IRQ_GROUP_MODULE_ENABLED
    HAL_GPIO_IRQ_GROUP_DispatchGIRQs(irqn);
#endif

    __enable_irq();

    HAL_IRQ_HANDLER_IRQHandler(irqn);
}
#else
void IRQ_HardIrqHandler(void)
{
    uint32_t irqn;
    GIC_IRQHandler handler;

    irqn = HAL_GIC_GetActiveIRQ();

    if (irqn >= HAL_GIC_IRQ_LINE_COUNT)
        return;

    if (irqn <= 15)
        HAL_GIC_EndOfInterrupt(irqn);

    HAL_IRQ_HANDLER_IRQHandler(irqn);

    if (irqn > 15)
        HAL_GIC_EndOfInterrupt(irqn);
}
#endif

#if defined(__GNUC__) && ! defined(__ARMCC_VERSION)
#pragma GCC push_options
#pragma GCC optimize ("-fomit-frame-pointer")
#elif defined(__ARMCC_VERSION)
#pragma push
#pragma -fomit-frame-pointer
#endif

void IRQ_Handler(void)
{
#ifdef HAL_GIC_PREEMPT_FEATURE_ENABLED
    __ASM volatile (
    "sub    lr, lr, #4                                \n"
    "stm    sp, {r0, lr}                              \n" // save r0, lr, spsr
    "mrs    lr, SPSR                                  \n"
    "str    lr, [sp, #8]                              \n"
    "mrs    r0, CPSR                                  \n"
    "eor    r0, r0, #1                                \n"
    "msr    SPSR_fsxc, r0                             \n" // set spsr to svc mode
    "mov    r0, sp                                    \n"
    "ldr    lr, =hw_irq                               \n"
    "movs   pc, lr                                    \n" // switch to svc mode
    "hw_irq:                                          \n"
    "sub    sp, sp, #72                               \n"
    "stmia  sp, {r1 - r12}                            \n" // save the cpu context
    "ldmia  r0, {r3 - r5}                             \n"
    "add    r7, sp, #48                               \n"
    "add    r2, sp, #72                               \n"
    "str    r3, [sp, #-4]!                            \n"
    "mov    r3, lr                                    \n"
    "stmia  r7, {r2 - r5}                             \n"
    "mrc    p15, 0, r6, c12, c12, 0                   \n" // get the irq id
    "cmp    r6, #1020                                 \n"
    "bhs    IRQ_HandlerEnd                            \n"
    "vmrs   r8, fpexc                                 \n"
    "tst    r8, #(1<<30)                              \n"
    "beq    1f                                        \n"
    "vstmdb sp!, {d0-d15}                             \n" // save fpu context
    "vstmdb sp!, {d16-d31}                            \n"
    "vmrs   r9, fpscr                                 \n"
    "stmfd  sp!, {r9}                                 \n"
    "1:                                               \n"
    "stmfd  sp!, {r8}                                 \n"
    "mrc    p15, 0, r2, c12, c11, 3                   \n" // get the running priority
    "mrc    p15, 0, r7, c4, c6, 0                     \n" // get the interrupt priority mask
    "mcr    p15, 0, r2, c4, c6, 0                     \n" // set the interrupt priority mask
    "mcr    p15, 0, r6, c12, c12, 1                   \n" // end of interrupt
    "cmp    r6, #16                                   \n"
    "bhs    2f                                        \n"
    "mcr    p15, 0, r6, c12, c11, 1                   \n" // deactivate sgi interrupt
    "2:                                               \n"
    "mov    r0, r6                                    \n"
    "bl     IRQ_HardIrqPreemptHandler                 \n"
    "ldmfd  sp!, {r8}                                 \n"
    "vmsr   fpexc, r8                                 \n"
    "tst    r8, #(1<<30)                              \n"
    "beq    1f                                        \n"
    "ldmfd  sp!, {r9}                                 \n"
    "vmsr   fpscr, r9                                 \n"
    "vldmia sp!, {d16-d31}                            \n"
    "vldmia sp!, {d0-d15}                             \n"
    "1:                                               \n"
    "cpsid  i                                         \n"
    "cmp    r6, #16                                   \n"
    "blo    2f                                        \n" // for sgi bypass deactivate interrupt
    "mcr    p15, 0, r6, c12, c11, 1                   \n" // deactivate interrupt
    "2:                                               \n"
    "mcr    p15, 0, r7, c4, c6, 0                     \n" // set the interrupt priority mask
    "IRQ_HandlerEnd:                                  \n"
    "msr    spsr_cxsf, r5                             \n" // restore the spsr
    "ldmia  sp, {r0 - pc}^                            \n" // restore the CPU context then exit irq handler
    );
#else
    __ASM volatile (
    "stmfd  sp!, {r0-r12,lr}                          \n" // save the CPU context
    "bl     IRQ_HardIrqHandler                        \n"
    "ldmfd  sp!, {r0-r12,lr}                          \n" // restore the CPU context
    "subs   pc,  lr, #4                               \n"
    );
#endif /* HAL_GIC_PREEMPT_FEATURE_ENABLED */
}
#endif /* HAL_GIC_MODULE_ENABLED */

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector Table
 *----------------------------------------------------------------------------*/
void Vectors(void)
{
    __ASM volatile(
    "LDR    PC, =Reset_Handler                        \n"
    "LDR    PC, =Undef_Handler                        \n"
    "LDR    PC, =SVC_Handler                          \n"
    "LDR    PC, =PAbt_Handler                         \n"
    "LDR    PC, =DAbt_Handler                         \n"
    "NOP                                              \n"
    "LDR    PC, =IRQ_Handler                          \n"
    "LDR    PC, =FIQ_Handler                          \n"
    );
}

/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
void Reset_Handler(void)
{
    __ASM volatile(

    // Mask interrupts
    "CPSID   if                                      \n"

    // Reset SCTLR Settings
    "MRC     p15, 0, R0, c1, c0, 0                   \n" // Read CP15 System Control register
    "BIC     R0, R0, #(0x1 << 12)                    \n" // Clear I bit 12 to disable I Cache
    "BIC     R0, R0, #(0x1 <<  2)                    \n" // Clear C bit  2 to disable D Cache
    "BIC     R0, R0, #0x1                            \n" // Clear M bit  0 to disable MMU
    "BIC     R0, R0, #(0x1 << 11)                    \n" // Clear Z bit 11 to disable branch prediction
    "BIC     R0, R0, #(0x1 << 13)                    \n" // Clear V bit 13 to disable hivecs
    "MCR     p15, 0, R0, c1, c0, 0                   \n" // Write value back to CP15 System Control register
    "ISB                                             \n"

    // Configure ACTLR
    "MRC     p15, 0, r0, c1, c0, 1                   \n" // Read CP15 Auxiliary Control Register
    "ORR     r0, r0, #(1 <<  1)                      \n" // Enable L2 prefetch hint (UNK/WI since r4p1)
    "MCR     p15, 0, r0, c1, c0, 1                   \n" // Write CP15 Auxiliary Control Register

    // Set Vector Base Address Register (VBAR) to point to this application's vector table
    "LDR    R0, =Vectors                             \n"
    "MCR    p15, 0, R0, c12, c0, 0                   \n"

    // Setup Stack for each exceptional mode
    "CPS    #0x11                                    \n" // FIQ
    "LDR    SP, =Image$$FIQ_STACK$$ZI$$Limit         \n"
    "CPS    #0x12                                    \n" // IRQ
    "LDR    SP, =Image$$IRQ_STACK$$ZI$$Limit         \n"
    "CPS    #0x13                                    \n" // SVC
    "LDR    SP, =Image$$SVC_STACK$$ZI$$Limit         \n"
    "CPS    #0x17                                    \n" // ABT
    "LDR    SP, =Image$$ABT_STACK$$ZI$$Limit         \n"
    "CPS    #0x1B                                    \n" // UND
    "LDR    SP, =Image$$UND_STACK$$ZI$$Limit         \n"
    "CPS    #0x1F                                    \n" // SYS
    "LDR    SP, =Image$$SYS_STACK$$ZI$$Limit         \n"
    "CPS    #0x13                                    \n" // SVC

    // Call DataInit
    "BL     DataInit                                 \n"

    // Call SystemInit
    "BL     SystemInit                               \n"

    // Unmask interrupts
    "CPSIE  if                                       \n"

    // Call _start
    "BL     _start                                   \n"
    );
}

/*----------------------------------------------------------------------------
  Default Handler for Exceptions / Interrupts
 *----------------------------------------------------------------------------*/
void Default_Handler(void)
{
    while(1);
}

#if defined(__GNUC__) && ! defined(__ARMCC_VERSION)
#pragma GCC pop_options
#elif defined(__ARMCC_VERSION)
#pragma pop
#endif

#endif /* HAL_AP_CORE */
