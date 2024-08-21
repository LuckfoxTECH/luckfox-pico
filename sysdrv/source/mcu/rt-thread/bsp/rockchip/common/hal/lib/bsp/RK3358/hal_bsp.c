/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2023 Rockchip Electronics Co., Ltd.
 */

#include "hal_bsp.h"

#ifdef HAL_I2C_MODULE_ENABLED
const struct HAL_I2C_DEV g_i2c0Dev =
{
    .pReg = I2C0,
    .irqNum = I2C0_IRQn,
    .clkID = CLK_I2C0,
    .clkGateID = CLK_I2C0_PLL_CLK_GATE,
    .pclkGateID = PCLK_I2C0_GATE,
    .runtimeID = PM_RUNTIME_ID_I2C0,
};

const struct HAL_I2C_DEV g_i2c1Dev =
{
    .pReg = I2C1,
    .irqNum = I2C1_IRQn,
    .clkID = CLK_I2C1,
    .clkGateID = CLK_I2C1_PLL_CLK_GATE,
    .pclkGateID = PCLK_I2C1_GATE,
    .runtimeID = PM_RUNTIME_ID_I2C1,
};

const struct HAL_I2C_DEV g_i2c2Dev =
{
    .pReg = I2C2,
    .irqNum = I2C2_IRQn,
    .clkID = CLK_I2C2,
    .clkGateID = CLK_I2C2_PLL_CLK_GATE,
    .pclkGateID = PCLK_I2C2_GATE,
    .runtimeID = PM_RUNTIME_ID_I2C2,
};

const struct HAL_I2C_DEV g_i2c3Dev =
{
    .pReg = I2C3,
    .irqNum = I2C3_IRQn,
    .clkID = CLK_I2C3,
    .clkGateID = CLK_I2C3_PLL_CLK_GATE,
    .pclkGateID = PCLK_I2C3_GATE,
    .runtimeID = PM_RUNTIME_ID_I2C3,
};
#endif

#ifdef HAL_PL330_MODULE_ENABLED
struct HAL_PL330_DEV g_pl330Dev0 =
{
    .pReg = DMA0,
    .peripReqType = BURST,
    .irq[0] = DMAC0_IRQn,
    .irq[1] = DMAC0_ABORT_IRQn,
    .pd = 0,
};
#endif

#ifdef HAL_PWM_MODULE_ENABLED
const struct HAL_PWM_DEV g_pwm0Dev =
{
    .pReg = PWM0,
    .clkID = CLK_PWM,
    .clkGateID = CLK_PWM_PLL_CLK_GATE,
    .pclkGateID = PCLK_PWM_GATE,
    .irqNum = PWM0_PWR_IRQn,
};

const struct HAL_PWM_DEV g_pwm1Dev =
{
    .pReg = PWM1,
    .clkID = CLK_PWM1,
    .clkGateID = CLK_PWM1_PLL_CLK_GATE,
    .pclkGateID = PCLK_PWM1_GATE,
    .irqNum = PWM1_PWR_IRQn,
};
#endif

#ifdef HAL_SFC_MODULE_ENABLED
struct HAL_SFC_HOST g_sfcDev =
{
    .instance = SFC,
    .sclkID = CLK_SFC,
    .irqNum = SFC_IRQn,
    .xmmcDev[0] =
    {
        .type = 0,
    },
};
#endif

#ifdef HAL_SPI_MODULE_ENABLED
const struct HAL_SPI_DEV g_spi0Dev = {
    .base = SPI0_BASE,
    .clkId = CLK_SPI0,
    .clkGateID = CLK_SPI0_PLL_CLK_GATE,
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
    .clkGateID = CLK_SPI1_PLL_CLK_GATE,
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
#endif

#ifdef HAL_UART_MODULE_ENABLED
const struct HAL_UART_DEV g_uart0Dev =
{
    .pReg = UART0,
    .sclkID = CLK_UART0,
    .pclkGateID = PCLK_UART0_GATE,
    .irqNum = UART0_IRQn,
    .isAutoFlow = false,
};

const struct HAL_UART_DEV g_uart1Dev =
{
    .pReg = UART1,
    .sclkID = CLK_UART1,
    .pclkGateID = PCLK_UART1_GATE,
    .irqNum = UART1_IRQn,
    .isAutoFlow = false,
};

const struct HAL_UART_DEV g_uart2Dev =
{
    .pReg = UART2,
    .sclkID = CLK_UART2,
    .pclkGateID = PCLK_UART2_GATE,
    .irqNum = UART2_IRQn,
    .isAutoFlow = false,
};

const struct HAL_UART_DEV g_uart3Dev =
{
    .pReg = UART3,
    .sclkID = CLK_UART3,
    .pclkGateID = PCLK_UART3_GATE,
    .irqNum = UART3_IRQn,
    .isAutoFlow = false,
};

const struct HAL_UART_DEV g_uart4Dev =
{
    .pReg = UART4,
    .sclkID = CLK_UART4,
    .pclkGateID = PCLK_UART4_GATE,
    .irqNum = UART4_IRQn,
    .isAutoFlow = false,
};
#endif

#if defined(HAL_PCD_MODULE_ENABLED) || defined(HAL_HCD_MODULE_ENABLED)
const struct HAL_USB_DEV g_usbdDev =
{
    .pReg = OTG,
    .hclkGateID = HCLK_USB2OTG_CLK_GATE,
    .utmiclkGateID = USBPHY_REF_CLK_GATE,
    .irqNum = OTG_IRQn,
    .cfg =
    {
        .epNum = 10,
        .ep0Mps = USB_OTG_MAX_EP0_SIZE,
        .phyif = USB_PHY_UTMI_WIDTH_16,
        .speed = USB_OTG_SPEED_HIGH,
        .hcNum = 8,
        .dmaEnable = true,
        .sofEnable = false,
        .lpmEnable = false,
        .vbusSensingEnable = false,
        .suspendEnable = false,
    },
};
#endif

#if defined(HAL_EHCI_MODULE_ENABLED) || defined(HAL_OHCI_MODULE_ENABLED)
const struct HAL_USBH_DEV g_usbhDev =
{
    .ehciReg = EHCI,
    .ohciReg = OHCI,
    .ehciIrqNum = EHCI_IRQn,
    .ohciIrqNum = OHCI_IRQn,
    .usbhGateID = HCLK_USB2HOST_CLK_GATE,
    .usbhArbGateID = HCLK_USB2HOST_ARB_CLK_GATE,
};
#endif

#ifdef HAL_GMAC1000_MODULE_ENABLED
const struct HAL_GMAC_DEV g_gmacDev =
{
    .pReg = GMAC,
    .clkID = CLK_GMAC,
    .clkGateID = ACLK_PDGMAC_CLK_GATE,
    .pclkID = PCLK_GMAC,
    .pclkGateID = PCLK_GMAC_CLK_GATE,
    .irqNum = GMAC0_IRQn,
};
#endif

void BSP_SetLoaderFlag(void)
{
    PMU_GRF->OS_REG[0] = LDR_UPGRADE_FLAG;
}

void BSP_SetMaskRomFlag(void)
{
    PMU_GRF->OS_REG[0] = SYS_UPGRADE_FLAG;
}

void BSP_Init(void)
{
}
