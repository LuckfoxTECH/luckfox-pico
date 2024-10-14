/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#ifndef __SOC_H
#define __SOC_H
#ifdef __cplusplus
  extern "C" {
#endif

#include "hal_conf.h"

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
    DMA_REQ_I2S0_8CH_TX = 0,
    DMA_REQ_I2S2_2CH_TX = 0,
    DMA_REQ_I2S4_8CH_TX = 0,
    DMA_REQ_I2S0_8CH_RX = 1,
    DMA_REQ_I2S2_2CH_RX = 1,
    DMA_REQ_I2S1_8CH_TX = 2,
    DMA_REQ_I2S3_2CH_TX = 2,
    DMA_REQ_I2S5_8CH_TX = 2,
    DMA_REQ_I2S1_8CH_RX = 3,
    DMA_REQ_I2S3_2CH_RX = 3,
    DMA_REQ_PDM0        = 4,
    DMA_REQ_PDM1        = 4,
    DMA_REQ_I2S6_8CH_TX = 4,
    DMA_REQ_SPDIF0_TX   = 5,
    DMA_REQ_SPDIF1_TX   = 5,
    DMA_REQ_UART0_TX    = 6,
    DMA_REQ_SPDIF2_TX   = 6,
    DMA_REQ_UART0_RX    = 7,
    DMA_REQ_SPDIF3_TX   = 7,
    DMA_REQ_UART7_TX    = 7,
    DMA_REQ_UART1_TX    = 8,
    DMA_REQ_SPDIF4_TX   = 8,
    DMA_REQ_UART7_RX    = 8,
    DMA_REQ_UART1_RX    = 9,
    DMA_REQ_UART4_TX    = 9,
    DMA_REQ_UART8_TX    = 9,
    DMA_REQ_UART2_TX    = 10,
    DMA_REQ_UART4_RX    = 10,
    DMA_REQ_UART8_RX    = 10,
    DMA_REQ_UART2_RX    = 11,
    DMA_REQ_UART5_TX    = 11,
    DMA_REQ_UART9_TX    = 11,
    DMA_REQ_UART3_TX    = 12,
    DMA_REQ_UART5_RX    = 12,
    DMA_REQ_UART9_RX    = 12,
    DMA_REQ_UART3_RX    = 13,
    DMA_REQ_UART6_TX    = 13,
    DMA_REQ_SPI4_TX     = 13,
    DMA_REQ_SPI0_TX     = 14,
    DMA_REQ_UART6_RX    = 14,
    DMA_REQ_SPI4_RX     = 14,
    DMA_REQ_SPI0_RX     = 15,
    DMA_REQ_SPI2_TX     = 15,
    DMA_REQ_PWM2        = 15,
    DMA_REQ_SPI1_TX     = 16,
    DMA_REQ_SPI2_RX     = 16,
    DMA_REQ_PWM3        = 16,
    DMA_REQ_SPI1_RX     = 17,
    DMA_REQ_SPI3_TX     = 17,
    DMA_REQ_CAN2_TX     = 17,
    DMA_REQ_PWM0        = 18,
    DMA_REQ_SPI3_RX     = 18,
    DMA_REQ_CAN2_RX     = 18,
    DMA_REQ_CAN0_TX     = 19,
    DMA_REQ_PWM1        = 19,
    DMA_REQ_SDMMC       = 19,
    DMA_REQ_CAN0_RX     = 20,
    DMA_REQ_CAN1_TX     = 20,
    DMA_REQ_SPDIF0_RX   = 21,
    DMA_REQ_CAN1_RX     = 21,
    DMA_REQ_I2S7_8CH_RX = 21,
    DMA_REQ_SPDIF1_RX   = 22,
    DMA_REQ_SPDIF5_TX   = 22,
    DMA_REQ_I2S8_8CH_TX = 22,
    DMA_REQ_SPDIF2_RX   = 23,
    DMA_REQ_I29_8CH_RX  = 23,
    DMA_REQ_I210_8CH_RX = 24,
} DMA_REQ_Type;

/* ================================================================================ */
/* ================                       IRQ                      ================ */
/* ================================================================================ */
#if defined(HAL_MCU_CORE)
#if defined(RKMCU_RK3588_PMU)
#define INTMUX_NUM_INT_PER_CON    256
#define INTMUX_NUM_OUT_PER_CON    4
#define INTMUX_NUM_INT_PER_OUT    64
#define INTMUX_NUM_GROUP_PER_OUT  8
#define INTMUX_NUM_GROUP_PER_CON  32
#define INTMUX_NUM_INT_PER_GROUP  8
/* INTMUX IRQ start from GIC SPI(Shared Peripheral Interrupt) */
#define INTMUX_IRQ_START_NUM      0
#define INTMUX_IRQ_OUT0
#define INTMUX_IRQ_OUT1
#define INTMUX_IRQ_OUT2
#define INTMUX_IRQ_OUT3
#define INTMUX_IRQ_OUT4
#define INTMUX_IRQ_OUT5
#define INTMUX_IRQ_OUT6
#define INTMUX_IRQ_OUT7
#define INTMUX_OUT_IRQ_START_NUM  16

#define NUM_EXT_INTERRUPTS        512

typedef enum {
/* -------------------  Processor Exceptions Numbers  ----------------------------- */
  NonMaskableInt_IRQn       = -14,     /*  2 Non Maskable Interrupt */
  HardFault_IRQn            = -13,     /*  3 HardFault Interrupt */



  SVCall_IRQn               =  -5,     /* 11 SV Call Interrupt */

  PendSV_IRQn               =  -2,     /* 14 Pend SV Interrupt */
  SysTick_IRQn              =  -1,     /* 15 System Tick Interrupt */

/******  Platform Exceptions Numbers ***************************************************/
  PMIC_IRQn                 =  0,      /*!< PMIC Interrupt                */
  SDMMC_DETECTN_IRQn        =  1,      /*!< SDMMC Interrupt               */
  UART0_IRQn                =  2,      /*!< UART0 Interrupt               */
  GPIO0_IRQn                =  3,      /*!< GPIO0 Interrupt               */
  GPIO0_EXP_IRQn            =  4,      /*!< GPIO0 EXP Interrupt           */
  I2C0_IRQn                 =  5,      /*!< I2C0 Interrupt                */
  VAD_IRQn                  =  6,      /*!< VAD Interrupt                 */
  PDM0_IRQn                 =  7,      /*!< PDM0 Interrupt                */
  I2S1_IRQn                 =  8,      /*!< I2S1 Interrupt                */
  PVTM_IRQn                 =  9,      /*!< PVTM Interrupt                */
  PWM0_PWR_IRQn             =  10,     /*!< PWM0 PWR Interrupt            */
  PWM0_IRQn                 =  11,     /*!< PWM0 Interrupt                */
  WDT_PMU_IRQn              =  12,     /*!< WDT PMU Interrupt             */
  TIMER1_PMU_IRQn           =  13,     /*!< TIMER1 PMU Interrupt          */
  TIMER0_PMU_IRQn           =  14,     /*!< TIMER0 PMU Interrupt          */
  CRC_CHK_RST_IRQn          =  15,     /*!< CRC_CHK_RST Interrupt         */
  INTMUX_OUT0_IRQn          =  16,     /*!< INTMUX OUT0 Interrupt         */
  INTMUX_OUT1_IRQn          =  17,     /*!< INTMUX OUT1 Interrupt         */
  INTMUX_OUT2_IRQn          =  18,     /*!< INTMUX OUT2 Interrupt         */
  INTMUX_OUT3_IRQn          =  19,     /*!< INTMUX OUT3 Interrupt         */
  INTMUX_OUT4_IRQn          =  20,     /*!< INTMUX OUT4 Interrupt         */
  INTMUX_OUT5_IRQn          =  21,     /*!< INTMUX OUT5 Interrupt         */
  INTMUX_OUT6_IRQn          =  22,     /*!< INTMUX OUT6 Interrupt         */
  INTMUX_OUT7_IRQn          =  23,     /*!< INTMUX OUT7 Interrupt         */
  HPTIMER_PMU0_IRQn         =  24,     /*!< HPTIMER_PMU0 Interrupt        */
  OSC_CHK_RST_IRQn          =  25,     /*!< OSC_CHK_RST Interrupt         */
  CACHE_IRQn                =  26,     /*!< CACHE Interrupt               */
  RSVD0_MCU_IRQn            =  27,     /*!< RSVD0 MCU Interrupt           */
  NUM_INTERRUPTS            =  32,     /*!< Number of internal IRQ        */
  MBOX0_CH0_BB_IRQn         =  97  + NUM_INTERRUPTS,     /*!< MAILBOX_CH0_BB Interrupt      */
  MBOX0_CH1_BB_IRQn         =  98  + NUM_INTERRUPTS,     /*!< MAILBOX_CH1_BB Interrupt      */
  MBOX0_CH2_BB_IRQn         =  99  + NUM_INTERRUPTS,     /*!< MAILBOX_CH2_BB Interrupt      */
  MBOX0_CH3_BB_IRQn         =  100 + NUM_INTERRUPTS,     /*!< MAILBOX_CH3_BB Interrupt      */
  GPIO1_IRQn                =  310 + NUM_INTERRUPTS,     /*!< GPIO1 Interrupt               */
  GPIO2_IRQn                =  311 + NUM_INTERRUPTS,     /*!< GPIO2 Interrupt               */
  GPIO3_IRQn                =  312 + NUM_INTERRUPTS,     /*!< GPIO3 Interrupt               */
  GPIO4_IRQn                =  313 + NUM_INTERRUPTS,     /*!< GPIO4 Interrupt               */
  GPIO1_EXP_IRQn            =  315 + NUM_INTERRUPTS,     /*!< GPIO1 EXP Interrupt           */
  GPIO2_EXP_IRQn            =  316 + NUM_INTERRUPTS,     /*!< GPIO2 EXP Interrupt           */
  GPIO3_EXP_IRQn            =  317 + NUM_INTERRUPTS,     /*!< GPIO3 EXP Interrupt           */
  GPIO4_EXP_IRQn            =  318 + NUM_INTERRUPTS,     /*!< GPIO4 EXP Interrupt           */
  TIMER0_IRQn               =  321 + NUM_INTERRUPTS,     /*!< TIMER0 Interrupt              */
  TIMER1_IRQn               =  322 + NUM_INTERRUPTS,     /*!< TIMER1 Interrupt              */
  TIMER2_IRQn               =  323 + NUM_INTERRUPTS,     /*!< TIMER2 Interrupt              */
  TIMER3_IRQn               =  324 + NUM_INTERRUPTS,     /*!< TIMER3 Interrupt              */
  TIMER4_IRQn               =  325 + NUM_INTERRUPTS,     /*!< TIMER4 Interrupt              */
  TIMER5_IRQn               =  326 + NUM_INTERRUPTS,     /*!< TIMER5 Interrupt              */
  TIMER6_IRQn               =  327 + NUM_INTERRUPTS,     /*!< TIMER6 Interrupt              */
  TIMER7_IRQn               =  328 + NUM_INTERRUPTS,     /*!< TIMER7 Interrupt              */
  TIMER8_IRQn               =  329 + NUM_INTERRUPTS,     /*!< TIMER8 Interrupt              */
  TIMER9_IRQn               =  330 + NUM_INTERRUPTS,     /*!< TIMER9 Interrupt              */
  TIMER10_IRQn              =  331 + NUM_INTERRUPTS,     /*!< TIMER10 Interrupt             */
  TIMER11_IRQn              =  332 + NUM_INTERRUPTS,     /*!< TIMER11 Interrupt             */
  UART1_IRQn                =  364 + NUM_INTERRUPTS,     /*!< UART1 Interrupt               */
  UART2_IRQn                =  365 + NUM_INTERRUPTS,     /*!< UART2 Interrupt               */
  UART3_IRQn                =  366 + NUM_INTERRUPTS,     /*!< UART3 Interrupt               */
  UART4_IRQn                =  367 + NUM_INTERRUPTS,     /*!< UART4 Interrupt               */
  UART5_IRQn                =  368 + NUM_INTERRUPTS,     /*!< UART5 Interrupt               */
  UART6_IRQn                =  369 + NUM_INTERRUPTS,     /*!< UART6 Interrupt               */
  UART7_IRQn                =  370 + NUM_INTERRUPTS,     /*!< UART7 Interrupt               */
  UART8_IRQn                =  371 + NUM_INTERRUPTS,     /*!< UART8 Interrupt               */
  UART9_IRQn                =  372 + NUM_INTERRUPTS,     /*!< UART9 Interrupt               */
  TOTAL_INTERRUPTS          =  (INTMUX_IRQ_START_NUM + NUM_INTERRUPTS + NUM_EXT_INTERRUPTS),
} IRQn_Type;
#elif defined(RKMCU_RK3588_NPU)
typedef enum {
/* -------------------  Processor Exceptions Numbers  ----------------------------- */
  NonMaskableInt_IRQn       = -14,     /*  2 Non Maskable Interrupt */
  HardFault_IRQn            = -13,     /*  3 HardFault Interrupt */



  SVCall_IRQn               =  -5,     /* 11 SV Call Interrupt */

  PendSV_IRQn               =  -2,     /* 14 Pend SV Interrupt */
  SysTick_IRQn              =  -1,     /* 15 System Tick Interrupt */

/******  Platform Exceptions Numbers ***************************************************/
  TIMER0_IRQn               =  0,      /*!< TIMER0 Interrupt              */
  TIMER1_IRQn               =  1,      /*!< TIMER1 Interrupt              */
  PVTM_IRQn                 =  2,      /*!< PVTM Interrupt                */
  WDT0_IRQn                 =  3,      /*!< WDT Interrupt                 */
  C0_RKNN_IRQn              =  4,      /*!< C0_RKNN Interrupt             */
  C1_RKNN_IRQn              =  5,      /*!< C1_RKNN Interrupt             */
  C2_RKNN_IRQn              =  6,      /*!< C2_RKNN Interrupt             */
  CACHE_IRQn                =  7,      /*!< CACHE Interrupt               */
  MBOX0_CH0_AP_IRQn         =  8,      /*!< MAILBOX_CH0_AP Interrupt      */
  MBOX0_CH1_AP_IRQn         =  9,      /*!< MAILBOX_CH1_AP Interrupt      */
  MBOX0_CH2_AP_IRQn         =  10,     /*!< MAILBOX_CH2_AP Interrupt      */
  MBOX0_CH3_AP_IRQn         =  11,     /*!< MAILBOX_CH3_AP Interrupt      */
  MBOX0_CH0_BB_IRQn         =  12,     /*!< MAILBOX_CH0_BB Interrupt      */
  MBOX0_CH1_BB_IRQn         =  13,     /*!< MAILBOX_CH1_BB Interrupt      */
  MBOX0_CH2_BB_IRQn         =  14,     /*!< MAILBOX_CH2_BB Interrupt      */
  MBOX0_CH3_BB_IRQn         =  15,     /*!< MAILBOX_CH3_BB Interrupt      */
  NUM_INTERRUPTS            =  16,     /*!< Number of internal IRQ        */
  DUMMY_IRQn                =  256,    /*!< Avoid compile warning: overflow in conversion   */
} IRQn_Type;
#elif defined(RKMCU_RK3588_DDR)
typedef enum
{
/* -------------------  Processor Exceptions Numbers  ----------------------------- */
  NonMaskableInt_IRQn       = -14,     /*  2 Non Maskable Interrupt */
  HardFault_IRQn            = -13,     /*  3 HardFault Interrupt */



  SVCall_IRQn               =  -5,     /* 11 SV Call Interrupt */

  PendSV_IRQn               =  -2,     /* 14 Pend SV Interrupt */
  SysTick_IRQn              =  -1,     /* 15 System Tick Interrupt */

/******  Platform Exceptions Numbers ***************************************************/
  INTMUX_OUT_START_IRQn     =  0,     /*!< INTMUX_OUT_START Interrupt    */
  INTMUX_OUT_END_IRQn       =  7,     /*!< INTMUX_OUT_END Interrupt      */
  NUM_INTERRUPTS            =  27,     /*!< Number of internal IRQ        */
  MBOX0_CH0_AP_IRQn         =  101  + NUM_INTERRUPTS,     /*!< MAILBOX_CH0_AP Interrupt      */
  MBOX0_CH1_AP_IRQn         =  102  + NUM_INTERRUPTS,     /*!< MAILBOX_CH1_AP Interrupt      */
  MBOX0_CH2_AP_IRQn         =  103  + NUM_INTERRUPTS,     /*!< MAILBOX_CH2_AP Interrupt      */
  MBOX0_CH3_AP_IRQn         =  104  + NUM_INTERRUPTS,     /*!< MAILBOX_CH3_AP Interrupt      */
  MBOX0_CH0_BB_IRQn         =  105  + NUM_INTERRUPTS,     /*!< MAILBOX_CH0_BB Interrupt      */
  MBOX0_CH1_BB_IRQn         =  106  + NUM_INTERRUPTS,     /*!< MAILBOX_CH1_BB Interrupt      */
  MBOX0_CH2_BB_IRQn         =  107  + NUM_INTERRUPTS,     /*!< MAILBOX_CH2_BB Interrupt      */
  MBOX0_CH3_BB_IRQn         =  108  + NUM_INTERRUPTS,     /*!< MAILBOX_CH3_BB Interrupt      */
  WDT0_IRQn                 =  433  + NUM_INTERRUPTS,     /*!< WDT Interrupt         */
  TIMER0_IRQn               =  434  + NUM_INTERRUPTS,     /*!< TIMER0 Interrupt      */
  TIMER1_IRQn               =  435  + NUM_INTERRUPTS,     /*!< TIMER1 Interrupt      */
  TOTAL_INTERRUPTS          =  (NUM_INTERRUPTS + NUM_EXT_INTERRUPTS),   /*!< For external interrupt from intmux    */
} IRQn_Type;
#else
#error missing IRQn_Type define for interrupt
#endif

#if defined(RKMCU_RK3588_PMU) || defined(RKMCU_RK3588_DDR)
#define HAS_CUSTOME_INTC
#endif
#endif /* HAL_MCU_CORE */

#if defined(HAL_AP_CORE)
typedef enum
{
/* When IPI_SGIs are used in AMP mode, you need to pay attention to whether it conflicts
 * with SMP mode. Especially in the case of Linux OS as The Master Core.
 * IPI_SGI 0~7 for non-secure and IPI_SGI 8~15 for secure.
 */
    IPI_SGI0               = 0,
    IPI_SGI1               = 1,
    IPI_SGI2               = 2,
    IPI_SGI3               = 3,
    IPI_SGI4               = 4,
    IPI_SGI5               = 5,
    IPI_SGI6               = 6,
    IPI_SGI7               = 7,
    IPI_SGI8               = 8,
    IPI_SGI9               = 9,
    IPI_SGI10              = 10,
    IPI_SGI11              = 11,
    IPI_SGI12              = 12,
    IPI_SGI13              = 13,
    IPI_SGI14              = 14,
    IPI_SGI15              = 15,

    CNTHP_IRQn             = 26,
    CNTV_IRQn              = 27,
    CNTPS_IRQn             = 29,
    CNTPNS_IRQn            = 30,

/******  Platform Exceptions Numbers ***************************************************/
    MBOX0_CH0_B2A_IRQn     = 93,       /*!< MBOX0 CH0 B2A Interrupt     */
    MBOX0_CH1_B2A_IRQn     = 94,       /*!< MBOX0 CH1 B2A Interrupt     */
    MBOX0_CH2_B2A_IRQn     = 95,       /*!< MBOX0 CH2 B2A Interrupt     */
    MBOX0_CH3_B2A_IRQn     = 96,       /*!< MBOX0 CH3 B2A Interrupt     */
    MBOX0_CH0_A2B_IRQn     = 97,       /*!< MBOX0 CH0 A2B Interrupt     */
    MBOX0_CH1_A2B_IRQn     = 98,       /*!< MBOX0 CH1 A2B Interrupt     */
    MBOX0_CH2_A2B_IRQn     = 99,       /*!< MBOX0 CH2 A2B Interrupt     */
    MBOX0_CH3_A2B_IRQn     = 100,      /*!< MBOX0 CH3 A2B Interrupt     */
    MBOX1_CH0_B2A_IRQn     = 101,      /*!< MBOX1 CH0 B2A Interrupt     */
    MBOX1_CH1_B2A_IRQn     = 102,      /*!< MBOX1 CH1 B2A Interrupt     */
    MBOX1_CH2_B2A_IRQn     = 103,      /*!< MBOX1 CH2 B2A Interrupt     */
    MBOX1_CH3_B2A_IRQn     = 104,      /*!< MBOX1 CH3 B2A Interrupt     */
    MBOX1_CH0_A2B_IRQn     = 105,      /*!< MBOX1 CH0 A2B Interrupt     */
    MBOX1_CH1_A2B_IRQn     = 106,      /*!< MBOX1 CH1 A2B Interrupt     */
    MBOX1_CH2_A2B_IRQn     = 107,      /*!< MBOX1 CH2 A2B Interrupt     */
    MBOX1_CH3_A2B_IRQn     = 108,      /*!< MBOX1 CH3 A2B Interrupt     */
    MBOX2_CH0_B2A_IRQn     = 109,      /*!< MBOX2 CH0 B2A Interrupt     */
    MBOX2_CH1_B2A_IRQn     = 110,      /*!< MBOX2 CH1 B2A Interrupt     */
    MBOX2_CH2_B2A_IRQn     = 111,      /*!< MBOX2 CH2 B2A Interrupt     */
    MBOX2_CH3_B2A_IRQn     = 112,      /*!< MBOX2 CH3 B2A Interrupt     */
    MBOX2_CH0_A2B_IRQn     = 113,      /*!< MBOX2 CH0 A2B Interrupt     */
    MBOX2_CH1_A2B_IRQn     = 114,      /*!< MBOX2 CH1 A2B Interrupt     */
    MBOX2_CH2_A2B_IRQn     = 115,      /*!< MBOX2 CH2 A2B Interrupt     */
    MBOX2_CH3_A2B_IRQn     = 116,      /*!< MBOX2 CH3 A2B Interrupt     */
    DMAC0_IRQn             = 118,      /*!< DMAC0 Interrupt             */
    DMAC0_ABORT_IRQn       = 119,      /*!< DMAC0 ABORT Interrupt       */
    DMAC1_IRQn             = 120,      /*!< DMAC1 Interrupt             */
    DMAC1_ABORT_IRQn       = 121,      /*!< DMAC1 ABORT Interrupt       */
    DMAC2_IRQn             = 122,      /*!< DMAC2 Interrupt             */
    DMAC2_ABORT_IRQn       = 123,      /*!< DMAC2 ABORT Interrupt       */
    I2S0_8CH_IRQn          = 212,      /*!< I2S0_8CH Interrupt          */
    I2S1_8CH_IRQn          = 213,      /*!< I2S1_8CH Interrupt          */
    I2S2_2CH_IRQn          = 214,      /*!< I2S2_2CH Interrupt          */
    I2S3_2CH_IRQn          = 215,      /*!< I2S3_2CH Interrupt          */
    PCIE30x4_LEGACY_IRQn   = 292,      /*!< PCIe3x4_legacy Interrupt    */
    GPIO0_IRQn             = 309,      /*!< GPIO0 Interrupt             */
    GPIO1_IRQn             = 310,      /*!< GPIO1 Interrupt             */
    GPIO2_IRQn             = 311,      /*!< GPIO2 Interrupt             */
    GPIO3_IRQn             = 312,      /*!< GPIO3 Interrupt             */
    GPIO4_IRQn             = 313,      /*!< GPIO4 Interrupt             */
    GPIO0_EXP_IRQn         = 314,      /*!< GPIO0 EXP Interrupt         */
    GPIO1_EXP_IRQn         = 315,      /*!< GPIO1 EXP Interrupt         */
    GPIO2_EXP_IRQn         = 316,      /*!< GPIO2 EXP Interrupt         */
    GPIO3_EXP_IRQn         = 317,      /*!< GPIO3 EXP Interrupt         */
    GPIO4_EXP_IRQn         = 318,      /*!< GPIO4 EXP Interrupt         */
    TIMER0_IRQn            = 321,      /*!< TIMER0 Interrupt            */
    TIMER1_IRQn            = 322,      /*!< TIMER1 Interrupt            */
    TIMER2_IRQn            = 323,      /*!< TIMER2 Interrupt            */
    TIMER3_IRQn            = 324,      /*!< TIMER3 Interrupt            */
    TIMER4_IRQn            = 325,      /*!< TIMER4 Interrupt            */
    TIMER5_IRQn            = 326,      /*!< TIMER5 Interrupt            */
    TIMER6_IRQn            = 327,      /*!< TIMER6 Interrupt            */
    TIMER7_IRQn            = 328,      /*!< TIMER7 Interrupt            */
    TIMER8_IRQn            = 329,      /*!< TIMER8 Interrupt            */
    TIMER9_IRQn            = 330,      /*!< TIMER9 Interrupt            */
    TIMER10_IRQn           = 331,      /*!< TIMER10 Interrupt           */
    TIMER11_IRQn           = 332,      /*!< TIMER11 Interrupt           */
    UART0_IRQn             = 363,      /*!< UART0  Interrupt            */
    UART1_IRQn             = 364,      /*!< UART1  Interrupt            */
    UART2_IRQn             = 365,      /*!< UART2  Interrupt            */
    UART3_IRQn             = 366,      /*!< UART3  Interrupt            */
    UART4_IRQn             = 367,      /*!< UART4  Interrupt            */
    UART5_IRQn             = 368,      /*!< UART5  Interrupt            */
    UART6_IRQn             = 369,      /*!< UART6  Interrupt            */
    UART7_IRQn             = 370,      /*!< UART7  Interrupt            */
    UART8_IRQn             = 371,      /*!< UART8  Interrupt            */
    UART9_IRQn             = 372,      /*!< UART9  Interrupt            */
    RSVD0_IRQn             = 454,      /*!< RSVD0  Interrupt            */
    NUM_INTERRUPTS         = 512,
} IRQn_Type;

#define RSVD_IRQn(_N)               (RSVD0_IRQn + (_N))

#define AMP_CPUOFF_REQ_IRQ(cpu)     RSVD_IRQn(15 + (cpu)) /* gic irq: 469 */

#define GPIO_IRQ_GROUP_DIRQ_BASE    480
#define GPIO_IRQ_GROUP_DIRQ_NUM     32

#define GPIO_IRQ_GROUP_GPIO0_HWIRQ  GPIO0_IRQn
#define GPIO_IRQ_GROUP_GPION_HWIRQ  GPIO4_IRQn

#endif /* HAL_AP_CORE */

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

#ifdef HAL_AP_CORE
#define __CORTEX_A            55U
#define __CORTEX_A_BIG        76U       /* Cortex-A76 Core                          */
#define __CORTEX_A_LIT        55U       /* Cortex-A55 Core                          */
#define __FPU_PRESENT         1U        /* FPU present                              */
#define __TIM_PRESENT         1U        /* Generic Timer                            */

#define CACHE_LINE_SHIFT    (6U)
#define CACHE_LINE_SIZE     (0x1U << CACHE_LINE_SHIFT)

#else
#define __CM0_REV                 0x0000U   /* Core revision r0p0 */
#define __MPU_PRESENT             0U        /* no MPU present */
#define __VTOR_PRESENT            0U        /* no VTOR present */
#define __NVIC_PRIO_BITS          2U        /* Number of Bits used for Priority Levels */
#define __Vendor_SysTickConfig    0U        /* Set to 1 if different SysTick Config is used */

#define NVIC_PERIPH_IRQ_OFFSET    16U
#define MAX_INTERRUPT_VECTOR      64U

#endif

#ifndef __ASSEMBLY__
#include "cmsis_compiler.h"
#ifdef __CORTEX_A
#include "core_ca.h"
#else
#include "core_cm0.h"
#endif
#include "system_rk3588.h"
#endif /* __ASSEMBLY__ */
#include "rk3588.h"

/****************************************************************************************/
/*                                                                                      */
/*                                Module Address Section                                */
/*                                                                                      */
/****************************************************************************************/
/* Memory Base */
#define GIC_DISTRIBUTOR_BASE    0xFE600000 /* GICD base address */
#define GIC_REDISTRIBUTOR_BASE  0xFE660000 /* GICR base address */

/****************************************************************************************/
/*                                                                                      */
/*                               Register Bitmap Section                                */
/*                                                                                      */
/****************************************************************************************/
#if defined(HAL_AP_CORE)
/********************************** CPU Topology ****************************************/
#define MPIDR_MT_MASK       ((1U) << 24)
#define MPIDR_AFFLVL_MASK   (0xFFU)
#define MPIDR_AFF0_SHIFT    (0U)
#define MPIDR_AFF1_SHIFT    (8U)
#define MPIDR_AFF2_SHIFT    (16U)
#define MPIDR_AFF3_SHIFT    (32U)
#define MPIDR_AFFINITY_MASK (0xFFFFFFU)
#define PLATFORM_CLUSTER0_CORE_COUNT (8)
#define PLATFORM_CORE_COUNT (8)
#define CPU_GET_AFFINITY(cpuId, clusterId) ((cpuId) << MPIDR_AFF1_SHIFT)

#endif /* HAL_AP_CORE */

#if defined(HAL_MCU_CORE)
/**************************************** CACHE ****************************************/
/* CACHE LINE SIZE */
#define CACHE_LINE_SHIFT                (5U)
#define CACHE_LINE_SIZE                 (0x1U << CACHE_LINE_SHIFT)
#define CACHE_LINE_ADDR_MASK            (0xFFFFFFFFU << CACHE_LINE_SHIFT)
#define CACHE_M_CLEAN                   0x0U
#define CACHE_M_INVALID                 0x2U
#define CACHE_M_CLEAN_INVALID           0x4U
#define CACHE_M_INVALID_ALL             0x6U
#define CACHE_REVISION                  (0x00000100U)

#ifndef HAL_CACHE_DECODED_ADDR_BASE
#error "Please define HAL_CACHE_DECODED_ADDR_BASE on hal_conf.h"
#endif

#endif /* HAL_MCU_CORE */

/******************************************CRU*******************************************/
#define CRU_CLK_USE_CON_BANK
#define CLK64(mux, div) ((((mux) & 0xffffffffULL) << 32) | ((div) & 0xffffffffULL))

#ifndef __ASSEMBLY__
typedef enum CLOCK_Name {
    CLK_INVALID = 0U,
    PLL_AUPLL,
    PLL_B0PLL,
    PLL_B1PLL,
    PLL_CPLL,
    PLL_GPLL,
    PLL_LPLL,
    PLL_NPLL,
    PLL_PPLL,
    PLL_V0PLL,
    ARMCLK_L,
    ARMCLK_B01,
    ARMCLK_B23,

    SCLK_DSU = CLK64(SCLK_DSU_SRC_T_SEL, 0U),

    /* storage */
    BCLK_EMMC            = CLK64(BCLK_EMMC_SEL, BCLK_EMMC_DIV),
    CCLK_EMMC            = CLK64(CCLK_EMMC_SEL, CCLK_EMMC_DIV),
    SCLK_SFC             = CLK64(SCLK_SFC_SEL, SCLK_SFC_DIV),
    CCLK_SRC_SDIO        = CLK64(CCLK_SRC_SDIO_SEL, CCLK_SRC_SDIO_DIV),
    CLK_REF_PIPE_PHY0    = CLK64(CLK_REF_PIPE_PHY0_SEL, CLK_REF_PIPE_PHY0_PLL_SRC_DIV),
    CLK_REF_PIPE_PHY1    = CLK64(CLK_REF_PIPE_PHY1_SEL, CLK_REF_PIPE_PHY1_PLL_SRC_DIV),
    CLK_REF_PIPE_PHY2    = CLK64(CLK_REF_PIPE_PHY2_SEL, CLK_REF_PIPE_PHY2_PLL_SRC_DIV),

    /* audio */
    HCLK_VAD             = CLK64(HCLK_PMU1_ROOT_I_SEL, 0U),
    MCLK_PDM0            = CLK64(MCLK_PDM0_SEL, 0U),
    CLK_I2S0_8CH_TX_SRC  = CLK64(CLK_I2S0_8CH_TX_SRC_SEL, CLK_I2S0_8CH_TX_SRC_DIV),
    CLK_I2S0_8CH_TX_FRAC = CLK64(0U, CLK_I2S0_8CH_TX_FRAC_DIV),
    CLK_I2S0_8CH_TX      = CLK64(MCLK_I2S0_8CH_TX_SEL, 0U),
    CLK_I2S0_8CH_RX_SRC  = CLK64(CLK_I2S0_8CH_RX_SRC_SEL, CLK_I2S0_8CH_RX_SRC_DIV),
    CLK_I2S0_8CH_RX_FRAC = CLK64(0U, CLK_I2S0_8CH_RX_FRAC_DIV),
    CLK_I2S0_8CH_RX      = CLK64(MCLK_I2S0_8CH_RX_SEL, 0U),
    CLK_I2S1_8CH_TX_SRC  = CLK64(0U, CLK_I2S1_8CH_TX_SRC_DIV),
    CLK_I2S1_8CH_TX_FRAC = CLK64(0U, CLK_I2S1_8CH_TX_FRAC_DIV),
    CLK_I2S1_8CH_TX      = CLK64(MCLK_I2S1_8CH_TX_SEL, 0U),
    CLK_I2S1_8CH_RX_SRC  = CLK64(0U, CLK_I2S1_8CH_RX_SRC_DIV),
    CLK_I2S1_8CH_RX_FRAC = CLK64(0U, CLK_I2S1_8CH_RX_FRAC_DIV),
    CLK_I2S1_8CH_RX      = CLK64(MCLK_I2S1_8CH_RX_SEL, 0U),
    CLK_I2S2_2CH         = CLK64(MCLK_I2S2_2CH_SEL, 0U),
    CLK_I2S2_2CH_SRC     = CLK64(CLK_I2S2_2CH_SRC_SEL, CLK_I2S2_2CH_SRC_DIV),
    CLK_I2S2_2CH_FRAC    = CLK64(0U, CLK_I2S2_2CH_FRAC_DIV),
    CLK_I2S3_2CH         = CLK64(MCLK_I2S3_2CH_SEL, 0U),
    CLK_I2S3_2CH_SRC     = CLK64(CLK_I2S3_2CH_SRC_SEL, CLK_I2S3_2CH_SRC_DIV),
    CLK_I2S3_2CH_FRAC    = CLK64(0U, CLK_I2S3_2CH_FRAC_DIV),
    CLK_I2S4_8CH_TX_SRC  = CLK64(0U, CLK_I2S4_8CH_TX_SRC_DIV),
    CLK_I2S4_8CH_TX_FRAC = CLK64(0U, CLK_I2S4_8CH_TX_FRAC_DIV),
    CLK_I2S4_8CH_TX      = CLK64(MCLK_I2S4_8CH_TX_SEL, 0U),
    CLK_I2S5_8CH_TX_SRC  = CLK64(0U, CLK_I2S5_8CH_TX_SRC_DIV),
    CLK_I2S5_8CH_TX_FRAC = CLK64(0U, CLK_I2S5_8CH_TX_FRAC_DIV),
    CLK_I2S5_8CH_TX      = CLK64(MCLK_I2S5_8CH_TX_SEL, 0U),
    CLK_I2S6_8CH_TX_SRC  = CLK64(CLK_I2S6_8CH_TX_SRC_SEL, CLK_I2S6_8CH_TX_SRC_DIV),
    CLK_I2S6_8CH_TX_FRAC = CLK64(0U, CLK_I2S6_8CH_TX_FRAC_DIV),
    CLK_I2S6_8CH_TX      = CLK64(MCLK_I2S6_8CH_TX_SEL, 0U),
    CLK_I2S6_8CH_RX_SRC  = CLK64(CLK_I2S6_8CH_RX_SRC_SEL, CLK_I2S6_8CH_RX_SRC_DIV),
    CLK_I2S6_8CH_RX_FRAC = CLK64(0U, CLK_I2S6_8CH_RX_FRAC_DIV),
    CLK_I2S6_8CH_RX      = CLK64(MCLK_I2S6_8CH_RX_SEL, 0U),
    CLK_I2S7_8CH_RX_SRC  = CLK64(CLK_I2S7_8CH_RX_SRC_SEL, CLK_I2S7_8CH_RX_SRC_DIV),
    CLK_I2S7_8CH_RX_FRAC = CLK64(0U, CLK_I2S7_8CH_RX_FRAC_DIV),
    CLK_I2S7_8CH_RX      = CLK64(MCLK_I2S7_8CH_RX_SEL, 0U),
    CLK_I2S8_8CH_TX_SRC  = CLK64(CLK_I2S8_8CH_TX_SRC_SEL, CLK_I2S8_8CH_TX_SRC_DIV),
    CLK_I2S8_8CH_TX_FRAC = CLK64(0U, CLK_I2S8_8CH_TX_FRAC_DIV),
    CLK_I2S8_8CH_TX      = CLK64(MCLK_I2S8_8CH_TX_SEL, 0U),
    CLK_I2S9_8CH_RX_SRC  = CLK64(CLK_I2S9_8CH_RX_SRC_SEL, CLK_I2S9_8CH_RX_SRC_DIV),
    CLK_I2S9_8CH_RX_FRAC = CLK64(0U, CLK_I2S9_8CH_RX_FRAC_DIV),
    CLK_I2S9_8CH_RX      = CLK64(MCLK_I2S9_8CH_RX_SEL, 0U),
    CLK_I2S10_8CH_RX_SRC = CLK64(CLK_I2S10_8CH_RX_SRC_SEL, CLK_I2S10_8CH_RX_SRC_DIV),
    CLK_I2S10_8CH_RX_FRAC = CLK64(0U, CLK_I2S10_8CH_RX_FRAC_DIV),
    CLK_I2S10_8CH_RX     = CLK64(MCLK_I2S10_8CH_RX_SEL, 0U),

    CLK_SPDIF0           = CLK64(MCLK_SPDIF0_SEL, 0U),
    CLK_SPDIF0_SRC       = CLK64(CLK_SPDIF0_SRC_SEL, CLK_SPDIF0_SRC_DIV),
    CLK_SPDIF0_FRAC      = CLK64(0U, CLK_SPDIF0_FRAC_DIV),
    CLK_SPDIF1           = CLK64(MCLK_SPDIF1_SEL, 0U),
    CLK_SPDIF1_SRC       = CLK64(CLK_SPDIF1_SRC_SEL, CLK_SPDIF1_SRC_DIV),
    CLK_SPDIF1_FRAC      = CLK64(0U, CLK_SPDIF1_FRAC_DIV),
    CLK_SPDIF2_DP0       = CLK64(MCLK_4X_SPDIF2_DP0_SEL, 0U),
    CLK_SPDIF2_DP0_SRC   = CLK64(CLK_SPDIF2_DP0_SRC_SEL, CLK_SPDIF2_DP0_SRC_DIV),
    CLK_SPDIF2_DP0_FRAC  = CLK64(0U, CLK_SPDIF2_DP0_FRAC_DIV),
    CLK_SPDIF3           = CLK64(MCLK_SPDIF3_SEL, 0U),
    CLK_SPDIF3_SRC       = CLK64(CLK_SPDIF3_SRC_SEL, CLK_SPDIF3_SRC_DIV),
    CLK_SPDIF3_FRAC      = CLK64(0U, CLK_SPDIF3_FRAC_DIV),
    CLK_SPDIF4           = CLK64(MCLK_SPDIF4_SEL, 0U),
    CLK_SPDIF4_SRC       = CLK64(CLK_SPDIF4_SRC_SEL, CLK_SPDIF4_SRC_DIV),
    CLK_SPDIF4_FRAC      = CLK64(0U, CLK_SPDIF4_FRAC_DIV),
    CLK_SPDIF5_DP1       = CLK64(MCLK_4X_SPDIF5_DP1_SEL, 0U),
    CLK_SPDIF5_DP1_SRC   = CLK64(CLK_SPDIF5_DP1_SRC_SEL, CLK_SPDIF5_DP1_SRC_DIV),
    CLK_SPDIF5_DP1_FRAC  = CLK64(0U, CLK_SPDIF5_DP1_FRAC_DIV),

    /* net */
    CLK_GMAC_125M        = CLK64(CLK_GMAC_125M_CRU_I_SEL, CLK_GMAC_125M_CRU_I_DIV),
    CLK_GMAC_50M         = CLK64(CLK_GMAC_50M_CRU_I_SEL, CLK_GMAC_50M_CRU_I_DIV),
    REFCLKO25M_ETH0_OUT  = CLK64(REFCLKO25M_ETH0_OUT_SEL, REFCLKO25M_ETH0_OUT_DIV),
    REFCLKO25M_ETH1_OUT  = CLK64(REFCLKO25M_ETH1_OUT_SEL, REFCLKO25M_ETH1_OUT_DIV),

    /* csi */
    DCLK_VICAP       = CLK64(DCLK_VICAP_SEL, DCLK_VICAP_DIV),
    CLK_MIPI_CAMOUT0 = CLK64(CLK_MIPI_CAMARAOUT_M0_SEL, CLK_MIPI_CAMARAOUT_M0_DIV),
    CLK_MIPI_CAMOUT1 = CLK64(CLK_MIPI_CAMARAOUT_M1_SEL, CLK_MIPI_CAMARAOUT_M1_DIV),
    CLK_MIPI_CAMOUT2 = CLK64(CLK_MIPI_CAMARAOUT_M2_SEL, CLK_MIPI_CAMARAOUT_M2_DIV),
    CLK_MIPI_CAMOUT3 = CLK64(CLK_MIPI_CAMARAOUT_M3_SEL, CLK_MIPI_CAMARAOUT_M3_DIV),
    CLK_MIPI_CAMOUT4 = CLK64(CLK_MIPI_CAMARAOUT_M4_SEL, CLK_MIPI_CAMARAOUT_M4_DIV),
    CLK_CIFOUT       = CLK64(CLK_CIFOUT_OUT_SEL, CLK_CIFOUT_OUT_DIV),

    /* display */
    DCLK_VOP0_SRC        = CLK64(DCLK_VOP0_SRC_SEL, DCLK_VOP0_SRC_DIV),
    DCLK_VOP1_SRC        = CLK64(DCLK_VOP1_SRC_SEL, DCLK_VOP1_SRC_DIV),
    DCLK_VOP2_SRC        = CLK64(DCLK_VOP2_SRC_SEL, DCLK_VOP2_SRC_DIV),
    DCLK_VOP0            = CLK64(DCLK_VOP0_SEL, 0U),
    DCLK_VOP1            = CLK64(DCLK_VOP1_SEL, 0U),
    DCLK_VOP2            = CLK64(DCLK_VOP2_SEL, 0U),
    DCLK_VOP3            = CLK64(DCLK_VOP3_SEL, DCLK_VOP3_DIV),

    /* other interface */
    CLK_UART0_SRC        = CLK64(0U, CLK_UART0_SRC_DIV),
    CLK_UART0_FRAC       = CLK64(0U, CLK_UART0_FRAC_DIV),
    CLK_UART0            = CLK64(SCLK_UART0_SEL, 0U),
    CLK_UART1_SRC        = CLK64(CLK_UART1_SRC_SEL, CLK_UART1_SRC_DIV),
    CLK_UART1_FRAC       = CLK64(0U, CLK_UART1_FRAC_DIV),
    CLK_UART1            = CLK64(SCLK_UART1_SEL, 0U),
    CLK_UART2_SRC        = CLK64(CLK_UART2_SRC_SEL, CLK_UART2_SRC_DIV),
    CLK_UART2_FRAC       = CLK64(0U, CLK_UART2_FRAC_DIV),
    CLK_UART2            = CLK64(SCLK_UART2_SEL, 0U),
    CLK_UART3_SRC        = CLK64(CLK_UART3_SRC_SEL, CLK_UART3_SRC_DIV),
    CLK_UART3_FRAC       = CLK64(0U, CLK_UART3_FRAC_DIV),
    CLK_UART3            = CLK64(SCLK_UART3_SEL, 0U),
    CLK_UART4_SRC        = CLK64(CLK_UART4_SRC_SEL, CLK_UART4_SRC_DIV),
    CLK_UART4_FRAC       = CLK64(0U, CLK_UART4_FRAC_DIV),
    CLK_UART4            = CLK64(SCLK_UART4_SEL, 0U),
    CLK_UART5_SRC        = CLK64(CLK_UART5_SRC_SEL, CLK_UART5_SRC_DIV),
    CLK_UART5_FRAC       = CLK64(0U, CLK_UART5_FRAC_DIV),
    CLK_UART5            = CLK64(SCLK_UART5_SEL, 0U),
    CLK_UART6_SRC        = CLK64(CLK_UART6_SRC_SEL, CLK_UART6_SRC_DIV),
    CLK_UART6_FRAC       = CLK64(0U, CLK_UART6_FRAC_DIV),
    CLK_UART6            = CLK64(SCLK_UART6_SEL, 0U),
    CLK_UART7_SRC        = CLK64(CLK_UART7_SRC_SEL, CLK_UART7_SRC_DIV),
    CLK_UART7_FRAC       = CLK64(0U, CLK_UART7_FRAC_DIV),
    CLK_UART7            = CLK64(SCLK_UART7_SEL, 0U),
    CLK_UART8_SRC        = CLK64(CLK_UART8_SRC_SEL, CLK_UART8_SRC_DIV),
    CLK_UART8_FRAC       = CLK64(0U, CLK_UART8_FRAC_DIV),
    CLK_UART8            = CLK64(SCLK_UART8_SEL, 0U),
    CLK_UART9_SRC        = CLK64(CLK_UART9_SRC_SEL, CLK_UART9_SRC_DIV),
    CLK_UART9_FRAC       = CLK64(0U, CLK_UART9_FRAC_DIV),
    CLK_UART9            = CLK64(SCLK_UART9_SEL, 0U),

    CLK_I2C0             = CLK64(CLK_I2C0_SEL, 0U),
    CLK_I2C1             = CLK64(CLK_I2C1_SEL, 0U),
    CLK_I2C2             = CLK64(CLK_I2C2_SEL, 0U),
    CLK_I2C3             = CLK64(CLK_I2C3_SEL, 0U),
    CLK_I2C4             = CLK64(CLK_I2C4_SEL, 0U),
    CLK_I2C5             = CLK64(CLK_I2C5_SEL, 0U),
    CLK_I2C6             = CLK64(CLK_I2C6_SEL, 0U),
    CLK_I2C7             = CLK64(CLK_I2C7_SEL, 0U),
    CLK_I2C8             = CLK64(CLK_I2C8_SEL, 0U),

    CLK_PWM1             = CLK64(CLK_PWM1_SEL, 0U),
    CLK_PWM2             = CLK64(CLK_PWM2_SEL, 0U),
    CLK_PWM3             = CLK64(CLK_PWM3_SEL, 0U),

    CLK_SPI0             = CLK64(CLK_SPI0_SEL, 0U),
    CLK_SPI1             = CLK64(CLK_SPI1_SEL, 0U),
    CLK_SPI2             = CLK64(CLK_SPI2_SEL, 0U),
    CLK_SPI3             = CLK64(CLK_SPI3_SEL, 0U),
    CLK_SPI4             = CLK64(CLK_SPI4_SEL, 0U),

    CLK_AUX16M_0         = CLK64(0U, CLK_AUX16MHZ_0_DIV),
    CLK_AUX16M_1         = CLK64(0U, CLK_AUX16MHZ_1_DIV),

    HCLK_PMU_CM0         = CLK64(HCLK_PMU_CM0_ROOT_I_SEL, 0U),
} eCLOCK_Name;
#endif /* __ASSEMBLY__ */
/****************************************MBOX********************************************/
#define MBOX_CNT             2
#define MBOX_CHAN_CNT        4

/***************************************INTMUX*******************************************/
#define INTMUX0_PMU_BASE               0xFECF0000U /* INTMUX0_PMU base address */
#define INTMUX1_PMU_BASE               0xFECF4000U /* INTMUX1_PMU base address */
#define INTMUX0_DDR_BASE               0xFECF8000U /* INTMUX0_DDR base address */
#define INTMUX1_DDR_BASE               0xFECFC000U /* INTMUX1_DDR base address */

/* INTMUX Register Structure Define */
struct INTMUX_REG {
    __IO uint32_t INT_ENABLE_GROUP[32];               /* Address Offset: 0x0000 */
    __IO uint32_t INT_FLAG_GROUP[32];                 /* Address Offset: 0x0080 */
};

#if defined(RKMCU_RK3588_PMU)
#define INTMUX0             ((struct INTMUX_REG *) INTMUX0_PMU_BASE)
#define INTMUX1             ((struct INTMUX_REG *) INTMUX1_PMU_BASE)
#elif defined(RKMCU_RK3588_DDR)
#define INTMUX0             ((struct INTMUX_REG *) INTMUX0_DDR_BASE)
#define INTMUX1             ((struct INTMUX_REG *) INTMUX1_DDR_BASE)
#endif

#define IS_INTMUX0_INSTANCE(instance) ((instance) == INTMUX0)
#define IS_INTMUX1_INSTANCE(instance) ((instance) == INTMUX1)

/****************************************************************************************/
/*                                                                                      */
/*                           Platform Differences Section                               */
/*                                                                                      */
/****************************************************************************************/
#if defined(HAL_AP_CORE)

#undef DCACHE
#undef ICACHE

#endif

#if defined(HAL_MCU_CORE)

#if defined(RKMCU_RK3588_PMU)
#undef TIMER0_BASE
#undef TIMER1_BASE
#define TIMER0_BASE                    0xFD8F0000U /* TIMER0 base address */
#define TIMER1_BASE                    0xFD8F0020U /* TIMER1 base address */
#elif defined(RKMCU_RK3588_DDR)
#undef DCACHE
#undef ICACHE
#undef MBOX0_BASE
#define MBOX0_BASE                     0xFEC70000U /* MBOX0 base address */
#undef WDT_BASE
#define WDT_BASE                       0xFE110000U /* WDT base address*/
#undef TIMER0_BASE
#undef TIMER1_BASE
#define TIMER0_BASE                    0xFE118000U /* TIMER0_BASE*/
#define TIMER1_BASE                    0xFE118020U /* TIMER1_BASE*/
#elif defined(RKMCU_RK3588_NPU)
#undef MBOX0_BASE
#define MBOX0_BASE                     0xFECE0000U /* MBOX0 base address */
#undef WDT_BASE
#define WDT_BASE                       0xFDAF8000U /* WDT base address*/
#undef TIMER0_BASE
#undef TIMER1_BASE
#define TIMER0_BASE                    0xFDB00000U /* TIMER0_BASE*/
#define TIMER1_BASE                    0xFDB00020U /* TIMER1_BASE*/
#endif

#endif

/****************************************GPIO********************************************/
#ifdef GPIO_VER_ID
#undef GPIO_VER_ID
#define GPIO_VER_ID             (0x01000C2BU)
#endif
/****************************************PCIE********************************************/
#define PCIE3_4L_S_BASE                0xF0000000U /* PCIE 32btis mmio base address */
#define PCIE3_4L_DBI_BASE              0xF5000000U /* PCIE dbi base address */
#define PCIE3PHY_GRF_BASE              0xFD5B8000U /* S-PHY_GRF base address */
#define PCIE3_4L_APB_BASE              0xFE150000U /* PCIE apb base address */
#define PCIE3_PHY_BASE                 0xFEE80000U /* S-PHY base address */
#define PCIE3_4L_S_64_BASE             0x900000000U /* PCIE 64btis mmio base address */

/****************************************PMU*********************************************/
#define RK3588_PD_GPU     0
#define RK3588_PD_NPU     1
#define RK3588_PD_VCODEC  2
#define RK3588_PD_NPUTOP  3
#define RK3588_PD_NPU1    4
#define RK3588_PD_NPU2    5
#define RK3588_PD_VENC0   6
#define RK3588_PD_VENC1   7
#define RK3588_PD_RKVDEC0 8
#define RK3588_PD_RKVDEC1 9
#define RK3588_PD_VDPU    10
#define RK3588_PD_RGA30   11
#define RK3588_PD_AV1     12
#define RK3588_PD_VI      13
#define RK3588_PD_FEC     14
#define RK3588_PD_ISP1    15
#define RK3588_PD_RGA31   16
#define RK3588_PD_VOP     17
#define RK3588_PD_VO0     18
#define RK3588_PD_VO1     19
#define RK3588_PD_AUDIO   20
#define RK3588_PD_PHP     21
#define RK3588_PD_GMAC    22
#define RK3588_PD_PCIE    23
#define RK3588_PD_NVM     24
#define RK3588_PD_NVM0    25
#define RK3588_PD_SDIO    26
#define RK3588_PD_USB     27
#define RK3588_PD_SDMMC   28

#define PD_RK3588(id) (id | 0x80000000U)

#ifndef __ASSEMBLY__
typedef enum PD_Id {
    PD_INVALID = 0U,
    PD_GPU     = PD_RK3588(RK3588_PD_GPU),
    PD_NPU     = PD_RK3588(RK3588_PD_NPU),
    PD_VCODEC  = PD_RK3588(RK3588_PD_VCODEC),
    PD_NPUTOP  = PD_RK3588(RK3588_PD_NPUTOP),
    PD_NPU1    = PD_RK3588(RK3588_PD_NPU1),
    PD_NPU2    = PD_RK3588(RK3588_PD_NPU2),
    PD_VENC0   = PD_RK3588(RK3588_PD_VENC0),
    PD_VENC1   = PD_RK3588(RK3588_PD_VENC1),
    PD_RKVDEC0 = PD_RK3588(RK3588_PD_RKVDEC0),
    PD_RKVDEC1 = PD_RK3588(RK3588_PD_RKVDEC1),
    PD_VDPU    = PD_RK3588(RK3588_PD_VDPU),
    PD_RGA30   = PD_RK3588(RK3588_PD_RGA30),
    PD_AV1     = PD_RK3588(RK3588_PD_AV1),
    PD_VI      = PD_RK3588(RK3588_PD_VI),
    PD_FEC     = PD_RK3588(RK3588_PD_FEC),
    PD_ISP1    = PD_RK3588(RK3588_PD_ISP1),
    PD_RGA31   = PD_RK3588(RK3588_PD_RGA31),
    PD_VOP     = PD_RK3588(RK3588_PD_VOP),
    PD_VO0     = PD_RK3588(RK3588_PD_VO0),
    PD_VO1     = PD_RK3588(RK3588_PD_VO1),
    PD_AUDIO   = PD_RK3588(RK3588_PD_AUDIO),
    PD_PHP     = PD_RK3588(RK3588_PD_PHP),
    PD_GMAC    = PD_RK3588(RK3588_PD_GMAC),
    PD_PCIE    = PD_RK3588(RK3588_PD_PCIE),
    PD_NVM     = PD_RK3588(RK3588_PD_NVM),
    PD_NVM0    = PD_RK3588(RK3588_PD_NVM0),
    PD_SDIO    = PD_RK3588(RK3588_PD_SDIO),
    PD_USB     = PD_RK3588(RK3588_PD_USB),
    PD_SDMMC   = PD_RK3588(RK3588_PD_SDMMC),
} ePD_Id;
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __SOC_H */
