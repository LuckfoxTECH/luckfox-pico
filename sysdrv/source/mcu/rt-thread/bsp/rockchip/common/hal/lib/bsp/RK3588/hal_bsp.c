/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_bsp.h"

#if defined(HAL_CRU_MODULE_ENABLED)
static struct CRU_BANK_INFO cruBanks[] = {
    CRU_BANK_CFG_FLAGS(CRU_BASE, 0x300, 0x800, 0xa00),
    CRU_BANK_CFG_FLAGS(PHPTOPCRU_BASE, 0x300, 0x800, 0xa00),
    CRU_BANK_CFG_FLAGS(SECURECRU_BASE, 0x300, 0x800, 0xa00),
    CRU_BANK_CFG_FLAGS(SBUSCRU_BASE, 0x300, 0x800, 0xa00),
    CRU_BANK_CFG_FLAGS(PMU1SCRU_BASE, 0x300, 0x800, 0xa00),
    CRU_BANK_CFG_FLAGS(PMU1CRU_BASE, 0x300, 0x800, 0xa00),
    CRU_BANK_CFG_FLAGS(DDR0CRU_BASE, 0x300, 0x800, 0xa00),
    CRU_BANK_CFG_FLAGS(DDR1CRU_BASE, 0x300, 0x800, 0xa00),
    CRU_BANK_CFG_FLAGS(DDR2CRU_BASE, 0x300, 0x800, 0xa00),
    CRU_BANK_CFG_FLAGS(DDR3CRU_BASE, 0x300, 0x800, 0xa00),
    CRU_BANK_CFG_FLAGS(BIGCORE0CRU_BASE, 0x300, 0x800, 0xa00),
    CRU_BANK_CFG_FLAGS(BIGCORE1CRU_BASE, 0x300, 0x800, 0xa00),
    CRU_BANK_CFG_FLAGS(DSUCRU_BASE, 0x300, 0x800, 0xa00),
};

const struct HAL_CRU_DEV g_cruDev = {
    .banks = cruBanks,
    .banksNum = HAL_ARRAY_SIZE(cruBanks),
};
#endif

#ifdef HAL_I2STDM_MODULE_ENABLED
struct HAL_I2STDM_DEV g_i2sTdm0Dev =
{
    .pReg = I2STDM0,
    .mclkTx = CLK_I2S0_8CH_TX,
    .mclkTxGate = CLK_I2S0_8CH_TX_GATE,
    .mclkRx = CLK_I2S0_8CH_RX,
    .mclkRxGate = CLK_I2S0_8CH_RX_GATE,
    .muxTxSel = MCLK_I2S0_8CH_TX_SEL,
    .muxRxSel = MCLK_I2S0_8CH_RX_SEL,
    .hclk = HCLK_I2S0_8CH_GATE,
    .rsts[0] = SRST_MRESETN_I2S0_8CH_TX,
    .rsts[1] = SRST_MRESETN_I2S0_8CH_RX,
    .bclkFs = 64,
    .pd = PD_AUDIO,
    .txDmaData =
    {
        .addr = (uint32_t)&(I2STDM0->TXDR),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_I2S0_8CH_TX,
        .dmac = DMA0,
    },
    .rxDmaData =
    {
        .addr = (uint32_t)&(I2STDM0->RXDR),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_I2S0_8CH_RX,
        .dmac = DMA0,
    },
};

struct HAL_I2STDM_DEV g_i2sTdm1Dev =
{
    .pReg = I2STDM1,
    .mclkTx = CLK_I2S1_8CH_TX,
    .mclkTxGate = CLK_I2S1_8CH_TX_GATE,
    .mclkRx = CLK_I2S1_8CH_RX,
    .mclkRxGate = CLK_I2S1_8CH_RX_GATE,
    .muxTxSel = MCLK_I2S1_8CH_TX_SEL,
    .muxRxSel = MCLK_I2S1_8CH_RX_SEL,
    .hclk = HCLK_I2S1_8CH_GATE,
    .rsts[0] = SRST_MRESETN_I2S1_8CH_TX,
    .rsts[1] = SRST_MRESETN_I2S1_8CH_RX,
    .bclkFs = 64,
    .txDmaData =
    {
        .addr = (uint32_t)&(I2STDM1->TXDR),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_I2S1_8CH_TX,
        .dmac = DMA0,
    },
    .rxDmaData =
    {
        .addr = (uint32_t)&(I2STDM1->RXDR),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_I2S1_8CH_RX,
        .dmac = DMA0,
    },
};
#endif

#ifdef HAL_I2S_MODULE_ENABLED
struct HAL_I2S_DEV g_i2s2Dev =
{
    .pReg = I2S2,
    .mclk = CLK_I2S2_2CH,
    .mclkGate = CLK_I2S2_2CH_GATE,
    .hclk = HCLK_I2S2_2CH_GATE,
    .bclkFs = 64,
    .pd = PD_AUDIO,
    .txDmaData =
    {
        .addr = (uint32_t)&(I2S2->TXDR),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_I2S2_2CH_TX,
        .dmac = DMA1,
    },
    .rxDmaData =
    {
        .addr = (uint32_t)&(I2S2->RXDR),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_I2S2_2CH_RX,
        .dmac = DMA1,
    },
};

struct HAL_I2S_DEV g_i2s3Dev =
{
    .pReg = I2S3,
    .mclk = CLK_I2S3_2CH,
    .mclkGate = CLK_I2S3_2CH_GATE,
    .hclk = HCLK_I2S3_2CH_GATE,
    .bclkFs = 64,
    .pd = PD_AUDIO,
    .txDmaData =
    {
        .addr = (uint32_t)&(I2S3->TXDR),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_I2S3_2CH_TX,
        .dmac = DMA1,
    },
    .rxDmaData =
    {
        .addr = (uint32_t)&(I2S3->RXDR),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_I2S3_2CH_RX,
        .dmac = DMA1,
    },
};
#endif

#ifdef HAL_PCIE_MODULE_ENABLED
const struct HAL_PHY_SNPS_PCIE3_DEV g_phy_pcie3Dev =
{
    .phyMode = PHY_MODE_PCIE_AGGREGATION,
};

struct HAL_PCIE_DEV g_pcieDev =
{
    .apbBase = PCIE3_4L_APB_BASE,
    .dbiBase = PCIE3_4L_DBI_BASE,
    .cfgBase = 0xF0000000,
    .lanes = 4,
    .gen = 3,
    .firstBusNo = 0,
    .legacyIrqNum = PCIE30x4_LEGACY_IRQn,
    .phy = (void *)&g_phy_pcie3Dev,
};
#endif

#ifdef HAL_PDM_MODULE_ENABLED
struct HAL_PDM_DEV g_pdm0Dev =
{
    .pReg = PDM0,
    .mclk = MCLK_PDM0,
    .mclkGate = MCLK_PDM0_GATE,
    .mclkRate = PDM_CLK_RATE,
    .hclk = HCLK_PDM0_GATE,
    .reset = SRST_RESETN_PDM0,
    .rxDmaData =
    {
        .addr = (uint32_t)&(PDM0->RXFIFO_DATA_REG),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_PDM0,
        .dmac = DMA0,
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
    .clkGate = ACLK_DMAC0_GATE,
    .pd = 0,
};

struct HAL_PL330_DEV g_pl330Dev1 =
{
    .pReg = DMA1,
    .peripReqType = BURST,
    .irq[0] = DMAC1_IRQn,
    .irq[1] = DMAC1_ABORT_IRQn,
    .clkGate = ACLK_DMAC1_GATE,
    .pd = 0,
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

const struct HAL_UART_DEV g_uart1Dev =
{
    .pReg = UART1,
    .sclkID = CLK_UART1,
    .irqNum = UART1_IRQn,
    .isAutoFlow = true,
    .runtimeID = PM_RUNTIME_ID_UART1,
};

const struct HAL_UART_DEV g_uart2Dev =
{
    .pReg = UART2,
    .sclkID = CLK_UART2,
    .irqNum = UART2_IRQn,
    .isAutoFlow = true,
    .runtimeID = PM_RUNTIME_ID_UART2,
};

const struct HAL_UART_DEV g_uart3Dev =
{
    .pReg = UART3,
    .sclkID = CLK_UART3,
    .irqNum = UART3_IRQn,
    .isAutoFlow = true,
    .runtimeID = PM_RUNTIME_ID_UART3,
};

const struct HAL_UART_DEV g_uart4Dev =
{
    .pReg = UART4,
    .sclkID = CLK_UART4,
    .irqNum = UART4_IRQn,
    .isAutoFlow = true,
    .runtimeID = PM_RUNTIME_ID_UART4,
};

const struct HAL_UART_DEV g_uart5Dev =
{
    .pReg = UART5,
    .sclkID = CLK_UART5,
    .irqNum = UART5_IRQn,
    .isAutoFlow = true,
    .runtimeID = PM_RUNTIME_ID_UART5,
};

const struct HAL_UART_DEV g_uart6Dev =
{
    .pReg = UART6,
    .sclkID = CLK_UART6,
    .irqNum = UART6_IRQn,
    .isAutoFlow = true,
    .runtimeID = PM_RUNTIME_ID_UART6,
};

const struct HAL_UART_DEV g_uart7Dev =
{
    .pReg = UART7,
    .sclkID = CLK_UART7,
    .irqNum = UART7_IRQn,
    .isAutoFlow = true,
    .runtimeID = PM_RUNTIME_ID_UART7,
};

const struct HAL_UART_DEV g_uart8Dev =
{
    .pReg = UART8,
    .sclkID = CLK_UART8,
    .irqNum = UART8_IRQn,
    .isAutoFlow = true,
    .runtimeID = PM_RUNTIME_ID_UART8,
};

const struct HAL_UART_DEV g_uart9Dev =
{
    .pReg = UART9,
    .sclkID = CLK_UART9,
    .irqNum = UART9_IRQn,
    .isAutoFlow = true,
    .runtimeID = PM_RUNTIME_ID_UART9,
};
#endif

#ifdef HAL_VAD_MODULE_ENABLED
/* VAD_CONTROL[3:1]: voice source mapping */
const struct AUDIO_SRC_ADDR_MAP g_audioSrcAddrMaps[] =
{
    { 0, PDM0_BASE + 0x400 },
    { /* sentinel */ }
};

struct HAL_VAD_DEV g_vadDev =
{
    .pReg = VAD,
    .hclk = HCLK_VAD_GATE,
    .pd = PD_AUDIO,
    .irq = VAD_IRQn,
};
#endif

void BSP_Init(void)
{
}

void BSP_SetLoaderFlag(void)
{
}
