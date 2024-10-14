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
    DMA_REQ_UART0_RX = 4,
    DMA_REQ_UART0_TX = 5,
    DMA_REQ_UART1_RX = 6,
    DMA_REQ_UART1_TX = 7,
    DMA_REQ_UART2_RX = 8,
    DMA_REQ_UART2_TX = 9,
    DMA_REQ_UART3_RX = 10,
    DMA_REQ_UART3_TX = 11,
    DMA_REQ_UART4_RX = 12,
    DMA_REQ_UART4_TX = 13,
    DMA_REQ_UART5_RX = 14,
    DMA_REQ_UART5_TX = 15,
    DMA_REQ_PWM0     = 16,
    DMA_REQ_PWM1     = 17,
    DMA_REQ_PWM2     = 18,
    DMA_REQ_I2S0_RX  = 19,
    DMA_REQ_I2S0_TX  = 20,
    DMA_REQ_I2S1_RX  = 21,
    DMA_REQ_I2S1_TX  = 22,
    DMA_REQ_I2S2_RX  = 23,
    DMA_REQ_I2S2_TX  = 24,
    DMA_REQ_PDM      = 25,
    DMA_REQ_AUDIOPWM = 26,
} DMA_REQ_Type;

/* ================================================================================ */
/* ================                       IRQ                      ================ */
/* ================================================================================ */
#ifdef HAL_MCU_CORE
typedef enum
{
/******  Platform Exceptions Numbers ***************************************************/
  I2C0_IRQn              = 4,      /*!< I2C0 Interrupt              */
  I2C1_IRQn              = 5,      /*!< I2C1 Interrupt              */
  I2C2_IRQn              = 6,      /*!< I2C2 Interrupt              */
  I2C3_IRQn              = 7,      /*!< I2C3 Interrupt              */
  I2C4_IRQn              = 8,      /*!< I2C4 Interrupt              */
  I2C5_IRQn              = 9,      /*!< I2C5 Interrupt              */
  SPI0_IRQn              = 10,      /*!< SPI0 Interrupt              */
  SPI1_IRQn              = 11,      /*!< SPI1 Interrupt              */
  UART2_IRQn             = 14,      /*!< UART2 Interrupt             */
  TIMER0_IRQn            = 24,      /*!< TIMER0 Interrupt            */
  TIMER1_IRQn            = 25,      /*!< TIMER1 Interrupt            */
  TIMER2_IRQn            = 26,      /*!< TIMER2 Interrupt            */
  TIMER3_IRQn            = 27,      /*!< TIMER3 Interrupt            */
  TIMER4_IRQn            = 28,      /*!< TIMER4 Interrupt            */
  TIMER5_IRQn            = 29,      /*!< TIMER5 Interrupt            */
  GPIO0_IRQn             = 34,      /*!< GPIO0 Interrupt             */
  GPIO1_IRQn             = 35,      /*!< GPIO1 Interrupt             */
  GPIO2_IRQn             = 36,      /*!< GPIO2 Interrupt             */
  GPIO3_IRQn             = 37,      /*!< GPIO3 Interrupt             */
  GPIO4_IRQn             = 38,      /*!< GPIO4 Interrupt             */
  SARADC_IRQn            = 40,      /*!< SARADC Interrupt	     */
  ISP_IRQn               = 52,      /*!< ISP Interrupt               */
  MI_ISP_IRQn            = 54,      /*!< MI ISP Interrupt            */
  MIPI_ISP_IRQn          = 55,      /*!< MIPI ISP Interrupt          */
  VOP_IRQn               = 59,      /*!< VOP Interrupt               */
  MAILBOX0_AP_IRQn       = 111,     /*!< MAILBOX0_AP Interrupt       */
  MAILBOX0_BB_IRQn       = 112,     /*!< MAILBOX0_BB Interrupt       */
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
  I2C0_IRQn              = 36,      /*!< I2C0 Interrupt              */
  I2C1_IRQn              = 37,      /*!< I2C1 Interrupt              */
  I2C2_IRQn              = 38,      /*!< I2C2 Interrupt              */
  I2C3_IRQn              = 39,      /*!< I2C3 Interrupt              */
  I2C4_IRQn              = 40,      /*!< I2C4 Interrupt              */
  I2C5_IRQn              = 41,      /*!< I2C5 Interrupt              */
  SPI0_IRQn              = 42,      /*!< SPI0 Interrupt              */
  SPI1_IRQn              = 43,      /*!< SPI1 Interrupt              */
  UART2_IRQn             = 46,      /*!< UART2 Interrupt             */
  TIMER0_IRQn            = 56,      /*!< TIMER0 Interrupt            */
  TIMER1_IRQn            = 57,      /*!< TIMER1 Interrupt            */
  TIMER2_IRQn            = 58,      /*!< TIMER2 Interrupt            */
  TIMER3_IRQn            = 59,      /*!< TIMER3 Interrupt            */
  TIMER4_IRQn            = 60,      /*!< TIMER4 Interrupt            */
  TIMER5_IRQn            = 61,      /*!< TIMER5 Interrupt            */
  GPIO0_IRQn             = 66,      /*!< GPIO0 Interrupt             */
  GPIO1_IRQn             = 67,      /*!< GPIO1 Interrupt             */
  GPIO2_IRQn             = 68,      /*!< GPIO2 Interrupt             */
  GPIO3_IRQn             = 69,      /*!< GPIO3 Interrupt             */
  GPIO4_IRQn             = 70,      /*!< GPIO4 Interrupt             */
  SARADC_IRQn            = 72,      /*!< SARADC Interrupt	     */
  ISP_IRQn               = 84,      /*!< ISP Interrupt               */
  MI_ISP_IRQn            = 86,      /*!< MI ISP Interrupt            */
  MIPI_ISP_IRQn          = 87,      /*!< MIPI ISP Interrupt          */
  VOP_IRQn               = 91,      /*!< VOP Interrupt               */
  MAILBOX0_AP_IRQn       = 143,     /*!< MAILBOX0_AP Interrupt       */
  MAILBOX0_BB_IRQn       = 144,     /*!< MAILBOX0_BB Interrupt       */
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
#include "system_rv1126.h"
#endif /* __ASSEMBLY__ */
#include "rv1126.h"
#include "rv1126_pinctrl.h"

/****************************************************************************************/
/*                                                                                      */
/*                               Register Bitmap Section                                */
/*                                                                                      */
/****************************************************************************************/
/******************************************CRU*******************************************/
#define CRU_SRST_CON_CNT    15
#define CRU_GATE_CON_CNT    25
#define CRU_CLK_DIV_CON_CNT 78
#define CRU_CLK_SEL_CON_CNT 78
#define CLK(mux, div) \
    (((mux) & 0x0F0F00FFU) | (((div) & 0xFFU) << 8) | (((div) & 0x0F0F0000U) << 4))
#ifndef __ASSEMBLY__
typedef enum CLOCK_Name {
    CLK_INVALID = 0U,
    PLL_APLL,
    PLL_CPLL,
    PLL_HPLL,
    PLL_GPLL,
    ARMCLK,
    PCLK_DBG        = CLK(0U, PCLK_DBG_PRE_DIV),
    HCLK_PDCORE     = CLK(0U, HCLK_PDCORE_NIU_DIV),
    ACLK_PDBUS      = CLK(ACLK_PDBUS_PRE_SEL, ACLK_PDBUS_PRE_DIV),
    HCLK_PDBUS      = CLK(HCLK_PDBUS_PRE_SEL, HCLK_PDBUS_PRE_DIV),
    PCLK_PDBUS      = CLK(PCLK_PDBUS_PRE_SEL, PCLK_PDBUS_PRE_DIV),
    ACLK_PDPHP      = CLK(ACLK_PDPHP_SEL, ACLK_PDPHP_DIV),
    HCLK_PDPHP      = CLK(0U, HCLK_PDPHP_DIV),
    HCLK_PDAUDIO    = CLK(0U, HCLK_PDAUDIO_DIV),
    PCLK_PDPMU      = CLK(0U, PCLK_PDPMU_PRE_DIV),
    CLK_SPI0        = CLK(CLK_SPI0_SEL, CLK_SPI0_DIV),
    CLK_SPI1        = CLK(CLK_SPI1_SEL, CLK_SPI1_DIV),
    CLK_UART2_SRC   = CLK(SCLK_UART2_DIV_SEL, SCLK_UART2_DIV_DIV),
    CLK_UART2_FRAC  = CLK(0U, SCLK_UART2_FRAC_DIV),
    CLK_UART2       = CLK(SCLK_UART2_SEL, 0U),
    CLK_I2C0        = CLK(0U, CLK_I2C0_DIV),
    CLK_I2C1        = CLK(0U, CLK_I2C1_DIV),
    CLK_I2C2        = CLK(0U, CLK_I2C2_DIV),
    CLK_I2C3        = CLK(0U, CLK_I2C3_DIV),
    CLK_I2C4        = CLK(0U, CLK_I2C4_DIV),
    CLK_I2C5        = CLK(0U, CLK_I2C5_DIV),
    ACLK_PDVO       = CLK(ACLK_PDVO_SEL, ACLK_PDVO_DIV),
    DCLK_VOP        = CLK(DCLK_VOP_DIV_SEL, DCLK_VOP_DIV_DIV),
    ACLK_PDVI       = CLK(ACLK_PDVI_SEL, ACLK_PDVI_DIV_DIV),
    CLK_ISP         = CLK(CLK_ISP_DIV_SEL, CLK_ISP_DIV_DIV),
    ACLK_PDISPP     = CLK(ACLK_PDISPP_DIV_SEL, ACLK_PDISPP_DIV_DIV),
    CLK_ISPP        = CLK(CLK_ISPP_DIV_SEL, CLK_ISPP_DIV_DIV),
    CLK_OSC0_DIV32K = CLK(0U, CLK_OSC0_DIV32K_DIV),
    CLK_RTC32K      = CLK(CLK_DEEPSLOW_SEL, 0U),
} eCLOCK_Name;

#define ACLK_VOP   ACLK_PDVO
#define DCLK_VOP_S DCLK_VOP

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __SOC_H */
