/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_bsp.h"

#ifdef HAL_PL330_MODULE_ENABLED
struct HAL_PL330_DEV g_pl330Dev0 =
{
    .pReg = DMA0,
    .peripReqType = BURST,
    .irq[0] = DMAC0_IRQn,
    .irq[1] = DMAC0_ABORT_IRQn,
    .pd = 0,
};

struct HAL_PL330_DEV g_pl330Dev1 =
{
    .pReg = DMA1,
    .peripReqType = BURST,
    .irq[0] = DMAC1_IRQn,
    .irq[1] = DMAC1_ABORT_IRQn,
    .pd = 0,
};
#endif

#ifdef HAL_SPI_MODULE_ENABLED
const struct HAL_SPI_DEV g_spi0Dev = {
    .base = SPI0_BASE,
    .clkId = CLK_SPI0,
    .clkGateID = CLK_SPI0_GATE,
    .pclkGateID = PCLK_SPI0_GATE,
    .irqNum = SPI0_IRQn,
    .isSlave = false,
    .txDma = {
        .channel = DMA_REQ_SPI0_TX,
        .direction = DMA_MEM_TO_DEV,
        .addr = SPI0_BASE + 0x400,
        .dmac = DMA0,
    },
    .rxDma = {
        .channel = DMA_REQ_SPI0_RX,
        .direction = DMA_DEV_TO_MEM,
        .addr = SPI0_BASE + 0x800,
        .dmac = DMA0,
    },
};

const struct HAL_SPI_DEV g_spi1Dev = {
    .base = SPI1_BASE,
    .clkId = CLK_SPI1,
    .clkGateID = CLK_SPI1_GATE,
    .pclkGateID = PCLK_SPI1_GATE,
    .irqNum = SPI1_IRQn,
    .isSlave = false,
    .txDma = {
        .channel = DMA_REQ_SPI1_TX,
        .direction = DMA_MEM_TO_DEV,
        .addr = SPI1_BASE + 0x400,
        .dmac = DMA0,
    },
    .rxDma = {
        .channel = DMA_REQ_SPI1_RX,
        .direction = DMA_DEV_TO_MEM,
        .addr = SPI1_BASE + 0x800,
        .dmac = DMA0,
    },
};

const struct HAL_SPI_DEV g_spi2Dev = {
    .base = SPI2_BASE,
    .clkId = CLK_SPI2,
    .clkGateID = CLK_SPI2_GATE,
    .pclkGateID = PCLK_SPI2_GATE,
    .irqNum = SPI2_IRQn,
    .isSlave = false,
    .txDma = {
        .channel = DMA_REQ_SPI2_TX,
        .direction = DMA_MEM_TO_DEV,
        .addr = SPI2_BASE + 0x400,
        .dmac = DMA0,
    },
    .rxDma = {
        .channel = DMA_REQ_SPI2_RX,
        .direction = DMA_DEV_TO_MEM,
        .addr = SPI2_BASE + 0x800,
        .dmac = DMA0,
    },
};

const struct HAL_SPI_DEV g_spi3Dev = {
    .base = SPI3_BASE,
    .clkId = CLK_SPI3,
    .clkGateID = CLK_SPI3_GATE,
    .pclkGateID = PCLK_SPI3_GATE,
    .irqNum = SPI3_IRQn,
    .isSlave = false,
    .txDma = {
        .channel = DMA_REQ_SPI3_TX,
        .direction = DMA_MEM_TO_DEV,
        .addr = SPI3_BASE + 0x400,
        .dmac = DMA0,
    },
    .rxDma = {
        .channel = DMA_REQ_SPI3_RX,
        .direction = DMA_DEV_TO_MEM,
        .addr = SPI3_BASE + 0x800,
        .dmac = DMA0,
    },
};
#endif

#ifdef HAL_UART_MODULE_ENABLED
const struct HAL_UART_DEV g_uart0Dev =
{
    .pReg = UART0,
    .sclkID = CLK_UART0,
    .irqNum = UART0_IRQn,
    .isAutoFlow = false,
};

const struct HAL_UART_DEV g_uart1Dev =
{
    .pReg = UART1,
    .sclkID = CLK_UART1,
    .sclkGateID = SCLK_UART1_GATE,
    .pclkGateID = PCLK_UART1_GATE,
    .irqNum = UART1_IRQn,
    .isAutoFlow = false,
};

const struct HAL_UART_DEV g_uart2Dev =
{
    .pReg = UART2,
    .sclkID = CLK_UART2,
    .sclkGateID = SCLK_UART2_GATE,
    .pclkGateID = PCLK_UART2_GATE,
    .irqNum = UART2_IRQn,
    .isAutoFlow = false,
};

const struct HAL_UART_DEV g_uart3Dev =
{
    .pReg = UART3,
    .sclkID = CLK_UART3,
    .sclkGateID = SCLK_UART3_GATE,
    .pclkGateID = PCLK_UART3_GATE,
    .irqNum = UART3_IRQn,
    .isAutoFlow = false,
};

const struct HAL_UART_DEV g_uart4Dev =
{
    .pReg = UART4,
    .sclkID = CLK_UART4,
    .sclkGateID = SCLK_UART4_GATE,
    .pclkGateID = PCLK_UART4_GATE,
    .irqNum = UART4_IRQn,
    .isAutoFlow = false,
};

const struct HAL_UART_DEV g_uart5Dev =
{
    .pReg = UART5,
    .sclkID = CLK_UART5,
    .sclkGateID = SCLK_UART5_GATE,
    .pclkGateID = PCLK_UART5_GATE,
    .irqNum = UART5_IRQn,
    .isAutoFlow = false,
};

const struct HAL_UART_DEV g_uart6Dev =
{
    .pReg = UART6,
    .sclkID = CLK_UART6,
    .sclkGateID = SCLK_UART6_GATE,
    .pclkGateID = PCLK_UART6_GATE,
    .irqNum = UART6_IRQn,
    .isAutoFlow = false,
};

const struct HAL_UART_DEV g_uart7Dev =
{
    .pReg = UART7,
    .sclkID = CLK_UART7,
    .sclkGateID = SCLK_UART7_GATE,
    .pclkGateID = PCLK_UART7_GATE,
    .irqNum = UART7_IRQn,
    .isAutoFlow = false,
};

const struct HAL_UART_DEV g_uart8Dev =
{
    .pReg = UART8,
    .sclkID = CLK_UART8,
    .sclkGateID = SCLK_UART8_GATE,
    .pclkGateID = PCLK_UART8_GATE,
    .irqNum = UART8_IRQn,
    .isAutoFlow = false,
};

const struct HAL_UART_DEV g_uart9Dev =
{
    .pReg = UART9,
    .sclkID = CLK_UART9,
    .sclkGateID = SCLK_UART9_GATE,
    .pclkGateID = PCLK_UART9_GATE,
    .irqNum = UART9_IRQn,
    .isAutoFlow = false,
};
#endif

#ifdef HAL_I2C_MODULE_ENABLED
const struct HAL_I2C_DEV g_i2c0Dev =
{
    .pReg = I2C0,
    .irqNum = I2C0_IRQn,
    .clkID = CLK_I2C,
    .clkGateID = CLK_I2C0_GATE,
    .pclkGateID = PCLK_I2C0_GATE,
    .runtimeID = PM_RUNTIME_ID_I2C0,
};

const struct HAL_I2C_DEV g_i2c1Dev =
{
    .pReg = I2C1,
    .irqNum = I2C1_IRQn,
    .clkID = CLK_I2C,
    .clkGateID = CLK_I2C1_GATE,
    .pclkGateID = PCLK_I2C1_GATE,
    .runtimeID = PM_RUNTIME_ID_I2C1,
};

const struct HAL_I2C_DEV g_i2c2Dev =
{
    .pReg = I2C2,
    .irqNum = I2C2_IRQn,
    .clkID = CLK_I2C,
    .clkGateID = CLK_I2C2_GATE,
    .pclkGateID = PCLK_I2C2_GATE,
    .runtimeID = PM_RUNTIME_ID_I2C2,
};

const struct HAL_I2C_DEV g_i2c3Dev =
{
    .pReg = I2C3,
    .irqNum = I2C3_IRQn,
    .clkID = CLK_I2C,
    .clkGateID = CLK_I2C3_GATE,
    .pclkGateID = PCLK_I2C3_GATE,
    .runtimeID = PM_RUNTIME_ID_I2C3,
};

const struct HAL_I2C_DEV g_i2c4Dev =
{
    .pReg = I2C4,
    .irqNum = I2C4_IRQn,
    .clkID = CLK_I2C,
    .clkGateID = CLK_I2C4_GATE,
    .pclkGateID = PCLK_I2C4_GATE,
    .runtimeID = PM_RUNTIME_ID_I2C4,
};

const struct HAL_I2C_DEV g_i2c5Dev =
{
    .pReg = I2C5,
    .irqNum = I2C5_IRQn,
    .clkID = CLK_I2C,
    .clkGateID = CLK_I2C5_GATE,
    .pclkGateID = PCLK_I2C5_GATE,
    .runtimeID = PM_RUNTIME_ID_I2C5,
};
#endif

#ifdef HAL_FSPI_MODULE_ENABLED
struct HAL_FSPI_HOST g_fspi0Dev =
{
    .instance = FSPI,
    .sclkGate = SCLK_SFC_GATE,
    .hclkGate = HCLK_SFC_GATE,
    .xipClkGate = 0,
    .sclkID = 0,
    .irqNum = FSPI0_IRQn,
    .xipMemCode = 0,
    .xipMemData = 0,
    .xmmcDev[0] =
    {
        .type = 0,
    },
};
#endif

#ifdef HAL_CANFD_MODULE_ENABLED
const struct HAL_CANFD_DEV g_can0Dev =
{
    .pReg = CAN0,
    .sclkID = CLK_CAN0,
    .sclkGateID = CLK_CAN0_GATE,
    .pclkGateID = PCLK_CAN0_GATE,
    .irqNum = CAN0_IRQn,
};

const struct HAL_CANFD_DEV g_can1Dev =
{
    .pReg = CAN1,
    .sclkID = CLK_CAN1,
    .sclkGateID = CLK_CAN1_GATE,
    .pclkGateID = PCLK_CAN1_GATE,
    .irqNum = CAN1_IRQn,
};

const struct HAL_CANFD_DEV g_can2Dev =
{
    .pReg = CAN2,
    .sclkID = CLK_CAN2,
    .sclkGateID = CLK_CAN2_GATE,
    .pclkGateID = PCLK_CAN2_GATE,
    .irqNum = CAN2_IRQn,
};
#endif

#ifdef HAL_GMAC_MODULE_ENABLED
const struct HAL_GMAC_DEV g_gmac0Dev =
{
    .pReg = GMAC0,
    .clkID = CLK_MAC0_2TOP,
    .clkGateID = CLK_MAC0_2TOP_GATE,
    .pclkID = PCLK_PHP,
    .pclkGateID = PCLK_GMAC0_GATE,
    .irqNum = GMAC0_IRQn,
};

const struct HAL_GMAC_DEV g_gmac1Dev =
{
    .pReg = GMAC1,
    .clkID = CLK_MAC1_2TOP,
    .clkGateID = CLK_MAC1_2TOP_GATE,
    .pclkID = PCLK_USB,
    .pclkGateID = PCLK_GMAC1_GATE,
    .irqNum = GMAC1_IRQn,
};
#endif

#ifdef HAL_PCIE_MODULE_ENABLED
struct HAL_PCIE_DEV g_pcieDev =
{
    .apbBase = PCIE3X2_APB_BASE,
    .dbiBase = PCIE3X2_DBI_BASE,
    .cfgBase = 0xF0000000,
    .lanes = 2,
    .gen = 3,
    .firstBusNo = 0x20,
    .legacyIrqNum = PCIE30x2_LEGACY_IRQn,
};
#endif

#ifdef HAL_PWM_MODULE_ENABLED
const struct HAL_PWM_DEV g_pwm0Dev =
{
    .pReg = PWM0,
    .clkID = 0,
    .clkGateID = CLK_PWM0_GATE,
    .pclkGateID = PCLK_PWM0_GATE,
    .irqNum = PWM_PMU_IRQn,
};

const struct HAL_PWM_DEV g_pwm1Dev =
{
    .pReg = PWM1,
    .clkID = CLK_PWM1,
    .clkGateID = CLK_PWM1_GATE,
    .pclkGateID = PCLK_PWM1_GATE,
    .irqNum = PWM1_IRQn,
};

const struct HAL_PWM_DEV g_pwm2Dev =
{
    .pReg = PWM2,
    .clkID = CLK_PWM2,
    .clkGateID = CLK_PWM2_GATE,
    .pclkGateID = PCLK_PWM2_GATE,
    .irqNum = PWM2_IRQn,
};

const struct HAL_PWM_DEV g_pwm3Dev =
{
    .pReg = PWM3,
    .clkID = CLK_PWM3,
    .clkGateID = CLK_PWM3_GATE,
    .pclkGateID = PCLK_PWM3_GATE,
    .irqNum = PWM3_IRQn,
};
#endif

void BSP_Init(void)
{
}
