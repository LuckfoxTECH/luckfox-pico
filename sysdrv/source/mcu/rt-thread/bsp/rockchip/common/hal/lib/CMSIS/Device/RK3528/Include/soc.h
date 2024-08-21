/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#ifndef __SOC_H
#define __SOC_H
#ifdef __cplusplus
  extern "C" {
#endif

/* IO definitions (access restrictions to peripheral registers) */
#ifdef __cplusplus
  #define   __I     volatile             /*!< brief Defines 'read only' permissions    */
#else
  #define   __I     volatile const       /*!< brief Defines 'read only' permissions    */
#endif
#define     __O     volatile             /*!< brief Defines 'write only' permissions   */
#define     __IO    volatile             /*!< brief Defines 'read / write' permissions */

/* ================================================================================ */
/* ================                    DMA REQ                      =============== */
/* ================================================================================ */
typedef enum {
    DMA_REQ_I2S0_RX  = 0,
    DMA_REQ_I2S0_TX  = 1,
    DMA_REQ_I2S1_RX  = 2,
    DMA_REQ_I2S1_TX  = 3,
    DMA_REQ_I2S2_TX  = 4,
    DMA_REQ_I2S3_TX  = 5,
    DMA_REQ_PDM      = 6,
    DMA_REQ_SPDIF    = 7,
    DMA_REQ_UART0_RX = 8,
    DMA_REQ_UART0_TX = 9,
    DMA_REQ_UART1_RX = 10,
    DMA_REQ_UART1_TX = 11,
    DMA_REQ_UART2_RX = 12,
    DMA_REQ_UART2_TX = 13,
    DMA_REQ_UART3_RX = 14,
    DMA_REQ_UART3_TX = 15,
    DMA_REQ_UART4_RX = 16,
    DMA_REQ_UART4_TX = 17,
    DMA_REQ_UART5_RX = 18,
    DMA_REQ_UART5_TX = 19,
    DMA_REQ_UART6_RX = 20,
    DMA_REQ_UART6_TX = 21,
    DMA_REQ_UART7_RX = 22,
    DMA_REQ_UART7_TX = 23,
    DMA_REQ_SPI0_RX  = 24,
    DMA_REQ_SPI0_TX  = 25,
    DMA_REQ_PWM0     = 26,
    DMA_REQ_PWM1     = 27,
    DMA_REQ_CAN0_RX  = 28,
    DMA_REQ_CAN0_TX  = 29,
    DMA_REQ_SPI1_RX  = 30,
    DMA_REQ_SPI1_TX  = 31,
} DMA_REQ_Type;

/* ================================================================================ */
/* ================                       IRQ                      ================ */
/* ================================================================================ */
#define INTMUX_NUM_INT_PER_CON    256
#define INTMUX_NUM_OUT_PER_CON    4
#define INTMUX_NUM_INT_PER_OUT    64
#define INTMUX_NUM_GROUP_PER_CON  8
#define INTMUX_NUM_INT_PER_GROUP  32

#define NUM_EXT_INTERRUPTS        256

typedef enum {
/* -------------------  Processor Exceptions Numbers  ----------------------------- */
  NonMaskableInt_IRQn       = -14,     /*  2 Non Maskable Interrupt */
  HardFault_IRQn            = -13,     /*  3 HardFault Interrupt */

  SVCall_IRQn               =  -5,     /* 11 SV Call Interrupt */

  PendSV_IRQn               =  -2,     /* 14 Pend SV Interrupt */
  SysTick_IRQn              =  -1,     /* 15 System Tick Interrupt */

/******  Platform Exceptions Numbers ***************************************************/
  GPIO0_IRQn                =  0,      /*!< GPIO0 Interrupt               */
  GPIO0_EXP_IRQn            =  1,      /*!< GPIO0 EXP Interrupt           */
  HPTIMER_IRQn              =  2,      /*!< HPTIMER Interrupt             */
  PMUPVTM_IRQn              =  3,      /*!< PMUPVTM Interrupt             */
  I2C2_IRQn                 =  4,      /*!< I2C2 Interrupt                */
  PMUWDT_IRQn               =  5,      /*!< PMUWDT Interrupt              */
  MAILBOX_AP_IRQn           =  6,      /*!< MAILBOX AP Interrupt          */
  MAILBOX_BB_IRQn           =  7,      /*!< MAILBOX BB Interrupt          */
  RKNPORPOWERGOOD_IRQn      =  8,      /*!< RKNPORPOWERGOOD Interrupt     */
  OSC_CHK_RST_IRQn          =  9,      /*!< OSC CHK RST Interrupt         */
  CRC_CHK_FAIL_IRQn         =  10,     /*!< CRC CHK FAIL Interrupt        */
  INTMUX_OUT0_IRQn          =  11,     /*!< INTMUX OUT0 Interrupt         */
  INTMUX_OUT1_IRQn          =  12,     /*!< INTMUX OUT1 Interrupt         */
  INTMUX_OUT2_IRQn          =  13,     /*!< INTMUX OUT2 Interrupt         */
  INTMUX_OUT3_IRQn          =  14,     /*!< INTMUX OUT3 Interrupt         */
  RSVD0_MCU_IRQn            =  15,     /*!< RSVD0 MCU Interrupt         */
  NUM_INTERRUPTS            =  32,     /*!< Number of internal IRQ        */
  TIMER0_IRQn               =  63  + NUM_INTERRUPTS,                    /*!< TIMER0 Interrupt              */
  TIMER1_IRQn               =  64  + NUM_INTERRUPTS,                    /*!< TIMER1 Interrupt              */
  TIMER2_IRQn               =  65  + NUM_INTERRUPTS,                    /*!< TIMER2 Interrupt              */
  TIMER3_IRQn               =  66  + NUM_INTERRUPTS,                    /*!< TIMER3 Interrupt              */
  TIMER4_IRQn               =  67  + NUM_INTERRUPTS,                    /*!< TIMER4 Interrupt              */
  TIMER5_IRQn               =  68  + NUM_INTERRUPTS,                    /*!< TIMER5 Interrupt              */
  UART0_IRQn                =  72  + NUM_INTERRUPTS,                    /*!< UART0 Interrupt               */
  UART1_IRQn                =  73  + NUM_INTERRUPTS,                    /*!< UART1 Interrupt               */
  UART2_IRQn                =  74  + NUM_INTERRUPTS,                    /*!< UART2 Interrupt               */
  UART3_IRQn                =  75  + NUM_INTERRUPTS,                    /*!< UART3 Interrupt               */
  UART4_IRQn                =  76  + NUM_INTERRUPTS,                    /*!< UART4 Interrupt               */
  UART5_IRQn                =  77  + NUM_INTERRUPTS,                    /*!< UART5 Interrupt               */
  UART6_IRQn                =  78  + NUM_INTERRUPTS,                    /*!< UART6 Interrupt               */
  UART7_IRQn                =  79  + NUM_INTERRUPTS,                    /*!< UART7 Interrupt               */
  TOTAL_INTERRUPTS          =  (NUM_INTERRUPTS + NUM_EXT_INTERRUPTS),   /*!< For external interrupt from intmux    */
} IRQn_Type;

#define RSVD_MCU_IRQn(_N)               (RSVD0_MCU_IRQn + (_N))

#define HAS_CUSTOME_INTC

/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */

#define PLL_INPUT_OSC_RATE       (24 * 1000 * 1000)

/* --------  Configuration of Core Peripherals  ----------------------------------- */
#if defined(HAL_AP_CORE) && defined(HAL_MCU_CORE)
#error "HAL_AP_CORE and HAL_MCU_CORE only one of them can be enabled"
#endif

#if !defined(HAL_AP_CORE) && !defined(HAL_MCU_CORE)
#error "Please define HAL_AP_CORE or HAL_MCU_CORE on hal_conf.h"
#endif

#ifdef HAL_MCU_CORE
#define __CM0_REV                 0x0000U   /* Core revision r0p0 */
#define __MPU_PRESENT             0U        /* no MPU present */
#define __VTOR_PRESENT            0U        /* no VTOR present */
#define __NVIC_PRIO_BITS          2U        /* Number of Bits used for Priority Levels */
#define __Vendor_SysTickConfig    0U        /* Set to 1 if different SysTick Config is used */

#define NVIC_PERIPH_IRQ_OFFSET    16U
#define MAX_INTERRUPT_VECTOR      64U
#endif

#ifndef __ASSEMBLY__
#ifdef HAL_MCU_CORE
#include "core_cm0.h"
#endif
#include "system_rk3528.h"
#endif /* __ASSEMBLY__ */
#include "rk3528.h"

/****************************************************************************************/
/*                                                                                      */
/*                               Register Bitmap Section                                */
/*                                                                                      */
/****************************************************************************************/

/****************************************************************************************/
/*                                                                                      */
/*                           Platform Differences Section                               */
/*                                                                                      */
/****************************************************************************************/

/******************************************CRU*******************************************/
#define CRU_CLK_USE_CON_BANK
#define CLK64(mux, div) ((((mux) & 0xffffffffULL) << 32) | ((div) & 0xffffffffULL))

#ifndef __ASSEMBLY__
typedef enum CLOCK_Name {
    /* TODO */
    CLK_INVALID = 0U,
} eCLOCK_Name;
#endif /* __ASSEMBLY__ */

/****************************************GPIO********************************************/
#ifdef GPIO_VER_ID
#undef GPIO_VER_ID
#define GPIO_VER_ID             (0x01000C2BU)
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __SOC_H */
