/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_bsp.h"

#ifdef HAL_I2STDM_MODULE_ENABLED
struct HAL_I2STDM_DEV g_i2sTdm0Dev =
{
    .pReg = I2STDM0,
    .mclkTx = CLK_I2S_TX,
    .mclkTxGate = CLK_I2S_TX_SRC_GATE,
    .mclkRx = CLK_I2S_RX,
    .mclkRxGate = CLK_I2S_RX_SRC_GATE,
    .hclk = HCLK_I2S_GATE,
    .pd = PD_BT_AUDIO,
    .bclkFs = 64,
    .rxDmaData =
    {
        .addr = (uint32_t)&(I2STDM0->RXDR),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_I2S0_RX,
        .dmac = DMA,
    },
    .txDmaData =
    {
        .addr = (uint32_t)&(I2STDM0->TXDR),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_I2S0_TX,
        .dmac = DMA,
    },
};
#endif

#ifdef HAL_PDM_MODULE_ENABLED
struct HAL_PDM_DEV g_pdm0Dev =
{
    .pReg = PDM0,
    .mclk = CLK_PDM,
    .mclkRate = PDM_CLK_RATE,
    .mclkGate = CLK_PDM_SRC_GATE,
    .hclk = HCLK_PDM_GATE,
    .pd = PD_BT_AUDIO,
    .reset = SRST_PDM,
    .rxDmaData =
    {
        .addr = (uint32_t)&(PDM0->RXFIFO_DATA_REG),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_PDM0,
        .dmac = DMA,
    },
};
#endif

#if defined(HAL_PINCTRL_MODULE_ENABLED)
static struct PINCTRL_BANK_INFO pinBanks[] = {
    PIN_BANK_CFG_FLAGS(GPIO_BANK0, 32, GRF_BASE,
                       0x0040, 2, 8,
                       0x0070, 2, 8,
                       0x00a0, 2, 8,
                       0x0000, 0, 0,
                       0x00d0, 1, 8),
    PIN_BANK_CFG_FLAGS(GPIO_BANK1, 32, GRF_BASE,
                       0x0050, 2, 8,
                       0x0080, 2, 8,
                       0x00b0, 2, 8,
                       0x0000, 0, 0,
                       0x00e0, 1, 8),
    PIN_BANK_CFG_FLAGS(GPIO_BANK2, 32, GRF_BASE,
                       0x0060, 2, 8,
                       0x0090, 2, 8,
                       0x00c0, 2, 8,
                       0x0000, 0, 0,
                       0x00f0, 1, 8),
};

const struct HAL_PINCTRL_DEV g_pinDev = {
    .banks = pinBanks,
    .banksNum = HAL_ARRAY_SIZE(pinBanks),
};
#endif

#ifdef HAL_PL330_MODULE_ENABLED
struct HAL_PL330_DEV g_pl330Dev =
{
    .pReg = DMA,
    .peripReqType = BURST,
    .irq[0] = DMAC_IRQn,
    .irq[1] = DMAC_ABORT_IRQn,
};
#endif

#ifdef HAL_UART_MODULE_ENABLED
const struct HAL_UART_DEV g_uart0Dev =
{
    .pReg = UART0,
    .sclkID = CLK_UART0,
    .sclkGateID = CLK_UART0_SRC_GATE,
    .pclkGateID = PCLK_UART0_GATE,
    .irqNum = UART0_IRQn,
    .isAutoFlow = true,
};

const struct HAL_UART_DEV g_uart1Dev =
{
    .pReg = UART1,
    .sclkID = CLK_UART1,
    .sclkGateID = CLK_UART1_SRC_GATE,
    .pclkGateID = PCLK_UART1_GATE,
    .irqNum = UART1_IRQn,
    .isAutoFlow = true,
};
#endif

#ifdef HAL_I2C_MODULE_ENABLED
const struct HAL_I2C_DEV g_i2c0Dev =
{
    .pReg = I2C0,
    .clkID = CLK_I2C0,
    .clkGateID = CLK_I2C0_SRC_GATE,
    .pclkGateID = PCLK_I2C0_GATE,
    .irqNum = I2CMST0_IRQn,
};

const struct HAL_I2C_DEV g_i2c1Dev =
{
    .pReg = I2C1,
    .clkID = CLK_I2C1,
    .clkGateID = CLK_I2C1_SRC_GATE,
    .pclkGateID = PCLK_I2C1_GATE,
    .irqNum = I2CMST1_IRQn,
};

const struct HAL_I2C_DEV g_i2c2Dev =
{
    .pReg = I2C2,
    .clkID = CLK_I2C2,
    .clkGateID = CLK_I2C2_SRC_GATE,
    .pclkGateID = PCLK_I2C2_GATE,
    .irqNum = I2CMST2_IRQn,
};
#endif

#if defined(HAL_PWM_MODULE_ENABLED)
const struct HAL_PWM_DEV g_pwm0Dev =
{
    .pReg = PWM,
    .clkID = CLK_PWM,
    .clkGateID = CLK_PWM_SRC_GATE,
    .pclkGateID = PCLK_PWM_GATE,
    .irqNum = PWM_IRQn,
};
#endif

#ifdef HAL_FSPI_MODULE_ENABLED
struct HAL_FSPI_HOST g_fspi0Dev =
{
    .instance = FSPI0,
    .sclkGate = CLK_SFC_SRC_GATE,
    .hclkGate = HCLK_SFC_GATE,
    .xipClkGate = HCLK_SFC_XIP_GATE,
    .sclkID = CLK_SFC,
    .irqNum = FSPI0_IRQn,
    .xipMemData = XIP_MAP0_BASE0,
    .xipMemCode = XIP_MAP0_BASE0,
    .xmmcDev[0] =
    {
        .type = DEV_NOR,
    },
};
#endif

#ifdef HAL_PVTM_MODULE_ENABLED
static const struct HAL_PVTM_INFO pvtmInfo[] =
{
    /* core pvtm */
    {
        .pvtmId = PVTM_ID_CORE,
        .clkGateID = CLK_PVTM_GATE,
        .con0 = &(GRF->DLL_CON0),
        .con1 = &(GRF->DLL_CON1),
        .sta0 = &(GRF->DLL_STATUS0),
        .sta1 = &(GRF->DLL_STATUS1),
        .startMask = GRF_DLL_CON0_PVTM_START_MASK,
        .startShift = GRF_DLL_CON0_PVTM_START_SHIFT,
        .enMask = GRF_DLL_CON0_OSC_EN_MASK,
        .enShift = GRF_DLL_CON0_OSC_EN_SHIFT,
        .selMask = GRF_DLL_CON0_PVTM_RING_SEL_MASK,
        .selShift = GRF_DLL_CON0_PVTM_RING_SEL_SHIFT,
        .doneMask = GRF_DLL_STATUS0_PVTM_FREQ_DONE_MASK,
        .doneShift = GRF_DLL_STATUS0_PVTM_FREQ_DONE_SHIFT,
    },
};

const struct HAL_PVTM_DEV g_pvtmDev =
{
    .info = pvtmInfo,
    .num = HAL_ARRAY_SIZE(pvtmInfo),
};
#endif

#ifdef HAL_SPI_MODULE_ENABLED
const struct HAL_SPI_DEV g_spi1Dev = {
    .base = SPI1_BASE,
    .clkId = CLK_SPI1,
    .clkGateID = CLK_SPI1_GATE,
    .pclkGateID = PCLK_SPI1_GATE,
    .irqNum = SPIMST1_IRQn,
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

void BSP_DeInit(void)
{
}

void BSP_Init(void)
{
}
