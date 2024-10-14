/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

.syntax	unified
.cpu	cortex-m3
.fpu	softvfp
.thumb

.global    __Vectors

.extern    __stack_start
.extern    __bss_start
.extern    __bss_end
.extern    SystemInit

.equ    CRU_SOFTRST4_CON,       0x44500310
.equ    GRF_DSP_CON0,           0x44300180
/**
 * Vector table
 */
	.section .isr_vector,"a",%progbits
	.type __Vectors, %object
	.size __Vectors, .-__Vectors
__Vectors:
        .long    __stack_start                      /*     Top of Stack          */
        .long    Reset_Handler                      /*     Reset Handler         */
        .long    Default_Handler                    /* -14 NMI Handler           */
        .long    HardFault_Handler                  /* -13 Hard Fault Handler    */
        .long    Default_Handler                    /* -12 MPU Fault Handler     */
        .long    Default_Handler                    /* -11 Bus Fault Handler     */
        .long    Default_Handler                    /* -10 Usage Fault Handler   */
        .long    0                                  /*     Reserved              */
        .long    0                                  /*     Reserved              */
        .long    0                                  /*     Reserved              */
        .long    0                                  /*     Reserved              */
        .long    SVC_Handler                        /*  -5 SVCall Handler        */
        .long    Default_Handler                    /*  -4 Debug Monitor Handler */
        .long    0                                  /*     Reserved              */
        .long    PendSV_Handler                     /*  -2 PendSV Handler        */
        .long    SysTick_Handler                    /*  -1 SysTick Handler       */

        /* Interrupts */
        .long    Default_Handler                    /*   0 Interrupt 0           */
        .long    Default_Handler                    /*   1 Interrupt 1           */
        .long    Default_Handler                    /*   2 Interrupt 2           */
        .long    Default_Handler                    /*   3 Interrupt 3           */
        .long    Default_Handler                    /*   4 Interrupt 4           */
        .long    Default_Handler                    /*   5 Interrupt 5           */
        .long    Default_Handler                    /*   6 Interrupt 6           */
        .long    Default_Handler                    /*   7 Interrupt 7           */
        .long    Default_Handler                    /*   8 Interrupt 8           */
        .long    Default_Handler                    /*   9 Interrupt 9           */

       .space    (47 * 4)                            /*   10 ... 56               */
__Vectors_End:
        .equ     __Vectors_Size, __Vectors_End - __Vectors
        .size    __Vectors, . - __Vectors

/**
 * Reset_Handler
 */
        .section .text.Reset_Handler
        .type    Reset_Handler, %function
        .globl   Reset_Handler
Reset_Handler:
        /* Deassert reset for dsp */
        ldr     r0, =0x00200020
        ldr     r1, =GRF_DSP_CON0
        str     r0, [r1]
        ldr     r0, =0x00060000
        ldr     r1, =CRU_SOFTRST4_CON
        str     r0, [r1]

        /* clear the bss segment */
        ldr     r0, =__bss_start
        ldr     r1, =__bss_end
        mov     r2, #0
zero_loop:
        cmp     r0, r1
        it      lt
        strlt   r2, [r0], #4
        blt     zero_loop

        /* System init */
        bl      SystemInit
        b       main
.size Reset_Handler, .-Reset_Handler

/**
 * Default_Handler
 */
        .section .text.Default_Handler,"ax",%progbits
Default_Handler:
        B       Default_Handler
.size Default_Handler, .-Default_Handler


        .type cm_backtrace_fault, %function
        .weak cm_backtrace_fault
cm_backtrace_fault:
        b .
/**
 * trap HardFault by cm_backtrace
 */
        .section .text.HardFault_Handler,"ax",%progbits
        .type HardFault_Handler, %function
HardFault_Handler:
        mov	r0, lr                  /* get lr */
        mov     r1, sp                  /* get stack pointer (current is MSP) */
        bl cm_backtrace_fault

Fault_Loop:
        bl      Fault_Loop              /* while(1) */
/**
 * Macro to define default exception/interrupt handlers.
 * Default handler are weak symbols with an endless loop.
 * They can be overwritten by real handlers.
 */
        .macro   Set_Default_Handler  Handler_Name
        .weak    \Handler_Name
        .set     \Handler_Name, Default_Handler
        .endm

/* Default exception/interrupt handler */
Set_Default_Handler  SVC_Handler
Set_Default_Handler  PendSV_Handler
Set_Default_Handler  SysTick_Handler
