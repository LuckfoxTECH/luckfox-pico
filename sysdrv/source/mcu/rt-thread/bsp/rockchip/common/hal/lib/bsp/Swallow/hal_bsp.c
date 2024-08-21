/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_bsp.h"

#ifdef HAL_DWDMA_MODULE_ENABLED
struct HAL_DWDMA_DEV g_dwDmaDev =
{
    .pReg = DMA,
    .irq[0] = DMAC0_IRQn,
    .irq[1] = DMAC1_IRQn,
    .irq[2] = DMAC2_IRQn,
    .maxChans = DMA_NUM_CHANNELS,
    .dataWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
    .blockSize = 0xfff, /** ref to trm */
};
#endif

#ifdef HAL_UART_MODULE_ENABLED
const struct HAL_UART_DEV g_uart0Dev =
{
    .pReg = UART0,
    .sclkID = CLK_UART0,
    .irqNum = UART0_IRQn,
    .isAutoFlow = true,
    .runtimeID = PM_RUNTIME_ID_UART0,
};
#endif

#ifdef HAL_I2C_MODULE_ENABLED
const struct HAL_I2C_DEV g_i2c0Dev =
{
    .pReg = I2C0,
    .clkID = CLK_I2C0,
    .clkGateID = CLK_I2C0_PLL_GATE,
    .pclkGateID = PCLK_I2C0_GATE,
    .irqNum = I2C0_IRQn,
    .runtimeID = PM_RUNTIME_ID_I2C0,
};

const struct HAL_I2C_DEV g_i2c1Dev =
{
    .pReg = I2C1,
    .clkID = CLK_I2C1,
    .clkGateID = CLK_I2C1_PLL_GATE,
    .pclkGateID = PCLK_I2C1_GATE,
    .irqNum = I2C1_IRQn,
    .runtimeID = PM_RUNTIME_ID_I2C1,
};
#endif

#ifdef HAL_SPI_MODULE_ENABLED
const struct HAL_SPI_DEV g_spi0Dev = {
    .base = SPI0_BASE,
    .clkId = CLK_SPI0,
    .clkGateID = CLK_SPI0_PLL_GATE,
    .pclkGateID = PCLK_SPI0_GATE,
    .irqNum = SPI0_IRQn,
    .isSlave = false,
    .txDma = {
        .channel = DMA_REQ_SPI0_TX,
        .direction = DMA_MEM_TO_DEV,
        .addr = SPI0_BASE + 0x400,
        .dmac = DMA,
    },
    .rxDma = {
        .channel = DMA_REQ_SPI0_RX,
        .direction = DMA_DEV_TO_MEM,
        .addr = SPI0_BASE + 0x800,
        .dmac = DMA,
    },
};

const struct HAL_SPI_DEV g_spi1Dev = {
    .base = SPI1_BASE,
    .clkId = CLK_SPI1,
    .clkGateID = CLK_SPI1_PLL_GATE,
    .pclkGateID = PCLK_SPI1_GATE,
    .irqNum = SPI1_IRQn,
    .isSlave = false,
    .txDma = {
        .channel = DMA_REQ_SPI1_TX,
        .direction = DMA_MEM_TO_DEV,
        .addr = SPI1_BASE + 0x400,
        .dmac = DMA,
    },
    .rxDma = {
        .channel = DMA_REQ_SPI1_RX,
        .direction = DMA_DEV_TO_MEM,
        .addr = SPI1_BASE + 0x800,
        .dmac = DMA,
    },
};
#endif

#ifdef HAL_PCD_MODULE_ENABLED
const struct HAL_USB_DEV g_usbdDev =
{
    .pReg = USB,
    .hclkGateID = HCLK_USB_GATE,
    .utmiclkGateID = HCLK_USB_PMU_GATE,
    .irqNum = USB_IRQn,
    .cfg =
    {
        .epNum = 10,
        .ep0Mps = USB_OTG_MAX_EP0_SIZE,
        .phyif = USB_PHY_UTMI_WIDTH_16,
        .speed = PCD_SPEED_HIGH,
        .dmaEnable = true,
        .sofEnable = false,
        .lpmEnable = false,
        .vbusSensingEnable = false,
        .suspendEnable = false,
    },
};
#endif

#ifdef HAL_FSPI_MODULE_ENABLED
struct HAL_FSPI_HOST g_fspi0Dev =
{
    .instance = FSPI0,
    .sclkGate = CLK_XIP_SFC_DT50_GATE,
    .hclkGate = HCLK_SFC_GATE,
    .xipClkGate = HCLK_SFC_XIP_GATE,
    .sclkID = CLK_XIP_SFC,
    .irqNum = FSPI0_IRQn,
    .xipMemCode = XIP_MAP_BASE,
    .xipMemData = XIP_MAP_BASE,
    .xmmcDev[0] =
    {
        .type = DEV_NOR,
    },
};
#endif

#ifdef HAL_CKCAL_MODULE_ENABLED

const struct HAL_CKCAL_DEV g_ckcalDev =
{
    .pReg = CKCAL,
    .irqNum = CKCAL_IRQn,
};

#endif

#ifdef HAL_PCD_MODULE_ENABLED
static void BSP_U2PHY_Init(void)
{
    /*
     * Turn off differential receiver to save power, active low.
     * USBGRF, 0x0018: bit[2] = 1'b0
     */
    WRITE_REG_MASK_WE(USB_GRF->REG6, 0x4U, 0U);

    /* Turn off PHY port 1 to save power, not used */
    WRITE_REG_MASK_WE(USB_GRF->CON1, 0x1FFU, 0x1d1U);
}
#endif

#ifdef HAL_PWM_MODULE_ENABLED
const struct HAL_PWM_DEV g_pwm0Dev =
{
    .pReg = PWM0,
    .clkID = CLK_PWM,
    .clkGateID = CLK_PWM_PLL_GATE,
    .pclkGateID = PCLK_PWM_GATE,
    .irqNum = PWM_IRQn,
};
#endif

void BSP_Init(void)
{
#ifdef HAL_PCD_MODULE_ENABLED
    BSP_U2PHY_Init();
#endif
}

void BSP_SetLoaderFlag(void)
{
    GRF->OS_REG0 = SYS_UPGRADE_FLAG;
}
