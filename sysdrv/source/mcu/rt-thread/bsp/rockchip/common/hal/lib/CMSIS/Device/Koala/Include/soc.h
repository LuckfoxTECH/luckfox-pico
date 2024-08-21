/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#ifndef __SOC_H
#define __SOC_H
#ifdef __cplusplus
  extern "C" {
#endif
#ifndef __ASSEMBLY__
/* ================================================================================ */
/* ================                    DMA REQ                      =============== */
/* ================================================================================ */
typedef enum {
    DMA_REQ_BT_TEST_RX = 0,
    DMA_REQ_I2S0_TX    = 1,
    DMA_REQ_I2S0_RX    = 2,
    DMA_REQ_UART0_TX   = 3,
    DMA_REQ_UART0_RX   = 4,
    DMA_REQ_UART1_TX   = 5,
    DMA_REQ_UART1_RX   = 6,
    DMA_REQ_SPI1_TX    = 7,
    DMA_REQ_SPI1_RX    = 8,
    DMA_REQ_PWM        = 9,
    DMA_REQ_PDM0       = 10,
    DMA_REQ_BT_TEST_TX = 11,
} DMA_REQ_Type;

/* ================================================================================ */
/* ================                      IRQ                      ================= */
/* ================================================================================ */
typedef enum
{
    /******  Cortex-M3 Processor Exceptions Numbers *********************************/
    NonMaskableInt_IRQn       = -14,       /*  2 Non Maskable Interrupt             */
    HardFault_IRQn            = -13,       /*  3 HardFault Interrupt                */
    MemoryManagement_IRQn     = -12,       /*  4 Memory Management Interrupt        */
    BusFault_IRQn             = -11,       /*  5 Bus Fault Interrupt                */
    UsageFault_IRQn           = -10,       /*  6 Usage Fault Interrupt              */
    SVCall_IRQn               = -5,        /*  11 SV Call Interrupt                 */
    DebugMonitor_IRQn         = -4,        /*  12 Debug Monitor Interrupt           */
    PendSV_IRQn               = -2,        /*  14 Pend SV Interrupt                 */
    SysTick_IRQn              = -1,        /*  15 System Tick Interrupt             */

    /******  Koala specific Interrupt Numbers ***************************************/
    PDM0_IRQn                 = 0,
    GPIO2_IRQn                = 1,
    GPIO1_IRQn                = 2,
    GPIO0_IRQn                = 3,
    I2S0_IRQn                 = 4,
    DSP_PFATAL_ERROR_IRQn     = 5,
    MAILBOX_CM0_IRQn          = 6,
    MAILBOX_CM1_IRQn          = 7,
    MAILBOX_CM2_IRQn          = 8,
    MAILBOX_CM3_IRQn          = 9,
    MAILBOX_CA0_IRQn          = 10,
    MAILBOX_CA1_IRQn          = 11,
    MAILBOX_CA2_IRQn          = 12,
    MAILBOX_CA3_IRQn          = 13,
    I2CMST2_IRQn              = 14,
    I2CMST1_IRQn              = 15,
    I2CMST0_IRQn              = 16,
    SPIMST1_IRQn              = 17,
    WDT1_IRQn                 = 18,
    WDT0_IRQn                 = 19,
    UART1_IRQn                = 20,
    UART0_IRQn                = 21,
    TIMER4_IRQn               = 22,
    TIMER3_IRQn               = 23,
    TIMER2_IRQn               = 24,
    TIMER1_IRQn               = 25,
    TIMER0_IRQn               = 26,
    PWM_IRQn                  = 27,
    DMAC_ABORT_IRQn           = 28,
    DMAC_IRQn                 = 29,
    FSPI0_IRQn                = 30,
    EFUSE_IRQn                = 31,
    PMU_IRQn                  = 32,
    BLE_IRQn                  = 33,
    NUM_INTERRUPTS
}IRQn_Type;
#endif /* __ASSEMBLY__ */

#define NVIC_PERIPH_IRQ_NUM MAX_IRQn
#define NVIC_PERIPH_IRQ_OFFSET 16

#define MAILBOX0_AP_IRQ0 MAILBOX_CA0_IRQn
#define MAILBOX0_AP_IRQ1 MAILBOX_CA1_IRQn
#define MAILBOX0_AP_IRQ2 MAILBOX_CA2_IRQn
#define MAILBOX0_AP_IRQ3 MAILBOX_CA3_IRQn

/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */

#define __CM3_REV                 0x0200U  /* Core revision r2p0                            */
#define __VTOR_PRESENT            1U       /* VTOR present                                  */
#define __NVIC_PRIO_BITS          3U       /* uses 3 Bits for the Priority Levels           */
#define __Vendor_SysTickConfig    0U       /* Set to 1 if different SysTick Config is used  */

#ifndef __ASSEMBLY__
#include "core_cm3.h"                      /* Processor and core peripherals                */
#include "system_koala.h"                  /* System Header                                 */
#endif /* __ASSEMBLY__ */
#include "koala.h"

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __SOC_H */
