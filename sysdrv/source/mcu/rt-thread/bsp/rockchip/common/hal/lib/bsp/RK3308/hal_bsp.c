/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#include "hal_bsp.h"

#ifdef HAL_ACODEC_MODULE_ENABLED
struct HAL_ACODEC_DEV g_acodecDev =
{
    .pReg = ACODEC,
    .hclk = PCLK_ACODEC_GATE,
    .mclk = 12288000,
    .micBias = RK3308_ADC_MICBIAS_VOLT_0_85,
    .micDifferential = true,
};
#endif

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

#ifdef HAL_I2S_MODULE_ENABLED
struct HAL_I2S_DEV g_i2s0Dev =
{
    .pReg = I2S0,
    .mclk = CLK_I2S0_2CH,
    .mclkGate = CLK_I2S0_2CH_CLK_GATE,
    .hclk = HCLK_I2S0_2CH_GATE,
    .bclkFs = 64,
    .rxDmaData =
    {
        .addr = (uint32_t)&(I2S0->RXDR),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_I2S_2CH_0_RX,
        .dmac = DMA1,
    },
    .txDmaData =
    {
        .addr = (uint32_t)&(I2S0->TXDR),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_I2S_2CH_0_TX,
        .dmac = DMA1,
    },
};

struct HAL_I2S_DEV g_i2s1Dev =
{
    .pReg = I2S1,
    .mclk = CLK_I2S1_2CH,
    .mclkGate = CLK_I2S1_2CH_CLK_GATE,
    .hclk = HCLK_I2S1_2CH_GATE,
    .bclkFs = 64,
    .rxDmaData =
    {
        .addr = (uint32_t)&(I2S1->RXDR),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_I2S_2CH_1_RX,
        .dmac = DMA1,
    },
    .txDmaData =
    {
        .addr = (uint32_t)&(I2S1->TXDR),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_I2S_2CH_1_TX,
        .dmac = DMA1,
    },
};
#endif

#ifdef HAL_I2STDM_MODULE_ENABLED
struct HAL_I2STDM_DEV g_i2sTdm0Dev =
{
    .pReg = I2STDM0,
    .mclkTx = CLK_I2S0_8CH_TX,
    .mclkTxGate = CLK_I2S0_8CH_TX_CLK_GATE,
    .mclkRx = CLK_I2S0_8CH_RX,
    .mclkRxGate = CLK_I2S0_8CH_RX_CLK_GATE,
    .muxTxSel = CLK_I2S0_8CH_TX_RX_CLK_SEL,
    .muxRxSel = CLK_I2S0_8CH_RX_TX_CLK_SEL,
    .hclk = HCLK_I2S0_8CH_GATE,
    .rsts[0] = SRST_M_I2S0_8CH_TX,
    .rsts[1] = SRST_M_I2S0_8CH_RX,
    .bclkFs = 64,
    .rxDmaData =
    {
        .addr = (uint32_t)&(I2STDM0->RXDR),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_I2S_8CH_0_RX,
        .dmac = DMA1,
    },
    .txDmaData =
    {
        .addr = (uint32_t)&(I2STDM0->TXDR),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_I2S_8CH_0_TX,
        .dmac = DMA1,
    },
};

struct HAL_I2STDM_DEV g_i2sTdm1Dev =
{
    .pReg = I2STDM1,
    .mclkTx = CLK_I2S1_8CH_TX,
    .mclkTxGate = CLK_I2S1_8CH_TX_CLK_GATE,
    .mclkRx = CLK_I2S1_8CH_RX,
    .mclkRxGate = CLK_I2S1_8CH_RX_CLK_GATE,
    .muxTxSel = CLK_I2S1_8CH_TX_RX_CLK_SEL,
    .muxRxSel = CLK_I2S1_8CH_RX_TX_CLK_SEL,
    .hclk = HCLK_I2S1_8CH_GATE,
    .rsts[0] = SRST_M_I2S1_8CH_TX,
    .rsts[1] = SRST_M_I2S1_8CH_RX,
    .bclkFs = 64,
    .rxDmaData =
    {
        .addr = (uint32_t)&(I2STDM1->RXDR),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_I2S_8CH_1_RX,
        .dmac = DMA1,
    },
    .txDmaData =
    {
        .addr = (uint32_t)&(I2STDM1->TXDR),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_I2S_8CH_1_TX,
        .dmac = DMA1,
    },
};

struct HAL_I2STDM_DEV g_i2sTdm2Dev =
{
    .pReg = I2STDM2,
    .mclkTx = CLK_I2S2_8CH_TX,
    .mclkTxGate = CLK_I2S2_8CH_TX_CLK_GATE,
    .mclkRx = CLK_I2S2_8CH_RX,
    .mclkRxGate = CLK_I2S2_8CH_RX_CLK_GATE,
    .muxTxSel = CLK_I2S2_8CH_TX_RX_CLK_SEL,
    .muxRxSel = CLK_I2S2_8CH_RX_TX_CLK_SEL,
    .hclk = HCLK_I2S2_8CH_GATE,
    .rsts[0] = SRST_M_I2S2_8CH_TX,
    .rsts[1] = SRST_M_I2S2_8CH_RX,
    .bclkFs = 64,
    .rxDmaData =
    {
        .addr = (uint32_t)&(I2STDM2->RXDR),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_I2S_8CH_2_RX,
        .dmac = DMA1,
    },
    .txDmaData =
    {
        .addr = (uint32_t)&(I2STDM2->TXDR),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_I2S_8CH_2_TX,
        .dmac = DMA1,
    },
};
#endif

#ifdef HAL_PDM_MODULE_ENABLED
struct HAL_PDM_DEV g_pdm0Dev =
{
    .pReg = PDM0,
    .mclk = CLK_PDM,
    .mclkRate = PDM_CLK_RATE,
    .mclkGate = CLK_PDM_CLK_GATE,
    .hclk = HCLK_PDM_GATE,
    .reset = SRST_M_PDM,
    .rxDmaData =
    {
        .addr = (uint32_t)&(PDM0->RXFIFO_DATA_REG),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_PDM0_RX,
        .dmac = DMA1,
    },
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

struct HAL_PL330_DEV g_pl330Dev1 =
{
    .pReg = DMA1,
    .peripReqType = BURST,
    .irq[0] = DMAC1_IRQn,
    .irq[1] = DMAC1_ABORT_IRQn,
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

const struct HAL_PWM_DEV g_pwm2Dev =
{
    .pReg = PWM2,
    .clkID = CLK_PWM2,
    .clkGateID = CLK_PWM2_PLL_CLK_GATE,
    .pclkGateID = PCLK_PWM2_GATE,
    .irqNum = PWM2_PWR_IRQn,
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

const struct HAL_SPI_DEV g_spi2Dev = {
    .base = SPI2_BASE,
    .clkId = CLK_SPI2,
    .clkGateID = CLK_SPI2_PLL_CLK_GATE,
    .pclkGateID = PCLK_SPI2_GATE,
    .irqNum = SPI2_IRQn,
    .isSlave = false,
    .txDma = {
        .channel = DMA_REQ_SPI2_TX,
        .direction = DMA_MEM_TO_DEV,
        .addr = SPI2_BASE + 0x400,
        .dmac = DMA1,
    },
    .rxDma = {
        .channel = DMA_REQ_SPI2_RX,
        .direction = DMA_DEV_TO_MEM,
        .addr = SPI2_BASE + 0x800,
        .dmac = DMA1,
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

#ifdef HAL_GMAC1000_MODULE_ENABLED
const struct HAL_GMAC_DEV g_gmac0Dev =
{
    .pReg = GMAC0,
    .clkID = CLK_MAC,
    .clkGateID = CLK_MAC_TXRX_GATE,
    .pclkID = PCLK_PERI,
    .pclkGateID = PCLK_MAC_GATE,
    .irqNum = MAC_IRQn,
};
#endif

void BSP_SetLoaderFlag(void)
{
    GRF->OS_REG0 = LDR_UPGRADE_FLAG;
}

void BSP_SetMaskRomFlag(void)
{
    GRF->OS_REG0 = SYS_UPGRADE_FLAG;
}

void BSP_Init(void)
{
}
