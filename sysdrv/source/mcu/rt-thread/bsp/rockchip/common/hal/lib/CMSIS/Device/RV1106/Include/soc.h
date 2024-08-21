/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2022 Rockchip Electronics Co., Ltd.
 */

#ifndef __SOC_H
#define __SOC_H
#ifdef __cplusplus
  extern "C" {
#endif

#include "hal_conf.h"

/* IO definitions (access restrictions to peripheral registers) */
#ifdef __cplusplus
  #define   __I     volatile             /*!< brief Defines 'read only' permissions */
#else
  #define   __I     volatile const       /*!< brief Defines 'read only' permissions */
#endif
#define     __O     volatile             /*!< brief Defines 'write only' permissions */
#define     __IO    volatile             /*!< brief Defines 'read / write' permissions */

/* ================================================================================ */
/* ================                    DMA REQ                      =============== */
/* ================================================================================ */
typedef enum {
    DMA_REQ_SPI0_RX  = 0,
    DMA_REQ_SPI0_TX  = 1,
    DMA_REQ_SPI1_RX  = 2,
    DMA_REQ_SPI1_TX  = 3,
    DMA_REQ_UART0_RX = 6,
    DMA_REQ_UART0_TX = 7,
    DMA_REQ_UART1_RX = 8,
    DMA_REQ_UART1_TX = 9,
    DMA_REQ_UART2_RX = 10,
    DMA_REQ_UART2_TX = 11,
    DMA_REQ_UART3_RX = 12,
    DMA_REQ_UART3_TX = 13,
    DMA_REQ_UART4_RX = 14,
    DMA_REQ_UART4_TX = 15,
    DMA_REQ_UART5_RX = 16,
    DMA_REQ_UART5_TX = 17,
    DMA_REQ_PWM0     = 18,
    DMA_REQ_PWM1     = 19,
    DMA_REQ_PWM2     = 20,
    DMA_REQ_I2S0_8CH_RX  = 21,
    DMA_REQ_I2S0_8CH_TX  = 22,
} DMA_REQ_Type;

/* ================================================================================ */
/* ================                       IRQ                      ================ */
/* ================================================================================ */
#ifdef HAL_MCU_CORE
typedef enum
{
/******  Platform Exceptions Numbers ***************************************************/
  CACHE_IRQn             = 0,       /*!< Cache Interrupt             */
  MAILBOX0_AP_IRQn       = 1,       /*!< MAILBOX0_AP Interrupt       */
  MAILBOX0_BB_IRQn       = 2,       /*!< MAILBOX0_BB Interrupt       */
  GPIO0_IRQn             = 5,       /*!< GPIO0 Interrupt             */
  GPIO0_EXT_IRQn         = 6,       /*!< GPIO0_EXT Interrupt         */
  GPIO1_IRQn             = 7,       /*!< GPIO1 Interrupt             */
  GPIO1_EXT_IRQn         = 8,       /*!< GPIO1_EXT Interrupt         */
  GPIO2_IRQn             = 9,       /*!< GPIO2 Interrupt             */
  GPIO2_EXT_IRQn         = 10,      /*!< GPIO2_EXT Interrupt         */
  GPIO3_IRQn             = 11,      /*!< GPIO3 Interrupt             */
  GPIO3_EXT_IRQn         = 12,      /*!< GPIO3_EXT Interrupt         */
  GPIO4_IRQn             = 13,      /*!< GPIO4 Interrupt             */
  GPIO4_EXT_IRQn         = 14,      /*!< GPIO4_EXT Interrupt         */
  I2C0_IRQn              = 18,      /*!< I2C1 Interrupt              */
  I2C1_IRQn              = 19,      /*!< I2C1 Interrupt              */
  I2C2_IRQn              = 20,      /*!< I2C2 Interrupt              */
  I2C3_IRQn              = 21,      /*!< I2C3 Interrupt              */
  I2C4_IRQn              = 22,      /*!< I2C4 Interrupt              */
  SPI0_IRQn              = 23,      /*!< SPI0 Interrupt              */
  SPI1_IRQn              = 24,      /*!< SPI1 Interrupt              */
  UART0_IRQn             = 25,      /*!< UART0 Interrupt             */
  UART1_IRQn             = 26,      /*!< UART1 Interrupt             */
  UART2_IRQn             = 27,      /*!< UART2 Interrupt             */
  UART3_IRQn             = 28,      /*!< UART3 Interrupt             */
  UART4_IRQn             = 29,      /*!< UART4 Interrupt             */
  UART5_IRQn             = 30,      /*!< UART5 Interrupt             */
  PWM0_IRQn              = 31,      /*!< PWM0  Interrupt             */
  PWM0_PWR_IRQn          = 32,      /*!< PWM0_PWR  Interrupt         */
  PWM1_IRQn              = 33,      /*!< PWM1  Interrupt             */
  PWM1_PWR_IRQn          = 34,      /*!< PWM1_PWR  Interrupt         */
  PWM2_IRQn              = 35,      /*!< PWM2  Interrupt             */
  PWM2_PWR_IRQn          = 36,      /*!< PWM3_PWR  Interrupt         */
  TIMER0_IRQn            = 38,      /*!< TIMER0 Interrupt            */
  TIMER1_IRQn            = 39,      /*!< TIMER1 Interrupt            */
  TIMER2_IRQn            = 40,      /*!< TIMER2 Interrupt            */
  TIMER3_IRQn            = 41,      /*!< TIMER3 Interrupt            */
  TIMER4_IRQn            = 42,      /*!< TIMER4 Interrupt            */
  TIMER5_IRQn            = 43,      /*!< TIMER5 Interrupt            */
  SARADC_IRQn            = 62,      /*!< SARADC Interrupt            */
  VOP_IRQn               = 89,      /*!< VOP Interrupt               */
  MIPI_ISP_IRQn          = 103,     /*!< MIPI ISP Interrupt          */
  MI_ISP_IRQn            = 104,     /*!< MI ISP Interrupt            */
  ISP_IRQn               = 105,     /*!< ISP Interrupt               */
  NUM_INTERRUPTS         = 128,
} IRQn_Type;
#else
typedef enum
{
/*******  SGI Interrupts Numbers  ***************************************************/
  SGI0_IRQn              =  0,
  SGI1_IRQn              =  1,
  SGI2_IRQn              =  2,
  SGI3_IRQn              =  3,
  SGI4_IRQn              =  4,
  SGI5_IRQn              =  5,
  SGI6_IRQn              =  6,
  SGI7_IRQn              =  7,
  SGI8_IRQn              =  8,
  SGI9_IRQn              =  9,
  SGI10_IRQn             = 10,
  SGI11_IRQn             = 11,
  SGI12_IRQn             = 12,
  SGI13_IRQn             = 13,
  SGI14_IRQn             = 14,
  SGI15_IRQn             = 15,

/******  Cortex-A7 Processor Exceptions Numbers ****************************************/
  SecurePhyTimer_IRQn    = 29,      /*!< Physical Timer Interrupt    */
  NoSecurePhyTimer_IRQn  = 30,    /*!< Physical Timer Interrupt    */

/******  Platform Exceptions Numbers ***************************************************/
  CACHE_IRQn             = 32,      /*!< Cache Interrupt             */
  MAILBOX0_AP_IRQn       = 33,      /*!< MAILBOX0_AP Interrupt       */
  MAILBOX0_BB_IRQn       = 34,      /*!< MAILBOX0_BB Interrupt       */
  GPIO0_IRQn             = 37,      /*!< GPIO0 Interrupt             */
  GPIO0_EXT_IRQn         = 38,      /*!< GPIO0_EXT Interrupt         */
  GPIO1_IRQn             = 39,      /*!< GPIO1 Interrupt             */
  GPIO1_EXT_IRQn         = 40,      /*!< GPIO1_EXT Interrupt         */
  GPIO2_IRQn             = 41,      /*!< GPIO2 Interrupt             */
  GPIO2_EXT_IRQn         = 42,      /*!< GPIO2_EXT Interrupt         */
  GPIO3_IRQn             = 43,      /*!< GPIO3 Interrupt             */
  GPIO3_EXT_IRQn         = 44,      /*!< GPIO3_EXT Interrupt         */
  GPIO4_IRQn             = 45,      /*!< GPIO4 Interrupt             */
  GPIO4_EXT_IRQn         = 46,      /*!< GPIO4_EXT Interrupt         */
  I2C0_IRQn              = 50,      /*!< I2C1 Interrupt              */
  I2C1_IRQn              = 51,      /*!< I2C1 Interrupt              */
  I2C2_IRQn              = 52,      /*!< I2C2 Interrupt              */
  I2C3_IRQn              = 53,      /*!< I2C3 Interrupt              */
  I2C4_IRQn              = 54,      /*!< I2C4 Interrupt              */
  SPI0_IRQn              = 55,      /*!< SPI0 Interrupt              */
  SPI1_IRQn              = 56,      /*!< SPI1 Interrupt              */
  UART0_IRQn             = 57,      /*!< UART0 Interrupt             */
  UART1_IRQn             = 58,      /*!< UART1 Interrupt             */
  UART2_IRQn             = 59,      /*!< UART2 Interrupt             */
  UART3_IRQn             = 60,      /*!< UART3 Interrupt             */
  UART4_IRQn             = 61,      /*!< UART4 Interrupt             */
  UART5_IRQn             = 62,      /*!< UART5 Interrupt             */
  PWM0_IRQn              = 63,      /*!< PWM0  Interrupt             */
  PWM0_PWR_IRQn          = 64,      /*!< PWM0_PWR  Interrupt         */
  PWM1_IRQn              = 65,      /*!< PWM1  Interrupt             */
  PWM1_PWR_IRQn          = 66,      /*!< PWM1_PWR  Interrupt         */
  PWM2_IRQn              = 67,      /*!< PWM2  Interrupt             */
  PWM2_PWR_IRQn          = 68,      /*!< PWM3_PWR  Interrupt         */
  TIMER0_IRQn            = 70,      /*!< TIMER0 Interrupt            */
  TIMER1_IRQn            = 71,      /*!< TIMER1 Interrupt            */
  TIMER2_IRQn            = 72,      /*!< TIMER2 Interrupt            */
  TIMER3_IRQn            = 73,      /*!< TIMER3 Interrupt            */
  TIMER4_IRQn            = 74,      /*!< TIMER4 Interrupt            */
  TIMER5_IRQn            = 75,      /*!< TIMER5 Interrupt            */
  SARADC_IRQn            = 94,      /*!< SARADC Interrupt	           */
  VOP_IRQn               = 121,     /*!< VOP Interrupt               */
  MIPI_ISP_IRQn          = 135,     /*!< MIPI ISP Interrupt          */
  MI_ISP_IRQn            = 136,     /*!< MI ISP Interrupt            */
  ISP_IRQn               = 137,     /*!< ISP Interrupt               */
  NUM_INTERRUPTS         = 256,
} IRQn_Type;
#endif
/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */

#if defined(HAL_AP_CORE) && defined(HAL_MCU_CORE)
#error "HAL_AP_CORE and HAL_MCU_CORE only one of them can be enabled"
#endif

#if !defined(HAL_AP_CORE) && !defined(HAL_MCU_CORE)
#error "Please define HAL_AP_CORE or HAL_MCU_CORE on hal_conf.h"
#endif

#ifdef HAL_AP_CORE
#define __CORTEX_A            7U          /* Cortex-A7 Core                  */
#define __FPU_PRESENT         1U          /* FPU present                     */
#else
#define __RISC_V
#endif

#ifndef __ASSEMBLY__
#include "cmsis_compiler.h"               /* CMSIS compiler specific defines */
#ifdef __CORTEX_A
#include "core_ca.h"
#endif
#include "system_rv1106.h"
#endif /* __ASSEMBLY__ */
#include "rv1106.h"

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
/****************************************MBOX********************************************/
#define MBOX_CNT             1
#define MBOX_CHAN_CNT        4

/****************************************************************************************/
/*                                                                                      */
/*                               Register Bitmap Section                                */
/*                                                                                      */
/****************************************************************************************/
/******************************************CRU*******************************************/
#define PLL_INPUT_OSC_RATE   (24 * 1000 * 1000)
#define CRU_CLK_USE_CON_BANK
#define CLK64(mux, div) ((((mux) & 0xffffffffULL) << 32) | ((div) & 0xffffffffULL))

#ifndef __ASSEMBLY__
typedef enum CLOCK_Name {
    CLK_INVALID = 0U,
    PLL_APLL,
    PLL_CPLL,
    PLL_GPLL,
    ARMCLK,
    PCLK_DBG             = CLK64(0U, PCLK_DBG_DIV),
    CLK_UART0_SRC        = CLK64(CLK_UART0_SRC_SEL, CLK_UART0_SRC_DIV),
    CLK_UART0_FRAC       = CLK64(0U, CLK_UART0_FRAC_DIV),
    CLK_UART0            = CLK64(SCLK_UART0_SRC_SEL, 0U),
    CLK_UART1_SRC        = CLK64(CLK_UART1_SRC_SEL, CLK_UART1_SRC_DIV),
    CLK_UART1_FRAC       = CLK64(0U, CLK_UART1_FRAC_DIV),
    CLK_UART1            = CLK64(SCLK_UART1_SRC_SEL, 0U),
    CLK_UART2_SRC        = CLK64(CLK_UART2_SRC_SEL, CLK_UART2_SRC_DIV),
    CLK_UART2_FRAC       = CLK64(0U, CLK_UART2_FRAC_DIV),
    CLK_UART2            = CLK64(SCLK_UART2_SRC_SEL, 0U),
    CLK_UART3_SRC        = CLK64(CLK_UART3_SRC_SEL, CLK_UART3_SRC_DIV),
    CLK_UART3_FRAC       = CLK64(0U, CLK_UART3_FRAC_DIV),
    CLK_UART3            = CLK64(SCLK_UART3_SRC_SEL, 0U),
    CLK_UART4_SRC        = CLK64(CLK_UART4_SRC_SEL, CLK_UART4_SRC_DIV),
    CLK_UART4_FRAC       = CLK64(0U, CLK_UART4_FRAC_DIV),
    CLK_UART4            = CLK64(SCLK_UART4_SRC_SEL, 0U),
    CLK_UART5_SRC        = CLK64(CLK_UART5_SRC_SEL, CLK_UART5_SRC_DIV),
    CLK_UART5_FRAC       = CLK64(0U, CLK_UART5_FRAC_DIV),
    CLK_UART5            = CLK64(SCLK_UART5_SRC_SEL, 0U),
    CLK_I2S0_8CH_TX_SRC  = CLK64(CLK_I2S0_8CH_TX_SRC_SEL, CLK_I2S0_8CH_TX_SRC_DIV),
    CLK_I2S0_8CH_TX_FRAC = CLK64(0U, CLK_I2S0_8CH_TX_FRAC_DIV),
    CLK_I2S0_8CH_TX      = CLK64(MCLK_I2S0_8CH_TX_SRC_PERI_SEL, 0U),
    CLK_I2S0_8CH_RX_SRC  = CLK64(CLK_I2S0_8CH_RX_SRC_SEL, CLK_I2S0_8CH_RX_SRC_DIV),
    CLK_I2S0_8CH_RX_FRAC = CLK64(0U, CLK_I2S0_8CH_RX_FRAC_DIV),
    CLK_I2S0_8CH_RX      = CLK64(MCLK_I2S0_8CH_RX_SRC_PERI_SEL, 0U),
    DCLK_VOP             = CLK64(DCLK_VOP_SRC_SEL, DCLK_VOP_SRC_DIV),
    PCLK_TOP_ROOT        = CLK64(PCLK_TOP_ROOT_SEL, 0U),
    CLK_REF_MIPI0_SRC    = CLK64(CLK_REF_MIPI0_SRC_SEL, CLK_REF_MIPI0_SRC_DIV),
    CLK_REF_MIPI0_FRAC   = CLK64(0U, CLK_REF_MIPI0_FRAC_DIV),
    CLK_REF_MIPI0        = CLK64(CLK_REF_MIPI0_OUT_SEL, 0U),
    CLK_REF_MIPI1_SRC    = CLK64(CLK_REF_MIPI1_SRC_SEL, CLK_REF_MIPI1_SRC_DIV),
    CLK_REF_MIPI1_FRAC   = CLK64(0U, CLK_REF_MIPI1_FRAC_DIV),
    CLK_REF_MIPI1        = CLK64(CLK_REF_MIPI1_OUT_SEL, 0U),
    CLK_VICAP_M0_SRC     = CLK64(CLK_VICAP_M0_SRC_SEL, CLK_VICAP_M0_SRC_DIV),
    CLK_VICAP_M0_FRAC    = CLK64(0U, CLK_VICAP_M0_FRAC_DIV),
    CLK_VICAP_M0         = CLK64(CLK_VICAP_M0_OUT_SEL, 0U),
    CLK_VICAP_M1_SRC     = CLK64(CLK_VICAP_M1_SRC_SEL, CLK_VICAP_M1_SRC_DIV),
    CLK_VICAP_M1_FRAC    = CLK64(0U, CLK_VICAP_M1_FRAC_DIV),
    CLK_VICAP_M1         = CLK64(CLK_VICAP_M1_OUT_SEL, 0U),
    HCLK_NPU_ROOT        = CLK64(HCLK_NPU_ROOT_SEL, 0U),
    ACLK_NPU_ROOT        = CLK64(ACLK_NPU_ROOT_SEL, 0U),
    PCLK_NPU_ROOT        = CLK64(PCLK_NPU_ROOT_SEL, 0U),
    HCLK_PERI_ROOT       = CLK64(HCLK_PERI_ROOT_SEL, 0U),
    ACLK_PERI_ROOT       = CLK64(ACLK_PERI_ROOT_SEL, 0U),
    PCLK_PERI_ROOT       = CLK64(PCLK_PERI_ROOT_SEL, 0U),
    CLK_I2C0             = CLK64(CLK_I2C0_SEL, 0U),
    CLK_I2C2             = CLK64(CLK_I2C2_SEL, 0U),
    CLK_I2C3             = CLK64(CLK_I2C3_SEL, 0U),
    CLK_I2C4             = CLK64(CLK_I2C4_SEL, 0U),
    CLK_SPI1             = CLK64(CLK_SPI1_SEL, 0U),
    CLK_PWM0             = CLK64(CLK_PWM0_PERI_SEL, 0U),
    CLK_PWM1             = CLK64(CLK_PWM1_PERI_SEL, 0U),
    CLK_PWM2             = CLK64(CLK_PWM2_PERI_SEL, 0U),
    CLK_EMMC             = CLK64(CCLK_SRC_EMMC_SEL, 0U),
    CLK_SFC              = CLK64(SCLK_SFC_SEL, 0U),
    ACLK_BUS_ROOT        = CLK64(ACLK_BUS_ROOT_SEL, 0U),
    CLK_I2C1             = CLK64(CLK_I2C1_SEL, 0U),
    HCLK_VEPU_ROOT       = CLK64(HCLK_VEPU_ROOT_SEL, 0U),
    ACLK_VEPU_ROOT       = CLK64(ACLK_VEPU_ROOT_SEL, 0U),
    PCLK_VEPU_ROOT       = CLK64(PCLK_VEPU_ROOT_SEL, 0U),
    ACLK_VEPU_COM_ROOT   = CLK64(ACLK_VEPU_COM_ROOT_SEL, 0U),
    CLK_CORE_VEPU        = CLK64(CLK_CORE_VEPU_SEL, 0U),
    CLK_SPI0             = CLK64(CLK_SPI0_SEL, 0U),
    HCLK_VI_ROOT         = CLK64(HCLK_VI_ROOT_SEL, 0U),
    ACLK_VI_ROOT         = CLK64(ACLK_VI_ROOT_SEL, 0U),
    PCLK_VI_ROOT         = CLK64(PCLK_VI_ROOT_SEL, 0U),
    DCLK_VICAP           = CLK64(DCLK_VICAP_SEL, 0U),
    CLK_SDMMC            = CLK64(CCLK_SRC_SDMMC_SEL, 0U),
    HCLK_VO_ROOT         = CLK64(HCLK_VO_ROOT_SEL, 0U),
    ACLK_VO_ROOT         = CLK64(ACLK_VO_ROOT_SEL, 0U),
    PCLK_VO_ROOT         = CLK64(PCLK_VO_ROOT_SEL, 0U),
    CLK_CORE_RGA2E       = CLK64(CLK_CORE_RGA2E_SEL, 0U),
    ACLK_VOP_ROOT        = CLK64(ACLK_VOP_ROOT_SEL, 0U),
    ACLK_MAC_ROOT        = CLK64(ACLK_MAC_ROOT_SEL, 0U),
    CLK_SDIO             = CLK64(CCLK_SRC_SDIO_SEL, 0U),
} eCLOCK_Name;

#endif

/****************************************GPIO********************************************/
#ifdef GPIO_VER_ID
#undef GPIO_VER_ID
#define GPIO_VER_ID             (0x01000C2BU)
#endif

#define PCLK_SARADC_CONTROL_GATE PCLK_SARADC_GATE

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __SOC_H */
