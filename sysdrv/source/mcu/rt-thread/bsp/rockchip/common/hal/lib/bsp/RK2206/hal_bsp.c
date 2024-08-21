/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_bsp.h"

#ifdef HAL_ACDCDIG_MODULE_ENABLED
struct HAL_ACDCDIG_DEV g_acdcDigDev =
{
    .pReg = ACDCDIG,
    .hclk = PCLK_ACODEC_GATE,
    .bclkFs = 64,
    .clkI2cCodec = CLK_I2C_CODEC,
};
#endif

#ifdef HAL_AUDIOPWM_MODULE_ENABLED
struct HAL_AUDIOPWM_DEV g_audioPwmDev =
{
    .pReg = AUDIOPWM,
    .mclk = CLK_AUDIOPWM,
    .mclkGate = HCLK_AUDIOPWM_GATE,
    .hclk = HCLK_AUDIOPWM_GATE,
    .txDmaData =
    {
        .addr = (uint32_t)&(AUDIOPWM->FIFO_ENTRY),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_AUDIOPWM,
        .dmac = DMA,
    },
};
#endif

#ifdef HAL_DWDMA_MODULE_ENABLED
struct HAL_DWDMA_DEV g_dwDmaDev =
{
    .pReg = DMA,
    .irq[0] = DMAC_CH0_IRQn,
    .irq[1] = DMAC_CH1_IRQn,
    .irq[2] = DMAC_CH2_IRQn,
    .irq[3] = DMAC_CH3_IRQn,
    .irq[4] = DMAC_CH4_IRQn,
    .irq[5] = DMAC_CH5_IRQn,
    .maxChans = DMA_NUM_CHANNELS,
    .dataWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
    .blockSize = 0xfff, /** ref to trm */
};
#endif

#ifdef HAL_HYPERPSRAM_MODULE_ENABLED
HAL_SECTION_SRAM_DATA
struct HAL_HYPERPSRAM_DEV g_hyperpsramDev =
{
    .pReg = HYPERBUS,
    .clkID = CLK_XIP_HYPERX8,
    .aclkGateID = ACLK_XIP_HYPERX8_GATE,
    .sclkGateID = CLK_XIP_HYPERX8_DT50_GATE,
    .hyperMem[0] = XIP_MAP1_BASE0,
    .hyperMaxFreq = 300000000,
    .psramFreq = PLL_INPUT_OSC_RATE / 2,
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
};

const struct HAL_I2C_DEV g_i2c1Dev =
{
    .pReg = I2C1,
    .clkID = CLK_I2C1,
    .clkGateID = CLK_I2C1_PLL_GATE,
    .pclkGateID = PCLK_I2C1_GATE,
    .irqNum = I2C1_IRQn,
};

const struct HAL_I2C_DEV g_i2c2Dev =
{
    .pReg = I2C2,
    .clkID = CLK_I2C2,
    .clkGateID = CLK_I2C2_PLL_GATE,
    .pclkGateID = PCLK_I2C2_GATE,
    .irqNum = I2C2_IRQn,
};
#endif

#ifdef HAL_PDM_MODULE_ENABLED
struct HAL_PDM_DEV g_pdm0Dev =
{
    .pReg = PDM0,
    .mclk = MCLK_PDM,
#ifdef IS_FPGA
    .mclkRate = PDM_FPGA_CLK_RATE,
#else
    .mclkRate = PDM_CLK_RATE,
#endif
    .mclkGate = MCLK_PDM_PLL_GATE,
    .hclk = HCLK_PDM_GATE,
    .reset = SRST_M_PDM,
    .rxDmaData =
    {
        .addr = (uint32_t)&(PDM0->RXFIFO_DATA_REG),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_PDM,
        .dmac = DMA,
    },
};
#endif

#if defined(HAL_PINCTRL_MODULE_ENABLED)
static struct PINCTRL_BANK_INFO pinBanks[] = {
    PIN_BANK_CFG_FLAGS(GPIO_BANK0, 32, GRF_BASE,
                       0x0000, 4, 4,
                       0x0100, 2, 8,
                       0x0000, 0, 0,
                       0x0000, 0, 0,
                       0x0000, 0, 0),
    PIN_BANK_CFG_FLAGS(GPIO_BANK1, 32, GRF_BASE,
                       0x0020, 4, 4,
                       0x0110, 2, 8,
                       0x0000, 0, 0,
                       0x0000, 0, 0,
                       0x0000, 0, 0),
};

static const struct PINCTRL_MUX_ROUTE_DATA muxRouteData[] = {
    {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_B6 | GPIO_PIN_B7,
        .func = 2, .routeReg = 0x240, .routeVal = ROUTE_VAL(0, 0, 0x1), //UART0_M0
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_C6 | GPIO_PIN_C7,
        .func = 3, .routeReg = 0x240, .routeVal = ROUTE_VAL(1, 0, 0x1), //UART0_M1
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_C2 | GPIO_PIN_C3,
        .func = 3, .routeReg = 0x240, .routeVal = ROUTE_VAL(0, 1, 0x3), //UART1_M0
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_A4 | GPIO_PIN_A5 | GPIO_PIN_A6 | GPIO_PIN_A7,
        .func = 3, .routeReg = 0x240, .routeVal = ROUTE_VAL(1, 1, 0x3), //UART1_M1
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_D0 | GPIO_PIN_D1,
        .func = 4, .routeReg = 0x240, .routeVal = ROUTE_VAL(2, 1, 0x3), //UART1_M2
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_D2 | GPIO_PIN_D3 | GPIO_PIN_D4 | GPIO_PIN_D5,
        .func = 4, .routeReg = 0x240, .routeVal = ROUTE_VAL(0, 3, 0x1), //UART2_M0
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_B0 | GPIO_PIN_B1 | GPIO_PIN_B2 | GPIO_PIN_B3,
        .func = 3, .routeReg = 0x240, .routeVal = ROUTE_VAL(1, 3, 0x1), //UART2_M1
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_C0,
        .func = 5, .routeReg = 0x240, .routeVal = ROUTE_VAL(0, 4, 0x1), //I2S_M0
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_C1 | GPIO_PIN_C2 | GPIO_PIN_C3 | GPIO_PIN_C4 | GPIO_PIN_C5 | GPIO_PIN_C6 | GPIO_PIN_C7,
        .func = 6, .routeReg = 0x240, .routeVal = ROUTE_VAL(0, 4, 0x1), //I2S_M0
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_D2 | GPIO_PIN_D3 | GPIO_PIN_D4 | GPIO_PIN_D5 | GPIO_PIN_D6,
        .func = 6, .routeReg = 0x240, .routeVal = ROUTE_VAL(1, 4, 0x1), //I2S_M1
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_B4,
        .func = 5, .routeReg = 0x240, .routeVal = ROUTE_VAL(0, 5, 0x1), //TKEY0_M0
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_D0,
        .func = 3, .routeReg = 0x240, .routeVal = ROUTE_VAL(1, 5, 0x1), //TKEY0_M1
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_B5,
        .func = 5, .routeReg = 0x240, .routeVal = ROUTE_VAL(0, 6, 0x1), //TKEY1_M0
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_D1,
        .func = 3, .routeReg = 0x240, .routeVal = ROUTE_VAL(1, 6, 0x1), //TKEY1_M1
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_B6,
        .func = 5, .routeReg = 0x240, .routeVal = ROUTE_VAL(0, 7, 0x1), //TKEY2_M0
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_D2,
        .func = 3, .routeReg = 0x240, .routeVal = ROUTE_VAL(1, 7, 0x1), //TKEY2_M1
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_B7,
        .func = 5, .routeReg = 0x240, .routeVal = ROUTE_VAL(0, 8, 0x1), //TKEY3_M0
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_D3,
        .func = 3, .routeReg = 0x240, .routeVal = ROUTE_VAL(1, 8, 0x1), //TKEY3_M1
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_C3,
        .func = 7, .routeReg = 0x240, .routeVal = ROUTE_VAL(0, 9, 0x1), //TKEY4_M0
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_D4,
        .func = 3, .routeReg = 0x240, .routeVal = ROUTE_VAL(1, 9, 0x1), //TKEY4_M1
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_C4,
        .func = 7, .routeReg = 0x240, .routeVal = ROUTE_VAL(0, 10, 0x1), //TKEY5_M0
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_D5,
        .func = 3, .routeReg = 0x240, .routeVal = ROUTE_VAL(1, 10, 0x1), //TKEY5_M1
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_C5,
        .func = 7, .routeReg = 0x240, .routeVal = ROUTE_VAL(0, 11, 0x1), //TKEY6_M0
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_D6,
        .func = 3, .routeReg = 0x240, .routeVal = ROUTE_VAL(1, 11, 0x1), //TKEY6_M1
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_D0 | GPIO_PIN_D1 | GPIO_PIN_D2 | GPIO_PIN_D3,
        .func = 1, .routeReg = 0x240, .routeVal = ROUTE_VAL(0, 13, 0x1), //CODEC_M0
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_A4 | GPIO_PIN_A5 | GPIO_PIN_A6 | GPIO_PIN_A7,
        .func = 7, .routeReg = 0x240, .routeVal = ROUTE_VAL(1, 13, 0x1), //CODEC_M1
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_B7,
        .func = 3, .routeReg = 0x204, .routeVal = ROUTE_VAL(0, 1, 0x1), //SPI0_MISO_M0
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_C3,
        .func = 4, .routeReg = 0x204, .routeVal = ROUTE_VAL(1, 1, 0x1), //SPI0_MISO_M1
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_C7,
        .func = 4, .routeReg = 0x204, .routeVal = ROUTE_VAL(0, 2, 0x3), //SPI1_MISO_M0
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_B3,
        .func = 4, .routeReg = 0x204, .routeVal = ROUTE_VAL(1, 2, 0x3), //SPI1_MISO_M1
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_D4,
        .func = 5, .routeReg = 0x204, .routeVal = ROUTE_VAL(2, 2, 0x3), //SPI1_MISO_M2
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_C0,
        .func = 2, .routeReg = 0x204, .routeVal = ROUTE_VAL(0, 4, 0x1), //PWM0_M0
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_B4,
        .func = 1, .routeReg = 0x204, .routeVal = ROUTE_VAL(1, 4, 0x1), //PWM0_M1
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_C1,
        .func = 2, .routeReg = 0x204, .routeVal = ROUTE_VAL(0, 5, 0x1), //PWM1_M0
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_B5,
        .func = 1, .routeReg = 0x204, .routeVal = ROUTE_VAL(1, 5, 0x1), //PWM1_M1
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_B4 | GPIO_PIN_B5,
        .func = 4, .routeReg = 0x204, .routeVal = ROUTE_VAL(0, 6, 0x3), //I2C0_M0
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_C6 | GPIO_PIN_C7,
        .func = 5, .routeReg = 0x204, .routeVal = ROUTE_VAL(1, 6, 0x3), //I2C0_M1
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_A0 | GPIO_PIN_A1,
        .func = 3, .routeReg = 0x204, .routeVal = ROUTE_VAL(2, 6, 0x3), //I2C0_M2
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_D0 | GPIO_PIN_D1,
        .func = 6, .routeReg = 0x204, .routeVal = ROUTE_VAL(3, 6, 0x3), //I2C0_M3
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_B6 | GPIO_PIN_B7,
        .func = 4, .routeReg = 0x204, .routeVal = ROUTE_VAL(0, 8, 0x3), //I2C1_M0
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_C1 | GPIO_PIN_C2,
        .func = 5, .routeReg = 0x204, .routeVal = ROUTE_VAL(1, 8, 0x3), //I2C1_M1
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_A2 | GPIO_PIN_A3,
        .func = 3, .routeReg = 0x204, .routeVal = ROUTE_VAL(2, 8, 0x3), //I2C1_M2
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_D5 | GPIO_PIN_D6,
        .func = 5, .routeReg = 0x204, .routeVal = ROUTE_VAL(3, 8, 0x3), //I2C1_M3
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_A5 | GPIO_PIN_A6 | GPIO_PIN_A7,
        .func = 4, .routeReg = 0x204, .routeVal = ROUTE_VAL(0, 10, 0x1), //PDM_M0
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_C3 | GPIO_PIN_C4 | GPIO_PIN_C5,
        .func = 5, .routeReg = 0x204, .routeVal = ROUTE_VAL(1, 10, 0x1), //PDM_M1
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_D5 | GPIO_PIN_D6,
        .func = 1, .routeReg = 0x204, .routeVal = ROUTE_VAL(0, 12, 0x1), //I2C2_M0
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_B4 | GPIO_PIN_B5,
        .func = 8, .routeReg = 0x204, .routeVal = ROUTE_VAL(1, 12, 0x1), //I2C2_M1
    },
};

const struct HAL_PINCTRL_DEV g_pinDev = {
    .banks = pinBanks,
    .banksNum = HAL_ARRAY_SIZE(pinBanks),
    .muxRouteData = muxRouteData,
    .muxRouteDataNum = HAL_ARRAY_SIZE(muxRouteData),
};
#endif

#ifdef HAL_PWM_MODULE_ENABLED
const struct HAL_PWM_DEV g_pwm0Dev =
{
    .pReg = PWM0,
    .clkID = CLK_PWM0,
    .clkGateID = CLK_PWM0_PLL_GATE,
    .pclkGateID = PCLK_PWM0_GATE,
    .irqNum = PWM_4CH_0_IRQn,
};

const struct HAL_PWM_DEV g_pwm1Dev =
{
    .pReg = PWM1,
    .clkID = CLK_PWM1,
    .clkGateID = CLK_PWM1_PLL_GATE,
    .pclkGateID = PCLK_PWM1_GATE,
    .irqNum = PWM_4CH_1_IRQn,
};

const struct HAL_PWM_DEV g_pwm2Dev =
{
    .pReg = PWM2,
    .clkID = CLK_PWM2,
    .clkGateID = CLK_PWM2_PLL_GATE,
    .pclkGateID = PCLK_PWM2_GATE,
    .irqNum = PWM_4CH_2_IRQn,
};
#endif

#ifdef HAL_UART_MODULE_ENABLED
const struct HAL_UART_DEV g_uart0Dev =
{
    .pReg = UART0,
    .sclkID = CLK_UART0,
    .sclkGateID = SCLK_UART0_GATE,
    .pclkGateID = PCLK_UART0_GATE,
    .irqNum = UART0_IRQn,
    .isAutoFlow = true,
    .dmaTxReqNum = DMA_REQ_UART0_TX,
    .dmaRxReqNum = DMA_REQ_UART0_RX,
    .dmac = DMA,
};

const struct HAL_UART_DEV g_uart1Dev =
{
    .pReg = UART1,
    .sclkID = CLK_UART1,
    .sclkGateID = SCLK_UART1_GATE,
    .pclkGateID = PCLK_UART1_GATE,
    .irqNum = UART1_IRQn,
    .isAutoFlow = true,
    .dmaTxReqNum = DMA_REQ_UART1_TX,
    .dmaRxReqNum = DMA_REQ_UART1_RX,
    .dmac = DMA,
};

const struct HAL_UART_DEV g_uart2Dev =
{
    .pReg = UART2,
    .sclkID = CLK_UART2,
    .sclkGateID = SCLK_UART2_GATE,
    .pclkGateID = PCLK_UART2_GATE,
    .irqNum = UART2_IRQn,
    .isAutoFlow = false,
    .dmaTxReqNum = DMA_REQ_UART0_TX,     /* share num with uart0 */
    .dmaRxReqNum = DMA_REQ_UART0_RX,     /* share num with uart0 */
    .dmac = DMA,
};
#endif

#ifdef HAL_I2STDM_MODULE_ENABLED
struct HAL_I2STDM_DEV g_i2sTdm0Dev =
{
    .pReg = I2STDM0,
    .mclkOut = MCLKOUT_I2S8CH_0,
    .mclkTx = MCLK_I2S8CH_0_TX,
    .mclkTxGate = CLK_I2S8CH_0_TX_PLL_GATE,
    .mclkRx = MCLK_I2S8CH_0_RX,
    .mclkRxGate = CLK_I2S8CH_0_RX_PLL_GATE,
    .hclk = HCLK_I2S0_GATE,
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

struct HAL_I2STDM_DEV g_i2sTdm1Dev =
{
    .pReg = I2STDM1,
    .mclkTx = MCLK_I2S8CH_1_TX,
    .mclkTxGate = CLK_I2S8CH_1_TX_PLL_GATE,
    .mclkRx = MCLK_I2S8CH_1_RX,
    .mclkRxGate = CLK_I2S8CH_1_RX_PLL_GATE,
    .hclk = HCLK_I2S1_GATE,
    .bclkFs = 64,
    .rxDmaData =
    {
        .addr = (uint32_t)&(I2STDM1->RXDR),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_I2S1_RX,
        .dmac = DMA,
    },
    .txDmaData =
    {
        .addr = (uint32_t)&(I2STDM1->TXDR),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_I2S1_TX,
        .dmac = DMA,
    },
};
#endif

#ifdef HAL_TSADC_MODULE_ENABLED
const struct HAL_TSADC_DEV g_tsadcDev =
{
    .sclkID = CLK_TSADC,
    .sclkGateID = CLK_TSADC_GATE,
    .pclkGateID = PCLK_TSADC_GATE,
    .sclkResetID = SRST_TSADC,
    .pclkResetID = SRST_P_TSADC,
    .speed = 650000,
    .polarity = TSHUT_LOW_ACTIVE,
    .mode = TSHUT_MODE_CRU,
};
#endif

#ifdef HAL_FSPI_MODULE_ENABLED
struct HAL_FSPI_HOST g_fspi0Dev =
{
    .instance = FSPI0,
    .sclkGate = CLK_XIP_SFC0_DT50_GATE,
    .hclkGate = HCLK_SFC0_GATE,
    .xipClkGate = HCLK_SFC0_XIP_GATE,
    .sclkID = CLK_XIP_SFC0,
    .irqNum = FSPI0_IRQn,
    .xipMemCode = XIP_MAP0_BASE0,
    .xipMemData = XIP_MAP0_BASE1,
    .xmmcDev[0] =
    {
        .type = DEV_NOR,
    },
};
struct HAL_FSPI_HOST g_fspi1Dev =
{
    .instance = FSPI1,
    .sclkGate = CLK_XIP_SFC1_DT50_GATE,
    .hclkGate = HCLK_SFC1_GATE,
    .xipClkGate = HCLK_SFC1_XIP_GATE,
    .sclkID = CLK_XIP_SFC1,
    .irqNum = FSPI1_IRQn,
    .xipMemData = XIP_MAP1_BASE1,
    .xmmcDev[0] =
    {
        .type = DEV_PSRAM,
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
        .pclkGateID = PCLK_PVTM_GATE,
        .con0 = &(PVTM->CON[0]),
        .con1 = &(PVTM->CON[1]),
        .sta0 = &(PVTM->STATUS[0]),
        .sta1 = &(PVTM->STATUS[1]),
        .startMask = PVTM_CON0_PVTM_START_MASK,
        .startShift = PVTM_CON0_PVTM_START_SHIFT,
        .enMask = PVTM_CON0_PVTM_OSC_EN_MASK,
        .enShift = PVTM_CON0_PVTM_OSC_EN_SHIFT,
        .selMask = PVTM_CON0_PVTM_OSC_SEL_MASK,
        .selShift = PVTM_CON0_PVTM_OSC_SEL_SHIFT,
        .doneMask = PVTM_STATUS0_PVTM_FREQ_DONE_MASK,
        .doneShift = PVTM_STATUS0_PVTM_FREQ_DONE_SHIFT,
    },
};

const struct HAL_PVTM_DEV g_pvtmDev =
{
    .info = pvtmInfo,
    .num = HAL_ARRAY_SIZE(pvtmInfo),
};
#endif

#if defined(HAL_PCD_MODULE_ENABLED) || defined(HAL_HCD_MODULE_ENABLED)
const struct HAL_USB_DEV g_usbdDev =
{
    .pReg = USB,
    .hclkGateID = HCLK_USBOTG_GATE,
    .utmiclkGateID = HCLK_USBOTG_PMU_GATE,
    .usbPhyGateID = CLK_OTG_USBPHY_PLL_GATE,
    .irqNum = USB2OTG_IRQn,
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

#ifdef HAL_VAD_MODULE_ENABLED
/* VAD_CONTROL[3:1]: voice source mapping */
const struct AUDIO_SRC_ADDR_MAP g_audioSrcAddrMaps[] =
{
    { 0, I2STDM0_BASE + 0x800 },
    { 1, I2STDM1_BASE + 0x800 },
    { 2, PDM0_BASE + 0x400 },
    { /* sentinel */ }
};

struct HAL_VAD_DEV g_vadDev =
{
    .pReg = VAD,
    .hclk = HCLK_VAD_GATE,
    .irq = VAD_IRQn,
};
#endif

#ifdef HAL_SPI_MODULE_ENABLED
const struct HAL_SPI_DEV g_spi0Dev = {
    .base = SPI0_BASE,
    .clkId = CLK_SPI0,
    .clkGateID = CLK_SPI0_PLL_GATE,
    .pclkGateID = PCLK_SPI0_GATE,
    .irqNum = SPIMST0_IRQn,
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
    /* Enable bootrom/ahbbuffer/intmem0/intmem1/intmem1_vad memory auto gating */
    WRITE_REG_MASK_WE(GRF->SOC_CON2,
                      GRF_SOC_CON2_BOOTROM_MEMAUTO_GATING_EN_MASK |
                      GRF_SOC_CON2_AHBBUFFER_MEMAUTO_GATING_EN_MASK |
                      GRF_SOC_CON2_INTMEM0_MEMAUTO_GATING_EN_MASK |
                      GRF_SOC_CON2_INTMEM1_MEMAUTO_GATING_EN_MASK |
                      GRF_SOC_CON2_INTMEM1_VAD_MEMAUTO_GATING_EN_MASK,
                      0 << GRF_SOC_CON2_BOOTROM_MEMAUTO_GATING_EN_SHIFT |
                      0 << GRF_SOC_CON2_AHBBUFFER_MEMAUTO_GATING_EN_SHIFT |
                      0 << GRF_SOC_CON2_INTMEM0_MEMAUTO_GATING_EN_SHIFT |
                      0 << GRF_SOC_CON2_INTMEM1_MEMAUTO_GATING_EN_SHIFT |
                      0 << GRF_SOC_CON2_INTMEM1_VAD_MEMAUTO_GATING_EN_SHIFT);
}
