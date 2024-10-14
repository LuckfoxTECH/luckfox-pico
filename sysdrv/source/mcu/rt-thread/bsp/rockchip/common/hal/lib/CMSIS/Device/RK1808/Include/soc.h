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
/* ================                       IRQ                      ================ */
/* ================================================================================ */
typedef enum {
    DMA_REQ_UART0_TX = 0,
    DMA_REQ_UART0_RX = 1,
    DMA_REQ_UART1_TX = 2,
    DMA_REQ_UART1_RX = 3,
    DMA_REQ_UART2_TX = 4,
    DMA_REQ_UART2_RX = 5,
    DMA_REQ_UART3_TX = 6,
    DMA_REQ_UART3_RX = 7,
    DMA_REQ_UART4_TX = 8,
    DMA_REQ_UART4_RX = 9,
    DMA_REQ_SPI0_TX = 10,
    DMA_REQ_SPI0_RX = 11,
    DMA_REQ_SPI1_TX = 12,
    DMA_REQ_SPI1_RX = 13,
    DMA_REQ_SPI2_TX = 14,
    DMA_REQ_SPI2_RX = 15,
    DMA_REQ_I2S0_TX = 16,
    DMA_REQ_I2S0_RX = 17,
    DMA_REQ_I2S1_TX = 18,
    DMA_REQ_I2S1_RX = 19,
    DMA_REQ_PWM0 = 21,
    DMA_REQ_PWM1 = 22,
    DMA_REQ_PWM2 = 23,
    DMA_REQ_PDM = 24,
    DMA_REQ_UART5_TX = 25,
    DMA_REQ_UART5_RX = 26,
    DMA_REQ_UART6_TX = 27,
    DMA_REQ_UART6_RX = 28,
    DMA_REQ_UART7_TX = 29,
    DMA_REQ_UART7_RX = 30
} DMA_REQ_Type;

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
  SecurePhyTimer_IRQn    = 29,    /*!< Physical Timer Interrupt    */
  NoSecurePhyTimer_IRQn  = 30,    /*!< Physical Timer Interrupt    */

/******  Platform Exceptions Numbers ***************************************************/
  DCF_IRQn               = 32,
  DMAC_IRQn              = 33,
  DMAC_ABORT_IRQn        = 34,
  GPIO0_IRQn             = 35,
  GPIO1_IRQn             = 36,
  GPIO2_IRQn             = 37,
  GPIO3_IRQn             = 38,
  I2C0_IRQn              = 39,
  I2C1_IRQn              = 40,
  I2C2_IRQn              = 41,
  I2S0_IRQn              = 42,   /*!< I2S 8CH Interrupt */
  I2S1_IRQn              = 43,   /*!< I2S 2CH Interrupt */
  PDM_IRQn               = 44,
  UART0_IRQn             = 45,
  UART1_IRQn             = 46,
  UART2_IRQn             = 47,
  UART3_IRQn             = 48,
  UART4_IRQn             = 49,
  EFUSENS_IRQn           = 50,
  SPI2APB_IRQn           = 53,
  SPI1_IRQn              = 54,
  SPI2_IRQn              = 55,
  TIMER0_IRQn            = 58,
  TIMER1_IRQn            = 59,
  TIMER2_IRQn            = 60,
  TIMER3_IRQn            = 61,
  TIMER4_IRQn            = 62,
  TIMER5_IRQn            = 63,
  NPU_IRQn               = 75,
  UART7_IRQn             = 76,
  UART6_IRQn             = 77,
  UART5_IRQn             = 78,
  SDMMC_IRQn             = 80,
  SDIO_IRQn              = 81,
  SFC_IRQn               = 82,
  EHCI_IRQn              = 85,
  OHCI_IRQn              = 86,
  SARADC_IRQn            = 96,
  VAD_IRQn               = 104,
  GPIO4_IRQn             = 114,
  I2C3_IRQn              = 115,
  I2C4_IRQn              = 133,
  I2C5_IRQn              = 134,

  NUM_INTERRUPTS         = 273
} IRQn_Type;
#endif /* __ASSEMBLY__ */

#define __FPU_PRESENT        1U
#define __CORTEX_A           7U        /* Cortex-A7 Core                          */
#ifndef __ASSEMBLY__
#include "core_ca.h"                   /* Cortex-A processor and core peripherals */
#include "system_rk1808.h"
#endif /* __ASSEMBLY__ */
#include "rk1808.h"
#include "rk1808_usb.h"

/****************************************************************************************/
/*                                                                                      */
/*                               Module Structure Section                               */
/*                                                                                      */
/****************************************************************************************/

/****************************************************************************************/
/*                                                                                      */
/*                                Module Address Section                                */
/*                                                                                      */
/****************************************************************************************/
/* Memory Base */
#define EHCI_BASE           0xffd80000U /* EHCI base address */
#define OHCI_BASE           0xffd90000U /* OHCI base address */

/****************************************************************************************/
/*                                                                                      */
/*                               Register Bitmap Section                                */
/*                                                                                      */
/****************************************************************************************/
/******************************************CRU*******************************************/
#define CRU_GATE_CON_CNT 20

/******************************************SPI*******************************************/
#define SPI_VERSION    0x0B050001

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __SOC_H */
