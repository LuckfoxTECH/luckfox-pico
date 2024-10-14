/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2023 Rockchip Electronics Co., Ltd.
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
    DMA_REQ_UART0_RX = 0,
    DMA_REQ_UART1_RX = 1,
    DMA_REQ_UART2_RX = 2,
    DMA_REQ_UART3_RX = 3,
    DMA_REQ_UART4_RX = 4,
    DMA_REQ_UART5_RX = 5,
    DMA_REQ_UART6_RX = 6,
    DMA_REQ_UART7_RX = 7,
    DMA_REQ_UART8_RX = 8,
    DMA_REQ_UART9_RX = 9,
    DMA_REQ_UART1_TX = 10,
    DMA_REQ_UART5_TX = 11,
    DMA_REQ_SPI0_RX  = 12,
    DMA_REQ_SPI0_TX  = 13,
    DMA_REQ_SPI1_RX  = 14,
    DMA_REQ_SPI1_TX  = 15,
    DMA_REQ_SPI2_RX  = 16,
    DMA_REQ_SPI2_TX  = 17,
    DMA_REQ_SAI0_RX  = 18,
    DMA_REQ_SAI0_TX  = 19,
    DMA_REQ_SAI1_RX  = 20,
    DMA_REQ_SAI1_TX  = 21,
    DMA_REQ_SAI2_RX  = 22,
    DMA_REQ_SAI2_TX  = 23,
    DMA_REQ_CAN0_RX  = 24,
    DMA_REQ_CAN9_TX  = 25,
    DMA_REQ_PWM0     = 26,
    DMA_REQ_PWM1     = 27,
    DMA_REQ_PWM2     = 28,
    DMA_REQ_PWM3     = 29,
    DMA_REQ_SPDIF    = 30,
    DMA_REQ_PDM      = 31,
    DMA_REQ_CAN1_RX  = 32,
    DMA_REQ_CAN1_TX  = 33,
    DMA_REQ_UART0_TX = 34,
    DMA_REQ_UART2_TX = 35,
    DMA_REQ_UART3_TX = 36,
    DMA_REQ_UART4_TX = 37,
    DMA_REQ_UART6_TX = 38,
    DMA_REQ_UART7_TX = 39,
    DMA_REQ_UART8_TX = 40,
    DMA_REQ_UART9_TX = 41,
} DMA_REQ_Type;

/* ================================================================================ */
/* ================                       IRQ                      ================ */
/* ================================================================================ */
#ifdef HAL_MCU_CORE
#if defined(RKMCU_RK3562_BUS)

#define INTMUX_NUM_INT_PER_CON    256
#define INTMUX_NUM_OUT_PER_CON    4
#define INTMUX_NUM_INT_PER_OUT    64
#define INTMUX_NUM_GROUP_PER_OUT  8
#define INTMUX_NUM_GROUP_PER_CON  32
#define INTMUX_NUM_INT_PER_GROUP  8
/* INTMUX IRQ start from GIC SPI(Shared Peripheral Interrupt) */
#define INTMUX_IRQ_START_NUM      32
#define INTMUX_IRQ_OUT0
#define INTMUX_IRQ_OUT1
#define INTMUX_IRQ_OUT2
#define INTMUX_IRQ_OUT3
#define INTMUX_OUT_IRQ_START_NUM  0

#define NUM_EXT_INTERRUPTS        256

typedef enum {
/* -------------------  Processor Exceptions Numbers  ----------------------------- */
  NonMaskableInt_IRQn       = -14,     /*  2 Non Maskable Interrupt */
  HardFault_IRQn            = -13,     /*  3 HardFault Interrupt */

  SVCall_IRQn               =  -5,     /* 11 SV Call Interrupt */

  PendSV_IRQn               =  -2,     /* 14 Pend SV Interrupt */
  SysTick_IRQn              =  -1,     /* 15 System Tick Interrupt */

/******  Platform Exceptions Numbers ***************************************************/
  INTMUX_OUT0_IRQn          =  0,      /*!< INTMUX OUT0 Interrupt         */
  INTMUX_OUT1_IRQn          =  1,      /*!< INTMUX OUT1 Interrupt         */
  INTMUX_OUT2_IRQn          =  2,      /*!< INTMUX OUT2 Interrupt         */
  INTMUX_OUT3_IRQn          =  3,      /*!< INTMUX OUT3 Interrupt         */
  MBOX_BB_IRQn              =  4,      /*!< MAILBOX_BB Interrupt          */
  TIMER0_IRQn               =  5,      /*!< TIMER0 Interrupt              */
  RSVD0_MCU_IRQn            =  6,      /*!< RSVD0 MCU Interrupt           */
  NUM_INTERRUPTS            =  8,      /*!< Number of internal IRQ        */
  GPIO0_IRQn                =  32 + NUM_INTERRUPTS,     /*!< GPIO0 Interrupt               */
  GPIO0_EXP_IRQn            =  33 + NUM_INTERRUPTS,     /*!< GPIO0 EXP Interrupt           */
  GPIO1_IRQn                =  34 + NUM_INTERRUPTS,     /*!< GPIO1 Interrupt               */
  GPIO1_EXP_IRQn            =  35 + NUM_INTERRUPTS,     /*!< GPIO1 EXP Interrupt           */
  GPIO2_IRQn                =  36 + NUM_INTERRUPTS,     /*!< GPIO2 Interrupt               */
  GPIO2_EXP_IRQn            =  37 + NUM_INTERRUPTS,     /*!< GPIO2 EXP Interrupt           */
  GPIO3_IRQn                =  38 + NUM_INTERRUPTS,     /*!< GPIO3 Interrupt               */
  GPIO3_EXP_IRQn            =  39 + NUM_INTERRUPTS,     /*!< GPIO3 EXP Interrupt           */
  GPIO4_IRQn                =  40 + NUM_INTERRUPTS,     /*!< GPIO4 Interrupt               */
  GPIO4_EXP_IRQn            =  41 + NUM_INTERRUPTS,     /*!< GPIO4 EXP Interrupt           */
  I2C0_IRQn                 =  44 + NUM_INTERRUPTS,     /*!< I2C0 Interrupt                */
  I2C1_IRQn                 =  45 + NUM_INTERRUPTS,     /*!< I2C1 Interrupt                */
  I2C2_IRQn                 =  46 + NUM_INTERRUPTS,     /*!< I2C2 Interrupt                */
  I2C3_IRQn                 =  47 + NUM_INTERRUPTS,     /*!< I2C3 Interrupt                */
  I2C4_IRQn                 =  48 + NUM_INTERRUPTS,     /*!< I2C4 Interrupt                */
  I2C5_IRQn                 =  49 + NUM_INTERRUPTS,     /*!< I2C5 Interrupt                */
  PWM0_IRQn                 =  52 + NUM_INTERRUPTS,     /*!< PWM0 Interrupt                */
  PWM0_PWR_IRQn             =  53 + NUM_INTERRUPTS,     /*!< PWM0 PWR Interrupt            */
  PWM1_IRQn                 =  54 + NUM_INTERRUPTS,     /*!< PWM1 Interrupt                */
  PWM1_PWR_IRQn             =  55 + NUM_INTERRUPTS,     /*!< PWM1 PWR Interrupt            */
  PWM2_IRQn                 =  56 + NUM_INTERRUPTS,     /*!< PWM2 Interrupt                */
  PWM2_PWR_IRQn             =  57 + NUM_INTERRUPTS,     /*!< PWM2 PWR Interrupt            */
  PWM3_IRQn                 =  58 + NUM_INTERRUPTS,     /*!< PWM3 Interrupt                */
  PWM3_PWR_IRQn             =  59 + NUM_INTERRUPTS,     /*!< PWM3 PWR Interrupt            */
  UART0_IRQn                =  62 + NUM_INTERRUPTS,     /*!< UART0 Interrupt               */
  UART1_IRQn                =  63 + NUM_INTERRUPTS,     /*!< UART1 Interrupt               */
  UART2_IRQn                =  64 + NUM_INTERRUPTS,     /*!< UART2 Interrupt               */
  UART3_IRQn                =  65 + NUM_INTERRUPTS,     /*!< UART3 Interrupt               */
  UART4_IRQn                =  66 + NUM_INTERRUPTS,     /*!< UART4 Interrupt               */
  UART5_IRQn                =  67 + NUM_INTERRUPTS,     /*!< UART5 Interrupt               */
  UART6_IRQn                =  68 + NUM_INTERRUPTS,     /*!< UART6 Interrupt               */
  UART7_IRQn                =  69 + NUM_INTERRUPTS,     /*!< UART7 Interrupt               */
  UART8_IRQn                =  70 + NUM_INTERRUPTS,     /*!< UART8 Interrupt               */
  UART9_IRQn                =  71 + NUM_INTERRUPTS,     /*!< UART9 Interrupt               */
  SARADC0_IRQn              =  72 + NUM_INTERRUPTS,     /*!< SARADC0 Interrupt             */
  TIMER1_IRQn               =  78 + NUM_INTERRUPTS,     /*!< TIMER1 Interrupt               */
  TIMER2_IRQn               =  79 + NUM_INTERRUPTS,     /*!< TIMER2 Interrupt               */
  TIMER3_IRQn               =  80 + NUM_INTERRUPTS,     /*!< TIMER3 Interrupt               */
  TIMER4_IRQn               =  81 + NUM_INTERRUPTS,     /*!< TIMER4 Interrupt               */
  TIMER5_IRQn               =  81 + NUM_INTERRUPTS,     /*!< TIMER5 Interrupt               */
  SPI0_IRQn                 =  84 + NUM_INTERRUPTS,     /*!< SPI0 Interrupt                 */
  SPI1_IRQn                 =  85 + NUM_INTERRUPTS,     /*!< SPI1 Interrupt                 */
  SPI2_IRQn                 =  86 + NUM_INTERRUPTS,     /*!< SPI2 Interrupt                 */
  WDT0_IRQn                 =  134 + NUM_INTERRUPTS,    /*!< WDT0 Interrupt                 */
  SARADC1_IRQn              =  156 + NUM_INTERRUPTS,    /*!< SARADC1 Interrupt              */
  FSPI0_IRQn                =  160 + NUM_INTERRUPTS,    /*!< FSPI0 Interrupt                */
  TOTAL_INTERRUPTS          =  (INTMUX_IRQ_START_NUM + NUM_INTERRUPTS + NUM_EXT_INTERRUPTS),
} IRQn_Type;

#elif defined(RKMCU_RK3562_PMU)

typedef enum {
/* -------------------  Processor Exceptions Numbers  ----------------------------- */
  NonMaskableInt_IRQn       = -14,     /*  2 Non Maskable Interrupt */
  HardFault_IRQn            = -13,     /*  3 HardFault Interrupt */

  SVCall_IRQn               =  -5,     /* 11 SV Call Interrupt */

  PendSV_IRQn               =  -2,     /* 14 Pend SV Interrupt */
  SysTick_IRQn              =  -1,     /* 15 System Tick Interrupt */

/******  Platform Exceptions Numbers ***************************************************/
  MBOX_BB_IRQn              =  0,      /*!< MAILBOX_BB Interrupt          */
  HPTIMER_IRQn              =  1,      /*!< HPTIMER Interrupt             */
  UART0_IRQn                =  2,      /*!< UART0 Interrupt               */
  WDT_IRQn                  =  3,      /*!< WDT Interrupt                 */
  GPIO0_IRQn                =  4,      /*!< GPIO0 Interrupt               */
  SPI0_IRQn                 =  5,      /*!< SPI0 Interrupt                */
  PWM0_IRQn                 =  6,      /*!< PWM0 Interrupt                */
  I2C0_IRQn                 =  7,      /*!< I2C0 Interrupt                */
  NUM_INTERRUPTS            =  8,      /*!< Number of internal IRQ        */
  DUMMY_IRQn                =  256,    /*!< Avoid compile warning: overflow in conversion   */
} IRQn_Type;

#else
#error missing IRQn_Type define for interrupt
#endif

#if defined(RKMCU_RK3562_BUS)

#define RSVD_MCU_IRQn(_N)               (RSVD0_MCU_IRQn + (_N))
#define HAS_CUSTOME_INTC

#endif

#else /* HAL_AP_CORE */

typedef enum {
/* When IPI_SGIs are used in AMP mode, you need to pay attention to whether it conflicts
 * with SMP mode. Especially in the case of Linux OS as The Master Core.
 * IPI_SGI 0~7 for non-secure and IPI_SGI 8~15 for secure.
 */
  IPI_SGI0                  = 0,
  IPI_SGI1                  = 1,
  IPI_SGI2                  = 2,
  IPI_SGI3                  = 3,
  IPI_SGI4                  = 4,
  IPI_SGI5                  = 5,
  IPI_SGI6                  = 6,
  IPI_SGI7                  = 7,
  IPI_SGI8                  = 8,
  IPI_SGI9                  = 9,
  IPI_SGI10                 = 10,
  IPI_SGI11                 = 11,
  IPI_SGI12                 = 12,
  IPI_SGI13                 = 13,
  IPI_SGI14                 = 14,
  IPI_SGI15                 = 15,

  CNTHP_IRQn                = 26,
  CNTV_IRQn                 = 27,
  CNTPS_IRQn                = 29,
  CNTPNS_IRQn               = 30,

/******  Platform Exceptions Numbers ***************************************************/
  GPIO0_IRQn                =  32,      /*!< GPIO0 Interrupt               */
  GPIO0_EXP_IRQn            =  33,      /*!< GPIO0 EXP Interrupt           */
  GPIO1_IRQn                =  34,      /*!< GPIO1 Interrupt               */
  GPIO1_EXP_IRQn            =  35,      /*!< GPIO1 EXP Interrupt           */
  GPIO2_IRQn                =  36,      /*!< GPIO2 Interrupt               */
  GPIO2_EXP_IRQn            =  37,      /*!< GPIO2 EXP Interrupt           */
  GPIO3_IRQn                =  38,      /*!< GPIO3 Interrupt               */
  GPIO3_EXP_IRQn            =  39,      /*!< GPIO3 EXP Interrupt           */
  GPIO4_IRQn                =  40,      /*!< GPIO4 Interrupt               */
  GPIO4_EXP_IRQn            =  41,      /*!< GPIO4 EXP Interrupt           */
  I2C0_IRQn                 =  44,      /*!< I2C0 Interrupt                */
  I2C1_IRQn                 =  45,      /*!< I2C1 Interrupt                */
  I2C2_IRQn                 =  46,      /*!< I2C2 Interrupt                */
  I2C3_IRQn                 =  47,      /*!< I2C3 Interrupt                */
  I2C4_IRQn                 =  48,      /*!< I2C4 Interrupt                */
  I2C5_IRQn                 =  49,      /*!< I2C5 Interrupt                */
  PWM0_IRQn                 =  52,      /*!< PWM0 Interrupt                */
  PWM0_PWR_IRQn             =  53,      /*!< PWM0 PWR Interrupt            */
  PWM1_IRQn                 =  54,      /*!< PWM1 Interrupt                */
  PWM1_PWR_IRQn             =  55,      /*!< PWM1 PWR Interrupt            */
  PWM2_IRQn                 =  56,      /*!< PWM2 Interrupt                */
  PWM2_PWR_IRQn             =  57,      /*!< PWM2 PWR Interrupt            */
  PWM3_IRQn                 =  58,      /*!< PWM3 Interrupt                */
  PWM3_PWR_IRQn             =  59,      /*!< PWM3 PWR Interrupt            */
  UART0_IRQn                =  62,      /*!< UART0 Interrupt               */
  UART1_IRQn                =  63,      /*!< UART1 Interrupt               */
  UART2_IRQn                =  64,      /*!< UART2 Interrupt               */
  UART3_IRQn                =  65,      /*!< UART3 Interrupt               */
  UART4_IRQn                =  66,      /*!< UART4 Interrupt               */
  UART5_IRQn                =  67,      /*!< UART5 Interrupt               */
  UART6_IRQn                =  68,      /*!< UART6 Interrupt               */
  UART7_IRQn                =  69,      /*!< UART7 Interrupt               */
  UART8_IRQn                =  70,      /*!< UART8 Interrupt               */
  UART9_IRQn                =  71,      /*!< UART9 Interrupt               */
  SARADC0_IRQn              =  72,      /*!< SARADC0 Interrupt             */
  TIMER0_IRQn               =  77,      /*!< TIMER0 Interrupt              */
  TIMER1_IRQn               =  78,      /*!< TIMER1 Interrupt              */
  TIMER2_IRQn               =  79,      /*!< TIMER2 Interrupt              */
  TIMER3_IRQn               =  80,      /*!< TIMER3 Interrupt              */
  TIMER4_IRQn               =  81,      /*!< TIMER4 Interrupt              */
  TIMER5_IRQn               =  81,      /*!< TIMER5 Interrupt              */
  SPI0_IRQn                 =  84,      /*!< SPI0 Interrupt                */
  SPI1_IRQn                 =  85,      /*!< SPI1 Interrupt                */
  SPI2_IRQn                 =  86,      /*!< SPI2 Interrupt                */
  WDT_IRQn                  =  134,     /*!< WDT Interrupt                 */
  SARADC1_IRQn              =  156,     /*!< SARADC1 Interrupt             */
  FSPI0_IRQn                =  160,     /*!< FSPI0 Interrupt               */
  RSVD0_IRQn                =  280,     /*!< RSVD0 Interrupt               */
  NUM_INTERRUPTS            =  282,
} IRQn_Type;

#define RSVD_IRQn(_N)               (RSVD0_IRQn + (_N))

#endif

/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */

#define PLL_INPUT_32K_RATE       (32 * 1000)
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

#else /* HAL_AP_CORE */

#define __CORTEX_A                53U       /* Cortex-A53 Core */
#define __FPU_PRESENT             1U        /* FPU present */
#define __TIM_PRESENT             1U        /* Generic Timer */

#define CACHE_LINE_SHIFT          (6U)
#define CACHE_LINE_SIZE           (0x1U << CACHE_LINE_SHIFT)

#define HAL_GIC_V2                1U        /* GIC version 2 */

#endif

#ifndef __ASSEMBLY__
#include "cmsis_compiler.h"                 /* CMSIS compiler specific defines */
#ifdef __CORTEX_A
#include "core_ca.h"
#endif
#ifdef HAL_MCU_CORE
#include "core_cm0.h"
#endif
#include "system_rk3562.h"
#endif /* __ASSEMBLY__ */
#include "rk3562.h"

/****************************************************************************************/
/*                                                                                      */
/*                                Module Address Section                                */
/*                                                                                      */
/****************************************************************************************/
/* Memory Base */
#define GIC_DISTRIBUTOR_BASE            (0xFE901000) /* GICD base address */
#define GIC_CPU_INTERFACE_BASE          (0xFE902000) /* GICC base address */

/****************************************************************************************/
/*                                                                                      */
/*                               Register Bitmap Section                                */
/*                                                                                      */
/****************************************************************************************/

#ifdef HAL_AP_CORE
/********************************** CPU Topology ****************************************/
#define MPIDR_MT_MASK                      ((1U) << 24)
#define MPIDR_AFFLVL_MASK                  (0xFFU)
#define MPIDR_AFF0_SHIFT                   (0U)
#define MPIDR_AFF1_SHIFT                   (8U)
#define MPIDR_AFF2_SHIFT                   (16U)
#define MPIDR_AFFINITY_MASK                (0xFFFFFFU)
#define PLATFORM_CLUSTER0_CORE_COUNT       (4)
#define PLATFORM_CLUSTER1_CORE_COUNT       (0)
#define PLATFORM_CORE_COUNT                PLATFORM_CLUSTER0_CORE_COUNT
#define CPU_GET_AFFINITY(cpuId, clusterId) ((cpuId) << MPIDR_AFF0_SHIFT)

#endif /* HAL_AP_CORE */

#ifdef HAL_MCU_CORE
/*****************************************CACHE*****************************************/
/* CACHE LINE SIZE */
#define CACHE_LINE_SHIFT                (5U)
#define CACHE_LINE_SIZE                 (0x1U << CACHE_LINE_SHIFT)
#define CACHE_LINE_ADDR_MASK            (0xFFFFFFFFU << CACHE_LINE_SHIFT)
#define CACHE_M_CLEAN                   0x0U
#define CACHE_M_INVALID                 0x2U
#define CACHE_M_CLEAN_INVALID           0x4U
#define CACHE_M_INVALID_ALL             0x6U
#define CACHE_REVISION                  (0x00000100U)

#if defined(RKMCU_RK3562_BUS) && !defined(HAL_CACHE_DECODED_ADDR_BASE)
#error "Please define HAL_CACHE_DECODED_ADDR_BASE on hal_conf.h"
#endif

#endif /* HAL_MCU_CORE */

/****************************************************************************************/
/*                                                                                      */
/*                           Platform Differences Section                               */
/*                                                                                      */
/****************************************************************************************/

/******************************************CRU*******************************************/

#define PCLK_WDT0_GATE           PCLK_WDTNS_GATE
#define PCLK_WDT                 PCLK_BUS

#define CRU_CLK_USE_CON_BANK
#define CLK64(mux, div) ((((mux) & 0xffffffffULL) << 32) | ((div) & 0xffffffffULL))

#ifndef __ASSEMBLY__
typedef enum CLOCK_Name {
    /* TODO */
    CLK_INVALID = 0U,
    PLL_APLL,
    PLL_GPLL,
    PLL_VPLL,
    PLL_HPLL,
    PLL_CPLL,
    PLL_DPLL,

    HCLK_BUS             = CLK64(HCLK_BUS_SEL, HCLK_BUS_DIV),
    PCLK_BUS             = CLK64(PCLK_BUS_SEL, PCLK_BUS_DIV),

    CLK_PMU0_I2C0        = CLK64(CLK_PMU0_I2C0_SEL, CLK_PMU0_I2C0_DIV),
    CLK_I2C              = CLK64(CLK_I2C_SEL, 0U),

    CLK_PMU1_SPI0        = CLK64(CLK_PMU1_SPI0_SEL, CLK_PMU1_SPI0_DIV),
    CLK_SPI1             = CLK64(CLK_SPI1_SEL, 0U),
    CLK_SPI2             = CLK64(CLK_SPI2_SEL, 0U),
    SCLK_SFC             = CLK64(SCLK_SFC_SEL, SCLK_SFC_DIV),

    CLK_PMU1_UART0_SRC   = CLK64(0U, CLK_PMU1_UART0_SRC_DIV),
    CLK_PMU1_UART0_FRAC  = CLK64(0U, CLK_PMU1_UART0_FRAC_DIV),
    CLK_PMU1_UART0       = CLK64(SCLK_PMU1_UART0_SEL, 0U),
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

    CLK_PMU1_PWM0        = CLK64(CLK_PMU1_PWM0_SEL, CLK_PMU1_PWM0_DIV),
    CLK_PWM1_PERI        = CLK64(CLK_PWM1_PERI_SEL, 0U),
    CLK_PWM2_PERI        = CLK64(CLK_PWM2_PERI_SEL, 0U),
    CLK_PWM3_PERI        = CLK64(CLK_PWM3_PERI_SEL, 0U),
} eCLOCK_Name;
#endif /* __ASSEMBLY__ */

/****************************************MBOX********************************************/
#define MBOX_CNT                (2)
#define MBOX_CHAN_CNT           (4)

/****************************************GPIO********************************************/
#ifdef GPIO_VER_ID
#undef GPIO_VER_ID
#define GPIO_VER_ID             (0x01000C2BU)
#endif
/****************************************FSPI********************************************/
#define FSPI_CHIP_CNT           (2)

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __SOC_H */
