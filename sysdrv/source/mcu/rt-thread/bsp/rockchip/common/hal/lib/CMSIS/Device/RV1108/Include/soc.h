/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2018-2020 Rockchip Electronics Co., Ltd.
 */

#ifndef __SOC_H
#define __SOC_H
#ifdef __cplusplus
  extern "C" {
#endif

/* IO definitions (access restrictions to peripheral registers) */
#ifdef __cplusplus
  #define   __I     volatile             /*!< \brief Defines 'read only' permissions */
#else
  #define   __I     volatile const       /*!< \brief Defines 'read only' permissions */
#endif
#define     __O     volatile             /*!< \brief Defines 'write only' permissions */
#define     __IO    volatile             /*!< \brief Defines 'read / write' permissions */

#define __CORTEX_A           7U    /* Cortex-A7 Core                                */

/* ================================================================================ */
/* ================                       IRQ                      ================ */
/* ================================================================================ */
typedef enum {
    DMA_REQ_I2S1_TX = 0,
    DMA_REQ_I2S1_RX = 1,
    DMA_REQ_UART0_TX = 2,
    DMA_REQ_UART0_RX = 3,
    DMA_REQ_UART1_TX = 4,
    DMA_REQ_UART1_RX = 5,
    DMA_REQ_UART2_TX = 6,
    DMA_REQ_UART2_RX = 7,
    DMA_REQ_SPI_TX = 8,
    DMA_REQ_SPI_RX = 9,
    DMA_REQ_I2S0_TX = 11,
    DMA_REQ_I2S0_RX = 12,
    DMA_REQ_PWM0_TX = 13,
    DMA_REQ_PWM1_TX = 14,
    DMA_REQ_I2S2_TX = 15,
    DMA_REQ_I2S2_RX = 16,
} DMA_REQ_Type;

/* ================================================================================ */
/* ================                       IRQ                      ================ */
/* ================================================================================ */
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
  TIMER0_IRQn            = 67,      /*!< TIMER0 Interrupt            */
  TIMER1_IRQn            = 68,      /*!< TIMER1 Interrupt            */
  GPIO0_IRQn             = 72,      /*!< GPIO0 Interrupt             */
  GPIO1_IRQn             = 73,      /*!< GPIO1 Interrupt             */
  GPIO2_IRQn             = 74,      /*!< GPIO2 Interrupt             */
  GPIO3_IRQn             = 75,      /*!< GPIO3 Interrupt             */
  UART0_IRQn             = 76,      /*!< UART0 Interrupt             */
  UART2_IRQn             = 78,      /*!< UART1 Interrupt             */
  SFC_IRQn               = 88,      /*!< SFC Interrupt               */
  NUM_INTERRUPTS         = 96,
} IRQn_Type;

/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */
#ifndef __ASSEMBLY__
#include "cmsis_compiler.h"               /* CMSIS compiler specific defines */
#include "system_rv1108.h"
#endif /* __ASSEMBLY__ */
#include "rv1108.h"


/****************************************************************************************/
/*                                                                                      */
/*                               Register Bitmap Section                                */
/*                                                                                      */
/****************************************************************************************/
/******************************************SPI*******************************************/
#define SPI_VERSION    0x0B050001

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __SOC_H */
