/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    common.h
  * @version V0.0.1
  * @date    6-July-2022
  * @brief   image signal processing (ISP) abstract device
  *
  ******************************************************************************
  */
#ifndef __COMMON_H__
#define __COMMON_H__
#include <stdbool.h>

#ifndef BIT
#define BIT(nr)         (1 << (nr))
#endif

#define TEST_FPGA_MIPI_DPHY_ENABLE              0

typedef void (*NVIC_IRQHandler)(void);

#define __I  volatile const   /*!< Defines 'read only' permissions    */
#define __O  volatile         /*!< Defines 'write only' permissions   */
#define __IO volatile         /*!< Defines 'read / write' permissions */

#define roundup(x, y)                                   ({\
                                                            (((x) + (y - 1)) / y) * y;\
                                                         })

static inline void writeb(uint8_t value, uint32_t address)
{
    *((volatile uint8_t *) address) = value;
}
static inline void writew(uint16_t value, uint32_t address)
{
    *((volatile uint16_t *) address) = value;
}
static inline void writel(uint32_t value, uint32_t address)
{
    *((volatile uint32_t *) address) = value;
}

static inline uint8_t readb(uint32_t address)
{
    return *((volatile uint8_t *)address);
}

static inline uint16_t readw(uint32_t address)
{
    return *((volatile uint16_t *)address);
}

static inline uint32_t readl(uint32_t address)
{
    return *((volatile uint32_t *)address);
}

#endif
